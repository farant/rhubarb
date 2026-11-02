/* testa_ios.m - testa iOS (testa-ios-spec II.1): UIKit cum scaena (D9),
 * filum principale SOLUM (B1 D1). Vocationes systematis -> cursus
 * communis (testa_cursus) et partes purae (testa_initus), ut
 * testa_android.c. applicatio_initium ad dispositionem primam in qua
 * limites visus ET area tuta nota sunt (superficies = area tuta in
 * punctis: documentum novum ei aptatur). Pixela: tabula nostra (scala
 * formae in punctis) -> alveus pixelorum schirmi (puncta x scala
 * schirmi) per testa_pixela_transferre -> CGImage ut contenta strati,
 * proximus vicinus. Argumentum -fumus (simctl/devicectl launch):
 * ictus syntheticus per viam tactus (R4), QUADRA_FUMI quadra,
 * <Documents>/fumus.png, EXITUS=, exit(). */

/* postulata_ios.h ante capita UIKit ('inline' sub C89); deinde
 * systemata primum ut vitare conflictos macros */
#include "postulata_ios.h"
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <PhotosUI/PhotosUI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "testa.h"
#include "testa_initus.h"
#include "fenestra_tempus.h"
#include "thema.h"
#include "color.h"
#include "chorda.h"
#include "eventus.h"
#include "reactor.h"
#include "veredarius.h"
#include "piscina.h"

#ifndef TESTA_TITULUS
#define TESTA_TITULUS "applicatio"   /* app.sh: -DTESTA_TITULUS="X" */
#endif

#define QUADRA_FUMI      CXX       /* plan T3: N quadra */
#define FUMUS_INITIUM    V         /* ictus post quadrum V incipit (R4) */
#define FUMUS_GRADUS     X         /* DEPRESSIO, MOTUS x VIII, LIBERATIO */
#define DENSITAS_PUNCTI  CLXIII    /* puncta iOS ~CLXIII per unciam */

/* constantae testa_initus.h == UITouchPhase (compilatio frangitur si
 * differunt: magnitudo negativa) */
externus character testa_congruentia_phasium[
    (   (s32)UITouchPhaseBegan == (s32)TESTA_PHASIS_INCIPIT
     && (s32)UITouchPhaseMoved == (s32)TESTA_PHASIS_MOVETUR
     && (s32)UITouchPhaseStationary == (s32)TESTA_PHASIS_QUIESCIT
     && (s32)UITouchPhaseEnded == (s32)TESTA_PHASIS_FINIT
     && (s32)UITouchPhaseCancelled == (s32)TESTA_PHASIS_ABRUPTA)
    ? I : -I];

nomen structura {
               Piscina* piscina;
          TestaAmbitus  ambitus;
           TestaCursus  cursus;
                   s32  argc;
            character** argv;
                   b32  incepta;          /* initium vocatum */
                   b32  fracta;           /* initium/forma defecit */
                   b32  finita;           /* testa_cursum_finire factum */
       TabulaPixelorum* tabula;
        TestaGeometria  geometria;
                   s32  area_x0;          /* area tuta, pixela schirmi */
                   s32  area_y0;
                   s32  area_x1;
                   s32  area_y1;
           TestaTactus  tactus;
     TestaTactusCrudus  crudus;           /* ~5 KiB: hic, non in acervo */
                vacuum* tactus_loci[TESTA_INDICATORES_MAXIMI]; /* UITouch*
                                           * per id; NIHIL = liber */
                   b32  vivus;
                   b32  pendens_statim;
                   b32  pendens_post;
                   b32  debitum;
                   b32  facultates_missae;
                   i32  fumus_quadra;     /* 0 = nullus */
                   i32* alveus;           /* pixela schirmi, RGBA */
                   i32  alveus_latitudo;
                   i32  alveus_altitudo;
                   i32  scala_schirmi;    /* pixela per punctum (II, III) */
                   i32  claviatura;       /* TestaClaviaturaGenus (T9) */
             character  sedes[DXII];      /* Documents */
} TestaIOS;

@class TestaVisus;
@class TestaElector;

/* instantia una (scaena una, UIApplicationSupportsMultipleScenes
 * FALSUM) */
hic_manens TestaIOS       _testa;
hic_manens TestaVisus*    _visus    = nil;
hic_manens CADisplayLink* _vinculum = nil;
/* C1b: electio imaginis - rector qui electorem ostendit, delegatus
 * electoris (vivit per processum), et vexillum 'eligens' quod electorem
 * apertum ET photographiam adhuc lectam tegit (petitio una simul) */
hic_manens UIViewController* _rector  = nil;
hic_manens TestaElector*     _elector = nil;
hic_manens b32               _eligens = FALSUM;

/* ==================================================
 * Veredarius per URLSession (communio-plan-5 T8, spec D11): rete
 * Apple, nulla cryptographia in fasciculo. Sessio ephemera, cauda
 * principalis (completio in filo principali - B1 D1); responsum in
 * piscinam vocantis copiatur, deinde quadrum statim petitur ut reactor
 * id tractet.
 * ================================================== */

#define VEREDARII_LOCI  LXIV

nomen structura {
                    b32  in_usu;
                    b32  abrupta;
                    i32  generatio;
                 vacuum* opus;        /* NSURLSessionDataTask* (retentum) */
        VeredariusFinis  finis;
                 vacuum* ctx;
                Piscina* piscina;
} LocusVeredariiIOS;

hic_manens NSURLSession*     _sessio = nil;
hic_manens LocusVeredariiIOS _loci_veredarii[VEREDARII_LOCI];
hic_manens Veredarius        _veredarius;

interior vacuum
_quadrum_statim_petere (
    vacuum);

interior vacuum
_claviaturam_ponere (
    vacuum* ctx,
       i32  genus);

