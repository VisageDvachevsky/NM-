/**
 * @file voice_manager_panel.hpp
 * @brief Voice Manager GUI Panel for NovelMind Editor v0.2.0
 *
 * Provides a complete voice management interface:
 * - Dialogue line table with voice status
 * - Status indicators: Bound, Missing, AutoMapped, Error
 * - Voice preview playback
 * - Manual voice file binding
 * - Automatic voice mapping
 * - Navigate to source node in StoryGraph
 * - Voice coverage reporting
 */

#pragma once

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/editor/voice_manager.hpp"
#include "NovelMind/renderer/color.hpp"
#include <string>
#include <vector>
#include <functional>

namespace NovelMind::editor {

/**
 * @brief Table column configuration for voice manager
 */
struct VoiceTableColumn {
    std::string header;
    f32 width;
    bool sortable = true;
    bool visible = true;

    enum class Type {
        LineId,
        Character,
        DialogueText,
        VoiceFile,
        Status,
        Duration,
        Scene,
        Actions
    };
    Type type;
};

/**
 * @brief Sort options for the voice table
 */
struct VoiceSortState {
    VoiceTableColumn::Type column = VoiceTableColumn::Type::LineId;
    bool ascending = true;
};

/**
 * @brief Voice Manager Panel - GUI for voice-over management
 *
 * This panel provides a comprehensive interface for managing voice-over:
 *
 * Features:
 * - Full table view of all dialogue lines
 * - Status column showing: Bound (green), Unbound (gray), Missing (red),
 *   AutoMapped (blue), Pending (yellow)
 * - Quick preview playback with play/stop buttons
 * - Drag-drop voice file binding
 * - Auto-map functionality with pattern configuration
 * - Filter by character, scene, or status
 * - Search in dialogue text
 * - Navigate to source node in StoryGraph
 * - Export/import voice assignment tables
 * - Coverage statistics display
 */
class VoiceManagerPanel : public GUIPanelBase {
public:
    VoiceManagerPanel();
    ~VoiceManagerPanel() override = default;

    /**
     * @brief Set the voice manager to use
     */
    void setVoiceManager(VoiceManager* manager);

    /**
     * @brief Get the voice manager
     */
    [[nodiscard]] VoiceManager* getVoiceManager() const { return m_voiceManager; }

    /**
     * @brief Refresh the panel data
     */
    void refresh();

    // =========================================================================
    // Filtering
    // =========================================================================

    /**
     * @brief Set character filter
     */
    void setCharacterFilter(const std::string& character);

    /**
     * @brief Set scene filter
     */
    void setSceneFilter(const std::string& scene);

    /**
     * @brief Set status filter
     */
    void setStatusFilter(VoiceBindingStatus status);

    /**
     * @brief Show all statuses
     */
    void clearStatusFilter();

    /**
     * @brief Set search text
     */
    void setSearchText(const std::string& text);

    /**
     * @brief Clear all filters
     */
    void clearAllFilters();

    // =========================================================================
    // Selection
    // =========================================================================

    /**
     * @brief Select a line by ID
     */
    void selectLine(const std::string& lineId);

    /**
     * @brief Get selected line IDs
     */
    [[nodiscard]] const std::vector<std::string>& getSelectedLines() const { return m_selectedLines; }

    /**
     * @brief Clear selection
     */
    void clearSelection();

    // =========================================================================
    // Actions
    // =========================================================================

    /**
     * @brief Preview selected line's voice
     */
    void previewSelectedVoice();

    /**
     * @brief Stop voice preview
     */
    void stopPreview();

    /**
     * @brief Bind voice file to selected line
     */
    void bindVoiceFile(const std::string& voicePath);

    /**
     * @brief Unbind voice from selected lines
     */
    void unbindSelectedVoices();

    /**
     * @brief Run auto-mapping
     */
    void runAutoMapping();

    /**
     * @brief Navigate to source node of selected line
     */
    void navigateToSource();

    /**
     * @brief Export voice table
     */
    void exportVoiceTable(const std::string& path, VoiceTableFormat format);

    /**
     * @brief Import voice table
     */
    void importVoiceTable(const std::string& path, VoiceTableFormat format);

    // =========================================================================
    // Callbacks
    // =========================================================================

    /**
     * @brief Set callback for navigating to a node
     */
    void setOnNavigateToNode(std::function<void(const std::string& nodeId)> callback);

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
    void renderTable();
    void renderTableHeader();
    void renderTableRow(const DialogueLine* line, size_t rowIndex);
    void renderStatusBadge(VoiceBindingStatus status);
    void renderCoverageStats();
    void renderAutoMapDialog();
    void renderExportImportDialog();
    void renderPatternConfigDialog();

    // Table helpers
    void sortTable();
    void applyFilters();
    std::vector<const DialogueLine*> getFilteredLines() const;

    // Input handling
    void handleRowClick(const std::string& lineId, bool ctrl, bool shift);
    void handleRowDoubleClick(const std::string& lineId);
    void handleDragDrop(const std::string& lineId);
    void handleContextMenu(const std::string& lineId);

    // Status helpers
    const char* getStatusText(VoiceBindingStatus status) const;
    renderer::Color getStatusColor(VoiceBindingStatus status) const;

    // Voice manager (not owned)
    VoiceManager* m_voiceManager = nullptr;

    // Filtered and sorted lines (cached)
    std::vector<const DialogueLine*> m_filteredLines;
    bool m_filterDirty = true;

    // Filter state
    VoiceLineFilter m_filter;
    char m_searchBuffer[256] = {0};

    // Sort state
    VoiceSortState m_sortState;

    // Selection
    std::vector<std::string> m_selectedLines;
    std::string m_lastSelectedLine;

    // Table columns
    std::vector<VoiceTableColumn> m_columns;

    // UI state
    bool m_showAutoMapDialog = false;
    bool m_showExportImportDialog = false;
    bool m_showPatternConfig = false;
    bool m_isExporting = true;  // true = export, false = import
    char m_exportPathBuffer[512] = {0};
    i32 m_exportFormatIndex = 0;

    // Preview state
    std::string m_previewingLineId;
    bool m_isPreviewPlaying = false;

    // Scroll state
    f32 m_scrollY = 0.0f;
    size_t m_visibleRowStart = 0;
    size_t m_visibleRowCount = 0;

    // Callbacks
    std::function<void(const std::string&)> m_onNavigateToNode;

    // Character and scene lists for dropdowns
    std::vector<std::string> m_characterList;
    std::vector<std::string> m_sceneList;
    i32 m_selectedCharacterIndex = 0;
    i32 m_selectedSceneIndex = 0;
    i32 m_selectedStatusIndex = 0;
};

} // namespace NovelMind::editor
