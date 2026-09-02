/* saltuarius_quaestio.c - Implementatio quaestionis incrementalis */

#include "saltuarius_quaestio.h"
#include "chorda.h"
#include "utf8.h"
#include <string.h>

vacuum
saltuarius_quaestio_parare (
    SaltuariusQuaestio* quaestio)
{
    memset(quaestio, ZEPHYRUM, magnitudo(SaltuariusQuaestio));
}

/* Textus strati ostensi (dominium quaestionis) */
interior chorda
_textus_ostensus (
    constans SaltuariusLiber* liber)
{
    redde saltuarius_liber_stratum_activum(liber)->textus;
}

/* Prima inventio >= ab; -1 si nulla */
interior s32
_invenire_prorsum (
         chorda  textus,
    constans i8* quaesitum,
            i32  mensura,
            s32  ab)
{
    s32 k;

    si (   mensura == ZEPHYRUM || textus.mensura < mensura
        || ab < ZEPHYRUM)
    {
        redde -I;
    }
    per (k = ab; k + (s32)mensura <= (s32)textus.mensura; k++)
    {
        si (memcmp(textus.datum + k, quaesitum,
            (memoriae_index)mensura) == ZEPHYRUM)
        {
            redde k;
        }
    }
    redde -I;
}

/* Ultima inventio <= ab; -1 si nulla */
interior s32
_invenire_retrorsum (
         chorda  textus,
    constans i8* quaesitum,
            i32  mensura,
            s32  ab)
{
    s32 k;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    {
        redde -I;
    }
    si (ab > (s32)textus.mensura - (s32)mensura)
    {
        ab = (s32)textus.mensura - (s32)mensura;
    }
    per (k = ab; k >= ZEPHYRUM; k--)
    {
        si (memcmp(textus.datum + k, quaesitum,
            (memoriae_index)mensura) == ZEPHYRUM)
        {
            redde k;
        }
    }
    redde -I;
}

/* Originem restituere (FUGA / nihil inventum) */
interior vacuum
_originem_restituere (
    constans SaltuariusQuaestio* quaestio,
                SaltuariusLiber* liber)
{
    liber->cursor_linea    = quaestio->origo_linea;
    liber->cursor_columna  = quaestio->origo_columna;
    liber->volumen_x       = quaestio->origo_vol_x;
    liber->volumen_y       = quaestio->origo_vol_y;
    saltuarius_liber_movere(liber, ZEPHYRUM);
}

/* Re-quaerere AB ORIGINE (incrementalis; cum volutione tacita) */
interior vacuum
_renovare (
    SaltuariusQuaestio* quaestio,
       SaltuariusLiber* liber)
{
    chorda textus = _textus_ostensus(liber);
       s32 inventum;

    quaestio->nihil_inventum = FALSUM;
    si (quaestio->mensura == ZEPHYRUM)
    {
        _originem_restituere(quaestio, liber);
        redde;
    }
    inventum = _invenire_prorsum(textus, quaestio->litterae,
        quaestio->mensura, quaestio->origo_offset);
    si (inventum < ZEPHYRUM)
    {
        inventum = _invenire_prorsum(textus, quaestio->litterae,
            quaestio->mensura, ZEPHYRUM);
    }
    si (inventum >= ZEPHYRUM)
    {
        saltuarius_liber_cursor_ad_offset(liber, inventum);
    }
    alioquin
    {
        _originem_restituere(quaestio, liber);
        quaestio->nihil_inventum = VERUM;
    }
}

vacuum
saltuarius_quaestio_aperire (
          SaltuariusQuaestio* quaestio,
    constans SaltuariusLiber* liber)
{
    quaestio->activa          = VERUM;
    quaestio->mensura         = ZEPHYRUM;
    quaestio->nihil_inventum  = FALSUM;
    quaestio->origo_offset    = saltuarius_liber_cursor_offset(liber);
    si (quaestio->origo_offset < ZEPHYRUM)
    {
        quaestio->origo_offset = ZEPHYRUM;
    }
    quaestio->origo_linea    = liber->cursor_linea;
    quaestio->origo_columna  = liber->cursor_columna;
    quaestio->origo_vol_x    = liber->volumen_x;
    quaestio->origo_vol_y    = liber->volumen_y;
}

