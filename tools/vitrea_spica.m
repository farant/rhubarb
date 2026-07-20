/* vitrea_spica.m — specimen calibrationis WebKit (2026-07-20).
 *
 * NON-domesticum CONSULTO (ObjC + ARC + blocks): haec spica figit
 * oraculum — WKWebView huius machinae — ANTE codicem productum,
 * eodem more quo specimina clang ante gradus examinis figebantur.
 * Quaestiones [VERIFY] ex project-specs/vitrea-spec.md §15:
 *
 *   I    secure-context paginarum capsula:// (crypto.subtle etc.)
 *        + resolutio URL relativorum per auctoritatem fictam
 *        + fetch() eiusdem schematis (NSHTTPURLResponse 200 vs
 *          NSURLResponse planum — specimen geminum)
 *        + localStorage (persistentia = cursu secundo visenda)
 *   II   foramen excitationis: nuntius pontis advenit dum pumpa
 *        obstructa BLOCAT — pumpa NON redit (ronda A)
 *   III  eventus syntheticus ApplicationDefined pumpam excitat
 *        (ronda B — contractus excitationis [SIM-1])
 *   IV   fetch mixtum: pagina capsula:// -> http://127.0.0.1
 *        (modus-dev: frontend capsula, servitium localhost)
 *   V    navigatio http nuda (ATS in executabili sine fasce)
 *   VI   captura pixelorum (canalis visionis vitrea-sonda)
 *
 * Curre per: tools/vitrea_spica.sh  (servum fixturae ponit)
 * Verdicta -> project-specs/vitrea-calibratio.md
 */

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

static BOOL g_paratum = NO;
static BOOL g_excitatum = NO;
static NSTimeInterval g_excitatum_ad = 0;
static NSTimeInterval g_t0 = 0;
static BOOL g_post_excitationem = NO;   /* ronda B: eventum ponere */
static BOOL g_missum = NO;

static void nota(NSString* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    NSString* s = [[NSString alloc] initWithFormat:fmt arguments:ap];
    va_end(ap);
    printf("%s\n", s.UTF8String);
    fflush(stdout);
}

static NSTimeInterval nunc(void)
{
    return [NSDate timeIntervalSinceReferenceDate];
}

/* pumpa segmentis brevibus dum vexillum aut tempus — fontes runloop
 * (nuntii scripti) inter eventa serviuntur */
static void pumpare_dum(BOOL* vexillum, NSTimeInterval secunda)
{
    NSTimeInterval finis = nunc() + secunda;
    while (!*vexillum && nunc() < finis) {
        NSEvent* ev = [NSApp nextEventMatchingMask:NSEventMaskAny
            untilDate:[NSDate dateWithTimeIntervalSinceNow:0.05]
            inMode:NSDefaultRunLoopMode dequeue:YES];
        if (ev) [NSApp sendEvent:ev];
    }
}

/* eventa pendentia exhaurire (ante rondas obstructionis) */
static void exhaurire(void)
{
    NSEvent* ev;
    while ((ev = [NSApp nextEventMatchingMask:NSEventMaskAny
        untilDate:[NSDate dateWithTimeIntervalSinceNow:0.1]
        inMode:NSDefaultRunLoopMode dequeue:YES]) != nil) {
        [NSApp sendEvent:ev];
    }
}

/* ronda obstructionis robusta contra strepitum muris: pumpat usque
 * ad ApplicationDefined (typus 15) aut tempus; eventa vera (motus
 * muris etc.) notantur ut contaminatio sed rondam NON finiunt -
 * cursus primus (2026-07-20) typis 5/9 ante nuntium contaminatus
 * erat. Redde typum finientem (0 = tempus exhaustum). */
static NSUInteger ronda_obstructa(NSTimeInterval secunda,
    NSTimeInterval* quando)
{
    NSTimeInterval finis = nunc() + secunda;

    while (nunc() < finis) {
        NSEvent* ev = [NSApp nextEventMatchingMask:NSEventMaskAny
            untilDate:[NSDate dateWithTimeIntervalSinceNow:finis - nunc()]
            inMode:NSDefaultRunLoopMode dequeue:YES];
        if (ev == nil) break;
        *quando = nunc();
        [NSApp sendEvent:ev];
        if (ev.type == NSEventTypeApplicationDefined) {
            return (NSUInteger)ev.type;
        }
        nota(@"    (eventum verum typus %lu ad +%.2fs - contaminatio,"
            " pergo obstruere)", (unsigned long)ev.type, nunc() - g_t0);
    }
    *quando = nunc();
    return 0;
}

