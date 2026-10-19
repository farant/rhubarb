/* probatio_sancti_visus.c - bibliotheca sanctorum in latere vici
 * (include/sancti_visus.h). Fixtura: probationes/fixa/sancti (agnes
 * 21 I, sine anno; thomas 28 I, c. 1225-1274; charles 3 VI, d. 1886);
 * hodie fixum 25 I. Index ordine calendarii (electus primus a hodie),
 * ordo s (nomen, aetas, calendarium), j k, paginae [ ], t, quaestio /,
 * visus sancti (Enter, caput, Esc, j k inter sanctos, paginae
 * textus), argumentum lateris. */
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
#include "sancti.h"
#include "sancti_visus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define FIXA "probationes/fixa/sancti"
#define LINEOLA "\342\200\223"

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
            SanctiCorpus* corpus;
       SanctorumMontatio  m;
             Dispensator* d;
} Mundus;

interior b32
hodie_fixum (
    vacuum* ctx,
       s32* mensis,
       s32* dies)
{
    (vacuum)ctx;
    *mensis  = I;
    *dies    = XXV;
    redde VERUM;
}

/* latus CCC x altitudo super fixturam */
interior Mundus*
mundus (
    CredoContextus* c,
               i32  altitudo)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern = internamentum_creare(c->piscina);
    w->corpus = sancti_corpus_aperire(FIXA, c->piscina, w->intern);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->corpus || !w->repo
        || !sancti_visus_montare(&w->m, c->piscina, w->intern, w->repo,
               "s1", w->corpus, hodie_fixum, w, CCC, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, sancti_visus_componere, &w->m, CCC);
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
scribere (
                 Mundus* w,
     constans character* textus)
{
    dum (*textus)
    {
        clavis(w, *textus);
        textus++;
    }
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

/* index paginae pictus "k/n" (textus digitis et '/' solis): k, n */
interior b32
_index_paginae (
     Mundus* w,
    Piscina* p,
        s32* k,
        s32* n)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;
                   i32  j;
                   b32  forma;

    md = reddere(w, p);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus != MANDATUM_TEXTUS || x->textus.mensura < III)
        {
            perge;
        }
        forma = chorda_continet(x->textus, chorda_ex_literis("/", p));
        per (j = ZEPHYRUM; forma && j < x->textus.mensura; j++)
        {
            character ch = (character)x->textus.datum[j];

            si (!((ch >= '0' && ch <= '9') || ch == '/'))
            {
                forma = FALSUM;
            }
        }
        si (forma && sscanf(chorda_ut_cstr(x->textus, p), "%d/%d", k, n)
            == II)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* index: titulus, ordo calendarii, electus primus a hodie */
interior vacuum
_sectio_index (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(pictum(w, "Saints", c->piscina));
    CREDO_VERUM(pictum(w, "Thomas Aquinas", c->piscina));
    CREDO_VERUM(pictum(w, "Jan 28", c->piscina));
    CREDO_VERUM(pictum(w, "c. 1225" LINEOLA "1274", c->piscina));
    CREDO_VERUM(pictum(w, "Agnes of Rome", c->piscina));
    CREDO_VERUM(pictum(w, "d. 1886", c->piscina));
    CREDO_VERUM(pictum(w, "virgin, martyr", c->piscina));
    /* electus: primus a 25 I = thomas (28 I) */
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus",
        "charles-lwanga-and-companions"));
    clavis(w, 'k');
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
    /* in initio: nihil */
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
    clavis(w, 't');
    CREDO_VERUM(campus_est(w, "electus", "thomas-aquinas"));
}

/* ordo: s cyclum facit - nomen, aetas, calendarium */
interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "ordo", "nomen"));
    CREDO_VERUM(pictum(w, "by name", c->piscina));
    clavis(w, 't');
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus",
        "charles-lwanga-and-companions"));
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "ordo", "aetas"));
    clavis(w, 't');
    /* aetas: thomas 1274, charles 1886, agnes ignota ultima */
    CREDO_VERUM(campus_est(w, "electus", "thomas-aquinas"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus",
        "charles-lwanga-and-companions"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
    clavis(w, 's');
    CREDO_VERUM(campus_est(w, "ordo", "calendarium"));
}

/* paginae: latus humile (ordines duo per paginam) */
interior vacuum
_sectio_paginae (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CXX);
    CREDO_NECESSE_NON_NIHIL(w);
    /* thomas (secundus) in pagina prima: agnes, thomas | charles */
    CREDO_VERUM(pictum(w, "Agnes of Rome", c->piscina));
    CREDO_VERUM(!pictum(w, "Charles Lwanga", c->piscina));
    clavis(w, ']');
    CREDO_VERUM(pictum(w, "Charles Lwanga", c->piscina));
    CREDO_VERUM(campus_est(w, "electus",
        "charles-lwanga-and-companions"));
    clavis(w, '[');
    CREDO_VERUM(pictum(w, "Agnes of Rome", c->piscina));
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
}

/* quaestio: / aqu Enter -> thomas solus; Esc quaestionem tollit, Esc
 * iterum non consumitur (vicus latus tollit) */
