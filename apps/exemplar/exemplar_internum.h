/* exemplar_internum.h - structurae exemplaris communes (internae)
 *
 * exemplar_nucleus.c (paginae, sessiones, notae) et exemplar_mcp.c
 * (servus MCP, admissio OAuth) eas legunt; consumptores exemplaris
 * exemplar_nucleus.h solum includunt (mcp-remotus P7).
 */
#ifndef EXEMPLAR_INTERNUM_H
#define EXEMPLAR_INTERNUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "hospitium.h"
#include "volumen.h"
#include "custodia.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include "exemplar_nucleus.h"

#define EXEMPLAR_PETITIO_MAXIMA  (LXIV * MXXIV * MXXIV)
#define EXEMPLAR_SESSIO_VITA_S   2592000      /* XXX dies */
#define EXEMPLAR_OPERA           LXIV
#define EXEMPLAR_NOMEN_MAXIMUM   XXXII
#define EXEMPLAR_TEXTUS_MAXIMUS  (XVI * MXXIV)
#define EXEMPLAR_VIA_MAXIMA      CCLVI


/* ==================================================
 * STATUS
 * ================================================== */

nomen structura {
    chorda nomen_usoris;
    chorda phc;
} Usor;

nomen structura {
    chorda usor;
       s64 expirat_ms;
} Sessio;

nomen structura {
       s64 id;          /* seq acti nota.scripta */
    chorda usor;
    chorda textus;
    chorda sigillum;    /* fili, vacua si nullum */
    chorda titulus;
    chorda mimen;
       i64 mensura;
       b32 deleta;
} Nota;

nomen enumeratio {
    OPUS_LIBERUM = 0,
    OPUS_INSCRIBERE,
    OPUS_INTRARE
} OpusGenus;

nomen structura {
             Exemplar* exemplar;
            OpusGenus  genus;
     HospitiumDilatum  d;
            character  nomen_usoris[EXEMPLAR_NOMEN_MAXIMUM + I];
                   i8  tessera[CUSTODIA_TESSERA_MAXIMA];
                  i32  tn;
            character  clavis_paris[LXXX];
            character  clavis_usoris[LXXX];
} Opus;

structura Exemplar {
                 Piscina* piscina;
    ExemplarConfiguratio  cfg;
                 Reactor* reactor;
                     b32  reactor_proprius;
               Hospitium* hospitium;
                 Volumen* volumen;
                Custodia* custodia;
          TabulaDispersa* usores;      /* nomen -> Usor* */
          TabulaDispersa* sessiones;   /* hex sigilli -> Sessio* */
                     Xar* notae;       /* Nota */
                    Opus  opera[EXEMPLAR_OPERA];
               character  via_temporaria[EXEMPLAR_VIA_MAXIMA];
    /* basis publica (auctoritas OAuth): -basis aut
     * http://<hospes>:<portus> (mcp-remotus P10) */
               character basis[EXEMPLAR_VIA_MAXIMA];
};

#endif /* EXEMPLAR_INTERNUM_H */
