/* ==================================================
 * PROBATIO CALENDARIUM LITURGICUM
 * Probationes pro bibliotheca calendarii liturgici
 *
 * liturgia-plan L0 (2026-10-15): sectiones credo II; custodiae quae
 * hodie transeunt (computus contra tabulam editam et computum alterum,
 * anchorae, dies praecedentiae et colorum - RECENSIO 4a-4e) et
 * symmetria limitum temporum (F9).
 * ================================================== */

#include "latina.h"
#include "calendarium_liturgicum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* hebdomada et hebdomada psalterii diei - brevitatis causa */
interior s32
_hebdomada_diei (
    CalendariumLiturgicum* cal,
                      s32  annus,
                      s32  mensis,
                      s32  dies)
{
    redde calendarium_tempus_info(cal,
        fasti_dies(annus, mensis, dies)).hebdomada;
}

interior s32
_psalterium_diei (
    CalendariumLiturgicum* cal,
                      s32  annus,
                      s32  mensis,
                      s32  dies)
{
    redde calendarium_hebdomada_psalterii(cal,
        fasti_dies(annus, mensis, dies));
}

interior b32
_dies_est (
    Dies d,
     s32 annus,
     s32 mensis,
     s32 dies)
{
    si (d.annus != annus || d.mensis != mensis || d.dies != dies)
    {
        imprimere("  dies: %d-%d-%d (exspectata %d-%d-%d)\n",
            (integer)d.annus, (integer)d.mensis, (integer)d.dies,
            (integer)annus, (integer)mensis, (integer)dies);
        redde FALSUM;
    }
    redde VERUM;
}


/* ==================================================
 * Casus dierum: celebratio principalis exspectata
 *
 * titulus_brevis NIHIL = titulus non probatur (dies temporales:
 * tituli in L2 mutantur).
 * ================================================== */

nomen structura {
                    s32  annus;
                    s32  mensis;
                    s32  dies;
     constans character* titulus_brevis;
    GradusCelebrationis  gradus;
        ColorLiturgicus  color;
} CasusDiei;

interior vacuum
_casum_probare (
    CalendariumLiturgicum* cal,
                  Piscina* piscina,
       constans CasusDiei* k)
{
     InformatioDiei* info;
         Celebratio* p;
                b32  bonus;

    info = calendarium_obtinere_diem(cal, fasti_dies(k->annus,
        k->mensis,
        k->dies), piscina);
    CREDO_VERUM(info != NIHIL && info->celebratio_principalis != NIHIL);
    si (!info || !info->celebratio_principalis)
    {
        redde;
    }
    p      = info->celebratio_principalis;
    bonus  = p->gradus == k->gradus && info->color_diei == k->color
        && (!k->titulus_brevis || chorda_aequalis_literis(
        p->titulus_brevis, k->titulus_brevis));
    si (!bonus)
    {
        imprimere("  casus %d-%02d-%02d: '%.*s' gradus %d color %d\n",
            (integer)k->annus, (integer)k->mensis, (integer)k->dies,
            (integer)p->titulus_brevis.mensura,
            (constans character*)p->titulus_brevis.datum,
            (integer)p->gradus, (integer)info->color_diei);
    }
    CREDO_AEQUALIS_S32((s32)p->gradus, (s32)k->gradus);
    CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)k->color);
    si (k->titulus_brevis)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(p->titulus_brevis,
            k->titulus_brevis);
    }
}

interior vacuum
_casus_probare (
         CredoContextus* c,
     constans CasusDiei* tabula,
                    i32  numerus)
{
    CalendariumLiturgicum* cal;
                      i32  i;

    cal = calendarium_creare(c->piscina);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _casum_probare(cal, c->piscina, &tabula[i]);
    }
    calendarium_destruere(cal);
}


/* ==================================================
 * Computus
 * ================================================== */

/* Pascha 2000-2040 ex tabula edita (mensis * C + dies) */
hic_manens constans s32 PASCHA_EDITUM[] = {
    CDXXIII, CDXV, CCCXXXI, CDXX, CDXI,          /* 2000-2004 */
    CCCXXVII, CDXVI, CDVIII, CCCXXIII, CDXII,    /* 2005-2009 */
    CDIV, CDXXIV, CDVIII, CCCXXXI, CDXX,         /* 2010-2014 */
    CDV, CCCXXVII, CDXVI, CDI, CDXXI,            /* 2015-2019 */
    CDXII, CDIV, CDXVII, CDIX, CCCXXXI,          /* 2020-2024 */
    CDXX, CDV, CCCXXVIII, CDXVI, CDI,            /* 2025-2029 */
    CDXXI, CDXIII, CCCXXVIII, CDXVII, CDIX,      /* 2030-2034 */
    CCCXXV, CDXIII, CDV, CDXXV, CDX,             /* 2035-2039 */
    CDI                                          /* 2040 */
};

interior vacuum
_probare_pascha_editum (
    CredoContextus* c)
{
    Dies p;
     s32 annus;
     i32 errores;

    (vacuum)c;
    errores = ZEPHYRUM;
    per (annus = MM; annus <= MMXL; annus++)
    {
        p = fasti_computus(annus);
        si (!_dies_est(p, annus, PASCHA_EDITUM[annus - MM] / C,
            PASCHA_EDITUM[annus - MM] % C))
        {
            errores++;
        }
    }
    CREDO_AEQUALIS_S32((s32)errores, ZEPHYRUM);
}

/* computus alter (Oudin 1940), a fasti_computus independens: mensis
 * et dies Paschatis Gregoriani */
interior vacuum
_pascha_alterum (
     s32  annus,
     s32* mensis,
     s32* dies)
{
    s32 centuria;
    s32 aureus;
    s32 correctio;
    s32 epacta;
    s32 feria;
    s32 distantia;

    centuria   = annus / C;
    aureus     = annus - XIX * (annus / XIX);
    correctio  = (centuria - XVII) / XXV;
    epacta     = centuria - centuria / IV - (centuria - correctio) / III
        + XIX * aureus + XV;
    epacta     = epacta - XXX * (epacta / XXX);
    epacta     = epacta - (epacta / XXVIII) * (I - (epacta / XXVIII)
        * (XXIX / (epacta + I)) * ((XXI - aureus) / XI));
    feria      = annus + annus / IV + epacta + II - centuria + centuria
        / IV;
    feria      = feria - VII * (feria / VII);
    distantia  = epacta - feria;
    *mensis    = III + (distantia + XL) / XLIV;
    *dies      = distantia + XXVIII - XXXI * (*mensis / IV);
}

