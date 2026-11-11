/* probatio_mundus_pictura.c - humus mundi (mundus_pictura.h)
 *
 * In loco ficto (mundus_locus_fictus.h). stabilis: copia pixelo lapsa
 * in pixelis communibus eadem. pigra: regio semel fit, cum visus eam
 * attingit. extra: extra locum fundus, nulla regio. profunditas:
 * eadem ac proiectio. margo: rectangulum partim extra locum. sutura:
 * ora inter regiones (numerus impressus, non assertus). vacua.
 */
#include "latina.h"
#include "piscina.h"
#include "thema.h"
#include "dithering.h"
#include "stereometria.h"
#include "statua.h"
#include "statua_pictura.h"
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "stabilis", _sectio_stabilis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pigra", _sectio_pigra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extra", _sectio_extra, NIHIL, NIHIL, NIHIL, NIHIL },
    { "profunditas", _sectio_profunditas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sutura", _sectio_sutura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacua", _sectio_vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_pictura", SECTIONES);
}
