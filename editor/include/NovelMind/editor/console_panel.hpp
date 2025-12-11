#pragma once

/**
 * @file console_panel.hpp
 * @brief Console Panel for NovelMind Editor v0.2.0
 *
 * The Console panel displays:
 * - Log messages (info, warning, error)
 * - Compilation output
 * - Runtime messages
 * - Search and filtering
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include "NovelMind/renderer/color.hpp"
#include <string>
#include <vector>
#include <deque>

namespace NovelMind::editor {

/**
 * @brief Log message severity
 */
enum class LogSeverity : u8 {
    Info,
    Warning,
    Error,
    Debug
};

/**
 * @brief Log message entry
 */
struct LogMessage {
    std::string text;
    std::string category;
    std::string timestamp;
    LogSeverity severity = LogSeverity::Info;
    std::string file;
    u32 line = 0;
    i32 count = 1;  // For collapse identical messages
};

/**
 * @brief Console Panel implementation
 */
class ConsolePanel : public GUIPanelBase {
public:
    ConsolePanel();
    ~ConsolePanel() override = default;

    /**
     * @brief Add a log message
     */
    void log(const std::string& message, LogSeverity severity = LogSeverity::Info,
             const std::string& category = "");

    /**
     * @brief Log with file/line info
     */
    void log(const std::string& message, const std::string& file, u32 line,
             LogSeverity severity = LogSeverity::Error);

    /**
     * @brief Clear all messages
     */
    void clear();

    /**
     * @brief Get message count by severity
     */
    [[nodiscard]] i32 getInfoCount() const { return m_infoCount; }
    [[nodiscard]] i32 getWarningCount() const { return m_warningCount; }
    [[nodiscard]] i32 getErrorCount() const { return m_errorCount; }

    /**
     * @brief Set filter by severity
     */
    void setShowInfo(bool show) { m_showInfo = show; }
    void setShowWarnings(bool show) { m_showWarnings = show; }
    void setShowErrors(bool show) { m_showErrors = show; }

    /**
     * @brief Set collapse identical messages
     */
    void setCollapse(bool collapse) { m_collapse = collapse; }

    /**
     * @brief Set auto-scroll
     */
    void setAutoScroll(bool autoScroll) { m_autoScroll = autoScroll; }

    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;

protected:
    void onInitialize() override;
    void onRender() override;
    void renderToolbar() override;

private:
    void renderMessage(const LogMessage& msg);
    renderer::Color getSeverityColor(LogSeverity severity) const;
    const char* getSeverityIcon(LogSeverity severity) const;
    void scrollToBottom();

    std::deque<LogMessage> m_messages;
    static constexpr size_t MAX_MESSAGES = 1000;

    // Filter state
    bool m_showInfo = true;
    bool m_showWarnings = true;
    bool m_showErrors = true;
    bool m_showDebug = false;
    bool m_collapse = true;
    bool m_autoScroll = true;

    // Counts
    i32 m_infoCount = 0;
    i32 m_warningCount = 0;
    i32 m_errorCount = 0;

    // Search
    char m_searchBuffer[256] = {0};
    std::string m_searchFilter;

    // Scroll state
    bool m_scrollToBottom = false;
};

} // namespace NovelMind::editor
