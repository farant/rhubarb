/* hospitium.h - Servus HTTP domesticus (domus hospitalis)
 *
 * Medium desideratum super quattuor dimidia plutei: tcp_servus
 * (fossae), http (parse/serialize), router (congruentia), reactor
 * (ansa eventuum). Filum unum, non-obstruens, keep-alive sine
 * pipelining (reliquiae => clausura honesta).
 *
 * USUS:
 *   HospitiumConfiguratio cfg;
 *   memset(&cfg, 0, magnitudo(cfg));          (defaltae omnes)
 *   Hospitium* h = hospitium_creare(piscina, &cfg);
 *   hospitium_praebere(h, HTTP_GET, "/salve", tractator, NIHIL);
 *   hospitium_directorium_servire(h, "/static", "./publica");
 *   hospitium_currere(h);                     (aut gressus in ansa hospitis)
 *
 *   intra tractatorem:
 *     colloquium_respondere(colloquium, CC, "text/html", corpus);
 *
 * VITA MEMORIAE: omnia ex colloquio (petitio, params, piscina)
 * vivunt usque ad finem petitionis SOLUM - post responsum missum
 * piscina connexionis reficitur.
 */

#ifndef HOSPITIUM_H
#define HOSPITIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

nomen structura Hospitium Hospitium;
nomen structura HospitiumColloquium HospitiumColloquium;

/* Tractator petitionis - vocatus intra gressum, cito redeat
 * (filum unum: opus lentum = problema actoris applicationis) */
nomen vacuum (*HospitiumTractator)(HospitiumColloquium* colloquium);

/* Configuratio - 0/NIHIL = defalta */
nomen structura {
    constans character* hospes;      /* NIHIL = "127.0.0.1" */
    i32 portus;                      /* 0 = auto-selectus */
    i32 connexiones_maximae;         /* 0 = CXXVIII (tectum CXXVIII) */
    i32 petitio_maxima;              /* 0 = 1MB (413 super) */
    i32 uri_maxima;                  /* 0 = 8KB (414 super) */
    MoraAngusta otium_maximum_ms;    /* 0 = 30000 */
    MoraAngusta capita_maxima_ms;    /* 0 = 10000 */
    b32 acao;                        /* Access-Control-Allow-Origin: * */
    b32 acta_accessus;               /* linea per petitionem ad stderr */
} HospitiumConfiguratio;

/* Numeratores - decisiones perf haec legunt */
nomen structura {
    i64 connexiones_acceptae;
    i64 connexiones_plenae_recusatae;  /* 503 ad tectum */
    i64 petitiones_tractatae;
    i64 petitiones_reiectae;           /* custodes 400/411/413/414/501 */
    i64 responsa_missa;
    i64 clausae_otio;                  /* termini (otium + capita) */
    i64 clausae_reliquiis;             /* clausura honesta pipelining */
} HospitiumFructus;


/* ========================================================================
 * FUNCTIONES - CYCLUS VITAE
 * ======================================================================== */

/* Creare servum: ligare + auscultare + reactor internus.
 * Redde NIHIL si ligatio fallit. */
Hospitium*
hospitium_creare(
    Piscina*                       piscina,
    constans HospitiumConfiguratio* configuratio);

/* Claudere omnes connexiones + auscultatorem */
vacuum
hospitium_destruere(Hospitium* hospitium);

/* Registrare tractatorem pro methodo + via (":param" supportatum).
 * datum opacum ad colloquium_datum redditur. */
b32
hospitium_praebere(
    Hospitium*          hospitium,
    HttpMethodus        methodus,
    constans character* via,
    HospitiumTractator  tractator,
    vacuum*             datum);

/* Servire directorium sub praefixo viae (GET/HEAD solum):
 * index.html documentum defaltum, pagina indicis generata si absens,
 * defensio traversalis (../ recusatur post decodificationem). */
b32
hospitium_directorium_servire(
    Hospitium*          hospitium,
    constans character* praefixum,
    constans character* directorium);

/* Gyrus unus ansae (pro ansa hospitis externa).
 * Redde FALSUM post hospitium_sistere. */
b32
hospitium_gressus(
    Hospitium* hospitium,
    s32        ms_maximae);

/* Gressus in ansa usque ad sistere */
vacuum
hospitium_currere(Hospitium* hospitium);

vacuum
hospitium_sistere(Hospitium* hospitium);

/* Portus ligatus (utilis cum portus 0 = auto) */
i32
hospitium_portus(constans Hospitium* hospitium);

HospitiumFructus
hospitium_fructus(constans Hospitium* hospitium);


/* ========================================================================
 * FUNCTIONES - INTRA TRACTATOREM (vita petitionis solum)
 * ======================================================================== */

constans HttpPetitioServeri*
colloquium_petitio(HospitiumColloquium* colloquium);

/* Valor parametri viae (":id" etc.); chorda vacua si absens */
chorda
colloquium_param(
    HospitiumColloquium* colloquium,
    constans character*  titulus);

/* Piscina petitionis - allocationes moriuntur post responsum */
Piscina*
colloquium_piscina(HospitiumColloquium* colloquium);

/* Datum registratum in hospitium_praebere */
vacuum*
colloquium_datum(HospitiumColloquium* colloquium);

/* Addere caput responsi - ANTE respondere vocandum */
vacuum
colloquium_caput_addere(
    HospitiumColloquium* colloquium,
    constans character*  titulus,
    constans character*  valor);

/* Respondere uno ictu (vocatio prima vincit; sine vocatione = D).
 * mimen: typus MIME pro Content-Type (NIHIL = sine capite) */
vacuum
colloquium_respondere(
    HospitiumColloquium* colloquium,
    i32                  status,
    constans character*  mimen,
    chorda               corpus);

#endif /* HOSPITIUM_H */
