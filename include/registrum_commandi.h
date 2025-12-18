#ifndef REGISTRUM_COMMANDI_H
#define REGISTRUM_COMMANDI_H

#include "latina.h"
#include "piscina.h"
#include "pagina.h"
#include "tabula_dispersa.h"

/* ==================================================
 * REGISTRUM COMMANDI - Command Registry System
 *
 * Central registry for all executable commands.
 * Commands can be invoked from text tags ($date, etc).
 *
 * PHILOSOPHIA:
 * - Commands are fundamental (not ad-hoc handlers)
 * - Registry enables discoverability
 * - Context provides access to state
 * - Easy to extend with new commands
 *
 * EXEMPLUM:
 *   RegistrumCommandi* reg = registrum_commandi_creare(piscina);
 *   registrum_commandi_registrare(reg, "date", command_date, NIHIL);
 *   registrum_commandi_executare(reg, "date", &ctx);
 *
 * ================================================== */


/* ==================================================
 * Command Context
 * ================================================== */

/* Context passed to all command functions */
nomen structura ContextusCommandi {
    Pagina* pagina;           /* Which pagina triggered command */
    i32 linea;                /* Grid row where command ends */
    i32 columna;              /* Grid column where command ends */
    Piscina* piscina;         /* Piscina for allocations */
    vacuum* datum_custom;     /* Call-site specific data (e.g., libro) */
    vacuum* datum_registratus;/* Data registered with command (e.g., schirmata) */
} ContextusCommandi;


/* ==================================================
 * Command Function Signature
 * ================================================== */

/* Command execution function
 *
 * ctx: command context
 *
 * Returns: VERUM if successful, FALSUM if error
 */
nomen b32 (*FunctioCommand)(ContextusCommandi* ctx);


/* ==================================================
 * Command Entry
 * ================================================== */

/* Registered command */
nomen structura Command {
    character titulus[XXXII]; /* Command name ("date", "time", etc) */
    FunctioCommand executare; /* Execution function */
    vacuum* datum;            /* Optional command-specific data */
} Command;


/* ==================================================
 * Registry
 * ================================================== */

/* Command registry */
nomen structura RegistrumCommandi {
    TabulaDispersa* tabula;  /* Hash table: chorda -> Command* */
    Piscina* piscina;        /* Piscina for allocations */
} RegistrumCommandi;


/* ==================================================
 * Registry - Creatio
 * ================================================== */

/* Create command registry
 *
 * piscina: piscina for allocations
 *
 * Returns: new registry, or NIHIL if failure
 */
RegistrumCommandi*
registrum_commandi_creare(
    Piscina* piscina);


/* ==================================================
 * Registry - Registratio
 * ================================================== */

/* Register a command
 *
 * reg: command registry
 * nomen_commandi: command name (e.g., "date")
 * functio: execution function
 * datum: optional command-specific data
 */
vacuum
registrum_commandi_registrare(
    RegistrumCommandi* reg,
    constans character* nomen_commandi,
    FunctioCommand functio,
    vacuum* datum);


/* ==================================================
 * Registry - Executio
 * ================================================== */

/* Execute a command by name
 *
 * reg: command registry
 * nomen_commandi: command name to execute
 * ctx: command context
 *
 * Returns: VERUM if command found and executed, FALSUM otherwise
 */
b32
registrum_commandi_executare(
    RegistrumCommandi* reg,
    constans character* nomen_commandi,
    ContextusCommandi* ctx);


/* ==================================================
 * Registry - Interrogatio
 * ================================================== */

/* Check if command exists
 *
 * reg: command registry
 * nomen_commandi: command name
 *
 * Returns: VERUM if command registered, FALSUM otherwise
 */
b32
registrum_commandi_habet(
    RegistrumCommandi* reg,
    constans character* nomen_commandi);

#endif /* REGISTRUM_COMMANDI_H */
