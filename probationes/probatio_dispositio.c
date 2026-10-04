/* probatio_dispositio.c - dispositio contra oraculum Clay (D1)
 *
 * Fixa oracula/clay/probationes/d1_*.stml (arbores) et .exspectata
 * (fines a Clay @ e6cc369 dati, commissi - probatio clonum non
 * requirit; tools/dispositio_oraculum.sh -probare ea contra Clay vivum
 * iterum confert). REGULA (D2, una pro omnibus): omnis ORA (x, x+w,
 * y, y+h) nostra == pavimentum(ora Clay + 0.02) - medium et pars (D1)
 * et divisio crescentium/contractionis (D2: ora pavimentum orae
 * exactae)
 * eadem regula tenentur; 0.02 strepitum fluitantem absorbet (0.7f, et
 * aequatio Clay quae ad epsilon 0.01 convergit, 5.99 pro 6). Omnia fixa
 * etiam manu verificata (phase log).
 * Praeterea: forma ordinaria, parens ignotus, vacare, computare bis.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "stml.h"
#include "dispositio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ---- forma arboris (eadem ac ductor oraculi) ---- */

interior b32
_aequat (
                 chorda* c,
     constans character* literae)
{
    redde (c && chorda_aequalis_literis(*c, literae)) ? VERUM : FALSUM;
}

