#pragma once

/**
 * @file timeline_panel.hpp
 * @brief Timeline Panel for NovelMind Editor v0.2.0
 *
 * The Timeline panel provides animation and sequencing capabilities:
 * - Keyframe-based animation editing
 * - Track management (position, rotation, scale, alpha, etc.)
 * - Curve editor for easing
 * - Playback controls
 * - Time scrubbing
 * - Multi-track selection and editing
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/renderer/color.hpp"
#include <vector>
#include <string>
#include <unordered_map>

namespace NovelMind::editor {

/**
 * @brief Track types for the timeline
 */
enum class TrackType : u8 {
    Transform,   // Position, rotation, scale
    Alpha,       // Opacity
    Color,       // Color tint
    Audio,       // Sound/music clips
    Event,       // Event markers
    Camera,      // Camera movement
    Custom       // User-defined
};

/**
 * @brief Keyframe easing type
 */
enum class EasingType : u8 {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
    Elastic,
    Custom
};

/**
 * @brief Keyframe data
 */
struct Keyframe {
    f64 time = 0.0;
    f32 value = 0.0f;
    EasingType easing = EasingType::Linear;
    f32 tangentIn = 0.0f;
    f32 tangentOut = 0.0f;
    bool selected = false;
};

/**
 * @brief Track data
 */
struct Track {
    std::string id;
    std::string name;
    std::string objectId;    // Associated scene object
    std::string property;    // Property being animated
    TrackType type = TrackType::Transform;
    bool visible = true;
    bool locked = false;
    bool expanded = true;
    renderer::Color color{100, 150, 200, 255};
    std::vector<Keyframe> keyframes;
};

/**
 * @brief Timeline Panel implementation
 */
class TimelinePanel : public GUIPanelBase {
public:
    TimelinePanel();
    ~TimelinePanel() override = default;

    // =========================================================================
    // Playback Control
    // =========================================================================

    /**
     * @brief Start playback
     */
    void play();

    /**
     * @brief Pause playback
     */
    void pause();

    /**
     * @brief Stop playback and reset to start
     */
    void stop();

    /**
     * @brief Toggle play/pause
     */
    void togglePlayPause();

    /**
     * @brief Check if playing
     */
    [[nodiscard]] bool isPlaying() const { return m_isPlaying; }

    /**
     * @brief Check if paused
     */
    [[nodiscard]] bool isPaused() const { return m_isPaused; }

    /**
     * @brief Step forward one frame
     */
    void stepForward();

    /**
     * @brief Step backward one frame
     */
    void stepBackward();

    /**
     * @brief Go to start
     */
    void goToStart();

    /**
     * @brief Go to end
     */
    void goToEnd();

    // =========================================================================
    // Time Management
    // =========================================================================

    /**
     * @brief Get current time
     */
    [[nodiscard]] f64 getCurrentTime() const { return m_currentTime; }

    /**
     * @brief Set current time (scrub)
     */
    void setCurrentTime(f64 time);

    /**
     * @brief Get timeline duration
     */
    [[nodiscard]] f64 getDuration() const { return m_duration; }

    /**
     * @brief Set timeline duration
     */
    void setDuration(f64 duration);

    /**
     * @brief Get frames per second
     */
    [[nodiscard]] f32 getFPS() const { return m_fps; }

    /**
     * @brief Set frames per second
     */
    void setFPS(f32 fps);

    /**
     * @brief Get playback speed multiplier
     */
    [[nodiscard]] f32 getPlaybackSpeed() const { return m_playbackSpeed; }

    /**
     * @brief Set playback speed multiplier
     */
    void setPlaybackSpeed(f32 speed);

    // =========================================================================
    // View State
    // =========================================================================

    /**
     * @brief Get view start time
     */
    [[nodiscard]] f64 getViewStart() const { return m_viewStartTime; }

    /**
     * @brief Get view end time
     */
    [[nodiscard]] f64 getViewEnd() const { return m_viewEndTime; }

    /**
     * @brief Set view range
     */
    void setViewRange(f64 start, f64 end);

    /**
     * @brief Zoom to fit all keyframes
     */
    void zoomToFit();

    /**
     * @brief Reset view to default
     */
    void resetView();

    // =========================================================================
    // Track Management
    // =========================================================================

    /**
     * @brief Add a new track
     */
    std::string addTrack(const std::string& name, TrackType type, const std::string& objectId = "");

