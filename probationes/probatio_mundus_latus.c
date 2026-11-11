/* probatio_mundus_latus.c - latus mundi (mundus_latus.h)
 *
 * Latus solum (ut probatio_statuae): repositorium, montatio,
 * dispensator, eventa ficta, horologium fictum; volumen temporarium
 * cum loco vero (planum.glb, genus LOCUS) et personis lapide et fungo.
 *
 * primum: quadrum post pulsum, persona ordinaria, caput. ambulat:
 * clavis tenta movet, liberata sistit, mora sequitur. focus: focus
 * amissus sistit. claves: contrariae, obliquae, modificantes, passus
 * primus post quietem. persona: [ ] et sagittae, locus manet. vacuum:
 * sine loco, sine personis, locus additus apparet. recusatum: locus
 * et persona quos lector recusat, locus sine solo gressibili.
 * magnitudines: latera minima sine ruina.
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
#include "mundus_latus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO CCCLX
#define ALTITUDO CDL

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"
#define VIA_PLANI   "probationes/fixa/statuae/planum.glb"

nomen structura {
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                Volumen* volumen;
    MundusLatusMontatio  m;
            Dispensator* d;
                    s64  hora;      /* horologium fictum, ms */
} Orbis;

interior s64
_nunc (
    vacuum* ctx)
{
    redde ((Orbis*)ctx)->hora;
}

interior chorda
_ch (
        CredoContextus* c,
    constans character* textus)
{
    redde chorda_ex_literis(textus, c->piscina);
}

/* volumen temporarium: locus verus si 'locus'; personae: I lapis, II
 * lapis et "mushroom man" (fungus) */
interior Volumen*
_volumen (
        CredoContextus* c,
    constans character* praefixum,
                   b32  locus,
                   i32  personae)
{
    Volumen* v = volumen_temporarium(c->piscina, praefixum);

    si (v && locus)
    {
        (vacuum)statuae_addere_generis(v, _ch(c, "clearing"),
            filum_legere_totum(VIA_PLANI, c->piscina),
            STATUAE_GENUS_LOCUS);
    }
    si (v && personae >= I)
    {
        (vacuum)statuae_addere(v, _ch(c, "lapis"),
            filum_legere_totum(VIA_LAPIDIS, c->piscina));
    }
    si (v && personae >= II)
    {
        (vacuum)statuae_addere(v, _ch(c, "mushroom man"),
            filum_legere_totum(VIA_FUNGI, c->piscina));
    }
    redde v;
}

interior Orbis*
_orbem (
    CredoContextus* c,
           Volumen* volumen,
               i32  latitudo,
               i32  altitudo)
{
               Orbis* w;
               Motus* motus;
    MundusLatusPorta  porta;

    thema_initiare();
    w = (Orbis*)piscina_allocare(c->piscina, magnitudo(Orbis));
    memset(w, ZEPHYRUM, magnitudo(Orbis));
    w->hora     = 100000;
    w->volumen  = volumen;
    porta.nunc  = _nunc;
    porta.ctx   = w;
    w->intern   = internamentum_creare(c->piscina);
    w->repo     = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !mundus_latus_montare(&w->m, c->piscina, w->intern, volumen,
               w->repo, "m1", &porta, latitudo, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, mundus_latus_componere, &w->m, CCC);
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
    Orbis* w)
{
    si (!w)
    {
        redde;
    }
    mundus_latus_dimittere(&w->m);
    si (w->volumen)
    {
        volumen_claudere(w->volumen);
    }
}

interior Componens*
_nodus (
                 Orbis* w,
    constans character* id,
               Piscina* p)
{
    redde componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
}

