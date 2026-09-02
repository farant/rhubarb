/* probatio_silva_mensura.c - Mensura: tempus + memoria super corpus
 *
 * "Measure from day one" (silva-spec §performance posture): lexare +
 * directivae + expansio positionalis super corpus laminarum; tempus
 * per clock(), apex memoriae per piscina_summa_apex_usus. Summae
 * semper imprimuntur (historia visibilis); per-plagula sub
 * MENSURA_VERBOSA=1. Assertiones minimae - haec probatio historiam
 * colligit, non regressiones capit (nondum: fines post baseline).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_tabulae_c89.h"
#include "silva_glr.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_contextus.h"
#include "silva_c89_oraculum.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>  /* probatio solum - macOS/POSIX */

#define VIA_MAXIMA 1024

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

/* (fasciculus localis GRAMMATICA_C89 sublatus M2d A - via
 * consumptoris silva_c89_parsare_cum_contextu fasciculum
 * SILVA_C89_GRAMMATICA cum unco fert) */

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
                   b32  verbosa;
             character  via_corporis[VIA_MAXIMA];
             character  via_plagulae[VIA_MAXIMA];
    DIR* corpus;
    structura dirent* introitus;
    i32 numerus;
    duplex summa_octetorum;
    duplex summa_lexare_ms;
    duplex summa_expandere_ms;
    duplex summa_parsare_ms;
    duplex summa_scribere_ms;
    duplex summa_apex_octetorum;
    duplex apex_maximus;
    i32 summa_errorum;
    i32 frons_maxima;
    i32 fideles_arboris;
    SilvaContextus* ctx_latinus;
    duplex summa_parsare_c89_ms;
    i32 summa_errorum_c89;
    i32 frons_maxima_c89;
    i32 fideles_arboris_c89;

    piscina = piscina_generare_dynamicum("probatio_silva_mensura",
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
    verbosa = (getenv("MENSURA_VERBOSA") != NIHIL) ? VERUM : FALSUM;
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);

    imprimere("\n--- Mensura corporis laminarum ---\n");

    numerus               = ZEPHYRUM;
    summa_octetorum       = 0.0;
    summa_lexare_ms       = 0.0;
    summa_expandere_ms    = 0.0;
    summa_parsare_ms      = 0.0;
    summa_scribere_ms     = 0.0;
    summa_apex_octetorum  = 0.0;
    apex_maximus          = 0.0;
    summa_errorum         = ZEPHYRUM;
    frons_maxima          = ZEPHYRUM;
    fideles_arboris       = ZEPHYRUM;
    summa_parsare_c89_ms  = 0.0;
    summa_errorum_c89     = ZEPHYRUM;
    frons_maxima_c89      = ZEPHYRUM;
    fideles_arboris_c89   = ZEPHYRUM;

    /* Contextus latinus communis (M2d Chunk A): lexicon latina
     * ante quamque plagulam - grammatica c89 tandem expansione
     * pascitur. Diu vivit, inter parsuras reusatur. */
    {
        ctx_latinus = silva_contextus_creare(piscina);
        si (ctx_latinus != NIHIL)
        {
            (vacuum)silva_contextus_latinam_addere(ctx_latinus);
        }
    }

    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus non apertum: %s\n", via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }

    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        Piscina* piscina_plagulae;
             i8* fons;
            Xar* lexemata;
            Xar* reliqua;
            Xar* expansa;
            i32  mensura;
        clock_t  t0;
        clock_t  t1;
        clock_t  t2;
        clock_t  t3;
        clock_t  t4;
         duplex  lexare_ms;
         duplex  expandere_ms;
         duplex  parsare_ms;
         duplex  scribere_ms;
         duplex  apex;

        si (!_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis, introitus->d_name);

        piscina_plagulae =
            piscina_generare_dynamicum("mensura_plagulae",
            8388608);
        si (piscina_plagulae == NIHIL)
        {
            perge;
        }

        fons = _plagulam_legere(piscina_plagulae, via_plagulae,
            &mensura);
        si (fons == NIHIL)
        {
            piscina_destruere(piscina_plagulae);
            perge;
        }

        t0 = clock();
        lexemata = silva_lexare(piscina_plagulae,
            (constans character*)fons, mensura, ZEPHYRUM);
        t1 = clock();
        {
            SilvaExpansio* exp;

            exp = silva_expansio_creare(piscina_plagulae);
            silva_fons_addere(exp, introitus->d_name, FALSUM);
            reliqua = silva_expansio_directivas_processare(exp,
                lexemata,
                NIHIL);
            expansa = silva_expansio_expandere_reliqua(exp, reliqua,
                NIHIL);
            (vacuum)expansa;
        }
        t2 = clock();

        /* Fistula tota per gubernatorem (lexare+expandere ITERUM
         * interius - mensura fistulae integrae, non additiva). Corpus
         * = C89 verum: sceletum recuperatione superat (soak nodorum
         * ERROR in scala). Deinde scribere: reconstructio strati 0
         * (fidelitas hic numerata, in fidelitate asserta). */
        t4 = t3 = clock();
        {
            SilvaParsura* parsura;

            parsura = silva_parsare(piscina_plagulae,
                introitus->d_name, (constans character*)fons, mensura,
                &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
            t3 = clock();
            si (parsura != NIHIL && parsura->successus)
            {
                SilvaScriptura scriptura;

                summa_errorum += parsura->numerus_errorum;
                si (parsura->frons_maxima > frons_maxima)
                {
                    frons_maxima = parsura->frons_maxima;
                }

                scriptura = silva_scribere_fontem(piscina_plagulae,
                    parsura, &SILVA_SCELETUM_REGISTRUM,
                    parsura->fons_princeps);
                t4 = clock();
                si (   scriptura.successus
                    && scriptura.textus.mensura == mensura
                    && (mensura == ZEPHYRUM
                        || memcmp(scriptura.textus.datum, fons,
                               (memoriae_index)mensura) == ZEPHYRUM))
                {
                    fideles_arboris++;
                }
            }
        }

        /* Grammatica c89 (M2a Chunk C): custodia explosionis
         * furcarum incipit HIC memorare - frons max + tempus
         * trans corpus verum (comparanda cum sceleto supra;
         * fines mensi M2b figentur) */
        {
                 Piscina* piscina_c89;
            SilvaParsura* parsura;
                 clock_t  c0;
                 clock_t  c1;

            piscina_c89 = piscina_generare_dynamicum(
                "mensura_c89", 8388608);
            si (piscina_c89 != NIHIL)
            {
                c0 = clock();
                parsura = silva_c89_parsare_cum_contextu(
                    piscina_c89, ctx_latinus, introitus->d_name,
                    (constans character*)fons, mensura, NIHIL);
                c1 = clock();
                summa_parsare_c89_ms += (duplex)(c1 - c0) * 1000.0
                    / (duplex)CLOCKS_PER_SEC;
                si (parsura != NIHIL && parsura->successus)
                {
                    SilvaScriptura scriptura;

                    summa_errorum_c89 +=
                        (i32)parsura->numerus_errorum;
                    si (   verbosa && parsura->numerus_errorum
                        > ZEPHYRUM)
                    {
                        imprimere("  [c89 errores %d] %s\n",
                            (int)parsura->numerus_errorum,
                            introitus->d_name);
                    }
                    si ((i32)parsura->frons_maxima > frons_maxima_c89)
                    {
                        frons_maxima_c89 = (i32)parsura->frons_maxima;
                    }
                    scriptura = silva_scribere_fontem(piscina_c89,
                        parsura, &SILVA_C89_REGISTRUM,
                        parsura->fons_princeps);
                    si (   scriptura.successus
                        && scriptura.textus.mensura == mensura
                        && (mensura == ZEPHYRUM
                            || memcmp(scriptura.textus.datum, fons,
                                   (memoriae_index)mensura)
                                == ZEPHYRUM))
                    {
                        fideles_arboris_c89++;
                    }
                }
                piscina_destruere(piscina_c89);
            }
        }

        lexare_ms = (duplex)(t1 - t0) * 1000.0 / (duplex)CLOCKS_PER_SEC;
        expandere_ms = (duplex)(t2 - t1) * 1000.0
            / (duplex)CLOCKS_PER_SEC;
        parsare_ms = (duplex)(t3 - t2) * 1000.0
            / (duplex)CLOCKS_PER_SEC;
        scribere_ms = (duplex)(t4 - t3) * 1000.0
            / (duplex)CLOCKS_PER_SEC;
        apex = (duplex)piscina_summa_apex_usus(piscina_plagulae);

        numerus++;
        summa_octetorum       += (duplex)mensura;
        summa_lexare_ms       += lexare_ms;
        summa_expandere_ms    += expandere_ms;
        summa_parsare_ms      += parsare_ms;
        summa_scribere_ms     += scribere_ms;
        summa_apex_octetorum  += apex;
        si (apex > apex_maximus)
        {
            apex_maximus = apex;
        }

        si (verbosa)
        {
            imprimere("  %-36s %7d B  lex %7.3f ms  exp %7.3f ms  pars %7.3f ms  scr %7.3f ms  apex %9.0f B\n",
                introitus->d_name, mensura, lexare_ms, expandere_ms,
                parsare_ms, scribere_ms, apex);
        }

        piscina_destruere(piscina_plagulae);
    }
    closedir(corpus);

    imprimere("  plagulae: %d, octeti: %.0f\n", numerus,
        summa_octetorum);
    imprimere("  lexare:   %8.2f ms totalis (%.4f ms/KB)\n",
        summa_lexare_ms,
        summa_octetorum > 0.0
            ? summa_lexare_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("  expandere:%8.2f ms totalis (%.4f ms/KB)\n",
        summa_expandere_ms,
        summa_octetorum > 0.0
            ? summa_expandere_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("  parsare:  %8.2f ms totalis (%.4f ms/KB) - fistula "
        "integra, recuperatio\n",
        summa_parsare_ms,
        summa_octetorum > 0.0
            ? summa_parsare_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("  scribere: %8.2f ms totalis (%.4f ms/KB) - "
        "reconstructio strati 0\n",
        summa_scribere_ms,
        summa_octetorum > 0.0
            ? summa_scribere_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("  recuperatio: %d nodi ERROR trans corpus, frons max %d; "
        "arbores fideles %d/%d\n",
        (int)summa_errorum, (int)frons_maxima, (int)fideles_arboris,
        (int)numerus);
    imprimere("  c89:      %8.2f ms parsare (%.4f ms/KB); %d nodi "
        "ERROR, frons max %d; fideles %d/%d\n",
        summa_parsare_c89_ms,
        summa_octetorum > 0.0
            ? summa_parsare_c89_ms / (summa_octetorum / 1024.0) : 0.0,
        (int)summa_errorum_c89, (int)frons_maxima_c89,
        (int)fideles_arboris_c89, (int)numerus);
    CREDO_AEQUALIS_I32 (fideles_arboris_c89, numerus);
    /* CUSTOS EXPLOSIONIS FURCARUM (M2b Chunk D): tectum fixum -
     * observatum 2 trans corpus totum; tectum generosum VIII.
     * Fractura = furcae inflatae, remedium nominatum = uncus
     * filtri actionum (Phase 4, evidentia-portatus) */
    CREDO_VERUM (frons_maxima_c89 <= VIII);
    /* MURUS ACCEPTIONIS (M2d Chunk A): corpus totum sine nodis
     * ERROR per contextum latinum (arcus M2 completus: M1 = 41k
     * nodi RECUPERATIONIS -> M2b 62 -> M2c 42 [corpora latina
     * inexpansa] -> M2d 0 [expansio pascit grammaticam]; #pragma
     * directiva vera facta eodem ictu). Fractura = regressio
     * comprehensionis - numerus verus, numquam laxandus. */
    CREDO_AEQUALIS_I32 (summa_errorum_c89, ZEPHYRUM);
    imprimere("  apex:     %8.0f B medius, %.0f B maximus\n",
        numerus > ZEPHYRUM ? summa_apex_octetorum
            / (duplex)numerus : 0.0,
        apex_maximus);

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
