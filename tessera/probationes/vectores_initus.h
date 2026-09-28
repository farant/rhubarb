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
 * debita sine causa frangat. Nulla hodie. */

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

#endif /* VECTORES_INITUS_H */
