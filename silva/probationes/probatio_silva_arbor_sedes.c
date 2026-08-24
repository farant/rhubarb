/* probatio_silva_arbor_sedes.c - tabula sedium valorum (0032)
 *
 * Probat SilvaArborScriptura.sedes_valorum: valor silvae quisque
 * (nodus aut lexema) ad extensionem elementi SUI in documento
 * emisso.
 *
 * ORACULA DUO:
 *  1. FETTA-TAG: pro clavi quaque tag expectatum ALIUNDE derivatur
 *     (nodus -> registrum, lexema -> silva_arbor_lexema_tag) et
 *     octeti documenti ad initium eius conferuntur - monstrator
 *     falsus aut extensio falsa tagum alienum inveniret.
 *  2. CENSUS AMBULATIONIS: arbor parsurae independenter ambulatur;
 *     nodi tabulae == nodi ambulati, lexemata tabulae == lexemata
 *     DISTINCTA + cauda (lexema communicatum SEMEL apparet - sedes
 *     definitionis). Sic tabula nec deficiens nec superflua esse
 *     potest sine clamore.
 *
 * Fixtura ambigua fragmenta cogit ('t * x;' oraculo NIHIL) -
 * praesentia eorum in documento ASSERITUR ne semita communicationis
 * tacite non exercita maneat.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_arbor.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Clavis tabulae = octeti monstratoris (exemplar silva_arbor.c) */
interior chorda
_clavis_monstratoris (
            Piscina* piscina,
    constans vacuum* monstrator)
{
    vacuum* cella;
    chorda  clavis;

    cella = piscina_allocare(piscina,
        magnitudo(constans vacuum*));
    si (cella == NIHIL)
    {
        clavis.mensura  = ZEPHYRUM;
        clavis.datum    = NIHIL;
        redde clavis;
    }
    *(constans vacuum**)cella  = monstrator;
    clavis.mensura             = (i32)magnitudo(constans vacuum*);
    clavis.datum               = (i8*)cella;
    redde clavis;
}

/* Ambulatio independens: nodos numerare, lexemata DISTINCTA in
 * tabulam ponere (bracchia ambigua lexemata eadem ferunt - semel
 * numeranda, sicut scriptor semel emittit). */
interior vacuum
_valorem_ambulare (
           Piscina* piscina,
        SilvaValor  valor,
    TabulaDispersa* lexemata_distincta,
               i32* nodi,
               i32* lexemata)
{
    i32 i;

    si (valor.genus == SILVA_VALOR_TOKEN)
    {
        chorda  clavis;
        vacuum* inventum;

        si (valor.datum.token == NIHIL)
        {
            redde;
        }
        clavis = _clavis_monstratoris(piscina,
            (constans vacuum*)valor.datum.token);
        si (clavis.datum == NIHIL)
        {
            redde;
        }
        si (!tabula_dispersa_invenire(lexemata_distincta, clavis,
                &inventum))
        {
            tabula_dispersa_inserere(lexemata_distincta, clavis,
                (vacuum*)valor.datum.token);
            *lexemata = *lexemata + I;
        }
        redde;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        per (i = ZEPHYRUM; i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* e;

            e = silva_valor_lista_obtinere(valor, i);
            si (e != NIHIL)
            {
                _valorem_ambulare(piscina, *e, lexemata_distincta,
                    nodi, lexemata);
            }
        }
        redde;
    }
    si (valor.genus == SILVA_VALOR_NODUS)
    {
        constans SilvaNodus* n;

        n = valor.datum.nodus;
        si (n == NIHIL)
        {
            redde;
        }
        *nodi = *nodi + I;
        per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
        {
            _valorem_ambulare(piscina, n->loci[i],
                lexemata_distincta, nodi, lexemata);
        }
    }
}

/* Oraculum fettae: octeti ad [initium, finis) elementum tagi
 * expectati esse debent. */
