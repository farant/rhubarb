/* psalmodia_tabula.c - lector tabulae tonorum psalmorum
 *
 * Arbor STML -> typi capitis. Lector vitia COLLIGIT (non ad primum
 * sistit, contra instrumentarium_legere): index lapsuum
 * transcriptionis plenus esse debet. Tabulae magnitudines ex numero
 * liberorum ante impletionem numerantur; vitia sola in Xar crescunt.
 *
 * Probationes (R3) in lectione: quaeque chorda notarum per
 * gabc_legere in piscina probationis legitur, quae ante lectionem
 * quamque vacatur - cantus probationis tabulam non onerant. Vitia
 * in fine ordine linearum ponuntur (bis post omnes tonos invenitur).
 */

#include "psalmodia_tabula.h"
#include "gabc.h"
#include "internamentum.h"
#include "stml.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Lector: piscina et vitia collecta
 * ================================================== */

nomen structura {
    Piscina* piscina;
    Piscina* probatio;           /* cantus gabc probationum, vacata */
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
 * Probationes: notae gabc, littera differentiae, bis
 * ================================================== */

interior constans character LITTERAE_GRADUUM[] = "CDEFGAB";

/* "(clavis)" aut "(clavis) a(notae)" per gabc_legere, in piscina
 * probationis (vacata: cantus prior perit) */
interior constans GabcCantus*
_gabc_legere (
              Lector* lector,
              chorda  clavis,
     constans chorda* notae)
{
    ChordaAedificator* a;
           GabcCantus* cantus;

    piscina_vacare(lector->probatio);
    a = chorda_aedificator_creare(lector->probatio, LXIV);
    si (a == NIHIL)
    {
        lector->memoria_defecit = VERUM;
        redde NIHIL;
    }
    chorda_aedificator_appendere_character(a, '(');
    chorda_aedificator_appendere_chorda(a, clavis);
    chorda_aedificator_appendere_character(a, ')');
    si (notae != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, " a(");
        chorda_aedificator_appendere_chorda(a, *notae);
        chorda_aedificator_appendere_character(a, ')');
    }
    cantus = gabc_legere(chorda_aedificator_finire(a),
        lector->probatio);
    si (cantus == NIHIL)
    {
        lector->memoria_defecit = VERUM;
    }
    redde cantus;
}

/* causa VITII gabc primi (monita licent), aut vacua */
interior chorda
_gabc_causa (
    constans GabcCantus* cantus)
{
    i32 k;

    per (k = ZEPHYRUM; k < cantus->numerus_vitiorum; k++)
    {
        si (cantus->vitia[k].gravitas == GABC_VITIUM)
        {
            redde cantus->vitia[k].causa;
        }
    }
    redde _vacua();
}

/* clavis sola legibilis: syllaba una, elementum unum CLAVIS */
interior b32
_clavem_probare (
      Lector* lector,
      chorda  clavis,
         i32  linea)
{
    constans GabcCantus* cantus = _gabc_legere(lector, clavis, NIHIL);
      ChordaAedificator* a;
                 chorda  causa;

    si (cantus == NIHIL)
    {
        redde FALSUM;
    }
    si (   gabc_successus(cantus) && cantus->numerus_syllabarum == I
        && cantus->numerus_notarum                        == ZEPHYRUM
        && cantus->syllabae[ZEPHYRUM].numerus_elementorum == I
        && cantus->syllabae[ZEPHYRUM].elementa[ZEPHYRUM].genus
           == GABC_EL_CLAVIS)
    {
        redde VERUM;
    }
    causa  = _gabc_causa(cantus);
    a      = _causa_incipere(lector);
    si (a != NIHIL)
    {
        chorda_aedificator_appendere_literis(a,
            "clavis non legibilis \"");
        chorda_aedificator_appendere_chorda(a, clavis);
        chorda_aedificator_appendere_literis(a, "\"");
        si (causa.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ": ");
            chorda_aedificator_appendere_chorda(a, causa);
        }
        _vitium_finire(lector, linea, a);
    }
    redde FALSUM;
}

/* notae sub clave: cantus si legibiles - gabc sine VITIO, syllaba
 * una notarum (parenthesis in notis syllabam novam facit), nota una
 * saltem, neumae solae cum alterationibus et spatiis (clavis mutata
 * non). Aliter vitium in linea et NIHIL. Cantus redditus perit
 * probatione proxima. */
