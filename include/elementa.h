/*
 * elementa.h - Immediate-mode GUI component library
 *
 * Pars Phase II Pinacothecae. Componenta GUI simplicia pro dialogis.
 * Omnes functiones reddunt structuram fructus cum statu interactionis.
 */

#ifndef ELEMENTA_H
#define ELEMENTA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fenestra.h"

/* ============================================================
 * Slider - Tractus horizontalis pro valoribus numeralibus
 * ============================================================ */

nomen structura {
    i32 valor;       /* Valor novus post interactionem */
    b32 mutatum;     /* VERUM si valor mutatus hoc frame */
    b32 hover;       /* Mus super slider */
    b32 dragging;    /* Nunc trahitur */
} FructusSlider;

FructusSlider
elementa_slider(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              valor,
    i32              valor_min,
    i32              valor_max,
    constans Eventus* eventus,
    f32              scala);

/* ============================================================
 * Bottone - Pulsus simplex cum label
 * ============================================================ */

nomen structura {
    b32 clicked;     /* VERUM si pulsatus hoc frame */
    b32 hover;       /* Mus super bottone */
} FructusBottone;

FructusBottone
elementa_bottone(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    chorda*          label,
    constans Eventus* eventus,
    f32              scala);

/* ============================================================
 * Capsa Optandi - Checkbox pro valoribus boolean
 * ============================================================ */

nomen structura {
    b32 valor;       /* Status novus post toggle */
    b32 mutatum;     /* VERUM si toggled hoc frame */
    b32 hover;       /* Mus super capsa */
} FructusCapsaOptandi;

FructusCapsaOptandi
elementa_capsa_optandi(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    chorda*          label,
    b32              valor,
    constans Eventus* eventus,
    f32              scala);

/* ============================================================
 * Campus Textus - Text input field
 * ============================================================ */

nomen structura {
    chorda* textus;  /* Textus novus (potest esse nova allocatio) */
    s32     cursor;  /* Positio cursor nova */
    b32     mutatum; /* VERUM si textus mutatus */
    b32     focused; /* Reflectit statum focus */
} FructusCampusTextus;

FructusCampusTextus
elementa_campus_textus(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    i32              latitudo,
    chorda*          textus,
    s32              cursor,
    b32              focused,
    constans Eventus* eventus,
    f32              scala);

/* ============================================================
 * Graticula Colorum - 2x8 grid pro selectione colorum Aquinas
 * ============================================================ */

nomen structura {
    b32 colores[16]; /* Status toggle novus pro omnibus coloribus */
    b32 mutatum;     /* VERUM si aliquis color toggled */
    s32 toggled_index; /* Qui color toggled, -1 si nullus */
} FructusGraticulaColorum;

FructusGraticulaColorum
elementa_graticula_colorum(
    TabulaPixelorum* tabula,
    Piscina*         piscina,
    i32              x,
    i32              y,
    constans b32*    colores,
    constans Eventus* eventus,
    f32              scala);

#endif /* ELEMENTA_H */
