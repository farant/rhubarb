/*
 * KNOTAPEL DEMO 114: Exact Audit of Demos 110-112
 * ================================================================
 *
 * Demos 110-112 built construction-word spectra on homemade arithmetic:
 *   - Rat on `long` with no overflow checks;
 *   - crossings counted only at strictly interior intersections, so a
 *     projection where a vertex lands on a segment, segments overlap, or
 *     three strands meet passes as clean;
 *   - "self-intersecting" = a crossing keeps sign 0 in all of 8 fixed
 *     directions (misses touching at vertices and collinear overlaps);
 *   - simplification = delete a vertex whenever the determinant of ONE
 *     (2,3,5) projection is unchanged (not an isotopy: the deleted
 *     triangle may sweep through the polygon, changing the knot while
 *     keeping the determinant);
 *   - D110/D111 never filtered singular polygons at all; D110 called
 *     det=5 "figure-eight" (a cinquefoil has det 5 too) and read
 *     chirality off the writhe of one projection (not an invariant).
 *
 * This demo re-checks the SAME polygons with rhubarb's exact house
 * libraries (laqueus over situs/fractio/polynomium/matrix):
 *   - laqueus_simplex: exact embedding test (any touching, any overlap);
 *   - laqueus_diagramma_genericum: a provably generic projection;
 *   - laqueus_motus_removere: a vertex deletion is legal only if the
 *     triangle it sweeps touches nothing else (a true isotopy step);
 *   - exact Alexander (Fox calculus over Z[t,t^-1]) and Jones (Kauffman
 *     bracket state sum) - Jones tells a knot from its mirror image.
 *
 * Inputs are D112's own artifacts: d112_export.c compiles Demo 112's
 * main.c UNMODIFIED and prints its polygons, its removal sequences and
 * its per-alternative verdicts (S = rejected as self-intersecting, V =
 * accepted); those lines are embedded below verbatim. The export
 * reproduces D112's published self-intersection census exactly.
 *
 *   Part A  base knots: raw braid polygons and their simplifications
 *   Part B  D112's simplification replayed move by move
 *   Part C  census + Alexander/Jones spectra on D112's polygons
 *   Part D  the 20 randomized 6_3 polygons (D112 Phase 5)
 *   Part E  D110's figure-eight "4-check classifier", re-checked
 *   Part F  D111's summary table (det spectra), re-checked
 *
 * House libraries: this demo includes laqueus.h, which brings latina.h:
 * Roman numerals and Latin keywords are macros here (see knotapel
 * CLAUDE.md). Build/run from the repo root:
 *   ./bin/aedilis knotapel/demo_114_exact_audit/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Frozen copy: demo-snapshot.c (knotapel/archive.sh).
 */

#include "laqueus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

/* ================================================================
 * Embedded data
 *
 * D112_DATA: the output of d112_export.c (this directory), one string
 * per line, verbatim. Regenerate:
 *   cd knotapel/demo_114_exact_audit &&
 *   cc -std=c89 -w -O2 -o d112_export d112_export.c &&
 *   ./d112_export | grep -v '^ ' > d112_export.txt
 * then each line of d112_export.txt becomes one C string below, in
 * order (no escaping needed: the lines contain no quotes or
 * backslashes).
 * ================================================================ */