interior b32
_fettam_conferre (
                chorda  textus,
                   i32  initium,
                   i32  finis,
    constans character* tag)
{
    memoriae_index longitudo_tagi;
                i8 proximus;

    longitudo_tagi = strlen(tag);
    si (   initium >= finis
        || finis > textus.mensura
        || (memoriae_index)(finis - initium) < longitudo_tagi + II)
    {
        redde FALSUM;
    }
    si (textus.datum[initium] != (i8)'<')
    {
        redde FALSUM;
    }
    si (memcmp(textus.datum + initium + I, tag, longitudo_tagi)
        != ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* post tagum: attributum, clausura aut finis lineae - numquam
     * littera tagi ulterior (ne 'lex-i' pro 'lex-int' transiret) */
    proximus = textus.datum[(memoriae_index)initium + I
                            + longitudo_tagi];
    si (   (proximus >= (i8)'a' && proximus <= (i8)'z')
        || (proximus >= (i8)'0' && proximus <= (i8)'9')
        || proximus == (i8)'-')
    {
        redde FALSUM;
    }
    redde (b32)(textus.datum[finis - I] == (i8)'>');
}

/* Circuitus unus: parsare, scribere, tabulam iudicare. */
interior vacuum
_fixturam_probare (
               Piscina* piscina,
    constans character* titulus,
    constans character* fons,
                   b32  fragmenta_expectata)
{
           SilvaParsura* parsura;
    SilvaArborScriptura  scriptura;
         TabulaDispersa* distincta;
                    i32  nodi_ambulati;
                    i32  lexemata_ambulata;
                    i32  nodi_tabulae;
                    i32  lexemata_tabulae;
                    i32  cauda;
                    i32  i;
                    b32  fragmentum_visum;

    imprimere("\n--- %s ---\n", titulus);

    parsura = silva_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), &SILVA_C89_GRAMMATICA, NIHIL, NIHIL,
        NIHIL);
    CREDO_NON_NIHIL (parsura);
    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        redde;
    }

    scriptura = silva_arbor_scribere_parsuram(piscina, parsura,
        &SILVA_C89_REGISTRUM, "c89", parsura->fons_princeps, NIHIL);
    CREDO_VERUM (scriptura.successus);
    CREDO_NON_NIHIL (scriptura.sedes_valorum);
    si (!scriptura.successus || scriptura.sedes_valorum == NIHIL)
    {
        redde;
    }

    /* census independens */
    distincta = tabula_dispersa_creare_chorda(piscina, 256);
    CREDO_NON_NIHIL (distincta);
    nodi_ambulati      = ZEPHYRUM;
    lexemata_ambulata  = ZEPHYRUM;
    _valorem_ambulare(piscina, parsura->commissio->radix,
        distincta, &nodi_ambulati, &lexemata_ambulata);
    CREDO_MAIOR_I32 (nodi_ambulati, (i32)ZEPHYRUM);
    CREDO_MAIOR_I32 (lexemata_ambulata, (i32)ZEPHYRUM);

    /* cauda: lexema EOF scriptum si plagulae huius est */
    cauda = (parsura->lexema_finis != NIHIL
             && parsura->lexema_finis->fons_index
                    == parsura->fons_princeps)
        ? (i32)I : (i32)ZEPHYRUM;

    /* tabulam percurrere: fetta-tag pro omni intranti */
    nodi_tabulae      = ZEPHYRUM;
    lexemata_tabulae  = ZEPHYRUM;
    fragmentum_visum  = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(scriptura.sedes_valorum);
         i++)
    {
        SilvaArborSedes* sedes_valoris;
              character  tag[SILVA_ARBOR_TAG_CAPACITAS];

        sedes_valoris = (SilvaArborSedes*)xar_obtinere(
            scriptura.sedes_valorum, i);
        si (sedes_valoris == NIHIL)
        {
            perge;
        }
        si (sedes_valoris->est_lexema)
        {
            constans SilvaToken* lexema;

            lexemata_tabulae = lexemata_tabulae + I;
            lexema = (constans SilvaToken*)sedes_valoris->clavis;
            si (silva_arbor_lexema_tag(lexema->genus, tag,
                    (i32)magnitudo(tag)) == ZEPHYRUM)
            {
                CREDO_VERUM (FALSUM);
                perge;
            }
        }
        alioquin
        {
            constans SilvaNodus* n;

            nodi_tabulae  = nodi_tabulae + I;
            n             = (constans SilvaNodus*)sedes_valoris->clavis;
            si ((i32)n->genus
                >= SILVA_C89_REGISTRUM.numerus_generum)
            {
                CREDO_VERUM (FALSUM);
                perge;
            }
            strcpy(tag,
                SILVA_C89_REGISTRUM.genera[n->genus].titulus);
        }
        CREDO_VERUM (_fettam_conferre(scriptura.textus,
            sedes_valoris->initium, sedes_valoris->finis, tag));
    }

    /* census: tabula nec deficiens nec superflua */
    CREDO_AEQUALIS_I32 (nodi_tabulae, nodi_ambulati);
    CREDO_AEQUALIS_I32 (lexemata_tabulae,
                        lexemata_ambulata + cauda);

    /* semita communicationis exercita ubi expectata */
    per (i = ZEPHYRUM;
         i + (i32)V < (i32)scriptura.textus.mensura; i++)
    {
        si (memcmp(scriptura.textus.datum + i, "<#lex",
                (memoriae_index)V) == ZEPHYRUM)
        {
            fragmentum_visum = VERUM;
            frange;
        }
    }
    si (fragmenta_expectata)
    {
        CREDO_VERUM (fragmentum_visum);
    }
    alioquin
    {
        CREDO_FALSUM (fragmentum_visum);
    }

    imprimere("nodi %d lexemata %d (+cauda %d) fragmenta %d\n",
        (integer)nodi_tabulae, (integer)lexemata_tabulae,
        (integer)cauda, (integer)fragmentum_visum);
}

s32
principale (
    vacuum)
{
    Piscina* piscina;

    /* simplex: sine communicatione lexematum */
    constans character* FIXTURA_SIMPLEX =
        "/* nota */\n"
        "int x;\n"
        "\n"
        "int y;\n";

    /* ambigua: 't * x;' oraculo NIHIL bracchia duo parit quae
     * lexemata EADEM ferunt -> fragmenta in documento */
    constans character* FIXTURA_AMBIGUA =
        "typedef int t;\n"
        "void f(void)\n"
        "{\n"
        "    t * x;\n"
        "}\n";

    piscina = piscina_generare_dynamicum(
        "probatio_silva_arbor_sedes", 16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    _fixturam_probare(piscina, "simplex", FIXTURA_SIMPLEX, FALSUM);
    _fixturam_probare(piscina, "ambigua", FIXTURA_AMBIGUA, VERUM);

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