interior constans GabcCantus*
_notas_probare (
      Lector* lector,
      chorda  notae,
      chorda  clavis,
         i32  linea)
{
    constans GabcCantus* cantus  = _gabc_legere(lector, clavis, &notae);
     constans character* nostra  = NIHIL;
                 chorda  causa   = _vacua();
      ChordaAedificator* a;
                    i32  k;

    si (cantus == NIHIL)
    {
        redde NIHIL;
    }
    si (!gabc_successus(cantus))
    {
        causa = _gabc_causa(cantus);
    }
    alioquin si (cantus->numerus_syllabarum != II)
    {
        nostra = "parenthesis in notis";
    }
    alioquin si (cantus->numerus_notarum == ZEPHYRUM)
    {
        nostra = "nulla nota";
    }
    alioquin
    {
        constans GabcSyllaba* syllaba = &cantus->syllabae[I];

        per (k = ZEPHYRUM; k < syllaba->numerus_elementorum; k++)
        {
            GabcElementiGenus genus = syllaba->elementa[k].genus;

            si (   genus != GABC_EL_NEUMA && genus != GABC_EL_ALTERATIO
                && genus != GABC_EL_SPATIUM)
            {
                nostra = "clavis aut signum praeter notas";
                frange;
            }
        }
    }
    si (causa.mensura == ZEPHYRUM && nostra == NIHIL)
    {
        redde cantus;
    }
    a = _causa_incipere(lector);
    si (a != NIHIL)
    {
        chorda_aedificator_appendere_literis(a,
            "notae non legibiles \"");
        chorda_aedificator_appendere_chorda(a, notae);
        chorda_aedificator_appendere_literis(a, "\": ");
        si (nostra != NIHIL)
        {
            chorda_aedificator_appendere_literis(a, nostra);
        }
        alioquin
        {
            chorda_aedificator_appendere_chorda(a, causa);
        }
        _vitium_finire(lector, linea, a);
    }
    redde NIHIL;
}

interior vacuum
_tenorem_probare (
      Lector* lector,
      chorda  tenor,
      chorda  clavis,
         i32  linea)
{
    constans GabcCantus* cantus = _notas_probare(lector, tenor, clavis,
        linea);

    si (cantus != NIHIL && cantus->numerus_notarum != I)
    {
        _vitium(lector, linea, "tenor \"", tenor, "\" non nota una");
    }
}

/* gradus a do -> littera ("CDEFGAB"[gradus mod VII]) */
interior character
_littera (
    s32 gradus)
{
    dum (gradus < ZEPHYRUM)
    {
        gradus += VII;
    }
    redde LITTERAE_GRADUUM[gradus % VII];
}

