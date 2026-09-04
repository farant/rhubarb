/* oratio_forma.c - Vide oratio_forma.h. */

#include "oratio_forma.h"
#include "oratio_lexicon.h"
#include "materia_token.h"
#include <string.h>


/* ==================================================
 * Tabula regularum - DATA (ordo = praecedentia; prima tenens vincit)
 * ================================================== */

constans OratioRegulaFormae ORATIO_REGULAE_FORMAE[] = {
    /* linea una tota capitalis (SANCTI HILARII, LIBER PRIMVS) */
    { ORATIO_FORMA_TITULUS, "titulus capitalis",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_AEQUALIS,
          1 },
        { ORATIO_INDICIUM_ELEMENTA,            ORATIO_CONDICIO_MAXIMUM,
            16 },
        { ORATIO_INDICIUM_CAPITALES_OMNES_PCT, ORATIO_CONDICIO_MINIMUM,
            100 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
    /* linea una brevis sine interpunctione terminali (Contents:, Chapter I) */
    { ORATIO_FORMA_TITULUS, "titulus brevis",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_AEQUALIS,
          1 },
        { ORATIO_INDICIUM_ELEMENTA,            ORATIO_CONDICIO_MAXIMUM,
            12 },
        { ORATIO_INDICIUM_TERMINALES_PCT,      ORATIO_CONDICIO_MAXIMUM,
            0 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
    /* lineae plures cum hiatibus internis (columnae) */
    { ORATIO_FORMA_TABULA, "tabula columnata",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          2 },
        { ORATIO_INDICIUM_COLUMNATAE_PCT,      ORATIO_CONDICIO_MINIMUM,
            50 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
    /* lineae plures notis incipientes (1. / - / I.) aut numero paginae finitae */
    { ORATIO_FORMA_INDEX, "index notatus",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          2 },
        { ORATIO_INDICIUM_NOTATAE_PCT,         ORATIO_CONDICIO_MINIMUM,
            67 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
        /* index sine numeris (Contents): lineae breves, capitales, terminatae */
    { ORATIO_FORMA_INDEX, "index brevis",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          3 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            64 },
        { ORATIO_INDICIUM_TERMINALES_PCT,      ORATIO_CONDICIO_MINIMUM,
            90 },
        { ORATIO_INDICIUM_CAPITALES_PCT,       ORATIO_CONDICIO_MINIMUM,
            90 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
    /* fracturae voluntariae plerumque: involucrum avidum eas non parit.
     * LATITUDO <= LXIV: prosa Gutenberg ad LXX-LXXV involvitur margine
     * INAEQUALI (typista, non machina - Lincoln 70-100% 'voluntariae'),
     * versus eam latitudinem numquam attingit (hexameter <= ~LVI) */
        /* ... et fines linearum interpuncti (Latini: versus minusculi) -
     * prosa in columna angusta typista involuta (Lincoln 'Resolved:'
     * XLVIII lata, XXXVI% interpuncta) neutram tenet */
    { ORATIO_FORMA_VERSUS, "versus voluntarius interpunctus",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          3 },
        { ORATIO_INDICIUM_VOLUNTARIAE_PCT,     ORATIO_CONDICIO_MINIMUM,
            67 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            64 },
        { ORATIO_INDICIUM_INTERPUNCTAE_PCT,    ORATIO_CONDICIO_MINIMUM,
            40 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
    /* ... aut initia linearum capitalia (Anglici: versus capitales) */
    { ORATIO_FORMA_VERSUS, "versus voluntarius capitalis",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          3 },
        { ORATIO_INDICIUM_VOLUNTARIAE_PCT,     ORATIO_CONDICIO_MINIMUM,
            67 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            64 },
        { ORATIO_INDICIUM_CAPITALES_PCT,       ORATIO_CONDICIO_MINIMUM,
            60 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },
        /* lineae breves plerumque interpunctione finitae (stropha aequalis:
     * distichi elegiaci XLIV-XLVIII lati nihil 'cepisset'). Limen LXX
     * inter casus mensuratos: Propertius I.5 (LXII lineae, LXXII%)
     * supra, citatio Constitutionis in Lincoln (VI lineae angustae,
     * LXVI%) infra */
    { ORATIO_FORMA_VERSUS, "versus interpunctus",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_MINIMUM,
          4 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            60 },
        { ORATIO_INDICIUM_INTERPUNCTAE_PCT,    ORATIO_CONDICIO_MINIMUM,
            70 },

        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },

    /* distichon: fractura voluntaria et linea interpuncta */
    { ORATIO_FORMA_VERSUS, "distichon voluntarium",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_AEQUALIS,
          2 },
        { ORATIO_INDICIUM_VOLUNTARIAE_PCT,     ORATIO_CONDICIO_MINIMUM,
            100 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            60 },
        { ORATIO_INDICIUM_INTERPUNCTAE_PCT,    ORATIO_CONDICIO_MINIMUM,
            50 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } },

    /* distichon breve: ambae lineae interpunctae */
    { ORATIO_FORMA_VERSUS, "distichon interpunctum",
      { { ORATIO_INDICIUM_LINEAE,              ORATIO_CONDICIO_AEQUALIS,
          2 },
        { ORATIO_INDICIUM_LATITUDO,            ORATIO_CONDICIO_MAXIMUM,
            60 },
        { ORATIO_INDICIUM_INTERPUNCTAE_PCT,    ORATIO_CONDICIO_MINIMUM,
            100 },
        { ORATIO_INDICIUM_NIHIL,               ORATIO_CONDICIO_MINIMUM,
            0 } } }
};

constans i32 ORATIO_REGULAE_FORMAE_NUMERUS =
    (i32)(magnitudo(ORATIO_REGULAE_FORMAE)
    / magnitudo(ORATIO_REGULAE_FORMAE[0]));

hic_manens constans character* TITULI_FORMARUM[] = {
    "prosa", "versus", "titulus", "tabula", "index"
};

hic_manens constans character* TITULI_INDICIORUM[] = {
    "nihil", "lineae", "elementa", "latitudo", "longitudo-media",
    "indentatio-diversa", "terminales-internae", "voluntariae-pct",
    "terminales-pct", "interpunctae-pct", "capitales-pct",
    "capitales-omnes-pct", "notatae-pct", "columnatae-pct"
};

/* elementum cum lexemate hoc longo aut longiore latitudinem non fingit */
#define ORATIO_LEXEMA_INFRANGIBILE 24


/* ==================================================
 * Elementa: lexemata, caudae, classes
 * ================================================== */

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

interior constans MateriaValor*
_partes (
    constans MateriaNodus* n)
{
    constans MateriaValor* v;

    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde NIHIL;
    }
    v = &n->loci[n->genus == (s32)ORATIO_GENUS_VOCABULUM
        ? (i32)ORATIO_VOCABULUM_PARTES : (i32)ORATIO_NUMERUS_CRUDUM];
    si (   v->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*v) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde v;
}

interior constans MateriaToken*
_lexema_primum (
    constans MateriaNodus* n)
{
    constans MateriaValor* v;

    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    }
    v = _partes(n);
    redde v == NIHIL ? NIHIL
        : materia_valor_lista_obtinere(*v, ZEPHYRUM)->datum.token;
}

interior constans MateriaToken*
_lexema_ultimum (
    constans MateriaNodus* n)
{
    constans MateriaValor* v;

    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    }
    v = _partes(n);
    redde v == NIHIL ? NIHIL
        : materia_valor_lista_obtinere(*v,
            materia_valor_lista_numerus(*v) - I)->datum.token;
}

interior s32
_initium (
    constans MateriaNodus* n)
{
    constans MateriaToken* t = _lexema_primum(n);

    redde t == NIHIL ? ZEPHYRUM : t->byte_offset;
}

interior s32
_finis (
    constans MateriaNodus* n)
{
    constans MateriaToken* t = _lexema_ultimum(n);

    redde t == NIHIL ? ZEPHYRUM : t->byte_offset
        + (s32)t->valor.mensura;
}

interior s32
_mensura_partium (
    constans MateriaNodus* n)
{
    redde _finis(n) - _initium(n);
}

interior insignatus character
_octetus_primus (
    constans MateriaNodus* n)
{
    constans MateriaToken* t = _lexema_primum(n);

    si (t == NIHIL || t->valor.mensura == ZEPHYRUM)
    {
        redde (insignatus character)ZEPHYRUM;
    }
    redde (insignatus character)t->valor.datum[ZEPHYRUM];
}

interior b32
_interpunctio_est (
    constans MateriaNodus* n)
{
    redde (b32)(n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO);
}

/* candidatus finis sententiae ut in oratio_arbor: PUNCTUM, '!' '?',
 * cursus punctorum */
interior b32
_terminalis_est (
    constans MateriaNodus* n)
{
    constans MateriaToken* t;
     insignatus character  c;

    si (!_interpunctio_est(n))
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    si (t->genus == (s32)ORATIO_LEX_PUNCTUM)
    {
        redde VERUM;
    }
    c = _octetus_primus(n);
    redde (b32)(c == '!' || c == '?' || c == '.');
}

/* interpunctio claudens: citationes, parentheses, uncus */
interior b32
_claudens_est (
    constans MateriaNodus* n)
{
    constans MateriaToken* t;
     insignatus character  c;

    si (!_interpunctio_est(n))
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    c = _octetus_primus(n);
    si (c == '"' || c == '\'' || c == ')' || c == ']' || c == '}')
    {
        redde VERUM;
    }
    /* U+2019 U+201D (E2 80 99 / E2 80 9D), U+00BB (C2 BB) */
    si (   t->valor.mensura >= (i32)III && c == 0xE2
        && (insignatus character)t->valor.datum[I] == 0x80)
    {
        insignatus character d =
            (insignatus character)t->valor.datum[II];

        redde (b32)(d == 0x99 || d == 0x9D);
    }
    redde (b32)(t->valor.mensura >= (i32)II && c == 0xC2
        && (insignatus character)t->valor.datum[I] == 0xBB);
}

interior b32
_capitalis_est (
    constans MateriaNodus* n)
{
    insignatus character c;

    si (n->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde FALSUM;
    }
    c = _octetus_primus(n);
    redde (b32)(c >= 'A' && c <= 'Z');
}

/* litterae ASCII vocabuli: numerus in *litterae; FALSUM si minuscula
 * ulla */
interior b32
_litterae_capitales (
    constans MateriaNodus* n,
                      i32* litterae)
{
    constans MateriaValor* v = _partes(n);
                      i32  m;
                      i32  k;

    si (n->genus != (s32)ORATIO_GENUS_VOCABULUM || v == NIHIL)
    {
        redde VERUM;
    }
    m = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaToken* t = materia_valor_lista_obtinere(*v,
            k)->datum.token;
                          i32 j;

        per (j = ZEPHYRUM; j < t->valor.mensura; j++)
        {
            character c = (character)t->valor.datum[j];

            si (c >= 'a' && c <= 'z')
            {
                redde FALSUM;
            }
            si (c >= 'A' && c <= 'Z')
            {
                *litterae = *litterae + I;
            }
        }
    }
    redde VERUM;
}

