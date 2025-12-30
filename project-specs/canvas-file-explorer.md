# Canvas File Explorer

Date: 2025-12-29
Status: Draft
Related: canvas-file-explorer-brainstorm.md

## Summary

A new widget combining spatial canvas organization (like Miro/desktop icons) with filesystem browsing. Files from the host OS can be mounted, displayed as draggable icons, annotated with metadata, and organized into virtual containment hierarchies independent of the actual filesystem structure.

**Vision:** Cross between arx_caeli/Miro, Obsidian, and Finder — eventually a daily-driver Finder replacement.

---

## Core Concepts

### Canvas
- Finite area (half-screen or full-screen widget)
- No scrolling — pagination for overflow (like mobile home screens)
- Items placed freely (x,y coordinates), not snapped to grid
- Object permanence: items stay where placed until explicitly moved
- Each canvas is an entity with its own metadata (status, dates, tags)

### Items on Canvas
- **File icons**: References to host filesystem files
- **Folder links**: Navigate to other canvases (hyperlink-style, not tree hierarchy)
- **Notes**: Text, rich content, typed notes (todo, reference, idea)
- **Mini-widgets**: Calendar, clock, entity views (future)

### Mounts
- Mount a host filesystem folder to make its contents available
- Live view: queries filesystem on demand
- Files get entity representation for annotations
- Same file can be mounted/aliased multiple times in different contexts

### Virtual Containment
- Any file can "contain" other files (entity relationships, not filesystem changes)
- Enables organization beyond filesystem limitations
- Files remain at original filesystem location
- Parallel organizational structure overlaid on real filesystem

---

## Architecture

### Canvas Entity Structure

```
Entitas: "Canvas::/work/projects/rhubarb"
├─ genus: "Canvas"
├─ proprietates:
│   ├─ titulus: "Rhubarb Project"
│   ├─ address: "/work/projects/rhubarb"
│   ├─ pagina_currens: "0"
│   ├─ paginae_numerus: "3"
│   ├─ status: "active"
│   └─ tags: "project,code"
├─ relationes:
│   ├─ "continet" → [CanvasItem entities]
│   └─ "mount" → [Mount entities]
└─ notae: ["#canvas", "#project"]
```

### Canvas Item Entity Structure

```
Entitas: "CanvasItem::abc123"
├─ genus: "CanvasItem"
├─ proprietates:
│   ├─ x: "150"
│   ├─ y: "200"
│   ├─ pagina: "0"
│   ├─ item_genus: "file" | "note" | "folder_link" | "widget"
│   └─ (item-type-specific properties)
├─ relationes:
│   ├─ "in_canvas" → "Canvas::/work/projects/rhubarb"
│   └─ "references_file" → "FileRef::def456" (if file item)
└─ notae: []
```

### File Reference Entity Structure

```
Entitas: "FileRef::def456"
├─ genus: "FileRef"
├─ proprietates:
│   ├─ host_path: "/Users/fran/Documents/spec.md"
│   ├─ filename: "spec.md"
│   ├─ extension: "md"
│   ├─ size_bytes: "4096"
│   ├─ modified_time: "1703836800000"
│   ├─ mirrored: "false"
│   └─ computer_specific: "false"
├─ relationes:
│   ├─ "from_mount" → "Mount::xyz789"
│   └─ "virtual_contains" → [...] (if file acts as folder)
└─ notae: []
```

### Layered Annotations

```
/* Base file annotations (shared across all aliases) */
Entitas: "FileRef::def456"
├─ proprietates:
│   └─ annotation_global: "Important document"

/* Context-specific annotations (per canvas item) */
Entitas: "CanvasItem::abc123"
├─ proprietates:
│   └─ annotation_local: "Needs review for Q1"
```

---

## Widget Structure

### New Widget: TabulaExplorator (Canvas Explorer)

```c
nomen structura {
    ContextusWidget* ctx;
    Entitas* canvas_entitas;           /* Current canvas entity */

    /* Pagination */
    i32 pagina_currens;
    i32 paginae_numerus;

    /* Items on current page */
    Xar* items;                         /* Array of TabulaItem* */

    /* Selection state */
    TabulaItem* selectio;
    b32 trahere_activum;                /* Dragging? */
    i32 trahere_offset_x, trahere_offset_y;

    /* Mounted folders */
    Xar* mounts;                        /* Array of Mount* */

    /* Layout */
    i32 latitudo, altitudo;
} TabulaExplorator;

nomen structura TabulaItem {
    Entitas* entitas;
    i32 x, y;
    i32 latitudo, altitudo;             /* Icon size */
    TabulaItemGenus genus;              /* file, note, folder_link, widget */

    /* Type-specific data */
    unio {
        TabulaItemFile file;
        TabulaItemNote note;
        TabulaItemLink link;
    } datum;
} TabulaItem;

nomen structura TabulaItemFile {
    chorda* host_path;
    chorda* filename;
    chorda* extension;
    b32 est_directorium;
    FileIconGenus icon_genus;           /* Determined by extension */
} TabulaItemFile;

nomen structura Mount {
    Entitas* entitas;
    chorda* host_path;
    b32 activum;
    /* FSEvents watch handle (future) */
} Mount;
```

