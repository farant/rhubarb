/* Includere systemae primum ut vitare conflictos macros */
#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import <objc/runtime.h>
#import <mach/mach_time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fenestra.h"

/* Forward declarations pro struct Fenestra */
@class FenestraVisus;
@class FenestraDelegatus;

#define MAXIMUS_EVENTUUM CCLVI

structura Fenestra {
    Piscina* piscina;
    NSWindow *fenestra_ns;
    FenestraVisus *visus;
    FenestraDelegatus *delegatus;
    Eventus eventus[MAXIMUS_EVENTUUM];
    i32 eventus_caput;
    i32 eventus_cauda;
    i32 eventus_numerus;
    b32 plena_visio;      /* Status plenae visionis */
    b32 cursor_occultus;  /* Si cursor systematis occultatus */
};

@interface FenestraDelegatus : NSObject <NSWindowDelegate>
@property (assign) BOOL debet_claudere;
@property (assign) Fenestra *fenestra;
@end

@implementation FenestraDelegatus
- (BOOL)windowShouldClose:(NSWindow *)sender {
    self.debet_claudere = YES;
    redde NO; /* Non claudere statim, usorem tractare sinere */
}

- (void)windowDidResize:(NSNotification *)notification {
    /* Tractare eventus mutationis magnitudinis */
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    /* Tractare eventus focus */
}

