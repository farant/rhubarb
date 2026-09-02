/* materia_lexicon.c - Porta et accessus descriptoris lexicalis
 *
 * Vide materia_lexicon.h pro ratione et mensuris.
 *
 * PORTA IN NATIVITATE (doctrina instrumentorum: porta tacita =
 * porta mortua). Lexicon nullum adhiberi potest nisi prius
 * materia_lexicon_ratum_facere transierit; invariantia quae
 * antea probatio per genera XCV currens custodiebat hic ad
 * tempus oneris iudicantur, SEMEL.
 */

#include "materia_lexicon.h"

#include <string.h>


/* ==================================================
 * Nomina vitiorum
 * ================================================== */

hic_manens constans character* NOMINA_VITIORUM[] = {
    "SANUM",
    "lexicon aut genera NIHIL",
    "numerus generum ZEPHYRUM",
    "praefixum tagi NIHIL",
    "genus sine titulo",
    "species extra fines",
    "munus extra fines",
    "invarians fracta: VERBATIM <=> orthographia NIHIL",
    "REPETITUM cuius unitas non character unus",
    "TERMINATOR non in nova linea desinens",
    "genus spatii pravum"
};

constans character*
materia_lexicon_vitium_nomen (
    MateriaLexVitium vitium)
{
    i32 numerus = (i32)(magnitudo(NOMINA_VITIORUM)
                      / magnitudo(NOMINA_VITIORUM[0]));

    si ((i32)vitium >= numerus)
    {
        redde "vitium ignotum";
    }
    redde NOMINA_VITIORUM[(i32)vitium];
}


/* ==================================================
 * Porta
 * ================================================== */

interior vacuum
_iudicium_ponere (
    MateriaLexIudicium* iudicium,
      MateriaLexVitium  vitium,
                   i32  genus)
{
    si (iudicium != NIHIL)
    {
        iudicium->vitium  = (s32)vitium;
        iudicium->genus   = genus;
    }
}

b32
materia_lexicon_ratum_facere (
              MateriaLexiconRatum* ratum,
    constans MateriaLexiconCoctum* lexicon,
               MateriaLexIudicium* iudicium)
{
    i32 i;
    i32 larva;

    si (ratum == NIHIL)
    {
        redde FALSUM;
    }

    ratum->lexicon            = NIHIL;
    ratum->munera_praesentia  = ZEPHYRUM;
    ratum->ratum              = FALSUM;

    si (lexicon == NIHIL || lexicon->genera == NIHIL)
    {
        _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_NIHIL, ZEPHYRUM);
        redde FALSUM;
    }
    si (lexicon->numerus_generum == ZEPHYRUM)
    {
        _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_NUMERUS,
            ZEPHYRUM);
        redde FALSUM;
    }
    si (lexicon->praefixum_tagi == NIHIL)
    {
        _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_PRAEFIXUM,
            lexicon->numerus_generum);
        redde FALSUM;
    }

    larva = ZEPHYRUM;

    per (i = ZEPHYRUM; i < lexicon->numerus_generum; i++)
    {
        constans MateriaLexGenus* g = &lexicon->genera[i];
                             b32  verbatim;

        si (g->titulus == NIHIL || g->titulus[0] == '\0')
        {
            _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_TITULUS, i);
            redde FALSUM;
        }
        si (   g->species < ZEPHYRUM
            || g->species >= (s32)MATERIA_LEX_NUMERUS_SPECIERUM)
        {
            _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_SPECIES, i);
            redde FALSUM;
        }
        si (   g->munus < ZEPHYRUM
            || g->munus >= (s32)MATERIA_MUNUS_NUMERUS_MUNERUM)
        {
            _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_MUNUS, i);
            redde FALSUM;
        }

        /* INVARIANS CENTRALIS. Antea haec conditio per casus
         * speciales (EOF, orthographia NIHIL) implicite tenebatur
         * et per tabulam TERTIAM manu servatam repetebatur.
         * Nunc dicta est, ergo custodiri potest. */
        verbatim = (b32)(g->species == (s32)MATERIA_LEX_VERBATIM);
        si (verbatim != (b32)(g->orthographia == NIHIL))
        {
            _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_INVARIANS, i);
            redde FALSUM;
        }

        si (g->species == (s32)MATERIA_LEX_REPETITUM)
        {
            si (strlen(g->orthographia) != (size_t)I)
            {
                _iudicium_ponere(iudicium,
                    MATERIA_LEX_VITIUM_REPETITUM, i);
                redde FALSUM;
            }
        }
        si (g->species == (s32)MATERIA_LEX_TERMINATOR)
        {
            size_t mensura = strlen(g->orthographia);

            si (   mensura == (size_t)ZEPHYRUM
                || g->orthographia[mensura - (size_t)I] != '\n')
            {
                _iudicium_ponere(iudicium,
                    MATERIA_LEX_VITIUM_TERMINATOR, i);
                redde FALSUM;
            }
        }

        larva |= MATERIA_MUNUS_VEXILLUM(g->munus);
    }

    /* genus_spatii: -I licet (lingua spatium canonicum non habet);
     * aliter intra fines esse ET munus SPATIUM ferre debet. */
    si (lexicon->genus_spatii != (s32)-I)
    {
        si (   lexicon->genus_spatii < ZEPHYRUM
            || (i32)lexicon->genus_spatii >= lexicon->numerus_generum
            || lexicon->genera[(i32)lexicon->genus_spatii].munus
               != (s32)MATERIA_MUNUS_SPATIUM)
        {
            _iudicium_ponere(iudicium, MATERIA_LEX_VITIUM_SPATIUM,
                lexicon->numerus_generum);
            redde FALSUM;
        }
    }

    ratum->lexicon            = lexicon;
    ratum->munera_praesentia  = larva;
    ratum->ratum              = VERUM;
    _iudicium_ponere(iudicium, MATERIA_LEX_SANUM,
        lexicon->numerus_generum);
    redde VERUM;
}