interior vacuum
_responsum_tradere (
    LocusVeredariiIOS* l,
                i32  generatio,
            NSData* data,
     NSURLResponse* responsum,
           NSError* error)
{
    VeredariusResponsum r;
        VeredariusFinis finis;
                 vacuum* ctx;
                    i8* octeti;
              character  causa[XCVI];
                    i32  n;

    si (!l->in_usu || l->abrupta || l->generatio != generatio)
    {
        redde;
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    si (error || ![responsum isKindOfClass:[NSHTTPURLResponse class]])
    {
        /* causa sine URL (NSError domain + codex solum) */
        sprintf(causa, "rete: %.40s %ld", error
            ? [[error domain] UTF8String] : "responsum", error
            ? (long)[error code] : 0L);
        r.causa = chorda_ex_literis(causa, l->piscina);
    }
    alioquin
    {
        r.successus  = VERUM;
        r.status     = (i32)[(NSHTTPURLResponse*)responsum statusCode];
        n            = data ? (i32)[data length] : ZEPHYRUM;
        octeti       = n > ZEPHYRUM ? (i8*)piscina_allocare(l->piscina,
            (memoriae_index)n) : NIHIL;
        si (octeti)
        {
            memcpy(octeti, [data bytes], (size_t)n);
            r.corpus.datum    = octeti;
            r.corpus.mensura  = n;
        }
    }
    finis      = l->finis;
    ctx        = l->ctx;
    l->in_usu  = FALSUM;
    [(NSURLSessionDataTask*)l->opus release];
    l->opus    = NIHIL;
    finis(ctx, &r);
    _quadrum_statim_petere();
}

interior b32
_veredarium_petere (
                        vacuum* sui,
    constans VeredariusPetitio* p,
                       Piscina* piscina,
               VeredariusFinis  finis,
                        vacuum* ctx,
                           i32* signum)
{
          NSMutableURLRequest* rogatio;
         NSURLSessionDataTask* opus;
                       NSURL* url;
            LocusVeredariiIOS* l;
                          i32  i;
                          i32  generatio;
                      NSTimeInterval tempus;

    (vacuum)sui;
    si (!_sessio || !p || !p->url || !piscina || !finis)
    {
        redde FALSUM;
    }
    l = NIHIL;
    per (i = ZEPHYRUM; i < VEREDARII_LOCI; i++)
    {
        si (!_loci_veredarii[i].in_usu)
        {
            l = &_loci_veredarii[i];
            frange;
        }
    }
    url = [NSURL URLWithString:[NSString stringWithUTF8String:p->url]];
    si (!l || !url)
    {
        redde FALSUM;
    }
    /* tempus petitionis + XV s (URLSession tempus otii suum habet) */
    tempus = (NSTimeInterval)(p->tempus_ms > ZEPHYRUM ? p->tempus_ms
        : (i32)XXX * (i32)M) / 1000.0 + 15.0;
    rogatio = [NSMutableURLRequest requestWithURL:url
        cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
        timeoutInterval:tempus];
    [rogatio setHTTPMethod:p->methodus == VEREDARIUS_POST ? @"POST"
        : p->methodus == VEREDARIUS_PUT ? @"PUT" : @"GET"];
    si (p->auctoritas)
    {
        [rogatio setValue:[NSString stringWithUTF8String:p->auctoritas]
            forHTTPHeaderField:@"Authorization"];
    }
    si (p->corpus.mensura > ZEPHYRUM)
    {
        [rogatio setValue:[NSString stringWithUTF8String:p->mimen
            ? p->mimen : "application/octet-stream"]
            forHTTPHeaderField:@"Content-Type"];
        [rogatio setHTTPBody:[NSData dataWithBytes:p->corpus.datum
            length:(NSUInteger)p->corpus.mensura]];
    }
    l->generatio++;
    generatio   = l->generatio;
    l->in_usu   = VERUM;
    l->abrupta  = FALSUM;
    l->finis    = finis;
    l->ctx      = ctx;
    l->piscina  = piscina;
    opus = [_sessio dataTaskWithRequest:rogatio
        completionHandler:^(NSData* data, NSURLResponse* responsum,
                            NSError* error) {
        _responsum_tradere(l, generatio, data, responsum, error);
    }];
    si (!opus)
    {
        l->in_usu = FALSUM;
        redde FALSUM;
    }
    l->opus = [opus retain];
    si (signum)
    {
        *signum = (i32)(l - _loci_veredarii) + generatio
            * (i32)VEREDARII_LOCI;
    }
    [opus resume];
    redde VERUM;
}

interior vacuum
_veredarium_abrumpere (
    vacuum* sui,
       i32  signum)
{
    LocusVeredariiIOS* l;

    (vacuum)sui;
    l = &_loci_veredarii[signum % (i32)VEREDARII_LOCI];
    si (!l->in_usu || l->generatio != signum / (i32)VEREDARII_LOCI)
    {
        redde;
    }
    l->abrupta  = VERUM;
    l->in_usu   = FALSUM;
    si (l->opus)
    {
        [(NSURLSessionDataTask*)l->opus cancel];
        [(NSURLSessionDataTask*)l->opus release];
        l->opus = NIHIL;
    }
}

interior Veredarius*
_veredarium_parare (
    vacuum)
{
    NSURLSessionConfiguration* forma;

    si (!_sessio)
    {
        forma = [NSURLSessionConfiguration ephemeralSessionConfiguration];
        [forma setRequestCachePolicy:
            NSURLRequestReloadIgnoringLocalCacheData];
        _sessio = [[NSURLSession sessionWithConfiguration:forma
            delegate:nil delegateQueue:[NSOperationQueue mainQueue]]
            retain];
    }
    _veredarius.petere     = _veredarium_petere;
    _veredarius.abrumpere  = _veredarium_abrumpere;
    _veredarius.sui        = NIHIL;
    redde _sessio ? &_veredarius : NIHIL;
}

/* -probare-rete <url>: GET unum per veredarium, status in stdout
 * (probatio simulatri; nihil aliud) */
interior vacuum
_probationis_finis (
                          vacuum* ctx,
    constans VeredariusResponsum* r)
{
    (vacuum)ctx;
    imprimere("testa: rete successus=%d status=%d\n", (int)r->successus,
        (int)r->status);
}

interior vacuum
_rete_probare (
    vacuum)
{
    VeredariusPetitio p;
                  s32 k;
                  i32 signum;

    per (k = I; k + I < _testa.argc; k++)
    {
        si (strcmp(_testa.argv[k], "-probare-rete") == ZEPHYRUM)
        {
            memset(&p, ZEPHYRUM, magnitudo(p));
            p.methodus   = VEREDARIUS_GET;
            p.url        = _testa.argv[k + I];
            p.tempus_ms  = (i32)XV * (i32)M;
            (vacuum)_veredarium_petere(NIHIL, &p, _testa.piscina,
                _probationis_finis, NIHIL, &signum);
            redde;
        }
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
    f.error      = chorda_ex_literis("clipboard iOS nondum (C1)",
                                     piscina);
    redde f;
}

/* cwd = Documents (volumina applicationis ibi; sedes_datorum),
 * TMPDIR = NSTemporaryDirectory(); HOME immutatum (receptaculum:
 * identitas machinae sub Library/, R2) */
interior b32
_sedem_ponere (
    vacuum)
{
               NSArray* viae;
              NSString* documenta;
    constans character* c;
    constans character* tmp;

    viae = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
        NSUserDomainMask, YES);
    si ([viae count] == ZEPHYRUM)
    {
        redde FALSUM;
    }
    documenta  = [viae objectAtIndex:ZEPHYRUM];
    c          = [documenta fileSystemRepresentation];
    tmp        = [NSTemporaryDirectory() fileSystemRepresentation];
    si (!c || !tmp || strlen(c) + I >= magnitudo(_testa.sedes))
    {
        redde FALSUM;
    }
    strcpy(_testa.sedes, c);
    redde chdir(_testa.sedes) == ZEPHYRUM
        && setenv("TMPDIR", tmp, I) == ZEPHYRUM;
}

/* tempus tactus (systemUptime, basis mach) in horologio nostro
 * (CLOCK_MONOTONIC): differentia ad nunc in basi tactus */
interior s64
_tempus_tactus (
    NSTimeInterval tempus)
{
    redde fenestra_tempus_ms()
        - (s64)((CACurrentMediaTime() - tempus) * 1000.0);
}

/* --- quadra --- */

interior vacuum
_quadrum_petere (
    vacuum);

interior b32
_imaginem_eligere (
    vacuum* ctx);

interior vacuum
_tradere (
    constans Eventus* e)
{
    si (!_testa.cursus.lf || _testa.fracta)
    {
        redde;
    }
    ludus_fenestra_tractare(_testa.cursus.lf, e, fenestra_tempus_ms());
    _testa.debitum = VERUM;
}

interior vacuum
_finire_fracte (
    constans character* causa)
{
    fprintf(stderr, "%s: %s\n", TESTA_TITULUS, causa);
    _testa.fracta = VERUM;
    si (_testa.fumus_quadra > ZEPHYRUM)
    {
        imprimere("EXITUS=1\n");
        (vacuum)fflush(stdout);
        exit(I);
    }
}

interior vacuum
_fumum_finire (
    vacuum)
{
    character via[DXII];
          s32 exitus;

    testa_cursum_finire(&_testa.cursus);
    _testa.finita  = VERUM;
    exitus         = I;
    si (strlen(_testa.sedes) + XII < magnitudo(via))
    {
        sprintf(via, "%s/fumus.png", _testa.sedes);
        si (ludus_fenestra_imaginem_scribere(_testa.cursus.lf, via))
        {
            exitus = ZEPHYRUM;
        }
    }
    testa_cursum_dimittere(&_testa.cursus);
    imprimere("EXITUS=%d\n", (int)exitus);
    (vacuum)fflush(stdout);
    (vacuum)fflush(stderr);
    exit((int)exitus);   /* in fumo solo (spec II.1) */
}

/* alveus -> contenta strati: CGImage super exemplar datorum (CFData
 * exscribit - alveus quadro proximo rescribitur dum Core Animation
 * imaginem priorem fortasse adhuc legit) */
interior vacuum
_alveum_monstrare (
    vacuum)
{
          CFDataRef data;
    CGDataProviderRef provisor;
      CGColorSpaceRef spatium;
           CGImageRef imago;
               size_t gradus;

    gradus  = (size_t)_testa.alveus_latitudo * magnitudo(i32);
    data    = CFDataCreate(NULL, (constans UInt8*)_testa.alveus,
        (CFIndex)(gradus * (size_t)_testa.alveus_altitudo));
    si (!data)
    {
        redde;
    }
    provisor  = CGDataProviderCreateWithCFData(data);
    spatium   = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    imago     = (provisor && spatium)
        ? CGImageCreate((size_t)_testa.alveus_latitudo,
              (size_t)_testa.alveus_altitudo, VIII, XXXII, gradus,
              spatium, (CGBitmapInfo)kCGImageAlphaNoneSkipLast, provisor,
              NULL, false, kCGRenderingIntentDefault)
        : NULL;
    si (imago)
    {
        [CATransaction begin];
        [CATransaction setDisableActions:YES];
        ((UIView*)_visus).layer.contents = (id)imago;
        [CATransaction commit];
        CGImageRelease(imago);
    }
    si (spatium)
    {
        CGColorSpaceRelease(spatium);
    }
    si (provisor)
    {
        CGDataProviderRelease(provisor);
    }
    CFRelease(data);
}

interior vacuum
_crudum_tradere (
    vacuum)
{
    Eventus e;

    si (!_testa.cursus.lf || _testa.fracta)
    {
        redde;
    }
    si (!_testa.facultates_missae)
    {
        _testa.facultates_missae = VERUM;
        testa_facultates_tactus(&e, fenestra_tempus_ms());
        _tradere(&e);
    }
    si (testa_tactum_vertere(&_testa.tactus, &_testa.crudus,
            &_testa.geometria, &e))
    {
        _tradere(&e);
        _quadrum_petere();
    }
}

/* R4: ictus syntheticus fumi per viam tactus veram sine UIKit -
 * (0.3, 0.3) -> (0.7, 0.45) areae tutae, gradus unus per quadrum */
interior vacuum
_ictum_fumi (
    i32 quadrum)
{
    TestaTactusCrudus* c;
                  s32  k;
                  s32  phasis;
                  i32  ante;
                  i32  post;
                  s32  actio;
                  f32  f;

    k = (s32)quadrum - (s32)FUMUS_INITIUM;
    si (k < ZEPHYRUM || k >= (s32)FUMUS_GRADUS)
    {
        redde;
    }
    phasis  = k == ZEPHYRUM ? (s32)TESTA_PHASIS_INCIPIT
            : k == (s32)FUMUS_GRADUS - I ? (s32)TESTA_PHASIS_FINIT
                                         : (s32)TESTA_PHASIS_MOVETUR;
    ante    = k == ZEPHYRUM ? ZEPHYRUM : I;
    post    = k == (s32)FUMUS_GRADUS - I ? ZEPHYRUM : I;
    actio   = testa_actio_ex_phase((i32)phasis, ante, post);
    si (actio < ZEPHYRUM)
    {
        redde;
    }
    f  = (f32)k / (f32)(FUMUS_GRADUS - I);
    c  = &_testa.crudus;
    memset(c, ZEPHYRUM, magnitudo(TestaTactusCrudus));
    c->actio                 = actio;
    c->index_actionis        = ZEPHYRUM;
    c->numerus               = I;
    c->ids[ZEPHYRUM]          = ZEPHYRUM;
    c->instrumenta[ZEPHYRUM]  = (s32)TESTA_INSTRUMENTUM_DIGITUS;
    c->x[ZEPHYRUM]  = (f32)_testa.area_x0 + (f32)(_testa.area_x1
        - _testa.area_x0) * (0.3f + 0.4f * f);
    c->y[ZEPHYRUM]  = (f32)_testa.area_y0 + (f32)(_testa.area_y1
        - _testa.area_y0) * (0.3f + 0.15f * f);
    c->tempus = fenestra_tempus_ms();
    _crudum_tradere();
}

interior vacuum
_quadrum (
    vacuum)
{
    LudusFenestra* lf;
              s64  nunc;
              b32  pingendum;

    lf = _testa.cursus.lf;
    si (!lf || !_testa.vivus || _testa.fracta || !_testa.alveus)
    {
        redde;
    }
    /* plan-5 T8: horae reactoris (communio) ante quadrum */
    si (lf->reactor)
    {
        (vacuum)reactor_poll(lf->reactor, ZEPHYRUM);
    }
    si (_testa.fumus_quadra > ZEPHYRUM)
    {
        _ictum_fumi(lf->mensurae.quadra);
    }
    nunc       = fenestra_tempus_ms();
    pingendum  = _testa.fumus_quadra > ZEPHYRUM
              || ludus_fenestra_pingendum(lf, _testa.debitum, nunc);
    _testa.debitum = FALSUM;
    si (pingendum)
    {
        ludus_quadrum(lf, nunc);
        testa_pixela_transferre(lf->tabula, &_testa.geometria,
            _testa.alveus, _testa.alveus_latitudo,
            color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
        _alveum_monstrare();
    }
    si (   _testa.fumus_quadra > ZEPHYRUM
        && lf->mensurae.quadra >= _testa.fumus_quadra)
    {
        _fumum_finire();
        redde;
    }
    _quadrum_petere();
}

/* --- initus --- */

interior s32
_locum_invenire (
    UITouch* tactus)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)TESTA_INDICATORES_MAXIMI; i++)
    {
        si (_testa.tactus_loci[i] == (vacuum*)tactus)
        {
            redde i;
        }
    }
    redde -I;
}

