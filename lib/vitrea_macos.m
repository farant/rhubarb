/* vitrea_macos.m - implementatio vitreae (vide vitrea.h; spec
 * project-specs/vitrea-spec-v2.md). CONTACTUS WEBKIT TOTUS hic
 * vivit - vitrea.h C89 purum manet. MRC (retentio manualis), SINE
 * litteris blocorum - mos fenestra_macos.m; completionHandler
 * semper nil. Politica navigationis (Phasis C): blocum RECEPTUM
 * (decisionHandler) vocamus - litterae blocorum vetitae manent
 * (decretum 2026-07-20). */

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

#include "vitrea.h"
#include "chorda_aedificator.h"
#include "internuntius.h"   /* effugator solus - directio sana */
#include "imago_png.h"      /* codificatio PORTABILIS - vide limes infra */
#include "mimen.h"
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
    /* origo pro politica navigationis (semel in creare extracta) */
    constans character* origo_schema;
    constans character* origo_hospes;
    s32                 origo_portus;   /* -I = nullus portus */
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

    /* Errores paginae: sutura optiva. NIHIL = clamor in stderr,
     * quod applicationi cuilibet SINE mutatione prodest. */
    VitreaErrator       errator;
    vacuum*             errator_datum;
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

/* Lineam consolae/erroris HUMANE imprimere.
 *
 * JSON crudum ad suturam it (applicatio campos vult); homo lineam
 * legit. '[vitrea/log] salve' longe utilius est quam
 * '{"genus":"log","nuntius":"salve","ubi":""}' in actis inter
 * lineas C mixtis.
 *
 * fflush semper: effusio ad FISTULAM (quod fit quando manus
 * applicationem generat aut cum acta in plagulam vertuntur) plene
 * bufferitur, et linea non effusa in ordine falso apparet - aut,
 * si applicatio cadit, omnino perit. Acta quae ordinem mentiuntur
 * peiora sunt quam nulla. */
