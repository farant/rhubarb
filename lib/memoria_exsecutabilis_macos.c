/* memoria_exsecutabilis_macos.c - memoria exsecutabilis per MAP_JIT
 *
 * SOLA plagula domus cum MAP_JIT, pthread_jit_write_protect_np et
 * sys_icache_invalidate (spec officina M5 F18). Vide
 * memoria_exsecutabilis.h.
 *
 * W^X in Apple silicio: regio MAP_JIT RWX mappatur, sed scriptura
 * per filum regitur - pthread_jit_write_protect_np(0) scribere
 * sinit filo vocanti, (1) exsequi. Post scripturam icache invalidanda
 * est (aliter codex vetus exsequi potest); ergo sigillare utrumque
 * facit.
 *
 * FURCA: status scripturae per filum a fork ex statu SERVATO parentis
 * (commutatio contextus ultima) in filium transit, non ex vivo. Parens
 * intra fenestram scripturae ultimo commutatus filium in modo
 * scripturae gignit - pagina ei non exsecutabilis (SIGBUS, vitium
 * permissionis in instructione). Ergo status logicus hic servatur et
 * manipulus pthread_atfork eum in filio restituit. Domus sine filis:
 * status unus sufficit. Vide worklog 2026-11-10.
 */
#include "postulata_posix.h"
#include "memoria_exsecutabilis.h"
#include <sys/mman.h>
#include <pthread.h>
#include <libkern/OSCacheControl.h>

#define MEMORIA_EXSECUTABILIS_PAGINA 0x4000

structura MemoriaExsecutabilis {
            vacuum* basis;
    memoriae_index  mensura;
};

/* status logicus scripturae (aperire/sigillare) et manipulus furcae */
hic_manens b32 _scriptura_aperta = FALSUM;
hic_manens b32 _furca_instituta  = FALSUM;

/* in filio post fork: status logicus parentis restituitur */
interior vacuum
_in_filio (vacuum)
{
    pthread_jit_write_protect_np(_scriptura_aperta ? ZEPHYRUM : I);
}

interior vacuum
_memoriae_causam_ponere (
                chorda* causa_out,
               Piscina* piscina,
    constans character* textus)
{
    si (causa_out != NIHIL)
    {
        *causa_out = chorda_ex_literis(textus, piscina);
    }
}

MemoriaExsecutabilis*
memoria_exsecutabilis_reservare (
           Piscina* piscina,
    memoriae_index  inscriptio_optata,
    memoriae_index  mensura_octetorum,
            chorda* causa_out)
{
    MemoriaExsecutabilis* m;
                  vacuum* locus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (   mensura_octetorum == ZEPHYRUM
        || (mensura_octetorum % MEMORIA_EXSECUTABILIS_PAGINA)
           != ZEPHYRUM)
    {
        _memoriae_causam_ponere(causa_out, piscina,
            "mensura non multiplex paginae (XVI KiB)");
        redde NIHIL;
    }
    si ((inscriptio_optata % MEMORIA_EXSECUTABILIS_PAGINA)
        != ZEPHYRUM)
    {
        _memoriae_causam_ponere(causa_out, piscina,
            "inscriptio optata non in initio paginae");
        redde NIHIL;
    }
    si (!_furca_instituta)
    {
        si (pthread_atfork(NIHIL, NIHIL, _in_filio) != ZEPHYRUM)
        {
            _memoriae_causam_ponere(causa_out, piscina,
                "pthread_atfork defecit");
            redde NIHIL;
        }
        _furca_instituta = VERUM;
    }
    locus = mmap((vacuum*)inscriptio_optata, mensura_octetorum,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANON | MAP_JIT, -I, ZEPHYRUM);
    si (locus == MAP_FAILED)
    {
        _memoriae_causam_ponere(causa_out, piscina,
            "mmap MAP_JIT defecit");
        redde NIHIL;
    }
    si (   inscriptio_optata     != ZEPHYRUM
        && (memoriae_index)locus != inscriptio_optata)
    {
        (vacuum)munmap(locus, mensura_octetorum);
        _memoriae_causam_ponere(causa_out, piscina,
            "systema sedem aliam dedit (inscriptio optata occupata)");
        redde NIHIL;
    }
    m = (MemoriaExsecutabilis*)piscina_allocare(piscina,
        magnitudo(MemoriaExsecutabilis));
    si (m == NIHIL)
    {
        (vacuum)munmap(locus, mensura_octetorum);
        _memoriae_causam_ponere(causa_out, piscina, "piscina defecit");
        redde NIHIL;
    }
    m->basis    = locus;
    m->mensura  = mensura_octetorum;
    redde m;
}

vacuum*
memoria_exsecutabilis_basis (
    constans MemoriaExsecutabilis* m)
{
    redde (m == NIHIL) ? NIHIL : m->basis;
}

memoriae_index
memoria_exsecutabilis_mensura (
    constans MemoriaExsecutabilis* m)
{
    redde (m == NIHIL) ? ZEPHYRUM : m->mensura;
}

vacuum
memoria_exsecutabilis_aperire (
    MemoriaExsecutabilis* m)
{
    si (m != NIHIL)
    {
        _scriptura_aperta = VERUM;
        pthread_jit_write_protect_np(ZEPHYRUM);
    }
}

b32
memoria_exsecutabilis_sigillare (
    MemoriaExsecutabilis* m,
          memoriae_index  offset,
          memoriae_index  mensura)
{
    si (m == NIHIL)
    {
        redde FALSUM;
    }
    _scriptura_aperta = FALSUM;
    pthread_jit_write_protect_np(I);
    si (offset > m->mensura || mensura > m->mensura - offset)
    {
        redde FALSUM;
    }
    sys_icache_invalidate((character*)m->basis + offset, mensura);
    redde VERUM;
}

vacuum
memoria_exsecutabilis_liberare (
    MemoriaExsecutabilis* m)
{
    si (m == NIHIL || m->basis == NIHIL)
    {
        redde;
    }
    (vacuum)munmap(m->basis, m->mensura);
    m->basis    = NIHIL;
    m->mensura  = ZEPHYRUM;
}