/* crudus ex tactibus activis (ordine locorum); 'locus' = tactus
 * mutatus, historia eius ex tactibus coalitis */
interior vacuum
_crudum_implere (
       s32  actio,
       s32  locus,
    UIEvent* eventum)
{
    TestaTactusCrudus* c;
              NSArray* coaliti;
             UITouch* t;
                  s32  i;
                  i32  n;
                  i32  h;
                  i32  historia;
              CGPoint  p;
                  f32  s;

    c  = &_testa.crudus;
    s  = (f32)_testa.scala_schirmi;
    memset(c, ZEPHYRUM, magnitudo(TestaTactusCrudus));
    c->actio  = actio;
    n         = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (s32)TESTA_INDICATORES_MAXIMI; i++)
    {
        si (_testa.tactus_loci[i] == NIHIL)
        {
            perge;
        }
        t = (UITouch*)_testa.tactus_loci[i];
        si (i == locus)
        {
            c->index_actionis  = n;
            c->tempus          = _tempus_tactus([t timestamp]);
        }
        p = [t locationInView:(UIView*)_visus];
        c->ids[n]          = i;
        c->instrumenta[n]  = [t type] == UITouchTypePencil
                           ? (s32)TESTA_INSTRUMENTUM_STILUS
                           : (s32)TESTA_INSTRUMENTUM_DIGITUS;
        c->x[n]            = (f32)p.x * s;
        c->y[n]            = (f32)p.y * s;
        n                 += I;
    }
    c->numerus = n;
    si (actio != (s32)TESTA_TACTUS_MOTUS || locus < ZEPHYRUM)
    {
        redde;
    }
    /* coaliti: exempla inter eventus, ultimum = positio praesens
     * (Android: historia SINE praesenti) */
    coaliti   = [eventum coalescedTouchesForTouch:
        (UITouch*)_testa.tactus_loci[locus]];
    historia  = coaliti ? (i32)[coaliti count] : ZEPHYRUM;
    historia  = historia > ZEPHYRUM ? historia - I : ZEPHYRUM;
    si (historia > EVENTUS_EXEMPLA_MAXIMA)
    {
        historia = EVENTUS_EXEMPLA_MAXIMA;
    }
    c->numerus_historiae = historia;
    per (h = ZEPHYRUM; h < historia; h++)
    {
        t = [coaliti objectAtIndex:(NSUInteger)h];
        c->historia_tempus[h] = _tempus_tactus([t timestamp]);
        per (i = ZEPHYRUM; i < (s32)n; i++)
        {
            si (c->ids[i] == locus)
            {
                p = [t locationInView:(UIView*)_visus];
                c->historia_x[i][h] = (f32)p.x * s;
                c->historia_y[i][h] = (f32)p.y * s;
            }
            alioquin
            {
                c->historia_x[i][h] = c->x[i];
                c->historia_y[i][h] = c->y[i];
            }
        }
    }
}

