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
#include "filum.h"
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

    /* 2026: Pascha 5 IV; Ascensio (USA: dominica) 17 V; Cor Iesu 12
     * VI; Baptisma 11 I (Epiphania dominica 4 I); Christus Rex 22 XI;
     * Adventus I 29 XI */
    anch = calendarium_anchorae(cal, MMXXVI);
    CREDO_VERUM(_dies_est(anch->pascha, MMXXVI, FASTI_APRILIS, V));
    CREDO_VERUM(_dies_est(anch->ascensio, MMXXVI, FASTI_MAIUS, XVII));
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
    /* Dominica per annum (dominica Nativitatis in USA semper
     * celebratio est - Sancta Familia, Maria Mater Dei, Epiphania,
     * Baptisma: L2) */
    { MMXXVI, FASTI_OCTOBER, XVIII, ORDO_DOMINICA_COMMUNIS },
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
        CREDO_VERUM(info->celebrationes[0].genus != GENUS_SANCTORALE);
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

/* proprietates per dies omnes 1970-2100 (L5; L1: 2000-2040):
 * principalis numquam NIHIL, ad libitum aut commemorata; ordo eius
 * minimus; color diei eius; 2 XI = Defuncti; in feria privilegiata
 * nulla memoria principalis; sollemnitas diei victa solum a die
 * ordinis I-II (RECENSIO 4f a-c) */
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
    dies         = fasti_dies(MCMLXX, FASTI_IANUARIUS, I);
    finis        = fasti_dies(MMC, FASTI_DECEMBER, XXXI);
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


/* ==================================================
 * Mobilia (liturgia-plan L2: proprium de tempore, USA)
 * ================================================== */

/* anchorae USA: Epiphania dominica 2-8 I; Baptisma dominica sequens,
 * feria II si Epiphania 7 aut 8 I; Ascensio Pascha + XLII; Corpus
 * Christi dominica post Trinitatem; Maria Mater Ecclesiae feria II
 * post Pentecosten; Cor Immaculatum sabbatum post Cor Iesu; Sancta
 * Familia dominica intra octavam Nativitatis aut 30 XII */
interior vacuum
_probare_anchorae_usa (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
          AnchoresMobiles* anch;

    cal = calendarium_creare(c->piscina);

    anch = calendarium_anchorae(cal, MMXXVI);
    CREDO_VERUM(_dies_est(anch->epiphania, MMXXVI, FASTI_IANUARIUS,
        IV));
    CREDO_VERUM(_dies_est(anch->baptisma_domini, MMXXVI,
        FASTI_IANUARIUS, XI));
    CREDO_VERUM(_dies_est(anch->ascensio, MMXXVI, FASTI_MAIUS, XVII));
    CREDO_VERUM(_dies_est(anch->maria_mater_ecclesiae, MMXXVI,
        FASTI_MAIUS, XXV));
    CREDO_VERUM(_dies_est(anch->corpus_christi, MMXXVI, FASTI_IUNIUS,
        VII));
    CREDO_VERUM(_dies_est(anch->cor_iesu, MMXXVI, FASTI_IUNIUS, XII));
    CREDO_VERUM(_dies_est(anch->cor_immaculatum, MMXXVI, FASTI_IUNIUS,
        XIII));
    CREDO_VERUM(_dies_est(anch->sancta_familia, MMXXVI, FASTI_DECEMBER,
        XXVII));

    /* 6 I dominica (2019): Epiphania 6 I, Baptisma dominica 13 I */
    anch = calendarium_anchorae(cal, MMXIX);
    CREDO_VERUM(_dies_est(anch->epiphania, MMXIX, FASTI_IANUARIUS, VI));
    CREDO_VERUM(_dies_est(anch->baptisma_domini, MMXIX, FASTI_IANUARIUS,
        XIII));

    /* Epiphania 7 I (2024): Baptisma feria II 8 I */
    anch = calendarium_anchorae(cal, MMXXIV);
    CREDO_VERUM(_dies_est(anch->epiphania, MMXXIV, FASTI_IANUARIUS,
        VII));
    CREDO_VERUM(_dies_est(anch->baptisma_domini, MMXXIV,
        FASTI_IANUARIUS, VIII));

    /* Epiphania 3 I (2027): Baptisma 10 I; Nativitas sabbato: Sancta
     * Familia dominica 26 XII */
    anch = calendarium_anchorae(cal, MMXXVII);
    CREDO_VERUM(_dies_est(anch->epiphania, MMXXVII, FASTI_IANUARIUS,
        III));
    CREDO_VERUM(_dies_est(anch->baptisma_domini, MMXXVII,
        FASTI_IANUARIUS, X));
    CREDO_VERUM(_dies_est(anch->sancta_familia, MMXXVII, FASTI_DECEMBER,
        XXVI));

    /* Nativitas dominica (2022): nulla dominica in octava - 30 XII */
    anch = calendarium_anchorae(cal, MMXXII);
    CREDO_VERUM(_dies_est(anch->sancta_familia, MMXXII, FASTI_DECEMBER,
        XXX));

    calendarium_destruere(cal);
}

