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

    /* --- documentum sine macris: expansio == clonatio --- */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

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
}

    /* --- (a) definitio demissa + vocatio impleta + nota --- */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f><a/><b/></#><<#@f>></radix>", piscina);
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
                        *nota->fragmentum_id, "@f");
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
                    "<radix><#@f><a/><b/></#><<#@f>></radix>");
            }
        }
    }
}

    /* --- (b) transclusio selectoris transit --- */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

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
}

    /* --- (c) FRAGMENTUM_IGNOTUM --- */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis("<radix><<#@nemo>></radix>",
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
                                           "@nemo");
            CREDO_VERUM (expansio.radix_expansa == NIHIL);
        }
    }
}

    /* --- (d) FRAGMENTUM_POSTERIUS --- */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><<#@f>><#@f><a/></#></radix>", piscina);
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
                                           "@f");
        }
    }
}

    /* --- (e) FRAGMENTUM_GEMINUM --- */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f><a/></#><#@f><b/></#></radix>", piscina);
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
                                           "@f");
        }
    }
}

    /* --- (g) fragmentum INTRA corpus = contentum --- */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f><#g/></#><<#@f>></radix>", piscina);
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
}

    /* --- SPATIUM CONTENTI: '<#id>' sine '@' = fragmentum
     * contenti/aliasis (transclusio = alias, non templum -
     * identitas rei consumentis est, e.g. lexN arboris). Machina
     * expansionis ea NON tangit: definitio manet ut contentum,
     * transclusio manet ut nodus, tabula vacua. --- */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#lex1><a/></#><<#lex1>></radix>", piscina);
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
                    "<radix><#lex1><a/></#><<#lex1>></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum),
                    ZEPHYRUM);
            }
        }
    }
}

    /* --- transclusio contenti ad nusquam: res consumentis, non
     * vitium machinae (FRAGMENTUM_IGNOTUM spatio templi solum) --- */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><<#solus>></radix>", piscina);
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
                    "<radix><<#solus>></radix>");
            }
        }
    }
}

    /* --- spatia mixta: templum et contentum in documento uno -
     * casus arboris ipse (post-spatia iuxta lexN) --- */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@t><x/></#><#lex1><a/></#>"
            "<<#@t>><<#lex1>></radix>", piscina);
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
                    "<radix><#lex1><a/></#><x/><<#lex1>></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum), I);
            }
        }
    }
}

    /* --- (T4 a+b) loculi: impletio attributi tota + interpolata --- */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\">"
            "<a x=\"&@p;\" y=\"pre-&@p;-post\"/></#>"
            "<<#@f p=\"123\">></radix>", piscina);
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
                    "<radix><a x=\"123\" y=\"pre-123-post\"/></radix>");
            }
        }
    }
}

    /* --- (T4 c) loculi: impletio textus --- */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@t v=\"@v\"><m>&@v;!</m></#>"
            "<<#@t v=\"salve\">></radix>", piscina);
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
                    "<radix><m>salve!</m></radix>");
            }
        }
    }
}

    /* --- (T4 d) LOCULUS_NON_IMPLETUS --- */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f>></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_LOCULUS_NON_IMPLETUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.loculus, "p");
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "@f");
        }
    }
}

    /* --- (T4 e) ARGUMENTUM_SUPERFLUUM --- */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f p=\"1\" q=\"2\">></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.loculus, "q");
        }
    }
}

    /* --- (T4 f) LOCULUS_IGNOTUS (collectione, sine vocatione) --- */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@ignotum;\"/></#></radix>",
            piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_LOCULUS_IGNOTUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.loculus,
                                           "ignotum");
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "@f");
        }
    }
}

    /* --- (T4 g) '&@x;' EXTRA corpus = littera --- */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons  = chorda_ex_literis("<radix>&@x;</radix>", piscina);
        res   = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            si (expansio.successus)
            {
                /* '&@x;' extra corpus littera manet - expansio
                 * emissionem originalis non turbat (ens ignotum:
                 * 'circuitus mutat, sensus non' - stml.h strictum) */
                CREDO_CHORDA_AEQUALIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    stml_scribere(res.radix, piscina, FALSUM));
            }
        }
    }
}

    /* --- (T4 h) argumentum cum '>>' in valore (T1 compositum) --- */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f p=\"a>>b\">></radix>", piscina);
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
                    "<radix><a x=\"a>>b\"/></radix>");
            }
        }
    }
}

    /* --- (T5 a) corpus vocat macronem priorem: stratum II --- */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@a><x/></#><#@b><<#@a>><y/></#>"
            "<<#@b>></radix>", piscina);
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

                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    "<radix><x/><y/></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum), II);
                nota = (StmlExpansioNota*)xar_obtinere(
                    expansio.tabula_expansionum, ZEPHYRUM);
                si (nota != NIHIL)
                {
                    CREDO_CHORDA_AEQUALIS_LITERIS (
                        *nota->fragmentum_id, "@b");
                    CREDO_AEQUALIS_I32 (nota->stratum, I);
                }
                nota = (StmlExpansioNota*)xar_obtinere(
                    expansio.tabula_expansionum, I);
                si (nota != NIHIL)
                {
                    CREDO_CHORDA_AEQUALIS_LITERIS (
                        *nota->fragmentum_id, "@a");
                    CREDO_AEQUALIS_I32 (nota->stratum, II);
                }
            }
        }
    }
}

    /* --- (T5 b) corpus vocat macronem POSTERIOREM --- */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@b><<#@a>></#><#@a><x/></#><<#@b>></radix>",
            piscina);
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
                                           "@a");
        }
    }
}

    /* --- (T5 c) loculus trans strata NON declaratus = IGNOTUS --- */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@a p=\"@p\"><x v=\"&@p;\"/></#>"
            "<#@b><<#@a p=\"&@q;\">></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (!expansio.successus);
            CREDO_AEQUALIS_I32 (expansio.vitium,
                                STML_EXPANSIO_LOCULUS_IGNOTUS);
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.loculus, "q");
            CREDO_CHORDA_AEQUALIS_LITERIS (expansio.fragmentum,
                                           "@b");
        }
    }
}

    /* --- (T5 c2) loculus trans strata declaratus = transitio --- */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@a p=\"@p\"><x v=\"&@p;\"/></#>"
            "<#@b q=\"@q\"><<#@a p=\"&@q;\">></#>"
            "<<#@b q=\"7\">></radix>", piscina);
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
                    "<radix><x v=\"7\"/></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum), II);
            }
        }
    }
}

    /* --- (T5 d) catena trium stratorum (terminatio) --- */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@a><x/></#><#@b><<#@a>></#><#@c><<#@b>></#>"
            "<<#@c>></radix>", piscina);
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

                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    "<radix><x/></radix>");
                CREDO_AEQUALIS_I32 (
                    xar_numerus(expansio.tabula_expansionum), III);
                nota = (StmlExpansioNota*)xar_obtinere(
                    expansio.tabula_expansionum, II);
                si (nota != NIHIL)
                {
                    CREDO_CHORDA_AEQUALIS_LITERIS (
                        *nota->fragmentum_id, "@a");
                    CREDO_AEQUALIS_I32 (nota->stratum, III);
                }
            }
        }
    }
}


    /* ================================================
     * FORMAE ARGUMENTORUM (par. 6.1): vocatio duas formas
     * aequivalentes habet - inscriptam ('p="v"', scalares) et
     * BLOCUM (elementa argumentorum statim sequentia, valor =
     * liberi -> argumenta SUBARBOREA nominata). Aequivalentia in
     * TABULA machinae vivit; elementa argumentorum pars vocationis
     * sunt et CONSUMUNTUR (in arbore expansa non apparent).
     * Vitium septimum: subarbor in positione CHORDAE (attributum,
     * valor transclusionis, textus interpolatus) clare recusat.
     * ================================================ */

    /* --- (6.1 a) argumentum bloci scalare == inscriptum --- */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f>><@p=>123</></radix>", piscina);
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
                    "<radix><a x=\"123\"/></radix>");
            }
        }
    }
}

    /* --- (6.1 b) argumentum subarboreum: splex silvae in
     * positione liberorum (textus totus '&@c;') --- */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f c=\"@c\"><wrap>&@c;</wrap></#>"
            "<<#@f>><@c=><x/><y/></></radix>", piscina);
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
                    "<radix><wrap><x/><y/></wrap></radix>");
            }
        }
    }
}

    /* --- (6.1 c) vitium septimum: subarbor in attributo --- */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f c=\"@c\"><a x=\"&@c;\"/></#>"
            "<<#@f>><@c=><x/></></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_ARGUMENTUM_ARBOREUM);
        }
    }
}

    /* --- (6.1 d) forma mixta: inscriptum + blocum una vocatione --- */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\" q=\"@q\">"
            "<a x=\"&@p;\" y=\"&@q;\"/></#>"
            "<<#@f p=\"1\">><@q=>2</></radix>", piscina);
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
                    "<radix><a x=\"1\" y=\"2\"/></radix>");
            }
        }
    }
}

    /* --- (6.1 e) nomen geminatum (inscriptum + blocum) clamat --- */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f p=\"1\">><@p=>2</></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_ARGUMENTUM_GEMINUM);
        }
    }
}

    /* --- (6.1 f) sepulcrum = absentia explicita -> loculus
     * declaratus NON impletur --- */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p\"><a x=\"&@p;\"/></#>"
            "<<#@f>><@p=/></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_LOCULUS_NON_IMPLETUS);
        }
    }
}

    /* --- (6.1 g) transitio subarboris trans strata: argumentum
     * bloci in corpore, textus totus '&@d;' ubi d subarbor -
     * silva transit (classificatio POST considerationem
     * referentiae: totus-ref ad subarborem = subarbor, non
     * scalaris) --- */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@inner c=\"@c\"><wrap>&@c;</wrap></#>"
            "<#@outer d=\"@d\"><<#@inner>><@c=>&@d;</></#>"
            "<<#@outer>><@d=><z/></></radix>", piscina);
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
                    "<radix><wrap><z/></wrap></radix>");
            }
        }
    }
}


    /* ================================================
     * COMMUTATIO (par. 6.2): selectio bracchiorum in corporibus
     * definitionum. LINEA: casus LITTERAE ('est="..."') aut
     * probationes PRAESENTIAE ('nihil'/'non-nihil') - numquam
     * praedicata. Primus congruens vincit; ORDINARIUS ultimus;
     * nullus congruens sine ORDINARIO = vitium clarum. Loculi
     * optionales 'p="@p?"' + REGULA ANGUSTATIONIS: loculus
     * optionalis extra bracchium praesentiam statuens relatus =
     * vitium collectionis (angustatio fluxus - optionalitas
     * honesta, numquam vacuum tacitum).
     * ================================================ */

    /* --- (6.2 a) selectio per est + ORDINARIUS --- */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f x=\"@x\"><COMMUTATIO de=\"&@x;\">"
            "<CASUS est=\"a\"><unus/></CASUS>"
            "<CASUS est=\"b\"><duo/></CASUS>"
            "<ORDINARIUS><tres/></ORDINARIUS>"
            "</COMMUTATIO></#>"
            "<<#@f x=\"b\">><<#@f x=\"z\">></radix>", piscina);
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
                    "<radix><duo/><tres/></radix>");
            }
        }
    }
}

    /* --- (6.2 b) bracchia praesentiae + loculus optionalis:
     * quattuor vocationes (congruens est; alia praesens; absens;
     * sepulcrum = absens). Bracchia est/non-nihil praesentiam
     * statuunt - '&@p;' intra ea licitum. --- */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p?\"><COMMUTATIO de=\"&@p;\">"
            "<CASUS est=\"1\"><unus v=\"&@p;\"/></CASUS>"
            "<CASUS non-nihil><habet>&@p;</habet></CASUS>"
            "<CASUS nihil><caret/></CASUS>"
            "</COMMUTATIO></#>"
            "<<#@f p=\"1\">><<#@f p=\"2\">><<#@f>>"
            "<<#@f>><@p=/></radix>", piscina);
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
                    "<radix><unus v=\"1\"/><habet>2</habet>"
                    "<caret/><caret/></radix>");
            }
        }
    }
}

    /* --- (6.2 c) exhaustio: nullus congruens, nullus ORDINARIUS
     * -> vitium impletionis --- */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f x=\"@x\"><COMMUTATIO de=\"&@x;\">"
            "<CASUS est=\"a\"><unus/></CASUS>"
            "</COMMUTATIO></#><<#@f x=\"z\">></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_CASUS_NULLUS);
        }
    }
}

    /* --- (6.2 d) angustatio: loculus optionalis extra bracchium
     * statuens relatus = vitium COLLECTIONIS (etiam ORDINARIUS
     * praesentiam NON statuit - exsequitur et praesente et
     * absente) --- */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p?\"><a v=\"&@p;\"/></#></radix>",
            piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS);
        }
    }
}


interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f p=\"@p?\"><COMMUTATIO de=\"&@p;\">"
            "<CASUS nihil><caret/></CASUS>"
            "<ORDINARIUS><a v=\"&@p;\"/></ORDINARIUS>"
            "</COMMUTATIO></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS);
        }
    }
}

    /* --- (6.2 e) malformatio: est cum referentia (LINEA - numquam
     * praedicata computata); de litterale (non totus-ref) --- */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><#@f x=\"@x\" y=\"@y\">"
            "<COMMUTATIO de=\"&@x;\">"
            "<CASUS est=\"&@y;\"><a/></CASUS>"
            "</COMMUTATIO></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_COMMUTATIO_MALFORMATA);
        }

        fons = chorda_ex_literis(
            "<radix><#@f x=\"@x\">"
            "<COMMUTATIO de=\"litterale\">"
            "<CASUS est=\"a\"><a/></CASUS>"
            "</COMMUTATIO></#></radix>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_FALSUM (expansio.successus);
            CREDO_AEQUALIS_I32 ((i32)expansio.vitium,
                (i32)STML_EXPANSIO_COMMUTATIO_MALFORMATA);
        }
    }
}

    /* --- (6.2 f) COMMUTATIO gradu documenti INTACTA transit
     * (machina eam solum in impletione interpretatur - regula
     * eadem ac '&@x;' extra corpora littera manens) --- */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<radix><COMMUTATIO de=\"x\">"
            "<CASUS est=\"a\"><a/></CASUS>"
            "</COMMUTATIO></radix>", piscina);
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
                    "<radix><COMMUTATIO de=\"x\">"
                    "<CASUS est=\"a\"><a/></CASUS>"
                    "</COMMUTATIO></radix>");
            }
        }
    }
}


    /* ==================================================
     * DISTRIBUTIO (ronda IV, rata 2026-08-26) - proiectio contenti
     * SECUNDA: legere -> expandere -> distribuere -> consumens.
     * NON intra stml_expandere (contractus machinae = spatium
     * templi '@' solum; distributio spatium contenti innotatum).
     *
     * Involucrum (elementum NOMINATUM cuius liberi post praefixum
     * attributorum fragmenta anonyma OMNES sunt) dissolvitur: item
     * N -> fratres N typo involucri; attributa descendunt;
     * collisio ITEM-VINCIT, sepulcro '<@x=/>' completa (verbum
     * delendi); mixtura anonymi/nominati = vitium CLARUM; item
     * vacuum = elementum vacuum; item silvae sunt; nidificatio
     * recursiva (listae anonymae nidificatae typo eodem PLANANTUR).
     * Arbor originalis INTACTA (contractus duarum arborum).
     * ================================================== */

    /* --- distributio: documentum sine involucris == clonatio --- */

interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis("<radix><a x=\"1\"/>t</radix>",
                                 piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            CREDO_NON_NIHIL (dist.radix_distributa);
            si (dist.successus && dist.radix_distributa != NIHIL)
            {
                CREDO_VERUM (dist.radix_distributa != res.radix);
                CREDO_CHORDA_AEQUALIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    stml_scribere(res.radix, piscina, FALSUM));
            }
        }
    }
}

    /* --- distributio basalis: involucrum dissolvitur, typus
     * descendit; originalis intacta --- */

interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<nota t=\"a\"><#>emere lac</#><#>ianuam</#></nota>",
            piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<nota t=\"a\">emere lac</nota>"
                    "<nota t=\"a\">ianuam</nota>");
                /* originalis: involucrum unum, item fragmenta duo */
                CREDO_AEQUALIS_I32 (
                    xar_numerus(res.radix->liberi), I);
                CREDO_AEQUALIS_I32 (
                    xar_numerus(res.elementum_radix->liberi), II);
            }
        }
    }
}

    /* --- attributa descendunt; collisio ITEM-VINCIT --- */

interior vacuum
_sectio_tricesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<a t=\"x\" u=\"y\"><#>p</#><# u=\"z\">q</#></a>",
            piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                StmlNodus* alterum;

                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a t=\"x\" u=\"y\">p</a>"
                    "<a t=\"x\" u=\"z\">q</a>");
                alterum = *(StmlNodus**)xar_obtinere(
                    dist.radix_distributa->liberi, I);
                CREDO_NON_NIHIL (
                    stml_attributum_capere(alterum, "t"));
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    *stml_attributum_capere(alterum, "u"), "z");
            }
        }
    }
}

    /* --- sepulcrum '<@u=/>' in praefixo item attributum
     * hereditatum DELET (verbum delendi par. 6.3) --- */

