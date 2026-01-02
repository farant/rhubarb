# Vultus (Visual Identity & Design System) - Raw Interview Brainstorm

Date: 2026-01-02

## Q&A Session

**Q: What's the feature you want to build? Give me the elevator pitch — what does it do and why does it matter to you?**
A: I want to talk about the visual identity and theming stuff / limitations and some integrations with low poly 3d models for static illustrations, etc.

**Q: When you say 'visual identity' — are you defining this primarily at the rhubarb library level (C code drawing primitives), the Lapideus app level (shared across all widgets/spheres), or both with the library providing constraints the app builds on?**
A: Kind of both. In theory there are low level things in rhubarb that could be used outside of these limitations but in practice nothing in the rhubarb library should really be outside of these limitations (at least for displaying on screen). So you have the 720x480 or whatever it is pixel resolution which is resolution of 640x480 but with modern aspect ratio. You have the limited color palette. It is a 16 color palette and that is it. It is not 16 color palettes where you choose the colors, there are 16 colors and that is all anything can use (I guess except in theory the low resolution political campaigning video). You can see the colors in thema.c.

**Q: For the 16-color palette constraint you've mentioned — is this already fully defined and you're codifying it, or is palette design part of what you want to explore?**
A: The 16 colors are already defined and being used consistently so far. Another visual constraint is that I think I am only going to do monospace fonts. At the moment I have a 6x8 font that is in a couple places being drawn at 2x size.

**Q: For the 6x8 monospace font at 1x and 2x sizes — do you envision needing additional sizes (4x for headers, 0.5x for dense data), or is the constraint intentionally 'two sizes only' to force consistent hierarchy?**
A: Two sizes is probably just about enough, it's already very big but it's possible in certain contexts you would want 3x or 4x.

**Q: For the low-poly 3D models you mentioned — what role do they play? Static illustrations (like woodcuts in a medieval book), interactive elements (rotate/inspect), or something more like game sprites that animate?**
A: So the idea is that you have a library of 3D low poly models and you can arrange them / rotate / zoom / position, and then they can act like a diorama or decorative kind of effect. So not as interactive objects but as static illustrations since drawing in perspective is hard but having the 3D actually makes it pretty accessible. Might have something like only black and white cell shading plus some kind of dithered shading (where what "black" and "white" are are colors you choose from 16 color palette).

**Q: For the 3D diorama authoring — is this a dedicated 'illustration editor' widget where you compose scenes, or is it inline within content (like embedding a scene in a Scientia article), or both with the editor producing assets that get placed elsewhere?**
A: So there will be a paint app, kind of like MacPaint/Kid Pix/Microsoft Paint, and it will also be like HyperCard. I think since you don't have any scrolling there is a lot more potential flexibility for "doodling" potentially across any widget/static screen as kind of an overlay and underlay layer.

**Q: For the low-poly model library — where do models come from? Bundled with app (curated library), user-created (import tools), purchasable via Mercatura, or federated from internets like other content?**
A: Probably a bundled library, possibly with imported models but there would be a hard limit to poly count. Want to keep kind of lowfi PS1 / Obra Dinn aesthetic and enforce that.

**Q: For the paint app with overlay/underlay doodling — when you doodle on top of a widget, is that annotation ephemeral (disappears on refresh), persistent per-screen (part of your schirmata state), or attached to the underlying content (like glosses)?**
A: It would be screen persistent. And then there would probably be "drawings" that are like image files in the system (there would also be dithered images you import probably) that you can use / place in certain contexts as illustrations. You can see an example of dithering code in dithering-example.js written in JavaScript if you want.

**Q: For the MacPaint/KidPix aesthetic — are you thinking bitmap tools only (pencil, fill, shapes), or do you also want vector primitives (lines, rectangles, ellipses that stay editable)?**
A: Probably hybrid. There would also probably be layers. But kind of similar to HyperCard.

**Q: For HyperCard-style interactivity in the paint app — can drawn elements have behaviors attached (click this shape to navigate, hover to show tooltip), or is HyperCard more about the 'card stack' organizational metaphor?**
A: Behavioral elements.

**Q: For imported dithered images — does the import process automatically reduce to the 16-color palette using dithering, or do users need to pre-process images externally to match the palette?**
A: Auto-dither on import.

**Q: For behavioral elements (scriptable drawn shapes) — is the scripting done via Sputnik (your JS-like language), via Acme-style command text attached to the element, or through a GUI properties panel?**
A: I think all three, plus maybe some kind of blueprint-ish/n8n graph editing.

