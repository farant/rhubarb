/* probatio_oratio_arbor.c - Arbor orationis (T3)
 *
 * Fixturae structurae: paragraphi per lineas vacuas (caudae elementi et
 * paragraphi sub lege possessoris, praefixa), sententiae v1 (casus
 * quos sententia_fissio nominat: abbreviationes, citationes, decimales,
 * ellipsis, attributio dialogi; initiales; punctum intra vocabulum;
 * capitalis ignota >= 0x80), partes vocabuli et numeri; circuitus
 * octetorum (parsare -> materia_scribere_nodum == fons) in omnibus et
 * in plagula vacua / spatii solius / CRLF. CORPUS: omnis paragraphus
 * markdown tractus (extentum per arborem md, non per STML) parsatur
 * et emittitur octetim; numeri publicati. Culpa plantata: tabula
 * abbreviationum sine 'mr'.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_lexicon.h"
#include "oratio_registrum.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_scribere.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* parsare -> emittere == fons */
interior b32
_circuitus (
               Piscina*  piscina,
    constans character*  fons,
                   i32   mensura,
          MateriaNodus** radix)
{
    MateriaScripturaConsilium consilium;
             MateriaScriptura emissa;

    *radix = oratio_arbor_parsare(piscina, fons, mensura);
    si (*radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &ORATIO_REGISTRUM);
    consilium.fons_index = ORATIO_FONS_PLAGULAE;
    emissa = materia_scribere_nodum(piscina, *radix, &consilium);
    si (!emissa.successus || emissa.textus.mensura != mensura)
    {
        redde FALSUM;
    }
    redde (b32)(mensura == ZEPHYRUM
        || memcmp(emissa.textus.datum, fons, (size_t)mensura)
            == ZEPHYRUM);
}

interior i32
_numerus_listae (
    constans MateriaNodus* n,
                      i32  locus)
{
    constans MateriaValor* v = &n->loci[locus];

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(*v);
}

interior constans MateriaNodus*
_nodus_listae (
    constans MateriaNodus* n,
                      i32  locus,
                      i32  i)
{
    redde materia_valor_lista_obtinere(n->loci[locus], i)->datum.nodus;
}

/* lexemata loci LISTA_TOKEN concatenata == literae */
interior b32
_lexemata_sunt (
    constans MateriaNodus* n,
                      i32  locus,
       constans character* literae)
{
    constans MateriaValor* v = &n->loci[locus];
                      i32  m;
                      i32  k;
                      i32  cursor  = ZEPHYRUM;
                      i32  l       = (i32)strlen(literae);

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde (b32)(l == ZEPHYRUM);
    }
    m = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaToken* t = materia_valor_lista_obtinere(*v,
            k)->datum.token;

        si (   cursor + t->valor.mensura > l
            || memcmp(literae + cursor, t->valor.datum,
            (size_t)t->valor.mensura)
                != ZEPHYRUM)
        {
            redde FALSUM;
        }
        cursor = cursor + t->valor.mensura;
    }
    redde (b32)(cursor == l);
}

interior constans MateriaNodus*
_paragraphus (
    constans MateriaNodus* doc,
                      i32  i)
{
    redde _nodus_listae(doc, (i32)ORATIO_DOCUMENTUM_PARAGRAPHI, i);
}

interior i32
_sententiae (
    constans MateriaNodus* par)
{
    redde _numerus_listae(par, (i32)ORATIO_PARAGRAPHUS_SENTENTIAE);
}

interior constans MateriaNodus*
_elementum (
    constans MateriaNodus* par,
                      i32  s,
                      i32  e)
{
    redde _nodus_listae(_nodus_listae(par,
        (i32)ORATIO_PARAGRAPHUS_SENTENTIAE, s),
        (i32)ORATIO_SENTENTIA_ELEMENTA, e);
}

interior i32
_elementa (
    constans MateriaNodus* par,
                      i32  s)
{
    redde _numerus_listae(_nodus_listae(par,
        (i32)ORATIO_PARAGRAPHUS_SENTENTIAE, s),
        (i32)ORATIO_SENTENTIA_ELEMENTA);
}

