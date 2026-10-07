/* tabula_nodorum.h - Tabula nodorum primorum (usque ad X transitus) et
 * agnitio per polynomia Alexander et Jones
 *
 * FONS: KnotInfo (C. Livingston, A. H. Moore, "KnotInfo: Table of Knot
 * Invariants", knotinfo.math.indiana.edu), per instantaneum
 * database_knotinfo 2026.10.5 (github.com/soehms/database_knotinfo).
 * Ex eo SOLUM nomina, numeri transituum, symmetria et codices PD
 * sumuntur (probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv); polynomia
 * Alexander et Jones per laqueus ex codicibus PD COMPUTANTUR (cum
 * columnis KnotInfo in extractione collata).
 *
 * CHIRALITAS: nomen "K" = diagramma tabulae (codex PD KnotInfo); "K*" =
 * eius speculum. Conventio KnotInfo = conventio physica laqueus: 3_1
 * tabulae est trifolium DEXTRUM (Jones -t^4 + t^3 + t). Knot Atlas
 * diagramma alterum pro quibusdam nodis pingit (3_1 sinistrum) -
 * tabulae chiralitate per nodum differunt; nomina hic semper ad codicem
 * PD referuntur.
 *
 * AGNITIO: congruentia Alexander ET Jones - non probatio typi (5_1
 * et 10_132* utrumque communicant: ambo redduntur). Nodus chiralis
 * cuius Jones symmetricus est (9_42, 10_48, 10_71, 10_91, 10_104,
 * 10_125) BIS redditur, K et K*: Jones chiralitatem non videt.
 * Compositi DUORUM nodorum non trivialium tabulae (Alexander et Jones
 * multiplicativi) quoque quaeruntur, speculo cuiusque factoris, sed
 * orientatione summandorum neglecta (K1 # K2 et K1 # rev K2 hic idem).
 *
 * USUS:
 *   TabulaNodorum* t = tabula_nodorum_aperire(piscina);
 *   Agnitio a[VIII];
 *   i32 n = tabula_nodorum_agnoscere(t, alexander, jones, piscina, a,
 *       VIII);
 *   ... a[0].primus->titulus, a[0].primus_speculum ...
 */
/* <aedilis corpus="lib/tabula_nodorum.c"/> */
/* <aedilis corpus="lib/tabula_nodorum_data.c"/> */
#ifndef TABULA_NODORUM_H
#define TABULA_NODORUM_H

#include "latina.h"
#include "piscina.h"
#include "polynomium.h"

/* symmetria (KnotInfo "symmetry type"): NULLA = nodus trivialis;
 * REVERSIBILIS = invertibilis, chiralis; CHIRALIS = nec invertibilis
 * nec amphichiralis */
#define TABULA_NODORUM_NULLA                  ZEPHYRUM
#define TABULA_NODORUM_REVERSIBILIS           I
#define TABULA_NODORUM_CHIRALIS               II
#define TABULA_NODORUM_AMPHICHIRALIS_PLENA    III
#define TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA IV
#define TABULA_NODORUM_AMPHICHIRALIS_POSITIVA V

nomen structura {
    constans character* titulus;      /* "3_1", "10_132" */
                   i32  transitus;    /* numerus transituum */
                   i32  symmetria;    /* TABULA_NODORUM_* */
                   i32  initium_pd;   /* in TABULA_NODORUM_PD */
    constans character* alexander;    /* forma normalis laqueus */
    constans character* jones;        /* diagrammatis tabulae */
} NodusTabulae;

/* data generata (lib/tabula_nodorum_data.c) */
extern constans NodusTabulae TABULA_NODORUM[];
extern constans i32          TABULA_NODORUM_NUMERUS;
extern constans i32          TABULA_NODORUM_PD[];
extern constans character*   TABULA_NODORUM_FONS;   /* provenientia */

i32
tabula_nodorum_numerus (vacuum);

/* NIHIL si i extra fines */
constans NodusTabulae*
tabula_nodorum_nodus (
    i32 i);

/* per titulum ("8_20"); NIHIL si ignotum */
constans NodusTabulae*
tabula_nodorum_quaere (
    constans character* titulus);

/* codex PD nodi (4 per transitum; NIHIL pro nodo triviali) */
constans i32*
tabula_nodorum_pd (
    constans NodusTabulae* n);

/* speculum nodus ipse est (amphichiralis) */
b32
tabula_nodorum_amphichiralis (
    constans NodusTabulae* n);

/* tabula parata: polynomia semel lecta (in piscina) */
nomen structura TabulaNodorum TabulaNodorum;

TabulaNodorum*
tabula_nodorum_aperire (
    Piscina* piscina);

/* candidatus: nodus primus (secundus NIHIL) aut compositum primus #
 * secundus; speculum = diagramma tabulae speculatum */
nomen structura {
    constans NodusTabulae* primus;
                      b32  primus_speculum;
    constans NodusTabulae* secundus;
                      b32  secundus_speculum;
} Agnitio;

/* omnes candidati quorum Alexander (forma normalis) et Jones datis
 * aequales sunt: primi, deinde compositi duorum (ordine tabulae, primus
 * <= secundus). Amphichiralis semel (speculum FALSUM). Scribit usque ad
 * maximus in exitus; reddit numerum TOTUM (potest > maximus). */
i32
tabula_nodorum_agnoscere (
    constans TabulaNodorum* tabula,
                Polynomium  alexander,
                Polynomium  jones,
                   Piscina* piscina,
                   Agnitio* exitus,
                       i32  maximus);

#endif /* TABULA_NODORUM_H */
