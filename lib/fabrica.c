/* fabrica.c - nucleus iudicis fabricae (T1)
 *
 * Machina pura: discum et processus per FabricaSutura solam tangit.
 * Vide include/fabrica.h et project-specs/fabrica-spec-v2.md. */

#include "fabrica.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "stml.h"
#include "stml_macros.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_iungere (
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post)
{
    ChordaAedificator* aedificator;

    aedificator = chorda_aedificator_creare(piscina, 128);
    si (aedificator == NIHIL)
    {
        redde medium;
    }
    (vacuum)chorda_aedificator_appendere_literis(aedificator, ante);
    (vacuum)chorda_aedificator_appendere_chorda(aedificator, medium);
    (vacuum)chorda_aedificator_appendere_literis(aedificator, post);
    redde chorda_aedificator_finire(aedificator);
}

interior vacuum
_causam_ponere (
                 chorda* causa_out,
                Piscina* piscina,
     constans character* ante,
                 chorda  medium,
     constans character* post)
{
    si (causa_out != NIHIL)
    {
        *causa_out = _iungere(piscina, ante, medium, post);
    }
}

interior Xar*
_xar_chordarum (
    Piscina* piscina)
{
    redde xar_creare(piscina, (i32)magnitudo(chorda));
}

interior vacuum
_chordam_addere (
       Xar* xar,
    chorda  valor)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = valor;
    }
}

vacuum
fabrica_suturam_parare (
    FabricaSutura* sutura)
{
    sutura->datum               = NIHIL;
    sutura->legere              = NIHIL;
    sutura->enumerare           = NIHIL;
    sutura->currere             = NIHIL;
    sutura->rogare              = NIHIL;
    sutura->meminisse           = NIHIL;
    sutura->inscribere          = NIHIL;
    sutura->sigilla             = NIHIL;
    sutura->regenerationes      = NIHIL;
    sutura->digesta             = NIHIL;
    sutura->agere               = NIHIL;
    sutura->agere_simul         = NIHIL;
    sutura->vestigium_capere    = NIHIL;
    sutura->cursum_inscribere   = NIHIL;
    sutura->cursum_legere       = NIHIL;
    sutura->lectiones_legere    = NIHIL;
    sutura->lectiones_scribere  = NIHIL;
    sutura->radix.datum         = NIHIL;
    sutura->radix.mensura       = ZEPHYRUM;
    sutura->auditus             = ZEPHYRUM;
    sutura->lectiones_ultimae   = NIHIL;
}


/* ==================================================
 * Manifestum aedilis
 * ================================================== */

interior vacuum
_sectionem_colligere (
             StmlNodus* radix,
    constans character* sectio_titulus,
    constans character* liberi_titulus,
               Piscina* piscina,
                   Xar* viae)
{
    StmlNodus* sectio;
          Xar* liberi;
          i32  i;
          i32  numerus;

    sectio = stml_invenire_liberum(radix, sectio_titulus);
    si (sectio == NIHIL)
    {
        redde;
    }
    liberi = stml_invenire_omnes_liberos(sectio, liberi_titulus,
        piscina);
    si (liberi == NIHIL)
    {
        redde;
    }
    numerus = xar_numerus(liberi);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* nodus;
           chorda* via;

        nodus  = *(StmlNodus**)xar_obtinere(liberi, i);
        via    = stml_attributum_capere(nodus, "via");
        si (via != NIHIL)
        {
            _chordam_addere(viae, *via);
        }
    }
}

b32
fabrica_manifestum_legere (
                 chorda   contentum,
                Piscina*  piscina,
                    Xar** viae_out,
                    Xar** inresolutae_out,
                 chorda*  causa_out)
{
    InternamentumChorda* intern;
           StmlResultus  lectum;

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde FALSUM;
    }
    lectum = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "aedilis-manifestum"))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_ex_literis("manifestum malformatum",
                piscina);
        }
        redde FALSUM;
    }
    *viae_out         = _xar_chordarum(piscina);
    *inresolutae_out  = _xar_chordarum(piscina);
    /* SCOPUS = fons principalis: manifestum eum in attributo SOLO
     * nominat, numquam inter obiecta - sine hoc mutatio fontis
     * principalis binarium stalum non faceret (T7, inventum per
     * digesta aequalia canon_examen / canon_coquere) */
    {
        chorda* scopus;

        scopus = stml_attributum_capere(lectum.elementum_radix,
            "scopus");
        si (scopus != NIHIL)
        {
            _chordam_addere(*viae_out, *scopus);
        }
    }
    /* systemata consulto omissa: plagulae nostrae non sunt */
    _sectionem_colligere(lectum.elementum_radix, "obiecta", "obiectum",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "capita", "caput",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "vendores", "fons",
        piscina, *viae_out);
    _sectionem_colligere(lectum.elementum_radix, "inresolutae",
        "caput", piscina, *inresolutae_out);
    redde VERUM;
}


/* ==================================================
 * Sigillum copiae ingressuum
 * ================================================== */

interior s32
_particulas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaParticula*)a)->via,
        ((constans FabricaParticula*)b)->via);
}

interior b32
_exclusum_est (
     constans Xar* exclusa,
           chorda  via)
{
    i32 i;
    i32 numerus;

    si (exclusa == NIHIL)
    {
        redde FALSUM;
    }
    numerus = xar_numerus(exclusa);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda exclusum;

        exclusum = *(chorda*)xar_obtinere(exclusa, i);
        /* '/' finalis: directorium totum (1b T5: provenientia) */
        si (   chorda_aequalis(exclusum, via)
            || (   exclusum.mensura > 0
                && exclusum.datum[exclusum.mensura - I] == '/'
                && chorda_incipit(via, exclusum)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_particulam_legere (
     constans FabricaSutura* sutura,
                     chorda  via,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
    FabricaParticula* particula;
              chorda  contentum;
              vacuum* memoratum;
            Sigillum  octeti;

    si (_exclusum_est(exclusa, via))
    {
        redde VERUM;
    }
    si (   sutura->sigilla != NIHIL
        && tabula_dispersa_invenire(sutura->sigilla, via, &memoratum))
    {
        octeti = *(Sigillum*)memoratum;
    }
    alioquin
    {
        si (!sutura->legere(sutura->datum,
                chorda_ut_cstr(via, piscina), piscina, &contentum))
        {
            _causam_ponere(causa_out, piscina, "ingressus absens: ",
                via, "");
            redde FALSUM;
        }
        octeti = sigillum_computare(contentum.datum,
            (memoriae_index)contentum.mensura);
        si (sutura->sigilla != NIHIL)
        {
            Sigillum* locus;

            locus = (Sigillum*)piscina_allocare(piscina,
                magnitudo(Sigillum));
            si (locus != NIHIL)
            {
                *locus = octeti;
                (vacuum)tabula_dispersa_inserere(sutura->sigilla, via,
                    locus);
            }
        }
    }
    particula = (FabricaParticula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via     = via;
    particula->octeti  = octeti;
    redde VERUM;
}

interior b32
_manifestum_explicare (
     constans FabricaSutura* sutura,
                     chorda  via,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
    chorda  contentum;
       Xar* viae;
       Xar* inresolutae;
       i32  i;
       i32  numerus;

    si (!sutura->legere(sutura->datum, chorda_ut_cstr(via, piscina),
            piscina, &contentum))
    {
        _causam_ponere(causa_out, piscina, "manifestum absens: ", via,
            "");
        redde FALSUM;
    }
    si (!fabrica_manifestum_legere(contentum, piscina, &viae,
            &inresolutae, causa_out))
    {
        _causam_ponere(causa_out, piscina, "manifestum malformatum: ",
            via, "");
        redde FALSUM;
    }
    /* Q9: incompletum numquam sigillatur - nominatur */
    si (xar_numerus(inresolutae) > 0)
    {
        _causam_ponere(causa_out, piscina,
            "manifestum incompletum: ",
            _iungere(piscina, "", via, ": inclusio citata inresoluta "),
            chorda_ut_cstr(*(chorda*)xar_obtinere(inresolutae,
                ZEPHYRUM), piscina));
        redde FALSUM;
    }
    numerus = xar_numerus(viae);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!_particulam_legere(sutura, *(chorda*)xar_obtinere(viae, i),
                exclusa, piscina, particulae, causa_out))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_directorium_explicare (
     constans FabricaSutura* sutura,
                     chorda  via,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
           ChordaAedificator* aedificator;
                         Xar* nomina;
            FabricaParticula* particula;
                         i32  i;
                         i32  numerus;
                      chorda  clavis;
                      vacuum* memoratum;

    /* memoria per cursum (clavis cum '/' finali, ut in particula):
     * radices inclusionum ab actionibus multis nominantur */
    clavis = _iungere(piscina, "", via, "/");
    si (   sutura->sigilla != NIHIL
        && tabula_dispersa_invenire(sutura->sigilla, clavis,
               &memoratum))
    {
        particula = (FabricaParticula*)xar_addere(particulae);
        si (particula == NIHIL)
        {
            redde FALSUM;
        }
        particula->via     = clavis;
        particula->octeti  = *(Sigillum*)memoratum;
        redde VERUM;
    }
    si (   sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum,
               chorda_ut_cstr(via, piscina), piscina, &nomina))
    {
        _causam_ponere(causa_out, piscina, "directorium absens: ", via,
            "");
        redde FALSUM;
    }
    aedificator = chorda_aedificator_creare(piscina, 256);
    si (aedificator == NIHIL)
    {
        redde FALSUM;
    }
    numerus = xar_numerus(nomina);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            *(chorda*)xar_obtinere(nomina, i));
        (vacuum)chorda_aedificator_appendere_character(aedificator,
            '\n');
    }
    particula = (FabricaParticula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    /* '/' finalis: directorium numquam cum plagula eiusdem viae
     * confunditur */
    particula->via        = clavis;
    {
        chorda nomina_iuncta;

        nomina_iuncta = chorda_aedificator_finire(aedificator);
        particula->octeti = sigillum_computare(nomina_iuncta.datum,
            (memoriae_index)nomina_iuncta.mensura);
    }
    si (sutura->sigilla != NIHIL)
    {
        Sigillum* locus;

        locus = (Sigillum*)piscina_allocare(piscina,
            magnitudo(Sigillum));
        si (locus != NIHIL)
        {
            *locus = particula->octeti;
            (vacuum)tabula_dispersa_inserere(sutura->sigilla, clavis,
                locus);
        }
    }
    redde VERUM;
}

/* VERUM si nomen uno suffixorum (spatio separatorum) terminatur;
 * suffixa vacua = omnia */
interior b32
_suffixo_congruit (
    chorda titulus,
    chorda suffixa)
{
    i32 i;

    si (suffixa.mensura == 0)
    {
        redde VERUM;
    }
    i = ZEPHYRUM;
    dum (i < suffixa.mensura)
    {
        i32 initium;

        dum (i < suffixa.mensura && suffixa.datum[i] == ' ')
        {
            i++;
        }
        initium = i;
        dum (i < suffixa.mensura && suffixa.datum[i] != ' ')
        {
            i++;
        }
        si (   i > initium
            && chorda_terminatur(titulus,
                   chorda_sectio(suffixa, initium, i)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* PLAGULAE: plagulae directorii profunditate 0, suffixis filtratae,
 * CONTENTIS sigillatae (via cuiusque in sigillo - plagula nova aut
 * deleta sigillum mutat). Subdirectoria praetermittuntur (corpus
 * infixum globis 'lib/X.c' profunditate 0 utitur). */
interior b32
_plagulas_explicare (
        constans FabricaSutura* sutura,
     constans FabricaIngressus* ingressus,
                  constans Xar* exclusa,
                       Piscina* piscina,
                           Xar* particulae,
                        chorda* causa_out)
{
    Xar* nomina;
    i32  i;

    si (   sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum,
               chorda_ut_cstr(ingressus->via, piscina), piscina,
               &nomina))
    {
        _causam_ponere(causa_out, piscina, "directorium absens: ",
            ingressus->via, "");
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        chorda  titulus;
        chorda  via;
           Xar* filii;

        titulus = *(chorda*)xar_obtinere(nomina, i);
        si (!_suffixo_congruit(titulus, ingressus->suffixa))
        {
            perge;
        }
        via = _iungere(piscina, "", _iungere(piscina, "",
            ingressus->via, "/"), chorda_ut_cstr(titulus, piscina));
        si (sutura->enumerare(sutura->datum,
                chorda_ut_cstr(via, piscina), piscina, &filii))
        {
            perge;   /* subdirectorium */
        }
        si (!_particulam_legere(sutura, via, exclusa, piscina,
                particulae, causa_out))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* MANIFESTA: directorium manifestorum (fragmenta T6: generator unam
 * clausuram per radicem scribit). Quodque '.stml' gradu 0 explicatur
 * ut MANIFESTUM; nomina directorii quoque sigillantur (radix deleta
 * manifestum suum aufert). Directorium absens (clonus recens,
 * generator numquam cucurrit) -> FALSUM nominatum. */
interior b32
_manifesta_explicare (
     constans FabricaSutura* sutura,
                     chorda  via,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
    Xar* nomina;
    i32  i;

    si (!_directorium_explicare(sutura, via, piscina, particulae,
            causa_out))
    {
        redde FALSUM;
    }
    (vacuum)sutura->enumerare(sutura->datum, chorda_ut_cstr(via,
        piscina), piscina, &nomina);
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        chorda titulus;

        titulus = *(chorda*)xar_obtinere(nomina, i);
        si (!chorda_terminatur(titulus, chorda_ex_literis(".stml",
                piscina)))
        {
            perge;
        }
        si (!_manifestum_explicare(sutura, _iungere(piscina, "",
                _iungere(piscina, "", via, "/"),
                chorda_ut_cstr(titulus, piscina)),
                exclusa, piscina, particulae, causa_out))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* RADICES: configuratio aedilis legitur, sectionis inclusarum viae
 * (capturae crudae) - nomina cuiusque directorii sigillantur (caput
 * novum in radice resolutionem obumbrare potest, Review Focus 2).
 * Sectio absens aut vacua -> FALSUM: radices nullae = tegmen nullum,
 * numquam tacite. */
interior b32
_radices_explicare (
     constans FabricaSutura* sutura,
                     chorda  via,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
    InternamentumChorda* intern;
           StmlResultus  lectum;
                 chorda  contentum;
              StmlNodus* inclusa;
                    Xar* nodi;
                    i32  i;

    si (!sutura->legere(sutura->datum, chorda_ut_cstr(via, piscina),
            piscina, &contentum))
    {
        _causam_ponere(causa_out, piscina, "configuratio absens: ",
            via, "");
        redde FALSUM;
    }
    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde FALSUM;
    }
    lectum   = stml_legere(contentum, piscina, intern);
    inclusa  = (lectum.successus && lectum.elementum_radix != NIHIL)
        ? stml_invenire_liberum(lectum.elementum_radix, "inclusa")
        : NIHIL;
    nodi = (inclusa != NIHIL)
        ? stml_invenire_omnes_liberos(inclusa, "via", piscina) : NIHIL;
    si (nodi == NIHIL || xar_numerus(nodi) == 0)
    {
        _causam_ponere(causa_out, piscina,
            "configuratio sine sectione inclusa: ", via, "");
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nodi); i++)
    {
        chorda radix;

        radix = chorda_praecidere(stml_textus_valor(
            *(StmlNodus**)xar_obtinere(nodi, i), piscina));
        si (!_directorium_explicare(sutura, radix, piscina,
                particulae, causa_out))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Genera: verba sigillare, enumerare, locare
 *
 * Corpora explicationum supra (_X_explicare) immota manent; hic
 * solum signaturae communes et registrum. Nomina tria (fasciculus,
 * configuratio, instrumentum) implementationem unam habent: octeti
 * plagulae - nomina documentationis sunt (spec 1b par. III).
 * ================================================== */

interior b32
_fasciculum_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    redde _particulam_legere(sutura, ingressus->via, exclusa, piscina,
        particulae, causa_out);
}

interior b32
_manifestum_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    redde _manifestum_explicare(sutura, ingressus->via, exclusa,
        piscina, particulae, causa_out);
}

interior b32
_directorium_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    (vacuum)exclusa;
    redde _directorium_explicare(sutura, ingressus->via, piscina,
        particulae, causa_out);
}

interior b32
_manifesta_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    redde _manifesta_explicare(sutura, ingressus->via, exclusa,
        piscina, particulae, causa_out);
}

interior b32
_radices_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    (vacuum)exclusa;
    redde _radices_explicare(sutura, ingressus->via, piscina,
        particulae, causa_out);
}

interior vacuum
_locum_addere (
                 Xar* loci,
    FabricaFormaLoci  forma,
              chorda  via,
              chorda  suffixa)
{
    FabricaLocus* locus;

    locus = (FabricaLocus*)xar_addere(loci);
    si (locus != NIHIL)
    {
        locus->forma    = forma;
        locus->via      = via;
        locus->suffixa  = suffixa;
    }
}

/* enumerare ordinarium: particula plagulae -> PLAGULA; particula
 * directorii ('dir/') -> PLAGULAE sine suffixis (nomen novum quodvis
 * in eo tangit) */
interior vacuum
_loci_ex_particulis (
    constans Xar* particulae,
         Piscina* piscina,
             Xar* loci)
{
    chorda vacua;
       i32 i;

    vacua = chorda_ex_literis("", piscina);
    per (i = ZEPHYRUM; i < xar_numerus(particulae); i++)
    {
        chorda via;

        via = ((FabricaParticula*)xar_obtinere(particulae, i))->via;
        si (via.mensura > 0 && via.datum[via.mensura - I] == '/')
        {
            _locum_addere(loci, FABRICA_LOCUS_PLAGULAE,
                chorda_sectio(via, 0, via.mensura - I), vacua);
        }
        alioquin
        {
            _locum_addere(loci, FABRICA_LOCUS_PLAGULA, via, vacua);
        }
    }
}

/* plagulae: plagulae congruentes (ex particulis) ET directorium
 * ipsum suffixis filtratum - plagula NOVA congruens tangit */
interior b32
_plagulas_enumerare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                      Piscina* piscina,
                          Xar* loci,
                       chorda* causa_out)
{
    Xar* particulae;

    particulae = xar_creare(piscina,
        (i32)magnitudo(FabricaParticula));
    si (   particulae == NIHIL
        || !_plagulas_explicare(sutura, ingressus, NIHIL, piscina,
               particulae, causa_out))
    {
        redde FALSUM;
    }
    _loci_ex_particulis(particulae, piscina, loci);
    _locum_addere(loci, FABRICA_LOCUS_PLAGULAE, ingressus->via,
        ingressus->suffixa);
    redde VERUM;
}

/* exitus plagula una: locus unus */
interior b32
_plagulam_locare (
    constans FabricaExitus* exitus,
                   Piscina* piscina,
                       Xar* loci)
{
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, exitus->via,
        chorda_ex_literis("", piscina));
    redde VERUM;
}

/* ordo membrorum: titulus, sigillare, enumerare, locare,
 * reproducibile */
interior constans FabricaGenus _genus_fasciculus = {
    "fasciculus", _fasciculum_sigillare, NIHIL, _plagulam_locare,
    VERUM
};

interior constans FabricaGenus _genus_configuratio = {
    "configuratio", _fasciculum_sigillare, NIHIL, NIHIL, VERUM
};

/* instrumentum ADHIBETUR, non consumitur: nullus locus (nullus arcus
 * ordinis, nulla '-tacta') - cyclus bootstrap aedilis frangitur (T5) */
interior b32
_nihil_enumerare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                      Piscina* piscina,
                          Xar* loci,
                       chorda* causa_out)
{
    (vacuum)sutura;
    (vacuum)ingressus;
    (vacuum)piscina;
    (vacuum)loci;
    (vacuum)causa_out;
    redde VERUM;
}

