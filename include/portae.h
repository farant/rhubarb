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
#include "stml.h"

/* registrum PORTAE (pythonica/silva.py) ab involucro traditum:
 * clavis -> cursor (via scripti, radici relativa); probationes =
 * directorium suitae (SUITAE: <dir>/probatio_*.c), vacua = porta sine
 * suita */
nomen structura {
    chorda clavis;
    chorda cursor;
    chorda probationes;
} PortaeIngressus;

nomen structura {
          vacuum* datum;
    /* viae OMNES arboris (git ls-files), radici relativae: Xar de
     * chorda; NIHIL = enumeratio fracta. Leguntur solae '.sh'
     * (cursores omnes scripta crustae sunt) */
            Xar* (*plagulas_enumerare) (vacuum* datum,
                Piscina* piscina);
    /* textus plagulae; FALSUM si legi nequit */
            b32  (*plagulam_legere) (vacuum* datum, chorda via,
                                     Piscina* piscina, chorda* textus);
    /* summarium effectuum cursoris (crusta_effectus_derivare: elementum
     * 'effectus'); NIHIL = ignotum (lacuna nominata, non recusatio) */
      StmlNodus* (*effectus) (vacuum* datum, chorda cursor,
                              Piscina* piscina);
    /* litterae chordarum probationis (fons C) quae programmata vocat
     * (system, processus_*; lexemata silvae - commenta absunt, macrones
     * expansi): Xar de chorda, vacua = nihil vocat; NIHIL = ignotum */
            Xar* (*litterae_vocationum) (vacuum* datum, chorda fons,
                                         Piscina* piscina);
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


/* ==================================================
 * DERIVATIO (plan T2): quae via portam tangit, ex repositorio solo
 * ================================================== */

nomen enumeratio {
    PORTAE_EXEMPLAR_VIA = ZEPHYRUM,  /* via aequalis */
    PORTAE_EXEMPLAR_GLOBUS,          /* '*' '?' - '*' etiam '/' transit
                                      * (directorium_titulus_congruit,
                                      * ut fnmatch pythonicum) */
    PORTAE_EXEMPLAR_PRAEFIXUM        /* via sic incipit */
} PortaeExemplarGenus;

nomen structura {
                 chorda exemplar;
    PortaeExemplarGenus genus;
                 chorda ratio;     /* unde: "lectio (clang) in x.sh" */
} PortaeExemplar;

/* opertura portae declaratae unius. exemplaria = viae quas porta
 * DIRECTE tegit: cursor ipse; situs effectuum (lectio exsecutio
 * fontatio probatio enumeratio; classis arbor, resolutio plena;
 * custoditi praetereuntur - aedificatores '[ -x P ] || S' extra
 * catenam verdicti); scripta quae probationes vocant; tegit
 * annotationis. fontes_c = fontes C quorum CLAUSURAE (aedilis, T3)
 * tegunt - fons ipse et quae includit: probationes suitae, fontes quos
 * cursor compilat (situs .c, globi .c in viis arboris expansi), fontes
 * introitus programmatum quae probationes aut annotatio 'currit'
 * vocant. ignota = lacunae nominatae (effectus ignotum, litterae
 * ignotae, fons introitus ambiguus) - nihil tegunt; T3 eas nominat.
 * LACUNA NOTA: '-I' directoria (tabula mandatorum effectus ea non
 * notat) - 'tegit' annotationis ea portat. */
nomen structura {
    constans PortaDeclarata* declaratio;
                        Xar* exemplaria;  /* PortaeExemplar */
                        Xar* fontes_c;    /* PortaeExemplar (VIA) */
                        Xar* ignota;      /* chorda */
} PortaeOpertura;

/* Operturae declarationum omnium (Xar de PortaeOpertura, ordine
 * declaratarum; duplicata in eadem opertura semel, ratio prima).
 * NIHIL = memoria deficit aut enumeratio fracta. */
Xar*
portae_operturas_derivare (
    constans PortaeSutura* sutura,
             constans Xar* declaratae,
                  Piscina* piscina);

#endif /* PORTAE_H */
