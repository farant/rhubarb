/* portae.h - PORTAE DECLARATAE: opertura portarum ex REPOSITORIO
 * (spec project-specs/portae-declaratae-spec.md; ledger …GGMGY94)
 *
 * Quae porta mutationi debetur ex declarationibus in capitibus
 * cursorum (annotatio STML in commentariis scripti) et derivatione
 * computatur - numquam ex tabulario (principium …RBA4W73). Nucleus
 * in C, involucra tenuia (principium …Z2Y555): pythonica solum vocat.
 *
 * ANNOTATIO (in commentario '#' cursoris; materia_annotationes per
 * clientem crustae - '#' intra chordam commentarium non est):
 *
 *   # <porta titulus="credo" tegit="tools/credo_*.py"/>
 *   # <porta manu causa="rete poscit (API vera - centesimae)"/>
 *
 *   titulus  clavis PORTAE quam hoc scriptum currit (plures elementa
 *            si scriptum plures portas currit - argumenta in PORTAE)
 *   manu     porta quam HOMO currit (sine PORTAE); causa necessaria
 *   tegit    exemplaria viarum (exemplar.h) ultra derivationem
 *   currit   programmata quae probationes lancant, derivationi caeca
 * Vocabulum CLAUSUM: attributum aliud = recusatio (error scribae).
 *
 * MACHINA PURA: ingressus et egressus per suturam (exemplar
 * aedilis) - probationes plagulas in memoria dant; CLI (bin/fabrica
 * portae, T3) discum et 'git ls-files'.
 */
#ifndef PORTAE_H
#define PORTAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* registrum PORTAE (pythonica/silva.py) ab involucro traditum:
 * clavis -> cursor (via scripti, radici relativa) */
nomen structura {
    chorda clavis;
    chorda cursor;
} PortaeIngressus;

nomen structura {
          vacuum* datum;
    /* viae candidatae (scripta), radici relativae: Xar de chorda;
     * NIHIL = enumeratio fracta */
            Xar* (*plagulas_enumerare) (vacuum* datum,
                Piscina* piscina);
    /* textus plagulae; FALSUM si legi nequit */
            b32  (*plagulam_legere) (vacuum* datum, chorda via,
                                     Piscina* piscina, chorda* textus);
    /* Xar de PortaeIngressus */
    constans Xar* tabula_portarum;
} PortaeSutura;

/* porta declarata una (elementum <porta> unum) */
nomen structura {
    chorda  via;        /* cursor: scriptum annotatum */
       i32  linea;      /* linea commentarii, I-basata */
    chorda  titulus;    /* clavis PORTAE; vacua si manu */
       b32  manu;
    chorda  causa;      /* manu: cur homo currit; vacua aliter */
       Xar* tegit;      /* chorda: exemplaria viarum */
       Xar* currit;     /* chorda: programmata */
} PortaDeclarata;

/* Declarationes omnium cursorum. Xar de PortaDeclarata (ordine viarum
 * enumeratarum, deinde commentariorum). NIHIL = RECUSATIO: *causa_out
 * causas OMNES nominat (linea una per causam, 'via:linea: ...'):
 * annotatio <porta> illegibilis, attributum ignotum, titulus et manu
 * simul aut neuter, manu sine causa, titulus extra registrum, cursor
 * alienus (registrum clavem alii scripto dat), titulus bis declaratus,
 * clavis registri sine cursore annotato. */
Xar*
portae_declaratas_legere (
    constans PortaeSutura* sutura,
                  Piscina* piscina,
                   chorda* causa_out);

#endif /* PORTAE_H */
