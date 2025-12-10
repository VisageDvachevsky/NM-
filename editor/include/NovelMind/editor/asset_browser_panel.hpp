#pragma once

/**
 * @file asset_browser_panel.hpp
 * @brief Asset Browser Panel for NovelMind Editor v0.2.0
 *
 * The Asset Browser panel provides:
 * - File system navigation for project assets
 * - Asset preview and metadata display
 * - Drag-drop to scene/timeline
 * - Asset import/export
 * - Search and filtering
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace NovelMind::editor {

/**
 * @brief Asset types
 */
enum class AssetType : u8 {
    Unknown,
    Image,
    Audio,
    Script,
    Scene,
    Character,
    Background,
    Font,
    Video,
    Data
};

/**
 * @brief Asset entry in browser
 */
struct AssetEntry {
    std::string name;
    std::string path;
    std::string extension;
    AssetType type = AssetType::Unknown;
    bool isDirectory = false;
    u64 size = 0;
    std::string modifiedTime;
};

/**
 * @brief Asset Browser Panel implementation
 */
class AssetBrowserPanel : public GUIPanelBase {
public:
    AssetBrowserPanel();
    ~AssetBrowserPanel() override = default;

    /**
     * @brief Navigate to directory
     */
    void navigateTo(const std::string& path);

    /**
     * @brief Navigate up one level
     */
    void navigateUp();

    /**
     * @brief Navigate back in history
     */
    void navigateBack();

    /**
     * @brief Navigate forward in history
     */
    void navigateForward();

    /**
     * @brief Refresh current directory
     */
    void refresh();

    /**
     * @brief Get current directory path
     */
    [[nodiscard]] const std::string& getCurrentPath() const { return m_currentPath; }

    /**
     * @brief Set view mode (grid/list)
     */
    void setGridView(bool gridView) { m_isGridView = gridView; }

    /**
     * @brief Check if using grid view
     */
    [[nodiscard]] bool isGridView() const { return m_isGridView; }

    /**
     * @brief Set thumbnail size
     */
    void setThumbnailSize(f32 size) { m_thumbnailSize = size; }

    /**
     * @brief Set search filter
     */
    void setFilter(const std::string& filter) { m_filter = filter; }

    [[nodiscard]] std::vector<MenuItem> getMenuItems() const override;
    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;
    [[nodiscard]] std::vector<MenuItem> getContextMenuItems() const override;

protected:
    void onInitialize() override;
    void onRender() override;
    void renderToolbar() override;

private:
    void renderBreadcrumb();
    void renderDirectoryTree();
    void renderAssetGrid();
    void renderAssetList();
    void renderAssetEntry(const AssetEntry& entry);
    void renderPreviewPanel();

    void loadDirectory(const std::string& path);
    AssetType getAssetType(const std::string& extension) const;
    void handleAssetDoubleClick(const AssetEntry& entry);
    void handleAssetDragDrop(const AssetEntry& entry);

    std::string m_currentPath;
    std::string m_rootPath;
    std::string m_filter;
    std::vector<AssetEntry> m_entries;
    std::vector<std::string> m_history;
    size_t m_historyIndex = 0;

    bool m_isGridView = true;
    f32 m_thumbnailSize = 80.0f;
    bool m_showPreview = true;

    // Selection
    std::string m_selectedAsset;
    std::vector<std::string> m_selectedAssets;

    // Search
    char m_searchBuffer[256] = {0};
};

} // namespace NovelMind::editor
