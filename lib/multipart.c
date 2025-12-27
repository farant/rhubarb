/* multipart.c - Multipart Form Data Builder
 *
 * Implementatio multipart/form-data (RFC 2046).
 */

#include "multipart.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"

#include <string.h>
#include <time.h>


/* ========================================================================
 * CONSTANTES
 * ======================================================================== */

#define MULTIPART_MAX_CAMPI XXXII
#define BOUNDARY_LONGITUDO  XXXII


/* ========================================================================
 * TYPI INTERNI
 * ======================================================================== */

nomen enumeratio {
    CAMPUS_TEXTUM,
    CAMPUS_FASCICULUM
} CampusTipus;

nomen structura {
    CampusTipus tipus;
    chorda      nomen_campi;
    chorda      valor;              /* Pro textum */
    chorda      nomen_fasciculi;    /* Pro fasciculum */
    chorda      content_type;       /* Pro fasciculum */
    constans i8* datum;             /* Pro fasciculum */
    i32         mensura;            /* Pro fasciculum */
} MultipartCampus;

structura MultipartAedificator {
    Piscina*        piscina;
    chorda          boundary;
    MultipartCampus campi[MULTIPART_MAX_CAMPI];
    i32             numerus_campi;
};


/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

/* Generare boundary aleatorium */
interior chorda
generare_boundary(Piscina* piscina)
{
    hic_manens constans character* characteres =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    chorda resultatus;
    i8*    buffer;
    i32    i;
    i32    seed;

    buffer = piscina_allocare(piscina, BOUNDARY_LONGITUDO);

    /* Simplex random seed */
    seed = (i32)time(NIHIL);

    per (i = 0; i < BOUNDARY_LONGITUDO; i++)
    {
        seed = seed * MDCCLXXVI + I;
        buffer[i] = (i8)characteres[(seed >> XVI) % LXII];
    }

    resultatus.mensura = BOUNDARY_LONGITUDO;
    resultatus.datum = buffer;
    redde resultatus;
}

/* Addere chorda ad buffer */
interior vacuum
addere_chorda(ChordaAedificator* aed, chorda s)
{
    chorda_aedificator_appendere_chorda(aed, s);
}

/* Addere C string ad buffer */
interior vacuum
addere_cstr(ChordaAedificator* aed, constans character* s)
{
    chorda_aedificator_appendere_literis(aed, s);
}

/* Addere CRLF */
interior vacuum
addere_crlf(ChordaAedificator* aed)
{
    chorda_aedificator_appendere_character(aed, '\r');
    chorda_aedificator_appendere_character(aed, '\n');
}

/* Addere data binaria */
interior vacuum
addere_data(ChordaAedificator* aed, constans i8* datum, i32 mensura)
{
    i32 i;

    per (i = 0; i < mensura; i++)
    {
        chorda_aedificator_appendere_character(aed, (character)datum[i]);
    }
}


/* ========================================================================
 * FUNCTIONES - CREARE
 * ======================================================================== */

MultipartAedificator*
multipart_creare(Piscina* piscina)
{
    MultipartAedificator* aed;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    aed = piscina_allocare(piscina, (i32)magnitudo(MultipartAedificator));
    aed->piscina = piscina;
    aed->boundary = generare_boundary(piscina);
    aed->numerus_campi = 0;

    redde aed;
}


/* ========================================================================
 * FUNCTIONES - ADDERE CAMPOS
 * ======================================================================== */

vacuum
multipart_addere_textum(
    MultipartAedificator* aed,
    constans character*   nomen_campi,
    chorda                valor)
{
    MultipartCampus* campus;

    si (aed == NIHIL || nomen_campi == NIHIL)
    {
        redde;
    }

    si (aed->numerus_campi >= MULTIPART_MAX_CAMPI)
    {
        redde;
    }

    campus = &aed->campi[aed->numerus_campi++];
    campus->tipus = CAMPUS_TEXTUM;
    campus->nomen_campi = chorda_ex_literis(nomen_campi, aed->piscina);
    campus->valor = valor;
    campus->nomen_fasciculi.datum = NIHIL;
    campus->nomen_fasciculi.mensura = 0;
    campus->content_type.datum = NIHIL;
    campus->content_type.mensura = 0;
    campus->datum = NIHIL;
    campus->mensura = 0;
}

