/* scriba_documentum.h - documentum scribae = cauda mutationum
 *
 * Folium fixum (TabulaCharacterum, latitudo x altitudo) cuius
 * veritas est acta in volumine (historia, genus "mutatio"); folium
 * est proiectio. Actum = EFFECTUS mutationis vim, non claves
 * (scriba-plan I.3): lineae substitutae, ut replicatio vim numquam
 * currat.
 *
 * MUTATIO (v1):
 *   <mutatio linea="3" deletae="1">
 *     <linea indentatio="4" textus="    nova"/>
 *     <linea/>
 *   </mutatio>
 * Folium = lineae eius sine lineis ALBIS finalibus (alba = omnes
 * cellulae ' ' et indentatio -1, ut tabula_initiare et
 * inserere/delere lineam scribunt; '\0' alius octetus est - linea
 * cum '\0' contentum est). Applicare: ad 'linea' 'deletae' lineas
 * tollere, liberos inserere, ad altitudinem lineis albis implere.
 * Mutatio ex duobus foliis = praefixum et suffixum communia - exacta
 * pro omni pari foliorum, quidquid vim fecit (linea non alba sed sine
 * contentu visibili ex fundo trusa quoque).
 * textus: cellulae lineae sine ' ' finalibus (lector ' ' implet);
 * effugia '\\' '\0' '\t' '\1' (TAB_CONTINUATIO) '\q' (") et '\xHH'
 * (ceteri < 0x20, >= 0x7F).
 * Valor attributi CRUDUS in STML est - nullae entitates.
 *
 * Memoria proiectionis: cellulae (latitudo x altitudo) deinde
 * indentatio (s32 x altitudo, ordine machinae) ad offset ordinatum;
 * tabula.cellulae et tabula.indentatio in eam monstrant. */

#ifndef SCRIBA_DOCUMENTUM_H
#define SCRIBA_DOCUMENTUM_H

/* <aedilis corpus="lib/scriba_documentum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "tabula_characterum.h"
#include "historia.h"

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    i32  latitudo;
                    i32  altitudo;
                    i32  intervallum;   /* acta per checkpoint */
                     i8* memoria;       /* cellulae + indentatio */
         memoriae_index  mensura;
      TabulaCharacterum  tabula;        /* proiectio (in memoria) */
               Historia* historia;
} ScribaDocumentum;


/* ==================================================
 * Mutatio (pura: sine volumine)
 * ================================================== */

/* mutatio ex folio 'ante' in folium 'post' (dimensionibus eisdem);
 * chorda vacua si aequalia aut dimensiones discordes */
chorda
scriba_mutatio_computare (
    constans TabulaCharacterum* ante,
    constans TabulaCharacterum* post,
                       Piscina* piscina);

/* mutationem in folium applicare; FALSUM (folio intacto) si mutatio
 * mala aut folium excederet */
b32
scriba_mutatio_applicare (
      TabulaCharacterum* tabula,
                 chorda  mutatio,
                Piscina* piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Documentum
 * ================================================== */

/* novum: manifestum 'documentum' scribitur, folium vacuum */
ScribaDocumentum*
scriba_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum);

/* ex volumine exsistente (manifestum + historia) */
ScribaDocumentum*
scriba_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* folium laboris 'post' in caudam: mutatio contra proiectionem
 * computatur et appenditur. Redde seq (> 0); 0 si nihil mutatum
 * aut recusatum. Post: proiectio == post. */
s64
scriba_documentum_committere (
               ScribaDocumentum* doc,
     constans TabulaCharacterum* post);

b32
scriba_documentum_revocare (
    ScribaDocumentum* doc);

b32
scriba_documentum_reficere (
    ScribaDocumentum* doc);

b32
scriba_documentum_verificare (
    ScribaDocumentum* doc);

/* proiectio (lectio sola - mutare per committere) */
constans TabulaCharacterum*
scriba_documentum_tabula (
    constans ScribaDocumentum* doc);

chorda
scriba_documentum_sigillum_hex (
    constans ScribaDocumentum* doc,
                      Piscina* piscina);

s64
scriba_documentum_cursor (
    constans ScribaDocumentum* doc);

s64
scriba_documentum_finis (
    constans ScribaDocumentum* doc);

i32
scriba_documentum_numerus_vivorum (
    constans ScribaDocumentum* doc);

#endif /* SCRIBA_DOCUMENTUM_H */
