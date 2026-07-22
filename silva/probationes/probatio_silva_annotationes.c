/* probatio_silva_annotationes.c - collector annotationum STML
 * (parcum 01KY3D7EJP frustum A; casus fixi: ancoratum / prosa /
 * malformatum / multilineare-decoratum / cumulatum / plagula-
 * scopus / INTERIOR / radix-macro + nid boolean + vacuum) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_annotationes.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* parsare + colligere uno gressu */
interior Xar*
_colligere (Piscina* piscina, constans character* fons,
    SilvaParsura** parsura_out)
{
    SilvaParsura* parsura = silva_c89_parsare(piscina,
        "probatio.c", fons, (i32)strlen(fons), NIHIL);

    si (parsura == NIHIL)
    {
        redde NIHIL;
    }
    si (parsura_out != NIHIL)
    {
        *parsura_out = parsura;
    }
    redde silva_annotationes_colligere(piscina, parsura, NIHIL);
}

/* unitas suprema per indicem */
interior constans SilvaNodus*
_unitas (Piscina* piscina, constans SilvaParsura* parsura, i32 index)
{
    Xar* unitates = silva_annotationes_unitates(piscina, parsura);

    si (unitates == NIHIL || index >= xar_numerus(unitates))
    {
        redde NIHIL;
    }
    redde *(SilvaNodus**)xar_obtinere(unitates, index);
}

