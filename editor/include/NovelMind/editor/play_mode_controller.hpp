/**
 * @file play_mode_controller.hpp
 * @brief Play Mode Controller for NovelMind Editor
 *
 * Manages play-in-editor functionality:
 * - Scene state capture and restoration
 * - Runtime preview initialization
 * - Play/pause/stop controls
 * - Debug visualization during play
 * - Step-through and breakpoint support
 */

#pragma once

#include "NovelMind/core/types.hpp"
#include "NovelMind/editor/event_bus.hpp"
#include "NovelMind/editor/timeline_playback.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace NovelMind::editor {

// Forward declarations
class EditorApp;

/**
 * @brief Play mode states
 */
enum class PlayModeState : u8
{
    Stopped = 0,     // Normal editor mode
    Starting,        // Transitioning to play mode
    Playing,         // Running in play mode
    Paused,          // Paused in play mode
    Stopping         // Transitioning back to editor mode
};

/**
 * @brief Play mode configuration
 */
struct PlayModeConfig
{
    bool startFromCurrentScene = true;      // Start from current scene vs beginning
    bool maximizeSceneView = false;         // Maximize scene view during play
    bool muteAudio = false;                  // Mute audio during play
    bool showDebugOverlay = true;           // Show debug info overlay
    bool pauseOnError = true;               // Pause when script error occurs
    bool enableBreakpoints = true;          // Enable breakpoint support
    f64 timeScale = 1.0;                    // Time scale factor
    std::string startSceneId;               // Override start scene (empty = current)
    std::vector<std::string> enabledDebugViews; // Debug visualizations to show
};

/**
 * @brief Snapshot of scene state for restoration
 */
struct SceneSnapshot
{
    std::string sceneId;
    std::vector<u8> serializedData;         // Serialized scene state
    f64 timelinePosition = 0.0;
    std::vector<std::string> selectedObjects;
    std::unordered_map<std::string, std::string> variableStates;
};

/**
 * @brief Debug breakpoint
 */
struct Breakpoint
{
    std::string id;
    std::string nodeId;                      // Story node ID
    std::string condition;                   // Optional condition expression
    bool enabled = true;
    bool hitOnce = false;                    // Delete after first hit
    u32 hitCount = 0;
};

/**
 * @brief Play mode statistics
 */
struct PlayModeStats
{
    f64 totalPlayTime = 0.0;
    u32 frameCount = 0;
    f64 averageFps = 0.0;
    f64 minFrameTime = 0.0;
    f64 maxFrameTime = 0.0;
    u32 scriptErrorCount = 0;
    u32 warningCount = 0;
    std::vector<std::string> visitedNodes;
    std::unordered_map<std::string, u32> nodeVisitCounts;
};

/**
 * @brief Event: Play mode state changed
 */
struct PlayModeStateChangedEvent : EditorEvent
{
    PlayModeStateChangedEvent()
        : EditorEvent(EditorEventType::PlayModeStarted) {}

    [[nodiscard]] std::string getDescription() const override {
        return "PlayModeStateChanged";
    }

    PlayModeState previousState = PlayModeState::Stopped;
    PlayModeState newState = PlayModeState::Stopped;
};

/**
 * @brief Event: Breakpoint hit
 */
struct BreakpointHitEvent : EditorEvent
{
    BreakpointHitEvent()
        : EditorEvent(EditorEventType::Custom) {}

    [[nodiscard]] std::string getDescription() const override {
        return "BreakpointHit";
    }

    std::string breakpointId;
    std::string nodeId;
    u32 hitCount = 0;
};

/**
 * @brief Event: Script error during play
 */
struct PlayModeErrorEvent : EditorEvent
{
    PlayModeErrorEvent()
        : EditorEvent(EditorEventType::ErrorOccurred) {}

    [[nodiscard]] std::string getDescription() const override {
        return "PlayModeError";
    }

    std::string errorMessage;
    std::string nodeId;
    std::string scriptName;
    u32 lineNumber = 0;
};

