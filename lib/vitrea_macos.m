/* vitrea_macos.m - implementatio vitreae (vide vitrea.h; spec
 * project-specs/vitrea-spec-v2.md). CONTACTUS WEBKIT TOTUS hic
 * vivit - vitrea.h C89 purum manet. MRC (retentio manualis), SINE
 * litteris blocorum - mos fenestra_macos.m; completionHandler
 * semper nil. Politica navigationis = Phasis C (methodus
 * decidePolicy blocum acciperet - dilata). */

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

#include "vitrea.h"
#include "chorda_aedificator.h"
#include "internuntius.h"   /* effugator solus - directio sana */
#include <stdio.h>
#include <string.h>

#define VITREA_MAXIMUS_NUNTIORUM CCLVI

@class VitreaPortitor;
@class VitreaAuscultator;
@class VitreaLegatus;

/* situs unus caudae anuli */
nomen structura {
    chorda             textus;
    VitreaNuntiusGenus genus;
} VitreaNuntiusSitus;

structura Vitrea {
    Piscina*            piscina;
    Fenestra*           fenestra;
    WKWebView*          textura;
    VitreaPortitor*     portitor;
    VitreaAuscultator*  auscultator;
    VitreaLegatus*      legatus;
    Capsula*            capsula;
    constans character* via_initialis;
    /* cauda anuli (exemplar fenestrae; overflow = amissio+clamor) */
    VitreaNuntiusSitus  nuntii[VITREA_MAXIMUS_NUNTIORUM];
    i32                 nuntii_caput;
    i32                 nuntii_index_caudae;
    i32                 nuntii_numerus;
    Piscina*            piscina_nuntiorum;
    PiscinaNotatio      nuntiorum_basis;
    Piscina*            piscina_petitionum;
    Piscina*            piscina_aestimationis;
    VitreaFructus       fructus;
};

/* ==================================================
 * auxilia
 * ================================================== */

/* copia NUL-terminata in piscinam */
interior constans character*
_litterae_copiare (Piscina* piscina, constans character* fons)
{
    memoriae_index m = strlen(fons);
    character* l = (character*)piscina_allocare(piscina, m + I);

    si (l == NIHIL)
    {
        redde "";
    }
    memcpy(l, fons, m);
    l[m] = '\0';
    redde l;
}

/* tabula extensio -> MIME (capsula metadata nulla fert -
 * verdictum exploratoris; consumptoris officium) */
nomen structura {
    constans character* extensio;
    constans character* mimen;
} VitreaMimen;

interior constans VitreaMimen MIMINA[] = {
    { "html",  "text/html" },
    { "css",   "text/css" },
    { "js",    "text/javascript" },
    { "mjs",   "text/javascript" },
    { "json",  "application/json" },
    { "svg",   "image/svg+xml" },
    { "png",   "image/png" },
    { "jpg",   "image/jpeg" },
    { "jpeg",  "image/jpeg" },
    { "gif",   "image/gif" },
    { "webp",  "image/webp" },
    { "woff2", "font/woff2" },
    { "wasm",  "application/wasm" },
    { "txt",   "text/plain" }
};

interior constans character*
_mimen_pro_via (constans character* via)
{
    constans character* punctum = NIHIL;
    constans character* c;
    i32 i;

    per (c = via; *c != '\0'; c++)
    {
        si (*c == '.')
        {
            punctum = c + I;
        }
        alioquin si (*c == '/')
        {
            punctum = NIHIL;
        }
    }
    si (punctum != NIHIL)
    {
        per (i = ZEPHYRUM;
             i < (i32)(magnitudo(MIMINA) / magnitudo(MIMINA[0]));
             i++)
        {
            si (strcmp(punctum, MIMINA[i].extensio) == ZEPHYRUM)
            {
                redde MIMINA[i].mimen;
            }
        }
    }
    redde "application/octet-stream";
}

/* nuntium in caudam inserere + pumpam expergefacere. Copia +
 * insertio SOLAE - nullus codex usoris intra vocamina WebKit.
 * Reset piscinae in adventu ad caudam VACUAM: chordae turmae
 * prioris tunc mortuae sunt (exhaustio praematura tuta - cauda
 * non-vacua numquam reficitur; crescentia limitata utroque modo). */
