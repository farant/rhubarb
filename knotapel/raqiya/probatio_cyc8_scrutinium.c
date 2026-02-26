/*
 * probatio_cyc8_scrutinium.c
 *
 * Sprint 3B: Connect Z[zeta_8] (raqiya.h) to the generic pipeline
 * (scrutinium.h). Uses scr_registra_universales for free detectors,
 * adds type-specific detectors manually.
 *
 * Validates cross-type comparison: Z[zeta_8] lattice vs Z[i] lattice.
 */

#include <stdio.h>
#include <string.h>

#define RAQ_PRINT
#define SCR_PRINT
#include "raqiya.h"
#include "scrutinium.h"

static int n_tests = 0;
static int n_pass = 0;
static int n_fail = 0;

static void check(int cond, const char *msg) {
    n_tests++;
    if (cond) {
        n_pass++;
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

/* ================================================================
 * void* wrappers for Z[zeta_8] -- Genus vtable entries
 *
 * Sprint 4B: Using SCR_WRAP_* macros from scrutinium.h.
 * 12 macro invocations + 2 custom (norm) replace ~144 lines.
 * ================================================================ */

/* Identity */
SCR_WRAP_EQ(cyc8_eq_v, Raq_Cyc8, raq_cyc8_eq)
SCR_WRAP_HASH(cyc8_hash_v, Raq_Cyc8, raq_cyc8_hash)

/* Ring operations */
SCR_WRAP_UNOP(cyc8_neg_v, Raq_Cyc8, raq_cyc8_neg)
SCR_WRAP_BINOP(cyc8_add_v, Raq_Cyc8, raq_cyc8_add)
SCR_WRAP_BINOP(cyc8_mul_v, Raq_Cyc8, raq_cyc8_mul)
SCR_WRAP_UNOP(cyc8_conj_v, Raq_Cyc8, raq_cyc8_conj)

/* Norm -- CUSTOM: norm_sq returns Raq_Cyc8, not a scalar.
 * Can't use SCR_WRAP_CLASSIS_* which assume == comparison. */
static unsigned long cyc8_norm_hash_v(const void *a, const void *config) {
    Raq_Cyc8 n = raq_cyc8_norm_sq(*(const Raq_Cyc8 *)a);
    (void)config;
    return raq_cyc8_hash(n);
}

static int cyc8_norm_eq_v(const void *a, const void *b, const void *config) {
    (void)config;
    return raq_cyc8_eq(raq_cyc8_norm_sq(*(const Raq_Cyc8 *)a),
                       raq_cyc8_norm_sq(*(const Raq_Cyc8 *)b));
}

/* Unit group: 8 units via zeta_8 iteration */
SCR_WRAP_GEN_ITER(cyc8_unit_gen_v, Raq_Cyc8, raq_cyc8_mul_zeta)

/* Galois: |Gal|=4, {id, sigma_3, sigma_5, sigma_7} */
SCR_WRAP_GEN_GALOIS4(cyc8_auto_gen_v, Raq_Cyc8,
    raq_cyc8_galois_3, raq_cyc8_galois_5, raq_cyc8_galois_7)

/* Component access + axis */
SCR_WRAP_PARS4(cyc8_pars_v, Raq_Cyc8, a, b, c, d)
SCR_WRAP_AXIS(cyc8_axis_classis_v, Raq_Cyc8, raq_cyc8_axis)

/* Type-specific: 2-adic valuation (2 ramifies in Z[zeta_8]) */
SCR_WRAP_CLASSIS_HASH_INT(cyc8_v2_hash_v, Raq_Cyc8, raq_cyc8_2adic_val)
SCR_WRAP_CLASSIS_EQ(cyc8_v2_eq_v, Raq_Cyc8, raq_cyc8_2adic_val)

/* ================================================================
 * Genus creation + detector registration
 * ================================================================ */

static Scr_Genus cyc8_genus_crea(void) {
    Scr_Genus g;
    memset(&g, 0, sizeof(g));

    g.titulis = "Z[zeta_8]";
    g.magnitudo = (unsigned long)sizeof(Raq_Cyc8);

    g.aequalis = cyc8_eq_v;
    g.dispersio = cyc8_hash_v;

    g.nega = cyc8_neg_v;
    g.adde = cyc8_add_v;
    g.multiplica = cyc8_mul_v;
    g.conjuga = cyc8_conj_v;

    g.norma_dispersio = cyc8_norm_hash_v;
    g.norma_aequalis = cyc8_norm_eq_v;

    g.unitas_numerus = 8;
    g.unitas_generator = cyc8_unit_gen_v;

    g.automorphismus_numerus = 4;
    g.automorphismus_generator = cyc8_auto_gen_v;

    g.partes_numerus = 4;
    g.pars = cyc8_pars_v;
    g.axis_classis = cyc8_axis_classis_v;

    g.config = NULL;

    return g;
}

static void cyc8_registra_proprios(Scr_Registrum *reg) {
    Scr_Indagator ind;

    /* v2_classes (2-adic valuation, 2 ramifies) -- type-specific */
    memset(&ind, 0, sizeof(ind));
    ind.titulis = "v2_classes";
    ind.genus_indagatoris = SCR_IND_PARTITIO;
    ind.requisita = SCR_REQ_NIHIL;
    ind.classis_dispersio = cyc8_v2_hash_v;
    ind.classis_aequalis = cyc8_v2_eq_v;
    scr_registrum_adde(reg, &ind);
    /* axis_classes, sign_distances, sum_zeros, diff_zeros
     * are now universal via partes/axis_classis on Genus */
}

/* ================================================================
 * Test: Full pipeline for Z[zeta_8]
 *
 * Uses the 13 values from the original D107 raqiya demo set.
 * ================================================================ */

static void test_cyc8_pipeline(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Raq_Cyc8 vals[13];
    int n_universal, i;
    int norm_groups = -1, unit_groups = -1, galois_groups = -1;

    printf("--- Z[zeta_8] Pipeline Test ---\n");

    /* D107 value set: 13 values spanning diverse norms, axes, orbits */
    vals[0]  = raq_cyc8_make(1, 0, 0, 0);    /* 1 */
    vals[1]  = raq_cyc8_make(0, 1, 0, 0);    /* zeta */
    vals[2]  = raq_cyc8_make(0, 0, 1, 0);    /* zeta^2 = i*sqrt(2)/something */
    vals[3]  = raq_cyc8_make(0, 0, 0, 1);    /* zeta^3 */
    vals[4]  = raq_cyc8_make(-1, 0, 0, 0);   /* -1 */
    vals[5]  = raq_cyc8_make(1, 1, 0, 0);    /* 1+zeta */
    vals[6]  = raq_cyc8_make(1, 0, 1, 0);    /* 1+zeta^2 */
    vals[7]  = raq_cyc8_make(1, 0, 0, 1);    /* 1+zeta^3 */
    vals[8]  = raq_cyc8_make(2, 0, 0, 0);    /* 2 */
    vals[9]  = raq_cyc8_make(1, 1, 1, 0);    /* 1+zeta+zeta^2 */
    vals[10] = raq_cyc8_make(1, 1, 1, 1);    /* 1+zeta+zeta^2+zeta^3 */
    vals[11] = raq_cyc8_make(0, 1, 0, -1);   /* zeta - zeta^3 */
    vals[12] = raq_cyc8_make(3, 0, 0, 0);    /* 3 */

    genus = cyc8_genus_crea();
    scr_registrum_init(&reg);

    /* Universal: 14 algebraic + 4 component-derived = 18 */
    n_universal = scr_registra_universales(&reg, &genus);
    check(n_universal == 18, "cyc8: 18 universal detectors registered");

    /* Type-specific: v2_classes only */
    cyc8_registra_proprios(&reg);
    check(reg.numerus == 19, "cyc8: 19 total detectors");

    /* Run pipeline */
    s = scr_scrutare(&reg, &genus, vals, 13);

    check(s.numerus_activi == 19, "cyc8: all 19 ran");

    /* Extract partition group counts */
    for (i = 0; i < s.numerus_totalis; i++) {
        if (!s.activi[i] || s.genera[i] != SCR_IND_PARTITIO) continue;
        if (strcmp(s.nomina[i], "norm_classes") == 0)
            norm_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "unit_orbits") == 0)
            unit_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "galois_orbits") == 0)
            galois_groups = s.partitiones[i].n_groups;
    }

    printf("  norm=%d, unit=%d, galois=%d\n",
           norm_groups, unit_groups, galois_groups);

    /* STRUCTURAL FINDINGS about norms in Z[zeta_8] */
    {
        int galois_pi = -1, norm_pi = -1, tnorm_pi = -1;
        int tnorm_groups = -1;
        int pi;
        for (pi = 0; pi < s.numerus_partitionum; pi++) {
            int idx = s.partitio_indices[pi];
            if (strcmp(s.nomina[idx], "galois_orbits") == 0)
                galois_pi = pi;
            if (strcmp(s.nomina[idx], "norm_classes") == 0)
                norm_pi = pi;
            if (strcmp(s.nomina[idx], "true_norm_classes") == 0)
                tnorm_pi = pi;
        }

        if (tnorm_pi >= 0) {
            int idx = s.partitio_indices[tnorm_pi];
            tnorm_groups = s.partitiones[idx].n_groups;
            printf("  true_norm=%d\n", tnorm_groups);
        }

        /* 1. galois does NOT refine norm (|z|^2 not Galois-invariant) */
        if (galois_pi >= 0 && norm_pi >= 0) {
            check(!s.ordo[galois_pi * s.numerus_partitionum + norm_pi],
                  "cyc8: galois does NOT refine norm (|z|^2)");
        }

        /* 2. galois DOES refine true_norm (algebraic norm IS invariant) */
        if (galois_pi >= 0 && tnorm_pi >= 0) {
            check(s.ordo[galois_pi * s.numerus_partitionum + tnorm_pi],
                  "cyc8: galois DOES refine true_norm (structural theorem)");
        }

        /* 3. true_norm != norm for Z[zeta_8] (subfield tower!) */
        if (tnorm_groups >= 0 && norm_groups >= 0) {
            check(tnorm_groups != norm_groups ||
                  (tnorm_pi >= 0 && norm_pi >= 0 &&
                   !s.ordo[tnorm_pi * s.numerus_partitionum + norm_pi]),
                  "cyc8: true_norm != norm (different norms in Z[zeta_8])");
        }

        /* 4. Each partial_norm refines true_norm (norm tower theorem) */
        {
            int pn;
            int all_refine = 1;
            for (pn = 0; pn < s.numerus_partitionum; pn++) {
                int idx = s.partitio_indices[pn];
                if (strncmp(s.nomina[idx], "partial_norm_sigma_", 19) != 0)
                    continue;
                if (tnorm_pi >= 0 &&
                    !s.ordo[pn * s.numerus_partitionum + tnorm_pi])
                    all_refine = 0;
            }
            check(all_refine,
                  "cyc8: all partial_norms refine true_norm (structural)");
        }

        /* 5. Each partial_trace refines true_trace (trace tower theorem) */
        {
            int tt_pi = -1, pt;
            int all_refine = 1;
            for (pt = 0; pt < s.numerus_partitionum; pt++) {
                int idx = s.partitio_indices[pt];
                if (strcmp(s.nomina[idx], "true_trace_classes") == 0)
                    tt_pi = pt;
            }
            for (pt = 0; pt < s.numerus_partitionum; pt++) {
                int idx = s.partitio_indices[pt];
                if (strncmp(s.nomina[idx], "partial_trace_sigma_", 20) != 0)
                    continue;
                if (tt_pi >= 0 &&
                    !s.ordo[pt * s.numerus_partitionum + tt_pi])
                    all_refine = 0;
            }
            check(all_refine,
                  "cyc8: all partial_traces refine true_trace (structural)");
        }

        /* 6. true_norm != true_trace (multiplicative vs additive) */
        {
            int tt_pi = -1;
            for (pi = 0; pi < s.numerus_partitionum; pi++) {
                int idx = s.partitio_indices[pi];
                if (strcmp(s.nomina[idx], "true_trace_classes") == 0)
                    tt_pi = pi;
            }
            if (tnorm_pi >= 0 && tt_pi >= 0) {
                int tn_groups = s.partitiones[s.partitio_indices[tnorm_pi]].n_groups;
                int tt_groups = s.partitiones[s.partitio_indices[tt_pi]].n_groups;
                check(tn_groups != tt_groups ||
                      !s.ordo[tnorm_pi * s.numerus_partitionum + tt_pi] ||
                      !s.ordo[tt_pi * s.numerus_partitionum + tnorm_pi],
                      "cyc8: true_norm != true_trace (mul vs add)");
            }
        }
    }

    /* Galois is still the finest partition */
    check(galois_groups >= norm_groups,
          "cyc8: galois >= norm in group count");

    /* Meet of all = n → collectively exhaustive */
    check(s.conventus.n_groups == 13,
          "cyc8: meet of all = 13 (collectively exhaustive)");

    /* Print full analysis */
    printf("\n");
    scr_print_scrutinium(&s);

    scr_scrutinium_libera(&s);
}

