/* probatio_md_census.c - Porta census corporis (C2)
 *
 * md_census_addere super fixturam unam cum omnibus generibus numeratis
 * (capitula h1/h2/h3 ATX et setext, listae punctata laxa + numerata,
 * officia apertum/perfectum, saeptum saeptum c + indentatum, nexus
 * inlineus/referens/auto + definitio, verbatim, html) et fixturam
 * alteram (cumulatio trans plagulas, lingua bis); md_census_citata
 * verbatim cum linea et contento (valor derivatus aut cruda). Culpa
 * plantata nativitatis: gradus capituli sine limite superiore.
 */

#include "latina.h"
#include "md_census.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

interior constans MdCensusLingua*
_lingua (
     constans MdCensus* c,
    constans character* titulus)
{
    i32 n = xar_numerus(c->linguae);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MdCensusLingua* e =
            (constans MdCensusLingua*)xar_obtinere(
            c->linguae, i);

        si (strcmp(e->lingua, titulus) == ZEPHYRUM)
        {
            redde e;
        }
    }
    redde NIHIL;
}

hic_manens constans character FONS_A[] =
    "# Uno\n\n## Duo\n\nPara `code` [a](/u) [b][x] <http://q>.\n\n"
    "[x]: /v\n\n- a\n\n- b\n\n1. n\n2. m\n\n- [ ] apertum\n"
    "- [x] perfectum\n\n```c\nx;\n```\n\n    indentatum\n\n"
    "Tres\n---\n\n<div>\n</div>\n";
hic_manens constans character FONS_B[] =
    "```c\ny;\n```\n\n```sh\nls\n```\n";

interior vacuum
_numeros_fixturae_probare (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
    MdCensus  census;

    md_census_nudum(&census, piscina);
    CREDO_VERUM (md_census_addere(&census, piscina, FONS_A,
        (i32)strlen(FONS_A)));
    CREDO_AEQUALIS_I32 (census.plagulae, I);
    CREDO_AEQUALIS_I32 (census.fractae, ZEPHYRUM);
    CREDO_VERUM (census.octeti == (s64)strlen(FONS_A));
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_CAPITULUM], III);
    CREDO_AEQUALIS_I32 (census.capitula_gradus[I], I);
    CREDO_AEQUALIS_I32 (census.capitula_gradus[II], II);   /* ATX h2 + setext '---' */
    CREDO_AEQUALIS_I32 (census.capitula_gradus[III], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_LISTA], III);
    CREDO_AEQUALIS_I32 (census.listae_punctatae, II);
    CREDO_AEQUALIS_I32 (census.listae_numeratae, I);
    CREDO_AEQUALIS_I32 (census.listae_laxae, I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_ELEMENTUM], VI);
    CREDO_AEQUALIS_I32 (census.officia_aperta, I);
    CREDO_AEQUALIS_I32 (census.officia_perfecta, I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_SAEPTUM], II);
    CREDO_AEQUALIS_I32 (census.saepta_saepta, I);
    CREDO_AEQUALIS_I32 (census.saepta_indentata, I);
    CREDO_VERUM (_lingua(&census, "c") != NIHIL
        && _lingua(&census, "c")->numerus == I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_NEXUS], III);
    CREDO_AEQUALIS_I32 (census.nexus_inlinei, I);
    CREDO_AEQUALIS_I32 (census.nexus_referentes, I);
    CREDO_AEQUALIS_I32 (census.nexus_auto, I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_DEFINITIO_NEXUS], I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_VERBATIM], I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_HTML], I);
    CREDO_AEQUALIS_I32 (census.genera[MD_GENUS_DOCUMENTUM], I);
    CREDO_VERUM (census.lexemata > census.derivata
        && census.derivata > ZEPHYRUM);
    CREDO_VERUM (census.lineae >= XXVI);
}

/* cumulatio fixuram primam EX PROPOSITO continuat: sectio eam sine
 * assertione refacit (sectio prior eam asserit), deinde alteram
 * addit */
interior vacuum
_cumulationem_probare (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
    MdCensus  census;

    md_census_nudum(&census, piscina);
    si (!md_census_addere(&census, piscina, FONS_A,
        (i32)strlen(FONS_A)))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    CREDO_VERUM (md_census_addere(&census, piscina, FONS_B,
        (i32)strlen(FONS_B)));
    CREDO_AEQUALIS_I32 (census.plagulae, II);
    CREDO_AEQUALIS_I32 (census.saepta_saepta, III);
    CREDO_VERUM (_lingua(&census, "c")->numerus == II);
    CREDO_VERUM (_lingua(&census, "sh") != NIHIL
        && _lingua(&census, "sh")->numerus == I);
    CREDO_AEQUALIS_I32 (xar_numerus(census.linguae), II);
}

interior vacuum
_citata_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
        Xar* citata;

    citata = md_census_citata(piscina, "a `x_y(` b\n\n`` a`b ``\n",
        (i32)strlen("a `x_y(` b\n\n`` a`b ``\n"));
    CREDO_VERUM (citata != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(citata), II);
    {
        constans MdCitatum* x =
            (constans MdCitatum*)xar_obtinere(citata,
            ZEPHYRUM);
        constans MdCitatum* y =
            (constans MdCitatum*)xar_obtinere(citata, I);

        CREDO_CHORDA_AEQUALIS_LITERIS (x->contentum, "x_y(");
        CREDO_AEQUALIS_I32 (x->linea, I);
        CREDO_CHORDA_AEQUALIS_LITERIS (y->contentum, "a`b");
        CREDO_AEQUALIS_I32 (y->linea, III);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "numeros fixturae", _numeros_fixturae_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "cumulationem", _cumulationem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "citata", _citata_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("md_census", SECTIONES);
}
