/* crusta_fontationes.c - fontationes scripti: PROIECTIO summarii
 * effectuum (crusta_effectus.h; effectus-plan T4).
 *
 * Aestimator hic olim vivebat (fabrica spec 3 T2); in crusta_effectus
 * translatus est, et hoc nunc situs 'fontatio' et 'exsecutio' (per
 * syntaxim, non per tabulam) summarii in genera veterum reddit.
 * Interfacies et effusio IMMOTAE - probatio_crusta_fontationes et
 * effusio super scripta domus octeto tenus aequales manent (T4 gradus
 * 2). Tabula mandatorum VACUA datur: mandata externa genera haec non
 * tangunt (python3 x.py lectio scripti est, non fontatio), et
 * arbores fictae probationum tabulam non habent.
 *
 * Correspondentia (situs -> genus):
 *   resolutio nulla              IRRESOLUTUM (via = textus; 'absens:
 *                                via' ubi causa absens)
 *   resolutio partialis, build   PRODUCTUM 'dir/' + stella
 *   resolutio partialis, alia    IRRESOLUTUM (via = attributum textus)
 *   plena, externa/systema       fontatio EXTERNUM; exsecutio omittitur
 *   plena, build                 PRODUCTUM
 *   plena, arbor/instrumentum    fontatio FASCICULUS; exsecutio
 *                                FASCICULUS si processus suus exstat
 *                                (scriptum), aliter INSTRUMENTUM
 *   ignotum plagulae (illegibilis, parsura non sana)  IRRESOLUTUM
 *   radix processus omnis        FASCICULUS, linea 0 */

#include "latina.h"
#include "crusta_fontationes.h"
#include "crusta_effectus.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdlib.h>
#include <string.h>

hic_manens constans character* TABULA_VACUA =
    "<mandata lingua=\"bash\">"
    "<mandatum titulus=\"-\" purum=\"verum\"/>"
    "</mandata>";

interior character*
_attributum (
               Piscina* piscina,
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    redde v == NIHIL ? NIHIL : chorda_ut_cstr(*v, piscina);
}

interior b32
_aequat (
    constans character* a,
    constans character* b)
{
    redde a != NIHIL && strcmp(a, b) == ZEPHYRUM;
}