interior vacuum
_nuntium_inserere (Vitrea* vitrea, VitreaNuntiusGenus genus,
    constans character* octeti, i32 mensura)
{
    VitreaNuntiusSitus* situs;
    chorda copia;

    si (vitrea->nuntii_numerus >= VITREA_MAXIMUS_NUNTIORUM)
    {
        vitrea->fructus.nuntii_amissi++;
        /* amissio CLAMOSA: JS console.error - suspensio mysteriosa
         * fit titulata ([INT-1]) */
        [vitrea->textura evaluateJavaScript:
            @"internuntius._superfluum(1)" completionHandler:nil];
        redde;
    }
    si (vitrea->nuntii_numerus == ZEPHYRUM)
    {
        piscina_reficere(vitrea->piscina_nuntiorum,
            vitrea->nuntiorum_basis);
    }
    copia.mensura = mensura;
    copia.datum = NIHIL;
    si (mensura > ZEPHYRUM)
    {
        copia.datum = (i8*)piscina_allocare(
            vitrea->piscina_nuntiorum, (memoriae_index)mensura);
        si (copia.datum == NIHIL)
        {
            vitrea->fructus.nuntii_amissi++;
            redde;
        }
        memcpy(copia.datum, octeti, (memoriae_index)mensura);
    }
    situs = &vitrea->nuntii[vitrea->nuntii_index_caudae];
    situs->textus = copia;
    situs->genus = genus;
    vitrea->nuntii_index_caudae = (vitrea->nuntii_index_caudae + I)
        % VITREA_MAXIMUS_NUNTIORUM;
    vitrea->nuntii_numerus++;
    /* excitatio pumpae obstructae: eventum syntheticum quod
     * translator fenestrae vorat (contractus [SIM-1], in ferro
     * probatus [CAL]) */
    [NSApp postEvent:[NSEvent
            otherEventWithType:NSEventTypeApplicationDefined
            location:NSZeroPoint modifierFlags:0 timestamp:0
            windowNumber:0 context:nil subtype:0 data1:0 data2:0]
        atStart:NO];
}

/* ==================================================
 * runtime JS iniectum (spec par VIII; ES2017, sine dependentiis)
 * ================================================== */

interior constans character* constans VITREA_CURSUS_JS =
"(function(){\n"
"var pendentia = {};\n"
"var proximus = 1;\n"
"var auditores = {};\n"
"window.internuntius = {\n"
"  vocare: function(methodus, argumenta) {\n"
"    return new Promise(function(solvere, reicere) {\n"
"      var id = proximus++;\n"
"      pendentia[id] = { solvere: solvere, reicere: reicere };\n"
"      window.webkit.messageHandlers.internuntius.postMessage(\n"
"        JSON.stringify({ id: id, methodus: methodus,\n"
"                         argumenta: argumenta || {} }));\n"
"    });\n"
"  },\n"
"  audire: function(eventus, functio) {\n"
"    (auditores[eventus] = auditores[eventus] || []).push(functio);\n"
"  },\n"
"  _accipere: function(textus) {\n"
"    var n = JSON.parse(textus);\n"
"    if (n.eventus !== undefined) {\n"
"      var fs = auditores[n.eventus] || [];\n"
"      var i;\n"
"      for (i = 0; i < fs.length; i++) {\n"
"        try { fs[i](n.datum); } catch (e) { console.error(e); }\n"
"      }\n"
"      return;\n"
"    }\n"
"    if (n.id === undefined) {\n"
"      if (n.culpa) {\n"
"        console.error('internuntius culpa orba: ' +\n"
"          n.culpa.nuntius);\n"
"      }\n"
"      return;\n"
"    }\n"
"    var p = pendentia[n.id];\n"
"    if (!p) { return; }\n"
"    delete pendentia[n.id];\n"
"    if (n.culpa) { p.reicere(new Error(n.culpa.nuntius)); }\n"
"    else { p.solvere(n.fructus); }\n"
"  },\n"
"  _superfluum: function(n) {\n"
"    console.error('internuntius: ' + n +\n"
"      ' nuntia amissa (cauda plena) - promissa pendentia' +\n"
"      ' quaedam numquam solventur');\n"
"  }\n"
"};\n"
"})();";

/* ==================================================
 * classes delegatorum (contactus WebKit)
 * ================================================== */

@interface VitreaPortitor : NSObject <WKURLSchemeHandler>
@property (nonatomic, assign) Vitrea* vitrea;
@end

