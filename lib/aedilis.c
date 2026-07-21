/* aedilis.c - machina clausurae dependentiarum (Phasis A)
 *
 * Machina PURA: extractio directivarum per suturam AedilisExtractor
 * inicitur - haec plagula silvam numquam tangit, ergo probationes
 * eam in ansa generica cum extractore fixturarum agunt (fixturae
 * disci verae sub probationes/fixa/aedilis/, probae existentiae
 * verae). Silva et cursus -MM in bin/aedilis vivunt.
 *
 * Vide include/aedilis.h pro contractu; spec aedilis-spec-v2.md.
 */

#include "latina.h"
#include "aedilis.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "xar.h"
#include "via.h"
#include "filum.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "stml.h"

#include <stdio.h>
#include <time.h>

/* ====================================================
 * Auxilia parva
 * ==================================================== */

interior vacuum
_chordam_addere (Xar* xar, chorda valor)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = valor;
    }
}

interior Xar*
_xar_chordarum (Piscina* piscina)
{
    redde xar_creare(piscina, (i32)magnitudo(chorda));
}

interior chorda
_iungere_binas (chorda a, chorda b, Piscina* piscina)
{
    chorda partes[2];

    partes[0] = a;
    partes[1] = b;
    redde via_iungere(partes, 2, piscina);
}

