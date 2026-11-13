/* scaena_repositorium.c - prospectus in volumine (vide
 * scaena_repositorium.h) */

#include "scaena_repositorium.h"
#include "scaena.h"
#include "json.h"
#include <string.h>

b32
scaena_titulus_validus (
    chorda titulus)
{
    i32 k;

    si (   titulus.mensura < I
        || titulus.mensura > SCAENA_TITULUS_MAXIMUS || !titulus.datum)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < titulus.mensura; k++)
    {
        i8 littera = titulus.datum[k];

        si (   !(littera >= 'a' && littera <= 'z')
            && !(littera >= '0' && littera <= '9') && littera != '-'
            && littera != '_')
        {
            redde FALSUM;
        }
    }
    redde !chorda_aequalis_literis(titulus, SCAENA_EXITUS_MUNDUS);
}

/* locus tituli in indice; -1 si abest */
interior s32
_locus (
       Xar* index,
    chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        si (chorda_aequalis(
            ((ScaenaInscriptio*)xar_obtinere(index, k))->titulus,
            titulus))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* acta propria generum scaenae, ordine seq */
interior Xar*
_acta (
    Volumen* volumen,
    Piscina* piscina)
{
    hic_manens constans character* constans GENERA[] = {
        SCAENA_ACTUM_ADDITUM, SCAENA_ACTUM_REMOTUM, NIHIL
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
scaena_enumerare (
    Volumen* volumen,
    Piscina* piscina)
{
                 Xar* index = xar_creare(piscina,
                     magnitudo(ScaenaInscriptio));
                 Xar* acta;
                 Xar* novus;
        VolumenActum* actum;
    ScaenaInscriptio* inscriptio;
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
        si (!scaena_titulus_validus(titulus))
        {
            perge;
        }
        locus = _locus(index, titulus);
        si (chorda_aequalis_literis(actum->genus,
            SCAENA_ACTUM_ADDITUM))
        {
            si (massa.mensura == ZEPHYRUM)
            {
                perge;
            }
            /* titulus notus: locus in ordine manet */
            inscriptio = locus >= ZEPHYRUM
                ? (ScaenaInscriptio*)xar_obtinere(index, (i32)locus)
                : (ScaenaInscriptio*)xar_addere(index);
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
        novus = xar_creare(piscina, magnitudo(ScaenaInscriptio));
        per (j = ZEPHYRUM; novus && j < xar_numerus(index); j++)
        {
            si ((s32)j != locus)
            {
                inscriptio = (ScaenaInscriptio*)xar_addere(novus);
                si (inscriptio)
                {
                    *inscriptio = *(ScaenaInscriptio*)xar_obtinere(
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

/* actum cum dato JSON appendere; massa vacua = sine campo massae */
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
    }
    redde volumen_actum_appendere(volumen, genus,
        json_scribere(objectum, piscina)) > ZEPHYRUM;
}

b32
scaena_addere (
    Volumen* volumen,
     chorda  titulus,
     chorda  massa)
{
      Piscina* piscina;
          Xar* index;
    character  hex[SIGILLUM_HEX_MENSURA];
       chorda  sigillum;
          s32  locus;
          b32  propria;
          b32  bene;

    si (   !volumen || !scaena_titulus_validus(titulus)
        || massa.mensura == ZEPHYRUM || !massa.datum)
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("scaena_addere", 0x10000);
    si (!piscina)
    {
        redde FALSUM;
    }
    /* massa et actum in transactione una (vocatoris, si tenet) */
    propria = !volumen_in_transactione(volumen);
    si (propria && !volumen_transactionem_incipere(volumen))
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    bene = volumen_massam_condere(volumen, massa, hex);
    si (bene)
    {
        sigillum  = chorda_ex_literis(hex, piscina);
        index     = scaena_enumerare(volumen, piscina);
        locus     = _locus(index, titulus);
        /* octeti iidem sub titulo eodem: nihil scribendum */
        si (   locus < ZEPHYRUM
            || !chorda_aequalis(((ScaenaInscriptio*)xar_obtinere(index,
                   (i32)locus))->sigillum_hex, sigillum))
        {
            bene = _actum_scribere(volumen, SCAENA_ACTUM_ADDITUM,
                titulus, sigillum, piscina);
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
scaena_removere (
    Volumen* volumen,
     chorda  titulus)
{
    Piscina* piscina;
     chorda  nulla;
        b32  bene = FALSUM;

    si (!volumen || !scaena_titulus_validus(titulus))
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("scaena_removere", 0x10000);
    si (!piscina)
    {
        redde FALSUM;
    }
    nulla.datum    = NIHIL;
    nulla.mensura  = ZEPHYRUM;
    si (_locus(scaena_enumerare(volumen, piscina), titulus)
        >= ZEPHYRUM)
    {
        bene = _actum_scribere(volumen, SCAENA_ACTUM_REMOTUM, titulus,
            nulla, piscina);
    }
    piscina_destruere(piscina);
    redde bene;
}

chorda
scaena_promere (
    Volumen* volumen,
     chorda  titulus,
    Piscina* piscina)
{
       Xar* index;
    chorda  massa;
    chorda  vacua;
       s32  locus;
       b32  inventum = FALSUM;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!volumen || !piscina)
    {
        redde vacua;
    }
    index = scaena_enumerare(volumen, piscina);
    locus = _locus(index, titulus);
    si (locus < ZEPHYRUM)
    {
        redde vacua;
    }
    massa = volumen_massam_promere(volumen,
        ((ScaenaInscriptio*)xar_obtinere(index, (i32)locus))
            ->sigillum_hex, piscina, &inventum);
    redde inventum ? massa : vacua;
}