/* signum listae: - * + aut U+2022 U+2013 U+2014 (E2 80 A2/93/94),
 * SPATIO sequente (cauda) - '**fortis**' markdown cursus est sine
 * spatio, non signum (corpus md: XL lineae 'Who:' index fingebant) */
interior b32
_signum_listae_est (
    constans MateriaNodus* n)
{
    constans MateriaToken* t;
     insignatus character  c;
    constans MateriaValor* cauda;

    si (!_interpunctio_est(n))
    {
        redde FALSUM;
    }
    cauda = &n->loci[ORATIO_INTERPUNCTIO_CAUDA];
    si (   cauda->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*cauda) == ZEPHYRUM
        || materia_valor_lista_obtinere(*cauda, ZEPHYRUM)->datum.token
            ->genus != (s32)ORATIO_LEX_SPATIUM)
    {
        redde FALSUM;
    }
    t = n->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    c = _octetus_primus(n);
    si (t->valor.mensura == I && (c == '-' || c == '*' || c == '+'))
    {
        redde VERUM;
    }

    si (   t->valor.mensura >= (i32)III && c == 0xE2
        && (insignatus character)t->valor.datum[I] == 0x80)
    {
        insignatus character d =
            (insignatus character)t->valor.datum[II];

        redde (b32)(d == 0xA2 || d == 0x93 || d == 0x94);
    }
    redde FALSUM;
}

