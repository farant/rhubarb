/* probatio_dispensator.c - ansa: derivare -> destinatio -> actio ->
 * componere; focus in insula; super derivatum; sedes quietis */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
mus (
    eventus_genus_t genus,
                s64 t,
                s32 x,
                s32 y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = genus;
    e.tempus       = t;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
}

interior Eventus
clavis (
          s64 t,
    character typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                 = t;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    /* clavis logica: clavis_t ASCII ipsum fert (Tab IX, Effugium
     * XXVII) - ut fenestra */
    e.datum.clavis.clavis = (clavis_t)typus;
    redde e;
}

/* S1a: gestus ludicer - litterae numerantur, effusio notatur */
nomen structura {
       i32 litterae;
       i32 effusiones;
       b32 recusare;
    chorda scriptor;   /* scriptor insulae dum effunditur */
} GestusLudicer;

interior vacuum
littera_addere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    ((GestusLudicer*)motus->gestus.status)->litterae++;
}

interior b32
gestum_effundere (
                vacuum* gestus,
    InsulaRepositorium* repo,
                vacuum* ctx)
{
    GestusLudicer* g;

    (vacuum)ctx;
    g = (GestusLudicer*)gestus;
    si (g->recusare)
    {
        redde FALSUM;
    }
    g->effusiones++;
    g->scriptor = repo->scriptor;
    redde VERUM;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
          ActioRegistrum* reg;
               ToyStatus  toy;
             Dispensator* d;
                 Eventus  e;
                  chorda* a;
           GestusLudicer  g;
                  chorda  vacua_focus;
                     i32  n0;
                     i32  versio_ictus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    CREDO_NON_NIHIL(d);
    /* quadrum 0 ante initum */
    CREDO_NON_NIHIL(dispensator_arbor(d));
    CREDO_AEQUALIS_I32(toy.compositiones, I);
    CREDO_AEQUALIS_I32(dispensator_numerus_compositionum(d), I);
    CREDO_VERUM(chorda_vacua(dispensator_focus(d)));
    CREDO_VERUM(chorda_vacua(dispensator_super(d)));

    CREDO_GRADUS("Ictus in b1: actio, focus, recompositio");
    n0     = toy.compositiones;
        e  = mus(EVENTUS_MUS_DEPRESSUS, M, XXXV, XX);
    dispensator_tractare(d, &e);
    /* focus_captus ad b1 TRADITUS post recompositionem, contra
     * arborem novam: compositiones in traditione > ante (T5) */
    CREDO_AEQUALIS_I32(toy.derivata, I);
    CREDO_VERUM(toy.compositiones_in_traditione > n0);
    a = insula_attributum(repo, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* regula staleness: post omnem eventum componitur */
    CREDO_VERUM(toy.compositiones > n0);
    e = mus(EVENTUS_MUS_LIBERATUS, M + L, XXXV, XX);
    dispensator_tractare(d, &e);
    /* alter ictus: incrementare SUBSTITUIT, non geminat */
    e = mus(EVENTUS_MUS_DEPRESSUS, M + DC, XXXV, XX);
    dispensator_tractare(d, &e);
    a = insula_attributum(repo, "numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "2");
    e = mus(EVENTUS_MUS_LIBERATUS, M + DC + L, XXXV, XX);
    dispensator_tractare(d, &e);

    CREDO_GRADUS("Motus super tabulam: super derivatur");
    e = mus(EVENTUS_MUS_MOTUS, M + DCC, CXX, L);
        dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "tabula");
    CREDO_AEQUALIS_I32(toy.derivata, II);        /* intravit tabula */
    e = mus(EVENTUS_MUS_MOTUS, M + DCC + X, V, XC);
        dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(d), "radix");
    CREDO_AEQUALIS_I32(toy.derivata, IV);        /* exiit, intravit */

    CREDO_GRADUS("Ictus in tabula: captura, motus, UNA scriptura"
        " durabilis");
    e = mus(EVENTUS_MUS_DEPRESSUS, II * M, CXX, L);
    dispensator_tractare(d, &e);
    versio_ictus = insula_versio(repo);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_motus(d)->captura,
                                  "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    e = mus(EVENTUS_MUS_MOTUS, II * M + XX, CXXV, LV);
    dispensator_tractare(d, &e);
    e = mus(EVENTUS_MUS_MOTUS, II * M + XL, CXXX, LX);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       III);
    /* nondum scriptum (C2: insula una - versio post depressionem
     * immota per motus) */
    CREDO_AEQUALIS_I32(insula_versio(repo), versio_ictus);
    e = mus(EVENTUS_MUS_LIBERATUS, II * M + C, CXXX, LX);
    dispensator_tractare(d, &e);
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    a = insula_attributum(repo, "ictus_numerus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    a = insula_attributum(repo, "puncta");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "3");
    /* ephemera intacta ab ictu */
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));

    CREDO_GRADUS("Clavis ESC ascendit ad radicem");
    e = clavis(II * M + CC, (character)XXVII);
    dispensator_tractare(d, &e);
    a = insula_attributum(repo, "fuga");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "verum");

    CREDO_GRADUS("Tab: focus proximum, involvens");
    e = clavis(II * M + CCC, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    e = clavis(II * M + CCCX, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    e = clavis(II * M + CCCXX, '\t');
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    CREDO_GRADUS("A5: Tab per clavem logicam, Shift verum");
    /* Ctrl+I: characteres '\t' (typus) sed clavis 'I' - focus NON
     * movetur (Tab != Ctrl+I) */
    e                            = clavis(II * M + CD, '\t');
    e.datum.clavis.clavis        = (clavis_t)'I';
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_IMPERIUM_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* directio: DUO focusabiles (b1, tabula) - ex foco b1 et ante et
     * retro 'tabula' est. Ex foco NULLO directio videtur: ante ->
     * primus (b1), retro -> ultimus (tabula). */
    /* Shift+Tab cum vexillis veris macOS (0x20002) -> retro */
    vacua_focus.datum    = NIHIL;
    vacua_focus.mensura  = ZEPHYRUM;
    dispensator_focus_ponere(d, vacua_focus);
    e                            = clavis(II * M + CDX, '\t');
    e.datum.clavis.modificantes  = MOD_SHIFT | MOD_SHIFT_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    /* Ctrl+Tab (0x40001): ante, NON retro - bitus 0x1 latus Ctrl est */
    dispensator_focus_ponere(d, vacua_focus);
    e                            = clavis(II * M + CDXX, '\t');
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_IMPERIUM_SINISTER;
    dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    CREDO_GRADUS("Acervus foci");
    dispensator_focus_impellere(d, chorda_ex_literis("tabula",
        piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    dispensator_focus_impellere(d, chorda_ex_literis("b1", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "tabula");
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");
    /* acervus vacuus: extrahere nihil mutat */
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    CREDO_GRADUS("Quies: pulsus post CCC ms effundit motus");
    dispensator_motus(d)->pan.x                      = VII;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = III * M;
    dispensator_pulsare(d, III * M + CC);
    CREDO_NIHIL(insula_attributum(repo, "pan_x"));
    dispensator_pulsare(d, III * M + CD);
    a = insula_attributum(repo, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "7");
    CREDO_FALSUM(dispensator_motus(d)->sordida);

    CREDO_GRADUS("Restitutio honesta post omnia; focus supervivit");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "b1");

    CREDO_GRADUS("Superficies (013 B1): magnitudo in ephemera,"
        " a dispensatore scripta");
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus                              = IV * M;
    e.datum.mutare_magnitudinem.latitudo  = CCXL;
    e.datum.mutare_magnitudinem.altitudo  = CIV;
    dispensator_tractare(d, &e);
    a = insula_attributum(repo,
        "superficies_latitudo");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "240");
    }
    a = insula_attributum(repo,
        "superficies_altitudo");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "104");
    }
    e.tempus                              = IV * M + C;
    e.datum.mutare_magnitudinem.latitudo  = CCC;
    e.datum.mutare_magnitudinem.altitudo  = LXXX;
    dispensator_tractare(d, &e);
    a = insula_attributum(repo,
        "superficies_latitudo");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "300");
    }
    a = insula_attributum(repo,
        "superficies_altitudo");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "80");
    }
    /* restitutio honesta etiam post superficiem */
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));

    CREDO_GRADUS("S1a: gestus in quiete propria et in fine");
    memset(&g, ZEPHYRUM, magnitudo(g));
    motus_gestum_ponere(dispensator_motus(d), &g, gestum_effundere,
        NIHIL,
                        M);
    mutare_gestum(dispensator_motus(d), littera_addere, NIHIL, V * M);
    dispensator_pulsare(d, V * M + D);
    CREDO_AEQUALIS_I32(g.effusiones, ZEPHYRUM);
    dispensator_pulsare(d, VI * M);
    CREDO_AEQUALIS_I32(g.effusiones, I);
    dispensator_pulsare(d, VIII * M);
    CREDO_AEQUALIS_I32(g.effusiones, I);
    /* finis: pendens effunditur sine quiete; deinde nihil */
    mutare_gestum(dispensator_motus(d), littera_addere, NIHIL, IX * M);
    dispensator_finire(d);
    CREDO_AEQUALIS_I32(g.effusiones, II);
    CREDO_FALSUM(dispensator_motus(d)->gestus.sordidus);
    dispensator_finire(d);
    CREDO_AEQUALIS_I32(g.effusiones, II);
    /* finis pan/zoom pendentes quoque effundit */
    dispensator_motus(d)->pan.x                      = XI;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = IX * M;
    dispensator_finire(d);
    a = insula_attributum(repo, "pan_x");
    CREDO_NON_NIHIL(a);
    si (a)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(*a, "11");
    }
    CREDO_FALSUM(dispensator_motus(d)->sordida);

}

