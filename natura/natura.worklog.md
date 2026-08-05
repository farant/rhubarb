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

## 2026-08-04 — round eight: elementa_programmatis (the descent)

New STRATUM, from a brainstorm with Fran on future applications:
the models so far described ARTIFACTS (a language, an OS, a file);
this one descends INTO the program, to its elements. Fran's
framing, which became the stratum's founding doctrine: the
interesting layer is language-ABSTRACT — "what a program actually
is vs the specific language implementation."

33. THE INTENTION DOCTRINE (Fran's ruling, the stratum's
    foundation): the natura kind is determined by the author's
    INTENTION; the language construct is only the realization —
    forma/materia, mapping many-to-many. Witness in both
    directions: one intention (constans) wears four C89 garments
    (macro / enumerator / const object / bare discipline); one
    garment (`const`) spells two intentions (true constancy vs a
    read-only covenant on an access path). Consequence: this
    stratum is NOT an annotated AST — the AST says what was
    written, natura says what the thing IS, and the description
    survives porting. This is also the vocabulary layer the
    CLAUDE.md "extra type system" (annotation vision, parked
    01KXTX7FA3) has been waiting for: an intention declaration
    turns a fluxus-provable fact into a JUDGMENT.
34. BEARER doctrine generalized DOWN a level: genus status with
    geritur_a (open relation — bearers cross models). durabilitas
    recorded as a DERIVED property: it follows the bearer's life
    (ephemera/persistens are the two regions, process-death the
    border). SLIDING LAW CASE FIVE: durability slides to the
    bearer. gerens= was born for state machines (round five);
    the same cut now organizes state itself.
35. EFFECTS UNIFIED BY BEARER: every effectus = a mutation of
    someone's status; species divide by WHAT is mutated
    (mutatio_memoriae / scriptio_externa / allocatio /
    terminatio), not how. First genus of EVENTS rather than
    substances. Plus the apertus/occultus cut: memcpy mutates
    openly (through a parameter, visible), strtok covertly —
    impure-but-honest vs the betrayal.
36. PURITY HAS TWO AXES — the stratum's flagship truth-catch:
    "no side effects" names only hidden OUTPUTS; hidden INPUTS
    (reading state not passed as a parameter) break purity too.
    rand is the perfect one-function lesson: reads AND writes
    its hidden seed. functio_pura is a sub-genus (v0 dares it;
    dubium carried — the argument is that the differentia buys a
    BUNDLE of licenses: memoize, reorder, parallelize, test
    without fixtures; a differentia that begets faculties marks
    essence).