interior vacuum
_lineam_imprimere (FILE* effusio, NSString* corpus,
    constans character* crudum)
{
    NSData*  datum = [corpus dataUsingEncoding:NSUTF8StringEncoding];
    NSError* error = nil;
    id       obiectum = nil;

    si (datum != nil)
    {
        obiectum = [NSJSONSerialization JSONObjectWithData:datum
            options:0 error:&error];
    }
    si (obiectum != nil
        && [obiectum isKindOfClass:[NSDictionary class]])
    {
        NSDictionary* d = (NSDictionary*)obiectum;
        NSString* genus  = [d objectForKey:@"genus"];
        NSString* textus = [d objectForKey:@"nuntius"];
        NSString* ubi    = [d objectForKey:@"ubi"];

        si (genus != nil && textus != nil)
        {
            si (ubi != nil && [ubi length] > 0)
            {
                fprintf(effusio, "[vitrea/%s] %s  (%s)\n",
                    [genus UTF8String], [textus UTF8String],
                    [ubi UTF8String]);
            }
            alioquin
            {
                fprintf(effusio, "[vitrea/%s] %s\n",
                    [genus UTF8String], [textus UTF8String]);
            }
            fflush(effusio);
            redde;
        }
    }
    /* Forma ignota: crudum potius quam nihil. */
    fprintf(effusio, "[vitrea] %s\n", crudum);
    fflush(effusio);
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
/* ERRORES PAGINAE -> C.
 *
 * Facies quae in via cecidit saepe eundem textum ostendit: nihil in
 * fenestra clamat, nihil in actis apparet, et exceptio ad usorem
 * tacita it. Consola WebKit eam habet - sed nemo consolam
 * applicationis vectae inspicit.
 *
 * CANALIS SEPARATUS ('erratum', non 'internuntius') CONSULTO: nuntii
 * pontis in caudam eunt quam applicatio exhaurit, et applicationes
 * omnes hodie quod PONS non est ut 'recarica' tractant. Error per
 * eandem viam missus ergo paginam in gyro cadendi-recargandi
 * verteret. Canalis alter nihil quod exstat tangit.
 *
 * INIECTIO AtDocumentStart: custodes ante scripta paginae ponuntur,
 * ergo errores ONERIS ipsius capiuntur - quos collector manus (post
 * aperire iniectus) numquam videre potest.
 *
 * Tres fontes quia nullus alterum capit: 'error' exceptiones non
 * captas; 'unhandledrejection' promissa reiecta (quae NIHIL usquam
 * scribunt); console.error quod codex ipse nuntiat. */
"(function(){\n"
"  function mittere(canalis, genus, nuntius, ubi) {\n"
"    try {\n"
"      window.webkit.messageHandlers[canalis].postMessage(\n"
"        JSON.stringify({ genus: genus, nuntius: String(nuntius),\n"
"                         ubi: ubi || '' }));\n"
"    } catch (e) { /* canalis abest: tacendum, ne gyrus fiat */ }\n"
"  }\n"
/* console.log('res', obiectum) DUO argumenta fert, et alterum
 * chorda non est. 'String(obiectum)' '[object Object]' redderet -
 * quod nihil docet et pessimum genus actorum est: linea quae adest
 * et tacet. Ergo JSON, cum reditu ad String pro circularibus (quae
 * stringify iactant) et pro undefined (quod stringify OMITTIT). */
"  function pingere(a) {\n"
"    if (typeof a === 'string') { return a; }\n"
"    if (a instanceof Error) { return a.message; }\n"
"    try {\n"
"      var s = JSON.stringify(a);\n"
"      return (s === undefined) ? String(a) : s;\n"
"    } catch (e) { return String(a); }\n"
"  }\n"
"  function iungere(argumenta) {\n"
"    return Array.prototype.map.call(argumenta, pingere).join(' ');\n"
"  }\n"
"  window.addEventListener('error', function (ev) {\n"
"    mittere('erratum', 'exceptio', ev.message || ev.error,\n"
"      ev.filename ? (ev.filename + ':' + ev.lineno) : '');\n"
"  });\n"
"  window.addEventListener('unhandledrejection', function (ev) {\n"
"    var r = ev.reason;\n"
"    mittere('erratum', 'promissum', (r && r.message) ? r.message : r,\n"
"      '');\n"
"  });\n"
/* Consola involuta - PRIOR semper vocatur, ergo inspector Safari
 * quod semper habebat retinet. Additio est, non substitutio. */
"  function involvere(titulus, canalis, genus) {\n"
"    var prior = console[titulus];\n"
"    console[titulus] = function () {\n"
"      mittere(canalis, genus, iungere(arguments), '');\n"
"      if (prior) { prior.apply(console, arguments); }\n"
"    };\n"
"  }\n"
"  involvere('error', 'erratum', 'console.error');\n"
"  involvere('warn',  'consola', 'monitum');\n"
"  involvere('log',   'consola', 'log');\n"
"  involvere('info',  'consola', 'info');\n"
"})();\n"
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
                    mimen_pro_via(via_l)],
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

    /* Canales 'erratum'/'consola' caudam pontis NON tangunt:
     * applicationes omnes quod PONS non est ut 'recarica' tractant,
     * ergo linea per caudam missa gyrum cadendi-recargandi
     * pareret. */
    si ([nuntius.name isEqualToString:@"erratum"])
    {
        vitrea->fructus.errores_paginae++;
        si (vitrea->errator != NIHIL)
        {
            /* Sutura JSON CRUDUM accipit: applicatio quae condit
             * campos vult, non lineam formatam. */
            vitrea->errator(vitrea->errator_datum, octeti,
                (i32)strlen(octeti));
        }
        alioquin
        {
            /* Ordinarius: clamare. Error paginae qui nusquam
             * apparet peior est quam nullus, quia fidem falsam
             * parit. */
            _lineam_imprimere(stderr, corpus, octeti);
        }
        redde;
    }

    /* console.log/warn/info -> stdout applicationis.
     *
     * CUR: qui faciem aedificat consolam WebKit habet - sed eam
     * aperire debet, et in applicatione vecta aut per manum agitata
     * eam omnino non habet. Effusio processus est ubi cetera acta
     * iam sunt, ergo linea faciei iuxta lineam C apparet, ordine
     * vero, sine inspectore.
     *
     * STDOUT, non stderr: log nuntius est, non defectus. Divisio
     * ordinaria servatur ut '2>/dev/null' errores solos taceat. */
    si ([nuntius.name isEqualToString:@"consola"])
    {
        vitrea->fructus.lineae_consolae++;
        _lineam_imprimere(stdout, corpus, octeti);
        redde;
    }

    vitrea->fructus.nuntii_recepti++;
    _nuntium_inserere(vitrea, VITREA_NUNTIUS_PONS, octeti,
        (i32)strlen(octeti));
}
@end

@interface VitreaLegatus : NSObject <WKNavigationDelegate,
    WKUIDelegate>
@property (nonatomic, assign) Vitrea* vitrea;
@end

@implementation VitreaLegatus
/* politica navigationis v1: origo configurata SOLA conceditur;
 * 'about' exemptum (paginae internae vacuae WebKit); cetera
 * recusata + numerata. Blocum RECEPTUM vocamus (decretum
 * 2026-07-20) - litteram bloci numquam scribimus. Petitiones
 * subresourcium (fetch, assets schematis) hac via NON transeunt -
 * navigationes solae. */
