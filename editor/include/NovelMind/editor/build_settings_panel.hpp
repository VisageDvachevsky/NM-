#pragma once

/**
 * @file build_settings_panel.hpp
 * @brief Build Settings Panel for NovelMind Editor v0.2.0
 *
 * The Build Settings panel provides:
 * - Target platform selection
 * - Build configuration (debug/release)
 * - Output settings
 * - Asset compression options
 * - Localization settings
 * - Build button and progress
 */

#include "NovelMind/editor/gui_panel_base.hpp"
#include <string>
#include <vector>

namespace NovelMind::editor {

/**
 * @brief Target platform
 */
enum class BuildPlatform : u8 {
    Windows,
    macOS,
    Linux,
    Web,
    Android,
    iOS
};

/**
 * @brief Build configuration
 */
enum class BuildConfig : u8 {
    Debug,
    Release,
    Distribution
};

/**
 * @brief Build settings data
 */
struct BuildSettings {
    BuildPlatform platform = BuildPlatform::Windows;
    BuildConfig config = BuildConfig::Release;
    std::string outputPath = "build";
    std::string productName = "MyVisualNovel";
    std::string version = "1.0.0";
    std::string companyName = "";
    std::string bundleIdentifier = "com.company.game";

    // Asset settings
    bool compressAssets = true;
    bool encryptAssets = false;
    i32 textureQuality = 2;  // 0=low, 1=medium, 2=high

    // Localization
    std::vector<std::string> languages = {"en"};
    std::string defaultLanguage = "en";

    // Window settings
    i32 windowWidth = 1920;
    i32 windowHeight = 1080;
    bool fullscreenByDefault = false;
    bool allowResizing = true;

    // Advanced
    bool includeDevelopmentBuild = false;
    bool stripDebugSymbols = true;
};

/**
 * @brief Build Settings Panel implementation
 */
class BuildSettingsPanel : public GUIPanelBase {
public:
    BuildSettingsPanel();
    ~BuildSettingsPanel() override = default;

    /**
     * @brief Get current build settings
     */
    [[nodiscard]] const BuildSettings& getSettings() const { return m_settings; }

    /**
     * @brief Set build settings
     */
    void setSettings(const BuildSettings& settings) { m_settings = settings; }

    /**
     * @brief Start a build
     */
    void startBuild();

    /**
     * @brief Check if currently building
     */
    [[nodiscard]] bool isBuilding() const { return m_isBuilding; }

    /**
     * @brief Get build progress (0-1)
     */
    [[nodiscard]] f32 getBuildProgress() const { return m_buildProgress; }

    [[nodiscard]] std::vector<ToolbarItem> getToolbarItems() const override;

protected:
    void onInitialize() override;
    void onUpdate(f64 deltaTime) override;
    void onRender() override;

private:
    void renderPlatformSection();
    void renderOutputSection();
    void renderAssetSection();
    void renderLocalizationSection();
    void renderWindowSection();
    void renderAdvancedSection();
    void renderBuildButton();

    const char* getPlatformName(BuildPlatform platform) const;
    const char* getConfigName(BuildConfig config) const;

    BuildSettings m_settings;
    bool m_isBuilding = false;
    f32 m_buildProgress = 0.0f;
    std::string m_buildStatus;
};

} // namespace NovelMind::editor