37. VERITAS/PROIECTIO enters the library: proiectio sub status,
    differentia = recomputable from its veritas, hence LICENTIA
    DELENDI — the operationally gravest property no syntax
    carries. The house lived this doctrine in three places
    (tabularium .db, silva caches, silex working tree) before it
    was written here. Species: artefactum_aedificationis ("make
    clean" is innocent BECAUSE projection), promptuarium,
    index_derivatus.
38. Surfaced, undescribed: TYPUS (functio.accipit/reddit),
    CONSTRUCTUM_LINGUAE (the material side of the intention
    doctrine), PROCESSUS (bearer of all ephemeral state —
    programma vs processus is form vs instance, again). The
    dictionary as BESTIARY emerges: libc individua chosen because
    each TEACHES a truth of the stratum (printf, memcpy, strtok,
    rand, malloc, abs, strlen).
39. libc forced the form/implementation recursion AGAIN: libc is
    form; glibc/musl/msvcrt implement it. Carried as dubium
    (implementatio_bibliothecae — born only when reality begs).
    sqlite's EMPTY dependet_ex recorded as itself notable.
    bibliotheca's dictionary is the first designed to be
    HARVESTED, not typed (aedilis measures dependet_ex today).

Naming: functio kept with the abuse-of-language note in place
(Pascal distinguished functio/procedura; C flattened it — the
Pieper catch is in the genus's own name); reddit_valorem falsum =
procedura by its old name. "modulus" avoided for the library
genus (taken by natura's own metamodel) → bibliotheca. DSL
deliberately deferred to lingua_programmandi (differentia
universalis/specialis, inserta/externa).

## 2026-08-04 — round nine: the twins kill effectus (v0→v1, same day)

Fran, off the top of his head: "state mutation" and "state
query/read." Two words; a genus died.

40. GEMINI: lectio + mutatio born as genera — the fundamental
    EVENTS of a status's life (written by mutation, made
    fruitful by reading). mutatio's definitio names the deep
    house fact: a mutation is exactly what an ACTUM records —
    event sourcing = the doctrine that mutations deserve
    reification, and the volumen has practiced it all along.
    lectio's dual fact: reads leave no trace unless custodia
    deliberately records them (audit domains reify reads too).
41. EFFECTUS DIED AS A GENUS one round after its birth — v0's
    own unification sentence ("omnis effectus = mutatio status
    cuiusdam gerentis") was the confession, unread. And
    effect-hood is BOUNDARY-RELATIVE: what memcpy does openly,
    a caller of memcpy on a global does covertly — same
    mutation, different verdict per declared interface. A
    position, not an essence. The multiplicity ruling recurring
    for events: "author" is not a kind of person, "side effect"
    is not a kind of mutation. Munera relationes sunt, in
    eventibus quoque. Species migrated to mutatio; "effectus
    secundarius" survives as explained vocabulary in mutatio's
    definitio. The declaratio (apertus/occultus) property
    survives WITH its relativity confessed in the nota.
42. PURITY UNIFIED: v0's "two axes" become ONE criterion (the
    declared channel) applied to the two twins — lectiones only
    through parameters, mutationes not at all. rand remains the
    one-function lesson (breaks both).
43. mutatio grew the two properties that matter for the house's
    future: idempotens (replay safety — refolding acta) and
    commutativa (order-independence — the G8 replica-merge
    conversation, arriving as a property of mutation kinds).
    transactio joined as species: the atomic compound mutation
    (K1 law: plagulam condere = one transaction).
44. Meyer's command-query separation named in the section
    comment as the discipline of the twins ("asking must not
    change the answer") with the house witnesses: tabularii
    quaerere/gerere, SQL SELECT/UPDATE. The cut recurs
    everywhere because it is real. scanf joined the bestiary as
    printf's twin (lectio externa + open memory mutation in one
    function).

Method note: this is the fastest genus-death yet (hours), and it
came from Fran REIFYING two relations I had left as mere verbs.
Lesson for the craft: when a relation's name keeps needing notas
(legit/mutat carried the whole purity doctrine in annotations),
the relation may be begging to become a thing.

## 2026-08-04 — round ten: promotions and divisions (v2 + lingua v3)

Four pulls from Fran, all landed:

45. constans DIVIDED: constans_numerica / constans_textualis.
    Division deliberately NON-exhaustive — null (constans
    monstratoris) stays under the parent genus as the honest
    witness, until a third species is begged for. numerica's
    differentia carries the domain's great abuse: the MAGIC
    NUMBER is a constant whose intention went undeclared ("a
    naked number does not carry its own intention"). textualis
    carries the C89 catch that this house has always compiled
    against: string literals are NOT const-typed (char*, not
    const char*) — writing them is UB but the type system is
    silent; -Wwrite-strings closes the gap between intention
    and type. Fran's flag set was intention-doctrine praxis
    years before the doctrine had a name.
46. proiectio's three species PROMOTED to sub-genera (Fran:
    "should be their own genera") — and the craft law from
    round nine confirmed immediately: each brought its own
    machinery the moment it had room. artefactum_aedificationis
    got relations (ex_fontibus; per_instrumentum surfacing
    INSTRUMENTUM_AEDIFICATIONIS — make, aedilis) and species
    (obiectum_compilatum / exsecutabile_ligatum). promptuarium
    got the model's first MACHINA STATUUM (fides:
    congruens/rancidum, gerens=individuum) whose key transitus
    carries the whole famous difficulty: mutatio_veritatis is
    INVISIBLE to the cache itself — nobody tells it the truth
    changed (Karlton's two hard things quoted; both live in
    this library). index got index_inversus / index_arboris.
    A species that accumulates properties is begging to be a
    genus — now measured twice in one day.
47. status grew CONGRUENTIA (statim/tandem) beside durabilitas,
    same shape: an electio whose value is not free — single
    bearer = statim by nature; tandem ARISES when plural
    bearers carry replicas of one status. The seed of
    replication (G8) recorded as a property of state, written
    long before the mechanism exists.
48. verbum_clavis born in lingua_programmandi (v3): the
    language's OWN vocabulary, denied to identifiers — the
    exact boundary latina.h lives on (every forbidden
    identifier is a verbum_clavis wearing a Latin name).
    additum_in → versio_linguae: keywords slide to the VERSION
    (restrict arrived in C99, which is why this house lives
    without it) — the sliding law reaching vocabulary.
    Bestiary: static (one word, three meanings — the house's
    interior/hic_manens/universalis as the published proof),
    const (two intentions, cross-model reference to
    elementa_programmatis), goto (Dijkstra 1968 — Dijkstra
    surfaces, persona not yet described), restrict (the
    version-slide witness). Keyword vs reserved-word noted as
    itself a real distinction (contextual keywords exist).

Surfaced this round: instrumentum_aedificationis, dijkstra
(persona). Next: Fran proposes reading actor.c against the
stratum — the first deliberate confrontation of the model with
a real source file, looking for genera the code demands.

## 2026-08-04 — round eleven: the actor.c harvest (12 models)

The confrontation paid: actor.c was read whole, discussed, and
the discussion became elementa v3 + two NEW models + persona v4.
Two validations logged first, because they are the method
working: (a) capsa_tollere/inspicere is EXACTLY the destructive-
read nota lectio carried hours before the code was read;
(b) the actor model is the canalis-declaratus criterion at
entity scale — capsa : actor :: parametra : functio. The purity
criterion is SCALE-INVARIANT; that promotes it from observation
toward law.

49. assertio (elementa v3): the executable claim — a
    proposition carrying its own oracle; "documentation that
    cannot lie for long: when false, it shouts." Species:
    praeconditio / postconditio / invarians (Meyer's contract
    trio as formal interface promises), assertio_statica,
    assertio_probationis (credo is the house realization).
    Sister-of-annotation doctrine recorded: assertion checked
    by the program, annotation by external tools — both
    propositions about code.
50. commentarium (elementa v3) with the definitio found in
    discussion: THE PART OF THE SOURCE THE LANGUAGE PROMISES
    NOT TO READ. Species divide by addressee and tense:
    explicativum (why, to humans), documentarium (contract),
    monitum_futuri (TODO — named debt where it is owed),
    titulus_iuris, codex_sepultus (commented-out code = a
    CATEGORY LIE — code wearing commentary as a shroud; where
    version control exists the grave is superfluous),
    annotatio_machinae (the ignored channel repurposed to
    address machines — the house's STML/TOLERA stratum; silva
    preserving comments byte-for-byte is why the sensor
    exists).
51. Deprecated/dead landed as DECLARED vs DISCOVERED (Fran's
    pull, the cut found in discussion): functio grew machina
    vita (vigens→deprecata→remota, gerens=individuum —
    deprecation is a SPEECH ACT with substituit chains like
    norma) and the ORTHOGONAL proprietas mortua (a provable
    FACT of the graphs — vocantes/fluxus measure it). The 2x2
    is real: deprecata-sed-vocata and mortua-numquam-deprecata
    both exist. Same machine noted as fitting bibliotheca —
    deferred until pulled.
52. identificator divided along Scotus's line (Fran associated
    id with haecceity; the association cracked the genus):
    identificator_baptismalis prensat haecceitatem (assigned —
    THIS one, even among perfect twins); sigillum_contenti
    prensat FORMAM — thisness deliberately discarded, and that
    discarding is WHY deduplication works. The volumen split
    its storage along this line (massae/plagulae) before the
    vocabulary existed.
