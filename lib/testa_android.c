/* testa_android.c - testa Android (testa-spec II.4): NativeActivity in
 * filo principali SOLO (nullum filum alterum, D1). Vocationes
 * systematis -> cursus communis (testa_cursus) et partes purae
 * (testa_android_initus); applicatio_initium ad fenestram primam
 * (magnitudo vera). Effusio (stdout/stderr) per fistulam in logcat.
 * Proprietates: debug.rhubarb.fumus = N quadra (deinde fumus.png,
 * EXITUS=, finis), debug.rhubarb.scala = scala imposita (mensura T3). */

#include "postulata_posix.h"
#include "postulata_android.h"

#include "testa.h"
#include "testa_android_initus.h"
#include "filum.h"
#include "thema.h"
#include "color.h"
#include "chorda.h"

#include <android/native_activity.h>
#include <android/input.h>
#include <android/looper.h>
#include <android/choreographer.h>
#include <android/native_window.h>
#include <android/log.h>
#include <android/configuration.h>
#include <sys/system_properties.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef TESTA_TITULUS
#define TESTA_TITULUS "applicatio"   /* apk.sh: -DTESTA_TITULUS="X" */
#endif

/* constantae testa_android_initus.h == NDK (compilatio frangitur si
 * differunt: magnitudo negativa) */
externus character testa_congruentia_initus[
    (   AMOTION_EVENT_ACTION_DOWN == TESTA_TACTUS_DEPRESSIO
     && AMOTION_EVENT_ACTION_UP == TESTA_TACTUS_LIBERATIO
     && AMOTION_EVENT_ACTION_MOVE == TESTA_TACTUS_MOTUS
     && AMOTION_EVENT_ACTION_CANCEL == TESTA_TACTUS_ABRUPTIO
     && AMOTION_EVENT_ACTION_POINTER_DOWN
         == TESTA_TACTUS_INDICATOR_DEPRESSIO
     && AMOTION_EVENT_ACTION_POINTER_UP
         == TESTA_TACTUS_INDICATOR_LIBERATIO
     && AMOTION_EVENT_TOOL_TYPE_FINGER == TESTA_INSTRUMENTUM_DIGITUS
     && AMOTION_EVENT_TOOL_TYPE_STYLUS == TESTA_INSTRUMENTUM_STILUS
     && AMOTION_EVENT_TOOL_TYPE_MOUSE == TESTA_INSTRUMENTUM_MUS
     && AMOTION_EVENT_TOOL_TYPE_ERASER == TESTA_INSTRUMENTUM_GUMMI)
    ? I : -I];

nomen structura {
       ANativeActivity* activitas;
               Piscina* piscina;
          TestaAmbitus  ambitus;
           TestaCursus  cursus;
             character* argv[III];
                   b32  incepta;          /* initium vocatum */
                   b32  fracta;           /* initium/forma defecit */
                   b32  finita;           /* testa_cursum_finire factum */
         ANativeWindow* fenestra;
       TabulaPixelorum* tabula;
        TestaGeometria  geometria;
           AInputQueue* cauda;
               ALooper* ansa;
        AChoreographer* chorographus;
           TestaTactus  tactus;
     TestaTactusCrudus  crudus;           /* ~5 KiB: hic, non in acervo */
                   b32  vivus;
                   b32  pendens_statim;
                   b32  pendens_post;
                   b32  debitum;
                   b32  facultates_missae;
                   i32  fumus_quadra;     /* 0 = nullus */
                   b32  contentum_notum;  /* onContentRectChanged venit */
                 ARect  contentum;        /* sine vectibus systematis */
} TestaAndroid;

/* argv applicationis (testa_cursus.h: mutabile; -Wwrite-strings
 * litteras constantes facit - regula T1) */
hic_manens character _argv_titulus[] = TESTA_TITULUS;
hic_manens character _argv_fumus[]   = "-fumus";

/* instantia viva: vocationes chorographi pendentes post onDestroy eam
 * comparant (nulla revocatio in API) - mortua = nihil agunt */
hic_manens TestaAndroid* _testa_viva = NIHIL;

/* --- effusio: stdout/stderr -> fistula -> logcat (per processum) --- */