interior vacuum
_causam_ponere (chorda* causa_out, Piscina* piscina,
    constans character* praefixum, chorda res)
{
    ChordaAedificator* aedificator;

    si (causa_out == NIHIL)
    {
        redde;
    }
    aedificator = chorda_aedificator_creare(piscina, 256);
    si (aedificator == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(aedificator, praefixum);
    chorda_aedificator_appendere_chorda(aedificator, res);
    *causa_out = chorda_aedificator_finire(aedificator);
}

interior b32
_existit_sub_radice (constans AedilisConfiguratio* configuratio,
    chorda via, Piscina* piscina)
{
    redde via_existit(
        _iungere_binas(configuratio->radix, via, piscina));
}

/* Verbum primum (ad spatium) scindere; reliquum tersum reddere */
interior chorda
_verbum_primum (chorda s, chorda* reliquum_out)
{
    chorda verbum;
    i32 i;

    i = 0;
    dum (i < s.mensura && s.datum[i] != (i8)' ')
    {
        i++;
    }
    verbum.datum = s.datum;
    verbum.mensura = i;
    dum (i < s.mensura && s.datum[i] == (i8)' ')
    {
        i++;
    }
    si (reliquum_out != NIHIL)
    {
        reliquum_out->datum = s.datum + i;
        reliquum_out->mensura = s.mensura - i;
    }
    redde verbum;
}

interior vacuum
_verba_scindere (chorda s, Xar* fructus)
{
    i32 i;

    i = 0;
    dum (i < s.mensura)
    {
        i32 initium;

        dum (i < s.mensura && s.datum[i] == (i8)' ')
        {
            i++;
        }
        initium = i;
        dum (i < s.mensura && s.datum[i] != (i8)' ')
        {
            i++;
        }
        si (i > initium)
        {
            chorda verbum;

            verbum.datum = s.datum + initium;
            verbum.mensura = i - initium;
            _chordam_addere(fructus, verbum);
        }
    }
}

/* ====================================================
 * Configuratio (aedilis.stml)
 * ==================================================== */

interior Xar*
_vexilla_colligere (StmlNodus* nodus, Piscina* piscina)
{
    Xar* fructus;
    Xar* liberi;
    i32  i;
    i32  numerus;

    fructus = _xar_chordarum(piscina);
    si (nodus == NIHIL)
    {
        redde fructus;
    }
    liberi = stml_invenire_omnes_liberos(nodus, "vexillum",
        piscina);
    numerus = xar_numerus(liberi);
    per (i = 0; i < numerus; i++)
    {
        StmlNodus* liber;
        chorda     textus;

        liber = *(StmlNodus**)xar_obtinere(liberi, i);
        textus = stml_textus_internus(liber, piscina);
        si (textus.mensura > 0)
        {
            _chordam_addere(fructus, textus);
        }
    }
    redde fructus;
}

AedilisConfiguratio*
aedilis_configurationem_legere (Piscina* piscina,
    constans character* via_stml, chorda* causa_out)
{
    AedilisConfiguratio* configuratio;
    InternamentumChorda* intern;
    StmlResultus         lectum;
    StmlNodus*           radix_nodus;
    StmlNodus*           sectio;
    chorda               textus;
    i32                  i;
    i32                  numerus;

    textus = filum_legere_totum(via_stml, piscina);
    si (textus.mensura == 0)
    {
        _causam_ponere(causa_out, piscina,
            "configuratio non lecta: ",
            chorda_ex_literis(via_stml, piscina));
        redde NIHIL;
    }
    intern = internamentum_creare(piscina);
    lectum = stml_legere(textus, piscina, intern);
    si (!lectum.successus || lectum.elementum_radix == NIHIL)
    {
        _causam_ponere(causa_out, piscina,
            "configuratio malformata: ",
            chorda_ex_literis(via_stml, piscina));
        redde NIHIL;
    }
    radix_nodus = lectum.elementum_radix;

    configuratio = (AedilisConfiguratio*)piscina_allocare(piscina,
        magnitudo(AedilisConfiguratio));
    si (configuratio == NIHIL)
    {
        redde NIHIL;
    }
    configuratio->radix = via_directorium(
        chorda_ex_literis(via_stml, piscina), piscina);
    configuratio->inclusa = _xar_chordarum(piscina);
    configuratio->vexilla = _xar_chordarum(piscina);
    configuratio->variantes = _xar_chordarum(piscina);
    configuratio->regulae_nexus = xar_creare(piscina,
        (i32)magnitudo(AedilisRegulaNexus));
    configuratio->regulae_vendor = xar_creare(piscina,
        (i32)magnitudo(AedilisRegulaVendor));
    configuratio->irregularia = xar_creare(piscina,
        (i32)magnitudo(AedilisIrregulare));

    sectio = stml_invenire_liberum(radix_nodus, "inclusa");
    si (sectio != NIHIL)
    {
        Xar* viae;

        viae = stml_invenire_omnes_liberos(sectio, "via", piscina);
        numerus = xar_numerus(viae);
        per (i = 0; i < numerus; i++)
        {
            StmlNodus* nodus;
            chorda     via;

            nodus = *(StmlNodus**)xar_obtinere(viae, i);
            via = stml_textus_internus(nodus, piscina);
            si (via.mensura > 0)
            {
                _chordam_addere(configuratio->inclusa, via);
            }
        }
    }

    sectio = stml_invenire_liberum(radix_nodus, "vexilla");
    configuratio->vexilla = _vexilla_colligere(sectio, piscina);

    sectio = stml_invenire_liberum(radix_nodus, "variantes");
    si (sectio != NIHIL)
    {
        chorda* praelatio;

        praelatio = stml_attributum_capere(sectio, "praelatio");
        si (praelatio != NIHIL)
        {
            _verba_scindere(*praelatio, configuratio->variantes);
        }
    }

    sectio = stml_invenire_liberum(radix_nodus, "nexus");
    si (sectio != NIHIL)
    {
        Xar* regulae;

        regulae = stml_invenire_omnes_liberos(sectio, "regula",
            piscina);
        numerus = xar_numerus(regulae);
        per (i = 0; i < numerus; i++)
        {
            StmlNodus*         nodus;
            chorda*            obiectum;
            AedilisRegulaNexus* regula;

            nodus = *(StmlNodus**)xar_obtinere(regulae, i);
            obiectum = stml_attributum_capere(nodus, "obiectum");
            si (obiectum == NIHIL)
            {
                perge;
            }
            regula = (AedilisRegulaNexus*)xar_addere(
                configuratio->regulae_nexus);
            si (regula != NIHIL)
            {
                regula->obiectum = *obiectum;
                regula->vexilla = _vexilla_colligere(nodus,
                    piscina);
            }
        }
    }

    sectio = stml_invenire_liberum(radix_nodus, "vendor");
    si (sectio != NIHIL)
    {
        Xar* fontes;

        fontes = stml_invenire_omnes_liberos(sectio, "fons",
            piscina);
        numerus = xar_numerus(fontes);
        per (i = 0; i < numerus; i++)
        {
            StmlNodus*           nodus;
            chorda*              fons;
            AedilisRegulaVendor* regula;

            nodus = *(StmlNodus**)xar_obtinere(fontes, i);
            fons = stml_attributum_capere(nodus, "via");
            si (fons == NIHIL)
            {
                perge;
            }
            regula = (AedilisRegulaVendor*)xar_addere(
                configuratio->regulae_vendor);
            si (regula != NIHIL)
            {
                regula->fons = *fons;
                regula->vexilla = _vexilla_colligere(nodus,
                    piscina);
            }
        }
    }

    sectio = stml_invenire_liberum(radix_nodus, "irregularia");
    si (sectio != NIHIL)
    {
        Xar* regulae;

        regulae = stml_invenire_omnes_liberos(sectio, "regula",
            piscina);
        numerus = xar_numerus(regulae);
        per (i = 0; i < numerus; i++)
        {
            StmlNodus*        nodus;
            chorda*           caput;
            AedilisIrregulare* regula;
            Xar*              obiecta;
            i32               k;
            i32               numerus_obiectorum;

            nodus = *(StmlNodus**)xar_obtinere(regulae, i);
            caput = stml_attributum_capere(nodus, "caput");
            si (caput == NIHIL)
            {
                perge;
            }
            regula = (AedilisIrregulare*)xar_addere(
                configuratio->irregularia);
            si (regula == NIHIL)
            {
                perge;
            }
            regula->caput = *caput;
            regula->obiecta = _xar_chordarum(piscina);
            obiecta = stml_invenire_omnes_liberos(nodus,
                "obiectum", piscina);
            numerus_obiectorum = xar_numerus(obiecta);
            per (k = 0; k < numerus_obiectorum; k++)
            {
                StmlNodus* obiectum_nodus;
                chorda     via;

                obiectum_nodus = *(StmlNodus**)xar_obtinere(
                    obiecta, k);
                via = stml_textus_internus(obiectum_nodus,
                    piscina);
                si (via.mensura > 0)
                {
                    _chordam_addere(regula->obiecta, via);
                }
            }
        }
    }

    redde configuratio;
}

/* ====================================================
 * Resolutio directivarum
 * ==================================================== */

/* Viam resolutam (relativam radici, normalizatam) reddere;
 * chorda vacua = nusquam inventa (systema) */
interior chorda
_directivam_resolvere (constans AedilisConfiguratio* configuratio,
    chorda includens_dir, chorda rogata, Piscina* piscina)
{
    chorda candidata;
    chorda vacua;
    i32    i;
    i32    numerus;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    /* rogata ipsa relativa radici (tegit -MM iam-resolutas) */
    si (_existit_sub_radice(configuratio, rogata, piscina))
    {
        redde via_normalizare(rogata, piscina);
    }

    /* relativa includenti */
    si (includens_dir.mensura > 0
        && !chorda_aequalis_literis(includens_dir, "."))
    {
        candidata = via_normalizare(
            _iungere_binas(includens_dir, rogata, piscina),
            piscina);
        si (_existit_sub_radice(configuratio, candidata, piscina))
        {
            redde candidata;
        }
    }

    /* directoria -I ordine */
    numerus = xar_numerus(configuratio->inclusa);
    per (i = 0; i < numerus; i++)
    {
        chorda inclusum;

        inclusum = *(chorda*)xar_obtinere(configuratio->inclusa,
            i);
        candidata = via_normalizare(
            _iungere_binas(inclusum, rogata, piscina), piscina);
        si (_existit_sub_radice(configuratio, candidata, piscina))
        {
            redde candidata;
        }
    }

    redde vacua;
}

/* ====================================================
 * Proba conventionis: caput -> fons obiecti
 * ==================================================== */

interior chorda
_candidatam_struere (chorda directorium, chorda basis,
    constans character* varians, constans character* extensio,
    Piscina* piscina)
{
    ChordaAedificator* aedificator;

    aedificator = chorda_aedificator_creare(piscina, 128);
    chorda_aedificator_appendere_chorda(aedificator, directorium);
    chorda_aedificator_appendere_literis(aedificator, "/");
    chorda_aedificator_appendere_chorda(aedificator, basis);
    si (varians != NIHIL)
    {
        chorda_aedificator_appendere_literis(aedificator, "_");
        chorda_aedificator_appendere_literis(aedificator, varians);
    }
    chorda_aedificator_appendere_literis(aedificator, extensio);
    redde chorda_aedificator_finire(aedificator);
}

/* Caput -> fons obiecti per conventionem (variantes praelatione,
 * deinde basis); chorda vacua = caput sine obiecto */
interior chorda
_obiectum_probare (constans AedilisConfiguratio* configuratio,
    Xar* variantes_effectivae, chorda caput, Piscina* piscina)
{
    chorda basis;
    chorda caput_dir;
    chorda obiecti_dir;
    chorda candidata;
    chorda vacua;
    i32    i;
    i32    numerus;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    basis = via_nomen_radix(via_nomen(caput, piscina), piscina);
    caput_dir = via_directorium(caput, piscina);
    si (chorda_aequalis_literis(caput_dir, "include"))
    {
        obiecti_dir = chorda_ex_literis("lib", piscina);
    }
    alioquin
    {
        obiecti_dir = caput_dir;
    }

    numerus = xar_numerus(variantes_effectivae);
    per (i = 0; i < numerus; i++)
    {
        chorda     varians;
        character* varians_cstr;

        varians = *(chorda*)xar_obtinere(variantes_effectivae, i);
        varians_cstr = chorda_ut_cstr(varians, piscina);
        candidata = _candidatam_struere(obiecti_dir, basis,
            varians_cstr, ".m", piscina);
        si (_existit_sub_radice(configuratio, candidata, piscina))
        {
            redde candidata;
        }
        candidata = _candidatam_struere(obiecti_dir, basis,
            varians_cstr, ".c", piscina);
        si (_existit_sub_radice(configuratio, candidata, piscina))
        {
            redde candidata;
        }
    }
    candidata = _candidatam_struere(obiecti_dir, basis, NIHIL,
        ".c", piscina);
    si (_existit_sub_radice(configuratio, candidata, piscina))
    {
        redde candidata;
    }
    redde vacua;
}

/* ====================================================
 * Machina derivationis
 * ==================================================== */

nomen structura {
    Piscina*                      piscina;
    constans AedilisConfiguratio* configuratio;
    AedilisFructus*               fructus;
    TabulaDispersa*               visa_fontium;
    TabulaDispersa*               visa_capitum;
    TabulaDispersa*               visa_obiectorum;
    TabulaDispersa*               visa_systematum;
    TabulaDispersa*               visa_vendorum;
    TabulaDispersa*               textualia;      /* .c inclusa */
    Xar*                          pendentia;      /* chorda */
    Xar*                          variantes_effectivae;
} _Machina;

interior Xar*
_regulam_nexus_invenire (constans AedilisConfiguratio* configuratio,
    chorda via_obiecti, Piscina* piscina)
{
    chorda basis;
    i32    i;
    i32    numerus;

    basis = via_nomen_radix(via_nomen(via_obiecti, piscina),
        piscina);
    numerus = xar_numerus(configuratio->regulae_nexus);
    per (i = 0; i < numerus; i++)
    {
        AedilisRegulaNexus* regula;

        regula = (AedilisRegulaNexus*)xar_obtinere(
            configuratio->regulae_nexus, i);
        si (chorda_aequalis(regula->obiectum, basis))
        {
            redde regula->vexilla;
        }
    }
    redde NIHIL;
}

interior vacuum
_obiectum_addere (_Machina* machina, chorda via, chorda caput,
    AedilisOrigo origo)
{
    AedilisObiectum* obiectum;
    b32              absens;

    si (chorda_aequalis(via, machina->fructus->scopus))
    {
        redde;  /* TU ipsum, non obiectum separatum */
    }
    si (tabula_dispersa_continet(machina->visa_obiectorum, via))
    {
        redde;
    }
    (vacuum)tabula_dispersa_inserere(machina->visa_obiectorum,
        via, NIHIL);

    absens = !_existit_sub_radice(machina->configuratio, via,
        machina->piscina);
    obiectum = (AedilisObiectum*)xar_addere(
        machina->fructus->obiecta);
    si (obiectum == NIHIL)
    {
        redde;
    }
    obiectum->via = via;
    obiectum->caput = caput;
    obiectum->origo = origo;
    obiectum->absens = absens;
    obiectum->vexilla_nexus = _regulam_nexus_invenire(
        machina->configuratio, via, machina->piscina);

    /* obiecta annotata = veritas nexus DECLARATA, numquam
     * ambulata: dependentiae compilationis eorum generatoribus
     * suis pertinent (capsulae generatae 9MB parsare = 17s pro
     * nihilo - inventum vestitoris vitreae). Fontes > 1MB = data-
     * ut-fons (biblia_dr 6.1MB: silva parsare = OOM; exemplar
     * limitis speculi) - notantur, compilantur, nectuntur, numquam
     * ambulantur. Cetera ambulantur. */
    si (!absens && origo != AEDILIS_ORIGO_ANNOTATIO
        && filum_mensura(chorda_ut_cstr(_iungere_binas(
                machina->configuratio->radix, via,
                machina->piscina), machina->piscina))
            <= 1048576)
    {
        _chordam_addere(machina->pendentia, via);
    }
}

interior vacuum
_vendor_tractare (_Machina* machina, chorda resoluta)
{
    chorda fons;
    i32    i;
    i32    numerus;
    chorda suffixum_h;

    suffixum_h = chorda_ex_literis(".h", machina->piscina);
    si (chorda_terminatur(resoluta, suffixum_h))
    {
        /* caput vendicatum: conventio X.h -> X.c eodem loco */
        ChordaAedificator* aedificator;
        chorda             sine_extensione;

        sine_extensione.datum = resoluta.datum;
        sine_extensione.mensura = resoluta.mensura - 2;
        aedificator = chorda_aedificator_creare(machina->piscina,
            128);
        chorda_aedificator_appendere_chorda(aedificator,
            sine_extensione);
        chorda_aedificator_appendere_literis(aedificator, ".c");
        fons = chorda_aedificator_finire(aedificator);
        /* caput vendicatum in capita quoque (differentia contra
         * -MM sextum capitum plenum postulat); numquam ambulatum */
        si (!tabula_dispersa_continet(machina->visa_capitum,
                resoluta))
        {
            AedilisCaput* caput;

            (vacuum)tabula_dispersa_inserere(
                machina->visa_capitum, resoluta, NIHIL);
            caput = (AedilisCaput*)xar_addere(
                machina->fructus->capita);
            si (caput != NIHIL)
            {
                caput->via = resoluta;
                caput->origo = AEDILIS_ORIGO_DERIVATUM;
            }
        }
        si (!_existit_sub_radice(machina->configuratio, fons,
                machina->piscina))
        {
            redde;  /* solo capite (stb-classis) */
        }
    }
    alioquin
    {
        fons = resoluta;
    }

    si (tabula_dispersa_continet(machina->visa_vendorum, fons))
    {
        redde;
    }
    (vacuum)tabula_dispersa_inserere(machina->visa_vendorum, fons,
        NIHIL);

    {
        AedilisVendor* vendor;

        vendor = (AedilisVendor*)xar_addere(
            machina->fructus->vendores);
        si (vendor == NIHIL)
        {
            redde;
        }
        vendor->fons = fons;
        vendor->vexilla = NIHIL;
        numerus = xar_numerus(
            machina->configuratio->regulae_vendor);
        per (i = 0; i < numerus; i++)
        {
            AedilisRegulaVendor* regula;

            regula = (AedilisRegulaVendor*)xar_obtinere(
                machina->configuratio->regulae_vendor, i);
            si (chorda_aequalis(regula->fons, fons))
            {
                vendor->vexilla = regula->vexilla;
                frange;
            }
        }
    }
}

interior Xar*
_irregulare_invenire (constans AedilisConfiguratio* configuratio,
    chorda caput)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(configuratio->irregularia);
    per (i = 0; i < numerus; i++)
    {
        AedilisIrregulare* regula;

        regula = (AedilisIrregulare*)xar_obtinere(
            configuratio->irregularia, i);
        si (chorda_aequalis(regula->caput, caput))
        {
            redde regula->obiecta;
        }
    }
    redde NIHIL;
}

