#pragma once

/**
 * @file inspector_panel.hpp
 * @brief Inspector Panel for NovelMind Editor v0.2.0
 *
 * The Inspector panel displays and allows editing of properties for:
 * - Scene objects
 * - Story graph nodes
 * - Timeline keyframes
 * - Assets
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/editor/selection_system.hpp"
#include <string>
#include <vector>
#include <functional>
#include <variant>
#include <utility>

namespace NovelMind::editor {

/**
 * @brief Property type for inspector
 */
enum class PropertyType : u8 {
    Bool,
    Int,
    Float,
    String,
    Vector2,
    Vector3,
    Color,
    Enum,
    Asset,
    Custom
};

/**
 * @brief Property definition
 */
struct PropertyDefinition {
    PropertyDefinition(std::string nameValue = {},
                       std::string displayNameValue = {},
                       std::string categoryValue = {},
                       std::string tooltipValue = {},
                       PropertyType typeValue = PropertyType::String,
                       bool readOnlyValue = false,
                       std::vector<std::string> enumValuesValue = {},
                       std::string assetTypeValue = {},
                       f32 minValueValue = 0.0f,
                       f32 maxValueValue = 1.0f,
                       f32 stepValue = 0.1f)
        : name(std::move(nameValue)),
          displayName(std::move(displayNameValue)),
          category(std::move(categoryValue)),
          tooltip(std::move(tooltipValue)),
          type(typeValue),
          readOnly(readOnlyValue),
          enumValues(std::move(enumValuesValue)),
          assetType(std::move(assetTypeValue)),
          minValue(minValueValue),
          maxValue(maxValueValue),
          step(stepValue) {}

    std::string name;
    std::string displayName;
    std::string category;
    std::string tooltip;
    PropertyType type;
    bool readOnly;

    // For enum types
    std::vector<std::string> enumValues;

    // For asset types
    std::string assetType;

    // For numeric types
    f32 minValue;
    f32 maxValue;
    f32 step;
};

/**
 * @brief Inspector Panel implementation
 */
class InspectorPanel : public GUIPanelBase {
public:
    InspectorPanel();
    ~InspectorPanel() override = default;

    /**
     * @brief Refresh the inspector to reflect current selection
     */
    void refresh();

    /**
     * @brief Clear the inspector
     */
    void clear();

    /**
     * @brief Lock inspector to current object
     */
    void setLocked(bool locked) { m_isLocked = locked; }

    /**
     * @brief Check if inspector is locked
     */
    [[nodiscard]] bool isLocked() const { return m_isLocked; }

    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;

protected:
    void onInitialize() override;
    void onRender() override;
    void renderToolbar() override;

    void onSelectionChanged(SelectionType type,
                           const std::vector<SelectionItem>& selection) override;
    void onPrimarySelectionChanged(const SelectionItem& item) override;

private:
    void renderSceneObjectInspector(const SceneObjectSelection& selection);
    void renderGraphNodeInspector(const StoryGraphNodeSelection& selection);
    void renderTimelineItemInspector(const TimelineItemSelection& selection);
    void renderProperty(const std::string& name, const std::string& value,
                       const PropertyDefinition& def);

    bool m_isLocked = false;
    SelectionItem m_lockedItem;

    // Cached property definitions
    std::unordered_map<std::string, std::vector<PropertyDefinition>> m_propertyCache;
};

} // namespace NovelMind::editor
