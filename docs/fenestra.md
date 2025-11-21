# FENESTRA: Native Window Library Documentation

Look, another person asking "how do I make a window." You know how many times I've explained this? Let me save you some time: if you just want to slap ImGui on top of something and call it a day, go use GLFW. This library is for people who want to understand what's actually happening when they put pixels on the screen.

FENESTRA means "window" in Latin. Get used to it. You're writing in Latin now because you've committed to this insane project. **fenestra** (noun, feminine, first declension) = window. Plural is **fenestrae**. The genitive is **fenestrae**. You'll see this pattern everywhere.

## What This Library Actually Does

FENESTRA creates native windows and lets you draw pixels to them. That's it. No OpenGL context. No Vulkan. No Metal. Just a buffer of pixels in RGBA8888 format that you fill however you want, and the library copies them to the screen.

It also handles events - keyboard, mouse, window resize, that kind of thing. Events go into a ring buffer so you can process them at your leisure. The ring buffer is 256 events deep, which is way more than you need unless you're doing something incredibly stupid.

The library includes a built-in 6x8 bitmap font so you can draw text without pulling in FreeType or some other monstrosity. It's not pretty, but it works. Sometimes "works" is all you need.

### Why Software Rendering?

Because hardware rendering is a disaster. You want to link against OpenGL? Great, now you're dealing with context creation, extension loading, shader compilation, and a thousand ways to shoot yourself in the foot. You want Vulkan? Hope you enjoy writing 500 lines of boilerplate to draw a triangle.

With software rendering, you write to a pixel buffer. That's it. The CPU is fast enough for most things. If you need hardware acceleration later, you can add it. But start simple.

## Installation and Setup

You need macOS. This library uses Cocoa/NSWindow under the hood. The implementation file is `fenestra_macos.m` - the `.m` extension means Objective-C. You're mixing C and Objective-C, which is legal and sometimes necessary when dealing with macOS APIs.

To compile, you need to link against Cocoa and Carbon frameworks:

```bash
clang -framework Cocoa -framework Carbon fenestra_macos.m fenestra_textus.c your_program.c -o your_program
```

Don't forget `-framework`. I've seen people waste hours because they forgot the framework flag. Don't be that person.

## Your First Window

Let me show you the absolute minimum code to get a window on screen:

```c
#include "fenestra.h"
#include "piscina.h"

integer principale(vacuum)
{
    Piscina* piscina;
    FenestraConfiguratio configuratio;
    Fenestra* fenestra;

    /* Create memory pool - "piscina" means "pool" in Latin */
    piscina = piscina_generare_dynamicum("fenestra", M);
    si (!piscina)
    {
        redde I;  /* "redde" = "return" */
    }

    /* Configure window */
    configuratio.titulus = "My First Window";
    configuratio.x = C;              /* 100 pixels from left */
    configuratio.y = C;              /* 100 pixels from top */
    configuratio.latitudo = DCCC;    /* 800 pixels wide */
    configuratio.altitudo = DCCC;    /* 800 pixels tall */
    configuratio.vexilla = FENESTRA_ORDINARIA;

    /* Create window */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (!fenestra)
    {
        piscina_destruere(piscina);
        redde I;
    }

    /* Main loop */
    dum (!fenestra_debet_claudere(fenestra))
    {
        fenestra_perscrutari_eventus(fenestra);
        fenestra_dormire(16666);  /* ~60 FPS */
    }

    /* Cleanup */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;  /* 0 = success */
}
```

Let me explain the Latin here before you ask:

- **piscina** = pool (for memory allocation)
- **configuratio** = configuration
- **titulus** = title
- **latitudo** = width (from Latin "latus" = wide)
- **altitudo** = height (from Latin "altus" = tall/high)
- **vexilla** = flags (plural of vexillum = banner/standard)
- **si** = if
- **dum** = while
- **redde** = return
- **ZEPHYRUM** = zero (from Greek "zephyros" = west wind, metaphorically "nothing")

Those Roman numerals? **C** = 100, **M** = 1000, **DCCC** = 800. You'll get used to it.

### What's FenestraConfiguratio?

It's a struct (or **structura** in Latin) that holds window settings:

```c
nomen structura {
    constans character* titulus;  /* Window title */
    i32 x;                         /* X position */
    i32 y;                         /* Y position */
    i32 latitudo;                  /* Width */
    i32 altitudo;                  /* Height */
    i32 vexilla;                   /* Window flags */
} FenestraConfiguratio;
```

The flags (**vexilla**) control window behavior:

- `FENESTRA_MUTABILIS` - resizable (from "mutare" = to change)
- `FENESTRA_CLAUDIBILIS` - closable (from "claudere" = to close)
- `FENESTRA_MINUIBILIS` - minimizable (from "minuere" = to diminish)
- `FENESTRA_MAXIMIZABILIS` - maximizable (from "maximizare" = to maximize)
- `FENESTRA_CENTRATA` - centered on screen (from "centrum" = center)
- `FENESTRA_PLENA_VISIO` - fullscreen (from "plenus" = full, "visio" = view)
- `FENESTRA_ORDINARIA` - standard window with all the usual controls

`FENESTRA_ORDINARIA` is just a combination of the common flags. Look at the header:

```c
#define FENESTRA_ORDINARIA (FENESTRA_MUTABILIS | FENESTRA_CLAUDIBILIS | \
                            FENESTRA_MINUIBILIS | FENESTRA_MAXIMIZABILIS)
```

It's a bitwise OR. Basic stuff. If you don't know what bitwise OR is, you're not ready for this library.

## Drawing Pixels

Alright, you've got a window. Now what? You want to draw something? You need a **TabulaPixelorum** - a "table of pixels" or what normal people call a pixel buffer.