interior vacuum
_probare_pascha_alterum (
    CredoContextus* c)
{
    Dies p;
     s32 annus;
     s32 mensis;
     s32 dies;
     i32 errores;

    (vacuum)c;
    errores = ZEPHYRUM;
    per (annus = MDLXXXIII; annus <= IV * M + XCIX; annus++)
    {
        p = fasti_computus(annus);
        _pascha_alterum(annus, &mensis, &dies);
        si (!_dies_est(p, annus, mensis, dies))
        {
            errores++;
        }
    }
    CREDO_AEQUALIS_S32((s32)errores, ZEPHYRUM);
}

/* extrema: 22 III (1818, 2285), 25 IV (1943, 2038); 19 IV non 26 IV
 * (1981, 2076 - correctio Gregoriana) */
interior vacuum
_probare_pascha_extrema (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(_dies_est(fasti_computus(MDCCCXVIII), MDCCCXVIII,
        FASTI_MARTIUS, XXII));
    CREDO_VERUM(_dies_est(fasti_computus(MMCCLXXXV), MMCCLXXXV,
        FASTI_MARTIUS, XXII));
    CREDO_VERUM(_dies_est(fasti_computus(MCMXLIII), MCMXLIII,
        FASTI_APRILIS, XXV));
    CREDO_VERUM(_dies_est(fasti_computus(MMXXXVIII), MMXXXVIII,
        FASTI_APRILIS, XXV));
    CREDO_VERUM(_dies_est(fasti_computus(MCMLXXXI), MCMLXXXI,
        FASTI_APRILIS, XIX));
    CREDO_VERUM(_dies_est(fasti_computus(MMLXXVI), MMLXXVI,
        FASTI_APRILIS, XIX));
}


/* ==================================================
 * Anchorae
 * ================================================== */

interior vacuum
_probare_anchorae (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
          AnchoresMobiles* anch;

    cal = calendarium_creare(c->piscina);

    /* 2024: Pascha 31 III, Cineres 14 II, Pentecoste 19 V, Adventus
     * I 1 XII */
    anch = calendarium_anchorae(cal, MMXXIV);
    CREDO_VERUM(_dies_est(anch->pascha, MMXXIV, FASTI_MARTIUS, XXXI));
    CREDO_VERUM(_dies_est(anch->feria_iv_cinerum, MMXXIV,
        FASTI_FEBRUARIUS, XIV));
    CREDO_VERUM(_dies_est(anch->pentecoste, MMXXIV, FASTI_MAIUS, XIX));
    CREDO_VERUM(_dies_est(anch->dominica_i_adventus, MMXXIV,
        FASTI_DECEMBER, I));

    /* 2025: Pascha 20 IV, Cineres 5 III, Pentecoste 8 VI, Adventus I
     * 30 XI */
    anch = calendarium_anchorae(cal, MMXXV);
    CREDO_VERUM(_dies_est(anch->pascha, MMXXV, FASTI_APRILIS, XX));
    CREDO_VERUM(_dies_est(anch->feria_iv_cinerum, MMXXV, FASTI_MARTIUS,
        V));
    CREDO_VERUM(_dies_est(anch->pentecoste, MMXXV, FASTI_IUNIUS, VIII));
    CREDO_VERUM(_dies_est(anch->dominica_i_adventus, MMXXV,
        FASTI_NOVEMBER, XXX));

    /* 2026: Pascha 5 IV; Ascensio (feria V) 14 V; Cor Iesu 12 VI;
     * Baptisma 11 I (Epiphania feria III); Christus Rex 22 XI;
     * Adventus I 29 XI */
    anch = calendarium_anchorae(cal, MMXXVI);
    CREDO_VERUM(_dies_est(anch->pascha, MMXXVI, FASTI_APRILIS, V));
    CREDO_VERUM(_dies_est(anch->ascensio, MMXXVI, FASTI_MAIUS, XIV));
    CREDO_VERUM(_dies_est(anch->cor_iesu, MMXXVI, FASTI_IUNIUS, XII));
    CREDO_VERUM(_dies_est(anch->baptisma_domini, MMXXVI,
        FASTI_IANUARIUS, XI));
    CREDO_VERUM(_dies_est(anch->christus_rex, MMXXVI, FASTI_NOVEMBER,
        XXII));
    CREDO_VERUM(_dies_est(anch->dominica_i_adventus, MMXXVI,
        FASTI_NOVEMBER, XXIX));

    /* Adventus 2023: vigilia Nativitatis IPSA dominica est, ergo
     * Dominica IV = 24 Dec et Adventus I = 3 Decembris */
    anch = calendarium_anchorae(cal, MMXXIII);
    CREDO_VERUM(_dies_est(anch->dominica_i_adventus, MMXXIII,
        FASTI_DECEMBER, III));

    calendarium_destruere(cal);
}

/* relationes anchorarum (Pascha praecox, serotinum, extremum): Cineres
 * = Pascha - XLVI feria IV; Pentecoste = Pascha + XLIX; Adventus I
 * dominica inter 27 XI et 3 XII; Christus Rex = Adventus I - VII */
interior vacuum
_probare_anchorae_relationes (
    CredoContextus* c)
{
    hic_manens constans s32 ANNI[] = {
        MMVIII, MMXI, MMXXXVIII, MMCCLXXXV
    };
    CalendariumLiturgicum* cal;
          AnchoresMobiles* anch;
                      i32  i;
                     Dies  primus;
                     Dies  ultimus;

    cal = calendarium_creare(c->piscina);
    per (i = ZEPHYRUM; i < (i32)(magnitudo(ANNI) / magnitudo(ANNI[0]));
        i++)
    {
        anch     = calendarium_anchorae(cal, ANNI[i]);
        primus   = fasti_dies(ANNI[i], FASTI_NOVEMBER, XXVII);
        ultimus  = fasti_dies(ANNI[i], FASTI_DECEMBER, III);
        CREDO_AEQUALIS_S32(fasti_comparare(anch->feria_iv_cinerum,
            fasti_addere_dies(anch->pascha, -XLVI)), ZEPHYRUM);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(
            anch->feria_iv_cinerum), FASTI_FERIA_IV);
        CREDO_AEQUALIS_S32(fasti_comparare(anch->pentecoste,
            fasti_addere_dies(anch->pascha, XLIX)), ZEPHYRUM);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(
            anch->dominica_i_adventus), FASTI_DOMINICA);
        CREDO_VERUM(fasti_comparare(anch->dominica_i_adventus, primus)
            >= ZEPHYRUM && fasti_comparare(anch->dominica_i_adventus,
            ultimus) <= ZEPHYRUM);
        CREDO_AEQUALIS_S32(fasti_comparare(anch->christus_rex,
            fasti_addere_dies(anch->dominica_i_adventus, -VII)),
            ZEPHYRUM);
    }
    calendarium_destruere(cal);
}


