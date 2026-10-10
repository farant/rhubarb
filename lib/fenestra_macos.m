/* Includere systemae primum ut vitare conflictos macros */
#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import <objc/runtime.h>
#import <mach/mach_time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fenestra.h"
#include "eventus_cauda.h"
#include "claves_physicae.h"
#include "utf8.h"

/* Forward declarations pro struct Fenestra */
@class FenestraVisus;
@class FenestraDelegatus;

structura Fenestra {
    Piscina* piscina;
    NSWindow *fenestra_ns;
    FenestraVisus *visus;
    FenestraDelegatus *delegatus;
    EventusCauda cauda;   /* lib/eventus_cauda: anulus + onera per
                           * lectionem (eventus A3) */
    f64 residuum_x;       /* rotula: fractiones nondum emissae */
    f64 residuum_y;
    b32 rotula_praecisa;  /* genus rotulae ultimum (mutatum: residua
                           * vacantur) */
    b32 plena_visio;      /* Status plenae visionis */
    b32 cursor_occultus;  /* Si cursor systematis occultatus */
};

@interface FenestraDelegatus : NSObject <NSWindowDelegate>
@property (assign) BOOL debet_claudere;
@property (assign) Fenestra *fenestra;
- (void)paste:(id)sender;
@end

/* prototypum (definitio infra): delegatus focum et glutinum impellit */
interior vacuum
impellere_eventum (
    Fenestra* fenestra,
    constans Eventus* eventus);

/* Focus fenestrae ut eventus (aemulator D6c: terminale ?1004 relationes
 * CSI I / CSI O). Fenestra clavis fit etiam in creatione: eventus
 * FOCUS primus caudae fere semper. */
interior vacuum
_focum_impellere (
           Fenestra* fenestra,
    eventus_genus_t  genus)
{
    Eventus eventus;

    si (!fenestra)
    {
        redde;
    }
    memset(&eventus, ZEPHYRUM, magnitudo(Eventus));
    eventus.genus = genus;
    impellere_eventum(fenestra, &eventus);
}

@implementation FenestraDelegatus
- (BOOL)windowShouldClose:(NSWindow *)sender {
    self.debet_claudere = YES;
    redde NO; /* Non claudere statim, usorem tractare sinere */
}

- (void)windowDidResize:(NSNotification *)notification {
    /* Tractare eventus mutationis magnitudinis */
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    _focum_impellere(self.fenestra, EVENTUS_FOCUS);
}

- (void)windowDidResignKey:(NSNotification *)notification {
    _focum_impellere(self.fenestra, EVENTUS_DEFOCUS);
}

/* Glutinare (Cmd-V; aemulator D6c). Res menu sine scopo actionem per
 * catenam respondentium mittit - visus primus respondens non fit
 * (claves per perscrutationem veniunt), ergo fenestra deinde delegatus
 * eius: hic. Textus tabulae communis = EVENTUS_TEXTUS origo GLUTINATA
 * (terminale: cancelli ?2004). Onus caudae LXIV KiB per lectionem:
 * longius truncatur (truncatum VERUM). */
