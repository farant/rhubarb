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
#include "mcp_servus.h"
#include "admissio.h"
#include "filum.h"
#include "ianua.h"
#include "mcp_servus_canalis.h"
#include "exemplar_nucleus.h"
#include "communio_servus.h"

#define EXEMPLAR_PETITIO_MAXIMA  (LXIV * MXXIV * MXXIV)
#define EXEMPLAR_TEXTUS_MAXIMUS  (XVI * MXXIV)
#define EXEMPLAR_VIA_MAXIMA      CCLVI


/* ==================================================
 * STATUS
 * ================================================== */

nomen structura {
       s64 id;          /* seq acti nota.scripta */
    chorda civis;       /* ULID (ianua T5; acta vetera per mappam) */
    chorda textus;
    chorda sigillum;    /* fili, vacua si nullum */
    chorda titulus;
    chorda mimen;
       i64 mensura;
       b32 deleta;
} Nota;

structura Exemplar {
                 Piscina* piscina;
    ExemplarConfiguratio  cfg;
                 Reactor* reactor;
                     b32  reactor_proprius;
               Hospitium* hospitium;
                 Volumen* volumen;
                Custodia* custodia;
    /* rationes: cives, credentialia, invitationes, sessiones (T5) */
                   Ianua* ianua;
    /* sera scriptoris unius (data/sera - ianua P1) */
               FilumSera* sera;
                     Xar* notae;       /* Nota */
               character  via_temporaria[EXEMPLAR_VIA_MAXIMA];
    /* basis publica (auctoritas OAuth): -basis aut
     * http://<hospes>:<portus> (mcp-remotus P10) */
               character basis[EXEMPLAR_VIA_MAXIMA];
    /* servus MCP et admissio OAuth (exemplar_mcp.c, mcp-remotus T7) */
                McpServus* mcp;
                 Admissio* admissio;
    /* administratio per canalem localem (exemplar_administratio.c,
     * canalis T4): servus alter, numquam in /mcp */
                McpServus* administratio;
               McpCanalis* canalis;
    /* communio plan-2 T7: servus actorum (volumen proprium, C1) */
                  Volumen* communio_volumen;
           CommunioServus* communio;
};


/* ==================================================
 * AUXILIA COMMUNIA (exemplar_nucleus.c, pro exemplar_mcp.c)
 * ================================================== */

/* nota textus solius per MCP: actum nota.scripta cum via "claude" et
 * cliente; id (seq) aut 0 */
s64
exemplar_notam_scribere (
     Exemplar* e,
       chorda  civis,
       chorda  textus,
       chorda  cliens,
      Piscina* p);

/* nota civis delenda (actum nota.deleta); FALSUM si non eius aut
 * ignota */
b32
exemplar_notam_delere (
     Exemplar* e,
       chorda  civis,
          s64  id,
      Piscina* p);

#endif /* EXEMPLAR_INTERNUM_H */
