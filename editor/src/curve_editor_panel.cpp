/**
 * @file curve_editor_panel.cpp
 * @brief Curve Editor Panel implementation
 */

#include "NovelMind/editor/curve_editor_panel.hpp"
#include "NovelMind/editor/imgui_integration.hpp"
#include "NovelMind/editor/style_guide.hpp"
#include <algorithm>
#include <cmath>

namespace NovelMind::editor {

CurveEditorPanel::CurveEditorPanel()
    : GUIPanelBase("Curve Editor")
{
    setFlags(PanelFlags::MenuBar);

    // Load preset curves into library
    m_library.loadPresets();
}

void CurveEditorPanel::onInitialize()
{
    // Create a default curve if none is set
    if (!m_curve) {
        createNewCurve();
    }
}

void CurveEditorPanel::onShutdown()
{
    m_ownedCurve.reset();
    m_curve = nullptr;
}

void CurveEditorPanel::onUpdate(f64 deltaTime)
{
    // Update preview animation
    if (m_previewPlaying && m_curve) {
        m_previewTime += static_cast<f32>(deltaTime) * m_previewSpeed;
        if (m_previewTime > 1.0f) {
            m_previewTime = 0.0f;
        }
    }
}

void CurveEditorPanel::onRender()
{
    // Main layout: sidebar + canvas
    f32 sidebarWidth = m_showPresets ? 200.0f : 0.0f;
    (void)sidebarWidth; // Reserved for layout calculations
    f32 infoHeight = m_showInfo ? 80.0f : 0.0f;
    (void)infoHeight; // Reserved for layout calculations

    // Presets sidebar
    if (m_showPresets) {
        renderPresetsSidebar();
    }

    // Curve canvas (main area)
    renderCurveCanvas();

    // Info panel at bottom
    if (m_showInfo) {
        renderCurveInfo();
    }
}

void CurveEditorPanel::renderToolbar()
{
    widgets::BeginToolbar("CurveEditorToolbar", Heights::Toolbar);

    // New curve
    if (widgets::ToolbarButton(Icons::New, "New Curve")) {
        createNewCurve();
    }

    widgets::ToolbarSeparator();

    // View controls
    if (widgets::ToolbarButton(Icons::ZoomFit, "Fit to View")) {
        fitToView();
    }
    if (widgets::ToolbarButton(Icons::ZoomReset, "Reset View")) {
        resetView();
    }

    widgets::ToolbarSeparator();

    // Display toggles
    widgets::ToolbarToggle(Icons::Grid, &m_showGrid, "Show Grid");
    widgets::ToolbarToggle(Icons::Visible, &m_showHandles, "Show Handles");

    widgets::ToolbarSeparator();

    // Snap toggle
    widgets::ToolbarToggle(Icons::Transform, &m_snapToGrid, "Snap to Grid");

    widgets::ToolbarSeparator();

    // Preview controls
    if (m_previewPlaying) {
        if (widgets::ToolbarButton(Icons::Pause, "Pause Preview")) {
            stopPreview();
        }
    } else {
        if (widgets::ToolbarButton(Icons::Play, "Start Preview")) {
            startPreview();
        }
    }

    widgets::EndToolbar();
}

void CurveEditorPanel::renderMenuBar()
{
    // File menu would be rendered here
}

void CurveEditorPanel::onResize(f32 width, f32 height)
{
    m_canvasWidth = width - (m_showPresets ? 200.0f : 0.0f);
    m_canvasHeight = height - Heights::Toolbar - (m_showInfo ? 80.0f : 0.0f);
}

void CurveEditorPanel::renderCurveCanvas()
{
    // Canvas background
    // ImGui::BeginChild would go here

    // Render layers
    if (m_showGrid) {
        renderGrid();
    }

    if (m_curve) {
        renderCurve();
        renderPoints();

        if (m_showHandles) {
            renderHandles();
        }
    }

    if (m_previewPlaying) {
        renderPreviewIndicator();
    }

    // Handle input
    handleInput();
}

void CurveEditorPanel::renderGrid()
{
    // Grid rendering would use ImGui draw lists
    // Draws horizontal and vertical lines at regular intervals

    const auto& theme = ImGuiLayer::instance().getCurrentTheme();
    (void)theme; // Reserved for grid rendering colors

    // Minor grid lines
    for (i32 i = 0; i <= 10; ++i) {
        f32 t = static_cast<f32>(i) / 10.0f;
        (void)t; // Reserved for grid line positioning
        // Draw vertical line at curveToScreen(t, 0) to curveToScreen(t, 1)
        // Draw horizontal line at curveToScreen(0, t) to curveToScreen(1, t)
    }

    // Major grid lines (at 0, 0.5, 1)
    // Draw with brighter color
}

void CurveEditorPanel::renderCurve()
{
    if (!m_curve) return;

    // Sample curve and draw as connected line segments
    auto samples = m_curve->sample(100);

    const auto& theme = ImGuiLayer::instance().getCurrentTheme();
    (void)theme; // Reserved for curve rendering colors

    for (size_t i = 1; i < samples.size(); ++i) {
        auto p0 = curveToScreen(samples[i - 1].x, samples[i - 1].y);
        (void)p0; // Reserved for line rendering
        auto p1 = curveToScreen(samples[i].x, samples[i].y);
        (void)p1; // Reserved for line rendering
        // Draw line from p0 to p1 with curveColor
    }
}

void CurveEditorPanel::renderPoints()
{
    if (!m_curve) return;

    const auto& points = m_curve->getPoints();
    const auto& theme = ImGuiLayer::instance().getCurrentTheme();
    (void)theme; // Reserved for point rendering colors

    for (size_t i = 0; i < points.size(); ++i) {
        const auto& pt = points[i];
        auto screenPos = curveToScreen(pt.time, pt.value);
        (void)screenPos; // Reserved for point rendering

        bool isSelected = std::find(m_selectedPoints.begin(), m_selectedPoints.end(), i)
                          != m_selectedPoints.end();

        // Draw point circle
        // Selected points are larger and highlighted
        f32 radius = isSelected ? 7.0f : 5.0f;
        (void)radius; // Reserved for point circle rendering
        // Draw filled circle at screenPos
    }
}

void CurveEditorPanel::renderHandles()
{
    if (!m_curve) return;

    const auto& points = m_curve->getPoints();

    for (size_t i = 0; i < points.size(); ++i) {
        const auto& pt = points[i];

        bool isSelected = std::find(m_selectedPoints.begin(), m_selectedPoints.end(), i)
                          != m_selectedPoints.end();

        if (!isSelected) continue;

        auto pointPos = curveToScreen(pt.time, pt.value);
        (void)pointPos; // Reserved for handle rendering

        // In handle
        if (i > 0) {
            auto inHandlePos = curveToScreen(
                pt.time + pt.inHandleX,
                pt.value + pt.inHandleY
            );
            (void)inHandlePos; // Reserved for in handle rendering
            // Draw line from pointPos to inHandlePos
            // Draw small circle at inHandlePos
        }

        // Out handle
        if (i < points.size() - 1) {
            auto outHandlePos = curveToScreen(
                pt.time + pt.outHandleX,
                pt.value + pt.outHandleY
            );
            (void)outHandlePos; // Reserved for out handle rendering
            // Draw line from pointPos to outHandlePos
            // Draw small circle at outHandlePos
        }
    }
}

void CurveEditorPanel::renderPreviewIndicator()
{
    if (!m_curve) return;

    f32 value = m_curve->evaluate(m_previewTime);
    auto pos = curveToScreen(m_previewTime, value);
    (void)pos; // Reserved for preview indicator rendering

    // Draw vertical line at current preview time
    // Draw circle at current value on curve
}

void CurveEditorPanel::renderPresetsSidebar()
{
    // Sidebar with preset curve list
    // Each preset shows a mini preview

    auto presetIds = m_library.getPresetIds();

    for (const auto& id : presetIds) {
        const auto* preset = m_library.getCurve(id);
        (void)preset; // Reserved for preset display
        if (!preset) continue;

        bool isSelected = (m_hoveredPreset == id);

        // Draw preset item with mini curve preview
        // On click, apply preset

        if (isSelected) {
            // Highlight
        }

        // Mini curve preview (small sampled version)
    }
}

void CurveEditorPanel::renderCurveInfo()
{
    // Info panel showing:
    // - Curve name
    // - Number of points
    // - Selected point info (time, value, handles)

    if (!m_curve) return;

    // Curve name field
    // strncpy_s(m_curveNameBuffer, m_curve->getName().c_str(), sizeof(m_curveNameBuffer));
    // ImGui::InputText("Name", m_curveNameBuffer, sizeof(m_curveNameBuffer));

    // Point count
    // ImGui::Text("Points: %zu", m_curve->getPointCount());

    // Selected point details
    if (!m_selectedPoints.empty()) {
        size_t idx = m_selectedPoints[0];
        const auto& points = m_curve->getPoints();
        if (idx < points.size()) {
            // Show editable fields for time, value, handle positions
        }
    }
}

void CurveEditorPanel::handleInput()
{
    // Mouse position would be obtained from ImGui
    // f32 mouseX = ImGui::GetMousePos().x - canvasX;
    // f32 mouseY = ImGui::GetMousePos().y - canvasY;

    // Handle different drag modes
    switch (m_dragMode) {
        case DragMode::None:
            // Check for click on point/handle/curve
            break;

        case DragMode::Pan:
            // Update pan offset
            break;

        case DragMode::Point:
            // Move selected point
            break;

        case DragMode::InHandle:
        case DragMode::OutHandle:
            // Move handle
            break;

        case DragMode::BoxSelect:
            // Update selection box
            break;
    }
}

CurveEditorPanel::Vec2 CurveEditorPanel::curveToScreen(f32 t, f32 v) const
{
    // Transform from curve space (0-1, 0-1) to screen space
    f32 margin = 20.0f;
    f32 usableWidth = m_canvasWidth - 2 * margin;
    f32 usableHeight = m_canvasHeight - 2 * margin;

    f32 x = m_canvasX + margin + (t - m_panX) * m_zoom * usableWidth;
    f32 y = m_canvasY + margin + (1.0f - v + m_panY) * m_zoom * usableHeight;  // Y is flipped

    return {x, y};
}

CurveEditorPanel::Vec2 CurveEditorPanel::screenToCurve(f32 x, f32 y) const
{
    // Transform from screen space to curve space
    f32 margin = 20.0f;
    f32 usableWidth = m_canvasWidth - 2 * margin;
    f32 usableHeight = m_canvasHeight - 2 * margin;

    f32 t = (x - m_canvasX - margin) / (m_zoom * usableWidth) + m_panX;
    f32 v = 1.0f - (y - m_canvasY - margin) / (m_zoom * usableHeight) - m_panY;

    return {t, v};
}

i32 CurveEditorPanel::hitTestPoint(f32 x, f32 y) const
{
    if (!m_curve) return -1;

    const f32 hitRadius = 8.0f;
    const auto& points = m_curve->getPoints();

    for (size_t i = 0; i < points.size(); ++i) {
        auto screenPos = curveToScreen(points[i].time, points[i].value);
        f32 dx = x - screenPos.x;
        f32 dy = y - screenPos.y;
        if (dx * dx + dy * dy <= hitRadius * hitRadius) {
            return static_cast<i32>(i);
        }
    }

    return -1;
}

i32 CurveEditorPanel::hitTestHandle(f32 x, f32 y, bool& isInHandle) const
{
    if (!m_curve) return -1;

    const f32 hitRadius = 6.0f;
    const auto& points = m_curve->getPoints();

    for (size_t i = 0; i < points.size(); ++i) {
        const auto& pt = points[i];

        // Check in handle
        if (i > 0) {
            auto handlePos = curveToScreen(pt.time + pt.inHandleX, pt.value + pt.inHandleY);
            f32 dx = x - handlePos.x;
            f32 dy = y - handlePos.y;
            if (dx * dx + dy * dy <= hitRadius * hitRadius) {
                isInHandle = true;
                return static_cast<i32>(i);
            }
        }

        // Check out handle
        if (i < points.size() - 1) {
            auto handlePos = curveToScreen(pt.time + pt.outHandleX, pt.value + pt.outHandleY);
            f32 dx = x - handlePos.x;
            f32 dy = y - handlePos.y;
            if (dx * dx + dy * dy <= hitRadius * hitRadius) {
                isInHandle = false;
                return static_cast<i32>(i);
            }
        }
    }

    return -1;
}

void CurveEditorPanel::setCurve(AnimationCurve* curve)
{
    m_curve = curve;
    m_ownedCurve.reset();
    clearSelection();
    m_isDirty = false;
}

void CurveEditorPanel::createNewCurve()
{
    m_ownedCurve = std::make_unique<AnimationCurve>("New Curve");

    // Add default points (linear)
    CurvePoint start;
    start.time = 0.0f;
    start.value = 0.0f;
    start.outHandleX = 0.33f;
    start.outHandleY = 0.0f;
    m_ownedCurve->addPoint(start);

    CurvePoint end;
    end.time = 1.0f;
    end.value = 1.0f;
    end.inHandleX = -0.33f;
    end.inHandleY = 0.0f;
    m_ownedCurve->addPoint(end);

    m_curve = m_ownedCurve.get();
    clearSelection();
    m_isDirty = false;
}

void CurveEditorPanel::loadCurveFromLibrary(const std::string& curveId)
{
    const auto* libCurve = m_library.getCurve(curveId);
    if (libCurve) {
        m_ownedCurve = std::make_unique<AnimationCurve>(*libCurve);
        m_curve = m_ownedCurve.get();
        clearSelection();
        m_isDirty = false;
    }
}

void CurveEditorPanel::saveCurveToLibrary(const std::string& name, const std::string& category)
{
    if (!m_curve) return;

    AnimationCurve curveToSave = *m_curve;
    curveToSave.setName(name);
    m_library.addCurve(curveToSave);
    m_library.setCurveCategory(curveToSave.getId(), category);
    m_isDirty = false;
}

void CurveEditorPanel::applyPreset(const std::string& presetId)
{
    loadCurveFromLibrary(presetId);
}

void CurveEditorPanel::setZoom(f32 zoom)
{
    m_zoom = std::clamp(zoom, 0.1f, 10.0f);
}

void CurveEditorPanel::fitToView()
{
    m_zoom = 1.0f;
    m_panX = 0.0f;
    m_panY = 0.0f;
}

void CurveEditorPanel::resetView()
{
    fitToView();
}

void CurveEditorPanel::startPreview()
{
    m_previewPlaying = true;
    m_previewTime = 0.0f;
}

void CurveEditorPanel::stopPreview()
{
    m_previewPlaying = false;
}

void CurveEditorPanel::togglePreview()
{
    if (m_previewPlaying) {
        stopPreview();
    } else {
        startPreview();
    }
}

void CurveEditorPanel::selectPoint(size_t index)
{
    m_selectedPoints.clear();
    m_selectedPoints.push_back(index);
}

void CurveEditorPanel::addToSelection(size_t index)
{
    if (std::find(m_selectedPoints.begin(), m_selectedPoints.end(), index) == m_selectedPoints.end()) {
        m_selectedPoints.push_back(index);
    }
}

void CurveEditorPanel::clearSelection()
{
    m_selectedPoints.clear();
}

void CurveEditorPanel::deleteSelectedPoints()
{
    if (!m_curve || m_selectedPoints.empty()) return;

    // Sort indices in descending order to safely remove
    std::sort(m_selectedPoints.begin(), m_selectedPoints.end(), std::greater<size_t>());

    // Don't delete first or last point
    for (size_t idx : m_selectedPoints) {
        if (idx > 0 && idx < m_curve->getPointCount() - 1) {
            m_curve->removePoint(idx);
            m_isDirty = true;
        }
    }

    clearSelection();
}

std::vector<MenuItem> CurveEditorPanel::getMenuItems() const
{
    return {
        {"New Curve", "Ctrl+N", [this]() { const_cast<CurveEditorPanel*>(this)->createNewCurve(); }},
        MenuItem::separator(),
        {"Reset View", "", [this]() { const_cast<CurveEditorPanel*>(this)->resetView(); }},
        {"Fit to View", "F", [this]() { const_cast<CurveEditorPanel*>(this)->fitToView(); }},
        MenuItem::separator(),
        {"Show Grid", "", [this]() { /* toggle */ }, []() { return true; },
         [this]() { return m_showGrid; }},
        {"Show Handles", "", [this]() { /* toggle */ }, []() { return true; },
         [this]() { return m_showHandles; }}
    };
}

std::vector<ToolbarItem> CurveEditorPanel::getToolbarItems() const
{
    return {
        {Icons::New, "New Curve", [this]() { const_cast<CurveEditorPanel*>(this)->createNewCurve(); }},
        ToolbarItem::separator(),
        {Icons::ZoomFit, "Fit to View", [this]() { const_cast<CurveEditorPanel*>(this)->fitToView(); }},
        {Icons::ZoomReset, "Reset View", [this]() { const_cast<CurveEditorPanel*>(this)->resetView(); }},
        ToolbarItem::separator(),
        {Icons::Play, "Preview", [this]() { const_cast<CurveEditorPanel*>(this)->togglePreview(); },
         []() { return true; }, [this]() { return m_previewPlaying; }}
    };
}

std::vector<MenuItem> CurveEditorPanel::getContextMenuItems() const
{
    std::vector<MenuItem> items;

    if (!m_selectedPoints.empty()) {
        items.push_back({"Delete Point", "Delete",
            [this]() { const_cast<CurveEditorPanel*>(this)->deleteSelectedPoints(); },
            [this]() { return m_selectedPoints.size() > 0; }, {}});
        items.push_back(MenuItem::separator());
    }

    items.push_back({"Add Point", "",
        [this]() { /* Add point at right-click location */ }, nullptr, {}});

    items.push_back(MenuItem::separator());

    items.push_back({"Reset View", "",
        [this]() { const_cast<CurveEditorPanel*>(this)->resetView(); }, nullptr, {}});

    return items;
}

} // namespace NovelMind::editor