static const char *const D112_DATA[] = {
    "RAW 3_1 18 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 20,-100,60 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,-105,60 0,-105,0",
    "REACH 3_1 7 0,0,0 20,0,60 0,-1,10 0,0,20 20,0,40 0,-1,50 0,-105,0",
    "SPEC 3_1 7 0,0,0 20,0,60 0,-1,10 0,0,20 20,0,40 0,-1,50 0,-105,0",
    "STEPS REACH 3_1 target_det=3 1 2 2 1 1 2 2 2 4 6 6",
    "STEPS SPEC 3_1 target_det=3 1 2 2 1 1 2 2 2 4 6 6",
    "VERDICT 3_1 16 VSVSSVSSSSVSSVSS",
    "RAW 4_1 29 0,0,0 20,1,10 20,0,20 40,-1,30 40,0,40 40,0,60 20,1,70 20,0,80 20,-100,80 20,-100,0 20,0,0 0,-1,10 0,0,20 0,0,40 20,1,50 20,0,60 40,-1,70 40,0,80 40,-105,80 40,-105,0 40,0,0 40,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,0,80 0,-110,80 0,-110,0",
    "REACH 4_1 8 40,0,60 20,0,80 0,-1,10 20,1,50 40,-105,80 40,0,20 0,0,80 0,-110,0",
    "SPEC 4_1 8 40,0,60 20,0,80 0,-1,10 20,1,50 40,-105,80 40,0,20 0,0,80 0,-110,0",
    "STEPS REACH 4_1 target_det=5 1 1 1 1 2 3 4 3 0 3 3 4 5 4 5 5 6 7 6 6 7",
    "STEPS SPEC 4_1 target_det=5 1 1 1 1 2 3 4 3 0 3 3 4 5 4 5 5 6 7 6 6 7",
    "VERDICT 4_1 32 VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV",
    "RAW 5_1 26 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 20,-100,100 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,-105,100 0,-105,0",
    "REACH 5_1 10 0,0,0 20,0,100 0,-1,10 0,0,20 20,0,40 0,-1,50 20,1,70 20,0,80 0,-1,90 0,-105,0",
    "SPEC 5_1 10 0,0,0 20,0,100 0,-1,10 0,0,20 20,0,40 0,-1,50 20,1,70 20,0,80 0,-1,90 0,-105,0",
    "STEPS REACH 5_1 target_det=5 1 2 2 1 1 2 2 1 1 2 2 2 4 6 9 9",
    "STEPS SPEC 5_1 target_det=5 1 2 2 1 1 2 2 1 1 2 2 2 4 6 9 9",
    "VERDICT 5_1 128 VSSSSSSSSSSSSSSSSSVSSVSSSSVSSVSSSSVSSVSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSVSSVSSSSVSSVSSSSVSSVSSSSSSSSSSSSSSSSSS",
    "RAW 5_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "REACH 5_2 11 20,0,20 20,-1,50 40,0,100 20,0,120 0,0,20 20,-1,30 20,1,90 40,-105,0 40,0,60 0,-1,90 0,-110,0",
    "SPEC 5_2 11 20,0,20 20,-1,50 40,0,100 20,0,120 0,0,20 20,-1,30 20,1,90 40,-105,0 40,0,60 0,-1,90 0,-110,0",
    "STEPS REACH 5_2 target_det=7 0 0 1 1 2 2 2 3 4 5 4 4 7 7 7 6 7 8 7 7 8 9 9 10 10 10",
    "STEPS SPEC 5_2 target_det=7 0 0 1 1 2 2 2 3 4 5 4 4 7 7 7 6 7 8 7 7 8 9 9 10 10 10",
    "VERDICT 5_2 256 VSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSSSSSSVVVVVVVVSSSS",
    "RAW 6_1 48 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,1,70 20,0,80 20,0,100 40,-1,110 40,0,120 60,1,130 60,0,140 60,-100,140 60,-100,0 60,0,0 60,0,80 40,-1,90 40,0,100 20,1,110 20,0,120 20,0,140 20,-105,140 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,-1,50 40,0,60 40,0,80 60,1,90 60,0,100 60,0,120 40,-1,130 40,0,140 40,-110,140 40,-110,0 40,0,0 40,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 0,0,140 0,-115,140 0,-115,0",
    "REACH 6_1 12 20,0,20 40,0,120 60,0,140 40,-1,90 40,0,100 0,0,20 60,0,120 40,-1,130 40,-110,0 20,1,50 20,0,60 0,-115,0",
    "SPEC 6_1 14 20,0,20 20,1,70 60,0,140 60,0,80 20,1,110 20,-105,0 20,0,0 0,0,20 60,1,90 60,0,100 40,0,140 40,0,40 20,1,50 20,0,60",
    "STEPS REACH 6_1 target_det=9 23 0 0 1 1 1 2 2 2 3 4 4 4 4 7 7 9 10 10 10 10 10 10 10 12 12 13 13 1 6 6 5 11 11 11 11",
    "STEPS SPEC 6_1 target_det=15 0 0 1 1 1 2 2 2 3 4 4 4 5 5 2 5 5 5 7 8 8 8 8 8 10 10 11 11 11 14 14 14 14 14",
    "VERDICT 6_1 512 VSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
    "RAW 6_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "REACH 6_2 10 20,0,20 20,-1,50 40,1,70 0,0,20 20,-1,30 20,0,100 40,0,0 20,-1,70 20,0,80 0,-110,0",
    "SPEC 6_2 10 20,0,20 20,-1,50 40,1,70 0,0,20 20,-1,30 20,0,100 40,0,0 20,-1,70 20,0,80 0,-110,0",
    "STEPS REACH 6_2 target_det=11 0 0 1 1 2 3 3 3 3 3 4 4 7 7 7 6 6 7 7 7 7 3 7 9 9 9 9",
    "STEPS SPEC 6_2 target_det=11 0 0 1 1 2 3 3 3 3 3 4 4 7 7 7 6 6 7 7 7 7 3 7 9 9 9 9",
    "VERDICT 6_2 128 VSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSSSSVVVVSS",
    "RAW 6_3 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,120 0,-110,120 0,-110,0",
    "REACH 6_3 11 20,0,20 20,-1,70 40,1,90 20,-100,0 0,0,20 20,-1,30 40,0,80 20,0,100 40,-105,0 20,0,60 0,-110,0",
    "SPEC 6_3 11 20,0,20 20,-1,70 40,1,90 20,-100,0 0,0,20 20,-1,30 40,0,80 20,0,100 40,-105,0 20,0,60 0,-110,0",
    "STEPS REACH 6_3 target_det=13 0 0 1 1 1 2 3 3 3 3 4 4 6 7 8 9 9 9 10 10 6 9 10 10 10 10",
    "STEPS SPEC 6_3 target_det=13 0 0 1 1 1 2 3 3 3 3 4 4 6 7 8 9 9 9 10 10 6 9 10 10 10 10",
    "VERDICT 6_3 256 VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV",
    "RAW 7_1 34 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 0,-1,110 0,0,120 20,1,130 20,0,140 20,-100,140 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 20,1,110 20,0,120 0,-1,130 0,0,140 0,-105,140 0,-105,0",
    "REACH 7_1 13 0,0,0 20,0,140 0,-1,10 0,0,20 20,0,40 0,-1,50 0,0,60 20,0,80 0,-1,90 20,1,110 20,0,120 0,-1,130 0,-105,0",
    "SPEC 7_1 13 0,0,0 20,0,140 0,-1,10 0,0,20 20,0,40 0,-1,50 0,0,60 20,0,80 0,-1,90 20,1,110 20,0,120 0,-1,130 0,-105,0",
    "STEPS REACH 7_1 target_det=7 1 2 2 1 1 2 2 1 1 2 2 1 1 2 2 2 4 7 9 12 12",
    "STEPS SPEC 7_1 target_det=7 1 2 2 1 1 2 2 1 1 2 2 1 1 2 2 2 4 7 9 12 12",
    "VERDICT 7_1 1024 VSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSVSSVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
    "RAW 7_2 56 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,0,140 40,1,150 40,0,160 60,-1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,1,130 40,0,140 20,-1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 60,-1,130 60,0,140 60,0,160 40,1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,180 0,-115,180 0,-115,0",
    "REACH 7_2 14 20,0,20 20,-1,50 40,1,150 60,-100,0 60,0,120 20,0,160 0,0,20 20,-1,30 20,0,100 60,-1,130 40,-110,180 40,0,60 0,-1,90 0,-115,0",
    "SPEC 7_2 14 20,0,20 20,-1,50 40,1,150 60,-100,0 60,0,120 20,0,160 0,0,20 20,-1,30 20,0,100 60,-1,130 40,-110,180 40,0,60 0,-1,90 0,-115,0",
    "STEPS REACH 7_2 target_det=11 0 0 1 1 2 2 2 3 3 3 2 3 3 3 3 4 5 5 5 6 6 7 6 6 9 9 9 8 10 10 11 11 11 11 12 12 13 13 8 13 13 13",
    "STEPS SPEC 7_2 target_det=11 0 0 1 1 2 2 2 3 3 3 2 3 3 3 3 4 5 5 5 6 6 7 6 6 9 9 9 8 10 10 11 11 11 11 12 12 13 13 8 13 13 13",
    "VERDICT 7_2 2048 VSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVSSSSSSSSSSSSSSSS",
    "RAW 7_3 45 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-100,160 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-105,160 40,-105,0 40,0,0 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    "REACH 7_3 11 0,0,0 20,0,100 40,0,140 20,-100,0 0,0,20 20,0,40 0,-1,50 20,1,70 40,-105,160 40,0,100 0,-110,160",
    "SPEC 7_3 11 0,0,0 20,0,100 40,0,140 20,-100,0 0,0,20 20,0,40 0,-1,50 20,1,70 40,-105,160 40,0,100 0,-110,160",
    "STEPS REACH 7_3 target_det=13 1 2 2 1 1 2 2 1 1 2 2 3 3 3 4 4 5 7 10 10 9 10 10 10 9 8 9 9 11 11 11 11 10 11",
    "STEPS SPEC 7_3 target_det=13 1 2 2 1 1 2 2 1 1 2 2 3 3 3 4 4 5 7 10 10 9 10 10 10 9 8 9 9 11 11 11 11 10 11",
    "VERDICT 7_3 256 VSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
    "RAW 7_4 56 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,0,140 40,-1,150 40,0,160 60,1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,-1,130 40,0,140 20,1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 60,1,130 60,0,140 60,0,160 40,-1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,180 0,-115,180 0,-115,0",
    "REACH 7_4 14 0,0,0 40,0,160 60,0,180 40,-1,130 40,0,140 20,0,180 0,0,20 40,0,80 20,-1,90 20,0,100 60,0,160 40,-1,170 20,-1,50 20,0,60",
    "SPEC 7_4 14 0,0,0 40,0,160 60,0,180 40,-1,130 40,0,140 20,0,180 0,0,20 40,0,80 20,-1,90 20,0,100 60,0,160 40,-1,170 20,-1,50 20,0,60",
    "STEPS REACH 7_4 target_det=15 1 2 2 2 1 1 2 3 3 3 2 1 1 2 3 3 3 3 5 5 6 7 6 6 7 7 7 7 10 10 10 10 12 12 13 13 12 14 14 15 15 14",
    "STEPS SPEC 7_4 target_det=15 1 2 2 2 1 1 2 3 3 3 2 1 1 2 3 3 3 3 5 5 6 7 6 6 7 7 7 7 10 10 10 10 12 12 13 13 12 14 14 15 15 14",
    "VERDICT 7_4 2048 VSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
    "RAW 8_18 49 0,0,0 20,-1,10 20,0,20 40,1,30 40,0,40 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-100,160 40,-100,0 40,0,0 40,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-105,160 20,-105,0 20,0,0 0,1,10 0,0,20 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    "REACH 8_18 12 40,0,60 0,0,120 40,0,160 20,-1,30 20,-1,90 40,1,110 20,-1,150 20,-105,0 20,0,60 40,0,100 0,0,160 0,-110,0",
    "SPEC 8_18 12 40,0,60 0,0,120 40,0,160 20,-1,30 20,-1,90 40,1,110 20,-1,150 20,-105,0 20,0,60 40,0,100 0,0,160 0,-110,0",
    "STEPS REACH 8_18 target_det=45 0 0 1 1 2 2 2 2 3 3 3 4 4 4 4 0 4 5 5 6 7 7 8 8 9 9 9 9 9 4 9 9 10 10 10 10 11",
    "STEPS SPEC 8_18 target_det=45 0 0 1 1 2 2 2 2 3 3 3 4 4 4 4 0 4 5 5 6 7 7 8 8 9 9 9 9 9 4 9 9 10 10 10 10 11",
    "VERDICT 8_18 512 VSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
    "TRIAL0 6_3 11 20,0,20 20,-1,70 40,1,90 20,-100,0 0,0,20 20,-1,30 40,0,80 20,0,100 40,-105,0 20,0,60 0,-110,0",
    "STEPS TRIAL0 6_3 target_det=13 0 0 1 1 1 2 3 3 3 3 4 4 6 7 8 9 9 9 10 10 6 9 10 10 10 10",
    "TRIAL1 6_3 15 20,0,20 0,0,40 20,0,80 40,1,90 20,-100,120 20,0,0 0,1,10 20,-1,30 40,0,60 20,-1,90 40,0,120 40,-105,0 20,0,60 0,0,120 0,-110,0",
    "STEPS TRIAL1 6_3 target_det=13 21 11 21 12 29 26 10 15 1 21 12 8 15 0 3 18 16 18 3 13 9 1",
    "TRIAL2 6_3 11 20,0,20 0,0,60 40,1,90 20,-100,0 0,0,20 20,-1,30 40,1,110 40,0,0 20,-1,50 0,1,70 0,-110,0",
    "STEPS TRIAL2 6_3 target_det=13 9 22 0 17 13 21 9 20 2 25 20 8 2 16 20 12 6 16 3 16 10 0 2 8 4 6",
    "TRIAL3 6_3 15 20,0,20 0,1,30 0,0,40 20,0,80 40,1,90 20,-1,110 20,-100,120 20,0,0 0,0,20 40,0,80 20,0,100 40,0,120 20,-1,50 0,1,70 0,-110,0",
    "STEPS TRIAL3 6_3 target_det=13 33 20 26 9 26 5 16 18 13 5 17 21 10 21 1 7 0 14 14 9 9 14",
    "TRIAL4 6_3 14 20,0,20 20,0,80 40,0,100 20,-1,110 20,-100,120 20,0,0 0,1,10 20,-1,30 40,1,50 20,-1,90 40,0,120 40,0,40 0,1,70 0,-110,0",
    "STEPS TRIAL4 6_3 target_det=13 4 1 6 32 25 26 4 25 9 11 20 18 14 12 17 0 18 1 5 10 15 1 10",
    "TRIAL5 6_3 13 20,0,20 0,1,30 0,0,60 40,1,90 20,-100,120 20,0,0 0,0,20 40,0,60 20,0,100 40,-105,120 20,0,60 0,1,70 0,-110,0",
    "STEPS TRIAL5 6_3 target_det=13 7 12 1 14 25 3 22 4 12 12 5 18 23 13 6 11 13 17 14 0 7 4 9 12",
    "TRIAL6 6_3 15 20,-1,10 0,1,30 0,0,60 20,-1,70 20,0,80 40,1,90 20,-100,120 20,0,0 0,0,20 40,0,60 20,0,100 40,-105,120 20,-1,50 0,0,80 0,-110,0",
    "STEPS TRIAL6 6_3 target_det=13 21 15 26 20 10 27 0 19 11 23 1 2 7 12 16 19 19 10 15 6 9 11",
    "TRIAL7 6_3 12 20,0,20 0,1,30 0,0,60 40,0,100 20,-100,0 0,0,20 40,1,50 20,-1,90 40,1,110 40,0,40 0,0,80 0,-110,0",
    "STEPS TRIAL7 6_3 target_det=13 8 11 23 13 15 27 21 25 27 4 18 15 19 13 19 0 0 6 14 16 3 5 6 3 7",
    "TRIAL8 6_3 13 20,-1,10 0,1,30 0,0,60 20,-1,70 40,1,90 20,0,0 0,0,20 40,0,80 20,0,100 40,0,40 20,-1,50 0,0,120 0,-110,0",
    "STEPS TRIAL8 6_3 target_det=13 22 2 9 9 9 22 26 3 0 23 25 9 11 21 18 6 14 11 14 9 4 8 10 5",
    "TRIAL9 6_3 14 20,0,20 20,0,80 40,0,100 20,-100,0 0,1,10 20,-1,30 40,1,50 20,-1,90 40,1,110 40,0,0 20,-1,50 20,0,60 0,1,70 0,-110,120",
    "STEPS TRIAL9 6_3 target_det=13 18 33 3 23 1 10 28 8 3 0 10 6 11 7 4 13 15 1 1 17 9 7 9",
    "TRIAL10 6_3 15 20,0,20 0,1,30 20,-1,70 20,0,80 40,0,100 20,0,120 20,-100,0 0,0,20 40,1,50 20,-1,90 20,0,100 40,0,120 20,-1,50 0,1,70 0,-110,120",
    "STEPS TRIAL10 6_3 target_det=13 21 17 8 21 4 24 27 17 1 25 7 8 21 19 12 10 16 0 8 2 16 12",
    "TRIAL11 6_3 16 20,0,20 0,0,40 20,-1,70 20,0,80 40,0,100 20,-100,120 20,-100,0 0,1,10 20,-1,30 40,1,50 20,-1,90 40,0,120 40,-105,120 20,-1,50 0,0,80 0,-110,120",
    "STEPS TRIAL11 6_3 target_det=13 8 27 1 21 0 7 3 27 16 27 20 9 21 6 13 9 1 15 9 11 14",
    "TRIAL12 6_3 14 20,-1,10 0,1,30 0,0,40 40,0,100 20,-1,110 20,0,120 0,1,10 0,0,20 40,1,50 20,-1,90 40,0,120 40,-105,0 20,0,60 0,0,120",
    "STEPS TRIAL12 6_3 target_det=13 26 35 20 20 17 31 5 20 22 5 11 14 16 0 1 19 9 18 8 14 4 3 12",
    "TRIAL13 6_3 13 20,0,20 0,0,60 40,1,90 20,-100,120 20,0,0 0,1,10 20,-1,30 40,1,50 20,-1,90 40,1,110 40,0,40 0,1,70 0,-110,120",
    "STEPS TRIAL13 6_3 target_det=13 28 13 3 5 20 15 7 3 15 0 6 17 19 5 11 3 0 17 16 12 16 11 6 11",
    "TRIAL14 6_3 11 20,0,20 0,0,60 40,1,90 20,-100,0 0,1,10 20,-1,30 40,0,120 40,-105,0 20,-1,50 0,1,70 0,-110,0",
    "STEPS TRIAL14 6_3 target_det=13 19 9 6 32 19 29 25 24 9 13 0 13 13 17 0 7 1 1 15 16 2 5 9 3 6 6",
    "TRIAL15 6_3 14 0,0,0 20,0,20 20,-1,70 20,0,80 40,0,100 20,0,120 20,0,0 0,0,20 20,-1,30 40,1,50 20,-1,90 40,1,110 40,-105,120 20,0,60",
    "STEPS TRIAL15 6_3 target_det=13 5 22 20 1 26 6 29 23 15 12 25 23 2 17 18 18 8 6 18 12 2 15 6",
    "TRIAL16 6_3 15 20,0,20 0,1,30 0,0,40 20,0,80 40,1,90 20,-100,0 0,0,20 40,0,80 20,0,100 40,1,110 40,0,40 20,-1,50 20,0,60 0,0,120 0,-110,120",
    "STEPS TRIAL16 6_3 target_det=13 25 19 34 21 1 16 9 16 5 24 18 18 8 13 8 15 18 4 9 0 5 6",
    "TRIAL17 6_3 13 20,0,20 0,1,30 0,0,40 40,0,100 20,-100,0 0,0,20 40,1,50 20,-1,90 40,0,120 40,-105,0 40,0,40 0,0,80 0,-110,120",
    "STEPS TRIAL17 6_3 target_det=13 30 11 19 22 28 27 1 13 0 6 19 12 12 15 10 3 6 3 13 5 8 3 12 13",
    "TRIAL18 6_3 13 20,-1,10 0,1,30 0,0,60 40,0,100 20,-100,0 0,0,20 40,1,50 20,-1,90 40,-105,120 40,-105,0 20,0,60 0,1,70 0,-110,120",
    "STEPS TRIAL18 6_3 target_det=13 2 9 21 5 18 24 9 16 26 5 24 7 19 19 15 21 15 9 3 9 0 3 5 6",
    "TRIAL19 6_3 11 20,0,20 0,0,60 40,0,100 0,1,10 20,-1,30 20,0,40 20,-1,90 40,1,110 20,-1,50 0,1,70 0,-110,120",
    "STEPS TRIAL19 6_3 target_det=13 16 18 32 18 7 12 28 4 17 1 2 6 0 2 4 16 20 2 16 11 12 12 8 10 3 3",
    NULL
};

