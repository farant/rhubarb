/* vates_r1 - LIMES via vates_vocare_rivum cum tabula timerum reactoris
 * plena: finis vocatur? (contractus capitis: ceteri defectus etiam LIMES
 * per finem, SEMEL) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "vates.h"
#include <stdio.h>

hic_manens i32 fines_numerus = 0;

interior vacuum
_nihil_agere (
    vacuum* datum)
{
    (vacuum)datum;
}

interior vacuum
_finis (
          vacuum* datum,
    VatesVocatio* vocatio)
{
    (vacuum)datum;
    (vacuum)vocatio;
    fines_numerus++;
}

interior vacuum
_experiri (
    b32 implere)
{
          Piscina* piscina = piscina_generare_dynamicum("sonda", 65536);
    VatesOptiones  o       = vates_optiones_ordinariae();
           Vates* v        = vates_fictus_aperire(piscina, &o);
         Reactor* rx       = reactor_creare(piscina);
    VatesPetitio* p        = vates_petitio_creare(piscina,
        chorda_ex_literis("m", piscina));
    VatesNuntius* n        = vates_nuntium_addere(p, VATES_USOR);
    VatesVocatio* vv;
              i32 i;
              i32 impleti = 0;

    fines_numerus = 0;
    per (i = 0; i < V; i++)
    {
        vates_textum_addere(n, chorda_ex_literis("x", piscina))
            ->signum_thesauri = VERUM;
    }
    si (implere)
    {
        per (i = 0; i < CXXVIII; i++)
        {
            si (reactor_timer_adicere(rx, 3600000, FALSUM, _nihil_agere,
                NIHIL) != REACTOR_TIMER_ERROR)
            {
                impleti++;
            }
        }
    }
    vv = vates_vocare_rivum(v, p, chorda_ex_literis("t", piscina), rx,
        NIHIL, NIHIL, _finis, NIHIL, piscina);
    per (i = 0; i < L; i++)
    {
        (vacuum)reactor_poll(rx, 0);
    }
    imprimere("implere=%d timers_impleti=%d vv=%s finita=%d fines=%d\n",
        (integer)implere, (integer)impleti, vv ? "non-NIHIL" : "NIHIL",
        (integer)vates_vocatio_finita(vv), (integer)fines_numerus);
    si (vv && vates_vocatio_responsum(vv))
    {
        imprimere("  error=%d\n",
            (integer)vates_vocatio_responsum(vv)->error);
    }
    /* abrumpere: cancellare id ERROR non debet ruere */
    vates_vocationem_abrumpere(vv);
    imprimere("  post abrumpere: finita=%d fines=%d\n",
        (integer)vates_vocatio_finita(vv), (integer)fines_numerus);
    reactor_destruere(rx);
    piscina_destruere(piscina);
}

integer
principale (vacuum)
{
    _experiri(FALSUM);
    _experiri(VERUM);
    redde 0;
}
