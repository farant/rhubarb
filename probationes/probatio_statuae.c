/* probatio_statuae.c - latus statuarum (statuae.h)
 *
 * Latus solum (ut probatio_aetherophonum): repositorium, montatio,
 * dispensator, eventa ficta, horologium fictum; volumen temporarium
 * cum fixis lapide et fungo per statuae_addere.
 *
 * primum: quadrum post pulsum. tempus: horologium quadrum mutat.
 * quies: Spatium sistit et pergit. tractus: gyrus et inclinatio,
 * coercitio, status solum in solutione. claves: [ ] f 2 r h l j k.
 * delectus: d et c, electio statum scribit. modi: omnes quattuor.
 * vacuum: volumen sine statuis, deinde statua addita apparet.
 * recusatum: massa corrupta titulum et causam dat, sagittae valent.
 * magnitudines: latera minima sine ruina. dispositio: ordo unus aut
 * duo. memoria: quadra multa piscinam non augent.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "filum.h"
#include "volumen.h"
#include "statuae_repositorium.h"
#include "statuae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO CCCLX
#define ALTITUDO CDL

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"

nomen structura {
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                Volumen* volumen;
        StatuaeMontatio  m;
            Dispensator* d;
                    s64  hora;      /* horologium fictum, ms */
} Mundus;

interior s64
_nunc (
    vacuum* ctx)
{
    redde ((Mundus*)ctx)->hora;
}

/* volumen temporarium; statuae: I lapis, II lapis et fungus */
interior Volumen*
_volumen (
        CredoContextus* c,
    constans character* praefixum,
                   i32  statuae)
{
    Volumen* v = volumen_temporarium(c->piscina, praefixum);

    si (v && statuae >= I)
    {
        (vacuum)statuae_addere(v, chorda_ex_literis("lapis",
            c->piscina), filum_legere_totum(VIA_LAPIDIS, c->piscina));
    }
    si (v && statuae >= II)
    {
        (vacuum)statuae_addere(v, chorda_ex_literis("fungus",
            c->piscina), filum_legere_totum(VIA_FUNGI, c->piscina));
    }
    redde v;
}

interior Mundus*
_mundum (
    CredoContextus* c,
           Volumen* volumen,
               i32  latitudo,
               i32  altitudo)
{
          Mundus* w;
           Motus* motus;
    StatuaePorta  porta;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->hora     = 100000;
    w->volumen  = volumen;
    porta.nunc  = _nunc;
    porta.ctx   = w;
    w->intern   = internamentum_creare(c->piscina);
    w->repo     = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !statuae_montare(&w->m, c->piscina, w->intern, volumen,
               w->repo, "s1", &porta, latitudo, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, statuae_componere, &w->m, CCC);
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
_finire (
    Mundus* w)
{
    si (!w)
    {
        redde;
    }
    statuae_dimittere(&w->m);
    si (w->volumen)
    {
        volumen_claudere(w->volumen);
    }
}

interior Componens*
_nodus (
                Mundus* w,
    constans character* id,
               Piscina* p)
{
    redde componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
}

/* eventus muris ad punctum (x, y) relativum componentis 'id' */
interior vacuum
_mus (
                Mundus* w,
    constans character* id,
       eventus_genus_t  genus,
                   s32  x,
                   s32  y,
               Piscina* p)
{
    Componens* n = _nodus(w, id, p);
      Eventus  e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = genus;
    e.datum.mus.x  = (n ? n->fines.x : ZEPHYRUM) + x;
    e.datum.mus.y  = (n ? n->fines.y : ZEPHYRUM) + y;
    /* optiones menu fines intra menu habent: menu additur */
    si (strncmp(id, "elenchus.", IX) == ZEPHYRUM)
    {
        n              = _nodus(w, "elenchus", p);
        e.datum.mus.x  += n ? n->fines.x : ZEPHYRUM;
        e.datum.mus.y  += n ? n->fines.y : ZEPHYRUM;
    }
    e.datum.mus.botton  = MUS_SINISTER;
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
_reditus (
    Mundus* w)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_REDITUS;
    e.datum.clavis.producta  = XIII;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* horologium promovere et pulsare */
interior b32
_pulsus (
    Mundus* w,
       s64  ms)
{
    b32 mutatum;

    w->hora += ms;
    mutatum = statuae_pulsare(&w->m);
    dispensator_recomponere(w->d);
    redde mutatum;
}

interior constans Imago*
_quadrum (
    Mundus* w)
{
    redde statuae_imago_fons(w->m.quadri_titulus, &w->m);
}

/* campus status: valor aequalis? valor NIHIL = campus abest */
interior b32
_campus (
                Mundus* w,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);

    redde valor ? (a && chorda_aequalis_literis(*a, valor)) : !a;
}

interior s32
_numerus (
                Mundus* w,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);
       s32  v;

    redde a && chorda_ut_s32(*a, &v) ? v : praestitutum;
}

