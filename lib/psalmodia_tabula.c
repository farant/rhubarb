/* psalmodia_tabula.c - lector tabulae tonorum psalmorum
 *
 * Arbor STML -> typi capitis. Lector vitia COLLIGIT (non ad primum
 * sistit, contra instrumentarium_legere): index lapsuum
 * transcriptionis plenus esse debet. Tabulae magnitudines ex numero
 * liberorum ante impletionem numerantur; vitia sola in Xar crescunt.
 */

#include "psalmodia_tabula.h"
#include "internamentum.h"
#include "stml.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Lector: piscina et vitia collecta
 * ================================================== */

nomen structura {
    Piscina* piscina;
        Xar* vitia;              /* PsalmodiaVitium */
        b32  memoria_defecit;
} Lector;

interior constans character* constans ATTRIBUTA_RADICIS[] = { NIHIL };
interior constans character* constans ATTRIBUTA_TONI[] = {
    "modus", "clavis", "tenor", "pagina", "positio", NIHIL
};
interior constans character* constans ATTRIBUTA_MEDIATIONIS[] = {
    "ad_libitum", "casus", NIHIL
};
interior constans character* constans ATTRIBUTA_TERMINATIONIS[] = {
    "differentia", "ad_libitum", "casus", "clavis", "tenor", "initium",
    "vicaria", NIHIL
};
interior constans character* constans ATTRIBUTA_ACCENTUS[] = {
    "superveniens", "anticipata", NIHIL
};
interior constans character* constans ATTRIBUTA_EXEMPLI[] = {
    "pagina", "pars", "forma", "differentia", "nota", "casus", "primus",
    "canticum", "textus", "notae", NIHIL
};

interior constans character* constans NOMINA_PARTIUM[] = {
    "versus", "semiversus", "initium", "mediatio", "terminatio",
        "flexa",
    NIHIL
};

interior constans character* constans NOMINA_LOCORUM[] = {
    "praeparatio", "accentus", "nota"
};

interior vacuum*
_allocare (
            Lector* lector,
               i32  numerus,
    memoriae_index  magnitudo_elementi)
{
    vacuum* p;

    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    p = piscina_allocare(lector->piscina,
            (memoriae_index)numerus * magnitudo_elementi);
    si (p == NIHIL)
    {
        lector->memoria_defecit = VERUM;
    }
    redde p;
}

interior ChordaAedificator*
_causa_incipere (
    Lector* lector)
{
    ChordaAedificator* a = chorda_aedificator_creare(lector->piscina,
        LXIV);

    si (a == NIHIL)
    {
        lector->memoria_defecit = VERUM;
    }
    redde a;
}

interior vacuum
_vitium_finire (
                Lector* lector,
                   i32  linea,
     ChordaAedificator* a)
{
    PsalmodiaVitium* vitium;

    si (a == NIHIL)
    {
        redde;
    }
    vitium = (PsalmodiaVitium*)xar_addere(lector->vitia);
    si (vitium == NIHIL)
    {
        lector->memoria_defecit = VERUM;
        redde;
    }
    vitium->linea = linea > ZEPHYRUM ? linea : I;
    vitium->causa = chorda_aedificator_finire(a);
}

/* vitium e partibus: prima "valor" secunda (chorda aut vacua) */
interior vacuum
_vitium (
                 Lector* lector,
                    i32  linea,
     constans character* prima,
                 chorda  valor,
     constans character* secunda)
{
    ChordaAedificator* a = _causa_incipere(lector);

    si (a == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, prima);
    chorda_aedificator_appendere_chorda(a, valor);
    chorda_aedificator_appendere_literis(a, secunda);
    _vitium_finire(lector, linea, a);
}


/* ==================================================
 * Auxilia: chordae, attributa, liberi
 * ================================================== */

interior b32
_spatium_album (
    i8 octetus)
{
    redde octetus == ' ' || octetus == '\t' || octetus == '\n'
        || octetus == '\r' || octetus == '\f' || octetus == '\v';
}

