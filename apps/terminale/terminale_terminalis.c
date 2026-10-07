/* terminale_terminalis.c - terminale in TERMINALI (aemulator-plan E3)
 *
 * Terminalis in terminali: compositio communis (terminale.h) + glutinum
 * tesserae. ANSA PROPRIA (exemplar ludus_tessera_currere, cum pulsu
 * hospitis): Ctrl-C ad conchum it (nulla chorda claudendi); finis cum
 * concha exit aut terminalis exterior abit. Mora lectionis ad MORA_MS
 * limitata, ut effusio conchae statim appareat. Magnitudo initialis
 * per eventum MUTARE_MAGNITUDINEM (ludus_tessera_tractare modulum
 * renovat). -fumus: concha scripta, LX quadra, exitus.
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "terminalis.h"
#include "rivus_terminalis.h"
#include "fenestra_tempus.h"
#include "tessera_pons.h"
#include "tessera_opus.h"
#include "ludus_tessera_pons.h"
#include "ludus_tessera.h"
#include "pseudoterminale.h"
#include "terminale.h"
#include <stdio.h>
#include <string.h>

#define CELLULA_X    VI
#define CELLULA_Y    VIII
#define MORA_MS      XVI
#define LECTIO       CCLVI
#define QUADRA_FUMI  LX

hic_manens constans character* constans ambitus[] = {
    "TERM=xterm-256color",
    "COLORTERM=truecolor",
    "TERM_PROGRAM=terminale",
    NIHIL
};

interior vacuum
magnitudinem_nuntiare (
    LudusTessera* lt,
             s64  nunc)
{
    TerminalisAmplitudo amplitudo;
                Eventus e;

    si (!terminalis_amplitudo(&amplitudo))
    {
        redde;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
    e.datum.mutare_magnitudinem.latitudo =
        (i32)(amplitudo.columnae * CELLULA_X);
    e.datum.mutare_magnitudinem.altitudo =
        (i32)(amplitudo.lineae * CELLULA_Y);
    ludus_tessera_tractare(lt, &e, nunc);
}

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
                    TesseraPons  pons;
                   TesseraOpus* opus;
                  LudusTessera* lt;
                RivusTerminalis* rivus;
          AemulatorHospesPulsus  p;
                        Eventus  e;
                             i8  intrandi[LUDUS_TESSERA_MODI_MAXIMI];
                             i8  exeundi[LUDUS_TESSERA_MODI_MAXIMI];
                             i8  buffer[LECTIO];
                            i32  n_intrandi;
                            i32  n_exeundi;
                            s64  nunc;
                            s32  mora;
                            s32  capax;
                            s32  lecti;
                            b32  fumus;
                            b32  currens;

    si (!terminalis_adest())
    {
        fprintf(stderr, "terminale_terminalis: non terminalis\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("terminale_terminalis",
        VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    ludus_tessera_pontem_initiare(&pons);
    opus = tessera_aperire(piscina, &pons);
    si (!opus)
    {
        redde I;
    }
    tessera_colores_ponere(opus, tessera_colores_ambitus());
    tessera_politicam_ponere(opus, tessera_politica_ambitus());

    pseudoterminale_configuratio_initiare(&cfg_pt);
    cfg_pt.argumenta  = terminale_argumenta(piscina, (s32)argc, argv,
        &fumus);
    cfg_pt.ambitus = ambitus;
    cfg_pt.latitudo = (i32)tessera_latitudo(opus);
    cfg_pt.altitudo = (i32)tessera_altitudo(opus);
    pt = pseudoterminale_posix_creare(piscina, &cfg_pt, NIHIL, NIHIL);
    si (!pt || !terminale_applicatio_aedificare(&app, piscina, intern,
            pt, (i32)tessera_latitudo(opus) * CELLULA_X,
            (i32)tessera_altitudo(opus) * CELLULA_Y))
    {
        fprintf(stderr, "terminale_terminalis: concha non generata\n");
        redde I;
    }
    /* tessellatio lineas in cellulas verteret: sublinea litteram
     * deleret (D7c) */
    app.ornamenta_pixelorum = FALSUM;
    lt = ludus_tessera_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        NIHIL, NIHIL, opus, CELLULA_X, CELLULA_Y);
    rivus = rivus_creare(piscina, CELLULA_X, CELLULA_Y);
    si (   !lt || !rivus
        || !ludus_tessera_modos_componere(rivus,
               RIVUS_MODUS_MUS | RIVUS_MODUS_GLUTINUM, intrandi,
               &n_intrandi, exeundi, &n_exeundi)
        || !terminalis_intrare(intrandi, n_intrandi, exeundi,
               n_exeundi))
    {
        terminale_claudere(&app);
        redde I;
    }
    magnitudinem_nuntiare(lt, fenestra_tempus_ms());

    currens = VERUM;
    dum (currens)
    {
        p = terminale_pulsare(&app, ZEPHYRUM);
        si (p.finitus && !fumus)
        {
            frange;
        }
        nunc = fenestra_tempus_ms();
        dum (rivus_eventum_coalitum(rivus, nunc, &e))
        {
            ludus_tessera_tractare(lt, &e, nunc);
        }
        si (terminalis_resumptum())
        {
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus = EVENTUS_RESUMPTIO;
            ludus_tessera_tractare(lt, &e, nunc);
        }
        si (terminalis_amplitudo_mutata())
        {
            magnitudinem_nuntiare(lt, nunc);
        }
        ludus_tessera_quadrum(lt, nunc);
        (vacuum)tessera_praesentare(lt->opus);
        si (fumus && lt->mensurae.quadra >= QUADRA_FUMI)
        {
            frange;
        }
        mora = rivus_mora_ms(rivus);
        si (mora <= ZEPHYRUM || mora > MORA_MS)
        {
            mora = MORA_MS;
        }
        capax = (s32)rivus_spatium(rivus);
        lecti = terminalis_legere(buffer,
            (i32)((capax < LECTIO) ? capax : LECTIO), mora);
        si (lecti > ZEPHYRUM)
        {
            (vacuum)rivus_tradere(rivus, buffer, (i32)lecti);
        }
        alioquin si (lecti == ZEPHYRUM)
        {
            si (rivus_mora_ms(rivus) > ZEPHYRUM)
            {
                rivus_moram(rivus, fenestra_tempus_ms());
            }
        }
        alioquin
        {
            currens = FALSUM;
        }
    }
    dispensator_finire(lt->d);
    tessera_claudere(lt->opus);
    (vacuum)terminalis_exire();
    terminale_claudere(&app);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
