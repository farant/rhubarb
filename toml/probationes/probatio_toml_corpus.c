/* probatio_toml_corpus.c - Lex octetorum super corpora omnia
 *
 * emissio(parsatio(x)) == x pro omni plagula trium fontium:
 *   I.   toml-test (index files-toml-1.0.0: valida ET invalida);
 *   II.  domus (build/toml_corpus.lst, a cursore per git ls-files);
 *   III. silvestria (silvestria.manifestum: plagulae VIVAE; sigillo
 *        mutato aut absente OMITTUNTUR CUM NUMERO - decisio Frani,
 *        manifestum solum).
 * Plagulae indicis VALIDI quae sanae non sunt per nomen dicuntur (nulla
 * exspectatur: quaeque vitium aedificatoris est). Custos ruinae: cursus
 * totus semel sub CREDO_NON_RUIT.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_registrum.h"
#include "materia_scribere.h"
#include "filum.h"
#include "sigillum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA (IV * MXXIV)

nomen structura {
    i32 plagulae;
    i32 octeti;
    i32 idem;
    i32 sanae;
    i32 omissae;
} Numeri;

/* una plagula: parsare, emittere, comparare; VERUM si octetim idem */
interior b32
_plagula (
                Piscina* opus,
                 chorda  textus,
            TomlParsura* relatio)
{
                   MateriaNodus* radix;
               MateriaScriptura  emissa;
      MateriaScripturaConsilium  consilium;
            constans character* fons =
                (constans character*)textus.datum;

    radix = toml_arbor_parsare(opus, fons, (s32)textus.mensura,
        relatio);
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &TOML_REGISTRUM);
    emissa = materia_scribere_nodum(opus, radix, &consilium);
    redde emissa.successus && emissa.textus.mensura == textus.mensura
        && (textus.mensura == ZEPHYRUM
            || memcmp(emissa.textus.datum, textus.datum,
                (size_t)textus.mensura) == ZEPHYRUM);
}

/* lineam proximam ex textu [*k, ...) legere in 'linea' (NUL); FALSUM
 * in fine */
interior b32
_linea (
         chorda  textus,
            i32* k,
      character* linea,
            i32  capacitas)
{
    i32 n = ZEPHYRUM;

    si (*k >= textus.mensura)
    {
        redde FALSUM;
    }
    dum (*k < textus.mensura && textus.datum[*k] != '\n')
    {
        si (n + I < capacitas)
        {
            linea[n++] = (character)textus.datum[*k];
        }
        (*k)++;
    }
    (*k)++;
    linea[n] = '\0';
    redde VERUM;
}

interior vacuum
_numerare (
          Numeri* n,
          chorda  textus,
             b32  idem,
     TomlParsura* r)
{
    n->plagulae++;
    n->octeti += textus.mensura;
    si (idem)
    {
        n->idem++;
    }
    si (r->sana)
    {
        n->sanae++;
    }
}

hic_manens constans character* radix_repositorii = ".";

/* cursus totus; 'asserere' FALSUM = sub custode ruinae, sine
 * assertionibus */
