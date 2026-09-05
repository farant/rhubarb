/* verba.c - vocabula annotata plagulae orationis (oratio/verba.sh)
 *
 * Usus: verba <plagula.txt>... [-machina] [-analyses]
 * Plagula quaeque parsatur (oratio_arbor) et annotatur (oratio_partes:
 * vocabularia la.bin + glossarium + Moby ex RHUBARB_RADIX, ut arbor.sh
 * -partes; lectiones Latinae et Anglicae, T16), deinde una linea per
 * VOCABULUM ordine documenti:
 *   via index initium finis linea paragraphus sententia forma classes
 *   linguae lemma analyses
 * (index = ordinalis vocabuli in plagula; paragraphus et sententia =
 * ordinales, sententia ut in sententiae.sh numerata; classes/linguae
 * = loci compendiarii vocabuli, spatiis separatae; lemma = analysis
 * primae, vacuum si nulla; analyses = numerus). -analyses: una linea
 * per ANALYSIN loco vocabuli:
 *   via index forma classis lemma lingua fons nativum sensus casus
 *   numerus genus persona tempus modus vox forma-verbi gradus species
 *   declinatio coniugatio
 * (accidentia titulis enumerationum registri, declinatio/coniugatio
 * numeris, vacua si non data aut si genus ea non fert). -machina
 * caput '#' addit. Exitus: 0 vocabula, 1 nulla, 2 usus/plagula/tabula/
 * parsura.
 */

#include "latina.h"
#include "oratio_arbor.h"
#include "oratio_registrum.h"
#include "oratio_partes.h"
#include "oratio_vocabularia.h"
#include "oratio_resolutio.h"
#include "oratio_lexicon.h"
#include "materia_lexicon.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* columna accidentis: titulus loci, series titulorum (NIHIL = numerus
 * nudus: declinatio, coniugatio) */
nomen structura {
    constans character* titulus;
    constans character* constans* tituli;
                             i32 numerus;
} ColumnaAccidentis;

interior constans ColumnaAccidentis COLUMNAE[] = {
    { "casus",       ORATIO_TITULI_CASUUM,
        (i32)ORATIO_CASUS_NUMERUS },
    { "numerus",     ORATIO_TITULI_NUMERORUM,
        (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS },
    { "genus",       ORATIO_TITULI_GENERUM_GRAMMATICORUM,
        (i32)ORATIO_GENUS_GRAMMATICUM_NUMERUS },
    { "persona",     ORATIO_TITULI_PERSONARUM,
        (i32)ORATIO_PERSONA_NUMERUS },
    { "tempus",      ORATIO_TITULI_TEMPORUM,
        (i32)ORATIO_TEMPUS_NUMERUS },
    { "modus",       ORATIO_TITULI_MODORUM,
        (i32)ORATIO_MODUS_NUMERUS },
    { "vox",         ORATIO_TITULI_VOCUM,
        (i32)ORATIO_VOX_NUMERUS },
    { "forma-verbi", ORATIO_TITULI_FORMARUM_VERBI,
        (i32)ORATIO_FORMA_VERBI_NUMERUS },
    { "gradus",      ORATIO_TITULI_GRADUUM,
        (i32)ORATIO_GRADUS_NUMERUS },
    { "species",     ORATIO_TITULI_SPECIERUM_NUMERALIS,
        (i32)ORATIO_SPECIES_NUMERALIS_NUMERUS },
    { "declinatio",  NIHIL, ZEPHYRUM },
    { "coniugatio",  NIHIL, ZEPHYRUM }
};
#define COLUMNAE_NUMERUS \
    ((i32)(magnitudo(COLUMNAE) / magnitudo(COLUMNAE[0])))

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

/* vocabularia (la.bin + glossarium + Moby) ex RHUBARB_RADIX; FALSUM
 * cum nuntio in stderr */
interior b32
_vocabularia_onerare (
              Piscina* piscina,
    OratioVocabularia* exitus)
{
    OratioVocabulariumVitium vitium;

    si (!oratio_vocabularia_onerare(piscina, getenv("RHUBARB_RADIX"),
            exitus, &vitium))
    {
        fprintf(stderr, "verba: vocabularia non onerata: %s:%d %s\n",
            vitium.plagula ? vitium.plagula : "?",
            (integer)vitium.linea,
            vitium.causa ? vitium.causa : "-");
        redde FALSUM;
    }
    redde VERUM;
}

/* chorda in stdout, tabulis et lineis novis in spatia versis */
interior vacuum
_chordam_imprimere (
    chorda c)
{
    i32 k;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        character ch = (character)c.datum[k];

        putchar((ch == '\t' || ch == '\n' || ch == '\r') ? ' ' : ch);
    }
}

