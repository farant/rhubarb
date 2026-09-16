/* probatio_crusta_arbor.c - Aedificator crustae, pars I
 *
 * Quisque casus: fons (in piscina) -> crusta_arbor_parsare ->
 * materia_scribere_nodum -> memcmp (ORACULUM SEPARANS: emissio
 * directa, sine STML), deinde assertiones structurae per locos.
 * Tabula locorum absentium (spec par. IV) ordine; regulae
 * positionales; mala totalitatis; ligator regulae inversae (C7);
 * valor staticus verbi.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

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

/* parsare + emissio octetim idem (asserta) */
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
    CREDO_VERUM (emissa.successus);
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

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus
        == MATERIA_VALOR_LISTA ? materia_valor_lista_numerus(*v)
        : ZEPHYRUM;
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

interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_TOKEN
        ? nodus->loci[locus].datum.token : NIHIL;
}

interior b32
_absens (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_NIHIL;
}

interior b32
_genus (
    constans MateriaNodus* nodus,
              CrustaGenus  genus)
{
    redde nodus != NIHIL && nodus->genus == (s32)genus;
}

interior b32
_tok_est (
    constans MateriaToken* t,
       constans character* litterae)
{
    redde t != NIHIL && t->valor.mensura == (i32)strlen(litterae)
        && memcmp(t->valor.datum, litterae, strlen(litterae))
            == ZEPHYRUM;
}

/* liberi programmatis */
interior MateriaNodus*
_sententia (
    constans MateriaNodus* programma,
                      i32  index)
{
    redde _filius(programma, (i32)CRUSTA_PROGRAMMA_LIBERI, index);
}

/* pars index verbi */
interior MateriaNodus*
_pars (
    constans MateriaNodus* verbum,
                      i32  index)
{
    redde _filius(verbum, (i32)CRUSTA_VERBUM_PARTES, index);
}

interior b32
_staticum_est (
                  Piscina* piscina,
    constans MateriaNodus* verbum,
       constans character* litterae)
{
    chorda v;

    si (!crusta_verbum_staticum(piscina, verbum, &v))
    {
        redde FALSUM;
    }
    redde v.mensura == (i32)strlen(litterae)
        && memcmp(v.datum, litterae, strlen(litterae)) == ZEPHYRUM;
}

