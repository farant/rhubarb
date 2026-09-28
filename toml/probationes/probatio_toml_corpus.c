/* probatio_toml_corpus.c - Lex octetorum super corpora omnia
 *
 * emissio(parsatio(x)) == x pro omni plagula trium fontium
 * (toml_corpus_ambulare: toml-test 1.0.0 valida ET invalida; domus;
 * silvestria vivae sigillo iudicatae, omissae cum numero). Plagulae
 * indicis VALIDI quae sanae non sunt per nomen dicuntur (nulla
 * exspectatur: quaeque vitium aedificatoris est). Custos ruinae:
 * cursus totus semel sub CREDO_NON_RUIT.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_registrum.h"
#include "toml_corpus_ambulare.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    i32 plagulae;
    i32 octeti;
    i32 idem;
    i32 sanae;
} Numeri;

nomen structura {
    Numeri fontes[TOML_CORPUS_NUMERUS_FONTIUM];
       i32 valida_insana;
       i32 invalida_insana;
       b32 loqui;
} Status;

/* una plagula: parsare, emittere, comparare */
interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
                      Status* st  = (Status*)datum;
                      Numeri* n   = &st->fontes[fons];
                 TomlParsura  r;
                MateriaNodus* radix;
            MateriaScriptura  emissa;
    MateriaScripturaConsilium consilium;
                          b32 idem = FALSUM;

    radix = toml_arbor_parsare(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r);
    si (radix != NIHIL)
    {
        materia_scriptura_consilium_nudum(&consilium, &TOML_REGISTRUM);
        emissa = materia_scribere_nodum(opus, radix, &consilium);
        idem = emissa.successus
            && emissa.textus.mensura == textus.mensura
            && (textus.mensura == ZEPHYRUM
                || memcmp(emissa.textus.datum, textus.datum,
                    (size_t)textus.mensura) == ZEPHYRUM);
    }
    n->plagulae++;
    n->octeti += textus.mensura;
    si (idem)
    {
        n->idem++;
    }
    alioquin si (st->loqui)
    {
        imprimere("  lex octetorum fracta: %s\n", via);
    }
    si (r.sana)
    {
        n->sanae++;
    }
    si (fons == TOML_CORPUS_TOML_TEST && !r.sana)
    {
        si (strncmp(via, "valid/", VI) == ZEPHYRUM)
        {
            st->valida_insana++;
            si (st->loqui)
            {
                imprimere("  validum insanum: %s (mala %u, "
                    "clausurae %u, absentiae %u)\n", via, r.mala,
                    r.clausurae_absentes, r.absentiae);
            }
        }
        alioquin
        {
            st->invalida_insana++;
        }
    }
}

hic_manens constans character* radix_repositorii = ".";

interior vacuum
_currere (
    Piscina* piscina,
    Piscina* opus,
        b32  asserere)
{
              Status  st;
    TomlCorpusNumeri  nn;
              Numeri* tt;
              Numeri* domus;
              Numeri* silva;

    memset(&st, ZEPHYRUM, magnitudo(st));
    st.loqui = asserere;
    toml_corpus_ambulare(piscina, opus, radix_repositorii, _visor, &st,
        &nn);
    si (!asserere)
    {
        redde;
    }
    tt     = &st.fontes[TOML_CORPUS_TOML_TEST];
    domus  = &st.fontes[TOML_CORPUS_DOMUS];
    silva  = &st.fontes[TOML_CORPUS_SILVESTRIA];
    CREDO_VERUM (nn.indices_lecti);
    imprimere("  toml-test: %u plagulae, %u octeti, idem %u, sanae %u "
        "(validae insanae %u; invalidae syntaxi reiectae %u/%u)\n",
        tt->plagulae, tt->octeti, tt->idem, tt->sanae, st.valida_insana,
        st.invalida_insana, (i32)CDLXXIV);
    imprimere("  domus: %u plagulae, %u octeti, idem %u, sanae %u\n",
        domus->plagulae, domus->octeti, domus->idem, domus->sanae);
    imprimere("  silvestria: %u plagulae, %u octeti, idem %u, "
        "sanae %u, omissae %u\n", silva->plagulae, silva->octeti,
        silva->idem, silva->sanae, nn.omissae);
    CREDO_AEQUALIS_I32 (tt->plagulae, (i32)(CCV + CDLXXIV));
    CREDO_AEQUALIS_I32 (tt->idem, tt->plagulae);
    CREDO_AEQUALIS_I32 (st.valida_insana, ZEPHYRUM);
    CREDO_MAIOR_I32 (domus->plagulae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (domus->idem, domus->plagulae);
    CREDO_AEQUALIS_I32 (domus->sanae, domus->plagulae);
    CREDO_MAIOR_I32 (silva->plagulae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (silva->idem, silva->plagulae);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                Piscina* opus;
     constans character* radix = getenv("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_corpus",
        262144);
    opus = piscina_generare_dynamicum("probatio_toml_corpus_opus",
        1048576);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix != NIHIL)
    {
        radix_repositorii = radix;
    }

    imprimere("\n--- Probans corpora (lex octetorum) ---\n");
    _currere(piscina, opus, VERUM);

    imprimere("\n--- Probans custodem ruinae (cursus totus) ---\n");
    CREDO_NON_RUIT (_currere(piscina, opus, FALSUM));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
