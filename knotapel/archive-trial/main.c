/*
 * KNOTAPEL ARCHIVE TRIAL: regression fixture for archive.sh
 * ================================================================
 *
 * Not a numbered demo. The smallest program that uses a house
 * library (laqueus, and through it magnus/fractio/situs/polynomium/
 * anulus/matrix) so that archive.sh can be exercised end to end:
 * closure, static renaming (the laqueus closure has ten colliding
 * file-local names), snapshot compile under house flags, and
 * live-vs-snapshot output identity.
 *
 * Prints the invariants of a 12-vertex trefoil (every 4th vertex of
 * the 48-vertex table in probationes/probatio_laqueus.c).
 *
 * NOTE for demos that include house headers: latina.h comes along,
 * so every Roman numeral (I, V, X, L, C, D, M, ...) and the Latin
 * keywords (si, per, dum, ...) are macros here. Avoid them as names.
 */

#include "laqueus.h"
#include <stdio.h>

static const char* TREFOIL =
    "[(300, 0, 0), (100, 173, -100), (-50, 87, 0), (-200, 0, 100), "
    "(-150, -260, 0), (100, -173, -100), (100, 0, 0), (100, 173, 100), "
    "(-150, 260, 0), (-200, 0, -100), (-50, -87, 0), (100, -173, 100)]";

static void
print_chorda (
    const char* label,
    chorda      text)
{
    printf("%s%.*s\n", label, (int)text.mensura, (const char*)text.datum);
}

int
main (void)
{
    Piscina*   pool = piscina_generare_dynamicum("archive_trial", 1048576);
    Laqueus    knot;
    Diagramma  diagram;
    Polynomium poly;

    if (pool == NULL
        || !laqueus_ex_chorda(chorda_ex_literis(TREFOIL, pool), pool,
               &knot))
    {
        printf("parse failed\n");
        return 1;
    }
    printf("vertices: %u\n", (unsigned)laqueus_numerus(knot));
    printf("simple: %d\n", (int)laqueus_simplex(knot, pool));
    if (!laqueus_diagramma_genericum(knot, pool, &diagram))
    {
        printf("no generic projection\n");
        return 1;
    }
    printf("crossings: %u\n", (unsigned)diagramma_numerus(diagram));
    printf("writhe: %d\n", (int)diagramma_scriptura(diagram));
    if (diagramma_jones(diagram, pool, &poly))
    {
        print_chorda("jones: ", polynomium_ad_chordam(poly, 't', pool));
    }
    if (diagramma_alexander(diagram, pool, &poly))
    {
        print_chorda("alexander: ", polynomium_ad_chordam(poly, 't',
            pool));
    }
    piscina_destruere(pool);
    return 0;
}