/* vocabulum ordinis: littera una, aut numerale Romanum capitale <= V
 * litterarum */
interior b32
_vocabulum_ordinis_est (
    constans MateriaNodus* n)
{
    constans MateriaValor* v = _partes(n);
    constans MateriaToken* t;
                      i32  j;

    si (   n->genus != (s32)ORATIO_GENUS_VOCABULUM || v == NIHIL
        || materia_valor_lista_numerus(*v) != I)
    {
        redde FALSUM;
    }
    t = materia_valor_lista_obtinere(*v, ZEPHYRUM)->datum.token;
    si (t->valor.mensura == I)
    {
        character c = (character)t->valor.datum[ZEPHYRUM];

        redde (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
    }
    si (t->valor.mensura > (i32)V)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < t->valor.mensura; j++)
    {
        character c = (character)t->valor.datum[j];

        si (   c != 'I' && c != 'V' && c != 'X' && c != 'L' && c != 'C'
            && c != 'D' && c != 'M')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* hiatus in cauda: SPATIUM >= II octetorum aut tabula */
interior b32
_hiatus_est (
    constans MateriaNodus* n,
                      b32* tabula)
{
    constans MateriaValor* v = &n->loci[_locus_caudae(n)];
                      i32  m;
                      i32  k;
                      b32  hiatus = FALSUM;

    *tabula = FALSUM;
    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    m = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaToken* t = materia_valor_lista_obtinere(*v,
            k)->datum.token;
                          i32 j;

        si (t->genus != (s32)ORATIO_LEX_SPATIUM)
        {
            perge;
        }
        si (t->valor.mensura >= (i32)II)
        {
            hiatus = VERUM;
        }
        per (j = ZEPHYRUM; j < t->valor.mensura; j++)
        {
            si (t->valor.datum[j] == '\t')
            {
                *tabula = VERUM;
            }
        }
    }
        redde (b32)(hiatus || *tabula);
}

interior constans MateriaToken*
_linea_in_cauda (
    constans MateriaNodus* n)
{
    constans MateriaValor* v = &n->loci[_locus_caudae(n)];
                      i32  m;
                      i32  k;

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    m = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaToken* t = materia_valor_lista_obtinere(*v,
            k)->datum.token;

        si (   t->genus == (s32)ORATIO_LEX_LINEA
            || t->genus == (s32)ORATIO_LEX_LINEA_CR)
        {
            redde t;
        }
    }
    redde NIHIL;
}

