/* portae.c - PORTAE DECLARATAE: lector declarationum <porta> (plan
 * portae-declaratae T1) et derivatio operturae (T2); caput
 * include/portae.h
 *
 * Commentaria per crustam inveniuntur (materia_annotationes super
 * arbore crustae - '#' intra chordam commentarium non est, prosa non
 * annotatio est). Praecribrum: plagula sine litteris '<porta' non
 * parsatur. Recusatio post lectionem TOTAM: causae omnes una
 * nominantur.
 *
 * Derivatio interpretatur quae sutura dat (summarium effectuum,
 * litterae probationum) - I/O nulla hic; regulae portationis ex
 * tools/inventarium_suitarum.py ('currit', 'fontes') et
 * silva.portae_debitae.
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
#include "iter_directoria.h"
#include <string.h>

/* vocabulum CLAUSUM attributorum <porta> */
hic_manens constans character* constans _portae_attributa[] = {
    "titulus", "manu", "causa", "tegit", "currit", NIHIL
};

interior b32
_finit (
                 chorda  s,
     constans character* suffixum)
{
    i32 n = (i32)strlen(suffixum);

    redde s.mensura >= n && memcmp(s.datum + (s.mensura - n), suffixum,
        (size_t)n) == ZEPHYRUM;
}

interior b32
_incipit (
                 chorda  s,
     constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);

    redde s.mensura >= n && memcmp(s.datum, praefixum, (size_t)n)
        == ZEPHYRUM;
}

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
            chorda_aedificator_appendere_character(a, ':');
            chorda_aedificator_appendere_i32(a, linea);
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

        si (!_finit(via, ".sh"))
        {
            perge;
        }
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


/* ==================================================
 * DERIVATIO (plan T2)
 * ================================================== */

nomen structura {
    constans PortaeSutura* sutura;
                      Xar* viae;       /* arboris, normatae */
                  Piscina* piscina;
           PortaeOpertura* o;
} PortaeDerivatio;

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

/* titulus post '/' ultimum (via sine '/' = tota) */
interior chorda
_titulus_viae (
    chorda via)
{
    i32 i = via.mensura;

    dum (i > ZEPHYRUM && via.datum[i - I] != '/')
    {
        i--;
    }
    redde chorda_sectio(via, i, via.mensura);
}

/* "ante a medium b post" (chordae vacuae licent) */
interior chorda
_ratio (
                Piscina* piscina,
     constans character* ante,
                 chorda  a,
     constans character* medium,
                 chorda  b,
     constans character* post)
{
    ChordaAedificator* r = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);

    chorda_aedificator_appendere_literis(r, ante);
    chorda_aedificator_appendere_chorda(r, a);
    chorda_aedificator_appendere_literis(r, medium);
    chorda_aedificator_appendere_chorda(r, b);
    chorda_aedificator_appendere_literis(r, post);
    redde chorda_aedificator_finire(r);
}

/* "via:linea" annotationis */
interior chorda
_locus (
                    Piscina* piscina,
    constans PortaDeclarata* d)
{
    ChordaAedificator* r = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);

    chorda_aedificator_appendere_chorda(r, d->via);
    chorda_aedificator_appendere_character(r, ':');
    chorda_aedificator_appendere_i32(r, d->linea);
    redde chorda_aedificator_finire(r);
}

/* duplicata (exemplar et genus eadem) semel - ratio prima manet */
interior vacuum
_exemplar_addere (
                    Xar* exemplaria,
                 chorda  exemplar,
    PortaeExemplarGenus  genus,
                 chorda  ratio)
{
    PortaeExemplar* e;
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(exemplaria); i++)
    {
        e = (PortaeExemplar*)xar_obtinere(exemplaria, i);
        si (e->genus == genus && chorda_aequalis(e->exemplar, exemplar))
        {
            redde;
        }
    }
    e            = (PortaeExemplar*)xar_addere(exemplaria);
    e->exemplar  = exemplar;
    e->genus     = genus;
    e->ratio     = ratio;
}

interior b32
_in_arbore (
    constans PortaeDerivatio* der,
                      chorda  via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(der->viae); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(der->viae, i), via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* attributum aequale literis */
interior b32
_valor_est (
             StmlNodus* nodus,
    constans character* titulus,
    constans character* valor)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    redde v != NIHIL && chorda_aequalis_literis(*v, valor);
}

