/* portae.c - PORTAE DECLARATAE: lector declarationum <porta>
 * (plan portae-declaratae T1; caput include/portae.h)
 *
 * Commentaria per crustam inveniuntur (materia_annotationes super
 * arbore crustae - '#' intra chordam commentarium non est, prosa non
 * annotatio est). Praecribrum: plagula sine litteris '<porta' non
 * parsatur. Recusatio post lectionem TOTAM: causae omnes una
 * nominantur.
 */
#include "portae.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "stml.h"
#include "materia_annotationes.h"
#include "materia_lexicon.h"
#include "crusta_arbor.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include <stdio.h>
#include <string.h>

/* vocabulum CLAUSUM attributorum <porta> */
hic_manens constans character* constans _portae_attributa[] = {
    "titulus", "manu", "causa", "tegit", "currit", NIHIL
};

/* './x' -> 'x' (registrum et enumeratio eadem forma) */
interior chorda
_via_norma (
    chorda via)
{
    dum (   via.mensura  >= II && via.datum[ZEPHYRUM] == '.'
         && via.datum[I] == '/')
    {
        via.datum    += II;
        via.mensura  -= II;
    }
    redde via;
}

/* valor attributi spatiis divisus -> Xar de chorda */
interior Xar*
_verba (
      chorda* valor,
     Piscina* piscina)
{
    Xar* verba = xar_creare(piscina, (i32)magnitudo(chorda));
    i32  i;
    s32  initium = -I;   /* signatum: -I = extra verbum */

    si (verba == NIHIL || valor == NIHIL)
    {
        redde verba;
    }
    per (i = ZEPHYRUM; i <= valor->mensura; i++)
    {
        b32 spatium = i == valor->mensura || valor->datum[i] == ' '
            || valor->datum[i] == '\t' || valor->datum[i] == '\n';

        si (!spatium && initium < ZEPHYRUM)
        {
            initium = (s32)i;
        }
        alioquin si (spatium && initium >= ZEPHYRUM)
        {
            *(chorda*)xar_addere(verba) = chorda_sectio(*valor,
                (i32)initium, i);
            initium = -I;
        }
    }
    redde verba;
}

/* linea causae: "via:linea: textus" (linea ZEPHYRUM = sine linea) */
interior vacuum
_causa (
     ChordaAedificator* a,
                   i32* numerus,
                chorda  via,
                   i32  linea,
    constans character* ante,
                chorda  medium,
    constans character* post)
{
    character numerus_lineae[XVI];

    si (*numerus > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '\n');
    }
    (*numerus)++;
    si (via.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, via);
        si (linea > ZEPHYRUM)
        {
            sprintf(numerus_lineae, ":%u", (insignatus integer)linea);
            chorda_aedificator_appendere_literis(a, numerus_lineae);
        }
        chorda_aedificator_appendere_literis(a, ": ");
    }
    chorda_aedificator_appendere_literis(a, ante);
    chorda_aedificator_appendere_chorda(a, medium);
    chorda_aedificator_appendere_literis(a, post);
}