```c
TabulaPixelorum* tabula;
i32 latitudo_fixa;

latitudo_fixa = CDLXXX;  /* 480 pixels tall */
tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, latitudo_fixa);
si (!tabula)
{
    /* Handle error */
}
```

Wait, why do you only specify height? Because the library calculates width from the window's aspect ratio. If your window is 800x600, and you ask for 480 pixels tall, you get 640x480. The math is simple:

```
aspect_ratio = window_width / window_height
buffer_width = buffer_height * aspect_ratio
```

This way the buffer scales to fit the window. When the window resizes, you might need to recreate the buffer. Or don't - the library will stretch it. Looks ugly, but it works.

### Clearing the Buffer

```c
tabula_pixelorum_vacare(tabula, RGB(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
```

**vacare** means "to empty." This fills the entire buffer with black (R=0, G=0, B=0). The `RGB()` macro creates a 32-bit color value in RGBA8888 format:

```c
#define RGB(r, g, b) RGBA(r, g, b, CCLV)
#define RGBA(r, g, b, a) (((i32)(a) << XXIV) | ((i32)(b) << XVI) | \
                          ((i32)(g) << VIII) | (i32)(r))
```

Yes, those are Roman numerals in the bit shifts. **VIII** = 8, **XVI** = 16, **XXIV** = 24, **CCLV** = 255. Alpha is always 255 (fully opaque) unless you use `RGBA()` directly.

### Setting Individual Pixels

```c
tabula_pixelorum_ponere_pixelum(tabula, x, y, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

**ponere** = to put/place. **pixelum** = pixel (singular of pixela). This sets pixel at (x, y) to red.

Coordinates start at (0, 0) in the top-left corner. X increases to the right, Y increases downward. Standard screen coordinates, not math coordinates.

### Getting Pixel Values

```c
i32 color = tabula_pixelorum_obtinere_pixelum(tabula, x, y);
```

**obtinere** = to obtain/get. Returns the color value at that pixel. Useful for hit detection or effects.

### Presenting to the Screen

```c
fenestra_praesentare_pixela(fenestra, tabula);
```

**praesentare** = to present. This copies your pixel buffer to the window. Under the hood it's creating a `CGImage` from your buffer and telling the NSView to redraw. You don't need to know that, but I'm telling you anyway.

Put this at the end of your main loop after you've drawn everything. The library handles scaling if the window is a different size than your buffer.

## A Complete Drawing Example

Here's a program that draws random noise. It's ugly but demonstrates the concepts:

```c
#include "fenestra.h"
#include "piscina.h"
#include <stdlib.h>
#include <time.h>

