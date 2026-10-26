/* acordum.c - Acordum: classes cum nomine (vide include/acordum.h)
 *
 * AGNITIO: pro radice quaque inter classes praesentes classes rotantur
 * (radix -> 0) et cum exemplaribus EXACTE comparantur; exemplar primum
 * ordine tabulae vincit. Radix = bassus si congruit, aliter radix
 * congruens minima (ordine classis a C).
 *
 * LECTIO: radix, deinde suffixum longissimum inter exemplaria
 * (aequalia: primum ordine tabulae), deinde '/' bassus; nihil post.
 */

#include "acordum.h"
#include <string.h>

nomen structura {
    AcordumGenus genus;
             s32 gradus[V];   /* intervalla a radice; -1 finit */
} Exemplar;

hic_manens constans Exemplar EXEMPLARIA[] = {
    { ACORDUM_MAIOR,            { 0, 4, 7, -1 } },
    { ACORDUM_MINOR,            { 0, 3, 7, -1 } },
    { ACORDUM_DIMINUTA,         { 0, 3, 6, -1 } },
    { ACORDUM_AUCTA,            { 0, 4, 8, -1 } },
    { ACORDUM_SEPTIMA,          { 0, 4, 7, 10, -1 } },
    { ACORDUM_SEPTIMA,          { 0, 4, 10, -1 } },
    { ACORDUM_SEPTIMA_MAIOR,    { 0, 4, 7, 11, -1 } },
    { ACORDUM_SEPTIMA_MINOR,    { 0, 3, 7, 10, -1 } },
    { ACORDUM_DIMIDIA_DIMINUTA, { 0, 3, 6, 10, -1 } },
    { ACORDUM_DIMINUTA_SEPTIMA, { 0, 3, 6, 9, -1 } },
    { ACORDUM_SUSPENSA_IV,      { 0, 5, 7, -1 } },
    { ACORDUM_SUSPENSA_II,      { 0, 2, 7, -1 } },
    { ACORDUM_QUINTA,           { 0, 7, -1 } },
    { ACORDUM_TERTIA_MAIOR,     { 0, 4, -1 } },
    { ACORDUM_TERTIA_MINOR,     { 0, 3, -1 } }
};

#define ACORDUM_EXEMPLARIA \
    ((i32)(magnitudo(EXEMPLARIA) / magnitudo(EXEMPLARIA[0])))

/* proprietates per genus (ordine enumerationis): suffixum,
 * numeralis, tertia, quinta, septima, gradus */
hic_manens constans AcordumProprietates PROPRIETATES[] = {
    { "",     "",     IV,  VII,  -I,  III },  /* maior */
    { "m",    "",     III, VII,  -I,  III },  /* minor */
    { "dim",  "o",    III, VI,   -I,  III },  /* diminuta */
    { "aug",  "+",    IV,  VIII, -I,  III },  /* aucta */
    { "7",    "",     IV,  VII,  X,   III },  /* septima */
    { "maj7", "M",    IV,  VII,  XI,  III },  /* septima maior */
    { "m7",   "",     III, VII,  X,   III },  /* septima minor */
    { "m7b5", "h",    III, VI,   X,   III },  /* dimidia diminuta */
    { "dim7", "o",    III, VI,   IX,  III },  /* diminuta septima */
    { "sus4", "sus4", -I,  VII,  -I,  II },   /* suspensa IV */
    { "sus2", "sus2", -I,  VII,  -I,  II },   /* suspensa II */
    { "(5)",  "(5)",  -I,  VII,  -I,  I },    /* quinta */
    { "(3)",  "(3)",  IV,  -I,   -I,  I },    /* tertia maior */
    { "m(3)", "(3)",  III, -I,   -I,  I },    /* tertia minor */
    { "",     "",     -I,  -I,   -I,  ZEPHYRUM },  /* classis una */
    { "",     "",     -I,  -I,   -I,  ZEPHYRUM },  /* ignota */
    { "",     "",     -I,  -I,   -I,  ZEPHYRUM }   /* nulla */
};