/* celebrationes mobiles: titulus, gradus, color (RECENSIO 4c, F5 F6
 * F8 F11 - hodie fracti) */
hic_manens constans CasusDiei CASUS_MOBILIUM[] = {
    /* Triduum et Hebdomada Sancta 2026 */
    { MMXXVI, FASTI_MARTIUS, XXIX, "Palm Sunday", GRADUS_DOMINICA,
      COLLIT_RUBER },
    { MMXXVI, FASTI_APRILIS, II, "Holy Thursday",
        GRADUS_DIES_PECULIARIS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_APRILIS, III, "Good Friday", GRADUS_DIES_PECULIARIS,
      COLLIT_RUBER },
    { MMXXVI, FASTI_APRILIS, IV, "Holy Saturday",
        GRADUS_DIES_PECULIARIS,
      COLLIT_VIOLACEUS },
    { MMXXVI, FASTI_APRILIS, V, "Easter Sunday", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_FEBRUARIUS, XVIII, "Ash Wednesday",
      GRADUS_DIES_PECULIARIS, COLLIT_VIOLACEUS },
    /* Ascensio dominica (USA); Matthias feria V celebratur */
    { MMXXVI, FASTI_MAIUS, XVII, "Ascension", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_MAIUS, XIV, "St. Matthias", GRADUS_FESTUM,
      COLLIT_RUBER },
    { MMXXVI, FASTI_MAIUS, XXIV, "Pentecost", GRADUS_SOLLEMNITAS,
      COLLIT_RUBER },
    { MMXXVI, FASTI_MAIUS, XXV, "Mary, Mother of the Church",
      GRADUS_MEMORIA, COLLIT_ALBUS },
    { MMXXVI, FASTI_MAIUS, XXXI, "Holy Trinity", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_IUNIUS, VII, "Corpus Christi", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_IUNIUS, XII, "Sacred Heart", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Cor Immaculatum et Antonius (memoriae duae): feria */
    { MMXXVI, FASTI_IUNIUS, XIII, NIHIL, GRADUS_FERIA, COLLIT_VIRIDIS },
    /* Cor Iesu 24 VI 2022: Ioannes Baptista victus (translatio L3) */
    { MMXXII, FASTI_IUNIUS, XXIV, "Sacred Heart", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_NOVEMBER, XXII, "Christ the King",
        GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Nativitas: Sancta Familia, Epiphania, Baptisma */
    { MMXXV, FASTI_DECEMBER, XXVIII, "Holy Family", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXXII, FASTI_DECEMBER, XXX, "Holy Family", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXXVII, FASTI_DECEMBER, XXVI, "Holy Family", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_IANUARIUS, IV, "Epiphany", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVI, FASTI_IANUARIUS, VI, NIHIL, GRADUS_FERIA, COLLIT_ALBUS },
    { MMXXVI, FASTI_IANUARIUS, XI, "Baptism of the Lord", GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXIX, FASTI_IANUARIUS, XIII, "Baptism of the Lord",
        GRADUS_FESTUM,
      COLLIT_ALBUS },
    { MMXXIV, FASTI_IANUARIUS, VIII, "Baptism of the Lord",
      GRADUS_FESTUM, COLLIT_ALBUS }
};

interior vacuum
_probare_mobilia (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;
      constans Celebratio* celeb;

    _casus_probare(c, CASUS_MOBILIUM, (i32)(magnitudo(CASUS_MOBILIUM)
        / magnitudo(CASUS_MOBILIUM[0])));

    cal = calendarium_creare(c->piscina);

    /* dies mobilis IPSE celebratio diei est (genus mobile, primus);
     * titulus diei plenus */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_MARTIUS, XXIX), c->piscina);
    CREDO_NECESSE(info != NIHIL);
    CREDO_AEQUALIS_S32((s32)info->celebrationes[0].genus,
        (s32)GENUS_MOBILE);
    CREDO_CHORDA_AEQUALIS_LITERIS(info->titulus_diei,
        "Palm Sunday of the Passion of the Lord");

    /* memoriae obligatoriae duae: ambae ad libitum, feria princeps */
    info   = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_IUNIUS, XIII), c->piscina);
    celeb  = _celebratio_inventa(info,
        "Immaculate Heart of Mary");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_VERUM(celeb->ad_libitum);
    CREDO_AEQUALIS_S32((s32)celeb->genus, (s32)GENUS_MOBILE);
    celeb = _celebratio_inventa(info, "St. Anthony of Padua");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_VERUM(celeb->ad_libitum);

    /* Epiphania 6 I non iam in sanctorali (USA: mobilis) - 6 I solum
     * memoria ad libitum (Andreas Bessette, L4) */
    CREDO_VERUM(_ordo_sancti(FASTI_IANUARIUS, VI)
        > ORDO_FESTUM_PROPRIUM);
    calendarium_destruere(cal);
}

