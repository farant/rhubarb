/* communio.c - acta et massae inter volumina (gradus I sine rete) */

#include "communio.h"
#include "json.h"
#include "xar.h"

#include <string.h>

#define SIGILLUM_HEX_LONGITUDO LXIV

interior b32
hex_minuscula (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

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
interior b32
derivatum (
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
              chorda  datum,
     CommunioRelatio* relatio,
             Piscina* piscina)
{
       chorda hex;
       chorda contentum;
          b32 inventum;
          i32 i;
          i32 j;
    character novum[SIGILLUM_HEX_MENSURA];

    i = ZEPHYRUM;
    dum (i < datum.mensura)
    {
        si (!hex_minuscula(datum.datum[i]))
        {
            i++;
            perge;
        }
        j = i;
        dum (j < datum.mensura && hex_minuscula(datum.datum[j]))
        {
            j++;
        }
        si (j - i == SIGILLUM_HEX_LONGITUDO)
        {
            hex.datum    = datum.datum + i;
            hex.mensura  = SIGILLUM_HEX_LONGITUDO;
            si (   volumen_massam_habet(fons, hex)
                && !volumen_massam_habet(destinatio, hex))
            {
                contentum = volumen_massam_promere(fons, hex, piscina,
                    &inventum);
                si (   !inventum
                    || !volumen_massam_condere(destinatio, contentum,
                           novum)
                    || memcmp(novum, hex.datum,
                           (size_t)SIGILLUM_HEX_LONGITUDO) != ZEPHYRUM)
                {
                    redde FALSUM;
                }
                si (relatio)
                {
                    relatio->massae++;
                }
            }
        }
        i = j;
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
        si (!derivatum(a, piscina))
        {
            si (!massas_mittere(fons, destinatio, a->datum, relatio,
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