interior chorda
_praecidere (
    chorda fons)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = fons.mensura;

    dum (initium < finis && _spatium_album(fons.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _spatium_album(fons.datum[finis - I]))
    {
        finis--;
    }
    fons.datum    += initium;
    fons.mensura  = finis - initium;
    redde fons;
}

interior chorda
_vacua (vacuum)
{
    chorda vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    redde vacua;
}

interior b32
_titulus_est (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

interior chorda
_titulus (
    constans StmlNodus* nodus)
{
    redde nodus->titulus != NIHIL ? *nodus->titulus : _vacua();
}

/* valor attributi; vacua si abest */
interior chorda
_attributum (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    chorda* valor = stml_attributum_capere(nodus, titulus);

    redde valor != NIHIL ? *valor : _vacua();
}

interior b32
_attributum_adest (
    constans StmlNodus* nodus,
    constans character* titulus)
{
    redde stml_attributum_capere(nodus, titulus) != NIHIL;
}

/* attributa non licita -> vitia */
interior vacuum
_attributa_probare (
                       Lector* lector,
           constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 k;
    i32 j;

    si (nodus->attributa == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->attributa); k++)
    {
        constans StmlAttributum* attributum =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa, k);
        b32 notum = FALSUM;

        si (attributum == NIHIL || attributum->titulus == NIHIL)
        {
            perge;
        }
        per (j = ZEPHYRUM; licita[j] != NIHIL; j++)
        {
            si (chorda_aequalis_literis(*attributum->titulus,
                licita[j]))
            {
                notum = VERUM;
            }
        }
        si (!notum)
        {
            ChordaAedificator* a = _causa_incipere(lector);

            si (a == NIHIL)
            {
                redde;
            }
            chorda_aedificator_appendere_literis(a,
                "attributum ignotum \"");
            chorda_aedificator_appendere_chorda(a,
                *attributum->titulus);
            chorda_aedificator_appendere_literis(a, "\" in <");
            chorda_aedificator_appendere_chorda(a, _titulus(nodus));
            chorda_aedificator_appendere_literis(a, ">");
            _vitium_finire(lector, nodus->linea, a);
        }
    }
}

/* attributum necessarium; abest -> vitium, FALSUM */
interior b32
_necessarium (
                Lector* lector,
    constans StmlNodus* nodus,
    constans character* titulus)
{
    ChordaAedificator* a;

    si (_attributum_adest(nodus, titulus))
    {
        redde VERUM;
    }
    a = _causa_incipere(lector);
    si (a != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, "<");
        chorda_aedificator_appendere_chorda(a, _titulus(nodus));
        chorda_aedificator_appendere_literis(a, "> sine attributo \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, "\"");
        _vitium_finire(lector, nodus->linea, a);
    }
    redde FALSUM;
}

/* valor attributi unus ex licitis aut abest; aliter vitium */
interior vacuum
_valorem_ignotum_notare (
                Lector* lector,
    constans StmlNodus* nodus,
    constans character* titulus,
    constans character* licita)
{
    ChordaAedificator* a = _causa_incipere(lector);

    si (a == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, "valor ignotus \"");
    chorda_aedificator_appendere_chorda(a, _attributum(nodus, titulus));
    chorda_aedificator_appendere_literis(a, "\" attributi \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "\" (");
    chorda_aedificator_appendere_literis(a, licita);
    chorda_aedificator_appendere_literis(a, ")");
    _vitium_finire(lector, nodus->linea, a);
}

interior b32
_veritatem_legere (
                 Lector* lector,
     constans StmlNodus* nodus,
     constans character* titulus,
                    b32  ordinarium)
{
    chorda valor;

    si (!_attributum_adest(nodus, titulus))
    {
        redde ordinarium;
    }
    valor = _attributum(nodus, titulus);
    si (chorda_aequalis_literis(valor, "verum"))
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(valor, "falsum"))
    {
        redde FALSUM;
    }
    _valorem_ignotum_notare(lector, nodus, titulus, "verum aut falsum");
    redde ordinarium;
}

/* casus="monosyllaba" */
interior b32
_monosyllaba (
                Lector* lector,
    constans StmlNodus* nodus)
{
    si (!_attributum_adest(nodus, "casus"))
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(_attributum(nodus, "casus"),
        "monosyllaba"))
    {
        redde VERUM;
    }
    _valorem_ignotum_notare(lector, nodus, "casus", "monosyllaba");
    redde FALSUM;
}

