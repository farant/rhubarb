/* toml_scalaris.h - Coctio scalarium toml: chordae, numeri, tempora,
 * commentaria; regulae octetorum et formae (Q7a)
 *
 * Arbor (toml_arbor) formam grossam iudicat: lexema numero simile
 * UNUM (NUMERUS/TEMPUS) et chordam ad claudentem. Hic eadem lexemata
 * accurate leguntur - effugia, bases, sublineae, fines s64, campi
 * temporis - et quod TOML 1.0 vetat diagnosticum NOMINATUM fit, sede
 * in octeto ipso vitii (non in lexemate toto).
 *
 * DOMINIUM DIAGNOSTICORUM: toml_scalaria_iudicare solum ea emittit
 * (omne lexema arboris, trivia comprehensa, ordine octetorum). Coctio
 * tabularum (toml_coctum) valores per toml_scalarem_coquere cum
 * diagnostica NIHIL tacite coquit - nullum diagnosticum bis.
 *
 * Lexemata intra 'malum' non iudicantur (syntaxis ea iam nominavit);
 * trivia eorum iudicantur. Chorda non clausa usque ad finem lexematis
 * decoditur sine diagnostico novo (aedificator eam numeravit).
 *
 * Verdicta pinnata (Q1, tomllib): CRLF in chorda multa -> '\n'
 * (tomllib CRLF ubique normat); CR solus = octetus moderans; BOM =
 * IGNOTUM syntaxis (non hic). Secundum 60 licet (RFC 3339; tomllib
 * id recusat - differentia nominata in worklog).
 */

#ifndef TOML_SCALARIS_H
#define TOML_SCALARIS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "materia_nodus.h"
#include "materia_diagnosticum.h"
#include "toml_valor.h"

/* Codices diagnosticorum scalarium ('grex/regula'; textus = chordae et
 * commentaria communiter) */
#define TOML_CODEX_OCTETUS_MODERANS  "textus/octetus-moderans"
#define TOML_CODEX_UTF8              "textus/utf8"
#define TOML_CODEX_EFFUGIUM          "chorda/effugium"
#define TOML_CODEX_EFFUGIUM_MANCUM   "chorda/effugium-mancum"
#define TOML_CODEX_PUNCTUM_CODICIS   "chorda/punctum-codicis"
#define TOML_CODEX_INTEGER           "numerus/integer"
#define TOML_CODEX_EXTRA_FINES       "numerus/extra-fines"
#define TOML_CODEX_FLUITANS          "numerus/fluitans"
#define TOML_CODEX_TEMPUS_FORMA      "tempus/forma"
#define TOML_CODEX_TEMPUS_LIMITES    "tempus/extra-limites"

/* Vitium primum lexematis numeri aut temporis (lectio pura) */
nomen structura {
    constans character* codex;     /* TOML_CODEX_*; NIHIL = sanum */
    constans character* causa;     /* regula violata (sine textu) */
                   s32  initium;   /* octeti vitii intra textum */
                   s32  finis;     /* exclusivus; == initium: punctum */
} TomlVitiumScalare;

/* Chorda (CHORDA_* quattuor) aut clavis quotata (CLAVIS_GEMINA,
 * CLAVIS_SIMPLEX) -> textus decoditus in piscina; CLAVIS_NUDA ->
 * valor ipse. FALSUM = vitium (diagnostica, si non NIHIL, omnia
 * accipiunt: omne effugium pravum, omnis octetus moderans) aut genus
 * alienum. */
b32
toml_chordam_coquere (
                  Piscina* piscina,
    constans MateriaToken* lexema,
                   chorda* exitus,
                      Xar* diagnostica);

/* Nodus chorda / numerus / boolean / tempus -> valor (nodus ponitur).
 * FALSUM = vitium (diagnosticum appensum si diagnostica non NIHIL);
 * exitus tunc genus et nodum fert, datum incertum. */
b32
toml_scalarem_coquere (
                  Piscina* piscina,
    constans MateriaNodus* nodus,
                TomlValor* exitus,
                      Xar* diagnostica);

/* Lectio pura temporis (oraculum Q8 eadem utitur): forma, deinde
 * limites campi. FALSUM = vitium in 'vitium' descriptum. */
b32
toml_tempus_legere (
                chorda  textus,
            TomlTempus* exitus,
     TomlVitiumScalare* vitium);

/* Omnia lexemata arboris (trivia comprehensa), ordine octetorum:
 * diagnostica scalaria in Xar NOVUM de MateriaDiagnosticum. Xar vacuum
 * = sanum; NIHIL = memoria defecit. */
Xar*
toml_scalaria_iudicare (
                  Piscina* piscina,
    constans MateriaNodus* radix);

#endif /* TOML_SCALARIS_H */
