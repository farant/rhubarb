/* vectores_initus.h - Vectores initus: octeti intro, eventa foras
 *
 * DATA SOLA (nulla logica): tabulae hic_manens constans quas
 * probatio_tessera_vectores.c per pontem frustorum currit, quaeque in
 * formis quattuor (integra, bipartita, singula, sequens). Data C89
 * plana - olim ad suitam lexematoris (terminal-planning modules/002)
 * sine mutatione migrare possunt.
 *
 * PROVENIENTIA: sectiones portatae ex OpenTUI (MIT License,
 * Copyright (c) 2025 opentui), github.com/sst/opentui @ 7581976f,
 * packages/core/src/lib/{parse.keypress,parse.mouse,stdin-parser}
 * .test.ts - fons cuiusque sectionis in capite eius nominatur.
 * Sectio SEMEN tesserae propria est (probatio_tessera_eventum.c
 * iterum expressa).
 *
 * DEBITUM PER FORMAM: formae_debitae = mascula formarum quae nondum
 * congruunt (VALET = nulla). Forma debita "debitum manet" imprimit;
 * forma debita quae congruere incipit FRANGIT ("debitum solutum -
 * promove") - debita per NOMEN tolerantur, numquam tacite.
 */

#ifndef VECTORES_INITUS_H
#define VECTORES_INITUS_H

#include "latina.h"
#include "tessera_eventum.h"

/* Eventum exspectatum - campi idem ac TesseraEventum (genera
 * AMPLITUDO/RESUMPTUM hic non adveniunt) */
nomen structura {
    TesseraEventumGenus genus;
          TesseraClavis clavis;
                    s32 runa;
                    i32 modificatores;
                    i32 numerus;
        TesseraMusGenus mus_genus;
                    s32 mus_x;
                    s32 mus_y;
                    i32 mus_pulsus;
} EventumExspectatum;

#define VECTOR_EVENTA_MAXIMA VIII

/* Formae (mascula) */
#define FORMA_INTEGRA    0x1   /* scriptum totum una lectione */
#define FORMA_BIPARTITA  0x2   /* omnis scissio in duo, intra moram */
#define FORMA_SINGULA    0x4   /* octetus quisque sua lectione, intra moram */
#define FORMA_SEQUENS    0x8   /* + 'a' post: nihil devoratur */

#define VECTOR_VALET     ZEPHYRUM
#define VECTOR_DEBITUM   (FORMA_INTEGRA | FORMA_BIPARTITA | FORMA_SINGULA | FORMA_SEQUENS)

nomen structura {
    constans character* titulus;
    constans character* octeti;
                   i32  mensura;
                   b32  mora_terminalis;  /* sensus moram in fine poscit (ESC solum) */
                   i32  formae_debitae;
    constans character* causa;            /* NIHIL si VALET */
    EventumExspectatum  eventa[VECTOR_EVENTA_MAXIMA]; /* genus NIHIL = finis */
} VectorInitus;

/* Causae debitorum: macro nominata per causam (CAUSA_*), ut forma
 * debita sine causa frangat. */
#define CAUSA_H2 "H2 ad 002 lexemator: ESC ] (OSC) non comprehenditur - alt+']' + corpus ut runae phantasma (+ alt+'\\\\' aut ctrl+g ad terminum)"
#define CAUSA_FUGA_PENDENS "H6: ESC pendens (ultimus octetus lectionis) primo moram VOCANTIS (mora_ms) exspectat, deinde moram fugae - clavis intra mora_ms + ~25ms adveniens in alt+clavem confunditur (saltuarius: CCL ms)"
#define CAUSA_FUGAE_PRAEFIXUM "ad 002 lexemator: ESC ante seriem (ESC ESC [ A) fuga spuria + clavis sine alterum emittit; exspectatum (consilium Frani confirmandum) = clavis + ALTERUM, ut ESC + clavis simplex"

/* Octeti cum mensura explicita (NUL licet intra) */
#define OCT(s) (s), (i32)(magnitudo(s) - I)

/* Modificatores breves */
#define IMP TESSERA_MODIFICATOR_IMPERIUM
#define ALT TESSERA_MODIFICATOR_ALTERUM
#define MAI TESSERA_MODIFICATOR_MAIUSCULA