/* ictus muris sinister in medio componentis 'id' */
interior vacuum
_ictus (
                 Orbis* w,
    constans character* id,
               Piscina* p)
{
    Componens* n = _nodus(w, id, p);
      Eventus  e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = (n ? n->fines.x : ZEPHYRUM) + II;
    e.datum.mus.y       = (n ? n->fines.y : ZEPHYRUM) + II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* eventus clavis: depressa aut liberata, cum modificantibus */
interior vacuum
_clavis_eventus (
        Orbis* w,
    character  typus,
          b32  depressa,
          i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = depressa ? EVENTUS_CLAVIS_DEPRESSUS
                       : EVENTUS_CLAVIS_LIBERATUS;
    e.datum.clavis.producta      = (s32)(insignatus character)typus;
    e.datum.clavis.clavis        = (clavis_t)typus;
    e.datum.clavis.runa          = (s32)typus;
    e.datum.clavis.modificantes  = modificantes;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
_premere (
        Orbis* w,
    character  typus)
{
    _clavis_eventus(w, typus, VERUM, ZEPHYRUM);
}

interior vacuum
_solvere (
        Orbis* w,
    character  typus)
{
    _clavis_eventus(w, typus, FALSUM, ZEPHYRUM);
}

/* horologium promovere et pulsare */
interior b32
_pulsus (
    Orbis* w,
      s64  ms)
{
    b32 mutatum;

    w->hora += ms;
    mutatum = mundus_latus_pulsare(&w->m);
    dispensator_recomponere(w->d);
    redde mutatum;
}

/* 'numerus' pulsus XXXIII ms */
interior vacuum
_pulsus_multi (
    Orbis* w,
      i32  numerus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    }
}

interior constans Imago*
_quadrum (
    Orbis* w)
{
    redde mundus_latus_imago_fons(w->m.quadri_titulus, &w->m);
}

/* campus status: valor aequalis? valor NIHIL = campus abest */
interior b32
_campus (
                 Orbis* w,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);

    redde valor ? (a && chorda_aequalis_literis(*a, valor)) : !a;
}

