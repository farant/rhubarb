/* oratio_arbor.c - Vide oratio_arbor.h. */

#include "oratio_arbor.h"
#include "oratio_lexema.h"
#include "oratio_lexicon.h"
#include "oratio_registrum.h"
#include "materia_token.h"
#include "materia_arbor.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Tabula abbreviationum (v1) - sine puncto, casus indifferens
 * ================================================== */

hic_manens constans character* ABBREVIATIONES[] = {
    "mr", "mrs", "ms", "dr", "prof", "st", "sr", "jr", "vs", "etc",
    "e.g", "i.e", "cf", "viz", "fig", "no", "vol", "pp", "ed", "eds",
    "ch", "sec", "co", "inc", "ltd", "ave", "blvd", "gen", "col",
        "capt",
    "sgt", "lt", "rev", "hon", "msgr", "fr", "bro", "mt", "ft", "al",
    /* latina */
    "ss", "cap", "lib", "op", "cit", "ib", "ibid", "loc", "sc", "sqq",
    "seq", "ep", "serm", "hom", "q", "a", "art", "dist", "sent", "resp",
    "obj", "ad", "n", "nn", "c", "cc", "v", "vv", "l", "ll", "p"
};

interior character
_minuscula (
    character c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (character)(c - 'A' + 'a');
    }
    redde c;
}

b32
oratio_abbreviatio_est (
    constans character* datum,
                   i32  mensura)
{
    i32 i;
    i32 n = (i32)(magnitudo(ABBREVIATIONES)
        / magnitudo(ABBREVIATIONES[0]));

    si (mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* initialis: littera capitalis una */
    si (   mensura         == I && datum[ZEPHYRUM] >= 'A'
        && datum[ZEPHYRUM] <= 'Z')
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans character* a = ABBREVIATIONES[i];
                       i32  k;

        si ((i32)strlen(a) != mensura)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < mensura; k++)
        {
            si (_minuscula(datum[k]) != a[k])
            {
                frange;
            }
        }
        si (k == mensura)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Parsura
 * ================================================== */

nomen structura {
               Piscina* piscina;
    constans character* fons;
                   Xar* lexemata;      /* MateriaToken* */
                   i32  numerus;        /* lexematum, FINIS inclusum */
                   i32  i;              /* cursor */
} Parsura;

interior MateriaToken*
_lex (
    constans Parsura* p,
                 i32  i)
{
    redde *(MateriaToken**)xar_obtinere(p->lexemata, i);
}

interior s32
_genus (
    constans Parsura* p,
                 i32  i)
{
    si (i >= p->numerus)
    {
        redde (s32)ORATIO_LEX_FINIS;
    }
    redde _lex(p, i)->genus;
}

interior b32
_spatii_classis (
    s32 genus)
{
    redde (b32)(genus == (s32)ORATIO_LEX_SPATIUM
        || genus == (s32)ORATIO_LEX_LINEA
        || genus == (s32)ORATIO_LEX_LINEA_CR
        || genus == (s32)ORATIO_LEX_SIGNUM);
}

interior b32
_linea_est (
    s32 genus)
{
    redde (b32)(genus == (s32)ORATIO_LEX_LINEA
        || genus == (s32)ORATIO_LEX_LINEA_CR);
}

interior MateriaNodus*
_nodus (
        Parsura* p,
    OratioGenus  genus)
{
    redde materia_nodus_creare(p->piscina, (s32)genus,
        ORATIO_REGISTRUM.genera[genus].loci_numerus);
}

interior b32
_appendere_lexema (
         Parsura* p,
    MateriaNodus* nodus,
             i32  locus,
             i32  i)
{
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_token(_lex(p, i)), MATERIA_LOCUS_LISTA_TOKEN);
}

interior b32
_appendere_nodum (
         Parsura* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_NODUS);
}

