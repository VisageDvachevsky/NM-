/**
 * @file style_guide.hpp
 * @brief Comprehensive UI Style Guide for NovelMind Editor v0.2.0
 *
 * This file defines the complete visual design system for the editor:
 * - Strict spacing system with consistent margins and padding
 * - Unified element heights and sizing
 * - Color palette with semantic meanings
 * - Typography standards
 * - Line thickness and border standards
 * - Shadow and elevation system
 * - Component-specific styling
 *
 * All panels and widgets should reference these constants to ensure
 * visual consistency across the entire editor interface.
 */

#pragma once

#include "NovelMind/core/types.hpp"
#include "NovelMind/renderer/color.hpp"

namespace NovelMind::editor {

/**
 * @brief Spacing scale based on 4px base unit
 */
namespace Spacing {
    constexpr f32 Base = 4.0f;
    constexpr f32 XXS = Base * 0.5f;   // 2px - Minimal spacing
    constexpr f32 XS = Base;            // 4px - Tight spacing
    constexpr f32 SM = Base * 2;        // 8px - Small spacing
    constexpr f32 MD = Base * 3;        // 12px - Medium spacing
    constexpr f32 LG = Base * 4;        // 16px - Large spacing
    constexpr f32 XL = Base * 6;        // 24px - Extra large spacing
    constexpr f32 XXL = Base * 8;       // 32px - Section spacing

    // Standard paddings
    constexpr f32 WindowPadding = SM;
    constexpr f32 FramePadding = XS;
    constexpr f32 ItemSpacing = SM;
    constexpr f32 ItemInnerSpacing = XS;
    constexpr f32 IndentSpacing = LG;
    constexpr f32 ScrollbarSize = MD;
    constexpr f32 GrabMinSize = SM;
    constexpr f32 ToolbarPadding = XS;
    constexpr f32 PanelMargin = SM;
} // namespace Spacing

/**
 * @brief Standard element heights
 */
namespace Heights {
    constexpr f32 InputField = 24.0f;
    constexpr f32 Button = 24.0f;
    constexpr f32 ButtonSmall = 20.0f;
    constexpr f32 ButtonLarge = 32.0f;
    constexpr f32 ToolbarButton = 24.0f;
    constexpr f32 Toolbar = 32.0f;
    constexpr f32 TabBar = 28.0f;
    constexpr f32 Tab = 24.0f;
    constexpr f32 MenuBar = 24.0f;
    constexpr f32 MenuItem = 22.0f;
    constexpr f32 TreeNode = 22.0f;
    constexpr f32 ListItem = 24.0f;
    constexpr f32 TableRow = 24.0f;
    constexpr f32 Header = 28.0f;
    constexpr f32 SectionHeader = 20.0f;
    constexpr f32 StatusBar = 24.0f;
    constexpr f32 Scrollbar = 12.0f;
    constexpr f32 Separator = 1.0f;
    constexpr f32 DragHandle = 4.0f;
} // namespace Heights

/**
 * @brief Standard widths
 */
namespace Widths {
    constexpr f32 PropertyLabel = 120.0f;
    constexpr f32 PropertyLabelWide = 160.0f;
    constexpr f32 PropertyLabelNarrow = 80.0f;
    constexpr f32 IconButton = 24.0f;
    constexpr f32 IconButtonSmall = 20.0f;
    constexpr f32 DropdownMin = 100.0f;
    constexpr f32 SearchInput = 200.0f;
    constexpr f32 ColorPicker = 24.0f;
    constexpr f32 Scrollbar = 12.0f;
    constexpr f32 InspectorMin = 250.0f;
    constexpr f32 InspectorMax = 450.0f;
    constexpr f32 HierarchyMin = 180.0f;
    constexpr f32 HierarchyMax = 350.0f;
    constexpr f32 AssetBrowserMin = 250.0f;
    constexpr f32 ConsoleMin = 200.0f;
    constexpr f32 Minimap = 150.0f;
} // namespace Widths

/**
 * @brief Border and line thickness
 */
namespace LineThickness {
    constexpr f32 None = 0.0f;
    constexpr f32 Hairline = 0.5f;
    constexpr f32 Thin = 1.0f;
    constexpr f32 Normal = 1.5f;
    constexpr f32 Medium = 2.0f;
    constexpr f32 Thick = 3.0f;
    constexpr f32 Bold = 4.0f;

