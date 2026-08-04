# natura worklog — the transformation record

This worklog is load-bearing differently than the code worklogs:
per the Alexander method, the artifact grows by structure-
preserving transformations, and THIS is where each transformation
and its reason is recorded. The models say what is true; this
file says how we came to say it.

## 2026-08-04 — birth: lingua_programmandi v0 → v1 in one day

Founding decisions (conversation with Fran, on the ledger res
01KZ6TNNAM):
- Discipline is PIEPER: describe things as they are, not as apps
  want them — properties recorded whether useful or not. A
  use-case schema is flattery and breaks at use case N+1; truth
  doesn't rot.
- Shape is LINNAEUS: taxonomy + dictionary in one artifact —
  the form of the genus AND the described species/individuals.
- Process is ALEXANDER: hand-crafted, discussed, format learned
  from content, plastic for a long time. Not mechanical.
- Latin is load-bearing here, not decorative: a semantic library
  is the universal-language application (Linnaeus's own reason).
- Rule of residence: the library holds what is timelessly
  describable (kinds at every rank + notable individuals of
  public record); YOUR individuals live in app volumina and cite
  the library.

v0 → v1 transformations (each forced by content, none planned):
1. "compiled/interpreted" DIED as a language property within the
   first hour — it belongs to implementations (officina interprets
   C). The commonest schema lie in this domain, caught by the
   discipline. → genus implementatio_linguae, where
   modus_exsecutionis truthfully lives, plus
   tutela_memoriae_addita (Fil-C: implementation-granted safety
   the language never promised).
2. Rust bent the gestio_memoriae enum mid-draft: per_possessionem
   is a third thing, neither manual nor collected. Recorded in a
   comment at the option itself.
3. The c89 dubium (varietas or edition?) RESOLVED by Fran's
   versio_linguae genus: an edition is an EVENT crystallized into
   a citable thing — the exact shape of a conditio in the volumen
   (named point in the log). c89/c99/c11 are individua of
   versio_linguae, chained by 'sequitur'.
4. auctores-as-textus died: Dennis Ritchie is a persona, not a
   string. → natura/persona.stml (Boethius' definitio for the
   genus), with the format's species/individuum distinction made
   EXPLICIT: C is a species; Ritchie and c89 are individua.
5. normae-as-textus died the same death → natura/norma.stml:
   norma has an editor, a date, a state machine
   (proposita/vigens/substituta/retracta — old standards don't
   vanish, C89 is our witness), and 'substituit' chains.
6. HISTORICAL LAYER (Fran's pull, shape found): a thing's history
   is its event log — <historia> holds dated <eventum> entries;
   current status is a FOLD of the historia. The storage-layer
   doctrine (acta = truth, state = projection) recurring one
   level up, unforced. annus_ortus migrated from property to
   first historia event.

Open dubia carried IN the artifacts (deliberately — honest
descriptions carry their unresolved tensions): bash's genus
(lingua_iussuum? a wider lingua_formalis?), organizatio surfacing
from norma.editor and persona (ISO is not a persona), POSIX's
continuous-renewal identity (same puzzle as language versions —
the versio solution may generalize).

Format so far: <natura modulus> → <genus> → <definitio> (dictionary
prose), <proprietates> (typed via the genera palette, options
inline), <machina_statuum> (+ named transitus), <actiones>,
<relationes> (modulus= for cross-model, externum= for
not-yet-described, ad="*" for open relations), then <species> and
<individuum> with <valor>/<relatum>/<historia>/<dubium>/<nota>.
Everything provisional; the next models (plagula_computatralis,
systema_operativum) will bend it again.

## 2026-08-04 — round two: depth (sub-genera), organizatio, architectura

Fran's pulls, each landed:

7. SUB-GENERA arrive (format: `sub=` attribute on genus +
   <differentia> at genus level). The Porphyrian tree gets depth,
   and it RESOLVED the bash dubium the scholastic way:
   subordination, not sisterhood — lingua_iussuum sub
   lingua_programmandi. The tell that subordination was right:
   bash's PROPERTY VALUES all fit the parent's palette (that's
   inheritance working), only the essence differed (that's what
   the differentia carries). Genus + differentia doing their
   ancient jobs unchanged.
8. organizatio born (natura/organizatio.stml): universitas
   personarum — the definitio hangs on PERDURATIO (persists
   through member change; what distinguishes it from a mere
   coetus). Sub-genera prove themselves immediately:
   corpus_normans (iso/iec/ieee/ansi) and societas
   (intel/amd/arm_ltd/bell_labs). norma.editor stops being a
   conscious lie (textus → relation); persona gains membrum_de;
   both dubia closed in place.
9. architectura_machinae + versio_architecturae
   (natura/architectura_machinae.stml): the language/
   implementation distinction recurring in iron — architecture
   is FORM (the chip genus surfaces, undescribed). THE SLIDING
   LAW gets its third case and a name: word width belongs to the
   VERSION, not the family (x86 crossed 16/32/64) — proprietates
   ad subiectum verum labuntur. Three cases now: compiled→
   implementation, annus_ortus→historia, word-width→version.
   Dictionary honesty paying off: x86_64's auctor is AMD, not
   Intel — version author ≠ family author, which a flat schema
   could never say.

Genera surfaced and waiting: chip (microarchitectura), att
(from bell_labs pars_de). The versio-solution generalizing
(lingua, architectura, probably norma/POSIX next) suggests versio
may eventually be a FORM — a genus-shape any model can stamp —
but three instances is not yet a law; let a fourth force it.

## 2026-08-04 — round three: plagula_computatralis (the parish model)

Fran's ruling recorded first: versio genera stay hand-coined per
model — legitimate entities, no special mechanism until reality
begs (the fourth knock never obligates, it only invites).

10. plagula_computatralis: the deepest tree yet — genus → five
    sub-genera (fontis / scriptum_exsecutabile / notationis /
    figurationis / datorum) → sub-sub (sqlite) → species. The
    four-level chain bottoms out at species VOLUMEN: the deepest
    entry in the dictionary is our own thing — the parish
    described all the way down. STML itself is in there too
    ("hoc ipsum documentum species haec est").
11. NEW MECHANISM forced by content: a sub-genus can FIX an
    inherited property (<valor> at genus level — plagula_fontis
    pins forma_contenti=textualis for everything beneath).
    Scholastic shape: the proprium of the subordinate genus.
12. The lingua model's v0 promise came home: fons_scriptus_in
    (externum since the first hour) is now implemented by
    plagula_fontis.scripta_in (+ scripta_in_versione — "this is
    c89," which this house says daily).
13. Honesty entries: suffixes and MIME types marked CONVENTIO
    non essentia (a C file without .c remains a C file); file
    identity persists through content change — the same
    perduratio as organizatio, and the same distinction the
    volumen already drew (named plagula vs content-addressed
    massa). Our semantics knew our storage before we wrote it
    down.
14. FIRST CROSS-CUTTING TENSION, carried openly: the sub-genera
    are not exclusive — a .py with a shebang is both fons and
    scriptum. Porphyry wants exclusive division; reality laughs.
    The dubium proposes executability may be ACCIDENS not
    essentia (+x is a mode, the crena is content) — but the
    scriptum sub-genus stays (Fran named it; the use is real)
    with the tension recorded. This is the classic
    multiple-inheritance fight arriving on schedule; we hold it
    as a dubium rather than solving it prematurely.

Surfaced and waiting: directorium (is it itself a plagula? Unix
almost says yes), chip, att. Parish remaining: systema_operativum.

## 2026-08-04 — round four: the multiplicity ruling

15. Cross-cutting RESOLVED by Fran's ruling: entities may be
    multiple things. The adoption revealed a pleasing structure —
    THREE distinct multiplicities, two of which needed nothing:
    (a) ROLES are RELATIONS, already: "author"/"inventor" were
        never genera in these models — a person is an author by
        standing in auctor relations. Fran's own example was
        solved before it was asked. Munera relationes sunt.
    (b) SPECIES-essential overlap → new attribute etiam= on
        species: scriptum_conchae etiam="plagula_fontis" (every
        shell script IS source; membership in both is essence,
        not accident). First and so far only use.
    (c) INDIVIDUAL-accidental overlap (.py with a shebang) →
        no format change: individua are classified where they
        live (app volumina cite multiple species). Doctrine,
        not mechanism.
    The Porphyrian tree stays primary (single sub= spine);
    etiam= is the honest crosslink, expected to stay rare — if
    it proliferates, that's a smell that a differentia is
    misplaced, and the worklog should catch it.

## 2026-08-04 — round five: systema_operativum (parish complete)

16. The state-machine BEARER question arrived and got its
    mechanism: machina_statuum gradus="species|individuum". The
    species macOS never sleeps; an INSTALLATION does. "vita" is
    borne by species (DOS is derelictum); "cursus"
    (exstinctum/initians/currens/dormiens, with CASUS as an
    honest unwilled transition) is borne by individua — which
    live entirely OUTSIDE the library, in app volumina (villa
    will cite these states for its servers). The
    describe-vs-bind doctrine made concrete: the library
    declares a machine whose every instance is elsewhere.
17. TRUTH-CATCH, genealogy edition: "Linux is Unix" is the
    common schema lie. macOS ORTA EX unix (real BSD descent);
    Linux AD_EXEMPLUM_FACTA unix (reimplementation, no line of
    code descends). Two relations where flat schemas have one
    boolean. Windows gets the same honesty via vms (surfaced,
    undescribed): NT is VMS-shaped, not Unix-shaped.
18. Dictionary grew its demanded residents: ken_thompson (Unix,
    B), linus_torvalds (Linux kernel, git) in persona; apple,
    microsoft in societas. Unix species notes its life
    continues in descendants while the AT&T line itself
    senesces — species whose vita lives in its children.
19. versio_systematis coined per the ruling (no mechanism);
    ventura→sonoma chained, sonoma noted as what this house
    runs (Darwin 23.6). New dubium carried: Linux
    DISTRIBUTIONS — kernel vs system vs distribution is the
    granularity question knocking again (species? version? a
    genus between?). Left open.

Parish of three complete: linguae, plagulae, systemata. Surfaced
and waiting: directorium, chip, att, vms, systema_plagularum
(APFS/ext4). Seven models, ~1500 lines, nineteen transformations
in one day — none of the mechanisms designed in advance, every
one forced by content. The next natural conversations: Fran's
editorial pass over the seven as a set, or the loader (where
plasticity starts trading against consumers).

## 2026-08-04 — round six: distributio_systematis

20. The distribution dubium resolved by Fran's ruling:
    distributio is a GENUS in its own right — "certainly a type
    of thing people have to deal with" — and deliberately
    GENERIC (distributio_systematis, not distributio_linux):
    Linux made the pattern famous, but the form isn't
    Linux-shaped. Definitio hangs on CURATION: the system is
    form, the distribution is the livable edition of the form —
    what you actually install. Dictionary: debian (with the
    written social contract noted), ubuntu (orta_ex debian —
    distribution genealogy is real descent), arch (whose
    differentia IS its rolling cadence — the volvens option
    earning its place in the enum the same day it was coined).
    canonical joins societas; debian_project surfaces the
    COMMUNITAS sub-genus of organizatio (purpose without
    profit, open works) — surfaced, undescribed, waiting.

## 2026-08-04 — round seven: THE FOREIGN STRESS TEST (books + plants)

Deliberately outside the parish, on the reasoning that every
mechanism so far was forced by describing SOFTWARE — a format that
has only described its own domain may be secretly overfit to it.
Two models, chosen so the second cites the first (real practice:
species names are published in works).

### opus_scriptum — and the format's first real INADEQUACY

21. OUR VERSIO PATTERN WAS TOO SIMPLE. Library science spent a
    century here and landed on four levels (FRBR: Work /
    Expression / Manifestation / Item) where we had one. Not our
    failure — the foreign domain's GIFT: book identity genuinely
    needs four, and the distinctions CARRY MEANING. Witness in the
    dictionary: the 1851 London edition of Moby-Dick omitted the
    epilogue, so Ishmael could not have survived to narrate —
    British reviewers said so. A manifestation changed the
    meaning of the work. A schema that doesn't distinguish
    editions cannot say that sentence.
    → genera opus_scriptum / expressio / editio / codex.
22. RESIDENCE RULE VALIDATED BY A FOREIGN DOMAIN: FRBR's cut
    falls exactly where our rule already cut — work/expression/
    edition are timelessly describable (library); the CODEX on
    your shelf, with its marginalia and lending state, is yours
    (volumen). The rule didn't bend; it agreed. And
    notae_marginales is a property NO other level can bear —
    proof the fourth level is real.
23. SLIDING LAW, CASE FOUR, IN A FOREIGN DOMAIN: page count
    belongs to the edition, not the work. The law is therefore
    about THINGS, not about software. Same round killed the
    domain's flagship lie: ISBN identifies a MANIFESTATION, not a
    work — catalogs that treat ISBN as book-identity are counting
    editions and calling them books.
24. imperfecta_relicta became a status because the Summa demands
    it (Aquinas stopped: "omnia quae scripsi videntur mihi
    palea"). A composition machine without "left unfinished"
    lies about the history of great books.
25. Self-reference, honestly earned: the dictionary now contains
    Pieper's Abusus Linguae (the book that gave this library its
    discipline, with its German original and English translation
    as two EXPRESSIONS — the translation test) and Linnaeus's
    Systema Naturae + Species Plantarum (the form's father; and
    the 10th edition is FORMALLY the start of zoological
    nomenclature — law citing an edition, not a work).
26. periodicum surfaced the first INHERITED-MACHINE MISFIT: a
    serial never reaches "perfecta". Dubium open — is differentia
    enough, or does a sub-genus need to override a parent's
    machine? Carried, unresolved.

### planta — the maximally foreign model

27. PARTES: composition arrived (radix/caulis/folium/flos/
    fructus/semen, each necessaria=verum|falsum — flos is
    honestly optional: ferns and conifers have none). We had
    relations but never part-whole composition of the described
    kind. Software never asked; a corporeal thing asked
    immediately. Aristotle's partes essentiales.
28. gradus= on genus names the RANK in domain terms (familia /
    genus / species / cultivar) — biology names its ranks because
    its LAW names them. Our sub= spine already carried depth;
    this labels it. Forced a discipline transformation: the
    state-machine attribute gradus= was RENAMED gerens= (bearer)
    — one attribute name cannot mean two things in a library
    about precise naming, and "bearer" was the more accurate word
    anyway. systema_operativum updated.
29. CYCLES in state machines: quiescens → florens per ver. A
    perennial flowers again every year; our earlier machines were
    linear or terminal. Same machine, two paths (annual dies
    after fruiting, perennial cycles) discriminated by the
    'durata' property — a nice economy the domain handed us.
30. CROSS-DOMAIN CITATION between two FOREIGN models: species
    cite Species Plantarum (1753) as the publication of their
    names, with auctor_nominis → persona ("Rosa canina L." — the
    author abbreviation is PART OF THE NAME). Biological
    nomenclature carries provenance inside the name itself; a
    standing rebuke to our undescribed <fons> gap, and a model
    for it.
31. New dubium from life itself: living properties are RANGES,
    not points (height 2-5m, flowering May-June). Our format
    holds single values. Software never needed intervals; a rose
    needs one immediately. Open.
32. rosaceae carries the note that makes taxonomy worth having:
    rose, apple, pear, plum, strawberry are kin — a truth common
    sense would deny and only a real taxonomy makes visible.

VERDICT ON THE STRESS TEST: the format bent in three places
(four-level identity, composition, cycles) and held everywhere
else; the residence rule was CONFIRMED by an alien domain rather
than strained; and the sliding law replicated outside software.
Overfit risk was real and is now measured, not assumed.