/* situs effectus unus -> exemplar (et fontes .c) */
interior vacuum
_situm_tractare (
    PortaeDerivatio* der,
          StmlNodus* situs)
{
                 chorda* via    = stml_attributum_capere(situs, "via");
                 chorda* mand   = stml_attributum_capere(situs,
                     "mandatum");
                 chorda genus_situs;
                 chorda ratio;
    PortaeExemplarGenus genus;
                    i32 i;

    si (   via == NIHIL || !_valor_est(situs, "classis", "arbor")
        || !_valor_est(situs, "resolutio", "plena")
        || chorda_aequalis_literis(*via, "."))
    {
        redde;
    }
    si (_valor_est(situs, "forma", "globus"))
    {
        genus = PORTAE_EXEMPLAR_GLOBUS;
    }
    alioquin si (_valor_est(situs, "forma", "praefixum"))
    {
        genus = PORTAE_EXEMPLAR_PRAEFIXUM;
    }
    alioquin
    {
        genus = PORTAE_EXEMPLAR_VIA;
    }
    genus_situs = *situs->titulus;
    si (mand != NIHIL)
    {
        genus_situs = _ratio(der->piscina, "", genus_situs, " (", *mand,
            ")");
    }
    ratio = _ratio(der->piscina, "", genus_situs, " in ", _valor(situs,
        "plagula"), "");
    _exemplar_addere(der->o->exemplaria, *via, genus, ratio);
    si (!_finit(*via, ".c"))
    {
        redde;
    }
    si (genus == PORTAE_EXEMPLAR_VIA)
    {
        _exemplar_addere(der->o->fontes_c, *via, PORTAE_EXEMPLAR_VIA,
            ratio);
    }
    alioquin si (genus == PORTAE_EXEMPLAR_GLOBUS)
    {
        per (i = ZEPHYRUM; i < xar_numerus(der->viae); i++)
        {
            chorda v = *(chorda*)xar_obtinere(der->viae, i);

            si (   _finit(v, ".c")
                && directorium_titulus_congruit(v, *via))
            {
                _exemplar_addere(der->o->fontes_c, v,
                    PORTAE_EXEMPLAR_VIA,
                    ratio);
            }
        }
    }
}

/* summarium ambulatur: situs (lectio exsecutio fontatio probatio
 * enumeratio) tractantur; scriptura et ambitus_lectio nihil tegunt;
 * elementum custoditum (processus aut situs) cum subarbore omittitur */
interior vacuum
_effectus_ambulare (
    PortaeDerivatio* der,
          StmlNodus* nodus)
{
    hic_manens constans character* constans situs[] = {
        "lectio", "exsecutio", "fontatio", "probatio", "enumeratio",
            NIHIL
    };
    i32 n = stml_numerus_liberorum(nodus);
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < n; i++)
    {
         StmlNodus* e          = stml_liberum_ad_indicem(nodus, i);
               b32  est_situs  = FALSUM;

        si (   e          == NIHIL || e->genus != STML_NODUS_ELEMENTUM
            || e->titulus == NIHIL
            || stml_attributum_habet(e, "custodia"))
        {
            perge;
        }
        per (j = ZEPHYRUM; situs[j] != NIHIL && !est_situs; j++)
        {
            est_situs = chorda_aequalis_literis(*e->titulus, situs[j]);
        }
        si (est_situs)
        {
            _situm_tractare(der, e);
        }
        alioquin
        {
            _effectus_ambulare(der, e);
        }
    }
}