- (void)paste:(id)sender {
    NSString*           textus;
    constans character* utf8;

    si (!self.fenestra)
    {
        redde;
    }
    textus = [[NSPasteboard generalPasteboard]
        stringForType:NSPasteboardTypeString];
    utf8 = textus ? [textus UTF8String] : NULL;
    /* sine textu nihil: clavis Cmd+V IPSA iam applicationi venit
     * (perscrutatio NSEventTypeKeyDown ante sendEvent - menu postea);
     * pictor imaginem per eam glutinat. Clavis synthetica hic addita
     * imaginem BIS glutinabat (Franus 2026-10-10). */
    si (!utf8 || utf8[ZEPHYRUM] == '\0')
    {
        redde;
    }
    (vacuum)eventus_caudae_textum_impellere(&self.fenestra->cauda,
        fenestra_tempus_ms(), (constans i8*)utf8, (i32)strlen(utf8),
        EVENTUS_ORIGO_GLUTINATA);
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification {
    /* Intrare plenam visionem. CURSOR NON OCCULTATUR HIC (mos
     * ludi antiquus remotus 2026-07-23): [NSCursor hide] numerum
     * referentiarum tenet - occultatio implicita + re-occultatio
     * per motum muris numerum sine fine augebat, unhide unicum in
     * exitu numquam compensabat -> cursor PERPETUO occultus etiam
     * fenestrato (symptoma fori). Apps immersivae
     * fenestra_occultare_cursorem expresse vocent. */
    self.fenestra->plena_visio = VERUM;
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
    /* Exire plenam visionem - ostendere cursor systematis */
    self.fenestra->plena_visio = FALSUM;
    si (self.fenestra->cursor_occultus)
    {
        [NSCursor unhide];
        self.fenestra->cursor_occultus = FALSUM;
    }
}
@end

@interface FenestraVisus : NSView
@property (assign) Fenestra *fenestra;
@end

@implementation FenestraVisus
- (BOOL)acceptsFirstResponder {
    redde YES;
}

- (void)keyDown:(NSEvent *)event {
    /* Tractare eventus clavis depressae */
}

- (void)keyUp:(NSEvent *)event {
    /* Tractare eventus clavis liberatae */
}

- (void)mouseDown:(NSEvent *)event {
    /* Tractare eventus muris depressi */
}

- (void)mouseUp:(NSEvent *)event {
    /* Tractare eventus muris liberati */
}

- (void)mouseMoved:(NSEvent *)event {
    /* Tractare eventus motus muris */
}

- (void)mouseDragged:(NSEvent *)event {
    /* Tractare eventus tractionis muris */
}

- (void)scrollWheel:(NSEvent *)event {
    /* Tractare eventus rotulae */
}

- (void)drawRect:(NSRect)dirtyRect {
    CGImageRef imago_pixelorum;
    CGContextRef contextus;
    CGRect limites;

    /* Verificare si habemus imaginem pixelorum pingere */
    imago_pixelorum =
        (__bridge CGImageRef)objc_getAssociatedObject(self,
            "imagoPixelorum");

    si (imago_pixelorum)
    {
        /* Obtinere contextum graphicum currentem */
        contextus = [[NSGraphicsContext currentContext] CGContext];

        /* Ponere interpolationem ad vicinum proximum pro pixelis acutis */
        CGContextSetInterpolationQuality(contextus,
            kCGInterpolationNone);

        /* Pingere imaginem scalatam ad implendum visum */
        limites = NSRectToCGRect(self.bounds);
        CGContextDrawImage(contextus, limites, imago_pixelorum);
    }
    alioquin
    {
        /* Fundum nigrum ordinarium */
        [[NSColor blackColor] setFill];
        NSRectFill(dirtyRect);
    }
}
@end

interior clavis_t
convertere_clavem (
    insignatus brevis codex_clavis)
{
    commutatio (codex_clavis)
    {
        /* Claves speciales */
        casus LIII: redde CLAVIS_EFFUGIUM;
        casus XXXVI: redde CLAVIS_REDITUS;
        casus XLVIII: redde CLAVIS_TABULA;
        casus LI: redde CLAVIS_RETRORSUM;
        casus CXVII: redde CLAVIS_DELERE;
        casus XLIX: redde CLAVIS_SPATIUM;

        /* Claves sagittae */
        casus CXXIII: redde CLAVIS_SINISTER;
        casus CXXIV: redde CLAVIS_DEXTER;
        casus CXXVI: redde CLAVIS_SURSUM;
        casus CXXV: redde CLAVIS_DEORSUM;

        /* Claves navigationis */
        casus CXV: redde CLAVIS_DOMUS;
        casus CXIX: redde CLAVIS_FINIS;
        casus CXVI: redde CLAVIS_PAGINA_SURSUM;
        casus CXXI: redde CLAVIS_PAGINA_DEORSUM;

        /* Claves functionis */
        casus CXXII: redde CLAVIS_F1;
        casus CXX: redde CLAVIS_F2;
        casus XCIX: redde CLAVIS_F3;
        casus CXVIII: redde CLAVIS_F4;
        casus XCVI: redde CLAVIS_F5;
        casus XCVII: redde CLAVIS_F6;
        casus XCVIII: redde CLAVIS_F7;
        casus C: redde CLAVIS_F8;
        casus CI: redde CLAVIS_F9;
        casus CIX: redde CLAVIS_F10;
        casus CIII: redde CLAVIS_F11;
        casus CXI: redde CLAVIS_F12;

        /* Claves modificantes */
        casus LVI: redde CLAVIS_SINISTER_SHIFT;
        casus LX: redde CLAVIS_DEXTER_SHIFT;
        casus LIX: redde CLAVIS_SINISTER_IMPERIUM;
        casus LXII: redde CLAVIS_DEXTER_IMPERIUM;
        casus LVIII: redde CLAVIS_SINISTER_ALT;
        casus LXI: redde CLAVIS_DEXTER_ALT;
        casus LV: redde CLAVIS_SINISTER_SUPER;
        casus LIV: redde CLAVIS_DEXTER_SUPER;
        casus LVII: redde CLAVIS_CAPS_LOCK;

        /* Claves litterarum A-Z */
        casus ZEPHYRUM: redde 'A';
        casus XI: redde 'B';
        casus VIII: redde 'C';
        casus II: redde 'D';
        casus XIV: redde 'E';
        casus III: redde 'F';
        casus V: redde 'G';
        casus IV: redde 'H';
        casus XXXIV: redde 'I';
        casus XXXVIII: redde 'J';
        casus XL: redde 'K';
        casus XXXVII: redde 'L';
        casus XLVI: redde 'M';
        casus XLV: redde 'N';
        casus XXXI: redde 'O';
        casus XXXV: redde 'P';
        casus XII: redde 'Q';
        casus XV: redde 'R';
        casus I: redde 'S';
        casus XVII: redde 'T';
        casus XXXII: redde 'U';
        casus IX: redde 'V';
        casus XIII: redde 'W';
        casus VII: redde 'X';
        casus XVI: redde 'Y';
        casus VI: redde 'Z';

        /* Claves numerorum 0-9 */
        casus XVIII: redde '1';
        casus XIX: redde '2';
        casus XX: redde '3';
        casus XXI: redde '4';
        casus XXIII: redde '5';
        casus XXII: redde '6';
        casus XXVI: redde '7';
        casus XXVIII: redde '8';
        casus XXV: redde '9';
        casus XXIX: redde '0';

        /* Punctuatio et claves communes aliae */
        casus XXVII: redde '-';
        casus XXIV: redde '=';
        casus XXXIII: redde '[';
        casus XXX: redde ']';
        casus XLII: redde '\\';
        casus XLI: redde ';';
        casus XXXIX: redde '\'';
        casus XLIII: redde ',';
        casus XLVII: redde '.';
        casus XLIV: redde '/';
        casus L: redde '`';

        /* Pro clave non mappata, reddere codicem clavem crudum + 1000 */
        ordinarius: redde M + codex_clavis;
    }
}

/* prototypum (definitio post fenestra_creare - vocatur in bloco
 * initii NSApp) */
interior vacuum
_menu_ordinarium_ponere (vacuum);

interior vacuum
impellere_eventum (
    Fenestra* fenestra,
    constans Eventus* eventus)
{
    Eventus e;

    /* Stampa UNICA: eventa omnia (NSEvent, immittere) hic transeunt.
     * Tempus a vocatore datum (replay, immittere) servatur. Cauda plena:
     * eventus abicitur (amissa numerantur). */
    e = *eventus;
    si (e.tempus == ZEPHYRUM) { e.tempus = fenestra_tempus_ms(); }
    si (e.genus == EVENTUS_MUS_MOTUS)
    {
        /* motus coalescit per lectionem cum exemplis (eventus A3b) */
        (vacuum)eventus_caudae_motum_impellere(&fenestra->cauda, &e);
    }
    alioquin
    {
        (vacuum)eventus_caudae_impellere(&fenestra->cauda, &e);
    }
}

interior b32
extrahere_eventum (
    Fenestra* fenestra,
    Eventus* eventus)
{
    redde eventus_caudae_extrahere(&fenestra->cauda, eventus);
}

/* Scopus rei menu: pressio in EVENTUS_MENU fenestrae suae vertitur -
 * nulla revocatio C, eventus per caudam ut claves. Res menu scopum
 * NON retinet (target assign), ergo scopus numquam dimittitur: vivit
 * quamdiu applicatio (res menu non removentur). */
@interface FenestraMenuScopus : NSObject
@property (assign) Fenestra *fenestra;
- (void)pressa:(id)sender;
@end

@implementation FenestraMenuScopus
- (void)pressa:(id)sender {
    Eventus eventus;

    memset(&eventus, 0, magnitudo(eventus));
    eventus.genus              = EVENTUS_MENU;
    eventus.datum.menu.signum  = (i32)[sender tag];
    impellere_eventum(self.fenestra, &eventus);
}
@end

/* Tempus eventus EX NSEvent (eventus A3b): [NSEvent timestamp] =
 * secundae ab initio systematis, horologium IDEM ac mach_absolute_time
 * (mensuratum 2026-10-01: differentia 0.003 ms) - ergo ms nostra
 * directe. Ante: tempus LECTIONIS stampabatur, eventa omnia lectionis
 * unius idem fere tempus ferebant (exemplum 3 ms ante eventum, XVIII
 * pixela distans). 0 (eventus sine tempore) -> horologium nunc. */
interior s64
_tempus_eventus (
    NSEvent* eventus_ns)
{
    NSTimeInterval secundae = [eventus_ns timestamp];

    si (secundae <= 0.0)
    {
        redde fenestra_tempus_ms();
    }
    redde (s64)(secundae * 1000.0);
}

/* Gradus rotulae: pixela NOSTRA per lineam quam rota sine praecisione
 * nuntiat (scrollingDelta in lineis). XVI = linea una litterarum
 * domus. Facultas publicata (gradus_rotulae), non lex: consumens qui
 * gradus vult dy / gradus_rotulae computat. */
#define FENESTRA_GRADUS_ROTULAE  XVI

/* Facultates fenestrae macOS (spec Q4, Q17) - eventus PRIMUS caudae.
 * praeeditio FALSUM donec NSTextInputClient (park 004); depositio
 * NULLA donec implementata; pressio FALSUM: mus pressionem non
 * nuntiat (stilus/Force Touch postea). */
interior vacuum
_facultates_impellere (
    Fenestra* fenestra)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_FACULTATES;
    e.datum.facultates.liberationes      = VERUM;
    e.datum.facultates.codex_physicus    = VERUM;
    e.datum.facultates.tabula_distincta  = VERUM;
    e.datum.facultates.latera            = VERUM;
    e.datum.facultates.super             = VERUM;
    e.datum.facultates.praeeditio        = FALSUM;
    e.datum.facultates.scriptura_copiae  = EVENTUS_FACULTAS_CERTA;
    e.datum.facultates.depositio         = EVENTUS_DEPOSITIO_NULLA;
    e.datum.facultates.gradus_rotulae    = FENESTRA_GRADUS_ROTULAE;
    e.datum.facultates.pressio           = FALSUM;
    e.datum.facultates.modificantes_textus = VERUM;   /* B4 */
    impellere_eventum(fenestra, &e);
}

/* Scala pixelorum NOSTRORUM per punctum fenestrae: tabula pixelorum
 * praesentata / visus contenti (1.0 si nulla tabula adhuc). */
interior vacuum
_scalam_obtinere (
    Fenestra* fenestra,
         f64* scala_x,
         f64* scala_y)
{
        NSRect  contentum;
    CGImageRef  imago;

    contentum  = [[fenestra->fenestra_ns contentView] frame];
    imago      = (__bridge CGImageRef)objc_getAssociatedObject(
        fenestra->visus, "imagoPixelorum");
    *scala_x = 1.0;
    *scala_y = 1.0;
    si (imago && contentum.size.width > 0.0
        && contentum.size.height > 0.0)
    {
        *scala_x = (f64)CGImageGetWidth(imago) / contentum.size.width;
        *scala_y = (f64)CGImageGetHeight(imago) / contentum.size.height;
    }
}

/* Positio indicatoris in pixelis NOSTRIS (origo summa sinistra).
 * locationInWindow ad fenestram EVENTUS refertur - et eventus sine
 * fenestra (motus extra fenestram clavem) coordinatas SCREENI fert
 * (vitium mensuratum in aspectu A3a: saltus 743,16 -> 96,363). Ergo
 * per screenum in fenestram NOSTRAM semper convertitur. Extra
 * contentum: positio VERA (s32, eventus A3c - Franus 2026-10-01):
 * negativa sinistrorsum/sursum, >= latitudo/altitudo dextrorsum/
 * deorsum. floor, non truncatio: -0.5 extra est (pixelum -1), non 0.
 * Pressio IGNOTA (mus). */
interior vacuum
_positionem (
    Fenestra* fenestra,
     NSEvent* eventus_ns,
         s32* x_ex,
         s32* y_ex)
{
    NSPoint  punctum;
     NSRect  rectum;
     NSRect  contentum;
        f64  x;
        f64  y;
        f64  scala_x;
        f64  scala_y;

    punctum = [eventus_ns locationInWindow];
    si ([eventus_ns window] != fenestra->fenestra_ns)
    {
        rectum = NSMakeRect(punctum.x, punctum.y, 0.0, 0.0);
        si ([eventus_ns window] != nil)
        {
            rectum = [[eventus_ns window] convertRectToScreen:rectum];
        }
        punctum = [fenestra->fenestra_ns convertRectFromScreen:rectum]
            .origin;
    }
    contentum = [[fenestra->fenestra_ns contentView] frame];
    _scalam_obtinere(fenestra, &scala_x, &scala_y);
    x = punctum.x * scala_x;
    y = (contentum.size.height - punctum.y) * scala_y;
    *x_ex = (s32)floor(x);
    *y_ex = (s32)floor(y);
}

