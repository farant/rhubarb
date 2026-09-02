/* probatio_vitrea_tabella.c - app secunda vitreae (vectis exitus
 * Phasis C): contra capita publica SOLA scripta (vitrea.h,
 * internuntius.h, fenestra.h, capsula.h) - probat API se ipsam
 * sufficere. Tabella salutationum: status C-lateris (Xar
 * chordarum in piscina appicationis), JS inscribit et relegit.
 *
 * FUMUS SINE OCULIS: pagina in onere 'ave, tabella' sponte
 * inscribit et relegit; tractatores "[tabella] inscriptum" et
 * "[tabella] lectio" imprimunt - gyrus status plenus (JS -> C
 * scribere -> JS -> C legere -> DOM) sine inspectione visuali. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "capsula_tabella.h"
#include <stdio.h>
#include <string.h>

/* status appicationis - datum usoris tractatorum */
nomen structura {
    Piscina* piscina;
        Xar* tituli;   /* Xar chordarum (copiae in piscina) */
} TabellaStatus;


/* ==================================================
 * tractatores
 * ================================================== */

interior JsonValor*
_inscribere (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    TabellaStatus* tabella = (TabellaStatus*)datum;
        JsonValor* fructus = json_objectum_creare(piscina);
           chorda  titulus;

    (vacuum)culpa;
    titulus.mensura  = ZEPHYRUM;
    titulus.datum    = NIHIL;
    si (argumenta != NIHIL)
    {
        titulus = json_ad_chorda(json_objectum_capere(argumenta,
            "titulus"));
    }
    si (titulus.mensura > ZEPHYRUM)
    {
        chorda* situs = (chorda*)xar_addere(tabella->tituli);

        si (situs != NIHIL)
        {
            i8* copia = (i8*)piscina_allocare(tabella->piscina,
                (memoriae_index)titulus.mensura);

            si (copia != NIHIL)
            {
                memcpy(copia, titulus.datum,
                    (memoriae_index)titulus.mensura);
            }
            *situs = chorda_ex_buffer(copia, titulus.mensura);
        }
    }
    imprimere("[tabella] inscriptum (%d): %.*s\n",
        (int)xar_numerus(tabella->tituli), (int)titulus.mensura,
        (constans character*)titulus.datum);
    fflush(stdout);
    json_objectum_ponere(fructus, "numerus",
        json_integer_creare(piscina,
            (s64)xar_numerus(tabella->tituli)));
    redde fructus;
}

interior JsonValor*
_legere (
    JsonValor* argumenta,
      Piscina* piscina,
       vacuum* datum,
       chorda* culpa)
{
    TabellaStatus* tabella  = (TabellaStatus*)datum;
        JsonValor* fructus  = json_objectum_creare(piscina);
        JsonValor* tituli   = json_tabulatum_creare(piscina);
              i32  i;

    (vacuum)argumenta;
    (vacuum)culpa;
    per (i = ZEPHYRUM; i < xar_numerus(tabella->tituli); i++)
    {
        chorda* t = (chorda*)xar_obtinere(tabella->tituli, i);

        si (t != NIHIL)
        {
            json_tabulatum_addere(tituli,
                json_chorda_creare(piscina, *t));
        }
    }
    json_objectum_ponere(fructus, "tituli", tituli);
    json_objectum_ponere(fructus, "numerus",
        json_integer_creare(piscina,
            (s64)xar_numerus(tabella->tituli)));
    imprimere("[tabella] lectio: %d tituli\n",
        (int)xar_numerus(tabella->tituli));
    fflush(stdout);
    redde fructus;
}

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("tabella_vitrea",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "tabella_vocationes", 4194304);
    FenestraConfiguratio  figura_fenestrae;
      VitreaConfiguratio  figura_vitreae;
                Fenestra* fenestra;
                 Capsula* capsula;
                  Vitrea* vitrea;
            Internuntius* inx;
           TabellaStatus  tabella;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    tabella.piscina  = piscina;
    tabella.tituli   = xar_creare(piscina, (i32)magnitudo(chorda));
    si (tabella.tituli == NIHIL)
    {
        redde I;
    }
    figura_fenestrae.titulus   = "tabella salutationum";
    figura_fenestrae.x         = CC;
    figura_fenestrae.y         = CC;
    figura_fenestrae.latitudo  = 560;
    figura_fenestrae.altitudo  = 440;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_tabella, piscina);
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
    (vacuum)internuntius_praebere(inx, "inscribere", _inscribere,
        &tabella);
    (vacuum)internuntius_praebere(inx, "legere", _legere,
        &tabella);

    imprimere("[tabella] app secunda incepta - fenestra aperta\n");
    fflush(stdout);

    dum (!fenestra_debet_claudere(fenestra))
    {
                   Eventus eventus;
                    chorda nuntium;
        VitreaNuntiusGenus genus;
            PiscinaNotatio nota;

        fenestra_expectare_eventus(fenestra, CC);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* eventa fenestralia sola; clavis/mus iam ad texturam
             * pervenerunt (livratio duplex) */
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
                vitrea_recargare(vitrea);
            }
        }
        piscina_reficere(piscina_vocationis, nota);
    }

    imprimere("[tabella] finis: %d tituli inscripti\n",
        (int)xar_numerus(tabella.tituli));
    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
