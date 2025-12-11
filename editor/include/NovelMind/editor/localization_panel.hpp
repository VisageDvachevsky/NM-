/**
 * @file localization_panel.hpp
 * @brief Localization Manager GUI Panel for NovelMind Editor v0.2.0
 *
 * Provides a complete localization management interface:
 * - String table view with all localization keys
 * - Multi-language editing
 * - Missing string detection and highlighting
 * - Filter by status (complete, missing, outdated)
 * - Search in keys and values
 * - Import/export (JSON, CSV, PO, XLIFF)
 * - Locale configuration
 */

#pragma once

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/localization/localization_manager.hpp"
#include <string>
#include <vector>
#include <functional>

namespace NovelMind::editor {

/**
 * @brief Localization string status
 */
enum class LocalizationStatus : u8 {
    Complete,    // String exists in all locales
    Missing,     // String missing in one or more locales
    Outdated,    // Source string changed, translations may be stale
    New          // Newly added string
};

/**
 * @brief Entry in the localization table
 */
struct LocalizationEntry {
    std::string key;
    std::string context;
    std::string sourceFile;
    u32 lineNumber = 0;
    LocalizationStatus status = LocalizationStatus::Complete;
    std::unordered_map<std::string, std::string> translations;  // locale -> value
    std::unordered_map<std::string, bool> isModified;           // locale -> modified flag
};

/**
 * @brief Filter options for localization panel
 */
struct LocalizationFilter {
    std::string searchText;
    LocalizationStatus statusFilter = LocalizationStatus::Complete;
    bool showAllStatuses = true;
    std::string localeFilter;  // Empty = show all
    bool showOnlyModified = false;
};

/**
 * @brief Localization Manager Panel
 *
 * This panel provides a complete interface for managing translations:
 *
 * Features:
 * - Table view of all localization keys
 * - Side-by-side editing of multiple languages
 * - Status indicators: Complete (green), Missing (red), Outdated (yellow)
 * - Search in keys and translation values
 * - Filter by locale and status
 * - Add/remove localization keys
 * - Import/export in multiple formats
 * - Locale management (add/remove languages)
 * - Coverage statistics per locale
 */
class LocalizationPanel : public GUIPanelBase {
public:
    LocalizationPanel();
    ~LocalizationPanel() override = default;

    /**
     * @brief Set the localization manager to use
     */
    void setLocalizationManager(localization::LocalizationManager* manager);

    /**
     * @brief Get the localization manager
     */
    [[nodiscard]] localization::LocalizationManager* getLocalizationManager() const { return m_locManager; }

    /**
     * @brief Refresh panel data from manager
     */
    void refresh();

    // =========================================================================
    // Locale Management
    // =========================================================================

    /**
     * @brief Get list of available locales
     */
    [[nodiscard]] std::vector<localization::LocaleId> getLocales() const;

    /**
     * @brief Set the primary editing locale (left column)
     */
    void setPrimaryLocale(const localization::LocaleId& locale);

    /**
     * @brief Set the secondary editing locale (right column)
     */
    void setSecondaryLocale(const localization::LocaleId& locale);

    /**
     * @brief Add a new locale
     */
    void addLocale(const localization::LocaleId& locale);

    /**
     * @brief Remove a locale
     */
    void removeLocale(const localization::LocaleId& locale);

    // =========================================================================
    // String Management
    // =========================================================================

    /**
     * @brief Add a new localization key
     */
    void addKey(const std::string& key, const std::string& defaultValue = "");

    /**
     * @brief Remove a localization key
     */
    void removeKey(const std::string& key);

    /**
     * @brief Rename a localization key
     */
    void renameKey(const std::string& oldKey, const std::string& newKey);

    /**
     * @brief Set translation value
     */
    void setTranslation(const std::string& key, const localization::LocaleId& locale,
                        const std::string& value);

    // =========================================================================
    // Filtering
    // =========================================================================

    /**
     * @brief Set search text
     */
    void setSearchText(const std::string& text);

    /**
     * @brief Set status filter
     */
    void setStatusFilter(LocalizationStatus status);

    /**
     * @brief Show all statuses
     */
    void clearStatusFilter();

    /**
     * @brief Show only modified strings
     */
    void setShowOnlyModified(bool show);

    /**
     * @brief Clear all filters
     */
    void clearAllFilters();

    // =========================================================================
    // Selection
    // =========================================================================

