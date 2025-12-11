/**
 * @file play_mode_controller.cpp
 * @brief Play Mode Controller implementation
 */

#include "NovelMind/editor/play_mode_controller.hpp"
#include "NovelMind/editor/imgui_integration.hpp"
#include <algorithm>
#include <sstream>

namespace NovelMind::editor {

// Singleton instance
std::unique_ptr<PlayModeController> PlayModeController::s_instance;

PlayModeController::PlayModeController() = default;
PlayModeController::~PlayModeController() = default;

PlayModeController& PlayModeController::instance()
{
    if (!s_instance)
    {
        s_instance = std::make_unique<PlayModeController>();
    }
    return *s_instance;
}

void PlayModeController::initialize(EditorApp* app)
{
    m_app = app;

    // Set default configuration
    m_defaultConfig.startFromCurrentScene = true;
    m_defaultConfig.showDebugOverlay = true;
    m_defaultConfig.pauseOnError = true;
    m_defaultConfig.enableBreakpoints = true;
    m_defaultConfig.timeScale = 1.0;

    m_config = m_defaultConfig;
}

void PlayModeController::shutdown()
{
    // Make sure we exit play mode cleanly
    if (isInPlayMode())
    {
        stop();
    }

    m_listeners.clear();
    m_breakpoints.clear();
    m_variables.clear();
    m_app = nullptr;
}

// =========================================================================
// Play Controls
// =========================================================================

void PlayModeController::play()
{
    play(m_defaultConfig);
}

void PlayModeController::play(const PlayModeConfig& config)
{
    if (m_state != PlayModeState::Stopped)
    {
        return;
    }

    m_config = config;
    enterPlayMode();
}

void PlayModeController::pause()
{
    if (m_state != PlayModeState::Playing)
    {
        return;
    }

    PlayModeState oldState = m_state;
    m_state = PlayModeState::Paused;

    // Pause timeline playback
    TimelinePlaybackEngine::instance().pause();

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModePaused();
    }

    // Publish event
    PlayModeStateChangedEvent event;
    event.previousState = oldState;
    event.newState = m_state;
    EventBus::instance().publish(event);
}

void PlayModeController::resume()
{
    if (m_state != PlayModeState::Paused)
    {
        return;
    }

    // Check if at breakpoint
    if (m_atBreakpoint)
    {
        // Need to call continueExecution() instead
        return;
    }

    PlayModeState oldState = m_state;
    m_state = PlayModeState::Playing;

    // Resume timeline playback
    TimelinePlaybackEngine::instance().resume();

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModeResumed();
    }

    // Publish event
    PlayModeStateChangedEvent event;
    event.previousState = oldState;
    event.newState = m_state;
    EventBus::instance().publish(event);
}

void PlayModeController::stop()
{
    if (m_state == PlayModeState::Stopped)
    {
        return;
    }

    exitPlayMode();
}

void PlayModeController::togglePlayPause()
{
    switch (m_state)
    {
        case PlayModeState::Stopped:
            play();
            break;
        case PlayModeState::Playing:
            pause();
            break;
        case PlayModeState::Paused:
            if (m_atBreakpoint)
            {
                continueExecution();
            }
            else
            {
                resume();
            }
            break;
        default:
            break;
    }
}

void PlayModeController::restart()
{
    if (m_state == PlayModeState::Stopped)
    {
        play();
        return;
    }

    // Reset timeline to start
    TimelinePlaybackEngine::instance().jumpToStart();

    // Reset stats
    resetStats();

    // Reset variables
    m_variables.clear();

    // Clear breakpoint state
    m_atBreakpoint = false;
    m_currentBreakpointId.clear();

    // If paused, resume
    if (m_state == PlayModeState::Paused)
    {
        resume();
    }
}

// =========================================================================
// State Queries
// =========================================================================

bool PlayModeController::isInPlayMode() const
{
    return m_state == PlayModeState::Playing ||
           m_state == PlayModeState::Paused ||
           m_state == PlayModeState::Starting ||
           m_state == PlayModeState::Stopping;
}

// =========================================================================
// Configuration
// =========================================================================

void PlayModeController::setDefaultConfig(const PlayModeConfig& config)
{
    m_defaultConfig = config;
}

void PlayModeController::setTimeScale(f64 scale)
{
    m_config.timeScale = std::max(0.0, std::min(scale, 10.0)); // Clamp 0-10x

    if (isInPlayMode())
    {
        TimelinePlaybackEngine::instance().setSpeed(m_config.timeScale);
    }
}

