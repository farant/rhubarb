/* mcp_servus.h - nucleus servi MCP (Model Context Protocol)
 *
 * Servus tertius (decretum ...TKJ0): legatus et tabularium sceletum
 * MCP manu COPIAVERUNT; hic nucleus EXTRACTUS est, vecturae nescius.
 *
 *   TABULA INSTRUMENTORUM typata (staticae, applicationis): ex ea
 *   tools/list (inputSchema + annotationes) generatur, et omnis
 *   tools/call ANTE tractatorem iudicatur - clavis ignota,
 *   necessarium absens, genus alienum, valor extra electiones:
 *   error JSON-RPC -32602 argumentum nominans. Tractator argumenta
 *   valida sola videt.
 *
 *   SCOPI a nucleo imponuntur: persona scopo instrumenti carens ->
 *   tractator numquam currit, MCP_REDITUS_SCOPUS (HTTP 403, stdio
 *   error). tools/list omnia monstrat (step-up a cliente pendet).
 *
 *   RESPONSA DILATA: mcp_differre -> McpDilatum (locus + generatio,
 *   exemplar HospitiumDilatum); manubrium vetus nihil agit.
 *
 *   VECTURA: mcp_servus_tractare corpus JSON-RPC unum accipit et
 *   responsum unum per functionem 'reddere' reddit (nunc aut postea);
 *   stdio (lineae tabellarii) hic, HTTP in mcp_servus_http.
 *
 * Protocollum: initialize (protocolVersion nota reddita, aliter
 * novissima nostra), ping, tools/list, tools/call; nuntiationes
 * acceptae et omissae; fasces refutati.
 */

#ifndef MCP_SERVUS_H
#define MCP_SERVUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"

#include <stdio.h>

/* vocationes simul pendentes (dilatae et currentes) */
#define MCP_VOCATIONES_MAXIMAE LXIV

/* error JSON-RPC (servi, -32000..-32099): scopus deest */
#define MCP_ERROR_SCOPUS (-32003L)

/* protocollum novissimum nostrum */
#define MCP_PROTOCOLLUM_NOVISSIMUM "2025-11-25"


/* ==================================================
 * TABULA INSTRUMENTORUM
 * ================================================== */

/* nulli fluitantes: pecunia ut chorda "-12.34" */
nomen enumeratio {
    MCP_CHORDA = ZEPHYRUM,
    MCP_INTEGER,
    MCP_BOOLEAN
} McpGenus;

nomen structura {
               constans character* titulus;
               constans character* descriptio;
                         McpGenus  genus;
                              b32  necessarium;
    constans character* constans*  electiones;   /* NIHIL-terminata;
                                                  * NIHIL = liber;
                                                  * chordae solae */
} McpArgumentum;

nomen structura McpVocatio McpVocatio;

nomen vacuum (*McpTractator)(McpVocatio* vocatio);

nomen structura {
         constans character* titulus;
         constans character* descriptio;
     constans McpArgumentum* argumenta;
                        i32  numerus;
         constans character* scopus;          /* NIHIL = nullus */
                        b32  legere_solum;    /* readOnlyHint */
                        b32  delens;          /* destructiveHint */
               McpTractator  tractator;
} McpInstrumentum;

/* quis vocat: usor, cliens OAuth, scopi concessi (spatiis separati).
 * stdio: persona fixa localis. */
nomen structura {
    chorda usor;
    chorda cliens;
    chorda scopi;
} McpPersona;

nomen structura {
           constans character* titulus;        /* serverInfo.name */
           constans character* versio;         /* serverInfo.version */
           constans character* instructiones;  /* NIHIL = omissae */
     constans McpInstrumentum* instrumenta;
                          i32  numerus;
                       vacuum* datum;          /* applicationis */
} McpConfiguratio;


/* ==================================================
 * VECTURA (limes)
 * ================================================== */