interior vacuum
_murem_implere (
    Fenestra* fenestra,
     NSEvent* eventus_ns,
     Eventus* eventus)
{
    _positionem(fenestra, eventus_ns, &eventus->datum.mus.x,
        &eventus->datum.mus.y);
    eventus->datum.mus.modificantes  = (i32)[eventus_ns modifierFlags];
    eventus->datum.mus.indicator     = ZEPHYRUM;
    eventus->datum.mus.indicator_genus = EVENTUS_INDICATOR_MUS;
    eventus->datum.mus.pressio       = EVENTUS_PRESSIO_IGNOTA;
    eventus->tempus                  = _tempus_eventus(eventus_ns);
}

/* Rotula integra (spec Q14): trackpad (hasPreciseScrollingDeltas) ->
 * PRAECISA, puncta in pixela nostra scalata; rota -> GRADATA, lineae
 * x FENESTRA_GRADUS_ROTULAE. Fractiones in residuis fenestrae manent
 * (eventus_residuum_integrare); genere mutato vacantur (S3b: delta_x/y
 * f32 deleta). Signum: idem ac scrollingDelta. */
interior vacuum
_rotulam_implere (
    Fenestra* fenestra,
     NSEvent* eventus_ns,
     Eventus* eventus)
{
    b32  praecisa;
    f64  scala_x;
    f64  scala_y;

    eventus->tempus = _tempus_eventus(eventus_ns);
    /* B3a: positio indicatoris et modificantes (shift+rota, zoom) */
    _positionem(fenestra, eventus_ns, &eventus->datum.rotula.x,
        &eventus->datum.rotula.y);
    eventus->datum.rotula.modificantes =
        (i32)[eventus_ns modifierFlags];
    praecisa = [eventus_ns hasPreciseScrollingDeltas] ? VERUM : FALSUM;
    si (praecisa != fenestra->rotula_praecisa)
    {
        fenestra->residuum_x       = 0.0;
        fenestra->residuum_y       = 0.0;
        fenestra->rotula_praecisa  = praecisa;
    }
    si (praecisa)
    {
        _scalam_obtinere(fenestra, &scala_x, &scala_y);
        eventus->datum.rotula.genus = EVENTUS_ROTULA_PRAECISA;
        eventus->datum.rotula.dx = eventus_residuum_integrare(
            &fenestra->residuum_x,
                [eventus_ns scrollingDeltaX] * scala_x);
        eventus->datum.rotula.dy = eventus_residuum_integrare(
            &fenestra->residuum_y,
                [eventus_ns scrollingDeltaY] * scala_y);
    }
    alioquin
    {
        eventus->datum.rotula.genus = EVENTUS_ROTULA_GRADATA;
        eventus->datum.rotula.dx = FENESTRA_GRADUS_ROTULAE
            * eventus_residuum_integrare(&fenestra->residuum_x,
                  [eventus_ns scrollingDeltaX]);
        eventus->datum.rotula.dy = FENESTRA_GRADUS_ROTULAE
            * eventus_residuum_integrare(&fenestra->residuum_y,
                  [eventus_ns scrollingDeltaY]);
    }
}

Fenestra*
fenestra_creare (
    Piscina*                       piscina,
    constans FenestraConfiguratio* configuratio)
{
    Fenestra *fenestra;
    NSWindowStyleMask mamma_styli;
    ProcessSerialNumber psn;
    NSRect forma;

    @autoreleasepool {
        /* Initializare NSApplication si necessarium */
        si (!NSApp)
        {
            /* Disablere objecta zombie in modo liberationis */
            setenv("NSZombieEnabled", "NO", 1);

            [NSApplication sharedApplication];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

            /* Assecurare nos non in modo terminali solum */
            psn.highLongOfPSN = 0;
            psn.lowLongOfPSN = kCurrentProcess;
            TransformProcessType(&psn,
                kProcessTransformToForegroundApplication);

            _menu_ordinarium_ponere();

            /* Coalitio motus AppKit EXTINGUITUR (eventus A3b): ea
             * motus in cauda systematis iungit et intermedios ABICIT
             * (aspectus: saltus 461,143 -> 374,0 sine exemplo). Nos
             * per lectionem coalescimus cum exemplis (spec Q13, D5) -
             * ergo omnia exempla hardware nobis veniant. */
            [NSEvent setMouseCoalescingEnabled:NO];
        }

        /* ordinatum: piscina_allocare octetis compactum est (alineatio
         * I) - structura monstratores et s64 fert (eventus A3) */
        fenestra = piscina_allocare_ordinatum(piscina,
            magnitudo(Fenestra), VIII);
        si (!fenestra) redde NIHIL;

        fenestra->piscina          = piscina;
        fenestra->residuum_x       = 0.0;
        fenestra->residuum_y       = 0.0;
        fenestra->rotula_praecisa  = FALSUM;
        fenestra->plena_visio      = FALSUM;
        fenestra->cursor_occultus  = FALSUM;
        eventus_caudam_initiare(&fenestra->cauda);
        _facultates_impellere(fenestra);

        /* Creare masquam styli fenestrae */
        mamma_styli = 0;
        si (configuratio->vexilla & FENESTRA_CLAUDIBILIS)
        {
            mamma_styli |= NSWindowStyleMaskClosable;
        }
        si (configuratio->vexilla & FENESTRA_MINUIBILIS)
        {
            mamma_styli |= NSWindowStyleMaskMiniaturizable;
        }
        si (configuratio->vexilla & FENESTRA_MUTABILIS)
        {
            mamma_styli |= NSWindowStyleMaskResizable;
        }
        mamma_styli |= NSWindowStyleMaskTitled;

        /* Creare fenestram */
        forma = NSMakeRect(configuratio->x, configuratio->y,
                          configuratio->latitudo,
                              configuratio->altitudo);
        fenestra->fenestra_ns =
            [[NSWindow alloc] initWithContentRect:forma
                                                            styleMask:mamma_styli
                                                              backing:NSBackingStoreBuffered
                                                                defer:NO];

        /* Ponere titulum fenestrae */
        si (configuratio->titulus)
        {
            [fenestra->fenestra_ns setTitle:[NSString stringWithUTF8String:configuratio->titulus]];
        }

        /* Creare et ponere visum */
        fenestra->visus = [[FenestraVisus alloc] initWithFrame:forma];
        fenestra->visus.fenestra = fenestra;
        [fenestra->fenestra_ns setContentView:fenestra->visus];

        /* Creare et ponere delegatum */
        fenestra->delegatus = [[FenestraDelegatus alloc] init];
        fenestra->delegatus.debet_claudere = NO;
        fenestra->delegatus.fenestra = fenestra;
        [fenestra->fenestra_ns setDelegate:fenestra->delegatus];

        /* Configurare aspectum fenestrae */
        [fenestra->fenestra_ns setAcceptsMouseMovedEvents:YES];
        [fenestra->fenestra_ns setReleasedWhenClosed:NO];

        /* Centrare si petitum */
        si (configuratio->vexilla & FENESTRA_CENTRATA)
        {
            [fenestra->fenestra_ns center];
        }

        /* Facere fenestram visibilem et clavem (app-localiter) */
        [fenestra->fenestra_ns makeKeyAndOrderFront:nil];

        /* Activare applicationem - NISI RETRO: raptus foci systematis
         * in hac una linea vivit; sine ea fenestra apparet, claves
         * app-locales perveniunt, sed usor scribens non interpellatur. */
        si (!(configuratio->vexilla & FENESTRA_RETRO))
        {
            [NSApp activateIgnoringOtherApps:YES];
        }

        /* Intrare plenam visionem si petitum */
        si (configuratio->vexilla & FENESTRA_PLENA_VISIO)
        {
            [fenestra->fenestra_ns toggleFullScreen:nil];
        }

        redde fenestra;
    }
}

/* Menu ordinarium minimum (App + Emendare): executabile nudum sine
 * menu aequivalentia clavium caret - Cmd+C/V/X/A mortua in quovis
 * campo textus (webview aut NSTextField). Selectores vulgares
 * responsorem primum petunt, ergo tituli Latini libere licent.
 * Constructum semel ad initium NSApp; res app-gradus, numquam
 * vitreae (verdictum interrogationis 2026-07-16). MRC: mainMenu et
 * addItem/setSubmenu retinent - nostra post insertionem
 * dimittuntur. */
