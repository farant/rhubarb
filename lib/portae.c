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
 * litterae probationum) - I/O nulla hic; regulae portatae ex
 * pythonica (inventarium_suitarum.py 'currit'/'fontes' et
 * silva.portae_debitae veteri, deleta T5).
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
    "titulus", "manu", "causa", "tegit", "currit", "instrumentum", NIHIL
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

/* causae recusationis: textus hominis (linea una per causam) et
 * eaedem structae (Xar de PortaeRecusatio) */
nomen structura {
    ChordaAedificator* textus;
                  i32  numerus;
                  Xar* recusationes;
              Piscina* piscina;
} PortaeCausae;

/* causa nova: "via:linea: ante medium post" (linea ZEPHYRUM = sine
 * linea, via vacua = sine loco) */
interior vacuum
_causa (
          PortaeCausae* k,
                chorda  via,
                   i32  linea,
    constans character* ante,
                chorda  medium,
    constans character* post)
{
       ChordaAedificator* a = k->textus;
       ChordaAedificator* c = chorda_aedificator_creare(k->piscina,
           (memoriae_index)LXIV);
         PortaeRecusatio* r = (PortaeRecusatio*)xar_addere(
             k->recusationes);

    si (k->numerus > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '\n');
    }
    k->numerus++;
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
    chorda_aedificator_appendere_literis(c, ante);
    chorda_aedificator_appendere_chorda(c, medium);
    chorda_aedificator_appendere_literis(c, post);
    r->via    = via;
    r->linea  = via.mensura > ZEPHYRUM ? linea : ZEPHYRUM;
    r->causa  = chorda_aedificator_finire(c);
    chorda_aedificator_appendere_chorda(a, r->causa);
}

/* causa ultima producitur: "... pars post" */
interior vacuum
_causam_extendere (
          PortaeCausae* k,
                chorda  pars,
    constans character* post)
{
       PortaeRecusatio* r = (PortaeRecusatio*)xar_obtinere(
           k->recusationes, xar_numerus(k->recusationes) - I);
     ChordaAedificator* c = chorda_aedificator_creare(k->piscina,
         (memoriae_index)LXIV);

    chorda_aedificator_appendere_chorda(k->textus, pars);
    chorda_aedificator_appendere_literis(k->textus, post);
    chorda_aedificator_appendere_chorda(c, r->causa);
    chorda_aedificator_appendere_chorda(c, pars);
    chorda_aedificator_appendere_literis(c, post);
    r->causa = chorda_aedificator_finire(c);
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
          PortaeCausae* causae,
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
            _causa(causae, via, linea, "attributum ignotum '",
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
    d->instrumenta = _verba(stml_attributum_capere(elementum,
        "instrumentum"), piscina);
    d->currit   = _verba(stml_attributum_capere(elementum, "currit"),
        piscina);
    /* titulus + manu = porta registrata (nomine curri potest) quam
     * tamen homo currit: debita eam nominant, numquam currunt */
    si (d->titulus.mensura == ZEPHYRUM && !d->manu)
    {
        _causa(causae, via, linea, "nec titulus nec manu in"
            " <porta>", d->titulus, "");
        bene = FALSUM;
    }
    si (d->manu && d->causa.mensura == ZEPHYRUM)
    {
        _causa(causae, via, linea, "manu sine causa (cur homo"
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
                    PortaeCausae* causae)
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
        _causa(causae, via, ZEPHYRUM, "crusta parsare nequit",
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
                _causa(causae, via, a->commentarium.linea,
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
                    causae, &d))
            {
                *(PortaDeclarata*)xar_addere(declaratae) = d;
            }
        }
    }
}

