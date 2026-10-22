/* probatio_crusta_reservatio.c - Porta reservationis (P9; spec par. V
 * deviationes nominatae, C13 nihil reservatum)
 *
 * DEVIATIONES NOMINATAE, quaeque effectu suo MENSURATO pinnata
 * (2026-09-16; verdictum 'bash -n' 5.2.15 in commentario):
 *
 *  extglob    optione ABSENTE semper: 'echo @(a|b)' = verbum '@',
 *             malum '(', pipa 'a|b', malum ')' - mala II (bash -n:
 *             error ad '('). Numquam acceptum.
 *  alias      numquam expansum: 'x echo hi' imperium cuius titulus
 *             'x'; 'fi' sequens malum I (bash -n: error ad 'fi').
 *  eval       argumentum verbum opacum: pars simplex una, nullum
 *             malum (bash -n: exitus 0).
 *  posix      'set -o posix', 'shopt -s ...' imperia plana; grammatica
 *             bash 5.2 optionum ordinariarum pro plagula tota manet
 *             (verba reservata postea idem).
 *  $"..."     pars-versa, numquam versa: valor staticus 'x'.
 *  $'...'     pars-effugia in arbore cruda; crusta_verbum_staticum
 *             petitione decoquit ('\x41' -> 'A').
 *
 * LATUS LECTORIS (materia_arbor.c, mensuratum ut in html H8):
 *  (a) elementum ignotum sub <verbum> ('<derivatum/>', extensio
 *      futura spec par. IX): lector RECUSAT 'locus generi ignotus';
 *  (b) attributum ignotum in <verbum> ('derivatum="x"'): lector
 *      ACCIPIT (census attributorum nullus - PINNATUM), canon iudicat
 *      'attributum extra canonem';
 *  (c) 'id' in <verbum>: canon id in heredoc solo declarat - vitium
 *      idem.
 * Testis positivus: documentum idem sine re aliena lectorem et
 * canonem transit, arbor relecta 'a' emittit.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "crusta_arbor.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* mala extglob MENSURATA (plantatio: numerus mutatus -> rubet) */
#define MALA_GLOBI_EXTENSI ((i32)II)

/* FIXURA SUITAE: internamentum, crusta.canon, lexicon, consilium
 * arboris (ratum per monstratorem tenet - ergo in fixura ipsa) et
 * sigillum semel facta; sectiones LEGUNT (internatio in internamentum
 * fixurae sola licet). */
