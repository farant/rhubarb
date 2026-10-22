/* probatio_sonitus.c - Probationes vocis effectuum (include/sonitus.h)
 *
 * Altitudo per circumvolutiones serrae numeratas: clivo +I octava/s
 * cycli in secundo uno = CDXL / ln 2 x (2 - 1) = 634.78; saltu ad
 * dimidium secundi (ratio II) = CCXX + CDXL. Praeparata solarii
 * portata: longitudo, fines, determinismus. Vide lib/sonitus.worklog.md.
 */
#include "latina.h"
#include "credo.h"
#include "sonitus.h"
#include <stddef.h>

/* IV secundae: crepitus (III s) et ventus (II.V s) capit */
#define TABULA_MAXIMA (IV * SONUS_FREQUENTIA)

hic_manens f32 tabula_a[TABULA_MAXIMA];
hic_manens f32 tabula_b[TABULA_MAXIMA];

/* parare omnium sectionum: tabulae communes ad silentium reponuntur,
 * ne sectio exempla prioris legat */
interior vacuum
_tabulas_parare (
    CredoContextus* c)
{
    i32 k;

    (vacuum)c;
    per (k = ZEPHYRUM; k < TABULA_MAXIMA; k++)
    {
        tabula_a[k] = 0.0f;
        tabula_b[k] = 0.0f;
    }
}

/* serra plena unius secundi: forma I, involucrum plenum sine
 * impetu/remissione, cetera neutra */
interior vacuum
_neutra (
    SonitusParametra* p)
{
    p->forma               = 1.0f;
    p->impetus             = 0.0f;
    p->mora                = 1.0f;
    p->ictus               = 0.0f;
    p->remissio            = 0.0f;
    p->frequentia          = 440.0f;
    p->clivus              = 0.0f;
    p->acceleratio         = 0.0f;
    p->vibratus            = 0.0f;
    p->vibratus_celeritas  = 0.0f;
    p->saltus              = 1.0f;
    p->saltus_tempus       = 0.0f;
    p->officium            = 0.5f;
    p->officium_clivus     = 0.0f;
    p->gravia              = 0.0f;
    p->acuta               = 0.0f;
    p->gradus              = 0.0f;
    p->retentio            = 0.0f;
    p->amplitudo           = 1.0f;
    p->varietas            = 0.0f;
}

interior s32
_saltus_serrae (
    constans f32* x,
             i32  n)
{
    i32 k;
    s32 numerus = ZEPHYRUM;

    per (k = I; k < n; k++)
    {
        si (x[k] < x[k - I] - 1.0f)
        {
            numerus++;
        }
    }
    redde numerus;
}

