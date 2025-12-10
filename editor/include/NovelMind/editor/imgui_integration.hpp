#pragma once

/**
 * @file imgui_integration.hpp
 * @brief ImGui Integration Layer for NovelMind Editor v0.2.0
 *
 * Provides the integration between ImGui and the NovelMind editor:
 * - ImGui context initialization and management
 * - SDL2 + OpenGL/Vulkan backend support
 * - Docking system wrapper
 * - Theme management
 * - Custom widgets for visual novel editing
 *
 * This is a core component for the v0.2.0 GUI milestone.
 */

#include "NovelMind/core/types.hpp"
#include "NovelMind/core/result.hpp"
#include "NovelMind/renderer/color.hpp"
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

// Forward declare ImGui types to avoid including imgui.h in header
struct ImGuiContext;
struct ImGuiStyle;
struct ImVec2;
struct ImVec4;

namespace NovelMind::editor {

// Forward declarations
class ImGuiLayer;

/**
 * @brief Editor theme configuration
 */
struct EditorTheme {
    std::string name = "Dark";

    // Primary colors
    renderer::Color background{30, 30, 30, 255};
    renderer::Color backgroundDark{20, 20, 20, 255};
    renderer::Color backgroundLight{45, 45, 45, 255};
    renderer::Color foreground{220, 220, 220, 255};
    renderer::Color foregroundDim{150, 150, 150, 255};

    // Accent colors
    renderer::Color accent{0, 122, 204, 255};      // Blue accent
    renderer::Color accentHover{0, 150, 230, 255};
    renderer::Color accentActive{0, 100, 180, 255};

    // Semantic colors
    renderer::Color success{76, 175, 80, 255};
    renderer::Color warning{255, 193, 7, 255};
    renderer::Color error{244, 67, 54, 255};
    renderer::Color info{33, 150, 243, 255};

    // Border and separators
    renderer::Color border{60, 60, 60, 255};
    renderer::Color borderHighlight{80, 80, 80, 255};
    renderer::Color separator{50, 50, 50, 255};

    // Selection colors
    renderer::Color selection{0, 122, 204, 128};
    renderer::Color selectionInactive{100, 100, 100, 128};

    // Text colors
    renderer::Color text{220, 220, 220, 255};
    renderer::Color textDisabled{120, 120, 120, 255};
    renderer::Color textHighlight{255, 255, 255, 255};

    // Graph editor colors
    renderer::Color nodeBackground{40, 40, 40, 230};
    renderer::Color nodeBorder{80, 80, 80, 255};
    renderer::Color nodeHeader{60, 60, 60, 255};
    renderer::Color connectionLine{150, 150, 150, 255};
    renderer::Color connectionLineActive{0, 122, 204, 255};

    // Timeline colors
    renderer::Color timelineBackground{25, 25, 25, 255};
    renderer::Color timelineRuler{40, 40, 40, 255};
    renderer::Color timelineKeyframe{0, 122, 204, 255};
    renderer::Color timelinePlayhead{255, 100, 100, 255};

    // Styling
    f32 windowRounding = 4.0f;
    f32 frameRounding = 2.0f;
    f32 scrollbarRounding = 2.0f;
    f32 grabRounding = 2.0f;
    f32 windowBorderSize = 1.0f;
    f32 frameBorderSize = 0.0f;
    f32 popupBorderSize = 1.0f;

    // Spacing
    f32 windowPadding = 8.0f;
    f32 framePadding = 4.0f;
    f32 itemSpacing = 8.0f;
    f32 itemInnerSpacing = 4.0f;
    f32 indentSpacing = 20.0f;

    // Font
    std::string fontPath = "";
    f32 fontSize = 14.0f;
    f32 iconSize = 16.0f;

    /**
     * @brief Create default dark theme
     */
    static EditorTheme createDarkTheme();

    /**
     * @brief Create light theme
     */
    static EditorTheme createLightTheme();

    /**
     * @brief Create high contrast theme
     */
    static EditorTheme createHighContrastTheme();
};

/**
 * @brief Docking layout configuration
 */
struct DockingLayout {
    std::string name = "Default";

    // Panel visibility
    bool showSceneView = true;
    bool showStoryGraph = true;
    bool showTimeline = true;
    bool showInspector = true;
    bool showAssetBrowser = true;
    bool showHierarchy = true;
    bool showConsole = true;
    bool showProjectBrowser = true;
    bool showVoiceManager = false;
    bool showLocalization = false;
    bool showDiagnostics = false;
    bool showBuildSettings = false;