/* Eventa exspectata (ordo campi: genus, clavis, runa, modificatores,
 * numerus, mus_genus, mus_x, mus_y, mus_pulsus) */
#define EX_CLAVIS(c, mod) \
    { TESSERA_EVENTUM_CLAVIS, (c), ZEPHYRUM, (mod), ZEPHYRUM, \
      TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM }
#define EX_RUNA(r, mod) \
    { TESSERA_EVENTUM_CLAVIS, TESSERA_CLAVIS_NULLA, (r), (mod), ZEPHYRUM, \
      TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM }
#define EX_FUNCTIO(n, mod) \
    { TESSERA_EVENTUM_CLAVIS, TESSERA_CLAVIS_FUNCTIO, ZEPHYRUM, (mod), (n), \
      TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM }
#define EX_MUS(g, px, py, pulsus) \
    { TESSERA_EVENTUM_MUS, TESSERA_CLAVIS_NULLA, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, \
      (g), (px), (py), (pulsus) }
/* Nullum eventum exspectatum (series tacite consumpta) */
#define EX_NIHIL \
    { TESSERA_EVENTUM_NIHIL, TESSERA_CLAVIS_NULLA, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, \
      TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM }


/* ================================================================
 * SEMEN - casus probatio_tessera_eventum.c iterum expressi (tesserae
 * proprii, noti boni: harnesium probant). Historia: XXI formae SINGULA
 * debitae (H1, 938c0df4) - emendatione lectoris solutae et promotae
 * (tessera_eventum.worklog.md, 2026-09-28).
 * ================================================================ */

hic_manens constans VectorInitus VECTORES_SEMEN[] = {

    /* runae */
    { "a", OCT("a"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('a', ZEPHYRUM) } },
    { "e acutum (II octeti)", OCT("\xC3\xA9"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_RUNA(0xE9, ZEPHYRUM) } },
    { "linea horizontalis (III octeti)", OCT("\xE2\x94\x80"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_RUNA(0x2500, ZEPHYRUM) } },
    { "octetus invalidus abicitur", OCT("\xFF" "b"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_RUNA('b', ZEPHYRUM) } },

    /* regimen - damna documentata (tessera_eventum.h) */
    { "CR = reditus", OCT("\r"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_REDITUS, ZEPHYRUM) } },
    { "LF = reditus (damnum)", OCT("\n"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_REDITUS, ZEPHYRUM) } },
    { "HT = tabula", OCT("\t"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_TABULA, ZEPHYRUM) } },
    { "DEL = retrorsum", OCT("\x7F"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_RETRORSUM, ZEPHYRUM) } },
    { "BS = retrorsum (damnum)", OCT("\x08"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_RETRORSUM, ZEPHYRUM) } },
    { "ctrl+c", OCT("\x03"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('c', IMP) } },
    { "NUL = ctrl+spatium", OCT("\0"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(' ', IMP) } },

    /* CSI */
    { "CSI A sursum", OCT("\033[A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM) } },
    { "CSI D sinistra", OCT("\033[D"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SINISTRA, ZEPHYRUM) } },
    { "CSI H domus", OCT("\033[H"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DOMUS, ZEPHYRUM) } },
    { "CSI 3~ deletio", OCT("\033[3~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, ZEPHYRUM) } },
    { "CSI 5~ pagina sursum", OCT("\033[5~"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_PAGINA_SURSUM, ZEPHYRUM) } },
    { "CSI 2~ insertio", OCT("\033[2~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_INSERTIO, ZEPHYRUM) } },
    { "CSI 15~ F5", OCT("\033[15~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(V, ZEPHYRUM) } },
    { "CSI 17~ F6", OCT("\033[17~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(VI, ZEPHYRUM) } },
    { "CSI 24~ F12", OCT("\033[24~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(XII, ZEPHYRUM) } },
    { "CSI 1;5C imperium+dextra", OCT("\033[1;5C"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, IMP) } },
    { "CSI 3;2~ maiuscula+deletio", OCT("\033[3;2~"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, MAI) } },
    { "CSI Z tabula retro", OCT("\033[Z"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_TABULA, MAI) } },

    /* SS3 + ALTERUM */
    { "SS3 B deorsum", OCT("\033OB"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEORSUM, ZEPHYRUM) } },
    { "SS3 P F1", OCT("\033OP"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(I, ZEPHYRUM) } },
    { "alt+b", OCT("\033b"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('b', ALT) } },
    { "alt+reditus", OCT("\033\r"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_REDITUS, ALT) } },

    /* mus SGR (1-basata -> 0-basata) */
    { "mus pressus", OCT("\033[<0;5;3M"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, IV, II, ZEPHYRUM) } },
    { "mus solutus dexter", OCT("\033[<2;1;1m"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_SOLUTUS, ZEPHYRUM, ZEPHYRUM, II) } },
    { "rota sursum", OCT("\033[<64;2;2M"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_SURSUM, I, I, ZEPHYRUM) } },
    { "rota deorsum", OCT("\033[<65;2;2M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_DEORSUM, I, I, ZEPHYRUM) } },

    /* robustitas */
    { "CSI privata tacite consumpta", OCT("\033[?1049h" "x"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_RUNA('x', ZEPHYRUM) } },
    { "tria eventa uno scripto", OCT("ab\033[A"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_RUNA('a', ZEPHYRUM), EX_RUNA('b', ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM) } },
    { "ESC solum per moram = fuga", OCT("\033"), VERUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM) } }
};


