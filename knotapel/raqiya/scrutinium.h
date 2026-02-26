/*
 * scrutinium.h -- Type-Agnostic Algebraic Graph Density Analysis
 * ===============================================================
 *
 * Single-header C89 library. Generic partition, edge-list, pair-histogram,
 * and graph analysis infrastructure for multi-relational algebraic analysis.
 *
 * This is the type-independent engine. Type-specific libraries (raqiya.h
 * for Z[zeta_8], gauss.h for Z[i], etc.) provide:
 *   - Element arithmetic
 *   - Value collection (hash + eq)
 *   - Detector criterion functions
 *
 * This library provides:
 *   - Partition type + utilities (free, refines, equal, meet, join, max_size)
 *   - Partition lattice (refinement DAG, redundancy detection)
 *   - PairHist type (dynamic bin count)
 *   - EdgeList type + utilities (init, free, add, restrict, intersect, quotient)
 *   - GraphResult type + full graph analyzer
 *   - Edge generation from partitions
 *   - Degree sequence comparison
 *
 * Name: Latin "scrutinium" = examination, analysis.
 *
 * C89, zero dependencies beyond stdlib.h/string.h.
 */

#ifndef SCRUTINIUM_H
#define SCRUTINIUM_H

#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Scr_Partition -- Group assignment result
 *
 * group_id[i] = which group value i belongs to.
 * group_sizes[g] = number of values in group g.
 * ================================================================ */

typedef struct {
    int *group_id;
    int *group_sizes;
    int n_groups;
    int n_values;
} Scr_Partition;

static void scr_partition_free(Scr_Partition *p) {
    free(p->group_id);
    free(p->group_sizes);
    p->group_id = NULL;
    p->group_sizes = NULL;
}

/* Count pairs within the same group: sum of C(size, 2) */
static int scr_partition_related_pairs(const Scr_Partition *p) {
    int count = 0, i;
    for (i = 0; i < p->n_groups; i++)
        count += p->group_sizes[i] * (p->group_sizes[i] - 1) / 2;
    return count;
}

/* Max group size */
static int scr_partition_max_size(const Scr_Partition *p) {
    int mx = 0, i;
    for (i = 0; i < p->n_groups; i++)
        if (p->group_sizes[i] > mx) mx = p->group_sizes[i];
    return mx;
}

/* Min group size */
static int scr_partition_min_size(const Scr_Partition *p) {
    int mn, i;
    if (p->n_groups == 0) return 0;
    mn = p->group_sizes[0];
    for (i = 1; i < p->n_groups; i++)
        if (p->group_sizes[i] < mn) mn = p->group_sizes[i];
    return mn;
}

/* Count singletons */
static int scr_partition_n_singletons(const Scr_Partition *p) {
    int count = 0, i;
    for (i = 0; i < p->n_groups; i++)
        if (p->group_sizes[i] == 1) count++;
    return count;
}

/* Returns 1 if partition a refines partition b:
 * every a-group is contained within a single b-group. */
static int scr_partition_refines(const Scr_Partition *a,
                                  const Scr_Partition *b) {
    int ga, i;
    if (a->n_values != b->n_values) return 0;
    for (ga = 0; ga < a->n_groups; ga++) {
        int first_b = -1;
        for (i = 0; i < a->n_values; i++) {
            if (a->group_id[i] != ga) continue;
            if (first_b == -1)
                first_b = b->group_id[i];
            else if (b->group_id[i] != first_b)
                return 0;
        }
    }
    return 1;
}

/* Returns 1 if partitions a and b are structurally identical */
static int scr_partition_equal(const Scr_Partition *a,
                                const Scr_Partition *b) {
    return scr_partition_refines(a, b) && scr_partition_refines(b, a);
}

/* ================================================================
 * Partition Lattice Operations
 * ================================================================ */

/* Meet: coarsest partition that refines BOTH p1 and p2.
 * Two elements are in the same meet-class iff they are in the same
 * class in p1 AND the same class in p2.
 * Implementation: pair (group_id_p1, group_id_p2) as compound key. */
static Scr_Partition scr_partition_meet(const Scr_Partition *p1,
                                         const Scr_Partition *p2) {
    Scr_Partition m;
    int *pair_keys;
    int i, j, n;

    n = p1->n_values;
    m.n_values = n;
    m.n_groups = 0;
    m.group_id = (int *)calloc((size_t)n, sizeof(int));
    m.group_sizes = (int *)calloc((size_t)n, sizeof(int));
    pair_keys = (int *)malloc((size_t)n * sizeof(int));

    for (i = 0; i < n; i++) {
        /* Encode (g1, g2) as g1 * max_g2 + g2 */
        pair_keys[i] = p1->group_id[i] * p2->n_groups + p2->group_id[i];
        m.group_id[i] = -1;
    }

    for (i = 0; i < n; i++) {
        if (m.group_id[i] >= 0) continue;
        m.group_id[i] = m.n_groups;
        for (j = i + 1; j < n; j++) {
            if (m.group_id[j] >= 0) continue;
            if (pair_keys[i] == pair_keys[j])
                m.group_id[j] = m.n_groups;
        }
        m.n_groups++;
    }

    for (i = 0; i < n; i++)
        m.group_sizes[m.group_id[i]]++;

    free(pair_keys);
    return m;
}

/* Join: finest partition that is coarser than BOTH p1 and p2.
 * Two elements are in the same join-class iff they are connected
 * by a chain where each link shares a class in p1 OR p2.
 * Implementation: union-find. */
static Scr_Partition scr_partition_join(const Scr_Partition *p1,
                                         const Scr_Partition *p2) {
    Scr_Partition j_part;
    int *parent;
    int *rank_arr;
    int i, k, n;

    n = p1->n_values;

    /* Union-find init */
    parent = (int *)malloc((size_t)n * sizeof(int));
    rank_arr = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) parent[i] = i;

    /* Find with path compression */
    #define SCR_FIND(x) do { \
        int _r = (x); \
        while (parent[_r] != _r) _r = parent[_r]; \
        { int _c = (x); while (_c != _r) { int _n = parent[_c]; parent[_c] = _r; _c = _n; } } \
        (x) = _r; \
    } while(0)

    /* Union by rank */
    #define SCR_UNION(a, b) do { \
        int _a = (a), _b = (b); \
        SCR_FIND(_a); SCR_FIND(_b); \
        if (_a != _b) { \
            if (rank_arr[_a] < rank_arr[_b]) { int _t = _a; _a = _b; _b = _t; } \
            parent[_b] = _a; \
            if (rank_arr[_a] == rank_arr[_b]) rank_arr[_a]++; \
        } \
    } while(0)

    /* For each group in p1, union all members */
    for (k = 0; k < p1->n_groups; k++) {
        int first = -1;
        for (i = 0; i < n; i++) {
            if (p1->group_id[i] != k) continue;
            if (first == -1) { first = i; continue; }
            SCR_UNION(first, i);
        }
    }

    /* For each group in p2, union all members */
    for (k = 0; k < p2->n_groups; k++) {
        int first = -1;
        for (i = 0; i < n; i++) {
            if (p2->group_id[i] != k) continue;
            if (first == -1) { first = i; continue; }
            SCR_UNION(first, i);
        }
    }

    /* Flatten and assign group IDs */
    j_part.n_values = n;
    j_part.n_groups = 0;
    j_part.group_id = (int *)calloc((size_t)n, sizeof(int));
    j_part.group_sizes = (int *)calloc((size_t)n, sizeof(int));

    {
        int *root_to_group = (int *)malloc((size_t)n * sizeof(int));
        for (i = 0; i < n; i++) root_to_group[i] = -1;

        for (i = 0; i < n; i++) {
            int r = i;
            SCR_FIND(r);
            if (root_to_group[r] == -1)
                root_to_group[r] = j_part.n_groups++;
            j_part.group_id[i] = root_to_group[r];
        }

        for (i = 0; i < n; i++)
            j_part.group_sizes[j_part.group_id[i]]++;

        free(root_to_group);
    }

    #undef SCR_FIND
    #undef SCR_UNION

    free(parent);
    free(rank_arr);
    return j_part;
}

/* Meet of ALL k partitions */
static Scr_Partition scr_partition_meet_all(const Scr_Partition *parts, int k) {
    Scr_Partition result;
    int i;
    if (k == 0) {
        result.n_values = 0;
        result.n_groups = 0;
        result.group_id = NULL;
        result.group_sizes = NULL;
        return result;
    }
    result = scr_partition_meet(&parts[0], &parts[k > 1 ? 1 : 0]);
    for (i = 2; i < k; i++) {
        Scr_Partition next = scr_partition_meet(&result, &parts[i]);
        scr_partition_free(&result);
        result = next;
    }
    return result;
}

/* Join of ALL k partitions */
static Scr_Partition scr_partition_join_all(const Scr_Partition *parts, int k) {
    Scr_Partition result;
    int i;
    if (k == 0) {
        result.n_values = 0;
        result.n_groups = 0;
        result.group_id = NULL;
        result.group_sizes = NULL;
        return result;
    }
    result = scr_partition_join(&parts[0], &parts[k > 1 ? 1 : 0]);
    for (i = 2; i < k; i++) {
        Scr_Partition next = scr_partition_join(&result, &parts[i]);
        scr_partition_free(&result);
        result = next;
    }
    return result;
}

/* Compute full refinement DAG over k partitions.
 * result[i*k + j] = 1 if partition i refines partition j.
 * Caller provides result array of size k*k. */
static void scr_partition_refinement_dag(const Scr_Partition *parts,
                                          int k, int *result) {
    int i, j;
    for (i = 0; i < k * k; i++) result[i] = 0;
    for (i = 0; i < k; i++) {
        result[i * k + i] = 1;  /* reflexive */
        for (j = 0; j < k; j++) {
            if (i == j) continue;
            result[i * k + j] = scr_partition_refines(&parts[i], &parts[j]);
        }
    }
}

/* Detect redundant partitions: partition i is redundant if there exists
 * some other partition j such that they are equal (same grouping).
 * redundans[i] = 1 if redundant (duplicate of an earlier partition).
 * Caller provides redundans array of size k. */
static void scr_partition_find_duplicates(const Scr_Partition *parts,
                                           int k, int *redundans) {
    int i, j;
    for (i = 0; i < k; i++) redundans[i] = 0;
    for (i = 1; i < k; i++) {
        for (j = 0; j < i; j++) {
            if (redundans[j]) continue;
            if (scr_partition_equal(&parts[i], &parts[j])) {
                redundans[i] = 1;
                break;
            }
        }
    }
}

/* ================================================================
 * Scr_PairHist -- Pairwise analysis histogram
 *
 * Dynamic bin count (max SCR_MAX_BINS).
 * ================================================================ */

#define SCR_MAX_BINS 16

typedef struct {
    int bins[SCR_MAX_BINS];
    int n_bins;       /* actual number of bins in use */
    int n_pairs;
    long total;
} Scr_PairHist;

static Scr_PairHist scr_pair_hist_make(int n_bins) {
    Scr_PairHist h;
    memset(&h, 0, sizeof(h));
    h.n_bins = n_bins > SCR_MAX_BINS ? SCR_MAX_BINS : n_bins;
    return h;
}

/* ================================================================
 * Scr_EdgeList -- Edge list for graph construction
 * ================================================================ */

typedef struct {
    int *src;
    int *dst;
    int count;
    int capacity;
    const char *titulis;
    int directus;   /* 0 = undirected, 1 = directed */
} Scr_EdgeList;

static void scr_el_init(Scr_EdgeList *el, int cap,
                          const char *titulis, int dir) {
    if (cap < 1) cap = 1;
    el->capacity = cap;
    el->count = 0;
    el->src = (int *)malloc((size_t)cap * sizeof(int));
    el->dst = (int *)malloc((size_t)cap * sizeof(int));
    el->titulis = titulis;
    el->directus = dir;
}

static void scr_el_free(Scr_EdgeList *el) {
    free(el->src);
    free(el->dst);
    el->src = NULL;
    el->dst = NULL;
    el->count = 0;
}

static void scr_el_add(Scr_EdgeList *el, int s, int d) {
    if (el->count >= el->capacity) {
        int new_cap = el->capacity * 2;
        int *ns = (int *)realloc(el->src, (size_t)new_cap * sizeof(int));
        int *nd = (int *)realloc(el->dst, (size_t)new_cap * sizeof(int));
        if (!ns || !nd) return;
        el->src = ns;
        el->dst = nd;
        el->capacity = new_cap;
    }
    el->src[el->count] = s;
    el->dst[el->count] = d;
    el->count++;
}

