/* probatio_briar_canon.c - Porta canonis thistle
 *
 *  I.  CUSTOS DERIVAE utrimque: genus quodque registri regulam
 *      globalem habet; locus quisque regulam intra= genus suum; genus
 *      lexematis quodque regulam 'briar-'; et REVERSUM: regula omnis
 *      canonis uni tabularum aut involucro congruit.
 *  II. PINNA SIGILLI: optio 'registrum-sigillum' == sigillum vivum.
 *  III. IUDICIUM: proiectio cuiusque fixturae contra canonem vitia
 *      ZEPHYRUM; numerus documentorum se metitur.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "briar_arbor.h"
#include "briar_stml.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

#define REGULAE_MAXIMAE 64

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle",
    "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
    "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
    "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
    "briar/probationes/fixa/thistle/adversa/crlf.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"
};

nomen structura {
    chorda nomen_regulae;
    chorda intra;        /* mensura ZEPHYRUM = globalis */
       b32 congruens;
} RegulaCanonis;

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

interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 mensura = (i32)strlen(literae);

    redde (b32)(c.mensura == mensura
        && (mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)mensura) == ZEPHYRUM));
}

interior b32
_aequalis (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, (size_t)a.mensura)
                == ZEPHYRUM));
}

/* tag lexematis mangulatum: "briar-" + minusculae, '_' -> '-' */
interior vacuum
_tagum_lexematis (
    constans character* titulus,
             character* exitus)
{
    i32 i = ZEPHYRUM;
    i32 k;

    memcpy(exitus, "briar-", (size_t)6);
    k = (i32)6;
    dum (titulus[i] != '\0')
    {
        character c = titulus[i];

        exitus[k] = (c == '_') ? '-'
                  : (c >= 'A'
                      && c <= 'Z') ? (character)(c + ('a' - 'A')) : c;
        i = i + I;
        k = k + I;
    }
    exitus[k] = '\0';
}

/* regula canonis quaerere (nomen, intra); NIHIL si nulla */
interior RegulaCanonis*
_regulam (
         RegulaCanonis* regulae,
                   i32  numerus,
    constans character* nomen_regulae,
    constans character* intra)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   _aequalis_literis(regulae[i].nomen_regulae,
            nomen_regulae)
            && _aequalis_literis(regulae[i].intra, intra))
        {
            redde &regulae[i];
        }
    }
    redde NIHIL;
}

/* proiectionem plagulae contra canonem iudicare: numerus vitiorum,
 * -I = apparatus */
