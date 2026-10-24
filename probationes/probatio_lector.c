/* probatio_lector.c - lector in latere (lector L3): bibliotheca
 * (ordines, electus j k, ictus, paginae [ ], liber non legibilis,
 * bibliotheca vacua), index et paginae (T4), semitae (T5). Fixa:
 * probationes/fixa/lector/bibliotheca (aeneis = beta, Alpha,
 * fractum.epub). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "volumen.h"
#include "lector_bibliotheca.h"
#include "lector.h"
#include "codex.h"
#include "fons.h"
#include "semita.h"
#include "organum_montatio.h"
#include "credo.h"
#include <string.h>

#define BIBLIOTHECA  "probationes/fixa/lector/bibliotheca"
#define LATITUDO     CCXL
#define ALTITUDO     CDLXXII

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                 Volumen* volumen;
       LectorBibliotheca* bibliotheca;
          LectorMontatio  m;
             Dispensator* d;
        constans Organum* organum;   /* semitae: semita_legere */
} Mundus;

/* latus LATITUDO x altitudo super bibliothecam (directorium) */
interior Mundus*
mundus_magnitudine (
         CredoContextus* c,
     constans character* directorium,
                    i32  latitudo,
                    i32  altitudo)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern   = internamentum_creare(c->piscina);
    w->volumen  = volumen_temporarium(c->piscina,
        "probatio_lector");
    w->bibliotheca  = lector_bibliotheca_aperire(directorium,
        w->volumen,
        c->piscina, w->intern);
    w->repo         = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->volumen || !w->bibliotheca || !w->repo
        || !lector_montare(&w->m, c->piscina, w->intern, w->repo, "l1",
               w->bibliotheca, latitudo, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, lector_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    w->organum = organum_legere_genus(c->piscina, w->intern, "lector",
        "lector");
    redde w;
}

interior Mundus*
mundus (
         CredoContextus* c,
     constans character* directorium,
                    i32  altitudo)
{
    redde mundus_magnitudine(c, directorium, LATITUDO, altitudo);
}

interior vacuum
mundum_claudere (
    Mundus* w)
{
    si (w && w->volumen)
    {
        volumen_claudere(w->volumen);
        w->volumen = NIHIL;
    }
}

interior vacuum
clavis (
       Mundus* w,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
clavis_nominata (
       Mundus* w,
     clavis_t  k,
          s32  producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = k;
    e.datum.clavis.producta  = producta;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
reditus (
    Mundus* w)
{
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
}

interior b32
campus_est (
                 Mundus* w,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&w->m.ramus, titulus);
    si (valor == NIHIL)
    {
        redde a == NIHIL || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

interior Mandata*
reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* textus pictus continens fragmentum? */
interior b32
pictum (
                 Mundus* w,
     constans character* fragmentum,
                Piscina* p)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
ictus_in (
                 Mundus* w,
     constans character* id,
                Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (!c || c->fines.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior vacuum
_sectio_bibliotheca (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("latus novum: bibliotheca; libri, auctor, progressus");
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));
    CREDO_VERUM(pictum(w, "Library", c->piscina));
    CREDO_VERUM(pictum(w, "aeneis", c->piscina));
    CREDO_VERUM(pictum(w, "Alpha", c->piscina));
    CREDO_VERUM(pictum(w, "Anna Scriptrix", c->piscina));
    CREDO_VERUM(pictum(w, "new", c->piscina));
    CREDO_VERUM(pictum(w, "fractum.epub", c->piscina));
    CREDO_VERUM(pictum(w, "unreadable", c->piscina));

    CREDO_GRADUS("j k: electus; ictus in ordine r2");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_liber", "urn:rhubarb:alpha"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus_liber", "urn:rhubarb:beta"));
    CREDO_VERUM(ictus_in(w, "r2", c->piscina));
    CREDO_VERUM(campus_est(w, "electus_liber", "urn:rhubarb:alpha"));

    CREDO_GRADUS("Enter in libro non legibili: nihil aperitur");
    clavis(w, 'j');
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));

    CREDO_GRADUS("Enter in Alpha: visus pagina, liber Alpha");
    clavis(w, 'k');
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "liber", "urn:rhubarb:alpha"));

    mundum_claudere(w);
}