/* fons spatiis albis divisus -> signa (sectiones fontis) */
interior i32
_signa_dividere (
      Lector*  lector,
      chorda   fons,
      chorda** exitus)
{
    i32 numerus  = ZEPHYRUM;
    i32 k        = ZEPHYRUM;
    i32 initium;

    *exitus = NIHIL;
    dum (k < fons.mensura)
    {
        si (_spatium_album(fons.datum[k]))
        {
            k++;
            perge;
        }
        numerus++;
        dum (k < fons.mensura && !_spatium_album(fons.datum[k]))
        {
            k++;
        }
    }
    *exitus = (chorda*)_allocare(lector, numerus, magnitudo(chorda));
    si (*exitus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    numerus  = ZEPHYRUM;
    k        = ZEPHYRUM;
    dum (k < fons.mensura)
    {
        si (_spatium_album(fons.datum[k]))
        {
            k++;
            perge;
        }
        initium = k;
        dum (k < fons.mensura && !_spatium_album(fons.datum[k]))
        {
            k++;
        }
        (*exitus)[numerus].datum    = fons.datum + initium;
        (*exitus)[numerus].mensura  = k - initium;
        numerus++;
    }
    redde numerus;
}

interior i32
_liberos_numerare (
             StmlNodus* nodus,
    constans character* titulus)
{
    i32 numerus = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < stml_numerus_liberorum(nodus); k++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(nodus, k);

        si (   liber != NIHIL && liber->genus == STML_NODUS_ELEMENTUM
            && _titulus_est(liber, titulus))
        {
            numerus++;
        }
    }
    redde numerus;
}

/* liber non elementum: commentum praeteritur, textus albus
 * praeteritur; cetera -> vitium. VERUM si liber elementum est. */
