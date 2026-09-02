/* probatio_silva_fluxus.c - FLUXUS-0: CFG + attingibilitas
 * (specificatio project-specs/silva-fluxus-0-spec.md; batteria e
 * simulatione XI: formae linearum/si/ansarum/commutationis/saltuum,
 * specimina corporis nominata [_aspicere_eff, iterum-salta],
 * contrastum cum/sine aestimatore [sutura auxiliorum = paritas]) */
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
#include "silva_c89_fluxus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior constans SilvaNodus*
_nodalis_probationis (
    SilvaValor v)
{
    si (v.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v.datum.nodus;
}

interior constans SilvaNodus*
_nodus (
    constans SilvaParsura* parsura,
                      i32  index)
{
    SilvaValor* e = silva_valor_lista_obtinere(
        parsura->commissio->radix, index);

    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}

/* Aestimator litteralis parvus: folium-integer decimale per
 * parentheses - sufficit probationibus (semantica plicationem
 * plenam tradet in gradu 2) */
interior b32
_aestimator_litteralis (
                 vacuum* contextus,
    constans SilvaNodus* expressio,
                    s64* valor)
{
    SilvaValor tok_v;
        chorda textus;
           s64 v = ZEPHYRUM;
           i32 i;

    (vacuum)contextus;
    dum (   expressio        != NIHIL
         && expressio->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        expressio = _nodalis_probationis(
            silva_c89_parenthesis_internum(expressio));
    }
    si (   expressio        == NIHIL
        || expressio->genus != (s32)SILVA_C89_GENUS_FOLIUM_INTEGER)
    {
        redde FALSUM;
    }
    tok_v = silva_c89_folium_integer_tok_valor(expressio);
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    textus = tok_v.datum.token->valor;
    si (textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        character c = (character)textus.datum[i];

        si (c < '0' || c > '9')
        {
            redde FALSUM;
        }
        v = v * X + (s64)(c - '0');
    }
    *valor = v;
    redde VERUM;
}

/* Fistula probationis: fons -> parsura -> CFG definitionis primae */
interior FluxusFunctionis*
_fluere (
               Piscina* piscina,
    constans character* fons,
                   b32  cum_aestimatore)
{
    SilvaParsura* parsura = silva_c89_parsare(piscina, "probatio.c",
        fons, (i32)strlen(fons), NIHIL);
    constans SilvaNodus* definitio;
          FluxusAuxilia  aux;

    si (parsura == NIHIL || parsura->numerus_errorum != ZEPHYRUM)
    {
        redde NIHIL;
    }
    definitio = _nodus(parsura, ZEPHYRUM);
    si (definitio == NIHIL)
    {
        redde NIHIL;
    }
    si (cum_aestimatore)
    {
        aux.canonicum   = NIHIL;
        aux.aestimator  = _aestimator_litteralis;
        aux.contextus   = NIHIL;
        redde silva_c89_fluxus_aedificare(piscina, definitio, &aux);
    }
    redde silva_c89_fluxus_aedificare(piscina, definitio, NIHIL);
}

/* Numerus marginum generis dati trans omnes blocos */
interior i32
_margines_generis (
    constans FluxusFunctionis* fluxus,
                          s32  genus)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(fluxus->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, b);
        i32 m;
        i32 numerus_marginum = xar_numerus(blocus->margines);

        per (m = ZEPHYRUM; m < numerus_marginum; m++)
        {
            constans FluxusMargo* margo = (constans FluxusMargo*)
                xar_obtinere(blocus->margines, m);

            si (margo->genus == genus)
            {
                summa++;
            }
        }
    }
    redde summa;
}

/* Numerus blocorum attingibilium (fissio: omnes segmenta vivere
 * debent - probatio marginis retro ansarum) */
interior i32
_bloci_attingibiles (
    constans FluxusFunctionis* fluxus)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(fluxus->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, b);

        si (blocus->attingibilis)
        {
            summa++;
        }
    }
    redde summa;
}

/* Summa granulorum trans omnes blocos */
interior i32
_sententiae_totae (
    constans FluxusFunctionis* fluxus)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(fluxus->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, b);

        summa += xar_numerus(blocus->sententiae);
    }
    redde summa;
}