**Q: For the 3D cell-shaded diorama rendering — is this rendered at runtime (real-time 3D), pre-rendered when you finalize the scene (baked to bitmap), or both options depending on use case?**
A: Runtime 3D.

**Q: For runtime 3D rendering — what's the rendering approach? Software rasterizer in C (maximum portability, full control), OpenGL/Metal (GPU acceleration), or WebGL for the PWA side with native equivalent on desktop?**
A: Software rasterizer.

**Q: For the Obra Dinn / PS1 aesthetic with cell shading — do you want the characteristic 'affine texture warping' and vertex jitter of PS1, or just the low-poly count and limited palette without the distortion artifacts?**
A: There might be different modes. I could imagine a clean linear "line art style" with some dithered shading and then possibly cases where you would try to do PS1 style with textures and everything (although I'm not sure how that would translate to our 16 color palette).

**Q: For the software rasterizer — is this something you want to build from scratch as a rhubarb library, or would you consider porting/adapting an existing minimal software renderer (like a cleaned-up version of Quake's, or something purpose-built for retro aesthetics)?**
A: Build from scratch.

**Q: For textures on 3D models within the 16-color constraint — are textures themselves limited to the palette (pre-dithered texture atlas), or does the renderer dither at render-time from higher-color source textures?**
A: I think there will be a built in pixel editor / animator tool like Aseprite but obviously limited to the palette so that could be used for textures. I don't know if the PS1 version would be dithered since it's limited to that palette already. But I could imagine it almost being a screen / image level effect? If there is dithering of smooth gradients on the surface of lit non-textured objects that seems like it could be a different thing too. I think there will also be kind of distressed masked borders in a lot of cases so the rendered area will not be 100% rectangular with sharp edges.

**Q: For the distressed/masked borders on rendered areas — are these procedurally generated (noise-based rough edges), hand-drawn masks (like torn paper overlays), or parametric (configurable raggedness level)?**
A: Either hand-drawn masks or possibly some kind of procedural thing if it's possible to do with good results based on a polygon/rectangle frame.

**Q: For the pixel editor/animator (Aseprite-like) — is this the same widget as the MacPaint/HyperCard app, or a separate specialized tool? How do they relate?**
A: Separate tools.

**Q: For the overall visual identity system — you've mentioned paint app, pixel editor, 3D diorama tool, dithering. Is there a unifying name for this visual/creative subsystem, or should each tool be named independently?**
A: Well the pixel editor is probably a widget, because it will be used for making/editing sprites & animations & tiles for SNES-ish games. The doodling thing might be a thing too. But overall this is just the kind of design system for the whole Lapideus app / operating system.

**Q: For the layers system in the paint app — are layers purely visual stacking, or do they have semantic meaning (background layer, interactive layer, annotation layer) that affects behavior?**
A: Pure visual.

**Q: For the 'design system' framing — should the spec codify things like: spacing units (everything in 8px increments), standard UI element sizes (button heights, margins), color usage rules (which palette colors for which purposes)?**
A: Well there are some hard limits like the 16 color palette, the resolution, monospace fonts, no scrolling but I think within that framework there will be some amount of flexibility.

**Q: For SNES-ish games you mentioned the pixel editor supports — is game development a first-class use case of Lapideus, or is this more about the memory palace / spatial navigation UI modes that happen to use game-like graphics?**
A: It's possible you will be using it as a development machine for making cartridge based games for gameboy form factor SNES-level capability device. It's possible there will be actual game development in Lapideus as well.

**Q: For the gameboy-form-factor SNES-level device — is this hypothetical future hardware you're designing toward, existing retro hardware (Analogue Pocket, etc), or a software emulator target within Lapideus?**
A: Custom hardware.

**Q: For the 720x480 resolution (640x480 with modern aspect ratio) — how does this map to physical display? Is it integer-scaled to fill the window, letterboxed to preserve pixels, or stretched to fit available space?**
A: Stretch to fit.

**Q: For custom hardware — is this something you're actively designing/prototyping, or is it a long-term vision that shapes current software decisions? And does it constrain the software renderer (must work on specific chip)?**
A: Long-term vision.

**Q: For UI elements beyond drawing — are standard widgets (buttons, text fields, lists, toggles) also part of this design system spec, or is widget appearance already established elsewhere?**
A: There would be a standard library of widgets like that at some point probably but those have not been defined yet.

**Q: For the standard widget library — should this spec define the visual vocabulary for these (how a button looks, how focus is indicated), or is that a separate future spec that builds on this one?**
A: Outline only.

**Q: For animation within the design system — beyond sprite animation in the pixel editor, what role does motion play? Transitions between screens, feedback animations, idle states?**
A: There could be some animation and transitions. There will also probably be sound effects at some point.

**Q: For sound effects — is there a similar constraint philosophy? Limited sample rate, chiptune-style synthesis, a curated library of sounds, or full audio flexibility unlike the visual constraints?**
A: It will probably be kind of chiptune / retro but the sound synthesizer, etc tools are a whole other system.

**Q: For the relationship between this design system and content authoring — when someone creates a Scientia article or Mercatura listing, do they use these tools directly (embed doodles, place 3D dioramas), or is content mostly text with illustrations as a separate asset type?**
A: Both workflows.

**Q: For the 3D model library with poly count limits — what's the approximate target? N64 era (~500-1000 polys per model), PS1 era (~200-500), or even lower for a more abstract/iconic look?**
A: PS1 level (~200-500).

**Q: For the spec naming — since this covers the visual identity / design system for all of Lapideus, what should it be called?**
A: Vultus.

**Q: For the 3D model format — what file format for bundled/imported models? Something standard (OBJ, glTF), a custom binary format optimized for the constraints, or both with conversion on import?**
A: Probably both.

**Q: For the dithering algorithms — you mentioned the JS example. Is Floyd-Steinberg the target, or do you want multiple dithering options (ordered/Bayer, pattern dithering, etc.) for different aesthetic effects?**
A: Context-based.

**Q: For the different dithering contexts — what determines which algorithm? Automatic based on content type (photos vs illustrations), user selection per image, or semantic (Scientia uses X, Mercatura uses Y)?**
A: Probably semantic defaults. But definitely I think tend towards Floyd-Steinberg / Atkinson unless other dithering style particularly has good effects in a particular context.

**Q: For the paint app's HyperCard-style behavioral elements — can these trigger navigation to other screens/widgets, or are behaviors scoped within the paint canvas?**
A: Probably full system access, but I think the most common use cases would be navigation. Although I could see some HyperCard-ish animation. Honestly it's possible there could be some HyperCard/SuperCard/Director functionality for animations and interactivity, etc.

**Q: For HyperCard-style stacks/cards — is this a separate organizational concept from the existing schirmata screens, or could a HyperCard stack BE a widget that runs on a schirmata screen?**
A: I think you could have widgets that kind of are HyperCard stacks and possibly this could be running inside of a widget like the pagina text editor. So probably you would be able to add a background and/or foreground layer to any screen but you would also be able to have pages display as a widget that are HyperCard-y screens in a widget / half screen area.

**Q: For the print output of visual content — beyond printable card decks mentioned in Scientia, can paint app creations, dioramas, etc. be exported for physical printing? And at what resolution/format?**
A: The whole system will be print aware and the visual language of print will probably be dynamically generated based on print specific form factor. Possibly black and white, but this will be trying to have more of a manuscript / classic typography / text design language.

**Q: For print output with manuscript/classic typography — is this a different font (serif/proportional for print vs monospace for screen), or does the monospace constraint apply everywhere?**
A: Print might get proportional or it might stick to monospace. Would definitely be a much higher resolution font.

**Q: For the background/foreground layers on any screen — what's the authoring model? Toggle into 'edit mode' to draw on layers, or a separate editor that produces layers you can apply to screens?**
A: In-place edit mode.

**Q: For the relationship between Vultus and the memory palace / Zelda-style navigation mentioned in other specs — does the 3D diorama system provide the visual foundation for those spatial UIs, or are they separate visual systems?**
A: Separate systems.

**Q: For the first milestone of Vultus — what's the minimum slice that proves the concept and is actually useful?**
A: Dithering pipeline.

**Q: For the dithering pipeline milestone — what's the input format? PNG/JPEG from filesystem, drag-and-drop from OS, paste from clipboard, or built-in camera capture?**
A: Probably file, clipboard and maybe camera.

**Q: For the biggest risk or open question about Vultus — what keeps you up at night?**
A: Just got need to iterate with the aesthetics in front of me to make something that looks good in different domains, like dithered images, JRPG tiles/sprites, 3D, etc.

**Q: For iterating on aesthetics — is there a mood board or reference collection of visual styles you're drawing from? Obra Dinn, specific PS1 games, medieval manuscripts, something else?**
A: Dithering, classic Mac, and then basically the aesthetic that is possible with the very limited color palette and "480p" pixel resolution.

**Q: Is there anything about Vultus I haven't asked about that's important to capture in the spec?**
A: All covered.