// =========================================================================
// Debug Features
// =========================================================================

void PlayModeController::stepNext()
{
    if (m_state != PlayModeState::Paused)
    {
        return;
    }

    // Execute one step then pause again
    // Implementation would depend on story graph execution
    // For now, step one frame
    update(1.0 / 60.0); // One frame at 60fps

    // Stay paused after step
}

void PlayModeController::stepInto()
{
    if (m_state != PlayModeState::Paused)
    {
        return;
    }

    // Step into current node (for nested nodes/branches)
    // Implementation depends on story graph structure
    stepNext();
}

void PlayModeController::stepOut()
{
    if (m_state != PlayModeState::Paused)
    {
        return;
    }

    // Step out of current node
    // Implementation depends on story graph structure
    stepNext();
}

void PlayModeController::addBreakpoint(const std::string& nodeId, const std::string& condition)
{
    // Check if breakpoint already exists for this node
    auto existing = getBreakpointForNode(nodeId);
    if (existing.has_value())
    {
        return;
    }

    Breakpoint bp;
    bp.id = generateBreakpointId();
    bp.nodeId = nodeId;
    bp.condition = condition;
    bp.enabled = true;

    m_breakpoints.push_back(bp);
}

void PlayModeController::removeBreakpoint(const std::string& breakpointId)
{
    auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(),
        [&breakpointId](const Breakpoint& bp) { return bp.id == breakpointId; });

    if (it != m_breakpoints.end())
    {
        m_breakpoints.erase(it);
    }
}

void PlayModeController::toggleBreakpoint(const std::string& nodeId)
{
    auto existing = getBreakpointForNode(nodeId);
    if (existing.has_value())
    {
        removeBreakpoint(existing->id);
    }
    else
    {
        addBreakpoint(nodeId);
    }
}

void PlayModeController::setBreakpointEnabled(const std::string& breakpointId, bool enabled)
{
    auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(),
        [&breakpointId](const Breakpoint& bp) { return bp.id == breakpointId; });

    if (it != m_breakpoints.end())
    {
        it->enabled = enabled;
    }
}

std::vector<Breakpoint> PlayModeController::getBreakpoints() const
{
    return m_breakpoints;
}

std::optional<Breakpoint> PlayModeController::getBreakpointForNode(const std::string& nodeId) const
{
    auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(),
        [&nodeId](const Breakpoint& bp) { return bp.nodeId == nodeId; });

    if (it != m_breakpoints.end())
    {
        return *it;
    }
    return std::nullopt;
}

void PlayModeController::clearAllBreakpoints()
{
    m_breakpoints.clear();
}

void PlayModeController::continueExecution()
{
    if (!m_atBreakpoint)
    {
        return;
    }

    m_atBreakpoint = false;
    m_currentBreakpointId.clear();

    // Resume execution
    if (m_state == PlayModeState::Paused)
    {
        PlayModeState oldState = m_state;
        m_state = PlayModeState::Playing;

        TimelinePlaybackEngine::instance().resume();

        notifyStateChanged(oldState, m_state);

        for (auto* listener : m_listeners)
        {
            listener->onPlayModeResumed();
        }
    }
}

// =========================================================================
// Scene Control
// =========================================================================

void PlayModeController::jumpToScene(const std::string& sceneId)
{
    if (!isInPlayMode())
    {
        return;
    }

    m_currentSceneId = sceneId;
    // Would trigger actual scene change in runtime
}

void PlayModeController::jumpToNode(const std::string& nodeId)
{
    if (!isInPlayMode())
    {
        return;
    }

    m_currentNodeId = nodeId;
    // Would trigger actual node jump in runtime
}

void PlayModeController::setVariable(const std::string& name, const std::string& value)
{
    m_variables[name] = value;
    // Would sync with runtime variable system
}

std::string PlayModeController::getVariable(const std::string& name) const
{
    auto it = m_variables.find(name);
    return it != m_variables.end() ? it->second : "";
}

std::unordered_map<std::string, std::string> PlayModeController::getAllVariables() const
{
    return m_variables;
}

// =========================================================================
// Update
// =========================================================================