interior vacuum
_sectio_quadragesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<a t=\"1\" u=\"2\"><#><@u=/>x</#></a>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                StmlNodus* e;

                e = *(StmlNodus**)xar_obtinere(
                    dist.radix_distributa->liberi, ZEPHYRUM);
                CREDO_NON_NIHIL (stml_attributum_capere(e, "t"));
                /* absentia explicita: capere NIHIL */
                CREDO_NIHIL (stml_attributum_capere(e, "u"));
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a t=\"1\"><@u=/>x</a>");
            }
        }
    }
}

    /* --- praefixum attributorum involucri quoque descendit;
     * item eadem lege vincit --- */

interior vacuum
_sectio_quadragesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<a><@t=>v</><#>x</#><# t=\"w\">y</#></a>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a><@t=>v</>x</a><a t=\"w\">y</a>");
            }
        }
    }
}

    /* --- homogeneitas CLARA: mixtura anonymi/nominati = vitium --- */

interior vacuum
_sectio_quadragesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons  = chorda_ex_literis("<a><#>x</#><b/></a>", piscina);
        res   = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (!dist.successus);
            CREDO_AEQUALIS_I32 ((i32)dist.vitium,
                                (i32)STML_DISTRIBUTIO_MIXTA);
            CREDO_CHORDA_AEQUALIS_LITERIS (dist.titulus, "a");
            CREDO_NIHIL (dist.radix_distributa);
        }
    }
}

    /* --- item vacuum = elementum vacuum --- */