/* ==================================================
 * Tempora
 * ================================================== */

interior vacuum
_probare_tempora (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
                     Dies  dies;

    cal = calendarium_creare(c->piscina);

    /* Adventus: 1 Dec 2024 = Dominica I; 24 Dec 2024 */
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, I)), (s32)TEMPUS_ADVENTUS);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, XXIV)), (s32)TEMPUS_ADVENTUS);

    /* Nativitas: 25 Dec 2024; 1 Ian 2025 (Maria Mater Dei) */
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, XXV)), (s32)TEMPUS_NATIVITATIS);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_IANUARIUS, I)), (s32)TEMPUS_NATIVITATIS);

    /* Quadragesima: 5 Mar 2025 Cineres; 13 Apr 2025 Palmae */
    dies = fasti_dies(MMXXV, FASTI_MARTIUS, V);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_QUADRAGESIMAE);
    CREDO_VERUM(calendarium_est_feria_cinerum(cal, dies));
    dies = fasti_dies(MMXXV, FASTI_APRILIS, XIII);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_QUADRAGESIMAE);
    CREDO_VERUM(calendarium_est_hebdomada_sancta(cal, dies));

    /* Triduum: 17, 18, 19 Apr 2025 */
    dies = fasti_dies(MMXXV, FASTI_APRILIS, XVII);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_TRIDUUM_SACRUM);
    CREDO_VERUM(calendarium_est_triduum(cal, dies));
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XVIII)), (s32)TEMPUS_TRIDUUM_SACRUM);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XIX)), (s32)TEMPUS_TRIDUUM_SACRUM);

    /* Pascha: 20 Apr 2025, 27 Apr (in Albis), 8 Iun (Pentecoste) */
    dies = fasti_dies(MMXXV, FASTI_APRILIS, XX);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_PASCHALE);
    CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));
    dies = fasti_dies(MMXXV, FASTI_APRILIS, XXVII);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_PASCHALE);
    CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_IUNIUS, VIII)), (s32)TEMPUS_PASCHALE);

    /* per Annum: 15 Ian 2025 (I), 15 Iun 2025 (II) */
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_IANUARIUS, XV)), (s32)TEMPUS_PER_ANNUM_I);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXXV,
        FASTI_IUNIUS, XV)), (s32)TEMPUS_PER_ANNUM_II);

    calendarium_destruere(cal);
}

/* limites temporum anni liturgici N (F9): finis(t) + I = initium
 * (t + I) per totum annum, et finis(per annum II, N) + I = initium
 * (Adventus, N + I). 2026: per annum II finit 28 XI 2026 (Adventus I
 * 29 XI 2026 annum 2027 incipit). */
interior vacuum
_probare_limites_temporum (
    CredoContextus* c)
{
    hic_manens constans s32  ANNI[] = { MMXXV, MMXXVI, MMXXXVIII };
      CalendariumLiturgicum* cal;
                        i32  i;
                        s32  t;
                       Dies  finis;
                       Dies  initium;

    cal = calendarium_creare(c->piscina);
    per (i = ZEPHYRUM; i < (i32)(magnitudo(ANNI) / magnitudo(ANNI[0]));
        i++)
    {
        per (t = TEMPUS_ADVENTUS; t < TEMPUS_PER_ANNUM_II; t++)
        {
            finis    = calendarium_finis_temporis(cal,
                (TempusLiturgicum)t, ANNI[i]);
            initium  = calendarium_initium_temporis(cal,
                (TempusLiturgicum)(t + I), ANNI[i]);
            si (fasti_comparare(fasti_addere_dies(finis, I), initium)
                != ZEPHYRUM)
            {
                imprimere("  annus %d tempus %d\n", (integer)ANNI[i],
                    (integer)t);
            }
            CREDO_AEQUALIS_S32(fasti_comparare(fasti_addere_dies(finis,
                I), initium), ZEPHYRUM);
        }
        finis    = calendarium_finis_temporis(cal, TEMPUS_PER_ANNUM_II,
            ANNI[i]);
        initium  = calendarium_initium_temporis(cal, TEMPUS_ADVENTUS,
            ANNI[i] + I);
        CREDO_VERUM(_dies_est(fasti_addere_dies(finis, I),
            initium.annus,
            initium.mensis, initium.dies));
    }
    CREDO_VERUM(_dies_est(calendarium_finis_temporis(cal,
        TEMPUS_PER_ANNUM_II, MMXXVI), MMXXVI, FASTI_NOVEMBER, XXVIII));
    calendarium_destruere(cal);
}


/* ==================================================
 * Hebdomadae (numerus hebdomadae et psalterii)
 *
 * Dies noti sine hac bibliotheca. Post Pentecosten numeratio
 * RETRORSUM fit: hebdomada Christi Regis est XXXIV, et hebdomada
 * psalterii ex hebdomada rotat.
 * ================================================== */

interior vacuum
_probare_hebdomadae (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
                     Dies  dies;
                   chorda  titulus;

    cal = calendarium_creare(c->piscina);

    /* Tempus per annum I: 12 Ian 2026 = feria II hebdomadae I; 18 Ian
     * = Dominica II; 17 Feb (ante Cineres) = VI */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_IANUARIUS,
        XII), I);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_IANUARIUS,
        XVIII), II);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XVII), VI);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XVII), II);

    /* Quadragesima et Pascha 2026 */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XXII), I);
    titulus = calendarium_formare_titulum(cal, fasti_dies(MMXXVI,
        FASTI_FEBRUARIUS, XXII), c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica I Quadragesimae");
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_APRILIS, XII),
        II);
    titulus = calendarium_formare_titulum(cal, fasti_dies(MMXXVI,
        FASTI_APRILIS, XII), c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica II Paschae");

    /* Adventus: 21 Dec 2025 = Dominica IV; 29 Nov 2026 = I */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXV, FASTI_DECEMBER, XXI),
        IV);
    dies = fasti_dies(MMXXVI, FASTI_NOVEMBER, XXIX);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_ADVENTUS);
    CREDO_AEQUALIS_S32(calendarium_tempus_info(cal, dies).hebdomada, I);
    titulus = calendarium_formare_titulum(cal, dies, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica I Adventus");

    /* Tempus per annum II. Ultima dies ante Adventum 2026 */
    dies = fasti_dies(MMXXVI, FASTI_NOVEMBER, XXVIII);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
        (s32)TEMPUS_PER_ANNUM_II);
    CREDO_AEQUALIS_S32(calendarium_tempus_info(cal, dies).hebdomada,
        XXXIV);

    /* Feria II post Pentecosten: 2024 = VII, 2025 = X, 2026 = VIII
     * (psalterium IV); Pascha praecox 2008 = VI, serotinum 2038 = XI */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXIV, FASTI_MAIUS, XX),
        VII);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXV, FASTI_IUNIUS, IX),
        X);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_MAIUS, XXV),
        VIII);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_MAIUS, XXV),
        IV);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMVIII, FASTI_MAIUS, XII),
        VI);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXXVIII, FASTI_IUNIUS,
        XIV), XI);

    /* ultima feria ante Cineres 2038 (Pascha serotinum): IX */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXXVIII, FASTI_MARTIUS,
        IX), IX);

    /* Septembris 2026: sabbatum 12 = hebdomada XXIII, dominica 13 =
     * XXIV (psalterium IV) */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_SEPTEMBER,
        XII), XXIII);
    titulus = calendarium_formare_titulum(cal, fasti_dies(MMXXVI,
        FASTI_SEPTEMBER, XII), c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Sabbatum, Hebdomada XXIII");
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_SEPTEMBER,
        XIII), XXIV);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_SEPTEMBER,
        XIII), IV);
    titulus = calendarium_formare_titulum(cal, fasti_dies(MMXXVI,
        FASTI_SEPTEMBER, XIII), c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica XXIV per Annum");

    /* Christus Rex = Dominica XXXIV (psalterium II): 23 Nov 2025 et
     * 22 Nov 2026 */
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXV, FASTI_NOVEMBER,
        XXIII), XXXIV);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXVI, FASTI_NOVEMBER,
        XXII), XXXIV);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_NOVEMBER,
        XXII), II);

    calendarium_destruere(cal);
}


