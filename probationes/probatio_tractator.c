/* probatio_tractator.c - Probationes Registri Tractatoris */
#include "latina.h"
#include "tractator.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Tractatores pro probatione
 * ================================================== */

interior b32
tractator_a (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    redde VERUM;
}

interior b32
tractator_b (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    redde VERUM;
}

interior b32
tractator_fallback (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    redde VERUM;
}


    /* ==================================================
     * Probare registrum_tractatoris_creare
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);
        CREDO_NON_NIHIL(reg->genera);
        CREDO_NON_NIHIL(reg->fallbacks);
        CREDO_VERUM(reg->piscina == piscina);
        CREDO_VERUM(reg->intern == intern);
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg),
            ZEPHYRUM);

        /* Argumenta invalida */
        reg = registrum_tractatoris_creare(NIHIL, intern);
        CREDO_NIHIL(reg);

        reg = registrum_tractatoris_creare(piscina, NIHIL);
        CREDO_NIHIL(reg);
    }
}


    /* ==================================================
     * Probare registrum_tractatoris_registrare
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "open", tractator_a));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), I);

        /* Registrare secundum tractator pro eodem genere entitatis */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "close", tractator_b));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), II);

        /* Registrare tractator pro alio genere entitatis */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Widget",
            "tick", tractator_a));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), III);

        /* Argumenta invalida */
        CREDO_FALSUM(registrum_tractatoris_registrare(NIHIL, "Page",
            "test", tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, NIHIL,
            "test", tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, "Page",
            NIHIL, tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, "Page",
            "test", NIHIL));
    }
}


    /* ==================================================
     * Probare registrum_tractatoris_habet
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Sine registratione */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Page", "open"));

        /* Post registrationem */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "open", tractator_a));
        CREDO_VERUM(registrum_tractatoris_habet(reg, "Page", "open"));

        /* Genus entitatis non existens */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Widget",
            "open"));

        /* Genus nuntii non existens */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Page", "close"));
    }
}


    /* ==================================================
     * Probare registrum_tractatoris_invenire_literis
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;
            FunctioTractandi  functio;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "open", tractator_a));
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "close", tractator_b));

        /* Invenire tractator_a */
        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "open");
        CREDO_VERUM(functio == tractator_a);

        /* Invenire tractator_b */
        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "close");
        CREDO_VERUM(functio == tractator_b);

        /* Non inventum */
        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "unknown");
        CREDO_NIHIL((vacuum*)functio);

        functio = registrum_tractatoris_invenire_literis(reg, "Widget",
            "open");
        CREDO_NIHIL((vacuum*)functio);
    }
}


    /* ==================================================
     * Probare registrum_tractatoris_invenire (cum chorda*)
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;
            FunctioTractandi  functio;
                      chorda  genus_ent_chorda;
                      chorda  genus_nun_chorda;
                      chorda* genus_ent;
                      chorda* genus_nun;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "render", tractator_a));

        /* Internare chordas */
        genus_ent_chorda  = chorda_ex_literis("Page", piscina);
        genus_ent         = chorda_internare(intern, genus_ent_chorda);
        CREDO_NON_NIHIL(genus_ent);

        genus_nun_chorda  = chorda_ex_literis("render", piscina);
        genus_nun         = chorda_internare(intern, genus_nun_chorda);
        CREDO_NON_NIHIL(genus_nun);

        functio = registrum_tractatoris_invenire(reg, genus_ent,
            genus_nun);
        CREDO_VERUM(functio == tractator_a);
    }
}


    /* ==================================================
     * Probare fallback tractator
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;
            FunctioTractandi  functio;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator specificum */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "open", tractator_a));

        /* Ponere fallback */
        CREDO_VERUM(registrum_tractatoris_ponere_fallback(reg, "Page",
            tractator_fallback));

        /* Quaerere tractator specificum - debet invenire tractator_a */
        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "open");
        CREDO_VERUM(functio == tractator_a);

        /* Quaerere nuntium ignotum - debet invenire fallback */
        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "unknown_message");
        CREDO_VERUM(functio == tractator_fallback);

        /* Quaerere genus entitatis sine fallback - debet reddere NIHIL */
        functio = registrum_tractatoris_invenire_literis(reg, "Widget",
            "test");
        CREDO_NIHIL((vacuum*)functio);
    }
}


    /* ==================================================
     * Probare multos tractatores
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;
                         i32  i;
                   character  genus_buffer[XXXII];
                   character  nuntius_buffer[XXXII];

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare multos tractatores */
        per (i = ZEPHYRUM; i < L; i++)
        {
            sprintf(genus_buffer, "Genus_%d", (integer)i);
            sprintf(nuntius_buffer, "nuntius_%d", (integer)i);

            CREDO_VERUM(registrum_tractatoris_registrare(
                reg, genus_buffer, nuntius_buffer, tractator_a));
        }

        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), L);

        /* Verificare omnes */
        per (i = ZEPHYRUM; i < L; i++)
        {
            sprintf(genus_buffer, "Genus_%d", (integer)i);
            sprintf(nuntius_buffer, "nuntius_%d", (integer)i);

            CREDO_VERUM(registrum_tractatoris_habet(reg, genus_buffer,
                nuntius_buffer));
        }
    }
}


    /* ==================================================
     * Probare overwrite tractator
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        redde;
    }
    {
        RegistrumTractatoris* reg;
            FunctioTractandi  functio;

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator_a */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "update", tractator_a));

        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "update");
        CREDO_VERUM(functio == tractator_a);

        /* Overwrite cum tractator_b */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page",
            "update", tractator_b));

        functio = registrum_tractatoris_invenire_literis(reg, "Page",
            "update");
        CREDO_VERUM(functio == tractator_b);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "registrum_tractatoris_creare", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "registrum_tractatoris_registrare", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "registrum_tractatoris_habet", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "registrum_tractatoris_invenire_literis", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "registrum_tractatoris_invenire", _sectio_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fallback tractator", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multos tractatores", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "overwrite tractator", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tractator", SECTIONES);
}
