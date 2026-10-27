/* probatio_navigator_latus.c - navigator in latere (N3.2): index
 * per hospitem (electus j k, ictus, duplex), articulus paginatus (j k,
 * sagittae pedis, Esc), versio in loco (comes versionis suae), semitae
 * (argumentum, locus, titulus loci), bibliotheca vacua, notae in pede.
 * Bibliotheca in volumine temporario: Rhubarb (fixum verum, en),
 * Rheum et Beta (la; Beta duas versiones habet). */
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
#include "moneta.h"
#include "filum.h"
#include "vicipaedia.h"
#include "navigator_bibliotheca.h"
#include "navigator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  CCCLX
#define ALTITUDO  CDLXXII

#define PAGINA(titulus, corpus) \
    "<html><head><title>" titulus "</title></head><body>" \
    "<section data-mw-section-id=\"0\">" corpus "</section>" \
    "</body></html>"

#define URL_ANGLICUM "https://en.wikipedia.org/wiki/Rhubarb"
#define URL_RHEUM   "https://la.wikipedia.org/wiki/Rheum"
#define URL_BETA    "https://la.wikipedia.org/wiki/Beta"

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                 Volumen* volumen;
    NavigatorBibliotheca* bibliotheca;
                     i32  numerator;
       NavigatorMontatio  m;
             Dispensator* d;
                     b32  notae;      /* notas_habet probationis */
                 Piscina* p;
} Mundus;

interior b32
notas_habere (
    vacuum* ctx,
    chorda  locus)
{
    (vacuum)locus;
    redde ((Mundus*)ctx)->notae;
}

interior vacuum
offerre (
                 Mundus* w,
     constans character* url,
                 chorda  html)
{
    (vacuum)navigator_versionem_offerre(w->bibliotheca,
        chorda_ex_literis(
        url, w->p), chorda_ex_literis(url, w->p), "vicipaedia", html,
        vicipaedia_articulus, NIHIL);
}

/* latus super bibliothecam; plena: Rhubarb (1, versio 2), Rheum (3,
 * 4), Beta (5, versiones 6 et 7) */
interior Mundus*
mundus (
    CredoContextus* c,
               b32  plena)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    c->datum   = w;   /* purgare volumen claudit, etiam post NECESSE */
    w->p       = c->piscina;
    w->intern  = internamentum_creare(c->piscina);
    w->volumen  = volumen_temporarium(c->piscina,
        "probatio_navigator");
    w->bibliotheca  = w->volumen ? navigator_bibliotheca_aperire(
        w->volumen, moneta_numerator, &w->numerator,
        c->piscina) : NIHIL;
    si (!w->bibliotheca)
    {
        redde NIHIL;
    }
    si (plena)
    {
        offerre(w, URL_ANGLICUM, filum_legere_totum(
            "probationes/fixa/vicipaedia/rhubarb.html", c->piscina));
        offerre(w, URL_RHEUM, chorda_ex_literis(PAGINA("Rheum",
            "<p>Rheum est <a rel=\"mw:WikiLink\" href=\"./Planta\">"
            "genus</a> plantarum.</p>"), c->piscina));
        offerre(w, URL_BETA, chorda_ex_literis(PAGINA("Beta",
            "<p>Beta est planta.</p>"), c->piscina));
        offerre(w, URL_BETA, chorda_ex_literis(PAGINA("Beta",
            "<p>Beta vulgaris est olus.</p>"), c->piscina));
    }
    w->repo = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !navigator_montare(&w->m, c->piscina, w->intern, w->repo,
               "n1", w->bibliotheca, LATITUDO, ALTITUDO, NIHIL))
    {
        redde NIHIL;
    }
    w->m.notas_habet      = notas_habere;
    w->m.notas_habet_ctx  = w;
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, navigator_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
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
mundum_purgare (
    CredoContextus* c)
{
    mundum_claudere((Mundus*)c->datum);
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

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
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

/* ictus (aut duplex) in medio componentis id */
interior b32
ictus_in (
                 Mundus* w,
     constans character* id,
                    b32  geminus,
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
    e.genus             = geminus ? EVENTUS_MUS_DUPLEX
                                 : EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior b32
argumentum (
                 Mundus* w,
     constans character* arg)
{
    b32 r;

    r = navigator_argumentum_ponere(&w->m, arg);
    dispensator_recomponere(w->d);
    redde r;
}

interior vacuum
_sectio_index (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("latus novum: index per hospitem");
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(pictum(w, "Saved articles", c->piscina));
    CREDO_VERUM(pictum(w, "en.wikipedia.org", c->piscina));
    CREDO_VERUM(pictum(w, "la.wikipedia.org", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "Rheum", c->piscina));
    CREDO_VERUM(pictum(w, "2 versions", c->piscina));
    CREDO_VERUM(pictum(w, "1 version", c->piscina));

    CREDO_GRADUS("ordo: hospites, deinde tituli (Rhubarb Beta Rheum)");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "3"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "3"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));

    CREDO_GRADUS("mus: ictus eligit, duplex aperit");
    CREDO_VERUM(ictus_in(w, "r1", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "electus_articulus", "1"));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(ictus_in(w, "r3", VERUM, c->piscina));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));

    CREDO_GRADUS("Esc: ad indicem, electus manet");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "articulus", NIHIL));
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));

    CREDO_GRADUS("Enter: articulus electus, pagina prima");
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(campus_est(w, "versio", "7"));
    mundum_claudere(w);
}

