/* sonda r1: imaginem_inserere cum actu <imago> recusato (vitium
 * scripturae simulatum per triggerum SQLite in connexione secunda) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "scrinium.h"
#include "pictor_documentum.h"
#include <stdio.h>

s32
principale (vacuum)
{
    Piscina* p;
    InternamentumChorda* intern;
    Volumen* vol;
    PictorDocumentum* doc;
    Scrinium* s;
    i8 indices[IV];
    s32 id;
    s64 seq;
    i32 ante;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    vol = volumen_temporarium(p, "sonda_pd_r1");
    doc = pictor_documentum_creare(p, intern, vol, "", XVI, XVI, LXIV);
    si (!doc) { imprimere("creare fallit\n"); redde I; }
    indices[0] = 1; indices[1] = 2; indices[2] = 3; indices[3] = 4;

    s = scrinium_aperire(p, volumen_via(vol));
    si (!scrinium_exsequi(s, "CREATE TRIGGER sonda_t BEFORE INSERT ON acta"
        " WHEN NEW.datum LIKE '<imago%' BEGIN SELECT RAISE(ABORT,"
        " 'sonda'); END"))
    {
        imprimere("trigger fallit: %s\n", scrinium_error(s));
        redde I;
    }
    ante = doc->numerus_stratorum;
    id = pictor_documentum_imaginem_inserere(doc, indices, II, II, 0, 0, 1);
    imprimere("strata ante=%u post=%u id redditum=%d (0 expectatum)\n",
        (insignatus)ante, (insignatus)doc->numerus_stratorum, (integer)id);
    imprimere("vivi=%u\n", (insignatus)pictor_documentum_numerus_vivorum(doc));

    (vacuum)scrinium_exsequi(s, "DROP TRIGGER sonda_t");
    scrinium_claudere(s);

    /* actum sequens usoris: ictus in strato 1 */
    seq = pictor_documentum_actum(doc, chorda_ex_literis(
        "<ictus color=\"3\" magnitudo=\"1\" stratum=\"1\">"
        "<punctum x=\"1\" y=\"1\"/></ictus>", p));
    imprimere("ictus seq=%ld strata=%u vivi=%u\n", (longus)seq,
        (insignatus)doc->numerus_stratorum,
        (insignatus)pictor_documentum_numerus_vivorum(doc));
    (vacuum)pictor_documentum_revocare(doc);
    imprimere("post revocare unum: strata=%u vivi=%u (si ictus solus"
        " revocatus: strata=2)\n", (insignatus)doc->numerus_stratorum,
        (insignatus)pictor_documentum_numerus_vivorum(doc));
    volumen_claudere(vol);
    redde 0;
}
