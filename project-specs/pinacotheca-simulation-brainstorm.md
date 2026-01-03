# Pinacotheca Implementation Simulation - Follow-up Interview

Date: 2026-01-02

## Context

After simulating 3 hours of implementing Pinacotheca, several complications and design decisions surfaced. This interview captures decisions on how to handle them.

## Q&A Session

**Q: The simulation revealed that during import, the dialog (left panel) and preview (right panel) need to share state (zoom, pan, dithering params). How should this cross-widget communication work?**
A: Maybe we can investigate using the actor system or some of the stuff from unified state paradigm to have them be entities sending messages to each other?

**Q: Using the actor/entity message system for dialog↔preview communication - should the 'import session' itself be an entity that both widgets observe, or should widgets send messages directly to each other?**
A: Maybe a session entity could make sense, we can try that and see if that works well.

**Q: The entity repository doesn't have a 'query all entities of genus X' function. For listing all images, the simulation used a tag workaround. Should we add proper genus querying to the repository?**
A: Add to repository.

**Q: The simulation noted that text input (for naming images, renaming) doesn't have a reusable widget. Should we build a proper text input field widget as a prerequisite, or inline the logic in each place that needs it?**
A: Well its a little confusing but widgets are kind of like the whole "apps" that can be in a whole panel and I think a text input or slider would be more like a "gui component" or something. But making a reusable text input gui component and slider makes sense.

**Q: For GUI components (text input, slider, buttons, palette grid) - should these be a new module with a consistent API, or ad-hoc implementations per dialog?**
A: New elementa module.

**Q: For the elementa GUI component system - should components be immediate-mode (render + handle events each frame, no retained state) or retained-mode (create component once, it maintains its own state)?**
A: Immediate mode.

**Q: For immediate-mode GUI - the containing dialog needs to track values (slider position, text field content). Should elementa functions take value pointers they modify, or return new values?**
A: I would say return new value if that is practical but I think any of these ways would be fine.

**Q: The simulation used a 'scratch piscina' that gets reset each dither to avoid accumulating old results. Is this a good pattern, or should dithering reuse a fixed output buffer?**
A: Scratch piscina.

**Q: The simulation found that base64-encoded compressed images are ~53KB per image. Is storing this in a single entity property acceptable, or should we use a different storage approach?**
A: We don't want to store it as base64 encoding, we want to store it as a gzipped binary format I think.

**Q: For storing gzipped binary directly - the entity system currently only supports chorda (text) properties. Should we add a new binary property type, or store binary in a separate blob storage system?**
A: Add TYPUS_BLOBUM.

**Q: For TYPUS_BLOBUM - should compression (flatura) be automatic when storing/retrieving blob properties, or explicit at the call site?**
A: Automatic.

**Q: The Objective-C clipboard code needs to link against Cocoa. Should we compile it as a separate .m file with special flags, or wrap the ObjC in a dylib we load at runtime?**
A: Separate m file you can see where we are already doing this with some objective c .m files.

**Q: The simulation suggested building prerequisites in order: elementa GUI components → dialogus framework → dithering → pinacotheca. Does this order make sense, or would you prefer a different sequence?**
A: As suggested.

**Q: For the elementa module - which GUI components should be included in the initial version? The simulation identified: slider, text input, button, palette grid (2x8 toggleable colors).**
A: Add checkbox too.

**Q: For immediate-mode GUI rendering - should elementa components render directly to TabulaPixelorum, or through ContextusDelineandi with clipping support?**
A: They should be drawing in an area provided by the widget? I guess you would want to provide multiple options. If the widget gives it something and coordinates as to where to draw that seems fine.

**Q: For the session entity pattern (Sessio::Importatio) - should this entity be persisted to storage, or purely ephemeral in-memory during the import session?**
A: I think if it's part of the entity repository it is just persisted? That seems fine to me.

**Q: The simulation noted potential performance concern with live dithering during pan/drag. If benchmarks show it's too slow, what's the fallback?**
A: Trust it's fast.

**Q: For quaerere_cum_genere (query by genus) - should it match exact genus only, or also match child genera? For example, should querying 'Vultus' return 'Vultus::Imago' entities?**
A: Both options.

**Q: Is there anything about the Pinacotheca implementation approach or the complications from the simulation that I haven't asked about that's important to capture?**
A: All covered.

## Key Decisions Summary

1. **Cross-widget state**: Use session entity (Sessio::Importatio) via actor/entity system
2. **Entity querying**: Add quaerere_cum_genere to repository (both exact and prefix match)
3. **GUI components**: New "elementa" module with immediate-mode API
4. **Components included**: Slider, text input, button, palette grid, checkbox
5. **Immediate-mode style**: Return new values (functional style)
6. **Memory management**: Scratch piscina pattern for dithering
7. **Binary storage**: Add TYPUS_BLOBUM with automatic compression
8. **ObjC clipboard**: Separate .m file (existing pattern)
9. **Build order**: elementa → dialogus → dithering → pinacotheca
10. **Render approach**: Flexible - components draw in provided area/coordinates
11. **Session persistence**: Entities persist naturally, that's acceptable
12. **Performance**: Trust it's fast, no fallback planned
