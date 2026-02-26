/*
 * probatio_eisenstein_scrutinium.c
 *
 * Sprint 3D: Connect Z[omega] (eisenstein.h) to the generic pipeline
 * (scrutinium.h). Third algebraic type through the framework --
 * the "adoption cost" validation.
 *
 * Z[omega]: Eisenstein integers, omega = e^{2*pi*i/3}.
 * |Gal| = 2 (same as Z[i]), but 6 units (vs 4).
 * Hexagonal geometry vs Z[i]'s square lattice.
 * 3 ramifies (vs 2 for Z[i]).
 */

#include <stdio.h>
#include <string.h>

#define EIS_PRINT
#define SCR_PRINT
#include "eisenstein.h"
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
 * void* wrappers for Z[omega] -- Genus vtable entries
 *
 * Sprint 4B: Using SCR_WRAP_* macros from scrutinium.h.
 * 14 macro invocations replace ~100 lines of hand-written wrappers.
 * ================================================================ */

/* Identity */
SCR_WRAP_EQ(eis_eq_v, Eis_Z, eis_eq)
SCR_WRAP_HASH(eis_hash_v, Eis_Z, eis_hash)

/* Ring operations */
SCR_WRAP_UNOP(eis_neg_v, Eis_Z, eis_neg)
SCR_WRAP_BINOP(eis_add_v, Eis_Z, eis_add)
SCR_WRAP_BINOP(eis_mul_v, Eis_Z, eis_mul)
SCR_WRAP_UNOP(eis_conj_v, Eis_Z, eis_conj)

/* Norm classifier */
SCR_WRAP_CLASSIS_HASH_LONG(eis_norm_hash_v, Eis_Z, eis_norm)
SCR_WRAP_CLASSIS_EQ(eis_norm_eq_v, Eis_Z, eis_norm)

/* Unit group: 6 units via -omega iteration */
SCR_WRAP_GEN_ITER(eis_unit_gen_v, Eis_Z, eis_mul_neg_omega)

/* Galois: |Gal|=2, {id, conj} */
SCR_WRAP_GEN_GALOIS2(eis_auto_gen_v, Eis_Z, eis_conj)

/* Component access + axis */
SCR_WRAP_PARS2(eis_pars_v, Eis_Z, a, b)
SCR_WRAP_AXIS(eis_axis_classis_v, Eis_Z, eis_axis)

/* Type-specific: 3-adic norm valuation (3 ramifies in Z[omega]) */
SCR_WRAP_CLASSIS_HASH_INT(eis_v3norm_hash_v, Eis_Z, eis_norm_3adic_val)
SCR_WRAP_CLASSIS_EQ(eis_v3norm_eq_v, Eis_Z, eis_norm_3adic_val)

/* ================================================================
 * Genus creation + detector registration
 * ================================================================ */

static Scr_Genus eis_genus_crea(void) {
    Scr_Genus g;
    memset(&g, 0, sizeof(g));

    g.titulis = "Z[omega]";
    g.magnitudo = (unsigned long)sizeof(Eis_Z);

    g.aequalis = eis_eq_v;
    g.dispersio = eis_hash_v;

    g.nega = eis_neg_v;
    g.adde = eis_add_v;
    g.multiplica = eis_mul_v;
    g.conjuga = eis_conj_v;

    g.norma_dispersio = eis_norm_hash_v;
    g.norma_aequalis = eis_norm_eq_v;

    g.unitas_numerus = 6;
    g.unitas_generator = eis_unit_gen_v;

    g.automorphismus_numerus = 2;
    g.automorphismus_generator = eis_auto_gen_v;

    g.partes_numerus = 2;
    g.pars = eis_pars_v;
    g.axis_classis = eis_axis_classis_v;

    g.config = NULL;

    return g;
}

