# vates plan 1 - smoke, the http seam, headers for review

> **For agentic workers:** execution mode is FIXED by Fran's standing
> rule (memory `plan-execution-inline-checkpoints`): inline, one task per
> turn, Fran approves each before the next; no subagents. Steps use
> checkbox (`- [ ]`) syntax for tracking.

**Goal:** prove the house HTTPS stack against the real services vates
will use, fix and extend `http` where vates and herbarium need it, and
put `herbarium.h` + `vates.h` in front of Fran before any
implementation.

**Architecture:** a hand-run smoke tool (never a gate) exercises TLS,
certificate refusals, long timeouts and the Anthropic API through the
unmodified house stack; the `http` library then gains the transport
seam (`HttpVectura`), a read view of requests, and the caller's timeout
on HTTPS; the two new headers are drafted, linted and compiled, with no
implementation. Plan 2 (implementation) is written only after Fran
approves the headers (Eskil's rule: the interface first).

**Tech stack:** C89 + latina.h, house libs (`http`, `tls`, `json`,
`filum`, `chorda`, `piscina`), credo, silva/commissio.

**Specs:** `project-specs/vates-spec.md`, `project-specs/herbarium-spec.md`
(and `optchat-spec.md` for context). Worktree `../rhubarb-quarta`
(memory `worktree-quarta`).

## Global constraints

- Flags: `tools/vexilla.sh` (`-std=c89 -pedantic -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings`); never declare flags by hand.
- Identifiers Latin; new non-oracle words need `oratio/glossarium.stml`
  entries (check: `VOCABULA_VIAE_ADDITAE=<via> ./oratio/vocabula.sh -nova`).
  `anthropic` is a proper noun (`classis="nomen-proprium"`).
- Never use a latina.h macro word as an identifier (`nomen`, `per`,
  `casus`, `duplex`, `magnitudo`, ...); Roman numerals are macros.
- `chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED, `s32`/`s64`
  signed.
- POSIX `.c` files include `postulata_posix.h` FIRST.
- The API key is read in-process from `~/.rhubarb/anthropic.clavis`
  and is never printed, logged, put in argv or env, or read by Claude.
- Worktree commits: `silva.commissio` with gates named by hand,
  `sine_debitis='<cause>'` always (never `tools/portae_debitae.sh`
  here), never `./tools/instituere.sh` or any `~/.bin` install.
- Every new behaviour is born red by a plant that COMPILES
  (`volatilis` if the optimizer could delete it), then restored with
  plain `cp`/edit (never `cp -p`).

## Review focus

1. **TLS 1.0 / 1.1 accepted by SecureTransport.** A reasonable person
   expects a modern client to refuse them; T1 probes
   `tls-v1-0.badssl.com:1010` and `tls-v1-1.badssl.com:1011` and
   expects refusal. A BENE here would be a finding, not a pass to
   ignore.
2. **The caller's timeout ignored on HTTPS** (the bug): T1 `/delay/10`
   with a 3 s timeout SUCCEEDS before T2 (red: the 30 s TLS default
   wins) and fails with `HTTP_ERROR_TIMEOUT` after T2 (green). This is
   the same plumbing a 10-minute model call needs. (httpbin caps
   `/delay/n` at 10 s, so a >30 s case cannot be staged there.)
3. **A generous timeout breaking ordinary calls** (the control):
   `/delay/5` with 20 s must succeed before AND after T2.
4. **The key leaking through an error path** (a failed request whose
   `error_descriptio` or the tool's own print includes header values):
   T1 prints only status, counts and model ids; Step T1.4 greps the
   tool output for the key's first 12 characters and expects nothing.
5. **A request view that lies** (`http_petitio_visus` returning
   defaults for a parsed URL with explicit port, query and body): T2's
   test asserts every field of a non-trivial request.

---

### Task 1: smoke probe of the unmodified house stack

**Files:**
- Create: `tools/rete_fumus.c`, `tools/rete_fumus.sh`
- Fran creates (not in repo): `~/.rhubarb/anthropic.clavis`

**Interfaces:**
- Consumes: `http_petitio_creare`, `http_petitio_caput_addere`,
  `http_petitio_tempus_ponere`, `http_exsequi`, `json_legere`,
  `json_capere_chorda`, `json_objectum_capere`,
  `json_tabulatum_numerus`, `json_tabulatum_obtinere`,
  `filum_existit`, `filum_legere_totum`, `chorda_praecidere`.
- Produces: `./tools/rete_fumus.sh [-tls] [-badssl] [-mora]
  [-anthropic]`, exit 0 iff every expectation held. Used again after
  T2 (mora) and in plan 2 (live vates smoke builds beside it).

- [ ] **Step 1.1: Fran writes the key file** (Claude never sees it).
  The key must belong to the Console organization linked to the Max
  plan's monthly credits. Suggested, typed by Fran with the key on the
  clipboard:

```bash
! mkdir -p ~/.rhubarb && (umask 077 && pbpaste | tr -d '\n' > ~/.rhubarb/anthropic.clavis) && ls -l ~/.rhubarb/anthropic.clavis
```

  Expected: `-rw-------` and a size around 100 bytes.

- [ ] **Step 1.2: write the tool** (`./silva/scribe.sh tools/rete_fumus.c <<'EOF'` ... `EOF`)

```c
/* rete_fumus.c - fumus retis: TLS, refusationes certificatorum,
 * tempora longa et brevia, API Anthropic (vates-plan-1 T1).
 *
 * NON PORTA: rete (et API) poscit - manu curritur, numquam in suite.
 *
 * Curre per: ./tools/rete_fumus.sh [-tls] [-badssl] [-mora] [-anthropic]
 * Sine optionibus omnia. Exitus 0 = omnes exspectationes impletae.
 *
 * Clavis: ~/.rhubarb/anthropic.clavis (0600, a Frano scripta). In
 * processu legitur, in caput x-api-key ponitur, NUMQUAM imprimitur.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "http.h"
#include "json.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens i32 _fracti = 0;

interior vacuum
_nuntiare (
    constans character* titulus,
                   b32  bene,
    constans character* nota)
{
    imprimere("%s  %-46s %s\n", bene ? "BENE " : "FRACT", titulus,
              nota ? nota : "");
    si (!bene)
    {
        _fracti++;
    }
}

interior HttpResultus
_petere (
               Piscina* piscina,
    constans character* url,
           MoraAngusta  tempus_ms)
{
    HttpPetitio* petitio;
    HttpResultus res;

    petitio = http_petitio_creare(piscina, HTTP_GET, url);
    si (!petitio)
    {
        memset(&res, 0, magnitudo(res));
        res.error = HTTP_ERROR_URL;
        redde res;
    }
    si (tempus_ms > 0)
    {
        http_petitio_tempus_ponere(petitio, tempus_ms);
    }
    redde http_exsequi(petitio, piscina);
}

/* ---- I. quam versionem TLS tractamus? ---- */
interior vacuum
_probare_tls (
    Piscina* piscina)
{
    HttpResultus res;
    JsonResultus json;
    character    nota[CCLVI];
    chorda       versio;
    chorda       gradus;

    res = _petere(piscina, "https://www.howsmyssl.com/a/check", 0);
    si (!res.successus || res.responsum->status != CC)
    {
        _nuntiare("howsmyssl: responsum CC", FALSUM,
                  http_error_descriptio(res.error));
        redde;
    }
    json = json_legere(res.responsum->corpus, piscina);
    si (!json.successus)
    {
        _nuntiare("howsmyssl: JSON", FALSUM, "non legibile");
        redde;
    }
    versio = json_capere_chorda(json.radix, "tls_version",
                                chorda_ex_literis("?", piscina));
    gradus = json_capere_chorda(json.radix, "rating",
                                chorda_ex_literis("?", piscina));
    sprintf(nota, "%.*s, gradus %.*s",
            (integer)versio.mensura, (constans character*)versio.datum,
            (integer)gradus.mensura, (constans character*)gradus.datum);
    _nuntiare("howsmyssl: versio TLS tractata", VERUM, nota);
}

/* ---- II. quod recusare debemus, recusamusne? ---- */
interior vacuum
_probare_recusationem (
               Piscina* piscina,
    constans character* url,
                   b32  recusandum)
{
    HttpResultus res;
    character    titulus[CCLVI];

    res = _petere(piscina, url, XV * M);
    sprintf(titulus, "%s %s", recusandum ? "recusat" : "accipit", url + VIII);
    si (recusandum)
    {
        _nuntiare(titulus, !res.successus,
                  res.successus ? "ACCEPTUM - exspectabatur recusatio" : "");
    }
    alioquin
    {
        _nuntiare(titulus, res.successus,
                  res.successus ? "" : http_error_descriptio(res.error));
    }
}

interior vacuum
_probare_badssl (
    Piscina* piscina)
{
    _probare_recusationem(piscina, "https://expired.badssl.com/", VERUM);
    _probare_recusationem(piscina, "https://wrong.host.badssl.com/", VERUM);
    _probare_recusationem(piscina, "https://self-signed.badssl.com/", VERUM);
    _probare_recusationem(piscina, "https://untrusted-root.badssl.com/", VERUM);
    _probare_recusationem(piscina, "https://tls-v1-0.badssl.com:1010/", VERUM);
    _probare_recusationem(piscina, "https://tls-v1-1.badssl.com:1011/", VERUM);
    _probare_recusationem(piscina, "https://tls-v1-2.badssl.com:1012/", FALSUM);
}

/* ---- III. tempus vocantis in via HTTPS ---- */
interior vacuum
_probare_moram (
    Piscina* piscina)
{
    HttpResultus res;

    /* longa (custodia): responsum post V s, tempus XX s - succedere
     * debet ante et post emendationem. httpbin /delay/n ad X s
     * limitatur: casus > XXX s hic fingi non potest. */
    res = _petere(piscina, "https://httpbin.org/delay/5", XX * M);
    _nuntiare("mora longa: /delay/5 cum tempore XX s",
              res.successus && res.responsum->status == CC,
              res.successus ? "" : http_error_descriptio(res.error));

    /* brevis: responsum post X s, tempus III s - debet deficere TEMPUS */
    res = _petere(piscina, "https://httpbin.org/delay/10", III * M);
    _nuntiare("mora brevis: /delay/10 cum tempore III s",
              !res.successus && res.error == HTTP_ERROR_TIMEOUT,
              res.successus ? "SUCCESSIT - tempus neglectum"
                            : http_error_descriptio(res.error));
}

/* ---- IV. API Anthropic: GET /v1/models (gratis) ---- */
interior vacuum
_probare_anthropic (
    Piscina* piscina)
{
    constans character* domus;
    character           via[MXXIV];
    character           clavis[DXII];
    chorda              crudum;
    HttpPetitio*        petitio;
    HttpResultus        res;
    JsonResultus        json;
    JsonValor*          data;
    character           nota[DXII];
    i32                 numerus;
    i32                 i;

    domus = getenv("HOME");
    si (!domus)
    {
        _nuntiare("anthropic: HOME", FALSUM, "HOME non positum");
        redde;
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    si (!filum_existit(via))
    {
        _nuntiare("anthropic: clavis", FALSUM,
                  "~/.rhubarb/anthropic.clavis abest (T1 gradus 1.1)");
        redde;
    }
    crudum = chorda_praecidere(filum_legere_totum(via, piscina));
    si (crudum.mensura == 0 || crudum.mensura >= (i32)magnitudo(clavis))
    {
        _nuntiare("anthropic: clavis", FALSUM, "vacua aut nimis longa");
        redde;
    }
    memcpy(clavis, crudum.datum, (size_t)crudum.mensura);
    clavis[crudum.mensura] = '\0';

    petitio = http_petitio_creare(piscina, HTTP_GET,
                                  "https://api.anthropic.com/v1/models");
    http_petitio_caput_addere(petitio, "x-api-key", clavis);
    http_petitio_caput_addere(petitio, "anthropic-version", "2023-06-01");
    res = http_exsequi(petitio, piscina);
    memset(clavis, 0, magnitudo(clavis));

    si (!res.successus)
    {
        _nuntiare("anthropic: /v1/models", FALSUM,
                  http_error_descriptio(res.error));
        redde;
    }
    si (res.responsum->status != CC)
    {
        sprintf(nota, "status %d", (integer)res.responsum->status);
        _nuntiare("anthropic: /v1/models", FALSUM, nota);
        redde;
    }
    json = json_legere(res.responsum->corpus, piscina);
    data = json.successus ? json_objectum_capere(json.radix, "data") : NIHIL;
    numerus = data ? json_tabulatum_numerus(data) : 0;
    sprintf(nota, "%u exemplaria", numerus);
    _nuntiare("anthropic: /v1/models", numerus > 0, nota);
    per (i = 0; i < numerus && i < VIII; i++)
    {
        chorda id = json_capere_chorda(json_tabulatum_obtinere(data, i),
                                       "id", chorda_ex_literis("?", piscina));
        imprimere("        %.*s\n", (integer)id.mensura,
                  (constans character*)id.datum);
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina;
         b32 omnia;
         b32 tls_      = FALSUM;
         b32 badssl    = FALSUM;
         b32 mora      = FALSUM;
         b32 anthropic = FALSUM;
     integer i;

    per (i = I; i < argc; i++)
    {
        si      (strcmp(argv[i], "-tls") == 0)       tls_      = VERUM;
        alioquin si (strcmp(argv[i], "-badssl") == 0)    badssl    = VERUM;
        alioquin si (strcmp(argv[i], "-mora") == 0)      mora      = VERUM;
        alioquin si (strcmp(argv[i], "-anthropic") == 0) anthropic = VERUM;
        alioquin
        {
            fprintf(stderr, "optio ignota: %s\n", argv[i]);
            redde II;
        }
    }
    omnia = !tls_ && !badssl && !mora && !anthropic;

    piscina = piscina_generare_dynamicum("rete_fumus", M * M);
    si (!piscina)
    {
        redde II;
    }
    si (omnia || tls_)      _probare_tls(piscina);
    si (omnia || badssl)    _probare_badssl(piscina);
    si (omnia || mora)      _probare_moram(piscina);
    si (omnia || anthropic) _probare_anthropic(piscina);

    imprimere("\nfumus retis: %s (%u fracti)\n",
              _fracti == 0 ? "sanum" : "FRACTUM", _fracti);
    piscina_destruere(piscina);
    redde _fracti == 0 ? 0 : I;
}
EOF
```

  Note on the `si (...) x = VERUM;` single-statement lines: if the
  house formatter or examen rejects brace-less bodies, give each its
  braces; behaviour is unchanged.

- [ ] **Step 1.3: write the wrapper** (`tools/rete_fumus.sh`, mode 755)

```bash
#!/bin/bash
# tools/rete_fumus.sh - fumus retis (vates-plan-1 T1): TLS, badssl,
# tempora HTTPS, API Anthropic. NON PORTA: rete poscit, manu curritur.
# Usus: ./tools/rete_fumus.sh [-tls] [-badssl] [-mora] [-anthropic]
#       (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/rete_fumus.c >&2 || exit 1
bash build/aedilis/rete_fumus/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/rete_fumus/rete_fumus "$@"
```

- [ ] **Step 1.4: build and run all four probes, record results**

Run: `AEDIFICARE_SOLUM=1 ./tools/rete_fumus.sh && ./tools/rete_fumus.sh | tee build/rete_fumus_T1.txt`

Expected BEFORE T2:
- `howsmyssl` BENE with the negotiated version printed (record it: it
  answers vates-spec AUDIENDA "TLS 1.2 from SecureTransport").
- badssl: four certificate refusals BENE; `tls-v1-2` BENE;
  `tls-v1-0` / `tls-v1-1` - record whatever happens (Review focus 1).
- `mora longa` BENE (control).
- `mora brevis` **FRACT** "SUCCESSIT - tempus neglectum": the 3 s is
  ignored on HTTPS, the 30 s TLS default waits out the 10 s delay.
  This is the bug, observed - the red half of T2's proof.
- `anthropic` BENE with a model count and up to eight ids.

Then the key-leak check (Review focus 4) - by Fran, since it reads the key:

```bash
! grep -c "$(head -c 12 ~/.rhubarb/anthropic.clavis)" build/rete_fumus_T1.txt
```

Expected: `0`.

- [ ] **Step 1.5: worklog + lint + commit**

Append to `lib/http.worklog.md` a dated note with the T1 table (TLS
version, refusal results, both mora results). Then:

```bash
VOCABULA_VIAE_ADDITAE=tools/rete_fumus.c ./oratio/vocabula.sh -nova
```

Add a glossary entry for every word it reports unknown (`anthropic`
as `nomen-proprium`; others as their real Latin class). Before this
first CODE commit in quarta, run the fabrica SANATIO list from the
4076ee93 refusal MINUS every `./tools/instituere.sh` line (memory
`worktree-quarta`). Commit:

```python
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio(
    "vates-plan-1 T1: tools/rete_fumus - fumus retis (TLS, badssl, tempora HTTPS, /v1/models) ...",
    ['tools/rete_fumus.c', 'tools/rete_fumus.sh', 'lib/http.worklog.md',
     'oratio/glossarium.stml'],
    portae=['oratio'],
    sine_debitis='arbor quarta: instrumentum manu curritur, non in suite; oratio pro glossario nominata')
```

---

### Task 2: http - caller's timeout on HTTPS, the transport seam, a request view

**Files:**
- Modify: `include/http.h` (comment at 155-158; new section after 206)
- Modify: `lib/http.c:796-812` (https connect), append new functions
  at end of file
- Modify: `probationes/probatio_http.c` (two tests + main)
- Modify: `lib/http.worklog.md`

**Interfaces:**
- Consumes: `tls_optiones_default`, `tls_connectere_cum_optionibus`
  (`include/tls.h:86-95`), `_creare_error` (`lib/http.c`, interior).
- Produces (herbarium + vates rely on these exact names):

```c
nomen structura {
    HttpResultus (*exsequi)(HttpPetitio* petitio, Piscina* piscina,
                            vacuum* datum);
    vacuum* datum;
} HttpVectura;

HttpVectura      http_vectura_ordinaria (vacuum);
HttpResultus     http_vectura_exsequi (HttpVectura vectura,
                     HttpPetitio* petitio, Piscina* piscina);

nomen structura {
          HttpMethodus  methodus;
                chorda  schema;
                chorda  hospes;
                   i32  portus;
                chorda  via;
                chorda  corpus;
    constans HttpCaput* capita;
                   i32  capita_numerus;
           MoraAngusta  tempus_ms;
} HttpPetitioVisus;

HttpPetitioVisus http_petitio_visus (constans HttpPetitio* petitio);
```

- [ ] **Step 2.1: write the failing tests** in `probationes/probatio_http.c`
  (before `principale`)

```c
/* ========================================================================
 * PROBATIONES - VISUS PETITIONIS ET VECTURA (vates-plan-1 T2)
 * ======================================================================== */

interior vacuum
probatio_petitio_visus(Piscina* piscina)
{
    HttpPetitio*     pet;
    HttpPetitioVisus v;

    printf("--- Probans http_petitio_visus ---\n");

    pet = http_petitio_creare(piscina, HTTP_POST,
        "https://api.example.com:8443/v1/messages?beta=1");
    CREDO_NON_NIHIL(pet);
    http_petitio_caput_addere(pet, "content-type", "application/json");
    http_petitio_caput_addere(pet, "anthropic-version", "2023-06-01");
    http_petitio_corpus_ponere(pet, "{\"a\":1}", VII);
    http_petitio_tempus_ponere(pet, DC * M);

    v = http_petitio_visus(pet);
    CREDO_VERUM(v.methodus == HTTP_POST);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.schema, "https");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.hospes, "api.example.com");
    CREDO_AEQUALIS_I32(v.portus, 8443);
    CREDO_CHORDA_INCIPIT(v.via, "/v1/messages");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.corpus, "{\"a\":1}");
    CREDO_AEQUALIS_I32(v.capita_numerus, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.capita[I].titulus, "anthropic-version");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.capita[I].valor, "2023-06-01");
    CREDO_AEQUALIS_S32(v.tempus_ms, DC * M);

    /* NIHIL -> visus vacuus, non ruina */
    v = http_petitio_visus(NIHIL);
    CREDO_AEQUALIS_I32(v.capita_numerus, 0);
    CREDO_NIHIL(v.capita);

    printf("\n");
}

