# norma plan 2 - fictio, norma, norma_gignere implemented; vates' first use

> **For agentic workers:** execution mode FIXED by Fran's standing rule:
> inline, one task per turn, Fran approves each; no subagents.

**Goal:** implement the three approved headers (7c635bb5) - `fictio`
(fake values), `norma` (builders, view, judge, JSON Schema export),
`norma_gignere` (TYPICA / FINES / INVALIDA) - prove them with the mutual
oracle, and make vates' novelty detection a declared NOTANDUM schema.

**Architecture:** three libraries in `lib/` (aedilis `include/X.h ->
lib/X.c`), each with its own credo suite. The judge and the generator
are written independently; the oracle (N4) runs them against each other
over hundreds of seeds. N5 touches only `lib/vates.c` internals.

**Spec:** `project-specs/norma-spec.md` (amended in N0: `NORMA_CAUSA_`
prefix, `liberum`). Headers are the contract.

## Global constraints

C89 + latina.h (`tools/vexilla.sh`); Latin identifiers, NO latina.h
macro word, no `_Capital`; compile-check in a TU (examen misses macro
and enum clashes - twice already); `chorda` not NUL-terminated; `i32`/
`i64` UNSIGNED, `s32`/`s64` signed; worktree commit guard (absolute
path + assert); gates by hand + `sine_debitis`; new `lib/*.c` ->
`./tools/compile_tests_fontes_generare.sh` in the same commit; every
behaviour born red by a plant that compiles.

## Rules decided in this plan (spec freedom; recorded in worklogs)

- **Path of an issue** - `DEEST`: the missing field's path
  (`$.obj.campus`); `EXTRA`: the extra key's path; `DISCRIMEN` and
  `VARIATIO`: the TAG's path (`$.content[0].type`); `GENUS`, bounds,
  `LONGITUDO`, `ELECTIO`, `FORMA`: the value's own path; `LIMES`,
  `SCHEMA_PRAVA`: `$`.
- **JSON integers vs floats**: `INTEGER` accepts only JSON integers
  (`1.0` parsed as float is `GENUS`); `NUMERUS` accepts both.
- **Per-path seed key** = the first 8 bytes of `sigillum` (SHA-256) of
  the path text, as `i64` - the AUDIENDA item; fixes every generated value.
- **Generator defaults**: integer/number bounds 0..1000; text 3..12 runes;
  arrays 1..3 (TYPICA), 0 or 3 (FINES) when unbounded; date-time within
  1970-01-01..2100-01-01.
- **INVALIDA mutations** (exactly one issue each): wrong type (text ->
  integer 7, anything else -> text "x"); remove a required field; add
  key `clavis_extranea` to a CLAUSUM object; bounds -> min-1 / max+1
  (floats -> min-1.0 / max+1.0); length -> min-1 / max+1 ONLY for text
  without electio/forma, and arrays (grown by a fresh valid element);
  electio -> `"extra_electionem"`; known forma -> `"non forma"`; tag ->
  `"variatio_ignota"` (CLAUSUM) or removed.

## Review focus

1. **INVALIDA cascading into two issues** (e.g. lengthening a dated text
   breaks its format too). Mutations are restricted (above); the oracle
   asserts EXACTLY one issue over hundreds of seeds and schemas.
2. **A hostile value**: 1000 wrong-typed array elements -> 256 issues +
   one `NORMA_CAUSA_LIMES`, not 1000 (N2 test).
3. **Keys needing brackets**: `a.b`, a quote, a non-ASCII key -> paths
   `$["a.b"]`, `$["q\""]`, `$["clāvis"]` (N2 test).
4. **Float boundaries** in FINES: `intra_fluitans(0.1, 0.3)` - the emitted
   maximum must survive `json_scribere` and be accepted (N4 oracle).
5. **NOTANDUM unknown variant**: value VALID, one note, and its known
   siblings still judged (N2 test).

---

### Task N1: fictio

**Files:** Create `lib/fictio.c`, `probationes/probatio_fictio.c`,
`lib/fictio.worklog.md`; regenerate the source list.

**Interfaces:** Produces `include/fictio.h` (approved). Consumes
`sors_proximum`, `sors_intra`, `sors_f64`, `sors_casu`,
`utf8_decodere`, `utf8_longitudo_byte`, `utf8_numerare_runas`,
`chorda_aedificator_*`.

- [ ] **N1.1 failing tests** - `probationes/probatio_fictio.c`:

```c
/* probatio_fictio.c - fictio: determinismus, fines, formae, corpus,
 * scrinium difficilium (norma-plan-2 N1) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "sors.h"
#include "utf8.h"
#include "fasti.h"
#include "fictio.h"

#include <stdio.h>
#include <string.h>

interior b32
_utf8_validum (chorda c)
{
    constans i8* p = c.datum;
    constans i8* finis = c.datum + c.mensura;

    dum (p < finis)
    {
        si (utf8_decodere(&p, finis) < 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
probatio_determinismus(Piscina* piscina)
{
    Sors a;
    Sors b;

    imprimere("\n--- Probans determinismus ---\n");
    sors_seminare(&a, XLII, 0);
    sors_seminare(&b, XLII, 0);
    CREDO_CHORDA_AEQUALIS(fictio_nomen(&a, piscina), fictio_nomen(&b, piscina));
    CREDO_CHORDA_AEQUALIS(fictio_email(&a, piscina), fictio_email(&b, piscina));
    CREDO_CHORDA_AEQUALIS(fictio_uuid(&a, piscina), fictio_uuid(&b, piscina));
    CREDO_CHORDA_AEQUALIS(fictio_textus_latinus(&a, III, piscina),
                          fictio_textus_latinus(&b, III, piscina));
    CREDO_CHORDA_AEQUALIS(fictio_textus_difficilis(&a, piscina),
                          fictio_textus_difficilis(&b, piscina));
}

interior vacuum
probatio_numeri(Piscina* piscina)
{
    Sors s;
    i32  i;
    b32  imum = FALSUM;
    b32  summum = FALSUM;

    (vacuum)piscina;
    imprimere("\n--- Probans fictio_integer / fictio_numerus ---\n");
    sors_seminare(&s, VII, 0);
    per (i = 0; i < M; i++)
    {
        s64 n = fictio_integer(&s, -V, V);
        f64 f = fictio_numerus(&s, 0.5, 2.5);

        CREDO_VERUM(n >= -V && n <= V);
        CREDO_VERUM(f >= 0.5 && f < 2.5);
        si (n == -V) imum = VERUM;
        si (n == V)  summum = VERUM;
    }
    CREDO_VERUM(imum && summum);
    /* minimum > maximum -> minimum; spatium totum s64 sine ruina */
    CREDO_AEQUALIS_S64(fictio_integer(&s, IX, III), IX);
    CREDO_NON_RUIT((vacuum)fictio_integer(&s, (s64)((i64)I << LXIII),
                                             (s64)(((i64)I << LXIII) - I)));
}

interior vacuum
probatio_formae(Piscina* piscina)
{
    Sors     s;
    i32      i;
    DiesHora dh;

    imprimere("\n--- Probans email, uuid, tempus, textus ---\n");
    sors_seminare(&s, XIII, 0);
    per (i = 0; i < C; i++)
    {
        chorda e = fictio_email(&s, piscina);
        chorda u = fictio_uuid(&s, piscina);
        chorda t = fictio_tempus(&s, 0, (s64)4102444800, piscina);
        chorda x = fictio_textus(&s, II, V,
            "\xce\xb1\xce\xb2\xce\xb3", piscina);   /* alpha beta gamma */
        s32    runae = utf8_numerare_runas(x.datum, (s32)x.mensura);
        i32    k;
        i32    ad = 0;

        per (k = 0; k < e.mensura; k++)
        {
            si (e.datum[k] == '@') ad++;
        }
        CREDO_AEQUALIS_I32(ad, I);
        CREDO_VERUM(chorda_terminatur(e, chorda_ex_literis("@example.org", piscina))
                 || chorda_terminatur(e, chorda_ex_literis("@example.com", piscina))
                 || chorda_terminatur(e, chorda_ex_literis("@example.net", piscina)));
        CREDO_AEQUALIS_I32(u.mensura, XXXVI);
        CREDO_VERUM(u.datum[VIII] == '-' && u.datum[XIII] == '-'
                 && u.datum[XVIII] == '-' && u.datum[XXIII] == '-');
        CREDO_VERUM(u.datum[XIV] == '4');
        CREDO_VERUM(strchr("89ab", u.datum[XIX]) != NIHIL);
        CREDO_VERUM(fasti_ex_iso(t, &dh));
        CREDO_VERUM(runae >= II && runae <= V);
    }
    /* epocha fixa */
    CREDO_CHORDA_AEQUALIS_LITERIS(fictio_tempus(&s, 0, 0, piscina),
                                  "1970-01-01T00:00:00Z");
    CREDO_CHORDA_AEQUALIS_LITERIS(fictio_tempus(&s, 951782400, 951782400, piscina),
                                  "2000-02-29T00:00:00Z");
}

interior vacuum
probatio_latinus_et_difficilia(Piscina* piscina)
{
    Sors   s;
    chorda l;
    i32    i;
    i32    fines = 0;

    imprimere("\n--- Probans corpus Latinum et scrinium difficilium ---\n");
    sors_seminare(&s, III, 0);
    l = fictio_textus_latinus(&s, III, piscina);
    per (i = 0; i < l.mensura; i++)
    {
        si (l.datum[i] == '.' || l.datum[i] == '?' || l.datum[i] == '!')
        {
            fines++;
        }
    }
    CREDO_MAIOR_AUT_AEQUALIS_I32(fines, III);
    CREDO_MAIOR_AUT_AEQUALIS_I32(fictio_difficilia_numerus(), IX);
    per (i = 0; i < fictio_difficilia_numerus(); i++)
    {
        CREDO_VERUM(_utf8_validum(fictio_difficile(i, piscina)));
    }
    /* scrinium continet U+0000 et chordam vacuam */
    {
        b32 nul = FALSUM;
        b32 vacua = FALSUM;

        per (i = 0; i < fictio_difficilia_numerus(); i++)
        {
            chorda d = fictio_difficile(i, piscina);

            si (d.mensura == 0) vacua = VERUM;
            si (d.mensura > 0 && memchr(d.datum, 0, (size_t)d.mensura)) nul = VERUM;
        }
        CREDO_VERUM(nul && vacua);
    }
}

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio_fictio", M * M);
    b32      successus;

    credo_aperire(piscina);
    probatio_determinismus(piscina);
    probatio_numeri(piscina);
    probatio_formae(piscina);
    probatio_latinus_et_difficilia(piscina);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
```

- [ ] **N1.2** run -> link failure (no `lib/fictio.c`).

- [ ] **N1.3 implement `lib/fictio.c`:**