/* hebdomadae per annum I a dominica in aut ante Baptisma: Baptisma
 * feria II (2024) - dominica 14 I = II; Baptisma dominica (2019) -
 * feria II 14 I = I, psalterium I */
interior vacuum
_probare_hebdomadae_baptismatis (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXIV, FASTI_IANUARIUS,
        IX),
        I);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXXIV, FASTI_IANUARIUS,
        XIV), II);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXIX, FASTI_IANUARIUS,
        XIV),
        I);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXIX, FASTI_IANUARIUS,
        XIV), I);
    CREDO_AEQUALIS_S32(_hebdomada_diei(cal, MMXIX, FASTI_IANUARIUS,
        XXI),
        II);
    CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, fasti_dies(MMXIX,
        FASTI_IANUARIUS, XIII)), (s32)TEMPUS_NATIVITATIS);
    calendarium_destruere(cal);
}


/* ==================================================
 * Translationes (liturgia-plan L3)
 * ================================================== */

/* sollemnitates impeditae in diem liberum proximum (UNLY 60);
 * Ioseph in Hebdomada Sancta et Ioannes Baptista anticipantur
 * (RECENSIO 4b, F4 - hodie fracti) */
hic_manens constans CasusDiei CASUS_TRANSLATIONUM[] = {
    /* Ioseph in dominica Quadragesimae: feria II */
    { MMXXIII, FASTI_MARTIUS, XX, "St. Joseph", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVIII, FASTI_MARTIUS, XX, "St. Joseph", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXXIV, FASTI_MARTIUS, XX, "St. Joseph", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Ioseph in Hebdomada Sancta: sabbatum ante Palmas (2035: non
     * Patricius) */
    { MMVIII, FASTI_MARTIUS, XV, "St. Joseph", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXXV, FASTI_MARTIUS, XVII, "St. Joseph", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Annuntiatio in Hebdomada Sancta, octava, Palmis, Paschate:
     * feria II post Dominicam II Paschae */
    { MMXXIV, FASTI_APRILIS, VIII, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXIX, FASTI_APRILIS, IX, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXXV, FASTI_APRILIS, II, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMVIII, FASTI_MARTIUS, XXXI, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXXVII, FASTI_APRILIS, V, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    { MMXL, FASTI_APRILIS, IX, "Annunciation", GRADUS_SOLLEMNITAS,
      COLLIT_ALBUS },
    /* Immaculata in dominica Adventus: 9 XII */
    { MMXXIV, FASTI_DECEMBER, IX, "Immaculate Conception",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    /* Ioannes Baptista a Corde Iesu (2022) aut Corpore Christi (1973,
     * dominica USA) impeditus: 23 VI */
    { MMXXII, FASTI_IUNIUS, XXIII, "Birth of St. John Baptist",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    { MCMLXXIII, FASTI_IUNIUS, XXIII, "Birth of St. John Baptist",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS },
    /* Petrus et Paulus a Corde Iesu (1973): 30 VI */
    { MCMLXXIII, FASTI_IUNIUS, XXX, "Ss. Peter & Paul",
      GRADUS_SOLLEMNITAS, COLLIT_RUBER }
};

interior vacuum
_probare_translationes (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;
      constans Celebratio* celeb;

    _casus_probare(c, CASUS_TRANSLATIONUM, (i32)(magnitudo(
        CASUS_TRANSLATIONUM) / magnitudo(CASUS_TRANSLATIONUM[0])));

    cal = calendarium_creare(c->piscina);

    /* translata notatur cum die propria; in die impedita abest */
    info   = calendarium_obtinere_diem(cal, fasti_dies(MMXXIII,
        FASTI_MARTIUS, XX), c->piscina);
    celeb  = _celebratio_inventa(info, "St. Joseph");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_VERUM(celeb->translata);
    CREDO_VERUM(_dies_est(celeb->dies_originis, MMXXIII, FASTI_MARTIUS,
        XIX));
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXIII,
        FASTI_MARTIUS, XIX), c->piscina);
    CREDO_VERUM(_celebratio_inventa(info, "St. Joseph") == NIHIL);

    /* non translata: dies propria = dies ipsa */
    info   = calendarium_obtinere_diem(cal, fasti_dies(MMXXV,
        FASTI_MARTIUS, XIX), c->piscina);
    celeb  = _celebratio_inventa(info, "St. Joseph");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_FALSUM(celeb->translata);
    CREDO_VERUM(_dies_est(celeb->dies_originis, MMXXV, FASTI_MARTIUS,
        XIX));

    /* Patricius 2035 sub Ioseph translato: omissus */
    info = calendarium_obtinere_diem(cal, fasti_dies(MMXXXV,
        FASTI_MARTIUS, XVII), c->piscina);
    CREDO_VERUM(_celebratio_inventa(info, "St. Patrick") == NIHIL);
    calendarium_destruere(cal);
}

/* sollemnitas fixa quaeque semel per annum (die sua aut translata),
 * 1970-2100 (RECENSIO 4f d) */
interior vacuum
_probare_sollemnitates_semel (
    CredoContextus* c)
{
    constans SanctoraleDatum* sanct;
          constans character* tituli[XVI];
                         s32  numeri[XVI];
                         i32  n;
                         i32  k;
                         s32  i;
                         s32  j;
                         s32  mensis;
                         s32  dies;
                         s32  numerus;
                         s32  annus;
                         i32  errores;
       CalendariumLiturgicum* cal;
                     Piscina* dierum;
              InformatioDiei* info;
                        Dies  d;

    n = ZEPHYRUM;
    per (mensis = FASTI_IANUARIUS; mensis <= FASTI_DECEMBER; mensis++)
    {
        per (dies = I; dies <= XXXI; dies++)
        {
            sanct = sanctorale_obtinere(mensis, dies, &numerus);
            per (i = ZEPHYRUM; sanct && i < numerus && n < XVI; i++)
            {
                si (sanct[i].gradus == GRADUS_SOLLEMNITAS)
                {
                    tituli[n++] = sanct[i].titulus_brevis;
                }
            }
        }
    }
    CREDO_AEQUALIS_S32((s32)n, IX);

    cal = calendarium_creare(c->piscina);
    dierum = piscina_generare_dynamicum("dies_sollemnitatum",
        M * LXIV);
    errores = ZEPHYRUM;
    CREDO_NECESSE(dierum != NIHIL);
    per (annus = MCMLXX; annus <= MMC; annus++)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            numeri[k] = ZEPHYRUM;
        }
        d = fasti_dies(annus, FASTI_IANUARIUS, I);
        dum (d.annus == annus)
        {
            piscina_vacare(dierum);
            info = calendarium_obtinere_diem(cal, d, dierum);
            per (j = ZEPHYRUM; info && j < info->numerus_celebrationum;
                j++)
            {
                per (k = ZEPHYRUM; k < n; k++)
                {
                    si (   info->celebrationes[j].genus
                        == GENUS_SANCTORALE
                        && chorda_aequalis_literis(
                        info->celebrationes[j].titulus_brevis,
                        tituli[k]))
                    {
                        numeri[k]++;
                    }
                }
            }
            d = fasti_addere_dies(d, I);
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (numeri[k] != I)
            {
                si (errores < V)
                {
                    imprimere("  %d %s: %d\n", (integer)annus,
                        tituli[k],
                        (integer)numeri[k]);
                }
                errores++;
            }
        }
    }
    CREDO_AEQUALIS_S32((s32)errores, ZEPHYRUM);
    piscina_destruere(dierum);
    calendarium_destruere(cal);
}


/* ==================================================
 * Sanctorale contra calendarium USA (liturgia-plan L4)
 * ================================================== */

#define VIA_FONTIS_USA "probationes/fixa/liturgia/usccb_sanctorale.tsv"

/* gradus fontis: -I = "MEMORIA?" (ordo non visibilis: memoria
 * quaelibet), -II = ignotus */
interior s32
_gradus_fontis (
    chorda g)
{
    si (chorda_aequalis_literis(g, "SOLLEMNITAS"))
    {
        redde GRADUS_SOLLEMNITAS;
    }
    si (chorda_aequalis_literis(g, "FESTUM"))
    {
        redde GRADUS_FESTUM;
    }
    si (chorda_aequalis_literis(g, "MEMORIA"))
    {
        redde GRADUS_MEMORIA;
    }
    si (chorda_aequalis_literis(g, "MEMORIA_AD_LIBITUM"))
    {
        redde GRADUS_MEMORIA_AD_LIBITUM;
    }
    si (chorda_aequalis_literis(g, "DEFUNCTORUM"))
    {
        redde GRADUS_DEFUNCTORUM;
    }
    redde chorda_aequalis_literis(g, "MEMORIA?") ? -I : -II;
}

/* color fontis: -I = "-" (non visibilis) */
interior s32
_color_fontis (
    chorda c)
{
    si (chorda_aequalis_literis(c, "ALBUS"))
    {
        redde COLLIT_ALBUS;
    }
    si (chorda_aequalis_literis(c, "RUBER"))
    {
        redde COLLIT_RUBER;
    }
    si (chorda_aequalis_literis(c, "VIOLACEUS"))
    {
        redde COLLIT_VIOLACEUS;
    }
    redde -I;
}

/* introitus sanctoralis diei nondum adhibitus qui lineae congruit */
interior s32
_introitus_congruens (
      constans SanctoraleDatum* sanct,
                           s32  numerus,
                  constans b32* adhibiti,
                           s32  gradus,
                           s32  color)
{
    s32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (   !adhibiti[k]
            && (gradus == -I
                ? (   sanct[k].gradus == GRADUS_MEMORIA
                   || sanct[k].gradus == GRADUS_MEMORIA_AD_LIBITUM)
                : (s32)sanct[k].gradus == gradus)
            && (color < ZEPHYRUM || (s32)sanct[k].color == color))
        {
            redde k;
        }
    }
    redde -I;
}

