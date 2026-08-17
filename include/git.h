/* git.h - lector repositorii git (LECTIO SOLA)
 *
 * Forma obiectorum git ab anno MMV congelata est - inter formas
 * stabilissimas computandi. Lector semel recte scriptus in
 * perpetuum rectus est: nulla putredo, nulla dependentia. Hoc
 * bibliothecae huius fundamentum morale est.
 *
 * SCOPUS: legere solum. Silex VCS domus manet; haec bibliotheca
 * interrogat quod git tenet ("da mihi lib/silex.c ut erat ad
 * HEAD~3") - pro differre_git, instrumentis, quibuslibet.
 * Scriptio obiectorum CONSULTO abest.
 *
 * VOCABULARIUM (praecedens: volumen massas suas iam habet - idem
 * conceptus, idem verbum):
 *   massa     = blob            arbor  = tree
 *   commissum = commit          ramus  = branch
 *   CAPUT     = HEAD            signum = tag adnotatum
 *   sha       = index obiecti (technicum, intactum ut 'offset') -
 *               NON sigillum: sigillum SHA-256 domus est, git
 *               SHA-1 utitur; nomina separata, confusio nulla.
 *
 * MECHANICA: obiecta laxa (zlib per flatura_inflare, adler
 * verificatum) + sarcinae (.pack/.idx v2: fanout + quaestio
 * binaria; deltae OFS/REF applicatae, catenae recursivae).
 * Indices sarcinarum toti in memoriam leguntur (parvi); corpora
 * sarcinarum per fseek/fread obiectatim (magna).
 *
 * REF-RESOLUTIO subcopia deliberata: sha plenum (40 hex), CAPUT,
 * nomina (refs/... aut refs/heads/X aut refs/tags/X, laxa aut
 * compacta in packed-refs), suffixum ~N (parens primus N-ies).
 * Non: ^, sha breve, intervalla. Ianuae nominatae, non lacunae.
 *
 * Repositoria SHA-256 (git init --object-format=sha256) NON
 * leguntur (recusatio clara ad aperire).
 */

#ifndef GIT_H
#define GIT_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define GIT_SHA_HEX_MENSURA 41   /* 40 + terminator */

/* sarcina una (.pack + .idx); idx totum in memoria */
nomen structura {
    constans character* via_sarcinae;   /* .pack */
    i8*                 idx_datum;
    i32                 idx_mensura;
    i32                 numerus;        /* obiecta in sarcina */
} GitSarcina;

nomen structura {
    Piscina*            piscina;    /* creationis - sarcinae etc. */
    constans character* via_git;    /* ".../.git" absoluta */
    Xar*                sarcinae;   /* GitSarcina */
} GitRepositorium;

/* ascendit ex via data (directorium) donec .git inveniat;
 * NIHIL = non repositorium (aut sha256, aut illegibile) */
GitRepositorium*
git_aperire (
    Piscina*            piscina,
    constans character* via);

/* textus -> sha hex in exitus[GIT_SHA_HEX_MENSURA];
 * subcopia supra descripta; FALSUM = non resolutum */
b32
git_ref_resolvere (
    GitRepositorium*    repositorium,
    constans character* textus,
    character*          sha_exitus);

nomen enumeratio {
    GIT_OBIECTUM_IGNOTUM = 0,
    GIT_OBIECTUM_COMMISSUM,
    GIT_OBIECTUM_ARBOR,
    GIT_OBIECTUM_MASSA,
    GIT_OBIECTUM_SIGNUM
} GitObiectumGenus;

nomen structura {
    b32              successus;
    GitObiectumGenus genus;
    chorda           datum;     /* corpus crudum (post caput) */
} GitObiectum;

/* laxum aut e sarcina (deltis applicatis); sha = hex 40 */
GitObiectum
git_obiectum_legere (
    GitRepositorium*    repositorium,
    constans character* sha,
    Piscina*            piscina);

nomen structura {
    b32    successus;
    chorda arbor;       /* sha hex arboris */
    Xar*   parentes;    /* de chorda (sha hex), ordine */
    chorda auctor;      /* linea cruda "Nomen <cursus> ts zona" */
    chorda commissor;
    s64    tempus;      /* epocha commissoris; 0 si illegibilis */
    chorda nuntius;
} GitCommissum;

GitCommissum
git_commissum_legere (
    GitRepositorium*    repositorium,
    constans character* sha,
    Piscina*            piscina);

nomen structura {
    chorda modus;      /* "100644" | "40000" | "120000" | ... */
    chorda titulus;
    chorda sha;        /* hex 40 */
    b32    est_arbor;  /* modus 40000 (subdirectorium) */
} GitArborIntroitus;

/* Xar de GitArborIntroitus ordine arboris; NIHIL = erratum */
Xar*
git_arborem_legere (
    GitRepositorium*    repositorium,
    constans character* sha,
    Piscina*            piscina);

/* 'git show <commissum>:<via>' ut vocatio C - arborem a radice
 * per segmenta viae ambulat, massam promit. *inventum FALSUM si
 * via ad commissum non exsistit (chorda vacua redditur). */
chorda
git_massam_per_viam (
    GitRepositorium*    repositorium,
    constans character* sha_commissi,
    constans character* via,
    Piscina*            piscina,
    b32*                inventum);

#endif /* GIT_H */
