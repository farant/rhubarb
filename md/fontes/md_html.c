/* md_html.c - markdown -> html per compositionem STML (vide md_html.h) */

#include "md_html.h"
#include "md_arbor.h"
#include "md_stml.h"
#include "md_lexicon.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "stml.h"
#include "stml_macros.h"
#include "stml_html.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

/* cauda documenti compositi: captura documenti + fragmentum html */
hic_manens constans character CAUDA_COMPOSITI[] =
    "\n<EXEMPLAR modus=\"unum\" output=\"$doc\"><documentum $d/></EXEMPLAR>\n"
    "<html-fragmentum><PER congruentia=\"$doc\">"
    "<<#@md-nodus>><@n=>&@d;</></PER></html-fragmentum>\n"
    "</radix>\n";

interior chorda
_vacua (
    vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

/* causa: litterae + detail (chorda, licet vacua) + numerus (si >= 0) */
interior chorda
_causa (
               Piscina* piscina,
    constans character* litterae,
                chorda  detail,
                   s32  numerus)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        CXXVIII);
            character buffer[XXXII];

    si (aed == NIHIL)
    {
        redde _vacua();
    }
    chorda_aedificator_appendere_literis(aed, litterae);
    si (detail.datum != NIHIL && detail.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, " '");
        chorda_aedificator_appendere_chorda(aed, detail);
        chorda_aedificator_appendere_literis(aed, "'");
    }
    si (numerus >= ZEPHYRUM)
    {
        sprintf(buffer, " (%d)", (integer)numerus);
        chorda_aedificator_appendere_literis(aed, buffer);
    }
    redde chorda_aedificator_finire(aed);
}

interior MdHtmlResultus
_fractum (
    MdHtmlResultus r,
            chorda causa)
{
    r.successus  = FALSUM;
    r.html       = _vacua();
    r.causa      = causa;
    redde r;
}

/* elementum titulo quaerere (ordine documenti, primum) */
interior StmlNodus*
_elementum_quaerere (
             StmlNodus* n,
    constans character* titulus)
{
    i32 i;
    i32 num;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (   n->genus   == STML_NODUS_ELEMENTUM
        && n->titulus != NIHIL
        && chorda_aequalis_literis(*n->titulus, titulus))
    {
        redde n;
    }
    num = n->liberi != NIHIL ? xar_numerus(n->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* inventum = _elementum_quaerere(
            *(StmlNodus**)xar_obtinere(n->liberi, i), titulus);

        si (inventum != NIHIL)
        {
            redde inventum;
        }
    }
    redde NIHIL;
}

MdHtmlResultus
md_html_reddere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons,
                    i32  mensura,
                 chorda  programma)
{
             MdHtmlResultus r;
        MateriaLexiconRatum ratum;
         MateriaLexIudicium iudicium;
      MateriaArborConsilium consilium;
               MateriaNodus* radix;
      MateriaArborScriptura s;
          ChordaAedificator* aed;
               StmlResultus lectum;
       StmlExpansioResultus expansio;
    StmlDistributioResultus distributio;
                 StmlNodus* fragmentum;
           StmlHtmlResultus h;

    r.successus  = FALSUM;
    r.html       = _vacua();
    r.causa      = _vacua();
    r.stml       = _vacua();

    si (!materia_lexicon_ratum_facere(&ratum, &MD_LEXICON, &iudicium))
    {
        redde _fractum(r, _causa(piscina, "lexicon md recusatum",
            _vacua(), (s32)-I));
    }
    md_stml_consilium(&consilium, &ratum);
    radix = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde _fractum(r, _causa(piscina, "parsura md fracta", _vacua(),
            (s32)-I));
    }
    s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    si (!s.successus)
    {
        redde _fractum(r, _causa(piscina, "proiectio STML fracta:",
            chorda_ex_literis(s.causa != NIHIL ? s.causa : "-",
            piscina),
            (s32)-I));
    }

    /* compositio: <radix> programma proiectio cauda */
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(programma.mensura + s.textus.mensura + DXII));
    si (aed == NIHIL)
    {
        redde _fractum(r, _causa(piscina, "memoria", _vacua(),
            (s32)-I));
    }
    chorda_aedificator_appendere_literis(aed, "<radix>\n");
    chorda_aedificator_appendere_chorda(aed, programma);
    chorda_aedificator_appendere_literis(aed, "\n");
    chorda_aedificator_appendere_chorda(aed, s.textus);
    chorda_aedificator_appendere_literis(aed, CAUDA_COMPOSITI);
    r.stml = chorda_aedificator_finire(aed);

    lectum = stml_legere(r.stml, piscina, intern);
    si (!lectum.successus)
    {
        redde _fractum(r, _causa(piscina,
            "lectio STML compositi fracta:",
            lectum.error, (s32)lectum.linea_erroris));
    }
    expansio = stml_expandere(lectum.radix, piscina, intern);
    si (!expansio.successus)
    {
        chorda detail = expansio.loculus.datum != NIHIL
                        && expansio.loculus.mensura > ZEPHYRUM
            ? expansio.loculus : expansio.fragmentum;

        redde _fractum(r, _causa(piscina, "expansio: vitium", detail,
            (s32)expansio.vitium));
    }
    distributio = stml_distribuere(expansio.radix_expansa, piscina,
        intern);
    si (!distributio.successus)
    {
        redde _fractum(r, _causa(piscina, "distributio: vitium",
            _vacua(),
            (s32)distributio.vitium));
    }
    fragmentum = _elementum_quaerere(distributio.radix_distributa,
        "html-fragmentum");
    si (fragmentum == NIHIL)
    {
        redde _fractum(r, _causa(piscina, "fragmentum html absens",
            _vacua(), (s32)-I));
    }
    h = stml_html_vertere_liberos(fragmentum, piscina);
    si (!h.successus)
    {
        redde _fractum(r, _causa(piscina, "versio html:",
            chorda_ex_literis(stml_html_vitium_titulus(h.vitium),
            piscina), (s32)-I));
    }
    r.successus  = VERUM;
    r.html       = h.html;
    redde r;
}