interior b32
_liberum_examinare (
       Lector* lector,
    StmlNodus* parens,
    StmlNodus* liber)
{
    si (liber == NIHIL)
    {
        redde FALSUM;
    }
    si (liber->genus == STML_NODUS_ELEMENTUM)
    {
        redde VERUM;
    }
    si (liber->genus == STML_NODUS_COMMENTUM)
    {
        redde FALSUM;
    }
    si (   liber->genus == STML_NODUS_TEXTUS && liber->valor != NIHIL
        && _praecidere(*liber->valor).mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    {
        ChordaAedificator* a = _causa_incipere(lector);

        si (a == NIHIL)
        {
            redde FALSUM;
        }
        chorda_aedificator_appendere_literis(a,
            "textus praeter elementa in <");
        chorda_aedificator_appendere_chorda(a, _titulus(parens));
        chorda_aedificator_appendere_literis(a, ">");
        si (liber->valor != NIHIL)
        {
            chorda_aedificator_appendere_literis(a, ": \"");
            chorda_aedificator_appendere_chorda(a,
                _praecidere(*liber->valor));
            chorda_aedificator_appendere_literis(a, "\"");
        }
        _vitium_finire(lector, liber->linea > ZEPHYRUM ? liber->linea
            : parens->linea, a);
    }
    redde FALSUM;
}

interior vacuum
_elementum_ignotum (
       Lector* lector,
    StmlNodus* parens,
    StmlNodus* liber)
{
    ChordaAedificator* a = _causa_incipere(lector);

    si (a == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, "elementum ignotum <");
    chorda_aedificator_appendere_chorda(a, _titulus(liber));
    chorda_aedificator_appendere_literis(a, "> in <");
    chorda_aedificator_appendere_chorda(a, _titulus(parens));
    chorda_aedificator_appendere_literis(a, ">");
    _vitium_finire(lector, liber->linea, a);
}

/* folium (initium, flexa, locus): textus solus; elementum intus ->
 * vitium. Reddit textum praecisum. */
interior chorda
_folium (
       Lector* lector,
    StmlNodus* nodus)
{
    i32 k;

    per (k = ZEPHYRUM; k < stml_numerus_liberorum(nodus); k++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(nodus, k);

        si (liber != NIHIL && liber->genus == STML_NODUS_ELEMENTUM)
        {
            _elementum_ignotum(lector, nodus, liber);
        }
    }
    redde _praecidere(stml_textus_valor(nodus, lector->piscina));
}


/* ==================================================
 * Clausulae et exempla
 * ================================================== */

interior vacuum
_clausulam_legere (
                Lector* lector,
             StmlNodus* nodus,
                   b32  terminatio,
     PsalmodiaClausula* clausula)
{
    i32 numerus;
    i32 k;

    memset(clausula, ZEPHYRUM, magnitudo(PsalmodiaClausula));
    _attributa_probare(lector, nodus,
        terminatio ? ATTRIBUTA_TERMINATIONIS
        : ATTRIBUTA_MEDIATIONIS);
    clausula->linea        = nodus->linea;
    clausula->monosyllaba  = _monosyllaba(lector, nodus);
    clausula->ad_libitum = _veritatem_legere(lector, nodus,
        "ad_libitum",
        FALSUM);
    si (terminatio)
    {
        clausula->differentia  = _attributum(nodus, "differentia");
        clausula->vicaria      = _attributum(nodus, "vicaria");
        clausula->clavis       = _attributum(nodus, "clavis");
        clausula->tenor        = _attributum(nodus, "tenor");
        clausula->initium      = _attributum(nodus, "initium");
    }
    numerus = _liberos_numerare(nodus, "praeparatio")
        + _liberos_numerare(nodus, "accentus")
        + _liberos_numerare(nodus, "nota");
    clausula->loci = (PsalmodiaLocus*)_allocare(lector, numerus,
        magnitudo(PsalmodiaLocus));
    si (numerus > ZEPHYRUM && clausula->loci == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < stml_numerus_liberorum(nodus); k++)
    {
             StmlNodus* liber = stml_liberum_ad_indicem(nodus, k);
        PsalmodiaLocus* locus;

        si (!_liberum_examinare(lector, nodus, liber))
        {
            perge;
        }
        si (   !_titulus_est(liber, "praeparatio")
            && !_titulus_est(liber, "accentus")
            && !_titulus_est(liber, "nota"))
        {
            _elementum_ignotum(lector, nodus, liber);
            perge;
        }
        locus = &clausula->loci[clausula->numerus_locorum++];
        memset(locus, ZEPHYRUM, magnitudo(PsalmodiaLocus));
        locus->linea = liber->linea;
        si (_titulus_est(liber, "accentus"))
        {
            _attributa_probare(lector, liber, ATTRIBUTA_ACCENTUS);
            locus->genus         = PSALMODIA_LOCUS_ACCENTUS;
            locus->superveniens  = _attributum(liber, "superveniens");
            locus->anticipata   = _veritatem_legere(lector, liber,
                "anticipata",
                FALSUM);
        }
        alioquin
        {
            _attributa_probare(lector, liber, ATTRIBUTA_RADICIS);
            locus->genus = _titulus_est(liber, "nota")
                ? PSALMODIA_LOCUS_NOTA : PSALMODIA_LOCUS_PRAEPARATIO;
        }
        locus->notae = _folium(lector, liber);
    }
}

/* VERUM si exemplum lectum (pars, textus, notae adsunt et pars nota) */
interior b32
_exemplum_legere (
               Lector* lector,
            StmlNodus* nodus,
    PsalmodiaExemplum* exemplum)
{
       b32 plenum;
    chorda pars;
       i32 k;

    memset(exemplum, ZEPHYRUM, magnitudo(PsalmodiaExemplum));
    _attributa_probare(lector, nodus, ATTRIBUTA_EXEMPLI);
    per (k = ZEPHYRUM; k < stml_numerus_liberorum(nodus); k++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(nodus, k);

        si (_liberum_examinare(lector, nodus, liber))
        {
            _elementum_ignotum(lector, nodus, liber);
        }
    }
    plenum = _necessarium(lector, nodus, "pars");
    plenum = _necessarium(lector, nodus, "textus") && plenum;
    plenum = _necessarium(lector, nodus, "notae") && plenum;
    si (!plenum)
    {
        redde FALSUM;
    }
    pars = _attributum(nodus, "pars");
    per (k = ZEPHYRUM; NOMINA_PARTIUM[k] != NIHIL; k++)
    {
        si (chorda_aequalis_literis(pars, NOMINA_PARTIUM[k]))
        {
            frange;
        }
    }
    si (NOMINA_PARTIUM[k] == NIHIL)
    {
        _valorem_ignotum_notare(lector, nodus, "pars",
            "versus, semiversus, initium, mediatio, terminatio, flexa");
        redde FALSUM;
    }
    exemplum->pars         = (PsalmodiaPars)k;
    exemplum->pagina       = _attributum(nodus, "pagina");
    exemplum->forma        = _attributum(nodus, "forma");
    exemplum->differentia  = _attributum(nodus, "differentia");
    exemplum->nota         = _attributum(nodus, "nota");
    exemplum->monosyllaba  = _monosyllaba(lector, nodus);
    exemplum->primus = _veritatem_legere(lector, nodus, "primus",
        VERUM);
    exemplum->canticum = _veritatem_legere(lector, nodus, "canticum",
        FALSUM);
    exemplum->textus = _attributum(nodus, "textus");
    exemplum->numerus_notarum = _signa_dividere(lector,
        _attributum(nodus, "notae"), &exemplum->notae);
    exemplum->linea = nodus->linea;
    redde VERUM;
}


/* ==================================================
 * Toni
 * ================================================== */

/* modus -> numerus 1..8, 0 alius; -1 ignotus */
interior s32
_modum_numerare (
    chorda modus)
{
    si (   modus.mensura         == I && modus.datum[ZEPHYRUM] >= '1'
        && modus.datum[ZEPHYRUM] <= '8')
    {
        redde (s32)(modus.datum[ZEPHYRUM] - '0');
    }
    si (   chorda_aequalis_literis(modus, "peregrinus")
        || chorda_aequalis_literis(modus, "in directum"))
    {
        redde ZEPHYRUM;
    }
    redde -I;
}

/* VERUM si tonus lectus (modus, clavis, tenor adsunt, modus notus) */
interior b32
_tonum_legere (
            Lector* lector,
         StmlNodus* nodus,
    PsalmodiaTonus* tonus)
{
    b32 plenum;
    b32 initium_lectum  = FALSUM;
    b32 flexa_lecta     = FALSUM;
    i32 k;

    memset(tonus, ZEPHYRUM, magnitudo(PsalmodiaTonus));
    _attributa_probare(lector, nodus, ATTRIBUTA_TONI);
    plenum = _necessarium(lector, nodus, "modus");
    plenum = _necessarium(lector, nodus, "clavis") && plenum;
    plenum = _necessarium(lector, nodus, "tenor") && plenum;
    si (!plenum)
    {
        redde FALSUM;
    }
    tonus->modus          = _attributum(nodus, "modus");
    tonus->modus_numerus  = _modum_numerare(tonus->modus);
    si (tonus->modus_numerus < ZEPHYRUM)
    {
        _valorem_ignotum_notare(lector, nodus, "modus",
            "1-8, peregrinus, in directum");
        redde FALSUM;
    }
    tonus->clavis  = _attributum(nodus, "clavis");
    tonus->tenor   = _attributum(nodus, "tenor");
    tonus->pagina  = _attributum(nodus, "pagina");
    tonus->linea   = nodus->linea;
    si (_attributum_adest(nodus, "positio"))
    {
        si (chorda_aequalis_literis(_attributum(nodus, "positio"),
                "altera"))
        {
            tonus->positio_altera = VERUM;
        }
        alioquin
        {
            _valorem_ignotum_notare(lector, nodus, "positio", "altera");
        }
    }

    tonus->mediationes = (PsalmodiaClausula*)_allocare(lector,
        _liberos_numerare(nodus, "mediatio"),
        magnitudo(PsalmodiaClausula));
    tonus->terminationes = (PsalmodiaClausula*)_allocare(lector,
        _liberos_numerare(nodus, "terminatio"),
        magnitudo(PsalmodiaClausula));
    tonus->exempla = (PsalmodiaExemplum*)_allocare(lector,
        _liberos_numerare(nodus, "exemplum"),
        magnitudo(PsalmodiaExemplum));
    si (lector->memoria_defecit)
    {
        redde FALSUM;
    }

    per (k = ZEPHYRUM; k < stml_numerus_liberorum(nodus); k++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(nodus, k);

        si (!_liberum_examinare(lector, nodus, liber))
        {
            perge;
        }
        si (_titulus_est(liber, "mediatio"))
        {
            _clausulam_legere(lector, liber, FALSUM,
                &tonus->mediationes[tonus->numerus_mediationum++]);
        }
        alioquin si (_titulus_est(liber, "terminatio"))
        {
            _clausulam_legere(lector, liber, VERUM,
                &tonus->terminationes[tonus->numerus_terminationum++]);
        }
        alioquin si (_titulus_est(liber, "exemplum"))
        {
            si (_exemplum_legere(lector, liber,
                    &tonus->exempla[tonus->numerus_exemplorum]))
            {
                tonus->numerus_exemplorum++;
            }
        }
        alioquin si (   _titulus_est(liber, "initium")
                     || _titulus_est(liber, "flexa"))
        {
            b32* lectum = _titulus_est(liber,
                "initium") ? &initium_lectum
                : &flexa_lecta;

            _attributa_probare(lector, liber, ATTRIBUTA_RADICIS);
            si (*lectum)
            {
                _vitium(lector, liber->linea, "<", _titulus(liber),
                    "> iteratum in tono");
                perge;
            }
            *lectum = VERUM;
            si (_titulus_est(liber, "initium"))
            {
                tonus->numerus_initii = _signa_dividere(lector,
                    _folium(lector, liber), &tonus->initium);
            }
            alioquin
            {
                tonus->flexa = _folium(lector, liber);
            }
        }
        alioquin
        {
            _elementum_ignotum(lector, nodus, liber);
        }
    }
    redde VERUM;
}


/* ==================================================
 * Lectio
 * ================================================== */

interior vacuum
_radicem_legere (
             Lector* lector,
          StmlNodus* radix,
    PsalmodiaTabula* tabula)
{
    i32 k;

    si (!_titulus_est(radix, "toni_psalmorum"))
    {
        _vitium(lector, radix->linea, "radix <", _titulus(radix),
            ">, non <toni_psalmorum>");
        redde;
    }
    _attributa_probare(lector, radix, ATTRIBUTA_RADICIS);
    tabula->toni = (PsalmodiaTonus*)_allocare(lector,
        _liberos_numerare(radix, "tonus"), magnitudo(PsalmodiaTonus));
    si (lector->memoria_defecit)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < stml_numerus_liberorum(radix); k++)
    {
        StmlNodus* liber = stml_liberum_ad_indicem(radix, k);

        si (!_liberum_examinare(lector, radix, liber))
        {
            perge;
        }
        si (!_titulus_est(liber, "tonus"))
        {
            _elementum_ignotum(lector, radix, liber);
            perge;
        }
        si (_tonum_legere(lector, liber,
                &tabula->toni[tabula->numerus_tonorum]))
        {
            tabula->numerus_tonorum++;
        }
    }
}

