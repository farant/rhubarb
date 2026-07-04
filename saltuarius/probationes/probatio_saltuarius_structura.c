/* probatio_saltuarius_structura.c - Index structurae / TOC
 * (Chunk D1)
 *
 * Fixum: plagula princeps cum inclusione resoluta (amicus.h,
 * praebenda) + inclusione ignota (<ignotum.h>) + definitionibus
 * (V bis - diarium! - et G functio) + regionibus (#if 0 numquam,
 * #ifdef omissum, #else sumptum). amicus.h IPSA includit et
 * definit - FILTRUM fons_princeps ea celare debet (exemplar
 * latina.h/stddef.h).
 *
 * NB "structura" macro latina (= struct) - variabiles "index".
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_structura.h"
#include "saltuarius_visum.h"
#include "saltuarius_proba.h"
#include <stdio.h>
#include <string.h>

interior b32
_continet (chorda textus, constans character* quaesitum)
{
    i32 m = ZEPHYRUM;
    i32 k;

    dum (quaesitum[m] != '\0')
    {
        m++;
    }
    si (m == ZEPHYRUM || textus.mensura < m)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + m <= textus.mensura; k++)
    {
        si (memcmp(textus.datum + k, quaesitum,
            (memoriae_index)m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Primus ordo cuius titulus quaesitum continet; -1 si nullus */
interior s32
_ordo_cum (constans SaltuariusStructura* index,
    constans character* quaesitum)
{
    s32 k;

    per (k = ZEPHYRUM; k < (s32)index->numerus; k++)
    {
        si (_continet(index->ordines[k].titulus, quaesitum))
        {
            redde k;
        }
    }
    redde -I;
}

