/**
 * @file debug_overlay_panel.hpp
 * @brief Debug Overlay Panel for NovelMind Editor v0.2.0
 *
 * Provides comprehensive play-mode debugging information:
 * - Active VM instruction display
 * - Call stack visualization
 * - Variable watch
 * - Active animations list
 * - Audio channels monitor
 * - Timeline track status
 * - Current StoryGraph node highlight
 * - Performance metrics
 */

#pragma once

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/editor/play_mode_controller.hpp"
#include "NovelMind/renderer/color.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace NovelMind::editor {

/**
 * @brief Debug overlay section visibility
 */
struct DebugOverlayConfig {
    bool showVMState = true;
    bool showCallStack = true;
    bool showVariables = true;
    bool showAnimations = true;
    bool showAudioChannels = true;
    bool showTimelineState = true;
    bool showNodeHighlight = true;
    bool showPerformance = true;
    bool compactMode = false;
    f32 opacity = 0.9f;
    bool alwaysOnTop = true;
};

/**
 * @brief VM instruction info for display
 */
struct VMInstructionInfo {
    u32 index;
    std::string opcode;
    std::string operand;
    bool isCurrent = false;
    bool hasBreakpoint = false;
};

/**
 * @brief Call stack frame info
 */
struct CallStackFrame {
    std::string functionName;
    std::string sourceFile;
    u32 lineNumber = 0;
    std::string nodeId;
    bool isCurrentFrame = false;
};

/**
 * @brief Variable watch entry
 */
struct WatchVariable {
    std::string name;
    std::string type;
    std::string value;
    bool isModified = false;      // Changed this frame
    bool isPinned = false;        // User-pinned
    std::string previousValue;    // For change detection
};

/**
 * @brief Animation state info
 */
struct AnimationStateInfo {
    std::string name;
    std::string targetObject;
    std::string property;
    f32 progress = 0.0f;          // 0.0 - 1.0
    f32 duration = 0.0f;
    bool isPlaying = true;
    bool isLooping = false;
};

/**
 * @brief Audio channel state info
 */
struct AudioChannelInfo {
    i32 channelId;
    std::string soundName;
    f32 volume = 1.0f;
    f32 pan = 0.0f;
    f32 position = 0.0f;          // Current playback position
    f32 duration = 0.0f;
    bool isPlaying = true;
    bool isLooping = false;
    bool isVoice = false;
    bool isMusic = false;
};

/**
 * @brief Timeline track state info
 */
struct TimelineTrackInfo {
    std::string trackName;
    std::string trackType;
    f32 currentTime = 0.0f;
    f32 duration = 0.0f;
    i32 currentKeyframe = -1;
    i32 nextKeyframe = -1;
    bool isActive = true;
};

/**
 * @brief Performance metrics
 */
struct PerformanceMetrics {
    f64 frameTime = 0.0;
    f64 fps = 0.0;
    f64 cpuTime = 0.0;
    f64 gpuTime = 0.0;
    u64 memoryUsage = 0;
    u32 drawCalls = 0;
    u32 triangles = 0;
    u32 textureMemory = 0;
};

/**
 * @brief Debug Overlay Panel
 *
 * This panel provides a comprehensive debugging interface during play mode:
 *
 * Features:
 * - VM State: Shows current instruction, PC, stack pointer
 * - Call Stack: Visual call hierarchy with navigation
 * - Variables: Watch variables with change highlighting
 * - Animations: List of active animations with progress
 * - Audio: Active sound channels with volume meters
 * - Timeline: Current track states and keyframe info
 * - Node Highlight: Shows current/next StoryGraph nodes
 * - Performance: FPS, frame time, memory, draw calls
 *
 * The overlay can be configured to show/hide sections and
 * can be docked or floating.
 */
class DebugOverlayPanel : public GUIPanelBase, public IPlayModeListener {
public:
    DebugOverlayPanel();
    ~DebugOverlayPanel() override;

    /**
     * @brief Set configuration
     */
    void setConfig(const DebugOverlayConfig& config);

    /**
     * @brief Get configuration
     */
    [[nodiscard]] const DebugOverlayConfig& getConfig() const { return m_config; }

    // =========================================================================
    // VM State
    // =========================================================================

    /**
     * @brief Set VM instructions to display
     */
    void setVMInstructions(const std::vector<VMInstructionInfo>& instructions);

    /**
     * @brief Set current instruction index
     */
    void setCurrentInstruction(u32 index);

    /**
     * @brief Set VM registers
     */
    void setVMRegisters(u32 pc, u32 sp, u32 fp);

    // =========================================================================
    // Call Stack
    // =========================================================================

    /**
     * @brief Set call stack frames
     */
    void setCallStack(const std::vector<CallStackFrame>& frames);

    /**
     * @brief Set current frame index
     */
    void setCurrentFrame(i32 frameIndex);

    // =========================================================================
    // Variables
    // =========================================================================

    /**
     * @brief Set watched variables
     */
    void setVariables(const std::vector<WatchVariable>& variables);

    /**
     * @brief Add a variable to watch
     */
    void addWatch(const std::string& variableName);

    /**
     * @brief Remove a variable from watch
     */
    void removeWatch(const std::string& variableName);