void PlayModeController::update(f64 deltaTime)
{
    if (m_state != PlayModeState::Playing)
    {
        return;
    }

    // Apply time scale
    f64 scaledDelta = deltaTime * m_config.timeScale;

    // Update statistics
    updateStats(scaledDelta);

    // Update timeline playback
    TimelinePlaybackEngine::instance().update(scaledDelta);

    // Check breakpoints (would check if current node matches any breakpoint)
    if (m_config.enableBreakpoints && !m_currentNodeId.empty())
    {
        auto bp = getBreakpointForNode(m_currentNodeId);
        if (bp.has_value() && bp->enabled)
        {
            // Hit breakpoint
            m_atBreakpoint = true;
            m_currentBreakpointId = bp->id;

            // Update hit count
            auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(),
                [&bp](const Breakpoint& b) { return b.id == bp->id; });
            if (it != m_breakpoints.end())
            {
                it->hitCount++;

                // Remove if hit once
                if (it->hitOnce)
                {
                    m_breakpoints.erase(it);
                }
            }

            // Pause
            pause();

            // Notify
            notifyBreakpointHit(*bp);
        }
    }
}

// =========================================================================
// Event Handling
// =========================================================================

void PlayModeController::addListener(IPlayModeListener* listener)
{
    if (std::find(m_listeners.begin(), m_listeners.end(), listener) == m_listeners.end())
    {
        m_listeners.push_back(listener);
    }
}

void PlayModeController::removeListener(IPlayModeListener* listener)
{
    auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end())
    {
        m_listeners.erase(it);
    }
}

// =========================================================================
// Toolbar Integration
// =========================================================================

void PlayModeController::renderToolbar()
{
    // Play/Pause button
    if (m_state == PlayModeState::Playing)
    {
        if (widgets::ToolbarButton("||", "Pause (Ctrl+P)", false))
        {
            pause();
        }
    }
    else
    {
        bool enabled = (m_state == PlayModeState::Stopped || m_state == PlayModeState::Paused);
        if (widgets::ToolbarButton(">", "Play (Ctrl+P)", false) && enabled)
        {
            if (m_state == PlayModeState::Paused)
            {
                resume();
            }
            else
            {
                play();
            }
        }
    }

    // Stop button
    {
        bool enabled = isInPlayMode();
        if (widgets::ToolbarButton("[]", "Stop (Ctrl+Shift+P)", false) && enabled)
        {
            stop();
        }
    }

    widgets::ToolbarSeparator();

    // Step controls (only when paused)
    if (m_state == PlayModeState::Paused)
    {
        if (widgets::ToolbarButton("|>", "Step Next (F10)", false))
        {
            stepNext();
        }

        if (widgets::ToolbarButton("|>>", "Step Into (F11)", false))
        {
            stepInto();
        }

        if (widgets::ToolbarButton("<<|", "Step Out (Shift+F11)", false))
        {
            stepOut();
        }

        widgets::ToolbarSeparator();
    }

    // Time scale
    if (isInPlayMode())
    {
        std::vector<std::string> speeds = {"0.25x", "0.5x", "1x", "2x", "4x"};
        static i32 speedIndex = 2; // Default 1x

        // Would render a dropdown for speed selection
        // For now, just show current time scale
        // widgets::Text(std::to_string(m_config.timeScale) + "x");
        (void)speeds;
        (void)speedIndex;
    }

    // Breakpoint indicator
    if (m_atBreakpoint)
    {
        // Would show breakpoint indicator
        // widgets::Text("Breakpoint Hit");
    }
}

void PlayModeController::renderDebugOverlay()
{
    if (!isInPlayMode() || !m_config.showDebugOverlay)
    {
        return;
    }

    // Render debug overlay in scene view
    // Would use ImGui overlay window

    // Show play time
    // Show current node
    // Show FPS
    // Show variables

    // Example structure (ImGui calls would be here):
    // ImGui::Begin("Play Mode Debug", nullptr, ImGuiWindowFlags_NoDecoration);
    // ImGui::Text("Time: %.2f", m_stats.totalPlayTime);
    // ImGui::Text("FPS: %.1f", m_stats.averageFps);
    // ImGui::Text("Node: %s", m_currentNodeId.c_str());
    // ImGui::End();
}

// =========================================================================
// Private Methods
// =========================================================================

void PlayModeController::enterPlayMode()
{
    PlayModeState oldState = m_state;
    m_state = PlayModeState::Starting;

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModeStarting();
    }

    // Capture current scene state
    captureSceneSnapshot();

    // Reset stats
    resetStats();

    // Configure timeline playback
    TimelinePlaybackEngine::instance().setSpeed(m_config.timeScale);
    if (!m_config.startFromCurrentScene)
    {
        TimelinePlaybackEngine::instance().jumpToStart();
    }

    // Transition to playing
    oldState = m_state;
    m_state = PlayModeState::Playing;

    // Start timeline playback
    TimelinePlaybackEngine::instance().play();

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModeStarted();
    }

    // Publish event
    PlayModeStateChangedEvent event;
    event.previousState = oldState;
    event.newState = m_state;
    EventBus::instance().publish(event);
}

