/**
 * @file asset_browser_panel.cpp
 * @brief Asset Browser Panel implementation
 */

#include "NovelMind/editor/asset_browser_panel.hpp"
#include "NovelMind/editor/imgui_integration.hpp"
#include <algorithm>
#include <cstring>

namespace NovelMind::editor {

AssetBrowserPanel::AssetBrowserPanel()
    : GUIPanelBase("Asset Browser")
{
}

void AssetBrowserPanel::navigateTo(const std::string& path)
{
    // Add to history
    if (m_historyIndex < m_history.size())
    {
        m_history.resize(m_historyIndex + 1);
    }
    m_history.push_back(path);
    m_historyIndex = m_history.size() - 1;

    m_currentPath = path;
    loadDirectory(path);
}

void AssetBrowserPanel::navigateUp()
{
    std::filesystem::path current(m_currentPath);
    if (current.has_parent_path() && current.parent_path() != current)
    {
        navigateTo(current.parent_path().string());
    }
}

void AssetBrowserPanel::navigateBack()
{
    if (m_historyIndex > 0)
    {
        --m_historyIndex;
        m_currentPath = m_history[m_historyIndex];
        loadDirectory(m_currentPath);
    }
}

void AssetBrowserPanel::navigateForward()
{
    if (m_historyIndex + 1 < m_history.size())
    {
        ++m_historyIndex;
        m_currentPath = m_history[m_historyIndex];
        loadDirectory(m_currentPath);
    }
}

void AssetBrowserPanel::refresh()
{
    loadDirectory(m_currentPath);
}

std::vector<MenuItem> AssetBrowserPanel::getMenuItems() const
{
    std::vector<MenuItem> items;

    MenuItem viewMenu;
    viewMenu.label = "View";
    viewMenu.subItems = {
        {"Grid View", "", [this]() { const_cast<AssetBrowserPanel*>(this)->setGridView(true); },
         []() { return true; }, [this]() { return m_isGridView; }},
        {"List View", "", [this]() { const_cast<AssetBrowserPanel*>(this)->setGridView(false); },
         []() { return true; }, [this]() { return !m_isGridView; }},
        MenuItem::separator(),
        {"Show Preview", "", [this]() { const_cast<AssetBrowserPanel*>(this)->m_showPreview = !m_showPreview; },
         []() { return true; }, [this]() { return m_showPreview; }},
    };
    items.push_back(viewMenu);

    return items;
}

std::vector<ToolbarItem> AssetBrowserPanel::getToolbarItems() const
{
    std::vector<ToolbarItem> items;

    items.push_back({"<", "Back", [this]() { const_cast<AssetBrowserPanel*>(this)->navigateBack(); },
                     [this]() { return m_historyIndex > 0; }});
    items.push_back({">", "Forward", [this]() { const_cast<AssetBrowserPanel*>(this)->navigateForward(); },
                     [this]() { return m_historyIndex + 1 < m_history.size(); }});
    items.push_back({"^", "Up", [this]() { const_cast<AssetBrowserPanel*>(this)->navigateUp(); }});
    items.push_back({"R", "Refresh", [this]() { const_cast<AssetBrowserPanel*>(this)->refresh(); }});

    items.push_back(ToolbarItem::separator());

    items.push_back({"Grid", "Grid View", [this]() { const_cast<AssetBrowserPanel*>(this)->setGridView(true); },
                     []() { return true; }, [this]() { return m_isGridView; }});
    items.push_back({"List", "List View", [this]() { const_cast<AssetBrowserPanel*>(this)->setGridView(false); },
                     []() { return true; }, [this]() { return !m_isGridView; }});

    return items;
}

std::vector<MenuItem> AssetBrowserPanel::getContextMenuItems() const
{
    std::vector<MenuItem> items;

    items.push_back({"Open", "", []() { /* Open asset */ }});
    items.push_back({"Open in External Editor", "", []() { /* External open */ }});
    items.push_back(MenuItem::separator());
    items.push_back({"Copy", "Ctrl+C", []() { /* Copy */ }});
    items.push_back({"Paste", "Ctrl+V", []() { /* Paste */ }});
    items.push_back({"Duplicate", "Ctrl+D", []() { /* Duplicate */ }});
    items.push_back({"Rename", "F2", []() { /* Rename */ }});
    items.push_back({"Delete", "Delete", []() { /* Delete */ }});
    items.push_back(MenuItem::separator());
    items.push_back({"Show in Explorer", "", []() { /* Show in explorer */ }});

    return items;
}

void AssetBrowserPanel::onInitialize()
{
    m_rootPath = ".";  // Would be project assets folder
    m_currentPath = m_rootPath;
    m_history.push_back(m_currentPath);
    loadDirectory(m_currentPath);
}

void AssetBrowserPanel::onRender()
{
    // Toolbar
    renderToolbar();

    // Breadcrumb navigation
    renderBreadcrumb();

    // Split: directory tree on left, content on right
    // ImGui::BeginChild("DirectoryTree", ImVec2(200, 0), true);
    renderDirectoryTree();
    // ImGui::EndChild();

    // ImGui::SameLine();

    // ImGui::BeginChild("AssetContent", ImVec2(0, 0), true);
    if (m_isGridView)
    {
        renderAssetGrid();
    }
    else
    {
        renderAssetList();
    }
    // ImGui::EndChild();

    // Preview panel (optional)
    if (m_showPreview && !m_selectedAsset.empty())
    {
        // ImGui::SameLine();
        renderPreviewPanel();
    }
}

void AssetBrowserPanel::renderToolbar()
{
    widgets::BeginToolbar("AssetBrowserToolbar");

    renderToolbarItems(getToolbarItems());

    // Search input
    // ImGui::SameLine(ImGui::GetContentRegionAvail().x - 200);
    // ImGui::SetNextItemWidth(180);
    widgets::SearchInput("##Search", m_searchBuffer, sizeof(m_searchBuffer), "Search assets...");

    widgets::EndToolbar();
}

void AssetBrowserPanel::renderBreadcrumb()
{
    std::filesystem::path path(m_currentPath);
    std::vector<std::string> parts;

    for (const auto& part : path)
    {
        parts.push_back(part.string());
    }

    for (size_t i = 0; i < parts.size(); ++i)
    {
        if (i > 0)
        {
            // ImGui::SameLine();
            // ImGui::Text(">");
            // ImGui::SameLine();
        }

        // if (ImGui::Button(parts[i].c_str()))
        // {
        //     // Navigate to this path
        //     std::filesystem::path targetPath;
        //     for (size_t j = 0; j <= i; ++j)
        //     {
        //         targetPath /= parts[j];
        //     }
        //     navigateTo(targetPath.string());
        // }
    }

    // ImGui::Separator();
}

void AssetBrowserPanel::renderDirectoryTree()
{
    // Recursive tree of directories
    // Would use ImGui::TreeNode for each directory

    // Placeholder: show current directory contents
    for (const auto& entry : m_entries)
    {
        if (entry.isDirectory)
        {
            bool isExpanded = false;
            if (widgets::TreeNode(entry.name.c_str(), false, false))
            {
                isExpanded = true;
                // Would recursively render subdirectories
            }

            if (isExpanded)
            {
                // ImGui::TreePop();
            }
        }
    }
}

void AssetBrowserPanel::renderAssetGrid()
{
    // Calculate number of columns based on thumbnail size
    f32 padding = 10.0f;
    f32 cellSize = m_thumbnailSize + padding;
    i32 columns = std::max(1, static_cast<i32>(m_contentWidth / cellSize));

    // ImGui::Columns(columns, nullptr, false);

    std::string filterLower = m_filter;
    std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);

