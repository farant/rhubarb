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

/* nidus 'apertura' x profunditas apertus ad EOF, parsatus in piscina
 * sua (tempus sub CREDO_NON_PENDET mensuratur) */
interior vacuum
_nidum_parsare (
    constans character* apertura,
                   i32  profunditas)
{
    Piscina* piscina = piscina_generare_dynamicum("crusta_nidus",
        1048576);
                   i32  la       = (i32)strlen(apertura);
                   i32  mensura  = la * profunditas;
             character* fons;
         CrustaParsura  r;
                   i32  i;

    si (piscina == NIHIL)
    {
        redde;
    }
    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    si (fons != NIHIL)
    {
        per (i = ZEPHYRUM; i < profunditas; i++)
        {
            memcpy(fons + i * la, apertura, (size_t)la);
        }
        crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH, &r);
    }
    piscina_destruere(piscina);
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

    /* 'A=1 if': post assignationem modus ASSIGNATIONES - 'if' verbum
     * nudum LITTERALE (bash 5.2 mensum: 'A=1 if b' sanum); 'X=1' +
     * linea nova imperium finit (corpus P6: 'OBJ=""' ante 'for') */
    p         = _casus(piscina, "A=1 if", &r);
    imperium  = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(imperium, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)II);
    n = _filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_VERBUM));
    CREDO_AEQUALIS_S32 (_tok(_pars(n, ZEPHYRUM),
        (i32)CRUSTA_PARS_TOK)->genus, (s32)CRUSTA_LEX_LITTERALIS);
    CREDO_VERUM (_staticum_est(piscina, n, "if"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "X=1\nfor i in a; do :; done\n>f\necho", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)VII);
    CREDO_VERUM (_genus(_sententia(p, II), CRUSTA_GENUS_ITERATIO));
    CREDO_VERUM (_genus(_sententia(p, IV), CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_sententia(p, VI), CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (r.sana);

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

    /* titulus solum post '${' aut praefixum; '[' post ']' non
     * subscriptum. INVENTUM P9 (porta totalitatis): littera quaelibet
     * titulus erat - '${@E}', '${E[]t}', '${x[1][2]}' locum bis
     * scribebant (parsator NIHIL). bash -n omnia accipit: 'bad
     * substitution' tempore currendi. */
    p = _casus(piscina, "echo ${@E} ${E[]t} ${x[1]y} ${x[1][2]} ${#@}",
        &r);
    CREDO_AEQUALIS_I32 (r.mala, ZEPHYRUM);
    imperium = _sententia(p, ZEPHYRUM);
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I),
        ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_TITULUS),
        "@"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA, ZEPHYRUM), "E"));
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, IV),
        ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM),
        "[1]"));
    CREDO_VERUM (_staticum_est(piscina,
        _filius(n, (i32)CRUSTA_EXPANSIO_ARGUMENTA, ZEPHYRUM), "[2]"));
    n = _pars(_filius(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, V),
        ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM),
        "#"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_EXPANSIO_TOK_TITULUS),
        "@"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_EXPANSIO_TOK_OPERATOR));

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
        /* vexillum initium_lineae per regulam lectoris STML (P7): 'a'
         * documentum incipit, 'b' post trivia LINEA lineam incipit,
         * '&&' non */
        CREDO_VERUM (materia_token_initium_lineae(_tok(_pars(_filius(
            _filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM),
            (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM), ZEPHYRUM),
            (i32)CRUSTA_PARS_TOK)));
        CREDO_VERUM (materia_token_initium_lineae(b));
        CREDO_FALSUM (materia_token_initium_lineae(signum));
    }

    p = _casus(piscina, "a # c\n", &r);
    n = _sententia(p, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_SEPARATOR));
    /* separator linea nova substantivus: FINIS lineam NON incipit */
    CREDO_FALSUM (materia_token_initium_lineae(_tok(p,
        (i32)CRUSTA_PROGRAMMA_CAUDA)));
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

    /* $ solus litteralis; '$((1+1))' pars arithmetica (P4) */
    p = _casus(piscina, "echo $ $((1+1))", &r);
    CREDO_VERUM (r.sana);


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


    /* ==================================================
     * PROBARE: conditio et rami (P5)
     * ================================================== */

    imprimere("\n--- Probans conditionem ---\n");

    p = _casus(piscina,
        "if a; then b; elif c; then d; else e; fi", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CONDITIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CONDITIO_TOK_APERTURA),
        "if"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_PROBATIO),
        (i32)II);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CONDITIO_PROBATIO,
        ZEPHYRUM), CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CONDITIO_PROBATIO, I),
        CRUSTA_GENUS_SEPARATOR));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CONDITIO_TOK_DEINDE),
        "then"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_LIBERI),
        (i32)II);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_RAMI),
        (i32)II);
    {
        MateriaNodus* ramus = _filius(n, (i32)CRUSTA_CONDITIO_RAMI,
            ZEPHYRUM);

        CREDO_VERUM (_genus(ramus, CRUSTA_GENUS_RAMUS));
        CREDO_VERUM (_tok_est(_tok(ramus,
            (i32)CRUSTA_RAMUS_TOK_APERTURA),
            "elif"));
        CREDO_AEQUALIS_I32 (_numerus(ramus, (i32)CRUSTA_RAMUS_PROBATIO),
            (i32)II);
        CREDO_VERUM (_tok_est(_tok(ramus, (i32)CRUSTA_RAMUS_TOK_DEINDE),
            "then"));
        CREDO_AEQUALIS_I32 (_numerus(ramus, (i32)CRUSTA_RAMUS_LIBERI),
            (i32)II);
        ramus = _filius(n, (i32)CRUSTA_CONDITIO_RAMI, I);
        CREDO_VERUM (_tok_est(_tok(ramus,
            (i32)CRUSTA_RAMUS_TOK_APERTURA),
            "else"));
        CREDO_VERUM (_absens(ramus, (i32)CRUSTA_RAMUS_PROBATIO));
        CREDO_VERUM (_absens(ramus, (i32)CRUSTA_RAMUS_TOK_DEINDE));
        CREDO_AEQUALIS_I32 (_numerus(ramus, (i32)CRUSTA_RAMUS_LIBERI),
            (i32)II);
    }
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CONDITIO_TOK_CLAUSURA),
        "fi"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_CONDITIO_REDIRECTIONES));
    CREDO_VERUM (r.sana);

    /* linea nova post 'a' separator in probatione, post 'then'
     * trivium */
    p = _casus(piscina, "if a\nthen\n b\nfi", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_PROBATIO),
        (i32)II);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_CONDITIO_PROBATIO,
        I), (i32)CRUSTA_SEPARATOR_TOK), "\n"));
    CREDO_AEQUALIS_I32 (_tok(n,
        (i32)CRUSTA_CONDITIO_TOK_DEINDE)->numerus_post, (i32)I);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_LIBERI),
        (i32)II);
    CREDO_VERUM (r.sana);

    /* grex non clausus intra: 'fi' grecem absentem claudit (regula
     * html: lexema clausurae proprium tantum) */
    p = _casus(piscina, "if a; then { b; fi", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CONDITIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CONDITIO_TOK_CLAUSURA),
        "fi"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CONDITIO_LIBERI,
        ZEPHYRUM),
        CRUSTA_GENUS_GREX));
    CREDO_VERUM (_absens(_filius(n, (i32)CRUSTA_CONDITIO_LIBERI,
        ZEPHYRUM), (i32)CRUSTA_GREX_TOK_CLAUSURA));
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);
    CREDO_FALSUM (r.sana);

    p = _casus(piscina, "if", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CONDITIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_CONDITIO_PROBATIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_CONDITIO_TOK_DEINDE));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_CONDITIO_TOK_CLAUSURA));
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);

    /* post '))' et ']]' bash 'then' reservatum agnoscit */
    p = _casus(piscina, "if ((x)) then :; fi; if [[ a ]] then :; fi",
        &r);
    CREDO_VERUM (r.sana);
    CREDO_VERUM (_genus(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_CONDITIO_PROBATIO, ZEPHYRUM),
        CRUSTA_GENUS_ARITHMETICA));
    CREDO_VERUM (_genus(_filius(_sententia(p, II),
        (i32)CRUSTA_CONDITIO_PROBATIO, ZEPHYRUM),
        CRUSTA_GENUS_IUDICIUM));

    p = _casus(piscina, "if a; then b; fi > f | c", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    n = _filius(n, (i32)CRUSTA_PIPA_LIBERI, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CONDITIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_REDIRECTIONES),
        (i32)I);
    CREDO_VERUM (r.sana);


    /* ==================================================
     * PROBARE: iteratio, cyclus, repetitio, cursus
     * ================================================== */

    imprimere("\n--- Probans cyclos ---\n");

    p = _casus(piscina, "for i in a b; do c; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ITERATIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ITERATIO_TOK_APERTURA),
        "for"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ITERATIO_TOK_TITULUS),
        "i"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ITERATIO_TOK_IN), "in"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_ITERATIO_VERBA),
        (i32)II);
    CREDO_VERUM (_tok_est(_tok(_filius(n,
        (i32)CRUSTA_ITERATIO_SEPARATOR,
        ZEPHYRUM), (i32)CRUSTA_SEPARATOR_TOK), ";"));
    {
        MateriaNodus* cursus = _filius(n, (i32)CRUSTA_ITERATIO_CURSUS,
            ZEPHYRUM);

        CREDO_VERUM (_genus(cursus, CRUSTA_GENUS_CURSUS));
        CREDO_VERUM (_tok_est(_tok(cursus,
            (i32)CRUSTA_CURSUS_TOK_APERTURA), "do"));
        CREDO_AEQUALIS_I32 (_numerus(cursus, (i32)CRUSTA_CURSUS_LIBERI),
            (i32)II);
        CREDO_VERUM (_tok_est(_tok(cursus,
            (i32)CRUSTA_CURSUS_TOK_CLAUSURA), "done"));
    }
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for i in a b\ndo c; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(_filius(n,
        (i32)CRUSTA_ITERATIO_SEPARATOR,
        ZEPHYRUM), (i32)CRUSTA_SEPARATOR_TOK), "\n"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for i; do :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_TOK_IN));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_VERBA));
    CREDO_VERUM (_tok_est(_tok(_filius(n,
        (i32)CRUSTA_ITERATIO_SEPARATOR,
        ZEPHYRUM), (i32)CRUSTA_SEPARATOR_TOK), ";"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for i do :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_SEPARATOR));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_ITERATIO_CURSUS,
        ZEPHYRUM), CRUSTA_GENUS_CURSUS));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for ((i=0;i<3;i++)); do :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CYCLUS));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CYCLUS_TOK_APERTURA),
        "for"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS),
        "(("));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CYCLUS_LIBERI), (i32)V);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CYCLUS_LIBERI, ZEPHYRUM),
        CRUSTA_GENUS_BINARIA));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CYCLUS_LIBERI, I),
        CRUSTA_GENUS_OPERATOR));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CYCLUS_LIBERI, IV),
        CRUSTA_GENUS_POSTPOSITA));
    CREDO_VERUM (_tok_est(_tok(n,
        (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA), "))"));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_CYCLUS_SEPARATOR,
        ZEPHYRUM), (i32)CRUSTA_SEPARATOR_TOK), ";"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CYCLUS_CURSUS, ZEPHYRUM),
        CRUSTA_GENUS_CURSUS));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for ((;;)) do :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CYCLUS));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CYCLUS_LIBERI),
        (i32)II);
    CREDO_VERUM (_absens(n, (i32)CRUSTA_CYCLUS_SEPARATOR));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "select x in a; do :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ITERATIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ITERATIO_TOK_APERTURA),
        "select"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "while :; do break; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_REPETITIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_REPETITIO_TOK_APERTURA),
        "while"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_REPETITIO_PROBATIO),
        (i32)II);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_REPETITIO_CURSUS,
        ZEPHYRUM), CRUSTA_GENUS_CURSUS));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "until a; do b; done | c", &r);
    n = _filius(_sententia(p, ZEPHYRUM), (i32)CRUSTA_PIPA_LIBERI,
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_REPETITIO));
    CREDO_VERUM (r.sana);

    /* 'for' ad EOF: cursus absens numeratur */
    p = _casus(piscina, "for i in a", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ITERATIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_CURSUS));
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);

    /* 'while true do' : 'do' verbum, 'done' malum (bash idem) */
    p = _casus(piscina, "while true do :; done", &r);
    CREDO_FALSUM (r.sana);


    /* ==================================================
     * PROBARE: electio et optiones
     * ================================================== */

    imprimere("\n--- Probans electionem ---\n");

    p = _casus(piscina, "case $x in a|b) echo 1 ;; (c) ;; d) esac",
        &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ELECTIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ELECTIO_TOK_APERTURA),
        "case"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_ELECTIO_VERBUM,
        ZEPHYRUM),
        CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ELECTIO_TOK_IN), "in"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_ELECTIO_LIBERI),
        (i32)III);
    {
        MateriaNodus* optio = _filius(n, (i32)CRUSTA_ELECTIO_LIBERI,
            ZEPHYRUM);

        CREDO_VERUM (_genus(optio, CRUSTA_GENUS_OPTIO));
        CREDO_VERUM (_absens(optio, (i32)CRUSTA_OPTIO_TOK_PARENTHESIS));
        CREDO_AEQUALIS_I32 (_numerus(optio,
            (i32)CRUSTA_OPTIO_EXEMPLARIA),
            (i32)III);
        CREDO_VERUM (_genus(_filius(optio, (i32)CRUSTA_OPTIO_EXEMPLARIA,
            I), CRUSTA_GENUS_OPERATOR));
        CREDO_VERUM (_tok_est(_tok(optio,
            (i32)CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA), ")"));
        CREDO_AEQUALIS_I32 (_numerus(optio, (i32)CRUSTA_OPTIO_LIBERI),
            (i32)I);
        CREDO_VERUM (_tok_est(_tok(optio,
            (i32)CRUSTA_OPTIO_TOK_TERMINATOR),
            ";;"));
        optio = _filius(n, (i32)CRUSTA_ELECTIO_LIBERI, I);
        CREDO_VERUM (_tok_est(_tok(optio,
            (i32)CRUSTA_OPTIO_TOK_PARENTHESIS),
            "("));
        CREDO_VERUM (_absens(optio, (i32)CRUSTA_OPTIO_LIBERI));
        optio = _filius(n, (i32)CRUSTA_ELECTIO_LIBERI, II);
        CREDO_VERUM (_absens(optio, (i32)CRUSTA_OPTIO_TOK_TERMINATOR));
    }
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_ELECTIO_TOK_CLAUSURA),
        "esac"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "case x in a) ;& b) ;;& c) esac", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_ELECTIO_LIBERI,
        ZEPHYRUM), (i32)CRUSTA_OPTIO_TOK_TERMINATOR), ";&"));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_ELECTIO_LIBERI,
        I),
        (i32)CRUSTA_OPTIO_TOK_TERMINATOR), ";;&"));
    CREDO_VERUM (r.sana);

    /* lineae novae post 'in' et ')' trivia; post 'b' separator */
    p = _casus(piscina, "case x in\n  a)\n b\n ;;\nesac", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_tok(n,
        (i32)CRUSTA_ELECTIO_TOK_IN)->numerus_post,
        (i32)I);
    {
        MateriaNodus* optio = _filius(n, (i32)CRUSTA_ELECTIO_LIBERI,
            ZEPHYRUM);

        CREDO_AEQUALIS_I32 (_tok(optio,
            (i32)CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA)->numerus_post,
            (i32)I);
        CREDO_AEQUALIS_I32 (_numerus(optio, (i32)CRUSTA_OPTIO_LIBERI),
            (i32)II);
        CREDO_VERUM (_genus(_filius(optio, (i32)CRUSTA_OPTIO_LIBERI, I),
            CRUSTA_GENUS_SEPARATOR));
    }
    CREDO_VERUM (r.sana);

    /* exemplar cum partibus */
    p = _casus(piscina, "case x in $v*) ;; esac", &r);
    n = _filius(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_ELECTIO_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_OPTIO_EXEMPLARIA,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_VERBUM_PARTES),
        (i32)II);
    CREDO_VERUM (_genus(_pars(n, ZEPHYRUM),
        CRUSTA_GENUS_PARS_PARAMETRUM));
    CREDO_VERUM (r.sana);

    /* 'esac' cum exemplari aperto: ')' absens numeratur */
    p = _casus(piscina, "case x in a esac", &r);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);
    CREDO_FALSUM (r.sana);


    /* ==================================================
     * PROBARE: grex, crustula, functio, socius
     * ================================================== */

    imprimere("\n--- Probans gregem et functionem ---\n");

    p = _casus(piscina, "{ a; b; } > f", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_GREX));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_GREX_TOK_APERTURA), "{"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_GREX_LIBERI), (i32)IV);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_GREX_TOK_CLAUSURA), "}"));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_GREX_REDIRECTIONES),
        (i32)I);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_GREX_REDIRECTIONES,
        ZEPHYRUM), CRUSTA_GENUS_REDIRECTIO));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "( a ) 2>&1", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CRUSTULA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CRUSTULA_LIBERI),
        (i32)I);
    CREDO_VERUM (_tok_est(_tok(_filius(n,
        (i32)CRUSTA_CRUSTULA_REDIRECTIONES, ZEPHYRUM),
        (i32)CRUSTA_REDIRECTIO_TOK_FD), "2"));
    CREDO_VERUM (r.sana);

    /* '{ a }' : '}' verbum (sine separatore), grex ad EOF absens */
    p = _casus(piscina, "{ a }", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_GREX));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_GREX_TOK_CLAUSURA));
    CREDO_AEQUALIS_I32 (_numerus(_filius(n, (i32)CRUSTA_GREX_LIBERI,
        ZEPHYRUM), (i32)CRUSTA_IMPERIUM_LIBERI), (i32)II);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);
    CREDO_FALSUM (r.sana);

    /* grex intra gregem: '}' post '}' reservatum */
    p = _casus(piscina, "{ { a; } }", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_GREX_LIBERI, ZEPHYRUM),
        CRUSTA_GENUS_GREX));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "f() { :; }", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_FUNCTIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_FUNCTIO_TOK_VERBUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_TITULUS),
        "f"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_APERTURA),
        "("));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_CLAUSURA),
        ")"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_FUNCTIO_CORPUS,
        ZEPHYRUM),
        CRUSTA_GENUS_GREX));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "function g { :; }", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_FUNCTIO));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_VERBUM),
        "function"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_TITULUS),
        "g"));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_FUNCTIO_TOK_APERTURA));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_FUNCTIO_TOK_CLAUSURA));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "function h () ( : )", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_FUNCTIO_TOK_CLAUSURA),
        ")"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_FUNCTIO_CORPUS,
        ZEPHYRUM),
        CRUSTA_GENUS_CRUSTULA));
    CREDO_VERUM (r.sana);

    /* corpus absens: functio clausa absens, redirectio imperium suum */
    p = _casus(piscina, "f() > x", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)II);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_FUNCTIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_FUNCTIO_CORPUS));
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_IMPERIUM));
    CREDO_FALSUM (r.sana);

    p = _casus(piscina, "f() { :; } && x", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM),
        CRUSTA_GENUS_FUNCTIO));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "coproc c { :; }", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_SOCIUS));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SOCIUS_TOK_VERBUM),
        "coproc"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_SOCIUS_TOK_TITULUS),
        "c"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_SOCIUS_IMPERIUM,
        ZEPHYRUM),
        CRUSTA_GENUS_GREX));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "coproc cmd a", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_SOCIUS));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_SOCIUS_TOK_TITULUS));
    n = _filius(n, (i32)CRUSTA_SOCIUS_IMPERIUM, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IMPERIUM));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_IMPERIUM_LIBERI),
        (i32)II);
    CREDO_VERUM (_staticum_est(piscina, crusta_imperium_titulus(n),
        "cmd"));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "time ls |& cat", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_PRAEFIXA), (i32)I);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_PIPA_LIBERI, I),
        (i32)CRUSTA_OPERATOR_TOK), "|&"));
    CREDO_VERUM (r.sana);

    /* verbum post compositum clausum: malum (bash idem) */
    p = _casus(piscina, "{ a; } x", &r);
    CREDO_FALSUM (r.sana);
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_MALUM));


    /* ==================================================
     * PROBARE: iudicium [[ ]]
     * ================================================== */

    imprimere("\n--- Probans iudicium ---\n");

    p = _casus(piscina,
        "[[ $a == b* && -f $c || ! ( x =~ ^a(b|c)$ ) ]]", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IUDICIUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_IUDICIUM_TOK_APERTURA),
        "[["));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_IUDICIUM_TOK_CLAUSURA),
        "]]"));
    {
        MateriaNodus* e = _filius(n, (i32)CRUSTA_IUDICIUM_EXPRESSIO,
            ZEPHYRUM);
        MateriaNodus* s;
        MateriaNodus* d;

        CREDO_VERUM (_genus(e, CRUSTA_GENUS_IUDICIUM_CONIUNCTA));
        CREDO_VERUM (_tok_est(_tok(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR),
            "||"));
        s = _filius(e, (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM);
        CREDO_VERUM (_genus(s, CRUSTA_GENUS_IUDICIUM_CONIUNCTA));
        CREDO_VERUM (_tok_est(_tok(s, (i32)CRUSTA_BINARIA_TOK_OPERATOR),
            "&&"));
        CREDO_VERUM (_genus(_filius(s, (i32)CRUSTA_BINARIA_SINISTER,
            ZEPHYRUM), CRUSTA_GENUS_IUDICIUM_BINARIA));
        CREDO_VERUM (_tok_est(_tok(_filius(s,
            (i32)CRUSTA_BINARIA_SINISTER,
            ZEPHYRUM), (i32)CRUSTA_BINARIA_TOK_OPERATOR), "=="));
        CREDO_VERUM (_genus(_filius(_filius(s,
            (i32)CRUSTA_BINARIA_SINISTER, ZEPHYRUM),
            (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM),
            CRUSTA_GENUS_VERBUM));
        CREDO_VERUM (_genus(_filius(s, (i32)CRUSTA_BINARIA_DEXTER,
            ZEPHYRUM), CRUSTA_GENUS_IUDICIUM_PRAEPOSITA));
        CREDO_VERUM (_tok_est(_tok(_filius(s,
            (i32)CRUSTA_BINARIA_DEXTER,
            ZEPHYRUM), (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR), "-f"));
        d = _filius(e, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM);
        CREDO_VERUM (_genus(d, CRUSTA_GENUS_IUDICIUM_PRAEPOSITA));
        CREDO_VERUM (_tok_est(_tok(d,
            (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR),
            "!"));
        d = _filius(d, (i32)CRUSTA_PRAEPOSITA_OPERANDUM, ZEPHYRUM);
        CREDO_VERUM (_genus(d, CRUSTA_GENUS_IUDICIUM_INCLUSA));
        CREDO_VERUM (_tok_est(_tok(d, (i32)CRUSTA_INCLUSA_TOK_CLAUSURA),
            ")"));
        d = _filius(d, (i32)CRUSTA_INCLUSA_EXPRESSIO, ZEPHYRUM);
        CREDO_VERUM (_genus(d, CRUSTA_GENUS_IUDICIUM_BINARIA));
        CREDO_VERUM (_tok_est(_tok(d, (i32)CRUSTA_BINARIA_TOK_OPERATOR),
            "=~"));
        d = _filius(d, (i32)CRUSTA_BINARIA_DEXTER, ZEPHYRUM);
        CREDO_VERUM (_genus(d, CRUSTA_GENUS_VERBUM));
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)CRUSTA_VERBUM_PARTES),
            (i32)I);
        CREDO_AEQUALIS_S32 (_tok(_pars(d, ZEPHYRUM),
            (i32)CRUSTA_PARS_TOK)->genus, (s32)CRUSTA_LEX_REGULA);
        CREDO_VERUM (_tok_est(_tok(_pars(d, ZEPHYRUM),
            (i32)CRUSTA_PARS_TOK), "^a(b|c)$"));
    }
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "[[ -n $x ]] && y", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CATENA_LIBERI, ZEPHYRUM),
        CRUSTA_GENUS_IUDICIUM));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "[[ a ]]", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_IUDICIUM_EXPRESSIO,
        ZEPHYRUM), CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (r.sana);

    /* regula cum partibus citatis: verbum partium trium */
    p = _casus(piscina, "[[ $x =~ a|b'c d'$v ]]", &r);
    n = _filius(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IUDICIUM_EXPRESSIO, ZEPHYRUM),
        (i32)CRUSTA_BINARIA_DEXTER,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_VERBUM_PARTES),
        (i32)III);
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "[[", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IUDICIUM));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_IUDICIUM_EXPRESSIO));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_IUDICIUM_TOK_CLAUSURA));
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);

    /* operandum sine operatore: binaria cum operatore absenti, malum
     * numeratum (bash: 'conditional binary operator expected') */
    p = _casus(piscina, "[[ a b ]]", &r);
    n = _filius(_sententia(p, ZEPHYRUM), (i32)CRUSTA_IUDICIUM_EXPRESSIO,
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IUDICIUM_BINARIA));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR));
    CREDO_FALSUM (r.sana);

    /* iuxtapositio sub '!': binaria implicita in adventu formatur,
     * '!' eam totam capit - ordo octetorum. INVENTUM P9 (porta
     * totalitatis): iuxtapositio olim in fine iungebatur, post '!'
     * reductum - emissio '[[ a ! b ]]' */
    p = _casus(piscina, "[[ ! a b ]]", &r);
    n = _filius(_sententia(p, ZEPHYRUM), (i32)CRUSTA_IUDICIUM_EXPRESSIO,
        ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IUDICIUM_PRAEPOSITA));
    n = _filius(n, (i32)CRUSTA_PRAEPOSITA_OPERANDUM, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IUDICIUM_BINARIA));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR));
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);
    p = _casus(piscina, "[[ \"\" ! \"\" P ]]", &r);
    p = _casus(piscina, "[[ ! { a", &r);
    p = _casus(piscina, "[[ a && b c || ! d e ]]", &r);


    /* ==================================================
     * PROBARE: heredoca (decretum 01M2NJ16RG: corpus ubi octeti
     * iacent, redirectio per referentiam)
     * ================================================== */

    imprimere("\n--- Probans heredoca ---\n");

    p = _casus(piscina, "cat <<A\nl1\n$v\nA\nrest\n", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)V);
    CREDO_VERUM (_genus(_sententia(p, ZEPHYRUM),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_sententia(p, I), CRUSTA_GENUS_SEPARATOR));
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_HEREDOC));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_HEREDOC_PARTES),
        (i32)III);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_HEREDOC_PARTES,
        ZEPHYRUM),
        CRUSTA_GENUS_PARS_LITTERALIS));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_HEREDOC_PARTES, I),
        CRUSTA_GENUS_PARS_PARAMETRUM));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR),
        "A"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_HEREDOC_TOK_FINIS),
        "\n"));
    CREDO_VERUM (_genus(_sententia(p, III), CRUSTA_GENUS_IMPERIUM));
    {
        MateriaNodus* redirectio = _filius(_sententia(p, ZEPHYRUM),
            (i32)CRUSTA_IMPERIUM_LIBERI, I);

        CREDO_VERUM (_genus(redirectio, CRUSTA_GENUS_REDIRECTIO));
        CREDO_AEQUALIS_S32 (
            (s32)redirectio->loci[CRUSTA_REDIRECTIO_CORPUS].genus,
            (s32)MATERIA_VALOR_REFERENTIA);
        CREDO_VERUM (
            redirectio->loci[CRUSTA_REDIRECTIO_CORPUS].datum.nodus
                == n);
    }
    CREDO_AEQUALIS_I32 (r.heredoca, (i32)I);
    CREDO_VERUM (r.sana);

    /* delimitator citatus: pars litteralis una */
    p = _casus(piscina, "cat <<'A'\nl1\n$v\nA\n", &r);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_HEREDOC));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_HEREDOC_PARTES),
        (i32)I);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_HEREDOC_PARTES,
        ZEPHYRUM), (i32)CRUSTA_PARS_TOK), "l1\n$v\n"));
    CREDO_VERUM (r.sana);

    /* <<- : tabulae servatae */
    p = _casus(piscina, "cat <<-A\n\tx\n\tA\n", &r);
    n = _sententia(p, II);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_HEREDOC_PARTES,
        ZEPHYRUM), (i32)CRUSTA_PARS_TOK), "\tx\n"));
    CREDO_VERUM (_tok_est(_tok(n, (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR),
        "\tA"));
    CREDO_VERUM (r.sana);

    /* corpora duo post operatorem pipae (linea nova trivium post
     * '|') */
    p = _casus(piscina, "cat <<A <<B |\n1\nA\n2\nB\nwc", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PIPA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_PIPA_LIBERI), (i32)V);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_PIPA_LIBERI, I),
        CRUSTA_GENUS_OPERATOR));
    CREDO_AEQUALIS_I32 (_tok(_filius(n, (i32)CRUSTA_PIPA_LIBERI, I),
        (i32)CRUSTA_OPERATOR_TOK)->numerus_post, (i32)I);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_PIPA_LIBERI, II),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_PIPA_LIBERI, II),
        (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR), "A"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_PIPA_LIBERI, III),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_PIPA_LIBERI, III),
        (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR), "B"));
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_PIPA_LIBERI, IV),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_AEQUALIS_I32 (r.heredoca, (i32)II);
    CREDO_VERUM (r.sana);

    /* corpora post lineam novam quae 'wc' sequitur: in lista
     * programmatis */
    p = _casus(piscina, "cat <<A <<B | wc\n1\nA\n2\nB\n", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)IV);
    CREDO_VERUM (_genus(_sententia(p, ZEPHYRUM), CRUSTA_GENUS_PIPA));
    CREDO_VERUM (_genus(_sententia(p, II), CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_genus(_sententia(p, III), CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "cat <<A &&\nx\nA\ny", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CATENA));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CATENA_LIBERI),
        (i32)IV);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CATENA_LIBERI, II),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (r.sana);

    /* intra $( ): corpus in lista substitutionis */
    p = _casus(piscina, "echo $(cat <<A\nx\nA\n)", &r);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        I), ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI),
        (i32)III);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI, II),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (r.sana);

    /* ')' ante lineam novam: corpus in lista EXTERIORE (bash 5.2
     * mensum) */
    p = _casus(piscina, "echo $(cat <<A)\nx\nA\n", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)III);
    CREDO_VERUM (_genus(_sententia(p, II), CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_tok_est(_tok(_filius(_sententia(p, II),
        (i32)CRUSTA_HEREDOC_PARTES, ZEPHYRUM), (i32)CRUSTA_PARS_TOK),
        "x\n"));
    CREDO_VERUM (r.sana);

    /* backtick: regio finita, corpus numquam extra (bash 5.2 mensum):
     * heredoc vacuum intra substitutionem, delimitator absens */
    p = _casus(piscina, "echo `cat <<A`\nx\nA\n", &r);
    n = _pars(_filius(_sententia(p, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI,
        I), ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_PARS_SUBSTITUTIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI),
        (i32)II);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI, I),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_absens(_filius(n, (i32)CRUSTA_SUBSTITUTIO_LIBERI, I),
        (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR));
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)VI);
    CREDO_FALSUM (r.sana);

    /* ad EOF sine linea nova: partes et lexemata absentia */
    p = _casus(piscina, "cat <<A", &r);
    CREDO_AEQUALIS_I32 (_numerus(p, (i32)CRUSTA_PROGRAMMA_LIBERI),
        (i32)II);
    n = _sententia(p, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_HEREDOC_PARTES));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_HEREDOC_TOK_FINIS));
    CREDO_AEQUALIS_I32 (r.heredoca, (i32)I);
    CREDO_AEQUALIS_I32 (r.clausurae_absentes, (i32)I);

    /* linea quae intra "..." finitur: corpus post lineam apicis
     * clausi */
    p = _casus(piscina, "cat <<A \"x\ny\"\nb\nA\n", &r);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_HEREDOC_PARTES,
        ZEPHYRUM), (i32)CRUSTA_PARS_TOK), "b\n"));
    CREDO_VERUM (r.sana);

    /* corpora heredoc in lacunis compositorum (corpus FreeBSD P6):
     * post separatorem iterationis, post nomen ('for i' + linea nova),
     * post verbum electionis; transpositum ('for' + linea nova) */
    p = _casus(piscina,
        "read x <<EOF; for i in \"$x\"\nvalue\nEOF\ndo\n :\ndone\n",
        &r);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ITERATIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_ITERATIO_SEPARATOR),
        (i32)II);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_ITERATIO_SEPARATOR, I),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_INTERIECTA));
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "read x <<EOF; for i\nvalue\nEOF\ndo :; done\n",
        &r);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ITERATIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_ITERATIO_INTERIECTA),
        (i32)I);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_ITERATIO_INTERIECTA,
        ZEPHYRUM), CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (_absens(n, (i32)CRUSTA_ITERATIO_SEPARATOR));
    CREDO_VERUM (r.sana);

    p = _casus(piscina,
        "read x <<EOF; case $x\nvalue\nEOF\nin a) ;; esac\n", &r);
    n = _sententia(p, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_ELECTIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_ELECTIO_INTERIECTA),
        (i32)I);
    CREDO_VERUM (r.sana);

    p = _casus(piscina, "for ((;;))\ndo :; done", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_tok_est(_tok(_filius(n, (i32)CRUSTA_CYCLUS_SEPARATOR,
        ZEPHYRUM), (i32)CRUSTA_SEPARATOR_TOK), "\n"));
    CREDO_VERUM (r.sana);

    /* transpositum: 'for' + linea nova + corpus - octeti in arbore
     * omnes, ordo ruptus, nominatum (heredoca_transposita), sana
     * FALSUM; emissio hic NON octetim idem - _casus id asserit, ergo
     * parsatur directe */
    {
                       i32  mensura;
        constans character* fons = _copia(piscina,
            "cat <<A; for\nx\nA\ni in a; do :; done\n", &mensura);

        p = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
            &r);
        CREDO_NON_NIHIL (p);
        CREDO_AEQUALIS_I32 (r.heredoca_transposita, (i32)I);
        CREDO_FALSUM (r.sana);
    }

    /* heredoc in probatione conditionis */
    p = _casus(piscina, "if cat <<A\nx\nA\nthen :; fi", &r);
    n = _sententia(p, ZEPHYRUM);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_CONDITIO));
    CREDO_AEQUALIS_I32 (_numerus(n, (i32)CRUSTA_CONDITIO_PROBATIO),
        (i32)III);
    CREDO_VERUM (_genus(_filius(n, (i32)CRUSTA_CONDITIO_PROBATIO, II),
        CRUSTA_GENUS_HEREDOC));
    CREDO_VERUM (r.sana);


    /* ==================================================
     * PROBARE: valor staticus in nido - tempus lineare (P9)
     * ================================================== */

    /* INVENTUM P9 (porta totalitatis): verbum primum imperii clausum
     * valorem staticum petit (_aedificator_est); longitudo cruda
     * subarborem TOTAM recursive ambulabat, substitutiones intra -
     * '$(' x C milia: tempus quadraticum (LXXX s) et SIGSEGV. XX milia
     * olim ~XXIV s. */
    imprimere("\n--- Probans valorem staticum in nido ---\n");
    CREDO_NON_PENDET (_nidum_parsare("$(", (i32)20000), (i32)MM);
    CREDO_NON_PENDET (_nidum_parsare("$(\"", (i32)20000), (i32)MM);
    p = _casus(piscina, "$(a)b c", &r);
    CREDO_FALSUM (_staticum_est(piscina, _filius(_sententia(p,
        ZEPHYRUM), (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM), "b"));
    p = _casus(piscina, "\"a$'x'\"b c", &r);
    CREDO_VERUM (_staticum_est(piscina, _filius(_sententia(p,
        ZEPHYRUM), (i32)CRUSTA_IMPERIUM_LIBERI, ZEPHYRUM), "a$'x'b"));


    imprimere("\n    casus %d\n", (integer)casus_numerus);
    CREDO_MAIOR_I32 (casus_numerus, (i32)LXXX);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
