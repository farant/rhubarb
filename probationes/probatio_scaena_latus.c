/* probatio_scaena_latus.c - latus scaenae (scaena_latus.h)
 *
 * Latus solum (ut probatio_mundus_latus): repositorium, montatio,
 * dispensator, eventa ficta; volumen temporarium cum casa vera
 * (apps/scaena/loci) per scaena_struere composita.
 *
 * primum: pictura post pulsum, caput, descriptio. tangere: ictus in
 * re textum ostendit; extra res nihil. exitus: transitus cum textu
 * exitus; "mundus" textus solus. super: motus muris rem in capite
 * nominat, non in statu. claves: [ ] et sagittae, j k Enter.
 * vacuum: sine prospectibus, sine volumine, prospectus additi
 * apparent. mutatio: massa substituta, remota, corrupta. status:
 * latus iterum montatum ubi erat. magnitudines: latera angusta,
 * lata, minima.
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
#include "dithering.h"
#include "graphis.h"
#include "scaena.h"
#include "scaena_repositorium.h"
#include "scaena_latus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  CCCLX
#define ALTITUDO  CDL
#define VIA_CASAE "apps/scaena/loci/casa.scaena.stml"

nomen structura {
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                Volumen* volumen;
    ScaenaLatusMontatio  m;
            Dispensator* d;
} Orbis;

interior chorda
_ch (
        CredoContextus* c,
    constans character* textus)
{
    redde chorda_ex_literis(textus, c->piscina);
}

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

/* volumen temporarium; casa in eo si 'cum_casa' */
interior Volumen*
_volumen (
        CredoContextus* c,
    constans character* praefixum,
                   b32  cum_casa)
{
          Volumen* v = volumen_temporarium(c->piscina, praefixum);
    ScaenaFructus  f;
              i32  k;

    si (v && cum_casa)
    {
        f = _casa(c);
        per (k = ZEPHYRUM; f.successus && k < f.numerus; k++)
        {
            (vacuum)scaena_addere(v, f.prospectus[k].titulus,
                scaena_scribere(&f.prospectus[k], c->piscina));
        }
    }
    redde v;
}