hic_manens i32 _vectura_ficta_vocata = 0;

interior HttpResultus
_vectura_ficta (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    HttpResultus   res;
    HttpResponsum* resp;

    (vacuum)petitio;
    _vectura_ficta_vocata += *(i32*)datum;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = CDXVIII;
    resp->corpus = chorda_ex_literis("ficta", piscina);
    memset(&res, 0, magnitudo(res));
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

interior vacuum
probatio_vectura(Piscina* piscina, i32 portus)
{
    HttpVectura  v;
    HttpPetitio* pet;
    HttpResultus res;
    character    url[CCLVI];
    i32          incrementum = VII;

    printf("--- Probans HttpVectura ---\n");

    /* ordinaria = http_exsequi verum, contra fixturam localem */
    pet = http_petitio_creare(piscina, HTTP_GET,
                              _url_fixturae(url, portus, "/get"));
    res = http_vectura_exsequi(http_vectura_ordinaria(), pet, piscina);
    CREDO_VERUM(res.successus);
    si (res.successus)
    {
        CREDO_AEQUALIS_I32(res.responsum->status, CC);
        CREDO_CHORDA_CONTINET(res.responsum->corpus, "fixtura localis");
    }

    /* ficta: functio vocatur cum datis suis, responsum eius redditur */
    v.exsequi = _vectura_ficta;
    v.datum   = &incrementum;
    res = http_vectura_exsequi(v, pet, piscina);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(_vectura_ficta_vocata, VII);
    CREDO_AEQUALIS_I32(res.responsum->status, CDXVIII);

    /* sine functione: error nominatus, non ruina */
    v.exsequi = NIHIL;
    res = http_vectura_exsequi(v, pet, piscina);
    CREDO_FALSUM(res.successus);
    CREDO_VERUM(res.error == HTTP_ERROR_CONNEXIO);

    printf("\n");
}
```

  In `principale`: call `probatio_petitio_visus(piscina);` after
  `probatio_petitio_creare(piscina);`, and `probatio_vectura(piscina,
  portus);` inside the `si (portus > 0)` block after
  `probatio_status_codes`.

- [ ] **Step 2.2: run, expect a compile failure** (the names do not
  exist yet)

Run: `./compile_tests.sh probatio_http`
Expected: FAIL - `implicit declaration of function 'http_petitio_visus'`
(or unknown type `HttpPetitioVisus`).

- [ ] **Step 2.3: add the declarations to `include/http.h`** after
  `http_exsequi_cum_redirectionibus` (line 206), and rewrite the
  comment at 155-158:

```c
/* Ponere tempus receptionis/connexionis (millisecunda; 0 = defaltum
 * XXX secundorum). Valet in semita plana ET https (vates-plan-1 T2:
 * olim https ordinarium tls XXX s semper habebat). Tempore icto
 * http_exsequi HTTP_ERROR_TIMEOUT reddit, corpore partiali consulto
 * abiecto. */
```

```c
/* ========================================================================
 * VECTURA - sutura transportus (herbarium-spec par. V)
 *
 * Functio quae petitionem exsequitur + datum eius. Ordinaria =
 * http_exsequi. Involucra (herbarium) vecturam interiorem in datum
 * tenent; probationes vecturam scriptam inserunt.
 * ======================================================================== */

nomen structura {
    HttpResultus (*exsequi)(HttpPetitio* petitio, Piscina* piscina,
                            vacuum* datum);
    vacuum* datum;
} HttpVectura;

/* Vectura ordinaria: http_exsequi, datum NIHIL. */
HttpVectura
http_vectura_ordinaria (vacuum);

/* Exsequi per vecturam. exsequi NIHIL -> HTTP_ERROR_CONNEXIO
 * nominatus, numquam ruina. */
HttpResultus
http_vectura_exsequi (
    HttpVectura  vectura,
    HttpPetitio* petitio,
       Piscina*  piscina);

/* Visus lectionis petitionis - petitio opaca manet. Campi ut dati
 * sunt; capita in petitione ipsa iacent (non mutanda). NIHIL ->
 * visus vacuus (capita NIHIL, numeri 0). */
nomen structura {
          HttpMethodus  methodus;
                chorda  schema;
                chorda  hospes;
                   i32  portus;
                chorda  via;
                chorda  corpus;
    constans HttpCaput* capita;
                   i32  capita_numerus;
           MoraAngusta  tempus_ms;
} HttpPetitioVisus;

HttpPetitioVisus
http_petitio_visus (
    constans HttpPetitio* petitio);
```

- [ ] **Step 2.4: implement in `lib/http.c`** - append at end of file:

```c
/* ========================================================================
 * VECTURA ET VISUS (vates-plan-1 T2; herbarium-spec par. V)
 * ======================================================================== */

interior HttpResultus
_vectura_ordinaria_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    (vacuum)datum;
    redde http_exsequi(petitio, piscina);
}

