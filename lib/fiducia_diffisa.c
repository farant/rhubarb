/* fiducia_diffisa.c - indicium diffisarum (vide fiducia_diffisa.h). fiducia_macos.m et fiducia_posix.c id PRIMUM
 * consulunt, ante iudicium systematis.
 *
 * Quisque certificatus catenae per fiducia_der in partes; emittens +
 * series aut subiectum + SHA-256(SPKI) contra tabulam octetim
 * comparantur. Perlustratio linearis (~MCCL diffisae x longitudo
 * catenae: microsecunda); tabula ordinata est, ergo quaestio binaria
 * postea possibilis sine forma nova. */
#include "fiducia_diffisa.h"
#include "fiducia_diffisa_tabula.h"
#include "fiducia_der.h"
#include "sigillum.h"
#include <stdio.h>
#include <string.h>

interior b32
_aequales (
    constans i8* a,
            i32  am,
    constans i8* b,
            i32  bm)
{
    redde am == bm && memcmp(a, b, (size_t)am) == ZEPHYRUM;
}

s32
fiducia_diffisam_invenire (
     constans FiduciaCertificatum* catena,
                              i32  numerus,
          constans FiduciaDiffisa* tabula,
                              i32  tabula_numerus,
                              i32* positio)
{
    i32 k;
    i32 j;

    si (catena == NIHIL || tabula == NIHIL)
    {
        redde -I;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
             FiduciaPartes  partes;
        constans character* causa = NIHIL;
                  Sigillum  clavis;

        si (!fiducia_partes_legere(catena[k].octeti, catena[k].mensura,
                &partes, &causa))
        {
            perge;
        }
        clavis = sigillum_computare(partes.clavis_publica.octeti,
            (memoriae_index)partes.clavis_publica.mensura);
        per (j = ZEPHYRUM; j < tabula_numerus; j++)
        {
             constans FiduciaDiffisa* d         = &tabula[j];
                                 b32  congruit  = FALSUM;

            si (d->genus == FIDUCIA_DIFFISA_EMITTENS_SERIES)
            {
                congruit = _aequales(d->primum, d->primum_mensura,
                               partes.emittens.octeti,
                               partes.emittens.mensura)
                           && _aequales(d->secundum,
                           d->secundum_mensura,
                               partes.series.octeti,
                               partes.series.mensura);
            }
            alioquin si (d->genus == FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS)
            {
                congruit = _aequales(d->primum, d->primum_mensura,
                               partes.subiectum.octeti,
                               partes.subiectum.mensura)
                           && _aequales(d->secundum,
                           d->secundum_mensura,
                               clavis.octeti, SIGILLUM_OCTETI);
            }
            si (congruit)
            {
                si (positio)
                {
                    *positio = k;
                }
                redde (s32)j;
            }
        }
    }
    redde -I;
}

b32
fiducia_diffisas_iudicare (
     constans FiduciaCertificatum* catena,
                              i32  numerus,
          constans FiduciaDiffisa* tabula,
                              i32  tabula_numerus,
                           chorda* causa,
                          Piscina* piscina)
{
          i32 positio = ZEPHYRUM;
          s32 index;
    character nuntius[CCCLXXX];

    si (tabula == NIHIL)
    {
        tabula          = fiducia_diffisae;
        tabula_numerus  = fiducia_diffisae_numerus;
    }
    index = fiducia_diffisam_invenire(catena, numerus, tabula,
        tabula_numerus, &positio);
    si (index < ZEPHYRUM)
    {
        redde VERUM;
    }
    si (causa && piscina)
    {
        sprintf(nuntius,
            "certificatum reiectum: clavis diffisa (catena [%u]): %.*s",
            (insignatus integer)positio, (integer)CCC,
            tabula[index].causa ? tabula[index].causa : "?");
        *causa = chorda_ex_literis(nuntius, piscina);
    }
    redde FALSUM;
}