interior b32
_mensura (
                StmlNodus* nodus,
       constans character* titulus,
        DispositioMensura* m,
                  Piscina* p)
{
                   chorda* a = stml_attributum_capere(nodus, titulus);
    chorda_fissio_fructus  partes;
                      s32  n;

    si (!a)
    {
        redde VERUM;
    }
    partes = chorda_fissio(*a, ':', p);
    si (partes.numerus < I)
    {
        redde FALSUM;
    }
    si (   chorda_aequalis_literis(partes.elementa[0], "fixa")
        && partes.numerus == II
        && chorda_ut_s32(partes.elementa[1], &n))
    {
        m->genus = DISPOSITIO_FIXA;
        m->valor = n;
        redde VERUM;
    }
    si (   chorda_aequalis_literis(partes.elementa[0], "pars")
        && partes.numerus == II
        && chorda_ut_s32(partes.elementa[1], &n))
    {
        m->genus = DISPOSITIO_PARS;
        m->valor = n;
        redde VERUM;
    }
    si (chorda_aequalis_literis(partes.elementa[0], "apta"))
    {
        m->genus = DISPOSITIO_APTA;
    }
    alioquin si (chorda_aequalis_literis(partes.elementa[0],
                 "crescens"))
    {
        m->genus = DISPOSITIO_CRESCENS;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (partes.numerus == III)
    {
        redde (chorda_ut_s32(partes.elementa[1], &m->minimum)
            && chorda_ut_s32(partes.elementa[2], &m->maximum))
            ? VERUM : FALSUM;
    }
    redde (partes.numerus == I) ? VERUM : FALSUM;
}

interior DispositioAllineatio
_allineatio (
              StmlNodus* nodus,
     constans character* titulus)
{
    chorda* a = stml_attributum_capere(nodus, titulus);

    si (_aequat(a, "medium"))
    {
        redde DISPOSITIO_MEDIUM;
    }
    si (_aequat(a, "finis"))
    {
        redde DISPOSITIO_FINIS;
    }
    redde DISPOSITIO_INITIUM;
}

interior b32
_ponere (
     Dispositio* d,
            s32  parens,
      StmlNodus* nodus,
        Piscina* p)
{
    DispositioForma  f;
             chorda* a;
                s32  index;
                i32  i;

    dispositio_formam_initiare(&f);
    si (_aequat(stml_attributum_capere(nodus, "directio"), "columna"))
    {
        f.directio = DISPOSITIO_COLUMNA;
    }
    si (   !_mensura(nodus, "latitudo", &f.latitudo, p)
        || !_mensura(nodus, "altitudo", &f.altitudo, p))
    {
        redde FALSUM;
    }
    a = stml_attributum_capere(nodus, "spatium");
    si (a)
    {
        chorda_fissio_fructus s = chorda_fissio(*a, ' ', p);

        si (   s.numerus != IV
            || !chorda_ut_s32(s.elementa[0], &f.spatium_sinistrum)
            || !chorda_ut_s32(s.elementa[1], &f.spatium_dextrum)
            || !chorda_ut_s32(s.elementa[2], &f.spatium_superum)
            || !chorda_ut_s32(s.elementa[3], &f.spatium_inferum))
        {
            redde FALSUM;
        }
    }
    a = stml_attributum_capere(nodus, "intervallum");
    si (a && !chorda_ut_s32(*a, &f.intervallum))
    {
        redde FALSUM;
    }
    f.allineatio_x = _allineatio(nodus, "allineatio_x");
    f.allineatio_y = _allineatio(nodus, "allineatio_y");
    f.praecidere_x  = _aequat(stml_attributum_capere(nodus,
        "praecidere_x"), "verum");
    f.praecidere_y  = _aequat(stml_attributum_capere(nodus,
        "praecidere_y"), "verum");
    index = dispositio_addere(d, parens, &f);
    si (index < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (   l && l->genus == STML_NODUS_ELEMENTUM && l->titulus
            && chorda_aequalis_literis(*l->titulus, "nodus")
            && !_ponere(d, index, l, p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Fixum unum: arbor -> dispositio -> conferre cum .exspectata */
interior b32
_fixum (
                 Piscina* p,
     InternamentumChorda* intern,
      constans character* titulus)
{
           character  via[CCLVI];
              chorda  fons;
        StmlResultus  res;
          Dispositio* d;
                 s32  latitudo = ZEPHYRUM;
                 s32  altitudo = ZEPHYRUM;
              chorda* a;
                 i32  i;
               FILE* exs;
           character  linea[CCLVI];
                 b32  bona = VERUM;
                 s32  lineae = ZEPHYRUM;

    sprintf(via, "oracula/clay/probationes/%s.stml", titulus);
    fons  = filum_legere_totum(via, p);
    res   = stml_legere(fons, p, intern);
    si (!res.successus || !res.elementum_radix)
    {
        imprimere("  FRACTA: %s non legitur\n", via);
        redde FALSUM;
    }
    a = stml_attributum_capere(res.elementum_radix, "latitudo");
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &latitudo);
    }
    a = stml_attributum_capere(res.elementum_radix, "altitudo");
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &altitudo);
    }
    d = dispositio_creare(p);
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(res.elementum_radix);
         i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(res.elementum_radix, i);

        si (   l && l->genus == STML_NODUS_ELEMENTUM && l->titulus
            && chorda_aequalis_literis(*l->titulus, "nodus")
            && !_ponere(d, -I, l, p))
        {
            imprimere("  FRACTA: %s forma mala\n", via);
            redde FALSUM;
        }
    }
    dispositio_computare(d, latitudo, altitudo, NIHIL, NIHIL);

    sprintf(via, "oracula/clay/probationes/%s.exspectata", titulus);
    exs = fopen(via, "r");
    si (!exs)
    {
        imprimere("  FRACTA: %s deest\n", via);
        redde FALSUM;
    }
    dum (fgets(linea, (int)magnitudo(linea), exs))
    {
           int k;
        double v[IV];
           s32 e[IV];
         Fines f;

        si (sscanf(linea, "nodus %d %lf %lf %lf %lf", &k, &v[0], &v[1],
                &v[2], &v[3]) != V)
        {
            perge;
        }
        lineae++;
        /* orae: sinistra, superior, dextra, inferior */
        e[0]  = (s32)floor(v[0] + 0.02);
        e[1]  = (s32)floor(v[1] + 0.02);
        e[2]  = (s32)floor(v[0] + v[2] + 0.02);
        e[3]  = (s32)floor(v[1] + v[3] + 0.02);
        f     = dispositio_fines(d, (s32)k);
        si (   f.x != e[0] || f.y != e[1] || f.x + f.latitudo != e[2]
            || f.y + f.altitudo != e[3])
        {
            imprimere("  FRACTA: %s nodus %d: orae nostrae %d %d %d %d,"
                " Clay %d %d %d %d\n", titulus, k, (int)f.x, (int)f.y,
                (int)(f.x + f.latitudo), (int)(f.y + f.altitudo),
                (int)e[0], (int)e[1], (int)e[2], (int)e[3]);
            bona = FALSUM;
        }
    }
    fclose(exs);
    si (lineae != dispositio_numerus(d))
    {
        imprimere("  FRACTA: %s: %d lineae, %d nodi\n", titulus,
            (int)lineae, (int)dispositio_numerus(d));
        bona = FALSUM;
    }
    redde bona;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_dispositio",
        IV * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- I. fixa D1 contra Clay ---\n");
    CREDO_VERUM (_fixum(piscina, intern, "d1_linea_fixa"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_allineatio"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_apta"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_pars"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_minmax"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_praecidere"));
    CREDO_VERUM (_fixum(piscina, intern, "d1_vacuum"));

    imprimere("\n--- II. fixa D2 (crescens, contractio) contra Clay"
              " ---\n");
    CREDO_VERUM (_fixum(piscina, intern, "columna"));
    CREDO_VERUM (_fixum(piscina, intern, "tres_crescentes"));
    CREDO_VERUM (_fixum(piscina, intern, "d2_inaequales"));
    CREDO_VERUM (_fixum(piscina, intern, "d2_maximum"));
    CREDO_VERUM (_fixum(piscina, intern, "d2_contractio"));
    CREDO_VERUM (_fixum(piscina, intern, "d2_praecidere"));
    CREDO_VERUM (_fixum(piscina, intern, "d2_minimum"));

    imprimere("\n--- III. forma, addere, vacare, computare bis ---\n");
    {
        DispositioForma  f;
             Dispositio* d = dispositio_creare(piscina);
                    s32  a;
                    s32  b;
                  Fines  f1;
                  Fines  f2;

        dispositio_formam_initiare(&f);
        CREDO_AEQUALIS_I32 ((i32)f.latitudo.genus,
            (i32)DISPOSITIO_APTA);
        CREDO_AEQUALIS_I32 ((i32)f.directio, (i32)DISPOSITIO_LINEA);
        CREDO_AEQUALIS_S32 (f.intervallum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (dispositio_addere(d, V, &f), -I);
        CREDO_AEQUALIS_S32 (dispositio_addere(d, -I, NIHIL), -I);
        f.latitudo.genus  = DISPOSITIO_FIXA;
        f.latitudo.valor  = X;
        f.altitudo.genus  = DISPOSITIO_FIXA;
        f.altitudo.valor  = III;
        a                 = dispositio_addere(d, -I, &f);
        b                 = dispositio_addere(d, a, &f);
        CREDO_AEQUALIS_S32 (a, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (b, I);
        CREDO_AEQUALIS_S32 (dispositio_numerus(d), II);
        dispositio_computare(d, XL, X, NIHIL, NIHIL);
        f1 = dispositio_fines(d, b);
        dispositio_computare(d, XL, X, NIHIL, NIHIL);
        f2 = dispositio_fines(d, b);
        CREDO_VERUM (f1.x == f2.x && f1.y == f2.y
            && f1.latitudo == f2.latitudo
            && f1.altitudo == f2.altitudo);
        CREDO_AEQUALIS_S32 (f2.latitudo, X);
        f1 = dispositio_fines(d, VII);
        CREDO_AEQUALIS_S32 (f1.latitudo, ZEPHYRUM);
        dispositio_vacare(d);
        CREDO_AEQUALIS_S32 (dispositio_numerus(d), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (dispositio_addere(d, -I, &f), ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
