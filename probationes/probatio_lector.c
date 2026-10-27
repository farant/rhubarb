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
#include <stdio.h>
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

/* textus primus continens fragmentum in colore thematis dato? */
interior b32
pictum_in_colore (
                  Mundus* w,
      constans character* fragmentum,
              ColorThema  color,
                 Piscina* p)
{
               Mandata* md;
     constans Mandatum* x;
          ColorMandati  expectatus;
                   i32  i;

    md          = reddere(w, p);
    expectatus  = figura_color_thematis(color);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde x->color.genus == expectatus.genus
                && x->color.valor == expectatus.valor;
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

    CREDO_GRADUS("ordo electus: obscura (auctor, progressus, "
        "unreadable) in colore fundi");
    CREDO_VERUM(pictum_in_colore(w, "Anna Scriptrix", COLOR_BACKGROUND,
        c->piscina));
    CREDO_VERUM(pictum_in_colore(w, "unreadable", COLOR_TEXT_DIM,
        c->piscina));
    clavis(w, 'j');
    CREDO_VERUM(pictum_in_colore(w, "Anna Scriptrix", COLOR_TEXT_DIM,
        c->piscina));
    CREDO_VERUM(pictum_in_colore(w, "unreadable", COLOR_BACKGROUND,
        c->piscina));
    clavis(w, 'k');

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

#define SENTENTIA_ALPHA "lector/sententia/urn%3Arhubarb%3Aalpha"

interior vacuum
emendationem_ponere (
                    Mundus* w,
                       Xar* lista,
    CodexEmendationisGenus  genus,
                       i32  caput,
                       i32  positio,
        constans character* vestigium,
                   Piscina* p)
{
    LectorEmendatio* e;

    e             = (LectorEmendatio*)xar_addere(lista);
    e->genus      = genus;
    e->caput      = caput;
    e->positio    = positio;
    e->vestigium  = chorda_ex_literis(vestigium, p);
    (vacuum)w;
}

interior vacuum
_sectio_sententia (
    CredoContextus* c)
{
           Mundus* w;
           Mundus* alter;
    LectorPositio  q;
              Xar* lista;
           chorda  persistens;
              i32  primum;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    primum = locus_in_capite(w, III, "Primum hic.", c->piscina);

    CREDO_GRADUS("s ex pagina: sententia prima paginae (titulus II)");
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=2&pagina=0"));
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "visus", "sententia"));
    CREDO_VERUM(campus_est(w, "sententia", "0"));
    CREDO_VERUM(pictum(w, "sentence 1/2", c->piscina));

    CREDO_GRADUS("j: sententia proxima, positio scripta; j trans "
        "capita");
    clavis(w, 'j');
    CREDO_VERUM(pictum(w, "Secundum caput.", c->piscina));
    CREDO_VERUM(pictum(w, "sentence 2/2", c->piscina));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(q.vestigium, "Secundum caput.");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "caput", "3"));
    CREDO_VERUM(campus_est(w, "sententia", "0"));
    CREDO_VERUM(pictum(w, "line one", c->piscina));
    CREDO_VERUM(pictum(w, "sentence 1/5", c->piscina));

    CREDO_GRADUS("k trans capita retro; s ad paginam sententiae");
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "caput", "2"));
    CREDO_VERUM(pictum(w, "Secundum caput.", c->piscina));
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "2"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("s ad positionem servatam si in pagina est");
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=0"));
    clavis(w, 'j');
    clavis(w, 'j');
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    clavis(w, 's');
    CREDO_VERUM(pictum(w, "Primum hic.", c->piscina));

    CREDO_GRADUS("semita sententiae; locus; semita persistens sine "
        "sententia");
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=31"));
    CREDO_VERUM(campus_est(w, "visus", "sententia"));
    CREDO_VERUM(pictum(w, "Secundum ibi.", c->piscina));
    CREDO_VERUM(chorda_continet(lector_locus(&w->m, c->piscina),
        chorda_ex_literis("sententia=31", c->piscina)));
    persistens = semita_ad_uri(organa_testis, w,
        semita_legere_persistentem(
        organa_testis, w, &w->m.ramus, "lector", c->piscina, w->intern),
        c->piscina);
    CREDO_FALSUM(chorda_continet(persistens, chorda_ex_literis(
        "sententia=", c->piscina)));
    CREDO_VERUM(chorda_continet(persistens, chorda_ex_literis(
        "lector/sententia/", c->piscina)));

    CREDO_GRADUS("emendatio servata applicatur (iuncta: 'Primum hic. "
        "Secundum ibi.', IV sententiae)");
    lista = xar_creare(c->piscina, (i32)magnitudo(LectorEmendatio));
    emendationem_ponere(w, lista, CODEX_IUNGERE, III, primum,
        "Primum hic.", c->piscina);
    CREDO_VERUM(lector_emendationes_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), lista));
    alter = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(argumentum(alter,
        SENTENTIA_ALPHA "?caput=3&sententia=31"));
    CREDO_VERUM(pictum(alter, "Primum hic. Secundum ibi.", c->piscina));
    CREDO_VERUM(pictum(alter, "sentence 3/4", c->piscina));

    CREDO_GRADUS("emendatio amissa: non applicata, numerata");
    emendationem_ponere(w, lista, CODEX_FINDERE, III, V, "Nusquam",
        c->piscina);
    CREDO_VERUM(lector_emendationes_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), lista));
    alter = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(argumentum(alter,
        SENTENTIA_ALPHA "?caput=3&sententia=0"));
    CREDO_VERUM(pictum(alter, "1 mark lost", c->piscina));
    CREDO_VERUM(pictum(alter, "sentence 1/4", c->piscina));

    mundum_claudere(w);

    CREDO_GRADUS("sententia longior latere parvo: '...' in pede");
    w = mundus(c, "probationes/fixa/lector/longus", LXIV);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w,
        "lector/sententia/urn%3Arhubarb%3Alongus?caput=3&sententia=0"));
    CREDO_FALSUM(pictum(w, "...", c->piscina));
    clavis(w, 'j');
    CREDO_VERUM(pictum(w, "...", c->piscina));
    mundum_claudere(w);
}

