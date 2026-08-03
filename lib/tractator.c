#include "tractator.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Pons functionis
 *
 * ISO C monstratorem functionis in vacuum* convertere vetat (gcc
 * -pedantic reicit, clang tacet - percursus glibc 2026-08-03).
 * Cellula pontis in piscina regestri obiectum verum tabulae praebet;
 * functio intra cellulam vivit, conversio nulla.
 * ================================================== */

nomen structura {
    FunctioTractandi functio;
} PonsTractatoris;

interior vacuum*
_pontem_struere(Piscina* piscina, FunctioTractandi functio)
{
    PonsTractatoris* pons;

    pons = (PonsTractatoris*)piscina_allocare(piscina,
        magnitudo(PonsTractatoris));
    si (!pons)
    {
        redde NIHIL;
    }
    pons->functio = functio;
    redde (vacuum*)pons;
}


/* ==================================================
 * Creatio
 * ================================================== */

RegistrumTractatoris*
registrum_tractatoris_creare(
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    RegistrumTractatoris* reg;

    si (!piscina || !intern)
    {
        redde NIHIL;
    }

    reg = (RegistrumTractatoris*)piscina_allocare(piscina, magnitudo(RegistrumTractatoris));
    si (!reg)
    {
        redde NIHIL;
    }

    reg->piscina = piscina;
    reg->intern  = intern;

    /* Creare tabulas */
    reg->genera = tabula_dispersa_creare_chorda(piscina, XXXII);
    si (!reg->genera)
    {
        redde NIHIL;
    }

    reg->fallbacks = tabula_dispersa_creare_chorda(piscina, XVI);
    si (!reg->fallbacks)
    {
        redde NIHIL;
    }

    redde reg;
}


/* ==================================================
 * Registratio
 * ================================================== */

b32
registrum_tractatoris_registrare(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii,
    FunctioTractandi      functio)
{
    chorda           genus_ent_chorda;
    chorda           genus_nun_chorda;
    chorda*          genus_ent_intern;
    chorda*          genus_nun_intern;
    TabulaDispersa*  tractatori_entitatis;
    vacuum*          valor;

    si (!reg || !genus_entitatis || !genus_nuntii || !functio)
    {
        redde FALSUM;
    }

    /* Internare chordas */
    genus_ent_chorda = chorda_ex_literis(genus_entitatis, reg->piscina);
    genus_ent_intern = chorda_internare(reg->intern, genus_ent_chorda);

    genus_nun_chorda = chorda_ex_literis(genus_nuntii, reg->piscina);
    genus_nun_intern = chorda_internare(reg->intern, genus_nun_chorda);

    si (!genus_ent_intern || !genus_nun_intern)
    {
        redde FALSUM;
    }

    /* Obtinere vel creare tabulam pro hoc genere entitatis */
    si (tabula_dispersa_invenire(reg->genera, *genus_ent_intern, &valor))
    {
        tractatori_entitatis = (TabulaDispersa*)valor;
    }
    alioquin
    {
        /* Creare novam tabulam pro hoc genere entitatis */
        tractatori_entitatis = tabula_dispersa_creare_chorda(reg->piscina, XVI);
        si (!tractatori_entitatis)
        {
            redde FALSUM;
        }

        si (!tabula_dispersa_inserere(reg->genera, *genus_ent_intern, tractatori_entitatis))
        {
            redde FALSUM;
        }
    }

    /* Inserere tractator in tabulam entitatis (per pontem) */
    {
        vacuum* pons = _pontem_struere(reg->piscina, functio);

        si (!pons)
        {
            redde FALSUM;
        }
        redde tabula_dispersa_inserere(tractatori_entitatis,
            *genus_nun_intern, pons);
    }
}

b32
registrum_tractatoris_ponere_fallback(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    FunctioTractandi      functio)
{
    chorda  genus_chorda;
    chorda* genus_intern;

    si (!reg || !genus_entitatis || !functio)
    {
        redde FALSUM;
    }

    genus_chorda = chorda_ex_literis(genus_entitatis, reg->piscina);
    genus_intern = chorda_internare(reg->intern, genus_chorda);

    si (!genus_intern)
    {
        redde FALSUM;
    }

    {
        vacuum* pons = _pontem_struere(reg->piscina, functio);

        si (!pons)
        {
            redde FALSUM;
        }
        redde tabula_dispersa_inserere(reg->fallbacks, *genus_intern,
            pons);
    }
}


/* ==================================================
 * Quaestio
 * ================================================== */