interior vacuum
_menu_ordinarium_ponere (vacuum)
{
    NSMenu* praecipuum = [[NSMenu alloc] init];
    NSMenuItem* app_sedes = [[NSMenuItem alloc] init];
    NSMenu* app_menu = [[NSMenu alloc] init];
    NSMenuItem* emendare_sedes = [[NSMenuItem alloc] init];
    NSMenu* emendare_menu =
        [[NSMenu alloc] initWithTitle:@"Emendare"];

    [app_menu addItemWithTitle:@"Exire"
                        action:@selector(terminate:)
                 keyEquivalent:@"q"];
    [app_sedes setSubmenu:app_menu];
    [praecipuum addItem:app_sedes];

    [emendare_menu addItemWithTitle:@"Revocare"
                             action:@selector(undo:)
                      keyEquivalent:@"z"];
    [emendare_menu addItemWithTitle:@"Iterare"
                             action:@selector(redo:)
                      keyEquivalent:@"Z"];
    [emendare_menu addItem:[NSMenuItem separatorItem]];
    [emendare_menu addItemWithTitle:@"Secare"
                             action:@selector(cut:)
                      keyEquivalent:@"x"];
    [emendare_menu addItemWithTitle:@"Copiare"
                             action:@selector(copy:)
                      keyEquivalent:@"c"];
    [emendare_menu addItemWithTitle:@"Glutinare"
                             action:@selector(paste:)
                      keyEquivalent:@"v"];
    [emendare_menu addItemWithTitle:@"Omnia Eligere"
                             action:@selector(selectAll:)
                      keyEquivalent:@"a"];
    [emendare_sedes setSubmenu:emendare_menu];
    [praecipuum addItem:emendare_sedes];

    [NSApp setMainMenu:praecipuum];
    [app_menu release];
    [app_sedes release];
    [emendare_menu release];
    [emendare_sedes release];
    [praecipuum release];
}

vacuum
fenestra_destruere (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns close];
        [fenestra->delegatus release];
        [fenestra->visus release];
        [fenestra->fenestra_ns release];
        /* Non liberare(fenestra) - piscina possidet memoriam */
    }
}

b32
fenestra_debet_claudere (
    constans Fenestra* fenestra)
{
    si (!fenestra) redde VERUM;
    redde fenestra->delegatus.debet_claudere;
}

/* Runa quam dispositio PRAESENS pro clave dat SINE modificatoribus
 * (Shift quoque: spec Q9 'clavis logica'). charactersIgnoringModifiers
 * Shift NON ignorat ("A" pro Shift+a, "!" pro Shift+1) - ergo
 * charactersByApplyingModifiers:0 (AppKit, macOS 10.15+; NON Carbon:
 * UCKeyTranslate -framework Carbon poscebat, quod octo scripta
 * nexus manu scripta - silex, briar spectator inter ea - tangeret).
 * Regimen et zona privata Apple (F700..F8FF: sagittae, functiones) ->
 * 0 (clavis nominata, non runa). */
/* Character a clave productus (S3a; olim 'typus' = characters[0] in
 * octetum truncatum): punctum Unicode primum characterum (paria
 * surrogata iuncta); claves functionis AppKit (0xF700-0xF8FF, usus
 * privatus) nullum characterem producunt -> 0. */
interior s32
_productam (
    NSString* characteres)
{
    unichar u;
    s32     cp;

    si ([characteres length] == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    u   = [characteres characterAtIndex:ZEPHYRUM];
    cp  = (s32)u;
    si (u >= 0xD800 && u <= 0xDBFF && [characteres length] > I)
    {
        unichar humilis = [characteres characterAtIndex:I];

        si (humilis >= 0xDC00 && humilis <= 0xDFFF)
        {
            cp = 0x10000 + (((s32)u - 0xD800) << X)
                + ((s32)humilis - 0xDC00);
        }
    }
    si (cp >= 0xF700 && cp <= 0xF8FF)
    {
        redde ZEPHYRUM;
    }
    redde cp;
}

interior s32
_runa_sine_maiuscula (
    NSEvent* eventus_ns)
{
               NSString* nudi;
     constans character* utf8;
            constans i8* p;
                    s32  runa;

    si (   [eventus_ns type] != NSEventTypeKeyDown
        && [eventus_ns type] != NSEventTypeKeyUp)
    {
        redde ZEPHYRUM;
    }
    nudi = [eventus_ns charactersByApplyingModifiers:0];
    si (nudi == nil || [nudi length] == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    utf8 = [nudi UTF8String];
    si (utf8 == NULL)
    {
        redde ZEPHYRUM;
    }
    p     = (constans i8*)utf8;
    runa  = utf8_decodere(&p, p + strlen(utf8));
    si (runa < 0x20 || runa == 0x7F
        || (runa >= 0xF700 && runa <= 0xF8FF))
    {
        redde ZEPHYRUM;
    }
    redde runa;
}

/* Eventum clavis (eventus A3a): logica (clavis, runa), physica (codex),
 * actio (ITERATA ex isARepeat; SOLUTA in liberatione), latera (in
 * modifierFlags crudis iam). typus servatur (spec D2 gradus I). Post
 * depressionem: EVENTUS_TEXTUS COMMISSUM (eodem tempore) si characteres
 * TEXTUS sunt - non regimen, non zona privata (sagittae), non sub Cmd
 * aut Ctrl (brevitates, non scriptura). */
interior vacuum
_clavem_impellere (
    Fenestra* fenestra,
     NSEvent* eventus_ns,
         b32  depressa)
{
              Eventus  eventus;
            NSString* characteres;
                  s64  tempus;
                  i32  modi;

    memset(&eventus, ZEPHYRUM, magnitudo(Eventus));
    tempus  = _tempus_eventus(eventus_ns);
    modi    = (i32)[eventus_ns modifierFlags];
    eventus.genus   = depressa ? EVENTUS_CLAVIS_DEPRESSUS
                               : EVENTUS_CLAVIS_LIBERATUS;
    eventus.tempus  = tempus;
    eventus.datum.clavis.clavis        = convertere_clavem(
        [eventus_ns keyCode]);
    eventus.datum.clavis.modificantes  = modi;
    eventus.datum.clavis.codex         = claves_codex_ex_macos(
        (s32)[eventus_ns keyCode]);
    eventus.datum.clavis.runa          = _runa_sine_maiuscula(
        eventus_ns);
    eventus.datum.clavis.actio = !depressa ? EVENTUS_ACTIO_SOLUTA
        : ([eventus_ns isARepeat] ? EVENTUS_ACTIO_ITERATA
                                  : EVENTUS_ACTIO_PRESSA);
    characteres = [eventus_ns characters];
    eventus.datum.clavis.producta = _productam(characteres);
    impellere_eventum(fenestra, &eventus);

    si (   depressa && [characteres length] > ZEPHYRUM
        && (modi & (MOD_SUPER | MOD_IMPERIUM)) == ZEPHYRUM)
    {
        constans character* utf8 = [characteres UTF8String];
        constans i8*        p    = (constans i8*)utf8;
        constans i8*        finis;
                       s32  prima;

        si (utf8 == NULL)
        {
            redde;
        }
        finis  = p + strlen(utf8);
        prima  = utf8_decodere(&p, finis);
        si (   prima >= 0x20 && prima != 0x7F
            && !(prima >= 0xF700 && prima <= 0xF8FF))
        {
            (vacuum)eventus_caudae_textum_impellere(&fenestra->cauda,
                tempus, (constans i8*)utf8, (i32)strlen(utf8),
                EVENTUS_ORIGO_SCRIPTA);
        }
    }
}

vacuum
fenestra_perscrutari_eventus (
    Fenestra* fenestra)
{
    NSEvent *eventus_ns;
    interior NSSize magnitudo_ultima = {0};
    NSSize magnitudo_currens;

    /* lectio nova: onera (textus) vacantur si cauda vacua (visus
     * eventuum extractorum consumpti; eventus A3) */
    eventus_cauda_lectio_incipit(&fenestra->cauda);

    @autoreleasepool {

        dum ((eventus_ns = [NSApp nextEventMatchingMask:NSEventMaskAny
                                               untilDate:[NSDate distantPast]
                                                  inMode:NSDefaultRunLoopMode
                                                 dequeue:YES]))
        {

            Eventus eventus;

            /* totum nullum (unio: {0} solum membrum primum ponit) */
            memset(&eventus, ZEPHYRUM, magnitudo(Eventus));
            commutatio ([eventus_ns type])
            {
                ordinarius:
                    frange;

                casus NSEventTypeApplicationDefined:
                    /* eventum excitationis syntheticum (contractus
                     * vitreae: nuntius pontis in cauda positus
                     * pumpam obstructam expergefacere debet -
                     * postEvent in tractatore nuntii). VORATUR:
                     * nec Eventus nec sendEvent - nulli
                     * respondenti destinatum est. */
                    perge;

                casus NSEventTypeKeyDown:
                    _clavem_impellere(fenestra, eventus_ns, VERUM);
                    frange;

                casus NSEventTypeKeyUp:
                    _clavem_impellere(fenestra, eventus_ns, FALSUM);
                    frange;

                casus NSEventTypeLeftMouseDown:
                casus NSEventTypeRightMouseDown:
                casus NSEventTypeOtherMouseDown:
                    eventus.genus = EVENTUS_MUS_DEPRESSUS;
                    _murem_implere(fenestra, eventus_ns, &eventus);
                    eventus.datum.mus.botton =
                        ([eventus_ns type] == NSEventTypeLeftMouseDown)
                            ? MUS_SINISTER
                        : ([eventus_ns type]
                            == NSEventTypeRightMouseDown)
                            ? MUS_DEXTER : MUS_MEDIUS;
                    impellere_eventum(fenestra, &eventus);
                    frange;

                casus NSEventTypeLeftMouseUp:
                casus NSEventTypeRightMouseUp:
                casus NSEventTypeOtherMouseUp:
                    eventus.genus = EVENTUS_MUS_LIBERATUS;
                    _murem_implere(fenestra, eventus_ns, &eventus);
                    eventus.datum.mus.botton =
                        ([eventus_ns type] == NSEventTypeLeftMouseUp)
                            ? MUS_SINISTER
                        : ([eventus_ns type] == NSEventTypeRightMouseUp)
                            ? MUS_DEXTER : MUS_MEDIUS;
                    impellere_eventum(fenestra, &eventus);
                    frange;

                /* re-occultatio cursoris per motum REMOTA (2026-07-23):
                 * hide iteratum numerum referentiarum NSCursor
                 * inflabat - fons cursoris perpetuo occulti */
                casus NSEventTypeMouseMoved:
                casus NSEventTypeLeftMouseDragged:
                casus NSEventTypeRightMouseDragged:
                casus NSEventTypeOtherMouseDragged:
                    eventus.genus = EVENTUS_MUS_MOTUS;
                    _murem_implere(fenestra, eventus_ns, &eventus);
                    /* tractus: botton tentus (spec D1, ut interpres et
                     * manus_ludus). Olim 0 semper: tractus ut motus
                     * nudus - tmux ?1002 nihil, Claude Code ?1003
                     * 'super' tantum (aemulator D7, Franus) */
                    eventus.datum.mus.botton =
                        ([eventus_ns type] == NSEventTypeLeftMouseDragged)
                            ? MUS_SINISTER
                        : ([eventus_ns type]
                            == NSEventTypeRightMouseDragged)
                            ? MUS_DEXTER
                        : ([eventus_ns type]
                            == NSEventTypeOtherMouseDragged)
                            ? MUS_MEDIUS : (mus_botton_t)ZEPHYRUM;
                    impellere_eventum(fenestra, &eventus);
                    frange;

                casus NSEventTypeScrollWheel:
                    eventus.genus = EVENTUS_MUS_ROTULA;
                    _rotulam_implere(fenestra, eventus_ns, &eventus);
                    impellere_eventum(fenestra, &eventus);
                    frange;
            }

            [NSApp sendEvent:eventus_ns];
        }

        /* Verificare pro claudendo fenestrae */
        si (fenestra->delegatus.debet_claudere)
        {
            Eventus eventus_claudendi = {ZEPHYRUM};
            eventus_claudendi.genus = EVENTUS_CLAUDERE;
            impellere_eventum(fenestra, &eventus_claudendi);
        }

        /* Verificare pro mutatione magnitudinis */
        /* 013 B3b: magnitudo CONTENTI (sine titulo), non quadri
         * fenestrae - tabula ex contento creata est */
        magnitudo_currens = [fenestra->fenestra_ns contentRectForFrameRect:
            [fenestra->fenestra_ns frame]].size;
        si (magnitudo_currens.width != magnitudo_ultima.width ||
            magnitudo_currens.height != magnitudo_ultima.height)
        {
            Eventus eventus_mutationis = {ZEPHYRUM};
            eventus_mutationis.genus = EVENTUS_MUTARE_MAGNITUDINEM;
            eventus_mutationis.datum.mutare_magnitudinem.latitudo =
                (i32)magnitudo_currens.width;
            eventus_mutationis.datum.mutare_magnitudinem.altitudo =
                (i32)magnitudo_currens.height;
            impellere_eventum(fenestra, &eventus_mutationis);
            magnitudo_ultima = magnitudo_currens;
        }
    }
}

vacuum
fenestra_expectare_eventus (
    Fenestra* fenestra,
    Mora ms_maximae)
{
    @autoreleasepool {
        NSEvent* primus;
        NSTimeInterval secundae;

        si (!fenestra) redde;
        secundae = (NSTimeInterval)ms_maximae / 1000.0;
        si (secundae < 0.0)
        {
            secundae = 0.0;
        }
        /* obstructio semel: morari donec eventus adveniat aut
         * tempus exhauriatur. Fontes runloop (nuntii scripti
         * WebKit, tempora) INTER moras serviuntur; nuntius pontis
         * per se pumpam NON expergefacit - tractator eius eventum
         * syntheticum ApplicationDefined ponit (foramen + remedium
         * in ferro probata, vitrea-calibratio.md). */
        primus = [NSApp nextEventMatchingMask:NSEventMaskAny
            untilDate:[NSDate dateWithTimeIntervalSinceNow:secundae]
            inMode:NSDefaultRunLoopMode
            dequeue:YES];
        si (primus)
        {
            /* in caput caudae reponere - perscrutari infra eum cum
             * ceteris ordine translatabit. Fluxus translationis
             * UNUS manet (nulla duplicatio status magnitudinis aut
             * commutationis). */
            [NSApp postEvent:primus atStart:YES];
        }
    }
    fenestra_perscrutari_eventus(fenestra);
}

b32
fenestra_obtinere_eventus (
    Fenestra* fenestra,
    Eventus* eventus)
{
    si (!fenestra || !eventus) redde FALSUM;
    redde extrahere_eventum(fenestra, eventus);
}

vacuum
fenestra_ponere_titulum (
    Fenestra* fenestra,
    constans character* titulus)
{
    si (!fenestra || !titulus) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns setTitle:[NSString stringWithUTF8String:titulus]];
    }
}