interior constans FabricaGenus _genus_instrumentum = {
    "instrumentum", _fasciculum_sigillare, _nihil_enumerare, NIHIL,
    FALSUM
};

/* binarium: octeti plagulae ut ingressus; ut exitus relatione sola
 * iudicatur (LC_UUID, signatura) */
interior constans FabricaGenus _genus_binarium = {
    "binarium", _fasciculum_sigillare, NIHIL, _plagulam_locare, FALSUM
};

interior constans FabricaGenus _genus_directorium = {
    "directorium", _directorium_sigillare, NIHIL, NIHIL, FALSUM
};

interior constans FabricaGenus _genus_manifestum = {
    "manifestum", _manifestum_sigillare, NIHIL, NIHIL, FALSUM
};

interior constans FabricaGenus _genus_plagulae = {
    "plagulae", _plagulas_explicare, _plagulas_enumerare, NIHIL, FALSUM
};

interior constans FabricaGenus _genus_manifesta = {
    "manifesta", _manifesta_sigillare, NIHIL, NIHIL, FALSUM
};

/* radices SIGILLANTUR (obumbratio: caput novum in radice priore), non
 * ENUMERANTUR: ordo ex manifestis (plagulae vere lectae). Arcus ad
 * omnem producentem in radice cyclum faceret ubi actio radicem legit
 * in quam actio posterior scribit (parcum …AR15: fontes_silva
 * sigillat silva/amalgama, amalgama_silva ibi scribit) - ut
 * instrumentum (T5) */
interior constans FabricaGenus _genus_radices = {
    "radices", _radices_sigillare, _nihil_enumerare, NIHIL, FALSUM
};

interior constans FabricaGenus* constans _genera[] = {
    &_genus_fasciculus,
    &_genus_configuratio,
    &_genus_instrumentum,
    &_genus_binarium,
    &_genus_directorium,
    &_genus_manifestum,
    &_genus_plagulae,
    &_genus_manifesta,
    &_genus_radices
};

constans FabricaGenus*
fabrica_genus_invenire (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_genera) / magnitudo(_genera[0]));
         i++)
    {
        si (chorda_aequalis_literis(titulus, _genera[i]->titulus))
        {
            redde _genera[i];
        }
    }
    redde NIHIL;
}

/* Particulae omnium ingressuum actionis (explicatae, non ordinatae):
 * via UNA explicationis pro sigillo ET pro 'tacta' - dissentire
 * nequeunt. */
interior b32
_particulas_colligere (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(actio->ingressus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FabricaIngressus* ingressus;
                     b32  bonum;

        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            i);
        bonum = ingressus->genus->sigillare(sutura, ingressus, exclusa,
            piscina, particulae, causa_out);
        si (!bonum)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
fabrica_ingressus_sigillare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
               constans Xar* exclusa,
                    Piscina* piscina,
                   Sigillum* sigillum_out,
                     chorda* causa_out)
{
                  Xar* particulae;
    SigillumContextus  contextus;
                  i32  i;
                  i32  numerus;
               chorda  prior;

    particulae = xar_creare(piscina, (i32)magnitudo(FabricaParticula));
    si (   particulae == NIHIL
        || !_particulas_colligere(sutura, actio, exclusa, piscina,
               particulae, causa_out))
    {
        redde FALSUM;
    }

    xar_ordinare(particulae, _particulas_comparare);

    sigillum_incipere(&contextus);
    prior.datum    = NIHIL;
    prior.mensura  = ZEPHYRUM;
    numerus        = xar_numerus(particulae);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FabricaParticula* particula;
                Sigillum  octeti;

        particula = (FabricaParticula*)xar_obtinere(particulae, i);
        /* eadem via bis (manifesta duo) semel sigillatur */
        si (i > 0 && chorda_aequalis(prior, particula->via))
        {
            perge;
        }
        prior   = particula->via;
        octeti  = particula->octeti;
        sigillum_addere(&contextus, particula->via.datum,
            (memoriae_index)particula->via.mensura);
        sigillum_addere(&contextus, "", 1);
        sigillum_addere(&contextus, octeti.octeti, SIGILLUM_OCTETI);
    }
    *sigillum_out = sigillum_finire(&contextus);
    redde VERUM;
}

b32
fabrica_actionem_enumerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* loci,
                    chorda* causa_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actio->ingressus); i++)
    {
        FabricaIngressus* ingressus;
                     Xar* particulae;

        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            i);
        si (ingressus->genus->enumerare != NIHIL)
        {
            si (!ingressus->genus->enumerare(sutura, ingressus, piscina,
                    loci, causa_out))
            {
                redde FALSUM;
            }
            perge;
        }
        particulae = xar_creare(piscina,
            (i32)magnitudo(FabricaParticula));
        si (   particulae == NIHIL
            || !ingressus->genus->sigillare(sutura, ingressus, NIHIL,
                   piscina, particulae, causa_out))
        {
            redde FALSUM;
        }
        _loci_ex_particulis(particulae, piscina, loci);
    }
    redde VERUM;
}

/* via -> directorium (sine '/' finali; vacuum in radice) et titulus */
interior vacuum
_viam_dividere (
     chorda  via,
     chorda* directorium_out,
     chorda* titulus_out)
{
    s32 k;

    k = (s32)via.mensura - I;
    dum (k >= 0 && via.datum[k] != (i8)'/')
    {
        k--;
    }
    *directorium_out  = chorda_sectio(via, 0, (k > 0) ? (i32)k : 0);
    *titulus_out      = chorda_sectio(via, (i32)(k + I), via.mensura);
}

/* VERUM si via (cum directorio et titulo suo) in loco cadit */
interior b32
_locus_tangit (
    constans FabricaLocus* locus,
                   chorda  via,
                   chorda  directorium,
                   chorda  titulus,
                  Piscina* piscina)
{
    commutatio (locus->forma)
    {
        casus FABRICA_LOCUS_PLAGULA:
            redde chorda_aequalis(locus->via, via);
        casus FABRICA_LOCUS_PLAGULAE:
            redde chorda_aequalis(locus->via, directorium)
                && _suffixo_congruit(titulus, locus->suffixa);
        casus FABRICA_LOCUS_ARBOR:
            redde chorda_incipit(via, _iungere(piscina, "", locus->via,
                "/"));
        ordinarius:
            redde FALSUM;
    }
}

b32
fabrica_actio_tacta (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
              constans Xar* viae,
                   Piscina* piscina)
{
       Xar* loci;
    chorda  causa;
       i32  i;
       i32  j;

    loci           = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (   loci == NIHIL
        || !fabrica_actionem_enumerare(sutura, actio, piscina, loci,
               &causa))
    {
        redde VERUM;   /* conservativum: iudex IGNOTUM nominabit */
    }
    /* exitus ipsi: verbum locare generis sui */
    per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
    {
        FabricaExitus* exitus;

        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
        si (exitus->genus != NIHIL && exitus->genus->locare != NIHIL)
        {
            (vacuum)exitus->genus->locare(exitus, piscina, loci);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
    {
        chorda via;
        chorda directorium;
        chorda titulus;

        via = *(chorda*)xar_obtinere(viae, i);
        _viam_dividere(via, &directorium, &titulus);
        per (j = ZEPHYRUM; j < xar_numerus(loci); j++)
        {
            si (_locus_tangit((FabricaLocus*)xar_obtinere(loci, j), via,
                    directorium, titulus, piscina))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

chorda
fabrica_provenientia_via (
     chorda  titulus,
    Piscina* piscina)
{
    redde _iungere(piscina, "build/fabrica/provenientia/", titulus,
        ".c");
}

b32
fabrica_actionem_sigillare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out)
{
    Xar* exclusa;

    exclusa = _xar_chordarum(piscina);
    si (exclusa == NIHIL)
    {
        redde FALSUM;
    }
    /* directorium provenientiae TOTUM: involucri exitus, numquam
     * ingressus - actio familiae (canon, natura: binaria plura)
     * plagulam per binarium habet, non per titulum (1b T5, D2) */
    _chordam_addere(exclusa, chorda_ex_literis(
        "build/fabrica/provenientia/", piscina));
    redde fabrica_ingressus_sigillare(sutura, actio, exclusa, piscina,
        sigillum_out, causa_out);
}


/* ==================================================
 * Iudicium
 * ================================================== */

/* numerus linearum quae differunt (positione; cauda longior tota) */
interior i32
_lineas_differentes_numerare (
    chorda a,
    chorda b)
{
    i32 i;
    i32 j;
    i32 numerus;

    i        = ZEPHYRUM;
    j        = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    dum (i < a.mensura || j < b.mensura)
    {
        i32 finis_a;
        i32 finis_b;
        b32 differt;

        finis_a = i;
        dum (finis_a < a.mensura && a.datum[finis_a] != '\n')
        {
            finis_a++;
        }
        finis_b = j;
        dum (finis_b < b.mensura && b.datum[finis_b] != '\n')
        {
            finis_b++;
        }
        differt = (i >= a.mensura || j >= b.mensura)
            || (finis_a - i) != (finis_b - j)
            || memcmp(a.datum + i, b.datum + j, (memoriae_index)(finis_a
                - i))
                != 0;
        si (differt)
        {
            numerus++;
        }
        i = (finis_a < a.mensura) ? finis_a + 1 : finis_a;
        j = (finis_b < b.mensura) ? finis_b + 1 : finis_b;
        si (i >= a.mensura && j >= b.mensura)
        {
            frange;
        }
    }
    redde numerus;
}

interior FabricaIudicium
_iudicium (
           chorda artificium,
    FabricaStatus status,
           chorda causa)
{
    FabricaIudicium iudicium;

    iudicium.artificium  = artificium;
    iudicium.status      = status;
    iudicium.causa       = causa;
    redde iudicium;
}

interior FabricaIudicium
_relationem_iudicare (
    constans FabricaSutura* sutura,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                   Piscina* piscina)
{
       chorda relatio;
       chorda signum;
       chorda hodie;
    character hex[SIGILLUM_HEX_MENSURA];
          s32 inventum;
          i32 index;
          i32 finis;

    si (   sutura->rogare == NIHIL
        || !sutura->rogare(sutura->datum,
               chorda_ut_cstr(exitus->via, piscina), piscina, &relatio))
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM,
            _iungere(piscina, "sine provenientia: ", exitus->via,
                " -provenientia nihil reddit"));
    }
    signum    = chorda_ex_literis("ingressus ", piscina);
    inventum  = chorda_invenire_index(relatio, signum);
    si (inventum < 0)
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM,
            chorda_ex_literis("relatio sine linea ingressus",
                piscina));
    }
    index = (i32)inventum + signum.mensura;
    finis = index;
    dum (finis < relatio.mensura && relatio.datum[finis] != '\n')
    {
        finis++;
    }
    sigillum_hex(ingressus, hex);
    hodie = chorda_ex_literis(hex, piscina);
    si (   (finis - index) == hodie.mensura
        && memcmp(relatio.datum + index, hodie.datum,
               (memoriae_index)hodie.mensura) == 0)
    {
        redde _iudicium(exitus->via, FABRICA_RECENS,
            chorda_ex_literis("relatio congruit", piscina));
    }
    redde _iudicium(exitus->via, FABRICA_STALUM,
        _iungere(piscina, "ingressus mutati post institutionem (hodie ",
            chorda_sectio(hodie, 0, VIII), "...)"));
}

/* sigillum actionis memoratum per cursum (sutura->digesta) */
nomen structura {
         b32 bonum;
    Sigillum sigillum;
      chorda causa;
} DigestumMemoratum;

/* fabrica_actionem_sigillare SEMEL per actionem et cursum: exitus
 * multi (silva XXII) ingressus eosdem non iterum explicant. Defectus
 * quoque memoratur (causa eadem). */
interior b32
_actionem_sigillare_semel (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out)
{
               vacuum* memoratum;
    DigestumMemoratum* locus;
                  b32  bonum;

    si (   sutura->digesta != NIHIL
        && tabula_dispersa_invenire(sutura->digesta, actio->titulus,
               &memoratum))
    {
        locus          = (DigestumMemoratum*)memoratum;
        *sigillum_out  = locus->sigillum;
        *causa_out     = locus->causa;
        redde locus->bonum;
    }
    bonum = fabrica_actionem_sigillare(sutura, actio, piscina,
        sigillum_out, causa_out);
    si (sutura->digesta != NIHIL)
    {
        locus = (DigestumMemoratum*)piscina_allocare(piscina,
            magnitudo(DigestumMemoratum));
        si (locus != NIHIL)
        {
            locus->bonum     = bonum;
            locus->sigillum  = *sigillum_out;
            locus->causa     = *causa_out;
            (vacuum)tabula_dispersa_inserere(sutura->digesta,
                actio->titulus, locus);
        }
    }
    redde bonum;
}

/* Clavis memoriae: sigillum ingressuum ET mandati (verbum NUL ...).
 * Radix nova in mandato declarata ingressus nondum mutat (manifesta
 * eius nondum scripta) - sine mandato verificatio vetus congrueret.
 * Digesta institutionum (relatio) intacta manent. */
interior Sigillum
_clavem_memoriae (
    constans FabricaActio* actio,
        constans Sigillum* ingressus)
{
    SigillumContextus contextus;
                  i32 i;

    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, ingressus->octeti, SIGILLUM_OCTETI);
    per (i = ZEPHYRUM; i < xar_numerus(actio->mandatum); i++)
    {
        chorda verbum;

        verbum = *(chorda*)xar_obtinere(actio->mandatum, i);
        sigillum_addere(&contextus, verbum.datum,
            (memoriae_index)verbum.mensura);
        sigillum_addere(&contextus, "", 1);
    }
    redde sigillum_finire(&contextus);
}


/* ==================================================
 * Vestigia lectionum (plan 2 T2)
 *
 * Actio lectiones="verum": generator cum FABRICA_LECTIONES currit
 * (via a nucleo electa, per suturam data); post regenerationem
 * CONGRUENTEM liber per suturam legitur, quaeque lectio statu suo
 * hodierno sigillatur, vestigium sub clave (titulus, ingressus,
 * artificium) servatur. Iudicium proximum: vestigio congruente RECENS
 * sine regeneratione.
 * ================================================== */

#define LIBRI_DIRECTORIUM "build/fabrica/lectiones/"
/* THESAURUS (plan 2 T3): blobi, actiones, generationes sunt cache per
 * sigilla ingressuum iam lectorum - non ingressus. Radix ipsa
 * (obiecta .o in build/aedilis/obiecta) ingressus verus manet. */
hic_manens constans character* constans _thesauri_partes[III] = {
    "build/aedilis/obiecta/blobi/",
    "build/aedilis/obiecta/actiones/",
    "build/aedilis/obiecta/generationes/"
};

