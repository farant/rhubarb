/* probatio_atrium.c - Probationes Atrii
 *
 * QUID HAEC PROBATIO PROBAT, ET QUID NON - aperte, ne viriditas
 * plus polliceatur quam praestat:
 *
 *  PROBATUR (hic, sine fenestra):
 *    - atrium_nuntium_discernere: PURA, superficies plena
 *    - atrium_vexilla_legere: lectio argv, cum decipulis suis
 *    - atrium_creare: SEMITAE RECUSATIONIS (omnes ante AppKit
 *      redeunt) et pactum 'causa numquam vacua'
 *    - accessus super NIHIL: nulla ruina
 *
 *  NON PROBATUR HIC: assemblatio ipsa (fenestra+vitrea+speculum+
 *    imperium) et gyrus. Ea fenestram VERAM poscunt, quam suita
 *    automatica aperire non potest. Custodes eorum sunt modi fumi
 *    apparentium qui iam exsistunt - villa -fumus, forum fumus
 *    plenus, mensor_ui -imago/-json. Migratio earum ad atrium EST
 *    probatio integrationis.
 *
 * CUR HOC HONESTUM EST: quod hic probatur est quod tacite falli
 * potest. Assemblatio fracta fenestram vacuam aut ruinam dat -
 * clamosa est. Discretio nuntii falsa strepitum in consola dat
 * quem nemo videt donec culpa vera obtegatur. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "atrium.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


    /* ========================================================
     * PROBARE: atrium_nuntium_discernere - casus vacui
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda n;

        n.datum    = NIHIL;
        n.mensura  = ZEPHYRUM;
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);

        /* datum adest sed mensura ZEPHYRUM: nihil legendum */
        n          = chorda_ex_literis("paratus", piscina);
        n.mensura  = ZEPHYRUM;
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);
    }
}


    /* ========================================================
     * PROBARE: discretio RPC (regula primae litterae)
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda n;

        n = chorda_ex_literis("{\"methodus\":\"x\"}", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_RPC);

        /* obiectum vacuum adhuc obiectum est */
        n = chorda_ex_literis("{}", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_RPC);

        /* REGULA EXACTA: prima littera, non 'JSON alicubi'.
         * Tabulatum obiectum non est - internuntius id reiceret. */
        n = chorda_ex_literis("[1,2]", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);

        /* spatium praecedens: PRIMA littera regit, ergo ignotum.
         * Hoc consilium est, non neglectus - pons noster spatium
         * praecedens numquam mittit, et 'discernere' parsare non
         * debet. */
        n = chorda_ex_literis(" {}", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);
    }
}


    /* ========================================================
     * PROBARE: discretio PARATUS - et chorda NON terminata
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda n;
            i8 tabula[XVI];

        n = chorda_ex_literis("paratus", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_PARATUS);

        /* DECIPULA CHORDAE: chorda NON terminatur zephyro. Hic
         * tabula post 'paratus' pergit; implementatio quae strcmp
         * adhiberet ultra mensuram legeret et FALLERET. */
        memcpy(tabula, "paratusXYZ", X);
        n.datum    = tabula;
        n.mensura  = (i32)VII;
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_PARATUS);

        /* eadem tabula, mensura maior: iam non 'paratus' */
        n.mensura = (i32)VIII;
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);

        /* brevius */
        n = chorda_ex_literis("paratu", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);

        /* litterae maiusculae: NON idem verbum */
        n = chorda_ex_literis("PARATUS", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);

        /* verbum aliud eiusdem mensurae */
        n = chorda_ex_literis("salvete", piscina);
        CREDO_AEQUALIS_I32 ((i32)atrium_nuntium_discernere(n),
                            (i32)ATRIUM_NUNTIUS_IGNOTUS);
    }
}


    /* ========================================================
     * PROBARE: atrium_vexilla_legere
     * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        AtriumConfiguratio  figura;
                 character  a0[] = "app";
                 character  a1[] = "-volumen";
                 character  a2[] = "/tmp/x";
                 character  a3[] = "-vivum";
                 character  a4[] = "-portus";
                 character  a5[] = "8080";
                 character* argv[VI];

        argv[0] = a0; argv[1] = a1; argv[2]  = a2;
        argv[3]                              = a3; argv[4] =
                                                                     a4; argv[5] =
                                                                             a5;

        /* sine vexillis: nihil mutatur */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)III, argv);
        CREDO_FALSUM (figura.vivum);
        CREDO_AEQUALIS_I32 (figura.portus, ZEPHYRUM);

        /* DECIPULA MENSURATA (mensor_ui -json id passus est):
         * vexillum nudum POST argumentum aliud latet nisi omne
         * argv perscrutamur. Hic -vivum quartum est. */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)VI, argv);
        CREDO_VERUM (figura.vivum);
        CREDO_AEQUALIS_I32 (figura.portus, (i32)8080);
    }
}


interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        AtriumConfiguratio  figura;
                 character  a0[] = "app";
                 character  a1[] = "-portus";
                 character  a2[] = "-5";
                 character* argv[III];

        argv[0] = a0; argv[1] = a1; argv[2] = a2;

        /* portus est i32 INSIGNATUS: sine custodia atoi("-5")
         * in numerum immanem verteretur et ligatio causa ignota
         * falleret. Recusamus tacite, ZEPHYRUM relinquentes
         * (= auto-selectus). */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)III, argv);
        CREDO_AEQUALIS_I32 (figura.portus, ZEPHYRUM);

        /* -portus ultimum, sine valore: nulla lectio ultra finem */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)II, argv);
        CREDO_AEQUALIS_I32 (figura.portus, ZEPHYRUM);

        /* argv NIHIL: nulla ruina */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)II, NIHIL);
        CREDO_AEQUALIS_I32 (figura.portus, ZEPHYRUM);

        /* figura NIHIL: nulla ruina */
        atrium_vexilla_legere(NIHIL, (integer)III, argv);
        CREDO_VERUM (VERUM);
    }
}


interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        AtriumConfiguratio  figura;
                 character  a0[] = "app";
                 character  a1[] = "-radix";
                 character  a2[] = "assets";
                 character  a3[] = "-vivum";
                 character* argv[IV];

        argv[0] = a0; argv[1] = a1; argv[2] = a2; argv[3] = a3;

        /* -radix SOLUM: assetum e disco, canalis imperii CLAUSUS.
         * Ea est figura quam laboratorium sub uno vexillo perdidit. */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)III, argv);
        CREDO_VERUM (figura.capsula_radix != NIHIL);
        CREDO_VERUM (strcmp(figura.capsula_radix, "assets")
            == ZEPHYRUM);
        CREDO_FALSUM (figura.vivum);

        /* ambo simul: independentia in utramque partem */
        memset(&figura, 0, magnitudo(figura));
        atrium_vexilla_legere(&figura, (integer)IV, argv);
        CREDO_VERUM (figura.vivum);
        CREDO_VERUM (strcmp(figura.capsula_radix, "assets")
            == ZEPHYRUM);

        /* -vivum SOLUM: capsula VECTA manet (id quod probare vis) */
        memset(&figura, 0, magnitudo(figura));
        argv[1] = a3;   /* "-vivum" */
        atrium_vexilla_legere(&figura, (integer)II, argv);
        CREDO_VERUM (figura.vivum);
        CREDO_VERUM (figura.capsula_radix == NIHIL);

        /* -radix ultimum, sine valore: nulla lectio ultra finem */
        memset(&figura, 0, magnitudo(figura));
        argv[1] = a1;   /* "-radix" restituere */
        atrium_vexilla_legere(&figura, (integer)II, argv);
        CREDO_VERUM (figura.capsula_radix == NIHIL);
    }
}


    /* ========================================================
     * PROBARE: atrium_creare - semitae recusationis
     *
     * OMNES ante AppKit redeunt, ergo hic sine fenestra probari
     * possunt. Pactum: NIHIL redditur ET causa NOMINAT.
     * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        AtriumConfiguratio  figura;
                    Atrium* atrium;
                    chorda  causa;
              CapsulaEmbed  capsula_ficta;

        memset(&capsula_ficta, 0, magnitudo(capsula_ficta));

        /* piscina deest: sola semita quae causam dare NON potest
         * (chorda sedem poscit). Vacuam ponimus ne vocans stalum
         * legat - quod probamus. */
        causa   = chorda_ex_literis("immunditia praecedens", piscina);
        atrium  = atrium_creare(NIHIL, NIHIL, &causa);
        CREDO_NIHIL (atrium);
        CREDO_CHORDA_VACUA (causa);

        /* configuratio deest */
        causa.datum = NIHIL; causa.mensura = ZEPHYRUM;
        atrium = atrium_creare(piscina, NIHIL, &causa);
        CREDO_NIHIL (atrium);
        CREDO_CHORDA_NON_VACUA (causa);

        /* titulus deest */
        memset(&figura, 0, magnitudo(figura));
        figura.capsula  = &capsula_ficta;
        causa.datum     = NIHIL; causa.mensura = ZEPHYRUM;
        atrium = atrium_creare(piscina, &figura, &causa);
        CREDO_NIHIL (atrium);
        CREDO_CHORDA_NON_VACUA (causa);

        /* capsula deest (titulo praesente) */
        memset(&figura, 0, magnitudo(figura));
        figura.titulus  = "probatio";
        causa.datum     = NIHIL; causa.mensura = ZEPHYRUM;
        atrium = atrium_creare(piscina, &figura, &causa);
        CREDO_NIHIL (atrium);
        CREDO_CHORDA_NON_VACUA (causa);

        /* visio VACUA recusatur, ANTE capsulam: sine capsula probatur,
         * ergo AppKit numquam tangitur (NIHIL = sine Visio) */
        memset(&figura, 0, magnitudo(figura));
        figura.titulus  = "probatio";
        figura.visio    = "";
        causa.datum     = NIHIL; causa.mensura = ZEPHYRUM;
        atrium = atrium_creare(piscina, &figura, &causa);
        CREDO_NIHIL (atrium);
        CREDO_VERUM (causa.mensura > ZEPHYRUM
            && strstr(chorda_ut_cstr(causa, piscina), "Visio vacua")
                != NIHIL);

        /* causa NIHIL: recusatio adhuc valet, nulla ruina */
        memset(&figura, 0, magnitudo(figura));
        atrium = atrium_creare(piscina, &figura, NIHIL);
        CREDO_NIHIL (atrium);
    }
}


    /* ========================================================
     * PROBARE: accessus super NIHIL
     *
     * App quae creationem fractam non probavit hic ruinam pateretur
     * loco NIHIL - et ruina in gyro difficilior est ad legendum
     * quam valor nullus.
     * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        CREDO_NIHIL (atrium_internuntius(NIHIL));
        CREDO_NIHIL (atrium_vitrea(NIHIL));
        CREDO_NIHIL (atrium_fenestra(NIHIL));
        CREDO_NIHIL (atrium_arena(NIHIL));
        CREDO_AEQUALIS_I32 (atrium_portus(NIHIL), ZEPHYRUM);
        CREDO_FALSUM (atrium_currendum(NIHIL));
        CREDO_AEQUALIS_I32 (atrium_gressus(NIHIL), ZEPHYRUM);

        /* haec nihil agere DEBENT, non ruere */
        atrium_monstrare(NIHIL);
        atrium_destruere(NIHIL);
        atrium_iniectionem_ponere(NIHIL,
            chorda_ex_literis("window.x=1;", piscina));
        CREDO_VERUM (VERUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "discretionem: vacua", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "discretionem: RPC", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "discretionem: paratus", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vexilla_legere", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vexilla_legere: portus pravus", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vexilla_legere: -radix", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "creare: recusationes", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "accessus super NIHIL", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("atrium", SECTIONES);
}
