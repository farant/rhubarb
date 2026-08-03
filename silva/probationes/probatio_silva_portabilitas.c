/* probatio_silva_portabilitas.c - codices 85-87 (postulata/
 * vernaculum/obsoletum; INTENTIO 2026-08-03, desiderata 01KYTGNA36
 * + 01KZ3RDX8B, decretum 01KZ3RYZWK)
 *
 * Systema fictum limitem + gradus fert; usor contra iudicatur.
 * Calibratio bidirectionalis: quisque codex flagrans VISUS et
 * quietus VISUS (prologus / bracchium omissum / ante limitem /
 * sine limite / sine systemate). Tagus gradus in litteris divisis
 * scribitur - lex scansoris (sequentia aperiens tagum evocaret). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* systema fictum: symbolum ANTE limitem (ISO fictum), limes,
 * post limitem: functio, gradus vernaculum super #define (semita
 * directivarum!), gradus obsoletum super declarationem (semita
 * triviorum fluxus). */
hic_manens constans character* FONS_SYS_CUM_LIMITE =
    "int ante_limitem_p(int f);\n"
    "/* " SILVA_LIMES_POSIX_TITULUS " - probatio */\n"
    "int furca_probatio(void);\n"
    "/* <gradu" "s titulus=\"SO_PROBATIO\" genus=\"vernaculum\""
    " pro=\"MSG_PROBATIO\"/> */\n"
    "#define SO_PROBATIO 7\n"
    "/* <gradu" "s titulus=\"vetus_probatio\" genus=\"obsoletum\""
    " pro=\"novum_probatio\"/> */\n"
    "int vetus_probatio(void);\n";

/* idem sine limite: gradus adsunt sed nihil POSIX notatum est -
 * custos fractus consulto (calibratio: limes portam OMNIUM tenet) */
hic_manens constans character* FONS_SYS_SINE_LIMITE =
    "int ante_limitem_p(int f);\n"
    "int furca_probatio(void);\n"
    "/* <gradu" "s titulus=\"SO_PROBATIO\" genus=\"vernaculum\""
    " pro=\"MSG_PROBATIO\"/> */\n"
    "#define SO_PROBATIO 7\n"
    "/* <gradu" "s titulus=\"vetus_probatio\" genus=\"obsoletum\""
    " pro=\"novum_probatio\"/> */\n"
    "int vetus_probatio(void);\n";

hic_manens constans character* FONS_USOR_OMNIA =
    "int probare(void);\n"
    "int probare(void)\n"
    "{\n"
    "    int p = furca_probatio();\n"
    "    p = p + SO_PROBATIO;\n"
    "    p = p + vetus_probatio();\n"
    "    return p;\n"
    "}\n";

hic_manens constans character* FONS_USOR_CUM_PROLOGO =
    "#include \"postulata_posix.h\"\n"
    "int probare(void);\n"
    "int probare(void)\n"
    "{\n"
    "    int p = furca_probatio();\n"
    "    p = p + SO_PROBATIO;\n"
    "    p = p + vetus_probatio();\n"
    "    return p;\n"
    "}\n";

hic_manens constans character* FONS_USOR_BRACCHIUM =
    "int probare(int fd);\n"
    "int probare(int fd)\n"
    "{\n"
    "    int flag = 1;\n"
    "#ifdef __APPLE__\n"
    "    flag = flag + SO_PROBATIO;\n"
    "#endif\n"
    "    return flag + fd;\n"
    "}\n";

hic_manens constans character* FONS_USOR_ANTE =
    "int probare(void);\n"
    "int probare(void)\n"
    "{\n"
    "    return ante_limitem_p(3);\n"
    "}\n";

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* titulus,
    constans character* fons)
{
    redde silva_c89_parsare(piscina, titulus, fons,
        (i32)strlen(fons), NIHIL);
}

interior i32
_numerus_codicis (constans SilvaSemantica* sem, s32 codex)
{
    i32 n = ZEPHYRUM;
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            n++;
        }
    }
    redde n;
}

interior b32
_causa_continet (constans SilvaSemantica* sem, s32 codex,
    constans character* fragmentum)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex && d->causa != NIHIL
            && strstr(d->causa, fragmentum) != NIHIL)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior SilvaSemantica*