/* ==================================================
 * Colores temporum
 * ================================================== */

interior vacuum
_probare_colores (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXIV, FASTI_DECEMBER, I)), (s32)COLLIT_VIOLACEUS);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXIV, FASTI_DECEMBER, XXV)), (s32)COLLIT_ALBUS);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXV, FASTI_IUNIUS, XV)), (s32)COLLIT_VIRIDIS);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXV, FASTI_MARTIUS, X)), (s32)COLLIT_VIOLACEUS);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXV, FASTI_APRILIS, XX)), (s32)COLLIT_ALBUS);
    CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, fasti_dies(
        MMXXV, FASTI_IUNIUS, VIII)), (s32)COLLIT_RUBER);
    calendarium_destruere(cal);
}


/* ==================================================
 * Cycli lectionum
 *
 * Annus liturgicus Dominica I Adventus incipit et nomen anni civilis
 * in quo FINIT fert: 29 Nov 2026 - 28 Nov 2027 est annus 2027 (cyclus
 * B, ferialis I).
 * ================================================== */

interior vacuum
_probare_cycli (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;

    /* 2024 = B, 2025 = C, 2023 = A; 2024 = II (par), 2025 = I */
    CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIV),
        (s32)CYCLUS_B);
    CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXV),
        (s32)CYCLUS_C);
    CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIII),
        (s32)CYCLUS_A);
    CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXIV),
        (s32)CYCLUS_II);
    CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXV),
        (s32)CYCLUS_I);

    cal = calendarium_creare(c->piscina);

    /* 28 Nov 2026, ultima dies anni liturgici 2026: A, II */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_NOVEMBER, XXVIII), c->piscina);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
        (s32)CYCLUS_A);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_quotidianus,
        (s32)CYCLUS_II);

    /* 29 Nov 2026, Dominica I Adventus: annus 2027 - B, I */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_NOVEMBER, XXIX), c->piscina);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
        (s32)CYCLUS_B);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_quotidianus,
        (s32)CYCLUS_I);

    /* 31 Dec 2024 (octava Nativitatis): annus 2025 - C, I */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, XXXI), c->piscina);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
        (s32)CYCLUS_C);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_quotidianus,
        (s32)CYCLUS_I);

    /* 6 Ian 2025: idem annus 2025 - C, I */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXV,
        FASTI_IANUARIUS, VI), c->piscina);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
        (s32)CYCLUS_C);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_quotidianus,
        (s32)CYCLUS_I);

    calendarium_destruere(cal);
}


/* ==================================================
 * Octavae
 * ================================================== */

interior vacuum
_probare_octavae (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_VERUM(calendarium_est_octava_paschae(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XX)));
    CREDO_VERUM(calendarium_est_octava_paschae(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XXVII)));
    CREDO_FALSUM(calendarium_est_octava_paschae(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XXVIII)));
    CREDO_VERUM(calendarium_est_octava_nativitatis(cal, fasti_dies(
        MMXXIV, FASTI_DECEMBER, XXV)));
    CREDO_VERUM(calendarium_est_octava_nativitatis(cal, fasti_dies(
        MMXXV, FASTI_IANUARIUS, I)));
    CREDO_FALSUM(calendarium_est_octava_nativitatis(cal, fasti_dies(
        MMXXV, FASTI_IANUARIUS, II)));
    calendarium_destruere(cal);
}


/* ==================================================
 * Formatatio
 * ================================================== */

interior vacuum
_probare_nomina (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
                   chorda  titulus;

    titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, FALSUM,
        c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Tempus Adventus");
    titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, VERUM,
        c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Adv");
    titulus = calendarium_nomen_coloris(COLLIT_ALBUS, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Albus");

    cal      = calendarium_creare(c->piscina);
    titulus  = calendarium_formare_titulum(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, I), c->piscina);
    CREDO_VERUM(titulus.mensura > ZEPHYRUM);
    calendarium_destruere(cal);
}


/* ==================================================
 * InformatioDiei
 * ================================================== */

interior vacuum
_probare_informatio_diei (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;

    cal   = calendarium_creare(c->piscina);
    info  = calendarium_obtinere_diem(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, I), c->piscina);
    CREDO_NECESSE(info != NIHIL);
    CREDO_VERUM(_dies_est(info->dies, MMXXIV, FASTI_DECEMBER, I));
    CREDO_AEQUALIS_S32((s32)info->tempus_info.tempus,
        (s32)TEMPUS_ADVENTUS);
    CREDO_VERUM(info->tempus_info.est_dominica);
    CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLLIT_VIOLACEUS);
    /* Adventus I 2024 INCIPIT annum liturgicum 2025: cyclus C,
     * ferialis I (olim hic 'CYCLUS_B' - annus civilis) */
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
        (s32)CYCLUS_C);
    CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_quotidianus,
        (s32)CYCLUS_I);
    calendarium_destruere(cal);
}


/* ==================================================
 * Sanctorale
 * ================================================== */

