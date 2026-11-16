/* antiphonale.c - cantus gabc in latere vici (vide
 * include/antiphonale.h)
 *
 * Involucrum plagulae circa visum (gabc_visus.h; psalterium planum
 * T2): via, specula, lectio, vexilla plagulae, tabula clavium. Arbor:
 * radix "antiphonale" (PARTES_TABULA, figura tota); liberos lineae
 * imae visus addit. Figura solum legit et mandata emittit.
 */
#include "antiphonale.h"
#include "membrum__praecepta.h"
#include "specula.h"
#include "reactor.h"
#include "filum.h"
#include "motus.h"
#include <stdio.h>
#include <string.h>

#define ANTIPHONALE_VIA        DXII
#define ANTIPHONALE_VEXILLUM   CCLVI
/* sine specula (directorium absens, platforma): relegere et speculam
 * iterum temptare omni CCL ms */
#define MORA_SINE_SPECULA      CCL
#define VISUS_ANTIPHONALIS     I

hic_manens constans character actio_antiphonalis[] =
    "antiphonale.agere";

nomen structura {
     character  via[ANTIPHONALE_VIA];
       Specula* specula;
       Piscina* piscina_speculae; /* speculae solius: temptatio fracta
                                   * aut via mutata eam destruit */
     GabcVisus* visus;
} Antiphonale;


/* ==================================================
 * Lectio (G13)
 * ================================================== */

/* plagulam legit: absens aut vacua -> vexillum (versio servata);
 * aliter textus visui datur (G18: continuare). VERUM si quid mutatum */
interior b32
relegere (
    AntiphonaleMontatio* m)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;
      character  prior[ANTIPHONALE_VEXILLUM];
      character  vexillum[ANTIPHONALE_VEXILLUM];
        Piscina* p;
         chorda  textus;
            b32  mutatum;

    si (a->via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    strncpy(prior, gabc_visus_vexillum(a->visus), ANTIPHONALE_VEXILLUM
        - I);
    prior[ANTIPHONALE_VEXILLUM - I] = '\0';
    si (!filum_existit(a->via))
    {
        sprintf(vexillum, "missing: %.200s", a->via);
        gabc_visus_vexillum_ponere(a->visus, vexillum);
        redde strcmp(prior, vexillum) != ZEPHYRUM;
    }
    p = piscina_generare_dynamicum("antiphonale_lectio",
        (memoriae_index)(LXIV * MXXIV));
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    textus = filum_legere_totum(a->via, p);
    si (textus.mensura == ZEPHYRUM)
    {
        gabc_visus_vexillum_ponere(a->visus, "empty file");
        mutatum = strcmp(prior, "empty file") != ZEPHYRUM;
    }
    alioquin
    {
        mutatum = gabc_visus_textum_ponere(a->visus, textus, VERUM);
    }
    piscina_destruere(p);
    redde mutatum;
}

interior vacuum
speculam_claudere (
    Antiphonale* a)
{
    si (a->specula)
    {
        specula_claudere(a->specula);
        a->specula = NIHIL;
    }
    si (a->piscina_speculae)
    {
        piscina_destruere(a->piscina_speculae);
        a->piscina_speculae = NIHIL;
    }
}

/* specula in piscina sua: temptatio fracta (directorium absens) nihil
 * relinquit, ergo iterum temptari potest sine fine */
interior vacuum
speculam_aperire (
    Antiphonale* a)
{
    speculam_claudere(a);
    a->piscina_speculae = piscina_generare_dynamicum(
        "antiphonale_specula", (memoriae_index)M);
    si (a->piscina_speculae == NIHIL)
    {
        redde;
    }
    a->specula = specula_creare(a->piscina_speculae, a->via);
    si (a->specula == NIHIL)
    {
        speculam_claudere(a);
    }
}

interior vacuum
viam_ponere (
    AntiphonaleMontatio* m,
     constans character* via)
{
    Antiphonale* a = (Antiphonale*)m->figurae_ctx;

    speculam_claudere(a);
    gabc_visus_vacuare(a->visus);
    a->via[ZEPHYRUM] = '\0';
    si (via && via[ZEPHYRUM])
    {
        strncpy(a->via, via, ANTIPHONALE_VIA - I);
        a->via[ANTIPHONALE_VIA - I] = '\0';
        speculam_aperire(a);
    }
    (vacuum)relegere(m);
}


