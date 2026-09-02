/* probatio_saltuarius_bibl.c - Cache librorum LRU (Chunk B3)
 *
 * Probantur: quaerere (motus ad frontem), condere, evictio
 * vetustissimi ad N=8, evicti non inveniuntur, positio cursoris
 * per cache servata (donum reapertionis).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_bibliotheca.h"
#include <stdio.h>

interior SaltuariusLiber*
_librum_facere (
               Piscina* piscina,
       SaltuariusNexus* nexus,
    constans character* via)
{
    redde saltuarius_liber_aperire(piscina, nexus,
        chorda_ex_literis(via, piscina),
        chorda_ex_literis("contentum\n", piscina));
}

s32
principale (vacuum)
{
                      b32  praeteritus;
                  Piscina* piscina;
          SaltuariusNexus* nexus;
    SaltuariusBibliotheca  bibliotheca;

    piscina = piscina_generare_dynamicum("probatio_salt_bibl",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    saltuarius_bibliotheca_parare(&bibliotheca);


    /* ========================================================
     * PROBARE: condere + quaerere + motus ad frontem
     * ======================================================== */

    {
        SaltuariusLiber* a;
        SaltuariusLiber* b;

        imprimere("\n--- Probans condere et quaerere ---\n");

        a = _librum_facere(piscina, nexus, "a.md");
        b = _librum_facere(piscina, nexus, "b.md");
        saltuarius_bibliotheca_condere(&bibliotheca, a);
        saltuarius_bibliotheca_condere(&bibliotheca, b);
        CREDO_AEQUALIS_I32 ((i32)bibliotheca.numerus, II);

        /* quaerere a: inventum ET ad frontem motum */
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_bibliotheca_quaerere(
                &bibliotheca, chorda_ex_literis("a.md", piscina)),
            (constans vacuum*)a);
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)bibliotheca.libri[ZEPHYRUM],
            (constans vacuum*)a);
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_bibliotheca_quaerere(
                &bibliotheca, chorda_ex_literis("absens.md",
                    piscina)),
            (constans vacuum*)NIHIL);

        /* positio cursoris servata trans cache */
        a->cursor_linea    = ZEPHYRUM;
        a->cursor_columna  = III;
        CREDO_AEQUALIS_S32 (
            saltuarius_bibliotheca_quaerere(&bibliotheca,
                chorda_ex_literis("a.md", piscina))->cursor_columna,
            III);
    }


    /* ========================================================
     * PROBARE: evictio ad N=8
     * ======================================================== */

    {
              i32 k;
        character titulus[XVI];

        imprimere("\n--- Probans evictionem ---\n");

        /* VI libri plures (a b iam insunt) -> VIII pleni; deinde
         * duo plures -> a et b evicti (vetustissimi) */
        per (k = ZEPHYRUM; k < VIII; k++)
        {
            titulus[ZEPHYRUM]  = (character)('c' + (s32)k);
            titulus[I]         = '.';
            titulus[II]        = 'm';
            titulus[III]       = 'd';
            titulus[IV]        = '\0';
            saltuarius_bibliotheca_condere(&bibliotheca,
                _librum_facere(piscina, nexus, titulus));
        }
        CREDO_AEQUALIS_I32 ((i32)bibliotheca.numerus,
            (i32)SALT_LIBRI_MAXIMI);
        /* a (fronte post quaerere) et b: b vetustissimus evictus
         * primo, deinde a */
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_bibliotheca_quaerere(
                &bibliotheca, chorda_ex_literis("b.md", piscina)),
            (constans vacuum*)NIHIL);
        CREDO_AEQUALIS_PTR (
            (constans vacuum*)saltuarius_bibliotheca_quaerere(
                &bibliotheca, chorda_ex_literis("a.md", piscina)),
            (constans vacuum*)NIHIL);
        /* recentiores manent */
        CREDO_NON_NIHIL (saltuarius_bibliotheca_quaerere(
            &bibliotheca, chorda_ex_literis("j.md", piscina)));

        saltuarius_bibliotheca_vacare(&bibliotheca);
        CREDO_AEQUALIS_I32 ((i32)bibliotheca.numerus, ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
