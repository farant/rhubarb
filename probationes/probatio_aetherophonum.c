/* probatio_aetherophonum.c - theremin in latere (aetherophonum.h)
 *
 * Latus solum (ut probatio_admonitiones_visus): repositorium, montatio,
 * dispensator, eventa ficta; porta soni FICTA vocationes numerat.
 * Puncta relativa ad componentem "campus" (area sonans) - dispositio
 * libera manet.
 *
 * nota: x -> nota (semitonia aequaliter, octavae), gradus rotundat,
 * coercitio; y -> lucrum (summum 1, imum 0). ictus: botton incipit,
 * motus ponit (glissando), solutio dimittit; mus sine bottone nihil;
 * captura extra latus coercetur. claves: f g v [ ] - = campos mutant et
 * vox proxima eos sequitur; Esc portam claudit. pictura: notae C
 * nominatae, linea status, nota currens dum tenetur. porta nulla et
 * vox 0: nihil ultra.
 */
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
#include "aetherophonum.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LATITUDO CCCLX
#define ALTITUDO CCXL

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
    AetherophoniMontatio  m;
             Dispensator* d;
       AetherophoniPorta  porta;
    /* porta ficta */
                     i32 incepta;
                     i32 positae;
                     i32 dimissae;
                     i64 proxima;     /* ansa proxima reddenda (0 =
                                        * nulla) */
                     i64 vox;         /* ultima tacta */
               character tibia[XXXII];
                     b32 vibratus;
                     f32 nota;
                     f32 lucrum;
                     i32 ms;
} Mundus;

interior i64
_incipere (
                 vacuum* ctx,
     constans character* tibia,
                    b32  vibratus,
                    f32  nota,
                    f32  lucrum)
{
    Mundus* w = (Mundus*)ctx;

    w->incepta++;
    sprintf(w->tibia, "%.30s", tibia ? tibia : "");
    w->vibratus  = vibratus;
    w->nota      = nota;
    w->lucrum    = lucrum;
    w->vox       = w->proxima;
    redde w->proxima;
}

interior vacuum
_ponere (
    vacuum* ctx,
       i64  vox,
       f32  nota,
       f32  lucrum,
       i32  ms)
{
    Mundus* w = (Mundus*)ctx;

    w->positae++;
    w->vox     = vox;
    w->nota    = nota;
    w->lucrum  = lucrum;
    w->ms      = ms;
}

interior vacuum
_dimittere (
    vacuum* ctx,
       i64  vox)
{
    Mundus* w = (Mundus*)ctx;

    w->dimissae++;
    w->vox = vox;
}

