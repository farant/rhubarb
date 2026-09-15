/* coctor.c - Instrumentum: registrum coctum coquere aut rancorem
 * probare (materia/coquere.sh)
 *
 * Usus: coctor <declaratio.stml> [-scribere] [-canon X.canon]
 *   sine -scribere: declarationem contra canonem iudicat, textus
 *   reddit, contra plagulas in sedibus confert - 'recens: via' aut
 *   'RANCIDUM: via:linea' ('via: absens'); -scribere: plagulas
 *   scribit ('scriptum: via').
 *   Radix repositorii = RHUBARB_RADIX (aliter '.'): sedes et canon
 *   ordinarius (materia/grammatica/registrum.canon) ei relativa;
 *   via declarationis radice praefixa in commentarium generatum
 *   radici relativa transit.
 * Exitus: 0 recens aut scriptum; 1 RANCIDUM; 2 defectus (plagula
 *   absens, declaratio recusata, canon fractus, usus) - numquam
 *   sanitas tacita.
 */
#include "postulata_posix.h"

#include "latina.h"
#include "materia_coctor.h"
#include "canon.h"
#include "filum.h"
#include "stml.h"
#include "internamentum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* via = radix + '/' + relativa (aut relativa sola si absoluta) */
interior constans character*
_via_radice (
               Piscina* piscina,
    constans character* radix,
                chorda  relativa)
{
    character* v;
       size_t  n = strlen(radix) + (size_t)relativa.mensura + II;

    si (   relativa.mensura > ZEPHYRUM
        && relativa.datum[ZEPHYRUM] == (i8)'/')
    {
        redde chorda_ut_cstr(relativa, piscina);
    }
    v = (character*)piscina_allocare(piscina, (memoriae_index)n);
    si (v == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(v, "%s/%.*s", radix, (integer)relativa.mensura,
        (constans character*)relativa.datum);
    redde v;
}

/* via declarationis radici relativa (praefixum 'radix/' demptum) */
interior constans character*
_via_relativa (
    constans character* radix,
    constans character* via)
{
    size_t n = strlen(radix);

    si (   n > ZEPHYRUM && strncmp(via, radix, n) == ZEPHYRUM
        && via[n] == '/')
    {
        redde via + n + I;
    }
    redde via;
}

/* declarationem contra canonem iudicare: numerus vitiorum (-I =
 * canon aut declaratio parsari nequit) */
interior s32
_iudicare (
               Piscina* piscina,
    constans character* via_canonis,
    constans character* via,
                chorda  declaratio)
{
    InternamentumChorda* intern = internamentum_creare(piscina);
                 chorda  fons_canonis;
                 chorda  causa;
                  Canon* canon;
           StmlResultus  r;
                    Xar* vitia;
                    i32  j;

    fons_canonis = filum_legere_totum(via_canonis, piscina);
    si (fons_canonis.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "coctor: canon '%s' legi nequit\n",
            via_canonis);
        redde (s32)-I;
    }
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    si (canon == NIHIL)
    {
        fprintf(stderr, "coctor: canon '%s' fractus: %.*s\n",
            via_canonis, (integer)causa.mensura,
            (constans character*)causa.datum);
        redde (s32)-I;
    }
    r = stml_legere(declaratio, piscina, intern);
    si (!r.successus)
    {
        fprintf(stderr, "coctor: '%s' parsari nequit (linea %u)\n",
            via, r.linea_erroris);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, r.radix, piscina);
    per (j = ZEPHYRUM; vitia != NIHIL && j < xar_numerus(vitia); j++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, j);

        fprintf(stderr, "%s:%u: <%.*s> %s: %.*s\n", via,
            v->nodus != NIHIL ? v->nodus->linea : ZEPHYRUM,
            v->elementum != NIHIL ? (integer)v->elementum->mensura : I,
            v->elementum != NIHIL
                ? (constans character*)v->elementum->datum : "-",
            canon_nuntius(v->genus),
            v->detail != NIHIL ? (integer)v->detail->mensura : I,
            v->detail != NIHIL
                ? (constans character*)v->detail->datum : "-");
    }
    redde vitia != NIHIL ? (s32)xar_numerus(vitia) : ZEPHYRUM;
}