vacuum
fenestra_obtinere_magnitudinem (
    constans Fenestra* fenestra,
    i32* latitudo,
    i32* altitudo)
{
    NSRect forma;

    si (!fenestra) redde;

    forma =
        [fenestra->fenestra_ns contentRectForFrameRect:[fenestra->fenestra_ns frame]];
    si (latitudo) *latitudo = (i32)forma.size.width;
    si (altitudo) *altitudo = (i32)forma.size.height;
}

vacuum
fenestra_ponere_magnitudinem (
    Fenestra* fenestra,
    i32 latitudo,
    i32 altitudo)
{
    si (!fenestra) redde;

    @autoreleasepool {
        NSRect forma = [fenestra->fenestra_ns frame];
        forma.size =
            [fenestra->fenestra_ns frameRectForContentRect:NSMakeRect(0,
                0, latitudo, altitudo)].size;
        [fenestra->fenestra_ns setFrame:forma display:YES];
    }
}

vacuum
fenestra_obtinere_positum (
    constans Fenestra* fenestra,
    i32* x,
    i32* y)
{
    NSRect forma;

    si (!fenestra) redde;

    forma = [fenestra->fenestra_ns frame];
    si (x) *x = (i32)forma.origin.x;
    si (y) *y = (i32)forma.origin.y;
}

vacuum
fenestra_ponere_positum (
    Fenestra* fenestra,
    i32 x,
    i32 y)
{
    si (!fenestra) redde;

    @autoreleasepool {
        NSRect forma = [fenestra->fenestra_ns frame];
        forma.origin.x = x;
        forma.origin.y = y;
        [fenestra->fenestra_ns setFrame:forma display:YES];
    }
}

vacuum
fenestra_monstrare (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns makeKeyAndOrderFront:nil];
    }
}

vacuum
fenestra_celare (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns orderOut:nil];
    }
}

vacuum
fenestra_focus (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns makeKeyWindow];
    }
}

b32
fenestra_est_visibilis (
    constans Fenestra* fenestra)
{
    si (!fenestra) redde FALSUM;
    redde [fenestra->fenestra_ns isVisible];
}

b32
fenestra_habet_focus (
    constans Fenestra* fenestra)
{
    si (!fenestra) redde FALSUM;
    redde [fenestra->fenestra_ns isKeyWindow];
}

vacuum
fenestra_centrare (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns center];
    }
}

vacuum
fenestra_maximizare (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns zoom:nil];
    }
}

b32
fenestra_spatium_utile (
    i32* x,
    i32* y,
    i32* latitudo,
    i32* altitudo)
{
    NSScreen* schirmus;
    NSRect    visibile;
    NSRect    contentum;

    si (!x || !y || !latitudo || !altitudo)
    {
        redde FALSUM;
    }
    @autoreleasepool {
        /* principalis = cum linea menuum (prima); mainScreen sine
         * fenestra clavi idem reddit */
        schirmus = [[NSScreen screens] count] > ZEPHYRUM
                 ? [[NSScreen screens] objectAtIndex:ZEPHYRUM]
                 : [NSScreen mainScreen];
        si (!schirmus)
        {
            redde FALSUM;
        }
        visibile  = [schirmus visibleFrame];
        /* styli fenestrae ordinariae: titulus subtrahitur */
        contentum = [NSWindow contentRectForFrameRect:visibile
            styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                       | NSWindowStyleMaskMiniaturizable
                       | NSWindowStyleMaskResizable)];
        *x         = (i32)contentum.origin.x;
        *y         = (i32)contentum.origin.y;
        *latitudo  = (i32)contentum.size.width;
        *altitudo  = (i32)contentum.size.height;
    }
    redde *latitudo > ZEPHYRUM && *altitudo > ZEPHYRUM;
}

