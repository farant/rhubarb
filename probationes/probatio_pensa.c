/* probatio_pensa.c - index cotidianus (pensa K2): forma (sectiones,
 * dedentatio, capita cum liberis, res cotidianae, vitia), res diei,
 * signa (acta, ultimum vincit, documenta duo), textus libri. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "fasti.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "recursus.h"
#include "pensa.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* greges ut "schedula:res|res;schedula:res" (schedula vacua: "*";
 * vitium: "!schedula") */
interior chorda
formam_describere (
    constans PensaForma* f,
                Piscina* p)
{
    ChordaAedificator* a;
                  i32  i;
                  i32  k;

    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    per (i = ZEPHYRUM; f && i < xar_numerus(f->greges); i++)
    {
        constans PensaGrex* g;

        g = (constans PensaGrex*)xar_obtinere(f->greges, i);
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ';');
        }
        si (!g->recursus)
        {
            chorda_aedificator_appendere_character(a, '!');
        }
        si (g->schedula.mensura == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '*');
        }
        chorda_aedificator_appendere_chorda(a, g->schedula);
        chorda_aedificator_appendere_character(a, ':');
        per (k = ZEPHYRUM; k < xar_numerus(g->res); k++)
        {
            si (k > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '|');
            }
            chorda_aedificator_appendere_chorda(a,
                *(chorda*)xar_obtinere(g->res, k));
        }
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
formam (
    constans character* textus,
               Piscina* p)
{
    redde formam_describere(pensa_formam_legere(chorda_ex_literis(textus,
        p), p), p);
}

/* res diei ut "a|b|c" */
interior chorda
res_diei (
     constans character* textus,
                   Dies  d,
                Piscina* p)
{
    ChordaAedificator* a;
                  Xar* l;
                  i32  i;

    l = pensa_res_diei(pensa_formam_legere(chorda_ex_literis(textus, p),
        p), d, p);
    a = chorda_aedificator_creare(p, (memoriae_index)LXIV);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '|');
        }
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(l,
            i));
    }
    redde chorda_aedificator_finire(a);
}

#define FORMA_EXEMPLI \
    "vitamins\n" \
    "morning offering\n" \
    "\n" \
    "weekends\n" \
    "    mow the lawn\n" \
    "    vitamins\n" \
    "\n" \
    "    every other monday from oct 26 2026\n" \
    "        call mom\n" \
    "\n" \
    "fortnightly\n" \
    "    water plants\n" \
    "evening prayer\n"

interior vacuum
_sectio_forma (
    CredoContextus* c)
{
    Piscina* p;

    p = c->piscina;
    CREDO_GRADUS("exemplum: sectiones, dedentatio, vitium, cotidiana");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam(FORMA_EXEMPLI, p),
        "*:vitamins|morning offering;weekends:mow the lawn|vitamins;"
        "every other monday from oct 26 2026:call mom;"
        "!fortnightly:water plants;*:evening prayer");

    CREDO_GRADUS("textus vacuus; lineae vacuae solae");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("", p), "");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("\n  \n\n", p), "");

    CREDO_GRADUS("sectio tota indentata sine liberis: cotidiana");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("   x\n   y", p), "*:x|y");

    CREDO_GRADUS("tab, CRLF, spatia finalia, linea ultima sine fine");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam(
        "sundays  \r\n\tmass \r\n\r\nrosary", p),
        "sundays:mass;*:rosary");

    CREDO_GRADUS("liberi profundiores: res eiusdem capitis");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("weekdays\n  a\n    b\n  c",
        p),
        "weekdays:a|b|c");

    CREDO_GRADUS("linea prima magis indentata quam sequens");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("  a\nmondays\n  c", p),
        "*:a;mondays:c");

    CREDO_GRADUS("in sectione una: cotidiana, caput, cotidiana - ordo");
    CREDO_CHORDA_AEQUALIS_LITERIS(formam("a\nmondays\n  x\nb", p),
        "*:a;mondays:x;*:b");

    CREDO_GRADUS("vitium in grege servatur");
    {
        PensaForma* f;
         PensaGrex* g;

        f = pensa_formam_legere(chorda_ex_literis("fortnightly\n  x",
            p), p);
        CREDO_NECESSE(f && xar_numerus(f->greges) == I);
        g = (PensaGrex*)xar_obtinere(f->greges, ZEPHYRUM);
        CREDO_VERUM(g->recursus == NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS(g->vitium,
            "unknown word \"fortnightly\"");
    }
}