/* ================= pagina specimen ================= */

static NSString* const PAGINA = @"<!doctype html><html><head>"
"<meta charset='utf-8'></head>"
"<body style='background:#1d2233;color:#dde;font:13px monospace;padding:1em'>"
"<h2>vitrea spica</h2><div id='st'>currens...</div>"
"<script>\n"
"const post = (o) => window.webkit.messageHandlers.spica.postMessage(o);\n"
"const spec = (t, v) => post({genus:'specimen', titulus:t, valor:String(v)});\n"
"window.spica_excita = (ms) => setTimeout(() => post({genus:'excita'}), ms);\n"
"window.spica_mixtum = (url) =>\n"
"  fetch(url).then(r => r.text().then(t => post({genus:'missum',\n"
"      titulus:'fetch-mixtum', valor:'ok status='+r.status+' corpus='+t.slice(0,40)})))\n"
"    .catch(e => post({genus:'missum', titulus:'fetch-mixtum',\n"
"      valor:'ERROR '+e.message}));\n"
"(async () => {\n"
"  spec('location.href', location.href);\n"
"  spec('location.origin', location.origin);\n"
"  spec('isSecureContext', window.isSecureContext);\n"
"  spec('crypto.subtle', typeof crypto.subtle);\n"
"  spec('crypto.randomUUID', typeof crypto.randomUUID);\n"
"  try { spec('localStorage.prior', localStorage.getItem('spica'));\n"
"        localStorage.setItem('spica', 'scriptum-' + Date.now());\n"
"        spec('localStorage.scribere', 'ok'); }\n"
"  catch (e) { spec('localStorage', 'ERROR ' + e.message); }\n"
"  try { const r = await fetch('capsula://radix/data.json');\n"
"        spec('fetch-absolutus-http200',\n"
"             'ok status=' + r.status + ' corpus=' + await r.text()); }\n"
"  catch (e) { spec('fetch-absolutus-http200', 'ERROR ' + e.message); }\n"
"  try { const r = await fetch('data.json');\n"
"        spec('fetch-relativus',\n"
"             'ok status=' + r.status + ' corpus=' + await r.text()); }\n"
"  catch (e) { spec('fetch-relativus', 'ERROR ' + e.message); }\n"
"  try { const r = await fetch('capsula://radix/data2.json');\n"
"        spec('fetch-nsurlresponse-planum',\n"
"             'ok status=' + r.status + ' corpus=' + await r.text()); }\n"
"  catch (e) { spec('fetch-nsurlresponse-planum', 'ERROR ' + e.message); }\n"
"  document.getElementById('st').textContent =\n"
"      'specimina missa ' + new Date().toISOString();\n"
"  post({genus:'paratum'});\n"
"})();\n"
"</script></body></html>";

/* ================= portitor schematis ================= */

@interface Portitor : NSObject <WKURLSchemeHandler>
@end
@implementation Portitor
- (void)webView:(WKWebView*)wv startURLSchemeTask:(id<WKURLSchemeTask>)task
{
    NSURL* url = task.request.URL;
    NSData* data = nil;
    NSString* mime = @"text/plain";
    BOOL http_responsum = YES;

    nota(@"  [portitor] petitio %@  (host='%@' path='%@')",
        url.absoluteString, url.host, url.path);
    if ([url.path isEqualToString:@"/index.html"]) {
        data = [PAGINA dataUsingEncoding:NSUTF8StringEncoding];
        mime = @"text/html";
    } else if ([url.path isEqualToString:@"/data.json"]) {
        data = [@"{\"salve\":\"http200\"}" dataUsingEncoding:NSUTF8StringEncoding];
        mime = @"application/json";
    } else if ([url.path isEqualToString:@"/data2.json"]) {
        /* geminum: NSURLResponse PLANUM (sine statu HTTP) */
        data = [@"{\"salve\":\"planum\"}" dataUsingEncoding:NSUTF8StringEncoding];
        mime = @"application/json";
        http_responsum = NO;
    }
    if (data == nil) {
        [task didFailWithError:[NSError errorWithDomain:@"spica"
            code:404 userInfo:nil]];
        return;
    }
    NSURLResponse* resp;
    if (http_responsum) {
        resp = [[NSHTTPURLResponse alloc] initWithURL:url statusCode:200
            HTTPVersion:@"HTTP/1.1" headerFields:@{
                @"Content-Type": mime,
                @"Content-Length": [NSString stringWithFormat:@"%lu",
                    (unsigned long)data.length] }];
    } else {
        resp = [[NSURLResponse alloc] initWithURL:url MIMEType:mime
            expectedContentLength:(NSInteger)data.length
            textEncodingName:@"utf-8"];
    }
    [task didReceiveResponse:resp];
    [task didReceiveData:data];
    [task didFinish];
}
- (void)webView:(WKWebView*)wv stopURLSchemeTask:(id<WKURLSchemeTask>)task
{
}
@end