HttpVectura
http_vectura_ordinaria (vacuum)
{
    HttpVectura v;

    v.exsequi = _vectura_ordinaria_exsequi;
    v.datum   = NIHIL;
    redde v;
}

HttpResultus
http_vectura_exsequi (
    HttpVectura  vectura,
    HttpPetitio* petitio,
       Piscina*  piscina)
{
    si (vectura.exsequi == NIHIL)
    {
        redde _creare_error(HTTP_ERROR_CONNEXIO,
            "Vectura sine functione exsequendi", piscina);
    }
    redde vectura.exsequi(petitio, piscina, vectura.datum);
}

HttpPetitioVisus
http_petitio_visus (
    constans HttpPetitio* petitio)
{
    HttpPetitioVisus v;

    memset(&v, 0, magnitudo(v));
    si (!petitio)
    {
        v.capita = NIHIL;
        redde v;
    }
    v.methodus       = petitio->methodus;
    v.schema         = petitio->schema;
    v.hospes         = petitio->hospes;
    v.portus         = petitio->portus;
    v.via            = petitio->via;
    v.corpus         = petitio->corpus;
    v.capita         = petitio->capita;
    v.capita_numerus = petitio->capita_numerus;
    v.tempus_ms      = petitio->tempus_ms;
    redde v;
}
```

  And replace the https connect block (`lib/http.c:797-811`):

```c
    si (est_https)
    {
        TlsOptiones tls_opt = tls_optiones_default();
        TlsResultus tls_res;

        /* tempus vocantis etiam in https (vates-plan-1 T2): olim
         * tls_connectere sine optionibus = XXX s semper, et vocatio
         * exemplaris sine fluxu longior frangebatur */
        si (petitio->tempus_ms > 0)
        {
            tls_opt.timeout_ms = (i32)petitio->tempus_ms;
        }
        tls_res = tls_connectere_cum_optionibus(
            (constans character*)petitio->hospes.datum,
            petitio->portus, &tls_opt, piscina);
        si (!tls_res.successus)
        {
            res.successus         = FALSUM;
            res.responsum         = NIHIL;
            res.error             = HTTP_ERROR_CONNEXIO;
            res.error_descriptio  = tls_res.error_descriptio;
            redde res;
        }
        tls_conn = tls_res.connexio;
    }
