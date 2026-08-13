/*
 * specimen.c - Regressio visualis
 *
 * Duo strata:
 *   specimen_iudicare - PURUM (imago intrat, sententia exit). Sine
 *                       applicatione probari potest.
 *   specimen_manus    - capit, secat, deinde iudicare vocat.
 *
 * Sectio hic PROPRIA est, non imago_extrahere_et_scalare: ille
 * scalatorem semper agit, et cursus bilinearis etiam I:I pixela
 * mutare potest. In collatione exemplarium id ipsum vetitum est.
 */

#include "specimen.h"
#include "imago.h"       /* decodificator exemplarium (stb_image) */
#include "imago_png.h"
#include <stdio.h>
#include <string.h>

#define SUFFIXUM_EXEMPLARIS   ".png"
#define SUFFIXUM_CANDIDATI    ".novum.png"
#define SUFFIXUM_DIFFERENTIAE ".differentia.png"

/* ============================================================
 * Auxilia: semitae
 * ============================================================ */

interior character*
_viam_struere (Piscina*            piscina,
               constans character* scrinium,
               constans character* titulus,
               constans character* suffixum)
{
    size_t     m_scrinii, m_tituli, m_suffixi;
    character* via;

    m_scrinii  = strlen(scrinium);
    m_tituli   = strlen(titulus);
    m_suffixi  = strlen(suffixum);

    via = (character*)piscina_allocare(
              piscina, (memoriae_index)(m_scrinii + m_tituli + m_suffixi + II));
    si (via == NIHIL)
    {
        redde NIHIL;
    }

    memcpy(via, scrinium, m_scrinii);
    via[m_scrinii] = '/';
    memcpy(via + m_scrinii + I, titulus, m_tituli);
    memcpy(via + m_scrinii + I + m_tituli, suffixum, m_suffixi);
    via[m_scrinii + I + m_tituli + m_suffixi] = '\0';

    redde via;
}

/*
 * _titulus_validus - Titulus semitam continere NON debet
 *
 * Titulus in semitam vertitur; si '/' aut '..' ferremus, probatio
 * extra scrinium scribere posset.
 */