/* D112's 12 base knots (reachability order) and their Alexander
 * polynomials as published in D112's findings (Fox calculus on the
 * braid; standard table values), in laqueus's normal form. */
#define N_KNOTS 12

static const char *const KNOT_CODE[N_KNOTS] = {
    "3_1", "4_1", "5_1", "5_2", "6_1", "6_2",
    "6_3", "7_1", "7_2", "7_3", "7_4", "8_18"
};

static const char *const KNOT_ALEXANDER[N_KNOTS] = {
    "t^2 - t + 1",
    "t^2 - 3t + 1",
    "t^4 - t^3 + t^2 - t + 1",
    "2t^2 - 3t + 2",
    "2t^2 - 5t + 2",
    "t^4 - 3t^3 + 3t^2 - 3t + 1",
    "t^4 - 3t^3 + 5t^2 - 3t + 1",
    "t^6 - t^5 + t^4 - t^3 + t^2 - t + 1",
    "3t^2 - 5t + 3",
    "2t^4 - 3t^3 + 3t^2 - 3t + 2",
    "4t^2 - 7t + 4",
    "t^6 - 5t^5 + 10t^4 - 13t^3 + 10t^2 - 5t + 1"
};

/* D112 findings, Phase 3 summary: distinct palindromic polynomials per
 * knot (including the base knot) */