interior vacuum
_sectio_quaestio (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis(w, '/');
    CREDO_VERUM(campus_est(w, "prompta", "quaestio"));
    scribere(w, "AQU");
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "quaestio", "AQU"));
    CREDO_VERUM(campus_est(w, "prompta", NIHIL)
        || campus_est(w, "prompta", "nulla"));
    CREDO_VERUM(pictum(w, "Thomas Aquinas", c->piscina));
    CREDO_VERUM(!pictum(w, "Agnes of Rome", c->piscina));
    CREDO_VERUM(campus_est(w, "electus", "thomas-aquinas"));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "quaestio", NIHIL));
    CREDO_VERUM(pictum(w, "Agnes of Rome", c->piscina));
    /* nihil inventum */
    clavis(w, '/');
    scribere(w, "zzz");
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(pictum(w, "No saints", c->piscina));
}

/* visus sancti: Enter, caput, j k, Esc */
interior vacuum
_sectio_sanctus (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI);
    CREDO_NECESSE_NON_NIHIL(w);
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "sanctus"));
    CREDO_VERUM(campus_est(w, "sanctus_apertus", "thomas-aquinas"));
    CREDO_VERUM(pictum(w, "Thomas Aquinas", c->piscina));
    CREDO_VERUM(pictum(w, "Thomas de Aquino", c->piscina));
    CREDO_VERUM(pictum(w, "Fossanova", c->piscina));
    CREDO_VERUM(pictum(w, "Canonized 18 July 1323 by John XXII",
        c->piscina));
    CREDO_VERUM(pictum(w, "Feast: 28 January (memorial)", c->piscina));
    CREDO_VERUM(pictum(w, "Sources:", c->piscina));
    /* j: sanctus proximus ordine indicis (charles) */
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "sanctus_apertus",
        "charles-lwanga-and-companions"));
    CREDO_VERUM(pictum(w, "Matthias Mulumba", c->piscina));
    clavis(w, 'k');
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "sanctus_apertus", "agnes-of-rome"));
    CREDO_VERUM(pictum(w, "No virgin martyr of Rome", c->piscina));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "electus", "agnes-of-rome"));
}

/* paginae textus: latus humile, descriptio agnetis paginas plures */
interior vacuum
_sectio_paginae_textus (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CLX);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(sancti_visus_argumentum_ponere(&w->m, "agnes-of-rome"));
    dispensator_recomponere(w->d);
    CREDO_VERUM(pictum(w, "No virgin martyr of Rome", c->piscina));
    CREDO_VERUM(!pictum(w, "Sources:", c->piscina));
    clavis(w, ']');
    CREDO_VERUM(campus_est(w, "pagina_textus", "1"));
    CREDO_VERUM(!pictum(w, "No virgin martyr of Rome", c->piscina));
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    clavis(w, ']');
    /* pagina ultima: fons ultimus (paragraphus fontium paginas
     * transit); ultra ultimam non progreditur */
    CREDO_VERUM(pictum(w, "USCCB calendar", c->piscina));
    {
        chorda* a;
        chorda  ultima;

        a = insula_ramus_attributum(&w->m.ramus, "pagina_textus");
        CREDO_NECESSE(a && a->mensura > ZEPHYRUM);
        ultima = chorda_transcribere(*a, c->piscina);
        clavis(w, ']');
        CREDO_VERUM(campus_est(w, "pagina_textus",
            chorda_ut_cstr(ultima, c->piscina)));
    }
    {
        s32 k;
        s32 n;

        /* index pictus: ultima = n/n; una retro = (n-1)/n */
        CREDO_NECESSE(_index_paginae(w, c->piscina, &k, &n));
        CREDO_VERUM(n > I);
        CREDO_AEQUALIS_S32(k, n);
        clavis(w, '[');
        CREDO_NECESSE(_index_paginae(w, c->piscina, &k, &n));
        CREDO_AEQUALIS_S32(k, n - I);
    }
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    clavis(w, '[');
    CREDO_VERUM(campus_est(w, "pagina_textus", NIHIL)
        || campus_est(w, "pagina_textus", "0"));
}

/* argumentum lateris */
interior vacuum
_sectio_argumentum (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c, CCLVI);
    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_VERUM(sancti_visus_argumentum_ponere(&w->m,
        "charles-lwanga-and-companions"));
    CREDO_VERUM(campus_est(w, "visus", "sanctus"));
    CREDO_VERUM(campus_est(w, "sanctus_apertus",
        "charles-lwanga-and-companions"));
    CREDO_FALSUM(sancti_visus_argumentum_ponere(&w->m, "nemo"));
    CREDO_VERUM(campus_est(w, "sanctus_apertus",
        "charles-lwanga-and-companions"));
    CREDO_VERUM(sancti_visus_argumentum_ponere(&w->m, ""));
    CREDO_VERUM(campus_est(w, "visus", "index"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index", _sectio_index, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordo", _sectio_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae", _sectio_paginae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quaestio", _sectio_quaestio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sanctus", _sectio_sanctus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae_textus", _sectio_paginae_textus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "argumentum", _sectio_argumentum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sancti_visus", SECTIONES);
}
