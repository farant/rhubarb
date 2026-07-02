/* probatio_silva_grammatica.c - Grammatica sceleti (Phase 3 Chunk B)
 *
 * Legit silva/grammatica/sceletum.stml EX DISCO (grammatica in disco,
 * non littera chordae - decisio interview) et probat:
 *   - lectionem + validationem grammaticae verae annotatae
 *   - tabulas LALR conflictus SERVANTES
 *   - CELLAM FURCAE: post IDENTIFICATOR, in STAR, reduce/reduce inter
 *     typus-nomen (lectio declarationis - typedef) et
 *     factor-identificator (lectio expressionis) - ipsa ambiguitas
 *     "foo * bar;" quam Phase 4 in AMBIGUUS colliget.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_generare.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024

interior character*
_plagulam_legere (Piscina* piscina, constans character* via)
{
    FILE* pl;
    character* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura] = '\0';
    redde buffer;
}

interior b32
_chorda_est (chorda* c, constans character* literis)
{
    si (c == NIHIL)
    {
        redde FALSUM;
    }
    si (c->mensura != (i32)strlen(literis))
    {
        redde FALSUM;
    }
    redde (memcmp(c->datum, literis, (memoriae_index)c->mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior s32
_symbolum (SilvaGenGrammatica* g, constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(g->symbola); i++)
    {
        SilvaGenSymbolum* sym;

        sym = (SilvaGenSymbolum*)xar_obtinere(g->symbola, i);
        si (sym != NIHIL && _chorda_est(sym->titulus, titulus))
        {
            redde sym->index;
        }
    }
    redde -I;
}

/* Invenire productionem per id; reddit indicem vel -1 */
interior s32
_productio_per_id (SilvaGenGrammatica* g, constans character* id)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(g->productiones); i++)
    {
        SilvaGenProductio* prod;

        prod = (SilvaGenProductio*)xar_obtinere(g->productiones, i);
        si (prod != NIHIL && _chorda_est(prod->id, id))
        {
            redde prod->index;
        }
    }
    redde -I;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    InternamentumChorda* intern;
    constans character* radix;
    character via[VIA_MAXIMA];
    character* fons;
    SilvaGenGrammatica* g;
    SilvaGenCollectio* collectio;
    SilvaGenTabula* tabula;

    piscina = piscina_generare_dynamicum("probatio_silva_grammatica",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via, "%s/silva/grammatica/sceletum.stml", radix);


    /* ========================================================
     * PROBARE: lectio grammaticae sceleti ex disco
     * ======================================================== */

    imprimere("\n--- Probans grammaticam sceleti ---\n");
    imprimere("  via: %s\n", via);

    fons = _plagulam_legere(piscina, via);
    CREDO_NON_NIHIL (fons);
    si (fons == NIHIL)
    {
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }

    g = silva_gen_grammaticam_legere(piscina, intern, fons);
    CREDO_NON_NIHIL (g);
    si (g == NIHIL)
    {
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }

    /* 9 terminalia; 9 non-terminalia + __initium__ */
    CREDO_AEQUALIS_I32 (g->numerus_terminalium, IX);
    CREDO_AEQUALIS_I32 (g->numerus_non_terminalium, X);
    /* 17 productiones + augmentata = XVIII */
    CREDO_AEQUALIS_I32 (xar_numerus(g->productiones), XVIII);
    /* genera structuralia */
    CREDO_AEQUALIS_I32 (xar_numerus(g->genera_extra), III);

    /* ambae lectiones IDENTIFICATORIS adsunt */
    CREDO_VERUM (_productio_per_id(g, "typus-nomen") >= ZEPHYRUM);
    CREDO_VERUM (_productio_per_id(g, "factor-identificator") >= ZEPHYRUM);


    /* ========================================================
     * PROBARE: FIRST/FOLLOW + epsilon listae
     * ======================================================== */

    {
        SilvaGenSymbolum* sym;

        imprimere("\n--- Probans FIRST/epsilon ---\n");

        CREDO_VERUM (silva_gen_first_computare(g));
        CREDO_VERUM (silva_gen_follow_computare(g));

        /* sententiae habet epsilon (lista vacua) */
        sym = (SilvaGenSymbolum*)xar_obtinere(g->symbola,
            (i32)_symbolum(g, "sententiae"));
        CREDO_NON_NIHIL (sym);
        CREDO_VERUM (sym->habet_epsilon);
    }


    /* ========================================================
     * PROBARE: tabulae LALR - conflictus servati
     * ======================================================== */

    imprimere("\n--- Probans tabulas et conflictus ---\n");

    collectio = silva_gen_collectio_lalr_construere(g);
    CREDO_NON_NIHIL (collectio);
    imprimere("  status: %d\n", (int)xar_numerus(collectio->status_omnes));

    tabula = silva_gen_tabulam_construere(collectio);
    CREDO_NON_NIHIL (tabula);
    imprimere("  conflictus: %d\n", (int)tabula->numerus_conflictuum);

    /* ambiguitas deliberata: conflictus ADSUNT et SERVANTUR */
    CREDO_VERUM (tabula->numerus_conflictuum > ZEPHYRUM);


    /* ========================================================
     * PROBARE: CELLA FURCAE "foo * bar;" - reduce/reduce in STAR
     * inter typus-nomen et factor-identificator
     * ======================================================== */

    {
        s32 star_idx;
        s32 prod_typus;
        s32 prod_factor;
        b32 furca_inventa;
        i32 s;

        imprimere("\n--- Probans cellam furcae ---\n");

        star_idx = _symbolum(g, "STAR");
        prod_typus = _productio_per_id(g, "typus-nomen");
        prod_factor = _productio_per_id(g, "factor-identificator");
        furca_inventa = FALSUM;

        per (s = ZEPHYRUM;
             s < xar_numerus(tabula->status_tabulae) && !furca_inventa;
             s++)
        {
            Xar* actiones;
            b32 habet_typus;
            b32 habet_factor;
            i32 a;

            actiones = silva_gen_actiones_quaerere(tabula, (s32)s, star_idx);
            si (actiones == NIHIL || xar_numerus(actiones) < II)
            {
                perge;
            }

            habet_typus = FALSUM;
            habet_factor = FALSUM;
            per (a = ZEPHYRUM; a < xar_numerus(actiones); a++)
            {
                SilvaGenActioIntroitus* actio;

                actio = (SilvaGenActioIntroitus*)xar_obtinere(actiones, a);
                si (actio == NIHIL
                    || actio->actio != SILVA_GEN_ACTIO_REDUCERE)
                {
                    perge;
                }
                si (actio->valor == prod_typus)
                {
                    habet_typus = VERUM;
                }
                si (actio->valor == prod_factor)
                {
                    habet_factor = VERUM;
                }
            }

            si (habet_typus && habet_factor)
            {
                furca_inventa = VERUM;
                imprimere("  furca in statu %d: reduce typus-nomen ET "
                    "reduce factor-identificator in STAR\n", (int)s);
            }
        }

        /* IPSA ambiguitas typedef: ambae lectiones in una cella vivunt */
        CREDO_VERUM (furca_inventa);
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