    /**
     * @brief Select a key
     */
    void selectKey(const std::string& key);

    /**
     * @brief Get selected keys
     */
    [[nodiscard]] const std::vector<std::string>& getSelectedKeys() const { return m_selectedKeys; }

    /**
     * @brief Clear selection
     */
    void clearSelection();

    // =========================================================================
    // Import/Export
    // =========================================================================

    /**
     * @brief Export strings for a locale
     */
    void exportStrings(const localization::LocaleId& locale,
                       const std::string& path,
                       localization::LocalizationFormat format);

    /**
     * @brief Export missing strings for translation
     */
    void exportMissingStrings(const localization::LocaleId& locale,
                              const std::string& path,
                              localization::LocalizationFormat format);

    /**
     * @brief Import strings for a locale
     */
    void importStrings(const localization::LocaleId& locale,
                       const std::string& path,
                       localization::LocalizationFormat format);

    // =========================================================================
    // Statistics
    // =========================================================================

    /**
     * @brief Get total string count
     */
    [[nodiscard]] size_t getTotalStringCount() const;

    /**
     * @brief Get translation coverage for a locale
     */
    [[nodiscard]] f32 getLocaleCoverage(const localization::LocaleId& locale) const;

    /**
     * @brief Get missing string count for a locale
     */
    [[nodiscard]] size_t getMissingCount(const localization::LocaleId& locale) const;

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

private:
    // Rendering
    void renderFilterBar();
    void renderLocaleSelector();
    void renderTable();
    void renderTableHeader();
    void renderTableRow(const LocalizationEntry& entry, size_t rowIndex);
    void renderStatusBadge(LocalizationStatus status);
    void renderCoverageStats();
    void renderAddKeyDialog();
    void renderImportExportDialog();
    void renderLocaleConfigDialog();

    // Table helpers
    void rebuildEntryList();
    void sortEntries();
    void applyFilters();
    LocalizationStatus computeStatus(const std::string& key) const;

    // Input handling
    void handleRowClick(const std::string& key, bool ctrl, bool shift);
    void handleRowDoubleClick(const std::string& key);
    void handleCellEdit(const std::string& key, const localization::LocaleId& locale,
                        const std::string& newValue);
    void handleContextMenu(const std::string& key);
    void handleKeyNavigation(i32 direction);

    // Localization manager (not owned)
    localization::LocalizationManager* m_locManager = nullptr;

    // Entry list
    std::vector<LocalizationEntry> m_entries;
    std::vector<size_t> m_filteredIndices;
    bool m_entriesDirty = true;
    bool m_filterDirty = true;

    // Filter state
    LocalizationFilter m_filter;
    char m_searchBuffer[256] = {0};

    // Sort state
    enum class SortColumn { Key, Status, PrimaryValue, SecondaryValue };
    SortColumn m_sortColumn = SortColumn::Key;
    bool m_sortAscending = true;

    // Locale selection
    localization::LocaleId m_primaryLocale{"en", ""};
    localization::LocaleId m_secondaryLocale{"ja", ""};
    std::vector<localization::LocaleId> m_availableLocales;
    i32 m_primaryLocaleIndex = 0;
    i32 m_secondaryLocaleIndex = 1;

    // Selection
    std::vector<std::string> m_selectedKeys;
    std::string m_lastSelectedKey;

    // Editing state
    std::string m_editingKey;
    localization::LocaleId m_editingLocale{"", ""};
    char m_editBuffer[4096] = {0};
    bool m_isEditing = false;

    // Dialog state
    bool m_showAddKeyDialog = false;
    bool m_showImportExportDialog = false;
    bool m_showLocaleConfigDialog = false;
    bool m_isExporting = true;
    char m_newKeyBuffer[256] = {0};
    char m_newValueBuffer[1024] = {0};
    char m_filePathBuffer[512] = {0};
    i32 m_formatIndex = 0;
    i32 m_targetLocaleIndex = 0;

    // Scroll state
    f32 m_scrollY = 0.0f;
    size_t m_visibleRowStart = 0;
    size_t m_visibleRowCount = 0;

    // Column widths
    f32 m_keyColumnWidth = 200.0f;
    f32 m_statusColumnWidth = 80.0f;
    bool m_isDraggingColumn = false;
    i32 m_draggingColumnIndex = -1;
};

} // namespace NovelMind::editor
