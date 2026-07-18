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
#include "silva_c89_fluxus.h"
#include "silva_c89_fluxus_datorum.h"
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

/* Chunk C: fons parvus -> codex exspectatus n-ies (aut purum) */
interior vacuum
_codicem_probare (Piscina* piscina, constans character* fons,
    s32 codex, i32 numerus)
{
    SilvaParsura* parsura = _parsare(piscina, fons);
    SilvaSemantica* sem;
    i32 inventi = ZEPHYRUM;
    i32 i;
    i32 m;

    CREDO_NON_NIHIL (parsura);
    si (parsura == NIHIL)
    {
        redde;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    CREDO_NON_NIHIL (sem);
    si (sem == NIHIL)
    {
        redde;
    }
    m = (i32)silva_c89_diagnostica_numerus(sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            inventi++;
        }
    }
    CREDO_AEQUALIS_I32 (inventi, numerus);
}

/* Sectio X: parsura cum contextu (praebenda + lexicon optionale) -
 * TU cum capitibus fictis pro examine alienorum */
interior SilvaSemantica*
_analysare_alienum (Piscina* piscina, constans character* fons,
    constans character* via_a, constans character* textus_a,
    constans character* via_b, constans character* textus_b,
    constans character* via_c, constans character* textus_c,
    constans character* lexicon)
{
    SilvaContextus* ctx = silva_contextus_creare(piscina);
    SilvaParsura* parsura;

    si (ctx == NIHIL)
    {
        redde NIHIL;
    }
    si (lexicon != NIHIL
        && !silva_contextus_lexicon_addere(ctx, "systema_probatio.h",
               lexicon, (i32)strlen(lexicon)))
    {
        redde NIHIL;
    }
    si (via_a != NIHIL
        && !silva_contextus_praebere(ctx, via_a, textus_a,
               (i32)strlen(textus_a)))
    {
        redde NIHIL;
    }
    si (via_b != NIHIL
        && !silva_contextus_praebere(ctx, via_b, textus_b,
               (i32)strlen(textus_b)))
    {
        redde NIHIL;
    }
    si (via_c != NIHIL
        && !silva_contextus_praebere(ctx, via_c, textus_c,
               (i32)strlen(textus_c)))
    {
        redde NIHIL;
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
        "probatio_examen.c", fons, (i32)strlen(fons), NIHIL);
    si (parsura == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_c89_semantica_analysare(piscina, parsura);
}

interior i32
_codicem_numerare (constans SilvaSemantica* sem, s32 codex)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);
    i32 inventi = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            inventi++;
        }
    }
    redde inventi;
}

