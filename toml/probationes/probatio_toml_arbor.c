/* probatio_toml_arbor.c - Aedificator toml
 *
 * Quisque casus: fons (in piscina) -> toml_arbor_parsare ->
 * materia_scribere_nodum -> memcmp (ORACULUM SEPARANS: emissio
 * directa, sine STML), deinde assertiones structurae per locos:
 * documenta, capita, paria, claves strictae, valores, series, tabulae
 * compactae, ligator (regula toml), mala et recuperatio ad lineam,
 * clausurae absentes ad finem, CRLF, BOM, vexillum initium_lineae.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
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
                   s32* mensura)
{
          i32  n = (i32)strlen(litterae);
    character* m = (character*)piscina_allocare(piscina, (i64)(n + I));

    memcpy(m, litterae, (size_t)n + I);
    *mensura = (s32)n;
    redde m;
}

/* '\n' -> "\r\n" (fons novus in piscina) */
interior constans character*
_crlf (
               Piscina* piscina,
    constans character* litterae)
{
          i32  n = (i32)strlen(litterae);
          i32  i;
          i32  j = ZEPHYRUM;
    character* m = (character*)piscina_allocare(piscina,
                       (i64)(II * n + I));

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (litterae[i] == '\n')
        {
            m[j++] = '\r';
        }
        m[j++] = litterae[i];
    }
    m[j] = '\0';
    redde m;
}

/* parsare + emissio octetim idem (asserta) */
interior MateriaNodus*
_casus (
               Piscina* piscina,
    constans character* litterae,
           TomlParsura* relatio)
{
                          s32  mensura;
           constans character* fons = _copia(piscina, litterae,
                                   &mensura);
                 MateriaNodus* radix;
             MateriaScriptura  emissa;
    MateriaScripturaConsilium  consilium;
                          b32  idem;

    casus_numerus++;
    radix = toml_arbor_parsare(piscina, fons, mensura, relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &TOML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    CREDO_VERUM (emissa.successus);
    idem = emissa.successus && emissa.textus.mensura == (i32)mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.textus.datum, fons, (size_t)mensura)
                == ZEPHYRUM);
    si (!idem)
    {
        imprimere("    casus %d: emissio '%.*s'\n",
            (integer)casus_numerus,
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

    redde v->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*v) : ZEPHYRUM;
}

interior MateriaNodus*
_filius (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  index)
{
    constans MateriaValor* v;
    constans MateriaValor* e;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    v = &nodus->loci[locus];
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

/* token in lista-token (index) aut in loco token (index 0) */
interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  index)
{
    constans MateriaValor* v;
    constans MateriaValor* e;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    v = &nodus->loci[locus];
    si (v->genus == MATERIA_VALOR_TOKEN)
    {
        redde index == ZEPHYRUM ? v->datum.token : NIHIL;
    }
    si (   v->genus != MATERIA_VALOR_LISTA
        || index    >= materia_valor_lista_numerus(*v))
    {
        redde NIHIL;
    }
    e = materia_valor_lista_obtinere(*v, index);
    redde e->genus == MATERIA_VALOR_TOKEN ? e->datum.token : NIHIL;
}

interior b32
_absens (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus != NIHIL
        && nodus->loci[locus].genus == MATERIA_VALOR_NIHIL;
}

interior b32
_genus (
    constans MateriaNodus* nodus,
                TomlGenus  genus)
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

/* liber documenti i */
interior MateriaNodus*
_d (
    constans MateriaNodus* documentum,
                      i32  i)
{
    redde _filius(documentum, (i32)TOML_DOCUMENTUM_LIBERI, i);
}

/* valor paris */
interior MateriaNodus*
_valor (
    constans MateriaNodus* par)
{
    redde _filius(par, (i32)TOML_PAR_VALOR, ZEPHYRUM);
}

