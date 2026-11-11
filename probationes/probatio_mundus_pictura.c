/* probatio_mundus_pictura.c - humus mundi (mundus_pictura.h)
 *
 * In loco ficto (mundus_locus_fictus.h). stabilis: copia pixelo lapsa
 * in pixelis communibus eadem. pigra: regio semel fit, cum visus eam
 * attingit. extra: extra locum fundus, nulla regio. profunditas:
 * eadem ac proiectio. margo: rectangulum partim extra locum. sutura:
 * ora inter regiones (numerus impressus, non assertus). vacua.
 *
 * Quadrum (corpora super humum): forma ex statua. medium: corpus
 * sequens in medio. occultum: corpus post tectum non apparet. ordo:
 * ordo corporum nihil mutat. extra, quies, mala.
 */
#include "latina.h"
#include "piscina.h"
#include "thema.h"
#include "dithering.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_pictura.h"
#include "filum.h"
#include "statua_glb.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "mundus_pictura.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define FUNDUS  0x55
#define LATUS   C

nomen structura {
    MundusHumus* humus;
         Statua* locus;
            b32  activi[XVI];
} Scaena;

interior Scaena*
_scaena (
    CredoContextus* c)
{
    Scaena* s = (Scaena*)loci_ficti_memoria(c, magnitudo(Scaena));

    thema_initiare();
    dithering_praeparare_omnes(s->activi);
    s->locus = loci_ficti_locus(c);
    s->humus = mundus_humus_creare(s->locus, s->activi, c->piscina);
    CREDO_VERUM (s->humus != NIHIL);
    redde s;
}

/* pixelum mundi (integrum) puncti mundi */
interior vacuum
_pixelum (
    f32  x,
    f32  y,
    f32  z,
    s32* px,
    s32* py,
    f32* profunditas)
{
    StatuaScaenaVisus visus      = mundus_visus(ZEPHYRUM, ZEPHYRUM);
                  f32 fractum_x  = 0.0f;
                  f32 fractum_y  = 0.0f;

    statua_scaena_proicere(&visus, stereometria_vector(x, y, z),
        &fractum_x,
        &fractum_y, profunditas);
    *px = (s32)floor((f64)fractum_x);
    *py = (s32)floor((f64)fractum_y);
}

interior i8*
_octeti (
    CredoContextus* c,
               i32  numerus)
{
    redde (i8*)loci_ficti_memoria(c, numerus);
}

interior vacuum
_sectio_stabilis (
    CredoContextus* c)
{
    Scaena* s            = _scaena(c);
        i8* a            = _octeti(c, LATUS * LATUS);
        i8* b            = _octeti(c, LATUS * LATUS);
       f32  profunditas  = 0.0f;
       s32  px;
       s32  py;
       i32  x;
       i32  y;
       i32  diversa = ZEPHYRUM;
       i32  colores[XVI];
       i32  varii = ZEPHYRUM;

    /* circa medium soli (4, 0, 6) */
    _pixelum(4.0f, 0.0f, 6.0f, &px, &py, &profunditas);
    CREDO_VERUM (mundus_humus_copiare(s->humus, px - L, py - L, LATUS,
        LATUS, FUNDUS, a, NIHIL));
    CREDO_VERUM (mundus_humus_copiare(s->humus, px - L + I, py - L
        - III,
        LATUS, LATUS, FUNDUS, b, NIHIL));
    /* b(x, y) = a(x + 1, y - 3) */
    per (y = III; y < LATUS; y++)
    {
        per (x = ZEPHYRUM; x + I < LATUS; x++)
        {
            si (b[y * LATUS + x] != a[(y - III) * LATUS + x + I])
            {
                diversa++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (diversa, ZEPHYRUM);
    /* probatio non vacua: pictura redacta est (colores plures quam
     * unus), omnes indices palettae */
    memset(colores, ZEPHYRUM, magnitudo(colores));
    per (x = ZEPHYRUM; x < LATUS * LATUS; x++)
    {
        si (a[x] < XVI)
        {
            colores[a[x]]++;
        }
    }
    per (x = ZEPHYRUM; x < XVI; x++)
    {
        varii += colores[x] ? I : ZEPHYRUM;
    }
    CREDO_VERUM (varii >= II);
}

interior vacuum
_sectio_pigra (
    CredoContextus* c)
{
    Scaena* s            = _scaena(c);
        i8* a            = _octeti(c, XVI * XVI);
       f32  profunditas  = 0.0f;
       s32  px;
       s32  py;
       s32  rx;
       s32  ry;
       i32  post_unam;
       i32  post_angulum;

    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), ZEPHYRUM);
    _pixelum(4.0f, 0.0f, 6.0f, &px, &py, &profunditas);
    /* angulus regionis in qua punctum iacet */
    rx = px - ((px % MUNDUS_REGIO) + MUNDUS_REGIO) % MUNDUS_REGIO;
    ry = py - ((py % MUNDUS_REGIO) + MUNDUS_REGIO) % MUNDUS_REGIO;
    /* intra regionem unam */
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + C, ry + C, XVI,
        XVI, FUNDUS, a, NIHIL));
    post_unam = mundus_humus_regiones(s->humus);
    CREDO_AEQUALIS_I32 (post_unam, I);
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + XX, ry + CC, XVI,
        XVI, FUNDUS, a, NIHIL));
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), I);
    /* trans angulum inferiorem dextrum: regiones novae (usque ad
     * tres; quae extra fines loci sunt non fiunt) */
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + MUNDUS_REGIO
        - VIII,
        ry + MUNDUS_REGIO - VIII, XVI, XVI, FUNDUS, a, NIHIL));
    post_angulum = mundus_humus_regiones(s->humus);
    CREDO_VERUM (post_angulum >= II);
    CREDO_VERUM (post_angulum <= IV);
    /* iterum: nulla nova */
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + MUNDUS_REGIO
        - VIII,
        ry + MUNDUS_REGIO - VIII, XVI, XVI, FUNDUS, a, NIHIL));
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + C, ry + C, XVI,
        XVI, FUNDUS, a, NIHIL));
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), post_angulum);
}

