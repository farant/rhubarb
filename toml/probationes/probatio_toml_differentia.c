/* probatio_toml_differentia.c - Differentia contra tomllib (Q9)
 *
 * Aurea tomllib (Q1): fixa/tomllib/aurum.txt (toml-test, commissum) et
 * build/aurum_silvestre.txt (silvestria, a cursore regeneratum si abest
 * aut manifesto vetustius; numquam commissum). Pro omni plagula
 * corporis: verdictum nostrum (sanum) contra STATUS; pro VALIDIS
 * valor coctus contra JSON clausulae per toml_oraculum_comparare.
 *
 * Pinnae: toml-test concordes DCLXXIX (corpus congelatum, numerus
 * exactus); silvestria discordes ZEPHYRUM (corpus vivum in disco Frani
 * - numerus plagularum cum sarcinis mutatur, ergo numerus concordium
 * pinnari non potest; discordia nulla potest). Divergentiae NOMINATAE
 * per CAUSAM, non per viam (viae silvestres versionem sarcinae ferunt):
 *   - microsecunda: tomllib fractionem secundi ad VI digitos abscindit
 *     (datetime Pythonis), nos ad IX; TOML 1.0 utrumque iubet ('the
 *     additional precision must be truncated, not rounded'). Post
 *     disparitatem tempora nostra ad microsecunda absciduntur et
 *     iterum comparantur; aequalia -> divergentia nominata, numerata.
 *     Mensuratum 2026-09-28: una plagula (test_tomllib localtime.toml,
 *     XIV digiti).
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_oraculum.h"
#include "toml_corpus_ambulare.h"
#include "tabula_dispersa.h"
#include "filum.h"
#include "json.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PINNA_TOML_TEST_CONCORDES  DCLXXIX


nomen enumeratio {
    STATUS_VALIDUM = 0,
    STATUS_INVALIDUM,
    STATUS_ABEST
} StatusAuri;

nomen structura {
    StatusAuri status;
        chorda json;       /* VALIDUM solum: linea una */
} Clausula;

nomen structura {
    i32 plagulae;
    i32 concordes;
    i32 verdicto_discordes;
    i32 valore_discordes;
    i32 sine_auro;
    i32 divergentiae;
} Numeri;

nomen structura {
        TabulaDispersa* aurum[II];     /* toml-test, silvestria */
                Numeri  numeri[II];
                   i32  nominati;
    constans character* domus;     /* $HOME, ut '~' reddatur */
} Status;


/* ==================================================
 * Aurum legere
 * ================================================== */

/* lineae: "#### via" / "## STATUS: X" / [json] / "## CAUSA" /
 * "## FINIS" */
