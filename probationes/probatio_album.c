/* probatio_album.c - index paginatus cum titulis gregum
 * (nomenclator-plan U1): paginae, titulus numquam ultimus, titulus
 * iteratus (fascia non), res et paginae */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "album.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* res n (altitudo II, ids "r<prima>".."r<prima+n-1>", res = numerus) */
interior vacuum
res_addere (
        Album* c,
          s32  prima,
          s32  n)
{
    character id[XVI];
          s32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        sprintf(id, "r%d", (integer)(prima + i));
        (vacuum)album_addere(c, id, ALBUM_RES, II, prima + i);
    }
}

/* pagina k: [a, b) et titulus aequales datis ? */
interior b32
pagina_est (
        Album* c,
          s32  k,
          s32  a,
          s32  b,
          s32  titulus)
{
    s32 x;
    s32 y;
    s32 t;

    album_pagina(c, k, &x, &y, &t);
    si (x != a || y != b || t != titulus)
    {
        imprimere("  pagina %d: [%d, %d) titulus %d\n", (integer)k,
            (integer)x, (integer)y, (integer)t);
        redde FALSUM;
    }
    redde VERUM;
}

/* paginae: vacuum I; capacitas; ordo maior solus; coactio */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
    Album* k;

    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    CREDO_AEQUALIS_S32(album_paginae(k), I);
    CREDO_VERUM(pagina_est(k, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, -I));
    CREDO_AEQUALIS_S32(album_res_prima(k, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32(album_pagina_coacta(k, V), ZEPHYRUM);
    CREDO_AEQUALIS_S32(album_pagina_coacta(k, -III), ZEPHYRUM);
    /* r1 (II), magnus (XIV > X), r2: tres paginae */
    res_addere(k, I, I);
    CREDO_VERUM(album_addere(k, "magnus", ALBUM_RES, XIV, L));
    res_addere(k, II, I);
    CREDO_AEQUALIS_S32(album_paginae(k), III);
    CREDO_VERUM(pagina_est(k, I, I, II, -I));
    CREDO_AEQUALIS_S32(album_pagina_coacta(k, VII), II);
    CREDO_AEQUALIS_I32(album_numerus(k), III);
}

/* titulus numquam ultimus: r1-r3 (VI), "p" fascia, "p0" titulus, r4 -
 * catena (VI) non capit: "p" in paginam II (non solus in fine) */
interior vacuum
_probare_titulum_ultimum (
    CredoContextus* c)
{
    Album* k;

    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    res_addere(k, I, III);
    (vacuum)album_addere(k, "p", ALBUM_FASCIA, II, ZEPHYRUM);
    (vacuum)album_addere(k, "p0", ALBUM_TITULUS, II, ZEPHYRUM);
    res_addere(k, IV, I);
    CREDO_AEQUALIS_S32(album_paginae(k), II);
    CREDO_VERUM(pagina_est(k, ZEPHYRUM, ZEPHYRUM, III, -I));
    CREDO_VERUM(pagina_est(k, I, III, VI, -I));
}

/* titulus iteratus cum spatio servato; fascia non iteratur; titulus
 * post fasciam iteratur */
interior vacuum
_probare_iterationem (
    CredoContextus* c)
{
    Album* k;

    /* g0 + r1-r9: pagina I g0 r1-r4, II (g0) r5-r8, III (g0) r9 - sine
     * spatio servato II paginae essent */
    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    (vacuum)album_addere(k, "g0", ALBUM_TITULUS, II, ZEPHYRUM);
    res_addere(k, I, IX);
    CREDO_AEQUALIS_S32(album_paginae(k), III);
    CREDO_VERUM(pagina_est(k, ZEPHYRUM, ZEPHYRUM, V, -I));
    CREDO_VERUM(pagina_est(k, I, V, IX, ZEPHYRUM));
    CREDO_VERUM(pagina_est(k, II, IX, X, ZEPHYRUM));
    /* fascia + r1-r5: pagina II incipit r5, nihil iteratum */
    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    (vacuum)album_addere(k, "p", ALBUM_FASCIA, II, ZEPHYRUM);
    res_addere(k, I, V);
    CREDO_AEQUALIS_S32(album_paginae(k), II);
    CREDO_VERUM(pagina_est(k, I, V, VI, -I));
    /* fascia, titulus, r1-r5: pagina II iterat "p0" (index I) */
    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    (vacuum)album_addere(k, "p", ALBUM_FASCIA, II, ZEPHYRUM);
    (vacuum)album_addere(k, "p0", ALBUM_TITULUS, II, ZEPHYRUM);
    res_addere(k, I, V);
    CREDO_AEQUALIS_S32(album_paginae(k), II);
    CREDO_VERUM(pagina_est(k, I, V, VII, I));
}

/* res: pagina rei (iterata in gregibus duobus: praeferenda), res prima
 * paginae, id copiatum */
interior vacuum
_probare_res (
    CredoContextus* c)
{
                     Album* k;
                 character  id[VIII];
        constans AlbumOrdo* o;

    k = album_creare(c->piscina, X);
    CREDO_NECESSE_NON_NIHIL(k);
    strcpy(id, "g0");
    (vacuum)album_addere(k, id, ALBUM_TITULUS, II, ZEPHYRUM);
    strcpy(id, "XX");
    res_addere(k, I, IV);
    (vacuum)album_addere(k, "g1", ALBUM_TITULUS, II, ZEPHYRUM);
    (vacuum)album_addere(k, "r2-1", ALBUM_RES, II, II);
    CREDO_AEQUALIS_S32(album_paginae(k), II);
    CREDO_AEQUALIS_S32(album_pagina_rei(k, II, I), I);
    CREDO_AEQUALIS_S32(album_pagina_rei(k, II, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32(album_pagina_rei(k, II, -I), ZEPHYRUM);
    CREDO_AEQUALIS_S32(album_pagina_rei(k, IV, I), ZEPHYRUM);
    CREDO_AEQUALIS_S32(album_pagina_rei(k, XCIX, ZEPHYRUM), -I);
    CREDO_AEQUALIS_S32(album_res_prima(k, ZEPHYRUM), I);
    CREDO_AEQUALIS_S32(album_res_prima(k, I), II);
    o = album_ordo(k, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_AEQUALIS_LITERIS(o->id, "g0");
    CREDO_VERUM(o->genus == ALBUM_TITULUS);
    o = album_ordo(k, V);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_AEQUALIS_LITERIS(o->id, "g1");
    CREDO_NIHIL(album_ordo(k, C));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "paginae", _probare_paginas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "titulus_ultimus", _probare_titulum_ultimum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iteratio", _probare_iterationem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "res", _probare_res, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("album", SECTIONES);
}