@implementation VitreaPortitor
- (vacuum)webView:(WKWebView*)textura
    startURLSchemeTask:(id<WKURLSchemeTask>)opus
{
    Vitrea* vitrea = self.vitrea;
    NSURL* url = opus.request.URL;
    PiscinaNotatio nota = piscina_notare(vitrea->piscina_petitionum);
    constans character* via_l;
    CapsulaFructus fructus;

    (vacuum)textura;
    vitrea->fructus.petitiones_capsulae++;
    /* normalizatio OBLIGATORIA (verdictum exploratoris): claves
     * capsulae sine '/' ductore, congruentia exacta, sine
     * ordinario indicis - portitor demit et mappat */
    {
        constans character* semita = [[url path] UTF8String];

        si (semita == NIHIL)
        {
            semita = "";
        }
        dum (*semita == '/')
        {
            semita++;
        }
        via_l = (*semita == '\0') ? vitrea->via_initialis : semita;
    }
    fructus = capsula_legere(vitrea->capsula, via_l,
        vitrea->piscina_petitionum);
    si (fructus.status != CAPSULA_OK)
    {
        vitrea->fructus.petitiones_fractae++;
        [opus didFailWithError:[NSError
            errorWithDomain:NSURLErrorDomain
            code:NSURLErrorFileDoesNotExist userInfo:nil]];
        piscina_reficere(vitrea->piscina_petitionum, nota);
        redde;
    }
    {
        /* NSHTTPURLResponse-200 OBLIGATORIUM: responsum planum dat
         * fetch status=0 / r.ok=false (verdictum geminum [CAL]) */
        NSData* corpus_data = [NSData
            dataWithBytes:(fructus.datum.mensura > ZEPHYRUM
                ? (constans vacuum*)fructus.datum.datum
                : (constans vacuum*)"")
            length:(NSUInteger)fructus.datum.mensura];
        character longitudo[XXXII];
        NSHTTPURLResponse* responsum;

        sprintf(longitudo, "%d", (int)fructus.datum.mensura);
        responsum = [[NSHTTPURLResponse alloc] initWithURL:url
            statusCode:200 HTTPVersion:@"HTTP/1.1"
            headerFields:@{
                @"Content-Type": [NSString stringWithUTF8String:
                    _mimen_pro_via(via_l)],
                @"Content-Length": [NSString stringWithUTF8String:
                    longitudo] }];
        [opus didReceiveResponse:responsum];
        [responsum release];
        [opus didReceiveData:corpus_data];
        [opus didFinish];
    }
    /* copia-et-refectio: WebKit copiam suam possidet (dataWithBytes)
     * - nulla copulatio vitae cum opere asynchrono */
    piscina_reficere(vitrea->piscina_petitionum, nota);
}
- (vacuum)webView:(WKWebView*)textura
    stopURLSchemeTask:(id<WKURLSchemeTask>)opus
{
    (vacuum)textura;
    (vacuum)opus;
}
@end

@interface VitreaAuscultator : NSObject <WKScriptMessageHandler>
@property (nonatomic, assign) Vitrea* vitrea;
@end

@implementation VitreaAuscultator
- (vacuum)userContentController:(WKUserContentController*)moderator
    didReceiveScriptMessage:(WKScriptMessage*)nuntius
{
    Vitrea* vitrea = self.vitrea;
    NSString* corpus;
    constans character* octeti;

    (vacuum)moderator;
    si (![nuntius.body isKindOfClass:[NSString class]])
    {
        redde;
    }
    corpus = (NSString*)nuntius.body;
    octeti = [corpus UTF8String];
    si (octeti == NIHIL)
    {
        redde;
    }
    vitrea->fructus.nuntii_recepti++;
    _nuntium_inserere(vitrea, VITREA_NUNTIUS_PONS, octeti,
        (i32)strlen(octeti));
}
@end

@interface VitreaLegatus : NSObject <WKNavigationDelegate>
@property (nonatomic, assign) Vitrea* vitrea;
@end

@implementation VitreaLegatus
- (vacuum)webViewWebContentProcessDidTerminate:(WKWebView*)textura
{
    (vacuum)textura;
    /* processus interretialis obiit - per eandem caudam ordinatam
     * livratur; app decernit (plerumque recargare). Promissa JS
     * cum pagina moriuntur - sponte sanans. */
    _nuntium_inserere(self.vitrea, VITREA_NUNTIUS_INTERITUS,
        NIHIL, ZEPHYRUM);
}
@end