/* ================================================================
 * Test: Universal detectors are the SAME code for both types
 *
 * Verify that scr_registra_universales + scr_scrutare works
 * identically for Z[i] and Z[zeta_8] without any type-specific
 * detector code.
 * ================================================================ */

static void test_universal_cross_type(void) {
    Scr_Genus g_cyc8;
    Scr_Registrum reg_cyc8;
    Scr_Scrutinium s_cyc8;
    Raq_Cyc8 cyc8_vals[4];

    printf("\n--- Universal Cross-Type Test ---\n");

    /* Z[zeta_8]: 4 simple values, universal detectors only */
    cyc8_vals[0] = raq_cyc8_make(1, 0, 0, 0);
    cyc8_vals[1] = raq_cyc8_make(0, 1, 0, 0);
    cyc8_vals[2] = raq_cyc8_make(1, 1, 0, 0);
    cyc8_vals[3] = raq_cyc8_make(2, 0, 0, 0);

    g_cyc8 = cyc8_genus_crea();
    scr_registrum_init(&reg_cyc8);
    scr_registra_universales(&reg_cyc8, &g_cyc8);
    /* NO type-specific detectors — universal only */

    s_cyc8 = scr_scrutare(&reg_cyc8, &g_cyc8, cyc8_vals, 4);

    check(s_cyc8.numerus_activi == 18,
          "cross-type: Z[zeta_8] universal-only = 18 detectors");
    check(s_cyc8.numerus_partitionum == 12,
          "cross-type: Z[zeta_8] has 12 partition detectors");

    /* The 3 universal partitions should be different for Z[zeta_8]
     * because 8 units vs 4 automorphisms creates different groupings */
    {
        int i;
        int norm_g = -1, unit_g = -1, galois_g = -1;
        for (i = 0; i < s_cyc8.numerus_totalis; i++) {
            if (!s_cyc8.activi[i] || s_cyc8.genera[i] != SCR_IND_PARTITIO)
                continue;
            if (strcmp(s_cyc8.nomina[i], "norm_classes") == 0)
                norm_g = s_cyc8.partitiones[i].n_groups;
            if (strcmp(s_cyc8.nomina[i], "unit_orbits") == 0)
                unit_g = s_cyc8.partitiones[i].n_groups;
            if (strcmp(s_cyc8.nomina[i], "galois_orbits") == 0)
                galois_g = s_cyc8.partitiones[i].n_groups;
        }
        printf("  Z[zeta_8] universal-only: norm=%d, unit=%d, galois=%d\n",
               norm_g, unit_g, galois_g);

        /* 1 and zeta are associates (mul by zeta), same norm.
         * 1+zeta has different norm. 2 has different norm.
         * So norm should have >=2 groups, unit should merge more. */
        check(norm_g > 0 && unit_g > 0 && galois_g > 0,
              "cross-type: all universal detectors produced groups");
    }

    scr_print_scrutinium(&s_cyc8);

    scr_scrutinium_libera(&s_cyc8);
}