void PlayModeController::exitPlayMode()
{
    PlayModeState oldState = m_state;
    m_state = PlayModeState::Stopping;

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModeStopping();
    }

    // Stop timeline playback
    TimelinePlaybackEngine::instance().stop();

    // Restore scene snapshot
    restoreSceneSnapshot();

    // Clear runtime state
    m_variables.clear();
    m_atBreakpoint = false;
    m_currentBreakpointId.clear();
    m_currentNodeId.clear();
    m_currentSceneId.clear();

    // Transition to stopped
    oldState = m_state;
    m_state = PlayModeState::Stopped;

    notifyStateChanged(oldState, m_state);

    for (auto* listener : m_listeners)
    {
        listener->onPlayModeStopped();
    }

    // Publish event
    PlayModeStateChangedEvent event;
    event.previousState = oldState;
    event.newState = m_state;
    EventBus::instance().publish(event);
}

void PlayModeController::captureSceneSnapshot()
{
    m_sceneSnapshot = std::make_unique<SceneSnapshot>();

    // Capture current scene ID
    m_sceneSnapshot->sceneId = m_currentSceneId;

    // Capture timeline position
    m_sceneSnapshot->timelinePosition = TimelinePlaybackEngine::instance().getCurrentTime();

    // Would serialize actual scene state here
    // m_sceneSnapshot->serializedData = serializeCurrentScene();
}

void PlayModeController::restoreSceneSnapshot()
{
    if (!m_sceneSnapshot)
    {
        return;
    }

    // Restore timeline position
    TimelinePlaybackEngine::instance().seekTo(m_sceneSnapshot->timelinePosition);

    // Would deserialize scene state here
    // deserializeScene(m_sceneSnapshot->serializedData);

    m_sceneSnapshot.reset();
}

void PlayModeController::notifyStateChanged(PlayModeState oldState, PlayModeState newState)
{
    for (auto* listener : m_listeners)
    {
        listener->onPlayModeStateChanged(oldState, newState);
    }
}

void PlayModeController::notifyBreakpointHit(const Breakpoint& breakpoint)
{
    for (auto* listener : m_listeners)
    {
        listener->onBreakpointHit(breakpoint);
    }

    // Publish event
    BreakpointHitEvent event;
    event.breakpointId = breakpoint.id;
    event.nodeId = breakpoint.nodeId;
    event.hitCount = breakpoint.hitCount;
    EventBus::instance().publish(event);
}

void PlayModeController::notifyError(const std::string& error, const std::string& nodeId)
{
    m_stats.scriptErrorCount++;

    for (auto* listener : m_listeners)
    {
        listener->onScriptError(error, nodeId);
    }

    // Publish event
    PlayModeErrorEvent event;
    event.errorMessage = error;
    event.nodeId = nodeId;
    EventBus::instance().publish(event);

    // Pause on error if configured
    if (m_config.pauseOnError && m_state == PlayModeState::Playing)
    {
        pause();
    }
}

void PlayModeController::updateStats(f64 deltaTime)
{
    m_stats.totalPlayTime += deltaTime;
    m_stats.frameCount++;

    // Track frame timing
    m_lastFrameTime = deltaTime;
    m_frameTimeAccumulator += deltaTime;
    m_frameTimeCount++;

    // Update min/max
    if (m_stats.frameCount == 1 || deltaTime < m_stats.minFrameTime)
    {
        m_stats.minFrameTime = deltaTime;
    }
    if (deltaTime > m_stats.maxFrameTime)
    {
        m_stats.maxFrameTime = deltaTime;
    }

    // Update average FPS every second
    if (m_frameTimeAccumulator >= 1.0)
    {
        m_stats.averageFps = static_cast<f64>(m_frameTimeCount) / m_frameTimeAccumulator;
        m_frameTimeAccumulator = 0.0;
        m_frameTimeCount = 0;
    }
}

void PlayModeController::resetStats()
{
    m_stats = PlayModeStats{};
    m_lastFrameTime = 0.0;
    m_frameTimeAccumulator = 0.0;
    m_frameTimeCount = 0;
}

std::string PlayModeController::generateBreakpointId()
{
    std::ostringstream oss;
    oss << "bp_" << m_nextBreakpointId++;
    return oss.str();
}

} // namespace NovelMind::editor