static const unsigned D112_DISTINCT[N_KNOTS] = {
    2, 4, 2, 4, 1, 4, 37, 2, 68, 1, 1, 1
};

/* D112 findings, Phase 4: reachability out-degree per knot */
static const unsigned D112_OUT_DEGREE[N_KNOTS] = {
    1, 3, 1, 3, 1, 3, 10, 1, 9, 1, 1, 1
};

/* D112 findings, Phase 5 per-trial table: valid alternatives and
 * distinct palindromic polynomials */
static const unsigned D112_TRIAL_VALID[20] = {
    256, 1, 256, 3073, 1025, 1024, 3073, 385, 673, 2048,
    1793, 4097, 1345, 1024, 256, 1025, 3073, 769, 1024, 192
};
static const unsigned D112_TRIAL_DISTINCT[20] = {
    37, 1, 11, 6, 3, 9, 21, 10, 9, 6, 5, 65, 5, 16, 27, 7, 6, 6, 65, 5
};

/* D111 findings, summary table (default base): distinct dets and
 * self-preserving alternatives (det equal to the base det), over ALL
 * alternatives (D111 had no self-intersection filter) */
#define N_D111 6
static const char *const D111_CODE[N_D111] = {
    "3_1", "4_1", "5_1", "5_2", "6_3", "8_18"
};
static const unsigned D111_DISTINCT_DETS[N_D111] = { 2, 4, 3, 4, 22, 10 };
static const unsigned D111_SELF_PRES[N_D111] = { 2, 2, 4, 4, 12, 8 };

/* D110's 12-vertex figure-eight at scale 10 (make_figure_eight) */
static const long FIG8_D110[12][3] = {
    {  30,   0,   0 }, {   0,  25,   9 }, { -15,   0,  -9 },
    {   0, -10,   0 }, {  15,   0,   9 }, {   0,  25,  -9 },
    { -30,   0,   0 }, {   0, -25,   9 }, {  15,   0,  -9 },
    {   0,  10,   0 }, { -15,   0,   9 }, {   0, -25,  -9 }
};

/* Jones polynomials of the trefoils, in laqueus's print form (terms by
 * descending exponent). laqueus's sign convention is the physical one
 * (its review confirmed the right-hand rule on a Hopf link), so a
 * right-handed trefoil gives -t^4 + t^3 + t, as in Jones's tables. */
#define JONES_RIGHT_TREFOIL "-t^4 + t^3 + t"
#define JONES_LEFT_TREFOIL  "t^-1 + t^-3 - t^-4"

/* Jones is a 2^c state sum: computed only up to this many crossings in
 * the bulk censuses (the base knots allow more) */
#define JONES_CAP_BULK 16
#define JONES_CAP_BASE 22

/* ================================================================
 * Data lookup and parsing
 * ================================================================ */

/* the line "<tag> <code> ..." (tag may contain one space, e.g.
 * "STEPS REACH") */
static const char *
find_line (
    const char *tag,
    const char *code)
{
    size_t lt = strlen(tag);
    size_t lc = strlen(code);
    int    k;

    for (k = 0; D112_DATA[k] != NULL; k++) {
        const char *s = D112_DATA[k];

        if (strncmp(s, tag, lt) == 0 && s[lt] == ' '
            && strncmp(s + lt + 1, code, lc) == 0 && s[lt + 1 + lc] == ' ')
            return s + lt + 1 + lc + 1;
    }
    return NULL;
}

/* "<n> x,y,z x,y,z ..." -> polygon (one component) */
static int
parse_polygon (
    const char *text,
    Piscina    *pool,
    Laqueus    *out)
{
    char    *end;
    long     n = strtol(text, &end, 10);
    Punctum *pts;
    i32      starts[2];
    long     k;

    if (end == text || n < 3)
        return 0;
    pts = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    for (k = 0; k < n; k++) {
        long x, y, z;

        x = strtol(end, &end, 10);
        if (*end != ',') return 0;
        y = strtol(end + 1, &end, 10);
        if (*end != ',') return 0;
        z = strtol(end + 1, &end, 10);
        pts[k] = situs_punctum((s64)x, (s64)y, (s64)z);
    }
    starts[0] = 0;
    starts[1] = (i32)n;
    return laqueus_ex_punctis(pts, starts, 1, pool, out) ? 1 : 0;
}

static int
polygon_of (
    const char *tag,
    const char *code,
    Piscina    *pool,
    Laqueus    *out)
{
    const char *line = find_line(tag, code);

    return line != NULL && parse_polygon(line, pool, out);
}

/* "VERDICT <code> <count> <letters>" -> letters */
static const char *
d112_verdicts (
    const char *code,
    unsigned   *count)
{
    const char *line = find_line("VERDICT", code);
    char       *end;

    if (line == NULL)
        return NULL;
    *count = (unsigned)strtoul(line, &end, 10);
    return end + 1;
}

/* ================================================================
 * Exact geometry: D110/D112 construction words
 * ================================================================ */