interior i32
emendationes_numerus (
                 Mundus* w,
                Piscina* p)
{
    redde xar_numerus(lector_emendationes_legere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", p), p));
}

interior i32
favores_numerus (
                 Mundus* w,
                Piscina* p)
{
    redde xar_numerus(lector_favores_legere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", p), p));
}

/* textus pictus EXACTE aequalis? (stella '*' sola) */
interior b32
pictum_exacte (
                 Mundus* w,
     constans character* textus,
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
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_emendare (
    CredoContextus* c)
{
                      Mundus* w;
                      Mundus* alter;
               LectorPositio  q;
                         Xar* lista;
                         i32  ibi;
                         i32  secundum;
    constans LectorEmendatio* e;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    ibi       = locus_in_capite(w, III, "ibi.", c->piscina);
    secundum  = locus_in_capite(w, III, "Secundum ibi.", c->piscina);

    CREDO_GRADUS("*: favor scriptus, stella picta; alia sententia "
        "sine");
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=31"));
    clavis(w, '*');
    CREDO_AEQUALIS_I32(favores_numerus(w, c->piscina), I);
    CREDO_VERUM(pictum_exacte(w, "*", c->piscina));
    clavis(w, 'k');
    CREDO_FALSUM(pictum_exacte(w, "*", c->piscina));

    CREDO_GRADUS("J: iuncta pingitur, IUNGERE una; stella partis "
        "(favor in 'Secundum'), positio in parte manet");
    q.caput      = III;
    q.initium    = (i32)secundum;
    q.vestigium  = chorda_ex_literis("Secundum ibi.", c->piscina);
    CREDO_VERUM(lector_positionem_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), &q));
    clavis(w, 'J');
    CREDO_VERUM(pictum(w, "Primum hic. Secundum ibi.", c->piscina));
    CREDO_VERUM(pictum(w, "sentence 3/4", c->piscina));
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), I);
    CREDO_VERUM(pictum_exacte(w, "*", c->piscina));
    CREDO_VERUM(argumentum(w, "urn:rhubarb:alpha"));
    clavis(w, 's');
    CREDO_VERUM(pictum(w, "Primum hic. Secundum ibi.", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "*", c->piscina));

    CREDO_GRADUS("latus alterum iam in capite: iunctam videt (versio)");
    alter = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(argumentum(alter,
        SENTENTIA_ALPHA "?caput=3&sententia=0"));
    CREDO_VERUM(pictum(alter, "sentence 1/4", c->piscina));

    CREDO_GRADUS("J iterum (catena): iunctura ad 'Secundum'; latus "
        "alterum renovatur");
    clavis(w, 'J');
    /* XXXIV litterae: in latere angusto duae lineae */
    CREDO_VERUM(pictum(w, "sentence 3/3", c->piscina));
    CREDO_VERUM(pictum(w, "Tertium!", c->piscina));
    lista = lector_emendationes_legere(w->bibliotheca,
        chorda_ex_literis(
        "urn:rhubarb:alpha", c->piscina), c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(lista), II);
    e = (constans LectorEmendatio*)xar_obtinere(lista, I);
    CREDO_AEQUALIS_I32(e->positio, (i32)secundum);
    CREDO_VERUM(pictum(alter, "sentence 1/3", c->piscina));

    CREDO_GRADUS("J in ultima capitis: nihil");
    clavis(w, 'J');
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), II);

    CREDO_GRADUS("J in ultima clausulae (versus 'line two' ante "
        "paragraphum): nihil");
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=9"));
    CREDO_VERUM(pictum_exacte(w, "line two", c->piscina));
    clavis(w, 'J');
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), II);
    CREDO_VERUM(pictum_exacte(w, "line two", c->piscina));
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=19"));

    CREDO_GRADUS("* in iuncta cum parte favorita: favores omnes "
        "partium "
        "tolluntur");
    clavis(w, '*');
    CREDO_AEQUALIS_I32(favores_numerus(w, c->piscina), ZEPHYRUM);
    CREDO_FALSUM(pictum_exacte(w, "*", c->piscina));

    CREDO_GRADUS("inversa: fissura servata ad 'ibi', J in 'Secundum' "
        "eam "
        "tollit (lista vacua)");
    lista = xar_creare(c->piscina, (i32)magnitudo(LectorEmendatio));
    emendationem_ponere(w, lista, CODEX_FINDERE, III, ibi, "ibi.",
        c->piscina);
    CREDO_VERUM(lector_emendationes_scribere(w->bibliotheca,
        chorda_ex_literis("urn:rhubarb:alpha", c->piscina), lista));
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=31"));
    CREDO_VERUM(pictum_exacte(w, "Secundum", c->piscina));
    clavis(w, 'J');
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), ZEPHYRUM);
    CREDO_VERUM(pictum(w, "Secundum ibi.", c->piscina));

    mundum_claudere(w);
}

