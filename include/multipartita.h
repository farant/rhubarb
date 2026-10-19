/* multipartita.h - parser incrementalis multipart/form-data (RFC 7578)
 *
 * Octeti adveniunt ut adveniunt (fragmenta quaevis, etiam singuli):
 * parser reditus vocat - initium partis, data partis (per partes),
 * finis partis. Nihil totum in memoria: partes fili in filum
 * temporarium fluere possunt dum adveniunt (hospitium id facit).
 *
 * LIMES: delimitator = CRLF "--" limes (RFC 2046 par. 5.1.1); corpus
 * tamquam CRLF praefixum tractatur, ergo "--limes" initio sine CRLF
 * quoque agnoscitur. Comparatio per tabulam defectus (Knuth-Morris-
 * Pratt): octeti retenti semper praefixum delimitatoris SUNT, ergo
 * nulla capsa retrospectiva - fragmentum ubivis scissum idem reddit.
 * "--limes" sequente alio quam "--", spatio aut CRLF data est, non
 * limes (ut "\r\n--limesX").
 *
 * CAPITA PARTIS: CRLF solum (LF solum = error nominatus). Content-
 * Disposition: form-data cum name (necesse), filename, filename*
 * (RFC 5987: charset'lingua'%XX - praevalet). Titulus fili ad nomen
 * ultimum redigitur (post '/' aut '\'; "." et ".." vacua) - numquam
 * via. Content-Type absens = "text/plain" (RFC 7578 par. 4.4).
 *
 * Errores: post errorem omnis vocatio FALSUM; multipartita_error
 * causam nominat. */
#ifndef MULTIPARTITA_H
#define MULTIPARTITA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

#define MULTIPARTITA_LIMES_MAXIMUS LXX

/* 0 = defalta */
nomen structura {
    i32 partes_maximae;          /* 0 = LXIV */
    i32 caput_partis_maximum;    /* 0 = 8 KB */
    i32 campus_maximus;          /* 0 = 64 KB (partes non fila) */
} MultipartitaLimites;

/* Reditus: FALSUM = recusare (parser in errorem cadit). Chordae
 * initii in piscina parseris vivunt. */
nomen structura {
    b32 (*initium)(
                     vacuum* datum,
                      chorda  campus,
                      chorda  titulus_fili,
                         b32  est_filum,
                      chorda  mimen);
    b32 (*data)(
                     vacuum* datum,
                constans i8* octeti,
                        i32  n);
    b32 (*finis)(
                     vacuum* datum);
    vacuum* datum;
} MultipartitaReditus;

nomen structura MultipartitaParser MultipartitaParser;

/* Limes ex Content-Type "multipart/form-data; boundary=..." (citatus
 * aut nudus; 1..LXX signa RFC 2046). Chorda vacua si typus alius,
 * limes absens aut malus. */
chorda
multipartita_limes_ex_typo (
      chorda  content_type,
     Piscina* piscina);

/* NIHIL si limes vacuus aut nimis longus */
MultipartitaParser*
multipartita_creare (
                          chorda  limes,
    constans MultipartitaLimites* limites,
             MultipartitaReditus  reditus,
                         Piscina* piscina);

/* Octetos adicere (n quodvis, etiam 0). FALSUM = error (nominatus);
 * octeti post limitem finalem (epilogus) ignorantur. */
b32
multipartita_adicere (
     MultipartitaParser* parser,
            constans i8* octeti,
                    i32  n);

/* Finis corporis: VERUM solum si clausum per "--limes--" */
b32
multipartita_finire (
    MultipartitaParser* parser);

/* Causa erroris; chorda vacua si nullus */
chorda
multipartita_error (
    MultipartitaParser* parser);

#endif /* MULTIPARTITA_H */
