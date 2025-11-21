# TEMPUS: Time and Animation Library Documentation

Oh great, another person who doesn't understand delta time. Look, I'm going to explain this once. Time in games and animation is not straightforward. You can't just assume every frame takes 16ms. You need delta time, you need frame limiting, you need easing curves, you need interpolation. That's what TEMPUS does.

**tempus** (noun, neuter, third declension) = time. Genitive is **temporis**. You'll see this all over.

## What This Library Does

TEMPUS handles everything related to time and animation:

1. **Frame timing** - Delta time, total time, FPS tracking
2. **Frame rate limiting** - Sleep intelligently to hit target FPS
3. **Easing functions** - 24 curves for smooth animation (ease-in, ease-out, ease-in-out)
4. **Animation helpers** - Lerp toward target, spring physics
5. **Interpolation** - Linear, angle, color, smoothstep
6. **Wave generators** - Sine, triangle, sawtooth, square, noise
7. **Utilities** - Clamp, wrap, ping-pong, remap

This is basically everything you need for time-based animation and game loops. It builds on FENESTRA's platform timing functions but adds the high-level stuff you actually use.

## Global Clock

The library has a global clock that tracks frame timing automatically. Most programs use this.

### Initializing

```c
#include "tempus.h"

tempus_initiare();
```

**initiare** = to begin/start. This sets up the global clock. You call it once at program start.

Actually, you don't have to call it. The first time you use any time function, it auto-initializes. But explicit initialization is clearer.

### Getting Current Time

```c
f64 secundae = tempus_nunc();
```

**nunc** = now. Returns seconds since program start. High precision - usually nanosecond accuracy on macOS.

Use this for measuring elapsed time:

```c
f64 initium = tempus_nunc();
/* Do work */
f64 elapsum = tempus_nunc() - initium;
imprimere("Took %f seconds\n", elapsum);
```

**initium** = beginning. **elapsum** = elapsed.

### Getting Delta Time

```c
f64 delta = tempus_delta();
```

**delta** is Greek for "change" but it's used universally in game programming. Returns the time (in seconds) since the last frame.

Delta time is how you make animations frame-rate independent:

```c
/* Move 100 pixels per second, regardless of FPS */
x += 100.0f * delta;
```

If your game runs at 60 FPS, delta is ~0.0167 seconds. If it drops to 30 FPS, delta is ~0.033 seconds. The object moves the same distance per second either way.

### Updating the Clock

```c
dum (!fenestra_debet_claudere(fenestra))
{
    tempus_quadrum();  /* Update clock */

    f64 delta = tempus_delta();
    /* Update game state using delta */

    /* Render */

    tempus_exspectare_quadrum();  /* Wait to hit target FPS */
}
```

**quadrum** = frame (literally "square" or "picture"). **exspectare** = to wait for.

`tempus_quadrum()` updates the global clock - calculates delta time, updates total time, tracks frame statistics. Call it once per frame, usually at the top of your main loop.

`tempus_exspectare_quadrum()` sleeps until it's time for the next frame. This caps your frame rate at the target FPS (default 60).

## Frame Rate Management

### Setting Target FPS

```c
tempus_ponere_quadra_destinata(60.0);
```

**quadra** = frames (plural of quadrum). **destinata** = intended/targeted.

Sets target frame rate to 60 FPS. The library will sleep to hit this target.

### Smart Frame Limiting

`tempus_exspectare_quadrum()` uses hybrid sleep/busy-wait:

1. If more than 2ms until next frame, sleep for most of it
2. For the last ~2ms, busy-wait for precision

Why? Because sleep is inaccurate. If you ask for 16ms, you might get 17ms or 18ms. Busy-waiting the last bit gives you precise frame timing.

The downside is busy-waiting wastes CPU. But we're only doing it for 2ms per frame, which is acceptable.

```c
tempus_ponere_quadra_destinata(60.0);

dum (!fenestra_debet_claudere(fenestra))
{
    tempus_quadrum();

    /* Your game logic */

    tempus_exspectare_quadrum();  /* Sleep until next frame */
}
```

This gives you rock-solid 60 FPS (assuming your frame logic takes less than 16.67ms).

### Getting Frame Statistics

```c
StatisticaeQuadri* stats = tempus_obtinere_statisticas();

imprimere("Target FPS: %f\n", stats->quadra_destinata_per_secundum);
imprimere("Current FPS: %f\n", stats->quadra_currentia_per_secundum);
imprimere("Average FPS: %f\n", stats->quadra_media_per_secundum);
imprimere("Min frame time: %f ms\n", stats->tempus_minimum_quadri * 1000.0);
imprimere("Max frame time: %f ms\n", stats->tempus_maximum_quadri * 1000.0);
imprimere("Total frames: %d\n", stats->numerus_quadrorum);
```

**statisticae** = statistics. **currentia** = current. **media** = average. **minimum** = minimum. **maximum** = maximum. **numerus** = number.

