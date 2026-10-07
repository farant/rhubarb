/* probatio_silva_conflatio.c - mechanismus plagulae unius (extractus
 * ex briar_amalgama 2026-10-07; porta extractionis = aurum briar
 * gamma.c octetim, HAEC probatio = mores directi): inclusio localis,
 * capita/fontes vendicati, tabula staticorum, ordo capitum
 * (postulata_posix.h primum, post-ordine), ordo fontium (gemellus post
 * caput), emissio (#line, inclusiones vacuae, #define/#undef, macra
 * #undef solum).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "silva_conflatio.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

interior constans character* constans RADICES[] = {
    "materia/fontes/", NIHIL
};

interior Piscina* piscina;

interior vacuum
_addere (
                    Xar* clausura,
     constans character* via,
     constans character* contentum);

interior vacuum
_addere (
                    Xar* clausura,
     constans character* via,
     constans character* contentum)
{
    ConflatioPlagula* p = (ConflatioPlagula*)xar_addere(clausura);

    p->via        = chorda_ex_literis(via, piscina);
    p->contentum  = chorda_ex_literis(contentum, piscina);
}

/* viae ordinis iunctae per ' ' */
interior chorda
_ordo (
    Xar* ordo);

interior chorda
_ordo (
    Xar* ordo)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, 256);
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        constans ConflatioPlagula* p =
            *(constans ConflatioPlagula**)xar_obtinere(ordo, i);

        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " ");
        }
        chorda_aedificator_appendere_chorda(a, p->via);
    }
    redde chorda_aedificator_finire(a);
}