interior vacuum
_sectio_quadragesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons  = chorda_ex_literis("<a t=\"1\"><#/></a>", piscina);
        res   = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a t=\"1\"/>");
            }
        }
    }
}

    /* --- item silvae sunt: liberi plures --- */

interior vacuum
_sectio_quadragesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons  = chorda_ex_literis("<a><#><b/><c/></#></a>", piscina);
        res   = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a><b/><c/></a>");
            }
        }
    }
}

    /* --- nidificatio recursiva: involucrum nominatum intra item --- */

interior vacuum
_sectio_quadragesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<r><a><#><b><#>k</#></b></#></a></r>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<r><a><b>k</b></a></r>");
            }
        }
    }
}

    /* --- nidificatio recursiva: listae anonymae typo eodem
     * PLANANTUR (rithmus PER degeneratae) --- */

interior vacuum
_sectio_quadragesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<r><a><#><#>x</#><#>y</#></#></a></r>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<r><a>x</a><a>y</a></r>");
            }
        }
    }
}

    /* --- fragmenta anonyma sine parente nominato transeunt
     * (gradu documenti; intra fragmentum nominatum) --- */

interior vacuum
_sectio_quadragesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons  = chorda_ex_literis("<#>x</#>", piscina);
        res   = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<#>x</#>");
            }
        }

        fons = chorda_ex_literis("<x><#f><#>i</#></#></x>",
                                 piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<x><#f><#>i</#></#></x>");
            }
        }
    }
}

    /* --- commenta inter item transeunt positione servata --- */

