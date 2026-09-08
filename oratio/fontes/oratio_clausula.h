/* oratio_clausula.h - CLAUSULA: stampa clausularum (T20a, 2026-09-08;
 * spec par. II decisiones XLIII-XLVII, par. VII 'Design - T20')
 *
 * Clausula = CAPSA sudoku inter vicinitatem (ordo) et sententiam totam
 * (tabula): structura quam verbum finitum regit. Nodus generis
 * 'clausula' in lista 'clausulae' sententiae (semen, species, pater,
 * umbrae verbum-finitum + subiectum e schemate generis natae, decisio
 * XLII gradu sententiae); elementa plana manent et per INDEX
 * 'clausula' + 'clausula-causa' eum monstrant - non scripta = APERTA
 * (nemo posuit). Lex super clausulam membra decisa sola legit, ergo
 * eliminatio super capsam partialem sana manet (decisio XXXVII).
 *
 * STRATA (decisio XLV), quodque causam suam scribens ut oraculum id
 * seorsum metiatur:
 *   I   SEMINA (data, causa et numerus in censu thesaurorum
 *       2026-09-08): subordinantes CERTAE (si quia qualiter dum quod
 *       sicut ... - SCONJ sine exceptione, limes clausulae 86-100 %
 *       chartis, 73-100 % Senecae), CORROBORANDAE (ut cum nisi ubi
 *       unde: limes solum cum verbo finito capaci ante semen proximum
 *       - ut comparativum Senecae 57 %, cum adpositio chartarum),
 *       RELATIVA (qui quicumque quisquis: 68 % Senecae, 94-97 %
 *       chartis), COORDINANTIA solum inter verba finita capacia (et
 *       76-78 % inter verba finita aurea, 22-31 % aliter; -que 0-3 %,
 *       omissum). Interpunctio NON seminat (comma 23-37 % limes) sed
 *       CLAUDIT (comma post clausulam subordinatam 53-59 %); parentheses
 *       clausulam parentheticam aperiunt et claudunt.
 *   II  EXTENTUM: semina acervo ambulata - subordinans/relativum
 *       clausulam novam impellit (pater = summa acervi), coordinans
 *       summam sorore substituit, verba inter semina summae accedunt;
 *       ante semen primum clausula 0 principalis; sententia sine semine
 *       ullo = causa UNICA ubique.
 *   III CLAUSURA: clausula non basalis ad verbum finitum CERTUM suum
 *       (lectiones omnes finitae) aut ad comma/semicolon/colon clauditur
 *       (prior verbi finalis); verba sequentia usque ad semen proximum
 *       ad parentem redduntur causa CLAUSURA.
 *   V   VERBUM (T20a bis, 2026-09-08): verbum finitum CERTUM alterum
 *       in clausula non clausibili quae suum iam habet = semen absens
 *       (asyndeton: Seneca imperativa et versus, formulae chartarum)
 *       - clausula coordinata nova, verbum semen eius; verba inter
 *       verbum prius et hoc ad novam (ordo verbo finali: verba verbum
 *       suum praecedunt; variatio mensurata) et sequentia usque ad
 *       semen proximum causa VERBUM.
 *   IV  CATENA (T20b): per umbras ligatas, relativo excepto.
 *
 * Verbum finitum CAPAX = lectio Latina verbi aut auxiliaris forma
 * finita (forma-verbi finitum, aut modus indicativus/subiunctivus/
 * imperativus sine forma scripta); CERTUM = lectiones omnes tales.
 * Anglica: nulla lectio capax (Moby sine modo) - sententiae Anglicae
 * UNICAE in T20a, semina Anglica tranche sua.
 *
 * Idempotens: sententia cuius locus 'clausulae' scriptus est praeteritur.
 */

#ifndef ORATIO_CLAUSULA_H
#define ORATIO_CLAUSULA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "materia_nodus.h"
#include "oratio_registrum.h"

nomen structura {
    i32 sententiae;      /* sententiae stampatae (hoc cursu) */
    i32 clausulae;       /* nodi clausulae structi */
    i32 species[ORATIO_SPECIES_CLAUSULAE_NUMERUS];
    i32 causae[ORATIO_CLAUSULA_CAUSA_NUMERUS];   /* elementa posita per causam */
    i32 apertae;         /* elementa non posita */
    i32 corroborata;     /* semina corroboranda quae verbum invenerunt */
    i32 reiecta;         /* candidata corroboranda sine verbo (non semina) */
        i32 clausae_verbo;   /* clausurae per verbum finitum certum */
    i32 clausae_signo;   /* clausurae per interpunctionem */
    i32 scissae;         /* scissiones verbi (stratum V, asyndeton) */
} OratioClausulaCensus;

/* DATA seminum (lemmata WORDS, litteris v; NIHIL terminata) */
externus constans character* constans ORATIO_SEMINA_CERTA[];
externus constans character* constans ORATIO_SEMINA_CORROBORANDA[];
externus constans character* constans ORATIO_SEMINA_RELATIVA[];
externus constans character* constans ORATIO_SEMINA_COORDINANTIA[];

vacuum
oratio_clausula_census_vacare (
    OratioClausulaCensus* census);

/* An lectio verbum finitum sit (Latina, verbum | auxiliare, forma
 * finita aut modus finitus sine forma). */
b32
oratio_clausula_lectio_finita (
    constans MateriaNodus* analysis);

/* Sententias radicis (documentum | paragraphus | sententia annotata)
 * seminare: strata I-III et V, nodi clausulae in sententiam, loci
 * clausula/clausula-causa in elementa. lingua = titulus linguae
 * documenti ("latina" | "anglica"; NIHIL = latina): documentum non
 * Latinum lectionibus Latinis NON seminatur nec scinditur (it, sit,
 * do verba Latina finita sunt - EWT CDXII clausulas falsas dabat) -
 * sententiae eius UNICAE donec semina suae linguae data sint.
 * Sententia iam stampata praeteritur. census NIHIL licet. FALSUM =
 * memoria sola. */
b32
oratio_clausulas_seminare (
                 Piscina* piscina,
            MateriaNodus* radix,
      constans character* lingua,
    OratioClausulaCensus* census);

#endif /* ORATIO_CLAUSULA_H */