PsalmodiaTabula*
psalmodia_tabulam_legere (
      chorda  stml,
     Piscina* piscina)
{
             Lector  lector;
    PsalmodiaTabula* tabula;
       StmlResultus  r;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    lector.piscina = piscina;
    lector.memoria_defecit = FALSUM;
    lector.vitia = xar_creare(piscina, (i32)magnitudo(PsalmodiaVitium));
    tabula = (PsalmodiaTabula*)piscina_allocare(piscina,
        magnitudo(PsalmodiaTabula));
    si (lector.vitia == NIHIL || tabula == NIHIL)
    {
        redde NIHIL;
    }
    memset(tabula, ZEPHYRUM, magnitudo(PsalmodiaTabula));

    r = stml_legere(stml, piscina, internamentum_creare(piscina));
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        si (r.status == STML_ERROR_MEMORIA)
        {
            redde NIHIL;
        }
        _vitium(&lector, r.linea_erroris, "STML non legitur: ",
            r.error.mensura > ZEPHYRUM ? r.error
                : chorda_ex_literis("documentum vacuum", piscina), "");
    }
    alioquin
    {
        _radicem_legere(&lector, r.elementum_radix, tabula);
    }

    tabula->numerus_vitiorum = xar_numerus(lector.vitia);
    tabula->vitia = (PsalmodiaVitium*)_allocare(&lector,
        tabula->numerus_vitiorum, magnitudo(PsalmodiaVitium));
    si (lector.memoria_defecit)
    {
        redde NIHIL;
    }
    si (tabula->numerus_vitiorum > ZEPHYRUM)
    {
        xar_copiare_ad_tabulam(lector.vitia, tabula->vitia, ZEPHYRUM,
            tabula->numerus_vitiorum);
    }
    redde tabula;
}