    // Specific uses
    constexpr f32 WindowBorder = Thin;
    constexpr f32 FrameBorder = None;
    constexpr f32 PopupBorder = Thin;
    constexpr f32 SeparatorLine = Thin;
    constexpr f32 SelectionBorder = Medium;
    constexpr f32 FocusBorder = Medium;
    constexpr f32 ConnectionLine = Normal;
    constexpr f32 ConnectionLineSelected = Medium;
    constexpr f32 GridLine = Hairline;
    constexpr f32 GridLineMajor = Thin;
    constexpr f32 TimelinePlayhead = Medium;
    constexpr f32 CurveLine = Medium;
} // namespace LineThickness

/**
 * @brief Corner rounding
 */
namespace Rounding {
    constexpr f32 None = 0.0f;
    constexpr f32 Small = 2.0f;
    constexpr f32 Medium = 4.0f;
    constexpr f32 Large = 6.0f;
    constexpr f32 XLarge = 8.0f;
    constexpr f32 Full = 9999.0f;  // For pills/circles

    // Specific uses
    constexpr f32 Window = Medium;
    constexpr f32 Frame = Small;
    constexpr f32 Button = Small;
    constexpr f32 Scrollbar = Small;
    constexpr f32 Grab = Small;
    constexpr f32 Tab = Small;
    constexpr f32 Popup = Medium;
    constexpr f32 Tooltip = Small;
    constexpr f32 Card = Medium;
    constexpr f32 Node = Medium;
    constexpr f32 Badge = Full;
} // namespace Rounding

/**
 * @brief Shadow/elevation system
 */
namespace Elevation {
    // Shadow offsets (x, y)
    constexpr f32 Level0OffsetY = 0.0f;
    constexpr f32 Level1OffsetY = 1.0f;
    constexpr f32 Level2OffsetY = 2.0f;
    constexpr f32 Level3OffsetY = 4.0f;
    constexpr f32 Level4OffsetY = 8.0f;

    // Shadow blur radii
    constexpr f32 Level0Blur = 0.0f;
    constexpr f32 Level1Blur = 2.0f;
    constexpr f32 Level2Blur = 4.0f;
    constexpr f32 Level3Blur = 8.0f;
    constexpr f32 Level4Blur = 16.0f;

    // Shadow opacity (0-255)
    constexpr u8 Level0Alpha = 0;
    constexpr u8 Level1Alpha = 30;
    constexpr u8 Level2Alpha = 40;
    constexpr u8 Level3Alpha = 50;
    constexpr u8 Level4Alpha = 60;
} // namespace Elevation

/**
 * @brief Typography standards
 */
namespace Typography {
    // Font sizes
    constexpr f32 FontSizeXS = 10.0f;
    constexpr f32 FontSizeSM = 12.0f;
    constexpr f32 FontSizeMD = 14.0f;  // Default/body text
    constexpr f32 FontSizeLG = 16.0f;
    constexpr f32 FontSizeXL = 18.0f;
    constexpr f32 FontSizeXXL = 24.0f;

    // Line heights (multipliers)
    constexpr f32 LineHeightTight = 1.2f;
    constexpr f32 LineHeightNormal = 1.5f;
    constexpr f32 LineHeightRelaxed = 1.75f;

    // Icon sizes
    constexpr f32 IconSizeXS = 12.0f;
    constexpr f32 IconSizeSM = 14.0f;
    constexpr f32 IconSizeMD = 16.0f;
    constexpr f32 IconSizeLG = 20.0f;
    constexpr f32 IconSizeXL = 24.0f;
    constexpr f32 IconSizeXXL = 32.0f;
} // namespace Typography

/**
 * @brief Animation/transition durations (in milliseconds)
 */
namespace Animation {
    constexpr i32 Instant = 0;
    constexpr i32 Fast = 100;
    constexpr i32 Normal = 200;
    constexpr i32 Slow = 300;
    constexpr i32 Slower = 500;