interior chorda
_valor (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);
    chorda  vacua;

    si (v != NIHIL)
    {
        redde *v;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* elementum <porta> unum -> declaratio (VERUM) aut causae (FALSUM) */
interior b32
_portam_legere (
            StmlNodus* elementum,
               chorda  via,
                  i32  linea,
              Piscina* piscina,
    ChordaAedificator* causae,
                  i32* numerus,
       PortaDeclarata* d)
{
    b32 bene = VERUM;
    i32 k;

    per (k = ZEPHYRUM; elementum->attributa != NIHIL
         && k < xar_numerus(elementum->attributa); k++)
    {
        constans StmlAttributum* at = (constans StmlAttributum*)
            xar_obtinere(elementum->attributa, k);
        b32 notum = FALSUM;
        i32 j;

        per (j = ZEPHYRUM; _portae_attributa[j] != NIHIL && !notum; j++)
        {
            notum = at->titulus != NIHIL
                && chorda_aequalis_literis(*at->titulus,
                       _portae_attributa[j]);
        }
        si (!notum)
        {
            _causa(causae, numerus, via, linea, "attributum ignotum '",
                at->titulus != NIHIL ? *at->titulus : _valor(elementum,
                ""), "' in <porta> (titulus manu causa tegit currit)");
            bene = FALSUM;
        }
    }
    memset(d, ZEPHYRUM, magnitudo(PortaDeclarata));
    d->via      = via;
    d->linea    = linea;
    d->titulus  = _valor(elementum, "titulus");
    d->manu     = stml_attributum_habet(elementum, "manu");
    d->causa    = _valor(elementum, "causa");
    d->tegit    = _verba(stml_attributum_capere(elementum, "tegit"),
        piscina);
    d->currit   = _verba(stml_attributum_capere(elementum, "currit"),
        piscina);
    si (d->titulus.mensura > ZEPHYRUM && d->manu)
    {
        _causa(causae, numerus, via, linea,
            "titulus et manu simul (porta"
            " aut in PORTAE aut manu)", d->titulus, "");
        bene = FALSUM;
    }
    alioquin si (d->titulus.mensura == ZEPHYRUM && !d->manu)
    {
        _causa(causae, numerus, via, linea, "nec titulus nec manu in"
            " <porta>", d->titulus, "");
        bene = FALSUM;
    }
    si (d->manu && d->causa.mensura == ZEPHYRUM)
    {
        _causa(causae, numerus, via, linea, "manu sine causa (cur homo"
            " currit?)", d->causa, "");
        bene = FALSUM;
    }
    redde bene;
}

/* plagula una: declarationes in 'declaratae' */
interior vacuum
_plagulam_legere (
                          chorda  via,
                          chorda  textus,
    constans MateriaLexiconRatum* ratum,
             InternamentumChorda* intern,
                         Piscina* piscina,
                             Xar* declaratae,
               ChordaAedificator* causae,
                             i32* numerus)
{
    CrustaParsura relatio;
     MateriaNodus* radix;
              Xar* omnes;
              i32  k;

    si (!chorda_continet(textus, chorda_ex_literis("<porta", piscina)))
    {
        redde;
    }
    radix = crusta_arbor_parsare(piscina,
        (constans character*)textus.datum,
        textus.mensura, &CRUSTA_BASH, &relatio);
    si (radix == NIHIL)
    {
        _causa(causae, numerus, via, ZEPHYRUM, "crusta parsare nequit",
            via, "");
        redde;
    }
    omnes = materia_annotationes_colligere(piscina, radix, ratum, "#",
        NIHIL, intern);
    per (k = ZEPHYRUM; omnes != NIHIL && k < xar_numerus(omnes); k++)
    {
        constans MateriaAnnotatio* a = (constans MateriaAnnotatio*)
            xar_obtinere(omnes, k);
        StmlNodus* documentum;
              i32  n;
              i32  j;

        si (!a->parsata)
        {
            si (chorda_continet(a->crudum, chorda_ex_literis("<porta",
                    piscina)))
            {
                _causa(causae, numerus, via, a->commentarium.linea,
                    "annotatio porta illegibilis: ", a->error, "");
            }
            perge;
        }
        documentum = a->documentum != NIHIL ? a->documentum : a->arbor;
        si (documentum == NIHIL)
        {
            perge;
        }
        n = (documentum == a->arbor) ? I
            : stml_numerus_liberorum(documentum);
        per (j = ZEPHYRUM; j < n; j++)
        {
            StmlNodus* e = (documentum == a->arbor) ? a->arbor
                : stml_liberum_ad_indicem(documentum, j);
            PortaDeclarata d;

            si (   e == NIHIL || e->genus != STML_NODUS_ELEMENTUM
                || e->titulus == NIHIL
                || !chorda_aequalis_literis(*e->titulus, "porta"))
            {
                perge;
            }
            si (_portam_legere(e, via, a->commentarium.linea, piscina,
                    causae, numerus, &d))
            {
                *(PortaDeclarata*)xar_addere(declaratae) = d;
            }
        }
    }
}

Xar*
portae_declaratas_legere (
    constans PortaeSutura* sutura,
                  Piscina* piscina,
                   chorda* causa_out)
{
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
    InternamentumChorda* intern;
      ChordaAedificator* causae;
                    Xar* viae;
                    Xar* declaratae;
                    i32  numerus            = ZEPHYRUM;
                    b32  fundamentum_bonum  = VERUM;
                    i32  i;
                    i32  j;
                 chorda  vacua;

    vacua = chorda_ex_literis("", piscina);
    causae = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    declaratae = xar_creare(piscina, (i32)magnitudo(PortaDeclarata));
    intern = internamentum_creare(piscina);
    si (   sutura == NIHIL || causae == NIHIL || declaratae == NIHIL
        || intern == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
        &iudicium))
    {
        _causa(causae, &numerus, vacua, ZEPHYRUM,
            "lexicon crustae ratum"
            " fieri non potest", vacua, "");
        fundamentum_bonum = FALSUM;
    }
    viae = sutura->plagulas_enumerare(sutura->datum, piscina);
    si (viae == NIHIL)
    {
        _causa(causae, &numerus, vacua, ZEPHYRUM,
            "enumeratio plagularum"
            " fracta", vacua, "");
        fundamentum_bonum = FALSUM;
    }
    /* plagulae OMNES leguntur (causae omnes, trans plagulas); solum
     * fundamentum fractum (lexicon, enumeratio) cursum sistit */
    per (i = ZEPHYRUM; fundamentum_bonum && viae != NIHIL
         && i < xar_numerus(viae); i++)
    {
        chorda via = _via_norma(*(chorda*)xar_obtinere(viae, i));
        chorda textus;

        si (!sutura->plagulam_legere(sutura->datum, via, piscina,
                &textus))
        {
            _causa(causae, &numerus, via, ZEPHYRUM, "legi nequit",
                vacua,
                "");
            perge;
        }
        _plagulam_legere(via, textus, &ratum, intern, piscina,
            declaratae,
            causae, &numerus);
    }
    /* registrum contra declarationes */
    per (i = ZEPHYRUM; i < xar_numerus(declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(declaratae, i);
        constans PortaeIngressus* ingressus = NIHIL;

        si (d->manu)
        {
            perge;
        }
        per (j = ZEPHYRUM; sutura->tabula_portarum != NIHIL
             && j < xar_numerus(sutura->tabula_portarum); j++)
        {
            constans PortaeIngressus* p = (constans PortaeIngressus*)
                xar_obtinere(sutura->tabula_portarum, j);

            si (chorda_aequalis(p->clavis, d->titulus))
            {
                ingressus = p;
            }
        }
        si (ingressus == NIHIL)
        {
            _causa(causae, &numerus, d->via, d->linea, "titulus '",
                d->titulus, "' extra registrum PORTAE");
        }
        alioquin si (!chorda_aequalis(_via_norma(ingressus->cursor),
                         d->via))
        {
            _causa(causae, &numerus, d->via, d->linea, "cursor alienus:"
                " registrum '", d->titulus, "' dat ");
            chorda_aedificator_appendere_chorda(causae,
                _via_norma(ingressus->cursor));
        }
        per (j = ZEPHYRUM; j < i; j++)
        {
            constans PortaDeclarata* prior = (constans PortaDeclarata*)
                xar_obtinere(declaratae, j);

            si (   !prior->manu
                && chorda_aequalis(prior->titulus, d->titulus))
            {
                _causa(causae, &numerus, d->via, d->linea, "titulus '",
                    d->titulus, "' bis declaratus");
                frange;
            }
        }
    }
    per (j = ZEPHYRUM; sutura->tabula_portarum != NIHIL
         && j < xar_numerus(sutura->tabula_portarum); j++)
    {
        constans PortaeIngressus* p = (constans PortaeIngressus*)
            xar_obtinere(sutura->tabula_portarum, j);
        b32 inventa = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(declaratae) && !inventa; i++)
        {
            constans PortaDeclarata* d = (constans PortaDeclarata*)
                xar_obtinere(declaratae, i);

            inventa = !d->manu && chorda_aequalis(d->titulus, p->clavis)
                && chorda_aequalis(d->via, _via_norma(p->cursor));
        }
        si (!inventa)
        {
            _causa(causae, &numerus, vacua, ZEPHYRUM, "porta '",
                p->clavis,
                "' (cursor ");
            chorda_aedificator_appendere_chorda(causae,
                _via_norma(p->cursor));
            chorda_aedificator_appendere_literis(causae,
                ") sine annotatione"
                " <porta titulus=\"...\"/>");
        }
    }
    si (numerus > ZEPHYRUM)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(causae);
        }
        redde NIHIL;
    }
    redde declaratae;
}