/* Generate undirected edges from a partition (connect all within-group pairs) */
static Scr_EdgeList scr_edges_from_partition(const Scr_Partition *p,
                                               const char *titulis) {
    Scr_EdgeList el;
    int max_e = scr_partition_related_pairs(p);
    int i, j;
    scr_el_init(&el, max_e > 0 ? max_e : 1, titulis, 0);
    for (i = 0; i < p->n_values; i++)
        for (j = i + 1; j < p->n_values; j++)
            if (p->group_id[i] == p->group_id[j])
                scr_el_add(&el, i, j);
    return el;
}

/* Restrict edge list to vertices in partition group g, re-indexing to 0..size-1 */
static Scr_EdgeList scr_el_restrict(const Scr_EdgeList *el,
                                      const Scr_Partition *p,
                                      int group,
                                      int *n_verts_out) {
    Scr_EdgeList res;
    int *remap;
    int new_n = 0;
    int i;
    remap = (int *)malloc((size_t)p->n_values * sizeof(int));
    for (i = 0; i < p->n_values; i++) {
        if (p->group_id[i] == group)
            remap[i] = new_n++;
        else
            remap[i] = -1;
    }
    scr_el_init(&res, el->count > 0 ? el->count : 1,
                el->titulis, el->directus);
    for (i = 0; i < el->count; i++) {
        if (remap[el->src[i]] >= 0 && remap[el->dst[i]] >= 0)
            scr_el_add(&res, remap[el->src[i]], remap[el->dst[i]]);
    }
    *n_verts_out = new_n;
    free(remap);
    return res;
}

/* Intersect two edge lists (same vertex set, size n) */
static Scr_EdgeList scr_el_intersect(const Scr_EdgeList *a,
                                       const Scr_EdgeList *b,
                                       int n) {
    Scr_EdgeList res;
    unsigned char *adj_b;
    int i;
    adj_b = (unsigned char *)calloc((size_t)n * (size_t)n, 1);
    for (i = 0; i < b->count; i++) {
        adj_b[b->src[i] * n + b->dst[i]] = 1;
        if (!b->directus)
            adj_b[b->dst[i] * n + b->src[i]] = 1;
    }
    scr_el_init(&res, a->count > 0 ? a->count : 1, "intersect",
                a->directus || b->directus);
    for (i = 0; i < a->count; i++) {
        if (adj_b[a->src[i] * n + a->dst[i]])
            scr_el_add(&res, a->src[i], a->dst[i]);
    }
    free(adj_b);
    return res;
}

/* Quotient graph: collapse partition classes to super-vertices */
static Scr_EdgeList scr_el_quotient(const Scr_EdgeList *el,
                                      const Scr_Partition *p,
                                      int *n_groups_out) {
    Scr_EdgeList res;
    int ng = p->n_groups;
    unsigned char *seen;
    int i;
    *n_groups_out = ng;
    seen = (unsigned char *)calloc((size_t)ng * (size_t)ng, 1);
    scr_el_init(&res, ng * ng > 0 ? ng * ng : 1,
                "quotient", el->directus);
    for (i = 0; i < el->count; i++) {
        int gs = p->group_id[el->src[i]];
        int gd = p->group_id[el->dst[i]];
        if (gs == gd) continue;
        if (!el->directus && gs > gd) { int t = gs; gs = gd; gd = t; }
        if (!seen[gs * ng + gd]) {
            seen[gs * ng + gd] = 1;
            scr_el_add(&res, gs, gd);
        }
    }
    free(seen);
    return res;
}

/* ================================================================
 * Scr_GraphResult -- Full graph analysis result
 * ================================================================ */

typedef struct {
    int n_vertices;
    int n_edges;

    /* Connectivity */
    int n_components;
    int *component_sizes;    /* sorted descending */
    int n_isolated;

    /* Bipartite (undirected only) */
    int est_bipartitus;      /* 1/0, -1 if not checked */
    int bipartitio_a;
    int bipartitio_b;

    /* Structure recognition (undirected only) */
    int est_completus;
    int est_arbor;
    int est_via;
    int est_cyclus;
    int ambitus;             /* girth */

    /* Directed only */
    int est_dag;
    int longitudo_catena;

    /* Degree stats */
    int gradus_min;
    int gradus_max;
    int gradus_summa;
    int est_regularis;
    int gradus_regularis;

    /* Degree histogram */
    int *gradus_hist;
    int gradus_hist_len;

    /* Edge density (permille, avoids floats) */
    int densitas_pm;

    /* Diameter (undirected, connected only) */
    int diameter;
} Scr_GraphResult;

static void scr_graph_result_free(Scr_GraphResult *gr) {
    free(gr->component_sizes);
    gr->component_sizes = NULL;
    free(gr->gradus_hist);
    gr->gradus_hist = NULL;
}

/* Sort helper for component sizes (descending) */
static int scr_cmp_int_desc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia > ib) return -1;
    if (ia < ib) return 1;
    return 0;
}

/* Core graph analyzer */
static Scr_GraphResult scr_graph_analyze(const Scr_EdgeList *el, int n) {
    Scr_GraphResult gr;
    unsigned char *adj = NULL;
    int *degree = NULL;
    int *bfs_deg = NULL;
    int *color = NULL;
    int *queue = NULL;
    int *comp_tmp = NULL;
    int nc, iso, bip;
    int n_active, n_ntcomp;
    int own_bfs_deg = 0;
    int i, j;

    memset(&gr, 0, sizeof(gr));
    gr.n_vertices = n;
    gr.n_edges = el->count;
    gr.est_bipartitus = -1;
    gr.est_dag = -1;
    gr.longitudo_catena = -1;
    gr.gradus_min = -1;
    gr.gradus_max = -1;
    gr.est_regularis = 0;
    gr.gradus_regularis = -1;
    gr.gradus_hist = NULL;
    gr.gradus_hist_len = 0;
    gr.densitas_pm = -1;
    gr.diameter = -1;

    if (n == 0) {
        gr.component_sizes = (int *)calloc(1, sizeof(int));
        return gr;
    }

    if (el->count == 0) {
        gr.n_components = n;
        gr.n_isolated = n;
        gr.component_sizes = (int *)malloc((size_t)n * sizeof(int));
        for (i = 0; i < n; i++) gr.component_sizes[i] = 1;
        if (!el->directus) {
            gr.est_bipartitus = 1;
            gr.bipartitio_a = n;
            gr.est_completus = (n <= 1) ? 1 : 0;
            gr.est_arbor = 1;
            gr.est_via = (n <= 1) ? 1 : 0;
            gr.est_cyclus = 0;
        } else {
            gr.est_dag = 1;
            gr.longitudo_catena = 0;
        }
        return gr;
    }

    /* Build adjacency matrix */
    adj = (unsigned char *)calloc((size_t)n * (size_t)n, 1);
    for (i = 0; i < el->count; i++) {
        adj[el->src[i] * n + el->dst[i]] = 1;
        if (!el->directus)
            adj[el->dst[i] * n + el->src[i]] = 1;
    }

    /* Out-degree */
    degree = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (adj[i * n + j]) degree[i]++;

    /* For directed: undirected degree for BFS */
    if (el->directus) {
        bfs_deg = (int *)calloc((size_t)n, sizeof(int));
        own_bfs_deg = 1;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (adj[i * n + j] || adj[j * n + i]) bfs_deg[i]++;
    } else {
        bfs_deg = degree;
    }

    /* BFS: components + bipartite */
    color = (int *)malloc((size_t)n * sizeof(int));
    queue = (int *)malloc((size_t)n * sizeof(int));
    comp_tmp = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) color[i] = -1;
    nc = 0; iso = 0; bip = 1;
    gr.bipartitio_a = 0; gr.bipartitio_b = 0;

    for (i = 0; i < n; i++) {
        int qh, qt, comp_a, comp_b;
        if (color[i] >= 0) continue;

        if (bfs_deg[i] == 0) {
            color[i] = 0;
            comp_tmp[nc] = 1;
            iso++;
            nc++;
            continue;
        }

        color[i] = 0;
        comp_a = 1; comp_b = 0;
        qh = 0; qt = 0;
        queue[qt++] = i;
        while (qh < qt) {
            int u = queue[qh++];
            int v;
            for (v = 0; v < n; v++) {
                int has_edge = adj[u * n + v];
                if (el->directus) has_edge |= adj[v * n + u];
                if (!has_edge) continue;
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    if (color[v] == 0) comp_a++;
                    else comp_b++;
                    queue[qt++] = v;
                } else if (!el->directus && color[v] == color[u]) {
                    bip = 0;
                }
            }
        }
        comp_tmp[nc] = comp_a + comp_b;
        gr.bipartitio_a += comp_a;
        gr.bipartitio_b += comp_b;
        nc++;
    }

    gr.n_components = nc;
    gr.n_isolated = iso;
    gr.component_sizes = (int *)malloc((size_t)nc * sizeof(int));
    memcpy(gr.component_sizes, comp_tmp, (size_t)nc * sizeof(int));
    qsort(gr.component_sizes, (size_t)nc, sizeof(int), scr_cmp_int_desc);

    /* Degree stats */
    {
        int dmin = n + 1, dmax = 0, dsum = 0;
        for (i = 0; i < n; i++) {
            if (bfs_deg[i] == 0) continue;
            if (degree[i] < dmin) dmin = degree[i];
            if (degree[i] > dmax) dmax = degree[i];
            dsum += degree[i];
        }
        if (dmax > 0) {
            gr.gradus_min = dmin;
            gr.gradus_max = dmax;
            gr.gradus_summa = dsum;
            gr.est_regularis = (dmin == dmax) ? 1 : 0;
            gr.gradus_regularis = (dmin == dmax) ? dmin : -1;
        }
    }

    /* Degree histogram */
    if (gr.gradus_max >= 0) {
        gr.gradus_hist_len = gr.gradus_max + 1;
        gr.gradus_hist = (int *)calloc((size_t)gr.gradus_hist_len, sizeof(int));
        for (i = 0; i < n; i++) {
            if (bfs_deg[i] == 0) continue;
            gr.gradus_hist[degree[i]]++;
        }
    }

    n_active = n - iso;
    n_ntcomp = nc - iso;

    /* Edge density (permille) */
    if (n_active >= 2) {
        long max_edges = (long)n_active * (long)(n_active - 1);
        if (!el->directus) max_edges /= 2;
        gr.densitas_pm = (int)((long)el->count * 1000L / max_edges);
    }

    if (!el->directus) {
        /* Undirected analyses */
        gr.est_bipartitus = bip;
        if (!bip) { gr.bipartitio_a = 0; gr.bipartitio_b = 0; }

        gr.est_completus = (n_active >= 2 &&
            el->count == n_active * (n_active - 1) / 2) ? 1 :
            (n_active <= 1) ? 1 : 0;

        gr.est_arbor = (n_active == 0) ? 1 :
            (el->count == n_active - n_ntcomp) ? 1 : 0;

        gr.est_via = (gr.est_arbor && n_ntcomp <= 1 &&
                      (gr.gradus_max <= 2 || gr.gradus_max == -1)) ? 1 : 0;

        gr.est_cyclus = (n_ntcomp == 1 && n_active >= 3 &&
                         gr.gradus_min == 2 && gr.gradus_max == 2) ? 1 : 0;

        /* Girth */
        gr.ambitus = 0;
        if (!gr.est_arbor && n_active >= 3) {
            int min_cycle = n + 1;
            int *dist = (int *)malloc((size_t)n * sizeof(int));
            int *par = (int *)malloc((size_t)n * sizeof(int));

            for (i = 0; i < n; i++) {
                int qi, qt2;
                if (degree[i] == 0) continue;
                for (j = 0; j < n; j++) { dist[j] = -1; par[j] = -1; }
                dist[i] = 0;
                qi = 0; qt2 = 0;
                queue[qt2++] = i;
                while (qi < qt2) {
                    int u = queue[qi++];
                    int v;
                    for (v = 0; v < n; v++) {
                        if (!adj[u * n + v]) continue;
                        if (dist[v] == -1) {
                            dist[v] = dist[u] + 1;
                            par[v] = u;
                            queue[qt2++] = v;
                        } else if (v != par[u]) {
                            int cl = dist[u] + dist[v] + 1;
                            if (cl < min_cycle) min_cycle = cl;
                        }
                    }
                }
                if (min_cycle == 3) break;
            }
            if (min_cycle <= n) gr.ambitus = min_cycle;
            free(dist);
            free(par);
        }

        /* Diameter */
        if (n_ntcomp == 1 && n_active >= 2) {
            int max_sp = 0;
            int *dist2 = (int *)malloc((size_t)n * sizeof(int));
            for (i = 0; i < n; i++) {
                int qi2, qt2;
                if (degree[i] == 0) continue;
                for (j = 0; j < n; j++) dist2[j] = -1;
                dist2[i] = 0;
                qi2 = 0; qt2 = 0;
                queue[qt2++] = i;
                while (qi2 < qt2) {
                    int u = queue[qi2++];
                    int v;
                    for (v = 0; v < n; v++) {
                        if (!adj[u * n + v] || dist2[v] >= 0) continue;
                        dist2[v] = dist2[u] + 1;
                        if (dist2[v] > max_sp) max_sp = dist2[v];
                        queue[qt2++] = v;
                    }
                }
            }
            gr.diameter = max_sp;
            free(dist2);
        }
    } else {
        /* Directed: DAG + longest chain */
        {
            int *in_deg = (int *)calloc((size_t)n, sizeof(int));
            int *dp = (int *)calloc((size_t)n, sizeof(int));
            int topo_count, qi, qt2, max_chain;

            for (i = 0; i < el->count; i++)
                in_deg[el->dst[i]]++;

            qi = 0; qt2 = 0;
            for (i = 0; i < n; i++)
                if (in_deg[i] == 0) queue[qt2++] = i;

            topo_count = 0;
            while (qi < qt2) {
                int u = queue[qi++];
                topo_count++;
                for (j = 0; j < n; j++) {
                    if (!adj[u * n + j]) continue;
                    if (dp[u] + 1 > dp[j]) dp[j] = dp[u] + 1;
                    in_deg[j]--;
                    if (in_deg[j] == 0) queue[qt2++] = j;
                }
            }

            gr.est_dag = (topo_count == n) ? 1 : 0;
            if (gr.est_dag) {
                max_chain = 0;
                for (i = 0; i < n; i++)
                    if (dp[i] > max_chain) max_chain = dp[i];
                gr.longitudo_catena = max_chain;
            }
            free(in_deg);
            free(dp);
        }
    }

    free(adj);
    free(degree);
    if (own_bfs_deg) free(bfs_deg);
    free(color);
    free(queue);
    free(comp_tmp);
    return gr;
}