- (void)windowDidResignKey:(NSNotification *)notification {
    /* Tractare eventus defocus */
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
    imago_pixelorum = (__bridge CGImageRef)objc_getAssociatedObject(self, "imagoPixelorum");

    si (imago_pixelorum)
    {
        /* Obtinere contextum graphicum currentem */
        contextus = [[NSGraphicsContext currentContext] CGContext];

        /* Ponere interpolationem ad vicinum proximum pro pixelis acutis */
        CGContextSetInterpolationQuality(contextus, kCGInterpolationNone);

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
    si (fenestra->eventus_numerus >= MAXIMUS_EVENTUUM)
    {
        redde; /* Cauda eventuum plena */
    }

    fenestra->eventus[fenestra->eventus_cauda] = *eventus;
    fenestra->eventus_cauda = (fenestra->eventus_cauda + I) % MAXIMUS_EVENTUUM;
    fenestra->eventus_numerus++;
}

interior b32
extrahere_eventum (
    Fenestra* fenestra,
    Eventus* eventus)
{
    si (fenestra->eventus_numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }

    *eventus = fenestra->eventus[fenestra->eventus_caput];
    fenestra->eventus_caput = (fenestra->eventus_caput + I) % MAXIMUS_EVENTUUM;
    fenestra->eventus_numerus--;
    redde VERUM;
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
            TransformProcessType(&psn, kProcessTransformToForegroundApplication);

            _menu_ordinarium_ponere();
        }

        fenestra = piscina_allocare(piscina, magnitudo(Fenestra));
        si (!fenestra) redde NIHIL;

        fenestra->piscina = piscina;

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
                          configuratio->latitudo, configuratio->altitudo);
        fenestra->fenestra_ns = [[NSWindow alloc] initWithContentRect:forma
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

vacuum
fenestra_perscrutari_eventus (
    Fenestra* fenestra)
{
    NSEvent *eventus_ns;
    interior NSSize magnitudo_ultima = {0};
    NSSize magnitudo_currens;

    @autoreleasepool {

        dum ((eventus_ns = [NSApp nextEventMatchingMask:NSEventMaskAny
                                               untilDate:[NSDate distantPast]
                                                  inMode:NSDefaultRunLoopMode
                                                 dequeue:YES]))
        {

            Eventus eventus = {ZEPHYRUM};

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

                casus NSEventTypeKeyDown: {
                    NSString* characteres;

                    eventus.genus = EVENTUS_CLAVIS_DEPRESSUS;
                    eventus.datum.clavis.clavis = convertere_clavem([eventus_ns keyCode]);
                    eventus.datum.clavis.modificantes = (i32)[eventus_ns modifierFlags];

                    /* Extrahere characterem typatum ex NSEvent */
                    characteres = [eventus_ns characters];
                    si ([characteres length] > ZEPHYRUM) {
                        eventus.datum.clavis.typus = (character)[characteres characterAtIndex:ZEPHYRUM];
                    } alioquin {
                        eventus.datum.clavis.typus = '\0';
                    }

                    impellere_eventum(fenestra, &eventus);
                    frange;
                }

                casus NSEventTypeKeyUp: {
                    NSString* characteres;

                    eventus.genus = EVENTUS_CLAVIS_LIBERATUS;
                    eventus.datum.clavis.clavis = convertere_clavem([eventus_ns keyCode]);
                    eventus.datum.clavis.modificantes = (i32)[eventus_ns modifierFlags];

                    /* Extrahere characterem typatum ex NSEvent */
                    characteres = [eventus_ns characters];
                    si ([characteres length] > ZEPHYRUM) {
                        eventus.datum.clavis.typus = (character)[characteres characterAtIndex:ZEPHYRUM];
                    } alioquin {
                        eventus.datum.clavis.typus = '\0';
                    }

                    impellere_eventum(fenestra, &eventus);
                    frange;
                }

                casus NSEventTypeLeftMouseDown:
                casus NSEventTypeRightMouseDown:
                casus NSEventTypeOtherMouseDown: {
                    NSRect rectangulum_contenti;
                    CGImageRef imago_pixelorum;
                    f64 window_x, window_y;
                    f64 bitmap_latitudo, bitmap_altitudo;
                    f64 scala_x, scala_y;

                    rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_DEPRESSUS;

                    /* Obtinere imaginem bitmap ad computandum scalam */
                    imago_pixelorum = (__bridge CGImageRef)objc_getAssociatedObject(fenestra->visus, "imagoPixelorum");

                    si (imago_pixelorum) {
                        /* Computare scalam inter fenestram et bitmap */
                        bitmap_latitudo = (f64)CGImageGetWidth(imago_pixelorum);
                        bitmap_altitudo = (f64)CGImageGetHeight(imago_pixelorum);
                        scala_x = bitmap_latitudo / rectangulum_contenti.size.width;
                        scala_y = bitmap_altitudo / rectangulum_contenti.size.height;

                        /* Convertere coordinatas fenestrae ad coordinatas bitmap */
                        window_x = [eventus_ns locationInWindow].x;
                        window_y = rectangulum_contenti.size.height - [eventus_ns locationInWindow].y;

                        eventus.datum.mus.x = (i32)(window_x * scala_x);
                        eventus.datum.mus.y = (i32)(window_y * scala_y);
                    } alioquin {
                        /* Fallback si nullum bitmap */
                        eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                        eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
                    }

                    eventus.datum.mus.botton = ([eventus_ns type] == NSEventTypeLeftMouseDown) ? MUS_SINISTER :
                                               ([eventus_ns type] == NSEventTypeRightMouseDown) ? MUS_DEXTER :
                                               MUS_MEDIUS;
                    eventus.datum.mus.modificantes = (i32)[eventus_ns modifierFlags];
                    impellere_eventum(fenestra, &eventus);
                    frange;
                }

                casus NSEventTypeLeftMouseUp:
                casus NSEventTypeRightMouseUp:
                casus NSEventTypeOtherMouseUp: {
                    NSRect rectangulum_contenti;
                    CGImageRef imago_pixelorum;
                    f64 window_x, window_y;
                    f64 bitmap_latitudo, bitmap_altitudo;
                    f64 scala_x, scala_y;

                    rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_LIBERATUS;

                    imago_pixelorum = (__bridge CGImageRef)objc_getAssociatedObject(fenestra->visus, "imagoPixelorum");
                    si (imago_pixelorum) {
                        bitmap_latitudo = (f64)CGImageGetWidth(imago_pixelorum);
                        bitmap_altitudo = (f64)CGImageGetHeight(imago_pixelorum);
                        scala_x = bitmap_latitudo / rectangulum_contenti.size.width;
                        scala_y = bitmap_altitudo / rectangulum_contenti.size.height;

                        window_x = [eventus_ns locationInWindow].x;
                        window_y = rectangulum_contenti.size.height - [eventus_ns locationInWindow].y;

                        eventus.datum.mus.x = (i32)(window_x * scala_x);
                        eventus.datum.mus.y = (i32)(window_y * scala_y);
                    } alioquin {
                        eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                        eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
                    }

                    eventus.datum.mus.botton = ([eventus_ns type] == NSEventTypeLeftMouseUp) ? MUS_SINISTER :
                                               ([eventus_ns type] == NSEventTypeRightMouseUp) ? MUS_DEXTER :
                                               MUS_MEDIUS;
                    eventus.datum.mus.modificantes = (i32)[eventus_ns modifierFlags];
                    impellere_eventum(fenestra, &eventus);
                    frange;
                }

                casus NSEventTypeMouseMoved:
                casus NSEventTypeLeftMouseDragged:
                casus NSEventTypeRightMouseDragged:
                casus NSEventTypeOtherMouseDragged: {
                    NSRect rectangulum_contenti;
                    CGImageRef imago_pixelorum;
                    f64 window_x, window_y;
                    f64 bitmap_latitudo, bitmap_altitudo;
                    f64 scala_x, scala_y;

                    rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_MOTUS;

                    imago_pixelorum = (__bridge CGImageRef)objc_getAssociatedObject(fenestra->visus, "imagoPixelorum");
                    si (imago_pixelorum) {
                        bitmap_latitudo = (f64)CGImageGetWidth(imago_pixelorum);
                        bitmap_altitudo = (f64)CGImageGetHeight(imago_pixelorum);
                        scala_x = bitmap_latitudo / rectangulum_contenti.size.width;
                        scala_y = bitmap_altitudo / rectangulum_contenti.size.height;

                        window_x = [eventus_ns locationInWindow].x;
                        window_y = rectangulum_contenti.size.height - [eventus_ns locationInWindow].y;

                        eventus.datum.mus.x = (i32)(window_x * scala_x);
                        eventus.datum.mus.y = (i32)(window_y * scala_y);
                    } alioquin {
                        eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                        eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
                    }

                    eventus.datum.mus.modificantes = (i32)[eventus_ns modifierFlags];

                    /* re-occultatio per motum REMOTA (2026-07-23):
                     * hide iteratum numerum referentiarum NSCursor
                     * inflabat - fons cursoris perpetuo occulti */

                    impellere_eventum(fenestra, &eventus);
                    frange;
                }

                casus NSEventTypeScrollWheel:
                    eventus.genus = EVENTUS_MUS_ROTULA;
                    eventus.datum.rotula.delta_x = (f32)[eventus_ns scrollingDeltaX];
                    eventus.datum.rotula.delta_y = (f32)[eventus_ns scrollingDeltaY];
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
        magnitudo_currens = [fenestra->fenestra_ns frame].size;
        si (magnitudo_currens.width != magnitudo_ultima.width ||
            magnitudo_currens.height != magnitudo_ultima.height)
        {
            Eventus eventus_mutationis = {ZEPHYRUM};
            eventus_mutationis.genus = EVENTUS_MUTARE_MAGNITUDINEM;
            eventus_mutationis.datum.mutare_magnitudinem.latitudo = (i32)magnitudo_currens.width;
            eventus_mutationis.datum.mutare_magnitudinem.altitudo = (i32)magnitudo_currens.height;
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

    forma = [fenestra->fenestra_ns contentRectForFrameRect:[fenestra->fenestra_ns frame]];
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
        forma.size = [fenestra->fenestra_ns frameRectForContentRect:NSMakeRect(0, 0, latitudo, altitudo)].size;
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

/* ==================================================
 * Claves NOMINATAE
 * ==================================================
 *
 * TABULA HIC, non in imperio: codices virtuales et vexilla
 * modificatorum res macOS sunt. Imperium nomen SOLUM transmittit,
 * ergo neutrum systema alterius scientiam portat.
 *
 * Litterae ABSUNT de industria (vide fenestra.h): codex positionem
 * nominat, non litteram. Nomina infra positione stabilia sunt.
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
    { "ArrowRight",124, ""     },
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

    /* Praefixa modificatorum, cumulabilia */
    per (;;)
    {
        si (strncmp(p, "Cmd+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagCommand;
            p = p + 4;
        }
        alioquin si (strncmp(p, "Ctrl+", 5) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagControl;
            p = p + 5;
        }
        alioquin si (strncmp(p, "Shift+", 6) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagShift;
            p = p + 6;
        }
        alioquin si (strncmp(p, "Alt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            p = p + 4;
        }
        alioquin si (strncmp(p, "Opt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            p = p + 4;
        }
        alioquin
        {
            frange;
        }
    }

    /* LITTERA CUM MODIFICATORE - 'Cmd+c', 'Cmd+Shift+z'.
     *
     * SINE MODIFICATORE RECUSATUR: textus per 'scribere' it, quae
     * dispositionis omnino nescia est. Haec via imperiis servit.
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
    si (modi != ZEPHYRUM && p[0] != '\0' && p[1] == '\0'
        && ((p[0] >= 'a' && p[0] <= 'z')
            || (p[0] >= 'A' && p[0] <= 'Z')))
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
        character littera[2];
        i32       index;
        i32       codex;

        littera[0] = (character)((p[0] >= 'A' && p[0] <= 'Z')
            ? (p[0] + ('a' - 'A')) : p[0]);
        index = (i32)(littera[0] - 'a');
        codex = CODICES_LITTERARUM[index];

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
    rectangulum_contenti = [fenestra->fenestra_ns contentRectForFrameRect:[fenestra->fenestra_ns frame]];
    tabula->fenestra_latitudo = (i32)rectangulum_contenti.size.width;
    tabula->fenestra_altitudo = (i32)rectangulum_contenti.size.height;

    /* Calculare dimensiones tabulae basatas in altitudine fixa */
    tabula->altitudo = altitudo_fixa;
    tabula->scala = (f32)tabula->fenestra_altitudo / (f32)altitudo_fixa;
    tabula->latitudo = (i32)(tabula->fenestra_latitudo / tabula->scala);

    /* Allocare tabulam pixelorum */
    tabula->pixela = piscina_allocare(piscina, tabula->latitudo * tabula->altitudo * magnitudo(i32));
    si (!tabula->pixela)
    {
        /* Piscina possidet memoriam - non liberare */
        redde NIHIL;
    }

    redde tabula;
}

vacuum
tabula_pixelorum_vacare (
    TabulaPixelorum* tabula,
    i32 color)
{
    i32 pixela_totalia;
    i32 i;

    si (!tabula || !tabula->pixela) redde;

    pixela_totalia = tabula->latitudo * tabula->altitudo;
    per (i = ZEPHYRUM; i < pixela_totalia; i++)
    {
        tabula->pixela[i] = color;
    }
}

vacuum
tabula_pixelorum_ponere_pixelum (
    TabulaPixelorum* tabula,
    i32 x,
    i32 y,
    i32 color)
{
    si (!tabula || !tabula->pixela) redde;
    si (x < ZEPHYRUM || x >= tabula->latitudo || y < ZEPHYRUM || y >= tabula->altitudo) redde;

    tabula->pixela[y * tabula->latitudo + x] = color;
}

i32
tabula_pixelorum_obtinere_pixelum (
    TabulaPixelorum* tabula,
    i32 x,
    i32 y)
{
    si (!tabula || !tabula->pixela) redde ZEPHYRUM;
    si (x < ZEPHYRUM || x >= tabula->latitudo || y < ZEPHYRUM || y >= tabula->altitudo) redde ZEPHYRUM;

    redde tabula->pixela[y * tabula->latitudo + x];
}

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
        objc_setAssociatedObject(visus, "imagoPixelorum", (__bridge id)imago, OBJC_ASSOCIATION_RETAIN);

        /* Purgare */
        CGImageRelease(imago);
        CGContextRelease(contextus_bitmap);
        CGColorSpaceRelease(spatium_coloris);
    }
}


/* ==================================================
 * Functiones Temporis pro Tempus Bibliotheca
 * ================================================== */

i64
fenestra_tempus_obtinere_pulsus (
    vacuum)
{
    redde (i64)mach_absolute_time();
}

f64
fenestra_tempus_obtinere_frequentiam (
    vacuum)
{
    mach_timebase_info_data_t informatio;

    mach_timebase_info(&informatio);
    redde 1e9 * (f64)informatio.denom / (f64)informatio.numer;
}

vacuum
fenestra_dormire (
    i32 microsecundae)
{
    si (microsecundae > ZEPHYRUM)
    {
        usleep((unsigned int)microsecundae);
    }
}
