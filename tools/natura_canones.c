/* tools/natura_canones.c - natura -> canon (proiectio pura)
 *
 * Genus quodque in elementum canonis proicitur. Vide
 * project-specs/natura-canones-generati-spec.md.
 *
 * PROIECTIO PURA: nihil hic manu emendatur; si generator
 * distinctionem poscit quam natura non fert, natura eam accipit
 * SOLUM si vere ontologica est (spec par. 3.3).
 *
 * Usus:
 *   bin/natura_canones -index          index entium (TSV)
 *   bin/natura_canones -radix DIR      alia sedes exemplarium
 *
 * Exitus: 0 = sanum; 2 = NIHIL CURSUM EST (disciplina domus:
 *   nihil onerari NON est successus).
 */
#include "latina.h"
#include "natura.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO ".genera"

nomen structura {
         chorda*  titulus;   /* nomen naturae, snake_case */
         chorda*  modulus;
    NaturaGenus*  genus;     /* genus ipsum, aut genus rei continentis */
      StmlNodus*  nodus;     /* nodus entis (genus aut res) */
             b32  est_res;   /* VERUM = species/individuum/cultivar */
} NcEns;

interior b32       _extensionem_habet(constans chorda* t);
interior vacuum    _stirpem_scribere(constans chorda* t, character* ex,
                                     i32 tectum);
interior vacuum    _kebab_scribere(FILE* f, constans chorda* t);
interior b32       _corpus_onerare(NaturaBibliotheca* bib,
                                   constans character* radix,
                                   Piscina* piscina);
interior Xar*      _entia_colligere(NaturaBibliotheca* bib,
                                    Piscina* piscina);

/* nomen naturae (snake) -> nomen canonis (kebab).
 * Bijectivum: genus 'nomen' naturae lineolam non fert. */
interior vacuum
_kebab_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '_')
        {
            c = '-';
        }
        putc(c, f);
    }
}

/* an titulus in ".genera" desinat - stirps VACUA non sufficit,
 * ergo aequalitas quoque reicitur (mos natura_examen) */
interior b32
_extensionem_habet(
    constans chorda*  t)
{
    i32 mensura_ext;

    mensura_ext = (i32)strlen(EXTENSIO);
    si (t->mensura <= mensura_ext)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(t->datum + (t->mensura - mensura_ext),
                       EXTENSIO, (memoriae_index)mensura_ext) ==
                ZEPHYRUM);
}

/* 'mensura.genera' -> 'mensura' */
interior vacuum
_stirpem_scribere(
    constans chorda*  t,
    character*        ex,
    i32               tectum)
{
    i32 n;

    n = t->mensura - (i32)strlen(EXTENSIO);
    si (n >= tectum)
    {
        n = tectum - I;
    }
    memcpy(ex, t->datum, (memoriae_index)n);
    ex[n] = '\0';
}

/* exemplaria OMNIA onerare - sub= fines modulorum transit,
 * ergo unum onerare non sufficit (mos natura_examen). */
interior b32
_corpus_onerare(
    NaturaBibliotheca*   bib,
    constans character*  radix,
    Piscina*             piscina)
{
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    i32                   onerata;

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_canones: '%s' aperiri nequit\n", radix);
        redde FALSUM;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        si ((memoriae_index)introitus->titulus.mensura +
            strlen(radix) + II > magnitudo(via))
        {
            perge;
        }

        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        _stirpem_scribere(&introitus->titulus, stirps,
                          (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_canones: '%s' legi nequit\n", via);
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: NULLUM exemplar in '%s' (extensio '%s')\n",
            radix, EXTENSIO);
        redde FALSUM;
    }
    natura_nectere(bib);
    redde VERUM;
}

/* genera et res dictionarii in indicem unum - discrimen relativum
 * est (spec par. 3.2), ergo ambo elementa fiunt. */
interior Xar*
_entia_colligere(
    NaturaBibliotheca*  bib,
    Piscina*            piscina)
{
    Xar* entia;
    i32  i;

    entia = xar_creare(piscina, (i32)magnitudo(NcEns));

    per (i = ZEPHYRUM; i < xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        NcEns*       e;

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus  = g->titulus;
        e->modulus  = g->modulus;
        e->genus    = g;
        e->nodus    = g->nodus;
        e->est_res  = FALSUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;
        NcEns*     e;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus  = r->titulus;
        e->modulus  = r->modulus;
        e->genus    = r->genus_suum;
        e->nodus    = r->nodus;
        e->est_res  = VERUM;
    }

    redde entia;
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*            piscina;
    NaturaBibliotheca*  bib;
    Xar*                entia;
    constans character* radix;
    b32                 modus_index;
    s32                 i;

    radix       = "natura";
    modus_index = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-index") == ZEPHYRUM)
        {
            modus_index = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_canones [-index] [-radix DIR]\n");
            redde II;
        }
    }

    piscina = piscina_generare_dynamicum("natura_canones", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib || !_corpus_onerare(bib, radix, piscina))
    {
        redde II;
    }

    entia = _entia_colligere(bib, piscina);
    si (xar_numerus(entia) == ZEPHYRUM)
    {
        fprintf(stderr, "natura_canones: NULLUM ens inventum\n");
        redde II;
    }

    si (modus_index)
    {
        i32 j;

        per (j = ZEPHYRUM; j < xar_numerus(entia); j++)
        {
            NcEns* e;

            e = (NcEns*)xar_obtinere(entia, j);
            imprimere("%.*s\t%.*s\t",
                      (integer)e->modulus->mensura,
                      (constans character*)e->modulus->datum,
                      (integer)e->titulus->mensura,
                      (constans character*)e->titulus->datum);
            _kebab_scribere(stdout, e->titulus);
            imprimere("\t%s\n", e->est_res ? "res" : "genus");
        }
        redde ZEPHYRUM;
    }

    fprintf(stderr, "natura_canones: nihil petitum (adde -index)\n");
    redde II;
}
