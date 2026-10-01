/* probatio_modulus.c - Modulus: cellulae, divisiones, mensura textus
 * (tessellatio T1; casus manu computati) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "runae.h"
#include "modulus.h"
#include "credo.h"
#include <stdio.h>

interior ModulusMensor
_mensor (
    ModulusMensorGenus genus,
         RunaePolitica politica)
{
    ModulusMensor r;

    r.genus     = genus;
    r.politica  = politica;
    redde r;
}

/* Latitudo et altitudo textus (cstr) sub modulo et mensore */
interior vacuum
_metiri (
    constans Modulus* m,
       ModulusMensor  mensor,
  constans character* cstr,
             Piscina* piscina,
                 s32* lat,
                 s32* alt)
{
    modulus_textum_metiri(m, mensor, chorda_ex_literis(cstr, piscina),
        lat, alt);
}

s32 principale (vacuum)
{
          Piscina* piscina;
          Modulus  m;
          Modulus  magnus;
          Modulus  fractus;
    ModulusMensor  fontis;
    ModulusMensor  runarum;
    ModulusMensor  simplex;
              s32  lat;
              s32  alt;
              s32  columnae;
              s32  lineae;

    piscina = piscina_generare_dynamicum("probatio_modulus", LXV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    m       = modulus_creare(VI, VIII, DCXL, CDLXXX);
    magnus  = modulus_creare(VIII, XVI, DCXL, CDLXXX);
    fontis = _mensor(MODULUS_MENSOR_FONTIS,
        RUNAE_POLITICA_GRAPHEMATUM);
    runarum  = _mensor(MODULUS_MENSOR_RUNARUM,
        RUNAE_POLITICA_GRAPHEMATUM);
    simplex  = _mensor(MODULUS_MENSOR_RUNARUM, RUNAE_POLITICA_SIMPLEX);

    imprimere("\n--- I. creare: fines degradant ---\n");
    CREDO_AEQUALIS_S32(m.cellula_latitudo, VI);
    CREDO_AEQUALIS_S32(m.cellula_altitudo, VIII);
    CREDO_AEQUALIS_S32(m.schirmus_x.numerator, I);
    CREDO_AEQUALIS_S32(m.schirmus_y.denominator, I);
    fractus = modulus_creare(ZEPHYRUM, -III, -I, X);
    CREDO_AEQUALIS_S32(fractus.cellula_latitudo, I);
    CREDO_AEQUALIS_S32(fractus.cellula_altitudo, I);
    CREDO_AEQUALIS_S32(fractus.extensio_latitudo, ZEPHYRUM);
    CREDO_AEQUALIS_S32(fractus.extensio_altitudo, X);

    imprimere("\n--- II. columna / linea: pavimentum ---\n");
    CREDO_AEQUALIS_S32(modulus_columna(&m, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna(&m, V), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna(&m, VI), I);
    CREDO_AEQUALIS_S32(modulus_columna(&m, XI), I);
    CREDO_AEQUALIS_S32(modulus_columna(&m, -I), -I);
    CREDO_AEQUALIS_S32(modulus_columna(&m, -VI), -I);
    CREDO_AEQUALIS_S32(modulus_columna(&m, -VII), -II);
    CREDO_AEQUALIS_S32(modulus_linea(&m, VII), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_linea(&m, VIII), I);
    CREDO_AEQUALIS_S32(modulus_linea(&m, -I), -I);

    imprimere("\n--- III. proxima: dimidium sursum ---\n");
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, ZEPHYRUM), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, II), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, III), I);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, VIII), I);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, IX), II);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, -III), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&m, -IV), -I);
    CREDO_AEQUALIS_S32(modulus_linea_proxima(&m, III), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_linea_proxima(&m, IV), I);
    CREDO_AEQUALIS_S32(modulus_linea_proxima(&m, XII), II);
    /* cellula impar (V): nullum dimidium exactum */
    fractus = modulus_creare(V, V, ZEPHYRUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&fractus, II), ZEPHYRUM);
    CREDO_AEQUALIS_S32(modulus_columna_proxima(&fractus, III), I);

    imprimere("\n--- IV. extensio in cellulis ---\n");
    modulus_extensio_cellularum(&m, &columnae, &lineae);
    CREDO_AEQUALIS_S32(columnae, CVI);   /* 640 / 6 = 106,67 */
    CREDO_AEQUALIS_S32(lineae, LX);
    modulus_extensio_cellularum(&magnus, &columnae, &lineae);
    CREDO_AEQUALIS_S32(columnae, LXXX);
    CREDO_AEQUALIS_S32(lineae, XXX);

    imprimere("\n--- V. textus: mensura per scopum ---\n");
    _metiri(&m, fontis, "", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, ZEPHYRUM);
    CREDO_AEQUALIS_S32(alt, ZEPHYRUM);
    _metiri(&m, fontis, "Ok", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XII);
    CREDO_AEQUALIS_S32(alt, VIII);
    _metiri(&magnus, fontis, "Ok", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XVI);
    CREDO_AEQUALIS_S32(alt, XVI);
    _metiri(&m, fontis, "a\nlonger", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XXXVI);
    CREDO_AEQUALIS_S32(alt, XVI);
    _metiri(&m, fontis, "a\n", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, VI);
    CREDO_AEQUALIS_S32(alt, XVI);
    /* 中: fons unam cellulam pingit, terminalis duas (D4) */
    _metiri(&m, fontis, "\xE4\xB8\xAD", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, VI);
    _metiri(&m, runarum, "\xE4\xB8\xAD", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XII);
    CREDO_AEQUALIS_S32(alt, VIII);
    /* e + U+0301: fons duas runas pingit, unitas una */
    _metiri(&m, fontis, "e\xCC\x81", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XII);
    _metiri(&m, runarum, "e\xCC\x81", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, VI);
    /* octetus invalidus: utrique I */
    _metiri(&m, fontis, "\xFF", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, VI);
    _metiri(&m, runarum, "\xFF", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, VI);
    /* 👨‍👩: graphema unum (II) vs SIMPLEX (II + 0 + II) */
    _metiri(&m, runarum, "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9",
        piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XII);
    _metiri(&m, simplex, "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9",
        piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XXIV);
    /* linea longissima ad latitudinem, sub mensore runarum quoque */
    _metiri(&m, runarum, "\xE4\xB8\xAD\nabc", piscina, &lat, &alt);
    CREDO_AEQUALIS_S32(lat, XVIII);
    CREDO_AEQUALIS_S32(alt, XVI);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