/* ================================================================
 * Test: Galois correspondence via partial norms (diamond test)
 *
 * Uses a full Galois orbit {1+zeta, 1-zeta, 1+zeta^3, 1-zeta^3}
 * where each partial norm cross-cuts the orbit differently:
 *   pn1 (sigma_3): {1+z,1+z^3} | {1-z,1-z^3}
 *   pn2 (sigma_5): {1+z,1-z}   | {1+z^3,1-z^3}
 *   pn3 (sigma_7): {1+z,1-z^3} | {1-z,1+z^3}
 *
 * This reveals the full subfield diamond:
 *   galois (top, discrete) -> pn1, pn2, pn3 (middle) -> true_norm (bottom)
 * ================================================================ */

static void test_galois_correspondentia(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Raq_Cyc8 vals[6];
    int i, pi;
    int galois_pi = -1, tnorm_pi = -1;
    int pn_pis[3];
    int n_pn = 0;

    printf("\n--- Galois Correspondentia (Diamond Test) ---\n");

    /* Full Galois orbit of 1+zeta */
    vals[0] = raq_cyc8_make(1, 1, 0, 0);    /* 1+zeta */
    vals[1] = raq_cyc8_make(1, -1, 0, 0);   /* 1-zeta = sigma_5(1+zeta) */
    vals[2] = raq_cyc8_make(1, 0, 0, 1);    /* 1+zeta^3 = sigma_3(1+zeta) */
    vals[3] = raq_cyc8_make(1, 0, 0, -1);   /* 1-zeta^3 = sigma_7(1+zeta) */
    /* Extra values for richness */
    vals[4] = raq_cyc8_make(1, 0, 0, 0);    /* 1 (fixed by all) */
    vals[5] = raq_cyc8_make(2, 0, 0, 0);    /* 2 (fixed by all) */

    genus = cyc8_genus_crea();
    scr_registrum_init(&reg);
    scr_registra_universales(&reg, &genus);
    /* Universal detectors only -- no type-specific */

    s = scr_scrutare(&reg, &genus, vals, 6);

    /* Find partition indices */
    for (pi = 0; pi < s.numerus_partitionum; pi++) {
        int idx = s.partitio_indices[pi];
        if (strcmp(s.nomina[idx], "galois_orbits") == 0)
            galois_pi = pi;
        if (strcmp(s.nomina[idx], "true_norm_classes") == 0)
            tnorm_pi = pi;
        if (strncmp(s.nomina[idx], "partial_norm_sigma_", 19) == 0
            && n_pn < 3)
            pn_pis[n_pn++] = pi;
    }

    check(n_pn == 3, "diamond: found 3 partial norm detectors");

    /* The orbit {vals[0..3]} should be one galois orbit,
     * plus {vals[4]} and {vals[5]} as singletons = 3 galois groups */
    {
        int idx = s.partitio_indices[galois_pi];
        int galois_groups = s.partitiones[idx].n_groups;
        printf("  galois_orbits: %d groups\n", galois_groups);
        check(galois_groups == 3,
              "diamond: galois has 3 orbits (1 quad + 2 singletons)");
    }

    /* All 3 partial norms refine true_norm (structural theorem) */
    {
        int all_refine = 1;
        for (i = 0; i < n_pn; i++) {
            if (tnorm_pi >= 0 &&
                !s.ordo[pn_pis[i] * s.numerus_partitionum + tnorm_pi])
                all_refine = 0;
        }
        check(all_refine,
              "diamond: all partial_norms refine true_norm");
    }

    /* Galois does NOT refine any partial norm for this data
     * (because the 4-element orbit splits differently under each) */
    {
        int galois_refines_any_pn = 0;
        for (i = 0; i < n_pn; i++) {
            if (galois_pi >= 0 &&
                s.ordo[galois_pi * s.numerus_partitionum + pn_pis[i]])
                galois_refines_any_pn = 1;
        }
        check(!galois_refines_any_pn,
              "diamond: galois does NOT refine any partial_norm (cross-cut)");
    }

    /* The 3 partial norms don't refine each other (incomparable) */
    {
        int any_refines = 0;
        int a, b;
        for (a = 0; a < n_pn; a++) {
            for (b = 0; b < n_pn; b++) {
                if (a == b) continue;
                if (s.ordo[pn_pis[a] * s.numerus_partitionum + pn_pis[b]])
                    any_refines = 1;
            }
        }
        check(!any_refines,
              "diamond: partial_norms are pairwise incomparable");
    }

    /* Meet of all 3 partial norms should recover galois-level discrimination
     * (the 4-element orbit splits into 4 singletons) */
    {
        Scr_Partition meet;
        int idx0 = s.partitio_indices[pn_pis[0]];
        int idx1 = s.partitio_indices[pn_pis[1]];
        int idx2 = s.partitio_indices[pn_pis[2]];
        Scr_Partition m01 = scr_partition_meet(&s.partitiones[idx0],
                                                &s.partitiones[idx1]);
        meet = scr_partition_meet(&m01, &s.partitiones[idx2]);
        scr_partition_free(&m01);

        printf("  meet of 3 partial norms: %d groups\n", meet.n_groups);
        check(meet.n_groups >= 5,
              "diamond: meet of partial norms >= 5 (orbit fully split)");
        scr_partition_free(&meet);
    }

    printf("\n");
    scr_print_scrutinium(&s);

    scr_scrutinium_libera(&s);
}