    for (const auto& entry : m_entries)
    {
        // Apply filter
        if (!filterLower.empty())
        {
            std::string nameLower = entry.name;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(filterLower) == std::string::npos)
            {
                continue;
            }
        }

        renderAssetEntry(entry);
        // ImGui::NextColumn();
    }

    // ImGui::Columns(1);
    (void)columns;
    (void)cellSize;
}

void AssetBrowserPanel::renderAssetList()
{
    // Render as list with columns: Name, Type, Size, Modified
    // ImGui::Columns(4, "AssetListColumns", true);
    // ImGui::SetColumnWidth(0, 200);
    // ImGui::SetColumnWidth(1, 80);
    // ImGui::SetColumnWidth(2, 80);

    // ImGui::Text("Name"); ImGui::NextColumn();
    // ImGui::Text("Type"); ImGui::NextColumn();
    // ImGui::Text("Size"); ImGui::NextColumn();
    // ImGui::Text("Modified"); ImGui::NextColumn();
    // ImGui::Separator();

    for (const auto& entry : m_entries)
    {
        bool isSelected = (m_selectedAsset == entry.path);

        // if (ImGui::Selectable(entry.name.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
        // {
        //     m_selectedAsset = entry.path;
        // }
        // if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        // {
        //     handleAssetDoubleClick(entry);
        // }

        // ImGui::NextColumn();
        // ImGui::Text("%s", entry.extension.c_str()); ImGui::NextColumn();
        // ImGui::Text("%llu KB", entry.size / 1024); ImGui::NextColumn();
        // ImGui::Text("%s", entry.modifiedTime.c_str()); ImGui::NextColumn();

        (void)isSelected;
    }

    // ImGui::Columns(1);
}