/* copia pixelorum quadri (quadrum proximum piscinam vacat) */
interior i8*
_copia (
    CredoContextus* c,
    constans Imago* im)
{
    memoriae_index  n;
                i8* p;

    n = (memoriae_index)im->latitudo * im->altitudo * IV;
    p = (i8*)piscina_allocare(c->piscina, n ? n : I);
    memcpy(p, im->pixela, n);
    redde p;
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

/* estne textus pictus qui fragmentum continet? */
interior b32
_textus_pictus (
               Mandata* md,
    constans character* fragmentum,
               Piscina* p)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus,
                   chorda_ex_literis(fragmentum, p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_imago_picta (
    Mandata* md)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* rectangulum impletum cum finibus componentis datae */
interior b32
_fundus_pictus (
               Mandata* md,
    constans Componens* area)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus          == MANDATUM_RECTANGULUM && x->impletum
            && x->exemplar       == ZEPHYRUM
            && x->color.genus    == COLOR_MANDATI_THEMA
            && x->color.valor    == (i32)COLOR_BACKGROUND
            && x->fines.x        == area->fines.x
            && x->fines.y        == area->fines.y
            && x->fines.latitudo == area->fines.latitudo
            && x->fines.altitudo == area->fines.altitudo)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_primum (
    CredoContextus* c)
{
            Mundus* w = _mundum(c, _volumen(c, "statuae_primum", II),
                LATITUDO, ALTITUDO);
     constans Imago* im;
          Componens* pictura;
                i32  k;
                i32  perlucida  = ZEPHYRUM;
                i32  opaca      = ZEPHYRUM;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    /* quadrum aream picturae implet: latus totum sub capite */
    CREDO_AEQUALIS_I32 (im->latitudo, (i32)pictura->fines.latitudo);
    CREDO_AEQUALIS_I32 (im->altitudo, (i32)pictura->fines.altitudo);
    CREDO_AEQUALIS_I32 (im->latitudo, LATITUDO);
    CREDO_VERUM (im->altitudo >= ALTITUDO - XXIV);
    per (k = ZEPHYRUM; k < im->latitudo * im->altitudo; k++)
    {
        /* charta perlucida (fundus thematis sub ea apparet), statua
         * opaca; nihil inter */
        si (im->pixela[k * IV + III] == ZEPHYRUM)
        {
            perlucida++;
        }
        alioquin si (im->pixela[k * IV + III] == 255)
        {
            opaca++;
        }
    }
    CREDO_AEQUALIS_I32 (perlucida + opaca, im->latitudo * im->altitudo);
    CREDO_VERUM (perlucida > im->latitudo * im->altitudo / IV);
    CREDO_VERUM (opaca > im->latitudo * im->altitudo / L);
    /* titulus alius: nulla imago */
    CREDO_VERUM (statuae_imago_fons(chorda_ex_literis("alia",
        c->piscina), &w->m) == NIHIL);
    /* caput pictum: numerus, titulus, cyclus, modus, commutatores */
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "1/2",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "lapis",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "walk",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "Atkinson",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "lit",
        c->piscina));
    CREDO_VERUM (_imago_picta(_reddere(w, c->piscina)));
    /* fundus planus sub pictura, colore fundi thematis (rectangulum
     * impletum areae eius) */
    CREDO_VERUM (_fundus_pictus(_reddere(w, c->piscina), pictura));
    CREDO_VERUM (chorda_aequalis_literis(statuae_titulus(&w->m,
        c->piscina), "lapis"));
    /* cyclus currit: pulsus crebri */
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_MOTUS);
    _finire(w);
}

