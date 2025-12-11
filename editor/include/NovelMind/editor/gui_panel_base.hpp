#pragma once

/**
 * @file gui_panel_base.hpp
 * @brief Base class for all GUI panels in NovelMind Editor v0.2.0
 *
 * Provides common functionality for editor panels:
 * - Panel lifecycle management (init, update, render, shutdown)
 * - Visibility and focus state management
 * - Integration with event bus for inter-panel communication
 * - Integration with selection system
 * - Undo/Redo support
 * - Common toolbar and menu patterns
 */

#include "NovelMind/core/types.hpp"
#include "NovelMind/editor/event_bus.hpp"
#include "NovelMind/editor/selection_system.hpp"
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <utility>

namespace NovelMind::editor {

// Forward declarations
class EditorApp;
class ImGuiLayer;

/**
 * @brief Panel flags for configuration
 */
enum class PanelFlags : u32 {
    None = 0,
    NoTitleBar = 1 << 0,
    NoResize = 1 << 1,
    NoMove = 1 << 2,
    NoScrollbar = 1 << 3,
    NoScrollWithMouse = 1 << 4,
    NoCollapse = 1 << 5,
    AlwaysAutoResize = 1 << 6,
    NoBackground = 1 << 7,
    NoSavedSettings = 1 << 8,
    NoMouseInputs = 1 << 9,
    MenuBar = 1 << 10,
    HorizontalScrollbar = 1 << 11,
    NoFocusOnAppearing = 1 << 12,
    NoBringToFrontOnFocus = 1 << 13,
    AlwaysVerticalScrollbar = 1 << 14,
    AlwaysHorizontalScrollbar = 1 << 15,
    NoNavInputs = 1 << 16,
    NoNavFocus = 1 << 17,
    UnsavedDocument = 1 << 18,
    NoDocking = 1 << 19,
};

inline PanelFlags operator|(PanelFlags a, PanelFlags b) {
    return static_cast<PanelFlags>(static_cast<u32>(a) | static_cast<u32>(b));
}

inline PanelFlags operator&(PanelFlags a, PanelFlags b) {
    return static_cast<PanelFlags>(static_cast<u32>(a) & static_cast<u32>(b));
}

inline bool hasFlag(PanelFlags flags, PanelFlags flag) {
    return (static_cast<u32>(flags) & static_cast<u32>(flag)) != 0;
}

/**
 * @brief Menu item definition for panel menus
 */
struct MenuItem {
    MenuItem(std::string labelText = {},
             std::string shortcutText = {},
             std::function<void()> actionFn = {},
             std::function<bool()> enabledFn = []() { return true; },
             std::function<bool()> checkedFn = nullptr,
             bool separator = false,
             std::vector<MenuItem> children = {})
        : label(std::move(labelText)),
          shortcut(std::move(shortcutText)),
          action(std::move(actionFn)),
          isEnabled(std::move(enabledFn)),
          isChecked(std::move(checkedFn)),
          isSeparator(separator),
          subItems(std::move(children)) {}

    std::string label;
    std::string shortcut;
    std::function<void()> action;
    std::function<bool()> isEnabled;
    std::function<bool()> isChecked;
    bool isSeparator;
    std::vector<MenuItem> subItems;

    static MenuItem separator() {
        return MenuItem{"", "", {}, []() { return true; }, nullptr, true, {}};
    }
};

/**
 * @brief Toolbar button definition
 */
struct ToolbarItem {
    std::string icon;
    std::string tooltip;
    std::function<void()> action;
    std::function<bool()> isEnabled = []() { return true; };
    std::function<bool()> isToggled = nullptr;
    bool isSeparator = false;

    static ToolbarItem separator() {
        ToolbarItem item;
        item.isSeparator = true;
        return item;
    }
};

/**
 * @brief Base class for all editor panels
 */
class GUIPanelBase : public ISelectionListener {
public:
    explicit GUIPanelBase(const std::string& name);
    virtual ~GUIPanelBase();

