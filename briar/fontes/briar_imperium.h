/* briar_imperium.h - Regulae imperii binarii briar: vexilla, argumentum
 * primum reservatum, domus proiecti, stampae, sigillum vexillorum.
 *
 * Forma: briar [-vexillum] [-f <radix>] <x.thistle> [argumenta...]
 * Ante plagulam: -probatio -struere [-iterum] -arbor -partes -versio
 * -auxilium/-h, -f <radix>; aliud '-...' = recusatio (spiritus
 * optionum declaratarum). Post plagulam argumentum PRIMUM solum
 * inspicitur: unum e quinque vexillis actionis = actio (forma shebang
 * './x.thistle -probatio'); '--' = finis vexillorum (abscisum);
 * cetera reliqua programmatis, intacta. Cur non argumenta.h: vexilla
 * ignota recusat nec in positionali sistit - '-foo' programmatis
 * recusaretur.
 *
 * Domus proiecti: $HOME/.rhubarb/briar/<titulus>-<clavis>. Stampa
 * fontis disci = 'clausura:' + SHA-256 contentorum clausurae (filum
 * mtime nescit; clavis movetur cum plagula quae in proiectum it);
 * infixi = corpus.versio (clavis ante parsuram).
 */

#ifndef BRIAR_IMPERIUM_H
#define BRIAR_IMPERIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen enumeratio {
    BRIAR_ACTIO_CURRERE = 0,   /* (nihil): aedificare si abest, fieri */
    BRIAR_ACTIO_PROBATIO,      /* -probatio */
    BRIAR_ACTIO_STRUERE,       /* -struere [-iterum] */
    BRIAR_ACTIO_ARBOR,         /* -arbor */
    BRIAR_ACTIO_PARTES,        /* -partes */
    BRIAR_ACTIO_VERSIO,        /* -versio (sine plagula) */
    BRIAR_ACTIO_AUXILIUM       /* -auxilium / -h (sine plagula) */
} BriarActio;

nomen structura {
                    BriarActio  actio;
                           b32  iterum;      /* -iterum (struere) */
            constans character* via;         /* plagula aut NIHIL */
            constans character* fabrica;     /* -f <radix> aut NIHIL */
    constans character* constans* reliqua;   /* programmatis */
                           i32 numerus_reliquorum;
                        chorda causa;       /* recusatio si mensura */
} BriarImperium;

/* argv[0] = binarium; VERUM = imperium sanum; FALSUM + causa */
b32
briar_imperium_legere (
                         Piscina* piscina,
                             i32  argc,
    constans character* constans* argv,
                   BriarImperium* imperium);

/* $HOME/.rhubarb/briar/<titulus>-<sigillum>; NIHIL si HOME abest */
constans character*
briar_domus_proiecti (
               Piscina* piscina,
    constans character* titulus,
    constans character* sigillum_xvi);

/* 'clausura:' + XVI hex SHA-256 super (via, contentum) omnium
 * plagularum clausurae (Xar de SilexRes), ordine clausurae */
constans character*
briar_stampa_clausurae (
    Piscina* piscina,
        Xar* clausura);

/* XVI hex de SHA-256 chordae vexillorum (pro -versio) */
vacuum
briar_vexilla_sigillum (
    constans character* vexilla,
             character* hex_xvii);

#endif /* BRIAR_IMPERIUM_H */
