/* runae_corpus.c - Specimen textus ex pagina Lapidis (runae U1)
 *
 * Usus: runae_corpus <pagina.html> <specimen.txt>
 *
 * Parsator html DOMUS (html_arbor_parsare, materia) paginam legit;
 * arbor ambulatur et omnis <p> ordine documenti colligitur: textus et
 * referentiae decocta (entitates_html_decoquere, ut html_coctum),
 * posteri inclusi (<b>, <a>), <br> = linea nova, cursus spatiorum
 * ASCII in spatium unum contracti (U+3000 et spatia non-ASCII manent),
 * paragraphi purgati et linea vacua separati. Paragraphi INTEGRI
 * usque ad SPECIMEN_MINIMUM octetorum.
 *
 * Instrumentum aetatis evolutionis: tools/runae_corpus.sh id contra
 * obiecta html/build/ struit (ut html/arbor.sh) et per linguas XXXV
 * currit. Paginae Lapidis mundae sunt (nulla sedes alibi, nulla
 * adoptio) - ambulatio arborem octetorum sequitur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "entitates_html.h"
#include "materia_nodus.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include <stdio.h>
#include <string.h>

#define SPECIMEN_MINIMUM (IV * MXXIV)

nomen structura {
    ChordaAedificator* paragraphus;
                  b32  spatium_pendens;
                  b32  initium_lineae;   /* nihil post initium/lineam novam */
} Collector;


/* ==================================================
 * Filum
 * ================================================== */

interior character*
_filum_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f;
        longus  longitudo;
    character*  memoria;
        size_t  lecti;

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


/* ==================================================
 * Arbor
 * ================================================== */

interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(nodus->loci[locus]);
}

interior MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