/* plagulam generatam contra discum conferre: VERUM recens */
interior b32
_conferre (
               Piscina* piscina,
    constans character* via,
                chorda  generatum)
{
    chorda in_disco = filum_legere_totum(via, piscina);
       i32 linea;

    si (in_disco.mensura == ZEPHYRUM)
    {
        imprimere("RANCIDUM: %s: absens\n", via);
        redde FALSUM;
    }
    si (!materia_coctio_aequalis(generatum, in_disco, &linea))
    {
        imprimere("RANCIDUM: %s:%u\n", via, linea);
        redde FALSUM;
    }
    imprimere("recens: %s\n", via);
    redde VERUM;
}

s32
principale (
          s32   numerus,
    character** argumenta)
{
               Piscina* piscina;
    constans character* radix;
    constans character* via              = NIHIL;
    constans character* canon_expressus  = NIHIL;
    constans character* via_canonis;
                   b32  scribere = FALSUM;
                   s32  i;
                chorda  declaratio;
         MateriaCoctio  coctio;
    constans character* via_capitis;
    constans character* via_fontis;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-scribere") == ZEPHYRUM)
        {
            scribere = VERUM;
        }
        alioquin si (   strcmp(argumenta[i], "-canon") == ZEPHYRUM
                     && i + I < numerus)
        {
            canon_expressus = argumenta[++i];
        }
        alioquin
        {
            via = argumenta[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: coctor <declaratio.stml> [-scribere] "
            "[-canon X.canon]\n");
        redde II;
    }
    piscina  = piscina_generare_dynamicum("coctor", 4194304);
    radix    = getenv("RHUBARB_RADIX");
    si (radix == NIHIL || radix[ZEPHYRUM] == '\0')
    {
        radix = ".";
    }
    declaratio = filum_legere_totum(via, piscina);
    si (declaratio.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "coctor: '%s' legi nequit\n", via);
        redde II;
    }
    via_canonis = canon_expressus != NIHIL ? canon_expressus
        : _via_radice(piscina, radix, chorda_ex_literis(
            "materia/grammatica/registrum.canon", piscina));
    {
        s32 vitia = _iudicare(piscina, via_canonis, via, declaratio);

        si (vitia != ZEPHYRUM)
        {
            fprintf(stderr, "coctor: declaratio '%s' RECUSATA "
                "(canon %s)\n", via, via_canonis);
            redde II;
        }
    }
    si (!materia_registrum_coquere(piscina, declaratio,
            _via_relativa(radix, via), &coctio))
    {
        fprintf(stderr, "coctor: declaratio '%s' RECUSATA: %.*s "
            "(linea %u)\n", via, (integer)coctio.causa.mensura,
            coctio.causa.datum != NIHIL
                ? (constans character*)coctio.causa.datum : "memoria",
            coctio.linea);
        redde II;
    }
    via_capitis  = _via_radice(piscina, radix, coctio.via_capitis);
    via_fontis   = _via_radice(piscina, radix, coctio.via_fontis);
    si (via_capitis == NIHIL || via_fontis == NIHIL)
    {
        redde II;
    }
    si (scribere)
    {
        si (   !filum_scribere(via_capitis, coctio.caput)
            || !filum_scribere(via_fontis, coctio.fons))
        {
            fprintf(stderr, "coctor: scriptura fracta (%s, %s)\n",
                via_capitis, via_fontis);
            redde II;
        }
        imprimere("scriptum: %s\nscriptum: %s\n", via_capitis,
            via_fontis);
        redde ZEPHYRUM;
    }
    {
        b32 caput_recens = _conferre(piscina, via_capitis,
            coctio.caput);
        b32 fons_recens = _conferre(piscina, via_fontis, coctio.fons);

        redde (caput_recens && fons_recens) ? ZEPHYRUM : I;
    }
}