    // Specific uses
    constexpr i32 Hover = Fast;
    constexpr i32 Click = Fast;
    constexpr i32 Expand = Normal;
    constexpr i32 Collapse = Normal;
    constexpr i32 FadeIn = Normal;
    constexpr i32 FadeOut = Fast;
    constexpr i32 Slide = Slow;
    constexpr i32 Toast = Slower;
} // namespace Animation

/**
 * @brief Z-index layers
 */
namespace ZIndex {
    constexpr i32 Background = 0;
    constexpr i32 Default = 10;
    constexpr i32 Panel = 100;
    constexpr i32 Dropdown = 200;
    constexpr i32 Overlay = 300;
    constexpr i32 Modal = 400;
    constexpr i32 Popup = 500;
    constexpr i32 Tooltip = 600;
    constexpr i32 Toast = 700;
    constexpr i32 Debug = 1000;
} // namespace ZIndex

/**
 * @brief Complete color palette for dark theme
 */
namespace DarkPalette {
    // Base colors
    constexpr renderer::Color Background{30, 30, 30, 255};
    constexpr renderer::Color BackgroundDark{20, 20, 20, 255};
    constexpr renderer::Color BackgroundLight{45, 45, 45, 255};
    constexpr renderer::Color BackgroundHover{55, 55, 55, 255};
    constexpr renderer::Color BackgroundActive{65, 65, 65, 255};
    constexpr renderer::Color Surface{40, 40, 40, 255};
    constexpr renderer::Color SurfaceHover{50, 50, 50, 255};
    constexpr renderer::Color SurfaceActive{60, 60, 60, 255};

    // Text colors
    constexpr renderer::Color Text{220, 220, 220, 255};
    constexpr renderer::Color TextSecondary{160, 160, 160, 255};
    constexpr renderer::Color TextTertiary{120, 120, 120, 255};
    constexpr renderer::Color TextDisabled{80, 80, 80, 255};
    constexpr renderer::Color TextInverse{30, 30, 30, 255};
    constexpr renderer::Color TextHighlight{255, 255, 255, 255};

    // Primary accent (blue)
    constexpr renderer::Color Primary{0, 122, 204, 255};
    constexpr renderer::Color PrimaryHover{0, 142, 224, 255};
    constexpr renderer::Color PrimaryActive{0, 102, 184, 255};
    constexpr renderer::Color PrimarySubtle{0, 122, 204, 40};

    // Secondary accent (purple)
    constexpr renderer::Color Secondary{138, 43, 226, 255};
    constexpr renderer::Color SecondaryHover{148, 53, 236, 255};
    constexpr renderer::Color SecondaryActive{128, 33, 216, 255};

    // Semantic colors
    constexpr renderer::Color Success{76, 175, 80, 255};
    constexpr renderer::Color SuccessHover{86, 185, 90, 255};
    constexpr renderer::Color SuccessSubtle{76, 175, 80, 40};

    constexpr renderer::Color Warning{255, 193, 7, 255};
    constexpr renderer::Color WarningHover{255, 203, 17, 255};
    constexpr renderer::Color WarningSubtle{255, 193, 7, 40};

    constexpr renderer::Color Error{244, 67, 54, 255};
    constexpr renderer::Color ErrorHover{254, 77, 64, 255};
    constexpr renderer::Color ErrorSubtle{244, 67, 54, 40};

    constexpr renderer::Color Info{33, 150, 243, 255};
    constexpr renderer::Color InfoHover{43, 160, 253, 255};
    constexpr renderer::Color InfoSubtle{33, 150, 243, 40};

    // Borders
    constexpr renderer::Color Border{60, 60, 60, 255};
    constexpr renderer::Color BorderLight{80, 80, 80, 255};
    constexpr renderer::Color BorderDark{40, 40, 40, 255};
    constexpr renderer::Color BorderFocus{0, 122, 204, 255};

    // Selection
    constexpr renderer::Color Selection{0, 122, 204, 128};
    constexpr renderer::Color SelectionInactive{100, 100, 100, 80};
    constexpr renderer::Color Highlight{255, 255, 0, 60};

    // Scrollbar
    constexpr renderer::Color ScrollbarTrack{30, 30, 30, 255};
    constexpr renderer::Color ScrollbarThumb{80, 80, 80, 255};
    constexpr renderer::Color ScrollbarThumbHover{100, 100, 100, 255};
    constexpr renderer::Color ScrollbarThumbActive{120, 120, 120, 255};

    // Graph/Node editor
    constexpr renderer::Color NodeBackground{40, 40, 40, 230};
    constexpr renderer::Color NodeBackgroundSelected{50, 50, 60, 230};
    constexpr renderer::Color NodeBorder{80, 80, 80, 255};
    constexpr renderer::Color NodeBorderSelected{0, 122, 204, 255};
    constexpr renderer::Color NodeHeader{60, 60, 60, 255};
    constexpr renderer::Color NodeHeaderSelected{70, 70, 80, 255};
    constexpr renderer::Color ConnectionLine{150, 150, 150, 255};
    constexpr renderer::Color ConnectionLineActive{0, 122, 204, 255};
    constexpr renderer::Color ConnectionLineDragging{100, 200, 255, 255};
    constexpr renderer::Color CanvasBackground{25, 25, 25, 255};
    constexpr renderer::Color Grid{40, 40, 40, 255};
    constexpr renderer::Color GridMajor{50, 50, 50, 255};

