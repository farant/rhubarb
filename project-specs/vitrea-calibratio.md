# vitrea — calibratio (verdicta speciminum WebKit)

*2026-07-20. Specimen: tools/vitrea_spica.m (+ .sh), NON-domesticum
consulto (ObjC/ARC/blocks) — spica abiectiva quae oraculum figit ante
codicem productum, more speciminum clang ante gradus examinis.
Ferrum: macOS 14.8.5, Apple clang 16, WebKit systematis (Safari
currens). **RE-CURRENDUM post renovationes macOS** — WebKit cum
systemate mutat; scriptum idem verdicta refert.*

Omnia quinque [VERIFY] ex vitrea-spec.md §15 DECISA, plus contractus
excitationis [SIM-1] in ferro probatus, plus canalis visionis.

## Verdicta

**1. Secure context: ITA.** Paginae `capsula://` sunt secure contexts
(`isSecureContext=true`, `crypto.subtle=object`,
`crypto.randomUUID=function`). Historia doloris Tauri huic WebKit
NON applicat. Nihil in design flectendum.

**2. Auctoritas ficta: recte operatur.** `location.origin =
"capsula://radix"` — origo bene formata; `fetch('data.json')`
relativum contra eam recte resolvit (portitor `/data.json` vidit).
Delta [SIM-1] 1 confirmata.

**3. fetch eiusdem schematis: operatur, sed CLASSIS RESPONSI
PORTAT (specimen geminum).** Cum `NSHTTPURLResponse` statu 200:
`fetch` videt `status=200`, `r.ok=true`. Cum `NSURLResponse` PLANO:
corpus advenit sed `status=0` — ergo **`r.ok=false`** et omnis
frontend qui `r.ok` probat frangeretur. ⇒ **portitor vitreae DEBET
NSHTTPURLResponse cum statu 200 (et capitibus Content-Type/Length)
reddere, numquam NSURLResponse planum.** Spec §4 emendanda. CORS
intra schema: nullus — same-origin tractatur.

**4. Foramen excitationis: PROBATUM in ferro.** Nuntius pontis in
tractatore advenit ad +1.50s DUM pumpa obstructa (runloop eum
servit), sed obstructio TENUIT ad tempus exhaustum 5.0s — nullus
NSEvent natus. (Eventa systematis typi 13 interdum stillant — sed
"evigilare fortuna" est exacte modus quo app non exhaurit donec mus
movetur.) Delta [SIM-1] 3 necessaria confirmata.

**5. Excitatio synthetica: OPERATUR.** `postEvent:atStart:NO` cum
`NSEventTypeApplicationDefined` (typus 15) obstructionem statim
finit — nuntius ad +1.50s, pumpa rediit ad +1.50s. Contractus
excitationis validus ut spec'd. (Ronda B etiam tempestatem murum
typi-5 vera transiit — robustitia gratuita demonstrata.)

**6. fetch mixtum (capsula → http://127.0.0.1): operatur** cum
servus caput `Access-Control-Allow-Origin` mittit (CORS ordinarius
applicat; nullum obstaculum mixed-content, nullum ATS in fetch ad
loopback). Implicatio modi-dev: frontend in capsula:// potest
servitium localhost alloqui si servitium capita CORS reddit.

**7. ATS in executabili sine fasce: NON obstat.** Navigatio nuda ad
`http://127.0.0.1` PERFECTA sine Info.plist. Modus-dev sine fasce
viabilis; quaestio §4 ⚠ soluta.

**8. localStorage: operatur ET persistit** trans cursus in
executabili nudo. Sedes: `~/Library/WebKit/<nomen-processus>`
(`vitrea_spica` observatum) — repositorium stabile nomine binarii
clavatum, etiam sine fasce. Promissio §12 nunc dari potest.

**9. Captura (canalis visionis vitrea-sonda): PROBATA usque ad
oculos.** `takeSnapshotWithConfiguration` → PNG 1120×800 (retina 2×
fenestrae 560×400); Claude imaginem legit et contenta reddita
verificavit. Fundamentum empiricum dilationis vitrea-sonda (§14):
aestimare = manus, captura = oculi — CDP non necessarium.

## Sequelae in spec v2

- §4: portitor NSHTTPURLResponse-200 OBLIGATORIUS (verdictum 3).
- §4 ⚠-notae secure-context/fetch/ATS → verdicta (1, 6, 7).
- §12: sedes repositorii pro executabilibus nudis nominari potest.
- §15 evacuanda — omnia [VERIFY] decisa (praeter block-syntax sub
  -std=c89, quod discipulina sine-blocks moot facit).