/* Degree sequence comparison */
static int scr_degree_seq_match(const Scr_EdgeList *a, int na,
                                  const Scr_EdgeList *b, int nb) {
    int *da, *db;
    int i, match;
    if (na != nb || a->count != b->count) return 0;
    if (na == 0) return 1;
    da = (int *)calloc((size_t)na, sizeof(int));
    db = (int *)calloc((size_t)nb, sizeof(int));
    for (i = 0; i < a->count; i++) {
        da[a->src[i]]++;
        if (!a->directus) da[a->dst[i]]++;
    }
    for (i = 0; i < b->count; i++) {
        db[b->src[i]]++;
        if (!b->directus) db[b->dst[i]]++;
    }
    qsort(da, (size_t)na, sizeof(int), scr_cmp_int_desc);
    qsort(db, (size_t)nb, sizeof(int), scr_cmp_int_desc);
    match = 1;
    for (i = 0; i < na; i++) {
        if (da[i] != db[i]) { match = 0; break; }
    }
    free(da); free(db);
    return match;
}

/* ================================================================
 * Generic Detector Scaffolding
 *
 * Four patterns for building partitions/histograms from algebraic
 * criteria, without the detector needing to manage memory or loops.
 *
 * Values are passed as a contiguous void* array with element size
 * given by 'magnitudo'. Element i is at (char*)values + i * magnitudo.
 * ================================================================ */

#define SCR_ELEM(values, i, mag) ((const void *)((const char *)(values) + (size_t)(i) * (size_t)(mag)))

/* --- Internal hash table for classifier-based partitioning --- */

typedef struct {
    unsigned long *keys;
    int *first_idx;      /* first value index with this hash-class */
    int *chain;          /* chain of value indices with same class */
    int *bucket_head;
    int bucket_count;
    int n_classes;
} Scr_ClassTable;

static void scr_ct_init(Scr_ClassTable *ct, int capacity) {
    int i;
    int primes[] = {127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749};
    int np = 9;
    ct->bucket_count = primes[np - 1];
    for (i = 0; i < np; i++) {
        if (primes[i] > 2 * capacity) {
            ct->bucket_count = primes[i];
            break;
        }
    }
    ct->keys = (unsigned long *)calloc((size_t)capacity, sizeof(unsigned long));
    ct->first_idx = (int *)malloc((size_t)capacity * sizeof(int));
    ct->chain = (int *)malloc((size_t)capacity * sizeof(int));
    ct->bucket_head = (int *)malloc((size_t)ct->bucket_count * sizeof(int));
    for (i = 0; i < capacity; i++) { ct->first_idx[i] = -1; ct->chain[i] = -1; }
    for (i = 0; i < ct->bucket_count; i++) ct->bucket_head[i] = -1;
    ct->n_classes = 0;
}

static void scr_ct_free(Scr_ClassTable *ct) {
    free(ct->keys);
    free(ct->first_idx);
    free(ct->chain);
    free(ct->bucket_head);
}

/* ----------------------------------------------------------------
 * Scaffolding 1: Classifier-based partitioning — O(n)
 *
 * For detectors where each element can be independently classified:
 * norm classes, axis classes, 2-adic valuation classes, etc.
 *
 * classis_hash(value, config) -> hash of the classification key
 * classis_eq(a, b, config) -> do a and b have the same classification?
 *
 * The partition groups all elements with equal classification.
 * ---------------------------------------------------------------- */

static Scr_Partition scr_partitio_a_classe(
    const void *values, int n, unsigned long magnitudo,
    unsigned long (*classis_hash)(const void *value, const void *config),
    int (*classis_eq)(const void *a, const void *b, const void *config),
    const void *config)
{
    Scr_Partition p;
    Scr_ClassTable ct;
    int i;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = NULL;

    if (n == 0) {
        p.group_sizes = (int *)calloc(1, sizeof(int));
        return p;
    }

    scr_ct_init(&ct, n);

    for (i = 0; i < n; i++) {
        const void *elem = SCR_ELEM(values, i, magnitudo);
        unsigned long h = classis_hash(elem, config);
        int bucket = (int)(h % (unsigned long)ct.bucket_count);
        int cls = ct.bucket_head[bucket];
        int found = -1;

        /* Search for existing class with same hash and equal classification */
        while (cls >= 0) {
            if (ct.keys[cls] == h &&
                classis_eq(SCR_ELEM(values, ct.first_idx[cls], magnitudo),
                           elem, config)) {
                found = cls;
                break;
            }
            cls = ct.chain[cls];
        }

        if (found >= 0) {
            p.group_id[i] = found;
        } else {
            /* New class */
            int new_cls = ct.n_classes++;
            ct.keys[new_cls] = h;
            ct.first_idx[new_cls] = i;
            ct.chain[new_cls] = ct.bucket_head[bucket];
            ct.bucket_head[bucket] = new_cls;
            p.group_id[i] = new_cls;
        }
    }

    p.n_groups = ct.n_classes;
    p.group_sizes = (int *)calloc((size_t)p.n_groups, sizeof(int));
    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    scr_ct_free(&ct);
    return p;
}

/* ----------------------------------------------------------------
 * Scaffolding 2: Generator-based partitioning — O(n * k)
 *
 * For detectors where equivalence is defined by a group action:
 * unit orbits, Galois orbits, etc.
 *
 * For each element x, we generate k equivalent elements via
 * genera(result, x, gen_index, config). If any generated element
 * exists in the value set, x and that element are in the same group.
 *
 * Requires a hash-set for O(1) lookup. Uses union-find internally.
 *
 * aequalis(a, b) -> are elements equal?
 * dispersio(a) -> hash of element
 * n_generators -> how many generators (e.g., 8 for Z[zeta_8] units)
 * genera(result, a, k, config) -> write k-th equivalent of a into result
 * ---------------------------------------------------------------- */

static Scr_Partition scr_partitio_ab_generatoribus(
    const void *values, int n, unsigned long magnitudo,
    int (*aequalis)(const void *a, const void *b),
    unsigned long (*dispersio)(const void *a),
    int n_generators,
    void (*genera)(void *result, const void *a, int gen_index,
                   const void *config),
    const void *config)
{
    Scr_Partition p;
    int *parent;
    int *rank_arr;
    int *hash_head;
    int *hash_next;
    int hash_size;
    void *temp;
    int i, k;

    /* Hash table primes */
    int primes[] = {127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749};
    int np = 9;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = NULL;

    if (n == 0) {
        p.group_sizes = (int *)calloc(1, sizeof(int));
        return p;
    }

    /* Build hash table of values for O(1) lookup */
    hash_size = primes[np - 1];
    for (i = 0; i < np; i++) {
        if (primes[i] > 2 * n) { hash_size = primes[i]; break; }
    }
    hash_head = (int *)malloc((size_t)hash_size * sizeof(int));
    hash_next = (int *)malloc((size_t)n * sizeof(int));
    for (i = 0; i < hash_size; i++) hash_head[i] = -1;
    for (i = 0; i < n; i++) {
        int bucket = (int)(dispersio(SCR_ELEM(values, i, magnitudo))
                           % (unsigned long)hash_size);
        hash_next[i] = hash_head[bucket];
        hash_head[bucket] = i;
    }

    /* Union-find */
    parent = (int *)malloc((size_t)n * sizeof(int));
    rank_arr = (int *)calloc((size_t)n, sizeof(int));
    for (i = 0; i < n; i++) parent[i] = i;

    temp = malloc((size_t)magnitudo);

    /* For each element, generate all equivalents and union */
    for (i = 0; i < n; i++) {
        const void *elem = SCR_ELEM(values, i, magnitudo);
        for (k = 0; k < n_generators; k++) {
            int bucket, idx;
            genera(temp, elem, k, config);
            bucket = (int)(dispersio(temp) % (unsigned long)hash_size);
            idx = hash_head[bucket];
            while (idx >= 0) {
                if (aequalis(SCR_ELEM(values, idx, magnitudo), temp)) {
                    /* Union i and idx */
                    int ri = i, rj = idx;
                    while (parent[ri] != ri) ri = parent[ri];
                    while (parent[rj] != rj) rj = parent[rj];
                    if (ri != rj) {
                        if (rank_arr[ri] < rank_arr[rj])
                            { int t = ri; ri = rj; rj = t; }
                        parent[rj] = ri;
                        if (rank_arr[ri] == rank_arr[rj]) rank_arr[ri]++;
                    }
                    break;
                }
                idx = hash_next[idx];
            }
        }
    }

    /* Flatten union-find into partition */
    {
        int *root_to_group = (int *)malloc((size_t)n * sizeof(int));
        for (i = 0; i < n; i++) root_to_group[i] = -1;
        for (i = 0; i < n; i++) {
            int r = i;
            while (parent[r] != r) r = parent[r];
            /* Path compression */
            { int c = i; while (c != r) { int nx = parent[c]; parent[c] = r; c = nx; } }
            if (root_to_group[r] == -1)
                root_to_group[r] = p.n_groups++;
            p.group_id[i] = root_to_group[r];
        }
        free(root_to_group);
    }

    p.group_sizes = (int *)calloc((size_t)p.n_groups, sizeof(int));
    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    free(temp);
    free(parent);
    free(rank_arr);
    free(hash_head);
    free(hash_next);
    return p;
}

/* ----------------------------------------------------------------
 * Scaffolding 3: Pairwise predicate partitioning — O(n^2)
 *
 * Fallback for detectors that can only express equivalence as a
 * binary predicate with no shortcut.
 *
 * praedicatum(a, b, config) -> 1 if a and b are equivalent
 * ---------------------------------------------------------------- */

static Scr_Partition scr_partitio_a_praedicato(
    const void *values, int n, unsigned long magnitudo,
    int (*praedicatum)(const void *a, const void *b, const void *config),
    const void *config)
{
    Scr_Partition p;
    int i, j;

    p.n_values = n;
    p.n_groups = 0;
    p.group_id = (int *)calloc((size_t)n, sizeof(int));
    p.group_sizes = NULL;

    if (n == 0) {
        p.group_sizes = (int *)calloc(1, sizeof(int));
        return p;
    }

    for (i = 0; i < n; i++) p.group_id[i] = -1;

    for (i = 0; i < n; i++) {
        if (p.group_id[i] >= 0) continue;
        p.group_id[i] = p.n_groups;
        for (j = i + 1; j < n; j++) {
            if (p.group_id[j] >= 0) continue;
            if (praedicatum(SCR_ELEM(values, i, magnitudo),
                            SCR_ELEM(values, j, magnitudo),
                            config))
                p.group_id[j] = p.n_groups;
        }
        p.n_groups++;
    }

    p.group_sizes = (int *)calloc((size_t)p.n_groups, sizeof(int));
    for (i = 0; i < n; i++)
        p.group_sizes[p.group_id[i]]++;

    return p;
}

