#ifndef FILUM_H
#define FILUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"


/* ==================================================
 * Lectio Simplex
 * ================================================== */

/* Legere filum totum in chordam (ex piscina allocatum)
 * Reddit chordam cum mensura=0 si defectu */
chorda
filum_legere_totum (
    constans character* via,
               Piscina* piscina);

/* Verificare si filum existit */
b32
filum_existit (
    constans character* via);

/* Cape mensuram fili in bytes (reddit 0 si defectu) */
memoriae_index
filum_mensura (
    constans character* via);


/* ==================================================
 * Lectio per Lineas
 * ================================================== */

nomen structura FilumLector FilumLector;

/* Aperire filum pro lectione per lineas
 * Reddit NIHIL si defectu */
FilumLector*
filum_lector_aperire (
    constans character* via,
               Piscina* piscina);

/* Legere lineam proximam (sine charactere lineae novae)
 * Reddit VERUM si linea lecta, FALSUM ad finem vel errore
 * Linea allocata ex piscina */
b32
filum_lector_lineam_proximam (
         FilumLector* lector,
              chorda* linea_out);

/* Cape numerum versus currentem (pro errore reportando) */
i32
filum_lector_numerus_versus (
    FilumLector* lector);

/* Verificare si ad finem fili */
b32
filum_lector_finis (
    FilumLector* lector);

/* Claudere lectorem et liberare res
 * Nota: chordae ex piscina manent validae */
vacuum
filum_lector_claudere (
    FilumLector* lector);


/* ==================================================
 * Scriptio
 * ================================================== */

/* Modus aperire pro FilumScriptor */
nomen enumeratio {
    FILUM_MODUS_CREARE,   /* Creare/superscribere filum */
    FILUM_MODUS_APPENDERE /* Appendere ad filum existens */
} FilumModus;

nomen structura FilumScriptor FilumScriptor;

/* Aperire filum pro scriptione continua
 * Reddit NIHIL si defectu */
FilumScriptor*
filum_scriptor_aperire (
     constans character* via,
             FilumModus  modus,
                Piscina* piscina);

/* Scribere chordam ad filum apertum */
b32
filum_scriptor_scribere (
     FilumScriptor* scriptor,
            chorda  contentum);

/* Scribere literas C ad filum apertum */
b32
filum_scriptor_scribere_literis (
         FilumScriptor* scriptor,
    constans character* contentum);

/* Scribere lineam cum newline ad finem */
b32
filum_scriptor_lineam_scribere (
         FilumScriptor* scriptor,
    constans character* linea);

/* Sync ad discum (fflush) */
b32
filum_scriptor_sync (
    FilumScriptor* scriptor);

/* Claudere scriptorem et liberare res */
vacuum
filum_scriptor_claudere (
    FilumScriptor* scriptor);


/* Scribere chordam ad filum (creare/superscribere)
 * Reddit VERUM in successu */
b32
filum_scribere (
    constans character* via,
                chorda  contentum);

/* Scribere literas C ad filum (creare/superscribere) */
b32
filum_scribere_literis (
    constans character* via,
    constans character* contentum);

/* Appendere chordam ad filum existens */
b32
filum_appendere (
    constans character* via,
                chorda  contentum);

/* Appendere literas C ad filum existens */
b32
filum_appendere_literis (
    constans character* via,
    constans character* contentum);


/* ==================================================
 * Manipulatio Filorum
 * ================================================== */

/* Delere filum
 * Reddit VERUM in successu */
b32
filum_delere (
    constans character* via);

/* Delere ARBOREM directorii (rm -r) SINE nexibus symbolicis sequendis.
 *
 * CUR NON directorium_ambulare: id stat() vocat, ergo nexum ad
 * directorium ut directorium tractat et IN EUM descendit - arbor aliena
 * extra viam datam deleretur. Hic lstat(): nexus UT NEXUS removetur,
 * destinatio eius intacta manet.
 *
 * Recusat NIHIL, viam vacuam et RADICEM systematis (per identitatem,
 * non per literas: "//" et "/tmp/.." quoque). Via non exsistens =
 * VERUM (nihil delendum). Filum aut nexus = ipse removetur. Directorium
 * relegitur donec vacuum sit (readdir post unlink introitus omittere
 * potest). FALSUM ad primum defectum; quod iam deletum est manet
 * deletum. POSIX solum (sub _WIN32 recusat). */