interior b32
_pares (
    constans f32* a,
    constans f32* b,
             i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (a[k] != b[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_intra_fines (
    constans f32* x,
             i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (x[k] < -1.0f || x[k] > 1.0f)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


    /* ==================================================
     * SCHEMA: viginti campi, ordine structurae, coercitio
     * ================================================== */

interior vacuum
_sectio_schema (
    CredoContextus* c)
{
    SonitusParametra p;
                 i32 k;
                 b32 ordo     = VERUM;
                 b32 omnes    = VERUM;
                 b32 reditus  = VERUM;

    (vacuum)c;
    CREDO_AEQUALIS_I32 (sonitus_campi_numerus(), XX);
    _neutra(&p);
    per (k = ZEPHYRUM; k < XX; k++)
    {
        constans SonitusCampus* campus = sonitus_campus((i32)k);

        si (campus == NIHIL)
        {
            omnes = FALSUM;
            perge;
        }
        /* viginti f32 sine impletione: offset = index x IV */
        si (campus->offset != (memoriae_index)k * magnitudo(f32))
        {
            ordo = FALSUM;
        }
        sonitus_campum_ponere(&p, (i32)k, campus->maximum);
        si (sonitus_campum_legere(&p, (i32)k) != campus->maximum)
        {
            reditus = FALSUM;
        }
    }
    CREDO_VERUM (omnes);
    CREDO_VERUM (ordo);
    CREDO_VERUM (reditus);
    CREDO_VERUM (sonitus_campus(XX) == NIHIL);

    /* nomina et offset campi nominati */
    si (sonitus_campus(V) != NIHIL)
    {
        constans SonitusCampus* f = sonitus_campus(V);

        CREDO_VERUM (f->titulus[ZEPHYRUM] == 'f'
            && f->titulus[I] == 'r');
        CREDO_VERUM (f->offset == offsetof(SonitusParametra,
            frequentia));
        /* coercitio ad fines campi */
        sonitus_campum_ponere(&p, V, 99999.0f);
        CREDO_F32_PROXIMUS (p.frequentia, f->maximum, 0.0f);
        sonitus_campum_ponere(&p, V, -5.0f);
        CREDO_F32_PROXIMUS (p.frequentia, f->minimum, 0.0f);
    }
    alioquin
    {
        CREDO_VERUM (FALSUM);
    }
}


    /* ==================================================
     * PRAEPARATA: VIII ex solario + tonus (domesticum), nomina per
     * titulum
     * ================================================== */

interior vacuum
_sectio_praeparata (
    CredoContextus* c)
{
    i32 k;
    b32 omnes = VERUM;

    (vacuum)c;
    CREDO_AEQUALIS_I32 (sonitus_praeparatorum_numerus(), IX);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        constans character* titulus = sonitus_praeparati_titulus(k);

        si (titulus == NIHIL || sonitus_praeparatum(titulus) == NIHIL)
        {
            omnes = FALSUM;
        }
    }
    CREDO_VERUM (omnes);
    CREDO_VERUM (sonitus_praeparati_titulus(IX) == NIHIL);
    CREDO_VERUM (sonitus_praeparatum("nullum") == NIHIL);
    CREDO_VERUM (sonitus_praeparatum("radius") != NIHIL);
    CREDO_VERUM (sonitus_praeparatum("radius")
                 != sonitus_praeparatum("punctum"));
    si (sonitus_praeparatum("punctum") != NIHIL)
    {
        CREDO_F32_PROXIMUS (sonitus_praeparatum("punctum")->frequentia,
                            880.0f, 0.0f);
    }
    /* tonus: vox cantus - altitudo FIXA (nec clivus nec saltus nec
     * vibratus nec varietas), ut nota sonet quae scripta est */
    {
        constans SonitusParametra* t = sonitus_praeparatum("tonus");

        CREDO_VERUM (t != NIHIL);
        si (t != NIHIL)
        {
            CREDO_F32_PROXIMUS (t->forma, 0.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->frequentia, 440.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->clivus, 0.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->acceleratio, 0.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->vibratus, 0.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->saltus, 1.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->officium, 0.5f, 0.0f);
            CREDO_F32_PROXIMUS (t->officium_clivus, 0.0f, 0.0f);
            CREDO_F32_PROXIMUS (t->varietas, 0.0f, 0.0f);
            CREDO_VERUM (t->impetus > 0.0f && t->impetus < 0.01f);
            CREDO_VERUM (t->remissio > 0.0f);
        }
    }
}


    /* ==================================================
     * LONGITUDO et FINES: omnia praeparata, semina V
     * ================================================== */

interior vacuum
_sectio_longitudo (
    CredoContextus* c)
{
    i32 k;
    i32 semen;
    b32 longitudines  = VERUM;
    b32 fines         = VERUM;
    b32 sonantia      = VERUM;

    (vacuum)c;
    per (k = ZEPHYRUM; k < sonitus_praeparatorum_numerus(); k++)
    {
        constans SonitusParametra* p =
            sonitus_praeparatum(sonitus_praeparati_titulus(k));

        per (semen = I; semen <= V; semen++)
        {
            i32 n = sonitus_longitudo(p, (i64)semen);
            i32 scripta = sonitus_reddere(p, (i64)semen, tabula_a,
                                          TABULA_MAXIMA);

            si (n == ZEPHYRUM || scripta != n)
            {
                longitudines = FALSUM;
            }
            si (!_intra_fines(tabula_a, scripta))
            {
                fines = FALSUM;
            }
            si (   scripta > M && tabula_a[M / II] == 0.0f
                && tabula_a[M] == 0.0f)
            {
                sonantia = FALSUM;
            }
        }
    }
    CREDO_VERUM (longitudines);
    CREDO_VERUM (fines);
    CREDO_VERUM (sonantia);

    /* punctum (varietas 0): (0 + 0.045 + 0.09) x XLVIII milia, ab
     * semine non pendet */
    {
        constans SonitusParametra* p = sonitus_praeparatum("punctum");

        si (p != NIHIL)
        {
            CREDO_AEQUALIS_I32 (sonitus_longitudo(p, I), VI * M
                + CDLXXX);
            CREDO_AEQUALIS_I32 (sonitus_longitudo(p, CMXCIX),
                                sonitus_longitudo(p, I));
        }
    }
    /* maximum minus longitudine: abscinditur */
    {
        constans SonitusParametra* p = sonitus_praeparatum("radius");

        si (p != NIHIL)
        {
            CREDO_AEQUALIS_I32 (sonitus_reddere(p, I, tabula_a, C), C);
        }
    }
}


    /* ==================================================
     * DETERMINISMUS: semen idem, semen aliud, varietas
     * ================================================== */

interior vacuum
_sectio_determinismus (
    CredoContextus* c)
{
    constans SonitusParametra* radius = sonitus_praeparatum("radius");
    constans SonitusParametra* punctum = sonitus_praeparatum("punctum");
    constans SonitusParametra* passus = sonitus_praeparatum("passus");

    (vacuum)c;
    si (radius == NIHIL || punctum == NIHIL || passus == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    {
        i32 n = sonitus_reddere(radius, VII, tabula_a, TABULA_MAXIMA);
        i32 m = sonitus_reddere(radius, VII, tabula_b, TABULA_MAXIMA);

        CREDO_AEQUALIS_I32 (n, m);
        CREDO_VERUM (_pares(tabula_a, tabula_b, n));
        /* varietas 0.2: semen aliud frequentiam perturbat */
        m = sonitus_reddere(radius, VIII, tabula_b, TABULA_MAXIMA);
        CREDO_VERUM (!_pares(tabula_a, tabula_b, n < m ? n : m));
    }
    {
        /* varietas 0 et forma sine strepitu: semen nihil mutat */
        i32 n = sonitus_reddere(punctum, I, tabula_a, TABULA_MAXIMA);
        i32 m = sonitus_reddere(punctum, II, tabula_b, TABULA_MAXIMA);

        CREDO_AEQUALIS_I32 (n, m);
        CREDO_VERUM (_pares(tabula_a, tabula_b, n));
    }
    {
        /* strepitus sine varietate: semen strepitum SOLUM mutat */
        SonitusParametra p = *passus;
                     i32 n;
                     i32 m;

        p.varietas  = 0.0f;
        n           = sonitus_reddere(&p, I, tabula_a, TABULA_MAXIMA);
        m           = sonitus_reddere(&p, II, tabula_b, TABULA_MAXIMA);
        CREDO_AEQUALIS_I32 (n, m);
        CREDO_VERUM (!_pares(tabula_a, tabula_b, n));
    }
}


    /* ==================================================
     * VOX: reddere = vocem manu trahere, octetim
     * ================================================== */

interior vacuum
_sectio_vox (
    CredoContextus* c)
{
     constans SonitusParametra* p = sonitus_praeparatum("fragor");
                    SonitusVox  vox;
                           i32  n;
                           i32  k = ZEPHYRUM;

    (vacuum)c;
    si (p == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    n = sonitus_reddere(p, XIII, tabula_a, TABULA_MAXIMA);
    sonitus_vocem_parare(&vox, p, XIII);
    CREDO_VERUM (!sonitus_vox_finita(&vox));
    dum (!sonitus_vox_finita(&vox) && k < TABULA_MAXIMA)
    {
        tabula_b[k] = sonitus_vocis_proximum(&vox);
        k++;
    }
    CREDO_AEQUALIS_I32 (k, n);
    CREDO_VERUM (_pares(tabula_a, tabula_b, n));
    /* post finem: silentium */
    CREDO_F32_PROXIMUS (sonitus_vocis_proximum(&vox), 0.0f, 0.0f);
    CREDO_VERUM (sonitus_vox_finita(&vox));
}


    /* ==================================================
     * ALTITUDO: frequentia, clivus, saltus, vibratus
     * ================================================== */

interior s32
_cycli (
    constans SonitusParametra* p)
{
    i32 n = sonitus_reddere(p, I, tabula_a, TABULA_MAXIMA);

    redde _saltus_serrae(tabula_a, n);
}

interior vacuum
_sectio_altitudo (
    CredoContextus* c)
{
    SonitusParametra p;
                 s32 n;

    (vacuum)c;
    _neutra(&p);
    CREDO_AEQUALIS_I32 (sonitus_longitudo(&p, I), SONUS_FREQUENTIA);
    n = _cycli(&p);
    CREDO_VERUM (n >= CDXXXIX && n <= CDXLI);

    /* clivus +I octava/s: 634.78 cycli */
    _neutra(&p);
    p.clivus  = 1.0f;
    n         = _cycli(&p);
    CREDO_VERUM (n >= DCXXXIII && n <= DCXXXVI);

    /* clivus -I: CDXL / ln 2 x (1 - 0.5) = 317.39 */
    _neutra(&p);
    p.clivus  = -1.0f;
    n         = _cycli(&p);
    CREDO_VERUM (n >= CCCXVI && n <= CCCXIX);

    /* saltus ad 0.5 s, ratio II: CCXX + CDXL */
    _neutra(&p);
    p.saltus         = 2.0f;
    p.saltus_tempus  = 0.5f;
    n                = _cycli(&p);
    CREDO_VERUM (n >= DCLIX && n <= DCLXI);

    /* saltus_tempus 0 = numquam, etiam ratione II */
    _neutra(&p);
    p.saltus  = 2.0f;
    n         = _cycli(&p);
    CREDO_VERUM (n >= CDXXXIX && n <= CDXLI);

    /* vibratus symmetricus: media fere CDXL (440.4) */
    _neutra(&p);
    p.vibratus            = 1.0f;
    p.vibratus_celeritas  = 5.0f;
    n                     = _cycli(&p);
    CREDO_VERUM (n >= CDXXXVIII && n <= CDXLIII);

    /* forma extra fines coercetur (IX -> strepitus), non silentium */
    _neutra(&p);
    p.forma  = 9.0f;
    n        = (s32)sonitus_reddere(&p, I, tabula_a, TABULA_MAXIMA);
    CREDO_VERUM (n == (s32)SONUS_FREQUENTIA
                 && (tabula_a[M] != 0.0f || tabula_a[MM] != 0.0f));
}


    /* ==================================================
     * ICTUS: lucrum extra initio morae, evanescens
     * ================================================== */

interior vacuum
_sectio_ictus (
    CredoContextus* c)
{
    SonitusParametra p;
                 i32 n;
                 f32 primum;
                 f32 ultimum;

    (vacuum)c;
    /* pulsus (+1 / -1): |x| = involucrum x ictus x amplitudo */
    _neutra(&p);
    p.forma      = 0.0f;
    p.ictus      = 0.5f;
    p.amplitudo  = 0.5f;
    n            = sonitus_reddere(&p, I, tabula_a, TABULA_MAXIMA);
    CREDO_AEQUALIS_I32 (n, SONUS_FREQUENTIA);
    si (n != SONUS_FREQUENTIA)
    {
        redde;   /* i32 insignatus: n - I sub zephyro extra tabulam */
    }
    primum  = tabula_a[ZEPHYRUM] < 0.0f ? -tabula_a[ZEPHYRUM]
                                        : tabula_a[ZEPHYRUM];
    ultimum = tabula_a[n - I] < 0.0f ? -tabula_a[n - I] : tabula_a[n
        - I];
    /* initio 0.5 x (1 + 0.5) = 0.75, fine 0.5 */
    CREDO_F32_PROXIMUS (primum, 0.75f, 0.001f);
    CREDO_F32_PROXIMUS (ultimum, 0.5f, 0.001f);
    {
        f32 medium = tabula_a[n / II] < 0.0f ? -tabula_a[n / II]
                                             : tabula_a[n / II];

        CREDO_F32_PROXIMUS (medium, 0.625f, 0.001f);
    }

    /* remissio: fine sonitus prope silentium */
    _neutra(&p);
    p.forma     = 0.0f;
    p.mora      = 0.1f;
    p.remissio  = 0.1f;
    n           = sonitus_reddere(&p, I, tabula_a, TABULA_MAXIMA);
    CREDO_AEQUALIS_I32 (n, IX * M + DC);
    CREDO_VERUM (n > ZEPHYRUM
                 && tabula_a[n - I] < 0.01f
                 && tabula_a[n - I] > -0.01f);
}


    /* ==================================================
     * SINE SUTURA: cauda in caput mixta
     * ================================================== */

interior vacuum
_sectio_sine_sutura (
    CredoContextus* c)
{
     constans SonitusParametra* p = sonitus_praeparatum("crepitus");
                           i32  n;
                           i32  longitudo;

    (vacuum)c;
    si (p == NIHIL)
    {
        CREDO_VERUM (FALSUM);
        redde;
    }
    n = sonitus_reddere(p, V, tabula_b, TABULA_MAXIMA);
    longitudo = sonitus_reddere_sine_sutura(p, V, tabula_a,
        TABULA_MAXIMA,
                                            (IV * M + DCCC));
    CREDO_AEQUALIS_I32 (longitudo, n - (IV * M + DCCC));
    /* caput = exemplum quod ultimum circuli sequebatur (pondus 0) */
    CREDO_F32_PROXIMUS (tabula_a[ZEPHYRUM], tabula_b[longitudo], 0.0f);
    /* medio transitu: dimidium utriusque */
    CREDO_F32_PROXIMUS (tabula_a[MMCD],
                        0.5f * tabula_b[MMCD]
                        + 0.5f * tabula_b[longitudo + MMCD], 0.000001f);
    /* post transitum intactum */
    CREDO_F32_PROXIMUS (tabula_a[V * M], tabula_b[V * M], 0.0f);

    /* transitus >= longitudo: sicut est */
    {
        SonitusParametra parvus;

        _neutra(&parvus);
        parvus.mora = 0.01f;
        CREDO_AEQUALIS_I32 (sonitus_reddere_sine_sutura(&parvus, I,
            tabula_a,
                                                        TABULA_MAXIMA,
                                                        M),
                            CDLXXX);
    }
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "schema", _sectio_schema, _tabulas_parare, NIHIL, NIHIL, NIHIL },
    { "praeparata", _sectio_praeparata, _tabulas_parare, NIHIL, NIHIL,
        NIHIL },
    { "longitudo", _sectio_longitudo, _tabulas_parare, NIHIL, NIHIL,
        NIHIL },
    { "determinismus", _sectio_determinismus, _tabulas_parare, NIHIL,
        NIHIL, NIHIL },
    { "vox", _sectio_vox, _tabulas_parare, NIHIL, NIHIL, NIHIL },
    { "altitudo", _sectio_altitudo, _tabulas_parare, NIHIL, NIHIL,
        NIHIL },
    { "ictus", _sectio_ictus, _tabulas_parare, NIHIL, NIHIL, NIHIL },
    { "sine sutura", _sectio_sine_sutura, _tabulas_parare, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sonitus", SECTIONES);
}
