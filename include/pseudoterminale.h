/* pseudoterminale.h - infans in pseudo-terminali (aemulator-plan B3,
 * terminal-planning modulus 008)
 *
 * Programma (argv, NULLA CONCHA - mos processus) in latere servo
 * pseudo-terminalis generatur, sessione nova et terminali regente;
 * hospes latus magistrum legit et scribit. Frater processus, non
 * extensio (decisio XII): unus fluxus mixtus, infans diu vivens,
 * magnitudo et signa propria.
 *
 * TABULA FUNCTIONUM = SUTURA PROBATIONUM (mos TesseraPons): hospes
 * (ansa + aemulator + codificator) per pontem memoriae sine processu
 * probatur; pons posix solus capita systematis tangit (in .c solo -
 * mos tcp.h). Nucleus aemulatoris hunc pontem numquam videt.
 *
 * VITA: creare -> legere/scribere/amplitudo in ansa hospitis ->
 * legere -1 (infans clausit) -> finitus (messis) -> claudere.
 * claudere semper vocandum; post id nullus zombi, nulla fossa
 * aperta - etiam si infans adhuc currit.
 */

#ifndef PSEUDOTERMINALE_H
#define PSEUDOTERMINALE_H

/* pons posix per conventionem (pseudoterminale_posix.c); pars
 * portabilis (configuratio, nomina errorum, pons memoriae) hic: */
/* <aedilis corpus="lib/pseudoterminale.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen structura {
    i32 codex;     /* WEXITSTATUS; 0 si signo occisus */
    i32 signum;    /* WTERMSIG, aut 0 */
} PseudoterminaleExitus;

nomen structura Pseudoterminale Pseudoterminale;

structura Pseudoterminale {
    vacuum* datum;
    /* octeti ab infante usque ad capacitatem. mora_ms: 0 = nulla
     * exspectatio, > 0 = milisecunda, < 0 = sine fine. Reddit
     * lectos (> 0), 0 = nihil paratum intra moram, -1 = finis
     * (infans latus servum clausit, aut error). */
    s32 (*legere)    (vacuum* datum, i8* buffer, i32 capacitas,
                      s32 mora_ms);
    /* octeti ad infantem (claves codificatae, responsa
     * quaestionum). NUMQUAM OBSTAT: reddit octetos acceptos (0..n;
     * minus quam n = nucleus plenus, reliquum hospes postea
     * mittit), -1 = error. */
    s32 (*scribere)  (vacuum* datum, constans i8* octeti, i32 n);
    /* TIOCSWINSZ; nucleus systematis SIGWINCH infanti mittit.
     * px = 0 licet (ignota). */
    b32 (*amplitudo) (vacuum* datum, i32 latitudo, i32 altitudo,
                      i32 px_latitudo, i32 px_altitudo);
    /* messis sine exspectatione (waitpid WNOHANG, decisio XIII):
     * VERUM si infans exivit (exitus impletur; iterum vocata idem
     * reddit), FALSUM si adhuc currit. */
    b32 (*finitus)   (vacuum* datum, PseudoterminaleExitus* exitus);
    /* descriptor lateris magistri, ut ansa hospitis in eo
     * expectet (decisio XIV); -1 si nullus (pons memoriae). Hospes
     * eum numquam claudit nec legit nisi per 'legere'. */
    s32 (*fossa)     (vacuum* datum);
    /* SIGHUP gregi infantis, magistrum claudere, metere (SIGKILL
     * post moram brevem si non exit). Idempotens. */
    vacuum (*claudere) (vacuum* datum);
};

nomen enumeratio {
    PSEUDOTERMINALE_OK = ZEPHYRUM,
    PSEUDOTERMINALE_ERROR_ARGUMENTA,  /* argv vacuum, magnitudo mala */
    PSEUDOTERMINALE_ERROR_APERIRE,    /* openpty fallita */
    PSEUDOTERMINALE_ERROR_GENERARE,   /* fistula/furca fallita */
    PSEUDOTERMINALE_ERROR_EXEC        /* binarium non inventum aut non
                                       * exsecutabile - DISTINCTUM ab
                                       * exitu 127 infantis veri */
} PseudoterminaleError;

nomen structura {
    /* NIHIL-terminata; [0] per PATH (execvp) */
    constans character* constans* argumenta;
    /* mutationes ambitus super ambitum hereditum, NIHIL-terminatae:
     * "NOMEN=valor" ponit, "NOMEN" tollit. NIHIL = nullae. */
    constans character* constans* ambitus;
    /* NIHIL = directorium hereditum */
     constans character* directorium;
                    i32  latitudo;     /* cellulae, >= I */
                    i32  altitudo;
                    i32  px_latitudo;  /* 0 = ignota */
                    i32  px_altitudo;
} PseudoterminaleConfiguratio;

/* Ordinaria: argumenta NIHIL (vocans ponit), ambitus =
 * { "TERM=xterm-256color", "COLORTERM=truecolor" } (decisio VIII),
 * directorium hereditum, LXXX x XXIV, px ignota. Campi postea
 * addendi hic defaltas accipiunt. */
vacuum
pseudoterminale_configuratio_initiare (
    PseudoterminaleConfiguratio* cfg);

constans character*
pseudoterminale_error_nomen (
    PseudoterminaleError error);


/* ==================================================
 * Pons posix (openpty, decisio XI)
 * ================================================== */

/* openpty + furca + setsid + TIOCSCTTY + execvp; error exec per
 * fistulam CLOEXEC (creare exspectat donec exec aut defectus - non
 * diutius). NIHIL in errore: *error (et *descriptio cum errno, si
 * non NIHIL) impletur; nihil relinquitur (fossae clausae, infans
 * messus). */
Pseudoterminale*
pseudoterminale_posix_creare (
                                 Piscina* piscina,
    constans PseudoterminaleConfiguratio* cfg,
                    PseudoterminaleError* error,
                                  chorda* descriptio);


/* ==================================================
 * Pons memoriae (sutura probationum)
 * ================================================== */

/* Infans fictus: 'effusio' (copiata) per legere redditur, deinde
 * -1 (finis) et finitus VERUM cum 'codex'. Quod hospes scribit
 * capitur. */
Pseudoterminale*
pseudoterminale_memoriae_creare (
         Piscina* piscina,
     constans i8* effusio,
             i32  n,
             i32  codex);

/* octeti ab hospite scripti hactenus (visus; valet usque ad
 * scribere proximum) */
chorda
pseudoterminale_memoriae_captum (
    constans Pseudoterminale* pt);

/* magnitudo ultima per amplitudo posita */
vacuum
pseudoterminale_memoriae_amplitudo (
    constans Pseudoterminale* pt,
                         i32* latitudo,
                         i32* altitudo);

#endif /* PSEUDOTERMINALE_H */
