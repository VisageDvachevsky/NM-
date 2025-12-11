/**
 * @file voice_manager_panel.cpp
 * @brief Voice Manager Panel implementation
 */

#include "NovelMind/editor/voice_manager_panel.hpp"
#include "NovelMind/editor/imgui_integration.hpp"
#include "NovelMind/editor/style_guide.hpp"
#include <algorithm>
#include <cstring>

namespace NovelMind::editor {

VoiceManagerPanel::VoiceManagerPanel()
    : GUIPanelBase("Voice Manager")
{
    setFlags(PanelFlags::MenuBar);

    // Initialize default columns
    m_columns = {
        {"ID", 100.0f, true, true, VoiceTableColumn::Type::LineId},
        {"Character", 100.0f, true, true, VoiceTableColumn::Type::Character},
        {"Dialogue", 300.0f, true, true, VoiceTableColumn::Type::DialogueText},
        {"Voice File", 200.0f, true, true, VoiceTableColumn::Type::VoiceFile},
        {"Status", 80.0f, true, true, VoiceTableColumn::Type::Status},
        {"Duration", 70.0f, true, true, VoiceTableColumn::Type::Duration},
        {"", 60.0f, false, true, VoiceTableColumn::Type::Actions}
    };
}

void VoiceManagerPanel::onInitialize()
{
    // Register for play mode events
    if (m_voiceManager) {
        m_voiceManager->setOnVoicePreviewStart([this](const std::string& lineId) {
            m_previewingLineId = lineId;
            m_isPreviewPlaying = true;
        });

        m_voiceManager->setOnVoicePreviewStop([this]() {
            m_previewingLineId.clear();
            m_isPreviewPlaying = false;
        });
    }
}

void VoiceManagerPanel::onShutdown()
{
    stopPreview();
}

void VoiceManagerPanel::onUpdate(f64 /*deltaTime*/)
{
    // Rebuild filtered list if needed
    if (m_filterDirty && m_voiceManager) {
        applyFilters();
        m_filterDirty = false;
    }
}

void VoiceManagerPanel::onRender()
{
    // Filter bar at top
    renderFilterBar();

    // Main table
    renderTable();

    // Coverage stats at bottom
    renderCoverageStats();

    // Dialogs
    if (m_showAutoMapDialog) {
        renderAutoMapDialog();
    }
    if (m_showExportImportDialog) {
        renderExportImportDialog();
    }
    if (m_showPatternConfig) {
        renderPatternConfigDialog();
    }
}

void VoiceManagerPanel::renderToolbar()
{
    widgets::BeginToolbar("VoiceManagerToolbar", Heights::Toolbar);

    // Refresh
    if (widgets::ToolbarButton(Icons::Refresh, "Refresh")) {
        refresh();
    }

    widgets::ToolbarSeparator();

    // Auto-map
    if (widgets::ToolbarButton(Icons::Transform, "Auto-Map Voice Files")) {
        m_showAutoMapDialog = true;
    }

    widgets::ToolbarSeparator();

    // Export/Import
    if (widgets::ToolbarButton(Icons::Export, "Export")) {
        m_isExporting = true;
        m_showExportImportDialog = true;
    }
    if (widgets::ToolbarButton(Icons::Import, "Import")) {
        m_isExporting = false;
        m_showExportImportDialog = true;
    }

    widgets::ToolbarSeparator();

    // Pattern config
    if (widgets::ToolbarButton(Icons::Settings, "Configure Patterns")) {
        m_showPatternConfig = true;
    }

    widgets::EndToolbar();
}

void VoiceManagerPanel::renderMenuBar()
{
    // Menu bar items handled by menu system
}

void VoiceManagerPanel::renderFilterBar()
{
    // Search input
    if (widgets::SearchInput("##VoiceSearch", m_searchBuffer, sizeof(m_searchBuffer), "Search dialogue...")) {
        m_filter.searchText = m_searchBuffer;
        m_filterDirty = true;
    }

    // Character dropdown
    std::vector<std::string> characterOptions = {"All Characters"};
    if (m_voiceManager) {
        auto chars = m_voiceManager->getCharacters();
        characterOptions.insert(characterOptions.end(), chars.begin(), chars.end());
    }
    if (widgets::Dropdown("Character", &m_selectedCharacterIndex, characterOptions)) {
        m_filter.characterFilter = m_selectedCharacterIndex > 0 ?
            characterOptions[m_selectedCharacterIndex] : "";
        m_filterDirty = true;
    }

    // Scene dropdown
    std::vector<std::string> sceneOptions = {"All Scenes"};
    if (m_voiceManager) {
        auto scenes = m_voiceManager->getScenes();
        sceneOptions.insert(sceneOptions.end(), scenes.begin(), scenes.end());
    }
    if (widgets::Dropdown("Scene", &m_selectedSceneIndex, sceneOptions)) {
        m_filter.sceneFilter = m_selectedSceneIndex > 0 ?
            sceneOptions[m_selectedSceneIndex] : "";
        m_filterDirty = true;
    }

    // Status dropdown
    std::vector<std::string> statusOptions = {
        "All Statuses", "Unbound", "Bound", "Missing", "Auto-Mapped", "Pending"
    };
    if (widgets::Dropdown("Status", &m_selectedStatusIndex, statusOptions)) {
        if (m_selectedStatusIndex == 0) {
            m_filter.showAllStatuses = true;
        } else {
            m_filter.showAllStatuses = false;
            m_filter.statusFilter = static_cast<VoiceBindingStatus>(m_selectedStatusIndex - 1);
        }
        m_filterDirty = true;
    }

    // Clear filters button
    if (widgets::ToolbarButton(Icons::Close, "Clear Filters")) {
        clearAllFilters();
    }
}

void VoiceManagerPanel::renderTable()
{
    // Table header
    renderTableHeader();

    // Table body - use virtual scrolling for large lists
    auto filteredLines = getFilteredLines();

    // Calculate visible range
    f32 rowHeight = Heights::TableRow;
    f32 tableHeight = getPanelHeight() - Heights::Toolbar - 100.0f;  // Minus filter bar and stats
    m_visibleRowCount = static_cast<size_t>(tableHeight / rowHeight) + 2;
    m_visibleRowStart = static_cast<size_t>(m_scrollY / rowHeight);

    if (m_visibleRowStart >= filteredLines.size()) {
        m_visibleRowStart = filteredLines.size() > 0 ? filteredLines.size() - 1 : 0;
    }

    size_t visibleEnd = std::min(m_visibleRowStart + m_visibleRowCount, filteredLines.size());

    // Render visible rows
    for (size_t i = m_visibleRowStart; i < visibleEnd; ++i) {
        renderTableRow(filteredLines[i], i);
    }
}

void VoiceManagerPanel::renderTableHeader()
{
    // Render column headers with sort indicators
    for (const auto& col : m_columns) {
        if (!col.visible) continue;

        // Header button/text
        bool isCurrentSort = (m_sortState.column == col.type);

        // Would render sortable column header
        // On click, toggle sort
    }
}

void VoiceManagerPanel::renderTableRow(const DialogueLine* line, size_t rowIndex)
{
    if (!line) return;

    bool isSelected = std::find(m_selectedLines.begin(), m_selectedLines.end(), line->id)
                      != m_selectedLines.end();

    bool isPreviewingThis = (m_previewingLineId == line->id);

    // Row background
    // Alternate colors, highlight if selected

    // Columns
    for (const auto& col : m_columns) {
        if (!col.visible) continue;

        switch (col.type) {
            case VoiceTableColumn::Type::LineId:
                // Render line->id
                break;

            case VoiceTableColumn::Type::Character:
                // Render line->characterId
                break;

            case VoiceTableColumn::Type::DialogueText:
                // Render truncated line->text
                break;

            case VoiceTableColumn::Type::VoiceFile:
                // Render line->voiceFile or "(none)"
                break;

            case VoiceTableColumn::Type::Status:
                renderStatusBadge(line->status);
                break;

            case VoiceTableColumn::Type::Duration:
                // Render formatted duration
                break;

            case VoiceTableColumn::Type::Scene:
                // Render line->sceneId
                break;

            case VoiceTableColumn::Type::Actions:
                // Play/stop button
                if (isPreviewingThis) {
                    if (widgets::ToolbarButton(Icons::Stop, "Stop")) {
                        stopPreview();
                    }
                } else if (!line->voiceFile.empty() && line->status != VoiceBindingStatus::MissingFile) {
                    if (widgets::ToolbarButton(Icons::Play, "Preview")) {
                        selectLine(line->id);
                        previewSelectedVoice();
                    }
                }

                // Navigate to source button
                if (widgets::ToolbarButton(Icons::ArrowRight, "Go to node")) {
                    if (m_onNavigateToNode) {
                        m_onNavigateToNode(line->id);
                    }
                }
                break;
        }
    }

    // Handle click on row
    // handleRowClick would be called from input handling
}

void VoiceManagerPanel::renderStatusBadge(VoiceBindingStatus status)
{
    renderer::Color color = getStatusColor(status);
    const char* text = getStatusText(status);

    // Render colored badge with text
    // Background pill shape with text
}

void VoiceManagerPanel::renderCoverageStats()
{
    if (!m_voiceManager) return;

    auto stats = m_voiceManager->getCoverageStats();

    // Stats bar at bottom
    // Total: X | Bound: Y | Unbound: Z | Missing: W | Coverage: XX%

    // Progress bar for coverage
    widgets::ProgressBarLabeled(stats.coveragePercent / 100.0f,
                                "Voice Coverage", Heights::StatusBar);
}

void VoiceManagerPanel::renderAutoMapDialog()
{
    // Modal dialog for auto-mapping
    // Shows:
    // - Pattern configuration summary
    // - Preview of what will be mapped
    // - Run/Cancel buttons

    // Would use ImGui modal popup
}

void VoiceManagerPanel::renderExportImportDialog()
{
    // Modal dialog for export/import
    // Shows:
    // - Format selection (CSV/JSON/TSV)
    // - File path input
    // - Options (export unbound only, etc.)
    // - Run/Cancel buttons
}

void VoiceManagerPanel::renderPatternConfigDialog()
{
    // Modal dialog for pattern configuration
    // Shows list of patterns with:
    // - Pattern name
    // - Regex pattern
    // - Enable/disable toggle
    // - Priority
    // - Add/remove buttons
}

const char* VoiceManagerPanel::getStatusText(VoiceBindingStatus status) const
{
    switch (status) {
        case VoiceBindingStatus::Bound: return "Bound";
        case VoiceBindingStatus::Unbound: return "Unbound";
        case VoiceBindingStatus::MissingFile: return "Missing";
        case VoiceBindingStatus::AutoMapped: return "Auto";
        case VoiceBindingStatus::Pending: return "Pending";
        default: return "Unknown";
    }
}

renderer::Color VoiceManagerPanel::getStatusColor(VoiceBindingStatus status) const
{
    switch (status) {
        case VoiceBindingStatus::Bound: return DarkPalette::VoiceBound;
        case VoiceBindingStatus::Unbound: return DarkPalette::VoiceUnbound;
        case VoiceBindingStatus::MissingFile: return DarkPalette::VoiceMissing;
        case VoiceBindingStatus::AutoMapped: return DarkPalette::VoiceAutoMapped;
        case VoiceBindingStatus::Pending: return DarkPalette::VoicePending;
        default: return DarkPalette::TextTertiary;
    }
}

void VoiceManagerPanel::setVoiceManager(VoiceManager* manager)
{
    m_voiceManager = manager;
    m_filterDirty = true;

    if (m_voiceManager) {
        m_characterList = m_voiceManager->getCharacters();
        m_sceneList = m_voiceManager->getScenes();
    }
}

void VoiceManagerPanel::refresh()
{
    if (m_voiceManager) {
        m_voiceManager->refreshVoiceFiles();
        m_voiceManager->refreshDialogueLines();
        m_characterList = m_voiceManager->getCharacters();
        m_sceneList = m_voiceManager->getScenes();
    }
    m_filterDirty = true;
}

void VoiceManagerPanel::setCharacterFilter(const std::string& character)
{
    m_filter.characterFilter = character;
    m_filterDirty = true;
}

void VoiceManagerPanel::setSceneFilter(const std::string& scene)
{
    m_filter.sceneFilter = scene;
    m_filterDirty = true;
}

void VoiceManagerPanel::setStatusFilter(VoiceBindingStatus status)
{
    m_filter.statusFilter = status;
    m_filter.showAllStatuses = false;
    m_filterDirty = true;
}

void VoiceManagerPanel::clearStatusFilter()
{
    m_filter.showAllStatuses = true;
    m_filterDirty = true;
}

void VoiceManagerPanel::setSearchText(const std::string& text)
{
    m_filter.searchText = text;
    strncpy(m_searchBuffer, text.c_str(), sizeof(m_searchBuffer) - 1);
    m_filterDirty = true;
}

void VoiceManagerPanel::clearAllFilters()
{
    m_filter = VoiceLineFilter{};
    m_searchBuffer[0] = '\0';
    m_selectedCharacterIndex = 0;
    m_selectedSceneIndex = 0;
    m_selectedStatusIndex = 0;
    m_filterDirty = true;
}

void VoiceManagerPanel::selectLine(const std::string& lineId)
{
    m_selectedLines.clear();
    m_selectedLines.push_back(lineId);
    m_lastSelectedLine = lineId;
}

void VoiceManagerPanel::clearSelection()
{
    m_selectedLines.clear();
    m_lastSelectedLine.clear();
}

void VoiceManagerPanel::previewSelectedVoice()
{
    if (!m_voiceManager || m_selectedLines.empty()) return;
    m_voiceManager->previewVoice(m_selectedLines[0]);
}

void VoiceManagerPanel::stopPreview()
{
    if (m_voiceManager) {
        m_voiceManager->stopPreview();
    }
}

void VoiceManagerPanel::bindVoiceFile(const std::string& voicePath)
{
    if (!m_voiceManager || m_selectedLines.empty()) return;

    for (const auto& lineId : m_selectedLines) {
        m_voiceManager->bindVoice(lineId, voicePath);
    }
    m_filterDirty = true;
}

void VoiceManagerPanel::unbindSelectedVoices()
{
    if (!m_voiceManager) return;

    for (const auto& lineId : m_selectedLines) {
        m_voiceManager->unbindVoice(lineId);
    }
    m_filterDirty = true;
}

void VoiceManagerPanel::runAutoMapping()
{
    if (!m_voiceManager) return;

    auto result = m_voiceManager->autoMapVoiceFiles();
    if (result.isOk()) {
        // Show success message with count
    }
    m_filterDirty = true;
}

void VoiceManagerPanel::navigateToSource()
{
    if (m_selectedLines.empty() || !m_onNavigateToNode) return;
    m_onNavigateToNode(m_selectedLines[0]);
}

void VoiceManagerPanel::exportVoiceTable(const std::string& path, VoiceTableFormat format)
{
    if (!m_voiceManager) return;
    m_voiceManager->exportVoiceTable(path, format);
}

void VoiceManagerPanel::importVoiceTable(const std::string& path, VoiceTableFormat format)
{
    if (!m_voiceManager) return;
    m_voiceManager->importVoiceTable(path, format);
    m_filterDirty = true;
}

void VoiceManagerPanel::setOnNavigateToNode(std::function<void(const std::string&)> callback)
{
    m_onNavigateToNode = std::move(callback);
}

void VoiceManagerPanel::applyFilters()
{
    m_filteredLines.clear();

    if (!m_voiceManager) return;

    m_filteredLines = m_voiceManager->getFilteredLines(m_filter);
    sortTable();
}

void VoiceManagerPanel::sortTable()
{
    // Sort m_filteredLines based on m_sortState
    std::sort(m_filteredLines.begin(), m_filteredLines.end(),
        [this](const DialogueLine* a, const DialogueLine* b) {
            i32 cmp = 0;

            switch (m_sortState.column) {
                case VoiceTableColumn::Type::LineId:
                    cmp = a->id.compare(b->id);
                    break;
                case VoiceTableColumn::Type::Character:
                    cmp = a->characterId.compare(b->characterId);
                    break;
                case VoiceTableColumn::Type::DialogueText:
                    cmp = a->text.compare(b->text);
                    break;
                case VoiceTableColumn::Type::VoiceFile:
                    cmp = a->voiceFile.compare(b->voiceFile);
                    break;
                case VoiceTableColumn::Type::Status:
                    cmp = static_cast<i32>(a->status) - static_cast<i32>(b->status);
                    break;
                case VoiceTableColumn::Type::Duration:
                    cmp = (a->voiceDuration < b->voiceDuration) ? -1 :
                          (a->voiceDuration > b->voiceDuration) ? 1 : 0;
                    break;
                case VoiceTableColumn::Type::Scene:
                    cmp = a->sceneId.compare(b->sceneId);
                    break;
                default:
                    cmp = 0;
            }

            return m_sortState.ascending ? (cmp < 0) : (cmp > 0);
        });
}

std::vector<const DialogueLine*> VoiceManagerPanel::getFilteredLines() const
{
    return m_filteredLines;
}

std::vector<MenuItem> VoiceManagerPanel::getMenuItems() const
{
    return {
        {"Refresh", "F5", [this]() { const_cast<VoiceManagerPanel*>(this)->refresh(); }},
        MenuItem::separator(),
        {"Auto-Map Voice Files...", "", [this]() {
            const_cast<VoiceManagerPanel*>(this)->m_showAutoMapDialog = true; }},
        MenuItem::separator(),
        {"Export...", "Ctrl+E", [this]() {
            auto* panel = const_cast<VoiceManagerPanel*>(this);
            panel->m_isExporting = true;
            panel->m_showExportImportDialog = true;
        }},
        {"Import...", "Ctrl+I", [this]() {
            auto* panel = const_cast<VoiceManagerPanel*>(this);
            panel->m_isExporting = false;
            panel->m_showExportImportDialog = true;
        }},
        MenuItem::separator(),
        {"Configure Patterns...", "", [this]() {
            const_cast<VoiceManagerPanel*>(this)->m_showPatternConfig = true; }}
    };
}

std::vector<ToolbarItem> VoiceManagerPanel::getToolbarItems() const
{
    return {
        {Icons::Refresh, "Refresh", [this]() { const_cast<VoiceManagerPanel*>(this)->refresh(); }},
        ToolbarItem::separator(),
        {Icons::Transform, "Auto-Map", [this]() {
            const_cast<VoiceManagerPanel*>(this)->m_showAutoMapDialog = true; }},
        ToolbarItem::separator(),
        {Icons::Export, "Export", [this]() {
            auto* panel = const_cast<VoiceManagerPanel*>(this);
            panel->m_isExporting = true;
            panel->m_showExportImportDialog = true;
        }},
        {Icons::Import, "Import", [this]() {
            auto* panel = const_cast<VoiceManagerPanel*>(this);
            panel->m_isExporting = false;
            panel->m_showExportImportDialog = true;
        }}
    };
}

std::vector<MenuItem> VoiceManagerPanel::getContextMenuItems() const
{
    std::vector<MenuItem> items;

    if (!m_selectedLines.empty()) {
        items.push_back({"Preview Voice", "", [this]() {
            const_cast<VoiceManagerPanel*>(this)->previewSelectedVoice();
        }, [this]() {
            if (m_selectedLines.empty() || !m_voiceManager) return false;
            auto* line = m_voiceManager->getLine(m_selectedLines[0]);
            return line && !line->voiceFile.empty() &&
                   line->status != VoiceBindingStatus::MissingFile;
        }});

        items.push_back({"Unbind Voice", "", [this]() {
            const_cast<VoiceManagerPanel*>(this)->unbindSelectedVoices();
        }, [this]() {
            if (m_selectedLines.empty() || !m_voiceManager) return false;
            auto* line = m_voiceManager->getLine(m_selectedLines[0]);
            return line && line->status != VoiceBindingStatus::Unbound;
        }});

        items.push_back(MenuItem::separator());

        items.push_back({"Go to Source Node", "", [this]() {
            const_cast<VoiceManagerPanel*>(this)->navigateToSource();
        }});
    }

    return items;
}

} // namespace NovelMind::editor