s32 principale (vacuum)
{
                   b32 praeteritus;
    ConflatioContextus contextus;

    piscina = piscina_generare_dynamicum("probatio_conflationis",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    contextus.radices_clientium  = RADICES;
    contextus.statica            = NIHIL;

    imprimere("\n--- Probans inclusionem localem ---\n");
    {
        chorda n;

        CREDO_VERUM(conflatio_inclusio_localis(chorda_ex_literis(
            "#include \"piscina.h\"", piscina), &n));
        CREDO_CHORDA_AEQUALIS_LITERIS(n, "piscina.h");
        CREDO_VERUM(conflatio_inclusio_localis(chorda_ex_literis(
            "  #  include\t\"a/b.h\"", piscina), &n));
        CREDO_CHORDA_AEQUALIS_LITERIS(n, "a/b.h");
        CREDO_FALSUM(conflatio_inclusio_localis(chorda_ex_literis(
            "#include <stdio.h>", piscina), &n));
        CREDO_FALSUM(conflatio_inclusio_localis(chorda_ex_literis(
            "#include \"tabula.c\"", piscina), &n));
        CREDO_FALSUM(conflatio_inclusio_localis(chorda_ex_literis(
            "/* #include \"x.h\" */", piscina), &n));
        CREDO_FALSUM(conflatio_inclusio_localis(chorda_ex_literis(
            "#include \"h\"", piscina), &n));
        CREDO_FALSUM(conflatio_inclusio_localis(chorda_ex_literis(
            "#define include \"x.h\"", piscina), &n));
    }

    imprimere("\n--- Probans capita et fontes vendicatos ---\n");
    {
        CREDO_VERUM(conflatio_caput_est(&contextus, chorda_ex_literis(
            "include/a.h", piscina)));
        CREDO_VERUM(conflatio_caput_est(&contextus, chorda_ex_literis(
            "materia/fontes/m.h", piscina)));
        CREDO_FALSUM(conflatio_caput_est(&contextus, chorda_ex_literis(
            "materia/fontes/m.c", piscina)));
        CREDO_FALSUM(conflatio_caput_est(&contextus, chorda_ex_literis(
            "knotapel/x.h", piscina)));
        CREDO_VERUM(conflatio_fons_est(&contextus, chorda_ex_literis(
            "lib/a.c", piscina)));
        CREDO_VERUM(conflatio_fons_est(&contextus, chorda_ex_literis(
            "materia/fontes/m.c", piscina)));
        CREDO_FALSUM(conflatio_fons_est(&contextus, chorda_ex_literis(
            "knotapel/demo/main.c", piscina)));
        CREDO_CHORDA_AEQUALIS_LITERIS(conflatio_stirps(
            chorda_ex_literis("lib/chorda_aedificator.c", piscina)),
            "chorda_aedificator");
        CREDO_CHORDA_AEQUALIS_LITERIS(conflatio_stirps(
            chorda_ex_literis("a.b/c", piscina)), "c");
    }

    imprimere("\n--- Probans tabulam staticorum ---\n");
    {
                            vacuum* v = NIHIL;
                               Xar* ordines;
        constans ConflatioStaticum* s;

        CREDO_VERUM(conflatio_statica_legere(piscina, chorda_ex_literis(
            "# commentarium\n"
            "_f\tfunctio\tlib/a.c\n"
            "M\tmacro\tlib/a.c\n"
            "publicum\tfunctio\tinclude/a.h\n"
            "_g\tfunctio\tknotapel/demo/main.c\n"
            "solum\n"
            "_h\tvariabile\tmateria/fontes/m.c\n", piscina),
            &contextus));
        CREDO_VERUM(tabula_dispersa_invenire(contextus.statica,
            chorda_ex_literis("lib/a.c", piscina), &v));
        ordines = (Xar*)v;
        CREDO_AEQUALIS_I32((i32)xar_numerus(ordines), (i32)II);
        s = (constans ConflatioStaticum*)xar_obtinere(ordines,
            ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(s->symbolum, "_f");
        CREDO_CHORDA_AEQUALIS_LITERIS(s->genus, "functio");
        CREDO_VERUM(tabula_dispersa_invenire(contextus.statica,
            chorda_ex_literis("materia/fontes/m.c", piscina), &v));
        CREDO_FALSUM(tabula_dispersa_invenire(contextus.statica,
            chorda_ex_literis("include/a.h", piscina), &v));
        CREDO_FALSUM(tabula_dispersa_invenire(contextus.statica,
            chorda_ex_literis("knotapel/demo/main.c", piscina), &v));
    }

    imprimere("\n--- Probans ordinem capitum et fontium ---\n");
    {
        Xar* clausura = xar_creare(piscina,
            (i32)magnitudo(ConflatioPlagula));
        Xar* capita;
        Xar* fontes;

        /* c includit b et a et m (radix clientis); b includit a;
         * ordo clausurae alienus: c, a, b, postulata, m */
        _addere(clausura, "include/c.h",
            "#include \"b.h\"\n#include \"a.h\"\n#include \"m.h\"\n");
        _addere(clausura, "include/a.h", "int a;\n");
        _addere(clausura, "include/b.h", "#include \"a.h\"\nint b;\n");
        _addere(clausura, "include/postulata_posix.h", "/* p */\n");
        _addere(clausura, "materia/fontes/m.h", "int m;\n");
        _addere(clausura, "lib/z.c", "int z;\n");
        _addere(clausura, "lib/c.c", "int cc;\n");
        _addere(clausura, "materia/fontes/m.c", "int mm;\n");
        _addere(clausura, "lib/a.c", "int aa;\n");
        _addere(clausura, "knotapel/demo/main.c", "int main;\n");

        capita = conflatio_capita_ordinare(piscina, &contextus,
            clausura);
        CREDO_CHORDA_AEQUALIS_LITERIS(_ordo(capita),
            "include/postulata_posix.h include/a.h include/b.h "
            "materia/fontes/m.h include/c.h");
        /* gemelli ordine capitum (b sine fonte), deinde reliqui ordine
         * clausurae; principale (non vendicatum) nusquam */
        fontes = conflatio_fontes_ordinare(piscina, &contextus,
            clausura, capita);
        CREDO_CHORDA_AEQUALIS_LITERIS(_ordo(fontes),
            "lib/a.c materia/fontes/m.c lib/c.c lib/z.c");
    }

    imprimere("\n--- Probans emissionem ---\n");
    {
        ChordaAedificator* a;
         ConflatioPlagula  pl;

        /* inclusio localis -> linea vacua; systematis manet; linea
         * ultima sine '\n' terminatur */
        a       = chorda_aedificator_creare(piscina, 256);
        pl.via  = chorda_ex_literis("include/a.h", piscina);
        pl.contentum  = chorda_ex_literis(
            "#include \"latina.h\"\n#include <stdio.h>\nint a;",
            piscina);
        conflatio_plagulam_emittere(a, &pl);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(a),
            "#line 1 \"include/a.h\"\n\n#include <stdio.h>\nint a;\n");

        /* fons: #define ante (macra praetermissa), #undef post
         * omnium */
        a       = chorda_aedificator_creare(piscina, 256);
        pl.via  = chorda_ex_literis("lib/a.c", piscina);
        pl.contentum  = chorda_ex_literis(
            "#include \"a.h\"\nstatic int _f(void) { return M; }\n",
            piscina);
        conflatio_fontem_emittere(a, &contextus, &pl);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(a),
            "/* lib/a.c: statica per plagulam renominata */\n"
            "#define _f _f_a\n"
            "#line 1 \"lib/a.c\"\n"
            "\n"
            "static int _f(void) { return M; }\n"
            "#undef _f\n"
            "#undef M\n");

        /* fons sine staticis: nec commentarium nec #undef */
        a             = chorda_aedificator_creare(piscina, 256);
        pl.via        = chorda_ex_literis("lib/z.c", piscina);
        pl.contentum  = chorda_ex_literis("int z;\n", piscina);
        conflatio_fontem_emittere(a, &contextus, &pl);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(a),
            "#line 1 \"lib/z.c\"\nint z;\n");

        /* statica data, suffixum datum (membra briar) */
        {
                         Xar* statica = xar_creare(piscina,
                             (i32)magnitudo(ConflatioStaticum));
            ConflatioStaticum* s = (ConflatioStaticum*)xar_addere(
                statica);

            s->symbolum  = chorda_ex_literis("_g", piscina);
            s->genus     = chorda_ex_literis("", piscina);
            a            = chorda_aedificator_creare(piscina, 256);
            pl.via        = chorda_ex_literis("fontes/m_regiones.c",
                piscina);
            pl.contentum  = chorda_ex_literis("int _g;\n", piscina);
            conflatio_renominatam_emittere(a, &pl, statica,
                chorda_ex_literis("m", piscina), "nota");
            CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(a),
                "/* fontes/m_regiones.c: nota */\n"
                "#define _g _g_m\n"
                "#line 1 \"fontes/m_regiones.c\"\n"
                "int _g;\n"
                "#undef _g\n");
        }
    }
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
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
