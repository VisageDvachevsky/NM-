#pragma once

/**
 * @file hierarchy_panel.hpp
 * @brief Hierarchy Panel for NovelMind Editor v0.2.0
 *
 * The Hierarchy panel displays the scene object tree:
 * - Parent-child relationships
 * - Object visibility toggles
 * - Drag-drop reparenting
 * - Multi-selection support
 * - Search filtering
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include <string>
#include <vector>

namespace NovelMind::editor {

/**
 * @brief Scene hierarchy node
 */
struct HierarchyNode {
    std::string id;
    std::string name;
    std::string type;
    std::vector<HierarchyNode> children;
    bool visible = true;
    bool locked = false;
    bool expanded = true;
};

/**
 * @brief Hierarchy Panel implementation
 */
class HierarchyPanel : public GUIPanelBase {
public:
    HierarchyPanel();
    ~HierarchyPanel() override = default;

    /**
     * @brief Refresh hierarchy from scene
     */
    void refresh();

    /**
     * @brief Set search filter
     */
    void setFilter(const std::string& filter) { m_filter = filter; }

    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;
    [[nodiscard]] std::vector<MenuItem> getContextMenuItems() const override;

protected:
    void onInitialize() override;
    void onRender() override;
    void renderToolbar() override;

    void onSelectionChanged(SelectionType type,
                           const std::vector<SelectionItem>& selection) override;

private:
    void renderNode(const HierarchyNode& node, int depth = 0);
    void handleNodeSelection(const std::string& nodeId, bool ctrlHeld, bool shiftHeld);
    void handleDragDrop(const std::string& draggedId, const std::string& targetId);

    std::vector<HierarchyNode> m_rootNodes;
    std::string m_filter;
    char m_searchBuffer[256] = {0};

    // For drag-drop reparenting
    std::string m_draggedNodeId;
};

} // namespace NovelMind::editor
