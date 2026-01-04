/*
 * clipboard_platform_macos.m - macOS clipboard implementation
 *
 * Utitur Cocoa NSPasteboard pro accessu ad clipboard systematis.
 */

/* Includere systemae primum ut vitare conflictos macros */
#import <Cocoa/Cocoa.h>
#include "clipboard_platform.h"
#include "chorda.h"

/* ============================================================
 * clipboard_habet_imaginem
 * ============================================================ */

b32
clipboard_habet_imaginem(vacuum)
{
    @autoreleasepool {
        NSPasteboard* pb = [NSPasteboard generalPasteboard];
        NSArray* types = [pb types];

        /* Quaerere pro typis imaginum */
        si ([types containsObject:NSPasteboardTypeTIFF] ||
            [types containsObject:NSPasteboardTypePNG])
        {
            redde VERUM;
        }

        redde FALSUM;
    }
}

/* ============================================================
 * clipboard_capere_imaginem
 * ============================================================ */

ImagoFructus
clipboard_capere_imaginem(Piscina* piscina)
{
    ImagoFructus fructus;
    fructus.successus = FALSUM;
    fructus.imago.pixela = NIHIL;
    fructus.imago.latitudo = 0;
    fructus.imago.altitudo = 0;
    fructus.error.datum = NIHIL;
    fructus.error.mensura = 0;

    @autoreleasepool {
        NSPasteboard* pb = [NSPasteboard generalPasteboard];
        NSData* data = NIHIL;
        NSBitmapImageRep* rep = NIHIL;
        NSInteger latitudo, altitudo;
        NSInteger samplesPerPixel, bitsPerSample;
        insignatus character* bitmapData;
        i8* pixela;
        memoriae_index mensura;
        NSInteger x, y;

        /* Tentare TIFF primum, deinde PNG */
        data = [pb dataForType:NSPasteboardTypeTIFF];
        si (!data)
        {
            data = [pb dataForType:NSPasteboardTypePNG];
        }

        si (!data)
        {
            fructus.error = chorda_ex_literis("Nulla imago in clipboard", piscina);
            redde fructus;
        }

        /* Creare bitmap representation */
        rep = [[NSBitmapImageRep alloc] initWithData:data];
        si (!rep)
        {
            fructus.error = chorda_ex_literis("Non possum decodere imaginem", piscina);
            redde fructus;
        }

        /* Obtinere dimensiones */
        latitudo = [rep pixelsWide];
        altitudo = [rep pixelsHigh];
        samplesPerPixel = [rep samplesPerPixel];
        bitsPerSample = [rep bitsPerSample];

        si (latitudo <= 0 || altitudo <= 0)
        {
            fructus.error = chorda_ex_literis("Imago habet dimensiones invalidas", piscina);
            redde fructus;
        }

        /* Obtinere stride (bytes per row - may have padding) */
        {
            NSInteger bytesPerRow = [rep bytesPerRow];

            /* Allocare buffer pro RGBA */
            mensura = (memoriae_index)(latitudo * altitudo * 4);
            pixela = (i8*)piscina_allocare(piscina, mensura);
            si (!pixela)
            {
                fructus.error = chorda_ex_literis("Non possum allocare memoriam", piscina);
                redde fructus;
            }

            /* Obtinere datum bitmap */
            bitmapData = [rep bitmapData];

            /* Convertere ad RGBA */
            si (samplesPerPixel >= 4 && bitsPerSample == 8)
            {
                /* RGBA vel ARGB - copiare cum stride */
                per (y = 0; y < altitudo; y++)
                {
                    insignatus character* rowPtr = bitmapData + y * bytesPerRow;
                    per (x = 0; x < latitudo; x++)
                    {
                        NSInteger srcIdx = x * samplesPerPixel;
                        NSInteger dstIdx = (y * latitudo + x) * 4;
                        pixela[dstIdx + 0] = (i8)rowPtr[srcIdx + 0];  /* R */
                        pixela[dstIdx + 1] = (i8)rowPtr[srcIdx + 1];  /* G */
                        pixela[dstIdx + 2] = (i8)rowPtr[srcIdx + 2];  /* B */
                        pixela[dstIdx + 3] = (i8)rowPtr[srcIdx + 3];  /* A */
                    }
                }
            }
            alioquin si (samplesPerPixel == 3 && bitsPerSample == 8)
            {
                /* RGB - addere alpha */
                /* Note: bytesPerRow may be 4 bytes/pixel for alignment even with 3 samples */
                NSInteger bytesPerPixel = bytesPerRow / latitudo;
                per (y = 0; y < altitudo; y++)
                {
                    insignatus character* rowPtr = bitmapData + y * bytesPerRow;
                    per (x = 0; x < latitudo; x++)
                    {
                        NSInteger srcIdx = x * bytesPerPixel;
                        NSInteger dstIdx = (y * latitudo + x) * 4;
                        pixela[dstIdx + 0] = (i8)rowPtr[srcIdx + 0];  /* R */
                        pixela[dstIdx + 1] = (i8)rowPtr[srcIdx + 1];  /* G */
                        pixela[dstIdx + 2] = (i8)rowPtr[srcIdx + 2];  /* B */
                        pixela[dstIdx + 3] = (i8)255;                  /* A = opaque */
                    }
                }
            }
            alioquin si (samplesPerPixel == 1 && bitsPerSample == 8)
            {
                /* Grayscale - expandere ad RGBA */
                per (y = 0; y < altitudo; y++)
                {
                    insignatus character* rowPtr = bitmapData + y * bytesPerRow;
                    per (x = 0; x < latitudo; x++)
                    {
                        NSInteger dstIdx = (y * latitudo + x) * 4;
                        i8 gray = (i8)rowPtr[x];
                        pixela[dstIdx + 0] = gray;      /* R */
                        pixela[dstIdx + 1] = gray;      /* G */
                        pixela[dstIdx + 2] = gray;      /* B */
                        pixela[dstIdx + 3] = (i8)255;   /* A */
                    }
                }
            }
            alioquin
            {
                /* Unsupported format - try to convert via drawing */
                NSBitmapImageRep* rgbaRep;
                NSGraphicsContext* ctx;
                NSImage* tempImage;

            rgbaRep = [[NSBitmapImageRep alloc]
                initWithBitmapDataPlanes:NIHIL
                pixelsWide:latitudo
                pixelsHigh:altitudo
                bitsPerSample:8
                samplesPerPixel:4
                hasAlpha:YES
                isPlanar:NO
                colorSpaceName:NSCalibratedRGBColorSpace
                bytesPerRow:latitudo * 4
                bitsPerPixel:32];

            si (rgbaRep)
            {
                [NSGraphicsContext saveGraphicsState];
                ctx = [NSGraphicsContext graphicsContextWithBitmapImageRep:rgbaRep];
                [NSGraphicsContext setCurrentContext:ctx];

                /* Draw original into RGBA context */
                tempImage = [[NSImage alloc] initWithSize:NSMakeSize(latitudo, altitudo)];
                [tempImage addRepresentation:rep];
                [tempImage drawAtPoint:NSZeroPoint fromRect:NSZeroRect operation:NSCompositingOperationCopy fraction:1.0];

                [NSGraphicsContext restoreGraphicsState];

                /* Copy converted data */
                bitmapData = [rgbaRep bitmapData];
                per (y = 0; y < altitudo; y++)
                {
                    per (x = 0; x < latitudo; x++)
                    {
                        NSInteger idx = (y * latitudo + x) * 4;
                        pixela[idx + 0] = (i8)bitmapData[idx + 0];
                        pixela[idx + 1] = (i8)bitmapData[idx + 1];
                        pixela[idx + 2] = (i8)bitmapData[idx + 2];
                        pixela[idx + 3] = (i8)bitmapData[idx + 3];
                    }
                }
            }
            alioquin
            {
                fructus.error = chorda_ex_literis("Format imaginis non supportatus", piscina);
                redde fructus;
            }
            }  /* Close alioquin block */
        }  /* Close bytesPerRow scope */

        /* Successus! */
        fructus.successus = VERUM;
        fructus.imago.pixela = pixela;
        fructus.imago.latitudo = (i32)latitudo;
        fructus.imago.altitudo = (i32)altitudo;

        redde fructus;
    }
}
