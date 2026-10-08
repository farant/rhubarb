/* probatio_ludus_tessera_reditus.c - reditus: tessera pingit, octeti
 * in aemulatorem, cellulae conferuntur (aemulator-plan A3)
 *
 * Quadrum quodque: tessera_praesentare -> octeti capti (pons
 * memoriae) -> aemulator_scribere -> purgare; deinde omnis cellula
 * frontis tesserae cum cellula aemulatoris confertur (octeti, genus
 * latitudinis, stilus totus) et cursor. Discrepantia prima nominatur
 * (contextus, x, y, expectatum, receptum). Post sessionem quamque
 * aemulator NULLAM seriem ignotam vidit: tessera nihil emittit quod
 * aemulator non intellegit.
 *
 * I.   quadra manu picta: textus, stili, latae, quadrum, replere,
 *      cursor visibilis et celatus, differentia (quadra sequentia)
 * II.  mutatio magnitudinis (pons et aemulator simul)
 * III. quadra fortuita (sors, semen fixum): cellulae, stili RGB et
 *      nativi, ornamenta, latae, replere, cursor
 * IV.  applicatio vera (vicus: scriba, Ctrl-A n, pictor, ictus)
 *
 * Colores PLENI: tessera RGB integrum emittit (CCLVI quantizaret).
 * Graphemata plurium runarum absunt (aemulator v2: notae iungentes
 * abiciuntur - limes nominatus).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "eventus.h"
#include "sors.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "ludus_tessera.h"
#include "stilus_terminalis.h"
#include "aemulator.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

nomen structura {
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;
              Aemulator* aem;
} Reditus;

interior Reditus
reditum_creare (
    i32 latitudo,
    i32 altitudo)
{
                  Reditus r;
    AemulatorConfiguratio cfg;

    r.pm    = tessera_pons_memoriae_creare(piscina, latitudo, altitudo);
    r.opus  = tessera_aperire(piscina, &r.pm->pons);
    tessera_colores_ponere(r.opus, TESSERA_COLORES_PLENI);
    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo  = latitudo;
    cfg.altitudo  = altitudo;
    r.aem         = aemulator_creare(piscina, &cfg);
    redde r;
}

/* praesentare, octeti in aemulatorem, captum purgare */
interior b32
quadrum (
    Reditus* r)
{
    chorda c;
       b32 ok;

    ok  = tessera_praesentare(r->opus);
    c   = tessera_pons_memoriae_captum(r->pm);
    aemulator_scribere(r->aem, c.datum, c.mensura);
    tessera_pons_memoriae_purgare(r->pm);
    redde ok;
}

interior StilusColor
color_tesserae (
    i32 c)
{
    StilusColor sc;

    si (c == TESSERA_COLOR_NATIVUS)
    {
        sc.genus = STILUS_COLOR_NATIVUS;
        sc.valor = ZEPHYRUM;
    }
    alioquin
    {
        sc.genus = STILUS_COLOR_RGB;
        sc.valor = c & 0x00FFFFFF;
    }
    redde sc;
}

/* stilus quem SGR tesserae in terminali relinquit */
interior StilusTerminalis
stilus_tesserae (
    TesseraCellula c)
{
    StilusTerminalis st;

    stilus_nativus(&st);
    st.color_litterae  = color_tesserae(c.color_litterae);
    st.color_fundi     = color_tesserae(c.color_fundi);
    si (c.ornamenta & TESSERA_ORNAMENTUM_CRASSUM)
    {
        st.ornamenta |= STILUS_CRASSUM;
    }
    si (c.ornamenta & TESSERA_ORNAMENTUM_OBSCURUM)
    {
        st.ornamenta |= STILUS_OBSCURUM;
    }
    si (c.ornamenta & TESSERA_ORNAMENTUM_CURSIVUM)
    {
        st.ornamenta |= STILUS_CURSIVUM;
    }
    si (c.ornamenta & TESSERA_ORNAMENTUM_INVERSUM)
    {
        st.ornamenta |= STILUS_INVERSUM;
    }
    si (c.ornamenta & TESSERA_ORNAMENTUM_TRANSFIXUM)
    {
        st.ornamenta |= STILUS_TRANSFIXUM;
    }
    si (c.ornamenta & TESSERA_ORNAMENTUM_SUBLINEATUM)
    {
        st.sublinea = STILUS_SUBLINEA_SIMPLEX;
    }
    redde st;
}

interior b32
vacuum_est (
    constans i8* o,
            i32  n)
{
    redde n == ZEPHYRUM || (n == I && o[ZEPHYRUM] == ' ');
}

