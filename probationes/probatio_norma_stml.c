/* probatio_norma_stml.c - facies STML normae: canon infixus (A2),
 * lector (A3), scriptor et oracula I, II, V (A4) - norma-plan-3 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "filum.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "norma.h"
#include "norma_canon.h"

#include <stdio.h>
#include <string.h>

/* canon infixus super documentum: numerus vitiorum; *genera = bit per
 * CanonVitiumGenus visum (canon unum defectum saepe bis nominat -
 * '<nemo/>' = LIBERUM_ILLICITUM + ELEMENTUM_IGNOTUM) */
interior i32
_canon_vitia (
    constans character* documentum,
                   i32* genera,
               Piscina* p)
{
    InternamentumChorda* in = internamentum_creare(p);
                 chorda  causa;
                  Canon* canon = canon_legere(norma_canon_textus(p), p,
                      in,
                                    &causa);
           StmlResultus  r = stml_legere_ex_literis(documentum, p, in);
                    Xar* v;
                    i32  i;

    CREDO_NON_NIHIL(canon);
    CREDO_VERUM(r.successus);
    si (!canon || !r.successus)
    {
        redde M;
    }
    v = canon_iudicare(canon, r.radix, p);
    si (genera)
    {
        *genera = 0;
        per (i = 0; i < xar_numerus(v); i++)
        {
            *genera |= (i32)I << (i32)((CanonVitium*)xar_obtinere(v,
                i))->genus;
        }
    }
    redde xar_numerus(v);
}

#define NORMA_A  "<normae versio=\"1\">\n<norma titulus=\"a\">\n"
#define NORMA_Z  "\n</norma>\n</normae>\n"

/* genus g inter vitia documenti */
interior b32
_canon_habet (
     constans character* documentum,
       CanonVitiumGenus  g,
                Piscina* p)
{
    i32 genera = 0;

    _canon_vitia(documentum, &genera, p);
    redde (genera & ((i32)I << (i32)g)) != 0;
}

interior vacuum
probatio_canon_infixus(Piscina* p)
{
    chorda discus = filum_legere_totum("norma.canon", p);

    imprimere("\n--- Probans canonem infixum ---\n");
    /* infixus == plagula: generatio non rancida */
    CREDO_CHORDA_AEQUALIS(norma_canon_textus(p), discus);
    CREDO_AEQUALIS_I32(_canon_vitia(
        "<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><objectum modus=\"notandum\">"
        "<campus titulus=\"x\"><ad norma=\"b\"/></campus>"
        "</objectum></norma>\n"
        "<norma titulus=\"b\"><textus electio=\"p q\"/></norma>\n"
        "</normae>\n", NIHIL, p), 0);
    CREDO_VERUM(_canon_habet(NORMA_A "<nemo/>" NORMA_Z,
        CANON_ELEMENTUM_IGNOTUM, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<integer forma=\"uuid\"/>" NORMA_Z,
        CANON_ATTRIBUTUM_IGNOTUM, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<ad norma=\"nemo\"/>" NORMA_Z,
        CANON_CITATIO_IRRITA, p));
    CREDO_VERUM(_canon_habet("<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><textus/></norma>\n"
        "<norma titulus=\"a\"><textus/></norma>\n</normae>\n",
        CANON_NOMEN_BIS, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<objectum><campus titulus=\"x\""
        " requiritur=\"fortasse\"><textus/></campus></objectum>" NORMA_Z,
        CANON_VALOR_MALUS, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<textus/><textus/>" NORMA_Z,
        CANON_LIBERI_MULTI, p));
    /* unicitas intra objectum PROXIMUM: clavis eadem in objecto
     * interiore et exteriore licet, bis in eodem non licet */
    CREDO_AEQUALIS_I32(_canon_vitia(NORMA_A "<objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "<campus titulus=\"interius\"><objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "</objectum></campus></objectum>" NORMA_Z, NIHIL, p), 0);
    CREDO_VERUM(_canon_habet(NORMA_A "<objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "<campus titulus=\"id\"><integer/></campus>"
        "</objectum>" NORMA_Z, CANON_NOMEN_BIS, p));
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_stml",
        M * M);
        b32 successus;

    credo_aperire(p);
    probatio_canon_infixus(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