/* sententiae paragraphi primi documenti dati */
interior i32
_sententiae_textus (
               Piscina* piscina,
    constans character* fons)
{
    MateriaNodus* d;

    si (!_circuitus(piscina, fons, (i32)strlen(fons), &d))
    {
        redde (i32)9999;
    }
    si (_numerus_listae(d, (i32)ORATIO_DOCUMENTUM_PARAGRAPHI) != I)
    {
        redde (i32)9998;
    }
    redde _sententiae(_paragraphus(d, ZEPHYRUM));
}

/* extentum paragraphi md: lexema fontis primum..ultimum subarboris */
interior vacuum
_extentum (
    constans MateriaNodus* n,
                      s32* initium,
                      s32* finis)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _extentum(v->datum.nodus, initium, finis);
        }
        alioquin si (v->genus == MATERIA_VALOR_TOKEN)
        {
            constans MateriaToken* t = v->datum.token;

            si (   t->fons_index  == MD_FONS_PLAGULAE
                && t->byte_offset >= ZEPHYRUM)
            {
                si (*initium < ZEPHYRUM || t->byte_offset < *initium)
                {
                    *initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->valor.mensura > *finis)
                {
                    *finis = t->byte_offset + (s32)t->valor.mensura;
                }
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _extentum(e->datum.nodus, initium, finis);
                }
                alioquin si (   e->genus == MATERIA_VALOR_TOKEN
                             && e->datum.token->fons_index
                                 == MD_FONS_PLAGULAE
                             && e->datum.token->byte_offset >= ZEPHYRUM)
                {
                    constans MateriaToken* t = e->datum.token;

                    si (   *initium < ZEPHYRUM
                        || t->byte_offset < *initium)
                    {
                        *initium = t->byte_offset;
                    }
                    si (t->byte_offset + (s32)t->valor.mensura > *finis)
                    {
                        *finis = t->byte_offset + (s32)t->valor.mensura;
                    }
                }
            }
        }
    }
}

nomen structura {
    i32 paragraphi;
    i32 fracti;
    s64 sententiae;
    s64 vocabula;
} Census;