/* tactus unus mutatus -> actio (testa_actio_ex_phase) -> machina
 * tactus: digitus primus ducit, secundus ignoratur, abruptio semel */
interior vacuum
_tactum_unum (
    UITouch* tactus,
    UIEvent* eventum)
{
    s32 phasis;
    s32 locus;
    s32 i;
    i32 activi;
    i32 post;
    s32 actio;

    phasis  = (s32)[tactus phase];
    locus   = _locum_invenire(tactus);
    activi  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (s32)TESTA_INDICATORES_MAXIMI; i++)
    {
        si (_testa.tactus_loci[i] != NIHIL)
        {
            activi += I;
        }
    }
    si (phasis == (s32)TESTA_PHASIS_INCIPIT)
    {
        si (locus < ZEPHYRUM)
        {
            per (i = ZEPHYRUM; i < (s32)TESTA_INDICATORES_MAXIMI; i++)
            {
                si (_testa.tactus_loci[i] == NIHIL)
                {
                    locus = i;
                    frange;
                }
            }
        }
        si (locus < ZEPHYRUM)
        {
            redde;   /* plures quam X digiti */
        }
        _testa.tactus_loci[locus] = (vacuum*)tactus;
        post = activi + I;
    }
    alioquin
    {
        si (locus < ZEPHYRUM)
        {
            redde;
        }
        post = (   phasis == (s32)TESTA_PHASIS_FINIT
                || phasis == (s32)TESTA_PHASIS_ABRUPTA)
             ? activi - I : activi;
    }
    actio = testa_actio_ex_phase((i32)phasis, activi, post);
    si (actio >= ZEPHYRUM && _testa.cursus.lf)
    {
        _crudum_implere(actio, locus, eventum);
        _crudum_tradere();
    }
    si (   phasis == (s32)TESTA_PHASIS_FINIT
        || phasis == (s32)TESTA_PHASIS_ABRUPTA)
    {
        _testa.tactus_loci[locus] = NIHIL;
    }
}