The statistics track:
- Target FPS (what you set)
- Current FPS (1 / last frame time)
- Average FPS over last 120 frames
- Min and max frame times seen
- Total frame count since program start
- Rolling buffer of last 120 frame times

Use this for performance monitoring, debug displays, etc.

## Independent Timers

Sometimes you need separate timers - stopwatches, cooldown timers, animations with different lifecycles.

### Creating a Timer

```c
Tempus* chronometer = tempus_creare(piscina);
```

**chronometer** is Greek/Latin for "time measurer". Creates an independent timer allocated from the pool.

### Using Independent Timers

```c
Tempus* timer = tempus_creare(piscina);

/* Later... */
f64 elapsum = tempus_elapsum(timer);
si (elapsum > 5.0)
{
    imprimere("5 seconds elapsed!\n");
    tempus_restituere(timer);  /* Reset timer */
}
```

**restituere** = to restore/reset. Resets the timer to zero.

Independent timers don't track delta time or frame statistics - they just measure elapsed time from when they were created (or last reset).

### Updating Independent Timers

```c
Tempus* timer = tempus_creare(piscina);

dum (VERUM)
{
    tempus_actualizare(timer);
    f64 delta = timer->delta_tempus;
    /* Use delta */
}
```

**actualizare** = to update/make current.

If you need delta time from an independent timer, call `tempus_actualizare()` each frame. The timer's `delta_tempus` field will have the time since last update.

Most of the time you just use `tempus_elapsum()` and don't bother with updates.

## Easing Functions

Easing functions make animations smooth. Instead of linear motion (constant speed), you get acceleration and deceleration.

All easing functions take `t` from 0.0 to 1.0 and return a value from 0.0 to 1.0 (usually - some overshoot).

### Linear (No Easing)

```c
f32 valor = lenire_linearis(t);
```

**lenire** = to soothe/ease/soften. **linearis** = linear.

This is just `return t`. No easing. Constant speed. Boring but sometimes what you want.

### Quadratic (Quadratic Curves)

```c
f32 ease_in = lenire_quadraticus_ingressus(t);      /* t² */
f32 ease_out = lenire_quadraticus_egressus(t);      /* 2t - t² */
f32 ease_in_out = lenire_quadraticus_ingressus_egressus(t);
```

**quadraticus** = quadratic (squared). **ingressus** = entry/in. **egressus** = exit/out.

Quadratic easing uses t².

- **Ease-in**: Starts slow, accelerates. Good for things starting to move.
- **Ease-out**: Starts fast, decelerates. Good for things coming to a stop.
- **Ease-in-out**: Starts slow, speeds up, slows down. Good for complete animations.

### Cubic (Cubic Curves)

```c
f32 ease_in = lenire_cubicus_ingressus(t);      /* t³ */
f32 ease_out = lenire_cubicus_egressus(t);
f32 ease_in_out = lenire_cubicus_ingressus_egressus(t);
```

**cubicus** = cubic (cubed). Like quadratic but more pronounced - stronger acceleration/deceleration.

### Sine (Sinusoidal)

```c
f32 ease_in = lenire_sinus_ingressus(t);
f32 ease_out = lenire_sinus_egressus(t);
f32 ease_in_out = lenire_sinus_ingressus_egressus(t);
```

**sinus** = sine wave. Uses sine curve for smooth, natural-feeling easing. Gentler than cubic.

### Exponential

```c
f32 ease_in = lenire_exponens_ingressus(t);
f32 ease_out = lenire_exponens_egressus(t);
f32 ease_in_out = lenire_exponens_ingressus_egressus(t);
```

**exponens** = exponential. Uses powers of 2. Very dramatic acceleration/deceleration.

### Back (Overshoot)

```c
f32 ease_in = lenire_retro_ingressus(t);
f32 ease_out = lenire_retro_egressus(t);
f32 ease_in_out = lenire_retro_ingressus_egressus(t);
```

**retro** = back/backward. The curve goes slightly negative before reaching the target (ease-in) or slightly past the target before settling (ease-out).

Creates a "wind-up" or "overshoot" effect. Good for playful, cartoon-style animations.

### Elastic (Bouncy Spring)

```c
f32 ease_in = lenire_elasticus_ingressus(t);
f32 ease_out = lenire_elasticus_egressus(t);
f32 ease_in_out = lenire_elasticus_ingressus_egressus(t);
```

**elasticus** = elastic/springy. Oscillates around the target like a spring. Very bouncy.

Overshoots multiple times with decreasing amplitude. Good for exaggerated, energetic animations.

### Bounce

```c
f32 ease_in = lenire_resiliens_ingressus(t);
f32 ease_out = lenire_resiliens_egressus(t);
f32 ease_in_out = lenire_resiliens_ingressus_egressus(t);
```

**resiliens** = bouncing/rebounding. Simulates a ball bouncing. Multiple impacts with decreasing height.