/* lexema loci TOKEN; chorda vacua si absens */
interior chorda
_lexema_loci (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];
                   chorda  vacua;

    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        redde v->datum.token->valor;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* valor loci INDEX; -I si absens */
interior s32
_index_loci (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus == MATERIA_VALOR_INDEX ? v->datum.index : (s32)-I;
}

/* titulus indicis in serie; vacuum si extra seriem */
interior constans character*
_titulus_indicis (
    constans character* constans* tituli,
                             i32  numerus,
                             s32  index)
{
    si (index < ZEPHYRUM || index >= (s32)numerus)
    {
        redde "";
    }
    redde tituli[index];
}

/* pars prima aut ultima vocabuli (lexema); NIHIL sine partibus */
interior constans MateriaToken*
_pars (
    constans MateriaNodus* vocabulum,
                      b32  ultima)
{
    constans MateriaValor* v =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
                      i32 n;

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    n = materia_valor_lista_numerus(*v);
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*v, ultima ? n - I : ZEPHYRUM)
        ->datum.token;
}

/* forma vocabuli = partes coniunctae in stdout */
interior vacuum
_formam_imprimere (
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* v =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
                      i32 n;
                      i32 k;

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    n = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*v, k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            _chordam_imprimere(e->datum.token->valor);
        }
    }
}

/* linea una vocabuli */
interior vacuum
_vocabulum_imprimere (
       constans character* via,
                      i32  index,
                      i32  paragraphus,
                      i32  sententia,
    constans MateriaNodus* vocabulum)
{
    constans MateriaToken* a = _pars(vocabulum, FALSUM);
    constans MateriaToken* b = _pars(vocabulum, VERUM);
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
                      i32 n = ZEPHYRUM;

    si (a == NIHIL || b == NIHIL)
    {
        redde;
    }
    si (analyses->genus == MATERIA_VALOR_LISTA)
    {
        n = materia_valor_lista_numerus(*analyses);
    }
    imprimere("%s\t%d\t%d\t%d\t%d\t%d\t%d\t", via, (integer)index,
        (integer)a->byte_offset,
        (integer)(b->byte_offset + (s32)b->valor.mensura),
        (integer)a->linea, (integer)paragraphus, (integer)sententia);
    _formam_imprimere(vocabulum);
    putchar('\t');
    _chordam_imprimere(_lexema_loci(vocabulum,
        (i32)ORATIO_VOCABULUM_CLASSES));
    putchar('\t');
    _chordam_imprimere(_lexema_loci(vocabulum,
        (i32)ORATIO_VOCABULUM_LINGUAE));
    putchar('\t');
    si (n > ZEPHYRUM)
    {
        _chordam_imprimere(_lexema_loci(
            materia_valor_lista_obtinere(*analyses,
            ZEPHYRUM)->datum.nodus,
            (i32)ORATIO_ANALYSIS_LEMMA));
    }
    imprimere("\t%d\n", (integer)n);
}

