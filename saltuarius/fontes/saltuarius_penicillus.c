/* saltuarius_penicillus.c - Implementatio primitivorum pingendi */

#include "saltuarius_penicillus.h"
#include "utf8.h"

i32
saltuarius_pen_textum (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* datum,
              i32  mensura,
              i32  runae_max,
    TesseraStilus  stilus)
{
    constans i8* cursor   = datum;
    constans i8* finis    = datum + mensura;
            i32  positae  = ZEPHYRUM;

    dum (cursor < finis && positae < runae_max)
    {
        constans i8* proxima  = utf8_proxima_runa(cursor, finis);
                i32  octeti    = (i32)(memoriae_index)(proxima
                    - cursor);
                i32 signum;

        si (   octeti == I
            && (cursor[ZEPHYRUM] < 0x20 || cursor[ZEPHYRUM] == 0x7F))
        {
            signum = (i32)' ';   /* imperium purgatum */
        }
        alioquin
        {
            signum = tessera_signum_ex_octetis(cursor, octeti);
        }
        tessera_cellulam_ponere(opus, x + (s32)positae, y, signum,
            stilus);
        cursor = proxima;
        positae++;
    }
    redde positae;
}

vacuum
saltuarius_pen_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* literis,
                   i32  runae_max,
         TesseraStilus  stilus)
{
    i32 mensura = ZEPHYRUM;

    dum (literis[mensura] != '\0')
    {
        mensura++;
    }
    (vacuum)saltuarius_pen_textum(opus, x, y,
        (constans i8*)literis, mensura, runae_max, stilus);
}

i32
saltuarius_pen_numerum (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              s32  valor,
    TesseraStilus  stilus)
{
    character buffer[XVI];
          i32 digiti    = ZEPHYRUM;
          s32 reliquum  = valor;
          i32 k;

    si (reliquum < ZEPHYRUM)
    {
        reliquum = ZEPHYRUM;
    }
    fac
    {
        buffer[digiti] = (character)('0' + (reliquum % X));
        digiti++;
        reliquum /= X;
    } dum (reliquum > ZEPHYRUM && digiti < XVI);

    per (k = ZEPHYRUM; k < digiti; k++)
    {
        tessera_cellulam_ponere(opus, x + (s32)k, y,
            (i32)buffer[digiti - I - k], stilus);
    }
    redde digiti;
}

i32
saltuarius_pen_digiti (
    s32 valor)
{
    i32 numerus = I;

    dum (valor >= X)
    {
        valor /= X;
        numerus++;
    }
    redde numerus;
}
