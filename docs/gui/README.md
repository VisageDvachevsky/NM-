# NovelMind Editor GUI Documentation

## Overview

The NovelMind Editor v0.2.0 provides a comprehensive graphical user interface for creating visual novels. This documentation covers all GUI panels, their features, and usage guidelines.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Main Editor Window](#main-editor-window)
3. [Panel Reference](#panel-reference)
4. [Keyboard Shortcuts](#keyboard-shortcuts)
5. [Style Guide](#style-guide)
6. [Customization](#customization)

---

## Getting Started

### Launching the Editor

The NovelMind Editor can be launched from the command line:

```bash
novelmind-editor [project_path]
```

### First Launch

On first launch, the editor will:
1. Create a default layout with commonly used panels
2. Apply the dark theme
3. Open the Welcome screen

### Opening a Project

1. Use **File > Open Project** (Ctrl+O)
2. Navigate to your project folder
3. Select the project file (`.nmproj`)

---

## Main Editor Window

### Layout System

The editor uses a flexible docking system based on ImGui:

- **Drag panels** by their title bar to rearrange
- **Dock panels** by dropping on docking zones
- **Resize panels** by dragging borders
- **Tab panels** by docking them together
- **Float panels** by dragging outside the main window

### Default Layouts

Access predefined layouts via **View > Layouts**:

| Layout | Description |
|--------|-------------|
| Default | Balanced view with all main panels |
| Scene Edit | Focused on scene composition |
| Story Graph | Maximized story graph editor |
| Timeline | Timeline-focused for animation work |

### Saving/Loading Layouts

- **View > Save Layout** - Save current arrangement
- **View > Load Layout** - Restore saved arrangement
- **View > Reset Layout** - Return to default

---

## Panel Reference

### Scene View

**Purpose:** Visual WYSIWYG preview of the current scene.

**Features:**
- Object selection and transformation
- Zoom and pan navigation
- Grid and snapping options
- Multiple view modes (Scene, Game, Debug)

**Controls:**
| Action | Input |
|--------|-------|
| Pan | Middle-click drag or Space+drag |
| Zoom | Scroll wheel |
| Select | Left-click |
| Multi-select | Ctrl+click or drag box |
| Transform | Use gizmo handles |

---

### Story Graph Editor

**Purpose:** Node-based visual scripting for story flow.

**Features:**
- Create and connect story nodes
- Visualize branching paths
- Navigate with minimap
- Real-time validation

**Node Types:**
- **Dialogue** - Character dialogue
- **Choice** - Player choices
- **Branch** - Conditional branching
- **Event** - Script events
- **Scene** - Scene transitions

**Controls:**
| Action | Input |
|--------|-------|
| Create Node | Right-click > Add Node |
| Connect | Drag from output to input |
| Delete | Select + Delete key |
| Navigate | Middle-click drag |
| Zoom | Scroll wheel |

---

### Timeline Editor

**Purpose:** Keyframe animation editing.

**Features:**
- Multi-track timeline
- Keyframe manipulation
- Curve editor integration
- Playback preview

**Track Types:**
- Transform tracks
- Property tracks
- Audio tracks
- Event tracks

---

### Inspector Panel

**Purpose:** Property editing for selected objects.

**Features:**
- Dynamic property display
- Type-specific editors
- Undo/redo support
- Component management

---

### Asset Browser

**Purpose:** Navigate and manage project assets.

**Features:**
- Grid and list views
- Thumbnail preview
- Drag-drop to scene
- Asset import/export
- Search and filtering

**Context Menu:**
- Rename (F2)
- Delete
- Duplicate
- Show in Explorer
- Reimport

---

### Hierarchy Panel

**Purpose:** Scene object tree view.

**Features:**
- Parent-child relationships
- Visibility toggles
- Drag-drop reparenting
- Multi-selection
- Search filtering

---

### Console Panel

**Purpose:** Log output and debugging.

**Features:**
- Severity filtering (Info/Warning/Error)
- Search in messages
- Timestamp display
- Collapsible repeated messages
- Clear button

---

### Voice Manager Panel

**Purpose:** Voice-over management for dialogue.

**Features:**
- Dialogue line table
- Status indicators (Bound/Missing/AutoMapped)
- Voice preview playback
- Auto-mapping configuration
- Export/import voice tables

**Status Colors:**
| Status | Color | Meaning |
|--------|-------|---------|
| Bound | Green | Voice file linked |
| Unbound | Gray | No voice file |
| Missing | Red | File not found |
| AutoMapped | Blue | Automatically linked |
| Pending | Yellow | Awaiting import |

---

### Localization Panel

**Purpose:** Multi-language string management.

**Features:**
- String table editor
- Side-by-side translation
- Missing string detection
- Import/export (JSON/CSV/PO)
- Coverage statistics

**Workflow:**
1. Set source language (default: English)
2. Set target language
3. Edit translations in-place
4. Export for external translation
5. Import completed translations

---

### Curve Editor Panel

**Purpose:** Animation easing curve editing.

**Features:**
- Bezier curve editing
- Preset library
- Custom curve creation
- Preview animation
- Timeline integration

**Presets:**
- Linear
- Ease In/Out variants
- Bounce, Elastic, Back

---

### Build Settings Panel

**Purpose:** Configure project builds.

**Features:**
- Platform selection
- Build configuration
- Asset compression
- Size estimation
- Build execution

---

### Diagnostics Panel

**Purpose:** Display errors and warnings.

**Features:**
- Categorized by severity
- Click to navigate to source
- Search and filtering
- Auto-refresh on changes

---

### Debug Overlay Panel

**Purpose:** Runtime debugging during play mode.

**Features:**
- Performance metrics
- Call stack view
- Variable watch
- Animation state
- Audio channel monitor

---

## Keyboard Shortcuts

### File Operations
| Shortcut | Action |
|----------|--------|
| Ctrl+N | New Project |
| Ctrl+O | Open Project |
| Ctrl+S | Save |
| Ctrl+Shift+S | Save As |
| Ctrl+W | Close |

### Edit Operations
| Shortcut | Action |
|----------|--------|
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| Ctrl+X | Cut |
| Ctrl+C | Copy |
| Ctrl+V | Paste |
| Delete | Delete |
| Ctrl+D | Duplicate |
| Ctrl+A | Select All |
| F2 | Rename |

### View Operations
| Shortcut | Action |
|----------|--------|
| Ctrl++ | Zoom In |
| Ctrl+- | Zoom Out |
| Ctrl+0 | Reset Zoom |
| F | Fit to View |

### Playback
| Shortcut | Action |
|----------|--------|
| F5 | Play/Pause |
| Shift+F5 | Stop |
| F10 | Step Forward |

### Debug
| Shortcut | Action |
|----------|--------|
| F9 | Toggle Breakpoint |
| F11 | Step Into |
| Shift+F11 | Step Out |

### Panel Navigation
| Shortcut | Action |
|----------|--------|
| Ctrl+P | Quick Panel Switch |
| Ctrl+` | Console |
| Ctrl+I | Inspector |

---

## Style Guide

### Design Principles

1. **Consistency** - All panels follow the same visual rules
2. **Clarity** - Clear visual hierarchy and feedback
3. **Efficiency** - Common actions are easily accessible

### Color Usage

**Semantic Colors:**
- **Blue** (#007ACC) - Primary accent, selection
- **Green** (#4CAF50) - Success, positive status
- **Yellow** (#FFC107) - Warning, attention needed
- **Red** (#F44336) - Error, destructive action

**Status Indicators:**
- Use color + icon for accessibility
- Maintain consistent meaning across panels

### Spacing System

Based on 4px unit:
- **4px** - Tight spacing (within controls)
- **8px** - Standard spacing (between items)
- **16px** - Section spacing
- **24px** - Group separation

### Typography

- **Body text** - 14px
- **Small text** - 12px
- **Headers** - 16-18px
- **Icons** - 16px standard, 24px large

---

## Customization

### Themes

The editor supports multiple themes:
- **Dark** (default)
- **Light**
- **High Contrast**

Change via **Edit > Preferences > Theme**

### Custom Shortcuts

Customize keyboard shortcuts:
1. Open **Edit > Preferences > Keyboard**
2. Find the command
3. Click the binding field
4. Press new key combination
5. Resolve any conflicts

### Panel Settings

Each panel has its own settings accessible via the panel menu (≡):
- View options
- Column visibility
- Sort preferences
- Filter defaults

---

## Troubleshooting

### Common Issues

**Panel not visible:**
- Check **View > Panels** menu
- Reset layout via **View > Reset Layout**

**Slow performance:**
- Reduce thumbnail size in Asset Browser
- Disable unused panels
- Check Debug Overlay for metrics

**File not importing:**
- Check console for error messages
- Verify file format is supported
- Check file permissions

### Getting Help

- Documentation: [docs/gui/](.)
- GitHub Issues: [Report bugs](https://github.com/VisageDvachevsky/NM-/issues)
- Community: [Discussions](https://github.com/VisageDvachevsky/NM-/discussions)

---

## Version History

### v0.2.0 (Current)
- Complete GUI system implementation
- All editor panels
- Theme and style system
- Keyboard shortcuts
- Voice Manager Panel
- Localization Panel
- Curve Editor Panel
- Debug Overlay
