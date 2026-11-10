/* psalmodia_tabula.h - lector tabulae tonorum psalmorum
 * (musica/toni_psalmorum.stml) -> tabula typata
 *
 * Spec: project-specs/psalmodia-spec.md (P2: tabula data est, forma
 * eius interfacies est; P5: clausula = series locorum; II, IV, V).
 * Planum: project-specs/psalmodia-plan-1.md. GRAMMATICA tabulae in
 * commentario capitis ipsius tabulae stat; hic quid lector inde
 * facit. Stratum: stml.h et gabc.h solum - carmen nescit, aptatorem
 * nescit (pars II: syllabae et aptator hanc tabulam legent).
 *
 * USUS:
 *   PsalmodiaTabula* t = psalmodia_tabulam_legere(octeti, piscina);
 *   si (!psalmodia_successus(t))
 *       psalmodia_vitia_scribere(t, "toni_psalmorum.stml", exitus);
 *   terminatio = psalmodia_signum_invenire(t, signum, &tonus, causa);
 *   si (terminatio == NIHIL) ... causa Latine dicit cur ...
 *   mediatio = psalmodia_mediationem_eligere(tonus, FALSUM);
 *   si (semiversus in monosyllabam desinit)
 *       mediatio = psalmodia_clausulam_monosyllabam(tonus, mediatio);
 *   per (i = ZEPHYRUM; i < mediatio->numerus_locorum; i++)
 *       ... mediatio->loci[i].genus, .notae, .superveniens ...
 *
 * TOTALITAS: octeti quilibet -> tabula (NIHIL solum si memoria
 * deficit) cum vitiis locatis ('linea N: causa'); lectio post
 * elementum malum pergit, toni boni legi manent. NIHIL TACITE:
 * elementum aut attributum ignotum, attributum necessarium absens
 * (tonus: modus clavis tenor; exemplum: pars textus notae), notae
 * non legibiles - quodque vitium cum linea sua.
 *
 * PROBATIONES (in lectione; vitia addunt, tabulam non abiciunt):
 *   - notae omnes (locorum, superveniens, flexa, initium, exemplorum
 *     praeter signa crucis et asterisci) gabc legibiles: "(" clavis
 *     ") a(" notae ")" per gabc_legere; clavis terminationis si
 *     data, alias toni;
 *   - differentia = littera notae ultimae clausulae ("CDEFGAB"[gradus
 *     mod VII], majuscula non refert), nisi nomen '*' habet (A*, G*)
 *     aut vacuum est;
 *   - eadem terminatio bis in eodem modo, positionibus ambabus
 *     numeratis (clavis: modus, differentia, monosyllaba, ad_libitum);
 *   - anticipata sine superveniente; clausula sine loco; tenor non
 *     nota una; exemplum differentiam toni absentem nominans.
 *
 * SIGNA (psalmodia_signum_invenire): signum = modus + differentia, ut
 * cantus scribunt ("8 G", "VIII g", "8.G", "8G", "1 d", "4 A*",
 * "5"). Modus: 1-8, I-VIII, "peregrinus", "in directum" (majuscula
 * non refert); deinde spatia aut '.' (aut nihil, si cifrae modum
 * claudunt); reliquum = differentia. Quaeritur in OMNIBUS tonis eius
 * modi (IV et IV altera): nomen exactum primum; aliter par sine
 * respectu majusculae, si unicum ("1 d" -> D); differentia vacua ->
 * terminatio toni unica, si una est. Formae monosyllaba et
 * ad_libitum numquam redduntur. NIHIL -> causa, e.g.:
 *   tonus "9" nullus
 *   tonus 8 differentiam "x" non habet; differentiae: G, c, G*
 *   tonus 2 plures differentias habet: D, A
 *
 * FORMA CANONICA (psalmodia_tabulam_scribere, probationes): linea una
 * per tonum, clausulam, locum, exemplum, ordine fontis, campi tabulis
 * separati; lineae fontis NON scribuntur (commentarium tabulae
 * mutatum formam non mutat). Grammatica in probatio_psalmodia_tabula.c
 * fixa.
 *
 * DATA: omnia in piscina (stml valores copiat); fons post lectionem
 * liberari potest. Tabula post lectionem tantum legitur. Chorda
 * "vacua" = mensura nulla.
 */

#ifndef PSALMODIA_TABULA_H
#define PSALMODIA_TABULA_H

/* <aedilis corpus="lib/psalmodia_tabula.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"


/* ==================================================
 * Clausulae: mediatio et terminatio
 * ================================================== */

/* locus = syllaba una clausulae (GRAMMATICA tabulae) */
nomen enumeratio {
    PSALMODIA_LOCUS_PRAEPARATIO,     /* numero fixo, sine accentu */
    PSALMODIA_LOCUS_ACCENTUS,        /* in syllabam accentu notatam */
    PSALMODIA_LOCUS_NOTA             /* post accentum; ultima = finis */
} PsalmodiaLociGenus;

nomen structura {
    PsalmodiaLociGenus genus;
                chorda notae;        /* gabc syllabae: "ixi", "gvFED" */
                chorda superveniens; /* ACCENTUS: nota syllabae
                                      * supervacuae; vacua = nulla */
    b32 anticipata;   /* nota addita in syllaba accentus,
                                      * neuma in sequentem transit */
    i32 linea;
} PsalmodiaLocus;

