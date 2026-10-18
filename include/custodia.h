/* custodia.h - custos applicationis: tesserae (argon2id per labores
 * reactoris), sessiones, CSRF, origo, frenum conatuum, secretum servi
 *
 * TESSERAE: hash et verificatio OPERA sunt in reactore (labor unus
 * per opus currens, argon2id_gradus caudices_per_gradum caudices per
 * gyrum) - ansa una interim alias petitiones servit. Plura quam
 * opera_concurrentia (defalta II: memoria 19 MiB per opus) in cauda
 * FIFO exspectant. Reditus in ansa vocatur, post opus finitum
 * (memoria iam reddita). Usor ignotus (phc vacua): hash ficticium
 * plenum cum parametris currentibus - tempus idem, responsum FALSUM
 * (nullum oraculum temporis de exsistentia rationis). Parametri
 * veteres verificant et 'renovanda' nuntiant (re-hash post successum).
 *
 * SESSIO: signum = XXXII octeti fortuiti base64url (XLIII signa),
 * crustulum; servatur solum SHA-256(signum) - copia datorum signa
 * non prodit.
 *
 * CSRF: base64url(HMAC-SHA256(secretum, sigillum sessionis)) - campus
 * occultus formae, comparatio temporis constantis; cum Origin
 * congruente in omni methodo non tuta.
 *
 * FRENUM: in memoria, clavis (IP, ratio) -> conatus falsi. Post
 * frenum_conatus: mora crescens (CCL ms, duplicata), post duplum:
 * recusatio usque ad fenestram post conatum ultimum. Successus purgat.
 * Tabula fixa (MXXIV), clavis per SHA-256 - memoria non crescit. */
#ifndef CUSTODIA_H
#define CUSTODIA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "reactor.h"
#include "sigillum.h"
#include "argon2id.h"

#define CUSTODIA_SECRETUM        XXXII
#define CUSTODIA_SIGNUM_MENSURA  XLIV    /* XLIII signa + terminator */
#define CUSTODIA_TESSERA_MINIMA  VIII
#define CUSTODIA_TESSERA_MAXIMA  MXXIV   /* custos DoS hashandi */
#define CUSTODIA_OPERA_MAXIMA    LXIV    /* currentia + exspectantia */

nomen structura {
                   i8 secretum[CUSTODIA_SECRETUM];
    Argon2idParametri tessera_parametri;   /* 0: 19456, t 2, p 1, 32 */
                  i32 opera_concurrentia;  /* 0 = II */
                  i32 caudices_per_gradum; /* 0 = XXXII */
                  i32 frenum_conatus;      /* 0 = V */
                  i32 frenum_fenestra_s;   /* 0 = CM (15 min) */
} CustodiaConfiguratio;

nomen structura Custodia Custodia;

Custodia*
custodia_creare (
    constans CustodiaConfiguratio* configuratio,
                          Reactor* reactor,
                          Piscina* piscina);

/* phc valida solum intra reditum; vacua = error */
nomen vacuum (*CustodiaHashReditus)(
    vacuum* datum,
     chorda  phc);

/* bona: tessera congruit; renovanda: bona et parametri phc a
 * configuratione differunt */
nomen vacuum (*CustodiaVerificatioReditus)(
    vacuum* datum,
        b32  bona,
        b32  renovanda);

/* FALSUM (sine reditu): tessera invalida, cauda plena, fons fortuitus
 * deficit */
b32
custodia_tesseram_digerere (
               Custodia* custodia,
                 chorda  tessera,
    CustodiaHashReditus  reditus,
                 vacuum* datum);

/* phc vacua aut mala = usor ignotus: hash ficticium plenum, bona
 * FALSUM. FALSUM (sine reditu): tessera nimis longa, cauda plena. */
b32
custodia_tesseram_verificare (
                      Custodia* custodia,
                        chorda  phc,
                        chorda  tessera,
    CustodiaVerificatioReditus  reditus,
                        vacuum* datum);

/* VIII..MXXIV octeti */
b32
custodia_tessera_valida (
    chorda tessera);

/* opera nunc currentia (probatio: tectum concurrentiae) */
i32
custodia_opera_currentia (
    constans Custodia* custodia);

/* summa graduum laborum ab initio (probatio: hash ficticium plenum) */
s64
custodia_gradus_summa (
    constans Custodia* custodia);

nomen structura {
    character signum[CUSTODIA_SIGNUM_MENSURA];   /* base64url, NUL */
     Sigillum sigillum;                          /* SHA-256(signum) */
} CustodiaSessio;

/* FALSUM si fons fortuitus deficit (exitus tunc intactum) */
b32
custodia_sessionem_creare (
    CustodiaSessio* exitus);

/* SHA-256 signi (crustuli) - clavis copiae sessionum */
Sigillum
custodia_signi_sigillum (
    chorda signum);

/* signum CSRF sessionis (XLIII signa base64url) */
chorda
custodia_csrf (
             Custodia* custodia,
    constans Sigillum* sessio,
              Piscina* piscina);

/* tempore constanti */
b32
custodia_csrf_verificare (
             Custodia* custodia,
    constans Sigillum* sessio,
               chorda  valor);

/* origo == schema "://" hospes (sine casu); origo vacua aut "null"
 * = FALSUM */
b32
custodia_origo_congruit (
    chorda origo,
    chorda schema,
    chorda hospes);

/* 0 = perge; > 0 = mora ms (responsum differendum); < 0 = recusa */
s32
custodia_frenum (
     Custodia* custodia,
       chorda  clavis,
          s64  nunc_ms);

vacuum
custodia_frenum_notare (
     Custodia* custodia,
       chorda  clavis,
          b32  successus,
          s64  nunc_ms);

/* Secretum servi: legit si exstat (modus 0600 strictus, XXXII
 * octeti), aliter creat (O_EXCL, 0600, fsync). FALSUM si modus
 * latior, mensura mala, aut fons deficit. Lectio CRUDA (non per
 * filum/lectiones: secretum numquam in libro lectionum). */
b32
custodia_secretum_parare (
    constans character* via,
                    i8* secretum);

#endif /* CUSTODIA_H */