    // Prevent copying
    GUIPanelBase(const GUIPanelBase&) = delete;
    GUIPanelBase& operator=(const GUIPanelBase&) = delete;

    // =========================================================================
    // Lifecycle
    // =========================================================================

    /**
     * @brief Initialize the panel
     * @param app Reference to the editor app
     */
    virtual void initialize(EditorApp* app);

    /**
     * @brief Shutdown the panel
     */
    virtual void shutdown();

    /**
     * @brief Update panel logic (called every frame)
     * @param deltaTime Time since last frame
     */
    virtual void update(f64 deltaTime);

    /**
     * @brief Render the panel
     */
    virtual void render();

    // =========================================================================
    // State Management
    // =========================================================================

    /**
     * @brief Get panel name/title
     */
    [[nodiscard]] const std::string& getName() const { return m_name; }

    /**
     * @brief Set panel title (displayed in title bar)
     */
    void setTitle(const std::string& title) { m_title = title; }

    /**
     * @brief Get panel title
     */
    [[nodiscard]] const std::string& getTitle() const { return m_title.empty() ? m_name : m_title; }

    /**
     * @brief Check if panel is open/visible
     */
    [[nodiscard]] bool isOpen() const { return m_isOpen; }

    /**
     * @brief Open/show the panel
     */
    void open() { m_isOpen = true; }

    /**
     * @brief Close/hide the panel
     */
    void close() { m_isOpen = false; }

    /**
     * @brief Toggle panel visibility
     */
    void toggle() { m_isOpen = !m_isOpen; }

    /**
     * @brief Set panel open state
     */
    void setOpen(bool open) { m_isOpen = open; }

    /**
     * @brief Check if panel is focused
     */
    [[nodiscard]] bool isFocused() const { return m_isFocused; }

    /**
     * @brief Check if panel is hovered
     */
    [[nodiscard]] bool isHovered() const { return m_isHovered; }

    /**
     * @brief Request focus for this panel
     */
    void requestFocus();

    /**
     * @brief Get panel flags
     */
    [[nodiscard]] PanelFlags getFlags() const { return m_flags; }

    /**
     * @brief Set panel flags
     */
    void setFlags(PanelFlags flags) { m_flags = flags; }

    // =========================================================================
    // Selection Listener Interface
    // =========================================================================

    void onSelectionChanged(SelectionType type,
                           const std::vector<SelectionItem>& selection) override;
    void onSelectionCleared() override;
    void onPrimarySelectionChanged(const SelectionItem& item) override;

    // =========================================================================
    // Menu and Toolbar
    // =========================================================================

    /**
     * @brief Get panel-specific menu items (for View menu)
     */
    [[nodiscard]] virtual std::vector<MenuItem> getMenuItems() const { return {}; }

    /**
     * @brief Get panel-specific toolbar items
     */
    [[nodiscard]] virtual std::vector<ToolbarItem> getToolbarItems() const { return {}; }

    /**
     * @brief Get context menu items
     */
    [[nodiscard]] virtual std::vector<MenuItem> getContextMenuItems() const { return {}; }

protected:
    // =========================================================================
    // Overridable Methods
    // =========================================================================

    /**
     * @brief Called during initialize() - override to perform custom init
     */
    virtual void onInitialize() {}

    /**
     * @brief Called during shutdown() - override to perform custom cleanup
     */
    virtual void onShutdown() {}

    /**
     * @brief Called during update() - override to update panel logic
     */
    virtual void onUpdate(f64 /*deltaTime*/) {}

    /**
     * @brief Called during render() - override to render panel content
     */
    virtual void onRender() {}

    /**
     * @brief Called when panel gains focus
     */
    virtual void onFocusGained() {}

    /**
     * @brief Called when panel loses focus
     */
    virtual void onFocusLost() {}

    /**
     * @brief Called when panel is resized
     */
    virtual void onResize(f32 /*width*/, f32 /*height*/) {}

