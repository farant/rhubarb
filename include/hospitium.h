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
 *
 * RESPONSUM DILATUM (hospitium-v3-spec II.1): tractator
 * colloquium_differre vocat et MANUBRIUM (valorem, non indicem)
 * servat; postea (labor reactoris, responsum alienum)
 * dilatum_colloquium colloquium reddit - aut NIHIL si connexio interim
 * finita est (cliens abiit, terminus responsi, sistere): manubrium
 * vetus numquam
 * petitionem alienam attingit (generatio loci). Reactor communis
 * (configuratio.reactor) applicationem et servum ansa UNA agit.
 *
 * FONTES CORPORIS: colloquium_respondere_fontem / _filum - capita cum
 * Content-Length vero, corpus per partes dum fossa haurit (numquam
 * totum in memoria).
 *
 * CORPORA PER RIVUM (II.1, T3): post capita hospitium corpus ipse
 * legit. multipart/form-data -> parser multipartitus: campi in
 * memoria (colloquium_campi), partes fili in fila temporaria cum
 * SHA-256 in itinere (colloquium_fila); corpus aliud <=
 * corpus_in_memoria_maximum in memoria (petitio->corpus), maius in
 * filum temporarium (petitio->corpus_via). Fila temporaria deletur ad
 * finem petitionis (etiam cliente abeunte), nisi
 * colloquium_filum_retinere. "Expect: 100-continue" -> 100 statim.
 */

#ifndef HOSPITIUM_H
#define HOSPITIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http.h"
#include "reactor.h"
#include "sigillum.h"
#include "crustula.h"
#include "url.h"


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
                   i32  portus;                      /* 0 = auto-selectus */
                   i32  connexiones_maximae;         /* 0 = CXXVIII (tectum CXXVIII) */
                   i32  petitio_maxima;              /* 0 = 1MB (413 super) */
                   i32  uri_maxima;                  /* 0 = 8KB (414 super) */
           MoraAngusta  otium_maximum_ms;    /* 0 = 30000 */
           MoraAngusta  capita_maxima_ms;    /* 0 = 10000 */
                   b32  acao;                        /* Access-Control-Allow-Origin: * */
                   b32  acta_accessus;               /* linea per petitionem ad stderr */
    /* NIHIL = reactor proprius; aliter communis (hospitium eum non
     * destruit) */
              Reactor* reactor;
    /* 0 = 30000 - terminus responsi dilati, deinde 503 */
           MoraAngusta responsum_maximum_ms;
    /* 0 = 1 MB - corpus maius (non multipartitum) in filum
     * temporarium (petitio->corpus_via) */
                   i32 corpus_in_memoria_maximum;
    /* NIHIL = $TMPDIR aut /tmp. Fila temporaria hic; idem systema
     * filorum ac destinatio colloquium_filum_retinere (rename) */
    constans character* directorium_temporarium;
    /* VERUM = post nginx: X-Forwarded-For / -Proto creduntur SOLUM si
     * par est loopback (127.x); FALSUM = numquam */
                   b32 procurator_fidus;
} HospitiumConfiguratio;

/* Manubrium responsi dilati - VALOR (copia libera); irritum fit cum
 * connexio finitur aut petitio respondetur */
nomen structura {
     Hospitium* hospitium;
           i32  locus;
           i32  generatio;
} HospitiumDilatum;

/* Fons corporis: mensura = Content-Length; legere: >0 octeti, 0 finis,
 * <0 error (connexio clauditur); claudere (NIHIL licet) semel vocatur */
nomen structura {
       i64 mensura;
       s32 (*legere)(vacuum* datum, i8* capsa, i32 n);
    vacuum (*claudere)(vacuum* datum);
    vacuum* datum;
} HospitiumFons;

/* Pars fili multipartiti, in filo temporario dum advenit (SHA-256 in
 * itinere). Deletur post petitionem nisi retentum. */
nomen structura {
                 chorda  campus;
                 chorda  titulus_fili;   /* clientis; nomen, numquam via */
                 chorda  mimen;
     constans character* via;            /* filum temporarium */
                    i64  mensura;
              character  sigillum_hex[SIGILLUM_HEX_MENSURA];
} HospitiumFilum;

/* Pars non-fili multipartiti (valor in memoria, campus_maximus) */
nomen structura {
    chorda campus;
    chorda valor;
} HospitiumCampus;

