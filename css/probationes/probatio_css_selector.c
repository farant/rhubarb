/* probatio_css_selector.c - Arbor selectorum (B9)
 *
 * DISTINCTIO OB QUAM MODULUS EXSISTIT: 'a b' et 'ab' arbores
 * DIVERSAS dant (complexus cum spatio ut combinatore / compositus
 * unus), et 'a>b' et 'a > b' arbores EASDEM (spatia iuxta
 * combinatorem punctuatum in arborem non intrant). Comparator
 * materiae (B1) aequalitatem iudicat - nulla subtilitas triviorum.
 *
 * TRES ORDINES: structura (genera, loci), aequalitas (comparator),
 * CANON (arbor selectorum ut documentum <arbor> scripta contra
 * css.canon iudicatur - constrictiones species-level B8 hic
 * falsificantur), CORPUS (praeludium omnis regulae qualificatae VI
 * plagularum parsatur: numquam NIHIL, canon 0 vitia, se metiens).
 */

#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
#include "credo.h"
#include "canon.h"
#include "css_arbor.h"
#include "css_lexema.h"
#include "css_lexicon.h"
#include "css_registrum.h"
#include "css_selector.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "probationes/fixa/css/knotapel_atlas_2026-02-26.css"
};

/* FIXURA SUITAE: internamentum, canon, lexicon et consilium arboris
 * semel facta (parare_suitae); sectiones et adiutores ea per
 * monstratorem constantem LEGUNT - internatio in internamentum
 * fixurae sola licet (decretum 2026-10-19). */
nomen structura {
     InternamentumChorda* intern;
                   Canon* canon;
     MateriaLexiconRatum  ratum;
   MateriaArborConsilium  consilium;
} FixuraSelectoris;


interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = lectiones_fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* '<sel>{}' parsare, ad praeludium ambulare, selectorem parsare.
 * FONS EX PISCINA: parsator octetos non copiat (lexemata in fontem
 * spectant), ergo buffer acervi post reditum mendax esset - primus
 * cursus huius probationis id mensuravit (VIII fracturae, NUL in
 * lexematibus). */
interior MateriaNodus*
_selectorem_parsare (
               Piscina* piscina,
    constans character* selector)
{
    character* fons;
    MateriaNodus* plagula;
    MateriaNodus* regula;

    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(selector) + III);
    sprintf(fons, "%s{}", selector);
    plagula = css_arbor_parsare(piscina, fons, (i32)strlen(fons));
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    si (materia_valor_lista_numerus(plagula->loci[CSS_PLAGULA_REGULAE])
        < I)
    {
        redde NIHIL;
    }
    regula = materia_valor_lista_obtinere(
        plagula->loci[CSS_PLAGULA_REGULAE], ZEPHYRUM)->datum.nodus;
    si (regula->genus != (s32)CSS_GENUS_REGULA_QUALIFICATA)
    {
        redde NIHIL;
    }
    redde css_selector_parsare(piscina,
        regula->loci[CSS_REGULA_PRAELUDIUM]);
}

interior MateriaNodus*
_pars (
    constans MateriaNodus* compositus,
                      i32  i)
{
    redde materia_valor_lista_obtinere(
        compositus->loci[CSS_COMPOSITUS_PARTES], i)->datum.nodus;
}

/* Arborem selectorum ut documentum scribere et contra canonem
 * iudicare. Redde numerum vitiorum; -I = fractura. */