interior b32
_in_thesauro (
      chorda  via,
     Piscina* piscina)
{
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (chorda_incipit(via, chorda_ex_literis(_thesauri_partes[k],
                piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

hic_manens constans character* constans SIGNUM_ABSENS   = "\001absens";
hic_manens constans character* constans SIGNUM_PRAESENS =
    "\001praesens";

/* status viae hodiernus ut sigillum: L contenta; A/X praesentia;
 * D nomina ordinata. FALSUM = genus non verificabile (E). */
interior b32
_lectionem_sigillare (
    constans FabricaSutura* sutura,
               LectioGenus  genus,
                    chorda  via,
                   Piscina* piscina,
                  Sigillum* sigillum_out)
{
     constans character* via_cstr;
                 chorda  contentum;
                    Xar* nomina;
                    b32  legibile;
                    b32  est_directorium;

    via_cstr     = chorda_ut_cstr(via, piscina);
    legibile     = sutura->legere(sutura->datum, via_cstr, piscina,
        &contentum);
    est_directorium  = (sutura->enumerare != NIHIL)
        && sutura->enumerare(sutura->datum, via_cstr, piscina, &nomina);
    commutatio (genus)
    {
        casus LECTIO_LEGIT:
            *sigillum_out = legibile
                ? sigillum_computare(contentum.datum,
                      (memoriae_index)contentum.mensura)
                : sigillum_computare(SIGNUM_ABSENS,
                      strlen(SIGNUM_ABSENS));
            redde VERUM;
        casus LECTIO_ABSENS:
        casus LECTIO_EXSTAT:
            *sigillum_out = (legibile || est_directorium)
                ? sigillum_computare(SIGNUM_PRAESENS,
                      strlen(SIGNUM_PRAESENS))
                : sigillum_computare(SIGNUM_ABSENS,
                      strlen(SIGNUM_ABSENS));
            redde VERUM;
        casus LECTIO_ENUMERAVIT:
            si (!est_directorium)
            {
                *sigillum_out = sigillum_computare(SIGNUM_ABSENS,
                    strlen(SIGNUM_ABSENS));
            }
            alioquin
            {
                SigillumContextus contextus;
                              i32 i;

                sigillum_incipere(&contextus);
                per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
                {
                    chorda titulus;

                    titulus = *(chorda*)xar_obtinere(nomina, i);
                    sigillum_addere(&contextus, titulus.datum,
                        (memoriae_index)titulus.mensura);
                    sigillum_addere(&contextus, "\n", 1);
                }
                *sigillum_out = sigillum_finire(&contextus);
            }
            redde VERUM;
        casus LECTIO_SCRIPSIT:
        casus LECTIO_AMBITUS:
            redde FALSUM;
    }
    redde FALSUM;
}

interior LectioGenus
_genus_litterae (
    character  littera,
          b32* notum_out)
{
    *notum_out = VERUM;
    commutatio (littera)
    {
        casus 'L': redde LECTIO_LEGIT;
        casus 'A': redde LECTIO_ABSENS;
        casus 'X': redde LECTIO_EXSTAT;
        casus 'D': redde LECTIO_ENUMERAVIT;
        casus 'S': redde LECTIO_SCRIPSIT;
        casus 'E': redde LECTIO_AMBITUS;
    }
    *notum_out = FALSUM;
    redde LECTIO_LEGIT;
}

/* librum legere et vestigium facere: S praetermittitur (exitus, non
 * ingressus); radix absoluta et './' demuntur; scriptura iudicis,
 * libri ipsi et viae absolutae extra arborem praetermittuntur;
 * (genus, via) semel. FALSUM = liber absens aut lectio non
 * verificabilis (E) - nullum vestigium (regeneratio semper). */
interior b32
_lectiones_colligere (
    constans FabricaSutura*  sutura,
        constans character*  liber_via,
                    chorda   scriptura_dir,
                   Piscina*  piscina,
                       Xar** lectiones_out)
{
             chorda  liber;
                Xar* lectiones;
     TabulaDispersa* visae;
                i32  i;
                i32  initium;

    si (!sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        redde FALSUM;
    }
    lectiones  = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    visae      = tabula_dispersa_creare_chorda(piscina, 256);
    si (lectiones == NIHIL || visae == NIHIL)
    {
        redde FALSUM;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
               chorda  linea;
               chorda  via;
          LectioGenus  genus;
                  b32  notum;
        FabricaLectio* lectio;

        si (liber.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(liber, initium, i);
        initium  = i + I;
        si (linea.mensura < III || linea.datum[I] != '\t')
        {
            perge;
        }
        genus = _genus_litterae((character)linea.datum[0], &notum);
        si (!notum || genus == LECTIO_SCRIPSIT)
        {
            perge;
        }
        si (genus == LECTIO_AMBITUS)
        {
            redde FALSUM;   /* ambitus a nucleo non verificatur */
        }
        via = chorda_sectio(linea, II, linea.mensura);
        si (   sutura->radix.mensura > 0
            && via.mensura > sutura->radix.mensura
            && chorda_incipit(via, sutura->radix)
            && via.datum[sutura->radix.mensura] == '/')
        {
            via = chorda_sectio(via, sutura->radix.mensura + I,
                via.mensura);
        }
        dum (   via.mensura > II && via.datum[0] == '.'
             && via.datum[I] == '/')
        {
            via = chorda_sectio(via, II, via.mensura);
        }
        si (   via.mensura == 0 || via.datum[0] == '/'
            || chorda_incipit(via, scriptura_dir)
            || chorda_incipit(via, chorda_ex_literis(LIBRI_DIRECTORIUM,
                   piscina))
            || _in_thesauro(via, piscina))
        {
            perge;
        }
        {
            chorda clavis;

            clavis = _iungere(piscina, "", chorda_sectio(linea, 0, I),
                chorda_ut_cstr(via, piscina));
            si (tabula_dispersa_continet(visae, clavis))
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(visae, clavis, NIHIL);
        }
        lectio = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL)
        {
            redde FALSUM;
        }
        lectio->genus  = genus;
        lectio->via    = via;
        si (!_lectionem_sigillare(sutura, genus, via, piscina,
                &lectio->sigillum))
        {
            redde FALSUM;
        }
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* VERUM si omnis lectio vestigii hodie idem sigillum dat */
interior b32
_lectiones_congruunt (
    constans FabricaSutura* sutura,
              constans Xar* lectiones,
                   Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio;
                      Sigillum  hodie;

        lectio = (constans FabricaLectio*)xar_obtinere(lectiones, i);
        si (   !_lectionem_sigillare(sutura, lectio->genus, lectio->via,
                   piscina, &hodie)
            || memcmp(hodie.octeti, lectio->sigillum.octeti,
                   SIGILLUM_OCTETI) != 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* auditus memoriae: ictus hic regenerandus? (sub -plenus solum;
 * specimen determinatum per clavem - nullus numerator servandus) */
interior b32
_auditum_eligere (
    constans FabricaSutura* sutura,
                       b32  plenus,
         constans Sigillum* clavis)
{
    si (!plenus || sutura->auditus == 0 || sutura->currere == NIHIL)
    {
        redde FALSUM;
    }
    si (sutura->auditus == I)
    {
        redde VERUM;
    }
    redde (clavis->octeti[0] % sutura->auditus) == 0;
}

/* Generatorem actionis currere SEMEL per cursum (exitus multi,
 * generator unus): exitus cursus - felix aut causa fracti - in
 * sutura->regenerationes memoratur; exitus sequentes scripturam
 * eandem legunt. */
interior b32
_regenerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  scriptura_dir,
        constans character* liber_via,
                   Piscina* piscina,
                    chorda* causa_out)
{
    vacuum* memoratum;
    chorda* locus;
       b32  felix;
       i32  duratio;

    si (   sutura->regenerationes != NIHIL
        && tabula_dispersa_invenire(sutura->regenerationes,
               actio->titulus, &memoratum))
    {
        *causa_out = *(chorda*)memoratum;
        redde causa_out->mensura == 0;
    }
    causa_out->datum    = NIHIL;
    causa_out->mensura  = ZEPHYRUM;
    duratio             = ZEPHYRUM;
    felix = sutura->currere(sutura->datum, actio->mandatum,
        chorda_ut_cstr(scriptura_dir, piscina), liber_via, piscina,
        causa_out, &duratio);
    si (felix)
    {
        causa_out->datum    = NIHIL;
        causa_out->mensura  = ZEPHYRUM;
    }
    alioquin si (causa_out->mensura == 0)
    {
        /* fractus sine causa: memoria 'mensura 0 = felix' falleret */
        *causa_out = chorda_ex_literis("causa ignota", piscina);
    }
    si (sutura->regenerationes != NIHIL)
    {
        locus = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (locus != NIHIL)
        {
            *locus = *causa_out;
            (vacuum)tabula_dispersa_inserere(sutura->regenerationes,
                actio->titulus, locus);
        }
    }
    /* cursus iudicis (parcum …AR15): omnis regeneratio VERA (non
     * memorata) metitur - ubi tempus iudicii consumitur */
    si (sutura->cursum_inscribere != NIHIL)
    {
        FabricaSanatio iudicium;

        iudicium.actio         = actio;
        iudicium.eventus       = FABRICA_IUDICIUM;
        iudicium.causa         = *causa_out;
        iudicium.duratio_ms    = duratio;
        iudicium.tempus_notum  = VERUM;
        sutura->cursum_inscribere(sutura->datum, &iudicium);
    }
    redde felix;
}


/* ==================================================
 * Strategiae: verbum iudicare
 * ================================================== */

interior FabricaIudicium
_relatione_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
    (vacuum)actio;
    (vacuum)plenus;
    redde _relationem_iudicare(sutura, exitus, ingressus, piscina);
}

/* REGENERATIO, memoria ante eam (actionibus memorabilibus solis) */
interior FabricaIudicium
_regeneratione_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
              Sigillum  clavis;
              Sigillum  artificium;
                chorda  causa;
                chorda  commissum;
                chorda  effusio;
                chorda  scriptura_dir;
                chorda  scriptura_via;
                chorda  generator;
             character  numerus[32];
                   Xar* vestigium;
    constans character* liber_via;
                   b32  ex_memoria;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), piscina, &commissum))
    {
        redde _iudicium(exitus->via, FABRICA_STALUM,
            chorda_ex_literis("artificium absens", piscina));
    }
    artificium = sigillum_computare(commissum.datum,
        (memoriae_index)commissum.mensura);
    clavis      = _clavem_memoriae(actio, ingressus);
    ex_memoria  = FALSUM;
    /* VESTIGIUM LECTIONUM (plan 2 T2): actio lectiones="verum" quam
     * cursus congruens sub eadem clave ingressuum et eodem artificio
     * legit - si omnis lectio hodie idem sigillum dat, RECENS sine
     * regeneratione (etiam celer). */
    si (   actio->lectiones
        && sutura->lectiones_legere != NIHIL
        && sutura->lectiones_legere(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina),
               chorda_ut_cstr(exitus->via, piscina), &clavis,
               &artificium, piscina, &vestigium)
        && _lectiones_congruunt(sutura, vestigium, piscina))
    {
        si (!_auditum_eligere(sutura, plenus, &clavis))
        {
            redde _iudicium(exitus->via, FABRICA_RECENS,
                chorda_ex_literis("lectiones congruunt (vestigium)",
                    piscina));
        }
        ex_memoria = VERUM;   /* auditus: regeneratur tamen */
    }
    /* memoria SOLUM pro actione memorabili: ingressus aliter non
     * probabiliter pleni, et verificatio vetus mutationem ingressus
     * non declarati celaret */
    si (   !ex_memoria
        && actio->memorabilis
        && sutura->meminisse != NIHIL
        && sutura->meminisse(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina), &clavis,
               &artificium))
    {
        si (!_auditum_eligere(sutura, plenus, &clavis))
        {
            redde _iudicium(exitus->via, FABRICA_RECENS,
                chorda_ex_literis("memoria", piscina));
        }
        ex_memoria = VERUM;
    }
    /* celer: regeneratio omissa nisi actio 'celer' (vilis) sit */
    si ((!plenus && !actio->celer) || sutura->currere == NIHIL)
    {
        redde _iudicium(exitus->via, FABRICA_NON_IUDICATUM,
            chorda_ex_literis("celer: regeneratio omissa", piscina));
    }

    generator = (xar_numerus(actio->mandatum) > 0)
        ? *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM)
        : actio->titulus;
    scriptura_dir = _iungere(piscina, "build/fabrica/scriptura/",
        actio->titulus, "");
    liber_via = actio->lectiones
        ? chorda_ut_cstr(_iungere(piscina, LIBRI_DIRECTORIUM,
              actio->titulus, ".tsv"), piscina)
        : NIHIL;
    si (!_regenerare(sutura, actio, scriptura_dir, liber_via, piscina,
            &causa))
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM,
            _iungere(piscina, "generator fractus: ", generator,
                chorda_ut_cstr(_iungere(piscina, ": ", causa, ""),
                    piscina)));
    }
    scriptura_via = _iungere(piscina, "",
        _iungere(piscina, "", scriptura_dir, "/"),
        chorda_ut_cstr(exitus->scriptura, piscina));
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(scriptura_via, piscina), piscina,
            &effusio))
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM,
            _iungere(piscina, "generator nihil scripsit: ",
                scriptura_via, ""));
    }
    si (chorda_aequalis(effusio, commissum))
    {
        si (   actio->lectiones && liber_via != NIHIL
            && sutura->lectiones_scribere != NIHIL
            && _lectiones_colligere(sutura, liber_via, scriptura_dir,
                   piscina, &vestigium))
        {
            sutura->lectiones_scribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina),
                chorda_ut_cstr(exitus->via, piscina), &clavis,
                &artificium, vestigium);
        }
        si (actio->memorabilis && sutura->inscribere != NIHIL)
        {
            sutura->inscribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina), &clavis,
                &artificium);
        }
        redde _iudicium(exitus->via, FABRICA_RECENS, chorda_ex_literis(
            ex_memoria ? "auditus: regeneratio congruit"
                       : "regeneratio congruit", piscina));
    }
    sprintf(numerus, "%u",
        (insignatus integer)_lineas_differentes_numerare(commissum,
            effusio));
    si (ex_memoria)
    {
        /* memoria aut vestigium RECENS dicebat: foramen ingressus -
         * nominatur et in cursu notatur */
        si (sutura->cursum_inscribere != NIHIL)
        {
            FabricaSanatio auditum;

            auditum.actio    = actio;
            auditum.eventus  = FABRICA_AUDITUM_DISCORS;
            auditum.causa         = chorda_ex_literis(
                "memoria RECENS, regeneratio differt", piscina);
            auditum.duratio_ms    = ZEPHYRUM;
            auditum.tempus_notum  = FALSUM;
            sutura->cursum_inscribere(sutura->datum, &auditum);
        }
        redde _iudicium(exitus->via, FABRICA_STALUM, _iungere(piscina,
            "AUDITUM DISCORS: memoria/vestigium RECENS dicebat,"
            " regeneratio differt (lineae differentes: ",
            chorda_ex_literis(numerus, piscina), ")"));
    }
    redde _iudicium(exitus->via, FABRICA_STALUM,
        _iungere(piscina, "regeneratio differt (lineae differentes: ",
            chorda_ex_literis(numerus, piscina), ")"));
}

/* IGNOTA: numquam iudicatur (machina eam non vocat; vocata tamen
 * IGNOTUM honestum reddit) - praecondicio sola (spec 1b par. II.3) */
interior FabricaIudicium
_ignote_iudicare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
    constans FabricaExitus* exitus,
         constans Sigillum* ingressus,
                       b32  plenus,
                   Piscina* piscina)
{
    (vacuum)sutura;
    (vacuum)actio;
    (vacuum)ingressus;
    (vacuum)plenus;
    redde _iudicium(exitus->via, FABRICA_IGNOTUM, chorda_ex_literis(
        "ignota: praecondicio, numquam iudicatur", piscina));
}

/* ordo membrorum: titulus, genus_ordinarium, octetis_comparat,
 * iudicatur, iudicare */
interior constans FabricaStrategia _strategia_regeneratio = {
    "regeneratio", "fasciculus", VERUM, VERUM, _regeneratione_iudicare
};

interior constans FabricaStrategia _strategia_relatio = {
    "relatio", "binarium", FALSUM, VERUM, _relatione_iudicare
};

interior constans FabricaStrategia _strategia_ignota = {
    "ignota", "fasciculus", FALSUM, FALSUM, _ignote_iudicare
};

interior constans FabricaStrategia* constans _strategiae[] = {
    &_strategia_regeneratio,
    &_strategia_relatio,
    &_strategia_ignota
};

constans FabricaStrategia*
fabrica_strategia_invenire (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_strategiae) / magnitudo(_strategiae[0]));
         i++)
    {
        si (chorda_aequalis_literis(titulus, _strategiae[i]->titulus))
        {
            redde _strategiae[i];
        }
    }
    redde NIHIL;
}

FabricaIudicium
fabrica_iudicare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
     constans FabricaExitus* exitus,
                        b32  plenus,
                    Piscina* piscina)
{
    Sigillum ingressus;
      chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (!_actionem_sigillare_semel(sutura, actio, piscina, &ingressus,
            &causa))
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM, causa);
    }
    redde exitus->strategia->iudicare(sutura, actio, exitus, &ingressus,
        plenus, piscina);
}


/* ==================================================
 * Declarationes
 * ================================================== */

/* a + b + c + d (b, d litterae) */
interior chorda
_quattuor (
                Piscina* piscina,
                 chorda  a,
     constans character* b,
                 chorda  c,
     constans character* d)
{
    ChordaAedificator* aedificator;

    aedificator = chorda_aedificator_creare(piscina, 128);
    si (aedificator == NIHIL)
    {
        redde a;
    }
    (vacuum)chorda_aedificator_appendere_chorda(aedificator, a);
    (vacuum)chorda_aedificator_appendere_literis(aedificator, b);
    (vacuum)chorda_aedificator_appendere_chorda(aedificator, c);
    (vacuum)chorda_aedificator_appendere_literis(aedificator, d);
    redde chorda_aedificator_finire(aedificator);
}