interior vacuum
_probare_sanctorale (
    CredoContextus* c)
{
    constans SanctoraleDatum* sanct;
                         s32  numerus;

    (vacuum)c;
    CREDO_VERUM(sanctorale_numerus() > C);

    /* 1 Ian - Maria Mater Dei (Sollemnitas, dies obligationis) */
    sanct = sanctorale_obtinere(FASTI_IANUARIUS, I, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32(numerus, I);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_AEQUALIS_S32((s32)sanct->color, (s32)COLLIT_ALBUS);
    CREDO_VERUM(sanct->dies_obligationis);

    /* 25 XII, 15 VIII, 1 XI, 8 XII: sollemnitates obligationis */
    sanct = sanctorale_obtinere(FASTI_DECEMBER, XXV, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_VERUM(sanct->dies_obligationis);
    sanct = sanctorale_obtinere(FASTI_AUGUSTUS, XV, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_VERUM(sanct->dies_obligationis);
    sanct = sanctorale_obtinere(FASTI_NOVEMBER, I, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_VERUM(sanct->dies_obligationis);
    sanct = sanctorale_obtinere(FASTI_DECEMBER, VIII, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_VERUM(sanct->dies_obligationis);

    /* 29 VI - Petrus et Paulus (sollemnitas, non obligationis US) */
    sanct = sanctorale_obtinere(FASTI_IUNIUS, XXIX, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
    CREDO_AEQUALIS_S32((s32)sanct->color, (s32)COLLIT_RUBER);
    CREDO_FALSUM(sanct->dies_obligationis);

    /* 20 Jan - Duo sancti (Fabianus et Sebastianus) */
    sanct = sanctorale_obtinere(FASTI_IANUARIUS, XX, &numerus);
    CREDO_VERUM(sanct != NIHIL);
    CREDO_AEQUALIS_S32(numerus, II);

    /* 10 Ian - Nullus sanctus */
    sanct = sanctorale_obtinere(FASTI_IANUARIUS, X, &numerus);
    CREDO_VERUM(sanct == NIHIL);
    CREDO_AEQUALIS_S32(numerus, ZEPHYRUM);
}


/* ==================================================
 * Praecedentia (custodiae: hodie transeunt - RECENSIO 4a-4c)
 * ================================================== */

hic_manens constans CasusDiei CASUS_PRAECEDENTIAE[] = {
    /* memoria per annum celebratur (Thomas Aquinas) */
    { MMXXV, FASTI_IANUARIUS, XXVIII, NIHIL, GRADUS_MEMORIA,
      COLLIT_ALBUS },
    /* Nativitas feria IV */
    { MMXXIV, FASTI_DECEMBER, XXV, NIHIL, GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Ioseph in Quadragesima, feria IV */
    { MMXXV, FASTI_MARTIUS, XIX, NIHIL, GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Cineres */
    { MMXXV, FASTI_MARTIUS, V, NIHIL, GRADUS_DIES_PECULIARIS,
      COLLIT_VIOLACEUS },
    /* Dominica I Adventus */
    { MMXXIV, FASTI_DECEMBER, I, NIHIL, GRADUS_DOMINICA,
      COLLIT_VIOLACEUS },
    /* Marcus in octava Paschae 2025: octava vincit */
    { MMXXV, FASTI_APRILIS, XXV, NIHIL, GRADUS_DIES_PECULIARIS,
      COLLIT_ALBUS },
    /* Marcus 2024 post octavam: festum */
    { MMXXIV, FASTI_APRILIS, XXV, "St. Mark", GRADUS_FESTUM,
      COLLIT_RUBER },
    /* Immaculata in Dominica II Adventus 2024: dominica manet */
    { MMXXIV, FASTI_DECEMBER, VIII, NIHIL, GRADUS_DOMINICA,
      COLLIT_VIOLACEUS },
    /* Guadalupe in Dominica III Adventus 2027 (Gaudete) omittitur */
    { MMXXVII, FASTI_DECEMBER, XII, NIHIL, GRADUS_DOMINICA,
      COLLIT_ROSACEUS },
    /* Lucas in dominica per annum 2026 omittitur (contra F3 nimium) */
    { MMXXVI, FASTI_OCTOBER, XVIII, NIHIL, GRADUS_DOMINICA,
      COLLIT_VIRIDIS },
    /* Cathedra Petri in Dominica I Quadragesimae 2026 omittitur */
    { MMXXVI, FASTI_FEBRUARIUS, XXII, NIHIL, GRADUS_DOMINICA,
      COLLIT_VIOLACEUS },
    /* Cathedra Petri feria II Quadragesimae 2027: festum vincit */
    { MMXXVII, FASTI_FEBRUARIUS, XXII, "Chair of St. Peter",
      GRADUS_FESTUM, COLLIT_ALBUS },
    /* Andreas feria II Adventus 2026 */
    { MMXXVI, FASTI_NOVEMBER, XXX, "St. Andrew", GRADUS_FESTUM,
      COLLIT_RUBER },
    /* Franciscus Xaverius in Adventu ante 17 XII: memoria celebratur */
    { MMXXV, FASTI_DECEMBER, III, "St. Francis Xavier", GRADUS_MEMORIA,
      COLLIT_ALBUS },
    /* feria II Hebdomadae Sanctae */
    { MMXXVI, FASTI_MARTIUS, XXX, NIHIL, GRADUS_DIES_PECULIARIS,
      COLLIT_VIOLACEUS },
    /* Ioseph in Dominica IV Quadragesimae 2023: dominica (Laetare)
     * manet (translatio in L3) */
    { MMXXIII, FASTI_MARTIUS, XIX, NIHIL, GRADUS_DOMINICA,
      COLLIT_ROSACEUS }
};

/* colores dierum signatorum (custodiae) */
hic_manens constans CasusDiei CASUS_COLORUM[] = {
    /* Feria VI in Passione Domini */
    { MMXXVI, FASTI_APRILIS, III, NIHIL, GRADUS_DIES_PECULIARIS,
      COLLIT_RUBER },
    /* Gaudete, Laetare */
    { MMXXVI, FASTI_DECEMBER, XIII, NIHIL, GRADUS_DOMINICA,
      COLLIT_ROSACEUS },
    { MMXXVI, FASTI_MARTIUS, XV, NIHIL, GRADUS_DOMINICA,
      COLLIT_ROSACEUS }
};

interior vacuum
_probare_praecedentia (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;

    _casus_probare(c, CASUS_PRAECEDENTIAE, (i32)(magnitudo(
        CASUS_PRAECEDENTIAE) / magnitudo(CASUS_PRAECEDENTIAE[0])));

    /* memoria per annum: temporale ET sanctorale in celebrationibus */
    cal   = calendarium_creare(c->piscina);
    info  = calendarium_obtinere_diem(cal, fasti_dies(MMXXV,
        FASTI_IANUARIUS, XXVIII), c->piscina);
    CREDO_NECESSE(info != NIHIL);
    CREDO_VERUM(info->numerus_celebrationum >= II);

    /* Triduum vincit omnia (17 IV 2025) */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXV,
        FASTI_APRILIS, XVII), c->piscina);
    CREDO_NECESSE(info != NIHIL && info->celebratio_principalis);
    CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
        (s32)GRADUS_DIES_PECULIARIS);
    calendarium_destruere(cal);
}

interior vacuum
_probare_colores_dierum (
    CredoContextus* c)
{
    _casus_probare(c, CASUS_COLORUM, (i32)(magnitudo(CASUS_COLORUM)
        / magnitudo(CASUS_COLORUM[0])));
}


/* ==================================================
 * Ordo tabulae (liturgia-plan L1)
 * ================================================== */

/* ordo sanctoralis cum gradu congruit */
interior b32
_ordo_congruit (
    GradusCelebrationis gradus,
            OrdoTabulae ordo)
{
    commutatio (gradus)
    {
        casus GRADUS_SOLLEMNITAS:
            redde ordo == ORDO_PRIVILEGIATUS
                || ordo == ORDO_SOLLEMNITAS_GENERALIS
                || ordo == ORDO_SOLLEMNITAS_PROPRIA;
        casus GRADUS_FESTUM:
            redde ordo == ORDO_FESTUM_DOMINI
                || ordo == ORDO_FESTUM_GENERALE
                || ordo == ORDO_FESTUM_PROPRIUM;
        casus GRADUS_MEMORIA:
            redde ordo == ORDO_MEMORIA_GENERALIS
                || ordo == ORDO_MEMORIA_PROPRIA;
        casus GRADUS_MEMORIA_AD_LIBITUM:
            redde ordo == ORDO_MEMORIA_AD_LIBITUM;
        casus GRADUS_DEFUNCTORUM:
            redde ordo == ORDO_SOLLEMNITAS_GENERALIS;
        ordinarius:
            redde FALSUM;
    }
}

/* ordo sanctoralis diei unius (primus introitus) */
interior OrdoTabulae
_ordo_sancti (
    s32 mensis,
    s32 dies)
{
    constans SanctoraleDatum* sanct;
                         s32  numerus;

    sanct = sanctorale_obtinere(mensis, dies, &numerus);
    redde sanct ? sanct->ordo : ORDO_FERIA;
}

/* sanctorale: omnis introitus ordinem congruentem habet; exceptiones
 * nominatae (festa Domini, Nativitas, Epiphania, Defuncti, proprium) */
interior vacuum
_probare_sanctorale_ordo (
    CredoContextus* c)
{
    constans SanctoraleDatum* sanct;
                         s32  numerus;
                         s32  mensis;
                         s32  dies;
                         s32  i;
                         i32  omnes;
                         i32  errores;

    (vacuum)c;
    omnes    = ZEPHYRUM;
    errores  = ZEPHYRUM;
    per (mensis = FASTI_IANUARIUS; mensis <= FASTI_DECEMBER; mensis++)
    {
        per (dies = I; dies <= XXXI; dies++)
        {
            sanct = sanctorale_obtinere(mensis, dies, &numerus);
            per (i = ZEPHYRUM; sanct && i < numerus; i++)
            {
                omnes++;
                si (!_ordo_congruit(sanct[i].gradus, sanct[i].ordo))
                {
                    imprimere("  %d-%d %s: gradus %d ordo %d\n",
                        (integer)mensis, (integer)dies,
                        sanct[i].titulus_brevis,
                        (integer)sanct[i].gradus,
                        (integer)sanct[i].ordo);
                    errores++;
                }
            }
        }
    }
    CREDO_AEQUALIS_S32((s32)omnes, sanctorale_numerus());
    CREDO_AEQUALIS_S32((s32)errores, ZEPHYRUM);

    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_DECEMBER, XXV),
        (s32)ORDO_PRIVILEGIATUS);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_IANUARIUS, VI),
        (s32)ORDO_PRIVILEGIATUS);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_NOVEMBER, I),
        (s32)ORDO_SOLLEMNITAS_GENERALIS);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_FEBRUARIUS, II),
        (s32)ORDO_FESTUM_DOMINI);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_AUGUSTUS, VI),
        (s32)ORDO_FESTUM_DOMINI);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_SEPTEMBER, XIV),
        (s32)ORDO_FESTUM_DOMINI);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_NOVEMBER, IX),
        (s32)ORDO_FESTUM_DOMINI);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_OCTOBER, XVIII),
        (s32)ORDO_FESTUM_GENERALE);
    CREDO_AEQUALIS_S32((s32)_ordo_sancti(FASTI_DECEMBER, XII),
        (s32)ORDO_FESTUM_PROPRIUM);
    sanct = sanctorale_obtinere(FASTI_NOVEMBER, II, &numerus);
    CREDO_NECESSE(sanct != NIHIL);
    CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_DEFUNCTORUM);
    CREDO_AEQUALIS_S32((s32)sanct->ordo,
        (s32)ORDO_SOLLEMNITAS_GENERALIS);
}

