/* probatio_ludus_tessera_pictor.c - pictor verus per glutinum
 * tesserae, sine terminali (A4)
 *
 * Compositio EADEM ac principalia (pictor_applicatio; volumina
 * temporaria, canones e radice). Sessio fenestrae: ictus penicilli
 * (pressio, tractus, solutio) in centris cellularum, notatus. Eadem
 * notata per terminalem iterantur: codificator (cellula VI x VIII) ->
 * octeti -> rivus (modi ut ludus_tessera_currere) ->
 * ludus_tessera_tractare. Status idem: sigillum documenti, cursor
 * actorum, insulae ambae. Quadrum glutini titulum status
 * ("penicillus") in cellulas pingit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "thema.h"
#include "volumen.h"
#include "eventus.h"
#include "insula.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "rivus_terminalis.h"
#include "codificator_terminalis.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "ludus_tessera_pons.h"
#include "ludus_tessera.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMNAE  LXXX
#define LINEAE    XXX

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

interior chorda
_status (
    PictorApplicatio* app,
             Piscina* piscina)
{
    chorda c;

    c = chorda_concatenare(insula_scribere(app->repo, INSULA_DURABILIS,
        piscina), insula_scribere(app->repo, INSULA_EPHEMERA, piscina),
        piscina);
    redde chorda_concatenare(c, pictor_documentum_sigillum_hex(app->doc,
        piscina), piscina);
}

/* Transitus terminalis: codificator -> rivus -> glutinum */
nomen structura {
    RivusTerminalis* rivus;
    CodificatorModi  modi;
       LudusTessera* lt;
            Piscina* piscina;
} Transitus;

interior vacuum
_transire (
              vacuum* ctx,
         Dispensator* d,
    constans Eventus* e)
{
            Transitus* t = (Transitus*)ctx;
    ChordaAedificator* a = chorda_aedificator_creare(t->piscina, LXIV);
               chorda  o;
              Eventus  x;

    (vacuum)d;
    (vacuum)codificator_eventa(&t->modi, e, I, a);
    o = chorda_aedificator_finire(a);
    (vacuum)rivus_tradere(t->rivus, o.datum, o.mensura);
    dum (rivus_eventum_coalitum(t->rivus, e->tempus, &x))
    {
        ludus_tessera_tractare(t->lt, &x, e->tempus);
    }
}

