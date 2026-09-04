/* probatio_md_extenta.c - Porta extentorum structuralium (C1)
 *
 * md_extenta_quaerere super fixturas breves: extenta capitulorum (ATX
 * et setext), elementorum (etiam nidificatorum: elementum exterius
 * interius continet), saepti, paragraphi, documenti totius, lexematum
 * ipsorum (md-textus), selectoris cum combinatoribus et pseudo-classe;
 * lexemata derivata (definitio nexus) octetos non movent; selector sine
 * congruentia = successus cum nihilo; selector malus = causa nominata.
 * Octeti extenti contra fontem COMPARANTUR (non numeri manu computati).
 * Culpa plantata nativitatis: finis - I in md_extenta.c.
 */

#include "latina.h"
#include "md_extenta.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

interior MdExtentaResultus
_quaerere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons,
     constans character* selector)
{
    redde md_extenta_quaerere(piscina, intern, fons, (i32)strlen(fons),
        selector);
}

interior constans MdExtentum*
_extentum (
    constans MdExtentaResultus* r,
                           i32  i)
{
    redde (constans MdExtentum*)xar_obtinere(r->extenta, i);
}

/* octeti [initium, finis) fontis == literae */
interior b32
_octeti (
     constans character* fons,
    constans MdExtentum* x,
     constans character* literae)
{
    i32 l = (i32)strlen(literae);

    si (x->initium < ZEPHYRUM || x->finis < x->initium)
    {
        redde FALSUM;
    }
    si ((i32)(x->finis - x->initium) != l)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(fons + x->initium, literae, (size_t)l)
        == ZEPHYRUM);
}

integer
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
      MdExtentaResultus  r;
     constans character* fons_a =
         "# Title one\n\ntext *em* here\n\n## Sub\n\n- a\n- b\n\n"
         "```c\nx;\n```\n";
    constans character* fons_b = "Title\n=====\n\n- a\n  - b\n- c\n";
    constans character* fons_c = "[x]: /u \"t\"\n\n[x]\n";

    piscina = piscina_generare_dynamicum("probatio_md_extenta",
        8388608);
    intern = internamentum_creare(piscina);


    credo_aperire(piscina);

    imprimere("\n--- Probans capitula ATX ---\n");
    r = _quaerere(piscina, intern, fons_a, "capitulum");
    CREDO_VERUM (r.successus);
    CREDO_VERUM (r.lexemata > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), II);
    CREDO_AEQUALIS_I32 (r.congruentiae, II);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM),
        "# Title one\n"));
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->linea, I);
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->columna, I);
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->linea_finis, I);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(
        _extentum(&r, ZEPHYRUM)->tag, piscina), "capitulum");
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, I), "## Sub\n"));
    CREDO_AEQUALIS_I32 (_extentum(&r, I)->linea, V);

    imprimere("\n--- Probans elementa, saeptum, paragraphum, documentum ---\n");
    r = _quaerere(piscina, intern, fons_a, "elementum");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), II);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM), "- a\n"));
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, I), "- b\n"));
    r = _quaerere(piscina, intern, fons_a, "saeptum");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM),
        "```c\nx;\n```\n"));
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->linea, X);
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->linea_finis, XII);
    r = _quaerere(piscina, intern, fons_a, "paragraphus");
    CREDO_VERUM (r.successus);
    /* elementa listae strictae paragraphos tenent (III); paragraphus
     * intra elementum marcam lineae suae POSSIDET (lex lineae: praefixa
     * folii) - extentum eius a marca incipit */
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), III);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM),
        "text *em* here\n"));
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, I), "- a\n"));
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, II), "- b\n"));
    r = _quaerere(piscina, intern, fons_a, "documentum");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM), fons_a));

    imprimere("\n--- Probans lexemata ipsa et combinatores ---\n");
    r = _quaerere(piscina, intern, fons_a, "md-textus");
    CREDO_VERUM (r.successus);
    CREDO_VERUM (xar_numerus(r.extenta) >= V);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM), "Title one"));
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->columna, III);
    r = _quaerere(piscina, intern, fons_a,
        "lista > elementa > elementum:first-child");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, ZEPHYRUM), "- a\n"));
    r = _quaerere(piscina, intern, fons_a, "capitulum md-textus");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), II);
    CREDO_VERUM (_octeti(fons_a, _extentum(&r, I), "Sub"));

    imprimere("\n--- Probans setext et nidificationem ---\n");
    r = _quaerere(piscina, intern, fons_b, "capitulum");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_b, _extentum(&r, ZEPHYRUM),
        "Title\n=====\n"));
    CREDO_AEQUALIS_I32 (_extentum(&r, ZEPHYRUM)->linea_finis, II);
    r = _quaerere(piscina, intern, fons_b, "elementum");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), III);
    CREDO_VERUM (_octeti(fons_b, _extentum(&r, ZEPHYRUM),
        "- a\n  - b\n"));
    CREDO_VERUM (_octeti(fons_b, _extentum(&r, I), "  - b\n"));
    CREDO_VERUM (_octeti(fons_b, _extentum(&r, II), "- c\n"));

    imprimere("\n--- Probans lexemata derivata et refusiones ---\n");
    r = _quaerere(piscina, intern, fons_c, "definitio-nexus");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_c, _extentum(&r, ZEPHYRUM),
        "[x]: /u \"t\"\n"));
    r = _quaerere(piscina, intern, fons_c, "nexus");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), I);
    CREDO_VERUM (_octeti(fons_c, _extentum(&r, ZEPHYRUM), "[x]"));
    r = _quaerere(piscina, intern, fons_a, "tabula");
    CREDO_VERUM (r.successus);
    CREDO_AEQUALIS_I32 (xar_numerus(r.extenta), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (r.congruentiae, ZEPHYRUM);
    r = _quaerere(piscina, intern, fons_a, "[");
    CREDO_FALSUM (r.successus);
    CREDO_VERUM (r.causa.mensura > ZEPHYRUM);
    CREDO_VERUM (r.extenta == NIHIL);

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
