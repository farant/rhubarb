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

    printf("--- Probans creare ---\n");

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
    chorda                ct;

    printf("--- Probans content type ---\n");

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
    chorda                corpus;

    printf("--- Probans textum simplex ---\n");

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_textum(aed, "name", chorda_ex_literis("John", piscina));

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
    chorda                corpus;

    printf("--- Probans multiples campi ---\n");

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
    chorda                corpus;
    constans i8           data[] = {0x01, 0x02, 0x03, 0x04, 0x05};

    printf("--- Probans fasciculum ---\n");

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
    chorda                corpus;
    chorda                ct;
    constans i8           audio_data[] = "fake audio data for testing";

    printf("--- Probans mixtum (textum + fasciculum) ---\n");

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
    chorda                ct;
    chorda                corpus;

    printf("--- Probans nullum argumenta ---\n");

    /* Content type cum NIHIL */
    ct = multipart_content_type(NIHIL);
    CREDO_NIHIL(ct.datum);

    /* Corpus cum NIHIL */
    corpus = multipart_corpus_finire(NIHIL);
    CREDO_NIHIL(corpus.datum);

    /* Addere textum cum NIHIL */
    aed = multipart_creare(piscina);
    multipart_addere_textum(NIHIL, "test", chorda_ex_literis("val", piscina));
    multipart_addere_textum(aed, NIHIL, chorda_ex_literis("val", piscina));

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
    chorda                corpus;
    i32                   i;
    b32                   invenit_boundary;
    b32                   invenit_closing;

    printf("--- Probans boundary format ---\n");

    aed = multipart_creare(piscina);
    CREDO_NON_NIHIL(aed);

    multipart_addere_textum(aed, "test", chorda_ex_literis("value", piscina));

    corpus = multipart_corpus_finire(aed);
    CREDO_NON_NIHIL(corpus.datum);

    /* Verificare corpus incipit cum "--" */
    CREDO_VERUM(corpus.mensura >= II);
    CREDO_VERUM(corpus.datum[0] == '-');
    CREDO_VERUM(corpus.datum[I] == '-');

    /* Verificare corpus continet closing boundary "--" */
    invenit_boundary = FALSUM;
    invenit_closing = FALSUM;

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

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES MULTIPART\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_multipart", CXXVIII * M);
    credo_aperire(piscina);

    probatio_creare(piscina);
    probatio_content_type(piscina);
    probatio_textum_simplex(piscina);
    probatio_multiples_campi(piscina);
    probatio_fasciculum(piscina);
    probatio_mixtum(piscina);
    probatio_nullum_argumenta(piscina);
    probatio_boundary_format(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
