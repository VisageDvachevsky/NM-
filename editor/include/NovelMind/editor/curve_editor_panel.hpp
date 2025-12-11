/**
 * @file curve_editor_panel.hpp
 * @brief Curve Editor Panel for NovelMind Editor v0.2.0
 *
 * A standalone GUI panel for editing animation curves:
 * - Visual bezier curve editing with control points
 * - Preset curve library with preview
 * - Custom curve creation and saving
 * - Timeline integration
 * - Real-time animation preview
 */

#pragma once

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/editor/curve_editor.hpp"
#include <string>
#include <vector>
#include <memory>

namespace NovelMind::editor {

/**
 * @brief Curve Editor Panel - GUI wrapper for the curve editor
 *
 * This panel provides a complete curve editing interface:
 * - Main curve canvas with grid
 * - Preset browser sidebar
 * - Control point manipulation
 * - Curve preview animation
 * - Integration with timeline panel
 */
class CurveEditorPanel : public GUIPanelBase {
public:
    CurveEditorPanel();
    ~CurveEditorPanel() override = default;

    // =========================================================================
    // Curve Management
    // =========================================================================

    /**
     * @brief Set the curve to edit
     * @param curve Pointer to curve (panel does not own)
     */
    void setCurve(AnimationCurve* curve);

    /**
     * @brief Get current curve
     */
    [[nodiscard]] AnimationCurve* getCurve() const { return m_curve; }

    /**
     * @brief Create and edit a new curve
     */
    void createNewCurve();

    /**
     * @brief Load curve from library
     */
    void loadCurveFromLibrary(const std::string& curveId);

    /**
     * @brief Save current curve to library
     */
    void saveCurveToLibrary(const std::string& name, const std::string& category = "Custom");

    /**
     * @brief Apply preset curve
     */
    void applyPreset(const std::string& presetId);

    // =========================================================================
    // View Control
    // =========================================================================

    /**
     * @brief Set zoom level
     */
    void setZoom(f32 zoom);

    /**
     * @brief Get zoom level
     */
    [[nodiscard]] f32 getZoom() const { return m_zoom; }

    /**
     * @brief Fit curve to view
     */
    void fitToView();

    /**
     * @brief Reset view to default
     */
    void resetView();

    /**
     * @brief Set grid visibility
     */
    void setShowGrid(bool show) { m_showGrid = show; }

    /**
     * @brief Get grid visibility
     */
    [[nodiscard]] bool getShowGrid() const { return m_showGrid; }

    /**
     * @brief Set handles visibility
     */
    void setShowHandles(bool show) { m_showHandles = show; }

    /**
     * @brief Get handles visibility
     */
    [[nodiscard]] bool getShowHandles() const { return m_showHandles; }

    // =========================================================================
    // Preview
    // =========================================================================

    /**
     * @brief Start preview animation
     */
    void startPreview();

    /**
     * @brief Stop preview animation
     */
    void stopPreview();

    /**
     * @brief Toggle preview animation
     */
    void togglePreview();

    /**
     * @brief Check if preview is playing
     */
    [[nodiscard]] bool isPreviewPlaying() const { return m_previewPlaying; }

    /**
     * @brief Set preview speed
     */
    void setPreviewSpeed(f32 speed) { m_previewSpeed = speed; }

    /**
     * @brief Get preview speed
     */
    [[nodiscard]] f32 getPreviewSpeed() const { return m_previewSpeed; }

    // =========================================================================
    // Selection
    // =========================================================================

    /**
     * @brief Select point by index
     */
    void selectPoint(size_t index);

    /**
     * @brief Add point to selection
     */
    void addToSelection(size_t index);

    /**
     * @brief Clear selection
     */
    void clearSelection();

    /**
     * @brief Delete selected points
     */
    void deleteSelectedPoints();

    /**
     * @brief Get selected point indices
     */
    [[nodiscard]] const std::vector<size_t>& getSelectedPoints() const { return m_selectedPoints; }

    // =========================================================================
    // Panel Interface
    // =========================================================================

    [[nodiscard]] std::vector<MenuItem> getMenuItems() const override;
    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;
    [[nodiscard]] std::vector<MenuItem> getContextMenuItems() const override;

protected:
    void onInitialize() override;
    void onShutdown() override;
    void onUpdate(f64 deltaTime) override;
    void onRender() override;
    void renderToolbar() override;
    void renderMenuBar() override;
    void onResize(f32 width, f32 height) override;

private:
    // Rendering
    void renderCurveCanvas();
    void renderGrid();
    void renderCurve();
    void renderPoints();
    void renderHandles();
    void renderPreviewIndicator();
    void renderPresetsSidebar();
    void renderCurveInfo();

    // Input handling
    void handleInput();
    void handleMouseDown(f32 x, f32 y, bool shift, bool ctrl);
    void handleMouseUp();
    void handleMouseDrag(f32 x, f32 y, f32 dx, f32 dy);
    void handleMouseWheel(f32 delta);
    void handleKeyPress(i32 key);

    // Coordinate conversion
    struct Vec2 { f32 x, y; };
    Vec2 curveToScreen(f32 t, f32 v) const;
    Vec2 screenToCurve(f32 x, f32 y) const;

    // Hit testing
    i32 hitTestPoint(f32 x, f32 y) const;
    i32 hitTestHandle(f32 x, f32 y, bool& isInHandle) const;
    bool hitTestCurve(f32 x, f32 y) const;

    // Point manipulation
    void addPointAt(f32 t, f32 v);
    void moveSelectedPoints(f32 dt, f32 dv);
    void updateHandlePosition(size_t pointIndex, bool isInHandle, f32 dx, f32 dy);

    // Curve data
    AnimationCurve* m_curve = nullptr;
    std::unique_ptr<AnimationCurve> m_ownedCurve;  // For new curves
    CurveLibrary m_library;

    // View state
    f32 m_zoom = 1.0f;
    f32 m_panX = 0.0f;
    f32 m_panY = 0.0f;
    f32 m_canvasX = 0.0f;
    f32 m_canvasY = 0.0f;
    f32 m_canvasWidth = 400.0f;
    f32 m_canvasHeight = 300.0f;

    // Display options
    bool m_showGrid = true;
    bool m_showHandles = true;
    bool m_showPresets = true;
    bool m_showInfo = true;
    bool m_snapToGrid = false;
    f32 m_gridSnapSize = 0.1f;

    // Selection
    std::vector<size_t> m_selectedPoints;

    // Drag state
    enum class DragMode {
        None,
        Pan,
        Point,
        InHandle,
        OutHandle,
        BoxSelect
    };
    DragMode m_dragMode = DragMode::None;
    i32 m_dragPointIndex = -1;
    f32 m_dragStartX = 0.0f;
    f32 m_dragStartY = 0.0f;
    f32 m_boxSelectStartX = 0.0f;
    f32 m_boxSelectStartY = 0.0f;

    // Preview animation
    bool m_previewPlaying = false;
    f32 m_previewTime = 0.0f;
    f32 m_previewSpeed = 1.0f;

    // UI state
    char m_curveNameBuffer[64] = "New Curve";
    char m_categoryBuffer[32] = "Custom";
    i32 m_selectedPresetIndex = -1;
    std::string m_hoveredPreset;
    bool m_isDirty = false;
};

} // namespace NovelMind::editor