---

## Canvas Addressing

URL-style paths with namespacing:

```
/                           # Root canvas
/work                       # Work canvas
/work/projects              # Projects canvas
/work/projects/rhubarb      # Rhubarb project canvas
/personal/photos/2024       # Personal photos canvas
```

- Addresses are entity IDs (interned)
- Flat graph: any canvas can link to any other
- Address hierarchy is for human organization, not enforced structure
- Canvases with same prefix are NOT required to have parent/child relationships

---

## Interaction Model

### Mouse-First

| Action | Behavior |
|--------|----------|
| Click item | Select |
| Click canvas | Deselect |
| Drag item | Move within page |
| Drag to drop zone | Move to prev/next page |
| Double-click file | Open launcher panel |
| Double-click folder link | Navigate to linked canvas |
| Right-click | Context menu (future) |

### Acme-Style Commands

- Any text area can contain executable commands
- Create a text note, type command, click to execute
- Commands registered in global RegistrumCommandi
- Context-sensitive: some commands only available when relevant widgets visible

### Keyboard

- Minimal keyboard support (mouse-first mode)
- Escape: Deselect
- Delete/Backspace: Delete selected item
- Cmd+C/V: Copy/paste items (future)

---

## Pagination

### Page Model

```
┌─────────────────────────────────────┐
│  Canvas content area                │
│  (items placed at x,y coordinates)  │
│                                     │
│                                     │
├─────────────────────────────────────┤
│ [<] Page 1 of 3 [>]  [+]           │ ← Page controls
│ [Drop: Prev]        [Drop: Next]   │ ← Drop zones
└─────────────────────────────────────┘
```

- Fixed number of items per page (based on canvas size and icon size)
- Page controls at bottom
- Explicit drop zones for cross-page moves
- New pages created automatically when needed

### Item Distribution

- Items have (x, y, pagina) coordinates
- Items on non-current pages not rendered
- Dragging item to drop zone changes its `pagina` property

---

## File Operations

### Phase 1 (First Milestone)

| Operation | Implemented |
|-----------|-------------|
| Browse mounted folder | Yes |
| Display files as icons | Yes |
| Navigate into subfolders | Yes |
| Open file (launcher panel) | Yes |
| Launch in system app | Yes |
| Select/deselect | Yes |
| Drag to reposition | Yes |

### Phase 2 (Future)

| Operation | Notes |
|-----------|-------|
| Create folder | Creates real filesystem folder |
| Rename | Renames real file |
| Delete | Moves to Trash |
| Copy/Move | Real filesystem operations |
| Create note | Entity-only, not filesystem |
| Annotate file | Entity metadata |

### Phase 3 (Future)

| Operation | Notes |
|-----------|-------|
| Mirror to database | Copy file content into entity graph |
| Event-source file history | Track changes via hashes/Merkle trees |
| Virtual containment | Files "inside" other files |
| Cross-computer sync | Database contains all data |

---

## External Change Handling

### Inbox Model

External filesystem changes don't auto-apply. Instead:

1. FSEvents or polling detects change
2. Change queued to user's Inbox widget
3. User reviews: "File X was modified/deleted/added"
4. User chooses action: Accept, Ignore, Investigate

**Benefits:**
- User knows what's changing
- No surprise mutations to large data graphs
- Audit trail of external changes

### Inbox Entity

```
Entitas: "InboxTask::change_123"
├─ genus: "InboxTask"
├─ proprietates:
│   ├─ task_type: "file_modified"
│   ├─ host_path: "/Users/fran/doc.md"
│   ├─ detected_time: "1703836800000"
│   ├─ status: "pending" | "accepted" | "ignored"
│   └─ details: "Size changed from 4096 to 8192"
└─ relationes:
    └─ "affects_file" → "FileRef::def456"
```

---

## Icon Rendering

### Type-Based Icons

| Extension | Icon |
|-----------|------|
| .md, .txt | Document |
| .c, .h | Code |
| .png, .jpg | Image |
| .pdf | PDF |
| .mp3, .wav | Audio |
| directory | Folder |
| unknown | Generic file |