    // Timeline
    constexpr renderer::Color TimelineBackground{25, 25, 25, 255};
    constexpr renderer::Color TimelineRuler{40, 40, 40, 255};
    constexpr renderer::Color TimelineRulerText{120, 120, 120, 255};
    constexpr renderer::Color TimelineTrack{35, 35, 35, 255};
    constexpr renderer::Color TimelineTrackAlt{32, 32, 32, 255};
    constexpr renderer::Color TimelineKeyframe{0, 122, 204, 255};
    constexpr renderer::Color TimelineKeyframeSelected{100, 180, 255, 255};
    constexpr renderer::Color TimelinePlayhead{255, 100, 100, 255};
    constexpr renderer::Color TimelineCursor{255, 255, 255, 128};

    // Voice/Audio status colors
    constexpr renderer::Color VoiceBound{76, 175, 80, 255};
    constexpr renderer::Color VoiceUnbound{120, 120, 120, 255};
    constexpr renderer::Color VoiceMissing{244, 67, 54, 255};
    constexpr renderer::Color VoiceAutoMapped{33, 150, 243, 255};
    constexpr renderer::Color VoicePending{255, 193, 7, 255};

    // Localization status colors
    constexpr renderer::Color LocaleComplete{76, 175, 80, 255};
    constexpr renderer::Color LocaleMissing{244, 67, 54, 255};
    constexpr renderer::Color LocaleOutdated{255, 193, 7, 255};
} // namespace DarkPalette

/**
 * @brief Complete color palette for light theme
 */
namespace LightPalette {
    // Base colors
    constexpr renderer::Color Background{245, 245, 245, 255};
    constexpr renderer::Color BackgroundDark{230, 230, 230, 255};
    constexpr renderer::Color BackgroundLight{255, 255, 255, 255};
    constexpr renderer::Color BackgroundHover{235, 235, 235, 255};
    constexpr renderer::Color BackgroundActive{225, 225, 225, 255};
    constexpr renderer::Color Surface{255, 255, 255, 255};
    constexpr renderer::Color SurfaceHover{248, 248, 248, 255};
    constexpr renderer::Color SurfaceActive{240, 240, 240, 255};

    // Text colors
    constexpr renderer::Color Text{40, 40, 40, 255};
    constexpr renderer::Color TextSecondary{100, 100, 100, 255};
    constexpr renderer::Color TextTertiary{140, 140, 140, 255};
    constexpr renderer::Color TextDisabled{180, 180, 180, 255};
    constexpr renderer::Color TextInverse{255, 255, 255, 255};
    constexpr renderer::Color TextHighlight{0, 0, 0, 255};

    // Primary accent (blue)
    constexpr renderer::Color Primary{0, 120, 215, 255};
    constexpr renderer::Color PrimaryHover{0, 100, 195, 255};
    constexpr renderer::Color PrimaryActive{0, 80, 175, 255};
    constexpr renderer::Color PrimarySubtle{0, 120, 215, 30};

    // Borders
    constexpr renderer::Color Border{200, 200, 200, 255};
    constexpr renderer::Color BorderLight{220, 220, 220, 255};
    constexpr renderer::Color BorderDark{180, 180, 180, 255};
    constexpr renderer::Color BorderFocus{0, 120, 215, 255};

    // Selection
    constexpr renderer::Color Selection{0, 120, 215, 80};
    constexpr renderer::Color SelectionInactive{180, 180, 180, 80};
    constexpr renderer::Color Highlight{255, 255, 0, 60};
} // namespace LightPalette

/**
 * @brief Icon set identifiers (for icon font or SVG)
 *
 * Icons are defined as UTF-8 codepoints for icon fonts like
 * Font Awesome, Material Icons, or custom icon fonts.
 */
namespace Icons {
    // File operations
    constexpr const char* New = "\xef\x85\x9b";           // file-plus
    constexpr const char* Open = "\xef\x81\xbc";          // folder-open
    constexpr const char* Save = "\xef\x83\x87";          // floppy-disk
    constexpr const char* SaveAs = "\xef\x83\x87";        // save
    constexpr const char* Import = "\xef\x81\x99";        // download
    constexpr const char* Export = "\xef\x82\x93";        // upload
    constexpr const char* Close = "\xef\x80\x8d";         // times