interior vacuum
_sectio_paginae_ordinum (
    CredoContextus* c)
{
    Mundus* w;

    /* LXXX px = X lineae: margines II, titulus III, caput II -> ordo
     * unus (III lineae) per paginam */
    w = mundus(c, BIBLIOTHECA, LXXX);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("ordo unus per paginam: ] -> Alpha, [ -> aeneis");
    CREDO_VERUM(pictum(w, "aeneis", c->piscina));
    CREDO_FALSUM(pictum(w, "Alpha", c->piscina));
    clavis(w, ']');
    CREDO_VERUM(pictum(w, "Alpha", c->piscina));
    CREDO_VERUM(campus_est(w, "electus_liber", "urn:rhubarb:alpha"));
    clavis(w, '[');
    CREDO_VERUM(pictum(w, "aeneis", c->piscina));

    CREDO_GRADUS("] deinde Enter: Alpha aperitur (electus paginam "
        "sequitur)");
    clavis(w, ']');
    reditus(w);
    CREDO_VERUM(campus_est(w, "liber", "urn:rhubarb:alpha"));

    mundum_claudere(w);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, "probationes/fixa/lector/nusquam", ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("bibliotheca vacua: nuntius, Enter nihil");
    CREDO_VERUM(pictum(w, "No books in ~/.rhubarb/codices",
        c->piscina));
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));

    mundum_claudere(w);
}

/* textus pictus in fonte dato continens fragmentum? */
interior b32
pictum_in_fonte (
                 Mundus* w,
     constans character* fragmentum,
                    i32  fons,
                Piscina* p)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && x->fons == fons
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
campus (
                 Mundus* w,
     constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_ramus_attributum(&w->m.ramus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior vacuum
effugium (
    Mundus* w)
{
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
}

/* positio servata libri */
interior b32
positio_servata (
                 Mundus* w,
     constans character* identitas,
          LectorPositio* q,
                Piscina* p)
{
    redde lector_positionem_legere(w->bibliotheca, chorda_ex_literis(
        identitas, p), q, p);
}

/* offset fragmenti in textu capitis k libri Alpha */
interior i32
locus_in_capite (
                 Mundus* w,
                    i32  k,
     constans character* fragmentum,
                Piscina* p)
{
         Codex* x;
    CodexCaput* caput;
        chorda  vitium;
           i32  n;
           i32  i;

    x = lector_bibliotheca_codex(w->bibliotheca,
        lector_bibliotheca_invenire(w->bibliotheca, chorda_ex_literis(
        "urn:rhubarb:alpha", p)), &vitium);
    caput  = x ? codex_caput_legere(x, k, p, &vitium) : NIHIL;
    n      = (i32)strlen(fragmentum);
    per (i = ZEPHYRUM; caput && i + n <= caput->textus.mensura; i++)
    {
        si (memcmp(caput->textus.datum + i, fragmentum, (size_t)n)
            == ZEPHYRUM)
        {
            redde i;
        }
    }
    redde ZEPHYRUM;
}

interior vacuum
_sectio_pagina (
    CredoContextus* c)
{
           Mundus* w;
    LectorPositio  q;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("Alpha sine positione: caput 0, pagina 0, positio "
        "scripta");
    clavis(w, 'j');
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "0"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(pictum(w, "Prooemium.", c->piscina));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, ZEPHYRUM);
    CREDO_AEQUALIS_I32(q.initium, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(q.vestigium, "Prooemium.");

    CREDO_GRADUS("]: caput I, titulus in FONS_TITULI, pes 'Caput I' et "
        "'1/1', positio {I, 0}");
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "caput", "1"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(pictum_in_fonte(w, "Caput I", FONS_TITULI, c->piscina));
    CREDO_VERUM(pictum_in_fonte(w, "Caput I", FONS_PARVUS, c->piscina));
    CREDO_VERUM(pictum(w, "1/1", c->piscina));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, I);
    CREDO_AEQUALIS_I32(q.initium, ZEPHYRUM);

    CREDO_GRADUS("[ ad caput prius (pagina ultima); [ in initio nihil");
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "caput", "0"));
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "caput", "0"));

    CREDO_GRADUS("] in fine libri nihil");
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "caput", "3"));
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "caput", "3"));

    CREDO_GRADUS("t: index, caput currens electum, 'Beginning'");
    clavis(w, 't');
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "electus_caput", "3"));
    CREDO_VERUM(pictum(w, "Beginning", c->piscina));
    CREDO_VERUM(pictum(w, "Caput II", c->piscina));

    CREDO_GRADUS("k, Enter: caput II a pagina prima");
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus_caput", "2"));
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "2"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("Esc: index; Esc: bibliotheca; Enter: positio libri");
    effugium(w);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    effugium(w);
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));
    reditus(w);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "2"));

    mundum_claudere(w);
}

