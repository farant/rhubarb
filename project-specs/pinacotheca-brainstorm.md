# Pinacotheca (Image Gallery & Dithering Pipeline) - Raw Interview Brainstorm

Date: 2026-01-02

## Q&A Session

**Q: For the image import pipeline, what's the expected workflow? Does the user select a file through a file picker widget, or do they invoke a command with a path, or both?**
A: Probably command with path and clipboard to start.

**Q: After dithering an image, what happens to it? Is it immediately displayed in a viewer, saved to a specific location, or returned as an in-memory asset for the calling widget to use?**
A: I think we need a new widget that is like an image gallery? Because I think to start out the main application of having images in the app will be that you can have the text editor in the left widget and then pick an image to be in the facing widget on the other half of the screen just as a kind of inspiration or devotional image. So we need that widget to have a browser type thing where you can pick a particular image, we need to have the ability to name and rename images so that you can have $image command in the text editor that pulls up a particular named image.

**Q: For the image gallery widget - is it a paginated grid of thumbnails where you select one to view full-size, or more like a carousel/slideshow, or a list view with image names?**
A: Probably kind of a list with preview to start? And maybe controls to rename / name the image in the preview area? I would want it to be navigable with vim keybindings.

**Q: For the $image command in the text editor - when you type $image <name> and it displays in the facing widget, should that persist across sessions (saved with the document) or is it ephemeral (just for current viewing)?**
A: It is just like any other command that shows a widget in the facing panel. And it would just show the image vertically and horizontally centered in the right hand panel. Which is the same view/widget as you would see in the image gallery if you would choose an image from the list. It's basically a deep link into the image gallery widget.

**Q: For the dithering process itself - should users have any control over dithering parameters (algorithm choice, error diffusion strength), or is it fully automatic with sensible defaults?**
A: I have two references for you as to how the code works that are in JavaScript HTML and I am happy with. First is reference/dithering-example.js and 2nd is reference/aquinas-dithering-tool.html which is a tool based on the dithering-example.js. So basically it lets you toggle which of the 16 colors are a part of the dithered image palette and the dithering process. In our widget for this import process I would like it to live update as you select/deselect colors without having to click the process button. This will be a new kind of interaction because I think it will be two panel. So when you initiate the import by pasting or using the command it actually will change the left hand panel to a full widget (or maybe an inline dialog, that is like 80% of the width and height of a normal widget?) This is what gives you the controls to name the image pick the palette of colors, move the black point sliders (with live update ideally as you interact with the sliders) that are in the dithering tool html. And then the right side will basically be a preview of the dithered image. You will also probably have a zoom slider in the left hand pane and a control to click and drag the image around in the right pane to control cropping. By default the image will be a little less than 480px high (basically the size of the widget drawing area, which I think is 480 minus the size of the schirmata bar on the bottom). When you click the done button in the left hand panel the right panel will be the image gallery widget deep linked to the image you just created and the left hand panel will dismiss the "dialog" and return to continue/reveal whatever content it was working with before.

**Q: For the two-panel import UI - when you say 'inline dialog that is 80% of width and height', do you mean it floats over the existing left widget content (modal overlay), or does it fully replace the left widget temporarily?**
A: Modal overlay, not necessarily even with the dimmed content because I don't know if that would be easy to do. We don't really have a blitter. You might be able to do one of our patterns with a light or dark color and as foreground and "transparent" as the background color to overlay the underlying widget and create an effect like that though.

**Q: For the color palette selection UI in the import dialog - how should the 16 colors be displayed? As a 4x4 grid of clickable squares, a horizontal strip, or integrated with color names/labels?**
A: You can see how the aquinas-dithering-tool.html in reference folder does it but basically it is a 2x8 grid of colors with a few preset palette options as buttons above it like "all", "gray", "warm", "cool" and "mono".

**Q: For the live update behavior during dithering - with real-time slider changes, are you concerned about performance? Should there be a debounce delay, or attempt immediate re-render on every slider tick?**
A: I'm not concerned about performance, in the browser we can dither live video at 60fps so I think it should be fine.

**Q: For the cropping/panning control in the preview - you mentioned click-and-drag to move the image. How do you envision the crop frame itself? Fixed to the widget output size, or adjustable with handles?**
A: Fixed + zoom.

**Q: For the grayscale bucketing mode with breakpoint sliders - I see there are 3 breakpoints in the HTML tool. Should we keep that same number, or is this configurable based on how many grayscale colors are selected?**
A: The way aquinas-dithering-tool is dithering images right now is very good so I think we want to replicate its algorithm as closely as possible. So I would just stick with fixed 3 sliders regardless of palette selection.

**Q: For the image storage format - you mentioned .vim in the v2 spec. Should imported images be stored as palette indices (8-bit per pixel), or as the full indexed file format with header and metadata like name?**
A: Well we are going to actually want store the data in the single data file / entity system. So that is one thing is we need to make sure that supports a binary data type for entity field. And ideally it might even transparently use flatura.h to compress/decompress the image data on storing and access.

**Q: For the entity genus of imported images - should images be their own top-level genus (like Imago::Dithered), or under Vultus (like Vultus::Imago), or something else?**
A: Vultus::Imago.