b32
fenestra_spatium_schirmi (
    i32* latitudo,
    i32* altitudo)
{
    NSScreen* schirmus;
    NSRect    totum;
    CGFloat   incisura;

    si (!latitudo || !altitudo)
    {
        redde FALSUM;
    }
    @autoreleasepool {
        schirmus = [[NSScreen screens] count] > ZEPHYRUM
                 ? [[NSScreen screens] objectAtIndex:ZEPHYRUM]
                 : [NSScreen mainScreen];
        si (!schirmus)
        {
            redde FALSUM;
        }
        totum     = [schirmus frame];
        incisura  = 0.0;
        /* plena visio sub incisura (MacBook) contentum non ponit */
        si (@available(macOS 12.0, *))
        {
            incisura = [schirmus safeAreaInsets].top;
        }
        *latitudo  = (i32)totum.size.width;
        *altitudo  = (i32)(totum.size.height - incisura);
    }
    redde *latitudo > ZEPHYRUM && *altitudo > ZEPHYRUM;
}

vacuum
fenestra_minuere (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns miniaturize:nil];
    }
}

vacuum
fenestra_restituere (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns deminiaturize:nil];
    }
}

vacuum
fenestra_commutare_plenam_visionem (
    Fenestra* fenestra)
{
    si (!fenestra) redde;

    @autoreleasepool {
        [fenestra->fenestra_ns toggleFullScreen:nil];
    }
}

b32
fenestra_est_plena_visio (
    constans Fenestra* fenestra)
{
    si (!fenestra) redde FALSUM;
    redde fenestra->plena_visio;
}

vacuum
fenestra_occultare_cursorem (
    Fenestra* fenestra)
{
    si (!fenestra) redde;
    si (!fenestra->cursor_occultus)
    {
        @autoreleasepool {
            [NSCursor hide];
        }
        fenestra->cursor_occultus = VERUM;
    }
}

vacuum
fenestra_ostendere_cursorem (
    Fenestra* fenestra)
{
    si (!fenestra) redde;
    si (fenestra->cursor_occultus)
    {
        @autoreleasepool {
            [NSCursor unhide];
        }
        fenestra->cursor_occultus = FALSUM;
    }
}

vacuum*
fenestra_obtinere_tractationem_nativam (
    Fenestra* fenestra)
{
    si (!fenestra) redde NIHIL;
    redde (__bridge vacuum*)fenestra->fenestra_ns;
}

i32
fenestra_numerus_nativus (
    Fenestra* fenestra)
{
    si (!fenestra || !fenestra->fenestra_ns) redde ZEPHYRUM;
    /* windowNumber = CGWindowID, quem screencapture -l accipit */
    redde (i32)[fenestra->fenestra_ns windowNumber];
}

vacuum
fenestra_clavem_capere (
    Fenestra* fenestra)
{
    si (!fenestra || !fenestra->fenestra_ns) redde;
    [NSApp activateIgnoringOtherApps:YES];
    [fenestra->fenestra_ns makeKeyAndOrderFront:nil];
}

b32
fenestra_clavem_immittere (
    Fenestra*           fenestra,
    i32                 codex,
    i32                 modificatores,
    constans character* characteres,
    b32                 depressa)
{
    NSString* chordae;
    NSEvent*  eventus;

    si (!fenestra || !fenestra->fenestra_ns) redde FALSUM;

    /* Characteres VACUI liciti sunt: claves mutae (Tab, Escape,
     * sagittae) nihil pariunt, et NSEvent id fert. */
    chordae = (characteres != NIHIL)
        ? [NSString stringWithUTF8String:characteres]
        : @"";
    si (chordae == nil)
    {
        redde FALSUM;   /* UTF-8 pravum */
    }

    eventus = [NSEvent
        keyEventWithType:(depressa ? NSEventTypeKeyDown : NSEventTypeKeyUp)
                location:NSZeroPoint
           modifierFlags:(NSEventModifierFlags)(unsigned long)modificatores
               timestamp:[[NSProcessInfo processInfo] systemUptime]
            windowNumber:[fenestra->fenestra_ns windowNumber]
                 context:nil
              characters:chordae
    charactersIgnoringModifiers:chordae
               isARepeat:NO
                 keyCode:(unsigned short)codex];
    si (eventus == nil)
    {
        redde FALSUM;
    }

    /* atStart:NO - post ea quae iam pendent, ut ordo humanus servetur
     * (clavis post clavem, non ante). */
    [NSApp postEvent:eventus atStart:NO];
    redde VERUM;
}

b32
fenestra_murem_immittere (
    Fenestra*        fenestra,
    FenestraMusGenus genus,
    i32              x,
    i32              y,
    i32              modificatores)
{
    NSView*     visus;
    NSPoint     in_visu;
    NSPoint     in_fenestra;
    NSEventType typus;
    NSEvent*    eventus;
    float       pressio;

    si (!fenestra || !fenestra->fenestra_ns) redde FALSUM;

    visus = [fenestra->fenestra_ns contentView];
    si (visus == nil) redde FALSUM;

    /* NE ADDAS 'setAcceptsMouseMovedEvents:YES' - BIS mensuratum
     * superfluam esse (semel dum causam quaero, iterum dum hanc
     * ipsam suspicor). Textura eam sibi ponit. */
    /* VERSIO COORDINATARUM - sola causa cur haec functio hic vivit.
     *
     * CSS: origo SUMMA sinistra, y deorsum crescit.
     * AppKit: origo IMA sinistra, y sursum crescit - NISI visus se
     * 'flipped' declarat, quod visus vitreae FACIT.
     *
     * MENSURATUM 2026-08-14, et haec sola causa cur spica ante ansam
     * scripta est: inversione manu addita, eventus 'mouseup @ 180,140'
     * in paginam venit ubi 260 missum erat (CD - CCLX = CXL). Duplex
     * inversio. Clicus omnis ALIBI cecidisset - intra fenestram, in
     * elemento verisimili, ergo 'operatur' visum esset donec aliquis
     * miraretur cur pyxis falsa premeretur.
     *
     * Ergo visum ipsum rogamus, non numerum scribimus. */
    in_visu = [visus isFlipped]
        ? NSMakePoint((CGFloat)x, (CGFloat)y)
        : NSMakePoint((CGFloat)x,
                      [visus bounds].size.height - (CGFloat)y);
    in_fenestra = [visus convertPoint:in_visu toView:nil];

    pressio = (float)0.0;
    commutatio (genus)
    {
        casus FENESTRA_MUS_MOTUS:
            typus = NSEventTypeMouseMoved; frange;
        casus FENESTRA_MUS_DEPRESSIO:
            typus = NSEventTypeLeftMouseDown;
            pressio = (float)1.0; frange;
        casus FENESTRA_MUS_TRACTUS:
            typus = NSEventTypeLeftMouseDragged;
            pressio = (float)1.0; frange;
        casus FENESTRA_MUS_LIBERATIO:
            typus = NSEventTypeLeftMouseUp; frange;
        casus FENESTRA_MUS_DEPRESSIO_DEXTRA:
            typus = NSEventTypeRightMouseDown; frange;
        casus FENESTRA_MUS_LIBERATIO_DEXTRA:
            typus = NSEventTypeRightMouseUp; frange;
        ordinarius:
            redde FALSUM;
    }

    eventus = [NSEvent
        mouseEventWithType:typus
                  location:in_fenestra
             modifierFlags:(NSEventModifierFlags)(unsigned long)
                           modificatores
                 timestamp:[[NSProcessInfo processInfo] systemUptime]
              windowNumber:[fenestra->fenestra_ns windowNumber]
                   context:nil
               eventNumber:0
                clickCount:1
                  pressure:pressio];
    si (eventus == nil) redde FALSUM;

    [NSApp postEvent:eventus atStart:NO];
    redde VERUM;
}