b32
oratio_forma_linea_finit (
    constans MateriaNodus* elementum)
{
    redde (b32)(_linea_in_cauda(elementum) != NIHIL);
}


/* ==================================================
 * Lineae
 * ================================================== */

nomen structura {
    i32 primum;        /* index elementi primi */
    i32 ultimum;       /* index elementi ultimi */
    s32 initium;       /* offset columnae 0 */
    s32 longitudo;     /* ad finem partium elementi ultimi */
    s32 indentatio;
    s32 lexema_maximum;
    b32 sequens;       /* linea successorem habet */
} Linea;

interior constans MateriaNodus*
_e (
    Xar* elementa,
    i32  k)
{
    redde *(constans MateriaNodus* constans*)xar_obtinere(elementa, k);
}

/* lineam proximam ab elemento *k legere; FALSUM si elementa exhausta */
interior b32
_linea_proxima (
             Xar* elementa,

             i32  n,
             i32* k,
             s32* initium_lineae,
           Linea* l)
{
    i32 j;

    si (*k >= n)
    {
        redde FALSUM;
    }
    l->primum          = *k;
    l->initium         = *initium_lineae;
    l->indentatio      = _initium(_e(elementa, *k)) - *initium_lineae;
    l->lexema_maximum  = ZEPHYRUM;
    l->sequens         = FALSUM;
    per (j = *k; j < n; j++)
    {
        constans MateriaNodus* e     = _e(elementa, j);
        constans MateriaToken* linea;
                          s32  m     = _mensura_partium(e);

        si (m > l->lexema_maximum)
        {
            l->lexema_maximum = m;
        }
        l->ultimum    = j;
        l->longitudo  = _finis(e) - l->initium;
        linea         = _linea_in_cauda(e);
        si (linea != NIHIL)
        {
            *initium_lineae = linea->byte_offset
                + (s32)linea->valor.mensura;
            l->sequens  = (b32)(j + I < n);
            *k          = j + I;
            redde VERUM;
        }
    }
    *k = n;
    redde VERUM;
}