static void eis_registra_proprios(Scr_Registrum *reg) {
    Scr_Indagator ind;

    /* v3_norm_classes (3-adic valuation, 3 ramifies) -- type-specific */
    memset(&ind, 0, sizeof(ind));
    ind.titulis = "v3_norm_classes";
    ind.genus_indagatoris = SCR_IND_PARTITIO;
    ind.requisita = SCR_REQ_NIHIL;
    ind.classis_dispersio = eis_v3norm_hash_v;
    ind.classis_aequalis = eis_v3norm_eq_v;
    scr_registrum_adde(reg, &ind);
    /* axis_classes, sign_distances, sum_zeros, diff_zeros
     * are now universal via partes/axis_classis on Genus */
}

/* ================================================================
 * Test: Basic eisenstein.h arithmetic validation
 * ================================================================ */

static void test_arithmetic(void) {
    Eis_Z one, omega, omega2, z, w;

    printf("--- Eisenstein Arithmetic Validation ---\n");

    one = eis_make(1, 0);
    omega = eis_make(0, 1);
    omega2 = eis_make(-1, -1);  /* omega^2 = -1 - omega */

    /* omega^2 + omega + 1 = 0 (minimal polynomial) */
    z = eis_add(eis_add(omega2, omega), one);
    check(eis_is_zero(z), "omega^2 + omega + 1 = 0");

    /* omega^3 = 1 */
    z = eis_mul(omega, omega2);
    check(eis_eq(z, one), "omega^3 = omega * omega^2 = 1");

    /* Norm: N(1 + omega) = 1 - 1 + 1 = 1 (it's a unit: -omega^2) */
    check(eis_norm(eis_make(1, 1)) == 1, "N(1+omega) = 1 (unit!)");

    /* Norm: N(1 - omega) = 1 + 1 + 1 = 3 (ramification prime) */
    check(eis_norm(eis_make(1, -1)) == 3, "N(1-omega) = 3");

    /* Conjugation: conj(omega) = omega^2 */
    z = eis_conj(omega);
    check(eis_eq(z, omega2), "conj(omega) = omega^2");

    /* z * conj(z) = norm(z) for any z */
    w = eis_make(2, 3);
    z = eis_mul(w, eis_conj(w));
    check(z.a == eis_norm(w) && z.b == 0,
          "z * conj(z) = N(z) (norm is real integer)");

    /* Unit group: -omega generates order 6 */
    z = one;
    z = eis_mul_neg_omega(z);  /* (-omega)^1 */
    z = eis_mul_neg_omega(z);  /* (-omega)^2 */
    z = eis_mul_neg_omega(z);  /* (-omega)^3 */
    z = eis_mul_neg_omega(z);  /* (-omega)^4 */
    z = eis_mul_neg_omega(z);  /* (-omega)^5 */
    z = eis_mul_neg_omega(z);  /* (-omega)^6 = 1 */
    check(eis_eq(z, one), "(-omega)^6 = 1 (unit group order 6)");

    /* 6 distinct units */
    {
        Eis_Z units[6];
        int i, j, all_distinct = 1;
        units[0] = one;
        for (i = 1; i < 6; i++)
            units[i] = eis_mul_neg_omega(units[i-1]);
        for (i = 0; i < 6; i++)
            for (j = i+1; j < 6; j++)
                if (eis_eq(units[i], units[j])) all_distinct = 0;
        check(all_distinct, "6 distinct units via -omega generator");
    }

    /* Multiplication check: (2+omega)(1-omega) */
    z = eis_mul(eis_make(2, 1), eis_make(1, -1));
    /* (2+w)(1-w) = 2 - 2w + w - w^2 = 2 - w - (-1-w) = 2 - w + 1 + w = 3 */
    check(eis_eq(z, eis_make(3, 0)), "(2+omega)(1-omega) = 3");
}

/* ================================================================
 * Test: Full pipeline for Z[omega]
 *
 * Uses 11 values spanning diverse norms, axes, and orbits.
 * Key feature: non-associate Galois conjugates (2-omega, 3+omega)
 * which breaks the galois-refines-unit_orbits coincidence seen
 * in the Z[i] test data.
 * ================================================================ */

