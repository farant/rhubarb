/* communio.c - acta et massae inter volumina (gradus I sine rete) */

#include "communio.h"
#include "communio_forma.h"
#include "json.h"
#include "xar.h"

#include <string.h>

/* chorda finitur in 'cauda'? */
interior b32
desinit (
                chorda  c,
    constans character* cauda)
{
    i32 n;

    n = (i32)strlen(cauda);
    redde c.mensura >= n
        && memcmp(c.datum + c.mensura - n, cauda, (size_t)n)
            == ZEPHYRUM;
}

/* derivatum (genera-datorum VIII-b, regula gradus I usque ad
 * registrum D6): plagula-condita cuius origo in ":checkpoint" desinit,
 * aut plagula-remota viae ".../checkpoint/<n>" */
b32
communio_actum_derivatum (
    constans VolumenActum* a,
                  Piscina* piscina)
{
    JsonResultus r;
          chorda vacua;
          chorda via;
             i32 i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (chorda_aequalis_literis(a->genus, "plagula-condita"))
    {
        r = json_legere(a->datum, piscina);
        redde r.successus && desinit(json_capere_chorda(r.radix,
            "origo", vacua), ":checkpoint");
    }
    si (chorda_aequalis_literis(a->genus, "plagula-remota"))
    {
        r = json_legere(a->datum, piscina);
        via = r.successus ? json_capere_chorda(r.radix, "via", vacua)
                          : vacua;
        per (i = ZEPHYRUM; i + XI <= via.mensura; i++)
        {
            si (   (i == ZEPHYRUM || via.datum[i - I] == '/')
                && memcmp(via.datum + i, "checkpoint/", XI) == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* massae quas datum nominat (series LXIV hex minusculorum exacta) et
 * quae in fonte sunt, in destinationem si desunt. FALSUM si missio
 * deficit. */
interior b32
massas_mittere (
                  Volumen* fons,
                  Volumen* destinatio,
    constans VolumenActum* actum,
          CommunioRelatio* relatio,
                  Piscina* piscina)
{
          Xar* nominatae;
       chorda  contentum;
          b32  inventum;
          i32  i;
    character  novum[SIGILLUM_HEX_MENSURA];

    nominatae = communio_massas_nominatas(actum, piscina);
    si (!nominatae)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nominatae); i++)
    {
        chorda hex = *(chorda*)xar_obtinere(nominatae, i);

        si (   !volumen_massam_habet(fons, hex)
            || volumen_massam_habet(destinatio, hex))
        {
            perge;
        }
        contentum = volumen_massam_promere(fons, hex, piscina,
            &inventum);
        si (   !inventum
            || !volumen_massam_condere(destinatio, contentum, novum)
            || memcmp(novum, hex.datum, (size_t)hex.mensura)
                   != ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (relatio)
        {
            relatio->massae++;
        }
    }
    redde VERUM;
}

b32
communio_transferre (
            Volumen* fons,
            Volumen* destinatio,
                s64* cursor,
    CommunioRelatio* relatio,
            Piscina* piscina)
{
       CommunioRelatio  vacua;
                   Xar* acta;
          VolumenActum* a;
     VolumenImportatio  exitus;
                   i32  i;

    si (relatio == NIHIL)
    {
        relatio = &vacua;
    }
    memset(relatio, ZEPHYRUM, magnitudo(CommunioRelatio));
    si (!fons || !destinatio || !cursor || !piscina)
    {
        redde FALSUM;
    }
    relatio->cursor = *cursor;
    acta = volumen_acta_propria_legere(fons, *cursor, piscina);
    si (acta == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        a = (VolumenActum*)xar_obtinere(acta, i);
        si (!communio_actum_derivatum(a, piscina))
        {
            si (!massas_mittere(fons, destinatio, a, relatio,
                    piscina))
            {
                relatio->refusa = I;
                redde FALSUM;
            }
            exitus = volumen_actum_importare(destinatio, a);
            si (exitus == VOLUMEN_IMPORTATUM)
            {
                relatio->importata++;
            }
            alioquin si (exitus == VOLUMEN_IAM_ADEST)
            {
                relatio->iam_aderant++;
            }
            alioquin
            {
                relatio->refusa = I;
                redde FALSUM;
            }
        }
        *cursor          = a->seq;
        relatio->cursor  = *cursor;
    }
    redde VERUM;
}
