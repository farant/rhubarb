/* dispositio_oraculum.c - ductor oraculi dispositionis (D0;
 * project-specs/dispositio-plan.md)
 *
 * Arborem STML legit, Clay (per clay_vinculum) eam disponit, fines
 * cuiusque nodi ordine praeordinis imprimit:
 *   nodus <index> <x> <y> <latitudo> <altitudo>     (%g, ut Clay dat)
 * Forma arboris (eadem quam probationes dispositionis legent):
 *   <dispositio latitudo="80" altitudo="24"> <nodus .../> ... </dispositio>
 *   nodus: directio="linea|columna"
 *          latitudo, altitudo = apta[:min:max] | crescens[:min:max]
 *                               | fixa:n | pars:centesimae
 *          spatium="s d sup inf"  intervallum="n"
 *          allineatio_x, allineatio_y = initium|medium|finis
 *          praecidere_x, praecidere_y = "verum"
 *          textus="..." (D3: textus Clay primus liber, sine involutione;
 *          mensura runae GRAPHEMATUM - eadem quam probationes adhibent)
 * Radix implicita (Clay): linea, magnitudine superficiei.
 * Usus: dispositio_oraculum <via.stml>   Exitus: 0; 1 forma mala aut
 * error Clay; 2 usus malus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "stml.h"
#include "runae.h"
#include "clay_vinculum.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina_ductoris;
hic_manens     s32  numerus_nodorum;

/* Mensor communis (D3): runae sub politica GRAPHEMATUM */
interior int
_mensor (
    constans char* octeti,
               int  mensura)
{
    redde (int)runae_latitudo_textus((constans i8*)octeti,
        (constans i8*)octeti + mensura, RUNAE_POLITICA_GRAPHEMATUM);
}

interior chorda*
_attributum (
              StmlNodus* nodus,
    constans character* titulus)
{
    redde stml_attributum_capere(nodus, titulus);
}

interior b32
_aequat (
                 chorda* c,
    constans character* literae)
{
    redde (c && chorda_aequalis_literis(*c, literae)) ? VERUM : FALSUM;
}

/* "apta[:min:max]" "crescens[:min:max]" "fixa:n" "pars:c" */
interior b32
_axis (
              StmlNodus* nodus,
    constans character* titulus,
                    int* genus,
                  float* valor,
                  float* minimum,
                  float* maximum)
{
                   chorda* a = _attributum(nodus, titulus);
    chorda_fissio_fructus  partes;
                      s32  n;

    *genus    = VINCULUM_APTA;
    *valor    = 0.0f;
    *minimum  = 0.0f;
    *maximum  = 0.0f;
    si (!a)
    {
        redde VERUM;
    }
    partes = chorda_fissio(*a, ':', piscina_ductoris);
    si (partes.numerus < I)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(partes.elementa[0], "apta"))
    {
        *genus = VINCULUM_APTA;
    }
    alioquin si (chorda_aequalis_literis(partes.elementa[0], "crescens"))
    {
        *genus = VINCULUM_CRESCENS;
    }
    alioquin si (chorda_aequalis_literis(partes.elementa[0], "fixa")
        && partes.numerus == II && chorda_ut_s32(partes.elementa[1], &n))
    {
        *genus  = VINCULUM_FIXA;
        *valor  = (float)n;
        redde VERUM;
    }
    alioquin si (chorda_aequalis_literis(partes.elementa[0], "pars")
        && partes.numerus == II && chorda_ut_s32(partes.elementa[1], &n))
    {
        *genus  = VINCULUM_PARS;
        *valor  = (float)n / 100.0f;
        redde VERUM;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (partes.numerus == III)
    {
        s32 mi;
        s32 ma;

        si (!chorda_ut_s32(partes.elementa[1], &mi)
            || !chorda_ut_s32(partes.elementa[2], &ma))
        {
            redde FALSUM;
        }
        *minimum  = (float)mi;
        *maximum  = (float)ma;
    }
    alioquin si (partes.numerus != I)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior int
_allineatio (
              StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a = _attributum(nodus, titulus);

    si (_aequat(a, "medium"))
    {
        redde VINCULUM_MEDIUM;
    }
    si (_aequat(a, "finis"))
    {
        redde VINCULUM_FINIS;
    }
    redde VINCULUM_INITIUM;
}

interior int
_numerus (
              StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a = _attributum(nodus, titulus);
       s32  n = ZEPHYRUM;

    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &n);
    }
    redde (int)n;
}