/* clavis paris aut capitis: numerus partium */
interior i32
_partes (
    constans MateriaNodus* clavis)
{
    redde clavis == NIHIL ? ZEPHYRUM
        : _numerus(clavis, (i32)TOML_CLAVIS_PARTES);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_toml_arbor", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: documenta, capita, paria, valores
     * ================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* p;
         TomlParsura  r;

        imprimere("\n--- Probans structuram ---\n");

        d = _casus(piscina, "a = 1\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            II);
        p = _d(d, ZEPHYRUM);
        CREDO_VERUM (_genus(p, TOML_GENUS_PAR));
        CREDO_VERUM (_tok_est(_tok(_filius(p, (i32)TOML_PAR_CLAVIS,
            ZEPHYRUM), (i32)TOML_CLAVIS_PARTES, ZEPHYRUM), "a"));
        CREDO_VERUM (_tok_est(_tok(p, (i32)TOML_PAR_TOK_SIGNUM,
            ZEPHYRUM), "="));
        CREDO_VERUM (_genus(_valor(p), TOML_GENUS_NUMERUS));
        CREDO_VERUM (_genus(_d(d, I), TOML_GENUS_LINEA));
        CREDO_NON_NIHIL (_tok(d, (i32)TOML_DOCUMENTUM_CAUDA, ZEPHYRUM));
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            ZEPHYRUM);
        /* fons NIHIL cum mensura nulla (plagula vacua ex disco) */
        {
            TomlParsura rv;

            d = toml_arbor_parsare(piscina, NIHIL, ZEPHYRUM, &rv);
            CREDO_NON_NIHIL (d);
            CREDO_VERUM (rv.sana);
        }
        CREDO_NON_NIHIL (_tok(d, (i32)TOML_DOCUMENTUM_CAUDA, ZEPHYRUM));
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "[a.b]\nc = 1\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            IV);
        CREDO_VERUM (_genus(_d(d, ZEPHYRUM), TOML_GENUS_CAPUT_TABULAE));
        CREDO_AEQUALIS_I32 (_partes(_filius(_d(d, ZEPHYRUM),
            (i32)TOML_CAPUT_CLAVIS, ZEPHYRUM)), III);
        CREDO_VERUM (_genus(_d(d, II), TOML_GENUS_PAR));
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "[[p]]\nx = 1\n[[p]]\nx = 2\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            VIII);
        CREDO_VERUM (_genus(_d(d, ZEPHYRUM), TOML_GENUS_CAPUT_SERIEI));
        CREDO_VERUM (_genus(_d(d, IV), TOML_GENUS_CAPUT_SERIEI));
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "[ a . \"b\" ]", &r);
        CREDO_AEQUALIS_I32 (_partes(_filius(_d(d, ZEPHYRUM),
            (i32)TOML_CAPUT_CLAVIS, ZEPHYRUM)), III);
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "s = \"x\"\ni = 1\nf = 1.5\nb = true\n"
            "t = 1979-05-27\nl = 'x'\nm = '''x'''\n", &r);
        CREDO_VERUM (_genus(_valor(_d(d, ZEPHYRUM)),
            TOML_GENUS_CHORDA));
        CREDO_VERUM (_genus(_valor(_d(d, II)), TOML_GENUS_NUMERUS));
        CREDO_VERUM (_genus(_valor(_d(d, IV)), TOML_GENUS_NUMERUS));
        CREDO_VERUM (_genus(_valor(_d(d, VI)), TOML_GENUS_BOOLEAN));
        CREDO_VERUM (_genus(_valor(_d(d, VIII)), TOML_GENUS_TEMPUS));
        CREDO_VERUM (_genus(_valor(_d(d, X)), TOML_GENUS_CHORDA));
        CREDO_VERUM (_genus(_valor(_d(d, XII)), TOML_GENUS_CHORDA));
        CREDO_VERUM (r.sana);
    }


    /* ==================================================
     * PROBARE: series et tabulae compactae
     * ================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* s;
         TomlParsura  r;

        imprimere("\n--- Probans series et tabulas compactas ---\n");

        d = _casus(piscina, "a = [1, [2, 3], {x = 1}]\n", &r);
        s = _valor(_d(d, ZEPHYRUM));
        CREDO_VERUM (_genus(s, TOML_GENUS_SERIES));
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)TOML_INCLUSA_LIBERI), V);
        CREDO_VERUM (_genus(_filius(s, (i32)TOML_INCLUSA_LIBERI, I),
            TOML_GENUS_COMMA));
        CREDO_VERUM (_genus(_filius(s, (i32)TOML_INCLUSA_LIBERI, II),
            TOML_GENUS_SERIES));
        CREDO_AEQUALIS_I32 (_numerus(_filius(s,
            (i32)TOML_INCLUSA_LIBERI,
            II), (i32)TOML_INCLUSA_LIBERI), III);
        CREDO_VERUM (_genus(_filius(s, (i32)TOML_INCLUSA_LIBERI, IV),
            TOML_GENUS_TABULA_COMPACTA));
        CREDO_VERUM (r.sana);

        /* comma caudale et commentarium intra seriem */
        d = _casus(piscina, "a = [\n  1, # c\n  2,\n]\n", &r);
        s = _valor(_d(d, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)TOML_INCLUSA_LIBERI), IV);
        CREDO_NON_NIHIL (_tok(s, (i32)TOML_INCLUSA_TOK_CLAUSURA,
            ZEPHYRUM));
        CREDO_VERUM (r.sana);

        d = _casus(piscina, "t = { a = 1, b.c = 2 }\n", &r);
        s = _valor(_d(d, ZEPHYRUM));
        CREDO_VERUM (_genus(s, TOML_GENUS_TABULA_COMPACTA));
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)TOML_INCLUSA_LIBERI), III);
        CREDO_AEQUALIS_I32 (_partes(_filius(_filius(s,
            (i32)TOML_INCLUSA_LIBERI, II), (i32)TOML_PAR_CLAVIS,
            ZEPHYRUM)), III);
        CREDO_VERUM (r.sana);

        /* tabula compacta vacua; series vacua */
        d = _casus(piscina, "t = {}\ns = []\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_LIBERI), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(_valor(_d(d, II)),
            (i32)TOML_INCLUSA_LIBERI), ZEPHYRUM);
        CREDO_VERUM (r.sana);

        /* linea nova intra tabulam compactam: arbor accipit (trivium),
         * coctio (Q7) iudicat */
        d = _casus(piscina, "t = {a = 1,\n b = 2}\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_LIBERI), III);
    }


    /* ==================================================
     * PROBARE: ligator (regula toml) et initium_lineae
     * ================================================== */

    {
        MateriaNodus* d;
        MateriaToken* t;
         TomlParsura  r;

        imprimere("\n--- Probans ligatorem ---\n");

        /* initium documenti: omnia ANTE */
        d = _casus(piscina, "# c\na = 1\n", &r);
        t = _tok(_filius(_d(d, ZEPHYRUM), (i32)TOML_PAR_CLAVIS,
            ZEPHYRUM),
            (i32)TOML_CLAVIS_PARTES, ZEPHYRUM);
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_I32 (t->numerus_ante, II);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            II);

        /* commentum caudale: POST valoris */
        d = _casus(piscina, "a = 1 # c\n", &r);
        t = _tok(_valor(_d(d, ZEPHYRUM)), (i32)TOML_LEXEMA_TOK,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->numerus_post, II);
        t = _tok(_d(d, I), (i32)TOML_LEXEMA_TOK, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

        /* post lineam terminantem: omnia ANTE sequentis; linea vacua
         * nodus non est */
        d = _casus(piscina, "a = 1\n\n# c\nb = 2\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            IV);
        t = _tok(_filius(_d(d, II), (i32)TOML_PAR_CLAVIS, ZEPHYRUM),
            (i32)TOML_CLAVIS_PARTES, ZEPHYRUM);
        CREDO_VERUM (_tok_est(t, "b"));
        CREDO_AEQUALIS_I32 (t->numerus_ante, III);

        /* intra seriem: usque ad lineam PRIMAM post commatis */
        d = _casus(piscina, "a = [1, # c\n 2]\n", &r);
        t = _tok(_filius(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_LIBERI,
            I), (i32)TOML_LEXEMA_TOK, ZEPHYRUM);
        CREDO_VERUM (_tok_est(t, ","));
        CREDO_AEQUALIS_I32 (t->numerus_post, III);
        t = _tok(_filius(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_LIBERI,
            II), (i32)TOML_LEXEMA_TOK, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->numerus_ante, I);
        /* initium_lineae: '2' post LINEA trivium */
        CREDO_VERUM ((t->vexilla & MATERIA_TOKEN_INITIUM_LINEAE) != 0);

        /* initium_lineae: 'b' post LINEA_FINIS (substantiva) NON */
        d = _casus(piscina, "a = 1\nb = 2\n", &r);
        t = _tok(_filius(_d(d, II), (i32)TOML_PAR_CLAVIS, ZEPHYRUM),
            (i32)TOML_CLAVIS_PARTES, ZEPHYRUM);
        CREDO_VERUM (_tok_est(t, "b"));
        CREDO_VERUM ((t->vexilla & MATERIA_TOKEN_INITIUM_LINEAE) == 0);
        /* 'a' initium documenti: VERUM */
        t = _tok(_filius(_d(d, ZEPHYRUM), (i32)TOML_PAR_CLAVIS,
            ZEPHYRUM), (i32)TOML_CLAVIS_PARTES, ZEPHYRUM);
        CREDO_VERUM ((t->vexilla & MATERIA_TOKEN_INITIUM_LINEAE) != 0);
    }


    /* ==================================================
     * PROBARE: mala, recuperatio, absentiae, clausurae
     * ================================================== */

    {
        MateriaNodus* d;
         TomlParsura  r;
                 i32  i;

        imprimere("\n--- Probans mala et recuperationem ---\n");

        d = _casus(piscina, "a = \nb = 2\n", &r);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)TOML_DOCUMENTUM_LIBERI),
            IV);
        CREDO_VERUM (_absens(_d(d, ZEPHYRUM), (i32)TOML_PAR_VALOR));
        CREDO_VERUM (_genus(_valor(_d(d, II)), TOML_GENUS_NUMERUS));
        CREDO_AEQUALIS_I32 (r.absentiae, I);
        CREDO_FALSUM (r.sana);

        /* segmentum post segmentum: clavis finitur, '=' et valor
         * absunt, reliquum lineae malum */
        d = _casus(piscina, "a 1\nb = 2\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);
        CREDO_AEQUALIS_I32 (r.absentiae, II);
        CREDO_VERUM (_genus(_d(d, I), TOML_GENUS_MALUM));
        CREDO_VERUM (_genus(_valor(_d(d, III)), TOML_GENUS_NUMERUS));

        d = _casus(piscina, "a..b = 1\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);

        d = _casus(piscina, "= 1\n", &r);
        CREDO_VERUM (_genus(_d(d, ZEPHYRUM), TOML_GENUS_MALUM));
        CREDO_AEQUALIS_I32 (r.mala, I);

        (vacuum)_casus(piscina, "]]\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);
        (vacuum)_casus(piscina, "}\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);

        /* tres lineae pravae: paria interposita omnia integra */
        d = _casus(piscina, "a = 1\n= x\nb = 2\n]]\nc = 3\n}\nd = 4\n",
            &r);
        CREDO_AEQUALIS_I32 (r.mala, III);
        per (i = ZEPHYRUM; i < _numerus(d, (i32)TOML_DOCUMENTUM_LIBERI);
             i++)
        {
            MateriaNodus* n = _d(d, i);

            si (_genus(n, TOML_GENUS_PAR))
            {
                CREDO_VERUM (_genus(_valor(n), TOML_GENUS_NUMERUS));
            }
        }

        /* valor pravus: malum ut valor */
        d = _casus(piscina, "a = hello\nb = 2\n", &r);
        CREDO_VERUM (_genus(_valor(_d(d, ZEPHYRUM)), TOML_GENUS_MALUM));
        CREDO_AEQUALIS_I32 (r.mala, I);

        /* post valorem: reliquum lineae malum */
        d = _casus(piscina, "a = 1 2\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);
        CREDO_VERUM (_genus(_valor(_d(d, ZEPHYRUM)),
            TOML_GENUS_NUMERUS));

        /* intra seriem: malum usque ad ',' aut ']' */
        d = _casus(piscina, "a = [1 2]\n", &r);
        CREDO_AEQUALIS_I32 (r.mala, I);
        CREDO_NON_NIHIL (_tok(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_TOK_CLAUSURA, ZEPHYRUM));

        /* clausurae absentes ad finem */
        d = _casus(piscina, "a = [1, 2", &r);
        CREDO_VERUM (_absens(_valor(_d(d, ZEPHYRUM)),
            (i32)TOML_INCLUSA_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        CREDO_FALSUM (r.sana);
        (vacuum)_casus(piscina, "a = \"abc", &r);
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        (vacuum)_casus(piscina, "a = \"abc\\\"", &r);
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        (vacuum)_casus(piscina, "a = '''x", &r);
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        d = _casus(piscina, "[a", &r);
        CREDO_VERUM (_absens(_d(d, ZEPHYRUM),
            (i32)TOML_CAPUT_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        (vacuum)_casus(piscina, "t = {a = 1", &r);
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, I);
        (vacuum)_casus(piscina, "a = [[[[", &r);
        CREDO_AEQUALIS_I32 (r.clausurae_absentes, IV);
        CREDO_AEQUALIS_I32 (r.profunditas_maxima, VI);
    }


    /* ==================================================
     * PROBARE: CRLF et BOM (lex octetorum; sanitas eadem)
     * ================================================== */

    {
        constans character* FONTES[] = {
            "a = 1\n",
            "[a.b]\nc = 1\n",
            "a = [\n  1, # c\n  2,\n]\n",
            "a = 1\n\n# c\nb = 2\n",
            "a = 1\n= x\nb = 2\n]]\nc = 3\n}\nd = 4\n",
            "t = { a = 1, b.c = 2 }\n"
        };
        i32 i;

        imprimere("\n--- Probans CRLF et BOM ---\n");
        per (i = ZEPHYRUM; i < (i32)(magnitudo(FONTES)
             / magnitudo(FONTES[0])); i++)
        {
            TomlParsura r1;
            TomlParsura r2;

            (vacuum)_casus(piscina, FONTES[i], &r1);
            (vacuum)_casus(piscina, _crlf(piscina, FONTES[i]), &r2);
            CREDO_AEQUALIS_I32 (r1.mala, r2.mala);
            CREDO_AEQUALIS_I32 ((i32)r1.sana, (i32)r2.sana);
        }
        {
            TomlParsura r;

            (vacuum)_casus(piscina, "\xef\xbb\xbf" "a = 1\n", &r);
            CREDO_FALSUM (r.sana);
            CREDO_AEQUALIS_I32 (r.mala, I);
        }
    }

    imprimere("\n  casus: %d\n", (integer)casus_numerus);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