```

  If `_creare_error` is defined after the end-of-file position (it is
  `interior` and used earlier, so it is defined above), no prototype is
  needed; if the compiler says otherwise, move the new block below it.

- [ ] **Step 2.5: run the suite, expect green**

Run: `./compile_tests.sh probatio_http`
Expected: PASS, `probatio_http` with the new assertions counted.

- [ ] **Step 2.6: plants (each must turn the suite red, then restore)**

  1. In `http_vectura_ordinaria`, set `v.exsequi = NIHIL;` ->
     `probatio_vectura` red (successus FALSUM on the ordinaria call).
  2. In `http_petitio_visus`, drop `v.portus = petitio->portus;` ->
     `probatio_petitio_visus` red on 8443.
  3. Revert the https block to `tls_connectere(...)` -> the suite stays
     GREEN (no offline proof possible: TLS needs a real host) - this is
     why Step 2.7 exists. Record that in the worklog honestly.

- [ ] **Step 2.7: live proof of the timeout fix**

Run: `./tools/rete_fumus.sh -mora`
Expected: both lines BENE (`mora longa` succeeds after ~5 s; `mora
brevis` fails with "Timeout" after ~3 s). With plant 3 in place `mora
brevis` is FRACT again - run it once with the plant to see red, then
restore.

- [ ] **Step 2.8: worklog + commit**

Append to `lib/http.worklog.md`: the timeout gap (cite T1 output), the
fix, the seam and the view, the honest note that the timeout fix is
proven only live (Step 2.7). Owed gates: `http.h` is included widely.
Find the suites that compile it:

```bash
grep -rl --include='*.c' --include='*.h' '"http.h"' . | grep -v '^./build' | cut -d/ -f2 | sort -u
```

Name `radix`, `generata`, plus every subsystem suite from that list
that appears in `PORTAE` (`pythonica/silva.py:1515`). Freeze the tree
and commit with the shadow runner:

```python
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio_umbra(
    "vates-plan-1 T2: http - tempus vocantis in https, HttpVectura, http_petitio_visus ...",
    ['include/http.h', 'lib/http.c', 'probationes/probatio_http.c',
     'lib/http.worklog.md'],
    portae=['radix', 'generata'],   # + suites from the grep
    sine_debitis='arbor quarta: portae manu nominatae ex grep includentium http.h')