The ease-out variant is most common - object falls and bounces to rest.

### Using Easing in Animation

```c
f32 initium = 100.0f;
f32 finis = 500.0f;
f32 duratio = 2.0f;  /* 2 seconds */

f32 tempus_animationis = 0.0f;

dum (tempus_animationis < duratio)
{
    f32 t = tempus_animationis / duratio;  /* 0.0 to 1.0 */
    f32 eased_t = lenire_cubicus_egressus(t);
    f32 x = initium + (finis - initium) * eased_t;

    /* Draw object at x */

    tempus_animationis += tempus_delta();
}
```

**duratio** = duration. This animates X from 100 to 500 over 2 seconds with cubic ease-out (starts fast, slows down).

The pattern:
1. Convert elapsed time to normalized t (0 to 1)
2. Apply easing function
3. Interpolate between start and end values

## Animation Helpers

### Animate Toward Target

```c
f32 animare_ad(f32 currens, f32 destinatum, f32 celeritas, f32 delta_tempus);
```

**currens** = current. **destinatum** = target/destination. **celeritas** = speed.

Moves `currens` toward `destinatum` at constant speed. Returns the new value.

```c
f32 x = 100.0f;
f32 x_destinatum = 500.0f;

dum (VERUM)
{
    x = animare_ad(x, x_destinatum, 200.0f, tempus_delta());
    /* x moves toward x_destinatum at 200 pixels/second */
}
```

When the distance is less than `celeritas * delta_tempus`, it snaps to the target. No overshooting.

This is basically "move at constant speed until you arrive." Simple but useful.

### Spring Animation

```c
f32 elastrum_ad(f32 currens, f32 destinatum, f32* velocitas,
                 f32 rigiditas, f32 damnum, f32 delta_tempus);
```

**elastrum** = spring. **velocitas** = velocity. **rigiditas** = stiffness/rigidity. **damnum** = damping/loss.

Simulates spring physics. Returns the new position.

```c
f32 x = 100.0f;
f32 x_destinatum = 500.0f;
f32 x_velocitas = 0.0f;

dum (VERUM)
{
    x = elastrum_ad(x, x_destinatum, &x_velocitas, 10.0f, 0.5f, tempus_delta());
    /* x springs toward x_destinatum */
}
```

Parameters:
- `rigiditas` - How stiff the spring is. Higher = faster oscillation. Try 5-20.
- `damnum` - How much energy is lost. Higher = less bouncy. Try 0.3-0.8.

The spring overshoots and oscillates before settling. Feels natural and responsive.

You need to track velocity yourself - pass a pointer to a variable that persists across frames.

## Interpolation

### Linear Interpolation

```c
f32 valor = interpolare(a, b, t);
```

**interpolare** = to interpolate/estimate between. Classic lerp: `a + (b - a) * t`.

When `t = 0`, returns `a`. When `t = 1`, returns `b`. When `t = 0.5`, returns the midpoint.

```c
f32 initium = 0.0f;
f32 finis = 100.0f;
f32 medio = interpolare(initium, finis, 0.5f);  /* 50.0 */
```

### Angle Interpolation

```c
f32 angulus = interpolare_angulum(a, b, t);
```

**angulum** = angle (accusative).

Like lerp but handles wrap-around correctly. Angles are in radians.

Regular lerp from 350° to 10° goes the long way (through 180°). Angle interpolation goes the short way (through 0°/360°).

```c
f32 angulus_a = 6.0f;     /* ~344° */
f32 angulus_b = 0.2f;     /* ~11° */
f32 angulus = interpolare_angulum(angulus_a, angulus_b, 0.5f);
/* Takes the short path, ~177° / ~6.1 radians */
```

Essential for rotating objects smoothly.

### Clamping

```c
f32 valor = cohibere(valor, minimum, maximum);
```

**cohibere** = to restrain/hold back.

Clamps `valor` between `minimum` and `maximum`.

```c
f32 vita = cohibere(vita + damnum, 0.0f, 100.0f);
```

**vita** = life/health. **damnum** = damage. This keeps health between 0 and 100.

### Smoothstep

```c
f32 t = gradus_lenis(limen_infimum, limen_supremum, x);
```

**gradus_lenis** = smooth step. **limen** = threshold. **infimum** = lowest. **supremum** = highest.

Maps `x` from `[limen_infimum, limen_supremum]` to `[0, 1]` with smooth interpolation.

```c
f32 t = gradus_lenis(100.0f, 200.0f, 150.0f);  /* 0.5, but smoothed */
```

Uses Hermite interpolation: `t² (3 - 2t)`. Smoother than linear but simpler than easing curves.

Good for fade effects, threshold-based transitions.

### Color Interpolation

```c
i32 color = interpolare_colorem(color_primus, color_secundus, t);
```

**colorem** = color (accusative). **primus** = first. **secundus** = second.