/* 'bin/x' aut 'sub/build/x' (litterae [a-z_]) - binarium domus */
interior b32
_binarium_est (
    chorda via)
{
    i32 partes   = I;
    i32 initium  = ZEPHYRUM;
    b32 bin      = FALSUM;
    b32 build    = FALSUM;
    i32 i;

    per (i = ZEPHYRUM; i <= via.mensura; i++)
    {
        si (i == via.mensura || via.datum[i] == '/')
        {
            chorda pars = chorda_sectio(via, initium, i);

            si (pars.mensura == ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (partes == I)
            {
                bin = chorda_aequalis_literis(pars, "bin");
            }
            alioquin si (partes == II)
            {
                build = chorda_aequalis_literis(pars, "build");
            }
            si (i < via.mensura)
            {
                partes++;
            }
            initium = i + I;
        }
        alioquin si (!(   (via.datum[i] >= 'a' && via.datum[i] <= 'z')
                       || via.datum[i] == '_'))
        {
            redde FALSUM;
        }
    }
    redde (partes == II && bin) || (partes == III && build);
}

/* programma vocatum (verbum primum litterae, aut verbum 'currit'):
 * fons C in arbore -> fontes_c; scriptum -> exemplaria; binarium ->
 * fons introitus per nomen (<b>_principale.c aut <b>.c, extra
 * probationes/; unicus); aliter litterae ignorantur, verbum 'currit'
 * ignotum nominatur */
interior vacuum
_vocatum_tractare (
    PortaeDerivatio* der,
             chorda  verbum,
             chorda  origo,
                b32  annotatio)
{
    chorda v = _via_norma(verbum);
    chorda ratio;
    chorda b;
    chorda inventus;
       i32 numerus = ZEPHYRUM;
       i32 i;

    ratio = annotatio ? _ratio(der->piscina, "currit ", v, " (", origo,
        ")")
        : _ratio(der->piscina, "", origo, " currit ", v, "");
    si (_in_arbore(der, v))
    {
        si (_finit(v, ".c"))
        {
            _exemplar_addere(der->o->fontes_c, v, PORTAE_EXEMPLAR_VIA,
                ratio);
        }
        alioquin si (annotatio || _finit(v, ".sh"))
        {
            _exemplar_addere(der->o->exemplaria, v, PORTAE_EXEMPLAR_VIA,
                ratio);
        }
        redde;
    }
    si (   !_binarium_est(v) || chorda_continet(v, chorda_ex_literis(
            "probatio", der->piscina)))
    {
        si (annotatio)
        {
            *(chorda*)xar_addere(der->o->ignota) = _ratio(der->piscina,
                "", ratio, ": nec in arbore nec binarium", _vacua(),
                "");
        }
        redde;
    }
    b         = _titulus_viae(v);
    inventus  = _vacua();
    per (i = ZEPHYRUM; i < xar_numerus(der->viae); i++)
    {
        chorda f = *(chorda*)xar_obtinere(der->viae, i);
        chorda t;

        si (   !_finit(f, ".c") || chorda_continet(f, chorda_ex_literis(
                "/probationes/", der->piscina)))
        {
            perge;
        }
        t = _titulus_viae(f);
        /* '/<b>.c': fons in radice (sine '/') non est introitus */
        si (   t.mensura < f.mensura
            && (   chorda_aequalis(t, _ratio(der->piscina, "", b,
                       "_principale.c", _vacua(), ""))
                || chorda_aequalis(t, _ratio(der->piscina, "", b, ".c",
                       _vacua(), ""))))
        {
            numerus++;
            si (numerus == I)
            {
                inventus = f;
            }
        }
    }
    si (numerus == I)
    {
        _exemplar_addere(der->o->fontes_c, inventus,
            PORTAE_EXEMPLAR_VIA,
            ratio);
    }
    alioquin
    {
        *(chorda*)xar_addere(der->o->ignota) = _ratio(der->piscina, "",
            ratio, numerus == ZEPHYRUM ? ": fons introitus ignotus"
            : ": fons introitus ambiguus", _vacua(), "");
    }
}

/* verbum primum litterae (spatia praecedentia praetereuntur) */
interior chorda
_verbum_primum (
    chorda littera)
{
    i32 i = ZEPHYRUM;
    i32 j;

    dum (   i < littera.mensura && (littera.datum[i] == ' '
            || littera.datum[i] == '\t' || littera.datum[i] == '\n'))
    {
        i++;
    }
    j = i;
    dum (   j < littera.mensura && littera.datum[j] != ' '
         && littera.datum[j] != '\t' && littera.datum[j] != '\n')
    {
        j++;
    }
    redde chorda_sectio(littera, i, j);
}

/* probationes suitae: <dir>/probatio_*.c (directorium ipsum solum) ->
 * fontes_c; litterae earum -> programmata vocata */
interior vacuum
_suitam_tractare (
    PortaeDerivatio* der,
             chorda  directorium)
{
    chorda ratio = _ratio(der->piscina, "probatio suitae ",
        directorium,
        "", _vacua(), "");
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(der->viae); i++)
    {
        chorda  f = *(chorda*)xar_obtinere(der->viae, i);
        chorda  titulus;
           Xar* litterae;
           i32  k;

        si (   f.mensura                    <= directorium.mensura + I
            || memcmp(f.datum, directorium.datum,
                   (size_t)directorium.mensura) != ZEPHYRUM
            || f.datum[directorium.mensura] != '/')
        {
            perge;
        }
        titulus = chorda_sectio(f, directorium.mensura + I, f.mensura);
        si (   _titulus_viae(titulus).mensura < titulus.mensura
            || !_incipit(titulus, "probatio_")
            || !_finit(titulus, ".c"))
        {
            perge;
        }
        _exemplar_addere(der->o->fontes_c, f, PORTAE_EXEMPLAR_VIA,
            ratio);
        litterae = der->sutura->litterae_vocationum != NIHIL
            ? der->sutura->litterae_vocationum(der->sutura->datum, f,
                der->piscina) : NIHIL;
        si (litterae == NIHIL)
        {
            *(chorda*)xar_addere(der->o->ignota) = _ratio(der->piscina,
                "litterae ", f, " ignotae (programmata vocata caeca)",
                _vacua(), "");
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(litterae); k++)
        {
            chorda verbum =
                _verbum_primum(*(chorda*)xar_obtinere(litterae,
                k));

            si (verbum.mensura > ZEPHYRUM)
            {
                _vocatum_tractare(der, verbum, f, FALSUM);
            }
        }
    }
}