interior Mundus*
_mundum (
    CredoContextus* c,
               b32  cum_porta)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->proxima          = XLII;
    w->porta.incipere   = _incipere;
    w->porta.ponere     = _ponere;
    w->porta.dimittere  = _dimittere;
    w->porta.ctx        = w;
    w->intern           = internamentum_creare(c->piscina);
    w->repo             = insula_repositorium_creare(c->piscina,
        w->intern, "<hospes/>");
    si (   !w->repo
        || !aetherophonum_montare(&w->m, c->piscina, w->intern, w->repo,
               "a1", cum_porta ? &w->porta : NIHIL, LATITUDO, ALTITUDO,
               NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, aetherophonum_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior Fines
_campus (
     Mundus* w,
    Piscina* p)
{
     Componens* c;
         Fines  f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis("campus", p));
    si (c)
    {
        f = c->fines;
    }
    redde f;
}

/* eventus muris ad punctum campi (x, y relativa) */
interior vacuum
_mus (
             Mundus* w,
    eventus_genus_t  genus,
                s32  x,
                s32  y,
                b32  botton,
            Piscina* p)
{
      Fines f = _campus(w, p);
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.datum.mus.x       = f.x + x;
    e.datum.mus.y       = f.y + y;
    e.datum.mus.botton  = botton ? MUS_SINISTER : ZEPHYRUM;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
_clavis (
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
_effugium (
    Mundus* w)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_EFFUGIUM;
    e.datum.clavis.producta  = XXVII;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior b32
_campus_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a = insula_ramus_attributum(ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

interior Mandata*
_reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md = mandata_creare(p, w->intern);

    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

interior b32
_textus_pictus (
               Mandata* md,
    constans character* textus)
{
    constans Mandatum* x;
                  i32  i;

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

interior b32
_prope (
    f32 a,
    f32 b)
{
    f32 d = a - b;

    redde (d < 0.0f ? -d : d) < 0.001f;
}


    /* ==================================================
     * NOTA et LUCRUM: functiones purae
     * ================================================== */

interior vacuum
_sectio_nota (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM (_prope(aetherophonum_nota(ZEPHYRUM, CCCLX, XLVIII, III,
                        FALSUM), 48.0f));
    CREDO_VERUM (_prope(aetherophonum_nota(CLXXX, CCCLX, XLVIII, III,
                        FALSUM), 66.0f));
    CREDO_VERUM (_prope(aetherophonum_nota(CCCLX, CCCLX, XLVIII, III,
                        FALSUM), 84.0f));
    /* fractiones: X pixela per semitonium */
    CREDO_VERUM (_prope(aetherophonum_nota(CLXXXIV, CCCLX, XLVIII, III,
                        FALSUM), 66.4f));
    /* gradus: semitonium proximum */
    CREDO_VERUM (_prope(aetherophonum_nota(CLXXXIV, CCCLX, XLVIII, III,
                        VERUM), 66.0f));
    CREDO_VERUM (_prope(aetherophonum_nota(CLXXXVI, CCCLX, XLVIII, III,
                        VERUM), 67.0f));
    /* coercitio extra fines */
    CREDO_VERUM (_prope(aetherophonum_nota(-V, CCCLX, XLVIII, III,
                        FALSUM), 48.0f));
    CREDO_VERUM (_prope(aetherophonum_nota(CD, CCCLX, XLVIII, III,
                        FALSUM), 84.0f));
    /* octavae aliae, nota_infima alia */
    CREDO_VERUM (_prope(aetherophonum_nota(CLXXX, CCCLX, LX, I, FALSUM),
                        66.0f));
    /* lucrum: summum 1, imum 0, medium 0.5 */
    CREDO_VERUM (_prope(aetherophonum_lucrum(ZEPHYRUM, CCI), 1.0f));
    CREDO_VERUM (_prope(aetherophonum_lucrum(CC, CCI), 0.0f));
    CREDO_VERUM (_prope(aetherophonum_lucrum(C, CCI), 0.5f));
    CREDO_VERUM (_prope(aetherophonum_lucrum(-X, CCI), 1.0f));
    CREDO_VERUM (_prope(aetherophonum_lucrum(D, CCI), 0.0f));
}


    /* ==================================================
     * ICTUS: botton tenet, motus ponit, solutio dimittit
     * ================================================== */

interior vacuum
_sectio_ictus (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, VERUM);
      Fines  f;

    CREDO_NECESSE (w != NIHIL);
    f = _campus(w, c->piscina);
    CREDO_NECESSE (f.latitudo > C && f.altitudo > L);

    CREDO_GRADUS ("mus sine bottone: nihil");
    _mus(w, EVENTUS_MUS_MOTUS, f.latitudo / II, X, FALSUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->incepta, ZEPHYRUM);

    CREDO_GRADUS ("ictus: vox ad notam et lucrum puncti");
    _mus(w, EVENTUS_MUS_DEPRESSUS, f.latitudo / IV, f.altitudo / IV,
         VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->incepta, I);
    CREDO_VERUM (strcmp(w->tibia, "aether") == ZEPHYRUM);
    CREDO_VERUM (w->vibratus);
    CREDO_VERUM (_prope(w->nota, aetherophonum_nota(f.latitudo / IV,
                 f.latitudo, XLVIII, III, FALSUM)));
    CREDO_VERUM (_prope(w->lucrum, aetherophonum_lucrum(f.altitudo / IV,
                 f.altitudo)));
    CREDO_VERUM (w->m.tenetur);
    CREDO_VERUM (w->m.vox == XLII);

    CREDO_GRADUS ("motus: ponit cum glissando");
    _mus(w, EVENTUS_MUS_MOTUS, f.latitudo / II, f.altitudo / II, VERUM,
         c->piscina);
    CREDO_AEQUALIS_I32 (w->positae, I);
    CREDO_VERUM (w->vox == XLII);
    CREDO_VERUM (_prope(w->nota, aetherophonum_nota(f.latitudo / II,
                 f.latitudo, XLVIII, III, FALSUM)));
    CREDO_VERUM (_prope(w->lucrum, aetherophonum_lucrum(f.altitudo / II,
                 f.altitudo)));
    CREDO_AEQUALIS_I32 (w->ms, AETHEROPHONUM_TRANSITUS);

    CREDO_GRADUS ("captura: extra latus coercetur");
    _mus(w, EVENTUS_MUS_MOTUS, f.latitudo + CC, -C, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->positae, II);
    CREDO_VERUM (_prope(w->nota, 84.0f));
    CREDO_VERUM (_prope(w->lucrum, 1.0f));

    CREDO_GRADUS ("solutio: porta clausa, deinde motus nihil");
    _mus(w, EVENTUS_MUS_LIBERATUS, f.latitudo / II, X, VERUM,
         c->piscina);
    CREDO_AEQUALIS_I32 (w->dimissae, I);
    CREDO_VERUM (w->vox == XLII);
    CREDO_FALSUM (w->m.tenetur);
    CREDO_VERUM (w->m.vox == ZEPHYRUM);
    _mus(w, EVENTUS_MUS_MOTUS, X, X, FALSUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->positae, II);
    CREDO_AEQUALIS_I32 (w->dimissae, I);
    /* mus sine bottone notam currentem non mutat */
    CREDO_VERUM (_prope(w->m.nota, 84.0f));

    CREDO_GRADUS ("Esc dum tenetur: porta clausa");
    w->proxima = XLIII;
    _mus(w, EVENTUS_MUS_DEPRESSUS, X, X, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->incepta, II);
    _effugium(w);
    CREDO_AEQUALIS_I32 (w->dimissae, II);
    CREDO_VERUM (w->vox == XLIII);
    CREDO_FALSUM (w->m.tenetur);
    _mus(w, EVENTUS_MUS_MOTUS, XX, XX, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->positae, II);
    _mus(w, EVENTUS_MUS_LIBERATUS, XX, XX, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->dimissae, II);
}


    /* ==================================================
     * CLAVES: praeferentiae et vox proxima
     * ================================================== */

interior vacuum
_sectio_claves (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, VERUM);
      Fines  f;
        f32  n;

    CREDO_NECESSE (w != NIHIL);
    f = _campus(w, c->piscina);

    CREDO_GRADUS ("f: tibia per circulum");
    _clavis(w, 'f');
    CREDO_VERUM (_campus_est(&w->m.ramus, "tibia", "quadrans"));
    _clavis(w, 'f');
    CREDO_VERUM (_campus_est(&w->m.ramus, "tibia", "triangulum"));
    _clavis(w, 'f');
    CREDO_VERUM (_campus_est(&w->m.ramus, "tibia", "organum"));
    _clavis(w, 'f');
    CREDO_VERUM (_campus_est(&w->m.ramus, "tibia", "schola"));
    _clavis(w, 'f');
    CREDO_VERUM (_campus_est(&w->m.ramus, "tibia", "aether"));
    _clavis(w, 'f');

    CREDO_GRADUS ("v g: vibratus non, gradus; vox sequitur");
    _clavis(w, 'v');
    CREDO_VERUM (_campus_est(&w->m.ramus, "vibratus", "falsum"));
    _clavis(w, 'g');
    CREDO_VERUM (_campus_est(&w->m.ramus, "gradus", "verum"));
    _mus(w, EVENTUS_MUS_DEPRESSUS, f.latitudo / III + III, X, VERUM,
         c->piscina);
    CREDO_VERUM (strcmp(w->tibia, "quadrans") == ZEPHYRUM);
    CREDO_FALSUM (w->vibratus);
    n = w->nota;
    CREDO_VERUM (_prope(n, (f32)floor((f64)n + 0.5)));
    _mus(w, EVENTUS_MUS_MOTUS, f.latitudo / II + III, X, VERUM,
         c->piscina);
    CREDO_AEQUALIS_I32 (w->ms, AETHEROPHONUM_TRANSITUS_GRADUUM);
    CREDO_VERUM (_prope(w->nota, (f32)floor((f64)w->nota + 0.5)));
    _mus(w, EVENTUS_MUS_LIBERATUS, X, X, VERUM, c->piscina);

    CREDO_GRADUS ("[ ] - =: ambitus");
    _clavis(w, ']');
    CREDO_VERUM (_campus_est(&w->m.ramus, "nota_infima", "60"));
    _clavis(w, '[');
    _clavis(w, '[');
    CREDO_VERUM (_campus_est(&w->m.ramus, "nota_infima", "36"));
    _clavis(w, '=');
    CREDO_VERUM (_campus_est(&w->m.ramus, "octavae", "4"));
    _clavis(w, '-');
    _clavis(w, '-');
    _clavis(w, '-');
    _clavis(w, '-');
    _clavis(w, '-');
    CREDO_VERUM (_campus_est(&w->m.ramus, "octavae", "1"));
    /* margo dexter (pixelum ultimum campi), gradus adhuc: XLVIII */
    _mus(w, EVENTUS_MUS_DEPRESSUS, f.latitudo - I, X, VERUM,
         c->piscina);
    CREDO_VERUM (_prope(w->nota, 48.0f));
    _mus(w, EVENTUS_MUS_LIBERATUS, X, X, VERUM, c->piscina);
    /* limites: nota_infima non infra 0, octavae non supra VI */
    per (n = 0.0f; n < 10.0f; n += 1.0f)
    {
        _clavis(w, '[');
        _clavis(w, '=');
    }
    CREDO_VERUM (_campus_est(&w->m.ramus, "nota_infima", "0"));
    CREDO_VERUM (_campus_est(&w->m.ramus, "octavae", "6"));
    /* summa <= CXXVII: VI octavae (LXXII) -> infima non supra XLVIII */
    per (n = 0.0f; n < 10.0f; n += 1.0f)
    {
        _clavis(w, ']');
    }
    CREDO_VERUM (_campus_est(&w->m.ramus, "nota_infima", "48"));
}


    /* ==================================================
     * PICTURA: notae C, linea status, nota currens
     * ================================================== */

interior vacuum
_sectio_pictura (
    CredoContextus* c)
{
      Mundus* w = _mundum(c, VERUM);
     Mandata* md;
       Fines  f;

    CREDO_NECESSE (w != NIHIL);
    f   = _campus(w, c->piscina);
    md  = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_pictus(md, "C3"));
    CREDO_VERUM (_textus_pictus(md, "C4"));
    CREDO_VERUM (_textus_pictus(md, "C5"));
    CREDO_FALSUM (_textus_pictus(md, "C7"));
    CREDO_VERUM (_textus_pictus(md, "aether  C3-C6  vibratus"));
    /* tenetur ad A4 (LXIX): nomen notae currentis */
    _clavis(w, 'g');
    _mus(w, EVENTUS_MUS_DEPRESSUS, (f.latitudo * XXI) / XXXVI + I, X,
         VERUM, c->piscina);
    CREDO_VERUM (_prope(w->nota, 69.0f));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_pictus(md, "A4"));
    CREDO_VERUM (_textus_pictus(md, "aether  C3-C6  vibratus  gradus"));
    _mus(w, EVENTUS_MUS_LIBERATUS, X, X, VERUM, c->piscina);
    md = _reddere(w, c->piscina);
    CREDO_FALSUM (_textus_pictus(md, "A4"));
}


    /* ==================================================
     * PORTA NULLA et VOX 0
     * ================================================== */

interior vacuum
_sectio_porta_nulla (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, FALSUM);
      Fines  f;

    CREDO_NECESSE (w != NIHIL);
    f = _campus(w, c->piscina);
    _mus(w, EVENTUS_MUS_DEPRESSUS, X, X, VERUM, c->piscina);
    _mus(w, EVENTUS_MUS_MOTUS, XX, XX, VERUM, c->piscina);
    _mus(w, EVENTUS_MUS_LIBERATUS, XX, XX, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->incepta, ZEPHYRUM);

    /* vox 0 (mutus, exitus nullus): nec ponere nec dimittere */
    w = _mundum(c, VERUM);
    CREDO_NECESSE (w != NIHIL);
    w->proxima  = ZEPHYRUM;
    f           = _campus(w, c->piscina);
    _mus(w, EVENTUS_MUS_DEPRESSUS, X, X, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->incepta, I);
    _mus(w, EVENTUS_MUS_MOTUS, f.latitudo / II, XX, VERUM, c->piscina);
    _mus(w, EVENTUS_MUS_LIBERATUS, XX, XX, VERUM, c->piscina);
    CREDO_AEQUALIS_I32 (w->positae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (w->dimissae, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nota", _sectio_nota, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ictus", _sectio_ictus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pictura", _sectio_pictura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "porta_nulla", _sectio_porta_nulla, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("aetherophonum", SECTIONES);
}