Interpolates between two RGBA8888 colors. Handles each channel independently.

```c
i32 ruber = RGB(CCLV, ZEPHYRUM, ZEPHYRUM);  /* Red */
i32 caeruleus = RGB(ZEPHYRUM, ZEPHYRUM, CCLV);  /* Blue */
i32 purpureus = interpolare_colorem(ruber, caeruleus, 0.5f);  /* Purple */
```

**ruber** = red. **caeruleus** = blue. **purpureus** = purple.

Use this for color transitions, fade effects, gradients.

## Wave Functions

Wave functions generate periodic values. Useful for bobbing, pulsing, oscillating animations.

### Sine Wave

```c
f32 valor = unda_sinus(t, frequentia, amplitudo);
```

**unda** = wave. **frequentia** = frequency. **amplitudo** = amplitude.

Standard sine wave: `amplitude * sin(2π * frequency * t)`.

```c
f32 y = 200.0f + unda_sinus(tempus_nunc(), 1.0f, 50.0f);
/* Oscillates between 150 and 250 at 1 Hz */
```

Smooth, natural oscillation.

### Triangle Wave

```c
f32 valor = unda_triangulus(t, frequentia, amplitudo);
```

**triangulus** = triangle. Linear ramps up and down. Sharper than sine.

### Sawtooth Wave

```c
f32 valor = unda_dens_serrae(t, frequentia, amplitudo);
```

**dens_serrae** = sawtooth (literally "tooth of a saw"). Linear ramp up, instant drop. Looks like sawblade teeth.

### Square Wave

```c
f32 valor = unda_quadratus(t, frequentia, amplitudo);
```

**quadratus** = square. Alternates between `+amplitude` and `-amplitude`. No transition - instant flips.

### Noise Wave

```c
f32 valor = unda_tumultus(t);
```

**tumultus** = noise/commotion. Returns pseudo-random value from -1 to +1 based on `t`.

Not actually a wave - more like deterministic noise. Same input always gives same output, but looks random.

Good for jitter, shake effects, random variation that needs to be repeatable.

## Utility Functions

### Wrap

```c
f32 valor = circumvolvere(valor, minimum, maximum);
```

**circumvolvere** = to wrap around (literally "to roll around").

Wraps `valor` into the range `[minimum, maximum)`. When you go past max, you wrap to min.

```c
f32 angulus = circumvolvere(angulus, 0.0f, 2.0f * M_PI);
```

Keeps angle in `[0, 2π)`. Useful for rotation.

### Ping-Pong (Oscillate)

```c
f32 valor = oscillare(t, longitudo);
```

**oscillare** = to swing/oscillate. **longitudo** = length.

Bounces back and forth between 0 and `longitudo`. Like a triangle wave but with specific range.

```c
f32 x = oscillare(tempus_nunc(), 100.0f);
/* Goes 0→100→0→100→0... */
```

Good for back-and-forth motion without using sine.

### Remap

```c
f32 valor = remappare(valor, ex_minimum, ex_maximum, ad_minimum, ad_maximum);
```

**remappare** = to remap. **ex** = from. **ad** = to.

Maps `valor` from range `[ex_minimum, ex_maximum]` to `[ad_minimum, ad_maximum]`.

```c
f32 pixel_x = remappare(mouse_x, 0.0f, 800.0f, -1.0f, 1.0f);
/* Maps screen coordinates [0, 800] to normalized coordinates [-1, 1] */
```

Essential for coordinate transformations, normalizing inputs, etc.

## A Complete Example

Here's a program that uses multiple TEMPUS features:

