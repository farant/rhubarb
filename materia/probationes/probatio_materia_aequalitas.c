/* probatio_materia_aequalitas.c - Comparator arborum
 *
 * Arbores manu structae (nullo registro - comparator tabularium
 * non accipit, genera numeri arbitrarii sunt). Una divergentia
 * plantata per campum collatum, quaeque campum SUUM nominare
 * debet - comparator qui 'inaequales' solum dicit dimidium
 * instrumenti est.
 *
 * Duo praeter ordinarium hic pinguntur:
 *   1. PROSPECTUS - repositorium listae ULTRA prospectum crescere
 *      potest (furca in eodem Xar); comparatio mensuram PROSPECTUS
 *      legere debet, numquam xar_numerus repositorii.
 *   2. MODUS FISSUS - byte_offset/linea in STRUCTURALI non
 *      conferuntur, in FIDELITATE conferuntur; signum provenientiae
 *      (offset < 0) UTROQUE modo. Mensuratum ex silva, hic pinctum
 *      ne portatio a fonte suo divergat.
 */

#include "latina.h"
#include "credo.h"
#include "materia_arbor.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

interior MateriaToken*
_lexema (
               Piscina* piscina,
                   s32  genus,
    constans character* literis,
                   s32  offset)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(literis, piscina), offset, (i32)I, (i32)I,
        (s32)ZEPHYRUM);
}

/* Arbor exemplaris:
 *   radix (genus VII, loci III)
 *     [0] TOKEN  genus I "alfa" offset 0, trivium ante (genus III " ")
 *     [1] LISTA  { filius (genus IX, locus I: TOKEN genus II "beta") }
 *     [2] NIHIL  (numquam positus)
 */