s32
principale (vacuum)
{
              b32  praeteritus;
          Piscina* piscina;
    CrustaParsura  r;
    MateriaNodus* p;
    MateriaNodus* imperium;
    MateriaNodus* verbum;
    MateriaNodus* n;

    piscina = piscina_generare_dynamicum("probatio_crusta_arbor",
        524288);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: programma, imperium, verba
     * ================================================== */

    imprimere("\n--- Probans programma et imperia ---\n");

    p = _casus(piscina, "echo hi\n", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)II);
    imperium = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(imperium, CRUSTA_GENUS_IMPERIUM));
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)II);
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_SEPARATOR));
    CREDO_VERUM (_tok_est(_tok(_sententia(p, I),
        (i32)CRUSTA_SEPARATOR_TOK),
        "\n"));
    CREDO_NON_NIHIL (_tok(p, (i32)CRUSTA_PROGRAMMA_CAUDA));
    CREDO_VERUM (_staticum_est(piscina,
        crusta_imperium_titulus(imperium),
        "echo"));
    {
        Xar* argumenta = crusta_imperium_argumenta(piscina, imperium);

        CREDO_AEQUALIS_I32 (xar_numerus(argumenta), (i32)I);
        CREDO_VERUM (_staticum_est(piscina,
            *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM), "hi"));
    }
    CREDO_VERUM (r.sana);
    CREDO_AEQUALIS_I32 (r.mala, ZEPHYRUM);

    p = _casus(piscina, "", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        ZEPHYRUM);
    CREDO_NON_NIHIL (_tok(p, (i32)CRUSTA_PROGRAMMA_CAUDA));
    CREDO_VERUM (r.sana);

    /* verbum unum trium partium; verba duo */
    p         = _casus(piscina, "\"a\"b'c' d", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)II);
    verbum = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(verbum, (i32)CRUSTA_VERBUM_PARTES),
        (i32)III);
    CREDO_VERUM (_genus(_pars(verbum, ZEPHYRUM),
        CRUSTA_GENUS_PARS_GEMINA));
    CREDO_VERUM (_genus(_pars(verbum, I),
        CRUSTA_GENUS_PARS_LITTERALIS));
    CREDO_VERUM (_genus(_pars(verbum, II), CRUSTA_GENUS_PARS_SIMPLEX));
    CREDO_VERUM (_staticum_est(piscina, verbum, "abc"));
    CREDO_VERUM (crusta_verbum_citatum(verbum));
    CREDO_FALSUM (crusta_verbum_citatum(_filius(imperium,
        (i32)CRUSTA_IMPERIUM_LIBERI, I)));

    /* assignationes positionales; a=b post verbum = verbum */
    p         = _casus(piscina, "x=1 y=2 cmd a=b", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)IV);
    CREDO_VERUM (_genus(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), CRUSTA_GENUS_ASSIGNATIO));
    CREDO_VERUM (_genus(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        I),
        CRUSTA_GENUS_ASSIGNATIO));
    CREDO_VERUM (_genus(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        II),
        CRUSTA_GENUS_VERBUM));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, III);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (_staticum_est(piscina, n, "a=b"));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS),
        "x"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR),
        "="));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_ASSIGNATIO_VALOR, ZEPHYRUM), "1"));
    CREDO_VERUM (r.sana);

    /* 'A=1 if': verbum reservatum post assignationem = malum */
    p = _casus(piscina, "A=1 if", &r);
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);
    CREDO_FALSUM (r.sana);
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_MALUM));

    /* aedificator: x=(1 2) assignatio cum tabulato */
    p         = _casus(piscina, "declare -a x=(1 [3]=y)", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)III);
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ASSIGNATIO));
    n = _filius(n, (i32)CRUSTA_ASSIGNATIO_VALOR, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_TABULATUM));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_TABULATUM_LIBERI),
        (i32)II);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_TABULATUM_LIBERI,
        ZEPHYRUM),
        CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_TABULATUM_LIBERI, I),
        CRUSTA_GENUS_ASSIGNATIO));
    CREDO_VERUM (_absens(_filius(n, (i32)CRUSTA_TABULATUM_LIBERI, I),
        (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_TABULATUM_TOK_CLAUSURA),
        ")"));
    CREDO_VERUM (r.sana);

    /* a= valor absens; a+=b; a[i]=v */
    p = _casus(piscina, "a= b+=c d[i]=v", &r);
    imperium = _sententia(p, ZEPHYRUM);
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ASSIGNATIO_VALOR));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR),
        "+="));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, II);
    CREDO_VERUM (_tok_est(_tok(n,
        (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM),
        "[i]"));
    CREDO_VERUM (r.sana);


    /* ==================================================
     * PROBARE: partes
     * ================================================== */

    imprimere("\n--- Probans partes ---\n");

    p = _casus(piscina, "$x ${y:-d} $(echo z) \\$ ~/p $'\\t' $\"s\"",
        &r);
    imperium = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)VII);
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM),
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_PARAMETRUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_PARAMETRUM_TOK_TITULUS),
        "x"));
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I),
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_EXPANSIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_TITULUS),
        "y"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_OPERATOR),
        ":-"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA),
        (i32)I);
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA, ZEPHYRUM), "d"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_CLAUSURA),
        "}"));
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, II),
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI),
        (i32)I);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI,
        ZEPHYRUM),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA),
        ")"));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, III);
    CREDO_VERUM (_genus(_pars(n, ZEPHYRUM),
        CRUSTA_GENUS_PARS_EFFUGIUM));
    CREDO_VERUM (_staticum_est(piscina, n, "$"));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, IV);
    CREDO_VERUM (_genus(_pars(n, ZEPHYRUM), CRUSTA_GENUS_PARS_DOMUS));
    CREDO_VERUM (_genus(_pars(n, I), CRUSTA_GENUS_PARS_LITTERALIS));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, V);
    CREDO_VERUM (_genus(_pars(n, ZEPHYRUM), CRUSTA_GENUS_PARS_EFFUGIA));
    CREDO_VERUM (_staticum_est(piscina, n, "\t"));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, VI);
    CREDO_VERUM (_genus(_pars(n, ZEPHYRUM), CRUSTA_GENUS_PARS_VERSA));
    CREDO_VERUM (_staticum_est(piscina, n, "s"));
    CREDO_VERUM (r.sana);

    /* expansio: argumenta cum divisoribus; praefixum */
    p = _casus(piscina, "${x/a/b}${x:1:2}${#x}${x}", &r);
    verbum = _filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(verbum, (i32)CRUSTA_VERBUM_PARTES),
        (i32)IV);
    n = _pars(verbum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA),
        (i32)III);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA, I),
        CRUSTA_GENUS_OPERATOR));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA, II), "b"));
    n = _pars(verbum, I);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_OPERATOR),
        ":"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA),
        (i32)III);
    n = _pars(verbum, II);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM),
        "#"));
    n = _pars(verbum, III);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_TOK_OPERATOR));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA));
    CREDO_FALSUM (_staticum_est(piscina, verbum, ""));
    CREDO_VERUM (r.sana);

    /* ${x:-}: operator praesens, argumenta absentia */
    p = _casus(piscina, "${x:-}", &r);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_OPERATOR),
        ":-"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA));

    /* gemina cum substitutione cum gemina */
    p = _casus(piscina, "\"$(echo \"in\")\"", &r);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_GEMINA));
    n = _filius(n, (i32)CRUSTA_GEMINA_PARTES, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    n = _filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IMPERIUM));
    n = _pars(_filius(n, (i32)CRUSTA_IMPERIUM_LIBERI, I), ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_GEMINA));
    CREDO_VERUM (r.sana);

    /* backtick nidificatus per effugia */
    p         = _casus(piscina, "echo `echo \\`echo d\\`` x", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)III);
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I),
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA),
        "`"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA),
        "`"));
    n = _filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI, ZEPHYRUM);
    n = _pars(_filius(n, (i32)CRUSTA_IMPERIUM_LIBERI, I), ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA),
        "\\`"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA),
        "\\`"));
    CREDO_VERUM (r.sana);

    /* adiacentia: a$(b)c verbum unum, a $(b) duo */
    p         = _casus(piscina, "a$(b)c", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)I);
    CREDO_AEQUALIS_I32 (_numerus(_filius(imperium,
        (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_VERBUM_PARTES),
        (i32)III);
    p = _casus(piscina, "a $(b)", &r);
    CREDO_AEQUALIS_I32 (_numerus(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI), (i32)II);


    /* ==================================================
     * PROBARE: redirectiones
     * ================================================== */

    imprimere("\n--- Probans redirectiones ---\n");

    p = _casus(piscina, "cmd 2>&1 >/dev/null <in &>all {fd}>x", &r);
    imperium = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)VI);
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_REDIRECTIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_REDIRECTIO_TOK_FD), "2"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR),
        ">&"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_REDIRECTIO_SCOPUS, ZEPHYRUM), "1"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_REDIRECTIO_CORPUS));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, II);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_REDIRECTIO_TOK_FD));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_REDIRECTIO_SCOPUS, ZEPHYRUM),
        "/dev/null"));
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, V);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_REDIRECTIO_TOK_FD),
        "{fd}"));
    CREDO_VERUM (r.sana);

    /* redirectio sola: imperium sine verbo */
    p         = _casus(piscina, "> f", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(imperium, CRUSTA_GENUS_IMPERIUM));
    CREDO_NIHIL (crusta_imperium_titulus(imperium));
    CREDO_VERUM (_genus(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), CRUSTA_GENUS_REDIRECTIO));
    CREDO_VERUM (r.sana);

    /* <<A: petitio heredoc numerata (corpus P5) */
    p = _casus(piscina, "cat <<A <<<w", &r);
    CREDO_AEQUALIS_I32 (r.heredoca, (i32)I);
    CREDO_AEQUALIS_I32 (_numerus(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI), (i32)III);


    /* ==================================================
     * PROBARE: listae - separatores, catena, pipa
     * ================================================== */

    imprimere("\n--- Probans listas ---\n");

    p = _casus(piscina, "a; b & c\n", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)VI);
    CREDO_VERUM (_genus(_sententia(p, ZEPHYRUM),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_tok_est(_tok(_sententia(p, I),
        (i32)CRUSTA_SEPARATOR_TOK),
        ";"));
    CREDO_VERUM (_tok_est(_tok(_sententia(p, III),
        (i32)CRUSTA_SEPARATOR_TOK),
        "&"));
    CREDO_VERUM (_tok_est(_tok(_sententia(p, V),
        (i32)CRUSTA_SEPARATOR_TOK),
        "\n"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "a && b || c", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)I);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CATENA_LIBERI), (i32)V);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CATENA_LIBERI, I),
        CRUSTA_GENUS_OPERATOR));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_CATENA_LIBERI,
        III),
        (i32)CRUSTA_OPERATOR_TOK), "||"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "a | b |& c && d", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CATENA_LIBERI),
        (i32)III);
    n = _filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_LIBERI), (i32)V);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_PIPA_LIBERI, III),
        (i32)CRUSTA_OPERATOR_TOK), "|&"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_PIPA_PRAEFIXA));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "! a | b; time -p c", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_PRAEFIXA), (i32)I);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_LIBERI), (i32)III);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_PRAEFIXA),
        (i32)II);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_LIBERI), (i32)I);
    CREDO_VERUM (r.sana);


    /* ==================================================
     * PROBARE: ligator (C7) - trivia retro per lineam novam ultimam
     * ================================================== */

    imprimere("\n--- Probans ligatorem ---\n");

    p = _casus(piscina, "a &&\n\n  b", &r);
    n = _sententia(p, ZEPHYRUM);
    {
        MateriaToken* signum = _tok(_filius(n,
            (i32)CRUSTA_CATENA_LIBERI,
            I),
            (i32)CRUSTA_OPERATOR_TOK);
        MateriaToken* b = _tok(_pars(_filius(_filius(n,
            (i32)CRUSTA_CATENA_LIBERI, II), (i32)CRUSTA_IMPERIUM_LIBERI,
            ZEPHYRUM), ZEPHYRUM), (i32)CRUSTA_PARS_TOK);

        CREDO_AEQUALIS_I32 (signum->numerus_post, (i32)II);
        CREDO_AEQUALIS_I32 (signum->numerus_ante, (i32)I);
        CREDO_AEQUALIS_I32 (b->numerus_ante, (i32)I);
        CREDO_AEQUALIS_I32 (b->numerus_post, ZEPHYRUM);
    }

    p = _casus(piscina, "a # c\n", &r);
    n = _sententia(p, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_SEPARATOR));
    CREDO_AEQUALIS_I32 (_tok(n,
        (i32)CRUSTA_SEPARATOR_TOK)->numerus_ante,
        (i32)II);

    p = _casus(piscina, "#!/bin/bash\n\na", &r);
    n = _tok(_pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM), ZEPHYRUM),
        (i32)CRUSTA_PARS_TOK) == NIHIL ? NIHIL : _sententia(p,
        ZEPHYRUM);
    CREDO_NON_NIHIL (n);
    CREDO_AEQUALIS_I32 (_tok(_pars(_filius(n,
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), ZEPHYRUM), (i32)CRUSTA_PARS_TOK)->numerus_ante,
        (i32)III);

    p = _casus(piscina, "a # c", &r);
    CREDO_AEQUALIS_I32 (_tok(p,
        (i32)CRUSTA_PROGRAMMA_CAUDA)->numerus_ante,
        (i32)II);

    p         = _casus(piscina, "a \\\n b", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)II);
    CREDO_AEQUALIS_I32 (_tok(_pars(_filius(imperium,
        (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM), ZEPHYRUM),
        (i32)CRUSTA_PARS_TOK)->numerus_post, (i32)II);


    /* ==================================================
     * PROBARE: mala (totalitas) et EOF
     * ================================================== */

    imprimere("\n--- Probans mala et EOF ---\n");

    {
        constans character* mala[] = { "fi", ")", ";;", "}", "; ;",
            "then",
            "a )", NIHIL };
        i32 i;

        per (i = ZEPHYRUM; mala[i] != NIHIL; i++)
        {
            p = _casus(piscina, mala[i], &r);
            CREDO_MAIOR_I32 (r.mala, ZEPHYRUM);
            CREDO_FALSUM (r.sana);
        }
    }
    /* 'a )' : imperium clausum, malum post */
    p = _casus(piscina, "a ) b", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)III);
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_MALUM));
    CREDO_VERUM (_genus(_sententia(p, II), CRUSTA_GENUS_IMPERIUM));

    p = _casus(piscina, "\"abc", &r);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);
    CREDO_FALSUM (r.sana);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), ZEPHYRUM);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_GEMINA_TOK_CLAUSURA));

    p = _casus(piscina, "$(a", &r);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        ZEPHYRUM), ZEPHYRUM);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA));

    p = _casus(piscina, "`a", &r);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);

    /* $ solus litteralis; '$((' interim pars litteralis, '))' mala
     * (intermedium nominatum: P4 arithmeticam dat) */
    p = _casus(piscina, "echo $ $((1+1))", &r);
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);


    /* ==================================================
     * PROBARE: valor staticus
     * ================================================== */

    imprimere("\n--- Probans valorem staticum ---\n");

    p = _casus(piscina,
        "'a b' \"a\\\"b\" a\\ b $'\\x41\\101' \\\\x \"\\x\"",
        &r);
    imperium = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM),
        "a b"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I), "a\"b"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, II), "a b"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, III), "AA"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, IV), "\\x"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, V), "\\x"));


    imprimere("\n    casus %d\n", (integer)casus_numerus);
    CREDO_MAIOR_I32 (casus_numerus, (i32)XXX);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
