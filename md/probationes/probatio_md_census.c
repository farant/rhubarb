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

integer
principale (vacuum)
{
               Piscina* piscina;
              MdCensus  c;
                   Xar* citata;
    constans character* fons_a =
        "# Uno\n\n## Duo\n\nPara `code` [a](/u) [b][x] <http://q>.\n\n"
        "[x]: /v\n\n- a\n\n- b\n\n1. n\n2. m\n\n- [ ] apertum\n"
        "- [x] perfectum\n\n```c\nx;\n```\n\n    indentatum\n\n"
        "Tres\n---\n\n<div>\n</div>\n";
    constans character* fons_b = "```c\ny;\n```\n\n```sh\nls\n```\n";

    piscina = piscina_generare_dynamicum("probatio_md_census", 8388608);
    credo_aperire(piscina);
    md_census_nudum(&c, piscina);

    imprimere("\n--- Probans numeros fixturae ---\n");
    CREDO_VERUM (md_census_addere(&c, piscina, fons_a,
        (i32)strlen(fons_a)));
    CREDO_AEQUALIS_I32 (c.plagulae, I);
    CREDO_AEQUALIS_I32 (c.fractae, ZEPHYRUM);
    CREDO_VERUM (c.octeti == (s64)strlen(fons_a));
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_CAPITULUM], III);
    CREDO_AEQUALIS_I32 (c.capitula_gradus[I], I);
    CREDO_AEQUALIS_I32 (c.capitula_gradus[II], II);   /* ATX h2 + setext '---' */
    CREDO_AEQUALIS_I32 (c.capitula_gradus[III], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_LISTA], III);
    CREDO_AEQUALIS_I32 (c.listae_punctatae, II);
    CREDO_AEQUALIS_I32 (c.listae_numeratae, I);
    CREDO_AEQUALIS_I32 (c.listae_laxae, I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_ELEMENTUM], VI);
    CREDO_AEQUALIS_I32 (c.officia_aperta, I);
    CREDO_AEQUALIS_I32 (c.officia_perfecta, I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_SAEPTUM], II);
    CREDO_AEQUALIS_I32 (c.saepta_saepta, I);
    CREDO_AEQUALIS_I32 (c.saepta_indentata, I);
    CREDO_VERUM (_lingua(&c, "c") != NIHIL
        && _lingua(&c, "c")->numerus == I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_NEXUS], III);
    CREDO_AEQUALIS_I32 (c.nexus_inlinei, I);
    CREDO_AEQUALIS_I32 (c.nexus_referentes, I);
    CREDO_AEQUALIS_I32 (c.nexus_auto, I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_DEFINITIO_NEXUS], I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_VERBATIM], I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_HTML], I);
    CREDO_AEQUALIS_I32 (c.genera[MD_GENUS_DOCUMENTUM], I);
    CREDO_VERUM (c.lexemata > c.derivata && c.derivata > ZEPHYRUM);
    CREDO_VERUM (c.lineae >= XXVI);

    imprimere("\n--- Probans cumulationem ---\n");
    CREDO_VERUM (md_census_addere(&c, piscina, fons_b,
        (i32)strlen(fons_b)));
    CREDO_AEQUALIS_I32 (c.plagulae, II);
    CREDO_AEQUALIS_I32 (c.saepta_saepta, III);
    CREDO_VERUM (_lingua(&c, "c")->numerus == II);
    CREDO_VERUM (_lingua(&c, "sh") != NIHIL
        && _lingua(&c, "sh")->numerus == I);
    CREDO_AEQUALIS_I32 (xar_numerus(c.linguae), II);

    imprimere("\n--- Probans citata ---\n");
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

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        si (praeteritus)
        {
            redde ZEPHYRUM;
        }
    }
    redde I;
}