```c
/* fictio.c - valores ficti sine JSON (norma-spec par. IV; norma-plan-2 N1)
 *
 * Corpus: XLVIII sententiae VERBATIM ex M. Tullii Ciceronis In Catilinam
 * (Project Gutenberg EBook #226, 'Cicero's Orations', Language: Latin;
 * textus in dominio publico), lectae ex ../gutenberg-mirror/2/2/226/226.txt
 * 2026-10-08, ex orationibus ipsis (non ex argumento editoris). */
#include "fictio.h"
#include "chorda_aedificator.h"
#include "utf8.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character* constans _praenomina[] = {
    "Marcus", "Gaius", "Lucius", "Publius", "Quintus", "Titus",
    "Gnaeus", "Sextus", "Aulus", "Decimus", NIHIL
};
hic_manens constans character* constans _nomina[] = {
    "Tullius", "Iulius", "Cornelius", "Claudius", "Valerius", "Aemilius",
    "Fabius", "Antonius", "Licinius", "Porcius", NIHIL
};
hic_manens constans character* constans _ordines_feminarum[] = {
    "Maior", "Minor", "Tertia", NIHIL
};
hic_manens constans character* constans _dominia[] = {
    "example.org", "example.com", "example.net", NIHIL
};
hic_manens constans character* constans _corpus[] = {
    "Quo usque tandem abutere, Catilina, patientia nostra?",
    "Patere tua consilia non sentis, constrictam iam horum omnium scientia teneri coniurationem tuam non vides?",
    "Nos autem fortes viri satis facere rei publicae videmur, si istius furorem ac tela vitemus.",
    "Fuit, fuit ista quondam in hac re publica virtus, ut viri fortes acrioribus suppliciis civem perniciosum quam acerbissimum hostem coercerent.",
    "Vivis, et vivis non ad deponendam, sed ad confirmandam audaciam.",
    "Cupio, patres conscripti, me esse clementem, cupio in tantis rei publicae periculis me non dissolutum videri, sed iam me ipse inertiae nequitiaeque condemno.",
    "Verum ego hoc, quod iam pridem factum esse oportuit, certa de causa nondum adducor ut faciam.",
    "Multorum te etiam oculi et aures non sentientem, sicut adhuc fecerunt, speculabuntur atque custodient.",
    "Muta iam istam mentem, mihi crede, obliviscere caedis atque incendiorum.",
    "Teneris undique; luce sunt clariora nobis tua consilia omnia; quae iam mecum licet recognoscas.",
    "Nihil agis, nihil moliris, nihil cogitas, quod non ego non modo audiam, sed etiam videam planeque sentiam.",
    "Recognosce tandem mecum noctem illam superiorem; iam intelleges multo me vigilare acrius ad salutem quam te ad perniciem rei publicae.",
    "Video enim esse hic in senatu quosdam, qui tecum una fuerunt.",
    "Quae cum ita sint, Catilina, perge, quo coepisti, egredere aliquando ex urbe; patent portae; proficiscere.",
    "Nimium diu te imperatorem tua illa Manliana castra desiderant.",
    "Educ tecum etiam omnes tuos, si minus, quam plurimos; purga urbem.",
    "Magno me metu liberabis, dum modo inter me atque te murus intersit.",
    "Nobiscum versari iam diutius non potes; non feram, non patiar, non sinam.",
    "Non est saepius in uno homine summa salus periclitanda rei publicae.",
    "Exire ex urbe iubet consul hostem.",
    "Interrogas me, num in exilium; non iubeo, sed, si me consulis, suadeo.",
    "Quid est enim, Catilina, quod te iam in hac urbe delectare possit?",
    "Nunc vero quae tua est ista vita?",
    "Quis te ex hac tanta frequentia totque tuis amicis ac necessariis salutavit?",
    "Quam ob rem discede atque hunc mihi timorem eripe; si est verus, ne opprimar, sin falsus, ut tandem aliquando timere desinam.",
    "Quid exspectas auctoritatem loquentium, quorum voluntatem tacitorum perspicis?",
    "Habes, ubi ostentes tuam illam praeclaram patientiam famis, frigoris, inopiae rerum omnium, quibus te brevi tempore confectum esse senties.",
    "At persaepe etiam privati in hac re publica perniciosos cives morte multarunt.",
    "At numquam in hac urbe, qui a re publica defecerunt, civium iura tenuerunt.",
    "His ego sanctissimis rei publicae vocibus et eorum hominum, qui hoc idem sentiunt, mentibus pauca respondebo.",
    "Hoc autem uno interfecto intellego hanc rei publicae pestem paulisper reprimi, non in perpetuum comprimi posse.",
    "Abiit, excessit, evasit, erupit.",
    "Nulla iam pernicies a monstro illo atque prodigio moenibus ipsis intra moenia comparabitur.",
    "Atque hunc quidem unum huius belli domestici ducem sine controversia vicimus.",
    "Non enim iam inter latera nostra sica illa versabitur, non in campo, non in foro, non in curia, non denique intra domesticos parietes pertimescemus.",
    "Loco ille motus est, cum est ex urbe depulsus.",
    "Palam iam cum hoste nullo inpediente bellum iustum geremus.",
    "Utinam ille omnis secum suas copias eduxisset!",
    "Ne illi vehementer errant, si illam meam pristinam lenitatem perpetuam sperant futuram.",
    "Non est iam lenitati locus; severitatem res ipsa flagitat.",
    "Demonstrabo iter: Aurelia via profectus est; si accelerare volent, ad vesperam consequentur.",
    "O fortunatam rem publicam, si quidem hanc sentinam urbis eiecerit!",
    "Uno mehercule Catilina exhausto levata mihi et recreata res publica videtur.",
    "Quid enim mali aut sceleris fingi aut cogitari potest, quod non ille conceperit?",
    "Non enim iam sunt mediocres hominum lubidines, non humanae ac tolerandae audaciae; nihil cogitant nisi caedem, nisi incendia, nisi rapinas.",
    "Sic enim iam tecum loquar, non ut odio permotus esse videar, quo debeo, sed ut misericordia, quae tibi nulla debetur.",
    "Quae nota domesticae turpitudinis non inusta vitae tuae est?",
    "Sed quam longe videtur a carcere atque a vinculis abesse debere, qui se ipse iam dignum custodia iudicarit!",
    NIHIL
};

/* scrinium: (datum, mensura) - mensura explicita quia U+0000 */
nomen structura {
    constans character* datum;
                   i32  mensura;
} FictioDifficile;

hic_manens constans FictioDifficile _difficilia[] = {
    { "", 0 },                                                 /* vacua */
    { "e\xcc\x81t\xc3\xa9", 6 },                               /* nota combinans + praecomposita */
    { "\xf0\x9d\x94\x99 \xf0\x9f\x98\x80", 9 },                /* non-BMP */
    { "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d abc", 12 },            /* dextrorsum + sinistrorsum */
    { "\"virgulae\" \\obliqua\\", 21 },                        /* " et \ */
    { "a\0b", 3 },                                             /* U+0000 */
    { "  \t \n ", 6 },                                         /* spatia sola */
    { "\xf0\x9f\x91\x8d\xf0\x9f\x8f\xbd", 8 },                 /* emoji + modificator */
    { "\xef\xbb\xbf" "BOM", 6 },                               /* U+FEFF */
    { NIHIL, 0 }                                               /* longus: computatus */
};
#define FICTIO_DIFFICILIA  X
#define FICTIO_LONGUS_RUNAE MM

/* ---- sors in LXIV bits ---- */

interior i64
_u64 (Sors* s)
{
    i64 alta  = (i64)sors_proximum(s);
    i64 bassa = (i64)sors_proximum(s);

    redde (alta << XXXII) | bassa;
}

s64
fictio_integer (Sors* sors, s64 minimum, s64 maximum)
{
    i64 spatium;
    i64 limen;
    i64 r;

    si (minimum >= maximum)
    {
        redde minimum;
    }
    spatium = (i64)maximum - (i64)minimum + I;   /* 0 = totum spatium */
    si (spatium == 0)
    {
        redde (s64)_u64(sors);
    }
    limen = (0 - spatium) % spatium;              /* Lemire/OpenBSD */
    fac
    {
        r = _u64(sors);
    } dum (r < limen);
    redde (s64)((i64)minimum + (r % spatium));
}

f64
fictio_numerus (Sors* sors, f64 minimum, f64 maximum)
{
    si (minimum >= maximum)
    {
        redde minimum;
    }
    redde minimum + (maximum - minimum) * sors_f64(sors);
}

constans character*
fictio_eligere (Sors* sors, constans character* constans* optiones)
{
    i32 numerus = 0;

    si (!optiones)
    {
        redde NIHIL;
    }
    dum (optiones[numerus])
    {
        numerus++;
    }
    si (numerus == 0)
    {
        redde NIHIL;
    }
    redde optiones[sors_intra(sors, numerus)];
}

/* ---- nomina et inscriptiones ---- */

chorda
fictio_nomen (Sors* sors, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, LXIV);
    constans character* gens = fictio_eligere(sors, _nomina);

    si (sors_casu(sors, II, III))
    {
        chorda_aedificator_appendere_literis(aed, fictio_eligere(sors, _praenomina));
        chorda_aedificator_appendere_character(aed, ' ');
        chorda_aedificator_appendere_literis(aed, gens);
    }
    alioquin
    {
        /* femina: nomen gentile in -a + ordo ("Tullia Minor") */
        i32 m = (i32)strlen(gens);
        i32 k;

        per (k = 0; k + II < m; k++)
        {
            chorda_aedificator_appendere_character(aed, gens[k]);
        }
        chorda_aedificator_appendere_literis(aed, "a ");
        chorda_aedificator_appendere_literis(aed,
            fictio_eligere(sors, _ordines_feminarum));
    }
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_email (Sors* sors, Piscina* piscina)
{
    chorda             nomen = fictio_nomen(sors, piscina);
    ChordaAedificator* aed   = chorda_aedificator_creare(piscina, LXIV);
    i32                i;

    per (i = 0; i < nomen.mensura; i++)
    {
        character c = (character)nomen.datum[i];

        si (c == ' ')
        {
            c = '.';
        }
        alioquin si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        chorda_aedificator_appendere_character(aed, c);
    }
    chorda_aedificator_appendere_character(aed, '@');
    chorda_aedificator_appendere_literis(aed, fictio_eligere(sors, _dominia));
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_uuid (Sors* sors, Piscina* piscina)
{
    i8        o[XVI];
    character buffer[XXXVII];
    i32       i;
    i32       k = 0;

    per (i = 0; i < XVI; i++)
    {
        o[i] = (i8)(sors_proximum(sors) & 0xFF);
    }
    o[VI] = (i8)((o[VI] & 0x0F) | 0x40);         /* versio IV */
    o[VIII] = (i8)((o[VIII] & 0x3F) | 0x80);     /* variatio RFC 4122 */
    per (i = 0; i < XVI; i++)
    {
        si (i == IV || i == VI || i == VIII || i == X)
        {
            buffer[k++] = '-';
        }
        sprintf(buffer + k, "%02x", (insignatus integer)o[i]);
        k += II;
    }
    buffer[k] = '\0';
    redde chorda_ex_literis(buffer, piscina);
}

/* dies civiles ex diebus epochae (Hinnant, days_from_civil inversum) */
interior vacuum
_dies_civiles (s64 z, s64* annus, s64* mensis, s64* dies)
{
    s64 era;
    s64 doe;
    s64 yoe;
    s64 doy;
    s64 mp;

    z  += 719468;
    era = (z >= 0 ? z : z - 146096) / 146097;
    doe = z - era * 146097;
    yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    mp  = (5 * doy + 2) / 153;
    *dies   = doy - (153 * mp + 2) / 5 + 1;
    *mensis = mp < 10 ? mp + 3 : mp - 9;
    *annus  = yoe + era * 400 + (*mensis <= 2 ? 1 : 0);
}

chorda
fictio_tempus (Sors* sors, s64 ab, s64 ad, Piscina* piscina)
{
    s64       t = fictio_integer(sors, ab, ad);
    s64       dies_ep = t >= 0 ? t / 86400 : (t - 86399) / 86400;
    s64       sec = t - dies_ep * 86400;
    s64       annus;
    s64       mensis;
    s64       dies;
    character buffer[LXIV];

    _dies_civiles(dies_ep, &annus, &mensis, &dies);
    sprintf(buffer, "%04ld-%02ld-%02ldT%02ld:%02ld:%02ldZ",
            (longus)annus, (longus)mensis, (longus)dies,
            (longus)(sec / 3600), (longus)((sec % 3600) / 60), (longus)(sec % 60));
    redde chorda_ex_literis(buffer, piscina);
}

chorda
fictio_textus_latinus (Sors* sors, i32 sententiae, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);
    i32                i;

    per (i = 0; i < (sententiae < I ? I : sententiae); i++)
    {
        si (i > 0)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        chorda_aedificator_appendere_literis(aed, fictio_eligere(sors, _corpus));
    }
    redde chorda_aedificator_finire(aed);
}

chorda
fictio_textus (
                 Sors* sors,
                  i32  minimum,
                  i32  maximum,
    constans character* alphabetum,
              Piscina* piscina)
{
    constans character* alph = alphabetum ? alphabetum
                                          : "abcdefghijklmnopqrstuvwxyz";
    i32                 runae_initia[CCLVI];
    i32                 runae_longitudo[CCLVI];
    i32                 numerus = 0;
    i32                 m = (i32)strlen(alph);
    i32                 i = 0;
    i32                 longitudo;
    ChordaAedificator*  aed;

    dum (i < m && numerus < CCLVI)
    {
        s32 l = utf8_longitudo_byte((i8)alph[i]);

        si (l < I)
        {
            l = I;
        }
        runae_initia[numerus]    = i;
        runae_longitudo[numerus] = (i32)l;
        numerus++;
        i += (i32)l;
    }
    longitudo = (i32)fictio_integer(sors, (s64)minimum, (s64)maximum);
    aed = chorda_aedificator_creare(piscina, (memoriae_index)(longitudo * IV + I));
    per (i = 0; numerus > 0 && i < longitudo; i++)
    {
        i32 r = sors_intra(sors, numerus);
        i32 k;

        per (k = 0; k < runae_longitudo[r]; k++)
        {
            chorda_aedificator_appendere_character(aed, alph[runae_initia[r] + k]);
        }
    }
    redde chorda_aedificator_finire(aed);
}

i32
fictio_difficilia_numerus (vacuum)
{
    redde FICTIO_DIFFICILIA;
}

chorda
fictio_difficile (i32 index, Piscina* piscina)
{
    chorda c;

    si (index >= FICTIO_DIFFICILIA)
    {
        index = 0;
    }
    si (_difficilia[index].datum == NIHIL)
    {
        /* longus: sententia prima repetita usque ad MM runas */
        ChordaAedificator* aed = chorda_aedificator_creare(piscina, MM + C);
        constans character* fons = _corpus[0];
        i32 i;

        per (i = 0; i < FICTIO_LONGUS_RUNAE; i++)
        {
            chorda_aedificator_appendere_character(aed, fons[i % (i32)strlen(fons)]);
        }
        redde chorda_aedificator_finire(aed);
    }
    c.mensura = _difficilia[index].mensura;
    c.datum   = (i8*)piscina_allocare(piscina, (i64)c.mensura + I);
    si (c.mensura > 0)
    {
        memcpy(c.datum, _difficilia[index].datum, (size_t)c.mensura);
    }
    redde c;
}

chorda
fictio_textus_difficilis (Sors* sors, Piscina* piscina)
{
    redde fictio_difficile(sors_intra(sors, FICTIO_DIFFICILIA), piscina);
}
```

  Note: the `"@example.org"`
  lengths, the difficult strings' byte lengths and the date arithmetic
  are what the tests pin; adjust a literal's length if the compiler or a
  test disagrees, and ledger it.

- [ ] **N1.4** register (`compile_tests_fontes_generare.sh`), green;
  lint (praenomina etc. are in strings, not identifiers).
- [ ] **N1.5 plants** - (1) `limen` loop removed and `% spatium` on a
  32-bit draw -> the full-range or bound test red; (2) uuid version nibble
  not set -> red; (3) a difficult entry's mensura off by one (cuts a
  UTF-8 sequence) -> validity red; (4) `_dies_civiles` leap handling
  broken (`doe / 1460` dropped) -> 2000-02-29 red.
- [ ] **N1.6** worklog (corpus provenance, generator choices) + commit;
  gates `radix`, `generata`.

---

### Task N2: norma - builders, view, judge

**Files:** Create `lib/norma.c` (parts A builders + view, B judge),
`probationes/probatio_norma.c`, `lib/norma.worklog.md`; regenerate list.

**Interfaces:** Produces `include/norma.h` (approved) except
`norma_json_schema` (N3). Consumes `json_*`, `xar_*`,
`utf8_numerare_runas`, `fasti_ex_iso`, `chorda_aedificator_*`.

- [ ] **N2.1 failing tests** - `probationes/probatio_norma.c`:

```c
/* probatio_norma.c - norma: genera, fines, formae, objecta, viae,
 * discrimen, limes, pravitas, visus (norma-plan-2 N2), exportatio (N3) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "chorda_aedificator.h"
#include "norma.h"

#include <stdio.h>
#include <string.h>

interior NormaIudicium
_iud (Norma* n, constans character* json, Piscina* p)
{
    redde norma_iudicare(n, json_legere_literis(json, p).radix, p);
}

interior NormaVitium*
_vit (NormaIudicium j, i32 i)
{
    redde (NormaVitium*)xar_obtinere(j.vitia, i);
}

interior NormaVitium*
_nota (NormaIudicium j, i32 i)
{
    redde (NormaVitium*)xar_obtinere(j.notae, i);
}

/* vitium unum exspectatum: causa + via */
interior vacuum
_unum (NormaIudicium j, NormaCausa causa, constans character* via)
{
    CREDO_FALSUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.vitia), I);
    si (xar_numerus(j.vitia) == I)
    {
        CREDO_VERUM(_vit(j, 0)->causa == causa);
        CREDO_CHORDA_AEQUALIS_LITERIS(_vit(j, 0)->via, via);
    }
}

interior vacuum
probatio_genera(Piscina* p)
{
    imprimere("\n--- Probans genera ---\n");
    CREDO_VERUM(_iud(norma_integer(p), "7", p).validum);
    _unum(_iud(norma_integer(p), "7.5", p), NORMA_CAUSA_GENUS, "$");
    _unum(_iud(norma_integer(p), "\"x\"", p), NORMA_CAUSA_GENUS, "$");
    _unum(_iud(norma_integer(p), "null", p), NORMA_CAUSA_GENUS, "$");
    CREDO_VERUM(_iud(norma_aut_nullum(norma_integer(p)), "null", p).validum);
    CREDO_VERUM(_iud(norma_numerus(p), "7", p).validum);
    CREDO_VERUM(_iud(norma_numerus(p), "7.5", p).validum);
    CREDO_VERUM(_iud(norma_boolean(p), "true", p).validum);
    CREDO_VERUM(_iud(norma_nullum(p), "null", p).validum);
    _unum(_iud(norma_nullum(p), "1", p), NORMA_CAUSA_GENUS, "$");
    CREDO_VERUM(_iud(norma_liberum(p), "null", p).validum);
    CREDO_VERUM(_iud(norma_liberum(p), "{\"a\":[1,2]}", p).validum);
    CREDO_VERUM(_iud(norma_textus(p), "\"x\"", p).validum);
    _unum(_iud(norma_textus(p), "1", p), NORMA_CAUSA_GENUS, "$");
}

interior vacuum
probatio_fines_et_formae(Piscina* p)
{
    constans character* constans licita[] = { "a", "b", NIHIL };

    imprimere("\n--- Probans fines, electio, formae ---\n");
    _unum(_iud(norma_intra(norma_integer(p), I, X), "0", p), NORMA_CAUSA_MINIMUM, "$");
    _unum(_iud(norma_intra(norma_integer(p), I, X), "11", p), NORMA_CAUSA_MAXIMUM, "$");
    CREDO_VERUM(_iud(norma_intra(norma_integer(p), I, X), "10", p).validum);
    _unum(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), "0.05", p),
          NORMA_CAUSA_MINIMUM, "$");
    _unum(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), "0.31", p),
          NORMA_CAUSA_MAXIMUM, "$");
    CREDO_VERUM(_iud(norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), "0.3", p).validum);
    /* longitudo in RUNIS: "āē" = II runae, IV octeti */
    CREDO_VERUM(_iud(norma_longitudo(norma_textus(p), II, III),
                     "\"\xc4\x81\xc4\x93\"", p).validum);
    _unum(_iud(norma_longitudo(norma_textus(p), II, III), "\"a\"", p),
          NORMA_CAUSA_LONGITUDO, "$");
    _unum(_iud(norma_longitudo(norma_tabulatum(p, norma_integer(p)), I, II), "[]", p),
          NORMA_CAUSA_LONGITUDO, "$");
    _unum(_iud(norma_electio(norma_textus(p), licita), "\"c\"", p),
          NORMA_CAUSA_ELECTIO, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "date-time"),
                     "\"2026-10-08T10:27:15Z\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "date-time"), "\"heri\"", p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "uuid"),
                     "\"123e4567-e89b-42d3-a456-426614174000\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "uuid"), "\"123e4567\"", p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "email"), "\"a@b.c\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "email"), "\"a@b\"", p),
          NORMA_CAUSA_FORMA, "$");
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "uri"), "\"https://x\"", p).validum);
    _unum(_iud(norma_forma(norma_textus(p), "uri"), "\"sine\"", p),
          NORMA_CAUSA_FORMA, "$");
    /* forma ignota: indicium solum, iudex tacet */
    CREDO_VERUM(_iud(norma_forma(norma_textus(p), "color"), "\"quidvis\"", p).validum);
}

interior vacuum
probatio_objecta_et_viae(Piscina* p)
{
    Norma*        o = norma_objectum(p);
    Norma*        apertum;
    Norma*        notandum;
    Norma*        vacuum_o;
    NormaIudicium j;

    imprimere("\n--- Probans objecta, modi, viae ---\n");
    norma_campus(o, "id", norma_integer(p), VERUM);
    norma_campus(o, "n", norma_integer(p), FALSUM);
    CREDO_VERUM(_iud(o, "{\"id\":1}", p).validum);
    _unum(_iud(o, "{}", p), NORMA_CAUSA_DEEST, "$.id");
    _unum(_iud(o, "{\"id\":1,\"x\":2}", p), NORMA_CAUSA_EXTRA, "$.x");
    apertum = norma_modus(norma_campus(norma_objectum(p), "id", norma_integer(p), VERUM),
                          NORMA_APERTUM);
    j = _iud(apertum, "{\"id\":1,\"x\":2}", p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), 0);
    notandum = norma_modus(norma_campus(norma_objectum(p), "id", norma_integer(p), VERUM),
                           NORMA_NOTANDUM);
    j = _iud(notandum, "{\"id\":1,\"x\":2}", p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_nota(j, 0)->via, "$.x");
    CREDO_VERUM(_nota(j, 0)->causa == NORMA_CAUSA_EXTRA);
    /* viae cum claves rarae */
    vacuum_o = norma_objectum(p);
    _unum(_iud(vacuum_o, "{\"a.b\":1}", p), NORMA_CAUSA_EXTRA, "$[\"a.b\"]");
    _unum(_iud(vacuum_o, "{\"q\\\"\":1}", p), NORMA_CAUSA_EXTRA, "$[\"q\\\"\"]");
    _unum(_iud(vacuum_o, "{\"cl\xc4\x81vis\":1}", p), NORMA_CAUSA_EXTRA,
          "$[\"cl\xc4\x81vis\"]");
    _unum(_iud(norma_tabulatum(p, o), "[{\"id\":\"x\"}]", p), NORMA_CAUSA_GENUS, "$[0].id");
}

interior vacuum
probatio_discrimen(Piscina* p)
{
    Norma*        textus = norma_objectum(p);
    Norma*        petitum = norma_objectum(p);
    Norma*        d = norma_discrimen(p, "type");
    Norma*        dn;
    NormaIudicium j;

    imprimere("\n--- Probans discrimen ---\n");
    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_variatio(d, "text", textus);
    norma_variatio(d, "tool_use", petitum);
    CREDO_VERUM(_iud(d, "{\"type\":\"text\",\"text\":\"t\"}", p).validum);
    _unum(_iud(d, "{\"type\":\"tool_use\"}", p), NORMA_CAUSA_DEEST, "$.id");
    _unum(_iud(d, "{\"type\":\"novum\"}", p), NORMA_CAUSA_VARIATIO, "$.type");
    _unum(_iud(d, "{\"text\":\"t\"}", p), NORMA_CAUSA_DISCRIMEN, "$.type");
    _unum(_iud(d, "{\"type\":3}", p), NORMA_CAUSA_DISCRIMEN, "$.type");
    /* NOTANDUM: variatio ignota = nota, valor validus, fratres iudicati */
    dn = norma_modus(norma_discrimen(p, "type"), NORMA_NOTANDUM);
    norma_variatio(dn, "text", textus);
    j = _iud(norma_tabulatum(p, dn),
             "[{\"type\":\"novum\"},{\"type\":\"text\",\"text\":\"t\"}]", p);
    CREDO_VERUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_nota(j, 0)->via, "$[0].type");
    j = _iud(norma_tabulatum(p, dn), "[{\"type\":\"novum\"},{\"type\":\"text\"}]", p);
    _unum(j, NORMA_CAUSA_DEEST, "$[1].text");
    CREDO_AEQUALIS_I32(xar_numerus(j.notae), I);
}

interior vacuum
probatio_limes(Piscina* p)
{
    ChordaAedificator* aed = chorda_aedificator_creare(p, M * VIII);
    NormaIudicium      j;
    i32                i;

    imprimere("\n--- Probans limes CCLVI ---\n");
    chorda_aedificator_appendere_character(aed, '[');
    per (i = 0; i < M; i++)
    {
        chorda_aedificator_appendere_literis(aed, i ? ",\"x\"" : "\"x\"");
    }
    chorda_aedificator_appendere_character(aed, ']');
    j = norma_iudicare(norma_tabulatum(p, norma_integer(p)),
                       json_legere(chorda_aedificator_finire(aed), p).radix, p);
    CREDO_FALSUM(j.validum);
    CREDO_AEQUALIS_I32(xar_numerus(j.vitia), CCLVII);
    CREDO_VERUM(_vit(j, CCLVI)->causa == NORMA_CAUSA_LIMES);
}

interior vacuum
probatio_pravitas_et_visus(Piscina* p)
{
    Norma*     o = norma_objectum(p);
    Norma*     n;
    NormaVisus v;

    imprimere("\n--- Probans pravitas et visus ---\n");
    norma_campus(o, "a", NIHIL, VERUM);
    _unum(_iud(o, "{}", p), NORMA_CAUSA_SCHEMA_PRAVA, "$");
    _unum(_iud(norma_intra(norma_integer(p), V, I), "3", p), NORMA_CAUSA_SCHEMA_PRAVA, "$");
    _unum(_iud(norma_campus(norma_integer(p), "a", norma_textus(p), VERUM), "1", p),
          NORMA_CAUSA_SCHEMA_PRAVA, "$");
    o = norma_objectum(p);
    norma_campus(o, "a", norma_textus(p), VERUM);
    norma_campus(o, "a", norma_textus(p), VERUM);
    _unum(_iud(o, "{\"a\":\"x\"}", p), NORMA_CAUSA_SCHEMA_PRAVA, "$");
    /* pravitas in nodo filio quoque invenitur */
    _unum(_iud(norma_tabulatum(p, norma_intra(norma_integer(p), V, I)), "[]", p),
          NORMA_CAUSA_SCHEMA_PRAVA, "$");

    n = norma_descriptio(norma_intra(norma_integer(p), II, IX), "numerus");
    v = norma_visus(n);
    CREDO_VERUM(v.genus == NORMA_INTEGER);
    CREDO_VERUM(v.habet_intra);
    CREDO_AEQUALIS_S64(v.minimum, II);
    CREDO_AEQUALIS_S64(v.maximum, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS(v.descriptio, "numerus");
    CREDO_CHORDA_VACUA(v.error_schematis);
    v = norma_visus(norma_intra(norma_integer(p), V, I));
    CREDO_CHORDA_NON_VACUA(v.error_schematis);
    o = norma_objectum(p);
    norma_campus(o, "a", norma_textus(p), VERUM);
    v = norma_visus(o);
    CREDO_AEQUALIS_I32(xar_numerus(v.campi), I);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma", M * M);
    b32      successus;

    credo_aperire(p);
    /* N2 */
    probatio_genera(p);
    probatio_fines_et_formae(p);
    probatio_objecta_et_viae(p);
    probatio_discrimen(p);
    probatio_limes(p);
    probatio_pravitas_et_visus(p);
    /* N3 addit hic vocationem suam */
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
```

- [ ] **N2.2** run -> link failure.

- [ ] **N2.3 implement `lib/norma.c`** (A + B):

