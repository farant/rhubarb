/* probatio_spatium.c - spatia arboris (vicus-latera S2a-1)
 *
 * Applicatio ludicra BIS montata in spatiis '1.sinistrum.toy' et
 * '1.dextrum.toy' sub radice hospitis (spatium ""): ids, nomina
 * actionum et partes figurarum EADEM. Resolutio intra spatium: ids
 * (componens_invenire_in_spatio), actiones (registrum cum spatiis,
 * spatium componentis), figurae (pingere spatium portat), dispensator
 * (focus, captura et Tab intra Motus.spatium). Spatium "" = mores
 * hodierni. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "xar.h"
#include "insula.h"
#include "componens.h"
#include "actio.h"
#include "figura.h"
#include "mandatum.h"
#include "motus.h"
#include "dispensator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* status applicationis ludicrae, unus per spatium */
nomen structura {
    i32 claves;
    i32 ictus;
    i32 figurae;
    i32 petitiones;
} Ludicra;

hic_manens Ludicra sinistra;
hic_manens Ludicra dextra;
hic_manens Ludicra hospes;

hic_manens constans character spatium_a[] = "1.sinistrum.toy";
hic_manens constans character spatium_b[] = "1.dextrum.toy";

interior chorda
chorda_internata (
    constans character* litterae)
{
    chorda* c;
    chorda  vacua;

    /* internamentum "" recusat (NIHIL): spatium hospitis = vacua */
    c = chorda_internare_ex_literis(intern, litterae);
    si (c)
    {
        redde *c;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* actio 'pagina.clavis': claves et ictus numerat; Tab non consumit */
interior b32
paginam_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    Ludicra* l;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    l = (Ludicra*)ctx;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (ev->datum.clavis.clavis == CLAVIS_TABULA)
        {
            redde FALSUM;
        }
        l->claves++;
        redde VERUM;
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        l->ictus++;
        redde VERUM;
    }
    redde FALSUM;
}

/* actio radicum: petitiones foci numerat et consumit */
interior b32
radicem_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    si (ev->genus != EVENTUS_FOCUS_PETITUS)
    {
        redde FALSUM;
    }
    ((Ludicra*)ctx)->petitiones++;
    redde VERUM;
}

interior vacuum
figuram_numerare (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    (vacuum)c;
    (vacuum)m;
    (vacuum)thema;
    ((Ludicra*)ctx)->figurae++;
}

interior Componens*
nodus (
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo)
{
    Componens* c;
        Fines  f;

    c           = componens_creare(piscina, intern, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(c, f);
    redde c;
}

interior Componens*
focusabile (
     constans character* id,
                    s32  x)
{
    Componens* c;

    c = nodus(id, PARTES_CAMPUS, x, X, XXX, XXX);
    componens_ponere_actio(c, "pagina.clavis");
    componens_ponere_focusabilis(c, VERUM);
    redde c;
}

/* radix (hospes, 200 x 100): latus A (0..100: pagina, alia), latus B
 * (100..200: tertia, pagina) - ordo B tertiam PRIMAM ponit, ut Tab
 * sine spatio ex 'alia' in B transiret. ctx non NIHIL: radices
 * actiones habent (hospes.radix, latus.radix - VI) */
interior Componens*
componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    Componens* radix;
    Componens* a;
    Componens* b;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)p;
    (vacuum)in;
    radix  = nodus("radix", PARTES_NULLUM, ZEPHYRUM, ZEPHYRUM, CC, C);
    a      = nodus("radix", PARTES_NULLUM, ZEPHYRUM, ZEPHYRUM, C, C);
    componens_ponere_spatium(a, spatium_a);
    componens_addere_liberum(a, focusabile("pagina", X));
    componens_addere_liberum(a, focusabile("alia", L));
    b = nodus("radix", PARTES_NULLUM, C, ZEPHYRUM, C, C);
    componens_ponere_spatium(b, spatium_b);
    componens_addere_liberum(b, focusabile("tertia", X));
    componens_addere_liberum(b, focusabile("pagina", L));
    si (ctx)
    {
        componens_ponere_actio(radix, "hospes.radix");
        componens_ponere_actio(a, "latus.radix");
        componens_ponere_actio(b, "latus.radix");
    }
    componens_addere_liberum(radix, a);
    componens_addere_liberum(radix, b);
    redde radix;
}

interior Eventus
ictus (
    s64 t,
    s32 x,
    s32 y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
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
    e.datum.clavis.clavis    = (clavis_t)typus;
    redde e;
}