nomen structura {
      InternamentumChorda* intern;
                    Canon* canon;
      MateriaLexiconRatum  ratum;
    MateriaArborConsilium  consilium;
                   chorda  sigillum;
} FixuraCanonis;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura)
{
     character plena[4096];
         FILE*  f;
        longus  longitudo;
    character*  memoria;
        size_t  lecti;

    sprintf(plena, "%s/%s", radix, via);
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

/* parsare + emissio octetim idem (asserta) */
interior MateriaNodus*
_casus (
               Piscina* piscina,
    constans character* litterae,
         CrustaParsura* relatio)
{
    i32 mensura = (i32)strlen(litterae);
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    radix = crusta_arbor_parsare(piscina, litterae, mensura,
        &CRUSTA_BASH,
        relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &CRUSTA_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    CREDO_VERUM (emissa.successus && emissa.textus.mensura == mensura
        && memcmp(emissa.textus.datum, litterae, (size_t)mensura)
            == ZEPHYRUM);
    redde radix;
}

/* liber i loci lista (NIHIL si absens aut non nodus) */
interior MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (   nodus == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA
        || i >= materia_valor_lista_numerus(nodus->loci[locus]))
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

interior b32
_genus (
     constans MateriaNodus* nodus,
               CrustaGenus  genus)
{
    redde nodus != NIHIL && nodus->genus == (s32)genus;
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

/* valor lexematis unici tokens[i] mali */
interior b32
_malum_fert (
    constans MateriaNodus* malum,
                      i32  i,
       constans character* litterae)
{
    constans MateriaValor* v;
    constans MateriaToken* t;

    si (   !_genus(malum, CRUSTA_GENUS_MALUM)
        || malum->loci[CRUSTA_MALUM_TOKENS].genus
            != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    v = materia_valor_lista_obtinere(malum->loci[CRUSTA_MALUM_TOKENS],
        i);
    si (v == NIHIL || v->genus != MATERIA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    t = v->datum.token;
    redde t->valor.mensura == (i32)strlen(litterae)
        && memcmp(t->valor.datum, litterae, strlen(litterae))
            == ZEPHYRUM;
}

/* Documentum manu scriptum: 'a' ut imperium unius verbi, cum attributo
 * alieno in <verbum> ('' = nullum) et libero alieno sub <verbum> (''
 * = nullum); sigillum VIVUM */
interior chorda
_documentum_struere (
                Piscina* piscina,
                 chorda  sigillum,
     constans character* attributum_alienum,
     constans character* liberum_alienum)
{
    character* buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)IV * M);
        chorda c;

    sprintf(buffer,
        "<arbor grammatica=\"crusta\" registrum-sigillum=\"%.*s\""
        " fons=\"0\" b=\"0\" linea=\"1\" columna=\"1\" linea-initium>\n"
        "  <programma>\n"
        "    <liberi>\n"
        "      <imperium>\n"
        "        <liberi>\n"
        "          <verbum%s>\n"
        "            <partes>\n"
        "              <pars-litteralis>\n"
        "                <tok><crusta-litteralis>a</crusta-litteralis>"
        "</tok>\n"
        "              </pars-litteralis>\n"
        "            </partes>\n"
        "%s"
        "          </verbum>\n"
        "        </liberi>\n"
        "      </imperium>\n"
        "    </liberi>\n"
        "    <cauda><crusta-finis/></cauda>\n"
        "  </programma>\n"
        "</arbor>\n",
        (integer)sigillum.mensura, (constans character*)sigillum.datum,
        attributum_alienum, liberum_alienum);
    c.datum    = (i8*)buffer;
    c.mensura  = (i32)strlen(buffer);
    redde c;
}

/* Vitia canonis (-I = fractura ante iudicium); 'nuntius' et 'detail'
 * vitii cuiusque contra exspectata conferuntur - congruentes numerantur
 * in *congruentes */
interior s32
_canone_iudicare (
                Piscina* piscina,
    InternamentumChorda* intern,
         constans Canon* canon,
                 chorda  textus,
     constans character* nuntius,
     constans character* detail,
                    i32* congruentes)
{
    StmlResultus  res;
             Xar* vitia;
             i32  n;
             i32  i;

    *congruentes  = ZEPHYRUM;
    res           = stml_legere(textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n; i++)
    {
               CanonVitium* v =
                   (CanonVitium*)xar_obtinere(vitia, i);
        constans character* m = canon_nuntius(v->genus);

        imprimere("      vitium: %s", m);
        si (v->detail != NIHIL)
        {
            imprimere(" '%.*s'", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
        imprimere("\n");
        si (   nuntius != NIHIL && strcmp(m, nuntius) == ZEPHYRUM
            && v->detail != NIHIL
            && v->detail->mensura == (i32)strlen(detail)
            && memcmp(v->detail->datum, detail, strlen(detail))
                == ZEPHYRUM)
        {
            *congruentes = *congruentes + I;
        }
    }
    redde (s32)n;
}

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
            FixuraCanonis* fx;
                  Piscina* piscina = c->piscina;
       MateriaLexIudicium  iudicium;

    fx = (FixuraCanonis*)piscina_allocare(piscina,
        magnitudo(FixuraCanonis));
    si (fx == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    fx->intern = internamentum_creare(piscina);
    si (!materia_lexicon_ratum_facere(&fx->ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        imprimere("LEXICON CRUSTAE RECUSATUM\n");
        redde;
    }
    materia_arbor_consilium_nudum(&fx->consilium, &CRUSTA_REGISTRUM,
        &fx->ratum, "crusta");
    fx->consilium.forma.mensura_caudae = (i32)magnitudo(CrustaCauda);
    fx->sigillum = materia_arbor_sigillum(piscina, &CRUSTA_REGISTRUM);

    {
        character* textus;
              i32  mensura = ZEPHYRUM;
           chorda  fons;
           chorda  causa;

        textus = _plagulam_legere(piscina, _radix_viae(),
            "crusta/grammatica/crusta.canon", &mensura);
        CREDO_NON_NIHIL (textus);
        fons.datum     = (i8*)textus;
        fons.mensura   = mensura;
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        fx->canon = textus ? canon_legere(fons, piscina, fx->intern,
                                 &causa)
                           : NIHIL;
        CREDO_NON_NIHIL (fx->canon);
        si (fx->canon == NIHIL)
        {
            redde;
        }
    }
    c->datum = fx;
}


/* ==================================================
 * DEVIATIO: extglob - optione absente semper
 * ================================================== */

interior vacuum
_globum_probare (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    CrustaParsura  r;
     MateriaNodus* p;
     MateriaNodus* n;

    p = _casus(piscina, "shopt -s extglob; echo @(a|b)", &r);
    imprimere("  mala %d (pinna %d)\n", (integer)r.mala,
        (integer)MALA_GLOBI_EXTENSI);
    CREDO_AEQUALIS_I32 (r.mala, MALA_GLOBI_EXTENSI);
    CREDO_FALSUM (r.sana);
    CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
        p->loci[CRUSTA_PROGRAMMA_LIBERI]), (i32)VI);
    CREDO_VERUM (_genus(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        ZEPHYRUM), CRUSTA_GENUS_IMPERIUM));
    n = _liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_staticum_est(piscina, _liber(n,
        (i32)CRUSTA_IMPERIUM_LIBERI, I), "@"));
    CREDO_VERUM (_malum_fert(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        III), ZEPHYRUM, "("));
    CREDO_VERUM (_genus(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, IV),
        CRUSTA_GENUS_PIPA));
    CREDO_VERUM (_malum_fert(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, V),
        ZEPHYRUM, ")"));
}