b32
psalmodia_successus (
    constans PsalmodiaTabula* tabula)
{
    redde tabula != NIHIL && tabula->numerus_vitiorum == ZEPHYRUM;
}


/* ==================================================
 * Scriptura
 * ================================================== */

interior b32
_evasum (
     ChordaAedificator* a,
                chorda  valor)
{
    i32 k;
    b32 bene = VERUM;

    per (k = ZEPHYRUM; k < valor.mensura && bene; k++)
    {
        commutatio (valor.datum[k])
        {
            casus '\t': bene = chorda_aedificator_appendere_literis(a,
                            "\\t");
                        frange;
            casus '\n': bene = chorda_aedificator_appendere_literis(a,
                            "\\n");
                        frange;
            casus '\r': bene = chorda_aedificator_appendere_literis(a,
                            "\\r");
                        frange;
            casus '\\': bene = chorda_aedificator_appendere_literis(a,
                            "\\\\");
                        frange;
            ordinarius:
                bene = chorda_aedificator_appendere_character(a,
                    (character)valor.datum[k]);
                frange;
        }
    }
    redde bene;
}

/* "\t" + valor evasus, aut "\t-" si vacuus */
interior b32
_campus (
     ChordaAedificator* a,
                chorda  valor)
{
    si (!chorda_aedificator_appendere_character(a, '\t'))
    {
        redde FALSUM;
    }
    si (valor.mensura == ZEPHYRUM)
    {
        redde chorda_aedificator_appendere_character(a, '-');
    }
    redde _evasum(a, valor);
}