interior Eventus
_murem (
    eventus_genus_t  genus,
                s64  tempus,
                s32  columna,
                s32  linea,
       mus_botton_t  botton)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = genus;
    e.tempus  = tempus;
    /* centrum cellulae (terminalis solum centra narrat) */
    e.datum.mus.x        = columna * VI + III;
    e.datum.mus.y        = linea * VIII + IV;
    e.datum.mus.botton   = botton;
    e.datum.mus.pressio  = EVENTUS_PRESSIO_IGNOTA;
    redde e;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
       PictorApplicatio  f;
       PictorApplicatio  t;
                    Xar* notata;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;
           LudusTessera* lt;
              Transitus  tr;
                     i8  intrandi[LUDUS_TESSERA_MODI_MAXIMI];
                     i8  exeundi[LUDUS_TESSERA_MODI_MAXIMI];
                    i32  mi;
                    i32  me;
                    i32  i;

    piscina =
        piscina_generare_dynamicum("probatio_ludus_tessera_pictor",
        IV * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    radix = getenv("RHUBARB_RADIX");

    imprimere("\n--- I. sessio fenestrae: ictus notatus ---\n");
    CREDO_VERUM (pictor_applicatio_aedificare(&f, piscina, intern,
        volumen_temporarium(piscina, "lt_pictor_f"), radix,
        COLUMNAE * VI, LINEAE * VIII));
    notata = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(f.d, notata);
    {
        Eventus ev[V];
        Eventus mutatio_magnitudinis;

        /* B1: magnitudo initialis, ut glutinum fenestrae eam mittit */
        memset(&mutatio_magnitudinis, ZEPHYRUM, magnitudo(Eventus));
        mutatio_magnitudinis.genus   = EVENTUS_MUTARE_MAGNITUDINEM;
        mutatio_magnitudinis.tempus  = M - I;
        mutatio_magnitudinis.datum.mutare_magnitudinem.latitudo =
            COLUMNAE * VI;
        mutatio_magnitudinis.datum.mutare_magnitudinem.altitudo =
            LINEAE * VIII;
        dispensator_tractare(f.d, &mutatio_magnitudinis);

        ev[0] = _murem(EVENTUS_MUS_DEPRESSUS, M, V, V, MUS_SINISTER);
        ev[1] = _murem(EVENTUS_MUS_MOTUS, M + XX, VI, V, MUS_SINISTER);
        ev[2] = _murem(EVENTUS_MUS_MOTUS, M + XL, VII, VI,
            MUS_SINISTER);
        ev[3] = _murem(EVENTUS_MUS_MOTUS, M + LX, VIII, VII,
            MUS_SINISTER);
        ev[4] = _murem(EVENTUS_MUS_LIBERATUS, M + LXXX, VIII, VII,
            MUS_SINISTER);
        per (i = ZEPHYRUM; i < V; i++)
        {
            dispensator_tractare(f.d, &ev[i]);
        }
    }
    dispensator_notarium_ponere(f.d, NIHIL);
    /* ictus actum in documento */
    CREDO_VERUM (pictor_documentum_cursor(f.doc) > ZEPHYRUM);

    imprimere("\n--- II. eadem per terminalem (glutinum) ---\n");
    CREDO_VERUM (pictor_applicatio_aedificare(&t, piscina, intern,
        volumen_temporarium(piscina, "lt_pictor_t"), radix,
        COLUMNAE * VI, LINEAE * VIII));
    pm    = tessera_pons_memoriae_creare(piscina, COLUMNAE, LINEAE);
    opus  = tessera_aperire(piscina, &pm->pons);
    lt   = ludus_tessera_creare(piscina, t.d, t.figurae, ZEPHYRUM,
        pictor_imago_fons, &t.montatio.figurae_ctx, opus, VI, VIII);
    CREDO_NON_NIHIL (lt);
    memset(&tr, ZEPHYRUM, magnitudo(Transitus));
    tr.piscina                = piscina;
    tr.lt                     = lt;
    tr.rivus                  = rivus_creare(piscina, VI, VIII);
    tr.modi.mus               = CODIFICATOR_MUS_OMNIS;
    tr.modi.cellula_latitudo  = VI;
    tr.modi.cellula_altitudo  = VIII;
    CREDO_VERUM (ludus_tessera_modos_componere(tr.rivus,
        RIVUS_MODUS_MUS | RIVUS_MODUS_GLUTINUM, intrandi, &mi, exeundi,
        &me));
    (vacuum)manus_ludus_iterare_per(manus_ludus_creare(piscina, t.d),
        notata, MANUS_ITERATIO_SEMANTICA, NIHIL, _transire, &tr);
    CREDO_VERUM (pictor_documentum_cursor(t.doc)
        == pictor_documentum_cursor(f.doc));
    CREDO_VERUM (chorda_aequalis(_status(&t, piscina),
        _status(&f, piscina)));
    /* B1: canon pictoris superficiem accipit (aliter tacite recusat) */
    CREDO_NON_NIHIL (insula_attributum(t.repo, INSULA_EPHEMERA,
        "superficies_latitudo"));
    si (insula_attributum(t.repo, INSULA_EPHEMERA,
        "superficies_latitudo"))
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (*insula_attributum(t.repo,
            INSULA_EPHEMERA, "superficies_latitudo"), "480");
    }

    imprimere("\n--- III. quadrum in cellulas ---\n");
    ludus_tessera_quadrum(lt, M + C);
    CREDO_VERUM (tessera_praesentare(opus));
    CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
        "penicillus"));
    /* 013 B3c: pagina (imago) SUPRA mensam - ordo pictoris in
     * tessellatione (olim mensa paginam et ictus celabat) */
    {
        Color mensa = thema_color(COLOR_SUPERFICIES);
          i32 color_mensae = ((i32)mensa.r << XVI)
              | ((i32)mensa.g << VIII) | (i32)mensa.b;

        CREDO_VERUM (tessera_cellulam_legere(opus, VI, V).color_fundi
            != color_mensae);
    }
    /* 013 B3 / P1a: linea status in lineis III ULTIMIS; titulus in
     * media (centratus), post quadrata V (P4a: x CXXXVI -> columna
     * XXIII, proxima; P3 CX = XVIII; olim LXXXIV = XIV) */
    CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, XXIII,
        LINEAE - II).signum, (i32)'p');

    imprimere("\n--- IV. amplitudo mutata: linea status sequitur"
              " ---\n");
    {
        Eventus e;

        pm->altitudo = XX;
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        e.datum.mutare_magnitudinem.latitudo = COLUMNAE * VI;
        e.datum.mutare_magnitudinem.altitudo = XX * VIII;
        ludus_tessera_tractare(lt, &e, M + CC);
        ludus_tessera_quadrum(lt, M + CC);
        CREDO_VERUM (tessera_praesentare(opus));
        CREDO_AEQUALIS_I32 ((i32)tessera_altitudo(opus), XX);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, XXIII,
            XVIII).signum, (i32)'p');
    }

    /* volumina temporaria claudenda: aliter /tmp/lt_pictor_*-N
     * cumulantur et post C cursus volumen_temporarium deficit (vitium
     * inventum 2026-10-06, porta commissionis aemulator C4) */
    volumen_claudere(f.volumen);
    volumen_claudere(t.volumen);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