interior i32
_aurum_legere (
               Piscina*  piscina,
    constans character*  via,
        TabulaDispersa** exitus)
{
      chorda  textus       = filum_legere_totum(via, piscina);
         i32  i            = ZEPHYRUM;
         i32  n            = textus.mensura;
         i32  clausulae    = ZEPHYRUM;
    Clausula* currens      = NIHIL;
         b32  post_statum  = FALSUM;

    *exitus = NIHIL;
    si (textus.datum == NIHIL || n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    *exitus = tabula_dispersa_creare_chorda(piscina, MMXLVIII);
    dum (i < n)
    {
                       i32  j = i;
                    chorda  linea;
        constans character* l;

        dum (j < n && textus.datum[j] != '\n')
        {
            j++;
        }
        linea.datum    = textus.datum + i;
        linea.mensura  = j - i;
        l              = (constans character*)linea.datum;
        si (linea.mensura > V && strncmp(l, "#### ", V) == ZEPHYRUM)
        {
            chorda clavis;

            clavis.datum    = linea.datum + V;
            clavis.mensura  = linea.mensura - V;
            currens = (Clausula*)piscina_allocare(piscina,
                (i64)magnitudo(Clausula));
            memset(currens, ZEPHYRUM, magnitudo(Clausula));
            tabula_dispersa_inserere(*exitus, clavis, currens);
            clausulae++;
            post_statum = FALSUM;
        }
        alioquin si (   currens != NIHIL && linea.mensura > XI
                     && strncmp(l, "## STATUS: ", XI) == ZEPHYRUM)
        {
            currens->status = strncmp(l + XI, "VALIDUM", VII)
                == ZEPHYRUM
                ? STATUS_VALIDUM
                : strncmp(l + XI, "INVALIDUM", IX) == ZEPHYRUM
                ? STATUS_INVALIDUM : STATUS_ABEST;
            post_statum = VERUM;
        }
        alioquin si (   currens         != NIHIL && post_statum
                     && currens->status == STATUS_VALIDUM
                     && linea.mensura > ZEPHYRUM && l[ZEPHYRUM] != '#')
        {
            currens->json  = linea;
            post_statum    = FALSUM;
        }
        i = j + I;
    }
    redde clausulae;
}


/* ==================================================
 * Visor
 * ================================================== */

/* tempora arboris coctae ad microsecunda abscindere (divergentia
 * nominata 'microsecunda'); VERUM si quid mutatum */
interior b32
_fractionem_abscindere (
    TomlValor* v)
{
    b32 mutatum = FALSUM;
    i32 k;

    si (v == NIHIL)
    {
        redde FALSUM;
    }
    si (   v->genus                              == TOML_VALOR_TEMPUS
        && v->datum.tempus_valor.nanosecunda % M != ZEPHYRUM)
    {
        v->datum.tempus_valor.nanosecunda -=
            v->datum.tempus_valor.nanosecunda % M;
        redde VERUM;
    }
    si (v->genus == TOML_VALOR_TABULA)
    {
        per (k = ZEPHYRUM; k
            < xar_numerus(v->datum.tabula.valores); k++)
        {
            si (_fractionem_abscindere(*(TomlValor**)xar_obtinere(
                    v->datum.tabula.valores, k)))
            {
                mutatum = VERUM;
            }
        }
    }
    si (v->genus == TOML_VALOR_SERIES)
    {
        per (k = ZEPHYRUM; k < xar_numerus(v->datum.series); k++)
        {
            si (_fractionem_abscindere(*(TomlValor**)xar_obtinere(
                    v->datum.series, k)))
            {
                mutatum = VERUM;
            }
        }
    }
    redde mutatum;
}

interior vacuum
_nominare (
                Status* st,
    constans character* titulus,
    constans character* via,
    constans character* causa,
                   i32  mensura)
{
    si (st->nominati++ < XX)
    {
        imprimere("    %s: %s%s%.*s\n", titulus, via,
            causa != NIHIL ? " - " : "", (integer)mensura,
            causa != NIHIL ? causa : "");
    }
}

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
          Status* st = (Status*)datum;
             i32  q;
          Numeri* nu;
       character  clavis_alveus[MXXIV];
          chorda  clavis;
          vacuum* inventum = NIHIL;
        Clausula* cl;
     TomlParsura  r;
    MateriaNodus* radix;
      TomlCoctum  c;
             b32  noster;

    si (fons == TOML_CORPUS_DOMUS)
    {
        redde;
    }
    q   = fons == TOML_CORPUS_TOML_TEST ? ZEPHYRUM : I;
    nu  = &st->numeri[q];
    nu->plagulae++;
    /* via silvestris: $HOME -> '~' (ut in auro) */
    si (   q == I && st->domus != NIHIL
        && strncmp(via, st->domus, strlen(st->domus)) == ZEPHYRUM)
    {
        sprintf(clavis_alveus, "~%s", via + strlen(st->domus));
    }
    alioquin
    {
        sprintf(clavis_alveus, "%s", via);
    }
    clavis.datum    = (i8*)clavis_alveus;
    clavis.mensura  = (i32)strlen(clavis_alveus);
    si (   st->aurum[q]                  == NIHIL
        || !tabula_dispersa_invenire(st->aurum[q], clavis, &inventum)
        || ((Clausula*)inventum)->status == STATUS_ABEST)
    {
        nu->sine_auro++;
        _nominare(st, "sine auro", clavis_alveus, NIHIL, ZEPHYRUM);
        redde;
    }
    cl     = (Clausula*)inventum;
    radix  = toml_arbor_parsare(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r);
    c       = toml_coquere(opus, radix, &r);
    noster  = c.sanum;
    si (noster != (cl->status == STATUS_VALIDUM))
    {
        nu->verdicto_discordes++;
        si (!noster && xar_numerus(c.diagnostica) > ZEPHYRUM)
        {
            constans MateriaDiagnosticum* d =
                (constans MateriaDiagnosticum*)xar_obtinere(
                    c.diagnostica, ZEPHYRUM);

            _nominare(st, "nos INVALIDUM, tomllib VALIDUM",
                clavis_alveus,
                d->causa, (i32)strlen(d->causa));
        }
        alioquin
        {
            _nominare(st, "nos VALIDUM, tomllib INVALIDUM",
                clavis_alveus,
                NIHIL, ZEPHYRUM);
        }
        redde;
    }
    si (cl->status == STATUS_VALIDUM)
    {
          JsonResultus j = json_legere(cl->json, opus);
        TomlComparatio cmp;

        si (!j.successus)
        {
            nu->valore_discordes++;
            _nominare(st, "JSON auri illegibile", clavis_alveus, NIHIL,
                ZEPHYRUM);
            redde;
        }
        cmp = toml_oraculum_comparare(opus, c.radix, j.radix);
        si (   !cmp.aequalis && _fractionem_abscindere(c.radix)
            && toml_oraculum_comparare(opus, c.radix, j.radix).aequalis)
        {
            nu->divergentiae++;
            _nominare(st, "divergentia nominata (microsecunda)",
                clavis_alveus, NIHIL, ZEPHYRUM);
            redde;
        }
        si (!cmp.aequalis)
        {
            nu->valore_discordes++;
            _nominare(st, "valor dispar", clavis_alveus,
                (constans character*)cmp.causa.datum,
                (i32)cmp.causa.mensura);
            redde;
        }
    }
    nu->concordes++;
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                     b32  praeteritus;
                 Piscina* piscina;
                 Piscina* opus;
                  Status  st;
        TomlCorpusNumeri  nn;
               character  via[MXXIV];
                     i32  k;
      constans character* radix     = getenv("RHUBARB_RADIX");
      constans character* TITULI[]  = { "toml-test", "silvestria" };
                     i32  clausulae[II];

    piscina = piscina_generare_dynamicum("probatio_toml_differentia",
        4194304);
    opus    =
        piscina_generare_dynamicum("probatio_toml_differentia_opus",
        4194304);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }
    memset(&st, ZEPHYRUM, magnitudo(st));
    st.domus = getenv("HOME");

    imprimere("\n--- Legens aurea tomllib ---\n");
    sprintf(via, "%s/toml/probationes/fixa/tomllib/aurum.txt", radix);
    clausulae[ZEPHYRUM] = _aurum_legere(piscina, via,
        &st.aurum[ZEPHYRUM]);
    sprintf(via, "%s/toml/build/aurum_silvestre.txt", radix);
    clausulae[I] = _aurum_legere(piscina, via, &st.aurum[I]);
    imprimere("  aurum toml-test: %u clausulae; silvestre: %u\n",
        clausulae[ZEPHYRUM], clausulae[I]);
    CREDO_AEQUALIS_I32 (clausulae[ZEPHYRUM], (i32)DCLXXIX);
    si (clausulae[I] == ZEPHYRUM)
    {
        imprimere("  AURUM SILVESTRE ABEST: ./toml/tomllib_aurum.sh "
            "-silvestre (cursor id regenerat)\n");
    }
    CREDO_MAIOR_I32 (clausulae[I], ZEPHYRUM);

    imprimere("\n--- Probans differentiam ---\n");
    toml_corpus_ambulare(piscina, opus, radix, _visor, &st, &nn);
    per (k = ZEPHYRUM; k < II; k++)
    {
        Numeri* nu = &st.numeri[k];

        imprimere("  %s: %u plagulae, concordes %u, discordes verdicto "
            "%u, "
            "valore %u, sine auro %u, divergentiae nominatae %u\n",
            TITULI[k], nu->plagulae, nu->concordes,
            nu->verdicto_discordes,
            nu->valore_discordes, nu->sine_auro, nu->divergentiae);
    }
    CREDO_VERUM (nn.indices_lecti);
    CREDO_AEQUALIS_I32 (st.numeri[ZEPHYRUM].concordes,
        (i32)PINNA_TOML_TEST_CONCORDES);
    CREDO_AEQUALIS_I32 (st.numeri[ZEPHYRUM].sine_auro, ZEPHYRUM);
    CREDO_MAIOR_I32 (st.numeri[I].concordes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (st.numeri[I].verdicto_discordes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (st.numeri[I].valore_discordes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (st.numeri[I].sine_auro, ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
