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

/* EXPLICATIO FAMILIAE (plan 2 T4; per gradus fabrica-6): quae omnis
 * gradus explicationis <familia> fert */
nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
               StmlNodus* familia;
                  chorda  sedes;
                  chorda* causa_out;
                  chorda  nihil;            /* "" pro valore absente */
      constans character* titulus;
      constans character* directorium;
      constans character* praefixum;        /* "" si absens */
      constans character* suffixum;         /* "" si absens */
          memoriae_index  mensura_praefixi;
          memoriae_index  mensura_suffixi;
               StmlNodus* templum;
                     b32  basis_declarata;  /* loculus @basis */
                     b32  fons_declaratus;  /* loculus @fons */
} ExplicatioFamiliae;

/* vocatio una templi: basis (nomen plagulae sine suffixo) et fons (via
 * plagulae) */
nomen structura {
    chorda basis;
    chorda fons;
} InstantiaFamiliae;

/* recusatio in gradu explicationis: causa posita (_recusare), FALSUM */
interior b32
_familia_recusata (
    constans ExplicatioFamiliae* e,
             constans character* nuntius,
                         chorda  valor)
{
    (vacuum)_recusare(e->piscina, e->causa_out, e->sedes, nuntius,
        valor);
    redde FALSUM;
}

/* attributa <familia>: titulus et via necessaria, praefixum et suffixum
 * ("" si absentes); sutura enumerationem praebere debet */
interior b32
_familiae_attributa (
         ExplicatioFamiliae* e,
     constans FabricaSutura* sutura)
{
    StmlNodus* f;

    f                    = e->familia;
    e->titulus           = _attributum_cstr(f, "titulus", e->piscina);
    e->directorium       = _attributum_cstr(f, "via", e->piscina);
    e->praefixum         = _attributum_cstr(f, "praefixum", e->piscina);
    e->suffixum          = _attributum_cstr(f, "suffixum", e->piscina);
    e->praefixum         = (e->praefixum != NIHIL) ? e->praefixum : "";
    e->suffixum          = (e->suffixum != NIHIL) ? e->suffixum : "";
    e->mensura_praefixi  = strlen(e->praefixum);
    e->mensura_suffixi   = strlen(e->suffixum);
    si (e->titulus == NIHIL || e->directorium == NIHIL)
    {
        redde _familia_recusata(e, "familia sine titulo aut via",
            e->nihil);
    }
    si (sutura == NIHIL || sutura->enumerare == NIHIL)
    {
        redde _familia_recusata(e,
            "familia sine enumeratione (lector purus) - "
            "fabrica_declarationes_legere_cum_sutura",
            chorda_ex_literis(e->titulus, e->piscina));
    }
    redde VERUM;
}

/* templum unum: elementum fragmentum cuius id '@' incipit; elementum
 * aliud aut templum alterum recusatur. Loculi basis/fons notantur */