/* ----------------------------------------------------------------
 * Scaffolding 4: Pairwise distance histogram — O(n^2)
 *
 * For detectors that compute a distance/category between pairs.
 *
 * distantia(a, b, config) -> integer distance in [0, n_bins-1]
 * ---------------------------------------------------------------- */

static Scr_PairHist scr_parhist_a_distantia(
    const void *values, int n, unsigned long magnitudo,
    int (*distantia)(const void *a, const void *b, const void *config),
    int n_bins,
    const void *config)
{
    Scr_PairHist h;
    int i, j;

    h = scr_pair_hist_make(n_bins);
    h.n_pairs = n * (n - 1) / 2;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++) {
            int d = distantia(SCR_ELEM(values, i, magnitudo),
                              SCR_ELEM(values, j, magnitudo),
                              config);
            if (d >= 0 && d < h.n_bins) {
                h.bins[d]++;
                h.total += d;
            }
        }

    return h;
}

/* ----------------------------------------------------------------
 * Scaffolding 5: Unary-operation edge generation — O(n * |ops|)
 *
 * For edge generators based on a unary operation (Galois action,
 * power map, negation): edge i→j if op(values[i]) == values[j].
 *
 * Requires hash-set for O(1) lookup.
 * ---------------------------------------------------------------- */

static Scr_EdgeList scr_edges_ab_operatione(
    const void *values, int n, unsigned long magnitudo,
    int (*aequalis)(const void *a, const void *b),
    unsigned long (*dispersio)(const void *a),
    void (*operatio)(void *result, const void *a, const void *config),
    const void *config,
    const char *titulis,
    int directus)
{
    Scr_EdgeList el;
    int *hash_head, *hash_next;
    int hash_size;
    void *temp;
    int i;

    int primes[] = {127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749};
    int np = 9;

    scr_el_init(&el, n > 1 ? n : 1, titulis, directus);

    if (n == 0) return el;

    /* Build hash table */
    hash_size = primes[np - 1];
    for (i = 0; i < np; i++) {
        if (primes[i] > 2 * n) { hash_size = primes[i]; break; }
    }
    hash_head = (int *)malloc((size_t)hash_size * sizeof(int));
    hash_next = (int *)malloc((size_t)n * sizeof(int));
    for (i = 0; i < hash_size; i++) hash_head[i] = -1;
    for (i = 0; i < n; i++) {
        int bucket = (int)(dispersio(SCR_ELEM(values, i, magnitudo))
                           % (unsigned long)hash_size);
        hash_next[i] = hash_head[bucket];
        hash_head[bucket] = i;
    }

    temp = malloc((size_t)magnitudo);

    for (i = 0; i < n; i++) {
        int bucket, idx;
        operatio(temp, SCR_ELEM(values, i, magnitudo), config);
        bucket = (int)(dispersio(temp) % (unsigned long)hash_size);
        idx = hash_head[bucket];
        while (idx >= 0) {
            if (idx != i &&
                aequalis(SCR_ELEM(values, idx, magnitudo), temp)) {
                if (!directus && idx < i) break;  /* avoid duplicate undirected */
                scr_el_add(&el, i, idx);
                break;
            }
            idx = hash_next[idx];
        }
    }

    free(temp);
    free(hash_head);
    free(hash_next);
    return el;
}

/* ----------------------------------------------------------------
 * Scaffolding 6: Binary-operation closure edge generation — O(n^2)
 *
 * Edge i—j if op(values[i], values[j]) is in the value set.
 * Requires hash-set for O(1) lookup.
 * ---------------------------------------------------------------- */

static Scr_EdgeList scr_edges_ab_operatione_binaria(
    const void *values, int n, unsigned long magnitudo,
    int (*aequalis)(const void *a, const void *b),
    unsigned long (*dispersio)(const void *a),
    void (*operatio)(void *result, const void *a, const void *b,
                     const void *config),
    const void *config,
    const char *titulis,
    int directus)
{
    Scr_EdgeList el;
    int *hash_head, *hash_next;
    int hash_size;
    void *temp;
    int i, j;

    int primes[] = {127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749};
    int np = 9;

    scr_el_init(&el, n > 1 ? n * 4 : 1, titulis, directus);

    if (n < 2) return el;

    /* Build hash table */
    hash_size = primes[np - 1];
    for (i = 0; i < np; i++) {
        if (primes[i] > 2 * n) { hash_size = primes[i]; break; }
    }
    hash_head = (int *)malloc((size_t)hash_size * sizeof(int));
    hash_next = (int *)malloc((size_t)n * sizeof(int));
    for (i = 0; i < hash_size; i++) hash_head[i] = -1;
    for (i = 0; i < n; i++) {
        int bucket = (int)(dispersio(SCR_ELEM(values, i, magnitudo))
                           % (unsigned long)hash_size);
        hash_next[i] = hash_head[bucket];
        hash_head[bucket] = i;
    }

    temp = malloc((size_t)magnitudo);

    for (i = 0; i < n; i++) {
        int start_j = directus ? 0 : i + 1;
        for (j = start_j; j < n; j++) {
            int bucket, idx;
            if (i == j) continue;
            operatio(temp,
                     SCR_ELEM(values, i, magnitudo),
                     SCR_ELEM(values, j, magnitudo),
                     config);
            bucket = (int)(dispersio(temp) % (unsigned long)hash_size);
            idx = hash_head[bucket];
            while (idx >= 0) {
                if (aequalis(SCR_ELEM(values, idx, magnitudo), temp)) {
                    scr_el_add(&el, i, j);
                    break;
                }
                idx = hash_next[idx];
            }
        }
    }

    free(temp);
    free(hash_head);
    free(hash_next);
    return el;
}

/* ----------------------------------------------------------------
 * Scaffolding 7: Predicate-based edge generation — O(n^2)
 *
 * Edge i—j if praedicatum(values[i], values[j]) is true.
 * ---------------------------------------------------------------- */

static Scr_EdgeList scr_edges_a_praedicato(
    const void *values, int n, unsigned long magnitudo,
    int (*praedicatum)(const void *a, const void *b, const void *config),
    const void *config,
    const char *titulis,
    int directus)
{
    Scr_EdgeList el;
    int i, j;

    scr_el_init(&el, n > 1 ? n * 4 : 1, titulis, directus);

    for (i = 0; i < n; i++) {
        int start_j = directus ? 0 : i + 1;
        for (j = start_j; j < n; j++) {
            if (i == j) continue;
            if (praedicatum(SCR_ELEM(values, i, magnitudo),
                            SCR_ELEM(values, j, magnitudo),
                            config))
                scr_el_add(&el, i, j);
        }
    }

    return el;
}

/* ================================================================
 * Scr_Genus -- Algebraic type vtable
 *
 * Function pointer table describing an algebraic type's capabilities.
 * NULL pointers indicate missing capabilities; detectors that need
 * a missing capability are automatically skipped.
 *
 * All function pointers use void* for values and a genus-specific
 * config pointer (typically NULL or a pointer to extra type data).
 * ================================================================ */

#define SCR_MAX_INDAGATORES 32

typedef struct {
    const char *titulis;              /* type name, e.g. "Z[i]" */
    unsigned long magnitudo;          /* sizeof(element) */

    /* --- Identity --- */
    int (*aequalis)(const void *a, const void *b);
    unsigned long (*dispersio)(const void *a);

    /* --- Ring operations --- */
    void (*nega)(void *r, const void *a, const void *config);
    void (*adde)(void *r, const void *a, const void *b, const void *config);
    void (*multiplica)(void *r, const void *a, const void *b,
                       const void *config);
    void (*conjuga)(void *r, const void *a, const void *config);

    /* --- Norm (opaque: hash+eq, not the value) --- */
    unsigned long (*norma_dispersio)(const void *a, const void *config);
    int (*norma_aequalis)(const void *a, const void *b, const void *config);

    /* --- Unit group generators --- */
    int unitas_numerus;               /* |U| = number of units */
    void (*unitas_generator)(void *r, const void *a, int k,
                              const void *config);

    /* --- Galois automorphism generators --- */
    int automorphismus_numerus;       /* |Gal| = number of automorphisms */
    void (*automorphismus_generator)(void *r, const void *a, int k,
                                      const void *config);

    /* --- Component access (optional) --- */
    int partes_numerus;               /* number of basis components */
    long (*pars)(const void *a, int k, const void *config);

    /* --- Axis classifier (optional) --- */
    int (*axis_classis)(const void *a, const void *config);

    /* --- Config pointer for all callbacks --- */
    const void *config;
} Scr_Genus;

/* ================================================================
 * Wrapper Macros -- Reduce adoption boilerplate
 *
 * These macros generate the void*-casting wrapper functions that
 * connect typed library functions to the Genus vtable.
 *
 * Each macro produces a single static function with the correct
 * signature for the corresponding Genus field.
 * ================================================================ */

/* --- Identity wrappers ---
 * SCR_WRAP_EQ:   int fn(T,T)   -> int name(void*,void*)
 * SCR_WRAP_HASH: ulong fn(T)   -> ulong name(void*)  */
#define SCR_WRAP_EQ(name, T, fn) \
    static int name(const void *a_, const void *b_) { \
        return fn(*(const T *)a_, *(const T *)b_); \
    }

#define SCR_WRAP_HASH(name, T, fn) \
    static unsigned long name(const void *a_) { \
        return fn(*(const T *)a_); \
    }

/* --- Ring operation wrappers ---
 * SCR_WRAP_UNOP:  T fn(T)    -> void name(void*,void*,void*)
 * SCR_WRAP_BINOP: T fn(T,T)  -> void name(void*,void*,void*,void*) */
#define SCR_WRAP_UNOP(name, T, fn) \
    static void name(void *r_, const void *a_, const void *c_) { \
        (void)c_; *(T *)r_ = fn(*(const T *)a_); \
    }

#define SCR_WRAP_BINOP(name, T, fn) \
    static void name(void *r_, const void *a_, const void *b_, \
                     const void *c_) { \
        (void)c_; \
        *(T *)r_ = fn(*(const T *)a_, *(const T *)b_); \
    }

/* --- Classifier pair wrappers (for partition detectors) ---
 * SCR_WRAP_CLASSIS_EQ:        fn(T)->scalar, compare with ==
 * SCR_WRAP_CLASSIS_HASH_LONG: fn(T)->long,  FNV-style hash
 * SCR_WRAP_CLASSIS_HASH_INT:  fn(T)->int,   shifted hash   */
#define SCR_WRAP_CLASSIS_EQ(name, T, fn) \
    static int name(const void *a_, const void *b_, \
                    const void *c_) { \
        (void)c_; \
        return fn(*(const T *)a_) == fn(*(const T *)b_); \
    }

#define SCR_WRAP_CLASSIS_HASH_LONG(name, T, fn) \
    static unsigned long name(const void *a_, const void *c_) { \
        (void)c_; \
        return (unsigned long)(fn(*(const T *)a_) + 1000000L) \
               * 2654435761UL; \
    }

#define SCR_WRAP_CLASSIS_HASH_INT(name, T, fn) \
    static unsigned long name(const void *a_, const void *c_) { \
        (void)c_; \
        return (unsigned long)(fn(*(const T *)a_) + 10); \
    }

/* --- Generator wrappers ---
 * SCR_WRAP_GEN_ITER:    apply step_fn k times (unit groups)
 * SCR_WRAP_GEN_GALOIS2: k=0 identity, k>0 fn  (|Gal|=2)
 * SCR_WRAP_GEN_GALOIS4: k=0..3 via identity,f1,f2,f3 (|Gal|=4) */
#define SCR_WRAP_GEN_ITER(name, T, step_fn) \
    static void name(void *r_, const void *a_, int k_, \
                     const void *c_) { \
        T val_ = *(const T *)a_; \
        int j_; \
        (void)c_; \
        for (j_ = 0; j_ < k_; j_++) val_ = step_fn(val_); \
        *(T *)r_ = val_; \
    }

#define SCR_WRAP_GEN_GALOIS2(name, T, fn) \
    static void name(void *r_, const void *a_, int k_, \
                     const void *c_) { \
        (void)c_; \
        if (k_ == 0) *(T *)r_ = *(const T *)a_; \
        else         *(T *)r_ = fn(*(const T *)a_); \
    }

#define SCR_WRAP_GEN_GALOIS4(name, T, fn1, fn2, fn3) \
    static void name(void *r_, const void *a_, int k_, \
                     const void *c_) { \
        (void)c_; \
        switch (k_) { \
        case 0:  *(T *)r_ = *(const T *)a_; break; \
        case 1:  *(T *)r_ = fn1(*(const T *)a_); break; \
        case 2:  *(T *)r_ = fn2(*(const T *)a_); break; \
        case 3:  *(T *)r_ = fn3(*(const T *)a_); break; \
        default: *(T *)r_ = *(const T *)a_; break; \
        } \
    }