/* omnis linea fontis (USCCB 2025-2026) introitui uni sui diei
 * congruit (gradus, color) et numerus aequalis: sanctorale = fons;
 * memoria obligatoria propria USA ordinis XI, ceterae X */
interior vacuum
_probare_sanctorale_usa (
    CredoContextus* c)
{
                      chorda  textus;
       chorda_fissio_fructus  lineae;
       chorda_fissio_fructus  campi;
    constans SanctoraleDatum* sanct;
                         b32  adhibiti[VIII];
                         s32  numerus;
                         s32  mensis;
                         s32  dies;
                         s32  k;
                         s32  gradus;
                         s32  color;
                         i32  i;
                         i32  omnes;
                         i32  errores;
                         b32  proprium;

    textus = filum_legere_totum(VIA_FONTIS_USA, c->piscina);
    CREDO_NECESSE(textus.mensura > ZEPHYRUM);
    lineae   = chorda_fissio(textus, '\n', c->piscina);
    omnes    = ZEPHYRUM;
    errores  = ZEPHYRUM;
    mensis   = ZEPHYRUM;
    dies     = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    sanct    = NIHIL;
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        s32 m;
        s32 d;

        si (   lineae.elementa[i].mensura         == ZEPHYRUM
            || lineae.elementa[i].datum[ZEPHYRUM] == '#')
        {
            perge;
        }
        m      = ZEPHYRUM;
        d      = ZEPHYRUM;
        campi  = chorda_fissio(lineae.elementa[i], '\t', c->piscina);
        CREDO_NECESSE(campi.numerus >= VI
            && chorda_ut_s32(campi.elementa[ZEPHYRUM], &m)
            && chorda_ut_s32(campi.elementa[I], &d));
        si (m != mensis || d != dies)
        {
            /* dies novus: introitus diei prioris omnes adhibiti */
            per (k = ZEPHYRUM; sanct && k < numerus; k++)
            {
                si (!adhibiti[k])
                {
                    imprimere("  nostra solum %d-%d: %s\n",
                        (integer)mensis, (integer)dies,
                        sanct[k].titulus);
                    errores++;
                }
            }
            mensis  = m;
            dies    = d;
            sanct   = sanctorale_obtinere(mensis, dies, &numerus);
            per (k = ZEPHYRUM; k < VIII; k++)
            {
                adhibiti[k] = FALSUM;
            }
        }
        omnes++;
        gradus    = _gradus_fontis(campi.elementa[II]);
        color     = _color_fontis(campi.elementa[III]);
        proprium  = chorda_aequalis_literis(campi.elementa[IV], "USA");
        k         = sanct ? _introitus_congruens(sanct, numerus,
            adhibiti,
            gradus, color) : -I;
        si (k < ZEPHYRUM)
        {
            imprimere("  fons solum %d-%d: %.*s\n", (integer)mensis,
                (integer)dies, (integer)campi.elementa[V].mensura,
                (constans character*)campi.elementa[V].datum);
            errores++;
            perge;
        }
        adhibiti[k] = VERUM;
        si (   gradus == GRADUS_MEMORIA
            && sanct[k].ordo != (proprium ? ORDO_MEMORIA_PROPRIA
            : ORDO_MEMORIA_GENERALIS))
        {
            imprimere("  ordo %d-%d: %s (%d)\n", (integer)mensis,
                (integer)dies, sanct[k].titulus,
                (integer)sanct[k].ordo);
            errores++;
        }
    }
    per (k = ZEPHYRUM; sanct && k < numerus; k++)
    {
        si (!adhibiti[k])
        {
            imprimere("  nostra solum %d-%d: %s\n", (integer)mensis,
                (integer)dies, sanct[k].titulus);
            errores++;
        }
    }
    CREDO_AEQUALIS_S32((s32)omnes, sanctorale_numerus());
    CREDO_AEQUALIS_S32((s32)errores, ZEPHYRUM);
}

