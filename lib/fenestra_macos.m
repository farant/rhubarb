/* Includere systemae primum ut vitare conflictos macros */
#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import <objc/runtime.h>
#import <mach/mach_time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fenestra.h"

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
};

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

        /* Facere fenestram visibilem et clavem */
        [fenestra->fenestra_ns makeKeyAndOrderFront:nil];

        /* Activare applicationem */
        [NSApp activateIgnoringOtherApps:YES];

        /* Intrare plenam visionem si petitum */
        si (configuratio->vexilla & FENESTRA_PLENA_VISIO)
        {
            [fenestra->fenestra_ns toggleFullScreen:nil];
        }

        redde fenestra;
    }
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
                    NSRect rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_DEPRESSUS;
                    eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                    /* Invertere coordinatam Y: Cocoa utitur origine infero-sinistro */
                    eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
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
                    NSRect rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_LIBERATUS;
                    eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                    eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
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
                    NSRect rectangulum_contenti = [[fenestra->fenestra_ns contentView] frame];
                    eventus.genus = EVENTUS_MUS_MOTUS;
                    eventus.datum.mus.x = (i32)[eventus_ns locationInWindow].x;
                    eventus.datum.mus.y = (i32)(rectangulum_contenti.size.height - [eventus_ns locationInWindow].y);
                    eventus.datum.mus.modificantes = (i32)[eventus_ns modifierFlags];
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

vacuum*
fenestra_obtinere_tractationem_nativam (
    Fenestra* fenestra)
{
    si (!fenestra) redde NIHIL;
    redde (__bridge vacuum*)fenestra->fenestra_ns;
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