interior b32
_templum_invenire (
    ExplicatioFamiliae* e)
{
    Xar* liberi;
    i32  i;

    liberi      = e->familia->liberi;
    e->templum  = NIHIL;
    per (i = ZEPHYRUM; liberi != NIHIL && i < xar_numerus(liberi); i++)
    {
        StmlNodus* liberum;

        liberum = *(StmlNodus**)xar_obtinere(liberi, i);
        si (liberum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (   e->templum != NIHIL || !liberum->fragmentum
            || liberum->fragmentum_id == NIHIL
            || liberum->fragmentum_id->mensura < II
            || liberum->fragmentum_id->datum[ZEPHYRUM] != (i8)'@')
        {
            redde _familia_recusata(e,
                "familia templum UNUM poscit (<#@id ...> corpus </#>)",
                chorda_ex_literis(e->titulus, e->piscina));
        }
        e->templum = liberum;
    }
    si (e->templum == NIHIL)
    {
        redde _familia_recusata(e,
            "familia sine templo (<#@id ...> corpus </#>)",
            chorda_ex_literis(e->titulus, e->piscina));
    }
    e->basis_declarata  = _loculum_declarat(e->templum, "basis",
        e->piscina);
    e->fons_declaratus  = _loculum_declarat(e->templum, "fons",
        e->piscina);
    redde VERUM;
}

/* congruitne plagula praefixo et suffixo familiae? (nomen longius
 * quam ambo simul) */
interior b32
_plagula_congruit (
    constans ExplicatioFamiliae* e,
                         chorda  plagula)
{
    memoriae_index lp;
    memoriae_index ls;

    lp = e->mensura_praefixi;
    ls = e->mensura_suffixi;
    redde (memoriae_index)plagula.mensura > lp + ls
        && memcmp(plagula.datum, e->praefixum, lp) == ZEPHYRUM
        && memcmp(plagula.datum + plagula.mensura - ls, e->suffixum,
               ls) == ZEPHYRUM;
}

/* ' titulus="valor"' ad vocationem */
interior vacuum
_argumentum_addere (
      ChordaAedificator* vocatio,
     constans character* titulus,
                 chorda  valor)
{
    (vacuum)chorda_aedificator_appendere_character(vocatio, ' ');
    (vacuum)chorda_aedificator_appendere_literis(vocatio, titulus);
    (vacuum)chorda_aedificator_appendere_literis(vocatio, "=\"");
    (vacuum)chorda_aedificator_appendere_chorda(vocatio, valor);
    (vacuum)chorda_aedificator_appendere_character(vocatio, '"');
}

/* vocatio templi pro plagula una in radicem: '#@id basis="..."
 * fons="..."' (loculi soli quos templum declarat); instantia servatur.
 * Nomen cum '"' aut '&' recusatur (vocationem frangeret) */
interior b32
_vocationem_addere (
    constans ExplicatioFamiliae* e,
                      StmlNodus* radix,
                         chorda  plagula,
                            Xar* instantiae)
{
    ChordaAedificator* vocatio;
    InstantiaFamiliae  instantia;

    si (   chorda_continet(plagula, chorda_ex_literis("\"", e->piscina))
        || chorda_continet(plagula, chorda_ex_literis("&", e->piscina)))
    {
        redde _familia_recusata(e,
            "familia: nomen plagulae cum '\"' aut '&'", plagula);
    }
    instantia.basis = chorda_sectio(plagula, ZEPHYRUM,
        plagula.mensura - (i32)e->mensura_suffixi);
    instantia.fons  = _quattuor(e->piscina,
        chorda_ex_literis(e->directorium, e->piscina), "/", plagula,
        "");
    vocatio = chorda_aedificator_creare(e->piscina, CCLVI);
    (vacuum)chorda_aedificator_appendere_character(vocatio, '#');
    (vacuum)chorda_aedificator_appendere_chorda(vocatio,
        *e->templum->fragmentum_id);
    si (e->basis_declarata)
    {
        _argumentum_addere(vocatio, "basis", instantia.basis);
    }
    si (e->fons_declaratus)
    {
        _argumentum_addere(vocatio, "fons", instantia.fons);
    }
    si (!stml_liberum_addere(radix, stml_transclusionem_creare(
            e->piscina, e->intern, chorda_aedificator_finire(vocatio))))
    {
        redde FALSUM;
    }
    *(InstantiaFamiliae*)xar_addere(instantiae) = instantia;
    redde VERUM;
}

/* instantiae: arbor expansa -> actio UNA per vocationem, ordine, in
 * 'nodi'; titulus 'familia:basis' a fabrica ponitur (in templo
 * recusatur), fons ut ingressus fasciculus additur */
interior b32
_instantias_colligere (
    constans ExplicatioFamiliae* e,
                      StmlNodus* expansa,
                   constans Xar* instantiae,
                            Xar* nodi)
{
    i32 i;
    i32 k;

    k = ZEPHYRUM;
    per (i = ZEPHYRUM; expansa->liberi != NIHIL
         && i < xar_numerus(expansa->liberi); i++)
    {
                          StmlNodus* actio;
                          StmlNodus* ingressus;
         constans InstantiaFamiliae* instantia;

        actio = *(StmlNodus**)xar_obtinere(expansa->liberi, i);
        si (actio->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (   k >= xar_numerus(instantiae) || actio->titulus == NIHIL
            || !chorda_aequalis_literis(*actio->titulus, "actio"))
        {
            redde _familia_recusata(e,
                "familia: corpus templi actionem UNAM poscit",
                chorda_ex_literis(e->titulus, e->piscina));
        }
        si (stml_attributum_capere(actio, "titulus") != NIHIL)
        {
            redde _familia_recusata(e,
                "familia: titulus in templo (fabrica "
                "'familia:basis' ponit)",
                chorda_ex_literis(e->titulus, e->piscina));
        }
        instantia =
            (constans InstantiaFamiliae*)xar_obtinere(instantiae,
            k);
        (vacuum)stml_attributum_addere_chorda(actio, e->piscina,
            e->intern, "titulus", fabricae_iungere(e->piscina,
                e->titulus, chorda_ex_literis(":", e->piscina),
                chorda_ut_cstr(instantia->basis, e->piscina)));
        ingressus = stml_elementum_creare(e->piscina, e->intern,
            "ingressus");
        si (ingressus == NIHIL)
        {
            redde FALSUM;
        }
        (vacuum)stml_attributum_addere(ingressus, e->piscina, e->intern,
            "genus", "fasciculus");
        (vacuum)stml_attributum_addere_chorda(ingressus, e->piscina,
            e->intern, "via", instantia->fons);
        (vacuum)stml_liberum_addere(actio, ingressus);
        si (actio->linea == ZEPHYRUM)
        {
            actio->linea = e->familia->linea;
        }
        *(StmlNodus**)xar_addere(nodi) = actio;
        k++;
    }
    si (k != xar_numerus(instantiae))
    {
        redde _familia_recusata(e,
            "familia: corpus templi actionem UNAM poscit",
            chorda_ex_literis(e->titulus, e->piscina));
    }
    redde VERUM;
}

/* FAMILIA explicare: nodi actionum instantiarum in 'nodi' appenduntur
 * (vide fabrica_declarationes_legere_cum_sutura), gradibus ordine fixo
 * (prima recusatio vincit): attributa, templum, plagulae directorii,
 * vocationes in arborem syntheticam, expansio, instantiae */
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
      ExplicatioFamiliae  e;
               StmlNodus* radix;
                     Xar* nomina;
                     Xar* instantiae;
    StmlExpansioResultus  expansio;
                     i32  i;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.piscina    = piscina;
    e.intern     = intern;
    e.familia    = familia;
    e.causa_out  = causa_out;
    e.nihil      = chorda_ex_literis("", piscina);
    e.sedes      = _sedes(piscina, via, familia);
    si (!_familiae_attributa(&e, sutura) || !_templum_invenire(&e))
    {
        redde FALSUM;
    }
    si (!sutura->enumerare(sutura->datum, e.directorium, piscina,
        &nomina))
    {
        redde _familia_recusata(&e, "familia: directorium absens",
            chorda_ex_literis(e.directorium, piscina));
    }
    /* arbor synthetica: templum (copia) deinde vocationes */
    radix = stml_elementum_creare(piscina, intern, "aedificatio");
    instantiae = xar_creare(piscina,
        (i32)magnitudo(InstantiaFamiliae));
    si (   radix == NIHIL || instantiae == NIHIL
        || !stml_liberum_addere(radix,
            stml_duplicare(e.templum, piscina, intern)))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        chorda plagula;

        plagula = *(chorda*)xar_obtinere(nomina, i);
        si (   _plagula_congruit(&e, plagula)
            && !_vocationem_addere(&e, radix, plagula, instantiae))
        {
            redde FALSUM;
        }
    }
    expansio = stml_expandere(radix, piscina, intern);
    si (!expansio.successus || expansio.radix_expansa == NIHIL)
    {
        redde _familia_recusata(&e,
            "familia: expansio templi fracta (vitium, loculus)",
            fabricae_iungere(piscina, "", expansio.loculus.mensura
                > ZEPHYRUM ? expansio.loculus : expansio.fragmentum,
                ""));
    }
    redde _instantias_colligere(&e, expansio.radix_expansa, instantiae,
        nodi);
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

/* LECTOR ACTIONUM (fabrica-6, declarationes per gradus): quae omnis
 * gradus lectionis <actio> fert - piscina, via plagulae (sedes),
 * causa recusationis */
nomen structura {
               Piscina* piscina;
    constans character* via;
                chorda* causa_out;
                chorda  nihil;   /* "" pro valore absente */
} LectorActionum;

/* recusatio in gradu lectionis: causa posita (_recusare), FALSUM */
interior b32
_lectio_recusata (
    constans LectorActionum* l,
                     chorda  sedes,
         constans character* nuntius,
                     chorda  valor)
{
    (vacuum)_recusare(l->piscina, l->causa_out, sedes, nuntius, valor);
    redde FALSUM;
}

/* attributum verum/falsum: absens aut "falsum" -> FALSUM, "verum" ->
 * VERUM, aliud recusatur ("<attributum> nec verum nec falsum") */
interior b32
_verum_falsum (
    constans LectorActionum* l,
                  StmlNodus* nodus,
                     chorda  sedes,
         constans character* attributum,
                        b32* valor_out)
{
       chorda* valor;
    character  nuntius[64];

    valor = stml_attributum_capere(nodus, attributum);
    si (valor == NIHIL || chorda_aequalis_literis(*valor, "falsum"))
    {
        *valor_out = FALSUM;
        redde VERUM;
    }
    si (chorda_aequalis_literis(*valor, "verum"))
    {
        *valor_out = VERUM;
        redde VERUM;
    }
    sprintf(nuntius, "%s nec verum nec falsum", attributum);
    redde _lectio_recusata(l, sedes, nuntius, *valor);
}

/* nomina filiorum: <elementum attributum="X"/>... -> X in Xar novo;
 * filius sine attributo recusatur (nuntius) */
interior b32
_nomina_filiorum (
    constans LectorActionum*  l,
                  StmlNodus*  nodus,
         constans character*  elementum,
         constans character*  attributum,
         constans character*  nuntius,
                        Xar** nomina_out)
{
    Xar* filii;
    i32  j;

    *nomina_out = fabricae_xar_chordarum(l->piscina);
    filii = stml_invenire_omnes_liberos(nodus, elementum, l->piscina);
    per (j = ZEPHYRUM; filii != NIHIL && j < xar_numerus(filii); j++)
    {
        StmlNodus* filius;
           chorda* valor;

        filius  = *(StmlNodus**)xar_obtinere(filii, j);
        valor   = stml_attributum_capere(filius, attributum);
        si (valor == NIHIL)
        {
            redde _lectio_recusata(l, _sedes(l->piscina, l->via,
                filius),
                nuntius, l->nihil);
        }
        fabricae_chordam_addere(*nomina_out, *valor);
    }
    redde VERUM;
}

/* attributa <actio>: titulus (unicus inter actiones iam lectas), genus,
 * celer, signum (iudicium solum), lectiones, memorabilis, iudex
 * (STADIUM IUDICUM, fabrica-6 T3), mandatum */
interior b32
_attributa_actionis (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               constans Xar* actiones,
               FabricaActio* actio)
{
     chorda* valor;
        i32  j;

    valor = stml_attributum_capere(nodus, "titulus");
    si (valor == NIHIL)
    {
        redde _lectio_recusata(l, actio->sedes, "actio sine titulo",
            l->nihil);
    }
    actio->titulus = *valor;
    per (j = ZEPHYRUM; j < xar_numerus(actiones); j++)
    {
        si (chorda_aequalis(actio->titulus,
                ((constans FabricaActio*)xar_obtinere(actiones,
                j))->titulus))
        {
            redde _lectio_recusata(l, actio->sedes, "titulus duplex",
                actio->titulus);
        }
    }
    valor = stml_attributum_capere(nodus, "genus");
    si (valor == NIHIL || !_genus_actionis(*valor, &actio->genus))
    {
        redde _lectio_recusata(l, actio->sedes,
            "genus actionis ignotum",
            valor != NIHIL ? *valor : l->nihil);
    }
    si (!_verum_falsum(l, nodus, actio->sedes, "celer", &actio->celer))
    {
        redde FALSUM;
    }
    /* SIGNUM (plan 5 T3): iudicium solum - fabrica cursorem ipsa
     * currit et verdictum ipsa scribit */
    valor = stml_attributum_capere(nodus, "signum");
    si (valor != NIHIL)
    {
        si (actio->genus != FABRICA_ACTIO_IUDICIUM)
        {
            redde _lectio_recusata(l, actio->sedes,
                "signum solum actioni iudicium", *valor);
        }
        actio->signum = *valor;
    }
    si (   !_verum_falsum(l, nodus, actio->sedes, "lectiones",
               &actio->lectiones)
        || !_verum_falsum(l, nodus, actio->sedes, "memorabilis",
               &actio->memorabilis)
        || !_verum_falsum(l, nodus, actio->sedes, "iudex",
               &actio->iudex))
    {
        redde FALSUM;
    }
    actio->mandatum = _mandatum_legere(nodus, l->piscina);
    redde VERUM;
}

/* GRADUS (fabrica-6 T6c): elementum generis gradus registrati
 * (attributa eius = attributa actionis, generice) - unum, in
 * iudicio solo */
interior b32
_gradum_legere (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               FabricaActio* actio)
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
            gradus->titulus, l->piscina);
        si (elementa == NIHIL || xar_numerus(elementa) == 0)
        {
            perge;
        }
        si (actio->gradus != NIHIL || xar_numerus(elementa) > I)
        {
            redde _lectio_recusata(l, actio->sedes,
                "gradus plures (unus per actionem)",
                actio->titulus);
        }
        si (actio->genus != FABRICA_ACTIO_IUDICIUM)
        {
            redde _lectio_recusata(l, actio->sedes,
                "gradus extra iudicium", actio->titulus);
        }
        actio->gradus     = gradus;
        actio->attributa  = xar_creare(l->piscina,
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

            f = (FabricaAttributum*)xar_addere(actio->attributa);
            si (   f        == NIHIL || a->titulus == NIHIL
                || a->valor == NIHIL)
            {
                perge;
            }
            f->titulus  = *a->titulus;
            f->valor    = *a->valor;
        }
    }
    redde VERUM;
}

