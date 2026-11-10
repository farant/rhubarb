/* vicus_sonus_auditus.c - porta soni vici super auditus (CoreAudio) */
#include "vicus_sonus_auditus.h"
#include "concentus_auditus.h"

b32
vicus_sonus_porta (
    VicusPortaSoni* porta,
           Piscina* piscina,
               i64  semen)
{
       MixturaForma  forma;
    ConcentusExitus* exitus;

    si (!porta || !piscina)
    {
        redde FALSUM;
    }
    forma.voces   = VICUS_SONUS_VOCES;
    forma.iussa   = VICUS_SONUS_ANULI;
    forma.nuntii  = VICUS_SONUS_ANULI;
    exitus        = concentus_exitus_auditus(piscina, &forma);
    redde exitus != NIHIL
        && vicus_sonus_porta_cum_exitu(porta, piscina, semen, exitus);
}
