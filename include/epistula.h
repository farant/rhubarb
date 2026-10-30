/* epistula.h - API domus epistularum electronicarum (communio-spec D7;
 * plan communio-plan-3 T2)
 *
 * Vocator dicit "hanc epistulam huic mitte"; provisor post eam HTTP
 * agit. Provisores per nomen (forma vates): "resend" (POST
 * https://api.resend.com/emails, clavis Bearer, Idempotency-Key ne
 * epistula repetita bis mittatur) et "fictus" (epistulas in memoria
 * notat - probationes). Provisor alius (Cloudflare, SMTP) = inscriptio
 * nova in tabula, vocatores immutati.
 *
 * ASYNCHRONUM in reactore: epistulam_mittere statim redit; finis semel
 * vocatur (numquam intra mittere), successus aut causa brevis (numquam
 * clavis). Missiones simul ad plurimum EPISTULAE_MISSIONES; quaeque
 * piscinam suam habet, post finem deletam (servus diuturnus non
 * crescit).
 *
 * CLAVIS: copia in Epistulario sola; claudere eam delet; numquam in
 * causa, in nuntio, in actis.
 */

#ifndef EPISTULA_H
#define EPISTULA_H

/* <aedilis corpus="lib/epistula.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"

#define EPISTULAE_MISSIONES XVI

/* a, ad, titulus, textus necessaria; html et clavis_idempotentiae
 * optionalia (vacua = omissa) */
nomen structura {
    chorda a;
    chorda ad;
    chorda titulus;
    chorda textus;
    chorda html;
    chorda clavis_idempotentiae;
} EpistulaNuntius;

/* semel post missionem; causa brevis vitae (copianda si servanda) */
nomen vacuum (*EpistulaFinis)(vacuum* ctx, b32 successus, chorda causa);

nomen structura Epistularius Epistularius;

/* provisor "resend" | "fictus"; basis NIHIL = ordinaria provisoris
 * (probationes: hospitium locale). NIHIL si provisor ignotus (noti in
 * stderr nominantur) aut argumenta mala. */
Epistularius*
epistularium_aperire (
               Piscina* piscina,
               Reactor* reactor,
    constans character* provisor,
                chorda  clavis,
    constans character* basis);

/* asynchrone mittit; FALSUM si nuntius malus (a/ad/titulus/textus
 * desunt) aut missiones plenae - tum finis NON vocatur */
b32
epistulam_mittere (
                Epistularius* epistularius,
    constans EpistulaNuntius* nuntius,
               EpistulaFinis  finis,
                      vacuum* ctx);

/* missiones in itinere abrumpit (finis non vocatur), horas cancellat,
 * clavem delet */
vacuum
epistularium_claudere (
    Epistularius* epistularius);

/* provisor fictus: epistulae notatae, ordine missionis */
i32
epistularius_fictus_numerus (
    constans Epistularius* epistularius);

b32
epistularius_fictus_nuntius (
    constans Epistularius* epistularius,
                      i32  index,
          EpistulaNuntius* exitus);

/* corpus Resend (purum): {"from","to":[..],"subject","text","html"};
 * html omittitur si vacuum */
chorda
epistula_resend_corpus (
    constans EpistulaNuntius* nuntius,
                     Piscina* piscina);

#endif /* EPISTULA_H */
