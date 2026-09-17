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
        (memoriae_index)MMMM);
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
                  Canon* canon,
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

s32
principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans character* radix_viae;
                   Canon* canon;
     MateriaLexiconRatum  ratum;
      MateriaLexIudicium  iudicium;
   MateriaArborConsilium  consilium;
      MateriaArborVitium  vitium;
                  chorda  sigillum;
           CrustaParsura  r;
           MateriaNodus*  p;
           MateriaNodus*  n;
                     b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_crusta_reservatio",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        imprimere("LEXICON CRUSTAE RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM, &ratum,
        "crusta");
    consilium.forma.mensura_caudae = (i32)magnitudo(CrustaCauda);
    sigillum = materia_arbor_sigillum(piscina, &CRUSTA_REGISTRUM);

    {
        character* textus;
              i32  mensura = ZEPHYRUM;
           chorda  fons;
           chorda  causa;

        textus = _plagulam_legere(piscina, radix_viae,
            "crusta/grammatica/crusta.canon", &mensura);
        CREDO_NON_NIHIL (textus);
        fons.datum     = (i8*)textus;
        fons.mensura   = mensura;
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        canon = textus ? canon_legere(fons, piscina, intern, &causa)
                       : NIHIL;
        CREDO_NON_NIHIL (canon);
        si (canon == NIHIL)
        {
            credo_imprimere_compendium();
            redde I;
        }
    }


    /* ==================================================
     * DEVIATIO: extglob - optione absente semper
     * ================================================== */

    imprimere("\n--- Deviatio: extglob ---\n");
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


    /* ==================================================
     * DEVIATIO: alias - numquam expansum
     * ================================================== */

    imprimere("\n--- Deviatio: alias ---\n");
    p = _casus(piscina, "alias x='if true; then'; x echo hi; fi", &r);
    CREDO_AEQUALIS_I32 (r.mala, (i32)I);
    n = _liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, II);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_IMPERIUM));
    CREDO_VERUM (_staticum_est(piscina, crusta_imperium_titulus(n),
        "x"));
    CREDO_VERUM (_malum_fert(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI,
        IV), ZEPHYRUM, "fi"));


    /* ==================================================
     * DEVIATIO: eval - argumentum opacum
     * ================================================== */

    imprimere("\n--- Deviatio: eval ---\n");
    p = _casus(piscina, "eval 'if'", &r);
    CREDO_AEQUALIS_I32 (r.mala, ZEPHYRUM);
    CREDO_VERUM (r.sana);
    n = _liber(_liber(p, (i32)CRUSTA_PROGRAMMA_LIBERI, ZEPHYRUM),
        (i32)CRUSTA_IMPERIUM_LIBERI, I);
    CREDO_VERUM (_genus(n, CRUSTA_GENUS_VERBUM));
    CREDO_VERUM (_genus(_liber(n, (i32)CRUSTA_VERBUM_PARTES, ZEPHYRUM),
        CRUSTA_GENUS_PARS_SIMPLEX));
    CREDO_VERUM (_staticum_est(piscina, n, "if"));


    /* ==================================================
     * DEVIATIO: set -o posix, shopt - grammatica non mutatur
     * ================================================== */

    imprimere("\n--- Deviatio: posix et shopt ---\n");
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


    /* ==================================================
     * DEVIATIO: $"..." et $'...' - numquam versa, decocta petitione
     * ================================================== */

    imprimere("\n--- Deviatio: $\"...\" et $'...' ---\n");
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


    /* ==================================================
     * LECTOR: testis positivus
     * ================================================== */

    imprimere("\n--- Testis positivus: documentum purum ---\n");
    {
        chorda doc = _documentum_struere(piscina, sigillum, "", "");
        MateriaNodus* lecta;
        i32 congruentes;

        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
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
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, intern, canon,
            doc,
            NIHIL, "", &congruentes), (s32)ZEPHYRUM);
    }


    /* ==================================================
     * LECTOR (a): elementum ignotum sub verbo - recusatum
     * ================================================== */

    imprimere("\n--- (a) <derivatum/> sub verbo ---\n");
    {
        chorda doc = _documentum_struere(piscina, sigillum, "",
            "            <derivatum/>\n");
        MateriaNodus* lecta;

        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
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


    /* ==================================================
     * LECTOR (b)/(c): attributa ignota - lector accipit, canon iudicat
     * ================================================== */

    imprimere("\n--- (b) derivatum=\"x\" et (c) id in verbo ---\n");
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
            chorda doc = _documentum_struere(piscina, sigillum,
                ATTRIBUTA[k], "");
            MateriaNodus* lecta;
            s32 vitia;
            i32 congruentes;

            lecta = materia_arbor_legere(piscina, NIHIL, doc,
                &consilium,
                &vitium);
            /* PINNA: lector attributa ignota IGNORAT (mensuratum). Cum
             * materia census attributorum addat, haec rubet et ordo in
             * recusationem lectoris promovetur. */
            CREDO_NON_NIHIL (lecta);
            CREDO_NIHIL (vitium.causa);
            vitia = _canone_iudicare(piscina, intern, canon, doc,
                "attributum extra canonem", TITULI[k], &congruentes);
            imprimere("  %s: lector accipit, canon vitia %d\n",
                TITULI[k], (integer)vitia);
            CREDO_MAIOR_S32 (vitia, (s32)ZEPHYRUM);
            CREDO_AEQUALIS_I32 (congruentes, (i32)I);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
