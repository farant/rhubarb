/* tessera_es256.h - tessera JWT ES256 (RFC 7515/7518; http2-plan T10)
 *
 * Nucleus communis: nundinae_apple (App Store Connect) et APNs (push).
 * Clavis PKCS#8 DER P-256; signatura per BearSSL (vendor), RFC 6979
 * determinata. Contextus clavis ante reditum zephyratur; clavis numquam
 * in errore aut tabula apparet. */
#ifndef TESSERA_ES256_H
#define TESSERA_ES256_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* base64url(caput) "." base64url(asserta) "." base64url(r || s), sine
 * '='. caput_json et asserta_json ut data (vocans formam JSON curat).
 * Vacua si clavis mala (non EC, non P-256, truncata) aut NIHIL. */
chorda
tessera_es256_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  caput_json,
         chorda  asserta_json,
        Piscina* piscina);

/* tessera APNs (Apple: "token-based provider authentication"): caput
 * {"alg":"ES256","kid":<clavis_id>}, asserta {"iss":<turma>,"iat":
 * <nunc_s>}. Identificatores litteris, numeris, - _ . soli (I..CXXVIII);
 * aliter vacua. Apple tesseram usque ad horam accipit, renovationem
 * crebriorem quam XX minutis non vult - vocans eam servat. */
chorda
tessera_apns_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  clavis_id,
         chorda  turma,
            s64  nunc_s,
        Piscina* piscina);

#endif /* TESSERA_ES256_H */