```c
/* norma.c - schema valoris JSON: aedificatores, visus, iudicium,
 * exportatio (norma-spec; norma-plan-2 N2/N3). Regulae viarum et
 * mutationum: project-specs/norma-plan-2.md, lib/norma.worklog.md. */
#include "norma.h"
#include "fasti.h"
#include "utf8.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define NORMA_VITIA_MAXIMA CCLVI

structura Norma {
         Piscina* piscina;
       NormaGenus genus;
              b32 aut_nullum;
       NormaModus modus;
           Norma* elementum;
             Xar* campi;
           chorda clavis_discriminis;
             Xar* variationes;
              b32 habet_intra;
              s64 minimum;
              s64 maximum;
              b32 habet_intra_fluitans;
              f64 minimum_fluitans;
              f64 maximum_fluitans;
              b32 habet_longitudinem;
              i32 longitudo_minima;
              i32 longitudo_maxima;
             Xar* licita;
           chorda forma;
           chorda descriptio;
     NormaGignens gignens;
           vacuum* gignens_datum;
           chorda error_schematis;
};

/* ====================================================================
 * A. AEDIFICATORES ET VISUS
 * ==================================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum   = NIHIL;
    v.mensura = 0;
    redde v;
}

interior Norma*
_nodus (Piscina* piscina, NormaGenus genus)
{
    Norma* n;

    si (!piscina)
    {
        redde NIHIL;
    }
    n = (Norma*)piscina_allocare(piscina, (i64)magnitudo(Norma));
    memset(n, 0, magnitudo(*n));
    n->piscina = piscina;
    n->genus   = genus;
    redde n;
}

interior Norma*
_pravus (Norma* n, constans character* nuntius)
{
    si (n && n->error_schematis.mensura == 0)
    {
        n->error_schematis = chorda_ex_literis(nuntius, n->piscina);
    }
    redde n;
}

Norma* norma_liberum (Piscina* piscina) { redde _nodus(piscina, NORMA_LIBERUM); }
Norma* norma_nullum  (Piscina* piscina) { redde _nodus(piscina, NORMA_NULLUM); }
Norma* norma_boolean (Piscina* piscina) { redde _nodus(piscina, NORMA_BOOLEAN); }
Norma* norma_integer (Piscina* piscina) { redde _nodus(piscina, NORMA_INTEGER); }
Norma* norma_numerus (Piscina* piscina) { redde _nodus(piscina, NORMA_NUMERUS); }
Norma* norma_textus  (Piscina* piscina) { redde _nodus(piscina, NORMA_TEXTUS); }

Norma*
norma_tabulatum (Piscina* piscina, Norma* elementum)
{
    Norma* n = _nodus(piscina, NORMA_TABULATUM);

    si (!n)
    {
        redde NIHIL;
    }
    n->elementum = elementum;
    redde elementum ? n : _pravus(n, "tabulatum sine elemento");
}

Norma*
norma_objectum (Piscina* piscina)
{
    Norma* n = _nodus(piscina, NORMA_OBJECTUM);

    si (n)
    {
        n->campi = xar_creare(piscina, (i32)magnitudo(NormaCampus));
    }
    redde n;
}

Norma*
norma_campus (
                 Norma* objectum,
    constans character* titulus,
                 Norma* valor,
                    b32 requiritur)
{
    NormaCampus* c;
    i32          i;

    si (!objectum)
    {
        redde NIHIL;
    }
    si (objectum->genus != NORMA_OBJECTUM)
    {
        redde _pravus(objectum, "campus in nodo non objecto");
    }
    si (!titulus || !valor)
    {
        redde _pravus(objectum, "campus sine titulo aut valore");
    }
    per (i = 0; i < xar_numerus(objectum->campi); i++)
    {
        si (chorda_aequalis_literis(
                ((NormaCampus*)xar_obtinere(objectum->campi, i))->titulus, titulus))
        {
            redde _pravus(objectum, "campus duplicatus");
        }
    }
    c = (NormaCampus*)xar_addere(objectum->campi);
    c->titulus    = chorda_ex_literis(titulus, objectum->piscina);
    c->valor      = valor;
    c->requiritur = requiritur;
    redde objectum;
}

Norma*
norma_modus (Norma* n, NormaModus modus)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_OBJECTUM && n->genus != NORMA_DISCRIMEN)
    {
        redde _pravus(n, "modus solum in objecto aut discrimine");
    }
    n->modus = modus;
    redde n;
}

Norma*
norma_discrimen (Piscina* piscina, constans character* clavis)
{
    Norma* n = _nodus(piscina, NORMA_DISCRIMEN);

    si (!n)
    {
        redde NIHIL;
    }
    n->variationes = xar_creare(piscina, (i32)magnitudo(NormaVariatio));
    si (!clavis)
    {
        redde _pravus(n, "discrimen sine clave");
    }
    n->clavis_discriminis = chorda_ex_literis(clavis, piscina);
    redde n;
}

Norma*
norma_variatio (Norma* discrimen, constans character* valor, Norma* objectum)
{
    NormaVariatio* v;
    i32            i;

    si (!discrimen)
    {
        redde NIHIL;
    }
    si (discrimen->genus != NORMA_DISCRIMEN)
    {
        redde _pravus(discrimen, "variatio in nodo non discrimine");
    }
    si (!valor || !objectum || objectum->genus != NORMA_OBJECTUM)
    {
        redde _pravus(discrimen, "variatio sine valore aut objecto");
    }
    per (i = 0; i < xar_numerus(discrimen->variationes); i++)
    {
        si (chorda_aequalis_literis(
                ((NormaVariatio*)xar_obtinere(discrimen->variationes, i))->valor, valor))
        {
            redde _pravus(discrimen, "variatio duplicata");
        }
    }
    v = (NormaVariatio*)xar_addere(discrimen->variationes);
    v->valor    = chorda_ex_literis(valor, discrimen->piscina);
    v->objectum = objectum;
    redde discrimen;
}

Norma*
norma_aut_nullum (Norma* n)
{
    si (n)
    {
        n->aut_nullum = VERUM;
    }
    redde n;
}

Norma*
norma_intra (Norma* n, s64 minimum, s64 maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_INTEGER)
    {
        redde _pravus(n, "intra solum in integro (numerus: intra_fluitans)");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "intra: minimum > maximum");
    }
    n->habet_intra = VERUM;
    n->minimum     = minimum;
    n->maximum     = maximum;
    redde n;
}

Norma*
norma_intra_fluitans (Norma* n, f64 minimum, f64 maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_NUMERUS)
    {
        redde _pravus(n, "intra_fluitans solum in numero");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "intra_fluitans: minimum > maximum");
    }
    n->habet_intra_fluitans = VERUM;
    n->minimum_fluitans     = minimum;
    n->maximum_fluitans     = maximum;
    redde n;
}

Norma*
norma_longitudo (Norma* n, i32 minimum, i32 maximum)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS && n->genus != NORMA_TABULATUM)
    {
        redde _pravus(n, "longitudo solum in textu aut tabulato");
    }
    si (minimum > maximum)
    {
        redde _pravus(n, "longitudo: minimum > maximum");
    }
    n->habet_longitudinem = VERUM;
    n->longitudo_minima   = minimum;
    n->longitudo_maxima   = maximum;
    redde n;
}

Norma*
norma_electio (Norma* n, constans character* constans* licita)
{
    i32 i;

    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS || !licita)
    {
        redde _pravus(n, "electio solum in textu, licita non NIHIL");
    }
    n->licita = xar_creare(n->piscina, (i32)magnitudo(chorda));
    per (i = 0; licita[i]; i++)
    {
        *(chorda*)xar_addere(n->licita) = chorda_ex_literis(licita[i], n->piscina);
    }
    redde n;
}

Norma*
norma_forma (Norma* n, constans character* forma)
{
    si (!n)
    {
        redde NIHIL;
    }
    si (n->genus != NORMA_TEXTUS || !forma)
    {
        redde _pravus(n, "forma solum in textu");
    }
    n->forma = chorda_ex_literis(forma, n->piscina);
    redde n;
}

Norma*
norma_descriptio (Norma* n, constans character* textus)
{
    si (n && textus)
    {
        n->descriptio = chorda_ex_literis(textus, n->piscina);
    }
    redde n;
}

Norma*
norma_gignens (Norma* n, NormaGignens functio, vacuum* datum)
{
    si (n)
    {
        n->gignens       = functio;
        n->gignens_datum = datum;
    }
    redde n;
}

NormaVisus
norma_visus (constans Norma* n)
{
    NormaVisus v;

    memset(&v, 0, magnitudo(v));
    si (!n)
    {
        v.error_schematis = _vacua();
        redde v;
    }
    v.genus                = n->genus;
    v.aut_nullum           = n->aut_nullum;
    v.modus                = n->modus;
    v.elementum            = n->elementum;
    v.campi                = n->campi;
    v.clavis_discriminis   = n->clavis_discriminis;
    v.variationes          = n->variationes;
    v.habet_intra          = n->habet_intra;
    v.minimum              = n->minimum;
    v.maximum              = n->maximum;
    v.habet_intra_fluitans = n->habet_intra_fluitans;
    v.minimum_fluitans     = n->minimum_fluitans;
    v.maximum_fluitans     = n->maximum_fluitans;
    v.habet_longitudinem   = n->habet_longitudinem;
    v.longitudo_minima     = n->longitudo_minima;
    v.longitudo_maxima     = n->longitudo_maxima;
    v.licita               = n->licita;
    v.forma                = n->forma;
    v.descriptio           = n->descriptio;
    v.gignens              = n->gignens;
    v.gignens_datum        = n->gignens_datum;
    v.error_schematis      = n->error_schematis;
    redde v;
}


/* ====================================================================
 * B. IUDICIUM
 * ==================================================================== */

nomen structura {
    Piscina* piscina;
        Xar* vitia;
        Xar* notae;
         b32 cessatum;
} NormaContextus;

constans character*
norma_causa_descriptio (NormaCausa causa)
{
    commutatio (causa)
    {
        casus NORMA_CAUSA_GENUS:        redde "genus falsum";
        casus NORMA_CAUSA_DEEST:        redde "campus requisitus deest";
        casus NORMA_CAUSA_EXTRA:        redde "clavis non declarata";
        casus NORMA_CAUSA_MINIMUM:      redde "infra minimum";
        casus NORMA_CAUSA_MAXIMUM:      redde "supra maximum";
        casus NORMA_CAUSA_LONGITUDO:    redde "longitudo extra fines";
        casus NORMA_CAUSA_ELECTIO:      redde "extra electionem";
        casus NORMA_CAUSA_FORMA:        redde "forma fracta";
        casus NORMA_CAUSA_VARIATIO:     redde "variatio ignota";
        casus NORMA_CAUSA_DISCRIMEN:    redde "campus discriminis deest aut non textus";
        casus NORMA_CAUSA_LIMES:        redde "limes vitiorum: iudicium cessavit";
        casus NORMA_CAUSA_SCHEMA_PRAVA: redde "schema pravum";
        ordinarius:                     redde "causa ignota";
    }
}

interior vacuum
_notare (NormaContextus* cx, b32 nota, chorda via, NormaCausa causa,
         constans character* nuntius)
{
    Xar*         x = nota ? cx->notae : cx->vitia;
    NormaVitium* v;

    si (cx->cessatum)
    {
        redde;
    }
    si (xar_numerus(x) >= NORMA_VITIA_MAXIMA)
    {
        si (nota)
        {
            redde;
        }
        v = (NormaVitium*)xar_addere(cx->vitia);
        v->via     = chorda_ex_literis("$", cx->piscina);
        v->causa   = NORMA_CAUSA_LIMES;
        v->nuntius = chorda_ex_literis(norma_causa_descriptio(NORMA_CAUSA_LIMES),
                                       cx->piscina);
        cx->cessatum = VERUM;
        redde;
    }
    v = (NormaVitium*)xar_addere(x);
    v->via     = via;
    v->causa   = causa;
    v->nuntius = chorda_ex_literis(nuntius, cx->piscina);
}

interior b32
_clavis_simplex (chorda k)
{
    i32 i;

    si (k.mensura == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < k.mensura; i++)
    {
        i8 c = k.datum[i];

        si (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
              || (i > 0 && c >= '0' && c <= '9')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior chorda
_via_clavis (chorda via, chorda clavis, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + clavis.mensura + VIII));

    chorda_aedificator_appendere_chorda(aed, via);
    si (_clavis_simplex(clavis))
    {
        chorda_aedificator_appendere_character(aed, '.');
        chorda_aedificator_appendere_chorda(aed, clavis);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(aed, "[\"");
        chorda_aedificator_appendere_evasus_json(aed, clavis);
        chorda_aedificator_appendere_literis(aed, "\"]");
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_via_index (chorda via, i32 index, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + XVI));
    character          numerus[XVI];

    chorda_aedificator_appendere_chorda(aed, via);
    sprintf(numerus, "[%u]", index);
    chorda_aedificator_appendere_literis(aed, numerus);
    redde chorda_aedificator_finire(aed);
}

interior constans character*
_genus_normae (NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_NULLUM:    redde "null";
        casus NORMA_BOOLEAN:   redde "boolean";
        casus NORMA_INTEGER:   redde "integer";
        casus NORMA_NUMERUS:   redde "numerus";
        casus NORMA_TEXTUS:    redde "textus";
        casus NORMA_TABULATUM: redde "tabulatum";
        casus NORMA_OBJECTUM:  redde "objectum";
        casus NORMA_DISCRIMEN: redde "objectum (discrimen)";
        ordinarius:            redde "quidvis";
    }
}

interior constans character*
_genus_valoris (JsonValor* v)
{
    commutatio (json_genus(v))
    {
        casus JSON_NULLUM:    redde "null";
        casus JSON_BOOLEAN:   redde "boolean";
        casus JSON_INTEGER:   redde "integer";
        casus JSON_FLUITANS:  redde "fluitans";
        casus JSON_CHORDA:    redde "textus";
        casus JSON_TABULATUM: redde "tabulatum";
        ordinarius:           redde "objectum";
    }
}

interior vacuum
_genus_falsum (NormaContextus* cx, constans Norma* n, JsonValor* v, chorda via)
{
    character nuntius[CXXVIII];

    sprintf(nuntius, "exspectatur %s, inventum %s", _genus_normae(n->genus),
            v ? _genus_valoris(v) : "null");
    _notare(cx, FALSUM, via, NORMA_CAUSA_GENUS, nuntius);
}

interior b32
_hex (i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/* -1 forma ignota (non probatur), 0 fracta, 1 bona */
interior s32
_forma_probare (chorda forma, chorda t)
{
    i32 i;

    si (chorda_aequalis_literis(forma, "date-time"))
    {
        DiesHora dh;

        redde fasti_ex_iso(t, &dh) ? I : 0;
    }
    si (chorda_aequalis_literis(forma, "uuid"))
    {
        si (t.mensura != XXXVI)
        {
            redde 0;
        }
        per (i = 0; i < XXXVI; i++)
        {
            b32 linea = (i == VIII || i == XIII || i == XVIII || i == XXIII);

            si (linea ? t.datum[i] != '-' : !_hex(t.datum[i]))
            {
                redde 0;
            }
        }
        redde I;
    }
    si (chorda_aequalis_literis(forma, "email"))
    {
        s32 ad = -I;
        s32 punctum = -I;

        per (i = 0; i < t.mensura; i++)
        {
            si (t.datum[i] == '@')
            {
                si (ad >= 0)
                {
                    redde 0;
                }
                ad = (s32)i;
            }
            alioquin si (t.datum[i] == '.' && ad >= 0)
            {
                punctum = (s32)i;
            }
        }
        redde (ad > 0 && punctum > ad + I && punctum < (s32)t.mensura - I) ? I : 0;
    }
    si (chorda_aequalis_literis(forma, "uri"))
    {
        si (t.mensura == 0 || !((t.datum[0] >= 'a' && t.datum[0] <= 'z')
                                || (t.datum[0] >= 'A' && t.datum[0] <= 'Z')))
        {
            redde 0;
        }
        per (i = I; i < t.mensura; i++)
        {
            i8 c = t.datum[i];

            si (c == ':')
            {
                redde i + I < t.mensura ? I : 0;
            }
            si (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                  || (c >= '0' && c <= '9') || c == '+' || c == '.' || c == '-'))
            {
                redde 0;
            }
        }
        redde 0;
    }
    redde -I;
}

interior vacuum _iudicare (constans Norma* n, JsonValor* v, chorda via,
                           NormaContextus* cx);

interior b32
_campus_declaratus (constans Norma* n, chorda clavis)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->campi); i++)
    {
        si (chorda_aequalis(((NormaCampus*)xar_obtinere(n->campi, i))->titulus, clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_iudicare_objectum (constans Norma* n, JsonValor* v, chorda via,
                    chorda clavis_implicita, NormaContextus* cx)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(n->campi, i);
        JsonValor*   f = json_objectum_capere_chorda(v, c->titulus);

        si (!f)
        {
            si (c->requiritur)
            {
                _notare(cx, FALSUM, _via_clavis(via, c->titulus, cx->piscina),
                        NORMA_CAUSA_DEEST, "campus requisitus deest");
            }
            perge;
        }
        _iudicare(c->valor, f, _via_clavis(via, c->titulus, cx->piscina), cx);
    }
    si (n->modus != NORMA_APERTUM)
    {
        JsonObjectumIterator it = json_objectum_iterator(v);
        chorda               k;
        JsonValor*           f;

        dum (json_objectum_iterator_proxima(&it, &k, &f))
        {
            si (clavis_implicita.mensura > 0 && chorda_aequalis(k, clavis_implicita))
            {
                perge;
            }
            si (!_campus_declaratus(n, k))
            {
                _notare(cx, n->modus == NORMA_NOTANDUM,
                        _via_clavis(via, k, cx->piscina), NORMA_CAUSA_EXTRA,
                        "clavis non declarata");
            }
        }
    }
}

interior vacuum
_iudicare_discrimen (constans Norma* n, JsonValor* v, chorda via,
                     NormaContextus* cx)
{
    chorda     via_tag = _via_clavis(via, n->clavis_discriminis, cx->piscina);
    JsonValor* tag = json_objectum_capere_chorda(v, n->clavis_discriminis);
    i32        i;

    si (!tag || !json_est_chorda(tag))
    {
        _notare(cx, FALSUM, via_tag, NORMA_CAUSA_DISCRIMEN,
                norma_causa_descriptio(NORMA_CAUSA_DISCRIMEN));
        redde;
    }
    per (i = 0; i < xar_numerus(n->variationes); i++)
    {
        NormaVariatio* var = (NormaVariatio*)xar_obtinere(n->variationes, i);

        si (chorda_aequalis(var->valor, json_ad_chorda(tag)))
        {
            _iudicare_objectum(var->objectum, v, via, n->clavis_discriminis, cx);
            redde;
        }
    }
    si (n->modus != NORMA_APERTUM)
    {
        _notare(cx, n->modus == NORMA_NOTANDUM, via_tag, NORMA_CAUSA_VARIATIO,
                "variatio ignota");
    }
}

interior vacuum
_iudicare_textum (constans Norma* n, JsonValor* v, chorda via, NormaContextus* cx)
{
    chorda t = json_ad_chorda(v);
    s32    runae = utf8_numerare_runas(t.datum, (s32)t.mensura);
    i32    i;

    si (n->habet_longitudinem
        && (runae < (s32)n->longitudo_minima || runae > (s32)n->longitudo_maxima))
    {
        _notare(cx, FALSUM, via, NORMA_CAUSA_LONGITUDO, "longitudo textus extra fines");
    }
    si (n->licita)
    {
        b32 inventum = FALSUM;

        per (i = 0; i < xar_numerus(n->licita); i++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(n->licita, i), t))
            {
                inventum = VERUM;
            }
        }
        si (!inventum)
        {
            _notare(cx, FALSUM, via, NORMA_CAUSA_ELECTIO, "valor extra electionem");
        }
    }
    si (n->forma.mensura > 0 && _forma_probare(n->forma, t) == 0)
    {
        _notare(cx, FALSUM, via, NORMA_CAUSA_FORMA, "forma fracta");
    }
}

interior vacuum
_iudicare (constans Norma* n, JsonValor* v, chorda via, NormaContextus* cx)
{
    si (cx->cessatum || n->genus == NORMA_LIBERUM)
    {
        redde;
    }
    si (!v || json_est_nullum(v))
    {
        si (n->genus != NORMA_NULLUM && !n->aut_nullum)
        {
            _genus_falsum(cx, n, v, via);
        }
        redde;
    }
    commutatio (n->genus)
    {
        casus NORMA_NULLUM:
            _genus_falsum(cx, n, v, via);
            frange;
        casus NORMA_BOOLEAN:
            si (!json_est_boolean(v))
            {
                _genus_falsum(cx, n, v, via);
            }
            frange;
        casus NORMA_INTEGER:
            si (!json_est_integer(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            si (n->habet_intra && json_ad_integer(v) < n->minimum)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MINIMUM, "infra minimum");
            }
            si (n->habet_intra && json_ad_integer(v) > n->maximum)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MAXIMUM, "supra maximum");
            }
            frange;
        casus NORMA_NUMERUS:
        {
            f64 x;

            si (!json_est_integer(v) && !json_est_fluitans(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            x = json_est_integer(v) ? (f64)json_ad_integer(v) : json_ad_fluitans(v);
            si (n->habet_intra_fluitans && x < n->minimum_fluitans)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MINIMUM, "infra minimum");
            }
            si (n->habet_intra_fluitans && x > n->maximum_fluitans)
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_MAXIMUM, "supra maximum");
            }
            frange;
        }
        casus NORMA_TEXTUS:
            si (!json_est_chorda(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_textum(n, v, via, cx);
            frange;
        casus NORMA_TABULATUM:
        {
            i32 i;
            i32 numerus;

            si (!json_est_tabulatum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            numerus = json_tabulatum_numerus(v);
            si (n->habet_longitudinem
                && (numerus < n->longitudo_minima || numerus > n->longitudo_maxima))
            {
                _notare(cx, FALSUM, via, NORMA_CAUSA_LONGITUDO,
                        "numerus elementorum extra fines");
            }
            per (i = 0; i < numerus && !cx->cessatum; i++)
            {
                _iudicare(n->elementum, json_tabulatum_obtinere(v, i),
                          _via_index(via, i, cx->piscina), cx);
            }
            frange;
        }
        casus NORMA_OBJECTUM:
            si (!json_est_objectum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_objectum(n, v, via, _vacua(), cx);
            frange;
        casus NORMA_DISCRIMEN:
            si (!json_est_objectum(v))
            {
                _genus_falsum(cx, n, v, via);
                frange;
            }
            _iudicare_discrimen(n, v, via, cx);
            frange;
        ordinarius:
            frange;
    }
}

interior chorda
_pravitas (constans Norma* n)
{
    i32    i;
    chorda e;

    si (!n)
    {
        redde _vacua();
    }
    si (n->error_schematis.mensura > 0)
    {
        redde n->error_schematis;
    }
    si (n->genus == NORMA_TABULATUM)
    {
        redde _pravitas(n->elementum);
    }
    per (i = 0; n->campi && i < xar_numerus(n->campi); i++)
    {
        e = _pravitas(((NormaCampus*)xar_obtinere(n->campi, i))->valor);
        si (e.mensura > 0)
        {
            redde e;
        }
    }
    per (i = 0; n->variationes && i < xar_numerus(n->variationes); i++)
    {
        e = _pravitas(((NormaVariatio*)xar_obtinere(n->variationes, i))->objectum);
        si (e.mensura > 0)
        {
            redde e;
        }
    }
    redde _vacua();
}

NormaIudicium
norma_iudicare (constans Norma* n, JsonValor* valor, Piscina* piscina)
{
    NormaIudicium  j;
    NormaContextus cx;
    chorda         pravitas;

    memset(&j, 0, magnitudo(j));
    j.vitia = xar_creare(piscina, (i32)magnitudo(NormaVitium));
    j.notae = xar_creare(piscina, (i32)magnitudo(NormaVitium));
    cx.piscina  = piscina;
    cx.vitia    = j.vitia;
    cx.notae    = j.notae;
    cx.cessatum = FALSUM;
    pravitas = n ? _pravitas(n) : chorda_ex_literis("schema NIHIL", piscina);
    si (pravitas.mensura > 0)
    {
        NormaVitium* v = (NormaVitium*)xar_addere(j.vitia);

        v->via     = chorda_ex_literis("$", piscina);
        v->causa   = NORMA_CAUSA_SCHEMA_PRAVA;
        v->nuntius = pravitas;
        j.validum  = FALSUM;
        redde j;
    }
    _iudicare(n, valor, chorda_ex_literis("$", piscina), &cx);
    j.validum = xar_numerus(j.vitia) == 0;
    redde j;
}
```

  To confirm on first compile (rule on mismatch): whether
  `chorda_aedificator_appendere_evasus_json` adds its own quotes (the
  path tests pin the result), `json_objectum_capere_chorda`'s argument
  order, `utf8_numerare_runas` taking `const i8*`.