interior vacuum
_sectio_tempus (
    CredoContextus* c)
{
            Mundus* w = _mundum(c, _volumen(c, "statuae_tempus", I),
                LATITUDO, ALTITUDO);
    constans Imago* im;
                i8* prior;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    prior = _copia(c, im);
    /* horologium immotum: nihil mutatum */
    CREDO_FALSUM (_pulsus(w, ZEPHYRUM));
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)im->latitudo * im->altitudo * IV) == ZEPHYRUM);
    /* C ms: quadrum novum */
    CREDO_VERUM (_pulsus(w, C));
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)im->latitudo * im->altitudo * IV) != ZEPHYRUM);
    /* horologium SEMEL per pulsum legitur: compositio sine pulsu
     * (eventus, repictio) tempus novum non videt - quadrum idem, non
     * iterum factum */
    {
        character clavis[CXXVIII];

        strcpy(clavis, w->m.quadri_clavis);
        w->hora += L;
        dispensator_recomponere(w->d);
        dispensator_recomponere(w->d);
        CREDO_VERUM (strcmp(clavis, w->m.quadri_clavis) == ZEPHYRUM);
        w->hora -= L;
    }
    /* cyclus plenus (walk 1.4 s) post: idem tempus cycli, ergo
     * eadem clavis - nihil reficitur, idem quadrum */
    prior = _copia(c, _quadrum(w));
    CREDO_FALSUM (_pulsus(w, 1400));
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)im->latitudo * im->altitudo * IV) == ZEPHYRUM);
    _finire(w);
}

interior vacuum
_sectio_quies (
    CredoContextus* c)
{
            Mundus* w = _mundum(c, _volumen(c, "statuae_quies", I),
                LATITUDO, ALTITUDO);
                i8* sistens;
    memoriae_index  octeti;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, CCC);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    octeti = (memoriae_index)_quadrum(w)->latitudo
        * _quadrum(w)->altitudo * IV;
    _clavis(w, ' ');
    CREDO_VERUM (_campus(w, "quiescit", "verum"));
    sistens = _copia(c, _quadrum(w));
    /* tempus transit, quadrum stat; pulsus rari (inspectio sola) */
    CREDO_FALSUM (_pulsus(w, D));
    CREDO_FALSUM (_pulsus(w, 777));
    CREDO_VERUM (memcmp(sistens, _quadrum(w)->pixela, octeti)
        == ZEPHYRUM);
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_INSPECTIONIS);
    /* cursus: inde pergit (statim idem, deinde aliud) */
    _clavis(w, ' ');
    CREDO_VERUM (_campus(w, "quiescit", NIHIL));
    CREDO_VERUM (memcmp(sistens, _quadrum(w)->pixela, octeti)
        == ZEPHYRUM);
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_MOTUS);
    CREDO_VERUM (_pulsus(w, C));
    CREDO_VERUM (memcmp(sistens, _quadrum(w)->pixela, octeti)
        != ZEPHYRUM);
    _finire(w);
}

interior vacuum
_sectio_tractus (
    CredoContextus* c)
{
    Mundus* w = _mundum(c, _volumen(c, "statuae_tractus", I),
        LATITUDO, ALTITUDO);
        i8* prior;

    CREDO_NECESSE (w != NIHIL);
    _clavis(w, ' ');
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    prior = _copia(c, _quadrum(w));
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", CCC), CCC);
    /* XL pixela dextrorsum: CCC + XL * V = D */
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, C, C, c->piscina);
    CREDO_VERUM (w->m.trahitur);
    /* dum trahitur: pulsus crebri etiam quiescens */
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_MOTUS);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, CXX, C, c->piscina);
    /* status nondum scriptus; quadrum iam gyrat */
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", CCC), CCC);
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)_quadrum(w)->latitudo * _quadrum(w)->altitudo
            * IV) != ZEPHYRUM);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, CXL, C, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, CXL, C, c->piscina);
    CREDO_FALSUM (w->m.trahitur);
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), D);
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", CL), CL);
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_INSPECTIONIS);
    /* M pixela deorsum: inclinatio ad maximum coercetur */
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, C, C, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, C, 1100, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, C, 1100, c->piscina);
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", ZEPHYRUM), 850);
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), D);
    /* sursum ultra: ad -850 */
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, C, CCC, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, C, -1100, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, C, -1100, c->piscina);
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", ZEPHYRUM), -850);
    /* gyrus circulum claudit: 500 + 800 * V = 4500 -> 900 */
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, C, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, 800, C, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, 800, C, c->piscina);
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), 900);
    /* motus sine pressione: nihil */
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, L, L, c->piscina);
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), 900);
    _finire(w);
}