/* vicus quies T5: mora usque ad effusionem quietis proximam */
interior vacuum
_sectio_mora_proxima (
    CredoContextus* c)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
          GestusLudicer  g;

    intern = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, intern,
        "<insula/>");
    reg = actio_registrum_creare(c->piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(c->piscina, intern, repo, reg,
        toy_componere, &toy, CCC);
    CREDO_NON_NIHIL(d);

    CREDO_GRADUS("nihil pendet: -1");
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, M), -I);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(NIHIL, M), -I);

    CREDO_GRADUS("pan sordidum: quies_ms post mutationem, deinde -1");
    dispensator_motus(d)->pan.x                      = VII;
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = M;
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, M + C), CC);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, M + D), ZEPHYRUM);
    dispensator_pulsare(d, M + D);
    CREDO_FALSUM(dispensator_motus(d)->sordida);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, M + D), -I);

    CREDO_GRADUS("gestus sordidus: quies sua; minimum duorum");
    memset(&g, ZEPHYRUM, magnitudo(g));
    motus_gestum_ponere(dispensator_motus(d), &g, gestum_effundere,
        NIHIL, M);
    mutare_gestum(dispensator_motus(d), littera_addere, NIHIL, II * M);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, II * M + C), CM);
    dispensator_motus(d)->sordida                    = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis  = II * M + C;
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, II * M + C), CCC);
    dispensator_pulsare(d, II * M + CD);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, II * M + CD), DC);

    CREDO_GRADUS("effusio recusata: iterum post quies_ms, non 0");
    g.recusare = VERUM;
    dispensator_pulsare(d, III * M);
    CREDO_AEQUALIS_I32(g.effusiones, ZEPHYRUM);
    CREDO_VERUM(dispensator_motus(d)->gestus.sordidus);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, III * M), M);
    g.recusare = FALSUM;
    dispensator_pulsare(d, IV * M);
    CREDO_AEQUALIS_I32(g.effusiones, I);
    CREDO_AEQUALIS_S64(dispensator_mora_proxima(d, IV * M), -I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ansa: derivare, destinatio, actio, componere", _sectio_prima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "vicus quies T5: mora proxima", _sectio_mora_proxima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("dispensator", SECTIONES);
}