static void test_pipeline(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Eis_Z vals[11];
    int n_universal, i;
    int norm_groups = -1, unit_groups = -1, galois_groups = -1;
    int tnorm_groups = -1, ttrace_groups = -1;

    printf("\n--- Z[omega] Pipeline Test ---\n");

    /* 11 values: diverse norms, axes, orbits.
     * Key pair: vals[7]=(2,-1) and vals[8]=(3,1) are
     * Galois conjugates (conj(2-omega) = 3+omega) but
     * NOT associates (different unit orbits).
     * This is a hexagonal-specific phenomenon. */
    vals[0]  = eis_make(1, 0);     /* 1, norm=1, axis=0 (real) */
    vals[1]  = eis_make(0, 1);     /* omega, norm=1, axis=1 */
    vals[2]  = eis_make(2, 0);     /* 2, norm=4, axis=0 */
    vals[3]  = eis_make(1, -1);    /* 1-omega, norm=3, axis=2 */
    vals[4]  = eis_make(2, 1);     /* 2+omega, norm=3, off-axis */
    vals[5]  = eis_make(3, 0);     /* 3, norm=9, axis=0 */
    vals[6]  = eis_make(1, 1);     /* 1+omega=-omega^2, norm=1, off-axis */
    vals[7]  = eis_make(2, -1);    /* 2-omega, norm=7, off-axis */
    vals[8]  = eis_make(3, 1);     /* 3+omega, norm=7, off-axis */
    vals[9]  = eis_make(0, 2);     /* 2*omega, norm=4, axis=1 */
    vals[10] = eis_make(3, -1);    /* 3-omega, norm=13, off-axis */

    genus = eis_genus_crea();
    scr_registrum_init(&reg);

    /* Universal: 10 algebraic + 4 component-derived = 14 */
    n_universal = scr_registra_universales(&reg, &genus);
    check(n_universal == 14, "eis: 14 universal detectors registered");

    /* Type-specific: v3_norm_classes only */
    eis_registra_proprios(&reg);
    check(reg.numerus == 15, "eis: 15 total detectors");

    /* Run pipeline */
    s = scr_scrutare(&reg, &genus, vals, 11);

    check(s.numerus_activi == 15, "eis: all 15 ran");

    /* Extract partition group counts */
    for (i = 0; i < s.numerus_totalis; i++) {
        if (!s.activi[i] || s.genera[i] != SCR_IND_PARTITIO) continue;
        if (strcmp(s.nomina[i], "norm_classes") == 0)
            norm_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "unit_orbits") == 0)
            unit_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "galois_orbits") == 0)
            galois_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "true_norm_classes") == 0)
            tnorm_groups = s.partitiones[i].n_groups;
        if (strcmp(s.nomina[i], "true_trace_classes") == 0)
            ttrace_groups = s.partitiones[i].n_groups;
    }

    printf("  norm=%d, unit=%d, galois=%d, true_norm=%d, true_trace=%d\n",
           norm_groups, unit_groups, galois_groups, tnorm_groups, ttrace_groups);

    /* Group count assertions */
    check(norm_groups == 6, "eis: 6 norm classes {1,3,4,7,9,13}");
    check(unit_groups == 7,
          "eis: 7 unit orbits (6 units split norm-7 into 2 orbits)");
    check(galois_groups == 9, "eis: 9 galois orbits");
    check(tnorm_groups == 6,
          "eis: 6 true_norm classes (= norm for |Gal|=2)");
    check(ttrace_groups == 9, "eis: 9 true_trace classes");

    /* ---- STRUCTURAL FINDINGS ---- */
    {
        int galois_pi = -1, norm_pi = -1, tnorm_pi = -1;
        int unit_pi = -1, ttrace_pi = -1, pn_pi = -1, pt_pi = -1;
        int pi;

        for (pi = 0; pi < s.numerus_partitionum; pi++) {
            int idx = s.partitio_indices[pi];
            if (strcmp(s.nomina[idx], "galois_orbits") == 0)
                galois_pi = pi;
            if (strcmp(s.nomina[idx], "norm_classes") == 0)
                norm_pi = pi;
            if (strcmp(s.nomina[idx], "true_norm_classes") == 0)
                tnorm_pi = pi;
            if (strcmp(s.nomina[idx], "unit_orbits") == 0)
                unit_pi = pi;
            if (strcmp(s.nomina[idx], "true_trace_classes") == 0)
                ttrace_pi = pi;
            if (strcmp(s.nomina[idx], "partial_norm_sigma_1") == 0)
                pn_pi = pi;
            if (strcmp(s.nomina[idx], "partial_trace_sigma_1") == 0)
                pt_pi = pi;
        }

        /* 1. galois DOES refine norm
         * (structural: N(conj(z)) = conj(z)*z = N(z)) */
        if (galois_pi >= 0 && norm_pi >= 0)
            check(s.ordo[galois_pi * s.numerus_partitionum + norm_pi],
                  "eis: galois refines norm (structural)");

        /* 2. galois DOES refine true_norm
         * (universal structural theorem) */
        if (galois_pi >= 0 && tnorm_pi >= 0)
            check(s.ordo[galois_pi * s.numerus_partitionum + tnorm_pi],
                  "eis: galois refines true_norm (structural)");

        /* 3. unit_orbits refines norm
         * (structural: associates have the same norm) */
        if (unit_pi >= 0 && norm_pi >= 0)
            check(s.ordo[unit_pi * s.numerus_partitionum + norm_pi],
                  "eis: unit_orbits refines norm (structural)");

        /* 4. galois does NOT refine unit_orbits
         * (2-omega and 3+omega are conjugates but NOT associates!
         *  Hexagonal-specific: in Z[i], galois happened to refine
         *  unit_orbits on the test data, but here the geometry
         *  breaks it.) */
        if (galois_pi >= 0 && unit_pi >= 0)
            check(!s.ordo[galois_pi * s.numerus_partitionum + unit_pi],
                  "eis: galois does NOT refine unit_orbits (hexagonal!)");

        /* 5. unit does NOT refine galois either
         * (1, omega, 1+omega are associates but in different
         *  galois orbits -- incomparable partition pair) */
        if (unit_pi >= 0 && galois_pi >= 0)
            check(!s.ordo[unit_pi * s.numerus_partitionum + galois_pi],
                  "eis: unit does NOT refine galois (incomparable)");

        /* 6. galois refines true_trace
         * (structural: trace is Galois-invariant) */
        if (galois_pi >= 0 && ttrace_pi >= 0)
            check(s.ordo[galois_pi * s.numerus_partitionum + ttrace_pi],
                  "eis: galois refines true_trace (structural)");

        /* 7. true_norm = norm for Z[omega]
         * (structural for |Gal|=2: z*conj(z) = a^2-ab+b^2 = N(z)) */
        if (tnorm_pi >= 0)
            check(s.duplices[tnorm_pi],
                  "eis: true_norm is duplicate of norm (|Gal|=2)");

        /* 8. partial_norm_sigma_1 = true_norm = norm
         * (structural: only 1 non-identity automorphism,
         *  so the single partial norm IS the full norm product) */
        if (pn_pi >= 0)
            check(s.duplices[pn_pi],
                  "eis: partial_norm_sigma_1 is duplicate (= norm for |Gal|=2)");

        /* 9. partial_trace_sigma_1 = true_trace
         * (same reason: only 1 non-identity automorphism) */
        if (pt_pi >= 0)
            check(s.duplices[pt_pi],
                  "eis: partial_trace_sigma_1 is duplicate (= true_trace)");

        /* 10. No diamond: exactly 1 partial norm (vs Z[zeta_8]'s 3).
         * |Gal|=2 means only one intermediate field (Q itself),
         * so the partial norm lattice is a linear chain. */
        check(pn_pi >= 0 && pt_pi >= 0,
              "eis: exactly 1 partial norm + 1 partial trace (no diamond)");
    }

    /* Meet of all partitions = discrete (collectively exhaustive) */
    check(s.conventus.n_groups == 11,
          "eis: meet of all = 11 (collectively exhaustive)");

    /* Print full analysis */
    printf("\n");
    scr_print_scrutinium(&s);

    scr_scrutinium_libera(&s);
}