/* vestigia (T4): opera propria aut communia (communis=verum) */
interior b32
_vestigia_legere (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               FabricaActio* actio)
{
    Xar* filii;
    i32  j;

    actio->vestigia = xar_creare(l->piscina,
        (i32)magnitudo(FabricaLocus));
    actio->communia = xar_creare(l->piscina,
        (i32)magnitudo(FabricaLocus));
    filii = stml_invenire_omnes_liberos(nodus, "vestigium",
        l->piscina);
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
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "vestigium sine via",
                l->nihil);
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
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "forma vestigii ignota",
                *forma);
        }
        si (   communis == NIHIL
            || chorda_aequalis_literis(*communis, "falsum"))
        {
            destinatio = actio->vestigia;
        }
        alioquin si (chorda_aequalis_literis(*communis, "verum"))
        {
            destinatio = actio->communia;
        }
        alioquin
        {
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "communis nec verum nec falsum", *communis);
        }
        fabricae_locum_addere(destinatio, forma_loci, *locus_via,
            suffixa != NIHIL ? *suffixa : l->nihil);
    }
    redde VERUM;
}

/* ingressus: genus (alias) AUT res+clavis, via, suffixa; actio gradus
 * (membra ingressus ferunt) sola sine ingressu */
interior b32
_ingressus_legere (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               FabricaActio* actio)
{
    Xar* filii;
    i32  j;

    actio->ingressus = xar_creare(l->piscina,
        (i32)magnitudo(FabricaIngressus));
    filii = stml_invenire_omnes_liberos(nodus, "ingressus",
        l->piscina);
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
        ingressus = (FabricaIngressus*)xar_addere(actio->ingressus);
        {
            /* AXES DUO (fabrica-6 T2): genus (alias) AUT res+clavis */
            chorda* res = stml_attributum_capere(filius, "res");
            chorda* clavis = stml_attributum_capere(filius,
                "clavis");

            si (genus != NIHIL && (res != NIHIL || clavis != NIHIL))
            {
                redde _lectio_recusata(l,
                    _sedes(l->piscina, l->via, filius),
                    "ingressus: genus et res/clavis simul (alterum)",
                    *genus);
            }
            si (genus == NIHIL && res != NIHIL && clavis != NIHIL)
            {
                ingressus->genus = fabrica_genus_ex_pari(*res,
                    *clavis);
                si (ingressus->genus == NIHIL)
                {
                    redde _lectio_recusata(l,
                        _sedes(l->piscina, l->via, filius),
                        "ingressus: par res/clavis sine genere",
                        fabricae_iungere(l->piscina, "", *res,
                        chorda_ut_cstr(
                            fabricae_iungere(l->piscina, "/", *clavis,
                            ""),
                            l->piscina)));
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
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "genus ingressus ignotum",
                genus != NIHIL ? *genus : l->nihil);
        }
        si (ingressus_via == NIHIL)
        {
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "ingressus sine via",
                l->nihil);
        }
        ingressus->via = *ingressus_via;
        {
            chorda* suffixa;

            suffixa = stml_attributum_capere(filius, "suffixa");
            ingressus->suffixa = (suffixa != NIHIL)
                ? *suffixa : chorda_ex_literis("", l->piscina);
        }
    }
    /* actio gradus: membra ingressus et exitus ferunt */
    si (xar_numerus(actio->ingressus) == 0 && actio->gradus == NIHIL)
    {
        redde _lectio_recusata(l, actio->sedes,
            "actio sine ingressu", actio->titulus);
    }
    redde VERUM;
}