interior vacuum
_sectio_positio (
    CredoContextus* c)
{
           Mundus* w;
    LectorPositio  q;
              i32  primum;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    primum = locus_in_capite(w, III, "Primum hic.", c->piscina);
    CREDO_VERUM(primum > ZEPHYRUM);

    CREDO_GRADUS("textus motus (III octeti): sententia vera invenitur, "
        "nulla nota, positio non rescripta");
    q.caput      = III;
    q.initium    = primum + III;
    q.vestigium  = chorda_ex_literis("Primum hic.", c->piscina);
    CREDO_VERUM(lector_positionem_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), &q));
    clavis(w, 'j');
    reditus(w);
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_VERUM(campus_est(w, "nota", NIHIL));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.initium, primum + III);

    CREDO_GRADUS("positio amissa: initium capitis cum nota");
    effugium(w);
    effugium(w);
    q.caput      = III;
    q.initium    = X;
    q.vestigium  = chorda_ex_literis("Nusquam", c->piscina);
    CREDO_VERUM(lector_positionem_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), &q));
    reditus(w);
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(campus_est(w, "nota", "position not found"));
    CREDO_VERUM(pictum(w, "position not found", c->piscina));

    CREDO_GRADUS("motus notam tollit");
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "nota", NIHIL));

    mundum_claudere(w);
}

/* pagina parva (CCXL x LXXX), caput longum: omnis pagina post motum et
 * reaperitionem eadem (positio = sententia prima quae in pagina
 * incipit) */
interior vacuum
_sectio_rotunda (
    CredoContextus* c)
{
            Mundus* w;
            chorda  caput;
            chorda  pagina;
     LectorPositio  q;
               i32  paginae;
               i32  i;

    w = mundus(c, "probationes/fixa/lector/longus", LXXX);
    CREDO_NECESSE_NON_NIHIL(w);
    reditus(w);

    CREDO_GRADUS("[ ad paginam ULTIMAM capitis prioris (caput I: II "
        "paginae in area parva)");
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "caput", "2"));
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "caput", "1"));
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    mundum_claudere(w);

    /* rotunda: latus CXII (area LXXX px = V lineae spatio duplici) -
     * pagina quaeque initium sententiae habet (sententiae < C
     * litterae);
     * in area minore pagina tota intra sententiam unam iacere potest,
     * quae reaperta paginam priorem ostendit (decisio I, accepta) */
    w = mundus(c, "probationes/fixa/lector/longus", CXII);
    CREDO_NECESSE_NON_NIHIL(w);
    reditus(w);
    per (i = ZEPHYRUM; i < X && !campus_est(w, "caput", "3"); i++)
    {
        clavis(w, ']');
    }
    CREDO_NECESSE(campus_est(w, "caput", "3"));

    CREDO_GRADUS("caput III: paginae omnes rotundae (ultima excepta, "
        "si "
        "sententia nulla in ea incipit: tunc priorem ostendit)");
    paginae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < C; i++)
    {
        caput   = chorda_transcribere(campus(w, "caput"), c->piscina);
        pagina  = chorda_transcribere(campus(w, "pagina"), c->piscina);
        si (!chorda_aequalis_literis(caput, "3"))
        {
            frange;
        }
        paginae++;
        effugium(w);
        effugium(w);
        reditus(w);
        CREDO_CHORDA_AEQUALIS(campus(w, "caput"), caput);
        si (!chorda_aequalis(campus(w, "pagina"), pagina))
        {
            /* decisio I: pagina sine initio sententiae - solum ultima
             * licet (] ad eam redit, ] iterum nihil) */
            clavis(w, ']');
            CREDO_CHORDA_AEQUALIS(campus(w, "pagina"), pagina);
            clavis(w, ']');
            CREDO_CHORDA_AEQUALIS(campus(w, "pagina"), pagina);
            frange;
        }
        clavis(w, ']');
        si (chorda_aequalis(campus(w, "pagina"), pagina))
        {
            frange;   /* finis libri */
        }
    }
    CREDO_VERUM(paginae > V);

    CREDO_GRADUS("reperire per vestigium: offset 0, sententia VIII "
        "longe -> pagina eius, non prima");
    effugium(w);
    effugium(w);
    q.caput      = III;
    q.initium    = ZEPHYRUM;
    q.vestigium  = chorda_ex_literis("Sententia 8 ", c->piscina);
    CREDO_VERUM(lector_positionem_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:longus", c->piscina), &q));
    reditus(w);
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_FALSUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(campus_est(w, "nota", NIHIL));
    CREDO_VERUM(pictum(w, "Sententia 8 ", c->piscina));

    mundum_claudere(w);
}