interior MateriaNodus*
_arborem_struere (
    Piscina* piscina)
{
      MateriaNodus* radix;
      MateriaNodus* filius;
      MateriaToken* caput;
      MateriaToken* trivium;
      MateriaValor  lista;

    radix = materia_nodus_creare(piscina, (s32)VII, (i32)III);
    si (radix == NIHIL)
    { redde NIHIL;
    }

    caput    = _lexema(piscina, (s32)I, "alfa", (s32)ZEPHYRUM);
    trivium  = _lexema(piscina, (s32)III, " ", (s32)ZEPHYRUM);
    si (caput == NIHIL || trivium == NIHIL)
    { redde NIHIL;
    }
    si (!materia_token_trivia_ante_ponere(caput, piscina, &trivium,
            (i32)I))
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(radix, (i32)ZEPHYRUM,
            materia_valor_token(caput), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }

    filius = materia_nodus_creare(piscina, (s32)IX, (i32)I);
    si (filius == NIHIL)
    { redde NIHIL;
    }
    si (!materia_nodus_ponere(filius, (i32)ZEPHYRUM,
            materia_valor_token(_lexema(piscina, (s32)II, "beta",
                (s32)V)),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }

    lista = materia_valor_lista_nova(piscina);
    lista = materia_valor_lista_appendere(piscina, lista,
        materia_valor_nodus(filius));
    si (!materia_nodus_ponere(radix, (i32)I, lista,
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde radix;
}

interior MateriaToken*
_caput_arboris (
    MateriaNodus* arbor)
{
    redde arbor->loci[ZEPHYRUM].datum.token;
}

interior MateriaNodus*
_filius_arboris (
    MateriaNodus* arbor)
{
    redde materia_valor_lista_obtinere(arbor->loci[I],
        (i32)ZEPHYRUM)->datum.nodus;
}

s32
principale (vacuum)
{
                        b32  praeteritus;
                    Piscina* piscina;
    MateriaArborDifferentia  d;

    piscina = piscina_generare_dynamicum("probatio_aequalitatis",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: aequalitas, nullitas, pater radicis
     * ================================================== */

    {
        MateriaNodus* a;
        MateriaNodus* b;

        imprimere("\n--- Probans arbores aequales ---\n");
        a = _arborem_struere(piscina);
        b = _arborem_struere(piscina);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (d.campus == NIHIL);
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d));

        imprimere("\n--- Probans NIHIL utrimque = aequales ---\n");
        CREDO_VERUM (materia_arbor_aequalis(NIHIL, NIHIL,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_FALSUM (materia_arbor_aequalis(a, NIHIL,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_NON_NIHIL (d.campus);
        CREDO_VERUM (strcmp(d.campus, "nodus/nihil") == ZEPHYRUM);

        imprimere("\n--- Probans patrem RADICUM exclusum ---\n");
        /* Subarbor electa patrem habet, arbor seorsum lecta non
         * habet - radices conferre divergentias falsas dat (silva:
         * CIX super corpus). Interiorum nullitas SOLA confertur. */
        a->pater = a;
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        a->pater = NIHIL;
    }


    /* ==================================================
     * PROBARE: divergentiae nodi
     * ================================================== */

    {
        MateriaNodus* a;
        MateriaNodus* b;

        imprimere("\n--- Probans divergentias nodi ---\n");
        a = _arborem_struere(piscina);

        b         = _arborem_struere(piscina);
        b->genus  = (s32)VIII;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "nodus/genus") == ZEPHYRUM);

        b = materia_nodus_creare(piscina, (s32)VII, (i32)II);
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "nodus/numerus-locorum")
            == ZEPHYRUM);

        /* locus positus contra numquam positum */
        b = materia_nodus_creare(piscina, (s32)VII, (i32)III);
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "locus/genus-valoris")
            == ZEPHYRUM);

        imprimere("\n--- Probans patrem INTERIORUM collatum ---\n");
        b                          = _arborem_struere(piscina);
        _filius_arboris(a)->pater  = a;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "nodus/pater-nullitas")
            == ZEPHYRUM);
        CREDO_VERUM (d.via[ZEPHYRUM] != '\0');
        _filius_arboris(a)->pater = NIHIL;
    }


    /* ==================================================
     * PROBARE: listae per PROSPECTUM
     * ================================================== */

    {
          MateriaNodus* a;
          MateriaNodus* b;
          MateriaValor  ampliata;

        imprimere("\n--- Probans prospectum contra repositorium ---\n");
        a = _arborem_struere(piscina);
        b = _arborem_struere(piscina);

        /* Furca: repositorium b ultra prospectum b crescit. Arbor b
         * IPSA immutata est - comparatio quae xar_numerus legeret
         * hic divergeret, quae prospectum legit aequalitatem videt. */
        ampliata = materia_valor_lista_appendere(piscina,
            b->loci[I], materia_valor_nodus(_arborem_struere(piscina)));
        (vacuum)ampliata;
        CREDO_AEQUALIS_I32 (xar_numerus(b->loci[I].datum.lista.xar),
            (i32)II);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(b->loci[I]),
            (i32)I);
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));

        imprimere("\n--- Probans numerum listae divergentem ---\n");
        b = _arborem_struere(piscina);
        b->loci[I] = materia_valor_lista_appendere(piscina,
            b->loci[I], materia_valor_nodus(_arborem_struere(piscina)));
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lista/numerus") == ZEPHYRUM);
    }


    /* ==================================================
     * PROBARE: divergentiae lexematum
     * ================================================== */

    {
        MateriaNodus* a;
        MateriaNodus* b;

        imprimere("\n--- Probans divergentias lexematum ---\n");
        a = _arborem_struere(piscina);

        b                         = _arborem_struere(piscina);
        _caput_arboris(b)->genus  = (s32)IV;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/genus") == ZEPHYRUM);
        CREDO_NON_NIHIL (d.lexema_a);
        CREDO_NON_NIHIL (d.lexema_b);

        b                         = _arborem_struere(piscina);
        _caput_arboris(b)->valor  = chorda_ex_literis("alfax", piscina);
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/valor") == ZEPHYRUM);

        b                              = _arborem_struere(piscina);
        _caput_arboris(b)->fons_index  = (s32)II;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/fons") == ZEPHYRUM);

        b = _arborem_struere(piscina);
        materia_token_initium_lineae_ponere(_caput_arboris(b), VERUM);
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/initium-lineae")
            == ZEPHYRUM);

        imprimere("\n--- Probans provenientiam UTROQUE modo ---\n");
        /* Signum offset (verum contra syntheticum) factum
         * STRUCTURALE est - custodia quae 'solum cum ambo adsint'
         * conferret transiret quia subiectum abesset. */
        b                               = _arborem_struere(piscina);
        _caput_arboris(b)->byte_offset  = -I;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/provenientia")
            == ZEPHYRUM);
    }


    /* ==================================================
     * PROBARE: series triviorum
     * ================================================== */

    {
        MateriaNodus* a;
        MateriaNodus* b;

        imprimere("\n--- Probans trivia divergentia ---\n");
        a = _arborem_struere(piscina);

        b                                = _arborem_struere(piscina);
        _caput_arboris(b)->numerus_ante  = (i32)ZEPHYRUM;
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_VERUM (strcmp(d.campus, "trivia/ante") == ZEPHYRUM);

        /* valor trivii - divergentia INTRA trivium ut lexema
         * nuntiatur (trivium per eandem viam confertur), ergo
         * campus 'lexema/valor' est, non 'trivia/...' */
        b = _arborem_struere(piscina);
        _caput_arboris(b)->spatia_ante[ZEPHYRUM]->valor =
            chorda_ex_literis("  ", piscina);
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_NON_NIHIL (d.campus);
    }


    /* ==================================================
     * PROBARE: modus fissus - STRUCTURALIS contra FIDELITATEM
     * ================================================== */

    {
        MateriaNodus* a;
        MateriaNodus* b;

        imprimere("\n--- Probans modum fissum (offset) ---\n");
        a                               = _arborem_struere(piscina);
        b                               = _arborem_struere(piscina);
        _caput_arboris(b)->byte_offset  = (s32)IX;
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/offset") == ZEPHYRUM);

        imprimere("\n--- Probans modum fissum (linea) ---\n");
        b                         = _arborem_struere(piscina);
        _caput_arboris(b)->linea  = (i32)V;
        CREDO_VERUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d));
        CREDO_VERUM (strcmp(d.campus, "lexema/linea") == ZEPHYRUM);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
