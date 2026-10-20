/* sonda: app.doc post pictor_picturam_condere */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "pictor_applicatio.h"
#include <stdio.h>
#include <string.h>

s32
principale (vacuum)
{
    Piscina* p;
    InternamentumChorda* in;
    Volumen* vol;
    PictorApplicatio app;
    PictorDocumentum* prior;
    b32 r;

    p = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    thema_initiare();
    vol = volumen_temporarium(p, "sonda_pictor_applicatio");
    si (!vol) { imprimere("volumen nullum\n"); redde 1; }
    memset(&app, 0, magnitudo(app));
    si (!pictor_applicatio_aedificare(&app, p, in, vol, 480, 480))
    { imprimere("aedificare fallit\n"); volumen_claudere(vol); redde 1; }
    prior = app.doc;
    imprimere("ante: app.doc=%p montatio.doc=%p par=%d\n",
        (vacuum*)app.doc, (vacuum*)app.montatio.doc,
        app.doc == app.montatio.doc);
    r = pictor_picturam_condere(&app.montatio, "alia");
    imprimere("condere=%d\n", (s32)r);
    imprimere("post: app.doc=%p montatio.doc=%p par=%d app.doc==prior %d\n",
        (vacuum*)app.doc, (vacuum*)app.montatio.doc,
        app.doc == app.montatio.doc, app.doc == prior);
    imprimere("compositio.doc==montatio.doc %d\n",
        app.montatio.compositio.doc == app.montatio.doc);
    volumen_claudere(vol);
    redde 0;
}
