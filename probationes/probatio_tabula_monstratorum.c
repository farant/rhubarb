/* probatio_tabula_monstratorum.c - Probationes Tabulae Monstratorum */
#include "latina.h"
#include "piscina.h"
#include "tabula_monstratorum.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Creatio, tabula vacua, argumenta NIHIL
 * ================================================== */

interior vacuum
_sectio_creatio (
    CredoContextus* c)
{
    TabulaMonstratorum* t;
                   i32  x = ZEPHYRUM;

    CREDO_NIHIL(tabula_monstratorum_creare(NIHIL, XVI));
    CREDO_NIHIL(tabula_monstratorum_creare(c->piscina,
        (i32)0x20000000));

    t = tabula_monstratorum_creare(c->piscina, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), ZEPHYRUM);
    CREDO_NIHIL(tabula_monstratorum_invenire(t, &x));

    CREDO_NIHIL(tabula_monstratorum_invenire(t, NIHIL));
    CREDO_NIHIL(tabula_monstratorum_locus(t, NIHIL));
    CREDO_NIHIL(tabula_monstratorum_invenire(NIHIL, &x));
    CREDO_NIHIL(tabula_monstratorum_locus(NIHIL, &x));
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(NIHIL), ZEPHYRUM);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), ZEPHYRUM);
}


/* ==================================================
 * Locus: clavis nova valorem NIHIL habet; clavis praesens eundem
 * locum reddit; valor renovari potest
 * ================================================== */

interior vacuum
_sectio_locus (
    CredoContextus* c)
{
    TabulaMonstratorum*  t;
                   i32   claves[III];
                   i32   valores[III];
                vacuum** l;
                vacuum** m;

    t = tabula_monstratorum_creare(c->piscina, XVI);
    CREDO_NECESSE_NON_NIHIL(t);

    l = tabula_monstratorum_locus(t, &claves[0]);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NIHIL(*l);
    *l = &valores[0];
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), I);

    m = tabula_monstratorum_locus(t, &claves[0]);
    CREDO_AEQUALIS_PTR(m, l);
    CREDO_AEQUALIS_PTR(*m, &valores[0]);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), I);

    *m = &valores[I];
    CREDO_AEQUALIS_PTR(tabula_monstratorum_invenire(t, &claves[0]),
        &valores[I]);

    /* clavis vicina (octeti IV distans) alia est */
    CREDO_NIHIL(tabula_monstratorum_invenire(t, &claves[I]));
    l = tabula_monstratorum_locus(t, &claves[I]);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NIHIL(*l);
    *l = &valores[II];
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), II);
    CREDO_AEQUALIS_PTR(tabula_monstratorum_invenire(t, &claves[0]),
        &valores[I]);
    CREDO_AEQUALIS_PTR(tabula_monstratorum_invenire(t, &claves[I]),
        &valores[II]);
    CREDO_NIHIL(tabula_monstratorum_invenire(t, &claves[II]));
}


/* ==================================================
 * Valor NIHIL relictus: clavis in numero, invenire NIHIL; locus
 * posterior eundem locum vacuum reddit
 * ================================================== */

interior vacuum
_sectio_valor_nihil (
    CredoContextus* c)
{
    TabulaMonstratorum*  t;
                   i32   clavis  = ZEPHYRUM;
                   i32   valor   = ZEPHYRUM;
                vacuum** l;

    t = tabula_monstratorum_creare(c->piscina, XVI);
    CREDO_NECESSE_NON_NIHIL(t);
    l = tabula_monstratorum_locus(t, &clavis);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), I);
    CREDO_NIHIL(tabula_monstratorum_invenire(t, &clavis));
    CREDO_AEQUALIS_PTR(tabula_monstratorum_locus(t, &clavis), l);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), I);
    *l = &valor;
    CREDO_AEQUALIS_PTR(tabula_monstratorum_invenire(t, &clavis),
        &valor);
}


/* ==================================================
 * Crescentia: XX.000 claves (gradu XVI octetorum, ut nodi piscinae)
 * a capacitate XVI - omnes post duplicationes inveniuntur, claves
 * aliae non
 * ================================================== */

#define CLAVES_MULTAE 20000

interior vacuum
_sectio_crescentia (
    CredoContextus* c)
{
    TabulaMonstratorum* t;
                    i8* claves;
                    i8* valores;
                    i8* alienae;
                   i32  k;
                   i32  errata = ZEPHYRUM;

    claves  = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_MULTAE * XVI);
    valores = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_MULTAE);
    alienae = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_MULTAE * XVI);
    t = tabula_monstratorum_creare(c->piscina, XVI);
    CREDO_NECESSE_NON_NIHIL(claves);
    CREDO_NECESSE_NON_NIHIL(valores);
    CREDO_NECESSE_NON_NIHIL(alienae);
    CREDO_NECESSE_NON_NIHIL(t);

    per (k = ZEPHYRUM; k < CLAVES_MULTAE; k++)
    {
        vacuum** l = tabula_monstratorum_locus(t, claves + k * XVI);

        si (l == NIHIL || *l != NIHIL)
        {
            errata++;
            perge;
        }
        *l = valores + k;
    }
    CREDO_AEQUALIS_I32(errata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), CLAVES_MULTAE);

    per (k = ZEPHYRUM; k < CLAVES_MULTAE; k++)
    {
        si (tabula_monstratorum_invenire(t, claves + k * XVI)
            != (vacuum*)(valores + k))
        {
            errata++;
        }
        si (tabula_monstratorum_invenire(t, alienae + k * XVI) != NIHIL)
        {
            errata++;
        }
    }
    CREDO_AEQUALIS_I32(errata, ZEPHYRUM);

    /* iterum addere: nihil novum */
    per (k = ZEPHYRUM; k < CLAVES_MULTAE; k++)
    {
        vacuum** l = tabula_monstratorum_locus(t, claves + k * XVI);

        si (l == NIHIL || *l != (vacuum*)(valores + k))
        {
            errata++;
        }
    }
    CREDO_AEQUALIS_I32(errata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), CLAVES_MULTAE);
}