/* --- Component access wrappers ---
 * SCR_WRAP_PARS2: 2-component type, members m0,m1 -> long
 * SCR_WRAP_PARS4: 4-component type, members m0..m3 -> long */
#define SCR_WRAP_PARS2(name, T, m0, m1) \
    static long name(const void *a_, int k_, const void *c_) { \
        const T *z_ = (const T *)a_; \
        (void)c_; \
        return k_ == 0 ? z_->m0 : z_->m1; \
    }

#define SCR_WRAP_PARS4(name, T, m0, m1, m2, m3) \
    static long name(const void *a_, int k_, const void *c_) { \
        const T *z_ = (const T *)a_; \
        (void)c_; \
        switch (k_) { \
        case 0: return z_->m0; case 1: return z_->m1; \
        case 2: return z_->m2; case 3: return z_->m3; \
        default: return 0; \
        } \
    }

/* --- Axis classifier wrapper ---
 * SCR_WRAP_AXIS: fn(T)->int -> int name(void*,void*) */
#define SCR_WRAP_AXIS(name, T, fn) \
    static int name(const void *a_, const void *c_) { \
        (void)c_; return fn(*(const T *)a_); \
    }

/* ================================================================
 * Scr_NormaPartialis_Config -- Partial norm detector config
 *
 * For each non-identity automorphism sigma_k, the partial norm is
 * N_k(z) = z * sigma_k(z), landing in Fix(sigma_k).
 *
 * These configs are stored in Scr_Registrum so pointers survive
 * past registration.
 * ================================================================ */

#define SCR_MAX_NORMAE_PARTIALES 16

typedef struct {
    const Scr_Genus *genus;
    int automorphismus_k;
} Scr_NormaPartialis_Config;

/* ================================================================
 * Scr_Indagator -- Detector descriptor
 *
 * Describes one detector: its name, what kind of output it produces,
 * the function pointers it needs, and prerequisites from the Genus.
 *
 * Three detector kinds:
 *   SCR_IND_PARTITIO  -- produces a Scr_Partition
 *   SCR_IND_HISTOGRAM -- produces a Scr_PairHist
 *   SCR_IND_ACIES     -- produces a Scr_EdgeList
 * ================================================================ */

enum {
    SCR_IND_PARTITIO  = 0,
    SCR_IND_HISTOGRAM = 1,
    SCR_IND_ACIES     = 2
};

/* Prerequisite flags -- which Genus fields must be non-NULL */
enum {
    SCR_REQ_NIHIL          = 0,
    SCR_REQ_NORMA          = (1 << 0),  /* norma_dispersio + norma_aequalis */
    SCR_REQ_UNITATES       = (1 << 1),  /* unitas_generator + unitas_numerus */
    SCR_REQ_AUTOMORPHISMI  = (1 << 2),  /* automorphismus_generator + numerus */
    SCR_REQ_NEGA           = (1 << 3),
    SCR_REQ_ADDE           = (1 << 4),
    SCR_REQ_MULTIPLICA     = (1 << 5),
    SCR_REQ_CONJUGA        = (1 << 6),
    SCR_REQ_PARTES         = (1 << 7),  /* pars + partes_numerus */
    SCR_REQ_AXIS           = (1 << 8)   /* axis_classis */
};

typedef struct {
    const char *titulis;          /* detector name */
    int genus_indagatoris;        /* SCR_IND_PARTITIO / HISTOGRAM / ACIES */
    unsigned int requisita;       /* bitmask of SCR_REQ_* */

    /* --- Partition detector callbacks --- */
    /* Exactly one of these three strategies is used: */

    /* Strategy 1: classifier (hash + eq) */
    unsigned long (*classis_dispersio)(const void *a, const void *config);
    int (*classis_aequalis)(const void *a, const void *b, const void *config);

    /* Strategy 2: generators (k generators, union-find) */
    int generatorum_numerus;
    void (*generator)(void *r, const void *a, int k, const void *config);

    /* Strategy 3: pairwise predicate */
    int (*praedicatum)(const void *a, const void *b, const void *config);

    /* --- Histogram detector callbacks --- */
    int (*distantia)(const void *a, const void *b, const void *config);
    int receptacula_numerus;      /* n_bins */

    /* --- Edge detector callbacks --- */
    /* Unary operation */
    void (*operatio_unaria)(void *r, const void *a, const void *config);
    /* Binary operation */
    void (*operatio_binaria)(void *r, const void *a, const void *b,
                             const void *config);
    int directus;                 /* directed edges? */

    /* --- Config for detector-specific data --- */
    const void *config;
} Scr_Indagator;

/* ================================================================
 * Scr_Registrum -- Detector registry
 *
 * Holds a list of registered detectors. Up to SCR_MAX_INDAGATORES.
 * ================================================================ */

typedef struct {
    Scr_Indagator indagatores[SCR_MAX_INDAGATORES];
    int numerus;

    /* Partial norm config storage (lifetime tied to registry) */
    Scr_NormaPartialis_Config normae_partiales[SCR_MAX_NORMAE_PARTIALES];
    int numerus_normarum_partialium;
} Scr_Registrum;

static void scr_registrum_init(Scr_Registrum *reg) {
    reg->numerus = 0;
    reg->numerus_normarum_partialium = 0;
}

static int scr_registrum_adde(Scr_Registrum *reg,
                               const Scr_Indagator *ind) {
    if (reg->numerus >= SCR_MAX_INDAGATORES) return 0;
    reg->indagatores[reg->numerus] = *ind;
    reg->numerus++;
    return 1;
}

/* Check if a genus satisfies a detector's prerequisites */
static int scr_genus_satisfacit(const Scr_Genus *g, unsigned int req) {
    if ((req & SCR_REQ_NORMA) &&
        (!g->norma_dispersio || !g->norma_aequalis))
        return 0;
    if ((req & SCR_REQ_UNITATES) &&
        (!g->unitas_generator || g->unitas_numerus <= 0))
        return 0;
    if ((req & SCR_REQ_AUTOMORPHISMI) &&
        (!g->automorphismus_generator || g->automorphismus_numerus <= 0))
        return 0;
    if ((req & SCR_REQ_NEGA) && !g->nega) return 0;
    if ((req & SCR_REQ_ADDE) && !g->adde) return 0;
    if ((req & SCR_REQ_MULTIPLICA) && !g->multiplica) return 0;
    if ((req & SCR_REQ_CONJUGA) && !g->conjuga) return 0;
    if ((req & SCR_REQ_PARTES) &&
        (!g->pars || g->partes_numerus <= 0))
        return 0;
    if ((req & SCR_REQ_AXIS) && !g->axis_classis) return 0;
    return 1;
}

/* ================================================================
 * True algebraic norm (norma vera)
 *
 * For a number field K/Q with Galois group {sigma_0=id, sigma_1, ...},
 * the true norm is N(z) = sigma_0(z) * sigma_1(z) * ... * sigma_{n-1}(z).
 *
 * This is always in the base field (rational integers) and is
 * Galois-invariant by construction: N(sigma(z)) = N(z) for all sigma.
 *
 * Computed using only existing Genus capabilities:
 *   automorphismus_generator + multiplica + dispersio/aequalis
 *
 * Stack buffer limit: elements up to SCR_NORMA_VERA_MAX bytes.
 * ================================================================ */

#define SCR_NORMA_VERA_MAX 128

/* Aligned stack buffer for temporary element storage */
typedef union {
    long align_l;
    double align_d;
    void *align_p;
    char data[SCR_NORMA_VERA_MAX];
} Scr_ElemBuf;

/* Compute the true norm of element a into the buffer 'out'.
 * config must point to a Scr_Genus.
 * Returns 1 on success, 0 if element too large.
 */
static int scr_norma_vera_computa(void *out, const void *a,
                                   const Scr_Genus *g)
{
    Scr_ElemBuf image_buf, temp_buf;
    int k;

    if (g->magnitudo > SCR_NORMA_VERA_MAX) return 0;

    /* product = sigma_0(a) = a */
    memcpy(out, a, (size_t)g->magnitudo);

    /* product *= sigma_k(a) for k = 1..n-1 */
    for (k = 1; k < g->automorphismus_numerus; k++) {
        g->automorphismus_generator(image_buf.data, a, k, g->config);
        g->multiplica(temp_buf.data, out, image_buf.data, g->config);
        memcpy(out, temp_buf.data, (size_t)g->magnitudo);
    }

    return 1;
}

/* Classifier hash: compute true norm, hash the result */
static unsigned long scr_norma_vera_dispersio(const void *a,
                                               const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf norm_buf;

    scr_norma_vera_computa(norm_buf.data, a, g);
    return g->dispersio(norm_buf.data);
}

/* Classifier eq: compute true norms of both, compare */
static int scr_norma_vera_aequalis(const void *a, const void *b,
                                    const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf norm_a, norm_b;

    scr_norma_vera_computa(norm_a.data, a, g);
    scr_norma_vera_computa(norm_b.data, b, g);
    return g->aequalis(norm_a.data, norm_b.data);
}

/* ================================================================
 * True trace (vestigium verum)
 *
 * For a number field K/Q with Galois group {sigma_0=id, sigma_1, ...},
 * the true trace is Tr(z) = sigma_0(z) + sigma_1(z) + ... + sigma_{n-1}(z).
 *
 * This is always in the base field (rational integers) and is
 * Galois-invariant by construction: Tr(sigma(z)) = Tr(z) for all sigma.
 *
 * Additive analog of the true norm. Uses adde instead of multiplica.
 * ================================================================ */

/* Compute the true trace of element a into the buffer 'out'. */
static int scr_vestigium_verum_computa(void *out, const void *a,
                                        const Scr_Genus *g)
{
    Scr_ElemBuf image_buf, temp_buf;
    int k;

    if (g->magnitudo > SCR_NORMA_VERA_MAX) return 0;

    /* sum = sigma_0(a) = a */
    memcpy(out, a, (size_t)g->magnitudo);

    /* sum += sigma_k(a) for k = 1..n-1 */
    for (k = 1; k < g->automorphismus_numerus; k++) {
        g->automorphismus_generator(image_buf.data, a, k, g->config);
        g->adde(temp_buf.data, out, image_buf.data, g->config);
        memcpy(out, temp_buf.data, (size_t)g->magnitudo);
    }

    return 1;
}

/* Classifier hash: compute true trace, hash the result */
static unsigned long scr_vestigium_verum_dispersio(const void *a,
                                                    const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf buf;

    scr_vestigium_verum_computa(buf.data, a, g);
    return g->dispersio(buf.data);
}

/* Classifier eq: compute true traces of both, compare */
static int scr_vestigium_verum_aequalis(const void *a, const void *b,
                                         const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf ta, tb;

    scr_vestigium_verum_computa(ta.data, a, g);
    scr_vestigium_verum_computa(tb.data, b, g);
    return g->aequalis(ta.data, tb.data);
}

/* ================================================================
 * Partial norms (normae partiales)
 *
 * For each non-identity automorphism sigma_k in the Galois group,
 * the partial norm is: N_k(z) = z * sigma_k(z)
 *
 * This value lies in Fix(sigma_k), the subfield fixed by sigma_k.
 * Together, the partial norms reveal the full Galois correspondence:
 * the lattice of subfields between Q and K.
 *
 * Structural theorems:
 *   - galois_orbits refines each partial_norm (Galois action)
 *   - each partial_norm refines true_norm (norm towers)
 *   - partial_norms for incomparable subgroups don't refine each other
 *
 * For Z[i]:     1 partial norm (= true_norm = |z|^2)
 * For Z[zeta_8]: 3 partial norms -> Q(i*sqrt(2)), Q(i), Q(sqrt(2))
 * ================================================================ */

/* Detector name table for partial norms (k=1..15) */
static const char * const scr_np_nomina_[] = {
    "partial_norm_sigma_1",
    "partial_norm_sigma_2",
    "partial_norm_sigma_3",
    "partial_norm_sigma_4",
    "partial_norm_sigma_5",
    "partial_norm_sigma_6",
    "partial_norm_sigma_7",
    "partial_norm_sigma_8",
    "partial_norm_sigma_9",
    "partial_norm_sigma_10",
    "partial_norm_sigma_11",
    "partial_norm_sigma_12",
    "partial_norm_sigma_13",
    "partial_norm_sigma_14",
    "partial_norm_sigma_15"
};

/* Compute partial norm: N_k(z) = z * sigma_k(z) */
static int scr_norma_partialis_computa(void *out, const void *a,
                                        const Scr_Genus *g, int k)
{
    Scr_ElemBuf image_buf;

    if (g->magnitudo > SCR_NORMA_VERA_MAX) return 0;

    /* out = a * sigma_k(a) */
    g->automorphismus_generator(image_buf.data, a, k, g->config);
    g->multiplica(out, a, image_buf.data, g->config);

    return 1;
}

