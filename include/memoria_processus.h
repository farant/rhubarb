/* memoria_processus.h - memoria processus legenda (vestigium, apex)
 *
 * Vestigium (footprint) = memoria quam processus revera tenet,
 * compressa inclusa: RSS sub compressione macOS MENTITUR (2026-10-19:
 * probatio_arbor_syntaxis XLIX G compressorum, RSS CCLXI M). Apex vitae
 * numquam reficitur - in processu uno sectio solum quantum apicem
 * AUXERIT nominare potest (project-specs/credo-memoria-spec.md
 * par. II).
 *
 * Implementatio: lib/memoria_processus_macos.c (proc_pid_rusage,
 * RUSAGE_INFO_V4). Stratum independentiae: caput sine systemate.
 */

#ifndef MEMORIA_PROCESSUS_H
#define MEMORIA_PROCESSUS_H

#include "latina.h"

nomen structura {
    i64 vestigium;        /* octeti nunc tenti (compressi inclusi) */
    i64 vestigium_apex;   /* apex vestigii a creatione processus */
    i64 residens;         /* RSS - ad comparandum solum, non fidendum */
} MemoriaProcessus;

/* memoriam processus 'pid' legere (ZEPHYRUM = processus ipse).
 * FALSUM si legi nequit (pid ignotus, negativus, permissio, exitus
 * NIHIL); exitus tum in zephyrum ponitur. */
b32
memoria_processus_legere (
                  s32  pid,
     MemoriaProcessus* exitus);

#endif /* MEMORIA_PROCESSUS_H */