```c
#include "fenestra.h"
#include "delineare.h"
#include "tempus.h"
#include "piscina.h"

integer principale(vacuum)
{
    Piscina* piscina;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    ContextusDelineandi* ctx;
    FenestraConfiguratio config;

    f32 circulus_x;
    f32 circulus_x_destinatum;
    f32 circulus_x_velocitas;

    f32 quadratus_y;
    f32 tempus_animationis;

    piscina = piscina_generare_dynamicum("demo", M * II);

    config.titulus = "Time Demo";
    config.x = C;
    config.y = C;
    config.latitudo = DCCC;
    config.altitudo = DC;
    config.vexilla = FENESTRA_ORDINARIA;

    fenestra = fenestra_creare(piscina, &config);
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    ctx = delineare_creare_contextum(piscina, tabula);

    tempus_initiare();
    tempus_ponere_quadra_destinata(60.0);

    circulus_x = 100.0f;
    circulus_x_destinatum = 500.0f;
    circulus_x_velocitas = 0.0f;

    quadratus_y = 100.0f;
    tempus_animationis = 0.0f;

    dum (!fenestra_debet_claudere(fenestra))
    {
        f32 delta;
        f32 t;
        f32 eased_t;
        StatisticaeQuadri* stats;

        tempus_quadrum();
        fenestra_perscrutari_eventus(fenestra);

        delta = (f32)tempus_delta();

        /* Spring animation for circle */
        circulus_x = elastrum_ad(circulus_x, circulus_x_destinatum,
                                  &circulus_x_velocitas, 10.0f, 0.5f, delta);

        /* Eased animation for rectangle */
        tempus_animationis += delta;
        si (tempus_animationis > 2.0f)
        {
            tempus_animationis = 0.0f;
        }

        t = tempus_animationis / 2.0f;
        eased_t = lenire_cubicus_ingressus_egressus(t);
        quadratus_y = 100.0f + 300.0f * eased_t;

        /* Clear and draw */
        delineare_vacare(ctx, RGB(XX, XX, XX));

        /* Spring circle - changes color based on velocity */
        {
            i32 color;
            i32 intensitas;

            intensitas = (i32)cohibere((f32)fabs(circulus_x_velocitas) * 5.0f, 0.0f, 255.0f);
            color = RGB(CCLV, intensitas, intensitas);
            delineare_circulum_plenum(ctx, (i32)circulus_x, CC, XXX, color);
        }

        /* Eased rectangle */
        delineare_rectangulum_plenum(ctx, CD, (i32)quadratus_y, L, L,
                                       RGB(ZEPHYRUM, CCLV, ZEPHYRUM));

        /* Sine wave line */
        {
            i32 x;
            per (x = ZEPHYRUM; x < tabula->latitudo - I; x++)
            {
                f32 y1;
                f32 y2;
                y1 = 400.0f + unda_sinus((f32)x * 0.01f + (f32)tempus_nunc(), 1.0f, 50.0f);
                y2 = 400.0f + unda_sinus((f32)(x + I) * 0.01f + (f32)tempus_nunc(), 1.0f, 50.0f);
                delineare_lineam(ctx, x, (i32)y1, x + I, (i32)y2, RGB(CCLV, CCLV, ZEPHYRUM));
            }
        }

        /* FPS display */
        {
            chorda textus;
            character buffer[LXIV];

            stats = tempus_obtinere_statisticas();
            sprintf(buffer, "FPS: %.1f", stats->quadra_currentia_per_secundum);
            textus = chorda_ex_literis(buffer, piscina);
            tabula_pixelorum_pingere_chordam(tabula, X, X, textus, RGB(CCLV, CCLV, CCLV));
        }

        fenestra_praesentare_pixela(fenestra, tabula);
        tempus_exspectare_quadrum();
    }

    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

This demo shows:
- Spring physics (red circle)
- Eased animation (green rectangle)
- Sine wave (yellow line)
- FPS counter
- Frame rate limiting to 60 FPS

The spring circle moves toward the target with bouncy physics. The velocity affects its color brightness. The rectangle animates up and down with cubic ease-in-out. The sine wave scrolls horizontally.

---

# API Reference

## Global Clock

### tempus_initiare

```c
vacuum tempus_initiare(vacuum);
```

Initializes the global clock.

**Notes:** Called automatically on first use. Explicit call is optional but recommended.

### tempus_nunc

```c
f64 tempus_nunc(vacuum);
```

Gets current time in seconds since program start.

**Returns:** Seconds with high precision (typically nanosecond accuracy)

### tempus_delta

```c
f64 tempus_delta(vacuum);
```

Gets time elapsed since last frame.

**Returns:** Delta time in seconds

**Notes:** Only valid after calling `tempus_quadrum()` at least once.

### tempus_quadrum

```c
vacuum tempus_quadrum(vacuum);
```

Updates the global clock for a new frame.

**Notes:** Call once per frame at the start of your main loop. Updates delta time, total time, and frame statistics.

### tempus_obtinere_horologium

```c
Tempus* tempus_obtinere_horologium(vacuum);
```

Gets pointer to global clock structure.

**Returns:** Pointer to global `Tempus` struct

**Notes:** Allows direct access to clock fields. Use with caution.

## Frame Rate Management

### tempus_ponere_quadra_destinata

```c
vacuum tempus_ponere_quadra_destinata(f64 quadra_per_secundum);
```

Sets target frame rate.

**Parameters:**
- `quadra_per_secundum` - Target FPS (e.g., 60.0 for 60 FPS)

### tempus_exspectare_quadrum

```c
vacuum tempus_exspectare_quadrum(vacuum);
```

Waits until next frame should start.

**Notes:** Uses hybrid sleep/busy-wait for precise timing. Targets the FPS set by `tempus_ponere_quadra_destinata()`.

### tempus_obtinere_statisticas

```c
StatisticaeQuadri* tempus_obtinere_statisticas(vacuum);
```

Gets frame statistics.

**Returns:** Pointer to global frame statistics

**Notes:** Statistics include target FPS, current FPS, average FPS, min/max frame times, frame count.

## Independent Timers

### tempus_creare

```c
Tempus* tempus_creare(Piscina* piscina);
```

Creates an independent timer.

**Parameters:**
- `piscina` - Memory pool for allocation

**Returns:** New timer, or `NIHIL` on error

### tempus_restituere

```c
vacuum tempus_restituere(Tempus* tempus);
```

Resets a timer to zero.

**Parameters:**
- `tempus` - Timer to reset

### tempus_elapsum

```c
f64 tempus_elapsum(Tempus* tempus);
```

Gets elapsed time from a timer.

**Parameters:**
- `tempus` - Timer

**Returns:** Seconds since timer was created or reset

### tempus_actualizare

```c
vacuum tempus_actualizare(Tempus* tempus);
```

Updates a timer (calculates delta time).

**Parameters:**
- `tempus` - Timer to update

**Notes:** Updates the `delta_tempus` field. Only needed if you want delta time from an independent timer.

## Easing Functions

All easing functions take `t` from 0.0 to 1.0 and return an eased value.

### lenire_linearis

```c
f32 lenire_linearis(f32 t);
```

Linear easing (no easing).

### lenire_quadraticus_ingressus

```c
f32 lenire_quadraticus_ingressus(f32 t);
```

Quadratic ease-in (t²).

### lenire_quadraticus_egressus

```c
f32 lenire_quadraticus_egressus(f32 t);
```

Quadratic ease-out.

### lenire_quadraticus_ingressus_egressus

```c
f32 lenire_quadraticus_ingressus_egressus(f32 t);
```

Quadratic ease-in-out.

### lenire_cubicus_ingressus

```c
f32 lenire_cubicus_ingressus(f32 t);
```

Cubic ease-in (t³).

### lenire_cubicus_egressus

```c
f32 lenire_cubicus_egressus(f32 t);
```

Cubic ease-out.

### lenire_cubicus_ingressus_egressus

```c
f32 lenire_cubicus_ingressus_egressus(f32 t);
```

Cubic ease-in-out.

### lenire_sinus_ingressus

```c
f32 lenire_sinus_ingressus(f32 t);
```

Sine ease-in.

### lenire_sinus_egressus

```c
f32 lenire_sinus_egressus(f32 t);
```

Sine ease-out.

### lenire_sinus_ingressus_egressus

```c
f32 lenire_sinus_ingressus_egressus(f32 t);
```

Sine ease-in-out.

### lenire_exponens_ingressus

```c
f32 lenire_exponens_ingressus(f32 t);
```

Exponential ease-in (2^(10(t-1))).

### lenire_exponens_egressus

```c
f32 lenire_exponens_egressus(f32 t);
```

Exponential ease-out.

### lenire_exponens_ingressus_egressus

```c
f32 lenire_exponens_ingressus_egressus(f32 t);
```

Exponential ease-in-out.

### lenire_retro_ingressus

```c
f32 lenire_retro_ingressus(f32 t);
```

Back ease-in (overshoots backward before moving forward).

### lenire_retro_egressus

```c
f32 lenire_retro_egressus(f32 t);
```

Back ease-out (overshoots past target).

### lenire_retro_ingressus_egressus

```c
f32 lenire_retro_ingressus_egressus(f32 t);
```

Back ease-in-out.

### lenire_elasticus_ingressus

```c
f32 lenire_elasticus_ingressus(f32 t);
```

Elastic ease-in (oscillates like a spring).

### lenire_elasticus_egressus

```c
f32 lenire_elasticus_egressus(f32 t);
```

Elastic ease-out.

### lenire_elasticus_ingressus_egressus

```c
f32 lenire_elasticus_ingressus_egressus(f32 t);
```

Elastic ease-in-out.

### lenire_resiliens_ingressus

```c
f32 lenire_resiliens_ingressus(f32 t);
```

Bounce ease-in.

### lenire_resiliens_egressus

```c
f32 lenire_resiliens_egressus(f32 t);
```

Bounce ease-out (like a ball bouncing to rest).

### lenire_resiliens_ingressus_egressus

```c
f32 lenire_resiliens_ingressus_egressus(f32 t);
```

Bounce ease-in-out.

## Animation Helpers

### animare_ad

```c
f32 animare_ad(
    f32 currens,
    f32 destinatum,
    f32 celeritas,
    f32 delta_tempus
);
```

Animates toward target at constant speed.

**Parameters:**
- `currens` - Current value
- `destinatum` - Target value
- `celeritas` - Speed (units per second)
- `delta_tempus` - Delta time

**Returns:** New value

**Notes:** Snaps to target when close enough. No overshooting.

### elastrum_ad

```c
f32 elastrum_ad(
    f32 currens,
    f32 destinatum,
    f32* velocitas,
    f32 rigiditas,
    f32 damnum,
    f32 delta_tempus
);
```

Spring physics animation.

**Parameters:**
- `currens` - Current value
- `destinatum` - Target value
- `velocitas` - Pointer to velocity (updated by function)
- `rigiditas` - Spring stiffness (5-20 typical)
- `damnum` - Damping (0.3-0.8 typical)
- `delta_tempus` - Delta time

**Returns:** New value

**Notes:** Overshoots and oscillates like a spring. You must maintain `velocitas` across frames.

## Interpolation

### interpolare

```c
f32 interpolare(f32 a, f32 b, f32 t);
```

Linear interpolation.

**Parameters:**
- `a` - Start value
- `b` - End value
- `t` - Interpolation factor (0 to 1)

**Returns:** Interpolated value

### interpolare_angulum

```c
f32 interpolare_angulum(f32 a, f32 b, f32 t);
```

Angle interpolation (handles wrap-around).

**Parameters:**
- `a` - Start angle (radians)
- `b` - End angle (radians)
- `t` - Interpolation factor (0 to 1)

**Returns:** Interpolated angle

**Notes:** Takes shortest path around the circle.

### cohibere

```c
f32 cohibere(f32 valor, f32 minimum, f32 maximum);
```

Clamps value to range.

**Parameters:**
- `valor` - Value to clamp
- `minimum` - Minimum value
- `maximum` - Maximum value

**Returns:** Clamped value

### gradus_lenis

```c
f32 gradus_lenis(f32 limen_infimum, f32 limen_supremum, f32 x);
```

Smoothstep interpolation.

**Parameters:**
- `limen_infimum` - Lower threshold
- `limen_supremum` - Upper threshold
- `x` - Input value

**Returns:** Smoothed value from 0 to 1

**Notes:** Uses Hermite interpolation (3t² - 2t³).

### interpolare_colorem

```c
i32 interpolare_colorem(i32 color_primus, i32 color_secundus, f32 t);
```

Color interpolation.

**Parameters:**
- `color_primus` - Start color (RGBA8888)
- `color_secundus` - End color (RGBA8888)
- `t` - Interpolation factor (0 to 1)

**Returns:** Interpolated color (RGBA8888)

## Wave Functions

### unda_sinus

```c
f32 unda_sinus(f32 t, f32 frequentia, f32 amplitudo);
```

Sine wave.

**Parameters:**
- `t` - Time
- `frequentia` - Frequency (Hz)
- `amplitudo` - Amplitude

**Returns:** Wave value

### unda_triangulus

```c
f32 unda_triangulus(f32 t, f32 frequentia, f32 amplitudo);
```

Triangle wave.

**Parameters:**
- `t` - Time
- `frequentia` - Frequency (Hz)
- `amplitudo` - Amplitude

**Returns:** Wave value

### unda_dens_serrae

```c
f32 unda_dens_serrae(f32 t, f32 frequentia, f32 amplitudo);
```

Sawtooth wave.

**Parameters:**
- `t` - Time
- `frequentia` - Frequency (Hz)
- `amplitudo` - Amplitude

**Returns:** Wave value

### unda_quadratus

```c
f32 unda_quadratus(f32 t, f32 frequentia, f32 amplitudo);
```

Square wave.

**Parameters:**
- `t` - Time
- `frequentia` - Frequency (Hz)
- `amplitudo` - Amplitude

**Returns:** Wave value (either +amplitudo or -amplitudo)

### unda_tumultus

```c
f32 unda_tumultus(f32 t);
```

Noise wave.

**Parameters:**
- `t` - Time

**Returns:** Pseudo-random value from -1 to +1

**Notes:** Deterministic - same input always gives same output.

## Utilities

### circumvolvere

```c
f32 circumvolvere(f32 valor, f32 minimum, f32 maximum);
```

Wraps value to range.

**Parameters:**
- `valor` - Value to wrap
- `minimum` - Minimum (inclusive)
- `maximum` - Maximum (exclusive)

**Returns:** Wrapped value

### oscillare

```c
f32 oscillare(f32 t, f32 longitudo);
```

Ping-pong oscillation.

**Parameters:**
- `t` - Time
- `longitudo` - Length (0 to longitudo and back)

**Returns:** Oscillating value

### remappare

```c
f32 remappare(
    f32 valor,
    f32 ex_minimum,
    f32 ex_maximum,
    f32 ad_minimum,
    f32 ad_maximum
);
```

Remaps value from one range to another.

**Parameters:**
- `valor` - Input value
- `ex_minimum` - Input range minimum
- `ex_maximum` - Input range maximum
- `ad_minimum` - Output range minimum
- `ad_maximum` - Output range maximum

**Returns:** Remapped value

---

# Frequently Asked Questions

## Why 24 Easing Functions?

Because that's the standard set. You have 6 base curves (quad, cubic, sine, exponential, back, elastic, bounce) × 3 variants (in, out, in-out) = 18, plus linear and the missing bounce-in = 20. Plus a few extras.

Actually I miscounted, there are 24. Point is: these cover every common animation need. Pick the one that feels right.

## Which Easing Should I Use?

- **Ease-out** for most things - objects coming to rest, UI appearing
- **Ease-in-out** for complete motions - doors opening, objects moving across screen
- **Ease-in** rarely - maybe for things disappearing

Cubic is a good default. Sine is gentler. Exponential is dramatic. Back/elastic/bounce are for playful effects.

Try them and see what feels right. Easing is about feel, not rules.

## Why Global State?

Because most programs have one game loop and one frame timer. A global clock is simpler than passing a timer everywhere.

If you need multiple independent timers (stopwatches, cooldowns, etc.), use `tempus_creare()`. But for frame timing, global is fine.

## How Accurate Is Frame Limiting?

Very accurate. The hybrid sleep/busy-wait approach typically hits within 1-2% of target FPS.

Pure sleep would be within 5-10% because system sleep is imprecise. Pure busy-wait would be perfect but wastes CPU.

The compromise - sleep for most of it, busy-wait the last 2ms - gives you precision without burning CPU.

## Should I Use Spring Physics or Easing?

Depends:

**Spring physics** (`elastrum_ad`):
- Responds to target changes immediately
- Overshoots and oscillates
- Feels natural and bouncy
- Requires tracking velocity

**Easing** (lerp with easing curve):
- Fixed duration animation
- No overshooting (unless you use back/elastic easing)
- More predictable
- Simpler (no velocity tracking)

For UI that responds to user input - springs. For scripted animations - easing.

## What's the Difference Between Spring and Elastic Easing?

**Spring physics** is simulation - it calculates actual spring forces and velocity. The motion depends on stiffness, damping, and initial conditions. Responds dynamically to changes.

**Elastic easing** is just a curve that looks springy. It's a fixed function from 0 to 1 with oscillation baked in. It doesn't respond to anything - it just plays out.

Spring physics is better for interactive stuff. Elastic easing is fine for one-shot animations.

## Why Use Angle Interpolation?

Because regular lerp doesn't understand wrap-around. If you lerp from 350° to 10°, regular lerp goes through 180° (the long way). Angle interpolation takes the short way (through 0°/360°).

This matters for rotating objects. Without angle interpolation, objects sometimes spin the wrong direction.

## What Are Wave Functions Good For?

- **Sine**: Smooth bobbing, floating, pulsing. Natural organic motion.
- **Triangle**: Linear back-and-forth. Mechanical looking.
- **Sawtooth**: Scrolling textures, scanning effects.
- **Square**: On/off flashing, digital effects.
- **Noise**: Shake, jitter, randomness.

Waves are great for idle animations, background motion, anything cyclical.

## Why Is There No `tempus_destruere()`?

Because timers are allocated from pools. When you destroy the pool, all timers are freed at once.

This is the pool allocation philosophy. No per-object destructors, just destroy the whole pool.

## Can I Have Multiple Target Frame Rates?

No. There's one global target FPS. If you need different update rates, use a fixed timestep loop:

```c
f64 accumulator = 0.0;
f64 tempus_per_quadrum = 1.0 / 30.0;  /* 30 Hz physics */

