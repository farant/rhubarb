/* probatio_tessera_eventum.c - Lector + parsator initus (Phase B)
 *
 * Omnis logica parsationis per pontem memoriae: scripta octetorum
 * -> series eventorum. ASCII, UTF-8 (incl. invalidum), regimen
 * (REDITUS/TABULA/RETRORSUM/imperium+littera), CSI (frecce,
 * ~-codices, modificatores, Z), SS3, ALTERUM praefixum, mus SGR
 * (pressus/solutus/rota), CSI ignota tacite, FUGA sola per moram,
 * eventa plura gestata, AMPLITUDO (interrogatio), RESUMPTUM
 * (vexillum), intermittere/resumere (numeri + pictura plena).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Mundus probationis: pons memoriae + lector. Sectio quaeque suum
 * aedificat (scriptum et status lectoris mutantur - status nullus
 * communis); creatio semel ASSERITUR in parare_suitae. Olim statica
 * globalia (pm_globalis, lector_globalis). */
nomen structura MundusEventuum {
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;
} MundusEventuum;

/* FALSUM = creatio fracta */
interior b32
_mundum_parare (
           Piscina* piscina,
    MundusEventuum* mundus)
{
    mundus->pm = tessera_pons_memoriae_creare(piscina, LXXX, XXIV);
    si (mundus->pm == NIHIL)
    {
        redde FALSUM;
    }
    mundus->lector = tessera_lector_creare(piscina, &mundus->pm->pons);
    redde mundus->lector != NIHIL;
}

/* creatio mundi semel asserta (ut olim ante blocos) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;

    pm = tessera_pons_memoriae_creare(c->piscina, LXXX, XXIV);
    CREDO_NON_NIHIL (pm);
    lector = tessera_lector_creare(c->piscina,
        &pm->pons);
    CREDO_NON_NIHIL (lector);
}

/* Scriptum ponere et eventum unum expectare */
interior TesseraEventum
_unum (
        MundusEventuum* mundus,
    constans character* octeti)
{
    TesseraEventum ev;

    si (octeti != NIHIL)
    {
        tessera_pons_memoriae_initum(mundus->pm,
            (constans i8*)octeti, (i32)strlen(octeti));
    }
    tessera_eventum_expectare(mundus->lector, &ev, X);
    redde ev;
}


/* ==================================================
 * PROBARE: runae - ASCII, UTF-8, invalidum
 * ================================================== */

interior vacuum
_runas_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        ev = _unum(&mundus, "a");
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_CLAVIS);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'a');
        CREDO_AEQUALIS_I32 (ev.modificatores, ZEPHYRUM);

        /* e cum acuto: C3 A9 = U+00E9 */
        ev = _unum(&mundus, "\xC3\xA9");
        CREDO_AEQUALIS_S32 (ev.runa, 0xE9);

        /* signum trium octetorum */
        ev = _unum(&mundus, "\xE2\x94\x80");
        CREDO_AEQUALIS_S32 (ev.runa, 0x2500);

        /* octetus invalidus abicitur, runa sequens valet */
        ev = _unum(&mundus, "\xFF" "b");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
    }
}


/* ==================================================
 * PROBARE: regimen
 * ================================================== */

interior vacuum
_regimen_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        ev = _unum(&mundus, "\r");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        ev = _unum(&mundus, "\n");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        ev = _unum(&mundus, "\t");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_TABULA);
        ev = _unum(&mundus, "\x7F");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_RETRORSUM);

        /* Ctrl+C = 0x03 */
        ev = _unum(&mundus, "\x03");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'c');
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_IMPERIUM);
    }
}


/* ==================================================
 * PROBARE: CSI - frecce, ~-codices, modificatores, Z
 * ================================================== */

interior vacuum
_csi_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        ev = _unum(&mundus, "\033[A");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SURSUM);
        ev = _unum(&mundus, "\033[D");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SINISTRA);
        ev = _unum(&mundus, "\033[H");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DOMUS);

        ev = _unum(&mundus, "\033[3~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DELETIO);
        ev = _unum(&mundus, "\033[5~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_PAGINA_SURSUM);
        ev = _unum(&mundus, "\033[2~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_INSERTIO);

        /* F5 = 15~, F6 = 17~, F12 = 24~ */
        ev = _unum(&mundus, "\033[15~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUNCTIO);
        CREDO_AEQUALIS_I32 (ev.numerus, V);
        ev = _unum(&mundus, "\033[17~");
        CREDO_AEQUALIS_I32 (ev.numerus, VI);
        ev = _unum(&mundus, "\033[24~");
        CREDO_AEQUALIS_I32 (ev.numerus, XII);

        /* modificatores: 1;5C = imperium+dextra; 3;2~ =
         * maiuscula+deletio */
        ev = _unum(&mundus, "\033[1;5C");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DEXTRA);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_IMPERIUM);
        ev = _unum(&mundus, "\033[3;2~");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DELETIO);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_MAIUSCULA);

        /* Z = tabula retro */
        ev = _unum(&mundus, "\033[Z");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_TABULA);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_MAIUSCULA);
    }
}


/* ==================================================
 * PROBARE: SS3 + ALTERUM
 * ================================================== */