/* ordo celebrationis temporalis (celebrationes[0]) */
nomen structura {
            s32 annus;
            s32 mensis;
            s32 dies;
    OrdoTabulae ordo;
} CasusOrdinis;

hic_manens constans CasusOrdinis CASUS_ORDINIS[] = {
    /* Triduum (Feria VI, Sabbatum Sanctum, Dominica Paschae) */
    { MMXXVI, FASTI_APRILIS, III, ORDO_TRIDUUM },
    { MMXXVI, FASTI_APRILIS, IV, ORDO_TRIDUUM },
    { MMXXVI, FASTI_APRILIS, V, ORDO_TRIDUUM },
    /* Feria V Hebdomadae Sanctae (dies; Triduum vespere incipit) */
    { MMXXVI, FASTI_APRILIS, II, ORDO_PRIVILEGIATUS },
    /* Cineres; Hebdomada Sancta; octava Paschae */
    { MMXXVI, FASTI_FEBRUARIUS, XVIII, ORDO_PRIVILEGIATUS },
    { MMXXVI, FASTI_MARTIUS, XXX, ORDO_PRIVILEGIATUS },
    { MMXXVI, FASTI_APRILIS, VII, ORDO_PRIVILEGIATUS },
    /* Dominicae Quadragesimae, Adventus, Paschae */
    { MMXXVI, FASTI_MARTIUS, I, ORDO_PRIVILEGIATUS },
    { MMXXVI, FASTI_DECEMBER, VI, ORDO_PRIVILEGIATUS },
    { MMXXVI, FASTI_MAIUS, III, ORDO_PRIVILEGIATUS },
    /* Dominicae per annum et Nativitatis */
    { MMXXVI, FASTI_OCTOBER, XVIII, ORDO_DOMINICA_COMMUNIS },
    { MMXXV, FASTI_DECEMBER, XXVIII, ORDO_DOMINICA_COMMUNIS },
    /* feriae Quadragesimae, 17-24 XII, octavae Nativitatis */
    { MMXXVI, FASTI_MARTIUS, X, ORDO_FERIA_PRIVILEGIATA },
    { MMXXVI, FASTI_DECEMBER, XXI, ORDO_FERIA_PRIVILEGIATA },
    { MMXXVI, FASTI_DECEMBER, XXIX, ORDO_FERIA_PRIVILEGIATA },
    /* feriae: Adventus ante 17 XII, per annum, Paschales post
     * octavam, Nativitatis post 1 I */
    { MMXXVI, FASTI_DECEMBER, VII, ORDO_FERIA },
    { MMXXVI, FASTI_OCTOBER, XX, ORDO_FERIA },
    { MMXXVI, FASTI_MAIUS, XII, ORDO_FERIA },
    { MMXXVII, FASTI_IANUARIUS, IV, ORDO_FERIA }
};