- [ ] **N2.4** register, green, lint.
- [ ] **N2.5 plants** - (1) NOTANDUM treated as CLAUSUM -> notes test red;
  (2) cap removed -> LIMES test red (1000 issues); (3) `_clavis_simplex`
  always VERUM -> bracket-path tests red; (4) runes counted as bytes ->
  "āē" test red; (5) implicit tag not skipped -> discrimen valid case red;
  (6) `_pravitas` not recursing into array elements -> nested pravus red.
- [ ] **N2.6** worklog (path rules, int-vs-float, cap) + commit; gates
  `radix`, `generata`.

---

### Task N3: JSON Schema export

**Files:** Modify `lib/norma.c` (part C, appended), `probationes/probatio_norma.c`.

Key order (fixed; pinned by goldens): scalar `type`, `description`,
`minimum`, `maximum`, `minLength`, `maxLength`, `enum`, `format`; array
`type`, `description`, `items`, `minItems`, `maxItems`; object `type`,
`description`, `properties`, `required`, `additionalProperties`;
discriminated union `description`, `oneOf` (each variant an object whose
`properties` start with the tag as `{"const": value}` and whose
`required` starts with the tag); `liberum` -> `{}` (+ description).

- [ ] **N3.1 failing tests** (call `probatio_exportatio(p);` under `/* N3 */`):

```c
interior chorda
_js (Norma* n, Piscina* p)
{
    redde json_scribere(norma_json_schema(n, p), p);
}

interior vacuum
probatio_exportatio(Piscina* p)
{
    constans character* constans licita[] = { "a", "b", NIHIL };
    Norma* o;
    Norma* d;
    Norma* t;

    imprimere("\n--- Probans exportatio JSON Schema ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_descriptio(
        norma_intra(norma_integer(p), 0, MMXLVIII), "n"), p),
        "{\"type\":\"integer\",\"description\":\"n\",\"minimum\":0,\"maximum\":2048}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_forma(norma_longitudo(
        norma_aut_nullum(norma_textus(p)), I, V), "date-time"), p),
        "{\"type\":[\"string\",\"null\"],\"minLength\":1,\"maxLength\":5,"
        "\"format\":\"date-time\"}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_electio(norma_textus(p), licita), p),
        "{\"type\":\"string\",\"enum\":[\"a\",\"b\"]}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_intra_fluitans(norma_numerus(p), 0.5, 2.5), p),
        "{\"type\":\"number\",\"minimum\":0.5,\"maximum\":2.5}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_longitudo(
        norma_tabulatum(p, norma_boolean(p)), I, III), p),
        "{\"type\":\"array\",\"items\":{\"type\":\"boolean\"},\"minItems\":1,\"maxItems\":3}");
    o = norma_objectum(p);
    norma_campus(o, "id", norma_integer(p), VERUM);
    norma_campus(o, "n", norma_integer(p), FALSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(o, p),
        "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"integer\"},"
        "\"n\":{\"type\":\"integer\"}},\"required\":[\"id\"],"
        "\"additionalProperties\":false}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_modus(o, NORMA_NOTANDUM), p),
        "{\"type\":\"object\",\"properties\":{\"id\":{\"type\":\"integer\"},"
        "\"n\":{\"type\":\"integer\"}},\"required\":[\"id\"]}");
    t = norma_objectum(p);
    norma_campus(t, "text", norma_textus(p), VERUM);
    d = norma_discrimen(p, "type");
    norma_variatio(d, "text", t);
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(d, p),
        "{\"oneOf\":[{\"type\":\"object\",\"properties\":{\"type\":{\"const\":\"text\"},"
        "\"text\":{\"type\":\"string\"}},\"required\":[\"type\",\"text\"],"
        "\"additionalProperties\":false}]}");
    CREDO_CHORDA_AEQUALIS_LITERIS(_js(norma_liberum(p), p), "{}");
    CREDO_CHORDA_AEQUALIS(_js(o, p), _js(o, p));
}
```

- [ ] **N3.2** run -> undefined `norma_json_schema`.

- [ ] **N3.3 implement part C** (append to `lib/norma.c`):