### Icon Size

- Standard: 64x64 pixels
- Configurable per-canvas (future)
- Label below icon with filename (truncated if needed)

---

## Integration Points

### With RegistrumCommandi

Canvas registers commands:
- `canvas:navigate [address]` - Go to canvas
- `canvas:mount [path]` - Mount folder
- `canvas:unmount [mount_id]` - Unmount
- `canvas:new_note` - Create text note
- `canvas:new_folder_link [address]` - Create link to canvas

### With Schirmata

- TabulaExplorator is a widget like BibliaVisus, NavigatorEntitatum
- Can appear in any screen's widget layout
- Per-screen widget instance (follows unified-state-paradigm)

### With Entity Repository

- Canvases, items, mounts, file refs are all entities
- Auto-persist on mutation (follows unified-state-paradigm)
- Event-sourced history

### With Launcher Panel

When file double-clicked:
1. Launcher panel opens in adjacent widget slot
2. Shows file info (name, size, modified date)
3. Action buttons:
   - Open in [internal widget] (if supported type)
   - Open in macOS app
   - Reveal in Finder
   - Copy path
   - View annotations
   - Add to canvas (if viewing from folder browse)

---

## Data Model Summary

```
Canvas (entity)
├── has many: CanvasItem (via "continet" relationship)
├── has many: Mount (via "mount" relationship)
└── properties: address, paginae_numerus, status, tags

CanvasItem (entity)
├── belongs to: Canvas
├── references: FileRef | Note | Canvas (folder_link)
└── properties: x, y, pagina, item_genus, local_annotations

FileRef (entity)
├── belongs to: Mount
├── has many: CanvasItem (can appear on multiple canvases)
├── has many: FileRef (virtual containment)
└── properties: host_path, mirrored, global_annotations

Mount (entity)
├── belongs to: Canvas
├── has many: FileRef
└── properties: host_path, activum

Note (entity)
├── appears on: CanvasItem
└── properties: content, note_type

InboxTask (entity)
└── properties: task_type, status, detected_time
```

---

## First Milestone: Mount + Icons

### Scope

1. New widget: TabulaExplorator
2. Mount a single folder from host filesystem
3. Display folder contents as icons on canvas
4. Click to select, drag to reposition
5. Double-click folder to navigate into it
6. Double-click file to show launcher panel
7. Launch file in macOS system app
8. Single page (no pagination yet)
9. Persist canvas state (item positions) via entity repository

### Files to Create

| File | Purpose |
|------|---------|
| `include/tabula_explorator.h` | Widget header |
| `lib/tabula_explorator.c` | Widget implementation |
| `include/mount.h` | Mount management |
| `lib/mount.c` | Filesystem queries |
| `include/file_icon.h` | Icon type determination |
| `lib/file_icon.c` | Icon rendering |

### Files to Modify

| File | Change |
|------|--------|
| `include/registrum_widget.h` | Register TabulaExplorator factory |
| `lib/registrum_widget.c` | Factory implementation |
| `include/schirmata.h` | Add canvas mode (or use existing widget slot) |
| `lib/schirmata.c` | Mode switching to canvas |

---

## Future Phases

### Phase 2: Full Canvas Features
- Pagination with drop zones
- Text notes
- Folder links (canvas-to-canvas navigation)
- Canvas metadata (status, tags)
- Multiple mounts per canvas

### Phase 3: Annotations & Virtual Structure
- Layered annotations (global + local)
- Virtual containment (file contains files)
- Annotation viewer (see all contexts for a file)

### Phase 4: File Mirroring
- Copy file contents into database
- Event-source file changes
- Merkle tree for change detection
- Computer-specific file marking

### Phase 5: Full Finder Replacement
- CRUD filesystem operations
- FSEvents watching
- Inbox for external changes
- Cross-computer data portability

---

## Open Questions

1. **Icon rendering library** - Use existing glyph system or new icon assets?
2. **Launcher panel design** - New widget or overlay on canvas?
3. **Mount persistence** - Store in canvas entity or separate mount registry?
4. **Drag visual feedback** - Ghost image while dragging?
5. **Multi-select** - Shift-click, rubber-band selection?

---

## Success Criteria (First Milestone)

1. Widget renders and responds to input
2. Can mount a folder and see its contents as icons
3. Icons show filename and type-based icon
4. Can drag icons to new positions
5. Positions persist across app restart
6. Can navigate into subfolders
7. Can double-click file to launch in system app
8. Works as half-screen widget alongside other widgets