    // Edit operations
    constexpr const char* Undo = "\xef\x83\xa2";          // rotate-left
    constexpr const char* Redo = "\xef\x80\x9e";          // rotate-right
    constexpr const char* Cut = "\xef\x83\x84";           // scissors
    constexpr const char* Copy = "\xef\x83\x85";          // copy
    constexpr const char* Paste = "\xef\x83\xaa";         // clipboard
    constexpr const char* Delete = "\xef\x80\x8d";        // trash
    constexpr const char* Duplicate = "\xef\x83\x85";     // clone

    // Playback controls
    constexpr const char* Play = "\xef\x81\x8b";          // play
    constexpr const char* Pause = "\xef\x81\x8c";         // pause
    constexpr const char* Stop = "\xef\x81\x8d";          // stop
    constexpr const char* StepForward = "\xef\x81\x91";   // step-forward
    constexpr const char* StepBackward = "\xef\x81\x88"; // step-backward
    constexpr const char* FastForward = "\xef\x81\x90";   // forward
    constexpr const char* Rewind = "\xef\x81\x8a";        // backward

    // View/Layout
    constexpr const char* Grid = "\xef\x80\x8e";          // th
    constexpr const char* List = "\xef\x80\xb9";          // list
    constexpr const char* Tree = "\xef\x86\x82";          // sitemap
    constexpr const char* Maximize = "\xef\x82\x98";      // expand
    constexpr const char* Minimize = "\xef\x81\xa6";      // compress
    constexpr const char* ZoomIn = "\xef\x80\x8e";        // search-plus
    constexpr const char* ZoomOut = "\xef\x80\x90";       // search-minus
    constexpr const char* ZoomFit = "\xef\x81\xae";       // expand-arrows
    constexpr const char* ZoomReset = "\xef\x80\x8e";     // search (reset zoom)

    // Objects
    constexpr const char* Object = "\xef\x87\x86";        // cube
    constexpr const char* Character = "\xef\x80\x87";     // user
    constexpr const char* Background = "\xef\x80\xbe";    // image
    constexpr const char* Scene = "\xef\x81\x88";         // film
    constexpr const char* Node = "\xef\x86\x82";          // circle
    constexpr const char* Connection = "\xef\x83\x9e";    // link

    // Asset types
    constexpr const char* Image = "\xef\x80\xbe";         // image
    constexpr const char* Audio = "\xef\x80\xa8";         // music
    constexpr const char* Video = "\xef\x80\x88";         // video
    constexpr const char* Script = "\xef\x82\x89";        // code
    constexpr const char* Font = "\xef\x80\xb1";          // font
    constexpr const char* Folder = "\xef\x81\xbb";        // folder
    constexpr const char* File = "\xef\x85\x9b";          // file

    // Tools
    constexpr const char* Select = "\xef\x89\x85";        // mouse-pointer
    constexpr const char* Move = "\xef\x81\xb2";          // arrows-alt
    constexpr const char* Rotate = "\xef\x80\xa1";        // sync-alt
    constexpr const char* Scale = "\xef\x82\x98";         // expand-arrows
    constexpr const char* Hand = "\xef\x89\x85";          // hand-paper
    constexpr const char* Eyedropper = "\xef\x86\xbb";    // eyedropper
    constexpr const char* Transform = "\xef\x80\xa1";     // sync/transform

    // Status indicators
    constexpr const char* Success = "\xef\x80\x8c";       // check
    constexpr const char* Warning = "\xef\x81\xb1";       // exclamation-triangle
    constexpr const char* Error = "\xef\x81\x97";         // times-circle
    constexpr const char* Info = "\xef\x81\x9a";          // info-circle
    constexpr const char* Question = "\xef\x81\x99";      // question-circle
    constexpr const char* Loading = "\xef\x86\x90";       // spinner

    // Panels
    constexpr const char* Inspector = "\xef\x85\x88";     // sliders-h
    constexpr const char* Hierarchy = "\xef\x86\x82";     // sitemap
    constexpr const char* Console = "\xef\x84\xa0";       // terminal
    constexpr const char* Assets = "\xef\x81\xbb";        // folder
    constexpr const char* Timeline = "\xef\x80\x87";      // clock
    constexpr const char* StoryGraph = "\xef\x86\x82";    // project-diagram
    constexpr const char* SceneView = "\xef\x80\x88";     // desktop
    constexpr const char* Voice = "\xef\x82\xa0";         // microphone
    constexpr const char* Localization = "\xef\x80\xac";  // globe
    constexpr const char* Diagnostics = "\xef\x81\x88";   // bug
    constexpr const char* Build = "\xef\x84\xa6";         // wrench
    constexpr const char* Settings = "\xef\x80\x93";      // cog
    constexpr const char* Curve = "\xef\x86\x80";         // bezier-curve