/* proprium USA: dies precationis pro nascituris (22 I; 23 I si 22 I
 * dominica), Gratiarum Actio (feria V quarta Novembris, ad libitum),
 * 29 VII Martha, Maria et Lazarus */
hic_manens constans CasusDiei CASUS_USA[] = {
    { MMXXVI, FASTI_IANUARIUS, XXII, "Day of Prayer for the Unborn",
      GRADUS_MEMORIA, COLLIT_VIOLACEUS },
    { MMXXIII, FASTI_IANUARIUS, XXIII, "Day of Prayer for the Unborn",
      GRADUS_MEMORIA, COLLIT_VIOLACEUS },
    { MMXXIII, FASTI_IANUARIUS, XXII, NIHIL, GRADUS_DOMINICA,
      COLLIT_VIRIDIS },
    { MMXXVI, FASTI_NOVEMBER, XXVI, NIHIL, GRADUS_FERIA,
      COLLIT_VIRIDIS },
    { MMXXVI, FASTI_IULIUS, XXIX, "Ss. Martha, Mary & Lazarus",
      GRADUS_MEMORIA, COLLIT_ALBUS }
};

interior vacuum
_probare_proprium_usa (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;
           InformatioDiei* info;
      constans Celebratio* celeb;

    _casus_probare(c, CASUS_USA, (i32)(magnitudo(CASUS_USA)
        / magnitudo(CASUS_USA[0])));

    cal = calendarium_creare(c->piscina);
    CREDO_VERUM(_dies_est(calendarium_anchorae(cal,
        MMXXVI)->gratiarum_actio, MMXXVI, FASTI_NOVEMBER, XXVI));
    CREDO_VERUM(_dies_est(calendarium_anchorae(cal,
        MMXXV)->gratiarum_actio, MMXXV, FASTI_NOVEMBER, XXVII));

    /* Gratiarum Actio ad libitum (memoria mobilis) */
    info   = calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_NOVEMBER, XXVI), c->piscina);
    celeb  = _celebratio_inventa(info, "Thanksgiving Day");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_VERUM(celeb->ad_libitum);

    /* 23 I 2023: Vincentius et Mariana Cope cum die precationis
     * translato manent (ad libitum) */
    info   = calendarium_obtinere_diem(cal, fasti_dies(MMXXIII,
        FASTI_IANUARIUS, XXIII), c->piscina);
    celeb  = _celebratio_inventa(info, "St. Vincent");
    CREDO_NECESSE(celeb != NIHIL);
    CREDO_VERUM(celeb->ad_libitum);
    CREDO_VERUM(_celebratio_inventa(info, "St. Marianne Cope")
        != NIHIL);
    calendarium_destruere(cal);
}