/* --- area, initium, vita --- */

/* geometria + tabula ad visum et aream tutam praesentes: prima vice
 * tabula et ludus struuntur, deinde MUTARE_MAGNITUDINEM (pixela
 * schirmi; ludus_fenestra tabulam aptat, 013 B3b) - ut Android */
interior vacuum
_aream_aptare (
    vacuum)
{
          CGRect  limites;
    UIEdgeInsets  tuta;
             i32  latitudo;
             i32  altitudo;
             i32  s;
             i32  scala;
             i32  area_latitudo;
             i32  area_altitudo;
         Eventus  e;

    si (!_visus || !_testa.incepta || _testa.fracta)
    {
        redde;
    }
    limites  = [(UIView*)_visus bounds];
    tuta     = [(UIView*)_visus safeAreaInsets];
    s        = _testa.scala_schirmi;
    latitudo = (i32)(limites.size.width * (CGFloat)s);
    altitudo = (i32)(limites.size.height * (CGFloat)s);
    _testa.area_x0 = (s32)(tuta.left * (CGFloat)s);
    _testa.area_y0 = (s32)(tuta.top * (CGFloat)s);
    _testa.area_x1 = (s32)latitudo - (s32)(tuta.right * (CGFloat)s);
    _testa.area_y1 = (s32)altitudo - (s32)(tuta.bottom * (CGFloat)s);
    scala = (i32)_testa.cursus.forma.scala * s;
    si (!testa_geometriam_in_area(latitudo, altitudo, _testa.area_x0,
            _testa.area_y0, _testa.area_x1, _testa.area_y1, scala,
            &_testa.geometria))
    {
        _finire_fracte("area tuta minor scala");
        redde;
    }
    area_latitudo = _testa.geometria.latitudo * scala;
    area_altitudo = _testa.geometria.altitudo * scala;
    si (   latitudo != _testa.alveus_latitudo
        || altitudo != _testa.alveus_altitudo)
    {
        free(_testa.alveus);
        _testa.alveus = (i32*)malloc((size_t)latitudo
            * (size_t)altitudo * magnitudo(i32));
        si (!_testa.alveus)
        {
            _finire_fracte("alveus pixelorum");
            redde;
        }
        _testa.alveus_latitudo = latitudo;
        _testa.alveus_altitudo = altitudo;
    }
    si (!_testa.tabula)
    {
        _testa.tabula = tabula_pixelorum_creare_nuda(_testa.piscina,
            _testa.geometria.latitudo, _testa.geometria.altitudo);
        si (!_testa.tabula)
        {
            _finire_fracte("tabula");
            redde;
        }
        _testa.tabula->scala              = (f32)scala;
        _testa.tabula->fenestra_latitudo  = area_latitudo;
        _testa.tabula->fenestra_altitudo  = area_altitudo;
        si (!testa_cursum_ludum_struere(&_testa.cursus, _testa.piscina,
            _testa.tabula))
        {
            _finire_fracte("ludus");
            redde;
        }
        /* plan-5 T8: reactor applicationis in quadris agitur, mora
         * quadri hora eius proxima finitur */
        si (_testa.cursus.forma.reactor)
        {
            ludus_fenestra_reactorem_ponere(_testa.cursus.lf,
                _testa.cursus.forma.reactor);
        }
    }
    alioquin si (   area_latitudo != _testa.tabula->fenestra_latitudo
                 || area_altitudo != _testa.tabula->fenestra_altitudo)
    {
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        e.datum.mutare_magnitudinem.latitudo = area_latitudo;
        e.datum.mutare_magnitudinem.altitudo = area_altitudo;
        _tradere(&e);
        _testa.tabula->fenestra_latitudo = area_latitudo;
        _testa.tabula->fenestra_altitudo = area_altitudo;
    }
    _testa.debitum = VERUM;
    _quadrum_petere();
}

/* initium semel, cum visus in fenestra est ET limites habet (area
 * tuta tum nota): superficies = area tuta in punctis */