/* ==================================================
 * vita
 * ================================================== */

Vitrea*
vitrea_creare (Piscina* piscina, Fenestra* fenestra,
    constans VitreaConfiguratio* configuratio)
{
    Vitrea* vitrea;

    si (piscina == NIHIL || fenestra == NIHIL
        || configuratio == NIHIL)
    {
        redde NIHIL;
    }
    si (configuratio->origo == VITREA_ORIGO_CAPSULA
        && configuratio->capsula == NIHIL)
    {
        redde NIHIL;
    }
    si (configuratio->origo == VITREA_ORIGO_URL
        && configuratio->url == NIHIL)
    {
        redde NIHIL;
    }
    vitrea = (Vitrea*)piscina_allocare(piscina,
        magnitudo(Vitrea));
    si (vitrea == NIHIL)
    {
        redde NIHIL;
    }
    memset(vitrea, ZEPHYRUM, magnitudo(Vitrea));
    vitrea->piscina = piscina;
    vitrea->fenestra = fenestra;
    vitrea->capsula = configuratio->capsula;
    vitrea->via_initialis = _litterae_copiare(piscina,
        configuratio->via_initialis != NIHIL
            ? configuratio->via_initialis : "index.html");
    vitrea->piscina_nuntiorum = piscina_generare_dynamicum(
        "vitrea_nuntii", 4194304);
    vitrea->piscina_petitionum = piscina_generare_dynamicum(
        "vitrea_petitiones", 8388608);
    vitrea->piscina_aestimationis = piscina_generare_dynamicum(
        "vitrea_aestimationes", 4194304);
    si (vitrea->piscina_nuntiorum == NIHIL
        || vitrea->piscina_petitionum == NIHIL
        || vitrea->piscina_aestimationis == NIHIL)
    {
        redde NIHIL;
    }
    vitrea->nuntiorum_basis = piscina_notare(
        vitrea->piscina_nuntiorum);

    @autoreleasepool {
        NSWindow* fenestra_ns = (NSWindow*)
            fenestra_obtinere_tractationem_nativam(fenestra);
        WKWebViewConfiguration* figura =
            [[WKWebViewConfiguration alloc] init];
        WKUserContentController* moderator =
            [[WKUserContentController alloc] init];
        WKUserScript* scriptum;

        vitrea->portitor = [[VitreaPortitor alloc] init];
        vitrea->portitor.vitrea = vitrea;
        vitrea->auscultator = [[VitreaAuscultator alloc] init];
        vitrea->auscultator.vitrea = vitrea;
        vitrea->legatus = [[VitreaLegatus alloc] init];
        vitrea->legatus.vitrea = vitrea;

        scriptum = [[WKUserScript alloc]
            initWithSource:[NSString stringWithUTF8String:
                VITREA_CURSUS_JS]
            injectionTime:WKUserScriptInjectionTimeAtDocumentStart
            forMainFrameOnly:YES];
        [moderator addUserScript:scriptum];
        [scriptum release];
        [moderator addScriptMessageHandler:vitrea->auscultator
            name:@"internuntius"];
        [figura setUserContentController:moderator];
        [moderator release];
        si (configuratio->origo == VITREA_ORIGO_CAPSULA)
        {
            [figura setURLSchemeHandler:vitrea->portitor
                forURLScheme:@"capsula"];
        }

        vitrea->textura = [[WKWebView alloc]
            initWithFrame:[[fenestra_ns contentView] frame]
            configuration:figura];
        [figura release];
        [vitrea->textura setNavigationDelegate:vitrea->legatus];
        si (@available(macOS 13.3, *))
        {
            si (configuratio->inspectabilis)
            {
                vitrea->textura.inspectable = YES;
            }
        }
        [vitrea->textura setAutoresizingMask:
            (NSViewWidthSizable | NSViewHeightSizable)];
        /* textura FIT contentum - visus pixellorum fenestrae
         * depellitur (Fenestra solum-textura per contractum;
         * fenestra_destruere manet tuta - retentionem PROPRIAM
         * dimittit) */
        [fenestra_ns setContentView:vitrea->textura];
        [fenestra_ns makeFirstResponder:vitrea->textura];

        {
            NSURL* url;

            si (configuratio->origo == VITREA_ORIGO_CAPSULA)
            {
                url = [NSURL URLWithString:[NSString
                    stringWithFormat:@"capsula://radix/%s",
                    vitrea->via_initialis]];
            }
            alioquin
            {
                url = [NSURL URLWithString:[NSString
                    stringWithUTF8String:configuratio->url]];
            }
            si (url != nil)
            {
                [vitrea->textura loadRequest:[NSURLRequest
                    requestWithURL:url]];
            }
        }
    }
    redde vitrea;
}