interior vacuum
_sectio_claves (
    CredoContextus* c)
{
            Mundus* w = _mundum(c, _volumen(c, "statuae_claves", II),
                LATITUDO, ALTITUDO);
    constans Imago* im;
                i8* prior;
               i32  latitudo;
               i32  altitudo;

    CREDO_NECESSE (w != NIHIL);
    _clavis(w, ' ');
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(statuae_titulus(&w->m,
        c->piscina), "lapis"));
    /* visus mutatus, deinde ']' statuam proximam et visum ordinarium */
    _clavis(w, 'l');
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), 450);
    _clavis(w, 'k');
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", ZEPHYRUM), 300);
    _clavis(w, 'h');
    _clavis(w, 'h');
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), 150);
    _clavis(w, 'j');
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", ZEPHYRUM), 150);
    _clavis(w, ']');
    CREDO_VERUM (_campus(w, "statua", "fungus"));
    CREDO_VERUM (_campus(w, "gyrus", NIHIL));
    CREDO_VERUM (_campus(w, "inclinatio", NIHIL));
    CREDO_VERUM (chorda_aequalis_literis(statuae_titulus(&w->m,
        c->piscina), "fungus"));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "2/2",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "walk_bouncy",
        c->piscina));
    /* circulus: ']' ad primam, '[' ad ultimam */
    _clavis(w, ']');
    CREDO_VERUM (_campus(w, "statua", "lapis"));
    _clavis(w, '[');
    CREDO_VERUM (_campus(w, "statua", "fungus"));
    /* sagittae capitis idem faciunt */
    _mus(w, "sagitta_proxima", EVENTUS_MUS_DEPRESSUS, II, II,
        c->piscina);
    CREDO_VERUM (_campus(w, "statua", "lapis"));
    _mus(w, "sagitta_prior", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "statua", "fungus"));
    /* 'f': planum, quadrum mutat */
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    latitudo  = im->latitudo;
    altitudo  = im->altitudo;
    prior     = _copia(c, im);
    _clavis(w, 'f');
    CREDO_VERUM (_campus(w, "planum", "verum"));
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)latitudo * altitudo * IV) != ZEPHYRUM);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "flat",
        c->piscina));
    _mus(w, "lux", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "planum", NIHIL));
    /* '2': dimidia mensura picta, pixela duplicata -> mensura eadem
     * (par) et quadrata II x II uniformia */
    _clavis(w, '2');
    CREDO_VERUM (_campus(w, "duplex", "verum"));
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    CREDO_AEQUALIS_I32 (im->latitudo, latitudo / II * II);
    CREDO_AEQUALIS_I32 (im->altitudo, altitudo / II * II);
    {
        i32 x;
        i32 y;
        i32 diversa = ZEPHYRUM;

        per (y = ZEPHYRUM; y + I < im->altitudo; y += II)
        {
            per (x = ZEPHYRUM; x + I < im->latitudo; x += II)
            {
                constans i8* a = im->pixela + (y * im->latitudo + x)
                    * IV;

                si (   memcmp(a, a + IV, IV)                != ZEPHYRUM
                    || memcmp(a, a + im->latitudo * IV, IV) != ZEPHYRUM
                    || memcmp(a, a + im->latitudo * IV + IV, IV)
                       != ZEPHYRUM)
                {
                    diversa++;
                }
            }
        }
        CREDO_AEQUALIS_I32 (diversa, ZEPHYRUM);
    }
    _mus(w, "duplex", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "duplex", NIHIL));
    CREDO_AEQUALIS_I32 (_quadrum(w)->latitudo, latitudo);
    /* 'r': visus ordinarius */
    _clavis(w, 'l');
    _clavis(w, 'k');
    _clavis(w, 'r');
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), CCC);
    CREDO_AEQUALIS_S32 (_numerus(w, "inclinatio", ZEPHYRUM), CL);
    /* clavis aliena: nihil */
    _clavis(w, 'q');
    CREDO_AEQUALIS_S32 (_numerus(w, "gyrus", ZEPHYRUM), CCC);
    _finire(w);
}