interior vacuum
componentia_probare (vacuum)
{
    Componens* arbor;
    Componens* c;

    imprimere("\n--- I: componens - spatium et ids ---\n");
    arbor = componere(NIHIL, NIHIL, piscina, intern, NIHIL);
    CREDO_VERUM(chorda_vacua(componens_spatium(arbor)));
    /* spatium hereditur: pagina sinistra in A */
    c = componens_liberum(componens_liberum(arbor, ZEPHYRUM), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(componens_spatium(c), spatium_a);
    c = componens_liberum(componens_liberum(arbor, I), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(componens_spatium(c), spatium_b);
    /* 'pagina' in B = liberum secundum lateris B */
    CREDO_VERUM(componens_invenire_in_spatio(arbor,
        chorda_internata(spatium_b), chorda_internata("pagina")) == c);
    c = componens_invenire_in_spatio(arbor,
        chorda_internata(spatium_a), chorda_internata("pagina"));
    CREDO_VERUM(c == componens_liberum(componens_liberum(arbor,
        ZEPHYRUM), ZEPHYRUM));
    /* radix spatii ipsa intra spatium (id 'radix' trium) */
    CREDO_VERUM(componens_invenire_in_spatio(arbor,
        chorda_internata(spatium_b), chorda_internata("radix"))
        == componens_liberum(arbor, I));
    /* "" = mores hodierni: primus in arbore tota */
    CREDO_VERUM(componens_invenire_in_spatio(arbor, chorda_internata(
        ""), chorda_internata("pagina")) == c);
    CREDO_VERUM(componens_invenire_per_id(arbor, chorda_internata(
        "pagina")) == c);
    /* absentia: id in altero spatio solo, spatium ignotum */
    CREDO_NIHIL(componens_invenire_in_spatio(arbor,
        chorda_internata(spatium_a), chorda_internata("tertia")));
    CREDO_NIHIL(componens_invenire_in_spatio(arbor,
        chorda_internata("2.sinistrum.toy"),
        chorda_internata("pagina")));
    /* spatium nidificatum non intratur */
    {
        Componens* nidus;

        nidus = nodus("nidus", PARTES_NULLUM, ZEPHYRUM, ZEPHYRUM, X, X);
        componens_ponere_spatium(nidus, "interior");
        componens_addere_liberum(nidus, focusabile("abdita", ZEPHYRUM));
        componens_addere_liberum(componens_liberum(arbor, ZEPHYRUM),
            nidus);
        CREDO_NIHIL(componens_invenire_in_spatio(arbor,
            chorda_internata(spatium_a), chorda_internata("abdita")));
        CREDO_NON_NIHIL(componens_invenire_in_spatio(arbor,
            chorda_internata("interior"), chorda_internata("abdita")));
        /* nidus ipse (radix spatii sui) non in A */
        CREDO_NIHIL(componens_invenire_in_spatio(arbor,
            chorda_internata(spatium_a), chorda_internata("nidus")));
    }
}

interior vacuum
actiones_probare (vacuum)
{
    ActioRegistrum* reg;
    ActioRegistrum* fa;
    ActioRegistrum* fb;
           ActioFn  fn;
            vacuum* ctx;
         Componens* arbor;
               Xar* x;

    imprimere("\n--- II: actiones in spatiis ---\n");
    reg  = actio_registrum_creare(piscina, intern);
    fa   = actio_registrum_creare(piscina, intern);
    fb   = actio_registrum_creare(piscina, intern);
    CREDO_VERUM(actio_registrare(fa, "pagina.clavis", paginam_tractare,
        &sinistra));
    CREDO_VERUM(actio_registrare(fb, "pagina.clavis", paginam_tractare,
        &dextra));
    CREDO_VERUM(actio_registrum_miscere_in_spatio(reg, fa,
        chorda_internata(spatium_a)));
    CREDO_VERUM(actio_registrum_miscere_in_spatio(reg, fb,
        chorda_internata(spatium_b)));
    /* collisio solum intra idem spatium */
    CREDO_FALSUM(actio_registrum_miscere_in_spatio(reg, fa,
        chorda_internata(spatium_a)));
    CREDO_VERUM(actio_invenire_in_spatio(reg, chorda_internata(
        spatium_a), chorda_internata("pagina.clavis"), &fn, &ctx));
    CREDO_VERUM(ctx == &sinistra);
    CREDO_VERUM(actio_invenire_in_spatio(reg, chorda_internata(
        spatium_b), chorda_internata("pagina.clavis"), &fn, &ctx));
    CREDO_VERUM(ctx == &dextra);
    /* hospes ("") non videt spatia; spatia non vident hospitem */
    CREDO_FALSUM(actio_invenire(reg, chorda_internata("pagina.clavis"),
        &fn, &ctx));
    CREDO_VERUM(actio_registrare(reg, "hospes.radix", paginam_tractare,
        &hospes));
    CREDO_VERUM(actio_invenire(reg, chorda_internata("hospes.radix"),
        &fn, &ctx));
    CREDO_VERUM(actio_invenire_in_spatio(reg, chorda_internata(""),
        chorda_internata("hospes.radix"), &fn, &ctx));
    CREDO_FALSUM(actio_invenire_in_spatio(reg, chorda_internata(
        spatium_a), chorda_internata("hospes.radix"), &fn, &ctx));
    /* idem nomen in "" iterum licet (spatium aliud) */
    CREDO_VERUM(actio_registrare(reg, "pagina.clavis", paginam_tractare,
        &hospes));
    CREDO_FALSUM(actio_registrare(reg, "pagina.clavis",
        paginam_tractare, &hospes));
    /* L10 per spatia: arbor relata = registrata */
    reg = actio_registrum_creare(piscina, intern);
    CREDO_VERUM(actio_registrum_miscere_in_spatio(reg, fa,
        chorda_internata(spatium_a)));
    arbor  = componere(NIHIL, NIHIL, piscina, intern, NIHIL);
    x      = actio_non_registratae(reg, arbor, piscina);
    /* B relata, in B non registrata */
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_VERUM(actio_registrum_miscere_in_spatio(reg, fb,
        chorda_internata(spatium_b)));
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_registratae(reg, arbor,
        piscina)), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_relatae(reg, arbor,
        piscina)), ZEPHYRUM);
    /* in "" registrata, a nullo componente "" relata */
    CREDO_VERUM(actio_registrare(reg, "pagina.clavis", paginam_tractare,
        &hospes));
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_relatae(reg, arbor,
        piscina)), I);
}