interior vacuum
_sectio_findere (
    CredoContextus* c)
{
                      Mundus* w;
    constans LectorEmendatio* e;
                         Xar* lista;
                         i32  hic_locus;
                         i32  secundum;
                         i32  ibi;
                   character  numerus[XVI];

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    hic_locus  = locus_in_capite(w, III, "hic.", c->piscina);
    secundum   = locus_in_capite(w, III, "Secundum ibi.", c->piscina);
    ibi        = locus_in_capite(w, III, "ibi.", c->piscina);

    CREDO_GRADUS("i: modus findendi, cursor in verbo secundo");
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=31"));
    clavis(w, 'i');
    sprintf(numerus, "%u", (insignatus integer)ibi);
    CREDO_VERUM(campus_est(w, "cursor_findendi", numerus));

    CREDO_GRADUS("Enter bis in eodem verbo: nulla fissura; Esc nihil "
        "servat");
    reditus(w);
    reditus(w);
    effugium(w);
    CREDO_VERUM(campus_est(w, "cursor_findendi", NIHIL));
    CREDO_VERUM(campus_est(w, "visus", "sententia"));
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), ZEPHYRUM);

    CREDO_GRADUS("Enter: linea vacua ante verbum; Esc: FINDERE ad "
        "verbum, duae sententiae");
    clavis(w, 'i');
    reditus(w);
    CREDO_VERUM(pictum_exacte(w, "Secundum", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "ibi.", c->piscina));
    effugium(w);
    lista = lector_emendationes_legere(w->bibliotheca,
        chorda_ex_literis(
        "urn:rhubarb:alpha", c->piscina), c->piscina);
    CREDO_AEQUALIS_I32(xar_numerus(lista), I);
    e = (constans LectorEmendatio*)xar_obtinere(lista, ZEPHYRUM);
    CREDO_VERUM(e->genus == CODEX_FINDERE);
    CREDO_AEQUALIS_I32(e->positio, ibi);
    CREDO_VERUM(pictum(w, "sentence 4/6", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Secundum", c->piscina));

    CREDO_GRADUS("J reddit (fissuram tollit); deinde J in 'Primum', i, "
        "w, "
        "Enter ante 'Secundum', Esc: iunctura tollitur (lista vacua)");
    clavis(w, 'J');
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), ZEPHYRUM);
    clavis(w, 'k');
    clavis(w, 'J');
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), I);
    clavis(w, 'i');
    sprintf(numerus, "%u", (insignatus integer)hic_locus);
    CREDO_VERUM(campus_est(w, "cursor_findendi", numerus));
    clavis(w, 'w');
    sprintf(numerus, "%u", (insignatus integer)secundum);
    CREDO_VERUM(campus_est(w, "cursor_findendi", numerus));
    clavis(w, 'b');
    sprintf(numerus, "%u", (insignatus integer)hic_locus);
    CREDO_VERUM(campus_est(w, "cursor_findendi", numerus));
    clavis(w, 'l');
    reditus(w);
    CREDO_VERUM(pictum_exacte(w, "Primum hic.", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Secundum ibi.", c->piscina));
    effugium(w);
    CREDO_AEQUALIS_I32(emendationes_numerus(w, c->piscina), ZEPHYRUM);
    CREDO_VERUM(pictum(w, "sentence 3/5", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Primum hic.", c->piscina));

    CREDO_GRADUS("b in verbo secundo: manet (verbum primum non "
        "finditur)");
    clavis(w, 'i');
    clavis(w, 'b');
    clavis(w, 'h');
    sprintf(numerus, "%u", (insignatus integer)hic_locus);
    CREDO_VERUM(campus_est(w, "cursor_findendi", numerus));
    effugium(w);

    mundum_claudere(w);

    CREDO_GRADUS("sententia in 6x8: partes in 6x8 manent, non "
        "praecisae");
    w = mundus(c, "probationes/fixa/lector/longus", CXII);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(argumentum(w,
        "lector/sententia/urn%3Arhubarb%3Alongus?caput=3&sententia=0"));
    clavis(w, 'j');
    CREDO_VERUM(pictum_in_fonte(w, "puella", FONS_PARVUS, c->piscina));
    CREDO_FALSUM(pictum_in_fonte(w, "puella", FONS_TITULI, c->piscina));
    clavis(w, 'i');
    reditus(w);
    CREDO_VERUM(pictum_exacte(w, "Sententia", c->piscina));
    CREDO_VERUM(pictum_in_fonte(w, "puella", FONS_PARVUS, c->piscina));
    CREDO_FALSUM(pictum_in_fonte(w, "puella", FONS_TITULI, c->piscina));
    CREDO_FALSUM(pictum(w, "...", c->piscina));
    effugium(w);
    mundum_claudere(w);
}

/* favor ad fragmentum capitis k libri Alpha in lista */
interior vacuum
favorem_ponere (
                 Mundus* w,
                    Xar* lista,
                    i32  caput,
     constans character* fragmentum,
                Piscina* p)
{
    LectorPositio* q;

    q             = (LectorPositio*)xar_addere(lista);
    q->caput      = caput;
    q->initium    = locus_in_capite(w, caput, fragmentum, p);
    q->vestigium  = chorda_ex_literis(fragmentum, p);
}

/* favores Alpha: I "In primis"; III "line one", "line two", "Primum
 * hic." (cursus trans paragraphum), "Tertium!" (solus) = ordines III */
interior b32
favores_libri (
     Mundus* w,
    Piscina* p)
{
    Xar* lista;

    lista = xar_creare(p, (i32)magnitudo(LectorPositio));
    favorem_ponere(w, lista, I, "In primis", p);
    favorem_ponere(w, lista, III, "line one", p);
    favorem_ponere(w, lista, III, "line two", p);
    favorem_ponere(w, lista, III, "Primum hic.", p);
    favorem_ponere(w, lista, III, "Tertium!", p);
    redde lector_favores_scribere(w->bibliotheca, chorda_ex_literis(
        "urn:rhubarb:alpha", p), lista);
}

interior vacuum
_sectio_favores (
    CredoContextus* c)
{
           Mundus* w;
    LectorPositio  q;
              i32  secundum;
              i32  linea_altera;
              i32  tertium;
        character  numerus[XVI];

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);
    secundum = locus_in_capite(w, III, "Secundum ibi.",
        c->piscina);
    linea_altera  = locus_in_capite(w, III, "line two", c->piscina);
    tertium       = locus_in_capite(w, III, "Tertium!", c->piscina);

    CREDO_GRADUS("nulli favores: 'No favourites yet'; Esc ad paginam");
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=3&pagina=0"));
    clavis(w, 'f');
    CREDO_VERUM(campus_est(w, "visus", "favores"));
    CREDO_VERUM(pictum(w, "No favourites yet", c->piscina));
    CREDO_VERUM(chorda_continet(lector_locus_titulus(&w->m, c->piscina),
        chorda_ex_literis(" - Favourites", c->piscina)));
    effugium(w);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "caput", "3"));

    CREDO_GRADUS("greges II cum titulis capitum; consecutivi (etiam "
        "trans paragraphum) ordo unus");
    CREDO_VERUM(favores_libri(w, c->piscina));
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=31"));
    clavis(w, 'f');
    CREDO_VERUM(pictum_exacte(w, "Caput I", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Caput III", c->piscina));
    CREDO_VERUM(pictum(w, "In primis", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "line one", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "line two", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Primum hic.", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Tertium!", c->piscina));
    CREDO_FALSUM(pictum(w, "Secundum", c->piscina));
    CREDO_VERUM(pictum(w, "3 favourites", c->piscina));

    CREDO_GRADUS("j k [ ] lustrant: positio libri non movetur");
    clavis(w, 'j');
    clavis(w, 'j');
    clavis(w, 'j');
    clavis(w, 'k');
    clavis(w, ']');
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "electus_favor", "1"));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.caput, III);
    CREDO_AEQUALIS_I32(q.initium, secundum);

    CREDO_GRADUS("Enter in 'Tertium!': ad sententiam eius (motus: "
        "positio scribitur)");
    clavis(w, 'j');
    reditus(w);
    sprintf(numerus, "%u", (insignatus integer)tertium);
    CREDO_VERUM(campus_est(w, "visus", "sententia"));
    CREDO_VERUM(campus_est(w, "sententia", numerus));
    CREDO_VERUM(pictum_exacte(w, "Tertium!", c->piscina));
    CREDO_VERUM(positio_servata(w, "urn:rhubarb:alpha", &q,
        c->piscina));
    CREDO_AEQUALIS_I32(q.initium, tertium);

    CREDO_GRADUS("f ex sententia, Esc redit ad sententiam eandem");
    clavis(w, 'f');
    CREDO_VERUM(campus_est(w, "visus", "favores"));
    effugium(w);
    CREDO_VERUM(campus_est(w, "visus", "sententia"));
    CREDO_VERUM(campus_est(w, "sententia", numerus));

    CREDO_GRADUS("* in media cursus: cursus in duos finditur");
    sprintf(numerus, "%u", (insignatus integer)linea_altera);
    CREDO_VERUM(argumentum(w, SENTENTIA_ALPHA "?caput=3&sententia=9"));
    CREDO_VERUM(campus_est(w, "sententia", numerus));
    clavis(w, '*');
    clavis(w, 'f');
    CREDO_VERUM(pictum(w, "4 favourites", c->piscina));
    CREDO_FALSUM(pictum(w, "line two", c->piscina));
    effugium(w);
    mundum_claudere(w);

    CREDO_GRADUS("latus parvum: titulus gregis in pagina "
        "continuationis iteratur");
    w = mundus(c, BIBLIOTHECA, CXII);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(favores_libri(w, c->piscina));
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=3&pagina=0"));
    clavis(w, 'f');
    clavis(w, 'j');
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_favor", "2"));
    CREDO_VERUM(pictum_exacte(w, "Tertium!", c->piscina));
    CREDO_FALSUM(pictum_exacte(w, "line one", c->piscina));
    CREDO_VERUM(pictum_exacte(w, "Caput III", c->piscina));
    mundum_claudere(w);
}