interior Mandata*
_reddere (
      Orbis* w,
    Piscina* p)
{
    Mandata* md = mandata_creare(p, w->intern);

    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* estne textus pictus qui fragmentum continet? */
interior b32
_textus_pictus (
        CredoContextus* c,
                 Orbis* w,
    constans character* fragmentum)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus,
                   chorda_ex_literis(fragmentum, c->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_imago_picta (
    CredoContextus* c,
             Orbis* w)
{
              Mandata* md = _reddere(w, c->piscina);
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

interior vacuum
_sectio_primum (
    CredoContextus* c)
{
              Orbis* w = _orbem(c, _volumen(c, "mundus_primum", VERUM,
                  II), LATITUDO, ALTITUDO);
     constans Imago* im;
          Componens* pictura;
                i32  k;
                i32  opaca = ZEPHYRUM;

    CREDO_NECESSE (w != NIHIL);
    CREDO_VERUM (_pulsus(w, ZEPHYRUM));
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    /* quadrum aream picturae implet: latus totum sub capite */
    CREDO_AEQUALIS_I32 (im->latitudo, (i32)pictura->fines.latitudo);
    CREDO_AEQUALIS_I32 (im->altitudo, (i32)pictura->fines.altitudo);
    CREDO_AEQUALIS_I32 (im->latitudo, LATITUDO);
    /* persona in medio loci: humus undique, pixela opaca */
    per (k = ZEPHYRUM; k < im->latitudo * im->altitudo; k++)
    {
        opaca += im->pixela[k * IV + III] == 255 ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (opaca, im->latitudo * im->altitudo);
    CREDO_VERUM (mundus_latus_imago_fons(chorda_ex_literis("alia",
        c->piscina), &w->m) == NIHIL);
    /* persona ordinaria: "mushroom man" (altera in indice) */
    CREDO_VERUM (_textus_pictus(c, w, "2/2"));
    CREDO_VERUM (_textus_pictus(c, w, "mushroom man"));
    CREDO_VERUM (_imago_picta(c, w));
    CREDO_VERUM (chorda_aequalis_literis(mundus_latus_titulus(&w->m,
        c->piscina), "mushroom man"));
    CREDO_VERUM (w->m.positum);
    CREDO_VERUM (mundus_solum_stare(w->m.solum, w->m.corpus.x,
        w->m.corpus.z, w->m.corpus.radius));
    /* stat: pulsus rari; pulsus sine mutatione nihil mutat */
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    CREDO_FALSUM (_pulsus(w, C));
    _finire(w);
}

interior vacuum
_sectio_ambulat (
    CredoContextus* c)
{
     Orbis* w = _orbem(c, _volumen(c, "mundus_ambulat", VERUM, II),
         LATITUDO, ALTITUDO);
        i8* prius;
       f32  x0;
       f32  z0;
       f32  x1;
       f32  z1;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    prius = (i8*)piscina_allocare(c->piscina, LATITUDO * ALTITUDO * IV);
    memcpy(prius, _quadrum(w)->pixela, (memoriae_index)_quadrum(w)
        ->latitudo * _quadrum(w)->altitudo * IV);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* d tenta: secunda una = circa IV unitates dextrorsum in schirmo
     * (+x +z in mundo) */
    _premere(w, 'd');
    CREDO_VERUM (mundus_latus_mora(&w->m) == MUNDUS_LATUS_MORA_MOTUS);
    _pulsus_multi(w, XXX);
    x1 = w->m.corpus.x;
    z1 = w->m.corpus.z;
    CREDO_VERUM (x1 - x0 > 1.0f);
    CREDO_VERUM (z1 - z0 > 1.0f);
    CREDO_VERUM ((x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0) > 4.0f);
    CREDO_VERUM ((x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0) < 17.0f);
    CREDO_VERUM (w->m.corpus.movetur);
    CREDO_VERUM (mundus_latus_mora(&w->m) == MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM (memcmp(prius, _quadrum(w)->pixela,
        (memoriae_index)_quadrum(w)->latitudo * _quadrum(w)->altitudo
            * IV) != ZEPHYRUM);
    /* liberata: sistit; pulsus rari redeunt */
    _solvere(w, 'd');
    _pulsus_multi(w, III);
    x1 = w->m.corpus.x;
    z1 = w->m.corpus.z;
    CREDO_FALSUM (w->m.corpus.movetur);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x1, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z1, 0.0001f);
    /* a tenta: retro (-x -z) */
    _premere(w, 'a');
    _pulsus_multi(w, XV);
    CREDO_VERUM (w->m.corpus.x < x1 - 0.5f);
    CREDO_VERUM (w->m.corpus.z < z1 - 0.5f);
    _solvere(w, 'a');
    _finire(w);
}

interior vacuum
_sectio_focus (
    CredoContextus* c)
{
    Orbis* w = _orbem(c, _volumen(c, "mundus_focus", VERUM, II),
        LATITUDO, ALTITUDO);
      f32 x1;
      f32 z1;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    _premere(w, 'd');
    _pulsus_multi(w, X);
    CREDO_VERUM (w->m.corpus.movetur);
    /* focus amissus: liberatio numquam venit, persona tamen sistit */
    mundus_latus_focus_amissus(&w->m);
    _pulsus_multi(w, II);
    x1 = w->m.corpus.x;
    z1 = w->m.corpus.z;
    CREDO_FALSUM (w->m.corpus.movetur);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x1, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z1, 0.0001f);
    mundus_latus_focus_amissus(NIHIL);
    _finire(w);
}

interior vacuum
_sectio_claves (
    CredoContextus* c)
{
    Orbis* w = _orbem(c, _volumen(c, "mundus_claves", VERUM, II),
        LATITUDO, ALTITUDO);
      f32 x0;
      f32 z0;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* contrariae se tollunt */
    _premere(w, 'a');
    _premere(w, 'd');
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    _solvere(w, 'a');
    _solvere(w, 'd');
    /* cum modificante (Cmd, Ctrl, Alt): non ambulat */
    _clavis_eventus(w, 'd', VERUM, MOD_SUPER);
    _clavis_eventus(w, 'w', VERUM, MOD_IMPERIUM);
    _clavis_eventus(w, 's', VERUM, MOD_ALT);
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    /* w et d simul: oblique in schirmo = +x solum in mundo (visus
     * XLV gradus) */
    _premere(w, 'w');
    _premere(w, 'd');
    _pulsus_multi(w, XV);
    CREDO_VERUM (w->m.corpus.x > x0 + 1.0f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.3f);
    _solvere(w, 'w');
    _solvere(w, 'd');
    _pulsus_multi(w, III);
    /* w sola: sursum in schirmo = +x -z */
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    _premere(w, 'w');
    _pulsus_multi(w, X);
    CREDO_VERUM (w->m.corpus.x > x0 + 0.3f);
    CREDO_VERUM (w->m.corpus.z < z0 - 0.3f);
    _solvere(w, 'w');
    _pulsus_multi(w, III);
    /* Shift permittitur (littera maiuscula) */
    x0 = w->m.corpus.x;
    _clavis_eventus(w, 's', VERUM, MOD_SHIFT);
    _pulsus_multi(w, X);
    CREDO_VERUM (w->m.corpus.x < x0 - 0.3f);
    _clavis_eventus(w, 's', FALSUM, MOD_SHIFT);
    _pulsus_multi(w, III);
    /* passus primus post quietem longam: ab ictu clavis metitur, non
     * ab inspectione ultima (0.033 s = 0.13 unitates, non 0.4) */
    (vacuum)_pulsus(w, 5000);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* DCC ms post pulsum ultimum clavis premitur (latus quiescens
     * semel per secundam pulsatur) */
    w->hora += 700;
    _premere(w, 'd');
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM ((w->m.corpus.x - x0) * (w->m.corpus.x - x0)
        + (w->m.corpus.z - z0) * (w->m.corpus.z - z0) < 0.04f);
    CREDO_VERUM (w->m.corpus.x > x0);
    /* mora longa dum ambulat: gradus coactus, non translatio */
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    (vacuum)_pulsus(w, 8000);
    CREDO_VERUM ((w->m.corpus.x - x0) * (w->m.corpus.x - x0)
        + (w->m.corpus.z - z0) * (w->m.corpus.z - z0) < 0.2f);
    _solvere(w, 'd');
    /* claves ignotae: nihil */
    _premere(w, 'q');
    _solvere(w, 'q');
    _finire(w);
}

interior vacuum
_sectio_persona (
    CredoContextus* c)
{
    Orbis* w = _orbem(c, _volumen(c, "mundus_persona", VERUM, II),
        LATITUDO, ALTITUDO);
      f32 x0;
      f32 z0;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_campus(w, "persona", NIHIL));
    /* ambulat paulum, deinde personam mutat: locus manet */
    _premere(w, 'd');
    _pulsus_multi(w, X);
    _solvere(w, 'd');
    _pulsus_multi(w, III);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    _premere(w, ']');
    CREDO_VERUM (_campus(w, "persona", "lapis"));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM (_textus_pictus(c, w, "1/2"));
    CREDO_VERUM (_textus_pictus(c, w, "lapis"));
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    CREDO_VERUM (chorda_aequalis_literis(mundus_latus_titulus(&w->m,
        c->piscina), "lapis"));
    /* circulo: ] iterum ad fungum; [ retro */
    _premere(w, ']');
    CREDO_VERUM (_campus(w, "persona", "mushroom man"));
    _premere(w, '[');
    CREDO_VERUM (_campus(w, "persona", "lapis"));
    /* sagittae */
    _ictus(w, "sagitta_proxima", c->piscina);
    CREDO_VERUM (_campus(w, "persona", "mushroom man"));
    _ictus(w, "sagitta_prior", c->piscina);
    CREDO_VERUM (_campus(w, "persona", "lapis"));
    /* ictus in pictura: nihil mutat */
    _ictus(w, "pictura", c->piscina);
    CREDO_VERUM (_campus(w, "persona", "lapis"));
    _finire(w);
}

/* dithering humi: 'g' et globulus commutant; regiones veteres
 * abiciuntur, persona manet; retro ad Atkinson quadrum pristinum */
interior vacuum
_sectio_humus (
    CredoContextus* c)
{
     Orbis* w = _orbem(c, _volumen(c, "mundus_humus", VERUM, II),
         LATITUDO, ALTITUDO);
        i8* atkinson;
        i8* exemplaria;
       f32  x0;
       f32  z0;
       i32  octeti;
       i32  regiones;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    octeti      = _quadrum(w)->latitudo * _quadrum(w)->altitudo * IV;
    atkinson    = (i8*)piscina_allocare(c->piscina, octeti);
    exemplaria  = (i8*)piscina_allocare(c->piscina, octeti);
    memcpy(atkinson, _quadrum(w)->pixela, octeti);
    x0        = w->m.corpus.x;
    z0        = w->m.corpus.z;
    regiones  = mundus_humus_regiones(w->m.humus);
    CREDO_VERUM (regiones >= I);
    CREDO_VERUM (_campus(w, "humus", NIHIL));
    CREDO_VERUM (_textus_pictus(c, w, "atkinson"));
    /* g: exemplaria */
    _premere(w, 'g');
    CREDO_VERUM (_campus(w, "humus", "exemplaria"));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "exemplaria"));
    CREDO_FALSUM (_textus_pictus(c, w, "atkinson"));
    memcpy(exemplaria, _quadrum(w)->pixela, octeti);
    CREDO_VERUM (memcmp(atkinson, exemplaria, octeti) != ZEPHYRUM);
    /* persona ubi stabat; humus nova eadem regiones fecit, non plures
     * (veteres abiectae, non accumulatae) */
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(w->m.humus), regiones);
    CREDO_VERUM (w->m.humi_modus == MUNDUS_HUMUS_EXEMPLARIA);
    /* stans: quadrum non mutatur, pulsus rari */
    CREDO_FALSUM (_pulsus(w, C));
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    /* globulus: retro ad Atkinson, quadrum pristinum octetim */
    _ictus(w, "humus", c->piscina);
    CREDO_VERUM (_campus(w, "humus", NIHIL));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_NECESSE (_quadrum(w) != NIHIL);
    CREDO_VERUM (memcmp(atkinson, _quadrum(w)->pixela, octeti)
        == ZEPHYRUM);
    /* ambulat in humo exemplariorum */
    _premere(w, 'g');
    _premere(w, 'd');
    _pulsus_multi(w, X);
    _solvere(w, 'd');
    CREDO_VERUM (w->m.corpus.x > x0 + 0.5f);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    _finire(w);
    /* sine loco: g nihil ruit */
    w = _orbem(c, _volumen(c, "mundus_humus_vacuus", FALSUM, I),
        LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    _premere(w, 'g');
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM (_campus(w, "humus", "exemplaria"));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (w->m.humus == NIHIL);
    _finire(w);
}

