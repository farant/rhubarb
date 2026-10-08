/* probatio_ludus_tessera_glutinum.c - glutinum sine terminali (A3)
 *
 * Applicatio ludicra (ludus_toy, eadem ac probatio_ludus_fenestra) in
 * opus tesserae per pontem memoriae (XL x XIII, modulus VI x VIII).
 * I.   Creatio: extensio = opus x cellula; argumenta NIHIL.
 * II.  Chorda claudendi: Ctrl-C solum.
 * III. Tempus stampatur ANTE traditionem (notarius dispensatoris).
 * IV.  Quadrum: pulsus recomponit; fines bottonis (figura_finium) ut
 *      runae delineandi in cellulis; quadrum II piscinam vacat.
 * V.   Amplitudo: opus relegit, extensio moduli renovatur.
 * VI.  Resumptio: pictura plena (2J) in quadro proximo.
 * VII. Ansa vera sine terminali: I, nihil tangit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "eventus.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "ludus_tessera.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior b32
_continet (
                 chorda  captum,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= captum.mensura; k++)
    {
        si (memcmp(captum.datum + k, literae, (memoriae_index)n)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Cellulae cum runa delineandi (U+2500-257F: E2 94/95 ..) */
interior i32
_cellulae_delineandi (
    constans TesseraOpus* opus)
{
    i32 n = ZEPHYRUM;
    s32 x;
    s32 y;

    per (y = ZEPHYRUM; y < (s32)tessera_altitudo(opus); y++)
    {
        per (x = ZEPHYRUM; x < (s32)tessera_latitudo(opus); x++)
        {
            i32 s = tessera_cellulam_legere(opus, x, y).signum;

            si (   (s & 0xFF) == 0xE2 && (((s >> VIII) & 0xFF) == 0x94
                || ((s >> VIII) & 0xFF) == 0x95))
            {
                n++;
            }
        }
    }
    redde n;
}

/* pulsus probandus (vicus-latera S1c) */
nomen structura {
    i32 vocationes;
    b32 reddere;
} PulsusProbandus;

interior b32
pulsus_probandus (
    vacuum* ctx)
{
    PulsusProbandus* p;

    p              = (PulsusProbandus*)ctx;
    p->vocationes  += I;
    redde p->reddere;
}

