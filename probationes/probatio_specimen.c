/*
 * probatio_specimen.c - Probationes regressionis visualis
 *
 * NUCLEUS SOLUS hic probatur (specimen_iudicare): imagines
 * finguntur, nulla applicatio aperitur, nulla fenestra. Stratum
 * specimen_manus probatione fumi tegitur - simulacrum applicationis
 * fingere probaret fictum, non rem.
 *
 * SCRINIUM = /tmp, quia sine POSIX directorium creare non possumus
 * et /tmp semper exstat. Tituli praefixum unicum ferunt.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "specimen.h"
#include "imago.h"
#include "imago_png.h"
#include <stdio.h>
#include <string.h>

#define SCRINIUM "/tmp"


/* ============================================================
 * Auxilia
 * ============================================================ */

interior Imago
_imago_ficta (
    Piscina* piscina,
        i32  lat,
        i32  alt,
         i8  valor)
{
    Imago im;
    i32   i, numerus;

    numerus      = lat * alt;
    im.latitudo  = lat;
    im.altitudo  = alt;
    im.pixela   = (i8*)piscina_allocare(
                      piscina, (memoriae_index)(numerus * (i32)IV));

    si (im.pixela != NIHIL)
    {
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            im.pixela[i * (i32)IV + 0]    = valor;
            im.pixela[i * (i32)IV + I]    = valor;
            im.pixela[i * (i32)IV + II]   = valor;
            im.pixela[i * (i32)IV + III]  = (i8)CCLV;
        }
    }

    redde im;
}

interior b32
_exstat (
    constans character* via)
{
    FILE* f;

    f = fopen(via, "rb");
    si (f != NIHIL)
    {
        fclose(f);
        redde VERUM;
    }
    redde FALSUM;
}

/* Omnia artificia tituli tollere - probatio ITERUM currere debet */
interior vacuum
_purgare (
    constans character* titulus)
{
    character tabula[CCLVI];

    sprintf(tabula, "%s/%s.png", SCRINIUM, titulus);
    remove(tabula);
    sprintf(tabula, "%s/%s.novum.png", SCRINIUM, titulus);
    remove(tabula);
    sprintf(tabula, "%s/%s.differentia.png", SCRINIUM, titulus);
    remove(tabula);
}

interior vacuum
_viam (
                character* tabula,
       constans character* titulus,
       constans character* suffixum)
{
    sprintf(tabula, "%s/%s%s", SCRINIUM, titulus, suffixum);
}