Xar*
portae_operturas_derivare (
    constans PortaeSutura* sutura,
             constans Xar* declaratae,
                  Piscina* piscina)
{
    PortaeDerivatio  der;
                Xar* operturae;
                Xar* crudae;
                i32  i;
                i32  j;

    si (sutura == NIHIL || declaratae == NIHIL)
    {
        redde NIHIL;
    }
    crudae     = sutura->plagulas_enumerare(sutura->datum, piscina);
    operturae  = xar_creare(piscina, (i32)magnitudo(PortaeOpertura));
    der.viae   = xar_creare(piscina, (i32)magnitudo(chorda));
    si (crudae == NIHIL || operturae == NIHIL || der.viae == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(crudae); i++)
    {
        *(chorda*)xar_addere(der.viae) = _via_norma(*(chorda*)
            xar_obtinere(crudae, i));
    }
    der.sutura   = sutura;
    der.piscina  = piscina;
    per (i = ZEPHYRUM; i < xar_numerus(declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(declaratae, i);
                  PortaeOpertura* o = (PortaeOpertura*)xar_addere(
                      operturae);
                       StmlNodus* summarium;
                          chorda  locus = _locus(piscina, d);
                          chorda  vacua = _vacua();

        o->declaratio    = d;
        o->exemplaria    = xar_creare(piscina,
            (i32)magnitudo(PortaeExemplar));
        o->fontes_c      = xar_creare(piscina,
            (i32)magnitudo(PortaeExemplar));
        o->ignota  = xar_creare(piscina, (i32)magnitudo(chorda));
        der.o      = o;
        _exemplar_addere(o->exemplaria, d->via, PORTAE_EXEMPLAR_VIA,
            _ratio(piscina, "cursor ipse", vacua, "", vacua, ""));
        summarium = sutura->effectus != NIHIL
            ? sutura->effectus(sutura->datum, d->via, piscina) : NIHIL;
        si (summarium == NIHIL)
        {
            *(chorda*)xar_addere(o->ignota) = _ratio(piscina,
                "effectus ",
                d->via,
                " ignotum (quae cursor legit et exsequitur caeca)",
                vacua, "");
        }
        alioquin
        {
            _effectus_ambulare(&der, summarium);
        }
        per (j = ZEPHYRUM; !d->manu && sutura->tabula_portarum != NIHIL
             && j < xar_numerus(sutura->tabula_portarum); j++)
        {
            constans PortaeIngressus* p = (constans PortaeIngressus*)
                xar_obtinere(sutura->tabula_portarum, j);

            si (   chorda_aequalis(p->clavis, d->titulus)
                && p->probationes.mensura > ZEPHYRUM)
            {
                _suitam_tractare(&der, _via_norma(p->probationes));
            }
        }
        per (j = ZEPHYRUM; d->tegit != NIHIL
            && j < xar_numerus(d->tegit);
             j++)
        {
            _exemplar_addere(o->exemplaria, *(chorda*)xar_obtinere(
                d->tegit, j), PORTAE_EXEMPLAR_GLOBUS, _ratio(piscina,
                "tegit (", locus, ")", vacua, ""));
        }
        per (j = ZEPHYRUM; d->currit != NIHIL
             && j < xar_numerus(d->currit); j++)
        {
            _vocatum_tractare(&der, *(chorda*)xar_obtinere(d->currit,
                j),
                locus, VERUM);
        }
    }
    redde operturae;
}