hic_manens integer   _fistula_lectio = -I;
hic_manens character _linea[MXXIV];
hic_manens i32       _linea_mensura = ZEPHYRUM;

interior integer
_effusionem_legere (
    integer  fd,
    integer  eventa,
     vacuum* data)
{
    character frustum[CCLVI];
      ssize_t lecta;
      ssize_t k;

    (vacuum)eventa;
    (vacuum)data;
    dum ((lecta = read(fd, frustum, magnitudo(frustum))) > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < lecta; k++)
        {
            si (   frustum[k]         == '\n'
                || _linea_mensura + I >= (i32)magnitudo(_linea))
            {
                _linea[_linea_mensura] = '\0';
                (vacuum)__android_log_write(ANDROID_LOG_INFO,
                    TESTA_TITULUS, _linea);
                _linea_mensura = ZEPHYRUM;
                si (frustum[k] == '\n')
                {
                    perge;
                }
            }
            _linea[_linea_mensura]  = frustum[k];
            _linea_mensura          += I;
        }
    }
    redde I;   /* vocatio manet */
}

interior vacuum
_effusionem_ducere (
    ALooper* ansa)
{
    integer f[II];

    si (_fistula_lectio >= ZEPHYRUM || pipe(f) != ZEPHYRUM)
    {
        redde;
    }
    /* finis SCRIPTURAE quoque non obstruens (recensio I2): lector solus
     * in eodem filo principali currit - fistula plena (~LXIV KiB in
     * vocatione una) scriptorem in aeternum obstrueret (ANR). Effusio
     * superflua perditur, applicatio vivit. Vexillum descriptionis
     * apertae: dup2 id communicat. */
    (vacuum)fcntl(f[I], F_SETFL, O_NONBLOCK);
    (vacuum)dup2(f[I], STDOUT_FILENO);
    (vacuum)dup2(f[I], STDERR_FILENO);
    (vacuum)close(f[I]);
    (vacuum)setvbuf(stdout, NIHIL, _IOLBF, BUFSIZ);
    (vacuum)setvbuf(stderr, NIHIL, _IONBF, ZEPHYRUM);
    (vacuum)fcntl(f[ZEPHYRUM], F_SETFL, O_NONBLOCK);
    _fistula_lectio = f[ZEPHYRUM];
    (vacuum)ALooper_addFd(ansa, _fistula_lectio, ALOOPER_POLL_CALLBACK,
        ALOOPER_EVENT_INPUT, _effusionem_legere, NIHIL);
}

/* ante finem: nihil in fistula perditur (AUDIENDA spec) */
interior vacuum
_effusionem_exhaurire (
    vacuum)
{
    (vacuum)fflush(stdout);
    (vacuum)fflush(stderr);
    si (_fistula_lectio >= ZEPHYRUM)
    {
        (vacuum)_effusionem_legere(_fistula_lectio, ZEPHYRUM, NIHIL);
    }
}

/* --- servitia et sedes --- */

interior ImagoFructus
_imaginem_nullam (
     vacuum* ctx,
    Piscina* piscina)
{
    ImagoFructus f;

    (vacuum)ctx;
    memset(&f, ZEPHYRUM, magnitudo(ImagoFructus));
    f.successus  = FALSUM;
    f.error      = chorda_ex_literis("clipboard Android nondum (B1)",
                                     piscina);
    redde f;
}

/* dpi schirmi (AConfiguration; Fire 7 = 160 = ACONFIGURATION_DENSITY_MEDIUM) */
interior i32
_densitas (
    ANativeActivity* activitas)
{
    AConfiguration* configuratio;
            int32_t densitas;

    configuratio = AConfiguration_new();
    si (!configuratio)
    {
        redde ZEPHYRUM;
    }
    AConfiguration_fromAssetManager(configuratio,
        activitas->assetManager);
    densitas = AConfiguration_getDensity(configuratio);
    AConfiguration_delete(configuratio);
    redde densitas > ZEPHYRUM ? (i32)densitas : ZEPHYRUM;
}

