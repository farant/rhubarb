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
 *
 * OSTIA (ostium-plan T3). ostia: nulla, remotum, propinquum - capsa
 * signi ubi punctum proiectum est, textus, area; ambulanti in mundo
 * fixum; propinquum iff distantia <= radius, gradu quoque. ostia_plura:
 * proximum, paria prius, index substitutus sequitur. ostia_margo:
 * capsa ad marginem picturae coacta. ostia_altitudo: solum ostii
 * altius quam persona; ostium in saxo altitudinem personae sumit.
 * ostia_vacua: sine persona, locus substitutus dum ostium propinquum.
 *
 * INTRARE ET EXIRE (ostium-plan T4). intrare: E procul nihil; E et
 * ictus in signo intrant (campi, claves purgatae, mundus stat);
 * intus W A S D g [ ] mortuae, scaena vivit; exitus "mundus" (mus et
 * claves) personam in cella ostio proxima ponit, quiescentem.
 * restitutio: latus iterum montatum intus incipit; locus iterum
 * conditus dum intus; ostium non iam utile aut illegibile: initium.
 * ignota: scaena absens, remota dum intus, massa recusata - nuntius,
 * E et ictus ubivis exeunt; sine loco nihil ruit.
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
#include "statua.h"
#include "statua_glb.h"
#include "statua_glb_scriptor.h"
#include "plastica.h"
#include "mundus_locus.h"
#include "mundus_ostia.h"
#include "mundus_pictura.h"
#include "mundus_solum.h"
#include "graphis.h"
#include "scaena.h"
#include "scaena_repositorium.h"
#include "scaena_latus.h"
#include "stereometria.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LATITUDO CCCLX
#define ALTITUDO CDL
/* sectiones scaenae hospitatae: latus picturam totam capit */
#define LATITUDO_SCAENAE GRAPHIS_LATITUDO

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"
#define VIA_PLANI   "probationes/fixa/statuae/planum.glb"
#define VIA_CASAE   "apps/scaena/loci/casa.scaena.stml"

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

/* plastica-plan T9 (spec AUDIENDA 6): locus apertus sub titulo
 * eodem substitutus (bin/mundus condere) relegitur - tabula gressus
 * nova valet et pictura manet */
interior vacuum
_sectio_substitutus (
    CredoContextus* c)
{
                  Volumen* v = _volumen(c, "mundus_substitutus",
                      VERUM, I);
                    Orbis* w;
            StatuaFructus  nudus = statua_glb_legere(
                filum_legere_totum(VIA_PLANI, c->piscina), c->piscina);
          PlasticaFructus arca = plastica_legere(_ch(c,
              "<res titulus=\"arca\">"
              "<capsa magnitudo=\"2 2 2\" color=\"#855\""
              " signum=\"solidum\"/>"
              "</res>"), c->piscina);
           MundusLocusRes  positio;
    MundusLocusInstructus  instructus;
          constans Statua* statuae[I];
                   chorda  octeti;

    CREDO_NECESSE (v != NIHIL && nudus.successus && arca.successus);
    w = _orbem(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.locus != NIHIL && w->m.solum != NIHIL);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(w->m.solum, 27.0f, 29.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    /* locus instructus: arca ad (27, 29) */
    memset(&positio, ZEPHYRUM, magnitudo(positio));
    positio.x          = 27.0f;
    positio.z          = 29.0f;
    positio.scala      = 1.0f;
    positio.linea      = I;
    statuae[ZEPHYRUM]  = arca.statua;
    instructus = mundus_locus_struere(nudus.statua, &positio, statuae,
        I, c->piscina);
    CREDO_NECESSE (instructus.successus);
    octeti = statua_glb_scribere(instructus.statua, NIHIL, c->piscina);
    CREDO_NECESSE (octeti.datum != NIHIL);
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "clearing"), octeti,
        STATUAE_GENUS_LOCUS));
    /* ante inspectionem voluminis locus vetus manet */
    (vacuum)_pulsus(w, CC);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(w->m.solum, 27.0f, 29.0f),
        MUNDUS_SOLUM_GRESSIBILE);
    /* post secundam: locus novus, arca solum claudit, pictura adest */
    CREDO_VERUM (_pulsus(w, 1100));
    CREDO_NECESSE (w->m.locus != NIHIL && w->m.solum != NIHIL);
    CREDO_AEQUALIS_I32 (w->m.locus->nodorum,
        instructus.statua->nodorum);
    CREDO_AEQUALIS_I32 (mundus_solum_genus(w->m.solum, 27.0f, 29.0f),
        MUNDUS_SOLUM_SOLIDUM);
    CREDO_VERUM (w->m.positum);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_VERUM (_imago_picta(c, w));
    /* persona adhuc ambulat */
    _premere(w, 'd');
    _pulsus_multi(w, V);
    _solvere(w, 'd');
    CREDO_VERUM (_quadrum(w) != NIHIL);
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


/* ==================================================
 * Ostia
 * ================================================== */

#define SIGNI_LATITUDO (III * VI + II * MUNDUS_LATUS_SIGNI_LIMBUS)
#define SIGNI_ALTITUDO (VIII + II * MUNDUS_LATUS_SIGNI_LIMBUS)
#define CAPUT_LATERIS  XII

interior s32
_millesimae (
    f32 x)
{
    redde (s32)floor((f64)x * 1000.0 + 0.5);
}

interior MundusOstium
_ostium (
        CredoContextus* c,
    constans character* scaena,
                   f32  x,
                   f32  z,
                   f32  radius)
{
    MundusOstium o;

    o.scaena             = _ch(c, scaena);
    o.x_millesimae       = _millesimae(x);
    o.z_millesimae       = _millesimae(z);
    o.radius_millesimae  = _millesimae(radius);
    redde o;
}

/* ostia loci "clearing" ponere et volumen inspici facere */
interior vacuum
_ostia_pone (
          CredoContextus* c,
                   Orbis* w,
    constans MundusOstium* ostia,
                     i32  numerus)
{
    CREDO_VERUM (mundus_ostia_ponere(w->volumen, _ch(c, "clearing"),
        ostia, numerus));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
}

/* capsa signi ut probatio eam computat: punctum (x, y +
 * altitudo signi, z) proiectum, minus origo quadri, in spatio
 * radicis; NON coacta */
interior Fines
_signum_speratum_alti (
    Orbis* w,
      f32  x,
      f32  y,
      f32  z,
      i32  altitudo_lateris)
{
    Fines f;
      s32 puncti_x  = ZEPHYRUM;
      s32 puncti_y  = ZEPHYRUM;
      s32 origo_x   = ZEPHYRUM;
      s32 origo_y   = ZEPHYRUM;

    mundus_pixelum_puncti(stereometria_vector(x,
        y + MUNDUS_LATUS_SIGNI_ALTITUDO, z), &puncti_x, &puncti_y);
    (vacuum)mundus_quadri_origo(&w->m.mundus, ZEPHYRUM, LATITUDO,
        altitudo_lateris - CAPUT_LATERIS, &origo_x, &origo_y);
    f.x         = puncti_x - origo_x - SIGNI_LATITUDO / II;
    f.y         = CAPUT_LATERIS + puncti_y - origo_y - SIGNI_ALTITUDO;
    f.latitudo  = SIGNI_LATITUDO;
    f.altitudo  = SIGNI_ALTITUDO;
    redde f;
}

