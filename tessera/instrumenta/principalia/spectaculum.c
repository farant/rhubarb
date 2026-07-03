/* spectaculum.c - Demonstratio interactiva tesserae (Phase B)
 *
 * VECTIS MANUALIS: oculi humani in terminali vero — colores,
 * ornamenta, quadra, cursor, claves, mus, magnitudo mutata,
 * intermissio (Ctrl-Z!). Cetera omnia per pontem memoriae probata;
 * hoc est ultimum digitum.
 *
 * Claves: q = exire; frecce = cursorem movere; i = intermittere
 * (5 secundis... immo: intermittit et statim resumit); ceterae
 * claves + mus in linea status ostenduntur.
 *
 * Curre per: ./tessera/spectaculum.sh
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_posix.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include <stdio.h>
#include <string.h>

interior constans character* CLAVIUM_NOMINA[] = {
    "nulla", "sursum", "deorsum", "dextra", "sinistra", "domus",
    "finis", "pag-sursum", "pag-deorsum", "insertio", "deletio",
    "FUGA", "reditus", "tabula", "retrorsum", "functio"
};

interior vacuum
_scaenam_pingere (TesseraOpus* opus)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    TesseraStilus titulus = tessera_stilus(0xFFD700,
        TESSERA_COLOR_NATIVUS, TESSERA_ORNAMENTUM_CRASSUM);
    i32 lat = tessera_latitudo(opus);
    i32 alt = tessera_altitudo(opus);
    s32 k;

    tessera_purgare(opus, nativus);

    /* quadrum marginale + titulus */
    tessera_quadrum_pingere(opus, ZEPHYRUM, ZEPHYRUM, (s32)lat,
        (s32)alt, TESSERA_LINEA_SIMPLEX, nativus);
    tessera_scribere_literis(opus, II, ZEPHYRUM,
        " TESSERA SPECTACULUM ", titulus);

    /* colores: fascia rubra->viridis->caerulea */
    tessera_scribere_literis(opus, II, II, "colores:", nativus);
    per (k = ZEPHYRUM; k < XXIV && k + XI < (s32)lat - I; k++)
    {
        i32 ruber = (k < VIII) ? 0xFF - (i32)k * XXXII : ZEPHYRUM;
        i32 viridis = (k >= VIII && k < XVI)
            ? 0xFF - ((i32)k - VIII) * XXXII : ZEPHYRUM;
        i32 caeruleus = (k >= XVI) ? 0xFF - ((i32)k - XVI) * XXXII
                                   : ZEPHYRUM;

        tessera_cellulam_ponere(opus, XI + k, II, (i32)' ',
            tessera_stilus(TESSERA_COLOR_NATIVUS,
                (ruber << XVI) | (viridis << VIII) | caeruleus,
                ZEPHYRUM));
    }

    /* ornamenta */
    tessera_scribere_literis(opus, II, IV, "crassum", tessera_stilus(
        TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
        TESSERA_ORNAMENTUM_CRASSUM));
    tessera_scribere_literis(opus, X, IV, "obscurum", tessera_stilus(
        TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
        TESSERA_ORNAMENTUM_OBSCURUM));
    tessera_scribere_literis(opus, XIX, IV, "cursivum",
        tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_CURSIVUM));
    tessera_scribere_literis(opus, XXVIII, IV, "sublineatum",
        tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_SUBLINEATUM));
    tessera_scribere_literis(opus, XL, IV, "inversum",
        tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_INVERSUM));
    tessera_scribere_literis(opus, XLIX, IV, "transfixum",
        tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_TRANSFIXUM));

    /* quadra trium generum */
    tessera_quadrum_pingere(opus, II, VI, X, IV,
        TESSERA_LINEA_SIMPLEX, nativus);
    tessera_scribere_literis(opus, III, VII, "simplex", nativus);
    tessera_quadrum_pingere(opus, XIII, VI, X, IV,
        TESSERA_LINEA_DUPLEX, nativus);
    tessera_scribere_literis(opus, XIV, VII, "duplex", nativus);
    tessera_quadrum_pingere(opus, XXIV, VI, X, IV,
        TESSERA_LINEA_ROTUNDATA, nativus);
    tessera_scribere_literis(opus, XXV, VII, "rotunda", nativus);

    tessera_scribere_literis(opus, II, (s32)alt - III,
        "q exire | frecce cursorem | i intermittere | "
        "claves + mus infra", tessera_stilus(TESSERA_COLOR_NATIVUS,
            TESSERA_COLOR_NATIVUS, TESSERA_ORNAMENTUM_OBSCURUM));
}

