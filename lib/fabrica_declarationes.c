/* fabrica_declarationes.c - declarationes fabricae: lectio
 * aedificatio.stml (actiones, familiae, gradus, ambitus, post),
 * subsystemata, composita (lectio, explicatio, iudicia coniuncta) et
 * praecondiciones. Pars bibliothecae fabrica (fabrica-6 H2; vide
 * fabrica.c). */

#include "fabrica.h"
#include "fabrica_interna.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "stml_macros.h"
#include "numerus_romanus.h"
#include <stdio.h>
#include <string.h>


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
    redde fabricae_iungere(piscina, via, chorda_ex_literis(linea,
        piscina), "");
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
    alioquin si (chorda_aequalis_literis(valor, "iudicium"))
    {
        *genus = FABRICA_ACTIO_IUDICIUM;
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
            chorda_ut_cstr(fabricae_iungere(piscina, ": ",
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

    verba     = fabricae_xar_chordarum(piscina);
    mandatum  = stml_invenire_liberum(actio_nodus, "mandatum");
    si (mandatum == NIHIL || verba == NIHIL)
    {
        redde verba;
    }
    nodi = stml_invenire_omnes_liberos(mandatum, "verbum", piscina);
    per (i = ZEPHYRUM; nodi != NIHIL && i < xar_numerus(nodi); i++)
    {
        fabricae_chordam_addere(verba, stml_textus_valor(
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
        fons  = fabricae_iungere(piscina, directorium,
            chorda_ex_literis("/",
            piscina), "");
        fons  = fabricae_iungere(piscina, chorda_ut_cstr(fons, piscina),
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
            fabricae_iungere(piscina, "", expansio.loculus.mensura
                > ZEPHYRUM
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
                "titulus", fabricae_iungere(piscina, titulus,
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
        memset(&actio, ZEPHYRUM, magnitudo(actio));
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
        /* SIGNUM (plan 5 T3): iudicium solum - fabrica cursorem ipsa
         * currit et verdictum ipsa scribit */
        valor = stml_attributum_capere(nodus, "signum");
        si (valor != NIHIL)
        {
            si (actio.genus != FABRICA_ACTIO_IUDICIUM)
            {
                redde _recusare(piscina, causa_out, actio.sedes,
                    "signum solum actioni iudicium", *valor);
            }
            actio.signum = *valor;
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
        /* STADIUM IUDICUM (fabrica-6 T3) */
        valor = stml_attributum_capere(nodus, "iudex");
        si (valor == NIHIL || chorda_aequalis_literis(*valor, "falsum"))
        {
            actio.iudex = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(*valor, "verum"))
        {
            actio.iudex = VERUM;
        }
        alioquin
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "iudex nec verum nec falsum", *valor);
        }
        actio.mandatum = _mandatum_legere(nodus, piscina);

        /* GRADUS (fabrica-6 T6c): elementum generis gradus registrati
         * (attributa eius = attributa actionis, generice) - unum, in
         * iudicio solo; <ambitus variabilis="X"/> = nomina declarata */
        {
            i32 g;

            per (g = ZEPHYRUM; g < fabrica_graduum_numerus(); g++)
            {
                constans FabricaGradus* gradus =
                    fabrica_gradus_obtinere(g);
                                   Xar* elementa;
                             StmlNodus* elementum;
                                   i32  k;

                elementa = stml_invenire_omnes_liberos(nodus,
                    gradus->titulus, piscina);
                si (elementa == NIHIL || xar_numerus(elementa) == 0)
                {
                    perge;
                }
                si (actio.gradus != NIHIL || xar_numerus(elementa) > I)
                {
                    redde _recusare(piscina, causa_out, actio.sedes,
                        "gradus plures (unus per actionem)",
                        actio.titulus);
                }
                si (actio.genus != FABRICA_ACTIO_IUDICIUM)
                {
                    redde _recusare(piscina, causa_out, actio.sedes,
                        "gradus extra iudicium", actio.titulus);
                }
                actio.gradus     = gradus;
                actio.attributa  = xar_creare(piscina,
                    (i32)magnitudo(FabricaAttributum));
                elementum = *(StmlNodus**)xar_obtinere(elementa,
                    ZEPHYRUM);
                per (k = ZEPHYRUM; elementum->attributa != NIHIL
                     && k < xar_numerus(elementum->attributa); k++)
                {
                    constans StmlAttributum* a = (constans
                        StmlAttributum*)xar_obtinere(
                        elementum->attributa, k);
                         FabricaAttributum* f;

                    f = (FabricaAttributum*)xar_addere(actio.attributa);
                    si (   f        == NIHIL || a->titulus == NIHIL
                        || a->valor == NIHIL)
                    {
                        perge;
                    }
                    f->titulus  = *a->titulus;
                    f->valor    = *a->valor;
                }
            }
        }
        actio.ambitus = fabricae_xar_chordarum(piscina);
        filii = stml_invenire_omnes_liberos(nodus, "ambitus", piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
            StmlNodus* filius;
               chorda* variabilis;

            filius      = *(StmlNodus**)xar_obtinere(filii, j);
            variabilis  = stml_attributum_capere(filius, "variabilis");
            si (variabilis == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "ambitus sine variabili", nihil);
            }
            fabricae_chordam_addere(actio.ambitus, *variabilis);
        }
        /* POST (fabrica-6 T7): producentes ante hanc - ordo, non
         * clavis */
        actio.post = fabricae_xar_chordarum(piscina);
        filii = stml_invenire_omnes_liberos(nodus, "post", piscina);
        per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii);
             j++)
        {
            StmlNodus* filius;
               chorda* producens;

            filius     = *(StmlNodus**)xar_obtinere(filii, j);
            producens  = stml_attributum_capere(filius, "actio");
            si (producens == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), "post sine actione",
                    nihil);
            }
            fabricae_chordam_addere(actio.post, *producens);
        }

        actio.praecondiciones  = fabricae_xar_chordarum(piscina);
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
            fabricae_chordam_addere(actio.praecondiciones, *nominata);
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
            fabricae_locum_addere(destinatio, forma_loci, *locus_via,
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
            {
                /* AXES DUO (fabrica-6 T2): genus (alias) AUT res+clavis */
                chorda* res = stml_attributum_capere(filius, "res");
                chorda* clavis = stml_attributum_capere(filius,
                    "clavis");

                si (genus != NIHIL && (res != NIHIL || clavis != NIHIL))
                {
                    redde _recusare(piscina, causa_out,
                        _sedes(piscina, via, filius),
                        "ingressus: genus et res/clavis simul (alterum)",
                        *genus);
                }
                si (genus == NIHIL && res != NIHIL && clavis != NIHIL)
                {
                    ingressus->genus = fabrica_genus_ex_pari(*res,
                        *clavis);
                    si (ingressus->genus == NIHIL)
                    {
                        redde _recusare(piscina, causa_out,
                            _sedes(piscina, via, filius),
                            "ingressus: par res/clavis sine genere",
                            fabricae_iungere(piscina, "", *res,
                            chorda_ut_cstr(
                                fabricae_iungere(piscina, "/", *clavis,
                                ""),
                                piscina)));
                    }
                }
                alioquin
                {
                    ingressus->genus = (genus != NIHIL)
                        ? fabrica_genus_invenire(*genus) : NIHIL;
                }
            }
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
        /* actio gradus: membra ingressus et exitus ferunt */
        si (xar_numerus(actio.ingressus) == 0 && actio.gradus == NIHIL)
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
        si (xar_numerus(actio.exitus) == 0 && actio.gradus == NIHIL)
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "actio sine exitu", actio.titulus);
        }
        /* IUDICIUM (spec 3 par. XII): vestigium clavis eius est, et
         * exitus verdictum - neutrum sine altero */
        si (   actio.genus == FABRICA_ACTIO_IUDICIUM && !actio.lectiones
            && actio.gradus == NIHIL)
        {
            redde _recusare(piscina, causa_out, actio.sedes,
                "iudicium sine lectiones=\"verum\"", actio.titulus);
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio.exitus); j++)
        {
            b32 verdictum;

            verdictum = chorda_aequalis_literis(chorda_ex_literis(
                ((FabricaExitus*)xar_obtinere(actio.exitus,
                j))->strategia->titulus, piscina), "verdictum");
            si (verdictum != (actio.genus == FABRICA_ACTIO_IUDICIUM))
            {
                redde _recusare(piscina, causa_out, actio.sedes,
                    verdictum ? "verdictum extra actionem iudicium"
                              : "iudicium sine exitu verdicti",
                    ((FabricaExitus*)xar_obtinere(actio.exitus,
                    j))->via);
            }
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
    viae = fabricae_xar_chordarum(piscina);
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
        fabricae_chordam_addere(viae, *via);
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

b32
fabricae_chordam_continet (
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
    si (!fabricae_chordam_continet(viae, exitus->via))
    {
        fabricae_chordam_addere(viae, exitus->via);
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
    si (fabricae_chordam_continet(acervus, titulus))
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
    fabricae_chordam_addere(acervus, titulus);
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

    acervus  = fabricae_xar_chordarum(piscina);
    viae     = fabricae_xar_chordarum(piscina);
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
        redde fabricae_iudicium(titulus, pessimum, chorda_ex_literis("",
            piscina));
    }
    si (pessimum == FABRICA_RECENS)
    {
        sprintf(numerus, "RECENS: omnes partes recentes (%u)",
            (insignatus integer)xar_numerus(iudicia));
        redde fabricae_iudicium(titulus, pessimum,
            chorda_ex_literis(numerus,
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
    redde fabricae_iudicium(titulus, pessimum,
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
        /* POST (fabrica-6 T7): producens nominatus exstat */
        per (j = ZEPHYRUM;
             actio->post != NIHIL && j < xar_numerus(actio->post); j++)
        {
            chorda nominata;
               b32 inventa;

            nominata  = *(chorda*)xar_obtinere(actio->post, j);
            inventa   = FALSUM;
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
                    "post ignotum (actio nulla)", nominata);
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