interior b32
_stellam_habet (
    chorda differentia)
{
    i32 k;

    per (k = ZEPHYRUM; k < differentia.mensura; k++)
    {
        si (differentia.datum[k] == '*')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* "terminatio monosyllaba "g"", "mediatio ad libitum" ... */
interior vacuum
_clausulam_nominare (
             ChordaAedificator* a,
                           b32  terminatio,
    constans PsalmodiaClausula* clausula)
{
    chorda_aedificator_appendere_literis(a,
        terminatio ? "terminatio" : "mediatio");
    si (clausula->monosyllaba)
    {
        chorda_aedificator_appendere_literis(a, " monosyllaba");
    }
    si (clausula->ad_libitum)
    {
        chorda_aedificator_appendere_literis(a, " ad libitum");
    }
    si (!terminatio)
    {
        redde;
    }
    si (clausula->differentia.mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " sine differentia");
        redde;
    }
    chorda_aedificator_appendere_literis(a, " \"");
    chorda_aedificator_appendere_chorda(a, clausula->differentia);
    chorda_aedificator_appendere_literis(a, "\"");
}

/* clausula sine loco, anticipata sine superveniente; si clavis bona:
 * notae locorum, tenor et initium, littera differentiae */
interior vacuum
_clausulam_probare (
                        Lector* lector,
    constans PsalmodiaClausula* clausula,
                           b32  terminatio,
                        chorda  clavis,
                           b32  clavis_bona)
{
                     b32 ultima_lecta    = FALSUM;
                     s32 gradus_ultimus  = ZEPHYRUM;
                     i32 k;

    si (clausula->numerus_locorum == ZEPHYRUM)
    {
        _vitium(lector, clausula->linea, terminatio
            ? "<terminatio> sine loco" : "<mediatio> sine loco",
            _vacua(), "");
    }
    per (k = ZEPHYRUM; k < clausula->numerus_locorum; k++)
    {
        constans PsalmodiaLocus* locus = &clausula->loci[k];

        si (   locus->anticipata
            && locus->superveniens.mensura == ZEPHYRUM)
        {
            _vitium(lector, locus->linea,
                "anticipata sine superveniente",
                _vacua(), "");
        }
        si (!clavis_bona)
        {
            perge;
        }
        {
            constans GabcCantus* cantus = _notas_probare(lector,
                locus->notae, clavis, locus->linea);

            si (cantus != NIHIL && k + I == clausula->numerus_locorum)
            {
                ultima_lecta    = VERUM;
                gradus_ultimus  =
                    cantus->notae[cantus->numerus_notarum - I].gradus;
            }
        }
        si (locus->superveniens.mensura > ZEPHYRUM)
        {
            (vacuum)_notas_probare(lector, locus->superveniens, clavis,
                locus->linea);
        }
    }
    si (!terminatio || !clavis_bona)
    {
        redde;
    }
    si (clausula->tenor.mensura > ZEPHYRUM)
    {
        _tenorem_probare(lector, clausula->tenor, clavis,
            clausula->linea);
    }
    si (clausula->initium.mensura > ZEPHYRUM)
    {
        (vacuum)_notas_probare(lector, clausula->initium, clavis,
            clausula->linea);
    }
    si (   ultima_lecta && clausula->differentia.mensura > ZEPHYRUM
        && !_stellam_habet(clausula->differentia))
    {
        character littera = (character)clausula->differentia.datum[
            ZEPHYRUM];
        character debita = _littera(gradus_ultimus);

        si (littera >= 'a' && littera <= 'z')
        {
            littera = (character)(littera - 'a' + 'A');
        }
        si (littera != debita)
        {
            ChordaAedificator* a = _causa_incipere(lector);

            si (a != NIHIL)
            {
                chorda_aedificator_appendere_literis(a,
                    "differentia \"");
                chorda_aedificator_appendere_chorda(a,
                    clausula->differentia);
                chorda_aedificator_appendere_literis(a,
                    "\" sed nota ultima ");
                chorda_aedificator_appendere_character(a, debita);
                _vitium_finire(lector, clausula->linea, a);
            }
        }
    }
}

/* signa notarum (praeter crucem et asteriscum); differentia nominata
 * in tono */
interior vacuum
_exemplum_probare (
                        Lector* lector,
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaExemplum* exemplum,
                           b32  clavis_bona)
{
    i32 k;

    per (k = ZEPHYRUM; clavis_bona
        && k < exemplum->numerus_notarum; k++)
    {
        chorda signum = exemplum->notae[k];

        si (   chorda_aequalis_literis(signum, "*")
            || chorda_aequalis_literis(signum, "\342\200\240"))
        {
            perge;
        }
        (vacuum)_notas_probare(lector, signum, tonus->clavis,
            exemplum->linea);
    }
    si (exemplum->differentia.mensura == ZEPHYRUM)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < tonus->numerus_terminationum; k++)
    {
        si (chorda_aequalis(tonus->terminationes[k].differentia,
                exemplum->differentia))
        {
            redde;
        }
    }
    _vitium(lector, exemplum->linea, "exemplum differentiam \"",
        exemplum->differentia, "\" nominat quam tonus non habet");
}

interior b32
_clausulae_pares (
    constans PsalmodiaClausula* prima,
    constans PsalmodiaClausula* secunda)
{
    redde prima->monosyllaba == secunda->monosyllaba
        && prima->ad_libitum == secunda->ad_libitum
        && chorda_aequalis(prima->differentia, secunda->differentia);
}

/* vitium "<clausula> bis in <ubi> (prior in linea N)" */
interior vacuum
_bis_notare (
                        Lector* lector,
                           b32  terminatio,
    constans PsalmodiaClausula* clausula,
    constans PsalmodiaClausula* prior,
       constans PsalmodiaTonus* tonus)
{
    ChordaAedificator* a = _causa_incipere(lector);

    si (a == NIHIL)
    {
        redde;
    }
    _clausulam_nominare(a, terminatio, clausula);
    si (terminatio)
    {
        chorda_aedificator_appendere_literis(a, " bis in modo ");
        chorda_aedificator_appendere_chorda(a, tonus->modus);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, " bis in tono");
    }
    chorda_aedificator_appendere_literis(a, " (prior in linea ");
    chorda_aedificator_appendere_i32(a, prior->linea);
    chorda_aedificator_appendere_literis(a, ")");
    _vitium_finire(lector, clausula->linea, a);
}