    // Navigation
    constexpr const char* ArrowUp = "\xef\x81\x82";       // chevron-up
    constexpr const char* ArrowDown = "\xef\x81\x83";     // chevron-down
    constexpr const char* ArrowLeft = "\xef\x81\x84";     // chevron-left
    constexpr const char* ArrowRight = "\xef\x81\x85";    // chevron-right
    constexpr const char* Home = "\xef\x80\x95";          // home
    constexpr const char* Refresh = "\xef\x80\xa1";       // sync

    // Misc
    constexpr const char* Search = "\xef\x80\x82";        // search
    constexpr const char* Filter = "\xef\x82\xb0";        // filter
    constexpr const char* Sort = "\xef\x80\xb7";          // sort
    constexpr const char* Lock = "\xef\x80\xa3";          // lock
    constexpr const char* Unlock = "\xef\x82\x9c";        // unlock
    constexpr const char* Visible = "\xef\x81\xae";       // eye
    constexpr const char* Hidden = "\xef\x81\xb0";        // eye-slash
    constexpr const char* Pin = "\xef\x82\x8d";           // thumbtack
    constexpr const char* Link = "\xef\x83\x9e";          // link
    constexpr const char* Unlink = "\xef\x84\xa7";        // unlink
    constexpr const char* Plus = "\xef\x81\xa7";          // plus
    constexpr const char* Minus = "\xef\x81\xa8";         // minus
    constexpr const char* Dots = "\xef\x85\x81";          // ellipsis-h
    constexpr const char* DotsVertical = "\xef\x85\x82";  // ellipsis-v
    constexpr const char* Drag = "\xef\x89\x85";          // grip-vertical
    constexpr const char* More = "\xef\x85\x81";          // ellipsis-h
} // namespace Icons

/**
 * @brief Style presets for common UI components
 */
struct ComponentStyles {
    // Button variants
    struct ButtonStyle {
        renderer::Color background;
        renderer::Color backgroundHover;
        renderer::Color backgroundActive;
        renderer::Color text;
        renderer::Color border;
        f32 rounding = Rounding::Button;
        f32 borderWidth = LineThickness::None;
        f32 height = Heights::Button;
    };

    static ButtonStyle PrimaryButton() {
        return {
            DarkPalette::Primary,
            DarkPalette::PrimaryHover,
            DarkPalette::PrimaryActive,
            {255, 255, 255, 255},
            DarkPalette::Primary
        };
    }

    static ButtonStyle SecondaryButton() {
        return {
            DarkPalette::Surface,
            DarkPalette::SurfaceHover,
            DarkPalette::SurfaceActive,
            DarkPalette::Text,
            DarkPalette::Border,
            Rounding::Button,
            LineThickness::Thin
        };
    }

    static ButtonStyle GhostButton() {
        return {
            {0, 0, 0, 0},
            DarkPalette::SurfaceHover,
            DarkPalette::SurfaceActive,
            DarkPalette::Text,
            {0, 0, 0, 0}
        };
    }

    static ButtonStyle DangerButton() {
        return {
            DarkPalette::Error,
            DarkPalette::ErrorHover,
            DarkPalette::Error,
            {255, 255, 255, 255},
            DarkPalette::Error
        };
    }

    // Input field style
    struct InputStyle {
        renderer::Color background;
        renderer::Color backgroundHover;
        renderer::Color backgroundFocus;
        renderer::Color text;
        renderer::Color placeholder;
        renderer::Color border;
        renderer::Color borderFocus;
        f32 rounding = Rounding::Frame;
        f32 borderWidth = LineThickness::Thin;
        f32 height = Heights::InputField;
    };

    static InputStyle DefaultInput() {
        return {
            DarkPalette::BackgroundDark,
            DarkPalette::BackgroundLight,
            DarkPalette::BackgroundLight,
            DarkPalette::Text,
            DarkPalette::TextTertiary,
            DarkPalette::Border,
            DarkPalette::BorderFocus
        };
    }
};

} // namespace NovelMind::editor