```c
/* ====================================================================
 * C. EXPORTATIO JSON SCHEMA
 * ==================================================================== */

interior constans character*
_typus_json (NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_NULLUM:    redde "null";
        casus NORMA_BOOLEAN:   redde "boolean";
        casus NORMA_INTEGER:   redde "integer";
        casus NORMA_NUMERUS:   redde "number";
        casus NORMA_TEXTUS:    redde "string";
        casus NORMA_TABULATUM: redde "array";
        ordinarius:            redde "object";
    }
}

interior vacuum
_typum_ponere (JsonValor* o, constans Norma* n, Piscina* p)
{
    si (n->aut_nullum && n->genus != NORMA_NULLUM)
    {
        JsonValor* arr = json_tabulatum_creare(p);

        json_tabulatum_addere(arr, json_chorda_creare_literis(p, _typus_json(n->genus)));
        json_tabulatum_addere(arr, json_chorda_creare_literis(p, "null"));
        json_objectum_ponere(o, "type", arr);
    }
    alioquin
    {
        json_objectum_ponere(o, "type", json_chorda_creare_literis(p, _typus_json(n->genus)));
    }
    si (n->descriptio.mensura > 0)
    {
        json_objectum_ponere(o, "description", json_chorda_creare(p, n->descriptio));
    }
}

interior JsonValor* _exportare (constans Norma* n, Piscina* p);

interior JsonValor*
_exportare_objectum (constans Norma* n, chorda tag, chorda valor_tag, Piscina* p)
{
    JsonValor* o = json_objectum_creare(p);
    JsonValor* props = json_objectum_creare(p);
    JsonValor* req = json_tabulatum_creare(p);
    i32        i;

    _typum_ponere(o, n, p);
    si (tag.mensura > 0)
    {
        JsonValor* c = json_objectum_creare(p);

        json_objectum_ponere(c, "const", json_chorda_creare(p, valor_tag));
        json_objectum_ponere_chorda(props, tag, c);
        json_tabulatum_addere(req, json_chorda_creare(p, tag));
    }
    per (i = 0; i < xar_numerus(n->campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(n->campi, i);

        json_objectum_ponere_chorda(props, c->titulus, _exportare(c->valor, p));
        si (c->requiritur)
        {
            json_tabulatum_addere(req, json_chorda_creare(p, c->titulus));
        }
    }
    json_objectum_ponere(o, "properties", props);
    si (json_tabulatum_numerus(req) > 0)
    {
        json_objectum_ponere(o, "required", req);
    }
    si (n->modus == NORMA_CLAUSUM)
    {
        json_objectum_ponere(o, "additionalProperties", json_boolean_creare(p, FALSUM));
    }
    redde o;
}

interior JsonValor*
_exportare (constans Norma* n, Piscina* p)
{
    JsonValor* o = json_objectum_creare(p);
    i32        i;

    si (!n)
    {
        redde o;
    }
    commutatio (n->genus)
    {
        casus NORMA_LIBERUM:
            si (n->descriptio.mensura > 0)
            {
                json_objectum_ponere(o, "description", json_chorda_creare(p, n->descriptio));
            }
            redde o;
        casus NORMA_OBJECTUM:
            redde _exportare_objectum(n, _vacua(), _vacua(), p);
        casus NORMA_DISCRIMEN:
        {
            JsonValor* una = json_tabulatum_creare(p);

            si (n->descriptio.mensura > 0)
            {
                json_objectum_ponere(o, "description", json_chorda_creare(p, n->descriptio));
            }
            per (i = 0; i < xar_numerus(n->variationes); i++)
            {
                NormaVariatio* v = (NormaVariatio*)xar_obtinere(n->variationes, i);

                json_tabulatum_addere(una, _exportare_objectum(v->objectum,
                    n->clavis_discriminis, v->valor, p));
            }
            si (n->aut_nullum)
            {
                JsonValor* nul = json_objectum_creare(p);

                json_objectum_ponere(nul, "type", json_chorda_creare_literis(p, "null"));
                json_tabulatum_addere(una, nul);
            }
            json_objectum_ponere(o, "oneOf", una);
            redde o;
        }
        ordinarius:
            frange;
    }
    _typum_ponere(o, n, p);
    si (n->habet_intra)
    {
        json_objectum_ponere(o, "minimum", json_integer_creare(p, n->minimum));
        json_objectum_ponere(o, "maximum", json_integer_creare(p, n->maximum));
    }
    si (n->habet_intra_fluitans)
    {
        json_objectum_ponere(o, "minimum", json_fluitans_creare(p, n->minimum_fluitans));
        json_objectum_ponere(o, "maximum", json_fluitans_creare(p, n->maximum_fluitans));
    }
    si (n->genus == NORMA_TABULATUM)
    {
        json_objectum_ponere(o, "items", _exportare(n->elementum, p));
        si (n->habet_longitudinem)
        {
            json_objectum_ponere(o, "minItems", json_integer_creare(p, (s64)n->longitudo_minima));
            json_objectum_ponere(o, "maxItems", json_integer_creare(p, (s64)n->longitudo_maxima));
        }
        redde o;
    }
    si (n->habet_longitudinem)
    {
        json_objectum_ponere(o, "minLength", json_integer_creare(p, (s64)n->longitudo_minima));
        json_objectum_ponere(o, "maxLength", json_integer_creare(p, (s64)n->longitudo_maxima));
    }
    si (n->licita)
    {
        JsonValor* e = json_tabulatum_creare(p);

        per (i = 0; i < xar_numerus(n->licita); i++)
        {
            json_tabulatum_addere(e, json_chorda_creare(p, *(chorda*)xar_obtinere(n->licita, i)));
        }
        json_objectum_ponere(o, "enum", e);
    }
    si (n->forma.mensura > 0)
    {
        json_objectum_ponere(o, "format", json_chorda_creare(p, n->forma));
    }
    redde o;
}

JsonValor*
norma_json_schema (constans Norma* n, Piscina* piscina)
{
    redde _exportare(n, piscina);
}
```

- [ ] **N3.4** green; **plants** - (1) CLAUSUM not exported -> object
  golden red; (2) tag not first in `required` -> union golden red;
  (3) aut_nullum ignored -> nullable golden red.
- [ ] **N3.5** worklog (mapping table) + commit; gates `radix`, `generata`.

---

### Task N4: norma_gignere + the mutual oracle

**Files:** Create `lib/norma_gignere.c`, `probationes/probatio_norma_gignere.c`;
regenerate list; worklog section in `lib/norma.worklog.md`.

**Interfaces:** Produces `include/norma_gignere.h` (approved). Consumes
`norma_visus`, `norma_iudicare` (tests), fictio, `sigillum_computare`,
`sors_derivare`, `sors_intra`, `sors_casu`, `utf8_proxima_runa`.