interior vacuum
_currere (
    Piscina* piscina,
    Piscina* opus,
        b32  asserere)
{
          character via[VIA_MAXIMA];
          character linea[VIA_MAXIMA];
             chorda index;
                i32 k;
             Numeri tt;
             Numeri domus;
             Numeri silva;
                i32 valida_insana    = ZEPHYRUM;
                i32 invalida_insana  = ZEPHYRUM;
        TomlParsura r;

    memset(&tt, ZEPHYRUM, magnitudo(tt));
    memset(&domus, ZEPHYRUM, magnitudo(domus));
    memset(&silva, ZEPHYRUM, magnitudo(silva));

    /* I. toml-test */
    sprintf(via, "%s/toml/probationes/fixa/toml-test/tests/"
        "files-toml-1.0.0", radix_repositorii);
    index = filum_legere_totum(via, piscina);
    si (asserere)
    {
        CREDO_MAIOR_I32 (index.mensura, ZEPHYRUM);
    }
    k = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
          i32 l = (i32)strlen(linea);
       chorda textus;
          b32 idem;

        si (l < V || strcmp(linea + l - V, ".toml") != ZEPHYRUM)
        {
            perge;
        }
        sprintf(via, "%s/toml/probationes/fixa/toml-test/tests/%s",
            radix_repositorii, linea);
        textus  = filum_legere_totum(via, opus);
        idem    = _plagula(opus, textus, &r);
        _numerare(&tt, textus, idem, &r);
        si (asserere && !idem)
        {
            imprimere("  lex octetorum fracta: %s\n", linea);
        }
        si (strncmp(linea, "valid/", VI) == ZEPHYRUM && !r.sana)
        {
            valida_insana++;
            si (asserere)
            {
                imprimere("  validum insanum: %s (mala %u, "
                    "clausurae %u, absentiae %u)\n", linea, r.mala,
                    r.clausurae_absentes,
                    r.absentiae);
            }
        }
        si (strncmp(linea, "invalid/", VIII) == ZEPHYRUM && !r.sana)
        {
            invalida_insana++;
        }
        piscina_vacare(opus);
    }

    /* II. domus */
    sprintf(via, "%s/build/toml_corpus.lst", radix_repositorii);
    index = filum_legere_totum(via, piscina);
    si (asserere && index.mensura == ZEPHYRUM)
    {
        CREDO_CULPA ("build/toml_corpus.lst absens - cursor scribit");
    }
    k = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
        chorda textus;
           b32 idem;

        si (linea[ZEPHYRUM] == '\0')
        {
            perge;
        }
        sprintf(via, "%s/%s", radix_repositorii, linea);
        textus  = filum_legere_totum(via, opus);
        idem    = _plagula(opus, textus, &r);
        _numerare(&domus, textus, idem, &r);
        si (asserere && !idem)
        {
            imprimere("  lex octetorum fracta: %s\n", linea);
        }
        piscina_vacare(opus);
    }

    /* III. silvestria (vivae, sigillo iudicatae) */
    sprintf(via, "%s/toml/probationes/fixa/silvestria.manifestum",
        radix_repositorii);
    index  = filum_legere_totum(via, piscina);
    k      = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
                 character* tab        = strchr(linea, '\t');
        constans character* domus_via  = getenv("HOME");
                    chorda  textus;
                  Sigillum  s;
                 character  hex[SIGILLUM_HEX_MENSURA];
                       b32  idem;

        si (linea[ZEPHYRUM] == '#' || tab == NIHIL)
        {
            perge;
        }
        *tab = '\0';
        si (tab[I] == '~' && domus_via != NIHIL)
        {
            sprintf(via, "%s%s", domus_via, tab + II);
        }
        alioquin
        {
            sprintf(via, "%s", tab + I);
        }
        textus = filum_legere_totum(via, opus);
        s = sigillum_computare(textus.datum,
            (memoriae_index)textus.mensura);
        sigillum_hex(&s, hex);
        si (textus.datum == NIHIL || strcmp(hex, linea) != ZEPHYRUM)
        {
            silva.omissae++;
            piscina_vacare(opus);
            perge;
        }
        idem = _plagula(opus, textus, &r);
        _numerare(&silva, textus, idem, &r);
        si (asserere && !idem)
        {
            imprimere("  lex octetorum fracta: %s\n", via);
        }
        piscina_vacare(opus);
    }

    si (!asserere)
    {
        redde;
    }
    imprimere("  toml-test: %u plagulae, %u octeti, idem %u, sanae %u "
        "(validae insanae %u; invalidae syntaxi reiectae %u/%u)\n",
        tt.plagulae, tt.octeti, tt.idem, tt.sanae, valida_insana,
        invalida_insana, (i32)CDLXXIV);
    imprimere("  domus: %u plagulae, %u octeti, idem %u, sanae %u\n",
        domus.plagulae, domus.octeti, domus.idem, domus.sanae);
    imprimere("  silvestria: %u plagulae, %u octeti, idem %u, "
        "sanae %u, omissae %u\n", silva.plagulae, silva.octeti,
        silva.idem,
        silva.sanae, silva.omissae);
    CREDO_AEQUALIS_I32 (tt.plagulae, (i32)(CCV + CDLXXIV));
    CREDO_AEQUALIS_I32 (tt.idem, tt.plagulae);
    CREDO_AEQUALIS_I32 (valida_insana, ZEPHYRUM);
    CREDO_MAIOR_I32 (domus.plagulae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (domus.idem, domus.plagulae);
    CREDO_AEQUALIS_I32 (domus.sanae, domus.plagulae);
    CREDO_MAIOR_I32 (silva.plagulae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (silva.idem, silva.plagulae);
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