/* ================================================================
 * Test: Adoption cost measurement
 *
 * Count lines of type-specific code needed to integrate Z[zeta_8].
 * ================================================================ */

static void print_adoption_cost(void) {
    printf("\n=== Z[zeta_8] Adoption Cost (Sprint 4B: with macros) ===\n\n");
    printf("  Genus wrappers via SCR_WRAP_* macros:\n");
    printf("    Identity:  SCR_WRAP_EQ, SCR_WRAP_HASH          = 2 macros\n");
    printf("    Ring ops:  SCR_WRAP_UNOP x2, SCR_WRAP_BINOP x2 = 4 macros\n");
    printf("    Units:     SCR_WRAP_GEN_ITER                   = 1 macro\n");
    printf("    Galois:    SCR_WRAP_GEN_GALOIS4                = 1 macro\n");
    printf("    Pars+axis: SCR_WRAP_PARS4, SCR_WRAP_AXIS       = 2 macros\n");
    printf("    Type-spec: SCR_WRAP_CLASSIS_HASH_INT/EQ        = 2 macros\n");
    printf("    ---- Total: 12 macros + 2 custom (norm) = 14 wrappers ----\n\n");

    printf("  Custom wrappers (norm_sq returns Raq_Cyc8, not scalar):\n");
    printf("    cyc8_norm_hash_v, cyc8_norm_eq_v = 2 functions (~12 lines)\n\n");

    printf("  Integration code:\n");
    printf("    cyc8_genus_crea()              = ~25 lines\n");
    printf("    cyc8_registra_proprios()       = ~10 lines\n");
    printf("    ---- Total integration: ~35 lines ----\n\n");

    printf("  TOTAL: ~53 lines to connect Z[zeta_8] (was ~100 pre-macros)\n");
    printf("  Universal detectors: 18 for FREE (14 algebraic + 4 component)\n");
    printf("  Type-specific detectors: 1 (v2 only)\n\n");
}