/**
 * @brief Listener interface for play mode events
 */
class IPlayModeListener
{
public:
    virtual ~IPlayModeListener() = default;

    virtual void onPlayModeStateChanged(PlayModeState oldState, PlayModeState newState) {}
    virtual void onPlayModeStarting() {}
    virtual void onPlayModeStarted() {}
    virtual void onPlayModePaused() {}
    virtual void onPlayModeResumed() {}
    virtual void onPlayModeStopping() {}
    virtual void onPlayModeStopped() {}
    virtual void onBreakpointHit(const Breakpoint& breakpoint) {}
    virtual void onScriptError(const std::string& error, const std::string& nodeId) {}
};

/**
 * @brief Play Mode Controller
 *
 * Manages the play-in-editor workflow:
 * 1. Captures editor state before entering play mode
 * 2. Initializes runtime preview
 * 3. Handles play/pause/stop controls
 * 4. Manages debug features (breakpoints, stepping)
 * 5. Restores editor state when exiting play mode
 */
class PlayModeController
{
public:
    PlayModeController();
    ~PlayModeController();

    // Prevent copying
    PlayModeController(const PlayModeController&) = delete;
    PlayModeController& operator=(const PlayModeController&) = delete;

    /**
     * @brief Get singleton instance
     */
    static PlayModeController& instance();

    /**
     * @brief Initialize with editor app
     */
    void initialize(EditorApp* app);

    /**
     * @brief Shutdown
     */
    void shutdown();

    // =========================================================================
    // Play Controls
    // =========================================================================

    /**
     * @brief Enter play mode
     */
    void play();

    /**
     * @brief Enter play mode with configuration
     */
    void play(const PlayModeConfig& config);

    /**
     * @brief Pause play mode
     */
    void pause();

    /**
     * @brief Resume from pause
     */
    void resume();

    /**
     * @brief Stop play mode and restore editor state
     */
    void stop();

    /**
     * @brief Toggle play/pause
     */
    void togglePlayPause();

    /**
     * @brief Restart from beginning
     */
    void restart();

    // =========================================================================
    // State Queries
    // =========================================================================

    /**
     * @brief Get current play mode state
     */
    [[nodiscard]] PlayModeState getState() const { return m_state; }

    /**
     * @brief Check if in play mode (playing or paused)
     */
    [[nodiscard]] bool isInPlayMode() const;

    /**
     * @brief Check if playing
     */
    [[nodiscard]] bool isPlaying() const { return m_state == PlayModeState::Playing; }

    /**
     * @brief Check if paused
     */
    [[nodiscard]] bool isPaused() const { return m_state == PlayModeState::Paused; }

    /**
     * @brief Check if stopped (normal editor mode)
     */
    [[nodiscard]] bool isStopped() const { return m_state == PlayModeState::Stopped; }

    /**
     * @brief Get current configuration
     */
    [[nodiscard]] const PlayModeConfig& getConfig() const { return m_config; }

    /**
     * @brief Get play mode statistics
     */
    [[nodiscard]] const PlayModeStats& getStats() const { return m_stats; }

    /**
     * @brief Get current play time
     */
    [[nodiscard]] f64 getPlayTime() const { return m_stats.totalPlayTime; }

    // =========================================================================
    // Configuration
    // =========================================================================

    /**
     * @brief Set default configuration
     */
    void setDefaultConfig(const PlayModeConfig& config);

    /**
     * @brief Get default configuration
     */
    [[nodiscard]] const PlayModeConfig& getDefaultConfig() const { return m_defaultConfig; }

    /**
     * @brief Set time scale
     */
    void setTimeScale(f64 scale);

    /**
     * @brief Get time scale
     */
    [[nodiscard]] f64 getTimeScale() const { return m_config.timeScale; }

    // =========================================================================
    // Debug Features
    // =========================================================================

    /**
     * @brief Step to next dialogue/choice
     */
    void stepNext();

    /**
     * @brief Step into current node
     */
    void stepInto();

    /**
     * @brief Step out of current node
     */
    void stepOut();