53. communicatio.stml born (model 11): actor / nuntius /
    eventum — genera LOGICA (Fran's word does the intention-
    doctrine's work). nuntius = reified ADDRESSED communication
    (a request made into a thing; machina cursus whose
    consumptio names the destructive read); eventum = the
    UNADDRESSED happening (ad aliquem vs coram omnibus); and
    the metamodel's own <eventum> element recognized as this
    genus — true cognate, unlike the status homonym.
    disciplina_schematis records the house's two living
    realizations: actor.c sine schemate, nuntium.h cum
    schemate.
54. formae_datorum.stml born (model 12): the purest Linnaean
    material yet — mathematical forms defined by OPERATIONS,
    before all languages. Six species with house realizations
    named: series (xar — segmentation invisible to the form),
    cauda (capsa), acervus (the CALL STACK: a form nobody
    creates and everyone uses, realized by the language
    itself), tabula_associativa (the house NAME tabula_dispersa
    names the IMPLEMENTATION, not the form — dispersion is
    technique, association is essence), copia (internamentum
    as hidden set), arbor (silva named after the form).
    Dictionary designed to be HARVESTED.
55. persona v4: Dijkstra (called in by a single keyword),
    Meyer ("doctor canalium declaratorum ante nos" — begged
    twice in one day), Duns Scotus (Doctor Subtilis; second
    scholastic in the dictionary, and like Aquinas his great
    work carries imperfecta relicta).

Ruling recorded from discussion: architecture-pattern genus
DEFERRED (catch-all risk); GoF specifically viable later as
PUBLICATION-membership (relatum publicatum_in → the 1994 opus,
four auctores) — the planta nomenclature precedent, not an
essence. Twelve models. Surfaced and waiting: momentum
(instant), instrumentum_aedificationis, typus,
constructum_linguae, processus, hewitt (persona), GoF quartet.

## 2026-08-04 — round twelve: fabricatio, schema, cliens (15 models)

Fran's list off the top of his head (bug, requirement, tests,
feature, schema trio, client + seven subs) grouped into three
models. Commit de07b0a sealed rounds 8-11 first.

