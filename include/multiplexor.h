/* multiplexor.h - tmux per lineam imperii (colloquia-plan T1)
 *
 * Imperia per processus (vector argumentorum, nulla concha). Lectio
 * PURA (effusio "-F" in structuras; probationes per fixa) separata ab
 * exsecutione. Contextus servitorem nominat: socket NIHIL = servitor
 * ordinarius usoris; probationes servitorem proprium creant (socket
 * via cum '/' = -S, ut in $TMPDIR scribatur; aliter nomen = -L).
 * Binarium: NIHIL = quaeritur (PATH, /opt/homebrew/bin,
 * /usr/local/bin) - vicus ex Doco PATH brevem habet; datum creditur.
 * Sessiones ut scopus exactus ("=nomen") - tmux aliter praefixum
 * congruit. Semper -u: sine LANG/LC_* tmux characteres regiminis
 * effusionis (tabulationes formae) in '_' mutat.
 *
 * Forma effusionis (huius bibliothecae, campi tabulatione separati):
 * panelli "#{pane_id} #{session_name} #{window_index} #{window_name}
 * #{pane_index} #{pane_pid} #{pane_tty} #{pane_current_command}
 * #{pane_current_path} #{pane_active} #{window_active}", clientes
 * "#{client_tty} #{session_name}".
 */

#ifndef MULTIPLEXOR_H
#define MULTIPLEXOR_H

/* <aedilis corpus="lib/multiplexor.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen structura {
    constans character* binarium;   /* NIHIL = quaerere */
    constans character* socket;     /* nomen (-L) aut via cum '/' (-S);
                                     * NIHIL = ordinarius */
                   i32 mora_ms;    /* terminus imperii; 0 = M */
} Multiplexor;

/* panellus (pane) cum fenestra et sessione sua */
nomen structura {
    chorda id;                /* "%17" - stabilis per vitam panelli */
    chorda sessio;            /* session_name */
       s32 fenestra;          /* window_index */
    chorda fenestra_titulus;  /* window_name */
       s32 index;             /* pane_index */
       s32 pid;               /* pane_pid (concha aut programma) */
    chorda tty;               /* /dev/ttys011 */
    chorda imperium;          /* pane_current_command ("2.1.296" =
                                * Claude Code) */
    chorda via;               /* pane_current_path */
       b32 activus;           /* pane_active in fenestra sua */
       b32 fenestra_activa;   /* window_active in sessione sua */
} MultiplexorPanellus;

/* cliens (terminale adhaerens) */
nomen structura {
    chorda tty;               /* client_tty - terminale vici agnoscitur
                                * per tty sui pty */
    chorda sessio;            /* session_name cui adhaeret */
} MultiplexorCliens;

/* PURA: effusio list-panes in Xar de MultiplexorPanellus; lineae
 * malae omittuntur */
Xar*
multiplexor_panellos_legere (
      chorda  effusio,
     Piscina* piscina);

/* PURA: effusio list-clients in Xar de MultiplexorCliens */
Xar*
multiplexor_clientes_legere (
      chorda  effusio,
     Piscina* piscina);

/* exsequi: omnes panelli servitoris; NIHIL si tmux abest, servitor
 * non currit, aut imperium deficit */
Xar*
multiplexor_panelli (
    constans Multiplexor* m,
                 Piscina* piscina);

/* exsequi: omnes clientes servitoris; NIHIL ut supra */
Xar*
multiplexor_clientes (
    constans Multiplexor* m,
                 Piscina* piscina);

/* panellus per id ("%17"); NIHIL si abest */
constans MultiplexorPanellus*
multiplexor_panellum_invenire (
       Xar* panelli,
    chorda  id);

/* sessio exsistit in servitore? */
b32
multiplexor_sessio_exsistit (
    constans Multiplexor* m,
                  chorda  sessio,
                 Piscina* piscina);

/* panellum eligere: fenestra eius activa in sessione, panellus
 * activus in fenestra (select-window + select-pane) */
b32
multiplexor_panellum_eligere (
    constans Multiplexor* m,
                  chorda  id,
                 Piscina* piscina);

/* clientem (per tty) ad sessionem panelli vertere (switch-client) */
b32
multiplexor_clientem_vertere (
    constans Multiplexor* m,
                  chorda  tty,
                  chorda  id,
                 Piscina* piscina);

/* vector "tmux -u [-L s | -S via] attach -t =<sessio>" pro terminali
 * novo (NIHIL finitum, in piscina); NIHIL si tmux non invenitur */
constans character**
multiplexor_argumenta_adhaerendi (
    constans Multiplexor* m,
                  chorda  sessio,
                 Piscina* piscina);

#endif /* MULTIPLEXOR_H */
