/* probatio_graphis_usus.c - partes in picturam positae (graphis.h)
 *
 * 'usus' partem nomine ponit: pars posita idem pingit ac formae
 * eius manu scriptae, locus additur (etiam in grege et in parte
 * alia), pars in margine crus extra habere potest, recusationes
 * plagulam et lineam nominant, pars vera ex graphis/res legitur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "graphis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT GRAPHIS_LATITUDO
#define ALT GRAPHIS_ALTITUDO

nomen structura {
    constans character* titulus;
    constans character* textus;
} Pars;

nomen structura {
    constans Pars* partes;     /* titulo NIHIL terminatae */
              i32  vocationes;
} Armarium;

/* GraphisFons super tabulam in memoria */
interior chorda
_fons (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    Armarium* a = (Armarium*)contextus;
      chorda  vacua;
         i32  k;

    a->vocationes++;
    per (k = ZEPHYRUM; a->partes[k].titulus; k++)
    {
        si (chorda_aequalis_literis(titulus, a->partes[k].titulus))
        {
            redde chorda_ex_literis(a->partes[k].textus, piscina);
        }
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

hic_manens constans Pars PARTES[] = {
    { "quadrum",
      "<pars titulus=\"quadrum\">\n"
      "<rectangulum fines=\"0 0 20 10\" colores=\"6\""
      " stilus=\"1 0\"/>\n"
      "<ovum fines=\"4 2 8 6\" colores=\"11 0\""
      " exemplar=\"tessellatum\" vis=\"1\"/>\n"
      "</pars>" },
    { "par",
      "<pars>\n"
      "<usus fons=\"quadrum\"/>\n"
      "<grex locus=\"0 20\">\n"
      "<usus fons=\"quadrum\" locus=\"30 0\"/>\n"
      "</grex>\n"
      "</pars>" },
    { "lucerna",
      "<pars>\n"
      "<rectangulum fines=\"0 0 10 10\" colores=\"#c8c8c8\""
      " lucens=\"verum\"/>\n"
      "</pars>" },
    { "se_ipsam", "<pars>\n\n<usus fons=\"se_ipsam\"/>\n</pars>" },
    { "orbis_a", "<pars>\n<usus fons=\"orbis_b\"/>\n</pars>" },
    { "orbis_b", "<pars>\n\n\n<usus fons=\"orbis_a\"/>\n</pars>" },
    { "non_stml", "<pars>\n<rectangulum" },
    { "radix_mala", "<pictura>\n</pictura>" },
    { "radix_attributum", "<pars lux=\"0.5\">\n</pars>" },
    { "cum_lumine",
      "<pars>\n<lumen locus=\"5 5\" radii=\"5 5\"/>\n</pars>" },
    { "cum_titulo",
      "<pars>\n\n<rectangulum titulus=\"a\" fines=\"0 0 20 20\""
      " colores=\"6\"/>\n</pars>" },
    { "forma_mala",
      "<pars>\n<rectangulum fines=\"0 0 20 20\" colores=\"6\"/>\n"
      "<rectangulum fines=\"0 0 20\" colores=\"6\"/>\n</pars>" },
    { "gradus_1",
        "<pars><usus fons=\"gradus_2\" locus=\"1 0\"/></pars>" },
    { "gradus_2",
        "<pars><usus fons=\"gradus_3\" locus=\"1 0\"/></pars>" },
    { "gradus_3",
        "<pars><usus fons=\"gradus_4\" locus=\"1 0\"/></pars>" },
    { "gradus_4",
        "<pars><usus fons=\"gradus_5\" locus=\"1 0\"/></pars>" },
    { "gradus_5",
        "<pars><usus fons=\"gradus_6\" locus=\"1 0\"/></pars>" },
    { "gradus_6",
        "<pars><usus fons=\"gradus_7\" locus=\"1 0\"/></pars>" },
    { "gradus_7",
        "<pars><usus fons=\"gradus_8\" locus=\"1 0\"/></pars>" },
    { "gradus_8",
      "<pars><rectangulum fines=\"0 0 20 10\" colores=\"6\"/></pars>" },
    { "gradus_0",
        "<pars><usus fons=\"gradus_1\" locus=\"1 0\"/></pars>" },
    { NIHIL, NIHIL }
};

interior GraphisFructus
_lege (
        CredoContextus* c,
    constans character* literae)
{
    Armarium a;

    a.partes      = PARTES;
    a.vocationes  = ZEPHYRUM;
    redde graphis_legere_cum_fonte(
        chorda_ex_literis(literae, c->piscina), _fons, &a,
        c->piscina);
}

interior GraphisFructus
_pictura (
        CredoContextus* c,
    constans character* literae)
{
    GraphisFructus f = _lege(c, literae);

    si (!f.successus)
    {
        printf("descriptio recusata (%.*s linea %u): %.*s\n",
            (integer)f.plagula.mensura,
            (constans character*)f.plagula.datum,
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.tabula != NIHIL);
    CREDO_AEQUALIS_I32 (f.plagula.mensura, ZEPHYRUM);
    redde f;
}

/* duae descriptiones picturam eandem dant */
#define EADEM(prima, secunda) \
    fac { \
        GraphisFructus ea = _pictura(c, (prima)); \
        GraphisFructus eb = _pictura(c, (secunda)); \
        CREDO_VERUM (memcmp(ea.indices, eb.indices, LAT * ALT) \
            == ZEPHYRUM); \
        CREDO_VERUM (memcmp(ea.tabula->fons, eb.tabula->fons, \
            LAT * ALT * IV) == ZEPHYRUM); \
        CREDO_VERUM (memcmp(ea.tabula->larva, eb.tabula->larva, \
            LAT * ALT) == ZEPHYRUM); \
        CREDO_VERUM (memcmp(ea.tabula->domini, eb.tabula->domini, \
            LAT * ALT) == ZEPHYRUM); \
    } dum (FALSUM)

/* recusatur in plagula et linea data, nuntio fragmentum continente */
#define RECUSATUR(literae, plagula_sperata, linea_sperata, fragmentum) \
    fac { \
        GraphisFructus rf = _lege(c, (literae)); \
        CREDO_FALSUM (rf.successus); \
        CREDO_VERUM (rf.tabula == NIHIL); \
        si (   rf.linea != (linea_sperata) \
            || !chorda_aequalis_literis(rf.plagula, \
                (plagula_sperata)) \
            || !chorda_continet(rf.error, \
                chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("plagula '%.*s' linea %u, nuntius: %.*s\n", \
                (integer)rf.plagula.mensura, \
                (constans character*)rf.plagula.datum, \
                (insignatus)rf.linea, (integer)rf.error.mensura, \
                (constans character*)rf.error.datum); \
        } \
        CREDO_AEQUALIS_I32 (rf.linea, (linea_sperata)); \
        CREDO_VERUM (chorda_aequalis_literis(rf.plagula, \
            (plagula_sperata))); \
        CREDO_VERUM (chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

#define QUADRUM_AD(x, y) \
    "<grex locus=\"" x " " y "\">\n" \
    "<rectangulum fines=\"0 0 20 10\" colores=\"6\"" \
    " stilus=\"1 0\"/>\n" \
    "<ovum fines=\"4 2 8 6\" colores=\"11 0\"" \
    " exemplar=\"tessellatum\" vis=\"1\"/>\n" \
    "</grex>\n"

interior vacuum
_sectio_pars (
    CredoContextus* c)
{
    GraphisFructus f;

    /* pars bis posita = formae eius bis scriptae */
    EADEM ("<pictura>\n"
        "<usus fons=\"quadrum\" locus=\"10 10\"/>\n"
        "<usus fons=\"quadrum\" locus=\"100 50\"/>\n"
        "</pictura>",
        "<pictura>\n" QUADRUM_AD ("10", "10")
        QUADRUM_AD ("100", "50") "</pictura>");
    /* locus ordinarius 0 0; in grege locus gregis additur */
    EADEM ("<pictura>\n"
        "<usus fons=\"quadrum\"/>\n"
        "<grex locus=\"200 100\">\n"
        "<usus fons=\"quadrum\" locus=\"-5 7\"/>\n"
        "</grex>\n"
        "</pictura>",
        "<pictura>\n" QUADRUM_AD ("0", "0")
        QUADRUM_AD ("195", "107") "</pictura>");
    /* pars in parte, cum grege intra partem */
    EADEM ("<pictura>\n"
        "<usus fons=\"par\" locus=\"40 40\"/>\n"
        "</pictura>",
        "<pictura>\n" QUADRUM_AD ("40", "40")
        QUADRUM_AD ("70", "60") "</pictura>");
    /* forma posterior picturae partem tegit; regio circa partem */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum titulus=\"locus\" fines=\"0 0 60 40\""
        " colores=\"3\"/>\n"
        "<usus fons=\"quadrum\" locus=\"10 10\"/>\n"
        "<rectangulum fines=\"20 0 100 100\" colores=\"12\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.indices[12 * LAT + 15], VI);
    CREDO_AEQUALIS_I32 (f.indices[12 * LAT + 25], XII);
    CREDO_AEQUALIS_I32 (f.tabula->titulorum, I);
    CREDO_AEQUALIS_I32 (f.tabula->domini[5 * LAT + 5], I);
    /* pars sine titulo: pixela eius nullius sunt */
    CREDO_AEQUALIS_I32 (f.tabula->domini[12 * LAT + 15], ZEPHYRUM);
    /* lucens in parte valet */
    f = _pictura(c,
        "<pictura lux=\"0.5\">\n"
        "<usus fons=\"lucerna\" locus=\"50 50\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.illustratus[(55 * LAT + 55) * IV], 200);
    /* altitudo VIII ipsa accipitur */
    f = _pictura(c,
        "<pictura>\n<usus fons=\"gradus_1\"/>\n</pictura>");
    CREDO_AEQUALIS_I32 (f.indices[5 * LAT + 12], VI);
}

interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    GraphisFructus f;

    /* pars in margine: forma eius tota extra non recusatur */
    f = _pictura(c,
        "<pictura>\n"
        "<usus fons=\"par\" locus=\"335 0\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.indices[5 * LAT + 350], VI);
    /* ... sed formae eaedem in pictura ipsa recusantur */
    CREDO_FALSUM (_lege(c, "<pictura>\n" QUADRUM_AD ("335", "0")
        QUADRUM_AD ("365", "20") "</pictura>").successus);
    /* pars tota extra picturam posita: nihil pingit, non recusatur */
    f = _pictura(c,
        "<pictura>\n"
        "<rectangulum fines=\"0 0 9 9\" colores=\"6\"/>\n"
        "<usus fons=\"quadrum\" locus=\"-500 0\"/>\n"
        "</pictura>");
    CREDO_AEQUALIS_I32 (f.indices[ZEPHYRUM], VI);
}

#define CAPUT "<pictura>\n"
#define CAUDA "\n</pictura>"

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    GraphisFructus f;

    /* sine fonte dato */
    f = graphis_legere(chorda_ex_literis(
        CAPUT "<usus fons=\"quadrum\"/>" CAUDA, c->piscina),
        c->piscina);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, II);
    CREDO_AEQUALIS_I32 (f.plagula.mensura, ZEPHYRUM);
    CREDO_VERUM (chorda_continet(f.error,
        chorda_ex_literis("source of pieces", c->piscina)));
    f = graphis_legere_cum_fonte(chorda_ex_literis(
        CAPUT "<usus fons=\"quadrum\"/>" CAUDA, c->piscina), NIHIL,
        NIHIL, c->piscina);
    CREDO_FALSUM (f.successus);
    /* usus ipse */
    RECUSATUR (CAPUT "<usus locus=\"0 0\"/>" CAUDA, "", II, "fons");
    RECUSATUR (CAPUT "<usus fons=\"Sella\"/>" CAUDA, "", II, "fons");
    RECUSATUR (CAPUT "<usus fons=\"a b\"/>" CAUDA, "", II, "fons");
    RECUSATUR (CAPUT "<usus fons=\"../sella\"/>" CAUDA, "", II,
        "fons");
    RECUSATUR (CAPUT "<usus fons=\"quadrum\" locus=\"5\"/>" CAUDA, "",
        II, "locus");
    RECUSATUR (CAPUT "<usus fons=\"quadrum\" vis=\"1\"/>" CAUDA, "",
        II, "unknown attribute 'vis'");
    RECUSATUR (CAPUT "<usus fons=\"quadrum\">"
        "<rectangulum fines=\"0 0 9 9\" colores=\"6\"/></usus>" CAUDA,
        "", II, "children");
    RECUSATUR (CAPUT "\n<usus fons=\"nulla\"/>" CAUDA, "", III,
        "'nulla' was not found");
    /* plagula partis */
    RECUSATUR (CAPUT "<usus fons=\"non_stml\"/>" CAUDA, "non_stml",
        II, "not valid STML");
    RECUSATUR (CAPUT "<usus fons=\"radix_mala\"/>" CAUDA,
        "radix_mala", I, "<pars>");
    RECUSATUR (CAPUT "<usus fons=\"radix_attributum\"/>" CAUDA,
        "radix_attributum", I, "unknown attribute 'lux'");
    RECUSATUR (CAPUT "<usus fons=\"cum_lumine\"/>" CAUDA,
        "cum_lumine", II, "piece");
    RECUSATUR (CAPUT "<usus fons=\"cum_titulo\"/>" CAUDA,
        "cum_titulo", III, "titulus");
    RECUSATUR (CAPUT "<usus fons=\"forma_mala\"/>" CAUDA,
        "forma_mala", III, "fines");
    /* orbes */
    RECUSATUR (CAPUT "<usus fons=\"se_ipsam\"/>" CAUDA, "se_ipsam",
        III, "'se_ipsam' uses itself");
    RECUSATUR (CAPUT "<usus fons=\"orbis_a\"/>" CAUDA, "orbis_b", IV,
        "'orbis_a' uses itself");
    /* altitudo IX */
    RECUSATUR (CAPUT "<usus fons=\"gradus_0\"/>" CAUDA, "gradus_7", I,
        "8 deep");
    /* recusatio in pictura ipsa post partem bonam: plagula vacua */
    RECUSATUR (CAPUT "<usus fons=\"quadrum\"/>\n"
        "<circulus/>" CAUDA, "", III, "unknown element");
}

/* fons qui partes ex graphis/res legit, ut instrumenta facient */
interior chorda
_fons_plagularum (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    character via[CXXVIII];

    (vacuum)contextus;
    (vacuum)sprintf(via, "graphis/res/%.*s.stml",
        (integer)titulus.mensura,
        (constans character*)titulus.datum);
    si (!filum_existit(via))
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

interior vacuum
_sectio_sella (
    CredoContextus* c)
{
    GraphisFructus f = graphis_legere_cum_fonte(chorda_ex_literis(
        "<pictura>\n"
        "<usus fons=\"sella\" locus=\"52 168\"/>\n"
        "<usus fons=\"sella\" locus=\"300 100\"/>\n"
        "</pictura>", c->piscina), _fons_plagularum, NIHIL,
        c->piscina);
               i32 k;
               i32 n = ZEPHYRUM;

    si (!f.successus)
    {
        printf("sella recusata (%.*s linea %u): %.*s\n",
            (integer)f.plagula.mensura,
            (constans character*)f.plagula.datum,
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    /* sedile textum: color IX aut X sub cruribus */
    per (k = ZEPHYRUM; k < LAT * ALT; k++)
    {
        si (f.indices[k] != GRAPHIS_COLOR_FUNDI)
        {
            n++;
        }
    }
    CREDO_VERUM (n > M);
    /* duae sellae eaedem: pixela translata paria */
    CREDO_AEQUALIS_I32 (f.indices[(168 - 22) * LAT + 52 + 10],
        f.indices[(100 - 22) * LAT + 300 + 10]);
    CREDO_AEQUALIS_I32 (f.indices[(168 - 22) * LAT + 52 + 10], IX);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "pars", _sectio_pars, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "sella", _sectio_sella, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("graphis_usus", SECTIONES);
}
