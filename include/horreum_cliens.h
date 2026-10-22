/* horreum_cliens.h - stratum OBSTANS horrei (horreum-spec II.5, plan
 * T3): opus quodlibet (horreum.h) per HttpVectura exsequitur - signat,
 * mittit, legit; iterat errores transitorios (mora exponentialis cum
 * fortuna, Retry-After servatur), horologium semel corrigit ex capite
 * Date servi (RequestTimeTooSkewed), et '412 post iterationem' per
 * HEAD diiudicat: x-amz-meta-sha256 == nostrum -> successus cum
 * iam_nostrum, aliter HORREUM_CONDITIO verus. Quisque conatus de novo
 * signatur. Numquam scribit in log aut stdout.
 *
 * Horologium, somnus et sors iniciuntur (probationes numquam
 * dormiunt). Sine horreum_cliens_parare: http_exsequi, time(),
 * nanosleep, III conatus, basis CC ms.
 *
 *   HorreumObjectum ob;  HorreumError e;
 *   si (!horreum_ponere(h, situla, clavis, corpus, NIHIL, &ob, &e, p))
 *       ... e.genus, e.codex, e.nuntius ... */
#ifndef HORREUM_CLIENS_H
#define HORREUM_CLIENS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fasti.h"
#include "http.h"
#include "horreum.h"

/* <aedilis corpus="lib/horreum_cliens.c"/> */

nomen structura {
          HttpVectura vectura;                 /* exsequi NIHIL =
                                                   * http_exsequi */
          MomentumSec (*nunc)(vacuum* datum);    /* NIHIL = time() */
               vacuum (*dormire)(vacuum* datum, i32 ms);  /* NIHIL =
                                                   * nanosleep */
               vacuum* datum;
                  i32  conatus_maximi;          /* 0 = III */
                  i32  mora_basis_ms;           /* 0 = CC */
                  i64  semen;                   /* sors; 0 = ex
                                                   * tempore */
    HorreumObservator  observator;              /* NIHIL = nemo */
               vacuum* observatoris_datum;
} HorreumClientisOptiones;

/* optiones (NIHIL = ordinariae) in sedem horrei; piscina sedem tenet
 * et horreum superesse debet. Deviatio horologii semel inventa in sede
 * manet (vocationes sequentes eam adhibent) */
vacuum
horreum_cliens_parare (
                             Horreum* h,
    constans HorreumClientisOptiones* optiones,
                             Piscina* piscina);

nomen structura {
              b32  successus;
    HttpResponsum* responsum;    /* ultimum (successus aut error) */
              i32  conatus;      /* petitiones operis missae (HEAD
                                  * iam_nostrum non numeratur) */
              s64 deviatio_s;   /* deviatio horologii adhibita */
              b32 iam_nostrum;
} HorreumExitus;

/* exitus et obiectum NIHIL licent */
b32
horreum_exsequi (
                 Horreum* h,
    constans HorreumOpus* opus,
           HorreumExitus* exitus,
         HorreumObjectum* obiectum,
            HorreumError* error,
                 Piscina* piscina);

b32
horreum_ponere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
                     chorda  corpus,
    constans HorreumPonendi* optiones,
            HorreumObjectum* obiectum,
               HorreumError* error,
                    Piscina* piscina);

/* corpus in memoria (<= XVI MiB, P1: maius -> HORREUM_MALUM,
 * horreum_capere_in_filum) */
b32
horreum_capere (
                     Horreum* h,
                      chorda  situla,
                      chorda  clavis,
    constans HorreumCapiendi* optiones,
                      chorda* corpus,
             HorreumObjectum* obiectum,
                HorreumError* error,
                     Piscina* piscina);

b32
horreum_inspicere (
            Horreum* h,
             chorda  situla,
             chorda  clavis,
    HorreumObjectum* obiectum,
       HorreumError* error,
            Piscina* piscina);

b32
horreum_delere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
    HorreumError* error,
         Piscina* piscina);