```

---

### Task 3: `herbarium.h` and `vates.h` drafted for Fran's review

**Files:**
- Create: `include/herbarium.h`, `include/vates.h` (no `.c`)
- Modify: `project-specs/vates-spec.md` §IV (fictus refinement, below)
- Modify: `oratio/glossarium.stml` (words the lint flags)

**Interfaces:**
- Consumes: T2's `HttpVectura`, `HttpPetitioVisus`, `HttpResponsum`;
  `json.h`, `xar.h`.
- Produces: the two headers as the CONTRACT plan 2 implements. Nothing
  includes them until plan 2.

Design refinement folded in here (recorded in the spec in Step 3.3):
**fictus is not a separate backend.** `vates_fictus_aperire` is the
Anthropic backend over a scripted `HttpVectura`. Consumers queue
neutral replies with helpers that render real Anthropic JSON; so
fictus exercises the real serializer and parser, and a test can read
back the exact JSON bytes vates sent. One code path instead of two.

- [ ] **Step 3.1: write `include/herbarium.h`**

```c
/* herbarium.h - specimina pressa responsorum API: captura et redditio
 *
 * project-specs/herbarium-spec.md. Responsum inexspectatum (status >=
 * limen, aut a consumptore pressum: NOVITAS) prima vice qua GENUS eius
 * videtur premitur; visiones posteriores numerantur (index.jsonl),
 * non iterum servantur. Redditio: vectura quae specimina servata
 * reddit, ut probationes contra responsa VERA currant sine rete.
 *
 * STATUS: PROPOSITUM (vates-plan-1 T3) - caput a Frano probandum ante
 * implementationem (vates-plan-2).
 *
 * USUS (captura):
 *   HerbariumOptiones o = herbarium_optiones_ordinariae();
 *   o.directorium = "/via/ad/captura";
 *   h = herbarium_aperire(piscina, &o);
 *   vectura = herbarium_vectura(h, http_vectura_ordinaria());
 *   res = http_vectura_exsequi(vectura, petitio, piscina);
 *
 * USUS (redditio in probatione):
 *   Xar* s = herbarium_enumerare(piscina, "probationes/fixa/vates/herbarium");
 *   vectura = herbarium_reddens(piscina, s);
 */

