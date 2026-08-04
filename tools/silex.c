/* silex.c (instrumentum) - lapis ignarius: proiecta e fabrica
 * excudere. Verba v0: novum, ui (sine argumentis = ui).
 *
 * Usus:
 *   silex                       # fenestra vitrea (ui)
 *   silex ui
 *   silex novum 001 -f /via/ad/rhubarb -d /via/ad/silicetum
 *   SILEX_FABRICA=/via/ad/rhubarb silex novum 001
 *
 * Aedificatio: ./tools/silex_struere.sh (capsula frontis +
 * obiecta suite; compile_tools.sh capsulam nesciret) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "argumenta.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "xar.h"
#include "silex.h"
#include "mensa.h"
#include "silex_assets/capsula_silex_frons.h"

#include <stdio.h>
#include <stdlib.h>

#define SILEX_VERSIO "v0"

/* fabrica in datum tractatoris - NIHIL = ignota (ui tolerat) */
interior JsonValor*
_status_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa);

interior JsonValor*
_status_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    JsonValor*          fructus = json_objectum_creare(piscina);
    constans character* fabrica = (constans character*)datum;
    b32                 valida = FALSUM;

    (vacuum)argumenta;
    (vacuum)culpa;
    si (fabrica != NIHIL)
    {
        character via[1024];

        sprintf(via, "%.1000s/include", fabrica);
        valida = filum_directorium_existit(via);
    }
    json_objectum_ponere(fructus, "versio",
        json_chorda_creare_literis(piscina, SILEX_VERSIO));
    json_objectum_ponere(fructus, "fabrica",
        json_chorda_creare_literis(piscina,
            fabrica == NIHIL ? "(ignota - SILEX_FABRICA pone)"
                : fabrica));
    json_objectum_ponere(fructus, "fabrica_valida",
        json_boolean_creare(piscina, valida));
    redde fructus;
}

interior s32
_ui_currere (Piscina* piscina, constans character* fabrica);

interior s32
_ui_currere (Piscina* piscina, constans character* fabrica)
{
    Piscina* piscina_vocationis;
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio   figura_vitreae;
    Fenestra*     fenestra;
    Capsula*      capsula;
    Vitrea*       vitrea;
    Internuntius* inx;

    piscina_vocationis = piscina_generare_dynamicum(
        "silex_ui_vocationes", 1048576);
    si (piscina_vocationis == NIHIL)
    {
        redde I;
    }
    figura_fenestrae.titulus = "silex";
    figura_fenestrae.x = CC;
    figura_fenestrae.y = CC;
    figura_fenestrae.latitudo = 720;
    figura_fenestrae.altitudo = 560;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        fprintf(stderr, "silex ui: fenestra creari non potuit\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_silex_frons, piscina);
    si (capsula == NIHIL)
    {
        fprintf(stderr, "silex ui: capsula frontis fracta\n");
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
        fprintf(stderr, "silex ui: vitrea creari non potuit\n");
        redde I;
    }
    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        fprintf(stderr, "silex ui: internuntius fractus\n");
        redde I;
    }
    /* exemplar mutabile fabricae - datum tractatoris vacuum* est */
    {
        character* fabrica_datum = NIHIL;

        si (fabrica != NIHIL)
        {
            fabrica_datum = chorda_ut_cstr(
                chorda_ex_literis(fabrica, piscina), piscina);
        }
        (vacuum)internuntius_praebere(inx, "status",
            _status_tractare, (vacuum*)fabrica_datum);
    }

    /* tabula persistens: ~/.rhubarb/silex.volumen (conventio
     * pilae mensae - defectus non fatalis, ui sine memoria vivit) */
    {
        constans character* domus = getenv("HOME");

        si (domus != NIHIL && domus[0] != '\0')
        {
            ChordaAedificator* aed = chorda_aedificator_creare(
                piscina, (memoriae_index)128);

            chorda_aedificator_appendere_literis(aed, domus);
            chorda_aedificator_appendere_literis(aed,
                "/.rhubarb/silex.volumen");
            si (mensa_praebere(inx, piscina, chorda_ut_cstr(
                chorda_aedificator_finire(aed), piscina)) == NIHIL)
            {
                fprintf(stderr, "silex ui: mensa sine memoria"
                    " (volumen status non aperiri potuit)\n");
            }
        }
    }

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus eventus;
        chorda nuntium;
        VitreaNuntiusGenus genus;
        PiscinaNotatio nota;

        fenestra_expectare_eventus(fenestra, CC);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* eventa fenestralia sola; textura clavem/murem iam
             * accepit (livratio duplex) */
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

    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    redde ZEPHYRUM;
}