interior b32
_titulus_validus (constans character* titulus)
{
    size_t i, m;

    si (titulus == NIHIL)
    {
        redde FALSUM;
    }

    m = strlen(titulus);
    si (m == 0)
    {
        redde FALSUM;
    }

    per (i = 0; i < m; i++)
    {
        si (titulus[i] == '/' || titulus[i] == '\\')
        {
            redde FALSUM;
        }
        si (titulus[i] == '.' && titulus[i + I] == '.')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ============================================================
 * Auxilia: sectio EXACTA (sine scalatore)
 * ============================================================ */

interior Imago
_secare (constans Imago* fons, i32 x, i32 y, i32 lat, i32 alt,
         Piscina* piscina)
{
    Imago nova;
    i32   versus;

    nova.pixela   = NIHIL;
    nova.latitudo = ZEPHYRUM;
    nova.altitudo = ZEPHYRUM;

    si (fons == NIHIL || fons->pixela == NIHIL)
    {
        redde nova;
    }

    /* Ad limites imaginis coercere */
    si (x >= fons->latitudo || y >= fons->altitudo)
    {
        redde nova;
    }
    si (x + lat > fons->latitudo)
    {
        lat = fons->latitudo - x;
    }
    si (y + alt > fons->altitudo)
    {
        alt = fons->altitudo - y;
    }
    si (lat == ZEPHYRUM || alt == ZEPHYRUM)
    {
        redde nova;
    }

    nova.pixela = (i8*)piscina_allocare(
                      piscina, (memoriae_index)(lat * alt * (i32)IV));
    si (nova.pixela == NIHIL)
    {
        redde nova;
    }

    per (versus = ZEPHYRUM; versus < alt; versus++)
    {
        memcpy(nova.pixela + versus * lat * (i32)IV,
               fons->pixela + ((y + versus) * fons->latitudo + x) * (i32)IV,
               (size_t)(lat * (i32)IV));
    }

    nova.latitudo = lat;
    nova.altitudo = alt;

    redde nova;
}

/* ============================================================
 * Auxilia: fructus
 * ============================================================ */

interior SpecimenFructus
_fructus_vacuus (vacuum)
{
    SpecimenFructus f;

    f.sententia               = SPECIMEN_ARGUMENTA_INVALIDA;
    f.collatio_facta          = FALSUM;
    f.via_exemplaris.datum    = NIHIL;
    f.via_exemplaris.mensura  = ZEPHYRUM;
    f.via_candidati.datum     = NIHIL;
    f.via_candidati.mensura   = ZEPHYRUM;
    f.via_differentiae.datum  = NIHIL;
    f.via_differentiae.mensura = ZEPHYRUM;
    f.causa.datum             = NIHIL;
    f.causa.mensura           = ZEPHYRUM;

    /* Fructum collationis ex bibliotheca ipsa sumimus ut omnis
     * campus impleatur, etiam si campus novus additur */
    f.collatio = imago_conferre(NIHIL, NIHIL,
                                collatio_regula_solita(), NIHIL);

    redde f;
}

/* ============================================================
 * Functiones Publicae
 * ============================================================ */

constans character*
specimen_sententia_nomen (SpecimenSententia sententia)
{
    commutatio (sententia)
    {
        casus SPECIMEN_CONGRUIT:            redde "SPECIMEN_CONGRUIT";
        casus SPECIMEN_DISCREPAT:           redde "SPECIMEN_DISCREPAT";
        casus SPECIMEN_EXEMPLAR_ABEST:      redde "SPECIMEN_EXEMPLAR_ABEST";
        casus SPECIMEN_MENSURAE_DISCREPANT: redde "SPECIMEN_MENSURAE_DISCREPANT";
        casus SPECIMEN_ELEMENTUM_ABEST:     redde "SPECIMEN_ELEMENTUM_ABEST";
        casus SPECIMEN_CAPTURA_FRACTA:      redde "SPECIMEN_CAPTURA_FRACTA";
        casus SPECIMEN_SCRINIUM_INACCESSUM: redde "SPECIMEN_SCRINIUM_INACCESSUM";
        casus SPECIMEN_TITULUS_INVALIDUS:   redde "SPECIMEN_TITULUS_INVALIDUS";
        ordinarius:                         redde "SPECIMEN_ARGUMENTA_INVALIDA";
    }
}

SpecimenRegula
specimen_regula_solita (constans character* scrinium)
{
    SpecimenRegula regula;

    regula.scrinium = scrinium;
    regula.collatio = collatio_regula_solita();
    /* Pictura PETITA: ea ipsa causa est cur fractura utilis sit */
    regula.collatio.picturam_facere = VERUM;

    redde regula;
}

SpecimenFructus
specimen_iudicare (constans Imago*     captura,
                   constans character* titulus,
                   SpecimenRegula      regula,
                   Piscina*            piscina)
{
    SpecimenFructus fructus;
    character*      via_exemplaris;
    character*      via_candidati;
    character*      via_differentiae;
    ImagoFructus    exemplar;
    PngFructus      scriptum;

    fructus = _fructus_vacuus();

    si (piscina == NIHIL)
    {
        redde fructus;
    }

    si (captura == NIHIL || captura->pixela == NIHIL ||
        regula.scrinium == NIHIL)
    {
        fructus.causa = chorda_ex_literis("Argumenta invalida", piscina);
        redde fructus;
    }

    si (!_titulus_validus(titulus))
    {
        fructus.sententia = SPECIMEN_TITULUS_INVALIDUS;
        fructus.causa = chorda_ex_literis(
            "Titulus vacuus est aut semitam continet ('/' aut '..')",
            piscina);
        redde fructus;
    }

    via_exemplaris   = _viam_struere(piscina, regula.scrinium, titulus,
                                     SUFFIXUM_EXEMPLARIS);
    via_candidati    = _viam_struere(piscina, regula.scrinium, titulus,
                                     SUFFIXUM_CANDIDATI);
    via_differentiae = _viam_struere(piscina, regula.scrinium, titulus,
                                     SUFFIXUM_DIFFERENTIAE);
    si (via_exemplaris == NIHIL || via_candidati == NIHIL ||
        via_differentiae == NIHIL)
    {
        fructus.causa = chorda_ex_literis("Allocatio fracta", piscina);
        redde fructus;
    }

    fructus.via_exemplaris = chorda_ex_literis(via_exemplaris, piscina);

    exemplar = imago_caricare_ex_file(via_exemplaris, piscina);

    /* ---- Exemplar abest: candidatum scribere et FRANGERE ---- */
    si (!exemplar.successus)
    {
        scriptum = imago_png_scribere(captura, via_candidati, piscina);
        si (!scriptum.successus)
        {
            fructus.sententia = SPECIMEN_SCRINIUM_INACCESSUM;
            fructus.causa = chorda_ex_literis(
                "Exemplar abest ET candidatus scribi non potest - "
                "scrinium abest aut scribi nequit", piscina);
            redde fructus;
        }

        fructus.sententia     = SPECIMEN_EXEMPLAR_ABEST;
        fructus.via_candidati = chorda_ex_literis(via_candidati, piscina);
        fructus.causa = chorda_ex_literis(
            "Exemplar abest. Candidatus scriptus est - eum INSPICE, "
            "deinde super exemplar move ut acceptes.", piscina);
        redde fructus;
    }

    /* ---- Mensurae: fere semper machina, non regressio ---- */
    si (exemplar.imago.latitudo != captura->latitudo ||
        exemplar.imago.altitudo != captura->altitudo)
    {
        scriptum = imago_png_scribere(captura, via_candidati, piscina);
        si (scriptum.successus)
        {
            fructus.via_candidati = chorda_ex_literis(via_candidati, piscina);
        }

        fructus.sententia = SPECIMEN_MENSURAE_DISCREPANT;
        fructus.causa = chorda_ex_literis(
            "Mensurae discrepant - fere semper machina aut scala "
            "schirmatis mutata est, NON regressio", piscina);
        redde fructus;
    }

    /* ---- Collatio vera ---- */
    fructus.collatio = imago_conferre(&exemplar.imago, captura,
                                      regula.collatio, piscina);
    fructus.collatio_facta = VERUM;

    si (fructus.collatio.sententia == COLLATIO_CONGRUUNT)
    {
        /* Artificia vetera tollere: aliter probatio SANATA indicem
         * fracturarum inspiciendarum in `git status` relinquit */
        remove(via_candidati);
        remove(via_differentiae);

        fructus.sententia = SPECIMEN_CONGRUIT;
        redde fructus;
    }

    scriptum = imago_png_scribere(captura, via_candidati, piscina);
    si (scriptum.successus)
    {
        fructus.via_candidati = chorda_ex_literis(via_candidati, piscina);
    }

    si (fructus.collatio.pictura_praesto)
    {
        scriptum = imago_png_scribere(&fructus.collatio.pictura,
                                      via_differentiae, piscina);
        si (scriptum.successus)
        {
            fructus.via_differentiae =
                chorda_ex_literis(via_differentiae, piscina);
        }
    }

    fructus.sententia = SPECIMEN_DISCREPAT;
    fructus.causa = chorda_ex_literis(
        "Captura ab exemplari discrepat - picturam differentiae "
        "INSPICE", piscina);

    redde fructus;
}

/* ============================================================
 * Stratum manus
 * ============================================================ */

/* Numeros signatos ex chorda commatibus separata legere.
 * Chorda NON NUL-terminata est, ergo mensura portanda. */
interior b32
_numeros_legere (chorda fons, s32* exitus, i32 quot)
{
    i32 lecti;
    i32 i;

    lecti = ZEPHYRUM;
    i     = ZEPHYRUM;

    dum (lecti < quot && i < (i32)fons.mensura)
    {
        b32 negativus;
        b32 aliqua;
        s32 valor;

        /* Ad numerum saltare (virgulas, virgulas geminas spernere) */
        dum (i < (i32)fons.mensura &&
             (fons.datum[i] < '0' || fons.datum[i] > '9') &&
             fons.datum[i] != '-')
        {
            i++;
        }
        si (i >= (i32)fons.mensura)
        {
            frange;
        }

        negativus = FALSUM;
        si (fons.datum[i] == '-')
        {
            negativus = VERUM;
            i++;
        }

        valor  = 0;
        aliqua = FALSUM;
        dum (i < (i32)fons.mensura &&
             fons.datum[i] >= '0' && fons.datum[i] <= '9')
        {
            valor = valor * (s32)X + (s32)(fons.datum[i] - '0');
            aliqua = VERUM;
            i++;
        }

        si (!aliqua)
        {
            frange;
        }

        exitus[lecti] = negativus ? -valor : valor;
        lecti++;
    }

    redde (lecti == quot) ? VERUM : FALSUM;
}

SpecimenFructus
specimen_manus (Manus*              manus,
                constans character* titulus,
                constans character* selector,
                SpecimenRegula      regula,
                Piscina*            piscina)
{
    SpecimenFructus fructus;
    character*      via_temporaria;
    ImagoFructus    captura;
    Imago           iudicanda;

    fructus = _fructus_vacuus();

    si (manus == NIHIL || piscina == NIHIL || regula.scrinium == NIHIL)
    {
        redde fructus;
    }

    si (!_titulus_validus(titulus))
    {
        fructus.sententia = SPECIMEN_TITULUS_INVALIDUS;
        fructus.causa = chorda_ex_literis(
            "Titulus vacuus est aut semitam continet", piscina);
        redde fructus;
    }

    via_temporaria = _viam_struere(piscina, "/tmp", titulus,
                                   ".specimen_captura.png");
    si (via_temporaria == NIHIL)
    {
        fructus.causa = chorda_ex_literis("Allocatio fracta", piscina);
        redde fructus;
    }

    si (!manus_imaginem(manus, via_temporaria))
    {
        fructus.sententia = SPECIMEN_CAPTURA_FRACTA;
        fructus.causa = chorda_ex_literis(
            "Imago capi non potuit (manus fracta?)", piscina);
        redde fructus;
    }

    captura = imago_caricare_ex_file(via_temporaria, piscina);
    remove(via_temporaria);

    si (!captura.successus)
    {
        fructus.sententia = SPECIMEN_CAPTURA_FRACTA;
        fructus.causa = chorda_ex_literis(
            "Captura scripta legi non potuit", piscina);
        redde fructus;
    }

    iudicanda = captura.imago;

    /* ---- Sectio ad elementum ---- */
    si (selector != NIHIL)
    {
        character* js;
        chorda     responsum;
        s32        numeri[V];
        size_t     m_selectoris;

        constans character* pars_prima =
            "(function(){var e=document.querySelector('";
        constans character* pars_altera =
            "');if(!e)return '';var r=e.getBoundingClientRect();"
            "return Math.round(r.left)+','+Math.round(r.top)+','"
            "+Math.round(r.width)+','+Math.round(r.height)+','"
            "+window.innerWidth;})()";

        m_selectoris = strlen(selector);
        js = (character*)piscina_allocare(
                 piscina,
                 (memoriae_index)(strlen(pars_prima) + m_selectoris +
                                  strlen(pars_altera) + I));
        si (js == NIHIL)
        {
            fructus.causa = chorda_ex_literis("Allocatio fracta", piscina);
            redde fructus;
        }
        js[0] = '\0';
        strcat(js, pars_prima);
        strcat(js, selector);
        strcat(js, pars_altera);

        responsum = manus_aestimare(manus, js, (Mora)MM);

        si (!_numeros_legere(responsum, numeri, (i32)V) ||
            numeri[II] <= 0 || numeri[III] <= 0 || numeri[IV] <= 0)
        {
            fructus.sententia = SPECIMEN_ELEMENTUM_ABEST;
            fructus.causa = chorda_ex_literis(
                "Selector nihil visibile invenit (aut mensura nulla)",
                piscina);
            redde fructus;
        }

        /* SCALA ex imagine ipsa derivatur, non ex devicePixelRatio
         * praesumpta: machina Retina imaginem duplam parit dum
         * getBoundingClientRect pixela CSS reddit. */
        {
            s32 css_latitudo = numeri[IV];
            s32 x, y, lat, alt;

            x   = (s32)((numeri[0]   * (s32)iudicanda.latitudo) / css_latitudo);
            y   = (s32)((numeri[I]   * (s32)iudicanda.latitudo) / css_latitudo);
            lat = (s32)((numeri[II]  * (s32)iudicanda.latitudo) / css_latitudo);
            alt = (s32)((numeri[III] * (s32)iudicanda.latitudo) / css_latitudo);

            si (x < 0) { x = 0; }
            si (y < 0) { y = 0; }

            iudicanda = _secare(&captura.imago, (i32)x, (i32)y,
                                (i32)lat, (i32)alt, piscina);
            si (iudicanda.pixela == NIHIL)
            {
                fructus.sententia = SPECIMEN_ELEMENTUM_ABEST;
                fructus.causa = chorda_ex_literis(
                    "Elementum extra imaginem iacet aut mensura nulla est",
                    piscina);
                redde fructus;
            }
        }
    }

    redde specimen_iudicare(&iudicanda, titulus, regula, piscina);
}