vacuum
vitrea_destruere (Vitrea* vitrea)
{
    si (vitrea == NIHIL)
    {
        redde;
    }
    @autoreleasepool {
        /* WKUserContentController tractatorem retinet - remotio
         * ANTE dimissionem (mos notus) */
        [[[vitrea->textura configuration] userContentController]
            removeScriptMessageHandlerForName:@"internuntius"];
        [vitrea->textura setNavigationDelegate:nil];
        [vitrea->textura stopLoading];
        [vitrea->textura release];
        [vitrea->portitor release];
        [vitrea->auscultator release];
        [vitrea->legatus release];
        /* fenestra retentionem contentView SUAM tenet donec
         * clauditur - vitrea_destruere ANTE fenestra_destruere */
    }
    piscina_destruere(vitrea->piscina_nuntiorum);
    piscina_destruere(vitrea->piscina_petitionum);
    piscina_destruere(vitrea->piscina_aestimationis);
}

/* ==================================================
 * cauda + aestimatio
 * ================================================== */

b32
vitrea_obtinere_nuntium (Vitrea* vitrea, chorda* nuntium,
    VitreaNuntiusGenus* genus)
{
    VitreaNuntiusSitus* situs;

    si (vitrea == NIHIL || nuntium == NIHIL || genus == NIHIL)
    {
        redde FALSUM;
    }
    si (vitrea->nuntii_numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    situs = &vitrea->nuntii[vitrea->nuntii_caput];
    *nuntium = situs->textus;
    *genus = situs->genus;
    vitrea->nuntii_caput = (vitrea->nuntii_caput + I)
        % VITREA_MAXIMUS_NUNTIORUM;
    vitrea->nuntii_numerus--;
    redde VERUM;
}

vacuum
vitrea_aestimare (Vitrea* vitrea, chorda javascript)
{
    si (vitrea == NIHIL || javascript.mensura == ZEPHYRUM)
    {
        redde;
    }
    @autoreleasepool {
        NSString* fons = [[NSString alloc]
            initWithBytes:javascript.datum
            length:(NSUInteger)javascript.mensura
            encoding:NSUTF8StringEncoding];

        si (fons == nil)
        {
            redde;
        }
        [vitrea->textura evaluateJavaScript:fons
            completionHandler:nil];
        [fons release];
        vitrea->fructus.aestimationes++;
    }
}

vacuum
vitrea_missor (vacuum* datum, chorda textus)
{
    Vitrea* vitrea = (Vitrea*)datum;
    PiscinaNotatio nota;
    ChordaAedificator* aed;
    chorda effugatum;

    si (vitrea == NIHIL)
    {
        redde;
    }
    nota = piscina_notare(vitrea->piscina_aestimationis);
    effugatum = internuntius_effugere_js(textus,
        vitrea->piscina_aestimationis);
    aed = chorda_aedificator_creare(vitrea->piscina_aestimationis,
        effugatum.mensura + XXXII);
    si (aed != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed,
            "internuntius._accipere(\"");
        chorda_aedificator_appendere_chorda(aed, effugatum);
        chorda_aedificator_appendere_literis(aed, "\")");
        vitrea_aestimare(vitrea, chorda_aedificator_finire(aed));
    }
    piscina_reficere(vitrea->piscina_aestimationis, nota);
}

vacuum
vitrea_recargare (Vitrea* vitrea)
{
    si (vitrea == NIHIL)
    {
        redde;
    }
    @autoreleasepool {
        [vitrea->textura reload];
    }
}

VitreaFructus
vitrea_fructus (constans Vitrea* vitrea)
{
    VitreaFructus vacua;

    si (vitrea == NIHIL)
    {
        memset(&vacua, ZEPHYRUM, magnitudo(vacua));
        redde vacua;
    }
    redde vitrea->fructus;
}