    // Default panel sizes (as percentage of window)
    f32 leftPanelWidth = 0.2f;
    f32 rightPanelWidth = 0.25f;
    f32 bottomPanelHeight = 0.25f;

    /**
     * @brief Create default layout
     */
    static DockingLayout createDefaultLayout();

    /**
     * @brief Create layout focused on scene editing
     */
    static DockingLayout createSceneEditLayout();

    /**
     * @brief Create layout focused on story graph editing
     */
    static DockingLayout createStoryGraphLayout();

    /**
     * @brief Create layout focused on timeline editing
     */
    static DockingLayout createTimelineLayout();
};

/**
 * @brief ImGui Layer - manages ImGui context and rendering
 */
class ImGuiLayer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    // Prevent copying
    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;

    /**
     * @brief Initialize ImGui with SDL2 window
     * @param windowHandle Platform window handle (SDL_Window*)
     * @param glContext OpenGL context (SDL_GLContext) or nullptr for Vulkan
     */
    Result<void> initialize(void* windowHandle, void* glContext = nullptr);

    /**
     * @brief Shutdown ImGui
     */
    void shutdown();

    /**
     * @brief Begin new ImGui frame
     */
    void beginFrame();

    /**
     * @brief End ImGui frame and render
     */
    void endFrame();

    /**
     * @brief Process SDL event
     * @return true if event was consumed by ImGui
     */
    bool processEvent(void* sdlEvent);

    /**
     * @brief Check if ImGui wants to capture keyboard
     */
    [[nodiscard]] bool wantsKeyboard() const;

    /**
     * @brief Check if ImGui wants to capture mouse
     */
    [[nodiscard]] bool wantsMouse() const;

    // =========================================================================
    // Theme Management
    // =========================================================================

    /**
     * @brief Apply a theme
     */
    void applyTheme(const EditorTheme& theme);

    /**
     * @brief Get current theme
     */
    [[nodiscard]] const EditorTheme& getCurrentTheme() const { return m_currentTheme; }

    /**
     * @brief Set UI scale factor
     */
    void setUIScale(f32 scale);

    /**
     * @brief Get current UI scale
     */
    [[nodiscard]] f32 getUIScale() const { return m_uiScale; }

    // =========================================================================
    // Docking System
    // =========================================================================

    /**
     * @brief Enable/disable docking
     */
    void setDockingEnabled(bool enabled);

    /**
     * @brief Check if docking is enabled
     */
    [[nodiscard]] bool isDockingEnabled() const { return m_dockingEnabled; }

    /**
     * @brief Apply a docking layout
     */
    void applyLayout(const DockingLayout& layout);

    /**
     * @brief Save current layout to file
     */
    Result<void> saveLayout(const std::string& filepath);

    /**
     * @brief Load layout from file
     */
    Result<DockingLayout> loadLayout(const std::string& filepath);

    /**
     * @brief Reset to default layout
     */
    void resetToDefaultLayout();

    /**
     * @brief Begin dockspace (call at start of frame)
     */
    void beginDockspace();

    /**
     * @brief End dockspace
     */
    void endDockspace();

    // =========================================================================
    // Font Management
    // =========================================================================

    /**
     * @brief Load a font
     * @param name Identifier for the font
     * @param filepath Path to TTF file
     * @param size Font size in pixels
     */
    Result<void> loadFont(const std::string& name, const std::string& filepath, f32 size);

    /**
     * @brief Load icon font
     * @param name Identifier for the icon font
     * @param filepath Path to icon font TTF file
     * @param size Icon size in pixels
     * @param iconRangeStart First unicode codepoint
     * @param iconRangeEnd Last unicode codepoint
     */
    Result<void> loadIconFont(const std::string& name, const std::string& filepath,
                               f32 size, u16 iconRangeStart, u16 iconRangeEnd);

    /**
     * @brief Push a font by name
     */
    void pushFont(const std::string& name);

    /**
     * @brief Pop font
     */
    void popFont();

    // =========================================================================
    // Utility
    // =========================================================================

    /**
     * @brief Get singleton instance
     */
    static ImGuiLayer& instance();

    /**
     * @brief Check if initialized
     */
    [[nodiscard]] bool isInitialized() const { return m_initialized; }

    /**
     * @brief Get ImGui context
     */
    [[nodiscard]] ImGuiContext* getContext() const { return m_context; }

private:
    void setupDefaultStyle();
    void setupDockspace();

    ImGuiContext* m_context = nullptr;
    void* m_windowHandle = nullptr;
    void* m_glContext = nullptr;

