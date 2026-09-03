/* probatio_md_arbor.c - Bloci folia (A3)
 *
 * DUO ORACULA: structura (genera blocorum, gradus, forma, numeri
 * liberorum) contra exempla CommonMark par. 4, et OCTETI: parsura ->
 * materia_scribere_nodum -> memcmp contra fontem - super fixturas ET
 * corpus TOTUM (MCXXII plagulae): lex octetorum non pendet ex
 * structura agnita, ergo porta corporis iam in A3 currit etsi listae
 * paragraphi sunt. Viride INTERROGATUM: capitula, saepta, praefationes
 * numerantur et asseruntur > 0 (numquam 'N/N mundae' sine praesentia).
 */

#include "latina.h"
#include "credo.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* parsura -> emissio -> memcmp */
interior MateriaNodus*
_circuitus (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   b32* octeti_sani)
{
              MateriaNodus* radix;
           MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    *octeti_sani  = FALSUM;
    radix         = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &MD_REGISTRUM);
    consilium.fons_index = MD_FONS_PLAGULAE;   /* derivata (fons I) omissa */
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    {
        imprimere("    (emissio: %s)\n",
            emissa.causa ? emissa.causa : "-");
        redde radix;
    }
    si (emissa.textus.mensura != mensura)
    {
        imprimere("    (emissa %d contra fontem %d)\n",
            (integer)emissa.textus.mensura, (integer)mensura);
        redde radix;
    }
    *octeti_sani = (b32)(mensura == ZEPHYRUM
        || memcmp(emissa.textus.datum, fons, (size_t)mensura)
            == ZEPHYRUM);
    si (!*octeti_sani)
    {
        i32 k;

        per (k = ZEPHYRUM; k < mensura; k++)
        {
            si (emissa.textus.datum[k] != fons[k])
            {
                frange;
            }
        }
        imprimere("    (octetus %d dispar)\n", (integer)k);
    }
    redde radix;
}

/* Emissio OMNIUM fontium (fons_index -I): cum derivatis adest, octeti
 * fonte LONGIORES - probat mechanismum omissionis, non parsuram. */
interior i32
_emissa_omnia (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
              MateriaNodus* radix = md_arbor_parsare(piscina, fons,
                  mensura);
             MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    si (radix == NIHIL)
    {
        redde (i32)-I;
    }
    materia_scriptura_consilium_nudum(&consilium, &MD_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    redde emissa.successus ? emissa.textus.mensura : (i32)-I;
}

interior b32
_valor_aequalis (
    constans MateriaNodus* nodus,
                      i32  locus,
       constans character* litterae)
{
    constans MateriaToken* t;

    si (nodus->loci[locus].genus != MATERIA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    t = nodus->loci[locus].datum.token;
    redde (b32)(t->valor.mensura == (i32)strlen(litterae)
                && memcmp(t->valor.datum, litterae,
                (size_t)t->valor.mensura) == ZEPHYRUM);
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde materia_valor_lista_numerus(nodus->loci[locus]);
}

interior MateriaNodus*
_elementum (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v = materia_valor_lista_obtinere(nodus->loci[locus],
        i);

    redde (v != NIHIL
        && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus
                                                          : NIHIL;
}

interior s32
_genus_bloci (
    constans MateriaNodus* doc,
                      i32  i)
{
    MateriaNodus* b = _elementum(doc, (i32)MD_DOCUMENTUM_BLOCI, i);

    redde b ? b->genus : (s32)-I;
}

interior s32
_index (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_INDEX
        ? nodus->loci[locus].datum.index : (s32)-I;
}

interior b32
_adest (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (b32)(nodus->loci[locus].genus != MATERIA_VALOR_NIHIL);
}

/* Numerus nodorum generis dati, recursivus */
interior i32
_genera_numerare (
    constans MateriaNodus* nodus,
                      s32  genus)
{
    i32 summa;
    i32 i;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    summa = (nodus->genus == genus) ? I : ZEPHYRUM;
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            summa = summa + _genera_numerare(v->datum.nodus, genus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    summa = summa + _genera_numerare(e->datum.nodus,
                        genus);
                }
            }
        }
    }
    redde summa;
}