/* ==================================================
 * DEVIATIO: alias - numquam expansum
 * ================================================== */

interior vacuum
_alias_probare (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    CrustaParsura  r;
     MateriaNodus* p;
     MateriaNodus* n;

    p = _casus(piscina, "alias x='if true; then'; x echo hi; fi", &r);
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);
    n = _liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_staticum_est(piscina, crusta_imperium_titulus(n),
        "x"));
    CREDO_VERUM (_malum_fert(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        IV), ZEPHYRUM, "fi"));
}


/* ==================================================
 * DEVIATIO: eval - argumentum opacum
 * ================================================== */

interior vacuum
_eval_probare (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    CrustaParsura  r;
     MateriaNodus* p;
     MateriaNodus* n;

    p = _casus(piscina, "eval 'if'", &r);
    CREDO_AEQUALIS_I32 (r.mala, ZEPHYRUM);
    CREDO_VERUM (r.sana);
    n = _liber(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (_genus(_liber(n, (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM),
        CRUSTA_GENUS_PARS_SIMPLEX));
    CREDO_VERUM (_staticum_est(piscina, n, "if"));
}


/* ==================================================
 * DEVIATIO: set -o posix, shopt - grammatica non mutatur
 * ================================================== */

interior vacuum
_posix_probare (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    CrustaParsura  r;
     MateriaNodus* p;

    p = _casus(piscina,
        "set -o posix; shopt -s anything; if a; then b; fi", &r);
    CREDO_AEQUALIS_I32 (r.mala, ZEPHYRUM);
    CREDO_VERUM (r.sana);
    CREDO_VERUM (_genus(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        ZEPHYRUM), CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, II),
        CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_genus(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, IV),
        CRUSTA_GENUS_CONDITIO));
}


/* ==================================================
 * DEVIATIO: $"..." et $'...' - numquam versa, decocta petitione
 * ================================================== */

