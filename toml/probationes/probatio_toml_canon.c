/* probatio_toml_canon.c - toml.canon: custos derivae et iudicium
 *
 * CANON MANU SCRIPTUM (toml/grammatica/toml.canon), species locorum
 * MENSURATAE (Q6 gradus I: sonda super documenta omnia portae Q5).
 * Custos derivae UTRIMQUE: (a) genus quodque regulam globalem unam,
 * (b) locus quisque regulam 'intra' generis sui unam, (c) lexema
 * quodque regulam 'toml-' unam, (d) involucrum et trivia, (e) regula
 * omnis alicui tabulae congruit (rancida rubet). Pinna sigilli: optio
 * 'registrum-sigillum' involucri == materia_arbor_sigillum vivum.
 * Iudicium: casus communes et corpora tota (toml-test valida ET
 * invalida, domus, silvestria) sine vitio.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "toml_corpus_ambulare.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "canon.h"
#include "filum.h"
#include "stml.h"
#include "internamentum.h"
#include "piscina.h"
#include "lectiones.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGULAE_MAXIMAE CCLVI

nomen structura {
     StmlNodus* nodus;
        chorda  titulus_regulae;
        chorda  intra;          /* mensura ZEPHYRUM = globalis */
           b32  congruens;      /* custos reversus */
} RegulaCanonis;

nomen structura {
    i32 iudicata;
    i32 vitiosa;
    i32 fracta;
} Summa;

/* FIXURA SUITAE: internamentum, lexicon, consilium arboris et canon
 * (cum fonte suo) semel facta; sectiones et adiutores ea per
 * monstratorem constantem LEGUNT - internatio in internamentum
 * fixurae sola licet (decretum 2026-10-19). */
nomen structura {
      InternamentumChorda* intern;
                    Canon* canon;
                   chorda  fons_canonis;
      MateriaLexiconRatum  ratum;
    MateriaArborConsilium  consilium;
} FixuraCanonis;

interior b32
_aequalis_literis (
                 chorda  c,
     constans character* literae)
{
    i32 mensura = (i32)strlen(literae);

    redde c.mensura == mensura && (mensura == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)mensura) == ZEPHYRUM);
}