    /**
     * @brief Clear all watches
     */
    void clearWatches();

    /**
     * @brief Pin/unpin a variable
     */
    void setPinned(const std::string& variableName, bool pinned);

    // =========================================================================
    // Animations
    // =========================================================================

    /**
     * @brief Set active animations
     */
    void setAnimations(const std::vector<AnimationStateInfo>& animations);

    // =========================================================================
    // Audio
    // =========================================================================

    /**
     * @brief Set audio channel states
     */
    void setAudioChannels(const std::vector<AudioChannelInfo>& channels);

    // =========================================================================
    // Timeline
    // =========================================================================

    /**
     * @brief Set timeline track states
     */
    void setTimelineTracks(const std::vector<TimelineTrackInfo>& tracks);

    /**
     * @brief Set current playback time
     */
    void setPlaybackTime(f32 time);

    // =========================================================================
    // Node Highlight
    // =========================================================================

    /**
     * @brief Set current story node
     */
    void setCurrentNode(const std::string& nodeId, const std::string& nodeName);

    /**
     * @brief Set next possible nodes (for branching visualization)
     */
    void setNextNodes(const std::vector<std::pair<std::string, std::string>>& nodes);

    // =========================================================================
    // Performance
    // =========================================================================

    /**
     * @brief Set performance metrics
     */
    void setPerformanceMetrics(const PerformanceMetrics& metrics);

    // =========================================================================
    // Callbacks
    // =========================================================================

    /**
     * @brief Set callback for navigating to node
     */
    void setOnNavigateToNode(std::function<void(const std::string& nodeId)> callback);

    /**
     * @brief Set callback for navigating to source location
     */
    void setOnNavigateToSource(std::function<void(const std::string& file, u32 line)> callback);

    /**
     * @brief Set callback for selecting stack frame
     */
    void setOnSelectFrame(std::function<void(i32 frameIndex)> callback);

    // =========================================================================
    // Play Mode Listener Interface
    // =========================================================================

    void onPlayModeStateChanged(PlayModeState oldState, PlayModeState newState) override;
    void onPlayModeStarted() override;
    void onPlayModeStopped() override;
    void onPlayModePaused() override;
    void onBreakpointHit(const Breakpoint& breakpoint) override;
    void onScriptError(const std::string& error, const std::string& nodeId) override;

    // =========================================================================
    // Panel Interface
    // =========================================================================

    [[nodiscard]] std::vector<MenuItem> getMenuItems() const override;
    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;

protected:
    void onInitialize() override;
    void onShutdown() override;
    void onUpdate(f64 deltaTime) override;
    void onRender() override;
    void renderToolbar() override;

private:
    // Rendering sections
    void renderVMState();
    void renderCallStack();
    void renderVariables();
    void renderAnimations();
    void renderAudioChannels();
    void renderTimelineState();
    void renderNodeHighlight();
    void renderPerformance();

    // Helpers
    void renderCollapsibleSection(const char* title, bool* isOpen, std::function<void()> content);
    void renderProgressBar(f32 progress, f32 width, const renderer::Color& color);
    void renderVolumeBar(f32 volume, f32 width);
    void renderVariableEntry(const WatchVariable& var);
    renderer::Color getValueTypeColor(const std::string& type) const;

    // Configuration
    DebugOverlayConfig m_config;

    // Section collapsed states
    bool m_vmStateOpen = true;
    bool m_callStackOpen = true;
    bool m_variablesOpen = true;
    bool m_animationsOpen = true;
    bool m_audioOpen = true;
    bool m_timelineOpen = true;
    bool m_nodeHighlightOpen = true;
    bool m_performanceOpen = true;

    // Data
    std::vector<VMInstructionInfo> m_vmInstructions;
    u32 m_currentInstruction = 0;
    u32 m_pc = 0, m_sp = 0, m_fp = 0;

    std::vector<CallStackFrame> m_callStack;
    i32 m_currentFrame = 0;

    std::vector<WatchVariable> m_variables;
    std::vector<std::string> m_userWatches;

    std::vector<AnimationStateInfo> m_animations;
    std::vector<AudioChannelInfo> m_audioChannels;
    std::vector<TimelineTrackInfo> m_timelineTracks;
    f32 m_playbackTime = 0.0f;

    std::string m_currentNodeId;
    std::string m_currentNodeName;
    std::vector<std::pair<std::string, std::string>> m_nextNodes;

    PerformanceMetrics m_metrics;

    // Error state
    bool m_hasError = false;
    std::string m_errorMessage;
    std::string m_errorNodeId;

    // UI state
    char m_watchInputBuffer[128] = {0};
    i32 m_selectedStackFrame = -1;
    i32 m_selectedVariable = -1;
    f32 m_scrollPositions[8] = {0};  // Per-section scroll

    // Frame time history for graph
    static constexpr size_t FrameHistorySize = 120;
    std::vector<f32> m_frameTimeHistory;
    size_t m_frameHistoryIndex = 0;

    // Callbacks
    std::function<void(const std::string&)> m_onNavigateToNode;
    std::function<void(const std::string&, u32)> m_onNavigateToSource;
    std::function<void(i32)> m_onSelectFrame;
};

} // namespace NovelMind::editor
