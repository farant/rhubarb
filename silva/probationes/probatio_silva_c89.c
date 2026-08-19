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
#include "silva_c89_oraculum.h"
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
    silva_c89_ambiguum_fabricare,
    NIHIL
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
     * PROBARE: census conflictuum == TRES FAMILIAE NOMINATAE,
     * EXACTE (M2b Chunk A). Omnes typedef-actae:
     *   F1 (4 cellae): typus-nominatus vs primarium-
     *      identificator - familia M2a, contextibus declarationis
     *      et parametrorum dilatata (STAR/PAREN_AP/PAREN_CL/QUADRA)
     *   F2 (3 cellae): typus-nominatus vs declarator-titulus -
     *      REGULA PARAMETRI ISO ("si nomen typi esse potest, est")
     *      ut furca GLR ("int f(foo)")
     *   F3 (7 cellae): transponere(IDENT ut specificator alter)
     *      vs reducere(lista specificatorum completa) - limes
     *      specificator/declarator ("unsigned foo x;"), ex
     *      tribus statibus listae x duabus formis (decl + membri/
     *      speciei: "struct S { foo bar; }")
     * Omnis cella extra familias = frange.
     * ======================================================== */

    {
        constans SilvaTabulaCocta* tabula = &SILVA_C89_TABULA;
        s32 prod_tn = -I;
        s32 prod_pi = -I;
        s32 prod_dt = -I;
        s32 sym_sd = -I;
        s32 sym_sq = -I;
        s32 sym_qual = -I;
        i32 f1 = ZEPHYRUM;
        i32 f2 = ZEPHYRUM;
        i32 f3 = ZEPHYRUM;
        i32 f4 = ZEPHYRUM;
        i32 f5 = ZEPHYRUM;
        i32 f6 = ZEPHYRUM;
        i32 ignotae = ZEPHYRUM;
        i32 s;
        i32 i;
        i32 j;

        imprimere("\n--- Probans censum conflictuum (familiae) ---\n");

        /* M2c Chunk A: XIV -> XVI (F1 +2, latitudo sententiarum;
         * alioquin pendens praelata - categoria sua infra; greges
         * et titulatum cellas NULLAS genuerunt).
         *
         * M2c Chunk B: XVI -> LII PARIA (44 cellae - cellae
         * ternis actionibus paria terna numerant). Fons: FOLLOW
         * declaratorum definitionibus K&R pollutum - post ")"
         * declarationes-kr sequi possunt, ergo omnes verba
         * specificatorum in prospectus reductionum declaratoris
         * intrant. EADEM ambiguitas subiacens (regula parametri
         * ISO), columnae plures:
         *   F2 (tn vs dt)          3 -> 23 cellae (litura K&R)
         *   F4 (tn vs dt vs pi)     4 cellae ternae - F1+F2 uno
         *   F5 (dt vs pi)           1 cella (QUADRA)
         *   F6 (trans vs qualificatores-reductio) 3 cellae -
         *      limes qualificatorum monstratoris, eadem litura
         * Tempus currendi: saltatio X10 lecturas invalidas
         * necat - "int add(int a, int b) { }" ambigui ZEPHYRUM
         * (fixtura infra). */
        CREDO_AEQUALIS_S32 (tabula->numerus_conflictuum, LII);

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
            si (strcmp(tabula->productiones[i].id,
                    "declarator-titulus") == ZEPHYRUM)
            {
                prod_dt = (s32)i;
            }
        }
        per (i = tabula->numerus_terminalium;
             i < tabula->numerus_symbolorum; i++)
        {
            si (strcmp(tabula->symbola[i].titulus,
                    "specificatores-decl") == ZEPHYRUM)
            {
                sym_sd = (s32)i;
            }
            si (strcmp(tabula->symbola[i].titulus,
                    "specificatores") == ZEPHYRUM)
            {
                sym_sq = (s32)i;
            }
            si (strcmp(tabula->symbola[i].titulus,
                    "qualificatores") == ZEPHYRUM)
            {
                sym_qual = (s32)i;
            }
        }
        CREDO_VERUM (prod_tn >= ZEPHYRUM && prod_pi >= ZEPHYRUM);
        CREDO_VERUM (prod_dt >= ZEPHYRUM && sym_sd >= ZEPHYRUM);
        CREDO_VERUM (sym_sq >= ZEPHYRUM && sym_qual >= ZEPHYRUM);

        per (s = ZEPHYRUM; s < tabula->numerus_statuum; s++)
        {
            i32 initium = tabula->status[s].actiones_offset;
            i32 finis = initium + tabula->status[s].actiones_numerus;

            per (i = initium; i < finis; i++)
            {
                b32 prior_par = FALSUM;
                i32 pares = I;
                b32 tn_adest = FALSUM;
                b32 pi_adest = FALSUM;
                b32 dt_adest = FALSUM;
                b32 transpositio_adest = FALSUM;
                b32 sd_reducitur = FALSUM;
                b32 qual_reducitur = FALSUM;

                per (j = initium; j < i; j++)
                {
                    si (tabula->actiones[j].terminalis
                        == tabula->actiones[i].terminalis)
                    {
                        prior_par = VERUM;
                    }
                }
                si (prior_par) perge;  /* cella iam numerata */

                per (j = i; j < finis; j++)
                {
                    constans SilvaTabActio* actio =
                        &tabula->actiones[j];

                    si (actio->terminalis
                        != tabula->actiones[i].terminalis)
                    {
                        perge;
                    }
                    si (j > i) pares++;
                    si (actio->actio == (s32)SILVA_TAB_ACTIO_TRANSPONERE)
                    {
                        transpositio_adest = VERUM;
                    }
                    alioquin si (actio->actio
                        == (s32)SILVA_TAB_ACTIO_REDUCERE)
                    {
                        si (actio->valor == prod_tn) tn_adest = VERUM;
                        si (actio->valor == prod_pi) pi_adest = VERUM;
                        si (actio->valor == prod_dt) dt_adest = VERUM;
                        si (tabula->productiones[actio->valor].sinistrum
                                == sym_sd
                            || tabula->productiones[actio->valor]
                                   .sinistrum == sym_sq)
                        {
                            sd_reducitur = VERUM;
                        }
                        si (tabula->productiones[actio->valor].sinistrum
                                == sym_qual)
                        {
                            qual_reducitur = VERUM;
                        }
                    }
                }
                si (pares < II) perge;

                si (pares == II && tn_adest && pi_adest)
                {
                    f1++;
                }
                alioquin si (pares == II && tn_adest && dt_adest)
                {
                    f2++;
                }
                alioquin si (pares == II && transpositio_adest
                    && sd_reducitur)
                {
                    f3++;
                }
                alioquin si (pares == III && tn_adest && dt_adest
                    && pi_adest)
                {
                    f4++;
                }
                alioquin si (pares == II && dt_adest && pi_adest)
                {
                    f5++;
                }
                alioquin si (pares == II && transpositio_adest
                    && qual_reducitur)
                {
                    f6++;
                }
                alioquin
                {
                    ignotae++;
                }
            }
        }
        CREDO_AEQUALIS_I32 (f1, VI);
        CREDO_AEQUALIS_I32 (f2, XXIII);
        CREDO_AEQUALIS_I32 (f3, VII);
        CREDO_AEQUALIS_I32 (f4, IV);
        CREDO_AEQUALIS_I32 (f5, I);
        CREDO_AEQUALIS_I32 (f6, III);
        CREDO_AEQUALIS_I32 (ignotae, ZEPHYRUM);

        /* CATEGORIA PRAELATA (M2c): cellae declarate resolutae
         * figuntur sicut cellae conflictuum - numerus ET
         * identitas (per titulum terminalis + id productionis,
         * non per numerum status - status cum grammatica
         * moventur). Una sola: alioquin pendens. */
        CREDO_AEQUALIS_I32 ((i32)SILVA_C89_NUMERUS_PRAELATARUM, I);
        {
            constans SilvaTabPraelata* cella = &SILVA_C89_PRAELATAE[0];

            CREDO_AEQUALIS_S32 (cella->actio_retenta,
                (s32)SILVA_TAB_ACTIO_TRANSPONERE);
            CREDO_VERUM (strcmp(
                tabula->symbola[cella->terminalis].titulus,
                "ELSE") == ZEPHYRUM);
            CREDO_VERUM (tabula->productiones[cella->productio_remota]
                .id != NIHIL);
            CREDO_VERUM (strcmp(
                tabula->productiones[cella->productio_remota].id,
                "si-sine-alioquin") == ZEPHYRUM);
        }
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

        imprimere("\n--- Probans honestatem (sententiae M2c) ---\n");

        /* M2b: "int x;" IAM declaratio est (ascensus honestus);
         * M2c Chunk A: sententiae regiminis IAM arbores sunt -
         * laqueus praemonitorius M2b hic ASCENDIT in probationem
         * positivam */
        parsura = _parsare(piscina, "int x;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);

        parsura = _parsare(piscina, "if (x) y;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_SI);

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
     * PROBARE: declarationes (M2b Chunk A)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* declaratio;
        SilvaNodus* nodus;
        SilvaValor  specificatores;
        SilvaValor  declaratores;
        SilvaValor* elem;

        imprimere("\n--- Probans declarationes ---\n");

        /* static const foo x = 5; - atoma mixta (lexemata nuda +
         * nodi), declarator initiatus */
        parsura = _parsare(piscina, "static const foo x = 5;");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        declaratio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            specificatores), III);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)elem->genus, (s32)SILVA_VALOR_TOKEN);
        elem = silva_valor_lista_obtinere(specificatores, II);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS);
        declaratores = silva_c89_declaratio_declaratores(declaratio);
        elem = silva_valor_lista_obtinere(declaratores, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS);

        /* unsigned long x; - cursus maximalis = UNUM
         * typus-primitivus (bistatualitas listarum) */
        declaratio = _sententia_prima(_parsare(piscina,
            "unsigned long x;"));
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            specificatores), I);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_typus_primitivus_tok_verba(elem->datum.nodus)),
            II);

        /* unsigned const long x; - qualificator cursum scindit:
         * DUO typus-primitivus (nota vocabularii) */
        declaratio = _sententia_prima(_parsare(piscina,
            "unsigned const long x;"));
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            specificatores), III);

        /* char *const p, **q; - monstrator cum qualificatoribus,
         * lista separata declaratorum */
        declaratio = _sententia_prima(_parsare(piscina,
            "char *const p, **q;"));
        declaratores = silva_c89_declaratio_declaratores(declaratio);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(declaratores),
            III);  /* p, COMMA, q */
        elem = silva_valor_lista_obtinere(declaratores, ZEPHYRUM);
        nodus = elem->datum.nodus;
        CREDO_AEQUALIS_S32 (nodus->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_declarator_monstrator_qualificatores(nodus)), I);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_declarator_monstrator_internum(
                nodus))->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS);

        /* int (*f)(int, char*); - monstrator functionis: functionis
         * { internum: parenthesis { monstrator { titulus f } },
         *   parametra }  */
        declaratio = _sententia_prima(_parsare(piscina,
            "int (*f)(int, char*);"));
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        declaratores = silva_c89_declaratio_declaratores(declaratio);
        elem = silva_valor_lista_obtinere(declaratores, ZEPHYRUM);
        nodus = elem->datum.nodus;
        CREDO_AEQUALIS_S32 (nodus->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_declarator_functionis_internum(
                nodus))->genus,
            (s32)SILVA_C89_GENUS_PARENTHESIS);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_declarator_functionis_parametra(nodus)), III);

        /* int a[10]; - acies cum mensura */
        declaratio = _sententia_prima(_parsare(piscina, "int a[10];"));
        declaratores = silva_c89_declaratio_declaratores(declaratio);
        elem = silva_valor_lista_obtinere(declaratores, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI);

        /* typedef int foo; - TYPEDEF lexema nudum in
         * specificatoribus (nota semantica M2.0; oraculum Chunk C
         * id hic detegit) */
        declaratio = _sententia_prima(_parsare(piscina,
            "typedef int foo;"));
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)elem->genus, (s32)SILVA_VALOR_TOKEN);
        CREDO_AEQUALIS_S32 ((s32)elem->datum.token->genus,
            (s32)SILVA_LEX_TYPEDEF);
    }


    /* ========================================================
     * PROBARE: aggregata (M2b Chunk B) - structura/unio/
     * enumeratio, membrum/campus, congeries
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* declaratio;
        SilvaNodus* nodus;
        SilvaValor  specificatores;
        SilvaValor  lista;
        SilvaValor* elem;

        imprimere("\n--- Probans aggregata ---\n");

        /* struct S { int x; char *p; } s; - plena cum
         * declaratore; membra II */
        parsura = _parsare(piscina,
            "struct S { int x; char *p; } s;");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        declaratio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        nodus = elem->datum.nodus;
        CREDO_AEQUALIS_S32 (nodus->genus,
            (s32)SILVA_C89_GENUS_STRUCTURA);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_c89_structura_tok_titulus(nodus).datum.token->valor,
            "S");
        lista = silva_c89_structura_membra(nodus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(lista), II);
        elem = silva_valor_lista_obtinere(lista, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_MEMBRUM);

        /* struct S { int x; }; - declaratio NUDA (tag-finalis) */
        parsura = _parsare(piscina, "struct S { int x; };");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        declaratio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (declaratio->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_declaratio_declaratores(declaratio)), ZEPHYRUM);

        /* int x; SINE ambiguo etiam nuda praesente (lectio
         * phantasma [int, nominatus(x)] statum tag-finalem non
         * habet - constructio, non filtrum) */
        parsura = _parsare(piscina, "int x;");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);

        /* campi bitorum. INVENTUM: membrum nominatum "T nomen : n"
         * IPSE furcat - lectio (a) [T] + campus(nomen, n),
         * lectio (b) [T, nominatus(nomen)] + campus SINE
         * declaratore (bitum anonymum)! Lectio (b) combinatione
         * invalida (typus + nominatus) - filtrum X10 Chunk C;
         * hic AMBIGUUS honeste retentus. Membrum anonymum
         * "int : 2;" contra UNICUM est. */
        parsura = _parsare(piscina,
            "struct F { unsigned a : 3; int : 2; };");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        declaratio = _sententia_prima(parsura);
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        lista = silva_c89_structura_membra(elem->datum.nodus);

        /* membra[0] = involucrum ambiguum; lectio campus-cum-
         * declaratore inter interpretationes adest */
        elem = silva_valor_lista_obtinere(lista, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);
        {
            SilvaValor interps = silva_c89_ambiguus_interpretationes(
                elem->datum.nodus);
            b32 campus_nominatus_adest = FALSUM;
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(interps); k++)
            {
                SilvaNodus* membrum = _nodus_valoris(
                    *silva_valor_lista_obtinere(interps, k));
                SilvaValor decll;
                SilvaNodus* campus_nodus;

                si (membrum == NIHIL
                    || membrum->genus != (s32)SILVA_C89_GENUS_MEMBRUM)
                {
                    perge;
                }
                decll = silva_c89_membrum_declaratores(membrum);
                campus_nodus = _nodus_valoris(
                    *silva_valor_lista_obtinere(decll, ZEPHYRUM));
                si (campus_nodus != NIHIL
                    && campus_nodus->genus
                        == (s32)SILVA_C89_GENUS_CAMPUS
                    && silva_c89_campus_declarator(campus_nodus)
                        .genus == SILVA_VALOR_NODUS)
                {
                    campus_nominatus_adest = VERUM;
                }
            }
            CREDO_VERUM (campus_nominatus_adest);
        }

        /* membra[1]: "int : 2;" - campus sine declaratore, UNICUS */
        elem = silva_valor_lista_obtinere(lista, I);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_MEMBRUM);
        {
            SilvaValor decll = silva_c89_membrum_declaratores(
                elem->datum.nodus);
            SilvaValor* campus_elem =
                silva_valor_lista_obtinere(decll, ZEPHYRUM);

            CREDO_AEQUALIS_S32 (campus_elem->datum.nodus->genus,
                (s32)SILVA_C89_GENUS_CAMPUS);
            CREDO_AEQUALIS_S32 ((s32)silva_c89_campus_declarator(
                campus_elem->datum.nodus).genus,
                (s32)SILVA_VALOR_NIHIL);
        }

        /* enum E { A, B = 2, C } e; */
        parsura = _parsare(piscina, "enum E { A, B = 2, C } e;");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        declaratio = _sententia_prima(parsura);
        specificatores =
            silva_c89_declaratio_specificatores(declaratio);
        elem = silva_valor_lista_obtinere(specificatores, ZEPHYRUM);
        nodus = elem->datum.nodus;
        CREDO_AEQUALIS_S32 (nodus->genus,
            (s32)SILVA_C89_GENUS_ENUMERATIO);
        lista = silva_c89_enumeratio_enumeratores(nodus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(lista), V);
        elem = silva_valor_lista_obtinere(lista, II);
        CREDO_AEQUALIS_S32 (elem->datum.nodus->genus,
            (s32)SILVA_C89_GENUS_ENUMERATOR);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_enumerator_valor(
                elem->datum.nodus))->genus,
            (s32)SILVA_C89_GENUS_FOLIUM_INTEGER);

        /* congeries: nidificata + virgula caudae */
        parsura = _parsare(piscina,
            "int m[2][2] = { { 1, 2 }, { 3, 4 }, };");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        declaratio = _sententia_prima(parsura);
        lista = silva_c89_declaratio_declaratores(declaratio);
        elem = silva_valor_lista_obtinere(lista, ZEPHYRUM);
        nodus = _nodus_valoris(silva_c89_declarator_initiatus_initiator(
            elem->datum.nodus));
        CREDO_AEQUALIS_S32 (nodus->genus,
            (s32)SILVA_C89_GENUS_CONGERIES);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(*silva_valor_lista_obtinere(
                silva_c89_congeries_elementa(nodus), ZEPHYRUM))->genus,
            (s32)SILVA_C89_GENUS_CONGERIES);

        /* (struct S*)p; - tag in conversione */
        parsura = _parsare(piscina, "(struct S*)p;");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_expressio_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);

        /* typedef struct S { int x; } S_t; - classicum */
        parsura = _parsare(piscina,
            "typedef struct S { int x; } S_t;");
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: FURCA SCELETI in gradu sententiae - "foo * bar;"
     * declaratio-vel-expressio (casus M1 = mechanismus generalis)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* ambiguum;

        imprimere("\n--- Probans furcam sceleti (declaratio) ---\n");

        parsura = _parsare(piscina, "foo * bar;");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        ambiguum = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (ambiguum->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);
        CREDO_VERUM (_index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_DECLARATIO) >= ZEPHYRUM);
        CREDO_VERUM (_index_generis(ambiguum,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS) >= ZEPHYRUM);

        /* int f(foo); - AMBIGUUM re vera: (1) functio f cum
         * parametro typi foo, (2) specificatores [int, f] +
         * declarator inclusus (foo) - "foo typi int-f"! Lectio 2
         * combinatione invalida est (int + nominatus componi non
         * possunt) - FILTRUM COMBINATIONIS eam necat, Chunk C
         * (inventum X10 simulationis, hic in carne). NB: ante
         * emendationem lista-valens haec furca FALSO 0 erat -
         * constructio fracta ramum tacite necabat; S32 clamavit,
         * emendatio lectionem legitimam resuscitavit. */
        parsura = _parsare(piscina, "int f(foo);");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* f(a)[3]; - declaratio ("acies a trium f") vel expressio
         * (vocatio + subscriptio): ambae verae lectiones C */
        parsura = _parsare(piscina, "f(a)[3];");
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
    }


    /* ========================================================
     * PROBARE: SALTATIO ORACULI (M2b Chunk C, simulatio X in
     * carne) - silva_c89_parsare: registratio praecommissionis +
     * resolutor verus (X6+X10) + oraculum positionale (X3) +
     * politica canonicae (X8), vocatione UNA
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus* elementum;

        imprimere("\n--- Probans saltationem oraculi ---\n");

        /* THE DANCE: typedef segmentum sequens alit */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int foo; foo * bar;", XXVII, NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->resolutiones), I);
        elementum = _elementum(parsura->commissio->radix, I);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);

        /* X3 POSITIONALE: typedef POST usum NON solvit -
         * ambiguitas prior honeste retenta */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "foo * bar; typedef int foo;", XXVII, NIHIL);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);

        /* X10 NECATIO DURA: int f(foo); - lectio phantasma
         * ([int, f-nominatus] + (foo)) combinatione cadit;
         * superstes unicus = functio, SINE oraculo */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "int f(foo);", XI, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->resolutiones), I);
        {
            SilvaValor decll;
            SilvaValor* delem;

            elementum = _sententia_prima(parsura);
            CREDO_AEQUALIS_S32 (elementum->genus,
                (s32)SILVA_C89_GENUS_DECLARATIO);
            decll = silva_c89_declaratio_declaratores(elementum);
            delem = silva_valor_lista_obtinere(decll, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (delem->datum.nodus->genus,
                (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS);
        }

        /* X10 in membris: campus nominatus sine phantasmate */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "struct F { unsigned a : 3; };", XXIX, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);

        /* politica per involucrum: ignotum retentum, canonica =
         * lectio expressionis (vocatio) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "(foo)(x);", IX, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);
        elementum = _expressio_prima(parsura);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_S32 (
            silva_c89_ambiguus_canonica(elementum).datum.index,
            _index_generis(elementum, (s32)SILVA_C89_GENUS_VOCATIO));

        /* ...et cum typedef: collapsus ad conversionem */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int foo; (foo)(x);", XXVI, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        elementum = _elementum(parsura->commissio->radix, I);
        CREDO_AEQUALIS_S32 (
            _nodus_valoris(silva_c89_sententia_expressionis_expressio(
                elementum))->genus,
            (s32)SILVA_C89_GENUS_CONVERSIO);

        /* saltatio per ramos conditionales (textura + registratio) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "#if 1\ntypedef int foo;\n#endif\nfoo * bar;", XL,
            NIHIL);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);

        /* segmentum ERROR typos non registrat (X9) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int foo @@ ; foo * bar;", XXXI, NIHIL);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            I);

        /* declaratores plures: typedef int a, *b; ambo saltant */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int a, *b; a * x; b * y;", XXXII, NIHIL);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->resolutiones), II);

        /* f(a)[3] cum f typo: collapsus ad DECLARATIONEM
         * ("acies a trium f" - C mirabile sed verum) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int f; f(a)[3];", XXIII, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(parsura->commissio->ambigui),
            ZEPHYRUM);
        elementum = _elementum(parsura->commissio->radix, I);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
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
     * PROBARE: vista declarationum (M2b Chunk D) - ordines TOC:
     * unus per declaratorem, nudae per tag, rami sumpti
     * visitantur, ambigua per spinam canonicam (hic: saltatio
     * furcam collapsit - bar ordo verus fit)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaDeclaratioVista vista;

        imprimere("\n--- Probans vistam declarationum ---\n");

        {
            hic_manens constans character* FONS_VISTAE =
                "typedef int foo;\n"
                "int a, *b;\n"
                "struct S { int x; };\n"
                "int (*f)(void);\n"
                "#if 1\n"
                "enum E { A } e;\n"
                "#endif\n"
                "foo * bar;\n";

            parsura = silva_c89_parsare(piscina, "probatio.c",
                FONS_VISTAE, (i32)strlen(FONS_VISTAE), NIHIL);
        }
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (
            silva_c89_declarationes_numerus(parsura), VII);

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, ZEPHYRUM,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "foo");
        CREDO_AEQUALIS_S32 (vista.linea, I);

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, II,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "b");
        CREDO_VERUM (strcmp(vista.genus,
            "declarator-monstrator") == ZEPHYRUM);

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, III,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "S");
        CREDO_VERUM (strcmp(vista.genus, "structura") == ZEPHYRUM);

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, IV,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "f");
        CREDO_VERUM (strcmp(vista.genus,
            "declarator-functionis") == ZEPHYRUM);

        /* ordo [5] e ramo sumpto; [6] ex ambiguo COLLAPSO
         * (saltatio: foo notum -> declaratio) */
        CREDO_VERUM (silva_c89_declaratio_vista(parsura, V,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "e");

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, VI,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "bar");

        CREDO_FALSUM (silva_c89_declaratio_vista(parsura, VII,
            &vista));
    }


    /* ========================================================
     * PROBARE: vectis lapifex DECL portata (M2b Chunk D) - omnes
     * 30 inputus probationis probatio_lapifex_c89_decl.c contra
     * genera silvae, per VIAM CONSUMPTORIS (silva_c89_parsare -
     * saltatio activa). "2+3" terminatorem accipit (mos M2a).
     * ======================================================== */

    {
        nomen structura {
            constans character* fons;
            s32                 genus;
        } VectisDecl;
        hic_manens constans VectisDecl VECTIS[] = {
            { "int x;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "long x = 12LL;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "char c;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "void *p;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int x = 42;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int a, b, c;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int *p;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int **pp;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "const int *p;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int arr[10];", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int arr[];", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int f(void);", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int f(int a, char b);",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int (*fp)(int);", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "void f(int, ...);", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "struct Foo { int x; char c; };",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "enum Color { RED, GREEN, BLUE };",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int x = 2 + 3;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int arr[] = {1, 2, 3};",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int a = 1, b = 2;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef unsigned long size_t;",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "static const int MAX = 100;",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "2+3;", (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS },
            { "int f();", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "enum E { A = 1, B = 2 };",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "struct Foo;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int a[] = {1, 2,};", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "extern int x;", (s32)SILVA_C89_GENUS_DECLARATIO },
            { "union U { int i; float f; };",
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "unsigned long long x;",
                (s32)SILVA_C89_GENUS_DECLARATIO }
        };
        i32 numerus = (i32)(magnitudo(VECTIS)
            / magnitudo(VECTIS[ZEPHYRUM]));
        i32 i;

        imprimere("\n--- Probans vectem lapifex decl (%d) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaParsura* parsura = silva_c89_parsare(piscina,
                "probatio.c", VECTIS[i].fons,
                (i32)strlen(VECTIS[i].fons), NIHIL);
            SilvaNodus* radix_nodus;

            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            radix_nodus = _sententia_prima(parsura);
            CREDO_NON_NIHIL (radix_nodus);
            si (radix_nodus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (radix_nodus->genus,
                    VECTIS[i].genus);
            }
        }
    }


    /* ========================================================
     * PROBARE: vectis lapifex SENT portata (M2c Chunk A) - omnes
     * 30 inputus probationis probatio_lapifex_c89_sent.c contra
     * genera silvae DECISA, per VIAM CONSUMPTORIS
     * (silva_c89_parsare). Omnes puri et sine ambiguis.
     * ======================================================== */

    {
        nomen structura {
            constans character* fons;
            s32                 genus;
        } VectisSent;
        hic_manens constans VectisSent VECTIS[] = {
            { "x = 5;",
                (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS },
            { ";", (s32)SILVA_C89_GENUS_SENTENTIA_VACUA },
            { "{ }", (s32)SILVA_C89_GENUS_CORPUS },
            { "{ x = 1; }", (s32)SILVA_C89_GENUS_CORPUS },
            { "{ x = 1; y = 2; z = 3; }",
                (s32)SILVA_C89_GENUS_CORPUS },
            { "{ int x; x = 1; }", (s32)SILVA_C89_GENUS_CORPUS },
            { "break;", (s32)SILVA_C89_GENUS_FRANGE },
            { "continue;", (s32)SILVA_C89_GENUS_PERGE },
            { "goto end;", (s32)SILVA_C89_GENUS_SALTA },
            { "return;", (s32)SILVA_C89_GENUS_REDDE },
            { "return x + 1;", (s32)SILVA_C89_GENUS_REDDE },
            { "if (x) y = 1;", (s32)SILVA_C89_GENUS_SI },
            { "if (x) y = 1; else y = 2;",
                (s32)SILVA_C89_GENUS_SI },
            { "if (a) if (b) x = 1; else y = 2;",
                (s32)SILVA_C89_GENUS_SI },
            { "while (x) x = x - 1;", (s32)SILVA_C89_GENUS_DUM },
            { "do x = x + 1; while (x < 10);",
                (s32)SILVA_C89_GENUS_FAC_DUM },
            { "for (i = 0; i < 10; i = i + 1) x = x + 1;",
                (s32)SILVA_C89_GENUS_PER },
            { "for (;;) x = 1;", (s32)SILVA_C89_GENUS_PER },
            { "for (i = 0;;) x = 1;", (s32)SILVA_C89_GENUS_PER },
            { "for (;i < 10;) x = 1;", (s32)SILVA_C89_GENUS_PER },
            { "switch (x) { case 1: y = 1; break; "
              "default: y = 0; break; }",
                (s32)SILVA_C89_GENUS_COMMUTATIO },
            { "label: x = 1;", (s32)SILVA_C89_GENUS_TITULATUM },
            { "if (x) { while (y) { if (z) break; "
              "else continue; } }",
                (s32)SILVA_C89_GENUS_SI },
            { "for (i = 0; i < 5; i = i + 1) { x = x + i; }",
                (s32)SILVA_C89_GENUS_PER },
            { "{ int x = 1; int y = 2; if (x < y) return x; "
              "else return y; }",
                (s32)SILVA_C89_GENUS_CORPUS },
            { "2 + 3;",
                (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS },
            { "for (;; i = i + 1) x = 1;",
                (s32)SILVA_C89_GENUS_PER },
            { "do { x = x + 1; y = y - 1; } while (x < y);",
                (s32)SILVA_C89_GENUS_FAC_DUM },
            { "while (1) { break; }", (s32)SILVA_C89_GENUS_DUM },
            { "if (x) { y = 1; } else { y = 2; }",
                (s32)SILVA_C89_GENUS_SI }
        };
        i32 numerus = (i32)(magnitudo(VECTIS)
            / magnitudo(VECTIS[ZEPHYRUM]));
        i32 i;

        imprimere("\n--- Probans vectem lapifex sent (%d) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaParsura* parsura = silva_c89_parsare(piscina,
                "probatio.c", VECTIS[i].fons,
                (i32)strlen(VECTIS[i].fons), NIHIL);
            SilvaNodus* radix_nodus;

            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            radix_nodus = _sententia_prima(parsura);
            CREDO_NON_NIHIL (radix_nodus);
            si (radix_nodus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (radix_nodus->genus,
                    VECTIS[i].genus);
            }
        }
    }


    /* ========================================================
     * PROBARE: definitio functionis (M2c Chunk B) - ANSI + K&R,
     * genus + loci + limites segmentorum (secans "()-ante-{")
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   definitio;

        imprimere("\n--- Probans definitiones functionum ---\n");

        /* ANSI: loci plena, declarationes-kr ABSENS */
        parsura = _parsare(piscina, "int f(void) { return 0; }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        definitio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (definitio->genus,
            (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);
        CREDO_NON_NIHIL (_nodus_valoris(
            silva_c89_definitio_functionis_declarator(definitio)));
        CREDO_NON_NIHIL (_nodus_valoris(
            silva_c89_definitio_functionis_corpus(definitio)));
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_definitio_functionis_declarationes_kr(
                definitio)), ZEPHYRUM);

        /* K&R: declarationes-kr praesens (II), ambigui ZEPHYRUM
         * (M2c Chunk C: lectio falsa ["int f" specificatores
         * bini] combinatione invalida - X10 necat; ISO 6.9.1
         * regulam confirmat: identificatores listae nomina sunt) */
        {
            SilvaParsura* parsura_kr = silva_c89_parsare(piscina,
                "probatio.c", "int f(a, b) int a; char b; { return a; }",
                (i32)strlen("int f(a, b) int a; char b; { return a; }"),
                NIHIL);
            SilvaNodus* definitio_kr;

            CREDO_VERUM (parsura_kr->successus);
            CREDO_AEQUALIS_I32 (parsura_kr->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (parsura_kr->numerus_segmentorum, I);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(
                parsura_kr->commissio->ambigui), ZEPHYRUM);
            definitio_kr = _sententia_prima(parsura_kr);
            CREDO_AEQUALIS_S32 (definitio_kr->genus,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                silva_c89_definitio_functionis_declarationes_kr(
                    definitio_kr)), II);
        }

        /* K&R int implicito: sine specificatoribus, purum */
        parsura = _parsare(piscina, "f(a, b) int a; char b; { }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);
        definitio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (definitio->genus,
            (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_definitio_functionis_declarationes_kr(
                definitio)), II);

        /* Limites segmentorum (secans M2c B): definitio unco
         * clauditur - definitiones binae = segmenta bina */
        parsura = _parsare(piscina, "int f(void) { } int g(void) { }");
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, II);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            parsura->commissio->radix), II);

        /* prototypum ";" statim post ")" = limes (non K&R) */
        parsura = _parsare(piscina, "int f(void); int f(void) { }");
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, II);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (_elementum(parsura->commissio->radix,
            I)->genus, (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);

        /* structura reddita: uncus structurae NON post ")" -
         * modus corporis recte ad uncum SECUNDUM */
        parsura = _parsare(piscina, "struct S { int x; } f(void) { }");
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, I);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);

        /* monstrator functionis initiatus: ASSIGNATIO clausam
         * exstinguit - "= &g" numquam corpus */
        parsura = _parsare(piscina, "int (*fp)(void) = &g; int y;");
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, II);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* cauda fac-dum post corpus uncatum: pendentia consumpta
         * ne in dum sequentem effluat */
        parsura = _parsare(piscina,
            "do { x; } while (a); while (b) y;");
        CREDO_AEQUALIS_I32 (parsura->numerus_segmentorum, II);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_FAC_DUM);
        CREDO_AEQUALIS_S32 (_elementum(parsura->commissio->radix,
            I)->genus, (s32)SILVA_C89_GENUS_DUM);
    }


    /* ========================================================
     * PROBARE: saltatio in corpus (Chunk B) - typedef gradus
     * plagulae in corpore functionis resolvit (furca
     * per-elementum intra corpus + oraculum positionale);
     * typedef INTRA corpus NON registratur (differentia
     * nominata, decisiones 13)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   definitio;
        SilvaNodus*   corpus;
        SilvaValor    elementa;

        imprimere("\n--- Probans saltationem in corpus ---\n");

        /* typedef gradus plagulae -> usus in corpore RESOLVITUR
         * (ambiguus nidificatus intra corpus, oraculum novit) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int T; void f(void) { T x; x = 1; }",
            (i32)strlen("typedef int T; void f(void) { T x; x = 1; }"),
            NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);
        definitio = _elementum(parsura->commissio->radix, I);
        CREDO_AEQUALIS_S32 (definitio->genus,
            (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS);
        corpus = _nodus_valoris(
            silva_c89_definitio_functionis_corpus(definitio));
        elementa = silva_c89_corpus_elementa(corpus);
        CREDO_AEQUALIS_S32 (_elementum(elementa, ZEPHYRUM)->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);

        /* typedef INTRA corpus: NON registratur (visus manet
         * planus gradu plagulae - decisiones 13); usus furcans
         * posterior ("T * x;" - declaratio-vel-expressio)
         * AMBIGUUS honeste manet. NB "T x;" non furcaret -
         * lectio expressionis grammatice non existit! */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "void f(void) { typedef int T; } T * x;",
            (i32)strlen("void f(void) { typedef int T; } T * x;"),
            NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_S32 (_elementum(parsura->commissio->radix,
            I)->genus, (s32)SILVA_C89_GENUS_AMBIGUUS);
    }


    /* ========================================================
     * PROBARE: vista FUNCTIONES + subscriptio (M2c Chunk C) -
     * ordines definitionum in vista declarationum; textus
     * subscriptionis octetim ex arbore (scribere e subarbore)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaDeclaratioVista vista;

        imprimere("\n--- Probans vistam FUNCTIONES ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c",
            "typedef int T; static int *f(int a, char b) "
            "{ return 0; } int g;",
            (i32)strlen("typedef int T; static int *f(int a, char b) "
            "{ return 0; } int g;"), NIHIL);
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* tres ordines: T (declaratio), f (definitio), g */
        CREDO_AEQUALIS_I32 (
            silva_c89_declarationes_numerus(parsura), III);

        CREDO_VERUM (silva_c89_declaratio_vista(parsura, I, &vista));
        CREDO_VERUM (strcmp(vista.genus,
            "definitio-functionis") == ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "f");
        CREDO_FALSUM (vista.est_typedef);

        /* est_typedef (M2c D - sectio TYPI): ordo T typedef est,
         * ordo g non est */
        CREDO_VERUM (silva_c89_declaratio_vista(parsura, ZEPHYRUM,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "T");
        CREDO_VERUM (vista.est_typedef);
        CREDO_VERUM (silva_c89_declaratio_vista(parsura, II,
            &vista));
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "g");
        CREDO_FALSUM (vista.est_typedef);

        /* subscriptio: specificatores + declarator, sine corpore
         * (trivia praecedentia manent - octeti fontis) */
        {
            SilvaScriptura subscriptio =
                silva_c89_functionis_subscriptio(piscina, parsura, I);

            CREDO_VERUM (subscriptio.successus);
            /* trivia SEQUENTIA lexematibus adhaerent - textus
             * sinistrorsum purus, spatium unum caudale (ante
             * corpus) - forma optima ordinis TOC */
            CREDO_CHORDA_AEQUALIS_LITERIS (subscriptio.textus,
                "static int *f(int a, char b) ");
        }

        /* ordo non functionis: fractura clara */
        {
            SilvaScriptura subscriptio =
                silva_c89_functionis_subscriptio(piscina, parsura,
                    ZEPHYRUM);

            CREDO_VERUM (!subscriptio.successus);
            CREDO_NON_NIHIL (subscriptio.causa);
        }

        /* K&R: titulus per catenam declaratoris post collapsum */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "int f(a) int a; { return a; }",
            (i32)strlen("int f(a) int a; { return a; }"), NIHIL);
        CREDO_AEQUALIS_I32 (
            silva_c89_declarationes_numerus(parsura), I);
        CREDO_VERUM (silva_c89_declaratio_vista(parsura, ZEPHYRUM,
            &vista));
        CREDO_VERUM (strcmp(vista.genus,
            "definitio-functionis") == ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "f");

        /* forma int-implicita: subscriptio = declarator solus */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "f(a) int a; { }",
            (i32)strlen("f(a) int a; { }"), NIHIL);
        {
            SilvaScriptura subscriptio =
                silva_c89_functionis_subscriptio(piscina, parsura,
                    ZEPHYRUM);

            CREDO_VERUM (subscriptio.successus);
            CREDO_CHORDA_AEQUALIS_LITERIS (subscriptio.textus,
                "f(a) ");
        }

        /* fons_index (M2d A+): ordo ex plagula inclusa fert
         * indicem SUUM, ordo plagulae principis fert
         * fons_princeps - filtrum TOC hoc distinguit */
        {
            SilvaContextus* ctx = silva_contextus_creare(piscina);
            SilvaParsura* parsura_ctx;

            CREDO_NON_NIHIL (ctx);
            CREDO_VERUM (silva_contextus_praebere(ctx, "amicus.h",
                "int ex_amico(void);\n",
                (i32)strlen("int ex_amico(void);\n")) >= ZEPHYRUM);
            parsura_ctx = silva_c89_parsare_cum_contextu(piscina,
                ctx, "probatio.c",
                "#include \"amicus.h\"\nint meus(void) { return 0; }",
                (i32)strlen("#include \"amicus.h\"\n"
                    "int meus(void) { return 0; }"), NIHIL);
            CREDO_VERUM (parsura_ctx->successus);
            CREDO_AEQUALIS_I32 (
                silva_c89_declarationes_numerus(parsura_ctx), II);
            /* ordo 0 = ex_amico (ex amicus.h), ordo 1 = meus */
            CREDO_VERUM (silva_c89_declaratio_vista(parsura_ctx,
                ZEPHYRUM, &vista));
            CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus,
                "ex_amico");
            CREDO_VERUM (vista.fons_index
                != parsura_ctx->fons_princeps);
            CREDO_VERUM (silva_c89_declaratio_vista(parsura_ctx, I,
                &vista));
            CREDO_CHORDA_AEQUALIS_LITERIS (vista.titulus, "meus");
            CREDO_AEQUALIS_S32 (vista.fons_index,
                parsura_ctx->fons_princeps);
        }

        /* trivia praecedentia TONSA (vexillum commenti ante
         * definitionem prorsum adhaeret - textus ordinis purus
         * manet; inventum probatione manuali Fran in
         * silva_scribere.c) */
        parsura = silva_c89_parsare(piscina, "probatio.c",
            "/* ==== vexillum ==== */\nint f(void) { return 0; }",
            (i32)strlen("/* ==== vexillum ==== */\n"
                "int f(void) { return 0; }"), NIHIL);
        {
            SilvaScriptura subscriptio =
                silva_c89_functionis_subscriptio(piscina, parsura,
                    ZEPHYRUM);

            CREDO_VERUM (subscriptio.successus);
            CREDO_CHORDA_AEQUALIS_LITERIS (subscriptio.textus,
                "int f(void) ");
        }
    }


    /* ========================================================
     * PROBARE: vectis lapifex FUNC portata (M2c Chunk B) - omnes
     * 20 inputus probationis probatio_lapifex_c89_func.c contra
     * genera silvae, per VIAM CONSUMPTORIS. Multi-elementa:
     * numerus elementorum + genus primi asseruntur.
     * ======================================================== */

    {
        nomen structura {
            constans character* fons;
            i32                 elementa;
            s32                 genus_primi;
        } VectisFunc;
        hic_manens constans VectisFunc VECTIS[] = {
            { "int f(void) { return 0; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int add(int a, int b) { return a + b; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "void noop(void) { }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int *get_ptr(void) { return 0; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int factorial(int n) { if (n <= 1) return 1; "
              "return n; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int x; int y;", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int f(void) { return 0; } "
              "int main(void) { return f(); }", II,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int a; int b; int c;", III,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "int x; void f(void) { }", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef int my_int; my_int x;", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef unsigned long size_t; size_t n;", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef struct Foo { int x; } Foo_t; Foo_t f;", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef int i32; "
              "i32 add(i32 a, i32 b) { return a + b; }", II,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef int i32;\n"
              "i32 add(i32 a, i32 b) { return a + b; }\n"
              "i32 main(void) { i32 x; x = add(1, 2); return x; }",
                III, (s32)SILVA_C89_GENUS_DECLARATIO },
            { "typedef int base_t; typedef base_t derived_t; "
              "derived_t v;", III,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            { "static int helper(void) { return 0; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "void setup(void) { int x; x = 0; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int x;", I, (s32)SILVA_C89_GENUS_DECLARATIO },
            { "unsigned long f(void) { return 0; }", I,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            { "int f(void) { } int g; int h(void) { } int i;", IV,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS }
        };
        i32 numerus = (i32)(magnitudo(VECTIS)
            / magnitudo(VECTIS[ZEPHYRUM]));
        i32 i;

        imprimere("\n--- Probans vectem lapifex func (%d) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaParsura* parsura = silva_c89_parsare(piscina,
                "probatio.c", VECTIS[i].fons,
                (i32)strlen(VECTIS[i].fons), NIHIL);
            SilvaNodus* primus;

            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                parsura->commissio->radix), VECTIS[i].elementa);
            primus = _sententia_prima(parsura);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    VECTIS[i].genus_primi);
            }
        }

        /* praeoneratio oraculi (vectis XX): typi externi ante
         * parsuram praeonerati (situs 0) resolvunt sine typedef
         * in fonte */
        {
            SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
            SilvaParsura* parsura;

            silva_oraculum_typum_addere_literis(oraculum, "size_t");
            silva_oraculum_typum_addere_literis(oraculum, "ptrdiff_t");
            parsura = silva_c89_parsare(piscina, "probatio.c",
                "size_t n; ptrdiff_t d;",
                (i32)strlen("size_t n; ptrdiff_t d;"), oraculum);
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
                (s32)SILVA_C89_GENUS_DECLARATIO);
            CREDO_AEQUALIS_S32 (_elementum(parsura->commissio->radix,
                I)->genus, (s32)SILVA_C89_GENUS_DECLARATIO);
        }
    }


    /* ========================================================
     * PROBARE: vectis lapifex EXPANDERE portata (M2d Chunk C) -
     * fistula expansionis integra per VIAM CONSUMPTORIS
     * (silva_c89_parsare_cum_contextu): macra in fonte, lexica
     * iniecta (silva_contextus_lexicon_addere), latina
     * (silva_contextus_latinam_addere), typedef per lexicon ad
     * oraculum. Directivae elementa radicis NON creant (empirice
     * probatum) - numerus elementorum + genus primi DECISA.
     * Omnes puri: errores 0, ambigui 0.
     * ======================================================== */

    {
        nomen structura {
            constans character* fons;
            constans character* lexicon;  /* NIHIL = sine iniecto */
            b32                 cum_latina;
            i32                 elementa;
            s32                 genus_primi;
        } VectisExp;
        hic_manens constans VectisExp VECTIS[] = {
            /* fons purus per contextum (sine macris) */
            { "int x;\n", NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* macrum obiecti in fonte */
            { "#define N 5\nint t[N];\n", NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* macrum functionis in fonte */
            { "#define SQ(x) ((x)*(x))\nint y = SQ(3);\n",
                NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* macrum functionis intra corpus definitionis */
            { "#define SQ(x) ((x)*(x))\n"
              "int f(void) { return SQ(2); }\n",
                NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            /* macra nidificata */
            { "#define A B\n#define B 7\nint z = A;\n",
                NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* macrum multi-elementorum (duo declarationes) */
            { "#define DECL int a; int b;\nDECL\n",
                NIHIL, FALSUM, 2,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* stringificatio (limes M1 - via parsationis) */
            { "#define STR(x) #x\n"
              "const char *s = STR(salve);\n",
                NIHIL, FALSUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* latina: definitio functionis cum si/redde */
            { "integer f(vacuum) { si (1) redde 0; redde 1; }\n",
                NIHIL, VERUM, 1,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            /* latina: interior = static */
            { "interior int g(vacuum) { redde 1; }\n",
                NIHIL, VERUM, 1,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            /* latina: nomen structura = typedef struct */
            { "nomen structura { int n; } Probandum;\n",
                NIHIL, VERUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* latina: typedef + usus (oraculum trans elementa) */
            { "nomen int Numerus;\nNumerus n;\n",
                NIHIL, VERUM, 2,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* latina: structura nominata */
            { "structura Punctum { int x; int y; };\n",
                NIHIL, VERUM, 1,
                (s32)SILVA_C89_GENUS_DECLARATIO },
            /* latina: sententiae per/dum/frange in corpore */
            { "vacuum h(vacuum) { per (;;) frange; }\n",
                NIHIL, VERUM, 1,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS },
            /* lexicon iniectum: macrum typi */
            { "MEUS_TYPUS x;\n", "#define MEUS_TYPUS int\n",
                FALSUM, 1, (s32)SILVA_C89_GENUS_DECLARATIO },
            /* lexicon iniectum: typedef ad oraculum */
            { "Meus m;\n", "typedef int Meus;\n",
                FALSUM, 1, (s32)SILVA_C89_GENUS_DECLARATIO },
            /* lexicon iniectum + macrum fontis compositum */
            { "#define QUINQUE 5\nMeus t[QUINQUE];\n",
                "typedef int Meus;\n",
                FALSUM, 1, (s32)SILVA_C89_GENUS_DECLARATIO },
            /* lexicon + latina simul */
            { "interior Meus quaesitum(vacuum) { redde 0; }\n",
                "typedef int Meus;\n",
                VERUM, 1,
                (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS }
        };
        i32 numerus = (i32)(magnitudo(VECTIS)
            / magnitudo(VECTIS[ZEPHYRUM]));
        i32 i;

        imprimere("\n--- Probans vectem lapifex expandere (%d) ---\n",
            (int)numerus);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaContextus* ctx = silva_contextus_creare(piscina);
            SilvaParsura*   parsura;
            SilvaNodus*     primus;

            CREDO_NON_NIHIL (ctx);
            si (VECTIS[i].cum_latina)
            {
                CREDO_VERUM (silva_contextus_latinam_addere(ctx));
            }
            si (VECTIS[i].lexicon != NIHIL)
            {
                CREDO_VERUM (silva_contextus_lexicon_addere(ctx,
                    "iniectum.h", VECTIS[i].lexicon,
                    (i32)strlen(VECTIS[i].lexicon)));
            }
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", VECTIS[i].fons,
                (i32)strlen(VECTIS[i].fons), NIHIL);
            CREDO_NON_NIHIL (parsura);
            si (parsura == NIHIL) perge;
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (
                parsura->commissio->radix.datum.lista.mensura,
                VECTIS[i].elementa);
            primus = _elementum(parsura->commissio->radix, ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    VECTIS[i].genus_primi);
            }
        }
    }


    /* ========================================================
     * PROBARE: sanatio oraculi (2026-07-06) - visibilitas per
     * ORDINEM, non per situm. Typedef SERO in capite suo (post
     * commentarium longum: offset in capite > offset usus in
     * fonte principali) solvere DEBET - comparatio situm trans
     * fontes 2,382 lectiones falso retinebat (probatio empirica,
     * silva-incrementalitas.md par V). Et inclusio POST usum
     * honeste retinetur (ordo ambulationis = semantica puncti
     * declarationis ISO).
     * ======================================================== */

    {
        constans character* caput_tardum =
            "/* commentarium longum longum longum longum longum"
            " longum longum longum longum longum longum longum"
            " longum longum longum longum longum longum */\n"
            "typedef int Tardus;\n";

        imprimere("\n--- Probans sanationem oraculi ---\n");

        /* typedef sero in capite -> solvitur (ambigui 0) */
        {
            SilvaContextus* ctx = silva_contextus_creare(piscina);
            constans character* fons =
                "#include \"tarde.h\"\nTardus * t;\n";
            SilvaParsura* parsura;
            SilvaNodus* primus;

            CREDO_NON_NIHIL (ctx);
            CREDO_VERUM (silva_contextus_praebere(ctx, "tarde.h",
                caput_tardum, (i32)strlen(caput_tardum)));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", fons, (i32)strlen(fons), NIHIL);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            primus = _elementum(parsura->commissio->radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    (s32)SILVA_C89_GENUS_DECLARATIO);
            }
        }

        /* inclusio POST usum -> retentio honesta (ambigui I) */
        {
            SilvaContextus* ctx = silva_contextus_creare(piscina);
            constans character* fons =
                "Tardus * t;\n#include \"tarde.h\"\n";
            SilvaParsura* parsura;
            SilvaNodus* primus;

            CREDO_NON_NIHIL (ctx);
            CREDO_VERUM (silva_contextus_praebere(ctx, "tarde.h",
                caput_tardum, (i32)strlen(caput_tardum)));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", fons, (i32)strlen(fons), NIHIL);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), I);
            primus = _elementum(parsura->commissio->radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    (s32)SILVA_C89_GENUS_AMBIGUUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: typi lexici oraculo visibiles (01M0D4QV5S pars 1).
     * Via lexici directivas solas processabat - "Reliqua lexici
     * abiciuntur" typos perdebat: 'i8 * t;' ubi clang
     * declarationem videt AMBIGUUM manebat (venatio nuntium
     * 43/44, damnum congelatum formatoris). Sanatio: plagulae
     * lexicae standalone parsantur ORACULO COMMUNI ante fontem
     * principalem - typedefs earum per uncum praecommissionis
     * registrantur (praeonerati = visibiles ubique, ordo
     * ambulationis).
     * ======================================================== */

    {
        imprimere("\n--- Probans typos lexici ---\n");

        /* latina: i8 * t; ad radicem -> declaratio soluta */
        {
            SilvaContextus*     ctx = silva_contextus_creare(
                piscina);
            SilvaOraculum*      oraculum = silva_oraculum_creare(
                piscina);
            constans character* fons = "i8 * t;\n";
            SilvaParsura*       parsura;
            SilvaNodus*         primus;

            CREDO_NON_NIHIL (ctx);
            CREDO_NON_NIHIL (oraculum);
            CREDO_VERUM (silva_contextus_latinam_addere(ctx));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", fons, (i32)strlen(fons), oraculum);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);
            /* typi latinae registrati */
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("i8", piscina)));
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("i32", piscina)));
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("s64", piscina)));
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("b32", piscina)));
            /* i8 * t solvitur - retentio nulla */
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            primus = _elementum(parsura->commissio->radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    (s32)SILVA_C89_GENUS_DECLARATIO);
            }
        }

        /* cascata: lexicon posterius typum lexici prioris videt
         * (oraculum commune trans plagulas lexicas, ordine) */
        {
            SilvaContextus*     ctx = silva_contextus_creare(
                piscina);
            SilvaOraculum*      oraculum = silva_oraculum_creare(
                piscina);
            constans character* fons = "Derivatus * d;\n";
            SilvaParsura*       parsura;
            SilvaNodus*         primus;

            CREDO_NON_NIHIL (ctx);
            CREDO_NON_NIHIL (oraculum);
            CREDO_VERUM (silva_contextus_lexicon_addere(ctx,
                "primum.h", "typedef int Basis;\n",
                (i32)strlen("typedef int Basis;\n")));
            CREDO_VERUM (silva_contextus_lexicon_addere(ctx,
                "secundum.h", "typedef Basis Derivatus;\n",
                (i32)strlen("typedef Basis Derivatus;\n")));
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", fons, (i32)strlen(fons), oraculum);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("Basis", piscina)));
            CREDO_VERUM (silva_oraculum_typum_novit(oraculum,
                chorda_ex_literis("Derivatus", piscina)));
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            primus = _elementum(parsura->commissio->radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    (s32)SILVA_C89_GENUS_DECLARATIO);
            }
        }

        /* custodia contra registrationem phantasma: sine lexico
         * retentio honesta manet */
        {
            SilvaContextus*     ctx = silva_contextus_creare(
                piscina);
            constans character* fons = "i8 * t;\n";
            SilvaParsura*       parsura;
            SilvaNodus*         primus;

            CREDO_NON_NIHIL (ctx);
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "probatio.c", fons, (i32)strlen(fons), NIHIL);
            CREDO_NON_NIHIL (parsura);
            CREDO_VERUM (parsura->successus);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), I);
            primus = _elementum(parsura->commissio->radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL (primus);
            si (primus != NIHIL)
            {
                CREDO_AEQUALIS_S32 (primus->genus,
                    (s32)SILVA_C89_GENUS_AMBIGUUS);
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

                /* decodificare literam C: cita exteriora cadunt;
                 * \" \\ \n \t solvuntur; literae ADIACENTES
                 * ("pars\n" "pars") CONCATENANTUR (forma corporis
                 * "paste-ready" - decodificator vetus ad citum
                 * primum frangebat et lineas multi-literas
                 * TRUNCABAT; inventum M2c Chunk B) */
                per (k = I; k < (i32)m; k++)
                {
                    si (linea[k] == '\\' && k + I < (i32)m)
                    {
                        si (linea[k + I] == '"'
                            || linea[k + I] == '\\')
                        {
                            decodata[d++] = linea[k + I];
                            k++;
                            perge;
                        }
                        si (linea[k + I] == 'n')
                        {
                            decodata[d++] = '\n';
                            k++;
                            perge;
                        }
                        si (linea[k + I] == 't')
                        {
                            decodata[d++] = '\t';
                            k++;
                            perge;
                        }
                        decodata[d++] = linea[k];
                        perge;
                    }
                    si (linea[k] == '"')
                    {
                        /* clausura - litera adiacens sequitur? */
                        i32 p = k + I;

                        dum (p < (i32)m && (linea[p] == ' '
                            || linea[p] == '\t'))
                        {
                            p++;
                        }
                        si (p < (i32)m && linea[p] == '"')
                        {
                            k = p;  /* resumere post citum aperiens */
                            perge;
                        }
                        frange;  /* cita clausurae vera */
                    }
                    decodata[d++] = linea[k];
                }

                piscina_lineae = piscina_generare_dynamicum(
                    "corpus_lineae_c89", 4194304);
                si (piscina_lineae == NIHIL)
                {
                    perge;
                }
                parsura = silva_c89_parsare(piscina_lineae,
                    "corpus.c", decodata, d, NIHIL);
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
        /* ASCENSUS COMPREHENSIONIS (metrum M2d in miniatura):
         * M2a = 0 -> M2b A = 69 (declarationes) -> B = 87 (aggregata
         * + sectio uncorum) -> M2c A = 87 (lineae sententiarum
         * omnes intra definitiones involutae) -> M2c B = 123
         * (MESSIS: definitiones + sententiae earum = 30 lineae;
         * decodificator harnessis correctus = 5 [lineae
         * multi-literae truncabantur - artefactum, non
         * comprehensio]; virgula caudalis enumeratorum = 1
         * [C99-ismus, norma robustitatis]).
         * RELIQUUM II = "__attribute__((packed)) int x;" et
         * "__extension__ __attribute__((unused)) int y;" -
         * extensiones gcc, NOMINATAE, porta evidentiae = si
         * corpus verum eas umquam poscit (fontes rhubarb eas
         * non habent). Mutatio deliberata, numquam tacita. */
        CREDO_AEQUALIS_I32 (sine_erroribus, CXXIII);
    }


    /* ========================================================
     * PROBARE: sententiae (M2c Chunk A) - genera regiminis
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   sententia;

        imprimere("\n--- Probans sententias (genera regiminis) ---\n");

        /* dum */
        parsura = _parsare(piscina, "while (x) y;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus, (s32)SILVA_C89_GENUS_DUM);
        CREDO_AEQUALIS_S32 (_nodus_valoris(
            silva_c89_dum_corpus(sententia))->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);

        /* fac-dum */
        parsura = _parsare(piscina, "do x; while (y);");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus,
            (s32)SILVA_C89_GENUS_FAC_DUM);

        /* per - clausula plena (nodus verus, tria loci) */
        parsura = _parsare(piscina, "for (i = 0; i < n; i++) f(i);");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus, (s32)SILVA_C89_GENUS_PER);
        {
            SilvaNodus* clausula = _nodus_valoris(
                silva_c89_per_clausula(sententia));

            CREDO_NON_NIHIL (clausula);
            CREDO_AEQUALIS_S32 (clausula->genus,
                (s32)SILVA_C89_GENUS_PER_CLAUSULA);
            CREDO_NON_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_initium(clausula)));
            CREDO_NON_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_conditio(clausula)));
            CREDO_NON_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_passus(clausula)));
        }

        /* per - clausula vacua: loci absentes = NIHIL */
        parsura = _parsare(piscina, "for (;;) x;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sententia = _sententia_prima(parsura);
        {
            SilvaNodus* clausula = _nodus_valoris(
                silva_c89_per_clausula(sententia));

            CREDO_NON_NIHIL (clausula);
            CREDO_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_initium(clausula)));
            CREDO_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_conditio(clausula)));
            CREDO_NIHIL (_nodus_valoris(
                silva_c89_per_clausula_passus(clausula)));
        }

        /* redde cum valore et sine */
        parsura = _parsare(piscina, "return x + 1;");
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus, (s32)SILVA_C89_GENUS_REDDE);
        CREDO_NON_NIHIL (_nodus_valoris(silva_c89_redde_valor(sententia)));

        parsura = _parsare(piscina, "return;");
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus, (s32)SILVA_C89_GENUS_REDDE);
        CREDO_NIHIL (_nodus_valoris(silva_c89_redde_valor(sententia)));

        /* salta / frange / perge / vacua */
        parsura = _parsare(piscina, "goto finis;");
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_SALTA);
        parsura = _parsare(piscina, "break;");
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_FRANGE);
        parsura = _parsare(piscina, "continue;");
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_PERGE);
        parsura = _parsare(piscina, ";");
        CREDO_AEQUALIS_S32 (_sententia_prima(parsura)->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_VACUA);

        /* titulatum: deterministicum (prospectus praecisi - cella
         * COLON numquam nata est, vide censum) */
        parsura = _parsare(piscina, "iterum: x = 1;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);
        sententia = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (sententia->genus,
            (s32)SILVA_C89_GENUS_TITULATUM);
        CREDO_AEQUALIS_S32 (_nodus_valoris(
            silva_c89_titulatum_sententia(sententia))->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);
    }


    /* ========================================================
     * PROBARE: corpus = UNA lista (emendatio DECISUS 2026-07-04)
     * - declaratio et sententia VICINAE in elementis; furca
     * declaratio-vel-expressio PER ELEMENTUM involvitur, numquam
     * corpus totum
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   corpus;
        SilvaValor    elementa;

        imprimere("\n--- Probans corpus (lista una, furca per elementum) ---\n");

        /* isomorphismus: declaratio + sententia vicinae */
        parsura = _parsare(piscina, "{ int x; f(); }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        corpus = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (corpus->genus, (s32)SILVA_C89_GENUS_CORPUS);
        elementa = silva_c89_corpus_elementa(corpus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(elementa), II);
        CREDO_AEQUALIS_S32 (_elementum(elementa, ZEPHYRUM)->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (_elementum(elementa, I)->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);

        /* corpus vacuum */
        parsura = _parsare(piscina, "{ }");
        corpus = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (corpus->genus, (s32)SILVA_C89_GENUS_CORPUS);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_corpus_elementa(corpus)), ZEPHYRUM);

        /* THE MONEY SHOT (intentio M2c, quaestio aperta I): furca
         * "foo * bar;" intra corpus UNUM elementum involvit -
         * vicinus purus manet, corpus ipsum purum manet. Sub
         * forma bi-listarum reiecta involucrum corpus TOTUM
         * cepisset et resolutor caecus fuisset. */
        parsura = _parsare(piscina, "{ foo * bar; baz(); }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        corpus = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (corpus->genus, (s32)SILVA_C89_GENUS_CORPUS);
        elementa = silva_c89_corpus_elementa(corpus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(elementa), II);
        CREDO_AEQUALIS_S32 (_elementum(elementa, ZEPHYRUM)->genus,
            (s32)SILVA_C89_GENUS_AMBIGUUS);
        CREDO_AEQUALIS_S32 (_elementum(elementa, I)->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);
    }


    /* ========================================================
     * PROBARE: alioquin pendens (praelatio) - ELSE intimo ligat,
     * ZEPHYRUM ambigui (resolutio in TABULIS, non per furcam)
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   si_externum;
        SilvaNodus*   si_internum;

        imprimere("\n--- Probans alioquin pendens (praelatio) ---\n");

        parsura = _parsare(piscina, "if (a) if (b) x; else y;");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);

        si_externum = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (si_externum->genus, (s32)SILVA_C89_GENUS_SI);
        /* externum: alioquin ABSENS */
        CREDO_NIHIL (_nodus_valoris(
            silva_c89_si_alioquin(si_externum)));
        /* internum: alioquin ADEST (intimo ligat - ISO 3.6.4.1) */
        si_internum = _nodus_valoris(
            silva_c89_si_consequens(si_externum));
        CREDO_NON_NIHIL (si_internum);
        CREDO_AEQUALIS_S32 (si_internum->genus, (s32)SILVA_C89_GENUS_SI);
        CREDO_NON_NIHIL (_nodus_valoris(
            silva_c89_si_alioquin(si_internum)));

        /* forma simplex adhuc integra */
        parsura = _parsare(piscina, "if (a) x; else y;");
        si_externum = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (si_externum->genus, (s32)SILVA_C89_GENUS_SI);
        CREDO_NON_NIHIL (_nodus_valoris(
            silva_c89_si_alioquin(si_externum)));
        parsura = _parsare(piscina, "if (a) x;");
        CREDO_NIHIL (_nodus_valoris(
            silva_c89_si_alioquin(_sententia_prima(parsura))));
    }


    /* ========================================================
     * PROBARE: commutatio gregata (decisiones 10) - casus/
     * ordinarius sententias possident; praefatio vaga ante
     * gregem primum; per-currentia = lista vacua
     * ======================================================== */

    {
        SilvaParsura* parsura;
        SilvaNodus*   selectio;
        SilvaNodus*   corpus;
        SilvaValor    elementa;

        imprimere("\n--- Probans commutationem gregatam ---\n");

        parsura = _parsare(piscina,
            "switch (x) { case 1: a; b; case 2: default: c; }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        selectio = _sententia_prima(parsura);
        CREDO_AEQUALIS_S32 (selectio->genus,
            (s32)SILVA_C89_GENUS_COMMUTATIO);
        corpus = _nodus_valoris(silva_c89_commutatio_corpus(selectio));
        CREDO_NON_NIHIL (corpus);
        CREDO_AEQUALIS_S32 (corpus->genus, (s32)SILVA_C89_GENUS_CORPUS);
        elementa = silva_c89_corpus_elementa(corpus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(elementa), III);
        {
            SilvaNodus* grex_i = _elementum(elementa, ZEPHYRUM);
            SilvaNodus* grex_ii = _elementum(elementa, I);
            SilvaNodus* grex_iii = _elementum(elementa, II);

            CREDO_AEQUALIS_S32 (grex_i->genus,
                (s32)SILVA_C89_GENUS_CASUS);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                silva_c89_casus_sententiae(grex_i)), II);

            /* per-currentia: "case 2:" statim sequente ordinario -
             * lista vacua, grex tamen verus */
            CREDO_AEQUALIS_S32 (grex_ii->genus,
                (s32)SILVA_C89_GENUS_CASUS);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                silva_c89_casus_sententiae(grex_ii)), ZEPHYRUM);

            CREDO_AEQUALIS_S32 (grex_iii->genus,
                (s32)SILVA_C89_GENUS_ORDINARIUS);
            CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
                silva_c89_ordinarius_sententiae(grex_iii)), I);
        }

        /* praefatio vaga (declaratio + sententia ante gregem
         * primum - scissio ec-ante/ec-post, cellae NULLAE) */
        parsura = _parsare(piscina,
            "switch (x) { int y; f(); case 1: a; }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        corpus = _nodus_valoris(silva_c89_commutatio_corpus(
            _sententia_prima(parsura)));
        elementa = silva_c89_corpus_elementa(corpus);
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(elementa), III);
        CREDO_AEQUALIS_S32 (_elementum(elementa, ZEPHYRUM)->genus,
            (s32)SILVA_C89_GENUS_DECLARATIO);
        CREDO_AEQUALIS_S32 (_elementum(elementa, I)->genus,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);
        CREDO_AEQUALIS_S32 (_elementum(elementa, II)->genus,
            (s32)SILVA_C89_GENUS_CASUS);

        /* commutatio vacua */
        parsura = _parsare(piscina, "switch (x) { }");
        CREDO_VERUM (parsura->successus);
        corpus = _nodus_valoris(silva_c89_commutatio_corpus(
            _sententia_prima(parsura)));
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(
            silva_c89_corpus_elementa(corpus)), ZEPHYRUM);

        /* commutatio intra gregem (nidificata) */
        parsura = _parsare(piscina,
            "switch (x) { case 1: switch (y) { case 2: a; } b; }");
        CREDO_VERUM (parsura->successus);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
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
            "*p++ + sizeof(int*);",
            "unsigned long x = 5;", "char *const p, **q;",
            "int (*f)(int, char*);", "int a[10], b[];",
            "typedef int foo;", "static const foo x;",
            "foo * bar;", "int f(foo);", "unsigned foo x;",
            "(const char*)x;", "f(a)[3];",
            "int f(int (*g)(void), ...);",
            "extern int a, b[3], (*c)(void);",
            "register int i = 0, j = f(i);",
            "struct S { int x; char *p; } s;",
            "struct S { int x; };",
            "struct { unsigned a : 3; int : 2; } f;",
            "union U { int i; float f; } u;",
            "enum E { A, B = 2, C } e;",
            "enum E x;",
            "int m[2][2] = { { 1, 2 }, { 3, 4 }, };",
            "struct S s = { 1, { 2, 3 } };",
            "(struct S*)p;",
            "sizeof(enum E);",
            "typedef struct S { int x; } S_t;",
            "struct A { struct B { int x; } b; } a;",
            /* sententiae (M2c Chunk A) */
            "if (a) x;",
            "if (a) x; else y;",
            "if (a) if (b) x; else y;",
            "while (x) y;",
            "do x; while (y);",
            "for (i = 0; i < n; i++) f(i);",
            "for (;;) x;",
            "for (i = 0, j = n; ; j--) ;",
            "{ int x; f(); }",
            "{ }",
            "{ foo * bar; baz(); }",
            "switch (x) { case 1: a; b; case 2: default: c; }",
            "switch (x) { int y; f(); case 1: a; }",
            "switch (x) { }",
            "iterum: x = 1;",
            "goto finis;",
            "break;", "continue;", "return;", "return x + 1;",
            "if (a) { x; } else { y; }",
            "while (x) { if (y) break; else continue; }",
            "  if  ( a )  x ;  else  y ;",
            "/* ante */ while (x) /* medium */ y; /* post */",
            /* definitiones functionum (M2c Chunk B) */
            "int f(void) { return 0; }",
            "int add(int a, int b) { return a + b; }",
            "int f(a, b) int a; char b; { return a; }",
            "f(a) int a; { }",
            "struct S { int x; } f(void) { return s; }",
            "int f(void) { } int g(void) { }",
            "int f(void); int f(void) { }",
            "int (*fp)(void) = &g;",
            "static void f(void) { do { x--; } while (x); }",
            "int main(void) { int x; x = 1; return x; }",
            "enum Flags { F1 = 1, F2 = 2, };",
            "void f(void) /* corpus */ { }  /* cauda */"
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


    /* ========================================================
     * PROBARE: commentarium ducens (regula arcte-supra -
     * consumptor primus pinnae "commenta sunt contenta";
     * INTENTIO silva/phase-log 2026-07-14)
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaNodus* nodus;
        SilvaCommentariumVista vista;

        imprimere("\n--- Probans commentarium ducens ---\n");

        /* attachatum simplex (etiam caput plagulae sine vacua) */
        parsura = _parsare(piscina,
            "/* contractus rei */\nint f(void);\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);
        nodus = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_NON_NIHIL (nodus);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), I);
        CREDO_AEQUALIS_I32 ((i32)vista.initium, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)vista.finis, 20);
        CREDO_AEQUALIS_I32 ((i32)vista.linea, I);

        /* linea vacua inter commentum et nodum = absens */
        parsura = _parsare(piscina,
            "/* alienum */\n\nint g(void);\n");
        CREDO_NON_NIHIL (parsura);
        nodus = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_NON_NIHIL (nodus);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)vista.initium, (i32)-I);

        /* bloccus multi-commentum contiguus - totus attachatur */
        parsura = _parsare(piscina,
            "/* pars a */\n/* pars b */\nint h(void);\n");
        CREDO_NON_NIHIL (parsura);
        nodus = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), I);
        CREDO_AEQUALIS_I32 ((i32)vista.initium, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)vista.finis, 25);

        /* linea vacua INTRA bloccum - pars superior cadit */
        parsura = _parsare(piscina,
            "/* vetus */\n\n/* recens */\nint i(void);\n");
        CREDO_NON_NIHIL (parsura);
        nodus = _elementum(parsura->commissio->radix, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), I);
        CREDO_AEQUALIS_I32 ((i32)vista.initium, 13);
        CREDO_AEQUALIS_I32 ((i32)vista.finis, 25);

        /* commentum finis-lineae prioris NON attachatur (politica
         * divisionis lexatoris: in spatia_post prioris manet) */
        parsura = _parsare(piscina,
            "int a; /* caudale */\nint j(void);\n");
        CREDO_NON_NIHIL (parsura);
        nodus = _elementum(parsura->commissio->radix, I);
        CREDO_NON_NIHIL (nodus);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), ZEPHYRUM);

        /* declaratio macro-initiata: lexema primum = expansio
         * synthetica, radix originis (invocatio) trivia fert */
        parsura = _parsare(piscina,
            "#define proprius static\n"
            "/* per macrum */\nproprius int k(void);\n");
        CREDO_NON_NIHIL (parsura);
        {
            i32 e;

            nodus = NIHIL;
            per (e = ZEPHYRUM; e < III; e++)
            {
                SilvaNodus* cand = _elementum(
                    parsura->commissio->radix, e);

                si (cand != NIHIL && cand->genus
                    == (s32)SILVA_C89_GENUS_DECLARATIO)
                {
                    nodus = cand;
                    frange;
                }
            }
        }
        CREDO_NON_NIHIL (nodus);
        CREDO_AEQUALIS_I32 ((i32)silva_commentarium_ducens(nodus,
            parsura->fons_princeps, &vista), I);
        CREDO_AEQUALIS_I32 ((i32)vista.initium, 24);
        CREDO_AEQUALIS_I32 ((i32)vista.finis, 40);
        CREDO_AEQUALIS_I32 ((i32)vista.linea, II);
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