/* ==================================================
 * Capacitas
 * ================================================== */

b32
materia_lexicon_munus_habet (
    constans MateriaLexiconRatum* ratum,
                 MateriaLexMunus  munus)
{
    si (ratum == NIHIL || !ratum->ratum)
    {
        redde FALSUM;
    }
    si (   (s32)munus < ZEPHYRUM
        || (s32)munus >= (s32)MATERIA_MUNUS_NUMERUS_MUNERUM)
    {
        redde FALSUM;
    }
    redde (b32)((ratum->munera_praesentia
                 & MATERIA_MUNUS_VEXILLUM(munus)) != ZEPHYRUM);
}

b32
materia_lexicon_munera_habet (
    constans MateriaLexiconRatum* ratum,
                             i32  munera_postulata)
{
    si (ratum == NIHIL || !ratum->ratum)
    {
        redde FALSUM;
    }
    redde (b32)((ratum->munera_praesentia & munera_postulata)
                == munera_postulata);
}


/* ==================================================
 * Accessus
 * ================================================== */

interior constans MateriaLexGenus*
_genus_capere (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    si (ratum == NIHIL || !ratum->ratum || ratum->lexicon == NIHIL)
    {
        redde NIHIL;
    }
    /* Genus SIGNATUM est quia lexemata id ita ferunt (MateriaToken.
     * genus). Custodia negativi ergo REALIS est: sine ea -I in
     * i32 versum ad 4294967295 fit et casu solo fines excedit. */
    si (   genus < ZEPHYRUM
        || (i32)genus >= ratum->lexicon->numerus_generum)
    {
        redde NIHIL;
    }
    redde &ratum->lexicon->genera[(i32)genus];
}

constans character*
materia_lexicon_titulus (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    redde (g == NIHIL) ? NIHIL : g->titulus;
}

constans character*
materia_lexicon_orthographia (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    redde (g == NIHIL) ? NIHIL : g->orthographia;
}

MateriaLexSpecies
materia_lexicon_species (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    /* Genus ignotum VERBATIM non est - species enumerationem
     * excedens redditur ut vocans discernere possit. */
    redde (g == NIHIL)
        ? MATERIA_LEX_NUMERUS_SPECIERUM
        : (MateriaLexSpecies)g->species;
}

MateriaLexMunus
materia_lexicon_munus (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    redde (g == NIHIL)
        ? MATERIA_MUNUS_NUMERUS_MUNERUM
        : (MateriaLexMunus)g->munus;
}

b32
materia_lexicon_textum_fert (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    si (g == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)(g->species == (s32)MATERIA_LEX_VERBATIM);
}

b32
materia_lexicon_trivium_est (
    constans MateriaLexiconRatum* ratum,
                             s32  genus)
{
    constans MateriaLexGenus* g = _genus_capere(ratum, genus);

    si (g == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)(   g->munus == (s32)MATERIA_MUNUS_SPATIUM
                || g->munus == (s32)MATERIA_MUNUS_LINEA
                || g->munus == (s32)MATERIA_MUNUS_LAMINA
                || g->munus == (s32)MATERIA_MUNUS_COMMENTUM);
}
