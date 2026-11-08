/* probatio_dithering_exemplaria_velox.c - modus per exemplaria, velox
 *
 * Sectiones communes (probationes/dithering_exemplaria_commune.h)
 * super functionem velocem, et praeterea in fixis
 * probationes/fixa/exemplaria/ (golem, pratum ad DC latitudinis):
 *   classica fixa     indices classici (optiones ordinariae, colores
 *                     omnes) per blake2b fixi - classicus numquam
 *                     mutatur (spec decretum 2)
 *   colores puri      per indicem i 0..XV imago plana XVIxXVI coloris
 *                     AQUINAS_PALETTE[i], colores omnes -> indices
 *                     omnes == i (index fractus nominatur)
 *   pauci colores     greges activi {V}, {0, V}, {0, V, VI}: scaena
 *                     LXXXxLX solum activos reddit; colores puri
 *                     intra gregem exacti
 *   memoria minor     in lapide DC: piscina_summa_usus veloci <
 *                     classici (piscinae novae ambae)
 *   velox fixa        indices veloces per blake2b in fixis (capti
 *                     T3, ante obscurationem divisam) - refactio
 *                     exacta nihil mutat; renovatur solum cum task
 *                     differre vult (et dicit cur)
 *   magnitudines oris XIIxXII, XIIIxXIII, XXVxIII, IIIxXXV, DCIxII
 *                     (scaena): successus, colores activi soli,
 *                     digestum idem ac T3
 *   alpha neglecta    canalis alpha non legitur (ut classicus)
 *   sine memoria      nihil inter vocationes servatur: A, B, A ->
 *                     A idem
 * Fixura suitae fixa semel legit et classicum semel currit.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "imago.h"
#include "blake2b.h"
#include "dithering.h"
#include "dithering_exemplaria.h"
#include "dithering_exemplaria_velox.h"
#include "dithering_exemplaria_commune.h"
#include <stdio.h>
#include <string.h>

#define FIXA_NUMERUS II

hic_manens constans character* FIXA_VIAE[FIXA_NUMERUS] = {
    "probationes/fixa/exemplaria/lapis_600.png",
    "probationes/fixa/exemplaria/pratum_600.png"
};

/* blake2b-256 indicum classicorum (T1, 2026-11-07, ante motum
 * fontium communium) */
hic_manens constans character* CLASSICA_DIGESTA[FIXA_NUMERUS] = {
    "bcbff2e7279f3ff51f084e22edf6080cceb122329abc4a509298208a358636f6",
    "80bb381c101607d0cb8c7b653cfe6d6dfc35a8375599647c7238d51765398d0a"
};

/* blake2b-256 indicum velocium (T3, 6cc3089d5: plana f32, ante
 * obscurationem divisam T4) */
hic_manens constans character* VELOX_DIGESTA[FIXA_NUMERUS] = {
    "0d160512a8ace113ad9fd8d815ee9151bf45ecbcbe7944d2071c05ba1ebb9936",
    "4b99fcaea0b96f51be58fdd041d84b989a927d6539340b5d56959030b49e19b4"
};

#define ORAE_NUMERUS V

hic_manens constans s32 ORAE_MAGNITUDINES[ORAE_NUMERUS][II] = {
    {XII, XII}, {XIII, XIII}, {XXV, III}, {III, XXV}, {DCI, II}
};

/* idem, scaena in magnitudinibus orae, colores grisei */
hic_manens constans character* ORAE_DIGESTA[ORAE_NUMERUS] = {
    "8253944afa6551bd02cad46b90bf3cc61d6f6a84f684c4c322f7e1f3b059d40c",
    "39995c9b4b2fb7962466007f09e03f9bc86c235bf9797dc7dfa239e2a558d0b2",
    "6d4ee9f24ea6067413a1e99d96ce537ad2f5fcd05d729bc89bebc12fdbc2a916",
    "20aba054d269361b66a39930febb5cc929e083186a17ad999c223a6db06541f8",
    "9ed48b1080e0f7977f84a9e0bfa8b4579380f2596cd3badc9d1e7da0d3fef32d"
};

nomen structura {
    DitheringExemplariaProbandum probandum;  /* primum: commune legit */
                           Imago imagines[FIXA_NUMERUS];
                DitheringFructus classica[FIXA_NUMERUS];
} FixuraVelox;

interior vacuum
_parare_suitae (
    CredoContextus* c)
{
    FixuraVelox* fx = (FixuraVelox*)piscina_allocare(c->piscina,
        (memoriae_index)magnitudo(FixuraVelox));
    b32 activi[XVI];
    s32 i;

    CREDO_NECESSE(fx != NIHIL);
    fx->probandum.functio = dithering_exemplaria_velox_colorum;
    dithering_praeparare_omnes(activi);
    per (i = 0; i < FIXA_NUMERUS; i++)
    {
        ImagoFructus f = imago_caricare_ex_file(FIXA_VIAE[i],
            c->piscina);
        CREDO_NECESSE(f.successus);
        fx->imagines[i] = f.imago;
        fx->classica[i] = dithering_exemplaria_colorum(f.imago.pixela,
            f.imago.latitudo, f.imago.altitudo, activi, NIHIL, NIHIL,
            c->piscina);
        CREDO_NECESSE(fx->classica[i].successus);
    }
    c->datum = fx;
}