interior s32
_canone_iudicare (
                      Piscina* piscina,
    constans FixuraSelectoris* f,
                 MateriaNodus* radix,
           constans character* titulus)
{
    MateriaArborScriptura  scriptura;
             StmlResultus  res;
                      Xar* vitia;
                      i32  n;
                      i32  i;

    scriptura = materia_arbor_scribere_nodum(piscina, radix,
        &f->consilium);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura recusata: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        redde (s32)-I;
    }
    res = stml_legere(scriptura.textus, piscina, f->intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(f->canon, res.elementum_radix,
        piscina);
    si (vitia == NIHIL)
    {
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < V; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        si (v->detail != NIHIL)
        {
            imprimere(" '%.*s'", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

/* Omnes regulae qualificatae plagulae (nidificatae inclusae):
 * praeludium parsare, numerare, canone iudicare. */
interior vacuum
_regulas_percurrere (
                    Piscina* piscina,
    constans FixuraSelectoris* f,
      constans MateriaNodus* nodus,
                        i32* parsata,
                        s32* vitia_summa)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)CSS_GENUS_REGULA_QUALIFICATA)
    {
        MateriaNodus* sel = css_selector_parsare(piscina,
            nodus->loci[CSS_REGULA_PRAELUDIUM]);

        CREDO_NON_NIHIL (sel);
        si (sel != NIHIL)
        {
            s32 vitia = _canone_iudicare(piscina, f, sel, "corpus");

            si (vitia > ZEPHYRUM)
            {
                *vitia_summa = *vitia_summa + vitia;
            }
            *parsata = *parsata + I;
        }
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _regulas_percurrere(piscina, f, v->datum.nodus, parsata,
                vitia_summa);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    _regulas_percurrere(piscina, f, e->datum.nodus,
                        parsata, vitia_summa);
                }
            }
        }
    }
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
      FixuraSelectoris* f;
    constans character* radix_viae = getenv("RHUBARB_RADIX");
    MateriaLexIudicium  iudicium;
             character  via[DXII];
             character* textus;
                   i32  mensura = ZEPHYRUM;
                chorda  fons;
                chorda  causa;

    f = (FixuraSelectoris*)piscina_allocare(c->piscina,
        magnitudo(FixuraSelectoris));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    f->intern = internamentum_creare(c->piscina);
    si (!materia_lexicon_ratum_facere(&f->ratum, &CSS_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON CSS RECUSATUM\n");
        redde;
    }
    materia_arbor_consilium_nudum(&f->consilium, &CSS_REGISTRUM,
        &f->ratum, "css");

    /* canon (B7) - arbores selectorum contra eum iudicantur */
    sprintf(via, "%s/css/grammatica/css.canon", radix_viae);
    textus = _plagulam_legere(c->piscina, via, &mensura);
    CREDO_NON_NIHIL (textus);
    si (textus == NIHIL)
    {
        redde;
    }
    fons.datum     = (i8*)textus;
    fons.mensura   = mensura;
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    f->canon       = canon_legere(fons, c->piscina, f->intern, &causa);
    CREDO_NON_NIHIL (f->canon);
    si (f->canon == NIHIL)
    {
        redde;
    }
    c->datum = f;
}


