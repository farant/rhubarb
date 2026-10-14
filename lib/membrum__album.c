/* membrum__album.c - index paginatus cum titulis gregum
 * (include/membrum__album.h).
 * Regulae ex calendario translatae (kalendarium A6b, M2a, titulus
 * iteratus et catena titulorum 2026-10-14). */
#include "membrum__album.h"

#include <string.h>

structura Album {
     Piscina* piscina;
         s32  capacitas;
         Xar* ordines;    /* AlbumOrdo */
         Xar* initia;     /* s32: index ordinis primi paginae; NIHIL =
                           * computandum */
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior constans AlbumOrdo*
ordo (
    constans Album* c,
               s32  i)
{
    redde (constans AlbumOrdo*)xar_obtinere(c->ordines, (i32)i);
}

/* titulus gregis cui ordo i pertinet: titulus (ALBUM_TITULUS)
 * proximus prior; -1 si ordo ipse titulus/fascia aut fascia proxima
 * prior (fascia gregem non possidet) */
interior s32
titulum_gregis (
    constans Album* c,
               s32  i)
{
    s32 j;

    si (   i <= ZEPHYRUM || i >= (s32)xar_numerus(c->ordines)
        || ordo(c, i)->genus != ALBUM_RES)
    {
        redde -I;
    }
    per (j = i - I; j >= ZEPHYRUM; j--)
    {
        si (ordo(c, j)->genus != ALBUM_RES)
        {
            redde ordo(c, j)->genus == ALBUM_TITULUS ? j : -I;
        }
    }
    redde -I;
}

/* altitudo tituli i cum titulis sequentibus et re prima post eos */
interior s32
altitudo_catenae (
    constans Album* c,
               s32  i)
{
    s32 h;

    h = ZEPHYRUM;
    per (; i < (s32)xar_numerus(c->ordines); i++)
    {
        h += ordo(c, i)->altitudo;
        si (ordo(c, i)->genus == ALBUM_RES)
        {
            frange;
        }
    }
    redde h;
}

/* initia paginarum (pigre; delentur in album_addere) */
interior Xar*
initia (
    Album* c)
{
    constans AlbumOrdo* o;
                   s32  usus;
                   s32  j;
                   s32  i;

    si (c->initia)
    {
        redde c->initia;
    }
    c->initia = xar_creare(c->piscina, (i32)magnitudo(s32));
    si (!c->initia)
    {
        redde NIHIL;
    }
    *(s32*)xar_addere(c->initia)  = ZEPHYRUM;
    usus                          = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (s32)xar_numerus(c->ordines); i++)
    {
        o = ordo(c, i);
        si (   usus > ZEPHYRUM
            && (   usus + o->altitudo > c->capacitas
                || (   o->genus != ALBUM_RES
                    && usus + altitudo_catenae(c, i) > c->capacitas)))
        {
            *(s32*)xar_addere(c->initia)  = i;
            usus                          = ZEPHYRUM;
            j                             = titulum_gregis(c, i);
            si (j >= ZEPHYRUM)
            {
                usus = ordo(c, j)->altitudo;
            }
        }
        usus += o->altitudo;
    }
    redde c->initia;
}


/* ==================================================
 * API
 * ================================================== */

Album*
album_creare (
     Piscina* piscina,
         s32  capacitas)
{
    Album* c;

    si (!piscina)
    {
        redde NIHIL;
    }
    c = (Album*)piscina_allocare(piscina, magnitudo(Album));
    si (!c)
    {
        redde NIHIL;
    }
    c->piscina    = piscina;
    c->capacitas  = capacitas;
    c->ordines    = xar_creare(piscina, (i32)magnitudo(AlbumOrdo));
    c->initia     = NIHIL;
    redde c->ordines ? c : NIHIL;
}

b32
album_addere (
                   Album* c,
      constans character* id,
              AlbumGenus  genus,
                     s32  altitudo,
                     s32  res)
{
    AlbumOrdo* o;

    si (!c || !id || altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    o = (AlbumOrdo*)xar_addere(c->ordines);
    si (!o)
    {
        redde FALSUM;
    }
    o->id        = chorda_ex_literis(id, c->piscina);   /* copia */
    o->genus     = genus;
    o->altitudo  = altitudo;
    o->res       = genus == ALBUM_RES ? res : ZEPHYRUM;
    c->initia    = NIHIL;
    redde VERUM;
}

i32
album_numerus (
    constans Album* c)
{
    redde c ? xar_numerus(c->ordines) : ZEPHYRUM;
}

constans AlbumOrdo*
album_ordo (
    constans Album* c,
               i32  index)
{
    si (!c || index >= xar_numerus(c->ordines))
    {
        redde NIHIL;
    }
    redde ordo(c, (s32)index);
}

s32
album_paginae (
    Album* c)
{
    Xar* x;

    x = c ? initia(c) : NIHIL;
    redde x ? (s32)xar_numerus(x) : I;
}

s32
album_pagina_coacta (
    Album* c,
      s32  k)
{
    s32 n;

    n = album_paginae(c);
    si (k >= n)
    {
        k = n - I;
    }
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

vacuum
album_pagina (
    Album* c,
      s32  k,
      s32* a,
      s32* b,
      s32* titulus)
{
    Xar* x;

    *a        = ZEPHYRUM;
    *b        = ZEPHYRUM;
    *titulus  = -I;
    x         = c ? initia(c) : NIHIL;
    si (!x)
    {
        redde;
    }
    k   = album_pagina_coacta(c, k);
    *a  = *(s32*)xar_obtinere(x, (i32)k);
    *b  = (i32)(k + I) < xar_numerus(x) ? *(s32*)xar_obtinere(x, (i32)(k
        + I)) : (s32)xar_numerus(c->ordines);
    si (*a > ZEPHYRUM)
    {
        *titulus = titulum_gregis(c, *a);
    }
}

s32
album_pagina_rei (
    Album* c,
      s32  res,
      s32  praeferenda)
{
    s32 k;
    s32 n;
    s32 a;
    s32 b;
    s32 t;
    s32 i;
    s32 prima;

    prima  = -I;
    n      = album_paginae(c);
    per (k = ZEPHYRUM; c && k < n; k++)
    {
        album_pagina(c, k, &a, &b, &t);
        per (i = a; i < b; i++)
        {
            si (   ordo(c, i)->genus == ALBUM_RES
                && ordo(c, i)->res   == res)
            {
                si (k == praeferenda)
                {
                    redde k;
                }
                si (prima < ZEPHYRUM)
                {
                    prima = k;
                }
            }
        }
    }
    redde prima;
}

s32
album_res_prima (
    Album* c,
      s32  k)
{
    s32 a;
    s32 b;
    s32 t;
    s32 i;

    si (!c)
    {
        redde ZEPHYRUM;
    }
    album_pagina(c, k, &a, &b, &t);
    per (i = a; i < b; i++)
    {
        si (ordo(c, i)->genus == ALBUM_RES)
        {
            redde ordo(c, i)->res;
        }
    }
    redde ZEPHYRUM;
}