/* reflection of p through the plane of (c0, c1, c2):
 * p - 2 (n.(p - c0)) / (n.n) n, n = (c1 - c0) x (c2 - c0) */
static Punctum
mirror_point (
    Punctum  c0,
    Punctum  c1,
    Punctum  c2,
    Punctum  p,
    Piscina *pool)
{
    Fractio ax = fractio_subtrahe(c1.x, c0.x, pool);
    Fractio ay = fractio_subtrahe(c1.y, c0.y, pool);
    Fractio az = fractio_subtrahe(c1.z, c0.z, pool);
    Fractio bx = fractio_subtrahe(c2.x, c0.x, pool);
    Fractio by = fractio_subtrahe(c2.y, c0.y, pool);
    Fractio bz = fractio_subtrahe(c2.z, c0.z, pool);
    Fractio nx = fractio_subtrahe(fractio_multiplica(ay, bz, pool),
        fractio_multiplica(az, by, pool), pool);
    Fractio ny = fractio_subtrahe(fractio_multiplica(az, bx, pool),
        fractio_multiplica(ax, bz, pool), pool);
    Fractio nz = fractio_subtrahe(fractio_multiplica(ax, by, pool),
        fractio_multiplica(ay, bx, pool), pool);
    Fractio nn = fractio_adde(fractio_adde(fractio_multiplica(nx, nx, pool),
        fractio_multiplica(ny, ny, pool), pool),
        fractio_multiplica(nz, nz, pool), pool);
    Fractio dd = fractio_adde(fractio_adde(
        fractio_multiplica(nx, fractio_subtrahe(p.x, c0.x, pool), pool),
        fractio_multiplica(ny, fractio_subtrahe(p.y, c0.y, pool), pool),
        pool),
        fractio_multiplica(nz, fractio_subtrahe(p.z, c0.z, pool), pool),
        pool);
    Fractio t;
    Punctum r;

    (void)fractio_divide(fractio_multiplica(fractio_ex_s64(2), dd, pool),
        nn, pool, &t);
    r.x = fractio_subtrahe(p.x, fractio_multiplica(t, nx, pool), pool);
    r.y = fractio_subtrahe(p.y, fractio_multiplica(t, ny, pool), pool);
    r.z = fractio_subtrahe(p.z, fractio_multiplica(t, nz, pool), pool);
    return r;
}

/* all-base construction word: vertex k >= 3 mirrored through the plane
 * of vertices 0, 1, 2 iff bit (k - 3) of choices is set (D110's
 * evaluate_construction with all-base centers, D112's evaluate_inplace
 * with base (0, 1, 2)) */
static void
build_alternative (
    Laqueus        base,
    unsigned long  choices,
    Piscina       *pool,
    Laqueus       *out)
{
    i32      n   = laqueus_numerus(base);
    Punctum *pts = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    Punctum  c0  = laqueus_vertex(base, 0);
    Punctum  c1  = laqueus_vertex(base, 1);
    Punctum  c2  = laqueus_vertex(base, 2);
    i32      starts[2];
    i32      k;

    for (k = 0; k < n; k++) {
        Punctum v = laqueus_vertex(base, k);

        if (k >= 3 && ((choices >> (k - 3)) & 1UL))
            pts[k] = mirror_point(c0, c1, c2, v, pool);
        else
            pts[k] = v;
    }
    starts[0] = 0;
    starts[1] = n;
    (void)laqueus_ex_punctis(pts, starts, 1, pool, out);
}

/* the polygon with vertex idx deleted, no checks (to follow D112) */
static void
delete_vertex (
    Laqueus   l,
    i32       idx,
    Piscina  *pool,
    Laqueus  *out)
{
    i32      n   = laqueus_numerus(l);
    Punctum *pts = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    i32      starts[2];
    i32      k, m = 0;

    for (k = 0; k < n; k++)
        if (k != idx)
            pts[m++] = laqueus_vertex(l, k);
    starts[0] = 0;
    starts[1] = m;
    (void)laqueus_ex_punctis(pts, starts, 1, pool, out);
}

/* ================================================================
 * Exact classification
 * ================================================================ */

typedef struct {
    int      simple;
    unsigned crossings;
    int      writhe;
    unsigned long det;      /* |Delta(-1)| */
    int      palindromic;
    chorda   alexander;     /* in the caller's pool */
    chorda   jones;         /* datum NULL: not computed */
    int      jones_skipped; /* crossings above the cap */
} Exact;

static chorda
empty_chorda (void)
{
    chorda c;

    c.datum   = NULL;
    c.mensura = 0;
    return c;
}

static unsigned long
chorda_to_ulong (
    chorda c)
{
    char buf[32];
    int  k;
    int  n = c.mensura < 31 ? (int)c.mensura : 31;

    for (k = 0; k < n; k++)
        buf[k] = (char)c.datum[k];
    buf[n] = '\0';
    return strtoul(buf, NULL, 10);
}

/* classify l; intermediate work in scratch (rolled back), strings
 * copied into out_pool */
static Exact
classify (
    Laqueus   l,
    unsigned  jones_cap,
    Piscina  *scratch,
    Piscina  *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Exact          e;
    Diagramma      d;
    Polynomium     p;

    e.simple        = laqueus_simplex(l, scratch) ? 1 : 0;
    e.crossings     = 0;
    e.writhe        = 0;
    e.det           = 0;
    e.palindromic   = 0;
    e.alexander     = empty_chorda();
    e.jones         = empty_chorda();
    e.jones_skipped = 0;
    if (e.simple && laqueus_diagramma_genericum(l, scratch, &d)) {
        e.crossings = (unsigned)diagramma_numerus(d);
        e.writhe    = (int)diagramma_scriptura(d);
        if (diagramma_alexander(d, scratch, &p)) {
            Polynomium inv;
            Fractio    v;

            e.alexander = chorda_transcribere(
                polynomium_ad_chordam(p, 't', scratch), out_pool);
            if (polynomium_contrahe(p, -1, scratch, &inv)
                && polynomium_normale(inv, scratch, &inv))
                e.palindromic = polynomium_aequalis(p, inv) ? 1 : 0;
            if (polynomium_valor(p, fractio_ex_s64(-1), scratch, &v))
                e.det = chorda_to_ulong(fractio_ad_chordam(
                    fractio_absolutum(v, scratch), scratch));
        }
        if (e.crossings <= jones_cap) {
            if (diagramma_jones(d, scratch, &p))
                e.jones = chorda_transcribere(
                    polynomium_ad_chordam(p, 't', scratch), out_pool);
        } else {
            e.jones_skipped = 1;
        }
    }
    piscina_reficere(scratch, mark);
    return e;
}

/* classify alternative `choices` of base (built in scratch) */
static Exact
classify_alternative (
    Laqueus        base,
    unsigned long  choices,
    unsigned       jones_cap,
    Piscina       *scratch,
    Piscina       *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Laqueus        alt;
    Exact          e;

    build_alternative(base, choices, scratch, &alt);
    e = classify(alt, jones_cap, scratch, out_pool);
    piscina_reficere(scratch, mark);
    return e;
}

static void
print_chorda (
    chorda c)
{
    if (c.datum == NULL)
        printf("-");
    else
        printf("%.*s", (int)c.mensura, (const char*)c.datum);
}

static int
chorda_is (
    chorda      c,
    const char *lit)
{
    return c.datum != NULL && chorda_aequalis_literis(c, lit);
}

/* ================================================================
 * Spectrum: distinct keys with populations
 * ================================================================ */

#define MAX_SPECTRUM 512

typedef struct {
    chorda   key[MAX_SPECTRUM];
    unsigned count[MAX_SPECTRUM];
    unsigned n;
    unsigned overflow;
} Spectrum;

static void
spectrum_clear (
    Spectrum *s)
{
    s->n        = 0;
    s->overflow = 0;
}