interior Fines
_signum_speratum (
    Orbis* w,
      f32  x,
      f32  y,
      f32  z)
{
    redde _signum_speratum_alti(w, x, y, z, ALTITUDO);
}

/* capsa ad marginem picturae coacta (ut latus facit) */
interior Fines
_coacta (
    Fines f)
{
    si (f.x + SIGNI_LATITUDO > LATITUDO)
    {
        f.x = LATITUDO - SIGNI_LATITUDO;
    }
    si (f.y + SIGNI_ALTITUDO > ALTITUDO)
    {
        f.y = ALTITUDO - SIGNI_ALTITUDO;
    }
    si (f.x < ZEPHYRUM)
    {
        f.x = ZEPHYRUM;
    }
    si (f.y < CAPUT_LATERIS)
    {
        f.y = CAPUT_LATERIS;
    }
    redde f;
}

interior b32
_fines_pares (
    Fines a,
    Fines b)
{
    redde a.x == b.x && a.y == b.y && a.latitudo == b.latitudo
        && a.altitudo == b.altitudo;
}

/* mandata signi picta: rectangulum impletum et limbus his finibus,
 * textus "[E]" intra limbum */
interior b32
_signum_pictum (
    CredoContextus* c,
             Orbis* w,
             Fines  f)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* x;
                  b32  fundus = FALSUM;
                  b32  limbus = FALSUM;
                  b32  textus = FALSUM;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_RECTANGULUM
            && _fines_pares(x->fines, f))
        {
            si (x->impletum)
            {
                fundus = VERUM;
            }
            alioquin
            {
                limbus = VERUM;
            }
        }
        si (   x->genus   == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, MUNDUS_LATUS_SIGNUM)
            && x->fines.x == f.x + MUNDUS_LATUS_SIGNI_LIMBUS
            && x->fines.y == f.y + MUNDUS_LATUS_SIGNI_LIMBUS)
        {
            /* textus POST fundum pingitur */
            textus = fundus;
        }
    }
    redde fundus && limbus && textus;
}

interior f32
_distantia (
    Orbis* w,
      f32  x,
      f32  z)
{
    f32 spatium_x = w->m.corpus.x - x;
    f32 spatium_z = w->m.corpus.z - z;

    redde (f32)sqrt((f64)(spatium_x * spatium_x
        + spatium_z * spatium_z));
}

interior vacuum
_sectio_ostia (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "mundus_ostia", VERUM, II),
                LATITUDO, ALTITUDO);
     MundusOstium  o[II];
        Componens* area;
            Fines  f;
            Fines  prior;
            Fines  speratum;
              f32  x0;
              f32  z0;
              f32  y0  = 0.0f;
              f32  ostii_x;
              f32  ostii_z;
              i32  k;
              i32  propinqua  = ZEPHYRUM;
              i32  remota     = ZEPHYRUM;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum && _quadrum(w) != NIHIL);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* nulla ostia: nullum signum */
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    CREDO_VERUM (_nodus(w, "ostium", c->piscina) == NIHIL);
    CREDO_FALSUM (mundus_latus_signum(NIHIL, &f));
    /* ostium remotum (III unitates, radius 1.5): lectum, non
     * propinquum */
    o[ZEPHYRUM] = _ostium(c, "hut-door", x0 + 3.0f, z0, 1.5f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostia != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(w->m.ostia), I);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    /* ostium propinquum (I unitas): signum ubi punctum proiectum
     * est. Ostia SOLA mutata (locus idem) leguntur */
    ostii_x      = x0 + 1.0f;
    ostii_z      = z0;
    o[ZEPHYRUM]  = _ostium(c, "hut-door", ostii_x, ostii_z, 1.5f);
    _ostia_pone(c, w, o, I);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, ostii_x, ostii_z,
        &y0));
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    speratum = _signum_speratum(w, ostii_x, y0, ostii_z);
    CREDO_AEQUALIS_S32 (f.x, speratum.x);
    CREDO_AEQUALIS_S32 (f.y, speratum.y);
    CREDO_AEQUALIS_I32 ((i32)f.latitudo, SIGNI_LATITUDO);
    CREDO_AEQUALIS_I32 ((i32)f.altitudo, SIGNI_ALTITUDO);
    /* supra et dextrorsum a persona (medio picturae) */
    CREDO_VERUM (f.y + (s32)f.altitudo < CAPUT_LATERIS
        + (ALTITUDO - CAPUT_LATERIS) / II);
    CREDO_VERUM (_signum_pictum(c, w, f));
    area = _nodus(w, "ostium", c->piscina);
    CREDO_NECESSE (area != NIHIL);
    CREDO_VERUM (_fines_pares(area->fines, f));
    /* ambulat: signum in MUNDO fixum - in schirmo cum origine
     * quadri movetur; propinquum iff distantia <= radius, gradu
     * quoque */
    prior = f;
    _premere(w, 'd');
    per (k = ZEPHYRUM; k < CC && remota < V; k++)
    {
        (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_MOTUS);
        si (_distantia(w, ostii_x, ostii_z) <= 1.5f)
        {
            propinqua++;
            CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
            CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
            speratum = _signum_speratum(w, ostii_x, y0, ostii_z);
            CREDO_VERUM (_fines_pares(f, speratum));
            CREDO_VERUM (_signum_pictum(c, w, f));
        }
        alioquin
        {
            remota++;
            CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
            CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
            CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
            CREDO_VERUM (_nodus(w, "ostium", c->piscina) == NIHIL);
        }
    }
    _solvere(w, 'd');
    CREDO_VERUM (propinqua >= III);
    CREDO_AEQUALIS_I32 (remota, V);
    /* dum propinquum erat, capsa in schirmo mota est */
    CREDO_VERUM (w->m.corpus.x > x0 + 1.0f);
    (vacuum)prior;
    _finire(w);
}