interior vacuum
_purum_probare (Piscina* piscina, constans character* fons)
{
    SilvaParsura* parsura = _parsare(piscina, fons);
    SilvaSemantica* sem;

    CREDO_NON_NIHIL (parsura);
    si (parsura == NIHIL)
    {
        redde;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    CREDO_NON_NIHIL (sem);
    si (sem != NIHIL)
    {
        CREDO_AEQUALIS_I32 (
            (i32)silva_c89_diagnostica_numerus(sem), ZEPHYRUM);
    }
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

    /* ========================================================
     * IX. Sedes (chunk C): codices novi flagrant; legalia PURA
     * ======================================================== */
    imprimere("\n--- Probans sedes (chunk C) ---\n");

    /* locus */
    _codicem_probare(piscina,
        "static void f(void) { 5 = 1; }\n",
        (s32)EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS, I);
    _codicem_probare(piscina,
        "static void f(void) { int a; int b; (a + b) = 1; }\n",
        (s32)EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS, I);
    _codicem_probare(piscina,
        "static void f(void) { const int c = 1; c = 2; }\n",
        (s32)EXAMEN_CODEX_LOCUS_IMMUTABILIS, I);
    _codicem_probare(piscina,
        "static void f(void) { int a[2]; a = 0; }\n",
        (s32)EXAMEN_CODEX_LOCUS_IMMUTABILIS, I);
    _codicem_probare(piscina,
        "static void f(void) { int i = 0; (i + 1)++; }\n",
        (s32)EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS, I);
    _codicem_probare(piscina,
        "static void f(void) { const int c = 1; ++c; }\n",
        (s32)EXAMEN_CODEX_LOCUS_IMMUTABILIS, I);
    _codicem_probare(piscina,
        "static void f(void) { int* p = &(1 + 2); }\n",
        (s32)EXAMEN_CODEX_ASSIGNATIO_NON_LOCUS, I);

    /* aritas */
    _codicem_probare(piscina,
        "static int g(int a, int b) { return a + b; }\n"
        "static int f(void) { return g(1); }\n",
        (s32)EXAMEN_CODEX_NUMERUS_ARGUMENTORUM, I);
    _codicem_probare(piscina,
        "static int g(int a, int b) { return a + b; }\n"
        "static int f(void) { return g(1, 2, 3); }\n",
        (s32)EXAMEN_CODEX_NUMERUS_ARGUMENTORUM, I);

    /* redde */
    _codicem_probare(piscina,
        "static void v(void) { return 5; }\n",
        (s32)EXAMEN_CODEX_REDDE_CUM_VALORE_IN_VACUO, I);
    _codicem_probare(piscina,
        "static int f(void) { return; }\n",
        (s32)EXAMEN_CODEX_REDDE_SINE_VALORE, I);

    /* condicio scalaris */
    _codicem_probare(piscina,
        "struct S { int a; };\n"
        "static void f(void) { struct S s; if (s) { s.a = 1; } }\n",
        (s32)EXAMEN_CODEX_CONDICIO_NON_SCALARIS, I);

    /* vocatio implicita (SUSPECTUM) */
    _codicem_probare(piscina,
        "static int f(void) { return ignotus(); }\n",
        (s32)EXAMEN_CODEX_VOCATIO_IMPLICITA, I);

    /* redeclaratio incompatibilis (+ socius) */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "extern int x;\n"
            "extern char x;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_REDECLARATIO_INCOMPATIBILIS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->socius != NIHIL);
            CREDO_AEQUALIS_I32 (d->linea, II);
        }
    }

    /* sedes finis (per iudicium) */
    _codicem_probare(piscina,
        "static void f(int* p, char* q) { p = q; }\n",
        (s32)EXAMEN_CODEX_MONSTRATORES_INCOMPATIBILES, I);
    _codicem_probare(piscina,
        "static void f(int* p) { int n; n = p; }\n",
        (s32)EXAMEN_CODEX_MONSTRATOR_INTEGER, I);
    _codicem_probare(piscina,
        "static void f(char* q) { const char* c = q; q = c; }\n",
        (s32)EXAMEN_CODEX_QUALES_ABIECTI, I);
    _codicem_probare(piscina,
        "struct S { int a; }; struct T { int a; };\n"
        "static void f(void) { struct S s; struct T t;\n"
        "  s = t; }\n",
        (s32)EXAMEN_CODEX_ASSIGNATIO_INCOMPATIBILIS, I);

    /* profundatio operatorum */
    _codicem_probare(piscina,
        "static int f(int* p, char* q) { return p == q; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_INCOMPATIBILIUM, I);
    _codicem_probare(piscina,
        "static long f(int* p, char* q) { return p - q; }\n",
        (s32)EXAMEN_CODEX_SUBTRACTIONIS_INCOMPATIBILIA, I);

    /* LEGALIA PURA (custodes C4: cauda aggregata, chorda in aciem,
     * vacuum*, crementa, &functio, comparatio cum nulla/vacuo) */
    _purum_probare(piscina,
        "struct S { int a; };\n"
        "static void f(void) { struct S a; struct S b; a = b;\n"
        "  a.a = 1; }\n");
    _purum_probare(piscina,
        "static char s[] = \"ab\";\n"
        "static char e[4] = \"xyz\";\n");
    _purum_probare(piscina,
        "static void f(int* p, void* v) { p = v; v = p;\n"
        "  p = 0; }\n");
    _purum_probare(piscina,
        "static void f(void) { int i = 0; i++; ++i; --i; i--; }\n");
    _purum_probare(piscina,
        "static void g(void) { ; }\n"
        "static void f(void) { void (*p)(void) = &g; p = g; }\n");
    _purum_probare(piscina,
        "static int f(int* p, void* v) { return p == v\n"
        "  || p == 0; }\n");
    _purum_probare(piscina,
        "static int g(int a, ...) { return a; }\n"
        "static int f(void) { return g(1, 2, 3); }\n");
    _purum_probare(piscina,
        "static void f(void) { const char* c = \"x\"; c = 0; }\n");

    /* ========================================================
     * X. Macro domesticum in capite alieno (sequela M4a): ordo
     * pravus SUSPECTUM ad 1:1 (dedup unum per par, causa omnia
     * nominat); ordo rectus TACET; lexicon (ISO) TACET;
     * transitivum (inclusum ab alieno) clamat
     * ======================================================== */
    {
        constans character* domesticum = "#define PLUVIA 100\n";
        constans character* specimen =
            "void aliena_f(char PLUVIA);\n"
            "void aliena_g(char PLUVIA);\n";
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        /* ordo pravus: domesticum ante specimen - unum (dedup) */
        sem = _analysare_alienum(piscina,
            "#include \"domesticum_probatio.h\"\n"
            "#include \"vendor/specimen_alienum.h\"\n"
            "int probe_x;\n",
            "domesticum_probatio.h", domesticum,
            "specimen_alienum.h", specimen,
            NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_codicem_numerare(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO), I);
            d = _diagnosticum_codicis(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO);
            CREDO_NON_NIHIL (d);
            si (d != NIHIL)
            {
                CREDO_VERUM (d->severitas
                    == (s32)EXAMEN_SUSPECTUM);
                CREDO_AEQUALIS_I32 (d->linea, I);
                CREDO_AEQUALIS_I32 (d->columna, I);
                CREDO_VERUM (d->via.mensura
                    == (i32)strlen("probatio_examen.c"));
                CREDO_VERUM (!d->provisionale);
                /* causa nominat: macro, fontem alienum (via
                 * scripta), definientem */
                CREDO_VERUM (strstr(d->causa,
                    "macro domesticum 'PLUVIA'") != NIHIL);
                CREDO_VERUM (strstr(d->causa,
                    "vendor/specimen_alienum.h:1") != NIHIL);
                CREDO_VERUM (strstr(d->causa,
                    "ANTE domesticum_probatio.h") != NIHIL);
            }
        }

        /* ordo rectus: specimen ante domesticum - TACET */
        sem = _analysare_alienum(piscina,
            "#include \"vendor/specimen_alienum.h\"\n"
            "#include \"domesticum_probatio.h\"\n"
            "int probe_y;\n",
            "domesticum_probatio.h", domesticum,
            "specimen_alienum.h", specimen,
            NIHIL, NIHIL, NIHIL);
        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_codicem_numerare(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO),
                ZEPHYRUM);
        }

        /* lexicon (ISO fictum): macro ex lexico in alieno - TACET
         * (est_lexicon; codex vendicatus NULL... iure adhibet) */
        sem = _analysare_alienum(piscina,
            "#include \"vendor/specimen_alienum.h\"\n"
            "int probe_z;\n",
            "specimen_alienum.h",
            "void aliena_h(char NIVIS);\n",
            NIHIL, NIHIL, NIHIL, NIHIL,
            "#define NIVIS 100\n");
        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_codicem_numerare(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO),
                ZEPHYRUM);
        }

        /* transitivum: alienum caput alterum includit (via scripta
         * SINE "vendor/") - alienitas per graphum propagatur */
        sem = _analysare_alienum(piscina,
            "#include \"domesticum_probatio.h\"\n"
            "#include \"vendor/specimen_alienum.h\"\n"
            "int probe_w;\n",
            "domesticum_probatio.h", domesticum,
            "specimen_alienum.h",
            "#include \"profundum_alienum.h\"\n",
            "profundum_alienum.h",
            "void aliena_p(char PLUVIA);\n",
            NIHIL);
        CREDO_NON_NIHIL (sem);
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_codicem_numerare(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO), I);
            d = _diagnosticum_codicis(sem,
                (s32)EXAMEN_CODEX_MACRO_DOMESTICUM_IN_ALIENO);
            CREDO_NON_NIHIL (d);
            si (d != NIHIL)
            {
                /* margo signans = inclusio profundi (in specimine
                 * scripta) - via eius in causa */
                CREDO_VERUM (strstr(d->causa,
                    "profundum_alienum.h:1") != NIHIL);
            }
        }
    }

    /* ========================================================
     * XI. Conversio signi (gradus DOMESTICUM): decipulae flagrant
     * (redde/-I argumentum/assignatio/UAC/init), suppressiones
     * tacent (cast/comparatio/constans-capit/promotiones/finis
     * maior/enum = limes nominatus). Calibratum contra clang
     * -Wsign-conversion 2026-07-16.
     * ======================================================== */
    imprimere("\n--- Probans conversionem signi ---\n");

    /* decipula domus #1: redde -I ex functione insignata */
    _codicem_probare(piscina,
        "static unsigned f(void) { return -1; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* -I ut argumentum prototypatum insignatum */
    _codicem_probare(piscina,
        "static void g(unsigned u) { (void)u; }\n"
        "static void f(void) { g(-1); }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* assignatio signatum -> insignatum (non constans) */
    _codicem_probare(piscina,
        "static void f(int s) { unsigned u; u = s; (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* insignatum -> signatum eiusdem magnitudinis */
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* UAC: signatum in insignatum arithmetica */
    _codicem_probare(piscina,
        "static unsigned f(int s, unsigned u) { return s + u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* initiator */
    _codicem_probare(piscina,
        "static void f(int s) { unsigned u = s; (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* constans negativa in initiatore */
    _codicem_probare(piscina,
        "static void f(void) { unsigned u = -1; (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    /* character (signatus in hac machina) -> character insignatus */
    _codicem_probare(piscina,
        "static void f(char c) { unsigned char u; u = c;\n"
        "  (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);

    /* SUPPRESSIONES */
    /* cast explicita */
    _codicem_probare(piscina,
        "static void f(int s) { unsigned u = (unsigned)s;\n"
        "  (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* comparatio (phasis II nominata) */
    _codicem_probare(piscina,
        "static int f(int s, unsigned u) { return s < u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* constans capit finem */
    _codicem_probare(piscina,
        "static void f(void) { unsigned u = 5; u = 0; (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* promotio valores servans (u8 -> int / long) */
    _codicem_probare(piscina,
        "static void f(unsigned char c) { int i = c; long l = c;\n"
        "  (void)i; (void)l; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* insignatum -> signatum maius */
    _codicem_probare(piscina,
        "static void f(unsigned u) { long l = u; (void)l; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* enumeratus = limes nominatus (clang enums non-negativos
     * insignatos tractat - non iudicatur) */
    _codicem_probare(piscina,
        "enum E { A = 1, B = 2 };\n"
        "static void f(enum E e) { unsigned u = e; (void)u; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* motus: numerus motus non convertitur */
    _codicem_probare(piscina,
        "static unsigned f(unsigned u, int n) { return u << n; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);

    /* severitas + causa structa + positio */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static unsigned f(int s)\n"
            "{\n"
            "    return s;\n"
            "}\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_CONVERSIO_SIGNI);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->severitas == (s32)EXAMEN_DOMESTICUM);
            CREDO_AEQUALIS_I32 (d->linea, III);
            CREDO_VERUM (!d->provisionale);
            CREDO_VERUM (strstr(d->causa,
                "conversio signi implicita:") != NIHIL);
            CREDO_VERUM (strstr(d->causa, "int") != NIHIL);
            CREDO_VERUM (strstr(d->causa, "->") != NIHIL);
        }
        /* totalis: sola conversio signi - probe aestimatoris
         * strepitum non reliquit */
        si (sem != NIHIL)
        {
            CREDO_AEQUALIS_I32 (
                (i32)silva_c89_diagnostica_numerus(sem), I);
        }
    }

    /* ========================================================
     * XII. Gradus severi + TOLERA: severa flagrat ubi analysis
     * sana probare nequit sed clang tacet (54 IBI silet - pinna
     * XOR); TOLERA linea eadem/praecedente cum causa supprimit;
     * sine causa non supprimit + IRRITUM; inutile IRRITUM; codex
     * paritatis non suppressibilis.
     * ======================================================== */
    imprimere("\n--- Probans gradus severos + TOLERA ---\n");

    /* subtractio u8-u8 in insignatum: heuristica tacet (clang
     * quoque), sana probare nequit -> SEVERA I, paritas ZEPHYRUM */
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA, I);
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, ZEPHYRUM);
    /* additio manet sana - severa ZEPHYRUM */
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a + b; }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA, ZEPHYRUM);
    /* TOLERA linea eadem: suppressum, IRRITUM nullum */
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b;"
        " /* TOLERA CONVERSIO_SIGNI_SEVERA: probatio */ }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA, ZEPHYRUM);
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b;"
        " /* TOLERA CONVERSIO_SIGNI_SEVERA: probatio */ }\n",
        (s32)EXAMEN_CODEX_TOLERA_IRRITUM, ZEPHYRUM);
    /* TOLERA linea praecedente: suppressum */
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{\n"
        "    /* TOLERA CONVERSIO_SIGNI_SEVERA: probatio lineae */\n"
        "    return a - b;\n"
        "}\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA, ZEPHYRUM);
    /* sine causa: NON supprimit + IRRITUM */
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b; /* TOLERA CONVERSIO_SIGNI_SEVERA */ }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA, I);
    _codicem_probare(piscina,
        "static unsigned f(unsigned char a, unsigned char b)\n"
        "{ return a - b; /* TOLERA CONVERSIO_SIGNI_SEVERA */ }\n",
        (s32)EXAMEN_CODEX_TOLERA_IRRITUM, I);
    /* inutile (nihil in linea sua/proxima flagrat): IRRITUM */
    _codicem_probare(piscina,
        "/* TOLERA CONVERSIO_SIGNI_SEVERA: nusquam adhibitum */\n"
        "static int probe_i;\n",
        (s32)EXAMEN_CODEX_TOLERA_IRRITUM, I);
    /* codex ignotus (paritas NON suppressibilis): IRRITUM et
     * firing paritatis MANET */
    _codicem_probare(piscina,
        "static unsigned f(int s)\n"
        "{ return s; /* TOLERA CONVERSIO_SIGNI: vetitum */ }\n",
        (s32)EXAMEN_CODEX_CONVERSIO_SIGNI, I);
    _codicem_probare(piscina,
        "static unsigned f(int s)\n"
        "{ return s; /* TOLERA CONVERSIO_SIGNI: vetitum */ }\n",
        (s32)EXAMEN_CODEX_TOLERA_IRRITUM, I);
    /* commentarium ordinarium: nulla tabula, nullum IRRITUM */
    _codicem_probare(piscina,
        "static int probe_j; /* commentarium simplex */\n",
        (s32)EXAMEN_CODEX_TOLERA_IRRITUM, ZEPHYRUM);

    /* ========================================================
     * XIII. Comparationes (phasis II): SIGNORUM paritas
     * -Wsign-compare (calibratio viva 2026-07-17) + VANA
     * tautologica contra zephyrum (typus communis insignatus AUT
     * intervallum sanum; per macra videt - ZEPHYRUM!).
     * ======================================================== */
    imprimere("\n--- Probans comparationes ---\n");

    /* SIGNORUM flagrat: s<u, u>s, ==, !=, int<u64, short<u32 */
    _codicem_probare(piscina,
        "static int f(int s, unsigned u) { return s < u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, I);
    _codicem_probare(piscina,
        "static int f(int s, unsigned u) { return u > s; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, I);
    _codicem_probare(piscina,
        "static int f(int s, unsigned u) { return s == u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, I);
    _codicem_probare(piscina,
        "static int f(unsigned long ul, int s)\n"
        "{ return ul != s; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, I);
    _codicem_probare(piscina,
        "static int f(short h, unsigned u) { return h < u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, I);
    /* SIGNORUM tacet: constans nonneg, u8 promotum, larva,
     * long<u32 (valores servati), s<s */
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u > 5; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);
    _codicem_probare(piscina,
        "static int f(unsigned char c, unsigned u)\n"
        "{ return c < u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);
    _codicem_probare(piscina,
        "static int f(int s, unsigned u)\n"
        "{ return (s & 0xff) < u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);
    _codicem_probare(piscina,
        "static int f(long l, unsigned u) { return l < u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);
    _codicem_probare(piscina,
        "static int f(int a, int b) { return a < b; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);

    /* VANA flagrat: quattuor formae contra zephyrum + per macrum
     * (ZEPHYRUM via lexicon iniectum probatur infra manualiter) */
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u < 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u >= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    _codicem_probare(piscina,
        "static int f(unsigned u) { return 0 > u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    _codicem_probare(piscina,
        "static int f(unsigned u) { return 0 <= u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    /* proba subfluxus fracta: (u - v) >= 0 semper verum */
    _codicem_probare(piscina,
        "static int f(unsigned u, unsigned v)\n"
        "{ return u - v >= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    /* u8 promotum (typus communis signatus, intervallum sanum) */
    _codicem_probare(piscina,
        "static int f(unsigned char c) { return c >= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    /* VANA tacet: formae non tautologicae + signata */
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u > 0; }\n"
        "static int g(unsigned u) { return u <= 0; }\n"
        "static int h(unsigned u) { return u == 0; }\n"
        "static int k(int s) { return s < 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* subtractio signata: (a - b) >= 0 legitima */
    _codicem_probare(piscina,
        "static int f(int a, int b) { return a - b >= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* TOLERA supprimit VANAM (linea praecedente) */
    _codicem_probare(piscina,
        "static int f(unsigned u)\n"
        "{\n"
        "    /* TOLERA COMPARATIO_VANA: proba defensiva consulta */\n"
        "    return u >= 0;\n"
        "}\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* VANA et SIGNORUM se excludunt: u < 0 dat VANAM solam */
    _codicem_probare(piscina,
        "static int f(unsigned u) { return u < 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_SIGNORUM, ZEPHYRUM);
    /* idiom custodiae bilateralis (DECISUS): dimidium mortuum in
     * custodia || tacet - involutio a limite altero capta */
    _codicem_probare(piscina,
        "static int f(unsigned u, unsigned limes)\n"
        "{ return u < 0 || u >= limes; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* variatio &&: proba validitatis */
    _codicem_probare(piscina,
        "static int f(unsigned u, unsigned limes)\n"
        "{ return u >= 0 && u < limes; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* custodia multiplex (x et y, catena ||) - ambo dimidia tacent */
    _codicem_probare(piscina,
        "static int f(unsigned x, unsigned y, unsigned l,\n"
        "    unsigned a)\n"
        "{ return x < 0 || x >= l || y < 0 || y >= a; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, ZEPHYRUM);
    /* frater identificatorem ALIUM ordinat: NON idiom - flagrat */
    _codicem_probare(piscina,
        "static int f(unsigned u, unsigned v, unsigned l)\n"
        "{ return u < 0 || v >= l; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);
    /* X compositum in custodia: NON idiom (u - v semper flagrat) */
    _codicem_probare(piscina,
        "static int f(unsigned u, unsigned v, unsigned l)\n"
        "{ return u - v >= 0 || u < l; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_VANA, I);

    /* ==================================================
     * XIV. Chorda nuda: chorda.datum ad lectorem NUL
     * ================================================== */

    /* punctum + casus: forma canonica flagrat */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda c)"
        " { return strlen((const char*)c.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, I);
    /* sagitta per monstratorem */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda* p)"
        " { return strcmp((const char*)p->datum, \"x\"); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, I);
    /* ambo argumenta strcmp: bina flagrantia */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda a, chorda b)"
        " { return strcmp((const char*)a.datum,"
        " (const char*)b.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, II);
    /* parentheses intra casum despiciuntur */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda c)"
        " { return strlen((const char*)((c.datum))); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, I);
    /* NEGATIVUM: lector limitatus (strncmp) tacet */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda c)"
        " { return strncmp((const char*)c.datum, \"x\", c.m); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);
    /* NEGATIVUM: ager 'datum' tag ALTERIUS (non chorda) tacet -
     * praecisio typorum, non nominis agri (RegioClicca!) */
    _codicem_probare(piscina,
        "typedef struct alia { unsigned char* datum; } alia;\n"
        "static int f(alia x)"
        " { return strlen((const char*)x.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);
    /* strcpy: destinatio (positio 0) EXTRA larvam tacet... */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static void f(chorda c)"
        " { strcpy((char*)c.datum, \"x\"); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);
    /* ...fons (positio 1) flagrat */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static void f(chorda c, char* effugium)"
        " { strcpy(effugium, (const char*)c.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, I);
    /* TOLERA absorbet (severa-classis) */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static int f(chorda c) {\n"
        "    /* TOLERA CHORDA_NUDA: probatio suppressionis */\n"
        "    return strlen((const char*)c.datum);\n"
        "}\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);
    /* NEGATIVUM: identificator planus (non membrum) tacet */
    _codicem_probare(piscina,
        "static int f(char* p) { return strlen(p); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);

    /* ==================================================
     * XV. Signum formati: charta formati -> argumenta
     * ================================================== */

    /* %u cum signato flagrat */
    _codicem_probare(piscina,
        "static void f(int s) { printf(\"%u\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* %d cum insignato TACET (DECRETUM: involutio negativa
     * visibilis = diagnostica; directio innoxia non iudicatur) */
    _codicem_probare(piscina,
        "static void f(unsigned u) { printf(\"%d\", u); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* congruentia tacet */
    _codicem_probare(piscina,
        "static void f(int s, unsigned u)"
        " { printf(\"%d %u\", s, u); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* conversiones plures, positiones rectae: solum %u+s flagrat
     * (%d+u decreto exemptum) - positio secunda recte legitur */
    _codicem_probare(piscina,
        "static void f(unsigned u, int s)"
        " { printf(\"%d et %u\", u, s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* constans non negativa tacet (utraque directione tuta) */
    _codicem_probare(piscina,
        "static void f(void) { printf(\"%d\", 5u); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* u8 promotum ad int (signatum) - %d congruit naturaliter */
    _codicem_probare(piscina,
        "static void f(unsigned char b) { printf(\"%d\", b); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* %x idioma repraesentationis - exemptum */
    _codicem_probare(piscina,
        "static void f(int s) { printf(\"%x\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* argumentum stellatum non iudicatur (idioma \"%.*s\") */
    _codicem_probare(piscina,
        "static void f(unsigned m, char* p)"
        " { printf(\"%.*s\", m, p); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* modificator longitudinis transparens: %lu cum longo signato */
    _codicem_probare(piscina,
        "static void f(long s) { printf(\"%lu\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* formatum non litterale: tacet (non analysabile) */
    _codicem_probare(piscina,
        "static void f(char* fmt, int s) { printf(fmt, s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* fprintf: formatum in positione 1 */
    _codicem_probare(piscina,
        "static void f(void* fp, int s)"
        " { fprintf(fp, \"%u\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* %% litterale praetermittitur */
    _codicem_probare(piscina,
        "static void f(int s) { printf(\"100%% %u\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* fuga \\045 = '%' - decodator per fugas videt */
    _codicem_probare(piscina,
        "static void f(int s) { printf(\"\\045u\", s); }\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, I);
    /* TOLERA absorbet */
    _codicem_probare(piscina,
        "static void f(int s) {\n"
        "    /* TOLERA SIGNUM_FORMATI: probatio suppressionis */\n"
        "    printf(\"%u\", s);\n"
        "}\n",
        (s32)EXAMEN_CODEX_SIGNUM_FORMATI, ZEPHYRUM);
    /* %s cum chorda.datum -> codex 59 per chartam formati */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static void f(chorda c)"
        " { printf(\"%s\", (char*)c.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, I);
    /* \"%.*s\" cum datum: praecisio lectionem limitat - IDIOMA,
     * tacet (finis differitionis gradus chordae clausus) */
    _codicem_probare(piscina,
        "typedef struct chorda { unsigned m;"
        " unsigned char* datum; } chorda;\n"
        "static void f(chorda c)"
        " { printf(\"%.*s\", (int)c.m, (char*)c.datum); }\n",
        (s32)EXAMEN_CODEX_CHORDA_NUDA, ZEPHYRUM);

    /* ==================================================
     * XVI. Sentinella insignata + comparatio degradata
     * ================================================== */

    /* casus vestitus flagrat (forma quae clang silet) */
    _codicem_probare(piscina,
        "static unsigned f(void) { return (unsigned)-1; }\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, I);
    /* forma nuda = paritas 54, NON 61 (nulla duplicatio) */
    _codicem_probare(piscina,
        "static unsigned f(void) { return -1; }\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, ZEPHYRUM);
    /* constans non negativa tacet */
    _codicem_probare(piscina,
        "static unsigned f(void) { return (unsigned)0; }\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, ZEPHYRUM);
    /* functio signata: sentinella legitima */
    _codicem_probare(piscina,
        "static int f(void) { return (int)-1; }\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, ZEPHYRUM);
    /* parentheses intra/circa casum despiciuntur */
    _codicem_probare(piscina,
        "static unsigned f(void) { return ((unsigned)(-1)); }\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, I);
    /* TOLERA absorbet (idioma omnium-bitorum consultum) */
    _codicem_probare(piscina,
        "static unsigned f(void) {\n"
        "    /* TOLERA SENTINELLA_INSIGNATA: omnes biti consulto */\n"
        "    return (unsigned)-1;\n"
        "}\n",
        (s32)EXAMEN_CODEX_SENTINELLA_INSIGNATA, ZEPHYRUM);
    /* degradata DORMIT (decretum: idioma vacuitatis possidet
     * formam; re-armatur cum analysi fluxus) - u <= 0 tacet */
    _codicem_probare(piscina,
        "static unsigned f(unsigned u) { return u <= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA, ZEPHYRUM);
    /* degradata dormit: 0 >= u (ordo alter) etiam tacet */
    _codicem_probare(piscina,
        "static unsigned f(unsigned u) { return 0 >= u; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA, ZEPHYRUM);
    /* u < 0 = VANA, non degradata (nulla duplicatio) */
    _codicem_probare(piscina,
        "static unsigned f(unsigned u) { return u < 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA, ZEPHYRUM);
    /* signatum <= 0: comparatio vera - tacet */
    _codicem_probare(piscina,
        "static int f(int s) { return s <= 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA, ZEPHYRUM);
    /* u > 0 idioma sanum (!= 0 legibile) - tacet */
    _codicem_probare(piscina,
        "static unsigned f(unsigned u) { return u > 0; }\n",
        (s32)EXAMEN_CODEX_COMPARATIO_DEGRADATA, ZEPHYRUM);

    /* ==================================================
     * XVII. Fluxus-0: semita sine redditu + violationes fluxus
     * (codices 63/66/67; spec silva-fluxus-0-spec.md)
     * ================================================== */

    /* si sine alioquin: semita falsa cadit - flagrat */
    _codicem_probare(piscina,
        "static int f(int x) { if (x > 0) { return 1; } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, I);
    /* omnes semitae reddunt - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { if (x > 0) { return 1; }"
        " return 0; }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* functio vacua cadit legitime - tacet */
    _codicem_probare(piscina,
        "static void f(int x) { x = x + 1; }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* commutatio incompleta sine redde postremo - flagrat */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: return 1; } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, I);
    /* idioma classificatoris (ordinarius reddit) - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: return 1;"
        " default: return 2; } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* ansa variabilis sine redde post - flagrat */
    _codicem_probare(piscina,
        "static int f(int x) { while (x) { x = x - 1; } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, I);
    /* dum (1): plicatio aestimatoris - margo falsus abest, tacet
     * (PARITAS: clang -Wreturn-type hic silet) */
    _codicem_probare(piscina,
        "static int f(void) { while (1) { } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* dum (2 - 1): PLICATIO vera, non litterae solae - tacet */
    _codicem_probare(piscina,
        "static int f(void) { while (2 - 1) { } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* per (;;): conditio absens = semper vera - tacet */
    _codicem_probare(piscina,
        "static int f(void) { for (;;) { } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* ansa per salta: attingibilitas pura - tacet */
    _codicem_probare(piscina,
        "static int f(void) { iterum: goto iterum; }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);
    /* exitus solus intra dum(1) (forma _aspicere_eff) - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { while (1) { if (x) { return 1; }"
        " x = x - 1; } }\n",
        (s32)EXAMEN_CODEX_SEMITA_SINE_REDDITU, ZEPHYRUM);

    /* codex 66: frange extra contextum (clang ERRAT - VIOLATIO) */
    _codicem_probare(piscina,
        "static int f(int x) { break; return x; }\n",
        (s32)EXAMEN_CODEX_FRANGE_EXTRA_CONTEXTUM, I);
    /* perge intra commutationem sine ansa - transit, flagrat */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: continue; }"
        " return x; }\n",
        (s32)EXAMEN_CODEX_FRANGE_EXTRA_CONTEXTUM, I);
    /* frange in ansa legitima - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { while (x) { break; } return x; }\n",
        (s32)EXAMEN_CODEX_FRANGE_EXTRA_CONTEXTUM, ZEPHYRUM);

    /* codex 67: salta ad titulum ignotum (clang ERRAT - VIOLATIO) */
    _codicem_probare(piscina,
        "static int f(int x) { goto ignotum; return x; }\n",
        (s32)EXAMEN_CODEX_SALTA_AD_TITULUM_IGNOTUM, I);
    /* salta ad titulum notum - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { goto finis; finis: return x; }\n",
        (s32)EXAMEN_CODEX_SALTA_AD_TITULUM_IGNOTUM, ZEPHYRUM);

    /* codex 64: lapsus incustoditus - flagrat ad titulum recipientem */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: x = x + 1;"
        " case 1: x = x - 1; break; } return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, I);
    /* lapsus custoditus (si frange) - semita cadens manet, flagrat
     * (forma arbor_syntaxis:3491, path-based ut clang) */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: if (x)"
        " { break; } case 1: x = x - 1; break; } return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, I);
    /* cumulus titulorum (grex vacuus) - tacet ut clang */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: case 1:"
        " x = x - 1; break; } return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, ZEPHYRUM);
    /* frange-terminatus - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: x = x + 1;"
        " break; case 1: x = x - 1; break; } return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, ZEPHYRUM);
    /* lapsus in ordinarium - flagrat etiam */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: x = x + 1;"
        " default: x = x - 1; } return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, I);
    /* grex ultimus e commutatione cadit - NON lapsus, tacet */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { case 0: x = x + 1; }"
        " return x; }\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, ZEPHYRUM);
    /* TOLERA supprimit (decisio Q2: TOLERA solum) */
    _codicem_probare(piscina,
        "static int f(int x)\n{\n    switch (x) {\n"
        "    case 0: x = x + 1;\n"
        "    /* TOLERA CASUS_LAPSUS: probatio suppressionis */\n"
        "    case 1: x = x - 1; break;\n    }\n    return x;\n}\n",
        (s32)EXAMEN_CODEX_CASUS_LAPSUS, ZEPHYRUM);

    /* codex 65: sententia vera post redde - flagrat */
    _codicem_probare(piscina,
        "static int f(int x) { return x; x = 99; return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, I);
    /* redde solum post redde - saltus defensivus, tacet (clang
     * sub-vexillo -return solum) */
    _codicem_probare(piscina,
        "static int f(int x) { return x; return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, ZEPHYRUM);
    /* sententiae ante titulum primum commutationis - flagrant */
    _codicem_probare(piscina,
        "static int f(int x) { switch (x) { x = 1; case 0:"
        " x = x - 1; } return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, I);
    /* redde post ansam infinitam - tacet (paritas plana; clang
     * -aggressive solum) */
    _codicem_probare(piscina,
        "static int f(void) { for (;;) { } return 0; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, ZEPHYRUM);
    /* codex attingibilis - tacet */
    _codicem_probare(piscina,
        "static int f(int x) { if (x) { x = x - 1; } return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, ZEPHYRUM);
    /* TOLERA supprimit */
    _codicem_probare(piscina,
        "static int f(int x)\n{\n    return x;\n"
        "    /* TOLERA SENTENTIA_INATTINGIBILIS: probatio */\n"
        "    x = 99;\n    return x;\n}\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, ZEPHYRUM);
    /* si (MACRO disabilitatum): exemptum - clang idem tacet
     * (idioma PISCINA_DEBUG; calibratio 2026-07-17) */
    _codicem_probare(piscina,
        "#define DEBUG_PROBATIONIS 0\n"
        "static void g(void);\n"
        "static int f(int x) { if (DEBUG_PROBATIONIS) { g(); }"
        " return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, ZEPHYRUM);
    /* si (0) litterale: NON exemptum - clang flagrat */
    _codicem_probare(piscina,
        "static void g(void);\n"
        "static int f(int x) { if (0) { g(); } return x; }\n",
        (s32)EXAMEN_CODEX_SENTENTIA_INATTINGIBILIS, I);

    /* ==================================================
     * XVII-b. Fluxus-1 chunk A: datorum per sedem semanticae
     * (facta VERA - umbrae, parametra, constantia s04f)
     * ================================================== */
    {
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        FluxusFunctionis* fluxus;

        /* umbrae: x interior et exterior identitates DISTINCTAE
         * (a + x-interior + x-exterior = variabiles III) */
        parsura = _parsare(piscina,
            "static int f(int a) { int x; { int x; x = a; }"
            " x = 2; return x; }\n");
        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 (xar_numerus(sem->fluxus_functionum), I);
        fluxus = *(FluxusFunctionis**)xar_obtinere(
            sem->fluxus_functionum, ZEPHYRUM);
        CREDO_NON_NIHIL (fluxus->datorum);
        CREDO_AEQUALIS_I32 (
            xar_numerus(fluxus->datorum->variabiles), III);

        /* parametrum T* constans: peek(&x) NULLUM eventum (s04f
         * per typum verum); geminus fill(&x) = eventum unum */
        parsura = _parsare(piscina,
            "void peek(const int *p);\n"
            "static void f(void) { int x; peek(&x); }\n");
        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        fluxus = *(FluxusFunctionis**)xar_obtinere(
            sem->fluxus_functionum, ZEPHYRUM);
        CREDO_NON_NIHIL (fluxus->datorum);
        CREDO_AEQUALIS_I32 (
            xar_numerus(fluxus->datorum->variabiles), ZEPHYRUM);

        parsura = _parsare(piscina,
            "void fill(int *p);\n"
            "static void f(void) { int x; fill(&x); }\n");
        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        fluxus = *(FluxusFunctionis**)xar_obtinere(
            sem->fluxus_functionum, ZEPHYRUM);
        CREDO_NON_NIHIL (fluxus->datorum);
        CREDO_AEQUALIS_I32 (
            xar_numerus(fluxus->datorum->variabiles), I);
    }

    /* ==================================================
     * XVIII. Angustatio (codex 68) - directio latitudinis
     * ================================================== */

    /* variabilis latior eiusdem signi - flagrat */
    _codicem_probare(piscina,
        "static void f(unsigned int m)"
        " { unsigned char p = m; (void)p; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, I);
    /* int -> short signatus - flagrat */
    _codicem_probare(piscina,
        "static void f(int m) { short b = m; (void)b; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, I);
    /* constans capiens - tacet */
    _codicem_probare(piscina,
        "static void f(void) { unsigned char p = 100; (void)p; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, ZEPHYRUM);
    /* constans NON capiens (trans-signum: int -> u8, 54 tacet
     * non-negativo - LACUNA inter gradus clausa) - flagrat */
    _codicem_probare(piscina,
        "static void f(void) { unsigned char p = 300; (void)p; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, I);
    /* -1 in s8 capit (tabulae base64!) - tacet */
    _codicem_probare(piscina,
        "static void f(void) { signed char n = -1; (void)n; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, ZEPHYRUM);
    /* -200 in s8 non capit - flagrat */
    _codicem_probare(piscina,
        "static void f(void) { signed char n = -200; (void)n; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, I);
    /* casus explicitus silet */
    _codicem_probare(piscina,
        "static void f(unsigned int m)"
        " { unsigned char p = (unsigned char)m; (void)p; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, ZEPHYRUM);
    /* larva intervallum capiens - tacet (ut clang) */
    _codicem_probare(piscina,
        "static void f(unsigned int m)"
        " { unsigned char p = m & 0xFFu; (void)p; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, ZEPHYRUM);
    /* ternarius bracchiis capientibus (forma selectio) - tacet */
    _codicem_probare(piscina,
        "static void f(int c, char y)"
        " { char b = c ? ' ' : (char)y; (void)b; }\n",
        (s32)EXAMEN_CODEX_ANGUSTATIO, ZEPHYRUM);

    /* ==================================================
     * XIX. Inutilia (codices 69/70)
     * ================================================== */

    /* variabilis localis numquam lecta - flagrat */
    _codicem_probare(piscina,
        "static void f(void) { int mortuus; }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, I);
    /* (void)x usus est (nexus) - tacet */
    _codicem_probare(piscina,
        "static void f(void) { int visus; (void)visus; }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, ZEPHYRUM);
    /* initiator cum effectu NON eximit (calibratio clang) */
    _codicem_probare(piscina,
        "static int g(void);\n"
        "static void f(void) { int e = g(); }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, I);
    /* staticus localis inutilis - flagrat ut clang */
    _codicem_probare(piscina,
        "static void f(void) { static int s; }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, I);
    /* externus localis = declaratio, non definitio - tacet */
    _codicem_probare(piscina,
        "static void f(void) { extern int e; }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, ZEPHYRUM);
    /* parametrum inutile - codex 70, non 69 */
    _codicem_probare(piscina,
        "static int f(int usatus, int otiosus) { return usatus; }\n",
        (s32)EXAMEN_CODEX_PARAMETRUM_INUTILE, I);
    _codicem_probare(piscina,
        "static int f(int usatus, int otiosus) { return usatus; }\n",
        (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS, ZEPHYRUM);
    /* parametrum per (vacuum) usatum - tacet */
    _codicem_probare(piscina,
        "static void f(int contextus) { (void)contextus; }\n",
        (s32)EXAMEN_CODEX_PARAMETRUM_INUTILE, ZEPHYRUM);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