    /**
     * @brief Render panel menu bar (if MenuBar flag is set)
     */
    virtual void renderMenuBar() {}

    /**
     * @brief Render panel toolbar
     */
    virtual void renderToolbar() {}

    /**
     * @brief Render panel footer/status bar
     */
    virtual void renderFooter() {}

    // =========================================================================
    // Utility Methods
    // =========================================================================

    /**
     * @brief Begin panel rendering
     * @return true if panel content should be rendered
     */
    bool beginPanel();

    /**
     * @brief End panel rendering
     */
    void endPanel();

    /**
     * @brief Render menu items
     */
    void renderMenuItems(const std::vector<MenuItem>& items);

    /**
     * @brief Render toolbar items
     */
    void renderToolbarItems(const std::vector<ToolbarItem>& items);

    /**
     * @brief Subscribe to an event type
     */
    template<typename EventType>
    void subscribeEvent(std::function<void(const EventType&)> handler) {
        auto sub = EventBus::instance().subscribe<EventType>(std::move(handler));
        m_eventSubscriptions.push_back(
            ScopedEventSubscription(&EventBus::instance(), sub));
    }

    /**
     * @brief Publish an event
     */
    template<typename EventType>
    void publishEvent(EventType event) {
        event.source = m_name;
        EventBus::instance().publish(event);
    }

    /**
     * @brief Get editor app reference
     */
    EditorApp* getApp() const { return m_app; }

    /**
     * @brief Get selection manager
     */
    EditorSelectionManager& getSelection() const {
        return EditorSelectionManager::instance();
    }

    /**
     * @brief Get current panel width
     */
    [[nodiscard]] f32 getPanelWidth() const { return m_width; }

    /**
     * @brief Get current panel height
     */
    [[nodiscard]] f32 getPanelHeight() const { return m_height; }

    /**
     * @brief Get panel content region width (excluding padding)
     */
    [[nodiscard]] f32 getContentWidth() const { return m_contentWidth; }

    /**
     * @brief Get panel content region height (excluding padding)
     */
    [[nodiscard]] f32 getContentHeight() const { return m_contentHeight; }

protected:
    std::string m_name;
    std::string m_title;
    bool m_isOpen = true;
    bool m_isFocused = false;
    bool m_isHovered = false;
    bool m_initialized = false;
    PanelFlags m_flags = PanelFlags::None;

    f32 m_width = 0.0f;
    f32 m_height = 0.0f;
    f32 m_contentWidth = 0.0f;
    f32 m_contentHeight = 0.0f;

    EditorApp* m_app = nullptr;
    std::vector<ScopedEventSubscription> m_eventSubscriptions;
};

/**
 * @brief Panel manager for coordinating all panels
 */
class PanelManager {
public:
    PanelManager();
    ~PanelManager();

    /**
     * @brief Register a panel
     */
    void registerPanel(std::unique_ptr<GUIPanelBase> panel);

    /**
     * @brief Get a panel by name
     */
    GUIPanelBase* getPanel(const std::string& name);

    /**
     * @brief Get all panels
     */
    [[nodiscard]] const std::vector<std::unique_ptr<GUIPanelBase>>& getPanels() const {
        return m_panels;
    }

    /**
     * @brief Initialize all panels
     */
    void initializeAll(EditorApp* app);

    /**
     * @brief Shutdown all panels
     */
    void shutdownAll();

    /**
     * @brief Update all panels
     */
    void updateAll(f64 deltaTime);

    /**
     * @brief Render all panels
     */
    void renderAll();

    /**
     * @brief Get currently focused panel
     */
    [[nodiscard]] GUIPanelBase* getFocusedPanel() const { return m_focusedPanel; }

    /**
     * @brief Set focused panel
     */
    void setFocusedPanel(GUIPanelBase* panel);

private:
    std::vector<std::unique_ptr<GUIPanelBase>> m_panels;
    GUIPanelBase* m_focusedPanel = nullptr;
};

} // namespace NovelMind::editor