- (vacuum)webView:(WKWebView*)textura
    decidePolicyForNavigationAction:(WKNavigationAction*)actio
    decisionHandler:(vacuum (^)(WKNavigationActionPolicy))decisor
{
    Vitrea* vitrea = self.vitrea;
    NSURL* url = actio.request.URL;
    constans character* schema = [[url scheme] UTF8String];
    constans character* hospes = [[url host] UTF8String];
    s32 portus = [url port] != nil
        ? (s32)[[url port] intValue] : -I;

    (vacuum)textura;
    si (schema != NIHIL && strcmp(schema, "about") == ZEPHYRUM)
    {
        decisor(WKNavigationActionPolicyAllow);
        redde;
    }
    si (schema != NIHIL && hospes != NIHIL
        && strcmp(schema, vitrea->origo_schema) == ZEPHYRUM
        && strcmp(hospes, vitrea->origo_hospes) == ZEPHYRUM
        && portus == vitrea->origo_portus)
    {
        decisor(WKNavigationActionPolicyAllow);
        redde;
    }
    vitrea->fructus.navigationes_recusatae++;
    decisor(WKNavigationActionPolicyCancel);
}
- (vacuum)webViewWebContentProcessDidTerminate:(WKWebView*)textura
{
    (vacuum)textura;
    /* processus interretialis obiit - per eandem caudam ordinatam
     * livratur; app decernit (plerumque recargare). Promissa JS
     * cum pagina moriuntur - sponte sanans. */
    _nuntium_inserere(self.vitrea, VITREA_NUNTIUS_INTERITUS,
        NIHIL, ZEPHYRUM);
}
/* <input type="file"> nativum: WKWebView tabellam apertionis SOLUM
 * praebet si legatus UI hunc uncum implet - sine eo input mutum
 * sedet (mensuratum in foro 2026-07-31). runModal ansam eventuum
 * propriam AppKit gyrat, ergo a pumpa nostra non pendet; textura
 * dum tabella patet congelata - electioni plagulae aequum.
 * completionHandler SEMPER vocandus (contractus WebKit). */
