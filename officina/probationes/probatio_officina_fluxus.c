/* probatio_officina_fluxus.c - analyses medullae (M5 T5, G24)
 *
 * Expectationes MANU computatae (tabulae infra). Positiones: 2k usus,
 * 2k+1 definitio, k = instructio in ordine dorsi.
 */

#include "postulata_posix.h"
#include "officina_medulla_fluxus.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXTRA_FINES 0xFFFFFFFF

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior chorda
_fixuram_legere (
               Piscina* piscina,
    constans character* via_partis)
{
             character  via[DXII];
    constans character* radix = getenv("RHUBARB_RADIX");

    via[0] = '\0';
    strncat(via, (radix != NIHIL) ? radix : ".", CCLV);
    strcat(via, "/");
    strncat(via, via_partis, CCLV);
    redde filum_legere_totum(via, piscina);
}

interior MedullaModulus*
_legere (
    Piscina* piscina,
     chorda  textus)
{
               i32  linea_erroris = ZEPHYRUM;
    MedullaModulus* modulus = medulla_textum_legere(piscina, textus,
        &linea_erroris);

    si (modulus == NIHIL)
    {
        imprimere("\n  FRACTA lectio (linea %u)", linea_erroris);
    }
    redde modulus;
}

interior constans MedullaFunctio*
_functio (
    constans MedullaModulus* modulus,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(modulus->functiones); i++)
    {
        MedullaFunctio* functio;

        functio = *(MedullaFunctio**)xar_obtinere(modulus->functiones,
            i);
        si (chorda_aequalis_literis(functio->titulus, titulus))
        {
            redde functio;
        }
    }
    redde NIHIL;
}

