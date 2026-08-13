/*
 * probatio_imago_collatio.c - Probationes collationis imaginum
 *
 * Omnes imagines hic FINGUNTUR. Nulla plagula legitur, nulla
 * fenestra aperitur. Ideo nihil hic viride esse potest propter
 * ambitum: si praeterit, computatio ipsa praeterit.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "imago_collatio.h"
#include <stdio.h>

/* ============================================================
 * Auxilia: imagines fictae
 * ============================================================ */

interior Imago
_imago_solida (Piscina* piscina, i32 lat, i32 alt, i8 valor)
{
    Imago im;
    i32   i, numerus;

    numerus     = lat * alt;
    im.latitudo = lat;
    im.altitudo = alt;
    im.pixela   = (i8*)piscina_allocare(
                      piscina, (memoriae_index)(numerus * (i32)IV));

    si (im.pixela != NIHIL)
    {
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            im.pixela[i * (i32)IV + 0]   = valor;
            im.pixela[i * (i32)IV + I]   = valor;
            im.pixela[i * (i32)IV + II]  = valor;
            im.pixela[i * (i32)IV + III] = (i8)CCLV;
        }
    }

    redde im;
}

interior vacuum
_ponere (Imago* im, i32 x, i32 y, i8 valor)
{
    i32 sedes;

    sedes = (y * im->latitudo + x) * (i32)IV;
    im->pixela[sedes + 0]  = valor;
    im->pixela[sedes + I]  = valor;
    im->pixela[sedes + II] = valor;
}

interior i32
_legere (constans Imago* im, i32 x, i32 y, i32 canalis)
{
    redde (i32)im->pixela[(y * im->latitudo + x) * (i32)IV + canalis];
}

/* Columnas a..b valore implere */
interior vacuum
_columnas_implere (Imago* im, i32 a, i32 b, i8 valor)
{
    i32 x, y;

    per (y = ZEPHYRUM; y < im->altitudo; y++)
    {
        per (x = a; x <= b; x++)
        {
            _ponere(im, x, y, valor);
        }
    }
}