/* key copied into keep when new */
static void
spectrum_add (
    Spectrum *s,
    chorda    key,
    Piscina  *keep)
{
    unsigned k;

    for (k = 0; k < s->n; k++) {
        if (chorda_aequalis(s->key[k], key)) {
            s->count[k]++;
            return;
        }
    }
    if (s->n >= MAX_SPECTRUM) {
        s->overflow++;
        return;
    }
    s->key[s->n]   = chorda_transcribere(key, keep);
    s->count[s->n] = 1;
    s->n++;
}

static int
spectrum_has (
    const Spectrum *s,
    const char     *lit)
{
    unsigned k;

    for (k = 0; k < s->n; k++)
        if (chorda_aequalis_literis(s->key[k], lit))
            return 1;
    return 0;
}

/* Alexander | Jones as one key (Jones "?" when skipped) */
static chorda
pair_key (
    Exact    e,
    Piscina *pool)
{
    chorda bar = chorda_ex_literis("  |  ", pool);
    chorda j   = e.jones.datum != NULL ? e.jones
               : chorda_ex_literis("?", pool);

    return chorda_concatenare(chorda_concatenare(e.alexander, bar, pool),
        j, pool);
}

/* ================================================================
 * Part A: base knots
 * ================================================================ */

static void
part_a (
    Piscina *keep,
    Piscina *scratch)
{
    int k;

    printf("\n=== Part A: base knots (raw braid polygon vs D112's "
        "simplifications) ===\n");
    printf("  %-5s %-6s %-6s %-6s  %-9s %-9s %-9s  %s\n", "knot",
        "raw", "reach", "spec", "rawAlex", "reachAlx", "specAlx",
        "Jones raw / reach / spec");
    for (k = 0; k < N_KNOTS; k++) {
        const char *code = KNOT_CODE[k];
        Laqueus     raw, reach, spec;
        Exact       er, ec, es;
        char        msg[160];
        int         same_rs, same_rp;

        if (!polygon_of("RAW", code, keep, &raw)
            || !polygon_of("REACH", code, keep, &reach)
            || !polygon_of("SPEC", code, keep, &spec)) {
            check("D112 polygons parse", 0);
            continue;
        }
        er = classify(raw, JONES_CAP_BASE, scratch, keep);
        ec = classify(reach, JONES_CAP_BASE, scratch, keep);
        es = classify(spec, JONES_CAP_BASE, scratch, keep);
        printf("  %-5s %2u/%-3u %2u/%-3u %2u/%-3u  %-9s %-9s %-9s  ", code,
            (unsigned)laqueus_numerus(raw), er.crossings,
            (unsigned)laqueus_numerus(reach), ec.crossings,
            (unsigned)laqueus_numerus(spec), es.crossings,
            !er.simple ? "SINGULAR" : chorda_is(er.alexander,
                KNOT_ALEXANDER[k]) ? "table" : "OTHER",
            !ec.simple ? "SINGULAR" : chorda_is(ec.alexander,
                KNOT_ALEXANDER[k]) ? "table" : "OTHER",
            !es.simple ? "SINGULAR" : chorda_is(es.alexander,
                KNOT_ALEXANDER[k]) ? "table" : "OTHER");
        print_chorda(er.jones);
        same_rs = er.jones.datum != NULL && ec.jones.datum != NULL
            && chorda_aequalis(er.jones, ec.jones);
        same_rp = er.jones.datum != NULL && es.jones.datum != NULL
            && chorda_aequalis(er.jones, es.jones);
        printf(" / %s / %s\n",
            ec.jones.datum == NULL ? "-" : same_rs ? "same" : "DIFFERENT",
            es.jones.datum == NULL ? "-" : same_rp ? "same" : "DIFFERENT");
        if (!chorda_is(ec.alexander, KNOT_ALEXANDER[k])
            || (ec.jones.datum != NULL && er.jones.datum != NULL
                && !same_rs)) {
            printf("        reach Alexander: ");
            print_chorda(ec.alexander);
            printf("   Jones: ");
            print_chorda(ec.jones);
            printf("\n");
        }
        if (laqueus_numerus(spec) != laqueus_numerus(reach)
            && !chorda_is(es.alexander, KNOT_ALEXANDER[k])) {
            printf("        spec  Alexander: ");
            print_chorda(es.alexander);
            printf("   Jones: ");
            print_chorda(es.jones);
            printf("\n");
        }
        sprintf(msg, "%s raw braid polygon is simple with the table "
            "Alexander polynomial", code);
        check(msg, er.simple && chorda_is(er.alexander, KNOT_ALEXANDER[k]));
    }
}

/* ================================================================
 * Part B: D112's simplification, replayed as triangle moves
 * ================================================================ */

static void
replay_steps (
    const char *which,
    const char *code,
    Laqueus     raw,
    Laqueus     expect,
    Piscina    *keep,
    unsigned   *n_steps,
    unsigned   *n_illegal,
    int        *first_illegal,
    int        *ends_equal)
{
    const char *line;
    char        tag[32];
    const char *p;
    Laqueus     cur = raw;

    *n_steps       = 0;
    *n_illegal     = 0;
    *first_illegal = -1;
    *ends_equal    = 0;
    sprintf(tag, "STEPS %s", which);
    line = find_line(tag, code);
    if (line == NULL)
        return;
    p = strchr(line, ' ');   /* skip "target_det=N" */
    while (p != NULL && *p == ' ') {
        char *end;
        long  idx = strtol(p + 1, &end, 10);
        Laqueus next;

        if (end == p + 1)
            break;
        if (!laqueus_motus_removere(cur, (i32)idx, keep, &next)) {
            if (*first_illegal < 0)
                *first_illegal = (int)*n_steps;
            (*n_illegal)++;
            delete_vertex(cur, (i32)idx, keep, &next);
        }
        cur = next;
        (*n_steps)++;
        p = end;
    }
    *ends_equal = chorda_aequalis(laqueus_ad_chordam(cur, keep),
        laqueus_ad_chordam(expect, keep));
}

/* greedy legal simplification: delete the first vertex whose triangle
 * move is legal, repeat (D112's deterministic scan order) */
static Laqueus
simplify_legal (
    Laqueus  l,
    Piscina *keep)
{
    int changed = 1;

    while (changed && laqueus_numerus(l) > 3) {
        i32 k;

        changed = 0;
        for (k = 0; k < laqueus_numerus(l); k++) {
            Laqueus next;

            if (laqueus_motus_removere(l, k, keep, &next)) {
                l       = next;
                changed = 1;
                break;
            }
        }
    }
    return l;
}

static void
part_b (
    Piscina *keep,
    Piscina *scratch)
{
    int k;
    unsigned total_illegal = 0;

    printf("\n=== Part B: D112's det-greedy simplification replayed as "
        "triangle moves ===\n");
    printf("  %-5s %-22s %-22s %-24s\n", "knot",
        "REACH: steps/illegal", "SPEC: steps/illegal",
        "legal greedy: verts, knot");
    for (k = 0; k < N_KNOTS; k++) {
        const char *code = KNOT_CODE[k];
        Laqueus     raw, reach, spec, legal;
        unsigned    ns1, ni1, ns2, ni2;
        int         f1, f2, eq1, eq2;
        Exact       er, el;
        char        msg[160];

        if (!polygon_of("RAW", code, keep, &raw)
            || !polygon_of("REACH", code, keep, &reach)
            || !polygon_of("SPEC", code, keep, &spec))
            continue;
        replay_steps("REACH", code, raw, reach, keep, &ns1, &ni1, &f1, &eq1);
        replay_steps("SPEC", code, raw, spec, keep, &ns2, &ni2, &f2, &eq2);
        legal = simplify_legal(raw, keep);
        er = classify(raw, 0, scratch, keep);
        el = classify(legal, 0, scratch, keep);
        printf("  %-5s %3u / %-3u (first #%-3d) %3u / %-3u (first #%-3d) "
            "%3u -> %-3u %s\n", code, ns1, ni1, f1, ns2, ni2, f2,
            (unsigned)laqueus_numerus(raw), (unsigned)laqueus_numerus(legal),
            el.simple && chorda_aequalis(el.alexander, er.alexander)
                ? "same Alexander" : "CHANGED");
        total_illegal += ni1;
        sprintf(msg, "%s: replay of D112's removal sequence ends on D112's "
            "polygon", code);
        check(msg, eq1 && eq2);
        sprintf(msg, "%s: legal greedy simplification keeps the knot", code);
        check(msg, el.simple && chorda_aequalis(el.alexander, er.alexander));
    }
    printf("  illegal deletions along the 12 reachability simplifications: "
        "%u\n", total_illegal);
}

