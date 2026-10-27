/* praeconium.h - acta colloquiorum Claude Code (colloquia-plan T2)
 *
 * Liber appensus solum: linea JSON una per eventum, via ordinaria
 * ~/.rhubarb/colloquia/eventa.jsonl (RHUBARB_COLLOQUIA = directorium
 * aliud). Scriptura = write() unum O_APPEND (filum_appendere_firmiter):
 * linea integra inter scriptores concurrentes (hami plures simul).
 * Textus longi praeciduntur (UTF-8 integrum) ut linea brevis maneat.
 *
 * Instrumentum: tools/praeco.c ("praeco hamus" ex hamis, "praeco
 * status ..." ab agentibus, "praeco cantus ..." cantilena ad vicum,
 * "praeco cauda [n]").
 *
 * EVENTUS 'cantus' (praeconium_cantus.h scribit, vicus legit): eventus
 * "cantus", genus "intactus" aut "", nuntius = textus MML integer
 * (numquam praecisus: praeco longiorem recusat).
 *
 * EVENTUS 'tmux' (comites P8; praeco tmux, ex hamis tmux): fenestra,
 * panellus aut sessio clientis mutata - eventus "tmux", servitor =
 * #{socket_path}, panellus = #{pane_id}, sessio vacua. vicus eum
 * legit ut signum: systema terminalium statim interrogat (non post
 * V s). 'praeco tmux instituere': fasciculus hamorum proprius
 * (<directorium libri>/tmux.conf), sectio signata in ~/.tmux.conf
 * quae eum legit, et servitor currens eum legit.
 */

#ifndef PRAECONIUM_H
#define PRAECONIUM_H

/* <aedilis corpus="lib/praeconium.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* nuntius summum (octeti): linea brevis manet (scriptura integra);
 * longior praeciditur */
#define PRAECONIUM_NUNTIUS_MAXIMUS CC

nomen structura {
       s64 tempus;        /* ms ab epocha */
    chorda sessio;        /* session_id ("" pro 'status' extra hamum -
                            * T3 per panellum resolvit) */
    chorda eventus;       /* hook_event_name, aut "status" */
    chorda panellus;      /* TMUX_PANE ("%17"; "" extra tmux) */
    chorda servitor;      /* socket tmux (TMUX ante ',') */
    chorda via;           /* cwd */
    chorda transcriptum;  /* transcript_path */
    chorda genus;         /* notification_type / tool_name / trigger /
                            * reason */
    chorda nuntius;       /* message / prompt / status / ultimum
                            * responsum - praecisum */
} PraeconiumEventus;

/* via ordinaria libri (RHUBARB_COLLOQUIA aut ~/.rhubarb/colloquia) */
constans character*
praeconium_via (
    Piscina* piscina);

/* eventus ex JSON hami (stdin) et ambitu (TMUX_PANE, TMUX); FALSUM si
 * JSON malum */
b32
praeconium_ex_hamo (
                 chorda  json,
     constans character* panellus,
     constans character* tmux,
                    s64  tempus,
      PraeconiumEventus* e,
                Piscina* piscina);

/* lineam appendere (directorium creatur si necesse) */
b32
praeconium_scribere (
            constans character* via,
    constans PraeconiumEventus* e,
                       Piscina* piscina);

/* cauda libri: eventus ex ultimis 'octeti' octetis (linea fracta
 * omittitur), ordine scripturae */
Xar*
praeconium_caudam_legere (
    constans character* via,
                   i32  octeti,
               Piscina* piscina);

/* eventus post positionem (octeti a principio libri); *positio
 * promovetur ad finem lineae integrae ultimae (linea dimidia manet
 * legenda). Liber brevior quam *positio (deletus, rotatus): ab initio.
 * Liber absens: Xar vacuus, *positio 0. (colloquia-plan T4a) */
Xar*
praeconium_legere_ab (
    constans character* via,
                   s64* positio,
               Piscina* piscina);

/* comites P8: textus fasciculi hamorum tmux - session-window-changed,
 * client-session-changed, window-pane-changed, quisque "run-shell -b
 * '<praeco> tmux \"#{socket_path}\" \"#{pane_id}\"'"; praeco = via
 * absoluta (servitor cum PATH brevi eum tamen invenit). Iterum lectus
 * hamos substituit, nihil duplicat. Chorda vacua si via ' aut "
 * continet. PURA */
chorda
praeconium_tmux_configuratio (
    constans character* praeco,
               Piscina* piscina);

/* comites P8: ~/.tmux.conf cum sectione signata ("# praeco tmux:
 * initium" ... "# praeco tmux: finis") quae "source-file -q
 * <fasciculus>" continet: sectio prior substituitur, aliter in fine
 * appenditur; cetera intacta. PURA */
chorda
praeconium_tmux_conf (
                 chorda  conf,
     constans character* fasciculus,
                Piscina* piscina);

#endif /* PRAECONIUM_H */