interior vacuum
figurae_probare (vacuum)
{
    FiguraRegistrum* reg;
    FiguraRegistrum* fa;
    FiguraRegistrum* fb;
           FiguraFn  fn;
             vacuum* ctx;
            Mandata* m;

    imprimere("\n--- III: figurae in spatiis ---\n");
    memset(&sinistra, ZEPHYRUM, magnitudo(Ludicra));
    memset(&dextra, ZEPHYRUM, magnitudo(Ludicra));
    memset(&hospes, ZEPHYRUM, magnitudo(Ludicra));
    reg  = figura_registrum_creare(piscina);
    fa   = figura_registrum_creare(piscina);
    fb   = figura_registrum_creare(piscina);
    CREDO_VERUM(figura_registrare(fa, PARTES_CAMPUS, ZEPHYRUM,
        figuram_numerare, &sinistra));
    CREDO_VERUM(figura_registrare(fb, PARTES_CAMPUS, ZEPHYRUM,
        figuram_numerare, &dextra));
    CREDO_VERUM(figura_registrare(reg, PARTES_NULLUM, ZEPHYRUM,
        figuram_numerare, &hospes));
    CREDO_VERUM(figura_registrum_miscere_in_spatio(reg, fa,
        chorda_internata(spatium_a)));
    CREDO_VERUM(figura_registrum_miscere_in_spatio(reg, fb,
        chorda_internata(spatium_b)));
    CREDO_FALSUM(figura_registrum_miscere_in_spatio(reg, fb,
        chorda_internata(spatium_b)));
    CREDO_VERUM(figura_invenire_in_spatio(reg, chorda_internata(
        spatium_b), PARTES_CAMPUS, ZEPHYRUM, &fn, &ctx));
    CREDO_VERUM(ctx == &dextra);
    CREDO_FALSUM(figura_invenire(reg, PARTES_CAMPUS, ZEPHYRUM, &fn,
        &ctx));
    CREDO_VERUM(figura_invenire(reg, PARTES_NULLUM, ZEPHYRUM, &fn,
        &ctx));
    /* pingere: campi per spatium suum; radix sola hospitis (latera
     * radices spatiorum sunt, NULLUM in spatiis non registratum) */
    m = mandata_creare(piscina, intern);
    pingere(componere(NIHIL, NIHIL, piscina, intern, NIHIL), reg,
        ZEPHYRUM, m);
    CREDO_AEQUALIS_I32(sinistra.figurae, II);
    CREDO_AEQUALIS_I32(dextra.figurae, II);
    CREDO_AEQUALIS_I32(hospes.figurae, I);
    /* pingere a subarbore: spatium ex ascendentibus */
    pingere(componens_liberum(componens_liberum(componere(NIHIL, NIHIL,
        piscina, intern, NIHIL), I), ZEPHYRUM), reg, ZEPHYRUM, m);
    CREDO_AEQUALIS_I32(dextra.figurae, III);
}