AedilisFructus*
aedilis_derivare (Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
    constans character* scopus, constans character* varians,
    AedilisExtractor extractor, vacuum* extractor_datum,
    chorda* causa_out)
{
    _Machina machina;
    AedilisFructus* fructus;
    chorda praefixum_vendor;
    chorda suffixum_h;
    i32 cursor;

    fructus = (AedilisFructus*)piscina_allocare(piscina,
        magnitudo(AedilisFructus));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    fructus->scopus = chorda_ex_literis(scopus, piscina);
    fructus->obiecta = xar_creare(piscina,
        (i32)magnitudo(AedilisObiectum));
    fructus->capita = xar_creare(piscina,
        (i32)magnitudo(AedilisCaput));
    fructus->systemata = _xar_chordarum(piscina);
    fructus->vendores = xar_creare(piscina,
        (i32)magnitudo(AedilisVendor));
    fructus->vexilla_annotata = _xar_chordarum(piscina);

    machina.piscina = piscina;
    machina.configuratio = configuratio;
    machina.fructus = fructus;
    machina.visa_fontium = tabula_dispersa_creare_chorda(piscina,
        256);
    machina.visa_capitum = tabula_dispersa_creare_chorda(piscina,
        256);
    machina.visa_obiectorum = tabula_dispersa_creare_chorda(
        piscina, 256);
    machina.visa_systematum = tabula_dispersa_creare_chorda(
        piscina, 64);
    machina.visa_vendorum = tabula_dispersa_creare_chorda(piscina,
        32);
    machina.textualia = tabula_dispersa_creare_chorda(piscina, 32);
    machina.pendentia = _xar_chordarum(piscina);

    /* variantes effectivae: -varians dato = illa sola;
     * aliter praelatio configurationis ordine */
    machina.variantes_effectivae = _xar_chordarum(piscina);
    si (varians != NIHIL)
    {
        _chordam_addere(machina.variantes_effectivae,
            chorda_ex_literis(varians, piscina));
    }
    alioquin
    {
        i32 i;
        i32 numerus;

        numerus = xar_numerus(configuratio->variantes);
        per (i = 0; i < numerus; i++)
        {
            _chordam_addere(machina.variantes_effectivae,
                *(chorda*)xar_obtinere(configuratio->variantes,
                    i));
        }
    }
    si (xar_numerus(machina.variantes_effectivae) > 0)
    {
        fructus->varians = *(chorda*)xar_obtinere(
            machina.variantes_effectivae, 0);
    }
    alioquin
    {
        fructus->varians.datum = NIHIL;
        fructus->varians.mensura = 0;
    }

    praefixum_vendor = chorda_ex_literis("vendor/", piscina);
    suffixum_h = chorda_ex_literis(".h", piscina);

    _chordam_addere(machina.pendentia, fructus->scopus);
    cursor = 0;

    dum (cursor < xar_numerus(machina.pendentia))
    {
        chorda fons;
        chorda includens_dir;
        Xar*   directivae;
        Xar*   annotationes;
        b32    ex_oraculo;
        i32    i;
        i32    numerus;

        fons = *(chorda*)xar_obtinere(machina.pendentia, cursor);
        cursor++;
        si (tabula_dispersa_continet(machina.visa_fontium, fons))
        {
            perge;
        }
        (vacuum)tabula_dispersa_inserere(machina.visa_fontium,
            fons, NIHIL);

        directivae = NIHIL;
        annotationes = NIHIL;
        ex_oraculo = FALSUM;
        si (!extractor(extractor_datum,
                chorda_ut_cstr(_iungere_binas(configuratio->radix,
                    fons, piscina), piscina),
                piscina, &directivae, &annotationes, &ex_oraculo))
        {
            _causam_ponere(causa_out, piscina,
                "plagula imparsabilis: ", fons);
            redde NIHIL;
        }

        includens_dir = via_directorium(fons, piscina);

        numerus = (directivae == NIHIL)
            ? 0 : xar_numerus(directivae);
        per (i = 0; i < numerus; i++)
        {
            chorda rogata;
            chorda resoluta;

            rogata = *(chorda*)xar_obtinere(directivae, i);
            resoluta = _directivam_resolvere(configuratio,
                includens_dir, rogata, piscina);

            si (resoluta.mensura == 0)
            {
                /* nusquam inventa = caput systematis (sine forma
                 * anguli/quotae indiscernibile - vide worklog) */
                si (!tabula_dispersa_continet(
                        machina.visa_systematum, rogata))
                {
                    (vacuum)tabula_dispersa_inserere(
                        machina.visa_systematum, rogata, NIHIL);
                    _chordam_addere(fructus->systemata, rogata);
                }
                perge;
            }

            si (chorda_incipit(resoluta, praefixum_vendor))
            {
                _vendor_tractare(&machina, resoluta);
                perge;
            }

            si (tabula_dispersa_continet(machina.visa_capitum,
                    resoluta))
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(machina.visa_capitum,
                resoluta, NIHIL);
            {
                AedilisCaput* caput;

                caput = (AedilisCaput*)xar_addere(fructus->capita);
                si (caput != NIHIL)
                {
                    caput->via = resoluta;
                    caput->origo = ex_oraculo
                        ? AEDILIS_ORIGO_ORACULUM
                        : AEDILIS_ORIGO_DERIVATUM;
                }
            }
            _chordam_addere(machina.pendentia, resoluta);

            si (!chorda_terminatur(resoluta, suffixum_h))
            {
                /* .c textualiter inclusum (persona_gen-classis):
                 * symbola in includentem funduntur - obiectum
                 * separatum symbola duplicaret; post punctum fixum
                 * ex obiectis purgatur */
                (vacuum)tabula_dispersa_inserere(machina.textualia,
                    resoluta, NIHIL);
            }
            alioquin
            {
                Xar* irregularia;

                irregularia = _irregulare_invenire(configuratio,
                    resoluta);
                si (irregularia != NIHIL)
                {
                    i32 k;
                    i32 numerus_irregularium;

                    numerus_irregularium = xar_numerus(
                        irregularia);
                    per (k = 0; k < numerus_irregularium; k++)
                    {
                        _obiectum_addere(&machina,
                            *(chorda*)xar_obtinere(irregularia,
                                k),
                            resoluta,
                            AEDILIS_ORIGO_CONFIGURATIO);
                    }
                }
                alioquin
                {
                    chorda obiectum;

                    obiectum = _obiectum_probare(configuratio,
                        machina.variantes_effectivae, resoluta,
                        piscina);
                    si (obiectum.mensura > 0)
                    {
                        _obiectum_addere(&machina, obiectum,
                            resoluta,
                            ex_oraculo ? AEDILIS_ORIGO_ORACULUM
                                       : AEDILIS_ORIGO_DERIVATUM);
                    }
                }
            }
        }

        numerus = (annotationes == NIHIL)
            ? 0 : xar_numerus(annotationes);
        per (i = 0; i < numerus; i++)
        {
            chorda annotatio;
            chorda verbum;
            chorda reliquum;

            annotatio = *(chorda*)xar_obtinere(annotationes, i);
            verbum = _verbum_primum(annotatio, &reliquum);
            si (chorda_aequalis_literis(verbum, "obiectum")
                && reliquum.mensura > 0)
            {
                chorda caput_vacuum;

                caput_vacuum.datum = NIHIL;
                caput_vacuum.mensura = 0;
                _obiectum_addere(&machina, reliquum, caput_vacuum,
                    AEDILIS_ORIGO_ANNOTATIO);
            }
            alioquin si (chorda_aequalis_literis(verbum, "corpus")
                && reliquum.mensura > 0)
            {
                /* caput implementatorem suum declarat (verbum in
                 * capitibus domesticum; cum proba conventionis
                 * COMPONITUR, eam non substituit). Ambulatur -
                 * contra obiectum annotatum. Absens = annotatio
                 * putrida, recusatio. */
                si (!_existit_sub_radice(configuratio, reliquum,
                        piscina))
                {
                    _causam_ponere(causa_out, piscina,
                        "corpus absens: ", reliquum);
                    redde NIHIL;
                }
                _obiectum_addere(&machina, reliquum, fons,
                    AEDILIS_ORIGO_CORPUS);
            }
            alioquin si (chorda_aequalis_literis(verbum,
                    "vexillum") && reliquum.mensura > 0)
            {
                _chordam_addere(fructus->vexilla_annotata,
                    reliquum);
            }
            alioquin
            {
                _causam_ponere(causa_out, piscina,
                    "annotatio ignota: ", annotatio);
                redde NIHIL;
            }
        }
    }

    /* obiecta textualiter inclusa purgare (symbola iam in
     * includentibus vivunt - duplicatio nexus aliter) */
    {
        Xar* purgata;
        i32  i;
        i32  numerus;

        purgata = xar_creare(piscina,
            (i32)magnitudo(AedilisObiectum));
        numerus = xar_numerus(fructus->obiecta);
        per (i = 0; i < numerus; i++)
        {
            AedilisObiectum* obiectum;

            obiectum = (AedilisObiectum*)xar_obtinere(
                fructus->obiecta, i);
            si (tabula_dispersa_continet(machina.textualia,
                    obiectum->via))
            {
                perge;
            }
            {
                AedilisObiectum* locus;

                locus = (AedilisObiectum*)xar_addere(purgata);
                si (locus != NIHIL)
                {
                    *locus = *obiectum;
                }
            }
        }
        fructus->obiecta = purgata;
    }

    redde fructus;
}

