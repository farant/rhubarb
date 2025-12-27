/* multipart.h - Multipart Form Data Builder
 *
 * Bibliotheca pro constructione multipart/form-data bodies.
 * Utilis pro file uploads (Whisper API, image APIs, etc.).
 *
 * USUS:
 *   MultipartAedificator* form = multipart_creare(piscina);
 *   multipart_addere_textum(form, "model", chorda_ex_literis("whisper-1", p));
 *   multipart_addere_fasciculum(form, "file", "audio.mp3", "audio/mpeg", data, len);
 *
 *   http_petitio_caput_addere(pet, "Content-Type",
 *       chorda_ut_cstr(multipart_content_type(form), p));
 *   http_petitio_corpus_ponere_chorda(pet, multipart_corpus_finire(form));
 */

#ifndef MULTIPART_H
#define MULTIPART_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Opaque builder type */
nomen structura MultipartAedificator MultipartAedificator;


/* ========================================================================
 * FUNCTIONES - CREARE
 * ======================================================================== */

/* Creare novum multipart form builder
 *
 * piscina: Arena pro allocatio
 *
 * Redde: MultipartAedificator* vel NIHIL si error
 */
MultipartAedificator*
multipart_creare(Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - ADDERE CAMPOS
 * ======================================================================== */

/* Addere text field ad form
 *
 * aed:         Builder
 * nomen_campi: Nomen campi (e.g., "model", "language")
 * valor:       Valor campi
 */
vacuum
multipart_addere_textum(
    MultipartAedificator* aed,
    constans character*   nomen_campi,
    chorda                valor);

/* Addere file field ad form
 *
 * aed:             Builder
 * nomen_campi:     Nomen campi (e.g., "file")
 * nomen_fasciculi: Nomen fasciculi (e.g., "audio.mp3")
 * content_type:    MIME type (e.g., "audio/mpeg", "image/png")
 * datum:           Data fasciculi
 * mensura:         Longitudo datorum
 */
vacuum
multipart_addere_fasciculum(
    MultipartAedificator* aed,
    constans character*   nomen_campi,
    constans character*   nomen_fasciculi,
    constans character*   content_type,
    constans i8*          datum,
    i32                   mensura);


/* ========================================================================
 * FUNCTIONES - FINIRE
 * ======================================================================== */

/* Obtinere Content-Type header (includit boundary)
 *
 * Redde: chorda "multipart/form-data; boundary=..."
 */
chorda
multipart_content_type(MultipartAedificator* aed);

/* Construere corpus finale
 *
 * Redde: chorda cum complete multipart body
 */
chorda
multipart_corpus_finire(MultipartAedificator* aed);


#endif /* MULTIPART_H */
