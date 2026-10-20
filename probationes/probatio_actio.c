/* probatio_actio.c - registrum actionum; resolutio UTRIMQUE (L10) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "componens.h"
#include "actio.h"
#include "credo.h"
#include <stdio.h>

/* Tractator probationis: numerat, consumit */
interior b32
numerare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* d,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)repo;
    (vacuum)motus;
    (vacuum)d;
    (vacuum)nodus;
    (vacuum)ev;
    (*(i32*)ctx)++;
    redde VERUM;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
         ActioRegistrum* reg;
              Componens* radix;
              Componens* b;
                ActioFn  fn;
                 vacuum* ctx;
                    i32  numerus;
                    Xar* desunt;
                    Xar* otiosae;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    numerus  = ZEPHYRUM;

    reg = actio_registrum_creare(piscina, intern);
    CREDO_NON_NIHIL(reg);
    CREDO_VERUM(actio_registrare(reg, "numerare", numerare, &numerus));
    CREDO_VERUM(actio_registrare(reg, "fugere", numerare, &numerus));
    /* duplex recusatur */
    CREDO_FALSUM(actio_registrare(reg, "numerare", numerare, &numerus));
    /* titulus vacuus recusatur (internamentum vacuum = NIHIL) */
    CREDO_FALSUM(actio_registrare(reg, "", numerare, &numerus));
    CREDO_FALSUM(actio_registrare(reg, "sine", NIHIL, &numerus));
    CREDO_AEQUALIS_I32(xar_numerus(reg->nomina), II);
    fn   = NIHIL;
    ctx  = NIHIL;
    CREDO_VERUM(actio_invenire(reg, chorda_ex_literis("numerare",
        piscina),
                               &fn, &ctx));
    CREDO_VERUM(fn == numerare);
    CREDO_VERUM(ctx == (vacuum*)&numerus);
    CREDO_VERUM(fn(NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, ctx));
    CREDO_AEQUALIS_I32(numerus, I);
    CREDO_FALSUM(actio_invenire(reg, chorda_ex_literis("ignota",
        piscina),
                                &fn, &ctx));
    CREDO_FALSUM(actio_invenire(NIHIL, chorda_ex_literis("numerare",
                                piscina), &fn, &ctx));

    CREDO_GRADUS("Resolutio utrimque");
    radix  = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    b      = componens_creare(piscina, intern, "b", PARTES_BOTTONE);
    componens_ponere_actio(b, "numerare");
    componens_addere_liberum(radix, b);
    b     = componens_creare(piscina, intern, "c", PARTES_BOTTONE);
    componens_ponere_actio(b, "ignota");
    componens_addere_liberum(radix, b);
    /* eadem actio ignota bis relata: semel deest */
    b     = componens_creare(piscina, intern, "c2", PARTES_BOTTONE);
    componens_ponere_actio(b, "ignota");
    componens_addere_liberum(radix, b);

    desunt   = actio_non_registratae(reg, radix, piscina);
    otiosae  = actio_non_relatae(reg, radix, piscina);
    CREDO_NON_NIHIL(desunt);
    CREDO_NON_NIHIL(otiosae);
    CREDO_AEQUALIS_I32(xar_numerus(desunt), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(desunt,
        ZEPHYRUM),
                                  "ignota");
    CREDO_AEQUALIS_I32(xar_numerus(otiosae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(
        *(chorda*)xar_obtinere(otiosae, ZEPHYRUM), "fugere");

    CREDO_GRADUS("Arbor sine actionibus: omnes otiosae");
    radix = componens_creare(piscina, intern, "sola", PARTES_NULLUM);
    CREDO_AEQUALIS_I32(
        xar_numerus(actio_non_registratae(reg, radix, piscina)),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(
        xar_numerus(actio_non_relatae(reg, radix, piscina)), II);
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                ActioFn  fn;
                 vacuum* ctx;
                    i32  numerus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    numerus  = ZEPHYRUM;
    {
         ActioRegistrum* hospes;
         ActioRegistrum* app;
                    i32  alius;

        alius   = ZEPHYRUM;
        hospes  = actio_registrum_creare(piscina, intern);
        app     = actio_registrum_creare(piscina, intern);
        CREDO_VERUM(actio_registrare(hospes, "tabula.eligere", numerare,
            &numerus));
        CREDO_VERUM(actio_registrare(app, "pagina.clavis", numerare,
            &alius));
        CREDO_VERUM(actio_registrum_miscere(hospes, app));
        CREDO_VERUM(actio_invenire(hospes, chorda_ex_literis(
            "pagina.clavis", piscina), &fn, &ctx));
        CREDO_VERUM(ctx == &alius);
        /* collisio: recusatur, nihil additur */
        CREDO_FALSUM(actio_registrum_miscere(hospes, app));
        actio_registrum_vacare(hospes);
        CREDO_FALSUM(actio_invenire(hospes, chorda_ex_literis(
            "tabula.eligere", piscina), &fn, &ctx));
        CREDO_FALSUM(actio_invenire(hospes, chorda_ex_literis(
            "pagina.clavis", piscina), &fn, &ctx));
        /* iterum utilis */
        CREDO_VERUM(actio_registrare(hospes, "tabula.eligere", numerare,
            &numerus));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "Registrare et invenire", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "T2a: vacare et miscere (vicus)", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("actio", SECTIONES);
}