/* ==================================================
 * Praecepta
 * ================================================== */

interior GabcVisus*
visus_eius (
    vacuum* ctx)
{
    redde ((Antiphonale*)((AntiphonaleMontatio*)ctx)->figurae_ctx)->visus;
}

interior b32
fac_proximam (
    vacuum* ctx)
{
    redde gabc_visus_paginam_movere(visus_eius(ctx), I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    redde gabc_visus_paginam_movere(visus_eius(ctx), -I);
}

interior b32
fac_maius (
    vacuum* ctx)
{
    redde gabc_visus_spatium_movere(visus_eius(ctx), II);
}

interior b32
fac_minus (
    vacuum* ctx)
{
    redde gabc_visus_spatium_movere(visus_eius(ctx), -II);
}

interior b32
fac_canere (
    vacuum* ctx)
{
    redde gabc_visus_canere(visus_eius(ctx));
}

interior b32
fac_priorem_syllabam (
    vacuum* ctx)
{
    redde gabc_visus_syllabam_movere(visus_eius(ctx), -I);
}

interior b32
fac_proximam_syllabam (
    vacuum* ctx)
{
    redde gabc_visus_syllabam_movere(visus_eius(ctx), I);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "antiphonale.canere", "Play / stop", " ", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_canere },
    { "antiphonale.syllaba_prior", "Previous syllable", "Left",
        VISUS_ANTIPHONALIS, ZEPHYRUM, fac_priorem_syllabam },
    { "antiphonale.syllaba_proxima", "Next syllable", "Right",
        VISUS_ANTIPHONALIS, ZEPHYRUM, fac_proximam_syllabam },
    { "antiphonale.proxima", "Next page", "PageDown",
        VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_proximam },
    { "antiphonale.prior", "Previous page", "PageUp",
        VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_priorem },
    { "antiphonale.deorsum", "Next page", "j", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_proximam },
    { "antiphonale.sursum", "Previous page", "k", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_priorem },
    { "antiphonale.maius", "Zoom in", "+", VISUS_ANTIPHONALIS, ZEPHYRUM,
        fac_maius },
    { "antiphonale.maius_aequale", "Zoom in", "=", VISUS_ANTIPHONALIS,
        ZEPHYRUM, fac_maius },
    { "antiphonale.minus", "Zoom out", "-", VISUS_ANTIPHONALIS,
        ZEPHYRUM,
        fac_minus },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
requisita_nulla (
        i32  requisita,
     vacuum* ctx)
{
    (vacuum)requisita;
    (vacuum)ctx;
    redde VERUM;
}


/* ==================================================
 * Actio
 * ================================================== */

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Praeceptum* t;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_ANTIPHONALIS,
            ev);
        redde t && praecepta_status(t, VISUS_ANTIPHONALIS,
            requisita_nulla, m) == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    redde gabc_visus_premere(visus_eius(m), nodus, ev);
}


/* ==================================================
 * Titulus
 * ================================================== */

/* name capitis aut nomen plagulae; vacuum si nulla */
interior chorda
titulum_capere (
    constans AntiphonaleMontatio* m)
{
    constans Antiphonale* a = (constans Antiphonale*)m->figurae_ctx;
      constans character* ultimum;
                  chorda  t;

    t = gabc_visus_titulus(a->visus);
    si (t.mensura > ZEPHYRUM)
    {
        redde t;
    }
    ultimum    = strrchr(a->via, '/');
    ultimum    = ultimum ? ultimum + I : a->via;
    t.datum    = (i8*)NIHIL;
    t.mensura  = ZEPHYRUM;
    si (*ultimum)
    {
        t = chorda_ex_literis(ultimum, m->piscina);
    }
    redde t;
}

chorda
antiphonale_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
                 chorda  t;

    t = titulum_capere(m);
    si (t.mensura == ZEPHYRUM)
    {
        t = chorda_ex_literis("Antiphonale", piscina);
    }
    redde t;
}


/* ==================================================
 * Figura
 * ================================================== */

interior vacuum
figura_antiphonalis (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
            Antiphonale* a;

    (vacuum)thema;
    si (!m || !m->figurae_ctx)
    {
        redde;
    }
    a = (Antiphonale*)m->figurae_ctx;
    gabc_visus_pingere(a->visus, md, c->fines,
        chorda_aequalis_literis(c->titulus, "focatum"),
        titulum_capere(m),
        a->via[ZEPHYRUM]
            == '\0' ? "Open a chant with $antiphonale(path)"
                                  : NIHIL);
}