interior vacuum
_initium_facere (
    vacuum)
{
    constans character* causa;
                CGRect  limites;
          UIEdgeInsets  tuta;
               CGFloat  scala;

    si (   _testa.incepta || !_visus || ![(UIView*)_visus window]
        || _testa.fracta)
    {
        redde;
    }
    limites = [(UIView*)_visus bounds];
    si (limites.size.width < I || limites.size.height < I)
    {
        redde;
    }
    tuta   = [(UIView*)_visus safeAreaInsets];
    scala  = [[(UIView*)_visus traitCollection] displayScale];
    _testa.scala_schirmi = scala >= (CGFloat)I ? (i32)scala : I;
    imprimere("testa: visus %dx%d puncta (scala %d), area tuta "
        "[%d,%d]-[%d,%d]\n", (int)limites.size.width,
        (int)limites.size.height, (int)_testa.scala_schirmi,
        (int)tuta.left, (int)tuta.top,
        (int)(limites.size.width - tuta.right),
        (int)(limites.size.height - tuta.bottom));
    _testa.incepta                 = VERUM;
    _testa.ambitus.piscina         = _testa.piscina;
    _testa.ambitus.fumus           = _testa.fumus_quadra > ZEPHYRUM;
    _testa.ambitus.argc            = _testa.argc;
    _testa.ambitus.argv            = _testa.argv;
    _testa.ambitus.schirmi_latitudo = (i32)limites.size.width;
    _testa.ambitus.schirmi_altitudo = (i32)limites.size.height;
    _testa.ambitus.utilis_x        = (i32)tuta.left;
    _testa.ambitus.utilis_y        = (i32)tuta.top;
    _testa.ambitus.utilis_latitudo = (i32)(limites.size.width
        - tuta.left - tuta.right);
    _testa.ambitus.utilis_altitudo = (i32)(limites.size.height
        - tuta.top - tuta.bottom);
    _testa.ambitus.superficies_latitudo =
        _testa.ambitus.utilis_latitudo;
    _testa.ambitus.superficies_altitudo =
        _testa.ambitus.utilis_altitudo;
    _testa.ambitus.sedes_datorum   = _testa.sedes;
    _testa.ambitus.densitas        = _testa.scala_schirmi
        * DENSITAS_PUNCTI;
    _testa.ambitus.imago_glutinata = _imaginem_nullam;
    /* C1b: electio imaginis - photographia ut depositio redit */
    _testa.ambitus.imaginem_eligere = _imaginem_eligere;
    /* plan-5 T8: rete per URLSession; reactor horas solas tenet */
    _testa.ambitus.machina     = "iPhone";
    /* T9: claviatura systematis (TestaVisus UIKeyInput) */
    _testa.ambitus.claviaturam_ponere = _claviaturam_ponere;
    _testa.ambitus.claviatura_ctx     = NIHIL;
    _testa.ambitus.reactor     = reactor_creare(_testa.piscina);
    _testa.ambitus.veredarius  = _testa.ambitus.reactor
        ? _veredarium_parare() : NIHIL;
    _rete_probare();
    si (!testa_cursum_incipere(&_testa.cursus, &_testa.ambitus,
            applicatio_initium, &causa))
    {
        testa_cursum_dimittere(&_testa.cursus);
        _finire_fracte(causa);
    }
}

interior vacuum
_dispositio_mutata (
    vacuum)
{
    _initium_facere();
    _aream_aptare();
}

interior vacuum
_vitam_mutare (
    b32 vivus)
{
    Eventus e;

    _testa.vivus = vivus;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = vivus ? EVENTUS_RESUMPTIO : EVENTUS_SUSPENSIO;
    _tradere(&e);
    _quadrum_petere();
}

interior vacuum
_scaenam_finire (
    vacuum)
{
    si (_testa.cursus.lf && !_testa.finita)
    {
        testa_cursum_finire(&_testa.cursus);
        _testa.finita = VERUM;
    }
    testa_cursum_dimittere(&_testa.cursus);
    (vacuum)fflush(stdout);
}


/* ==================================================
 * Claviatura systematis (communio-plan-5 T9): TestaVisus ipse
 * UIKeyInput est - applicatio campum suum pingit, testa claviaturam
 * ostendit et litteras ut eventus reddit (testa_textum_eventus)
 * ================================================== */

#define CLAVIATURAE_EVENTUS  XVI   /* per insertText; reliqua omittuntur */

interior vacuum
_textum_tradere (
    i8* octeti,
    i32 mensura)
{
    Eventus ev[CLAVIATURAE_EVENTUS];
        i32 n;
        i32 i;

    n = testa_textum_eventus(octeti, mensura, fenestra_tempus_ms(), ev,
        CLAVIATURAE_EVENTUS);
    per (i = ZEPHYRUM; i < n; i++)
    {
        _tradere(&ev[i]);
    }
    _quadrum_petere();
}

interior vacuum
_retrorsum_tradere (
    vacuum)
{
    Eventus e;

    testa_clavem_eventus(CLAVIS_RETRORSUM, fenestra_tempus_ms(), &e);
    _tradere(&e);
    _quadrum_petere();
}

/* TestaAmbitus.claviaturam_ponere: NULLA celat; genus mutatum dum
 * claviatura ostenditur -> reloadInputViews (proprietates novae) */
interior vacuum
_claviaturam_ponere (
    vacuum* ctx,
       i32  genus)
{
    UIView* v;

    (vacuum)ctx;
    v = (UIView*)_visus;
    _testa.claviatura = genus;
    si (!v)
    {
        redde;
    }
    si (genus == (i32)TESTA_CLAVIATURA_NULLA)
    {
        (vacuum)[v resignFirstResponder];
        redde;
    }
    si ([v isFirstResponder])
    {
        [v reloadInputViews];
        redde;
    }
    (vacuum)[v becomeFirstResponder];
}

/* ==================================================
 * Classes UIKit
 * ================================================== */

@interface TestaVisus : UIView <UIKeyInput, UITextInputTraits>
- (void)quadrum_statim:(CADisplayLink*)vinculum;
- (void)quadrum_post:(NSTimer*)horologium;
@end

@implementation TestaVisus
- (instancetype)initWithFrame:(CGRect)forma {
    self = [super initWithFrame:forma];
    si (self)
    {
        [self setMultipleTouchEnabled:YES];
        [self setBackgroundColor:[UIColor blackColor]];
        [[self layer] setMagnificationFilter:kCAFilterNearest];
        [[self layer] setMinificationFilter:kCAFilterNearest];
        [[self layer] setContentsGravity:kCAGravityResize];
    }
    redde self;
}

- (void)touchesBegan:(NSSet<UITouch*>*)tactus
           withEvent:(UIEvent*)eventum {
    UITouch* t;

    per (t in tactus)
    {
        _tactum_unum(t, eventum);
    }
}

- (void)touchesMoved:(NSSet<UITouch*>*)tactus
           withEvent:(UIEvent*)eventum {
    UITouch* t;

    per (t in tactus)
    {
        _tactum_unum(t, eventum);
    }
}

- (void)touchesEnded:(NSSet<UITouch*>*)tactus
           withEvent:(UIEvent*)eventum {
    UITouch* t;

    per (t in tactus)
    {
        _tactum_unum(t, eventum);
    }
}

- (void)touchesCancelled:(NSSet<UITouch*>*)tactus
               withEvent:(UIEvent*)eventum {
    UITouch* t;

    per (t in tactus)
    {
        _tactum_unum(t, eventum);
    }
}

/* T9: UIKeyInput - responsor primus solum dum applicatio claviaturam
 * petit */
- (BOOL)canBecomeFirstResponder {
    redde _testa.claviatura != (i32)TESTA_CLAVIATURA_NULLA;
}

/* VERUM semper: deleteBackward etiam campo vacuo venit (applicatio
 * iudicat) */