interior vacuum
_versa_probare (
    CredoContextus* c)
{
          Piscina* piscina = c->piscina;
    CrustaParsura  r;
     MateriaNodus* p;
     MateriaNodus* n;

    p = _casus(piscina, "echo $\"x\" $'\\x41'", &r);
    CREDO_VERUM (r.sana);
    n = _liber(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_genus(_liber(n, (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM),
        CRUSTA_GENUS_PARS_VERSA));
    CREDO_VERUM (_staticum_est(piscina, n, "x"));
    n = _liber(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, II);
    CREDO_VERUM (_genus(_liber(n, (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM),
        CRUSTA_GENUS_PARS_EFFUGIA));
    CREDO_VERUM (_staticum_est(piscina, n, "A"));
}


/* ==================================================
 * LECTOR: testis positivus
 * ================================================== */

interior vacuum
_purum_probare (
    CredoContextus* c)
{
                         Piscina* piscina = c->piscina;
          constans FixuraCanonis* fx =
              (constans FixuraCanonis*)c->suita;
               MateriaArborVitium vitium;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        chorda doc = _documentum_struere(piscina, fx->sigillum, "", "");
        MateriaNodus* lecta;
        i32 congruentes;

        lecta = materia_arbor_legere(piscina, NIHIL, doc,
            &fx->consilium,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        si (lecta != NIHIL)
        {
            MateriaScripturaConsilium cs;
                     MateriaScriptura emissa;

            materia_scriptura_consilium_nudum(&cs, &CRUSTA_REGISTRUM);
            emissa = materia_scribere_nodum(piscina, lecta, &cs);
            CREDO_VERUM (emissa.successus && emissa.textus.mensura == I
                && emissa.textus.datum[ZEPHYRUM] == 'a');
        }
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, fx->intern,
            fx->canon,
            doc,
            NIHIL, "", &congruentes), (s32)ZEPHYRUM);
    }
}


/* ==================================================
 * LECTOR (a): elementum ignotum sub verbo - recusatum
 * ================================================== */

interior vacuum
_ignotum_probare (
    CredoContextus* c)
{
                         Piscina* piscina = c->piscina;
          constans FixuraCanonis* fx =
              (constans FixuraCanonis*)c->suita;
               MateriaArborVitium vitium;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        chorda doc = _documentum_struere(piscina, fx->sigillum, "",
            "            <derivatum/>\n");
        MateriaNodus* lecta;

        lecta = materia_arbor_legere(piscina, NIHIL, doc,
            &fx->consilium,
            &vitium);
        imprimere("  %s\n", vitium.causa ? vitium.causa : "(accepta)");
        CREDO_NIHIL (lecta);
        CREDO_NON_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            CREDO_VERUM (strcmp(vitium.causa, "locus generi ignotus")
                == ZEPHYRUM);
        }
    }
}


/* ==================================================
 * LECTOR (b)/(c): attributa ignota - lector accipit, canon iudicat
 * ================================================== */

interior vacuum
_attributa_probare (
    CredoContextus* c)
{
                         Piscina* piscina = c->piscina;
          constans FixuraCanonis* fx =
              (constans FixuraCanonis*)c->suita;
               MateriaArborVitium vitium;

    si (fx == NIHIL)
    {
        redde;
    }
    {
        hic_manens constans character* ATTRIBUTA[] = {
            " derivatum=\"x\"",
            " id=\"nod1\""
        };
        hic_manens constans character* TITULI[] = {
            "derivatum",
            "id"
        };
        i32 k;

        per (k = ZEPHYRUM; k < II; k++)
        {
            chorda doc = _documentum_struere(piscina, fx->sigillum,
                ATTRIBUTA[k], "");
            MateriaNodus* lecta;
            s32 vitia;
            i32 congruentes;

            lecta = materia_arbor_legere(piscina, NIHIL, doc,
                &fx->consilium,
                &vitium);
            /* PINNA: lector attributa ignota IGNORAT (mensuratum). Cum
             * materia census attributorum addat, haec rubet et ordo in
             * recusationem lectoris promovetur. */
            CREDO_NON_NIHIL (lecta);
            CREDO_NIHIL (vitium.causa);
            vitia = _canone_iudicare(piscina, fx->intern, fx->canon,
                doc,
                "attributum extra canonem", TITULI[k], &congruentes);
            imprimere("  %s: lector accipit, canon vitia %d\n",
                TITULI[k], (integer)vitia);
            CREDO_MAIOR_S32 (vitia, (s32)ZEPHYRUM);
            CREDO_AEQUALIS_I32 (congruentes, (i32)I);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Deviatio: extglob",
        _globum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Deviatio: alias", _alias_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Deviatio: eval", _eval_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Deviatio: posix et shopt",
        _posix_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Deviatio: $\"...\" et $'...'",
        _versa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Testis positivus: documentum purum",
        _purum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(a) <derivatum/> sub verbo",
        _ignotum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(b) derivatum=\"x\" et (c) id in verbo",
        _attributa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("crusta_reservatio", SECTIONES,
        _suitam_parare, NIHIL);
}