SaltuariusQuaestioExitus
saltuarius_quaestio_tradere (
         SaltuariusQuaestio* quaestio,
            SaltuariusLiber* liber,
    constans TesseraEventum* eventum)
{
    si (   !quaestio->activa
        || eventum->genus != TESSERA_EVENTUM_CLAVIS)
    {
        redde SALT_QUAESTIO_PERGIT;
    }

    commutatio (eventum->clavis)
    {
        casus TESSERA_CLAVIS_FUGA:
            _originem_restituere(quaestio, liber);
            quaestio->activa = FALSUM;
            redde SALT_QUAESTIO_CANCELLATA;
        casus TESSERA_CLAVIS_REDITUS:
            /* vacua: commissum prius servatur (vim: '/' Reditus
             * quaestionem ultimam repetit per n) */
            si (quaestio->mensura > ZEPHYRUM)
            {
                memcpy(quaestio->commissum, quaestio->litterae,
                    (memoriae_index)quaestio->mensura);
                quaestio->commissum_mensura = quaestio->mensura;
            }
            quaestio->activa = FALSUM;
            redde SALT_QUAESTIO_COMMISSA;
        casus TESSERA_CLAVIS_RETRORSUM:
        {
            si (quaestio->mensura > ZEPHYRUM)
            {
                constans i8* prior = utf8_prior_runa(
                    quaestio->litterae + quaestio->mensura,
                    quaestio->litterae);

                quaestio->mensura = (i32)(memoriae_index)
                    (prior - quaestio->litterae);
            }
            _renovare(quaestio, liber);
            redde SALT_QUAESTIO_PERGIT;
        }
        ordinarius:
            frange;
    }

    /* runa scribibilis (imperium nullum; pasta = imber runarum) */
    si (   eventum->runa > ZEPHYRUM
        && (eventum->modificatores & TESSERA_MODIFICATOR_IMPERIUM)
            == ZEPHYRUM
        && quaestio->mensura + IV <= SALT_QUAESTIO_MAXIMA)
    {
        s32 scripti = utf8_codere(eventum->runa,
            quaestio->litterae + quaestio->mensura);

        si (scripti > ZEPHYRUM)
        {
            quaestio->mensura += (i32)scripti;
            _renovare(quaestio, liber);
        }
    }
    redde SALT_QUAESTIO_PERGIT;
}

b32
saltuarius_quaestio_proximum (
    constans SaltuariusQuaestio* quaestio,
                SaltuariusLiber* liber,
                            s32  directio,
                            b32* volvit_out)
{
    chorda textus = _textus_ostensus(liber);
       s32 cursor = saltuarius_liber_cursor_offset(liber);
       s32 inventum;

    *volvit_out = FALSUM;
    si (quaestio->commissum_mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (cursor < ZEPHYRUM)
    {
        cursor = ZEPHYRUM;
    }
    si (directio > ZEPHYRUM)
    {
        inventum = _invenire_prorsum(textus, quaestio->commissum,
            quaestio->commissum_mensura, cursor + I);
        si (inventum < ZEPHYRUM)
        {
            inventum = _invenire_prorsum(textus,
                quaestio->commissum, quaestio->commissum_mensura,
                ZEPHYRUM);
            *volvit_out = VERUM;
        }
    }
    alioquin
    {
        inventum = _invenire_retrorsum(textus, quaestio->commissum,
            quaestio->commissum_mensura, cursor - I);
        si (inventum < ZEPHYRUM)
        {
            inventum = _invenire_retrorsum(textus,
                quaestio->commissum, quaestio->commissum_mensura,
                (s32)textus.mensura);
            *volvit_out = VERUM;
        }
    }
    si (inventum < ZEPHYRUM)
    {
        *volvit_out = FALSUM;
        redde FALSUM;
    }
    saltuarius_liber_cursor_ad_offset(liber, inventum);
    redde VERUM;
}
