/* probatio_silva_solarium.c - Corpus solarii (Phase 7 Chunk C,
 * anulus corporis 1.5)
 *
 * ../solarium: ~148 plagulae C89 planae SINE latina - corpus
 * alienum verum. Vectis: OMNIS plagula per fistulam integram
 * (parsare cum contextu, capitibus praebitis -> scribere) octetim
 * exacta. Numerus honestus die 2026-07-03 fixus: 148/148, regiones
 * textae 56 / omissae 9, PROSSER imparia 0.
 *
 * Solarium NON vendicatur (iuxta vivit, evolvitur - decisio
 * memorata): absens -> probatio VIRIDIS cum nota clara. Numeri
 * regionum evolvuntur cum corpore - assertiones RATIONEM (omnes
 * fideles) figunt, non numeros crudos.
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
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define VIA_MAXIMA 1024
#define PLAGULAE_MAXIMAE 512

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

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
    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
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
    buffer[mensura]  = '\0';
    *mensura_out     = (i32)mensura;
    redde buffer;
}

interior b32
_suffixum_habet (
    constans character* titulus,
    constans character* suffixum)
{
    memoriae_index lt = strlen(titulus);
    memoriae_index ls = strlen(suffixum);

    si (lt < ls)
    {
        redde FALSUM;
    }
    redde (strcmp(titulus + (lt - ls), suffixum) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

s32 principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
        SilvaContextus* contextus;
    constans character* radix_env;
             character  via_solarii[VIA_MAXIMA];
             character  via[VIA_MAXIMA];
             character* nomina[PLAGULAE_MAXIMAE];
                   i32  numerus_plagularum  = ZEPHYRUM;
                   i32  fideles             = ZEPHYRUM;
                   i32  summa_textarum      = ZEPHYRUM;
                   i32  summa_omissarum     = ZEPHYRUM;
                   i32  k;

    piscina = piscina_generare_dynamicum("probatio_solarium",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    radix_env = getenv("RHUBARB_RADIX");
    si (radix_env == NIHIL)
    {
        radix_env = "..";
    }
    sprintf(via_solarii, "%s/../solarium", radix_env);

    /* Nomina colligere; solarium absens = VIRIDIS cum nota */
    {
        DIR* dir = opendir(via_solarii);
        structura dirent* introitus;

        si (dir == NIHIL)
        {
            imprimere("\n--- SOLARIUM ABEST (%s) - probatio "
                "praetermissa (corpus iuxta vivit, non vendicatur) "
                "---\n", via_solarii);
            credo_imprimere_compendium();
            piscina_destruere(piscina);
            redde ZEPHYRUM;
        }
        dum (   (introitus = readdir(dir)) != NIHIL
             && numerus_plagularum < PLAGULAE_MAXIMAE)
        {
            si (   _suffixum_habet(introitus->d_name, ".c")
                || _suffixum_habet(introitus->d_name, ".h"))
            {
                memoriae_index m = strlen(introitus->d_name) + I;
                character* nomen_novum = (character*)piscina_allocare(
                    piscina, m);

                si (nomen_novum != NIHIL)
                {
                    memcpy(nomen_novum, introitus->d_name, m);
                    nomina[numerus_plagularum++] = nomen_novum;
                }
            }
        }
        closedir(dir);
    }

    imprimere("\n--- Probans corpus solarii (%d plagulae) ---\n",
        (int)numerus_plagularum);
    CREDO_MAIOR_I32 (numerus_plagularum, C);

    /* Contextus: clausura capitum (receptum unum, parsurae multae) */
    contextus = silva_contextus_creare(piscina);
    CREDO_NON_NIHIL (contextus);
    per (k = ZEPHYRUM; k < numerus_plagularum; k++)
    {
         i8* textus;
        i32  mensura;

        si (!_suffixum_habet(nomina[k], ".h"))
        {
            perge;
        }
        sprintf(via, "%s/%s", via_solarii, nomina[k]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus != NIHIL)
        {
            silva_contextus_praebere(contextus, nomina[k],
                (constans character*)textus, mensura);
        }
    }

    /* Vectis: quaeque plagula octetim exacta */
    per (k = ZEPHYRUM; k < numerus_plagularum; k++)
    {
               Piscina* piscina_plagulae;
                    i8* textus;
                   i32  mensura;
          SilvaParsura* parsura;
        SilvaScriptura  scriptura;
                   b32  fidelis = FALSUM;

        sprintf(via, "%s/%s", via_solarii, nomina[k]);
        piscina_plagulae = piscina_generare_dynamicum("plagula",
            268435456);
        si (piscina_plagulae == NIHIL)
        {
            perge;
        }
        textus = _plagulam_legere(piscina_plagulae, via, &mensura);
        si (textus != NIHIL)
        {
            parsura = silva_parsare_cum_contextu(piscina_plagulae,
                contextus, nomina[k], (constans character*)textus,
                mensura, &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
            si (parsura != NIHIL && parsura->successus)
            {
                scriptura = silva_scribere_fontem(piscina_plagulae,
                    parsura, &SILVA_SCELETUM_REGISTRUM,
                    parsura->fons_princeps);
                si (   scriptura.successus
                    && scriptura.textus.mensura == (i32)mensura
                    && (mensura == ZEPHYRUM
                        || memcmp(scriptura.textus.datum, textus,
                               (memoriae_index)mensura) == ZEPHYRUM))
                {
                    fidelis = VERUM;
                }
                summa_textarum   += parsura->regiones_textae;
                summa_omissarum  += parsura->regiones_omissae;
            }
        }
        si (fidelis)
        {
            fideles++;
        }
        alioquin
        {
            imprimere("  INFIDELIS: %s\n", nomina[k]);
        }
        piscina_destruere(piscina_plagulae);
    }

    imprimere("  fideles %d/%d; regiones textae %d, omissae %d\n",
        (int)fideles, (int)numerus_plagularum, (int)summa_textarum,
        (int)summa_omissarum);
    /* RATIO figitur: omnes fideles (numerus crudus evolvitur cum
     * corpore); textura regionum viva (>0 textae in corpore vero) */
    CREDO_AEQUALIS_I32 (fideles, numerus_plagularum);
    CREDO_MAIOR_I32 (summa_textarum, ZEPHYRUM);

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