    /**
     * @brief Remove a track
     */
    void removeTrack(const std::string& trackId);

    /**
     * @brief Get track by ID
     */
    Track* getTrack(const std::string& trackId);

    /**
     * @brief Get all tracks
     */
    [[nodiscard]] const std::vector<Track>& getTracks() const { return m_tracks; }

    /**
     * @brief Clear all tracks
     */
    void clearTracks();

    // =========================================================================
    // Keyframe Operations
    // =========================================================================

    /**
     * @brief Add keyframe to track
     */
    void addKeyframe(const std::string& trackId, f64 time, f32 value, EasingType easing = EasingType::Linear);

    /**
     * @brief Remove keyframe
     */
    void removeKeyframe(const std::string& trackId, size_t keyframeIndex);

    /**
     * @brief Move keyframe
     */
    void moveKeyframe(const std::string& trackId, size_t keyframeIndex, f64 newTime);

    /**
     * @brief Set keyframe value
     */
    void setKeyframeValue(const std::string& trackId, size_t keyframeIndex, f32 value);

    /**
     * @brief Set keyframe easing
     */
    void setKeyframeEasing(const std::string& trackId, size_t keyframeIndex, EasingType easing);

    /**
     * @brief Delete selected keyframes
     */
    void deleteSelectedKeyframes();

    /**
     * @brief Select keyframe
     */
    void selectKeyframe(const std::string& trackId, size_t keyframeIndex, bool addToSelection = false);

    /**
     * @brief Clear keyframe selection
     */
    void clearKeyframeSelection();

    // =========================================================================
    // GUIPanelBase Overrides
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
    // Rendering methods
    void renderRuler();
    void renderTrackHeaders();
    void renderTrackContent();
    void renderPlayhead();
    void renderKeyframes(const Track& track, f32 trackY, f32 trackHeight);
    void renderCurveEditor();

    // Input handling
    void handleMouseInput();
    void handleKeyboardInput();

    // Time/pixel conversion
    f32 timeToPixel(f64 time) const;
    f64 pixelToTime(f32 pixel) const;
    f64 snapTime(f64 time) const;

    // Interpolation
    f32 evaluateTrack(const Track& track, f64 time) const;
    f32 interpolateKeyframes(const Keyframe& k1, const Keyframe& k2, f64 time) const;

    // Playback state
    bool m_isPlaying = false;
    bool m_isPaused = false;
    f64 m_currentTime = 0.0;
    f64 m_duration = 10.0;
    f32 m_fps = 60.0f;
    f32 m_playbackSpeed = 1.0f;

    // View state
    f64 m_viewStartTime = 0.0;
    f64 m_viewEndTime = 10.0;
    f32 m_pixelsPerSecond = 100.0f;
    f32 m_trackHeight = 30.0f;
    f32 m_headerWidth = 200.0f;
    f32 m_rulerHeight = 30.0f;

    // Snapping
    bool m_snapEnabled = true;
    f64 m_snapInterval = 0.1;  // Snap to 0.1 second increments

    // Tracks
    std::vector<Track> m_tracks;
    u64 m_nextTrackId = 1;

    // Selection
    std::vector<std::pair<std::string, size_t>> m_selectedKeyframes;  // (trackId, keyframeIndex)

    // Interaction state
    bool m_isDraggingPlayhead = false;
    bool m_isDraggingKeyframe = false;
    bool m_isBoxSelecting = false;
    f32 m_dragStartX = 0.0f;
    f32 m_boxSelectStartX = 0.0f;
    f32 m_boxSelectEndX = 0.0f;

    // Curve editor
    bool m_showCurveEditor = false;
    std::string m_curveEditorTrackId;

    // Loop mode
    bool m_loopPlayback = false;
    f64 m_loopStart = 0.0;
    f64 m_loopEnd = 10.0;

    // Colors
    renderer::Color m_rulerColor{50, 50, 50, 255};
    renderer::Color m_playheadColor{255, 100, 100, 255};
    renderer::Color m_keyframeColor{0, 122, 204, 255};
    renderer::Color m_keyframeSelectedColor{255, 200, 0, 255};
    renderer::Color m_trackBackgroundColor{35, 35, 35, 255};
    renderer::Color m_trackBackgroundAltColor{40, 40, 40, 255};
};

} // namespace NovelMind::editor