/* locus continens fragmentum? */
interior b32
locus_continet (
                 Mundus* w,
     constans character* fragmentum,
                Piscina* p)
{
    redde chorda_continet(lector_locus(&w->m, p), chorda_ex_literis(
        fragmentum, p));
}

/* lector L5: locus per visum - pagina = liber, caput, pagina;
 * sententia = liber, caput, sententia; ceteri = liber solus */
interior vacuum
_sectio_loci (
    CredoContextus* c)
{
     Mundus* w;
     Mundus* alter;
     chorda  locus;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("pagina: caput et pagina, non sententia");
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=3&pagina=0"));
    CREDO_VERUM(locus_continet(w, PAGINA_ALPHA, c->piscina));
    CREDO_VERUM(locus_continet(w, "caput=3", c->piscina));
    CREDO_VERUM(locus_continet(w, "pagina=0", c->piscina));
    CREDO_FALSUM(locus_continet(w, "sententia=", c->piscina));

    CREDO_GRADUS("s: sententia sine pagina; latus alterum (semita "
        "sententiae sola) eundem locum habet");
    clavis(w, 's');
    CREDO_VERUM(locus_continet(w, SENTENTIA_ALPHA, c->piscina));
    CREDO_VERUM(locus_continet(w, "caput=3", c->piscina));
    CREDO_VERUM(locus_continet(w, "sententia=", c->piscina));
    CREDO_FALSUM(locus_continet(w, "pagina=", c->piscina));
    locus = chorda_transcribere(lector_locus(&w->m, c->piscina),
        c->piscina);
    alter = mundus_alter(c, w);
    CREDO_NECESSE_NON_NIHIL(alter);
    CREDO_VERUM(argumentum(alter, chorda_ut_cstr(locus, c->piscina)));
    CREDO_VERUM(chorda_aequalis(lector_locus(&alter->m, c->piscina),
        locus));

    CREDO_GRADUS("favores, index, bibliotheca: liber solus");
    clavis(w, 'f');
    CREDO_VERUM(locus_continet(w, "lector/favores/", c->piscina));
    CREDO_FALSUM(locus_continet(w, "?", c->piscina));
    effugium(w);
    clavis(w, 't');
    CREDO_VERUM(locus_continet(w, "lector/index/", c->piscina));
    CREDO_FALSUM(locus_continet(w, "?", c->piscina));
    effugium(w);
    CREDO_VERUM(campus_est(w, "visus", "bibliotheca"));
    CREDO_FALSUM(locus_continet(w, "?", c->piscina));
    mundum_claudere(w);
}