/* Saepta indentata numerare; prima X sedes (linea) imprimere */
interior i32
_indentata_numerare (
    constans MateriaNodus* nodus,
       constans character* via,
                      i32* impressa)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (   nodus->genus == (s32)MD_GENUS_SAEPTUM
        && _index(nodus, (i32)MD_SAEPTUM_FORMA)
            == (s32)MD_SAEPTUM_INDENTATUS)
    {
        summa = I;
        si (*impressa < X)
        {
            MateriaNodus* l = _elementum(nodus, (i32)MD_SAEPTUM_LINEAE,
                ZEPHYRUM);
            i32 linea = ZEPHYRUM;

            si (   l != NIHIL
                && materia_valor_lista_numerus(l->loci[MD_LINEA_CONTENTUM])
                    > ZEPHYRUM)
            {
                linea =
                    materia_valor_lista_obtinere(l->loci[MD_LINEA_CONTENTUM],
                    ZEPHYRUM)->datum.token->linea;
            }
            imprimere("    indentatum: %s:%d\n", via, (integer)linea);
            *impressa = *impressa + I;
        }
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            summa = summa + _indentata_numerare(v->datum.nodus, via,
                impressa);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    summa = summa + _indentata_numerare(e->datum.nodus,
                        via,
                        impressa);
                }
            }
        }
    }
    redde summa;
}

