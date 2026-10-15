/* vicus_quaestio_interna.h - porta $question IN PROCESSU (via B,
 * http-vocatio-spec II.6)
 *
 * Porta VicusPortaQuaestionis super vates_vocare_rivum et reactorem
 * HOSPITIS: hospes reactorem in omni pulsu poscit (R1), porta solum
 * manubrium legit - nihil obstat. Textus crudus colligitur et planatur
 * (quaestio_textum_planare, ut via A).
 *
 * Clavis API per petitionem legitur (via_clavis aut
 * $HOME/.rhubarb/anthropic.clavis) in piscinam VOCATIONIS propriam,
 * quae in fine VACATUR (piscina_vacare: omnia blocka zephyrantur -
 * copiae in Vates, capitibus, octetis) et destruitur (R2, decretum
 * ...AAQVQ3). Clavis numquam in textu, errore, statu. */
#ifndef VICUS_QUAESTIO_INTERNA_H
#define VICUS_QUAESTIO_INTERNA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "vicus_quaestio.h"

nomen structura {
    constans character* via_clavis;        /* NIHIL = $HOME/.rhubarb/
                                            * anthropic.clavis */
    constans character* url;               /* NIHIL = API Anthropic */
                   i32  conatus_maximi;    /* 0 = vatis ordinarium */
                   i32  mora_iterandi_ms;  /* 0 = vatis ordinarium */
} VicusQuaestioInternaOptiones;

/* optiones NIHIL = ordinariae; reactor aut piscina NIHIL -> porta nulla
 * (incipere NIHIL) */
VicusPortaQuaestionis
vicus_quaestio_interna (
                                  Reactor* reactor,
    constans VicusQuaestioInternaOptiones* optiones,
                                  Piscina* piscina);

#endif /* VICUS_QUAESTIO_INTERNA_H */