FunctioTractandi
registrum_tractatoris_invenire(
    RegistrumTractatoris* reg,
    chorda*               genus_entitatis,
    chorda*               genus_nuntii)
{
    TabulaDispersa* tractatori_entitatis;
    vacuum*         valor;

    si (!reg || !genus_entitatis || !genus_nuntii)
    {
        redde NIHIL;
    }

    /* Quaerere tabulam pro genere entitatis */
    si (!tabula_dispersa_invenire(reg->genera, *genus_entitatis, &valor))
    {
        /* Nullus tractator pro hoc genere - quaerere fallback */
        salta quaerere_fallback;
    }

    tractatori_entitatis = (TabulaDispersa*)valor;

    /* Quaerere tractator pro genere nuntii */
    si (tabula_dispersa_invenire(tractatori_entitatis, *genus_nuntii, &valor))
    {
        redde ((PonsTractatoris*)valor)->functio;
    }

    /* Nullus tractator specificus - quaerere fallback */
quaerere_fallback:
    si (tabula_dispersa_invenire(reg->fallbacks, *genus_entitatis, &valor))
    {
        redde ((PonsTractatoris*)valor)->functio;
    }

    redde NIHIL;
}

FunctioTractandi
registrum_tractatoris_invenire_literis(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii)
{
    chorda  genus_ent_chorda;
    chorda  genus_nun_chorda;
    chorda* genus_ent_intern;
    chorda* genus_nun_intern;

    si (!reg || !genus_entitatis || !genus_nuntii)
    {
        redde NIHIL;
    }

    /* Internare chordas */
    genus_ent_chorda = chorda_ex_literis(genus_entitatis, reg->piscina);
    genus_ent_intern = chorda_internare(reg->intern, genus_ent_chorda);

    genus_nun_chorda = chorda_ex_literis(genus_nuntii, reg->piscina);
    genus_nun_intern = chorda_internare(reg->intern, genus_nun_chorda);

    si (!genus_ent_intern || !genus_nun_intern)
    {
        redde NIHIL;
    }

    redde registrum_tractatoris_invenire(reg, genus_ent_intern, genus_nun_intern);
}

b32
registrum_tractatoris_habet(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii)
{
    redde registrum_tractatoris_invenire_literis(reg, genus_entitatis, genus_nuntii) != NIHIL;
}


/* ==================================================
 * Utilitas
 * ================================================== */

i32
registrum_tractatoris_numerus(
    RegistrumTractatoris* reg)
{
    TabulaIterator  iter_genera;
    chorda          clavis_genus;
    vacuum*         valor_tabula;
    i32             summa;

    si (!reg)
    {
        redde ZEPHYRUM;
    }

    summa = ZEPHYRUM;

    /* Iterare per omnes genera entitatum */
    iter_genera = tabula_dispersa_iterator_initium(reg->genera);
    dum (tabula_dispersa_iterator_proximum(&iter_genera, &clavis_genus, &valor_tabula))
    {
        TabulaDispersa* tractatori;
        tractatori = (TabulaDispersa*)valor_tabula;
        summa += tabula_dispersa_numerus(tractatori);
    }

    redde summa;
}

vacuum
registrum_tractatoris_imprimere(
    RegistrumTractatoris* reg)
{
    TabulaIterator  iter_genera;
    TabulaIterator  iter_nuntii;
    chorda          clavis_genus;
    chorda          clavis_nuntius;
    vacuum*         valor_tabula;
    vacuum*         valor_functio;

    si (!reg)
    {
        imprimere("RegistrumTractatoris: NIHIL\n");
        redde;
    }

    imprimere("RegistrumTractatoris (%d tractatores):\n",
              registrum_tractatoris_numerus(reg));

    /* Iterare per genera */
    iter_genera = tabula_dispersa_iterator_initium(reg->genera);
    dum (tabula_dispersa_iterator_proximum(&iter_genera, &clavis_genus, &valor_tabula))
    {
        TabulaDispersa* tractatori;

        imprimere("  Genus: %.*s\n", clavis_genus.mensura, clavis_genus.datum);

        tractatori = (TabulaDispersa*)valor_tabula;

        /* Iterare per nuntios */
        iter_nuntii = tabula_dispersa_iterator_initium(tractatori);
        dum (tabula_dispersa_iterator_proximum(&iter_nuntii, &clavis_nuntius, &valor_functio))
        {
            imprimere("    -> %.*s: %p\n",
                      clavis_nuntius.mensura, clavis_nuntius.datum,
                      valor_functio);
        }

        /* Verificare fallback */
        si (tabula_dispersa_invenire(reg->fallbacks, clavis_genus, &valor_functio))
        {
            imprimere("    -> (fallback): %p\n", valor_functio);
        }
    }
}