/* ==================================================
 * Robustitas et functiones reliquae (liturgia-plan L5)
 * ================================================== */

/* dominium: anni 1970-9999, dies valida, cal non NIHIL */
interior vacuum
_probare_dominium (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_VERUM(calendarium_obtinere_diem(cal, fasti_dies(MCMLXIX,
        FASTI_DECEMBER, XXXI), c->piscina) == NIHIL);
    CREDO_VERUM(calendarium_obtinere_diem(cal, fasti_dies(MCMLXX,
        FASTI_IANUARIUS, I), c->piscina) != NIHIL);
    CREDO_VERUM(calendarium_obtinere_diem(cal, fasti_dies(IX * M
        + CMXCIX,
        FASTI_DECEMBER, XXXI), c->piscina) != NIHIL);
    CREDO_VERUM(calendarium_obtinere_diem(cal, fasti_dies(X * M,
        FASTI_IANUARIUS, I), c->piscina) == NIHIL);
    CREDO_VERUM(calendarium_obtinere_diem(cal, fasti_dies(MMXXVI,
        FASTI_FEBRUARIUS, XXX), c->piscina) == NIHIL);
    CREDO_VERUM(calendarium_obtinere_diem(NIHIL, fasti_dies(MMXXVI,
        FASTI_FEBRUARIUS, I), c->piscina) == NIHIL);
    calendarium_destruere(cal);
}