/* omnes cellulae et cursor; discrepantia prima impressa */
interior b32
conferre (
               Reditus* r,
    constans character* contextus)
{
      TesseraCellula t;
    AemulatorCellula e;
     AemulatorCursor c;
    StilusTerminalis st;
                  i8 o[XVI];
                 i32 n;
   AemulatorLatitudo lat;
                 s32 x;
                 s32 y;

    si (   aemulator_latitudo(r->aem) != r->opus->latitudo
        || aemulator_altitudo(r->aem) != r->opus->altitudo)
    {
        imprimere("  reditus '%s': magnitudo %dx%d contra %dx%d\n",
            contextus, (integer)aemulator_latitudo(r->aem),
            (integer)aemulator_altitudo(r->aem),
            (integer)r->opus->latitudo, (integer)r->opus->altitudo);
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < (s32)r->opus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)r->opus->latitudo; x++)
        {
            /* gradus frontis = latitudo MAXIMA (tessera_opus _index) */
            t = r->opus->frons[(i32)y * TESSERA_LATITUDO_MAXIMA
                + (i32)x];
            n = tessera_cellulae_octeti(r->opus, x, y, o, XVI);
            si (!aemulator_cellula(r->aem, (i32)x, (i32)y, &e))
            {
                redde FALSUM;
            }
            lat = (t.ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
                ? AEMULATOR_CAUDA
                : ((t.ornamenta & TESSERA_ORNAMENTUM_LATUM)
                    ? AEMULATOR_LATA : AEMULATOR_ANGUSTA);
            st = stilus_tesserae(t);
            si (   !(vacuum_est(o, n)
                     && vacuum_est(e.graphema.datum,
                                   e.graphema.mensura))
                && (   n != e.graphema.mensura
                    || memcmp(o, e.graphema.datum,
                              (memoriae_index)n) != ZEPHYRUM))
            {
                imprimere("  reditus '%s' (%d,%d): textus '%.*s' contra"
                    " '%.*s'\n", contextus, (integer)x, (integer)y,
                    (integer)n, (constans character*)o,
                    (integer)e.graphema.mensura,
                    (constans character*)e.graphema.datum);
                redde FALSUM;
            }
            si (lat != e.latitudo)
            {
                imprimere("  reditus '%s' (%d,%d): latitudo %d contra"
                    " %d\n", contextus, (integer)x, (integer)y,
                    (integer)lat, (integer)e.latitudo);
                redde FALSUM;
            }
            si (!stilus_aequalis(&st, &e.stilus))
            {
                imprimere("  reditus '%s' (%d,%d): stilus litterae"
                    " %d/%x fundi %d/%x orn %x contra %d/%x %d/%x"
                    " %x\n", contextus, (integer)x, (integer)y,
                    (integer)st.color_litterae.genus,
                    (unsigned)st.color_litterae.valor,
                    (integer)st.color_fundi.genus,
                    (unsigned)st.color_fundi.valor,
                    (unsigned)st.ornamenta,
                    (integer)e.stilus.color_litterae.genus,
                    (unsigned)e.stilus.color_litterae.valor,
                    (integer)e.stilus.color_fundi.genus,
                    (unsigned)e.stilus.color_fundi.valor,
                    (unsigned)e.stilus.ornamenta);
                redde FALSUM;
            }
        }
    }
    c = aemulator_cursor(r->aem);
    si (r->opus->cursor_visibilis_actus != c.visibilis)
    {
        imprimere("  reditus '%s': cursor visibilis %d contra %d\n",
            contextus, (integer)r->opus->cursor_visibilis_actus,
            (integer)c.visibilis);
        redde FALSUM;
    }
    si (   r->opus->cursor_visibilis_actus
        && (   r->opus->cursor_x_actus != (s32)c.x
            || r->opus->cursor_y_actus != (s32)c.y))
    {
        imprimere("  reditus '%s': cursor (%d,%d) contra (%d,%d)\n",
            contextus, (integer)r->opus->cursor_x_actus,
            (integer)r->opus->cursor_y_actus, (integer)c.x,
            (integer)c.y);
        redde FALSUM;
    }
    redde VERUM;
}

/* signa fortuita: ASCII, lineae, latae (CJK, emoji) */
hic_manens constans character* signa_fortuita[] = {
    "a", "Z", "7", "#", " ", "\xE2\x94\x80", "\xE2\x95\x91",
    "\xE2\x96\x88", "\xC3\xA9", "\xE6\xA9\x8B", "\xF0\x9F\x98\x80",
    "\xE5\xAD\x97"
};

interior TesseraStilus
stilus_fortuitus (
    Sors* s)
{
    i32 littera;
    i32 fundus;

    littera = sors_casu(s, I, IV) ? (i32)TESSERA_COLOR_NATIVUS
                              : (sors_proximum(s) & 0x00FFFFFF);
    fundus = sors_casu(s, I, II) ? (i32)TESSERA_COLOR_NATIVUS
                              : (sors_proximum(s) & 0x00FFFFFF);
    redde tessera_stilus(littera, fundus,
        sors_intra(s, (i32)TESSERA_ORNAMENTA_STILI + I));
}

