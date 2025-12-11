# NovelMind Visual Editor

add_library(novelmind_editor STATIC
    # Core editor
    src/editor_app.cpp
    src/editor_runtime_host.cpp
    src/diagnostics_panel.cpp
    src/asset_pipeline.cpp
    src/editor_settings.cpp
    src/voice_manager.cpp
    src/timeline_editor.cpp
    src/curve_editor.cpp

    # v0.2.0 backend systems
    src/selection_system.cpp
    src/event_bus.cpp
    src/project_manager.cpp
    src/inspector_binding.cpp
    src/asset_preview.cpp
    src/timeline_playback.cpp
    src/editor_state.cpp
    src/error_reporter.cpp

    # v0.2.0 GUI systems
    src/imgui_integration.cpp
    src/gui_panel_base.cpp
    src/scene_view_panel.cpp
    src/story_graph_panel.cpp
    src/timeline_panel.cpp
    src/inspector_panel.cpp
    src/asset_browser_panel.cpp
    src/hierarchy_panel.cpp
    src/console_panel.cpp
    src/build_settings_panel.cpp
    src/play_mode_controller.cpp

    # v0.2.0 Additional GUI panels
    src/curve_editor_panel.cpp
    src/voice_manager_panel.cpp
    src/localization_panel.cpp
    src/debug_overlay_panel.cpp
    src/hotkeys_manager.cpp
)

# Alias for backward compatibility
add_library(editor ALIAS novelmind_editor)

target_include_directories(novelmind_editor
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(novelmind_editor
    PUBLIC
        engine_core
    PRIVATE
        novelmind_compiler_options
)

# Editor executable (optional, only build if main.cpp exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    add_executable(novelmind_editor_app
        src/main.cpp
    )

    target_link_libraries(novelmind_editor_app
        PRIVATE
            novelmind_editor
            engine_core
            novelmind_compiler_options
    )

    # Set output name to novelmind_editor
    set_target_properties(novelmind_editor_app PROPERTIES
        OUTPUT_NAME "novelmind_editor"
    )
endif()
