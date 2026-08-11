/* census_recensere.c - signa e censu extrahere (pars C)
 *
 * Emittit TSV: titulus <TAB> contentum <TAB> via
 * pro quoque elemento <signum> in documento .census.
 * Involucrum (census_recensere.sh) vias currit et confert -
 * hic solum analysta (pactum parseris unius: stml legit,
 * crusta praesentat).
 *
 * Exitus: 0 = signa emissa; 2 = NIHIL LECTUM (defectus, non
 * salus - pactum exituum).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "stml.h"

#include <stdio.h>

interior i32 signa_emissa = ZEPHYRUM;

interior vacuum signa_emittere(StmlNodus* nodus);

interior vacuum
campum_scribere(
    constans chorda* valor)
{
    si (valor)
    {
        imprimere("%.*s", (integer)valor->mensura,
                  (constans character*)valor->datum);
    }
    alioquin
    {
        imprimere("-");
    }
}

interior vacuum
signa_emittere(
    StmlNodus* nodus)
{
    i32 i;
    i32 numerus;

    si (!nodus)
    {
        redde;
    }

    si (nodus->genus == STML_NODUS_ELEMENTUM && nodus->titulus &&
        chorda_aequalis_literis(*nodus->titulus, "signum"))
    {
        chorda* titulus_s;
        chorda* contentum;
        chorda* via;

        titulus_s = stml_attributum_capere(nodus, "nomen");
        contentum = stml_attributum_capere(nodus, "contentum");
        via       = stml_attributum_capere(nodus, "via");

        campum_scribere(titulus_s);
        imprimere("\t");
        campum_scribere(contentum);
        imprimere("\t");
        campum_scribere(via);
        imprimere("\n");
        signa_emissa++;
    }

    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        signa_emittere(stml_liberum_ad_indicem(nodus, i));
    }
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*      piscina;
    chorda        fons;
    StmlResultus  resultus;

    si (numerus < II)
    {
        fprintf(stderr,
            "usus: census_recensere <via.census>\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("census_recensere",
                                         1048576);
    si (!piscina)
    {
        fprintf(stderr, "census_recensere: piscina deest\n");
        redde II;
    }

    fons = filum_legere_totum(argumenta[I], piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr,
            "census_recensere: '%s' legi nequit - NIHIL LECTUM\n",
            argumenta[I]);
        redde II;
    }

    resultus = stml_legere(fons, piscina,
                           internamentum_creare(piscina));
    si (!resultus.successus || !resultus.radix)
    {
        fprintf(stderr,
            "census_recensere: analysis fracta - NIHIL LECTUM\n");
        redde II;
    }

    signa_emittere(resultus.radix);

    si (signa_emissa == ZEPHYRUM)
    {
        fprintf(stderr,
            "census_recensere: signum NULLUM inventum - NIHIL"
            " probatum\n");
        redde II;
    }

    redde ZEPHYRUM;
}