interior i32
_linea (
    constans character* sedes)
{
    si (sedes == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde (i32)strtol(sedes, NIHIL, X);
}

interior vacuum
_addere (
                    Xar* exitus,
    CrustaFontatioGenus  genus,
              character* via,
              character* plagula,
                    i32  linea,
                    b32  fontatum)
{
    CrustaFontatio* f = (CrustaFontatio*)xar_addere(exitus);

    f->genus     = genus;
    f->via       = via;
    f->plagula   = plagula;
    f->linea     = linea;
    f->fontatum  = fontatum;
}

/* via cum processu suo (scriptum exsecutum)? */
interior b32
_processus_est (
             StmlNodus* summarium,
               Piscina* piscina,
    constans character* via)
{
    i32 k;

    per (k = ZEPHYRUM; summarium->liberi
                       && k < xar_numerus(summarium->liberi); k++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, k);

        si (   p->genus == STML_NODUS_ELEMENTUM
            && _aequat(_attributum(piscina, p, "radix"), via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* situs unus -> genus (vide caput); FALSUM = situs non fontatio */
interior vacuum
_situm_proicere (
          Piscina* piscina,
        StmlNodus* summarium,
        StmlNodus* s,
              Xar* exitus)
{
    character* el         = chorda_ut_cstr(*s->titulus, piscina);
    character* via        = _attributum(piscina, s, "via");
    character* resolutio  = _attributum(piscina, s, "resolutio");
    character* classis    = _attributum(piscina, s, "classis");
    character* causa      = _attributum(piscina, s, "causa");
    character* plagula    = _attributum(piscina, s, "plagula");
          i32  linea      = _linea(_attributum(piscina, s, "sedes"));
          b32  fontatum   = _aequat(el, "fontatio");

    si (_aequat(el, "ignotum"))
    {
        si (   _attributum(piscina, s, "mandatum") == NIHIL
            && (_aequat(causa, "illegibilis")
                || _aequat(causa, "parsura non sana")))
        {
            _addere(exitus, CRUSTA_FONTATIO_IRRESOLUTUM, causa, plagula,
                ZEPHYRUM, FALSUM);
        }
        redde;
    }
    si (   !fontatum
        && !(_aequat(el, "exsecutio")
             && _aequat(_attributum(piscina, s, "per"), "aedificium")))
    {
        redde;
    }
    si (_aequat(resolutio, "nulla"))
    {
        si (_aequat(causa, "absens"))
        {
            character* t = (character*)piscina_allocare(piscina,
                strlen(via) + IX);

            strcpy(t, "absens: ");
            strcat(t, via);
            via = t;
        }
        _addere(exitus, CRUSTA_FONTATIO_IRRESOLUTUM, via, plagula,
            linea,
            fontatum);
        redde;
    }
    si (_aequat(resolutio, "partialis"))
    {
        si (_aequat(classis, "build"))
        {
            memoriae_index  n = strlen(via);
                 character* t = (character*)piscina_allocare(piscina,
                                   n + III);

            strcpy(t, via);
            si (n > ZEPHYRUM && t[n - I] == '/')
            {
                t[n - I] = '\0';
            }
            strcat(t, "/*");
            _addere(exitus, CRUSTA_FONTATIO_PRODUCTUM, t, plagula,
                linea,
                fontatum);
        }
        alioquin
        {
            _addere(exitus, CRUSTA_FONTATIO_IRRESOLUTUM,
                _attributum(piscina, s, "textus"), plagula, linea,
                fontatum);
        }
        redde;
    }
    si (_aequat(classis, "externa") || _aequat(classis, "systema"))
    {
        si (fontatum)
        {
            _addere(exitus, CRUSTA_FONTATIO_EXTERNUM, via, plagula,
                linea,
                fontatum);
        }
        redde;   /* exsecutum externum: identitas alibi */
    }
    si (_aequat(classis, "build"))
    {
        _addere(exitus, CRUSTA_FONTATIO_PRODUCTUM, via, plagula, linea,
            fontatum);
        redde;
    }
    _addere(exitus,
        fontatum || _processus_est(summarium, piscina, via)
            ? CRUSTA_FONTATIO_FASCICULUS : CRUSTA_FONTATIO_INSTRUMENTUM,
        via, plagula, linea, fontatum);
}

interior s32
_fontationes_comparare (
    constans vacuum* x,
    constans vacuum* y)
{
    constans CrustaFontatio* a = (constans CrustaFontatio*)x;
    constans CrustaFontatio* b = (constans CrustaFontatio*)y;

    s32 c;

    si (a->genus != b->genus)
    {
        redde a->genus < b->genus ? -I : I;
    }
    c = (s32)strcmp(a->via, b->via);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    /* unicum servat primum: locus verus (linea > 0) ante radicem */
    si ((a->linea == ZEPHYRUM) != (b->linea == ZEPHYRUM))
    {
        redde a->linea == ZEPHYRUM ? I : -I;
    }
    redde ZEPHYRUM;
}

constans character*
crusta_fontatio_genus_titulus (
    CrustaFontatioGenus genus)
{
    commutatio (genus)
    {
        casus CRUSTA_FONTATIO_FASCICULUS:   redde "fasciculus";
        casus CRUSTA_FONTATIO_INSTRUMENTUM: redde "instrumentum";
        casus CRUSTA_FONTATIO_PRODUCTUM:    redde "productum";
        casus CRUSTA_FONTATIO_EXTERNUM:     redde "externum";
        casus CRUSTA_FONTATIO_IRRESOLUTUM:  redde "irresolutum";
    }
    redde "?";
}

Xar*
crusta_fontationes_derivare (
               Piscina*  piscina,
    constans character*  radix,
    constans character*  scriptum,
    constans character** causa_out)
{
    InternamentumChorda* intern;
           StmlResultus  tabula;
              StmlNodus* summarium;
                    Xar* exitus;
                    Xar* unica;
                    i32  k;
                    i32  j;

    si (causa_out != NIHIL)
    {
        *causa_out = NIHIL;
    }
    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    tabula = stml_legere_ex_literis(TABULA_VACUA, piscina, intern);
    si (!tabula.successus)
    {
        redde NIHIL;
    }
    summarium = crusta_effectus_derivare(piscina, intern, radix,
        scriptum, tabula.elementum_radix, causa_out);
    exitus = xar_creare(piscina, (i32)magnitudo(CrustaFontatio));
    si (summarium == NIHIL || exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; summarium->liberi
                       && k < xar_numerus(summarium->liberi); k++)
    {
        StmlNodus* p = *(StmlNodus**)xar_obtinere(summarium->liberi, k);
        character* r;

        si (p->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        r = _attributum(piscina, p, "radix");
        si (r != NIHIL && r[ZEPHYRUM] != '/')
        {
            _addere(exitus, CRUSTA_FONTATIO_FASCICULUS, r, r, ZEPHYRUM,
                FALSUM);
        }
        per (j = ZEPHYRUM; p->liberi && j < xar_numerus(p->liberi); j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(p->liberi, j);

            si (s->genus == STML_NODUS_ELEMENTUM)
            {
                _situm_proicere(piscina, summarium, s, exitus);
            }
        }
    }
    xar_ordinare(exitus, _fontationes_comparare);
    unica = xar_creare(piscina, (i32)magnitudo(CrustaFontatio));
    si (unica == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(exitus); k++)
    {
        CrustaFontatio* f = (CrustaFontatio*)xar_obtinere(exitus, k);

        si (xar_numerus(unica) > ZEPHYRUM)
        {
            CrustaFontatio* u = (CrustaFontatio*)xar_obtinere(unica,
                xar_numerus(unica) - I);

            si (   u->genus               == f->genus
                && strcmp(u->via, f->via) == ZEPHYRUM)
            {
                perge;
            }
        }
        *(CrustaFontatio*)xar_addere(unica) = *f;
    }
    redde unica;
}