interior vacuum
_sectio_extra (
    CredoContextus* c)
{
    Scaena* s = _scaena(c);
        i8* a = _octeti(c, XVI * XVI);
       f32* p = (f32*)loci_ficti_memoria(c, XVI * XVI * magnitudo(f32));
       i32  k;

    CREDO_VERUM (mundus_humus_copiare(s->humus, 1000000, -1000000, XVI,
        XVI, FUNDUS, a, p));
    per (k = ZEPHYRUM; k < XVI * XVI; k++)
    {
        si (a[k] != FUNDUS || !(p[k] > 1.0e29f))
        {
            frange;
        }
    }
    CREDO_AEQUALIS_I32 (k, XVI * XVI);
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), ZEPHYRUM);
    /* origo immanis: nec ruit nec regionem facit */
    CREDO_VERUM (mundus_humus_copiare(s->humus, 0x7FFFFFF0, 0x7FFFFFF0,
        XVI, XVI, FUNDUS, a, p));
    CREDO_VERUM (mundus_humus_copiare(s->humus, -0x7FFFFFF0, 0, XVI,
        XVI,
        FUNDUS, a, p));
    CREDO_AEQUALIS_I32 (a[ZEPHYRUM], FUNDUS);
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), ZEPHYRUM);
    /* proxime extra fines loci (pixelum mundi x minimum loci est 0,
     * ad angulum (0, 0, 0)): regiones vicinae non fiunt */
    {
        f32 profunditas = 0.0f;
        s32 px;
        s32 py;

        _pixelum(0.0f, 0.0f, 0.0f, &px, &py, &profunditas);
        CREDO_VERUM (mundus_humus_copiare(s->humus,
            px - III * MUNDUS_REGIO, py, XVI, XVI, FUNDUS, a, p));
        CREDO_VERUM (mundus_humus_copiare(s->humus, px,
            py - III * MUNDUS_REGIO, XVI, XVI, FUNDUS, a, p));
        per (k = ZEPHYRUM; k < XVI * XVI; k++)
        {
            si (a[k] != FUNDUS)
            {
                frange;
            }
        }
        CREDO_AEQUALIS_I32 (k, XVI * XVI);
        CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), ZEPHYRUM);
    }
}