/* exitus: strategia (provenientia), genus (ordinarium strategiae nisi
 * datum), licentia strategiae per genus */
interior b32
_exitus_legere (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               FabricaActio* actio)
{
    Xar* filii;
    i32  j;

    actio->exitus = xar_creare(l->piscina,
        (i32)magnitudo(FabricaExitus));
    filii = stml_invenire_omnes_liberos(nodus, "exitus", l->piscina);
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
        exitus        = (FabricaExitus*)xar_addere(actio->exitus);
        si (exitus_via == NIHIL)
        {
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius), "exitus sine via",
                l->nihil);
        }
        exitus->strategia = (provenientia != NIHIL)
            ? fabrica_strategia_invenire(*provenientia) : NIHIL;
        si (exitus->strategia == NIHIL)
        {
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "provenientia ignota",
                provenientia != NIHIL ? *provenientia : l->nihil);
        }
        /* genus absens: genus ordinarium strategiae */
        exitus->genus = fabrica_genus_invenire(chorda_ex_literis(
            exitus->strategia->genus_ordinarium, l->piscina));
        si (genus_exitus != NIHIL)
        {
            exitus->genus = fabrica_genus_invenire(*genus_exitus);
            si (exitus->genus == NIHIL)
            {
                redde _lectio_recusata(l,
                    _sedes(l->piscina, l->via, filius),
                    "genus exitus ignotum", *genus_exitus);
            }
        }
        si (exitus->genus->locare == NIHIL)
        {
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius),
                "genus ingressus solum, exitus esse nequit",
                chorda_ex_literis(exitus->genus->titulus, l->piscina));
        }
        /* typus strategias validas constringit (binarium non
         * reproducibile: numquam regeneratio - 1a T6) */
        si (   exitus->strategia->octetis_comparat
            && !exitus->genus->reproducibile)
        {
            character nuntius[128];

            sprintf(nuntius, "strategia %s generi %s non licet",
                exitus->strategia->titulus, exitus->genus->titulus);
            redde _lectio_recusata(l,
                _sedes(l->piscina, l->via, filius), nuntius,
                *exitus_via);
        }
        exitus->via = *exitus_via;
        exitus->scriptura = (scriptura != NIHIL)
            ? *scriptura : *exitus_via;
    }
    redde VERUM;
}

