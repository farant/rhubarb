#ifndef TESTA_H
#define TESTA_H

/* testa.h - "applicatio sum: da mihi testam" (testa-spec II.1).
 * Includere = testam accipere: lib/testa.c (macOS, et quaevis fenestra
 * perscrutabilis) aut lib/testa_android.c (--varians android,posix) -
 * regula variantium aedilis, nullus casus in scriptis. Applicatio
 * applicatio_initium definit et principale unius lineae quod
 * testa_principale vocat (bibliothecae principale non habent).
 * Probationes testa_cursus.h solum includunt. */

/* REGULAE (testa-spec II.9; Franus 2026-10-23: "shared material ...
 * continuity / same aesthetics", non applicatio una ubique).
 *
 * MATERIA COMMUNIS (documenta, actiones, figurae, insula, volumen,
 * thema) - leges, porta android-struere eas probat:
 *  1. nulla vocatio platformae: servitia ut portae ab hospite veniunt
 *     (exemplar: actiones_ctx.imago_capere pictoris)
 *  2. tempus ex 'nunc' (dispensatoris), numquam "N ms post cursum
 *     meum"; nihil obstruit, nulla ansa propria - applicatio in pausa
 *     (Android) quadra nulla accipit
 *  3. status qui valet in insula / volumine, numquam in memoria C sola
 *     (Android processus in fundo necat - B2)
 *
 * COMPOSITIO (pictor_componentia et similia, per superficiem) suam
 * superficiem supponere potest: super, dextra, claves in mensa; tactus
 * et situs erectus in tabula. Viae ab hospite (a->sedes_datorum),
 * numquam ex cwd binarii supposito. Documentum aspectui originis suae
 * pertinet; clientes alii fortasse solum legunt. */

#include "testa_cursus.h"

/* Status applicationis in a->piscina aedificare et formam implere.
 * Semel vocatur, schirmo noto; FALSUM = exitus I (nuntium ipsa
 * scribit). Monstratores in formam dati vivere debent usque ad finem:
 * nulla variabilis localis (R3). */
b32
applicatio_initium (
    constans TestaAmbitus* a,
               TestaForma* forma);

/* Introitus testae: principale APPLICATIONIS hoc solum vocat (linea
 * una - bibliothecae principale non habent, regula domus; porta
 * instrumenta introitus aedificat). macOS: fenestra, ansa, exitus.
 * Android: introitus verus ANativeActivity_onCreate
 * (lib/testa_android.c) - testa_principale ibi numquam vocatur, I
 * reddit. */
s32
testa_principale (
        integer   argc,
      character** argv);

#endif /* TESTA_H */