interior b32
_campus_veritatis (
     ChordaAedificator* a,
                   b32  valor)
{
    redde chorda_aedificator_appendere_literis(a,
        valor ? "\t1" : "\t0");
}

/* "\t" + signa spatio iuncta, aut "\t-" */
interior b32
_campus_signorum (
      ChordaAedificator* a,
        constans chorda* signa,
                    i32  numerus)
{
    i32 k;
    b32 bene;

    si (numerus == ZEPHYRUM)
    {
        redde chorda_aedificator_appendere_literis(a, "\t-");
    }
    bene = chorda_aedificator_appendere_character(a, '\t');
    per (k = ZEPHYRUM; k < numerus && bene; k++)
    {
        si (k > ZEPHYRUM)
        {
            bene = chorda_aedificator_appendere_character(a, ' ');
        }
        bene = bene && _evasum(a, signa[k]);
    }
    redde bene;
}

interior b32
_clausulam_scribere (
             ChordaAedificator* a,
            constans character* titulus,
    constans PsalmodiaClausula* clausula)
{
    b32 bene;
    i32 k;

    bene = chorda_aedificator_appendere_literis(a, titulus)
        && _campus(a, clausula->differentia)
        && _campus_veritatis(a, clausula->monosyllaba)
        && _campus_veritatis(a, clausula->ad_libitum)
        && _campus(a, clausula->vicaria)
        && _campus(a, clausula->clavis)
        && _campus(a, clausula->tenor)
        && _campus(a, clausula->initium)
        && chorda_aedificator_appendere_character(a, '\n');
    per (k = ZEPHYRUM; k < clausula->numerus_locorum && bene; k++)
    {
        constans PsalmodiaLocus* locus = &clausula->loci[k];

        bene = chorda_aedificator_appendere_literis(a, "locus\t")
            && chorda_aedificator_appendere_literis(a,
                   NOMINA_LOCORUM[locus->genus])
            && _campus(a, locus->notae)
            && _campus(a, locus->superveniens)
            && _campus_veritatis(a, locus->anticipata)
            && chorda_aedificator_appendere_character(a, '\n');
    }
    redde bene;
}