interior vacuum
_alterum_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        ev = _unum(&mundus, "\033OB");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_DEORSUM);
        ev = _unum(&mundus, "\033OP");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUNCTIO);
        CREDO_AEQUALIS_I32 (ev.numerus, I);

        /* alt+b */
        ev = _unum(&mundus, "\033b");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_ALTERUM);

        /* alt+reditus */
        ev = _unum(&mundus, "\033\r");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_REDITUS);
        CREDO_AEQUALIS_I32 (ev.modificatores,
            TESSERA_MODIFICATOR_ALTERUM);
    }
}


/* ==================================================
 * PROBARE: mus SGR
 * ================================================== */

interior vacuum
_murem_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        ev = _unum(&mundus, "\033[<0;5;3M");
        CREDO_AEQUALIS_I32 ((i32)ev.genus, (i32)TESSERA_EVENTUM_MUS);
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_PRESSUS);
        CREDO_AEQUALIS_S32 (ev.mus_x, IV);   /* 1-basata -> 0 */
        CREDO_AEQUALIS_S32 (ev.mus_y, II);
        CREDO_AEQUALIS_I32 (ev.mus_pulsus, ZEPHYRUM);

        ev = _unum(&mundus, "\033[<2;1;1m");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_SOLUTUS);
        CREDO_AEQUALIS_I32 (ev.mus_pulsus, II);

        ev = _unum(&mundus, "\033[<64;2;2M");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_ROTA_SURSUM);
        ev = _unum(&mundus, "\033[<65;2;2M");
        CREDO_AEQUALIS_I32 ((i32)ev.mus_genus,
            (i32)TESSERA_MUS_ROTA_DEORSUM);
    }
}


/* ==================================================
 * PROBARE: CSI ignota tacite; eventa plura gestata; FUGA
 * sola per moram; NIHIL in vacuo
 * ================================================== */

interior vacuum
_firmitatem_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        /* CSI ignota (privata '?') consumitur, clavis sequens
         * valet */
        ev = _unum(&mundus, "\033[?1049h" "x");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'x');

        /* tria eventa uno scripto */
        ev = _unum(&mundus, "ab\033[A");
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'a');
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'b');
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_SURSUM);

        /* FUGA sola: scriptum exhaustum = mora -> FUGA */
        ev = _unum(&mundus, "\033");
        CREDO_AEQUALIS_I32 ((i32)ev.clavis,
            (i32)TESSERA_CLAVIS_FUGA);

        /* vacuum = NIHIL */
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);
    }
}


/* ==================================================
 * PROBARE: AMPLITUDO (interrogatio) + RESUMPTUM (vexillum)
 * ================================================== */

interior vacuum
_amplitudinem_probare (
    CredoContextus* c)
{
    MundusEventuum mundus;

    si (!_mundum_parare(c->piscina, &mundus))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TesseraEventum ev;

        tessera_pons_memoriae_amplitudo(mundus.pm, XL, XII);
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_AMPLITUDO);
        CREDO_AEQUALIS_I32 (ev.latitudo, XL);
        CREDO_AEQUALIS_I32 (ev.altitudo, XII);

        /* semel tantum */
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);

        mundus.pm->resumendum  = VERUM;
        ev                     = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_RESUMPTUM);
        ev = _unum(&mundus, NIHIL);
        CREDO_AEQUALIS_I32 ((i32)ev.genus,
            (i32)TESSERA_EVENTUM_NIHIL);
    }
}


/* ==================================================
 * PROBARE: intermittere/resumere - numeri pontis + pictura
 * plena post resumptionem
 * ================================================== */

interior vacuum
_intermissionem_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        TesseraPonsMemoriae* pi = tessera_pons_memoriae_creare(
            piscina, VIII, III);
        TesseraOpus* opus;
             chorda  captum;

        CREDO_NON_NIHIL (pi);
        opus = tessera_aperire(piscina, &pi->pons);
        CREDO_NON_NIHIL (opus);
        tessera_scribere_literis(opus, ZEPHYRUM, ZEPHYRUM, "st",
            tessera_stilus_nativus());
        CREDO_VERUM (tessera_praesentare(opus));

        tessera_pons_memoriae_purgare(pi);
        tessera_intermittere(opus);
        CREDO_FALSUM (pi->intratum);
        CREDO_AEQUALIS_I32 (pi->numerus_exitum, I);
        captum = tessera_pons_memoriae_captum(pi);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?2026l" "\033[0m\033[?25h");

        tessera_resumere(opus);
        CREDO_VERUM (pi->intratum);
        CREDO_AEQUALIS_I32 (pi->numerus_intratum, II);

        /* pictura plena: quadrum proximum ED emittit et omnia
         * pingit */
        tessera_pons_memoriae_purgare(pi);
        CREDO_VERUM (tessera_praesentare(opus));
        captum = tessera_pons_memoriae_captum(pi);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum,
            "\033[?2026h" "\033[?25l\033[0m\033[2J\033[1;1H\033[0mst" "\033[?2026l");
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "runas", _runas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "regimen", _regimen_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "CSI", _csi_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "SS3 et ALTERUM", _alterum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "murem", _murem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "robustitatem", _firmitatem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "amplitudinem et resumptum", _amplitudinem_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "intermittere/resumere", _intermissionem_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tessera_eventum", SECTIONES,
        _suitam_parare, NIHIL);
}