s32 principale (vacuum)
{
    Piscina* piscina;
    TesseraPons* pons;
    TesseraOpus* opus;
    TesseraLector* lector;
    character status[128];
    s32 cur_x = II;
    s32 cur_y = II;
    b32 currens = VERUM;

    piscina = piscina_generare_dynamicum("spectaculum", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "spectaculum: piscina deest\n");
        redde I;
    }
    pons = tessera_pons_posix_creare(piscina);
    si (pons == NIHIL)
    {
        fprintf(stderr,
            "spectaculum: terminal verum requiritur (isatty)\n");
        redde I;
    }
    opus = tessera_aperire(piscina, pons);
    lector = tessera_lector_creare(piscina, pons);
    si (opus == NIHIL || lector == NIHIL)
    {
        fprintf(stderr, "spectaculum: apertura fracta\n");
        redde I;
    }

    strcpy(status, "salve! claves preme...");
    dum (currens)
    {
        TesseraEventum ev;

        _scaenam_pingere(opus);
        tessera_scribere_literis(opus, II, (s32)tessera_altitudo(opus)
            - II, status, tessera_stilus_nativus());
        tessera_cursorem_ponere(opus, cur_x, cur_y);
        tessera_praesentare(opus);

        commutatio (tessera_eventum_expectare(lector, &ev, 250))
        {
            casus TESSERA_EVENTUM_CLAVIS:
                si (ev.runa == (s32)'q')
                {
                    currens = FALSUM;
                }
                alioquin si (ev.runa == (s32)'i')
                {
                    tessera_intermittere(opus);
                    /* hic $EDITOR curreret; nos statim redimus */
                    tessera_resumere(opus);
                    strcpy(status,
                        "intermissum et resumptum (pictura plena)");
                }
                alioquin si (ev.clavis == TESSERA_CLAVIS_SURSUM)
                {
                    cur_y--;
                }
                alioquin si (ev.clavis == TESSERA_CLAVIS_DEORSUM)
                {
                    cur_y++;
                }
                alioquin si (ev.clavis == TESSERA_CLAVIS_SINISTRA)
                {
                    cur_x--;
                }
                alioquin si (ev.clavis == TESSERA_CLAVIS_DEXTRA)
                {
                    cur_x++;
                }
                alioquin si (ev.runa > ZEPHYRUM)
                {
                    sprintf(status, "clavis: runa U+%04X '%c'%s%s",
                        (insignatus)ev.runa,
                        (ev.runa >= 0x20 && ev.runa < 0x7F)
                            ? (character)ev.runa : '?',
                        (ev.modificatores
                            & TESSERA_MODIFICATOR_IMPERIUM)
                            ? " +imperium" : "",
                        (ev.modificatores
                            & TESSERA_MODIFICATOR_ALTERUM)
                            ? " +alterum" : "");
                }
                alioquin
                {
                    sprintf(status, "clavis: %s%s (F%d)",
                        CLAVIUM_NOMINA[(i32)ev.clavis
                            % (i32)(magnitudo(CLAVIUM_NOMINA)
                                / magnitudo(CLAVIUM_NOMINA[0]))],
                        (ev.modificatores != ZEPHYRUM)
                            ? " +mod" : "",
                        (int)ev.numerus);
                }
                frange;
            casus TESSERA_EVENTUM_MUS:
                sprintf(status, "mus: genus %d pulsus %d ad %d,%d",
                    (int)ev.mus_genus, (int)ev.mus_pulsus,
                    (int)ev.mus_x, (int)ev.mus_y);
                si (ev.mus_genus == TESSERA_MUS_PRESSUS)
                {
                    cur_x = ev.mus_x;
                    cur_y = ev.mus_y;
                }
                frange;
            casus TESSERA_EVENTUM_AMPLITUDO:
                tessera_magnitudinem_renovare(opus);
                sprintf(status, "amplitudo: %dx%d",
                    (int)ev.latitudo, (int)ev.altitudo);
                frange;
            casus TESSERA_EVENTUM_RESUMPTUM:
                tessera_magnitudinem_renovare(opus);
                strcpy(status, "resumptum (Ctrl-Z functum!)");
                frange;
            ordinarius:
                frange;  /* mora: iterum pingere (horologium vivit) */
        }
        si (cur_x < ZEPHYRUM) cur_x = ZEPHYRUM;
        si (cur_y < ZEPHYRUM) cur_y = ZEPHYRUM;
        si (cur_x >= (s32)tessera_latitudo(opus))
        {
            cur_x = (s32)tessera_latitudo(opus) - I;
        }
        si (cur_y >= (s32)tessera_altitudo(opus))
        {
            cur_y = (s32)tessera_altitudo(opus) - I;
        }
    }

    tessera_claudere(opus);
    imprimere("spectaculum: %d praesentationes, %d octeti emissi, "
        "%.2f ms in emissione\n",
        (int)opus->fructus.praesentationes,
        (int)opus->fructus.octeti_emissi,
        opus->fructus.tempus_praesentandi_ms);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