/* dies de praecepto USA (USCCB 2025-2026): 15 VIII et 1 XI sabbato
 * non; 8 XII feria II manet; 9 XII translata non; 1 I feria II non;
 * Nativitas semper; dominica ordinaria non */
interior vacuum
_probare_obligatio (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXVI,
        FASTI_AUGUSTUS, XV)));
    CREDO_VERUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXV,
        FASTI_AUGUSTUS, XV)));
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXV,
        FASTI_NOVEMBER, I)));
    CREDO_VERUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXVI,
        FASTI_NOVEMBER, I)));
    CREDO_VERUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXV,
        FASTI_DECEMBER, VIII)));
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, VIII)));
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXIV,
        FASTI_DECEMBER, IX)));
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXIV,
        FASTI_IANUARIUS, I)));
    CREDO_VERUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXVI,
        FASTI_IANUARIUS, I)));
    CREDO_VERUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXII,
        FASTI_DECEMBER, XXV)));
    CREDO_FALSUM(calendarium_dies_obligationis(cal, fasti_dies(MMXXVI,
        FASTI_OCTOBER, XVIII)));
    CREDO_FALSUM(calendarium_dies_obligationis(NIHIL, fasti_dies(MMXXV,
        FASTI_DECEMBER, XXV)));
    calendarium_destruere(cal);
}