/* tonus lectus: clavis, tenor, initium, flexa, clausulae, exempla;
 * mediatio bis in tono */
interior vacuum
_tonum_probare (
                     Lector* lector,
    constans PsalmodiaTonus* tonus,
                        i32  linea_initii,
                        i32  linea_flexae)
{
    b32 clavis_bona = _clavem_probare(lector, tonus->clavis,
        tonus->linea);
    i32 k;
    i32 j;

    si (clavis_bona)
    {
        _tenorem_probare(lector, tonus->tenor, tonus->clavis,
            tonus->linea);
        per (k = ZEPHYRUM; k < tonus->numerus_initii; k++)
        {
            (vacuum)_notas_probare(lector, tonus->initium[k],
                tonus->clavis, linea_initii);
        }
        si (tonus->flexa.mensura > ZEPHYRUM)
        {
            (vacuum)_notas_probare(lector, tonus->flexa, tonus->clavis,
                linea_flexae);
        }
    }
    per (k = ZEPHYRUM; k < tonus->numerus_mediationum; k++)
    {
        _clausulam_probare(lector, &tonus->mediationes[k], FALSUM,
            tonus->clavis, clavis_bona);
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (_clausulae_pares(&tonus->mediationes[j],
                    &tonus->mediationes[k]))
            {
                _bis_notare(lector, FALSUM, &tonus->mediationes[k],
                    &tonus->mediationes[j], tonus);
                frange;
            }
        }
    }
    per (k = ZEPHYRUM; k < tonus->numerus_terminationum; k++)
    {
        constans PsalmodiaClausula* clausula = &tonus->terminationes[k];

        si (clausula->clavis.mensura > ZEPHYRUM)
        {
            _clausulam_probare(lector, clausula, VERUM,
                clausula->clavis,
                _clavem_probare(lector, clausula->clavis,
                    clausula->linea));
        }
        alioquin
        {
            _clausulam_probare(lector, clausula, VERUM, tonus->clavis,
                clavis_bona);
        }
    }
    per (k = ZEPHYRUM; k < tonus->numerus_exemplorum; k++)
    {
        _exemplum_probare(lector, tonus, &tonus->exempla[k],
            clavis_bona);
    }
}

/* eadem terminatio bis in eodem modo, positionibus ambabus numeratis
 * (clavis: modus, differentia, monosyllaba, ad_libitum) */
interior vacuum
_bis_probare (
             Lector* lector,
    PsalmodiaTabula* tabula)
{
    i32 k;
    i32 j;
    i32 m;
    i32 n;

    per (k = ZEPHYRUM; k < tabula->numerus_tonorum; k++)
    {
        constans PsalmodiaTonus* tonus = &tabula->toni[k];

        per (j = ZEPHYRUM; j < tonus->numerus_terminationum; j++)
        {
            constans PsalmodiaClausula* prior = NIHIL;

            per (m = ZEPHYRUM; m <= k && prior == NIHIL; m++)
            {
                constans PsalmodiaTonus* alter = &tabula->toni[m];
                                    i32  finis;

                si (!chorda_aequalis(alter->modus, tonus->modus))
                {
                    perge;
                }
                finis = m == k ? j : alter->numerus_terminationum;
                per (n = ZEPHYRUM; n < finis; n++)
                {
                    si (_clausulae_pares(&alter->terminationes[n],
                            &tonus->terminationes[j]))
                    {
                        prior = &alter->terminationes[n];
                        frange;
                    }
                }
            }
            si (prior != NIHIL)
            {
                _bis_notare(lector, VERUM, &tonus->terminationes[j],
                    prior, tonus);
            }
        }
    }
}

/* ordo linearum, stabilis (insertio) */
interior vacuum
_vitia_ordinare (
    PsalmodiaVitium* vitia,
                i32  numerus)
{
                i32 k;
                i32 j;
    PsalmodiaVitium vitium;

    per (k = I; k < numerus; k++)
    {
        vitium  = vitia[k];
        j       = k;
        dum (j > ZEPHYRUM && vitia[j - I].linea > vitium.linea)
        {
            vitia[j] = vitia[j - I];
            j--;
        }
        vitia[j] = vitium;
    }
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
    i32 linea_initii    = nodus->linea;
    i32 linea_flexae    = nodus->linea;
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
                linea_initii = liber->linea;
                tonus->numerus_initii = _signa_dividere(lector,
                    _folium(lector, liber), &tonus->initium);
            }
            alioquin
            {
                linea_flexae = liber->linea;
                tonus->flexa = _folium(lector, liber);
            }
        }
        alioquin
        {
            _elementum_ignotum(lector, nodus, liber);
        }
    }
    _tonum_probare(lector, tonus, linea_initii, linea_flexae);
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
    _bis_probare(lector, tabula);
}