interior chorda
_sedes (
               Piscina* piscina,
    constans character* via,
             StmlNodus* nodus)
{
    character linea[32];

    sprintf(linea, ":%u", (insignatus integer)nodus->linea);
    redde _iungere(piscina, via, chorda_ex_literis(linea, piscina), "");
}

interior b32
_genus_actionis (
                   chorda  valor,
     FabricaGenusActionis* genus)
{
    si (chorda_aequalis_literis(valor, "generator"))
    {
        *genus = FABRICA_ACTIO_GENERATOR;
    }
    alioquin si (chorda_aequalis_literis(valor, "formatio"))
    {
        *genus = FABRICA_ACTIO_FORMATIO;
    }
    alioquin si (chorda_aequalis_literis(valor, "institutio"))
    {
        *genus = FABRICA_ACTIO_INSTITUTIO;
    }
    alioquin
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* recusatio: causa = "sedes: nuntius 'valor'" */
interior Xar*
_recusare (
                Piscina* piscina,
                 chorda* causa_out,
                 chorda  sedes,
     constans character* nuntius,
                 chorda  valor)
{
    si (causa_out != NIHIL)
    {
        *causa_out = _quattuor(piscina, sedes,
            chorda_ut_cstr(_iungere(piscina, ": ",
                chorda_ex_literis(nuntius, piscina), " '"), piscina),
            valor, "'");
    }
    redde NIHIL;
}

interior Xar*
_mandatum_legere (
    StmlNodus* actio_nodus,
      Piscina* piscina)
{
    StmlNodus* mandatum;
          Xar* verba;
          Xar* nodi;
          i32  i;

    verba     = _xar_chordarum(piscina);
    mandatum  = stml_invenire_liberum(actio_nodus, "mandatum");
    si (mandatum == NIHIL || verba == NIHIL)
    {
        redde verba;
    }
    nodi = stml_invenire_omnes_liberos(mandatum, "verbum", piscina);
    per (i = ZEPHYRUM; nodi != NIHIL && i < xar_numerus(nodi); i++)
    {
        _chordam_addere(verba, stml_textus_valor(
            *(StmlNodus**)xar_obtinere(nodi, i), piscina));
    }
    redde verba;
}

/* attributi valor ut litterae C (NIHIL si absens) */
interior constans character*
_attributum_cstr (
             StmlNodus* nodus,
    constans character* titulus,
               Piscina* piscina)
{
    chorda* valor;

    valor = stml_attributum_capere(nodus, titulus);
    redde (valor != NIHIL) ? chorda_ut_cstr(*valor, piscina) : NIHIL;
}

/* declaratne templum loculum 'titulus' (attributum titulus="@titulus")? */
interior b32
_loculum_declarat (
              StmlNodus* templum,
     constans character* titulus,
                Piscina* piscina)
{
    constans character* valor;

    valor = _attributum_cstr(templum, titulus, piscina);
    redde valor != NIHIL && valor[0] == '@'
        && strcmp(valor + I, titulus) == ZEPHYRUM;
}

/* FAMILIA explicare: nodi actionum instantiarum in 'nodi' appenduntur
 * (vide fabrica_declarationes_legere_cum_sutura) */
interior b32
_familiam_explicare (
                  StmlNodus* familia,
         constans character* via,
     constans FabricaSutura* sutura,
                    Piscina* piscina,
        InternamentumChorda* intern,
                        Xar* nodi,
                     chorda* causa_out)
{
        constans character* titulus;
        constans character* directorium;
        constans character* praefixum;
        constans character* suffixum;
                    chorda  sedes;
                    chorda  nihil;
                 StmlNodus* templum;
                 StmlNodus* radix;
                       Xar* nomina;
                       Xar* fontes;
      StmlExpansioResultus  expansio;
                       b32  basis_declarata;
                       b32  fons_declaratus;
                       i32  i;
                       i32  instantiae;

    nihil        = chorda_ex_literis("", piscina);
    sedes        = _sedes(piscina, via, familia);
    titulus      = _attributum_cstr(familia, "titulus", piscina);
    directorium  = _attributum_cstr(familia, "via", piscina);
    praefixum    = _attributum_cstr(familia, "praefixum", piscina);
    suffixum     = _attributum_cstr(familia, "suffixum", piscina);
    praefixum    = (praefixum != NIHIL) ? praefixum : "";
    suffixum     = (suffixum != NIHIL) ? suffixum : "";
    si (titulus == NIHIL || directorium == NIHIL)
    {
        _recusare(piscina, causa_out, sedes,
            "familia sine titulo aut via", nihil);
        redde FALSUM;
    }
    si (sutura == NIHIL || sutura->enumerare == NIHIL)
    {
        _recusare(piscina, causa_out, sedes,
            "familia sine enumeratione (lector purus) - "
            "fabrica_declarationes_legere_cum_sutura",
            chorda_ex_literis(titulus, piscina));
        redde FALSUM;
    }
    /* templum unum: elementum fragmentum cuius id '@' incipit */
    templum = NIHIL;
    per (i = ZEPHYRUM; familia->liberi != NIHIL
         && i < xar_numerus(familia->liberi); i++)
    {
        StmlNodus* liberum;

        liberum = *(StmlNodus**)xar_obtinere(familia->liberi, i);
        si (liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (   templum != NIHIL || !liberum->fragmentum
            || liberum->fragmentum_id == NIHIL
            || liberum->fragmentum_id->mensura < II
            || liberum->fragmentum_id->datum[ZEPHYRUM] != (i8)'@')
        {
            _recusare(piscina, causa_out, sedes,
                "familia templum UNUM poscit (<#@id ...> corpus </#>)",
                chorda_ex_literis(titulus, piscina));
            redde FALSUM;
        }
        templum = liberum;
    }
    si (templum == NIHIL)
    {
        _recusare(piscina, causa_out, sedes,
            "familia sine templo (<#@id ...> corpus </#>)",
            chorda_ex_literis(titulus, piscina));
        redde FALSUM;
    }
    si (!sutura->enumerare(sutura->datum, directorium, piscina,
        &nomina))
    {
        _recusare(piscina, causa_out, sedes,
            "familia: directorium absens",
            chorda_ex_literis(directorium, piscina));
        redde FALSUM;
    }
    basis_declarata = _loculum_declarat(templum, "basis", piscina);
    fons_declaratus = _loculum_declarat(templum, "fons", piscina);
    /* arbor synthetica: templum (copia) deinde vocationes */
    radix   = stml_elementum_creare(piscina, intern, "aedificatio");
    fontes  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   radix == NIHIL || fontes == NIHIL
        || !stml_liberum_addere(radix,
            stml_duplicare(templum, piscina, intern)))
    {
        redde FALSUM;
    }
    instantiae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
                          chorda  nomen_plagulae;
                          chorda  basis;
                          chorda  fons;
               ChordaAedificator* vocatio;
                  memoriae_index  lp;
                  memoriae_index  ls;

        nomen_plagulae  = *(chorda*)xar_obtinere(nomina, i);
        lp              = strlen(praefixum);
        ls              = strlen(suffixum);
        si (   (memoriae_index)nomen_plagulae.mensura      <= lp + ls
            || memcmp(nomen_plagulae.datum, praefixum, lp) != ZEPHYRUM
            || memcmp(nomen_plagulae.datum + nomen_plagulae.mensura
                - ls,
                suffixum, ls) != ZEPHYRUM)
        {
            perge;
        }
        si (   chorda_continet(nomen_plagulae,
                chorda_ex_literis("\"", piscina))
            || chorda_continet(nomen_plagulae,
                chorda_ex_literis("&", piscina)))
        {
            _recusare(piscina, causa_out, sedes,
                "familia: nomen plagulae cum '\"' aut '&'",
                nomen_plagulae);
            redde FALSUM;
        }
        basis = chorda_sectio(nomen_plagulae, ZEPHYRUM,
            nomen_plagulae.mensura - (i32)ls);
        fons  = _iungere(piscina, directorium, chorda_ex_literis("/",
            piscina), "");
        fons  = _iungere(piscina, chorda_ut_cstr(fons, piscina),
            nomen_plagulae, "");
        vocatio = chorda_aedificator_creare(piscina, CCLVI);
        (vacuum)chorda_aedificator_appendere_character(vocatio, '#');
        (vacuum)chorda_aedificator_appendere_chorda(vocatio,
            *templum->fragmentum_id);
        si (basis_declarata)
        {
            (vacuum)chorda_aedificator_appendere_literis(vocatio,
                " basis=\"");
            (vacuum)chorda_aedificator_appendere_chorda(vocatio, basis);
            (vacuum)chorda_aedificator_appendere_character(vocatio,
                '"');
        }
        si (fons_declaratus)
        {
            (vacuum)chorda_aedificator_appendere_literis(vocatio,
                " fons=\"");
            (vacuum)chorda_aedificator_appendere_chorda(vocatio, fons);
            (vacuum)chorda_aedificator_appendere_character(vocatio,
                '"');
        }
        si (!stml_liberum_addere(radix, stml_transclusionem_creare(
                piscina, intern, chorda_aedificator_finire(vocatio))))
        {
            redde FALSUM;
        }
        *(chorda*)xar_addere(fontes) = basis;
        *(chorda*)xar_addere(fontes) = fons;
        instantiae++;
    }
    expansio = stml_expandere(radix, piscina, intern);
    si (!expansio.successus || expansio.radix_expansa == NIHIL)
    {
        _recusare(piscina, causa_out, sedes,
            "familia: expansio templi fracta (vitium, loculus)",
            _iungere(piscina, "", expansio.loculus.mensura > ZEPHYRUM
                ? expansio.loculus : expansio.fragmentum, ""));
        redde FALSUM;
    }
    /* instantiae: actio una per vocationem, ordine */
    {
        i32 k;

        k = ZEPHYRUM;
        per (i = ZEPHYRUM; expansio.radix_expansa->liberi != NIHIL
             && i < xar_numerus(expansio.radix_expansa->liberi); i++)
        {
            StmlNodus* actio;
            StmlNodus* ingressus;
               chorda  basis;
               chorda  fons;

            actio = *(StmlNodus**)xar_obtinere(
                expansio.radix_expansa->liberi, i);
            si (actio->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            si (   k >= instantiae || actio->titulus == NIHIL
                || !chorda_aequalis_literis(*actio->titulus, "actio"))
            {
                _recusare(piscina, causa_out, sedes,
                    "familia: corpus templi actionem UNAM poscit",
                    chorda_ex_literis(titulus, piscina));
                redde FALSUM;
            }
            si (stml_attributum_capere(actio, "titulus") != NIHIL)
            {
                _recusare(piscina, causa_out, sedes,
                    "familia: titulus in templo (fabrica "
                    "'familia:basis' ponit)",
                    chorda_ex_literis(titulus, piscina));
                redde FALSUM;
            }
            basis  = *(chorda*)xar_obtinere(fontes, k * II);
            fons   = *(chorda*)xar_obtinere(fontes, k * II + I);
            (vacuum)stml_attributum_addere_chorda(actio, piscina,
                intern,
                "titulus", _iungere(piscina, titulus,
                    chorda_ex_literis(":", piscina),
                    chorda_ut_cstr(basis, piscina)));
            ingressus = stml_elementum_creare(piscina, intern,
                "ingressus");
            si (ingressus == NIHIL)
            {
                redde FALSUM;
            }
            (vacuum)stml_attributum_addere(ingressus, piscina, intern,
                "genus", "fasciculus");
            (vacuum)stml_attributum_addere_chorda(ingressus, piscina,
                intern, "via", fons);
            (vacuum)stml_liberum_addere(actio, ingressus);
            si (actio->linea == ZEPHYRUM)
            {
                actio->linea = familia->linea;
            }
            *(StmlNodus**)xar_addere(nodi) = actio;
            k++;
        }
        si (k != instantiae)
        {
            _recusare(piscina, causa_out, sedes,
                "familia: corpus templi actionem UNAM poscit",
                chorda_ex_literis(titulus, piscina));
            redde FALSUM;
        }
    }
    redde VERUM;
}

Xar*
fabrica_declarationes_legere (
                 chorda  contentum,
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out)
{
    redde fabrica_declarationes_legere_cum_sutura(contentum, via, NIHIL,
        piscina, intern, causa_out);
}

Xar*
fabrica_declarationes_legere_cum_sutura (
                    chorda  contentum,
        constans character* via,
    constans FabricaSutura* sutura,
                   Piscina* piscina,
       InternamentumChorda* intern,
                    chorda* causa_out)
{
    StmlResultus  lectum;
             Xar* actiones;
             Xar* nodi;
          chorda  nihil;
             i32  i;

    nihil   = chorda_ex_literis("", piscina);
    lectum  = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "aedificatio"))
    {
        redde _recusare(piscina, causa_out,
            chorda_ex_literis(via, piscina),
            "radix non aedificatio (aut STML fractum)", nihil);
    }
    actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
    nodi = stml_invenire_omnes_liberos(lectum.elementum_radix, "actio",
        piscina);
    /* FAMILIAE (plan 2 T4): instantiae post actiones scriptas */
    {
        Xar* familiae;
        i32  f;

        familiae = stml_invenire_omnes_liberos(lectum.elementum_radix,
            "familia", piscina);
        si (   familiae != NIHIL && xar_numerus(familiae) > ZEPHYRUM
            && nodi     == NIHIL)
        {
            nodi = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        }
        per (f = ZEPHYRUM; familiae != NIHIL
             && f < xar_numerus(familiae); f++)
        {
            si (!_familiam_explicare(
                    *(StmlNodus**)xar_obtinere(familiae, f), via,
                    sutura, piscina, intern, nodi, causa_out))
            {
                redde NIHIL;
            }
        }
    }
    per (i = ZEPHYRUM; nodi != NIHIL && i < xar_numerus(nodi); i++)
    {
           StmlNodus* nodus;
        FabricaActio  actio;
              chorda* valor;
                 Xar* filii;
                 i32  j;

        nodus        = *(StmlNodus**)xar_obtinere(nodi, i);
        actio.sedes  = _sedes(piscina, via, nodus);

        valor = stml_attributum_capere(nodus, "titulus");
        si (valor == NIHIL)
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "actio sine titulo", nihil);
        }
        actio.titulus = *valor;
        per (j = ZEPHYRUM; j < xar_numerus(actiones); j++)
        {
            si (chorda_aequalis(actio.titulus,
                    ((FabricaActio*)xar_obtinere(actiones,
                    j))->titulus))
            {
                redde _recusare(piscina, causa_out, actio.sedes,
                    "titulus duplex", actio.titulus);
            }
        }

        valor = stml_attributum_capere(nodus, "genus");
        si (valor == NIHIL || !_genus_actionis(*valor, &actio.genus))
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "genus actionis ignotum",
                valor != NIHIL ? *valor : nihil);
        }
        valor = stml_attributum_capere(nodus, "celer");
        si (valor == NIHIL || chorda_aequalis_literis(*valor, "falsum"))
        {
            actio.celer = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(*valor, "verum"))
        {
            actio.celer = VERUM;
        }
        alioquin
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "celer nec verum nec falsum", *valor);
        }
        valor = stml_attributum_capere(nodus, "lectiones");
        si (valor == NIHIL || chorda_aequalis_literis(*valor, "falsum"))
        {
            actio.lectiones = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(*valor, "verum"))
        {
            actio.lectiones = VERUM;
        }
        alioquin
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "lectiones nec verum nec falsum", *valor);
        }
        valor = stml_attributum_capere(nodus, "memorabilis");
        si (valor == NIHIL || chorda_aequalis_literis(*valor, "falsum"))
        {
            actio.memorabilis = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(*valor, "verum"))
        {
            actio.memorabilis = VERUM;
        }
        alioquin
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "memorabilis nec verum nec falsum", *valor);
        }
        actio.mandatum = _mandatum_legere(nodus, piscina);

        actio.praecondiciones  = _xar_chordarum(piscina);
        actio.dependentiae     = NIHIL;
        filii = stml_invenire_omnes_liberos(nodus, "praecondicio",
            piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
            StmlNodus* filius;
               chorda* nominata;

            filius    = *(StmlNodus**)xar_obtinere(filii, j);
            nominata  = stml_attributum_capere(filius, "actio");
            si (nominata == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "praecondicio sine actione", nihil);
            }
            _chordam_addere(actio.praecondiciones, *nominata);
        }

        /* vestigia (T4): opera propria aut communia (communis=verum) */
        actio.vestigia = xar_creare(piscina,
            (i32)magnitudo(FabricaLocus));
        actio.communia = xar_creare(piscina,
            (i32)magnitudo(FabricaLocus));
        filii = stml_invenire_omnes_liberos(nodus, "vestigium",
            piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
                StmlNodus* filius;
                   chorda* locus_via;
                   chorda* forma;
                   chorda* suffixa;
                   chorda* communis;
         FabricaFormaLoci  forma_loci;
                      Xar* destinatio;

            filius     = *(StmlNodus**)xar_obtinere(filii, j);
            locus_via  = stml_attributum_capere(filius, "via");
            forma      = stml_attributum_capere(filius, "forma");
            suffixa    = stml_attributum_capere(filius, "suffixa");
            communis   = stml_attributum_capere(filius, "communis");
            si (locus_via == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), "vestigium sine via",
                    nihil);
            }
            si (   forma == NIHIL
                || chorda_aequalis_literis(*forma, "arbor"))
            {
                forma_loci = FABRICA_LOCUS_ARBOR;
            }
            alioquin si (chorda_aequalis_literis(*forma, "plagulae"))
            {
                forma_loci = FABRICA_LOCUS_PLAGULAE;
            }
            alioquin si (chorda_aequalis_literis(*forma, "plagula"))
            {
                forma_loci = FABRICA_LOCUS_PLAGULA;
            }
            alioquin
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "forma vestigii ignota",
                    *forma);
            }
            si (   communis == NIHIL
                || chorda_aequalis_literis(*communis, "falsum"))
            {
                destinatio = actio.vestigia;
            }
            alioquin si (chorda_aequalis_literis(*communis, "verum"))
            {
                destinatio = actio.communia;
            }
            alioquin
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "communis nec verum nec falsum", *communis);
            }
            _locum_addere(destinatio, forma_loci, *locus_via,
                suffixa != NIHIL ? *suffixa : nihil);
        }

        actio.ingressus = xar_creare(piscina,
            (i32)magnitudo(FabricaIngressus));
        filii = stml_invenire_omnes_liberos(nodus, "ingressus",
            piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
                   StmlNodus* filius;
            FabricaIngressus* ingressus;
                      chorda* genus;
                      chorda* ingressus_via;

            filius = *(StmlNodus**)xar_obtinere(filii, j);
            genus = stml_attributum_capere(filius, "genus");
            ingressus_via = stml_attributum_capere(filius, "via");
            ingressus = (FabricaIngressus*)xar_addere(actio.ingressus);
            ingressus->genus = (genus != NIHIL)
                ? fabrica_genus_invenire(*genus) : NIHIL;
            si (ingressus->genus == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "genus ingressus ignotum",
                    genus != NIHIL ? *genus : nihil);
            }
            si (ingressus_via == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), "ingressus sine via",
                    nihil);
            }
            ingressus->via = *ingressus_via;
            {
                chorda* suffixa;

                suffixa = stml_attributum_capere(filius, "suffixa");
                ingressus->suffixa = (suffixa != NIHIL)
                    ? *suffixa : chorda_ex_literis("", piscina);
            }
        }
        si (xar_numerus(actio.ingressus) == 0)
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "actio sine ingressu", actio.titulus);
        }

        actio.exitus = xar_creare(piscina,
            (i32)magnitudo(FabricaExitus));
        filii = stml_invenire_omnes_liberos(nodus, "exitus", piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
                StmlNodus* filius;
            FabricaExitus* exitus;
                   chorda* exitus_via;
                   chorda* provenientia;
                   chorda* scriptura;
                   chorda* genus_exitus;

            filius      = *(StmlNodus**)xar_obtinere(filii, j);
            exitus_via  = stml_attributum_capere(filius, "via");
            provenientia = stml_attributum_capere(filius,
                "provenientia");
            scriptura     = stml_attributum_capere(filius, "scriptura");
            genus_exitus  = stml_attributum_capere(filius, "genus");
            exitus        = (FabricaExitus*)xar_addere(actio.exitus);
            si (exitus_via == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), "exitus sine via",
                    nihil);
            }
            exitus->strategia = (provenientia != NIHIL)
                ? fabrica_strategia_invenire(*provenientia) : NIHIL;
            si (exitus->strategia == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "provenientia ignota",
                    provenientia != NIHIL ? *provenientia : nihil);
            }
            /* genus absens: genus ordinarium strategiae */
            exitus->genus = fabrica_genus_invenire(chorda_ex_literis(
                exitus->strategia->genus_ordinarium, piscina));
            si (genus_exitus != NIHIL)
            {
                exitus->genus = fabrica_genus_invenire(*genus_exitus);
                si (exitus->genus == NIHIL)
                {
                    redde _recusare(piscina, causa_out,
                        _sedes(piscina, via, filius),
                        "genus exitus ignotum", *genus_exitus);
                }
            }
            si (exitus->genus->locare == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "genus ingressus solum, exitus esse nequit",
                    chorda_ex_literis(exitus->genus->titulus, piscina));
            }
            /* typus strategias validas constringit (binarium non
             * reproducibile: numquam regeneratio - 1a T6) */
            si (   exitus->strategia->octetis_comparat
                && !exitus->genus->reproducibile)
            {
                character nuntius[128];

                sprintf(nuntius, "strategia %s generi %s non licet",
                    exitus->strategia->titulus, exitus->genus->titulus);
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), nuntius, *exitus_via);
            }
            exitus->via = *exitus_via;
            exitus->scriptura = (scriptura != NIHIL)
                ? *scriptura : *exitus_via;
        }
        si (xar_numerus(actio.exitus) == 0)
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "actio sine exitu", actio.titulus);
        }
        *(FabricaActio*)xar_addere(actiones) = actio;
    }
    redde actiones;
}

