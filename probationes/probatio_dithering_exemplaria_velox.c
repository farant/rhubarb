/* probatio_dithering_exemplaria_velox.c - modus per exemplaria, velox
 *
 * Sectiones communes (probationes/dithering_exemplaria_commune.h)
 * super functionem velocem, et praeterea in fixis
 * probationes/fixa/exemplaria/ (golem, pratum ad DC latitudinis):
 *   classica fixa     indices classici (optiones ordinariae, colores
 *                     omnes) per blake2b fixi - classicus numquam
 *                     mutatur (spec decretum 2)
 *   par classicae     velox == classicus octetim (dum copia est;
 *                     plan T3 eam removet, cum f32 differre incipit)
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
_sectio_par_classicae (
    CredoContextus* c)
{
    constans FixuraVelox* fx = _fixura(c);
                     b32  activi[XVI];
                     s32  i;

    dithering_praeparare_omnes(activi);
    per (i = 0; i < FIXA_NUMERUS; i++)
    {
          constans Imago* im = &fx->imagines[i];
        DitheringFructus  f = dithering_exemplaria_velox_colorum(
            im->pixela, im->latitudo, im->altitudo, activi, NIHIL,
            NIHIL,
            c->piscina);
        CREDO_NECESSE(f.successus);
        CREDO_OCTETI_AEQUALES(f.indices, fx->classica[i].indices,
            im->latitudo * im->altitudo);
    }
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
    /* gravis: classicus (et copia) ~CCL MB arenae per vocationem ad
     * DC x DCCLI; velox T3 id minuere debet ("memoria minor") */
    { "par classicae", _sectio_par_classicae, NIHIL, NIHIL, "gravis",
        NIHIL },
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