interior vacuum
_probare_ordo_temporalis (
    CredoContextus* c)
{
      CalendariumLiturgicum* cal;
             InformatioDiei* info;
      constans CasusOrdinis* k;
                        i32  i;

    cal = calendarium_creare(c->piscina);
    per (i = ZEPHYRUM; i < (i32)(magnitudo(CASUS_ORDINIS)
        / magnitudo(CASUS_ORDINIS[0])); i++)
    {
        k     = &CASUS_ORDINIS[i];
        info  = calendarium_obtinere_diem(cal, fasti_dies(k->annus,
            k->mensis, k->dies), c->piscina);
        CREDO_NECESSE(info != NIHIL && info->numerus_celebrationum > 0);
        CREDO_AEQUALIS_S32((s32)info->celebrationes[0].genus,
            (s32)GENUS_TEMPORALE);
        si (info->celebrationes[0].ordo != k->ordo)
        {
            imprimere("  ordo %d-%02d-%02d: %d\n", (integer)k->annus,
                (integer)k->mensis, (integer)k->dies,
                (integer)info->celebrationes[0].ordo);
        }
        CREDO_AEQUALIS_S32((s32)info->celebrationes[0].ordo,
            (s32)k->ordo);
    }
    calendarium_destruere(cal);
}

/* principalis per ordinem (RECENSIO 4a: F1 F2 F3 F7 F13 - hodie
 * fracti) */
hic_manens constans CasusDiei CASUS_ORDINIS_PRINCIPALIS[] = {
    /* F1: sollemnitates in dominica per annum / Nativitatis */
    { MMXXVI, FASTI_NOVEMBER, I, "All Saints", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXX, FASTI_NOVEMBER, I, "All Saints", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXV, FASTI_IUNIUS, XXIX, "Ss. Peter & Paul", GRADUS_SOLLEMNITAS,
      COLLIT_RUBER },
    { MMXXI, FASTI_AUGUSTUS, XV, "Assumption", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXIX, FASTI_IUNIUS, XXIV, "Birth of St. John Baptist",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    { MMXXII, FASTI_DECEMBER, XXV, "Christmas", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXIII, FASTI_IANUARIUS, I, "Mary, Mother of God",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    { MMXXX, FASTI_IANUARIUS, VI, "Epiphany", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* F3: festa Domini in dominica per annum */
    { MMXXV, FASTI_SEPTEMBER, XIV, "Exaltation of Holy Cross",
      GRADUS_FESTUM, COLLIT_RUBER },
    { MMXXV, FASTI_NOVEMBER, IX, "Lateran Basilica", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXX, FASTI_FEBRUARIUS, II, "Presentation", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXXIII, FASTI_AUGUSTUS, VI, "Transfiguration", GRADUS_FESTUM,
      COLLIT_ALBUS },
    /* F2: Defuncti feria II, dominica, sabbato */
    { MMXXVI, FASTI_NOVEMBER, II, "All Souls", GRADUS_DEFUNCTORUM,
      COLLIT_VIOLACEUS },
    { MMXXV, FASTI_NOVEMBER, II, "All Souls", GRADUS_DEFUNCTORUM,
      COLLIT_VIOLACEUS },
    { MMXXIV, FASTI_NOVEMBER, II, "All Souls", GRADUS_DEFUNCTORUM,
      COLLIT_VIOLACEUS },
    /* F7: memoriae in feriis privilegiatis - feria principalis */
    { MMXXV, FASTI_MARTIUS, VII, NIHIL, GRADUS_FERIA,
        COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_FEBRUARIUS, XXIII, NIHIL, GRADUS_FERIA,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_MARTIUS, XVII, NIHIL, GRADUS_FERIA,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_FEBRUARIUS, XXI, NIHIL, GRADUS_FERIA,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_DECEMBER, XXI, NIHIL, GRADUS_FERIA,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_DECEMBER, XXIII, NIHIL, GRADUS_FERIA,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_DECEMBER, XXIX, NIHIL, GRADUS_FERIA, COLLIT_ALBUS },
    { MMXXVI, FASTI_DECEMBER, XXXI, NIHIL, GRADUS_FERIA, COLLIT_ALBUS },
    /* F13: memoriae ad libitum - feria principalis */
    { MMXXVI, FASTI_IANUARIUS, XX, NIHIL, GRADUS_FERIA, COLLIT_VIRIDIS }
};

interior vacuum
_probare_principalis_ordo (
    CredoContextus* c)
{
    _casus_probare(c, CASUS_ORDINIS_PRINCIPALIS, (i32)(magnitudo(
        CASUS_ORDINIS_PRINCIPALIS) / magnitudo(
        CASUS_ORDINIS_PRINCIPALIS[0])));
}

/* celebratio diei per titulum brevem (NIHIL si abest) */
interior constans Celebratio*
_celebratio_inventa (
    constans InformatioDiei* info,
         constans character* titulus_brevis)
{
    s32 i;

    per (i = ZEPHYRUM; info && i < info->numerus_celebrationum; i++)
    {
        si (chorda_aequalis_literis(
            info->celebrationes[i].titulus_brevis, titulus_brevis))
        {
            redde &info->celebrationes[i];
        }
    }
    redde NIHIL;
}

/* commemoratio et ad libitum: memoriae in feria privilegiata manent
 * cum commemoratio; ad libitum notatae; victae omittuntur */
interior vacuum
_probare_commemorationes (
    CredoContextus* c)
{
    hic_manens constans structura {
                       s32  annus;
                       s32  mensis;
                       s32  dies;
        constans character* titulus_brevis;
                       b32  commemoratio;
                       b32  ad_libitum;
    } NOTAE[] = {
        { MMXXV, FASTI_MARTIUS, VII, "Ss. Perpetua & Felicity", VERUM,
          FALSUM },
        { MMXXVI, FASTI_MARTIUS, XVII, "St. Patrick", VERUM, FALSUM },
        { MMXXVI, FASTI_DECEMBER, XXI, "St. Peter Canisius", VERUM,
          FALSUM },
        { MMXXVI, FASTI_DECEMBER, XXIX, "St. Thomas Becket", VERUM,
          FALSUM },
        { MMXXVI, FASTI_IANUARIUS, XX, "St. Fabian", FALSUM, VERUM },
        { MMXXVI, FASTI_IANUARIUS, XX, "St. Sebastian", FALSUM, VERUM },
        { MMXXV, FASTI_DECEMBER, III, "St. Francis Xavier", FALSUM,
          FALSUM }
    };
    CalendariumLiturgicum* cal;
           InformatioDiei* info;
      constans Celebratio* celeb;
                      i32  i;

    cal = calendarium_creare(c->piscina);
    per (i = ZEPHYRUM; i < (i32)(magnitudo(NOTAE)
        / magnitudo(NOTAE[0]));
        i++)
    {
        info   = calendarium_obtinere_diem(cal,
            fasti_dies(NOTAE[i].annus,
            NOTAE[i].mensis, NOTAE[i].dies), c->piscina);
        celeb  = _celebratio_inventa(info, NOTAE[i].titulus_brevis);
        si (!celeb)
        {
            imprimere("  abest: %s\n", NOTAE[i].titulus_brevis);
        }
        CREDO_NECESSE(celeb != NIHIL);
        CREDO_AEQUALIS_S32((s32)celeb->commemoratio,
            (s32)NOTAE[i].commemoratio);
        CREDO_AEQUALIS_S32((s32)celeb->ad_libitum,
            (s32)NOTAE[i].ad_libitum);
    }

    /* Lucas in dominica per annum: victus, omissus; temporale manet */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_OCTOBER, XVIII), c->piscina);
    CREDO_VERUM(_celebratio_inventa(info, "St. Luke") == NIHIL);

    /* Omnes Sancti in dominica: temporale primum manet in indice */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_NOVEMBER, I), c->piscina);
    CREDO_NECESSE(info != NIHIL && info->numerus_celebrationum == II);
    CREDO_AEQUALIS_S32((s32)info->celebrationes[0].genus,
        (s32)GENUS_TEMPORALE);
    calendarium_destruere(cal);
}