/* ==================================================
 * Claves congestae: gradus IV.XCVI octetorum (bita infima omnia
 * aequalia) - series longae et circuitus per finem tabulae
 * ================================================== */

#define CLAVES_CONGESTAE 1000

interior vacuum
_sectio_congestae (
    CredoContextus* c)
{
    TabulaMonstratorum* t;
                    i8* area;
                    i8* valores;
                   i32  k;
                   i32  errata = ZEPHYRUM;

    area     = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_CONGESTAE * 4096);
    valores  = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_CONGESTAE);
    t = tabula_monstratorum_creare(c->piscina, XVI);
    CREDO_NECESSE_NON_NIHIL(area);
    CREDO_NECESSE_NON_NIHIL(valores);
    CREDO_NECESSE_NON_NIHIL(t);

    per (k = ZEPHYRUM; k < CLAVES_CONGESTAE; k++)
    {
        vacuum** l = tabula_monstratorum_locus(t, area + k * 4096);

        si (l == NIHIL || *l != NIHIL)
        {
            errata++;
            perge;
        }
        *l = valores + k;
    }
    per (k = ZEPHYRUM; k < CLAVES_CONGESTAE; k++)
    {
        si (tabula_monstratorum_invenire(t, area + k * 4096)
            != (vacuum*)(valores + k))
        {
            errata++;
        }
        /* media inter claves: absens */
        si (tabula_monstratorum_invenire(t, area + k * 4096 + 2048)
            != NIHIL)
        {
            errata++;
        }
    }
    CREDO_AEQUALIS_I32(errata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t),
        CLAVES_CONGESTAE);
}


/* ==================================================
 * Allocatio fracta: piscina certae magnitudinis - crescentia
 * deficit, locus NIHIL reddit, tabula integra manet
 * ================================================== */

interior vacuum
_sectio_allocatio_fracta (
    CredoContextus* c)
{
               Piscina* fixa;
    TabulaMonstratorum* t;
                    i8* claves;
                    i8* valores;
                   i32  k;
                   i32  positae  = ZEPHYRUM;
                   i32  errata   = ZEPHYRUM;

    claves  = (i8*)piscina_allocare(c->piscina, (memoriae_index)
        CLAVES_MULTAE * XVI);
    valores = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)CLAVES_MULTAE);
    fixa    = piscina_generare_certae_magnitudinis("tabula_fixa",
        XVI * CCLVI);
    CREDO_NECESSE_NON_NIHIL(claves);
    CREDO_NECESSE_NON_NIHIL(valores);
    CREDO_NECESSE_NON_NIHIL(fixa);
    t = tabula_monstratorum_creare(fixa, XVI);
    CREDO_NECESSE_NON_NIHIL(t);

    per (k = ZEPHYRUM; k < CLAVES_MULTAE; k++)
    {
        vacuum** l = tabula_monstratorum_locus(t, claves + k * XVI);

        si (l == NIHIL)
        {
            frange;
        }
        *l = valores + k;
        positae++;
    }
    /* aliquot crescentiae successerunt, deinde una defecit */
    CREDO_MAIOR_I32(positae, XII);
    CREDO_MINOR_I32(positae, CLAVES_MULTAE);
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), positae);

    /* clavis praesens etiam nunc: crescentia non poscitur */
    CREDO_NON_NIHIL(tabula_monstratorum_locus(t, claves));
    per (k = ZEPHYRUM; k < positae; k++)
    {
        si (tabula_monstratorum_invenire(t, claves + k * XVI)
            != (vacuum*)(valores + k))
        {
            errata++;
        }
    }
    CREDO_AEQUALIS_I32(errata, ZEPHYRUM);
    CREDO_NIHIL(tabula_monstratorum_invenire(t, claves
        + positae * XVI));
    CREDO_AEQUALIS_I32(tabula_monstratorum_numerus(t), positae);

    piscina_destruere(fixa);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "creatio et argumenta NIHIL", _sectio_creatio, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "locus: novus, praesens, renovatus", _sectio_locus, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "valor NIHIL relictus", _sectio_valor_nihil, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "crescentia (XX.000 claves)", _sectio_crescentia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "claves congestae", _sectio_congestae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "allocatio fracta", _sectio_allocatio_fracta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tabula_monstratorum", SECTIONES);
}