interior vacuum
_sectio_delectus (
    CredoContextus* c)
{
    Mundus* w = _mundum(c, _volumen(c, "statuae_delectus", II),
        LATITUDO, ALTITUDO);
        i8* prior;

    CREDO_NECESSE (w != NIHIL);
    _clavis(w, ' ');
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    prior = _copia(c, _quadrum(w));
    /* 'd': delectus modi apertus cum valore currente illuminato */
    _clavis(w, 'd');
    CREDO_VERUM (_campus(w, "delectus", "modus"));
    CREDO_VERUM (_campus(w, "delectus_electa", "0"));
    CREDO_VERUM (_nodus(w, "elenchus", c->piscina) != NIHIL);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "Exemplaria",
        c->piscina));
    /* dum apertus: claves lateris non agunt ('f' deglutitur) */
    _clavis(w, 'f');
    CREDO_VERUM (_campus(w, "planum", NIHIL));
    /* j, Reditus: bayer */
    _clavis(w, 'j');
    CREDO_VERUM (_campus(w, "delectus_electa", "1"));
    _reditus(w);
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    CREDO_VERUM (_campus(w, "modus", "bayer"));
    CREDO_VERUM (_nodus(w, "elenchus", c->piscina) == NIHIL);
    CREDO_VERUM (memcmp(prior, _quadrum(w)->pixela,
        (memoriae_index)_quadrum(w)->latitudo * _quadrum(w)->altitudo
            * IV) != ZEPHYRUM);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "Bayer",
        c->piscina));
    /* ictus in bottone cycli aperit; ictus in optione II eligit */
    _mus(w, "cyclus", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "delectus", "cyclus"));
    CREDO_VERUM (_nodus(w, "elenchus.1", c->piscina) != NIHIL);
    CREDO_VERUM (_nodus(w, "elenchus.2", c->piscina) == NIHIL);
    _mus(w, "elenchus.1", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    CREDO_VERUM (_campus(w, "cyclus", "boulder_throw"));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina),
        "boulder_throw", c->piscina));
    /* ictus in bottone aperto claudit (non reaperit) */
    _mus(w, "modus", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "delectus", "modus"));
    _mus(w, "modus", EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    /* 'c' deinde 'd': unus tantum apertus */
    _clavis(w, 'c');
    CREDO_VERUM (_campus(w, "delectus", "cyclus"));
    /* cyclus currens (boulder_throw, index I) illuminatus */
    CREDO_VERUM (_campus(w, "delectus_electa", "1"));
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, C, C, c->piscina);
    /* ictus extra claudit et tractum NON incipit */
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    CREDO_FALSUM (w->m.trahitur);
    /* fungus cyclum lapidis non habet: primus suus */
    _clavis(w, ']');
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "walk_bouncy",
        c->piscina));
    _finire(w);
}

interior vacuum
_sectio_modi (
    CredoContextus* c)
{
    hic_manens constans character* constans MODI[IV] = {
        "atkinson", "bayer", "exemplaria", "nullum"
    };
            Mundus* w = _mundum(c, _volumen(c, "statuae_modi", I),
                CXX, CL);
    constans Imago* im;
                i8* priores[IV];
               i32  k;
               i32  j;
               i32  n;

    CREDO_NECESSE (w != NIHIL);
    _clavis(w, ' ');
    per (k = ZEPHYRUM; k < IV; k++)
    {
        /* delectus apertus; optio k per murem electa */
        _clavis(w, 'd');
        {
            character id[XXXII];

            sprintf(id, "elenchus.%u", (insignatus)k);
            _mus(w, id, EVENTUS_MUS_DEPRESSUS, II, II, c->piscina);
        }
        CREDO_VERUM (_campus(w, "modus", MODI[k]));
        im = _quadrum(w);
        CREDO_NECESSE (im != NIHIL);
        n           = im->latitudo * im->altitudo;
        priores[k]  = _copia(c, im);
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (   im->pixela[j * IV + III] != ZEPHYRUM
                && im->pixela[j * IV + III] != 255)
            {
                CREDO_AEQUALIS_I32 (j, 0xFFFFFF);
                frange;
            }
        }
        per (j = ZEPHYRUM; j < k; j++)
        {
            CREDO_VERUM (memcmp(priores[j], priores[k],
                (memoriae_index)n * IV) != ZEPHYRUM);
        }
    }
    _finire(w);
}

