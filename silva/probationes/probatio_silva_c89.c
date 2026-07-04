/* probatio_silva_c89.c - Grammatica C89, M2a Chunks A+B
 *
 * A: scala praecedentiae integra. B: typus intrat - conversio,
 * magnitudo-expressionis/-typi, species-typi minima, et FAMILIA
 * FURCARUM typedef: (t)(x), (t)+x -x *x &x, magnitudo(t). Tota
 * familia in UNA cella r/r vivit (typus-nominatus vs primarium-
 * identificator post IDENTIFICATOR intra parenthesim, prospectus
 * STAR/PAREN_CLAUSA) - census EXACTE fixus infra per paria id.
 *
 * SPINA CANONICA (politica M2a, tota programmate): lectio
 * EXPRESSIONIS canonica est ubi oraculum nomen nescit; oraculum
 * ad lectionem TYPI per recanonicare vertit. Fixum in probatione
 * furcarum - si ordo motoris umquam mutatur, probatio frangit et
 * politica EXPLICITE imponenda est, non re-figenda.
 *
 * Vocabularium: grammatica/genera-c89.md (DECISUS 2026-07-04).
 * Oraculum implacabile: OMNIS fixtura per arborem octetim exacta
 * redit (silva_scribere_fontem), etiam segmenta ERROR.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024
#define LINEA_MAXIMA 4096

hic_manens constans SilvaGrammatica GRAMMATICA_C89 = {
    &SILVA_C89_TABULA,
    &SILVA_C89_REGISTRUM,
    silva_c89_construere,
    silva_c89_ambiguum_fabricare
};

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), &GRAMMATICA_C89, NIHIL, NIHIL, NIHIL);
}

interior SilvaNodus*
_elementum (SilvaValor lista, i32 index)
{
    SilvaValor* elem;

    elem = silva_valor_lista_obtinere(lista, index);
    si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde elem->datum.nodus;
}

/* Primum elementum radicis - sententia unica */
interior SilvaNodus*
_sententia_prima (SilvaParsura* parsura)
{
    redde _elementum(parsura->commissio->radix, ZEPHYRUM);
}

/* Expressio sub sententia-expressionis prima */
interior SilvaNodus*
_expressio_prima (SilvaParsura* parsura)
{
    SilvaNodus* sententia = _sententia_prima(parsura);
    SilvaValor  expressio;

    si (sententia == NIHIL
        || sententia->genus != (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS)
    {
        redde NIHIL;
    }
    expressio = silva_c89_sententia_expressionis_expressio(sententia);
    si (expressio.genus != SILVA_VALOR_NODUS) redde NIHIL;
    redde expressio.datum.nodus;
}

interior SilvaNodus*
_nodus_valoris (SilvaValor valor)
{
    si (valor.genus != SILVA_VALOR_NODUS) redde NIHIL;
    redde valor.datum.nodus;
}

/* Lexema tituli typi nominati intra interpretationem conversionis
 * vel magnitudinis-typi; NIHIL si lectio typi non est vel typus
 * primitivus est */
interior SilvaToken*
_titulus_typi (constans SilvaNodus* interp)
{
    SilvaValor typus;
    SilvaValor specificatores;
    SilvaValor* primum;
    SilvaValor titulus;

    si (interp == NIHIL) redde NIHIL;
    si (interp->genus == (s32)SILVA_C89_GENUS_CONVERSIO)
    {
        typus = silva_c89_conversio_typus(interp);
    }
    alioquin si (interp->genus == (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI)
    {
        typus = silva_c89_magnitudo_typi_typus(interp);
    }
    alioquin
    {
        redde NIHIL;
    }
    si (typus.genus != SILVA_VALOR_NODUS) redde NIHIL;
    specificatores = silva_c89_species_typi_specificatores(
        typus.datum.nodus);
    primum = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
    si (primum == NIHIL || primum->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    si (primum->datum.nodus->genus
        != (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS)
    {
        redde NIHIL;
    }
    titulus = silva_c89_typus_nominatus_tok_titulus(
        primum->datum.nodus);
    si (titulus.genus != SILVA_VALOR_TOKEN) redde NIHIL;
    redde titulus.datum.token;
}

/* Resolutor c89: victoria lectioni TYPI si oraculum titulum novit
 * (imago resolutoris veri M2b; sine scientia AMBIGUUS superstes -
 * pin quaestionum joculatoriarum id vult) */
interior vacuum
_resolutor_c89 (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interpretationes =
        silva_c89_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)contextus;
    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(
            interpretationes, i);
        SilvaToken* titulus;

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS) perge;
        titulus = _titulus_typi(elem->datum.nodus);
        si (titulus != NIHIL
            && silva_oraculum_typum_novit(oraculum, titulus->valor))
        {
            responsum->victor = (s32)i;
            responsum->discriminans = titulus;
            redde;
        }
    }
}

/* Resolutor contrarius (probatio recanonicationis utroque sensu):
 * victoria lectioni EXPRESSIONIS (non-typus) */
interior vacuum
_resolutor_expressionis (
    constans SilvaNodus*     ambiguum,
    constans SilvaOraculum*  oraculum,
    vacuum*                  contextus,
    SilvaResolutioResponsum* responsum)
{
    SilvaValor interpretationes =
        silva_c89_ambiguus_interpretationes(ambiguum);
    i32 i;

    (vacuum)oraculum;
    (vacuum)contextus;
    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(
            interpretationes, i);

        si (elem == NIHIL || elem->genus != SILVA_VALOR_NODUS) perge;
        si (_titulus_typi(elem->datum.nodus) == NIHIL)
        {
            responsum->victor = (s32)i;
            redde;
        }
    }
}

