# Pinacotheca - Image Gallery & Dithering Pipeline

## Overview

Pinacotheca ("picture gallery" in Latin, from Greek) is the image management system for Lapideus. It provides:

1. **Image Import Pipeline**: Convert external images (PNG, JPEG, etc.) to the 16-color Aquinas palette using Atkinson dithering
2. **Image Gallery Widget**: Browse, view, and manage the collection of imported images
3. **Facing Panel Integration**: Display images alongside text content via the `$image` command

This is the first milestone of the Vultus visual identity system.

## Core Workflow

### Import Flow

1. User invokes `$import-image /path/to/image.png` or pastes from clipboard (Cmd+V)
2. Modal dialog appears on left panel with import controls
3. Right panel shows live-updating dithered preview
4. User adjusts palette selection, breakpoints, zoom, and pan
5. User names the image (timestamp default) and clicks "Salvare"
6. Image saved to entity system, Pinacotheca opens to the new image
7. Original left panel content restored

### Viewing Flow

1. User invokes `$image <name>` from text editor
2. Pinacotheca widget opens in facing panel, deep-linked to named image
3. Image displayed centered in the panel

### Browse Flow

1. User opens Pinacotheca widget directly
2. Ranger-style UI: left half shows alphabetical list, right half shows preview
3. j/k navigation, Enter to view full, Escape to return to browser

## Import Dialog

### Layout

Modal overlay on left panel (~80% width/height), right panel shows preview.

**Left Panel Controls:**
- Name field (prefilled with timestamp like `imago_20260102_1430`)
- Preset buttons: All, Grays, Warm, Cool, Mono
- 2x8 palette grid (click to toggle colors)
- 3 breakpoint sliders (gray thresholds 0-255)
- Zoom slider (affects both preview and output size)
- Pan control (drag in preview to offset crop)
- Salvare / Abicere buttons

**Right Panel:**
- Live dithered preview
- Fixed crop frame at widget aspect ratio
- Image can be panned via drag
- Zoom affects how much of source fits in frame

### Behavior

- All controls update preview immediately (no debounce)
- Color dithering mode only (no grayscale bucketing toggle)
- Zoom re-centers image automatically
- Default output: ~448px height, widget aspect ratio
- Can resize smaller than default via zoom

### Dithering Algorithm

Replicate the Atkinson dithering from `aquinas-dithering-tool.html`:

- Color space: RGB with Euclidean distance for nearest color
- Error diffusion: Atkinson pattern (1/8 to 6 neighbors)
- Palette: User-selected subset of 16 Aquinas colors
- Breakpoint sliders affect luminance bucketing (kept for compatibility)

## Pinacotheca Widget

### Modes

**Browser Mode** (default):
- Split view: list on left half, preview on right half
- j/k to navigate list
- Enter to switch to View Mode
- Can rename selected image
- Can delete with confirmation

**View Mode**:
- Full panel shows single image, centered
- Escape returns to Browser Mode
- This is also what `$image <name>` displays

### Image List

- Sorted alphabetically by name
- Shows image name in list
- Selected item highlighted
- Preview updates as selection changes

## Entity Model

### Genus

```
Genus: Vultus::Imago
  - titulus: chorda (the image name, unique identifier)
  - latitudo: i32 (width in pixels)
  - altitudo: i32 (height in pixels)
  - data: binary (palette indices, compressed with flatura.h)
  - creatum: timestamp
```

### Storage

- Image data stored as 1 byte per pixel (palette index 0-15)
- Compressed transparently using flatura.h on store/load
- Binary blob support needed in entity property system

## Dialog System

### Reusable Affordance

Create a generic dialog module that can be reused:

**Primitives:**
- Modal overlay positioning (centered, percentage of parent)
- Optional background dimming (pattern overlay)
- Focus capture for owning panel
- Dismiss callback mechanism

**Each dialog provides:**
- Its own content rendering
- Its own input handling
- Its own dismiss conditions (Salvare, Abicere, escape)

## Platform Integration

### Image Loading

Wrap stb_image.h in platform API:
- `imago_caricare_ex_file(path)` → raw RGBA pixels
- `imago_caricare_ex_memoria(data, size)` → raw RGBA pixels
- May replace with custom implementation later

### Clipboard

New platform library for clipboard access:
- macOS: NSPasteboard API
- `clipboard_habet_imaginem()` → bool
- `clipboard_capere_imaginem()` → raw RGBA pixels
- Future: text clipboard support

### Cmd+V Detection

- Cmd+V triggers clipboard check
- If image data present, initiate import flow
- Otherwise, handle as text paste (future)

## UI Primitives Needed

### Slider Widget

New UI element:
- Mouse-draggable only (no keyboard)
- Horizontal track with handle
- Value range and current value
- Callback on change

### Clickable Color Grid

- 2x8 grid of palette colors
- Toggle state per color
- Checkmark indicator on selected
- Click to toggle, immediate callback

### Modal Overlay

- Positioned within parent panel
- Captures input for that panel
- Background dimming optional

## Commands

```
$import-image <path>    # Open import dialog for file
$image <name>           # Display named image in facing panel
$pinacotheca            # Open gallery browser
```

## Output Dimensions

- Default height: ~448px (widget drawing area minus schirmata bar)
- Aspect ratio: matches widget (720/2 = 360 width for half-panel)
- Zoom slider allows smaller output
- Zoom re-centers automatically

## First Implementation Steps

1. Platform clipboard library (macos)
2. Platform image loading (stb_image.h wrapper)
3. Dithering algorithm in C
4. Entity binary blob support
5. Slider widget
6. Modal dialog primitives
7. Import dialog
8. Pinacotheca widget (browser + view modes)
9. $image command integration

## Open Questions

### Binary Blob in Entities

Need to verify current entity system supports binary properties. If not, extend `proprietas_ponere` to handle binary data with length.

### Flatura Integration

How to make compression transparent:
- Compress on `proprietas_ponere` for binary type?
- Decompress on `proprietas_capere` for binary type?
- Or explicit at image save/load points?

## Related Specs

- vultus-spec.md - Parent design system
- thema.c - Aquinas palette definition
- reference/aquinas-dithering-tool.html - Reference implementation
- reference/dithering-example.js - Dithering algorithm reference
- flatura.h - Compression library