    /**
     * @brief Add a breakpoint
     */
    void addBreakpoint(const std::string& nodeId, const std::string& condition = "");

    /**
     * @brief Remove a breakpoint
     */
    void removeBreakpoint(const std::string& breakpointId);

    /**
     * @brief Toggle breakpoint on a node
     */
    void toggleBreakpoint(const std::string& nodeId);

    /**
     * @brief Enable/disable a breakpoint
     */
    void setBreakpointEnabled(const std::string& breakpointId, bool enabled);

    /**
     * @brief Get all breakpoints
     */
    [[nodiscard]] std::vector<Breakpoint> getBreakpoints() const;

    /**
     * @brief Get breakpoint for a node
     */
    [[nodiscard]] std::optional<Breakpoint> getBreakpointForNode(const std::string& nodeId) const;

    /**
     * @brief Clear all breakpoints
     */
    void clearAllBreakpoints();

    /**
     * @brief Continue execution after breakpoint
     */
    void continueExecution();

    // =========================================================================
    // Scene Control
    // =========================================================================

    /**
     * @brief Jump to specific scene during play
     */
    void jumpToScene(const std::string& sceneId);

    /**
     * @brief Jump to specific node during play
     */
    void jumpToNode(const std::string& nodeId);

    /**
     * @brief Set variable value during play
     */
    void setVariable(const std::string& name, const std::string& value);

    /**
     * @brief Get variable value during play
     */
    [[nodiscard]] std::string getVariable(const std::string& name) const;

    /**
     * @brief Get all variables
     */
    [[nodiscard]] std::unordered_map<std::string, std::string> getAllVariables() const;

    // =========================================================================
    // Update
    // =========================================================================

    /**
     * @brief Update play mode (call from main loop)
     */
    void update(f64 deltaTime);

    // =========================================================================
    // Event Handling
    // =========================================================================

    /**
     * @brief Add play mode listener
     */
    void addListener(IPlayModeListener* listener);

    /**
     * @brief Remove play mode listener
     */
    void removeListener(IPlayModeListener* listener);

    // =========================================================================
    // Toolbar Integration
    // =========================================================================

    /**
     * @brief Render play mode toolbar (for embedding in main toolbar)
     */
    void renderToolbar();

    /**
     * @brief Render debug overlay (for scene view)
     */
    void renderDebugOverlay();

private:
    // State transition methods
    void enterPlayMode();
    void exitPlayMode();
    void captureSceneSnapshot();
    void restoreSceneSnapshot();

    // Notification methods
    void notifyStateChanged(PlayModeState oldState, PlayModeState newState);
    void notifyBreakpointHit(const Breakpoint& breakpoint);
    void notifyError(const std::string& error, const std::string& nodeId);

    // Update stats
    void updateStats(f64 deltaTime);
    void resetStats();

    // Generate unique ID
    std::string generateBreakpointId();

    // State
    PlayModeState m_state = PlayModeState::Stopped;
    PlayModeConfig m_config;
    PlayModeConfig m_defaultConfig;
    PlayModeStats m_stats;

    // Scene snapshot for restoration
    std::unique_ptr<SceneSnapshot> m_sceneSnapshot;

    // Breakpoints
    std::vector<Breakpoint> m_breakpoints;
    u64 m_nextBreakpointId = 1;
    bool m_atBreakpoint = false;
    std::string m_currentBreakpointId;

    // Variables (runtime state)
    std::unordered_map<std::string, std::string> m_variables;

    // Current execution context
    std::string m_currentNodeId;
    std::string m_currentSceneId;

    // Frame timing
    f64 m_lastFrameTime = 0.0;
    f64 m_frameTimeAccumulator = 0.0;
    u32 m_frameTimeCount = 0;

    // Listeners
    std::vector<IPlayModeListener*> m_listeners;

    // Editor reference
    EditorApp* m_app = nullptr;

    // Singleton
    static std::unique_ptr<PlayModeController> s_instance;
};

} // namespace NovelMind::editor
