/* probatio_pictor_revocare.c - Cmd+Z / Cmd+Shift+Z in pictore
 *
 * I: ictus duo; Cmd+Z bis (revocatio multiplex), Cmd+Shift+Z bis.
 * II: 'z' sine Cmd nihil. III: linea pendens - Cmd+Z eam solum
 * abicit, nihil revocat. IV: stratum novum revocatur (currens ad
 * summum). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include "delineare.h"
#include "tabula_pixelorum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tabula in applicatione sola ad cellulam (VI, VIII) */
#define ORIGO_X VI
#define ORIGO_Y VIII

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* eventus muris ad punctum TABULAE (x, y) tempore t, modificantibus */
interior vacuum
mus (
      Dispensator* d,
  eventus_genus_t  genus,
              s32  x,
              s32  y,
              s64  t,
              i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = genus;
    e.tempus                  = t;
    e.datum.mus.x             = ORIGO_X + x;
    e.datum.mus.y             = ORIGO_Y + y;
    e.datum.mus.botton        = MUS_SINISTER;
    e.datum.mus.modificantes  = modificantes;
    dispensator_tractare(d, &e);
}

/* ictus (pressio + solutio) ad punctum tabulae */
interior vacuum
premere (
    Dispensator* d,
            s32  x,
            s32  y,
            s64  t,
            i32  modificantes)
{
    mus(d, EVENTUS_MUS_DEPRESSUS, x, y, t, modificantes);
    mus(d, EVENTUS_MUS_LIBERATUS, x, y, t + X, modificantes);
}

interior vacuum
clavis_z (
    PictorApplicatio* app,
                 i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = M;
    e.datum.clavis.clavis        = (clavis_t)'z';
    e.datum.clavis.runa          = 'z';
    e.datum.clavis.modificantes  = modificantes;
    e.datum.clavis.actio         = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;

    piscina = piscina_generare_dynamicum("probatio_pictor_revocare",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_revocare");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m    = manus_ludus_creare(piscina, app.d);
    doc  = app.doc;

    imprimere("\n--- I: revocatio multiplex, refectio ---\n");
    premere(app.d, L, L, M, ZEPHYRUM);
    premere(app.d, C, C, MM, ZEPHYRUM);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);
    clavis_z(&app, MOD_SUPER);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), I);
    clavis_z(&app, MOD_SUPER);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc),
        ZEPHYRUM);
    clavis_z(&app, MOD_SUPER | MOD_SHIFT);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), I);
    clavis_z(&app, MOD_SUPER | MOD_SHIFT);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);
    /* in fine: refectio nihil */
    clavis_z(&app, MOD_SUPER | MOD_SHIFT);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);

    imprimere("\n--- II: 'z' sine Cmd ---\n");
    clavis_z(&app, ZEPHYRUM);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);

    imprimere("\n--- III: linea pendens - Cmd+Z abicit solum ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'l', ZEPHYRUM));
    premere(app.d, CC, CC, MMM, ZEPHYRUM);
    CREDO_VERUM(!chorda_vacua(dispensator_motus(app.d)->captura));
    clavis_z(&app, MOD_SUPER);
    CREDO_VERUM(chorda_vacua(dispensator_motus(app.d)->captura));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(doc), II);

    imprimere("\n--- IV: stratum novum revocatur ---\n");
    CREDO_VERUM(pictor_documentum_actum(doc, chorda_ex_literis(
        "<stratum actio=\"novum\" id=\"2\" supra=\"1\"/>", piscina))
        > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), II);
    clavis_z(&app, MOD_SUPER);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    clavis_z(&app, MOD_SUPER | MOD_SHIFT);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), II);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