/* Numeratores - decisiones perf haec legunt */
nomen structura {
    i64 connexiones_acceptae;
    i64 connexiones_plenae_recusatae;  /* 503 ad tectum */
    i64 petitiones_tractatae;
    i64 petitiones_reiectae;           /* custodes 400/411/413/414/501 */
    i64 responsa_missa;
    i64 clausae_otio;                  /* termini (otium + capita) */
    i64 clausae_reliquiis;             /* clausura honesta pipelining */
    i64 capita_abiecta;                /* tectum XXXII aut CR/LF */
} HospitiumFructus;


/* ========================================================================
 * FUNCTIONES - CYCLUS VITAE
 * ======================================================================== */

/* Creare servum: ligare + auscultare + reactor internus.
 * Redde NIHIL si ligatio fallit. */
Hospitium*
hospitium_creare (
                           Piscina* piscina,
    constans HospitiumConfiguratio* configuratio);

/* Claudere omnes connexiones + auscultatorem */
vacuum
hospitium_destruere (
    Hospitium* hospitium);

/* Registrare tractatorem pro methodo + via (":param" supportatum).
 * datum opacum ad colloquium_datum redditur. */
b32
hospitium_praebere (
             Hospitium* hospitium,
          HttpMethodus  methodus,
    constans character* via,
    HospitiumTractator  tractator,
                vacuum* datum);

/* Servire directorium sub praefixo viae (GET/HEAD solum):
 * index.html documentum defaltum, pagina indicis generata si absens,
 * defensio traversalis (../ recusatur post decodificationem). */
b32
hospitium_directorium_servire (
             Hospitium* hospitium,
    constans character* praefixum,
    constans character* directorium);

/* Gyrus unus ansae (pro ansa hospitis externa).
 * Redde FALSUM post hospitium_sistere. */
b32
hospitium_gressus (
    Hospitium* hospitium,
          s32  ms_maximae);

/* Gressus in ansa usque ad sistere */
vacuum
hospitium_currere (
    Hospitium* hospitium);

vacuum
hospitium_sistere (
    Hospitium* hospitium);

/* Portus ligatus (utilis cum portus 0 = auto) */
i32
hospitium_portus (
    constans Hospitium* hospitium);

HospitiumFructus
hospitium_fructus (
    constans Hospitium* hospitium);

/* Reactor quo servus agitur (proprius aut communis) */
Reactor*
hospitium_reactor (
    Hospitium* hospitium);

/* Sistere molliter: auscultator clauditur (connexiones novae
 * recusantur), petitiones currentes (et dilatae) perficiuntur usque ad
 * mora_ms, deinde omnia clauduntur; hospitium_gressus postea FALSUM */
vacuum
hospitium_sistere_molliter (
    Hospitium* hospitium,
          i32  mora_ms);

/* VERUM postquam sistere molliter perfectum est */
b32
hospitium_sistit (
    constans Hospitium* hospitium);


/* ========================================================================
 * FUNCTIONES - INTRA TRACTATOREM (vita petitionis solum)
 * ======================================================================== */

constans HttpPetitioServeri*
colloquium_petitio (
    HospitiumColloquium* colloquium);

/* Valor parametri viae (":id" etc.); chorda vacua si absens */
chorda
colloquium_param (
    HospitiumColloquium* colloquium,
     constans character* titulus);

/* Piscina petitionis - allocationes moriuntur post responsum */
Piscina*
colloquium_piscina (
    HospitiumColloquium* colloquium);

/* Datum registratum in hospitium_praebere */
vacuum*
colloquium_datum (
    HospitiumColloquium* colloquium);

/* Addere caput responsi - ANTE respondere vocandum. Tectum XXXII
 * (Content-Type et Location locum reservatum habent); ultra tectum aut
 * titulus/valor cum CR, LF aut NUL: ABICITUR, numeratur
 * (fructus.capita_abiecta) et in actis accessus nominatur. */
vacuum
colloquium_caput_addere (
    HospitiumColloquium* colloquium,
     constans character* titulus,
     constans character* valor);

/* Respondere uno ictu (vocatio prima vincit; sine vocatione = D).
 * mimen: typus MIME pro Content-Type (NIHIL = sine capite) */
