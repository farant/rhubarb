/* probatio_silva_examen.c - M4a chunk A: diagnosticum v2
 * (codex/severitas/positio/provisionale/socius; tabula codicum;
 * consilium project-specs/officina-m4a-spec.md chunk A) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_c89_parsare(piscina, "probatio_examen.c", fons,
        (i32)strlen(fons), NIHIL);
}

interior constans SemanticaDiagnosticum*
_diagnosticum_codicis (constans SilvaSemantica* sem, s32 codex)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            redde d;
        }
    }
    redde NIHIL;
}

interior TypusC89*
_typus_symboli (SilvaSemantica* sem, constans character* titulus)
{
    chorda c;
    unio { constans character* l; i8* m; } u;
    SemanticaSymbolum* s;

    u.l = titulus;
    c.datum = u.m;
    c.mensura = (i32)strlen(titulus);
    s = silva_c89_symbolum_invenire(sem, c);
    redde (s != NIHIL) ? s->typus : NIHIL;
}

interior constans SilvaNodus*
_initiator_primus (constans SilvaParsura* parsura)
{
    SilvaValor* e = silva_valor_lista_obtinere(
        parsura->commissio->radix, ZEPHYRUM);
    SilvaValor ds;
    SilvaValor* d0;
    SilvaValor iv;

    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    ds = silva_c89_declaratio_declaratores(e->datum.nodus);
    d0 = silva_valor_lista_obtinere(ds, ZEPHYRUM);
    si (d0 == NIHIL || d0->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    iv = silva_c89_declarator_initiatus_initiator(d0->datum.nodus);
    si (iv.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde iv.datum.nodus;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_examen",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. Integritas tabulae: omnis codex causam + severitatem
     * validam habet (magnitudinem assertum staticum in .c
     * custodit; hic valores per additionem NIHIL-nodi)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int probe;\n");
        SilvaSemantica* sem;
        s32 c;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 ((i32)silva_c89_diagnostica_numerus(sem),
            ZEPHYRUM);

        per (c = ZEPHYRUM; c < (s32)EXAMEN_CODEX_NUMERUS; c++)
        {
            constans SemanticaDiagnosticum* d;

            silva_c89_diagnosticum_addere(sem, NIHIL, c);
            d = silva_c89_diagnosticum_per_indicem(sem, (i32)c);
            CREDO_NON_NIHIL (d);
            si (d != NIHIL)
            {
                CREDO_VERUM (d->causa != NIHIL);
                CREDO_VERUM (d->severitas >= (s32)EXAMEN_VIOLATIO
                    && d->severitas <= (s32)EXAMEN_INFRA);
                CREDO_VERUM (d->codex == c);
                /* NIHIL nodus: positio absens, non provisionale */
                CREDO_AEQUALIS_I32 (d->linea, ZEPHYRUM);
                CREDO_VERUM (!d->provisionale);
                CREDO_VERUM (d->socius == NIHIL);
            }
        }
    }

    /* ========================================================
     * II. Positio materializata: identificator ignotus linea 2,
     * columna 16; via = titulus fontis
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int a = 1;\n"
            "static int q = mysterium;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 ((i32)silva_c89_diagnostica_numerus(sem),
            I);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->severitas == (s32)EXAMEN_VIOLATIO);
            CREDO_AEQUALIS_I32 (d->linea, II);
            CREDO_AEQUALIS_I32 (d->columna, XVI);
            CREDO_AEQUALIS_I32 (d->via.mensura,
                (i32)strlen("probatio_examen.c"));
            CREDO_VERUM (d->via.datum != NIHIL
                && memcmp(d->via.datum, "probatio_examen.c",
                       (memoriae_index)d->via.mensura) == ZEPHYRUM);
            CREDO_VERUM (!d->provisionale);
            CREDO_VERUM (d->socius == NIHIL);
        }
    }

    /* ========================================================
     * III. Macro: positio = RADIX (sedes usus, non definitio) -
     * praecedens indicii (silva_token_radix in additione)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "#define ARCANUM mysterium\n"
            "static int q = ARCANUM;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 (d->linea, II);
            CREDO_AEQUALIS_I32 (d->columna, XVI);
        }
    }

    /* ========================================================
     * IV. Provisionale: furca decl/expr irresoluta
     * ('Ignotum * x;' intra corpus) - canonicum = multiplicatio;
     * identificator ignotus flagrat sed PROVISIONALE
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static void f(void) { Ignotum * x; }\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->provisionale);
        }
    }

    /* ========================================================
     * V. Socius: addere_cum_socio sedem cognatam fert
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int a;\n");
        SilvaSemantica* sem;
        constans SilvaNodus* nodus_a = NIHIL;
        constans SemanticaDiagnosticum* d;
        SilvaValor* e;
        i32 m;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        e = silva_valor_lista_obtinere(parsura->commissio->radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
        {
            nodus_a = e->datum.nodus;
        }
        CREDO_NON_NIHIL (nodus_a);
        silva_c89_diagnosticum_addere_cum_socio(sem, nodus_a,
            (s32)EXAMEN_CODEX_REDECLARATIO_GENERIS, nodus_a);
        m = (i32)silva_c89_diagnostica_numerus(sem);
        CREDO_VERUM (m > ZEPHYRUM);
        d = silva_c89_diagnosticum_per_indicem(sem, m - I);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->socius == nodus_a);
            CREDO_VERUM (d->linea > ZEPHYRUM);
        }
    }

    /* ========================================================
     * VI. Sedes NIHIL sanatae: specificatores mixti positionem
     * ferunt (nodus primus listae)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "typedef int T;\n"
            "static T int x;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_SPECIFICATORES_MIXTI);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 (d->linea, II);
        }
    }

    /* ========================================================
     * VII. Relatio compatibilitatis (chunk B): paria tabulata
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "enum A { XA }; enum B { XB };\n"
            "struct S { int a; }; struct T { int a; };\n"
            "static enum A ea; static enum B eb;\n"
            "static struct S s1; static struct T t1;\n");
        SilvaSemantica* sem;
        TypusC89* t_int;
        TypusC89* t_longus;
        TypusC89* t_char;
        TypusC89* t_char_sig;
        TypusC89* t_duplex;
        TypusC89* t_fluitans;
        TypusC89* t_vacuum;
        TypusC89* enum_a;
        TypusC89* enum_b;
        TypusC89* struct_s;
        TypusC89* struct_t;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        t_int = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_INTEGER);
        t_longus = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_LONGUS);
        t_char = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_CHARACTER);
        t_char_sig = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_CHARACTER_SIGNATUM);
        t_duplex = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_DUPLEX);
        t_fluitans = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_FLUITANS);
        t_vacuum = silva_c89_typus_primitivum(sem,
            (s32)PRIMITIVUM_VACUUM);
        enum_a = _typus_symboli(sem, "ea");
        enum_b = _typus_symboli(sem, "eb");
        struct_s = _typus_symboli(sem, "s1");
        struct_t = _typus_symboli(sem, "t1");
        CREDO_NON_NIHIL (enum_a);
        CREDO_NON_NIHIL (struct_s);

        /* identitas + primitivi distincti */
        CREDO_VERUM (silva_c89_typi_compatibiles(t_int, t_int));
        CREDO_VERUM (!silva_c89_typi_compatibiles(t_int, t_longus));
        CREDO_VERUM (!silva_c89_typi_compatibiles(t_char,
            t_char_sig));   /* char != signed char in C89 */

        /* venenum absorbet */
        CREDO_VERUM (silva_c89_typi_compatibiles(sem->typus_erroris,
            t_int));
        CREDO_VERUM (silva_c89_typi_compatibiles(struct_s,
            sem->typus_erroris));

        /* quales ut copiae */
        {
            TypusC89* c_int = silva_c89_typus_qualificatus(sem,
                t_int, QUALIS_CONSTANS);

            CREDO_VERUM (silva_c89_typi_compatibiles(c_int, c_int));
            CREDO_VERUM (!silva_c89_typi_compatibiles(c_int, t_int));
        }

        /* monstratores: pointee qualibus inclusis */
        {
            TypusC89* p_int = silva_c89_typus_monstrator(sem, t_int);
            TypusC89* p_longus = silva_c89_typus_monstrator(sem,
                t_longus);
            TypusC89* p_char = silva_c89_typus_monstrator(sem,
                t_char);
            TypusC89* p_c_char = silva_c89_typus_monstrator(sem,
                silva_c89_typus_qualificatus(sem, t_char,
                    QUALIS_CONSTANS));
            TypusC89* pp_int = silva_c89_typus_monstrator(sem,
                p_int);

            CREDO_VERUM (silva_c89_typi_compatibiles(p_int, p_int));
            CREDO_VERUM (!silva_c89_typi_compatibiles(p_int,
                p_longus));
            /* DECIPULA INTERNAMENTI: numquam == pro his */
            CREDO_VERUM (!silva_c89_typi_compatibiles(p_char,
                p_c_char));
            CREDO_VERUM (silva_c89_typi_compatibiles(pp_int,
                pp_int));
        }

        /* acies: elementum + sentinella mensurae */
        {
            TypusC89* a10 = silva_c89_typus_acies(sem, t_int, 10);
            TypusC89* a10b = silva_c89_typus_acies(sem, t_int, 10);
            TypusC89* a5 = silva_c89_typus_acies(sem, t_int, 5);
            TypusC89* a_ign = silva_c89_typus_acies(sem, t_int, -1);
            TypusC89* a10_l = silva_c89_typus_acies(sem, t_longus,
                10);

            CREDO_VERUM (silva_c89_typi_compatibiles(a10, a10b));
            CREDO_VERUM (!silva_c89_typi_compatibiles(a10, a5));
            CREDO_VERUM (silva_c89_typi_compatibiles(a10, a_ign));
            CREDO_VERUM (silva_c89_typi_compatibiles(a_ign, a_ign));
            CREDO_VERUM (!silva_c89_typi_compatibiles(a10, a10_l));
        }

        /* functiones: prototypatae / K&R / mixtae */
        {
            TypusC89* par_int[1];
            TypusC89* par_longus[1];
            TypusC89* par_char[1];
            TypusC89* par_duplex[1];
            TypusC89* par_fluitans[1];
            TypusC89* f_int;
            TypusC89* f_longus;
            TypusC89* f_char;
            TypusC89* f_duplex;
            TypusC89* f_fluitans;
            TypusC89* f_kr;
            TypusC89* f_var;

            par_int[0] = t_int;
            par_longus[0] = t_longus;
            par_char[0] = t_char;
            par_duplex[0] = t_duplex;
            par_fluitans[0] = t_fluitans;
            f_int = silva_c89_typus_functio(sem, t_int, par_int, 1,
                FALSUM, VERUM);
            f_longus = silva_c89_typus_functio(sem, t_int,
                par_longus, 1, FALSUM, VERUM);
            f_char = silva_c89_typus_functio(sem, t_int, par_char,
                1, FALSUM, VERUM);
            f_duplex = silva_c89_typus_functio(sem, t_int,
                par_duplex, 1, FALSUM, VERUM);
            f_fluitans = silva_c89_typus_functio(sem, t_int,
                par_fluitans, 1, FALSUM, VERUM);
            f_kr = silva_c89_typus_functio(sem, t_int, NIHIL, 0,
                FALSUM, FALSUM);
            f_var = silva_c89_typus_functio(sem, t_int, par_int, 1,
                VERUM, VERUM);

            CREDO_VERUM (silva_c89_typi_compatibiles(f_int, f_int));
            CREDO_VERUM (!silva_c89_typi_compatibiles(f_int,
                f_longus));
            CREDO_VERUM (silva_c89_typi_compatibiles(f_kr, f_kr));
            /* mixtae: int/duplex immota promotione -> compatibiles;
             * char/fluitans mutantur -> incompatibiles */
            CREDO_VERUM (silva_c89_typi_compatibiles(f_kr, f_int));
            CREDO_VERUM (silva_c89_typi_compatibiles(f_kr,
                f_duplex));
            CREDO_VERUM (!silva_c89_typi_compatibiles(f_kr,
                f_char));
            CREDO_VERUM (!silva_c89_typi_compatibiles(f_kr,
                f_fluitans));
            CREDO_VERUM (!silva_c89_typi_compatibiles(f_kr, f_var));
            CREDO_VERUM (!silva_c89_typi_compatibiles(f_int,
                f_var));
        }

        /* tags nominales + enum <-> int */
        si (enum_a != NIHIL && enum_b != NIHIL && struct_s != NIHIL
            && struct_t != NIHIL)
        {
            CREDO_VERUM (silva_c89_typi_compatibiles(enum_a,
                enum_a));
            CREDO_VERUM (!silva_c89_typi_compatibiles(enum_a,
                enum_b));
            CREDO_VERUM (silva_c89_typi_compatibiles(enum_a,
                t_int));
            CREDO_VERUM (silva_c89_typi_compatibiles(t_int,
                enum_a));
            CREDO_VERUM (!silva_c89_typi_compatibiles(enum_a,
                t_longus));
            CREDO_VERUM (silva_c89_typi_compatibiles(struct_s,
                struct_s));
            CREDO_VERUM (!silva_c89_typi_compatibiles(struct_s,
                struct_t));  /* forma eadem, nomina diversa */
        }

        /* vacuum non est arithmeticum nec compatibile cum int */
        CREDO_VERUM (!silva_c89_typi_compatibiles(t_vacuum, t_int));

        /* ====================================================
         * VIII. Iudicium assignationis (directionale)
         * ==================================================== */
        {
            TypusC89* p_int = silva_c89_typus_monstrator(sem, t_int);
            TypusC89* p_longus = silva_c89_typus_monstrator(sem,
                t_longus);
            TypusC89* p_char = silva_c89_typus_monstrator(sem,
                t_char);
            TypusC89* p_c_char = silva_c89_typus_monstrator(sem,
                silva_c89_typus_qualificatus(sem, t_char,
                    QUALIS_CONSTANS));
            TypusC89* p_vacuum = silva_c89_typus_monstrator(sem,
                t_vacuum);
            TypusC89* a10 = silva_c89_typus_acies(sem, t_int, 10);
            s32 codex = -1;

            /* arith <- arith semper licita */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, t_int, t_longus, &codex),
                (i32)EXAMEN_LICET_CONVERSIO);

            /* quales summi finis exuti (locus possidet, non hic) */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, silva_c89_typus_qualificatus(sem, t_int,
                    QUALIS_CONSTANS), t_int, &codex),
                (i32)EXAMEN_LICET_CONVERSIO);

            /* monstrator identicus -> LICET */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_int, p_int, &codex),
                (i32)EXAMEN_LICET);

            /* acies -> monstrator (lapsus) -> LICET */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_int, a10, &codex),
                (i32)EXAMEN_LICET);

            /* pointee incompatibilis */
            codex = -1;
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_int, p_longus, &codex),
                (i32)EXAMEN_VETITUM);
            CREDO_VERUM (codex
                == (s32)EXAMEN_CODEX_MONSTRATORES_INCOMPATIBILES);

            /* monstrator <- integer (sine constante nulla) */
            codex = -1;
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_int, t_int, &codex),
                (i32)EXAMEN_VETITUM);
            CREDO_VERUM (codex
                == (s32)EXAMEN_CODEX_MONSTRATOR_INTEGER);

            /* integer <- monstrator */
            codex = -1;
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, t_int, p_int, &codex),
                (i32)EXAMEN_VETITUM);
            CREDO_VERUM (codex
                == (s32)EXAMEN_CODEX_MONSTRATOR_INTEGER);

            /* dilatatio qualium licita; abiectio vetita */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_c_char, p_char, &codex),
                (i32)EXAMEN_LICET_CONVERSIO);
            codex = -1;
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_char, p_c_char, &codex),
                (i32)EXAMEN_VETITUM);
            CREDO_VERUM (codex == (s32)EXAMEN_CODEX_QUALES_ABIECTI);

            /* void* utroque modo */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_vacuum, p_int, &codex),
                (i32)EXAMEN_LICET_CONVERSIO);
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, p_int, p_vacuum, &codex),
                (i32)EXAMEN_LICET_CONVERSIO);

            /* aggregata: identica LICET, diversa VETITUM */
            si (struct_s != NIHIL && struct_t != NIHIL)
            {
                CREDO_AEQUALIS_I32 (
                    (i32)silva_c89_assignationem_iudicare(sem,
                        NIHIL, struct_s, struct_s, &codex),
                    (i32)EXAMEN_LICET);
                codex = -1;
                CREDO_AEQUALIS_I32 (
                    (i32)silva_c89_assignationem_iudicare(sem,
                        NIHIL, struct_s, struct_t, &codex),
                    (i32)EXAMEN_VETITUM);
                CREDO_VERUM (codex
                    == (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS);
                /* int <- structura */
                codex = -1;
                CREDO_AEQUALIS_I32 (
                    (i32)silva_c89_assignationem_iudicare(sem,
                        NIHIL, t_int, struct_s, &codex),
                    (i32)EXAMEN_VETITUM);
                CREDO_VERUM (codex
                    == (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS);
            }

            /* venenum absorbet - numquam re-iudica */
            CREDO_AEQUALIS_I32 ((i32)silva_c89_assignationem_iudicare(
                sem, NIHIL, sem->typus_erroris, t_int, &codex),
                (i32)EXAMEN_LICET);
        }

        /* constans nulla: 'static int* pz = 0;' - initiator
         * verus (sine eo iudicium nullam constantem non videt) */
        {
            SilvaParsura* pn = _parsare(piscina,
                "static int* pz = 0;\n");
            SilvaSemantica* semn;

            CREDO_NON_NIHIL (pn);
            semn = silva_c89_semantica_analysare(piscina, pn);
            CREDO_NON_NIHIL (semn);
            si (semn != NIHIL)
            {
                constans SilvaNodus* init = _initiator_primus(pn);
                TypusC89* ti = silva_c89_typus_primitivum(semn,
                    (s32)PRIMITIVUM_INTEGER);
                TypusC89* pi = silva_c89_typus_monstrator(semn, ti);

                CREDO_NON_NIHIL (init);
                CREDO_AEQUALIS_I32 (
                    (i32)silva_c89_assignationem_iudicare(semn,
                        init, pi, ti, NIHIL),
                    (i32)EXAMEN_LICET_CONVERSIO);
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