/* hospes fictus notarum: locus notatus unus (ctx = chorda*) */
interior b32
notas_fictas_habere (
    vacuum* ctx,
    chorda  locus)
{
    redde chorda_aequalis(*(chorda*)ctx, locus);
}

/* lector L5: signum notarum '¶' in pede paginae et sententiae, ex
 * hospite (notas_habet) pro locu currenti */
interior vacuum
_sectio_notae (
    CredoContextus* c)
{
     Mundus* w;
     chorda  notatus;

    w = mundus(c, BIBLIOTHECA, ALTITUDO);
    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("hospes nullus: signum nullum");
    CREDO_VERUM(argumentum(w, PAGINA_ALPHA "?caput=3&pagina=0"));
    CREDO_VERUM(pictum(w, "1/1", c->piscina));
    CREDO_FALSUM(pictum(w, "\302\266", c->piscina));

    CREDO_GRADUS("pagina cum notis: '1/1 \302\266' in pede");
    notatus = chorda_transcribere(lector_locus(&w->m, c->piscina),
        c->piscina);
    w->m.notas_habet      = notas_fictas_habere;
    w->m.notas_habet_ctx  = &notatus;
    CREDO_VERUM(pictum(w, "1/1 \302\266", c->piscina));

    CREDO_GRADUS("s: sententia sine notis; notata: signum");
    clavis(w, 's');
    CREDO_VERUM(pictum(w, "sentence 1/5", c->piscina));
    CREDO_FALSUM(pictum(w, "\302\266", c->piscina));
    notatus = chorda_transcribere(lector_locus(&w->m, c->piscina),
        c->piscina);
    CREDO_VERUM(pictum(w, "sentence 1/5 \302\266", c->piscina));

    CREDO_GRADUS("j: proxima sine signo; k redit cum signo");
    clavis(w, 'j');
    CREDO_FALSUM(pictum(w, "\302\266", c->piscina));
    clavis(w, 'k');
    CREDO_VERUM(pictum(w, "\302\266", c->piscina));

    CREDO_GRADUS("J: iuncta notas primae servat (locus = offset "
        "primum)");
    clavis(w, 'J');
    CREDO_VERUM(pictum(w, "sentence 1/4 \302\266", c->piscina));
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
    { "sententia", _sectio_sententia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "emendare", _sectio_emendare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "findere", _sectio_findere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "favores", _sectio_favores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "loci", _sectio_loci, NIHIL, NIHIL, NIHIL, NIHIL },
    { "notae", _sectio_notae, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector", SECTIONES);
}