interior i32
_pct (
    i32 pars,
    i32 totum)
{
    redde totum == ZEPHYRUM ? ZEPHYRUM : (i32)(pars * (i32)100 / totum);
}

vacuum
oratio_forma_indicia (
              Xar* elementa,
              s32  initium_lineae,
    OratioIndicia* indicia)
{

      i32 n;
      i32 k;
      s32 cursor;
      s64 summa     = ZEPHYRUM;
      s32 latitudo  = ZEPHYRUM;
    Linea l;

    memset(indicia, ZEPHYRUM, magnitudo(*indicia));
    si (elementa == NIHIL)
    {
        redde;
    }
    n                  = xar_numerus(elementa);
    indicia->elementa  = n;
    si (n == ZEPHYRUM)
    {
        redde;
    }

    /* cursus I: longitudines, latitudo, classes linearum */
    k       = ZEPHYRUM;
    cursor  = initium_lineae;
    dum (_linea_proxima(elementa, n, &k, &cursor, &l))
    {
                          i32  ult      = l.ultimum;
                          i32  j;
                          i32  litterae = ZEPHYRUM;
                          b32  capitales_omnes = VERUM;
                          b32  columnata = FALSUM;
                          i32  virgulae = ZEPHYRUM;
        constans MateriaNodus* primum = _e(elementa, l.primum);
        constans MateriaNodus* finis;

        indicia->lineae  = indicia->lineae + I;
        summa            = summa + (s64)l.longitudo;
        si (   indicia->lineae == I
            || l.longitudo > (s32)indicia->longitudo_maxima)
        {
            indicia->longitudo_maxima = (i32)l.longitudo;
        }
        si (   indicia->lineae == I
            || l.longitudo < (s32)indicia->longitudo_minima)
        {
            indicia->longitudo_minima = (i32)l.longitudo;
        }
        si (   indicia->lineae == I
            || l.indentatio > (s32)indicia->indentatio_maxima)
        {
            indicia->indentatio_maxima = (i32)l.indentatio;
        }
        si (   indicia->lineae == I
            || l.indentatio < (s32)indicia->indentatio_minima)
        {
            indicia->indentatio_minima = (i32)l.indentatio;
        }
        si (   l.lexema_maximum < (s32)ORATIO_LEXEMA_INFRANGIBILE
            && l.longitudo > latitudo)
        {
            latitudo = l.longitudo;
        }
        /* finis lineae: claudentia praeterita */
        dum (ult > l.primum && _claudens_est(_e(elementa, ult)))
        {
            ult = ult - I;
        }
        finis = _e(elementa, ult);
        si (_terminalis_est(finis))
        {
            indicia->terminales = indicia->terminales + I;
        }
        si (_interpunctio_est(finis))
        {
            indicia->interpunctae = indicia->interpunctae + I;
        }
        per (j = l.primum; j < ult; j++)
        {
            si (_terminalis_est(_e(elementa, j)))
            {
                indicia->terminales_internae =
                    indicia->terminales_internae + I;
            }
        }
        /* initium lineae */
        si (_capitalis_est(primum))
        {
            indicia->capitales = indicia->capitales + I;
        }
        per (j = l.primum; j <= l.ultimum; j++)
        {
            constans MateriaNodus* e = _e(elementa, j);
                              b32  tabula;

            si (!_litterae_capitales(e, &litterae))
            {
                capitales_omnes = FALSUM;
            }
            si (_interpunctio_est(e) && _octetus_primus(e) == '|')
            {
                virgulae = virgulae + I;
            }
            si (   j < l.ultimum && _hiatus_est(e, &tabula)
                && (tabula || !_interpunctio_est(e)))
            {
                columnata = VERUM;
            }
        }
        si (capitales_omnes && litterae > ZEPHYRUM)
        {
            indicia->capitales_omnes = indicia->capitales_omnes + I;
        }
        si (columnata || virgulae >= (i32)II)
        {
            indicia->columnatae = indicia->columnatae + I;
        }
        /* notae: numerus | signum listae | littera+punctum ; numerus paginae */
        {
            b32 notata = FALSUM;

            si (   primum->genus == (s32)ORATIO_GENUS_NUMERUS
                || _signum_listae_est(primum))
            {
                notata = VERUM;
            }
            alioquin si (   _vocabulum_ordinis_est(primum)
                         && l.primum + I <= l.ultimum)
            {
                insignatus character c = _octetus_primus(_e(elementa,
                    l.primum + I));

                notata = (b32)(_interpunctio_est(_e(elementa, l.primum
                    + I))
                    && (c == '.' || c == ')'));
            }
            si (   !notata && l.ultimum > l.primum
                && _e(elementa, l.ultimum)->genus
                    == (s32)ORATIO_GENUS_NUMERUS)
            {
                constans MateriaNodus* ante = _e(elementa, l.ultimum
                    - I);
                                  b32 tabula;

                si (   (_interpunctio_est(ante)
                    && _octetus_primus(ante) == '.'
                        && _mensura_partium(ante) >= (s32)II)
                    || (_hiatus_est(ante, &tabula)))
                {
                    notata = VERUM;
                }
            }
            si (notata)
            {
                indicia->notatae = indicia->notatae + I;
            }
        }
    }
    indicia->latitudo = latitudo > ZEPHYRUM ? (i32)latitudo
        : indicia->longitudo_maxima;
    indicia->longitudo_media  = (i32)(summa / (s64)indicia->lineae);
    indicia->fracturae        = indicia->lineae - I;

    /* cursus II: fracturae voluntariae contra latitudinem */
    k       = ZEPHYRUM;
    cursor  = initium_lineae;
    dum (_linea_proxima(elementa, n, &k, &cursor, &l))
    {
        si (l.sequens)
        {
            s32 verbum = _mensura_partium(_e(elementa, l.ultimum + I));

            si (l.longitudo + I + verbum <= (s32)indicia->latitudo)
            {
                indicia->voluntariae = indicia->voluntariae + I;
            }
        }
    }

    indicia->voluntariae_pct     = _pct(indicia->voluntariae,
        indicia->fracturae);
    indicia->terminales_pct      = _pct(indicia->terminales,
        indicia->lineae);
    indicia->interpunctae_pct    = _pct(indicia->interpunctae,
        indicia->lineae);
    indicia->capitales_pct       = _pct(indicia->capitales,
        indicia->lineae);
    indicia->capitales_omnes_pct = _pct(indicia->capitales_omnes,
        indicia->lineae);
    indicia->notatae_pct         = _pct(indicia->notatae,
        indicia->lineae);
    indicia->columnatae_pct      = _pct(indicia->columnatae,
        indicia->lineae);
}


