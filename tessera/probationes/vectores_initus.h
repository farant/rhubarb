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
#define CAUSA_H2 "H2 ad 002 lexemator: ESC ] / ESC P / ESC _ (OSC, DCS, APC) non comprehenduntur - alt+']' (aut P, _) + corpus ut runae phantasma (+ alt+'\\\\' aut ctrl+g ad terminum)"
#define CAUSA_FUGAE_PRAEFIXUM "ad 002 lexemator: ESC ante seriem (ESC ESC [ A) fuga spuria + clavis sine alterum emittit; exspectatum (DECISUM Frani 2026-09-28) = clavis + ALTERUM, ut ESC + clavis simplex"
#define CAUSA_H7 "H7 ad 002: series PARTIALIS post moram ut fuga + runae phantasma redditur; exspectatum (consilium Frani): abicitur tacite, praeter ESC O / ESC [ = alt+O / alt+[ (xterm alt+O ipsum mittit)"
#define CAUSA_H8 "H8 ad 002: series muris trans moram scissa (ssh lentus) - pars prior fuga + phantasmata, continuatio phantasmata; exspectatum (OpenTUI): mus integer (pars servatur; continuatio sine ESC post fugam recuperatur)"
#define CAUSA_ALIENA "forma terminalis alieni (Linux console CSI [ A, putty [[5~, rxvt 2$) finalem falsum habet -> reliqua clavis phantasma; exspectatum: tacite consumpta, ut ceterae formae alienae (rxvt ^, SS3 minusculae)"

/* MORA in octetis vectoris: mora exacta HIC (pons frustorum signum
 * tollit et frustum vacuum inserit). 0xFE in UTF-8 numquam occurrit;
 * ut octetus probandus ergo NON adhibendum. */