/* corpus lectionis; lector paratus (piscina probationis creata) */
interior PsalmodiaTabula*
_tabulam_legere (
      Lector* lector_parans,
      chorda  stml,
     Piscina* piscina)
{
             Lector  lector = *lector_parans;
    PsalmodiaTabula* tabula;
       StmlResultus  r;

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
        _vitia_ordinare(tabula->vitia, tabula->numerus_vitiorum);
    }
    redde tabula;
}

PsalmodiaTabula*
psalmodia_tabulam_legere (
      chorda  stml,
     Piscina* piscina)
{
             Lector  lector;
    PsalmodiaTabula* tabula;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    lector.piscina          = piscina;
    lector.vitia            = NIHIL;
    lector.memoria_defecit  = FALSUM;
    lector.probatio = piscina_generare_dynamicum("psalmodia_probatio",
        (memoriae_index)(XVI * MXXIV));
    si (lector.probatio == NIHIL)
    {
        redde NIHIL;
    }
    tabula = _tabulam_legere(&lector, stml, piscina);
    piscina_destruere(lector.probatio);
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


/* ==================================================
 * Inventio: signa, variantes
 * ================================================== */

interior constans character* constans NUMERI_ROMANI[] = {
    "I", "II", "III", "IV", "V", "VI", "VII", "VIII", NIHIL
};

/* comparatio par: 0 ullum (differentia vacua), 1 exacta,
 * 2 sine respectu majusculae */
#define PAR_ULLUM       0
#define PAR_EXACTUM     1
#define PAR_MAJUSCULAE  2

interior character
_minuscula (
    character littera)
{
    si (littera >= 'A' && littera <= 'Z')
    {
        redde (character)(littera - 'A' + 'a');
    }
    redde littera;
}

interior b32
_signi_separator (
    i8 octetus)
{
    redde _spatium_album(octetus) || octetus == '.';
}

/* nomina paria, spatiis albis neglectis ("D 2" = "D2") */
interior b32
_nomina_paria (
    chorda signum,
    chorda titulus,
       b32 majuscula_refert)
{
    i32 k = ZEPHYRUM;
    i32 j = ZEPHYRUM;

    per (;;)
    {
        dum (k < signum.mensura && _spatium_album(signum.datum[k]))
        {
            k++;
        }
        dum (j < titulus.mensura && _spatium_album(titulus.datum[j]))
        {
            j++;
        }
        si (k == signum.mensura || j == titulus.mensura)
        {
            redde k == signum.mensura && j == titulus.mensura;
        }
        si (majuscula_refert
            ? signum.datum[k] != titulus.datum[j]
            : _minuscula((character)signum.datum[k])
              != _minuscula((character)titulus.datum[j]))
        {
            redde FALSUM;
        }
        k++;
        j++;
    }
}

/* chorda = literae sine respectu majusculae */
interior b32
_literis_par (
                 chorda  fons,
     constans character* literae)
{
    i32 k;

    per (k = ZEPHYRUM; k < fons.mensura; k++)
    {
        si (   literae[k] == '\0'
            || _minuscula((character)fons.datum[k])
               != _minuscula(literae[k]))
        {
            redde FALSUM;
        }
    }
    redde literae[k] == '\0';
}

/* praefixum sine respectu majusculae, deinde finis aut separator */
interior b32
_verbum_praefixum (
                 chorda  fons,
     constans character* verbum,
                    i32* longitudo)
{
    i32 k;

    per (k = ZEPHYRUM; verbum[k] != '\0'; k++)
    {
        si (   k                                    >= fons.mensura
            || _minuscula((character)fons.datum[k]) != verbum[k])
        {
            redde FALSUM;
        }
    }
    si (k < fons.mensura && !_signi_separator(fons.datum[k]))
    {
        redde FALSUM;
    }
    *longitudo = k;
    redde VERUM;
}

/* initium fontis (praecisi) -> modus. numerus 1..8, aut 0 cum
 * titulo ("peregrinus", "in directum"). longitudo = octeti modi
 * (etiam si ignotus: textus causae). FALSUM si ignotus. */
interior b32
_modum_signi_legere (
                  chorda   fons,
                     s32*  numerus,
      constans character** titulus,
                     i32*  longitudo)
{
    i32 k = ZEPHYRUM;
    i32 j;

    *numerus = ZEPHYRUM;
    *titulus = NIHIL;
    si (_verbum_praefixum(fons, "peregrinus", longitudo))
    {
        *titulus = "peregrinus";
        redde VERUM;
    }
    si (_verbum_praefixum(fons, "in directum", longitudo))
    {
        *titulus = "in directum";
        redde VERUM;
    }
    dum (   k < fons.mensura && fons.datum[k] >= '0'
         && fons.datum[k] <= '9')
    {
        k++;
    }
    si (k > ZEPHYRUM)
    {
        *longitudo = k;
        si (   k                    == I && fons.datum[ZEPHYRUM] >= '1'
            && fons.datum[ZEPHYRUM] <= '8')
        {
            *numerus = (s32)(fons.datum[ZEPHYRUM] - '0');
            redde VERUM;
        }
        redde FALSUM;
    }
    dum (   k < fons.mensura
         && (   _minuscula((character)fons.datum[k]) == 'i'
            || _minuscula((character)fons.datum[k]) == 'v'
            || _minuscula((character)fons.datum[k]) == 'x'))
    {
        k++;
    }
    si (k > ZEPHYRUM)
    {
        chorda romanus;

        *longitudo       = k;
        romanus.datum    = fons.datum;
        romanus.mensura  = k;
        per (j = ZEPHYRUM; NUMERI_ROMANI[j] != NIHIL; j++)
        {
            si (_literis_par(romanus, NUMERI_ROMANI[j]))
            {
                *numerus = (s32)(j + I);
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    dum (k < fons.mensura && !_signi_separator(fons.datum[k]))
    {
        k++;
    }
    *longitudo = k;
    redde FALSUM;
}

interior b32
_tonus_modi (
    constans PsalmodiaTonus* tonus,
                        s32  numerus,
         constans character* titulus)
{
    si (numerus > ZEPHYRUM)
    {
        redde tonus->modus_numerus == numerus;
    }
    redde titulus != NIHIL && chorda_aequalis_literis(tonus->modus,
        titulus);
}

interior b32
_clausula_par (
    constans PsalmodiaClausula* clausula,
                        chorda  differentia,
                           i32  modus_paris)
{
    si (clausula->monosyllaba || clausula->ad_libitum)
    {
        redde FALSUM;
    }
    commutatio (modus_paris)
    {
        casus PAR_EXACTUM:
            redde _nomina_paria(differentia, clausula->differentia,
                VERUM);
        casus PAR_MAJUSCULAE:
            redde _nomina_paria(differentia, clausula->differentia,
                FALSUM);
        ordinarius:
            redde VERUM;
    }
}

/* terminatio communis prima par in tonis modi; numerus parium et
 * tonus eius; tonus_modi = tonus primus modi (aut NIHIL) */
interior constans PsalmodiaClausula*
_parem_quaerere (
       constans PsalmodiaTabula*  tabula,
                            s32   numerus,
             constans character*  titulus,
                         chorda   differentia,
                            i32   modus_paris,
                            i32*  numerus_parium,
        constans PsalmodiaTonus** tonus_paris,
        constans PsalmodiaTonus** tonus_modi)
{
    constans PsalmodiaClausula* prima = NIHIL;
                           i32  k;
                           i32  j;

    *numerus_parium  = ZEPHYRUM;
    *tonus_paris     = NIHIL;
    *tonus_modi      = NIHIL;
    per (k = ZEPHYRUM; k < tabula->numerus_tonorum; k++)
    {
        constans PsalmodiaTonus* tonus = &tabula->toni[k];

        si (!_tonus_modi(tonus, numerus, titulus))
        {
            perge;
        }
        si (*tonus_modi == NIHIL)
        {
            *tonus_modi = tonus;
        }
        per (j = ZEPHYRUM; j < tonus->numerus_terminationum; j++)
        {
            si (_clausula_par(&tonus->terminationes[j], differentia,
                    modus_paris))
            {
                si (prima == NIHIL)
                {
                    prima         = &tonus->terminationes[j];
                    *tonus_paris  = tonus;
                }
                (*numerus_parium)++;
            }
        }
    }
    redde prima;
}

/* nomina parium, ", " iuncta */
interior vacuum
_pares_enumerare (
           ChordaAedificator* causa,
    constans PsalmodiaTabula* tabula,
                         s32  numerus,
          constans character* titulus,
                      chorda  differentia,
                         i32  modus_paris)
{
    b32 primum = VERUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < tabula->numerus_tonorum; k++)
    {
        constans PsalmodiaTonus* tonus = &tabula->toni[k];

        si (!_tonus_modi(tonus, numerus, titulus))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < tonus->numerus_terminationum; j++)
        {
            constans PsalmodiaClausula* clausula =
                &tonus->terminationes[j];

            si (!_clausula_par(clausula, differentia, modus_paris))
            {
                perge;
            }
            si (!primum)
            {
                chorda_aedificator_appendere_literis(causa, ", ");
            }
            primum = FALSUM;
            si (clausula->differentia.mensura == ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(causa,
                    "(sine nomine)");
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(causa,
                    clausula->differentia);
            }
        }
    }
}

/* nucleus: modus iam lectus; differentia ut scripta (praecisa) */
interior constans PsalmodiaClausula*
_terminationem_quaerere (
       constans PsalmodiaTabula*  tabula,
                            s32   numerus,
             constans character*  titulus,
                         chorda   differentia,
        constans PsalmodiaTonus** tonus,
              ChordaAedificator*  causa)
{
    constans PsalmodiaClausula* inventa;
       constans PsalmodiaTonus* tonus_paris;
       constans PsalmodiaTonus* tonus_modi;
                           i32  pares;
                           i32  modus_paris;
                           b32  vacua;
                           i32  k;

    vacua = VERUM;
    per (k = ZEPHYRUM; k < differentia.mensura; k++)
    {
        si (!_spatium_album(differentia.datum[k]))
        {
            vacua = FALSUM;
        }
    }
    modus_paris = vacua ? PAR_ULLUM : PAR_EXACTUM;
    inventa = _parem_quaerere(tabula, numerus, titulus, differentia,
        modus_paris, &pares, &tonus_paris, &tonus_modi);
    si (tonus_modi == NIHIL)
    {
        si (causa != NIHIL)
        {
            chorda_aedificator_appendere_literis(causa, "tonus \"");
            si (titulus != NIHIL)
            {
                chorda_aedificator_appendere_literis(causa, titulus);
            }
            alioquin
            {
                chorda_aedificator_appendere_s32(causa, numerus);
            }
            chorda_aedificator_appendere_literis(causa, "\" nullus");
        }
        redde NIHIL;
    }
    si (inventa == NIHIL && !vacua)
    {
        modus_paris  = PAR_MAJUSCULAE;
        inventa      = _parem_quaerere(tabula, numerus, titulus,
            differentia, modus_paris, &pares, &tonus_paris,
            &tonus_modi);
    }
    si (inventa != NIHIL && (modus_paris == PAR_EXACTUM || pares == I))
    {
        si (tonus != NIHIL)
        {
            *tonus = tonus_paris;
        }
        redde inventa;
    }
    si (causa == NIHIL)
    {
        redde NIHIL;
    }
    chorda_aedificator_appendere_literis(causa, "tonus ");
    chorda_aedificator_appendere_chorda(causa, tonus_modi->modus);
    si (vacua)
    {
        chorda_aedificator_appendere_literis(causa, pares == ZEPHYRUM
            ? " nullam terminationem habet" : " plures differentias"
            " habet: ");
        _pares_enumerare(causa, tabula, numerus, titulus, differentia,
            PAR_ULLUM);
        redde NIHIL;
    }
    chorda_aedificator_appendere_literis(causa, " differentiam \"");
    chorda_aedificator_appendere_chorda(causa, differentia);
    si (pares > I)
    {
        chorda_aedificator_appendere_literis(causa,
            "\" ambigue habet: ");
        _pares_enumerare(causa, tabula, numerus, titulus, differentia,
            PAR_MAJUSCULAE);
        redde NIHIL;
    }
    chorda_aedificator_appendere_literis(causa,
        "\" non habet; differentiae: ");
    _pares_enumerare(causa, tabula, numerus, titulus, differentia,
        PAR_ULLUM);
    redde NIHIL;
}

constans PsalmodiaClausula*
psalmodia_terminationem_invenire (
      constans PsalmodiaTabula*  tabula,
                        chorda   modus,
                        chorda   differentia,
       constans PsalmodiaTonus** tonus,
             ChordaAedificator*  causa)
{
                    s32  numerus;
     constans character* titulus;
                    i32  longitudo;

    si (tonus != NIHIL)
    {
        *tonus = NIHIL;
    }
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    modus = _praecidere(modus);
    si (   !_modum_signi_legere(modus, &numerus, &titulus, &longitudo)
        || longitudo != modus.mensura)
    {
        si (causa != NIHIL)
        {
            chorda_aedificator_appendere_literis(causa, "tonus \"");
            chorda_aedificator_appendere_chorda(causa, modus);
            chorda_aedificator_appendere_literis(causa, "\" nullus");
        }
        redde NIHIL;
    }
    redde _terminationem_quaerere(tabula, numerus, titulus,
        _praecidere(differentia), tonus, causa);
}

constans PsalmodiaClausula*
psalmodia_signum_invenire (
      constans PsalmodiaTabula*  tabula,
                        chorda   signum,
       constans PsalmodiaTonus** tonus,
             ChordaAedificator*  causa)
{
                    s32  numerus;
     constans character* titulus;
                    i32  longitudo;
                    i32  k = ZEPHYRUM;
                 chorda  reliquum;

    si (tonus != NIHIL)
    {
        *tonus = NIHIL;
    }
    si (tabula == NIHIL)
    {
        redde NIHIL;
    }
    dum (k < signum.mensura && _signi_separator(signum.datum[k]))
    {
        k++;
    }
    signum.datum    += k;
    signum.mensura  -= k;
    signum          = _praecidere(signum);
    si (signum.mensura == ZEPHYRUM)
    {
        si (causa != NIHIL)
        {
            chorda_aedificator_appendere_literis(causa,
                "signum vacuum");
        }
        redde NIHIL;
    }
    si (!_modum_signi_legere(signum, &numerus, &titulus, &longitudo))
    {
        si (causa != NIHIL)
        {
            chorda_aedificator_appendere_literis(causa, "tonus \"");
            chorda_aedificator_appendere_chorda(causa,
                chorda_sectio(signum, ZEPHYRUM, longitudo));
            chorda_aedificator_appendere_literis(causa, "\" nullus");
        }
        redde NIHIL;
    }
    k = longitudo;
    dum (k < signum.mensura && _signi_separator(signum.datum[k]))
    {
        k++;
    }
    reliquum.datum    = signum.datum + k;
    reliquum.mensura  = signum.mensura - k;
    redde _terminationem_quaerere(tabula, numerus, titulus, reliquum,
        tonus, causa);
}

constans PsalmodiaClausula*
psalmodia_mediationem_eligere (
     constans PsalmodiaTonus* tonus,
                         b32  ad_libitum)
{
    constans PsalmodiaClausula* communis = NIHIL;
                           i32  k;

    si (tonus == NIHIL || tonus->numerus_mediationum == ZEPHYRUM)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < tonus->numerus_mediationum; k++)
    {
        constans PsalmodiaClausula* clausula = &tonus->mediationes[k];

        si (clausula->monosyllaba)
        {
            perge;
        }
        si (ad_libitum && clausula->ad_libitum)
        {
            redde clausula;
        }
        si (!clausula->ad_libitum && communis == NIHIL)
        {
            communis = clausula;
        }
    }
    redde communis != NIHIL ? communis : &tonus->mediationes[ZEPHYRUM];
}

/* VERUM si clausula in serie (comparatio indicum, non ordinis) */
interior b32
_in_serie (
    constans PsalmodiaClausula* series,
                           i32  numerus,
    constans PsalmodiaClausula* clausula)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (&series[k] == clausula)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

constans PsalmodiaClausula*
psalmodia_clausulam_monosyllabam (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula)
{
    constans PsalmodiaClausula* series;
                           i32  numerus;
                           i32  k;

    si (tonus == NIHIL || clausula == NIHIL || clausula->monosyllaba)
    {
        redde clausula;
    }
    si (_in_serie(tonus->mediationes, tonus->numerus_mediationum,
            clausula))
    {
        series   = tonus->mediationes;
        numerus  = tonus->numerus_mediationum;
    }
    alioquin si (_in_serie(tonus->terminationes,
                     tonus->numerus_terminationum, clausula))
    {
        series   = tonus->terminationes;
        numerus  = tonus->numerus_terminationum;
    }
    alioquin
    {
        redde clausula;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   series[k].monosyllaba
            && series[k].ad_libitum == clausula->ad_libitum
            && chorda_aequalis(series[k].differentia,
                   clausula->differentia))
        {
            redde &series[k];
        }
    }
    redde clausula;
}