interior vacuum
_sectio_ostia_plura (
    CredoContextus* c)
{
           Orbis* w = _orbem(c, _volumen(c, "mundus_ostia2", VERUM,
               II), LATITUDO, ALTITUDO);
    MundusOstium o[III];
           Fines f;
           Fines prior;
             f32 x0;
             f32 z0;
             f32 y0 = 0.0f;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* duo in radio: proximum vincit, quocumque ordine */
    o[ZEPHYRUM]  = _ostium(c, "longe", x0 + 1.25f, z0, 2.0f);
    o[I]         = _ostium(c, "prope", x0, z0 + 0.5f, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, I);
    CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, x0, z0 + 0.5f,
        &y0));
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_VERUM (_fines_pares(f, _signum_speratum(w, x0, y0,
        z0 + 0.5f)));
    prior        = f;
    o[ZEPHYRUM]  = _ostium(c, "prope", x0, z0 + 0.5f, 2.0f);
    o[I]         = _ostium(c, "longe", x0 + 1.25f, z0, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_VERUM (_fines_pares(f, prior));
    /* proximum cuius radius non attingit non numeratur: remotius
     * cuius radius attingit vincit */
    o[ZEPHYRUM]  = _ostium(c, "prope", x0, z0 + 0.75f, 0.5f);
    o[I]         = _ostium(c, "longe", x0 + 1.25f, z0, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, I);
    /* paria (unitas una utrimque): prius in indice */
    o[ZEPHYRUM]  = _ostium(c, "sinistrum", x0 - 1.0f, z0, 2.0f);
    o[I]         = _ostium(c, "dextrum", x0 + 1.0f, z0, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    o[ZEPHYRUM]  = _ostium(c, "dextrum", x0 + 1.0f, z0, 2.0f);
    o[I]         = _ostium(c, "sinistrum", x0 - 1.0f, z0, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_VERUM (_fines_pares(f, _signum_speratum(w, x0 + 1.0f, y0,
        z0)));
    /* in margine ipso radii: propinquum (non plus radio) */
    o[ZEPHYRUM] = _ostium(c, "margo", x0 + 1.0f, z0, 1.0f);
    _ostia_pone(c, w, o, I);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    /* index vacuus: signum abit; index ex memoria non legitur */
    _ostia_pone(c, w, NIHIL, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    /* ostia loci ALTERIUS non leguntur */
    o[ZEPHYRUM] = _ostium(c, "alibi", x0, z0, 2.0f);
    CREDO_VERUM (mundus_ostia_ponere(w->volumen, _ch(c, "cave"), o,
        I));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    _finire(w);
}

interior vacuum
_sectio_ostia_margo (
    CredoContextus* c)
{
           Orbis* w = _orbem(c, _volumen(c, "mundus_ostia3", VERUM,
               II), LATITUDO, ALTITUDO);
    MundusOstium o[I];
           Fines f;
           Fines speratum;
             f32 x0;
             f32 z0;
             f32 y0 = 0.0f;
             i32 k;
             b32 sinister  = FALSUM;
             b32 dexter    = FALSUM;
    /* directiones mundi: in schirmo dextrorsum, sinistrorsum,
     * sursum, deorsum */
    hic_manens constans f32 DIRECTIONES[IV][II] = {
        { 5.5f, 5.5f }, { -5.5f, -5.5f }, { 5.5f, -5.5f },
        { -5.5f, 5.5f }
    };

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        f32 ostii_x = x0 + DIRECTIONES[k][ZEPHYRUM];
        f32 ostii_z = z0 + DIRECTIONES[k][I];
        s32 x_coactum;
        s32 y_coactum;

        /* radius maximus, punctum extra picturam */
        o[ZEPHYRUM] = _ostium(c, "procul", ostii_x, ostii_z, 8.0f);
        _ostia_pone(c, w, o, I);
        CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
        CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, ostii_x,
            ostii_z, &y0));
        speratum  = _signum_speratum(w, ostii_x, y0, ostii_z);
        sinister  = sinister || speratum.x < ZEPHYRUM;
        dexter    = dexter || speratum.x + SIGNI_LATITUDO > LATITUDO;
        CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
        x_coactum = speratum.x < ZEPHYRUM ? ZEPHYRUM
            : speratum.x + SIGNI_LATITUDO > LATITUDO
                ? LATITUDO - SIGNI_LATITUDO : speratum.x;
        y_coactum = speratum.y < CAPUT_LATERIS ? CAPUT_LATERIS
            : speratum.y + SIGNI_ALTITUDO > ALTITUDO
                ? ALTITUDO - SIGNI_ALTITUDO : speratum.y;
        CREDO_AEQUALIS_S32 (f.x, x_coactum);
        CREDO_AEQUALIS_S32 (f.y, y_coactum);
        CREDO_AEQUALIS_I32 ((i32)f.latitudo, SIGNI_LATITUDO);
        CREDO_AEQUALIS_I32 ((i32)f.altitudo, SIGNI_ALTITUDO);
        CREDO_VERUM (_signum_pictum(c, w, f));
    }
    /* puncta vere extra picturam ceciderunt: sinistrorsum et
     * dextrorsum */
    CREDO_VERUM (sinister);
    CREDO_VERUM (dexter);
    _finire(w);
    /* margines imus et summus: latus humile (visus inclinatus
     * puncta supra et infra comprimit - in latere alto intra
     * picturam manent) */
    w = _orbem(c, _volumen(c, "mundus_ostia3b", VERUM, II), LATITUDO,
        C);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    /* deorsum in schirmo = -x +z in mundo; sursum = +x -z */
    o[ZEPHYRUM] = _ostium(c, "infra", x0 - 5.5f, z0 + 5.5f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, x0 - 5.5f,
        z0 + 5.5f, &y0));
    speratum = _signum_speratum_alti(w, x0 - 5.5f, y0, z0 + 5.5f, C);
    CREDO_VERUM (speratum.y + SIGNI_ALTITUDO > C);
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_AEQUALIS_S32 (f.y, C - SIGNI_ALTITUDO);
    CREDO_AEQUALIS_S32 (f.x, speratum.x);
    o[ZEPHYRUM] = _ostium(c, "supra", x0 + 5.5f, z0 - 5.5f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, x0 + 5.5f,
        z0 - 5.5f, &y0));
    speratum = _signum_speratum_alti(w, x0 + 5.5f, y0, z0 - 5.5f, C);
    CREDO_VERUM (speratum.y < CAPUT_LATERIS);
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_AEQUALIS_S32 (f.y, CAPUT_LATERIS);
    CREDO_AEQUALIS_S32 (f.x, speratum.x);
    _finire(w);
}

/* volumen cum loco FICTO (mundus_locus_fictus.h) et personis duabus */
interior Volumen*
_volumen_fictum (
        CredoContextus* c,
    constans character* praefixum)
{
    Volumen* v = _volumen(c, praefixum, FALSUM, II);
     chorda  octeti = statua_glb_scribere(loci_ficti_locus(c), NIHIL,
         c->piscina);

    si (v && octeti.datum)
    {
        (vacuum)statuae_addere_generis(v, _ch(c, "clearing"), octeti,
            STATUAE_GENUS_LOCUS);
    }
    redde v;
}