/* lexemata [ab, ad) in locum LISTA_TOKEN */
interior b32
_lexemata_appendere (
         Parsura* p,
    MateriaNodus* nodus,
             i32  locus,
             i32  ab,
             i32  ad)
{
    i32 i;

    per (i = ab; i < ad; i++)
    {
        si (!_appendere_lexema(p, nodus, locus, i))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* finis cursus spatii ab i; *lineae = numerus linearum novarum in eo */
interior i32
_spatium_finis (
    constans Parsura* p,
                 i32  i,
                 i32* lineae)
{
    *lineae = ZEPHYRUM;
    dum (i < p->numerus && _spatii_classis(_genus(p, i)))
    {
        si (_linea_est(_genus(p, i)))
        {
            *lineae = *lineae + I;
        }
        i = i + I;
    }
    redde i;
}

/* ---------- elementa ---------- */

/* vocabulum ab i (LITTERAE); reddit finem partium */
interior i32
_vocabulum_finis (
    constans Parsura* p,
                 i32  i)
{
    i = i + I;   /* LITTERAE prima */
    per (;;)
    {
        s32 g = _genus(p, i);

        si (   g == (s32)ORATIO_LEX_LITTERAE
            || g == (s32)ORATIO_LEX_DIGITI)
        {
            i = i + I;
            perge;
        }
        si (   (g == (s32)ORATIO_LEX_HYPHEN
            || g == (s32)ORATIO_LEX_APOSTROPHUS
             || g == (s32)ORATIO_LEX_PUNCTUM)
            && _genus(p, i + I) == (s32)ORATIO_LEX_LITTERAE)
        {
            i = i + II;
            perge;
        }
        frange;
    }
    redde i;
}

/* numerus ab i (DIGITI); reddit finem */
interior i32
_numerus_finis (
    constans Parsura* p,
                 i32  i)
{
    i = i + I;
    per (;;)
    {
        s32 g = _genus(p, i);

        si (g == (s32)ORATIO_LEX_DIGITI)
        {
            i = i + I;
            perge;
        }
        si (   g                == (s32)ORATIO_LEX_PUNCTUM
            && _genus(p, i + I) == (s32)ORATIO_LEX_DIGITI)
        {
            i = i + II;
            perge;
        }
        si (   g == (s32)ORATIO_LEX_INTERPUNCTIO
            && _lex(p, i)->valor.mensura == I
            && (character)_lex(p, i)->valor.datum[ZEPHYRUM] == ','
            && _genus(p, i + I) == (s32)ORATIO_LEX_DIGITI)
        {
            i = i + II;
            perge;
        }
        si (g == (s32)ORATIO_LEX_LITTERAE)
        {
            i = i + I;   /* suffixum: 3rd, 1980s */
            frange;
        }
        frange;
    }
    redde i;
}

/* elementum unum ab i (non spatium, non FINIS): nodus cum partibus,
 * *ad = index post partes (cauda a vocante) */
interior MateriaNodus*
_elementum (
    Parsura* p,
        i32  i,
        i32* ad)
{
    s32 g = _genus(p, i);
    MateriaNodus* n;

    si (g == (s32)ORATIO_LEX_LITTERAE)
    {
        *ad  = _vocabulum_finis(p, i);
        n    = _nodus(p, ORATIO_GENUS_VOCABULUM);
        si (   n == NIHIL
            || !_lexemata_appendere(p, n, (i32)ORATIO_VOCABULUM_PARTES,
            i, *ad))
        {
            redde NIHIL;
        }
        redde n;
    }
    si (g == (s32)ORATIO_LEX_DIGITI)
    {
        *ad  = _numerus_finis(p, i);
        n    = _nodus(p, ORATIO_GENUS_NUMERUS);
        si (   n == NIHIL
            || !_lexemata_appendere(p, n, (i32)ORATIO_NUMERUS_CRUDUM, i,
            *ad))
        {
            redde NIHIL;
        }
        redde n;
    }
    /* PUNCTUM, INTERPUNCTIO, HYPHEN/APOSTROPHUS solivagi */
    *ad  = i + I;
    n    = _nodus(p, ORATIO_GENUS_INTERPUNCTIO);
    si (   n == NIHIL
        || !materia_nodus_ponere(n, (i32)ORATIO_INTERPUNCTIO_SIGNUM,
               materia_valor_token(_lex(p, i)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

interior i32
_locus_caudae (
    constans MateriaNodus* n)
{
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde (i32)ORATIO_VOCABULUM_CAUDA;
    }
    si (n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        redde (i32)ORATIO_NUMERUS_CAUDA;
    }
    redde (i32)ORATIO_INTERPUNCTIO_CAUDA;
}

/* ---------- sententiae ---------- */

/* lexema primum elementi (signum aut pars prima) */
interior constans MateriaToken*
_lexema_primum (
    constans MateriaNodus* n)
{
    constans MateriaValor* v;

    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    }
    v = &n->loci[n->genus == (s32)ORATIO_GENUS_VOCABULUM
        ? (i32)ORATIO_VOCABULUM_PARTES : (i32)ORATIO_NUMERUS_CRUDUM];
    si (   v->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*v) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*v, ZEPHYRUM)->datum.token;
}

interior b32
_signum_est (
    constans MateriaNodus* n,
       constans character* signa)
{
    constans MateriaToken* t;

    si (n->genus != (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    si (t->valor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde (b32)(strchr(signa, (character)t->valor.datum[ZEPHYRUM])
        != NIHIL
        && (character)t->valor.datum[ZEPHYRUM] != '\0');
}

/* interpunctio unicode claudens/aperiens: U+2019 U+201D (claudens),
 * U+2018 U+201C (aperiens) */
interior b32
_curva_est (
    constans MateriaNodus* n,
                      b32  claudens)
{
    constans MateriaToken* t;
     insignatus character  c3;

    si (n->genus != (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    si (   t->valor.mensura                               != III
        || (insignatus character)t->valor.datum[ZEPHYRUM] != 0xE2
        || (insignatus character)t->valor.datum[I]        != 0x80)
    {
        redde FALSUM;
    }
    c3 = (insignatus character)t->valor.datum[II];
    redde claudens ? (b32)(c3 == 0x99 || c3 == 0x9D)
                   : (b32)(c3 == 0x98 || c3 == 0x9C);
}

interior b32
_claudens_est (
    constans MateriaNodus* n)
{
    redde (b32)(_signum_est(n, "\"')]}") || _curva_est(n, VERUM));
}

interior b32
_aperiens_est (
    constans MateriaNodus* n)
{
    redde (b32)(_signum_est(n, "\"'([{") || _curva_est(n, FALSUM));
}

/* finis candidatus: PUNCTUM, '!'/'?' (cursus), cursus punctorum */
interior b32
_candidatus_est (
    constans MateriaNodus* n)
{
    constans MateriaToken* t;
                character  c;

    si (n->genus != (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    si (t->genus == (s32)ORATIO_LEX_PUNCTUM)
    {
        redde VERUM;
    }
    si (t->valor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    c = (character)t->valor.datum[ZEPHYRUM];
    redde (b32)(c == '!' || c == '?' || c == '.');
}

/* an elementum sententiam INCIPERE possit: capitalis ASCII, octetus
 * >= 0x80, numerus */
interior b32
_initium_est (
    constans MateriaNodus* n)
{
    constans MateriaToken* t;
     insignatus character  c;

    si (n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        redde VERUM;
    }
    si (n->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde FALSUM;
    }
    t = _lexema_primum(n);
    si (t == NIHIL || t->valor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    c = (insignatus character)t->valor.datum[ZEPHYRUM];
    redde (b32)((c >= 'A' && c <= 'Z') || c >= 0x80);
}

/* vocabulum: partes in buffer (ASCII solum ad tabulam); FALSUM si
 * punctum intra (e.g) aut longius quam buffer */
interior b32
_vocabulum_abbreviatio (
    constans MateriaNodus* n)
{
    constans MateriaValor* v;
                character  buffer[XVI];
                      i32  m;
                      i32  k;
                      i32  l = ZEPHYRUM;

    si (n->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde FALSUM;
    }
    v = &n->loci[ORATIO_VOCABULUM_PARTES];
    m = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaToken* t = materia_valor_lista_obtinere(*v,
            k)->datum.token;
                          i32 j;

        si (l + t->valor.mensura >= (i32)magnitudo(buffer))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < t->valor.mensura; j++)
        {
            buffer[l++] = (character)t->valor.datum[j];
        }
    }
    redde oratio_abbreviatio_est(buffer, l);
}

/* elementa paragraphi (Xar de MateriaNodus*) in sententias
 * distribuere et paragrapho appendere */
interior b32
_sententias_distribuere (
         Parsura* p,
    MateriaNodus* paragraphus,
             Xar* elementa)
{
             i32  n          = xar_numerus(elementa);
             i32  i          = ZEPHYRUM;
    MateriaNodus* sententia  = NIHIL;

    dum (i < n)
    {
        MateriaNodus* e = *(MateriaNodus**)xar_obtinere(elementa, i);
                 b32  finis = FALSUM;

        si (sententia == NIHIL)
        {
            sententia = _nodus(p, ORATIO_GENUS_SENTENTIA);
            si (sententia == NIHIL)
            {
                redde FALSUM;
            }
        }
        si (!_appendere_nodum(p, sententia,
            (i32)ORATIO_SENTENTIA_ELEMENTA, e))
        {
            redde FALSUM;
        }
        i = i + I;
        si (_candidatus_est(e))
        {
            /* abbreviatio ante punctum? (punctum solum, non '!' '?') */
            b32 abbreviatio = FALSUM;

            si (e->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token->genus
                    == (s32)ORATIO_LEX_PUNCTUM
                && i >= II)
            {
                MateriaNodus* ante =
                    *(MateriaNodus**)xar_obtinere(elementa,
                    i - II);

                abbreviatio = _vocabulum_abbreviatio(ante);
            }
            /* claudentia adhaerent */
            dum (   i < n
                 && _claudens_est(*(MateriaNodus**)xar_obtinere(elementa,
                i)))
            {
                si (!_appendere_nodum(p, sententia,
                    (i32)ORATIO_SENTENTIA_ELEMENTA,
                        *(MateriaNodus**)xar_obtinere(elementa, i)))
                {
                    redde FALSUM;
                }
                i = i + I;
            }
            si (i >= n)
            {
                finis = VERUM;   /* paragraphus finit */
            }
            alioquin si (!abbreviatio)
            {
                i32 j = i;

                dum (   j < n
                     && _aperiens_est(*(MateriaNodus**)xar_obtinere(elementa,
                    j)))
                {
                    j = j + I;
                }
                si (   j < n
                    && _initium_est(*(MateriaNodus**)xar_obtinere(elementa,
                    j)))
                {
                    finis = VERUM;
                }
            }
        }
        si (finis || i >= n)
        {
            si (!_appendere_nodum(p, paragraphus,
                (i32)ORATIO_PARAGRAPHUS_SENTENTIAE,
                    sententia))
            {
                redde FALSUM;
            }
            sententia = NIHIL;
        }
    }
    redde VERUM;
}

/* ---------- documentum ---------- */

MateriaNodus*
oratio_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Parsura p;
    MateriaNodus* documentum;
    i32 lineae;
    i32 i;

    p.piscina   = piscina;
    p.fons      = fons;
    p.lexemata  = oratio_lexare(piscina, fons, mensura);
    si (p.lexemata == NIHIL)
    {
        redde NIHIL;
    }
    p.numerus   = xar_numerus(p.lexemata);
    documentum  = _nodus(&p, ORATIO_GENUS_DOCUMENTUM);
    si (documentum == NIHIL)
    {
        redde NIHIL;
    }
    /* praefixa documenti: spatium totum ante elementum primum */
    i = _spatium_finis(&p, ZEPHYRUM, &lineae);
    si (!_lexemata_appendere(&p, documentum,
        (i32)ORATIO_DOCUMENTUM_PRAEFIXA,
            ZEPHYRUM, i))
    {
        redde NIHIL;
    }
    dum (_genus(&p, i) != (s32)ORATIO_LEX_FINIS)
    {
        MateriaNodus* paragraphus = _nodus(&p,
            ORATIO_GENUS_PARAGRAPHUS);
        Xar* elementa = xar_creare(piscina,
            (i32)magnitudo(MateriaNodus*));
        i32 praefixa_ab = i;

        si (paragraphus == NIHIL || elementa == NIHIL)
        {
            redde NIHIL;
        }
        /* praefixa paragraphi: indentatio (spatia sine lineis) - iam
         * a cauda praecedente separata */
        i = _spatium_finis(&p, i, &lineae);
        si (!_lexemata_appendere(&p, paragraphus,
            (i32)ORATIO_PARAGRAPHUS_PRAEFIXA,
                praefixa_ab, i))
        {
            redde NIHIL;
        }
        /* elementa cum caudis usque ad finem paragraphi */
        dum (_genus(&p, i) != (s32)ORATIO_LEX_FINIS)
        {
                     i32  ad;
                     i32  spatii_finis;
            MateriaNodus* e = _elementum(&p, i, &ad);
            MateriaNodus** locus;

            si (e == NIHIL)
            {
                redde NIHIL;
            }
            locus = (MateriaNodus**)xar_addere(elementa);
            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus        = e;
            spatii_finis  = _spatium_finis(&p, ad, &lineae);
            si (   lineae                   >= II
                || _genus(&p, spatii_finis) == (s32)ORATIO_LEX_FINIS)
            {
                /* finis paragraphi: cauda elementi usque ad lineam primam
                 * inclusive; cauda paragraphi usque ad lineam ultimam
                 * inclusive (finis documenti: totum); praefixa sequentis
                 * = residuum */
                i32 k       = ad;
                i32 ultima  = ad;

                dum (k < spatii_finis && !_linea_est(_genus(&p, k)))
                {
                    k = k + I;
                }
                si (k < spatii_finis)
                {
                    k = k + I;   /* linea prima inclusa */
                }
                si (!_lexemata_appendere(&p, e, _locus_caudae(e), ad,
                    k))
                {
                    redde NIHIL;
                }
                si (_genus(&p, spatii_finis) == (s32)ORATIO_LEX_FINIS)
                {
                    ultima = spatii_finis;
                }
                alioquin
                {
                    i32 m;

                    ultima = k;
                    per (m = k; m < spatii_finis; m++)
                    {
                        si (_linea_est(_genus(&p, m)))
                        {
                            ultima = m + I;
                        }
                    }
                }
                si (!_lexemata_appendere(&p, paragraphus,
                        (i32)ORATIO_PARAGRAPHUS_CAUDA, k, ultima))
                {
                    redde NIHIL;
                }
                i = ultima;
                frange;
            }
            /* intra paragraphum: spatium totum caudae elementi */
            si (!_lexemata_appendere(&p, e, _locus_caudae(e), ad,
                spatii_finis))
            {
                redde NIHIL;
            }
            i = spatii_finis;
        }
        /* forma (T6b): prosa donec classificator adveniat */
        si (!materia_nodus_ponere(paragraphus,
                (i32)ORATIO_PARAGRAPHUS_FORMA,
                materia_valor_index((s32)ORATIO_FORMA_PROSA),
                MATERIA_LOCUS_INDEX))
        {
            redde NIHIL;
        }
        si (   !_sententias_distribuere(&p, paragraphus, elementa)
            || !_appendere_nodum(&p, documentum,
            (i32)ORATIO_DOCUMENTUM_PARAGRAPHI,
                   paragraphus))
        {
            redde NIHIL;
        }
    }
    /* FINIS */
    si (!materia_nodus_ponere(documentum, (i32)ORATIO_DOCUMENTUM_FINIS,
            materia_valor_token(_lex(&p, p.numerus - I)),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    /* patres post acceptum (comparator structuralis eos confert) */
    materia_arbor_patres_figere(piscina, documentum);
    redde documentum;
}