interior vacuum
_sectio_profunditas (
    CredoContextus* c)
{
    Scaena* s        = _scaena(c);
        i8  index    = FUNDUS;
       f32  visa     = 0.0f;
       f32  sperata  = 0.0f;
       s32  px;
       s32  py;

    /* solum (y 0) et superius (y 4); pixelum unum profunditatem
     * circa 0.04 mutat, ergo tolerantia 0.1; strata ipsa multo plus
     * differunt */
    _pixelum(4.0f, 0.0f, 6.0f, &px, &py, &sperata);
    CREDO_VERUM (mundus_humus_copiare(s->humus, px, py, I, I, FUNDUS,
        &index, &visa));
    CREDO_VERUM (index < XVI);
    CREDO_F32_PROXIMUS (visa, sperata, 0.1f);
    _pixelum(10.0f, 4.0f, 6.0f, &px, &py, &sperata);
    index = FUNDUS;
    CREDO_VERUM (mundus_humus_copiare(s->humus, px, py, I, I, FUNDUS,
        &index, &visa));
    CREDO_VERUM (index < XVI);
    CREDO_F32_PROXIMUS (visa, sperata, 0.1f);
}

interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    Scaena* s = _scaena(c);
        i8* a = _octeti(c, CC * CC);
       f32* p = (f32*)loci_ficti_memoria(c, CC * CC * magnitudo(f32));
       f32  profunditas = 0.0f;
       s32  px;
       s32  py;
       i32  k;
       i32  tecta  = ZEPHYRUM;
       i32  vacua  = ZEPHYRUM;
       i32  mala   = ZEPHYRUM;

    /* angulus loci (0, 0, 0) in medio: pars intra, pars extra */
    _pixelum(0.0f, 0.0f, 0.0f, &px, &py, &profunditas);
    CREDO_VERUM (mundus_humus_copiare(s->humus, px - C, py - C, CC, CC,
        FUNDUS, a, p));
    per (k = ZEPHYRUM; k < CC * CC; k++)
    {
        si (a[k] == FUNDUS && p[k] > 1.0e29f)
        {
            vacua++;
        }
        alioquin si (a[k] < XVI && p[k] < 1.0e29f)
        {
            tecta++;
        }
        alioquin
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    CREDO_VERUM (tecta > M);
    CREDO_VERUM (vacua > M);
    /* copia = pictura directa eiusdem rectanguli: tectum et
     * profunditas pixelo cuique eadem (regio loco suo picta est) */
    {
               StatuaLocata  locata;
          StatuaScaenaVisus  visus;
        StatuaScaenaPictura  pictura;
         StereometriaMatrix* m;

        m = (StereometriaMatrix*)loci_ficti_memoria(c,
            (s->locus->nodorum + I) * magnitudo(StereometriaMatrix));
        statua_positura(s->locus, -I, 0.0f, m);
        locata.statua    = s->locus;
        locata.matrices  = m;
        locata.locatio   = stereometria_identitas();
        visus            = mundus_visus(px - C, py - C);
        CREDO_NECESSE (statua_scaenam_pingere(&locata, I, &visus, CC,
            CC,
            &pictura, c->piscina));
        mala = ZEPHYRUM;
        per (k = ZEPHYRUM; k < CC * CC; k++)
        {
            si (   (pictura.tectum[k] != ZEPHYRUM) != (a[k] != FUNDUS)
                || (   pictura.tectum[k]
                    && memcmp(&pictura.profunditas[k], &p[k],
                           magnitudo(f32))))
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
    }
}

/* ora inter regiones: copia trans oram contra copiam eiusdem loci.
 * Sutura oculo iudicanda est; hic solum numeratur quot pixela in
 * fascia XVI pixelorum circa oram colorem mutant inter columnas
 * vicinas, contra fasciam mediam regionis (mensura rudis). */
interior vacuum
_sectio_sutura (
    CredoContextus* c)
{
    Scaena* s            = _scaena(c);
        i8* a            = _octeti(c, XXXII * LATUS);
        i8* b            = _octeti(c, XXXII * LATUS);
       f32  profunditas  = 0.0f;
       s32  px;
       s32  py;
       s32  rx;
       i32  x;
       i32  y;
       i32  ad_oram   = ZEPHYRUM;
       i32  in_medio  = ZEPHYRUM;

    _pixelum(4.0f, 0.0f, 6.0f, &px, &py, &profunditas);
    rx = px - ((px % MUNDUS_REGIO) + MUNDUS_REGIO) % MUNDUS_REGIO;
    /* fascia trans oram sinistram regionis, et fascia in medio */
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx - XVI, py - L, XXXII,
        LATUS, FUNDUS, a, NIHIL));
    CREDO_VERUM (mundus_humus_copiare(s->humus, rx + C, py - L, XXXII,
        LATUS, FUNDUS, b, NIHIL));
    per (y = ZEPHYRUM; y < LATUS; y++)
    {
        per (x = ZEPHYRUM; x + I < XXXII; x++)
        {
            ad_oram += a[y * XXXII + x] != a[y * XXXII + x + I]
                ? I : ZEPHYRUM;
            in_medio += b[y * XXXII + x] != b[y * XXXII + x + I]
                ? I : ZEPHYRUM;
        }
    }
    printf("  sutura: mutationes inter columnas - ad oram %u, in medio"
        " %u (de %u)\n", ad_oram, in_medio, LATUS * XXXI);
    CREDO_VERUM (mundus_humus_regiones(s->humus) >= I);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
                Scaena* s = _scaena(c);
    LociFictiQuadratum  q[I];
           MundusHumus* h;
                    i8  a[IV];
     StatuaScaenaVisus  visus;

    /* magnitudo nulla */
    CREDO_VERUM (mundus_humus_copiare(s->humus, 0, 0, ZEPHYRUM, X,
        FUNDUS, a, NIHIL));
    CREDO_VERUM (mundus_humus_copiare(s->humus, 0, 0, X, ZEPHYRUM,
        FUNDUS, a, NIHIL));
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(s->humus), ZEPHYRUM);
    /* argumenta NIHIL */
    CREDO_FALSUM (mundus_humus_copiare(NIHIL, 0, 0, II, II, FUNDUS, a,
        NIHIL));
    CREDO_FALSUM (mundus_humus_copiare(s->humus, 0, 0, II, II, FUNDUS,
        NIHIL, NIHIL));
    CREDO_NIHIL (mundus_humus_creare(NIHIL, s->activi, c->piscina));
    CREDO_NIHIL (mundus_humus_creare(s->locus, NIHIL, c->piscina));
    CREDO_NIHIL (mundus_humus_creare(s->locus, s->activi, NIHIL));
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(NIHIL), ZEPHYRUM);
    /* locus sine verticibus: omnia fundus */
    h = mundus_humus_creare(loci_ficti_statua(c, q, ZEPHYRUM),
        s->activi,
        c->piscina);
    CREDO_NECESSE (h != NIHIL);
    memset(a, ZEPHYRUM, magnitudo(a));
    CREDO_VERUM (mundus_humus_copiare(h, 0, 0, II, II, FUNDUS, a,
        NIHIL));
    CREDO_AEQUALIS_I32 (a[III], FUNDUS);
    CREDO_AEQUALIS_I32 (mundus_humus_regiones(h), ZEPHYRUM);
    /* visus mundi */
    visus = mundus_visus(-VII, IX);
    CREDO_AEQUALIS_S32 (visus.origo_x, -VII);
    CREDO_AEQUALIS_S32 (visus.origo_y, IX);
    CREDO_F32_PROXIMUS (visus.gyrus, MUNDUS_GYRUS, 0.0001f);
    CREDO_F32_PROXIMUS (visus.pixela_per_unitatem,
        MUNDUS_PIXELA_PER_UNITATEM, 0.0001f);
    CREDO_VERUM (visus.illuminatum);
}