/* ================= nuntius pontis ================= */

@interface Nuntius : NSObject <WKScriptMessageHandler>
@end
@implementation Nuntius
- (void)userContentController:(WKUserContentController*)ucc
    didReceiveScriptMessage:(WKScriptMessage*)m
{
    NSDictionary* d = m.body;
    NSString* genus = d[@"genus"];

    if ([genus isEqualToString:@"specimen"]) {
        nota(@"SPECIMEN %@ = %@", d[@"titulus"], d[@"valor"]);
    } else if ([genus isEqualToString:@"paratum"]) {
        g_paratum = YES;
    } else if ([genus isEqualToString:@"missum"]) {
        nota(@"SPECIMEN %@ = %@", d[@"titulus"], d[@"valor"]);
        g_missum = YES;
    } else if ([genus isEqualToString:@"excita"]) {
        g_excitatum = YES;
        g_excitatum_ad = nunc();
        nota(@"  [nuntius] 'excita' advenit in tractatore ad +%.2fs%@",
            g_excitatum_ad - g_t0,
            g_post_excitationem ? @" -> eventum syntheticum pono" : @"");
        if (g_post_excitationem) {
            NSEvent* ev = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                location:NSZeroPoint modifierFlags:0 timestamp:0
                windowNumber:0 context:nil subtype:0 data1:0 data2:0];
            [NSApp postEvent:ev atStart:NO];
        }
    }
}
@end

/* ================= legatus navigationis (ATS) ================= */

@interface Legatus : NSObject <WKNavigationDelegate>
@property (nonatomic) BOOL perfecta;
@property (nonatomic) BOOL fracta;
@property (nonatomic, strong) NSString* culpa;
@end
@implementation Legatus
- (void)webView:(WKWebView*)wv didFinishNavigation:(WKNavigation*)nav
{
    self.perfecta = YES;
}
- (void)webView:(WKWebView*)wv didFailProvisionalNavigation:(WKNavigation*)nav
    withError:(NSError*)error
{
    self.fracta = YES;
    self.culpa = error.localizedDescription;
}
- (void)webView:(WKWebView*)wv didFailNavigation:(WKNavigation*)nav
    withError:(NSError*)error
{
    self.fracta = YES;
    self.culpa = error.localizedDescription;
}
@end

/* ================= principale ================= */