Xar*
fabrica_subsystemata_legere (
                 chorda  contentum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out)
{
    StmlResultus  lectum;
             Xar* viae;
             Xar* nodi;
             i32  i;

    lectum = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "fabrica"))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_ex_literis(
                "fabrica.stml: radix non fabrica (aut STML fractum)",
                piscina);
        }
        redde NIHIL;
    }
    viae = _xar_chordarum(piscina);
    nodi = stml_invenire_omnes_liberos(lectum.elementum_radix,
        "subsystema", piscina);
    per (i = ZEPHYRUM; nodi != NIHIL && i < xar_numerus(nodi); i++)
    {
        StmlNodus* nodus;
           chorda* via;

        nodus  = *(StmlNodus**)xar_obtinere(nodi, i);
        via    = stml_attributum_capere(nodus, "via");
        si (via == NIHIL)
        {
            redde _recusare(piscina, causa_out,
                _sedes(piscina, "fabrica.stml", nodus),
                "subsystema sine via", chorda_ex_literis("", piscina));
        }
        _chordam_addere(viae, *via);
    }
    redde viae;
}


/* ==================================================
 * Composita et praecondiciones (plan 1b T2)
 * ================================================== */

Xar*
fabrica_composita_legere (
                 chorda  contentum,
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out)
{
    StmlResultus  lectum;
             Xar* composita;
             Xar* nodi;
          chorda  nihil;
             i32  i;

    nihil   = chorda_ex_literis("", piscina);
    lectum  = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "aedificatio"))
    {
        redde _recusare(piscina, causa_out,
            chorda_ex_literis(via, piscina),
            "radix non aedificatio (aut STML fractum)", nihil);
    }
    composita = xar_creare(piscina, (i32)magnitudo(FabricaCompositum));
    nodi = stml_invenire_omnes_liberos(lectum.elementum_radix,
        "compositum", piscina);
    per (i = ZEPHYRUM; nodi != NIHIL && i < xar_numerus(nodi); i++)
    {
                StmlNodus* nodus;
        FabricaCompositum  compositum;
                   chorda* valor;
                      Xar* filii;
                      i32  j;

        nodus             = *(StmlNodus**)xar_obtinere(nodi, i);
        compositum.sedes  = _sedes(piscina, via, nodus);
        valor             = stml_attributum_capere(nodus, "titulus");
        si (valor == NIHIL)
        {
            redde _recusare(piscina, causa_out, compositum.sedes,
                "compositum sine titulo", nihil);
        }
        compositum.titulus = *valor;
        per (j = ZEPHYRUM; j < xar_numerus(composita); j++)
        {
            si (chorda_aequalis(compositum.titulus,
                    ((FabricaCompositum*)xar_obtinere(composita,
                    j))->titulus))
            {
                redde _recusare(piscina, causa_out, compositum.sedes,
                    "titulus compositi duplex", compositum.titulus);
            }
        }
        compositum.partes = xar_creare(piscina,
            (i32)magnitudo(FabricaPars));
        filii = stml_invenire_omnes_liberos(nodus, "pars", piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
              StmlNodus* filius;
            FabricaPars  pars;
                 chorda* artificium;
                 chorda* actio;
                 chorda* aliud;
                    i32  numerus;

            filius      = *(StmlNodus**)xar_obtinere(filii, j);
            pars.sedes  = _sedes(piscina, via, filius);
            artificium  = stml_attributum_capere(filius, "artificium");
            actio       = stml_attributum_capere(filius, "actio");
            aliud       = stml_attributum_capere(filius, "compositum");
            numerus     = (artificium != NIHIL ? I : ZEPHYRUM)
                + (actio != NIHIL ? I : ZEPHYRUM)
                + (aliud != NIHIL ? I : ZEPHYRUM);
            si (numerus != I)
            {
                redde _recusare(piscina, causa_out, pars.sedes,
                    "pars nomen unum exacte postulat (artificium, "
                    "actio aut compositum)", nihil);
            }
            si (artificium != NIHIL)
            {
                pars.forma    = FABRICA_PARS_ARTIFICIUM;
                pars.titulus  = *artificium;
            }
            alioquin si (actio != NIHIL)
            {
                pars.forma    = FABRICA_PARS_ACTIO;
                pars.titulus  = *actio;
            }
            alioquin
            {
                pars.forma    = FABRICA_PARS_COMPOSITUM;
                pars.titulus  = *aliud;
            }
            *(FabricaPars*)xar_addere(compositum.partes) = pars;
        }
        si (xar_numerus(compositum.partes) == 0)
        {
            redde _recusare(piscina, causa_out, compositum.sedes,
                "compositum sine parte", compositum.titulus);
        }
        *(FabricaCompositum*)xar_addere(composita) = compositum;
    }
    redde composita;
}

interior b32
_chordam_continet (
     constans Xar* xar,
           chorda  valor)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(xar); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(xar, i), valor))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* exitum addere (semel); exitus non iudicatus recusatur */
interior b32
_partem_exitum_addere (
    constans FabricaExitus* exitus,
                    chorda  sedes,
                       Xar* viae,
                   Piscina* piscina,
                    chorda* causa_out)
{
    si (!exitus->strategia->iudicatur)
    {
        (vacuum)_recusare(piscina, causa_out, sedes,
            "pars est praecondicio (strategia numquam iudicata)",
            exitus->via);
        redde FALSUM;
    }
    si (!_chordam_continet(viae, exitus->via))
    {
        _chordam_addere(viae, exitus->via);
    }
    redde VERUM;
}

/* compositum 'titulus' in viae explicare; acervus = tituli in cursu
 * (cyclus) */
interior b32
_compositum_colligere (
     constans Xar* composita,
     constans Xar* actiones,
           chorda  titulus,
           chorda  sedes,
              Xar* acervus,
              Xar* viae,
          Piscina* piscina,
           chorda* causa_out)
{
    constans FabricaCompositum* compositum;
                           i32  i;
                           i32  j;
                           i32  k;

    compositum = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(composita); i++)
    {
        si (chorda_aequalis(((FabricaCompositum*)xar_obtinere(
                composita, i))->titulus, titulus))
        {
            compositum = (FabricaCompositum*)xar_obtinere(composita, i);
        }
    }
    si (compositum == NIHIL)
    {
        (vacuum)_recusare(piscina, causa_out, sedes,
            "compositum ignotum", titulus);
        redde FALSUM;
    }
    si (_chordam_continet(acervus, titulus))
    {
        ChordaAedificator* aedificator;

        aedificator = chorda_aedificator_creare(piscina, 128);
        si (aedificator == NIHIL)
        {
            redde FALSUM;
        }
        (vacuum)chorda_aedificator_appendere_literis(aedificator,
            "cyclus inter composita: ");
        per (i = ZEPHYRUM; i < xar_numerus(acervus); i++)
        {
            (vacuum)chorda_aedificator_appendere_chorda(aedificator,
                *(chorda*)xar_obtinere(acervus, i));
            (vacuum)chorda_aedificator_appendere_literis(aedificator,
                " -> ");
        }
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            titulus);
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(aedificator);
        }
        redde FALSUM;
    }
    _chordam_addere(acervus, titulus);
    per (i = ZEPHYRUM; i < xar_numerus(compositum->partes); i++)
    {
        constans FabricaPars* pars;
                         b32  inventa;

        pars     = (FabricaPars*)xar_obtinere(compositum->partes, i);
        inventa  = FALSUM;
        si (pars->forma == FABRICA_PARS_COMPOSITUM)
        {
            si (!_compositum_colligere(composita, actiones,
                    pars->titulus, pars->sedes, acervus, viae, piscina,
                    causa_out))
            {
                redde FALSUM;
            }
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actiones); j++)
        {
            constans FabricaActio* actio;

            actio = (FabricaActio*)xar_obtinere(actiones, j);
            si (   pars->forma == FABRICA_PARS_ACTIO
                && chorda_aequalis(actio->titulus, pars->titulus))
            {
                inventa = VERUM;
                per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
                {
                    si (!_partem_exitum_addere(
                            (FabricaExitus*)xar_obtinere(actio->exitus,
                            k), pars->sedes, viae, piscina, causa_out))
                    {
                        redde FALSUM;
                    }
                }
            }
            si (pars->forma == FABRICA_PARS_ARTIFICIUM)
            {
                per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
                {
                    constans FabricaExitus* exitus;

                    exitus = (FabricaExitus*)xar_obtinere(actio->exitus,
                        k);
                    si (!chorda_aequalis(exitus->via, pars->titulus))
                    {
                        perge;
                    }
                    inventa = VERUM;
                    si (!_partem_exitum_addere(exitus, pars->sedes,
                        viae,
                            piscina, causa_out))
                    {
                        redde FALSUM;
                    }
                }
            }
        }
        si (!inventa)
        {
            (vacuum)_recusare(piscina, causa_out, pars->sedes,
                pars->forma == FABRICA_PARS_ACTIO
                    ? "pars ignota (actio nulla)"
                    : "pars ignota (artificium nullum declaratum)",
                pars->titulus);
            redde FALSUM;
        }
    }
    (vacuum)xar_removere_ultimum(acervus);
    redde VERUM;
}

Xar*
fabrica_compositum_explicare (
     constans Xar* composita,
     constans Xar* actiones,
           chorda  titulus,
          Piscina* piscina,
           chorda* causa_out)
{
    Xar* acervus;
    Xar* viae;

    acervus  = _xar_chordarum(piscina);
    viae     = _xar_chordarum(piscina);
    si (acervus == NIHIL || viae == NIHIL)
    {
        redde NIHIL;
    }
    si (!_compositum_colligere(composita, actiones, titulus,
            chorda_ex_literis("fabrica", piscina), acervus, viae,
            piscina, causa_out))
    {
        redde NIHIL;
    }
    redde viae;
}

/* gravitas status: pessimum = maximum */
interior i32
_gravitas (
    FabricaStatus status)
{
    commutatio (status)
    {
        casus FABRICA_RECENS:
            redde ZEPHYRUM;
        casus FABRICA_NON_IUDICATUM:
            redde I;
        casus FABRICA_IGNOTUM:
            redde II;
        ordinarius:
            redde III;
    }
}

interior constans character*
_status_titulus (
    FabricaStatus status)
{
    commutatio (status)
    {
        casus FABRICA_RECENS:
            redde "RECENS";
        casus FABRICA_NON_IUDICATUM:
            redde "NON IUDICATUM";
        casus FABRICA_IGNOTUM:
            redde "IGNOTUM";
        ordinarius:
            redde "STALUM";
    }
}

FabricaIudicium
fabrica_iudicia_coniungere (
     constans Xar* iudicia,
           chorda  titulus,
          Piscina* piscina)
{
        FabricaStatus  pessimum;
    ChordaAedificator* aedificator;
                  i32  i;
                  i32  nominatae;
                  i32  reliquae;
            character  numerus[64];

    pessimum = FABRICA_RECENS;
    per (i = ZEPHYRUM; i < xar_numerus(iudicia); i++)
    {
        FabricaStatus status;

        status = ((FabricaIudicium*)xar_obtinere(iudicia, i))->status;
        si (_gravitas(status) > _gravitas(pessimum))
        {
            pessimum = status;
        }
    }
    aedificator = chorda_aedificator_creare(piscina, 128);
    si (aedificator == NIHIL)
    {
        redde _iudicium(titulus, pessimum, chorda_ex_literis("",
            piscina));
    }
    si (pessimum == FABRICA_RECENS)
    {
        sprintf(numerus, "RECENS: omnes partes recentes (%u)",
            (insignatus integer)xar_numerus(iudicia));
        redde _iudicium(titulus, pessimum, chorda_ex_literis(numerus,
            piscina));
    }
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        _status_titulus(pessimum));
    (vacuum)chorda_aedificator_appendere_literis(aedificator, ": ");
    nominatae  = ZEPHYRUM;
    reliquae   = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(iudicia); i++)
    {
        constans FabricaIudicium* iudicium;

        iudicium = (FabricaIudicium*)xar_obtinere(iudicia, i);
        si (iudicium->status != pessimum)
        {
            perge;
        }
        si (nominatae >= III)
        {
            reliquae++;
            perge;
        }
        si (nominatae > 0)
        {
            (vacuum)chorda_aedificator_appendere_literis(aedificator,
                ", ");
        }
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            iudicium->artificium);
        nominatae++;
    }
    si (reliquae > 0)
    {
        chorda romanus;

        (vacuum)chorda_aedificator_appendere_literis(aedificator, " +");
        romanus = numerus_romanus_exprimere((i64)reliquae, NIHIL,
            piscina);
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            romanus);
    }
    redde _iudicium(titulus, pessimum,
        chorda_aedificator_finire(aedificator));
}