interior StmlNodus*
_liberum_invenire (
              StmlNodus* parens,
     constans character* titulus)
{
    i32 i;

    si (parens == NIHIL || parens->liberi == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(parens->liberi); i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(parens->liberi, i);

        si (   l->genus == STML_NODUS_ELEMENTUM && l->titulus != NIHIL
            && _aequalis_literis(*l->titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

interior StmlNodus*
_attributum_regulae_invenire (
              StmlNodus* regula,
     constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(regula->liberi); i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(regula->liberi, i);
           chorda* nomen_attributi;

        si (   l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL
            || !_aequalis_literis(*l->titulus, "attributum"))
        {
            perge;
        }
        nomen_attributi = stml_attributum_capere(l, "nomen");
        si (   nomen_attributi != NIHIL
            && _aequalis_literis(*nomen_attributi, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

interior i32
_regulas_colligere (
        StmlNodus* radix,
    RegulaCanonis* regulae)
{
    i32 i;
    i32 numerus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(radix->liberi)
         && numerus < REGULAE_MAXIMAE; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(radix->liberi, i);
           chorda* titulus_regulae;
           chorda* intra;

        si (   l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL
            || !_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        titulus_regulae = stml_attributum_capere(l, "nomen");
        si (titulus_regulae == NIHIL)
        {
            perge;
        }
        intra = stml_attributum_capere(l, "intra");
        regulae[numerus].nodus = l;
        regulae[numerus].titulus_regulae = *titulus_regulae;
        si (intra != NIHIL)
        {
            regulae[numerus].intra = *intra;
        }
        alioquin
        {
            regulae[numerus].intra.datum    = NIHIL;
            regulae[numerus].intra.mensura  = ZEPHYRUM;
        }
        regulae[numerus].congruens = FALSUM;
        numerus++;
    }
    redde numerus;
}

/* quot regulae (titulus, intra) congruant; congruentes notantur */
interior i32
_regulam_numerare (
         RegulaCanonis* regulae,
                   i32  numerus,
    constans character* titulus,
    constans character* intra)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        b32 idem = _aequalis_literis(regulae[i].titulus_regulae,
            titulus)
            && (intra == NIHIL ? regulae[i].intra.mensura == ZEPHYRUM
                : _aequalis_literis(regulae[i].intra, intra));

        si (idem)
        {
            regulae[i].congruens = VERUM;
            summa++;
        }
    }
    redde summa;
}

/* praefixum + minusculae, '_' -> '-' (speculum materia_arbor) */
interior vacuum
_tag_lexematis (
              character* buffer,
     constans character* praefixum,
     constans character* titulus)
{
    i32 i;
    i32 j = ZEPHYRUM;

    per (i = ZEPHYRUM; praefixum[i] != '\0'; i++)
    {
        buffer[j++] = praefixum[i];
    }
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        buffer[j++] = c == '_' ? '-'
            : (character)tolower((insignatus character)c);
    }
    buffer[j] = '\0';
}

/* parsare -> STML -> legere -> iudicare */
interior vacuum
_documentum_iudicare (
                   Piscina* piscina,
    constans FixuraCanonis* fx,
        constans character* fons,
                       s32  mensura,
        constans character* titulus,
                     Summa* summa,
                       i32* impressa)
{
             MateriaNodus* radix;
    MateriaArborScriptura  scriptura;
             StmlResultus  res;
                      Xar* vitia;
                      i32  i;

    radix = toml_arbor_parsare(piscina, fons, mensura, NIHIL);
    si (radix == NIHIL)
    {
        summa->fracta++;
        redde;
    }
    scriptura = materia_arbor_scribere_nodum(piscina, radix,
        &fx->consilium);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura recusata: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        summa->fracta++;
        redde;
    }
    res = stml_legere(scriptura.textus, piscina, fx->intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        summa->fracta++;
        redde;
    }
    vitia = canon_iudicare(fx->canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        summa->fracta++;
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(vitia)
        && *impressa < XX; i++)
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
        (*impressa)++;
    }
    summa->iudicata++;
    si (xar_numerus(vitia) > ZEPHYRUM)
    {
        summa->vitiosa++;
    }
}

nomen structura {
                     Summa  fontes[TOML_CORPUS_NUMERUS_FONTIUM];
    constans FixuraCanonis* fx;
                       i32  impressa;
} Status;

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
    Status* st = (Status*)datum;

    _documentum_iudicare(opus, st->fx, textus.datum != NIHIL
        ? (constans character*)textus.datum : "", (s32)textus.mensura,
        via, &st->fontes[fons], &st->impressa);
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
          FixuraCanonis* fx;
     constans character* radix = lectiones_ambitus("RHUBARB_RADIX");
                 chorda  causa;
     MateriaLexIudicium  iudicium;
              character  via[CCLVI];

    fx = (FixuraCanonis*)piscina_allocare(c->piscina,
        magnitudo(FixuraCanonis));
    si (fx == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fx->intern = internamentum_creare(c->piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&fx->ratum, &TOML_LEXICON,
        &iudicium));
    materia_arbor_consilium_nudum(&fx->consilium, &TOML_REGISTRUM,
        &fx->ratum, "toml");
    {
        chorda sg = materia_arbor_sigillum(c->piscina, &TOML_REGISTRUM);

        imprimere("  sigillum vivum registri: %.*s\n",
            (integer)sg.mensura,
            (constans character*)sg.datum);
    }

    /* canonem onerare */
    sprintf(via, "%s/toml/grammatica/toml.canon", radix);
    fx->fons_canonis = filum_legere_totum(via, c->piscina);
    CREDO_MAIOR_I32 (fx->fons_canonis.mensura, ZEPHYRUM);
    si (fx->fons_canonis.mensura == ZEPHYRUM)
    {
        imprimere("  canon ABEST\n");
        redde;
    }
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    fx->canon = canon_legere(fx->fons_canonis, c->piscina, fx->intern,
        &causa);
    CREDO_NON_NIHIL (fx->canon);
    si (fx->canon == NIHIL)
    {
        si (causa.datum != NIHIL)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        redde;
    }
    c->datum = fx;
}


/* ==================================================
 * CUSTOS DERIVAE: regulae contra tres tabulas, utrimque
 * ================================================== */

interior vacuum
_regulas_custodire (
    CredoContextus* c)
{
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                   Piscina* piscina = c->piscina;
             RegulaCanonis  regulae[REGULAE_MAXIMAE];
                       i32  numerus_regularum = ZEPHYRUM;
                       i32  i;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        StmlResultus res = stml_legere(fx->fons_canonis, piscina,
                               fx->intern);

        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            numerus_regularum = _regulas_colligere(res.elementum_radix,
                regulae);
        }
        CREDO_MAIOR_I32 (numerus_regularum, ZEPHYRUM);
        CREDO_MINOR_I32 (numerus_regularum, (i32)REGULAE_MAXIMAE);
    }

    /* (a) genera */
    per (i = ZEPHYRUM; i < TOML_REGISTRUM.numerus_generum; i++)
    {
        constans character* titulus;
                       i32  quot;

        titulus = TOML_REGISTRUM.genera[i].titulus;
        quot = _regulam_numerare(regulae, numerus_regularum, titulus,
            NIHIL);

        si (quot != I)
        {
            imprimere("  genus '%s': regulae %d\n", titulus,
                (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }

    /* (b) loci */
    {
        i32 loci = ZEPHYRUM;

        per (i = ZEPHYRUM; i < TOML_REGISTRUM.numerus_generum; i++)
        {
            constans MateriaTabGenus* g = &TOML_REGISTRUM.genera[i];
                                 i32  j;

            per (j = ZEPHYRUM; j < g->loci_numerus; j++)
            {
                constans character* locus =
                    TOML_REGISTRUM.loci[g->loci_offset + j].titulus;
                i32 quot = _regulam_numerare(regulae, numerus_regularum,
                    locus, g->titulus);

                si (quot != I)
                {
                    imprimere("  locus '%s' intra '%s': regulae %d\n",
                        locus, g->titulus, (integer)quot);
                }
                CREDO_AEQUALIS_I32 (quot, I);
                loci++;
            }
        }
        CREDO_AEQUALIS_I32 (loci, (i32)XXV);
    }

    /* (c) lexemata */
    per (i = ZEPHYRUM; i < TOML_LEXICON.numerus_generum; i++)
    {
        character tag[LXIV];
              i32 quot;

        _tag_lexematis(tag, TOML_LEXICON.praefixum_tagi,
            TOML_LEXICON.genera[i].titulus);
        quot = _regulam_numerare(regulae, numerus_regularum, tag,
            NIHIL);
        si (quot != I)
        {
            imprimere("  lexema '%s': regulae %d\n", tag,
                (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }

    /* (d) involucrum et trivia */
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "arbor", NIHIL), I);
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "ante", NIHIL), I);
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "post", NIHIL), I);

    /* (e) REVERSUM */
    {
        i32 congruentes = ZEPHYRUM;

        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (regulae[i].congruens)
            {
                congruentes++;
            }
            alioquin
            {
                chorda t = regulae[i].titulus_regulae;

                imprimere("  regula RANCIDA: '%.*s'\n",
                    (integer)t.mensura, (constans character*)t.datum);
            }
        }
        CREDO_AEQUALIS_I32 (congruentes, numerus_regularum);
        imprimere("  regulae %d\n", (integer)numerus_regularum);
    }

    /* PINNA SIGILLI */
    {
            chorda vivum = materia_arbor_sigillum(piscina,
                        &TOML_REGISTRUM);
        StmlNodus* arbor = NIHIL;
        StmlNodus* attributum;
        StmlNodus* optio;

        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (   regulae[i].intra.mensura == ZEPHYRUM
                && _aequalis_literis(regulae[i].titulus_regulae,
                "arbor"))
            {
                arbor = regulae[i].nodus;
            }
        }
        CREDO_NON_NIHIL (arbor);
        attributum = arbor ? _attributum_regulae_invenire(arbor,
            "registrum-sigillum") : NIHIL;
        optio = attributum ? _liberum_invenire(attributum, "optio")
            : NIHIL;
        CREDO_NON_NIHIL (optio);
        si (optio != NIHIL)
        {
            chorda pinna = stml_textus_normalizatus(optio, piscina);

            imprimere("  sigillum: pinna %.*s, vivum %.*s\n",
                (integer)pinna.mensura,
                (constans character*)pinna.datum,
                (integer)vivum.mensura,
                (constans character*)vivum.datum);
            CREDO_VERUM (chorda_aequalis(pinna, vivum));
        }
    }

}