**Q: For the binary data field in entities - does the current entity system already have binary blob support, or is this something we need to add as part of this feature?**
A: Not sure.

**Q: For the image gallery widget name - what should it be called? Something descriptive like 'Imaginum Visus' (image viewer) or more evocative?**
A: Pinacotheca.

**Q: For the import dialog widget name - the modal overlay that appears during import with palette controls, sliders, etc. What should this be called?**
A: We can call it dithering dialog or something. But this will be a reusable UI affordance so we do want to make a "dialog" module or something that can be reused across different widgets when they want this kind of behavior. So basically overlay a smaller widget on the left, maybe dim the underlying widget, then restore underlying widget once dialog is dismissed / completed.

**Q: For the reusable dialog system - should the dialog take over keyboard focus entirely (modal), or should escape key be the only way to dismiss, or should there be flexibility in how dialogs handle input?**
A: I think the modal would own all input for the left hand pane when it is focused (it could be the right hand pane widget is focused) and I think the dialog itself would handle when it is dismissed. Although you might have a default escape handler so that if the modal wants to have that behavior it is easy for it to opt-in to it and run its cleanup function or whatever? But not too worried about that for now. I don't mind having an explicit "cancel" button that the modal is in charge of providing if it wants to provide the option to cancel (since some modals might not allow cancel).

**Q: For the $image command in the text editor - when the command is executed, should it just show the named image, or also provide controls in the facing panel to browse to other images?**
A: So the browse stuff is not in the facing panel, that is part of the right hand widget. Basically there would be a kind of "ranger" UI where right hand / pinacotheca widget is split with jk navigable list in its left half and preview in its right half, then enter would go to the image view and escape would go back to the browser view. For what we are doing now the modal would only be utilized when importing an image to do running like $import-image /path/to/image.png or pasting an image from the clipboard.

**Q: For pasting from clipboard - how should that be triggered? A global keyboard shortcut (cmd+v) when a text editor is focused, or a specific command like $paste-image?**
A: So far we have no clipboard integration and I haven't decided how or if I want to do that yet for text. So it would be command v and then smart detection as to if an image is pasting would trigger it.

**Q: For clipboard image reading on macOS - this requires native APIs (NSPasteboard). Do you want to add this capability now as part of this feature, or defer clipboard to later and focus on file import first?**
A: Add it now, I think we would want to wrap this in a platform clipboard library or something somehow.

**Q: For loading image files (PNG, JPEG, etc.) - the v2 spec mentioned stb_image.h. Are you okay with using this single-header library, or do you want to implement image decoding from scratch?**
A: I guess we can probably use stb_image.h but wrap it in our own "platform" API for image operations probably? Might implement our own later.

**Q: For the 'color dithering vs grayscale bucketing' toggle in the HTML tool - should this be exposed in our import dialog, or pick one mode as default and hide the complexity?**
A: I think color only for now.

**Q: For the zoom slider behavior - when zooming in, should the crop frame stay centered on the image, or maintain whatever pan position the user had set?**
A: Stay centered.

**Q: For naming images - is the name required before saving, or can images be saved unnamed with auto-generated names like 'imago_001'?**
A: I think it could be prefilled with a timestamp name but name would be required.

**Q: For the slider UI in the import dialog - do we have existing slider widget code to reuse, or does this need to be built as a new UI primitive?**
A: Need to build.

**Q: For the slider widget design - should it be keyboard-navigable (left/right to adjust value) and/or mouse-draggable, or both?**
A: Mouse only.

**Q: For the 2x8 palette grid - should clicking toggle colors immediately trigger re-dither, or wait until the user moves to another control?**
A: Immediate re-dither.

**Q: For the Pinacotheca list view - should images be sorted alphabetically by name, by creation date, or by most recently viewed?**
A: Alphabetical.

**Q: For editing images after import - can users re-open the dithering dialog to adjust an existing image's palette/settings, or is it one-way (import only, no re-edit)?**
A: One-way import.

**Q: For deleting images from Pinacotheca - should there be a delete command/button, and if so, should it have a confirmation dialog?**
A: Delete + confirm.

**Q: For the dialog modal system architecture - should dialogs be a separate layer in schirmata (like an overlay system), or managed by each widget individually?**
A: Hybrid approach.

**Q: For the preset palette buttons (All, Grays, Warm, Cool, Mono) - should these be keyboard-accessible with shortcuts, or mouse-only like the sliders?**
A: Mouse only.

**Q: For the output image size - should the dithered result always be exactly widget-height (roughly 480px minus schirmata bar), or should users be able to choose a different fixed output resolution?**
A: I think by default ~448px high and crop to aspect ratio of widget but can resize to something smaller than that.

**Q: For the flatura.h compression mentioned - is this compression library already implemented, or is it something we need to build as part of this feature?**
A: Already exists.

**Q: For the 'done' button in the import dialog - what should the label be? And should there be a 'cancel' button as well?**
A: Salvare / Abicere.

**Q: Is there anything about the dithering pipeline or Pinacotheca widget that I haven't asked about that's important to capture in the spec?**
A: All covered.
