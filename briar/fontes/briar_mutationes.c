/* briar_mutationes.c - vide briar_mutationes.h */
#include "briar_mutationes.h"

/* linea [a, b) caput versionis '## vN' est? *numerus = N (>= 1),
 * *caput = textus post '## ' sine spatiis finalibus */
interior b32
_caput_versionis (
    chorda  textus,
       i32  a,
       i32  b,
       i32* numerus,
    chorda* caput)
{
    i32 k      = a + IV;
    i32 n      = ZEPHYRUM;
    i32 finis  = b;

    si (   b - a < V || textus.datum[a] != (i8)'#'
        || textus.datum[a + I]   != (i8)'#'
        || textus.datum[a + II]  != (i8)' '
        || textus.datum[a + III] != (i8)'v')
    {
        redde FALSUM;
    }
    dum (   k < b && textus.datum[k] >= (i8)'0'
         && textus.datum[k] <= (i8)'9')
    {
        n = n * X + (i32)(textus.datum[k] - (i8)'0');
        k++;
    }
    si (   k == a + IV || n == ZEPHYRUM
        || (k < b && textus.datum[k] != (i8)' '
            && textus.datum[k] != (i8)'\r'))
    {
        redde FALSUM;
    }
    dum (   finis > a
         && (   textus.datum[finis - I] == (i8)' '
             || textus.datum[finis - I] == (i8)'\r'))
    {
        finis--;
    }
    *numerus        = n;
    caput->datum    = textus.datum + a + III;
    caput->mensura  = (i32)(finis - a - III);
    redde VERUM;
}

/* capita versionum per ordinem: reddit numerum capitum inventorum;
 * primum in *numerus_primi / *caput_primi; ordo stricte descendens
 * in *descendens */
interior i32
_capita_percurrere (
    chorda  textus,
       i32* numerus_primi,
    chorda* caput_primi,
       b32* descendens)
{
    i32 initium  = ZEPHYRUM;
    i32 inventa  = ZEPHYRUM;
    i32 prior    = ZEPHYRUM;
    i32 k;

    *descendens = VERUM;
    per (k = ZEPHYRUM; k <= (i32)textus.mensura; k++)
    {
           i32 n;
        chorda caput;

        si (k < (i32)textus.mensura && textus.datum[k] != (i8)'\n')
        {
            perge;
        }
        si (_caput_versionis(textus, initium, k, &n, &caput))
        {
            si (inventa == ZEPHYRUM)
            {
                *numerus_primi  = n;
                *caput_primi    = caput;
            }
            alioquin si (n >= prior)
            {
                *descendens = FALSUM;
            }
            prior = n;
            inventa++;
        }
        initium = k + I;
    }
    redde inventa;
}

/* linea [a, b) cum praefixo incipit? */
interior b32
_incipit (
                 chorda  textus,
                    i32  a,
                    i32  b,
     constans character* praefixum)
{
    i32 k;

    per (k = ZEPHYRUM; praefixum[k] != '\0'; k++)
    {
        si (a + k >= b || textus.datum[a + k] != (i8)praefixum[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

i32
briar_mutationes_inedita (
    chorda textus)
{
    i32 initium  = ZEPHYRUM;
    i32 numerus  = ZEPHYRUM;
    b32 intra    = FALSUM;
    i32 k;

    per (k = ZEPHYRUM; k <= (i32)textus.mensura; k++)
    {
        si (k < (i32)textus.mensura && textus.datum[k] != (i8)'\n')
        {
            perge;
        }
        si (_incipit(textus, initium, k, "## "))
        {
            si (intra)
            {
                frange;
            }
            intra = _incipit(textus, initium, k, "## inedita");
        }
        alioquin si (intra && _incipit(textus, initium, k, "- "))
        {
            numerus++;
        }
        initium = k + I;
    }
    redde numerus;
}

i32
briar_mutationes_versio (
    chorda textus)
{
       i32 n = ZEPHYRUM;
    chorda caput;
       b32 descendens;

    caput.datum    = NIHIL;
    caput.mensura  = ZEPHYRUM;
    (vacuum)_capita_percurrere(textus, &n, &caput, &descendens);
    redde n;
}

chorda
briar_mutationes_caput (
    chorda textus)
{
       i32 n = ZEPHYRUM;
    chorda caput;
       b32 descendens;

    caput.datum    = NIHIL;
    caput.mensura  = ZEPHYRUM;
    (vacuum)_capita_percurrere(textus, &n, &caput, &descendens);
    redde caput;
}

b32
briar_mutationes_ordo_rectus (
    chorda textus)
{
       i32 n = ZEPHYRUM;
    chorda caput;
       b32 descendens;

    caput.datum    = NIHIL;
    caput.mensura  = ZEPHYRUM;
    redde _capita_percurrere(textus, &n, &caput, &descendens) > ZEPHYRUM
        && descendens;
}