/* ==================================================
 * Iudicium
 * ================================================== */

i32
oratio_forma_indicium (
    constans OratioIndicia* indicia,
            OratioIndicium  quod)
{
    commutatio (quod)
    {
        casus ORATIO_INDICIUM_LINEAE:              redde indicia->lineae;
        casus ORATIO_INDICIUM_ELEMENTA:            redde indicia->elementa;
        casus ORATIO_INDICIUM_LATITUDO:            redde indicia->latitudo;
        casus ORATIO_INDICIUM_LONGITUDO_MEDIA:     redde indicia->longitudo_media;
        casus ORATIO_INDICIUM_INDENTATIO_DIVERSA:
            redde indicia->indentatio_maxima
                - indicia->indentatio_minima;
        casus ORATIO_INDICIUM_TERMINALES_INTERNAE: redde indicia->terminales_internae;
        casus ORATIO_INDICIUM_VOLUNTARIAE_PCT:     redde indicia->voluntariae_pct;
        casus ORATIO_INDICIUM_TERMINALES_PCT:      redde indicia->terminales_pct;
        casus ORATIO_INDICIUM_INTERPUNCTAE_PCT:    redde indicia->interpunctae_pct;
        casus ORATIO_INDICIUM_CAPITALES_PCT:       redde indicia->capitales_pct;
        casus ORATIO_INDICIUM_CAPITALES_OMNES_PCT: redde indicia->capitales_omnes_pct;
        casus ORATIO_INDICIUM_NOTATAE_PCT:         redde indicia->notatae_pct;
        casus ORATIO_INDICIUM_COLUMNATAE_PCT:      redde indicia->columnatae_pct;
        ordinarius:                                redde ZEPHYRUM;
    }
}