_iudicare (Piscina* piscina, constans character* fons_sys,
    constans character* fons_usoris)
{
    SilvaParsura* systema;
    SilvaParsura* usor;

    systema = _parsare(piscina, "systema_c89.h", fons_sys);
    usor = _parsare(piscina, "usor_probationis.c", fons_usoris);
    CREDO_NON_NIHIL (systema);
    CREDO_NON_NIHIL (usor);
    si (systema == NIHIL || usor == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_c89_semantica_analysare_cum_systemate(piscina,
        usor, systema);
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_portabilitas",
        67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. Omnia flagrant: usor sine prologo omnia tria evocat;
     * causae radices et vicarios nominant
     * ======================================================== */
    imprimere("--- I: omnia flagrant ---\n");
    {
        SilvaSemantica* sem = _iudicare(piscina, FONS_SYS_CUM_LIMITE,
            FONS_USOR_OMNIA);

        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT), I);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), I);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM), I);
            CREDO_VERUM (_causa_continet(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT,
                "furca_probatio"));
            CREDO_VERUM (_causa_continet(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT,
                "postulata_posix.h"));
            CREDO_VERUM (_causa_continet(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM,
                "MSG_PROBATIO"));
            CREDO_VERUM (_causa_continet(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM,
                "novum_probatio"));
        }
    }

    /* ========================================================
     * II. Prologus sanat 85 SOLUM: vernaculum/obsoletum manent
     * (independentia codicum)
     * ======================================================== */
    imprimere("--- II: prologus sanat 85 ---\n");
    {
        SilvaSemantica* sem = _iudicare(piscina, FONS_SYS_CUM_LIMITE,
            FONS_USOR_CUM_PROLOGO);

        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), I);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM), I);
        }
    }

    /* ========================================================
     * III. Bracchium omissum quietat: usus in bracchio ifdef
     * non sumpto in crudis latet - conventio gratis
     * ======================================================== */
    imprimere("--- III: bracchium omissum quietat ---\n");
    {
        SilvaSemantica* sem = _iudicare(piscina, FONS_SYS_CUM_LIMITE,
            FONS_USOR_BRACCHIUM);

        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), ZEPHYRUM);
        }
    }

    /* ========================================================
     * IV. Ante limitem non censetur: symbolum ISO (ante limitem)
     * postulata non evocat - praedicatum USU basatum, non
     * inclusione (stdlib.h sectio POSIX est - 182 plagulae)
     * ======================================================== */
    imprimere("--- IV: ante limitem non censetur ---\n");
    {
        SilvaSemantica* sem = _iudicare(piscina, FONS_SYS_CUM_LIMITE,
            FONS_USOR_ANTE);

        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM), ZEPHYRUM);
        }
    }

    /* ========================================================
     * V. Sine limite: 85 SOLUM silet - limes partem POSIX pro
     * POSTULATIS discernit; gradus autem notae EXPLICITAE per
     * symbolum sunt, sensus earum a limite NON pendet (symbolum
     * vernaculum vernaculum est ubicumque declaratur). Calibratio
     * hoc invenit: assertio prima 'limes portam OMNIUM tenet'
     * FALSA erat - independentia mechanismorum melior lex.
     * ======================================================== */
    imprimere("--- V: sine limite 85 silet, gradus manent ---\n");
    {
        SilvaSemantica* sem = _iudicare(piscina,
            FONS_SYS_SINE_LIMITE, FONS_USOR_OMNIA);

        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), I);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM), I);
        }
    }

    /* ========================================================
     * VI. Sine systemate silent: analysare simplex (systema
     * NIHIL) portabilitatem numquam iudicat
     * ======================================================== */
    imprimere("--- VI: sine systemate silent ---\n");
    {
        SilvaParsura* usor = _parsare(piscina, "usor_probationis.c",
            FONS_USOR_OMNIA);
        SilvaSemantica* sem;

        CREDO_NON_NIHIL (usor);
        sem = (usor != NIHIL)
            ? silva_c89_semantica_analysare(piscina, usor) : NIHIL;
        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_POSTULATA_DESUNT), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_VERNACULUM_ADHIBITUM), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM), ZEPHYRUM);
        }
    }

    /* ========================================================
     * VII-IX. Professiones (codex 88, regula lenis v1): plagula
     * portabilis vernaculum includens flagrat; sutura et sine
     * professione transeunt; principalis sine professione tacet.
     * Tagi in litteris divisis - lex scansoris.
     * ======================================================== */
    imprimere("--- VII: portabile vernaculum includit ---\n");
    {
        SilvaContextus* ctx = silva_contextus_creare(piscina);
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        constans character* caput_vern =
            "/* fenestra ficta <vernacu" "lum/> */\n"
            "int fenestra_p(void);\n";
        constans character* usor =
            "/* <porta" "bile/> */\n"
            "#include \"caput_p.h\"\n"
            "int probare(void);\n"
            "int probare(void) { return fenestra_p(); }\n";

        CREDO_NON_NIHIL (ctx);
        si (ctx != NIHIL)
        {
            CREDO_VERUM (silva_contextus_praebere(ctx, "caput_p.h",
                caput_vern, (i32)strlen(caput_vern)));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "usor_p.c", usor, (i32)strlen(usor), NIHIL);
            CREDO_NON_NIHIL (parsura);
            sem = (parsura != NIHIL)
                ? silva_c89_semantica_analysare(piscina, parsura)
                : NIHIL;
            CREDO_NON_NIHIL (sem);
            si (sem != NIHIL)
            {
                CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                    (s32)EXAMEN_CODEX_PORTABILE_VIOLATUM), I);
                CREDO_VERUM (_causa_continet(sem,
                    (s32)EXAMEN_CODEX_PORTABILE_VIOLATUM,
                    "caput_p.h"));
            }
        }
    }

    imprimere("--- VIII: sutura et sine professione transeunt ---\n");
    {
        SilvaContextus* ctx = silva_contextus_creare(piscina);
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        constans character* caput_sut =
            "/* <sutu" "ra/> */\n"
            "int sutura_p(void);\n";
        constans character* caput_liber =
            "int liber_p(void);\n";
        constans character* usor =
            "/* <porta" "bile/> */\n"
            "#include \"caput_s.h\"\n"
            "#include \"caput_l.h\"\n"
            "int probare(void);\n"
            "int probare(void) { return sutura_p() + liber_p(); }\n";

        CREDO_NON_NIHIL (ctx);
        si (ctx != NIHIL)
        {
            CREDO_VERUM (silva_contextus_praebere(ctx, "caput_s.h",
                caput_sut, (i32)strlen(caput_sut)));
            CREDO_VERUM (silva_contextus_praebere(ctx, "caput_l.h",
                caput_liber, (i32)strlen(caput_liber)));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "usor_s.c", usor, (i32)strlen(usor), NIHIL);
            sem = (parsura != NIHIL)
                ? silva_c89_semantica_analysare(piscina, parsura)
                : NIHIL;
            CREDO_NON_NIHIL (sem);
            si (sem != NIHIL)
            {
                CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                    (s32)EXAMEN_CODEX_PORTABILE_VIOLATUM),
                    ZEPHYRUM);
            }
        }
    }

    imprimere("--- IX: principalis sine professione tacet ---\n");
    {
        SilvaContextus* ctx = silva_contextus_creare(piscina);
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        constans character* caput_vern =
            "/* <vernacu" "lum/> */\n"
            "int fenestra_p(void);\n";
        constans character* usor =
            "#include \"caput_p.h\"\n"
            "int probare(void);\n"
            "int probare(void) { return fenestra_p(); }\n";

        CREDO_NON_NIHIL (ctx);
        si (ctx != NIHIL)
        {
            CREDO_VERUM (silva_contextus_praebere(ctx, "caput_p.h",
                caput_vern, (i32)strlen(caput_vern)));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "usor_n.c", usor, (i32)strlen(usor), NIHIL);
            sem = (parsura != NIHIL)
                ? silva_c89_semantica_analysare(piscina, parsura)
                : NIHIL;
            CREDO_NON_NIHIL (sem);
            si (sem != NIHIL)
            {
                CREDO_AEQUALIS_I32 (_numerus_codicis(sem,
                    (s32)EXAMEN_CODEX_PORTABILE_VIOLATUM),
                    ZEPHYRUM);
            }
        }
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
