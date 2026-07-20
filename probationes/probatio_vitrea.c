/* probatio_vitrea.c - demo salve_vitrea (app GUI; manualiter ex
 * bin/ currenda). Exitus spec-v2 par XI: gyrus rotundus visibilis
 * (bulla premitur -> functio C currit -> DOM renovatur), horologium
 * C pulsat, otium ~0% in pumpa obstructiva.
 *
 * FUMUS SINE OCULIS: pagina in onere 'salve' sponte vocat; tractator
 * C "[vitrea] salve vocatum" imprimit - lancea automatica lineam
 * quaerit et gyrum plenum (oneratio capsulae -> iniectio -> vocare
 * -> postMessage -> cauda + excitatio -> dispatch -> responsum ->
 * promissum solutum) sine inspectione visuali probat. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "capsula_templates.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * tractatores
 * ================================================== */

interior JsonValor*
_salutare (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    JsonValor* fructus = json_objectum_creare(piscina);
    chorda quem;
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        LXIV);

    (vacuum)datum;
    (vacuum)culpa;
    quem.mensura = ZEPHYRUM;
    quem.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        quem = json_ad_chorda(json_objectum_capere(argumenta,
            "quem"));
    }
    chorda_aedificator_appendere_literis(aed, "salve, ");
    chorda_aedificator_appendere_chorda(aed, quem);
    chorda_aedificator_appendere_literis(aed, "!");
    json_objectum_ponere(fructus, "salutatio",
        json_chorda_creare(piscina,
            chorda_aedificator_finire(aed)));
    imprimere("[vitrea] salve vocatum\n");
    fflush(stdout);
    redde fructus;
}

interior JsonValor*
_addere (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    s64 a = ZEPHYRUM;
    s64 b = ZEPHYRUM;
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)datum;
    (vacuum)culpa;
    si (argumenta != NIHIL)
    {
        a = json_ad_integer(json_objectum_capere(argumenta, "a"));
        b = json_ad_integer(json_objectum_capere(argumenta, "b"));
    }
    json_objectum_ponere(fructus, "summa",
        json_integer_creare(piscina, a + b));
    imprimere("[vitrea] addere vocatum\n");
    fflush(stdout);
    redde fructus;
}

/* numeratores vitreae legere - datum usoris = Vitrea* */
interior JsonValor*
_fructus_legere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    Vitrea* vitrea = (Vitrea*)datum;
    VitreaFructus fr = vitrea_fructus(vitrea);
    JsonValor* fructus = json_objectum_creare(piscina);

    (vacuum)argumenta;
    (vacuum)culpa;
    json_objectum_ponere(fructus, "nuntii_recepti",
        json_integer_creare(piscina, (s64)fr.nuntii_recepti));
    json_objectum_ponere(fructus, "nuntii_amissi",
        json_integer_creare(piscina, (s64)fr.nuntii_amissi));
    json_objectum_ponere(fructus, "aestimationes",
        json_integer_creare(piscina, (s64)fr.aestimationes));
    json_objectum_ponere(fructus, "petitiones_capsulae",
        json_integer_creare(piscina, (s64)fr.petitiones_capsulae));
    json_objectum_ponere(fructus, "petitiones_fractae",
        json_integer_creare(piscina, (s64)fr.petitiones_fractae));
    json_objectum_ponere(fructus, "navigationes_recusatae",
        json_integer_creare(piscina,
            (s64)fr.navigationes_recusatae));
    redde fructus;
}

/* vocatur a JS POSTQUAM responsum 'salve' promissum solvit -
 * impressio probat gyrum plenum C->JS->C sine oculis */
interior JsonValor*
_confirmare (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
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
    imprimere("[vitrea] confirmatum: %.*s\n", (int)textus.mensura,
        (constans character*)textus.datum);
    fflush(stdout);
    redde json_objectum_creare(piscina);
}

s32 principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("salve_vitrea",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "salve_vitrea_vocationes", 4194304);
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio figura_vitreae;
    Fenestra* fenestra;
    Capsula* capsula;
    Vitrea* vitrea;
    Internuntius* inx;
    i64 pulsus_horologii;
    f64 frequentia;
    s64 ictus = ZEPHYRUM;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    figura_fenestrae.titulus = "salve vitrea";
    figura_fenestrae.x = CC;
    figura_fenestrae.y = CC;
    figura_fenestrae.latitudo = 640;
    figura_fenestrae.altitudo = 480;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_templates, piscina);
    si (capsula == NIHIL)
    {
        imprimere("FRACTA: capsula\n");
        redde I;
    }
    figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url = NIHIL;
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
    (vacuum)internuntius_praebere(inx, "salve", _salutare, NIHIL);
    (vacuum)internuntius_praebere(inx, "confirmare", _confirmare,
        NIHIL);
    (vacuum)internuntius_praebere(inx, "addere", _addere, NIHIL);
    (vacuum)internuntius_praebere(inx, "fructus_legere",
        _fructus_legere, vitrea);

    imprimere("[vitrea] demo inceptum - fenestra aperta\n");
    fflush(stdout);
    frequentia = fenestra_tempus_obtinere_frequentiam();
    pulsus_horologii = fenestra_tempus_obtinere_pulsus();

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus eventus;
        chorda nuntium;
        VitreaNuntiusGenus genus;
        PiscinaNotatio nota;

        /* pumpa obstructiva: otium ~0%; nuntius pontis eventum
         * syntheticum ponit et nos expergefacit */
        fenestra_expectare_eventus(fenestra, CC);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* eventa fenestralia sola curamus; clavis/mus ad
             * texturam per sendEvent iam pervenerunt (livratio
             * duplex - spec par III) */
        }
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
                imprimere("[vitrea] interitus - recargo\n");
                vitrea_recargare(vitrea);
            }
        }
        /* horologium: pulsus quisque secundus */
        {
            i64 nunc = fenestra_tempus_obtinere_pulsus();

            si ((f64)(nunc - pulsus_horologii) / frequentia >= 1.0)
            {
                JsonValor* datum_horae = json_objectum_creare(
                    piscina_vocationis);
                character hora[XXXII];

                ictus = ictus + I;
                sprintf(hora, "%d s", (int)ictus);
                json_objectum_ponere(datum_horae, "ictus",
                    json_integer_creare(piscina_vocationis,
                        ictus));
                json_objectum_ponere(datum_horae, "hora",
                    json_chorda_creare_literis(piscina_vocationis,
                        hora));
                internuntius_eventum_mittere(inx, "horologium",
                    datum_horae, piscina_vocationis);
                pulsus_horologii = nunc;
            }
        }
        piscina_reficere(piscina_vocationis, nota);
    }

    {
        VitreaFructus fr = vitrea_fructus(vitrea);
        InternuntiusFructus fi = internuntius_fructus(inx);

        imprimere("[vitrea] finis: nuntii %d (amissi %d),"
            " aestimationes %d, petitiones %d (fractae %d),"
            " navigationes recusatae %d;"
            " vocationes %d, culpae %d, eventus %d\n",
            (int)fr.nuntii_recepti, (int)fr.nuntii_amissi,
            (int)fr.aestimationes, (int)fr.petitiones_capsulae,
            (int)fr.petitiones_fractae,
            (int)fr.navigationes_recusatae,
            (int)fi.vocationes_tractatae, (int)fi.culpae,
            (int)fi.eventus_missi);
    }
    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