interior Orbis*
_orbem_in (
        CredoContextus* c,
                 Orbis* w,
               Volumen* volumen,
                   i32  latitudo,
                   i32  altitudo)
{
    Motus* motus;

    w->volumen = volumen;
    si (   !w->repo
        || !scaena_latus_montare(&w->m, c->piscina, w->intern, volumen,
               w->repo, "s1", latitudo, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, scaena_latus_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior Orbis*
_orbem (
    CredoContextus* c,
           Volumen* volumen,
               i32  latitudo,
               i32  altitudo)
{
    Orbis* w;

    thema_initiare();
    w = (Orbis*)piscina_allocare(c->piscina, magnitudo(Orbis));
    memset(w, ZEPHYRUM, magnitudo(Orbis));
    w->intern  = internamentum_creare(c->piscina);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    redde _orbem_in(c, w, volumen, latitudo, altitudo);
}

interior vacuum
_finire (
    Orbis* w)
{
    si (!w)
    {
        redde;
    }
    scaena_latus_dimittere(&w->m);
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

/* eventus muris in puncto radicis */
interior vacuum
_mus (
              Orbis* w,
    eventus_genus_t  genus,
                s32  x,
                s32  y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.datum.mus.x       = x;
    e.datum.mus.y       = y;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

/* ictus sinister in angulo componentis 'id' */
interior vacuum
_ictus (
                 Orbis* w,
    constans character* id,
               Piscina* p)
{
    Componens* n = _nodus(w, id, p);

    _mus(w, EVENTUS_MUS_DEPRESSUS, (n ? n->fines.x : ZEPHYRUM) + II,
        (n ? n->fines.y : ZEPHYRUM) + II);
}

/* pixelum picturae (x, y) in spatio radicis: ictus aut motus */
interior vacuum
_in_pictura (
              Orbis* w,
    eventus_genus_t  genus,
                s32  x,
                s32  y,
            Piscina* p)
{
    Componens* n = _nodus(w, "pictura", p);

    _mus(w, genus, (n ? n->fines.x : ZEPHYRUM) + x,
        (n ? n->fines.y : ZEPHYRUM) + y);
}

/* pixelum INTERIUS regionis rei 'titulus' (vicini quattuor eiusdem
 * regionis), x < limes; titulus NIHIL = pixelum nullius regionis.
 * FALSUM si nullum */
interior b32
_pixelum (
    constans ScaenaProspectus* p,
           constans character* titulus,
                          s32  limes,
                          s32* x_exitus,
                          s32* y_exitus)
{
    i32 dominus = ZEPHYRUM;
    s32 x;
    s32 y;
    i32 k;

    per (k = ZEPHYRUM; titulus && k < p->rerum; k++)
    {
        si (chorda_aequalis_literis(p->res[k].titulus, titulus))
        {
            dominus = k + I;
        }
    }
    si (titulus && dominus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (y = I; y < GRAPHIS_ALTITUDO - I; y++)
    {
        per (x = I; x < GRAPHIS_LATITUDO - I && x < limes - I; x++)
        {
            constans i8* d = p->domini + y * GRAPHIS_LATITUDO + x;

            si (   d[ZEPHYRUM] == dominus && d[-I] == dominus
                && d[I] == dominus && d[-GRAPHIS_LATITUDO] == dominus
                && d[GRAPHIS_LATITUDO] == dominus)
            {
                *x_exitus = x;
                *y_exitus = y;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* rem prospectus 'p' tangere (ictus) aut super eam movere */
interior b32
_in_re (
                           Orbis* w,
       constans ScaenaProspectus* p,
              constans character* titulus,
                 eventus_genus_t  genus,
                         Piscina* piscina)
{
    s32 x = ZEPHYRUM;
    s32 y = ZEPHYRUM;

    si (!_pixelum(p, titulus, GRAPHIS_LATITUDO, &x, &y))
    {
        redde FALSUM;
    }
    _in_pictura(w, genus, x, y, piscina);
    redde VERUM;
}

interior vacuum
_clavis (
        Orbis* w,
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

interior b32
_pulsus (
    Orbis* w)
{
    b32 mutatum = scaena_latus_pulsare(&w->m);

    dispensator_recomponere(w->d);
    redde mutatum;
}

interior constans Imago*
_quadrum (
    Orbis* w)
{
    redde scaena_latus_imago_fons(w->m.quadri_titulus, &w->m);
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

/* estne textus pictus qui fragmentum continet (aut, si 'totus', ei
 * aequalis est)? */
interior b32
_textus (
        CredoContextus* c,
                 Orbis* w,
    constans character* fragmentum,
                   b32  totus)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus != MANDATUM_TEXTUS)
        {
            perge;
        }
        si (totus ? chorda_aequalis_literis(x->textus, fragmentum)
                  : chorda_continet(x->textus,
                        chorda_ex_literis(fragmentum, c->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

#define _PICTUS(c, w, f)  _textus(c, w, f, FALSUM)
#define _TOTUS(c, w, f)   _textus(c, w, f, VERUM)

/* numerus imaginum pictarum; fines ultimae in *f (si non NIHIL) */
interior i32
_imagines (
    CredoContextus* c,
             Orbis* w,
             Fines* f)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* x;
                  i32  numerus = ZEPHYRUM;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            numerus++;
            si (f)
            {
                *f = x->fines;
            }
        }
    }
    redde numerus;
}

/* estne rectangulum impletum plenum his finibus pictum? */
interior b32
_fundus (
    CredoContextus* c,
             Orbis* w,
               s32  x,
               s32  y,
               s32  latitudo,
               s32  altitudo)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* m;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        m = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   m->genus == MANDATUM_RECTANGULUM && m->impletum
            && m->exemplar == ZEPHYRUM && m->fines.x == x
            && m->fines.y == y && (s32)m->fines.latitudo == latitudo
            && (s32)m->fines.altitudo == altitudo)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lineae textus sub pictura (y >= 'infra'): numerus; *primus = y
 * lineae summae; *extra = lineae quae extra [sinister, dexter)
 * cadunt (littera VI pixela lata) aut infra 'imum' desinunt */
interior i32
_lineae_textus (
    CredoContextus* c,
             Orbis* w,
               s32  infra,
               s32  sinister,
               s32  dexter,
               s32  imum,
               s32* primus,
               i32* extra)
{
              Mandata* md = _reddere(w, c->piscina);
    constans Mandatum* m;
                  i32  numerus = ZEPHYRUM;
                  i32  i;

    *primus  = -I;
    *extra   = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        m = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (m->genus != MANDATUM_TEXTUS || m->fines.y < infra)
        {
            perge;
        }
        si (numerus == ZEPHYRUM || m->fines.y < *primus)
        {
            *primus = m->fines.y;
        }
        numerus++;
        si (   m->fines.x < sinister
            || m->fines.x + (s32)m->textus.mensura * VI > dexter
            || m->fines.y + VIII > imum)
        {
            (*extra)++;
        }
    }
    redde numerus;
}

/* quadrum = indices prospectus per palettam, pars sinistra summa */
interior b32
_quadrum_est (
               constans Imago* im,
    constans ScaenaProspectus* p)
{
    i32 x;
    i32 y;

    si (!im || !im->pixela)
    {
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < im->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < im->latitudo; x++)
        {
            constans i8* q = im->pixela + (y * im->latitudo + x) * IV;
                     i8  k = p->indices[y * GRAPHIS_LATITUDO + x];

            si (   q[ZEPHYRUM] != AQUINAS_PALETTE[k][ZEPHYRUM]
                || q[I] != AQUINAS_PALETTE[k][I]
                || q[II] != AQUINAS_PALETTE[k][II] || q[III] != 255)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_primum (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_primum", VERUM),
                LATITUDO, ALTITUDO);
    ScaenaFructus  f = _casa(c);
   constans Imago* im;
        Componens* pictura;
            Fines  fines_imaginis;
              s32  primus  = ZEPHYRUM;
              i32  extra   = ZEPHYRUM;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    (vacuum)_pulsus(w);
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    CREDO_AEQUALIS_I32 (im->latitudo, GRAPHIS_LATITUDO);
    CREDO_AEQUALIS_I32 (im->altitudo, GRAPHIS_ALTITUDO);
    CREDO_VERUM (_quadrum_est(im, &f.prospectus[ZEPHYRUM]));
    CREDO_FALSUM (_quadrum_est(im, &f.prospectus[I]));
    CREDO_VERUM (scaena_latus_imago_fons(_ch(c, "alia"), &w->m)
        == NIHIL);
    /* pictura sub capite, latus totum latum */
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    CREDO_AEQUALIS_S32 (pictura->fines.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (pictura->fines.y, XX);
    CREDO_AEQUALIS_I32 ((i32)pictura->fines.latitudo,
        GRAPHIS_LATITUDO);
    CREDO_AEQUALIS_I32 ((i32)pictura->fines.altitudo,
        GRAPHIS_ALTITUDO);
    CREDO_AEQUALIS_I32 (_imagines(c, w, &fines_imaginis), I);
    CREDO_AEQUALIS_S32 (fines_imaginis.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (fines_imaginis.y, XX);
    /* caput et descriptio; nulla res tacta, nulla sub indice */
    CREDO_VERUM (_PICTUS(c, w, "1/2"));
    CREDO_VERUM (_TOTUS(c, w, "hut-hearth"));
    CREDO_VERUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_VERUM (_PICTUS(c, w, "is still lit."));
    CREDO_FALSUM (_PICTUS(c, w, "The stones are black"));
    CREDO_FALSUM (_TOTUS(c, w, "hearth"));
    CREDO_FALSUM (_PICTUS(c, w, "The other end"));
    CREDO_VERUM (chorda_aequalis_literis(scaena_latus_titulus(&w->m,
        c->piscina), "hut-hearth"));
    /* fundus planus intra marginem (cella una): textura foci solum
     * in margine */
    CREDO_VERUM (_fundus(c, w, VI, VIII, LATITUDO - XII,
        ALTITUDO - XVI));
    /* textus: linea una vacua sub pictura (XX + CCXL + X), duae
     * cellae a fundi lateribus (VI + XII), intra marginem imum */
    CREDO_VERUM (_lineae_textus(c, w, CCLX, XVIII, LATITUDO - XVIII,
        ALTITUDO - VIII, &primus, &extra) >= III);
    CREDO_AEQUALIS_S32 (primus, CCLXX);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_VERUM (_in_re(w, &f.prospectus[ZEPHYRUM], "hearth",
        EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM (_lineae_textus(c, w, CCLX, XVIII, LATITUDO - XVIII,
        ALTITUDO - VIII, &primus, &extra) >= V);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/hearth"));
    _clavis(w, ']');
    _clavis(w, '[');
    /* pulsus rari; pulsus sine mutatione nihil mutat */
    CREDO_VERUM (scaena_latus_mora(&w->m)
        == SCAENA_LATUS_MORA_INSPECTIONIS);
    CREDO_FALSUM (_pulsus(w));
    _finire(w);
}

interior vacuum
_sectio_tangere (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_tangere", VERUM),
                LATITUDO, ALTITUDO);
    ScaenaFructus  f = _casa(c);
 ScaenaProspectus* focus;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    focus = &f.prospectus[ZEPHYRUM];
    (vacuum)_pulsus(w);
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_VERUM (_in_re(w, focus, "hearth", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/hearth"));
    CREDO_VERUM (_PICTUS(c, w, "The stones are black"));
    CREDO_VERUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_VERUM (_TOTUS(c, w, "hut-hearth"));
    /* res alia: textus substituitur */
    CREDO_VERUM (_in_re(w, focus, "book", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/book"));
    CREDO_VERUM (_PICTUS(c, w, "A ledger, ruled by hand"));
    CREDO_FALSUM (_PICTUS(c, w, "The stones are black"));
    /* ictus extra res: nihil mutatur */
    CREDO_VERUM (_in_re(w, focus, NIHIL, EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/book"));
    CREDO_VERUM (_PICTUS(c, w, "A ledger, ruled by hand"));
    /* ictus in textu sub pictura: nihil */
    _mus(w, EVENTUS_MUS_DEPRESSUS, C, CCC);
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/book"));
    CREDO_VERUM (_campus(w, "prospectus", NIHIL));
    _finire(w);
}

interior vacuum
_sectio_exitus (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_exitus", VERUM),
                LATITUDO, ALTITUDO);
    ScaenaFructus  f = _casa(c);
 ScaenaProspectus* focus;
 ScaenaProspectus* ostium;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    focus   = &f.prospectus[ZEPHYRUM];
    ostium  = &f.prospectus[I];
    (vacuum)_pulsus(w);
    /* turn: in prospectum alterum, textus exitus manet */
    CREDO_VERUM (_in_re(w, focus, "turn", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/turn"));
    CREDO_VERUM (_PICTUS(c, w, "2/2"));
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    CREDO_VERUM (_PICTUS(c, w, "The other end of the room"));
    CREDO_VERUM (_PICTUS(c, w, "You turn your back to the fire."));
    CREDO_FALSUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_VERUM (_quadrum_est(_quadrum(w), ostium));
    CREDO_VERUM (chorda_aequalis_literis(scaena_latus_titulus(&w->m,
        c->piscina), "hut-door"));
    /* res sub indice post transitum nulla */
    CREDO_AEQUALIS_I32 (w->m.super, ZEPHYRUM);
    /* door: exitus "mundus" - textus solus, prospectus manet */
    CREDO_VERUM (_in_re(w, ostium, "door", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_campus(w, "tactum", "hut-door/door"));
    CREDO_VERUM (_PICTUS(c, w, "Planks, braced and pegged."));
    CREDO_FALSUM (_PICTUS(c, w, "You turn your back"));
    CREDO_VERUM (_quadrum_est(_quadrum(w), ostium));
    /* turn iterum: redit */
    CREDO_VERUM (_in_re(w, ostium, "turn", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", "hut-hearth"));
    CREDO_VERUM (_PICTUS(c, w, "You turn back to the fire."));
    CREDO_VERUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_VERUM (_quadrum_est(_quadrum(w), focus));
    _finire(w);
}

interior vacuum
_sectio_super (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_super", VERUM),
                LATITUDO, ALTITUDO);
    ScaenaFructus  f = _casa(c);
 ScaenaProspectus* focus;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    focus = &f.prospectus[ZEPHYRUM];
    (vacuum)_pulsus(w);
    CREDO_VERUM (_in_re(w, focus, "window", EVENTUS_MUS_MOTUS,
        c->piscina));
    CREDO_VERUM (_TOTUS(c, w, "window"));
    CREDO_AEQUALIS_I32 (w->m.super, II);
    /* motus rem non tangit et in statu nihil ponit */
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_VERUM (_campus(w, "super", NIHIL));
    CREDO_FALSUM (_PICTUS(c, w, "Through the glass"));
    CREDO_VERUM (_in_re(w, focus, "hearth", EVENTUS_MUS_MOTUS,
        c->piscina));
    CREDO_VERUM (_TOTUS(c, w, "hearth"));
    CREDO_FALSUM (_TOTUS(c, w, "window"));
    /* extra res: nulla */
    CREDO_VERUM (_in_re(w, focus, NIHIL, EVENTUS_MUS_MOTUS,
        c->piscina));
    CREDO_FALSUM (_TOTUS(c, w, "hearth"));
    CREDO_AEQUALIS_I32 (w->m.super, ZEPHYRUM);
    /* ex pictura in textum: nulla */
    CREDO_VERUM (_in_re(w, focus, "jars", EVENTUS_MUS_MOTUS,
        c->piscina));
    CREDO_VERUM (_TOTUS(c, w, "jars"));
    _mus(w, EVENTUS_MUS_MOTUS, C, CCC);
    CREDO_FALSUM (_TOTUS(c, w, "jars"));
    CREDO_AEQUALIS_I32 (w->m.super, ZEPHYRUM);
    _finire(w);
}

interior vacuum
_sectio_claves (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_claves", VERUM),
                LATITUDO, ALTITUDO);
    ScaenaFructus  f = _casa(c);
 ScaenaProspectus* focus;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    focus = &f.prospectus[ZEPHYRUM];
    (vacuum)_pulsus(w);
    /* ] et [: circulo; res tacta vacatur */
    CREDO_VERUM (_in_re(w, focus, "hearth", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    _clavis(w, ']');
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    CREDO_FALSUM (_PICTUS(c, w, "The stones are black"));
    _clavis(w, ']');
    CREDO_VERUM (_campus(w, "prospectus", "hut-hearth"));
    _clavis(w, '[');
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    /* sagittae capitis idem agunt */
    _ictus(w, "sagitta_prior", c->piscina);
    CREDO_VERUM (_campus(w, "prospectus", "hut-hearth"));
    _ictus(w, "sagitta_proxima", c->piscina);
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    _ictus(w, "sagitta_proxima", c->piscina);
    CREDO_VERUM (_campus(w, "prospectus", "hut-hearth"));
    /* Enter sine re electa: nihil */
    _reditus(w);
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    /* j k: res eligunt, circulo; titulus in capite */
    _clavis(w, 'j');
    CREDO_VERUM (_TOTUS(c, w, "hearth"));
    _clavis(w, 'j');
    CREDO_VERUM (_TOTUS(c, w, "window"));
    CREDO_FALSUM (_TOTUS(c, w, "hearth"));
    _clavis(w, 'k');
    CREDO_VERUM (_TOTUS(c, w, "hearth"));
    _clavis(w, 'k');
    CREDO_VERUM (_TOTUS(c, w, "turn"));
    _clavis(w, 'j');
    CREDO_VERUM (_TOTUS(c, w, "hearth"));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    /* Enter electam tangit */
    _reditus(w);
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/hearth"));
    CREDO_VERUM (_PICTUS(c, w, "The stones are black"));
    /* k ab initio: res ultima; Enter exitum sequitur */
    _clavis(w, 'k');
    CREDO_VERUM (_TOTUS(c, w, "turn"));
    _reditus(w);
    CREDO_VERUM (_campus(w, "prospectus", "hut-door"));
    CREDO_VERUM (_PICTUS(c, w, "You turn your back to the fire."));
    CREDO_AEQUALIS_I32 (w->m.super, ZEPHYRUM);
    _clavis(w, 'k');
    CREDO_VERUM (_TOTUS(c, w, "turn"));
    _clavis(w, 'j');
    CREDO_VERUM (_TOTUS(c, w, "door"));
    _finire(w);
}

interior vacuum
_sectio_vacuum (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_vacuum", FALSUM),
                LATITUDO, ALTITUDO);
            Orbis* sine;
    ScaenaFructus  f = _casa(c);

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    (vacuum)_pulsus(w);
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_AEQUALIS_I32 (_imagines(c, w, NIHIL), ZEPHYRUM);
    CREDO_VERUM (_PICTUS(c, w, "No scenes yet"));
    CREDO_VERUM (_PICTUS(c, w, "bin/scaena condere"));
    CREDO_VERUM (_PICTUS(c, w, "0/0"));
    CREDO_AEQUALIS_I32 (scaena_latus_titulus(&w->m,
        c->piscina).mensura, ZEPHYRUM);
    /* claves et ictus innocui */
    _clavis(w, ']');
    _clavis(w, '[');
    _clavis(w, 'j');
    _clavis(w, 'k');
    _reditus(w);
    _ictus(w, "sagitta_proxima", c->piscina);
    _in_pictura(w, EVENTUS_MUS_DEPRESSUS, C, C, c->piscina);
    _in_pictura(w, EVENTUS_MUS_MOTUS, C, C, c->piscina);
    CREDO_VERUM (_campus(w, "prospectus", NIHIL));
    CREDO_VERUM (_campus(w, "tactum", NIHIL));
    CREDO_FALSUM (_pulsus(w));
    /* prospectus ab instrumento additi: pulsus eos invenit */
    CREDO_VERUM (scaena_addere(w->volumen,
        f.prospectus[I].titulus,
        scaena_scribere(&f.prospectus[I], c->piscina)));
    CREDO_VERUM (_pulsus(w));
    CREDO_VERUM (_quadrum_est(_quadrum(w), &f.prospectus[I]));
    CREDO_VERUM (_PICTUS(c, w, "1/1"));
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    CREDO_FALSUM (_PICTUS(c, w, "No scenes yet"));
    /* exitus in prospectum qui in volumine non est: textus solus */
    CREDO_VERUM (_in_re(w, &f.prospectus[I], "turn",
        EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM (_campus(w, "prospectus", NIHIL));
    CREDO_VERUM (_PICTUS(c, w, "You turn back to the fire."));
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    /* prospectus unus: [ ] nihil mutant */
    _clavis(w, ']');
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    _finire(w);
    /* sine volumine */
    sine = _orbem(c, NIHIL, LATITUDO, ALTITUDO);
    CREDO_NECESSE (sine != NIHIL);
    CREDO_FALSUM (_pulsus(sine));
    CREDO_VERUM (scaena_latus_mora(&sine->m) == -I);
    CREDO_VERUM (_PICTUS(c, sine, "No scenes yet"));
    CREDO_VERUM (_quadrum(sine) == NIHIL);
    _finire(sine);
}

interior vacuum
_sectio_mutatio (
    CredoContextus* c)
{
             Orbis* w = _orbem(c, _volumen(c, "scaena_mutatio", VERUM),
                 LATITUDO, ALTITUDO);
     ScaenaFructus f = _casa(c);
  ScaenaProspectus novus;

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    (vacuum)_pulsus(w);
    CREDO_VERUM (_in_re(w, &f.prospectus[ZEPHYRUM], "hearth",
        EVENTUS_MUS_DEPRESSUS, c->piscina));
    /* massa substituta (bin/scaena condere dum latus apertum):
     * descriptio et textus rei novi sine restitutione */
    novus             = f.prospectus[ZEPHYRUM];
    novus.descriptio  = _ch(c, "A room rebuilt while you watched.");
    novus.res         = (ScaenaRes*)piscina_allocare(c->piscina,
        (memoriae_index)novus.rerum * magnitudo(ScaenaRes));
    memcpy(novus.res, f.prospectus[ZEPHYRUM].res,
        (memoriae_index)novus.rerum * magnitudo(ScaenaRes));
    novus.res[ZEPHYRUM].textus = _ch(c, "Cold ashes now.");
    CREDO_VERUM (scaena_addere(w->volumen, novus.titulus,
        scaena_scribere(&novus, c->piscina)));
    CREDO_VERUM (_pulsus(w));
    CREDO_VERUM (_PICTUS(c, w, "A room rebuilt while you watched."));
    CREDO_FALSUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_VERUM (_PICTUS(c, w, "Cold ashes now."));
    CREDO_FALSUM (_PICTUS(c, w, "The stones are black"));
    CREDO_VERUM (_PICTUS(c, w, "1/2"));
    CREDO_FALSUM (_pulsus(w));
    /* massa quam lector recusat: nuntius, sagittae tamen valent */
    CREDO_VERUM (scaena_addere(w->volumen, _ch(c, "hut-hearth"),
        _ch(c, "non est massa scaenae")));
    CREDO_VERUM (_pulsus(w));
    CREDO_VERUM (_quadrum(w) == NIHIL);
    CREDO_AEQUALIS_I32 (_imagines(c, w, NIHIL), ZEPHYRUM);
    CREDO_VERUM (_PICTUS(c, w, "This scene cannot be shown"));
    CREDO_VERUM (_TOTUS(c, w, "hut-hearth"));
    _in_pictura(w, EVENTUS_MUS_DEPRESSUS, C, C, c->piscina);
    _in_pictura(w, EVENTUS_MUS_MOTUS, C, C, c->piscina);
    _clavis(w, 'j');
    _reditus(w);
    _clavis(w, ']');
    CREDO_VERUM (_TOTUS(c, w, "hut-door"));
    CREDO_VERUM (_quadrum_est(_quadrum(w), &f.prospectus[I]));
    CREDO_FALSUM (_PICTUS(c, w, "This scene cannot be shown"));
    /* prospectus ostensus remotus: primus qui manet */
    CREDO_VERUM (scaena_removere(w->volumen, _ch(c, "hut-door")));
    CREDO_VERUM (_pulsus(w));
    CREDO_VERUM (_PICTUS(c, w, "1/1"));
    CREDO_VERUM (_TOTUS(c, w, "hut-hearth"));
    CREDO_VERUM (_PICTUS(c, w, "This scene cannot be shown"));
    /* omnes remoti */
    CREDO_VERUM (scaena_removere(w->volumen, _ch(c, "hut-hearth")));
    CREDO_VERUM (_pulsus(w));
    CREDO_VERUM (_PICTUS(c, w, "No scenes yet"));
    CREDO_FALSUM (_PICTUS(c, w, "This scene cannot be shown"));
    _finire(w);
}

interior vacuum
_sectio_status (
    CredoContextus* c)
{
            Orbis* w = _orbem(c, _volumen(c, "scaena_status", VERUM),
                LATITUDO, ALTITUDO);
            Orbis* alter;
    ScaenaFructus  f = _casa(c);

    CREDO_NECESSE (w != NIHIL);
    CREDO_NECESSE (f.successus && f.numerus == II);
    (vacuum)_pulsus(w);
    CREDO_VERUM (_in_re(w, &f.prospectus[ZEPHYRUM], "turn",
        EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM (_in_re(w, &f.prospectus[I], "coat",
        EVENTUS_MUS_MOTUS, c->piscina));
    CREDO_VERUM (_TOTUS(c, w, "coat"));
    scaena_latus_dimittere(&w->m);
    /* latus iterum montatum super repositorium idem: prospectus et
     * res tacta manent, res sub indice non */
    alter = (Orbis*)piscina_allocare(c->piscina, magnitudo(Orbis));
    memset(alter, ZEPHYRUM, magnitudo(Orbis));
    alter->intern  = w->intern;
    alter->repo    = w->repo;
    CREDO_NECESSE (_orbem_in(c, alter, w->volumen, LATITUDO, ALTITUDO)
        != NIHIL);
    (vacuum)_pulsus(alter);
    CREDO_VERUM (_TOTUS(c, alter, "hut-door"));
    CREDO_VERUM (_PICTUS(c, alter, "You turn your back to the fire."));
    CREDO_VERUM (_quadrum_est(_quadrum(alter), &f.prospectus[I]));
    CREDO_FALSUM (_TOTUS(c, alter, "coat"));
    /* status qui prospectum ignotum nominat: primus */
    _clavis(alter, ']');
    CREDO_VERUM (_campus(alter, "prospectus", "hut-hearth"));
    CREDO_VERUM (scaena_removere(alter->volumen,
        _ch(c, "hut-hearth")));
    CREDO_VERUM (_pulsus(alter));
    CREDO_VERUM (_TOTUS(c, alter, "hut-door"));
    CREDO_VERUM (_PICTUS(c, alter, "1/1"));
    _finire(alter);
}

interior vacuum
_sectio_magnitudines (
    CredoContextus* c)
{
            Orbis* w;
    ScaenaFructus  f = _casa(c);
   constans Imago* im;
        Componens* pictura;
            Fines  fi;
              s32  x       = ZEPHYRUM;
              s32  y       = ZEPHYRUM;
              s32  primus  = ZEPHYRUM;
              i32  extra   = ZEPHYRUM;

    CREDO_NECESSE (f.successus && f.numerus == II);
    /* angustum: pictura secta, nihil extra latus */
    w = _orbem(c, _volumen(c, "scaena_angustum", VERUM), CC, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w);
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    CREDO_AEQUALIS_I32 (im->latitudo, CC);
    CREDO_AEQUALIS_I32 (im->altitudo, GRAPHIS_ALTITUDO);
    CREDO_VERUM (_quadrum_est(im, &f.prospectus[ZEPHYRUM]));
    CREDO_AEQUALIS_I32 (_imagines(c, w, &fi), I);
    CREDO_AEQUALIS_S32 (fi.x, ZEPHYRUM);
    CREDO_VERUM (fi.x + (s32)fi.latitudo <= CC);
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    CREDO_AEQUALIS_I32 ((i32)pictura->fines.latitudo, CC);
    /* res in parte visa tangitur loco suo */
    CREDO_NECESSE (_pixelum(&f.prospectus[ZEPHYRUM], "hearth", CC, &x,
        &y));
    _in_pictura(w, EVENTUS_MUS_DEPRESSUS, x, y, c->piscina);
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/hearth"));
    _finire(w);
    /* latum: pictura in medio; ictus loco translato */
    w = _orbem(c, _volumen(c, "scaena_latum", VERUM), D, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w);
    pictura = _nodus(w, "pictura", c->piscina);
    CREDO_NECESSE (pictura != NIHIL);
    CREDO_AEQUALIS_S32 (pictura->fines.x, LXX);
    CREDO_AEQUALIS_I32 ((i32)pictura->fines.latitudo,
        GRAPHIS_LATITUDO);
    CREDO_AEQUALIS_I32 (_imagines(c, w, &fi), I);
    CREDO_AEQUALIS_S32 (fi.x, LXX);
    CREDO_VERUM (_in_re(w, &f.prospectus[ZEPHYRUM], "window",
        EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM (_campus(w, "tactum", "hut-hearth/window"));
    /* idem punctum sine translatione rem aliam aut nullam tangit */
    CREDO_NECESSE (_pixelum(&f.prospectus[ZEPHYRUM], "book",
        GRAPHIS_LATITUDO, &x, &y));
    _mus(w, EVENTUS_MUS_DEPRESSUS, x, y + XX);
    CREDO_FALSUM (_campus(w, "tactum", "hut-hearth/book"));
    _finire(w);
    /* humile: pictura secta deorsum, textus nullus */
    w = _orbem(c, _volumen(c, "scaena_humile", VERUM), LATITUDO, C);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w);
    im = _quadrum(w);
    CREDO_NECESSE (im != NIHIL);
    CREDO_AEQUALIS_I32 (im->altitudo, C - XX);
    CREDO_VERUM (_quadrum_est(im, &f.prospectus[ZEPHYRUM]));
    CREDO_FALSUM (_PICTUS(c, w, "A low room of logs"));
    _finire(w);
    /* textus qui non capitur: lineae extra latus non pinguntur */
    w = _orbem(c, _volumen(c, "scaena_breve", VERUM), LATITUDO,
        CCXC);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w);
    CREDO_VERUM (_PICTUS(c, w, "A low room of logs"));
    CREDO_FALSUM (_PICTUS(c, w, "is still lit."));
    /* linea una capitur (CCLXX .. CCLXXVIII); proxima in marginem
     * imum (CCLXXXII) caderet */
    CREDO_AEQUALIS_I32 (_lineae_textus(c, w, CCLX, XVIII,
        LATITUDO - XVIII, CCXC - VIII, &primus, &extra), I);
    CREDO_AEQUALIS_I32 (extra, ZEPHYRUM);
    _finire(w);
    /* minima: sine ruina */
    w = _orbem(c, _volumen(c, "scaena_minimum", VERUM), V, V);
    CREDO_NECESSE (w != NIHIL);
    (vacuum)_pulsus(w);
    _clavis(w, ']');
    _clavis(w, 'j');
    _reditus(w);
    _mus(w, EVENTUS_MUS_DEPRESSUS, II, II);
    (vacuum)_imagines(c, w, NIHIL);
    _finire(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "primum", _sectio_primum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tangere", _sectio_tangere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exitus", _sectio_exitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "super", _sectio_super, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuum", _sectio_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mutatio", _sectio_mutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "status", _sectio_status, NIHIL, NIHIL, NIHIL, NIHIL },
    { "magnitudines", _sectio_magnitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scaena_latus", SECTIONES);
}