integer principale(vacuum)
{
    Piscina* piscina;
    FenestraConfiguratio config;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    i32 x, y;

    srand((unsigned int)time(NIHIL));

    piscina = piscina_generare_dynamicum("fenestra", M);
    si (!piscina) redde I;

    config.titulus = "Random Noise";
    config.x = C;
    config.y = C;
    config.latitudo = DCCC;
    config.altitudo = DC;
    config.vexilla = FENESTRA_ORDINARIA;

    fenestra = fenestra_creare(piscina, &config);
    si (!fenestra)
    {
        piscina_destruere(piscina);
        redde I;
    }

    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    si (!tabula)
    {
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde I;
    }

    dum (!fenestra_debet_claudere(fenestra))
    {
        fenestra_perscrutari_eventus(fenestra);

        /* Draw random noise */
        per (y = ZEPHYRUM; y < tabula->altitudo; y++)
        {
            per (x = ZEPHYRUM; x < tabula->latitudo; x++)
            {
                i32 r = rand() % CCLVI;  /* 0-255 */
                i32 g = rand() % CCLVI;
                i32 b = rand() % CCLVI;
                tabula_pixelorum_ponere_pixelum(tabula, x, y, RGB(r, g, b));
            }
        }

        fenestra_praesentare_pixela(fenestra, tabula);
        fenestra_dormire(16666);
    }

    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

**per** = for. **CCLVI** = 256. **DC** = 600. This draws completely random pixels every frame at about 60 FPS. Will it hurt your eyes? Yes. Does it demonstrate pixel buffer manipulation? Also yes.

Direct pixel access is slow for large buffers. If you're drawing 640x480 = 307,200 pixels every frame, you're doing 307,200 function calls. That's why `tabula_pixelorum_vacare()` exists - it's optimized to fill the whole buffer at once.

## Event Handling

Events tell you what the user is doing. Keyboard presses, mouse clicks, window resizes - all events. They're stored in a ring buffer inside the `Fenestra` struct.

### The Event Loop

```c
dum (!fenestra_debet_claudere(fenestra))
{
    fenestra_perscrutari_eventus(fenestra);

    Eventus eventus;
    dum (fenestra_obtinere_eventus(fenestra, &eventus))
    {
        commutatio (eventus.genus)
        {
            casus EVENTUS_CLAVIS_DEPRESSUS:
                /* Handle key press */
                frange;

            casus EVENTUS_MUS_DEPRESSUS:
                /* Handle mouse button */
                frange;

            ordinarius:
                frange;
        }
    }

    /* Draw your frame */

    fenestra_dormire(16666);
}
```

Let me translate:

- **perscrutari** = to search through/poll
- **eventus** = event
- **commutatio** = switch statement
- **casus** = case
- **ordinarius** = default
- **frange** = break
- **genus** = type/kind
- **clavis** = key
- **depressus** = pressed (past participle of "deprimere" = to press down)

`fenestra_perscrutari_eventus()` pulls events from the OS and puts them in your ring buffer. `fenestra_obtinere_eventus()` pops events off the buffer one at a time. When the buffer is empty, it returns `FALSUM` (false) and you move on.

### Event Types

The `Eventus` struct has a `genus` field (type) and a `datum` union (data):

```c
nomen structura {
    eventus_genus_t genus;
    unio {
        structura {
            i32 latitudo;
            i32 altitudo;
        } mutare_magnitudinem;

        structura {
            clavis_t  clavis;
            character typus;
            i32       modificantes;
        } clavis;

        structura {
            i32          x;
            i32          y;
            mus_botton_t botton;
            i32          modificantes;
        } mus;

        structura {
            f32 delta_x;
            f32 delta_y;
        } rotula;
    } datum;
} Eventus;
```

**unio** = union. Only one field is valid at a time, determined by `genus`. If `genus` is `EVENTUS_CLAVIS_DEPRESSUS`, read `datum.clavis`. If it's `EVENTUS_MUS_MOTUS`, read `datum.mus`. If you read the wrong one, you get garbage. This is C, not Rust. No hand-holding.

Event types:

- `EVENTUS_NIHIL` - nothing happened (shouldn't see this)
- `EVENTUS_CLAUDERE` - user wants to close the window
- `EVENTUS_MUTARE_MAGNITUDINEM` - window resized
- `EVENTUS_FOCUS` - window gained focus
- `EVENTUS_DEFOCUS` - window lost focus
- `EVENTUS_EXPONERE` - window needs redraw (rarely used with software rendering)
- `EVENTUS_CLAVIS_DEPRESSUS` - key pressed down
- `EVENTUS_CLAVIS_LIBERATUS` - key released
- `EVENTUS_MUS_DEPRESSUS` - mouse button pressed
- `EVENTUS_MUS_LIBERATUS` - mouse button released
- `EVENTUS_MUS_MOTUS` - mouse moved
- `EVENTUS_MUS_ROTULA` - scroll wheel

### Keyboard Events

```c
si (eventus.genus == EVENTUS_CLAVIS_DEPRESSUS)
{
    clavis_t clavis = eventus.datum.clavis.clavis;
    character typus = eventus.datum.clavis.typus;
    i32 mods = eventus.datum.clavis.modificantes;

    si (clavis == CLAVIS_EFFUGIUM)
    {
        /* Escape key - exit program */
    }

    si (typus >= 'a' && typus <= 'z')
    {
        /* Lowercase letter was typed */
    }

    si (mods & MOD_SHIFT)
    {
        /* Shift key is held */
    }
}
```

Three pieces of information:

1. **clavis** - the physical key code (like `CLAVIS_A`, `CLAVIS_SPATIUM`, etc.)
2. **typus** - the character that was typed (accounts for shift, caps lock, etc.)
3. **modificantes** - modifier keys held (shift, control, alt, super)

Key codes are enums. Letters A-Z are their ASCII uppercase values ('A' through 'Z'). Numbers 0-9 are their ASCII values ('0' through '9'). Special keys have their own constants:

- `CLAVIS_EFFUGIUM` - Escape (from "effugere" = to flee)
- `CLAVIS_REDITUS` - Return/Enter (from "reditus" = return)
- `CLAVIS_SPATIUM` - Space (from "spatium" = space)
- `CLAVIS_RETRORSUM` - Backspace (from "retrorsum" = backward)
- `CLAVIS_TABULA` - Tab (from "tabula" = table/board)
- `CLAVIS_DELERE` - Delete (from "delere" = to destroy)

Arrow keys:
- `CLAVIS_SINISTER` - Left (from "sinister" = left)
- `CLAVIS_DEXTER` - Right (from "dexter" = right)
- `CLAVIS_SURSUM` - Up (from "sursum" = upward)
- `CLAVIS_DEORSUM` - Down (from "deorsum" = downward)

Function keys are `CLAVIS_F1` through `CLAVIS_F12`. Modifier keys:

- `MOD_SHIFT` - Shift key
- `MOD_IMPERIUM` - Control key (from "imperium" = command/control)
- `MOD_ALT` - Alt/Option key
- `MOD_SUPER` - Super/Windows/Command key
- `MOD_CAPS_LOCK` - Caps lock is on
- `MOD_NUM_LOCK` - Num lock is on

Check modifiers with bitwise AND:

```c
si (mods & MOD_IMPERIUM && clavis == 'S')
{
    /* Ctrl+S - save */
}
```

### Mouse Events

```c
si (eventus.genus == EVENTUS_MUS_DEPRESSUS)
{
    i32 x = eventus.datum.mus.x;
    i32 y = eventus.datum.mus.y;
    mus_botton_t botton = eventus.datum.mus.botton;

    si (botton == MUS_SINISTER)
    {
        /* Left click at (x, y) */
    }
}
```

Mouse coordinates are in *pixel buffer space*, not window space. If your buffer is 640x480 but your window is 1280x960, the library automatically scales coordinates. When you click the middle of the window, you get (320, 240), not (640, 480). This is what you want 99% of the time.

Mouse buttons:
- `MUS_SINISTER` - Left button
- `MUS_DEXTER` - Right button
- `MUS_MEDIUS` - Middle button

Mouse motion events give you the current position. There's no "delta" - if you want mouse delta, store the previous position yourself.

### Scroll Wheel

```c
si (eventus.genus == EVENTUS_MUS_ROTULA)
{
    f32 delta_x = eventus.datum.rotula.delta_x;
    f32 delta_y = eventus.datum.rotula.delta_y;

    /* Scroll up = positive delta_y */
    /* Scroll down = negative delta_y */
}
```

**rotula** = little wheel. Scroll values are floats because macOS gives you smooth scrolling with fractional values. On a traditional scroll wheel, you'll get integer values. On a trackpad, you'll get smooth sub-pixel scrolling.

Positive Y = scroll up. Negative Y = scroll down. Positive X = scroll right. Negative X = scroll left. Most mice only scroll vertically.

## Drawing Text

The library includes a 6x8 bitmap font. It's not beautiful, but it's functional and has zero dependencies. The font is baked into `fons_6x8.h` as a byte array.

### Basic Text

```c
chorda textus = chorda_ex_literis("Hello, World!", piscina);
tabula_pixelorum_pingere_chordam(tabula, X, X, textus, RGB(CCLV, CCLV, CCLV));
```

**chorda** = string (literally "chord" or "rope"). **pingere** = to paint. **X** = 10 in Roman numerals.

This draws white text at position (10, 10). The font is 6 pixels wide and 8 pixels tall per character. The text cursor advances 6 pixels per character, so "Hello, World!" is 78 pixels wide (13 chars × 6 pixels).

### Scaled Text

```c
i32 scala = II;  /* 2x scale */
tabula_pixelorum_pingere_chordam_scalatam(tabula, X, X, textus,
                                            RGB(CCLV, ZEPHYRUM, ZEPHYRUM), scala);
```

**scala** = scale/ladder. This draws text at 2x size - each character is 12x16 pixels instead of 6x8. The quality is blocky because it's just nearest-neighbor scaling, but it's readable.

### Clipped Text

```c
tabula_pixelorum_pingere_chordam_praecisum(
    tabula,
    X, X,              /* Position */
    textus,
    RGB(ZEPHYRUM, CCLV, ZEPHYRUM),  /* Green */
    C, C,              /* Clip rect: x=100, y=100 */
    CC, L              /* Clip rect: width=200, height=50 */
);
```

**praecisum** = cut off/clipped. Only pixels inside the clip rectangle (100, 100, 200, 50) are drawn. Everything outside is discarded. Useful for text boxes or scrolling areas.

### Measuring Text

```c
i32 latitudo = fons_latitudo_chordae(textus, I);  /* Scale = 1 */
i32 altitudo = fons_altitudo_chordae(I);          /* Scale = 1 */
```

**fons** = font/source. Returns the width and height in pixels. Height is always `8 × scale`. Width depends on the string length and whether there are newlines.

### Newlines

The font renderer handles '\n' characters:

```c
chorda multiline = chorda_ex_literis("Line 1\nLine 2\nLine 3", piscina);
tabula_pixelorum_pingere_chordam(tabula, X, X, multiline, RGB(CCLV, CCLV, CCLV));
```

Each '\n' moves the cursor down 8 pixels and back to the starting X position. It's basic but it works.

### Individual Characters

```c
tabula_pixelorum_pingere_characterem(tabula, x, y, 'A', RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

**character** = character. Draws a single character. Faster than creating a chorda for one letter, but usually you're drawing words, not individual letters.

## Window Properties

You can change window properties after creation. Some of this is straightforward, some of it isn't.

### Title

```c
fenestra_ponere_titulum(fenestra, "New Title");
```

Sets the window title. Takes a C string (null-terminated). The string is copied internally, so you don't need to keep it around.

### Size

```c
i32 latitudo, altitudo;
fenestra_obtinere_magnitudinem(fenestra, &latitudo, &altitudo);

fenestra_ponere_magnitudinem(fenestra, MXXIV, DCCLXVIII);  /* 1024x768 */
```

**magnitudo** = size/magnitude. Get and set work as you'd expect. Setting size might trigger a resize event, so handle that in your event loop.

### Position

```c
i32 x, y;
fenestra_obtinere_positum(fenestra, &x, &y);

fenestra_ponere_positum(fenestra, CC, CC);  /* (200, 200) */
```

**positum** = position. Position is relative to the bottom-left of the screen because macOS uses Cartesian coordinates. I know, it's backwards from everything else. Blame Apple.

### Center Window

```c
fenestra_centrare(fenestra);
```

Centers the window on screen. Useful after changing size.

### Maximize/Minimize

```c
fenestra_maximizare(fenestra);  /* Zoom to fill screen (not fullscreen) */
fenestra_minuere(fenestra);     /* Minimize to dock */
fenestra_restituere(fenestra);  /* Restore from minimized */
```

**restituere** = to restore. Maximizing on macOS doesn't mean fullscreen - it means "zoom to fit content" which usually makes the window bigger but not fullscreen. If you want actual fullscreen:

```c
fenestra_commutare_plenam_visionem(fenestra);
```

**commutare** = to toggle. Toggles fullscreen mode. Call it again to exit fullscreen.

### Show/Hide

```c
fenestra_monstrare(fenestra);  /* Show window */
fenestra_celare(fenestra);     /* Hide window */
```

**monstrare** = to show. **celare** = to hide. Hidden windows still exist and still receive some events, they're just not visible.

### Focus

```c
fenestra_focus(fenestra);  /* Give keyboard focus */

b32 has_focus = fenestra_habet_focus(fenestra);
b32 visible = fenestra_est_visibilis(fenestra);
```

**habet** = has (from "habere" = to have). **est** = is (from "esse" = to be). These return `VERUM` (true) or `FALSUM` (false).

## Timing Functions

The library includes platform timing functions because you need them for frame limiting.

### High-Precision Timer

```c
i64 initium = fenestra_tempus_obtinere_pulsus();

/* Do work */

i64 finis = fenestra_tempus_obtinere_pulsus();
f64 frequentia = fenestra_tempus_obtinere_frequentiam();

f64 secundae = (finis - initium) / frequentia;
```

**tempus** = time. **pulsus** = beat/pulse. **frequentia** = frequency. **initium** = beginning. **finis** = end. **secundae** = seconds.

`fenestra_tempus_obtinere_pulsus()` returns a 64-bit tick count from an arbitrary starting point. The ticks-per-second rate varies by system, so you need `fenestra_tempus_obtinere_frequentiam()` to convert ticks to seconds.

On macOS this uses `mach_absolute_time()` which is nanosecond-precision. Use it for profiling or frame timing.

### Sleep

```c
fenestra_dormire(16666);  /* Sleep for 16.666ms ≈ 60 FPS */
```

**dormire** = to sleep. Takes microseconds. 16666 microseconds = 16.666 milliseconds ≈ 60 FPS. Use this at the end of your main loop to cap frame rate.

Why microseconds? Because milliseconds are too coarse for precise timing. 16ms vs 17ms is the difference between 60 FPS and 58 FPS. With microseconds you can hit frame timings exactly.

Don't rely on sleep for precise frame timing. Sleep is "at least this long" not "exactly this long." For precise timing, measure frame time and adjust sleep duration each frame.

## Memory Management

This library uses Piscina (memory pool) for allocation. You don't call `malloc()` and `free()`. You allocate from a pool and destroy the whole pool at once.

### Why Pools?

Because `malloc()` and `free()` are slow, fragmented, and error-prone. With a pool:

1. Allocation is fast (bump allocator)
2. No fragmentation
3. No memory leaks - destroy the pool and everything is freed
4. No double-free bugs

### Creating a Pool

```c
Piscina* piscina = piscina_generare_dynamicum("fenestra", M);
```

Creates a pool named "fenestra" with 1MB initial size (M = 1000 in Roman numerals). The pool grows automatically if you allocate more than 1MB.

### Using the Pool

```c
Fenestra* fenestra = fenestra_creare(piscina, &configuratio);
TabulaPixelorum* tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
```

All FENESTRA functions that allocate memory take a `Piscina*` argument. The allocated memory belongs to the pool.

### Destroying Everything

```c
fenestra_destruere(fenestra);  /* Release native resources (NSWindow, etc.) */
piscina_destruere(piscina);    /* Free ALL pool memory at once */
```

`fenestra_destruere()` releases native OS resources (windows, graphics contexts). The memory is still owned by the pool - `piscina_destruere()` actually frees it.

Order matters: destroy FENESTRA objects before destroying the pool. Otherwise you're releasing OS resources from freed memory, which might crash.

### What Gets Allocated?

The `Fenestra` struct, the `TabulaPixelorum` struct, and the pixel buffer itself all come from the pool. For a 640x480 window:

- `Fenestra`: ~300 bytes (includes 256-element event queue)
- `TabulaPixelorum`: ~30 bytes
- Pixel buffer: 640 × 480 × 4 = 1,228,800 bytes

Total ≈ 1.2 MB for one window with one buffer. The 1MB pool will auto-expand.

## Platform-Specific Details

### macOS and Objective-C

The implementation uses Cocoa (NSWindow, NSView) which requires Objective-C. You're compiling `.m` files which means the compiler accepts both C and Objective-C syntax.

Inside `fenestra_macos.m` you'll see code like this:

```objc
[fenestra->fenestra_ns setTitle:[NSString stringWithUTF8String:titulus]];
```

That's Objective-C message sending. You don't need to understand it. The header (`fenestra.h`) only exposes C functions. The Objective-C is hidden.

### @autoreleasepool

You'll see this in the implementation:

```objc
@autoreleasepool {
    /* Cocoa code */
}
```

Cocoa uses reference counting for memory management. Objects need to be released. The autorelease pool handles temporary objects automatically. Each function that touches Cocoa wraps its code in an autorelease pool.

Why should you care? You probably shouldn't. But if you see "SIGABRT" crashes related to over-released objects, it might be an autorelease pool issue. File a bug.

### NSApplication

Creating a window requires initializing `NSApplication`. The library does this automatically on first window creation:

```objc
if (!NSApp)
{
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    /* More setup... */
}
```

This is why your program needs to link against Cocoa. Without it, you get linker errors about undefined NSApplication symbols.

### Event Polling

`fenestra_perscrutari_eventus()` calls `[NSApp nextEventMatchingMask:...]` to pull events from the OS event queue. The function runs in a loop until the queue is empty.

Events are translated from `NSEvent` to our `Eventus` struct. Key codes are mapped from macOS virtual key codes to our key constants. Mouse coordinates are transformed from window space to pixel buffer space.

### Performance

Software rendering on macOS works like this:

1. You fill a buffer with RGBA8888 pixels
2. `fenestra_praesentare_pixela()` creates a `CGImage` from your buffer
3. The image is stored in the NSView using associated objects
4. The view's `drawRect:` method draws the image using Core Graphics
5. Core Graphics scales the image to fill the view (if necessary)

The scaling uses `kCGInterpolationNone` (nearest-neighbor) to keep pixels sharp. If you want smooth scaling, you'll need to modify `fenestra_macos.m`.

Creating a new `CGImage` every frame is not free. For a 640x480 buffer, it takes maybe 0.1-0.5ms. At 60 FPS that's fine. At 1000 FPS it becomes noticeable.

If you need higher performance, consider keeping the CGImage around and using `CGImageCreate` with a data provider that points to your buffer. But that's more complex and probably unnecessary.

## Common Patterns

### Fixed Timestep Loop

```c
f64 frequentia = fenestra_tempus_obtinere_frequentiam();
i64 tempus_initium = fenestra_tempus_obtinere_pulsus();
f64 tempus_per_frame = 1.0 / 60.0;  /* 60 FPS */
f64 accumulator = 0.0;

dum (!fenestra_debet_claudere(fenestra))
{
    i64 tempus_currens = fenestra_tempus_obtinere_pulsus();
    f64 tempus_frame = (tempus_currens - tempus_initium) / frequentia;
    tempus_initium = tempus_currens;

    accumulator += tempus_frame;

    fenestra_perscrutari_eventus(fenestra);

    /* Fixed timestep updates */
    dum (accumulator >= tempus_per_frame)
    {
        /* Update game state */
        accumulator -= tempus_per_frame;
    }

    /* Render */
    tabula_pixelorum_vacare(tabula, RGB(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    /* Draw frame */
    fenestra_praesentare_pixela(fenestra, tabula);
}
```

This decouples update rate from render rate. Game logic runs at exactly 60 FPS. Rendering happens as fast as possible. If rendering is slow, multiple updates happen per frame to catch up.

### Dirty Rectangle Optimization

Instead of clearing and redrawing everything:

```c
/* Only clear and redraw changed regions */
si (dirty)
{
    per (y = dirty_y; y < dirty_y + dirty_height; y++)
    {
        per (x = dirty_x; x < dirty_x + dirty_width; x++)
        {
            /* Redraw pixel */
        }
    }
    dirty = FALSUM;
}
```

This is significantly faster for UIs where most of the screen is static.

### Double Buffering

The pixel buffer is already a back buffer. You draw to it, then `fenestra_praesentare_pixela()` copies it to the screen. That's double buffering.

If you want triple buffering (one buffer displaying, one ready to display, one being drawn), create multiple `TabulaPixelorum` and swap between them. But it's probably overkill.

---

# API Reference

Alright, here's every function in detail. If you've read this far, most of this should make sense.

## Window Creation and Destruction

### fenestra_creare

```c
Fenestra* fenestra_creare(
    Piscina* piscina,
    constans FenestraConfiguratio* configuratio
);
```

Creates a native window.

**Parameters:**
- `piscina` - Memory pool for allocation
- `configuratio` - Window configuration (title, size, flags)

**Returns:** New window, or `NIHIL` on error

**Notes:** Automatically initializes NSApplication on first call. Window is visible immediately unless you hide it.

### fenestra_destruere

```c
vacuum fenestra_destruere(Fenestra* fenestra);
```

Destroys a window and releases native resources.

**Parameters:**
- `fenestra` - Window to destroy

**Notes:** Memory is not freed - the pool still owns it. Call `piscina_destruere()` to actually free memory.

## Event Handling

### fenestra_debet_claudere

```c
b32 fenestra_debet_claudere(constans Fenestra* fenestra);
```

Checks if the window should close.

**Parameters:**
- `fenestra` - Window to check

**Returns:** `VERUM` if close was requested, `FALSUM` otherwise

**Notes:** This is set when user clicks the close button. Window doesn't actually close until you handle it.

### fenestra_perscrutari_eventus

```c
vacuum fenestra_perscrutari_eventus(Fenestra* fenestra);
```

Polls events from the OS and adds them to the event queue.

**Parameters:**
- `fenestra` - Window to poll events for

**Notes:** Call this once per frame, before processing events. Events go into a 256-element ring buffer.

### fenestra_obtinere_eventus

```c
b32 fenestra_obtinere_eventus(
    Fenestra* fenestra,
    Eventus* eventus
);
```

Gets the next event from the queue.

**Parameters:**
- `fenestra` - Window to get event from
- `eventus` - Output parameter for event data

**Returns:** `VERUM` if event was retrieved, `FALSUM` if queue is empty

**Notes:** Call in a loop until it returns `FALSUM`.

## Window Properties

### fenestra_ponere_titulum

```c
vacuum fenestra_ponere_titulum(
    Fenestra* fenestra,
    constans character* titulus
);
```

Sets the window title.

**Parameters:**
- `fenestra` - Window
- `titulus` - New title (null-terminated C string)

### fenestra_obtinere_magnitudinem

```c
vacuum fenestra_obtinere_magnitudinem(
    constans Fenestra* fenestra,
    i32* latitudo,
    i32* altitudo
);
```

Gets the window content size.

**Parameters:**
- `fenestra` - Window
- `latitudo` - Output: width in pixels
- `altitudo` - Output: height in pixels

**Notes:** This is the content area size, not including title bar and borders.

### fenestra_ponere_magnitudinem

```c
vacuum fenestra_ponere_magnitudinem(
    Fenestra* fenestra,
    i32 latitudo,
    i32 altitudo
);
```

Sets the window content size.

**Parameters:**
- `fenestra` - Window
- `latitudo` - New width
- `altitudo` - New height

**Notes:** May trigger a resize event.

### fenestra_obtinere_positum

```c
vacuum fenestra_obtinere_positum(
    constans Fenestra* fenestra,
    i32* x,
    i32* y
);
```

Gets the window position.

**Parameters:**
- `fenestra` - Window
- `x` - Output: X position
- `y` - Output: Y position

**Notes:** Position is relative to bottom-left of screen (macOS convention).

### fenestra_ponere_positum

```c
vacuum fenestra_ponere_positum(
    Fenestra* fenestra,
    i32 x,
    i32 y
);
```

Sets the window position.

**Parameters:**
- `fenestra` - Window
- `x` - New X position
- `y` - New Y position

## Window State

### fenestra_monstrare

```c
vacuum fenestra_monstrare(Fenestra* fenestra);
```

Shows the window.

### fenestra_celare

```c
vacuum fenestra_celare(Fenestra* fenestra);
```

Hides the window.

### fenestra_focus

```c
vacuum fenestra_focus(Fenestra* fenestra);
```

Gives the window keyboard focus.

### fenestra_est_visibilis

```c
b32 fenestra_est_visibilis(constans Fenestra* fenestra);
```

Checks if window is visible.

**Returns:** `VERUM` if visible, `FALSUM` if hidden

### fenestra_habet_focus

```c
b32 fenestra_habet_focus(constans Fenestra* fenestra);
```

Checks if window has keyboard focus.

**Returns:** `VERUM` if focused, `FALSUM` otherwise

## Window Utilities

### fenestra_centrare

```c
vacuum fenestra_centrare(Fenestra* fenestra);
```

Centers the window on screen.

### fenestra_maximizare

```c
vacuum fenestra_maximizare(Fenestra* fenestra);
```

Maximizes the window (zoom to fit).

**Notes:** On macOS this is "zoom" not fullscreen.

### fenestra_minuere

```c
vacuum fenestra_minuere(Fenestra* fenestra);
```

Minimizes the window to the dock.

### fenestra_restituere

```c
vacuum fenestra_restituere(Fenestra* fenestra);
```

Restores the window from minimized state.

### fenestra_commutare_plenam_visionem

```c
vacuum fenestra_commutare_plenam_visionem(Fenestra* fenestra);
```

Toggles fullscreen mode.

**Notes:** Call again to exit fullscreen.

### fenestra_obtinere_tractationem_nativam

```c
vacuum* fenestra_obtinere_tractationem_nativam(Fenestra* fenestra);
```

Gets the native window handle.

**Returns:** `NSWindow*` cast to `vacuum*`

**Notes:** For advanced platform-specific code only.

## Pixel Buffer

### fenestra_creare_tabulam_pixelorum

```c
TabulaPixelorum* fenestra_creare_tabulam_pixelorum(
    Piscina* piscina,
    Fenestra* fenestra,
    i32 altitudo_fixa
);
```

Creates a pixel buffer.

**Parameters:**
- `piscina` - Memory pool
- `fenestra` - Window (for aspect ratio calculation)
- `altitudo_fixa` - Buffer height in pixels

**Returns:** New pixel buffer, or `NIHIL` on error

**Notes:** Width is calculated from window aspect ratio.

### tabula_pixelorum_vacare

```c
vacuum tabula_pixelorum_vacare(
    TabulaPixelorum* tabula,
    i32 color
);
```

Fills the entire buffer with a color.

**Parameters:**
- `tabula` - Pixel buffer
- `color` - RGBA8888 color value

### tabula_pixelorum_ponere_pixelum

```c
vacuum tabula_pixelorum_ponere_pixelum(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    i32 color
);
```

Sets a single pixel.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - X coordinate
- `y` - Y coordinate
- `color` - RGBA8888 color value

**Notes:** Coordinates are bounds-checked. Out-of-bounds writes are ignored.

### tabula_pixelorum_obtinere_pixelum

```c
i32 tabula_pixelorum_obtinere_pixelum(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y
);
```

Gets a pixel's color.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - X coordinate
- `y` - Y coordinate

**Returns:** RGBA8888 color value, or 0 if out of bounds

### fenestra_praesentare_pixela

```c
vacuum fenestra_praesentare_pixela(
    Fenestra* fenestra,
    TabulaPixelorum* tabula
);
```

Presents the pixel buffer to the window.

**Parameters:**
- `fenestra` - Window
- `tabula` - Pixel buffer to display

**Notes:** Scales buffer to fit window if necessary. Uses nearest-neighbor interpolation.

## Text Rendering

### tabula_pixelorum_pingere_characterem

```c
vacuum tabula_pixelorum_pingere_characterem(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    character c,
    i32 color
);
```

Draws a single character.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - X position
- `y` - Y position
- `c` - Character to draw
- `color` - Text color

**Notes:** Uses built-in 6x8 bitmap font.

### tabula_pixelorum_pingere_chordam

```c
vacuum tabula_pixelorum_pingere_chordam(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    chorda textus,
    i32 color
);
```

Draws a string.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - Starting X position
- `y` - Starting Y position
- `textus` - String to draw
- `color` - Text color

**Notes:** Handles '\n' for newlines. Each character is 6 pixels wide, 8 pixels tall.

### tabula_pixelorum_pingere_characterem_scalatum

```c
vacuum tabula_pixelorum_pingere_characterem_scalatum(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    character c,
    i32 color,
    i32 scala
);
```

Draws a scaled character.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - X position
- `y` - Y position
- `c` - Character to draw
- `color` - Text color
- `scala` - Scale factor (1 = normal, 2 = double size, etc.)

### tabula_pixelorum_pingere_chordam_scalatam

```c
vacuum tabula_pixelorum_pingere_chordam_scalatam(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    chorda textus,
    i32 color,
    i32 scala
);
```

Draws a scaled string.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - Starting X position
- `y` - Starting Y position
- `textus` - String to draw
- `color` - Text color
- `scala` - Scale factor

### tabula_pixelorum_pingere_chordam_praecisum

```c
vacuum tabula_pixelorum_pingere_chordam_praecisum(
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    chorda textus,
    i32 color,
    i32 praecisio_x,
    i32 praecisio_y,
    i32 praecisio_latitudo,
    i32 praecisio_altitudo
);
```

Draws a clipped string.

**Parameters:**
- `tabula` - Pixel buffer
- `x` - Starting X position
- `y` - Starting Y position
- `textus` - String to draw
- `color` - Text color
- `praecisio_x` - Clip rectangle X
- `praecisio_y` - Clip rectangle Y
- `praecisio_latitudo` - Clip rectangle width
- `praecisio_altitudo` - Clip rectangle height

**Notes:** Only pixels inside the clip rectangle are drawn.

### fons_latitudo_chordae

```c
i32 fons_latitudo_chordae(chorda textus, i32 scala);
```

Gets the width of a string in pixels.

**Parameters:**
- `textus` - String to measure
- `scala` - Scale factor

**Returns:** Width in pixels

**Notes:** Accounts for newlines - returns the width of the widest line.

### fons_altitudo_chordae

```c
i32 fons_altitudo_chordae(i32 scala);
```

Gets the height of text in pixels.

**Parameters:**
- `scala` - Scale factor

**Returns:** Height in pixels (always `8 × scala`)

## Timing Functions

### fenestra_tempus_obtinere_pulsus

```c
i64 fenestra_tempus_obtinere_pulsus(vacuum);
```

Gets high-precision tick count.

**Returns:** Tick count from arbitrary starting point

**Notes:** Use with `fenestra_tempus_obtinere_frequentiam()` to convert to seconds.

### fenestra_tempus_obtinere_frequentiam

```c
f64 fenestra_tempus_obtinere_frequentiam(vacuum);
```

Gets timer frequency in ticks per second.

**Returns:** Frequency as a double

### fenestra_dormire

```c
vacuum fenestra_dormire(i32 microsecundae);
```

Sleeps for the specified duration.

**Parameters:**
- `microsecundae` - Sleep duration in microseconds

**Notes:** Sleep duration is approximate, not exact.

---

# Frequently Asked Questions

## Why Latin?

Why not? English technical terms are arbitrary too. At least Latin is stable - it's been dead for 1500 years, so the vocabulary isn't going to change. Plus it forces you to think about what you're actually doing instead of relying on muscle memory.

## Why Software Rendering?

Because hardware rendering is complex. Setting up an OpenGL context requires understanding pixel formats, color spaces, double buffering, vsync, and a dozen other concepts. Vulkan is even worse.

With software rendering, you write to a buffer. That's the entire concept. You can understand it in 5 minutes.

Yes, it's slower. But modern CPUs are fast. A 640x480 buffer is only 307,200 pixels. You can fill that millions of times per second.

## Why RGBA8888?

Because it's the simplest format that supports transparency. Each pixel is 4 bytes: red, green, blue, alpha. Each channel is 0-255. The format is widely supported by graphics APIs.

Other formats exist (RGB565, RGBA4444, etc.) but they save minimal memory and add complexity. 32 bits per pixel is a good default.

## Why a 6x8 Font?

Because it's small and fast. The entire font is 2KB. No dependencies on FreeType or stb_truetype. No font file parsing. Just a byte array.

The font is monospace - every character is exactly 6x8 pixels. This makes text layout trivial. No kerning, no ligatures, no complexity.

If you need prettier fonts, render text to a bitmap offline and load that. But for debug text and UIs, 6x8 is fine.

## Why a Ring Buffer for Events?

Because it's simple and fast. Push events at the tail, pop at the head. When the buffer wraps around, it uses modulo arithmetic to stay within bounds.

The buffer is 256 events deep. At 60 FPS with 20 events per frame, that's enough for 12 frames worth of events. If you're generating more events than that, you're doing something wrong.

## Why Fixed Height for Pixel Buffers?

Because keeping aspect ratio consistent is important for pixel-perfect rendering. If you specify width and height independently, resizing the window breaks your carefully chosen pixel dimensions.

With fixed height, the buffer always has the same vertical resolution. Width scales with aspect ratio. Your game logic can assume a constant vertical resolution and adapt to varying horizontal resolution.

If you really want fixed width instead, modify the creation function. It's 10 lines of code.

## Why Piscina (Memory Pools)?

Because `malloc()` and `free()` are error-prone. You forget to free - memory leak. You free twice - crash. You free and then use - crash.

With pools, you allocate everything from the pool and destroy the pool at the end. No leaks, no double frees, no use-after-free. Plus allocation is faster (bump allocator vs. searching free lists).

The downside is you can't free individual allocations. If you allocate 1MB for something temporary, that memory is used until you destroy the pool. For most programs this doesn't matter.

## What's the Performance?

On a 2019 MacBook Pro, I can run a 640x480 window at 60 FPS while filling the entire buffer with random pixels every frame. That's about 18 million pixels per second.

Text rendering is faster than pixel manipulation because it only draws non-transparent pixels. You can easily render thousands of characters per frame.

The bottleneck is usually your drawing code, not the library. If you're drawing pixel-by-pixel with `tabula_pixelorum_ponere_pixelum()`, you're calling a function 307,200 times per frame. That's slow. Use `tabula_pixelorum_vacare()` for fills and memcpy for blits.

## Can I Use This With OpenGL?

Not directly. This library gives you a pixel buffer, not a graphics context. If you want OpenGL, use a different library (GLFW, SDL).

That said, you could render to the pixel buffer with OpenGL by creating an offscreen framebuffer, but at that point why are you using this library?

## Can I Create Multiple Windows?

Yes. Call `fenestra_creare()` multiple times with the same or different pools. Each window has its own event queue.

Poll events for each window separately. If you have 3 windows, call `fenestra_perscrutari_eventus()` 3 times per frame.

## Can I Change the Font?

The font is baked into `fons_6x8.h` as a byte array. If you want a different font, you'll need to:

1. Create a new font array (256 entries, one per ASCII character)
2. Each character is 8 bytes (8 rows, 1 byte per row)
3. Each bit represents a pixel (1 = foreground, 0 = transparent)
4. Replace the array in `fons_6x8.h`

Or just don't use the built-in text rendering and implement your own.

## What About Unicode?

The font only supports ASCII (0-127). If you need Unicode, you'll need a different text rendering solution. FreeType, stb_truetype, or pre-rendered texture atlases are your options.

For most debug UIs and early development, ASCII is enough.

## Why No Audio?

Because this is a window library, not a game engine. Audio is a separate concern. Use miniaudio or PortAudio if you need sound.

## Why No File I/O?

Because standard C file I/O works fine. Use `fopen()`, `fread()`, `fwrite()`. This library doesn't need to wrap it.

## Why macOS Only?

Because writing cross-platform windowing code is a nightmare. Each platform has its own API (Win32, X11/Wayland, Cocoa) with different semantics and quirks.

Abstracting over these differences adds complexity. I'd rather have one solid implementation than three half-working ones.

If you need Windows or Linux support, you'll need to write `fenestra_win32.c` or `fenestra_x11.c`. The header is platform-agnostic, so the API doesn't change.

## Can I Port This to Other Platforms?

Yes. The API in `fenestra.h` is platform-independent. Only `fenestra_macos.m` is macOS-specific. To port:

1. Implement all functions in `fenestra.h` for your platform
2. Use the same event types and structures
3. Test thoroughly

The text rendering (`fenestra_textus.c`) is already portable - it just manipulates the pixel buffer.

---

Alright, that's everything. You've got windows, events, pixels, and text. Go make something. And if you have questions, read the header file first. The answer is probably there.
