/* probatio_materia_nodus.c - Nodus, valor, sedes
 *
 * Duo hic praeter usum ordinarium probantur:
 *   1. PROSPECTUS FURCA-TUTUS - duae furcae in eodem repositorio
 *      appendentes se invicem laedere NON debent. Illa proprietas
 *      est tota causa cur valor listae {xar, mensura} sit et non
 *      Xar nudus; sine probatione argumentum verbum est.
 *   2. UNCUS SEDIS - via NIHIL campos lexematis reddit (CSS/HTML
 *      recte), via unci quod frons dicit (C89 catenam originis).
 */

#include "latina.h"
#include "credo.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Uncus fictus: sedem fixam reddit, ut via unci a via lexematis
 * DISCERNI possit */
hic_manens vacuum
_uncus_fictus (
                     vacuum* datum,
      constans MateriaToken* token,
               MateriaSedes* sedes)
{
    (vacuum)token;
    sedes->byte_offset  = *(s32*)datum;
    sedes->linea        = (i32)999;
    sedes->columna      = (i32)7;
    sedes->fons_index   = (s32)3;
    sedes->est_fons     = FALSUM;
}

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;
     chorda  valor;

    piscina = piscina_generare_dynamicum("probatio_materia_nodus",
        32768);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    valor = chorda_ex_literis("x", piscina);


    /* ========================================================
     * PROBARE: valores et congruentia
     * ======================================================== */

    {
        MateriaValor v;

        imprimere("\n--- Probans valores ---\n");

        v = materia_valor_nihil();
        CREDO_AEQUALIS_S32 ((s32)v.genus, (s32)MATERIA_VALOR_NIHIL);
        v = materia_valor_index((s32)5);
        CREDO_AEQUALIS_S32 ((s32)v.genus, (s32)MATERIA_VALOR_INDEX);
        CREDO_AEQUALIS_S32 (v.datum.index, (s32)5);

        /* Matrix congruentiae: species locorum contra genera valorum */
        CREDO_VERUM  (materia_valor_congruit(materia_valor_nodus(NIHIL),
            MATERIA_LOCUS_NODUS));
        CREDO_FALSUM (materia_valor_congruit(materia_valor_nodus(NIHIL),
            MATERIA_LOCUS_TOKEN));
        CREDO_VERUM  (materia_valor_congruit(materia_valor_token(NIHIL),
            MATERIA_LOCUS_TOKEN));
        CREDO_FALSUM (materia_valor_congruit(materia_valor_token(NIHIL),
            MATERIA_LOCUS_INDEX));
        CREDO_VERUM  (materia_valor_congruit(materia_valor_index(ZEPHYRUM),
            MATERIA_LOCUS_INDEX));
        CREDO_VERUM  (materia_valor_congruit(
            materia_valor_lista_nova(piscina),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM  (materia_valor_congruit(
            materia_valor_lista_nova(piscina),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_FALSUM (materia_valor_congruit(materia_valor_nihil(),
            MATERIA_LOCUS_NODUS));
        /* Species extra fines: FALSUM, non ruina */
        CREDO_FALSUM (materia_valor_congruit(materia_valor_nodus(NIHIL),
            MATERIA_LOCUS_NUMERUS_SPECIERUM));

        /* lista ex Xar NIHIL = nihil-valor, non lista fracta */
        CREDO_AEQUALIS_S32 ((s32)materia_valor_lista(NIHIL).genus,
            (s32)MATERIA_VALOR_NIHIL);
    }


    /* ========================================================
     * PROBARE: prospectus FURCA-TUTUS
     * ======================================================== */

    {
        MateriaValor basis;
        MateriaValor furca_a;
        MateriaValor furca_b;

        imprimere("\n--- Probans prospectum furca-tutum ---\n");

        basis = materia_valor_lista_nova(piscina);
        basis = materia_valor_lista_appendere(piscina, basis,
            materia_valor_index((s32)10));
        basis = materia_valor_lista_appendere(piscina, basis,
            materia_valor_index((s32)20));
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(basis),
            (i32)II);

        /* Furca A: in loco appendit (ad finem vivum stat) */
        furca_a = materia_valor_lista_appendere(piscina, basis,
            materia_valor_index((s32)31));
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(furca_a),
            (i32)III);

        /* Furca B ex EADEM basi: repositorium iam ultra prospectum
         * basis scriptum est, ergo COPIA fieri debet */
        furca_b = materia_valor_lista_appendere(piscina, basis,
            materia_valor_index((s32)32));
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(furca_b),
            (i32)III);

        /* Repositoria DIVERSA - hoc est copia-in-divergentia */
        CREDO_INAEQUALITAS_PTR (furca_a.datum.lista.xar,
                                furca_b.datum.lista.xar);

        /* Et neutra alteram laesit */
        CREDO_AEQUALIS_S32 (
            materia_valor_lista_obtinere(furca_a, (i32)II)->datum.index,
            (s32)31);
        CREDO_AEQUALIS_S32 (
            materia_valor_lista_obtinere(furca_b, (i32)II)->datum.index,
            (s32)32);

        /* Basis intacta - mensura SUA, non repositorii */
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(basis),
            (i32)II);
        CREDO_NIHIL (materia_valor_lista_obtinere(basis, (i32)II));

        /* Praefixum commune utrique furcae superest */
        CREDO_AEQUALIS_S32 (
            materia_valor_lista_obtinere(furca_a,
            ZEPHYRUM)->datum.index,
            (s32)10);
        CREDO_AEQUALIS_S32 (
            materia_valor_lista_obtinere(furca_b,
            ZEPHYRUM)->datum.index,
            (s32)10);

        /* Appendere in non-listam: nihil-valor */
        CREDO_AEQUALIS_S32 ((s32)materia_valor_lista_appendere(piscina,
            materia_valor_index(ZEPHYRUM),
            materia_valor_index(ZEPHYRUM)).genus,
            (s32)MATERIA_VALOR_NIHIL);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            materia_valor_index(ZEPHYRUM)), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: nodus - custodiae PLANTATAE
     * ======================================================== */

    {
        MateriaNodus* n;
        MateriaNodus* filius;

        imprimere("\n--- Probans nodum (custodiae) ---\n");

        n = materia_nodus_creare(piscina, (s32)4, (i32)III);
        CREDO_NON_NIHIL (n);
        CREDO_AEQUALIS_S32 (n->genus, (s32)4);
        CREDO_AEQUALIS_I32 (n->numerus_locorum, (i32)III);
        CREDO_NIHIL (n->pater);
        CREDO_AEQUALIS_S32 ((s32)n->loci[0].genus,
            (s32)MATERIA_VALOR_NIHIL);

        filius = materia_nodus_creare(piscina, (s32)9, ZEPHYRUM);
        CREDO_NON_NIHIL (filius);
        CREDO_NIHIL (filius->loci);

        imprimere("  (tres nuntii 'materia_nodus:' infra EXSPECTANTUR)\n");

        /* locus extra fines RECUSATUR */
        CREDO_FALSUM (materia_nodus_ponere(n, (i32)III,
            materia_valor_nodus(filius), MATERIA_LOCUS_NODUS));
        /* species non congruens RECUSATUR */
        CREDO_FALSUM (materia_nodus_ponere(n, ZEPHYRUM,
            materia_valor_nodus(filius), MATERIA_LOCUS_TOKEN));
        /* et nihil scriptum est */
        CREDO_AEQUALIS_S32 ((s32)n->loci[0].genus,
            (s32)MATERIA_VALOR_NIHIL);

        CREDO_VERUM (materia_nodus_ponere(n, ZEPHYRUM,
            materia_valor_nodus(filius), MATERIA_LOCUS_NODUS));
        /* scriptura DUPLEX recusatur - dominus unicus */
        CREDO_FALSUM (materia_nodus_ponere(n, ZEPHYRUM,
            materia_valor_nodus(filius), MATERIA_LOCUS_NODUS));
        CREDO_AEQUALIS_PTR (n->loci[0].datum.nodus, filius);

        CREDO_FALSUM (materia_nodus_ponere(NIHIL, ZEPHYRUM,
            materia_valor_nihil(), MATERIA_LOCUS_NODUS));
    }


    /* ========================================================
     * PROBARE: appendere et liberi
     * ======================================================== */

    {
        MateriaNodus* n;
        MateriaNodus* a;
        MateriaNodus* b;
        MateriaToken* t;
                 Xar* liberi;

        imprimere("\n--- Probans appendere et liberos ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)II);
        a = materia_nodus_creare(piscina, (s32)I, ZEPHYRUM);
        b = materia_nodus_creare(piscina, (s32)II, ZEPHYRUM);
        t = materia_token_creare(piscina, &FORMA, ZEPHYRUM, valor,
            (s32)5, (i32)2, (i32)8, ZEPHYRUM);
        CREDO_NON_NIHIL (n); CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b); CREDO_NON_NIHIL (t);

        /* locus 0 = nodus unus; locus 1 = lista */
        CREDO_VERUM (materia_nodus_ponere(n, ZEPHYRUM,
            materia_valor_nodus(a), MATERIA_LOCUS_NODUS));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, (i32)I,
            materia_valor_nodus(b), MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, (i32)I,
            materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(n->loci[1]),
            (i32)II);

        imprimere("  (unus nuntius 'materia_nodus:' infra EXSPECTATUR)\n");
        /* appendere in speciem non-listam RECUSATUR */
        CREDO_FALSUM (materia_nodus_appendere(piscina, n, (i32)I,
            materia_valor_nodus(b), MATERIA_LOCUS_NODUS));

        /* liberi: nodus loci + nodi listae; lexema NON numeratur */
        liberi = materia_nodus_liberi(piscina, n);
        CREDO_NON_NIHIL (liberi);
        CREDO_AEQUALIS_I32 (xar_numerus(liberi), (i32)II);
        CREDO_AEQUALIS_PTR (*(MateriaNodus**)xar_obtinere(liberi,
            ZEPHYRUM), a);
        CREDO_AEQUALIS_PTR (*(MateriaNodus**)xar_obtinere(liberi,
            (i32)I), b);

        /* nodus NIHIL: Xar vacuus, non NIHIL */
        liberi = materia_nodus_liberi(piscina, NIHIL);
        CREDO_NON_NIHIL (liberi);
        CREDO_AEQUALIS_I32 (xar_numerus(liberi), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: uncus sedis
     * ======================================================== */

    {
             MateriaToken* t;
             MateriaSedes  sedes;
        MateriaOrigoUncus  uncus;
                      s32  fictum = (s32)1234;

        imprimere("\n--- Probans uncum sedis ---\n");

        t = materia_token_creare(piscina, &FORMA, ZEPHYRUM, valor,
            (s32)77, (i32)5, (i32)9, (s32)2);
        CREDO_NON_NIHIL (t);

        /* Sine unco: campi lexematis. CSS/HTML hac via eunt, et
         * hoc RECTUM est - lexema origo sua est. */
        materia_sedes_tokeni(NIHIL, t, &sedes);
        CREDO_AEQUALIS_S32 (sedes.byte_offset, (s32)77);
        CREDO_AEQUALIS_I32 (sedes.linea, (i32)5);
        CREDO_AEQUALIS_I32 (sedes.columna, (i32)9);
        CREDO_AEQUALIS_S32 (sedes.fons_index, (s32)2);
        CREDO_VERUM (sedes.est_fons);

        /* Uncus cum campo NIHIL = idem ac uncus absens */
        uncus.datum           = NIHIL;
        uncus.sedes_quaerere  = NIHIL;
        materia_sedes_tokeni(&uncus, t, &sedes);
        CREDO_AEQUALIS_S32 (sedes.byte_offset, (s32)77);
        CREDO_VERUM (sedes.est_fons);

        /* Cum unco: quod FRONS dicit, non quod lexema fert.
         * C89 hac via catenam originis ad radicem ambulat. */
        uncus.datum           = &fictum;
        uncus.sedes_quaerere  = _uncus_fictus;
        materia_sedes_tokeni(&uncus, t, &sedes);
        CREDO_AEQUALIS_S32 (sedes.byte_offset, (s32)1234);
        CREDO_AEQUALIS_I32 (sedes.linea, (i32)999);
        CREDO_AEQUALIS_S32 (sedes.fons_index, (s32)3);
        CREDO_FALSUM (sedes.est_fons);

        /* Lexema NIHIL: sedes IMPLETUR (inscibilis), non relinquitur */
        sedes.byte_offset = (s32)55;
        materia_sedes_tokeni(&uncus, NIHIL, &sedes);
        CREDO_AEQUALIS_S32 (sedes.byte_offset, (s32)-I);
        CREDO_FALSUM (sedes.est_fons);

        materia_sedes_tokeni(&uncus, t, NIHIL);   /* non ruat */
    }


    /* ========================================================
     * PROBARE: verba mutationis - permutare, reponere (gradus V)
     * ======================================================== */

    {
        MateriaNodus* pater;
        MateriaNodus* filii[III];
        MateriaValor  vetus;
                 i32  ordo[III];
                 i32  k;

        imprimere("\n--- Probans verba mutationis (permutare, reponere) ---\n");

        pater = materia_nodus_creare(piscina, (s32)7, (i32)II);
        CREDO_NON_NIHIL (pater);
        per (k = ZEPHYRUM; k < (i32)III; k++)
        {
            filii[k] = materia_nodus_creare(piscina, (s32)(10 + k), I);
            CREDO_NON_NIHIL (filii[k]);
            CREDO_VERUM (materia_nodus_appendere(piscina, pater,
                ZEPHYRUM,
                materia_valor_nodus(filii[k]),
                MATERIA_LOCUS_LISTA_NODUS));
        }
        vetus = pater->loci[ZEPHYRUM];
        /* permutatio recta: [2, 0, 1] */
        ordo[ZEPHYRUM]  = (i32)II;
        ordo[I]         = ZEPHYRUM;
        ordo[II]        = I;
        CREDO_VERUM (materia_nodus_lista_permutare(piscina, pater,
            ZEPHYRUM,
            ordo, (i32)III));
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pater->loci[ZEPHYRUM], ZEPHYRUM)->datum.nodus->genus,
            (s32)12);
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pater->loci[ZEPHYRUM], I)->datum.nodus->genus, (s32)10);
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pater->loci[ZEPHYRUM], (i32)II)->datum.nodus->genus,
            (s32)11);
        /* prospectus vetus intactus (repositorium novum) */
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(vetus,
            ZEPHYRUM)->datum.nodus->genus, (s32)10);
        CREDO_INAEQUALITAS_PTR (vetus.datum.lista.xar,
                                pater->loci[ZEPHYRUM].datum.lista.xar);
        /* refusiones: index duplex, extra fines, mensura falsa, locus
         * non lista - nihil mutatum */
        ordo[I] = (i32)II;
        CREDO_FALSUM (materia_nodus_lista_permutare(piscina, pater,
            ZEPHYRUM,
            ordo, (i32)III));
        ordo[I] = (i32)III;
        CREDO_FALSUM (materia_nodus_lista_permutare(piscina, pater,
            ZEPHYRUM,
            ordo, (i32)III));
        ordo[I] = ZEPHYRUM;
        CREDO_FALSUM (materia_nodus_lista_permutare(piscina, pater,
            ZEPHYRUM,
            ordo, (i32)II));
        CREDO_FALSUM (materia_nodus_lista_permutare(piscina, pater, I,
            ordo,
            (i32)III));
        CREDO_AEQUALIS_S32 (materia_valor_lista_obtinere(
            pater->loci[ZEPHYRUM], ZEPHYRUM)->datum.nodus->genus,
            (s32)12);
        /* reponere: locus scriptus tantum, species congruens */
        CREDO_VERUM (materia_nodus_ponere(pater, I, materia_valor_index(
            (s32)5), MATERIA_LOCUS_INDEX));
        CREDO_VERUM (materia_nodus_reponere(pater, I,
            materia_valor_index(
            (s32)6), MATERIA_LOCUS_INDEX));
        CREDO_AEQUALIS_S32 (pater->loci[I].datum.index, (s32)6);
        CREDO_FALSUM (materia_nodus_reponere(pater, I,
            materia_valor_nodus(filii[ZEPHYRUM]), MATERIA_LOCUS_INDEX));
        CREDO_FALSUM (materia_nodus_reponere(filii[ZEPHYRUM], ZEPHYRUM,
            materia_valor_index((s32)1), MATERIA_LOCUS_INDEX));
        CREDO_AEQUALIS_S32 (pater->loci[I].datum.index, (s32)6);
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
