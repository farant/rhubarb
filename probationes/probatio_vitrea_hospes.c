/* probatio_vitrea_hospes.c - Convergentia: vitrea se-hospitans
 *
 * Hospitium INTRA ansam vitreae pumpatur (gressus 0ms non-obstruens
 * post expectationem eventuum) - filum unum, ansa una, servus et UI.
 *
 * Posturae duae (fumus se-annuntians uterque):
 *   (defalta)  B: pagina capsula:// + fetch trans-originem ad
 *              http://127.0.0.1:<portus> - succedit SOLUM si ACAO
 *              emittitur = re-probatio viva verdicti calibrationis
 *   -url       A: pagina IPSA ex hospitio servita (VITREA_ORIGO_URL,
 *              eadem-origo) = postura modus-dev
 *
 * USUS:  ./bin/probatio_vitrea_hospes [-url]
 */

#include "fenestra.h"
#include "vitrea.h"
#include "internuntius.h"
#include "hospitium.h"
#include "capsula.h"
#include "hospes_assets/capsula_hospes.h"
#include "json.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * TRACTATORES HOSPITII
 * ======================================================================== */

interior vacuum
_salve_tractator(HospitiumColloquium* colloquium)
{
    chorda corpus = chorda_ex_literis("salve ex hospitio",
                                      colloquium_piscina(colloquium));
    colloquium_respondere(colloquium, CC, "text/plain", corpus);
}


/* ========================================================================
 * TRACTATORES PONTIS
 * ======================================================================== */

interior JsonValor*
_confirmare(JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    chorda textus;

    (vacuum)datum;
    (vacuum)culpa;
    textus.mensura = ZEPHYRUM;
    textus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        textus = json_ad_chorda(json_objectum_capere(argumenta,
            "textus"));
    }
    imprimere("[hospes] confirmatum: %.*s\n", (int)textus.mensura,
        (constans character*)textus.datum);
    fflush(stdout);
    redde json_objectum_creare(piscina);
}

interior JsonValor*
_portum_petere(JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    Hospitium* h = (Hospitium*)datum;
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)culpa;
    json_objectum_ponere(fructus, "portus",
        json_integer_creare(piscina, (s64)hospitium_portus(h)));
    redde fructus;
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32 principale (s32 argc, character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("vitrea_hospes",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "vitrea_hospes_vocationes", 4194304);
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio figura_vitreae;
    HospitiumConfiguratio figura_hospitii;
    Fenestra* fenestra;
    Capsula* capsula = NIHIL;
    Vitrea* vitrea;
    Internuntius* inx;
    Hospitium* hospitium;
    character url[CXXVIII];
    b32 postura_url = (argc > I && strcmp(argv[I], "-url") == 0);

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }

    /* hospitium PRIMUM - portus auto, ACAO (postura dev plena) */
    memset(&figura_hospitii, 0, magnitudo(figura_hospitii));
    figura_hospitii.acao = VERUM;
    figura_hospitii.acta_accessus = VERUM;
    hospitium = hospitium_creare(piscina, &figura_hospitii);
    si (hospitium == NIHIL)
    {
        imprimere("FRACTA: hospitium\n");
        redde I;
    }
    hospitium_praebere(hospitium, HTTP_GET, "/salve",
        _salve_tractator, NIHIL);
    hospitium_directorium_servire(hospitium, "/static",
        "probationes/hospes_assets/servita");

    figura_fenestrae.titulus = postura_url
        ? "vitrea modus-dev (A)" : "vitrea se-hospitans (B)";
    figura_fenestrae.x = CC;
    figura_fenestrae.y = CC;
    figura_fenestrae.latitudo = 640;
    figura_fenestrae.altitudo = 400;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }

    si (postura_url)
    {
        /* postura A: pagina ex hospitio - eadem-origo, sine CORS */
        sprintf(url, "http://127.0.0.1:%u/static/",
            hospitium_portus(hospitium));
        figura_vitreae.origo = VITREA_ORIGO_URL;
        figura_vitreae.capsula = NIHIL;
        figura_vitreae.via_initialis = NIHIL;
        figura_vitreae.url = url;
    }
    alioquin
    {
        /* postura B: capsula + fetch trans-originem (ACAO probatur) */
        capsula = capsula_aperire(&capsula_hospes, piscina);
        si (capsula == NIHIL)
        {
            imprimere("FRACTA: capsula\n");
            redde I;
        }
        figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
        figura_vitreae.capsula = capsula;
        figura_vitreae.via_initialis = "index.html";
        figura_vitreae.url = NIHIL;
    }
    figura_vitreae.inspectabilis = VERUM;

    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        imprimere("FRACTA: vitrea\n");
        redde I;
    }

    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        imprimere("FRACTA: internuntius\n");
        redde I;
    }
    (vacuum)internuntius_praebere(inx, "confirmare", _confirmare,
        NIHIL);
    (vacuum)internuntius_praebere(inx, "portum_petere",
        _portum_petere, hospitium);

    imprimere("[hospes] postura %s - hospitium in portu %u\n",
        postura_url ? "A (modus-dev)" : "B (capsula+ACAO)",
        hospitium_portus(hospitium));
    fflush(stdout);

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus eventus;
        chorda nuntium;
        VitreaNuntiusGenus genus;
        PiscinaNotatio nota;

        /* limes L ms = tectum latentiae servi dum UI dormit
         * (nuntius pontis expergefacit statim, ut in demo) */
        fenestra_expectare_eventus(fenestra, L);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            ;
        }

        /* SERVUS: gyrus non-obstruens intra ansam UI - haec linea
         * est tota convergentia */
        hospitium_gressus(hospitium, 0);

        nota = piscina_notare(piscina_vocationis);
        dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
        {
            si (genus == VITREA_NUNTIUS_PONS)
            {
                internuntius_tractare(inx, nuntium,
                    piscina_vocationis);
            }
            alioquin
            {
                imprimere("[hospes] interitus - recargo\n");
                vitrea_recargare(vitrea);
            }
        }
        piscina_reficere(piscina_vocationis, nota);
    }

    {
        HospitiumFructus fh = hospitium_fructus(hospitium);
        imprimere("[hospes] finis: petitiones %ld, responsa %ld\n",
            (longus)fh.petitiones_tractatae, (longus)fh.responsa_missa);
    }

    hospitium_destruere(hospitium);
    redde ZEPHYRUM;
}