/* regulae trans campos (post exitus lectos) */
interior b32
_actionem_probare (
      constans LectorActionum* l,
        constans FabricaActio* actio)
{
    i32 j;

    si (xar_numerus(actio->exitus) == 0 && actio->gradus == NIHIL)
    {
        redde _lectio_recusata(l, actio->sedes,
            "actio sine exitu", actio->titulus);
    }
    /* IUDICIUM (spec 3 par. XII): vestigium clavis eius est, et
     * exitus verdictum - neutrum sine altero */
    si (   actio->genus  == FABRICA_ACTIO_IUDICIUM && !actio->lectiones
        && actio->gradus == NIHIL)
    {
        redde _lectio_recusata(l, actio->sedes,
            "iudicium sine lectiones=\"verum\"", actio->titulus);
    }
    per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
    {
        b32 verdictum;

        verdictum = chorda_aequalis_literis(chorda_ex_literis(
            ((FabricaExitus*)xar_obtinere(actio->exitus,
            j))->strategia->titulus, l->piscina), "verdictum");
        si (verdictum != (actio->genus == FABRICA_ACTIO_IUDICIUM))
        {
            redde _lectio_recusata(l, actio->sedes,
                verdictum ? "verdictum extra actionem iudicium"
                          : "iudicium sine exitu verdicti",
                ((FabricaExitus*)xar_obtinere(actio->exitus,
                j))->via);
        }
    }
    redde VERUM;
}