/* index registri per titulum; -I si ignotum */
interior s32
_registrum (
    constans MedullaFunctio* functio,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(functio->registra); i++)
    {
        si (chorda_aequalis_literis(*(constans chorda*)xar_obtinere(
                functio->registra, i), titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* VERUM si intervallum registri == [initium, finis] */
interior b32
_intervallum_est (
     constans MedullaFluxus* f,
    constans MedullaFunctio* functio,
         constans character* titulus,
                        i32  initium,
                        i32  finis)
{
    i32 a;
    i32 b;

    a = EXTRA_FINES;
    b = EXTRA_FINES;
    si (   !medulla_fluxus_intervallum(f, _registrum(functio, titulus),
            &a, &b) || a != initium || b != finis)
    {
        imprimere("\n  %%%s: exspectatum [%u, %u], datum [%u, %u]",
            titulus, initium, finis, a, b);
        redde FALSUM;
    }
    redde VERUM;
}


    /* ==================================================
     * PROBARE: praedecessores ($dum_probat, fluxus.medulla)
     *
     * Blocci: 0 initium_0 -> 1; 1 dum_proba_1 -> 2|3;
     * 2 dum_corpus_2 -> 4|5; 3 dum_finis_3 -> 8; 4 si_verum_4 -> 1;
     * 5 si_finis_5 -> 6|7; 6 si_verum_6 -> 3; 7 si_finis_7 -> 1;
     * 8 fac_corpus_8 -> 9; 9 fac_proba_9 -> 8|10; 10 redde.
     * ================================================== */

interior vacuum
_sectio_praedecessores (
    CredoContextus* c)
{
    /* -I terminat; ordine crescente */
    hic_manens constans s32 PRAEDECESSORES[XI][IV] = {
        { -I },
        { 0, IV, VII, -I },
        { I, -I },
        { I, VI, -I },
        { II, -I },
        { II, -I },
        { V, -I },
        { V, -I },
        { III, IX, -I },
        { VIII, -I },
        { IX, -I }
    };
             MedullaModulus* modulus;
    constans MedullaFunctio* functio;
              MedullaFluxus* f;
                        s32  b;

    modulus = _legere(c->piscina, _fixuram_legere(c->piscina,
        "officina/probationes/fixa/fluxus.medulla"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "dum_probat");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    per (b = ZEPHYRUM; b < XI; b++)
    {
        constans s32* indices;
                 s32  numerus;
                 s32  k;
                 s32  exspectati;

        exspectati = ZEPHYRUM;
        dum (PRAEDECESSORES[b][exspectati] >= ZEPHYRUM)
        {
            exspectati++;
        }
        indices = NIHIL;
        numerus = medulla_fluxus_praedecessores(f, b, &indices);
        CREDO_AEQUALIS_S32 (numerus, exspectati);
        si (numerus != exspectati)
        {
            imprimere("\n  bloccus %d", (integer)b);
            perge;
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            CREDO_AEQUALIS_S32 (indices[k], PRAEDECESSORES[b][k]);
        }
    }
}


    /* ==================================================
     * PROBARE: ordo (ansae contiguae; insequens absens)
     *
     * RPO (successores b ante c): 0 1 2 5 7 6 3 8 9 10 4.
     * Ansa {1,2,4,5,7} (caput 1) et {8,9} (caput 8) contiguae:
     * 0 | 1 2 5 7 4 | 6 3 | 8 9 | 10.
     * Instructiones per bloccum (Xar): 3 2 4 1 1 2 1 3 3 2 1 ->
     * indices plani in ordine dorsi: 0 1 2 | 3 4 | 5 6 7 8 |
     * 11 12 | 14 15 16 | 10 | 13 | 9 | 17 18 19 | 20 21 | 22.
     * ================================================== */

interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
    hic_manens constans s32 ORDO[XI] = {
        0, I, II, V, VII, IV, VI, III, VIII, IX, X
    };
    hic_manens constans s32 PROFUNDITAS[XI] = {
        0, I, I, 0, I, I, 0, I, I, I, 0
    };
             MedullaModulus* modulus;
    constans MedullaFunctio* functio;
              MedullaFluxus* f;
               constans s32* ordo;
                        s32  numerus;
                        s32  k;
                        i32  initium;
                        i32  finis;

    modulus = _legere(c->piscina, _fixuram_legere(c->piscina,
        "officina/probationes/fixa/fluxus.medulla"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "dum_probat");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    numerus  = ZEPHYRUM;
    ordo     = medulla_fluxus_ordo(f, &numerus);
    CREDO_NECESSE_NON_NIHIL (ordo);
    CREDO_AEQUALIS_S32 (numerus, XI);
    per (k = ZEPHYRUM; k < numerus && k < XI; k++)
    {
        CREDO_AEQUALIS_S32 (ordo[k], ORDO[k]);
        CREDO_AEQUALIS_S32 (medulla_fluxus_profunditas_ansae(f, k),
            PROFUNDITAS[k]);
        CREDO_VERUM (medulla_fluxus_attingibilis(f, k));
    }
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, 0), 0);
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, XVIII), XI);
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, XXIX), X);
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, XXXII), IX);
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, XLV), XXII);
    CREDO_AEQUALIS_I32 (medulla_fluxus_index_planus(f, XLVI),
        EXTRA_FINES);

    /* bloccus post redde non attingibilis: absens ab ordine */
    modulus = _legere(c->piscina, _ch(
        "modulus \"insequens.c\"\n"
        "\n"
        "functio $insequens (s32 %n) -> s32\n"
        "@initium_0:\n"
        "    redde %n\n"
        "@insequens_1:\n"
        "    %t1 = addere.s32 %n, 1\n"
        "    redde %t1\n"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "insequens");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    ordo = medulla_fluxus_ordo(f, &numerus);
    CREDO_AEQUALIS_S32 (numerus, I);
    CREDO_AEQUALIS_S32 (ordo[0], 0);
    CREDO_VERUM (medulla_fluxus_attingibilis(f, 0));
    CREDO_FALSUM (medulla_fluxus_attingibilis(f, I));
    CREDO_VERUM (_intervallum_est(f, functio, "n", 0, 0));
    CREDO_FALSUM (medulla_fluxus_intervallum(f,
        _registrum(functio, "t1"), &initium, &finis));
}


    /* ==================================================
     * PROBARE: iunctura (registrum in duobus bloccis scriptum)
     *
     * 0: ramus (0,1) | 1: movere (2,3) salire (4,5) |
     * 2: movere (6,7) salire (8,9) | 3: redde (10,11).
     * %t1 scribitur 3 et 7, legitur 10 -> [3, 10]. Si definitio
     * secunda neglegeretur, %t1 in bloccum 2 intraret et ab
     * initio viveret.
     * ================================================== */

