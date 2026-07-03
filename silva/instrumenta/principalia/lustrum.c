/* lustrum.c - Census corporis solarii (Phase 7 Chunk C)
 *
 * LUSTRUM: census Romanus. Omnis plagula .c/.h solarii per fistulam
 * integram (parsare cum contextu -> scribere) agitur; fidelitas
 * octetim exacta, causae fracturarum, nodi ERROR (recuperatio
 * honesta - grammatica sceleti C89 verum non intellegit), textura
 * regionum (textae/omissae - datum consilii petrae config-query),
 * tempora, atque INDICIUM PROSSER: macros functio-similes corporibus
 * parenthesium imparium (praeconditio classis HS_call∩HS_rparen) -
 * nulla inventa = simplificatio conservativa permanens documentanda.
 *
 * Usus: lustrum <via_solarii>
 * Instrumentum dev-time (principalia/) - numquam amalgamatum.
 */
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define VIA_MAXIMA 1024
#define PLAGULAE_MAXIMAE 512

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare
};

interior i8*
_plagulam_legere (Piscina* piscina, constans character* via,
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
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura] = '\0';
    *mensura_out = (i32)mensura;
    redde buffer;
}

interior b32
_suffixum_habet (constans character* titulus,
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

/* Prosser: parenthesium imparium in corporibus macrorum
 * functio-similium (praeconditio classis HS_call∩HS_rparen).
 * Reddit numerum imparium novorum; nomina imprimit. */
interior i32
_prosser_scandere (SilvaExpansio* exp)
{
    i32 imparia = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(exp->acta); k++)
    {
        SilvaEventum* ev = (SilvaEventum*)xar_obtinere(exp->acta, k);
        s32 par = ZEPHYRUM;
        b32 impar = FALSUM;
        i32 t;

        si (ev == NIHIL || ev->genus != SILVA_EVENTUM_DEFINITIO
            || ev->def == NIHIL || !ev->def->est_functio
            || ev->def->corpus == NIHIL)
        {
            perge;
        }
        per (t = ZEPHYRUM; t < xar_numerus(ev->def->corpus); t++)
        {
            SilvaToken* lex = *(SilvaToken**)xar_obtinere(
                ev->def->corpus, t);

            si (lex->genus == SILVA_LEX_PAREN_APERTA)
            {
                par++;
            }
            alioquin si (lex->genus == SILVA_LEX_PAREN_CLAUSA)
            {
                par--;
                si (par < ZEPHYRUM)
                {
                    impar = VERUM;
                }
            }
        }
        si (par != ZEPHYRUM)
        {
            impar = VERUM;
        }
        si (impar && ev->titulus != NIHIL)
        {
            imprimere("    PROSSER: macro '%.*s' parenthesibus "
                "imparibus\n", (int)ev->titulus->mensura,
                (constans character*)ev->titulus->datum);
            imparia++;
        }
    }
    redde imparia;
}

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina_census;
    SilvaContextus* contextus;
    character* nomina[PLAGULAE_MAXIMAE];
    i32 numerus_plagularum = ZEPHYRUM;
    constans character* via_solarii;
    character via[VIA_MAXIMA];
    i32 k;

    /* Summae */
    i32 fideles = ZEPHYRUM;
    i32 infideles = ZEPHYRUM;
    i32 fracturae_parsurae = ZEPHYRUM;
    i32 summa_octetorum = ZEPHYRUM;
    i32 summa_errorum = ZEPHYRUM;
    i32 summa_segmentorum = ZEPHYRUM;
    i32 summa_textarum = ZEPHYRUM;
    i32 summa_omissarum = ZEPHYRUM;
    i32 summa_inclusionum_ignotarum = ZEPHYRUM;
    i32 prosser_imparia = ZEPHYRUM;
    i32 fines_tactae_plagulae = ZEPHYRUM;
    duplex tempus_totale = 0.0;

    si (argc < II)
    {
        fprintf(stderr, "usus: lustrum <via_solarii>\n");
        redde I;
    }
    via_solarii = argv[I];

    piscina_census = piscina_generare_dynamicum("lustrum", 33554432);
    si (piscina_census == NIHIL)
    {
        fprintf(stderr, "lustrum: piscina non generata\n");
        redde I;
    }

    /* Nomina plagularum colligere (.c + .h, planum) */
    {
        DIR* dir = opendir(via_solarii);
        structura dirent* introitus;

        si (dir == NIHIL)
        {
            fprintf(stderr, "lustrum: %s non apertum\n", via_solarii);
            redde I;
        }
        dum ((introitus = readdir(dir)) != NIHIL
            && numerus_plagularum < PLAGULAE_MAXIMAE)
        {
            si (_suffixum_habet(introitus->d_name, ".c")
                || _suffixum_habet(introitus->d_name, ".h"))
            {
                memoriae_index m = strlen(introitus->d_name) + I;
                character* nomen_novum = (character*)piscina_allocare(
                    piscina_census, m);

                si (nomen_novum != NIHIL)
                {
                    memcpy(nomen_novum, introitus->d_name, m);
                    nomina[numerus_plagularum++] = nomen_novum;
                }
            }
        }
        closedir(dir);
    }
    /* Ordinare (insertio - numeri parvi; exitus stabilis) */
    per (k = I; k < numerus_plagularum; k++)
    {
        character* clavis = nomina[k];
        i32 j = k;

        dum (j > ZEPHYRUM && strcmp(nomina[j - I], clavis) > ZEPHYRUM)
        {
            nomina[j] = nomina[j - I];
            j--;
        }
        nomina[j] = clavis;
    }
    imprimere("lustrum: %d plagulae in %s\n",
        (int)numerus_plagularum, via_solarii);

    /* Contextus: clausura capitum praebita (receptum reusabile -
     * quaeque parsura expansionem recentem accipit) */
    contextus = silva_contextus_creare(piscina_census);
    si (contextus == NIHIL)
    {
        redde I;
    }
    {
        i32 capita = ZEPHYRUM;

        per (k = ZEPHYRUM; k < numerus_plagularum; k++)
        {
            i8* textus;
            i32 mensura;

            si (!_suffixum_habet(nomina[k], ".h"))
            {
                perge;
            }
            sprintf(via, "%s/%s", via_solarii, nomina[k]);
            textus = _plagulam_legere(piscina_census, via, &mensura);
            si (textus != NIHIL
                && silva_contextus_praebere(contextus, nomina[k],
                       (constans character*)textus, mensura))
            {
                capita++;
            }
        }
        imprimere("lustrum: %d capita praebita\n", (int)capita);
    }

    /* Census: quaeque plagula fistula integra */
    per (k = ZEPHYRUM; k < numerus_plagularum; k++)
    {
        Piscina* piscina_plagulae;
        i8* textus;
        i32 mensura;
        SilvaParsura* parsura;
        SilvaScriptura scriptura;
        clock_t t0;
        clock_t t1;

        sprintf(via, "%s/%s", via_solarii, nomina[k]);
        piscina_plagulae = piscina_generare_dynamicum("plagula",
            268435456);
        si (piscina_plagulae == NIHIL)
        {
            fprintf(stderr, "lustrum: piscina plagulae deest\n");
            redde I;
        }
        textus = _plagulam_legere(piscina_plagulae, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  %-28s NON LECTA\n", nomina[k]);
            piscina_destruere(piscina_plagulae);
            perge;
        }
        summa_octetorum += mensura;

        t0 = clock();
        parsura = silva_parsare_cum_contextu(piscina_plagulae,
            contextus, nomina[k], (constans character*)textus,
            mensura, &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
        si (parsura == NIHIL || !parsura->successus)
        {
            imprimere("  %-28s FRACTURA PARSURAE\n", nomina[k]);
            fracturae_parsurae++;
            piscina_destruere(piscina_plagulae);
            perge;
        }
        scriptura = silva_scribere_fontem(piscina_plagulae, parsura,
            &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
        t1 = clock();
        tempus_totale += (duplex)(t1 - t0) / (duplex)CLOCKS_PER_SEC;

        si (scriptura.successus
            && scriptura.textus.mensura == (i32)mensura
            && (mensura == ZEPHYRUM
                || memcmp(scriptura.textus.datum, textus,
                       (memoriae_index)mensura) == ZEPHYRUM))
        {
            fideles++;
        }
        alioquin
        {
            infideles++;
            si (!scriptura.successus)
            {
                imprimere("  %-28s INFIDELIS: %s\n", nomina[k],
                    (scriptura.causa != NIHIL) ? scriptura.causa
                                               : "(sine causa)");
            }
            alioquin
            {
                /* offset primae differentiae (diagnostica) */
                i32 d = ZEPHYRUM;
                i32 minor = (scriptura.textus.mensura < (i32)mensura)
                    ? (i32)scriptura.textus.mensura : mensura;

                dum (d < minor
                    && scriptura.textus.datum[d] == (i8)textus[d])
                {
                    d++;
                }
                imprimere("  %-28s INFIDELIS: octeti differunt ad "
                    "%d (mensurae %d vs %d)\n", nomina[k], (int)d,
                    (int)scriptura.textus.mensura, (int)mensura);
            }
        }

        summa_errorum += parsura->numerus_errorum;
        summa_segmentorum += parsura->numerus_segmentorum;
        summa_textarum += parsura->regiones_textae;
        summa_omissarum += parsura->regiones_omissae;
        si (parsura->fines_tactae)
        {
            fines_tactae_plagulae++;
        }
        si (parsura->expansio != NIHIL)
        {
            i32 n;

            prosser_imparia += _prosser_scandere(parsura->expansio);
            per (n = ZEPHYRUM;
                 n < xar_numerus(parsura->expansio->inclusiones); n++)
            {
                SilvaInclusio* inc = (SilvaInclusio*)xar_obtinere(
                    parsura->expansio->inclusiones, n);

                si (inc != NIHIL && inc->fons_ad == -I)
                {
                    summa_inclusionum_ignotarum++;
                }
            }
        }
        piscina_destruere(piscina_plagulae);
    }

    imprimere("\n==== LUSTRUM SOLARII ====\n");
    imprimere("plagulae:            %d\n", (int)numerus_plagularum);
    imprimere("fideles:             %d\n", (int)fideles);
    imprimere("infideles:           %d\n", (int)infideles);
    imprimere("fracturae parsurae:  %d\n", (int)fracturae_parsurae);
    imprimere("octeti:              %d\n", (int)summa_octetorum);
    imprimere("tempus:              %.1f ms (%.3f ms/KB)\n",
        tempus_totale * 1000.0,
        (summa_octetorum > ZEPHYRUM)
            ? tempus_totale * 1000.0
                / ((duplex)summa_octetorum / 1024.0)
            : 0.0);
    imprimere("nodi ERROR:          %d (in %d segmentis - "
        "recuperatio honesta, grammatica sceleti)\n",
        (int)summa_errorum, (int)summa_segmentorum);
    imprimere("regiones textae:     %d\n", (int)summa_textarum);
    imprimere("regiones omissae:    %d\n", (int)summa_omissarum);
    imprimere("inclusiones ignotae: %d (systematis - discens)\n",
        (int)summa_inclusionum_ignotarum);
    imprimere("fines tactae:        %d plagulae\n",
        (int)fines_tactae_plagulae);
    imprimere("PROSSER imparia:     %d (0 = simplificatio "
        "conservativa permanens documentanda)\n",
        (int)prosser_imparia);

    piscina_destruere(piscina_census);
    redde (infideles == ZEPHYRUM && fracturae_parsurae == ZEPHYRUM)
        ? ZEPHYRUM : I;
}