interior i32
_ordines_cum (constans SaltuariusStructura* index,
    constans character* quaesitum)
{
    i32 summa = ZEPHYRUM;
    s32 k;

    per (k = ZEPHYRUM; k < (s32)index->numerus; k++)
    {
        si (_continet(index->ordines[k].titulus, quaesitum))
        {
            summa++;
        }
    }
    redde summa;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    SaltuariusNexus* nexus;
    SaltuariusLiber* liber;
    SaltuariusStructura* index;
    constans character* FONS =
        "#include \"amicus.h\"\n"      /* linea 1 */
        "#include <ignotum.h>\n"       /* linea 2 */
        "#define V 5\n"                /* linea 3 */
        "#define G(x) ((x)+(x))\n"     /* linea 4 */
        "#define V 6\n"                /* linea 5 */
        "#if 0\n"                      /* linea 6 */
        "int a;\n"
        "#endif\n"
        "#ifdef ABSENS\n"              /* linea 9 */
        "int b;\n"                     /* linea 10 */
        "#else\n"                      /* linea 11 */
        "int c;\n"                     /* linea 12 */
        "#endif\n"
        "#ifdef VACUUS\n"              /* linea 14: bracchium
                                        * VACUUM (corpus -1) */
        "#endif\n"
        "G(2);\n"                      /* linea 16 */
        "typedef int T;\n"             /* linea 17: TYPI */
        "int quadratum(int x) { return x * x; }\n"
                                       /* linea 18: FUNCTIONES */
        "int duplum(int x);\n"         /* linea 19: PROTOTYPA */
        "nomen structura { int n; } Probandum;\n";
                                       /* linea 20: LATINA! sine
                                        * contextu = falsa functio
                                        * "structura"; cum contextu
                                        * nexus = typedef struct ->
                                        * TYPI (M2d A) */

    piscina = piscina_generare_dynamicum("probatio_salt_index",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    CREDO_NON_NIHIL (nexus);
    {
        SaltuariusCaput capita[I];

        capita[ZEPHYRUM].titulus = chorda_ex_literis("amicus.h",
            piscina);
        capita[ZEPHYRUM].textus = chorda_ex_literis(
            "#include <absens.h>\n#define CELATUM 1\n"
            "int ex_amico(void);\n", piscina);
        capita[ZEPHYRUM].via_absoluta = chorda_ex_literis(
            "/abs/amicus.h", piscina);
        CREDO_AEQUALIS_S32 (
            saltuarius_nexus_silvam_parare(nexus, capita, I), I);
    }
    liber = saltuarius_liber_aperire(piscina, nexus,
        chorda_ex_literis("probe.c", piscina),
        chorda_ex_literis(FONS, piscina));
    CREDO_NON_NIHIL (liber);
    CREDO_NON_NIHIL (liber->parsura);
    index = saltuarius_structura_creare(piscina);
    CREDO_NON_NIHIL (index);

    /* ========================================================
     * PROBARE: aedificatio - sectiones, filtrum, insignia
     * ======================================================== */
    {
        s32 k;

        imprimere("\n--- Probans aedificationem ---\n");

        CREDO_VERUM (saltuarius_structura_aedificare(index, liber, nexus));
        CREDO_VERUM (index->apertum);
        per (k = ZEPHYRUM; k < (s32)index->numerus; k++)
        {
            imprimere("  [%d]%s %.*s\n", (int)k,
                index->ordines[k].saltabile ? "*" : " ",
                (int)index->ordines[k].titulus.mensura,
                (constans character*)
                    index->ordines[k].titulus.datum);
        }

        /* sectiones tres */
        CREDO_VERUM (_ordo_cum(index, "INCLUSIONES") >= ZEPHYRUM);
        CREDO_VERUM (_ordo_cum(index, "DEFINITIONES") >= ZEPHYRUM);
        CREDO_VERUM (_ordo_cum(index, "REGIONES") >= ZEPHYRUM);

        /* inclusiones: resoluta saltabilis, ignota insignita */
        {
            s32 amicus = _ordo_cum(index, "amicus.h");
            s32 ignotum = _ordo_cum(index, "ignotum.h");

            CREDO_VERUM (amicus >= ZEPHYRUM);
            CREDO_VERUM (index->ordines[amicus].saltabile);
            CREDO_AEQUALIS_I32 (
                (i32)index->ordines[amicus].genus,
                (i32)SALT_ORDO_INCLUSIO);
            CREDO_VERUM (ignotum >= ZEPHYRUM);
            CREDO_FALSUM (index->ordines[ignotum].saltabile);
            CREDO_VERUM (_continet(
                index->ordines[ignotum].titulus,
                "(non resoluta)"));
        }

        /* FILTRUM: quae amicus.h includit/definit ABSUNT -
         * etiam PROTOTYPA eius (filtrum fons_princeps sectionum
         * c89, M2d A+: quaestio Fran "omnis plagula omnes
         * functiones inclusas monstraret?") */
        CREDO_AEQUALIS_S32 (_ordo_cum(index, "absens.h"), -I);
        CREDO_AEQUALIS_S32 (_ordo_cum(index, "CELATUM"), -I);
        CREDO_AEQUALIS_S32 (_ordo_cum(index, "ex_amico"), -I);

        /* definitiones: G functio cum linea; V BIS (diarium) */
        {
            s32 g = _ordo_cum(index, "G()");

            CREDO_VERUM (g >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)index->ordines[g].genus,
                (i32)SALT_ORDO_DEFINITIO);
            CREDO_AEQUALIS_I32 (index->ordines[g].linea, IV);
            CREDO_VERUM (_continet(index->ordines[g].titulus,
                ":4"));
        }
        CREDO_AEQUALIS_I32 (_ordines_cum(index, "V  :"), II);

        /* regiones: numquam / omissum / sumptum sine insigni.
         * Linea = CORPUS bracchii (ex corpus_initium - vista
         * lineam regionis communem fert, #else :9 daret!) */
        CREDO_VERUM (_ordo_cum(index, "(numquam)") >= ZEPHYRUM);
        CREDO_VERUM (_ordo_cum(index, "(omissum)") >= ZEPHYRUM);
        {
            s32 alter = _ordo_cum(index, "#else");

            CREDO_VERUM (alter >= ZEPHYRUM);
            CREDO_VERUM (index->ordines[alter].saltabile);
            CREDO_AEQUALIS_I32 (index->ordines[alter].linea, XII);
        }
        /* bracchium vacuum: refugium = linea regionis (14) */
        CREDO_VERUM (_ordo_cum(index, ":14") >= ZEPHYRUM);

        /* sectiones c89 (M2c Chunk D): FUNCTIONES / TYPI /
         * DECLARATIONES post sectiones praeprocessoris */
        CREDO_VERUM (_ordo_cum(index, "FUNCTIONES") >= ZEPHYRUM);
        CREDO_VERUM (_ordo_cum(index, "FUNCTIONES")
            > _ordo_cum(index, "REGIONES"));
        CREDO_VERUM (_ordo_cum(index, "TYPI") >= ZEPHYRUM);
        CREDO_VERUM (_ordo_cum(index, "DECLARATIONES")
            >= ZEPHYRUM);

        /* FUNCTIO: subscriptio octetim ("int quadratum(int x)")
         * + linea; saltabilis */
        {
            s32 quadratum = _ordo_cum(index, "quadratum");

            CREDO_VERUM (quadratum >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 (
                (i32)index->ordines[quadratum].genus,
                (i32)SALT_ORDO_FUNCTIO);
            CREDO_VERUM (index->ordines[quadratum].saltabile);
            CREDO_AEQUALIS_I32 (
                index->ordines[quadratum].linea, XVIII);
            CREDO_VERUM (_continet(
                index->ordines[quadratum].titulus,
                "int quadratum(int x)"));
        }

        /* TYPUS: typedef T linea XVII */
        {
            s32 t = _ordo_cum(index, "T  :17");

            CREDO_VERUM (t >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)index->ordines[t].genus,
                (i32)SALT_ORDO_TYPUS);
        }

        /* LATINA per contextum nexus (M2d A): "nomen structura
         * { } Probandum;" expanditur -> typedef -> TYPI. Sine
         * contextu haec linea functio falsa "structura" esset
         * (probatio manualis Fran in silva_scribere.c). */
        {
            s32 p = _ordo_cum(index, "Probandum");

            CREDO_VERUM (p >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)index->ordines[p].genus,
                (i32)SALT_ORDO_TYPUS);
            CREDO_AEQUALIS_I32 (index->ordines[p].linea, XX);
        }
        CREDO_AEQUALIS_I32 (_ordines_cum(index,
            "nomen structura"), ZEPHYRUM);

        /* PROTOTYPUM: duplum() linea XIX - sectio propria (Fran
         * 2026-07-04), NON inter declarationes */
        {
            s32 d = _ordo_cum(index, "duplum()");

            CREDO_VERUM (_ordo_cum(index, "PROTOTYPA")
                >= ZEPHYRUM);
            CREDO_VERUM (d >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)index->ordines[d].genus,
                (i32)SALT_ORDO_PROTOTYPUM);
            CREDO_AEQUALIS_I32 (index->ordines[d].linea, XIX);
            CREDO_VERUM (index->ordines[d].saltabile);
        }

        /* DECLARATIO: "int c;" ex bracchio SUMPTO (#else) linea
         * XII; bracchia non sumpta ABSUNT (a linea 7 numquam) */
        {
            s32 c = _ordo_cum(index, "c  :12");

            CREDO_VERUM (c >= ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)index->ordines[c].genus,
                (i32)SALT_ORDO_DECLARATIO);
        }
        CREDO_AEQUALIS_S32 (_ordo_cum(index, "a  :7"), -I);
    }

    /* ========================================================
     * PROBARE: selectio, motus, saltus
     * ======================================================== */
    {
        imprimere("\n--- Probans motum et saltum ---\n");

        /* selectio initialis: primus saltabilis (post titulum) */
        CREDO_VERUM (index->ordines[index->selectio].saltabile);
        {
            constans SaltuariusOrdo* ordo =
                saltuarius_structura_saltus(index);

            CREDO_NON_NIHIL ((constans vacuum*)ordo);
            CREDO_AEQUALIS_I32 ((i32)ordo->genus,
                (i32)SALT_ORDO_INCLUSIO);
            CREDO_CHORDA_AEQUALIS_LITERIS (ordo->via, "amicus.h");
        }

        /* in titulum sectionis: saltus NIHIL */
        saltuarius_structura_movere(index, -C);
        CREDO_AEQUALIS_S32 (index->selectio, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_structura_saltus(index),
            (constans vacuum*)NIHIL);

        /* ad G: saltus definitio linea IV */
        saltuarius_structura_movere(index,
            _ordo_cum(index, "G()"));
        {
            constans SaltuariusOrdo* ordo =
                saltuarius_structura_saltus(index);

            CREDO_NON_NIHIL ((constans vacuum*)ordo);
            CREDO_AEQUALIS_I32 ((i32)ordo->genus,
                (i32)SALT_ORDO_DEFINITIO);
            CREDO_AEQUALIS_I32 (ordo->linea, IV);
        }

        /* clausura inferior */
        saltuarius_structura_movere(index, C);
        CREDO_AEQUALIS_S32 (index->selectio,
            (s32)index->numerus - I);
    }

    /* ========================================================
     * PROBARE: volumen (aptare) sequitur selectionem
     * ======================================================== */
    {
        imprimere("\n--- Probans volumen ---\n");

        /* selectio in ultimo (ex probatione priore) */
        saltuarius_structura_aptare(index, III);
        CREDO_AEQUALIS_S32 (index->volumen,
            (s32)index->numerus - III);
        saltuarius_structura_movere(index, -C);
        saltuarius_structura_aptare(index, III);
        CREDO_AEQUALIS_S32 (index->volumen, ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: merus recusat
     * ======================================================== */
    {
        SaltuariusLiber* merus;

        imprimere("\n--- Probans merum ---\n");

        merus = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("adnotationes.md", piscina),
            chorda_ex_literis("# titulus\ntextus.\n", piscina));
        CREDO_NON_NIHIL (merus);
        CREDO_FALSUM (saltuarius_structura_aedificare(index,
            merus, nexus));
        CREDO_FALSUM (index->apertum);
    }

    /* ========================================================
     * PROBARE: aurei cellularum - tabula super scaenam
     * ======================================================== */
    {
        TesseraPiscina* tp;
        TesseraPonsMemoriae* pm;
        TesseraOpus* opus;
        SaltuariusRes* res;

        imprimere("\n--- Probans aureos tabulae ---\n");

        tp = tessera_piscina_generare_dynamicum(
            "salt_index_tessera", 16777216);
        pm = tessera_pons_memoriae_creare(tp, XL, XII);
        opus = tessera_aperire(tp, &pm->pons);
        res = saltuarius_res_creare(piscina, "/probe");
        CREDO_NON_NIHIL (opus);
        CREDO_NON_NIHIL (res);

        CREDO_VERUM (saltuarius_structura_aedificare(index,
            liber, nexus));
        saltuarius_visum_pingere(liber, res, opus);
        saltuarius_visum_structura(index, opus);

        /* titulus tabulae in ordine 0 */
        CREDO_VERUM (saltuarius_proba_quaerere(opus, ZEPHYRUM,
            "STRUCTURA") >= ZEPHYRUM);
        /* caput sectionis in ordine I */
        CREDO_VERUM (saltuarius_proba_quaerere(opus, I,
            "INCLUSIONES") >= ZEPHYRUM);
        /* selectio (amicus.h, ordo II) INVERSA */
        {
            s32 col = saltuarius_proba_quaerere(opus, II,
                "amicus.h");

            CREDO_VERUM (col >= ZEPHYRUM);
            CREDO_VERUM ((PROBA_CELLA(opus, col, II).ornamenta
                & TESSERA_ORNAMENTUM_INVERSUM) != ZEPHYRUM);
        }
        /* motus movet INVERSUM (repingere totum - disciplina
         * quadri) */
        saltuarius_structura_movere(index, I);
        saltuarius_visum_pingere(liber, res, opus);
        saltuarius_visum_structura(index, opus);
        {
            s32 col = saltuarius_proba_quaerere(opus, II,
                "amicus.h");
            s32 col_iii = saltuarius_proba_quaerere(opus, III,
                "ignotum.h");

            CREDO_VERUM (col >= ZEPHYRUM);
            CREDO_FALSUM ((PROBA_CELLA(opus, col, II).ornamenta
                & TESSERA_ORNAMENTUM_INVERSUM) != ZEPHYRUM);
            CREDO_VERUM (col_iii >= ZEPHYRUM);
            CREDO_VERUM ((PROBA_CELLA(opus, col_iii, III)
                .ornamenta & TESSERA_ORNAMENTUM_INVERSUM)
                != ZEPHYRUM);
        }
        tessera_claudere(opus);
        tessera_piscina_destruere(tp);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