/* SemitaOrganum probationis: manifestum lectoris (ctx = Mundus*) */
interior constans Organum*
organa_testis (
    vacuum* ctx,
    chorda  genus)
{
    Mundus* w;

    w = (Mundus*)ctx;
    redde chorda_aequalis_literis(genus, "lector") ? w->organum : NIHIL;
}

/* latus alterum super EANDEM bibliothecam (et volumen) - ut latus
 * alterum vici aut restitutio post initium */
interior Mundus*
mundus_alter (
    CredoContextus* c,
            Mundus* prior)
{
    Mundus* w;
     Motus* motus;

    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern       = prior->intern;
    w->bibliotheca  = prior->bibliotheca;
    w->repo         = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !lector_montare(&w->m, c->piscina, w->intern, w->repo, "l2",
               w->bibliotheca, LATITUDO, ALTITUDO, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, lector_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    w->organum = organum_legere_genus(c->piscina, w->intern, "lector",
        "lector");
    redde w;
}

interior b32
argumentum (
                 Mundus* w,
     constans character* uri)
{
    b32 ok;

    ok = lector_argumentum_ponere(&w->m, uri);
    dispensator_recomponere(w->d);
    redde ok;
}

#define PAGINA_ALPHA "lector/pagina/urn%3Arhubarb%3Aalpha"

interior vacuum
_sectio_semitae (
    CredoContextus* c)
{
           Mundus* w;
           Mundus* restitutus;
    LectorPositio  q;
           chorda  locus;
           chorda  persistens;
           chorda  vitium;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("URI paginae: caput II, positio scripta (motus)");
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=2&pagina=0"));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "liber", "urn:rhubarb:alpha"));
    CREDO_VERUM(campus_est(w, "caput", "2"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, II);

    CREDO_GRADUS("locus et titulus loci");
    locus = lector_locus(&w->m, c->piscina);
    CREDO_VERUM(chorda_continet(locus, chorda_ex_literis(PAGINA_ALPHA,
        c->piscina)));
    CREDO_VERUM(chorda_continet(locus, chorda_ex_literis("caput=2",
        c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(lector_locus_titulus(&w->m,
        c->piscina), "Alpha - Caput II - 1/1");

    CREDO_GRADUS("semita persistens: visus et liber, NON caput et "
        "pagina");
    persistens = semita_ad_uri(organa_testis, w,
        semita_legere_persistentem(
        organa_testis, w, &w->m.ramus, "lector", c->piscina, w->intern),
        c->piscina);
    CREDO_VERUM(chorda_continet(persistens, chorda_ex_literis(
        PAGINA_ALPHA, c->piscina)));
    CREDO_FALSUM(chorda_continet(persistens, chorda_ex_literis("caput",
        c->piscina)));
    persistens = chorda_transcribere(persistens, c->piscina);

    CREDO_GRADUS("locus ad latus alterum: eaedem campi");
    restitutus = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(restitutus);
    CREDO_VERUM(argumentum(restitutus, chorda_ut_cstr(locus,
        c->piscina)));
    CREDO_VERUM(campus_est(restitutus, "caput", "2"));
    CREDO_VERUM(campus_est(restitutus, "pagina", "0"));

    CREDO_GRADUS("restitutio post motum alterius lateris: positio "
        "LIBRI, non pagina vetus lateris");
    CREDO_VERUM(argumentum(restitutus,
        PAGINA_ALPHA "?caput=3&pagina=0"));
    restitutus = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(restitutus);
    CREDO_VERUM(argumentum(restitutus, chorda_ut_cstr(persistens,
        c->piscina)));
    CREDO_VERUM(campus_est(restitutus, "caput", "3"));

    CREDO_GRADUS("latus iam in libro (caput II), alterum ad III motum: "
        "identitas nuda -> III, positio non rescripta");
    CREDO_VERUM(campus_est(w, "caput", "2"));
    CREDO_VERUM(argumentum(w, "urn:rhubarb:alpha"));
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, III);

    CREDO_GRADUS("identitas nuda: liber ad positionem suam");
    CREDO_VERUM(argumentum(w, ""));
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));
    CREDO_CHORDA_AEQUALIS_LITERIS(lector_locus_titulus(&w->m,
        c->piscina), "Library");
    CREDO_VERUM(argumentum(w, "urn:rhubarb:alpha"));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "3"));

    CREDO_GRADUS("index: titulus loci = titulus libri");
    clavis(w, 't');
    CREDO_CHORDA_AEQUALIS_LITERIS(lector_locus_titulus(&w->m,
        c->piscina), "Alpha");

    CREDO_GRADUS("liber ignotus et URI malum: FALSUM, nihil mutatum");
    CREDO_FALSUM(argumentum(w, "lector/pagina/nemo"));
    CREDO_FALSUM(argumentum(w, "lector/nusquam/x"));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "liber", "urn:rhubarb:alpha"));

    (vacuum)vitium;
    mundum_claudere(w);
}

