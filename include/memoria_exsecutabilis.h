/* memoria_exsecutabilis.h - memoria in qua codex nativus scribitur
 * et exsequitur (W^X)
 *
 * Reservatio una ad inscriptionem optatam (MAP_JIT); scriptura per
 * filum aperitur, sigillatio scripturam claudit ET icache invalidat
 * uno vocamine (invalidatio oblivioni dari non potest). Inscriptio
 * optata non data -> refusio, numquam sedes tacite mota (officina:
 * codex inter se per 'bl' vocat, inscriptiones deterministicae).
 *
 * Implementatio: lib/memoria_exsecutabilis_macos.c - SOLA plagula
 * cum MAP_JIT, pthread_jit_write_protect_np, sys_icache_invalidate.
 * Stratum independentiae: caput sine systemate.
 */

#ifndef MEMORIA_EXSECUTABILIS_H
#define MEMORIA_EXSECUTABILIS_H

#include "latina.h"
#include "chorda.h"

nomen structura MemoriaExsecutabilis MemoriaExsecutabilis;

/* Reservatio una ad inscriptio_optata (ZEPHYRUM = quaevis). NIHIL +
 * causa si systema aliam sedem dat (regio liberatur), si mmap
 * deficit, aut si mensura non multiplex paginae (XVI KiB). */
MemoriaExsecutabilis*
memoria_exsecutabilis_reservare (
           Piscina* piscina,
    memoriae_index  inscriptio_optata,
    memoriae_index  mensura_octetorum,
            chorda* causa_out);

vacuum*
memoria_exsecutabilis_basis (
    constans MemoriaExsecutabilis* m);

memoriae_index
memoria_exsecutabilis_mensura (
    constans MemoriaExsecutabilis* m);

/* scriptura permissa - filo vocanti solum (status per filum) */
vacuum
memoria_exsecutabilis_aperire (
    MemoriaExsecutabilis* m);

/* scriptura clausa + icache invalidata pro [offset, offset+mensura).
 * FALSUM si intervallum extra regionem (scriptura tamen clauditur,
 * nihil invalidatur). */
b32
memoria_exsecutabilis_sigillare (
    MemoriaExsecutabilis* m,
          memoriae_index  offset,
          memoriae_index  mensura);

/* regio redditur systemati; m post hoc invalidus */
vacuum
memoria_exsecutabilis_liberare (
    MemoriaExsecutabilis* m);

#endif /* MEMORIA_EXSECUTABILIS_H */
