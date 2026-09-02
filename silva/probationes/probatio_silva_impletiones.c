/* probatio_silva_impletiones - clausura generum per locum (T4)
 *
 * QUAESTIO: registrum (S20) locum SPECIE typat ('nodus hic sedet').
 * Schema utile vocabularium poscit: 'unum ex his quinque'. Relatio
 * illa in tabulis coctis NON est - dextrum productionis (vector
 * symbolorum) non coquitur, sola eius longitudo - ergo ex
 * grammatica ipsa computanda est, ubi adhuc integra est.
 *
 * NUMERI FIXI consulto: emendatio quae eos movet ANNUNTIANDA est,
 * non tacite accipienda.
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

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via)
{
    FILE* pl;
    character* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(pl); redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    { fclose(pl); redde NIHIL;
    }
    rewind(pl);
    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    { fclose(pl); redde NIHIL;
    }
    si (fread(buffer, 1, (memoriae_index)mensura, pl)
        != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    buffer[mensura] = '\0';
    redde buffer;
}

/* an index titulorum titulum ferat */
interior b32
_fert (
                   Xar* index,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(index); i++)
    {
        chorda** t = (chorda**)xar_obtinere(index, i);
        si (   t != NIHIL && *t != NIHIL
            && chorda_aequalis_literis(**t, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior SilvaGenImpletio*
_impletio (
                          Xar* impletiones,
           constans character* genus,
           constans character* locus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(impletiones); i++)
    {
        SilvaGenImpletio* imp =
            (SilvaGenImpletio*)xar_obtinere(impletiones, i);
        si (   imp        != NIHIL && imp->genus != NIHIL
            && imp->locus != NIHIL
            && chorda_aequalis_literis(*imp->genus, genus)
            && chorda_aequalis_literis(*imp->locus, locus))
        {
            redde imp;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
              character  via[512];
              character* fons;
     SilvaGenGrammatica* g;
                    Xar* impletiones;

    piscina = piscina_generare_dynamicum("impletiones",
                                         LXIV * M * M);
    si (piscina == NIHIL)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL) radix = "..";
    sprintf(via, "%s/silva/grammatica/c89.stml", radix);

    imprimere("\n--- Probans clausuram impletionum (c89) ---\n");

    fons = _plagulam_legere(piscina, via);
    CREDO_NON_NIHIL (fons);
    si (fons == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    g = silva_gen_grammaticam_legere(piscina, intern, fons);
    CREDO_NON_NIHIL (g);
    si (g == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    impletiones = silva_gen_impletiones_computare(g);
    CREDO_NON_NIHIL (impletiones);

    {
        i32  paria;
        i32  genera_distincta;
        i32  sine_impletione;
        i32  i;
        Xar* genera_visa;

        paria            = xar_numerus(impletiones);
        sine_impletione  = ZEPHYRUM;
        genera_visa       = xar_creare(piscina,
                                       (i32)magnitudo(chorda*));

        per (i = ZEPHYRUM; i < paria; i++)
        {
            SilvaGenImpletio* imp =
                (SilvaGenImpletio*)xar_obtinere(impletiones, i);

            si (imp == NIHIL) perge;

            /* RESIDUUM: par quod nihil admittit = clausura
             * defecit, non grammatica tacet */
            si (   xar_numerus(imp->nodi)     == ZEPHYRUM
                && xar_numerus(imp->lexemata) == ZEPHYRUM)
            {
                sine_impletione++;
                imprimere("  RESIDUUM: %.*s / %.*s\n",
                    (integer)imp->genus->mensura,
                    (character*)imp->genus->datum,
                    (integer)imp->locus->mensura,
                    (character*)imp->locus->datum);
            }

            si (!_fert(genera_visa,
                       chorda_ut_cstr(*imp->genus, piscina)))
            {
                chorda** locus_g  = (chorda**)xar_addere(genera_visa);
                *locus_g          = imp->genus;
            }
        }
        genera_distincta = xar_numerus(genera_visa);

        imprimere("  paria %d, genera %d, residua %d\n",
                  (integer)paria, (integer)genera_distincta,
                  (integer)sine_impletione);

        /* NUMERI FIXI: 58 genera - 5 genera-extra = 53;
         * 176 ingressus locorum - 11 extra-generum = 165. */
        CREDO_AEQUALIS_I32 (paria, (i32)165);
        CREDO_AEQUALIS_I32 (genera_distincta, (i32)53);
        CREDO_AEQUALIS_I32 (sine_impletione, ZEPHYRUM);
    }

    /* CASUS QUI CONSILIUM DECREVIT: locus 'declarator' definitionis
     * functionis quinque genera admittit - et 'declarator-abstractus'
     * NON, quod species sola dicere non potest. Numerus solus hic
     * non sufficit: adesse ET abesse ambo asseruntur. */
    {
        SilvaGenImpletio* imp;

        imp = _impletio(impletiones, "definitio-functionis",
                        "declarator");
        CREDO_NON_NIHIL (imp);
        si (imp != NIHIL)
        {
            CREDO_VERUM (_fert(imp->nodi, "declarator-aciei"));
            CREDO_VERUM (_fert(imp->nodi, "declarator-functionis"));
            CREDO_VERUM (_fert(imp->nodi, "declarator-monstrator"));
            CREDO_VERUM (_fert(imp->nodi, "declarator-titulus"));
            CREDO_VERUM (_fert(imp->nodi, "parenthesis"));
            /* ABSENTIA est probatio vera - praesentia sola ex
             * clausura nimis lata quoque sequeretur */
            CREDO_FALSUM (_fert(imp->nodi, "declarator-abstractus"));
        }

        /* et ubi declarator-abstractus IURE sedet */
        imp = _impletio(impletiones, "parametrum", "declarator");
        CREDO_NON_NIHIL (imp);
        si (imp != NIHIL)
        {
            CREDO_VERUM (_fert(imp->nodi, "declarator-abstractus"));
        }
    }

    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    piscina_destruere(piscina);
    redde I;
}