/* ====================================================
 * Manifestum STML
 * ==================================================== */

interior constans character*
_origo_titulus (AedilisOrigo origo)
{
    commutatio (origo)
    {
        casus AEDILIS_ORIGO_CONFIGURATIO: redde "configuratio";
        casus AEDILIS_ORIGO_ANNOTATIO:    redde "annotatio";
        casus AEDILIS_ORIGO_ORACULUM:     redde "oraculum";
        casus AEDILIS_ORIGO_CORPUS:       redde "corpus";
        casus AEDILIS_ORIGO_DERIVATUM:
        ordinarius:                       redde "derivatum";
    }
}

interior vacuum
_vexilla_appendere (StmlNodus* parens, Xar* vexilla,
    Piscina* piscina, InternamentumChorda* intern)
{
    i32 i;
    i32 numerus;

    si (vexilla == NIHIL)
    {
        redde;
    }
    numerus = xar_numerus(vexilla);
    per (i = 0; i < numerus; i++)
    {
        StmlNodus* nodus;
        chorda     vexillum;

        vexillum = *(chorda*)xar_obtinere(vexilla, i);
        nodus = stml_elementum_creare(piscina, intern, "vexillum");
        si (nodus == NIHIL)
        {
            perge;
        }
        nodus->crudus = VERUM;
        nodus->captio_directio = STML_CAPTIO_ANTE;
        nodus->captio_numerus = 1;
        {
            StmlNodus* textus;

            textus = stml_textum_creare_ex_chorda(piscina, intern,
                vexillum);
            si (textus != NIHIL)
            {
                (vacuum)stml_liberum_addere(nodus, textus);
            }
        }
        (vacuum)stml_liberum_addere(parens, nodus);
    }
}