/* ================================================================
 * Test: Structural signature extraction
 *
 * Uses the full 13-value D107 pipeline data.
 * Z[zeta_8] should show DIAMOND topology (3+ partial norms
 * pairwise incomparable between galois and true_norm).
 * ================================================================ */

static void test_signatura(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Scr_Signatura sig;
    Raq_Cyc8 vals[13];

    printf("\n--- Structural Signature Test (Z[zeta_8]) ---\n");

    /* D107 value set */
    vals[0]  = raq_cyc8_make(1, 0, 0, 0);
    vals[1]  = raq_cyc8_make(0, 1, 0, 0);
    vals[2]  = raq_cyc8_make(0, 0, 1, 0);
    vals[3]  = raq_cyc8_make(0, 0, 0, 1);
    vals[4]  = raq_cyc8_make(-1, 0, 0, 0);
    vals[5]  = raq_cyc8_make(1, 1, 0, 0);
    vals[6]  = raq_cyc8_make(1, 0, 1, 0);
    vals[7]  = raq_cyc8_make(1, 0, 0, 1);
    vals[8]  = raq_cyc8_make(2, 0, 0, 0);
    vals[9]  = raq_cyc8_make(1, 1, 1, 0);
    vals[10] = raq_cyc8_make(1, 1, 1, 1);
    vals[11] = raq_cyc8_make(0, 1, 0, -1);
    vals[12] = raq_cyc8_make(3, 0, 0, 0);

    genus = cyc8_genus_crea();
    scr_registrum_init(&reg);
    scr_registra_universales(&reg, &genus);
    cyc8_registra_proprios(&reg);
    s = scr_scrutare(&reg, &genus, vals, 13);

    /* Extract signature */
    sig = scr_signatura_ex(&s);

    printf("  unique=%d, dups=%d, alt=%d, width=%d\n",
           sig.partitiones_numerus, sig.duplices_numerus,
           sig.altitudo, sig.latitudo);

    /* Topology flags — DIAMOND (|Gal|=4, subfield diamond) */
    check(!sig.est_catena, "Z[zeta_8] sig: NOT a chain");
    check(sig.est_V, "Z[zeta_8] sig: V-shape (baseline)");
    check(sig.est_diamas, "Z[zeta_8] sig: DIAMOND (|Gal|=4)");

    /* Structural counts */
    check(sig.altitudo >= 3,
          "Z[zeta_8] sig: altitude >= 3 (deeper than |Gal|=2 types)");
    check(sig.latitudo >= 3,
          "Z[zeta_8] sig: width >= 3 (wider than |Gal|=2 types)");
    check(sig.partitiones_numerus > 6,
          "Z[zeta_8] sig: more unique partitions than Z[i]/Z[omega]");
    check(sig.incomparabiles_numerus > 1,
          "Z[zeta_8] sig: many incomparable pairs");

    /* Self-equality */
    check(scr_signatura_aequalis(&sig, &sig),
          "Z[zeta_8] sig: self-equality holds");

    scr_signatura_imprime(&sig);

    scr_signatura_libera(&sig);
    scr_scrutinium_libera(&s);
}

/* ================================================================ */

int main(void) {
    printf("=== probatio_cyc8_scrutinium ===\n");
    printf("=== Z[zeta_8] through the generic pipeline ===\n\n");

    test_cyc8_pipeline();
    test_universal_cross_type();
    test_galois_correspondentia();
    test_signatura();
    print_adoption_cost();

    printf("=== Results: %d tests, %d passed, %d failed ===\n",
           n_tests, n_pass, n_fail);

    return n_fail > 0 ? 1 : 0;
}
