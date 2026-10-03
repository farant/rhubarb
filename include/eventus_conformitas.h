/* eventus_conformitas.h - Tabula conformitatis fontium eventuum
 * (eventus A4; spec D7)
 *
 * SCAENA: quid immittitur (fenestra: claves et mus per API
 * immissionis; terminalis: octeti - phasis B) et quem fluxum Eventuum
 * fons reddere DEBET. Tabula in STML:
 *
 *   <conformitas>
 *     <scaena titulus="shift-a" genera="clavis_depressus textus ...">
 *       <immissio>
 *         <clavis codex="0" modificantes="131074" characteres="A"
 *                 depressa="1"/>
 *         <mus genus="depressio" x="10" y="20"/>
 *       </immissio>
 *       <expectata>
 *         <eventus genus="clavis_depressus" codex="KeyA" runa="97"/>
 *         ...
 *       </expectata>
 *     </scaena>
 *   </conformitas>
 *
 * COMPARATIO (pura, sine fenestra):
 * - 'genera' = genera CONSIDERATA: eventa actualia aliorum generum
 *   omittuntur (motus veri muris, facultates, magnitudo). Genus
 *   consideratum quod expectata non habent = vitium (textus sub Ctrl).
 * - ORDO EXACTUS, NUMERUS EXACTUS.
 * - attributa expectata SUBSET actualium: quae scaena nominat aequalia
 *   esse debent; cetera libera. 'tempus' semper neglegitur. Liberi
 *   (exempla) non comparantur.
 * - actualia per eventus_scribere_stml scribuntur et relecta
 *   comparantur: tituli et formae eaedem ac in plagulis.
 * - 'modificantes' (clavis, mus, rotula) in bits VOCABULARII solum
 *   comparantur (MOD_* et latera; eventus B4): bits platformae extra
 *   vocabularium (AppKit Function 0x800000 in sagittis) neglecti.
 *
 * EXCUSATIONES (spec D7; eventus B4): scaena differentias permissas
 * PER FACULTATEM nominat (attributum 'excusationes', titulis
 * facultatum). Excusatio valet SOLUM si FACULTATES ultimae fluxus
 * actualis eam facultatem negant; fluxus sine FACULTATIBUS: nihil
 * excusatur (fenestra post vacuationem: aeque stricta ac antea).
 */

#ifndef EVENTUS_CONFORMITAS_H
#define EVENTUS_CONFORMITAS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "eventus.h"

/* Excusationes: titulus in tabula = facultas quae deesse debet.
 *   liberationes         clavis_liberatus expectata omittuntur
 *   codex_physicus       'codex' non comparatur
 *   latera               bits laterum in modificantibus neglecti
 *   tabula_distincta     scaena tota EXCUSATA (Ctrl+I = Tab)
 *   modificantes_textus  'modificantes' non comparantur */
#define CONFORMITAS_EXCUSATIO_LIBERATIONES         0x01
#define CONFORMITAS_EXCUSATIO_CODEX_PHYSICUS       0x02
#define CONFORMITAS_EXCUSATIO_LATERA               0x04
#define CONFORMITAS_EXCUSATIO_TABULA_DISTINCTA     0x08
#define CONFORMITAS_EXCUSATIO_MODIFICANTES_TEXTUS  0x10

nomen enumeratio {
    CONFORMITAS_FRACTA = ZEPHYRUM,
    CONFORMITAS_CONFORMIS,
    CONFORMITAS_EXCUSATA        /* scaena excusata per facultatem */
} ConformitasVerdictum;

nomen enumeratio {
    CONFORMITAS_IMMISSIO_CLAVIS = ZEPHYRUM,
    CONFORMITAS_IMMISSIO_MUS
} ConformitasImmissioGenus;

/* Immissio una. clavis: codex VIRTUALIS platformae (macOS kVK),
 * modificantes cruda, characteres (UTF-8), depressa. mus: genus ut
 * verba fenestra_musarius ("motus", "depressio", "tractus",
 * "liberatio", "depressio-dextra", "liberatio-dextra"), x, y,
 * modificantes. Executio = res instrumenti (fons), non huius. */
nomen structura {
    ConformitasImmissioGenus genus;
                         s32 codex;
                         i32 modificantes;
                      chorda characteres;
                         b32 depressa;
                      chorda mus_genus;
                         s32 x;
                         s32 y;
} ConformitasImmissio;

nomen structura {
     chorda  titulus;
        Xar* genera;        /* Xar de s32 (eventus_genus_t) */
        Xar* immissiones;   /* Xar de ConformitasImmissio */
        Xar* expectata;     /* Xar de StmlNodus* (<eventus/>) */
        i32  excusationes;  /* CONFORMITAS_EXCUSATIO_* */
} ConformitasScaena;

/* Tabulam legere. Redde Xar de ConformitasScaena, aut NIHIL si
 * STML pravum, genus ignotum in 'genera' aut facultas ignota in
 * 'excusationes'. */
Xar*
eventus_conformitas_legere (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

/* Fluxum actualem (Xar de Eventus) cum scaena comparare (excusationes
 * per FACULTATES fluxus). *diagnosis (si non NIHIL): quid differt, cum
 * fluxibus ambobus (vacua nisi FRACTA). */
ConformitasVerdictum
eventus_conformitas_comparare (
    constans ConformitasScaena* scaena,
                  constans Xar* actualia,
                       Piscina* piscina,
           InternamentumChorda* intern,
                        chorda* diagnosis);

#endif /* EVENTUS_CONFORMITAS_H */