dum (VERUM)
{
    tempus_quadrum();
    accumulator += tempus_delta();

    dum (accumulator >= tempus_per_quadrum)
    {
        /* Physics update at 30 Hz */
        accumulator -= tempus_per_quadrum;
    }

    /* Render at whatever FPS */
    tempus_exspectare_quadrum();
}
```

This decouples physics rate from render rate. Common pattern in game engines.

## Why Are Easing Functions f32 Not f64?

Because animations don't need double precision. 32-bit floats have 7 decimal digits of precision, which is way more than needed for screen positions, rotations, etc.

The timer functions use f64 for accuracy over long periods. Animation functions use f32 for speed and simplicity.

## What's Smoothstep?

Smoothstep is a special interpolation curve: `3t² - 2t³`. It's like ease-in-out but with zero derivatives at the endpoints.

This means velocity starts at zero, accelerates smoothly, then decelerates smoothly to zero. No discontinuities.

Useful for fading, threshold transitions, anything where you want smooth entry and exit.

## Can I Combine Easing Functions?

Sure. Chain them, mix them, do whatever:

```c
f32 t1 = lenire_cubicus_ingressus(t);
f32 t2 = lenire_sinus_egressus(t1);
f32 valor = interpolare(a, b, t2);
```

Or mix two animations:

```c
f32 anim1 = interpolare(0.0f, 100.0f, lenire_cubicus_egressus(t));
f32 anim2 = interpolare(0.0f, 50.0f, lenire_sinus_ingressus(t));
f32 combined = anim1 + anim2;
```

Easing functions are just curves. Do math with them.

---

Alright, that's time and animation. You've got frame timing, easing curves, spring physics, interpolation, waves, and a bunch of utilities. Go make smooth animations. And if your animations still look jerky, that's a you problem, not a TEMPUS problem.