Xar*
portae_declaratas_legere (
    constans PortaeSutura*  sutura,
                  Piscina*  piscina,
                   chorda*  causa_out,
                      Xar** recusationes_out)
{
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
    InternamentumChorda* intern;
           PortaeCausae  causae;
                    Xar* viae;
                    Xar* declaratae;
                    b32  fundamentum_bonum  = VERUM;
                    i32  i;
                    i32  j;
                 chorda  vacua;

    vacua = chorda_ex_literis("", piscina);
    causae.textus = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
    causae.numerus = ZEPHYRUM;
    causae.recusationes = xar_creare(piscina,
        (i32)magnitudo(PortaeRecusatio));
    causae.piscina = piscina;
    declaratae = xar_creare(piscina, (i32)magnitudo(PortaDeclarata));
    intern = internamentum_creare(piscina);
    si (   sutura              == NIHIL || causae.textus == NIHIL
        || causae.recusationes == NIHIL || declaratae == NIHIL
        || intern              == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
        &iudicium))
    {
        _causa(&causae, vacua, ZEPHYRUM,
            "lexicon crustae ratum"
            " fieri non potest", vacua, "");
        fundamentum_bonum = FALSUM;
    }
    viae = sutura->plagulas_enumerare(sutura->datum, piscina);
    si (viae == NIHIL)
    {
        _causa(&causae, vacua, ZEPHYRUM,
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
            _causa(&causae, via, ZEPHYRUM, "legi nequit",
                vacua,
                "");
            perge;
        }
        _plagulam_legere(via, textus, &ratum, intern, piscina,
            declaratae, &causae);
    }
    /* possessio instrumentorum: binarium uni portae solum */
    per (i = ZEPHYRUM; i < xar_numerus(declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(declaratae, i);
                             i32 k;

        per (k = ZEPHYRUM; k < xar_numerus(d->instrumenta); k++)
        {
            chorda b = *(chorda*)xar_obtinere(d->instrumenta, k);
               b32 inventum = FALSUM;
               i32 m;

            per (j = ZEPHYRUM; j < i && !inventum; j++)
            {
                constans PortaDeclarata* prior =
                    (constans PortaDeclarata*)
                    xar_obtinere(declaratae, j);

                per (m = ZEPHYRUM; m < xar_numerus(prior->instrumenta)
                     && !inventum; m++)
                {
                    si (chorda_aequalis(b, *(chorda*)xar_obtinere(
                            prior->instrumenta, m)))
                    {
                        inventum = VERUM;
                        _causa(&causae, d->via, d->linea,
                            "instrumentum '",
                            b, "' bis possessum (prior: ");
                        _causam_extendere(&causae,
                            prior->titulus.mensura > ZEPHYRUM
                            ? prior->titulus : prior->via, ")");
                    }
                }
            }
        }
    }
    /* registrum contra declarationes */
    per (i = ZEPHYRUM; i < xar_numerus(declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(declaratae, i);
        constans PortaeIngressus* ingressus = NIHIL;

        si (d->titulus.mensura == ZEPHYRUM)
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
            _causa(&causae, d->via, d->linea, "titulus '",
                d->titulus, "' extra registrum PORTAE");
        }
        alioquin si (!chorda_aequalis(_via_norma(ingressus->cursor),
                         d->via))
        {
            _causa(&causae, d->via, d->linea, "cursor alienus:"
                " registrum '", d->titulus, "' dat ");
            _causam_extendere(&causae, _via_norma(ingressus->cursor),
                "");
        }
        per (j = ZEPHYRUM; j < i; j++)
        {
            constans PortaDeclarata* prior = (constans PortaDeclarata*)
                xar_obtinere(declaratae, j);

            si (   prior->titulus.mensura > ZEPHYRUM
                && chorda_aequalis(prior->titulus, d->titulus))
            {
                _causa(&causae, d->via, d->linea, "titulus '",
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

            inventa = d->titulus.mensura > ZEPHYRUM
                && chorda_aequalis(d->titulus, p->clavis)
                && chorda_aequalis(d->via, _via_norma(p->cursor));
        }
        si (!inventa)
        {
            _causa(&causae, vacua, ZEPHYRUM, "porta '",
                p->clavis,
                "' (cursor ");
            _causam_extendere(&causae, _via_norma(p->cursor),
                ") sine annotatione <porta titulus=\"...\"/>");
        }
    }
    si (causae.numerus > ZEPHYRUM)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(causae.textus);
        }
        si (recusationes_out != NIHIL)
        {
            *recusationes_out = causae.recusationes;
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
             constans Xar* declaratae;   /* possessores */
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

interior vacuum
_vocatum_tractare (
    PortaeDerivatio* der,
             chorda  verbum,
             chorda  origo,
                b32  annotatio);

/* situs effectus unus -> exemplar (et fontes .c); instrumentum domus
 * exsecutum (classis instrumentum_domus, 'bin/x') -> fons introitus
 * eius, ut binaria quae probationes vocant */
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

    si (   via != NIHIL
        && _valor_est(situs, "classis", "instrumentum_domus")
        && _valor_est(situs, "resolutio", "plena")
        && chorda_aequalis_literis(*situs->titulus, "exsecutio"))
    {
        _vocatum_tractare(der, *via, _valor(situs, "plagula"), FALSUM);
        redde;
    }
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
    /* fons NON tractus (scriptum eum ipsum scribit, e.g.
     * .fumus_generatum.c): clausura nulla, nihil tegit */
    si (genus == PORTAE_EXEMPLAR_VIA && _in_arbore(der, *via))
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

/* summarium ambulatur: situs CONTENTI (lectio exsecutio fontatio)
 * tractantur; probatio et enumeratio tempus aut existentiam solum
 * inspiciunt ('[ obj -nt caput ]', 'ls include/<caput>.h' custodiae
 * recentium - mensuratum T4: MMLXX tactus falsi ex
 * silva/materia_substratum.sh), scriptura et ambitus_lectio nihil
 * tegunt; elementum custoditum (processus aut situs) cum subarbore
 * omittitur */
interior vacuum
_effectus_ambulare (
    PortaeDerivatio* der,
          StmlNodus* nodus)
{
    hic_manens constans character* constans situs[] = {
        "lectio", "exsecutio", "fontatio", NIHIL
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

/* programma vocatum aut via nominata (verbum primum litterae, aut
 * verbum 'currit'): via arboris -> fontes_c (.c) aut exemplaria
 * (cetera: scripta, fixa, tabulae); binarium ->
 * fons introitus per nomen (<b>_principale.c aut <b>.c, extra
 * probationes/; ambiguus = candidati OMNES; nullus = nota, aut
 * caecum si annotatio id nominat); litterae aliae ignorantur, verbum
 * 'currit' ignotum caecum est */
/* declaratio quae binarium possidet (instrumentum=), aut NIHIL */
interior constans PortaDeclarata*
_possessor (
    constans PortaeDerivatio* der,
                      chorda  binarium)
{
    i32 i;
    i32 k;

    per (i = ZEPHYRUM; der->declaratae != NIHIL
         && i < xar_numerus(der->declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(der->declaratae, i);

        per (k = ZEPHYRUM; k < xar_numerus(d->instrumenta); k++)
        {
            si (chorda_aequalis(binarium, *(chorda*)xar_obtinere(
                    d->instrumenta, k)))
            {
                redde d;
            }
        }
    }
    redde NIHIL;
}

interior vacuum
_vocatum_tractare (
    PortaeDerivatio* der,
             chorda  verbum,
             chorda  origo,
                b32  annotatio)
{
    chorda  v = _via_norma(verbum);
    chorda  ratio;
    chorda  b;
       Xar* candidati;
       i32  i;

    ratio = annotatio ? _ratio(der->piscina, "currit ", v, " (", origo,
        ")")
        : _ratio(der->piscina, "", origo, " currit ", v, "");
    si (_in_arbore(der, v))
    {
        /* 'currit tools/<b>.c' (annotatio): binarium bin/<b> candidatus
         * (circulus commissionis II T2; briar-fumus currit $RADIX/
         * bin/briar per variabilem - effectus id non videt) */
        si (   annotatio && _finit(v, ".c")
            && chorda_incipit(v, chorda_ex_literis("tools/",
                   der->piscina)))
        {
            chorda nomen_b = chorda_sectio(v, (i32)VI, v.mensura - II);

            _exemplar_addere(der->o->binaria, _ratio(der->piscina,
                "bin/",
                nomen_b, "", _vacua(), ""), PORTAE_EXEMPLAR_VIA, ratio);
        }
        /* probatio quae viam arboris nominat ab ea pendet (scriptum
         * vocatum, fixum lectum, fons compilatus) */
        si (!annotatio && !_finit(v, ".sh"))
        {
            ratio = _ratio(der->piscina, "", origo, " nominat ", v, "");
        }
        _exemplar_addere(_finit(v, ".c") ? der->o->fontes_c
            : der->o->exemplaria, v, PORTAE_EXEMPLAR_VIA, ratio);
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
    /* binarium arboris quod porta currit (circulus commissionis II
     * T2): ETIAM si alius portae instrumentum est - possessor fontes
     * hereditat, haec tamen id currit, ergo recens esse debet */
    si (chorda_incipit(v, chorda_ex_literis("bin/", der->piscina)))
    {
        _exemplar_addere(der->o->binaria, v, PORTAE_EXEMPLAR_VIA,
            ratio);
    }
    /* possessio: instrumentum alius portae - infrastructura, non
     * subiectum; 'currit' expressum (annotatio) semper valet */
    si (!annotatio)
    {
        constans PortaDeclarata* possessor = _possessor(der, v);

        si (possessor != NIHIL && possessor != der->o->declaratio)
        {
            *(chorda*)xar_addere(der->o->notae) = _ratio(der->piscina,
                "",
                ratio, ": instrumentum portae '",
                possessor->titulus.mensura
                > ZEPHYRUM ? possessor->titulus : possessor->via,
                "' (possessor hereditat, haec non)");
            redde;
        }
    }
    b          = _titulus_viae(v);
    candidati  = xar_creare(der->piscina, (i32)magnitudo(chorda));
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
            *(chorda*)xar_addere(candidati) = f;
        }
    }
    si (xar_numerus(candidati) == ZEPHYRUM)
    {
        *(chorda*)xar_addere(annotatio ? der->o->ignota : der->o->notae)
            = _ratio(der->piscina, "", ratio,
            ": fons introitus ignotus",
                _vacua(), "");
        redde;
    }
    /* praelatio: 'bin/x' -> candidatus unicus in tools/; 'sub/build/x'
     * -> unicus sub 'sub/' (tools/fabrica.c, non lib/fabrica.c nec
     * briar/instrumenta/fabrica.c) */
    si (xar_numerus(candidati) > I)
    {
        chorda sedes = chorda_incipit(v, chorda_ex_literis("bin/",
            der->piscina)) ? chorda_ex_literis("tools/", der->piscina)
            : chorda_sectio(v, ZEPHYRUM, (i32)(chorda_invenire_index(v,
                chorda_ex_literis("/", der->piscina)) + I));
        chorda praelatus  = _vacua();
           i32 praelati   = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(candidati); i++)
        {
            chorda f = *(chorda*)xar_obtinere(candidati, i);

            si (chorda_incipit(f, sedes))
            {
                praelati++;
                praelatus = f;
            }
        }
        si (praelati == I)
        {
            xar_vacare(candidati);
            *(chorda*)xar_addere(candidati) = praelatus;
        }
    }
    si (xar_numerus(candidati) > I)
    {
        ChordaAedificator* r = chorda_aedificator_creare(der->piscina,
            (memoriae_index)LXIV);

        chorda_aedificator_appendere_chorda(r, ratio);
        chorda_aedificator_appendere_literis(r,
            " (nomen ambiguum: unus ex ");
        chorda_aedificator_appendere_i32(r, xar_numerus(candidati));
        chorda_aedificator_appendere_character(r, ')');
        ratio = chorda_aedificator_finire(r);
    }
    per (i = ZEPHYRUM; i < xar_numerus(candidati); i++)
    {
        _exemplar_addere(der->o->fontes_c, *(chorda*)xar_obtinere(
            candidati, i), PORTAE_EXEMPLAR_VIA, ratio);
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
        litterae = der->sutura->litterae_probationis != NIHIL
            ? der->sutura->litterae_probationis(der->sutura->datum, f,
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
    der.sutura      = sutura;
    der.declaratae  = declaratae;
    der.piscina     = piscina;
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
        o->notae   = xar_creare(piscina, (i32)magnitudo(chorda));
        o->binaria = xar_creare(piscina,
            (i32)magnitudo(PortaeExemplar));
        der.o      = o;
        _exemplar_addere(o->exemplaria, d->via, PORTAE_EXEMPLAR_VIA,
            _ratio(piscina, "cursor ipse", vacua, "", vacua, ""));
        /* instrumenta possessa: porta ea ipsa currit */
        per (j = ZEPHYRUM; d->instrumenta != NIHIL
             && j < xar_numerus(d->instrumenta); j++)
        {
            _exemplar_addere(o->binaria, *(chorda*)xar_obtinere(
                d->instrumenta, j), PORTAE_EXEMPLAR_VIA, _ratio(piscina,
                "instrumentum (", locus, ")", vacua, ""));
        }
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
        per (j = ZEPHYRUM; sutura->tabula_portarum != NIHIL
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


/* ==================================================
 * DEBITA (plan T3)
 * ================================================== */

/* clausura in memoria: semel per fontem (NIHIL = ignota) */
nomen structura {
    chorda  fons;
       Xar* clausura;
} PortaeClausura;

interior b32
_via_c (
    chorda via)
{
    redde _finit(via, ".c") || _finit(via, ".h") || _finit(via, ".m");
}

interior Xar*
_clausuram_obtinere (
    constans PortaeSutura* sutura,
                      Xar* memoria,
                   chorda  fons,
                  Piscina* piscina)
{
    PortaeClausura* c;
               i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(memoria); i++)
    {
        c = (PortaeClausura*)xar_obtinere(memoria, i);
        si (chorda_aequalis(c->fons, fons))
        {
            redde c->clausura;
        }
    }
    c        = (PortaeClausura*)xar_addere(memoria);
    c->fons  = fons;
    c->clausura  = sutura->clausura_c != NIHIL
        ? sutura->clausura_c(sutura->datum, fons, piscina) : NIHIL;
    redde c->clausura;
}

interior b32
_chorda_in (
    constans Xar* x,
          chorda  valor)
{
    i32 i;

    per (i = ZEPHYRUM; x != NIHIL && i < xar_numerus(x); i++)
    {
        si (chorda_aequalis(*(constans chorda*)xar_obtinere(x, i),
            valor))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ratio addita nisi eadem iam adsit (cursor qui et fons_c est) */
interior vacuum
_rationem_addere (
       Xar* rationes,
    chorda  ratio)
{
    si (!_chorda_in(rationes, ratio))
    {
        *(chorda*)xar_addere(rationes) = ratio;
    }
}

interior b32
_exemplar_congruit (
    constans PortaeExemplar* e,
                     chorda  via)
{
    si (e->genus == PORTAE_EXEMPLAR_GLOBUS)
    {
        redde directorium_titulus_congruit(via, e->exemplar);
    }
    si (e->genus == PORTAE_EXEMPLAR_PRAEFIXUM)
    {
        redde chorda_incipit(via, e->exemplar);
    }
    redde chorda_aequalis(via, e->exemplar);
}

/* rationes operturae unius pro via una; *tegit si via tecta */
interior vacuum
_viam_iudicare (
      constans PortaeSutura* sutura,
    constans PortaeOpertura* o,
                     chorda  v,
                        Xar* memoria,
                        Xar* rationes,
                        b32* tegit,
                    Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(o->exemplaria); i++)
    {
        constans PortaeExemplar* e = (constans PortaeExemplar*)
            xar_obtinere(o->exemplaria, i);

        si (!_exemplar_congruit(e, v))
        {
            perge;
        }
        *tegit = VERUM;
        _rationem_addere(rationes, e->genus == PORTAE_EXEMPLAR_VIA
            ? _ratio(piscina, "", v, " (", e->ratio, ")")
            : _ratio(piscina, "", v, "", _ratio(piscina, " congruit '",
                e->exemplar, "' (", e->ratio, ")"), ""));
    }
    per (i = ZEPHYRUM; i < xar_numerus(o->fontes_c); i++)
    {
        constans PortaeExemplar* f = (constans PortaeExemplar*)
            xar_obtinere(o->fontes_c, i);
                            Xar* clausura;

        si (chorda_aequalis(v, f->exemplar))
        {
            *tegit = VERUM;
            _rationem_addere(rationes, _ratio(piscina, "", v, " (",
                f->ratio, ")"));
            perge;
        }
        si (!_via_c(v))
        {
            perge;
        }
        clausura = _clausuram_obtinere(sutura, memoria, f->exemplar,
            piscina);
        si (clausura == NIHIL)
        {
            *tegit = VERUM;
            _rationem_addere(rationes, _ratio(piscina, "", v,
                ": clausura ", f->exemplar, " ignota - debetur"));
        }
        alioquin si (_chorda_in(clausura, v))
        {
            *tegit = VERUM;
            _rationem_addere(rationes, _ratio(piscina, "", v, "",
                _ratio(piscina, " in clausura ", f->exemplar, " (",
                f->ratio, ")"), ""));
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(o->ignota); i++)
    {
        _rationem_addere(rationes, _ratio(piscina, "", v, ": ",
            *(constans chorda*)xar_obtinere(o->ignota, i),
            " - debetur"));
    }
}

/* "ratio prima (+N aliae)" */
interior chorda
_causam_debiti (
    constans Xar* rationes,
         Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);

    chorda_aedificator_appendere_chorda(a, *(constans chorda*)
        xar_obtinere(rationes, ZEPHYRUM));
    si (xar_numerus(rationes) > I)
    {
        chorda_aedificator_appendere_literis(a, " (+");
        chorda_aedificator_appendere_i32(a, xar_numerus(rationes) - I);
        chorda_aedificator_appendere_literis(a, " aliae)");
    }
    redde chorda_aedificator_finire(a);
}

/* porta operturae: titulus, aut via cursoris (manu sine registro) */
interior chorda
_porta_operturae (
    constans PortaeOpertura* o)
{
    redde o->declaratio->titulus.mensura > ZEPHYRUM
        ? o->declaratio->titulus : o->declaratio->via;
}

Xar*
portae_tactus_computare (
    constans PortaeSutura*  sutura,
             constans Xar*  operturae,
             constans Xar*  viae,
                      Xar** intecta_out,
                  Piscina*  piscina)
{
    Xar* tactus;
    Xar* memoria;
    Xar* mutatae;
    Xar* tecta;     /* b32 per viam */
    i32  i;
    i32  j;

    si (sutura == NIHIL || operturae == NIHIL || viae == NIHIL)
    {
        redde NIHIL;
    }
    tactus   = xar_creare(piscina, (i32)magnitudo(PortaeTactus));
    memoria  = xar_creare(piscina, (i32)magnitudo(PortaeClausura));
    mutatae  = xar_creare(piscina, (i32)magnitudo(chorda));
    tecta    = xar_creare(piscina, (i32)magnitudo(b32));
    si (   tactus == NIHIL || memoria == NIHIL || mutatae == NIHIL
        || tecta  == NIHIL)
    {
        redde NIHIL;
    }
    per (j = ZEPHYRUM; j < xar_numerus(viae); j++)
    {
        *(chorda*)xar_addere(mutatae) = _via_norma(*(constans chorda*)
            xar_obtinere(viae, j));
        *(b32*)xar_addere(tecta) = FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(operturae); i++)
    {
        constans PortaeOpertura* o = (constans PortaeOpertura*)
            xar_obtinere(operturae, i);

        per (j = ZEPHYRUM; j < xar_numerus(mutatae); j++)
        {
            Xar* rationes = xar_creare(piscina,
                (i32)magnitudo(chorda));
            PortaeTactus* t;

            _viam_iudicare(sutura, o, *(chorda*)xar_obtinere(mutatae,
                j),
                memoria, rationes, (b32*)xar_obtinere(tecta, j),
                piscina);
            si (xar_numerus(rationes) == ZEPHYRUM)
            {
                perge;
            }
            t            = (PortaeTactus*)xar_addere(tactus);
            t->via       = *(chorda*)xar_obtinere(mutatae, j);
            t->porta     = _porta_operturae(o);
            t->manu      = o->declaratio->manu;
            t->rationes  = rationes;
            t->opertura  = o;
        }
    }
    si (intecta_out != NIHIL)
    {
        *intecta_out = xar_creare(piscina, (i32)magnitudo(chorda));
        per (j = ZEPHYRUM; j < xar_numerus(mutatae); j++)
        {
            si (!*(b32*)xar_obtinere(tecta, j))
            {
                *(chorda*)xar_addere(*intecta_out) = *(chorda*)
                    xar_obtinere(mutatae, j);
            }
        }
    }
    redde tactus;
}

/* debitum operturae unius ex tactibus eius (ordine viarum) */
interior vacuum
_debitum_addere (
                        Xar* debita,
               constans Xar* tactus,
    constans PortaeOpertura* o,
                    Piscina* piscina)
{
              Xar* rationes = xar_creare(piscina,
                  (i32)magnitudo(chorda));
    PortaeDebitum* d;
              i32  i;
              i32  k;

    per (i = ZEPHYRUM; i < xar_numerus(tactus); i++)
    {
        constans PortaeTactus* t = (constans PortaeTactus*)xar_obtinere(
            tactus, i);

        per (k = ZEPHYRUM; t->opertura == o
             && k < xar_numerus(t->rationes); k++)
        {
            _rationem_addere(rationes,
                *(chorda*)xar_obtinere(t->rationes,
                k));
        }
    }
    si (xar_numerus(rationes) == ZEPHYRUM)
    {
        redde;
    }
    d           = (PortaeDebitum*)xar_addere(debita);
    d->porta    = _porta_operturae(o);
    d->causa    = _causam_debiti(rationes, piscina);
    d->manu     = o->declaratio->manu;
    d->binaria  = o->binaria;
}

Xar*
portae_debita_computare (
    constans PortaeSutura*  sutura,
             constans Xar*  operturae,
             constans Xar*  viae,
                      Xar** intecta_out,
                  Piscina*  piscina)
{
    Xar* tactus;
    Xar* debita;
    i32  i;
    i32  k;

    tactus = portae_tactus_computare(sutura, operturae, viae,
        intecta_out,
        piscina);
    debita = xar_creare(piscina, (i32)magnitudo(PortaeDebitum));
    si (tactus == NIHIL || debita == NIHIL)
    {
        redde NIHIL;
    }
    /* ordo: registrum (portae non manu), deinde manu ordine
     * operturarum */
    per (k = ZEPHYRUM; sutura->tabula_portarum != NIHIL
         && k < xar_numerus(sutura->tabula_portarum); k++)
    {
        constans PortaeIngressus* p = (constans PortaeIngressus*)
            xar_obtinere(sutura->tabula_portarum, k);

        per (i = ZEPHYRUM; i < xar_numerus(operturae); i++)
        {
            constans PortaeOpertura* o = (constans PortaeOpertura*)
                xar_obtinere(operturae, i);

            si (   !o->declaratio->manu
                && chorda_aequalis(o->declaratio->titulus, p->clavis))
            {
                _debitum_addere(debita, tactus, o, piscina);
                frange;
            }
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(operturae); i++)
    {
        constans PortaeOpertura* o = (constans PortaeOpertura*)
            xar_obtinere(operturae, i);

        si (o->declaratio->manu)
        {
            _debitum_addere(debita, tactus, o, piscina);
        }
    }
    redde debita;
}


/* ==================================================
 * EFFUSIO (plan T3)
 * ================================================== */

interior vacuum
_campum_appendere (
     ChordaAedificator* a,
                chorda  campus)
{
    i32 i;

    per (i = ZEPHYRUM; i < campus.mensura; i++)
    {
        character c = (character)campus.datum[i];

        si (c == '\\')
        {
            chorda_aedificator_appendere_literis(a, "\\\\");
        }
        alioquin si (c == '\t')
        {
            chorda_aedificator_appendere_literis(a, "\\t");
        }
        alioquin si (c == '\n')
        {
            chorda_aedificator_appendere_literis(a, "\\n");
        }
        alioquin si (c == '\r')
        {
            chorda_aedificator_appendere_literis(a, "\\r");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, c);
        }
    }
}

chorda
portae_campum_effugere (
      chorda  campus,
     Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);

    _campum_appendere(a, campus);
    redde chorda_aedificator_finire(a);
}

/* "\t<campus>" */
interior vacuum
_campus (
     ChordaAedificator* a,
                chorda  campus)
{
    chorda_aedificator_appendere_character(a, '\t');
    _campum_appendere(a, campus);
}

interior vacuum
_campus_numerus (
     ChordaAedificator* a,
                   i32  n)
{
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_i32(a, n);
}

/* recusationes: machina RECUSATIO, homo 'via:linea: causa' */
interior vacuum
_recusationes_scribere (
    ChordaAedificator* a,
         constans Xar* recusationes,
                  b32  machina)
{
    i32 i;

    si (!machina)
    {
        chorda_aedificator_appendere_literis(a,
            "RECUSATIO (declarationes portarum):\n");
    }
    per (i = ZEPHYRUM; i < xar_numerus(recusationes); i++)
    {
        constans PortaeRecusatio* r = (constans PortaeRecusatio*)
            xar_obtinere(recusationes, i);

        si (machina)
        {
            chorda_aedificator_appendere_literis(a, "RECUSATIO");
            _campus(a, r->via);
            _campus_numerus(a, r->linea);
            _campus(a, r->causa);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "  ");
            si (r->via.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_chorda(a, r->via);
                si (r->linea > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_character(a, ':');
                    chorda_aedificator_appendere_i32(a, r->linea);
                }
                chorda_aedificator_appendere_literis(a, ": ");
            }
            chorda_aedificator_appendere_chorda(a, r->causa);
        }
        chorda_aedificator_appendere_character(a, '\n');
    }
}

/* homo: debita manu aut non, cum titulo sectionis */
interior b32
_debita_hominis (
    ChordaAedificator* a,
         constans Xar* debita,
                  b32  manu)
{
    b32 ullum = FALSUM;
    i32 i;

    per (i = ZEPHYRUM; debita != NIHIL && i < xar_numerus(debita); i++)
    {
        constans PortaeDebitum* d = (constans PortaeDebitum*)
            xar_obtinere(debita, i);

        si (d->manu != manu)
        {
            perge;
        }
        si (manu && !ullum)
        {
            chorda_aedificator_appendere_literis(a,
                "MANU (homo currit):\n");
        }
        ullum = VERUM;
        chorda_aedificator_appendere_literis(a, "  ");
        chorda_aedificator_appendere_chorda(a, d->porta);
        chorda_aedificator_appendere_literis(a, "\n      ");
        chorda_aedificator_appendere_chorda(a, d->causa);
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde ullum;
}

chorda
portae_debita_scribere (
    constans Xar* debita,
    constans Xar* intecta,
    constans Xar* recusationes,
             i32  viae_numerus,
             b32  machina,
         Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
                   i32 i;

    si (recusationes != NIHIL && xar_numerus(recusationes) > ZEPHYRUM)
    {
        _recusationes_scribere(a, recusationes, machina);
        redde chorda_aedificator_finire(a);
    }
    si (machina)
    {
        per (i = ZEPHYRUM; debita != NIHIL && i < xar_numerus(debita);
             i++)
        {
            constans PortaeDebitum* d = (constans PortaeDebitum*)
                xar_obtinere(debita, i);

            i32 k;

            chorda_aedificator_appendere_literis(a, "DEBITUM");
            _campus(a, d->porta);
            _campus_numerus(a, d->manu ? I : ZEPHYRUM);
            _campus(a, d->causa);
            chorda_aedificator_appendere_character(a, '\n');
            per (k = ZEPHYRUM; !d->manu && d->binaria != NIHIL
                 && k < xar_numerus(d->binaria); k++)
            {
                constans PortaeExemplar* b = (constans PortaeExemplar*)
                    xar_obtinere(d->binaria, k);

                chorda_aedificator_appendere_literis(a, "BINARIUM");
                _campus(a, d->porta);
                _campus(a, b->exemplar);
                _campus(a, b->ratio);
                chorda_aedificator_appendere_character(a, '\n');
            }
        }
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "PORTAE DEBITAE (");
        chorda_aedificator_appendere_i32(a, viae_numerus);
        chorda_aedificator_appendere_literis(a, viae_numerus == I
            ? " via):\n" : " viae):\n");
        si (!_debita_hominis(a, debita, FALSUM))
        {
            chorda_aedificator_appendere_literis(a, "  (nulla)\n");
        }
        (vacuum)_debita_hominis(a, debita, VERUM);
        si (intecta != NIHIL && xar_numerus(intecta) > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a,
                "INTECTA (nulla porta):\n");
        }
    }
    per (i = ZEPHYRUM; intecta != NIHIL
        && i < xar_numerus(intecta); i++)
    {
        chorda_aedificator_appendere_literis(a, machina ? "INTECTUM\t"
            : "  ");
        si (machina)
        {
            _campum_appendere(a,
                *(constans chorda*)xar_obtinere(intecta,
                i));
        }
        alioquin
        {
            chorda_aedificator_appendere_chorda(a, *(constans chorda*)
                xar_obtinere(intecta, i));
        }
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde chorda_aedificator_finire(a);
}

interior constans character*
_genus_nomen (
    PortaeExemplarGenus genus)
{
    redde genus == PORTAE_EXEMPLAR_GLOBUS ? "globus"
        : genus == PORTAE_EXEMPLAR_PRAEFIXUM ? "praefixum" : "via";
}

/* lineae 'GENUS clavis textus' pro Xar de chorda */
interior vacuum
_lineas_textuum (
      ChordaAedificator* a,
     constans character* genus,
                 chorda  clavis,
           constans Xar* textus,
                    b32  machina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(textus); i++)
    {
        chorda t = *(constans chorda*)xar_obtinere(textus, i);

        si (machina)
        {
            chorda_aedificator_appendere_literis(a, genus);
            _campus(a, clavis);
            _campus(a, t);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "    ");
            chorda_aedificator_appendere_literis(a, genus);
            chorda_aedificator_appendere_literis(a, "  ");
            chorda_aedificator_appendere_chorda(a, t);
        }
        chorda_aedificator_appendere_character(a, '\n');
    }
}

chorda
portae_declaratas_scribere (
    constans Xar* operturae,
    constans Xar* recusationes,
             b32  machina,
         Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
                   i32 i;
                   i32 j;

    si (recusationes != NIHIL && xar_numerus(recusationes) > ZEPHYRUM)
    {
        _recusationes_scribere(a, recusationes, machina);
        redde chorda_aedificator_finire(a);
    }
    per (i = ZEPHYRUM; operturae != NIHIL && i < xar_numerus(operturae);
         i++)
    {
        constans PortaeOpertura* o = (constans PortaeOpertura*)
            xar_obtinere(operturae, i);
         constans PortaDeclarata* d = o->declaratio;
                          chorda  clavis;

        clavis = d->titulus.mensura > ZEPHYRUM ? d->titulus : d->via;
        si (machina)
        {
            chorda_aedificator_appendere_literis(a, "PORTA");
            _campus(a, clavis);
            _campus_numerus(a, d->manu ? I : ZEPHYRUM);
            _campus(a, d->via);
            _campus_numerus(a, d->linea);
            _campus(a, d->causa);
        }
        alioquin
        {
            chorda_aedificator_appendere_chorda(a, clavis);
            chorda_aedificator_appendere_literis(a, "  ");
            chorda_aedificator_appendere_chorda(a, _locus(piscina, d));
            si (d->manu)
            {
                chorda_aedificator_appendere_literis(a, "  MANU: ");
                chorda_aedificator_appendere_chorda(a, d->causa);
            }
        }
        chorda_aedificator_appendere_character(a, '\n');
        per (j = ZEPHYRUM; j < xar_numerus(d->instrumenta); j++)
        {
            chorda b = *(chorda*)xar_obtinere(d->instrumenta, j);

            si (machina)
            {
                chorda_aedificator_appendere_literis(a, "INSTRUMENTUM");
                _campus(a, clavis);
                _campus(a, b);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(a,
                    "    instrumentum ");
                chorda_aedificator_appendere_chorda(a, b);
            }
            chorda_aedificator_appendere_character(a, '\n');
        }
        per (j = ZEPHYRUM; j < xar_numerus(o->exemplaria); j++)
        {
            constans PortaeExemplar* e = (constans PortaeExemplar*)
                xar_obtinere(o->exemplaria, j);

            si (machina)
            {
                chorda_aedificator_appendere_literis(a, "TEGIT");
                _campus(a, clavis);
                chorda_aedificator_appendere_character(a, '\t');
                chorda_aedificator_appendere_literis(a,
                    _genus_nomen(e->genus));
                _campus(a, e->exemplar);
                _campus(a, e->ratio);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(a,
                    "    tegit     ");
                chorda_aedificator_appendere_chorda(a, e->exemplar);
                chorda_aedificator_appendere_literis(a, "  (");
                chorda_aedificator_appendere_chorda(a, e->ratio);
                chorda_aedificator_appendere_character(a, ')');
            }
            chorda_aedificator_appendere_character(a, '\n');
        }
        per (j = ZEPHYRUM; j < xar_numerus(o->fontes_c); j++)
        {
            constans PortaeExemplar* f = (constans PortaeExemplar*)
                xar_obtinere(o->fontes_c, j);

            si (machina)
            {
                chorda_aedificator_appendere_literis(a, "CLAUSURA");
                _campus(a, clavis);
                _campus(a, f->exemplar);
                _campus(a, f->ratio);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(a,
                    "    clausura  ");
                chorda_aedificator_appendere_chorda(a, f->exemplar);
                chorda_aedificator_appendere_literis(a, "  (");
                chorda_aedificator_appendere_chorda(a, f->ratio);
                chorda_aedificator_appendere_character(a, ')');
            }
            chorda_aedificator_appendere_character(a, '\n');
        }
        _lineas_textuum(a, machina ? "IGNOTUM" : "IGNOTUM ", clavis,
            o->ignota, machina);
        _lineas_textuum(a, machina ? "NOTA" : "nota    ", clavis,
            o->notae, machina);
    }
    redde chorda_aedificator_finire(a);
}

/* lineae tactuum: machina TANGIT <via> <porta> <manu> <ratio> (linea
 * per rationem); homo per viam */
chorda
portae_tactus_scribere (
    constans Xar* tactus,
    constans Xar* intecta,
             b32  machina,
         Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)CCLVI);
                   i32 i;
                   i32 k;

    per (i = ZEPHYRUM; tactus != NIHIL && i < xar_numerus(tactus); i++)
    {
        constans PortaeTactus* t = (constans PortaeTactus*)xar_obtinere(
            tactus, i);

        si (!machina)
        {
            chorda_aedificator_appendere_chorda(a, t->via);
            chorda_aedificator_appendere_literis(a, " -> ");
            chorda_aedificator_appendere_chorda(a, t->porta);
            chorda_aedificator_appendere_literis(a,
                t->manu ? " (MANU)\n"
                : "\n");
        }
        per (k = ZEPHYRUM; k < xar_numerus(t->rationes); k++)
        {
            chorda r = *(chorda*)xar_obtinere(t->rationes, k);

            si (machina)
            {
                chorda_aedificator_appendere_literis(a, "TANGIT");
                _campus(a, t->via);
                _campus(a, t->porta);
                _campus_numerus(a, t->manu ? I : ZEPHYRUM);
                _campus(a, r);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(a, "      ");
                chorda_aedificator_appendere_chorda(a, r);
            }
            chorda_aedificator_appendere_character(a, '\n');
        }
    }
    per (i = ZEPHYRUM; intecta != NIHIL
        && i < xar_numerus(intecta); i++)
    {
        chorda_aedificator_appendere_literis(a, machina ? "INTECTUM\t"
            : "INTECTUM ");
        _campum_appendere(a, *(constans chorda*)xar_obtinere(intecta,
            i));
        chorda_aedificator_appendere_character(a, '\n');
    }
    redde chorda_aedificator_finire(a);
}
