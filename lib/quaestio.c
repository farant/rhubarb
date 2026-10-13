/* quaestio.c - quaestio una ad vates (vide caput) */

#include "quaestio.h"
#include "xar.h"

#include <stdio.h>
#include <string.h>

/* codex Unicode -> ASCII (NIHIL = '?') */
interior constans character*
ascii_substitutum (
    i32 codex)
{
    si (   codex == 0x2018 || codex == 0x2019 || codex == 0x201A
        || codex == 0x201B || codex == 0x2032)
    {
        redde "'";
    }
    si (   codex == 0x201C || codex == 0x201D || codex == 0x201E
        || codex == 0x2033)
    {
        redde "\"";
    }
    si (   codex == 0x2010 || codex == 0x2011 || codex == 0x2012
        || codex == 0x2013 || codex == 0x2014 || codex == 0x2212)
    {
        redde "-";
    }
    si (codex == 0x2026)
    {
        redde "...";
    }
    si (codex == 0x00A0 || codex == 0x2009 || codex == 0x202F)
    {
        redde " ";
    }
    si (codex == 0x2022)
    {
        redde "*";
    }
    redde NIHIL;
}

/* textus UTF-8 -> ASCII: '\r' tollitur, '\n' manet, imperia cetera ->
 * spatium, multi-octeti per ascii_substitutum aut '?' */
interior chorda
ad_ascii (
    chorda  textus,
   Piscina* piscina)
{
    chorda  c;
        i8* q;
       s32  i;
       s32  n;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    /* maximum: '...' ex III octetis = III - numquam longior */
    q = (i8*)piscina_allocare(piscina, (memoriae_index)(textus.mensura
        + I));
    si (!q)
    {
        redde c;
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (s32)textus.mensura; i++)
    {
        i8 b = textus.datum[i];

        si (b < 0x80)
        {
            si (b == (i8)'\r')
            {
                perge;
            }
            q[n++] = (b == (i8)'\n') ? b
                   : (b < 0x20 || b == 0x7F) ? (i8)' ' : b;
            perge;
        }
        {
            s32 longitudo = (b & 0xE0) == 0xC0 ? II
                          : (b & 0xF0) == 0xE0 ? III
                          : (b & 0xF8) == 0xF0 ? IV : I;
            i32 codex = longitudo == II ? (i32)(b & 0x1F)
                      : longitudo == III ? (i32)(b & 0x0F)
                      : (i32)(b & 0x07);
            constans character* sub;
                           s32  k;

            si (longitudo == I || i + longitudo > (s32)textus.mensura)
            {
                q[n++] = (i8)'?';
                perge;
            }
            per (k = I; k < longitudo; k++)
            {
                codex = (codex << VI) | (i32)(textus.datum[i
                    + k] & 0x3F);
            }
            i    += longitudo - I;
            sub  = ascii_substitutum(codex);
            si (!sub)
            {
                q[n++] = (i8)'?';
                perge;
            }
            per (k = ZEPHYRUM; sub[k]; k++)
            {
                q[n++] = (i8)sub[k];
            }
        }
    }
    c.datum    = q;
    c.mensura  = (i32)n;
    redde c;
}

interior QuaestioResponsum
errorem_reddere (
    constans character* causa,
               Piscina* piscina)
{
    QuaestioResponsum r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.error = chorda_ex_literis(causa, piscina);
    redde r;
}

QuaestioResponsum
quaestio_rogare (
        Vates* vates,
       chorda  exemplar,
       chorda  quaestio,
          s32  characteres,
          s32  latitudo,
      Piscina* piscina)
{
     QuaestioResponsum  r;
          VatesPetitio* petitio;
          VatesNuntius* nuntius;
        VatesResponsum* responsum;
             character  systema[CCCLXXXIV];
                chorda  textus;
                   i32  k;

    si (!piscina)
    {
        memset(&r, ZEPHYRUM, magnitudo(r));
        redde r;
    }
    si (!vates)
    {
        redde errorem_reddere("nullus vates", piscina);
    }
    si (quaestio.mensura <= ZEPHYRUM)
    {
        redde errorem_reddere("quaestio vacua", piscina);
    }
    si (characteres <= ZEPHYRUM)
    {
        redde errorem_reddere("spatium nullum", piscina);
    }
    petitio = vates_petitio_creare(piscina, exemplar.mensura > ZEPHYRUM
        ? exemplar : chorda_ex_literis(QUAESTIO_EXEMPLAR, piscina));
    /* signa ~ characteres / IV; margo pro verbis longis */
    vates_petitio_maxima_ponere(petitio, characteres / III + LXIV);
    sprintf(systema, "Answer in plain ASCII text only: no Markdown, no "
        "lists, no headings. The whole answer must fit in at most %d "
        "characters; it will be shown on a fixed-width page %d columns "
        "wide. Be direct.", (integer)characteres, (integer)latitudo);
    vates_systema_addere(petitio, chorda_ex_literis(systema, piscina),
        FALSUM);
    nuntius = vates_nuntium_addere(petitio, VATES_USOR);
    (vacuum)vates_textum_addere(nuntius, quaestio);
    responsum = vates_mittere(vates, petitio,
        chorda_ex_literis("quaestio",
        piscina), piscina);
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.usus = responsum->usus;
    si (!responsum->successus)
    {
        r.error =
            chorda_ex_literis(vates_error_descriptio(responsum->error),
            piscina);
        si (responsum->error_nuntius.mensura > ZEPHYRUM)
        {
            r.error = chorda_concatenare(r.error, chorda_concatenare(
                chorda_ex_literis(": ", piscina),
                responsum->error_nuntius,
                piscina), piscina);
        }
        redde r;
    }
    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    per (k = ZEPHYRUM; responsum->bloci
        && k < xar_numerus(responsum->bloci);
         k++)
    {
        /* bloci in Xar ut structurae (non indices - ut probatio_vates
         * legit), quamvis caput "VatesBlocus*" dicat */
        VatesBlocus* b = (VatesBlocus*)xar_obtinere(responsum->bloci,
            k);
        si (b && b->genus == VATES_TEXTUS)
        {
            textus = chorda_concatenare(textus, b->textus, piscina);
        }
    }
    si (textus.mensura <= ZEPHYRUM)
    {
        r.error = chorda_ex_literis(responsum->causa_finis
            == VATES_FINIS_RECUSATIO ? "recusatio" : "responsum vacuum",
            piscina);
        redde r;
    }
    r.textus     = ad_ascii(textus, piscina);
    r.successus  = VERUM;
    redde r;
}
