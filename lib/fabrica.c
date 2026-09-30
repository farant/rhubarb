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

/* particula copiae sigillandae: via et octeti eius */
nomen structura {
    chorda via;
    chorda contentum;
} Particula;

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
    redde chorda_comparare(((constans Particula*)a)->via,
        ((constans Particula*)b)->via);
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
    Particula* particula;
       chorda  contentum;

    si (_exclusum_est(exclusa, via))
    {
        redde VERUM;
    }
    si (!sutura->legere(sutura->datum, chorda_ut_cstr(via, piscina),
            piscina, &contentum))
    {
        _causam_ponere(causa_out, piscina, "ingressus absens: ", via,
            "");
        redde FALSUM;
    }
    particula = (Particula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    particula->via        = via;
    particula->contentum  = contentum;
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
            Particula* particula;
                  i32  i;
                  i32  numerus;

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
    particula = (Particula*)xar_addere(particulae);
    si (particula == NIHIL)
    {
        redde FALSUM;
    }
    /* '/' finalis: directorium numquam cum plagula eiusdem viae
     * confunditur */
    particula->via        = _iungere(piscina, "", via, "/");
    particula->contentum  = chorda_aedificator_finire(aedificator);
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

    particulae = xar_creare(piscina, (i32)magnitudo(Particula));
    si (particulae == NIHIL)
    {
        redde FALSUM;
    }
    numerus = xar_numerus(actio->ingressus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FabricaIngressus* ingressus;
                     b32  bonum;

        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            i);
        commutatio (ingressus->genus)
        {
            casus FABRICA_INGRESSUS_MANIFESTUM:
                bonum = _manifestum_explicare(sutura, ingressus->via,
                    exclusa, piscina, particulae, causa_out);
                frange;
            casus FABRICA_INGRESSUS_DIRECTORIUM:
                bonum = _directorium_explicare(sutura, ingressus->via,
                    piscina, particulae, causa_out);
                frange;
            ordinarius:
                bonum = _particulam_legere(sutura, ingressus->via,
                    exclusa, piscina, particulae, causa_out);
                frange;
        }
        si (!bonum)
        {
            redde FALSUM;
        }
    }

    xar_ordinare(particulae, _particulas_comparare);

    sigillum_incipere(&contextus);
    prior.datum    = NIHIL;
    prior.mensura  = ZEPHYRUM;
    numerus        = xar_numerus(particulae);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Particula* particula;
         Sigillum  octeti;

        particula = (Particula*)xar_obtinere(particulae, i);
        /* eadem via bis (manifesta duo) semel sigillatur */
        si (i > 0 && chorda_aequalis(prior, particula->via))
        {
            perge;
        }
        prior = particula->via;
        octeti = sigillum_computare(particula->contentum.datum,
            (memoriae_index)particula->contentum.mensura);
        sigillum_addere(&contextus, particula->via.datum,
            (memoriae_index)particula->via.mensura);
        sigillum_addere(&contextus, "", 1);
        sigillum_addere(&contextus, octeti.octeti, SIGILLUM_OCTETI);
    }
    *sigillum_out = sigillum_finire(&contextus);
    redde VERUM;
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

FabricaIudicium
fabrica_iudicare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
     constans FabricaExitus* exitus,
                        b32  plenus,
                    Piscina* piscina)
{
     Sigillum ingressus;
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
    si (!fabrica_ingressus_sigillare(sutura, actio, NIHIL, piscina,
            &ingressus, &causa))
    {
        redde _iudicium(exitus->via, FABRICA_IGNOTUM, causa);
    }
    si (exitus->provenientia == FABRICA_PROVENIENTIA_RELATIO)
    {
        redde _relationem_iudicare(sutura, exitus, &ingressus, piscina);
    }

    /* REGENERATIO */
    si (!sutura->legere(sutura->datum,
            chorda_ut_cstr(exitus->via, piscina), piscina, &commissum))
    {
        redde _iudicium(exitus->via, FABRICA_STALUM,
            chorda_ex_literis("artificium absens", piscina));
    }
    artificium = sigillum_computare(commissum.datum,
        (memoriae_index)commissum.mensura);
    si (   sutura->meminisse != NIHIL
        && sutura->meminisse(sutura->datum,
               chorda_ut_cstr(actio->titulus, piscina), &ingressus,
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
    si (!sutura->currere(sutura->datum, actio->mandatum,
            chorda_ut_cstr(scriptura_dir, piscina), piscina, &causa))
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

interior b32
_genus_ingressus (
                    chorda  valor,
     FabricaGenusIngressus* genus)
{
    si (chorda_aequalis_literis(valor, "fasciculus"))
    {
        *genus = FABRICA_INGRESSUS_FASCICULUS;
    }
    alioquin si (chorda_aequalis_literis(valor, "manifestum"))
    {
        *genus = FABRICA_INGRESSUS_MANIFESTUM;
    }
    alioquin si (chorda_aequalis_literis(valor, "configuratio"))
    {
        *genus = FABRICA_INGRESSUS_CONFIGURATIO;
    }
    alioquin si (chorda_aequalis_literis(valor, "instrumentum"))
    {
        *genus = FABRICA_INGRESSUS_INSTRUMENTUM;
    }
    alioquin si (chorda_aequalis_literis(valor, "directorium"))
    {
        *genus = FABRICA_INGRESSUS_DIRECTORIUM;
    }
    alioquin
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_provenientia (
                  chorda  valor,
     FabricaProvenientia* provenientia)
{
    si (chorda_aequalis_literis(valor, "regeneratio"))
    {
        *provenientia = FABRICA_PROVENIENTIA_REGENERATIO;
    }
    alioquin si (chorda_aequalis_literis(valor, "relatio"))
    {
        *provenientia = FABRICA_PROVENIENTIA_RELATIO;
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
            si (   genus == NIHIL
                || !_genus_ingressus(*genus, &ingressus->genus))
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

            filius      = *(StmlNodus**)xar_obtinere(filii, j);
            exitus_via  = stml_attributum_capere(filius, "via");
            provenientia = stml_attributum_capere(filius,
                "provenientia");
            scriptura  = stml_attributum_capere(filius, "scriptura");
            exitus     = (FabricaExitus*)xar_addere(actio.exitus);
            si (exitus_via == NIHIL)
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius), "exitus sine via",
                    nihil);
            }
            si (   provenientia == NIHIL
                || !_provenientia(*provenientia, &exitus->provenientia))
            {
                redde _recusare(piscina, causa_out,
                    _sedes(piscina, via, filius),
                    "provenientia ignota",
                    provenientia != NIHIL ? *provenientia : nihil);
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