interior vacuum
_sectio_dies (
    CredoContextus* c)
{
    Piscina* p;

    p = c->piscina;
    CREDO_GRADUS("sabbatum: cotidiana, weekends, iterata semel");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXIV), p),
        "vitamins|morning offering|mow the lawn|evening prayer");
    CREDO_GRADUS("lunae 26 Oct: call mom; vitium nihil dat");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXVI), p),
        "vitamins|morning offering|call mom|evening prayer");
    CREDO_GRADUS("martis 27 Oct: nihil schedulae");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, X, XXVII), p),
        "vitamins|morning offering|evening prayer");
    CREDO_GRADUS("lunae 2 Nov: hebdomas altera, nulla");
    CREDO_CHORDA_AEQUALIS_LITERIS(res_diei(FORMA_EXEMPLI,
        fasti_dies(MMXXVI, XI, II), p),
        "vitamins|morning offering|evening prayer");
    CREDO_GRADUS("forma NIHIL: Xar vacuus");
    CREDO_AEQUALIS_I32(xar_numerus(pensa_res_diei(NIHIL,
        fasti_dies(MMXXVI, X, XXVI), p)), ZEPHYRUM);
}

interior vacuum
_sectio_signa (
    CredoContextus* c)
{
                Piscina* p;
    InternamentumChorda* intern;
                Volumen* vol;
        PensaDocumentum* a;
        PensaDocumentum* b;
                   Dies  d;
                   Dies  e;
                    s64  acta;

    p       = c->piscina;
    intern  = internamentum_creare(p);
    vol     = volumen_temporarium(p, "probatio_pensa");
    CREDO_NECESSE_NON_NIHIL(vol);
    d = fasti_dies(MMXXVI, X, XXIII);
    e = fasti_dies(MMXXVI, X, XXIV);
    a = pensa_aperire(p, intern, vol);
    CREDO_NECESSE_NON_NIHIL(a);

    CREDO_GRADUS("ponere, legere, idem iterum sine actu");
    CREDO_FALSUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        VERUM));
    CREDO_VERUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_FALSUM(pensa_factum(a, e, chorda_ex_literis("vitamins", p)));
    acta = volumen_summa_actorum(vol);
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        VERUM));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta);

    CREDO_GRADUS("tollere: ultimum vincit");
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis("vitamins", p),
        FALSUM));
    CREDO_FALSUM(pensa_factum(a, d, chorda_ex_literis("vitamins", p)));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol), acta + I);

    CREDO_GRADUS("textus cum signis notationis");
    CREDO_VERUM(pensa_ponere(a, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p), VERUM));
    CREDO_VERUM(pensa_factum(a, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p)));

    CREDO_GRADUS("documentum alterum videt; scriptura eius videtur");
    b = pensa_aperire(p, intern, vol);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_VERUM(pensa_factum(b, d, chorda_ex_literis(
        "R&D \"notes\" <x>", p)));
    CREDO_FALSUM(pensa_factum(b, d, chorda_ex_literis("vitamins", p)));
    CREDO_VERUM(pensa_ponere(b, e, chorda_ex_literis("mass", p),
        VERUM));
    CREDO_VERUM(pensa_factum(a, e, chorda_ex_literis("mass", p)));

    CREDO_GRADUS("res vacua recusatur; spatia extrema demuntur");
    CREDO_FALSUM(pensa_ponere(a, d, chorda_ex_literis("   ", p),
        VERUM));
    CREDO_VERUM(pensa_factum(a, e, chorda_ex_literis("  mass ", p)));
    volumen_claudere(vol);
}

interior vacuum
_sectio_liber (
    CredoContextus* c)
{
                Piscina* p;
    InternamentumChorda* intern;
                Volumen* vol;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;

    p       = c->piscina;
    intern  = internamentum_creare(p);
    vol     = volumen_temporarium(p, "probatio_pensa");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_AEQUALIS_I32(pensa_textum_legere(vol, intern, p).mensura,
        ZEPHYRUM);
    l = scriba_liber_aperire_in_spatio(p, intern, vol, "pensa", XL, X);
    CREDO_NECESSE_NON_NIHIL(l);
    d = scriba_liber_pagina(l, chorda_ex_literis("1", p));
    CREDO_NECESSE_NON_NIHIL(d);
    tabula_ex_literis_cum_dimensionibus(&folium, p, d->latitudo,
        d->altitudo, "vitamins\n\nweekends\n  mow the lawn");
    CREDO_VERUM(scriba_documentum_committere(d, &folium) > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(formam_describere(pensa_formam_legere(
        pensa_textum_legere(vol, intern, p), p), p),
        "*:vitamins;weekends:mow the lawn");
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dies", _sectio_dies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa", _sectio_signa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liber", _sectio_liber, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pensa", SECTIONES);
}