b32
fabrica_praecondiciones_probare (
    constans Xar* actiones,
         Piscina* piscina,
          chorda* causa_out)
{
    i32 i;
    i32 j;
    i32 k;
    i32 l;

    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        constans FabricaActio* actio;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        per (j = ZEPHYRUM;
             actio->praecondiciones != NIHIL
             && j < xar_numerus(actio->praecondiciones);
             j++)
        {
            chorda nominata;
               b32 inventa;

            nominata = *(chorda*)xar_obtinere(actio->praecondiciones,
                j);
            inventa = FALSUM;
            per (k = ZEPHYRUM; k < xar_numerus(actiones); k++)
            {
                si (chorda_aequalis(((FabricaActio*)xar_obtinere(
                        actiones, k))->titulus, nominata))
                {
                    inventa = VERUM;
                }
            }
            si (!inventa)
            {
                (vacuum)_recusare(piscina, causa_out, actio->sedes,
                    "praecondicio ignota (actio nulla)", nominata);
                redde FALSUM;
            }
        }
        /* exitus ignotus ut ingressus: verdictum eius numquam notum -
         * sigillum quoque nullum habet sensum */
        per (j = ZEPHYRUM; j < xar_numerus(actio->ingressus); j++)
        {
            chorda via;

            via = ((FabricaIngressus*)xar_obtinere(actio->ingressus,
                j))->via;
            per (k = ZEPHYRUM; k < xar_numerus(actiones); k++)
            {
                constans FabricaActio* altera;

                altera = (FabricaActio*)xar_obtinere(actiones, k);
                per (l = ZEPHYRUM; l < xar_numerus(altera->exitus); l++)
                {
                    constans FabricaExitus* exitus;

                    exitus = (FabricaExitus*)xar_obtinere(
                        altera->exitus, l);
                    si (   !exitus->strategia->iudicatur
                        && chorda_aequalis(exitus->via, via))
                    {
                        (vacuum)_recusare(piscina, causa_out,
                            actio->sedes,
                            "ingressus est exitus numquam iudicatus - "
                            "praecondicio sola licet", via);
                        redde FALSUM;
                    }
                }
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Dependentiae per locos (plan 1b T5)
 * ================================================== */

/* loci unius ingressus: verbum enumerare (aut ex particulis); si
 * explicari nequit (absens - clonus recens), via declarata */
interior vacuum
_ingressus_loci (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                      Piscina* piscina,
                          Xar* loci)
{
       Xar* particulae;
    chorda  causa;
       b32  bonum;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    si (ingressus->genus->enumerare != NIHIL)
    {
        bonum = ingressus->genus->enumerare(sutura, ingressus, piscina,
            loci, &causa);
    }
    alioquin
    {
        particulae = xar_creare(piscina,
            (i32)magnitudo(FabricaParticula));
        bonum = particulae != NIHIL
            && ingressus->genus->sigillare(sutura, ingressus, NIHIL,
                   piscina, particulae, &causa);
        si (bonum)
        {
            _loci_ex_particulis(particulae, piscina, loci);
        }
    }
    si (!bonum)
    {
        _locum_addere(loci, FABRICA_LOCUS_PLAGULA, ingressus->via,
            chorda_ex_literis("", piscina));
    }
}

b32
fabrica_dependentias_computare (
    constans FabricaSutura* sutura,
                       Xar* actiones,
                   Piscina* piscina,
                    chorda* causa_out)
{
    TabulaDispersa* producentes;   /* via exitus -> FabricaActio* */
               i32  i;
               i32  j;
               i32  k;

    (vacuum)causa_out;
    producentes = tabula_dispersa_creare_chorda(piscina, 256);
    si (producentes == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            (vacuum)tabula_dispersa_inserere(producentes,
                ((FabricaExitus*)xar_obtinere(actio->exitus, j))->via,
                actio);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;
                 Xar* loci;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        actio->dependentiae = _xar_chordarum(piscina);
        si (loci == NIHIL || actio->dependentiae == NIHIL)
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->ingressus); j++)
        {
            _ingressus_loci(sutura,
                (FabricaIngressus*)xar_obtinere(actio->ingressus, j),
                piscina, loci);
        }
        /* ORDO EX VESTIGIO (plan 2 T2): quod actio lectiones="verum"
         * cursu ultimo legit, quaesivit, enumeravit - arcus ad
         * producentes, ut manifesta quae vestigium supplet */
        {
            Xar* ultimae;

            si (   actio->lectiones
                && sutura->lectiones_ultimae != NIHIL
                && sutura->lectiones_ultimae(sutura->datum,
                       chorda_ut_cstr(actio->titulus, piscina), piscina,
                       &ultimae))
            {
                per (j = ZEPHYRUM; j < xar_numerus(ultimae); j++)
                {
                    constans FabricaLectio* l;

                    l = (constans FabricaLectio*)xar_obtinere(ultimae,
                        j);
                    _locum_addere(loci,
                        l->genus == LECTIO_ENUMERAVIT
                            ? FABRICA_LOCUS_PLAGULAE
                            : FABRICA_LOCUS_PLAGULA,
                        l->via, chorda_ex_literis("", piscina));
                }
            }
        }
        per (j = ZEPHYRUM; j < xar_numerus(loci); j++)
        {
            FabricaLocus* locus;
                  vacuum* inventa;

            locus = (FabricaLocus*)xar_obtinere(loci, j);
            si (locus->forma == FABRICA_LOCUS_PLAGULA)
            {
                FabricaActio* producens;

                si (!tabula_dispersa_invenire(producentes, locus->via,
                        &inventa))
                {
                    perge;
                }
                producens = (FabricaActio*)inventa;
                si (   producens != actio
                    && !_chordam_continet(actio->dependentiae,
                           producens->titulus))
                {
                    _chordam_addere(actio->dependentiae,
                        producens->titulus);
                }
                perge;
            }
            /* PLAGULAE aut ARBOR: exitus omnes inspiciuntur (rari) */
            per (k = ZEPHYRUM; k < xar_numerus(actiones); k++)
            {
                FabricaActio* altera;
                         i32  l;

                altera = (FabricaActio*)xar_obtinere(actiones, k);
                si (   altera == actio
                    || _chordam_continet(actio->dependentiae,
                           altera->titulus))
                {
                    perge;
                }
                per (l = ZEPHYRUM; l < xar_numerus(altera->exitus); l++)
                {
                    chorda via;
                    chorda directorium;
                    chorda titulus;

                    via = ((FabricaExitus*)xar_obtinere(altera->exitus,
                        l))->via;
                    _viam_dividere(via, &directorium, &titulus);
                    si (_locus_tangit(locus, via, directorium, titulus,
                            piscina))
                    {
                        _chordam_addere(actio->dependentiae,
                            altera->titulus);
                        frange;
                    }
                }
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ordo
 * ================================================== */

/* VERUM si posterior exitum prioris legit aut priorem praecondicionem
 * nominat */
interior b32
_pendet (
    constans FabricaActio* posterior,
    constans FabricaActio* prior)
{
    i32 i;
    i32 j;

    /* praecondicio: ordo sine sigillo */
    per (i = ZEPHYRUM;
         posterior->praecondiciones != NIHIL
         && i < xar_numerus(posterior->praecondiciones);
         i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(
                posterior->praecondiciones, i), prior->titulus))
        {
            redde VERUM;
        }
    }
    /* dependentiae computatae (per locos, T5) vias declaratas
     * supplent */
    si (posterior->dependentiae != NIHIL)
    {
        per (i = ZEPHYRUM; i
            < xar_numerus(posterior->dependentiae); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(
                    posterior->dependentiae, i), prior->titulus))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(posterior->ingressus); i++)
    {
        FabricaIngressus* ingressus;

        ingressus = (FabricaIngressus*)xar_obtinere(
            posterior->ingressus, i);
        per (j = ZEPHYRUM; j < xar_numerus(prior->exitus); j++)
        {
            FabricaExitus* exitus;

            exitus = (FabricaExitus*)xar_obtinere(prior->exitus, j);
            si (chorda_aequalis(ingressus->via, exitus->via))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

Xar*
fabrica_ordinare (
    constans Xar* actiones,
         Piscina* piscina,
          chorda* causa_out)
{
    Xar* ordo;
    i32* gradus;
    b32* posita;
    i32  numerus;
    i32  positae;
    i32  i;
    i32  j;

    numerus  = xar_numerus(actiones);
    ordo     = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
    gradus = (i32*)piscina_allocare(piscina,
        magnitudo(i32) * (memoriae_index)(numerus + 1));
    posita = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)(numerus + 1));
    si (ordo == NIHIL || gradus == NIHIL || posita == NIHIL)
    {
        redde NIHIL;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        gradus[j] = ZEPHYRUM;
        posita[j] = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (   i != j && _pendet(
                    (FabricaActio*)xar_obtinere(actiones, j),
                    (FabricaActio*)xar_obtinere(actiones, i)))
            {
                gradus[j]++;
            }
        }
    }

    /* Kahn, ordine dato inter paratas (determinismus) */
    positae = ZEPHYRUM;
    dum (positae < numerus)
    {
        s32 electa;

        electa = -I;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (!posita[j] && gradus[j] == 0)
            {
                electa = (s32)j;
                frange;
            }
        }
        si (electa < 0)
        {
            frange;
        }
        posita[electa] = VERUM;
        positae++;
        *(FabricaActio**)xar_addere(ordo) =
            (FabricaActio*)xar_obtinere(actiones, (i32)electa);
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   !posita[j] && _pendet(
                    (FabricaActio*)xar_obtinere(actiones, j),
                    (FabricaActio*)xar_obtinere(actiones,
                        (i32)electa)))
            {
                gradus[j]--;
            }
        }
    }

    si (positae < numerus)
    {
        ChordaAedificator* aedificator;
                      b32  primum;

        aedificator = chorda_aedificator_creare(piscina, 128);
        si (aedificator == NIHIL)
        {
            redde NIHIL;
        }
        (vacuum)chorda_aedificator_appendere_literis(aedificator,
            "cyclus inter actiones: ");
        primum = VERUM;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (!posita[j])
            {
                si (!primum)
                {
                    (vacuum)chorda_aedificator_appendere_literis(
                        aedificator, ", ");
                }
                (vacuum)chorda_aedificator_appendere_chorda(aedificator,
                    ((FabricaActio*)xar_obtinere(actiones,
                    j))->titulus);
                primum = FALSUM;
            }
        }
        si (causa_out != NIHIL)
        {
            *causa_out = chorda_aedificator_finire(aedificator);
        }
        redde NIHIL;
    }
    redde ordo;
}


/* ==================================================
 * Vestigia (plan 1b T4): actio scribit SOLUM intra vestigium suum
 * ================================================== */

/* vestigium actionis: locare exituum + vestigia + communia +
 * involucrum (acta, provenientia, scriptura iudicis). status_fabricae:
 * etiam status executoris ipsius (memoria sqlite, sera) - pro
 * probatione scripturae VERUM, pro undis FALSUM (omnibus communis,
 * executoris non actionis: undas omnes separaret) */
interior Xar*
_vestigium_actionis (
    constans FabricaActio* actio,
                      b32  status_fabricae,
                  Piscina* piscina)
{
       Xar* loci;
    chorda  vacua;
       i32  i;

    loci   = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
    vacua  = chorda_ex_literis("", piscina);
    si (loci == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        constans FabricaExitus* exitus;

        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, i);
        si (exitus->genus != NIHIL && exitus->genus->locare != NIHIL)
        {
            (vacuum)exitus->genus->locare(exitus, piscina, loci);
        }
    }
    per (i = ZEPHYRUM;
         actio->vestigia != NIHIL && i < xar_numerus(actio->vestigia);
         i++)
    {
        *(FabricaLocus*)xar_addere(loci) =
            *(FabricaLocus*)xar_obtinere(actio->vestigia, i);
    }
    per (i = ZEPHYRUM;
         actio->communia != NIHIL && i < xar_numerus(actio->communia);
         i++)
    {
        *(FabricaLocus*)xar_addere(loci) =
            *(FabricaLocus*)xar_obtinere(actio->communia, i);
    }
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, _iungere(piscina,
        "build/fabrica/acta/", actio->titulus, ".log"), vacua);
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, _iungere(piscina,
        "build/fabrica/provenientia/", actio->titulus, ".c"), vacua);
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, _iungere(piscina,
        "build/fabrica/provenientia/", actio->titulus, ".o"), vacua);
    _locum_addere(loci, FABRICA_LOCUS_ARBOR, _iungere(piscina,
        "build/fabrica/scriptura/", actio->titulus, ""), vacua);
    /* libri lectionum actionis (plan 2 T2 iudex, T6 sanare simul) */
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, _iungere(piscina,
        LIBRI_DIRECTORIUM, actio->titulus, ".tsv"), vacua);
    _locum_addere(loci, FABRICA_LOCUS_PLAGULA, _iungere(piscina,
        LIBRI_DIRECTORIUM, actio->titulus, ".sanare.tsv"), vacua);
    /* status fabricae ipsius (memoria sqlite cum -wal/-shm, sera):
     * involucrum, non actio - T4: mensor_ui -shm tetigit */
    si (status_fabricae)
    {
        _locum_addere(loci, FABRICA_LOCUS_PLAGULAE, chorda_ex_literis(
            "build", piscina), chorda_ex_literis(
            "fabrica.db fabrica.db-wal fabrica.db-shm", piscina));
        _locum_addere(loci, FABRICA_LOCUS_PLAGULA, chorda_ex_literis(
            "build/fabrica/sera", piscina), vacua);
        /* THESAURUS (plan 2 T3): cache communis fabricae, scriptura
         * atomica - quaevis actio domus (aedilis --thesaurus,
         * compilator) in eo scribit; involucrum, non vestigium actionis
         * (radix ipsa, obiecta .o, NON: ea vestigia sua manent) */
        {
            i32 k;

            per (k = ZEPHYRUM; k < III; k++)
            {
                chorda pars;

                /* sine '/' finali: locus ARBOR 'via/' ipse addit */
                pars = chorda_ex_literis(_thesauri_partes[k], piscina);
                pars.mensura--;
                _locum_addere(loci, FABRICA_LOCUS_ARBOR, pars, vacua);
            }
        }
    }
    redde loci;
}