/* Bloci inattingibiles NON vacui (cibus codicis 65) */
interior i32
_bloci_mortui_pleni (
    constans FluxusFunctionis* fluxus)
{
    i32 summa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(fluxus->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, b);

        si (   !blocus->attingibilis
            && xar_numerus(blocus->sententiae) > ZEPHYRUM)
        {
            summa++;
        }
    }
    redde summa;
}

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_fluxus", 33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: formae lineares et si
     * ======================================================== */

    {
        FluxusFunctionis* f;

        /* linearis cum redde: finis numquam cadit */
        f = _fluere(piscina,
            "int a(int x) { x = x + 1; return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), I);

        /* vacuum sine redde: cadit (vexillum typo-caecum) */
        f = _fluere(piscina, "void b(int x) { x = 1; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f->cadit_attingibilis);

        /* si sine alioquin: semita falsa cadit (specimen 63) */
        f = _fluere(piscina,
            "int c(int x) { if (x > 0) { return 1; } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), I);

        /* si/alioquin ambo reddunt: iunctio inattingibilis */
        f = _fluere(piscina,
            "int d(int x) { if (x > 0) { return 1; }"
            " else { return 0; } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), II);
    }


    /* ========================================================
     * PROBARE: ansae - conditio constans = sutura auxiliorum
     * ======================================================== */

    {
        FluxusFunctionis* f;

        /* dum variabilis: exitus falsus, cadit post redde FALSUM */
        f = _fluere(piscina,
            "int e(int x) { while (x) { x = x - 1; } return x; }",
            FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), I);

        /* dum (1) CUM aestimatore: margo falsus omissus - postis
         * inattingibilis - cadit FALSUM (paritas -Wreturn-type!) */
        f = _fluere(piscina, "int f(void) { while (1) { } }", VERUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), ZEPHYRUM);

        /* eadem SINE aestimatore: margo falsus manet - cadit VERUM.
         * CONTRASTUM CONSULTO: pretium suturae mensuratum */
        f = _fluere(piscina, "int f(void) { while (1) { } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f->cadit_attingibilis);

        /* per (;;): conditio ABSENS = semper vera SINE aestimatore */
        f = _fluere(piscina, "int g(void) { for (;;) { } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);

        /* fac..dum (1) cum aestimatore */
        f = _fluere(piscina,
            "int h(int x) { do { x = x - 1; } while (1); }", VERUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);

        /* specimen corporis: exitus solus intra dum(1) - forma
         * silva_lexema._aspicere_eff (validatio XI-6) */
        f = _fluere(piscina,
            "int i(int x) { while (1) { if (x) { return 1; }"
            " x = x - 1; } }", VERUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);

        /* perge: destinatio caput ansae */
        f = _fluere(piscina,
            "int j(int x) { while (x) { if (x == 2) { continue; }"
            " x = x - 1; } return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_PERSECUTIO), I);
    }


    /* ========================================================
     * PROBARE: commutatio - dispersio, lapsus, ordinarius
     * ======================================================== */

    {
        FluxusFunctionis* f;

        /* idioma classificatoris: omnes rami reddunt, ordinarius
         * praesens, SINE redde postremo - cadit FALSUM (idioma
         * dominans corporis, XI-6) */
        f = _fluere(piscina,
            "int k(int x) { switch (x) { case 0: return 1;"
            " case 1: return 2; default: return 3; } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_CASUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_ORDINARIUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_LAPSUS), ZEPHYRUM);

        /* sine ordinario: semita non-congruentiae implicita */
        f = _fluere(piscina,
            "int l(int x) { switch (x) { case 0: return 1; }"
            " return 9; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_ORDINARIUS), I);

        /* commutatio incompleta sine redde postremo: cadit VERUM
         * (specimen calibrationis) */
        f = _fluere(piscina,
            "int m(int x) { switch (x) { case 0: return 1;"
            " case 1: return 2; } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_VERUM (f->cadit_attingibilis);

        /* cumulus titulorum: grex vacuus - margo lapsus adest
         * (gradus 64 vacuitate filtrat, non aedificator) */
        f = _fluere(piscina,
            "int n(int x) { switch (x) { case 0: case 1:"
            " x = x - 1; break; } return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_LAPSUS), I);

        /* lapsus custoditus: si (x) frange; - finis gregis
         * attingibilis - margo lapsus (forma arbor_syntaxis:3491,
         * lapsus verus unicus corporis) */
        f = _fluere(piscina,
            "int o(int x) { switch (x) { case 0: if (x) { break; }"
            " case 1: x = x - 1; break; } return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_LAPSUS), I);

        /* lapsus incustoditus */
        f = _fluere(piscina,
            "int p(int x) { switch (x) { case 0: x = x + 1;"
            " case 1: x = x - 1; break; } return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_LAPSUS), I);

        /* sententiae ante titulum primum: blocus orbus non vacuus
         * (inattingibilis naturaliter - cibus codicis 65) */
        f = _fluere(piscina,
            "int q(int x) { switch (x) { x = 1; case 0:"
            " x = x - 1; } return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_bloci_mortui_pleni(f), I);
    }


    /* ========================================================
     * PROBARE: tituli, salta, violationes ut data
     * ======================================================== */

    {
        FluxusFunctionis* f;

        /* salta antrorsum et retrorsum */
        f = _fluere(piscina,
            "int r(int x) { if (x) { goto finis; } x = x - 1;"
            " iterum: x = x + 1; if (x < 10) { goto iterum; }"
            " finis: return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (xar_numerus(f->tituli), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_SALTUS), II);
        CREDO_AEQUALIS_I32 (xar_numerus(f->saltus_ignoti), ZEPHYRUM);
        CREDO_FALSUM (f->cadit_attingibilis);

        /* ansa per salta solum: attingibilitas pura - cadit FALSUM
         * (specimen calibrationis: clang tacet sine plicatione) */
        f = _fluere(piscina, "int s(void) { iterum: goto iterum; }",
            FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_SALTUS), I);

        /* salta ad titulum ignotum: datum codicis 67 */
        f = _fluere(piscina, "int t(int x) { goto ignotum;"
            " return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (xar_numerus(f->saltus_ignoti), I);

        /* frange extra contextum: datum codicis 66 */
        f = _fluere(piscina, "int u(int x) { break; return x; }",
            FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (xar_numerus(f->fractiones_extra), I);

        /* perge intra commutationem sine ansa: transit commutationem
         * - etiam datum codicis 66 */
        f = _fluere(piscina,
            "int v(int x) { switch (x) { case 0: continue; }"
            " return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (xar_numerus(f->fractiones_extra), I);

        /* sententiae post redde: blocus orbus plenus */
        f = _fluere(piscina,
            "int w(int x) { return x; x = 99; return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_bloci_mortui_pleni(f), I);
    }


    /* ========================================================
     * PROBARE: conditionalis - semita sumpta transit
     * ======================================================== */

    {
        FluxusFunctionis* f;

        f = _fluere(piscina,
            "int y(int x)\n{\n#if 1\n    x = x + 1;\n#else\n"
            "    x = x - 1;\n#endif\n    return x;\n}\n", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), I);
    }


    /* ========================================================
     * PROBARE: fissio sectionum brevium (FLUXUS-1 chunk 0)
     * ======================================================== */

    {
        FluxusFunctionis* f;

        /* conditio simplex GRANULUM est (fons unicus ambulationis):
         * granula = conditio + redde = II */
        f = _fluere(piscina,
            "int c2(int x) { if (x > 0) { return 1; } }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_sententiae_totae(f), II);

        /* si (a && b): cascada - margines sectionis IPSI margines
         * rami (VERUS 2 = a->dexter, b->dein; FALSUS 2 = breves) */
        f = _fluere(piscina,
            "int f1(int a, int b) { if (a && b) { return 1; }"
            " return 0; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), II);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));
        CREDO_AEQUALIS_I32 (_bloci_mortui_pleni(f), ZEPHYRUM);

        /* positio sententiae: x = a && b - fissio cum iunctione */
        f = _fluere(piscina,
            "int f2(int a, int b) { int x; x = a && b;"
            " return x; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), I);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* ternarius in positione sententiae */
        f = _fluere(piscina,
            "int f3(int c) { int x; x = c ? 1 : 2; return x; }",
            FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), I);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* dum (a || b): margo retro caput petit (segmentum PRIMUM -
         * aestimatio tota recomincat); omnes bloci vivi */
        f = _fluere(piscina,
            "int f4(int a, int b) { while (a || b) { a = a - 1; }"
            " return a; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), II);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* plicatio partialis NON fit in aedificatore: 0 && x sine
         * aestimatore toto = cascada cum AMBOBUS marginibus (nullum
         * falsum inattingibile; plicatio tota = officium
         * aestimatoris) */
        f = _fluere(piscina,
            "int f5(int x) { if (0 && x) { return 1; } return 0; }",
            VERUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), II);
        CREDO_AEQUALIS_I32 (_bloci_mortui_pleni(f), ZEPHYRUM);

        /* redde a && b: REDDITUS ex bloco iunctionis */
        f = _fluere(piscina,
            "int f6(int a, int b) { return a && b; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), I);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_REDDITUS), I);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* nidificatio: a && (b || c) - tres terminales */
        f = _fluere(piscina,
            "int f7(int a, int b, int c) {"
            " if (a && (b || c)) { return 1; } return 0; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), III);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), III);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* negatio: !(a && b) - permutatio destinationum, forma
         * eadem, omnes bloci vivi */
        f = _fluere(piscina,
            "int f8(int a, int b) { if (!(a && b)) { return 0; }"
            " return 1; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), II);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));

        /* passus ansae per cum sectione: fissio in passus_b, margo
         * retro ex iunctione eius */
        f = _fluere(piscina,
            "int f9(int a, int b) { int i;"
            " for (i = 0; i < a; i = i + (a && b)) { b = b + 1; }"
            " return i; }", FALSUM);
        CREDO_NON_NIHIL (f);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_VERUS), II);
        CREDO_AEQUALIS_I32 (_margines_generis(f,
            (s32)FLUXUS_MARGO_FALSUS), II);
        CREDO_FALSUM (f->cadit_attingibilis);
        CREDO_AEQUALIS_I32 (_bloci_attingibiles(f),
            xar_numerus(f->bloci));
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