interior b32
_condicio_tenet (
     constans OratioIndicia* indicia,
    constans OratioCondicio* c)
{
    i32 v = oratio_forma_indicium(indicia, c->indicium);

    commutatio (c->genus)
    {
        casus ORATIO_CONDICIO_MINIMUM:  redde (b32)(v >= c->valor);
        casus ORATIO_CONDICIO_MAXIMUM:  redde (b32)(v <= c->valor);
        casus ORATIO_CONDICIO_AEQUALIS: redde (b32)(v == c->valor);
        ordinarius:                     redde FALSUM;
    }
}

OratioForma
oratio_forma_iudicare (
         constans OratioIndicia*  indicia,
    constans OratioRegulaFormae** regula)
{
    i32 r;

    si (regula != NIHIL)
    {
        *regula = NIHIL;
    }
    per (r = ZEPHYRUM; r < ORATIO_REGULAE_FORMAE_NUMERUS; r++)
    {
        constans OratioRegulaFormae* reg = &ORATIO_REGULAE_FORMAE[r];
                                i32  c;
                                b32  tenet = VERUM;

        per (c = ZEPHYRUM; c < (i32)ORATIO_CONDICIONES_MAXIMAE; c++)
        {
            si (reg->condiciones[c].indicium == ORATIO_INDICIUM_NIHIL)
            {
                frange;
            }
            si (!_condicio_tenet(indicia, &reg->condiciones[c]))
            {
                tenet = FALSUM;
                frange;
            }
        }
        si (tenet)
        {
            si (regula != NIHIL)
            {
                *regula = reg;
            }
            redde reg->forma;
        }
    }
    redde ORATIO_FORMA_PROSA;
}

constans character*
oratio_forma_titulus (
    OratioForma forma)
{
    si ((i32)forma >= (i32)ORATIO_FORMA_NUMERUS_FORMARUM)
    {
        redde "?";
    }
    redde TITULI_FORMARUM[forma];
}

constans character*
oratio_forma_indicii_titulus (
    OratioIndicium quod)
{
    si ((i32)quod >= (i32)ORATIO_INDICIUM_NUMERUS_INDICIORUM)
    {
        redde "?";
    }
    redde TITULI_INDICIORUM[quod];
}


