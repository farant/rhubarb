/* probatio_silva_fidelitas.c - Fidelitas octetorum trans corpus laminarum
 *
 * DUO oracula super OMNES laminas in probationes/fixa/roundtrip/
 * (corpus commune generationum priorum - 78+ plagulae; quaeque
 * cicatrix defectus arbor v1/v2 aut fons verus rhubarb):
 *   1. emittere(lexare(x)) == x        (lexator, Phase 1)
 *   2. scribere(parsare(x)) == x       (ARBOR, Phase 5 Chunk C)
 * Oraculum arboris fistulam TOTAM currit: praeprocessor (macros
 * quas plagula ipsa definit expanduntur - limes expansionis!),
 * recuperatio (C89 verum sceleto maiore parte nodi ERROR fiunt),
 * reinserenda (lineae directivae, rami non sumpti), reconstructio
 * strati 0. Includenda ignota manent (modus discens) - lineae
 * earum captae et reinsertae.
 *
 * Radix repositorii per RHUBARB_RADIX (compile_probationes.sh eam
 * praebet); defaltum ".." pro cursu manuali ex silva/.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>  /* probatio solum - macOS/POSIX */

#define VIA_MAXIMA 1024

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

/* Legere plagulam totam; reddit NIHIL in defectu */
interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)(mensura
        + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    *mensura_out = (i32)mensura;
    redde buffer;
}

/* Probare fidelitatem unius plagulae per DUO oracula */
interior vacuum
_fidelitatem_probare (
    constans character* via,
                   b32* lex_out,
                   b32* arbor_out)
{
    Piscina* piscina;
         i8* fons;
        Xar* lexemata;
     chorda  emissa;
        i32  mensura;

    *lex_out    = FALSUM;
    *arbor_out  = FALSUM;

    piscina = piscina_generare_dynamicum("fidelitas_plagulae",
        16777216);
    si (piscina == NIHIL)
    {
        redde;
    }

    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        piscina_destruere(piscina);
        redde;
    }

    /* Oraculum 1: lexator */
    lexemata = silva_lexare(piscina, (constans character*)fons, mensura,
        ZEPHYRUM);
    emissa = silva_lexemata_emittere(piscina, lexemata);
    *lex_out = (emissa.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.datum, fons, (memoriae_index)mensura)
                == ZEPHYRUM)) ? VERUM : FALSUM;

    /* Oraculum 2: arbor - fistula tota + reconstructio strati 0 */
    {
        SilvaParsura* parsura;

        parsura = silva_parsare(piscina, via,
            (constans character*)fons, mensura, &GRAMMATICA_SCELETI,
            NIHIL, NIHIL, NIHIL);
        si (parsura != NIHIL && parsura->successus)
        {
            SilvaScriptura scriptura;

            scriptura = silva_scribere_fontem(piscina, parsura,
                &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
            *arbor_out = (scriptura.successus
                && scriptura.textus.mensura == mensura
                && (mensura == ZEPHYRUM
                    || memcmp(scriptura.textus.datum, fons,
                           (memoriae_index)mensura) == ZEPHYRUM))
                ? VERUM : FALSUM;
        }
    }

    piscina_destruere(piscina);
}

interior b32
_est_c_vel_h (
    constans character* titulus)
{
    memoriae_index m;

    m = strlen(titulus);
    si (m < III)
    {
        redde FALSUM;
    }
    si (   titulus[m - II] == '.'
        && (titulus[m - I] == 'c' || titulus[m - I] == 'h'))
    {
        redde VERUM;
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix;
             character  via_corporis[VIA_MAXIMA];
             character  via_plagulae[VIA_MAXIMA];
    DIR* corpus;
    structura dirent* introitus;
    i32 numerus;
    i32 fideles;
    i32 fideles_arboris;

    piscina = piscina_generare_dynamicum("probatio_silva_fidelitas",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);

    imprimere("\n--- Probans fidelitatem corporis laminarum ---\n");
    imprimere("  corpus: %s\n", via_corporis);

    numerus          = ZEPHYRUM;
    fideles          = ZEPHYRUM;
    fideles_arboris  = ZEPHYRUM;
    corpus           = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus non apertum: %s\n", via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }

    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        b32 fidelis_lex;
        b32 fidelis_arboris;

        si (!_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis, introitus->d_name);

        _fidelitatem_probare(via_plagulae, &fidelis_lex,
            &fidelis_arboris);
        numerus++;
        si (fidelis_lex)
        {
            fideles++;
        }
        si (fidelis_arboris)
        {
            fideles_arboris++;
        }
        si (!fidelis_lex || !fidelis_arboris)
        {
            imprimere("  INFIDELIS%s: %s\n",
                fidelis_lex ? " (arbor)" : "", introitus->d_name);
        }
        CREDO_VERUM (fidelis_lex);
        CREDO_VERUM (fidelis_arboris);
    }
    closedir(corpus);

    imprimere("  plagulae: %d, fideles (lex): %d, fideles (arbor): %d\n",
        numerus, fideles, fideles_arboris);

    /* corpus integrum inventum (LXXVIII cum scriptum; crescere licet) */
    CREDO_VERUM (numerus >= LXXVIII);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
