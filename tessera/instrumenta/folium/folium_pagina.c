/* folium_pagina.c - Vide folium_pagina.h */

#include "folium_pagina.h"
#include "utf8.h"

/* Lineam [initium, finis) addere */
interior vacuum
_lineam_addere (
    FoliumInvolutio* involutio,
                i32  initium,
                i32  finis,
                i32  latitudo)
{
    FoliumLinea* l = &involutio->lineae[involutio->numerus++];

    l->initium   = initium;
    l->finis     = finis;
    l->latitudo  = latitudo;
}

/* Unitas NATURA lata: runa prima ipsa II (East_Asian W/F, emoji) -
 * non graphema per signum spatians (Mc) amplificatum, aliter verba
 * Hindi intra se frangerentur (mensuratum in aureo hi) */
interior b32
_natura_lata (
    constans i8* cursor,
    constans i8* finis)
{
    constans i8* post = cursor;
            s32  runa = utf8_decodere(&post, finis);

    redde (b32)(runa >= ZEPHYRUM && runae_latitudo(runa) == II);
}

/* Paragraphum [initium, finis) involvere: ruptura licita memoratur
 * (post spatium; inter unitates ubi alterutra NATURA lata); cum unitas
 * limitem excedit, ibi frangitur, aliter ante unitatem ipsam (Thai).
 * Unitates latitudinis 0 rupturas non mutant. */
interior vacuum
_paragraphum_involvere (
        constans i8* t,
                i32  initium,
                i32  finis,
                i32  latitudo,
      RunaePolitica  politica,
    FoliumInvolutio* involutio)
{
    i32 linea         = initium;   /* initium lineae currentis */
    i32 columna       = ZEPHYRUM;
    i32 ruptura       = initium;   /* == linea: nulla ruptura */
    i32 col_rupturae  = ZEPHYRUM;
    b32 lata_prior    = FALSUM;
    b32 spatium       = FALSUM;
    i32 cursor        = initium;

    dum (cursor < finis)
    {
                i32  lat;
        constans i8* post = runae_unitas_proxima(t + cursor, t + finis,
            politica, &lat);
                 b32 lata = _natura_lata(t + cursor, t + finis);

        involutio->unitates++;
        involutio->columnae += lat;
        si (lat == II)
        {
            involutio->latae++;
        }
        si (lat == ZEPHYRUM)
        {
            involutio->nullae++;
        }
        si (   lat > ZEPHYRUM && cursor > linea
            && (spatium || lata_prior || lata))
        {
            ruptura       = cursor;   /* licita ANTE hanc unitatem */
            col_rupturae  = columna;
        }
        dum (   lat > ZEPHYRUM && columna + lat > latitudo
             && cursor > linea)
        {
            si (ruptura > linea)
            {
                _lineam_addere(involutio, linea, ruptura, col_rupturae);
                linea    = ruptura;
                columna  -= col_rupturae;
            }
            alioquin
            {
                _lineam_addere(involutio, linea, cursor, columna);
                linea    = cursor;
                columna  = ZEPHYRUM;
            }
            ruptura = linea;
        }
        columna += lat;
        si (lat > ZEPHYRUM)
        {
            spatium     = (b32)(t[cursor] == ' ');
            lata_prior  = lata;
        }
        cursor = (i32)(post - t);
    }
    _lineam_addere(involutio, linea, finis, columna);
}

b32
folium_involvere (
             Piscina* piscina,
              chorda  textus,
                 i32  latitudo,
       RunaePolitica  politica,
     FoliumInvolutio* involutio)
{
    constans i8* t        = textus.datum;
            i32  initium  = ZEPHYRUM;
            i32  finis;

    involutio->lineae    = NIHIL;
    involutio->numerus   = ZEPHYRUM;
    involutio->latitudo  = latitudo;
    involutio->unitates  = ZEPHYRUM;
    involutio->columnae  = ZEPHYRUM;
    involutio->latae     = ZEPHYRUM;
    involutio->nullae    = ZEPHYRUM;
    si (latitudo < I)
    {
        redde FALSUM;
    }
    /* lineae <= unitates + paragraphi <= octeti + I */
    involutio->lineae = (FoliumLinea*)piscina_allocare_ordinatum(
        piscina, ((memoriae_index)textus.mensura + I)
            * (memoriae_index)magnitudo(FoliumLinea), IV);
    si (involutio->lineae == NIHIL)
    {
        redde FALSUM;
    }
    dum (VERUM)
    {
        i32 cauda;

        finis = initium;
        dum (finis < textus.mensura && t[finis] != '\n')
        {
            finis++;
        }
        cauda = finis;
        si (cauda > initium && t[cauda - I] == '\r')
        {
            cauda--;
        }
        _paragraphum_involvere(t, initium, cauda, latitudo, politica,
            involutio);
        si (finis >= textus.mensura || finis + I >= textus.mensura)
        {
            frange;   /* \n ultimus paragraphum vacuum non aperit */
        }
        initium = finis + I;
    }
    redde VERUM;
}

vacuum
folium_paginam_pingere (
                 TesseraOpus* opus,
                      chorda  textus,
    constans FoliumInvolutio* involutio,
                         i32  prima,
                         i32  altitudo,
               TesseraStilus  stilus)
{
    i32 k;

    per (k = ZEPHYRUM; k < altitudo && prima + k < involutio->numerus;
         k++)
    {
        constans FoliumLinea* l       = &involutio->lineae[prima + k];
                 constans i8* cursor  = textus.datum + l->initium;
                 constans i8* finis   = textus.datum + l->finis;
                         s32  x       = ZEPHYRUM;

        dum (cursor < finis)
        {
            i32 lat;

            si (*cursor < 0x20 || *cursor == 0x7F)
            {
                /* imperium (tabula inclusa) -> spatium */
                tessera_cellulam_ponere(opus, x, (s32)k, (i32)' ',
                    stilus);
                x++;
                cursor++;
                perge;
            }
            cursor = tessera_graphema_ponere(opus, x, (s32)k, cursor,
                finis, stilus, &lat);
            x += (s32)lat;
        }
    }
}