interior i32
_proprietas_numerus (
    constans character* titulus)
{
    character valor[PROP_VALUE_MAX];
      integer n;

    si (__system_property_get(titulus, valor) <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    n = atoi(valor);
    redde n > ZEPHYRUM ? (i32)n : ZEPHYRUM;
}

/* cwd, HOME = internalDataPath; TMPDIR = <sedes>/tmp (volumen
 * temporarium fumi, identitas machinae - android-gradus) */
interior b32
_sedem_ponere (
    constans character* sedes)
{
    character tmp[DXII];

    si (!sedes || strlen(sedes) + V >= magnitudo(tmp))
    {
        redde FALSUM;
    }
    sprintf(tmp, "%s/tmp", sedes);
    si (   !filum_directorium_creare_cum_parentibus(tmp)
        || chdir(sedes) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde setenv("HOME", sedes, I) == ZEPHYRUM
        && setenv("TMPDIR", tmp, I) == ZEPHYRUM;
}

/* --- quadra --- */

interior vacuum
_quadrum_petere (
    TestaAndroid* t);

interior vacuum
_fumum_finire (
    TestaAndroid* t)
{
    character via[DXII];
          s32 exitus;

    testa_cursum_finire(&t->cursus);
    t->finita  = VERUM;
    exitus     = I;
    si (strlen(t->activitas->internalDataPath) + XII < magnitudo(via))
    {
        sprintf(via, "%s/fumus.png", t->activitas->internalDataPath);
        si (ludus_fenestra_imaginem_scribere(t->cursus.lf, via))
        {
            exitus = ZEPHYRUM;
        }
    }
    imprimere("EXITUS=%d\n", (int)exitus);
    _effusionem_exhaurire();
    t->fumus_quadra = ZEPHYRUM;
    ANativeActivity_finish(t->activitas);
}

interior vacuum
_quadrum (
    TestaAndroid* t)
{
           LudusFenestra* lf;
    ANativeWindow_Buffer  alveus;
                     s64  nunc;
                     b32  pingendum;

    lf = t->cursus.lf;
    si (!t->fenestra || !lf || !t->vivus || t->fracta)
    {
        redde;
    }
    nunc       = fenestra_tempus_ms();
    pingendum  = t->fumus_quadra > ZEPHYRUM
              || ludus_fenestra_pingendum(lf, t->debitum, nunc);
    t->debitum = FALSUM;
    si (pingendum)
    {
        ludus_quadrum(lf, nunc);
        si (ANativeWindow_lock(t->fenestra, &alveus, NIHIL) == ZEPHYRUM)
        {
            si (   alveus.width
                >= (integer)t->geometria.fenestra_latitudo
                && alveus.height
                    >= (integer)t->geometria.fenestra_altitudo)
            {
                testa_pixela_transferre(lf->tabula, &t->geometria,
                    (i32*)alveus.bits, (i32)alveus.stride,
                    color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
            }
            (vacuum)ANativeWindow_unlockAndPost(t->fenestra);
        }
    }
    si (   t->fumus_quadra > ZEPHYRUM
        && lf->mensurae.quadra >= t->fumus_quadra)
    {
        _fumum_finire(t);
        redde;
    }
    _quadrum_petere(t);
}

interior vacuum
_quadrum_statim (
    int64_t  tempus_ns,
     vacuum* data)
{
    TestaAndroid* t;

    (vacuum)tempus_ns;
    t = (TestaAndroid*)data;
    si (t != _testa_viva)
    {
        redde;
    }
    t->pendens_statim = FALSUM;
    _quadrum(t);
}

interior vacuum
_quadrum_post (
    int64_t  tempus_ns,
     vacuum* data)
{
    TestaAndroid* t;

    (vacuum)tempus_ns;
    t = (TestaAndroid*)data;
    si (t != _testa_viva)
    {
        redde;
    }
    t->pendens_post = FALSUM;
    _quadrum(t);
}

interior vacuum
_quadrum_petere (
    TestaAndroid* t)
{
     TestaQuadriStatus s;
    TestaQuadriPetitio p;

    s.fenestra        = t->fenestra != NIHIL && t->cursus.lf != NIHIL
                     && !t->fracta;
    s.vivus           = t->vivus;
    s.pendens_statim  = t->pendens_statim;
    s.pendens_post    = t->pendens_post;
    s.debitum         = t->debitum;
    s.mora            = t->cursus.lf ? ludus_fenestra_mora(t->cursus.lf)
                                     : ZEPHYRUM;
    s.pulsus          = t->cursus.forma.pulsator != NIHIL;
    s.fumus_reliqua   = (   t->fumus_quadra > ZEPHYRUM && t->cursus.lf
                         && t->cursus.lf->mensurae.quadra
                             < t->fumus_quadra)
                      ? t->fumus_quadra - t->cursus.lf->mensurae.quadra
                      : ZEPHYRUM;
    p = testa_quadrum_petere(&s);
    si (p.genus == TESTA_QUADRUM_STATIM)
    {
        t->pendens_statim = VERUM;
        AChoreographer_postFrameCallback64(t->chorographus,
            _quadrum_statim, t);
    }
    alioquin si (p.genus == TESTA_QUADRUM_POST)
    {
        t->pendens_post = VERUM;
        AChoreographer_postFrameCallbackDelayed64(t->chorographus,
            _quadrum_post, t, (uint32_t)p.post_ms);
    }
}

/* --- initus --- */

interior vacuum
_tradere (
              TestaAndroid* t,
          constans Eventus* e)
{
    si (!t->cursus.lf || t->fracta)
    {
        redde;
    }
    ludus_fenestra_tractare(t->cursus.lf, e, fenestra_tempus_ms());
    t->debitum = VERUM;
}

interior vacuum
_crudum_implere (
       TestaTactusCrudus* c,
    constans AInputEvent* ev)
{
    int32_t actio;
     size_t numerus;
     size_t historia;
     size_t i;
     size_t h;

    actio     = AMotionEvent_getAction(ev);
    c->actio  = (s32)(actio & AMOTION_EVENT_ACTION_MASK);
    c->index_actionis = (i32)((actio
        & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT);
    numerus = AMotionEvent_getPointerCount(ev);
    si (numerus > TESTA_INDICATORES_MAXIMI)
    {
        numerus = TESTA_INDICATORES_MAXIMI;
    }
    historia = AMotionEvent_getHistorySize(ev);
    si (historia > EVENTUS_EXEMPLA_MAXIMA)
    {
        historia = EVENTUS_EXEMPLA_MAXIMA;
    }
    c->numerus = (i32)numerus;
    c->numerus_historiae = (i32)historia;
    c->tempus = (s64)(AMotionEvent_getEventTime(ev) / 1000000);
    per (h = ZEPHYRUM; h < historia; h++)
    {
        c->historia_tempus[h] =
            (s64)(AMotionEvent_getHistoricalEventTime(ev, h) / 1000000);
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        c->ids[i]          = (s32)AMotionEvent_getPointerId(ev, i);
        c->instrumenta[i]  = (s32)AMotionEvent_getToolType(ev, i);
        c->x[i]            = AMotionEvent_getX(ev, i);
        c->y[i]            = AMotionEvent_getY(ev, i);
        per (h = ZEPHYRUM; h < historia; h++)
        {
            c->historia_x[i][h] = AMotionEvent_getHistoricalX(ev, i, h);
            c->historia_y[i][h] = AMotionEvent_getHistoricalY(ev, i, h);
        }
    }
}

/* VERUM = tractatum (systema non tangit) */
interior b32
_eventum_tractare (
    TestaAndroid* t,
     AInputEvent* ev)
{
    Eventus e;

    si (AInputEvent_getType(ev) == AINPUT_EVENT_TYPE_KEY)
    {
        si (AKeyEvent_getKeyCode(ev) != AKEYCODE_BACK)
        {
            redde FALSUM;   /* volumen etc.: systematis */
        }
        si (AKeyEvent_getAction(ev) == AKEY_EVENT_ACTION_UP)
        {
            ANativeActivity_finish(t->activitas);   /* D6: back = exire */
        }
        redde VERUM;
    }
    si (AInputEvent_getType(ev) != AINPUT_EVENT_TYPE_MOTION)
    {
        redde FALSUM;
    }
    si (!t->cursus.lf)
    {
        redde VERUM;    /* ante fenestram: devoratur */
    }
    si (!t->facultates_missae)
    {
        t->facultates_missae = VERUM;
        testa_facultates_tactus(&e, fenestra_tempus_ms());
        _tradere(t, &e);
    }
    _crudum_implere(&t->crudus, ev);
    si (testa_tactum_vertere(&t->tactus, &t->crudus, &t->geometria, &e))
    {
        _tradere(t, &e);
        _quadrum_petere(t);
    }
    redde VERUM;
}

interior integer
_caudam_legere (
    integer  fd,
    integer  eventa,
     vacuum* data)
{
    TestaAndroid* t;
     AInputEvent* ev;

    (vacuum)fd;
    (vacuum)eventa;
    t = (TestaAndroid*)data;
    dum (t->cauda && AInputQueue_getEvent(t->cauda, &ev) >= ZEPHYRUM)
    {
        si (AInputQueue_preDispatchEvent(t->cauda, ev))
        {
            perge;
        }
        AInputQueue_finishEvent(t->cauda, ev,
            _eventum_tractare(t, ev) ? I : ZEPHYRUM);
    }
    redde I;
}

/* --- vocationes activitatis --- */

interior TestaAndroid*
_testa (
    ANativeActivity* activitas)
{
    redde (TestaAndroid*)activitas->instance;
}

interior vacuum
_finire_fracte (
          TestaAndroid* t,
    constans character* causa)
{
    fprintf(stderr, "%s: %s\n", TESTA_TITULUS, causa);
    t->fracta = VERUM;
    _effusionem_exhaurire();
    ANativeActivity_finish(t->activitas);
}

/* area contenti in pixelis fenestrae: vectes systematis (status,
 * navalis) tactus suos devorant - in Fire 7 navalis [552,600] lineam
 * instrumentorum pictoris tegebat (T3, Franus). Ignota -> fenestra
 * tota. */
interior vacuum
_aream_legere (
    TestaAndroid* t,
             s32* x0,
             s32* y0,
             s32* x1,
             s32* y1)
{
    si (t->contentum_notum)
    {
        *x0 = (s32)t->contentum.left;
        *y0 = (s32)t->contentum.top;
        *x1 = (s32)t->contentum.right;
        *y1 = (s32)t->contentum.bottom;
        redde;
    }
    *x0 = ZEPHYRUM;
    *y0 = ZEPHYRUM;
    *x1 = (s32)ANativeWindow_getWidth(t->fenestra);
    *y1 = (s32)ANativeWindow_getHeight(t->fenestra);
}

/* geometria + tabula ad fenestram et aream praesentes: prima vice
 * tabula et ludus struuntur, deinde MUTARE_MAGNITUDINEM (puncta areae;
 * ludus_fenestra tabulam aptat, 013 B3b) */
interior vacuum
_aream_aptare (
    TestaAndroid* t)
{
    integer latitudo;
    integer altitudo;
        s32 x0;
        s32 y0;
        s32 x1;
        s32 y1;
        i32 scala;
        i32 area_latitudo;
        i32 area_altitudo;
    Eventus e;

    si (!t->fenestra || !t->incepta || t->fracta)
    {
        redde;
    }
    latitudo = ANativeWindow_getWidth(t->fenestra);
    altitudo = ANativeWindow_getHeight(t->fenestra);
    _aream_legere(t, &x0, &y0, &x1, &y1);
    scala = _proprietas_numerus("debug.rhubarb.scala");
    si (scala == ZEPHYRUM)
    {
        scala = t->cursus.forma.scala;
    }
    si (!testa_geometriam_in_area((i32)latitudo, (i32)altitudo,
            x0, y0, x1, y1, scala, &t->geometria))
    {
        _finire_fracte(t, "area contenti minor scala");
        redde;
    }
    area_latitudo = t->geometria.latitudo * scala;
    area_altitudo = t->geometria.altitudo * scala;
    (vacuum)ANativeWindow_setBuffersGeometry(t->fenestra, latitudo,
        altitudo, WINDOW_FORMAT_RGBA_8888);
    si (!t->tabula)
    {
        t->tabula = tabula_pixelorum_creare_nuda(t->piscina,
            t->geometria.latitudo, t->geometria.altitudo);
        si (!t->tabula)
        {
            _finire_fracte(t, "tabula");
            redde;
        }
        t->tabula->scala              = (f32)scala;
        t->tabula->fenestra_latitudo  = area_latitudo;
        t->tabula->fenestra_altitudo  = area_altitudo;
        si (!testa_cursum_ludum_struere(&t->cursus, t->piscina,
            t->tabula))
        {
            _finire_fracte(t, "ludus");
            redde;
        }
    }
    alioquin si (   area_latitudo != t->tabula->fenestra_latitudo
                 || area_altitudo != t->tabula->fenestra_altitudo)
    {
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        e.datum.mutare_magnitudinem.latitudo = area_latitudo;
        e.datum.mutare_magnitudinem.altitudo = area_altitudo;
        _tradere(t, &e);
        t->tabula->fenestra_latitudo = area_latitudo;
        t->tabula->fenestra_altitudo = area_altitudo;
    }
    t->debitum = VERUM;
    _quadrum_petere(t);
}

/* initium semel, cum fenestra ET area contenti adsunt: applicationes
 * superficiem veram poscunt (documentum novum pictoris ei aptatur) -
 * onContentRectChanged post fenestram venire potest */
interior vacuum
_initium_facere (
    TestaAndroid* t)
{
    constans character* causa;
                   s32  x0;
                   s32  y0;
                   s32  x1;
                   s32  y1;

    si (t->incepta || !t->fenestra || !t->contentum_notum)
    {
        redde;
    }
    _aream_legere(t, &x0, &y0, &x1, &y1);
    imprimere("testa: fenestra %dx%d, area contenti [%d,%d]-[%d,%d]\n",
        (int)ANativeWindow_getWidth(t->fenestra),
        (int)ANativeWindow_getHeight(t->fenestra), (int)x0, (int)y0,
        (int)x1, (int)y1);
    t->incepta          = VERUM;
    t->argv[ZEPHYRUM]   = _argv_titulus;
    t->argv[I]          = _argv_fumus;
    t->argv[II]         = NIHIL;
    t->ambitus.piscina  = t->piscina;
    t->ambitus.fumus    = t->fumus_quadra > ZEPHYRUM;
    t->ambitus.argc     = t->ambitus.fumus ? II : I;
    t->ambitus.argv     = t->argv;
    t->ambitus.schirmi_latitudo =
        (i32)ANativeWindow_getWidth(t->fenestra);
    t->ambitus.schirmi_altitudo =
        (i32)ANativeWindow_getHeight(t->fenestra);
    t->ambitus.utilis_x              = (i32)x0;
    t->ambitus.utilis_y              = (i32)y0;
    t->ambitus.utilis_latitudo       = (i32)(x1 - x0);
    t->ambitus.utilis_altitudo       = (i32)(y1 - y0);
    t->ambitus.superficies_latitudo  = (i32)(x1 - x0);
    t->ambitus.superficies_altitudo  = (i32)(y1 - y0);
    t->ambitus.sedes_datorum         = t->activitas->internalDataPath;
    t->ambitus.densitas              = _densitas(t->activitas);
    t->ambitus.imago_glutinata       = _imaginem_nullam;
    si (!testa_cursum_incipere(&t->cursus, &t->ambitus,
            applicatio_initium, &causa))
    {
        testa_cursum_dimittere(&t->cursus);
        _finire_fracte(t, causa);
    }
}

interior vacuum
_fenestra_creata (
    ANativeActivity* activitas,
      ANativeWindow* fenestra)
{
    TestaAndroid* t;

    t            = _testa(activitas);
    t->fenestra  = fenestra;
    _initium_facere(t);
    _aream_aptare(t);
}

/* area contenti mutata (vectes systematis, situs): tabula aptatur */
interior vacuum
_contentum_mutatum (
      ANativeActivity* activitas,
       constans ARect* area)
{
    TestaAndroid* t;

    t                   = _testa(activitas);
    t->contentum        = *area;
    t->contentum_notum  = VERUM;
    _initium_facere(t);
    _aream_aptare(t);
}

interior vacuum
_fenestra_deleta (
    ANativeActivity* activitas,
      ANativeWindow* fenestra)
{
    (vacuum)fenestra;
    _testa(activitas)->fenestra = NIHIL;
}

interior vacuum
_vitam_mutare (
    ANativeActivity* activitas,
                b32  vivus)
{
    TestaAndroid* t;
         Eventus  e;

    t         = _testa(activitas);
    t->vivus  = vivus;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = vivus ? EVENTUS_RESUMPTIO : EVENTUS_SUSPENSIO;
    _tradere(t, &e);
    _quadrum_petere(t);
}

interior vacuum
_resumpta (
    ANativeActivity* activitas)
{
    _vitam_mutare(activitas, VERUM);
}

interior vacuum
_pausata (
    ANativeActivity* activitas)
{
    _vitam_mutare(activitas, FALSUM);
}

interior vacuum
_cauda_creata (
    ANativeActivity* activitas,
        AInputQueue* cauda)
{
    TestaAndroid* t;

    t         = _testa(activitas);
    t->cauda  = cauda;
    AInputQueue_attachLooper(cauda, t->ansa, ALOOPER_POLL_CALLBACK,
        _caudam_legere, t);
}

interior vacuum
_cauda_deleta (
    ANativeActivity* activitas,
        AInputQueue* cauda)
{
    AInputQueue_detachLooper(cauda);
    _testa(activitas)->cauda = NIHIL;
}

interior vacuum
_deleta (
    ANativeActivity* activitas)
{
    TestaAndroid* t;

    t            = _testa(activitas);
    _testa_viva  = NIHIL;
    si (t->cursus.lf && !t->finita)
    {
        testa_cursum_finire(&t->cursus);
    }
    testa_cursum_dimittere(&t->cursus);
    _effusionem_exhaurire();
    activitas->instance = NIHIL;
    piscina_destruere(t->piscina);
}

/* principale applicationis in Android numquam vocatur (introitus =
 * ANativeActivity_onCreate); symbolum adest ut nexus
 * (-Wl,--no-undefined) consentiat */
s32
testa_principale (
        integer   argc,
      character** argv)
{
    (vacuum)argc;
    (vacuum)argv;
    redde I;
}

vacuum
ANativeActivity_onCreate (
     ANativeActivity* activitas,
              vacuum* status_servatus,
              size_t  magnitudo_status)
{
         Piscina* piscina;
    TestaAndroid* t;

    (vacuum)status_servatus;     /* B2 */
    (vacuum)magnitudo_status;
    _effusionem_ducere(ALooper_forThread());
    piscina = piscina_generare_dynamicum(TESTA_TITULUS, VIII * M * M);
    t = piscina ? (TestaAndroid*)piscina_allocare(piscina,
                      magnitudo(TestaAndroid))
                : NIHIL;
    si (!t || !_sedem_ponere(activitas->internalDataPath))
    {
        fprintf(stderr, "%s: piscina aut sedes deficit\n",
            TESTA_TITULUS);
        _effusionem_exhaurire();
        ANativeActivity_finish(activitas);
        redde;
    }
    memset(t, ZEPHYRUM, magnitudo(TestaAndroid));
    t->activitas         = activitas;
    t->piscina           = piscina;
    t->ansa              = ALooper_forThread();
    t->chorographus      = AChoreographer_getInstance();
    t->fumus_quadra      = _proprietas_numerus("debug.rhubarb.fumus");
    activitas->instance  = t;
    _testa_viva          = t;

    activitas->callbacks->onNativeWindowCreated    = _fenestra_creata;
    activitas->callbacks->onNativeWindowDestroyed  = _fenestra_deleta;
    activitas->callbacks->onResume                 = _resumpta;
    activitas->callbacks->onPause                  = _pausata;
    activitas->callbacks->onInputQueueCreated      = _cauda_creata;
    activitas->callbacks->onInputQueueDestroyed    = _cauda_deleta;
    activitas->callbacks->onContentRectChanged     = _contentum_mutatum;
    activitas->callbacks->onDestroy                = _deleta;
}
