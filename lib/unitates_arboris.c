/* unitates_arboris.c - sutura arboris verae (git + filum); caput
 * include/unitates_arboris.h */
#include "postulata_posix.h"

#include "unitates_arboris.h"
#include "filum.h"
#include "processus.h"
#include "chorda_aedificator.h"

interior Xar*
_enumerare (
      vacuum* datum,
     Piscina* piscina)
{
       constans character* argumenta[VII];
        ProcessusResultus  r;
    chorda_fissio_fructus  lineae;
                      Xar* viae;
                      i32  i;

    argumenta[ZEPHYRUM]  = "git";
    argumenta[I]         = "-C";
    argumenta[II]        = datum != NIHIL ? (constans character*)datum
                                          : ".";
    argumenta[III] = "ls-files";
    argumenta[IV] = "-co";
    argumenta[V] = "--exclude-standard";
    argumenta[VI] = NIHIL;
    r = processus_exsequi(argumenta, (i32)XXX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae  = chorda_fissio(r.effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (lineae.elementa[i].mensura > ZEPHYRUM)
        {
            *(chorda*)xar_addere(viae) = lineae.elementa[i];
        }
    }
    redde viae;
}

interior b32
_legere (
      vacuum* datum,
      chorda  via,
     Piscina* piscina,
      chorda* textus)
{
    constans character* v;

    si (datum != NIHIL)
    {
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            (memoriae_index)CCLVI);

        chorda_aedificator_appendere_literis(a, (constans character*)
            datum);
        chorda_aedificator_appendere_character(a, '/');
        chorda_aedificator_appendere_chorda(a, via);
        v = chorda_ut_cstr(chorda_aedificator_finire(a), piscina);
    }
    alioquin
    {
        v = chorda_ut_cstr(via, piscina);
    }
    si (!filum_existit(v))
    {
        redde FALSUM;
    }
    *textus = filum_legere_totum(v, piscina);
    redde VERUM;
}

UnitatesSutura*
unitates_sutura_arboris (
    constans character* radix,
               Piscina* piscina)
{
    UnitatesSutura* s = (UnitatesSutura*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(UnitatesSutura));

    s->datum               = radix != NIHIL
        ? (vacuum*)chorda_ut_cstr(chorda_ex_literis(radix, piscina),
            piscina)
        : NIHIL;
    s->plagulas_enumerare  = _enumerare;
    s->plagulam_legere     = _legere;
    redde s;
}