#define MORA_SIGNUM 0xFE
#define MORA "\xFE"

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
/* Mus cum modificatoribus */
#define EX_MUS_MOD(g, px, py, pulsus, mod) \
    { TESSERA_EVENTUM_MUS, TESSERA_CLAVIS_NULLA, ZEPHYRUM, (mod), ZEPHYRUM, \
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
      VECTOR_VALET, NIHIL,
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
      VECTOR_VALET, NIHIL,
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


/* ================================================================
 * MURES - ex OpenTUI packages/core/src/lib/parse.mouse.test.ts (MIT,
 * (c) 2025 opentui, @ 7581976f). encodeSGR(b,x,y) = ESC [ < b ; x+1 ;
 * y+1 M/m; encodeBasic(b,x,y) = ESC [ M (b+32) (x+33) (y+33).
 *
 * HISTORIA: XXV debita (H3 X10, H4 motus, modificatores, rota soluta,
 * rota lateralis) emendatione T3b soluta et promota (2026-09-28).
 * TRACTUS (tessera 1.2 T3, 2026-09-28): tessera ?1002 petit; motus
 * (bit 32) cum bottone 0-2 = TESSERA_MUS_TRACTUS (finalis M/m neglecta,
 * ut OpenTUI); 35 (motus sine bottone, ?1003) et 96/97 (motus + rota)
 * tacite manent. Exspectata H4 ideo mutata, ut hic olim praedictum.
 *
 * PRAETERMISSA: status bottonum (mouseButtonsPressed, 'drag' vs
 * 'move' - OpenTUI propria, tessera statum non tenet); framing
 * "incomplete SGR" (T4, formae morae); "coordinates >= 95 under utf8
 * toString" (limes decodendi JS, non protocolli).
 * ================================================================ */

hic_manens constans VectorInitus VECTORES_MURIUM[] = {

    /* SGR - pressus et solutus */
    { "sgr sinister pressus", OCT("\033[<0;11;6M"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM) } },
    { "sgr sinister solutus", OCT("\033[<0;11;6m"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_SOLUTUS, X, V, ZEPHYRUM) } },
    { "sgr medius pressus", OCT("\033[<1;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, I) } },
    { "sgr dexter pressus", OCT("\033[<2;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, II) } },
    { "sgr dexter solutus", OCT("\033[<2;11;6m"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_SOLUTUS, X, V, II) } },

    /* SGR - rota */
    { "sgr rota sursum", OCT("\033[<64;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_ROTA_SURSUM, X, V, ZEPHYRUM) } },
    { "sgr rota deorsum", OCT("\033[<65;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_ROTA_DEORSUM, X, V, ZEPHYRUM) } },
    { "sgr rotae tres in frusto",
      OCT("\033[<64;83;68M\033[<64;83;68M\033[<65;83;68M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_SURSUM, LXXXII, LXVII, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_ROTA_SURSUM, LXXXII, LXVII, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_ROTA_DEORSUM, LXXXII, LXVII, ZEPHYRUM) } },

    /* SGR - coordinatae */
    { "sgr origo (1-basata)", OCT("\033[<0;1;1M"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM,
          ZEPHYRUM) } },
    { "sgr coordinatae magnae", OCT("\033[<0;501;301M"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, D, CCC,
          ZEPHYRUM) } },

    /* debita - modificatores muris */
    { "sgr shift+sinister", OCT("\033[<4;11;6M"), FALSUM,
        VECTOR_VALET,
      NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM, MAI) } },
    { "sgr alt+sinister", OCT("\033[<8;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM, ALT) } },
    { "sgr ctrl+sinister", OCT("\033[<16;11;6M"), FALSUM,
        VECTOR_VALET,
      NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM, IMP) } },
    { "sgr omnes modificatores", OCT("\033[<28;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM, MAI | ALT
          | IMP) } },
    { "sgr ctrl+dexter (bottone servato)", OCT("\033[<18;11;6M"),
        FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, X, V, II, IMP) } },
    { "sgr shift+rota", OCT("\033[<68;11;6M"), FALSUM, VECTOR_VALET,
      NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_ROTA_SURSUM, X, V, ZEPHYRUM, MAI) } },

    /* debita - rota soluta, rota lateralis */
    { "sgr rota soluta (m)", OCT("\033[<64;11;6m"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "sgr rota sinistrorsum 66", OCT("\033[<66;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_MUS(TESSERA_MUS_ROTA_SINISTRORSUM, X, V,
          ZEPHYRUM) } },
    { "sgr rota dextrorsum 67", OCT("\033[<67;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_MUS(TESSERA_MUS_ROTA_DEXTRORSUM, X, V,
          ZEPHYRUM) } },

    /* motus et tractus (H4; ?1002 T3) */
    { "sgr motus sine bottone 35", OCT("\033[<35;11;6m"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "sgr shift+motus 39 (tacitus)", OCT("\033[<39;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "sgr tractus sinister 32", OCT("\033[<32;13;6m"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, XII, V, ZEPHYRUM) } },
    { "sgr tractus medius 33", OCT("\033[<33;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, X, V, I) } },
    { "sgr tractus dexter 34", OCT("\033[<34;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, X, V, II) } },
    { "sgr ctrl+tractus 48", OCT("\033[<48;11;6M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_TRACTUS, X, V, ZEPHYRUM, IMP) } },
    { "sgr motus+rota 96", OCT("\033[<96;81;67M"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "sgr motus+rota 97", OCT("\033[<97;81;67M"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "sgr duo tractus in frusto",
      OCT("\033[<32;70;50M\033[<32;69;50M"), FALSUM, VECTOR_VALET,
          NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, LXIX, XLIX, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_TRACTUS, LXVIII, XLIX, ZEPHYRUM) } },
    { "sgr pressus + tractus + solutus",
      OCT("\033[<0;11;11M\033[<32;13;11M\033[<0;13;11m"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, X, X, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_TRACTUS, XII, X, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_SOLUTUS, XII, X, ZEPHYRUM) } },
    { "sgr ictus: pressus, tractus II, solutus",
      OCT("\033[<0;5;5M\033[<32;6;5M\033[<32;7;6M\033[<0;7;6m"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, IV, IV, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_TRACTUS, V, IV, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_TRACTUS, VI, V, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_SOLUTUS, VI, V, ZEPHYRUM) } },
    { "sgr rota + motus 96/97 in frusto",
      OCT("\033[<64;83;68M\033[<96;82;68M\033[<97;81;68M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_SURSUM, LXXXII, LXVII, ZEPHYRUM) } },

    /* debita - X10 (H3); exspectatum provisorium: ut SGR parsatum */
    { "x10 sinister pressus", OCT("\033[M +&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM) } },
    { "x10 medius pressus", OCT("\033[M!+&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, I) } },
    { "x10 dexter pressus", OCT("\033[M\"+&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_PRESSUS, X, V, II) } },
    { "x10 solutus (bottone ignoto)", OCT("\033[M#+&"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_SOLUTUS, X, V, III) } },
    { "x10 rota sursum", OCT("\033[M`+&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_ROTA_SURSUM, X, V, ZEPHYRUM) } },
    { "x10 rota deorsum", OCT("\033[Ma+&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_MUS(TESSERA_MUS_ROTA_DEORSUM, X, V, ZEPHYRUM) } },
    { "x10 origo", OCT("\033[M !!"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM) } },
    { "x10 coordinata 94 (octetus 0x7F)", OCT("\033[M \x7F\x7F"),
        FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, XCIV, XCIV, ZEPHYRUM) } },
    { "x10 duo in frusto", OCT("\033[M +&\033[M#+&"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, X, V, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_SOLUTUS, X, V, III) } }
};


/* ================================================================
 * FRUSTA - ex OpenTUI packages/core/src/lib/stdin-parser.test.ts (MIT,
 * (c) 2025 opentui, @ 7581976f): compago fluminis - morae, series
 * partiales, UTF-8, ESC intra seriem, responsa, fluxus mixti. Formae
 * quattuor "chunk-shape invariance" iam systematice probant; MORA =
 * mora in medio (formae scissuras circa eam faciunt).
 *
 * PRAETERMISSA (causa nominata):
 *   - glutinum (bracketed paste): exspectata genus GLUTINUM poscunt
 *     quod nondum est - cum features/001 portanda.
 *   - "protocol context" totum (explicitWidthCpr, pixelResolution,
 *     privateCapabilityReplies, startupCursorCpr): tessera numquam
 *     quaerit, ergo responsa talia non exspectat.
 *   - status obiecti OpenTUI (reset, destroy, read/drain, bufferCapacity,
 *     onTimeoutFlush, horologium dissentiens): API propria.
 *   - kitty/modifyOtherKeys ut CLAVES: tessera ea non petit (ut
 *     strepitus infra).
 * ================================================================ */

hic_manens constans VectorInitus VECTORES_FRUSTORUM[] = {

    /* regimen et signa - tabula plena */
    { "ctrl+b d e f g k l n", OCT("\x02\x04\x05\x06\x07\x0b\x0c\x0e"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('b', IMP), EX_RUNA('d', IMP), EX_RUNA('e', IMP),
        EX_RUNA('f', IMP), EX_RUNA('g', IMP), EX_RUNA('k', IMP),
        EX_RUNA('l', IMP), EX_RUNA('n', IMP) } },
    { "ctrl+o p q r s t v w", OCT("\x0f\x10\x11\x12\x13\x14\x16\x17"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('o', IMP), EX_RUNA('p', IMP), EX_RUNA('q', IMP),
        EX_RUNA('r', IMP), EX_RUNA('s', IMP), EX_RUNA('t', IMP),
        EX_RUNA('v', IMP), EX_RUNA('w', IMP) } },
    { "ctrl+u x y", OCT("\x15\x18\x19"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('u', IMP), EX_RUNA('x', IMP), EX_RUNA('y', IMP) } },
    { "litterae", OCT("abcdefgh"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('a', ZEPHYRUM), EX_RUNA('b', ZEPHYRUM),
        EX_RUNA('c', ZEPHYRUM), EX_RUNA('d', ZEPHYRUM),
        EX_RUNA('e', ZEPHYRUM), EX_RUNA('f', ZEPHYRUM),
        EX_RUNA('g', ZEPHYRUM), EX_RUNA('h', ZEPHYRUM) } },
    { "signa I", OCT("!@#$%^&*"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('!', ZEPHYRUM), EX_RUNA('@', ZEPHYRUM),
        EX_RUNA('#', ZEPHYRUM), EX_RUNA('$', ZEPHYRUM),
        EX_RUNA('%', ZEPHYRUM), EX_RUNA('^', ZEPHYRUM),
        EX_RUNA('&', ZEPHYRUM), EX_RUNA('*', ZEPHYRUM) } },
    { "signa II", OCT("()-_=+[]"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('(', ZEPHYRUM), EX_RUNA(')', ZEPHYRUM),
        EX_RUNA('-', ZEPHYRUM), EX_RUNA('_', ZEPHYRUM),
        EX_RUNA('=', ZEPHYRUM), EX_RUNA('+', ZEPHYRUM),
        EX_RUNA('[', ZEPHYRUM), EX_RUNA(']', ZEPHYRUM) } },
    { "signa III", OCT("{}|;':,."), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('{', ZEPHYRUM), EX_RUNA('}', ZEPHYRUM),
        EX_RUNA('|', ZEPHYRUM), EX_RUNA(';', ZEPHYRUM),
        EX_RUNA('\'', ZEPHYRUM), EX_RUNA(':', ZEPHYRUM),
        EX_RUNA(',', ZEPHYRUM), EX_RUNA('.', ZEPHYRUM) } },
    { "signa IV", OCT("/<>?`~"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('/', ZEPHYRUM), EX_RUNA('<', ZEPHYRUM),
        EX_RUNA('>', ZEPHYRUM), EX_RUNA('?', ZEPHYRUM),
        EX_RUNA('`', ZEPHYRUM), EX_RUNA('~', ZEPHYRUM) } },
    { "tres runae UTF-8", OCT("\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA(0x65E5, ZEPHYRUM), EX_RUNA(0x672C, ZEPHYRUM),
        EX_RUNA(0x8A9E, ZEPHYRUM) } },
    { "alt+DEL", OCT("\033\x7f"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_RETRORSUM, ALT) } },
    { "alt+BS", OCT("\033\b"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_RETRORSUM, ALT) } },
    { "alt+5", OCT("\0335"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('5', ALT) } },
    { "alt+! (OpenTUI: clavis sine nomine)", OCT("\033!"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_RUNA('!', ALT) } },

    /* navigatio et functiones */
    { "CSI 1~ domus, 4~ finis", OCT("\033[1~\033[4~"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DOMUS, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_FINIS, ZEPHYRUM) } },
    { "SS3 F2 F3 F4", OCT("\033OQ\033OR\033OS"), FALSUM, VECTOR_VALET,
      NIHIL,
      { EX_FUNCTIO(II, ZEPHYRUM), EX_FUNCTIO(III, ZEPHYRUM),
        EX_FUNCTIO(IV, ZEPHYRUM) } },
    { "SS3 C D H F", OCT("\033OC\033OD\033OH\033OF"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_SINISTRA, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_DOMUS, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_FINIS, ZEPHYRUM) } },
    { "F2-F4 F7-F11 (~)",
      OCT("\033[12~\033[13~\033[14~\033[18~\033[19~\033[20~\033[21~\033[23~"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_FUNCTIO(II, ZEPHYRUM), EX_FUNCTIO(III, ZEPHYRUM),
        EX_FUNCTIO(IV, ZEPHYRUM), EX_FUNCTIO(VII, ZEPHYRUM),
        EX_FUNCTIO(VIII, ZEPHYRUM), EX_FUNCTIO(IX, ZEPHYRUM),
        EX_FUNCTIO(X, ZEPHYRUM), EX_FUNCTIO(XI, ZEPHYRUM) } },
    { "shift+ctrl frecce 1;6",
        OCT("\033[1;6A\033[1;6B\033[1;6C\033[1;6D"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, MAI | IMP),
        EX_CLAVIS(TESSERA_CLAVIS_DEORSUM, MAI | IMP),
        EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, MAI | IMP),
        EX_CLAVIS(TESSERA_CLAVIS_SINISTRA, MAI | IMP) } },
    { "frecce modificatae mixtae",
        OCT("\033[1;2B\033[1;3C\033[1;4D\033[1;5B"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_DEORSUM, MAI),
        EX_CLAVIS(TESSERA_CLAVIS_DEXTRA, ALT),
        EX_CLAVIS(TESSERA_CLAVIS_SINISTRA, MAI | ALT),
        EX_CLAVIS(TESSERA_CLAVIS_DEORSUM, IMP) } },

    /* damna: formae alienae iam tacite consumptae */
    { "CSI E (OpenTUI: clear)", OCT("\033[E"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_NIHIL } },
    { "SS3 E (OpenTUI: clear)", OCT("\033OE"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_NIHIL } },
    { "rxvt 7~ 8~ (OpenTUI: domus, finis)", OCT("\033[7~\033[8~"),
        FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "rxvt shift b c d", OCT("\033[b\033[c\033[d"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "rxvt ctrl SS3 b c d", OCT("\033Ob\033Oc\033Od"), FALSUM,
      VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "rxvt ^ 3 5 6", OCT("\033[3^\033[5^\033[6^"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },

    /* strepitus tacitus */
    { "kitty formae ':' et u",
        OCT("\033[97;1:3u\033[1;1:1A\033[5;1:1~\033[27u"),
      FALSUM, VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "modifyOtherKeys 27;3;9 27;2;53",
        OCT("\033[27;3;9~\033[27;2;53~"),
      FALSUM, VECTOR_VALET, NIHIL, { EX_NIHIL } },
    { "CSI h ignota", OCT("\033[h"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "sgr malformatum <0M", OCT("\033[<0M"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_NIHIL } },
    { "DA1 longa", OCT("\033[?62;1;2;6;7;8;9;15;22c"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "DECRPM ?2004;1$y", OCT("\033[?2004;1$y"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "focus inter claves", OCT("a\033[Ib\033[Oc"), FALSUM,
        VECTOR_VALET,
      NIHIL,
      { EX_RUNA('a', ZEPHYRUM), EX_RUNA('b', ZEPHYRUM),
        EX_RUNA('c', ZEPHYRUM) } },

    /* mus in fluxu */
    { "sgr tres in frusto", OCT("\033[<0;1;1M\033[<0;2;1M\033[<0;2;1m"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_PRESSUS, I, ZEPHYRUM, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_SOLUTUS, I, ZEPHYRUM, ZEPHYRUM) } },
    { "sgr pressus + tractus",
      OCT("\033[<0;5;5M\033[<32;6;5M"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, IV, IV, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_TRACTUS, V, IV, ZEPHYRUM) } },
    { "x10 + clavis", OCT("\033[M !!x"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM),
        EX_RUNA('x', ZEPHYRUM) } },
    { "x10 motus 35 (tacitus)", OCT("\033[MC%&"), FALSUM, VECTOR_VALET,
      NIHIL, { EX_NIHIL } },
    { "x10 tractus sinister 10,5", OCT("\033[M@+&"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, X, V, ZEPHYRUM) } },
    { "x10 tractus dexter + clavis", OCT("\033[MB+&x"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_TRACTUS, X, V, II),
        EX_RUNA('x', ZEPHYRUM) } },
    { "x10 rota sursum 2,3", OCT("\033[M`#$"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_SURSUM, II, III, ZEPHYRUM) } },
    { "x10 shift+sinister", OCT("\033[M$!!"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                   MAI) } },
    { "x10 ctrl+rota 7,8", OCT("\033[Mp()"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS_MOD(TESSERA_MUS_ROTA_SURSUM, VII, VIII, ZEPHYRUM,
          IMP) } },
    { "x10 10,20", OCT("\033[M +5"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, X, XX, ZEPHYRUM) } },

    /* ESC intra seriem (CSI recte abortit) et morae */
    { "CSI abrupta ESC, deinde sgr integra",
        OCT("\033[<0;\033[<0;21;6M"),
      FALSUM, VECTOR_VALET, NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, XX, V, ZEPHYRUM) } },
    { "CSI 123 + ESC, mora = fuga", OCT("\033[123\033"), VERUM,
      VECTOR_VALET, NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_FUGA,
          ZEPHYRUM) } },
    { "fuga, mora, b (sine mora: alt+b)", OCT("\033" MORA "b"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM), EX_RUNA('b',
          ZEPHYRUM) } },
    { "tres fugae per moras", OCT("\033" MORA "\033" MORA "\033"),
        VERUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM) } },

    /* fluxus mixti - formae omnes scissiones probant */
    { "x + rota + focus + pollex",
      OCT("x\033[<64;10;5M\033[I\xF0\x9F\x91\x8D"), FALSUM,
          VECTOR_VALET,
      NIHIL,
      { EX_RUNA('x', ZEPHYRUM),
        EX_MUS(TESSERA_MUS_ROTA_SURSUM, IX, IV, ZEPHYRUM),
        EX_RUNA(0x1F44D, ZEPHYRUM) } },
    { "ascii + utf8", OCT("xy\xF0\x9F\x91\x8D"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_RUNA('x', ZEPHYRUM), EX_RUNA('y', ZEPHYRUM),
        EX_RUNA(0x1F44D, ZEPHYRUM) } },
    { "utf8 + arcus", OCT("\xF0\x9F\x91\x8D\033[A"), FALSUM,
        VECTOR_VALET,
      NIHIL,
      { EX_RUNA(0x1F44D, ZEPHYRUM),
        EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM) } },
    { "arcus + sgr", OCT("\033[A\033[<64;10;5M"), FALSUM, VECTOR_VALET,
      NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_ROTA_SURSUM, IX, IV, ZEPHYRUM) } },
    { "sgr + x10", OCT("\033[<64;10;5M\033[M !!"), FALSUM, VECTOR_VALET,
      NIHIL,
      { EX_MUS(TESSERA_MUS_ROTA_SURSUM, IX, IV, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM) } },
    { "x10 + kitty", OCT("\033[M !!\033[97u"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM) } },
    { "kitty + ascii", OCT("\033[97uxy"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('x', ZEPHYRUM), EX_RUNA('y', ZEPHYRUM) } },
    { "arcus + textus + mus", OCT("\033[Ax\033[<0;1;1M"), FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM), EX_RUNA('x',
          ZEPHYRUM),
        EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM) } },
    { "[< sine ESC = litterae", OCT("[<35;5m"), FALSUM, VECTOR_VALET,
        NIHIL,
      { EX_RUNA('[', ZEPHYRUM), EX_RUNA('<', ZEPHYRUM),
        EX_RUNA('3', ZEPHYRUM), EX_RUNA('5', ZEPHYRUM),
        EX_RUNA(';', ZEPHYRUM), EX_RUNA('5', ZEPHYRUM),
        EX_RUNA('m', ZEPHYRUM) } },

    /* UTF-8 invalidum (OpenTUI: meta 8-bit; tessera abicit) */
    { "0xC0 + A", OCT("\xC0" "A"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('A', ZEPHYRUM) } },
    { "0xE9 (III) + x", OCT("\xE9" "x"), FALSUM, VECTOR_VALET, NIHIL,
      { EX_RUNA('x', ZEPHYRUM) } },
    { "0xE9 solus + mora", OCT("\xE9"), VERUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },
    { "0xFF solus + mora", OCT("\xFF"), VERUM, VECTOR_VALET, NIHIL,
      { EX_NIHIL } },

    /* debita - H2 (OSC, DCS, APC) */
    { "OSC BEL", OCT("\033]4;0;#ffffff\x07"), FALSUM, VECTOR_DEBITUM,
      CAUSA_H2, { EX_NIHIL } },
    { "OSC ST", OCT("\033]4;0;rgb:ff/ff/ff\033\\"), FALSUM,
        VECTOR_DEBITUM,
      CAUSA_H2, { EX_NIHIL } },
    { "DCS XTVERSION", OCT("\033P>|kitty(0.40.1)\033\\"), FALSUM,
      VECTOR_DEBITUM, CAUSA_H2, { EX_NIHIL } },
    { "APC kitty", OCT("\033_Gi=1;OK\033\\"), FALSUM, VECTOR_DEBITUM,
      CAUSA_H2, { EX_NIHIL } },
    { "OSC + DCS + APC",
        OCT("\033]4;0;#fff\x07\033P>|test\033\\\033_OK\033\\"),
      FALSUM, VECTOR_DEBITUM, CAUSA_H2, { EX_NIHIL } },
    { "x + OSC + y", OCT("x\033]4;0;#fff\x07y"), FALSUM, VECTOR_DEBITUM,
      CAUSA_H2, { EX_RUNA('x', ZEPHYRUM), EX_RUNA('y', ZEPHYRUM) } },

    /* debita - H7 (series partialis post moram) */
    { "ESC O + mora = alt+O", OCT("\033O"), VERUM, VECTOR_DEBITUM,
        CAUSA_H7,
      { EX_RUNA('O', ALT) } },
    { "ESC [ + mora = alt+[", OCT("\033["), VERUM, VECTOR_DEBITUM,
        CAUSA_H7,
      { EX_RUNA('[', ALT) } },
    { "CSI 123 + mora", OCT("\033[123"), VERUM, VECTOR_DEBITUM,
        CAUSA_H7,
      { EX_NIHIL } },
    { "CSI 1;5 + mora + A", OCT("\033[1;5" MORA "A"), FALSUM,
      VECTOR_DEBITUM, CAUSA_H7, { EX_RUNA('A', ZEPHYRUM) } },
    { "CSI 24;80 + mora + R", OCT("\033[24;80" MORA "R"), FALSUM,
      VECTOR_DEBITUM, CAUSA_H7, { EX_RUNA('R', ZEPHYRUM) } },
    { "OSC partialis + mora", OCT("\033]incomplete"), VERUM,
        VECTOR_DEBITUM,
      CAUSA_H7, { EX_NIHIL } },
    { "DCS partialis + mora", OCT("\033Ppartial"), VERUM,
        VECTOR_DEBITUM,
      CAUSA_H7, { EX_NIHIL } },
    { "APC partialis + mora", OCT("\033_partial"), VERUM,
        VECTOR_DEBITUM,
      CAUSA_H7, { EX_NIHIL } },
    { "OSC 52 partialis + mora", OCT("\033]52;c;"), VERUM,
        VECTOR_DEBITUM,
      CAUSA_H7, { EX_NIHIL } },
    { "x10 partialis + mora", OCT("\033[M !"), VERUM, VECTOR_DEBITUM,
      CAUSA_H7, { EX_NIHIL } },
    { "CSI XL digitorum + mora",
        OCT("\033[1111111111111111111111111111111111111111"), VERUM,
      VECTOR_DEBITUM, CAUSA_H7, { EX_NIHIL } },

    /* debita - H8 (mus trans moram) */
    { "sgr scissa per moram", OCT("\033[<0;20" MORA ";5M"), FALSUM,
      VECTOR_DEBITUM, CAUSA_H8,
      { EX_MUS(TESSERA_MUS_PRESSUS, XIX, IV, ZEPHYRUM) } },
    { "fuga, mora, continuatio sgr", OCT("\033" MORA "[<64;38;15M"),
        FALSUM,
      VECTOR_DEBITUM, CAUSA_H8,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_ROTA_SURSUM, XXXVII, XIV, ZEPHYRUM) } },
    { "fuga, mora, continuatio x10", OCT("\033" MORA "[M !!"), FALSUM,
      VECTOR_DEBITUM, CAUSA_H8,
      { EX_CLAVIS(TESSERA_CLAVIS_FUGA, ZEPHYRUM),
        EX_MUS(TESSERA_MUS_PRESSUS, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM) } },

    /* ESC intra SS3 (T4b: series abrupta, olim debitum) */
    { "SS3 abruptum ESC, SS3 A", OCT("\033O\033OA"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_SURSUM,
          ZEPHYRUM) } },
    { "SS3 abruptum ESC, CSI A", OCT("\033O\033[A"), FALSUM,
        VECTOR_VALET,
      NIHIL, { EX_CLAVIS(TESSERA_CLAVIS_SURSUM,
          ZEPHYRUM) } },

    /* debita - formae alienae */
    { "Linux console [[A (F1)", OCT("\033[[A"), FALSUM, VECTOR_DEBITUM,
      CAUSA_ALIENA, { EX_NIHIL } },
    { "putty [[5~", OCT("\033[[5~"), FALSUM, VECTOR_DEBITUM,
        CAUSA_ALIENA,
      { EX_NIHIL } },
    { "rxvt 2$ + mora", OCT("\033[2$"), VERUM, VECTOR_DEBITUM,
        CAUSA_ALIENA,
      { EX_NIHIL } },

    /* parametrum ingens (T4b: limes, olim overflow = UB) */
    { "1;4294967301A ingens = invalidum (olim overflow -> ctrl)",
        OCT("\033[1;4294967301A"),
        FALSUM,
      VECTOR_VALET, NIHIL,
      { EX_CLAVIS(TESSERA_CLAVIS_SURSUM, ZEPHYRUM) } }
};

#endif /* VECTORES_INITUS_H */