interior Eventus
_clavis (
    s32 runa,
    i32 modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.modificantes  = modificantes;
    redde e;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
InternamentumChorda* intern;
 InsulaRepositorium* repo;
     ActioRegistrum* reg;
          ToyStatus  toy;
        Dispensator* d;
    FiguraRegistrum* figurae;
TesseraPonsMemoriae* pm;
        TesseraOpus* opus;
       LudusTessera* lt;
                Xar* notata;
            Eventus  e;

    piscina =
        piscina_generare_dynamicum("probatio_ludus_tessera_glutinum",
        LXIV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>",
        "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
        &toy, CCC);
    figurae = figura_registrum_creare(piscina);
    CREDO_VERUM (figura_registrare(figurae, PARTES_BOTTONE, ZEPHYRUM,
        figura_finium, NIHIL));
    pm    = tessera_pons_memoriae_creare(piscina, XL, XIII);
    opus  = tessera_aperire(piscina, &pm->pons);

    imprimere("\n--- I. creatio ---\n");
    lt = ludus_tessera_creare(piscina, d, figurae, ZEPHYRUM, NIHIL,
        NIHIL,
        opus, VI, VIII);
    CREDO_NON_NIHIL (lt);
    si (!lt)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_AEQUALIS_S32 (lt->modulus.cellula_latitudo, VI);
    CREDO_AEQUALIS_S32 (lt->modulus.extensio_latitudo, XL * VI);
    CREDO_AEQUALIS_S32 (lt->modulus.extensio_altitudo, XIII * VIII);
    CREDO_NIHIL (ludus_tessera_creare(piscina, d, figurae, ZEPHYRUM,
        NIHIL, NIHIL, NIHIL, VI, VIII));
    CREDO_NIHIL (ludus_tessera_creare(piscina, NIHIL, figurae, ZEPHYRUM,
        NIHIL, NIHIL, opus, VI, VIII));

    imprimere("\n--- II. chorda claudendi ---\n");
    e = _clavis('c', MOD_IMPERIUM);
    CREDO_VERUM (ludus_tessera_claudendum_est(lt, &e));
    e = _clavis('c', ZEPHYRUM);
    CREDO_FALSUM (ludus_tessera_claudendum_est(lt, &e));
    e = _clavis('d', MOD_IMPERIUM);
    CREDO_FALSUM (ludus_tessera_claudendum_est(lt, &e));
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_MUS_DEPRESSUS;
    CREDO_FALSUM (ludus_tessera_claudendum_est(lt, &e));

    imprimere("\n--- III. tempus ante traditionem ---\n");
    notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(d, notata);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x  = XXXV;
    e.datum.mus.y  = XX;
    ludus_tessera_tractare(lt, &e, M + L);
    CREDO_VERUM (xar_numerus(notata) > ZEPHYRUM);
    si (xar_numerus(notata) > ZEPHYRUM)
    {
        EventusNotatum* n = (EventusNotatum*)xar_obtinere(notata,
            xar_numerus(notata) - I);

        CREDO_VERUM (n->eventus.tempus == M + L);
    }
    e.tempus = DCCLXXVII;
    ludus_tessera_tractare(lt, &e, M + C);
    {
        EventusNotatum* n = (EventusNotatum*)xar_obtinere(notata,
            xar_numerus(notata) - I);

        CREDO_VERUM (n->eventus.tempus == DCCLXXVII);
    }
    dispensator_notarium_ponere(d, NIHIL);
    CREDO_NON_NIHIL (insula_attributum(repo, INSULA_EPHEMERA,
        "numerus"));

    imprimere("\n--- IV. quadrum ---\n");
    {
        i32 n0 = toy.compositiones;
        i32 n1;

        ludus_tessera_quadrum(lt, M + CC);
        CREDO_AEQUALIS_I32 (lt->mensurae.quadra, I);
        /* B1: magnitudo initialis nuntiata (cellulae x modulus) */
        CREDO_NON_NIHIL (insula_attributum(repo, INSULA_EPHEMERA,
            "superficies_latitudo"));
        si (insula_attributum(repo, INSULA_EPHEMERA,
                "superficies_latitudo"))
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (*insula_attributum(repo,
                INSULA_EPHEMERA, "superficies_latitudo"), "240");
            CREDO_CHORDA_AEQUALIS_LITERIS (*insula_attributum(repo,
                INSULA_EPHEMERA, "superficies_altitudo"), "104");
        }
        CREDO_VERUM (toy.compositiones > n0);
        CREDO_VERUM (mandata_numerus(lt->mandata) >= III);
        CREDO_VERUM (_cellulae_delineandi(opus) > ZEPHYRUM);
        CREDO_VERUM (lt->mensurae.ms_quadri_maximum >= ZEPHYRUM);
        n1 = mandata_numerus(lt->mandata);
        ludus_tessera_quadrum(lt, M + CCC);
        CREDO_AEQUALIS_I32 (lt->mensurae.quadra, II);
        CREDO_AEQUALIS_I32 (mandata_numerus(lt->mandata), n1);
    }

    imprimere("\n--- V. amplitudo ---\n");
    pm->latitudo = L;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.datum.mutare_magnitudinem.latitudo  = L * VI;
    e.datum.mutare_magnitudinem.altitudo  = XIII * VIII;
    ludus_tessera_tractare(lt, &e, M + CD);
    CREDO_AEQUALIS_I32 (tessera_latitudo(opus), L);
    CREDO_AEQUALIS_S32 (lt->modulus.extensio_latitudo, L * VI);
    CREDO_AEQUALIS_S32 (lt->modulus.extensio_altitudo, XIII * VIII);

    imprimere("\n--- VI. resumptio ---\n");
    ludus_tessera_quadrum(lt, M + D);
    (vacuum)tessera_praesentare(opus);
    tessera_pons_memoriae_purgare(pm);
    ludus_tessera_quadrum(lt, M + DC);
    (vacuum)tessera_praesentare(opus);
    CREDO_FALSUM (_continet(tessera_pons_memoriae_captum(pm),
        "\033[2J"));
    tessera_pons_memoriae_purgare(pm);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_RESUMPTIO;
    ludus_tessera_tractare(lt, &e, M + DCC);
    ludus_tessera_quadrum(lt, M + DCC);
    (vacuum)tessera_praesentare(opus);
    CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
        "\033[2J"));

    imprimere("\n--- VII. ansa vera sine terminali ---\n");
    CREDO_AEQUALIS_S32 (ludus_tessera_currere(lt, I), I);

    imprimere("\n--- VIII. vicus-latera S1c: pingendum ---\n");
    {
        PulsusProbandus pp;

        CREDO_VERUM (ludus_tessera_pingendum(lt, FALSUM, M + CC));
        pp.vocationes  = ZEPHYRUM;
        pp.reddere     = FALSUM;
        ludus_tessera_pulsum_ponere(lt, pulsus_probandus, &pp);
        ludus_tessera_quadrum(lt, M + CC);
        CREDO_FALSUM (ludus_tessera_pingendum(lt, FALSUM, M + CCC));
        CREDO_AEQUALIS_I32 (pp.vocationes, I);
        CREDO_VERUM (ludus_tessera_pingendum(lt, VERUM, M + CCC));
        pp.reddere = VERUM;
        CREDO_VERUM (ludus_tessera_pingendum(lt, FALSUM, M + CCC));
        pp.reddere = FALSUM;
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus        = EVENTUS_MUS_DEPRESSUS;
        e.datum.mus.x  = XXXV;
        e.datum.mus.y  = XX;
        dispensator_tractare(lt->d, &e);
        CREDO_VERUM (ludus_tessera_pingendum(lt, FALSUM, M + CD));
        ludus_tessera_quadrum(lt, M + CD);
        CREDO_FALSUM (ludus_tessera_pingendum(lt, FALSUM, M + D));
        ludus_tessera_pulsum_ponere(lt, NIHIL, NIHIL);
        CREDO_VERUM (ludus_tessera_pingendum(lt, FALSUM, M + D));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