interior b32
_in_locis (
     constans Xar* loci,
           chorda  via,
          Piscina* piscina)
{
    chorda directorium;
    chorda titulus;
       i32 i;

    _viam_dividere(via, &directorium, &titulus);
    per (i = ZEPHYRUM; i < xar_numerus(loci); i++)
    {
        si (_locus_tangit((FabricaLocus*)xar_obtinere(loci, i), via,
                directorium, titulus, piscina))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

Xar*
fabrica_vestigia_comparare (
    constans FabricaActio* actio,
             constans Xar* ante,
             constans Xar* post,
                  Piscina* piscina)
{
    Xar* loci;
    Xar* extra;
    i32  i;
    i32  j;

    loci   = _vestigium_actionis(actio, VERUM, piscina);
    extra  = _xar_chordarum(piscina);
    si (loci == NIHIL || extra == NIHIL)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (i < xar_numerus(ante) || j < xar_numerus(post))
    {
        constans FabricaVestigium* a;
        constans FabricaVestigium* b;
                              s32  ordo;
                           chorda  mutata;

        a = (i < xar_numerus(ante))
            ? (FabricaVestigium*)xar_obtinere(ante, i) : NIHIL;
        b = (j < xar_numerus(post))
            ? (FabricaVestigium*)xar_obtinere(post, j) : NIHIL;
        si (a == NIHIL)
        {
            ordo = I;
        }
        alioquin si (b == NIHIL)
        {
            ordo = -I;
        }
        alioquin
        {
            ordo = chorda_comparare(a->via, b->via);
        }
        si (ordo < 0)
        {
            mutata = a->via;   /* deleta */
            i++;
        }
        alioquin si (ordo > 0)
        {
            mutata = b->via;   /* nova */
            j++;
        }
        alioquin
        {
            i++;
            j++;
            si (   a->tempus_ns == b->tempus_ns
                && a->mensura   == b->mensura)
            {
                perge;
            }
            mutata = a->via;   /* scripta (octetis eisdem quoque) */
        }
        si (!_in_locis(loci, mutata, piscina))
        {
            _chordam_addere(extra, mutata);
        }
    }
    redde extra;
}

/* loci concurrunt si via una alteram continet (conservativum) */
interior b32
_loci_concurrunt (
    constans FabricaLocus* a,
    constans FabricaLocus* b,
                  Piscina* piscina)
{
    si (chorda_aequalis(a->via, b->via))
    {
        redde VERUM;
    }
    redde chorda_incipit(b->via, _iungere(piscina, "", a->via, "/"))
        || chorda_incipit(a->via, _iungere(piscina, "", b->via, "/"));
}

interior b32
_simul_possunt (
    constans FabricaActio* x,
    constans FabricaActio* y,
                  Piscina* piscina)
{
    Xar* lx;
    Xar* ly;
    i32  i;
    i32  j;

    si (   (x->communia != NIHIL && xar_numerus(x->communia) > 0)
        || (y->communia != NIHIL && xar_numerus(y->communia) > 0))
    {
        redde FALSUM;
    }
    si (_pendet(x, y) || _pendet(y, x))
    {
        redde FALSUM;
    }
    lx = _vestigium_actionis(x, FALSUM, piscina);
    ly = _vestigium_actionis(y, FALSUM, piscina);
    per (i = ZEPHYRUM; i < xar_numerus(lx); i++)
    {
        per (j = ZEPHYRUM; j < xar_numerus(ly); j++)
        {
            si (_loci_concurrunt((FabricaLocus*)xar_obtinere(lx, i),
                    (FabricaLocus*)xar_obtinere(ly, j), piscina))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

Xar*
fabrica_undas_formare (
    constans Xar* ordo,
         Piscina* piscina)
{
     Xar* undae;
     i32* unda;
     i32  numerus;
     i32  i;
     i32  j;

    numerus  = xar_numerus(ordo);
    undae    = xar_creare(piscina, (i32)magnitudo(Xar*));
    unda = (i32*)piscina_allocare(piscina,
        magnitudo(i32) * (memoriae_index)(numerus + 1));
    si (undae == NIHIL || unda == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FabricaActio* actio;
                 i32  w;

        actio  = *(FabricaActio**)xar_obtinere(ordo, i);
        w      = ZEPHYRUM;
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (   _pendet(actio, *(FabricaActio**)xar_obtinere(ordo,
                j))
                && unda[j] + I > w)
            {
                w = unda[j] + I;
            }
        }
        dum (w < xar_numerus(undae))
        {
            Xar* membra;
            b32  apta;

            membra  = *(Xar**)xar_obtinere(undae, w);
            apta    = VERUM;
            per (j = ZEPHYRUM; apta && j < xar_numerus(membra); j++)
            {
                apta = _simul_possunt(actio,
                    *(FabricaActio**)xar_obtinere(membra, j), piscina);
            }
            si (apta)
            {
                frange;
            }
            w++;
        }
        si (w == xar_numerus(undae))
        {
            *(Xar**)xar_addere(undae) = xar_creare(piscina,
                (i32)magnitudo(FabricaActio*));
        }
        *(FabricaActio**)xar_addere(*(Xar**)xar_obtinere(undae, w)) =
            actio;
        unda[i] = w;
    }
    redde undae;
}


/* ==================================================
 * Sanare (plan 1b T3): iudicare, agere, iterum iudicare
 * ================================================== */

/* status interni actionis per cursum */
nomen enumeratio {
    SANANDI_NONDUM = ZEPHYRUM,
    SANANDI_RECENS,
    SANANDI_SANATUM,
    SANANDI_PRAEPARATUM,
    SANANDI_FRACTUM,
    SANANDI_OMISSUM,
    SANANDI_AGENDUM,
    SANANDI_FORTASSE
} StatusSanandi;

/* VERUM si omnes exitus strategiae non iudicatae (ignota): actio
 * praecondicione SOLA realizatur */
interior b32
_actio_ignota (
    constans FabricaActio* actio)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        si (((FabricaExitus*)xar_obtinere(actio->exitus,
                i))->strategia->iudicatur)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* exitus iudicati actionis NUNC (plenus): VERUM si omnes RECENS;
 * aliter causa = primus non RECENS "via: causa" */
interior b32
_exitus_recentes (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                    chorda* causa_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(actio->exitus); i++)
    {
        constans FabricaExitus* exitus;
               FabricaIudicium  iudicium;

        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, i);
        si (!exitus->strategia->iudicatur)
        {
            perge;
        }
        iudicium = fabrica_iudicare(sutura, actio, exitus, VERUM,
            piscina);
        si (iudicium.status != FABRICA_RECENS)
        {
            *causa_out = _iungere(piscina, "",
                _iungere(piscina, "", exitus->via, ": "),
                chorda_ut_cstr(iudicium.causa, piscina));
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* memoriae per cursum descriptiones octetorum sunt quae actum modo
 * mutavit (Review Focus 2): post quodque actum vacantur */
interior vacuum
_memorias_vacare (
    constans FabricaSutura* sutura)
{
    si (sutura->sigilla != NIHIL)
    {
        tabula_dispersa_vacare(sutura->sigilla);
    }
    si (sutura->regenerationes != NIHIL)
    {
        tabula_dispersa_vacare(sutura->regenerationes);
    }
    si (sutura->digesta != NIHIL)
    {
        tabula_dispersa_vacare(sutura->digesta);
    }
}

/* agere per suturam; VERUM si codex 0. causa: "exitus N: cauda" aut
 * cauda (non incepit) */
interior b32
_actionem_agere (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       i32* duratio_out,
                    chorda* causa_out)
{
    FabricaActum  actum;
             b32  incepit;
       character  numerus[32];
             Xar* ante;
             Xar* post;

    actum.codex       = -I;
    actum.duratio_ms  = ZEPHYRUM;
    actum.cauda       = chorda_ex_literis("", piscina);
    ante              = NIHIL;
    post              = NIHIL;
    si (sutura->agere == NIHIL)
    {
        *causa_out = chorda_ex_literis("sutura sine agere", piscina);
        redde FALSUM;
    }
    /* photographia ante et post (T4): scriptura extra vestigium */
    si (sutura->vestigium_capere != NIHIL)
    {
        (vacuum)sutura->vestigium_capere(sutura->datum, piscina, &ante);
    }
    incepit = sutura->agere(sutura->datum, actio,
        chorda_ut_cstr(_iungere(piscina, "build/fabrica/acta/",
            actio->titulus, ".log"), piscina), piscina, &actum);
    _memorias_vacare(sutura);
    *duratio_out = actum.duratio_ms;
    si (!incepit)
    {
        *causa_out = _iungere(piscina, "non actum: ", actum.cauda, "");
        redde FALSUM;
    }
    si (actum.codex != 0)
    {
        sprintf(numerus, "exitus %d: ", (integer)actum.codex);
        *causa_out = _iungere(piscina, numerus, actum.cauda, "");
        redde FALSUM;
    }
    si (   ante != NIHIL
        && sutura->vestigium_capere(sutura->datum, piscina, &post))
    {
        Xar* extra;

        extra = fabrica_vestigia_comparare(actio, ante, post, piscina);
        si (extra != NIHIL && xar_numerus(extra) > 0)
        {
            chorda nuntius;
               i32 k;

            /* III nominatae, deinde "+N" (ut iudicia_coniungere) */
            nuntius = chorda_ex_literis("scripsit extra vestigium: ",
                piscina);
            per (k = ZEPHYRUM; k < xar_numerus(extra) && k < III; k++)
            {
                si (k > 0)
                {
                    nuntius = _iungere(piscina, "", nuntius, ", ");
                }
                nuntius = chorda_concatenare(nuntius,
                    *(chorda*)xar_obtinere(extra, k), piscina);
            }
            si (xar_numerus(extra) > III)
            {
                nuntius = _iungere(piscina, "", nuntius, " +");
                nuntius = chorda_concatenare(nuntius,
                    numerus_romanus_exprimere(
                        (i64)(xar_numerus(extra) - III), NIHIL,
                        piscina),
                    piscina);
            }
            *causa_out = _iungere(piscina, "", nuntius,
                " - aut manu mutata dum currebat");
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior s32
_index_actionis (
     constans Xar* ordo,
           chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        si (chorda_aequalis((*(FabricaActio**)xar_obtinere(ordo,
                i))->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* sanationem addere: eventus solus decernit - actum (sanatum,
 * fractum, praeparatum) in cursu scribitur; siccum (agendum,
 * fortasse) tempus ex cursu ultimo aestimat; omissum neutrum (1b T7) */
interior vacuum
_sanationem_notare (
     constans FabricaSutura* sutura,
                    Piscina* piscina,
                        Xar* sanationes,
      constans FabricaActio* actio,
             FabricaEventus  eventus,
                     chorda  causa,
                        i32  duratio_ms)
{
    FabricaSanatio* sanatio;

    sanatio = (FabricaSanatio*)xar_addere(sanationes);
    si (sanatio == NIHIL)
    {
        redde;
    }
    sanatio->actio         = actio;
    sanatio->eventus       = eventus;
    sanatio->causa         = causa;
    sanatio->duratio_ms    = duratio_ms;
    sanatio->tempus_notum  = VERUM;
    commutatio (eventus)
    {
        casus FABRICA_SANATUM:
        casus FABRICA_FRACTUM:
        casus FABRICA_PRAEPARATUM:
            si (sutura->cursum_inscribere != NIHIL)
            {
                sutura->cursum_inscribere(sutura->datum, sanatio);
            }
            frange;
        casus FABRICA_AGENDUM:
        casus FABRICA_FORTASSE:
            sanatio->duratio_ms    = ZEPHYRUM;
            sanatio->tempus_notum  = sutura->cursum_legere != NIHIL
                && sutura->cursum_legere(sutura->datum,
                       chorda_ut_cstr(actio->titulus, piscina),
                       &sanatio->duratio_ms);
            frange;
        ordinarius:
            sanatio->tempus_notum = FALSUM;
            frange;
    }
}

/* dependentia fracta aut omissa (ingressus aut praecondicio): VERUM
 * et titulus eius */
interior b32
_dependentia_fracta (
     constans Xar* ordo,
              i32  i,
    StatusSanandi* status,
           chorda* titulus_out)
{
    constans FabricaActio* actio;
                      i32  j;

    actio = *(FabricaActio**)xar_obtinere(ordo, i);
    per (j = ZEPHYRUM; j < i; j++)
    {
        si (   (   status[j] == SANANDI_FRACTUM
                || status[j] == SANANDI_OMISSUM)
            && _pendet(actio, *(FabricaActio**)xar_obtinere(ordo, j)))
        {
            *titulus_out = (*(FabricaActio**)xar_obtinere(ordo,
                j))->titulus;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ANTE AGERE: dependentia fracta, recentia, siccum, praecondiciones
 * ignotae. VERUM = actio agenda (status SANANDI_NONDUM manet); FALSUM =
 * iam tractata (OMISSUM, RECENS, FORTASSE, AGENDUM notata). */
interior b32
_ante_agere (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       i32  i,
             StatusSanandi* status,
                       Xar* sanationes,
                       b32  siccum,
                   Piscina* piscina)
{
    constans FabricaActio* actio;
                   chorda  causa;
                   chorda  fracta;
                      b32  impedita;
                      i32  duratio;
                      i32  j;

    actio = *(FabricaActio**)xar_obtinere(ordo, i);

    /* dependentia fracta aut omissa (ingressus aut praecondicio) */
    impedita  = FALSUM;
    fracta    = chorda_ex_literis("", piscina);
    per (j = ZEPHYRUM; j < i; j++)
    {
        si (   (   status[j] == SANANDI_FRACTUM
                || status[j] == SANANDI_OMISSUM)
            && _pendet(actio, *(FabricaActio**)xar_obtinere(ordo, j)))
        {
            impedita  = VERUM;
            fracta    = (*(FabricaActio**)xar_obtinere(ordo,
                j))->titulus;
            frange;
        }
    }
    si (impedita)
    {
        status[i] = SANANDI_OMISSUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_OMISSUM,
            _iungere(piscina, "dependentia fracta: ", fracta, ""),
            ZEPHYRUM);
        redde FALSUM;
    }

    causa = chorda_ex_literis("", piscina);
    si (_exitus_recentes(sutura, actio, piscina, &causa))
    {
        /* siccum: recens nunc, sed post actionem agendam fortasse
         * non */
        si (siccum)
        {
            per (j = ZEPHYRUM; j < i; j++)
            {
                si (   (   status[j] == SANANDI_AGENDUM
                        || status[j] == SANANDI_FORTASSE)
                    && _pendet(actio,
                           *(FabricaActio**)xar_obtinere(ordo, j)))
                {
                    status[i] = SANANDI_FORTASSE;
                    _sanationem_notare(sutura, piscina, sanationes,
                        actio,
                        FABRICA_FORTASSE, _iungere(piscina,
                            "post ", (*(FabricaActio**)xar_obtinere(
                            ordo, j))->titulus, ""), ZEPHYRUM);
                    frange;
                }
            }
        }
        si (status[i] == SANANDI_NONDUM)
        {
            status[i] = SANANDI_RECENS;
        }
        redde FALSUM;
    }
    si (siccum)
    {
        status[i] = SANANDI_AGENDUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_AGENDUM,
            causa,
            ZEPHYRUM);
        redde FALSUM;
    }

    /* praecondiciones ignotae: semel per cursum */
    per (j = ZEPHYRUM;
         actio->praecondiciones != NIHIL
         && j < xar_numerus(actio->praecondiciones);
         j++)
    {
        s32 k;

        k = _index_actionis(ordo,
            *(chorda*)xar_obtinere(actio->praecondiciones, j));
        si (   k < 0
            || status[k] != SANANDI_NONDUM
            || !_actio_ignota(*(FabricaActio**)xar_obtinere(ordo,
                   (i32)k)))
        {
            perge;
        }
        duratio = ZEPHYRUM;
        si (_actionem_agere(sutura, *(FabricaActio**)xar_obtinere(
                ordo, (i32)k), piscina, &duratio, &causa))
        {
            status[k] = SANANDI_PRAEPARATUM;
            _sanationem_notare(sutura, piscina, sanationes,
                *(FabricaActio**)xar_obtinere(ordo, (i32)k),
                FABRICA_PRAEPARATUM, chorda_ex_literis("", piscina),
                duratio);
        }
        alioquin
        {
            status[k] = SANANDI_FRACTUM;
            _sanationem_notare(sutura, piscina, sanationes,
                *(FabricaActio**)xar_obtinere(ordo, (i32)k),
                FABRICA_FRACTUM, causa, duratio);
        }
    }
    /* praecondicio modo fracta: omittitur */
    per (j = ZEPHYRUM;
         actio->praecondiciones != NIHIL
         && j < xar_numerus(actio->praecondiciones);
         j++)
    {
        s32 k;

        k = _index_actionis(ordo,
            *(chorda*)xar_obtinere(actio->praecondiciones, j));
        si (   k >= 0
            && (   status[k] == SANANDI_FRACTUM
                || status[k] == SANANDI_OMISSUM))
        {
            status[i] = SANANDI_OMISSUM;
            _sanationem_notare(sutura, piscina, sanationes, actio,
                FABRICA_OMISSUM,
                _iungere(piscina, "praecondicio fracta: ",
                    *(chorda*)xar_obtinere(actio->praecondiciones, j),
                    ""), ZEPHYRUM);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* POST AGERE: actum FALSUM -> FRACTUM (causa); aliter post-condicio
 * (Review Focus 3: exitus 0 non sufficit) -> SANATUM aut FRACTUM */
interior vacuum
_post_agere (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                       i32  i,
             StatusSanandi* status,
                       Xar* sanationes,
                       b32  actum,
                    chorda  causa,
                       i32  duratio,
                   Piscina* piscina)
{
    si (!actum)
    {
        status[i] = SANANDI_FRACTUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_FRACTUM, causa, duratio);
        redde;
    }
    si (!_exitus_recentes(sutura, actio, piscina, &causa))
    {
        status[i] = SANANDI_FRACTUM;
        _sanationem_notare(sutura, piscina, sanationes, actio,
            FABRICA_FRACTUM,
            _iungere(piscina, "exitus 0 sed non RECENS: ", causa, ""),
            duratio);
        redde;
    }
    status[i] = SANANDI_SANATUM;
    _sanationem_notare(sutura, piscina, sanationes, actio,
        FABRICA_SANATUM, chorda_ex_literis("", piscina), duratio);
}

/* via libri normata: radix et './' demptae; NIHIL-chorda si extra
 * arborem (absoluta) */
interior chorda
_viam_libri_normare (
    constans FabricaSutura* sutura,
                    chorda  via)
{
    si (   sutura->radix.mensura > 0
        && via.mensura > sutura->radix.mensura
        && chorda_incipit(via, sutura->radix)
        && via.datum[sutura->radix.mensura] == '/')
    {
        via = chorda_sectio(via, sutura->radix.mensura + I,
            via.mensura);
    }
    dum (via.mensura > II && via.datum[0] == '.' && via.datum[I] == '/')
    {
        via = chorda_sectio(via, II, via.mensura);
    }
    si (via.mensura > 0 && via.datum[0] == '/')
    {
        via.mensura = ZEPHYRUM;
    }
    redde via;
}

interior s32
_vestigia_via_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaVestigium*)a)->via,
        ((constans FabricaVestigium*)b)->via);
}

/* SCRIPTURAE NOTATAE (S) libri membri: viae normatae (extra arborem
 * praetermissae - photographia eas quoque non videt). Xar de chorda;
 * vacua si liber absens. */
interior Xar*
_scripturas_notatas (
    constans FabricaSutura* sutura,
        constans character* liber_via,
                   Piscina* piscina)
{
    chorda  liber;
       Xar* viae;
       i32  i;
       i32  initium;

    viae = _xar_chordarum(piscina);
    si (   viae == NIHIL || sutura->legere == NIHIL
        || !sutura->legere(sutura->datum, liber_via, piscina, &liber))
    {
        redde viae;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
        chorda linea;
        chorda via;

        si (liber.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(liber, initium, i);
        initium  = i + I;
        si (   linea.mensura < III || linea.datum[0] != 'S'
            || linea.datum[I] != '\t')
        {
            perge;
        }
        via = _viam_libri_normare(sutura,
            chorda_sectio(linea, II, linea.mensura));
        si (via.mensura > ZEPHYRUM)
        {
            _chordam_addere(viae, via);
        }
    }
    redde viae;
}

/* prima via notata extra vestigium actionis (vacua = nulla) */
interior chorda
_scripturam_alienam (
     constans FabricaActio* actio,
              constans Xar* notatae,
                   Piscina* piscina)
{
    Xar* ante;
    Xar* post;
    Xar* extra;
    i32  i;

    ante = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    post = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    per (i = ZEPHYRUM; post != NIHIL && i < xar_numerus(notatae); i++)
    {
        FabricaVestigium* v;

        v = (FabricaVestigium*)xar_addere(post);
        si (v == NIHIL)
        {
            frange;
        }
        v->via        = *(chorda*)xar_obtinere(notatae, i);
        v->tempus_ns  = ZEPHYRUM;
        v->mensura    = ZEPHYRUM;
    }
    si (post == NIHIL || xar_numerus(post) == 0)
    {
        redde chorda_ex_literis("", piscina);
    }
    xar_ordinare(post, _vestigia_via_comparare);
    extra = fabrica_vestigia_comparare(actio, ante, post, piscina);
    si (extra != NIHIL && xar_numerus(extra) > 0)
    {
        redde *(chorda*)xar_obtinere(extra, ZEPHYRUM);
    }
    redde chorda_ex_literis("", piscina);
}

/* UNDA una simul: membra tuta (indices in ordine, ordine tituli),
 * photographia una circa undam, liber per membrum. VERUM si quid
 * fractum. */
interior b32
_undam_agere (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       Xar* indices,
             StatusSanandi* status,
                       Xar* sanationes,
                   Piscina* piscina)
{
    constans FabricaActio** actiones;
       constans character** acta_viae;
       constans character** libri;
             FabricaActum*  acta;
                      b32*  incepta;
                      Xar*  ante;
                      Xar*  post;
                      Xar*  extra_undae;
                      Xar** notatae;
                      i32   n;
                      i32   k;
                      i32   m;
                      b32   fractum;

    n = xar_numerus(indices);
    /* ordo tituli (cursus et sanationes deterministice) */
    per (k = I; k < n; k++)
    {
        per (m = k; m > ZEPHYRUM; m--)
        {
            i32* a;
            i32* b;

            a = (i32*)xar_obtinere(indices, m - I);
            b = (i32*)xar_obtinere(indices, m);
            si (chorda_comparare(
                    (*(FabricaActio**)xar_obtinere(ordo, *a))->titulus,
                    (*(FabricaActio**)xar_obtinere(ordo, *b))->titulus)
                <= 0)
            {
                frange;
            }
            {
                i32 t;

                t   = *a;
                *a  = *b;
                *b  = t;
            }
        }
    }
    actiones  = (constans FabricaActio**)piscina_allocare(piscina,
        magnitudo(FabricaActio*) * (memoriae_index)n);
    acta_viae = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)n);
    libri     = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)n);
    acta      = (FabricaActum*)piscina_allocare(piscina,
        magnitudo(FabricaActum) * (memoriae_index)n);
    incepta   = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)n);
    si (   actiones == NIHIL || acta_viae == NIHIL || libri == NIHIL
        || acta     == NIHIL || incepta == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        actiones[k] = *(FabricaActio**)xar_obtinere(ordo,
            *(i32*)xar_obtinere(indices, k));
        acta_viae[k] = chorda_ut_cstr(_iungere(piscina,
            "build/fabrica/acta/", actiones[k]->titulus, ".log"),
            piscina);
        libri[k] = chorda_ut_cstr(_iungere(piscina, LIBRI_DIRECTORIUM,
            actiones[k]->titulus, ".sanare.tsv"), piscina);
        incepta[k] = FALSUM;
    }
    ante = NIHIL;
    post = NIHIL;
    si (sutura->vestigium_capere != NIHIL)
    {
        (vacuum)sutura->vestigium_capere(sutura->datum, piscina, &ante);
    }
    sutura->agere_simul(sutura->datum,
        (constans FabricaActio* constans*)actiones, n,
        (constans character* constans*)acta_viae,
        (constans character* constans*)libri, piscina, acta, incepta);
    _memorias_vacare(sutura);
    notatae = (Xar**)piscina_allocare(piscina,
        magnitudo(Xar*) * (memoriae_index)n);
    si (notatae == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        notatae[k] = _scripturas_notatas(sutura, libri[k], piscina);
    }
    /* scripturae extra UNIONEM vestigiorum: extra cuiusque membri ET a
     * nullo libro notatae (notata membro suo imputatur, non undae) */
    extra_undae = NIHIL;
    si (   ante != NIHIL
        && sutura->vestigium_capere(sutura->datum, piscina, &post))
    {
        extra_undae = fabrica_vestigia_comparare(actiones[0], ante,
            post,
            piscina);
        per (k = I; extra_undae != NIHIL && k < n; k++)
        {
            Xar* extra_k;
            Xar* communia;
            i32  e;

            extra_k  = fabrica_vestigia_comparare(actiones[k], ante,
                post,
                piscina);
            communia = _xar_chordarum(piscina);
            per (e = ZEPHYRUM; extra_k != NIHIL && communia != NIHIL
                 && e < xar_numerus(extra_undae); e++)
            {
                chorda via;
                   i32 f;

                via = *(chorda*)xar_obtinere(extra_undae, e);
                per (f = ZEPHYRUM; f < xar_numerus(extra_k); f++)
                {
                    si (chorda_aequalis(via,
                            *(chorda*)xar_obtinere(extra_k, f)))
                    {
                        _chordam_addere(communia, via);
                        frange;
                    }
                }
            }
            extra_undae = communia;
        }
    }
    si (extra_undae != NIHIL)
    {
        Xar* ignotae;
        i32  e;

        ignotae = _xar_chordarum(piscina);
        per (e = ZEPHYRUM; ignotae != NIHIL
             && e < xar_numerus(extra_undae); e++)
        {
            chorda via;
               b32 notata;
               i32 f;

            via     = *(chorda*)xar_obtinere(extra_undae, e);
            notata  = FALSUM;
            per (k = ZEPHYRUM; !notata && k < n; k++)
            {
                per (f = ZEPHYRUM; f < xar_numerus(notatae[k]); f++)
                {
                    si (chorda_aequalis(via,
                            *(chorda*)xar_obtinere(notatae[k], f)))
                    {
                        notata = VERUM;
                        frange;
                    }
                }
            }
            si (!notata)
            {
                _chordam_addere(ignotae, via);
            }
        }
        extra_undae = ignotae;
    }
    fractum = FALSUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
           i32 i;
        chorda aliena;
        chorda causa;

        i = *(i32*)xar_obtinere(indices, k);
        si (   !incepta[k] && acta[k].codex == -I
            && acta[k].duratio_ms    == 0
            && acta[k].cauda.mensura == 0)
        {
            status[i] = SANANDI_OMISSUM;
            _sanationem_notare(sutura, piscina, sanationes, actiones[k],
                FABRICA_OMISSUM,
                chorda_ex_literis("non incepta: fractura in unda (nova "
                    "non incipiuntur)", piscina), ZEPHYRUM);
            fractum = VERUM;
            perge;
        }
        si (!incepta[k])
        {
            _post_agere(sutura, actiones[k], i, status, sanationes,
                FALSUM,
                _iungere(piscina, "non actum: ", acta[k].cauda, ""),
                acta[k].duratio_ms, piscina);
            fractum = VERUM;
            perge;
        }
        si (acta[k].codex != 0)
        {
            character numerus[XXXII];

            sprintf(numerus, "exitus %d: ", (integer)acta[k].codex);
            _post_agere(sutura, actiones[k], i, status, sanationes,
                FALSUM,
                _iungere(piscina, numerus, acta[k].cauda, ""),
                acta[k].duratio_ms, piscina);
            fractum = VERUM;
            perge;
        }
        aliena = _scripturam_alienam(actiones[k], notatae[k], piscina);
        si (aliena.mensura > 0)
        {
            _post_agere(sutura, actiones[k], i, status, sanationes,
                FALSUM,
                _iungere(piscina, "scripsit extra vestigium (S): ",
                aliena,
                ""), acta[k].duratio_ms, piscina);
            fractum = VERUM;
            perge;
        }
        si (extra_undae != NIHIL && xar_numerus(extra_undae) > 0)
        {
            causa = _iungere(piscina, "unda scripsit extra vestigia "
                "omnium membrorum: ",
                *(chorda*)xar_obtinere(extra_undae,
                ZEPHYRUM), "");
            si (xar_numerus(extra_undae) > I)
            {
                causa = _iungere(piscina, "", causa, " +");
                causa = chorda_concatenare(causa,
                    numerus_romanus_exprimere(
                    (i64)(xar_numerus(extra_undae) - I), NIHIL,
                    piscina),
                    piscina);
            }
            _post_agere(sutura, actiones[k], i, status, sanationes,
                FALSUM,
                _iungere(piscina, "", causa,
                " (scriptor ignotus - membra "
                "omnia fracta)"), acta[k].duratio_ms, piscina);
            fractum = VERUM;
            perge;
        }
        _post_agere(sutura, actiones[k], i, status, sanationes, VERUM,
            chorda_ex_literis("", piscina), acta[k].duratio_ms,
            piscina);
        si (status[i] == SANANDI_FRACTUM)
        {
            fractum = VERUM;
        }
    }
    redde fractum;
}

/* SANARE PER UNDAS (plan 2 T6): undae ex actionibus ambitus; in unda
 * non tutae SOLAE (photographia sua), tutae (lectiones="verum") simul.
 * Fractura: nova non incipiuntur (OMISSUM), dependentes OMISSUM. */
interior b32
_sanare_per_undas (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
                       b32* ambitus,
             StatusSanandi* status,
                       Xar* sanationes,
                   Piscina* piscina)
{
     Xar* membra_ambitus;
     Xar* undae;
     i32  i;
     i32  w;
     b32  fractum;

    membra_ambitus = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
    si (membra_ambitus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
        si (ambitus[i] && !_actio_ignota(actio))
        {
            *(FabricaActio**)xar_addere(membra_ambitus) = actio;
        }
    }
    undae = fabrica_undas_formare(membra_ambitus, piscina);
    si (undae == NIHIL)
    {
        redde FALSUM;
    }
    fractum = FALSUM;
    per (w = ZEPHYRUM; w < xar_numerus(undae); w++)
    {
        Xar* membra;
        Xar* tuta;
        i32  m;

        membra  = *(Xar**)xar_obtinere(undae, w);
        tuta    = xar_creare(piscina, (i32)magnitudo(i32));
        si (tuta == NIHIL)
        {
            redde FALSUM;
        }
        per (m = ZEPHYRUM; m < xar_numerus(membra); m++)
        {
            FabricaActio* actio;
                     s32  index;

            actio = *(FabricaActio**)xar_obtinere(membra, m);
            index = _index_actionis(ordo, actio->titulus);
            si (index < 0)
            {
                perge;
            }
            si (fractum)
            {
                chorda fracta;
                chorda causa;

                /* post fracturam nihil novum incipitur - sed iudicatur:
                 * recens tacet, dependentia fracta nominatur, sola
                 * agenda OMISSUM 'post fracturam' */
                causa = chorda_ex_literis("", piscina);
                si (_dependentia_fracta(ordo, (i32)index, status,
                    &fracta))
                {
                    status[index] = SANANDI_OMISSUM;
                    _sanationem_notare(sutura, piscina, sanationes,
                        actio,
                        FABRICA_OMISSUM, _iungere(piscina,
                        "dependentia fracta: ", fracta, ""), ZEPHYRUM);
                }
                alioquin si (_exitus_recentes(sutura, actio, piscina,
                             &causa))
                {
                    status[index] = SANANDI_RECENS;
                }
                alioquin
                {
                    status[index] = SANANDI_OMISSUM;
                    _sanationem_notare(sutura, piscina, sanationes,
                        actio,
                        FABRICA_OMISSUM, chorda_ex_literis("post "
                        "fracturam: nova non incipiuntur", piscina),
                        ZEPHYRUM);
                }
                perge;
            }
            si (!_ante_agere(sutura, ordo, (i32)index, status,
                sanationes,
                    FALSUM, piscina))
            {
                perge;
            }
            si (actio->lectiones)
            {
                *(i32*)xar_addere(tuta) = (i32)index;
            }
            alioquin
            {
                /* non tuta: SOLA, photographia sua (via vetus) */
                   i32 duratio;
                chorda causa;
                   b32 actum;

                duratio  = ZEPHYRUM;
                causa    = chorda_ex_literis("", piscina);
                actum   = _actionem_agere(sutura, actio, piscina,
                    &duratio,
                    &causa);
                _post_agere(sutura, actio, (i32)index, status,
                    sanationes,
                    actum, causa, duratio, piscina);
                si (status[index] == SANANDI_FRACTUM)
                {
                    fractum = VERUM;
                }
            }
        }
        si (fractum || xar_numerus(tuta) == 0)
        {
            per (m = ZEPHYRUM; fractum && m < xar_numerus(tuta); m++)
            {
                i32 index;

                index          = *(i32*)xar_obtinere(tuta, m);
                status[index]  = SANANDI_OMISSUM;
                _sanationem_notare(sutura, piscina, sanationes,
                    *(FabricaActio**)xar_obtinere(ordo, index),
                    FABRICA_OMISSUM,
                    chorda_ex_literis("post fracturam: "
                    "nova non incipiuntur", piscina), ZEPHYRUM);
            }
            perge;
        }
        si (xar_numerus(tuta) == I)
        {
            /* tuta sola: nihil simul - via vetus cum photographia */
               i32 index;
               i32 duratio;
            chorda causa;
               b32 actum;

            index    = *(i32*)xar_obtinere(tuta, ZEPHYRUM);
            duratio  = ZEPHYRUM;
            causa    = chorda_ex_literis("", piscina);
            actum   = _actionem_agere(sutura,
                *(FabricaActio**)xar_obtinere(ordo, index), piscina,
                &duratio, &causa);
            _post_agere(sutura, *(FabricaActio**)xar_obtinere(ordo,
                index),
                index, status, sanationes, actum, causa, duratio,
                piscina);
            si (status[index] == SANANDI_FRACTUM)
            {
                fractum = VERUM;
            }
            perge;
        }
        si (_undam_agere(sutura, ordo, tuta, status, sanationes,
            piscina))
        {
            fractum = VERUM;
        }
    }
    redde VERUM;
}

Xar*
fabrica_sanare (
    constans FabricaSutura* sutura,
              constans Xar* ordo,
              constans Xar* electa,
                       b32  siccum,
                   Piscina* piscina,
                    chorda* causa_out)
{
               Xar* sanationes;
               b32* ambitus;
     StatusSanandi* status;
               i32  numerus;
               i32  i;
               i32  j;
               b32  mutatum;

    numerus     = xar_numerus(ordo);
    sanationes  = xar_creare(piscina, (i32)magnitudo(FabricaSanatio));
    ambitus = (b32*)piscina_allocare(piscina,
        magnitudo(b32) * (memoriae_index)(numerus + 1));
    status = (StatusSanandi*)piscina_allocare(piscina,
        magnitudo(StatusSanandi) * (memoriae_index)(numerus + 1));
    si (sanationes == NIHIL || ambitus == NIHIL || status == NIHIL)
    {
        redde NIHIL;
    }

    /* ambitus: actiones viarum electarum (aut omnes), deinde omnes
     * supra eas (clausura per _pendet) */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        status[i]   = SANANDI_NONDUM;
        ambitus[i]  = (electa == NIHIL);
    }
    per (j = ZEPHYRUM; electa != NIHIL && j < xar_numerus(electa); j++)
    {
        chorda via;
           b32 inventa;

        via      = *(chorda*)xar_obtinere(electa, j);
        inventa  = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            constans FabricaActio* actio;
                              i32  k;

            actio = *(FabricaActio**)xar_obtinere(ordo, i);
            per (k = ZEPHYRUM; k < xar_numerus(actio->exitus); k++)
            {
                si (chorda_aequalis(((FabricaExitus*)xar_obtinere(
                        actio->exitus, k))->via, via))
                {
                    ambitus[i]  = VERUM;
                    inventa     = VERUM;
                }
            }
        }
        si (!inventa)
        {
            _causam_ponere(causa_out, piscina,
                "artificium a nulla actione productum: ", via, "");
            redde NIHIL;
        }
    }
    fac
    {
        mutatum = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (!ambitus[i])
            {
                perge;
            }
            per (j = ZEPHYRUM; j < numerus; j++)
            {
                si (   !ambitus[j]
                    && _pendet(*(FabricaActio**)xar_obtinere(ordo, i),
                           *(FabricaActio**)xar_obtinere(ordo, j)))
                {
                    ambitus[j]  = VERUM;
                    mutatum     = VERUM;
                }
            }
        }
    }
    dum (mutatum);

    si (sutura->agere_simul != NIHIL && !siccum)
    {
        si (!_sanare_per_undas(sutura, ordo, ambitus, status,
            sanationes,
                piscina))
        {
            redde NIHIL;
        }
        redde sanationes;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans FabricaActio* actio;
                       chorda  causa;
                          i32  duratio;
                          b32  actum;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
        si (!ambitus[i] || _actio_ignota(actio))
        {
            perge;   /* ignota: praecondicione sola realizatur */
        }
        si (!_ante_agere(sutura, ordo, i, status, sanationes, siccum,
                piscina))
        {
            perge;
        }
        duratio  = ZEPHYRUM;
        causa    = chorda_ex_literis("", piscina);
        actum = _actionem_agere(sutura, actio, piscina, &duratio,
            &causa);
        _post_agere(sutura, actio, i, status, sanationes, actum, causa,
            duratio, piscina);
    }
    redde sanationes;
}
