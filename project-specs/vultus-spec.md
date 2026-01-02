# Vultus - Visual Identity & Design System

## Overview

Vultus ("face/appearance" in Latin) defines the visual identity and design constraints for the entire Lapideus operating system. It establishes hard constraints that ensure aesthetic coherence across all widgets, spheres, and content types, while providing tools for creating visual content within those constraints.

The philosophy is constraint-as-liberation: by enforcing strict limits (16 colors, fixed resolution, monospace fonts, no scrolling), the system achieves a distinctive, timeless aesthetic reminiscent of classic Macintosh, early console games, and Return of the Obra Dinn.

## Core Constraints

### Display Resolution
- **Canvas size**: 720x480 pixels (640x480 with modern aspect ratio)
- **Scaling**: Stretch to fit available window/screen
- **No scrolling**: All content must fit within pagination; overflow navigates to adjacent pages

### Color Palette
- **Exactly 16 colors** defined in `thema.c`
- **Universal constraint**: All widgets, images, and rendered content use only these 16 colors
- **Exception**: Low-resolution political campaign video streaming (separate spec)

### Typography
- **Monospace only** for screen display
- **Base font**: 6x8 pixel monospace
- **Sizes**: 1x (6x8) and 2x (12x16) standard; 3x/4x available for special contexts
- **Print output**: May use proportional fonts at higher resolution

## Visual Tools

### Paint App (HyperCard-style)

A hybrid bitmap/vector drawing application with interactive capabilities:

**Drawing Features:**
- Bitmap tools: pencil, fill, shapes (MacPaint/KidPix style)
- Vector primitives: editable lines, rectangles, ellipses
- Pure visual layers for compositing
- Distressed/masked borders (hand-drawn masks or procedural)

**HyperCard Capabilities:**
- Behavioral elements: drawn shapes can trigger actions
- Full system access: navigation, commands, widget spawning
- Animation support (Director/SuperCard-style)
- Scripting via: Sputnik, Acme commands, GUI properties, or blueprint/node graphs

**Screen Integration:**
- Any schirmata screen can have background/foreground doodle layers
- In-place edit mode: toggle to draw directly on current screen
- Screen-persistent: annotations saved with schirmata state
- Can also create standalone "drawings" as image assets

### Pixel Editor (Aseprite-style)

Separate specialized tool for game asset creation:

- Sprite editing limited to 16-color palette
- Animation timeline support
- Tile editing for SNES-style games
- Texture creation for 3D models
- Export for custom hardware target (gameboy form factor, SNES-level capability)

### 3D Diorama System

Low-poly 3D rendering for static illustrations:

**Rendering:**
- Software rasterizer (C, built from scratch)
- Runtime 3D (not pre-baked)
- Cell shading with dithered gradients
- Two-color mode: pick "black" and "white" from palette

**Render Modes:**
- Clean linear "line art" style with dithered shading
- PS1-style with textures (affine warping, vertex jitter optional)
- Distressed/masked borders on render output

**Model Library:**
- Bundled curated library
- Import supported (OBJ, glTF) with conversion to internal format
- Hard poly limit: PS1 era (~200-500 polygons per model)
- Textures created in pixel editor, pre-dithered to palette

**Authoring:**
- Position, rotate, zoom models to compose dioramas
- Output used as static illustrations (not interactive 3D)
- Accessible alternative to hand-drawing perspective

### Image Dithering Pipeline

Convert external images to palette:

**Input Sources:**
- File import (PNG, JPEG, etc.)
- Clipboard paste
- Camera capture (potentially)

**Dithering Algorithms:**
- Default: Floyd-Steinberg / Atkinson
- Context-based: semantic defaults per sphere/widget type
- Ordered/Bayer dithering available for specific effects

**Output:**
- 16-color dithered bitmap
- Stored as drawing/image asset for embedding

## Content Integration

### Embedding in Content

Visual assets can be used throughout Lapideus:

- **Inline workflow**: Create illustrations directly within content authoring
- **Asset workflow**: Create in dedicated tools, reference/embed elsewhere
- **Scientia articles**: Embed dioramas, diagrams, dithered images
- **Mercatura listings**: Product illustrations
- **Any widget**: Background/foreground layers available

### Print Output

System is print-aware with different visual language:

- Dynamically generated for print form factors
- Possibly black and white only
- Manuscript / classic typography aesthetic
- Higher resolution fonts (monospace or proportional)
- Export formats: PDF (via future Scriptoria integration)

## Standard UI Widgets (Outline)

Future specification will define visual vocabulary for:

- Buttons
- Text fields
- Lists/tables
- Toggles/checkboxes
- Focus indicators
- Selection states
- Disabled states

Within constraints: 16 colors, monospace text, no scrolling.

## Animation & Sound

### Animation
- Sprite animation in pixel editor
- Screen transitions (to be defined)
- Feedback animations
- HyperCard-style interactive animations

### Sound (Separate System)
- Chiptune / retro aesthetic
- Sound synthesizer tools (separate spec)
- Sound effects for UI feedback

## Relationship to Other Systems

### Separate from Memory Palace
The 3D diorama system is distinct from the Zelda-style memory palace navigation mode. Memory palace uses 2D tile-based graphics, not the 3D renderer.

### Game Development
Pixel editor supports development for:
- Custom hardware (long-term vision): gameboy form factor, SNES-level capability
- Lapideus-internal games
- Memory palace / spatial navigation UI modes

## Aesthetic References

- **Dithering**: Return of the Obra Dinn
- **UI**: Classic Macintosh (MacPaint, HyperCard)
- **3D**: PS1 era low-poly
- **Overall**: What emerges from the constraint of 16 colors at 720x480

## First Milestone

**Dithering Pipeline**: Import images and automatically convert to 16-color palette.

Features:
- File import (PNG/JPEG)
- Clipboard paste support
- Floyd-Steinberg / Atkinson dithering
- Save as internal image asset
- Display in a basic viewer

## Open Questions / Risks

### Aesthetic Coherence
Primary concern is making the constrained aesthetic look good across diverse content types:
- Dithered photographs
- JRPG tiles and sprites
- 3D rendered dioramas
- UI widgets
- Print output

Requires iteration with real content to validate.

### Software Rasterizer Performance
Building 3D renderer from scratch without GPU. May need optimization work to maintain usable frame rates for diorama preview/authoring.

### PS1 Mode with Palette
Unclear how PS1-style textured rendering translates to 16-color palette. May be image-level dithering effect, or textures pre-converted to palette.

## Related Specs

- thema.c - 16-color palette definition
- dithering-example.js - Reference dithering algorithm
- Scientia spec - Content embedding use case
- Mercatura spec - Product illustration use case
- Systema Paradigmata spec - HyperCard behaviors integrate with command system
