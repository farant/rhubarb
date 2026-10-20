/* admissio.h - servus auctoritatis OAuth 2.1 (pro servis MCP)
 *
 * Applicatio (e.g. exemplar) admissionem in volumine suo habet:
 * tabulae propriae (admissio_clientes, _concessiones, _codices,
 * _tesserae) per volumen_scrinium; concessio creata / revocata
 * etiam ut acta voluminis (vestigium auditus).
 *
 *   METADATA RFC 8414: issuer, endpoints, S256, "none", code,
 *   authorization_code + refresh_token, scopi + offline_access, iss.
 *
 *   INSCRIPTIO DYNAMICA RFC 7591: clientes publici (sine secreto);
 *   redirectiones https exactae aut loopback (127.0.0.1 / localhost,
 *   portus quivis); frenum per IP (Claude se inscribit omni
 *   connexione nova); clientes numquam usi post diem putantur.
 *
 *   TESSERAE OPACAE: XXXII octeti fortuiti, base64url (XLIII
 *   signa); SOLUM digestum SHA-256 conditur, comparatio constans.
 *   Accessus (I h) et renovatio (XXX d) audientiae (resource)
 *   ligatae.
 *
 *   VERIFICATOR: McpVerificator (mcp_servus_http) - tessera ->
 *   persona (usor, cliens, scopi).
 *
 * Tesserae codicesque numquam in actis, erroribus aut paginis.
 * (Pars II, T6: /oauth/authorize, consensus, /oauth/token.)
 */

#ifndef ADMISSIO_H
#define ADMISSIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "mcp_servus.h"

/* inscriptiones per IP intra LX secunda */
#define ADMISSIO_INSCRIPTIONES_PER_MINUTUM XX

/* signa tesserae: XXXII octeti base64url */
#define ADMISSIO_TESSERA_MENSURA XLIII

nomen structura {
              constans character* auctoritas;   /* issuer, sine '/'
                                                  * finali */
              constans character* audientia;    /* OAuth "resource":
                                                  * "https://h/mcp" */
    constans character* constans*  scopi;        /* NIHIL-terminata */
    /* vitae: 0 = LX s (codex), I h (accessus), XXX d (renovatio) */
                              s64 vita_codicis_ms;
                              s64 vita_accessus_ms;
                              s64 vita_renovationis_ms;
    /* ms ab epocha; NIHIL = horologium verum (probationes iniciunt) */
                              s64  (*nunc_ms)(vacuum);
} AdmissioOptiones;

nomen structura Admissio Admissio;

/* connexio una personae (pagina /applicationes) */
nomen structura {
    chorda id;
    chorda cliens;
    chorda titulus_clientis;
    chorda usor;
    chorda scopi;
       s64 creata_ms;
       s64 usus_ultimus_ms;
       b32 revocata;
} AdmissioConcessio;

/* tabulas creat si desunt; optiones transcribuntur */
Admissio*
admissio_creare (
                      Volumen* volumen,
    constans AdmissioOptiones* optiones,
                      Piscina* piscina);

/* RFC 8414 JSON */
chorda
admissio_documentum (
    Admissio* admissio,
     Piscina* piscina);

/* corpus JSON RFC 7591 -> status (201, 400, 429) + responsum JSON */
s32
admissio_inscribere (
     Admissio* admissio,
       chorda  corpus,
       chorda  cliens_ip,
       chorda* responsum,
      Piscina* piscina);

/* McpVerificator: datum = Admissio* */
b32
admissio_verificator (
        vacuum* admissio,
        chorda  tessera,
        chorda  audientia,
    McpPersona* exitus,
       Piscina* piscina);

/* concessiones vivae usoris (Xar de AdmissioConcessio), creatione
 * ordinatae */
Xar*
admissio_concessiones (
     Admissio* admissio,
       chorda  usor,
      Piscina* piscina);

/* concessionem et omnes eius tesseras necat (petitione proxima
 * effectum); FALSUM si non usoris aut iam revocata */
b32
admissio_revocare (
     Admissio* admissio,
       chorda  usor,
       chorda  concessio);

/* clientes sine concessione ultra diem delet; numerus deletorum.
 * Codices et tesserae expirati quoque purgantur (non numerati). */
s32
admissio_putare (
    Admissio* admissio);

#ifdef ADMISSIO_PROBATIO
/* uncus probationis (T5): concessio + tesserae sine /oauth/token.
 * FALSUM si cliens ignotus. */
b32
admissio_concessionem_creare_probationis (
    Admissio* admissio,
       chorda  usor,
       chorda  cliens,
       chorda  scopi,
      chorda* accessus,
      chorda* renovatio,
     Piscina* piscina);
#endif

#endif /* ADMISSIO_H */