interior vacuum
dispensatorem_probare (vacuum)
{
    InsulaRepositorium* repo;
        ActioRegistrum* reg;
        ActioRegistrum* fa;
        ActioRegistrum* fb;
           Dispensator* d;
               Eventus  e;

    imprimere("\n--- IV: dispensator - focus et actiones ---\n");
    memset(&sinistra, ZEPHYRUM, magnitudo(Ludicra));
    memset(&dextra, ZEPHYRUM, magnitudo(Ludicra));
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>",
        "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    fa   = actio_registrum_creare(piscina, intern);
    fb   = actio_registrum_creare(piscina, intern);
    (vacuum)actio_registrare(fa, "pagina.clavis", paginam_tractare,
        &sinistra);
    (vacuum)actio_registrare(fb, "pagina.clavis", paginam_tractare,
        &dextra);
    (vacuum)actio_registrare(fa, "latus.radix", radicem_tractare,
        &sinistra);
    (vacuum)actio_registrare(fb, "latus.radix", radicem_tractare,
        &dextra);
    (vacuum)actio_registrare(reg, "hospes.radix", radicem_tractare,
        &hospes);
    (vacuum)actio_registrum_miscere_in_spatio(reg, fa,
        chorda_internata(spatium_a));
    (vacuum)actio_registrum_miscere_in_spatio(reg, fb,
        chorda_internata(spatium_b));
    memset(&hospes, ZEPHYRUM, magnitudo(Ludicra));
    d = dispensator_creare(piscina, intern, repo, reg, componere,
        &hospes,
        CCC);
    CREDO_NON_NIHIL(d);
    si (!d)
    {
        redde;
    }
    /* focus 'pagina' in spatio B: clavis ad dextram */
    dispensator_motus(d)->spatium = chorda_internata(spatium_b);
    dispensator_focus_ponere(d, chorda_internata("pagina"));
    e = clavis(M, 'x');
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(dextra.claves, I);
    CREDO_AEQUALIS_I32(sinistra.claves, ZEPHYRUM);
    /* idem id, spatium A: ad sinistram */
    dispensator_motus(d)->spatium  = chorda_internata(spatium_a);
    e                              = clavis(M + C, 'y');
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(sinistra.claves, I);
    CREDO_AEQUALIS_I32(dextra.claves, I);
    /* ictus: actio per spatium COMPONENTIS (tertia in B), quodcumque
     * Motus.spatium */
    e = ictus(M + CC, CXV, XV);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(dextra.ictus, I);
    CREDO_AEQUALIS_I32(sinistra.ictus, ZEPHYRUM);

    imprimere("\n--- V: Tab intra spatium ---\n");
    dispensator_motus(d)->spatium = chorda_internata(spatium_a);
    dispensator_focus_ponere(d, chorda_internata("alia"));
    e = clavis(M + CCC, '\t');
    dispensator_tractare(d, &e);
    /* sine spatio 'tertia' (B) sequeretur */
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "pagina");
    e = clavis(M + CD, 'z');
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(sinistra.claves, II);
    CREDO_AEQUALIS_I32(dextra.claves, I);

    imprimere("\n--- VI: focus et captura extra spatium ---\n");
    /* 'tertia' solum in B: in spatio A absens -> petitio foci ad
     * radicem HOSPITIS (spatium ""), non ad radicem lateris eiusdem
     * id 'radix' */
    dispensator_focus_ponere(d, chorda_internata("tertia"));
    e = clavis(M + D, 'q');
    dispensator_tractare(d, &e);
    CREDO_VERUM(hospes.petitiones > ZEPHYRUM);
    CREDO_AEQUALIS_I32(sinistra.petitiones, ZEPHYRUM);
    CREDO_AEQUALIS_I32(dextra.petitiones, ZEPHYRUM);
    CREDO_AEQUALIS_I32(dextra.claves, I);
    /* petitione vana focus TOLLITUR (attributum removetur - valor ""
     * internari non potest) */
    CREDO_VERUM(chorda_vacua(dispensator_focus(d)));
    /* idem sine spatio: id nusquam */
    dispensator_motus(d)->spatium = chorda_internata("");
    dispensator_focus_ponere(d, chorda_internata("nusquam"));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "nusquam");
    e = clavis(M + DL, 'r');
    dispensator_tractare(d, &e);
    CREDO_VERUM(chorda_vacua(dispensator_focus(d)));
    /* captura 'pagina' in spatio B: ictus super 'alia' (A) ad
     * paginam B */
    dispensator_motus(d)->spatium  = chorda_internata(spatium_b);
    dispensator_motus(d)->captura  = chorda_internata("pagina");
    e                              = ictus(M + DC, LXV, XXV);
    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32(dextra.ictus, II);
    CREDO_AEQUALIS_I32(sinistra.ictus, ZEPHYRUM);
}

s32
principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_spatium", LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    componentia_probare();
    actiones_probare();
    figurae_probare();
    dispensatorem_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
