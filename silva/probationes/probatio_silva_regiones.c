/* probatio_silva_regiones.c - Probationes regionum conditionalium,
 * custodum, includendorum (Chunk D) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaExpansio*
_praeparare (Piscina* piscina, constans character* fons, Xar** reliqua_out)
{
    SilvaExpansio* exp;
    Xar* lexemata;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "probatio.c", FALSUM);
    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    *reliqua_out = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
    redde exp;
}

interior SilvaToken*
_ad (Xar* lexemata, i32 i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_regiones", 2097152);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: #ifdef duorum ramorum - ramus primus sumptus
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        SilvaRamus* ramus;
        Xar* reliqua;

        imprimere("\n--- Probans #ifdef sumptum ---\n");

        exp = _praeparare(piscina,
            "#define HAS 1\n#ifdef HAS\nint a;\n#else\nint b;\n#endif\n",
            &reliqua);

        /* reliqua: int a ; EOF - 'b' absens */
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "a");

        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(regio->rami), II);
        CREDO_FALSUM (regio->est_imperfecta);

        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)ramus->genus, (i32)SILVA_RAMUS_IFDEF);
        CREDO_VERUM (ramus->est_sumptum);
        CREDO_VERUM (ramus->est_evaluatum);
        CREDO_NIHIL (ramus->lexemata_cruda);
        CREDO_VERUM (ramus->conditio_id > ZEPHYRUM);

        /* ramus alter: NON evaluatus (post sumptum), lamina cruda */
        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, I);
        CREDO_AEQUALIS_I32 ((i32)ramus->genus, (i32)SILVA_RAMUS_ELSE);
        CREDO_FALSUM (ramus->est_sumptum);
        CREDO_FALSUM (ramus->est_evaluatum);
        CREDO_NON_NIHIL (ramus->lexemata_cruda);
        CREDO_AEQUALIS_I32 (xar_numerus(ramus->lexemata_cruda), III);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _ad(ramus->lexemata_cruda, I)->valor, "b");
    }


    /* ========================================================
     * PROBARE: ramus primus non sumptus - else sumitur
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        SilvaRamus* ramus;
        Xar* reliqua;

        imprimere("\n--- Probans #ifdef non sumptum ---\n");

        exp = _praeparare(piscina,
            "#ifdef NOPE\nint a;\n#else\nint b;\n#endif\n", &reliqua);

        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "b");

        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, ZEPHYRUM);
        CREDO_FALSUM (ramus->est_sumptum);
        CREDO_VERUM (ramus->est_evaluatum);
        CREDO_AEQUALIS_I32 (xar_numerus(ramus->lexemata_cruda), III);
        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, I);
        CREDO_VERUM (ramus->est_sumptum);
    }


    /* ========================================================
     * PROBARE: catena #if/#elif/#else
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        SilvaRamus* ramus;
        Xar* reliqua;

        imprimere("\n--- Probans catenam elif ---\n");

        exp = _praeparare(piscina,
            "#define V 2\n"
            "#if V == 1\nint a;\n#elif V == 2\nint b;\n#else\nint c;\n#endif\n",
            &reliqua);

        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "b");

        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(regio->rami), III);

        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)ramus->genus, (i32)SILVA_RAMUS_IF);
        CREDO_VERUM (ramus->est_evaluatum);
        CREDO_FALSUM (ramus->est_sumptum);

        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, I);
        CREDO_AEQUALIS_I32 ((i32)ramus->genus, (i32)SILVA_RAMUS_ELIF);
        CREDO_VERUM (ramus->est_sumptum);

        /* post sumptum: non evaluatus */
        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, II);
        CREDO_FALSUM (ramus->est_evaluatum);
        CREDO_FALSUM (ramus->est_sumptum);
    }


    /* ========================================================
     * PROBARE: definitiones in ramis - duae viae
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaEventum* eventum;
        Xar* reliqua;

        imprimere("\n--- Probans definitiones in ramis ---\n");

        /* in ramo sumpto: applicata + acta cum conditio_id */
        exp = _praeparare(piscina,
            "#define A 1\n#ifdef A\n#define B 2\n#endif\nint x;\n",
            &reliqua);
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("B")));
        CREDO_AEQUALIS_I32 (xar_numerus(exp->acta), II);
        eventum = (SilvaEventum*)xar_obtinere(exp->acta, I);
        CREDO_VERUM (eventum->conditio_id > ZEPHYRUM);

        /* in ramo NON sumpto: nec applicata nec in actis */
        exp = _praeparare(piscina,
            "#ifdef NOPE\n#define W 3\n#endif\nint x;\n", &reliqua);
        CREDO_NIHIL (silva_expansio_quaerere(exp, _ch("W")));
        CREDO_AEQUALIS_I32 (xar_numerus(exp->acta), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: regiones nidificatae
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        Xar* reliqua;

        imprimere("\n--- Probans nidificationem ---\n");

        /* in ramo sumpto: regio filia */
        exp = _praeparare(piscina,
            "#define A 1\n#define B 1\n"
            "#ifdef A\n#ifdef B\nint ab;\n#endif\n#endif\n", &reliqua);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "ab");
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(regio->filiae), I);
        regio = *(SilvaRegio**)xar_obtinere(regio->filiae, ZEPHYRUM);
        CREDO_NON_NIHIL (regio->pater);

        /* in ramo NON sumpto: profunditas numerata, nulla filia */
        exp = _praeparare(piscina,
            "#ifdef NOPE\n#ifdef X\nint malum;\n#endif\nint malum2;\n#endif\n"
            "int bonum;\n", &reliqua);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "bonum");
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(regio->filiae), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: #if 0 idioma (est_numquam)
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        SilvaRamus* ramus;
        Xar* reliqua;

        imprimere("\n--- Probans #if 0 ---\n");

        exp = _praeparare(piscina,
            "#if 0\npurgamentum $$$ hic\n#endif\nint ok;\n", &reliqua);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "ok");

        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        ramus = *(SilvaRamus**)xar_obtinere(regio->rami, ZEPHYRUM);
        CREDO_VERUM (ramus->est_numquam);
        CREDO_FALSUM (ramus->est_sumptum);
        CREDO_NON_NIHIL (ramus->lexemata_cruda);
    }


    /* ========================================================
     * PROBARE: regio imperfecta (EOF ante #endif) - nullus fragor
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaRegio* regio;
        Xar* reliqua;

        imprimere("\n--- Probans regionem imperfectam ---\n");

        exp = _praeparare(piscina, "#ifdef A\nint a;\n", &reliqua);
        regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
        CREDO_VERUM (regio->est_imperfecta);

        /* #endif sine regione: transit ad reliqua */
        exp = _praeparare(piscina, "int a;\n#endif\nint b;\n", &reliqua);
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IX);  /* omnia manent */
    }


    /* ========================================================
     * PROBARE: custos (est_custos) - transparentia
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaFons* fons;
        Xar* reliqua;
        Xar* lexemata;

        imprimere("\n--- Probans custodem ---\n");

        /* plagula custodita directe processata: NULLA regio */
        exp = _praeparare(piscina,
            "#ifndef CUSTOS_H\n#define CUSTOS_H\nint g;\n#endif\n", &reliqua);
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);  /* int g ; EOF */
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("CUSTOS_H")));
        fons = (SilvaFons*)xar_obtinere(exp->fontes, ZEPHYRUM);
        CREDO_VERUM (fons->est_custos);
        CREDO_NON_NIHIL (fons->custos_titulus);
        CREDO_CHORDA_AEQUALIS_LITERIS (*fons->custos_titulus, "CUSTOS_H");

        /* quasi-custos: contentum post #endif - regio normalis */
        exp = _praeparare(piscina,
            "#ifndef NG_H\n#define NG_H\nint x;\n#endif\nint post;\n",
            &reliqua);
        fons = (SilvaFons*)xar_obtinere(exp->fontes, ZEPHYRUM);
        CREDO_FALSUM (fons->est_custos);
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), VII);

        /* custos IAM definitus ante processionem (sim ⑥ C7, decisio
         * Fran): NON transparentia - ambulator normalis, #ifndef
         * falsum, interior ramus non sumptus (extra rationem sed
         * numquam perditus), lineae captae */
        {
            Xar* lexemata_c7;
            Xar* directivae_c7;
            SilvaRegio* regio;
            SilvaRamus* ramus;

            exp = silva_expansio_creare(piscina);
            silva_fons_addere(exp, "probatio.c", FALSUM);
            CREDO_VERUM (silva_macro_addere(exp, "CUSTOS_PRAE", "1"));
            lexemata_c7 = silva_lexare(piscina,
                "#ifndef CUSTOS_PRAE\n#define CUSTOS_PRAE\n"
                "int g;\n#endif\n",
                (i32)strlen("#ifndef CUSTOS_PRAE\n#define CUSTOS_PRAE\n"
                "int g;\n#endif\n"), ZEPHYRUM);
            directivae_c7 = NIHIL;
            reliqua = silva_expansio_directivas_processare(exp,
                lexemata_c7, &directivae_c7);

            /* interior NON processatus: reliqua = EOF solum; regio
             * una cum ramo non sumpto, lamina cruda interiorem fert */
            CREDO_AEQUALIS_I32 (xar_numerus(reliqua), I);
            CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
            regio = *(SilvaRegio**)xar_obtinere(exp->regiones, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(regio->rami), I);
            ramus = *(SilvaRamus**)xar_obtinere(regio->rami, ZEPHYRUM);
            CREDO_FALSUM (ramus->est_sumptum);
            CREDO_NON_NIHIL (ramus->lexemata_cruda);
            CREDO_MAIOR_I32 (xar_numerus(ramus->lexemata_cruda),
                ZEPHYRUM);
            /* β (sim ⑦ C2): lineae structurales regioni IPSI - ramus
             * lineam #ifndef fert, regio lineam #endif; NIHIL in
             * fluxu directivarum (dominus unus etiam pro lineis) */
            CREDO_AEQUALIS_I32 (xar_numerus(directivae_c7), ZEPHYRUM);
            CREDO_NON_NIHIL (ramus->directiva);
            CREDO_MAIOR_I32 (xar_numerus(ramus->directiva), ZEPHYRUM);
            CREDO_NON_NIHIL (regio->directiva_finis);
            CREDO_MAIOR_I32 (xar_numerus(regio->directiva_finis),
                ZEPHYRUM);
            /* definitio interior numquam registrata (semantica cpp:
             * interior praetermissus definitionem non currit) -
             * CUSTOS_PRAE ipsum ex API manet */
            {
                SilvaMacroDef* def = silva_expansio_quaerere(exp,
                    _ch("CUSTOS_PRAE"));

                CREDO_NON_NIHIL (def);
                CREDO_VERUM (def->ex_api);
            }
        }

        /* includere bis: interior semel, secunda praetermissa */
        {
            SilvaInclusio* inclusio;
            i32 i;
            i32 numerus_g;

            exp = silva_expansio_creare(piscina);
            silva_fons_addere(exp, "principalis.c", FALSUM);
            silva_includendum_praebere(exp, "custos.h",
                "#ifndef CUSTOS_H\n#define CUSTOS_H\nint g;\n#endif\n",
                (i32)strlen(
                "#ifndef CUSTOS_H\n#define CUSTOS_H\nint g;\n#endif\n"));
            lexemata = silva_lexare(piscina,
                "#include \"custos.h\"\n#include \"custos.h\"\nint m;\n",
                (i32)strlen(
                "#include \"custos.h\"\n#include \"custos.h\"\nint m;\n"),
                ZEPHYRUM);
            reliqua = silva_expansio_directivas_processare(exp, lexemata,
                NIHIL);

            /* g semel tantum */
            numerus_g = ZEPHYRUM;
            per (i = ZEPHYRUM; i < xar_numerus(reliqua); i++)
            {
                SilvaToken* t;

                t = _ad(reliqua, i);
                si (t->valor.mensura == I && t->valor.datum[ZEPHYRUM] == 'g')
                {
                    numerus_g++;
                }
            }
            CREDO_AEQUALIS_I32 (numerus_g, I);
            CREDO_AEQUALIS_I32 (xar_numerus(reliqua), VII);

            CREDO_AEQUALIS_I32 (xar_numerus(exp->inclusiones), II);
            inclusio = (SilvaInclusio*)xar_obtinere(exp->inclusiones, ZEPHYRUM);
            CREDO_FALSUM (inclusio->est_praetermissa);
            CREDO_VERUM (inclusio->fons_ad >= ZEPHYRUM);
            inclusio = (SilvaInclusio*)xar_obtinere(exp->inclusiones, I);
            CREDO_VERUM (inclusio->est_praetermissa);
        }
    }


    /* ========================================================
     * PROBARE: includenda - insertio, discens, profunditas
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaInclusio* inclusio;
        Xar* reliqua;
        Xar* lexemata;

        imprimere("\n--- Probans includenda ---\n");

        /* insertio in loco */
        exp = silva_expansio_creare(piscina);
        silva_fons_addere(exp, "principalis.c", FALSUM);
        silva_includendum_praebere(exp, "medius.h", "int medius;",
            (i32)strlen("int medius;"));
        lexemata = silva_lexare(piscina,
            "int ante;\n#include \"medius.h\"\nint post;\n",
            (i32)strlen("int ante;\n#include \"medius.h\"\nint post;\n"),
            ZEPHYRUM);
        reliqua = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), X);  /* 3+3+3+EOF */
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, I)->valor, "ante");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, IV)->valor, "medius");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(reliqua, VII)->valor, "post");

        /* definitiones ex includendo visibiles post insertionem */
        exp = silva_expansio_creare(piscina);
        silva_fons_addere(exp, "principalis.c", FALSUM);
        silva_includendum_praebere(exp, "defs.h", "#define QUAD 4\n",
            (i32)strlen("#define QUAD 4\n"));
        lexemata = silva_lexare(piscina, "#include \"defs.h\"\nint x;\n",
            (i32)strlen("#include \"defs.h\"\nint x;\n"), ZEPHYRUM);
        reliqua = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("QUAD")));

        /* ignotum ("discens"): via memorata, processio pergit */
        {
            Xar* r2;

            exp = _praeparare(piscina, "#include <stdio.h>\nint x;\n", &r2);
            CREDO_AEQUALIS_I32 (xar_numerus(r2), IV);
            CREDO_AEQUALIS_I32 (xar_numerus(exp->inclusiones), I);
            inclusio = (SilvaInclusio*)xar_obtinere(exp->inclusiones,
                ZEPHYRUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (*inclusio->via, "stdio.h");
            CREDO_AEQUALIS_S32 (inclusio->fons_ad, -I);
            CREDO_FALSUM (inclusio->est_praetermissa);
        }

        /* inclusio sui sine custode: profunditas capta, nullus fragor */
        exp = silva_expansio_creare(piscina);
        silva_fons_addere(exp, "principalis.c", FALSUM);
        silva_includendum_praebere(exp, "gyrus.h",
            "#include \"gyrus.h\"\nint l;",
            (i32)strlen("#include \"gyrus.h\"\nint l;"));
        lexemata = silva_lexare(piscina, "#include \"gyrus.h\"\n",
            (i32)strlen("#include \"gyrus.h\"\n"), ZEPHYRUM);
        reliqua = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
        /* XXXII gradus x (int l ;) + EOF */
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), XCVII);
    }


    /* ========================================================
     * PROBARE: sceletus integer - custos + duo rami + expansio
     * (obligatio M1: silva-spec-v2.md §10.3)
     * ======================================================== */

    {
        SilvaExpansio* exp;
        Xar* reliqua;
        Xar* lexemata;
        Xar* expansa;

        imprimere("\n--- Probans sceletum integrum ---\n");

        exp = silva_expansio_creare(piscina);
        silva_fons_addere(exp, "principalis.c", FALSUM);
        silva_includendum_praebere(exp, "config.h",
            "#ifndef CONFIG_H\n#define CONFIG_H\n#define MODUS 2\n#endif\n",
            (i32)strlen(
            "#ifndef CONFIG_H\n#define CONFIG_H\n#define MODUS 2\n#endif\n"));
        lexemata = silva_lexare(piscina,
            "#include \"config.h\"\n"
            "#if MODUS == 2\n#define GRADUS 42\n#else\n#define GRADUS 7\n"
            "#endif\n"
            "int x = GRADUS;\n",
            (i32)strlen(
            "#include \"config.h\"\n"
            "#if MODUS == 2\n#define GRADUS 42\n#else\n#define GRADUS 7\n"
            "#endif\n"
            "int x = GRADUS;\n"), ZEPHYRUM);
        reliqua = silva_expansio_directivas_processare(exp, lexemata, NIHIL);
        expansa = silva_expansio_expandere(exp, reliqua, NIHIL);

        /* int x = 42 ; EOF */
        CREDO_AEQUALIS_I32 (xar_numerus(expansa), VI);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(expansa, III)->valor, "42");
        CREDO_AEQUALIS_I32 ((i32)_ad(expansa, III)->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_AEQUALIS_I32 (xar_numerus(exp->regiones), I);
    }


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