/* ==================================================
 * IUDICIUM: casus communes, deinde corpora
 * ================================================== */

interior vacuum
_casus_iudicare (
    CredoContextus* c)
{
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                   Piscina* opus;
                       i32  impressa = ZEPHYRUM;

    si (fx == NIHIL)
    {
        redde;
    }
    opus = piscina_generare_dynamicum("probatio_toml_canon_opus",
        (memoriae_index)IV * M * M);
    si (opus == NIHIL)
    {
        redde;
    }
    {
        Summa summa_casuum;
          i32 k;

        imprimere("  casus %d\n", (integer)toml_casus_numerus());
        memset(&summa_casuum, ZEPHYRUM, magnitudo(summa_casuum));
        per (k = ZEPHYRUM; k < toml_casus_numerus(); k++)
        {
                   i32  l = (i32)strlen(TOML_CASUS[k]);
             character* f = (character*)piscina_allocare(opus,
                 (i64)(l + I));

            memcpy(f, TOML_CASUS[k], (size_t)(l + I));
            _documentum_iudicare(opus, fx, f, (s32)l, TOML_CASUS[k],
                &summa_casuum, &impressa);
            piscina_vacare(opus);
        }
        CREDO_AEQUALIS_I32 (summa_casuum.iudicata,
            toml_casus_numerus());
        CREDO_AEQUALIS_I32 (summa_casuum.vitiosa, ZEPHYRUM);
    }
    piscina_destruere(opus);
}