/* Classifier hash: compute partial norm, hash the result */
static unsigned long scr_norma_partialis_dispersio(const void *a,
                                                    const void *config)
{
    const Scr_NormaPartialis_Config *c =
        (const Scr_NormaPartialis_Config *)config;
    Scr_ElemBuf norm_buf;

    scr_norma_partialis_computa(norm_buf.data, a, c->genus,
                                 c->automorphismus_k);
    return c->genus->dispersio(norm_buf.data);
}

/* Classifier eq: compute partial norms of both, compare */
static int scr_norma_partialis_aequalis(const void *a, const void *b,
                                         const void *config)
{
    const Scr_NormaPartialis_Config *c =
        (const Scr_NormaPartialis_Config *)config;
    Scr_ElemBuf norm_a, norm_b;

    scr_norma_partialis_computa(norm_a.data, a, c->genus,
                                 c->automorphismus_k);
    scr_norma_partialis_computa(norm_b.data, b, c->genus,
                                 c->automorphismus_k);
    return c->genus->aequalis(norm_a.data, norm_b.data);
}

/* ================================================================
 * Partial traces (vestigia partialia)
 *
 * For each non-identity automorphism sigma_k in the Galois group,
 * the partial trace is: Tr_k(z) = z + sigma_k(z)
 *
 * This value lies in Fix(sigma_k), the subfield fixed by sigma_k.
 * Additive analog of partial norms. Same subfield correspondence.
 *
 * Structural theorems (parallel to partial norms):
 *   - each partial_trace refines true_trace (trace towers)
 *   - partial_traces for incomparable subgroups don't refine each other
 * ================================================================ */

/* Detector name table for partial traces (k=1..15) */
static const char * const scr_vp_nomina_[] = {
    "partial_trace_sigma_1",
    "partial_trace_sigma_2",
    "partial_trace_sigma_3",
    "partial_trace_sigma_4",
    "partial_trace_sigma_5",
    "partial_trace_sigma_6",
    "partial_trace_sigma_7",
    "partial_trace_sigma_8",
    "partial_trace_sigma_9",
    "partial_trace_sigma_10",
    "partial_trace_sigma_11",
    "partial_trace_sigma_12",
    "partial_trace_sigma_13",
    "partial_trace_sigma_14",
    "partial_trace_sigma_15"
};

/* Compute partial trace: Tr_k(z) = z + sigma_k(z) */
static int scr_vestigium_partiale_computa(void *out, const void *a,
                                           const Scr_Genus *g, int k)
{
    Scr_ElemBuf image_buf;

    if (g->magnitudo > SCR_NORMA_VERA_MAX) return 0;

    /* out = a + sigma_k(a) */
    g->automorphismus_generator(image_buf.data, a, k, g->config);
    g->adde(out, a, image_buf.data, g->config);

    return 1;
}

/* Classifier hash: compute partial trace, hash the result */
static unsigned long scr_vestigium_partiale_dispersio(const void *a,
                                                       const void *config)
{
    const Scr_NormaPartialis_Config *c =
        (const Scr_NormaPartialis_Config *)config;
    Scr_ElemBuf buf;

    scr_vestigium_partiale_computa(buf.data, a, c->genus,
                                    c->automorphismus_k);
    return c->genus->dispersio(buf.data);
}

/* Classifier eq: compute partial traces of both, compare */
static int scr_vestigium_partiale_aequalis(const void *a, const void *b,
                                            const void *config)
{
    const Scr_NormaPartialis_Config *c =
        (const Scr_NormaPartialis_Config *)config;
    Scr_ElemBuf ta, tb;

    scr_vestigium_partiale_computa(ta.data, a, c->genus,
                                    c->automorphismus_k);
    scr_vestigium_partiale_computa(tb.data, b, c->genus,
                                    c->automorphismus_k);
    return c->genus->aequalis(ta.data, tb.data);
}

/* ================================================================
 * Component-derived universal detectors
 *
 * These use the Genus.pars and Genus.axis_classis function pointers
 * to derive detectors generically, without type-specific wrappers.
 * ================================================================ */

/* axis_classes: partition by axis classifier */
static unsigned long scr_axis_dispersio(const void *a, const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    return (unsigned long)(g->axis_classis(a, g->config) + 10)
           * 2654435761UL;
}

static int scr_axis_aequalis(const void *a, const void *b,
                              const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    return g->axis_classis(a, g->config)
        == g->axis_classis(b, g->config);
}

/* sign_distances: ternary Hamming on component signs */
static int scr_signum_distantia(const void *a, const void *b,
                                 const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    int d = 0, k;
    for (k = 0; k < g->partes_numerus; k++) {
        long va = g->pars(a, k, g->config);
        long vb = g->pars(b, k, g->config);
        int sa = (va > 0) - (va < 0);
        int sb = (vb > 0) - (vb < 0);
        if (sa != sb) d++;
    }
    return d;
}

/* sum_zeros: count zero components of a+b */
static int scr_summa_nulla(const void *a, const void *b,
                            const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf buf;
    int count = 0, k;
    g->adde(buf.data, a, b, g->config);
    for (k = 0; k < g->partes_numerus; k++) {
        if (g->pars(buf.data, k, g->config) == 0) count++;
    }
    return count;
}

/* diff_zeros: count zero components of a-b */
static int scr_differentia_nulla(const void *a, const void *b,
                                  const void *config) {
    const Scr_Genus *g = (const Scr_Genus *)config;
    Scr_ElemBuf buf_neg, buf_diff;
    int count = 0, k;
    g->nega(buf_neg.data, b, g->config);
    g->adde(buf_diff.data, a, buf_neg.data, g->config);
    for (k = 0; k < g->partes_numerus; k++) {
        if (g->pars(buf_diff.data, k, g->config) == 0) count++;
    }
    return count;
}

/* ================================================================
 * scr_registra_universales -- Auto-register universal detectors
 *
 * Checks which Genus capabilities are available and registers
 * the corresponding standard detectors. Returns count registered.
 *
 * Universal partition detectors:
 *   - norm_classes       (needs norma_dispersio + norma_aequalis)
 *   - unit_orbits        (needs unitas_generator + unitas_numerus)
 *   - galois_orbits      (needs automorphismus_generator + numerus)
 *   - true_norm_classes   (needs automorphismi + multiplica)
 *   - true_trace_classes  (needs automorphismi + adde)
 *   - partial_norm_sigma_k  (one per non-identity auto, needs multiplica)
 *   - partial_trace_sigma_k (one per non-identity auto, needs adde)
 *   - axis_classes        (needs axis_classis)
 *
 * Universal histogram detectors:
 *   - sign_distances      (needs pars + partes_numerus)
 *   - sum_zeros           (needs pars + partes_numerus + adde)
 *   - diff_zeros          (needs pars + partes_numerus + adde + nega)
 *
 * Universal edge detectors:
 *   - negation_edges   (needs nega)
 *   - additive_closure (needs adde)
 *   - multiplicative_closure (needs multiplica)
 * ================================================================ */

