/* statuae_repositorium.c - statuae in volumine (vide
 * statuae_repositorium.h) */

#include "statuae_repositorium.h"
#include "json.h"
#include <string.h>

b32
statuae_titulus_validus (
    chorda titulus)
{
    i32 k;

    si (   titulus.mensura < I
        || titulus.mensura > STATUAE_TITULUS_MAXIMUS || !titulus.datum)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < titulus.mensura; k++)
    {
        si (   titulus.datum[k] < 0x20 || titulus.datum[k] == 0x7F
            || titulus.datum[k] == '/')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* locus tituli in indice; -1 si abest */
interior s32
_locus (
       Xar* index,
    chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(index); k++)
    {
        si (chorda_aequalis(
            ((StatuaeInscriptio*)xar_obtinere(index, k))->titulus,
            titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* acta propria generum statuarum, ordine seq */
interior Xar*
_acta (
    Volumen* volumen,
    Piscina* piscina)
{
    hic_manens constans character* constans GENERA[] = {
        STATUAE_ACTUM_ADDITUM, STATUAE_ACTUM_REMOTUM, NIHIL
    };
    chorda replica = volumen_replica(volumen);

    si (replica.mensura > ZEPHYRUM)
    {
        redde volumen_acta_generum_replicae_legere(volumen, replica,
            ZEPHYRUM, GENERA, piscina);
    }
    redde volumen_acta_generum_legere(volumen, ZEPHYRUM, GENERA,
        piscina);
}

Xar*
statuae_enumerare (
    Volumen* volumen,
    Piscina* piscina)
{
                  Xar* index = xar_creare(piscina,
                      magnitudo(StatuaeInscriptio));
                  Xar* acta;
                  Xar* novus;
         VolumenActum* actum;
    StatuaeInscriptio* inscriptio;
         JsonResultus  r;
               chorda  titulus;
               chorda  massa;
                  s32  locus;
                  i32  k;
                  i32  j;

    si (!volumen || !index)
    {
        redde index;
    }
    acta = _acta(volumen, piscina);
    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        actum  = (VolumenActum*)xar_obtinere(acta, k);
        r      = json_legere(actum->datum, piscina);
        si (!r.successus || !r.radix)
        {
            perge;
        }
        titulus  = json_ad_chorda(json_objectum_capere(r.radix,
            "titulus"));
        massa    = json_ad_chorda(json_objectum_capere(r.radix,
            "massa"));
        si (!statuae_titulus_validus(titulus))
        {
            perge;
        }
        locus = _locus(index, titulus);
        si (chorda_aequalis_literis(actum->genus,
            STATUAE_ACTUM_ADDITUM))
        {
            si (massa.mensura == ZEPHYRUM)
            {
                perge;
            }
            inscriptio = locus >= ZEPHYRUM
                ? (StatuaeInscriptio*)xar_obtinere(index, (i32)locus)
                : (StatuaeInscriptio*)xar_addere(index);
            si (inscriptio)
            {
                inscriptio->titulus       = chorda_transcribere(titulus,
                    piscina);
                inscriptio->sigillum_hex  = chorda_transcribere(massa,
                    piscina);
            }
            perge;
        }
        si (locus < ZEPHYRUM)
        {
            perge;
        }
        /* remota: index sine ea refectus (ordo ceterarum manet) */
        novus = xar_creare(piscina, magnitudo(StatuaeInscriptio));
        per (j = ZEPHYRUM; novus && j < xar_numerus(index); j++)
        {
            si ((s32)j != locus)
            {
                inscriptio = (StatuaeInscriptio*)xar_addere(novus);
                si (inscriptio)
                {
                    *inscriptio = *(StatuaeInscriptio*)xar_obtinere(
                        index, j);
                }
            }
        }
        si (novus)
        {
            index = novus;
        }
    }
    redde index;
}

/* actum cum dato JSON appendere */
interior b32
_actum_scribere (
               Volumen* volumen,
    constans character* genus,
                chorda  titulus,
                chorda  massa,
               Piscina* piscina)
{
    JsonValor* objectum = json_objectum_creare(piscina);

    si (!objectum)
    {
        redde FALSUM;
    }
    json_objectum_ponere(objectum, "titulus",
        json_chorda_creare(piscina, titulus));
    si (massa.mensura > ZEPHYRUM)
    {
        json_objectum_ponere(objectum, "massa",
            json_chorda_creare(piscina, massa));
        json_objectum_ponere(objectum, "forma",
            json_chorda_creare_literis(piscina, "glb"));
    }
    redde volumen_actum_appendere(volumen, genus,
        json_scribere(objectum, piscina)) > ZEPHYRUM;
}

b32
statuae_addere (
    Volumen* volumen,
     chorda  titulus,
     chorda  octeti)
{
      Piscina* piscina;
          Xar* index;
    character  hex[SIGILLUM_HEX_MENSURA];
       chorda  massa;
          s32  locus;
          b32  propria;
          b32  bene;

    si (   !volumen || !statuae_titulus_validus(titulus)
        || octeti.mensura == ZEPHYRUM || !octeti.datum)
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("statuae_addere", 0x10000);
    si (!piscina)
    {
        redde FALSUM;
    }
    propria = !volumen_in_transactione(volumen);
    si (propria && !volumen_transactionem_incipere(volumen))
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    bene = volumen_massam_condere(volumen, octeti, hex);
    si (bene)
    {
        massa = chorda_ex_literis(hex, piscina);
        index = statuae_enumerare(volumen, piscina);
        locus = _locus(index, titulus);
        /* octeti iidem sub titulo eodem: nihil scribendum */
        si (   locus < ZEPHYRUM
            || !chorda_aequalis(((StatuaeInscriptio*)xar_obtinere(
                   index, (i32)locus))->sigillum_hex, massa))
        {
            bene = _actum_scribere(volumen, STATUAE_ACTUM_ADDITUM,
                titulus, massa, piscina);
        }
    }
    si (propria)
    {
        si (bene)
        {
            bene = volumen_transactionem_committere(volumen);
        }
        alioquin
        {
            (vacuum)volumen_transactionem_revolvere(volumen);
        }
    }
    piscina_destruere(piscina);
    redde bene;
}

b32
statuae_removere (
    Volumen* volumen,
     chorda  titulus)
{
    Piscina* piscina;
     chorda  nulla;
        b32  bene = VERUM;

    si (!volumen)
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("statuae_removere", 0x10000);
    si (!piscina)
    {
        redde FALSUM;
    }
    nulla.datum    = NIHIL;
    nulla.mensura  = ZEPHYRUM;
    si (_locus(statuae_enumerare(volumen, piscina), titulus)
        >= ZEPHYRUM)
    {
        bene = _actum_scribere(volumen, STATUAE_ACTUM_REMOTUM, titulus,
            nulla, piscina);
    }
    piscina_destruere(piscina);
    redde bene;
}