interior vacuum
_corpora_iudicare (
    CredoContextus* c)
{
    constans FixuraCanonis* fx = (constans FixuraCanonis*)c->suita;
                   Piscina* opus;
        constans character* radix = lectiones_ambitus("RHUBARB_RADIX");

    si (fx == NIHIL)
    {
        redde;
    }
    opus = piscina_generare_dynamicum("probatio_toml_canon_opus",
        (memoriae_index)IV * M * M);
    si (opus == NIHIL)
    {
        redde;
    }
    si (radix == NIHIL)
    {
        radix = ".";
    }
    {
                    Status  st;
          TomlCorpusNumeri  nn;
                       i32  f;
        constans character* TITULI[] = { "toml-test", "domus",
            "silvestria" };

        memset(&st, ZEPHYRUM, magnitudo(st));
        st.fx = fx;
        toml_corpus_ambulare(c->piscina, opus, radix, _visor, &st, &nn);
        per (f = ZEPHYRUM; f < (i32)TOML_CORPUS_NUMERUS_FONTIUM; f++)
        {
            Summa* s = &st.fontes[f];

            imprimere("  %s: iudicata %d, vitiosa %d, fracta %d\n",
                TITULI[f], (integer)s->iudicata, (integer)s->vitiosa,
                (integer)s->fracta);
            CREDO_MAIOR_I32 (s->iudicata, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (s->vitiosa, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (s->fracta, ZEPHYRUM);
        }
    }
    piscina_destruere(opus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "custos derivae", _regulas_custodire, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iudicium: casus communes", _casus_iudicare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iudicium: corpora", _corpora_iudicare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("toml_canon", SECTIONES,
        _suitam_parare, NIHIL);
}
