/* facies.c - Instrumentum diagnosticorum crustae (crusta/facies.sh)
 *
 * Usus:
 *   facies <plagula...> [-machina] [-cruda] [-sine-excusatione]
 *                       [-sine-excerpto]
 *
 * Exitus: 0 nullum erratum | 1 erratum | 2 nihil iudicatum
 *
 * DEMONSTRATIO FACIEI: hoc 'crusta_facies.h' SOLUM includit - nec
 * materiam, nec registrum, nec lexicon, nec stml. Si consumptor
 * quilibet plus poscere deberet, facies officium suum non faceret.
 *
 * '-cruda' subtractiones declaratas omittit (bracchia ut auctor
 * regulae ea scripsit); '-sine-excusatione' '<tolera>' omittit. AMBO
 * PORTAE DIFFERENTIAE SERVIUNT: extractor prior neutrum agebat, ergo
 * comparatio sine his numeros disparis compararet.
 *
 * REGULAE SEMEL LECTAE: piscina regularum plagulas omnes superstes
 * est, et internamentum idem manet - lex internamenti (materia/
 * CLAUDE.md) aliter exemplaria OMNIA MUTA redderet. Piscina plagulae
 * post quamque delet, aliter corpus domus memoriam exhauriret.
 */

#include "latina.h"
#include "crusta_facies.h"
#include "materia_pictor.h"
#include "internamentum.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i32 errata;
    i32 monita;
    i32 plagulae;
} Summa;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

/* FALSUM = plagula iudicari non potuit (causa nominata) */
interior b32
_plagulam_iudicare (
      constans character* via,
 constans CrustaOptiones* optiones,
                     b32  machina,
                     b32  excerptum,
                   Summa* summa)
{
              Piscina* piscina;
            character* fons;
                  i32  mensura = ZEPHYRUM;
                  Xar* d;
   constans character* causa = NIHIL;
                  i32  k;

    piscina = piscina_generare_dynamicum("facies", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "facies: piscina deficit\n");
        redde FALSUM;
    }
    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "facies: plagula legi non potest: %s\n", via);
        piscina_destruere(piscina);
        redde FALSUM;
    }
    d = crusta_diagnostica_omnia(piscina, fons, mensura, optiones,
            &causa);
    si (d == NIHIL)
    {
        fprintf(stderr, "facies: %s: %s\n", via,
            causa != NIHIL ? causa : "causa ignota");
        piscina_destruere(piscina);
        redde FALSUM;
    }
    summa->plagulae++;
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        constans MateriaDiagnosticum* x =
            (constans MateriaDiagnosticum*)xar_obtinere(d, k);
        chorda textus;

        si (x->gravitas == (s32)MATERIA_GRAVITAS_MONITUM)
        {
            summa->monita++;
        }
        alioquin
        {
            summa->errata++;
        }
        textus = machina
            ? materia_pictor_machina(piscina, x, via)
            : materia_pictor_scribere(piscina, x, via, "crusta", fons,
                  mensura, excerptum);
        si (textus.mensura > ZEPHYRUM)
        {
            fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
        }
    }
    piscina_destruere(piscina);
    redde VERUM;
}

integer
principale (
    integer   argc,
  character** argv)
{
                Summa  summa;
       CrustaOptiones  optiones;
              Piscina* regularum;
  InternamentumChorda* intern;
   constans character* causa      = NIHIL;
                  b32  machina    = FALSUM;
                  b32  excerptum  = VERUM;
              integer  i;
                  b32  ulla    = FALSUM;
                  b32  fracta  = FALSUM;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine-excerpto") == ZEPHYRUM)
        {
            excerptum = FALSUM;
        }
        alioquin si (strcmp(argv[i], "-cruda") == ZEPHYRUM)
        {
            optiones.crudum = VERUM;
        }
        alioquin si (strcmp(argv[i], "-sine-excusatione") == ZEPHYRUM)
        {
            optiones.sine_excusatione = VERUM;
        }
        alioquin si (argv[i][ZEPHYRUM] == '-')
        {
            fprintf(stderr, "facies: optio ignota: %s\n", argv[i]);
            redde (integer)II;
        }
    }
    /* REGULAE SEMEL, internamento uno - vide caput */
    regularum = piscina_generare_dynamicum("facies_regulae", 4194304);
    si (regularum == NIHIL)
    {
        fprintf(stderr, "facies: piscina regularum deficit\n");
        redde (integer)II;
    }
    intern = internamentum_creare(regularum);
    si (intern == NIHIL)
    {
        fprintf(stderr, "facies: internamentum deficit\n");
        redde (integer)II;
    }
    optiones.intern   = intern;
    optiones.regulae  = crusta_regulae_legere(regularum,
                            CRUSTA_LINTRUM, intern, &causa);
    si (optiones.regulae == NIHIL)
    {
        fprintf(stderr, "facies: %s\n",
            causa != NIHIL ? causa : "regulae legi non possunt");
        redde (integer)II;
    }
    /* REGULAE ZERO INVENTA ZERO REDDERENT, ET ID A SANITATE NON
     * DIFFERT - ergo refutatio nominata, non numerus tacitus. */
    si (xar_numerus(optiones.regulae) == ZEPHYRUM)
    {
        fprintf(stderr, "facies: regula nulla in %s\n",
            CRUSTA_LINTRUM);
        redde (integer)II;
    }
    per (i = I; i < argc; i++)
    {
        si (argv[i][ZEPHYRUM] == '-')
        {
            perge;
        }
        ulla = VERUM;
        si (!_plagulam_iudicare(argv[i], &optiones, machina, excerptum,
                &summa))
        {
            fracta = VERUM;
        }
    }
    si (!ulla)
    {
        fprintf(stderr, "usus: facies <plagula...> [-machina] "
            "[-cruda] [-sine-excusatione] [-sine-excerpto]\n");
        redde (integer)II;
    }
    si (fracta)
    {
        redde (integer)II;
    }
    redde summa.errata > ZEPHYRUM ? (integer)I : (integer)ZEPHYRUM;
}