/* ==================================================
 * Corpora et quadrum (mundus-plan T7)
 * ================================================== */

#define VIA_LAPIDIS "probationes/fixa/statuae/lapis.glb"
#define VIA_FUNGI   "probationes/fixa/statuae/fungus.glb"
#define QUADRI_LATUS CLX
#define CORPORA_MAXIMA IV

nomen structura {
     MundusHumus* humus;
          Statua* locus;
          Mundus  mundus;
    MundusCorpus  corpora[CORPORA_MAXIMA];
     MundusForma  formae[II];
             b32  activi[XVI];
} Theatrum;

interior Statua*
_statuam_legere (
        CredoContextus* c,
    constans character* via)
{
    StatuaFructus f = statua_glb_legere(filum_legere_totum(via,
        c->piscina), c->piscina);

    redde f.successus ? f.statua : NIHIL;
}

/* theatrum: locus datus, formae lapis et fungus, nulla corpora */
interior Theatrum*
_theatrum (
    CredoContextus* c,
            Statua* locus)
{
    Theatrum* t = (Theatrum*)loci_ficti_memoria(c, magnitudo(Theatrum));

    thema_initiare();
    dithering_praeparare_omnes(t->activi);
    t->locus           = locus;
    t->humus           = mundus_humus_creare(locus, t->activi,
        c->piscina);
    t->mundus.solum    = mundus_solum_struere(locus, c->piscina);
    t->mundus.corpora  = t->corpora;
    t->formae[ZEPHYRUM] = mundus_forma(_statuam_legere(c, VIA_LAPIDIS),
        c->piscina);
    t->formae[I]        = mundus_forma(_statuam_legere(c, VIA_FUNGI),
        c->piscina);
    CREDO_VERUM (t->humus != NIHIL);
    CREDO_VERUM (t->mundus.solum != NIHIL);
    CREDO_VERUM (t->formae[ZEPHYRUM].statua != NIHIL);
    CREDO_VERUM (t->formae[I].statua != NIHIL);
    redde t;
}