vacuum
colloquium_respondere (
    HospitiumColloquium* colloquium,
                    i32  status,
     constans character* mimen,
                 chorda  corpus);

/* Respondere cum fonte corporis (capita + corpus per partes); HEAD:
 * capita sola, fons statim clauditur */
vacuum
colloquium_respondere_fontem (
     HospitiumColloquium* colloquium,
                     i32  status,
      constans character* mimen,
           HospitiumFons  fons);

/* Respondere cum filo per fontem; FALSUM (et 404 missum) si filum
 * aperiri nequit */
b32
colloquium_respondere_filum (
    HospitiumColloquium* colloquium,
                    i32  status,
     constans character* mimen,
     constans character* via);

/* ---- auxilia petitionis (II.2): resoluta pigre, in piscina
 * petitionis servata ---- */

/* Valor primus clavis in quaestione (decodificatus, '+' = spatium);
 * datum NIHIL si absens */
chorda
colloquium_quaestio (
    HospitiumColloquium* colloquium,
     constans character* clavis);

/* Omnia paria quaestionis (ordine, claves repetitae omnes) */
UrlParams
colloquium_quaestiones (
    HospitiumColloquium* colloquium);

/* Caput petitionis (titulus sine casu); datum NIHIL si absens */
chorda
colloquium_caput (
    HospitiumColloquium* colloquium,
     constans character* titulus);

/* Campus formae: application/x-www-form-urlencoded (corpus in
 * memoria) aut multipart/form-data (colloquium_campi); valor primus;
 * datum NIHIL si absens */
chorda
colloquium_forma (
    HospitiumColloquium* colloquium,
     constans character* campus);

/* Crustulum petitionis (omnia capita Cookie); datum NIHIL si absens */
chorda
colloquium_crustulum (
    HospitiumColloquium* colloquium,
     constans character* titulus);

/* Set-Cookie addere. optiones NIHIL = defaltae tutae: HttpOnly,
 * SameSite=Lax, Path=/, sessionis, Secure si colloquium_schema https.
 * FALSUM si recusatum (crustulum_scribere) aut tectum capitum. Plura
 * crustula = plura capita Set-Cookie. */
b32
colloquium_crustulum_ponere (
           HospitiumColloquium* colloquium,
            constans character* titulus,
                        chorda  valor,
    constans CrustulumOptiones* optiones);

/* IP clientis: X-Forwarded-For (ultimus saltus) si procurator_fidus
 * ET par loopback; aliter par ipse */
chorda
colloquium_cliens (
    HospitiumColloquium* colloquium);

/* "https" si procurator_fidus, par loopback et X-Forwarded-Proto
 * https; aliter "http" */
chorda
colloquium_schema (
    HospitiumColloquium* colloquium);

/* Respondere redirectione (e.g. 303) cum Location; locus cum CR/LF
 * recusatur (D) */
vacuum
colloquium_redirigere (
    HospitiumColloquium* colloquium,
                    i32  status,
     constans character* locus);

/* Fila partium multipartitarum (vita petitionis): numerus, *fila =
 * tabula. Corpus multipartitum petitio->corpus NON implet. */
i32
colloquium_fila (
         HospitiumColloquium*  colloquium,
     constans HospitiumFilum** fila);

/* Filum servare: movetur (rename) ad via_nova, non deletur post
 * petitionem; filum->via deinde via_nova. FALSUM si movere fallit
 * (e.g. systema filorum aliud) - filum tunc temporarium manet. */
b32
colloquium_filum_retinere (
        HospitiumColloquium* colloquium,
    constans HospitiumFilum* filum,
         constans character* via_nova);

/* Campi (partes non fila) multipartiti, ordine adventus */
i32
colloquium_campi (
          HospitiumColloquium*  colloquium,
     constans HospitiumCampus** campi);

/* Responsum differre: tractator redit sine responso; manubrium
 * servandum. Terminus: responsum_maximum_ms (503). */
HospitiumDilatum
colloquium_differre (
    HospitiumColloquium* colloquium);

/* VERUM si responsum dilatum adhuc dari potest */
b32
dilatum_valet (
    HospitiumDilatum dilatum);

/* Colloquium dilati, aut NIHIL si non valet (cliens abiit, terminus,
 * iam responsum) */
HospitiumColloquium*
dilatum_colloquium (
    HospitiumDilatum dilatum);

#endif /* HOSPITIUM_H */