- (BOOL)hasText {
    redde YES;
}

- (void)insertText:(NSString*)textus {
    NSMutableData* d;

    d = [[[textus dataUsingEncoding:NSUTF8StringEncoding] mutableCopy]
        autorelease];
    si (d && [d length] > 0)
    {
        _textum_tradere((i8*)[d mutableBytes], (i32)[d length]);
    }
}

- (void)deleteBackward {
    _retrorsum_tradere();
}

/* UITextInputTraits: EPISTULA = claviatura epistulae sine correctione;
 * CODEX = ASCII, maiusculae omnes, codex unius usus (oneTimeCode) */
- (UIKeyboardType)keyboardType {
    redde _testa.claviatura == (i32)TESTA_CLAVIATURA_CODEX
        ? UIKeyboardTypeASCIICapable : UIKeyboardTypeEmailAddress;
}

- (UITextAutocapitalizationType)autocapitalizationType {
    redde _testa.claviatura == (i32)TESTA_CLAVIATURA_CODEX
        ? UITextAutocapitalizationTypeAllCharacters
        : UITextAutocapitalizationTypeNone;
}

- (UITextAutocorrectionType)autocorrectionType {
    redde UITextAutocorrectionTypeNo;
}

- (UITextSpellCheckingType)spellCheckingType {
    redde UITextSpellCheckingTypeNo;
}

- (UITextContentType)textContentType {
    redde _testa.claviatura == (i32)TESTA_CLAVIATURA_CODEX
        ? UITextContentTypeOneTimeCode : UITextContentTypeEmailAddress;
}

- (UIReturnKeyType)returnKeyType {
    redde _testa.claviatura == (i32)TESTA_CLAVIATURA_CODEX
        ? UIReturnKeyGo : UIReturnKeySend;
}

/* STATIM: vinculum unum quadrum currit, deinde iterum pausatur */
- (void)quadrum_statim:(CADisplayLink*)vinculum {
    [vinculum setPaused:YES];
    _testa.pendens_statim = FALSUM;
    _quadrum();
}

- (void)quadrum_post:(NSTimer*)horologium {
    (vacuum)horologium;
    _testa.pendens_post = FALSUM;
    _quadrum();
}
@end

interior vacuum
_quadrum_petere (
    vacuum)
{
     TestaQuadriStatus s;
    TestaQuadriPetitio p;
       LudusFenestra* lf;

    lf = _testa.cursus.lf;
    s.fenestra        = _visus != nil && lf != NIHIL && !_testa.fracta
                     && _testa.alveus != NIHIL;
    s.vivus           = _testa.vivus;
    s.pendens_statim  = _testa.pendens_statim;
    s.pendens_post    = _testa.pendens_post;
    s.debitum         = _testa.debitum;
    s.mora            = lf ? ludus_fenestra_mora(lf) : ZEPHYRUM;
    s.pulsus          = _testa.cursus.forma.pulsator != NIHIL;
    s.fumus_reliqua   = (   _testa.fumus_quadra > ZEPHYRUM && lf
                         && lf->mensurae.quadra < _testa.fumus_quadra)
                      ? _testa.fumus_quadra - lf->mensurae.quadra
                      : ZEPHYRUM;
    p = testa_quadrum_petere(&s);
    si (p.genus == TESTA_QUADRUM_STATIM)
    {
        _testa.pendens_statim = VERUM;
        [_vinculum setPaused:NO];
    }
    alioquin si (p.genus == TESTA_QUADRUM_POST)
    {
        _testa.pendens_post = VERUM;
        (vacuum)[NSTimer scheduledTimerWithTimeInterval:
                (NSTimeInterval)p.post_ms / 1000.0
            target:_visus selector:@selector(quadrum_post:) userInfo:nil
            repeats:NO];
    }
}

/* plan-5 T8: responsum retis advenit - quadrum mox (reactor tractat) */
interior vacuum
_quadrum_statim_petere (
    vacuum)
{
    _testa.debitum = VERUM;
    _quadrum_petere();
}

@interface TestaRector : UIViewController
@end

@implementation TestaRector
- (void)loadView {
    TestaVisus* visus;

    visus = [[[TestaVisus alloc] initWithFrame:CGRectZero] autorelease];
    [self setView:visus];
    _visus   = visus;
    _rector  = self;
    si (!_vinculum)
    {
        _vinculum = [[CADisplayLink displayLinkWithTarget:visus
            selector:@selector(quadrum_statim:)] retain];
        [_vinculum setPaused:YES];
        [_vinculum addToRunLoop:[NSRunLoop mainRunLoop]
            forMode:NSRunLoopCommonModes];
    }
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
    _dispositio_mutata();
}

- (void)viewSafeAreaInsetsDidChange {
    [super viewSafeAreaInsetsDidChange];
    _dispositio_mutata();
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
    redde UIInterfaceOrientationMaskPortrait;
}
@end

/* ==================================================
 * Electio imaginis (testa-ios C1b): PHPickerViewController extra
 * processum (nulla licentia bibliothecae), photographia erecta et
 * intra MXXIV (R4) in $TMPDIR/imago_electa.png, deinde
 * EVENTUS_DEPOSITIO ut plagula deposita (D2)
 * ================================================== */

interior vacuum
_imaginem_non_lectam (
    constans character* causa)
{
    imprimere("testa: imago non lecta: %s\n", causa);
    _eligens = FALSUM;
}