/* nomina celebrationum ex obtinere_diem (F14): translatae et victae
 * absunt; feria sine celebratione vacua */
interior vacuum
_probare_nomen_celebrationum (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(calendarium_nomen_celebrationum(cal,
        fasti_dies(MMXXIX, FASTI_MARTIUS, XXV), c->piscina),
        "Palm Sunday of the Passion of the Lord");
    CREDO_CHORDA_AEQUALIS_LITERIS(calendarium_nomen_celebrationum(cal,
        fasti_dies(MMXXVI, FASTI_MAIUS, XIV), c->piscina),
        "Saint Matthias the Apostle");
    CREDO_CHORDA_AEQUALIS_LITERIS(calendarium_nomen_celebrationum(cal,
        fasti_dies(MMXXVI, FASTI_IANUARIUS, XX), c->piscina),
        "Saint Fabian, Saint Sebastian");
    CREDO_AEQUALIS_S32((s32)calendarium_nomen_celebrationum(cal,
        fasti_dies(MMXXVI, FASTI_OCTOBER, XXI), c->piscina).mensura,
        ZEPHYRUM);
    calendarium_destruere(cal);
}

/* invenire festum per titulum: mobile, sanctorale, translatum, dies
 * precationis in feria II (2023), ignotum */
interior vacuum
_probare_invenire_festum (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("Easter Sunday", c->piscina), MMXXVI), MMXXVI,
        FASTI_APRILIS, V));
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("The Ascension of the Lord", c->piscina),
        MMXXVI), MMXXVI, FASTI_MAIUS, XVII));
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("St. Matthias", c->piscina), MMXXVI), MMXXVI,
        FASTI_MAIUS, XIV));
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("St. Joseph", c->piscina), MMXXIII), MMXXIII,
        FASTI_MARTIUS, XX));
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("Day of Prayer for the Unborn", c->piscina),
        MMXXIII), MMXXIII, FASTI_IANUARIUS, XXIII));
    CREDO_VERUM(_dies_est(calendarium_invenire_festum(cal,
        chorda_ex_literis("Saint Nobody", c->piscina), MMXXVI),
        ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    calendarium_destruere(cal);
}

/* psalterium: Cineres ad sabbatum hebdomada IV (F12), dominica I
 * Quadragesimae I */
interior vacuum
_probare_psalterium_cinerum (
    CredoContextus* c)
{
    CalendariumLiturgicum* cal;

    cal = calendarium_creare(c->piscina);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XVIII), IV);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XXI), IV);
    CREDO_AEQUALIS_S32(_psalterium_diei(cal, MMXXVI, FASTI_FEBRUARIUS,
        XXII), I);
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
    { "anchorae_usa", _probare_anchorae_usa, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "mobilia", _probare_mobilia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "hebdomadae_baptismatis", _probare_hebdomadae_baptismatis, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "translationes", _probare_translationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "sollemnitates_semel", _probare_sollemnitates_semel, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "sanctorale_usa", _probare_sanctorale_usa, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "proprium_usa", _probare_proprium_usa, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "dominium", _probare_dominium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "obligatio", _probare_obligatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomen_celebrationum", _probare_nomen_celebrationum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "invenire_festum", _probare_invenire_festum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "psalterium_cinerum", _probare_psalterium_cinerum, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("calendarium", SECTIONES);
}