s32
principale (integer argc, character** argv)
{
    Piscina*            piscina;
    ArgumentaParser*    parser;
    ArgumentaFructus*   lecta;
    chorda              verbum;
    chorda              titulus;
    chorda              fabrica_opt;
    chorda              destinatio_opt;
    constans character* fabrica;
    constans character* destinatio;
    SilexNovumOptiones  optiones;
    SilexNovumFructus   fructus;

    piscina = piscina_generare_dynamicum("silex", 65536);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "silex: piscina generari non potuit\n");
        redde I;
    }

    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "silex - proiecta nova e fabrica rhubarb excudere");
    argumenta_addere_positionalem(parser, "verbum",
        "verbum (novum | ui; sine argumentis = ui)", FALSUM);
    argumenta_addere_positionalem(parser, "titulus",
        "nomen proiecti (pro novo)", FALSUM);
    argumenta_addere_optionem(parser, "-f", "--fabrica",
        "radix arboris rhubarb (aut SILEX_FABRICA)");
    argumenta_addere_optionem(parser, "-d", "--destinatio",
        "directorium parens proiecti (ordinarie '.')");
    argumenta_addere_optionem(parser, "-n", "--nuntius",
        "nuntius conditionis (pro condere)");
    argumenta_addere_exemplum(parser,
        "silex novum 001 -f ~/Documents/projects/rhubarb");

    lecta = argumenta_conari_parsere(parser, (i32)argc,
        (constans character* constans*)argv);
    si (lecta == NIHIL)
    {
        argumenta_imprimere_errorem(parser);
        argumenta_imprimere_auxilium(parser);
        redde I;
    }

    verbum = argumenta_obtinere_positionalem(lecta, 0, piscina);
    titulus = argumenta_obtinere_positionalem(lecta, 1, piscina);

    /* fabrica: optio > ambiens > NIHIL (ui tolerat, novum poscit) */
    fabrica_opt = argumenta_obtinere_optionem(lecta, "--fabrica",
        piscina);
    si (fabrica_opt.mensura > ZEPHYRUM)
    {
        fabrica = chorda_ut_cstr(fabrica_opt, piscina);
    }
    alioquin
    {
        fabrica = getenv("SILEX_FABRICA");
        si (fabrica != NIHIL && fabrica[0] == '\0')
        {
            fabrica = NIHIL;
        }
    }

    /* sine argumentis aut 'ui' = fenestra */
    si (argumenta_numerus_positionalium(lecta) == 0
        || chorda_aequalis_literis(verbum, "ui"))
    {
        redde _ui_currere(piscina, fabrica);
    }

    /* verba VCS: via = positionale secundum (ordinarie ".") */
    si (chorda_aequalis_literis(verbum, "status")
        || chorda_aequalis_literis(verbum, "condere")
        || chorda_aequalis_literis(verbum, "historia"))
    {
        constans character* via_proiecti = titulus.mensura > ZEPHYRUM
            ? chorda_ut_cstr(titulus, piscina) : ".";

        si (chorda_aequalis_literis(verbum, "status"))
        {
            SilexStatusFructus s = silex_status(piscina,
                via_proiecti);
            i32 index;

            si (!s.successus)
            {
                fprintf(stderr, "silex status: %s\n", s.erratum);
                redde I;
            }
            si (xar_numerus(s.res) == 0)
            {
                imprimere("silex status: omnia munda"
                    " (%d plagulae)\n", (integer)s.mundae);
                redde ZEPHYRUM;
            }
            imprimere("silex status: %d mundae, %d aliae\n",
                (integer)s.mundae, (integer)xar_numerus(s.res));
            per (index = 0; index < xar_numerus(s.res);
                index = index + 1)
            {
                SilexStatusRes* r = (SilexStatusRes*)xar_obtinere(
                    s.res, index);
                constans character* signum =
                    r->status == SILEX_PLAGULA_MUTATA ? "MUTATA"
                    : r->status == SILEX_PLAGULA_NOVA ? "NOVA  "
                    : "ABSENS";

                imprimere("  %s  %.*s\n", signum,
                    (integer)r->via.mensura,
                    (constans character*)r->via.datum);
            }
            redde ZEPHYRUM;
        }
        si (chorda_aequalis_literis(verbum, "condere"))
        {
            chorda nuntius_opt = argumenta_obtinere_optionem(lecta,
                "--nuntius", piscina);
            constans character* nuntius = nuntius_opt.mensura
                    > ZEPHYRUM
                ? chorda_ut_cstr(nuntius_opt, piscina)
                : "(sine nuntio)";
            SilexConditioFructus c = silex_condere(piscina,
                via_proiecti, nuntius);

            si (!c.successus)
            {
                fprintf(stderr, "silex condere: %s\n", c.erratum);
                redde I;
            }
            imprimere("silex condere: conditio seq %ld -"
                " %d conditae, %d remotae\n", (longus)c.seq,
                (integer)c.conditae, (integer)c.remotae);
            redde ZEPHYRUM;
        }
        {
            Xar* ordo = silex_historia(piscina, via_proiecti);
            s32  index;   /* SIGNATUS: numeratio descendens -
                           * i32 >= 0 semper verum esset (examen
                           * comparationem vanam cepit) */

            si (ordo == NIHIL)
            {
                fprintf(stderr, "silex historia: volumen legi non"
                    " potuit\n");
                redde I;
            }
            /* recentissima primum */
            per (index = (s32)xar_numerus(ordo) - 1; index >= 0;
                index = index - 1)
            {
                SilexConditio* c = (SilexConditio*)xar_obtinere(
                    ordo, (i32)index);

                imprimere("  [%ld] %.*s  %.*s (%d plagulae)\n",
                    (longus)c->seq,
                    (integer)c->momentum.mensura,
                    (constans character*)c->momentum.datum,
                    (integer)c->nuntius.mensura,
                    (constans character*)c->nuntius.datum,
                    (integer)c->tactae);
            }
            redde ZEPHYRUM;
        }
    }

    si (!chorda_aequalis_literis(verbum, "novum"))
    {
        fprintf(stderr, "silex: verbum ignotum: %.*s"
            " (verba: novum, ui, status, condere, historia)\n",
            (integer)verbum.mensura,
            (constans character*)verbum.datum);
        redde I;
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "silex novum: titulus deest\n");
        redde I;
    }
    si (fabrica == NIHIL)
    {
        fprintf(stderr, "silex: fabrica ignota - da --fabrica"
            " aut SILEX_FABRICA pone\n");
        redde I;
    }

    destinatio_opt = argumenta_obtinere_optionem(lecta,
        "--destinatio", piscina);
    si (destinatio_opt.mensura > ZEPHYRUM)
    {
        destinatio = chorda_ut_cstr(destinatio_opt, piscina);
    }
    alioquin
    {
        destinatio = ".";
    }

    optiones.fabrica = fabrica;
    optiones.destinatio = destinatio;
    optiones.titulus = chorda_ut_cstr(titulus, piscina);

    fructus = silex_novum(piscina, &optiones);
    si (!fructus.successus)
    {
        fprintf(stderr, "silex novum: FRACTUM - %s\n",
            fructus.erratum == NIHIL ? "causa ignota"
                : fructus.erratum);
        redde I;
    }

    imprimere("silex novum: %s excusum\n", optiones.titulus);
    imprimere("  volumen:    %s (veritas)\n", fructus.volumen_via);
    imprimere("  vendicatae: %d plagulae (e %s)\n",
        (integer)fructus.vendicatae, fabrica);
    imprimere("  genitae:    %d plagulae\n",
        (integer)fructus.genitae);
    imprimere("  deinde:     cd %s/%s && ./aedificare.sh &&"
        " ./probare.sh\n", destinatio, optiones.titulus);

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