#ifndef HERBARIUM_H
#define HERBARIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "http.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Functio clavis GENERIS: quid 'unicum' significet. Duo responsa eadem
 * clave = idem genus (visio numeratur, non servatur). */
nomen chorda (*HerbariumClavis)(
    HttpPetitio*   petitio,
    HttpResponsum* responsum,
    Piscina*       piscina,
    vacuum*        datum);

nomen structura {
    constans character*           directorium;      /* NIHIL = captura exstincta */
                   i32            status_minimus;   /* 0 = CD */
                   i32            variantes_maximae; /* 0 = III */
        HerbariumClavis           clavis;           /* NIHIL = herbarium_clavis_sceleti */
                vacuum*           clavis_datum;
    constans character* constans* capita_admissa;   /* NIHIL-terminatum; ADDUNTUR ordinariis */
    constans character* constans* campi_petitionis; /* campi JSON corporis petitionis in summario (e.g. "model"); NIHIL-terminatum */
} HerbariumOptiones;

nomen structura Herbarium Herbarium;   /* opacum */

/* Specimen unum (ex disco lectum). */
nomen structura {
        chorda  clavis;          /* clavis generis */
        chorda  sigillum;        /* hex SHA-256 clavis (nomen plagulae) */
           i32  variantes_index; /* I.. */
        chorda  causa;           /* "status" aut causa consumptoris */
           i32  status;
     HttpCaput* capita;          /* admissa sola */
           i32  capita_numerus;
        chorda  corpus;          /* verbatim */
        chorda  primum_visum;    /* ISO 8601 */
        chorda  methodus;
        chorda  hospes;
        chorda  via;
        chorda  summarium;       /* objectum JSON: campi_petitionis */
} HerbariumSpecimen;


/* ========================================================================
 * CAPTURA
 * ======================================================================== */

HerbariumOptiones
herbarium_optiones_ordinariae (vacuum);

/* Aperire. directorium creatur si abest. NIHIL si creari non potest
 * (nuntius in stderr) - vocans sine captura pergit. */
Herbarium*
herbarium_aperire (
                     Piscina* piscina,
    constans HerbariumOptiones* optiones);

/* Vectura capiens: interiorem exsequitur, responsum TRANSMITTIT
 * immutatum; si praedicatum congruit, premit. Defectus capturae
 * (discus plenus) semel in stderr nuntiatur, numquam petitionem
 * frangit. */
HttpVectura
herbarium_vectura (
     Herbarium* herbarium,
    HttpVectura interior);

/* Premere explicite (NOVITAS): causa nominat quid inexspectatum fuit,
 * e.g. "blocus ignotus: server_tool_use". Praedicatum status NON
 * consulitur. */
vacuum
herbarium_premere (
     Herbarium* herbarium,
    HttpPetitio* petitio,
    HttpResponsum* responsum,
         chorda  causa);

/* Clavis ordinaria: status + sceleton JSON corporis (claves et genera,
 * recursive, valores omissi; corpus non-JSON -> status + content-type
 * + classis longitudinis). Publica ut consumptores eam componant. */
chorda
herbarium_clavis_sceleti (
     HttpPetitio* petitio,
    HttpResponsum* responsum,
        Piscina* piscina,
         vacuum* datum);


/* ========================================================================
 * REDDITIO
 * ======================================================================== */

/* Specimina directorii (HerbariumSpecimen*), ordine clavis deinde
 * variantis. Directorium absens -> Xar vacuum. */
Xar*
herbarium_enumerare (
               Piscina* piscina,
    constans character* directorium);

/* Vectura reddens: specimina ordine reddit (status, capita, corpus
 * verbatim); post finem HTTP_ERROR_CONNEXIO "herbarium exhaustum". */
HttpVectura
herbarium_reddens (
    Piscina* piscina,
        Xar* specimina);

#endif /* HERBARIUM_H */
```

- [ ] **Step 3.2: write `include/vates.h`**

```c
/* vates.h - provisor neuter exemplarium linguae (LLM)
 *
 * project-specs/vates-spec.md. Petitio neutra -> provisor (Anthropic
 * primus) -> responsum neutrum; usus et pretium in loco UNO pro omni
 * vocante. Principia: (1) nihil abicitur - blocus ignotus OPACUM fit
 * et verbatim redditur; (2) octeti petitionis deterministici (cache =
 * praefixum); (3) clavis A VOCANTE datur, numquam ex ambitu legitur.
 *
 * STATUS: PROPOSITUM (vates-plan-1 T3) - caput a Frano probandum ante
 * implementationem (vates-plan-2).
 *
 * USUS:
 *   VatesOptiones o = vates_optiones_ordinariae();
 *   Vates* v = vates_anthropic_aperire(piscina, clavis, &o);
 *   VatesPetitio* p = vates_petitio_creare(piscina, exemplar);
 *   vates_systema_addere(p, systema, VERUM);
 *   n = vates_nuntium_addere(p, VATES_USOR);
 *   vates_textum_addere(n, textus);
 *   r = vates_mittere(v, p, chorda_ex_literis("magister", piscina), piscina);
 *   si (r->successus && r->causa_finis == VATES_FINIS_INSTRUMENTUM) ...
 *   vates_responsum_addere(p, r);    -- bloci responsi in petitionem
 */