/* Nodum et liberos (praeordine) in Clay aperire */
interior b32
_ponere (
    StmlNodus* nodus)
{
    VinculumForma  f;
           chorda* spatium;
              s32  index;
              i32  i;

    memset(&f, ZEPHYRUM, magnitudo(VinculumForma));
    f.directio = _aequat(_attributum(nodus, "directio"), "columna")
        ? I : ZEPHYRUM;
    si (!_axis(nodus, "latitudo", &f.genus_x, &f.valor_x, &f.minimum_x,
            &f.maximum_x)
        || !_axis(nodus, "altitudo", &f.genus_y, &f.valor_y,
            &f.minimum_y, &f.maximum_y))
    {
        fprintf(stderr, "dispositio_oraculum: mensura mala (linea %d)\n",
            (int)nodus->linea);
        redde FALSUM;
    }
    spatium = _attributum(nodus, "spatium");
    si (spatium)
    {
        chorda_fissio_fructus p = chorda_fissio(*spatium, ' ',
            piscina_ductoris);
        s32 v[IV];

        si (p.numerus != IV
            || !chorda_ut_s32(p.elementa[0], &v[0])
            || !chorda_ut_s32(p.elementa[1], &v[1])
            || !chorda_ut_s32(p.elementa[2], &v[2])
            || !chorda_ut_s32(p.elementa[3], &v[3]))
        {
            fprintf(stderr, "dispositio_oraculum: spatium malum (linea %d)\n",
                (int)nodus->linea);
            redde FALSUM;
        }
        f.spatium_sinistrum  = (int)v[0];
        f.spatium_dextrum    = (int)v[1];
        f.spatium_superum    = (int)v[2];
        f.spatium_inferum    = (int)v[3];
    }
    f.intervallum   = _numerus(nodus, "intervallum");
    f.allineatio_x  = _allineatio(nodus, "allineatio_x");
    f.allineatio_y  = _allineatio(nodus, "allineatio_y");
    f.praecidere_x  = _aequat(_attributum(nodus, "praecidere_x"), "verum");
    f.praecidere_y  = _aequat(_attributum(nodus, "praecidere_y"), "verum");

    index = numerus_nodorum++;
    vinculum_aperire((int)index, &f);
    {
        chorda* textus = _attributum(nodus, "textus");

        si (textus && textus->mensura > ZEPHYRUM)
        {
            vinculum_textum((constans char*)textus->datum,
                (int)textus->mensura);
        }
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(nodus); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(nodus, i);

        si (l && l->genus == STML_NODUS_ELEMENTUM && l->titulus
            && chorda_aequalis_literis(*l->titulus, "nodus"))
        {
            si (!_ponere(l))
            {
                redde FALSUM;
            }
        }
    }
    vinculum_claudere();
    redde VERUM;
}

s32
principale (
      integer   argc,
    character** argv)
{
    InternamentumChorda* intern;
                 chorda  fons;
           StmlResultus  res;
             StmlNodus* radix;
                    i32  i;
                    s32  latitudo;
                    s32  altitudo;
                    s32  k;

    si (argc != II)
    {
        fprintf(stderr, "usus: dispositio_oraculum <via.stml>\n");
        redde II;
    }
    piscina_ductoris = piscina_generare_dynamicum("dispositio_oraculum",
        M * M);
    intern = internamentum_creare(piscina_ductoris);
    fons = filum_legere_totum(argv[I], piscina_ductoris);
    res = stml_legere(fons, piscina_ductoris, intern);
    radix = res.successus ? res.elementum_radix : NIHIL;
    si (!radix || !radix->titulus
        || !chorda_aequalis_literis(*radix->titulus, "dispositio"))
    {
        fprintf(stderr, "dispositio_oraculum: radix <dispositio> deest: %s\n",
            argv[I]);
        redde I;
    }
    latitudo  = (s32)_numerus(radix, "latitudo");
    altitudo  = (s32)_numerus(radix, "altitudo");
    si (!vinculum_initiare((float)latitudo, (float)altitudo))
    {
        redde I;
    }
    vinculum_mensorem_ponere(_mensor);
    vinculum_incipere();
    numerus_nodorum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(radix, i);

        si (l && l->genus == STML_NODUS_ELEMENTUM && l->titulus
            && chorda_aequalis_literis(*l->titulus, "nodus")
            && !_ponere(l))
        {
            redde I;
        }
    }
    si (!vinculum_finire())
    {
        redde I;
    }
    per (k = ZEPHYRUM; k < numerus_nodorum; k++)
    {
        float x;
        float y;
        float w;
        float h;

        si (!vinculum_fines((int)k, &x, &y, &w, &h))
        {
            fprintf(stderr, "dispositio_oraculum: nodus %d non inventus\n",
                (int)k);
            redde I;
        }
        printf("nodus %d %g %g %g %g\n", (int)k, (double)x, (double)y,
            (double)w, (double)h);
    }
    redde ZEPHYRUM;
}
