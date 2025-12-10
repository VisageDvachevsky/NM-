/**
 * @file hierarchy_panel.cpp
 * @brief Hierarchy Panel implementation
 */

#include "NovelMind/editor/hierarchy_panel.hpp"
#include "NovelMind/editor/imgui_integration.hpp"

namespace NovelMind::editor {

HierarchyPanel::HierarchyPanel()
    : GUIPanelBase("Hierarchy")
{
}

void HierarchyPanel::refresh()
{
    // Would reload from scene manager
    m_rootNodes.clear();

    // Placeholder data
    HierarchyNode background;
    background.id = "bg_001";
    background.name = "Background";
    background.type = "Background";

    HierarchyNode character1;
    character1.id = "char_001";
    character1.name = "Character: Alice";
    character1.type = "CharacterSprite";

    HierarchyNode character2;
    character2.id = "char_002";
    character2.name = "Character: Bob";
    character2.type = "CharacterSprite";

    HierarchyNode ui;
    ui.id = "ui_001";
    ui.name = "UI";
    ui.type = "Container";

    HierarchyNode dialogueBox;
    dialogueBox.id = "dlg_001";
    dialogueBox.name = "Dialogue Box";
    dialogueBox.type = "DialogueBox";
    ui.children.push_back(dialogueBox);

    HierarchyNode choiceMenu;
    choiceMenu.id = "choice_001";
    choiceMenu.name = "Choice Menu";
    choiceMenu.type = "ChoiceMenu";
    choiceMenu.visible = false;
    ui.children.push_back(choiceMenu);

    m_rootNodes.push_back(background);
    m_rootNodes.push_back(character1);
    m_rootNodes.push_back(character2);
    m_rootNodes.push_back(ui);
}

std::vector<ToolbarItem> HierarchyPanel::getToolbarItems() const
{
    std::vector<ToolbarItem> items;

    items.push_back({"+", "Create Object", []() { /* Create */ }});
    items.push_back({"Refresh", "Refresh Hierarchy", [this]() { const_cast<HierarchyPanel*>(this)->refresh(); }});

    return items;
}

std::vector<MenuItem> HierarchyPanel::getContextMenuItems() const
{
    std::vector<MenuItem> items;

    items.push_back({"Create Empty", "", []() { /* Create empty */ }});
    items.push_back({"Create Character", "", []() { /* Create character */ }});
    items.push_back({"Create Background", "", []() { /* Create background */ }});
    items.push_back({"Create UI Element", "", []() { /* Create UI */ }});
    items.push_back(MenuItem::separator());
    items.push_back({"Duplicate", "Ctrl+D", []() { /* Duplicate */ }});
    items.push_back({"Delete", "Delete", []() { /* Delete */ }});
    items.push_back(MenuItem::separator());
    items.push_back({"Rename", "F2", []() { /* Rename */ }});

    return items;
}

void HierarchyPanel::onInitialize()
{
    refresh();

    subscribeEvent<SceneObjectEvent>(
        [this](const SceneObjectEvent& /*event*/) {
            refresh();
        });
}

void HierarchyPanel::onRender()
{
    // Search bar
    // ImGui::SetNextItemWidth(-1);
    if (widgets::SearchInput("##HierarchySearch", m_searchBuffer, sizeof(m_searchBuffer), "Search..."))
    {
        m_filter = m_searchBuffer;
    }

    // ImGui::Separator();

    // Render tree
    for (const auto& node : m_rootNodes)
    {
        renderNode(node);
    }

    // Context menu
    // if (ImGui::BeginPopupContextWindow())
    // {
    //     renderMenuItems(getContextMenuItems());
    //     ImGui::EndPopup();
    // }
}

void HierarchyPanel::renderToolbar()
{
    widgets::BeginToolbar("HierarchyToolbar");
    renderToolbarItems(getToolbarItems());
    widgets::EndToolbar();
}

void HierarchyPanel::onSelectionChanged(SelectionType type,
                                        const std::vector<SelectionItem>& /*selection*/)
{
    if (type == SelectionType::SceneObject)
    {
        // Scroll to selected item
    }
}

void HierarchyPanel::renderNode(const HierarchyNode& node, int depth)
{
    // Apply filter
    if (!m_filter.empty())
    {
        std::string nameLower = node.name;
        std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        std::string filterLower = m_filter;
        std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);

        bool matchesFilter = nameLower.find(filterLower) != std::string::npos;
        bool childMatches = false;

        for (const auto& child : node.children)
        {
            std::string childNameLower = child.name;
            std::transform(childNameLower.begin(), childNameLower.end(), childNameLower.begin(), ::tolower);
            if (childNameLower.find(filterLower) != std::string::npos)
            {
                childMatches = true;
                break;
            }
        }

        if (!matchesFilter && !childMatches)
        {
            return;
        }
    }

    bool isSelected = getSelection().isObjectSelected(node.id);
    bool isLeaf = node.children.empty();

    // Visibility toggle
    bool visible = node.visible;
    // ImGui::PushID(node.id.c_str());
    // if (ImGui::Checkbox("##vis", &visible))
    // {
    //     // Toggle visibility
    // }
    // ImGui::SameLine();

    // Tree node
    bool isExpanded = widgets::TreeNode(node.name.c_str(), isLeaf, isSelected,
                                        "HIERARCHY_NODE", (void*)node.id.c_str());

    // Handle selection
    // if (ImGui::IsItemClicked())
    // {
    //     handleNodeSelection(node.id, ImGui::GetIO().KeyCtrl, ImGui::GetIO().KeyShift);
    // }

    // Drag-drop source
    // if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    // {
    //     m_draggedNodeId = node.id;
    //     ImGui::SetDragDropPayload("HIERARCHY_NODE", node.id.c_str(), node.id.size() + 1);
    //     ImGui::Text("%s", node.name.c_str());
    //     ImGui::EndDragDropSource();
    // }

    // Drag-drop target
    // if (ImGui::BeginDragDropTarget())
    // {
    //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_NODE"))
    //     {
    //         handleDragDrop(m_draggedNodeId, node.id);
    //     }
    //     ImGui::EndDragDropTarget();
    // }

    // Render children
    if (isExpanded && !isLeaf)
    {
        for (const auto& child : node.children)
        {
            renderNode(child, depth + 1);
        }
        // ImGui::TreePop();
    }

    // ImGui::PopID();

    (void)visible;
    (void)depth;
}

void HierarchyPanel::handleNodeSelection(const std::string& nodeId, bool ctrlHeld, bool shiftHeld)
{
    if (ctrlHeld)
    {
        // Toggle selection
        if (getSelection().isObjectSelected(nodeId))
        {
            getSelection().removeFromSelection(SelectionItem(nodeId));
        }
        else
        {
            getSelection().addToSelection(SelectionItem(nodeId));
        }
    }
    else if (shiftHeld)
    {
        // Range selection (would need to implement)
        getSelection().addToSelection(SelectionItem(nodeId));
    }
    else
    {
        // Single selection
        getSelection().selectObject(nodeId);
    }
}

void HierarchyPanel::handleDragDrop(const std::string& draggedId, const std::string& targetId)
{
    if (draggedId == targetId)
    {
        return;
    }

    // Would reparent the dragged object under the target
    // This requires scene manager integration

    SceneObjectEvent event(EditorEventType::SceneLayerChanged);
    event.objectId = draggedId;
    publishEvent(event);
}

} // namespace NovelMind::editor