interior vacuum
_sectio_pagina (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);
    clavis_nominata(w, CLAVIS_REDITUS, XIII);

    CREDO_GRADUS("Rhubarb: titulus, textus, pes (hospes, paginae)");
    CREDO_VERUM(campus_est(w, "articulus", "1"));
    CREDO_VERUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "fleshy, edible stalks", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb - en.wikipedia.org - 1/",
        c->piscina));

    CREDO_GRADUS("j k: paginae; limes ad initium");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(pictum(w, " - 2/", c->piscina));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("mus: sagittae pedis");
    CREDO_VERUM(ictus_in(w, "pagina-proxima", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(ictus_in(w, "pagina-prior", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("limes ad finem: j in pagina ultima manet");
    per (;;)
    {
        chorda* a = insula_ramus_attributum(&w->m.ramus, "pagina");
        chorda  ante = a ? chorda_transcribere(*a, c->piscina)
                         : chorda_ex_literis("", c->piscina);

        clavis(w, 'j');
        a = insula_ramus_attributum(&w->m.ramus, "pagina");
        si (a && chorda_aequalis(*a, ante))
        {
            frange;
        }
    }
    CREDO_VERUM(pictum(w, "Rhubarb - en.wikipedia.org", c->piscina));

    CREDO_GRADUS("notae: pilcrow in pede si locus notas habet");
    CREDO_FALSUM(pictum(w, "\302\266", c->piscina));
    w->notae = VERUM;
    CREDO_VERUM(pictum(w, "\302\266", c->piscina));
    mundum_claudere(w);
}

interior vacuum
_sectio_versio (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);
    chorda  locus;

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("sine versione: ultima, et versio FIGITUR (locus)");
    CREDO_VERUM(argumentum(w, "navigator/pagina/5"));
    CREDO_VERUM(pictum(w, "Beta vulgaris est olus.", c->piscina));
    CREDO_VERUM(campus_est(w, "versio", "7"));

    CREDO_GRADUS("versio nominata: vetus legitur");
    CREDO_VERUM(argumentum(w, "navigator/pagina/5?versio=6"));
    CREDO_VERUM(pictum(w, "Beta est planta.", c->piscina));
    CREDO_FALSUM(pictum(w, "vulgaris", c->piscina));

    CREDO_GRADUS("locus versionem nominat (comes versionis suae)");
    locus = navigator_locus(&w->m, c->piscina);
    CREDO_VERUM(chorda_incipit(locus, chorda_ex_literis(
        "navigator/pagina/5", c->piscina)));
    CREDO_VERUM(chorda_continet(locus, chorda_ex_literis("versio=6",
        c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Beta - la.wikipedia.org - 1/1");

    CREDO_GRADUS("index: locus sine articulo; 'Saved articles'");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    locus = navigator_locus(&w->m, c->piscina);
    CREDO_FALSUM(chorda_continet(locus, chorda_ex_literis("versio",
        c->piscina)));
    CREDO_FALSUM(chorda_continet(locus, chorda_ex_literis("/5",
        c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Saved articles");
    mundum_claudere(w);
}

interior vacuum
_sectio_argumentum (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("identitas nuda: articulus, pagina prima");
    CREDO_VERUM(argumentum(w, "3"));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));
    CREDO_VERUM(pictum(w, "Rheum est genus plantarum.", c->piscina));

    CREDO_GRADUS("index nexuum: pagina nova post articulum (N3.1b)");
    CREDO_VERUM(pictum(w, "Rheum - la.wikipedia.org - 1/2",
        c->piscina));
    CREDO_FALSUM(pictum(w, "Links", c->piscina));
    clavis(w, 'j');
    CREDO_VERUM(pictum(w, "Links", c->piscina));
    CREDO_VERUM(pictum(w, "Planta", c->piscina));
    CREDO_FALSUM(pictum(w, "plantarum", c->piscina));
    CREDO_VERUM(pictum(w, "Rheum - la.wikipedia.org - 2/2",
        c->piscina));
    clavis(w, 'k');
    CREDO_VERUM(pictum(w, "plantarum", c->piscina));

    CREDO_GRADUS("articulus ignotus: FALSUM, nihil mutatum");
    CREDO_FALSUM(argumentum(w, "99"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));
    CREDO_FALSUM(argumentum(w, "navigator/pagina/99"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));

    CREDO_GRADUS("vacuum: index");
    CREDO_VERUM(argumentum(w, ""));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    mundum_claudere(w);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Mundus* w = mundus(c, FALSUM);

    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_GRADUS("bibliotheca vacua: nuntius, Enter nihil");
    CREDO_VERUM(pictum(w, "No saved articles", c->piscina));
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    mundum_claudere(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index", _sectio_index, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "pagina", _sectio_pagina, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "versio", _sectio_versio, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "argumentum", _sectio_argumentum, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_latus", SECTIONES);
}