/* VERUM si petitio successit; singulae claves in exitus (deletum aut
 * error suus) - numquam omnia aut nihil */
b32
horreum_delere_plura (
            Horreum*  h,
             chorda   situla,
    constans chorda*  claves,
                i32   numerus,
     HorreumDeletio** exitus,
                i32*  exitus_numerus,
       HorreumError*  error,
            Piscina*  piscina);

b32
horreum_copiare (
                    Horreum* h,
                     chorda  situla_fons,
                     chorda  clavis_fons,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
            HorreumObjectum* obiectum,
               HorreumError* error,
                    Piscina* piscina);

/* pagina una; signum_proximum in optiones proximas */
b32
horreum_enumerare (
                       Horreum* h,
                        chorda  situla,
    constans HorreumEnumerandi* optiones,
                 HorreumPagina* pagina,
                  HorreumError* error,
                       Piscina* piscina);

/* ===== multipars (II.4) ===== */

b32
horreum_mp_incipere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
                     chorda* id_oneris,
               HorreumError* error,
                    Piscina* piscina);

/* exitus: numerus, ETag (sine virgulis), mensura - in perficere */
b32
horreum_mp_partem_mittere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id_oneris,
             i32  numerus,
          chorda  corpus,
     HorreumPars* exitus,
    HorreumError* error,
         Piscina* piscina);

/* '200 + <Error>' error est; NoSuchUpload post iterationem -> HEAD,
 * ETag expectatum -> successus cum iam_nostrum (II.5) */
b32
horreum_mp_perficere (
                 Horreum* h,
                  chorda  situla,
                  chorda  clavis,
                  chorda  id_oneris,
    constans HorreumPars* partes,
                     i32  numerus,
         HorreumObjectum* obiectum,
            HorreumError* error,
                 Piscina* piscina);

b32
horreum_mp_abicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id_oneris,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_mp_partes (
          Horreum*  h,
           chorda   situla,
           chorda   clavis,
           chorda   id_oneris,
      HorreumPars** exitus,
              i32*  numerus,
     HorreumError*  error,
          Piscina*  piscina);

/* onera inchoata nec perfecta (pecunia constant, enumeratione
 * invisibilia) - ad purgandum */
b32
horreum_mp_pendentes (
                 Horreum*  h,
                  chorda   situla,
                  chorda   praefixum,
      HorreumOnusPendens** exitus,
                     i32*  numerus,
            HorreumError*  error,
                 Piscina*  piscina);

/* ===== fila ===== */

nomen structura {
    i64 pars_mensura;          /* 0 = HORREUM_PARS_ORDINARIA */
} HorreumFiliOptiones;

/* infra partem (aut aequale): PUT unum; supra: multipars partibus
 * aequis (P12), pars una in memoria (piscina propria quaeque);
 * defectus finalis -> abicere (nihil orbum a nobis relinquitur).
 * optiones NIHIL licet */
b32
horreum_ponere_ex_filo (
                         Horreum* h,
                          chorda  situla,
                          chorda  clavis,
              constans character* via,
         constans HorreumPonendi* optiones,
    constans HorreumFiliOptiones* fili_optiones,
                 HorreumObjectum* obiectum,
                    HorreumError* error,
                         Piscina* piscina);

/* HEAD, deinde GET per frusta HORREUM_FRUSTUM_LECTIONIS cum If-Match
 * (P1) in filum temporarium eiusdem directorii ("<via>.horreum-<pid>-
 * <signum>"); mensura (et md5 si ETag partis unius) probata, deinde
 * filum_movere. Numquam filum dimidium aut mixtum ad destinationem;
 * filum temporarium in omni defectu deletur, destinatio exsistens
 * intacta manet. */
b32
horreum_capere_in_filum (
                Horreum* h,
                 chorda  situla,
                 chorda  clavis,
     constans character* via,
        HorreumObjectum* obiectum,
           HorreumError* error,
                Piscina* piscina);

#endif /* HORREUM_CLIENS_H */