interior vacuum
_sectio_vacuum (
    CredoContextus* c)
{
    Volumen* v = _volumen(c, "mundus_vacuum", FALSUM, II);
      Orbis* w = _orbem(c, v, LATITUDO, ALTITUDO);

    CREDO_NECESSE (w != NIHIL);
    /* sine loco */
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "No place yet"));
    CREDO_VERUM (_textus_pictus(c, w, "-locus"));
    CREDO_FALSUM (_imago_picta(c, w));
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    /* claves in latere sine loco: nihil ruit, nihil ambulat */
    _premere(w, 'd');
    _pulsus_multi(w, V);
    _solvere(w, 'd');
    _premere(w, ']');
    _ictus(w, "pictura", c->piscina);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    /* locus additus (ut ab alio processu): post secundam apparet */
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "clearing"),
        filum_legere_totum(VIA_PLANI, c->piscina),
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (_pulsus(w, 1100));
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_VERUM (_imago_picta(c, w));
    /* locus remotus: nuntius redit */
    CREDO_VERUM (statuae_removere(v, _ch(c, "clearing")));
    CREDO_VERUM (_pulsus(w, 1100));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "No place yet"));
    _finire(w);
    /* locus sine personis */
    w = _orbem(c, _volumen(c, "mundus_sine_personis", VERUM, ZEPHYRUM),
        LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "No characters yet"));
    CREDO_VERUM (_textus_pictus(c, w, "0/0"));
    CREDO_AEQUALIS_I32 (mundus_latus_titulus(&w->m, c->piscina).mensura,
        ZEPHYRUM);
    _premere(w, 'd');
    _pulsus_multi(w, III);
    _solvere(w, 'd');
    _finire(w);
    /* sine volumine */
    w = _orbem(c, NIHIL, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "No place yet"));
    CREDO_VERUM (mundus_latus_mora(&w->m) == -I);
    _finire(w);
}