b32
fenestra_musarius (
    vacuum*             datum,
    constans character* genus,
    i32                 x,
    i32                 y)
{
    Fenestra* fenestra = (Fenestra*)datum;

    si (fenestra == NIHIL || genus == NIHIL)
    {
        redde FALSUM;
    }
    /* Focum rapere ut claviarius: eventus ad fenestram CLAVEM it.
     * Spica focum ceperat (per clavem) ante immissionem; verbum
     * 'movere' non capiebat - sola differentia structuralis quae
     * inter spicam operantem et verbum mutum restabat. */
    fenestra_clavem_capere(fenestra);

    si (strcmp(genus, "motus") == 0)
    {
        redde fenestra_murem_immittere(fenestra, FENESTRA_MUS_MOTUS,
            x, y, ZEPHYRUM);
    }
    si (strcmp(genus, "depressio") == 0)
    {
        redde fenestra_murem_immittere(fenestra,
            FENESTRA_MUS_DEPRESSIO, x, y, ZEPHYRUM);
    }
    si (strcmp(genus, "tractus") == 0)
    {
        redde fenestra_murem_immittere(fenestra,
            FENESTRA_MUS_TRACTUS, x, y, ZEPHYRUM);
    }
    si (strcmp(genus, "liberatio") == 0)
    {
        redde fenestra_murem_immittere(fenestra,
            FENESTRA_MUS_LIBERATIO, x, y, ZEPHYRUM);
    }
    si (strcmp(genus, "depressio-dextra") == 0)
    {
        redde fenestra_murem_immittere(fenestra,
            FENESTRA_MUS_DEPRESSIO_DEXTRA, x, y, ZEPHYRUM);
    }
    si (strcmp(genus, "liberatio-dextra") == 0)
    {
        redde fenestra_murem_immittere(fenestra,
            FENESTRA_MUS_LIBERATIO_DEXTRA, x, y, ZEPHYRUM);
    }

    redde FALSUM;   /* genus ignotum: RECUSATIO, non motus mutus */
}

b32
fenestra_magnitudinator (
    vacuum* datum,
    i32     latitudo,
    i32     altitudo,
    i32*    latitudo_facta,
    i32*    altitudo_facta)
{
    Fenestra* fenestra = (Fenestra*)datum;

    si (fenestra == NIHIL)
    {
        redde FALSUM;
    }
    /* Non positiva RECUSANTUR hic, non transmittuntur. NSWindow
     * mensuram ZEPHYRUM sine querela accipit et fenestram simpliciter
     * non mutat - quod verbum 'factum' redderet ubi nihil factum est. */
    si (latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }

    fenestra_ponere_magnitudinem(fenestra, latitudo, altitudo);
    /* RURSUS LEGERE, semper. Numerus petitus nihil probat: systema
     * minimas suas silenter imponit. Solum haec lectio scit quid
     * pagina revera accepit. */
    fenestra_obtinere_magnitudinem(fenestra, latitudo_facta,
        altitudo_facta);
    redde VERUM;
}

/* Praefixa modificatorum ('Cmd+' 'Ctrl+' 'Shift+' 'Alt+' 'Opt+'),
 * cumulabilia; *p post ea ponitur. SEDES UNA: fenestra_claviarius
 * (claves immissae) et fenestra_menu_addere (aequivalens rei). */