/* <actio> una -> FabricaActio, gradibus ordine fixo (prima recusatio
 * vincit): attributa, gradus, ambitus, post, praecondiciones, vestigia,
 * ingressus, exitus, regulae trans campos */
interior b32
_actionem_legere (
    constans LectorActionum* l,
                  StmlNodus* nodus,
               constans Xar* actiones,
               FabricaActio* actio)
{
    memset(actio, ZEPHYRUM, magnitudo(FabricaActio));
    actio->sedes = _sedes(l->piscina, l->via, nodus);
    redde _attributa_actionis(l, nodus, actiones, actio)
        && _gradum_legere(l, nodus, actio)
        /* <ambitus variabilis="X"/> = nomina declarata (T6c) */
        && _nomina_filiorum(l, nodus, "ambitus", "variabilis",
               "ambitus sine variabili", &actio->ambitus)
        /* POST (fabrica-6 T7): producentes ante hanc - ordo, non
         * clavis */
        && _nomina_filiorum(l, nodus, "post", "actio",
               "post sine actione", &actio->post)
        && _nomina_filiorum(l, nodus, "praecondicio", "actio",
               "praecondicio sine actione", &actio->praecondiciones)
        && _vestigia_legere(l, nodus, actio)
        && _ingressus_legere(l, nodus, actio)
        && _exitus_legere(l, nodus, actio)
        && _actionem_probare(l, actio);
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
  LectorActionum  lector;
             Xar* actiones;
             Xar* nodi;
             i32  i;

    lector.piscina    = piscina;
    lector.via        = via;
    lector.causa_out  = causa_out;
    lector.nihil      = chorda_ex_literis("", piscina);
    lectum            = stml_legere(contentum, piscina, intern);
    si (   !lectum.successus
        || lectum.elementum_radix == NIHIL
        || !chorda_aequalis_literis(*lectum.elementum_radix->titulus,
               "aedificatio"))
    {
        redde _recusare(piscina, causa_out,
            chorda_ex_literis(via, piscina),
            "radix non aedificatio (aut STML fractum)", lector.nihil);
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
        FabricaActio actio;

        si (!_actionem_legere(&lector,
                *(StmlNodus**)xar_obtinere(nodi, i), actiones, &actio))
        {
            redde NIHIL;
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