interior constans FixuraVelox*
_fixura (
    CredoContextus* c)
{
    redde (constans FixuraVelox*)c->suita;
}

/* digestum hexadecimale (LXIV litterae + NUL) */
interior vacuum
_digestum (
    constans DitheringFructus* f,
                    character* hex)
{
     i8 d[XXXII];
    s32 k;

    blake2b(d, XXXII, f->indices, f->latitudo * f->altitudo, NIHIL, 0);
    per (k = 0; k < XXXII; k++) sprintf(hex + k * II, "%02x",
                                    (insignatus)d[k]);
    hex[LXIV] = '\0';
}

interior vacuum
_sectio_classica_fixa (
    CredoContextus* c)
{
    constans FixuraVelox* fx = _fixura(c);
               character  hex[LXV];
                     s32  i;

    per (i = 0; i < FIXA_NUMERUS; i++)
    {
        _digestum(&fx->classica[i], hex);
        CREDO_LITERAE_AEQUALES(hex, CLASSICA_DIGESTA[i]);
    }
}

interior vacuum
_sectio_colores_puri (
    CredoContextus* c)
{
          b32 activi[XVI];
    character titulus[XXXII];
    s32 i, p, errata;

    dithering_praeparare_omnes(activi);
    per (i = 0; i < XVI; i++)
    {
                      i8* plana = _imago_plana(c->piscina, XVI, XVI, i);
        DitheringFructus  f;
        f = dithering_exemplaria_velox_colorum(plana, XVI, XVI, activi,
            NIHIL, NIHIL, c->piscina);
        CREDO_NECESSE(f.successus);
        errata = 0;
        per (p = 0; p < XVI * XVI; p++) si ((s32)f.indices[p]
                                            != i) errata++;
        sprintf(titulus, "color %d", (integer)i);
        CREDO_LITERAE_AEQUALES(errata == 0 ? titulus : "fractus",
            titulus);
    }
}

#define PAUCI_NUMERUS III

/* greges colorum activorum: unus, duo, tres */
hic_manens constans s32 PAUCI_GREGES[PAUCI_NUMERUS][IV] = {
    {I, V, 0, 0}, {II, 0, V, 0}, {III, 0, V, VI}
};

interior vacuum
_sectio_pauci_colores (
    CredoContextus* c)
{
          b32 activi[XVI];
    character titulus[XXXII];
    s32 g, i, k, p, errata;

    per (g = 0; g < PAUCI_NUMERUS; g++)
    {
                     s32  numerus = PAUCI_GREGES[g][0];
                      i8* scaena;
        DitheringFructus  f;

        scaena = _imago_scaena(c->piscina, LXXX, LX);
        per (i = 0; i < XVI; i++) activi[i] = FALSUM;
        per (k = 0; k < numerus; k++) activi[PAUCI_GREGES[g][k + I]]
                                          = VERUM;
        f = dithering_exemplaria_velox_colorum(scaena, LXXX, LX, activi,
            NIHIL, NIHIL, c->piscina);
        CREDO_NECESSE(f.successus);
        CREDO_VERUM(_omnes_activi(&f, activi));

        /* colores puri intra gregem */
        per (k = 0; k < numerus; k++)
        {
            s32  color = PAUCI_GREGES[g][k + I];
             i8* plana = _imago_plana(c->piscina, XVI, XVI, color);
            f = dithering_exemplaria_velox_colorum(plana, XVI, XVI,
                activi, NIHIL, NIHIL, c->piscina);
            CREDO_NECESSE(f.successus);
            errata = 0;
            per (p = 0; p < XVI * XVI; p++) si ((s32)f.indices[p]
                                                != color) errata++;
            sprintf(titulus, "grex %d color %d", (integer)numerus,
                (integer)color);
            CREDO_LITERAE_AEQUALES(errata == 0 ? titulus : "fractus",
                titulus);
        }
    }
}

/* digestum velocis in fixo, piscina nova (memoria non cumulatur) */
interior b32
_velox_digestum (
    constans Imago* im,
         character* hex)
{
    Piscina* p = piscina_generare_dynamicum("velox_fixa",
        (memoriae_index)(LXIV * 1024 * 1024));
                 b32 activi[XVI];
    DitheringFructus f;

    si (p == NIHIL) redde FALSUM;
    dithering_praeparare_omnes(activi);
    f = dithering_exemplaria_velox_colorum(im->pixela, im->latitudo,
        im->altitudo, activi, NIHIL, NIHIL, p);
    si (f.successus) _digestum(&f, hex);
    piscina_destruere(p);
    redde f.successus;
}

interior vacuum
_sectio_velox_fixa (
    CredoContextus* c)
{
    constans FixuraVelox* fx = _fixura(c);
               character  hex[LXV];
                     s32  i;

    per (i = 0; i < FIXA_NUMERUS; i++)
    {
        CREDO_NECESSE(_velox_digestum(&fx->imagines[i], hex));
        CREDO_LITERAE_AEQUALES(hex, VELOX_DIGESTA[i]);
    }
}