- (vacuum)webView:(WKWebView*)textura
    runOpenPanelWithParameters:(WKOpenPanelParameters*)parametra
    initiatedByFrame:(WKFrameInfo*)margo
    completionHandler:(vacuum (^)(NSArray<NSURL*>*))perfector
{
    NSOpenPanel* tabella = [NSOpenPanel openPanel];

    (vacuum)textura;
    (vacuum)margo;
    [tabella setCanChooseFiles:YES];
    [tabella setCanChooseDirectories:NO];
    [tabella setAllowsMultipleSelection:
        [parametra allowsMultipleSelection]];
    si ([tabella runModal] == NSModalResponseOK)
    {
        perfector([tabella URLs]);
    }
    alioquin
    {
        perfector(nil);
    }
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
    si (configuratio->origo == VITREA_ORIGO_CAPSULA)
    {
        vitrea->origo_schema = "capsula";
        vitrea->origo_hospes = "radix";
        vitrea->origo_portus = -I;
    }
    alioquin
    {
        /* origo ex URL configurata semel extracta - politica
         * navigationis contra eam congruit. Extractio ANTE piscinas
         * dynamicas: fractura hic nihil effundit. */
        b32 fracta = FALSUM;

        @autoreleasepool {
            NSURL* origo_url = [NSURL URLWithString:[NSString
                stringWithUTF8String:configuratio->url]];
            constans character* s = origo_url != nil
                ? [[origo_url scheme] UTF8String] : NIHIL;
            constans character* h = origo_url != nil
                ? [[origo_url host] UTF8String] : NIHIL;

            si (s == NIHIL || h == NIHIL)
            {
                fracta = VERUM;
            }
            alioquin
            {
                vitrea->origo_schema = _litterae_copiare(piscina, s);
                vitrea->origo_hospes = _litterae_copiare(piscina, h);
                vitrea->origo_portus = [origo_url port] != nil
                    ? (s32)[[origo_url port] intValue] : -I;
            }
        }
        si (fracta)
        {
            redde NIHIL;
        }
    }
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
        [moderator addScriptMessageHandler:vitrea->auscultator
            name:@"erratum"];
        [moderator addScriptMessageHandler:vitrea->auscultator
            name:@"consola"];
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
        [vitrea->textura setUIDelegate:vitrea->legatus];
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
        [[[vitrea->textura configuration] userContentController]
            removeScriptMessageHandlerForName:@"erratum"];
        [[[vitrea->textura configuration] userContentController]
            removeScriptMessageHandlerForName:@"consola"];
        [vitrea->textura setNavigationDelegate:nil];
        [vitrea->textura setUIDelegate:nil];
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
vitrea_aestimator (vacuum* datum, chorda js)
{
    vitrea_aestimare((Vitrea*)datum, js);
}

/* Praemultiplicationem solvere: CG colores alpha praemultiplicat,
 * PNG non. Terminus superior necessarius est quia rotundatio
 * valorem supra CCLV ferre potest. */
interior i8
_depraemultiplicare (i32 color, i32 alpha)
{
    i32 valor;

    valor = (color * (i32)CCLV) / alpha;
    si (valor > (i32)CCLV)
    {
        valor = (i32)CCLV;
    }
    redde (i8)valor;
}

/*
 * _cg_in_png_scribere - CGImage in plagulam PNG per imago_png
 *
 * LIMES PLATFORMAE: pixela capere macOS est; PNG scribere non est.
 * Ideo hic pixela SOLA extrahuntur et codificator portabilis
 * cetera agit (vide lib/imago_png.worklog.md).
 */
interior b32
_cg_in_png_scribere (CGImageRef cg, NSString* semita)
{
    i32             lat, alt;
    i32             i, numerus;
    Piscina*        piscina;
    i8*             pixela;
    CGColorSpaceRef spatium;
    CGContextRef    contextus;
    Imago           imago;
    PngFructus      fructus;
    b32             successus;

    lat = (i32)CGImageGetWidth(cg);
    alt = (i32)CGImageGetHeight(cg);
    si (lat == ZEPHYRUM || alt == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Piscina PROPRIA et brevis: imagines magnae sunt et arena
     * singula non liberat - in vitrea->piscina sine fine crescerent */
    piscina = piscina_generare_dynamicum(
                  "vitrea_imago", (memoriae_index)(lat * alt * (i32)IV));
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }

    pixela = (i8*)piscina_allocare(
                 piscina, (memoriae_index)(lat * alt * (i32)IV));
    si (pixela == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }

    /* 32Big + PremultipliedLast = ordo memoriae R,G,B,A */
    spatium   = CGColorSpaceCreateDeviceRGB();
    contextus = CGBitmapContextCreate(
                    pixela, (size_t)lat, (size_t)alt, (size_t)VIII,
                    (size_t)(lat * (i32)IV), spatium,
                    kCGImageAlphaPremultipliedLast
                        | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(spatium);

    si (contextus == NULL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }

    CGContextDrawImage(
        contextus, CGRectMake(0, 0, (CGFloat)lat, (CGFloat)alt), cg);
    CGContextRelease(contextus);

    numerus = lat * alt;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i8* p;
        i32 alpha;

        p     = pixela + i * (i32)IV;
        alpha = (i32)p[III];

        si (alpha != ZEPHYRUM && alpha != (i32)CCLV)
        {
            p[0]  = _depraemultiplicare((i32)p[0],  alpha);
            p[I]  = _depraemultiplicare((i32)p[I],  alpha);
            p[II] = _depraemultiplicare((i32)p[II], alpha);
        }
    }

    imago.pixela   = pixela;
    imago.latitudo = lat;
    imago.altitudo = alt;

    fructus   = imago_png_scribere(&imago, [semita UTF8String], piscina);
    successus = fructus.successus;

    piscina_destruere(piscina);
    redde successus;
}

/* Imago: WKWebView contenta SUA reddit (takeSnapshot...), ergo
 * nulla permissio scrinii, nulla condicio de fenestra prima aut
 * obtecta - et res aliena in imagine apparere non potest. */
b32
vitrea_imaginem_petere (
    Vitrea*             vitrea,
    constans character* via,
    VitreaImagoFacta    facta,
    vacuum*             facta_datum)
{
    si (vitrea == NIHIL || vitrea->textura == nil || via == NIHIL)
    {
        redde FALSUM;
    }
    @autoreleasepool {
        WKSnapshotConfiguration* figura =
            [[WKSnapshotConfiguration alloc] init];
        /* NSString ante bloccum: bloccum obiectum ObjC retinet,
         * monstratorem C non - via vocantis mori potest */
        NSString* semita = [NSString stringWithUTF8String:via];

        [vitrea->textura takeSnapshotWithConfiguration:figura
            completionHandler:^(NSImage* imago, NSError* err) {
                b32 successus = FALSUM;

                si (imago != nil && err == nil)
                {
                    CGImageRef cg = [imago
                        CGImageForProposedRect:NULL
                        context:nil hints:nil];

                    si (cg != NULL)
                    {
                        successus = _cg_in_png_scribere(cg, semita);
                    }
                }
                si (facta != NIHIL)
                {
                    facta(facta_datum, successus);
                }
            }];
    }
    redde VERUM;
}

b32
vitrea_imaginator (
    vacuum*             datum,
    constans character* via,
    VitreaImagoFacta    facta,
    vacuum*             facta_datum)
{
    redde vitrea_imaginem_petere((Vitrea*)datum, via, facta,
        facta_datum);
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

vacuum
vitrea_erratorem_ponere (Vitrea* vitrea, VitreaErrator errator,
    vacuum* datum)
{
    si (vitrea == NIHIL)
    {
        redde;
    }
    vitrea->errator = errator;
    vitrea->errator_datum = datum;
}