int main(void)
{
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSWindow* fen = [[NSWindow alloc]
            initWithContentRect:NSMakeRect(140, 140, 560, 400)
            styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable)
            backing:NSBackingStoreBuffered defer:NO];
        fen.title = @"vitrea spica";

        WKWebViewConfiguration* cfg = [WKWebViewConfiguration new];
        Portitor* portitor = [Portitor new];
        [cfg setURLSchemeHandler:portitor forURLScheme:@"capsula"];
        Nuntius* nuntius = [Nuntius new];
        [cfg.userContentController addScriptMessageHandler:nuntius
            name:@"spica"];

        WKWebView* wv = [[WKWebView alloc]
            initWithFrame:((NSView*)fen.contentView).bounds
            configuration:cfg];
        wv.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        if (@available(macOS 13.3, *)) {
            wv.inspectable = YES;
        }
        [fen.contentView addSubview:wv];
        [fen makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        nota(@"== SPICA I: origo capsula:// — specimina paginae ==");
        [wv loadRequest:[NSURLRequest requestWithURL:
            [NSURL URLWithString:@"capsula://radix/index.html"]]];
        pumpare_dum(&g_paratum, 10.0);
        if (!g_paratum) nota(@"CAUTIO: 'paratum' non advenit intra 10s");

        nota(@"== SPICA II: ronda A — pumpa obstructa SINE eventu synthetico ==");
        exhaurire();
        g_excitatum = NO;
        g_post_excitationem = NO;
        [wv evaluateJavaScript:@"spica_excita(1500)" completionHandler:nil];
        g_t0 = nunc();
        {
            NSTimeInterval quando = g_t0;
            NSUInteger typus = ronda_obstructa(5.0, &quando);

            /* foramen probatum: nuntius ADVENIT (tractator currit dum
             * obstructa - runloop eum servit) sed obstructio TENET
             * usque ad tempus exhaustum */
            nota(@"  ronda A: nuntius advenit=%@ (ad +%.2fs);"
                " obstructio finita ad +%.2fs per %@",
                g_excitatum ? @"ITA" : @"NON",
                g_excitatum ? g_excitatum_ad - g_t0 : -1.0,
                quando - g_t0,
                typus == 0 ? @"TEMPUS EXHAUSTUM (foramen probatum)"
                    : [NSString stringWithFormat:@"typum %lu (?)",
                        (unsigned long)typus]);
        }

        nota(@"== SPICA III: ronda B — CUM eventu synthetico (contractus excitationis) ==");
        exhaurire();
        g_excitatum = NO;
        g_post_excitationem = YES;
        [wv evaluateJavaScript:@"spica_excita(1500)" completionHandler:nil];
        g_t0 = nunc();
        {
            NSTimeInterval quando = g_t0;
            NSUInteger typus = ronda_obstructa(5.0, &quando);

            nota(@"  ronda B: nuntius advenit=%@ (ad +%.2fs);"
                " obstructio finita ad +%.2fs per %@",
                g_excitatum ? @"ITA" : @"NON",
                g_excitatum ? g_excitatum_ad - g_t0 : -1.0,
                quando - g_t0,
                typus == 15
                    ? @"ApplicationDefined (excitatio confirmata)"
                    : typus == 0 ? @"TEMPUS EXHAUSTUM (excitatio FRACTA)"
                    : [NSString stringWithFormat:@"typum %lu (?)",
                        (unsigned long)typus]);
        }
        g_post_excitationem = NO;

        nota(@"== SPICA IV: fetch mixtum — pagina capsula -> http://127.0.0.1:8747 ==");
        g_missum = NO;
        [wv evaluateJavaScript:
            @"spica_mixtum('http://127.0.0.1:8747/data.json')"
            completionHandler:nil];
        pumpare_dum(&g_missum, 6.0);
        if (!g_missum) nota(@"SPECIMEN fetch-mixtum = (nullum responsum intra 6s)");

        nota(@"== SPICA V: navigatio http nuda (ATS, textura altera) ==");
        {
            Legatus* legatus = [Legatus new];
            WKWebView* wv2 = [[WKWebView alloc]
                initWithFrame:NSMakeRect(0, 0, 300, 200)
                configuration:[WKWebViewConfiguration new]];
            BOOL finis = NO;
            NSTimeInterval terminus = nunc() + 6.0;

            wv2.navigationDelegate = legatus;
            [wv2 loadRequest:[NSURLRequest requestWithURL:
                [NSURL URLWithString:@"http://127.0.0.1:8747/index.html"]]];
            while (!finis && nunc() < terminus) {
                NSEvent* ev = [NSApp nextEventMatchingMask:NSEventMaskAny
                    untilDate:[NSDate dateWithTimeIntervalSinceNow:0.05]
                    inMode:NSDefaultRunLoopMode dequeue:YES];
                if (ev) [NSApp sendEvent:ev];
                finis = legatus.perfecta || legatus.fracta;
            }
            nota(@"SPECIMEN navigatio-http = %@",
                legatus.perfecta ? @"PERFECTA (ATS non obstat)"
                : legatus.fracta ? [NSString stringWithFormat:@"FRACTA: %@",
                        legatus.culpa]
                : @"(tempus exhaustum)");
        }

        nota(@"== SPICA VI: captura pixelorum (canalis visionis) ==");
        {
            __block BOOL captum = NO;

            [wv takeSnapshotWithConfiguration:nil
                completionHandler:^(NSImage* img, NSError* err) {
                if (img != nil) {
                    CGImageRef cg = [img CGImageForProposedRect:NULL
                        context:nil hints:nil];
                    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc]
                        initWithCGImage:cg];
                    NSData* png = [rep
                        representationUsingType:NSBitmapImageFileTypePNG
                        properties:@{}];
                    [png writeToFile:@"tools/build/vitrea_spica_captura.png"
                        atomically:YES];
                    nota(@"SPECIMEN captura = scripta tools/build/"
                        "vitrea_spica_captura.png (%ldx%ld)",
                        (long)rep.pixelsWide, (long)rep.pixelsHigh);
                } else {
                    nota(@"SPECIMEN captura = ERROR %@",
                        err.localizedDescription);
                }
                captum = YES;
            }];
            pumpare_dum(&captum, 5.0);
        }

        nota(@"== FINIS — persistentia localStorage: cursu SECUNDO "
            "'localStorage.prior' non-null esse debet ==");
        return 0;
    }
}