interior vacuum
_sectio_iunctura (
    CredoContextus* c)
{
             MedullaModulus* modulus;
    constans MedullaFunctio* functio;
              MedullaFluxus* f;

    modulus = _legere(c->piscina, _ch(
        "modulus \"iunctura.c\"\n"
        "\n"
        "functio $iunctura (s32 %a) -> s32\n"
        "@initium_0:\n"
        "    ramus %a -> @verum_1 | @falsum_2\n"
        "@verum_1:\n"
        "    %t1 = movere.s32 1\n"
        "    salire @finis_3\n"
        "@falsum_2:\n"
        "    %t1 = movere.s32 2\n"
        "    salire @finis_3\n"
        "@finis_3:\n"
        "    redde %t1\n"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "iunctura");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    CREDO_VERUM (_intervallum_est(f, functio, "t1", III, X));
    CREDO_VERUM (_intervallum_est(f, functio, "a", 0, 0));
}


    /* ==================================================
     * PROBARE: usus sine definitione (F13: a positione 0)
     *
     * 0: ramus (0,1) | 1: movere (2,3) salire (4,5) |
     * 2: salire (6,7) | 3: redde (8,9). %u scribitur in 1 solum;
     * per bloccum 2 in initium vivit -> [0, 8].
     * ================================================== */

interior vacuum
_sectio_sine_definitione (
    CredoContextus* c)
{
             MedullaModulus* modulus;
    constans MedullaFunctio* functio;
              MedullaFluxus* f;

    modulus = _legere(c->piscina, _ch(
        "modulus \"sine.c\"\n"
        "\n"
        "functio $sine_definitione (s32 %n) -> s32\n"
        "@initium_0:\n"
        "    ramus %n -> @a_1 | @b_2\n"
        "@a_1:\n"
        "    %u = movere.s32 5\n"
        "    salire @c_3\n"
        "@b_2:\n"
        "    salire @c_3\n"
        "@c_3:\n"
        "    redde %u\n"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "sine_definitione");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    CREDO_VERUM (_intervallum_est(f, functio, "u", 0, VIII));
    CREDO_VERUM (_intervallum_est(f, functio, "n", 0, 0));
}


    /* ==================================================
     * PROBARE: vocationem transit
     *
     * addere (0,1) | vocare (2,3) | addere (4,5) | redde (6,7).
     * %b [0,2] (argumentum solum) FALSUM; %c [1,4] VERUM;
     * %d [3,4] (fructus vocationis) FALSUM.
     * ================================================== */

interior vacuum
_sectio_vocatio (
    CredoContextus* c)
{
             MedullaModulus* modulus;
    constans MedullaFunctio* functio;
              MedullaFluxus* f;

    modulus = _legere(c->piscina, _ch(
        "modulus \"vocat.c\"\n"
        "\n"
        "functio $vocat (s32 %a, s32 %b) -> s32\n"
        "@initium_0:\n"
        "    %c = addere.s32 %a, 1\n"
        "    %d = vocare.s32 $alia, s32 %b\n"
        "    %e = addere.s32 %c, %d\n"
        "    redde %e\n"));
    CREDO_NECESSE_NON_NIHIL (modulus);
    functio = _functio(modulus, "vocat");
    CREDO_NECESSE_NON_NIHIL (functio);
    f = medulla_fluxus_computare(c->piscina, functio);
    CREDO_NECESSE_NON_NIHIL (f);
    CREDO_VERUM (_intervallum_est(f, functio, "b", 0, II));
    CREDO_VERUM (_intervallum_est(f, functio, "c", I, IV));
    CREDO_VERUM (_intervallum_est(f, functio, "d", III, IV));
    CREDO_FALSUM (medulla_fluxus_transit_vocationem(f,
        _registrum(functio, "a")));
    CREDO_FALSUM (medulla_fluxus_transit_vocationem(f,
        _registrum(functio, "b")));
    CREDO_VERUM (medulla_fluxus_transit_vocationem(f,
        _registrum(functio, "c")));
    CREDO_FALSUM (medulla_fluxus_transit_vocationem(f,
        _registrum(functio, "d")));
    CREDO_FALSUM (medulla_fluxus_transit_vocationem(f,
        _registrum(functio, "e")));
}


    /* ==================================================
     * PROBARE: pactum (textus octetim idem post computare)
     * ================================================== */

interior vacuum
_sectio_pactum (
    CredoContextus* c)
{
    hic_manens constans character* constans FIXURAE[] = {
        "officina/probationes/fixa/fluxus.medulla",
        "officina/probationes/fixa/omnia.medulla",
        "officina/probationes/fixa/summa.medulla",
        "officina/probationes/fixa/aggregata.medulla"
    };
    i32 k;

    per (k = ZEPHYRUM; k < (i32)(magnitudo(FIXURAE)
             / magnitudo(FIXURAE[0])); k++)
    {
        MedullaModulus* modulus;
                chorda  ante;
                chorda  post;
                   i32  i;

        modulus = _legere(c->piscina, _fixuram_legere(c->piscina,
            FIXURAE[k]));
        CREDO_NECESSE_NON_NIHIL (modulus);
        ante = medulla_textum_scribere(c->piscina, modulus);
        per (i = ZEPHYRUM; i < xar_numerus(modulus->functiones); i++)
        {
            MedullaFunctio* functio;

            functio = *(MedullaFunctio**)xar_obtinere(
                modulus->functiones, i);
            CREDO_NON_NIHIL (medulla_fluxus_computare(c->piscina,
                functio));
        }
        post = medulla_textum_scribere(c->piscina, modulus);
        CREDO_CHORDA_AEQUALIS (post, ante);
    }
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "praedecessores", _sectio_praedecessores, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ordo", _sectio_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iunctura", _sectio_iunctura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "usus sine definitione", _sectio_sine_definitione, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vocationem transit", _sectio_vocatio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pactum", _sectio_pactum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_fluxus", SECTIONES);
}