/* ================================================================
 * Test: Universal detectors only (cross-type comparison)
 * ================================================================ */

static void test_universal_only(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Eis_Z vals[4];

    printf("\n--- Z[omega] Universal-Only Test ---\n");

    vals[0] = eis_make(1, 0);   /* 1, norm=1 */
    vals[1] = eis_make(0, 1);   /* omega, norm=1 */
    vals[2] = eis_make(1, 1);   /* 1+omega (unit! norm=1) */
    vals[3] = eis_make(2, 0);   /* 2, norm=4 */

    genus = eis_genus_crea();
    scr_registrum_init(&reg);
    scr_registra_universales(&reg, &genus);
    /* NO type-specific detectors -- universal only */

    s = scr_scrutare(&reg, &genus, vals, 4);

    check(s.numerus_activi == 14,
          "universal-only: 14 detectors for Z[omega]");

    /* Compare: Z[i] also gets 14 (same |Gal|=2, same partes_numerus=2).
     * Z[zeta_8] gets 18 (|Gal|=4, partes_numerus=4). */

    {
        int k;
        int norm_g = -1, unit_g = -1, galois_g = -1;
        for (k = 0; k < s.numerus_totalis; k++) {
            if (!s.activi[k] || s.genera[k] != SCR_IND_PARTITIO) continue;
            if (strcmp(s.nomina[k], "norm_classes") == 0)
                norm_g = s.partitiones[k].n_groups;
            if (strcmp(s.nomina[k], "unit_orbits") == 0)
                unit_g = s.partitiones[k].n_groups;
            if (strcmp(s.nomina[k], "galois_orbits") == 0)
                galois_g = s.partitiones[k].n_groups;
        }
        printf("  Z[omega] universal-only: norm=%d, unit=%d, galois=%d\n",
               norm_g, unit_g, galois_g);

        /* 1, omega, 1+omega are all units (norm=1, same orbit).
         * 2 has norm=4, different orbit. */
        check(norm_g == 2, "universal-only: 2 norm classes (1 and 4)");
        check(unit_g == 2, "universal-only: 2 unit orbits (units and 2)");

        /* Galois: conj(1)=1, conj(omega)=omega^2 (not in set),
         * conj(1+omega)=-omega (not in set), conj(2)=2.
         * So 4 singletons = 4 galois orbits. */
        check(galois_g == 4,
              "universal-only: 4 galois orbits (all singletons)");
    }

    scr_print_scrutinium(&s);

    scr_scrutinium_libera(&s);
}