/* Index interpretationis generis dati intra nodum ambiguum;
 * -1 si absens */
interior s32
_index_generis (constans SilvaNodus* ambiguum, s32 genus)
{
    SilvaValor interpretationes =
        silva_c89_ambiguus_interpretationes(ambiguum);
    i32 i;

    per (i = ZEPHYRUM;
         i < silva_valor_lista_numerus(interpretationes); i++)
    {
        SilvaValor* elem = silva_valor_lista_obtinere(
            interpretationes, i);

        si (elem != NIHIL && elem->genus == SILVA_VALOR_NODUS
            && elem->datum.nodus->genus == genus)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_c89",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: census conflictuum == FAMILIA NOMINATA, EXACTE
     * (M2a Chunk B INTENTIO). Duae cellae r/r in UNO statu:
     * typus-nominatus vs primarium-identificator, prospectus
     * STAR et PAREN_CLAUSA. Omnis cella alia = frange.
     * ======================================================== */

    {
        constans SilvaTabulaCocta* tabula = &SILVA_C89_TABULA;
        s32 prod_tn = -I;
        s32 prod_pi = -I;
        s32 idx_star = -I;
        s32 idx_clausa = -I;
        i32 cellae = ZEPHYRUM;
        i32 cellae_nominatae = ZEPHYRUM;
        i32 s;
        i32 i;
        i32 j;

        imprimere("\n--- Probans censum conflictuum (familia) ---\n");

        CREDO_AEQUALIS_S32 (tabula->numerus_conflictuum, II);

        per (i = ZEPHYRUM; i < tabula->numerus_productionum; i++)
        {
            si (tabula->productiones[i].id == NIHIL) perge;
            si (strcmp(tabula->productiones[i].id,
                    "typus-nominatus") == ZEPHYRUM)
            {
                prod_tn = (s32)i;
            }
            si (strcmp(tabula->productiones[i].id,
                    "primarium-identificator") == ZEPHYRUM)
            {
                prod_pi = (s32)i;
            }
        }
        per (i = ZEPHYRUM; i < tabula->numerus_terminalium; i++)
        {
            si (strcmp(tabula->symbola[i].titulus, "STAR") == ZEPHYRUM)
            {
                idx_star = (s32)i;
            }
            si (strcmp(tabula->symbola[i].titulus,
                    "PAREN_CLAUSA") == ZEPHYRUM)
            {
                idx_clausa = (s32)i;
            }
        }
        CREDO_VERUM (prod_tn >= ZEPHYRUM && prod_pi >= ZEPHYRUM);
        CREDO_VERUM (idx_star >= ZEPHYRUM && idx_clausa >= ZEPHYRUM);

        /* Scansio exhaustiva: omnis cella multi-actionis */
        per (s = ZEPHYRUM; s < tabula->numerus_statuum; s++)
        {
            i32 initium = tabula->status[s].actiones_offset;
            i32 finis = initium + tabula->status[s].actiones_numerus;

            per (i = initium; i < finis; i++)
            {
                b32 prior_par = FALSUM;
                i32 pares = I;
                b32 ambo_recte = VERUM;

                per (j = initium; j < i; j++)
                {
                    si (tabula->actiones[j].terminalis
                        == tabula->actiones[i].terminalis)
                    {
                        prior_par = VERUM;
                    }
                }
                si (prior_par) perge;  /* cella iam numerata */

                per (j = i + I; j < finis; j++)
                {
                    si (tabula->actiones[j].terminalis
                        != tabula->actiones[i].terminalis)
                    {
                        perge;
                    }
                    pares++;
                    si (tabula->actiones[j].actio
                            != (s32)SILVA_TAB_ACTIO_REDUCERE
                        || tabula->actiones[i].actio
                            != (s32)SILVA_TAB_ACTIO_REDUCERE)
                    {
                        ambo_recte = FALSUM;
                    }
                }
                si (pares < II) perge;

                cellae++;
                /* cella nominata: r/r inter prod_tn et prod_pi,
                 * prospectus STAR vel PAREN_CLAUSA */
                si (pares == II && ambo_recte
                    && (tabula->actiones[i].terminalis == idx_star
                        || tabula->actiones[i].terminalis == idx_clausa))
                {
                    b32 tn_adest = FALSUM;
                    b32 pi_adest = FALSUM;

                    per (j = i; j < finis; j++)
                    {
                        si (tabula->actiones[j].terminalis
                            != tabula->actiones[i].terminalis)
                        {
                            perge;
                        }
                        si (tabula->actiones[j].valor == prod_tn)
                        {
                            tn_adest = VERUM;
                        }
                        si (tabula->actiones[j].valor == prod_pi)
                        {
                            pi_adest = VERUM;
                        }
                    }
                    si (tn_adest && pi_adest)
                    {
                        cellae_nominatae++;
                    }
                }
            }
        }
        CREDO_AEQUALIS_I32 (cellae, II);
        CREDO_AEQUALIS_I32 (cellae_nominatae, II);
    }


    /* ========================================================
     * PROBARE: folia
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* folium;

        imprimere("\n--- Probans folia ---\n");

        parsura = _parsare(piscina, "x;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        folium = _expressio_prima(parsura);
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_folium_identificator_tok_valor(folium)
                .datum.token->valor, "x");

        folium = _expressio_prima(_parsare(piscina, "42;"));
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_INTEGER);

        folium = _expressio_prima(_parsare(piscina, "3.14;"));
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS);

        folium = _expressio_prima(_parsare(piscina, "'a';"));
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER);
    }


    /* ========================================================
     * PROBARE: folium-chorda - chordae adiacentes UNUM folium,
     * tok_valor = lista lexematum (decisio M2a ex vocabulario)
     * ======================================================== */

    {
        SilvaNodus* folium;
        SilvaValor  valores;

        imprimere("\n--- Probans folium-chorda ---\n");

        folium = _expressio_prima(_parsare(piscina, "\"salve\";"));
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_CHORDA);
        valores = silva_c89_folium_chorda_tok_valor(folium);
        CREDO_AEQUALIS_S32 ((s32)valores.genus,
            (s32)SILVA_VALOR_LISTA);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(valores), I);

        /* "a" "b" adiacentes -> UNUM folium, lista II */
        folium = _expressio_prima(_parsare(piscina, "\"a\" \"b\";"));
        CREDO_AEQUALIS_S32 (folium->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_CHORDA);
        valores = silva_c89_folium_chorda_tok_valor(folium);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(valores), II);
        {
            SilvaValor* secundum = silva_valor_lista_obtinere(valores, I);
            CREDO_AEQUALIS_S32 ((s32)secundum->genus,
                (s32)SILVA_VALOR_TOKEN);
            CREDO_CHORDA_AEQUALIS_LITERIS (secundum->datum.token->valor,
                "\"b\"");
        }
    }


    /* ========================================================
     * PROBARE: praecedentia et associativitas
     * ======================================================== */

    {
        SilvaNodus* radix;
        SilvaNodus* internus;

        imprimere("\n--- Probans praecedentiam ---\n");

        /* 2+3*4 -> +(2, *(3,4)) */
        radix = _expressio_prima(_parsare(piscina, "2+3*4;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_BINARIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_binarium_tok_operator(radix).datum.token->valor,
            "+");
        internus = _nodus_valoris(silva_c89_binarium_dexter(radix));
        CREDO_AEQUALIS_S32 (internus->genus,
            (s32)SILVA_C89_GENUS_BINARIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_binarium_tok_operator(internus).datum.token->valor,
            "*");

        /* 1-2-3 -> -(-(1,2), 3): sinistro-associativa */
        radix = _expressio_prima(_parsare(piscina, "1-2-3;"));
        internus = _nodus_valoris(silva_c89_binarium_sinister(radix));
        CREDO_AEQUALIS_S32 (internus->genus,
            (s32)SILVA_C89_GENUS_BINARIUM);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_binarium_dexter(radix))->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_INTEGER);

        /* a=b=c -> =(a, =(b,c)): dextro-associativa */
        radix = _expressio_prima(_parsare(piscina, "a=b=c;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_ASSIGNATIO);
        internus = _nodus_valoris(silva_c89_assignatio_dexter(radix));
        CREDO_AEQUALIS_S32 (internus->genus,
            (s32)SILVA_C89_GENUS_ASSIGNATIO);

        /* a-=b: operator compositus in lexemate */
        radix = _expressio_prima(_parsare(piscina, "a-=b;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_ASSIGNATIO);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_assignatio_tok_operator(radix).datum.token->valor,
            "-=");

        /* a,b,c -> virgula(virgula(a,b), c) */
        radix = _expressio_prima(_parsare(piscina, "a,b,c;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_VIRGULA);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_virgula_sinister(radix))->genus,
            (s32)SILVA_C89_GENUS_VIRGULA);

        /* catena profunda: scala tota in una expressione */
        radix = _expressio_prima(_parsare(piscina,
            "a||b&&c|d^e&f==g<h<<i+j*k;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_BINARIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_binarium_tok_operator(radix).datum.token->valor,
            "||");
    }


    /* ========================================================
     * PROBARE: ternarius
     * ======================================================== */

    {
        SilvaNodus* radix;

        imprimere("\n--- Probans ternarium ---\n");

        radix = _expressio_prima(_parsare(piscina, "a?b:c;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_TERNARIUS);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_ternarius_conditio(radix))->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_folium_identificator_tok_valor(
                _nodus_valoris(silva_c89_ternarius_falsum(radix)))
                .datum.token->valor, "c");
    }


    /* ========================================================
     * PROBARE: unarium et postcrementum
     * ======================================================== */

    {
        SilvaNodus* radix;
        SilvaNodus* internus;

        imprimere("\n--- Probans unarium ---\n");

        radix = _expressio_prima(_parsare(piscina, "-x;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_UNARIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_unarium_tok_operator(radix).datum.token->valor,
            "-");

        /* !*p -> !(*(p)): unaria nidificant */
        radix = _expressio_prima(_parsare(piscina, "!*p;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_UNARIUM);
        internus = _nodus_valoris(silva_c89_unarium_internum(radix));
        CREDO_AEQUALIS_S32 (internus->genus,
            (s32)SILVA_C89_GENUS_UNARIUM);

        radix = _expressio_prima(_parsare(piscina, "++i;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_UNARIUM);

        /* x++ -> postcrementum */
        radix = _expressio_prima(_parsare(piscina, "x++;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_POSTCREMENTUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_postcrementum_tok_operator(radix)
                .datum.token->valor, "++");
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_postcrementum_internum(radix))
                ->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: postfixa - accessus, subscriptio, vocatio
     * ======================================================== */

    {
        SilvaNodus* radix;
        SilvaValor  argumenta;

        imprimere("\n--- Probans postfixa ---\n");

        radix = _expressio_prima(_parsare(piscina, "s.x;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_ACCESSUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_accessus_tok_operator(radix).datum.token->valor,
            ".");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_accessus_tok_titulus(radix).datum.token->valor,
            "x");

        radix = _expressio_prima(_parsare(piscina, "p->y;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_ACCESSUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_accessus_tok_operator(radix).datum.token->valor,
            "->");

        radix = _expressio_prima(_parsare(piscina, "a[i];"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_SUBSCRIPTIO);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_subscriptio_index(radix))->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR);

        /* f(): argumenta lista VACUA */
        radix = _expressio_prima(_parsare(piscina, "f();"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_VOCATIO);
        argumenta = silva_c89_vocatio_argumenta(radix);
        CREDO_AEQUALIS_S32 ((s32)argumenta.genus,
            (s32)SILVA_VALOR_LISTA);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(argumenta),
            ZEPHYRUM);

        /* f(a,b): lista separata - signum COMMA IN lista intersertum
         * (ordo locorum == ordo octetorum) */
        radix = _expressio_prima(_parsare(piscina, "f(a,b);"));
        argumenta = silva_c89_vocatio_argumenta(radix);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(argumenta), III);
        {
            SilvaValor* medius = silva_valor_lista_obtinere(argumenta, I);
            CREDO_AEQUALIS_S32 ((s32)medius->genus,
                (s32)SILVA_VALOR_TOKEN);
            CREDO_AEQUALIS_S32 ((s32)medius->datum.token->genus,
                (s32)SILVA_LEX_COMMA);
        }

        /* vocatio in catena: a->b.c[i]() - postfixa componuntur */
        radix = _expressio_prima(_parsare(piscina, "a->b.c[i]();"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_VOCATIO);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_vocatio_functio(radix))->genus,
            (s32)SILVA_C89_GENUS_SUBSCRIPTIO);
    }


    /* ========================================================
     * PROBARE: parenthesis
     * ======================================================== */

    {
        SilvaNodus* radix;
        SilvaNodus* sinister;

        imprimere("\n--- Probans parenthesim ---\n");

        /* (a+b)*c: parenthesis praecedentiam vertit */
        radix = _expressio_prima(_parsare(piscina, "(a+b)*c;"));
        CREDO_AEQUALIS_S32 (radix->genus, (s32)SILVA_C89_GENUS_BINARIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_binarium_tok_operator(radix).datum.token->valor,
            "*");
        sinister = _nodus_valoris(silva_c89_binarium_sinister(radix));
        CREDO_AEQUALIS_S32 (sinister->genus,
            (s32)SILVA_C89_GENUS_PARENTHESIS);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_parenthesis_internum(sinister))
                ->genus,
            (s32)SILVA_C89_GENUS_BINARIUM);
    }


    /* ========================================================
     * PROBARE: honestas M2a - typi ignoti = ERROR, numquam fragor
     * ======================================================== */

    {
        SilvaParsura* parsura;

        imprimere("\n--- Probans honestatem (typi Chunk B) ---\n");

        /* "int" terminale ignotum grammaticae M2a -> segmentum ERROR */
        parsura = _parsare(piscina, "int x;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_ERROR);

        /* sententiae plures */
        parsura = _parsare(piscina, "a; b;");
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), II);

        /* fons vacuus */
        parsura = _parsare(piscina, "");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: conversio et species-typi (Chunk B)
     * ======================================================== */

    {
        SilvaNodus* radix;
        SilvaNodus* typus;
        SilvaNodus* declarator;
        SilvaValor  specificatores;
        SilvaValor  verba;
        SilvaValor* elem;

        imprimere("\n--- Probans conversionem ---\n");

        /* (int)x: conversio, typus primitivus, declarator NIHIL */
        radix = _expressio_prima(_parsare(piscina, "(int)x;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);
        typus = _nodus_valoris(silva_c89_conversio_typus(radix));
        CREDO_AEQUALIS_S32 (typus->genus,
            (s32)SILVA_C89_GENUS_SPECIES_TYPI);
        CREDO_AEQUALIS_S32 ((s32)silva_c89_species_typi_declarator(
            typus).genus, (s32)SILVA_VALOR_NIHIL);
        specificatores =
            silva_c89_species_typi_specificatores(typus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            specificatores), I);

        /* (unsigned long)x: UNUM typus-primitivus, verba II */
        radix = _expressio_prima(_parsare(piscina,
            "(unsigned long)x;"));
        typus = _nodus_valoris(silva_c89_conversio_typus(radix));
        specificatores =
            silva_c89_species_typi_specificatores(typus);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS);
        verba = silva_c89_typus_primitivus_tok_verba(
            elem->datum.nodus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(verba), II);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_valor_lista_obtinere(verba, I)->datum.token->valor,
            "long");

        /* (char**)p: catena stellarum abstracta */
        radix = _expressio_prima(_parsare(piscina, "(char**)p;"));
        typus = _nodus_valoris(silva_c89_conversio_typus(radix));
        declarator = _nodus_valoris(
            silva_c89_species_typi_declarator(typus));
        CREDO_AEQUALIS_S32 (declarator->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_declarator_abstractus_internum(
                declarator))->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS);

        /* (foo)~x: lectio expressionis moritur - conversio SINE
         * ambiguitate, typus nominatus */
        {
            SilvaParsura* parsura = _parsare(piscina, "(foo)~x;");

            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            radix = _expressio_prima(parsura);
            CREDO_AEQUALIS_S32 (radix->genus,
                (s32)SILVA_C89_GENUS_CONVERSIO);
            CREDO_CHORDA_AEQUALIS_LITERIS (
                _titulus_typi(radix)->valor, "foo");
        }

        /* (int)(char)x: conversiones nidificant */
        radix = _expressio_prima(_parsare(piscina, "(int)(char)x;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_conversio_internum(radix))->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);
    }


    /* ========================================================
     * PROBARE: magnitudo - duo genera (vocabularium M2.0)
     * ======================================================== */

    {
        SilvaNodus* radix;

        imprimere("\n--- Probans magnitudinem ---\n");

        radix = _expressio_prima(_parsare(piscina, "sizeof x;"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS);

        radix = _expressio_prima(_parsare(piscina, "sizeof(int);"));
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI);

        /* sizeof(foo*): lectio expressionis moritur ad ')' */
        {
            SilvaParsura* parsura = _parsare(piscina, "sizeof(foo*);");

            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            radix = _expressio_prima(parsura);
            CREDO_AEQUALIS_S32 (radix->genus,
                (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI);
        }
    }


    /* ========================================================
     * PROBARE: FAMILIA FURCARUM - ambae lectiones retentae,
     * spina canonica = lectio expressionis (politica fixa)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* ambiguum;
        s32 index_vocationis;
        s32 index_conversionis;
        SilvaValor canonica;

        imprimere("\n--- Probans familiam furcarum ---\n");

        /* (foo)(x): conversio-vel-vocatio */
        parsura = _parsare(piscina, "(foo)(x);");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        ambiguum = _expressio_prima(parsura);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_ambiguus_interpretationes(ambiguum)), II);
        index_vocationis = _index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_VOCATIO);
        index_conversionis = _index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_CONVERSIO);
        CREDO_VERUM (index_vocationis >= ZEPHYRUM);
        CREDO_VERUM (index_conversionis >= ZEPHYRUM);

        /* POLITICA: spina canonica = lectio EXPRESSIONIS. Hodie
         * ordo motoris eam gratis dat; si haec assertio frangit,
         * politicam explicite imponere (gubernator/recanonicare),
         * numquam re-figere. */
        canonica = silva_c89_ambiguus_canonica(ambiguum);
        CREDO_AEQUALIS_S32 (canonica.datum.index, index_vocationis);

        /* familia conversio-vel-binarium: + - * & ambigua... */
        {
            hic_manens constans character* AMBIGUA[] = {
                "(foo)+x;", "(foo)-x;", "(foo)*x;", "(foo)&x;"
            };
            i32 i;

            per (i = ZEPHYRUM; i < IV; i++)
            {
                parsura = _parsare(piscina, AMBIGUA[i]);
                CREDO_AEQUALIS_I32 (xar_numerus(
                    parsura->commissio->ambigui), I);
            }
        }
        /* ...~ ! non ambigua (operatores solum unarii) */
        parsura = _parsare(piscina, "(foo)!x;");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);

        /* MORES MOTORIS OBSERVATI (Chunk C, non politica): furca
         * quae operatorem sequentem transit BIS compingitur -
         * rami in statibus diversis post ')' vivunt donec ad
         * gradum additivum reducantur, ergo compactio interior
         * (span furcae) ET exterior (binarium) - ambigui == 2,
         * lectio nulla amissa, octeti exacti. Sedes nominata: si
         * custodes furcarum M2b (frons_maxima in capitibus densis
         * typedef) numerationem inflatam ostendunt, compactio
         * redundans evidentia refinitionem motoris meret. */
        parsura = _parsare(piscina, "(foo)(x) + sizeof(int);");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            II);

        /* magnitudo(foo): typi-vel-expressionis. INVENTUM Chunk B:
         * ordo motoris NON est politica - huic furcae spina nativa
         * = lectio TYPI (contra (foo)(x) supra). Politica
         * (expressionis canonica) mechanismo sancito imponitur:
         * recanonicare post commissionem cum resolutore politicae
         * - involucrum manet, ambae lectiones vivunt, canonica
         * vertitur. Sedes vera huius vocationis = INTENTIO M2b
         * (integratio gubernatoris cum oraculo). */
        parsura = _parsare(piscina, "sizeof(foo);");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        ambiguum = _expressio_prima(parsura);
        CREDO_VERUM (_index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI) >= ZEPHYRUM);

        /* oraculum vacuum: recanonicare oraculum non-NIHIL poscit;
         * resolutor politicae eo non utitur */
        silva_recanonicare(parsura->commissio,
            silva_oraculum_creare(piscina),
            _resolutor_expressionis, NIHIL);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);  /* ambae lectiones vivunt */
        canonica = silva_c89_ambiguus_canonica(ambiguum);
        CREDO_AEQUALIS_S32 (canonica.datum.index,
            _index_generis(ambiguum,
                (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS));
    }


    /* ========================================================
     * PROBARE: oraculum vertit - collapsus in fistula + '
     * recanonicare UTROQUE SENSU (involucrum manet)
     * ======================================================== */

    {
        SilvaOraculum* oraculum;
        SilvaParsura* parsura;
        SilvaNodus* radix;
        SilvaNodus* ambiguum;
        s32 index_conversionis;
        s32 index_vocationis;

        imprimere("\n--- Probans oraculum et recanonicare ---\n");

        oraculum = silva_oraculum_creare(piscina);
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "foo"));

        /* Cum oraculo in fistula: COLLAPSUS ad conversionem */
        parsura = silva_parsare(piscina, "probatio.c", "(foo)(x);",
            IX, &GRAMMATICA_C89, oraculum, _resolutor_c89, NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->resolutiones), I);
        radix = _expressio_prima(parsura);
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);

        /* Et magnitudo: */
        parsura = silva_parsare(piscina, "probatio.c", "sizeof(foo);",
            XII, &GRAMMATICA_C89, oraculum, _resolutor_c89, NIHIL);
        CREDO_AEQUALIS_S32 (_expressio_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI);

        /* Sine oraculo, deinde RECANONICARE: scientia sera indicem
         * canonicum vertit IN LOCO - involucrum manet, nulla
         * reparsura */
        parsura = _parsare(piscina, "(foo)(x);");
        ambiguum = _expressio_prima(parsura);
        index_conversionis = _index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_CONVERSIO);
        index_vocationis = _index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_VOCATIO);
        CREDO_AEQUALIS_S32 (
            silva_c89_ambiguus_canonica(ambiguum).datum.index,
            index_vocationis);

        silva_recanonicare(parsura->commissio, oraculum,
            _resolutor_c89, NIHIL);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);  /* involucrum manet */
        CREDO_AEQUALIS_S32 (
            silva_c89_ambiguus_canonica(ambiguum).datum.index,
            index_conversionis);

        /* ...et retro (uterque sensus, INTENTIO M2a) */
        silva_recanonicare(parsura->commissio, oraculum,
            _resolutor_expressionis, NIHIL);
        CREDO_AEQUALIS_S32 (
            silva_c89_ambiguus_canonica(ambiguum).datum.index,
            index_vocationis);
    }


    /* ========================================================
     * PROBARE: vectis lapifex portata (Chunk C) - omnes 32
     * inputus probationis probatio_lapifex_c89_expr.c contra
     * genera silvae re-asserti (semantica portatur, non forma;
     * generatio prior AST arbor2 verificabat, nos genera DECISA)
     * ======================================================== */

    {
        nomen structura {
            constans character* fons;
            s32                 genus;
        } VectisPortata;
        hic_manens constans VectisPortata VECTIS[] = {
            { "x;",      (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR },
            { "42;",     (s32)SILVA_C89_GENUS_FOLIUM_INTEGER },
            { "3.14;",   (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS },
            { "'a';",    (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER },
            { "\"hello\";", (s32)SILVA_C89_GENUS_FOLIUM_CHORDA },
            { "2+3;",    (s32)SILVA_C89_GENUS_BINARIUM },
            { "2+3*4;",  (s32)SILVA_C89_GENUS_BINARIUM },
            { "1-2-3;",  (s32)SILVA_C89_GENUS_BINARIUM },
            { "a=b=c;",  (s32)SILVA_C89_GENUS_ASSIGNATIO },
            { "-x;",     (s32)SILVA_C89_GENUS_UNARIUM },
            { "!flag;",  (s32)SILVA_C89_GENUS_UNARIUM },
            { "&var;",   (s32)SILVA_C89_GENUS_UNARIUM },
            { "*ptr;",   (s32)SILVA_C89_GENUS_UNARIUM },
            { "++i;",    (s32)SILVA_C89_GENUS_UNARIUM },
            { "~mask;",  (s32)SILVA_C89_GENUS_UNARIUM },
            { "x++;",    (s32)SILVA_C89_GENUS_POSTCREMENTUM },
            { "p--;",    (s32)SILVA_C89_GENUS_POSTCREMENTUM },
            { "s.x;",    (s32)SILVA_C89_GENUS_ACCESSUS },
            { "p->y;",   (s32)SILVA_C89_GENUS_ACCESSUS },
            { "a[i];",   (s32)SILVA_C89_GENUS_SUBSCRIPTIO },
            { "f();",    (s32)SILVA_C89_GENUS_VOCATIO },
            { "f(a,b);", (s32)SILVA_C89_GENUS_VOCATIO },
            { "(a+b)*c;", (s32)SILVA_C89_GENUS_BINARIUM },
            { "a?b:c;",  (s32)SILVA_C89_GENUS_TERNARIUS },
            { "a,b,c;",  (s32)SILVA_C89_GENUS_VIRGULA },
            { "(int)x;", (s32)SILVA_C89_GENUS_CONVERSIO },
            { "(char*)p;", (s32)SILVA_C89_GENUS_CONVERSIO },
            { "sizeof x;",
                (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS },
            { "sizeof(int);", (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI },
            { "a->b.c[i]();", (s32)SILVA_C89_GENUS_VOCATIO },
            { "a||b&&c|d^e&f==g<h<<i+j*k;",
                (s32)SILVA_C89_GENUS_BINARIUM },
            { "*p++ + sizeof(int*);", (s32)SILVA_C89_GENUS_BINARIUM }
        };
        i32 numerus = (i32)(magnitudo(VECTIS)
            / magnitudo(VECTIS[ZEPHYRUM]));
        i32 i;

        imprimere("\n--- Probans vectem lapifex portatam (%d) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaParsura* parsura = _parsare(piscina, VECTIS[i].fons);
            SilvaNodus* radix;

            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            radix = _expressio_prima(parsura);
            CREDO_NON_NIHIL (radix);
            si (radix != NIHIL)
            {
                CREDO_AEQUALIS_S32 (radix->genus, VECTIS[i].genus);
            }
        }
    }


    /* ========================================================
     * PROBARE: corpus syntaxis v1 (Chunk C). INVENTUM: corpus
     * paene totum DECLARATIONES est (expressiones intra
     * initiatores vivunt) - "dimidium expressionum" INTENTIONIS
     * vacuum erat; vectis expressionum M2a tota a lapifex fertur
     * (supra). Hoc metrum ergo ad ZEPHYRUM incipit et M2b/M2c
     * ascendere DEBENT (metrum M2d in miniatura; mutatio
     * deliberata, numquam tacita). Lineae = literae C citatae -
     * decodificatio \" et \\ ante parsationem.
     * ======================================================== */

    {
        constans character* radix_env;
        character via[VIA_MAXIMA];
        character linea[LINEA_MAXIMA];
        FILE* corpus;
        i32 lineae = ZEPHYRUM;
        i32 arbores = ZEPHYRUM;
        i32 sine_erroribus = ZEPHYRUM;

        imprimere("\n--- Probans corpus syntaxis v1 (c89) ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }
        sprintf(via, "%s/silva/probationes/fixa/syntaxis_v1_corpus.txt",
            radix_env);
        corpus = fopen(via, "rb");
        CREDO_NON_NIHIL (corpus);

        si (corpus != NIHIL)
        {
            dum (fgets(linea, LINEA_MAXIMA, corpus) != NIHIL)
            {
                Piscina* piscina_lineae;
                SilvaParsura* parsura;
                character decodata[LINEA_MAXIMA];
                memoriae_index m = strlen(linea);
                i32 d = ZEPHYRUM;
                i32 k;

                si (m == ZEPHYRUM || linea[ZEPHYRUM] == '#'
                    || linea[ZEPHYRUM] == '\n' || linea[ZEPHYRUM] != '"')
                {
                    perge;
                }
                lineae++;

                /* decodificare literam C: cita exteriora cadunt,
                 * \" et \\ solvuntur (corpus = "paste-ready") */
                per (k = I; k < (i32)m; k++)
                {
                    si (linea[k] == '\\'
                        && (linea[k + I] == '"' || linea[k + I] == '\\'))
                    {
                        decodata[d++] = linea[k + I];
                        k++;
                    }
                    alioquin si (linea[k] == '"')
                    {
                        frange;  /* cita clausurae */
                    }
                    alioquin
                    {
                        decodata[d++] = linea[k];
                    }
                }

                piscina_lineae = piscina_generare_dynamicum(
                    "corpus_lineae_c89", 4194304);
                si (piscina_lineae == NIHIL)
                {
                    perge;
                }
                parsura = silva_parsare(piscina_lineae, "corpus.c",
                    decodata, d, &GRAMMATICA_C89, NIHIL, NIHIL,
                    NIHIL);
                si (parsura != NIHIL && parsura->successus)
                {
                    arbores++;
                    si (parsura->numerus_errorum == ZEPHYRUM)
                    {
                        sine_erroribus++;
                    }
                }
                piscina_destruere(piscina_lineae);
            }
            fclose(corpus);
        }

        imprimere("  lineae: %d, arbores: %d, sine erroribus: %d\n",
            (int)lineae, (int)arbores, (int)sine_erroribus);
        CREDO_AEQUALIS_I32 (lineae, CXXV);
        CREDO_AEQUALIS_I32 (arbores, lineae);  /* totalitas */
        /* fundamentum comprehensionis M2a = 0 (corpus totum
         * declarationes; expressiones intra initiatores). M2b/M2c
         * hunc numerum ASCENDERE debent - ascensus assertione
         * deliberata, numquam tacite */
        CREDO_AEQUALIS_I32 (sine_erroribus, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: oraculum implacabile - OMNIS fixtura octetim
     * exacta per arborem (etiam ERROR, etiam trivia insolita)
     * ======================================================== */

    {
        hic_manens constans character* FIXTURAE[] = {
            "x;", "42;", "3.14;", "'a';", "\"salve\";",
            "\"a\" \"b\";",
            "2+3*4;", "1-2-3;", "a=b=c;", "a-=b;", "a?b:c;",
            "a,b,c;",
            "-x;", "!*p;", "++i;", "x++;", "p--;",
            "s.x;", "p->y;", "a[i];", "f();", "f(a,b);",
            "(a+b)*c;", "a->b.c[i]();",
            "a||b&&c|d^e&f==g<h<<i+j*k;",
            "a >>= b << 2;",
            "  x  +  y ;",
            "a;\nb;\n",
            "/* commentum */ x + 1;",
            "int x;",
            "f(a, g(b, c), \"x\" \"y\");",
            "(int)x;", "(unsigned long)x;", "(char**)p;",
            "(foo)~x;", "(foo)(x);", "(foo)+x;", "(foo)*x;",
            "(foo)&x;", "(foo)-x;",
            "sizeof x;", "sizeof(int);", "sizeof(foo);",
            "sizeof(foo*);", "(int)(char)x;",
            "sizeof x + 1;",
            "f((int)x, sizeof(long));",
            "*p++ + sizeof(int*);"
        };
        i32 i;
        i32 numerus = (i32)(magnitudo(FIXTURAE)
            / magnitudo(FIXTURAE[ZEPHYRUM]));

        imprimere("\n--- Probans oraculum (roundtrip %d fixturae) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaParsura* parsura;
            SilvaScriptura scriptura;

            parsura = _parsare(piscina, FIXTURAE[i]);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);

            scriptura = silva_scribere_fontem(piscina, parsura,
                &SILVA_C89_REGISTRUM, parsura->fons_princeps);
            CREDO_VERUM (scriptura.successus);
            CREDO_CHORDA_AEQUALIS_LITERIS (scriptura.textus,
                FIXTURAE[i]);
        }
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