interior vacuum
_spatium_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* duo   = _selectorem_parsare(piscina, "a b");
        MateriaNodus* unus  = _selectorem_parsare(piscina, "ab");

        CREDO_NON_NIHIL (duo);
        CREDO_NON_NIHIL (unus);
        CREDO_AEQUALIS_S32 (duo->genus,
            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        CREDO_AEQUALIS_S32 (unus->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        {
            MateriaToken* comb =
                duo->loci[CSS_COMPLEXUS_COMBINATOR].datum.token;

            CREDO_NON_NIHIL (comb);
            CREDO_AEQUALIS_S32 (comb->genus, (s32)CSS_LEX_SPATIA);
        }
        CREDO_AEQUALIS_S32 (
            duo->loci[CSS_COMPLEXUS_SINISTER].datum.nodus->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        CREDO_AEQUALIS_S32 (
            duo->loci[CSS_COMPLEXUS_DEXTER].datum.nodus->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, duo, "a b"),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_aequalitatem_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* strictus;
        MateriaNodus* laxus;
        MateriaNodus* alius;
        MateriaArborDifferentia d;

        strictus  = _selectorem_parsare(piscina, "a>b");
        laxus     = _selectorem_parsare(piscina, "a > b");
        alius     = _selectorem_parsare(piscina, "a + b");

        CREDO_NON_NIHIL (strictus);
        CREDO_NON_NIHIL (laxus);
        CREDO_VERUM (materia_arbor_aequalis(strictus, laxus,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        /* et comparator DISCERNIT: '+' contra '>' */
        CREDO_FALSUM (materia_arbor_aequalis(strictus, alius,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        {
            MateriaToken* comb =
                strictus->loci[CSS_COMPLEXUS_COMBINATOR].datum.token;

            CREDO_AEQUALIS_S32 (comb->genus, (s32)CSS_LEX_DELIM);
            CREDO_VERUM (comb->valor.datum[ZEPHYRUM] == '>');
        }
    }
}


interior vacuum
_sinistrorsum_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        /* 'a + b ~ c' = ((a + b) ~ c) */
        MateriaNodus* radix = _selectorem_parsare(piscina, "a + b ~ c");
        MateriaNodus* sinister;

        CREDO_NON_NIHIL (radix);
        CREDO_AEQUALIS_S32 (radix->genus,
            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        CREDO_VERUM (radix->loci[CSS_COMPLEXUS_COMBINATOR]
            .datum.token->valor.datum[ZEPHYRUM] == '~');
        sinister = radix->loci[CSS_COMPLEXUS_SINISTER].datum.nodus;
        CREDO_AEQUALIS_S32 (sinister->genus,
            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        CREDO_VERUM (sinister->loci[CSS_COMPLEXUS_COMBINATOR]
            .datum.token->valor.datum[ZEPHYRUM] == '+');
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, radix,
            "a + b ~ c"),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_partes_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* compositus = _selectorem_parsare(piscina,
            "div.foo#bar:hover::before[x^=\"y\"]");
        MateriaNodus* attr;

        CREDO_NON_NIHIL (compositus);
        CREDO_AEQUALIS_S32 (compositus->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            compositus->loci[CSS_COMPOSITUS_PARTES]), VI);
        CREDO_AEQUALIS_S32 (_pars(compositus, ZEPHYRUM)->genus,
            (s32)CSS_GENUS_SELECTOR_TYPI);
        CREDO_AEQUALIS_S32 (_pars(compositus, I)->genus,
            (s32)CSS_GENUS_SELECTOR_CLASSIS);
        CREDO_AEQUALIS_S32 (_pars(compositus, II)->genus,
            (s32)CSS_GENUS_SELECTOR_IDENTITATIS);
        CREDO_AEQUALIS_S32 (_pars(compositus, III)->genus,
            (s32)CSS_GENUS_PSEUDO_CLASSIS);
        CREDO_AEQUALIS_S32 (_pars(compositus, IV)->genus,
            (s32)CSS_GENUS_PSEUDO_ELEMENTUM);
        CREDO_AEQUALIS_S32 (_pars(compositus, V)->genus,
            (s32)CSS_GENUS_SELECTOR_ATTRIBUTI);

        /* classis: '.' + 'foo' */
        CREDO_VERUM (_pars(compositus, I)->loci[CSS_CLASSIS_TOK_NOMEN]
            .datum.token->valor.mensura == III);
        /* pseudo-elementum: colon PRIMUM refertur, nomen 'before' */
        {
            MateriaNodus* pe = _pars(compositus, IV);
            MateriaToken* colon =
                pe->loci[CSS_PSEUDO_ELEMENTI_TOK_COLON].datum.token;
            MateriaToken* titulus =
                pe->loci[CSS_PSEUDO_ELEMENTI_TOK_NOMEN].datum.token;

            CREDO_AEQUALIS_S32 (colon->genus, (s32)CSS_LEX_COLON);
            CREDO_VERUM (titulus->valor.mensura == VI);
        }
        /* attributum: operator = '^' (lexema primum), valor chorda */
        attr = _pars(compositus, V);
        CREDO_VERUM (attr->loci[CSS_ATTRIBUTI_TOK_OPERATOR]
            .datum.token->valor.datum[ZEPHYRUM] == '^');
        CREDO_AEQUALIS_S32 (attr->loci[CSS_ATTRIBUTI_VALOR]
            .datum.token->genus, (s32)CSS_LEX_CHORDA);
        CREDO_AEQUALIS_S32 (attr->loci[CSS_ATTRIBUTI_TOK_CLAUSUM]
            .datum.token->genus, (s32)CSS_LEX_QUADRA_CLAUSA);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, compositus,
            "compositus"),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_attributum_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* compositus = _selectorem_parsare(piscina, "*[x]");
        MateriaNodus* attr;

        CREDO_NON_NIHIL (compositus);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            compositus->loci[CSS_COMPOSITUS_PARTES]), II);
        CREDO_AEQUALIS_S32 (_pars(compositus, ZEPHYRUM)->genus,
            (s32)CSS_GENUS_SELECTOR_UNIVERSALIS);
        attr = _pars(compositus, I);
        CREDO_AEQUALIS_S32 (attr->genus,
            (s32)CSS_GENUS_SELECTOR_ATTRIBUTI);
        CREDO_AEQUALIS_S32 (
            (s32)attr->loci[CSS_ATTRIBUTI_TOK_OPERATOR].genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_S32 ((s32)attr->loci[CSS_ATTRIBUTI_VALOR].genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, compositus,
            "*[x]"),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_listam_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* lista = _selectorem_parsare(piscina,
            "a, :not(.c, d), li:nth-child(2n+1)");
        MateriaNodus* secundus;
        MateriaNodus* tertius;
        MateriaNodus* pseudo;

        CREDO_NON_NIHIL (lista);
        CREDO_AEQUALIS_S32 (lista->genus,
            (s32)CSS_GENUS_SELECTOR_LISTA);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            lista->loci[CSS_SEL_LISTA_SELECTORES]), III);

        /* ':not(.c, d)' - argumenta = selector-lista una */
        secundus = materia_valor_lista_obtinere(
            lista->loci[CSS_SEL_LISTA_SELECTORES], I)->datum.nodus;
        CREDO_AEQUALIS_S32 (secundus->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        pseudo = _pars(secundus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (pseudo->genus,
            (s32)CSS_GENUS_PSEUDO_CLASSIS);
        CREDO_AEQUALIS_S32 (pseudo->loci[CSS_PSEUDO_CLASSIS_TOK_NOMEN]
            .datum.token->genus, (s32)CSS_LEX_FUNCTIO);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            pseudo->loci[CSS_PSEUDO_CLASSIS_ARGUMENTA]), I);
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pseudo->loci[CSS_PSEUDO_CLASSIS_ARGUMENTA], ZEPHYRUM)
            ->datum.nodus->genus, (s32)CSS_GENUS_SELECTOR_LISTA);

        /* 'li:nth-child(2n+1)' - argumenta cruda: lexemata II
         * ('2n' dimensio, '+1' numerus) ut servata NOVA */
        tertius = materia_valor_lista_obtinere(
            lista->loci[CSS_SEL_LISTA_SELECTORES], II)->datum.nodus;
        pseudo = _pars(tertius, I);
        CREDO_AEQUALIS_S32 (pseudo->genus,
            (s32)CSS_GENUS_PSEUDO_CLASSIS);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            pseudo->loci[CSS_PSEUDO_CLASSIS_ARGUMENTA]), II);
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pseudo->loci[CSS_PSEUDO_CLASSIS_ARGUMENTA], ZEPHYRUM)
            ->datum.nodus->genus, (s32)CSS_GENUS_LEXEMA_SERVATUM);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, lista,
            "lista"),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_recuperationem_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
        MateriaNodus* m1 = _selectorem_parsare(piscina, "1");
        MateriaNodus* m2 = _selectorem_parsare(piscina, ". a");
        MateriaNodus* m3 = _selectorem_parsare(piscina, "> b");
        MateriaNodus* m4 = _selectorem_parsare(piscina, "a >");
        MateriaNodus* m5 = _selectorem_parsare(piscina, ",");

        CREDO_NON_NIHIL (m1);
        CREDO_NON_NIHIL (m2);
        CREDO_NON_NIHIL (m3);
        CREDO_NON_NIHIL (m4);
        CREDO_NON_NIHIL (m5);
        /* '1' - pars mala intra compositum */
        CREDO_AEQUALIS_S32 (_pars(m1, ZEPHYRUM)->genus,
            (s32)CSS_GENUS_SELECTOR_MALUS);
        /* '. a' - '.' mala, spatium combinator, 'a' typus */
        CREDO_AEQUALIS_S32 (m2->genus,
            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        /* '> b' - selector relativus: sinister ABSENS */
        CREDO_AEQUALIS_S32 (m3->genus,
            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        CREDO_AEQUALIS_S32 ((s32)m3->loci[CSS_COMPLEXUS_SINISTER].genus,
            (s32)MATERIA_VALOR_NIHIL);
        /* 'a >' - dexter ABSENS */
        CREDO_AEQUALIS_S32 ((s32)m4->loci[CSS_COMPLEXUS_DEXTER].genus,
            (s32)MATERIA_VALOR_NIHIL);
        /* ',' - lista duorum malorum vacuorum */
        CREDO_AEQUALIS_S32 (m5->genus, (s32)CSS_GENUS_SELECTOR_LISTA);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            m5->loci[CSS_SEL_LISTA_SELECTORES]), II);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, m1, "1"),
            (s32)ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, m3, "> b"),
            (s32)ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, f, m5, ","),
            (s32)ZEPHYRUM);
    }
}