/* ============================================================ */

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_specimen",
        M * M * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * I. Titulus semitam continere non potest
     * ======================================================== */

    imprimere("\n--- I. Titulus invalidus ---\n");
    {
                  Imago captura;
        SpecimenFructus f;
         SpecimenRegula regula;

        captura  = _imago_ficta(piscina, VIII, VIII, (i8)C);
        regula   = specimen_regula_solita(SCRINIUM);

        f = specimen_iudicare(&captura, "supra/infra", regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia,
            (s32)SPECIMEN_TITULUS_INVALIDUS);
        CREDO_CHORDA_NON_VACUA(f.causa);

        f = specimen_iudicare(&captura, "..", regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia,
            (s32)SPECIMEN_TITULUS_INVALIDUS);

        f = specimen_iudicare(&captura, "", regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia,
            (s32)SPECIMEN_TITULUS_INVALIDUS);
    }


    /* ========================================================
     * II. Exemplar absens FRANGIT (numquam tacite creat)
     * ======================================================== */

    imprimere("\n--- II. Exemplar abest ---\n");
    {
                     Imago  captura;
           SpecimenFructus  f;
            SpecimenRegula  regula;
                 character  via[CCLVI];
        constans character* titulus = "probatio_specimen_absens";

        _purgare(titulus);

        captura  = _imago_ficta(piscina, VIII, VIII, (i8)C);
        regula   = specimen_regula_solita(SCRINIUM);

        f = specimen_iudicare(&captura, titulus, regula, piscina);

        CREDO_AEQUALIS_S32((s32)f.sententia,
            (s32)SPECIMEN_EXEMPLAR_ABEST);
        CREDO_CHORDA_NON_VACUA(f.causa);
        CREDO_CHORDA_NON_VACUA(f.via_candidati);

        /* Candidatus SCRIPTUS est - id ritum acceptionis possibilem facit */
        _viam(via, titulus, ".novum.png");
        CREDO_VERUM(_exstat(via));

        /* Exemplar NON creatum est: probatio iterum frangere debet */
        _viam(via, titulus, ".png");
        CREDO_FALSUM(_exstat(via));

        _purgare(titulus);
    }


    /* ========================================================
     * III. Ritus acceptionis: motus plagulae SOLUS
     * ======================================================== */

    imprimere("\n--- III. Acceptio per motum ---\n");
    {
                  Imago captura;
        SpecimenFructus f;
         SpecimenRegula regula;
        character       candidatus[CCLVI], exemplar[CCLVI];
        constans character* titulus = "probatio_specimen_acceptio";

        _purgare(titulus);

        captura  = _imago_ficta(piscina, VIII, VIII, (i8)C);
        regula   = specimen_regula_solita(SCRINIUM);

        f = specimen_iudicare(&captura, titulus, regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia,
            (s32)SPECIMEN_EXEMPLAR_ABEST);

        /* ACCEPTIO: candidatum super exemplar movere. Nihil aliud. */
        _viam(candidatus, titulus, ".novum.png");
        _viam(exemplar, titulus, ".png");
        CREDO_AEQUALIS_S32((s32)rename(candidatus, exemplar),
            (s32)ZEPHYRUM);

        /* Eadem captura nunc CONGRUIT */
        f = specimen_iudicare(&captura, titulus, regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia, (s32)SPECIMEN_CONGRUIT);
        CREDO_VERUM(f.collatio_facta);
        CREDO_AEQUALIS_I32(f.collatio.pixela_diversa, ZEPHYRUM);

        _purgare(titulus);
    }


    /* ========================================================
     * IV. Mensurae discrepantes: sententia PROPRIA
     * ======================================================== */

    imprimere("\n--- IV. Mensurae discrepantes ---\n");
    {
        Imago           parva, magna;
        SpecimenFructus f;
         SpecimenRegula regula;
        character       candidatus[CCLVI], exemplar[CCLVI];
        constans character* titulus = "probatio_specimen_mensurae";

        _purgare(titulus);

        parva   = _imago_ficta(piscina, VIII, VIII, (i8)C);
        magna   = _imago_ficta(piscina, X, X, (i8)C);
        regula  = specimen_regula_solita(SCRINIUM);

        f = specimen_iudicare(&parva, titulus, regula, piscina);
        _viam(candidatus, titulus, ".novum.png");
        _viam(exemplar, titulus, ".png");
        rename(candidatus, exemplar);

        f = specimen_iudicare(&magna, titulus, regula, piscina);
        CREDO_AEQUALIS_S32((s32)f.sententia,
                           (s32)SPECIMEN_MENSURAE_DISCREPANT);
        CREDO_CHORDA_NON_VACUA(f.causa);
        /* Collatio NON facta est - conferri enim non possunt */
        CREDO_FALSUM(f.collatio_facta);

        _purgare(titulus);
    }


    /* ========================================================
     * V. Discrepantia: candidatus ET pictura scribuntur
     * ======================================================== */

    imprimere("\n--- V. Discrepantia ---\n");
    {
        Imago           exemplar_imago, mutata;
        SpecimenFructus f;
         SpecimenRegula regula;
        character       candidatus[CCLVI], exemplar[CCLVI],
            differentia[CCLVI];
        constans character* titulus = "probatio_specimen_discrepat";

        _purgare(titulus);

        exemplar_imago = _imago_ficta(piscina, X, X, (i8)C);
        regula = specimen_regula_solita(SCRINIUM);
        regula.collatio.sperne_lenimentum = FALSUM;

        f = specimen_iudicare(&exemplar_imago, titulus, regula,
            piscina);
        _viam(candidatus, titulus, ".novum.png");
        _viam(exemplar, titulus, ".png");
        _viam(differentia, titulus, ".differentia.png");
        rename(candidatus, exemplar);

        /* Unum pixelum mutare */
        mutata = _imago_ficta(piscina, X, X, (i8)C);
        mutata.pixela[(III * (i32)X + II) * (i32)IV] = (i8)CCLV;

        f = specimen_iudicare(&mutata, titulus, regula, piscina);

        CREDO_AEQUALIS_S32((s32)f.sententia, (s32)SPECIMEN_DISCREPAT);
        CREDO_VERUM(f.collatio_facta);
        CREDO_AEQUALIS_I32(f.collatio.pixela_diversa, (i32)I);
        CREDO_AEQUALIS_I32(f.collatio.arca_x, (i32)II);
        CREDO_AEQUALIS_I32(f.collatio.arca_y, (i32)III);

        /* AMBAE plagulae scriptae: sine pictura fractura inutilis est */
        CREDO_VERUM(_exstat(candidatus));
        CREDO_VERUM(_exstat(differentia));
        CREDO_CHORDA_NON_VACUA(f.via_candidati);
        CREDO_CHORDA_NON_VACUA(f.via_differentiae);


        /* ====================================================
         * VI. Limes: sententia mitis TOLLIT artificia vetera
         *
         * Aliter probatio SANATA indicem fracturarum in `git
         * status` in perpetuum relinqueret.
         * ==================================================== */

        imprimere("\n--- VI. Limes tollit artificia ---\n");

        regula.collatio.pixela_ferenda = (i32)I;
        f = specimen_iudicare(&mutata, titulus, regula, piscina);

        CREDO_AEQUALIS_S32((s32)f.sententia, (s32)SPECIMEN_CONGRUIT);
        /* MENSURA INTACTA quamquam iudicium mite est */
        CREDO_AEQUALIS_I32(f.collatio.pixela_diversa, (i32)I);
        CREDO_FALSUM(_exstat(candidatus));
        CREDO_FALSUM(_exstat(differentia));

        _purgare(titulus);
    }


    /* ========================================================
     * VII. Scrinium inaccessum
     * ======================================================== */

    imprimere("\n--- VII. Scrinium inaccessum ---\n");
    {
                  Imago captura;
        SpecimenFructus f;
         SpecimenRegula regula;

        captura  = _imago_ficta(piscina, VIII, VIII, (i8)C);
        regula   = specimen_regula_solita("/nusquam/omnino/scrinium");

        f = specimen_iudicare(&captura, "probatio_specimen_nusquam",
                              regula, piscina);

        CREDO_AEQUALIS_S32((s32)f.sententia,
                           (s32)SPECIMEN_SCRINIUM_INACCESSUM);
        CREDO_CHORDA_NON_VACUA(f.causa);
    }


    /* ========================================================
     * VIII. Nomina sententiarum
     * ======================================================== */

    imprimere("\n--- VIII. Nomina sententiarum ---\n");
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_literis(
                specimen_sententia_nomen(SPECIMEN_CONGRUIT), piscina),
            "SPECIMEN_CONGRUIT");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_literis(
                specimen_sententia_nomen(SPECIMEN_EXEMPLAR_ABEST),
                piscina),
            "SPECIMEN_EXEMPLAR_ABEST");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_literis(
                specimen_sententia_nomen(SPECIMEN_MENSURAE_DISCREPANT),
                piscina),
            "SPECIMEN_MENSURAE_DISCREPANT");
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