56. fabricatio.stml — THE INTENTION SQUARE: postulatum
    (declared before), facultas (intention POSSESSED — the unit
    of value), probatio (executable interrogation), vitium
    (measured divergence). The flagship argument: "non vitium
    sed facultas est" is a dispute about GENUS assignment —
    same behavior, contested kind — resolvable only by
    consulting intention, which proves all four genera live in
    the intention layer. vitium's definitio: behavior alone
    cannot be buggy; a bug is a RELATION to an intention.
    Richest state: non_sanandum — the bug dies by
    RE-DECLARATION, code unchanged (wontfix as metaphysics).
    sanatio = mutatio fontis (cross-model to elementa);
    regressio species carries the house's founding reason for
    tests, guarded by probatio_regressionis (the bug's ghost
    standing guard); probatio's honesty nota records the
    house-measured fact that tests can lie (green-while-lying,
    fixtures-share-assumptions). non_reproductum kept as an
    honest state: neither declared false nor proven true.
57. schema.stml — SUI DESCRIPTIO: natura's proprietates ARE
    campi, the genera palette IS genus_valoris's dictionary,
    the metamodel's genus= attribute refers to this genus. The
    naturalist describing his own skeleton. The house-found
    DISCIPLINA BIFIDA recorded as the schema's gravest
    property: constringens (nonconforming data REJECTED — the
    database way) vs iudicans (accepted and FLAGGED — the
    forum's "definition judges, doesn't constrain"; judging
    never loses truth, constraining turns it away at the
    door). genus_valoris = the INTENTION of a value, not its
    machine representation (typus is materia). "relatio" of
    the palette renamed CITATIO at species level: a value that
    IS another thing — the interop seed from this morning's
    brainstorm, now a dictionary entry. Dubium carried:
    area/annus — species, or textus-with-presentation and
    dies-with-precision? (presentation/precision may be CAMPUS
    properties).
58. cliens.stml — seven sub-genera flat per Fran's list, with
    the TWO AXES named in the header but not yet mechanized
    (quis utitur: homo/machina/agens × ubi vivit). cliens_mcp
    recognized as a genuinely NEW kind of our age: the client
    whose USER is an artificial agent — the who-uses axis
    grew a third member. servus surfaced externum (hospitium
    and tabulariumd the future witnesses). vitrea noted
    honestly as hybrid (web client in a native vessel). curl
    the first resident: witness that cliens_cli and cliens_api
    can be one body.

Fifteen models, one day. The surfaced queue now: momentum,
instrumentum_aedificationis, typus, constructum_linguae,
processus, servus, hewitt, GoF quartet, directorium, chip,
att, vms, systema_plagularum, communitas, domus_editoria,
animal.