/* corpus addere; index eius */
interior i32
_corpus_addere (
    Theatrum* t,
         s32  forma,
         f32  x,
         f32  z)
{
    MundusCorpus* corpus = &t->corpora[t->mundus.corporum];

    corpus->radius  = MUNDUS_CORPORIS_RADIUS;
    corpus->forma   = forma;
    CREDO_VERUM (mundus_corpus_ponere(&t->mundus, corpus, x, z));
    redde t->mundus.corporum++;
}

/* quadrum QUADRI_LATUS quadratum, corpus sequens datum */
interior i8*
_quadrum (
    CredoContextus* c,
          Theatrum* t,
               i32  sequens)
{
    i8* indices = _octeti(c, QUADRI_LATUS * QUADRI_LATUS);

    CREDO_VERUM (mundus_quadrum_pingere(t->humus, &t->mundus, t->formae,
        II, sequens, t->activi, FUNDUS, QUADRI_LATUS, QUADRI_LATUS,
        indices, c->piscina));
    redde indices;
}

/* humus sola eiusdem quadri (sine corporibus) */
interior i8*
_humus_quadri (
    CredoContextus* c,
          Theatrum* t,
               i32  sequens)
{
     i8* indices = _octeti(c, QUADRI_LATUS * QUADRI_LATUS);
    s32  origo_x = ZEPHYRUM;
    s32  origo_y = ZEPHYRUM;

    CREDO_VERUM (mundus_quadri_origo(&t->mundus, sequens, QUADRI_LATUS,
        QUADRI_LATUS, &origo_x, &origo_y));
    CREDO_VERUM (mundus_humus_copiare(t->humus, origo_x, origo_y,
        QUADRI_LATUS, QUADRI_LATUS, FUNDUS, indices, NIHIL));
    redde indices;
}

/* pixela quibus quadrum ab humo differt (corpora); fines eorum in
 * 'fines' (x minimum, x maximum, y minimum, y maximum) si non NIHIL */
interior i32
_pixela_corporum (
    constans i8* quadrum,
    constans i8* humus,
            s32* fines)
{
    i32 numerus = ZEPHYRUM;
    s32 x;
    s32 y;

    per (y = ZEPHYRUM; y < QUADRI_LATUS; y++)
    {
        per (x = ZEPHYRUM; x < QUADRI_LATUS; x++)
        {
            si (quadrum[y * QUADRI_LATUS + x]
                == humus[y * QUADRI_LATUS + x])
            {
                perge;
            }
            si (fines)
            {
                si (!numerus || x < fines[ZEPHYRUM])
                {
                    fines[ZEPHYRUM] = x;
                }
                si (!numerus || x > fines[I])
                {
                    fines[I] = x;
                }
                si (!numerus || y < fines[II])
                {
                    fines[II] = y;
                }
                si (!numerus || y > fines[III])
                {
                    fines[III] = y;
                }
            }
            numerus++;
        }
    }
    redde numerus;
}