/* lineae analysium vocabuli (-analyses): una per nodum analysis-* */
interior vacuum
_analyses_imprimere (
       constans character* via,
                      i32  index,
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
                      i32 n;
                      i32 a;

    si (analyses->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    n = materia_valor_lista_numerus(*analyses);
    per (a = ZEPHYRUM; a < n; a++)
    {
        constans MateriaNodus* nodus =
            materia_valor_lista_obtinere(*analyses, a)->datum.nodus;
                OratioClassis classis =
                    oratio_genus_classis((OratioGenus)nodus->genus);
           constans character* titulus_classis =
               oratio_classis_titulus(classis);
                          i32 k;

        imprimere("%s\t%d\t", via, (integer)index);
        _formam_imprimere(vocabulum);
        imprimere("\t%s\t", titulus_classis != NIHIL ? titulus_classis
            : "");
        _chordam_imprimere(_lexema_loci(nodus,
            (i32)ORATIO_ANALYSIS_LEMMA));
        imprimere("\t%s\t%s\t",
            _titulus_indicis(ORATIO_TITULI_LINGUARUM,
                (i32)ORATIO_LINGUA_NUMERUS,
                _index_loci(nodus, (i32)ORATIO_ANALYSIS_LINGUA)),
            _titulus_indicis(ORATIO_TITULI_FONTIUM_ANALYSIS,
                (i32)ORATIO_FONS_ANALYSIS_NUMERUS,
                _index_loci(nodus, (i32)ORATIO_ANALYSIS_FONS)));
        _chordam_imprimere(_lexema_loci(nodus,
            (i32)ORATIO_ANALYSIS_NATIVUM));
        putchar('\t');
        _chordam_imprimere(_lexema_loci(nodus,
            (i32)ORATIO_ANALYSIS_SENSUS));
        per (k = ZEPHYRUM; k < COLUMNAE_NUMERUS; k++)
        {
            constans ColumnaAccidentis* c      = &COLUMNAE[k];
                                   s32  locus  = (s32)-I;
                                   s32  valor  = (s32)-I;

            si (classis < ORATIO_CLASSIS_NUMERUS_CLASSIUM)
            {
                locus = oratio_partes_locus(classis, c->titulus);
            }
            si (locus >= ZEPHYRUM)
            {
                valor = _index_loci(nodus, (i32)locus);
            }
            putchar('\t');
            si (valor < ZEPHYRUM)
            {
                perge;
            }
            si (c->tituli == NIHIL)
            {
                imprimere("%d", (integer)valor);
            }
            alioquin
            {
                fputs(_titulus_indicis(c->tituli, c->numerus, valor),
                    stdout);
            }
        }
        putchar('\n');
    }
}

/* arbor ambulata documentum > paragraphus > sententia > vocabulum;
 * numerus vocabulorum. Sententia sine elementis non numeratur (ut
 * sententiae.sh), ergo ordinales congruunt. */
interior i32
_arborem_imprimere (
       constans character* via,
    constans MateriaNodus* radix,
                      b32  analyses_modus)
{
    constans MateriaValor* paragraphi =
        &radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
                      i32 np;
                      i32 p;
                      i32 index      = ZEPHYRUM;
                      i32 sententia  = ZEPHYRUM;

    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    np = materia_valor_lista_numerus(*paragraphi);
    per (p = ZEPHYRUM; p < np; p++)
    {
        constans MateriaNodus* par =
            materia_valor_lista_obtinere(*paragraphi, p)->datum.nodus;
        constans MateriaValor* sententiae =
            &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
                          i32 ns;
                          i32 s;

        si (sententiae->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        ns = materia_valor_lista_numerus(*sententiae);
        per (s = ZEPHYRUM; s < ns; s++)
        {
            constans MateriaNodus* sen =
                materia_valor_lista_obtinere(*sententiae,
                s)->datum.nodus;
            constans MateriaValor* elementa =
                &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
                              i32 ne;
                              i32 e;

            si (elementa->genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            ne = materia_valor_lista_numerus(*elementa);
            si (ne == ZEPHYRUM)
            {
                perge;
            }
            per (e = ZEPHYRUM; e < ne; e++)
            {
                constans MateriaNodus* elementum =
                    materia_valor_lista_obtinere(*elementa,
                    e)->datum.nodus;

                si (elementum->genus != (s32)ORATIO_GENUS_VOCABULUM)
                {
                    perge;
                }
                si (analyses_modus)
                {
                    _analyses_imprimere(via, index, elementum);
                }
                alioquin
                {
                    _vocabulum_imprimere(via, index, p, sententia,
                        elementum);
                }
                index = index + I;
            }
            sententia = sententia + I;
        }
    }
    redde index;
}

integer
principale (
      integer   argc,
    character** argv)
{
                          Piscina* piscina_tabulae;
                OratioVocabularia  vocabularia;
                              b32  machina         = FALSUM;
                              b32  analyses_modus  = FALSUM;
                              b32  crudus          = FALSUM;
              InternamentumChorda* intern;
              MateriaLexiconRatum  ratum;
               MateriaLexIudicium  iudicium;
                  OratioProgramma* programma = NIHIL;
         OratioVocabulariumVitium  vitium;
                          integer  i;
                              i32  viae   = ZEPHYRUM;
                              i32  summa  = ZEPHYRUM;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-analyses") == ZEPHYRUM)
        {
            analyses_modus = VERUM;
        }
        alioquin si (strcmp(argv[i], "-crudus") == ZEPHYRUM)
        {
            crudus = VERUM;   /* T17: ordo fontis, sine resolutione */
        }
        alioquin
        {
            viae = viae + I;
        }
    }
    si (viae == ZEPHYRUM)
    {
        fprintf(stderr,
            "usus: verba <plagula.txt>... [-machina] [-analyses]"
            " [-crudus]\n");
        redde II;
    }
    piscina_tabulae = piscina_generare_dynamicum("oratio_verba_tabula",
        8388608);
    si (!_vocabularia_onerare(piscina_tabulae, &vocabularia))
    {
        redde II;
    }
    /* T17: programma resolutionis (absens = sine resolutione) */
    intern = internamentum_creare(piscina_tabulae);
    si (   intern == NIHIL
        || !materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium))
    {
        fprintf(stderr, "verba: lexicon recusatum\n");
        redde II;
    }
    programma = oratio_resolutio_programma_onerare(piscina_tabulae,
        intern,
        getenv("RHUBARB_RADIX"), &vitium);
    si (   programma == NIHIL && vitium.causa != NIHIL
        && strcmp(vitium.causa, "plagula absens") != ZEPHYRUM)
    {
        fprintf(stderr, "verba: programma resolutionis: %s:%d %s\n",
            vitium.plagula ? vitium.plagula : "?",
            (integer)vitium.linea,
            vitium.causa);
        redde II;
    }
    si (machina)
    {
        fputs(analyses_modus
            ? "# via\tindex\tforma\tclassis\tlemma\tlingua\tfons"
              "\tnativum\tsensus\tcasus\tnumerus\tgenus\tpersona"
              "\ttempus\tmodus\tvox\tforma-verbi\tgradus\tspecies"
              "\tdeclinatio\tconiugatio\n"
            : "# via\tindex\tinitium\tfinis\tlinea\tparagraphus"
              "\tsententia\tforma\tclasses\tlinguae\tlemma\tanalyses\n",
            stdout);
    }
    per (i = I; i < argc; i++)
    {
             Piscina* piscina;
           character* textus;
                 i32  mensura = ZEPHYRUM;
        MateriaNodus* radix;
  OratioPartesCensus  census;

        si (   strcmp(argv[i], "-machina")  == ZEPHYRUM
            || strcmp(argv[i], "-analyses") == ZEPHYRUM
            || strcmp(argv[i], "-crudus")   == ZEPHYRUM)
        {
            perge;
        }
        piscina =
            piscina_generare_dynamicum("oratio_verba_instrumentum",
            33554432);
        textus = _plagulam_legere(piscina, argv[i], &mensura);
        si (textus == NIHIL)
        {
            fprintf(stderr, "verba: plagula absens: %s\n", argv[i]);
            redde II;
        }
        radix = oratio_arbor_parsare(piscina, textus, mensura);
        si (radix == NIHIL)
        {
            fprintf(stderr, "verba: parsura fracta: %s\n", argv[i]);
            redde II;
        }
        si (!oratio_partes_annotare(piscina, &vocabularia, radix,
            &census))
        {
            fprintf(stderr, "verba: annotatio fracta: %s\n", argv[i]);
            redde II;
        }
        si (   programma != NIHIL && !crudus
            && !oratio_resolutio_applicare(piscina, intern, &ratum,
            programma,
                (s32)-I, oratio_resolutio_lingua_censu(census.linguae),
                radix,
                NIHIL))
        {
            fprintf(stderr, "verba: resolutio fracta: %s\n", argv[i]);
            redde II;
        }
        summa = summa + _arborem_imprimere(argv[i], radix,
            analyses_modus);
        piscina_destruere(piscina);
    }
    piscina_destruere(piscina_tabulae);
    redde summa > ZEPHYRUM ? ZEPHYRUM : I;
}
