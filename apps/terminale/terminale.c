/* terminale.c - terminale in FENESTRA (aemulator-plan E3)
 *
 * Compositio communis (terminale.h) + fenestra; ANSA PROPRIA (decisio
 * XXV): quadro quoque hospes pulsatur (sine mora), deinde fenestra
 * exspectatur (MORA_MS), eventus tractantur, et pingitur SOLUM si quid
 * mutatum est (effusio infantis, eventus, magnitudo) - concha otiosa
 * nihil pingit. Concha exiens fenestram claudit (ut Ghostty). Ctrl-C
 * ad conchum it, numquam applicationem claudit.
 *
 * Optiones: -fumus (concha scripta, LX quadra, exitus), -imago <via>
 * (quadrum ultimum in PNG), -provenientia (fabrica: digestum
 * structurae; ~/.bin/terminale per institutio_terminale).
 */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/terminale.c"/> */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "fenestra.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "pseudoterminale.h"
#include "terminale.h"
#include "provenientia.h"
#include <stdio.h>
#include <string.h>

#define COLUMNAE      LXXX
#define LINEAE        XXIV
#define CELLULA_X     VI
#define CELLULA_Y     VIII
#define SCALA         II
#define MORA_MS       XVI
#define QUADRA_FUMI   LX

hic_manens constans character* constans ambitus[] = {
    "TERM=xterm-256color",
    "COLORTERM=truecolor",
    "TERM_PROGRAM=terminale",
    NIHIL
};

externus constans ProvenientiaRelatio provenientia_terminale;

s32
principale (
      integer   argc,
    character** argv)
{
                         Piscina* piscina;
             InternamentumChorda* intern;
             TerminaleApplicatio  app;
     PseudoterminaleConfiguratio  cfg_pt;
                 Pseudoterminale* pt;
            FenestraConfiguratio  cfg;
                        Fenestra* fenestra;
                 TabulaPixelorum* tabula;
                   LudusFenestra* lf;
           AemulatorHospesPulsus  p;
                         Eventus  e;
                             s64  nunc;
                             b32  fumus;
                             b32  claudendum;
                             b32  pingendum;
                             s32  exitus;
                             s32  k;
                          chorda  titulus;
                             b32  mutatus;
                       character  titulus_c[CCLVII];
              constans character* via_imaginis;

    si (provenientia_respondere((s32)argc, argv,
        &provenientia_terminale))
    {
        redde ZEPHYRUM;
    }
    piscina = piscina_generare_dynamicum("terminale", VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();
    via_imaginis = NIHIL;
    per (k = I; k + I < (s32)argc; k++)
    {
        si (strcmp(argv[k], "-imago") == ZEPHYRUM)
        {
            via_imaginis = argv[k + I];
        }
    }

    pseudoterminale_configuratio_initiare(&cfg_pt);
    cfg_pt.argumenta  = terminale_argumenta(piscina, (s32)argc, argv,
        &fumus);
    cfg_pt.ambitus = ambitus;
    cfg_pt.latitudo = COLUMNAE;
    cfg_pt.altitudo = LINEAE;
    pt = pseudoterminale_posix_creare(piscina, &cfg_pt, NIHIL, NIHIL);
    si (!pt || !terminale_applicatio_aedificare(&app, piscina, intern,
            pt, COLUMNAE * CELLULA_X, LINEAE * CELLULA_Y))
    {
        fprintf(stderr, "terminale: concha non generata\n");
        redde I;
    }

    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus   = "terminale";
    cfg.x         = C;
    cfg.y         = C;
    cfg.latitudo  = COLUMNAE * CELLULA_X * SCALA;
    cfg.altitudo  = LINEAE * CELLULA_Y * SCALA;
    cfg.vexilla   = FENESTRA_ORDINARIA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "terminale: fenestra\n");
        terminale_claudere(&app);
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               LINEAE * CELLULA_Y);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
                               NIHIL, NIHIL, tabula);
    si (!tabula || !lf)
    {
        terminale_claudere(&app);
        redde I;
    }

    claudendum  = FALSUM;
    pingendum   = VERUM;
    dum (!claudendum && !fenestra_debet_claudere(fenestra))
    {
        p = terminale_pulsare(&app, ZEPHYRUM);
        si (p.mutatum)
        {
            pingendum = VERUM;
        }
        /* titulus programmatis (OSC 0/2, D6b); vacuus = nomen nostrum.
         * In acervo, non piscina: concha titulum omni mandato mutat */
        titulus = terminale_titulus(&app, &mutatus);
        si (mutatus)
        {
            memcpy(titulus_c, titulus.datum,
                (memoriae_index)titulus.mensura);
            titulus_c[titulus.mensura] = '\0';
            fenestra_ponere_titulum(fenestra, titulus.mensura > ZEPHYRUM
                ? titulus_c : "terminale");
        }
        si (p.finitus && !fumus)
        {
            frange;
        }
        fenestra_expectare_eventus(fenestra, pingendum ? ZEPHYRUM
                                                       : MORA_MS);
        nunc = fenestra_tempus_ms();
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
            pingendum = VERUM;
        }
        si (pingendum || fumus)
        {
            ludus_quadrum(lf, nunc);
            fenestra_praesentare_pixela(fenestra, lf->tabula);
            pingendum = FALSUM;
        }
        si (fumus && lf->mensurae.quadra >= QUADRA_FUMI)
        {
            claudendum = VERUM;
        }
    }
    dispensator_finire(lf->d);
    exitus = ZEPHYRUM;
    si (   via_imaginis && !ludus_fenestra_imaginem_scribere(lf,
            via_imaginis))
    {
        fprintf(stderr, "terminale: imago non scripta: %s\n",
            via_imaginis);
        exitus = I;
    }
    imprimere("terminale: quadra=%d\n", (int)lf->mensurae.quadra);
    terminale_claudere(&app);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);
    redde exitus;
}