interior vacuum
_sectio_quadragesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        StmlResultus res;
              chorda fons;

        fons = chorda_ex_literis(
            "<a t=\"1\"><#>x</#><!--n--><#>y</#></a>", piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlDistributioResultus dist;

            dist = stml_distribuere(res.radix, piscina, intern);
            CREDO_VERUM (dist.successus);
            si (dist.successus)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS (
                    stml_scribere(dist.radix_distributa, piscina,
                                  FALSUM),
                    "<a t=\"1\">x</a><!--n--><a t=\"1\">y</a>");
            }
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "documentum sine macris", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "definitio + vocatio nuda", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "transclusio selectoris transit", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fragmentum ignotum", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fragmentum posterius", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fragmentum geminum", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fragmentum intra corpus", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fragmenta contenti transeunt", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "transclusio contenti sine definitione", _sectio_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "spatia mixta (templum + contentum)", _sectio_decima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "loculi: impletio attributorum", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "loculi: impletio textus", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "loculus non impletus", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "argumentum superfluum", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "loculus ignotus", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "template-spatium extra corpus", _sectio_decima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "argumentum cum '>>'", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vocatio nidificata", _sectio_decima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "posterius in corpore", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "trans strata non declaratus", _sectio_vicesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "trans strata declaratus", _sectio_vicesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "catena trium stratorum", _sectio_vicesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "argumentum bloci scalare", _sectio_vicesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "argumentum subarboreum", _sectio_vicesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vitium septimum (subarbor in chorda)", _sectio_vicesima_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma mixta", _sectio_vicesima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "argumentum geminum", _sectio_vicesima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "sepulcrum argumenti", _sectio_vicesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "transitio subarboris", _sectio_vicesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "COMMUTATIO: selectio est", _sectio_tricesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "COMMUTATIO: praesentia + optionalis", _sectio_tricesima_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "COMMUTATIO: casus nullus", _sectio_tricesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "angustatio violata (extra)", _sectio_tricesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "angustatio violata (ORDINARIUS)", _sectio_tricesima_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "COMMUTATIO malformata", _sectio_tricesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "COMMUTATIO gradu documenti", _sectio_tricesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio: sine involucris", _sectio_tricesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio basalis", _sectio_tricesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "distributio: item vincit", _sectio_tricesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "distributio: sepulcrum delet", _sectio_quadragesima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio: praefixum involucri", _sectio_quadragesima_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "distributio: mixtura", _sectio_quadragesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio: item vacuum", _sectio_quadragesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio: item silva", _sectio_quadragesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "distributio: nidificatio nominata", _sectio_quadragesima_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "distributio: planatio anonyma", _sectio_quadragesima_sexta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "distributio: sine parente nominato",
        _sectio_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "distributio: commenta inter item", _sectio_quadragesima_octava,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("stml_macros", SECTIONES);
}