/* proprietates per dies omnes 2000-2040: principalis numquam NIHIL,
 * ad libitum aut commemorata; ordo eius minimus; color diei eius;
 * 2 XI = Defuncti; in feria privilegiata nulla memoria principalis;
 * sollemnitas diei victa solum a die ordinis I-II (RECENSIO 4f a-c) */
interior vacuum
_probare_proprietates_ordinis (
    CredoContextus* c)
{
       CalendariumLiturgicum* cal;
                     Piscina* dierum;
              InformatioDiei* info;
         constans Celebratio* p;
                        Dies  dies;
                        Dies  finis;
                         s32  i;
                         i32  violationes;
                         b32  mala;
                         b32  sollemnitas;

    cal = calendarium_creare(c->piscina);
    dierum = piscina_generare_dynamicum("dies_calendarii",
        M * LXIV);
    violationes  = ZEPHYRUM;
    dies         = fasti_dies(MM, FASTI_IANUARIUS, I);
    finis        = fasti_dies(MMXL, FASTI_DECEMBER, XXXI);
    CREDO_NECESSE(dierum != NIHIL);
    dum (fasti_comparare(dies, finis) <= ZEPHYRUM)
    {
        piscina_vacare(dierum);
        info  = calendarium_obtinere_diem(cal, dies, dierum);
        p     = info ? info->celebratio_principalis : NIHIL;
        mala  = !p || p->ad_libitum || p->commemoratio
            || info->color_diei != p->color;
        sollemnitas = FALSUM;
        per (i = ZEPHYRUM; !mala
            && i < info->numerus_celebrationum; i++)
        {
            si (   !info->celebrationes[i].ad_libitum
                && !info->celebrationes[i].commemoratio
                && info->celebrationes[i].ordo < p->ordo)
            {
                mala = VERUM;
            }
            si (info->celebrationes[i].gradus == GRADUS_SOLLEMNITAS)
            {
                sollemnitas = VERUM;
            }
        }
        si (!mala && dies.mensis == FASTI_NOVEMBER && dies.dies == II)
        {
            mala = p->gradus != GRADUS_DEFUNCTORUM;
        }
        si (   !mala && info->celebrationes[0].ordo
            == ORDO_FERIA_PRIVILEGIATA)
        {
            mala = p->gradus == GRADUS_MEMORIA
                || p->gradus == GRADUS_MEMORIA_AD_LIBITUM;
        }
        si (!mala && sollemnitas && p->gradus != GRADUS_SOLLEMNITAS)
        {
            mala = p->ordo > ORDO_PRIVILEGIATUS;
        }
        si (mala)
        {
            si (violationes < V)
            {
                imprimere("  violatio %d-%02d-%02d\n",
                    (integer)dies.annus,
                    (integer)dies.mensis, (integer)dies.dies);
            }
            violationes++;
        }
        dies = fasti_addere_dies(dies, I);
    }
    CREDO_AEQUALIS_S32((s32)violationes, ZEPHYRUM);
    piscina_destruere(dierum);
    calendarium_destruere(cal);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "pascha_editum", _probare_pascha_editum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "pascha_alterum", _probare_pascha_alterum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "pascha_extrema", _probare_pascha_extrema, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "anchorae", _probare_anchorae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anchorae_relationes", _probare_anchorae_relationes, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "tempora", _probare_tempora, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites_temporum", _probare_limites_temporum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "hebdomadae", _probare_hebdomadae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colores", _probare_colores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cycli", _probare_cycli, NIHIL, NIHIL, NIHIL, NIHIL },
    { "octavae", _probare_octavae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomina", _probare_nomina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "informatio_diei", _probare_informatio_diei, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "sanctorale", _probare_sanctorale, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praecedentia", _probare_praecedentia, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "colores_dierum", _probare_colores_dierum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "sanctorale_ordo", _probare_sanctorale_ordo, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "ordo_temporalis", _probare_ordo_temporalis, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "principalis_ordo", _probare_principalis_ordo, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "commemorationes", _probare_commemorationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "proprietates_ordinis", _probare_proprietates_ordinis, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("calendarium", SECTIONES);
}