/* Elementum hoc titulo (litteris neglectis)? Apertura = "<titulus" */
interior b32
_titulus_est (
       constans MateriaNodus* nodus,
          constans character* titulus)
{
    MateriaToken* apertura;
             i32  longitudo = (i32)strlen(titulus);
             i32  i;

    si (nodus->genus != (s32)HTML_GENUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    apertura = _tok(nodus, (i32)HTML_ELEMENTUM_TOK_APERTURA);
    si (apertura == NIHIL || apertura->valor.mensura != longitudo + I)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        i8 c = apertura->valor.datum[i + I];

        si (c >= 'A' && c <= 'Z')
        {
            c = (i8)(c + ('a' - 'A'));
        }
        si ((character)c != titulus[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Collector paragraphi
 * ================================================== */

interior vacuum
_octetum_addere (
    Collector* c,
           i8  octetus)
{
    si (   octetus == ' ' || octetus == '\t' || octetus == '\n'
        || octetus == '\r' || octetus == '\f')
    {
        si (!c->initium_lineae)
        {
            c->spatium_pendens = VERUM;
        }
        redde;
    }
    si (octetus == ZEPHYRUM)
    {
        redde;
    }
    si (c->spatium_pendens)
    {
        chorda_aedificator_appendere_character(c->paragraphus, ' ');
        c->spatium_pendens = FALSUM;
    }
    chorda_aedificator_appendere_character(c->paragraphus,
        (character)octetus);
    c->initium_lineae = FALSUM;
}

/* Textus aut referentia: '&...;' decoctum ut in html_coctum
 * (referentia ignota cruda manet) */
interior vacuum
_textum_addere (
    Collector* c,
       chorda  textus)
{
    s32 i = ZEPHYRUM;

    dum (i < (s32)textus.mensura)
    {
        si (textus.datum[i] == '&')
        {
            character exitus[XVI];
                  i32 longitudo;
                  s32 post;

            si (entitates_html_decoquere(
                    (constans character*)textus.datum, i,
                    (s32)textus.mensura, FALSUM, exitus, &longitudo,
                    &post))
            {
                i32 k;

                per (k = ZEPHYRUM; k < longitudo; k++)
                {
                    _octetum_addere(c, (i8)exitus[k]);
                }
                i = post;
                perge;
            }
        }
        _octetum_addere(c, textus.datum[i]);
        i = i + I;
    }
}

/* Posteros paragraphi colligere */
interior vacuum
_colligere (
                 Collector* c,
     constans MateriaNodus* nodus)
{
    i32 n = _numerus(nodus, (i32)HTML_ELEMENTUM_LIBERI);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* liber = _liber(nodus, (i32)HTML_ELEMENTUM_LIBERI,
            i);

        si (liber == NIHIL)
        {
            perge;
        }
        si (   liber->genus == (s32)HTML_GENUS_TEXTUS
            || liber->genus == (s32)HTML_GENUS_REFERENTIA)
        {
            MateriaToken* tok = _tok(liber, ZEPHYRUM);

            si (tok != NIHIL)
            {
                _textum_addere(c, tok->valor);
            }
        }
        alioquin si (_titulus_est(liber, "br"))
        {
            chorda_aedificator_appendere_character(c->paragraphus,
                '\n');
            c->spatium_pendens  = FALSUM;
            c->initium_lineae   = VERUM;
        }
        alioquin si (liber->genus == (s32)HTML_GENUS_ELEMENTUM)
        {
            _colligere(c, liber);
        }
    }
}

/* Paragraphos quaerere ordine documenti; specimen crescit usque ad
 * SPECIMEN_MINIMUM (paragraphi integri) */
interior vacuum
_paragraphos_quaerere (
                   Piscina* piscina,
    constans MateriaNodus* nodus,
                      i32  locus,
        ChordaAedificator* specimen)
{
    i32 n = _numerus(nodus, locus);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* liber = _liber(nodus, locus, i);

        si (chorda_aedificator_longitudo(specimen) >= SPECIMEN_MINIMUM)
        {
            redde;
        }
        si (liber == NIHIL || liber->genus != (s32)HTML_GENUS_ELEMENTUM)
        {
            perge;
        }
        si (_titulus_est(liber, "p"))
        {
            Collector c;
               chorda p;

            c.paragraphus = chorda_aedificator_creare(piscina,
                CCLVI);
            c.spatium_pendens  = FALSUM;
            c.initium_lineae   = VERUM;
            _colligere(&c, liber);
            p = chorda_aedificator_spectare(c.paragraphus);
            dum (p.mensura > ZEPHYRUM && p.datum[p.mensura - I] == '\n')
            {
                p.mensura--;   /* linea nova ultima (br) */
            }
            si (p.mensura > ZEPHYRUM)
            {
                si (chorda_aedificator_longitudo(specimen) > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(specimen,
                        "\n\n");
                }
                chorda_aedificator_appendere_chorda(specimen, p);
            }
        }
        alioquin
        {
            _paragraphos_quaerere(piscina, liber,
                (i32)HTML_ELEMENTUM_LIBERI, specimen);
        }
    }
}


integer
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
             character* fons;
                   i32  mensura = ZEPHYRUM;
         MateriaNodus* radix;
    ChordaAedificator* specimen;
                chorda  exitus;
                 FILE* f;

    si (argc != III)
    {
        fprintf(stderr,
            "usus: runae_corpus <pagina.html> <specimen.txt>\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("runae_corpus", 16777216);
    si (piscina == NIHIL)
    {
        redde I;
    }
    fons = _filum_legere(piscina, argv[I], &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "runae_corpus: legi non potest: %s\n", argv[I]);
        piscina_destruere(piscina);
        redde II;
    }
    radix = html_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        fprintf(stderr, "runae_corpus: parsura defecit: %s\n", argv[I]);
        piscina_destruere(piscina);
        redde I;
    }
    specimen = chorda_aedificator_creare(piscina, VIII * MXXIV);
    _paragraphos_quaerere(piscina, radix, (i32)HTML_DOCUMENTUM_LIBERI,
        specimen);
    chorda_aedificator_appendere_character(specimen, '\n');
    exitus = chorda_aedificator_spectare(specimen);

    f = fopen(argv[II], "wb");
    si (   f == NIHIL
        || fwrite(exitus.datum, I, (size_t)exitus.mensura, f)
               != (size_t)exitus.mensura)
    {
        fprintf(stderr, "runae_corpus: scribi non potest: %s\n",
            argv[II]);
        si (f != NIHIL)
        {
            fclose(f);
        }
        piscina_destruere(piscina);
        redde I;
    }
    fclose(f);
    printf("%s: %u octeti\n", argv[II],
        (insignatus integer)exitus.mensura);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
