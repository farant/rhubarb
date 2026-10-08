/* fabrica_genera.c - genera ingressuum fabricae: sigillum copiae
 * ingressuum (particulae, manifesta, directoria, plagulae, radices),
 * verba sigillare/enumerare/locare per genus, instrumentum domus,
 * identitas clang, effectus, repositorium, axes duo (res, clavis),
 * registrum generum, sigillum ingressuum et actionis, actio tacta.
 * Pars bibliothecae fabrica (fabrica-6 H2; vide fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Sigillum copiae ingressuum
 * ================================================== */

s32
fabricae_particulas_comparare (
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
            fabricae_causam_ponere(causa_out, piscina,
                "ingressus absens: ",
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
        fabricae_causam_ponere(causa_out, piscina,
            "manifestum absens: ", via,
            "");
        redde FALSUM;
    }
    si (!fabrica_manifestum_legere(contentum, piscina, &viae,
            &inresolutae, causa_out))
    {
        fabricae_causam_ponere(causa_out, piscina,
            "manifestum malformatum: ",
            via, "");
        redde FALSUM;
    }
    /* Q9: incompletum numquam sigillatur - nominatur */
    si (xar_numerus(inresolutae) > 0)
    {
        fabricae_causam_ponere(causa_out, piscina,
            "manifestum incompletum: ",
            fabricae_iungere(piscina, "", via,
            ": inclusio citata inresoluta "),
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
    clavis = fabricae_iungere(piscina, "", via, "/");
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
        fabricae_causam_ponere(causa_out, piscina,
            "directorium absens: ", via,
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
        fabricae_causam_ponere(causa_out, piscina,
            "directorium absens: ",
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
        via = fabricae_iungere(piscina, "", fabricae_iungere(piscina,
            "",
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
        si (!_manifestum_explicare(sutura, fabricae_iungere(piscina, "",
                fabricae_iungere(piscina, "", via, "/"),
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
        fabricae_causam_ponere(causa_out, piscina,
            "configuratio absens: ",
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
        fabricae_causam_ponere(causa_out, piscina,
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

vacuum
fabricae_locum_addere (
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
vacuum
fabricae_loci_ex_particulis (
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
            fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULAE,
                chorda_sectio(via, 0, via.mensura - I), vacua);
        }
        alioquin
        {
            fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA, via,
                vacua);
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
    fabricae_loci_ex_particulis(particulae, piscina, loci);
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULAE, ingressus->via,
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
    fabricae_locum_addere(loci, FABRICA_LOCUS_PLAGULA, exitus->via,
        chorda_ex_literis("", piscina));
    redde VERUM;
}

/* ordo membrorum: titulus, sigillare, enumerare, locare,
 * reproducibile */
interior constans FabricaGenus _genus_fasciculus = {
    "fasciculus", _fasciculum_sigillare, NIHIL, _plagulam_locare,
    VERUM,
    VERUM, FABRICA_SUMPTUS_VILIS
};

interior constans FabricaGenus _genus_configuratio = {
    "configuratio", _fasciculum_sigillare, NIHIL, NIHIL, VERUM,
    VERUM, FABRICA_SUMPTUS_VILIS
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
    FALSUM,
    VERUM, FABRICA_SUMPTUS_VILIS
};

/* binarium: octeti plagulae ut ingressus; ut exitus relatione sola
 * iudicatur (LC_UUID, signatura) */
interior constans FabricaGenus _genus_binarium = {
    "binarium", _fasciculum_sigillare, NIHIL, _plagulam_locare, FALSUM,
    VERUM, FABRICA_SUMPTUS_VILIS
};

interior constans FabricaGenus _genus_directorium = {
    "directorium", _directorium_sigillare, NIHIL, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
};

interior constans FabricaGenus _genus_manifestum = {
    "manifestum", _manifestum_sigillare, NIHIL, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
};

interior constans FabricaGenus _genus_plagulae = {
    "plagulae", _plagulas_explicare, _plagulas_enumerare, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
};

interior constans FabricaGenus _genus_manifesta = {
    "manifesta", _manifesta_sigillare, NIHIL, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
};

/* radices SIGILLANTUR (obumbratio: caput novum in radice priore), non
 * ENUMERANTUR: ordo ex manifestis (plagulae vere lectae). Arcus ad
 * omnem producentem in radice cyclum faceret ubi actio radicem legit
 * in quam actio posterior scribit (parcum …AR15: fontes_silva
 * sigillat silva/amalgama, amalgama_silva ibi scribit) - ut
 * instrumentum (T5) */
interior constans FabricaGenus _genus_radices = {
    "radices", _radices_sigillare, _nihil_enumerare, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
};

/* INSTRUMENTUM DOMUS (spec 3 par. XII v4): binarium domus per lineam
 * 'ingressus <sigillum>' relationis '-provenientia' sigillatur -
 * sigillum FONTIUM ex quibus structum est. Octeti eius relinkatione
 * mutantur (LC_UUID) etsi nihil mutatum: portae binaria domus omni
 * commissione restruunt, ergo octeti transitum post omnem commissionem
 * delerent (inventum T5c: bin/aedilis a porta restructum). Linea
 * 'commissum' (omni commissione mutata) NON sigillatur. Sine relatione:
 * octeti (cautum). */
interior b32
_provenientia_legere (
     constans FabricaSutura* sutura,
                     chorda  via,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out)
{
              chorda  relatio;
              chorda  linea;
                 s32  inventum;
                 i32  finis;
    FabricaParticula* particula;

    si (   sutura->rogare == NIHIL
        || !sutura->rogare(sutura->datum, chorda_ut_cstr(via, piscina),
               piscina, &relatio))
    {
        redde _particulam_legere(sutura, via, exclusa, piscina,
            particulae,
            causa_out);
    }
    inventum = chorda_invenire_index(relatio, chorda_ex_literis(
        "ingressus ", piscina));
    si (inventum < ZEPHYRUM)
    {
        redde _particulam_legere(sutura, via, exclusa, piscina,
            particulae,
            causa_out);
    }
    finis = (i32)inventum;
    dum (finis < relatio.mensura && relatio.datum[finis] != '\n')
    {
        finis++;
    }
    linea      = chorda_sectio(relatio, (i32)inventum, finis);
    particula  = (FabricaParticula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via     = via;
    particula->octeti  = sigillum_computare(linea.datum,
        (memoriae_index)linea.mensura);
    redde VERUM;
}

interior b32
_instrumentum_domus_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    redde _provenientia_legere(sutura, ingressus->via, exclusa,
        piscina,
        particulae, causa_out);
}

/* IDENTITAS CLANG (spec 3 par. XII, IX.7): identitas compilatoris quam
 * bin/compilator adhibet - radices systematis (SDK) eam sequuntur */
interior b32
_identitatem_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    FabricaParticula* particula;
            Sigillum  identitas;

    (vacuum)ingressus;
    (vacuum)exclusa;
    si (sutura->identitas == NIHIL)
    {
        *causa_out = chorda_ex_literis("sutura sine identitate clang",
            piscina);
        redde FALSUM;
    }
    si (!sutura->identitas(sutura->datum, piscina, &identitas,
        causa_out))
    {
        redde FALSUM;
    }
    particula = (FabricaParticula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via     = chorda_ex_literis("identitas:clang", piscina);
    particula->octeti  = identitas;
    redde VERUM;
}

interior constans FabricaGenus _genus_identitas_clang = {
    "identitas_clang", _identitatem_sigillare, _nihil_enumerare, NIHIL,
    FALSUM,
    VERUM, FABRICA_SUMPTUS_CARUS
};

interior constans FabricaGenus _genus_instrumentum_domus = {
    "instrumentum_domus", _instrumentum_domus_sigillare,
        _nihil_enumerare,
    NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_CARUS
};

/* congruentia globi ('*' '?' '[...]' intra segmentum - bash) */
b32
fabricae_globus_congruit (
    constans character* exemplar,
    constans character* textus)
{
    dum (*exemplar != '\0')
    {
        si (*exemplar == '*')
        {
            exemplar++;
            per (;;)
            {
                si (fabricae_globus_congruit(exemplar, textus))
                {
                    redde VERUM;
                }
                si (*textus == '\0' || *textus == '/')
                {
                    redde FALSUM;
                }
                textus++;
            }
        }
        si (*textus == '\0')
        {
            redde FALSUM;
        }
        si (*exemplar == '[')
        {
            constans character* f = strchr(exemplar, ']');
            constans character* k;
                           b32  inventum = FALSUM;

            si (f == NIHIL)
            {
                redde FALSUM;
            }
            per (k = exemplar + I; k < f; k++)
            {
                si (*k == *textus)
                {
                    inventum = VERUM;
                }
            }
            si (!inventum)
            {
                redde FALSUM;
            }
            exemplar = f;
        }
        alioquin si (*exemplar != '?' && *exemplar != *textus)
        {
            redde FALSUM;
        }
        exemplar++;
        textus++;
    }
    redde *textus == '\0';
}

/* particula ficta: 'genus:via' cum sigillo valoris dati */
interior b32
_particulam_valoris (
               Piscina* piscina,
                   Xar* particulae,
    constans character* genus,
                chorda  via,
                chorda  valor)
{
    FabricaParticula* particula = (FabricaParticula*)xar_addere(
        particulae);

    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via     = fabricae_iungere(piscina, genus, via, "");
    particula->octeti  = sigillum_computare(valor.datum,
        (memoriae_index)valor.mensura);
    redde VERUM;
}

/* ARBOR (effectus-plan-2 T6): subarbor tota - nomina cuiusque
 * directorii (additio, remotio) et octeti cuiusque plagulae (mutatio
 * contenti), recursive. 'directorium' nomina SOLA sigillat (radices
 * inclusionum); lectio per praefixum ('grep -r', 'cat "dir/$x"')
 * contenta legit - ante T6 clavis eam per 'directorium' tegebat et
 * mutationem contenti non videbat. Absens: particula 'absens' (creatio
 * clavem mutat). Directorium = via quam enumerare accipit. */
interior b32
_arborem_explicare (
     constans FabricaSutura* sutura,
                     chorda  via,
               constans Xar* exclusa,
                    Piscina* piscina,
                        Xar* particulae,
                     chorda* causa_out,
                        i32  profunditas)
{
                  Xar* nomina;
               chorda  basis;
    ChordaAedificator* aedificator;
                  i32  i;

    si (profunditas > XVI)
    {
        fabricae_causam_ponere(causa_out, piscina,
            "arbor nimis profunda: ",
            via,
            "");
        redde FALSUM;
    }
    basis = via.mensura > ZEPHYRUM && via.datum[via.mensura - I] == '/'
        ? via : fabricae_iungere(piscina, "", via, "/");
    si (   sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum, chorda_ut_cstr(via,
               piscina), piscina, &nomina))
    {
        redde _particulam_valoris(piscina, particulae, "arbor:", basis,
            chorda_ex_literis("absens", piscina));
    }
    aedificator = chorda_aedificator_creare(piscina, 256);
    si (aedificator == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            *(chorda*)xar_obtinere(nomina, i));
        (vacuum)chorda_aedificator_appendere_character(aedificator,
            '\n');
    }
    si (!_particulam_valoris(piscina, particulae, "arbor:", basis,
            chorda_aedificator_finire(aedificator)))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        chorda plena = fabricae_iungere(piscina, chorda_ut_cstr(basis,
            piscina),
                     *(chorda*)xar_obtinere(nomina, i), "");
           Xar* sub;

        si (sutura->enumerare(sutura->datum, chorda_ut_cstr(plena,
                piscina), piscina, &sub))
        {
            si (!_arborem_explicare(sutura, plena, exclusa, piscina,
                    particulae, causa_out, profunditas + I))
            {
                redde FALSUM;
            }
        }
        alioquin si (!_particulam_legere(sutura, plena, exclusa,
                         piscina, particulae, causa_out))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* nomina directorii 'dir' exemplari congruentia, ordinata, iuncta */
interior b32
_nomina_colligere (
     constans FabricaSutura*  sutura,
                     chorda   directorium,
         constans character*  exemplar,
                    Piscina*  piscina,
                     chorda*  nomina_out,
                        Xar** congruentia_out)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CCLVI);
                  Xar* nomina = NIHIL;
                  Xar* congruentia = xar_creare(piscina,
                           (i32)magnitudo(chorda));
                  i32 k;

    si (   a                 == NIHIL || congruentia == NIHIL
        || sutura->enumerare == NIHIL
        || !sutura->enumerare(sutura->datum,
               chorda_ut_cstr(directorium, piscina), piscina, &nomina))
    {
        nomina = NIHIL;   /* directorium absens: nomina nulla */
    }
    per (k = ZEPHYRUM; nomina != NIHIL && k < xar_numerus(nomina); k++)
    {
        chorda n = *(chorda*)xar_obtinere(nomina, k);

        si (fabricae_globus_congruit(exemplar, chorda_ut_cstr(n,
            piscina)))
        {
            chorda_aedificator_appendere_chorda(a, n);
            chorda_aedificator_appendere_character(a, '\n');
            *(chorda*)xar_addere(congruentia) = n;
        }
    }
    *nomina_out = chorda_aedificator_finire(a);
    si (congruentia_out != NIHIL)
    {
        *congruentia_out = congruentia;
    }
    redde VERUM;
}

/* EFFECTUS (effectus-plan T7, spec par. VII): lineae '-clavis' scripti
 * in particulas - octeti (absentia quoque), provenientia, probatio
 * (species), nomina, globus, directorium, ambitus (variabiles fabricae
 * FABRICA_* propriae omittuntur: protocollum, non ingressus), dominus
 * (exitus declaratus aut IGNOTUM), ignotum (IGNOTUM, sedes
 * nominata). */
interior b32
_effectus_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
    chorda effusio;
       i32 codex;
       i32 i;
       i32 initium = ZEPHYRUM;

    si (sutura->effectus == NIHIL)
    {
        *causa_out = chorda_ex_literis("sutura sine effectibus",
            piscina);
        redde FALSUM;
    }
    si (!sutura->effectus(sutura->datum,
            chorda_ut_cstr(ingressus->via, piscina), piscina, &effusio,
            &codex))
    {
        *causa_out = fabricae_iungere(piscina,
            "effectus currere nequit: ",
            ingressus->via, "");
        redde FALSUM;
    }
    si (codex != ZEPHYRUM)
    {
        character numerus[32];

        sprintf(numerus, "%d", (integer)codex);
        *causa_out = fabricae_iungere(piscina,
            "effectus fracti (exitus ",
            chorda_ex_literis(numerus, piscina), ")");
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < effusio.mensura; i++)
    {
        chorda linea;
        chorda genus;
        chorda via;
        chorda extra;
           s32 t;
           s32 u;

        si (effusio.datum[i] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(effusio, initium, i);
        initium  = i + I;
        t = chorda_invenire_index(linea, chorda_ex_literis("\t",
            piscina));
        si (t < ZEPHYRUM)
        {
            perge;
        }
        genus  = chorda_sectio(linea, 0, (i32)t);
        via    = chorda_sectio(linea, (i32)t + I, linea.mensura);
        extra  = chorda_ex_literis("", piscina);
        u = chorda_invenire_index(via, chorda_ex_literis("\t",
            piscina));
        si (u >= ZEPHYRUM)
        {
            extra  = chorda_sectio(via, (i32)u + I, via.mensura);
            via    = chorda_sectio(via, 0, (i32)u);
        }
        si (chorda_aequalis_literis(genus, "ignotum"))
        {
            *causa_out = fabricae_iungere(piscina, "effectus ignotus: ",
                linea,
                "");
            redde FALSUM;
        }
        si (   chorda_aequalis_literis(genus, "octeti")
            || chorda_aequalis_literis(genus, "dominus"))
        {
             vacuum* dominus = NIHIL;
                i32  species = sutura->species != NIHIL
                    ? sutura->species(sutura->datum,
                      chorda_ut_cstr(via, piscina))
                    : (i32)FABRICA_SPECIES_PLAGULA;

            (vacuum)dominus;

            si (   chorda_aequalis_literis(genus, "dominus")
                && (sutura->exitus_noti == NIHIL
                    || !tabula_dispersa_invenire(sutura->exitus_noti,
                           via, &dominus)))
            {
                *causa_out = fabricae_iungere(piscina,
                    "ingressus build/ sine domino: ", via, "");
                redde FALSUM;
            }
            si (species == (i32)FABRICA_SPECIES_ALIA)
            {
                *causa_out = fabricae_iungere(piscina,
                    "effectus: via non sigillabilis: ", via, "");
                redde FALSUM;
            }
            si (species == (i32)FABRICA_SPECIES_ABSENS)
            {
                si (!_particulam_valoris(piscina, particulae, "absens:",
                        via, chorda_ex_literis("absens", piscina)))
                {
                    redde FALSUM;
                }
            }
            alioquin si (!_particulam_legere(sutura, via, exclusa,
                         piscina,
                         particulae, causa_out))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "provenientia"))
        {
            si (!_provenientia_legere(sutura, via, exclusa, piscina,
                    particulae, causa_out))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "probatio"))
        {
            character species[XVI];

            sprintf(species, "%d", (integer)(sutura->species != NIHIL
                ? sutura->species(sutura->datum,
                      chorda_ut_cstr(via, piscina))
                : (i32)FABRICA_SPECIES_PLAGULA));
            si (!_particulam_valoris(piscina, particulae, "probatio:",
                    via, chorda_ex_literis(species, piscina)))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "nomina"))
        {
            chorda nomina;

            si (   !_nomina_colligere(sutura, via,
                       chorda_ut_cstr(extra, piscina), piscina, &nomina,
                       NIHIL)
                || !_particulam_valoris(piscina, particulae, "nomina:",
                       fabricae_iungere(piscina, "", via,
                           chorda_ut_cstr(extra, piscina)), nomina))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "globus"))
        {
             constans character* e   = chorda_ut_cstr(via, piscina);
             constans character* sl  = strrchr(e, '/');
                         chorda  dir = sl != NIHIL
                             ? chorda_sectio(via, 0, (i32)(sl - e))
                             : chorda_ex_literis(".", piscina);
                         chorda  nomina;
                            Xar* congruentia;
                            i32  k;

            si (   !_nomina_colligere(sutura, dir, sl != NIHIL ? sl
                + I : e,
                       piscina, &nomina, &congruentia)
                || !_particulam_valoris(piscina, particulae, "globus:",
                       via, nomina))
            {
                redde FALSUM;
            }
            per (k = ZEPHYRUM; k < xar_numerus(congruentia); k++)
            {
                chorda plena = sl != NIHIL
                    ? fabricae_iungere(piscina, "", dir, "/")
                    : chorda_ex_literis("", piscina);

                plena = fabricae_iungere(piscina, chorda_ut_cstr(plena,
                    piscina),
                    *(chorda*)xar_obtinere(congruentia, k), "");
                si (!_particulam_legere(sutura, plena, exclusa, piscina,
                        particulae, causa_out))
                {
                    redde FALSUM;
                }
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "directorium"))
        {
            si (!_directorium_explicare(sutura, via, piscina,
                particulae,
                    causa_out))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "arbor"))
        {
            si (!_arborem_explicare(sutura, via, exclusa, piscina,
                    particulae, causa_out, ZEPHYRUM))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(genus, "ambitus"))
        {
            chorda valor;

            si (   via.mensura                         >= VIII
                && memcmp(via.datum, "FABRICA_", VIII) == ZEPHYRUM)
            {
                perge;   /* protocollum fabricae ipsius */
            }
            si (   sutura->ambitus == NIHIL
                || !sutura->ambitus(sutura->datum,
                       chorda_ut_cstr(via, piscina), piscina, &valor))
            {
                valor = chorda_ex_literis("\001absens", piscina);
            }
            si (!_particulam_valoris(piscina, particulae, "ambitus:",
                via,
                    valor))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            *causa_out = fabricae_iungere(piscina,
                "effectus: linea ignota: ",
                linea, "");
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior constans FabricaGenus _genus_effectus = {
    "effectus", _effectus_sigillare, _nihil_enumerare, NIHIL, FALSUM,
    VERUM, FABRICA_SUMPTUS_CARUS
};


/* REPOSITORIUM (fabrica-6 T2): status repositorii clave data - hodie
 * "commissum" (sha HEAD) solum. Particula 'repositorium:commissum'.
 * Loci nulli: commissio via non est (quaestionibus inversis invisibile,
 * consulto - census id nominat). */
interior b32
_repositorium_sigillare (
       constans FabricaSutura* sutura,
    constans FabricaIngressus* ingressus,
                 constans Xar* exclusa,
                      Piscina* piscina,
                          Xar* particulae,
                       chorda* causa_out)
{
     FabricaParticula* particula;
               chorda  valor;

    (vacuum)ingressus;
    (vacuum)exclusa;
    si (sutura->repositorium == NIHIL)
    {
        *causa_out = chorda_ex_literis("sutura sine repositorio",
            piscina);
        redde FALSUM;
    }
    si (!sutura->repositorium(sutura->datum, "commissum", piscina,
            &valor))
    {
        *causa_out =
            chorda_ex_literis("repositorium: commissum ignotum",
            piscina);
        redde FALSUM;
    }
    particula = (FabricaParticula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via     = chorda_ex_literis("repositorium:commissum",
        piscina);
    particula->octeti  = sigillum_computare(valor.datum,
        (memoriae_index)valor.mensura);
    redde VERUM;
}

interior constans FabricaGenus _genus_repositorium = {
    "repositorium", _repositorium_sigillare, _nihil_enumerare, NIHIL,
    FALSUM,
    VERUM, FABRICA_SUMPTUS_MEDIUS
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
    &_genus_radices,
    &_genus_identitas_clang,

    &_genus_instrumentum_domus,
    &_genus_effectus,
    &_genus_repositorium
};

/* AXES DUO (fabrica-6 T2): (res, clavis) -> genus. Genera sine pari
 * (configuratio, binarium, radices) alias soli manent. */
interior constans character* constans _paria[][III] = {
    { "plagula", "contentum", "fasciculus" },
    { "plagulae", "contentum", "plagulae" },
    { "directorium", "nomina", "directorium" },
    { "plagula", "clausura", "manifestum" },
    { "directorium", "clausura", "manifesta" },
    { "instrumentum", "contentum", "instrumentum" },
    { "instrumentum", "provenientia", "instrumentum_domus" },
    { "instrumentum", "identitas", "identitas_clang" },
    { "plagula", "effectus", "effectus" },
    { "repositorium", "commissum", "repositorium" }
};

constans FabricaGenus*
fabrica_genus_ex_pari (
    chorda res,
    chorda clavis)
{
    i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_paria) / magnitudo(_paria[0])); i++)
    {
        si (   chorda_aequalis_literis(res, _paria[i][0])
            && chorda_aequalis_literis(clavis, _paria[i][1]))
        {
            i32 g;

            per (g = ZEPHYRUM; g < fabrica_genera_numerus(); g++)
            {
                si (strcmp(_genera[g]->titulus, _paria[i][2])
                    == ZEPHYRUM)
                {
                    redde _genera[g];
                }
            }
            redde NIHIL;
        }
    }
    redde NIHIL;
}

constans character*
fabrica_genus_par (
    constans FabricaGenus* genus)
{
    hic_manens character par[LXIV];
                     i32 i;

    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(_paria) / magnitudo(_paria[0])); i++)
    {
        si (strcmp(genus->titulus, _paria[i][2]) == ZEPHYRUM)
        {
            sprintf(par, "%s/%s", _paria[i][0], _paria[i][1]);
            redde par;
        }
    }
    redde NIHIL;
}

i32
fabrica_genera_numerus (vacuum)
{
    redde (i32)(magnitudo(_genera) / magnitudo(_genera[0]));
}

constans character*
fabrica_genus_loci (
    constans FabricaGenus* genus)
{
    si (genus->enumerare == NIHIL)
    {
        redde "ex particulis";
    }
    si (genus->enumerare == _nihil_enumerare)
    {
        redde "nulli";
    }
    redde "proprii";
}

constans FabricaGenus*
fabrica_genus_obtinere (
    i32 index)
{
    si (index < ZEPHYRUM || index >= fabrica_genera_numerus())
    {
        redde NIHIL;
    }
    redde _genera[index];
}

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
b32
fabricae_particulas_colligere (
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
        || !fabricae_particulas_colligere(sutura, actio, exclusa,
        piscina,
               particulae, causa_out))
    {
        redde FALSUM;
    }

    xar_ordinare(particulae, fabricae_particulas_comparare);

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
        fabricae_loci_ex_particulis(particulae, piscina, loci);
    }
    redde VERUM;
}

/* via -> directorium (sine '/' finali; vacuum in radice) et titulus */
vacuum
fabricae_viam_dividere (
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
b32
fabricae_locus_tangit (
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
            redde chorda_incipit(via, fabricae_iungere(piscina, "",
                locus->via,
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
        fabricae_viam_dividere(via, &directorium, &titulus);
        per (j = ZEPHYRUM; j < xar_numerus(loci); j++)
        {
            si (fabricae_locus_tangit((FabricaLocus*)xar_obtinere(loci,
                j), via,
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
    redde fabricae_iungere(piscina, "build/fabrica/provenientia/",
        titulus,
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

    exclusa = fabricae_xar_chordarum(piscina);
    si (exclusa == NIHIL)
    {
        redde FALSUM;
    }
    /* directorium provenientiae TOTUM: involucri exitus, numquam
     * ingressus - actio familiae (canon, natura: binaria plura)
     * plagulam per binarium habet, non per titulum (1b T5, D2) */
    fabricae_chordam_addere(exclusa, chorda_ex_literis(
        "build/fabrica/provenientia/", piscina));
    redde fabrica_ingressus_sigillare(sutura, actio, exclusa, piscina,
        sigillum_out, causa_out);
}