void AssetBrowserPanel::renderAssetEntry(const AssetEntry& entry)
{
    bool isSelected = (m_selectedAsset == entry.path);

    // ImGui::BeginGroup();

    // Thumbnail area
    // ImVec2 thumbnailSize(m_thumbnailSize, m_thumbnailSize);
    // ImGui::PushStyleColor(ImGuiCol_Button, isSelected ?
    //     ImVec4(0.3f, 0.5f, 0.8f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    // Icon based on type
    const char* icon = entry.isDirectory ? "[D]" : "[F]";
    switch (entry.type)
    {
        case AssetType::Image: icon = "[IMG]"; break;
        case AssetType::Audio: icon = "[SND]"; break;
        case AssetType::Script: icon = "[SCR]"; break;
        case AssetType::Scene: icon = "[SCN]"; break;
        default: break;
    }

    // if (ImGui::Button(icon, thumbnailSize))
    // {
    //     m_selectedAsset = entry.path;
    // }

    // ImGui::PopStyleColor();

    // Handle double-click
    // if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
    // {
    //     handleAssetDoubleClick(entry);
    // }

    // Handle drag-drop source
    // if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    // {
    //     ImGui::SetDragDropPayload("ASSET_PATH", entry.path.c_str(), entry.path.size() + 1);
    //     ImGui::Text("%s", entry.name.c_str());
    //     ImGui::EndDragDropSource();
    // }

    // Name label (truncated)
    // ImGui::TextWrapped("%s", entry.name.c_str());

    // ImGui::EndGroup();

    (void)icon;
    (void)isSelected;
}

void AssetBrowserPanel::renderPreviewPanel()
{
    // ImGui::BeginChild("PreviewPanel", ImVec2(200, 0), true);
    // ImGui::Text("Preview");
    // ImGui::Separator();

    if (!m_selectedAsset.empty())
    {
        // Find selected entry
        auto it = std::find_if(m_entries.begin(), m_entries.end(),
            [this](const AssetEntry& e) { return e.path == m_selectedAsset; });

        if (it != m_entries.end())
        {
            // ImGui::Text("Name: %s", it->name.c_str());
            // ImGui::Text("Type: %s", it->extension.c_str());
            // ImGui::Text("Size: %llu bytes", it->size);
            // ImGui::Text("Modified: %s", it->modifiedTime.c_str());

            // Type-specific preview
            switch (it->type)
            {
                case AssetType::Image:
                    // Would render image preview
                    break;
                case AssetType::Audio:
                    // Would show audio waveform/controls
                    break;
                default:
                    break;
            }
        }
    }

    // ImGui::EndChild();
}

void AssetBrowserPanel::loadDirectory(const std::string& path)
{
    m_entries.clear();

    try
    {
        for (const auto& dirEntry : std::filesystem::directory_iterator(path))
        {
            AssetEntry entry;
            entry.name = dirEntry.path().filename().string();
            entry.path = dirEntry.path().string();
            entry.isDirectory = dirEntry.is_directory();

            if (!entry.isDirectory)
            {
                entry.extension = dirEntry.path().extension().string();
                entry.type = getAssetType(entry.extension);
                entry.size = std::filesystem::file_size(dirEntry);
            }

            // Skip hidden files
            if (!entry.name.empty() && entry.name[0] != '.')
            {
                m_entries.push_back(entry);
            }
        }

        // Sort: directories first, then by name
        std::sort(m_entries.begin(), m_entries.end(),
            [](const AssetEntry& a, const AssetEntry& b) {
                if (a.isDirectory != b.isDirectory)
                {
                    return a.isDirectory > b.isDirectory;
                }
                return a.name < b.name;
            });
    }
    catch (const std::exception& /*e*/)
    {
        // Handle filesystem errors
    }
}

AssetType AssetBrowserPanel::getAssetType(const std::string& extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga")
    {
        return AssetType::Image;
    }
    if (ext == ".wav" || ext == ".mp3" || ext == ".ogg" || ext == ".flac")
    {
        return AssetType::Audio;
    }
    if (ext == ".nms" || ext == ".txt")
    {
        return AssetType::Script;
    }
    if (ext == ".nmscene")
    {
        return AssetType::Scene;
    }
    if (ext == ".ttf" || ext == ".otf")
    {
        return AssetType::Font;
    }
    if (ext == ".mp4" || ext == ".webm" || ext == ".avi")
    {
        return AssetType::Video;
    }
    if (ext == ".json" || ext == ".xml" || ext == ".yaml")
    {
        return AssetType::Data;
    }

    return AssetType::Unknown;
}

void AssetBrowserPanel::handleAssetDoubleClick(const AssetEntry& entry)
{
    if (entry.isDirectory)
    {
        navigateTo(entry.path);
    }
    else
    {
        // Open asset based on type
        AssetEvent event(EditorEventType::AssetModified);
        event.assetPath = entry.path;
        event.assetType = entry.extension;
        publishEvent(event);
    }
}

void AssetBrowserPanel::handleAssetDragDrop(const AssetEntry& /*entry*/)
{
    // Set up drag-drop payload
}

} // namespace NovelMind::editor