interior vacuum
_sectio_recusatum (
    CredoContextus* c)
{
    Volumen* v = _volumen(c, "mundus_recusatum", FALSUM, ZEPHYRUM);
      Orbis* w;

    CREDO_NECESSE (v != NIHIL);
    /* locus qui GLB non est */
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "fractus"),
        _ch(c, "non glb!!"), STATUAE_GENUS_LOCUS));
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"),
        filum_legere_totum(VIA_LAPIDIS, c->piscina)));
    w = _orbem(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "This place cannot be shown"));
    CREDO_VERUM (_textus_pictus(c, w, "not a GLB"));
    _premere(w, 'd');
    _pulsus_multi(w, III);
    _solvere(w, 'd');
    _finire(w);
    /* locus sine solo gressibili (statua sine signis ut locus) */
    v = _volumen(c, "mundus_sine_solo", FALSUM, I);
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "saxum"),
        filum_legere_totum(VIA_FUNGI, c->piscina),
        STATUAE_GENUS_LOCUS));
    w = _orbem(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "nowhere to walk"));
    CREDO_FALSUM (w->m.positum);
    _finire(w);
    /* persona recusata: titulus et causa; sagitta ad sanam ducit */
    v = _volumen(c, "mundus_persona_fracta", VERUM, ZEPHYRUM);
    CREDO_VERUM (statuae_addere(v, _ch(c, "fracta"),
        _ch(c, "non glb!!")));
    CREDO_VERUM (statuae_addere(v, _ch(c, "lapis"),
        filum_legere_totum(VIA_LAPIDIS, c->piscina)));
    w = _orbem(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, "fracta"));
    CREDO_VERUM (_textus_pictus(c, w,
        "This character cannot be shown"));
    CREDO_VERUM (_textus_pictus(c, w, "not a GLB"));
    _premere(w, ']');
    CREDO_VERUM (_campus(w, "persona", "lapis"));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    _premere(w, '[');
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    _finire(w);
}