vacuum
multipart_addere_fasciculum(
    MultipartAedificator* aed,
    constans character*   nomen_campi,
    constans character*   nomen_fasciculi,
    constans character*   content_type,
    constans i8*          datum,
    i32                   mensura)
{
    MultipartCampus* campus;

    si (aed == NIHIL || nomen_campi == NIHIL || nomen_fasciculi == NIHIL)
    {
        redde;
    }

    si (aed->numerus_campi >= MULTIPART_MAX_CAMPI)
    {
        redde;
    }

    campus = &aed->campi[aed->numerus_campi++];
    campus->tipus = CAMPUS_FASCICULUM;
    campus->nomen_campi = chorda_ex_literis(nomen_campi, aed->piscina);
    campus->valor.datum = NIHIL;
    campus->valor.mensura = 0;
    campus->nomen_fasciculi = chorda_ex_literis(nomen_fasciculi, aed->piscina);

    si (content_type != NIHIL)
    {
        campus->content_type = chorda_ex_literis(content_type, aed->piscina);
    }
    alioquin
    {
        campus->content_type = chorda_ex_literis("application/octet-stream",
                                                  aed->piscina);
    }

    campus->datum = datum;
    campus->mensura = mensura;
}


/* ========================================================================
 * FUNCTIONES - FINIRE
 * ======================================================================== */

chorda
multipart_content_type(MultipartAedificator* aed)
{
    ChordaAedificator* builder;
    chorda             resultatus;

    si (aed == NIHIL)
    {
        resultatus.datum = NIHIL;
        resultatus.mensura = 0;
        redde resultatus;
    }

    builder = chorda_aedificator_creare(aed->piscina, CXXVIII);

    addere_cstr(builder, "multipart/form-data; boundary=");
    addere_chorda(builder, aed->boundary);

    redde chorda_aedificator_finire(builder);
}

chorda
multipart_corpus_finire(MultipartAedificator* aed)
{
    ChordaAedificator*  builder;
    MultipartCampus*    campus;
    i32                 i;

    si (aed == NIHIL)
    {
        chorda vacua;
        vacua.mensura = 0;
        vacua.datum = NIHIL;
        redde vacua;
    }

    builder = chorda_aedificator_creare(aed->piscina, M * IV);

    per (i = 0; i < aed->numerus_campi; i++)
    {
        campus = &aed->campi[i];

        /* Boundary line */
        addere_cstr(builder, "--");
        addere_chorda(builder, aed->boundary);
        addere_crlf(builder);

        si (campus->tipus == CAMPUS_TEXTUM)
        {
            /* Content-Disposition pro textum */
            addere_cstr(builder, "Content-Disposition: form-data; name=\"");
            addere_chorda(builder, campus->nomen_campi);
            addere_cstr(builder, "\"");
            addere_crlf(builder);
            addere_crlf(builder);

            /* Valor */
            addere_chorda(builder, campus->valor);
            addere_crlf(builder);
        }
        alioquin
        {
            /* Content-Disposition pro fasciculum */
            addere_cstr(builder, "Content-Disposition: form-data; name=\"");
            addere_chorda(builder, campus->nomen_campi);
            addere_cstr(builder, "\"; filename=\"");
            addere_chorda(builder, campus->nomen_fasciculi);
            addere_cstr(builder, "\"");
            addere_crlf(builder);

            /* Content-Type */
            addere_cstr(builder, "Content-Type: ");
            addere_chorda(builder, campus->content_type);
            addere_crlf(builder);
            addere_crlf(builder);

            /* Data */
            addere_data(builder, campus->datum, campus->mensura);
            addere_crlf(builder);
        }
    }

    /* Closing boundary */
    addere_cstr(builder, "--");
    addere_chorda(builder, aed->boundary);
    addere_cstr(builder, "--");
    addere_crlf(builder);

    redde chorda_aedificator_finire(builder);
}
