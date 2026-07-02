/* probatio_silva_tabulae.c - Tabulae coctae (Phase 3 Chunk C)
 *
 * Circulus honestus: harnessa plagulas COCTAS compilat (fontes/
 * silva_tabulae_sceleti.c, GENERATAS per generare.sh) et eas contra
 * tabulam RECENTER constructam (eadem grammatica ex disco) verificat:
 * numeri, laminae per statum, initium, et cella furcae in forma cocta.
 * Si grammatica mutatur sine regeneratione, haec probatio frangit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_generare.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
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
_literis_est (constans character* a, chorda* b)
{
    si (a == NIHIL || b == NIHIL)
    {
        redde (a == NIHIL && b == NIHIL) ? VERUM : FALSUM;
    }
    si ((i32)strlen(a) != b->mensura)
    {
        redde FALSUM;
    }
    redde (memcmp(a, b->datum, (memoriae_index)b->mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
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
    SilvaGenTabula* recens;
    constans SilvaTabulaCocta* cocta;

    piscina = piscina_generare_dynamicum("probatio_silva_tabulae", 16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    cocta = &SILVA_SCELETUM_TABULA;

    /* Tabulam recentem construere ex eadem grammatica */
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }
    sprintf(via, "%s/silva/grammatica/sceletum.stml", radix);
    fons = _plagulam_legere(piscina, via);
    g = (fons != NIHIL)
        ? silva_gen_grammaticam_legere(piscina, intern, fons) : NIHIL;
    CREDO_NON_NIHIL (g);
    si (g == NIHIL)
    {
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    silva_gen_first_computare(g);
    silva_gen_follow_computare(g);
    recens = silva_gen_tabulam_construere(
        silva_gen_collectio_lalr_construere(g));
    CREDO_NON_NIHIL (recens);


    /* ========================================================
     * PROBARE: numeri cocti == numeri recentes
     * ======================================================== */

    {
        imprimere("\n--- Probans numeros ---\n");

        CREDO_AEQUALIS_I32 (cocta->numerus_symbolorum,
            xar_numerus(g->symbola));
        CREDO_AEQUALIS_I32 (cocta->numerus_terminalium,
            g->numerus_terminalium);
        CREDO_AEQUALIS_I32 (cocta->numerus_productionum,
            xar_numerus(g->productiones));
        CREDO_AEQUALIS_I32 (cocta->numerus_statuum,
            xar_numerus(recens->status_tabulae));
        CREDO_AEQUALIS_S32 (cocta->numerus_conflictuum,
            recens->numerus_conflictuum);
        CREDO_AEQUALIS_S32 (cocta->initium_index, g->initium_index);
    }


    /* ========================================================
     * PROBARE: symbola et productiones congruunt
     * ======================================================== */

    {
        i32 i;

        imprimere("\n--- Probans symbola/productiones ---\n");

        per (i = ZEPHYRUM; i < cocta->numerus_symbolorum; i++)
        {
            SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
                g->symbola, i);

            si (!_literis_est(cocta->symbola[i].titulus, sym->titulus)
                || cocta->symbola[i].est_terminale != sym->est_terminale)
            {
                frange;
            }
        }
        CREDO_AEQUALIS_I32 (i, cocta->numerus_symbolorum);

        per (i = ZEPHYRUM; i < cocta->numerus_productionum; i++)
        {
            SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                g->productiones, i);

            si (cocta->productiones[i].sinistrum != prod->sinistrum
                || cocta->productiones[i].longitudo
                    != (i32)xar_numerus(prod->dextrum)
                || !_literis_est(cocta->productiones[i].id, prod->id))
            {
                frange;
            }
        }
        CREDO_AEQUALIS_I32 (i, cocta->numerus_productionum);

        /* genus lexematis terminalis primi: IDENTIFICATOR */
        CREDO_AEQUALIS_I32 ((i32)cocta->symbola[ZEPHYRUM].genus_lexematis,
            (i32)SILVA_LEX_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: laminae statuum congruunt (numeri actionum/goto)
     * ======================================================== */

    {
        i32 s;
        i32 summa_actionum;

        imprimere("\n--- Probans laminas statuum ---\n");

        summa_actionum = ZEPHYRUM;
        per (s = ZEPHYRUM; s < cocta->numerus_statuum; s++)
        {
            SilvaGenStatusTabula* st = (SilvaGenStatusTabula*)xar_obtinere(
                recens->status_tabulae, s);

            si (cocta->status[s].actiones_numerus
                    != (i32)xar_numerus(st->actiones)
                || cocta->status[s].actiones_offset != summa_actionum)
            {
                frange;
            }
            summa_actionum += cocta->status[s].actiones_numerus;
        }
        CREDO_AEQUALIS_I32 (s, cocta->numerus_statuum);
        CREDO_AEQUALIS_I32 (summa_actionum, cocta->numerus_actionum);
    }


    /* ========================================================
     * PROBARE: cella furcae in forma COCTA
     * ======================================================== */

    {
        s32 prod_typus;
        s32 prod_factor;
        s32 star_idx;
        b32 furca_inventa;
        i32 s;
        i32 i;

        imprimere("\n--- Probans cellam furcae coctam ---\n");

        /* invenire indices per id in tabula COCTA */
        prod_typus = -I;
        prod_factor = -I;
        per (i = ZEPHYRUM; i < cocta->numerus_productionum; i++)
        {
            si (cocta->productiones[i].id != NIHIL)
            {
                si (strcmp(cocta->productiones[i].id, "typus-nomen")
                    == ZEPHYRUM)
                {
                    prod_typus = (s32)i;
                }
                si (strcmp(cocta->productiones[i].id,
                        "factor-identificator") == ZEPHYRUM)
                {
                    prod_factor = (s32)i;
                }
            }
        }
        CREDO_VERUM (prod_typus >= ZEPHYRUM);
        CREDO_VERUM (prod_factor >= ZEPHYRUM);

        star_idx = -I;
        per (i = ZEPHYRUM; i < cocta->numerus_symbolorum; i++)
        {
            si (strcmp(cocta->symbola[i].titulus, "STAR") == ZEPHYRUM)
            {
                star_idx = (s32)i;
            }
        }
        CREDO_VERUM (star_idx >= ZEPHYRUM);

        furca_inventa = FALSUM;
        per (s = ZEPHYRUM; s < cocta->numerus_statuum && !furca_inventa; s++)
        {
            b32 habet_typus = FALSUM;
            b32 habet_factor = FALSUM;

            per (i = cocta->status[s].actiones_offset;
                 i < cocta->status[s].actiones_offset
                     + cocta->status[s].actiones_numerus;
                 i++)
            {
                constans SilvaTabActio* actio = &cocta->actiones[i];

                si (actio->terminalis != star_idx
                    || actio->actio != (s32)SILVA_TAB_ACTIO_REDUCERE)
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
                imprimere("  furca cocta in statu %d\n", (int)(s));
            }
        }
        CREDO_VERUM (furca_inventa);

        /* pretium: actiones normales pretium 0 ferunt */
        CREDO_AEQUALIS_I32 (cocta->actiones[ZEPHYRUM].pretium, ZEPHYRUM);
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