chorda
aedilis_manifestum_scribere (constans AedilisFructus* fructus,
    Piscina* piscina, constans character* commissum)
{
    InternamentumChorda* intern;
    StmlNodus* radix;
    StmlNodus* sectio;
    character  tempus_litterae[32];
    i32 i;
    i32 numerus;

    intern = internamentum_creare(piscina);
    radix = stml_elementum_creare(piscina, intern,
        "aedilis-manifestum");
    (vacuum)stml_attributum_addere_chorda(radix, piscina, intern,
        "scopus", fructus->scopus);
    si (fructus->varians.mensura > 0)
    {
        (vacuum)stml_attributum_addere_chorda(radix, piscina,
            intern, "varians", fructus->varians);
    }
    sprintf(tempus_litterae, "%ld", (longus)time(NIHIL));
    (vacuum)stml_attributum_addere(radix, piscina, intern,
        "generatum", tempus_litterae);
    si (commissum != NIHIL)
    {
        (vacuum)stml_attributum_addere(radix, piscina, intern,
            "commissum", commissum);
    }

    sectio = stml_elementum_creare(piscina, intern, "obiecta");
    (vacuum)stml_liberum_addere(radix, sectio);
    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;
        StmlNodus*       nodus;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta,
            i);
        nodus = stml_elementum_creare(piscina, intern, "obiectum");
        (vacuum)stml_attributum_addere_chorda(nodus, piscina,
            intern, "via", obiectum->via);
        si (obiectum->caput.mensura > 0)
        {
            (vacuum)stml_attributum_addere_chorda(nodus, piscina,
                intern, "caput", obiectum->caput);
        }
        (vacuum)stml_attributum_addere(nodus, piscina, intern,
            "origo", _origo_titulus(obiectum->origo));
        si (obiectum->absens)
        {
            (vacuum)stml_attributum_boolean_addere(nodus, piscina,
                intern, "absens");
        }
        _vexilla_appendere(nodus, obiectum->vexilla_nexus,
            piscina, intern);
        (vacuum)stml_liberum_addere(sectio, nodus);
    }

    sectio = stml_elementum_creare(piscina, intern, "capita");
    (vacuum)stml_liberum_addere(radix, sectio);
    numerus = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;
        StmlNodus*    nodus;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        nodus = stml_elementum_creare(piscina, intern, "caput");
        (vacuum)stml_attributum_addere_chorda(nodus, piscina,
            intern, "via", caput->via);
        (vacuum)stml_attributum_addere(nodus, piscina, intern,
            "origo", _origo_titulus(caput->origo));
        (vacuum)stml_liberum_addere(sectio, nodus);
    }

    sectio = stml_elementum_creare(piscina, intern, "systemata");
    (vacuum)stml_liberum_addere(radix, sectio);
    numerus = xar_numerus(fructus->systemata);
    per (i = 0; i < numerus; i++)
    {
        StmlNodus* nodus;

        nodus = stml_elementum_creare(piscina, intern, "caput");
        (vacuum)stml_attributum_addere_chorda(nodus, piscina,
            intern, "via",
            *(chorda*)xar_obtinere(fructus->systemata, i));
        (vacuum)stml_liberum_addere(sectio, nodus);
    }

    sectio = stml_elementum_creare(piscina, intern, "vendores");
    (vacuum)stml_liberum_addere(radix, sectio);
    numerus = xar_numerus(fructus->vendores);
    per (i = 0; i < numerus; i++)
    {
        AedilisVendor* vendor;
        StmlNodus*     nodus;

        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores,
            i);
        nodus = stml_elementum_creare(piscina, intern, "fons");
        (vacuum)stml_attributum_addere_chorda(nodus, piscina,
            intern, "via", vendor->fons);
        _vexilla_appendere(nodus, vendor->vexilla, piscina,
            intern);
        (vacuum)stml_liberum_addere(sectio, nodus);
    }

    sectio = stml_elementum_creare(piscina, intern,
        "vexilla-annotata");
    (vacuum)stml_liberum_addere(radix, sectio);
    _vexilla_appendere(sectio, fructus->vexilla_annotata, piscina,
        intern);

    redde stml_scribere(radix, piscina, VERUM);
}

