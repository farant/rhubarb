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
             s32 gradus[VI];     /* intervalla a radice; -1 finit */
             s32 litterae[VI];   /* gradus litterarum a radice, per
                                  * tonum (sexta +5, septima diminuta
                                  * +6: intervallum IX ambiguum) */
             b32 radix_basso;    /* nomen solum radice in basso (6, m6:
                                  * classes m7 / m7b5 eaedem) */
} Exemplar;

hic_manens constans Exemplar EXEMPLARIA[] = {
    { ACORDUM_MAIOR,            { 0, 4, 7, -1, -1, -1 },
                                { 0, 2, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_MINOR,            { 0, 3, 7, -1, -1, -1 },
                                { 0, 2, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_DIMINUTA,         { 0, 3, 6, -1, -1, -1 },
                                { 0, 2, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_AUCTA,            { 0, 4, 8, -1, -1, -1 },
                                { 0, 2, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA,          { 0, 4, 7, 10, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA,          { 0, 4, 10, -1, -1, -1 },
                                { 0, 2, 6, -1, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA_MAIOR,    { 0, 4, 7, 11, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA_MINOR,    { 0, 3, 7, 10, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM },
    { ACORDUM_DIMIDIA_DIMINUTA, { 0, 3, 6, 10, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM },
    { ACORDUM_DIMINUTA_SEPTIMA, { 0, 3, 6, 9, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM },
    { ACORDUM_SUSPENSA_IV,      { 0, 5, 7, -1, -1, -1 },
                                { 0, 3, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_SUSPENSA_II,      { 0, 2, 7, -1, -1, -1 },
                                { 0, 1, 4, -1, -1, -1 }, FALSUM },
    { ACORDUM_QUINTA,           { 0, 7, -1, -1, -1, -1 },
                                { 0, 4, -1, -1, -1, -1 }, FALSUM },
    { ACORDUM_TERTIA_MAIOR,     { 0, 4, -1, -1, -1, -1 },
                                { 0, 2, -1, -1, -1, -1 }, FALSUM },
    { ACORDUM_TERTIA_MINOR,     { 0, 3, -1, -1, -1, -1 },
                                { 0, 2, -1, -1, -1, -1 }, FALSUM },
    { ACORDUM_SEXTA,            { 0, 4, 7, 9, -1, -1 },
                                { 0, 2, 4, 5, -1, -1 }, VERUM },
    { ACORDUM_SEXTA_MINOR,      { 0, 3, 7, 9, -1, -1 },
                                { 0, 2, 4, 5, -1, -1 }, VERUM },
    { ACORDUM_NONA_ADDITA,      { 0, 2, 4, 7, -1, -1 },
                                { 0, 1, 2, 4, -1, -1 }, FALSUM },
    { ACORDUM_NONA,             { 0, 4, 7, 10, 2, -1 },
                                { 0, 2, 4, 6, 1, -1 }, FALSUM },
    { ACORDUM_NONA,             { 0, 4, 10, 2, -1, -1 },
                                { 0, 2, 6, 1, -1, -1 }, FALSUM },
    { ACORDUM_NONA_MINOR,       { 0, 4, 7, 10, 1, -1 },
                                { 0, 2, 4, 6, 1, -1 }, FALSUM },
    { ACORDUM_NONA_MINOR,       { 0, 4, 10, 1, -1, -1 },
                                { 0, 2, 6, 1, -1, -1 }, FALSUM },
    { ACORDUM_NONA_MAIOR,       { 0, 4, 7, 11, 2, -1 },
                                { 0, 2, 4, 6, 1, -1 }, FALSUM },
    { ACORDUM_NONA_MOLLIS,      { 0, 3, 7, 10, 2, -1 },
                                { 0, 2, 4, 6, 1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA_SUSPENSA, { 0, 5, 7, 10, -1, -1 },
                                { 0, 3, 4, 6, -1, -1 }, VERUM },
    { ACORDUM_SEPTIMA_AUCTA,    { 0, 4, 8, 10, -1, -1 },
                                { 0, 2, 4, 6, -1, -1 }, FALSUM }
};

#define ACORDUM_EXEMPLARIA \
    ((i32)(magnitudo(EXEMPLARIA) / magnitudo(EXEMPLARIA[0])))

/* bit intervalli (additae) */
#define ADDITUM(x) ((i32)I << (x))
/* 7sus4: omnes praeter radicem tenendae (quarta, quinta, septima) */
#define ADDITAE_SUSPENSAE (ADDITUM(V) | ADDITUM(VII) | ADDITUM(X))

/* proprietates per genus (ordine enumerationis): suffixum,
 * numeralis, tertia, quinta, septima, gradus, additae */
hic_manens constans AcordumProprietates PROPRIETATES[] = {
    { "",      "",      IV,  VII,  -I,  III, ZEPHYRUM },  /* maior */
    { "m",     "",      III, VII,  -I,  III, ZEPHYRUM },  /* minor */
    { "dim",   "o",     III, VI,   -I,  III, ZEPHYRUM },  /* diminuta */
    { "aug",   "+",     IV,  VIII, -I,  III, ZEPHYRUM },  /* aucta */
    { "7",     "",      IV,  VII,  X,   III, ZEPHYRUM },  /* septima */
    { "maj7",  "M",     IV,  VII,  XI,  III, ZEPHYRUM },  /* septima
                                                           * maior */
    { "m7",    "",      III, VII,  X,   III, ZEPHYRUM },  /* septima
                                                           * minor */
    { "m7b5",  "h",     III, VI,   X,   III, ZEPHYRUM },  /* dimidia
                                                           * diminuta */
    { "dim7",  "o",     III, VI,   IX,  III, ZEPHYRUM },  /* diminuta
                                                           * septima */
    { "sus4",  "sus4",  -I,  VII,  -I,  II,  ZEPHYRUM },  /* suspensa
                                                           * IV */
    { "sus2",  "sus2",  -I,  VII,  -I,  II,  ZEPHYRUM },  /* suspensa
                                                           * II */
    { "(5)",   "(5)",   -I,  VII,  -I,  I,   ZEPHYRUM },  /* quinta */
    { "(3)",   "(3)",   IV,  -I,   -I,  I,   ZEPHYRUM },  /* tertia
                                                           * maior */
    { "m(3)",  "(3)",   III, -I,   -I,  I,   ZEPHYRUM },  /* tertia
                                                           * minor */
    /* sexta */
    { "6",     "add6",  IV,  VII,  -I,  III, ADDITUM(IX) },
    /* sexta minor */
    { "m6",    "add6",  III, VII,  -I,  III, ADDITUM(IX) },
    /* nona addita */
    { "add9",  "add9",  IV,  VII,  -I,  III, ADDITUM(II) },
    /* nona */
    { "9",     "9",     IV,  VII,  X,   III, ADDITUM(II) },
    /* nona minor */
    { "7b9",   "7b9",   IV,  VII,  X,   III, ADDITUM(I) },
    /* nona maior */
    { "maj9",  "M9",    IV,  VII,  XI,  III, ADDITUM(II) },
    /* nona mollis */
    { "m9",    "9",     III, VII,  X,   III, ADDITUM(II) },
    /* septima suspensa */
    { "7sus4", "7sus4", -I,  VII,  X,   II,  ADDITAE_SUSPENSAE },
    /* septima aucta */
    { "7#5",   "+7",    IV,  VIII, X,   III, ADDITUM(VIII) },
    /* classis una */
    { "",      "",      -I,  -I,   -I,  ZEPHYRUM, ZEPHYRUM },
    /* ignota */
    { "",      "",      -I,  -I,   -I,  ZEPHYRUM, ZEPHYRUM },
    /* nulla */
    { "",      "",      -I,  -I,   -I,  ZEPHYRUM, ZEPHYRUM }
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

    per (k = ZEPHYRUM; k < VI && e->gradus[k] >= ZEPHYRUM; k++)
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

/* index exemplaris quod classibus a radice congruit; -1 nullum.
 * Exemplaria radicis in basso (6, m6) solum radice = bassus */
interior s32
_exemplar (
    i32 classes,
    i32 radix,
    i32 bassus)
{
    i32 r = _rotare(classes, radix);
    i32 i;

    per (i = ZEPHYRUM; i < ACORDUM_EXEMPLARIA; i++)
    {
        si (   _intervalla(&EXEMPLARIA[i]) == r
            && (!EXEMPLARIA[i].radix_basso || radix == bassus))
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
    si (   (h.classes & _bit(bassus))
        && _exemplar(h.classes, bassus, bassus) >= 0)
    {
        electum = _exemplar(h.classes, bassus, bassus);
        h.radix = bassus;
    }
    per (r = ZEPHYRUM; r < XII && electum < ZEPHYRUM; r++)
    {
        si (   (h.classes & _bit(r))
            && _exemplar(h.classes, r, bassus) >= 0)
        {
            electum = _exemplar(h.classes, r, bassus);
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
    { ACORDUM_CLASSIS_UNA,   { 0, -1, -1, -1, -1, -1 },
                             { 0, -1, -1, -1, -1, -1 }, FALSUM },
    { ACORDUM_QUINTA,        { 0, 7, -1, -1, -1, -1 },
                             { 0, 4, -1, -1, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA,       { 0, 7, 10, -1, -1, -1 },
                             { 0, 4, 6, -1, -1, -1 }, FALSUM },
    { ACORDUM_SEPTIMA_MAIOR, { 0, 7, 11, -1, -1, -1 },
                             { 0, 4, 6, -1, -1, -1 }, FALSUM }
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
    per (k = ZEPHYRUM; k < VI
        && EXEMPLARIA[electum].gradus[k] >= ZEPHYRUM;
         k++)
    {
        i32 g = (i32)EXEMPLARIA[electum].gradus[k];
        i32 c = (radix + g) % XII;
        i32 l = (i32)EXEMPLARIA[electum].litterae[k];

        a.classes      |= _bit(c);
        a.litterae[c]  = LITTERAE[(gradus + l) % VII];
    }
    si (a.litterae[bassus] == '\0')
    {
        a.litterae[bassus] = LITTERAE[gradus_bassi];
    }
    *acordum = a;
    redde VERUM;
}