/* ================================================================
 * Adoption cost measurement
 * ================================================================ */

static void print_adoption_cost(void) {
    printf("\n=== Z[omega] Adoption Cost (Sprint 4B: with macros) ===\n\n");
    printf("  Genus wrappers via SCR_WRAP_* macros:\n");
    printf("    Identity:  SCR_WRAP_EQ, SCR_WRAP_HASH          = 2 macros\n");
    printf("    Ring ops:  SCR_WRAP_UNOP x2, SCR_WRAP_BINOP x2 = 4 macros\n");
    printf("    Norm:      SCR_WRAP_CLASSIS_HASH_LONG/EQ       = 2 macros\n");
    printf("    Units:     SCR_WRAP_GEN_ITER                   = 1 macro\n");
    printf("    Galois:    SCR_WRAP_GEN_GALOIS2                = 1 macro\n");
    printf("    Pars+axis: SCR_WRAP_PARS2, SCR_WRAP_AXIS       = 2 macros\n");
    printf("    Type-spec: SCR_WRAP_CLASSIS_HASH_INT/EQ        = 2 macros\n");
    printf("    ---- Total: 14 macro invocations, 14 lines ----\n\n");

    printf("  Integration code:\n");
    printf("    eis_genus_crea()            = ~25 lines\n");
    printf("    eis_registra_proprios()     = ~10 lines\n");
    printf("    ---- Total integration: ~35 lines ----\n\n");

    printf("  TOTAL: ~49 lines to connect Z[omega] (was ~92 pre-macros)\n");
    printf("  Universal detectors: 14 for FREE (10 algebraic + 4 component)\n");
    printf("  Type-specific detectors: 1 (v3_norm only)\n\n");

    printf("  === Cross-Type Comparison (Sprint 4B) ===\n");
    printf("  Type       | Macros | Custom | Integration | Total Lines\n");
    printf("  -----------|--------|--------|-------------|------------\n");
    printf("  Z[i]       |   16   |   4    |    ~35      |    ~55\n");
    printf("  Z[zeta_8]  |   12   |   2    |    ~35      |    ~53\n");
    printf("  Z[omega]   |   14   |   0    |    ~35      |    ~49\n\n");

    printf("  KEY FINDINGS:\n");
    printf("    - Macros reduce wrapper LOC by ~50%% vs hand-written\n");
    printf("    - Z[omega] is 100%% macroized (0 custom wrappers)\n");
    printf("    - Z[zeta_8] needs 2 custom (norm returns struct, not scalar)\n");
    printf("    - Z[i] needs 4 custom (axis_hash map + comparison tests)\n\n");
}

