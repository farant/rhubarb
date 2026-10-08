/* series_terminalis.h - Lexemator fluminis terminalis (DEC/Williams)
 *
 * Grammatica octetorum terminalis, utraque directione: quod applicatio
 * scribit (emulator consumit) et quod terminalis reddit (claves, mus,
 * glutinum, responsa). Octeti intrant, LEXEMATA exeunt. SENSUM non
 * possidet: quid CSI 'A' significet (sursum? cursor?) strata superiora
 * decernunt. Nullum tempus, nulla UTF-8, nulla allocatio post
 * creationem.
 * (eventus B1a; terminal-planning modules/002; machina vt100.net
 * dec_ansi_parser, ex Ghostty Parser.zig / parse_table.zig, MIT, pin
 * 12752b2.)
 *
 * DIVERGENTIAE A WILLIAMS/GHOSTTY (consultae; omnes initus serviunt):
 * - C1 octeti (0x80..0x9F) NON agnoscuntur: continuationes UTF-8 sunt,
 *   ut cetera imprimuntur (Franus, planum B par. III).
 * - DEL (0x7F) in solo = EXSEQUI (clavis retrorsum), non imprimere.
 * - CSI cum ':' quovis finali SERVATUR (separatores): kitty claves
 *   'CSI 97:65;2u' mittit; Ghostty eas extra 'm' abicit.
 * - ESC N / ESC O + octetus = lexema UNUM (SERIES_SS; parametra
 *   licent: 'ESC O 2 P'). Ghostty ESC O statim mittit.
 * - ESC ante seriem (ESC ESC [ A) non perit: 'praefixum' VERUM.
 * - Series abrupta (ESC, CAN, SUB in medio; ESC + octetus altus) =
 *   SERIES_FUGA cum octetis crudis, NON tacite abiecta; octetus
 *   abrumpens NON consumitur (vocatio proxima eum tractat).
 * - 'ESC \' post chordam (OSC/DCS/APC) = terminator, consumptum; nullum
 *   lexema ESC '\' sequitur.
 * - DCS TOTUM (non hook/put/unhook) et truncatum (Franus).
 * Ut Ghostty: plus quam SERIES_PARAMETRA_MAXIMA parametra -> series
 * TOTA abicitur (csi_ignore) - SGR dimidia peior est quam nulla.
 *
 * VISUS: 'textus' et 'crudum' valent usque ad vocationem proximam
 * (IMPRIMERE: visus in initum vocantis, valet dum initus).
 *
 * TEMPUS: lector solum 'pendet' nuntiat; vocans (lector initus) cum
 * mora
 * sua 'evacuare' vocat ut ESC solum aut seriem dimidiam reddat.
 */

#ifndef SERIES_TERMINALIS_H
#define SERIES_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

#define SERIES_PARAMETRA_MAXIMA   XXIV         /* Ghostty MAX_PARAMS */
#define SERIES_INTERMEDIA_MAXIMA  IV           /* Ghostty */
#define SERIES_CHORDA_MAXIMA      (II * MXXIV) /* osc.zig */
#define SERIES_CRUDUM_MAXIMUM     LXIV         /* octeti crudi seriei */
#define SERIES_PARAMETRUM_MAXIMUM 0x7FFFFFFF   /* saturatio */

nomen enumeratio {
    SERIES_NIHIL = ZEPHYRUM,  /* initus consumptus, nullum lexema */
    SERIES_IMPRIMERE,         /* cursus imprimibilis (visus) */
    SERIES_EXSEQUI,           /* C0 aut DEL; octetus in 'finale' */
    SERIES_ESC,               /* ESC + intermedia + finale */
    SERIES_CSI,               /* ESC [ privatum? parametra intermedia
                               * finale */
    SERIES_SS,                /* ESC N|O parametra? finale (SS2/SS3) */
    SERIES_OSC,               /* ESC ] corpus BEL|ST (textus) */
    SERIES_DCS,               /* ESC P parametra intermedia finale
                               * corpus ST (textus) */
    SERIES_APC,               /* ESC _ | ^ | X corpus ST (introductor
                               * dicit APC, PM, SOS) */
    SERIES_FUGA               /* series abrupta aut evacuata: crudum */
} SeriesGenus;