interior SilvaAnnotatio*
_annotatio (Xar* annotationes, i32 index)
{
    redde (SilvaAnnotatio*)xar_obtinere(annotationes, index);
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_annotationes",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. ancoratio + salta-vacua + prosa numquam lecta
     * ======================================================== */
    {
        constans character* fons =
            "/* prosa normalis hic est */\n"
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE1R\"/> */\n"
            "\n"
            "\n"
            "int a;\n"
            "/* < 5 spatio non ancoratum */\n"
            "int b;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;
        chorda* v;

        imprimere("\n--- Probans ancorationem fundamentalem ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_NON_NIHIL (a->documentum);
        CREDO_NON_NIHIL (a->arbor);
        CREDO_CHORDA_AEQUALIS_LITERIS (*a->arbor->titulus, "nid");
        v = stml_attributum_capere(a->arbor, "v");
        CREDO_NON_NIHIL (v);
        CREDO_CHORDA_AEQUALIS_LITERIS (*v,
            "01KY3A2FQ8XN4VJ7TT9M2CDE1R");
        CREDO_AEQUALIS_I32 (a->linea, II);
        CREDO_AEQUALIS_S32 (a->fons_index, parsura->fons_princeps);
        /* salta-vacua: lineae vacuae affixionem non frangunt */
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * II. registrum leve: captura lineae in commentario lineae
     * ======================================================== */
    {
        constans character* fons =
            "// <intentio (>cur probamus\n"
            "int b;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;
        chorda internus;

        imprimere("\n--- Probans registrum leve ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_NON_NIHIL (a->arbor);
        CREDO_CHORDA_AEQUALIS_LITERIS (*a->arbor->titulus,
            "intentio");
        internus = stml_textus_internus(a->arbor, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (internus, "cur probamus");
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * III. multilineare decoratum + nid boolean
     * ======================================================== */
    {
        constans character* fons =
            "/*\n"
            " * <intentio nid>\n"
            " *   <causa>quia probamus</causa>\n"
            " * </intentio>\n"
            " */\n"
            "int c;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;
        StmlNodus* causa;
        chorda internus;

        imprimere("\n--- Probans praetransitum decorationis ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_NON_NIHIL (a->arbor);
        CREDO_CHORDA_AEQUALIS_LITERIS (*a->arbor->titulus,
            "intentio");
        /* nid boolean = petitio mintationis (spec par 2.4) */
        CREDO_VERUM (stml_attributum_habet(a->arbor, "nid"));
        causa = stml_invenire_liberum(a->arbor, "causa");
        CREDO_NON_NIHIL (causa);
        internus = stml_textus_internus(causa, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS (internus, "quia probamus");
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * IV. malformatum ancoratum = INVENTUM, numquam tacite prosa
     * ======================================================== */
    {
        constans character* fons =
            "/* <intentio sine fine */\n"
            "int d;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;

        imprimere("\n--- Probans malformatum ancoratum ---\n");
        CREDO_NON_NIHIL (annotationes);
        /* collectum EST - ancoratum numquam evanescit */
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_FALSUM (a->parsata);
        CREDO_INAEQUALITAS_S32 ((s32)a->status,
            (s32)STML_SUCCESSUS);
        CREDO_VERUM (a->linea_erroris >= I);
        /* affixio nihilominus computata */
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * V. cumulata: ambae supra unitatem eandem
     * ======================================================== */
    {
        constans character* fons =
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE2A\"/> */\n"
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE2B\"/> */\n"
            "int e;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* prima;
        SilvaAnnotatio* secunda;

        imprimere("\n--- Probans annotationes cumulatas ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), II);

        prima = _annotatio(annotationes, ZEPHYRUM);
        secunda = _annotatio(annotationes, I);
        CREDO_AEQUALIS_I32 (prima->linea, I);
        CREDO_AEQUALIS_I32 (secunda->linea, II);
        CREDO_AEQUALIS_S32 ((s32)prima->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_S32 ((s32)secunda->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_NON_NIHIL (prima->unitas);
        CREDO_AEQUALIS_PTR (prima->unitas, secunda->unitas);
    }

    /* ========================================================
     * VI. scopus plagulae: sine unitate sequente
     * ======================================================== */
    {
        constans character* fons =
            "int f;\n"
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE3B\"/> */\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;

        imprimere("\n--- Probans scopum plagulae ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_PLAGULA);
        CREDO_NIHIL (a->unitas);
    }

    /* ========================================================
     * VII. INTERIOR: intra unitatem continentem
     * ======================================================== */
    {
        constans character* fons =
            "int g(void)\n"
            "{\n"
            "    /* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE4C\"/> */\n"
            "    return 0;\n"
            "}\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;

        imprimere("\n--- Probans modum INTERIOR ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_INTERIOR);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * VIII. radix originis: commentarium ante lineam macro ductam
     * (lectio TOLERA - trivia in lexemate invocationis equitant)
     * ======================================================== */
    {
        constans character* fons =
            "#define numerus int\n"
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE5D\"/> */\n"
            "numerus h;\n";
        SilvaParsura* parsura;
        Xar* annotationes = _colligere(piscina, fons, &parsura);
        SilvaAnnotatio* a;

        imprimere("\n--- Probans radicem originis ---\n");
        CREDO_NON_NIHIL (annotationes);
        /* unum - duplicata per (fons, byte_offset) omissa */
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), I);

        a = _annotatio(annotationes, ZEPHYRUM);
        CREDO_VERUM (a->parsata);
        CREDO_AEQUALIS_I32 (a->linea, II);
        CREDO_AEQUALIS_S32 ((s32)a->modus,
            (s32)SILVA_ANNOTATIO_SUPRA);
        CREDO_AEQUALIS_PTR (a->unitas,
            _unitas(piscina, parsura, ZEPHYRUM));
    }

    /* ========================================================
     * X. identitates: lectio arboris (frustum B)
     * ======================================================== */
    {
        Xar* annotationes;
        Xar* identitates;
        SilvaIdentitas* id;

        imprimere("\n--- Probans lectionem identitatum ---\n");

        /* valuta solitaria */
        annotationes = _colligere(piscina,
            "/* <nid v=\"01KY3A2FQ8XN4VJ7TT9M2CDE1R\"/> */\n"
            "int a;\n", NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_NON_NIHIL (identitates);
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_FALSUM (id->petitio);
        CREDO_CHORDA_AEQUALIS_LITERIS (id->valor,
            "01KY3A2FQ8XN4VJ7TT9M2CDE1R");

        /* valuta ut attributum elementi */
        annotationes = _colligere(piscina,
            "/* <intentio nid=\"01KY3A2FQ8XN4VJ7TT9M2CDE6E\">x"
            "</intentio> */\nint a;\n", NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_FALSUM (id->petitio);
        CREDO_CHORDA_AEQUALIS_LITERIS (id->valor,
            "01KY3A2FQ8XN4VJ7TT9M2CDE6E");

        /* prosa "nid" in textu + "nid" in valore attributi:
         * numquam lectae */
        annotationes = _colligere(piscina,
            "/* <intentio res=\"nid\">dic nid verbum</intentio> */\n"
            "int a;\n", NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_NON_NIHIL (identitates);
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), ZEPHYRUM);

        /* malformata: NIHIL (numquam mintanda) */
        annotationes = _colligere(piscina,
            "/* <intentio sine fine */\nint a;\n", NIHIL);
        CREDO_NIHIL (silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM)));
    }

    /* ========================================================
     * XI. sedes mintationis: offsets textuales (frustum B)
     * ======================================================== */
    {
        Xar* annotationes;
        Xar* identitates;
        SilvaIdentitas* id;
        constans character* fons;

        imprimere("\n--- Probans sedes mintationis ---\n");

        /* attributum booleanum in elemento */
        fons = "/* <intentio nid>x</intentio> */\nint a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_AEQUALIS_S32 ((s32)id->insertio_genus,
            (s32)SILVA_INSERTIO_POST_ATTRIBUTUM);
        CREDO_VERUM (id->insertio_offset >= III);
        CREDO_VERUM (memcmp(fons + id->insertio_offset - III,
            "nid", III) == ZEPHYRUM);

        /* elementum nid nudum: post titulum */
        fons = "/* <nid/> */\nint a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_AEQUALIS_S32 ((s32)id->insertio_genus,
            (s32)SILVA_INSERTIO_POST_TITULUM);
        CREDO_VERUM (memcmp(fons + id->insertio_offset - III,
            "nid", III) == ZEPHYRUM);
        CREDO_VERUM (fons[id->insertio_offset] == '/');

        /* v booleanum in elemento nid */
        fons = "/* <nid v/> */\nint a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_AEQUALIS_S32 ((s32)id->insertio_genus,
            (s32)SILVA_INSERTIO_POST_ATTRIBUTUM);
        CREDO_VERUM (fons[id->insertio_offset - I] == 'v');

        /* tag multilineare decoratum: sedes trans decorationem */
        fons =
            "/*\n"
            " * <intentio\n"
            " *   nid>corpus</intentio>\n"
            " */\n"
            "int a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_VERUM (id->insertio_offset >= III);
        CREDO_VERUM (memcmp(fons + id->insertio_offset - III,
            "nid", III) == ZEPHYRUM);
        CREDO_VERUM (fons[id->insertio_offset] == '>');

        /* petitiones duae uno commentario: ordo documenti */
        fons = "/* <intentio nid><causa nid>q</causa></intentio> */\n"
            "int a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), II);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_VERUM (id->insertio_offset >= ZEPHYRUM);
        {
            SilvaIdentitas* secunda = (SilvaIdentitas*)xar_obtinere(
                identitates, I);

            CREDO_VERUM (secunda->petitio);
            CREDO_VERUM (secunda->insertio_offset
                > id->insertio_offset);
        }

        /* nid="true" literalis: arbor petitionem videt, textus
         * valutam - dissensus -> offset -1 (instrumentum recusat) */
        fons = "/* <intentio nid=\"true\">x</intentio> */\nint a;\n";
        annotationes = _colligere(piscina, fons, NIHIL);
        identitates = silva_annotationes_identitates(piscina,
            _annotatio(annotationes, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (xar_numerus(identitates), I);
        id = (SilvaIdentitas*)xar_obtinere(identitates, ZEPHYRUM);
        CREDO_VERUM (id->petitio);
        CREDO_AEQUALIS_S32 (id->insertio_offset, -I);
    }

    /* ========================================================
     * IX. commentaria vacua nihil frangunt
     * ======================================================== */
    {
        constans character* fons =
            "/**/\n"
            "/* */\n"
            "//\n"
            "int i;\n";
        Xar* annotationes = _colligere(piscina, fons, NIHIL);

        imprimere("\n--- Probans commentaria vacua ---\n");
        CREDO_NON_NIHIL (annotationes);
        CREDO_AEQUALIS_I32 (xar_numerus(annotationes), ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