static int scr_registra_universales(Scr_Registrum *reg,
                                     const Scr_Genus *genus) {
    Scr_Indagator ind;
    int count = 0;

    /* norm_classes */
    if (genus->norma_dispersio && genus->norma_aequalis) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "norm_classes";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_NORMA;
        ind.classis_dispersio = genus->norma_dispersio;
        ind.classis_aequalis = genus->norma_aequalis;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* unit_orbits */
    if (genus->unitas_generator && genus->unitas_numerus > 0) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "unit_orbits";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_UNITATES;
        ind.generatorum_numerus = genus->unitas_numerus;
        ind.generator = genus->unitas_generator;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* galois_orbits */
    if (genus->automorphismus_generator && genus->automorphismus_numerus > 0) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "galois_orbits";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_AUTOMORPHISMI;
        ind.generatorum_numerus = genus->automorphismus_numerus;
        ind.generator = genus->automorphismus_generator;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* true_norm_classes (algebraic norm = product over Galois orbit) */
    if (genus->automorphismus_generator && genus->automorphismus_numerus > 0
        && genus->multiplica
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "true_norm_classes";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_AUTOMORPHISMI | SCR_REQ_MULTIPLICA;
        ind.classis_dispersio = scr_norma_vera_dispersio;
        ind.classis_aequalis = scr_norma_vera_aequalis;
        ind.config = genus;  /* pass genus itself as config */
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* partial_norm_classes: one per non-identity automorphism.
     * N_k(z) = z * sigma_k(z), landing in Fix(sigma_k). */
    if (genus->automorphismus_generator && genus->automorphismus_numerus > 1
        && genus->multiplica
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        int k;
        for (k = 1; k < genus->automorphismus_numerus; k++) {
            Scr_NormaPartialis_Config *pc;
            if (reg->numerus_normarum_partialium >= SCR_MAX_NORMAE_PARTIALES)
                break;
            if (k - 1 >= 15) break; /* name table limit */

            pc = &reg->normae_partiales[reg->numerus_normarum_partialium];
            pc->genus = genus;
            pc->automorphismus_k = k;
            reg->numerus_normarum_partialium++;

            memset(&ind, 0, sizeof(ind));
            ind.titulis = scr_np_nomina_[k - 1];
            ind.genus_indagatoris = SCR_IND_PARTITIO;
            ind.requisita = SCR_REQ_AUTOMORPHISMI | SCR_REQ_MULTIPLICA;
            ind.classis_dispersio = scr_norma_partialis_dispersio;
            ind.classis_aequalis = scr_norma_partialis_aequalis;
            ind.config = pc;
            if (scr_registrum_adde(reg, &ind)) count++;
        }
    }

    /* true_trace_classes (trace = sum over Galois orbit) */
    if (genus->automorphismus_generator && genus->automorphismus_numerus > 0
        && genus->adde
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "true_trace_classes";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_AUTOMORPHISMI | SCR_REQ_ADDE;
        ind.classis_dispersio = scr_vestigium_verum_dispersio;
        ind.classis_aequalis = scr_vestigium_verum_aequalis;
        ind.config = genus;  /* pass genus itself as config */
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* partial_trace_classes: one per non-identity automorphism.
     * Tr_k(z) = z + sigma_k(z), landing in Fix(sigma_k). */
    if (genus->automorphismus_generator && genus->automorphismus_numerus > 1
        && genus->adde
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        int k;
        for (k = 1; k < genus->automorphismus_numerus; k++) {
            Scr_NormaPartialis_Config *pc;
            if (reg->numerus_normarum_partialium >= SCR_MAX_NORMAE_PARTIALES)
                break;
            if (k - 1 >= 15) break; /* name table limit */

            pc = &reg->normae_partiales[reg->numerus_normarum_partialium];
            pc->genus = genus;
            pc->automorphismus_k = k;
            reg->numerus_normarum_partialium++;

            memset(&ind, 0, sizeof(ind));
            ind.titulis = scr_vp_nomina_[k - 1];
            ind.genus_indagatoris = SCR_IND_PARTITIO;
            ind.requisita = SCR_REQ_AUTOMORPHISMI | SCR_REQ_ADDE;
            ind.classis_dispersio = scr_vestigium_partiale_dispersio;
            ind.classis_aequalis = scr_vestigium_partiale_aequalis;
            ind.config = pc;
            if (scr_registrum_adde(reg, &ind)) count++;
        }
    }

    /* negation_edges */
    if (genus->nega) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "negation_edges";
        ind.genus_indagatoris = SCR_IND_ACIES;
        ind.requisita = SCR_REQ_NEGA;
        ind.operatio_unaria = genus->nega;
        ind.directus = 0;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* additive_closure */
    if (genus->adde) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "additive_closure";
        ind.genus_indagatoris = SCR_IND_ACIES;
        ind.requisita = SCR_REQ_ADDE;
        ind.operatio_binaria = genus->adde;
        ind.directus = 0;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* multiplicative_closure */
    if (genus->multiplica) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "multiplicative_closure";
        ind.genus_indagatoris = SCR_IND_ACIES;
        ind.requisita = SCR_REQ_MULTIPLICA;
        ind.operatio_binaria = genus->multiplica;
        ind.directus = 0;
        ind.config = genus->config;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* axis_classes (from axis_classis function pointer) */
    if (genus->axis_classis) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "axis_classes";
        ind.genus_indagatoris = SCR_IND_PARTITIO;
        ind.requisita = SCR_REQ_AXIS;
        ind.classis_dispersio = scr_axis_dispersio;
        ind.classis_aequalis = scr_axis_aequalis;
        ind.config = genus;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* sign_distances (ternary Hamming on component signs) */
    if (genus->pars && genus->partes_numerus > 0) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "sign_distances";
        ind.genus_indagatoris = SCR_IND_HISTOGRAM;
        ind.requisita = SCR_REQ_PARTES;
        ind.distantia = scr_signum_distantia;
        ind.receptacula_numerus = genus->partes_numerus + 1;
        ind.config = genus;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* sum_zeros (zero count in a+b) */
    if (genus->pars && genus->partes_numerus > 0 && genus->adde
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "sum_zeros";
        ind.genus_indagatoris = SCR_IND_HISTOGRAM;
        ind.requisita = SCR_REQ_PARTES | SCR_REQ_ADDE;
        ind.distantia = scr_summa_nulla;
        ind.receptacula_numerus = genus->partes_numerus + 1;
        ind.config = genus;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    /* diff_zeros (zero count in a-b) */
    if (genus->pars && genus->partes_numerus > 0
        && genus->adde && genus->nega
        && genus->magnitudo <= SCR_NORMA_VERA_MAX) {
        memset(&ind, 0, sizeof(ind));
        ind.titulis = "diff_zeros";
        ind.genus_indagatoris = SCR_IND_HISTOGRAM;
        ind.requisita = SCR_REQ_PARTES | SCR_REQ_ADDE | SCR_REQ_NEGA;
        ind.distantia = scr_differentia_nulla;
        ind.receptacula_numerus = genus->partes_numerus + 1;
        ind.config = genus;
        if (scr_registrum_adde(reg, &ind)) count++;
    }

    return count;
}

/* ================================================================
 * Scr_Scrutinium -- Full analysis result
 *
 * Contains all detector results, the refinement DAG, duplicate
 * detection, and the meet-of-all partitions.
 * ================================================================ */

typedef struct {
    /* Detector results -- parallel arrays */
    const char *nomina[SCR_MAX_INDAGATORES];
    int genera[SCR_MAX_INDAGATORES];        /* SCR_IND_* kind */
    int activi[SCR_MAX_INDAGATORES];        /* 1 if ran, 0 if skipped */

    Scr_Partition partitiones[SCR_MAX_INDAGATORES];
    Scr_PairHist histogrammata[SCR_MAX_INDAGATORES];
    Scr_EdgeList acies[SCR_MAX_INDAGATORES];

    int numerus_totalis;            /* total registered */
    int numerus_activi;             /* total that ran */

    /* Partition-specific lattice results */
    int numerus_partitionum;        /* how many produced partitions */
    int *partitio_indices;          /* which detector indices are partitions */
    int *ordo;                      /* refinement DAG (k x k) */
    int *duplices;                  /* duplicate flags */
    Scr_Partition conventus;        /* meet of all partitions */
} Scr_Scrutinium;

static void scr_scrutinium_libera(Scr_Scrutinium *s) {
    int i;
    for (i = 0; i < s->numerus_totalis; i++) {
        if (!s->activi[i]) continue;
        switch (s->genera[i]) {
        case SCR_IND_PARTITIO:
            scr_partition_free(&s->partitiones[i]);
            break;
        case SCR_IND_ACIES:
            scr_el_free(&s->acies[i]);
            break;
        default:
            break;
        }
    }
    free(s->partitio_indices);
    free(s->ordo);
    free(s->duplices);
    scr_partition_free(&s->conventus);
}

/* ================================================================
 * scr_scrutare -- Run all applicable detectors
 *
 * The core pipeline:
 * 1. For each registered detector, check prerequisites against genus
 * 2. If satisfied, run the appropriate scaffolding function
 * 3. Collect all partition results, compute refinement DAG + meet
 *
 * Returns a Scr_Scrutinium with all results.
 * ================================================================ */

static Scr_Scrutinium scr_scrutare(const Scr_Registrum *reg,
                                    const Scr_Genus *genus,
                                    const void *values,
                                    int n)
{
    Scr_Scrutinium s;
    int i, pi;
    const Scr_Indagator *ind;

    memset(&s, 0, sizeof(s));
    s.numerus_totalis = reg->numerus;

    /* --- Pass 1: run all applicable detectors --- */
    for (i = 0; i < reg->numerus; i++) {
        ind = &reg->indagatores[i];
        s.nomina[i] = ind->titulis;
        s.genera[i] = ind->genus_indagatoris;

        if (!scr_genus_satisfacit(genus, ind->requisita)) {
            s.activi[i] = 0;
            continue;
        }

        s.activi[i] = 1;
        s.numerus_activi++;

        switch (ind->genus_indagatoris) {
        case SCR_IND_PARTITIO:
            if (ind->classis_dispersio && ind->classis_aequalis) {
                /* Classifier strategy */
                s.partitiones[i] = scr_partitio_a_classe(
                    values, n, genus->magnitudo,
                    ind->classis_dispersio, ind->classis_aequalis,
                    ind->config);
            } else if (ind->generator && ind->generatorum_numerus > 0) {
                /* Generator strategy */
                s.partitiones[i] = scr_partitio_ab_generatoribus(
                    values, n, genus->magnitudo,
                    genus->aequalis, genus->dispersio,
                    ind->generatorum_numerus, ind->generator,
                    ind->config);
            } else if (ind->praedicatum) {
                /* Pairwise predicate strategy */
                s.partitiones[i] = scr_partitio_a_praedicato(
                    values, n, genus->magnitudo,
                    ind->praedicatum, ind->config);
            } else {
                s.activi[i] = 0;
                s.numerus_activi--;
            }
            break;

        case SCR_IND_HISTOGRAM:
            if (ind->distantia && ind->receptacula_numerus > 0) {
                s.histogrammata[i] = scr_parhist_a_distantia(
                    values, n, genus->magnitudo,
                    ind->distantia, ind->receptacula_numerus,
                    ind->config);
            } else {
                s.activi[i] = 0;
                s.numerus_activi--;
            }
            break;

        case SCR_IND_ACIES:
            if (ind->operatio_unaria) {
                s.acies[i] = scr_edges_ab_operatione(
                    values, n, genus->magnitudo,
                    genus->aequalis, genus->dispersio,
                    ind->operatio_unaria,
                    ind->config, ind->titulis, ind->directus);
            } else if (ind->operatio_binaria) {
                s.acies[i] = scr_edges_ab_operatione_binaria(
                    values, n, genus->magnitudo,
                    genus->aequalis, genus->dispersio,
                    ind->operatio_binaria,
                    ind->config, ind->titulis, ind->directus);
            } else if (ind->praedicatum) {
                s.acies[i] = scr_edges_a_praedicato(
                    values, n, genus->magnitudo,
                    ind->praedicatum,
                    ind->config, ind->titulis, ind->directus);
            } else {
                s.activi[i] = 0;
                s.numerus_activi--;
            }
            break;

        default:
            s.activi[i] = 0;
            s.numerus_activi--;
            break;
        }
    }

    /* --- Pass 2: collect partition indices, compute lattice --- */
    s.numerus_partitionum = 0;
    for (i = 0; i < reg->numerus; i++) {
        if (s.activi[i] && s.genera[i] == SCR_IND_PARTITIO)
            s.numerus_partitionum++;
    }

    if (s.numerus_partitionum > 0) {
        Scr_Partition *parts_arr;
        int k = s.numerus_partitionum;

        s.partitio_indices = (int *)malloc((size_t)k * sizeof(int));
        s.ordo = (int *)calloc((size_t)(k * k), sizeof(int));
        s.duplices = (int *)calloc((size_t)k, sizeof(int));

        /* Build array of partition pointers for lattice ops */
        parts_arr = (Scr_Partition *)malloc((size_t)k * sizeof(Scr_Partition));
        pi = 0;
        for (i = 0; i < reg->numerus; i++) {
            if (s.activi[i] && s.genera[i] == SCR_IND_PARTITIO) {
                s.partitio_indices[pi] = i;
                parts_arr[pi] = s.partitiones[i];
                pi++;
            }
        }

        /* Refinement DAG */
        scr_partition_refinement_dag(parts_arr, k, s.ordo);

        /* Duplicate detection */
        scr_partition_find_duplicates(parts_arr, k, s.duplices);

        /* Meet of all partitions */
        s.conventus = scr_partition_meet_all(parts_arr, k);

        free(parts_arr);
    } else {
        s.partitio_indices = NULL;
        s.ordo = NULL;
        s.duplices = NULL;
        memset(&s.conventus, 0, sizeof(s.conventus));
    }

    return s;
}

/* ================================================================
 * Scr_Signatura -- Structural fingerprint of a partition lattice
 *
 * Extracted from a completed Scr_Scrutinium. Operates on the
 * DEDUPLICATED partition set (duplicates collapsed to one
 * representative). Characterizes the lattice topology.
 *
 * Altitude = longest chain (edges) in the reduced DAG.
 * Width = max antichain size (Dilworth, brute force for k <= 20).
 * V-shape = incomparable pair with a common lower bound.
 * Diamond = 3+ pairwise incomparable with common upper AND lower.
 * ================================================================ */

typedef struct {
    /* Partition profile (deduplicated) */
    int partitiones_numerus;      /* unique (non-dup) partition count */
    int *profila;                 /* n_groups for each unique partition */

    /* Refinement topology */
    int *ordo;                    /* reduced DAG (k x k) */
    int altitudo;                 /* longest chain (edges) */
    int latitudo;                 /* max antichain size */

    /* Structural counts */
    int duplices_numerus;         /* duplicate partitions removed */
    int incomparabiles_numerus;   /* incomparable pairs */
    int complementares_numerus;   /* pairs whose meet = discrete (n groups) */

    /* Topology classification */
    int est_catena;               /* 1 if total order (linear chain) */
    int est_V;                    /* 1 if incomparable pair + common lower bound */
    int est_diamas;               /* 1 if 3+ incomp + common upper + lower */
} Scr_Signatura;

static void scr_signatura_libera(Scr_Signatura *sig) {
    free(sig->profila);
    free(sig->ordo);
    sig->profila = NULL;
    sig->ordo = NULL;
}

static int scr_signatura_aequalis(const Scr_Signatura *a,
                                    const Scr_Signatura *b) {
    int i, k;
    int *sa, *sb;

    if (a->partitiones_numerus != b->partitiones_numerus) return 0;
    if (a->altitudo != b->altitudo) return 0;
    if (a->latitudo != b->latitudo) return 0;
    if (a->duplices_numerus != b->duplices_numerus) return 0;
    if (a->incomparabiles_numerus != b->incomparabiles_numerus) return 0;
    if (a->complementares_numerus != b->complementares_numerus) return 0;
    if (a->est_catena != b->est_catena) return 0;
    if (a->est_V != b->est_V) return 0;
    if (a->est_diamas != b->est_diamas) return 0;

    /* Compare sorted profiles (n_groups values) */
    k = a->partitiones_numerus;
    if (k == 0) return 1;

    sa = (int *)malloc((size_t)k * sizeof(int));
    sb = (int *)malloc((size_t)k * sizeof(int));
    memcpy(sa, a->profila, (size_t)k * sizeof(int));
    memcpy(sb, b->profila, (size_t)k * sizeof(int));

    /* Insertion sort (k is small) */
    for (i = 1; i < k; i++) {
        int v = sa[i], j = i;
        while (j > 0 && sa[j-1] > v) { sa[j] = sa[j-1]; j--; }
        sa[j] = v;
    }
    for (i = 1; i < k; i++) {
        int v = sb[i], j = i;
        while (j > 0 && sb[j-1] > v) { sb[j] = sb[j-1]; j--; }
        sb[j] = v;
    }

    for (i = 0; i < k; i++) {
        if (sa[i] != sb[i]) {
            free(sa); free(sb);
            return 0;
        }
    }
    free(sa); free(sb);
    return 1;
}

/* Extract signature from a completed Scrutinium */
static Scr_Signatura scr_signatura_ex(const Scr_Scrutinium *s) {
    Scr_Signatura sig;
    int k_total = s->numerus_partitionum;
    int *keep;    /* partition-array indices that are NOT duplicates */
    int k;        /* unique partition count */
    int i, j, m;
    int n_values = 0;

    memset(&sig, 0, sizeof(sig));
    if (k_total == 0) return sig;

    /* Step 1: identify non-duplicate partitions */
    keep = (int *)malloc((size_t)k_total * sizeof(int));
    k = 0;
    for (i = 0; i < k_total; i++) {
        if (s->duplices[i])
            sig.duplices_numerus++;
        else
            keep[k++] = i;
    }
    sig.partitiones_numerus = k;
    if (k == 0) { free(keep); return sig; }

    /* Get n_values from first partition */
    n_values = s->partitiones[s->partitio_indices[keep[0]]].n_values;

    /* Step 2: extract profiles */
    sig.profila = (int *)malloc((size_t)k * sizeof(int));
    for (i = 0; i < k; i++) {
        int fi = s->partitio_indices[keep[i]];
        sig.profila[i] = s->partitiones[fi].n_groups;
    }

    /* Step 3: extract reduced DAG */
    sig.ordo = (int *)calloc((size_t)(k * k), sizeof(int));
    for (i = 0; i < k; i++)
        for (j = 0; j < k; j++) {
            if (i == j) continue;
            sig.ordo[i * k + j] =
                s->ordo[keep[i] * k_total + keep[j]];
        }

    /* Step 4: altitude (longest chain in edges) via relaxation */
    {
        int *dp = (int *)calloc((size_t)k, sizeof(int));
        int changed;
        do {
            changed = 0;
            for (i = 0; i < k; i++)
                for (j = 0; j < k; j++) {
                    if (i == j) continue;
                    if (sig.ordo[i * k + j] && dp[j] < dp[i] + 1) {
                        dp[j] = dp[i] + 1;
                        changed = 1;
                    }
                }
        } while (changed);
        sig.altitudo = 0;
        for (i = 0; i < k; i++)
            if (dp[i] > sig.altitudo) sig.altitudo = dp[i];
        free(dp);
    }

    /* Step 5: incomparability matrix + count */
    {
        int *incomp = (int *)calloc((size_t)(k * k), sizeof(int));
        sig.incomparabiles_numerus = 0;
        for (i = 0; i < k; i++)
            for (j = i + 1; j < k; j++) {
                if (!sig.ordo[i * k + j] && !sig.ordo[j * k + i]) {
                    incomp[i * k + j] = 1;
                    incomp[j * k + i] = 1;
                    sig.incomparabiles_numerus++;
                }
            }

        /* Step 6: width (max antichain) by brute force, k <= 20 */
        sig.latitudo = (k > 0) ? 1 : 0;
        if (k <= 20) {
            unsigned long mask;
            unsigned long max_mask = (1UL << (unsigned)k) - 1UL;
            for (mask = 1; mask <= max_mask; mask++) {
                int bits[20];
                int count = 0;
                int is_ac = 1;
                unsigned long mt = mask;
                int a, b;

                /* Fast popcount */
                { unsigned long pc = mask;
                  while (pc) { count++; pc &= pc - 1UL; } }
                if (count <= sig.latitudo) continue;

                /* Extract bit positions */
                count = 0;
                for (a = 0; a < k && mt; a++) {
                    if (mt & 1UL) bits[count++] = a;
                    mt >>= 1;
                }

                /* Check antichain */
                for (a = 0; a < count && is_ac; a++)
                    for (b = a + 1; b < count && is_ac; b++)
                        if (sig.ordo[bits[a] * k + bits[b]] ||
                            sig.ordo[bits[b] * k + bits[a]])
                            is_ac = 0;

                if (is_ac && count > sig.latitudo)
                    sig.latitudo = count;
            }
        }

        /* Step 7: est_catena (total order) */
        sig.est_catena = 1;
        for (i = 0; i < k && sig.est_catena; i++)
            for (j = i + 1; j < k && sig.est_catena; j++)
                if (!sig.ordo[i * k + j] && !sig.ordo[j * k + i])
                    sig.est_catena = 0;

        /* Step 8: est_V (incomparable pair + common lower bound) */
        sig.est_V = 0;
        for (i = 0; i < k && !sig.est_V; i++)
            for (j = i + 1; j < k && !sig.est_V; j++) {
                int c;
                if (!incomp[i * k + j]) continue;
                for (c = 0; c < k; c++) {
                    if (c == i || c == j) continue;
                    if (sig.ordo[i * k + c] && sig.ordo[j * k + c]) {
                        sig.est_V = 1;
                        break;
                    }
                }
            }

        /* Step 9: est_diamas (3+ pairwise incomp + common upper + lower) */
        sig.est_diamas = 0;
        for (i = 0; i < k && !sig.est_diamas; i++)
            for (j = i + 1; j < k && !sig.est_diamas; j++) {
                if (!incomp[i * k + j]) continue;
                for (m = j + 1; m < k && !sig.est_diamas; m++) {
                    int top, bot, has_top = 0, has_bot = 0;
                    if (!incomp[i * k + m] || !incomp[j * k + m])
                        continue;
                    /* i,j,m pairwise incomparable — check bounds */
                    for (top = 0; top < k; top++) {
                        if (top == i || top == j || top == m) continue;
                        if (sig.ordo[top * k + i] &&
                            sig.ordo[top * k + j] &&
                            sig.ordo[top * k + m]) {
                            has_top = 1; break;
                        }
                    }
                    for (bot = 0; bot < k; bot++) {
                        if (bot == i || bot == j || bot == m) continue;
                        if (sig.ordo[i * k + bot] &&
                            sig.ordo[j * k + bot] &&
                            sig.ordo[m * k + bot]) {
                            has_bot = 1; break;
                        }
                    }
                    if (has_top && has_bot)
                        sig.est_diamas = 1;
                }
            }

        free(incomp);
    }

    /* Step 10: complementary pairs (meet of pair = discrete) */
    sig.complementares_numerus = 0;
    for (i = 0; i < k; i++)
        for (j = i + 1; j < k; j++) {
            int fi = s->partitio_indices[keep[i]];
            int fj = s->partitio_indices[keep[j]];
            Scr_Partition mt = scr_partition_meet(
                &s->partitiones[fi], &s->partitiones[fj]);
            if (mt.n_groups == n_values)
                sig.complementares_numerus++;
            scr_partition_free(&mt);
        }

    free(keep);
    return sig;
}

/* ================================================================
 * Print Helpers (require stdio.h before including this header)
 * ================================================================ */

#ifdef SCR_PRINT

static void scr_print_partition(const char *titulis, const Scr_Partition *p) {
    int max_sz = scr_partition_max_size(p);
    int related = scr_partition_related_pairs(p);
    int total_pairs = p->n_values * (p->n_values - 1) / 2;
    int sz, gi;

    printf("  %s: %d values, %d groups\n", titulis, p->n_values, p->n_groups);
    if (total_pairs > 0) {
        int pct = (int)((long)related * 100L / (long)total_pairs);
        printf("    Related pairs: %d / %d (%d%%)\n",
               related, total_pairs, pct);
    }
    printf("    Max group: %d, Min group: %d, Singletons: %d\n",
           max_sz, scr_partition_min_size(p),
           scr_partition_n_singletons(p));

    printf("    Size histogram:");
    for (sz = 1; sz <= max_sz; sz++) {
        int cnt = 0;
        for (gi = 0; gi < p->n_groups; gi++)
            if (p->group_sizes[gi] == sz) cnt++;
        if (cnt > 0)
            printf(" [%d]x%d", sz, cnt);
    }
    printf("\n");
}

static void scr_print_pair_hist(const char *titulis, const Scr_PairHist *h) {
    int i;
    printf("  %s: %d pairs, total=%ld\n", titulis, h->n_pairs, h->total);
    printf("    bins:");
    for (i = 0; i < h->n_bins; i++) printf(" [%d]=%d", i, h->bins[i]);
    printf("\n");
}

static void scr_print_graph_result(const char *titulis,
                                     const Scr_GraphResult *gr) {
    int i;
    printf("  %s: %d vertices, %d edges\n", titulis, gr->n_vertices, gr->n_edges);
    printf("    Components: %d (isolated: %d)\n", gr->n_components, gr->n_isolated);
    if (gr->n_components > 0 && gr->component_sizes) {
        int show = gr->n_components < 5 ? gr->n_components : 5;
        printf("    Largest components:");
        for (i = 0; i < show; i++) printf(" %d", gr->component_sizes[i]);
        if (gr->n_components > 5) printf(" ...");
        printf("\n");
    }
    if (gr->est_bipartitus >= 0)
        printf("    Bipartite: %s", gr->est_bipartitus ? "yes" : "no");
    if (gr->est_bipartitus == 1)
        printf(" (%d + %d)", gr->bipartitio_a, gr->bipartitio_b);
    if (gr->est_bipartitus >= 0) printf("\n");
    if (gr->gradus_min >= 0)
        printf("    Degree: min=%d max=%d regular=%s\n",
               gr->gradus_min, gr->gradus_max,
               gr->est_regularis ? "yes" : "no");
    if (gr->densitas_pm >= 0)
        printf("    Density: %d permille\n", gr->densitas_pm);
    if (!gr->est_bipartitus && gr->est_completus >= 0)
        printf("    Complete: %s\n", gr->est_completus ? "yes" : "no");
    if (gr->est_arbor >= 0)
        printf("    Tree/forest: %s\n", gr->est_arbor ? "yes" : "no");
    if (gr->ambitus > 0)
        printf("    Girth: %d\n", gr->ambitus);
    if (gr->diameter >= 0)
        printf("    Diameter: %d\n", gr->diameter);
    if (gr->est_dag >= 0)
        printf("    DAG: %s\n", gr->est_dag ? "yes" : "no");
    if (gr->longitudo_catena >= 0)
        printf("    Longest chain: %d\n", gr->longitudo_catena);
}

static void scr_print_refinement_dag(const char **nomina,
                                       const int *dag,
                                       int k) {
    int i, j;
    printf("  Refinement DAG (%d partitions):\n", k);
    for (i = 0; i < k; i++) {
        for (j = 0; j < k; j++) {
            if (i == j) continue;
            if (dag[i * k + j])
                printf("    %s -> %s\n", nomina[i], nomina[j]);
        }
    }
}

static void scr_signatura_imprime(const Scr_Signatura *sig) {
    int i;
    printf("  --- Structural Signature ---\n");
    printf("  Partitions: %d unique (+ %d duplicates removed)\n",
           sig->partitiones_numerus, sig->duplices_numerus);

    printf("  Profile (n_groups):");
    for (i = 0; i < sig->partitiones_numerus; i++)
        printf(" %d", sig->profila[i]);
    printf("\n");

    printf("  Altitude: %d, Width: %d\n",
           sig->altitudo, sig->latitudo);
    printf("  Incomparable pairs: %d, Complementary pairs: %d\n",
           sig->incomparabiles_numerus, sig->complementares_numerus);

    printf("  Topology:");
    if (sig->est_catena) printf(" CHAIN");
    if (sig->est_V) printf(" V-SHAPE");
    if (sig->est_diamas) printf(" DIAMOND");
    if (!sig->est_catena && !sig->est_V && !sig->est_diamas)
        printf(" (trivial)");
    printf("\n");
}

static void scr_print_scrutinium(const Scr_Scrutinium *s) {
    int i, j, k;

    printf("=== Scrutinium: %d detectors ran (%d registered, %d skipped) ===\n",
           s->numerus_activi, s->numerus_totalis,
           s->numerus_totalis - s->numerus_activi);

    /* Print each active detector result */
    for (i = 0; i < s->numerus_totalis; i++) {
        if (!s->activi[i]) {
            printf("  [SKIP] %s\n", s->nomina[i]);
            continue;
        }
        switch (s->genera[i]) {
        case SCR_IND_PARTITIO:
            scr_print_partition(s->nomina[i], &s->partitiones[i]);
            break;
        case SCR_IND_HISTOGRAM:
            scr_print_pair_hist(s->nomina[i], &s->histogrammata[i]);
            break;
        case SCR_IND_ACIES:
            printf("  %s: %d edges\n", s->nomina[i], s->acies[i].count);
            break;
        default:
            break;
        }
    }

    /* Print partition lattice if we have partitions */
    k = s->numerus_partitionum;
    if (k > 0) {
        const char *pnames[SCR_MAX_INDAGATORES];

        printf("\n  --- Partition Lattice (%d partitions) ---\n", k);

        for (i = 0; i < k; i++)
            pnames[i] = s->nomina[s->partitio_indices[i]];

        /* Refinement relations */
        printf("  Refinement:\n");
        for (i = 0; i < k; i++)
            for (j = 0; j < k; j++) {
                if (i == j) continue;
                if (s->ordo[i * k + j])
                    printf("    %s -> %s\n", pnames[i], pnames[j]);
            }

        /* Duplicates */
        printf("  Duplicates:");
        {
            int any = 0;
            for (i = 0; i < k; i++)
                if (s->duplices[i]) { printf(" %s", pnames[i]); any = 1; }
            if (!any) printf(" (none)");
        }
        printf("\n");

        /* Meet */
        if (s->conventus.n_values > 0)
            printf("  Meet of all: %d groups (of %d values)\n",
                   s->conventus.n_groups, s->conventus.n_values);
    }

    printf("=== /Scrutinium ===\n");
}

#endif /* SCR_PRINT */

#endif /* SCRUTINIUM_H */