nomen structura {
    SeriesGenus genus;
            s32 parametra[SERIES_PARAMETRA_MAXIMA];
            i32 numerus_parametrorum;
            i32 separatores;    /* bitus i: ':' POST parametrum i */
             i8 intermedia[SERIES_INTERMEDIA_MAXIMA];
            i32 numerus_intermediorum;
             i8 privatum;       /* '?' '>' '<' '=' aut 0 */
             i8 introductor;    /* octetus post ESC ('[' ']' 'P' 'N'
                                 * 'O' '_' '^' 'X') aut 0 */
             i8 finale;         /* finale; EXSEQUI: octetus regiminis;
                                 * OSC: terminator - 0x07 (BEL) aut
                                 * '\\' (ESC: ST aut series nova) */
            b32 praefixum;      /* ESC solum seriem praecessit */
         chorda textus;         /* IMPRIMERE, OSC, DCS, APC */
         chorda crudum;         /* octeti seriei (ESC ... finale) */
            b32 truncatum;      /* corpus aut intermedia praecisa */
} SeriesLexema;

nomen structura SeriesLector SeriesLector;

/* Lector novus in statu solo; tabula transitionum hic struitur. */
SeriesLector*
series_lectorem_creare (
    Piscina* piscina);

/* MODUS INITUS (eventus B1b): grammatica directionis INITUS - quod
 * terminalis reddit, non quod applicatio scribit. Octeti idem ambigui
 * sunt (ESC P = DCS aut alt+P); responsa vera terminalium solum formas
 * certas habent, ergo in modo initus:
 * - ESC + (0x20..0x2F) = ESC finale (alt+spatium, alt+!), non
 *   intermedia (designatio characterum numquam ex terminali venit);
 * - ESC + C0 aut DEL = FUGA (ESC solus, octetus NON consumptus) -
 *   vocans ut alterum + regimen legit (alt+reditus, alt+retrorsum);
 * - ESC N, ESC X, ESC ^ = ESC finale (SS2, SOS, PM numquam reddita);
 * - ESC P + octetus finalis (0x40..0x7E) STATIM = ESC finale 'P' +
 *   octetus non consumptus (responsa DCS digito, '>', '!' aut '$'
 *   incipiunt; 'alt+P a' una lectione venit). ESC ] et ESC _ series
 *   manent: alt+] / alt+_ per moram vocantis (FUGA introductoris)
 *   leguntur.
 * Modus scriptionis (ordinarius, FALSUM) DEC purum manet. Purgatio
 * modum servat. */
vacuum
series_lectorem_initus_ponere (
    SeriesLector* lector,
             b32  initus);

/* Ad statum solum redire (seriem pendentem abicere sine lexemate). */
vacuum
series_lectorem_purgare (
    SeriesLector* lector);

/* Lexema proximum ex [*ptr, finis): *ptr promovetur. SERIES_NIHIL si
 * initus consumptus sine lexemate completo - series dimidia in statu
 * manet et vocatione proxima perficitur (scissura quaevis licet). */
SeriesGenus
series_lexema_proximum (
     SeriesLector*  lector,
      constans i8** ptr,
      constans i8*  finis,
     SeriesLexema*  lexema);

/* VERUM si in medio seriei (ESC solum, CSI dimidia, chorda aperta). */
b32
series_lector_pendet (
    constans SeriesLector* lector);

/* Post moram vocantis: series pendens ut SERIES_FUGA redditur (crudum,
 * introductor, praefixum), lector ad solum redit. FALSUM si nihil
 * reddendum (non pendet, aut solum 'ESC' terminatoris chordae
 * exspectabatur). */
b32
series_lectorem_evacuare (
    SeriesLector* lector,
    SeriesLexema* lexema);

#endif /* SERIES_TERMINALIS_H */
