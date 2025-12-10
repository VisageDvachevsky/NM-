/**
 * @file build_settings_panel.cpp
 * @brief Build Settings Panel implementation
 */

#include "NovelMind/editor/build_settings_panel.hpp"
#include "NovelMind/editor/imgui_integration.hpp"
#include <cstring>

namespace NovelMind::editor {

BuildSettingsPanel::BuildSettingsPanel()
    : GUIPanelBase("Build Settings")
{
}

void BuildSettingsPanel::startBuild()
{
    if (m_isBuilding)
    {
        return;
    }

    m_isBuilding = true;
    m_buildProgress = 0.0f;
    m_buildStatus = "Starting build...";

    // Would trigger actual build process
    // For now, simulate progress in onUpdate
}

std::vector<ToolbarItem> BuildSettingsPanel::getToolbarItems() const
{
    std::vector<ToolbarItem> items;

    items.push_back({"Build", "Build Project", [this]() { const_cast<BuildSettingsPanel*>(this)->startBuild(); },
                     [this]() { return !m_isBuilding; }});
    items.push_back({"Build & Run", "Build and Run Project", []() { /* Build and run */ },
                     [this]() { return !m_isBuilding; }});

    return items;
}

void BuildSettingsPanel::onInitialize()
{
    // Load saved build settings
}

void BuildSettingsPanel::onUpdate(f64 deltaTime)
{
    // Simulate build progress
    if (m_isBuilding)
    {
        m_buildProgress += static_cast<f32>(deltaTime) * 0.1f;

        if (m_buildProgress < 0.2f)
        {
            m_buildStatus = "Validating scripts...";
        }
        else if (m_buildProgress < 0.4f)
        {
            m_buildStatus = "Compiling story graph...";
        }
        else if (m_buildProgress < 0.6f)
        {
            m_buildStatus = "Processing assets...";
        }
        else if (m_buildProgress < 0.8f)
        {
            m_buildStatus = "Packaging...";
        }
        else if (m_buildProgress < 1.0f)
        {
            m_buildStatus = "Finalizing build...";
        }
        else
        {
            m_isBuilding = false;
            m_buildProgress = 1.0f;
            m_buildStatus = "Build completed successfully!";
        }
    }
}

void BuildSettingsPanel::onRender()
{
    renderPlatformSection();
    renderOutputSection();
    renderAssetSection();
    renderLocalizationSection();
    renderWindowSection();
    renderAdvancedSection();

    // ImGui::Separator();

    renderBuildButton();
}

void BuildSettingsPanel::renderPlatformSection()
{
    if (widgets::CollapsingHeader("Platform"))
    {
        // Platform selection
        std::vector<std::string> platforms = {"Windows", "macOS", "Linux", "Web", "Android", "iOS"};
        i32 platformIndex = static_cast<i32>(m_settings.platform);
        if (widgets::Dropdown("Target Platform", &platformIndex, platforms))
        {
            m_settings.platform = static_cast<BuildPlatform>(platformIndex);
        }

        // Configuration
        std::vector<std::string> configs = {"Debug", "Release", "Distribution"};
        i32 configIndex = static_cast<i32>(m_settings.config);
        if (widgets::Dropdown("Configuration", &configIndex, configs))
        {
            m_settings.config = static_cast<BuildConfig>(configIndex);
        }
    }
}

void BuildSettingsPanel::renderOutputSection()
{
    if (widgets::CollapsingHeader("Output"))
    {
        char outputBuffer[256];
        std::strncpy(outputBuffer, m_settings.outputPath.c_str(), sizeof(outputBuffer) - 1);
        // if (ImGui::InputText("Output Path", outputBuffer, sizeof(outputBuffer)))
        // {
        //     m_settings.outputPath = outputBuffer;
        // }
        // ImGui::SameLine();
        // if (ImGui::Button("Browse..."))
        // {
        //     // File dialog
        // }

        char nameBuffer[256];
        std::strncpy(nameBuffer, m_settings.productName.c_str(), sizeof(nameBuffer) - 1);
        // if (ImGui::InputText("Product Name", nameBuffer, sizeof(nameBuffer)))
        // {
        //     m_settings.productName = nameBuffer;
        // }

        char versionBuffer[64];
        std::strncpy(versionBuffer, m_settings.version.c_str(), sizeof(versionBuffer) - 1);
        // if (ImGui::InputText("Version", versionBuffer, sizeof(versionBuffer)))
        // {
        //     m_settings.version = versionBuffer;
        // }

        char companyBuffer[256];
        std::strncpy(companyBuffer, m_settings.companyName.c_str(), sizeof(companyBuffer) - 1);
        // if (ImGui::InputText("Company Name", companyBuffer, sizeof(companyBuffer)))
        // {
        //     m_settings.companyName = companyBuffer;
        // }

        char bundleBuffer[256];
        std::strncpy(bundleBuffer, m_settings.bundleIdentifier.c_str(), sizeof(bundleBuffer) - 1);
        // if (ImGui::InputText("Bundle Identifier", bundleBuffer, sizeof(bundleBuffer)))
        // {
        //     m_settings.bundleIdentifier = bundleBuffer;
        // }

        (void)outputBuffer;
        (void)nameBuffer;
        (void)versionBuffer;
        (void)companyBuffer;
        (void)bundleBuffer;
    }
}

void BuildSettingsPanel::renderAssetSection()
{
    if (widgets::CollapsingHeader("Assets"))
    {
        // ImGui::Checkbox("Compress Assets", &m_settings.compressAssets);
        // ImGui::Checkbox("Encrypt Assets", &m_settings.encryptAssets);

        std::vector<std::string> qualities = {"Low", "Medium", "High"};
        widgets::Dropdown("Texture Quality", &m_settings.textureQuality, qualities);
    }
}

void BuildSettingsPanel::renderLocalizationSection()
{
    if (widgets::CollapsingHeader("Localization"))
    {
        // ImGui::Text("Languages:");

        // Would show list of languages with checkboxes
        // ImGui::Text("  English (en)");
        // ImGui::Text("  Japanese (ja)");

        // Default language dropdown
        // ImGui::InputText("Default Language", ...);
    }
}

void BuildSettingsPanel::renderWindowSection()
{
    if (widgets::CollapsingHeader("Window"))
    {
        // ImGui::DragInt("Width", &m_settings.windowWidth, 1, 640, 3840);
        // ImGui::DragInt("Height", &m_settings.windowHeight, 1, 480, 2160);

        // ImGui::Checkbox("Fullscreen by Default", &m_settings.fullscreenByDefault);
        // ImGui::Checkbox("Allow Resizing", &m_settings.allowResizing);
    }
}

void BuildSettingsPanel::renderAdvancedSection()
{
    if (widgets::CollapsingHeader("Advanced", nullptr, false))
    {
        // ImGui::Checkbox("Development Build", &m_settings.includeDevelopmentBuild);
        // ImGui::Checkbox("Strip Debug Symbols", &m_settings.stripDebugSymbols);
    }
}

void BuildSettingsPanel::renderBuildButton()
{
    if (m_isBuilding)
    {
        // Show progress
        widgets::ProgressBarLabeled(m_buildProgress, m_buildStatus.c_str());

        // Cancel button
        // if (ImGui::Button("Cancel"))
        // {
        //     m_isBuilding = false;
        //     m_buildStatus = "Build cancelled";
        // }
    }
    else
    {
        // Build buttons
        // ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.2f, 1.0f));
        // if (ImGui::Button("Build", ImVec2(100, 30)))
        // {
        //     startBuild();
        // }
        // ImGui::PopStyleColor();

        // ImGui::SameLine();

        // if (ImGui::Button("Build and Run", ImVec2(120, 30)))
        // {
        //     startBuild();
        //     // Would run after build completes
        // }

        // Show last build status
        if (!m_buildStatus.empty())
        {
            // ImGui::Text("%s", m_buildStatus.c_str());
        }
    }
}

const char* BuildSettingsPanel::getPlatformName(BuildPlatform platform) const
{
    switch (platform)
    {
        case BuildPlatform::Windows: return "Windows";
        case BuildPlatform::macOS: return "macOS";
        case BuildPlatform::Linux: return "Linux";
        case BuildPlatform::Web: return "Web";
        case BuildPlatform::Android: return "Android";
        case BuildPlatform::iOS: return "iOS";
        default: return "Unknown";
    }
}

const char* BuildSettingsPanel::getConfigName(BuildConfig config) const
{
    switch (config)
    {
        case BuildConfig::Debug: return "Debug";
        case BuildConfig::Release: return "Release";
        case BuildConfig::Distribution: return "Distribution";
        default: return "Unknown";
    }
}

} // namespace NovelMind::editor
