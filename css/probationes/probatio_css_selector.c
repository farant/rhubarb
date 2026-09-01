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
    "knotapel/atlas/web/style.css"
};

hic_manens Canon*                canon_globalis   = NIHIL;
hic_manens InternamentumChorda*  intern_globalis  = NIHIL;
hic_manens MateriaArborConsilium consilium_globale;


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

    f = fopen(via, "rb");
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
          MateriaNodus* radix,
    constans character* titulus)
{
    MateriaArborScriptura  scriptura;
             StmlResultus  res;
                      Xar* vitia;
                      i32  n;
                      i32  i;

    scriptura = materia_arbor_scribere_nodum(piscina, radix,
        &consilium_globale);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura recusata: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        redde (s32)-I;
    }
    res = stml_legere(scriptura.textus, piscina, intern_globalis);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon_globalis, res.elementum_radix,
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
            s32 vitia = _canone_iudicare(piscina, sel, "corpus");

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
            _regulas_percurrere(piscina, v->datum.nodus, parsata,
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
                    _regulas_percurrere(piscina, e->datum.nodus,
                        parsata, vitia_summa);
                }
            }
        }
    }
}

integer
principale (vacuum)
{
                Piscina* piscina;
     constans character* radix_viae;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
                    b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_css_selector",
        (memoriae_index)64 * M * M);
    credo_aperire(piscina);
    intern_globalis = internamentum_creare(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON, &iudicium))
    {
        imprimere("LEXICON CSS RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium_globale, &CSS_REGISTRUM,
        &ratum, "css");

    /* canon (B7) - arbores selectorum contra eum iudicantur */
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;
           chorda  fons;
           chorda  causa;

        sprintf(via, "%s/css/grammatica/css.canon", radix_viae);
        textus = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            credo_imprimere_compendium();
            redde I;
        }
        fons.datum     = (i8*)textus;
        fons.mensura   = mensura;
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        canon_globalis = canon_legere(fons, piscina, intern_globalis,
            &causa);
        CREDO_NON_NIHIL (canon_globalis);
        si (canon_globalis == NIHIL)
        {
            credo_imprimere_compendium();
            redde I;
        }
    }


    imprimere("\n--- Probans spatium UT COMBINATOREM ---\n");
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
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, duo, "a b"),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans 'a>b' et 'a > b' AEQUALES ---\n");
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


    imprimere("\n--- Probans nidificationem sinistrorsum ---\n");
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
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, radix,
            "a + b ~ c"),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans partes compositi ---\n");
    {
        MateriaNodus* c = _selectorem_parsare(piscina,
            "div.foo#bar:hover::before[x^=\"y\"]");
        MateriaNodus* attr;

        CREDO_NON_NIHIL (c);
        CREDO_AEQUALIS_S32 (c->genus,
            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            c->loci[CSS_COMPOSITUS_PARTES]), VI);
        CREDO_AEQUALIS_S32 (_pars(c, ZEPHYRUM)->genus,
            (s32)CSS_GENUS_SELECTOR_TYPI);
        CREDO_AEQUALIS_S32 (_pars(c, I)->genus,
            (s32)CSS_GENUS_SELECTOR_CLASSIS);
        CREDO_AEQUALIS_S32 (_pars(c, II)->genus,
            (s32)CSS_GENUS_SELECTOR_IDENTITATIS);
        CREDO_AEQUALIS_S32 (_pars(c, III)->genus,
            (s32)CSS_GENUS_PSEUDO_CLASSIS);
        CREDO_AEQUALIS_S32 (_pars(c, IV)->genus,
            (s32)CSS_GENUS_PSEUDO_ELEMENTUM);
        CREDO_AEQUALIS_S32 (_pars(c, V)->genus,
            (s32)CSS_GENUS_SELECTOR_ATTRIBUTI);

        /* classis: '.' + 'foo' */
        CREDO_VERUM (_pars(c, I)->loci[CSS_CLASSIS_TOK_NOMEN]
            .datum.token->valor.mensura == III);
        /* pseudo-elementum: colon PRIMUM refertur, nomen 'before' */
        {
            MateriaNodus* pe = _pars(c, IV);
            MateriaToken* colon =
                pe->loci[CSS_PSEUDO_ELEMENTI_TOK_COLON].datum.token;
            MateriaToken* titulus =
                pe->loci[CSS_PSEUDO_ELEMENTI_TOK_NOMEN].datum.token;

            CREDO_AEQUALIS_S32 (colon->genus, (s32)CSS_LEX_COLON);
            CREDO_VERUM (titulus->valor.mensura == VI);
        }
        /* attributum: operator = '^' (lexema primum), valor chorda */
        attr = _pars(c, V);
        CREDO_VERUM (attr->loci[CSS_ATTRIBUTI_TOK_OPERATOR]
            .datum.token->valor.datum[ZEPHYRUM] == '^');
        CREDO_AEQUALIS_S32 (attr->loci[CSS_ATTRIBUTI_VALOR]
            .datum.token->genus, (s32)CSS_LEX_CHORDA);
        CREDO_AEQUALIS_S32 (attr->loci[CSS_ATTRIBUTI_TOK_CLAUSUM]
            .datum.token->genus, (s32)CSS_LEX_QUADRA_CLAUSA);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, c, "compositus"),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans attributum nudum et universalem ---\n");
    {
        MateriaNodus* c = _selectorem_parsare(piscina, "*[x]");
        MateriaNodus* attr;

        CREDO_NON_NIHIL (c);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            c->loci[CSS_COMPOSITUS_PARTES]), II);
        CREDO_AEQUALIS_S32 (_pars(c, ZEPHYRUM)->genus,
            (s32)CSS_GENUS_SELECTOR_UNIVERSALIS);
        attr = _pars(c, I);
        CREDO_AEQUALIS_S32 (attr->genus,
            (s32)CSS_GENUS_SELECTOR_ATTRIBUTI);
        CREDO_AEQUALIS_S32 (
            (s32)attr->loci[CSS_ATTRIBUTI_TOK_OPERATOR].genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_S32 ((s32)attr->loci[CSS_ATTRIBUTI_VALOR].genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, c, "*[x]"),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans listam et pseudo-classes ---\n");
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
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, lista, "lista"),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans recuperationem (selector-malus) ---\n");
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
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, m1, "1"),
            (s32)ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, m3, "> b"),
            (s32)ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, m5, ","),
            (s32)ZEPHYRUM);
    }


    imprimere("\n--- Probans corpus: praeludia omnia ---\n");
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
            _regulas_percurrere(piscina, plagula, &parsata, &vitia);
        }
        imprimere("  plagulae %d, praeludia %d, vitia canonis %d\n",
            (integer)lectae, (integer)parsata, (integer)vitia);
        CREDO_AEQUALIS_I32 (lectae, VI);
        CREDO_MAIOR_I32 (parsata, (i32)C);
        CREDO_AEQUALIS_S32 (vitia, (s32)ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