s32
principale (vacuum)
{
         b32 praeteritus;
     Reditus r;
        Sors s;
         i32 i;
         i32 k;
         i32 latitudo_unitatis;
   character contextus[LXIV];

    piscina =
        piscina_generare_dynamicum("probatio_ludus_tessera_reditus",
        XVI * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- I. quadra manu picta ---\n");
    r = reditum_creare(XL, XII);
    CREDO_NON_NIHIL(r.aem);
    tessera_scribere_literis(r.opus, ZEPHYRUM, ZEPHYRUM, "salve munde",
        tessera_stilus_nativus());
    tessera_scribere_literis(r.opus, II, I, "crassum",
        tessera_stilus((i32)TESSERA_COLOR_NATIVUS,
        (i32)TESSERA_COLOR_NATIVUS,
        TESSERA_ORNAMENTUM_CRASSUM | TESSERA_ORNAMENTUM_SUBLINEATUM));
    tessera_scribere_literis(r.opus, II, II, "rubrum in caeruleo",
        tessera_stilus(0xFF0000, 0x0000FF, ZEPHYRUM));
    tessera_scribere_literis(r.opus, ZEPHYRUM, III,
        "\xE6\xA9\x8B\xE5\xAD\x97 \xF0\x9F\x98\x80!",
        tessera_stilus(0x00FF00, (i32)TESSERA_COLOR_NATIVUS,
        TESSERA_ORNAMENTUM_CURSIVUM));
    tessera_quadrum_pingere(r.opus, XX, IV, X, V, TESSERA_LINEA_DUPLEX,
        tessera_stilus(0xC0C0C0, 0x202020, ZEPHYRUM));
    tessera_replere(r.opus, ZEPHYRUM, IX, XL, II, (i32)' ',
        tessera_stilus((i32)TESSERA_COLOR_NATIVUS, 0x334455, ZEPHYRUM));
    tessera_cursorem_ponere(r.opus, V, X);
    CREDO_VERUM(quadrum(&r));
    CREDO_VERUM(conferre(&r, "I primum"));
    /* differentia: pauca mutata, cursor celatus */
    tessera_scribere_literis(r.opus, ZEPHYRUM, ZEPHYRUM, "SALVE",
        tessera_stilus(0xFFFF00, (i32)TESSERA_COLOR_NATIVUS,
        TESSERA_ORNAMENTUM_INVERSUM));
    tessera_scribere_literis(r.opus, I, III, "x",
        tessera_stilus_nativus());
    tessera_cursorem_ponere(r.opus, (s32)-I, (s32)-I);
    CREDO_VERUM(quadrum(&r));
    CREDO_VERUM(conferre(&r, "I differentia"));
    /* quadrum sine mutatione */
    CREDO_VERUM(quadrum(&r));
    CREDO_VERUM(conferre(&r, "I idem"));
    CREDO_AEQUALIS_I32(aemulator_ignota(r.aem), ZEPHYRUM);

    imprimere("\n--- II. mutatio magnitudinis ---\n");
    tessera_pons_memoriae_amplitudo(r.pm, XXX, X);
    CREDO_VERUM(tessera_magnitudinem_renovare(r.opus));
    CREDO_VERUM(aemulator_amplitudo(r.aem, XXX, X));
    tessera_scribere_literis(r.opus, ZEPHYRUM, IX, "ima linea",
        tessera_stilus(0x112233, 0x445566,
        TESSERA_ORNAMENTUM_TRANSFIXUM));
    CREDO_VERUM(quadrum(&r));
    CREDO_VERUM(conferre(&r, "II minor"));
    tessera_pons_memoriae_amplitudo(r.pm, L, XV);
    CREDO_VERUM(tessera_magnitudinem_renovare(r.opus));
    CREDO_VERUM(aemulator_amplitudo(r.aem, L, XV));
    tessera_scribere_literis(r.opus, XL, XIV, "angulus",
        tessera_stilus_nativus());
    CREDO_VERUM(quadrum(&r));
    CREDO_VERUM(conferre(&r, "II maior"));
    CREDO_AEQUALIS_I32(aemulator_ignota(r.aem), ZEPHYRUM);

    imprimere("\n--- III. quadra fortuita (semen 0x5EED) ---\n");
    sors_seminare(&s, 0x5EED, I);
    r = reditum_creare(XXXII, XII);
    per (i = ZEPHYRUM; i < LX; i++)
    {
        per (k = ZEPHYRUM; k < XL; k++)
        {
            constans character* signum;

            signum = signa_fortuita[sors_intra(&s,
                (i32)(magnitudo(signa_fortuita)
                / magnitudo(signa_fortuita[ZEPHYRUM])))];
            (vacuum)tessera_graphema_ponere(r.opus,
                sors_inter(&s, ZEPHYRUM, XXXI),
                sors_inter(&s, ZEPHYRUM, XI),
                (constans i8*)signum,
                (constans i8*)signum + strlen(signum),
                stilus_fortuitus(&s), &latitudo_unitatis);
        }
        si (sors_casu(&s, I, VIII))
        {
            tessera_replere(r.opus, sors_inter(&s, ZEPHYRUM, XX),
                sors_inter(&s, ZEPHYRUM, VIII), sors_inter(&s, I, XII),
                sors_inter(&s, I, IV), (i32)'.', stilus_fortuitus(&s));
        }
        si (sors_casu(&s, I, III))
        {
            tessera_cursorem_ponere(r.opus, (s32)-I, (s32)-I);
        }
        alioquin
        {
            tessera_cursorem_ponere(r.opus,
                sors_inter(&s, ZEPHYRUM, XXXI),
                sors_inter(&s, ZEPHYRUM, XI));
        }
        CREDO_VERUM(quadrum(&r));
        sprintf(contextus, "III quadrum %d", (integer)i);
        si (!conferre(&r, contextus))
        {
            CREDO_VERUM(FALSUM);
            frange;
        }
    }
    CREDO_AEQUALIS_I32(aemulator_ignota(r.aem), ZEPHYRUM);

    imprimere("\n--- IV. applicatio vera (vicus) ---\n");
    {
         VicusApplicatio app;
            LudusTessera* lt;
              ManusLudus* m;
                 Punctum  p[III];

        r.pm    = tessera_pons_memoriae_creare(piscina, LXXX, XXX);
        r.opus  = tessera_aperire(piscina, &r.pm->pons);
        tessera_colores_ponere(r.opus, TESSERA_COLORES_PLENI);
        {
            AemulatorConfiguratio cfg;

            aemulator_configuratio_initiare(&cfg);
            cfg.latitudo  = LXXX;
            cfg.altitudo  = XXX;
            r.aem         = aemulator_creare(piscina, &cfg);
        }
        /* tabula 1 ordinaria: latus dextrum terminale - concha brevis,
         * non initialis cum ambitu probantis (vicus-latera S1c) */
        (vacuum)setenv("SHELL", "/bin/sh", I);
        CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern,
            volumen_temporarium(piscina, "lt_reditus"),
            getenv("RHUBARB_RADIX"), LXXX * VI, XXX * VIII));
        lt = ludus_tessera_creare(piscina, app.d,
            vicus_figurae(app.vicus), ZEPHYRUM, vicus_imago_fons,
            app.vicus, r.opus, VI, VIII);
        CREDO_NON_NIHIL(lt);
        ludus_tessera_quadrum(lt, M);
        CREDO_VERUM(quadrum(&r));
        CREDO_VERUM(conferre(&r, "IV scriba"));
        m = manus_ludus_creare(piscina, app.d);
        CREDO_VERUM(manus_ludus_scribere(m, "isalve munde"));
        ludus_tessera_quadrum(lt, M * II);
        CREDO_VERUM(quadrum(&r));
        CREDO_VERUM(conferre(&r, "IV scriptio"));
        CREDO_VERUM(manus_ludus_clavem(m, 'a', MOD_IMPERIUM));
        CREDO_VERUM(manus_ludus_scribere(m, "n"));
        ludus_tessera_quadrum(lt, M * III);
        CREDO_VERUM(quadrum(&r));
        CREDO_VERUM(conferre(&r, "IV pictor"));
        /* vicus-latera S2a: pictor = latus DEXTRUM tabulae 2 - focus
         * ante tractum (ictus primus solum focaret) */
        CREDO_VERUM(vicus_focum_ponere(app.vicus, VICUS_DEXTRUM));
        p[0].x = X;
        p[0].y = X;
        p[1].x = XL;
        p[1].y = XXV;
        p[2].x = LXX;
        p[2].y = XL;
        CREDO_VERUM(manus_ludus_trahere(m, "#tabula", p, III));
        ludus_tessera_quadrum(lt, M * IV);
        CREDO_VERUM(quadrum(&r));
        CREDO_VERUM(conferre(&r, "IV ictus"));
        CREDO_AEQUALIS_I32(aemulator_ignota(r.aem), ZEPHYRUM);
        /* volumen temporarium claudendum (aliter /tmp/lt_reditus-N
         * cumulantur; volumen_temporarium post C deficit) */
        volumen_claudere(app.volumen);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
