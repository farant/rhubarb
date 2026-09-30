/* saltuarius_penicillus.c - Implementatio primitivorum pingendi */

#include "saltuarius_penicillus.h"
#include "runae.h"
#include "utf8.h"

RunaePolitica
saltuarius_pen_politica (
    constans TesseraOpus* opus)
{
    redde (opus != NIHIL && opus->politica == TESSERA_POLITICA_SIMPLEX)
        ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM;
}

constans i8*
saltuarius_pen_unitatem (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* cursor,
      constans i8* finis,
              i32  reliqua,
    TesseraStilus  stilus,
              i32* latitudo)
{
    si (*cursor < 0x20 || *cursor == 0x7F)
    {
        /* imperium purgatum: spatium, columna una */
        si (reliqua < I)
        {
            redde NIHIL;
        }
        tessera_cellulam_ponere(opus, x, y, (i32)' ', stilus);
        *latitudo = I;
        redde cursor + I;
    }
        /* latitudo PRIUS mensurata (unitas pingenda, ut tessera pingit):
     * unitas lata extra pannum non ponitur */
    (vacuum)runae_unitas_proxima(cursor, finis,
        saltuarius_pen_politica(opus), latitudo);
    si (*latitudo > reliqua)
    {
        redde NIHIL;
    }
    redde tessera_graphema_ponere(opus, x, y, cursor, finis, stilus,
        latitudo);
}

i32
saltuarius_pen_textum (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* datum,
              i32  mensura,
              i32  latitudo_max,
    TesseraStilus  stilus)
{
    constans i8* cursor   = datum;
    constans i8* finis    = datum + mensura;
            i32  positae  = ZEPHYRUM;

    dum (cursor < finis && positae < latitudo_max)
    {
                i32  latitudo;
        constans i8* post = saltuarius_pen_unitatem(opus,
            x + (s32)positae, y, cursor, finis, latitudo_max - positae,
            stilus, &latitudo);

        si (post == NIHIL)
        {
            frange;   /* unitas lata non capit */
        }
        positae  += latitudo;
        cursor   = post;
    }
    redde positae;
}

vacuum
saltuarius_pen_cellulas (
                 TesseraOpus* opus,
                         s32  x,
                         s32  y,
    constans QuadransCellula* cellulae,
                         i32  lat,
                         i32  alt)
{
    i32 i;
    i32 j;

    per (j = ZEPHYRUM; j < alt; j++)
    {
        per (i = ZEPHYRUM; i < lat; i++)
        {
             constans QuadransCellula* c = &cellulae[j * lat + i];
                                   i8  octeti[IV];
                                  s32  n = utf8_codere(c->runa, octeti);

            tessera_cellulam_ponere(opus, x + (s32)i, y + (s32)j,
                tessera_signum_ex_octetis(octeti,
                    n > ZEPHYRUM ? (i32)n : ZEPHYRUM),
                tessera_stilus(c->color_litterae, c->color_fundi,
                    ZEPHYRUM));
        }
    }
}

vacuum
saltuarius_pen_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* literis,
                   i32  latitudo_max,
         TesseraStilus  stilus)
{
    i32 mensura = ZEPHYRUM;

    dum (literis[mensura] != '\0')
    {
        mensura++;
    }
    (vacuum)saltuarius_pen_textum(opus, x, y,
        (constans i8*)literis, mensura, latitudo_max, stilus);
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