/* ==================================================
 * Componere, montare, pulsus
 * ================================================== */

Componens*
antiphonale_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
              Componens* radix;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "antiphonale",
        PARTES_TABULA);
    si (!radix)
    {
        redde radix;
    }
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_antiphonalis);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    gabc_visus_componere(visus_eius(m), radix, piscina, intern);
    redde radix;
}

b32
antiphonale_montare (
          AntiphonaleMontatio* m,
                      Piscina* piscina,
          InternamentumChorda* intern,
           InsulaRepositorium* repo,
           constans character* id,
           constans character* via,
                          i32  latitudo,
                          i32  altitudo,
    constans AntiphonalePorta* porta,
              StatusSystemata* systemata)
{
        Organum* organum;
    Antiphonale* a;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(AntiphonaleMontatio));
    m->piscina   = piscina;
    m->intern    = intern;
    m->latitudo  = latitudo;
    m->altitudo  = altitudo;
    si (porta)
    {
        m->porta = *porta;
    }
    m->ramus = id ? insula_ramus(repo, "antiphonale", id)
                  : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "antiphonale",
        "antiphonale");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"antiphonale\"", id ? id : "antiphonale",
               piscina, intern))
    {
        redde FALSUM;
    }
    a = (Antiphonale*)piscina_allocare(piscina, magnitudo(Antiphonale));
    si (a == NIHIL)
    {
        redde FALSUM;
    }
    memset(a, ZEPHYRUM, magnitudo(Antiphonale));
    a->visus = gabc_visus_creare(piscina, &m->ramus, actio_antiphonalis,
        "antiphonale", &m->porta, latitudo, altitudo, ZEPHYRUM);
    m->figurae_ctx  = a;
    m->actiones     = actio_registrum_creare(piscina, intern);
    m->figurae      = figura_registrum_creare(piscina);
    si (   !a->visus || !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_antiphonalis, agere, m)
        || !figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_antiphonalis, m))
    {
        redde FALSUM;
    }
    viam_ponere(m, via);
    redde VERUM;
}

b32
antiphonale_argumentum_ponere (
       AntiphonaleMontatio* m,
        constans character* via)
{
    Antiphonale* a;

    si (!m || !m->figurae_ctx || !via || via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    a = (Antiphonale*)m->figurae_ctx;
    si (strcmp(a->via, via) != ZEPHYRUM)
    {
        viam_ponere(m, via);
        (vacuum)gabc_visus_paginam_ponere(a->visus, ZEPHYRUM);
    }
    redde VERUM;
}

b32
antiphonale_pulsare (
    vacuum* ctx)
{
    AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
            Antiphonale* a;
                    b32  mutatum;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    a = (Antiphonale*)m->figurae_ctx;
    si (a->via[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    mutatum = FALSUM;
    si (a->specula == NIHIL)
    {
        /* platforma sine specula aut directorium absens: relegere et
         * iterum temptare omni pulsu (MORA_SINE_SPECULA) */
        speculam_aperire(a);
        mutatum = relegere(m);
    }
    alioquin si (specula_exhaurire(a->specula))
    {
        mutatum = relegere(m);
    }
    si (gabc_visus_pulsare(a->visus))
    {
        mutatum = VERUM;
    }
    redde mutatum;
}

Mora
antiphonale_mora (
    vacuum* ctx)
{
     AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Antiphonale* a;
                    Mora  mora;

    si (!m || !m->figurae_ctx)
    {
        redde -I;
    }
    a = (constans Antiphonale*)m->figurae_ctx;
    si (a->via[ZEPHYRUM] == '\0')
    {
        redde -I;
    }
    mora = gabc_visus_mora(a->visus);
    si (mora >= ZEPHYRUM)
    {
        redde mora;
    }
    redde a->specula ? -I : MORA_SINE_SPECULA;
}

s32
antiphonale_fossa (
    vacuum* ctx,
       i32* eventus)
{
     AntiphonaleMontatio* m = (AntiphonaleMontatio*)ctx;
    constans Antiphonale* a;

    *eventus = REACTOR_LEGERE;
    si (!m || !m->figurae_ctx)
    {
        redde -I;
    }
    a = (constans Antiphonale*)m->figurae_ctx;
    redde a->specula ? specula_fossa(a->specula) : -I;
}