/* gradus litterarum a radice per intervallum (semitonia 0..11) */
hic_manens constans i32 GRADUS_LITTERARUM[XII] = {
    0, 1, 1, 2, 2, 3, 4, 4, 4, 6, 6, 6
};

/* litterae ordine gradus et classes earum naturales */
hic_manens constans character LITTERAE[] = "cdefgab";
hic_manens constans i32 NATURALES[VII] = { 0, 2, 4, 5, 7, 9, 11 };

/* classis -> bit */
interior i32
_bit (
    i32 classis)
{
    redde (i32)I << classis;
}

/* intervalla exemplaris ut bits */
interior i32
_intervalla (
    constans Exemplar* e)
{
    i32 b = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < V && e->gradus[k] >= ZEPHYRUM; k++)
    {
        b |= _bit((i32)e->gradus[k]);
    }
    redde b;
}

/* classes rotatae ut radix 0 sit */
interior i32
_rotare (
    i32 classes,
    i32 radix)
{
    redde ((classes >> radix) | (classes << (XII - radix))) & 0xFFF;
}

/* index exemplaris quod classibus a radice congruit; -1 nullum */
interior s32
_exemplar (
    i32 classes,
    i32 radix)
{
    i32 r = _rotare(classes, radix);
    i32 i;

    per (i = ZEPHYRUM; i < ACORDUM_EXEMPLARIA; i++)
    {
        si (_intervalla(&EXEMPLARIA[i]) == r)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

Acordum
acordum_ex_classibus (
    i32 classes,
    i32 bassus)
{
    Acordum h;
        s32 electum = -I;
        i32 r;

    memset(h.litterae, ZEPHYRUM, magnitudo(h.litterae));
    h.classes  = classes & 0xFFF;
    h.bassus   = bassus;
    h.radix    = bassus;
    si (h.classes == ZEPHYRUM)
    {
        h.genus = ACORDUM_NULLA;
        redde h;
    }
    si ((h.classes & (h.classes - I)) == ZEPHYRUM)
    {
        h.genus = ACORDUM_CLASSIS_UNA;
        per (r = ZEPHYRUM; r < XII; r++)
        {
            si (h.classes == _bit(r))
            {
                h.radix = r;
            }
        }
        redde h;
    }
    /* bassus primum, deinde classes a C */
    si ((h.classes & _bit(bassus)) && _exemplar(h.classes, bassus) >= 0)
    {
        electum = _exemplar(h.classes, bassus);
        h.radix = bassus;
    }
    per (r = ZEPHYRUM; r < XII && electum < ZEPHYRUM; r++)
    {
        si ((h.classes & _bit(r)) && _exemplar(h.classes, r) >= 0)
        {
            electum = _exemplar(h.classes, r);
            h.radix = r;
        }
    }
    h.genus = electum >= ZEPHYRUM ? EXEMPLARIA[electum].genus
                                  : ACORDUM_IGNOTA;
    si (h.genus == ACORDUM_IGNOTA)
    {
        h.radix = bassus;
    }
    redde h;
}

/* exemplaria sine tertia (acordum_sine_tertia); radix unica in
 * singulis - nulla rotatio alterius congruit */
hic_manens constans Exemplar SINE_TERTIA[IV] = {
    { ACORDUM_CLASSIS_UNA,   { 0, -1 } },
    { ACORDUM_QUINTA,        { 0, 7, -1 } },
    { ACORDUM_SEPTIMA,       { 0, 7, 10, -1 } },
    { ACORDUM_SEPTIMA_MAIOR, { 0, 7, 11, -1 } }
};

b32
acordum_sine_tertia (
     i32  classes,
     i32* radix)
{
    i32 r;
    i32 i;

    classes &= 0xFFF;
    /* radix absens: bit 0 rotatus deest, exemplaria omnia eum habent */
    per (r = ZEPHYRUM; r < XII; r++)
    {
        per (i = ZEPHYRUM; i < IV; i++)
        {
            si (_rotare(classes, r) == _intervalla(&SINE_TERTIA[i]))
            {
                *radix = r;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

constans character*
acordum_suffixum (
    AcordumGenus genus)
{
    redde acordum_proprietates(genus)->suffixum;
}

constans AcordumProprietates*
acordum_proprietates (
    AcordumGenus genus)
{
    si ((i32)genus > (i32)ACORDUM_NULLA)
    {
        redde &PROPRIETATES[ACORDUM_NULLA];
    }
    redde &PROPRIETATES[genus];
}

/* radix ad *i: littera magna A..G, deinde '#' '##' 'b' 'bb'. VERUM ->
 * classis, index litterae (0 = c) */
interior b32
_radicem_legere (
    chorda  titulus,
       i32* i,
       i32* classis,
       i32* gradus)
{
    character c;
          s32 alteratio = ZEPHYRUM;
          i32 k;

    si (*i >= titulus.mensura)
    {
        redde FALSUM;
    }
    c = (character)titulus.datum[*i];
    si (c < 'A' || c > 'G')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < VII; k++)
    {
        si (LITTERAE[k] == (character)(c - 'A' + 'a'))
        {
            *gradus = k;
        }
    }
    (*i)++;
    si (*i < titulus.mensura && titulus.datum[*i] == '#')
    {
        dum (   *i < titulus.mensura && titulus.datum[*i] == '#'
             && alteratio < II)
        {
            alteratio++;
            (*i)++;
        }
    }
    alioquin
    {
        dum (   *i < titulus.mensura && titulus.datum[*i] == 'b'
             && alteratio > -II)
        {
            alteratio--;
            (*i)++;
        }
    }
    *classis = (i32)(((s32)NATURALES[*gradus] + alteratio + XII) % XII);
    redde VERUM;
}

b32
acordum_legere (
      chorda  titulus,
    Acordum*  acordum)
{
    Acordum a;
        i32 i        = ZEPHYRUM;
        i32 radix    = ZEPHYRUM;
        i32 gradus   = ZEPHYRUM;
        s32 electum  = -I;
        i32 longum   = ZEPHYRUM;
        i32 bassus;
        i32 gradus_bassi;
        i32 e;
        i32 k;

    si (!_radicem_legere(titulus, &i, &radix, &gradus))
    {
        redde FALSUM;
    }
    /* suffixum longissimum; aequalia: primum ordine tabulae */
    per (e = ZEPHYRUM; e < ACORDUM_EXEMPLARIA; e++)
    {
         constans character* s =
             PROPRIETATES[EXEMPLARIA[e].genus].suffixum;
                        i32 n = (i32)strlen(s);

        si (   i + n <= titulus.mensura
            && memcmp(titulus.datum + i, s, (size_t)n) == ZEPHYRUM
            && (electum < ZEPHYRUM || n > longum))
        {
            electum  = (s32)e;
            longum   = n;
        }
    }
    si (electum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    i             += longum;
    bassus        = radix;
    gradus_bassi  = gradus;
    si (i < titulus.mensura && titulus.datum[i] == '/')
    {
        i++;
        si (!_radicem_legere(titulus, &i, &bassus, &gradus_bassi))
        {
            redde FALSUM;
        }
    }
    si (i != titulus.mensura)
    {
        redde FALSUM;
    }
    memset(a.litterae, ZEPHYRUM, magnitudo(a.litterae));
    a.genus    = EXEMPLARIA[electum].genus;
    a.radix    = radix;
    a.bassus   = bassus;
    a.classes  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < V
        && EXEMPLARIA[electum].gradus[k] >= ZEPHYRUM;
         k++)
    {
        i32 g = (i32)EXEMPLARIA[electum].gradus[k];
        i32 c = (radix + g) % XII;

        a.classes |= _bit(c);
        a.litterae[c] = LITTERAE[(gradus + GRADUS_LITTERARUM[g]) % VII];
    }
    si (a.litterae[bassus] == '\0')
    {
        a.litterae[bassus] = LITTERAE[gradus_bassi];
    }
    *acordum = a;
    redde VERUM;
}