nomen enumeratio {
    MCP_REDITUS_RESPONSUM = ZEPHYRUM,  /* corpus JSON-RPC (200) */
    MCP_REDITUS_NUNTIATIO,             /* sine corpore (202) */
    MCP_REDITUS_SCOPUS,                /* scopus deest: 403; corpus =
                                        * error JSON-RPC */
    MCP_REDITUS_PRAVUM                 /* JSON-RPC malum (400 +
                                        * corpus) */
} McpReditusGenus;

nomen structura {
        McpReditusGenus  genus;
                 chorda  corpus;   /* in piscina petitionis */
     constans character* scopus_requisitus;   /* SCOPUS solum */
    /* pro actis vecturae: titulus instrumenti (tools/call; aliter
     * NIHIL) et exitus: "bene", "vitium" (isError), "error" (JSON-RPC),
     * "scopus", "pravum", "nuntiatio" */
     constans character* instrumentum;
     constans character* exitus;
} McpReditus;

nomen vacuum (*McpReddere)(vacuum* datum, constans McpReditus* r);

nomen structura McpServus McpServus;

/* manubrium valoris: irritum post responsum aut loci reusum */
nomen structura {
    McpServus* servus;
          i32  locus;
          i32  generatio;
} McpDilatum;

/* configuratio transcribitur; tabula (instrumenta, argumenta,
 * chordae) NON - vivere debet quamdiu servus (staticae) */
McpServus*
mcp_servus_creare (
    constans McpConfiguratio* configuratio,
                     Piscina* piscina);

/* corpus JSON-RPC unum; 'reddere' SEMEL vocatur pro omni corpore
 * (etiam nuntiationibus, MCP_REDITUS_NUNTIATIO). initium_postulatum
 * (stdio): methodi praeter initialize/ping ante initialize -32002.
 * VERUM si responsum iam redditum; FALSUM = dilatum - piscina tunc
 * vivere debet donec reddere vocatur. */
b32
mcp_servus_tractare (
              McpServus* servus,
                 chorda  corpus,
    constans McpPersona* persona,
                    b32  initium_postulatum,
             McpReddere  reddere,
                 vacuum* datum_reddendi,
                Piscina* piscina);


/* ==================================================
 * VOCATIO (pro tractatore)
 * ================================================== */

constans McpPersona*
mcp_vocatio_persona (
    McpVocatio* vocatio);

vacuum*
mcp_vocatio_datum (
    McpVocatio* vocatio);

Piscina*
mcp_vocatio_piscina (
    McpVocatio* vocatio);

/* VERUM si argumentum adest (iam iudicatum: genus certum); null =
 * absens */
b32
mcp_argumentum_chorda (
             McpVocatio* vocatio,
     constans character* titulus,
                 chorda* exitus);

b32
mcp_argumentum_integer (
             McpVocatio* vocatio,
     constans character* titulus,
                    s64* exitus);

b32
mcp_argumentum_boolean (
             McpVocatio* vocatio,
     constans character* titulus,
                    b32* exitus);

/* responsa: semel; vocatio iam responsa nihil agit */
vacuum
mcp_respondere_textum (
    McpVocatio* vocatio,
        chorda  textus);

/* isError: instrumentum cucurrit et defecit (non error protocolli) */
vacuum
mcp_respondere_vitium (
    McpVocatio* vocatio,
        chorda  textus);

/* structuredContent + textus idem serializatus */
vacuum
mcp_respondere_json (
    McpVocatio* vocatio,
     JsonValor* valor);

/* tractator redit sine responso; manubrium servandum */
McpDilatum
mcp_differre (
    McpVocatio* vocatio);

/* vocatio manubrii, aut NIHIL si irritum (responsum, locus reusus) */
McpVocatio*
mcp_dilatum_vocatio (
    McpDilatum dilatum);


/* ==================================================
 * STDIO
 * ================================================== */

/* lineae tabellarii; initium postulatum; persona fixa. 0 ad EOF, I
 * si scriptura fracta */
s32
mcp_servus_stdio_currere (
              McpServus* servus,
                   FILE* intra,
                   FILE* extra,
    constans McpPersona* persona);

#endif /* MCP_SERVUS_H */