interior vacuum
_sectio_vacuum (
    CredoContextus* c)
{
    Volumen* v = _volumen(c, "statuae_vacuum", ZEPHYRUM);
     Mundus* w = _mundum(c, v, LATITUDO, ALTITUDO);

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina),
        "No models yet", c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina),
        "statuae addere", c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "0/0",
        c->piscina));
    CREDO_FALSUM (_imago_picta(_reddere(w, c->piscina)));
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_INSPECTIONIS);
    CREDO_AEQUALIS_I32 (statuae_titulus(&w->m, c->piscina).mensura,
        ZEPHYRUM);
    /* claves in latere vacuo: nihil ruit, nihil mutatur */
    _clavis(w, ']');
    _clavis(w, 'c');
    _clavis(w, ' ');
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, X, X, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, X, X, c->piscina);
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    /* statua addita (ut ab alio processu): ante secundam nondum
     * visa, post secundam apparet */
    CREDO_VERUM (statuae_addere(v, chorda_ex_literis("lapis",
        c->piscina), filum_legere_totum(VIA_LAPIDIS, c->piscina)));
    CREDO_FALSUM (_pulsus(w, CC));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_pulsus(w, 900));
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "1/1",
        c->piscina));
    CREDO_VERUM (_imago_picta(_reddere(w, c->piscina)));
    /* statua remota: latus iterum vacuum */
    CREDO_VERUM (statuae_removere(v, chorda_ex_literis("lapis",
        c->piscina)));
    CREDO_VERUM (_pulsus(w, 1100));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina),
        "No models yet", c->piscina));
    _finire(w);
}

interior vacuum
_sectio_recusatum (
    CredoContextus* c)
{
    Volumen* v = _volumen(c, "statuae_recusatum", ZEPHYRUM);
     Mundus* w;

    CREDO_NECESSE (v != NIHIL);
    /* massa quae GLB non est, deinde statua sana */
    CREDO_VERUM (statuae_addere(v, chorda_ex_literis("fractum",
        c->piscina), chorda_ex_literis("non glb!!", c->piscina)));
    CREDO_VERUM (statuae_addere(v, chorda_ex_literis("lapis",
        c->piscina), filum_legere_totum(VIA_LAPIDIS, c->piscina)));
    w = _mundum(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "fractum",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina),
        "cannot be shown", c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "not a GLB",
        c->piscina));
    CREDO_VERUM (_textus_pictus(_reddere(w, c->piscina), "no clips",
        c->piscina));
    /* delectus cycli non aperitur; sine cyclo pulsus rari */
    _clavis(w, 'c');
    CREDO_VERUM (_campus(w, "delectus", NIHIL));
    CREDO_VERUM (statuae_mora(&w->m) == STATUAE_MORA_INSPECTIONIS);
    /* sagitta ad statuam sanam ducit, et retro */
    _clavis(w, ']');
    CREDO_VERUM (_campus(w, "statua", "lapis"));
    CREDO_VERUM (_quadrum(w) != NIHIL);
    _clavis(w, '[');
    CREDO_VERUM (_campus(w, "statua", "fractum"));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    _finire(w);
}

interior vacuum
_latus_parvum (
    CredoContextus* c,
               i32  latitudo,
               i32  altitudo,
               b32  geminatum)
{
    Mundus* w = _mundum(c, _volumen(c, "statuae_parvum", II), latitudo,
        altitudo);

    si (!w)
    {
        redde;
    }
    si (geminatum)
    {
        _clavis(w, '2');
    }
    (vacuum)_pulsus(w, L);
    (vacuum)_reddere(w, c->piscina);
    _clavis(w, 'd');
    (vacuum)_reddere(w, c->piscina);
    _clavis(w, 'j');
    _reditus(w);
    _mus(w, "pictura", EVENTUS_MUS_DEPRESSUS, I, I, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_MOTUS, IX, IX, c->piscina);
    _mus(w, "pictura", EVENTUS_MUS_LIBERATUS, IX, IX, c->piscina);
    _clavis(w, ']');
    (vacuum)_pulsus(w, L);
    (vacuum)_reddere(w, c->piscina);
    (vacuum)statuae_mora(&w->m);
    _finire(w);
}