#define PARSA(lit) _circuitus(piscina, (lit), (i32)strlen(lit), &sani)

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
                   b32  sani;
    constans character* radix_viae;

    piscina = piscina_generare_dynamicum("probatio_md_arbor", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }


    /* ========================================================
     * PROBARE: capitula ATX (par. 4.2)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* c;

        imprimere("\n--- Probans capitula ATX ---\n");

        d = PARSA("# Titulus\n");
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_CAPITULUM);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CAPITULUM_GRADUS),
            (s32)I);
        CREDO_VERUM (_adest(c, (i32)MD_CAPITULUM_MARCA));
        CREDO_FALSUM (_adest(c, (i32)MD_CAPITULUM_CLAUSUM));
        CREDO_FALSUM (_adest(c, (i32)MD_CAPITULUM_SUBDUCTIO));
        CREDO_VERUM (_adest(c, (i32)MD_CAPITULUM_FINIS));
        CREDO_AEQUALIS_I32 (_numerus(c->loci[MD_CAPITULUM_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), I);
        CREDO_VERUM (_adest(d, (i32)MD_DOCUMENTUM_FINIS));

        d = PARSA("## foo ##\n#\n   ### x\n####### y\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), IV);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CAPITULUM_GRADUS),
            (s32)II);
        CREDO_VERUM (_adest(c, (i32)MD_CAPITULUM_CLAUSUM));
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, I);   /* '#' solus */
        CREDO_AEQUALIS_S32 (c->genus, (s32)MD_GENUS_CAPITULUM);
        CREDO_AEQUALIS_I32 (_numerus(c->loci[MD_CAPITULUM_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), ZEPHYRUM);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, II);  /* indentatum III */
        CREDO_AEQUALIS_S32 (c->genus, (s32)MD_GENUS_CAPITULUM);
        CREDO_AEQUALIS_I32 (_numerus(c, (i32)MD_CAPITULUM_PRAEFIXA), I);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, III),
            (s32)MD_GENUS_PARAGRAPHUS);
    }


    /* ========================================================
     * PROBARE: paragraphi, lineae vacuae, continuatio (par. 4.8)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* par;
        MateriaNodus* inl;
        MateriaNodus* fr;

        imprimere("\n--- Probans paragraphos ---\n");

        d = PARSA("Para one\ncontinued\n\nSecond\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_LINEA_VACUA);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);
        par = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(par, (i32)MD_PARAGRAPHUS_NUDUS),
            ZEPHYRUM);
        CREDO_VERUM (_adest(par, (i32)MD_PARAGRAPHUS_FINIS));
        inl = par->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus;
        CREDO_AEQUALIS_I32 (_numerus(inl, (i32)MD_INLINEA_LIBERI), III);
        CREDO_AEQUALIS_S32 (_elementum(inl, (i32)MD_INLINEA_LIBERI,
            I)->genus,
            (s32)MD_GENUS_FRACTURA_MOLLIS);

        /* continuatio indentata: praefixa in fractura-mollis */
        d = PARSA("a\n    b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        par  = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        inl  = par->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus;
        fr   = _elementum(inl, (i32)MD_INLINEA_LIBERI, I);
        CREDO_AEQUALIS_I32 (_numerus(fr, (i32)MD_MOLLIS_PRAEFIXA), I);

        /* sine terminatore ultimo: finis absens */
        d = PARSA("a");
        CREDO_VERUM (sani);
        par = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_FALSUM (_adest(par, (i32)MD_PARAGRAPHUS_FINIS));

        d = PARSA("");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI),
            ZEPHYRUM);
        d = PARSA("\n  \n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(d,
            (i32)MD_DOCUMENTUM_BLOCI, I),
            (i32)MD_VACUA_PRAEFIXA), I);

        /* CRLF */
        d = PARSA("a\r\nb\r\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
    }


    /* ========================================================
     * PROBARE: setext, divisio (par. 4.3, 4.1)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* c;

        imprimere("\n--- Probans setext et divisiones ---\n");

        d = PARSA("Setext\n===\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (c->genus, (s32)MD_GENUS_CAPITULUM);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CAPITULUM_GRADUS),
            (s32)I);
        CREDO_FALSUM (_adest(c, (i32)MD_CAPITULUM_MARCA));
        CREDO_VERUM (_adest(c, (i32)MD_CAPITULUM_SUBDUCTIO));
        CREDO_VERUM (_adest(c, (i32)MD_CAPITULUM_FINIS));

        /* setext vincit divisionem */
        d = PARSA("Foo\n---\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (c->genus, (s32)MD_GENUS_CAPITULUM);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CAPITULUM_GRADUS),
            (s32)II);

        /* multi-linea setext: inlinea III liberi */
        d = PARSA("Foo\nBar\n===\n");
        CREDO_VERUM (sani);
        c = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(c->loci[MD_CAPITULUM_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), III);

        d = PARSA("---\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_DIVISIO);
        d = PARSA("para\n***\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_DIVISIO);
        d = PARSA("    ***\n");   /* indentatum = codex */
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_SAEPTUM);
    }


    /* ========================================================
     * PROBARE: saepta (par. 4.4, 4.5)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* s;

        imprimere("\n--- Probans saepta ---\n");

        d = PARSA("```c\nint x;\n```\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        s = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (s->genus, (s32)MD_GENUS_SAEPTUM);
        CREDO_AEQUALIS_S32 (_index(s, (i32)MD_SAEPTUM_FORMA),
            (s32)MD_SAEPTUM_SAEPTUS);
        CREDO_VERUM (_adest(s, (i32)MD_SAEPTUM_APERTUM));
        CREDO_VERUM (_adest(s->loci[MD_SAEPTUM_APERTUM].datum.nodus,
            (i32)MD_LIMES_INFO));
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)MD_SAEPTUM_LINEAE), I);
        CREDO_VERUM (_adest(s, (i32)MD_SAEPTUM_CLAUSUM));

        /* non clausum: usque ad finem */
        d = PARSA("```\nfoo\n\nbar\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        s = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)MD_SAEPTUM_LINEAE), III);
        CREDO_FALSUM (_adest(s, (i32)MD_SAEPTUM_CLAUSUM));

        /* clausum longius, tildae, '```' intra saeptum tildarum */
        d = PARSA("~~~~\n```\n~~~~\n");
        CREDO_VERUM (sani);
        s = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)MD_SAEPTUM_LINEAE), I);
        CREDO_VERUM (_adest(s, (i32)MD_SAEPTUM_CLAUSUM));

        /* indentatum: vacuae interiores intra, finales extra */
        d = PARSA("    code\n\n    more\n\nafter\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        s = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(s, (i32)MD_SAEPTUM_FORMA),
            (s32)MD_SAEPTUM_INDENTATUS);
        CREDO_AEQUALIS_I32 (_numerus(s, (i32)MD_SAEPTUM_LINEAE), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_LINEA_VACUA);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* codex indentatus paragraphum NON interrumpit */
        d = PARSA("para\n    still para\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* saeptum paragraphum interrumpit */
        d = PARSA("para\n```\nx\n```\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_SAEPTUM);
    }


    /* ========================================================
     * PROBARE: bloci html (par. 4.6)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* h;

        imprimere("\n--- Probans blocos html ---\n");

        d = PARSA("<div>\nx\n</div>\n\npara\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        h = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (h->genus, (s32)MD_GENUS_HTML);
        CREDO_AEQUALIS_I32 (_numerus(h, (i32)MD_HTML_LINEAE), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_LINEA_VACUA);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* conditio VII paragraphum non interrumpit */
        d = PARSA("para\n<span>\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        /* sed VI interrumpit */
        d = PARSA("para\n<div>\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        /* I: finis in eadem linea */
        d = PARSA("<pre>x</pre>\ntext\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_HTML);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_PARAGRAPHUS);
        /* II: commentum trans lineas */
        d = PARSA("<!-- a\nb -->\nc\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        h = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(h, (i32)MD_HTML_LINEAE), II);
        /* VII: tag integrum solum in linea */
        d = PARSA("<a href=\"x\">\nb\n\nc\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_HTML);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(d,
            (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM), (i32)MD_HTML_LINEAE), II);
        d = PARSA("<a href=\"x\">b\n");   /* non integrum: paragraphus */
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
    }


    /* ========================================================
     * PROBARE: praefatio
     * ======================================================== */

    {
        MateriaNodus* d;

        imprimere("\n--- Probans praefationem ---\n");

        d = PARSA("---\nname: x\n\ndescription: y\n---\nbody\n");
        CREDO_VERUM (sani);
        CREDO_VERUM (_adest(d, (i32)MD_DOCUMENTUM_PRAEFATIO));
        CREDO_AEQUALIS_I32 (_numerus(d->loci[MD_DOCUMENTUM_PRAEFATIO].datum.nodus,
            (i32)MD_PRAEFATIO_LINEAE), III);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* sine clausura: divisio + paragraphus */
        d = PARSA("---\nno close\n");
        CREDO_VERUM (sani);
        CREDO_FALSUM (_adest(d, (i32)MD_DOCUMENTUM_PRAEFATIO));
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_DIVISIO);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_PARAGRAPHUS);
    }


    /* ========================================================
     * PROBARE: continentia (par. 5) - listae, citationes, laxitas
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* li;
        MateriaNodus* e;
        MateriaNodus* q;
        MateriaNodus* par;

        imprimere("\n--- Probans continentia ---\n");

        /* lista stricta: paragraphi nudi */
        d = PARSA("- a\n- b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (li->genus, (s32)MD_GENUS_LISTA);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_GENUS),
            (s32)MD_LISTA_PUNCTATA);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_LAXA), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(li, (i32)MD_LISTA_ELEMENTA), II);
        e = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(e, (i32)MD_ELEMENTUM_OFFICIUM),
            (s32)MD_OFFICIUM_NULLUM);
        CREDO_AEQUALIS_I32 (_numerus(e, (i32)MD_ELEMENTUM_BLOCI), I);
        par = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (par->genus, (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_S32 (_index(par, (i32)MD_PARAGRAPHUS_NUDUS),
            (s32)I);
        CREDO_AEQUALIS_I32 (_numerus(par, (i32)MD_PARAGRAPHUS_PRAEFIXA),
            I);

        /* laxa: vacua inter elementa; paragraphi non nudi */
        d = PARSA("- a\n\n- b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_LAXA), (s32)I);
        e = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(e, (i32)MD_ELEMENTUM_BLOCI), II);
        par = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(par, (i32)MD_PARAGRAPHUS_NUDUS),
            ZEPHYRUM);

        /* vacuae finales listae ad documentum, non ad elementum ultimum */
        d = PARSA("- a\n\npost\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_LAXA), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_LINEA_VACUA);

        /* numerata: initium et delimitator; punctae diversae = listae duae */
        d = PARSA("3) a\n4) b\n");
        CREDO_VERUM (sani);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_GENUS),
            (s32)MD_LISTA_NUMERATA);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_INITIUM),
            (s32)III);
        CREDO_AEQUALIS_I32 (_numerus(li, (i32)MD_LISTA_ELEMENTA), II);
        d = PARSA("* a\n- b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);

        /* nidificatio */
        d = PARSA("- a\n  - b\n    - c\n");
        CREDO_VERUM (sani);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(li, (i32)MD_LISTA_ELEMENTA), I);
        e = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(e, (i32)MD_ELEMENTUM_BLOCI), II);
        li = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, I);
        CREDO_AEQUALIS_S32 (li->genus, (s32)MD_GENUS_LISTA);
        e   = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        li  = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, I);
        CREDO_AEQUALIS_S32 (li->genus, (s32)MD_GENUS_LISTA);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_LAXA), ZEPHYRUM);

        /* continuatio elementi post vacuam: laxa; codex intra elementum */
        d = PARSA("- a\n\n  b\n");
        CREDO_VERUM (sani);
        li = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(li, (i32)MD_LISTA_LAXA), (s32)I);
        e = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(e, (i32)MD_ELEMENTUM_BLOCI), III);
        d = PARSA("- a\n\n      code\n");
        CREDO_VERUM (sani);
        e = _elementum(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_elementum(e, (i32)MD_ELEMENTUM_BLOCI,
            II)->genus,
            (s32)MD_GENUS_SAEPTUM);

        /* continuatio pigra */
        d = PARSA("- a\nb\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        e = _elementum(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        par = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(par->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), III);

        /* elementum vacuum: vacua secunda claudit */
        d = PARSA("-\n  foo\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        e = _elementum(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(e, (i32)MD_ELEMENTUM_BLOCI), II);
        d = PARSA("-\n\n  foo\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* interruptio paragraphi */
        d = PARSA("foo\n- bar\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        d = PARSA("foo\n2. bar\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        d = PARSA("foo\n1. bar\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        d = PARSA("foo\n-\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_CAPITULUM);
        d = PARSA("- - -\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_DIVISIO);

        /* officia GFM */
        d = PARSA("- [ ] a\n- [x] b\n");
        CREDO_VERUM (sani);
        li  = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        e   = _elementum(li, (i32)MD_LISTA_ELEMENTA, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(e, (i32)MD_ELEMENTUM_OFFICIUM),
            (s32)MD_OFFICIUM_APERTUM);
        par = _elementum(e, (i32)MD_ELEMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(par, (i32)MD_PARAGRAPHUS_PRAEFIXA),
            II);
        e = _elementum(li, (i32)MD_LISTA_ELEMENTA, I);
        CREDO_AEQUALIS_S32 (_index(e, (i32)MD_ELEMENTUM_OFFICIUM),
            (s32)MD_OFFICIUM_PERFECTUM);

        /* citationes */
        d = PARSA("> a\n> b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        q = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (q->genus, (s32)MD_GENUS_CITATIO);
        CREDO_AEQUALIS_I32 (_numerus(q, (i32)MD_CITATIO_BLOCI), I);
        par = _elementum(q, (i32)MD_CITATIO_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(
            par->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI, I),
            (i32)MD_MOLLIS_PRAEFIXA), I);
        d = PARSA("> a\nb\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        d = PARSA("> a\n\n> b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II), (s32)MD_GENUS_CITATIO);
        d = PARSA("> - a\n> - b\n");
        CREDO_VERUM (sani);
        q   = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        li  = _elementum(q, (i32)MD_CITATIO_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (li->genus, (s32)MD_GENUS_LISTA);
        CREDO_AEQUALIS_I32 (_numerus(li, (i32)MD_LISTA_ELEMENTA), II);
        d = PARSA("> foo\n> ---\n");
        CREDO_VERUM (sani);
        q = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_elementum(q, (i32)MD_CITATIO_BLOCI,
            ZEPHYRUM)->genus,
            (s32)MD_GENUS_CAPITULUM);
        d = PARSA("> foo\n---\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_DIVISIO);
        d = PARSA("- a\n> b\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_CITATIO);

        /* saeptum intra citationem clauditur cum ea */
        d = PARSA("> ```\n> a\n```\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        q = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_FALSUM (_adest(_elementum(q, (i32)MD_CITATIO_BLOCI,
            ZEPHYRUM),
            (i32)MD_SAEPTUM_CLAUSUM));
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_SAEPTUM);
    }


    /* ========================================================
     * PROBARE: tabulae GFM (par. 4.10)
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* t;
        MateriaNodus* o;
        MateriaNodus* c;

        imprimere("\n--- Probans tabulas ---\n");

        d = PARSA("| a | b |\n| --- | :-: |\n| 1 | 2 |\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        t = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (t->genus, (s32)MD_GENUS_TABULA);
        CREDO_VERUM (_adest(t, (i32)MD_TABULA_CAPUT));
        CREDO_VERUM (_adest(t, (i32)MD_TABULA_SEPARATOR));
        CREDO_AEQUALIS_I32 (_numerus(t, (i32)MD_TABULA_ORDINES), I);
        o = t->loci[MD_TABULA_CAPUT].datum.nodus;
        CREDO_AEQUALIS_I32 (_numerus(o, (i32)MD_ORDO_CELLAE), II);
        CREDO_VERUM (_adest(o, (i32)MD_ORDO_CLAUSUM));
        CREDO_VERUM (_adest(o, (i32)MD_ORDO_FINIS));
        c = _elementum(o, (i32)MD_ORDO_CELLAE, I);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CELLA_ORDINATIO),
            (s32)MD_ORDINATIO_MEDIA);
        CREDO_VERUM (_adest(c, (i32)MD_CELLA_APERTUM));
        CREDO_AEQUALIS_I32 (_numerus(c->loci[MD_CELLA_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), I);
        c = _elementum(o, (i32)MD_ORDO_CELLAE, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_index(c, (i32)MD_CELLA_ORDINATIO),
            (s32)MD_ORDINATIO_NULLA);

        /* sine pipis ducentibus/finalibus; vacua tabulam claudit */
        d = PARSA("a | b\n--|--\n1 | 2\n\npost\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        t = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        o = t->loci[MD_TABULA_CAPUT].datum.nodus;
        c = _elementum(o, (i32)MD_ORDO_CELLAE, ZEPHYRUM);
        CREDO_FALSUM (_adest(c, (i32)MD_CELLA_APERTUM));
        CREDO_FALSUM (_adest(o, (i32)MD_ORDO_CLAUSUM));
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);

        /* caput ex linea ultima paragraphi longioris */
        d = PARSA("para line\n| a |\n| - |\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I), (s32)MD_GENUS_TABULA);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(d,
            (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM)
            ->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), I);
        CREDO_VERUM (_adest(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_PARAGRAPHUS_FINIS));

        /* ordo brevior SUPPLETUS ad numerum capitis (cella sine octetis) */
        d = PARSA("| a | b |\n| - | - |\n| only |\n");
        CREDO_VERUM (sani);
        t = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        o = _elementum(t, (i32)MD_TABULA_ORDINES, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(o, (i32)MD_ORDO_CELLAE), II);
        c = _elementum(o, (i32)MD_ORDO_CELLAE, I);
        CREDO_FALSUM (_adest(c, (i32)MD_CELLA_APERTUM));
        CREDO_AEQUALIS_I32 (_numerus(c->loci[MD_CELLA_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), ZEPHYRUM);

        /* initium blocci alterius tabulam claudit; pipa effugita; numeri dispares */
        d = PARSA("| a |\n| - |\n# h\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_CAPITULUM);
        d = PARSA("| a \\| b |\n| --- |\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_TABULA);
        d = PARSA("| a | b |\n| --- |\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);

        /* sine pipa ulla: setext vincit */
        d = PARSA("Foo\n---\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_CAPITULUM);

        /* intra elementum listae */
        d = PARSA("- | a |\n  | - |\n  | 1 |\n");
        CREDO_VERUM (sani);
        t = _elementum(_elementum(_elementum(d,
            (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM),
            (i32)MD_LISTA_ELEMENTA, ZEPHYRUM), (i32)MD_ELEMENTUM_BLOCI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (t->genus, (s32)MD_GENUS_TABULA);
        CREDO_AEQUALIS_I32 (_numerus(t, (i32)MD_TABULA_ORDINES), I);
        CREDO_AEQUALIS_I32 (_numerus(t->loci[MD_TABULA_CAPUT].datum.nodus,
            (i32)MD_ORDO_PRAEFIXA), I);   /* marca listae */
    }


    /* ========================================================
     * PROBARE: definitiones nexuum (par. 4.7) + lexemata derivata
     * ======================================================== */

    {
        MateriaNodus* d;
        MateriaNodus* def;

        imprimere("\n--- Probans definitiones nexuum ---\n");

        d = PARSA("[foo]: /url \"title\"\n\n[foo]\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_DEFINITIO_NEXUS);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_LINEA_VACUA);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, II),
            (s32)MD_GENUS_PARAGRAPHUS);
        def = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(def, (i32)MD_DEFINITIO_LINEAE), I);
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_TITULUS,
            "foo"));
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_URL,
            "/url"));
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_DESCRIPTIO,
            "title"));
        CREDO_AEQUALIS_S32 (def->loci[MD_DEFINITIO_URL].datum.token->fons_index,
            MD_FONS_DERIVATUS);
        CREDO_AEQUALIS_S32 (def->loci[MD_DEFINITIO_URL].datum.token->byte_offset,
            (s32)-I);
        /* mechanismus omissionis: omnia emissa = fonte longiora */
        CREDO_MAIOR_I32 ((i32)_emissa_omnia(piscina,
            "[foo]: /url \"title\"\n",
            (i32)strlen("[foo]: /url \"title\"\n")),
            (i32)strlen("[foo]: /url \"title\"\n"));

        /* tres lineae, titulus normalizatus, reliquum paragraphus */
        d = PARSA("[Foo  Bar]:\n/url\n'the title'\nrest\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        def = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(def, (i32)MD_DEFINITIO_LINEAE),
            III);
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_TITULUS,
            "foo bar"));
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_DESCRIPTIO,
            "the title"));
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(d,
            (i32)MD_DOCUMENTUM_BLOCI, I)
            ->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus,
            (i32)MD_INLINEA_LIBERI), I);

        /* duae definitiones + paragraphus; '<>' vacua; effugia/entia */
        d = PARSA("[foo]: /a\n[bar]: <>\ntext\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), III);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, I),
            (s32)MD_GENUS_DEFINITIO_NEXUS);
        CREDO_FALSUM (_adest(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI, I),
            (i32)MD_DEFINITIO_DESCRIPTIO));
        d = PARSA("[foo]: /f\\*o \"t&amp;t\"\n");
        CREDO_VERUM (sani);
        def = _elementum(d, (i32)MD_DOCUMENTUM_BLOCI, ZEPHYRUM);
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_URL,
            "/f*o"));
        CREDO_VERUM (_valor_aequalis(def, (i32)MD_DEFINITIO_DESCRIPTIO,
            "t&t"));

        /* non definitiones */
        d = PARSA("[foo]: /url \"title\" extra\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        d = PARSA("[foo]:\n\n/url\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        d = PARSA("[foo](/url)\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_PARAGRAPHUS);
        d = PARSA("text\n[foo]: /url\n");   /* non interrumpit */
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), I);
        /* descriptio mala in linea proxima: definitio sine ea + paragraphus */
        d = PARSA("[foo]: /url\n\"title\" ok\n");
        CREDO_VERUM (sani);
        CREDO_AEQUALIS_I32 (_numerus(d, (i32)MD_DOCUMENTUM_BLOCI), II);
        CREDO_AEQUALIS_S32 (_genus_bloci(d, ZEPHYRUM),
            (s32)MD_GENUS_DEFINITIO_NEXUS);
        CREDO_FALSUM (_adest(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_DEFINITIO_DESCRIPTIO));

        /* intra citationem: praefixa lineae in definitione */
        d = PARSA("> [foo]: /url\n");
        CREDO_VERUM (sani);
        def = _elementum(_elementum(d, (i32)MD_DOCUMENTUM_BLOCI,
            ZEPHYRUM),
            (i32)MD_CITATIO_BLOCI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (def->genus, (s32)MD_GENUS_DEFINITIO_NEXUS);
        CREDO_AEQUALIS_I32 (_numerus(_elementum(def,
            (i32)MD_DEFINITIO_LINEAE, ZEPHYRUM),
            (i32)MD_LINEA_PRAEFIXA), I);
    }


    /* ========================================================
     * PROBARE: corpus totum - octeti et praesentia
     * ======================================================== */

    {
        character via[DXII];
             FILE* lista;
              i32  plagulae = ZEPHYRUM;
              i32  fractae  = ZEPHYRUM;
              i32  absentes = ZEPHYRUM;
              s64  octeti   = ZEPHYRUM;
              i32  capitula = ZEPHYRUM;
              i32  saepta   = ZEPHYRUM;
              i32  indentata = ZEPHYRUM;
              i32  html     = ZEPHYRUM;
              i32  praefationes = ZEPHYRUM;
              i32  divisiones = ZEPHYRUM;
              i32  impressa = ZEPHYRUM;
              i32  listae = ZEPHYRUM;
              i32  elementa = ZEPHYRUM;
              i32 citationes = ZEPHYRUM;
              i32 tabulae = ZEPHYRUM;
              i32 definitiones = ZEPHYRUM;

        imprimere("\n--- PORTA CORPORIS: parsura + emissio, omnes plagulae ---\n");

        sprintf(via, "%s/md/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("md/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                     Piscina* p;
                   character* textus;
                MateriaNodus* d;
                         i32  mensura = ZEPHYRUM;
                      size_t  l = strlen(linea);
                         b32  s;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[l - I]  = '\0';
                    l             = l - I;
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                sprintf(via, "%s/%s", radix_viae, linea);
                p = piscina_generare_dynamicum("corpus_md_arbor",
                    1048576);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    absentes = absentes + I;
                    imprimere("  ABEST: %s\n", linea);
                    piscina_destruere(p);
                    perge;
                }
                plagulae  = plagulae + I;
                octeti    = octeti + (s64)mensura;
                d         = _circuitus(p, textus, mensura, &s);
                si (d == NIHIL || !s)
                {
                    fractae = fractae + I;
                    imprimere("  FRACTA: %s\n", linea);
                }
                alioquin
                {
                    capitula += _genera_numerare(d,
                        (s32)MD_GENUS_CAPITULUM);
                    saepta += _genera_numerare(d,
                        (s32)MD_GENUS_SAEPTUM);
                    html += _genera_numerare(d,
                        (s32)MD_GENUS_HTML);
                    listae += _genera_numerare(d, (s32)MD_GENUS_LISTA);
                    elementa += _genera_numerare(d,
                        (s32)MD_GENUS_ELEMENTUM);
                    citationes += _genera_numerare(d,
                        (s32)MD_GENUS_CITATIO);
                    tabulae += _genera_numerare(d,
                        (s32)MD_GENUS_TABULA);
                    definitiones += _genera_numerare(d,
                        (s32)MD_GENUS_DEFINITIO_NEXUS);
                    divisiones += _genera_numerare(d,
                        (s32)MD_GENUS_DIVISIO);
                    praefationes += _adest(d,
                        (i32)MD_DOCUMENTUM_PRAEFATIO) ? I : ZEPHYRUM;
                    indentata += _indentata_numerare(d, linea,
                        &impressa);
                }
                piscina_destruere(p);
            }
            fclose(lista);

            imprimere("  plagulae %d, octeti %ld, fractae %d, absentes %d\n",
                (integer)plagulae, (longus)octeti, (integer)fractae,
                (integer)absentes);
            imprimere("  capitula %d, saepta %d (indentata %d), html %d,"
                " divisiones %d, praefationes %d\n",
                (integer)capitula, (integer)saepta, (integer)indentata,
                (integer)html, (integer)divisiones,
                (integer)praefationes);
            imprimere("  listae %d, elementa %d, citationes %d, tabulae %d, definitiones %d\n",
                (integer)listae, (integer)elementa, (integer)citationes,
                (integer)tabulae, (integer)definitiones);
            CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (absentes, ZEPHYRUM);
            CREDO_MAIOR_I32 (plagulae, (i32)1000);
            CREDO_MAIOR_I32 (capitula, (i32)500);
            CREDO_MAIOR_I32 (saepta - indentata, (i32)1000);
            CREDO_MAIOR_I32 (praefationes, (i32)300);
            CREDO_MAIOR_I32 (listae, (i32)500);
            CREDO_MAIOR_I32 (elementa, listae);
            CREDO_MAIOR_I32 (citationes, (i32)30);
            CREDO_MAIOR_I32 (tabulae, (i32)200);
        }
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