/* positio (px) textus picti continentis fragmentum; FALSUM si nullus */
interior b32
positio_picta (
                 Mundus* w,
     constans character* fragmentum,
                    s32* x,
                    s32* y,
                Piscina* p)
{
               Mandata* md;
     constans Mandatum* t;
                   i32  i;

    md = reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        t = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   t->genus == MANDATUM_TEXTUS
            && chorda_continet(t->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            *x = t->fines.x;
            *y = t->fines.y;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* margo dexter textus picti (px, FONS_PARVUS): x + runae * VI
 * maximum */
interior s32
dextrum_maximum (
     Mundus* w,
    Piscina* p)
{
               Mandata* md;
     constans Mandatum* t;
                   i32  i;
                   i32  j;
                   s32  runae;
                   s32  maximum;

    md       = reddere(w, p);
    maximum  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        t = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (t->genus != MANDATUM_TEXTUS || t->fons != FONS_PARVUS)
        {
            perge;
        }
        runae = ZEPHYRUM;
        per (j = ZEPHYRUM; j < t->textus.mensura; j++)
        {
            si ((t->textus.datum[j] & 0xC0) != 0x80)
            {
                runae++;
            }
        }
        si (t->fines.x + runae * VI > maximum)
        {
            maximum = t->fines.x + runae * VI;
        }
    }
    redde maximum;
}

/* margines paginae ut liber: X cellulae utrimque, VI lineae supra et
 * infra (in latere lato); titulus currens in margine supero, numerus
 * paginae in infero; latus angustum margines minuit (columna >= XL) */
interior vacuum
_sectio_margines (
    CredoContextus* c)
{
    Mundus* w;
       s32  x;
       s32  y;
       s32  x_titulus;
       s32  y_titulus;
       s32  x_folium;
       s32  y_folium;

    CREDO_GRADUS("latus CDLXXX x CDLXXII: X cellulae, VI lineae");
    w = mundus_magnitudine(c, BIBLIOTHECA, CDLXXX, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis(w, 'j');
    reditus(w);
    CREDO_VERUM(positio_picta(w, "Prooemium.", &x, &y, c->piscina));
    /* margo radicis (cellula una) + X cellulae; linea + VI lineae */
    CREDO_AEQUALIS_S32(x, VI + X * VI);
    CREDO_AEQUALIS_S32(y, VIII + VI * VIII);

    CREDO_GRADUS("titulus currens supra, numerus paginae infra");
    CREDO_VERUM(positio_picta(w, "Beginning", &x_titulus, &y_titulus,
        c->piscina));
    CREDO_VERUM(y_titulus < y);
    CREDO_VERUM(x_titulus > X * VI);
    CREDO_VERUM(positio_picta(w, "1/1", &x_folium, &y_folium,
        c->piscina));
    CREDO_VERUM(y_folium > ALTITUDO - VI * VIII - VIII);
    CREDO_VERUM(x_folium > CC);
    mundum_claudere(w);

    CREDO_GRADUS("paragraphi longi intra columnam: nulla linea in "
        "marginem dextrum (CDLXXX - VI - X cellulae)");
    w = mundus_magnitudine(c, "probationes/fixa/lector/longus", CDLXXX,
        ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    reditus(w);
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_VERUM(dextrum_maximum(w, c->piscina) > CCC);
    CREDO_VERUM(dextrum_maximum(w, c->piscina) <= CDLXXX - VI
        - X * VI);
    mundum_claudere(w);

    CREDO_GRADUS("latus angustum (CCXL): margo lateralis ad cellulam "
        "unam");
    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis(w, 'j');
    reditus(w);
    CREDO_VERUM(positio_picta(w, "Prooemium.", &x, &y, c->piscina));
    CREDO_AEQUALIS_S32(x, VI + VI);
    mundum_claudere(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bibliotheca", _sectio_bibliotheca, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae_ordinum", _sectio_paginae_ordinum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pagina", _sectio_pagina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "positio", _sectio_positio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rotunda", _sectio_rotunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "semitae", _sectio_semitae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margines", _sectio_margines, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector", SECTIONES);
}