interior vacuum
_sectio_magnitudines_oris (
    CredoContextus* c)
{
          b32 activi[XVI];
    character hex[LXV];
          s32 m;

    dithering_praeparare_griseum(activi);
    per (m = 0; m < ORAE_NUMERUS; m++)
    {
        s32 lat = ORAE_MAGNITUDINES[m][0], alt =
            ORAE_MAGNITUDINES[m][1];
                      i8* rgba = _imago_scaena(c->piscina, lat, alt);
        DitheringFructus  f = dithering_exemplaria_velox_colorum(rgba,
            (i32)lat, (i32)alt, activi, NIHIL, NIHIL, c->piscina);
        CREDO_NECESSE(f.successus);
        CREDO_VERUM(_omnes_activi(&f, activi));
        _digestum(&f, hex);
        CREDO_LITERAE_AEQUALES(hex, ORAE_DIGESTA[m]);
    }
}

/* usus arenae unius vocationis in piscina nova */
interior memoriae_index
_usus_vocationis (
    DitheringExemplariaFunctio  functio,
                constans Imago* im)
{
    Piscina* p = piscina_generare_dynamicum("memoria_minor",
        (memoriae_index)(LXIV * 1024 * 1024));
                 b32 activi[XVI];
    DitheringFructus f;
      memoriae_index usus = 0;

    si (p == NIHIL) redde 0;
    dithering_praeparare_omnes(activi);
    f = functio(im->pixela, im->latitudo, im->altitudo, activi, NIHIL,
        NIHIL, p);
    si (f.successus) usus = piscina_summa_usus(p);
    piscina_destruere(p);
    redde usus;
}

interior vacuum
_sectio_memoria_minor (
    CredoContextus* c)
{
    constans FixuraVelox* fx = _fixura(c);
          memoriae_index  classica, velox;

    classica = _usus_vocationis(dithering_exemplaria_colorum,
        &fx->imagines[0]);
    velox = _usus_vocationis(dithering_exemplaria_velox_colorum,
        &fx->imagines[0]);
    imprimere("  memoria lapidis DC: classica %lu MB, velox %lu MB\n",
        (insignatus longus)(classica >> XX),
        (insignatus longus)(velox >> XX));
    CREDO_NECESSE(classica > 0 && velox > 0);
    CREDO_VERUM(velox < classica);
}

interior vacuum
_sectio_alpha_neglecta (
    CredoContextus* c)
{
    b32  activi[XVI];
     i8* plena = _imago_scaena(c->piscina, XCVI, LXXII);
     i8* nulla = _imago_scaena(c->piscina, XCVI, LXXII);
    DitheringFructus a, b;
    s32 p;

    per (p = 0; p < XCVI * LXXII; p++) nulla[p * IV + III] = 0;
    dithering_praeparare_omnes(activi);
    a = dithering_exemplaria_velox_colorum(plena, XCVI, LXXII, activi,
        NIHIL, NIHIL, c->piscina);
    b = dithering_exemplaria_velox_colorum(nulla, XCVI, LXXII, activi,
        NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE(a.successus && b.successus);
    CREDO_OCTETI_AEQUALES(a.indices, b.indices, XCVI * LXXII);
}

interior vacuum
_sectio_sine_memoria (
    CredoContextus* c)
{
    b32  activi[XVI];
     i8* scaena  = _imago_scaena(c->piscina, XCVI, LXXII);
     i8* rubra   = _imago_gradiens_rubra(c->piscina, XCVI, LXIV);
    DitheringFructus a, m, b;

    dithering_praeparare_omnes(activi);
    a = dithering_exemplaria_velox_colorum(scaena, XCVI, LXXII, activi,
        NIHIL, NIHIL, c->piscina);
    m = dithering_exemplaria_velox_colorum(rubra, XCVI, LXIV, activi,
        NIHIL, NIHIL, c->piscina);
    b = dithering_exemplaria_velox_colorum(scaena, XCVI, LXXII, activi,
        NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE(a.successus && m.successus && b.successus);
    CREDO_OCTETI_AEQUALES(a.indices, b.indices, XCVI * LXXII);
}

hic_manens constans CredoSectio SECTIONES[] = {
    DITHERING_EXEMPLARIA_SECTIONES_COMMUNES
    { "classica fixa", _sectio_classica_fixa, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "colores puri", _sectio_colores_puri, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pauci colores", _sectio_pauci_colores, NIHIL, NIHIL, NIHIL,
        NIHIL },
    /* gravis: classicus ~CCL MB arenae per vocationem ad DC x DCCLI */
    { "memoria minor", _sectio_memoria_minor, NIHIL, NIHIL, "gravis",
        NIHIL },
    { "velox fixa", _sectio_velox_fixa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines oris", _sectio_magnitudines_oris, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "alpha neglecta", _sectio_alpha_neglecta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "sine memoria inter vocationes", _sectio_sine_memoria, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("dithering_exemplaria_velox",
        SECTIONES, _parare_suitae, NIHIL);
}
