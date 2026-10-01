/* fabrica.c - nucleus iudicis fabricae (T1)
 *
 * Machina pura: discum et processus per FabricaSutura solam tangit.
 * Vide include/fabrica.h et project-specs/fabrica-spec-v2.md. */

#include "fabrica.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "stml.h"
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
    sutura->datum           = NIHIL;
    sutura->legere          = NIHIL;
    sutura->enumerare       = NIHIL;
    sutura->currere         = NIHIL;
    sutura->rogare          = NIHIL;
    sutura->meminisse       = NIHIL;
    sutura->inscribere      = NIHIL;
    sutura->sigilla         = NIHIL;
    sutura->regenerationes  = NIHIL;
    sutura->digesta         = NIHIL;
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
        si (chorda_aequalis(*(chorda*)xar_obtinere(exclusa, i), via))
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

interior constans FabricaGenus _genus_instrumentum = {
    "instrumentum", _fasciculum_sigillare, NIHIL, NIHIL, FALSUM
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

interior constans FabricaGenus _genus_radices = {
    "radices", _radices_sigillare, NIHIL, NIHIL, FALSUM
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
           s32 k;

        via  = *(chorda*)xar_obtinere(viae, i);
        k    = (s32)via.mensura - I;
        dum (k >= 0 && via.datum[k] != (i8)'/')
        {
            k--;
        }
        directorium  = chorda_sectio(via, 0, (k > 0) ? (i32)k : 0);
        titulus      = chorda_sectio(via, (i32)(k + I), via.mensura);
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
    _chordam_addere(exclusa,
        fabrica_provenientia_via(actio->titulus, piscina));
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

/* Generatorem actionis currere SEMEL per cursum (exitus multi,
 * generator unus): exitus cursus - felix aut causa fracti - in
 * sutura->regenerationes memoratur; exitus sequentes scripturam
 * eandem legunt. */
interior b32
_regenerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                    chorda  scriptura_dir,
                   Piscina* piscina,
                    chorda* causa_out)
{
    vacuum* memoratum;
    chorda* locus;
       b32  felix;

    si (   sutura->regenerationes != NIHIL
        && tabula_dispersa_invenire(sutura->regenerationes,
               actio->titulus, &memoratum))
    {
        *causa_out = *(chorda*)memoratum;
        redde causa_out->mensura == 0;
    }
    causa_out->datum    = NIHIL;
    causa_out->mensura  = ZEPHYRUM;
    felix = sutura->currere(sutura->datum, actio->mandatum,
        chorda_ut_cstr(scriptura_dir, piscina), piscina, causa_out);
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
     Sigillum clavis;
     Sigillum artificium;
       chorda causa;
       chorda commissum;
       chorda effusio;
       chorda scriptura_dir;
       chorda scriptura_via;
       chorda generator;
    character numerus[32];

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
    clavis = _clavem_memoriae(actio, ingressus);
    /* memoria SOLUM pro actione memorabili: ingressus aliter non
     * probabiliter pleni, et verificatio vetus mutationem ingressus
     * non declarati celaret */
    si (   actio->memorabilis
        && sutura->meminisse != NIHIL
        && sutura->meminisse(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina), &clavis,
               &artificium))
    {
        redde _iudicium(exitus->via, FABRICA_RECENS,
            chorda_ex_literis("memoria", piscina));
    }
    si (!plenus || sutura->currere == NIHIL)
    {
        redde _iudicium(exitus->via, FABRICA_NON_IUDICATUM,
            chorda_ex_literis("celer: regeneratio omissa", piscina));
    }

    generator = (xar_numerus(actio->mandatum) > 0)
        ? *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM)
        : actio->titulus;
    scriptura_dir = _iungere(piscina, "build/fabrica/scriptura/",
        actio->titulus, "");
    si (!_regenerare(sutura, actio, scriptura_dir, piscina, &causa))
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
        si (actio->memorabilis && sutura->inscribere != NIHIL)
        {
            sutura->inscribere(sutura->datum,
                chorda_ut_cstr(actio->titulus, piscina), &clavis,
                &artificium);
        }
        redde _iudicium(exitus->via, FABRICA_RECENS,
            chorda_ex_literis("regeneratio congruit", piscina));
    }
    sprintf(numerus, "%u",
        (insignatus integer)_lineas_differentes_numerare(commissum,
            effusio));
    redde _iudicium(exitus->via, FABRICA_STALUM,
        _iungere(piscina, "regeneratio differt (lineae differentes: ",
            chorda_ex_literis(numerus, piscina), ")"));
}


/* ordo membrorum: titulus, genus_ordinarium, octetis_comparat,
 * iudicare */
interior constans FabricaStrategia _strategia_regeneratio = {
    "regeneratio", "fasciculus", VERUM, _regeneratione_iudicare
};

interior constans FabricaStrategia _strategia_relatio = {
    "relatio", "binarium", FALSUM, _relatione_iudicare
};

interior constans FabricaStrategia* constans _strategiae[] = {
    &_strategia_regeneratio,
    &_strategia_relatio
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

Xar*
fabrica_declarationes_legere (
                 chorda  contentum,
     constans character* via,
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
 * Ordo
 * ================================================== */

/* VERUM si posterior exitum prioris legit */
interior b32
_pendet (
    constans FabricaActio* posterior,
    constans FabricaActio* prior)
{
    i32 i;
    i32 j;

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