/* paragraphos md ambulare: quisque per oratio parsatur et emittitur */
interior vacuum
_paragraphos_probare (
                  Piscina* piscina,
    constans MateriaNodus* n,
       constans character* fons,
                   Census* c)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    si (n->genus == (s32)MD_GENUS_PARAGRAPHUS)
    {
        s32 a = (s32)-I;
        s32 b = ZEPHYRUM;
        MateriaNodus* d;

        _extentum(n, &a, &b);
        si (a >= ZEPHYRUM && b > a)
        {
            c->paragraphi++;
            si (!_circuitus(piscina, fons + a, (i32)(b - a), &d))
            {
                c->fracti++;
            }
            alioquin
            {
                i32 np = _numerus_listae(d,
                    (i32)ORATIO_DOCUMENTUM_PARAGRAPHI);
                i32 k;

                per (k = ZEPHYRUM; k < np; k++)
                {
                    constans MateriaNodus* par  = _paragraphus(d, k);
                                      i32  ns    =
                                          _sententiae(par);
                                      i32 s;

                    c->sententiae = c->sententiae + (s64)ns;
                    per (s = ZEPHYRUM; s < ns; s++)
                    {
                        i32 ne = _elementa(par, s);
                        i32 e;

                        per (e = ZEPHYRUM; e < ne; e++)
                        {
                            si (_elementum(par, s, e)->genus
                                == (s32)ORATIO_GENUS_VOCABULUM)
                            {
                                c->vocabula = c->vocabula + I;
                            }
                        }
                    }
                }
            }
        }
        redde;   /* paragraphi non nidificant */
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _paragraphos_probare(piscina, v->datum.nodus, fons, c);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    _paragraphos_probare(piscina, e->datum.nodus, fons,
                        c);
                }
            }
        }
    }
}

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    MateriaNodus* d;
    constans character* radix_viae;

    piscina = piscina_generare_dynamicum("probatio_oratio_arbor",
        8388608);
    credo_aperire(piscina);

    imprimere("\n--- Probans paragraphos, caudas, praefixa ---\n");
    {
        constans character* f =
            "Hello world. This is it.\n\nSecond para.\n";
        constans MateriaNodus* p1;
        constans MateriaNodus* p2;

        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &d));
        CREDO_AEQUALIS_I32 (_numerus_listae(d,
            (i32)ORATIO_DOCUMENTUM_PARAGRAPHI), II);
        p1 = _paragraphus(d, ZEPHYRUM);
        p2 = _paragraphus(d, I);
        CREDO_AEQUALIS_I32 (_sententiae(p1), II);
        CREDO_AEQUALIS_I32 (_sententiae(p2), I);
        CREDO_AEQUALIS_I32 (_elementa(p1, ZEPHYRUM), III);   /* Hello world . */
        CREDO_VERUM (_lexemata_sunt(_elementum(p1, ZEPHYRUM, ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_PARTES, "Hello"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p1, ZEPHYRUM, ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_CAUDA, " "));
        CREDO_VERUM (_lexemata_sunt(_elementum(p1, ZEPHYRUM, I),
            (i32)ORATIO_VOCABULUM_CAUDA, ""));
        CREDO_AEQUALIS_S32 (_elementum(p1, ZEPHYRUM, II)->genus,
            (s32)ORATIO_GENUS_INTERPUNCTIO);
        CREDO_VERUM (_lexemata_sunt(_elementum(p1, ZEPHYRUM, II),
            (i32)ORATIO_INTERPUNCTIO_CAUDA, " "));
        /* 'it.' : punctum cauda "\n" (linea prima), paragraphus cauda "\n" */
        CREDO_AEQUALIS_I32 (_elementa(p1, I), IV);
        CREDO_VERUM (_lexemata_sunt(_elementum(p1, I, III),
            (i32)ORATIO_INTERPUNCTIO_CAUDA, "\n"));
        CREDO_VERUM (_lexemata_sunt(p1, (i32)ORATIO_PARAGRAPHUS_CAUDA,
            "\n"));
        CREDO_VERUM (_lexemata_sunt(p2,
            (i32)ORATIO_PARAGRAPHUS_PRAEFIXA, ""));
        CREDO_VERUM (_lexemata_sunt(p2, (i32)ORATIO_PARAGRAPHUS_CAUDA,
            ""));
        CREDO_VERUM (_lexemata_sunt(_elementum(p2, ZEPHYRUM, II),
            (i32)ORATIO_INTERPUNCTIO_CAUDA, "\n"));
        CREDO_VERUM (_lexemata_sunt(d, (i32)ORATIO_DOCUMENTUM_PRAEFIXA,
            ""));
    }
    {
        constans character* f = "  Indented start.\n\n\n  Next one\n";

        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &d));
        CREDO_AEQUALIS_I32 (_numerus_listae(d,
            (i32)ORATIO_DOCUMENTUM_PARAGRAPHI), II);
        CREDO_VERUM (_lexemata_sunt(d, (i32)ORATIO_DOCUMENTUM_PRAEFIXA,
            "  "));
        CREDO_VERUM (_lexemata_sunt(_paragraphus(d, ZEPHYRUM),
            (i32)ORATIO_PARAGRAPHUS_CAUDA, "\n\n"));
        CREDO_VERUM (_lexemata_sunt(_paragraphus(d, I),
            (i32)ORATIO_PARAGRAPHUS_PRAEFIXA, "  "));
        CREDO_AEQUALIS_I32 (_sententiae(_paragraphus(d, I)), I);
        CREDO_VERUM (_lexemata_sunt(_elementum(_paragraphus(d, I),
            ZEPHYRUM, I),
            (i32)ORATIO_VOCABULUM_CAUDA, "\n"));
    }

    imprimere("\n--- Probans sententias v1 ---\n");
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "Mr. Smith went home. He slept."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "\"Hello!\" she said. Then left."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "It cost $3.50. Cheap."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina, "Wait... What?"),
        II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "Wait... what? Ok."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "See e.g. the docs. Done."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "See e.g. The docs."), I);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "J. Smith came. Then went."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina, "(Really.) Yes."),
        II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "Vidit? \xC3\x86neas venit."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "No. 5 is here. Ok."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina, "One! Two? Three."),
        III);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "He said \xE2\x80\x9Cgo.\xE2\x80\x9D She went."), II);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina,
        "Ends without period"), I);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina, "a. b. c."), I);
    CREDO_AEQUALIS_I32 (_sententiae_textus(piscina, "In 1900. 5 men."),
        II);

    imprimere("\n--- Probans partes vocabuli et numeri ---\n");
    {
        constans character* f =
            "well-known don't e.g B2 1980s 1,024 3.14 x";
        constans MateriaNodus* p;

        CREDO_VERUM (_circuitus(piscina, f, (i32)strlen(f), &d));
        p = _paragraphus(d, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_sententiae(p), I);
        CREDO_AEQUALIS_I32 (_elementa(p, ZEPHYRUM), VIII);
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_PARTES, "well-known"));
        CREDO_AEQUALIS_I32 (_numerus_listae(_elementum(p, ZEPHYRUM,
            ZEPHYRUM),
            (i32)ORATIO_VOCABULUM_PARTES), III);
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, I),
            (i32)ORATIO_VOCABULUM_PARTES, "don't"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, II),
            (i32)ORATIO_VOCABULUM_PARTES, "e.g"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, III),
            (i32)ORATIO_VOCABULUM_PARTES, "B2"));
        CREDO_AEQUALIS_S32 (_elementum(p, ZEPHYRUM, IV)->genus,
            (s32)ORATIO_GENUS_NUMERUS);
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, IV),
            (i32)ORATIO_NUMERUS_CRUDUM, "1980s"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, V),
            (i32)ORATIO_NUMERUS_CRUDUM, "1,024"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, VI),
            (i32)ORATIO_NUMERUS_CRUDUM, "3.14"));
        CREDO_VERUM (_lexemata_sunt(_elementum(p, ZEPHYRUM, VII),
            (i32)ORATIO_VOCABULUM_PARTES, "x"));
    }

    imprimere("\n--- Probans circuitus marginales ---\n");
    CREDO_VERUM (_circuitus(piscina, "", ZEPHYRUM, &d));
    CREDO_AEQUALIS_I32 (_numerus_listae(d,
        (i32)ORATIO_DOCUMENTUM_PARAGRAPHI), ZEPHYRUM);
    CREDO_VERUM (_circuitus(piscina, "\n\n \t\n", V, &d));
    CREDO_AEQUALIS_I32 (_numerus_listae(d,
        (i32)ORATIO_DOCUMENTUM_PARAGRAPHI), ZEPHYRUM);
    CREDO_VERUM (_lexemata_sunt(d, (i32)ORATIO_DOCUMENTUM_PRAEFIXA,
        "\n\n \t\n"));
    CREDO_VERUM (_circuitus(piscina, "a.\r\n\r\nb.\r\n", XI, &d));
    CREDO_AEQUALIS_I32 (_numerus_listae(d,
        (i32)ORATIO_DOCUMENTUM_PARAGRAPHI), II);
    CREDO_VERUM (_circuitus(piscina, "x\x01\x02y \xFF\xFE z", IX, &d));
    CREDO_VERUM (oratio_abbreviatio_est("Mr", II));
    CREDO_VERUM (oratio_abbreviatio_est("etc", III));
    CREDO_VERUM (oratio_abbreviatio_est("J", I));
    CREDO_FALSUM (oratio_abbreviatio_est("j", I));
    CREDO_FALSUM (oratio_abbreviatio_est("Smith", V));

    imprimere("\n--- CORPUS: paragraphi md tracti, circuitus octetorum ---\n");
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    {
        character via[DXII];
        FILE* lista;
        Census c;
        i32 plagulae = ZEPHYRUM;

        memset(&c, ZEPHYRUM, magnitudo(c));
        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                  Piscina* p;
                character* textus;
                      i32  mensura  = ZEPHYRUM;
                   size_t  l        = strlen(linea);
                MateriaNodus* md;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("oratio_corpus",
                    33554432);
                sprintf(via, "%s/%s", radix_viae, linea);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                plagulae++;
                md = md_arbor_parsare(p, textus, mensura);
                si (md != NIHIL)
                {
                    i32 fracti_ante = c.fracti;

                    _paragraphos_probare(p, md, textus, &c);
                    si (c.fracti > fracti_ante && c.fracti <= X)
                    {
                        imprimere("    FRACTA: %s\n", linea);
                    }
                }
                piscina_destruere(p);
            }
            fclose(lista);
            imprimere("    plagulae %d, paragraphi %d, fracti %d, sententiae %ld,"
                      " vocabula %ld\n", (integer)plagulae,
                      (integer)c.paragraphi,
                (integer)c.fracti, (longus)c.sententiae,
                (longus)c.vocabula);
            CREDO_MAIOR_I32 (c.paragraphi, (i32)50000);
            CREDO_AEQUALIS_I32 (c.fracti, ZEPHYRUM);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
