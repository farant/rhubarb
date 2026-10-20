/* probatio_multipart.c - Probationes pro Multipart bibliotheca
 *
 * Probationes constructionis multipart/form-data.
 */

#include "multipart.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - CREARE
 * ======================================================================== */

interior vacuum
probatio_creare(Piscina* piscina)
{
    MultipartAedificator* aed;

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    /* Cum NIHIL piscina */
    aed = multipart_creare(NIHIL);
    CREDO_NIHIL(aed);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONTENT TYPE
 * ======================================================================== */

interior vacuum
probatio_content_type(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  ct;

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    ct = multipart_content_type(aed);
    CREDO_NON_NIHIL(ct.datum);
    CREDO_VERUM(ct.mensura > 0);

    /* Verificare continet "multipart/form-data" */
    CREDO_VERUM(ct.mensura > XXX);

    printf("  Content-Type: %.*s\n", ct.mensura, ct.datum);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - TEXTUM SIMPLEX
 * ======================================================================== */

interior vacuum
probatio_textum_simplex(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_textum(aed, "name", chorda_ex_literis("John",
        piscina));

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);
    CREDO_VERUM(corpus.mensura > 0);

    printf("  Corpus longitudo: %d bytes\n", corpus.mensura);

    /* Verificare continet "name" et "John" */
    CREDO_VERUM(corpus.mensura > L);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MULTIPLES CAMPI
 * ======================================================================== */

interior vacuum
probatio_multiples_campi(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_textum(aed, "model",
                            chorda_ex_literis("whisper-1", piscina));
    multipart_addere_textum(aed, "language",
                            chorda_ex_literis("en", piscina));
    multipart_addere_textum(aed, "response_format",
                            chorda_ex_literis("json", piscina));

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);
    CREDO_VERUM(corpus.mensura > C);

    printf("  Corpus longitudo: %d bytes\n", corpus.mensura);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - FASCICULUM
 * ======================================================================== */

interior vacuum
probatio_fasciculum(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  corpus;
             constans i8  data[] = {0x01, 0x02, 0x03, 0x04,
                 0x05};

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_fasciculum(aed, "file", "test.bin",
                                 "application/octet-stream", data, V);

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);
    CREDO_VERUM(corpus.mensura > 0);

    printf("  Corpus longitudo: %d bytes\n", corpus.mensura);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - MIXTUM (TEXTUM + FASCICULUM)
 * ======================================================================== */

interior vacuum
probatio_mixtum(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  corpus;
                  chorda  ct;
             constans i8  audio_data[] =
                 "fake audio data for testing";

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    /* Text fields */
    multipart_addere_textum(aed, "model",
                            chorda_ex_literis("whisper-1", piscina));
    multipart_addere_textum(aed, "language",
                            chorda_ex_literis("en", piscina));

    /* File field */
    multipart_addere_fasciculum(aed, "file", "recording.mp3",
                                 "audio/mpeg",
                                 audio_data, XXVIII);

    ct = multipart_content_type(aed);
    CREDO_NON_NIHIL(ct.datum);
    printf("  Content-Type: %.*s\n", ct.mensura, ct.datum);

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);
    CREDO_VERUM(corpus.mensura > CC);

    printf("  Corpus longitudo: %d bytes\n", corpus.mensura);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - NULLUM ARGUMENTA
 * ======================================================================== */

interior vacuum
probatio_nullum_argumenta(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  ct;
                  chorda  corpus;

    /* Content type cum NIHIL */
    ct = multipart_content_type(NIHIL);
    CREDO_NIHIL(ct.datum);

    /* Corpus cum NIHIL */
    corpus = multipart_corpus_finire(NIHIL);
    CREDO_NIHIL(corpus.datum);

    /* Addere textum cum NIHIL */
    aed = multipart_creare(piscina);
    multipart_addere_textum(NIHIL, "test", chorda_ex_literis("val",
        piscina));
    multipart_addere_textum(aed, NIHIL, chorda_ex_literis("val",
        piscina));

    /* Addere fasciculum cum NIHIL */
    multipart_addere_fasciculum(NIHIL, "file", "test.bin", "text/plain",
                                 (constans i8*)"x", I);
    multipart_addere_fasciculum(aed, NIHIL, "test.bin", "text/plain",
                                 (constans i8*)"x", I);
    multipart_addere_fasciculum(aed, "file", NIHIL, "text/plain",
                                 (constans i8*)"x", I);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - BOUNDARY FORMAT
 * ======================================================================== */

interior vacuum
probatio_boundary_format(Piscina* piscina)
{
    MultipartAedificator* aed;
                  chorda  corpus;
                     i32  i;
                     b32  invenit_boundary;
                     b32  invenit_closing;

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_textum(aed, "test", chorda_ex_literis("value",
        piscina));

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);

    /* Verificare corpus incipit cum "--" */
    CREDO_VERUM(corpus.mensura >= II);
    CREDO_VERUM(corpus.datum[0] == '-');
    CREDO_VERUM(corpus.datum[I] == '-');

    /* Verificare corpus continet closing boundary "--" */
    invenit_boundary  = FALSUM;
    invenit_closing   = FALSUM;

    per (i = 0; i < corpus.mensura - III; i++)
    {
        si (corpus.datum[i] == '-' && corpus.datum[i + I] == '-')
        {
            invenit_boundary = VERUM;
        }
    }
    CREDO_VERUM(invenit_boundary);

    /* Verificare corpus finitur cum CRLF */
    si (corpus.mensura >= II)
    {
        CREDO_VERUM(corpus.datum[corpus.mensura - II] == '\r');
        CREDO_VERUM(corpus.datum[corpus.mensura - I] == '\n');
    }

    (vacuum)invenit_closing;

    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    probatio_creare(c->piscina);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    probatio_content_type(c->piscina);
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    probatio_textum_simplex(c->piscina);
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    probatio_multiples_campi(c->piscina);
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    probatio_fasciculum(c->piscina);
}

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    probatio_mixtum(c->piscina);
}

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    probatio_nullum_argumenta(c->piscina);
}

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    probatio_boundary_format(c->piscina);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "content type", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textum simplex", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiples campi", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasciculum", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixtum (textum + fasciculum)", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "nullum argumenta", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "boundary format", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("multipart", SECTIONES);
}