/* ================================================================
 * Test: Structural signature extraction
 *
 * Uses the full 11-value pipeline data set.
 * Z[omega] should show V-shape (galois || unit), no diamond.
 * ================================================================ */

static void test_signatura(void) {
    Scr_Genus genus;
    Scr_Registrum reg;
    Scr_Scrutinium s;
    Scr_Signatura sig;
    Eis_Z vals[11];

    printf("\n--- Structural Signature Test (Z[omega]) ---\n");

    /* Same pipeline data */
    vals[0]  = eis_make(1, 0);
    vals[1]  = eis_make(0, 1);
    vals[2]  = eis_make(2, 0);
    vals[3]  = eis_make(1, -1);
    vals[4]  = eis_make(2, 1);
    vals[5]  = eis_make(3, 0);
    vals[6]  = eis_make(1, 1);
    vals[7]  = eis_make(2, -1);
    vals[8]  = eis_make(3, 1);
    vals[9]  = eis_make(0, 2);
    vals[10] = eis_make(3, -1);

    genus = eis_genus_crea();
    scr_registrum_init(&reg);
    scr_registra_universales(&reg, &genus);
    eis_registra_proprios(&reg);
    s = scr_scrutare(&reg, &genus, vals, 11);

    /* Extract signature */
    sig = scr_signatura_ex(&s);

    printf("  unique=%d, dups=%d, alt=%d, width=%d\n",
           sig.partitiones_numerus, sig.duplices_numerus,
           sig.altitudo, sig.latitudo);

    /* Topology flags — same as Z[i]: V-shape, no diamond */
    check(!sig.est_catena, "Z[omega] sig: NOT a chain");
    check(sig.est_V, "Z[omega] sig: V-shape (galois || unit)");
    check(!sig.est_diamas, "Z[omega] sig: no diamond (|Gal|=2)");

    /* Structural counts */
    check(sig.altitudo >= 2,
          "Z[omega] sig: altitude >= 2");
    check(sig.latitudo >= 2,
          "Z[omega] sig: width >= 2");
    check(sig.incomparabiles_numerus >= 1,
          "Z[omega] sig: at least 1 incomparable pair");

    scr_signatura_imprime(&sig);

    scr_signatura_libera(&sig);
    scr_scrutinium_libera(&s);
}

/* ================================================================ */

int main(void) {
    printf("=== probatio_eisenstein_scrutinium ===\n");
    printf("=== Z[omega] through the generic pipeline ===\n\n");

    test_arithmetic();
    test_pipeline();
    test_universal_only();
    test_signatura();
    print_adoption_cost();

    printf("=== Results: %d tests, %d passed, %d failed ===\n",
           n_tests, n_pass, n_fail);

    return n_fail > 0 ? 1 : 0;
}