interior i32
_modificantes_legere (
    constans character** p)
{
    i32 modi = ZEPHYRUM;

    per (;;)
    {
        si (strncmp(*p, "Cmd+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagCommand;
            *p = *p + 4;
        }
        alioquin si (strncmp(*p, "Ctrl+", 5) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagControl;
            *p = *p + 5;
        }
        alioquin si (strncmp(*p, "Shift+", 6) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagShift;
            *p = *p + 6;
        }
        alioquin si (strncmp(*p, "Alt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            *p = *p + 4;
        }
        alioquin si (strncmp(*p, "Opt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            *p = *p + 4;
        }
        alioquin
        {
            frange;
        }
    }
    redde modi;
}


/* ==================================================
 * Claves NOMINATAE
 * ==================================================
 *
 * TABULA HIC, non in imperio: codices virtuales et vexilla
 * modificatorum res macOS sunt. Imperium nomen SOLUM transmittit,
 * ergo neutrum systema alterius scientiam portat.
 *
 * Litterae et numeri hic ABSUNT: tabulas proprias habent
 * (CODICES_LITTERARUM, CODICES_NUMERORUM in fenestra_claviarius) -
 * codex positionem nominat, non litteram. Nomina infra positione
 * stabilia sunt.
 */

nomen structura {
    constans character* titulus;
    i32                 codex;
    constans character* characteres;   /* quod clavis parit */
} ClavisNominata;

interior constans ClavisNominata CLAVES[] = {
    { "Enter",      36, "\r"   },
    { "Tab",        48, "\t"   },
    { "Escape",     53, "\033" },
    { "Space",      49, " "    },
    { "Backspace",  51, "\010" },
    { "Delete",    117, ""     },
    { "ArrowUp",   126, ""     },
    { "ArrowDown", 125, ""     },
    { "ArrowLeft", 123, ""     },
    { "ArrowRight", 124, ""     },
    { "Home",      115, ""     },
    { "End",       119, ""     },
    { "PageUp",    116, ""     },
    { "PageDown",  121, ""     },
    { "F1",        122, ""     }, { "F2",  120, "" },
    { "F3",         99, ""     }, { "F4",  118, "" },
    { "F5",         96, ""     }, { "F6",   97, "" },
    { "F7",         98, ""     }, { "F8",  100, "" },
    { "F9",        101, ""     }, { "F10", 109, "" },
    { "F11",       103, ""     }, { "F12", 111, "" }
};

#define CLAVES_NUMERUS \
    ((i32)(magnitudo(CLAVES) / magnitudo(CLAVES[0])))

b32
fenestra_claviarius (
    vacuum*             datum,
    constans character* clavis)
{
    Fenestra*           fenestra = (Fenestra*)datum;
    constans character* p        = clavis;
    i32                 modi     = ZEPHYRUM;
    i32                 i;

    si (fenestra == NIHIL || clavis == NIHIL)
    {
        redde FALSUM;
    }

    /* Praefixa modificatorum, cumulabilia (sedes una) */
    modi = _modificantes_legere(&p);

    /* LITTERA aut NUMERUS UNUS - 'a', '7', 'Cmd+c', 'Cmd+Shift+z'.
     *
     * SINE MODIFICATORE quoque (lapide feature-requests/024,
     * 2026-09-29; olim recusatum): pressio UNA clavis nativa (keydown
     * + keyup), ut digitus hominis - brevitates paginae ('a', 'n',
     * '1'-'9' in document) eam audiunt, et 'premere-textum' sine foco
     * nullum keydown excitat. TEXTUS (chorda) per 'scribere' manet.
     * Littera MAIUSCULA sola Shift implicat, ut in claviatura vera.
     * Shift cum numero numerum servat - signum ('!') dispositionis
     * res est, non divinatur.
     *
     * QUOD AEQUIVALENTIAM MENU REGIT - MENSURATUM 2026-08-15, et
     * mensura consilium mutavit: AppKit aequivalentias per
     * 'charactersIgnoringModifiers' congruit, NON per codicem.
     * Spica codicem ZEPHYRUM (positionem 'a') omni litterae dedit,
     * et tamen 'Cmd+c' deinde 'Cmd+v' textum inter campos
     * transtulit - quamquam 'c' VIII est et 'v' IX. Ergo nulla
     * tabula dispositionum opus est, nullum argumentum
     * dispositionis: characteres sufficiunt, et macOS ipse idem
     * facit (aequivalentia Cmd operatur dum Graece scribis).
     *
     * TABULA TAMEN ADEST, ob 'e.code' SOLUM. Spica ostendit paginam
     * 'KeyA' pro omni littera videre - 'e.key' et 'e.keyCode' recta
     * erant (ex characteribus veniunt), 'e.code' solus pravus.
     *
     * ET HIC 'ANSI' NON EST DIVINATIO: 'e.code' POSITIONEM nominat
     * ex definitione, et 'KeyC' significat 'ubi C in tabula US
     * sedet' - quod est ipsum quod kVK_ANSI_C significat. Duo nomina
     * eiusdem rei. (Limes honestus: in dispositione AZERTY homo qui
     * 'a' scribit positionem 'KeyQ' premit, ergo noster 'KeyA' ab eo
     * differret. 'e.key' ubique rectum manet - et id est quod codex
     * legere debet.) */
    si (   p[0] != '\0' && p[1] == '\0'
        && ((p[0] >= 'a' && p[0] <= 'z')
            || (p[0] >= 'A' && p[0] <= 'Z')
            || (p[0] >= '0' && p[0] <= '9')))
    {
        /* kVK_ANSI_* pro a..z, ordine litterarum.
         *
         * NUMERI, non constantes Carbon, et hoc consulto: Carbon ab
         * Apple deprecatum est, et arbor haec saecula spectat.
         * Codices ipsi ABI sunt - immoti ab anno MCMLXXXIV, nec
         * mutari possunt sine omni agitatore claviaturae frangendo.
         * Ergo periculum non est mutatio sed TRANSCRIPTIO.
         *
         * Contra eam: ./tools/claves_codices_probare.sh omnes XXVI
         * contra systema ipsum comparat (culpa inserta probatum -
         * litteram et utrumque numerum nominat). Si Carbon evanescet,
         * scriptum solum perit; tabula manet. */
        interior constans i32 CODICES_LITTERARUM[XXVI] = {
            0, 11,  8,  2, 14,  3,  5,  4, 34, 38, 40, 37, 46,
           45, 31, 35, 12, 15,  1, 17, 32,  9, 13,  7, 16,  6
        };
        /* kVK_ANSI_* pro 0..9, ordine numerorum (idem scriptum
         * probat) */
        interior constans i32 CODICES_NUMERORUM[X] = {
           29, 18, 19, 20, 21, 23, 22, 26, 28, 25
        };
        character littera[2];
        i32       codex;

        si (p[0] >= '0' && p[0] <= '9')
        {
            littera[0]  = p[0];
            littera[1]  = '\0';
            codex       = CODICES_NUMERORUM[(i32)(p[0] - '0')];
            fenestra_clavem_capere(fenestra);
            si (!fenestra_clavem_immittere(fenestra, codex, modi,
                    littera, VERUM))
            {
                redde FALSUM;
            }
            redde fenestra_clavem_immittere(fenestra, codex, modi,
                littera, FALSUM);
        }
        /* maiuscula sola = Shift implicitum (claviatura vera) */
        si (p[0] >= 'A' && p[0] <= 'Z')
        {
            modi = modi | (i32)NSEventModifierFlagShift;
        }
        littera[0] = (character)((p[0] >= 'A' && p[0] <= 'Z')
            ? (p[0] + ('a' - 'A')) : p[0]);
        codex = CODICES_LITTERARUM[(i32)(littera[0] - 'a')];

        /* Shift depressus litteram MAIUSCULAM parit:
         * 'charactersIgnoringModifiers' modificatores ignorat PRAETER
         * Shift, ergo eventus verus 'Z' fert, non 'z'. Menu cuius
         * aequivalens '@"Z"' est ita solum congruit. */
        si ((modi & (i32)NSEventModifierFlagShift) != ZEPHYRUM)
        {
            littera[0] = (character)(littera[0] - ('a' - 'A'));
        }
        littera[1] = '\0';

        fenestra_clavem_capere(fenestra);
        si (!fenestra_clavem_immittere(fenestra, codex, modi,
                littera, VERUM))
        {
            redde FALSUM;
        }
        redde fenestra_clavem_immittere(fenestra, codex, modi,
            littera, FALSUM);
    }

    per (i = ZEPHYRUM; i < CLAVES_NUMERUS; i++)
    {
        si (strcmp(p, CLAVES[i].titulus) == 0)
        {
            /* Focus PRIMUM: eventus ad fenestram clavem it, et
             * agitator eam non tenet. Ante depressionem solum -
             * bis rapere nihil addit. */
            fenestra_clavem_capere(fenestra);

            si (!fenestra_clavem_immittere(fenestra, CLAVES[i].codex,
                    modi, CLAVES[i].characteres, VERUM))
            {
                redde FALSUM;
            }
            redde fenestra_clavem_immittere(fenestra, CLAVES[i].codex,
                modi, CLAVES[i].characteres, FALSUM);
        }
    }

    /* Nomen ignotum: RECUSATIO, non ictus mutus qui 'factum'
     * nuntiaret. */
    redde FALSUM;
}

/* Menu applicationis: vide fenestra.h */
b32
fenestra_menu_addere (
               Fenestra* fenestra,
    constans character* titulus,
    constans character* clavis,
                    i32  signum)
{
                 NSMenu* app_menu;
             NSMenuItem* res;
     FenestraMenuScopus* scopus;
               NSString* aequivalens = @"";
               NSInteger  locus;
     constans character* p;
                     i32  modi = ZEPHYRUM;

    si (   fenestra == NIHIL || titulus == NIHIL || NSApp == nil
        || [NSApp mainMenu] == nil)
    {
        redde FALSUM;
    }
    @autoreleasepool {
        app_menu = [[[NSApp mainMenu] itemAtIndex:0] submenu];
        locus    = (app_menu != nil)
                 ? [app_menu indexOfItemWithTitle:@"Exire"] : -1;
        si (locus < 0)
        {
            redde FALSUM;
        }
        si (clavis != NIHIL)
        {
            character littera[2];

            p     = clavis;
            modi  = _modificantes_legere(&p);
            si (p[0] == '\0' || p[1] != '\0')
            {
                redde FALSUM;   /* littera UNA post modificantes */
            }
            littera[0]  = p[0];
            littera[1]  = '\0';
            /* Shift + littera = aequivalens MAIUSCULUM sine Shift in
             * masca: forma '@"Z"' rei Iterare, quam claves immissae
             * congruere MENSURATAE sunt (fenestra_claviarius) */
            si (   (modi & (i32)NSEventModifierFlagShift) != ZEPHYRUM
                && littera[0] >= 'a' && littera[0] <= 'z')
            {
                littera[0]  = (character)(littera[0] - ('a' - 'A'));
                modi        = modi & ~(i32)NSEventModifierFlagShift;
            }
            aequivalens = [NSString stringWithUTF8String:littera];
        }
        scopus           = [[FenestraMenuScopus alloc] init];
        scopus.fenestra  = fenestra;
        res = [[NSMenuItem alloc]
            initWithTitle:[NSString stringWithUTF8String:titulus]
                   action:@selector(pressa:)
            keyEquivalent:aequivalens];
        [res setTarget:scopus];
        [res setTag:(NSInteger)signum];
        [res setKeyEquivalentModifierMask:
            (NSEventModifierFlags)(unsigned long)modi];
        /* separator SEMEL ante 'Exire'; res priores supra eum */
        si (   locus > 0
            && [[app_menu itemAtIndex:locus - 1] isSeparatorItem])
        {
            locus = locus - 1;
        }
        alioquin
        {
            [app_menu insertItem:[NSMenuItem separatorItem]
                         atIndex:locus];
        }
        [app_menu insertItem:res atIndex:locus];
        [res release];
    }
    redde VERUM;
}

/* Implementatio tabulae pixelorum */

TabulaPixelorum*
fenestra_creare_tabulam_pixelorum (
    Piscina*  piscina,
    Fenestra* fenestra,
    i32 altitudo_fixa)
{
    TabulaPixelorum *tabula;
    NSRect rectangulum_contenti;

    si (!fenestra || altitudo_fixa <= ZEPHYRUM) redde NIHIL;

    tabula = piscina_allocare(piscina, magnitudo(TabulaPixelorum));
    si (!tabula) redde NIHIL;

    /* Obtinere dimensiones fenestrae */
    rectangulum_contenti =
        [fenestra->fenestra_ns contentRectForFrameRect:[fenestra->fenestra_ns frame]];
    tabula->fenestra_latitudo = (i32)rectangulum_contenti.size.width;
    tabula->fenestra_altitudo = (i32)rectangulum_contenti.size.height;

    /* Calculare dimensiones tabulae basatas in altitudine fixa */
    tabula->altitudo = altitudo_fixa;
    tabula->scala = (f32)tabula->fenestra_altitudo / (f32)altitudo_fixa;
    tabula->latitudo = (i32)(tabula->fenestra_latitudo / tabula->scala);
    tabula->capacitas = tabula->latitudo * tabula->altitudo;

    /* Allocare tabulam pixelorum */
    tabula->pixela = piscina_allocare(piscina,
        tabula->latitudo * tabula->altitudo * magnitudo(i32));
    si (!tabula->pixela)
    {
        /* Piscina possidet memoriam - non liberare */
        redde NIHIL;
    }

    redde tabula;
}

/* tabula_pixelorum_vacare / _ponere_pixelum / _obtinere_pixelum: in
 * lib/fenestra_textus.c (tabula_pixelorum.h; puri, sine Cocoa -
 * tessellatio T4) */

vacuum
fenestra_praesentare_pixela (
    Fenestra* fenestra,
    TabulaPixelorum* tabula)
{
    si (!fenestra || !tabula || !tabula->pixela) redde;

    @autoreleasepool {
        /* Obtinere visum contenti */
        NSView *visus = fenestra->visus;

        /* Creare contextum bitmap ex tabula pixelorum nostra */
        CGColorSpaceRef spatium_coloris = CGColorSpaceCreateDeviceRGB();
        CGContextRef contextus_bitmap = CGBitmapContextCreate(
            tabula->pixela,
            tabula->latitudo,
            tabula->altitudo,
            VIII,  /* bits per componentem */
            tabula->latitudo * IV,  /* bytes per ordinem */
            spatium_coloris,
            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big
        );

        /* Creare imaginem ex contextu bitmap */
        CGImageRef imago = CGBitmapContextCreateImage(contextus_bitmap);

        /* Cogere redesignationem visus cum imagine nostra */
        [visus setNeedsDisplay:YES];

        /* Reponere imaginem in visu pro pingendo */
        objc_setAssociatedObject(visus, "imagoPixelorum",
            (__bridge id)imago, OBJC_ASSOCIATION_RETAIN);

        /* Purgare */
        CGImageRelease(imago);
        CGContextRelease(contextus_bitmap);
        CGColorSpaceRelease(spatium_coloris);
    }
}


/* ==================================================
 * Functiones Temporis pro Tempus Bibliotheca
 * ================================================== */

/* fenestra_tempus_*, fenestra_dormire: lib/fenestra_tempus_macos.c
 * (mach/POSIX puri; eventus A3a) */