/* ====================================================
 * Emissio scriptorum bash
 * ==================================================== */

/* Nomen obiecti planatum: lib/tcp_posix.c -> lib__tcp_posix.o
 * (collisiones basium trans directoria impossibiles) */
interior chorda
_obiecti_nomen (chorda fons, Piscina* piscina)
{
    ChordaAedificator* aedificator;
    i32 i;
    i32 finis;

    finis = fons.mensura;
    dum (finis > 0 && fons.datum[finis - 1] != (i8)'.')
    {
        finis--;
    }
    si (finis > 0)
    {
        finis--;  /* punctum ipsum quoque demptum */
    }
    aedificator = chorda_aedificator_creare(piscina, 128);
    per (i = 0; i < finis; i++)
    {
        si (fons.datum[i] == (i8)'/')
        {
            chorda_aedificator_appendere_literis(aedificator,
                "__");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aedificator,
                (character)fons.datum[i]);
        }
    }
    chorda_aedificator_appendere_literis(aedificator, ".o");
    redde chorda_aedificator_finire(aedificator);
}

interior vacuum
_scriptum_vexilla_iungere (ChordaAedificator* aedificator,
    Xar* vexilla)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(vexilla);
    per (i = 0; i < numerus; i++)
    {
        si (i > 0)
        {
            chorda_aedificator_appendere_literis(aedificator, " ");
        }
        chorda_aedificator_appendere_chorda(aedificator,
            *(chorda*)xar_obtinere(vexilla, i));
    }
}

