/* probatio_stml_macros.c - expansio fragmentorum parametrizatorum
 *
 * CONTRACTUS DUARUM ARBORUM: expansio arborem NOVAM reddit,
 * originalis intacta manet (plagula formam macroneam servat;
 * scriptor arborem expansam numquam videt). Documentum sine
 * macris: expansio == clonatio - emissio octetim aequalis,
 * tabula expansionum vacua.
 *
 * Spec: project-specs/stml-macros-spec.md; planum:
 * stml-macros-plan.md.
 */
#include "latina.h"
#include "stml.h"
#include "stml_macros.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_stml_macros",
                                         1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    /* --- documentum sine macris: expansio == clonatio --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- documentum sine macris ---\n");
        fons = chorda_ex_literis("<radix><a x=\"1\"/>t</radix>",
                                 piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            CREDO_NON_NIHIL (expansio.radix_expansa);
            si (expansio.successus && expansio.radix_expansa != NIHIL)
            {
                CREDO_VERUM (expansio.radix_expansa != res.radix);
                CREDO_NON_NIHIL (expansio.tabula_expansionum);
                si (expansio.tabula_expansionum != NIHIL)
                {
                    CREDO_AEQUALIS_I32 (
                        xar_numerus(expansio.tabula_expansionum),
                        ZEPHYRUM);
                }
                CREDO_CHORDA_AEQUALIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    stml_scribere(res.radix, piscina, FALSUM));
            }
        }
    }

    /* --- (a) definitio demissa + vocatio impleta + nota --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- definitio + vocatio nuda ---\n");
        fons = chorda_ex_literis(
            "<radix><#f><a/><b/></#><<#f>></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            si (expansio.successus)
            {
                StmlExpansioNota* nota;
                       StmlNodus* radix_expansa_elem;
                       StmlNodus* vocatio_originalis;

                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    "<radix><a/><b/></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum), I);

                nota = (StmlExpansioNota*)xar_obtinere(
                    expansio.tabula_expansionum, ZEPHYRUM);
                vocatio_originalis =
                    stml_liberum_ad_indicem(res.elementum_radix, I);
                radix_expansa_elem = stml_liberum_ad_indicem(
                    expansio.radix_expansa, ZEPHYRUM);
                CREDO_NON_NIHIL (nota);
                CREDO_NON_NIHIL (vocatio_originalis);
                CREDO_NON_NIHIL (radix_expansa_elem);
                si (   nota               != NIHIL
                    && vocatio_originalis != NIHIL
                    && radix_expansa_elem != NIHIL)
                {
                    CREDO_CHORDA_AEQUALIS_LITERIS (
                        *nota->fragmentum_id, "f");
                    CREDO_AEQUALIS_I32 (nota->stratum, I);
                    /* vocatio = nodus in arbore ORIGINALI */
                    CREDO_VERUM (nota->vocatio
                                     == vocatio_originalis);
                    /* nodus = clonis primus splicis */
                    CREDO_VERUM (nota->nodus
                                     == stml_liberum_ad_indicem(
                                            radix_expansa_elem,
                                            ZEPHYRUM));
                }

                /* (f) arbor originalis intacta */
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(res.radix, piscina, FALSUM),
                    "<radix><#f><a/><b/></#><<#f>></radix>");
            }
        }
    }

    /* --- (b) transclusio selectoris transit --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- transclusio selectoris transit ---\n");
        fons = chorda_ex_literis(
            "<radix><<caput titulus>></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            si (expansio.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    "<radix><<caput titulus>></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum),
                    ZEPHYRUM);
            }
        }
    }

    /* --- (c) FRAGMENTUM_IGNOTUM --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- fragmentum ignotum ---\n");
        fons = chorda_ex_literis("<radix><<#nemo>></radix>",
                                 piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_FRAGMENTUM_IGNOTUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "nemo");
            CREDO_VERUM (expansio.radix_expansa == NIHIL);
        }
    }

    /* --- (d) FRAGMENTUM_POSTERIUS --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- fragmentum posterius ---\n");
        fons = chorda_ex_literis(
            "<radix><<#f>><#f><a/></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_FRAGMENTUM_POSTERIUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "f");
        }
    }

    /* --- (e) FRAGMENTUM_GEMINUM --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- fragmentum geminum ---\n");
        fons = chorda_ex_literis(
            "<radix><#f><a/></#><#f><b/></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_FRAGMENTUM_GEMINUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "f");
        }
    }

    /* --- (g) fragmentum INTRA corpus = contentum --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- fragmentum intra corpus ---\n");
        fons = chorda_ex_literis(
            "<radix><#f><#g/></#><<#f>></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            si (expansio.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    "<radix><#g/></radix>");
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