#ifndef VATES_H
#define VATES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "http.h"


/* ========================================================================
 * BLOCI ET NUNTII
 * ======================================================================== */

nomen enumeratio {
    VATES_USOR = 0,
    VATES_ASSISTENS
} VatesMunus;

nomen enumeratio {
    VATES_TEXTUS = 0,
    VATES_INSTRUMENTUM_PETITUM,   /* tool_use */
    VATES_INSTRUMENTI_EFFECTUS,   /* tool_result */
    VATES_OPACUM                  /* cogitatio, signatura, ignota: verbatim */
} VatesBlociGenus;

nomen structura {
    VatesBlociGenus  genus;
             chorda  textus;     /* TEXTUS; INSTRUMENTI_EFFECTUS: contentum */
             chorda  id;         /* PETITUM: id; EFFECTUS: id petiti */
             chorda  titulus;    /* PETITUM: nomen instrumenti */
         JsonValor*  input;      /* PETITUM */
                b32  erratum;    /* EFFECTUS: is_error */
         JsonValor*  crudum;     /* OPACUM: blocus provisoris verbatim */
                b32  signum_thesauri; /* punctum cache post hunc blocum */
} VatesBlocus;

nomen structura {
    VatesMunus  munus;
          Xar*  bloci;          /* VatesBlocus* */
} VatesNuntius;

nomen structura {
        chorda  titulus;
        chorda  descriptio;
    JsonValor*  schema;          /* JSON Schema input */
} VatesInstrumentum;


/* ========================================================================
 * PETITIO
 * ======================================================================== */

nomen structura VatesPetitio VatesPetitio;   /* opaca */

VatesPetitio*
vates_petitio_creare (
    Piscina* piscina,
      chorda exemplar);

/* max_tokens (0 = XVI M). */
vacuum
vates_petitio_maxima_ponere (
    VatesPetitio* petitio,
            s32  signa_maxima);

/* effort, verbis provisoris ("low".."max"); vacua = ordinarium. */
vacuum
vates_petitio_conatum_ponere (
    VatesPetitio* petitio,
          chorda  conatus);

/* Summarium cogitationis reddi (display "summarized"); FALSUM =
 * ordinarium provisoris. Cogitatio OPACA manet utroque modo. */
vacuum
vates_petitio_cogitationem_monstrare (
    VatesPetitio* petitio,
             b32  monstrare);

/* Punctum cache automaticum in cauda petitionis (cache_control summum). */
vacuum
vates_petitio_caudam_signare (
    VatesPetitio* petitio,
             b32  signare);

vacuum
vates_systema_addere (
    VatesPetitio* petitio,
          chorda  textus,
             b32  signum_thesauri);

vacuum
vates_instrumentum_addere (
                  VatesPetitio* petitio,
    constans VatesInstrumentum* instrumentum);

/* Nuntius novus in fine; bloci per functiones sequentes. */
VatesNuntius*
vates_nuntium_addere (
    VatesPetitio* petitio,
       VatesMunus munus);

/* Bloci: reddunt blocum ut vocans signum_thesauri ponere possit. */
VatesBlocus*
vates_textum_addere (
    VatesNuntius* nuntius,
           chorda textus);

VatesBlocus*
vates_effectum_addere (
    VatesNuntius* nuntius,
           chorda id,
           chorda contentum,
              b32 erratum);

/* Effugia pro provisoribus: campi corporis (ultimi immisti) et capita
 * (e.g. anthropic-beta). Ita res nondum modellatae (fallbacks, betae)
 * sine mutatione vatis adhibentur. */
vacuum
vates_extra_ponere (
           VatesPetitio* petitio,
    constans character* clavis,
             JsonValor* valor);

vacuum
vates_caput_addere (
           VatesPetitio* petitio,
    constans character* titulus,
    constans character* valor);


/* ========================================================================
 * RESPONSUM
 * ======================================================================== */

nomen enumeratio {
    VATES_FINIS = 0,                /* end_turn, stop_sequence */
    VATES_FINIS_INSTRUMENTUM,       /* tool_use */
    VATES_FINIS_MAXIMUM,            /* max_tokens */
    VATES_FINIS_RECUSATIO,          /* refusal */
    VATES_FINIS_PAUSA,              /* pause_turn */
    VATES_FINIS_ALIA                /* ignota: vide causa_finis_cruda */
} VatesCausaFinis;

nomen enumeratio {
    VATES_OK = 0,
    VATES_ERROR_RETE,               /* connexio, TLS */
    VATES_ERROR_TEMPUS,
    VATES_ERROR_STATUS,             /* HTTP non CC: genus + nuntius provisoris */
    VATES_ERROR_PARSE,
    VATES_ERROR_LIMES               /* recusatio localis ante missionem */
} VatesError;

nomen structura {
    s64  input;
    s64  cache_lectum;
    s64  cache_scriptum_5m;
    s64  cache_scriptum_1h;
    s64  output;
    s64  mora_ms;
    s64  pretium;          /* micro-dollaria; -I = pretium ignotum */
} VatesUsus;

nomen structura {
               b32  successus;
        VatesError  error;
               i32  status_http;
            chorda  error_genus;          /* e.g. "overloaded_error" */
            chorda  error_nuntius;
            chorda  id;
            chorda  exemplar;
              Xar*  bloci;                /* VatesBlocus* */
   VatesCausaFinis  causa_finis;
            chorda  causa_finis_cruda;
            chorda  recusatio_categoria;  /* stop_details.category */
         VatesUsus  usus;                 /* summa conatuum omnium */
               i32  conatus;              /* conatus facti */
} VatesResponsum;

/* Bloci responsi ut nuntius ASSISTENS in fine petitionis (OPACA
 * verbatim) - vicis proximae praeparatio. */