interior vacuum
_sectio_ostia_altitudo (
    CredoContextus* c)
{
           Orbis* w = _orbem(c, _volumen_fictum(c, "mundus_ostia4"),
               LATITUDO, ALTITUDO);
    MundusOstium o[I];
           Fines f;
             f32 y = 0.0f;
             f32 lx;
             f32 lz;

    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum && _quadrum(w) != NIHIL);
    /* ostium in suggesto superiore (y 4), persona infra: signum in
     * altitudine SOLI OSTII, non personae */
    lx           = w->m.corpus.x < 8.0f ? 8.5f : w->m.corpus.x;
    lz           = w->m.corpus.z;
    o[ZEPHYRUM]  = _ostium(c, "ledge", lx, lz, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    CREDO_NECESSE (mundus_solum_altitudo(w->m.solum, lx, lz, &y));
    CREDO_F32_PROXIMUS (y, 4.0f, 0.001f);
    CREDO_VERUM (w->m.corpus.y < 3.0f);
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_VERUM (_fines_pares(f, _coacta(_signum_speratum(w, lx, y,
        lz))));
    CREDO_FALSUM (_fines_pares(f, _coacta(_signum_speratum(w, lx,
        w->m.corpus.y, lz))));
    /* ostium in saxo solido (ubi solum gressibile non est):
     * altitudo PERSONAE */
    o[ZEPHYRUM] = _ostium(c, "rock", 5.5f, 1.5f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    CREDO_FALSUM (mundus_solum_altitudo(w->m.solum, 5.5f, 1.5f, &y));
    CREDO_NECESSE (mundus_latus_signum(&w->m, &f));
    CREDO_VERUM (_fines_pares(f, _coacta(_signum_speratum(w, 5.5f,
        w->m.corpus.y, 1.5f))));
    _finire(w);
}

interior vacuum
_sectio_ostia_vacua (
    CredoContextus* c)
{
           Orbis* w;
         Volumen* v;
    MundusOstium  o[II];
           Fines  f;
             f32  x0;
             f32  z0;

    /* sine personis: ostium in volumine, signum nullum */
    w = _orbem(c, _volumen(c, "mundus_ostia5", VERUM, ZEPHYRUM),
        LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    o[ZEPHYRUM] = _ostium(c, "hut-door", 27.0f, 29.0f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    CREDO_VERUM (_nodus(w, "ostium", c->piscina) == NIHIL);
    _finire(w);
    /* sine volumine: nihil ruit */
    w = _orbem(c, NIHIL, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    _finire(w);
    /* locus sine solo gressibili (persona nusquam posita): ostium
     * circa originem lectum, numquam propinquum */
    v = _volumen(c, "mundus_ostia7", FALSUM, I);
    CREDO_NECESSE (v != NIHIL);
    CREDO_VERUM (statuae_addere_generis(v, _ch(c, "clearing"),
        filum_legere_totum(VIA_FUNGI, c->piscina),
        STATUAE_GENUS_LOCUS));
    w = _orbem(c, v, LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_FALSUM (w->m.positum);
    o[ZEPHYRUM] = _ostium(c, "hut-door", 0.0f, 0.0f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostia != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(w->m.ostia), I);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    _finire(w);
    /* locus substitutus dum ostium SECUNDUM propinquum est, et index
     * simul brevior factus: index vetus non manet */
    w = _orbem(c, _volumen(c, "mundus_ostia6", VERUM, II), LATITUDO,
        ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0           = w->m.corpus.x;
    z0           = w->m.corpus.z;
    o[ZEPHYRUM]  = _ostium(c, "longe", x0 + 6.0f, z0, 1.0f);
    o[I]         = _ostium(c, "prope", x0 + 0.5f, z0, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, I);
    CREDO_VERUM (statuae_addere_generis(w->volumen, _ch(c, "clearing"),
        statua_glb_scribere(loci_ficti_locus(c), NIHIL, c->piscina),
        STATUAE_GENUS_LOCUS));
    o[ZEPHYRUM] = _ostium(c, "ledge", 10.0f, 4.0f, 8.0f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.positum);
    CREDO_AEQUALIS_I32 (xar_numerus(w->m.ostia), I);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    CREDO_VERUM (mundus_latus_signum(&w->m, &f));
    /* locus sine ostiis: nullum */
    _ostia_pone(c, w, NIHIL, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_FALSUM (mundus_latus_signum(&w->m, &f));
    _finire(w);
}


/* ==================================================
 * Intrare et exire (ostium-plan T4)
 * ================================================== */

interior chorda
_fons_plagularum (
    constans character* directorium,
                chorda  titulus,
               Piscina* piscina)
{
    character via[CXXVIII];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    (vacuum)sprintf(via, "%s/%.*s.stml", directorium,
        (integer)titulus.mensura,
        (constans character*)titulus.datum);
    si (!filum_existit(via))
    {
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

interior chorda
_fons_picturarum (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    (vacuum)contextus;
    redde _fons_plagularum("apps/scaena/loci", titulus, piscina);
}

interior chorda
_fons_partium (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    (vacuum)contextus;
    redde _fons_plagularum("graphis/res", titulus, piscina);
}

/* casa composita: prospectus[0] hut-hearth, [1] hut-door */
interior ScaenaFructus
_casa (
    CredoContextus* c)
{
    redde scaena_struere(filum_legere_totum(VIA_CASAE, c->piscina),
        _fons_picturarum, _fons_partium, NIHIL, c->piscina);
}

/* casam in volumen addere */
interior b32
_casam_addere (
    CredoContextus* c,
           Volumen* v)
{
    ScaenaFructus f = _casa(c);
              i32 k;

    si (!v || !f.successus || f.numerus != II)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < f.numerus; k++)
    {
        si (!scaena_addere(v, f.prospectus[k].titulus,
            scaena_scribere(&f.prospectus[k], c->piscina)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParProbae;

interior vacuum
_campum_pone (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParProbae* par = (ParProbae*)ctx;

    si (!par->valor)
    {
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

/* campum rami scribere ut latus ipsum faceret; NIHIL tollit */
interior b32
_campum_scribe (
        CredoContextus* c,
                 Orbis* w,
    constans character* titulus,
    constans character* valor)
{
    ParProbae par;
       chorda prior = w->repo->scriptor;
          b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    insula_scriptorem_ponere(w->repo, _ch(c, "mundus.agere"));
    ok = mutare_ramum(&w->m.ramus, _campum_pone, &par);
    insula_scriptorem_ponere(w->repo, prior);
    redde ok;
}

/* latus dimittere et in ramo eodem iterum montare (vicus
 * restitutus) */
interior b32
_iterum_montare (
    CredoContextus* c,
             Orbis* w)
{
    MundusLatusPorta  porta;
               Motus* motus;

    mundus_latus_dimittere(&w->m);
    porta.nunc  = _nunc;
    porta.ctx   = w;
    si (!mundus_latus_montare(&w->m, c->piscina, w->intern, w->volumen,
        w->repo, "m1", &porta, LATITUDO_SCAENAE, ALTITUDO, NIHIL))
    {
        redde FALSUM;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, mundus_latus_componere, &w->m, CCC);
    si (!w->d)
    {
        redde FALSUM;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde VERUM;
}

/* ictus muris in puncto radicis */
interior vacuum
_ictus_botonis (
           Orbis* w,
             s32  x,
             s32  y,
    mus_botton_t  botton)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = x;
    e.datum.mus.y       = y;
    e.datum.mus.botton  = botton;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* ictus sinister in puncto radicis */
interior vacuum
_ictus_in (
    Orbis* w,
      s32  x,
      s32  y)
{
    _ictus_botonis(w, x, y, MUS_SINISTER);
}

/* ictus in pixelo INTERIORE regionis rei 'titulus' prospectus 'p'
 * (pictura scaenae in area "pictura") */
interior b32
_rem_icere (
                        Orbis* w,
    constans ScaenaProspectus* p,
           constans character* titulus,
                      Piscina* piscina)
{
    Componens* n        = _nodus(w, "pictura", piscina);
          i32  dominus  = ZEPHYRUM;
          s32  x;
          s32  y;
          i32  k;

    per (k = ZEPHYRUM; k < p->rerum; k++)
    {
        si (chorda_aequalis_literis(p->res[k].titulus, titulus))
        {
            dominus = k + I;
        }
    }
    si (!n || dominus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (y = I; y < GRAPHIS_ALTITUDO - I; y++)
    {
        per (x = I; x < GRAPHIS_LATITUDO - I; x++)
        {
            constans i8* d = p->domini + y * GRAPHIS_LATITUDO + x;

            si (   d[ZEPHYRUM] == dominus && d[-I] == dominus
                && d[I] == dominus && d[-GRAPHIS_LATITUDO] == dominus
                && d[GRAPHIS_LATITUDO] == dominus)
            {
                _ictus_in(w, n->fines.x + x, n->fines.y + y);
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_reditus (
    Orbis* w)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_REDITUS;
    e.datum.clavis.producta  = XIII;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* distantia minima a (x, z) ad centrum cellae in qua corpus stare
 * potest, intra radium (vi bruta); negativa si nulla */
interior f32
_proxima_distantia (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32  radius)
{
    f32 cella    = 1.0f / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    f32 optimum  = -1.0f;
    i32 i;
    i32 j;

    per (j = ZEPHYRUM; j < s->altitudo; j++)
    {
        per (i = ZEPHYRUM; i < s->latitudo; i++)
        {
            f32 medium_x = s->origo_x + ((f32)i + 0.5f) * cella;
            f32 medium_z = s->origo_z + ((f32)j + 0.5f) * cella;
            f32 spatium    = (f32)sqrt((f64)((medium_x - x)
                * (medium_x - x) + (medium_z - z) * (medium_z - z)));

            si (   spatium <= radius
                && mundus_solum_stare(s, medium_x, medium_z,
                       MUNDUS_CORPORIS_RADIUS)
                && (optimum < 0.0f || spatium < optimum))
            {
                optimum = spatium;
            }
        }
    }
    redde optimum;
}

/* corpus in centro cellae stat? */
interior b32
_in_centro_cellae (
    Orbis* w)
{
    f32 cella   = 1.0f / (f32)MUNDUS_CELLAE_PER_UNITATEM;
    f32 pars_x  = (w->m.corpus.x - w->m.solum->origo_x) / cella;
    f32 pars_z  = (w->m.corpus.z - w->m.solum->origo_z) / cella;

    pars_x -= (f32)floor((f64)pars_x);
    pars_z -= (f32)floor((f64)pars_z);
    redde fabs((f64)pars_x - 0.5) < 0.001
        && fabs((f64)pars_z - 0.5) < 0.001;
}

/* latus foris est: campi vacui, areae mundi, nulla scaenae */
interior vacuum
_foris_probare (
    CredoContextus* c,
             Orbis* w)
{
    CREDO_FALSUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", NIHIL));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_VERUM (_campus(w, "ostium", NIHIL));
    CREDO_VERUM (_nodus(w, "humus", c->piscina) != NIHIL);
    CREDO_VERUM (_nodus(w, "sagitta_prior", c->piscina) != NIHIL);
    CREDO_VERUM (mundus_latus_imago_fons(w->m.scaenae_titulus, &w->m)
        == NIHIL);
}

/* persona ad ostium (ostii_x, ostii_z) reddita: in centro cellae
 * gressibilis ostio PROXIMAE, quiescens */
interior vacuum
_ad_ostium_probare (
    CredoContextus* c,
             Orbis* w,
               f32  ostii_x,
               f32  ostii_z,
               f32  radius)
{
    f32 speratum = _proxima_distantia(w->m.solum, ostii_x, ostii_z,
        radius);

    CREDO_VERUM (speratum >= 0.0f);
    CREDO_VERUM (w->m.positum);
    CREDO_F32_PROXIMUS (_distantia(w, ostii_x, ostii_z), speratum,
        0.0005f);
    CREDO_VERUM (_in_centro_cellae(w));
    CREDO_VERUM (mundus_solum_stare(w->m.solum, w->m.corpus.x,
        w->m.corpus.z, w->m.corpus.radius));
    CREDO_FALSUM (w->m.corpus.movetur);
    CREDO_FALSUM (w->m.corpus.intendit);
    CREDO_FALSUM (w->m.tentae[ZEPHYRUM] || w->m.tentae[I]
        || w->m.tentae[II] || w->m.tentae[III]);
    (vacuum)c;
}

interior vacuum
_sectio_intrare (
    CredoContextus* c)
{
              Volumen* v = _volumen(c, "mundus_intrare", VERUM, II);
                Orbis* w;
        ScaenaFructus  casa = _casa(c);
     ScaenaProspectus* focus;
     ScaenaProspectus* ianua;
         MundusOstium  o[I];
       constans Imago* im;
            character  speratum[LXXX];
                  f32  x0;
                  f32  z0;
                  f32  ostii_x;
                  f32  ostii_z;
                  f32  x1;
                  f32  z1;
                  f32  facies;
                  f32  tempus;
                  i32  k;

    CREDO_NECESSE (_casam_addere(c, v));
    CREDO_NECESSE (casa.successus && casa.numerus == II);
    focus  = &casa.prospectus[ZEPHYRUM];
    ianua  = &casa.prospectus[I];
    w      = _orbem(c, v, LATITUDO_SCAENAE, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum && _quadrum(w) != NIHIL);
    x0 = w->m.corpus.x;
    z0 = w->m.corpus.z;
    _foris_probare(c, w);
    /* E sine ostiis et cum ostio remoto: nihil */
    _premere(w, 'e');
    _solvere(w, 'e');
    _foris_probare(c, w);
    o[ZEPHYRUM] = _ostium(c, "hut-door", x0 + 4.0f, z0, 1.5f);
    _ostia_pone(c, w, o, I);
    _premere(w, 'e');
    _solvere(w, 'e');
    _foris_probare(c, w);
    /* ostium propinquum; W TENTA dum E premitur (plan, focus 5) */
    ostii_x      = x0 + 1.0f;
    ostii_z      = z0 + 0.3f;
    o[ZEPHYRUM]  = _ostium(c, "hut-door", ostii_x, ostii_z, 1.5f);
    _ostia_pone(c, w, o, I);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    _premere(w, 'w');
    _pulsus_multi(w, II);
    CREDO_VERUM (w->m.corpus.movetur);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    facies = w->m.corpus.facies;
    /* E cum modificante non intrat */
    _clavis_eventus(w, 'e', VERUM, MOD_IMPERIUM);
    _foris_probare(c, w);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    sprintf(speratum, "%d %d %d", (integer)_millesimae(ostii_x),
        (integer)_millesimae(ostii_z), MD);
    CREDO_VERUM (_campus(w, "ostium", speratum));
    /* claves purgatae, mundus stat, pulsus rari */
    CREDO_FALSUM (w->m.tentae[ZEPHYRUM] || w->m.tentae[I]
        || w->m.tentae[II] || w->m.tentae[III]);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    x1      = w->m.corpus.x;
    z1      = w->m.corpus.z;
    tempus  = w->m.corpus.tempus_motus;
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x1, 0.00001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z1, 0.00001f);
    /* mundus non promovetur: persona in medio passu haeret */
    CREDO_VERUM (w->m.corpus.movetur);
    CREDO_F32_PROXIMUS (w->m.corpus.tempus_motus, tempus, 0.00001f);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    _solvere(w, 'w');
    _solvere(w, 'e');
    /* intus: scaena ostenditur, mundi partes absunt */
    CREDO_VERUM (_textus_pictus(c, w, "The other end of the room."));
    CREDO_VERUM (_textus_pictus(c, w, "hut-door"));
    CREDO_FALSUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    CREDO_FALSUM (_textus_pictus(c, w, "2/2"));
    CREDO_FALSUM (_textus_pictus(c, w, "mushroom man"));
    CREDO_FALSUM (_textus_pictus(c, w, "atkinson"));
    CREDO_VERUM (_nodus(w, "humus", c->piscina) == NIHIL);
    CREDO_VERUM (_nodus(w, "sagitta_prior", c->piscina) == NIHIL);
    CREDO_VERUM (_nodus(w, "sagitta_proxima", c->piscina) == NIHIL);
    CREDO_VERUM (_nodus(w, "ostium", c->piscina) == NIHIL);
    CREDO_NECESSE (_nodus(w, "pictura", c->piscina) != NIHIL);
    CREDO_AEQUALIS_I32 ((i32)_nodus(w, "pictura", c->piscina)
        ->fines.altitudo, GRAPHIS_ALTITUDO);
    im = mundus_latus_imago_fons(w->m.scaenae_titulus, &w->m);
    CREDO_NECESSE (im != NIHIL);
    CREDO_AEQUALIS_I32 (im->latitudo, GRAPHIS_LATITUDO);
    CREDO_AEQUALIS_I32 (im->altitudo, GRAPHIS_ALTITUDO);
    CREDO_VERUM (_imago_picta(c, w));
    CREDO_VERUM (chorda_aequalis_literis(mundus_latus_titulus(&w->m,
        c->piscina), "hut-door"));
    /* W A S D, g, [ ] intus mortuae */
    _premere(w, 'd');
    CREDO_FALSUM (w->m.tentae[MUNDUS_LATUS_DEXTRUM]);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x1, 0.00001f);
    _solvere(w, 'd');
    _premere(w, 'g');
    CREDO_VERUM (_campus(w, "humus", NIHIL));
    _premere(w, ']');
    _premere(w, '[');
    CREDO_VERUM (_campus(w, "persona", NIHIL));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    /* E in scaena bona nihil agit */
    _premere(w, 'e');
    CREDO_VERUM (mundus_latus_intus(&w->m));
    /* res tacta: textus, campus scriptore lateris */
    CREDO_VERUM (_rem_icere(w, ianua, "coat", c->piscina));
    CREDO_VERUM (_campus(w, "tactum", "hut-door/coat"));
    CREDO_VERUM (_textus_pictus(c, w, "Heavy wool"));
    /* exitus inter prospectus */
    CREDO_VERUM (_rem_icere(w, ianua, "turn", c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", "hut-hearth"));
    CREDO_VERUM (_textus_pictus(c, w, "A low room of logs"));
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_rem_icere(w, focus, "turn", c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    /* W iterum pressa intus (clavis adhuc tenta iterat): post
     * exitum non ambulat */
    _premere(w, 'w');
    /* exitus "mundus": foris, ad ostium, facies ut ante */
    CREDO_VERUM (_rem_icere(w, ianua, "door", c->piscina));
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 1.5f);
    CREDO_F32_PROXIMUS (w->m.corpus.facies, facies, 0.0001f);
    /* non ubi intravit (ibi ambulabat) */
    CREDO_VERUM (_distantia(w, x1, z1) > 0.3f);
    /* signum statim redit, quadrum mundi quoque */
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, ZEPHYRUM);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    CREDO_VERUM (_textus_pictus(c, w, MUNDUS_LATUS_SIGNUM));
    CREDO_VERUM (_textus_pictus(c, w, "mushroom man"));
    CREDO_FALSUM (_textus_pictus(c, w, "The other end of the room."));
    CREDO_VERUM (chorda_aequalis_literis(mundus_latus_titulus(&w->m,
        c->piscina), "mushroom man"));
    x1 = w->m.corpus.x;
    z1 = w->m.corpus.z;
    _pulsus_multi(w, X);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x1, 0.00001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z1, 0.00001f);
    CREDO_FALSUM (w->m.corpus.movetur);
    /* foris W iterum ambulat */
    _premere(w, 'w');
    _pulsus_multi(w, III);
    CREDO_VERUM (w->m.corpus.movetur);
    _solvere(w, 'w');
    _pulsus_multi(w, III);
    CREDO_NECESSE (w->m.ostium_propinquum == ZEPHYRUM);
    /* ictus in signo intrat; tactum prius vacatur */
    CREDO_NECESSE (_nodus(w, "ostium", c->piscina) != NIHIL);
    _ictus(w, "ostium", c->piscina);
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_FALSUM (_textus_pictus(c, w, "Heavy wool"));
    /* res sub indice (ianua, modo icta) non manet */
    CREDO_AEQUALIS_I32 (w->m.scaena.super, ZEPHYRUM);
    /* clavibus exit: j usque ad "door", Enter */
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        _premere(w, 'j');
        si (   w->m.scaena.super > ZEPHYRUM
            && chorda_aequalis_literis(ianua->res[w->m.scaena.super
                - I].titulus, "door"))
        {
            frange;
        }
    }
    CREDO_NECESSE (k < VIII);
    _reditus(w);
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 1.5f);
    _finire(w);
}

interior vacuum
_sectio_restitutio (
    CredoContextus* c)
{
          Volumen* v = _volumen(c, "mundus_restitutio", VERUM, II);
            Orbis* w;
    ScaenaFructus  casa = _casa(c);
     MundusOstium  o[II];
        character  campus[LXXX];
              f32  x0;
              f32  z0;
              f32  ostii_x;
              f32  ostii_z;
              f32  extra_x;
              f32  initii_x = 0.0f;
              f32  initii_z = 0.0f;

    CREDO_NECESSE (_casam_addere(c, v));
    CREDO_NECESSE (casa.successus && casa.numerus == II);
    w = _orbem(c, v, LATITUDO_SCAENAE, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0       = w->m.corpus.x;
    z0       = w->m.corpus.z;
    ostii_x  = x0 - 1.1f;
    ostii_z  = z0 + 0.6f;
    /* ostium SECUNDUM propinquum: scaena et punctum eius */
    o[ZEPHYRUM]  = _ostium(c, "longe", x0 + 6.0f, z0, 1.0f);
    o[I]         = _ostium(c, "hut-door", ostii_x, ostii_z, 2.0f);
    _ostia_pone(c, w, o, II);
    CREDO_NECESSE (w->m.ostium_propinquum == I);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    /* vicus restitutus: latus intus incipit, scaena ostenditur */
    CREDO_NECESSE (_iterum_montare(c, w));
    CREDO_VERUM (mundus_latus_intus(&w->m));
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_textus_pictus(c, w, "The other end of the room."));
    CREDO_VERUM (mundus_latus_imago_fons(w->m.scaenae_titulus, &w->m)
        != NIHIL);
    /* quadrum mundi intus non fit */
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_VERUM (w->m.positum);
    CREDO_VERUM (_nodus(w, "humus", c->piscina) == NIHIL);
    CREDO_VERUM (mundus_latus_mora(&w->m)
        == MUNDUS_LATUS_MORA_INSPECTIONIS);
    /* exitus post restitutionem: ad ostium, non ad initium */
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 2.0f);
    CREDO_VERUM (_distantia(w, x0, z0) > 0.5f);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    /* 'ostium' illegibile aut extra locum: initium loci */
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", "ibi 12 x"));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    /* ad ostium redire: a tenta */
    _premere(w, 'a');
    _pulsus_multi(w, VI);
    _solvere(w, 'a');
    _pulsus_multi(w, II);
    CREDO_NECESSE (w->m.ostium_propinquum == I);
    CREDO_VERUM (_distantia(w, x0, z0) > 0.5f);
    /* numeri tres boni cum cauda: illegibile, initium */
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    sprintf(campus, "%d %d 2000 x", (integer)_millesimae(ostii_x),
        (integer)_millesimae(ostii_z));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", campus));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    /* RADIUS campi valet: punctum unitate extra marginem loci -
     * radio 0.5 nulla cella (initium), radio III cella marginis */
    extra_x = w->m.solum->origo_x - 1.0f;
    CREDO_NECESSE (_proxima_distantia(w->m.solum, extra_x, z0, 0.5f)
        < 0.0f);
    CREDO_NECESSE (_proxima_distantia(w->m.solum, extra_x, z0, 3.0f)
        > 0.0f);
    CREDO_NECESSE (_campum_scribe(c, w, "prospectus", "hut-door"));
    sprintf(campus, "%d %d 500", (integer)_millesimae(extra_x),
        (integer)_millesimae(z0));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", campus));
    dispensator_recomponere(w->d);
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    CREDO_NECESSE (_campum_scribe(c, w, "prospectus", "hut-door"));
    sprintf(campus, "%d %d 3000", (integer)_millesimae(extra_x),
        (integer)_millesimae(z0));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", campus));
    dispensator_recomponere(w->d);
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, (f32)_millesimae(extra_x) / 1000.0f,
        (f32)_millesimae(z0) / 1000.0f, 3.0f);
    CREDO_VERUM (_distantia(w, x0, z0) > 3.0f);
    /* radius immanis (campus corruptus): cella proxima tamen, sine
     * mora */
    CREDO_NECESSE (_campum_scribe(c, w, "prospectus", "hut-door"));
    sprintf(campus, "%d %d 2000000000", (integer)_millesimae(ostii_x),
        (integer)_millesimae(ostii_z));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", campus));
    dispensator_recomponere(w->d);
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 2.0f);
    CREDO_NECESSE (w->m.ostium_propinquum == I);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium",
        "999000 999000 1500"));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, z0, 0.0001f);
    /* campus 'ostium' absens: initium */
    _premere(w, 'a');
    _pulsus_multi(w, VI);
    _solvere(w, 'a');
    _pulsus_multi(w, II);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", NIHIL));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_F32_PROXIMUS (w->m.corpus.x, x0, 0.0001f);
    /* locus iterum conditus DUM INTUS (plan, focus 3): intus manet;
     * ostium vetus in loco novo non utile: initium loci novi */
    _premere(w, 'a');
    _pulsus_multi(w, VI);
    _solvere(w, 'a');
    _pulsus_multi(w, II);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (statuae_addere_generis(w->volumen, _ch(c, "clearing"),
        statua_glb_scribere(loci_ficti_locus(c), NIHIL, c->piscina),
        STATUAE_GENUS_LOCUS));
    CREDO_VERUM (mundus_ostia_ponere(w->volumen, _ch(c, "clearing"),
        NIHIL, ZEPHYRUM));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_textus_pictus(c, w, "The other end of the room."));
    CREDO_NECESSE (w->m.solum != NIHIL);
    CREDO_NECESSE (_proxima_distantia(w->m.solum, ostii_x, ostii_z,
        2.0f) < 0.0f);
    CREDO_NECESSE (mundus_solum_initium(w->m.solum,
        MUNDUS_CORPORIS_RADIUS, &initii_x, &initii_z));
    CREDO_VERUM (_rem_icere(w, &casa.prospectus[I], "door",
        c->piscina));
    _foris_probare(c, w);
    CREDO_VERUM (w->m.positum);
    CREDO_F32_PROXIMUS (w->m.corpus.x, initii_x, 0.0001f);
    CREDO_F32_PROXIMUS (w->m.corpus.z, initii_z, 0.0001f);
    CREDO_AEQUALIS_S32 (w->m.ostium_propinquum, -I);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    _finire(w);
}

interior vacuum
_sectio_ignota (
    CredoContextus* c)
{
          Volumen* v = _volumen(c, "mundus_ignota", VERUM, II);
            Orbis* w;
    ScaenaFructus  casa = _casa(c);
     MundusOstium  o[I];
          Eventus  e;
           chorda  sordes;
              f32  x0;
              f32  z0;
              f32  ostii_x;
              f32  ostii_z;

    CREDO_NECESSE (casa.successus && casa.numerus == II);
    w = _orbem(c, v, LATITUDO_SCAENAE, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_NECESSE (w->m.positum);
    x0       = w->m.corpus.x;
    z0       = w->m.corpus.z;
    ostii_x  = x0 + 0.9f;
    ostii_z  = z0 - 0.4f;
    /* scaena absens: intratur tamen; nuntius et monitum */
    o[ZEPHYRUM] = _ostium(c, "cellar", ostii_x, ostii_z, 1.5f);
    _ostia_pone(c, w, o, I);
    _premere(w, 'e');
    _solvere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", "cellar"));
    CREDO_VERUM (_textus_pictus(c, w, "This scene cannot be shown:"));
    CREDO_VERUM (_textus_pictus(c, w, "cellar"));
    CREDO_VERUM (_textus_pictus(c, w, MUNDUS_LATUS_MONITUM));
    CREDO_FALSUM (_imago_picta(c, w));
    CREDO_VERUM (_nodus(w, "humus", c->piscina) == NIHIL);
    /* claves aliae nihil: W, j, Enter, g */
    _premere(w, 'w');
    _solvere(w, 'w');
    _premere(w, 'j');
    _reditus(w);
    _premere(w, 'g');
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "humus", NIHIL));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    /* E cum modificante non exit; ictus dexter non exit */
    _clavis_eventus(w, 'e', VERUM, MOD_IMPERIUM);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    /* E exit, ad ostium */
    _premere(w, 'e');
    _solvere(w, 'e');
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 1.5f);
    CREDO_VERUM (_quadrum(w) != NIHIL);
    /* E maiuscula (Shift) quoque intrat; ictus dexter non exit,
     * sinister ubivis exit */
    _clavis_eventus(w, 'E', VERUM, MOD_SHIFT);
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    _ictus_botonis(w, CC, CCC, MUS_DEXTER);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    _ictus_in(w, CC, CCC);
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 1.5f);
    /* scaena addita dum intus: apparet; E tunc non exit */
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (scaena_addere(w->volumen, _ch(c, "cellar"),
        scaena_scribere(&casa.prospectus[I], c->piscina)));
    /* E ANTE pulsum et compositionem: scaena iam bona iudicatur,
     * non ex statu vetere - non exit */
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = 'e';
    e.datum.clavis.clavis    = (clavis_t)'e';
    e.datum.clavis.runa      = 'e';
    dispensator_tractare(w->d, &e);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
    CREDO_FALSUM (_textus_pictus(c, w, "cannot be shown"));
    CREDO_VERUM (_textus_pictus(c, w, "The other end of the room."));
    _premere(w, 'e');
    _ictus_in(w, II, CD);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    /* remota dum intus: nuntius; E exit (plan, focus 4) */
    CREDO_VERUM (scaena_removere(w->volumen, _ch(c, "cellar")));
    /* pulsus SOLUS (sine compositione) scaenam sequitur */
    CREDO_VERUM (scaena_latus_valet(&w->m.scaena));
    w->hora += MUNDUS_LATUS_MORA_INSPECTIONIS;
    CREDO_VERUM (mundus_latus_pulsare(&w->m));
    CREDO_FALSUM (scaena_latus_valet(&w->m.scaena));
    dispensator_recomponere(w->d);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_textus_pictus(c, w, "This scene cannot be shown:"));
    CREDO_VERUM (_textus_pictus(c, w, MUNDUS_LATUS_MONITUM));
    _premere(w, 'e');
    _foris_probare(c, w);
    _ad_ostium_probare(c, w, ostii_x, ostii_z, 1.5f);
    /* massa quam lector scaenae recusat: nuntius, E exit */
    sordes = _ch(c, "haec non est scaena");
    CREDO_VERUM (scaena_addere(w->volumen, _ch(c, "cellar"), sordes));
    (vacuum)_pulsus(w, MUNDUS_LATUS_MORA_INSPECTIONIS);
    _premere(w, 'e');
    CREDO_NECESSE (mundus_latus_intus(&w->m));
    CREDO_VERUM (_textus_pictus(c, w, "This scene cannot be shown:"));
    CREDO_VERUM (_textus_pictus(c, w, MUNDUS_LATUS_MONITUM));
    _premere(w, 'e');
    _foris_probare(c, w);
    _finire(w);
    /* sine loco, 'prospectus' in statu: intus, E exit, nihil ruit */
    w = _orbem(c, _volumen(c, "mundus_ignota2", FALSUM, I),
        LATITUDO_SCAENAE,
        ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (_campum_scribe(c, w, "prospectus", "hut-door"));
    CREDO_NECESSE (_campum_scribe(c, w, "ostium", "1000 1000 1500"));
    dispensator_recomponere(w->d);
    (vacuum)_pulsus(w, ZEPHYRUM);
    CREDO_VERUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_textus_pictus(c, w, "This scene cannot be shown:"));
    _premere(w, 'e');
    CREDO_FALSUM (mundus_latus_intus(&w->m));
    CREDO_VERUM (_campus(w, "prospectus", NIHIL));
    CREDO_VERUM (_campus(w, "ostium", NIHIL));
    CREDO_FALSUM (w->m.positum);
    CREDO_VERUM (_textus_pictus(c, w, "No place yet"));
    _finire(w);
    /* sine volumine */
    w = _orbem(c, NIHIL, LATITUDO_SCAENAE, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    _premere(w, 'e');
    CREDO_FALSUM (mundus_latus_intus(&w->m));
    CREDO_FALSUM (mundus_latus_intus(NIHIL));
    /* latus dimissum scaenam hospitatam quoque dimittit (piscinae
     * eius) */
    CREDO_VERUM (w->m.scaena_montata);
    CREDO_VERUM (w->m.scaena.piscina_indicis != NIHIL);
    mundus_latus_dimittere(&w->m);
    CREDO_FALSUM (w->m.scaena_montata);
    CREDO_VERUM (w->m.scaena.piscina_indicis == NIHIL);
    CREDO_VERUM (w->m.scaena.piscina_prospectus == NIHIL);
    _finire(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "primum", _sectio_primum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambulat", _sectio_ambulat, NIHIL, NIHIL, NIHIL, NIHIL },
    { "focus", _sectio_focus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "persona", _sectio_persona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "humus", _sectio_humus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuum", _sectio_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "substitutus", _sectio_substitutus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "recusatum", _sectio_recusatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines", _sectio_magnitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ostia", _sectio_ostia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ostia_plura", _sectio_ostia_plura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ostia_margo", _sectio_ostia_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ostia_altitudo", _sectio_ostia_altitudo, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ostia_vacua", _sectio_ostia_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "intrare", _sectio_intrare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "restitutio", _sectio_restitutio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ignota", _sectio_ignota, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_latus", SECTIONES);
}
