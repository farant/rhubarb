/* probatio_crusta_arithmetica.c - Machina praecedentiae et aestimator
 *
 * (a) STRUCTURA per crusta_arbor_parsare super '$(( ... ))': forma
 *     arboris (praecedentia, associatio, praeposita artius quam '**',
 *     ternaria, comma, assignationes, inclusa, folia partes), omnia
 *     octetim per emissorem; '(( ))' ut imperium; ambiguitas '$(('
 *     (reversio ad substitutionem).
 * (b) AESTIMATOR contra valores bash 5.2.15 (2026-09-16, computati
 *     '/opt/homebrew/bin/bash -c echo $((e))'): XLIII ordines - forma
 *     arboris probatur per valorem, quia deparse bash arithmeticam
 *     verbatim imprimit (spec par. VI).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_arithmetica.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    constans character* fons;
                   s64  valor;
} Aestimandum;

hic_manens constans Aestimandum AESTIMANDA[] = {
    { "1+2*3", (s64)7 }, { "(1+2)*3", (s64)9 }, { "2**3**2", (s64)512 },
    { "-2**2", (s64)4 }, { "10/3", (s64)3 }, { "10%3", (s64)1 },
    { "-7/2", (s64)-3 }, { "-7%2", (s64)-1 }, { "1<<4", (s64)16 },
    { "256>>2", (s64)64 }, { "5&3", (s64)1 }, { "5|3", (s64)7 },
    { "5^3", (s64)6 }, { "~5", (s64)-6 }, { "!0", (s64)1 }, { "!5",
        (s64)0 },
    { "1&&0", (s64)0 }, { "1||0", (s64)1 }, { "0||0&&1", (s64)0 },
    { "1?2:3", (s64)2 }, { "0?2:3", (s64)3 }, { "1?0?4:5:6", (s64)5 },
    { "0x1f", (s64)31 }, { "010", (s64)8 }, { "2#1010", (s64)10 },
    { "16#ff", (s64)255 }, { "36#zz", (s64)1295 }, { "1,2,3", (s64)3 },
    { "3>2", (s64)1 }, { "2>=3", (s64)0 }, { "1==1", (s64)1 },
    { "1!=1", (s64)0 }, { "-(3)", (s64)-3 }, { "+4", (s64)4 },
    { "9223372036854775807+1", (s64)(-9223372036854775807LL - 1LL) },
    { "-9223372036854775808-1", (s64)9223372036854775807LL },
    { "1+2<<1", (s64)6 }, { "6/2*3", (s64)9 }, { "2*3%4", (s64)2 },
    { "7-3-2", (s64)2 }, { "1<2==1", (s64)1 }, { "(2)", (s64)2 },
    { "- - 3", (s64)3 }
};

hic_manens i32 casus_numerus = ZEPHYRUM;

interior constans character*
_copia (
               Piscina* piscina,
    constans character* litterae,
                   i32* mensura)
{
          i32  n = (i32)strlen(litterae);
    character* m = (character*)piscina_allocare(piscina, n + I);

    memcpy(m, litterae, (size_t)n + I);
    *mensura = n;
    redde m;
}

interior MateriaNodus*
_casus (
               Piscina* piscina,
    constans character* litterae,
         CrustaParsura* relatio)
{
                   i32  mensura;
    constans character* fons = _copia(piscina, litterae, &mensura);
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;
    b32 idem;

    casus_numerus++;
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &CRUSTA_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    idem = emissa.successus && emissa.textus.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.textus.datum, fons, (size_t)mensura)
                == ZEPHYRUM);
    si (!idem)
    {
        imprimere("    casus %d '%s': emissio '%.*s'\n",
            (integer)casus_numerus, litterae,
            (integer)emissa.textus.mensura,
            (constans character*)emissa.textus.datum);
    }
    CREDO_VERUM (idem);
    redde radix;
}

interior MateriaNodus*
_filius (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  index)
{
    constans MateriaValor* v = &nodus->loci[locus];
    constans MateriaValor* e;

    si (v->genus == MATERIA_VALOR_NODUS)
    {
        redde index == ZEPHYRUM ? v->datum.nodus : NIHIL;
    }
    si (   v->genus != MATERIA_VALOR_LISTA
        || index    >= materia_valor_lista_numerus(*v))
    {
        redde NIHIL;
    }
    e = materia_valor_lista_obtinere(*v, index);
    redde e->genus == MATERIA_VALOR_NODUS ? e->datum.nodus : NIHIL;
}

interior b32
_tok_est (
    constans MateriaNodus* nodus,
                      i32  locus,
       constans character* litterae)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus == MATERIA_VALOR_TOKEN
        && v->datum.token->valor.mensura == (i32)strlen(litterae)
        && memcmp(v->datum.token->valor.datum, litterae,
        strlen(litterae))
            == ZEPHYRUM;
}

interior b32
_genus (
    constans MateriaNodus* nodus,
              CrustaGenus  genus)
{
    redde nodus != NIHIL && nodus->genus == (s32)genus;
}

/* expressio partis '$((' verbi primi imperii primi */
interior MateriaNodus*
_expressio (
    constans MateriaNodus* programma)
{
    MateriaNodus* n = _filius(programma, (i32)CRUSTA_PROGRAMMA_LIBERI,
        ZEPHYRUM);

    n = _filius(n, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM);
    n = _filius(n, (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM);
    si (!_genus(n, CRUSTA_GENUS_PARS_ARITHMETICA))
    {
        redde NIHIL;
    }
    redde _filius(n, (i32)CRUSTA_PARS_ARITHMETICA_EXPRESSIO, ZEPHYRUM);
}

interior MateriaNodus*
_expressio_ex (
               Piscina* piscina,
    constans character* interior_textus)
{
        character f[128];
    CrustaParsura r;

    sprintf(f, "$((%s))", interior_textus);
    redde _expressio(_casus(piscina, f, &r));
}

s32
principale (vacuum)
{
              b32  praeteritus;
          Piscina* piscina;
    CrustaParsura  r;
    MateriaNodus* p;
    MateriaNodus* e;
    MateriaNodus* n;

    piscina = piscina_generare_dynamicum("probatio_crusta_arithmetica",
        524288);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: structura
     * ================================================== */

    imprimere("\n--- Probans structuram ---\n");

    /* 1+2*3: binaria(+, 1, binaria(*, 2, 3)) */
    e = _expressio_ex(piscina, "1+2*3");
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR, "+"));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM),
        CRUSTA_GENUS_NUMERUS));
    n = _filius(e, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR, "*"));

    /* 2**3**2 dextra: binaria(**, 2, binaria(**, 3, 2)) */
    e = _expressio_ex(piscina, "2**3**2");
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_DEXTER,
        ZEPHYRUM),
        CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM),
        CRUSTA_GENUS_NUMERUS));

    /* -2**2: binaria(**, praeposita(-, 2), 2) */
    e = _expressio_ex(piscina, "-2**2");
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM),
        CRUSTA_GENUS_PRAEPOSITA));

    /* a++ + ++b */
    e = _expressio_ex(piscina, "a++ + ++b");
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR, "+"));
    n = _filius(e, (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_POSTPOSITA));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_POSTPOSITA_OPERANDUM,
        ZEPHYRUM), CRUSTA_GENUS_VARIABILIS));
    n = _filius(e, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PRAEPOSITA));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR,
        "++"));

    /* x = y += 2 dextra */
    e = _expressio_ex(piscina, "x = y += 2");
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR, "="));
    n = _filius(e, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM);
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR, "+="));

    /* comma; ternaria; inclusa; subscriptum; partes ut folia */
    e = _expressio_ex(piscina, "1,2");
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR, ","));
    e = _expressio_ex(piscina, "c ? a : b");
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_TERNARIA));
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_TERNARIA_TOK_COLON, ":"));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_TERNARIA_DEXTER,
        ZEPHYRUM),
        CRUSTA_GENUS_VARIABILIS));
    e = _expressio_ex(piscina, "(1+2)*3");
    n = _filius(e, (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_INCLUSA));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_INCLUSA_TOK_CLAUSURA, ")"));
    e = _expressio_ex(piscina, "a[i+1]");
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_VARIABILIS));
    CREDO_VERUM (_tok_est(e, (i32)CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM,
        "[i+1]"));
    e = _expressio_ex(piscina, "$x + ${y} + $(cmd)");
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    n = _filius(e, (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM),
        CRUSTA_GENUS_PARS_PARAMETRUM));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_BINARIA_DEXTER,
        ZEPHYRUM),
        CRUSTA_GENUS_PARS_EXPANSIO));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_DEXTER,
        ZEPHYRUM),
        CRUSTA_GENUS_PARS_SUBSTITUTIO));

    /* vacua: expressio absens */
    p = _casus(piscina, "$(( ))", &r);
    n = _filius(_filius(_filius(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_VERBUM_PARTES,
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_ARITHMETICA));
    CREDO_NIHIL (_filius(n, (i32)CRUSTA_PARS_ARITHMETICA_EXPRESSIO,
        ZEPHYRUM));
    CREDO_VERUM (r.sana);

    /* (( )) ut imperium, cum redirectione et in catena */
    p = _casus(piscina, "(( i += 1 )) > f && x", &r);
    n = _filius(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    n = _filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ARITHMETICA));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_ARITHMETICA_TOK_CLAUSURA,
        "))"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_ARITHMETICA_EXPRESSIO,
        ZEPHYRUM),
        CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(n,
        (i32)CRUSTA_ARITHMETICA_REDIRECTIONES,
        ZEPHYRUM), CRUSTA_GENUS_REDIRECTIO));
    CREDO_VERUM (r.sana);

    /* ambiguitas: $((1+1)) arithmetica, $((echo a); b) substitutio */
    p = _casus(piscina, "echo $((1+1)) $((echo a); b)", &r);
    n = _filius(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(_filius(_filius(n, (i32)CRUSTA_IMPERIUM_LIBERI,
        I),
        (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM),
        CRUSTA_GENUS_PARS_ARITHMETICA));
    n = _filius(_filius(n, (i32)CRUSTA_IMPERIUM_LIBERI, II),
        (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA,
        "$("));
    CREDO_VERUM (_tok_est(n, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA,
        ")"));
    /* trivia aperturae servata: ' ' ante '$(' */
    {
        MateriaToken* apertura =
            n->loci[CRUSTA_SUBSTITUTIO_TOK_APERTURA].datum.token;

        CREDO_AEQUALIS_I32 (apertura->numerus_ante, (i32)I);
    }

    /* operandum absens: '1 +' -> binaria dexter absens, mala */
    p = _casus(piscina, "$((1 +))", &r);
    CREDO_MAIOR_I32 (r.mala, ZEPHYRUM);
    e = _expressio(p);
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    CREDO_NIHIL (_filius(e, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM));

    /* iuxtapositio: 'a b' -> binaria operatore ABSENTE, malum; signa
     * priora ante reducuntur ('-a b' = binaria(praeposita, _, b)).
     * INVENTUM P9 (porta totalitatis): operandum prius in finire olim
     * perdebatur - emissio '$(( b))' */
    p = _casus(piscina, "$((a b))", &r);
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);
    e = _expressio(p);
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM), CRUSTA_GENUS_VARIABILIS));
    CREDO_VERUM (e != NIHIL
        && e->loci[CRUSTA_BINARIA_TOK_OPERATOR].genus
        == MATERIA_VALOR_NIHIL);
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_DEXTER,
        ZEPHYRUM), CRUSTA_GENUS_VARIABILIS));
    p = _casus(piscina, "$((-a b c))", &r);
    e = _expressio(p);
    CREDO_VERUM (_genus(e, CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM), CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(_filius(e, (i32)CRUSTA_BINARIA_SINISTER,
        ZEPHYRUM), (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM),
        CRUSTA_GENUS_PRAEPOSITA));
    p = _casus(piscina, "(( a b ))", &r);
    p = _casus(piscina, "((a\ti))", &r);

    /* $(( ad EOF: clausura absens */
    p = _casus(piscina, "$((1+1", &r);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);


    /* ==================================================
     * PROBARE: aestimator contra bash
     * ================================================== */

    imprimere("\n--- Probans aestimatorem ---\n");

    {
        i32 i;
        i32 aestimati = ZEPHYRUM;
        i32 numerus = (i32)(magnitudo(AESTIMANDA)
            / magnitudo(AESTIMANDA[0]));

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            s64 v = ZEPHYRUM;
            b32 bene;

            e     = _expressio_ex(piscina, AESTIMANDA[i].fons);
            bene  = crusta_arithmetica_aestimare(e, &v);
            si (!bene || v != AESTIMANDA[i].valor)
            {
                imprimere("    '%s': %ld exspectatum, %ld receptum"
                    " (%s)\n",
                    AESTIMANDA[i].fons, (longus)AESTIMANDA[i].valor,
                    (longus)v, bene ? "aestimatum" : "RECUSATUM");
            }
            CREDO_VERUM (bene);
            CREDO_VERUM (v == AESTIMANDA[i].valor);
            aestimati++;
        }
        CREDO_AEQUALIS_I32 (aestimati, (i32)XLIII);
    }

    /* recusationes */
    {
        s64 v;

        CREDO_FALSUM (crusta_arithmetica_aestimare(
            _expressio_ex(piscina, "1/0"), &v));
        CREDO_FALSUM (crusta_arithmetica_aestimare(
            _expressio_ex(piscina, "1%0"), &v));
        CREDO_FALSUM (crusta_arithmetica_aestimare(
            _expressio_ex(piscina, "2**-1"), &v));
        CREDO_FALSUM (crusta_arithmetica_aestimare(
            _expressio_ex(piscina, "$x+1"), &v));
        CREDO_FALSUM (crusta_arithmetica_aestimare(
            _expressio_ex(piscina, "a=1"), &v));
        CREDO_FALSUM (crusta_arithmetica_aestimare(NIHIL, &v));
    }

    imprimere("\n    casus %d\n", (integer)casus_numerus);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