interior vacuum
_latus_parvum (
    CredoContextus* c,
               i32  latitudo,
               i32  altitudo)
{
    Orbis* w = _orbem(c, _volumen(c, "mundus_parvum", VERUM, II),
        latitudo, altitudo);

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    _premere(w, 'd');
    _pulsus_multi(w, III);
    _premere(w, ']');
    _pulsus_multi(w, II);
    _solvere(w, 'd');
    (vacuum)_reddere(w, c->piscina);
    CREDO_VERUM (w->m.positum);
    _finire(w);
}

interior vacuum
_sectio_magnitudines (
    CredoContextus* c)
{
    _latus_parvum(c, I, I);
    _latus_parvum(c, I, XIII);
    _latus_parvum(c, XIII, XIII);
    _latus_parvum(c, XX, C);
    _latus_parvum(c, 77, 55);
    _latus_parvum(c, 481, 543);
    /* argumenta NIHIL */
    CREDO_FALSUM (mundus_latus_pulsare(NIHIL));
    CREDO_VERUM (mundus_latus_mora(NIHIL) == -I);
    CREDO_VERUM (mundus_latus_imago_fons(chorda_ex_literis("x",
        c->piscina), NIHIL) == NIHIL);
    CREDO_FALSUM (mundus_latus_montare(NIHIL, c->piscina, NIHIL, NIHIL,
        NIHIL, NIHIL, NIHIL, X, X, NIHIL));
    mundus_latus_dimittere(NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "primum", _sectio_primum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambulat", _sectio_ambulat, NIHIL, NIHIL, NIHIL, NIHIL },
    { "focus", _sectio_focus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "persona", _sectio_persona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "humus", _sectio_humus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuum", _sectio_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusatum", _sectio_recusatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines", _sectio_magnitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_latus", SECTIONES);
}