vacuum
vates_responsum_addere (
             VatesPetitio* petitio,
    constans VatesResponsum* responsum);


/* ========================================================================
 * PROVISOR
 * ======================================================================== */

/* Pretia in micro-dollariis per MTok - a VOCANTE data, numquam in
 * codice (pretia mutantur). */
nomen structura {
    constans character* exemplar;
                   s64  input;
                   s64  output;
                   s64  cache_lectum;
                   s64  cache_5m;
                   s64  cache_1h;
} VatesPretium;

nomen structura {
            MoraAngusta  tempus_ms;              /* 0 = X minuta */
                    i32  conatus_maximi;         /* 0 = III */
            MoraAngusta  mora_iterandi_ms;       /* 0 = II s */
            MoraAngusta  mora_iterandi_maxima_ms; /* limes retry-after; 0 = LX s */
    constans VatesPretium* pretia;
                    i32  pretia_numerus;
    constans character*  rationarium_via;        /* NIHIL = sine rationario */
    constans character*  herbarium_via;          /* NIHIL = sine captura */
            HttpVectura  vectura;                /* exsequi NIHIL = ordinaria */
} VatesOptiones;

nomen structura Vates Vates;   /* opacum */

VatesOptiones
vates_optiones_ordinariae (vacuum);

/* clavis: octeti clavis API, a vocante lecti. Numquam scribitur in
 * rationarium, herbarium, nuntios errorum. */
Vates*
vates_anthropic_aperire (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones);

/* Vocatio una, obstruens, cum iterationibus. propositum = signum
 * rationarii ("magister", "compactor"), numquam provisori missum.
 * Numquam NIHIL: defectus in responsum->error. */
VatesResponsum*
vates_mittere (
            Vates* vates,
     VatesPetitio* petitio,
           chorda  propositum,
          Piscina* piscina);

constans character*
vates_error_descriptio (
    VatesError error);


/* ========================================================================
 * PURA (sine rete) - pro probationibus et instrumentis
 * ======================================================================== */

/* Corpus JSON Anthropic petitionis, octeti EXACTI (deterministici). */
chorda
vates_anthropic_corpus (
    constans VatesPetitio* petitio,
                 Piscina* piscina);

/* Responsum Anthropic (status + corpus) -> responsum neutrum. */
VatesResponsum*
vates_anthropic_legere (
         i32  status_http,
      chorda  corpus,
    Piscina*  piscina);


/* ========================================================================
 * FICTUS - provisor Anthropic super vecturam scriptam (probationes
 * consumptorum: sine rete, gratis). Responsa neutra in caudam ponuntur
 * et ut JSON Anthropic VERUM redduntur; petitiones missae servantur.
 * ======================================================================== */

Vates*
vates_fictus_aperire (
                   Piscina* piscina,
    constans VatesOptiones* optiones);

/* Responsum textus cum usu. */
vacuum
vates_fictus_textum (
           Vates* vates,
           chorda textus,
        VatesUsus usus);

/* Responsum petitionis instrumenti (stop_reason tool_use). */
vacuum
vates_fictus_instrumentum (
                  Vates* vates,
                  chorda id,
                  chorda titulus,
    constans character* input_json);

/* Responsum crudum: status + corpus JSON quodvis (errores, novitates). */
vacuum
vates_fictus_crudum (
                  Vates* vates,
                     i32 status_http,
    constans character* corpus_json);

i32
vates_fictus_petitiones_numerus (
    Vates* vates);

/* Corpus JSON exactum petitionis index-ae missae. */
chorda
vates_fictus_petitio (
    Vates* vates,
      i32  index);

#endif /* VATES_H */
```

- [ ] **Step 3.3: record the fictus refinement in the spec.** In
  `project-specs/vates-spec.md` §IV, replace the `fictus` bullet's
  first sentence with: "**fictus**: the Anthropic backend over a
  scripted `HttpVectura` (not a separate backend): consumers queue
  neutral replies that render as real Anthropic JSON, so the real
  serializer and parser run, and `vates_fictus_petitio` returns the
  exact JSON vates sent."

- [ ] **Step 3.4: compile-check both headers (no implementation)**

```bash
printf '#include "herbarium.h"\n#include "vates.h"\ns32 principale (vacuum) { redde 0; }\n' > /private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/capita_probare.c
source tools/vexilla.sh && clang "${VEXILLA_C89[@]}" -Iinclude -fsyntax-only /private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/capita_probare.c
./silva/examen.sh include/herbarium.h include/vates.h
VOCABULA_VIAE_ADDITAE=include/herbarium.h,include/vates.h ./oratio/vocabula.sh -nova
```

Expected: clang silent; examen accepts; the lint lists unknown words -
add each to `oratio/glossarium.stml` (proper nouns as `nomen-proprium`,
house coinages with a `nota` saying what they name), re-run until clean.
(If `VOCABULA_VIAE_ADDITAE` does not accept a comma list, run it once
per file.)

- [ ] **Step 3.5: STOP - Fran reviews both headers.** Present them in
  the conversation with the decisions a reviewer is most likely to
  change: (a) fictus as a scripted vectura, not a backend; (b)
  `VatesBlocus` as one struct with genus-dependent fields vs a union;
  (c) usage summed across retry attempts on the response, with each
  attempt also a ledger line; (d) pure functions
  `vates_anthropic_corpus` / `_legere` public; (e) herbarium's
  NIHIL-terminated string arrays for allowlist and request fields.
  Apply his changes, re-run Step 3.4.

- [ ] **Step 3.6: commit the approved headers**

```python
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio(
    "vates-plan-1 T3: include/herbarium.h + include/vates.h - capita a Frano probata ...",
    ['include/herbarium.h', 'include/vates.h', 'project-specs/vates-spec.md',
     'oratio/glossarium.stml'],
    portae=['oratio'],
    sine_debitis='arbor quarta: capita sine implementatione, a nullo inclusa; oratio pro glossario')
```

- [ ] **Step 3.7: write `project-specs/vates-plan-2.md`** against the
  approved headers (herbarium implementation; vates neutral model,
  Anthropic body + parse with golden JSON; mittere + retries; fictus;
  accounting + ledger; specimen sweep; live smoke + cache proof;
  SANATIO, gates, merge to main by the tertia procedure).