Decisions (worklog): path helpers are a small COPY of norma.c's (the
paths must match the judge's byte for byte - the oracle checks it);
a field's presence draws from its OWN stream (`<path>?`), so adding an
optional field never changes whether later fields appear; a known
format wins over length bounds when generating (a schema whose bounds
contradict its format cannot be satisfied - not the generator's call).

- [ ] **N4.1 failing tests** - `probationes/probatio_norma_gignere.c`:

```c
/* probatio_norma_gignere.c - oraculum mutuum (norma-plan-2 N4) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "norma.h"
#include "norma_gignere.h"

#include <stdio.h>
#include <string.h>

#define SEMINA CCC

interior JsonValor*
_ex_functione (Sors* sors, Piscina* p, vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_chorda_creare_literis(p, "ex functione");
}

interior JsonValor*
_functio_prava (Sors* sors, Piscina* p, vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_integer_creare(p, VII);   /* textus exspectatur */
}

interior Norma*
_schema_simplex (Piscina* p)
{
    constans character* constans licita[] = { "a", "b", "c", NIHIL };
    Norma* o = norma_objectum(p);

    norma_campus(o, "id", norma_intra(norma_integer(p), 0, MMXLVIII), VERUM);
    norma_campus(o, "nomen", norma_longitudo(norma_textus(p), I, XL), FALSUM);
    norma_campus(o, "email", norma_forma(norma_textus(p), "email"), VERUM);
    norma_campus(o, "codex", norma_longitudo(norma_forma(norma_textus(p), "uuid"),
                                              XXXVI, XXXVI), VERUM);
    norma_campus(o, "tags", norma_longitudo(norma_tabulatum(p,
        norma_electio(norma_textus(p), licita)), 0, IV), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1, 0.3), VERUM);
    norma_campus(o, "nota", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(o, "datum", norma_forma(norma_textus(p), "date-time"), VERUM);
    norma_campus(o, "origo", norma_gignens(norma_textus(p), _ex_functione, NIHIL), VERUM);
    redde o;
}

interior Norma*
_schema_responsi (Piscina* p)
{
    constans character* constans fines[] = { "end_turn", "tool_use", "max_tokens", NIHIL };
    Norma* textus = norma_objectum(p);
    Norma* petitum = norma_objectum(p);
    Norma* blocus = norma_discrimen(p, "type");
    Norma* usus = norma_objectum(p);
    Norma* r = norma_objectum(p);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_modus(norma_objectum(p), NORMA_APERTUM), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_campus(usus, "input_tokens", norma_intra(norma_integer(p), 0, M * M), VERUM);
    norma_campus(usus, "output_tokens", norma_intra(norma_integer(p), 0, M * M), VERUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_electio(norma_textus(p), fines), VERUM);
    norma_campus(r, "usage", usus, VERUM);
    redde r;
}

/* scribere + relegere: fines fluitantes et effugia per filum vera */
interior JsonValor*
_iterum (JsonValor* v, Piscina* p)
{
    redde json_legere(json_scribere(v, p), p).radix;
}

interior vacuum
_oraculum (Norma* n, constans character* titulus, Piscina* p)
{
    s64 semen;
    i32 fracti = 0;

    imprimere("\n--- Oraculum mutuum: %s (%u semina) ---\n", titulus, SEMINA);
    per (semen = 0; semen < SEMINA; semen++)
    {
        NormaGenitum  t = norma_gignere(n, NORMA_TYPICA, semen, p);
        NormaGenitum  f = norma_gignere(n, NORMA_FINES, semen, p);
        NormaGenitum  x = norma_gignere(n, NORMA_INVALIDA, semen, p);
        NormaIudicium jt = norma_iudicare(n, _iterum(t.valor, p), p);
        NormaIudicium jf = norma_iudicare(n, _iterum(f.valor, p), p);
        NormaIudicium jx;

        si (!jt.validum || !jf.validum)
        {
            fracti++;
            si (fracti <= III)
            {
                NormaVitium* v = (NormaVitium*)xar_obtinere(
                    jt.validum ? jf.vitia : jt.vitia, 0);

                imprimere("  semen %ld %s: %.*s %s\n", (longus)semen,
                          jt.validum ? "FINES" : "TYPICA",
                          (integer)v->via.mensura, (constans character*)v->via.datum,
                          norma_causa_descriptio(v->causa));
            }
            perge;
        }
        CREDO_NON_NIHIL(x.valor);
        si (!x.valor)
        {
            perge;
        }
        jx = norma_iudicare(n, _iterum(x.valor, p), p);
        si (xar_numerus(jx.vitia) != I
            || !chorda_aequalis(((NormaVitium*)xar_obtinere(jx.vitia, 0))->via, x.via_fracta)
            || ((NormaVitium*)xar_obtinere(jx.vitia, 0))->causa != x.causa_fracta)
        {
            fracti++;
            si (fracti <= III)
            {
                imprimere("  semen %ld INVALIDA: fracta %.*s (%s), vitia %u\n",
                          (longus)semen, (integer)x.via_fracta.mensura,
                          (constans character*)x.via_fracta.datum,
                          norma_causa_descriptio(x.causa_fracta),
                          xar_numerus(jx.vitia));
            }
        }
    }
    CREDO_AEQUALIS_I32(fracti, 0);
}

interior vacuum
probatio_determinismus_et_stabilitas(Piscina* p)
{
    Norma* a = norma_objectum(p);
    Norma* b = norma_objectum(p);
    s64    semen;

    imprimere("\n--- Probans determinismus et stabilitas ---\n");
    CREDO_CHORDA_AEQUALIS(
        json_scribere(norma_gignere(_schema_simplex(p), NORMA_TYPICA, XLII, p).valor, p),
        json_scribere(norma_gignere(_schema_simplex(p), NORMA_TYPICA, XLII, p).valor, p));
    norma_campus(a, "x", norma_integer(p), VERUM);
    norma_campus(a, "y", norma_textus(p), FALSUM);
    norma_campus(b, "x", norma_integer(p), VERUM);
    norma_campus(b, "w", norma_textus(p), FALSUM);    /* campus additus */
    norma_campus(b, "y", norma_textus(p), FALSUM);
    norma_modus(b, NORMA_APERTUM);
    per (semen = 0; semen < L; semen++)
    {
        JsonValor* va = norma_gignere(a, NORMA_TYPICA, semen, p).valor;
        JsonValor* vb = norma_gignere(b, NORMA_TYPICA, semen, p).valor;

        CREDO_CHORDA_AEQUALIS(json_scribere(json_objectum_capere(va, "x"), p),
                              json_scribere(json_objectum_capere(vb, "x"), p));
        CREDO_VERUM((json_objectum_capere(va, "y") == NIHIL)
                    == (json_objectum_capere(vb, "y") == NIHIL));
        si (json_objectum_capere(va, "y"))
        {
            CREDO_CHORDA_AEQUALIS(json_scribere(json_objectum_capere(va, "y"), p),
                                  json_scribere(json_objectum_capere(vb, "y"), p));
        }
    }
}

interior vacuum
probatio_functiones(Piscina* p)
{
    Norma*        o = norma_objectum(p);
    NormaGenitum  g;

    imprimere("\n--- Probans functiones gignentes ---\n");
    g = norma_gignere(_schema_simplex(p), NORMA_TYPICA, VII, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(g.valor, "origo")),
                                  "ex functione");
    /* officina prava ab oraculo capitur */
    norma_campus(o, "t", norma_gignens(norma_textus(p), _functio_prava, NIHIL), VERUM);
    g = norma_gignere(o, NORMA_TYPICA, I, p);
    CREDO_FALSUM(norma_iudicare(o, g.valor, p).validum);
    /* nihil violabile */
    CREDO_NIHIL(norma_gignere(norma_liberum(p), NORMA_INVALIDA, I, p).valor);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_gignere", M * M);
    b32      successus;

    credo_aperire(p);
    _oraculum(_schema_simplex(p), "simplex", p);
    _oraculum(_schema_responsi(p), "responsum", p);
    probatio_determinismus_et_stabilitas(p);
    probatio_functiones(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
```

- [ ] **N4.2** run -> link failure.

- [ ] **N4.3 implement `lib/norma_gignere.c`:**

```c
/* norma_gignere.c - valores JSON ficti ex schemate (norma-spec par. IV;
 * norma-plan-2 N4). Semina PER VIAM: clavis = VIII octeti primi
 * sigilli (SHA-256) textus viae. Viae = eaedem ac iudicis (copia
 * auxiliorum norma.c; oraculum aequalitatem probat). */
#include "norma_gignere.h"
#include "fictio.h"
#include "sigillum.h"
#include "utf8.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

nomen structura {
              Piscina* piscina;
                 Sors  radix;
    NormaModusGignendi modus;   /* TYPICA aut FINES */
} Gignitor;

/* ---- viae (copia norma.c) ---- */

interior b32
_clavis_simplex (chorda k)
{
    i32 i;

    si (k.mensura == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < k.mensura; i++)
    {
        i8 c = k.datum[i];

        si (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
              || (i > 0 && c >= '0' && c <= '9')))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior chorda
_via_clavis (chorda via, chorda clavis, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + clavis.mensura + VIII));

    chorda_aedificator_appendere_chorda(aed, via);
    si (_clavis_simplex(clavis))
    {
        chorda_aedificator_appendere_character(aed, '.');
        chorda_aedificator_appendere_chorda(aed, clavis);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(aed, "[\"");
        chorda_aedificator_appendere_evasus_json(aed, clavis);
        chorda_aedificator_appendere_literis(aed, "\"]");
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_via_index (chorda via, i32 index, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + XVI));
    character          numerus[XVI];

    chorda_aedificator_appendere_chorda(aed, via);
    sprintf(numerus, "[%u]", index);
    chorda_aedificator_appendere_literis(aed, numerus);
    redde chorda_aedificator_finire(aed);
}

interior chorda
_via_suffixa (chorda via, constans character* suffixum, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(via.mensura + XVI));

    chorda_aedificator_appendere_chorda(aed, via);
    chorda_aedificator_appendere_literis(aed, suffixum);
    redde chorda_aedificator_finire(aed);
}

interior Sors
_sors_viae (constans Gignitor* g, chorda via)
{
    Sigillum s = sigillum_computare(via.datum, (memoriae_index)via.mensura);
    i64      clavis = 0;
    i32      i;

    per (i = 0; i < VIII; i++)
    {
        clavis = (clavis << VIII) | (i64)s.octeti[i];
    }
    redde sors_derivare(&g->radix, clavis);
}

/* textus ex runis 'fons' cyclice usque ad 'longitudo' runas */
interior chorda
_runis_implere (chorda fons, i32 longitudo, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(longitudo * IV + I));
    constans i8*       p;
    constans i8*       finis;
    i32                n = 0;

    si (fons.mensura == 0)
    {
        fons = chorda_ex_literis("a", piscina);
    }
    p = fons.datum;
    finis = fons.datum + fons.mensura;
    dum (n < longitudo)
    {
        constans i8* q = utf8_proxima_runa(p, finis);

        si (q <= p)
        {
            q = p + I;
        }
        dum (p < q)
        {
            chorda_aedificator_appendere_character(aed, (character)*p);
            p++;
        }
        n++;
        si (p >= finis)
        {
            p = fons.datum;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior JsonValor* _gignere (constans Gignitor* g, constans Norma* n, chorda via);

interior JsonValor*
_textum_gignere (constans Gignitor* g, NormaVisus v, chorda via)
{
    Sors     s = _sors_viae(g, via);
    Piscina* p = g->piscina;
    i32      minimum = v.habet_longitudinem ? v.longitudo_minima : III;
    i32      maximum = v.habet_longitudinem ? v.longitudo_maxima : XII;
    i32      longitudo;
    chorda   fons;

    si (v.licita && xar_numerus(v.licita) > 0)
    {
        i32 numerus = xar_numerus(v.licita);
        i32 k = g->modus == NORMA_FINES ? (sors_casu(&s, I, II) ? 0 : numerus - I)
                                        : sors_intra(&s, numerus);

        redde json_chorda_creare(p, *(chorda*)xar_obtinere(v.licita, k));
    }
    si (chorda_aequalis_literis(v.forma, "date-time"))
    {
        redde json_chorda_creare(p, fictio_tempus(&s, 0, (s64)4102444800, p));
    }
    si (chorda_aequalis_literis(v.forma, "uuid"))
    {
        redde json_chorda_creare(p, fictio_uuid(&s, p));
    }
    si (chorda_aequalis_literis(v.forma, "email"))
    {
        redde json_chorda_creare(p, fictio_email(&s, p));
    }
    si (chorda_aequalis_literis(v.forma, "uri"))
    {
        ChordaAedificator* aed = chorda_aedificator_creare(p, LXIV);

        chorda_aedificator_appendere_literis(aed, "https://example.org/");
        chorda_aedificator_appendere_chorda(aed, fictio_textus(&s, III, VIII, NIHIL, p));
        redde json_chorda_creare(p, chorda_aedificator_finire(aed));
    }
    longitudo = g->modus == NORMA_FINES
              ? (sors_casu(&s, I, II) ? minimum : maximum)
              : (i32)fictio_integer(&s, (s64)minimum, (s64)maximum);
    fons = g->modus == NORMA_FINES ? fictio_textus_difficilis(&s, p)
                                   : fictio_textus_latinus(&s, I, p);
    redde json_chorda_creare(p, _runis_implere(fons, longitudo, p));
}

interior JsonValor*
_objectum_gignere (constans Gignitor* g, constans Norma* n, chorda via,
                   chorda tag, chorda valor_tag)
{
    Piscina*   p = g->piscina;
    NormaVisus v = norma_visus(n);
    JsonValor* o = json_objectum_creare(p);
    Sors       s = _sors_viae(g, via);
    b32        omnes = sors_casu(&s, I, II);   /* FINES: omnes aut nulli */
    i32        i;

    si (tag.mensura > 0)
    {
        json_objectum_ponere_chorda(o, tag, json_chorda_creare(p, valor_tag));
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);
        chorda       via_c = _via_clavis(via, c->titulus, p);
        b32          adest = c->requiritur;

        si (!adest)
        {
            Sors sp = _sors_viae(g, _via_suffixa(via_c, "?", p));

            adest = g->modus == NORMA_FINES ? omnes : sors_casu(&sp, I, II);
        }
        si (adest)
        {
            json_objectum_ponere_chorda(o, c->titulus, _gignere(g, c->valor, via_c));
        }
    }
    redde o;
}

interior JsonValor*
_gignere (constans Gignitor* g, constans Norma* n, chorda via)
{
    Piscina*   p = g->piscina;
    NormaVisus v = norma_visus(n);
    Sors       s = _sors_viae(g, via);

    si (v.gignens)
    {
        redde v.gignens(&s, p, v.gignens_datum);
    }
    si (v.aut_nullum && v.genus != NORMA_NULLUM
        && (g->modus == NORMA_FINES ? sors_casu(&s, I, II) : sors_casu(&s, I, X)))
    {
        redde json_nullum_creare(p);
    }
    commutatio (v.genus)
    {
        casus NORMA_LIBERUM:
            si (g->modus == NORMA_FINES)
            {
                redde json_nullum_creare(p);
            }
            redde sors_casu(&s, I, II)
                ? json_integer_creare(p, fictio_integer(&s, 0, M))
                : json_chorda_creare(p, fictio_textus_latinus(&s, I, p));
        casus NORMA_NULLUM:
            redde json_nullum_creare(p);
        casus NORMA_BOOLEAN:
            redde json_boolean_creare(p, sors_casu(&s, I, II));
        casus NORMA_INTEGER:
        {
            s64 minimum = v.habet_intra ? v.minimum : 0;
            s64 maximum = v.habet_intra ? v.maximum : M;

            redde json_integer_creare(p, g->modus == NORMA_FINES
                ? (sors_casu(&s, I, II) ? minimum : maximum)
                : fictio_integer(&s, minimum, maximum));
        }
        casus NORMA_NUMERUS:
        {
            f64 minimum = v.habet_intra_fluitans ? v.minimum_fluitans : 0.0;
            f64 maximum = v.habet_intra_fluitans ? v.maximum_fluitans : 1000.0;

            redde json_fluitans_creare(p, g->modus == NORMA_FINES
                ? (sors_casu(&s, I, II) ? minimum : maximum)
                : fictio_numerus(&s, minimum, maximum));
        }
        casus NORMA_TEXTUS:
            redde _textum_gignere(g, v, via);
        casus NORMA_TABULATUM:
        {
            JsonValor* arr = json_tabulatum_creare(p);
            i32        numerus;
            i32        i;

            si (v.habet_longitudinem)
            {
                i32 summum = v.longitudo_maxima < v.longitudo_minima + II
                           ? v.longitudo_maxima : v.longitudo_minima + II;

                numerus = g->modus == NORMA_FINES
                        ? (sors_casu(&s, I, II) ? v.longitudo_minima : v.longitudo_maxima)
                        : (i32)fictio_integer(&s, (s64)v.longitudo_minima, (s64)summum);
            }
            alioquin
            {
                numerus = g->modus == NORMA_FINES ? (sors_casu(&s, I, II) ? 0 : III)
                                                  : (i32)fictio_integer(&s, I, III);
            }
            per (i = 0; i < numerus; i++)
            {
                json_tabulatum_addere(arr, _gignere(g, v.elementum, _via_index(via, i, p)));
            }
            redde arr;
        }
        casus NORMA_OBJECTUM:
            redde _objectum_gignere(g, n, via, chorda_ex_literis("", p),
                                    chorda_ex_literis("", p));
        casus NORMA_DISCRIMEN:
        {
            NormaVariatio* var;

            si (!v.variationes || xar_numerus(v.variationes) == 0)
            {
                redde json_objectum_creare(p);
            }
            var = (NormaVariatio*)xar_obtinere(v.variationes,
                      sors_intra(&s, xar_numerus(v.variationes)));
            redde _objectum_gignere(g, var->objectum, via, v.clavis_discriminis,
                                    var->valor);
        }
        ordinarius:
            redde json_nullum_creare(p);
    }
}


/* ====================================================================
 * INVALIDA - mutationes, vitium UNUM quaeque
 * ==================================================================== */

nomen enumeratio {
    MUTATIO_GENUS = 0,
    MUTATIO_DEEST,
    MUTATIO_EXTRA,
    MUTATIO_MINIMUM,
    MUTATIO_MAXIMUM,
    MUTATIO_BREVIUS,
    MUTATIO_LONGIUS,
    MUTATIO_ELECTIO,
    MUTATIO_FORMA,
    MUTATIO_VARIATIO,
    MUTATIO_SINE_TAG
} MutatioGenus;

nomen structura {
     MutatioGenus genus;
    constans Norma* n;
        JsonValor* valor;     /* valor mutandus (aut parens pro DEEST/SINE_TAG) */
           chorda  clavis;    /* DEEST/SINE_TAG: clavis removenda */
           chorda  via;       /* via vitii a iudice reddenda */
} Mutatio;

interior b32
_forma_nota (chorda forma)
{
    redde chorda_aequalis_literis(forma, "date-time") || chorda_aequalis_literis(forma, "uuid")
        || chorda_aequalis_literis(forma, "email") || chorda_aequalis_literis(forma, "uri");
}

interior vacuum
_addere (Xar* m, MutatioGenus genus, constans Norma* n, JsonValor* valor,
         chorda clavis, chorda via)
{
    Mutatio* x = (Mutatio*)xar_addere(m);

    x->genus  = genus;
    x->n      = n;
    x->valor  = valor;
    x->clavis = clavis;
    x->via    = via;
}

/* textus 'quid' (runae) intra fines nodi? */
interior b32
_intra_longitudinem (NormaVisus v, i32 runae)
{
    redde !v.habet_longitudinem
        || (runae >= v.longitudo_minima && runae <= v.longitudo_maxima);
}

interior vacuum _colligere (Xar* m, constans Norma* n, JsonValor* valor, chorda via,
                            Piscina* p);

interior vacuum
_colligere_objectum (Xar* m, constans Norma* n, JsonValor* valor, chorda via,
                     chorda tag, Piscina* p)
{
    NormaVisus v = norma_visus(n);
    i32        i;

    si (v.modus == NORMA_CLAUSUM)
    {
        chorda extranea = chorda_ex_literis("clavis_extranea", p);

        _addere(m, MUTATIO_EXTRA, n, valor, extranea, _via_clavis(via, extranea, p));
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);
        JsonValor*   f = json_objectum_capere_chorda(valor, c->titulus);
        chorda       via_c = _via_clavis(via, c->titulus, p);

        si (!f)
        {
            perge;
        }
        si (c->requiritur)
        {
            _addere(m, MUTATIO_DEEST, c->valor, valor, c->titulus, via_c);
        }
        _colligere(m, c->valor, f, via_c, p);
    }
    (vacuum)tag;
}

interior vacuum
_colligere (Xar* m, constans Norma* n, JsonValor* valor, chorda via, Piscina* p)
{
    NormaVisus v = norma_visus(n);
    i32        i;

    si (!valor || json_est_nullum(valor) || v.genus == NORMA_LIBERUM || v.gignens)
    {
        redde;   /* null licitum, liberum, functio vocantis: non mutantur */
    }
    _addere(m, MUTATIO_GENUS, n, valor, chorda_ex_literis("", p), via);
    commutatio (v.genus)
    {
        casus NORMA_INTEGER:
            si (v.habet_intra && v.minimum > (s64)((i64)I << LXIII))
            {
                _addere(m, MUTATIO_MINIMUM, n, valor, chorda_ex_literis("", p), via);
            }
            si (v.habet_intra && v.maximum < (s64)(((i64)I << LXIII) - I))
            {
                _addere(m, MUTATIO_MAXIMUM, n, valor, chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_NUMERUS:
            si (v.habet_intra_fluitans)
            {
                _addere(m, MUTATIO_MINIMUM, n, valor, chorda_ex_literis("", p), via);
                _addere(m, MUTATIO_MAXIMUM, n, valor, chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_TEXTUS:
            si (v.licita)
            {
                si (_intra_longitudinem(v, XVI))
                {
                    _addere(m, MUTATIO_ELECTIO, n, valor, chorda_ex_literis("", p), via);
                }
            }
            alioquin si (_forma_nota(v.forma))
            {
                si (_intra_longitudinem(v, IX))
                {
                    _addere(m, MUTATIO_FORMA, n, valor, chorda_ex_literis("", p), via);
                }
            }
            alioquin si (v.habet_longitudinem)
            {
                si (v.longitudo_minima > 0)
                {
                    _addere(m, MUTATIO_BREVIUS, n, valor, chorda_ex_literis("", p), via);
                }
                _addere(m, MUTATIO_LONGIUS, n, valor, chorda_ex_literis("", p), via);
            }
            frange;
        casus NORMA_TABULATUM:
            si (v.habet_longitudinem)
            {
                si (v.longitudo_minima > 0)
                {
                    _addere(m, MUTATIO_BREVIUS, n, valor, chorda_ex_literis("", p), via);
                }
                _addere(m, MUTATIO_LONGIUS, n, valor, chorda_ex_literis("", p), via);
            }
            per (i = 0; i < json_tabulatum_numerus(valor); i++)
            {
                _colligere(m, v.elementum, json_tabulatum_obtinere(valor, i),
                           _via_index(via, i, p), p);
            }
            frange;
        casus NORMA_OBJECTUM:
            _colligere_objectum(m, n, valor, via, chorda_ex_literis("", p), p);
            frange;
        casus NORMA_DISCRIMEN:
        {
            JsonValor* tag = json_objectum_capere_chorda(valor, v.clavis_discriminis);
            chorda     via_tag = _via_clavis(via, v.clavis_discriminis, p);

            _addere(m, MUTATIO_SINE_TAG, n, valor, v.clavis_discriminis, via_tag);
            si (v.modus == NORMA_CLAUSUM)
            {
                _addere(m, MUTATIO_VARIATIO, n, valor, v.clavis_discriminis, via_tag);
            }
            per (i = 0; tag && i < xar_numerus(v.variationes); i++)
            {
                NormaVariatio* var = (NormaVariatio*)xar_obtinere(v.variationes, i);

                si (chorda_aequalis(var->valor, json_ad_chorda(tag)))
                {
                    _colligere_objectum(m, var->objectum, valor, via,
                                        v.clavis_discriminis, p);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

/* objectum sine clave (API json removendi caret: aedificatur novum) */
interior vacuum
_removere (JsonValor* objectum, chorda clavis, Piscina* p)
{
    JsonValor*           novum = json_objectum_creare(p);
    JsonObjectumIterator it = json_objectum_iterator(objectum);
    chorda               k;
    JsonValor*           f;

    dum (json_objectum_iterator_proxima(&it, &k, &f))
    {
        si (!chorda_aequalis(k, clavis))
        {
            json_objectum_ponere_chorda(novum, k, f);
        }
    }
    *objectum = *novum;
}

interior NormaCausa
_mutare (constans Gignitor* g, Mutatio* x)
{
    Piscina*   p = g->piscina;
    NormaVisus v = norma_visus(x->n);

    commutatio (x->genus)
    {
        casus MUTATIO_GENUS:
            *x->valor = v.genus == NORMA_TEXTUS ? *json_integer_creare(p, VII)
                                                : *json_chorda_creare_literis(p, "x");
            redde NORMA_CAUSA_GENUS;
        casus MUTATIO_DEEST:
            _removere(x->valor, x->clavis, p);
            redde NORMA_CAUSA_DEEST;
        casus MUTATIO_EXTRA:
            json_objectum_ponere_chorda(x->valor, x->clavis, json_integer_creare(p, I));
            redde NORMA_CAUSA_EXTRA;
        casus MUTATIO_MINIMUM:
            *x->valor = v.genus == NORMA_INTEGER
                ? *json_integer_creare(p, v.minimum - I)
                : *json_fluitans_creare(p, v.minimum_fluitans - 1.0);
            redde NORMA_CAUSA_MINIMUM;
        casus MUTATIO_MAXIMUM:
            *x->valor = v.genus == NORMA_INTEGER
                ? *json_integer_creare(p, v.maximum + I)
                : *json_fluitans_creare(p, v.maximum_fluitans + 1.0);
            redde NORMA_CAUSA_MAXIMUM;
        casus MUTATIO_BREVIUS:
        casus MUTATIO_LONGIUS:
        {
            i32 meta = x->genus == MUTATIO_BREVIUS ? v.longitudo_minima - I
                                                   : v.longitudo_maxima + I;

            si (v.genus == NORMA_TEXTUS)
            {
                *x->valor = *json_chorda_creare(p,
                    _runis_implere(json_ad_chorda(x->valor), meta, p));
            }
            alioquin
            {
                JsonValor* arr = json_tabulatum_creare(p);
                i32        i;

                per (i = 0; i < meta; i++)
                {
                    json_tabulatum_addere(arr, i < json_tabulatum_numerus(x->valor)
                        ? json_tabulatum_obtinere(x->valor, i)
                        : _gignere(g, v.elementum, _via_index(x->via, i, p)));
                }
                *x->valor = *arr;
            }
            redde NORMA_CAUSA_LONGITUDO;
        }
        casus MUTATIO_ELECTIO:
            *x->valor = *json_chorda_creare_literis(p, "extra_electionem");
            redde NORMA_CAUSA_ELECTIO;
        casus MUTATIO_FORMA:
            *x->valor = *json_chorda_creare_literis(p, "non forma");
            redde NORMA_CAUSA_FORMA;
        casus MUTATIO_VARIATIO:
            json_objectum_ponere_chorda(x->valor, x->clavis,
                                        json_chorda_creare_literis(p, "variatio_ignota"));
            redde NORMA_CAUSA_VARIATIO;
        casus MUTATIO_SINE_TAG:
            _removere(x->valor, x->clavis, p);
            redde NORMA_CAUSA_DISCRIMEN;
        ordinarius:
            redde NORMA_CAUSA_GENUS;
    }
}

NormaGenitum
norma_gignere (constans Norma* n, NormaModusGignendi modus, s64 semen,
               Piscina* piscina)
{
    NormaGenitum r;
    Gignitor     g;
    chorda       radix_via = chorda_ex_literis("$", piscina);

    memset(&r, 0, magnitudo(r));
    r.via_fracta = chorda_ex_literis("", piscina);
    g.piscina = piscina;
    g.modus   = modus == NORMA_FINES ? NORMA_FINES : NORMA_TYPICA;
    sors_seminare(&g.radix, (i64)semen, 0);
    r.valor = _gignere(&g, n, radix_via);
    si (modus == NORMA_INVALIDA)
    {
        Xar*     m = xar_creare(piscina, (i32)magnitudo(Mutatio));
        Sors     s = _sors_viae(&g, chorda_ex_literis("INVALIDA", piscina));
        Mutatio* x;

        _colligere(m, n, r.valor, radix_via, piscina);
        si (xar_numerus(m) == 0)
        {
            r.valor = NIHIL;
            redde r;
        }
        x = (Mutatio*)xar_obtinere(m, sors_intra(&s, xar_numerus(m)));
        r.causa_fracta = _mutare(&g, x);
        r.via_fracta   = x->via;
    }
    redde r;
}
```

  Known risk to rule on at first run (the oracle names it): a mutation at
  the ROOT replaces `*r.valor` in place - fine; `_removere` copies a new
  object over the old struct (`*objectum = *novum`) - relies on JsonValor
  being a plain value struct (json.h:81-92: it is).

- [ ] **N4.4** register, green; print the oracle's per-schema counts.
- [ ] **N4.5 plants** - (1) per-path seeding replaced by the radix stream
  for every node -> stability red; (2) BREVIUS/LONGIUS allowed on texts
  with a known forma -> "exactly one" red (uuid lengthened breaks two
  rules); (3) FINES float maximum emitted as `max + 1e-9` -> FINES
  validity red; (4) GENUS mutation emits null for an aut_nullum node ->
  INVALIDA red (zero issues).
- [ ] **N4.6** worklog + commit; gates `radix`, `generata`.

---

### Task N5: vates' novelty = a declared NOTANDUM schema

**Files:** Modify `lib/vates.c` (part B), `lib/vates.worklog.md`. NO header
change; NO test change (T4, T5, T6, T7b tests are the regression oracle
and must pass UNCHANGED).

- [ ] **N5.1** In `lib/vates.c`: `#include "norma.h"`; delete
  `_claves_summae`, `_claves_textus`, `_claves_petiti`, `_in_indice`,
  `_claves_probare` and their calls in `_legere`; add:

```c
interior Norma*
_forma_responsi (Piscina* p)
{
    Norma* textus = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* petitum = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* cogitatio = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* redacta = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* blocus = norma_modus(norma_discrimen(p, "type"), NORMA_NOTANDUM);
    Norma* cc = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* usus = norma_modus(norma_objectum(p), NORMA_NOTANDUM);
    Norma* r = norma_modus(norma_objectum(p), NORMA_NOTANDUM);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(textus, "citations", norma_liberum(p), FALSUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_liberum(p), VERUM);
    norma_campus(cogitatio, "thinking", norma_textus(p), VERUM);
    norma_campus(cogitatio, "signature", norma_textus(p), VERUM);
    norma_campus(redacta, "data", norma_textus(p), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_variatio(blocus, "thinking", cogitatio);
    norma_variatio(blocus, "redacted_thinking", redacta);
    norma_campus(cc, "ephemeral_5m_input_tokens", norma_integer(p), FALSUM);
    norma_campus(cc, "ephemeral_1h_input_tokens", norma_integer(p), FALSUM);
    norma_campus(usus, "input_tokens", norma_integer(p), VERUM);
    norma_campus(usus, "output_tokens", norma_integer(p), VERUM);
    norma_campus(usus, "cache_read_input_tokens", norma_integer(p), FALSUM);
    norma_campus(usus, "cache_creation_input_tokens", norma_integer(p), FALSUM);
    norma_campus(usus, "cache_creation", cc, FALSUM);
    norma_campus(usus, "output_tokens_details", norma_liberum(p), FALSUM);
    norma_campus(usus, "service_tier", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(usus, "inference_geo", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "type", norma_textus(p), VERUM);
    norma_campus(r, "role", norma_textus(p), VERUM);
    norma_campus(r, "model", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_aut_nullum(norma_textus(p)), VERUM);
    norma_campus(r, "stop_sequence", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(r, "stop_details", norma_liberum(p), FALSUM);
    norma_campus(r, "usage", usus, VERUM);
    norma_campus(r, "container", norma_liberum(p), FALSUM);
    norma_campus(r, "diagnostics", norma_liberum(p), FALSUM);
    redde r;
}

/* "$.content[N].type" -> typus blocu N (nota VARIATIO); aliter via ipsa */
interior chorda
_typum_ex_via (JsonValor* radix, chorda via, Piscina* p)
{
    constans character* praefixum = "$.content[";
    i32                 lp = (i32)strlen(praefixum);
    i32                 index = 0;
    i32                 i;
    JsonValor*          content;
    JsonValor*          blocus;

    (vacuum)p;
    si (via.mensura <= lp || memcmp(via.datum, praefixum, (size_t)lp) != 0)
    {
        redde via;
    }
    per (i = lp; i < via.mensura && via.datum[i] >= '0' && via.datum[i] <= '9'; i++)
    {
        index = index * X + (i32)(via.datum[i] - '0');
    }
    content = json_objectum_capere(radix, "content");
    blocus  = content ? json_tabulatum_obtinere(content, index) : NIHIL;
    redde json_capere_chorda(blocus, "type", via);
}

/* novitas = notae (et vitia) normae, OMNES, '; ' iunctae. Nota VARIATIO
 * in content -> "blocus ignotus: <type>" (forma veterum probationum). */
interior vacuum
_novitates_ex_norma (JsonValor* radix, chorda* novitas, Piscina* p)
{
    NormaIudicium      j = norma_iudicare(_forma_responsi(p), radix, p);
    ChordaAedificator* aed = chorda_aedificator_creare(p, CCLVI);
    i32                i;

    si (novitas->mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, *novitas);
    }
    per (i = 0; i < xar_numerus(j.notae) + xar_numerus(j.vitia); i++)
    {
        b32          nota = i < xar_numerus(j.notae);
        NormaVitium* v = (NormaVitium*)xar_obtinere(nota ? j.notae : j.vitia,
                             nota ? i : i - xar_numerus(j.notae));

        si (chorda_aedificator_longitudo(aed) > 0)
        {
            chorda_aedificator_appendere_literis(aed, "; ");
        }
        si (nota && v->causa == NORMA_CAUSA_VARIATIO)
        {
            chorda_aedificator_appendere_literis(aed, "blocus ignotus: ");
            chorda_aedificator_appendere_chorda(aed, _typum_ex_via(radix, v->via, p));
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed,
                nota ? "campus ignotus: " : "forma fracta: ");
            chorda_aedificator_appendere_chorda(aed, v->via);
        }
    }
    *novitas = chorda_aedificator_finire(aed);
}
```

  In `_legere`: keep `_causa_finis`'s
  stop_reason novelty (it seeds `*novitas`), drop the per-block
  `_novitas` calls, and call `_novitates_ex_norma(j.radix, novitas, p)`
  once after the content loop.

- [ ] **N5.2** Run `./compile_tests.sh probatio_vates` - all 106 green
  UNCHANGED (probatio_novitas_pressa still sees exactly
  "blocus ignotus: server_tool_use"; probatio_forma_viva still zero).
- [ ] **N5.3 plants** - (1) drop `diagnostics` from `_forma_responsi` ->
  probatio_forma_viva red (the live body has it: a note, a press);
  (2) the content blocks' discrimen declared APERTUM -> probatio_novitas_
  pressa red (an unknown block no longer noted). Restore + `cmp`.
- [ ] **N5.4** Live check: `./tools/vates_fumus.sh` again (cents);
  herbarium presses NOTHING for ordinary calls now.
- [ ] **N5.5** worklog (old key lists retired; all novelties reported) +
  commit; gates `radix`, `generata`.

---

## After plan 2

T8 (merge quarta -> main) carries vates, herbarium, norma, fictio and
the http/tls/filum changes together; frigida first, Fran before the ff.