    bool m_initialized = false;
    bool m_dockingEnabled = true;
    f32 m_uiScale = 1.0f;

    EditorTheme m_currentTheme;
    DockingLayout m_currentLayout;

    // Font management
    struct FontEntry {
        void* font = nullptr;  // ImFont*
        std::string path;
        f32 size;
    };
    std::unordered_map<std::string, FontEntry> m_fonts;

    static std::unique_ptr<ImGuiLayer> s_instance;
};

// ============================================================================
// Custom ImGui Widgets for NovelMind
// ============================================================================

/**
 * @brief Custom widget helpers for the NovelMind editor
 */
namespace widgets {

/**
 * @brief Draw a property label with consistent styling
 */
bool PropertyLabel(const char* label, f32 labelWidth = 100.0f);

/**
 * @brief Draw a property row with label and value
 */
bool PropertyRow(const char* label, const std::function<bool()>& valueWidget,
                 f32 labelWidth = 100.0f);

/**
 * @brief Draw a collapsing header with custom styling
 */
bool CollapsingHeader(const char* label, bool* isOpen = nullptr, bool defaultOpen = true);

/**
 * @brief Draw a section header
 */
void SectionHeader(const char* label);

/**
 * @brief Draw a horizontal separator with optional label
 */
void Separator(const char* label = nullptr);

/**
 * @brief Draw a toolbar button
 */
bool ToolbarButton(const char* icon, const char* tooltip = nullptr, bool selected = false);

/**
 * @brief Draw a toolbar toggle button
 */
bool ToolbarToggle(const char* icon, bool* value, const char* tooltip = nullptr);

/**
 * @brief Draw a toolbar separator
 */
void ToolbarSeparator();

/**
 * @brief Draw a search input field
 */
bool SearchInput(const char* label, char* buffer, size_t bufferSize,
                 const char* hint = "Search...");

/**
 * @brief Draw a color picker button
 */
bool ColorPickerButton(const char* label, f32* color4, bool showAlpha = true);

/**
 * @brief Draw a vector2 input
 */
bool Vector2Input(const char* label, f32* values, f32 speed = 1.0f);

/**
 * @brief Draw a vector3 input
 */
bool Vector3Input(const char* label, f32* values, f32 speed = 1.0f);

/**
 * @brief Draw an asset reference field
 */
bool AssetReference(const char* label, std::string& assetPath, const char* assetType);

/**
 * @brief Draw a dropdown combo box
 */
bool Dropdown(const char* label, i32* currentIndex, const std::vector<std::string>& items);

/**
 * @brief Draw a tree node with drag/drop support
 */
bool TreeNode(const char* label, bool isLeaf = false, bool isSelected = false,
              const char* dragDropType = nullptr, void* dragDropData = nullptr);

/**
 * @brief Draw a timeline ruler
 */
void TimelineRuler(f32 startTime, f32 endTime, f32 currentTime, f32& viewStart, f32& viewEnd);

/**
 * @brief Draw a keyframe marker
 */
bool KeyframeMarker(f32 time, bool selected = false, renderer::Color color = {0, 122, 204, 255});

/**
 * @brief Draw a minimap for node graph
 */
void NodeGraphMinimap(f32 x, f32 y, f32 width, f32 height,
                      const std::vector<std::pair<f32, f32>>& nodePositions);

/**
 * @brief Draw a splitter between panels
 */
bool Splitter(bool splitVertically, f32 thickness, f32* size1, f32* size2,
              f32 minSize1 = 100.0f, f32 minSize2 = 100.0f);

/**
 * @brief Begin a toolbar
 */
void BeginToolbar(const char* id, f32 height = 30.0f);

/**
 * @brief End toolbar
 */
void EndToolbar();

/**
 * @brief Begin a panel window with standard styling
 */
bool BeginPanel(const char* name, bool* open = nullptr, i32 flags = 0);

/**
 * @brief End panel window
 */
void EndPanel();

/**
 * @brief Show a tooltip with rich content
 */
void RichTooltip(const std::function<void()>& content);

/**
 * @brief Draw a loading spinner
 */
void LoadingSpinner(const char* label, f32 radius = 10.0f, f32 thickness = 3.0f);

/**
 * @brief Draw a progress bar with label
 */
void ProgressBarLabeled(f32 progress, const char* label, f32 height = 20.0f);

/**
 * @brief Draw a notification badge
 */
void NotificationBadge(i32 count, renderer::Color color = {244, 67, 54, 255});

} // namespace widgets

} // namespace NovelMind::editor