/* ================================================================
 * Part C: census and spectra on D112's polygons
 * ================================================================ */

static Spectrum spectra[N_KNOTS];      /* Alexander, per base knot */

static void
part_c (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    int k;
    unsigned reach_exact[N_KNOTS][N_KNOTS];

    printf("\n=== Part C: census + spectra on D112's reachability polygons "
        "===\n");
    printf("  %-5s %5s | %-21s | %-26s | %-24s\n", "knot", "alts",
        "D112 valid / S", "exact simple / singular",
        "D112 S->simple, V->sing");
    for (k = 0; k < N_KNOTS; k++) {
        const char   *code = KNOT_CODE[k];
        Laqueus       base;
        unsigned      count, n_v = 0, n_s = 0, n_simple = 0, n_sing = 0;
        unsigned      s_simple = 0, v_sing = 0, jones_skipped = 0;
        unsigned      nonpal = 0;
        const char   *verd = d112_verdicts(code, &count);
        unsigned long c;
        Spectrum      pairs;
        int           j;
        char          msg[200];

        spectrum_clear(&spectra[k]);
        spectrum_clear(&pairs);
        if (verd == NULL || !polygon_of("REACH", code, keep, &base))
            continue;
        for (c = 0; c < count; c++) {
            PiscinaNotatio mark = piscina_notare(work);
            Exact          e = classify_alternative(base, c, JONES_CAP_BULK,
                scratch, work);

            if (verd[c] == 'V') n_v++; else n_s++;
            if (e.simple) {
                n_simple++;
                if (verd[c] != 'V') s_simple++;
                spectrum_add(&spectra[k], e.alexander, keep);
                spectrum_add(&pairs, pair_key(e, work), keep);
                if (e.jones_skipped) jones_skipped++;
                if (!e.palindromic) nonpal++;
            } else {
                n_sing++;
                if (verd[c] == 'V') v_sing++;
            }
            piscina_reficere(work, mark);
        }
        printf("  %-5s %5u | %5u / %-13u | %5u / %-18u | %5u / %-16u\n",
            code, count, n_v, n_s, n_simple, n_sing, s_simple, v_sing);
        printf("        distinct Alexander: exact %u (D112 %u); "
            "Alexander|Jones pairs %u; Jones skipped (>%d crossings) %u; "
            "non-palindromic %u\n", spectra[k].n, D112_DISTINCT[k], pairs.n,
            JONES_CAP_BULK, jones_skipped, nonpal);
        sprintf(msg, "%s: every exact Alexander polynomial is palindromic",
            code);
        check(msg, nonpal == 0);
        for (j = 0; j < N_KNOTS; j++)
            reach_exact[k][j] = (unsigned)spectrum_has(&spectra[k],
                KNOT_ALEXANDER[j]);
        if (strcmp(code, "6_3") == 0) {
            unsigned q;

            printf("        6_3 spectrum (Alexander [population]):\n");
            for (q = 0; q < spectra[k].n; q++) {
                printf("          ");
                print_chorda(spectra[k].key[q]);
                printf(" [%u]\n", spectra[k].count[q]);
            }
            printf("        6_3 Alexander|Jones pairs with population "
                ">= 4:\n");
            for (q = 0; q < pairs.n; q++) {
                if (pairs.count[q] >= 4) {
                    printf("          ");
                    print_chorda(pairs.key[q]);
                    printf(" [%u]\n", pairs.count[q]);
                }
            }
        }
    }

    printf("\n  reachability (exact; 'x' = knot j's Alexander polynomial "
        "occurs among i's simple alternatives)\n  %-6s", "");
    for (k = 0; k < N_KNOTS; k++)
        printf("%5s", KNOT_CODE[k]);
    printf("  out (D112)\n");
    for (k = 0; k < N_KNOTS; k++) {
        int      j;
        unsigned out = 0;

        printf("  %-6s", KNOT_CODE[k]);
        for (j = 0; j < N_KNOTS; j++) {
            printf("%5s", reach_exact[k][j] ? "x" : ".");
            out += reach_exact[k][j];
        }
        printf("  %3u (%u)\n", out, D112_OUT_DEGREE[k]);
    }
}

/* ================================================================
 * Part D: D112's 20 randomized 6_3 polygons
 * ================================================================ */

static void
part_d (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    int trial;

    printf("\n=== Part D: the 20 randomized 6_3 simplifications (D112 Phase "
        "5) ===\n");
    printf("  %-6s %5s %5s | %-15s | %-15s | %-17s | %s\n", "trial",
        "verts", "alts", "valid D112/exact", "distinct D112/ex",
        "illegal deletions", "base polygon");
    for (trial = 0; trial < 20; trial++) {
        char          tag[32];
        Laqueus       base, raw;
        unsigned long c, count;
        unsigned      n_simple = 0, ns, ni;
        int           first, eq;
        Spectrum      sp;
        Exact         eb;

        sprintf(tag, "TRIAL%d", trial);
        if (!polygon_of(tag, "6_3", keep, &base)
            || !polygon_of("RAW", "6_3", keep, &raw))
            continue;
        replay_steps(tag, "6_3", raw, base, keep, &ns, &ni, &first, &eq);
        eb = classify(base, 0, scratch, keep);
        count = 1UL << (laqueus_numerus(base) - 3);
        spectrum_clear(&sp);
        for (c = 0; c < count; c++) {
            PiscinaNotatio mark = piscina_notare(work);
            Exact          e = classify_alternative(base, c, 0, scratch,
                work);

            if (e.simple) {
                n_simple++;
                spectrum_add(&sp, e.alexander, keep);
            }
            piscina_reficere(work, mark);
        }
        printf("  %-6d %5u %5lu | %6u / %-6u | %6u / %-6u | %5u of %-7u | ",
            trial, (unsigned)laqueus_numerus(base), count,
            D112_TRIAL_VALID[trial], n_simple, D112_TRIAL_DISTINCT[trial],
            sp.n, ni, ns);
        if (!eb.simple)
            printf("SINGULAR\n");
        else if (chorda_is(eb.alexander, KNOT_ALEXANDER[6]))
            printf("6_3 Alexander\n");
        else {
            printf("Alexander ");
            print_chorda(eb.alexander);
            printf("\n");
        }
    }
}

/* ================================================================
 * Part E: D110's figure-eight 4-check classifier
 * ================================================================ */

enum { T_SINGULAR, T_UNKNOT, T_FIG8, T_RIGHT3, T_LEFT3, T_OTHER, N_TYPES };

static const char *const TYPE_NAME[N_TYPES] = {
    "singular", "unknot", "fig-8", "R-trefoil", "L-trefoil", "other"
};