interior b32
_exemplum_scribere (
             ChordaAedificator* a,
    constans PsalmodiaExemplum* exemplum)
{
    redde chorda_aedificator_appendere_literis(a, "exemplum\t")
        && chorda_aedificator_appendere_literis(a,
               NOMINA_PARTIUM[exemplum->pars])
        && _campus(a, exemplum->pagina)
        && _campus(a, exemplum->forma)
        && _campus(a, exemplum->differentia)
        && _campus(a, exemplum->nota)
        && _campus_veritatis(a, exemplum->monosyllaba)
        && _campus_veritatis(a, exemplum->primus)
        && _campus_veritatis(a, exemplum->canticum)
        && _campus(a, exemplum->textus)
        && _campus_signorum(a, exemplum->notae,
        exemplum->numerus_notarum)
        && chorda_aedificator_appendere_character(a, '\n');
}

b32
psalmodia_tabulam_scribere (
    constans PsalmodiaTabula* tabula,
           ChordaAedificator* exitus)
{
    i32 k;
    i32 j;
    b32 bene = VERUM;

    si (tabula == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < tabula->numerus_tonorum && bene; k++)
    {
        constans PsalmodiaTonus* tonus = &tabula->toni[k];

        bene = chorda_aedificator_appendere_literis(exitus, "tonus")
            && _campus(exitus, tonus->modus)
            && _campus(exitus, tonus->clavis)
            && _campus(exitus, tonus->tenor)
            && _campus(exitus, tonus->pagina)
            && _campus_veritatis(exitus, tonus->positio_altera)
            && chorda_aedificator_appendere_character(exitus, '\n');
        si (bene && tonus->numerus_initii > ZEPHYRUM)
        {
            bene = chorda_aedificator_appendere_literis(exitus,
                "initium")
                && _campus_signorum(exitus, tonus->initium,
                       tonus->numerus_initii)
                && chorda_aedificator_appendere_character(exitus, '\n');
        }
        si (bene && tonus->flexa.mensura > ZEPHYRUM)
        {
            bene = chorda_aedificator_appendere_literis(exitus, "flexa")
                && _campus(exitus, tonus->flexa)
                && chorda_aedificator_appendere_character(exitus, '\n');
        }
        per (j = ZEPHYRUM; j < tonus->numerus_mediationum && bene; j++)
        {
            bene = _clausulam_scribere(exitus, "mediatio",
                &tonus->mediationes[j]);
        }
        per (j = ZEPHYRUM; j < tonus->numerus_terminationum
            && bene; j++)
        {
            bene = _clausulam_scribere(exitus, "terminatio",
                &tonus->terminationes[j]);
        }
        per (j = ZEPHYRUM; j < tonus->numerus_exemplorum && bene; j++)
        {
            bene = _exemplum_scribere(exitus, &tonus->exempla[j]);
        }
    }
    redde bene;
}

b32
psalmodia_vitia_scribere (
    constans PsalmodiaTabula* tabula,
          constans character* via,
           ChordaAedificator* exitus)
{
    i32 k;
    b32 bene = VERUM;

    si (tabula == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < tabula->numerus_vitiorum && bene; k++)
    {
        bene = chorda_aedificator_appendere_literis(exitus,
                   via != NIHIL ? via : "")
            && chorda_aedificator_appendere_character(exitus, ':')
            && chorda_aedificator_appendere_i32(exitus,
                   tabula->vitia[k].linea)
            && chorda_aedificator_appendere_literis(exitus, ": ")
            && chorda_aedificator_appendere_chorda(exitus,
                   tabula->vitia[k].causa)
            && chorda_aedificator_appendere_character(exitus, '\n');
    }
    redde bene;
}