/* ============================================================ */

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_imago_collatio", M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. Imagines identicae congruunt
     * ======================================================== */

    imprimere("\n--- I. Imagines identicae ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)C);
        b = _imago_solida(piscina, X, X, (i8)C);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;  /* nucleum solum probare */

        fructus = imago_conferre(&a, &b, regula, NIHIL);

        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.pixela_tota, (i32)(X * X));
        CREDO_AEQUALIS_I32(fructus.delta_maximum, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.arca_latitudo, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.arca_altitudo, ZEPHYRUM);
        CREDO_FALSUM(fructus.pictura_praesto);
    }

    /* ========================================================
     * II. Unum pixelum mutatum: arca id NOMINAT
     * ======================================================== */

    imprimere("\n--- II. Unum pixelum mutatum ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        b = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        _ponere(&b, III, IV, (i8)CCLV);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;

        fructus = imago_conferre(&a, &b, regula, NIHIL);

        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)I);
        CREDO_AEQUALIS_I32(fructus.delta_maximum, (i32)CCLV);
        CREDO_AEQUALIS_I32(fructus.arca_x, (i32)III);
        CREDO_AEQUALIS_I32(fructus.arca_y, (i32)IV);
        CREDO_AEQUALIS_I32(fructus.arca_latitudo, (i32)I);
        CREDO_AEQUALIS_I32(fructus.arca_altitudo, (i32)I);
    }

    /* ========================================================
     * III. Tolerantia EXCEDITUR, non aequatur
     * ======================================================== */

    imprimere("\n--- III. Tolerantia (excedit, non aequat) ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, IV, IV, (i8)C);
        b = _imago_solida(piscina, IV, IV, (i8)C);
        _ponere(&b, I, I, (i8)(C + V));   /* delta = V */

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;

        /* tolerantia V: delta V non EXCEDIT, ergo congruunt */
        regula.tolerantia = (i32)V;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, ZEPHYRUM);
        /* mensura nuda superest quamquam iudicium mundum est */
        CREDO_AEQUALIS_I32(fructus.delta_maximum, (i32)V);

        /* tolerantia IV: delta V excedit */
        regula.tolerantia = (i32)IV;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)I);
    }

    /* ========================================================
     * IV. Mensurae discrepantes: sententia propria, non culpa
     * ======================================================== */

    imprimere("\n--- IV. Mensurae discrepantes ---\n");
    {
        Imago           a, b;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)C);
        b = _imago_solida(piscina, X, VIII, (i8)C);

        fructus = imago_conferre(&a, &b, collatio_regula_solita(), NIHIL);

        CREDO_AEQUALIS_S32((s32)fructus.sententia,
                           (s32)COLLATIO_MENSURAE_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_tota, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, ZEPHYRUM);
    }

    /* ========================================================
     * V. Argumenta invalida
     * ======================================================== */

    imprimere("\n--- V. Argumenta invalida ---\n");
    {
        Imago           a, vacua;
        CollatioFructus fructus;

        a = _imago_solida(piscina, IV, IV, (i8)C);

        fructus = imago_conferre(NIHIL, &a, collatio_regula_solita(), NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia,
                           (s32)COLLATIO_ARGUMENTA_INVALIDA);

        fructus = imago_conferre(&a, NIHIL, collatio_regula_solita(), NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia,
                           (s32)COLLATIO_ARGUMENTA_INVALIDA);

        vacua.pixela   = NIHIL;
        vacua.latitudo = IV;
        vacua.altitudo = IV;
        fructus = imago_conferre(&vacua, &a, collatio_regula_solita(), NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia,
                           (s32)COLLATIO_ARGUMENTA_INVALIDA);
    }

    /* ========================================================
     * VI. Arca pixela dispersa amplectitur
     * ======================================================== */

    imprimere("\n--- VI. Arca mutationis ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        b = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        _ponere(&b, II,  III,  (i8)CCLV);
        _ponere(&b, VII, VIII, (i8)CCLV);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;

        fructus = imago_conferre(&a, &b, regula, NIHIL);

        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)II);
        CREDO_AEQUALIS_I32(fructus.arca_x, (i32)II);
        CREDO_AEQUALIS_I32(fructus.arca_y, (i32)III);
        CREDO_AEQUALIS_I32(fructus.arca_latitudo, (i32)VI);
        CREDO_AEQUALIS_I32(fructus.arca_altitudo, (i32)VI);
    }

    /* ========================================================
     * VII. Pictura differentiae
     * ======================================================== */

    imprimere("\n--- VII. Pictura ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, IV, IV, (i8)C);
        b = _imago_solida(piscina, IV, IV, (i8)C);
        _ponere(&b, I, I, (i8)CC);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;
        regula.picturam_facere   = VERUM;

        fructus = imago_conferre(&a, &b, regula, piscina);

        CREDO_VERUM(fructus.pictura_praesto);
        CREDO_NON_NIHIL(fructus.pictura.pixela);
        CREDO_AEQUALIS_I32(fructus.pictura.latitudo, (i32)IV);
        CREDO_AEQUALIS_I32(fructus.pictura.altitudo, (i32)IV);

        /* Pixelum diversum: rubrum plenum */
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, I, I, 0),     (i32)CCLV);
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, I, I, (i32)I),   ZEPHYRUM);
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, I, I, (i32)II),  ZEPHYRUM);
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, I, I, (i32)III), (i32)CCLV);

        /* Pixelum congruens: canum, pallidum, opacum */
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, 0, 0, 0),
                           _legere(&fructus.pictura, 0, 0, (i32)I));
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, 0, 0, (i32)I),
                           _legere(&fructus.pictura, 0, 0, (i32)II));
        CREDO_AEQUALIS_I32(_legere(&fructus.pictura, 0, 0, (i32)III), (i32)CCLV);
        /* in album fugit: clarius quam fons (C) */
        CREDO_MAIOR_I32(_legere(&fructus.pictura, 0, 0, 0), (i32)CC);

        /* Sine piscina pictura non fit, collatio tamen valet */
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_FALSUM(fructus.pictura_praesto);
        CREDO_NIHIL(fructus.pictura.pixela);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)I);
    }

    /* ========================================================
     * VIII. Lenimentum: ora spernitur
     *
     * Eaedem imagines, duae regulae, contraria iudicia. Hoc par
     * probat regulam vere operari - non fortuito praeterire.
     * ======================================================== */

    imprimere("\n--- VIII. Lenimentum: ora spernitur ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;
        i32             y;

        /* Terminus verticalis: nigrum | album */
        a = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        _columnas_implere(&a, (i32)V, (i32)IX, (i8)CCLV);

        b = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        _columnas_implere(&b, (i32)V, (i32)IX, (i8)CCLV);

        /* Columna V leniata fit - sed in versibus INTERIORIBUS solis */
        per (y = (i32)I; y <= (i32)VIII; y++)
        {
            _ponere(&b, (i32)V, y, (i8)CXXVIII);
        }

        regula = collatio_regula_solita();

        /* Cum lenimento spreto: nulla mutatio */
        regula.sperne_lenimentum = VERUM;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_CONGRUUNT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.pixela_leniata, (i32)VIII);
        CREDO_AEQUALIS_I32(fructus.delta_maximum, (i32)CXXVII);

        /* Sine lenimento spreto: eadem pixela clamant */
        regula.sperne_lenimentum = FALSUM;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)VIII);
        CREDO_AEQUALIS_I32(fructus.pixela_leniata, ZEPHYRUM);
    }

    /* ========================================================
     * IX. Lenimentum mutationem VERAM non devorat
     *
     * Custos contrarius: sine hoc, regula lenimenti probationem
     * viridem in perpetuum tenere posset.
     * ======================================================== */

    imprimere("\n--- IX. Mutatio vera in campo plano ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)CCLV);
        b = _imago_solida(piscina, X, X, (i8)CCLV);
        _ponere(&b, (i32)V, (i32)V, (i8)ZEPHYRUM);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = VERUM;

        fructus = imago_conferre(&a, &b, regula, NIHIL);

        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)I);
        CREDO_AEQUALIS_I32(fructus.pixela_leniata, ZEPHYRUM);
        CREDO_AEQUALIS_I32(fructus.arca_x, (i32)V);
        CREDO_AEQUALIS_I32(fructus.arca_y, (i32)V);
    }

    /* ========================================================
     * X. Limes numeri (pixela_ferenda)
     *
     * Tolerantia de pixelo UNO iudicat, hic limes de NUMERO.
     * Necessarius est quia spretio lenimenti motum subpixelarem
     * non totum tollit (MENSURATUM: CCXI pixela superfuerunt) et
     * tolerantia tanta mutationes veras quoque devoraret.
     * ======================================================== */

    imprimere("\n--- X. Limes numeri ---\n");
    {
        Imago           a, b;
        CollatioRegula  regula;
        CollatioFructus fructus;

        a = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        b = _imago_solida(piscina, X, X, (i8)ZEPHYRUM);
        _ponere(&b, II,  III,  (i8)CCLV);
        _ponere(&b, VII, VIII, (i8)CCLV);

        regula = collatio_regula_solita();
        regula.sperne_lenimentum = FALSUM;

        /* Limes ZEPHYRUM: mos prior, nihil fertur */
        CREDO_AEQUALIS_I32(regula.pixela_ferenda, ZEPHYRUM);
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);

        /* Limes II: duo pixela non EXCEDUNT duo */
        regula.pixela_ferenda = (i32)II;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_CONGRUUNT);

        /* MENSURA INTACTA MANET quamquam iudicium mite est - hoc
         * cardo est: limes altior probationem caecam faceret si
         * numeros quoque tolleret */
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)II);
        CREDO_AEQUALIS_I32(fructus.delta_maximum, (i32)CCLV);
        CREDO_AEQUALIS_I32(fructus.arca_x, (i32)II);
        CREDO_AEQUALIS_I32(fructus.arca_y, (i32)III);
        CREDO_AEQUALIS_I32(fructus.arca_latitudo, (i32)VI);
        CREDO_AEQUALIS_I32(fructus.arca_altitudo, (i32)VI);

        /* Limes I: duo pixela unum EXCEDUNT */
        regula.pixela_ferenda = (i32)I;
        fructus = imago_conferre(&a, &b, regula, NIHIL);
        CREDO_AEQUALIS_S32((s32)fructus.sententia, (s32)COLLATIO_DISCREPANT);
        CREDO_AEQUALIS_I32(fructus.pixela_diversa, (i32)II);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