/* Vexillum in xar addere nisi iam adest (deduplicatio nexus) */
interior vacuum
_vexillum_unicum (Xar* xar, chorda vexillum)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(xar);
    per (i = 0; i < numerus; i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(xar, i),
                vexillum))
        {
            redde;
        }
    }
    _chordam_addere(xar, vexillum);
}

chorda
aedilis_scriptum_scribere (constans AedilisFructus* fructus,
    constans AedilisConfiguratio* configuratio, Piscina* piscina,
    b32 solitarius, constans character* commissum)
{
    ChordaAedificator* s;
    chorda             basis;
    character          tempus_litterae[32];
    i32                i;
    i32                numerus;

    basis = via_nomen_radix(via_nomen(fructus->scopus, piscina),
        piscina);
    s = chorda_aedificator_creare(piscina, 8192);

    chorda_aedificator_appendere_literis(s, "#!/bin/bash\n");
    chorda_aedificator_appendere_literis(s,
        "# GENERATUM AB AEDILE - NE MANU EDITES\n# scopus: ");
    chorda_aedificator_appendere_chorda(s, fructus->scopus);
    chorda_aedificator_appendere_literis(s, "\n# varians: ");
    chorda_aedificator_appendere_chorda(s, fructus->varians);
    sprintf(tempus_litterae, "%ld", (longus)time(NIHIL));
    chorda_aedificator_appendere_literis(s, "\n# generatum: ");
    chorda_aedificator_appendere_literis(s, tempus_litterae);
    si (commissum != NIHIL)
    {
        chorda_aedificator_appendere_literis(s, " | commissum: ");
        chorda_aedificator_appendere_literis(s, commissum);
    }
    chorda_aedificator_appendere_literis(s,
        "\n# regeneratio: bin/aedilis ");
    chorda_aedificator_appendere_chorda(s, fructus->scopus);
    chorda_aedificator_appendere_literis(s,
        solitarius ? " (-solitarius)\n" : "\n");
    chorda_aedificator_appendere_literis(s,
        "set -u\n"
        "[ -f aedilis.stml ] || { echo \"AEDILIS: curre ex"
        " radice repositorii\" >&2; exit 1; }\n\n");

    chorda_aedificator_appendere_literis(s, "VEXILLA=\"");
    _scriptum_vexilla_iungere(s, configuratio->vexilla);
    chorda_aedificator_appendere_literis(s, "\"\nINCLUSA=\"");
    numerus = xar_numerus(configuratio->inclusa);
    per (i = 0; i < numerus; i++)
    {
        si (i > 0)
        {
            chorda_aedificator_appendere_literis(s, " ");
        }
        chorda_aedificator_appendere_literis(s, "-I");
        chorda_aedificator_appendere_chorda(s,
            *(chorda*)xar_obtinere(configuratio->inclusa, i));
    }
    chorda_aedificator_appendere_literis(s,
        "\"\nOBIECTA_DIR=\"");
    si (solitarius)
    {
        chorda_aedificator_appendere_literis(s,
            "build/aedilis/");
        chorda_aedificator_appendere_chorda(s, basis);
        chorda_aedificator_appendere_literis(s, "/solitarius");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(s,
            "build/aedilis/obiecta");
    }
    chorda_aedificator_appendere_literis(s,
        "\"\nEXITUS_DIR=\"build/aedilis/");
    chorda_aedificator_appendere_chorda(s, basis);
    chorda_aedificator_appendere_literis(s,
        "\"\nmkdir -p \"$OBIECTA_DIR\" \"$EXITUS_DIR\"\n\n");

    /* capita clausurae = copia vetustatis praecisa ex manifesto */
    chorda_aedificator_appendere_literis(s, "CAPITA=\"");
    numerus = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        si (i > 0)
        {
            chorda_aedificator_appendere_literis(s, " ");
        }
        chorda_aedificator_appendere_chorda(s, caput->via);
    }
    chorda_aedificator_appendere_literis(s, "\"\n\n");

    si (solitarius)
    {
        chorda_aedificator_appendere_literis(s,
            "vetustum () {\n"
            "    return 0\n"  /* hermeticum: semper recompila */
            "}\n\n");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(s,
            "vetustum () {\n"
            "    obj=\"$1\"; fons=\"$2\"\n"
            "    [ ! -f \"$obj\" ] && return 0\n"
            "    [ \"$fons\" -nt \"$obj\" ] && return 0\n"
            "    for c in $CAPITA; do\n"
            "        [ \"$c\" -nt \"$obj\" ] && return 0\n"
            "    done\n"
            "    return 1\n"
            "}\n\n");
    }

    chorda_aedificator_appendere_literis(s,
        "compilare () {\n"
        "    fons=\"$1\"; obj=\"$OBIECTA_DIR/$2\"\n"
        "    [ -f \"$fons\" ] || { echo \"AEDILIS: fons absens:"
        " $fons (generatum nondum?)\" >&2; exit 1; }\n"
        "    if vetustum \"$obj\" \"$fons\"; then\n"
        "        echo \"  [obiectum] $fons\"\n"
        "        clang $VEXILLA $INCLUSA -c \"$fons\" -o"
        " \"$obj\" || exit 1\n"
        "    fi\n"
        "}\n\n");

    /* obiecta clausurae */
    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta,
            i);
        chorda_aedificator_appendere_literis(s, "compilare \"");
        chorda_aedificator_appendere_chorda(s, obiectum->via);
        chorda_aedificator_appendere_literis(s, "\" \"");
        chorda_aedificator_appendere_chorda(s,
            _obiecti_nomen(obiectum->via, piscina));
        chorda_aedificator_appendere_literis(s, "\"\n");
    }

    /* vendicata (vexilla propria, vetustas fonte solo) */
    numerus = xar_numerus(fructus->vendores);
    per (i = 0; i < numerus; i++)
    {
        AedilisVendor* vendor;
        chorda         obiecti;

        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores,
            i);
        obiecti = _obiecti_nomen(vendor->fons, piscina);
        chorda_aedificator_appendere_literis(s,
            "if [ ! -f \"$OBIECTA_DIR/");
        chorda_aedificator_appendere_chorda(s, obiecti);
        chorda_aedificator_appendere_literis(s, "\" ] || [ \"");
        chorda_aedificator_appendere_chorda(s, vendor->fons);
        chorda_aedificator_appendere_literis(s,
            "\" -nt \"$OBIECTA_DIR/");
        chorda_aedificator_appendere_chorda(s, obiecti);
        chorda_aedificator_appendere_literis(s,
            "\" ]; then\n    echo \"  [vendicatum] ");
        chorda_aedificator_appendere_chorda(s, vendor->fons);
        chorda_aedificator_appendere_literis(s,
            "\"\n    clang ");
        si (vendor->vexilla != NIHIL)
        {
            _scriptum_vexilla_iungere(s, vendor->vexilla);
        }
        chorda_aedificator_appendere_literis(s, " -c \"");
        chorda_aedificator_appendere_chorda(s, vendor->fons);
        chorda_aedificator_appendere_literis(s,
            "\" -o \"$OBIECTA_DIR/");
        chorda_aedificator_appendere_chorda(s, obiecti);
        chorda_aedificator_appendere_literis(s,
            "\" || exit 1\nfi\n");
    }

    /* scopus ipse */
    chorda_aedificator_appendere_literis(s, "compilare \"");
    chorda_aedificator_appendere_chorda(s, fructus->scopus);
    chorda_aedificator_appendere_literis(s, "\" \"");
    chorda_aedificator_appendere_chorda(s,
        _obiecti_nomen(fructus->scopus, piscina));
    chorda_aedificator_appendere_literis(s, "\"\n\n");

    /* nexus */
    chorda_aedificator_appendere_literis(s,
        "clang $VEXILLA \\\n    \"$OBIECTA_DIR/");
    chorda_aedificator_appendere_chorda(s,
        _obiecti_nomen(fructus->scopus, piscina));
    chorda_aedificator_appendere_literis(s, "\" \\\n");
    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta,
            i);
        chorda_aedificator_appendere_literis(s,
            "    \"$OBIECTA_DIR/");
        chorda_aedificator_appendere_chorda(s,
            _obiecti_nomen(obiectum->via, piscina));
        chorda_aedificator_appendere_literis(s, "\" \\\n");
    }
    numerus = xar_numerus(fructus->vendores);
    per (i = 0; i < numerus; i++)
    {
        AedilisVendor* vendor;

        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores,
            i);
        chorda_aedificator_appendere_literis(s,
            "    \"$OBIECTA_DIR/");
        chorda_aedificator_appendere_chorda(s,
            _obiecti_nomen(vendor->fons, piscina));
        chorda_aedificator_appendere_literis(s, "\" \\\n");
    }
    /* vexilla nexus deduplicata (regulae) + annotata */
    {
        Xar* nexus_vexilla;

        nexus_vexilla = _xar_chordarum(piscina);
        numerus = xar_numerus(fructus->obiecta);
        per (i = 0; i < numerus; i++)
        {
            AedilisObiectum* obiectum;
            i32 k;
            i32 numerus_vexillorum;

            obiectum = (AedilisObiectum*)xar_obtinere(
                fructus->obiecta, i);
            si (obiectum->vexilla_nexus == NIHIL)
            {
                perge;
            }
            numerus_vexillorum = xar_numerus(
                obiectum->vexilla_nexus);
            per (k = 0; k < numerus_vexillorum; k++)
            {
                _vexillum_unicum(nexus_vexilla,
                    *(chorda*)xar_obtinere(
                        obiectum->vexilla_nexus, k));
            }
        }
        numerus = xar_numerus(fructus->vexilla_annotata);
        per (i = 0; i < numerus; i++)
        {
            _vexillum_unicum(nexus_vexilla,
                *(chorda*)xar_obtinere(fructus->vexilla_annotata,
                    i));
        }
        si (xar_numerus(nexus_vexilla) > 0)
        {
            chorda_aedificator_appendere_literis(s, "    ");
            _scriptum_vexilla_iungere(s, nexus_vexilla);
            chorda_aedificator_appendere_literis(s, " \\\n");
        }
    }
    chorda_aedificator_appendere_literis(s,
        "    -o \"$EXITUS_DIR/");
    chorda_aedificator_appendere_chorda(s, basis);
    chorda_aedificator_appendere_literis(s,
        "\" || exit 1\necho \"AEDILIS STRUCTUM: $EXITUS_DIR/");
    chorda_aedificator_appendere_chorda(s, basis);
    chorda_aedificator_appendere_literis(s, "\"\n");

    redde chorda_aedificator_finire(s);
}