nomen structura {
    PsalmodiaLocus* loci;
               i32  numerus_locorum;
               b32  monosyllaba;     /* casus="monosyllaba" */
               b32  ad_libitum;      /* forma altera (VI: antiqua) */
            chorda  differentia;     /* terminatio; vacua = unica */
            chorda  vicaria;         /* differentia loco huius
                                      * permissa; vacua = nulla */
    chorda clavis;          /* vacua = clavis toni */
    chorda tenor;           /* secundae partis; vacua = toni */
    chorda initium;         /* intonatio secundae partis,
                                      * versu primo (peregrinus) */
    i32 linea;
} PsalmodiaClausula;


/* ==================================================
 * Exempla libri (probationes aptatoris, pars II)
 * ================================================== */

nomen enumeratio {
    PSALMODIA_PARS_VERSUS,
    PSALMODIA_PARS_SEMIVERSUS,
    PSALMODIA_PARS_INITIUM,
    PSALMODIA_PARS_MEDIATIO,
    PSALMODIA_PARS_TERMINATIO,
    PSALMODIA_PARS_FLEXA
} PsalmodiaPars;

nomen structura {
    PsalmodiaPars pars;
           chorda pagina;            /* "5*" */
           chorda forma;             /* "A".."D", "contracta" ...;
                                      * vacua = nulla */
    chorda differentia;       /* vacua = nulla */
    chorda nota;              /* "hebr." ...; vacua = nulla */
       b32 monosyllaba;       /* casus="monosyllaba" */
       b32 primus;            /* versus: VERUM nisi
                                      * primus="falsum" */
       b32  canticum;          /* intonatio in omni versu */
    chorda  textus;
    chorda* notae;             /* per syllabam, "r" = superveniens;
                                      * crux et asteriscus signa sua */
    i32 numerus_notarum;
    i32 linea;
} PsalmodiaExemplum;


/* ==================================================
 * Toni et tabula
 * ================================================== */

nomen structura {
    chorda modus;        /* "1".."8", "peregrinus",
                                      * "in directum" */
                  s32  modus_numerus; /* 1..8; 0 = alius */
                  b32  positio_altera;
               chorda  clavis;       /* gabc: "c4", "f3" */
               chorda  tenor;        /* nota recitationis: "h" */
               chorda  pagina;
               chorda* initium;      /* per syllabam; 0 = nulla */
                  i32  numerus_initii;
               chorda  flexa;        /* vacua = nulla */
    PsalmodiaClausula* mediationes;
                  i32  numerus_mediationum;
    PsalmodiaClausula* terminationes;
                  i32  numerus_terminationum;
    PsalmodiaExemplum* exempla;
                  i32  numerus_exemplorum;
                  i32  linea;
} PsalmodiaTonus;

nomen structura {
       i32 linea;
    chorda causa;
} PsalmodiaVitium;

nomen structura {
     PsalmodiaTonus* toni;
                i32  numerus_tonorum;
    PsalmodiaVitium* vitia;
                i32  numerus_vitiorum;
} PsalmodiaTabula;


/* ==================================================
 * Lectio et scriptura
 * ================================================== */

/* textus STML -> tabula cum vitiis; NIHIL solum si memoria deficit */
PsalmodiaTabula*
psalmodia_tabulam_legere (
      chorda  stml,
     Piscina* piscina);

/* VERUM si tabula lecta est et nullum vitium habet */
b32
psalmodia_successus (
    constans PsalmodiaTabula* tabula);

/* 'via:linea: causa' per lineam */
b32
psalmodia_vitia_scribere (
    constans PsalmodiaTabula* tabula,
          constans character* via,
           ChordaAedificator* exitus);

/* forma canonica (probationes) - vide caput */
b32
psalmodia_tabulam_scribere (
    constans PsalmodiaTabula* tabula,
           ChordaAedificator* exitus);


/* ==================================================
 * Inventio
 * ================================================== */

/* modus (formae ut in SIGNA) + differentia -> terminatio communis.
 * tonus (si non NIHIL) tonum eius accipit. NIHIL -> causa (si non
 * NIHIL) Latine scribitur, differentiis toni enumeratis. */
constans PsalmodiaClausula*
psalmodia_terminationem_invenire (
      constans PsalmodiaTabula*  tabula,
                        chorda   modus,
                        chorda   differentia,
       constans PsalmodiaTonus** tonus,
             ChordaAedificator*  causa);

/* signum totum ("8 G") -> terminatio communis; cetera ut supra */
constans PsalmodiaClausula*
psalmodia_signum_invenire (
      constans PsalmodiaTabula*  tabula,
                        chorda   signum,
       constans PsalmodiaTonus** tonus,
             ChordaAedificator*  causa);

/* mediatio communis, aut ad_libitum (VI: antiqua) si petita et
 * impressa; NIHIL solum si tonus nullam mediationem habet */
constans PsalmodiaClausula*
psalmodia_mediationem_eligere (
     constans PsalmodiaTonus* tonus,
                         b32  ad_libitum);

/* soror casus="monosyllaba" clausulae (eiusdem generis, differentiae,
 * ad_libitum), aut ipsa clausula si tonus sororem non imprimit
 * (regula communis tunc valet) */
constans PsalmodiaClausula*
psalmodia_clausulam_monosyllabam (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula);

#endif /* PSALMODIA_TABULA_H */