interior vacuum
_sectio_forma (
    CredoContextus* c)
{
    LociFictiQuadratum  q[I];
           MundusForma  f;
                Statua* lapis = _statuam_legere(c, VIA_LAPIDIS);

    CREDO_NECESSE (lapis != NIHIL);
    /* lapis: cycli "walk", "boulder_throw" */
    f = mundus_forma(lapis, c->piscina);
    CREDO_VERUM (f.statua == lapis);
    CREDO_AEQUALIS_S32 (f.cyclus_ambulandi, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (f.cyclus_quietis, -I);
    CREDO_VERUM (f.scala > 0.3f && f.scala < 3.0f);
    /* fungus: "walk_bouncy" */
    f = mundus_forma(_statuam_legere(c, VIA_FUNGI), c->piscina);
    CREDO_AEQUALIS_S32 (f.cyclus_ambulandi, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (f.cyclus_quietis, -I);
    /* quadratum II x IV iacens ad y 3 (sine cyclis): latitudo IV
     * scalam cogit ad III / IV; pedes ad y 3; medium (1, 2) */
    q[ZEPHYRUM] = loci_ficti_quadratum(0.0f, 2.0f, 0.0f, 4.0f, 3.0f,
        3.0f, ZEPHYRUM);
    f = mundus_forma(loci_ficti_statua(c, q, I), c->piscina);
    CREDO_AEQUALIS_S32 (f.cyclus_ambulandi, -I);
    CREDO_AEQUALIS_S32 (f.cyclus_quietis, -I);
    CREDO_F32_PROXIMUS (f.scala, 0.75f, 0.001f);
    CREDO_F32_PROXIMUS (f.imum, 3.0f, 0.001f);
    CREDO_F32_PROXIMUS (f.medium_x, 1.0f, 0.001f);
    CREDO_F32_PROXIMUS (f.medium_z, 2.0f, 0.001f);
    /* clivus altus I, latus I: altitudo scalam dat (II) */
    q[ZEPHYRUM] = loci_ficti_quadratum(0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, ZEPHYRUM);
    f = mundus_forma(loci_ficti_statua(c, q, I), c->piscina);
    CREDO_F32_PROXIMUS (f.scala, 2.0f, 0.001f);
    /* vacua */
    f = mundus_forma(NIHIL, c->piscina);
    CREDO_VERUM (f.statua == NIHIL);
    CREDO_F32_PROXIMUS (f.scala, 1.0f, 0.0001f);
    CREDO_AEQUALIS_S32 (f.cyclus_ambulandi, -I);
    f = mundus_forma(loci_ficti_statua(c, q, ZEPHYRUM), c->piscina);
    CREDO_F32_PROXIMUS (f.scala, 1.0f, 0.0001f);
    /* sine piscina: per sphaeram, scala finita et positiva */
    f = mundus_forma(lapis, NIHIL);
    CREDO_VERUM (f.scala > 0.2f && f.scala < 3.0f);
}

interior vacuum
_sectio_medium (
    CredoContextus* c)
{
    Theatrum* t = _theatrum(c, loci_ficti_locus(c));
          i8* quadrum;
          i8* humus;
         s32  fines[IV];
         i32  pixela;
         i32  k;

    k        = _corpus_addere(t, ZEPHYRUM, 4.0f, 6.0f);
    quadrum  = _quadrum(c, t, k);
    humus    = _humus_quadri(c, t, k);
    memset(fines, ZEPHYRUM, magnitudo(fines));
    pixela = _pixela_corporum(quadrum, humus, fines);
    /* corpus II unitates altum = circa LXV pixela; pixela eius multa,
     * sed non quadrum totum */
    CREDO_VERUM (pixela > D);
    CREDO_VERUM (pixela < QUADRI_LATUS * QUADRI_LATUS / III);
    /* medium quadri intra corpus, et medium corporis prope medium
     * quadri */
    CREDO_VERUM (fines[ZEPHYRUM] < QUADRI_LATUS / II);
    CREDO_VERUM (fines[I] > QUADRI_LATUS / II);
    CREDO_VERUM (fines[II] < QUADRI_LATUS / II);
    CREDO_VERUM (fines[III] > QUADRI_LATUS / II);
    CREDO_VERUM ((fines[ZEPHYRUM] + fines[I]) / II
        > QUADRI_LATUS / II - XX);
    CREDO_VERUM ((fines[ZEPHYRUM] + fines[I]) / II
        < QUADRI_LATUS / II + XX);
    CREDO_VERUM ((fines[II] + fines[III]) / II > QUADRI_LATUS / II
        - XX);
    CREDO_VERUM ((fines[II] + fines[III]) / II < QUADRI_LATUS / II
        + XX);
    /* altitudo in schirmo: II unitates * XL * cos(XXXV) = LXV, plus
     * latitudo corporis inclinata; inter XL et CXX */
    CREDO_VERUM (fines[III] - fines[II] > XL);
    CREDO_VERUM (fines[III] - fines[II] < CXX);
    /* omnia pixela indices palettae aut fundus */
    per (k = ZEPHYRUM; k < QUADRI_LATUS * QUADRI_LATUS; k++)
    {
        si (quadrum[k] >= XVI && quadrum[k] != FUNDUS)
        {
            frange;
        }
    }
    CREDO_AEQUALIS_I32 (k, QUADRI_LATUS * QUADRI_LATUS);
}

/* locus cum tecto: solum x [0, 8] z [0, 12]; tectum sine signo ad y
 * III super x [-1.5, 3.5] z [5, 9.5]. Oculus a (-x, +z) spectat,
 * XXXV gradus supra: corpus ad (3, 5) totum post tectum latet, corpus
 * ad (6.5, 2) patet. */
interior Statua*
_locus_cum_tecto (
    CredoContextus* c)
{
    LociFictiQuadratum q[II];

    q[ZEPHYRUM] = loci_ficti_quadratum(0.0f, 8.0f, 0.0f, 12.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    q[I] = loci_ficti_quadratum(-1.5f, 3.5f, 5.0f, 9.5f, 3.0f, 3.0f,
        ZEPHYRUM);
    redde loci_ficti_statua(c, q, II);
}

interior vacuum
_sectio_occultum (
    CredoContextus* c)
{
    Theatrum* t = _theatrum(c, _locus_cum_tecto(c));
         i32  k;

    /* tectum sine signo solum non claudit */
    k = _corpus_addere(t, ZEPHYRUM, 3.0f, 5.0f);
    CREDO_AEQUALIS_I32 (_pixela_corporum(_quadrum(c, t, k),
        _humus_quadri(c, t, k), NIHIL), ZEPHYRUM);
    /* idem corpus in aperto */
    CREDO_VERUM (mundus_corpus_ponere(&t->mundus, &t->corpora[k], 6.5f,
        2.0f));
    CREDO_VERUM (_pixela_corporum(_quadrum(c, t, k),
        _humus_quadri(c, t, k), NIHIL) > D);
}

interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
    Theatrum* t = _theatrum(c, loci_ficti_locus(c));
    Theatrum* u = _theatrum(c, loci_ficti_locus(c));
          i8* a;
          i8* b;
          i8* solus;

    /* duo corpora in schirmo se tegentia: alterum unitate remotius
     * secundum axem visus (+x, -z) */
    (vacuum)_corpus_addere(t, ZEPHYRUM, 4.0f, 6.0f);
    (vacuum)_corpus_addere(t, I, 4.7f, 5.3f);
    (vacuum)_corpus_addere(u, I, 4.7f, 5.3f);
    (vacuum)_corpus_addere(u, ZEPHYRUM, 4.0f, 6.0f);
    a = _quadrum(c, t, ZEPHYRUM);
    b = _quadrum(c, u, I);
    CREDO_VERUM (memcmp(a, b, QUADRI_LATUS * QUADRI_LATUS) == ZEPHYRUM);
    /* corpus remotius aliquid mutat (apparet), sed corpus propius
     * partem eius tegit: quadrum cum duobus non est quadrum cum
     * propiore solo */
    u->mundus.corporum = ZEPHYRUM;
    (vacuum)_corpus_addere(u, ZEPHYRUM, 4.0f, 6.0f);
    solus = _quadrum(c, u, ZEPHYRUM);
    CREDO_VERUM (memcmp(a, solus, QUADRI_LATUS * QUADRI_LATUS)
        != ZEPHYRUM);
    /* medium quadri (corpus propius) idem cum et sine remotiore */
    CREDO_AEQUALIS_I32 (
        a[(QUADRI_LATUS / II) * QUADRI_LATUS + QUADRI_LATUS / II],
        solus[(QUADRI_LATUS / II) * QUADRI_LATUS + QUADRI_LATUS / II]);
}

interior vacuum
_sectio_quadri_extra (
    CredoContextus* c)
{
    Theatrum* t = _theatrum(c, loci_ficti_locus(c));
          i8* quadrum;
          i8  unum[I];
         i32  k;
         i32  fundi = ZEPHYRUM;
         i32  sequens;

    /* corpus prope angulum loci: pars quadri extra locum */
    sequens = _corpus_addere(t, I, 0.7f, 0.7f);
    quadrum = _quadrum(c, t, sequens);
    per (k = ZEPHYRUM; k < QUADRI_LATUS * QUADRI_LATUS; k++)
    {
        fundi += quadrum[k] == FUNDUS ? I : ZEPHYRUM;
    }
    CREDO_VERUM (fundi > M);
    CREDO_VERUM (fundi < QUADRI_LATUS * QUADRI_LATUS);
    /* magnitudines minimae */
    CREDO_VERUM (mundus_quadrum_pingere(t->humus, &t->mundus, t->formae,
        II, sequens, t->activi, FUNDUS, ZEPHYRUM, ZEPHYRUM, unum,
        c->piscina));
    unum[ZEPHYRUM] = 0x77;
    CREDO_VERUM (mundus_quadrum_pingere(t->humus, &t->mundus, t->formae,
        II, sequens, t->activi, FUNDUS, I, I, unum, c->piscina));
    CREDO_VERUM (unum[ZEPHYRUM] != 0x77);
    /* quies: idem quadrum bis */
    CREDO_VERUM (memcmp(quadrum, _quadrum(c, t, sequens),
        QUADRI_LATUS * QUADRI_LATUS) == ZEPHYRUM);
    /* corpus ambulans aliud quadrum dat quam quiescens */
    t->corpora[sequens].movetur       = VERUM;
    t->corpora[sequens].tempus_motus  = 0.3f;
    CREDO_VERUM (memcmp(quadrum, _quadrum(c, t, sequens),
        QUADRI_LATUS * QUADRI_LATUS) != ZEPHYRUM);
}

interior vacuum
_sectio_quadri_mala (
    CredoContextus* c)
{
    Theatrum* t        = _theatrum(c, loci_ficti_locus(c));
          i8* indices  = _octeti(c, QUADRI_LATUS * QUADRI_LATUS);
          i8* humus;
         s32  x = ZEPHYRUM;
         s32  y = ZEPHYRUM;
         i32  k;

    k = _corpus_addere(t, ZEPHYRUM, 4.0f, 6.0f);
    /* forma extra fines aut sine statua: corpus non pingitur */
    humus                = _humus_quadri(c, t, k);
    t->corpora[k].forma  = VII;
    CREDO_AEQUALIS_I32 (_pixela_corporum(_quadrum(c, t, k), humus,
        NIHIL), ZEPHYRUM);
    t->corpora[k].forma = -I;
    CREDO_AEQUALIS_I32 (_pixela_corporum(_quadrum(c, t, k), humus,
        NIHIL), ZEPHYRUM);
    t->corpora[k].forma         = ZEPHYRUM;
    t->formae[ZEPHYRUM].statua  = NIHIL;
    CREDO_AEQUALIS_I32 (_pixela_corporum(_quadrum(c, t, k), humus,
        NIHIL), ZEPHYRUM);
    /* sequens extra fines, argumenta NIHIL */
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, &t->mundus,
        t->formae,
        II, V, t->activi, FUNDUS, VIII, VIII, indices, c->piscina));
    CREDO_FALSUM (mundus_quadrum_pingere(NIHIL, &t->mundus, t->formae,
        II, k, t->activi, FUNDUS, VIII, VIII, indices, c->piscina));
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, NIHIL, t->formae, II,
        k, t->activi, FUNDUS, VIII, VIII, indices, c->piscina));
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, &t->mundus,
        t->formae,
        II, k, NIHIL, FUNDUS, VIII, VIII, indices, c->piscina));
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, &t->mundus,
        t->formae,
        II, k, t->activi, FUNDUS, VIII, VIII, NIHIL, c->piscina));
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, &t->mundus,
        t->formae,
        II, k, t->activi, FUNDUS, VIII, VIII, indices, NIHIL));
    CREDO_FALSUM (mundus_quadri_origo(&t->mundus, V, VIII, VIII, &x,
        &y));
    CREDO_FALSUM (mundus_quadri_origo(NIHIL, ZEPHYRUM, VIII, VIII, &x,
        &y));
    /* mundus sine corporibus: sequens nullus */
    t->mundus.corporum = ZEPHYRUM;
    CREDO_FALSUM (mundus_quadrum_pingere(t->humus, &t->mundus,
        t->formae,
        II, ZEPHYRUM, t->activi, FUNDUS, VIII, VIII, indices,
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "stabilis", _sectio_stabilis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pigra", _sectio_pigra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extra", _sectio_extra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "profunditas", _sectio_profunditas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sutura", _sectio_sutura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "medium", _sectio_medium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "occultum", _sectio_occultum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordo", _sectio_ordo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "quadri_extra", _sectio_quadri_extra, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "quadri_mala", _sectio_quadri_mala, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_pictura", SECTIONES);
}
