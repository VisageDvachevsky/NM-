/**
 * @file imgui_integration.cpp
 * @brief ImGui Integration Layer implementation
 */

#include "NovelMind/editor/imgui_integration.hpp"
#include <cmath>
#include <algorithm>

// ImGui is optional at compile time - stub implementation when not available
// This allows the editor to compile without ImGui for testing purposes.
// When ImGui is available (NOVELMIND_HAS_IMGUI), real implementation is used.

namespace NovelMind::editor {

// ============================================================================
// Static instances
// ============================================================================

std::unique_ptr<ImGuiLayer> ImGuiLayer::s_instance = nullptr;

// ============================================================================
// EditorTheme Implementation
// ============================================================================

EditorTheme EditorTheme::createDarkTheme()
{
    EditorTheme theme;
    theme.name = "Dark";

    // Primary colors
    theme.background = {30, 30, 30, 255};
    theme.backgroundDark = {20, 20, 20, 255};
    theme.backgroundLight = {45, 45, 45, 255};
    theme.foreground = {220, 220, 220, 255};
    theme.foregroundDim = {150, 150, 150, 255};

    // Accent colors (VS Code blue)
    theme.accent = {0, 122, 204, 255};
    theme.accentHover = {0, 150, 230, 255};
    theme.accentActive = {0, 100, 180, 255};

    // Semantic colors
    theme.success = {76, 175, 80, 255};
    theme.warning = {255, 193, 7, 255};
    theme.error = {244, 67, 54, 255};
    theme.info = {33, 150, 243, 255};

    // Border and separators
    theme.border = {60, 60, 60, 255};
    theme.borderHighlight = {80, 80, 80, 255};
    theme.separator = {50, 50, 50, 255};

    // Selection colors
    theme.selection = {0, 122, 204, 128};
    theme.selectionInactive = {100, 100, 100, 128};

    // Text colors
    theme.text = {220, 220, 220, 255};
    theme.textDisabled = {120, 120, 120, 255};
    theme.textHighlight = {255, 255, 255, 255};

    // Graph editor colors
    theme.nodeBackground = {40, 40, 40, 230};
    theme.nodeBorder = {80, 80, 80, 255};
    theme.nodeHeader = {60, 60, 60, 255};
    theme.connectionLine = {150, 150, 150, 255};
    theme.connectionLineActive = {0, 122, 204, 255};

    // Timeline colors
    theme.timelineBackground = {25, 25, 25, 255};
    theme.timelineRuler = {40, 40, 40, 255};
    theme.timelineKeyframe = {0, 122, 204, 255};
    theme.timelinePlayhead = {255, 100, 100, 255};

    // Styling
    theme.windowRounding = 4.0f;
    theme.frameRounding = 2.0f;
    theme.scrollbarRounding = 2.0f;
    theme.grabRounding = 2.0f;
    theme.windowBorderSize = 1.0f;
    theme.frameBorderSize = 0.0f;
    theme.popupBorderSize = 1.0f;

    // Spacing
    theme.windowPadding = 8.0f;
    theme.framePadding = 4.0f;
    theme.itemSpacing = 8.0f;
    theme.itemInnerSpacing = 4.0f;
    theme.indentSpacing = 20.0f;

    theme.fontSize = 14.0f;
    theme.iconSize = 16.0f;

    return theme;
}

EditorTheme EditorTheme::createLightTheme()
{
    EditorTheme theme;
    theme.name = "Light";

    // Primary colors
    theme.background = {245, 245, 245, 255};
    theme.backgroundDark = {230, 230, 230, 255};
    theme.backgroundLight = {255, 255, 255, 255};
    theme.foreground = {40, 40, 40, 255};
    theme.foregroundDim = {120, 120, 120, 255};

    // Accent colors
    theme.accent = {0, 120, 215, 255};
    theme.accentHover = {0, 100, 190, 255};
    theme.accentActive = {0, 80, 170, 255};

    // Semantic colors
    theme.success = {56, 142, 60, 255};
    theme.warning = {245, 127, 23, 255};
    theme.error = {211, 47, 47, 255};
    theme.info = {25, 118, 210, 255};

    // Border and separators
    theme.border = {200, 200, 200, 255};
    theme.borderHighlight = {180, 180, 180, 255};
    theme.separator = {210, 210, 210, 255};

    // Selection colors
    theme.selection = {0, 120, 215, 80};
    theme.selectionInactive = {180, 180, 180, 80};

    // Text colors
    theme.text = {40, 40, 40, 255};
    theme.textDisabled = {160, 160, 160, 255};
    theme.textHighlight = {0, 0, 0, 255};

    // Graph editor colors
    theme.nodeBackground = {250, 250, 250, 230};
    theme.nodeBorder = {180, 180, 180, 255};
    theme.nodeHeader = {235, 235, 235, 255};
    theme.connectionLine = {120, 120, 120, 255};
    theme.connectionLineActive = {0, 120, 215, 255};

    // Timeline colors
    theme.timelineBackground = {240, 240, 240, 255};
    theme.timelineRuler = {220, 220, 220, 255};
    theme.timelineKeyframe = {0, 120, 215, 255};
    theme.timelinePlayhead = {220, 80, 80, 255};

    // Same styling as dark theme
    theme.windowRounding = 4.0f;
    theme.frameRounding = 2.0f;
    theme.scrollbarRounding = 2.0f;
    theme.grabRounding = 2.0f;
    theme.windowBorderSize = 1.0f;
    theme.frameBorderSize = 0.0f;
    theme.popupBorderSize = 1.0f;
    theme.windowPadding = 8.0f;
    theme.framePadding = 4.0f;
    theme.itemSpacing = 8.0f;
    theme.itemInnerSpacing = 4.0f;
    theme.indentSpacing = 20.0f;
    theme.fontSize = 14.0f;
    theme.iconSize = 16.0f;

    return theme;
}

EditorTheme EditorTheme::createHighContrastTheme()
{
    EditorTheme theme;
    theme.name = "High Contrast";

    // High contrast primary colors
    theme.background = {0, 0, 0, 255};
    theme.backgroundDark = {0, 0, 0, 255};
    theme.backgroundLight = {30, 30, 30, 255};
    theme.foreground = {255, 255, 255, 255};
    theme.foregroundDim = {200, 200, 200, 255};

    // Bright accent colors
    theme.accent = {0, 200, 255, 255};
    theme.accentHover = {100, 220, 255, 255};
    theme.accentActive = {0, 160, 220, 255};

    // High contrast semantic colors
    theme.success = {100, 255, 100, 255};
    theme.warning = {255, 255, 0, 255};
    theme.error = {255, 100, 100, 255};
    theme.info = {100, 200, 255, 255};

    // Visible borders
    theme.border = {128, 128, 128, 255};
    theme.borderHighlight = {200, 200, 200, 255};
    theme.separator = {100, 100, 100, 255};

    // High contrast selection
    theme.selection = {0, 200, 255, 180};
    theme.selectionInactive = {150, 150, 150, 180};

    // High contrast text
    theme.text = {255, 255, 255, 255};
    theme.textDisabled = {150, 150, 150, 255};
    theme.textHighlight = {255, 255, 0, 255};

    // Graph editor colors
    theme.nodeBackground = {20, 20, 20, 255};
    theme.nodeBorder = {128, 128, 128, 255};
    theme.nodeHeader = {40, 40, 40, 255};
    theme.connectionLine = {200, 200, 200, 255};
    theme.connectionLineActive = {0, 200, 255, 255};

    // Timeline colors
    theme.timelineBackground = {10, 10, 10, 255};
    theme.timelineRuler = {50, 50, 50, 255};
    theme.timelineKeyframe = {0, 200, 255, 255};
    theme.timelinePlayhead = {255, 100, 100, 255};

    // Sharp corners for high contrast
    theme.windowRounding = 0.0f;
    theme.frameRounding = 0.0f;
    theme.scrollbarRounding = 0.0f;
    theme.grabRounding = 0.0f;
    theme.windowBorderSize = 2.0f;
    theme.frameBorderSize = 1.0f;
    theme.popupBorderSize = 2.0f;
    theme.windowPadding = 8.0f;
    theme.framePadding = 4.0f;
    theme.itemSpacing = 8.0f;
    theme.itemInnerSpacing = 4.0f;
    theme.indentSpacing = 20.0f;
    theme.fontSize = 15.0f;
    theme.iconSize = 18.0f;

    return theme;
}

// ============================================================================
// DockingLayout Implementation
// ============================================================================

DockingLayout DockingLayout::createDefaultLayout()
{
    DockingLayout layout;
    layout.name = "Default";
    layout.showSceneView = true;
    layout.showStoryGraph = true;
    layout.showTimeline = true;
    layout.showInspector = true;
    layout.showAssetBrowser = true;
    layout.showHierarchy = true;
    layout.showConsole = true;
    layout.showProjectBrowser = true;
    layout.leftPanelWidth = 0.2f;
    layout.rightPanelWidth = 0.25f;
    layout.bottomPanelHeight = 0.25f;
    return layout;
}

DockingLayout DockingLayout::createSceneEditLayout()
{
    DockingLayout layout = createDefaultLayout();
    layout.name = "Scene Edit";
    layout.showStoryGraph = false;
    layout.showTimeline = true;
    layout.rightPanelWidth = 0.3f;
    return layout;
}

DockingLayout DockingLayout::createStoryGraphLayout()
{
    DockingLayout layout = createDefaultLayout();
    layout.name = "Story Graph";
    layout.showSceneView = false;
    layout.showTimeline = false;
    layout.showStoryGraph = true;
    return layout;
}

DockingLayout DockingLayout::createTimelineLayout()
{
    DockingLayout layout = createDefaultLayout();
    layout.name = "Timeline";
    layout.showTimeline = true;
    layout.bottomPanelHeight = 0.4f;
    return layout;
}

// ============================================================================
// ImGuiLayer Implementation
// ============================================================================

ImGuiLayer::ImGuiLayer()
{
    m_currentTheme = EditorTheme::createDarkTheme();
    m_currentLayout = DockingLayout::createDefaultLayout();
}

ImGuiLayer::~ImGuiLayer()
{
    if (m_initialized)
    {
        shutdown();
    }
}

ImGuiLayer& ImGuiLayer::instance()
{
    if (!s_instance)
    {
        s_instance = std::make_unique<ImGuiLayer>();
    }
    return *s_instance;
}

Result<void> ImGuiLayer::initialize(void* windowHandle, void* glContext)
{
    if (m_initialized)
    {
        return Result<void>::error("ImGui already initialized");
    }

    m_windowHandle = windowHandle;
    m_glContext = glContext;

    // Note: Actual ImGui initialization requires imgui.h and backends
    // This is a stub that sets up the state without ImGui
    // When ImGui is available:
    // - Create ImGui context
    // - Initialize SDL2 backend
    // - Initialize OpenGL/Vulkan backend
    // - Set up docking
    // - Load fonts

    m_context = nullptr; // Would be ImGui::CreateContext()

    setupDefaultStyle();
    setupDockspace();

    m_initialized = true;
    return Result<void>::ok();
}

void ImGuiLayer::shutdown()
{
    if (!m_initialized)
    {
        return;
    }

    // Cleanup ImGui backends and context
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplSDL2_Shutdown();
    // ImGui::DestroyContext();

    m_context = nullptr;
    m_windowHandle = nullptr;
    m_glContext = nullptr;
    m_initialized = false;
    m_fonts.clear();
}

void ImGuiLayer::beginFrame()
{
    if (!m_initialized)
    {
        return;
    }

    // Begin new ImGui frame
    // ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplSDL2_NewFrame();
    // ImGui::NewFrame();

    if (m_dockingEnabled)
    {
        beginDockspace();
    }
}

void ImGuiLayer::endFrame()
{
    if (!m_initialized)
    {
        return;
    }

    if (m_dockingEnabled)
    {
        endDockspace();
    }

    // Render ImGui
    // ImGui::Render();
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Handle viewport windows for docking
    // ImGuiIO& io = ImGui::GetIO();
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // {
    //     ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    // }
}

bool ImGuiLayer::processEvent(void* /*sdlEvent*/)
{
    if (!m_initialized)
    {
        return false;
    }

    // Process SDL event
    // return ImGui_ImplSDL2_ProcessEvent((SDL_Event*)sdlEvent);
    return false;
}

bool ImGuiLayer::wantsKeyboard() const
{
    if (!m_initialized)
    {
        return false;
    }
    // return ImGui::GetIO().WantCaptureKeyboard;
    return false;
}

bool ImGuiLayer::wantsMouse() const
{
    if (!m_initialized)
    {
        return false;
    }
    // return ImGui::GetIO().WantCaptureMouse;
    return false;
}

void ImGuiLayer::applyTheme(const EditorTheme& theme)
{
    m_currentTheme = theme;

    if (!m_initialized)
    {
        return;
    }

    // Apply theme to ImGui style
    // ImGuiStyle& style = ImGui::GetStyle();
    //
    // style.WindowRounding = theme.windowRounding;
    // style.FrameRounding = theme.frameRounding;
    // style.ScrollbarRounding = theme.scrollbarRounding;
    // style.GrabRounding = theme.grabRounding;
    // etc.
}

void ImGuiLayer::setUIScale(f32 scale)
{
    m_uiScale = std::clamp(scale, 0.5f, 3.0f);

    if (!m_initialized)
    {
        return;
    }

    // Apply scale to ImGui
    // ImGuiStyle& style = ImGui::GetStyle();
    // style.ScaleAllSizes(m_uiScale);
}

void ImGuiLayer::setDockingEnabled(bool enabled)
{
    m_dockingEnabled = enabled;

    if (!m_initialized)
    {
        return;
    }

    // Update ImGui config flags
    // ImGuiIO& io = ImGui::GetIO();
    // if (enabled)
    //     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // else
    //     io.ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void ImGuiLayer::applyLayout(const DockingLayout& layout)
{
    m_currentLayout = layout;
    // Layout is applied in beginDockspace()
}

Result<void> ImGuiLayer::saveLayout(const std::string& /*filepath*/)
{
    // Would save ImGui.ini and custom layout data
    return Result<void>::ok();
}

Result<DockingLayout> ImGuiLayer::loadLayout(const std::string& /*filepath*/)
{
    // Would load ImGui.ini and custom layout data
    return Result<DockingLayout>::ok(m_currentLayout);
}

void ImGuiLayer::resetToDefaultLayout()
{
    m_currentLayout = DockingLayout::createDefaultLayout();
    // Would reset ImGui docking state
}

void ImGuiLayer::beginDockspace()
{
    if (!m_initialized || !m_dockingEnabled)
    {
        return;
    }

    // Create main dockspace
    // ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking |
    //     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
    //     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
    //     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    //
    // ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(viewport->WorkPos);
    // ImGui::SetNextWindowSize(viewport->WorkSize);
    // ImGui::SetNextWindowViewport(viewport->ID);
    //
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    //
    // ImGui::Begin("DockSpace", nullptr, flags);
    // ImGui::PopStyleVar(3);
    //
    // ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
    // ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
}

void ImGuiLayer::endDockspace()
{
    if (!m_initialized || !m_dockingEnabled)
    {
        return;
    }

    // ImGui::End(); // End DockSpace window
}

Result<void> ImGuiLayer::loadFont(const std::string& name, const std::string& filepath, f32 size)
{
    if (!m_initialized)
    {
        return Result<void>::error("ImGui not initialized");
    }

    // Would load font using ImGui
    // ImGuiIO& io = ImGui::GetIO();
    // ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), size * m_uiScale);
    // if (!font) return Result<void>::error("Failed to load font: " + filepath);

    FontEntry entry;
    entry.path = filepath;
    entry.size = size;
    entry.font = nullptr; // Would be ImFont*
    m_fonts[name] = entry;

    return Result<void>::ok();
}

Result<void> ImGuiLayer::loadIconFont(const std::string& name, const std::string& filepath,
                                       f32 size, u16 /*iconRangeStart*/, u16 /*iconRangeEnd*/)
{
    if (!m_initialized)
    {
        return Result<void>::error("ImGui not initialized");
    }

    // Would load icon font with glyph range
    // static const ImWchar iconRanges[] = { iconRangeStart, iconRangeEnd, 0 };
    // ImFontConfig config;
    // config.MergeMode = true;
    // config.GlyphMinAdvanceX = size;
    // ImGuiIO& io = ImGui::GetIO();
    // ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), size, &config, iconRanges);

    FontEntry entry;
    entry.path = filepath;
    entry.size = size;
    entry.font = nullptr;
    m_fonts[name] = entry;

    return Result<void>::ok();
}

void ImGuiLayer::pushFont(const std::string& name)
{
    if (!m_initialized)
    {
        return;
    }

    auto it = m_fonts.find(name);
    if (it != m_fonts.end() && it->second.font != nullptr)
    {
        // ImGui::PushFont((ImFont*)it->second.font);
    }
}

void ImGuiLayer::popFont()
{
    if (!m_initialized)
    {
        return;
    }
    // ImGui::PopFont();
}

void ImGuiLayer::setupDefaultStyle()
{
    applyTheme(m_currentTheme);
}

void ImGuiLayer::setupDockspace()
{
    // Configure docking
    if (!m_initialized)
    {
        return;
    }

    // ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.ConfigDockingWithShift = false;
    // io.ConfigViewportsNoTaskBarIcon = true;
}

// ============================================================================
// Custom Widgets Implementation
// ============================================================================

namespace widgets {

bool PropertyLabel(const char* label, f32 /*labelWidth*/)
{
    // Would render a property label
    // ImGui::Text("%s", label);
    // ImGui::SameLine();
    // ImGui::SetCursorPosX(labelWidth);
    (void)label;
    return true;
}

bool PropertyRow(const char* label, const std::function<bool()>& valueWidget, f32 labelWidth)
{
    PropertyLabel(label, labelWidth);
    return valueWidget();
}

bool CollapsingHeader(const char* label, bool* isOpen, bool /*defaultOpen*/)
{
    // Would render collapsing header
    // return ImGui::CollapsingHeader(label, isOpen,
    //     defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0);
    if (isOpen) *isOpen = true;
    (void)label;
    return true;
}

void SectionHeader(const char* label)
{
    // Would render section header with styling
    // ImGui::Spacing();
    // ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%s", label);
    // ImGui::Separator();
    (void)label;
}

void Separator(const char* label)
{
    if (label)
    {
        // ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s", label);
    }
    // ImGui::Separator();
    (void)label;
}

bool ToolbarButton(const char* icon, const char* tooltip, bool selected)
{
    // Would render toolbar button
    // bool clicked = ImGui::Button(icon);
    // if (tooltip && ImGui::IsItemHovered())
    //     ImGui::SetTooltip("%s", tooltip);
    (void)icon;
    (void)tooltip;
    (void)selected;
    return false;
}

bool ToolbarToggle(const char* icon, bool* value, const char* tooltip)
{
    // Would render toggle button
    bool clicked = ToolbarButton(icon, tooltip, *value);
    if (clicked)
    {
        *value = !*value;
    }
    return clicked;
}

void ToolbarSeparator()
{
    // ImGui::SameLine();
    // ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    // ImGui::SameLine();
}

bool SearchInput(const char* label, char* buffer, size_t bufferSize, const char* hint)
{
    // Would render search input
    // return ImGui::InputTextWithHint(label, hint, buffer, bufferSize);
    (void)label;
    (void)buffer;
    (void)bufferSize;
    (void)hint;
    return false;
}

bool ColorPickerButton(const char* label, f32* color4, bool /*showAlpha*/)
{
    // Would render color picker button
    // return ImGui::ColorEdit4(label, color4, showAlpha ? 0 : ImGuiColorEditFlags_NoAlpha);
    (void)label;
    (void)color4;
    return false;
}

bool Vector2Input(const char* label, f32* values, f32 speed)
{
    // Would render vector2 input
    // return ImGui::DragFloat2(label, values, speed);
    (void)label;
    (void)values;
    (void)speed;
    return false;
}

bool Vector3Input(const char* label, f32* values, f32 speed)
{
    // Would render vector3 input
    // return ImGui::DragFloat3(label, values, speed);
    (void)label;
    (void)values;
    (void)speed;
    return false;
}

bool AssetReference(const char* label, std::string& /*assetPath*/, const char* /*assetType*/)
{
    // Would render asset reference field with browse button
    // char buffer[256];
    // strncpy(buffer, assetPath.c_str(), sizeof(buffer));
    // bool changed = ImGui::InputText(label, buffer, sizeof(buffer));
    // if (changed) assetPath = buffer;
    //
    // ImGui::SameLine();
    // if (ImGui::Button("..."))
    //     // Open asset picker
    (void)label;
    return false;
}

bool Dropdown(const char* label, i32* currentIndex, const std::vector<std::string>& items)
{
    // Would render dropdown combo
    // const char* preview = (*currentIndex >= 0 && *currentIndex < items.size()) ?
    //     items[*currentIndex].c_str() : "";
    // if (ImGui::BeginCombo(label, preview))
    // {
    //     for (int i = 0; i < items.size(); i++)
    //     {
    //         bool selected = (i == *currentIndex);
    //         if (ImGui::Selectable(items[i].c_str(), selected))
    //             *currentIndex = i;
    //     }
    //     ImGui::EndCombo();
    // }
    (void)label;
    (void)currentIndex;
    (void)items;
    return false;
}

bool TreeNode(const char* label, bool isLeaf, bool isSelected,
              const char* /*dragDropType*/, void* /*dragDropData*/)
{
    // Would render tree node with optional drag/drop
    // ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
    //     ImGuiTreeNodeFlags_SpanAvailWidth;
    // if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf;
    // if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;
    // return ImGui::TreeNodeEx(label, flags);
    (void)label;
    (void)isLeaf;
    (void)isSelected;
    return false;
}

void TimelineRuler(f32 startTime, f32 endTime, f32 currentTime,
                   f32& /*viewStart*/, f32& /*viewEnd*/)
{
    // Would render timeline ruler
    // Draw time markers, frame numbers, playhead indicator
    (void)startTime;
    (void)endTime;
    (void)currentTime;
}

bool KeyframeMarker(f32 /*time*/, bool /*selected*/, renderer::Color /*color*/)
{
    // Would render keyframe marker
    return false;
}

void NodeGraphMinimap(f32 /*x*/, f32 /*y*/, f32 /*width*/, f32 /*height*/,
                      const std::vector<std::pair<f32, f32>>& /*nodePositions*/)
{
    // Would render minimap of node graph
}

bool Splitter(bool /*splitVertically*/, f32 /*thickness*/, f32* size1, f32* size2,
              f32 minSize1, f32 minSize2)
{
    // Would render resizable splitter
    // ImGuiContext& g = *GImGui;
    // ImGuiWindow* window = g.CurrentWindow;
    //
    // ImGuiID id = window->GetID("##Splitter");
    // ImRect bb;
    // bb.Min = window->DC.CursorPos + (splitVertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    // bb.Max = bb.Min + CalcItemSize(splitVertically ? ImVec2(thickness, -1.0f) : ImVec2(-1.0f, thickness), 0.0f, 0.0f);
    // return SplitterBehavior(bb, id, splitVertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, minSize1, minSize2, 0.0f);
    (void)size1;
    (void)size2;
    (void)minSize1;
    (void)minSize2;
    return false;
}

void BeginToolbar(const char* /*id*/, f32 /*height*/)
{
    // Would begin toolbar
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
}

void EndToolbar()
{
    // Would end toolbar
    // ImGui::PopStyleVar(2);
}

bool BeginPanel(const char* name, bool* open, i32 flags)
{
    // Would begin panel window
    // return ImGui::Begin(name, open, flags);
    (void)name;
    (void)open;
    (void)flags;
    return true;
}

void EndPanel()
{
    // Would end panel window
    // ImGui::End();
}

void RichTooltip(const std::function<void()>& content)
{
    // Would show rich tooltip
    // if (ImGui::IsItemHovered())
    // {
    //     ImGui::BeginTooltip();
    //     content();
    //     ImGui::EndTooltip();
    // }
    (void)content;
}

void LoadingSpinner(const char* label, f32 radius, f32 thickness)
{
    // Would draw animated spinner
    // Using ImGui circle drawing with rotation animation
    (void)label;
    (void)radius;
    (void)thickness;
}

void ProgressBarLabeled(f32 progress, const char* label, f32 /*height*/)
{
    // Would draw progress bar with label
    // ImGui::ProgressBar(progress, ImVec2(-1, height));
    // ImGui::SameLine();
    // ImGui::Text("%s", label);
    (void)progress;
    (void)label;
}

void NotificationBadge(i32 count, renderer::Color /*color*/)
{
    // Would draw notification badge
    // if (count > 0)
    // {
    //     ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //     // Draw circle with count
    // }
    (void)count;
}

} // namespace widgets

} // namespace NovelMind::editor