interior s32
_iudicare (
                           Piscina* piscina,
                             Canon* canon,
               InternamentumChorda* intern,
    constans MateriaArborConsilium* consilium,
                constans character* titulus,
                constans character* fons,
                               i32  mensura)
{
            MateriaNodus* radix;
    MateriaArborScriptura scriptura;
             StmlResultus res;
                     Xar* vitia;
                     i32  n;
                     i32  i;

    radix = briar_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde (s32)-I;
    }
    scriptura = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!scriptura.successus)
    {
        imprimere("  %s: scriptura fracta: %s\n", titulus,
            scriptura.causa ? scriptura.causa : "?");
        redde (s32)-I;
    }
    res = stml_legere(scriptura.textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < (i32)10; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

s32
principale (vacuum)
{
                      b32  praeteritus;
                  Piscina* piscina;
      InternamentumChorda* intern;
                   chorda  fons_canonis;
                   chorda  causa;
                    Canon* canon;
             StmlResultus  canon_res;
            RegulaCanonis  regulae[REGULAE_MAXIMAE];
                      i32  numerus_regularum = ZEPHYRUM;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_briar_canon",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
        &iudicium));
    briar_stml_consilium(&consilium, &ratum);

    {
        character* textus;
              i32  mensura = ZEPHYRUM;

        textus = _plagulam_legere(piscina,
            "briar/grammatica/briar.canon",
            &mensura);
        si (textus == NIHIL)
        {
            CREDO_CULPA ("briar/grammatica/briar.canon absens");
            credo_imprimere_compendium();
            redde I;
        }
        fons_canonis.datum    = (i8*)textus;
        fons_canonis.mensura  = mensura;
    }
    causa.datum = NIHIL;
    causa.mensura = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        credo_imprimere_compendium();
        redde I;
    }

    imprimere("\n--- I. custos derivae ---\n");
    canon_res = stml_legere(fons_canonis, piscina, intern);
    CREDO_VERUM (canon_res.successus
        && canon_res.elementum_radix != NIHIL);
    {
        Xar* liberi = canon_res.elementum_radix->liberi;
        i32  n = (liberi != NIHIL) ? xar_numerus(liberi) : ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            StmlNodus* e = *(StmlNodus**)xar_obtinere(liberi, i);
               chorda* nomen_regulae;
               chorda* intra;

            si (   e->titulus == NIHIL
                || !_aequalis_literis(*e->titulus, "elementum"))
            {
                perge;
            }
            nomen_regulae  = stml_attributum_capere(e, "nomen");
            intra          = stml_attributum_capere(e, "intra");
            CREDO_NON_NIHIL (nomen_regulae);
            si (   nomen_regulae     == NIHIL
                || numerus_regularum >= REGULAE_MAXIMAE)
            {
                perge;
            }
            regulae[numerus_regularum].nomen_regulae  = *nomen_regulae;
            regulae[numerus_regularum].intra          = (intra != NIHIL)
                ? *intra : chorda_ex_literis("", piscina);
            regulae[numerus_regularum].congruens = FALSUM;
            numerus_regularum = numerus_regularum + I;
        }
    }
    CREDO_MAIOR_I32 (numerus_regularum, (i32)20);

    /* involucrum */
    {
        RegulaCanonis* r = _regulam(regulae, numerus_regularum, "arbor",
            "");

        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* genera: regula globalis quaeque */
    per (i = ZEPHYRUM; i < BRIAR_REGISTRUM.numerus_generum; i++)
    {
        RegulaCanonis* r = _regulam(regulae, numerus_regularum,
            BRIAR_REGISTRUM.genera[i].titulus, "");

        si (r == NIHIL)
        {
            imprimere("  genus sine regula: %s\n",
                BRIAR_REGISTRUM.genera[i].titulus);
        }
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* loci: regula intra= genus suum */
    per (i = ZEPHYRUM; i < BRIAR_REGISTRUM.numerus_generum; i++)
    {
        constans MateriaTabGenus* g = &BRIAR_REGISTRUM.genera[i];
                             i32  k;

        per (k = ZEPHYRUM; k < g->loci_numerus; k++)
        {
            RegulaCanonis* r = _regulam(regulae, numerus_regularum,
                BRIAR_REGISTRUM.loci[g->loci_offset + k].titulus,
                g->titulus);

            si (r == NIHIL)
            {
                imprimere("  locus sine regula: %s intra %s\n",
                    BRIAR_REGISTRUM.loci[g->loci_offset + k].titulus,
                    g->titulus);
            }
            CREDO_NON_NIHIL (r);
            si (r != NIHIL)
            {
                r->congruens = VERUM;
            }
        }
    }
    /* lexemata: regula 'briar-...' quaeque */
    per (i = ZEPHYRUM; i < (i32)BRIAR_LEX_NUMERUS_GENERUM; i++)
    {
            character  tag[64];
        RegulaCanonis* r;

        _tagum_lexematis(materia_lexicon_titulus(&ratum, (s32)i), tag);
        r = _regulam(regulae, numerus_regularum, tag, "");
        si (r == NIHIL)
        {
            imprimere("  lexema sine regula: %s\n", tag);
        }
        CREDO_NON_NIHIL (r);
        si (r != NIHIL)
        {
            r->congruens = VERUM;
        }
    }
    /* reversum: regula omnis congruit */
    per (i = ZEPHYRUM; i < numerus_regularum; i++)
    {
        si (!regulae[i].congruens)
        {
            imprimere("  regula rancida: %.*s (intra %.*s)\n",
                (integer)regulae[i].nomen_regulae.mensura,
                (constans character*)regulae[i].nomen_regulae.datum,
                (integer)regulae[i].intra.mensura,
                (constans character*)regulae[i].intra.datum);
        }
        CREDO_VERUM (regulae[i].congruens);
    }

    imprimere("\n--- II. pinna sigilli ---\n");
    {
        chorda sigillum_vivum = materia_arbor_sigillum(piscina,
            &BRIAR_REGISTRUM);
           Xar* liberi = canon_res.elementum_radix->liberi;
           i32  n = (liberi != NIHIL) ? xar_numerus(liberi) : ZEPHYRUM;
           b32  inventum = FALSUM;

        per (i = ZEPHYRUM; i < n && !inventum; i++)
        {
            StmlNodus* e = *(StmlNodus**)xar_obtinere(liberi, i);
               chorda* nomen_regulae;
                  Xar* attributa;
                  i32  m;
                  i32  k;

            si (   e->titulus == NIHIL
                || !_aequalis_literis(*e->titulus, "elementum"))
            {
                perge;
            }
            nomen_regulae = stml_attributum_capere(e, "nomen");
            si (   nomen_regulae == NIHIL
                || !_aequalis_literis(*nomen_regulae, "arbor"))
            {
                perge;
            }
            attributa = e->liberi;
            m = (attributa
                != NIHIL) ? xar_numerus(attributa) : ZEPHYRUM;
            per (k = ZEPHYRUM; k < m; k++)
            {
                StmlNodus* a = *(StmlNodus**)xar_obtinere(attributa, k);
                   chorda* an;

                si (   a->titulus == NIHIL
                    || !_aequalis_literis(*a->titulus, "attributum"))
                {
                    perge;
                }
                an = stml_attributum_capere(a, "nomen");
                si (   an        != NIHIL
                    && _aequalis_literis(*an, "registrum-sigillum")
                    && a->liberi != NIHIL
                    && xar_numerus(a->liberi) > ZEPHYRUM)
                {
                    i32 q;

                    per (q = ZEPHYRUM; q < xar_numerus(a->liberi); q++)
                    {
                        StmlNodus* o =
                            *(StmlNodus**)xar_obtinere(a->liberi, q);

                        si (   o->titulus != NIHIL
                            && _aequalis_literis(*o->titulus, "optio"))
                        {
                            chorda pinna = stml_textus_valor(o,
                                piscina);

                            imprimere("  pinna %.*s, vivum %.*s\n",
                                (integer)pinna.mensura,
                                (constans character*)pinna.datum,
                                (integer)sigillum_vivum.mensura,
                                (constans character*)sigillum_vivum.datum);
                            CREDO_VERUM (_aequalis(pinna,
                                sigillum_vivum));
                            inventum = VERUM;
                        }
                    }
                }
            }
        }
        CREDO_VERUM (inventum);
    }

    imprimere("\n--- III. iudicium fixturarum ---\n");
    {
        i32 iudicata = ZEPHYRUM;

        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA)
            / magnitudo(FIXA[0])); i++)
        {
            character* textus;
                  i32  mensura = ZEPHYRUM;
                  s32  vitia;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            CREDO_NON_NIHIL (textus);
            si (textus == NIHIL)
            {
                perge;
            }
            vitia = _iudicare(piscina, canon, intern, &consilium,
                FIXA[i],
                textus, mensura);
            CREDO_AEQUALIS_S32 (vitia, ZEPHYRUM);
            iudicata = iudicata + I;
        }
        CREDO_AEQUALIS_I32 (iudicata, (i32)14);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