b32
filum_arborem_delere (
    constans character* via);

/* Renominare/movere filum */
b32
filum_movere (
    constans character* via_vetus,
    constans character* via_nova);

/* Copiare filum */
b32
filum_copiare (
    constans character* via_fons,
    constans character* via_destinatio);

/* Creare directorium
 * Reddit VERUM in successu, FALSUM si iam existit vel error */
b32
filum_directorium_creare (
    constans character* via);

/* Creare directorium si non existit
 * Reddit VERUM in successu (vel iam existit) */
b32
filum_directorium_creare_si_necesse (
    constans character* via);

/* Creare directorium CUM MODO dato si non existit; si iam existit,
 * modum COERCET (chmod).
 *
 * CUR SEPARATUM a creare_si_necesse: illud 0755 figit, quod pro
 * datis publicis rectum est sed pro secretis falsum. Directorium
 * socketorum moderantium ssh 0700 postulat - socketus enim sessioni
 * plenae aditum praebet, ergo nomen eius aliis usoribus ne
 * appareat quidem. Arca (thesaurus secretorum, futurum nominatum)
 * idem postulabit.
 *
 * Modum COERCET etiam in directorio praeexsistenti: directorium
 * quod alia causa 0755 creavit aliter tacite laxum maneret. */
b32
filum_directorium_creare_cum_modo (
    constans character* via,
               integer  modus);

/* Creare directorium ET OMNES PARENTES desideratos (mkdir -p)
 *
 * CUR EXSISTIT: tres creatores supra UNUM mkdir faciunt, ergo semita
 * nidificata cuius parentes absunt cum ENOENT DEFICIT - et quisquis
 * semitam struit id tacite patitur (concha, mensa, villa_agens
 * creatores simplices vocant hodie). fasciculum_scribere
 * Contents/MacOS/ creare debet antequam exsecutabile eo copiet
 * (fasciculum-spec par. V).
 *
 * Modus 0755 ut in creare_si_necesse. SINE REVERSIONE: si segmentum
 * medium deficit, quae iam creata sunt MANENT (mos ipse ipsius
 * mkdir -p).
 *
 * Reddit VERUM si semita tota post vocationem exsistit (iam
 * exsistens VERUM est, ergo idempotens); FALSUM si via NIHIL aut
 * vacua aut nimis longa, aut si segmentum creari non potest - causa
 * specifica vocationis interioris SERVATUR (filum_error_recens).
 */
b32
filum_directorium_creare_cum_parentibus (
    constans character* via);

/* Verificare si directorium existit */
b32
filum_directorium_existit (
    constans character* via);

/* Ponere modum fili (chmod)
 *
 * CUR EXSISTIT: filum_copiare ansa octetorum nuda est, ergo modum NON
 * servat - binarium in fasciculum .app copiatum non exsecutabile
 * caderet, et fasciculus sine bite +x duplici ictu sine ulla diagnosi
 * deficit (fasciculum-spec par. VII.1).
 *
 * Reddit VERUM in successu; FALSUM si via NIHIL aut chmod deficit.
 */
b32
filum_modum_ponere (
    constans character* via,
               integer  modus);


/* ==================================================
 * Interrogatio Status
 * ================================================== */

nomen structura {
    memoriae_index mensura;         /* Mensura in bytes */
               b32 est_directorium; /* Est directorium? */
               b32 est_filum;       /* Est filum regulare? */
               b32 potest_legere;   /* Legibile? */
               b32 potest_scribere; /* Scribabile? */
               b32 potest_exsequi;  /* Exsequibile? (bit +x usoris) */
} FilumStatus;

/* Cape statum/informationem fili
 * Reddit VERUM si filum existit et status captus */
b32
filum_status (
    constans character* via,
           FilumStatus* status_out);


/* ==================================================
 * Errores
 * ================================================== */

/* Cape nuntium erroris recens (literas C, non possessus a vocante)
 * Reddit errorem descriptivum vel NIHIL si nullus error */
constans character*
filum_error_recens (
    vacuum);

#endif /* FILUM_H */