interior vacuum
_sectio_magnitudines (
    CredoContextus* c)
{
    Mundus* w;

    /* montatio 0 x 0 mensuram ordinariam sumit; minimae verae per
     * attributa superficiei fieri non possunt hic, ergo montatio
     * parva directe */
    CREDO_NON_RUIT (_latus_parvum(c, I, I, FALSUM));
    CREDO_NON_RUIT (_latus_parvum(c, I, I, VERUM));
    CREDO_NON_RUIT (_latus_parvum(c, CCCLXI, CDLI, VERUM));
    CREDO_NON_RUIT (_latus_parvum(c, CC, X, FALSUM));
    CREDO_NON_RUIT (_latus_parvum(c, X, CC, VERUM));
    CREDO_NON_RUIT (_latus_parvum(c, III, XXV, VERUM));
    /* latus caput solum capiens: nullum quadrum, nihil ruit */
    w = _mundum(c, _volumen(c, "statuae_humile", I), LATITUDO, XII);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    _finire(w);
    /* I x I pictura sub capite */
    w = _mundum(c, _volumen(c, "statuae_unum", I), LATITUDO, XIII);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    CREDO_AEQUALIS_I32 (_quadrum(w)->altitudo, I);
    _finire(w);
}

interior vacuum
_sectio_dispositio (
    CredoContextus* c)
{
       Mundus* w;
    Componens* pictura;
    Componens* cyclus;

    /* latus latum: ordo unus (pictura ad XII) */
    w = _mundum(c, _volumen(c, "statuae_latum", II), DC, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    pictura  = _nodus(w, "pictura", c->piscina);
    cyclus   = _nodus(w, "cyclus", c->piscina);
    CREDO_NECESSE (pictura != NIHIL && cyclus != NIHIL);
    CREDO_AEQUALIS_S32 (pictura->fines.y, XII);
    CREDO_AEQUALIS_S32 (cyclus->fines.y, ZEPHYRUM);
    CREDO_VERUM (_nodus(w, "duplex", c->piscina)->fines.x
        + _nodus(w, "duplex", c->piscina)->fines.latitudo <= DC);
    _finire(w);
    /* latus angustum: ordines duo (pictura ad XXIV) */
    w = _mundum(c, _volumen(c, "statuae_angustum", II), CC, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    pictura  = _nodus(w, "pictura", c->piscina);
    cyclus   = _nodus(w, "cyclus", c->piscina);
    CREDO_NECESSE (pictura != NIHIL && cyclus != NIHIL);
    CREDO_AEQUALIS_S32 (pictura->fines.y, XXIV);
    CREDO_AEQUALIS_S32 (cyclus->fines.y, XII);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    CREDO_AEQUALIS_I32 (_quadrum(w)->altitudo, ALTITUDO - XXIV);
    _finire(w);
    /* latus ordinarium Frani (CCCLX): uter ordo? relatio sola */
    w = _mundum(c, _volumen(c, "statuae_ordinarium", II), LATITUDO,
        ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    printf("  dispositio CCCLX: pictura y = %d (XII = ordo unus,"
        " XXIV = duo)\n", (integer)pictura->fines.y);
    CREDO_VERUM (pictura->fines.y == XII || pictura->fines.y == XXIV);
    _finire(w);
}

interior vacuum
_sectio_memoria (
    CredoContextus* c)
{
            Mundus* w = _mundum(c, _volumen(c, "statuae_memoria", I),
                CXX, CL);
    memoriae_index post_primum;
               i32 k;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, XXXIII);
    (vacuum)_pulsus(w, XXXIII);
    post_primum = piscina_summa_usus(w->m.piscina_quadri)
        + piscina_summa_usus(w->m.piscina_statuae);
    per (k = ZEPHYRUM; k < CC; k++)
    {
        CREDO_NECESSE (_pulsus(w, XXXIII) || VERUM);
    }
    CREDO_VERUM (piscina_summa_usus(w->m.piscina_quadri)
        + piscina_summa_usus(w->m.piscina_statuae) <= post_primum
            + 0x1000);
    _finire(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "primum", _sectio_primum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tempus", _sectio_tempus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quies", _sectio_quies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tractus", _sectio_tractus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "delectus", _sectio_delectus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modi", _sectio_modi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuum", _sectio_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusatum", _sectio_recusatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines", _sectio_magnitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "dispositio", _sectio_dispositio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "memoria", _sectio_memoria, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("statuae", SECTIONES);
}