interior vacuum
_corpus_probare (
    CredoContextus* c)
{
    constans FixuraSelectoris* f = (constans FixuraSelectoris*)c->suita;
                      Piscina* piscina = c->piscina;
           constans character* radix_viae = getenv("RHUBARB_RADIX");

    si (f == NIHIL)
    {
        redde;
    }
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    {
        i32 numerus  = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));
        i32 lectae   = ZEPHYRUM;
        i32 parsata  = ZEPHYRUM;
        s32 vitia    = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            character  via[DXII];
            character* textus;
                  i32  mensura = ZEPHYRUM;
            MateriaNodus* plagula;

            sprintf(via, "%s/%s", radix_viae, CORPUS[i]);
            textus = _plagulam_legere(piscina, via, &mensura);
            si (textus == NIHIL)
            {
                imprimere("  ABEST: %s\n", CORPUS[i]);
                perge;
            }
            lectae   = lectae + I;
            plagula  = css_arbor_parsare(piscina, textus, mensura);
            CREDO_NON_NIHIL (plagula);
            _regulas_percurrere(piscina, f, plagula, &parsata, &vitia);
        }
        imprimere("  plagulae %d, praeludia %d, vitia canonis %d\n",
            (integer)lectae, (integer)parsata, (integer)vitia);
        CREDO_AEQUALIS_I32 (lectae, VI);
        CREDO_MAIOR_I32 (parsata, (i32)C);
        CREDO_AEQUALIS_S32 (vitia, (s32)ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "spatium ut combinator", _spatium_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "'a>b' et 'a > b' aequales", _aequalitatem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "nidificatio sinistrorsum", _sinistrorsum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "partes compositi", _partes_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributum nudum et universalis", _attributum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "lista et pseudo-classes", _listam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "recuperatio (selector-malus)", _recuperationem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "corpus: praeludia omnia", _corpus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("css_selector", SECTIONES,
        _suitam_parare, NIHIL);
}