static int
exact_type (
    Exact e)
{
    if (!e.simple) return T_SINGULAR;
    if (chorda_is(e.alexander, "1") && chorda_is(e.jones, "1"))
        return T_UNKNOT;
    if (chorda_is(e.alexander, "t^2 - 3t + 1")) return T_FIG8;
    if (chorda_is(e.alexander, "t^2 - t + 1")) {
        if (chorda_is(e.jones, JONES_RIGHT_TREFOIL)) return T_RIGHT3;
        if (chorda_is(e.jones, JONES_LEFT_TREFOIL)) return T_LEFT3;
    }
    return T_OTHER;
}

/* D110 Result 2: b2 = b4? no -> unknot; b7 = maj(b0,b1,b5)? no ->
 * unknot; b7 = b2 -> figure-eight; else trefoil, left if b2 = 1 */
static int
d110_classifier (
    unsigned long c)
{
    int b0 = (int)(c & 1UL), b1 = (int)((c >> 1) & 1UL);
    int b2 = (int)((c >> 2) & 1UL), b4 = (int)((c >> 4) & 1UL);
    int b5 = (int)((c >> 5) & 1UL), b7 = (int)((c >> 7) & 1UL);
    int maj = (b0 + b1 + b5) >= 2;

    if (b2 != b4) return T_UNKNOT;
    if (b7 != maj) return T_UNKNOT;
    if (b7 == b2) return T_FIG8;
    return b2 ? T_LEFT3 : T_RIGHT3;
}

static void
part_e (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    Punctum       pts[12];
    i32           starts[2];
    Laqueus       fig8;
    Exact         e0;
    unsigned      confusion[N_TYPES][N_TYPES];
    unsigned      exact_count[N_TYPES];
    unsigned long c;
    int           a, b, k;
    unsigned      agree = 0, simple_total = 0, agree_simple = 0;
    unsigned      agree_swapped = 0;
    Spectrum      others;

    printf("\n=== Part E: D110's figure-eight 4-check classifier (512 "
        "alternatives, all-base) ===\n");
    for (k = 0; k < 12; k++)
        pts[k] = situs_punctum((s64)FIG8_D110[k][0], (s64)FIG8_D110[k][1],
            (s64)FIG8_D110[k][2]);
    starts[0] = 0;
    starts[1] = 12;
    if (!laqueus_ex_punctis(pts, starts, 1, keep, &fig8)) {
        check("D110 figure-eight parses", 0);
        return;
    }
    e0 = classify(fig8, JONES_CAP_BULK, scratch, keep);
    printf("  base polygon: simple %d, %u crossings, Alexander ", e0.simple,
        e0.crossings);
    print_chorda(e0.alexander);
    printf(", Jones ");
    print_chorda(e0.jones);
    printf("\n");
    check("D110 figure-eight polygon is a figure-eight",
        exact_type(e0) == T_FIG8);

    for (a = 0; a < N_TYPES; a++) {
        exact_count[a] = 0;
        for (b = 0; b < N_TYPES; b++)
            confusion[a][b] = 0;
    }
    spectrum_clear(&others);
    for (c = 0; c < 512UL; c++) {
        PiscinaNotatio mark = piscina_notare(work);
        Exact          e = classify_alternative(fig8, c, JONES_CAP_BASE,
            scratch, work);
        int            t = exact_type(e);
        int            p = d110_classifier(c);

        confusion[p][t]++;
        exact_count[t]++;
        if (p == t) agree++;
        if (p == t
            || (p == T_RIGHT3 && t == T_LEFT3)
            || (p == T_LEFT3 && t == T_RIGHT3))
            agree_swapped++;
        if (t != T_SINGULAR) {
            simple_total++;
            if (p == t) agree_simple++;
        }
        if (t == T_OTHER)
            spectrum_add(&others, pair_key(e, work), keep);
        piscina_reficere(work, mark);
    }
    printf("  exact census:");
    for (a = 0; a < N_TYPES; a++)
        printf(" %s %u", TYPE_NAME[a], exact_count[a]);
    printf("\n  (D110 reported: fig-8 64, L-trefoil 32, R-trefoil 32, "
        "unknot 384)\n");
    printf("  confusion (rows: D110 classifier, columns: exact)\n  %-10s",
        "");
    for (b = 0; b < N_TYPES; b++)
        printf("%11s", TYPE_NAME[b]);
    printf("\n");
    for (a = 0; a < N_TYPES; a++) {
        if (a == T_SINGULAR || a == T_OTHER) continue;
        printf("  %-10s", TYPE_NAME[a]);
        for (b = 0; b < N_TYPES; b++)
            printf("%11u", confusion[a][b]);
        printf("\n");
    }
    printf("  classifier agrees with the exact type on %u / 512; on the "
        "%u simple alternatives %u\n", agree, simple_total, agree_simple);
    printf("  with D110's L/R names swapped: %u / 512\n", agree_swapped);
    check("D110 classifier is exact on all 512 alternatives up to "
        "swapped chirality names", agree_swapped == 512);
    check("D110's 512 alternatives are all simple (no singular polygon)",
        exact_count[T_SINGULAR] == 0);
    if (others.n > 0) {
        unsigned q;

        printf("  'other' knot types (Alexander | Jones [population]):\n");
        for (q = 0; q < others.n; q++) {
            printf("    ");
            print_chorda(others.key[q]);
            printf(" [%u]\n", others.count[q]);
        }
    }
}

/* ================================================================
 * Part F: D111's summary table
 * ================================================================ */

static void
part_f (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    int k;

    printf("\n=== Part F: D111's det spectra (D111 had no "
        "self-intersection filter) ===\n");
    printf("  %-5s %5s | %-22s | %-22s | %s\n", "knot", "alts",
        "distinct dets D111/ex", "self-pres D111/exact", "singular");
    for (k = 0; k < N_D111; k++) {
        const char   *code = D111_CODE[k];
        Laqueus       base;
        unsigned long c, count;
        unsigned      n_sing = 0, self_pres = 0;
        unsigned long dets[256];
        unsigned      n_dets = 0, q;
        unsigned long base_det = 0;

        if (!polygon_of("SPEC", code, keep, &base))
            continue;
        count = 1UL << (laqueus_numerus(base) - 3);
        for (c = 0; c < count; c++) {
            PiscinaNotatio mark = piscina_notare(work);
            Exact          e = classify_alternative(base, c, 0, scratch,
                work);

            if (!e.simple) {
                n_sing++;
            } else {
                int seen = 0;

                if (c == 0) base_det = e.det;
                if (e.det == base_det) self_pres++;
                for (q = 0; q < n_dets; q++)
                    if (dets[q] == e.det) seen = 1;
                if (!seen && n_dets < 256)
                    dets[n_dets++] = e.det;
            }
            piscina_reficere(work, mark);
        }
        printf("  %-5s %5lu | %8u / %-11u | %8u / %-11u | %u\n", code, count,
            D111_DISTINCT_DETS[k], n_dets, D111_SELF_PRES[k], self_pres,
            n_sing);
    }
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    Piscina *keep    = piscina_generare_dynamicum("d114_keep", 1 << 24);
    Piscina *scratch = piscina_generare_dynamicum("d114_scratch", 1 << 22);
    Piscina *work    = piscina_generare_dynamicum("d114_work", 1 << 20);

    printf("KNOTAPEL DEMO 114: Exact Audit of Demos 110-112\n");
    printf("================================================\n");
    if (keep == NULL || scratch == NULL || work == NULL) {
        printf("pool allocation failed\n");
        return 1;
    }
    part_a(keep, scratch);
    part_b(keep, scratch);
    part_c(keep, scratch, work);
    part_d(keep, scratch, work);
    part_e(keep, scratch, work);
    part_f(keep, scratch, work);

    printf("\n================================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    piscina_destruere(work);
    piscina_destruere(scratch);
    piscina_destruere(keep);
    return n_fail > 0 ? 1 : 0;
}