/* in filo principali (provisor in cauda Apple respondet - D1 B1) */
interior vacuum
_imaginem_tradere (
    UIImage* imago)
{
    UIGraphicsImageRendererFormat* forma;
          UIGraphicsImageRenderer* pictor;
                          UIImage* parva;
                           NSData* png;
                         NSString* via;
              constans character* via_c;
                          Eventus  e;
                              i32  latitudo;
                              i32  altitudo;
                              i32  lat;
                              i32  alt;

    si (!imago)
    {
        _imaginem_non_lectam("nulla");
        redde;
    }
    latitudo = (i32)([imago size].width * [imago scale]);
    altitudo = (i32)([imago size].height * [imago scale]);
    si (!testa_mensuras_aptare(latitudo, altitudo, M + XXIV, &lat,
            &alt))
    {
        _imaginem_non_lectam("mensurae nullae");
        redde;
    }
    /* drawInRect orientationem EXIF adhibet: photographia erecta */
    forma = [UIGraphicsImageRendererFormat defaultFormat];
    [forma setScale:1.0];
    pictor = [[UIGraphicsImageRenderer alloc]
        initWithSize:CGSizeMake((CGFloat)lat, (CGFloat)alt) format:forma];
    parva = [pictor imageWithActions:^(UIGraphicsImageRendererContext* c) {
        (vacuum)c;
        [imago drawInRect:CGRectMake(0.0, 0.0, (CGFloat)lat,
            (CGFloat)alt)];
    }];
    [pictor release];
    png  = parva ? UIImagePNGRepresentation(parva) : nil;
    via  = [NSTemporaryDirectory()
        stringByAppendingPathComponent:@"imago_electa.png"];
    si (!png || ![png writeToFile:via atomically:YES])
    {
        _imaginem_non_lectam("PNG non scriptum");
        redde;
    }
    via_c = [via fileSystemRepresentation];
    imprimere("testa: imago electa %dx%d -> %dx%d\n", (int)latitudo,
        (int)altitudo, (int)lat, (int)alt);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                         = EVENTUS_DEPOSITIO;
    e.tempus                        = fenestra_tempus_ms();
    e.datum.depositio.viae.datum    = (i8*)(vacuum*)(uintptr_t)via_c;
    e.datum.depositio.viae.mensura  = (i32)strlen(via_c);
    e.datum.depositio.numerus       = I;
    _eligens = FALSUM;
    _tradere(&e);
    _quadrum_petere();
}

@interface TestaElector : NSObject <PHPickerViewControllerDelegate>
@end

@implementation TestaElector
- (void)picker:(PHPickerViewController*)electio
    didFinishPicking:(NSArray<PHPickerResult*>*)electa {
    NSItemProvider* provisor;

    [electio dismissViewControllerAnimated:YES completion:nil];
    si ([electa count] == ZEPHYRUM)
    {
        _eligens = FALSUM;    /* abiecta */
        redde;
    }
    provisor = [[electa objectAtIndex:ZEPHYRUM] itemProvider];
    si (![provisor canLoadObjectOfClass:[UIImage class]])
    {
        _imaginem_non_lectam("forma ignota");
        redde;
    }
    [provisor loadObjectOfClass:[UIImage class]
        completionHandler:^(id<NSItemProviderReading> res, NSError* e) {
        UIImage* imago;

        (vacuum)e;
        imago = [(UIImage*)res retain];
        dispatch_async(dispatch_get_main_queue(), ^{
            _imaginem_tradere(imago);
            [imago release];
        });
    }];
}
@end

/* servitium TestaImaginemEligere: electorem ostendit nisi iam eligitur
 * (electio aperta aut photographia adhuc legitur) */
interior b32
_imaginem_eligere (
    vacuum* ctx)
{
      PHPickerConfiguration* configuratio;
    PHPickerViewController* electio;

    (vacuum)ctx;
    si (_eligens || !_rector)
    {
        redde FALSUM;
    }
    si (!_elector)
    {
        _elector = [[TestaElector alloc] init];
    }
    configuratio = [[[PHPickerConfiguration alloc] init] autorelease];
    [configuratio setFilter:[PHPickerFilter imagesFilter]];
    [configuratio setSelectionLimit:I];
    electio = [[[PHPickerViewController alloc]
        initWithConfiguration:configuratio] autorelease];
    [electio setDelegate:_elector];
    _eligens = VERUM;
    [_rector presentViewController:electio animated:YES completion:nil];
    redde VERUM;
}

/* scaena (D9): Info.plist UISceneDelegateClassName = TestaScaenaDelegatus */
@interface TestaScaenaDelegatus : UIResponder <UIWindowSceneDelegate>
@property (retain, nonatomic) UIWindow* window;
@end

@implementation TestaScaenaDelegatus
@synthesize window;

- (void)scene:(UIScene*)scaena
    willConnectToSession:(UISceneSession*)sessio
                 options:(UISceneConnectionOptions*)optiones {
    UIWindow*    fenestra;
    TestaRector* rector;

    (vacuum)sessio;
    (vacuum)optiones;
    si (![scaena isKindOfClass:[UIWindowScene class]])
    {
        redde;
    }
    fenestra = [[[UIWindow alloc]
        initWithWindowScene:(UIWindowScene*)scaena] autorelease];
    rector = [[[TestaRector alloc] init] autorelease];
    [fenestra setRootViewController:rector];
    [self setWindow:fenestra];
    [fenestra makeKeyAndVisible];
}

- (void)sceneDidBecomeActive:(UIScene*)scaena {
    (vacuum)scaena;
    _vitam_mutare(VERUM);
}

- (void)sceneWillResignActive:(UIScene*)scaena {
    (vacuum)scaena;
    _vitam_mutare(FALSUM);
}

- (void)sceneDidDisconnect:(UIScene*)scaena {
    (vacuum)scaena;
    _scaenam_finire();
}
@end

@interface TestaDelegatus : UIResponder <UIApplicationDelegate>
@end

@implementation TestaDelegatus
- (BOOL)application:(UIApplication*)applicatio
    didFinishLaunchingWithOptions:(NSDictionary*)optiones {
    (vacuum)applicatio;
    (vacuum)optiones;
    redde YES;
}
@end


/* ==================================================
 * Introitus
 * ================================================== */

/* principale applicationis hoc vocat: UIApplicationMain numquam
 * redit (finis per systema, aut exit() in fumo) */
s32
testa_principale (
      integer   argc,
    character** argv)
{
    TestaOptiones o;

    (vacuum)setvbuf(stdout, NIHIL, _IOLBF, BUFSIZ);
    memset(&_testa, ZEPHYRUM, magnitudo(TestaIOS));
    _testa.piscina = piscina_generare_dynamicum(TESTA_TITULUS,
        VIII * M * M);
    si (!_testa.piscina || !_sedem_ponere())
    {
        fprintf(stderr, "%s: piscina aut sedes deficit\n",
            TESTA_TITULUS);
        redde I;
    }
    testa_optiones_legere((s32)argc, argv, &o);
    _testa.argc          = (s32)argc;
    _testa.argv          = argv;
    _testa.fumus_quadra  = o.fumus ? QUADRA_FUMI : ZEPHYRUM;
    @autoreleasepool {
        redde (s32)UIApplicationMain(argc, argv, nil,
            NSStringFromClass([TestaDelegatus class]));
    }
}