/* ================================================================
 * CLAVES - ex OpenTUI packages/core/src/lib/parse.keypress.test.ts
 * (MIT, (c) 2025 opentui, @ 7581976f). Responsum TESSERAE asseritur;
 * ubi OpenTUI aliter dicit, commentarium "OpenTUI:" damnum documentat
 * (exemplar vetus et deperditum, tessera_eventum.h).
 *
 * PRAETERMISSA (causa nominata):
 *   - kitty (u-finalis) et modifyOtherKeys (27;m;c~) ut CLAVES: tessera
 *     ea numquam petit (features/008); hic solum ut strepitus tacitus
 *     probantur (tres vectores infra).
 *   - mus SGR/X10 ("filters out ... mouse"): T3.
 *   - series partiales et continuationes sine ESC: T4 (formae morae).
 *   - rxvt "\033[2$": '$' intermedium est (Williams) - finalem
 *     proximum, i.e. CLAVEM SEQUENTEM, devorat; exspectatum ex
 *     lexematore (002) pendet: T4.
 *   - "empty input", "Buffer input", nonAlphanumericKeys, KeyEventType,
 *     campus 'source': OpenTUI propria, sensus tesserae nullus.
 * ================================================================ */

hic_manens constans VectorInitus VECTORES_CLAVIUM[] = {

    /* litterae, numeri, signa */
    { "A (maiuscula invisibilis; OpenTUI: shift)", OCT("A"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_RUNA('A', ZEPHYRUM) } },
    { "1", OCT("1"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('1', ZEPHYRUM) } },
    { "!", OCT("!"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('!', ZEPHYRUM) } },
    { "@", OCT("@"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('@', ZEPHYRUM) } },
    { "spatium", OCT(" "), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(' ', ZEPHYRUM) } },

    /* claves speciales */
    { "alt+LF (OpenTUI: meta+linefeed)", OCT("\033\n"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_REDITUS, ALT) } },
    { "BS \\b", OCT("\b"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_RETRORSUM, ZEPHYRUM) } },
    { "BEL = ctrl+g", OCT("\x07"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('g', IMP) } },

    /* imperium + littera et octeti regiminis */
    { "ctrl+a", OCT("\x01"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('a', IMP) } },
    { "ctrl+z (sine ISIG)", OCT("\x1a"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('z', IMP) } },
    { "ctrl+\\", OCT("\x1c"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('\\', IMP) } },
    { "ctrl+]", OCT("\x1d"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(']', IMP) } },
    { "ctrl+^", OCT("\x1e"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('^', IMP) } },
    { "ctrl+_", OCT("\x1f"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('_', IMP) } },
    { "alt+ctrl+\\", OCT("\033\x1c"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('\\', IMP | ALT) } },
    { "alt+ctrl+]", OCT("\033\x1d"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(']', IMP | ALT) } },
    { "alt+ctrl+^", OCT("\033\x1e"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('^', IMP | ALT) } },
    { "alt+ctrl+_", OCT("\033\x1f"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('_', IMP | ALT) } },
    { "alt+ctrl+u", OCT("\033\x15"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('u', IMP | ALT) } },
    { "alt+ctrl+a", OCT("\033\x01"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('a', IMP | ALT) } },
    { "alt+ctrl+z", OCT("\033\x1a"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('z', IMP | ALT) } },

    /* spatium */
    { "alt+spatium", OCT("\033 "), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(' ', ALT) } },

    /* alt + littera */
    { "alt+a", OCT("\033a"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('a', ALT) } },
    { "alt+A (maiuscula in runa)", OCT("\033A"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_RUNA('A', ALT) } },
    { "alt+U", OCT("\033U"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('U', ALT) } },
    { "ESC F vetus (OpenTUI: meta+dextra)", OCT("\033F"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_RUNA('F', ALT) } },
    { "ESC B vetus (OpenTUI: meta+sinistra)", OCT("\033B"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_RUNA('B', ALT) } },
    { "alt+P", OCT("\033P"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('P', ALT) } },
    { "alt+N", OCT("\033N"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('N', ALT) } },
    { "alt+f", OCT("\033f"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('f', ALT) } },
    { "ESC ESC = fuga bis (OpenTUI: meta+escape)", OCT("\033\033"),
        VERUM,
      FORMA_SEQUENS, CAUSA_FUGA_PENDENS,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM) } },

    /* claves functionis */
    { "CSI 11~ F1", OCT("\033[11~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(I, ZEPHYRUM) } },
    { "CSI 11;5~ ctrl+F1", OCT("\033[11;5~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_FUNCTIO(I, IMP) } },
    { "CSI 11;3~ alt+F1", OCT("\033[11;3~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_FUNCTIO(I, ALT) } },
    { "CSI 11;9~ super+F1 (super perditur)", OCT("\033[11;9~"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_FUNCTIO(I, ZEPHYRUM) } },
    { "CSI 11;6~ shift+ctrl+F1", OCT("\033[11;6~"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_FUNCTIO(I, MAI | IMP) } },

    /* frecce et navigatio */
    { "CSI B deorsum", OCT("\033[B"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEORSUM, ZEPHYRUM) } },
    { "CSI C dextra", OCT("\033[C"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, ZEPHYRUM) } },
    { "CSI F finis", OCT("\033[F"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_FINIS, ZEPHYRUM) } },
    { "CSI 6~ pagina deorsum", OCT("\033[6~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_PAGINA_DEORSUM, ZEPHYRUM) } },
    { "SS3 A sursum", OCT("\033OA"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM) } },

    /* modificatores 1;m (bits m-1: maiuscula 1, alterum 2, imperium
     * 4, super 8 - super a tessera non tenetur) */
    { "1;2A shift+sursum", OCT("\033[1;2A"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI) } },
    { "1;3A alt+sursum", OCT("\033[1;3A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ALT) } },
    { "1;4A shift+alt+sursum", OCT("\033[1;4A"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI | ALT) } },
    { "1;5A ctrl+sursum", OCT("\033[1;5A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, IMP) } },
    { "1;7A ctrl+alt+sursum", OCT("\033[1;7A"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, IMP | ALT) } },
    { "1;8A shift+alt+ctrl+sursum", OCT("\033[1;8A"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI | ALT | IMP) } },
    { "1;9A super+sursum (super perditur)", OCT("\033[1;9A"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_SURSUM,
          ZEPHYRUM) } },
    { "1;10A shift+super", OCT("\033[1;10A"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI) } },
    { "1;11A alt+super", OCT("\033[1;11A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ALT) } },
    { "1;13A ctrl+super", OCT("\033[1;13A"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, IMP) } },
    { "1;16A omnes", OCT("\033[1;16A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI | ALT | IMP) } },
    { "1;3C alt+dextra", OCT("\033[1;3C"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, ALT) } },
    { "1;9C super+dextra (super perditur)", OCT("\033[1;9C"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA,
          ZEPHYRUM) } },
    { "1;11C alt+super+dextra", OCT("\033[1;11C"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, ALT) } },

    /* deletio cum modificatoribus */
    { "3;3~ alt+deletio", OCT("\033[3;3~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, ALT) } },
    { "3;5~ ctrl+deletio", OCT("\033[3;5~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, IMP) } },
    { "3;4~ shift+alt+deletio", OCT("\033[3;4~"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, MAI | ALT) } },
    { "3;7~ ctrl+alt+deletio", OCT("\033[3;7~"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_DELETIO, IMP | ALT) } },

    /* Unicode extra BMP */
    { "zhong (III octeti)", OCT("\xE4\xB8\xAD"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_RUNA(0x4E2D, ZEPHYRUM) } },
    { "pollex (IV octeti, extra BMP)", OCT("\xF0\x9F\x91\x8D"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_RUNA(0x1F44D, ZEPHYRUM) } },

    /* damna: formae quas tessera tacite consumit (OpenTUI claves
     * facit) */
    { "rxvt CSI a (OpenTUI: shift+sursum)", OCT("\033[a"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "rxvt CSI 2^ (OpenTUI: ctrl+insertio)", OCT("\033[2^"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "rxvt SS3 a (OpenTUI: ctrl+sursum)", OCT("\033Oa"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "SS3 p tabella numerica (OpenTUI: 0)", OCT("\033Op"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "SS3 M reditus tabellae (OpenTUI: reditus)", OCT("\033OM"),
        FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "octetus altus solus 0xA0 (OpenTUI: meta+spatium)", OCT("\xA0"),
      FALSUM, VECTOR_VALET, NIHIL, { EX_NIHIL } },

    /* strepitus: responsa terminalis, kitty, modifyOtherKeys, glutinum
     * - tacite consumpta, nulla clavis phantasma */
    { "responsum magnitudinis 4;...t", OCT("\033[4;1782;3012t"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "responsum cellularum 8;...t", OCT("\033[8;24;80t"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "CPR 10;25R", OCT("\033[10;25R"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "DA ?1;2c", OCT("\033[?1;2c"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "DA ?62;c", OCT("\033[?62;c"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "DA ?1;0;6;9;15c (parametra > IV)", OCT("\033[?1;0;6;9;15c"),
      FALSUM, VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "DECRPM ?1;2$y", OCT("\033[?1;2$y"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "focus intra CSI I", OCT("\033[I"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "focus extra CSI O", OCT("\033[O"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "glutinum initium 200~", OCT("\033[200~"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "glutinum finis 201~", OCT("\033[201~"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_NIHIL } },
    { "kitty 97;5u (ctrl+a)", OCT("\033[97;5u"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "modifyOtherKeys 27;2;13~ (shift+reditus)", OCT("\033[27;2;13~"),
      FALSUM, VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "modifyOtherKeys 27;5;127~ (ctrl+retrorsum)",
      OCT("\033[27;5;127~"), FALSUM, VECTOR_VALET, NIHIL,
          { EX_NIHIL } },

    /* debita */
    { "x + ESC in fine lectionis, mora, clavis", OCT("x\033"), VERUM,
      FORMA_SEQUENS, CAUSA_FUGA_PENDENS,
      { EX_RUNA('x', ZEPHYRUM), EX_CLAVIS(TESSERA_CLAVIS_FUGA,
          ZEPHYRUM) } },
    { "OSC 11 responsum ST", OCT("\033]11;rgb:0000/0000/0000\033\\"),
      FALSUM, VECTOR_DEBITUM, CAUSA_H2, { EX_NIHIL } },
    { "OSC 10 responsum BEL", OCT("\033]10;rgb:ffff/ffff/ffff\x07"),
      FALSUM, VECTOR_DEBITUM, CAUSA_H2, { EX_NIHIL } },
    { "ESC praefixum + CSI A (OpenTUI: meta+sursum)", OCT("\033\033[A"),
      FALSUM, VECTOR_DEBITUM, CAUSA_FUGAE_PRAEFIXUM,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ALT) } },
    { "ESC praefixum + CSI 1;5A", OCT("\033\033[1;5A"), FALSUM,
      VECTOR_DEBITUM, CAUSA_FUGAE_PRAEFIXUM,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, IMP | ALT) } }
};

#endif /* VECTORES_INITUS_H */