/* ==================================================
 * Ex arbore constructa
 * ================================================== */

OratioForma
oratio_forma_paragraphi (
    constans MateriaNodus* paragraphus)
{
    constans MateriaValor* v;

    si (   paragraphus == NIHIL
        || paragraphus->numerus_locorum
            <= (i32)ORATIO_PARAGRAPHUS_FORMA)
    {
        redde ORATIO_FORMA_PROSA;
    }
    v = &paragraphus->loci[ORATIO_PARAGRAPHUS_FORMA];
    si (   v->genus       != MATERIA_VALOR_INDEX
        || v->datum.index < ZEPHYRUM
        || v->datum.index >= (s32)ORATIO_FORMA_NUMERUS_FORMARUM)
    {
        redde ORATIO_FORMA_PROSA;
    }
    redde (OratioForma)v->datum.index;
}

Xar*
oratio_forma_elementa (
                  Piscina* piscina,
    constans MateriaNodus* paragraphus)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    constans MateriaValor* sententiae;
    i32 ns;
    i32 s;

    si (exitus == NIHIL || paragraphus == NIHIL)
    {
        redde exitus;
    }
    sententiae = &paragraphus->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    si (sententiae->genus != MATERIA_VALOR_LISTA)
    {
        redde exitus;
    }
    ns = materia_valor_lista_numerus(*sententiae);
    per (s = ZEPHYRUM; s < ns; s++)
    {
        constans MateriaNodus* sen =
            materia_valor_lista_obtinere(*sententiae, s)->datum.nodus;
        constans MateriaValor* elementa =
            &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
                          i32 ne;
                          i32 k;

        si (elementa->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        ne = materia_valor_lista_numerus(*elementa);
        per (k = ZEPHYRUM; k < ne; k++)
        {
            constans MateriaNodus** locus =
                (constans MateriaNodus**)xar_addere(exitus);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus = materia_valor_lista_obtinere(*elementa,
                k)->datum.nodus;
        }
    }
    redde exitus;
}

s32
oratio_forma_initium_lineae (
    constans MateriaNodus* paragraphus)
{
    constans MateriaValor* praefixa;

    si (paragraphus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    /* columna 0 lineae lexematis primi: praefixa paragraphi propria
     * indentationem lineae primae non fert (praefixa documenti aut
     * cauda praecedens eam tenet) - columna lexematis (I-basata,
     * octeti) eam reddit */
    praefixa = &paragraphus->loci[ORATIO_PARAGRAPHUS_PRAEFIXA];
    si (   praefixa->genus == MATERIA_VALOR_LISTA
        && materia_valor_lista_numerus(*praefixa) > ZEPHYRUM)
    {
        constans MateriaToken* t =
            materia_valor_lista_obtinere(*praefixa,
            ZEPHYRUM)->datum.token;

        redde t->byte_offset - ((s32)t->columna - I);
    }

    {
        constans MateriaValor* sententiae =
            &paragraphus->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];

        si (   sententiae->genus == MATERIA_VALOR_LISTA
            && materia_valor_lista_numerus(*sententiae) > ZEPHYRUM)
        {
            constans MateriaNodus* sen =
                materia_valor_lista_obtinere(*sententiae, ZEPHYRUM)
                ->datum.nodus;
            constans MateriaValor* elementa =
                &sen->loci[ORATIO_SENTENTIA_ELEMENTA];

                        si (   elementa->genus == MATERIA_VALOR_LISTA
                            && materia_valor_lista_numerus(*elementa)
                                > ZEPHYRUM)
                        {
                constans MateriaToken* t = _lexema_primum(
                    materia_valor_lista_obtinere(*elementa, ZEPHYRUM)
                    ->datum.nodus);

                redde t == NIHIL ? ZEPHYRUM
                    : t->byte_offset - ((s32)t->columna - I);
                        }

        }
    }
    redde ZEPHYRUM;
}
