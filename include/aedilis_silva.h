/* aedilis_silva.h - extractor aedilis per silvam, ut bibliotheca
 * (fabrica-6 T6; olim in tools/aedilis.c solum; caput Franus probavit)
 *
 * aedilis_derivare extractorem per suturam accipit (AedilisExtractor).
 * Hic vivit extractor verus: silva pro .c/.h (directivae inclusionum
 * et annotationes <aedilis verbum="arg"/>), clang -MM pro .m (cursus
 * oraculi), memoria per cursum (caput quodque semel parsatur) et
 * thesaurus trans cursus (recordum per sigillum praefixi et octetorum
 * fontis). bin/aedilis et bin/fabrica eundem adhibent.
 *
 * USUS:
 *   AedilisSilva* ext = aedilis_silva_creare(piscina, configuratio,
 *       "build/aedilis/obiecta", &praefixum, NIHIL);
 *   fructus = aedilis_derivare(piscina, configuratio, scopus, NIHIL,
 *       aedilis_silva_extrahere, ext, &causa);
 */

/* <aedilis corpus="lib/aedilis_silva.c"/> */
#ifndef AEDILIS_SILVA_H
#define AEDILIS_SILVA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"
#include "aedilis.h"

nomen structura AedilisSilva AedilisSilva;

/* extractor parare: configuratio (inclusa pro clang -MM .m);
 * radix_thesauri NIHIL = sine thesauro; praefixum = clavis thesauri
 * (vocans dat: bin/aedilis sigillum binarii sui et aedilis.stml);
 * memoria_oraculi NIHIL = sine. NIHIL si silva aut thesaurus parari
 * nequit. */
AedilisSilva*
aedilis_silva_creare (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* radix_thesauri,
               constans Sigillum* praefixum,
              constans character* memoria_oraculi);

/* AedilisExtractor (datum = AedilisSilva*): silva pro .c/.h, -MM pro
 * .m, memoria per cursum + thesaurus trans cursus */
b32
aedilis_silva_extrahere (
                 vacuum*  datum,
     constans character*  via,
                Piscina*  piscina,
                    Xar** directivae_out,
                    Xar** annotationes_out,
                    b32*  ex_oraculo_out,
                    Xar** angulatae_out);

/* ORACULUM clang -MM fontis cuiusvis (.c quoque): viae capitum IAM
 * RESOLUTAE (normalizatae) in *directivae_out ADDUNTUR (Xar de chorda,
 * a vocante creata). bin/aedilis --differentia eo utitur. FALSUM si
 * clang defecit. */
b32
aedilis_silva_oraculum (
           AedilisSilva*  extractor,
     constans character*  via,
                Piscina*  piscina,
                    Xar** directivae_out);

#endif /* AEDILIS_SILVA_H */
