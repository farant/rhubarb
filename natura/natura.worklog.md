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

## 2026-08-04 — round thirteen: THE PISTRINA EXPERIMENT
## (coverage-simulatio: model a foreign scenario, harvest genera)

Fran's experiment design: pick a random situation, attempt to
model it with existing genera, and surface NEW GENUS SUGGESTIONS
instead of complications (simulatio adapted to coverage-
building). First run: a neighborhood BAKERY (pistrina) — three
never-touched axes: physical production, commerce, recurring
time. Discussion only; no models written — harvest awaits
Fran's blessing per the Alexander process.

COVERAGE CONFIRMED (the 15 hold in foreign territory):
- organizatio (the shop is a societas with perduratio),
  persona + roles-are-relations, norma THREE times (health
  code, ISO 4217 currencies, SI units — the norma model keeps
  paying), eventum, machina with time-driven transitions
  (perishability: recens→vetus per moram — planta's "per ver"
  legitimized it), opus_scriptum (cookbooks; Pacioli).
- MULTIPLICITY RULING confirmed: "merchandise" is NOT a genus —
  being-for-sale is a role, roles are relations. Applied
  cleanly, unprompted, in a domain it was not built for.
- THE INTENTION SQUARE GENERALIZES TO MATTER: a custom cake
  order IS a postulatum; the burnt batch a vitium; the taste
  test a probatio (probatio_fumi literal in a bakery). ONE
  BEND: physical vitium is not SANABILE — sanatio = mutatio
  fontis assumes the wounded thing is editable; matter is
  irreversible (discard and rebake). Sanatio is software's
  privilege; fabricatio may someday need reficere beside
  sanare.

HARVEST (candidate genera, ranked by pull strength):
1. mensura + unitas_mensurae — number-with-unit; units are
   individua of PUBLIC RECORD normata per SI, with historiae
   (kilogram redefined 2019). Purest Linnaean material since
   formae_datorum; also the true home of planta's open
   ranges-dubium (oven 170-190°C).
2. praescriptum — ordered instructions executed by a PERSONA:
   functio's human cousin. Scales, versions (versio pattern's
   fifth knock), published in opera. The house already runs on
   unnamed praescripta: SOPs, checklists, the forum's
   contractus correspondentis.
3. artefactum + materia — made corporeal things (partes ready)
   + STUFF measured-not-counted; the mass/count cut is real
   ontology.
4. commerce cluster: venditio (event mutating possessio),
   possessio, pecunia/moneta (EUR/USD individua, ISO 4217),
   pretium (time-varying relation to a mensura of money).
   DEEP CATCH: the register tape = acta venditionum, the
   day-end count = plicatura — DOUBLE-ENTRY BOOKKEEPING IS THE
   ORIGINAL EVENT-SOURCING (Pacioli 1494, Summa de
   arithmetica: opus + persona waiting). The volumen doctrine
   has a 500-year ancestor.
5. obligatio + contractus + licentia — debt/agreement/grant.
   PRIOR ART OF THE HIGHEST ORDER: Roman law already built
   this taxonomy (ius obligationum; ex contractu / ex delicto
   — Gaius and Justinian as the Linnaeus of obligation). The
   Latin is not translation here; it is REPATRIATION.
6. locus — premises, address, jurisdiction.
7. horarium — recurrence over momenta (4am bake, shifts,
   opening hours); momentum itself still queued beneath it.
8. organizatio wants sub-genus magistratus/res_publica (the
   health code's editor is a government).

VERDICT ON THE LOOP: it works. One scenario, ~90 minutes of
walking: 8 genus clusters surfaced EACH WITH FORCING CONTENT
ATTACHED (what the Alexander process wants as input), 3
doctrine confirmations, 1 doctrine bend, 0 dubious genera —
every candidate has public-record dictionary material. Rough
coverage: half the bakery was expressible with 15 models built
from a software parish. The loop is repeatable: scenario →
stations → cite-or-surface → harvest.

## 2026-08-04 — round fourteen: the pistrina harvest built (22 models)

Fran blessed the harvest; seven models written from it. The
LOOP CLOSED: scenario → surfaced genera → built models. This
is the first material in the library that came from a
deliberate coverage instrument rather than from a pull.

59. mensura.stml — magnitudo_mensurabilis / unitas_mensurae /
    quantitas. The catch that motivated it: a property typed
    "numerus" carries a BARE NUMBER, which is a lie — 500 is
    nothing until "grams" is present. Units are individua of
    public record with real histories: the KILOGRAM was a
    THING (1889 platinum-iridium prototype) and became a LAW
    (2019, Planck constant) — a unit with a genuine historia.
    Celsius carries the interval/ratio-scale catch (conventional
    zero: differences add, values don't multiply). Mars Climate
    Orbiter cited in the definitio as the cost of unitless
    quantities.
60. NEW MECHANISM (unforeseen, content-forced): properties
    typed by a LIBRARY GENUS rather than a palette primitive —
    `genus="quantitas" modulus="mensura"`. The palette
    (textus/numerus/dies/...) was closed and primitive; the
    library can now type its own properties. Seven uses at
    birth. natura_visus taught to validate this reference
    class the same hour, gate-tested negatively.
61. praescriptum.stml — functio's HUMAN cousin, and the
    difference that changes everything: the executor is a
    persona (varying skill, own judgment), so a praescriptum
    MAY carry ambiguity where a functio may not ("coque donec
    aureum fiat") — and that ambiguity is trust in the
    executor, not a defect. Also: dependency between steps is
    a GRAPH written as a series for convenience; yield does
    not scale linearly (leavening and bake time refuse). Fifth
    knock of the versio pattern (versio_praescripti). The
    house is full of unnamed praescripta.
62. corporalia.stml — res_corporea / artefactum / materia /
    instrumentum. THE COUNT/MASS CUT as real ontology:
    "three flours" is meaningless; divided material stays what
    it was (half a mass of flour is flour), a divided artifact
    perishes (half a loaf is not a loaf). Identity consequence
    linking to the Scotus cut: materia has NO haecceity —
    form alone constitutes it, exactly as content-addressed
    massae have none. cibus's differentia: use DESTROYS it
    (instrument survives use, food does not). res_corporea's
    machine transitions PER MORAM — the first machine whose
    transitions have no actor: nobody makes bread stale, time
    does.
63. commercium.stml — moneta/pecunia/possessio/pretium/
    venditio/ratio_rationaria. THE ROUND'S DEEPEST CATCH:
    double-entry bookkeeping IS event sourcing — the register
    tape is acta, the day-end count a plicatura, correction is
    a NEW contrary entry (stornatio) never an erasure, and
    ratio_duplex's balance identity is an INVARIANS built into
    the log itself. Pacioli 1494 (describing, not inventing —
    Venetian merchants already did it). The volumen doctrine
    has a 500-year-old ancestor, and its reason is ours: a
    ledger that can be rewritten is not a ledger. pretium
    carries the sliding law again (price belongs to a
    time-varying RELATION, not to the thing). pecunia carries
    the float-money prohibition. merx confirmed NOT a genus.
64. obligatio.stml — REPATRIATION, not translation: Roman law
    built this taxonomy and the language is already ours
    (Institutiones' "iuris vinculum quo necessitate
    adstringimur"; ex contractu / ex delicto / ex lege). The
    states no software schema would invent: REMISSA (creditor
    forgives — dies without performance) and PRAESCRIPTA (dies
    by mere lapse of time). Fran's contractus/licentia cut
    made structural: contract = bilateral consent, binds both;
    licence = unilateral grant, revocable, expiring. Note
    recorded that software licences (GPL/MIT) are of the
    licentia genus — forensic and computational domains share
    a genus.
65. locus.stml — locus / inscriptio / iurisdictio / aedificium.
    The three-way cut schemas collapse into "address": the
    place is in nature, the address is a CONVENTION (streets
    are renamed, the place does not move), the jurisdiction is
    a POWER (and powers overlap — one place under municipality,
    province, nation simultaneously — where places nest
    strictly). Address-line format varies by country: the
    one-schema-fits-all lie named.
66. tempus.stml — momentum (queued since round eleven, finally
    called) / intervallum / recurrentia / horarium /
    calendarium. RECURRENTIA IS A RULE, NOT A SET — its
    extension is infinite, so storing it expanded truncates
    the future; exceptions (feast days) are PART of the rule.
    momentum carries its own PRECISION (1225 is known to the
    year; faking precision lies) and the house's signed-delta
    war story. Gregorian calendar's TEN DELETED DAYS (Oct 4 →
    Oct 15, 1582) as the witness that calendars are convention
    with teeth. Liturgical calendar carried as an open dubium:
    movable feasts need COMPUTED recurrence, which our simple
    periodicity cannot express. horarium.vinctum_per records
    the bakery's hard truth: the oven's capacity determines
    the schedule — iron commands hours.
67. Surgical edits: organizatio grew MAGISTRATUS (authority by
    IMPERIUM, not consensus or profit — with the note that
    ISO's own standards have no force until a magistrate
    adopts them); planta's ranges-dubium PARTLY RESOLVED
    (quantitas carries min/max; the TIME range Maius-Iunius
    remains open, pointing at tempus); fabricatio grew the
    sanatio-vs-refectio dubium (matter is irreversible;
    corporalia.instrumentum already says refectio — the
    language had divided before the doctrine did); persona
    gained Pacioli.

Measured after: 22 exemplaria / 96 genera / 169 dictionary
residents / 251 arcs / 0 vulnera. The library grew ~50% in one
round from a single imagined bakery.

## 2026-08-04 — round fifteen: THE CLINIC (loop, second run)

Second run of the coverage loop, scenario chosen to stress the
axes the 22 models still had not touched: living bodies,
evidence, and UNCERTAINTY. A small family medical practice.
Discussion only; harvest awaits Fran.

THE FIND OF THE RUN — THE EPISTEMIC GAP:
The library asserts everything FLATLY. <valor>3</valor> carries
no confidence, no author, no revisability. A DIAGNOSIS cannot
be said in our format: it is a judgment, not a fact —
provisional, revisable, probabilistic ("likely X, rule out Y"),
authored, dated. And this is THE SAME GAP as the <fons> dubium
open since round seven: "Ritchie born 1941-09-09" is a
claim-with-a-source held at some confidence, exactly like a
diagnosis. The citation question was never about formatting;
it is a missing genus.
→ IUDICIUM: a claim about reality bearing author, moment,
  grounds, and DEGREE OF ASSENT. The naming has magnificent
  prior art and it is the house's own: the scholastic degrees
  of assent beat any high/medium/low confidence field —
  SCIENTIA (certain), OPINIO (held with fear of the opposite),
  FIDES (held on authority — precisely what this scribe's
  training-sourced dates are), SUSPICIO. Aquinas's epistemology
  as a schema, and honest about the scribe's own limits.

HARVEST (rest, ranked):
2. VIVENS as res_corporea's THIRD branch (artefactum = made,
   materia = stuff, vivens = alive) + corpus_vivum, animal
   (queued since planta's pollinators). STRUCTURAL FIND: planta
   currently floats free and BELONGS under vivens — which
   raises a format question never faced: CAN A GENUS IN ONE
   MODEL BE sub= A GENUS IN ANOTHER? Cross-model subordination
   is not in the metamodel. Real question, not cosmetic.
3. OBSERVATIO divided by source — reported by the subject
   (symptoma: unverifiable by anyone else), measured by
   instrument (signum: a quantitas), judged by an expert. The
   library has no vocabulary for WHO CAN VERIFY A CLAIM;
   medicine has drawn this line for millennia.
4. MORBUS — kinds of disease are timelessly describable with a
   typical course (machina); the domain has its own Linnaeus
   (ICD, published by WHO). Real library material.
5. RESERVATIO — a claim on FUTURE capacity (appointment slots,
   seats, rooms). tempus can say when; nothing says "claimed."
6. TITULUS — the earned, permanent, evidentiary credential (an
   MD degree): not revocable, does not expire, attests
   achievement rather than granting permission. Distinct from
   licentia, which the same scenario CONFIRMED.
7. PERICULUM / probabilitas — risk ("1 in 10,000"); part of the
   epistemic gap, and what insurance contracts are ABOUT.

CONFIRMATIONS (the 22 holding in a third foreign domain):
- LICENTIA confirmed by its own namesake: a medical licence is
  granted by a board (magistratus), revocable, expiring,
  renewable — genus and English word coincide.
- HISTORIA confirmed at maximum strength: medical records are
  LEGALLY REQUIRED to be append-only, amendments as new entries,
  never erasures. The volumen doctrine enforced by law — a
  SECOND ancestor after Pacioli (round fourteen). Two domains
  now, independently, for our exact reason.
- QUANTITAS + RECURRENTIA composing in one instruction ("500 mg
  twice daily") — two models built hours apart from a BAKERY,
  meeting correctly in a clinic. The strongest evidence yet
  that the models compose rather than merely coexist.
- praescriptum (treatment protocol, judgment-ambiguity licit),
  residence rule (disease KINDS in the library, THIS patient's
  illness in an app volumen), multiplicity ruling (patient is a
  role → relation, third confirmation), count/mass cut (pills
  countable, syrup measured), norma (clinical guidelines),
  organizatio/magistratus (the board), mensura (38.5 °C).
- CONSENT stress-tested LICENTIA rather than surfacing a genus:
  informed consent is a unilateral revocable permission from
  the one with authority over the thing — the authority being
  PERSONAL, not governmental. The shape held.
- TRIAGE deliberately NOT surfaced: an ordering criterion, not
  a thing — like merx in round thirteen. Recording the
  non-surfacings is part of the discipline.

## 2026-08-04 — round sixteen: IUDICIUM built (23 models)
## (the epistemic layer — Fran: "lets do iudicium first")

Built ahead of the cheap surfaces deliberately: iudicium changes
how every future dictionary entry is written, so its form should
settle while 22 models exist to retrofit, not 40.

68. iudicium.stml — three genera. GRADUS_ASSENSUS carries the
    scholastic degrees as real things with definitions
    (scientia / opinio / fides / suspicio / dubitatio) rather
    than a high/medium/low enum — philosophy already drew this
    taxonomy and drew it better. FONS closes the round-seven
    dubium: species fons_scriptus / fons_mensurae /
    fons_testimonii / CORPUS_INSTITUTIONIS — the last being a
    species no library before this age could have needed: the
    scribe's own training memory, unverifiable BY NATURE, whose
    honesty consists in calling the reader to verification
    rather than to belief. IUDICIUM itself rests on the
    ontological cut that governs everything: the judgment is
    not the fact judged — "the patient has pneumonia" may or
    may not obtain, but "Dr X judged on day D that..." CERTAINLY
    happened, and stays true even when its content is false.
    That is WHY medical records keep wrong diagnoses, and why
    revisum supersedes rather than deletes (norma's substituit
    chain again). Species: diagnosis, aestimatio (three
    appraisers give three prices — not error but the nature of
    judgment), praedictio (the only judgment TIME itself
    verifies), sententia_iudicis (where judgment touches law and
    parit obligationem).
69. THE THREE SIBLINGS COMPLETED: assertio (checked by the
    program, running), annotatio (checked by external tools),
    iudicium (held by a person, fallible, revisable). Three
    propositions, three verifiers — a structure that was
    implicit since round eleven and is now stated.
70. METAMODEL MECHANISM (content-forced, the point of the
    round): attributes certitudo= and fons= on <valor>,
    <relatum>, <eventum>, plus a module-level <fontes>
    declaring keys. Applied REFLEXIVELY — the library describing
    judgment then judging its own claims.
71. DOGFOODED ON THE LIBRARY'S OWN WEAKEST CLAIMS (persona v5,
    chosen because it was the most mendacious file: every birth
    and death date comes from the scribe's training and NONE is
    verified, yet the old form presented them exactly as it
    presented our definitions). Aquinas's 1225 marked OPINIO,
    not fides — the sources themselves disagree, a degree below
    faith. Ritchie's dates marked fides with the nota naming
    the old form's exact lie. Graydon Hoare's missing birth date
    documented as a JUDGMENT not a data gap: living persons are
    private, and absence is honester than conjecture.
72. GATE AT BIRTH (house law since round fourteen) — and it
    PAID THE SAME HOUR: natura_visus taught to resolve fons=
    against declared keys and certitudo= against the real
    degrees, then negative-tested both. The first run FAILED
    with 5 vulnera: I had written certitudo="fide" (ablative,
    "held BY faith") where the species is fides. Elsewhere the
    library's values match declared names exactly
    (vita→vigens, paradigmata→imperativum), so machine-
    checkability wins over grammatical flourish. A validator
    born the same hour as its mechanism caught a consistency
    drift a reader would have missed for months.

Measured: 23 exemplaria / 99 genera / 182 residents / 255 arcs
/ 0 vulnera.

REMAINING FROM THE CLINIC HARVEST (cheap, deferred by design):
vivens (+ the cross-model sub= question), observatio (surfaced
as externum from iudicium.fundatur_in — the evidence side),
morbus, reservatio, titulus, periculum.
[BUILT round seventeen, same day.]

## 2026-08-04 — round seventeen: the clinic harvest completed
## (25 models — and the Porphyrian tree closes on itself)

73. VIVENS born as res_corporea's THIRD branch (corporalia v1):
    neither made (artefactum) nor stuff (materia) — the thing
    with its principle of motion WITHIN. Carries the
    Aristotelian anima ladder (vegetativa/sensitiva/rationalis)
    as ONE form graded, not three things. Its death transition
    is unique in the whole library: the only transitus that
    LEAVES A DIFFERENT THING — a corpse is still res_corporea
    but has ceased to be vivens. Substantial, not accidental
    change; hence no return path, unlike every other machine
    we have written.
74. NEW MECHANISM — CROSS-MODEL SUBORDINATION: `sub=` plus
    `modulus=` on a genus. Rationale recorded in corporalia's
    header and it is the real point: THE TREE IS ONE; models
    are FILES, not kingdoms. Splitting into files is our
    convenience, not a fact about things, and the Porphyrian
    tree knows nothing of file boundaries. This completes the
    family: relations crossed models (round one), properties
    crossed models (round fourteen), subordination now. First
    two uses: planta and animal under corporalia.vivens.
75. ANIMAL born (model 24) — closing the library's OLDEST
    shadow: surfaced by planta's pollinatur_a in round seven,
    it waited ten rounds. apis now described, and the
    malus_domestica pollination edge is real in both
    directions. Proof that computed umbrae are a genuine
    agenda, not decoration.
76. THE PORPHYRIAN TREE CLOSED ON ITSELF: substantia → corpus
    → vivens → animal → animal rationale (homo). The format
    took its NAME from that tree on day one and has now
    RECONSTRUCTED it branch by branch without ever planning
    to — recorded in vivens's nota as evidence that the
    taxonomy was found, not invented. homo and persona held
    apart cleanly by the multiplicity ruling: homo is the
    body-species, persona is rational substance (Boethius); a
    man is both, an angel is persona and not animal, a dog is
    animal and not persona. Individuals stay in persona — "he
    who speaks of Ritchie speaks of a person, not of a body."
77. MORBUS (model 25) with the foundation that makes the
    domain possible: DISEASE PRESUPPOSES A NATURE. To be sick
    is to fall short of what is OWED to the thing — so only
    what has a nature and an end can be sick; a rock cannot,
    and a machine is broken rather than ill because its end
    was given by its maker, not its own. Pieper's discipline
    proved in an alien domain: true description of things
    presupposes teleology whether we like it or not. Disease
    as PRIVATIO (what is missing from health, as darkness to
    light), not a thing. Residence rule confirmed again
    (kinds here, this patient's illness in an app volumen),
    with ICD named as the domain's normative taxonomy — where
    our library must CITE an external authority rather than
    invent one. Species divided by CAUSE (cause governs cure).
    Two cross-domain rhymes recorded: recidivum ≡ regressio
    (same thing, two domains), and vulnus deliberately shares
    its name with silex's hand-edited-file wound — the pun
    turns out to be true (integrity injured by external
    force).
78. OBSERVATIO (iudicium v1) — the evidence side, closing
    iudicium's own shadow the same day it was cast. Divided by
    WHO CAN VERIFY: symptoma (only the sufferer has access —
    "the species that shows private truth is still truth"),
    signum (any competent observer, and the instrument yields
    a QUANTITAS), testimonium (verifiable through the witness,
    not the thing). The epistemic gap that hides inside data.
79. PERICULUM (iudicium v1) — risk as DOUBLE: probability AND
    gravity, so a single number lies (rare-and-ruinous ≠
    frequent-and-trivial, even at equal product). Its nota
    names what the pre-round-sixteen library could not do at
    all: no probability, no future harm — meaning it could not
    express PRUDENCE, the virtue that deliberates about
    uncertain futures. transfertur_per → contractus: insurance
    is a contract whose subject matter IS a risk.
80. RESERVATIO (tempus v1) — a claim on future capacity: "we
    could say WHEN; we could not say CLAIMED." Its deserta
    state is pure loss (capacity perishes unused and ungiven),
    which is exactly why overbooking exists — recorded as the
    place where periculum, obligatio and reservatio all
    operate at once and none alone explains the practice.
81. TITULUS (obligatio v1) — Fran's licence/credential contrast
    made structural: a licence permits and is revocable, a
    title attests and is permanent ("what has been done cannot
    become undone"). The model is honestly noted as having
    grown past pure obligation into the family of WHAT
    AUTHORITY CONFERS. Species: gradus_academicus (with the
    lovely historical note that medieval "licentia docendi"
    was BOTH at once, and our modern word descends from it —
    the history of the word preserves the distinction of
    genera), character_indelebilis (the terminal case that
    PROVES the differentia: not even the conferring authority
    can remove it — a laicized priest remains ordained, only
    the faculty is withdrawn), titulus_honoris (where the
    authority publishes its own aestimatio — title touching
    judgment).
82. natura_visus taught cross-model sub= (else the tree
    renderer would SILENTLY DROP planta — caught by reasoning
    before running, then verified), rendering such genera as
    roots marked "⊂ corporalia.vivens". Both failure modes
    negative-tested (bad parent name, bad module).

Measured: 25 exemplaria / 106 genera / 195 residents / 278
arcs / 0 vulnera. Clinic harvest fully built; the loop's
second run is now closed end to end.

## 2026-08-04 — round eighteen: THE FORWARDER (loop, third run)

Scenario chosen for the one fundamental axis 25 models had
never touched: MOTION. We could say where a thing IS; nothing
in the library moved. A small freight forwarder. Discussion
only; harvest awaits Fran.

THE FIND OF THE RUN — THE TEMPORAL GAP (valid-time):
The library cannot say WHEN a claim was true. Round sixteen
gave every claim HOW SURE (certitudo) and FROM WHOM (fons);
this run names the third missing coordinate: VALID WHEN. A
shipment's position, an employer, a price, a border — all
time-varying, all asserted flatly.
Two pieces of evidence it is real, not over-engineering:
(1) THE LIBRARY ALREADY IMPROVISED IT ONCE — commercium.pretium
    carries valens_a / valens_ad, a local hack invented in
    round fourteen because price forced it. When one model
    quietly invents a mechanism, the metamodel usually needs
    it generally (same tell as round nine: when a relation's
    notas get heavy, the relation wants to be a thing).
(2) A LIVE FALSEHOOD IN THE LIBRARY RIGHT NOW — organizatio
    asserts `bell_labs pars_de att` flatly. True 1925 until
    the 1996 Lucent spinoff; we present as timeless a relation
    that stopped holding thirty years ago, with no way to say
    so. The pistrina-grade catch of this run, and it is OUR
    error, not a hypothetical.
Prior art to consult when building: bitemporal modelling
(valid time vs transaction time) — our iudicium.latum is
already assertion-time for judgments, so the library has one
axis and needs the other. Design question for Fran: attributes
on <valor>/<relatum> (valens_a=/valens_ad=, matching pretium's
own words), or a wrapper element? Cheap either way; the
DOCTRINE is the expensive part.

HARVEST (rest):
2. ITER (journey: origin, destination, purpose) + VIA (route
   as a path through loci) + VEHICULUM (an instrumentum that
   moves and carries — sub-genus; capacity and
   use-not-consumption already fit) + ONUS (cargo under an
   iter — which is what "a shipment" actually IS).
3. DAMNUM — and the symmetry it completes: vitium (divergence
   from INTENTION — software), morbus (falling short of a
   NATURE — living), damnum (injury to the INTEGRITY of a
   corporeal thing). Three kinds of wrongness for three kinds
   of thing, divided by what each is measured against. The
   morbus model half-saw this already when vulnus deliberately
   shared its name with silex's hand-edited-file wound.
4. TRIBUTUM — obligation to a magistratus arising from an act,
   not from consent; slots under the Roman ex_lege species
   already written.
5. CHARTA NEGOTIABILIS — the run's most interesting object: a
   document whose POSSESSION CONVEYS THE GOODS (bill of
   lading; cheques and bearer bonds are the same kind). A
   corporeal thing carrying an incorporeal right, where
   possessio of the paper and possessio of the cargo are
   welded. Bridges corporalia / commercium / obligatio.

CONFIRMATIONS (unusually strong this run):
- AN ETA IS A PRAEDICTIO: a judgment about the future,
  authored, revisable, verified by time itself. Tracking pages
  display IUDICIA and call them facts — the epistemic layer
  earning its keep in a mundane domain one round after birth.
- INSURANCE EXERCISED A RELATION WRITTEN HOURS EARLIER:
  periculum.transfertur_per → contractus was speculative when
  written in the clinic round; freight insurance is exactly
  that, and nothing needed changing. Models composing without
  retrofit is the strongest evidence they describe things
  rather than use cases.
- delivery discharges obligatio → soluta; contract of carriage
  is already locatio_conductio (Roman law species, round
  fourteen); driver's licence = licentia; hours-of-service =
  norma + horarium; warehouse = aedificium + capacitas;
  "where is it now" = historia + fold (position needs NO new
  mechanism — only valid-time).

## 2026-08-04 — round nineteen: VALID-TIME built (the third coordinate)

Fran took the dimension before the nouns again, same as
iudicium — settle structure while 25 models are cheap to
retrofit.

83. MECHANISM: attributes valens_a= / valens_ad= on <valor> and
    <relatum>. Round sixteen gave HOW SURE (certitudo) and FROM
    WHOM (fons); this is WHEN TRUE. Cheap in mechanism, and the
    expensive part — the doctrine — came out sharper than
    expected.
84. THE TRIPLE RULE (when to mark; documented in tempus.stml's
    header, which is now the metamodel's temporal chapter):
    I.   ESSENTIALIA — definitions, differentiae, species
         properties ("C is imperative"): TIMELESS, never marked.
         What belongs to a thing AS SUCH has no date.
    II.  STATUS — machine values (vita=vigens): PRESENT by
         nature, because a status IS a fold of historia and
         therefore means "now" by definition. Not marked; the
         historia carries the truth.
    III. RELATIONAL ACCIDENTS — an organization's parent, a
         price, a thing's location, an employment: THESE ALONE
         get marked. They were true, they ceased, and flat form
         necessarily lies.
    This explains why 25 models survived nearly lie-free: they
    were writing cases I and II almost exclusively.
85. DOCTRINE EMPIRICALLY CONFIRMED, not asserted: surveyed all
    117 dictionary relata in the library. Exactly ONE was a
    case-III falsehood — bell_labs pars_de att, true 1925 until
    the 1996 Lucent spinoff. Fixed with valens_a/valens_ad plus
    certitudo/fons, and its nota names it as our own first
    time-healed lie. Everything else (authorship, genealogy,
    versio_de, scripta_in, standard editors) is genuinely
    timeless. A survey that could have embarrassed the doctrine
    instead measured it.
86. TERMINAL CONVENTION DECIDED: absent valens_ad = STILL HOLDS,
    following organizatio.dissoluta's existing precedent
    ("vacuum = perdurans"). If genuinely unknown, certitudo=
    says so — the two mechanisms compose rather than overlap.
87. REIFICATION recorded as the deliberate ALTERNATIVE: a
    time-bound fact may become an entity with its own
    properties (commercium.pretium already carries valens_a/
    valens_ad as PROPERTIES). Heavier but richer — choose it
    when the fact itself has an author, a cause, a price of its
    own; choose attributes when the fact is bare. And the tell
    is recorded: pretium INVENTED this mechanism locally in
    round fourteen, which is the same law as round nine (a
    relation whose notas grow heavy wants to be a thing).
88. GATE AT BIRTH, third consecutive round: natura_visus
    validates date FORM (yyyy / yyyy-mm / yyyy-mm-dd) and
    INTERVAL ORDER, and reports a tempus-validitatis count.
    Both negative-tested (inverted interval caught; "MCMXXV"
    caught). The inverted-interval check is the one that will
    earn its keep — a transposed pair is invisible to a reader
    and fatal to a fold.

SURFACED BY THIS ROUND (not built): the metamodel now has five
mechanisms (cross-model relations, library-typed properties,
cross-model subordination, certitudo/fons, valid-time)
documented across scattered file headers. THERE IS NO SINGLE
METAMODEL DOCUMENT. That is a real gap for any reader who is
not the scribe, and it is the natural companion to the
LOADER=VALIDATOR when that is built — the loader will need
exactly this specification. Flagged for Fran, not built.

Measured: 25 exemplaria / 106 genera / 195 residents / 278
arcs / 1 tempus-validitatis / 0 vulnera.

## 2026-08-04 — round twenty: METAMODULUS (the format specified)

Flagged at the end of round nineteen, built before the freight
nouns at Fran's direction: five mechanisms were documented
across scattered file headers with no single specification.

89. natura/METAMODULUS.md written — and written from MEASURED
    reality, not memory: the element and attribute vocabulary
    was extracted mechanically from the corpus first (25
    elements, 26 attributes). Sections: doctrine, document
    shape, element reference with live usage counts, attribute
    reference, the three cross-model mechanisms, THE THREE
    COORDINATES OF A CLAIM (certitudo / fons / valid-time with
    the triple rule), umbrae as computed agenda, NORMATIVE
    RULES, conventions, deliberate non-decisions, open
    questions. English prose with Latin identifiers, following
    natura.worklog.md's precedent in the same directory —
    recorded as a decision so it is not read as drift.
90. THE LOADER'S CONTRACT written as 15 numbered rules, each
    marked [E]nforced or [S]pecified. Eight are enforced today;
    seven (rules 9-15: valor names a declared property, electio
    values match a declared optio, relatum names a declared
    relation, transitus endpoints are declared statuses, eventum
    actio resolves, modulus equals filename, no duplicate genus
    names) need GENUS-INHERITANCE RESOLUTION — which is exactly
    the loader's job and now has a written work list. Rules 9-13
    are named as the Cyc-consistency defense: what keeps a
    hand-written library coherent past the point where one mind
    holds it.
91. RULE 8 MAKES THE SPEC SELF-ENFORCING: natura_visus now
    checks a CLOSED VOCABULARY — any element or attribute not in
    METAMODULUS §3/§4 is a vulnus. The document therefore cannot
    silently rot away from the artifacts; adding a mechanism
    without amending the spec fails the gate. This is the
    strongest form of gate-at-birth we have built: the
    documentation is now load-bearing.
92. TWO WARTS RECORDED RATHER THAN HIDDEN (METAMODULUS §4):
    `genus` is both an element (a kind) and an attribute (a
    value type); `a`/`ad` mean origin/destination on transitus
    but source/target on relatio. Analogous, not identical — a
    parser must not assume.
93. THE CHECKER ITSELF TAUGHT THREE LESSONS, all measured, all
    from the same root — GREP ON RAW TEXT IS NOT A PARSER:
    (a) first inventory was polluted by element names mentioned
        inside COMMENTS (fontes/fons double-counted); fixed by
        stripping comments before scanning;
    (b) `puritas="verum"` was flagged as an unknown attribute —
        it is PROSE inside a <dubium> discussing a hypothetical.
        Fixed by extracting attributes only from within tags,
        and by CONSUMING quoted values so prose like
        "sine schemate = sacculus" cannot masquerade as an
        attribute (three such false positives);
    (c) the tag regex ended at the first `>`, so attributes
        AFTER a value containing "PowerPC -> Intel" were never
        scanned — a silent FALSE NEGATIVE, the dangerous kind.
        Fixed with an alternation that lets quoted values
        contain `>`, then PROVEN by planting an unknown
        attribute after that very value and watching it get
        caught.
    Plus a pure shell trap worth its own line: newlines inside
    $(...) are COMMAND SEPARATORS, so a multi-line word list
    must be built as a quoted string and normalized by unquoted
    expansion. The first two versions of the check reported
    175 and 513 phantom vulnera respectively — a validator that
    fails loudly and wrongly is still better than one that
    passes quietly and wrongly, but only just.

Measured after: 25 exemplaria / 106 genera / 195 residents /
278 arcs / 0 vulnera, with vocabulary now closed.

STILL UNBUILT from run three: the freight nouns (iter, via,
vehiculum, onus, damnum, tributum, charta negotiabilis) —
damnum wants writing WITH vitium and morbus in view so the
three-way symmetry (intention / nature / integrity) is stated
in all three files.
[BUILT round twenty-one.]

## 2026-08-04 — round twenty-one: the freight nouns (26 models)

94. vectura.stml — iter / via / vehiculum / onus. MOTION, the
    last untouched fundamental axis: we could say where a thing
    IS; nothing moved. iter is change-of-place-with-purpose;
    its interruptum state is named as the one binary schemas
    (sent/delivered) drop and business suffers daily. via is
    the CONDITION of a journey rather than a journey — "the
    Appian Way lies there though no one carries freight on
    it" — with via_maritima recording that a way need not be
    BUILT, and via_aeria that a way can exist by LAW alone.
    vehiculum uses cross-model subordination (sub instrumentum
    in corporalia).
95. LAW FOUND TWICE, IN TWO DOMAINS: vehiculum.capacitas_oneris
    carries the same note as the bakery oven — FERRUM MODUM
    IMPERAT, iron commands the schedule. A physical capacity
    ordering the whole operation, discovered independently in
    round fourteen and again here. Two witnesses make it a
    law rather than an observation.
96. onus is a CONDITION, not a kind: the same barrel is cargo
    while carried, merchandise while sold, furniture while
    used. Roles are relations — the multiplicity ruling's
    fourth confirmation, unprompted. Its custodia machine
    names the chain-of-custody distinction the law already
    draws: THE CARRIER HOLDS, THE OWNER POSSESSES — which is
    exactly what makes charta negotiabilis possible.
97. onus's closing nota is the round's best dogfood: "where is
    the cargo" is a CASE III accident (METAMODULUS §6) —
    "in Rotterdam" was true and ceased. Position history with
    valens_a/valens_ad, current position a FOLD. The volumen
    doctrine in a shipping container.
98. DAMNUM — and the TRIAD OF DEFECTS completed, the round's
    real find: three kinds of "wrong" distinguished by the
    STANDARD each is measured against, not by the thing harmed
    — vitium from INTENTION, morbus from NATURE, damnum from
    INTEGRITY. Hence a stone can be neither faulty nor sick but
    CAN be damaged: it has integrity though it has neither
    intention nor a nature of its own. The triad has an order:
    the more that is owed to a thing, the more kinds of evil it
    can suffer. WRITTEN INTO ALL THREE FILES verbatim so no
    reader landing in one takes the part for the whole.
    damnum.contaminatio pays off the v0 count/mass cut:
    countables break, measurables are contaminated.
99. tributum (obligatio v2) — full Roman vocabulary
    repatriated: portorium (the frontier duty that makes
    jurisdiction itself lucrative — same word, same mechanism,
    two thousand years), vectigal, decuma (a tax by PROPORTION,
    not by sum), tributum_capitis (whose fairness has been
    disputed for centuries — recorded as evidence that a tax
    always carries a moral judgment).
100. charta_negotiabilis (commercium v1) — a corporeal thing
    carrying an incorporeal right: stealing the paper steals
    the right, losing it loses the right, though the goods sit
    untouched in a warehouse. charta_onerum let goods be sold
    while the ship was still at sea — "the thing at sea, the
    ownership in the market". AND THE HOUSE-RELEVANT CATCH:
    a bearer token in our own systems is this exact form —
    possession confers authority, no name asked; same benefit
    (transfers without permission), same danger (the thief
    has it, the loser lacks it). Merchant law knew this form
    and its risks in the 14th century; our machines
    reinvented it without the memory.
101. The gate caught its author again, correctly: fons="corpus"
    was cited in vectura before any <fontes> block declared
    the key (rule 5). Fixed. And the diagnostic printed an
    EMPTY name because <eventum> has no nomen — repaired per
    house law that diagnostics must be SELF-NAMING, now
    falling back to quando ("vectura.-312").

Measured: 26 exemplaria / 113 genera / 214 residents / 305
arcs / 0 vulnera. Run three fully built; loop closed end to
end for the third time.

## 2026-08-05 — round twenty-two: INDEX + Fran's division ruling

102. natura/INDEX.md born — the agent-facing projection, emitted
     on EVERY validator run (never behind a flag: what can be
     forgotten will be, and git shows a stale index immediately).
     Genera with glosses, alphabetical lookup, dictionary,
     umbrae, dubia, closed vocabulary. METAMODULUS correspondingly
     DROPPED its live counts — a specification that quotes a
     moving number is stale by the next afternoon.
     Writing it caught four real bugs: two in the tool (BSD
     `paste -sd', '` CYCLES delimiters; a filter running before
     the substitution it depended on) and two in the models —
     mensura declared a FALSE umbra (a genus in its own file
     marked externum) and planta carried a STALE externum flag on
     apis, described five rounds earlier. Umbrae 15 → 13, all
     thirteen now genuine. The §7 warning about stale flags
     proved itself within a day of being written.

103. FRAN'S DIVISION RULING — and it corrected a real error in my
     working heuristic. I had been using "does the differentia
     buy affordances" (purity earns memoize/reorder/parallelize,
     therefore essence). Fran's criterion is ONTOLOGICAL: is the
     genus about something REAL — moderate realism, universals
     in re? If yes, coin it, application or not.
     THE CORRECTION MATTERS: my affordance test was a USE-CASE
     test wearing a taxonomy costume — the exact Pieper failure
     we killed at the property layer, relocated to the genus
     layer where it is harder to see. A library that only names
     distinctions it can currently exploit is a use-case schema
     with better manners.
     Consequences recorded in METAMODULUS §9:
     - risk is ASYMMETRIC — failing to name a real distinction is
       nominalism creeping in; a redundant genus for something
       real is nearly free and locally fixable. WHEN UNCERTAIN,
       DIVIDE.
     - expect HUNDREDS of genera at general scope; that is the
       right order of magnitude, and the work does not obsolesce.
     - notas growing long is not a defect: thinking clearly about
       a nature IS the hard work, and the record of it is the
       artifact's value (Alexander).

105. GENERA VS ANALOGIES (Fran, same conversation) — the second
     correction, and it names something the session had been
     doing without vocabulary. The cross-domain rhymes this work
     keeps producing (mailbox:actor :: parametra:functio;
     ledger:balance :: acta:manifestum; bearer bond:goods ::
     bearer token:authority) are ANALOGIES — analogia
     proportionalitatis — NOT genera. A genus admits univocal
     predication; an analogy does not. Forcing one into the other
     is precisely where the material stops supporting the
     abstraction, and it is the classic CS failure
     (everything-is-an-object, universal upper ontologies, deep
     inheritance).
     FRAN'S POINT ABOUT METHOD: nominalism is GOOD at finding
     these patterns — it looks for regularity in usage rather
     than essence — but cannot say "genuinely similar, not the
     same kind", so every strong similarity becomes a candidate
     abstraction and gears spin. Moderate realism can affirm the
     resemblance, decline the genus, articulate the individual
     case, and MOVE ON. That freedom is the methodological
     advantage, not the metaphysics alone.
     Also his: interfaces/duck typing are the engineering form of
     analogical grouping — "responds to read()" is about
     behaviour, not essence — on a DIFFERENT AXIS from the
     Porphyrian spine, hence not to be forced onto it. Graph
     relations and schema patterns are further tools, each right
     in its own scenario.
     Recorded in METAMODULUS §9. Audit of the session's own
     rhymes against the new criterion: nearly all were already
     held as notas (correct); the GoF deferral was this judgement
     before it had a name; and ONE case changes — charta
     negotiabilis may be a genuine GENUS whose NAME is too narrow
     ("charta" = paper) with the digital bearer token as a real
     member, not merely an analogue. Left open, not renamed:
     under Fran's own ruling the cost of getting it wrong is
     local and revision passes are indefinite.

106. THE LINNAEAN DRILL-DOWN (Fran, rota XXIII) — method
     correction with an unexpected payoff. I had left
     charta_negotiabilis open as "a genus whose NAME may be too
     narrow", i.e. an ABSTRACTION question. Fran: don't attack
     it from abstraction — go Linnaean. What are the species out
     in the wild?
     Collected specimens: bill of lading, warehouse receipt,
     promissory note, cheque, BANKNOTE, bearer bond, theatre
     ticket, cloakroom token, pawn ticket, digital bearer token /
     capability. Plus negative specimens: registered shares, a
     door key.
     THE TREE APPEARED BY ITSELF, deeper and sharper than v0:
     - genus TESSERA (the Roman word is exact — tessera
       hospitalis, frumentaria, militaris): a thing whose
       EXHIBITION CONSTITUTES a right rather than proving it.
       That differentia — constitutive vs evidentiary — is much
       sharper than v0's "possession transfers a right", and it
       is what a receipt fails.
     - sub-genus TESSERA_NEGOTIABILIS: the good-faith purchaser
       takes CLEAN TITLE even from a thief — an EXCEPTION to
       nemo dat quod non habet. THAT is what negotiability
       actually means, and v0 had it wrong by conflating it with
       mere bearer-possession.
     - sub-genus TESSERA_SIMPLEX: possession suffices, but no
       title-cleansing doctrine.
     THE PAYOFF, AND IT REVERSED MY EXPECTATION: from abstraction
     I would have filed the digital bearer token under
     negotiable — "possession confers right, same form". The
     specimens show it shares the MECHANISM but not the
     DOCTRINE: no clean-title rule exists in our systems, a
     stolen token stays stolen. So it belongs under
     tessera_simplex. A division that abstraction alone would
     never have seen, and it answers round XXII's open question
     in an unexpected direction — SPLIT, not rename.
     Two more finds fell out: the BANKNOTE is a member ("I
     promise to pay the bearer on demand" is still printed on
     Bank of England notes) — so pecunia and tessera touch,
     which v0 never saw; and the historical DECLINE of bearer
     instruments (US bearer bonds effectively ended 1982;
     bearer shares banned in many jurisdictions) is explained by
     the differentia itself — title-cleansing helps the thief
     too, so the form died not because it failed but because it
     worked TOO well.
     Negative boundaries recorded because they prove the genus:
     registered instruments are NOT tesserae (the right lives in
     the register, the certificate merely evidences it), and a
     KEY is not one either (it confers power, not right — posse
     vs licere).

104. PHASE NAMED (Fran's recalibration): this is FILLING OUT, not
     perfecting. Revision passes are available indefinitely and
     no division decision can paint the work into a corner.
     Which dovetails exactly with the cheap/expensive asymmetry
     the session had already discovered by instinct: divisions
     are LOCAL (rename, re-parent, split, merge — the gate names
     what broke), metamodel changes are GLOBAL (retrofit every
     model). That is the real reason dimensions were taken before
     nouns, and it is now written down rather than felt.

VERDICT, RUN THREE: the loop keeps finding one structural gap
per run, and they are stacking into a pattern worth naming.
Run one (pistrina): missing NOUNS — whole domains.
Run two (valetudinarium): missing EPISTEMIC dimension — how
sure, from whom.
Run three (forwarder): missing TEMPORAL dimension — true when.
The gaps are getting deeper and rarer, which is the expected
shape: nouns are surface, dimensions are structure. A fourth
run should be expected to yield mostly nouns again — and if it
yields another dimension, that itself is information.

VERDICT, RUN TWO: the loop is not just additive, it is
DIAGNOSTIC. Run one (pistrina) surfaced missing NOUNS — whole
domains the library had never described. Run two surfaced a
missing DIMENSION: every existing model asserts flatly, and no
amount of new genera fixes that. A structural gap of this kind
is exactly what a coverage instrument is for, and it would not
have appeared by adding models in the parish. Second run also
CLOSED an open dubium (fons) by reframing it — the loop
resolves as well as surfaces.

## 2026-08-05 — round twenty-four: identificatio (27 models)

Fran's distinction that generated the model: **programming
languages are singletons** — one C, one Python — so they are
entities in a flat list and want no sub-genera. An identification
SCHEME is different: ISBN is one thing but its instances are
countless. That is exactly where shared properties do real work,
because what is common across all schemes carries payload that
each species fills in.

107. schema_identificandi born with ten species chosen by the
     EXPOSURE test (uuid, ulid, isbn + isbn_10/isbn_13, issn,
     doi, orcid, iban, e164, gtin, uri). Long tail (VIN, national
     ID cards, tax numbers) deliberately deferred per the
     enumeration rule — the escape hatch will handle them when a
     specimen arrives.
108. THE GENUS'S PAYLOAD — properties that turned out to carry
     real bug-prevention, not description:
     - `quid_designat` — the most-neglected fact about any
       scheme. ISBN designates an EDITION, E.164 a LINE (not a
       person), ORCID a PERSON across changed names, GTIN a KIND
       (not an item). Get this wrong and you are counting
       different things than you think.
     - `reassignabilis` — the property e164 DEMANDED: phone
       numbers get recycled, so "same number" ≠ "same person",
       and account recovery by SMS breaks not rarely but
       CERTAINLY, given time.
     - `opacus` — the smart/dumb identifier trade: a smart id
       tells you something and BREAKS when the thing changes
       (publisher sold); a dumb id promises nothing and breaks
       nothing.
     - `praesentatio` — hyphens are CONVENTION not essence, the
       same law suffixes taught in round three; compare two ISBNs
       with hyphens intact and equal identifiers read unequal.
     - `probabilis_solus` — a check digit catches TYPOS, never
       fictions: a well-formed number need not exist.
109. Machine `vigor` found a state software rarely models:
     schemes are CLOSED, not killed — ISBN-10 stopped being
     assigned in 2007 while existing numbers stay valid forever.
110. THE SYNERGY DEMONSTRATED, not just claimed (Fran's point):
     opus_scriptum.editio.index_isbn was `genus="textus"` — a
     bare string that knows nothing. It is now `genus="isbn"
     modulus="identificatio"`, so a model about BOOKS inherits
     check-digit rules, the 2007 transition, the parts breakdown
     and the manifestation-not-work fact for free. Second major
     use of the library-typed-property mechanism (first was
     quantitas), and the first where the payload is domain
     expertise rather than units.
111. Cross-link recorded: a scheme is the MACHINE THAT MINTS
     baptismal identifiers (elementa_programmatis.identificator,
     the Scotus line) — assigned names, never content-derived.
     The two models met without either being bent.
112. Honesty note carried in the model's own fontes: this file
     needs verification more than any other, because its facts
     are PRECISE (check-digit algorithms, digit counts, dates)
     and precision fails fluently. Everything historical marked
     certitudo="fides".

## 2026-08-05 — round twenty-five: THE PARISH (loop, fourth run)

Scenario chosen for a stated reason: the parish is the one domain
where FRAN CAN CHECK THE SCRIBE, and the accuracy caveat says the
scribe is most dangerous where unverifiable. It also had an open
dubium waiting (liturgical calendar). Discussion only; harvest
awaits blessing.

PREDICTION TESTED: after run 3 I predicted run 4 would yield
"mostly nouns, and if it yields another dimension that is itself
information." Result: mostly nouns AND one structural REFINEMENT
of an existing ruling — not a new dimension. Prediction held.

THE FIND — OFFICIUM, and it cuts an established ruling:
"Roles are relations" (round IV, confirmed 4x) needs a division.
A parish has a pastor; when he leaves the office PERSISTS —
*sede vacante* is a named state, not missing data. Duties,
powers and eligibility attach to the POSITION, not the man.
Canon law reifies this (officium ecclesiasticum = a function
constituted in a stable manner, existing independently of any
holder).
→ TEST, crisp: DOES THE POSITION PERSIST WHEN VACANT?
  yes → OFFICIUM, a real thing bundling duties/powers/conditions,
        and "holding" it is the relation;
  no  → a bare role, which remains a relation.
"Author" is a bare role — there is no vacant authorship waiting.
"Bishop of Rome" is an office. Generalises far past the parish:
CEO, chair, trustee, president. Modelling offices as bare
relations is why org systems cannot answer "who SHOULD do this"
when the seat is empty.

HARVEST (rest):
2. PERSONA_IURIDICA — an organizatio that IS a person in law
   (owns, sues, persists). Parishes, corporations, trusts,
   foundations. organizatio currently cannot say it.
3. LEX, distinct from NORMA — a norma is CONFORMED TO (ISO), a
   lex is IMPOSED with sanction. Follows exactly the line
   magistratus already drew (auctoritas ex imperio vs ex
   consensu) — a split the library should arguably have made
   in round XIV.
4. RITUS + SACRAMENTUM — praescriptum-shaped (prescribed,
   ordered, executed by persons) but its output is a CHANGE OF
   STATE, not an artefact. Either praescriptum.efficit
   generalises or ritus is a sibling — real tension, not
   cosmetic. sacramentum adds materia / forma / minister /
   recipiens / effectus / repeatability, and the beachhead
   already exists (titulus.character_indelebilis).
5. RES_SACRA — things set apart by consecration, with genuinely
   different rules (alienation restricted, disposal regulated).
6. RECURRENTIA_COMPUTATA — CLOSES the dubium open since round
   XIV: Easter is computed from a lunar rule, which simple
   periodicity cannot express.
7. Smaller: parish membership is by TERRITORY, not by choice —
   a bend in organizatio.membrum as modelled; status_vitae
   (married / religious / ordained) noted, may be covered by
   titulus + character.

CONFIRMATION WORTH ITS OWN LINE — A THIRD INDEPENDENT ANCESTOR:
parish registers (baptism, marriage, burial) became universally
required at Trent (1563): append-only, corrections as marginal
annotations never erasures, among the longest continuous event
logs in Europe. That makes THREE domains — Pacioli's ledgers
(round XIV), medical records (round XVII), parish registers —
arriving independently at "the log is truth, status is a fold",
each for the same reason: rewriting destroys evidentiary value.
A doctrine reached three times by unrelated professions is not
a design choice; it is a discovery.

Other confirmations: iurisdictio (parish territory), titulus +
character_indelebilis, calendarium_liturgicum, obligatio,
organizatio.pars_de (diocese/province hierarchy).

## 2026-08-05 — round twenty-six: actus sollemnis (28 models)

Fran: build both the parent and ritus, then articulate the legal
collection that surfaced anyway.

113. THE TENSION RESOLVED PROPERLY. My first framing was too
     narrow — "a recipe makes an artefact, a rite makes a
     state-change" — which would have needed only a more general
     `efficit`. Worked through, FIVE differences appear and they
     CLUSTER, which is what makes a genus rather than a variant:
     (I) VALIDITY vs QUALITY — a badly-made loaf is still bread;
         a defective baptism is not a bad baptism, it is nothing.
         Binary threshold vs gradient.
     (II) CAPACITY vs SKILL — praescriptum already has
         peritia_postulata; an unskilled bishop ordains validly,
         a skilled layman does not ordain at all.
     (III) INTENTION AS CONSTITUENT — bread does not care what
         you were thinking; the rite requires an interior act.
         An ingredient nobody can observe.
     (IV) FORM AS DETERMINING, not a step among steps.
     (V) Some are unrepeatable by nature (character).
114. THE ESCAPE HATCH, AGAIN, AND IT GENERALISED THE GENUS: the
     specimens of "valid-or-void, qualified agent, form-
     dependent" are NOT religious — will, notarial deed,
     verdict, promulgation, oath of office, conferral,
     mancipatio, stipulatio. So ritus is a SPECIES of
     actus_sollemnis, and the parent earns its place for a
     library reason rather than a religious one:
     **WE HAD MANY INSTITUTED THINGS AND NO ACCOUNT OF HOW ANY
     OF THEM COMES TO BE.** officium, titulus, licentia,
     obligatio, possessio, contractus all begin at some moment
     by some act. `constituit` is the cardinal relation of the
     model, and it is why the specimens keep landing on genera
     we already had.
115. RESCISSIO vs SOLUTIO — the machine's real payload, and a
     classic confusion: rescission declares the act NEVER
     valid (retroactive, effects evaporate); dissolution says it
     WAS valid and now ends (past stands). Annulment vs divorce;
     void vs terminated contract. A schema with one "cancelled"
     field loses both, and cannot answer "what about what
     happened in between?".
116. Specimens chosen because they show FORM PURELY:
     - stipulatio — nothing handed over, nothing written, full
       obligation from the WORDS ALONE. Austin described
       performative utterance after two thousand years; Roman
       law was already using it.
     - mancipatio — the bronze and scales survived the reason
       for them (real weighing → symbol once coined money
       existed): form outliving its rationale and still working.
     - baptism — measures CAPACITY across the genus: in
       necessity ANYONE baptises validly, while only a bishop
       ordains. Proof that capacity is a property of the ACT,
       not of the genus.
     - testamentum — forced `revocabilis`: mutable until death,
       then abruptly immutable; the one act whose force depends
       on the agent's death, and why its formalities are so
       strict (the principal witness cannot be questioned).
     - inauguratio — the clearest witness for OFFICIUM: the act
       does not change the person, it JOINS them to an office
       that already existed, vacant.
     - promulgatio — an honest circle: a solemn act is performed
       according to law, and law begins by a solemn act. The
       regress does not run forever; it rests in a constitution.
117. SUBTLETY RECORDED: iudicium.sententia_iudicis is the
     PROPOSITION (what the judge holds); actus.sententia_lata is
     the ACT that gives it force. The same sentence in the
     judge's mind binds nobody; pronounced, it creates an
     obligation. Proposition and act are two things — and the
     library can now say so, which it could not before this
     model.

Measured: 28 exemplaria / 121 genera / 251 residents / 327 arcs
/ 0 vulnera. Model deliberately CREATED THREE UMBRAE (officium,
lex, res_sacra) rather than half-describing them inline — the
agenda stays computed.

## 2026-08-05 — round twenty-seven: officium + persona_iuridica

118. OFFICIUM built (organizatio v5) — the round-XXV structural
     find, and it DIVIDES the roles-are-relations ruling rather
     than overturning it. Both halves stay true: the office is a
     THING, holding it is a RELATION. The test is written into
     the genus's own nota so a reader cannot miss it — does the
     position persist when vacant? "Author" fails (no vacant
     authorship waits); "bishop of Rome" passes.
     `vacans` is the primary state, not an absence — and the
     locution "sede vacante" is itself the argument: THE SEAT is
     vacant, not the man, and languages that borrowed the phrase
     borrowed the ontology with it.
119. TENERE vs EXERCERE separated — a vicar/administrator
     EXERCISES an office he does not HOLD. Without that
     distinction there is no way to say who acts during a
     vacancy, which is exactly when someone must.
120. `confert_capacitatem` closes the loop with actus: an office
     is where the CAPACITY for solemn acts comes from (notary →
     public instrument, bishop → ordination). Capacity from
     office, never from skill — the round-XXVI differentia now
     has its source described.
121. PERSONA_IURIDICA built, with the distinction modern schemas
     lose: universitas PERSONARUM (a body of members) vs
     universitas RERUM (goods devoted to a purpose, WITH NO
     MEMBERS — a foundation). Anyone who models legal personhood
     as always-a-group cannot say "foundation", and foundations,
     trusts and pious causes are everywhere.
     Recorded in its nota: Innocent IV's "persona ficta" is the
     received phrase, but moderate realism grants more — an
     ordered union acting as one and enduring through membership
     change is a REAL instituted kind, like ISBN or officium. It
     would be a fiction only if nothing in things answered to it;
     the order itself answers.
122. FALSE UMBRA caught by the index again (third instance of
     this exact shape): identificatio pointed `administrata_a`
     at `organizatio` — a GENUS THAT EXISTS — and marked it
     externum. What is actually missing is an individuum, the
     International ISBN Agency. Retargeted to `agentia_isbn`.
     Lesson worth keeping: `externum` must name the thing that
     is missing, not the kind of thing it is.

Measured: 28 exemplaria / 123 genera / 258 residents / 335 arcs
/ 0 vulnera.

STILL OUTSTANDING from the parish scenario:
- lex (distinct from norma — imposed with sanction vs conformed
  to; the split magistratus already implied)
- res_sacra (things set apart; alienation restricted)
- recurrentia_computata (CLOSES the round-XIV dubium: Easter is
  computed from a lunar rule)

## 2026-08-05 — round twenty-eight: the parish queue closed + addressing

123. LEX built (norma v2) — the split magistratus implied two
     rounds earlier and this model failed to follow. Definitio
     from Thomas (ST I-II q.90 a.4). The differentia that
     matters operationally: a NORMA HAS NO BORDERS (ISO is the
     same everywhere), a LEX DOES — outside its jurisdiction it
     does not bind. Machine gained two states software schemas
     usually lack: VACATIO (promulgated but not yet binding, so
     it can be known before it obliges) and DESUETA (killed by
     contrary custom, with no act at all — a law that use, not
     a legislator, ends). Species by Thomas's own fourfold
     source-division; lex_humana carries the conclusion-vs-
     determination distinction that explains why some laws agree
     worldwide and others diverge by jurisdiction — not
     inconsistency but two modes of derivation.
     consuetudo BENDS our machine honestly: it is never
     promulgated, it GROWS. Dubium carried, not papered over.
124. RES_SACRA built (corporalia v3) — status by ACT, not by
     matter: the same wood is a table or an altar, and the whole
     difference is the consecration. Purest test yet of the
     actus doctrine. Its VIOLATA state is one the library could
     not previously express: INJURY WITHOUT PHYSICAL DAMAGE — a
     desecrated church stands intact and may not be used. Noted
     as the boundary of the triad of defects rather than a
     fourth member: some things are measured by more than one
     norm at once. reliquiae are the library's only thing that
     may be possessed and NEVER sold.
125. RECURRENTIA_COMPUTATA built (tempus v3) — CLOSES the dubium
     open since round XIV. And the escape hatch generalised it
     again: the specimens are not religious. dies_negotiorum
     ("net 30 business days") needs a computed rule that
     CONSULTS ANOTHER RECURRENCE (the holiday calendar), and
     holidays vary by jurisdiction — so the same contract clause
     means different dates in two countries.
126. ADDRESSING ANSWERED AND MEASURED (Fran's tangent): an
     entity's address is `modulus` + `nomen`, flat — which is
     what internal references already use. Measured: 381 names,
     ZERO collisions. So `/persona/dennis_ritchie` works today.
     Three non-obvious consequences written into METAMODULUS
     §4b: rank does not appear in the address (a citation refers
     to a THING; kind-or-instance is learned on arrival);
     NESTING IS DISPLAY, NOT ADDRESS (rosa_canina is
     /planta/rosa_canina, four levels notwithstanding — which
     also means re-parenting, which the coining rules encourage,
     does NOT break addresses); and hyphen-vs-underscore is
     presentation, not essence.
127. RULE 15 PROMOTED [S] → [E]: uniqueness is what makes
     addressing work, so it is now gated, not hygiene.
     Negative-tested.
128. OPEN QUESTION RECORDED, not solved — RENAME STABILITY.
     "Divisions are cheap and local" is true INSIDE the library;
     this session renamed charta_negotiabilis → tessera on that
     licence. Once app volumina cite addresses, a rename breaks
     consumers — ordinary API stability, and Eskil's
     interface-first rule applies to a semantic library exactly
     as to a C header. Pre-publication, renames stay free. At
     first external citation one of two disciplines must begin:
     freeze published names, or forward via a tombstone naming
     the successor (norma.substituit and identificatio.successit
     already suggest the shape). NOT built — no consumer exists,
     and inventing mechanism ahead of content breaks Alexander.

Measured: 28 exemplaria / 126 genera / 269 residents / 343 arcs
/ 0 vulnera. Parish scenario fully built; nine gates enforced.

## 2026-08-05 — round twenty-nine: rank audit + the particular-marker

Fran asked whether we conflate things and kinds, guessing that
scholastic "species" is thing-like where taxonomic species is a
kind. The answer sharpened into two findings.

129. CLARIFICATION: species is a KIND in BOTH senses (a
     predicable, said of many). What is real behind the
     intuition: (a) the *species specialissima* is the last
     universal before individuals, so it sits closest to the
     essence and feels concrete; (b) Latin *species* also means
     appearance/form, a different sense entirely.
     THE LOAD-BEARING POINT, and it is Porphyry's own: GENUS AND
     SPECIES ARE RELATIVE, not absolute ranks — everything
     mid-tree is a subaltern genus, genus toward what is below
     and species toward what is above (our `rosa` is both).
     So: genus/species = POSITIONAL, moves on re-parenting.
     Universal/particular = ONTOLOGICAL, does not move.
     Our three element names read as three peers; there are
     really TWO categories, the first internally graded.
130. THE ACTUAL CONFLATION WAS IN OUR DATA, not our vocabulary.
     Test: IS IT PREDICATED OF MANY? `euro` was filed as an
     individuum of moneta (right — one particular currency), but
     `c`, `python`, `macos`, `x86` were filed as SPECIES though
     there is exactly one of each and nothing is "a C".
     Structurally identical to euro, filed differently.
     THIRTEEN re-filed (c/python/rust/bash;
     macos/linux/windows/debian/ubuntu/arch; x86/arm/riscv).
     Genuinely species-shaped entries pass the test untouched:
     many bills of lading are charta_onerum, many strings an
     isbn, many infections morbus_infectiosus.
     Fran had ALREADY said this two rounds earlier ("programming
     languages are kind singletons") — quoted in the
     identificatio header and never acted on. Recorded as a miss.
131. THE CORRECTION PROVED THE ADDRESSING DESIGN BY ACCIDENT:
     343 arcs before, 343 after, zero vulnera. Thirteen entities
     changed RANK and not one citation broke, because the
     resolver keys on modulus+nomen across genera and residents
     alike. Had rank been load-bearing in addresses, that same
     correction would have broken every citation to C, Python,
     macOS and x86.
132. FRAN'S RULING on the marker: only PARTICULAR THINGS get
     flagged — `/persona/:dennis_ritchie`, but `/actus/stipulatio`
     bare, since species are kinds too. Recorded in METAMODULUS
     §4b as a CHECKED HINT rather than a resolution key, with
     today's thirteen-fold correction as the measured
     justification. Not yet exercisable: no external consumer
     cites addresses.
133. FRAN'S RULING on promotion — DO NOT PROMOTE, ADJOIN. When a
     thing acquires sub-things, coin a SIBLING genus and relate,
     leaving the thing a thing. The library already did this
     once without naming it: C did not become a genus when
     versions appeared; versio_linguae was coined. Dialects take
     the same shape (dialectus_linguae, generic).
     REASON: "GNU C is a dialect of C" is DERIVATION,
     "Socrates is a man" is PREDICATION — promotion silently
     converts one into the other.
     TEST recorded: are the would-be members predicated of it,
     or merely related to it? Related → sibling genus. Truly
     predicated → it was a kind all along and you are CORRECTING
     a misfiling, not upgrading. Three cases converge (versions,
     implementations, dialects), and libc's open dubium resolves
     the same way: glibc IMPLEMENTS libc, so
     implementatio_bibliothecae, not promotion.
     CONSEQUENCE I OWED FRAN HONESTLY: this makes rank far more
     stable than I had argued against the colon — if upgrades
     are avoided by doctrine, only corrections remain. The
     checked-hint recommendation survives, but on the weaker
     ground that corrections happen (thirteen today) and a
     verified hint costs nothing.
134. TWO CASES LEFT OPEN as dubia rather than forced:
     - unix: the predication test passes BOTH ways here alone
       ("AIX is a unix" is true, and Unix is also a particular
       Bell Labs system). Possibly two entities, as language and
       version are two.
     - pascha: one paschal RULE (individuum) but "this year's
       Easter" predicates; the rule/occurrence split may be the
       residence rule again.

Measured: 28 exemplaria / 126 genera / 269 residents / 343 arcs
/ 11 dubia / 0 vulnera.

## 2026-08-05 — round thirty: unix split + the terminology decision

135. UNIX DUBIUM RESOLVED Fran's way, and the resolution is the
     promotion rule applied: the predication test passed BOTH
     ways there alone BECAUSE THERE WERE TWO THINGS.
     → genus `systema_unixiforme` sub systema_operativum (the
       family: AIX, Solaris, BSD, macOS are all truly said to
       BE Unixes) with the differentia stated as FORM not
       genealogy — one tree from a root, everything-is-a-file,
       composable processes, the shell as an ordinary program.
       Which is why both true descendants (macOS) and
       reimplementations (Linux) belong, with orta_ex vs
       ad_exemplum_facta doing the distinguishing.
     → individuum `unix_bell_labs` — the particular 1969 system.
       QUALIFIED NAME, because usage says "a Unix" of the family
       far more often than of the original, so the genus earns
       the plain name.
     macos and linux moved under the new genus (correct
     taxonomy). Genealogical relations retargeted at the
     PARTICULAR, which is historically right: macOS descends
     from real Bell Labs code, Linux was built after its example.
     Arcs 343 → 343 again: a genus was created, three entities
     re-parented, and zero citations broke. Third demonstration.
136. TERMINOLOGY DECIDED — two registers, because no single word
     serves both:
     - artifacts keep `individuum` (Porphyry's own; literally
       "undivided"; what lies below the species specialissima);
     - English prose says "particular" (the standard
       universals-and-particulars term, no person-connotation).
     Rejections recorded with reasons, since they are the useful
     part: `particulare` is a bad LATIN choice because in
     scholastic logic "particular" means QUANTIFICATION (some vs
     all), not individuality; "entity" is bad because in ER
     modelling it means type-or-instance depending on the
     speaker — importing the exact confusion we are removing;
     "instance" is bad because it imports OOP vocabulary, and
     this library's whole framing is the taxonomy layer NOT the
     implementation layer. `singulare` was the one real
     alternative (universale/singulare; harmonises with Scotus's
     haecceitas, already cited in identificator) — rejected only
     as a rename with no functional gain.
     Noted: the connotation gap is real. Our individua split
     into person-like (Ritchie, ISO, Moby-Dick) and
     artefact-like (c89, x86, euro, macOS); both are
     particulars, only the first sound like "individuals".

Measured: 28 exemplaria / 127 genera / 269 residents / 343 arcs
/ 10 dubia / 0 vulnera.

## 2026-08-05 — round thirty-one: the marker strengthened

137. Fran pressed: individuum-ness is stable, so should the
     marker be visible at a glance by construction? Position
     moved one notch — from OPTIONAL hint to MANDATORY hint,
     still ignored in resolution. Three properties chosen
     together: mandatory (at-a-glance GUARANTEED — an optional
     marker fails precisely here, since a bare name might mean
     "kind" or might mean "lazy author"); ignored in resolution
     (a correction flags stale rather than killing the link);
     checked both directions by the gate.
138. LOAD-BEARING REJECTED, on the project's own value rather
     than on convenience: if rank were part of the key, every
     correction of a misfiling becomes a BREAKING CHANGE, so the
     schema would quietly pressure us to leave `c` filed as a
     species because fixing it is expensive. A mechanism that
     penalises truth-correction fights the discipline it exists
     to serve. Thirteen such corrections happened today alone.
139. AND THE TEMPTING BENEFIT REFUSED: a load-bearing marker
     would let a genus and a particular share a name
     (/systema_operativum/unix vs :unix), sparing the
     unix_bell_labs rename. Refused because it PRESERVES an
     ambiguity rather than resolving it — the rename made the
     distinction visible in prose, in the index, in
     conversation; a sigil makes it visible only where the sigil
     appears.
140. Made exercisable today in the one place addresses actually
     appear: INDEX.md dictionary lists now render individua with
     the colon (`:c`, `:python`, `:unix_bell_labs`), so the
     at-a-glance property is real now rather than promised.

141. COROLLARY Fran drew, and it is the right one: because
     resolution ignores the colon, `/x/unix` and `/x/:unix` are
     THE SAME NAME — hence a collision, caught by rule 15. The
     marker informs but cannot DISCRIMINATE.
     That is discipline, not limitation: a namespace split would
     let two things called "unix" coexist forever, distinguished
     only where a sigil appears, while colliding in prose, in
     indexes and in speech. Forcing the rename resolves the
     ambiguity everywhere rather than hiding it in punctuation.
     VERIFIED rather than asserted: a genus `macos` beside the
     individuum `macos` is rejected — "bis definitum (allocutio
     ambigua)".

## 2026-08-05 — round thirty-two: PM/DEVOPS (loop, fifth run)

Fran's scenario: project manager / devops for a software product.
Chosen well — ADJACENT to our parish but stressing what the
parish never touched (work, plans, environments, operations).
Discussion only; harvest awaits blessing.

TWO HOLES FOUND BEFORE THE WALK EVEN STARTED:
142. **PROGRAMMA DOES NOT EXIST.** And `bibliotheca` DEFINES
     ITSELF AGAINST IT: "a programmate differt quia initio
     proprio caret - vivit vocata, numquam sponte currit."
     Twenty-eight models in, we have been defining a genus by
     contrast with a genus we never described. The most ordinary
     thing in the parish, missing. A reminder that the loop
     finds holes introspection does not: nobody re-reads a
     definitio asking "does the thing I contrasted with exist?"
143. `causa` and `ambitus`: ZERO occurrences in the whole corpus.

HARVEST:
1. PROGRAMMA — that which has its own beginning and runs of
   itself, against a bibliotheca which lives only when called.
2. THE MACHINE-EXECUTED PROCEDURE (the round's structural find).
   praescriptum is defined *ab homine exsequendus*, and its
   licence to carry ambiguity ("coque donec aureum fiat") exists
   BECAUSE a human judges. A CI pipeline is the identical shape
   — ordered stages, declared inputs, failure at any step — with
   the opposite executor and therefore ZERO ambiguity tolerance.
   The ritus/praescriptum split MIRRORED: that sibling differed
   by having MORE than skill (capacity, intention); this one by
   having LESS (no judgment at all). Parent genus is an ordered
   procedure; the differentia is who executes, and what that
   permits.
3. AMBITUS (environment) with an ontological differentia rather
   than a technical one: dev/staging/production differ not by
   hardware or configuration but by REALITY OF CONSEQUENCE. A
   mistake in production is real.
4. PENSUM (work item) — not any of fabricatio's four: a
   postulatum is what must be true, a facultas what the product
   can do, a vitium a divergence. A ticket is a COMMITMENT TO
   ACT, with assignee, state and estimate. Its planned-vs-actual
   gap is the PM's whole job and is measurable.
5. CAUSA — a genuine vocabulary hole. The library causes things
   constantly (parit, constituit, illatum_per, factum_a,
   orta_ex) with no genus for cause, so a post-mortem's "root
   cause" is expressible as a JUDGMENT but its CHAIN is not.
   And the praedicamenta echo returns: ARISTOTLE'S FOUR CAUSES
   ARE ALREADY DISTRIBUTED THROUGH THE LIBRARY UNNAMED —
   material in ex_materia, formal in differentia, efficient in
   factum_a/constituit, final in every definitio that says
   "ad finem".
6. INCIDENT — NOT a kind of bug. The same defect is a vitium
   always but an incident only once it reaches users: a vitium
   in a RELATION OF EXPOSURE, plus duration and severity. Open
   question whether that makes it a genus or a relation.

CONFIRMATIONS (several unexpected):
- MONITORING IS OBSERVATIO: an alert is a `signum`
  (instrument-measured, verifiable by anyone) crossing a
  threshold — the clinic's symptoma/signum cut doing work in a
  datacenter, two rounds after it was coined for a fever.
- ON-CALL IS OFFICIUM WITH ad_tempus: you HOLD the on-call
  office for a shift, and the tenere/exercere split covers
  escalation exactly.
- runbook = praescriptum (human-executed, judgment permitted);
  estimation = iudicium.aestimatio, whose "three appraisers,
  three prices — not error but the nature of judgment" needed no
  adjustment for three engineers and three estimates; SLA =
  obligatio + mensura + periculum; releases take the versio
  pattern; rotations = horarium; build artifacts =
  artefactum_aedificationis.
- NON-SURFACING recorded: backlog priority is an ORDERING, not
  a thing — third of its kind after merx (round XIII) and triage
  (round XVII).

VERDICT, RUN FIVE: nouns plus one structural ABSENCE (programma)
and one vocabulary gap (causa) — no new dimension, consistent
with the prediction after run three that dimensions get rarer
and deeper. The novelty this run is the KIND of finding: not
"we never modelled X" but "we defined something against an X we
never modelled", which only a scenario walk exposes.

## 2026-08-05 — round thirty-three: PM/devops harvest built (30 models)

144. FRAN'S ADDRESSABILITY TEST — the round's doctrine, and the
     most practical criterion yet because it is EMPIRICAL:
     "anything that can have individua you would want to address
     by ID unambiguously is worth making a genus."
     The load-bearing half is *can have individua* (ontological);
     ID-assignment is the EVIDENCE, not the justification — if
     practitioners routinely give these things identifiers, that
     is evidence they are real enough to pick out singly.
     IT REPRODUCES EVERY REFUSAL THE LIBRARY MADE ON OTHER
     GROUNDS, which is why it is trustworthy: incident passes
     (INC-1234), merx fails (you address the LOAF), triage fails,
     backlog priority fails. And it EXPLAINS them: all three are
     relations or orderings, and RELATIONS DO NOT GET IDS —
     THEIR RELATA DO.
     Deep form recorded: a relation CAN earn one (a marriage has
     a certificate number), and when it does it has been REIFIED
     into an entity — the same move pretium made against
     valid-time attributes. Refinement recorded: read "individua"
     as *members worth picking out*, since gradus_assensus and
     formae_datorum pass at SPECIES level with no individua at
     all.
145. PROGRAMMA built — the hole that had hidden for twenty-eight
     models while `bibliotheca` defined itself against it. Its
     differentia is the one bibliotheca already implied: it has
     its OWN BEGINNING and runs of itself. Species
     instrumentum_lineae / daemon / applicatio, split by
     modus_finiendi (terminans vs perpetuum), which is the
     practical difference that matters.
146. PROCEDURA parent built + PROCEDURA_AUTOMATA sibling, with
     praescriptum re-parented under it. The differentia is the
     ritus split MIRRORED: praescriptum may carry ambiguity
     BECAUSE a human judges; the automated procedure may carry
     NONE, because nothing judges. What is trust in one is a
     defect in the other. In exchange it gains what a human
     cannot give: infinite repetition with identical effect.
     porta_humana names the seam where an automated procedure
     becomes an ACTUS SOLLEMNIS — an approval needs capacity,
     not skill. retentatio carries the honest note that a retry
     often SUFFICES, and that "intermittent failures" are a
     hidden vitium wearing a property's clothes.
147. CAUSA built as its own model, and the praedicamenta echo
     returned a third time: ARISTOTLE'S FOUR CAUSES WERE ALREADY
     DISTRIBUTED THROUGH THE LIBRARY UNNAMED — material in
     ex_materia, formal in differentia/definitio, efficient in
     factum_a/constituit/parit, final in every "ad finem". The
     model NAMES them rather than replacing the relations.
     Two load-bearing notes: (a) necessary and sufficient RARELY
     COINCIDE (a spark is not sufficient without oxygen, nor
     necessary when lightning would do), so a single "root
     cause" is nearly always a simplification, and the chain
     does not end by itself — THE INVESTIGATOR STOPS IT;
     (b) machines find ANTECEDENTS, humans assert CAUSES —
     causa.asseritur_per → iudicium, so a root cause carries an
     author and a degree of assent.
     morbus's final cause is cited: only what has an END can be
     sick, which we used tacitly in round XVII.
148. OPERATIO built (ambitus / pensum / incidens).
     - ambitus's differentia is ONTOLOGICAL not technical:
       environments differ by REALITY OF CONSEQUENCE, which is
       why "it worked in staging" is a true sentence and not an
       excuse.
     - pensum is the fourth thing fabricatio's square lacked: a
       COMMITMENT TO ACT, distinct from what must be true
       (postulatum), what the product does (facultas), and a
       divergence (vitium). Its `omissum` state is an honest
       outcome that trackers usually hide, as if every ticket
       must be finished.
     - incidens got Fran's ruling with the criterion attached:
       a genus, because incidents have individua we address by
       ID. Its `mitigatum` state is where operations actually
       live and binary open/closed schemas lose it. Its
       recidivum cycle is the THIRD domain of one law, after
       morbus (relapse) and fabricatio (regression).
       causa_eius is MULTIPLEX deliberately, for the reason
       causa gives.

Measured: 30 exemplaria / 134 genera / 281 residents / 364 arcs
/ 0 vulnera.

## 2026-08-05 — round thirty-four: three modes of membership

149. FRAN'S DISTINCTION, and it names something the library has
     been doing in three ways without noticing: HOW A THING
     COMES TO BELONG to a genus is not one story.
     - PER NATURAM: read off the thing (a rock is a rock).
       "Tautological" in Fran's word. planta, animal, corporalia.
     - PER IUDICIUM: ASSERTED by someone competent, revisably.
       morbus is diagnosed, vitium judged, incidens declared.
     - PER ACTUM: CONFERRED by a solemn act. titulus, officium,
       licentia, matrimonium, persona_iuridica — the whole actus
       model is this mode.
     Third instance of the same pattern (after the praedicamenta
     and the four causes): the library already operated a
     distinction it had never stated.
150. AND fabricatio HAD ALREADY RECORDED THE CONSEQUENCE without
     the vocabulary: "non vitium sed facultas est" is a dispute
     about GENUS ASSIGNMENT — identical behaviour, contested
     kind, resolvable only by consulting intention. That is a
     per-iudicium membership dispute, written in round XIV.
151. FRAN'S ARGUMENT FOR WHY THIS MATTERS, recorded as the
     strongest case yet for the library's practical value: much
     EXPERTISE IS A REPERTOIRE OF APPLICABLE CONCEPTS. A novice
     sees code that is annoying to change; someone with the
     vocabulary sees DEBT, and the word immediately licenses
     moves (estimate principal, weigh interest, service or
     repay). Per-iudicium genera are exactly where that pays.
     Checked against Pieper and it holds: an applied concept
     must still be REAL — the expert RECOGNISES a genuine
     pattern rather than inventing one. Only the mode of
     establishing membership changes, not the requirement of
     truth.
     Modelling consequence recorded: a per-iudicium genus should
     carry `asseritur_per → iudicium`, so membership has an
     author, a moment and a degree of assent, and two experts
     may disagree without either being wrong.
152. DEBITUM_TECHNICUM built (fabricatio v2) on exactly that
     footing, and it carries a Pieper catch in its own name:
     - it is NOT a defect but a TRADE — value taken now against
       a price paid later, which is why the financial metaphor
       works at all (there is good debt and bad debt);
     - THE POPULAR SENSE IS A CORRUPTION. Ward Cunningham, who
       coined it in 1992, meant shipping what reflects your
       CURRENT understanding and refactoring as understanding
       grows; he said explicitly he never favoured writing code
       poorly. So deliberate imperfection with a known price is
       debt; ignorant or careless mess is a vitium or bad craft.
       The term drifted precisely because it EXCUSES ("we have
       debt" sounds better than "we wrote it badly") — abuse of
       language in our own domain.
     Its `usura` property carries the operative truth: INTEREST,
     not principal, drives the decision — debt with no interest
     need never be repaid. Its `emit → facultas` relation makes
     the judgement honest: debt that bought no capability is not
     debt but pure damage. And `servitum` is named as the state
     most debts live in PERPETUALLY, which trackers rarely admit.

Measured: 30 exemplaria / 135 genera / 281 residents / 367 arcs
/ 0 vulnera.

153. FRAN ON NAMING AS LEVERAGE — completes the value
     proposition, and isolates why per iudicium is the PURE
     case: it is the only mode where naming changes NOTHING
     about the world and EVERYTHING about your powers. Per
     naturam adds description (the rock was already a rock);
     per actum is efficacious but requires AUTHORITY; per
     iudicium requires only COMPETENCE and still changes what
     you can do. The naming effect is isolated there.
     MECHANISM, in this format specifically: a genus is not a
     label, it carries machina + proprietates + relationes +
     actiones, so applying one imports an APPARATUS. Judging
     "this is debt" inherits a state machine, an interest
     question, and "what did it buy?". NAMING HANDS YOU THE
     NEXT QUESTIONS — which is what expertise looks like from
     outside: not more facts, but knowing what to ask next.
     LINEAGE + DIAGNOSIS: Minsky's frames, Schank's scripts,
     1980s expert systems. They failed to scale because THEIR
     FRAMES WERE TASK-SHAPED; ours are PIEPER-SHAPED. A
     "restaurant visit" frame does not compose with a "hospital
     admission" frame, because each encodes a use case; frames
     built for what things ARE compose because reality does.
     Measured evidence already in hand: periculum, written for
     a clinic, exercised unchanged by freight insurance a round
     later.
     FAILURE MODE recorded, since leverage has a direction:
     misapplied vocabulary imports the WRONG apparatus — call a
     design error "technical debt" and you inherit moves that
     say manage and service what you should simply FIX. It
     misleads COMPETENTLY, which is worse than confusion.
     Hence the sharpest formulation of the session:
     VOCABULARY AND JARGON ARE BOTH APPLIED; THE DIFFERENCE IS
     WHETHER THE THING IS REALLY THERE. Jargon imports
     affordances without warrant — the actual subject of
     Pieper's book. So the realism constraint is not decoration
     beside the practical value; it is what keeps the practical
     value from becoming its counterfeit.
     BOTH HALVES OF THE VALUE PROPOSITION NOW STATED: per
     naturam material saves a consumer from getting STRUCTURE
     wrong (ISBN, recurrence, valid time); per iudicium material
     gives them LEVERAGE THEY LACKED. Different mechanisms,
     both real.

## 2026-08-05 — round thirty-five: COMPLICATIO — the first genus
## generated from experience rather than inventoried

154. Fran's own term, defined precisely: "an implementation
     consideration you cannot figure out in the planning process
     but which only surfaces once implementation begins, and
     that can require returning to or revising the plan."
     MILESTONE: every prior genus was INVENTORIED — Rome had
     obligatio, FRBR had the four levels, Porphyry had the tree,
     medicine had symptoma/signum. This one was PERCEIVED
     THROUGH WORK and named by the practitioner. It is the test
     case for whether the library can host generated concepts,
     which Fran named as the novel opportunity.
155. THE DIFFERENTIA IS EPISTEMIC, and it is where all the
     leverage lives: NON PRAEVISA vs NON PRAEVIDENDA —
     unforeseen vs UNFORESEEABLE. An oversight could have been
     caught and was not (a failure of planning, curable by
     better planning); a complication could NOT have been
     caught, and more planning would have bought nothing.
     LATIN EARNS ITS KEEP HERE: the gerundive carries in one
     morpheme what English needs a periphrasis for ("was not
     foreseen" vs "could not have been foreseen"). First time
     the language choice has done semantic work rather than
     cultural work.
156. THE LEVERAGE, written into the genus because it IS the
     reason to coin it: the two conditions have OPPOSITE
     remedies. Oversight → plan BETTER. Complication → revise
     CHEAPER (shorter cycles, later commitment, lighter plans).
     Confusing them inverts the response: a team that treats
     complications as oversights demands MORE up-front
     planning, which makes complications surface LATER and
     revision COSTLIER. The error feeds itself, and the word
     alone breaks it — "that was a complication, not a miss" is
     a sentence that changes a retrospective's conclusion.
157. EVIDENCE OF REALNESS, and the strongest kind: THE CONCEPT
     GENERATED AN INSTRUMENT. Fran's simulate-work practice
     (pretend to implement, report every "five minutes") exists
     precisely to surface complications CHEAPLY before real
     commitment. A concept that produces a method is doing work,
     not decorating.
     Recorded in the machine: latens → aperta happens BY ACTION
     ALONE — neither time nor thought effects it, which is why
     instruments that simulate action pay.
158. PIEPER RISK NOTED IN THE ARTIFACT, since generated terms
     have no external check: "complicatio" can decay into an
     excuse exactly as "technical debt" did — a shield over a
     miss. The guard is the `praevidenda` property, which is a
     JUDGMENT and revisable: an honest review sometimes returns
     "no, that was foreseeable." Stated as doctrine in the
     dubium: A GENUS THAT CAN NEVER FALSIFY ITSELF IS JARGON,
     NOT VOCABULARY.
159. Standard for generated genera made explicit (it was already
     our informal practice): ONE INSTANCE IS AN OBSERVATION, TWO
     IN UNRELATED DOMAINS IS A LAW. ferrum modum imperat waited
     for a bakery oven AND a freight vehicle; the triad of
     defects waited for damnum to force a comparison with two
     existing genera. Generated concepts need MORE evidence than
     inventoried ones, not less.
160. SURFACED: `consilium` (plan) — complicatio is defined
     against it and we do not have it. postulatum and pensum are
     its parts; the whole is missing.
     ALSO NOTED: the library has been generating knowledge all
     along and FILING IT WHERE IT CANNOT BE USED — the sliding
     law, the triad, ferrum modum imperat, the declared-channel
     scale invariance, the three modes of membership all live in
     PROSE (worklog, file comments), not in the taxonomy. They
     pass Fran's own addressability test (we cite "the sliding
     law" by name across a dozen rounds). Whether found laws
     should become library content is now an open question worth
     its own round.

Measured: 30 exemplaria / 136 genera / 281 residents / 369 arcs
/ 0 vulnera.

## 2026-08-05 — round thirty-six: cross-pollination + the
## sensemaking mode (Fran's two observations)

161. CROSS-POLLINATION — Fran's thesis: each domain elaborates
     ONE thing exceptionally well because that thing is
     unusually visible there; the same pattern recurs elsewhere
     poorly solved, and people painfully reimplement e.g. "legal
     contract" from first principles. A universal taxonomy makes
     the best-elaborated version available to every domain.
     ALREADY MEASURABLE, seven instances in this corpus:
     obligatio (Roman law → software licensing, SLAs,
     reservations); gradus_assensus (scholastic epistemology →
     diagnoses, estimates, root causes, our own dictionary
     dates); FRBR (library science → BENT our versio pattern
     into something better); Pacioli (accounting → a
     five-century ancestor for the volumen doctrine); tessera
     (merchant law); morbus's latent/chronic/relapse (medicine →
     the same shape as regression); officium (canon law → org
     software).
     AND IT HAS RUN BACKWARDS ONCE: merchant law's
     negotiability (clean title to a good-faith purchaser)
     revealed that a bearer TOKEN LACKS that doctrine — a
     commercial-law distinction yielding a security insight.
162. WHY a domain elaborates one thing — the addition that makes
     the thesis SEARCHABLE: A DOMAIN ELABORATES WHAT IT IS
     PUNISHED FOR GETTING WRONG. Law elaborated obligation
     because failure means injustice; medicine elaborated
     diagnosis because failure kills; accounting elaborated the
     immutable ledger under adversarial pressure from cheats.
     HEURISTIC: "who is punished for getting X wrong?" → go
     there for the good version. This is the Linnaean escape
     hatch AIMED rather than broadcast.
     BIGGEST UNTAPPED SOURCE by that heuristic: AVIATION —
     checklists, incident reporting, blameless investigation,
     crew resource management. It elaborated failure-and-
     recovery because crashes kill and are investigated in
     public. Software borrowed post-mortems from aviation and
     borrowed them badly.
163. THE SENSEMAKING MODE — a FOURTH use mode, not previously
     articulated. The three recorded were: read as design
     reference / generate from / cite at runtime. Fran's is
     THE LIBRARY AS AN INSTRUMENT OF PERCEPTION: apply terms to
     an unstructured environment to SEE structure. His users:
     researcher, founder, CTO, a company pivoting into new
     software, someone reading a legacy codebase — anyone facing
     a firehose whose job is to make sense of it. This is the
     per-iudicium mode operationalised, and where the leverage
     argument bites hardest.
164. THE SHARPEST AFFORDANCE IN THAT MODE: DETECTING WHAT IS
     MISSING. Unstructured observation cannot do this — absence
     has no perceptual signature. A genus with declared partes,
     relationes and status makes absence VISIBLE: walk the
     environment asking "where is the X?"
     AND THAT IS OUR OWN UMBRAE MECHANISM ONE LEVEL OUT. The
     library already computes its missing pieces from
     `externum`; the same move against a codebase yields "this
     system has configuratio but no secretum handling" — a
     finding, not a vibe.
165. TOOL IMPLIED, over machinery that already exists: a
     CHECKLIST PROJECTION (given a genus, what should be
     present?), a REVERSE LOOKUP (I see this — what kind might
     it be?), and a GAP REPORT (present / absent / mis-filed).
     That is umbrae generalised to an arbitrary environment, and
     it is the same missing pair identified in round XXXIV
     (reverse index + apparatus-as-questions). Two independent
     routes now point at the same next build.

## 2026-08-05 — round thirty-seven: APOPHATIC NODES + AVIATION
## (Fran's insight, and loop run six)

166. FRAN'S APOPHATIC INSIGHT: unknowns should be FIRST-CLASS,
     with their own menagerie of genera — "kind of like the
     concept of zero". Rigour, and dodging the risk that the
     information is not properly tracked.
     EVIDENCE THAT THIS NAMES A REAL PATTERN: we have hit it
     SEVEN TIMES and solved it locally each time —
     officium.vacans (absence as a first-class STATE),
     vitium.non_reproductum ("neither declared false nor proven
     true"), gradus_assensus.dubitatio, plus five separate
     `vacuum = …` notas each defining locally what empty means.
     AND persona.mortuus ADMITS A CONFLATION IN WRITING:
     "vacuum = vivens aut ignotum" — empty means ALIVE OR
     UNKNOWN. Codd's NULL problem inside our own library,
     documented and unfixed.
167. PROPOSED: genus `ignotum` whose SPECIES EACH LICENSE A
     DIFFERENT MOVE (which is what makes it leverage rather
     than bookkeeping):
     - ignotum_notum (known unknown) → ASSIGNABLE; it is a
       pensum, go find out;
     - ignotum_ignotum (unknown unknown) → not actionable
       singly, only in aggregate (slack, shorter cycles);
     - indeterminatum (no fact of the matter YET) → wait or
       decide;
     - amissum (was known, lost) → recover or accept;
     - retentum (someone knows and will not say) → negotiate;
     - non_applicabile (the question does not apply) → CODD'S
       SECOND NULL; your schema is wrong;
     - controversum (contested — TOO MANY answers, not none)
       → adjudicate, and it is a iudicium dispute.
     EXACT TIE TO FRAN'S OWN TERM: A COMPLICATION IS AN
     UNKNOWN-UNKNOWN THAT MATERIALISED. "non praevidenda" just
     is "was an ignotum_ignotum".

AVIATION WALK (loop run six) — the domain PUNISHED for not
knowing, hence the apophatic domain par excellence.

168. CHECKLIST FORCES A REFINEMENT OF PRAESCRIPTUM: an aviation
     checklist is NOT instructions — the crew knows how to fly.
     It is a set of CONFIRMATIONS, existing because knowing is
     not doing under stress (Do-Confirm vs Read-Do is
     formalised). A recipe TEACHES, a checklist VERIFIES.
     Conflating them is why "just be careful" fails as a remedy.
169. MEL (Minimum Equipment List) — PURE APOPHATIC MACHINERY, and
     the cleanest specimen of Fran's point anywhere: a formal,
     regulated list of WHAT MAY BE ABSENT and the aircraft still
     fly. Absence as a certified condition.
170. THE INSTITUTIONAL FIND — BLAMELESS INVESTIGATION: ICAO
     Annex 13 declares the sole objective of an accident
     investigation to be PREVENTION, explicitly not the
     apportioning of blame, and separates it legally from
     liability proceedings. THE SEPARATION IS WHAT MAKES PEOPLE
     REPORT. Software borrowed the word "blameless" and none of
     the structure — Fran's painful-reimplementation pattern,
     exactly.
171. RECURRENTIA BENT: "every 500 flight hours" is a recurrence
     keyed to ACCUMULATED USE, not the calendar. We have
     periodic and computed; this is a third kind
     (recurrentia_per_usum).
172. LICENTIA BENT the same way — CURRENCY: three landings in
     ninety days to carry passengers. The licence's validity
     depends on RECENT PRACTICE, not a past examination.
     Neither licentia nor titulus can say that.
173. CREW RESOURCE MANAGEMENT — sharpest cross-domain item and
     an OFFICIUM PATHOLOGY: the authority gradient means the
     junior does not contradict the captain, so the office's
     authority IMPEDES INFORMATION FLOW UPWARD. Aviation had to
     DELIBERATELY WEAKEN an institutional structure for safety.
     Our officium models the authority and not the pathology.
174. Also surfaced: NOTAM (expiring notices of the
     non-permanent — addressed, temporal, obligatory reading);
     the accident / serious-incident / incident gradation with
     REGULATORY thresholds (our `incidens` came from devops and
     has none); airworthiness as a status conferred and
     maintained BY ACTS; redundancy as a design property about
     TOLERATING absence — apophatic again.

Nothing built; harvest awaits blessing. Note that aviation was
chosen by the round-XXXVI heuristic ("who is punished for
getting X wrong?") and it paid on the first attempt.

## 2026-08-05 — round thirty-eight: IGNOTUM built (absence made
## first-class)

175. Fran directed ignotum first, and it was the right order:
     it is cross-cutting, it fixes an admitted conflation, and
     every later model can now be written with it available.
     Built in `iudicium` because it is epistemic.
176. THE DEFINITION TURNS ON ONE DISTINCTION: not knowing vs
     KNOWING THAT YOU DO NOT KNOW. The first posits nothing —
     the gap is invisible. The second POSITS the gap, and once
     named it can be reasoned about, assigned, counted,
     resolved. So CREATING an ignotum is an epistemic ACT, not
     a defect. Fran's zero analogy is exact and is written into
     the header: zero is not "no number" but a numeral
     representing absence POSITIVELY, and positional notation
     became possible only once it existed.
177. SEVEN SPECIES, EACH LICENSING A DIFFERENT MOVE — which is
     what makes the genus leverage rather than bookkeeping:
     - ignotum_notum → ASSIGNABLE (it is a pensum; go find out)
     - ignotum_ignotum → not treatable singly, only in aggregate
     - indeterminatum → wait, or DECIDE (your decision may
       determine it)
     - amissum → recover or accept, but RECORD that it was once
       known, since that is itself knowledge
     - retentum → negotiate or compel; investigation is useless
       here, which is exactly why the species is needed
     - impertinens → fix the SCHEMA, not the world
     - controversum → ADJUDICATE, do not investigate (more
       investigation only adds a third answer)
178. TWO FINDINGS INSIDE THE SPECIES:
     (a) ignotum_ignotum CANNOT HAVE INDIVIDUA BY ITS NATURE —
         name one and it becomes an ignotum_notum. This is the
         terminal case of Fran's addressability criterion AND
         its vindication: the SPECIES is addressable
         (/iudicium/ignotum_ignotum) even though its members are
         not, which is precisely the refinement already recorded
         ("members worth picking out" includes species level).
     (b) indeterminatum is NOT ignorance: ignorance is in the
         KNOWER, indeterminacy is in the THING (Aristotle on
         future contingents — tomorrow's sea battle). Schemas
         calling both "unknown" mislead, because one is solvable
         by investigation and the other never is.
     And impertinens is CODD'S SECOND NULL, which he wanted and
     did not get — half a century of database confusion from one
     missing distinction.
179. TIE TO FRAN'S OWN TERM MADE EXPLICIT IN THE ARTIFACT:
     A COMPLICATION IS AN IGNOTUM_IGNOTUM THAT MATERIALISED.
     "non praevidenda" says exactly that, and the shared remedy
     follows — not more planning but instruments that surface
     them sooner.
180. DOGFOOD: persona.mortuus's confessed conflation ("vacuum =
     vivens aut ignotum") now names its resolution — empty means
     ALIVE, and a genuinely unknown death date must be posited
     as an ignotum, because a NAMED GAP can be handled and an
     empty field cannot. And it records a THIRD kind of absence
     that neither vacuum nor ignotum covers: graydon_hoare's
     missing birth date is DELIBERATE — a judgement about what
     ought not be said. Absence by discretion, noted as an open
     edge.
181. Also repaired: iudicium's `versio` attribute said 0 while
     its own header said 1 — drift from an earlier round, found
     while bumping. Small, but exactly the class of thing the
     loader's rule 14 (modulus/filename agreement) would catch
     if extended to version coherence.

METAMODEL QUESTION RAISED, NOT ANSWERED: should <valor> gain a
way to say "unknown" directly (e.g. ignotum="ignotum_notum")?
There IS content pushing on it — graydon_hoare's deliberate
absence is currently prose. But metamodel changes are expensive
and global while genera are cheap and local, so the discipline
says: wait for a second forcing case.

Measured: 30 exemplaria / 137 genera / 288 residents / 371 arcs
/ 0 vulnera.

## 2026-08-05 — round thirty-nine: the aviation nouns built

182. INDEX_VERIFICATIONIS (praescriptum v2) — a checklist is
     NOT instructions. The executor already holds the art; the
     list exists because KNOWING IS NOT DOING under load,
     haste and fatigue. So the failure it prevents is not
     ignorance but OMISSION.
     Its two practical consequences are the reason it deserves
     a genus: (a) TRAINING CANNOT SUBSTITUTE FOR IT — you get
     more expert and still forget, indeed the expert omits MORE
     easily because the hands run without the mind, so "be more
     careful" is powerless against omission where a list is
     not; (b) it is USELESS TO A NOVICE — an expert's
     instrument, not a learner's, and whoever uses a checklist
     as teaching loses both. Do-Confirm vs Read-Do carried as a
     property, with the aviation reasoning (haste vs lethality).
183. INVESTIGATIO (iudicium v3) — the round's institutional
     find, from ICAO Annex 13: the sole objective is
     PREVENTION, blame-apportioning is EXPRESSLY excluded, and
     the inquiry is separated in law from liability
     proceedings.
     THE MECHANISM WRITTEN INTO THE GENUS: those who best know
     what happened are the same people at risk from blame. If
     reporting costs you, you do not report, and an inquiry
     without the testimony of those present is blind. So
     immunity is not softness — IT IS THE PRICE OF TRUTH.
     Hence `finis="uterque"` is recorded as a FAILURE MODE, not
     a compromise: an inquiry attempting both prevents neither,
     because the precondition of the first (free confession) is
     destroyed by the second (consequence). And
     `inquisitio_forensis` is included as the CONTRARY SPECIES
     that proves the point — assigning blame is legitimate and
     necessary, which is exactly why it cannot be the same
     process. `suspensa` is a state that itself betrays a
     failure of separation.
     recensio_post_mortem named honestly: the aviation form
     borrowed WITHOUT the separation that makes it work.
184. RECURRENTIA_PER_USUM (tempus v4) — keyed to accumulated
     use, not the calendar. Species per_horas_operis,
     per_cyclos (where damage comes from the TRANSITION, not
     duration — metal fatigues by alternating stress, not by
     standing), per_spatium.
     HIDDEN COST recorded: time-recurrence is FREE (everyone
     has a calendar); use-recurrence REQUIRES AN INSTRUMENT
     that measures and retains usage. A rule presupposes a
     measurement, and measurement is not free.
185. LICENTIA gained `recentia_postulata` — and it completes a
     TRICHOTOMY that neither genus could state before:
     a TITULUS never lapses (it attests a fact); an ordinary
     LICENTIA lapses by TIME; a licence with currency lapses by
     DISUSE — you lose it by not exercising it, though no term
     expired. Only the third acknowledges that SKILL DECAYS.
186. OFFICIUM gained the AUTHORITY-GRADIENT PATHOLOGY as an
     open nota: the authority of the seat can IMPEDE
     information flowing upward — the junior does not
     contradict the captain even when he sees the error.
     Aviation learned this from crashes and DELIBERATELY
     WEAKENED an instituted structure (CRM gives the junior a
     duty to question and the captain a duty to hear). Our
     genus can state authority but not its failure mode;
     carried as a dubium (property? relation? its own genus?).
187. MEL recorded as a METAMODEL finding rather than a genus:
     `<pars necessaria="verum|falsum">` is BINARY, and a Minimum
     Equipment List is necessity WITH A PREDICATE AND A CLOCK
     (one radar may be inoperative if the other works and the
     flight avoids convective weather, for ten days). Written
     into METAMODULUS §3 as open. One forcing case is an
     observation; a second domain (medical device tolerances,
     N+1 redundancy) would make it a mechanism.
     Also noted from the walk and NOT coined: redundancy fails
     the addressability test (you do not address "redundancy
     #3" — it is a design property), and incidens.gravitas in
     aviation is fixed BY REGULATION where ours is per iudicium.

Measured: 30 exemplaria / 140 genera / 294 residents / 377 arcs
/ 0 vulnera.

## 2026-08-05 — round forty: instrumenta — measured, not guessed

Fran asked what tools I want. Answered from FRICTION DATA rather
than speculation, and the measurement inverted my expectation.

188. TWO SUSPICIONS, BOTH CONFIRMED AND WORSE THAN GUESSED:
     (a) FOUR STALE UMBRAE — `actus` still flagged `lex`,
         `res_sacra` and `officium` as undescribed, all three
         BUILT IN ROUND XXVIII. THE COMPUTED AGENDA WAS LYING,
         which is worse than having no agenda: we rely on it to
         know what remains. Fourth-plus instance of this exact
         bug, previously caught by eye three times.
     (b) FIFTEEN OF THIRTY FILES had `versio` drift — the
         attribute disagreeing with the file's own header
         comment. Half the corpus. Pure bookkeeping, but the
         headers were lying about which version they document.
     Both fixed; both now GATED (rules 16 and 17) and
     negative-tested.
189. THE BIG MEASUREMENT — RULE 9, and it inverted the
     recommendation. I expected the corpus to be dirty on
     "does every <valor> name a property declared on its genus
     or an ancestor" — it is the check most likely to rot,
     needs full inheritance resolution INCLUDING cross-model
     parents, and I had been writing values freely for forty
     rounds.
     RESULT: 324 valores, ZERO violations. Rule 10 (electio
     values matching declared optiones) likewise clean.
     CONSEQUENCE: the loader would be PREVENTIVE, not
     corrective. Its value is future-proofing, not cleanup —
     which lowers its priority relative to what I had assumed
     and is worth knowing before spending the effort.
190. THE LESSON, and it generalises past this library: THE ROT
     WAS IN THE METADATA, NOT THE CONTENT. I was careful with
     content because content was what I was thinking about, and
     careless with bookkeeping because it was incidental to
     every edit. Nineteen real defects in flags and version
     numbers; zero in the semantic core.
     RULE: CHECK THE THINGS YOU ARE NOT THINKING ABOUT. The
     expensive semantic gate found nothing; two cheap
     structural gates found nineteen.

Measured: 30 exemplaria / 140 genera / 294 residents / 381 arcs
/ 0 vulnera, eleven gates enforced.

## 2026-08-05 — round forty-one: natura_quaere — the reverse
## lookup, and the apparatus made explicit

191. Built the tool both routes pointed at (round XXXIV's
     leverage decomposition and round XXXVI's sensemaking mode
     named the same two absences). Two modes:
     - `natura_quaere.sh <terminus>` — searches NAMES,
       DEFINITIONES, DIFFERENTIAE and SPECIES names, ranked by
       where the hit lands (name 6, definitio/differentia 3,
       species 4), and reports matching UMBRAE separately so
       "we have no genus for X, but it is surfaced" is an
       answerable result.
     - `natura_quaere.sh -a [mod.]genus` — THE APPARATUS: turns
       a genus into the QUESTIONS IT MAKES YOU ASK, with full
       inheritance up the sub= chain INCLUDING cross-model
       parents, each inherited item labelled with its source.
192. THE APPARATUS MODE IS THE LEVERAGE CLAIM MADE MECHANICAL.
     `-a sacramentum` walks actus_sollemnis > ritus >
     sacramentum and yields seventeen questions the genus
     forces — materia, minister, intention, witnesses,
     validity state, what it constitutes — none of which the
     word alone would have surfaced for someone who had not
     read the model. That is "naming hands you the next
     questions", executable.
     And PARTES render as a CHECKLIST with [NECESSARIA] marked,
     which is the missing-detection affordance in miniature:
     walk an environment asking "adestne pars X?".
193. DELIBERATELY INDEPENDENT of natura_visus: the validator is
     now the GATE (eleven rules) and must not be destabilised by
     an advisory read-only tool. Duplicated parsing accepted;
     header records that IF THEY DISAGREE, THE GATE WINS.
194. THREE PARSING BUGS, all the same family as before and worth
     the record:
     (a) machina_statuum is a DIRECT child of genus while
         proprietas/pars/relatio sit inside wrappers — one
         XPath shape does not fit all, and the state machines
         silently vanished;
     (b) xmllint returns an attribute node-set NEWLINE-separated,
         so a TSV field built from it spanned multiple lines and
         broke the table — only the first state survived. Join
         BEFORE emitting, never after;
     (c) tree chain printed raw tabs until rendered as
         `a > b > c`.
     Family resemblance to the natura_visus lessons: the parse
     is never the hard part; the SHAPE of what the parser
     returns is.
195. INDEX.md now advertises both modes in its §I ("the most
     common question"), so a new session finds the tool where it
     is already looking.

Measured: 30 exemplaria / 140 genera / 294 residents / 381 arcs
/ 0 vulnera.

## 2026-08-05 — round forty-two: RATIO — the missing document

196. Fran asked whether the philosophy and applications material
     was captured. Honest audit: THE WRITING DOCTRINE WAS WELL
     CAPTURED (METAMODULUS §9, now indexed); THE WHY AND
     WHAT-FOR WERE SCATTERED — value proposition in a ledger
     nota, cross-pollination and the sensemaking mode in worklog
     rounds, the four use modes split across both, and several
     process laws living ONLY IN COMMIT MESSAGES.
     STRUCTURAL DIAGNOSIS: those had drifted into whichever
     document was durable. METAMODULUS is a SPECIFICATION whose
     stated audience is "anyone writing a model, and whoever
     implements the loader" — the wrong home for a rationale.
     And the worklog is CHRONOLOGICAL: an excellent record and a
     terrible reference, since "how do we work on this?" required
     reading forty-one rounds.
197. natura/RATIO.md written — the fourth document, and the one
     you hand someone asking "what IS this?" rather than "how do
     I write one?" (INDEX = what exists; METAMODULUS = the
     format; worklog = how we got here; RATIO = why, for what,
     and how we work). Six sections:
     - what this is, and what it is NOT (not a wire format, not
       a compliance target, not an upper ontology);
     - WHY MODERATE REALISM IS LOAD-BEARING — the three
       positions and what each affords: only realism makes the
       escape hatch possible (specimens are evidence), only it
       grants PERMISSION TO STOP (affirm the likeness, refuse the
       genus, move on), and it is what admits instituted kinds so
       the library can descend to where the payload lives;
     - WHAT IT IS FOR — the two halves (structure you would get
       wrong / leverage you lacked), the four use modes, the
       residence limit, and why this differs from frames and
       expert systems (task-shaped vs Pieper-shaped, plus the
       specimen-access diagnosis);
     - CROSS-POLLINATION with the seven measured instances, the
       backwards case, and the punishment heuristic;
     - HOW WE WORK — the loop with all six runs tabulated, and
       nine rules of the craft that had been scattered
       (dimensions before nouns, gate at birth, measure before
       building, check what you are not thinking about,
       one-instance-two-instances, record the non-surfacings, a
       concept that generates an instrument, a genus that cannot
       falsify itself is jargon, grep is not a parser);
     - HONEST LIMITS — the scribe's epistemic position, why the
       DISCIPLINE rather than the expertise catches errors, and
       the structural consolation that the taxonomy is more
       robust than the dictionary.
198. Wired into the orient path in FOUR places so it cannot be
     missed: METAMODULUS header now says it is the specification
     only and points here; INDEX.md (generated) names it first in
     its orientation block; MAP.txt; and the project memory.

Measured: 30 exemplaria / 140 genera / 294 residents / 381 arcs
/ 0 vulnera. Four documents, each with one job.

## 2026-08-05 — round forty-three: the tools grow eyes

Post-compact session. No new genera; instead a pass over
`natura_visus.sh` after reading the whole INDEX cold and noticing
that several things the corpus KNEW were not things the tools
SAID.

199. THE UMBRA LIST WAS TWO AGENDAS WEARING ONE HAT. Classified
     all 16 by which element cited them, and the split is clean:
     an umbra cited by `<relatio ad=>` needs a GENUS (relations
     point at kinds — ontological work, needs a ruling), while one
     cited by `<relatum ad=>` alone needs an INDIVIDUUM under a
     genus that already exists (dictionary work, no ruling, anyone
     can do it any time). MEASURED: 10 genera, 6 individua.
     So roughly a third of the standing agenda was CHEAP and had
     been carried as if it were all design work — the easy labour
     was invisible because it was filed under the hard.
     Now emitted as V.a / V.b, ordered by citation count, which
     immediately surfaced `processus` at 4x (next-largest is 2x)
     and cited BOTH ways — related-to structurally and pointed at
     as a particular, which is the addressability test firing
     twice. It is the strongest candidate on the list.
     Recorded as an EURISTICA, not a law: a relatum can reach a
     genus. The tool says so where it prints.

200. THE ARC TABLE WAS LOSSY AND NOBODY HAD LOOKED. `ARCUS`
     recorded the ancestor GENUS as each arc's source, so a
     `relatum` asserted inside a species was filed as coming from
     the genus. 381 arcs, and not one of them knew which entity
     actually made the claim: "who cites Linnaeus?" answered
     `rosa` where the truth is `rosaceae`, `rosa`, AND
     `rosa_canina` — three assertions at three taxonomic ranks,
     collapsed into one. Added a precise-source field. Two
     latent bugs fell out on the way: the ancestor lookup used
     `ancestor::genus` (which `string()` resolves to the
     OUTERMOST for nested genera — planta's whole subtree
     reported `rosaceae`), and it had to be `ancestor::` rather
     than `ancestor-or-self::` because a relatum's own `@nomen`
     is the RELATION's name, not an entity's.
     Verified by planting a dangling relatum inside `rosa_canina`:
     the gate fails, and the diagnostic now names `rosa_canina`.
     Diagnostics that name themselves — the house law — had been
     quietly violated in the one table that describes the library
     to itself.

201. RULE XVIII, ADVISORY: a name bearing another name as its
     stem (`x_y` where `x` is also a name) should either descend
     from it or declare a relation to it. Shared stem ought to
     mean shared ancestry; where it does not, either the name
     misleads or the tree lost an arc.
     Built as MONITA — non-fatal, no effect on exit code —
     BECAUSE IT CANNOT BE ENFORCED. Most hits are honest
     homonyms: `fons_c` is a source FILE and `fons` is the source
     of a PROPOSITION; `titulus_iuris` is a copyright notice and
     `titulus` is a credential; `codex_sepultus` is
     commented-out code and `codex` is a physical book. What the
     rule buys is that each such coincidence is seen ONCE.
     First run: 29. Then suppressed the cases where a declared
     relation already explains the stem — the
     version/edition/expression family, where house doctrine
     (FRBR) POSITIVELY DENIES that an edition is a kind of work,
     so `sub=` would be wrong and a relation is right. 29 → 21,
     and the survivors are essentially all real.

202. AND IT FOUND SOMETHING ON ITS FIRST RUN. Three `editio`
     individua survived suppression, which meant no declared
     relation joined them to their work. True: `expressio_de` and
     `editio_de` are both DECLARED on their genera, the
     expressiones use them (`abusus_linguae_anglice` →
     `abusus_linguae`, and `translata_ex` → the German), and all
     three editiones declare NOTHING. Moby-Dick jumps opus →
     editio with no expressio in between.
     So the FRBR chain — this library's most-cited borrowing, the
     thing RATIO §4 credits with bending our versio pattern into
     something better — was declared and unused at exactly the
     layer it was built for. A name-shape heuristic that knows
     nothing about FRBR found it. NOT FIXED THIS ROUND: it needs
     a `moby_dick_anglice` expressio and a ruling on whether a
     single-language work gets a trivial one, which is a
     modelling question, not a tooling one.

203. SPEC DRIFT IN THE ANTI-DRIFT DOCUMENT. METAMODULUS §8 listed
     15 rules; the validator enforces 17 — rules 16 (stale
     `externum`) and 17 (version mismatch) were built last round
     and never written down. The closed-vocabulary gate (rule 8)
     keeps §3/§4 honest against the corpus; NOTHING was watching
     §8 against the validator. Both rows added, plus 18. The
     lesson is the one already on the books, one level up: CHECK
     THE THINGS YOU ARE NOT THINKING ABOUT — and note that the
     project memory had "17 rules" correct while the document
     said 15, so the drift was visible and unread.

204. Smaller: dubia now carry their TEXT into the index (11 open
     questions readable without opening a file — an index that
     NAMES a doubt but will not SAY it just sends you back to the
     source, which is the thing an index exists not to do);
     apparatus markers `⚙` machina and `▣N` partes in §II, so
     "which genus gives me the most to work with" is visible;
     and prose is now stripped of `|` before entering pipe-
     delimited intermediates, which was a live field-corruption
     trap that had simply not fired yet.

MEASURED after: 30 exemplaria / 140 genera / 294 res / 381 arcus
/ 0 vulnera / 21 monita / umbrae 16 = 10 genera + 6 individua.

ONE OBSERVATION FOR THE NEXT ROUND, unactioned: `partes` is
carried by SEVEN of 140 genera. RATIO §3 names it the sharpest
affordance the library has — "a genus with declared parts makes
absence visible, and unstructured observation cannot do that
because absence has no perceptual signature." Five per cent of
the corpus exercises the mechanism we claim as the differentiator.
Either the claim is overstated or the corpus is under-built, and
which one it is has not been established.

## 2026-08-05 — round forty-four: modelling as a method, and the log that had no name

205. NEW EXERCISE, Fran's: not a coverage loop (which asks what is
     MISSING) but a MODELLING pass (which asks whether what we
     have COMPOSES). The distinction matters because every prior
     round was additive — 140 genera had each been individually
     defended and never once asked to work together on one
     subject. Its failure modes are different in kind: not "we
     lack a genus for X" but "we have every genus and they still
     don't compose", or "applying them was awkward".
     Subject: silex + volumen, chosen by Fran because he can
     verify every claim — which is the only real answer to the
     scribe's epistemic limit in RATIO §6, where I am equally
     fluent whether or not I am right.

206. IT MOSTLY COMPOSED, and two results were better than that.
     (a) `proiectio` PREDICTED A PROPERTY THE CODE HAS A TEST
     FOR. Its differentia claims an operative property no syntax
     carries — "licentia delendi: deleting a projection loses
     nothing that will not be reborn". Volumen's manifest is a
     projection whose truth is `acta`, and volumen ships an
     assertion for exactly that: plicam_ad(0) == plagulas_
     enumerare, which its worklog calls a free pin. Genus and
     code arrived at the same operative claim without contact.
     (b) PACIOLI PREDICTED SILEX'S MOST DISTINCTIVE DECISION.
     `ratio_rationaria`: "correction never by erasure but by a
     new contrary entry (stornatio) — because a ledger that can
     be rewritten is not a ledger." `proicere -ad` is
     deliberately HEADLESS: the log stays whole, the tree becomes
     old, the next condere makes the past present as a NEW point.
     That is the accounting discipline, not the git discipline —
     git has a HEAD and rebase is erasure. A five-century-old
     genus predicted the choice that most distinguishes silex
     from the obvious model.

207. AND THE ABSENCE. No genus for an APPEND-ONLY EVENT LOG
     WHOSE FOLDS ARE THE TRUTH. `ratio_rationaria`'s definitio
     states the general idea in as many words while its apparatus
     is monetary (continet→venditio, rationarius→persona): a
     species whose parent had never been born. Same species of
     absence as `programma` in run 5.
     Evidence was unusually heavy — FIVE implementations in one
     house: gesta, tabularium, volumen, ratio_rationaria itself,
     and the <historia>/<eventum> mechanism OF OUR OWN FORMAT.
     The phrase "cauda eventuum" appears three times in the
     corpus, every time inside prose. Recognised, unnamed.

208. acta.stml BUILT (model 31), and named from Roman public
     record rather than from event-sourcing — acta diurna,
     regesta, the ledger all precede the software idea, so the
     software word would have been the parochial one. Species:
     acta_publica, regesta, acta_transactionum, acta_versionum,
     acta_auditus. `ratio_rationaria` RE-PARENTED cross-model
     (sub="acta"), keeping only what is monetary as differentia.
     MEASURED PAYOFF: its apparatus went from 2 questions to 17,
     including the parts checklist — which now makes an
     accountant asked about a ledger answer "does it have named
     points?", i.e. period closes.
     `partes` used deliberately (the 5%-of-corpus mechanism from
     round 43), and it earns its place here: acta without named
     points can fold to any position but no MEANINGFUL one —
     "as at event 4471" always works, "as at release" does not.
     Nobody feels that absence until they ask.

209. PUNCTUM_ACTORUM — second genus, and it dissolves the
     awkwardness the exercise measured. silex's `conditio` is a
     point in a SEQUENCE, not in time; `tempus.momentum` could
     not carry it because "NUMQUAM timestamp" is a stated law of
     silex (only content distinguishes "you edited it" from "it
     is newer"). Clocks disagree, jump backwards, and give two
     events the same value; a position cannot. Species: conditio
     (commit), clausura_rationis (period close), statio
     (checkpoint) — and the third differs from the other two in
     that it exists only for COST, so it can be deleted without
     loss. That is a sign the genus carries two offices.

210. FOUR MORE, all obvious kinds that were blocking (Fran's
     rule: anything a reasonable person knows as a "type of
     thing" is in scope):
     - `processus` (→ operatio, not elementa_programmatis: it is
       a running thing, so it belongs beside ambitus and
       incidens). Its four citations demanded two different
       things — instance-of-a-form, and BEARER OF STATE (three
       status individua declare geritur_a) — and the conjunction
       is not accidental: errno without an owner means nothing.
       State `larva` recorded: dead but not yet reaped, the thing
       gone and its RECORD still owed. And `pid` documented as an
       identifier that is LEASED, not given — every other
       identifier in the library is stable, and storing a pid
       names a different thing an hour later.
     - `servus` (→ cliens): eight client genera and the answering
       half had never been named.
     - `systema_plagularum` + `directorium` (→ plagula_
       computatralis): these blocked IMMEDIATELY — silex's
       `fabrica` is a directory. Recorded `capsas_discernit` as
       the property that costs more hours than any other in that
       model, and refused to subordinate directorium under
       plagula: Unix says a directory IS a file and other systems
       do not, so it is a RELATION (est_plagula). A taxonomy that
       builds one system's design decision into its tree stops
       describing the others.

211. THREE DUBIA RECORDED RATHER THAN RESOLVED, each because the
     forcing case is currently ONE and the house law asks two:
     whether `daemon` belongs to programma or to processus (what
     makes a daemon a daemon — detached, session leader — is said
     of the process, not the code); whether cliens and servus are
     genera or MUNERA (a proxy is both at once, and the house
     already ruled this way once: "merx non est genus, esse-venale
     MUNUS est"); and how branching acta fold, since
     punctum_actorum.praecedens assumes a simple series.

212. THE GATE CAUGHT ME IMMEDIATELY, as designed. Describing four
     umbrae made rule XVI fire on all four in the same run —
     stale `externum` flags whose targets now exist. This is the
     rule written last round working on its first real occasion,
     and it is worth noting that the agenda would otherwise have
     lied in exactly the direction that feels safest: claiming
     work still open that was already done.

MEASURED after: 31 exemplaria / 146 genera / 307 res / 405 arcus
/ 0 vulnera / 21 monita / umbrae 16 → 12 (genera 10 → 6).

## 2026-08-05 — round forty-five: the queue cleared before the loop

Cleanup round, deliberately BEFORE the next coverage loop: the
umbra agenda is now a measured work queue, and harvesting a new
scenario onto a backlog still holding six any-time items would
bury the cheap work a second time — the exact failure the V.a/V.b
split was built to stop. Clear the queue, let the loop refill it.

213. SIX INDIVIDUA (the whole V.b list): `b` and `abc` (both
     languages MORTUAE — and B's death has a named cause: the
     PDP-11 discerned bytes and B refused types, a language
     killed by a machine — ferrum modum imperat, third case and
     first on an incorporeal thing); `att` (whose LEGAL history
     explains more than its products: C was free because the 1956
     decree forbade AT&T to sell it — and the 1984 divestiture
     forced actio `dividi` onto organizatio, with `fundi` as its
     honest inverse); `vms` (the living counter-example that
     keeps `systema_unixiforme` a real differentia — and its
     native file versioning had inside the filesystem what
     volumen builds outside it); `debian_project` (SPECIMEN THAT
     PROVES A DIVISION: an organizatio that deliberately REFUSES
     legal personhood — SPI holds its assets — so the
     organizatio/persona_iuridica split is confirmed by a thing
     that has one half and declines the other); `agentia_isbn`
     (neither corpus_normans nor magistratus: it operates a
     scheme it does not write — and a GENUS MAY LURK: scheme
     administrator, specimens IANA/ISSN centre/GS1/civil
     registries. Noted, not coined — cleanup is not coining
     hour).

214. THREE GENERA from the V.a list:
     - `processorium` (umbra 'chip' RENAMED at birth): 'chip'
       covered two things — the product model (8086: one thing,
       millions of dies) and the physical die in your hand. The
       individua we cite are MODELS; the precedent is :x86
       itself. Grade MIDDLE in the form-instance series:
       architecture is pure form, the die is haecceity-bearing
       artefactum, the product model sits between — a form that
       can be manufactured. Books already made this split
       (editio/codex). Two relations designatum_a/fabricatum_a
       because the fabless age split them (Apple designs, TSMC
       prints) — and for TSMC I deliberately OMITTED the relatum
       rather than plant an umbra: honest absence beats a
       pending citation.
     - `instrumentum_aedificationis` (sub programma): the
       ORCHESTRATOR, kept narrow by a collision check — the
       compiler is implementatio_linguae and stays there; make
       calls cc, make is a build tool, cc is not. Property
       graphum_unde declaratum/derivatum turns out to be the
       whole difference between :make and :aedilis (first
       house-tool individuum in the library — we are our own
       fons). Make's origin story recorded: Feldman built it
       after a colleague lost half a day to a stale executable —
       dolor → mandatum, forty years before we named it.
     - `consilium`: the umbra `complicatio` DEFINED ITSELF
       AGAINST (same structural absence as programma, run 5).
       Differentia settled by iteration: praescriptum lives by
       repetition, consilium dies completed or retracted —
       QUID/QUOMODO/QUO ORDINE = postulatum/praescriptum/
       consilium. Machine has a CYCLE (second after planta):
       retractatum → susceptum via renovatio, because the life
       of a real plan is to be retracted and continue, not to
       guess right once. Species consilium_computatum = the
       silex proicere/renovare plan — a PROIECTIO (regenerated,
       not retracted), dogfooded twice before it was named.

215. ONE UMBRA DISSOLVED RATHER THAN FILLED: `actio`. What
     index_verificationis confirms is not an action but THE
     THING AS IT SHOULD BE — "flaps SET" confirms a state, not
     a hand motion; half of aviation checklists confirm states.
     Retargeted to ad="*" with the reasoning in the nota.
     Coining 'actio' would have been an open lie, and it would
     have collided with the <actio> format element and the
     actus model besides.

216. DEFERRED, EXPLICITLY: `typus` and `constructum_linguae` —
     the seam where elementa_programmatis (intentions) meets
     lingua_programmandi (languages), and typus sits next to
     schema.genus_valoris ("INTENTIO valoris, non repraesentatio
     machinae"): same kind, or its representation-level sibling?
     That is an ontological commitment, not a cleanup item. They
     are now the ONLY two umbrae in the library.

217. GATES EARNED THEIR KEEP THREE WAYS THIS ROUND: rule 5
     caught fons="corpus" in four files with no <fontes> block
     (fixed by declaring, not by removing the honesty); rule 13
     caught actio= values on eventa with no declared actio
     (organizatio genuinely LACKED dividi/fundi — the gate
     forced a real improvement, not just compliance); and TWO
     STALE PROSE notas ("nondum descriptum" about things now
     described) were found by eye in citation notas — rule XVI
     cannot see prose, so the fix-the-nota-when-you-touch-it
     habit is the only gate that layer has.

218. DIMENSION-CROSSING OBSERVED AND PARKED: the old debian
     citation proposed sub-genus 'communitas' (finis sine
     quaestu). But organizatio's sub-genera divide by FINIS
     (societas/corpus_normans/magistratus) while persona_iuridica
     divides by LEGAL STATUS — axes already crossing in the tree
     (a societas typically IS a persona iuridica). 'communitas'
     would add a third crossing. Dimensions before nouns says
     this wants a REFACTOR (one axis as property or relation),
     not another sibling. Parked with the observation; not
     cleanup material.

MEASURED after: 31 exemplaria / 149 genera / 318 res / 433 arcus
/ 0 vulnera / 22 monita / UMBRAE 16 → 2 (typus,
constructum_linguae — both deliberate). The queue is clear; the
next loop writes onto a clean agenda.

## 2026-08-05 — round forty-six: pharmacy (coverage loop, run 7)

Chosen by the punishment heuristic — a domain punished for dosage,
interaction, and dispensing-against-order kills people, so it has
elaborated all three past anywhere else. First loop run onto a
CLEAN agenda (umbrae were 2, both deliberate), so every surfacing
was visible rather than landing in a pile.

219. PRE-COINING COLLISION CHECK, before the walk, and it burned
     three names in advance: `massa` (twice taken — mensura's
     species and volumen's blobs), `perscriptio` (already a
     negotiable-instrument species), `praescriptio` (free but one
     letter from `praescriptum` — deliberate rule-XVIII
     violation). Worth keeping as practice: checking AFTER
     drafting means arguing with a name you have already grown
     attached to.

220. THE RICHEST FAILURE WAS AT STATION ONE, and it is an
     etymological trap. A prescription LOOKS like `praescriptum`
     and is not one. Our praescriptum is a PROCEDURE (ordered
     steps, followed by the doer); a prescription is an ORDER
     (a directive issued by an authorised party, executed by a
     THIRD party, valid or void by form). English "recipe"
     descends from the pharmacy Rx, so the confusion is real in
     language and false in things. The compounding station holds
     both at once and keeps them apart: the formula is a
     praescriptum because it repeats, the paper ordering it is an
     order because it names this patient once.

221. IUSSUM built (→ obligatio) — the round's structural find,
     filling a hole two genera had been standing around: an
     `actus_iuridicus` is the momentary ACT, an `obligatio` is
     the BOND, and the persisting DIRECTIVE was neither. Roman
     law had the word: iussum was the paterfamilias's
     authorisation that made another's act valid — same
     structure, another's authority making an act effective.
     Its distinguishing property is one neither neighbour has:
     `usus_reliqui`. ORDERS ARE CONSUMED. A prescription carries
     two refills; an obligation is not spent by being met once.
     And status `recusatum` is what separates an order from a
     necessity: the pharmacist MUST refuse a void one, and the
     refusal is an act of the profession, not disobedience.
     THREE PARTIES, which is what really divides it from
     contractus (two): prescriber orders, pharmacist executes,
     patient benefits.
     Species: praescriptio_medica, mandatum_iudicis,
     iussum_emptionis, iussum_operis, iussum_ne_resuscitetur —
     the last chosen because its content is an OMISSION, proving
     "what is to be done" must be read widely; the PO chosen as
     the LIMIT CASE (the executor may freely refuse, penalty
     none) which is what made `poena_neglegentis` the axis that
     divides order from request.

222. CLASSIS_NORMATIVA built (→ norma) — and this is the house
     law paying out exactly as designed. Schedule I–V surfaced
     it; the SECOND forcing case was the aviation gradation
     (accident / serious incident / incident), which had been
     sitting in the open arcs since run 6 with no second domain.
     It arrived from an unrelated walk, unprompted. One instance
     is an observation, two in unrelated domains is a law.
     Why not species: a species divides a genus by what a thing
     IS; a normative class collects already-defined things by
     what the law SAYS of them. Reschedule a substance and its
     nature is untouched — which cannot happen to a species.
     Membership per actum, consequences attach to the CLASS.
     `ordinata` turned out to be a real property, not a
     universal: hazmat class 3 is neither graver nor lighter
     than class 8, while schedules and accident grades are
     ranked — and ranked ones have BOUNDARIES, which get
     litigated, which is why judgment rather than observation
     assigns membership.

223. PHARMACON built (model 32) — three genera that the DOMAIN
     had already laid out as a ladder and given identifiers to at
     every rung (RxNorm: ingredient → clinical drug → brand →
     pack; CAS, NDC, lot). Not our abstraction imposed but their
     structure read off, which is the strongest form the
     Linnaean hatch takes.
     - `substantia_chemica` — the chemical KIND, distinct from
       `materia` as a kind from its heaps: CAS numbers the
       substance, never the heap.
     - `medicamentum` — substance + strength + form, the rung the
       prescription names. Its `aequivalens` relation is PER
       IUDICIUM: the Orange Book ASSERTS therapeutic equivalence
       with ratings; two drugs can share an ingredient and not be
       equivalent.
     - `coctura` (batch) — A NEW RANK for the library, between
       the product model (`processorium`, `medicamentum`) and the
       haecceity-bearing single thing: everything made in one
       run, sharing provenance and therefore SHARING FATE. The
       recall is the event that demands it — it addresses a
       batch and can never address a unit. `expirat` belongs
       HERE, not to the drug, which is the proof the rank is
       real. Cross-domain immediately: vaccine lots, steel heats,
       food lots, seed batches.
     - `interactio` — a REIFIED relation, carrying both house
       tests at once (notas grown heavy: severity, mechanism,
       evidence, management; plus interaction databases assign
       IDs). Second domain: hazmat segregation tables. Kept the
       `inter` relation OPEN, and the hazmat specimen justified
       it — those tables relate CLASSES, not goods, so
       classis_normativa and interactio meet inside the species.
       `testimonium` deliberately separate from `gravitas`: a
       theoretical guess about a severe effect and firm proof of
       a mild one are different things, and collapsing them is
       how interaction warnings became noise.

224. FOUR SCHEMES to the dictionary (identificatio v1): `cas`,
     `ndc`, `rxnorm`, `numerus_dea`. Two earn their keep beyond
     inventory. NDC does NOT address the batch — so a recall must
     name NDC *and* lot number, which independently proves
     `coctura` is a rank rather than a subdivision. And
     `numerus_dea` identifies a TITULUS, not a person: the
     authority to prescribe, which dies with the office while the
     person lives — plus a check digit, so the pharmacist can
     compute before believing. Identifier schemes doing SECURITY
     work, which none of the previous twelve showed.

225. NON-SURFACINGS RECORDED, with the trigger named for the one
     closest to the line: patient (persona suffices), dose
     (quantitas), refill (a count on the iussum, now literally
     `usus_reliqui`), regimen (compositional — the sig "1 cap TID
     × 7 days" is quantitas + recurrentia + intervallum with no
     residue, the bakery-clinic pair carrying a third domain
     unchanged), PBM (organizatio), and `petitio` (insurance
     claim = demand for performance under an obligation) — NOT
     coined on one domain; a warranty or legal claim would be the
     second case that flips it.

226. COMPOSITION WINS, worth recording because they are the first
     time a loop found YESTERDAY'S genera already load-bearing:
     `acta_auditus` (built round 44) took the controlled-substance
     perpetual inventory cleanly — appender and audited are the
     same person, so its own nota "disciplina is always the WRONG
     answer here" was already written; the compounding cluster
     (praescriptum + index_verificationis + licentia) held a
     pharmacy's hardest procedure with no strain.

227. METAMODEL DUBIUM — **CLAIM WITHDRAWN, corrected in round
     48.** Written here as: "store at 2–8 °C is an
     interval-valued requirement, the second forcing case for
     planta's range dubium, now eligible for action." CHECKED
     ONE ROUND LATER AND FALSE: `mensura.quantitas` already
     carries `minimum` and `maximum` beside `valor`, so quantity
     ranges were solved before pharmacy arrived and no metamodel
     work was ever owed. The genuinely open remainder is
     narrower than stated — RECURRING TIME intervals
     ("Maius–Iunius", which is neither a quantitas nor a plain
     intervallum) — and it still stands at ONE forcing case.
     Kept rather than deleted because the failure mode is worth
     seeing: I read planta's dubium, saw the word AMBITUS, and
     matched on it without reading the sentence in the same
     dubium that says the quantity half was already solved. The
     corpus-contact law applies to our own notes.

228. And the gate caught my own prose: I wrote `morbus.interactio`
     in a norma nota when interactio lives in pharmacon. Rule
     XVIII surfaced two new monita too, both legitimate homonyms
     worth seeing once — `mandatum_iudicis` vs `mandatum` (the
     Roman consensual contract) and `numerus_dea` vs `numerus`
     (the datatype).

MEASURED after: 32 exemplaria / 155 genera / 338 res / 454 arcus
/ 0 vulnera / 24 monita / umbrae 2 (unchanged — the run added no
dangling citations, which is what building AFTER a cleanup buys).

## 2026-08-05 — round forty-seven: the domain door, left shut but unlocked

229. Fran raised namespacing — `domain_a::foo` distinct from
     `domain_b::foo`, opt-in — pressure he felt piling up in the
     pharmacy round. MEASURED BEFORE DESIGNING, and the
     measurement reframed the question entirely: 493 names (155
     genera + 338 residents) and ZERO cross-model homonyms, even
     though rule 15 only requires uniqueness WITHIN a model.
     So the capability already exists — `/chemia/substantia`
     beside `/metaphysica/substantia` validates today, and the
     address `/modulus/nomen` IS `domain::name`. Nothing needed
     building. What was missing was permission and a detector.
     GLOBAL UNIQUENESS IS A CONVENTION, NOT A LAW, and nobody had
     written that down — so every session (me included) read the
     convention as a rule and reached for a suffix instead.

230. AND RULE 18 HAD BEEN MEASURING THE PRESSURE ALL ALONG. The
     monita list is empirically the list of places a domain
     qualifier got encoded INTO a name because the namespace
     looked flat: `numerus_dea`, `mandatum_iudicis`,
     `titulus_iuris`, `fons_c`, `index_verificationis`,
     `instrumentum_lineae`. ~5% of names. A rule built last round
     to catch homonyms turned out to be a demand signal for the
     feature that would prevent them — not designed for that, and
     visible only because the output was in front of us.

231. THE FORCING CASE IS SCHOLASTIC, NOT MEDICAL, and it is worse
     than pharmacy's. `materia` is TAKEN (corporalia: bulk
     measurable stuff, flour and oil — not *materia prima*).
     `actus` is TAKEN (a model of solemn acts — not act/potency).
     And `species`, `genus`, `actio`, `fons`, `status` are
     RESERVED by §3/§4 besides. In a scholastic project the core
     Aristotelian vocabulary is already occupied or reserved;
     model hylomorphism properly and it bites on line one.
     Recorded because it is the case that will actually open the
     door, and it is nearer than any clinical one.

232. THE ARGUMENT AGAINST, kept on the record because it is the
     one that would be lost first: NAMESPACING IS A SOLVENT FOR
     CROSS-POLLINATION. `interactio` exists only because someone
     asked whether drug interaction and hazmat segregation are
     the same kind. `classis_normativa` only because drug
     schedules and the aviation gradation are. `periculum`,
     written for a clinic, took freight insurance unchanged;
     `obligatio` from Roman law serves software licensing. Every
     one of those needed the ABSENCE of a domain wall. Automatic
     per-domain namespacing would have prevented all of them —
     it is the Pieper failure one level up again: not "does this
     buy us anything" but "I need not ask whether this is the
     same thing." Hence: homonymy only, never convenience, and
     the declaration must carry the ARGUMENT.

233. BUILT, and deliberately small (Fran: "a milder escape hatch"
     — and he explicitly allowed 'too early' as an answer):
     - a MONITUM detector for cross-model homonymy, ~10 lines
       reusing the measurement itself. Reports nothing today;
       names both models the moment it is not nothing. PROVEN
       BOTH WAYS — planted a fake `interactio` in causa, watched
       it fire, restored. A detector that has never been seen to
       fire is not a detector.
     - rule 19 in §8, marked `[M] → [E] on first use`, plus a
       doctrine row in §9.
     - NO NEW ATTRIBUTE. Declaration is a `nota` until prose
       proves insufficient: coining a metamodel element for a
       case that has occurred zero times is exactly the global
       change the doctrine says to defer.
     - the two future gates RECORDED rather than built:
       undeclared homonymy → VULNUS, and citing a homonymous
       name without `modulus=` → VULNUS. The second is the real
       safety property and does NOT exist today — rule 2 catches
       a DANGLING reference but never a WRONG-BUT-RESOLVING one,
       and 188 citations currently omit `modulus=` and resolve
       locally. Writing the design down means the first homonym
       gets a gate instead of a rediscovery.

MEASURED after: 32 exemplaria / 155 genera / 338 res / 454 arcus
/ 0 vulnera / 24 monita / homonyma 0. Nothing changed in the
corpus; the door is unlocked, shut, and watched.

## 2026-08-05 — round forty-eight: a claim checked and withdrawn

234. Asked whether the pharmacy loop had models left. It did not
     — all six genera plus the identifier row landed in round 46.
     But verifying one claim before recommending what came next
     falsified it, so the round is a correction rather than a
     build.

235. `mensura.quantitas` ALREADY CARRIES minimum/maximum beside
     valor. Round 46 recorded "store at 2–8 °C" as the second
     forcing case for planta's range dubium, "now eligible for
     action". False: the quantity half was solved when mensura
     was born, and planta's own dubium SAYS SO ("partim solutum
     ... genus quantitas minimum/maximum fert IUXTA valorem")
     three lines below the sentence I matched on. I read the
     word AMBITUS, recognised the shape, and did not read to the
     end of the note I was citing.
     THE COST OF NOT CHECKING would have been a metamodel
     change — the global, expensive kind — proposed for a
     problem that did not exist. Cheapest possible place to
     catch it, and only because a recommendation was about to
     rest on it. Corpus-contact law extends to our OWN notes:
     the dubium was as available as any source, and being
     already-written made it feel already-known.
     Remaining open part, honestly restated: RECURRING TIME
     intervals ("Maius–Iunius" — not a quantitas, not a plain
     intervallum, a recurrentia whose occurrences have extent).
     ONE forcing case, not two.

236. AND IT EXPOSED AN UNDER-BUILD. Storage conditions surfaced
     at station eight of the walk and never reached
     `medicamentum` — I wrote the observation into the harvest
     and lost it on the way to the file. Added `conservatio` as
     a `quantitas`, which DEMONSTRATES the mechanism instead of
     asserting it: min 2, max 8, unit celsius, valor empty.
     A mechanism placed correctly once took a new domain with no
     change, which is what "dimensions before nouns" buys and
     the first time we have watched it pay across two unrelated
     domains without touching the metamodel.
     Lesson for the loop: the harvest table is a PLAN, and plans
     are not gates. Nothing checked that everything surfaced was
     either built or explicitly refused — the walk's own
     non-surfacings were recorded carefully while a genuine
     surfacing fell through.

MEASURED: 32 exemplaria / 155 genera / 338 res / 455 arcus /
0 vulnera / 24 monita / homonyma 0.

## 2026-08-05 — round forty-nine: the civil registry (coverage loop, run 8)

Second run chosen by the punishment heuristic — an institution
punished for getting identity itself wrong. Collision check run
BEFORE the walk (the run-7 discipline), and it decided two
stations before they were walked.

237. THE CHECK PAID IMMEDIATELY, twice, and both hits were
     semantic rather than nominal:
     (a) `titulus` is DEFINED as "attestatio publica et
     permanens ... non expirat nec revocatur - quod factum est
     infectum fieri nequit". So the certificate candidate could
     NOT be a titulus: a birth certificate ages (many authorities
     refuse one over three months old), is superseded, is
     reissued. Titulus is the thing attested; the certificate is
     the paper.
     (b) `instrumentum_publicum` already existed — as a SPECIES
     OF ACTUS_IURIDICUS — and its own definitio gave it away:
     "quod IN EO SCRIPTUM est verum praesumitur." That sentence
     is about a document, sitting inside a genus of acts.

238. ATTESTATIO built (→ acta, since its differentia depends on
     being an extract FROM a record). The general law it carries
     is far larger than vital records: A PROJECTION YOU HAND OUT
     STOPS BEING A PROJECTION.
     `proiectio` has licentia delendi — delete it, it is reborn.
     An issued certificate lacks that not by nature but by
     SITUATION: it is in someone else's hands. The register
     changes, the paper does not, and nobody can recall it
     because nobody knows where it is. Same shape as printed
     reports, cached credentials, distributed configuration,
     issued API keys.
     Hence the proper question about an attestatio is never "is
     it true?" but "WHEN was it true?" — which no in-house
     projection is ever forced to ask.
     Status `superata` is the one worth keeping: the acta moved
     after issue, NOTHING HAPPENED TO THE DOCUMENT, and it became
     false anyway. A state no corporeal thing can have except by
     depending on another.
     And `ad_punctum` records the fix almost nobody uses: an
     attestatio naming a PUNCTUM_ACTORUM cannot be superseded,
     because the point does not move.

239. THE MOVE, and what it taught. `instrumentum_publicum` →
     `acta.attestatio`; what stays in `actus` is renamed
     `confectio_instrumenti` (the notary's ACT of drawing it up).
     Two things had shared one name: the act (momentary, capacity-
     dependent) and the paper (persisting, in hand, ageing).
     THE SAME SPLIT `iussum` MADE, FOUND FROM THE OTHER SIDE:
     there an act concealed a persisting thing; here a thing sat
     among acts. Worth naming as a recurring shape — when a genus
     of acts contains a member whose definitio describes an
     object, the object wants out.
     Notarial structure vindicates the placement exactly:
     the notary keeps the original in his protocol and issues
     authenticated copies — a register with extracts, run on
     private authority.

240. FICTIO_IURIS built (→ norma). Demanded TWICE IN ONE DAY
     (adoption at station 4, presumed death at station 5), then
     found a THIRD specimen retroactively that the library had
     been holding unnamed: `persona_iuridica`. A universitas
     "possesses, contracts, sues and is sued in its own name" —
     which nobody has ever watched one do. We had been treating
     a fiction as a plain sub-genus.
     NOT A LIE: a lie conceals, a fiction DECLARES. The law knows
     the adoptive father did not beget the child and directs that
     he be treated as if he had, stating the reason. Hence
     `ratio` is a property of the fiction, not commentary — a
     fiction with its reason stripped is an instituted lie.
     Distinguished from presumption by PROOF: a presumption
     yields to contrary evidence; a fiction never does, because
     it never asserted the thing.
     `ambitus` came from adoption: the fiction reaches its
     purpose and no further — parentage for inheritance and
     support, while consanguinity impediments to marriage remain
     blood-based. Fictions are almost never universal, and that
     is where implementations get them wrong.
     Presumed death forced the honest nota: WHEN THE MISSING MAN
     RETURNS, the fiction falls but its effects largely stand —
     property distributed is generally not clawed back. Effects
     outlive the fiction, which is why `refragabilis=verum`
     would not have captured it.

241. PERSONA v6 — five revisions and it had NO machina, while the
     domain that handles persons professionally elaborates civil
     status above all else. Not a genus: a machine, since the
     states inhere in the person and are changed by acts already
     modelled. `viduus` and `divortiatus` recorded as PROPER
     states rather than "single again" — the rights that flow
     from them differ, and collapsing widowhood into celibacy
     disinherits the widow. And one transition is unlike any
     other in the library: `coniugatus → viduus per mors_coniugis`
     is MOVED BY AN EVENT THE SUBJECT DOES NOT ACT — my status
     changes by something another undergoes.
     Plus `domicilium` (jurisdiction over a person follows
     domicile, not presence — the distinction every expatriate
     discovers) and `inscriptus_in → acta`, whose nota records
     the implication: statelessness is a defect of the REGISTERS,
     not of the person.
     PATTERN WORTH A NAME: "thin where the domain is thick." The
     gap was not a missing genus but a missing APPARATUS on a
     genus we had had since round one.

242. `vis` ON ACTA (declaratoria / constitutiva) — the question
     nobody asks of a database: does the record MAKE the fact or
     RECORD it? Birth is declaratory (the child was born whether
     or not anyone registers; the entry can be wrong because
     truth lies elsewhere). A company is constitutive — before
     registration there is nothing to be wrong about. Building a
     constitutive register with a "sync from source of truth" job
     inverts reality, because it IS the source.

243. A PARKED CANDIDATE DISSOLVED ON CONTACT. "Scheme
     administrator" was parked with four specimens; walking
     station 6 killed it. `organizatio.administrat →
     schema_identificandi` already exists, and everything the
     genus was to hold — assignment policy, delegation to
     national agencies, fees, disputes — are properties of the
     SCHEME'S GOVERNANCE, belonging on schema_identificandi.
     There is a relation and some ungathered properties, and no
     thing. Second time (after `actio`) a candidate has died on a
     real walk rather than in reflection — reflection kept both
     alive, walking killed both.

244. COMPOSITION WIN, unsurfaced by design: correction by
     MARGINAL ANNOTATION — never erasure, the original left
     legible. That is `stornatio`, and the civil registry has
     practised it since centuries before Pacioli. `acta`, built
     round 44 for event logs, predicted the working practice of
     the oldest record-keeping institution walked so far.
     Apostille likewise: an attestatio whose subject is another
     attestatio, recursion falling out of the definition with no
     added machinery — a sign the genus was cut at the joint.

MEASURED after: 32 exemplaria / 157 genera / 344 res / 464 arcus
/ 0 vulnera / 24 monita / umbrae 2.

## 2026-08-05 — round fifty: synthesis pass (overlap, dubia, RATIO)

Deliberately between loops: seventeen genera were built this
session across rounds 43–49, several from a single walk's forcing
content, and nothing had checked them against each other.

245. THE OVERLAP CHECK NOBODY HAD RUN: four genera about WHAT
     OUGHT TO HAPPEN, built in four separate rounds and never
     laid side by side — `postulatum` (what must be TRUE; silent
     on order and executor), `praescriptum` (HOW, iterated, any
     competent doer), `consilium` (what ORDER, once, provisional),
     `iussum` (WHAT, this time, BY AUTHORITY, to a named
     executor, consumed).
     RESULT: no overlap. They divide on two clean axes —
     iterated/once, and authority present/absent — which is a
     better outcome than it sounds, because four genera coined
     independently across a month could easily have collided.
     BUT A MISSING LINK: consilium's own differentia says
     "consilium quod mutari non potest non consilium est sed
     VINCULUM" — and that vinculum was built the NEXT round as
     `iussum`, neither citing the other. Adjacent, mutually
     defining, discovered a round apart, unconnected. Added
     `consilium.exsequitur_per → iussum`: a plan proposes and
     stays provisional; where work must be committed, orders are
     issued, which add authority and bear obligation. A plan
     without orders is an exhortation.
     LESSON: coining genera one at a time is safe for
     CORRECTNESS and unsafe for CONNECTEDNESS. The gate catches
     dangling references, never missing ones.

246. TWO DUBIA CLOSED, both by material this session produced
     without anyone noticing — which is why the sweep was worth
     running at all.
     (a) CLIENS/SERVUS — genera or MUNERA? Answer: BOTH, AT
     DIFFERENT RANKS. The bare genera name munera (positions in
     a colloquy), which is exactly why a proxy occupies both at
     once with no contradiction. The SUB-genera are real kinds,
     because a thing made FOR a role is shaped by it — a browser
     is not "a program presently requesting" but a thing built to
     request, and everything else about it follows. House
     precedent that settles the reasoning:
     `corporalia.instrumentum`, defined as "ad faciendum factum"
     — the library already constitutes a genus by ordination to
     an end.
     SECOND EVIDENCE, given by round 46 without recognition: in
     `iussum` I made iubens/exsecutor/beneficiarius RELATIONS
     without hesitating. Same structure, same judgment, different
     round. What is solved twice the same way is not arbitrary.
     GENERAL RULE EXTRACTED: a munus does not make a genus, but a
     thing MADE FOR a munus is one.
     (b) TESSERA's parent — solved by TRIPLE NEGATION, which is a
     real resolution: not `instrumentum` (an instrument OPERATES,
     a tessera signifies), not `res_corporea` (digital tesserae
     have no body), and now not `attestatio` either — and that
     third rejection CLOSES the argument. Attestatio TESTIFIES
     to a fact recorded elsewhere; tessera CONSTITUTES the right
     by being presented. Tessera's own definitio had carried the
     distinction from the start ("apocha te solvisse TESTATUR,
     tessera ius ipsum FERT") but had no contrary genus to point
     at, so it could not say what a tessera was NOT.
     NOTABLE MECHANISM: a new genus can resolve a FOREIGN
     dubium by supplying the contrast class rather than the
     parent. Worth watching for.

247. RATIO.md REFRESHED, and its staleness was the argument for
     the whole round: written round 42 expressly to stop
     orientation material from drifting, it was seven rounds
     behind — six loop runs recorded when there were eight, no
     knowledge of the modelling pass, the domain door, or four
     later cross-pollination cases. THE DOCUMENT THAT EXISTS TO
     PREVENT DRIFT DRIFTS TOO, and nothing gates it because
     prose is not checkable. Added: runs 7–8; the modelling pass
     as a SECOND instrument with its distinct question; the four
     later cross-pollination cases (stronger than the original
     seven, because in each the shared kind was found BEFORE the
     genus was written); and four new rules of the craft —
     collision-check before walking, park-then-walk, the
     acts-holding-an-object tell, and "the gap may be apparatus,
     not a genus."

MEASURED after: 32 exemplaria / 157 genera / 344 res / 465 arcus
/ 0 vulnera / 24 monita / DUBIA 15 → 13 / umbrae 2.

## 2026-08-05 — round fifty-one: PKI (coverage loop, run 9)

Run chosen not by breadth but as a TEST: `attestatio` was one
round old and already cited, and PKI is its obvious second
domain. If it survived unchanged, strongest possible
confirmation; if it broke, far better to know at one round than
at five citations.

248. IT DID BOTH, which is the useful outcome. Two measured
     DEFECTS and two PREDICTIONS.

249. DEFECT ONE — NO INTRINSIC EXPIRY. I gave attestatio
     `aetas_accepta`, noted as "a property of the RECIPIENT, not
     the paper", modelled on birth certificates: those never
     expire in themselves, and a registrar refusing a
     three-month-old one imposes his own rule. A TLS certificate
     expires IN ITSELF on a date the issuer fixed. The machine
     already had an `expirata` state — A STATUS WITH NO MOTOR,
     which nobody noticed because the civil registry never
     exercised it. Added `expirat`; both properties kept, because
     both are real and conflating them means renewing when nobody
     asked or failing to when everyone will.

250. DEFECT TWO — no `fundamentum`: what the issuer actually
     VERIFIED before attesting. Domain-validated and
     extended-validation certificates are objects of identical
     shape carrying wildly different warrant (one proves the
     applicant could read an email; the other that a company
     exists and owns the name). Without the field an attestatio
     says it testifies and stays silent about how much it knows.

251. PREDICTION ONE, and the best result of the session:
     `ex_actis` carries the sentence "an attestation with no
     named record is an ASSERTION, not an attestation." A
     pre-2013 TLS certificate was exactly that — a CA signed, and
     nobody could enumerate what it had signed, so a mis-issued
     certificate stayed invisible until used. THAT IS THE
     DIGINOTAR FAILURE, and Certificate Transparency exists to
     retrofit the missing register. A genus built from vital
     records one round earlier states, as a DEFINITIONAL
     REQUIREMENT, the defect that broke web PKI for two decades.
     Not a warning bolted on: a consequence of saying what an
     attestation IS.

252. PREDICTION TWO: the revocation nota already read "revocatio
     NUNTIANDA est ne frustra sit." Web PKI's central operational
     failure is soft-fail revocation checking — that sentence
     unheeded. And the industry's actual remedy is the one the
     genus implies: unable to announce revocation reliably,
     SHORTEN VALIDITY. Ninety-day certificates are a workaround
     for a broken announcement channel, which reads as an
     accident until the genus makes it a consequence.

253. SECRETUM built (→ iudicium, PAIRED WITH IGNOTUM: ignotum
     names absence of knowledge — nobody knows; secretum names
     RESTRICTED knowledge — some know and others must not. Two
     genera about the distribution of knowledge from opposite
     sides, which is why it belongs in the epistemic model and
     not among the computational things that forced it).
     Not "data with an access rule": an access rule is extrinsic
     and changeable, while here restriction is the ESSENCE. A
     disclosed key is still a key and is worth nothing.
     ONE-WAY LIFE is the differentia: everything else in the
     library can be repaired (an instrument refitted, a defect
     healed); a secret is never repaired, only SUBSTITUTED, and
     the substitute is a different thing.
     `suspectum` carries the doctrine: returning to `custoditum`
     requires proving a NEGATIVE, which is why the craft rule is
     "treat suspicion as compromise" — not timidity but logic.
     The transition back is present because it is possible, and
     annotated because it is almost never taken; omitting it
     would be a lie about the shape.
     `substituibile` is the property that matters most and is
     least modelled in practice: keys rotate, passwords change,
     but biometrics, national identity numbers, and historical
     secrets CANNOT — and their disclosure is permanent damage.
     Treating an unsubstitutable secret as substitutable means
     having no recovery plan at all.
     `sigillum_confessionis` included as the SCALE TERMINUS, not
     an exception: the one species where the count of knowers
     cannot even grow, since the confessor may tell no one.

254. RADIX_FIDUCIAE built (→ acta, beside attestatio). A chain of
     attestation must END, and the end cannot be proved — proof
     would be another attestation and the regress would continue.
     So the terminus is DECIDED.
     RISK ACKNOWLEDGED IN THE FILE: the library has killed two
     genera for naming a POSITION rather than a nature (effectus,
     actio). Argument that this one survives: a root is not a
     place a thing happens to occupy but a CONFERRED status — a
     vendor admits it, a statute creates it — with a date, an
     author, and revocability; and roots are enumerated and
     addressed. The addressability test passes.
     `modus_susceptionis` is the sharp property: what ships with
     the system is chosen by nobody and trusted by everyone —
     ordinary trust is INHERITED, not decided. And status
     `reiecta` records the structural hazard: rejecting one root
     kills every attestation beneath it at once, none of them
     individually faulty.
     Specimens deliberately across a millennium — a trust store,
     a notary's own commission (who notarises the notary? nobody;
     the law constitutes him and the regress stops), and the root
     of a chain of title, which is a root by EXHAUSTION rather
     than decision: nobody chose it, the search simply must stop
     somewhere and statute says where.

255. RULE XVIII EARNED ITS KEEP BEFORE THE COMMIT: flagged
     `titulus_originarius` as bearing the stem `titulus` without
     descending from it — and it was genuinely misleading, since
     `obligatio.titulus` is an ATTESTATION (a degree, an honour)
     while this is the origin of a CHAIN OF OWNERSHIP. Both
     senses are good Latin (titulus = both honour and just cause
     of acquisition), so it is a real homonym, not an error — and
     still worth removing from an index. Renamed `origo_dominii`
     BEFORE the commit. An advisory rule producing a correction
     at the moment of writing rather than an argument later.

MEASURED after: 32 exemplaria / 159 genera / 351 res / 470 arcus
/ 0 vulnera / 24 monita / dubia 13 / umbrae 2.

## 2026-08-05 — round fifty-two: versio — the third structural absence

256. THE SIGNAL, and this time it was the strongest of the three:
     FOUR genera named `versio_X` (linguae, systematis,
     architecturae, praescripti), each with its own `versio_de`
     and `sequitur`, and NO general `versio`. `programma` had one
     genus defining itself against an absence; `acta` had five
     scattered implementations; this had four siblings all
     reinventing the same apparatus in parallel.

257. THE COMPARISON FOUND A DEFECT NOBODY COULD SEE ALONE.
     Three of the four carry `titulus` + `edita`;
     `versio_praescripti` carries NEITHER — while its own
     definitio calls it "editio nominata et CITABILIS." A thing
     professing citability with no field to be cited by. Nothing
     was wrong within that file; the fault existed only
     RELATIVE to its siblings, and duplication is not merely
     wasteful but UNEVEN — the unevenness invisible until the
     copies are laid side by side.

258. AND WHAT ALL FOUR OMITTED is the thing versioning is FOR:
     COMPATIBILITY. Not one asked whether a new version can
     stand where the old did — which is the only reason anyone
     numbers versions at all. Four independent modellings of a
     concept, and all four captured the bookkeeping (name, date,
     predecessor) and missed the payload.
     Recorded in the parent with the sharp part: A VERSION
     NUMBER UNDER SEMVER IS A CLAIM, NOT A LABEL — an assertion
     about compatibility, per iudicium, with an author, which
     can be and routinely is broken. Hence the paradox worth
     keeping: date- and name-based schemes are never violated
     BECAUSE they promise nothing; semver is violated constantly
     BECAUSE it says something.

259. Identity clause in the differentia, since versioning is the
     Ship of Theseus with a release process: what makes v2 the
     same THING as v1 is not content (that changed — hence a new
     version) but CONTINUITY OF NAMING AUTHORITY AND PURPOSE.
     Where either lapses it is not a new version but a new
     thing, which is what a fork is. Dubium recorded for the
     middle cases (a fork keeping the name; an authority
     changing while the thing persists) — one forcing case so
     far.

260. `sustentata` is the state binary schemes lose, and where
     most software actually lives: between "use this" and "stop
     using this" lies a long stretch of "maintained, not
     recommended", on which every migration plan depends.
     And `deprecata` is a SPEECH ACT, not a fact — the same
     declared/discovered split `functio` already carries
     (deprecata declared vs mortua measured).

261. RE-PARENTING PAID IMMEDIATELY AND MEASURABLY:
     `versio_praescripti` went from 3 apparatus questions to 14,
     and acquired the `titulus` its own definitio had promised.
     No content was written for it — inheritance supplied what
     four rounds of independent authorship had missed.

262. NEW FORMAT PATTERN, first occurrence: NARROWING AN INHERITED
     RELATION. The parent declares `versio_de → *` (anything can
     have versions); each child keeps its own `versio_de →
     lingua_programmandi` etc. Both stand, and the apparatus
     prints both — parent says what the relation IS, child what
     is permitted HERE. Deleting the child's would lose a
     gate-checkable constraint; deleting the parent's would lose
     the general truth. Recorded in §9 because it will recur the
     moment any other general genus adopts specialised children.

263. Doctrine row added for the signal itself: SEVERAL
     SPECIALISED, NO GENERAL. Three payouts now, each larger than
     the last, and the tell is cheap to check — grep for a shared
     name prefix among genera and ask whether the stem exists.

MEASURED after: 33 exemplaria / 160 genera / 351 res / 478 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-05 — round fifty-three: distributed systems (coverage loop, run 10)

Scenario: a card payment through a distributed system —
authorise, capture, settle. Punished harder than any other
software domain (double-charge, lost funds, regulators), which is
why it has elaborated retry, ordering and agreement past anywhere
else.

264. THE DEFERRAL PAID EXACTLY AS DESIGNED. `petitio` was
     proposed in run 7 (the insurance claim) and REFUSED on one
     domain, with the trigger named: "a warranty or legal claim
     would be the second case that flips it." Run 10 supplied a
     stronger one than predicted. This is the first time a
     recorded refusal has been redeemed by its own stated
     condition, and it is an argument for writing the trigger
     down rather than just saying no.

265. WHAT THE SECOND DOMAIN ADDED that the first could not: in a
     single process a call is an EVENT — it happens and is gone.
     Across a network it CANNOT BE, because the caller who gets
     no answer will ask again, and the receiver must recognise
     the second asking as the same asking. So the request
     acquires identity, a deadline and an attempt count. That is
     an ontological shift, not an implementation detail, and it
     is why `identificator_petitionis` carries the load-bearing
     nota: without it a repetition becomes a NEW petition, and
     what was owed once is paid twice.
     `petitio` PLACED BESIDE `iussum` deliberately — round 50
     taught that mutually-defining genera discovered a round
     apart stay unlinked. The dividing axis was already written
     in iussum (`poena_neglegentis`): an order neglected incurs
     a penalty, a request may be refused with no violation at
     all. Hence `negata` is a LEGITIMATE OUTCOME, not a failure.
     Best state in the genus: `elapsa` is a state OF THE ASKER,
     not of the request — the receiver may still grant it, and
     often does, after the asker has given up. Two states
     simultaneously true and contradictory, which no other genus
     in the library tolerates.

266. QUORUM — cross-pollination running BACKWARDS, which is rarer
     and better. Distributed systems appear to have invented
     "majority"; the word is Latin, and canon law and parliaments
     used it a thousand years earlier FOR THE SAME REASON: below
     quorum an act is not wrong but VOID. That is exactly
     `actus_sollemnis` (effect depends on form and the agent's
     capacity), which the library already had.
     PLACED IN `actus`, NOT `organizatio`, and the reason is
     load-bearing: organizatio is defined as "universitas
     PERSONARUM", and a cluster of machines is not persons.
     Validity, however, does not require persons. Putting the
     genus in the validity model captures both domains; putting
     it with organisations would have excluded the case that
     surfaced it.
     `modus_computandi` is the sharp property, and the
     distributed case makes the ancient one legible: computing
     from ALL MEMBERS rather than those PRESENT is what prevents
     split-brain — reckon from those present and both halves of
     a partition believe themselves quorate, which is the
     failure itself. The same ambiguity produces litigation in
     chapter meetings.

267. DEPRAVATIO — the missing middle of a triad. The library had
     `vitium` (defect against intention — the code is wrong) and
     `incidens` (visible service degradation — the service is
     failing), and NOTHING between them for the state that is
     now wrong and not yet seen.
     THE COST OF THE GAP IS THE POINT: fixing the fault does not
     fix the data. A bug repaired at 14:00 leaves balances
     corrupted between 09:00 and 14:00 corrupted; repair acts
     forward, never backward. That is among the most expensive
     misunderstandings in operations and the library could not
     state it at all.
     Its differentia is that IT HIDES BY NATURE: a fault can be
     read in the code, an incident shouts, corruption is silent
     — a wrong datum sits exactly as well as a right one. Alone
     of the three it must be SOUGHT, and is usually sought only
     after an incident betrays it.
     `sanabilis` is where the house doctrine pays its largest
     dividend yet: where acta are kept the truth can be
     recomputed; where only status was stored the prior truth
     IS GONE and repair becomes guesswork. Veritas/proiectio,
     cashed at the worst possible moment.
     SECOND TRIAD RECORDED: the library already had a triad of
     defects by the NORM measured against (vitium/morbus/damnum).
     This is a different one, by DEGREE OF MANIFESTATION
     (vitium/depravatio/incidens). They intersect and neither
     replaces the other — "defect" is a three-dimensional word.

268. `replica` REFUSED, and the refusal tests the rule made last
     round: being-a-replica is a MUNUS, and a munus does not make
     a genus. Nothing is built to be a replica — a database
     instance is made, then assigned, and promoting it to primary
     does not change its kind. Recorded with the condition that
     would admit it: purpose-built read-only appliances would
     qualify under the same rule that excludes these.

269. THREE COMPOSITION WINS, all PREDICTIVE rather than merely
     compatible, which is now the pattern worth expecting:
     - `ignotum` (built for a clinic) states the field's founding
       fact — a slow node and a dead node are indistinguishable
       from outside. A timeout is not a failure but a NAMED GAP,
       and the discipline is to design for it, never to resolve
       it.
     - `acta.ordinatio` carries the nota written for silex —
       "clocks lie, disagree and jump backwards; a position
       cannot" — which is verbatim the argument for logical
       clocks, and `punctum_actorum` already is one.
     - `acta.stornare` IS the compensating transaction. The
       chain now runs accounting → version control → distributed
       systems, one genus across all three.

270. Delivery guarantee added to `nuntius` with the law stated
     plainly: EXACTLY-ONCE DELIVERY IS IMPOSSIBLE, exactly-once
     PROCESSING is achievable — via idempotence at the receiver.
     Systems promising the former either assume the latter or are
     lying, and a consumer needs to know which.
     Noted but NOT acted on: `idempotens` is declared twice
     independently (on `mutatio` and on `procedura_automata`).
     A weaker echo of the several-specialised-no-general signal,
     at the PROPERTY level — but two shared properties is not the
     same weight as four duplicated apparatus, and properties
     legitimately recur. Recorded rather than refactored.

271. Rule 17 caught me mid-round: `actus` had its versio
     attribute bumped and its header comment left at v0. The
     gate that exists because I did this before.

MEASURED after: 33 exemplaria / 163 genera / 356 res / 488 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-05 — round fifty-four: diplomacy (coverage loop, run 11)

Chosen deliberately as a CROSS-POLLINATION TEST rather than for
breadth: a domain that is implicitly a distributed system, aimed
at the cluster built in run 10, to see whether those genera
describe the problem or merely the technology.

272. THE PRE-WALK CHECK EARNED ITS PLACE OUTRIGHT: I intended to
     coin `reservatio` for the treaty-law concept, and it is
     ALREADY A GENUS — in `tempus`, meaning a booking (capacity
     held for someone against a future time). `exceptio` was
     taken too (`commercium`). Both would have been discovered
     only after the definitio was written and grown attached to.
     Named `limitatio` instead. This is precisely the failure the
     run-7 discipline was invented against, arriving on schedule.

273. DELEGATIO built, and it is the round's real find because
     FOUR domains demanded it at once: diplomatic full powers,
     the notarial procuratio, canonical faculties, and capability
     tokens. The library had `licentia` (what YOU may do),
     `officium` (a standing post) and `titulus` (what you ARE),
     and nothing for power exercised in ANOTHER'S NAME whose acts
     bind the principal.
     PLACED BESIDE LICENTIA because the contrast is the sharpest
     available: licentia lets you act for yourself and your acts
     stay yours; delegatio makes your acts ANOTHER'S.
     AND THE CROSS-POLLINATION IS EXACT. Powers revoked in March;
     the envoy, hearing nothing, signs in May. Is the sovereign
     bound? Agency law says generally YES — the counterparty who
     relied in good faith is protected, and the loss falls on the
     principal who should have given notice. That is APPARENT
     AUTHORITY, and it is the same law `attestatio` states as
     "revocation must be announced or it is futile": a revoked
     credential that has not propagated still works on whoever
     has not heard. The old domain has centuries of doctrine on
     WHO BEARS THE LOSS, which the new one has barely begun.
     Recorded as `effectus_post_revocationem`.
     Status `extincta` is the one with no analogue in the new
     domain and a sharp one in the old: a delegation dies with
     the principal, and NO NOTIFICATION PRECEDES IT — the agent
     believes himself to be acting for a living man. Durable
     powers of attorney exist to defeat exactly that, which is
     the orphaned-session problem with four centuries of law.

274. TRACTATUS built (sub contractus), and the two-phase
     structure is NOT a metaphor: signature fixes the text and
     binds nobody; ratification binds. The interval between is a
     real legal state carrying a real duty — a signatory must not
     defeat the treaty's object and purpose while ratification is
     pending. A prepared transaction holding its locks.
     `abortus` matters as much: ratification REFUSED is a
     legitimate outcome, not a fault — and it is the whole reason
     two phases exist, since months of latency made it intolerable
     for a sovereign to be bound by an envoy's signature without
     final inspection. The structure exists BECAUSE of latency,
     which is why it recurs wherever latency does.
     And the parent turned out thin again: `contractus` carries
     initium/finis/partes/parit and NO STATE MACHINE at all —
     "thin where the domain is thick", now on the genus the child
     inherits from.

275. THE INDIVIDUUM TEACHES THE STATE BETTER THAN THE DEFINITION.
     Treaty of Ghent signed 24 Dec 1814; New Orleans fought 8 Jan
     1815; ratifications exchanged 17 Feb 1815. Thousands died
     INSIDE THE PREPARED-BUT-NOT-COMMITTED WINDOW — not a stale
     read merely (though it was that too) but action taken during
     the interval the two-phase structure itself creates. The
     cost of the uncommitted window, paid in bodies.

276. LIMITATIO built, and it has a shape nothing else in the
     library has. A reservation is partial consent — but its
     effect is PAIRWISE: it operates between the reserving party
     and each other party separately, according to whether that
     party objected. One multilateral treaty dissolves into a
     RETICULUM of bilateral obligations, as many as there are
     pairs. Three different legal relations can issue from one
     document.
     Its computing cousin is option negotiation: two peers offer
     capability sets and the session is governed by their
     INTERSECTION, not by the specification — so "implements the
     standard" says very little. Diplomacy named this centuries
     earlier AND developed the limit that computing lacks:
     a reservation incompatible with the object and purpose is
     not admitted, since otherwise a party could accept
     everything by denying everything.

277. COMPOSITION WINS: `quorum` took its THIRD domain unchanged
     (entry into force on the Nth ratification — below it the
     treaty is not weak but not in force, void rather than
     wrong, exactly as written); `acta.vis=constitutiva` took its
     second (you become a party when the deposit is INSCRIBED,
     not when sent — the register makes the fact); `attestatio`
     for the depositary's certified copies; `iussum` + `ignotum`
     for stale instructions.
     NON-SURFACING RECORDED: instructions stating OBJECTIVES
     rather than actions, so an envoy out of contact can still
     act rightly. A remedy, not a kind — but worth the note that
     a high-latency domain independently invented commander's
     intent, and that declarative desired-state reconciliation is
     the same move a third time.

278. Rule 5 fired again: `fons="corpus"` used in a model with no
     `<fontes>` block. Same gate, third occurrence — and each
     time the fix is to DECLARE the source rather than delete the
     honesty.

MEASURED after: 33 exemplaria / 166 genera / 360 res / 496 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-05 — round fifty-five: apiculture (coverage loop, run 12)

Chosen after refusing three related domains, and the refusals are
worth as much as the run. Fran asked whether insect colonies,
ecosystems or fungal networks would repay a walk.

279. THE METHODOLOGICAL TRAP, NAMED BEFORE WALKING. Diplomacy
     worked because it is an INDEPENDENT domain that solved the
     problem on its own terms centuries earlier. "Ant colony as
     distributed system" is the inverse: much of that vocabulary
     was IMPORTED INTO biology by physicists and computer
     scientists from the 1980s on. Walk it naively and the
     cross-pollination is MY OWN VOICE ECHOING BACK — the CS
     framing applied to the ants, then read out again as
     discovery. A failure mode the library had not met, and one
     that would be nearly undetectable from inside.
     SECOND OBJECTION: the punishment heuristic needs
     PRACTITIONERS who are punished and then WRITE DOWN the
     distinctions that settle disputes. In biology punishment
     falls on the organism via selection, which produces
     mechanisms, not doctrine.
     THIRD, AND MOST HONEST: mycorrhizal networks are exactly
     where my corpus is thick with popularisations that overstate
     ("trees share altruistically"), and RATIO §6 says I would be
     equally fluent whether or not I was right, with no way for
     Fran to check. Declined to model ecosystems or fungal
     networks at all.
     APICULTURE FIXES ALL THREE: practitioners, severe
     punishment, and a vocabulary thousands of years older than
     computing, so it cannot be an echo.

280. STIGMERGIA — the round's real find, and a genuine third
     mode. The library had `nuntius` (ADDRESSED, a recipient is
     owed it) and `eventum` (unaddressed but MOMENTARY, seen by
     whoever is present). This is neither: a trace left in a
     shared medium, unaddressed AND PERSISTENT, read by whoever
     passes, decaying by itself.
     No bee tells another anything. The queen emits pheromone; it
     diffuses; a worker reads the LOCAL CONCENTRATION; below
     threshold, queen-rearing begins. Nobody announced a death.
     DECAY IS THE MECHANISM, NOT A DEFECT — the property most
     often missed. A trail that did not fade would route ants to
     an exhausted flower forever. What computing calls cache
     invalidation and finds hard, nature delegates to time and
     gets free. Recorded as `tabescentia`, and the species where
     it is ABSENT (`tabula_communis` — a wiki, a shared board)
     shows the cost: what was written stays until someone deletes
     it, so shared boards go stale and nobody knows what still
     holds.
     `cumulativa` is the second half: where traces REINFORCE,
     voting happens with no voters — the path more walked is
     stronger and draws more, so the best route is chosen with no
     one choosing. Desire paths are the purest specimen, and wise
     architects WAIT for them before laying pavement.
     And the house is already doing it: silex projects a tree,
     clang reads it, neither knows the other. "The projection is
     the CONTRACT" names stigmergy without the word.

281. UNIVERSITAS — Fran's ruling, and the walk made the case
     rather than an argument. `organizatio`'s own definitio says
     "a body ordered to a common end which acts AS ONE and
     endures though its members change." That is a bee colony
     verbatim: it thermoregulates, defends, allocates foragers,
     and persists while EVERY member is replaced in about six
     weeks. Only the first two words excluded it — "universitas
     PERSONARUM."
     ROMAN LAW ALREADY HAD THE BREADTH: universitas rerum (a
     herd, an inheritance) requires no persons, and we carry that
     species already. The narrowing was ours, not the tradition's.
     What went UP: endurance, acting as one, membership,
     `dividi`, `fundi`. What stayed: persons, legal capacity,
     offices, publication.

282. AND THE RE-PARENTING PAID A DIVIDEND NOBODY LOOKED FOR:
     SWARMING IS `dividi` — the action written in round 45 for
     the AT&T 1984 divestiture. "Split into several, none
     identical to the prior; new endurances begin; assets and
     burdens are divided between them." A bee colony: the old
     queen leaves with half the workers, the remaining half rears
     a new queen, and both must survive alone. An antitrust
     decree and a swarm, the same verb, and neither was written
     with the other in view.
     Which retroactively justifies station 5's non-surfacing —
     swarming needed no genus because the ACTION already existed
     one level up, unnoticed until the parent was built.

283. DEPRAVATIO REFINED, from a domain with zero computing
     contact. A colony queenless too long grows laying workers:
     unfertilised eggs, drones only. There IS brood, there IS
     activity, THE HIVE LOOKS ALIVE, and it is finished — which
     is depravatio exactly, and the hardest thing for a novice to
     diagnose. But the bees added what the genus lacked: SUCH A
     COLONY KILLS AN INTRODUCED QUEEN. The corrupt state does not
     merely resist repair, it DEFEATS it.
     `insanabilis` says "cannot be fixed"; `repugnans` says
     "fights the fix", and the practical consequence is an
     INVERTED ORDER OF OPERATIONS — remove the resistance first
     (frames of open brood, or move the hive far off), then
     repair. Whoever repairs first loses the medicine and learns
     nothing. Computing twins: a cache that re-poisons itself on
     refresh; a config that survives redeploy.

284. COMPOSITION WINS, all from craft with no theory attached:
     `iudicium`/`observatio.signum` — a beekeeper almost never
     sees the queen and checks for EGGS, under the rule "eggs
     mean a queen within three days": inference from a sign WITH
     A FRESHNESS BOUND, which is the heartbeat-with-timeout,
     invented by people who had never heard of either.
     `officium` — a queenless colony is a VACANT POST, and the
     craft distinguishes two successions our language did not:
     SUPERSEDURE (the failing queen still lays while her
     successor is reared — time to choose) versus EMERGENCY
     (she dies suddenly, and the colony must use whatever larvae
     are young enough). The difference is not execution but
     MATERIAL AVAILABLE AT THE MOMENT OF FAILURE. Prepared
     succession wins because it CHOOSES ITS MATERIAL.

285. Rule 18 fired again and again correctly: `status_operis`
     read as a kind of `status` (a guarded mutable value) when it
     meant the working tree AS A MEDIUM. Renamed `vestigia_operis`
     before the commit — second consecutive round where the
     advisory produced a correction at the moment of writing.

MEASURED after: 33 exemplaria / 169 genera / 365 res / 500 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-05 — round fifty-six: sheep (coverage loop, run 13)

Fran chose sheep over gardening or arable, and the choice was
right for a reason worth recording: a failed garden costs a
season, but a shepherd is punished hard and continuously, and the
vocabulary is among the oldest continuous technical language we
have.

286. THE FLOCK CONFIRMED `universitas` FROM THE SOURCE. Roman
     jurists used GREX as the paradigm case of universitas rerum
     — a flock stays the same flock though the sheep change.
     A genus built one round earlier, validated not by us but by
     the authors of the word.

287. AND IT FOUND THE BOUNDARY IMMEDIATELY. A bee colony orders
     ITSELF — thermoregulates, allocates foragers, decides. A
     flock is ordered BY THE SHEPHERD: it endures as one because
     someone keeps it as one, and its members keep individual
     identity (tagged, treated, sold singly) which no bee has.
     `unde_ordo` (ab_intus / ab_extra) added, and the consequence
     is not decorative: WHAT IS ORDERED FROM WITHIN CAN BE LEFT
     ALONE AND PERSISTS; what is ordered from without dissolves
     the moment the orderer stops. A flock without a shepherd
     becomes sheep; a colony without a beekeeper is still a
     colony.
     The parent was one round old and already needed a dimension
     — which is the argument for building parents early, while
     they are still cheap to shape.

288. PERITIA COMMUNIS — the round's find, and it completes a
     triad the library had been building without noticing:
       ignotum            NOBODY knows (absence, acknowledged)
       secretum           SOME know, others must not
       peritia_communis   THE GROUP knows, no member alone
     Three genera about the distribution of knowledge, by WHO
     holds it, and the third arrived from shepherding.
     THE FORCING CASE: a hefted flock on unfenced fell knows its
     own ground and stays on it — no wall, no watching. Lambs
     learn it from their mothers. In 2001 foot-and-mouth culls
     destroyed hefted flocks across northern England, and the
     loss was NOT the sheep (those could be bought). The HEFT was
     gone, and re-hefting took years of fencing and shepherding.
     Hence the differentia: the bearer is the COLLECTIVE, so the
     mode of death is DISCONTINUITY, NOT FORGETTING. Members
     replaced one at a time and it survives, because the new
     learn from the old; replaced all at once and it dies,
     however expert each replacement is. Damage is therefore
     measured not by how many were lost but by HOW MANY AT ONCE.
     `scribi_potest` carries the trap: writing it down is NOT
     preserving it. A document does not hold a heft — sheep
     cannot read, but neither does a person learn from a book
     what is learned by hand. Writing aids RESTORATION; it does
     not substitute for CONTINUITY. Whoever confuses the two
     believes himself covered and is not.
     `tempus_restituendi` is the property that separates this
     from everything else in the library: MONEY CANNOT SHORTEN
     IT, because transmission happens by participation and
     participation takes time.
     Species deliberately across three domains — notitia_pascui
     (the heft, where the bearer cannot even speak, so no
     confusion between the knowledge and its documentation is
     possible), ars_officinae (a workshop's practice, where the
     written part is the part that matters least), and
     memoria_operis — which is what worklogs, commit messages and
     decision records are FOR, and which this house does daily.

289. PORTIO COMMUNIS — a stinted common. NOT a licentia: a
     licence permits what would otherwise be forbidden and
     implies no pool; a stint presupposes a FINITE RIVALROUS
     source where your use subtracts from mine, so the measure is
     part of the essence rather than a modality. And it attaches
     to the HOLDING, not the person — it sells with the farm.
     HISTORY WORTH STATING PLAINLY: stinting IS the answer to
     what was later named the tragedy of the commons and declared
     inevitable. Commons were stinted for centuries, enforced in
     manorial courts, and did not collapse. The thing called
     inevitable had been solved before it was named.
     `adhaeret` turned out to be the sharpest property, and the
     species show why: a grazing stint attached to LAND stays in
     the community by construction; a transferable fishing quota
     attached to a VESSEL is bought by whoever can pay most, so
     fisheries concentrate and coastal communities are stripped
     of their own right. THE DIFFERENCE IS ARCHITECTURAL, NOT
     MORAL — one structure forbids what the other permits.
     `portio_computandi` (a rate limit) is the same form again,
     and it leaves unanswered the two things shepherds settled:
     what the share ATTACHES TO, and who may transfer it.

290. METAMODEL, one word: `cultivar` was declared "planta only".
     A Suffolk is a cultivar in exactly the sense Granny Smith
     is — a rank below species produced by human selection and
     maintained by human choice. The restriction was arbitrary
     and only visible once an animal was modelled properly.
     Relaxed rather than duplicated: a new plant/animal pair of
     ranks would have been the wrong repair.

291. BEST NON-SURFACING OF THE SESSION: grafting a lamb. A ewe's
     lamb dies; an orphan needs a mother; the shepherd ties the
     dead lamb's pelt onto the orphan and the ewe accepts it.
     That is DELIBERATE SPOOFING OF A BIOLOGICAL AUTHENTICATOR,
     done by the trusted party, for the animal's good, as routine
     best practice. The credential is smell, everyone knows it is
     forgeable, and the forgery is CORRECT. An act, not a kind —
     so nothing coined — but recorded because it is the sharpest
     thing in the walk and someone will want it later.

MEASURED after: 33 exemplaria / 171 genera / 372 res / 505 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-06 — round fifty-seven: the museum (coverage loop, run 14)

Fran's choice, and it passed all three gates on inspection:
curators and registrars are punished severely (a forgery bought,
a provenance lost, an irreversible treatment); the vocabulary
runs to the Renaissance Kunstkammer; and museum registration
EXPORTED to digital asset management rather than importing from
it, so no echo risk.

292. FIDUCIA — and the second domain was already on the books.
     A museum does not own its collection in the ordinary sense:
     it holds IN TRUST, with legal title and no beneficial
     ownership. And round 45 had already recorded that the Debian
     Project refuses legal personhood and SPI holds its assets.
     Two domains, two weeks apart, neither prompting the other.
     PLACED BESIDE DELEGATIO, and the contrast is exact:
     a delegate acts in ANOTHER'S NAME (his acts bind the
     principal); a trustee acts in HIS OWN name and holds real
     title — but the BENEFIT is another's. One alienates the
     name, the other the benefit.
     `lucrum_proprium` carries the load: self-dealing is
     forbidden even when the price is fair, because the trustee's
     judgment as buyer cannot be separated from his judgment as
     seller. WHAT IS FORBIDDEN IS THE CONFLICT, NOT THE LOSS —
     a distinction most access-control thinking never reaches.
     `beneficiarius` records the hardest case: a museum holds for
     generations not yet born, who can neither complain nor sue.
     Where the beneficiary cannot defend himself, external
     custody (statute, professional norm) becomes structurally
     necessary rather than merely desirable.

293. AND STATION 7 CONFIRMED THE CUT. Deaccession proceeds are
     restricted to acquisitions, never to operating costs — and
     that is not a rule bolted on but a CONSEQUENCE: a trustee
     may not convert trust property to his own running expenses.
     When a domain's ethics fall out of the genus rather than
     needing to be added, the genus was cut at the joint.

294. INTERVENTIO — the conservators' doctrine, which is better
     than anything the library had for acting on what one did not
     make. Three conditions: REVERSIBLE, MINIMAL,
     DISTINGUISHABLE.
     The reason for the first is the best part and belongs to
     them, not us: FUTURE CONSERVATORS WILL KNOW MORE. Epistemic
     humility turned into a hard constraint on work — whoever
     acts irreversibly IMPOSES HIS JUDGMENT ON POSTERITY. That
     applies without alteration to migrations, refactors, and
     every edit to something another will handle after you.
     The third is the sharpest and least obvious: a repair that
     cannot be told from the original is a FAULT, however
     beautiful. An anti-forgery ethic INTERNAL TO REPAIR, for two
     reasons — that posterity not mistake your work for the
     original, and that nobody studying the original afterwards
     be deceived. Nothing in the library said this. Tratteggio is
     the purest specimen: retouching that matches at viewing
     distance and reveals itself close up. AN ART THAT CONFESSES
     ITSELF.
     `curatio_medica` included deliberately AS THE LIMIT: there
     reversibility is mostly impossible (an incision cannot be
     revoked), so the whole weight shifts to minimality — "first
     do no harm". Which proves the three are ORDERED, not equal.

295. ATTRIBUTIO — and the finding is better than the genus.
     The art trade has a standardised graded vocabulary — by /
     attributed to / studio of / circle of / follower of / after
     / in the manner of — each with settled meaning and legal
     force at auction.
     I TOOK THEM FOR DEGREES OF ASSENT AND THEY ARE NOT.
     "Studio of Rembrandt" does not mean "I am unsure it is
     Rembrandt"; it means "I am confident it is NOT by him and
     confident it came from his workshop." They are degrees of
     PROXIMITY TO ORIGIN, and certainty is a SEPARATE AXIS
     running across all of them — one can be quite sure a thing
     is "after Rembrandt".
     The library carried one axis and silently conflated the
     other into it. Now `certitudo` (how firmly held) and
     `propinquitas` (how much of the named hand was present) are
     distinct, which is why `attributio` had to be a sub-genus of
     `iudicium` rather than a species: only a genus can declare
     properties.
     The Rembrandt Research Project is the individuum-grade
     lesson: it de-attributed hundreds of paintings. THE PANELS
     DID NOT CHANGE. The judgment changed, and the price followed
     the judgment, not the panel. Whoever stores attribution as a
     PROPERTY OF THE WORK will treat that as correcting an error
     rather than as a new judgment about the same thing, and will
     lose the history of both.

296. COMMODATUM — cheap missing species of contractus, and it
     carries a counter-intuitive rule the Romans held firmly:
     BECAUSE the loan is gratuitous, the borrower owes a HIGHER
     standard of care than for his own goods, since he alone
     profits. Where both parties gain (hire), ordinary care
     suffices. Benefit INCREASES the burden. Museum loan
     conditions — light, humidity, courier, insurance — follow
     that logic exactly.

297. COMPOSITION, and one of them is the best `ignotum` result so
     far: provenance is a chain of `possessio` over time,
     `origo_dominii` (built two rounds ago) is precisely the root
     of title beyond which enquiry cannot pass, and a gap is
     `ignotum`. But in THIS domain THE GAP IS THE FINDING — an
     unexplained 1933–45 interval is not missing data awaiting
     completion but a reportable result with legal consequences.
     A named absence doing institutional work.
     Also: an accession number is the ANTI-PID — permanent and
     never reused even after deaccession, because the record must
     outlive the object, where a pid is a lease.

298. DEFERRED WITH ITS TRIGGER (the discipline that redeemed
     `petitio`): THE MATRIX — mould, plate, negative — a thing
     whose purpose is to generate others, and which can be
     CANCELLED to make the existing set final. One domain so far.
     A destroyed signing key or an unreproducible build would be
     the second.

MEASURED after: 33 exemplaria / 174 genera / 376 res / 509 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-06 — round fifty-eight: metrology (coverage loop, run 15)

Domain chosen on evidence rather than appetite: `mensura` was one
of the thinnest models (3 genera) while `quantitas` is cited from
dozens of places — thin exactly where it bears weight. Metrology
passes every gate: punished universally (short measure is fraud,
a bad dose kills, a tolerance miss scraps a batch), its
vocabulary predates computing, and the literature is thick.

299. A CLAIM CORRECTED BEFORE IT WAS BUILT ON. In the walk I said
     `quantitas.praecisio` "smudges bias and dispersion". READING
     IT SHOWED OTHERWISE: it means SIGNIFICANT FIGURES ("500 g"
     vs "500.0 g"), which is a clean third concept and correctly
     stated. The real gap was not a confusion in what existed but
     an absence beside it. Recorded because the walk-then-read
     order is what caught it, and the reverse order would not
     have.

300. INCERTITUDO built, and the distinction that matters is
     against something the library ALREADY had. `quantitas`
     carries minimum/maximum — a RANGE — added for planta
     ("a rose grows 2–5 m"). Uncertainty is not that.
     A RANGE IS IN THE THING; UNCERTAINTY IS IN US. Roses really
     do differ in height, and both endpoints are true. A 3 kg
     mass has ONE true mass and we do not know it exactly.
     The operational test, recorded in the differentia: a range
     is NOT reduced by better measurement; an uncertainty is —
     and if better measurement narrows what you called a range,
     it was never a range but a mis-named uncertainty.
     Doctrine taken from the domain (GUM): A MEASUREMENT
     REPORTED WITHOUT ITS UNCERTAINTY IS NOT IMPRECISE, IT IS
     INCOMPLETE.
     `factor_tegumenti` recorded because omitting it makes the
     number meaningless: "±0.5" at k=1 and at k=2 differ by a
     factor of two, and reports that suppress k cannot be
     compared at all.
     `genus_aestimationis` keeps the domain's honesty: Type A is
     computed from repetitions; Type B is ESTIMATED from other
     sources — an instrument's certificate, an assumed
     distribution, expert judgment. Type B is a JUDGMENT, not a
     computation, and is usually the LARGER part of the budget,
     which few admit.
     `componitur_ex` is the budget, and it combines in quadrature
     — so the largest component DROWNS the rest, and shaving the
     small ones buys nothing. That is why the budget is written
     down and audited: to show which term is actually paying.
     AND THE HONEST SELF-INDICTMENT, put in the nota: NOT ONE
     measurement in this library carries an uncertainty — no
     birth date, no plant height, no historical year. The
     certitudo/fons mechanism grades assent to a PROPOSITION;
     this grades error in a NUMBER. Two strata, neither
     substituting for the other: "born 1225, held as opinion"
     and "1225 ± 2 years" say different things and history uses
     both.

301. INSTRUMENTUM_MENSURAE (sub corporalia.instrumentum), and its
     differentia is sharp: an instrument that DOES NOT CHANGE its
     object but characterises it — an oven bakes the bread, a
     balance leaves it as it was. And alone among instruments IT
     IS ITSELF MEASURED, which is what a calibration is: the
     measurement of the measurer, and the origin of a chain no
     other tool has.
     `error_systematicus` carries the lesson: REPETITION NEVER
     REVEALS BIAS. An instrument returning the same wrong number
     two hundred times looks superbly consistent. Only comparison
     against something better exposes it — which is why
     calibration is NECESSARY and diligence is NOT SUFFICIENT.
     Bias, dispersion and resolution kept separate because
     ordinary speech folds all three into "accuracy", and each
     fails differently: a high-resolution instrument can be
     grossly biased — MANY DIGITS DO NOT CONFER TRUTH.

302. AND THE PREDICTED HIT LANDED. An out-of-tolerance result
     acts BACKWARDS: everything the instrument measured since its
     last good calibration becomes suspect, the lab must notify,
     and product already shipped must be assessed. That is
     `depravatio` with `fenestra` (between calibrations),
     `ambitus` (which measurements), `propagata` (did results
     feed downstream decisions) and `sanabilis` (can they be
     remeasured, or are the parts gone). Written three rounds ago
     for corrupted balances in a payment system; it takes a
     machine-shop recall WITHOUT A WORD CHANGED.
     What that confirms: the genus is about WRONGNESS THAT HAS
     ALREADY SPREAD WHILE LOOKING FINE, and it does not care
     whether the medium is a database or a workshop.

303. RADIX_FIDUCIAE GAINED THE STATE IT WAS MISSING, and the
     forcing case is the best story in the walk. Until 2019 the
     kilogram chain terminated in a PHYSICAL OBJECT, and the
     prototype and its official copies drifted apart by tens of
     micrograms over a century — with NO WAY TO SAY WHICH HAD
     CHANGED, because the prototype WAS the kilogram and could
     not be wrong, only different from everything else. A root
     that cannot be checked against anything is a root that can
     rot silently.
     So the SI was redefined to derive the kilogram from Planck's
     constant — AND THE NUMERICAL VALUE WAS CHOSEN SO THAT
     NOTHING CHANGED AT THE MOMENT OF TRANSITION.
     The genus had `suscepta → limitata → reiecta` and nothing
     for this: the root was neither rejected nor limited but
     RE-FOUNDED UNDERNEATH, continuity preserved, and everything
     standing on it felt nothing. Added `refundata`, repeatable.
     Computing twins are exact and hard: migrating a trust
     anchor, changing digest algorithm while preserving identity,
     re-deriving a key without invalidating what it signed. The
     only way a root changes WITHOUT the collateral damage
     `reiecta` inflicts.

304. FOUR COMPOSITION WINS, which is the most in any run:
     `licentia.recentia_postulata` (written for pilot currency)
     takes calibration intervals unchanged — validity from recent
     exercise rather than past proof, subject swapped from person
     to instrument; `attestatio` fits the calibration certificate
     using BOTH properties PKI forced us to add (`expirat` set by
     the issuer, `fundamentum` = what was actually verified);
     `depravatio` as above; and the traceability chain is
     `radix_fiduciae`'s third domain.
     NON-SURFACING: calibration itself. It is a measurement of the
     measurer producing an `attestatio` — and notably NOT an
     `interventio`, since it characterises without changing,
     which is the very differentia written for
     instrumentum_mensurae one hour earlier.

MEASURED after: 33 exemplaria / 176 genera / 376 res / 516 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

## 2026-08-06 — round fifty-nine: review pass (the fourth absence, and a gate on prose)

Between-loops review at Fran's suggestion, and both findings came
from applying an existing instrument rather than inventing one.

305. THE FOURTH STRUCTURAL ABSENCE, and the first at the PROPERTY
     layer. Four properties, four separate rounds, all asking ON
     WHAT DOES THIS CLAIM REST, none with a parent:
       attestatio.fundamentum              (PKI, LI)
       interactio.testimonium              (pharmacy, XLVI)
       attributio.fundamentum_attributionis (museum, LVII)
       incertitudo.genus_aestimationis     (metrology, LVIII)
     THE SIGNAL WAS SEEN ONCE BEFORE AND CORRECTLY REFUSED:
     round 53 noted `idempotens` declared twice independently and
     declined to act, since "two shared properties is not the same
     weight as four duplicated apparatus, and properties
     legitimately recur." That was right. Four is different, and
     the earlier refusal is what makes this one credible.

306. AND `fons` IS NOT IT — its own definitio says so outright:
     "a source does not prove a proposition true; it says WHENCE
     IT CAME." Two different questions that ordinary speech
     merges: whence I received it, and on what it rests. I got
     it from a book; the book rested on stylistic comparison.
     So the library now carries FOUR DISTINCT AXES for
     qualifying a claim, each with a home:
       gradus_assensus  how firmly held
       fons             whence received
       fundamentum      on what it rests
       incertitudo      how much numerical error
     Built across forty rounds by four different domains, and
     only visible as a set when laid side by side — which is the
     round-50 lesson repeating: coining one at a time is safe for
     CORRECTNESS and unsafe for CONNECTEDNESS.

307. `verificabile_ab_alio` is the property that earns the genus:
     a document or a measurement can be inspected by anyone, an
     expert's estimate cannot — so disputes about the first can
     be settled and disputes about the second only met with
     another expert. Not a defect but a CONDITION worth knowing
     before the argument starts.
     `autopsia` recovered as the technical word (auto-opsis,
     self-seeing) that medicine and history both use, and it
     carries the sharpest nota: THE FIRMEST FOUNDATION IS THE
     LEAST TRANSMISSIBLE. Nobody can use another's autopsia —
     told, it becomes testimony, and a grade is lost at every
     retelling. Which is precisely what `corpus_institutionis`
     confesses about this library's own sourcing.
     `praesumptio` distinguished from `fictio_iuris` in one line:
     the fiction KNOWS the contrary, the presumption does not.

308. THE FOUR PROPERTIES WERE RE-TYPED, not merely annotated:
     each now carries `genus="fundamentum" modulus="iudicium"`
     through the cross-model property-typing mechanism that
     already existed, and their local `<optio>` lists were
     DELETED — a library-typed property takes species of the
     genus, not a private enumeration. Leaving the optios would
     have been the incoherent half-migration that looks finished.

309. A GATE ON PROSE — the layer nothing watched. RATIO.md was
     written round 42 expressly to stop orientation material
     drifting; round 50 found it seven runs stale; round 59 found
     it seven runs stale AGAIN. The same document, the same
     interval, twice — which makes it a mechanism failure, not an
     oversight.
     So instead of only refreshing it: the validator now compares
     the highest "coverage loop, run N" in the worklog against
     the highest row of RATIO's loop table, and emits a monitum
     on mismatch. PROVEN BY THE DRIFT IT WAS BUILT FOR — it fired
     "RATIO.md lists 8, worklog has 15" before the refresh and
     went silent after.
     It watches the one thing about prose that CAN be checked (a
     number appearing in two places), which is the whole trick:
     do not try to gate meaning, gate a countable shadow of it.

310. RATIO refreshed with runs 9–15, and with the three MODES the
     loop has grown that were never written down: pure
     gap-finding (1–8), aiming a domain at a ONE-ROUND-OLD GENUS
     on purpose (9), and aiming a domain at a whole CLUSTER to
     test whether the genera describe the problem or the
     technology (11–12). Plus the two disciplines learned by
     being burned (collision-check before walking; write the
     trigger when you refuse) and the echo trap.

MEASURED after: 33 exemplaria / 177 genera / 384 res / 519 arcus
/ 0 vulnera / 24 monita / dubia 14 / umbrae 2.

---

## 2026-08-06 — round 60: THE LOADER (lib/natura.c)

311. FIRST RUNNING CONSUMER. Until today every consumer of natura
     was us, at design time: the validator is shell over xmllint,
     the apparatus is shell over grep, and no program had ever
     loaded a genus. Meanwhile METAMODULUS §8 is titled "the
     loader's contract" and grades rules [E]/[S], where [S] means
     "needs genus-inheritance resolution, which is precisely the
     loader's job and the reason to build it." The doctrine was
     written ahead of the software for eighteen rounds. This
     closes it.

312. THREE DECISIONS, taken before a line was written:
     (a) FILES STAY TRUTH, graph is a projection. No emitter, no
         mutation API, ever in v1 — same liber/tree doctrine as
         sententiae. A loader that can write becomes a printer,
         and a printer has to round-trip.
     (b) HYBRID DEPTH. Typed C structs for the SKELETON only
         (bibliotheca/exemplar/genus/res), each holding its
         StmlNodus; everything else read from the DOM on demand.
         27 elements as structs would mean every format evolution
         churns C. The format can now grow without touching C.
     (c) TWO PHASES: legere (parse+index one model) then nectere
         (link all, resolve, enforce). Cross-model references
         cannot resolve until every model is in — compile, then
         link, the shape C programmers already trust.

313. RULES 9-13 ARE NOW ENFORCED, and they found things on the
     first corpus contact that four rounds of shell validation
     could not, because the shell has no inheritance walk:

       systema_operativum  ortus cited 8x, NEVER DECLARED
       distributio_systematis (3 of those 8 — separate genus,
                               no sub=, so it inherits nothing)
       implementatio_linguae  ortus cited 2x, NO actiones block
       opus_scriptum          desertio cited 1x

     Eleven real violations, all of one shape: an eventum citing
     an action nobody declared. The shell could see that `ortus`
     existed SOMEWHERE and that the citation resolved to a
     string; only a loader that walks sub= can ask whether it
     resolves FROM HERE.

314. THE PRETTIEST OF THE ELEVEN — opus_scriptum's `desertio`
     IS declared. As a `per=` on a TRANSITUS, not as an actio.
     The event recording Aquinas abandoning the Summa cited the
     name of a state change in the slot that wants the name of an
     act. Two adjacent vocabularies that look alike and read
     alike, confusable by anyone including the person who wrote
     both lines three feet apart.
     Fixed by declaring `deseri` (passive infinitive, matching
     componi/exprimi/edi) and retargeting the citation. BOTH
     names stay, because both are true: `desertio` is what
     happens to the STATE, `deseri` is what happens to the WORK.
     The slot for an act wants an act.

315. ONE LOADER BUG, found the same way and worth as much as the
     eleven: `etiam=` — essential dual membership — was in §3 and
     §4 and not in my inheritance walk. `scriptum_conchae` is a
     species of scriptum_exsecutabile AND `etiam="plagula_fontis"`,
     and its `scripta_in` relatum is declared only on the second
     parent. Reported as a rule-11 violation; the corpus was
     right and the loader was wrong.
     Fixed on NaturaRes (etiam is a species-level attribute per
     §4, not a genus-level one) with in_membris_invenire walking
     the primary chain then the secondary. Rules 9/10/11/13 all
     go through it now, and the test fixture grew a `corona`
     species whose properties, relation and event ALL resolve
     through the etiam chain alone.

316. ONE DOCTRINE QUESTION LEFT STANDING, for Fran:
     `editio.index_isbn` is typed `genus="isbn" modulus="identificatio"`
     — but `isbn` is a SPECIES of schema_identificandi, not a
     genus, and §5.2 says a library-typed property takes a GENUS.
     Not made a vulnus, because rule 2 (relations) already
     accepts "a genus OR a dictionary entry", and §4b insists an
     address is RANK-BLIND on purpose. Made a MONITUM instead,
     carrying the real observation:
       TYPING A PROPERTY BY A SCHEME MEANS THE PROPERTY'S
       APPARATUS DESCRIBES THE SCHEME, NOT THE VALUE.
     `opacus=falsum` is true of ISBN-as-a-scheme and says nothing
     about this book's ISBN. Same category the format already
     knows elsewhere (`gerens`: the species macOS never sleeps,
     an installation does). We have no way to say "a value drawn
     from scheme X" as distinct from "a thing which is an X".
     One occurrence — recorded, not built.

317. THE GATE IS A DIFFERENTIAL, not a new opinion. The loader's
     apparatus was run against `natura_quaere.sh -a` across ALL
     177 genera: 1286 members, sorted, diffed — ZERO lines of
     difference. The C reproduces the incumbent exactly, and adds
     179 actiones the shell projection never emitted (deliberate:
     knowing the available actions is exactly what rule 13 makes
     you need). Script kept as tools/natura_apparatus_umbra.sh so
     the differential can be re-run whenever either side moves.

318. AND THE DIFFERENTIAL LIED FIRST. Its initial run showed 363
     differing lines, all of the shape "C finds inheritance the
     shell misses" — a tidy, plausible story about the shell
     under-reporting nested genera. It was false: I had written
     my own `maiores` and read corpus fields $4/$5 as (sub,
     modulus) when natura_quaere.sh's own maiores() reads them as
     (modulus, sub). Reimplementing the incumbent instead of
     calling it invented a difference and then explained it.
     THE TELL WAS THE STORY BEING GOOD. A differential whose
     disagreements all point one flattering way is reporting on
     its own harness. Second run after using the tool's own field
     order: 1286 vs 1286, zero.

MEASURED after: 33 exemplaria / 177 genera / 384 res / 519 arcus
/ 0 vulnera (utroque instrumento) / 1 monitum novum (regula III).
probatio_natura: 62 assertiones, omnes praetereunt.

---

## 2026-08-06 — round 61: `.genera`, and a gate at the moment of writing

319. EXTENSION CHANGED: `natura/*.stml` -> `natura/*.genera`, 33
     files, via `git mv` so history follows. Fran's proposal, and
     the argument that carries it is that **STML is a SYNTAX, not
     a GENRE** — this repo also writes silva queries, grammar
     fixtures and vitrea layouts in it. A taxonomy file obeys
     more: the closed vocabulary of §3/§4 and the loader's
     contract in §8. So `.genera` is to `.stml` what `.svg` is to
     `.xml` — same grammar, its own schema, its own validator.
     The practical payoff is that a tool can now know WHICH
     CONTRACT APPLIES without opening the file, which is exactly
     what an edit hook needs.
     Worklog entries before this one still say `.stml`. LEFT
     ALONE ON PURPOSE: this log is append-only history, and those
     entries describe a time when the files really were .stml.
     Rewriting them would make the record lie to make the present
     look tidy.

320. THE HOOK EXISTS BECAUSE THE LOADER DOES, and the number is
     the whole argument:

       tools/natura_visus.sh   14 s
       bin/natura_examen        0.03 s     (480x)

     Fourteen seconds cannot live in a PostToolUse hook. Thirty
     milliseconds can. This is the first thing the loader bought
     that shell could not have been optimised into — and note
     which direction the win runs: the C tool is not only faster,
     it sees MORE (rules 9-13 need the inheritance walk).
     `tools/natura_examen.c` + `tools/natura_struere.sh` ->
     `bin/natura_examen`; `.claude/hooks/natura-custos.sh`
     registered PostToolUse, matching the censor-custos PUSH
     discipline: clean run = NOTHING in context, findings only
     inject `additionalContext`, never block the turn.

321. THE HOOK'S LOAD-BEARING DECISION — **it always loads ALL 33
     models even when judging one file.** Rules 2/3/4 cross model
     boundaries, so a single-file judgment cannot see them: edit
     `mensura` and the reference that breaks may be in `pharmacon`.
     `-plagula` therefore filters what is DISPLAYED and never what
     is CHECKED, and a break anywhere still exits 1. There is a
     third branch for exactly that case — "this file is sound but
     the library carries N vulnera elsewhere" — because a
     cross-model break is USUALLY caused by the edit that just
     happened, and reporting only on the edited file would hide
     precisely the failure the whole design exists to catch.

322. EXIT 2 = NOTHING RAN, and the hook treats it as a DEFECT of
     the gate rather than as health. If the radix is empty or
     moved, `natura_examen` says so and the hook says so; a gate
     that silently passes when it ran on zero files is worse than
     no gate, because it manufactures confidence. Same reason
     `bin/natura_examen` missing produces a message rather than
     silence.

323. AND THE PIPE LIED, ON SCHEDULE. Verifying that a planted
     dangling relatum was caught, I read `exit=0` from
     `... | tail -2` and nearly recorded that filtering to a clean
     file wrongly reported health. It was `tail`'s status, not the
     tool's; unpiped it was 1, correct all along. The documented
     trap, walked into while testing the very discipline it
     belongs to. **Never gate on `$?` after a pipe** — the one
     habit that keeps re-earning its place in memory.

324. RULES 9-13 AND 14 PROMOTED TO [E] in METAMODULUS §8, with a
     two-enforcer table (visus = slow, emits INDEX/HTML/monita;
     examen = fast, enforces more, feeds the hook) so a later
     session does not have to guess which one is authoritative
     for what. §2 records why the extension exists. §8 records
     rule 3's second case as a MONITUM with its reasoning.

MEASURED after: 33 exemplaria / 177 genera / 384 res / 0 vulnera
sub UTROQUE instrumento; differentia apparatus adhuc 1286 = 1286,
ZERO lineae; probatio_natura 62/62.

## 2026-08-06 — round 62: gradus 0 — visus rogat, non duplicat

325. REGULA I ERAT MENDACIUM SCRIPTUM. METAMODULUS §8 dicebat
     "well-formed XML — [E] (xmllint)". Nihil eam cogebat.
     `natura_visus.sh` xmllint ut MACHINAM XPATH solam adhibebat,
     cum `2>/dev/null`, unde plagula malformata chordas vacuas
     ubique reddebat: genera eius evanescebant, VULNERA nulla
     nuntiabantur, exitus ZEPHYRUM erat. Peius quam non probare -
     plagula in 'exemplaria' NUMERABATUR (34), ergo processa
     videbatur dum nihil ex ea legeretur.
     Genus idem quod hodie ter iam inventum est: **porta quae
     'nihil mali' et 'nihil factum' discernere non potest.**

326. CONTRACTUS MUTATUS: regula I iam STML BENE FORMATUM poscit,
     non XML (Fran, hodie). Mensuratum prius quam mutatum -
     stml.c iam REICIT quod refert: tag male clausum, tag non
     clausum ad finem, radicem non clausam, clausuram sine
     apertura, quotam non clausam, commentum non clausum. Id est
     tota classis quae documentum vere corrumpit.
     Differentia vera contra xmllint SEX casus est (attributum
     sine quotis, attributum duplicatum, radices duae, textus
     ante radicem, ens ignotum, tag vacuum), quorum QUATTUOR
     ambulationem arboris post-parsationem solam poscunt - arbor
     iam exstat. GLR NIHIL ad rem: GLR grammaticis AMBIGUIS
     servit (C typedef contra identificatorem), STML ambigua non
     est.

327. GRADUS 0 = VISUS ROGAT, IUDICIUM NON DUPLICAT. Auctoritas
     regulae I est bin/natura_examen; visus eum vocat et sistit
     si quid parsari nequit. Nulla altera implementatio regulae I
     nascitur - una res, una sedes. SOLA regula I obstat: cetera
     vulnera analysin visus non corrumpunt, ergo eum non morantur.

328. TRES PORTAE, non una, quia tres modi TACENDI erant:
       porta ABSENS  -> exitus II (non ZEPHYRUM: instrumentum
                        quod numquam cucurrit successus non est)
       porta STALA   -> exitus II (lectio legati: binarium stalum
                        idem mentitur ac binarium absens; lib/
                        natura.c aut include/natura.h aut
                        tools/natura_examen.c recentior = sisto)
       parsatio      -> exitus I, plagula et regula NOMINATAE
     Porta stala se statim probavit: primo cursu flagravit, quia
     lib/natura.c horam ante tacta erat dum uncos probaremus.

329. GUARD TRANSITORIUS, et cur necessarius est. Contractu ad STML
     mutato, `<proprietas multiplex/>` (attributum booleanum) iam
     LICET - sed visus adhuc per xmllint interrogat, ergo talis
     plagula eum frangeret EODEM MODO TACITO quem modo clausimus.
     Ergo visus insuper asserit xmllint plagulam adhuc legere
     posse, et RECUSAT CLARE si non. Probatum utroque modo:
     onerator eam ACCIPIT (recte - STML valet), visus RECUSAT
     (recte - instrumentum eius non valet).
     Lex: contractus et instrumentum dissentire possunt; dissensus
     TACERE non potest. Guard delendus cum gradu II (XLVIII sedes
     xp() ad lib/selectio.c migrantes).

MEASURED: 33 exemplaria / 177 genera / 384 res / 0 vulnera; portae
IV probatae (sana, malformata, STML-non-XML, porta absens).

## 2026-08-06 — round 63: gradus II — xmllint ex viso abiit

330. XLVIII VOCATIONES XMLLINT DELETAE. Quod illic stabat non erat
     'quaedam interrogationes' sed PARSATIO ALTERA TOTIUS CORPORIS
     cum interpretatione altera: onerator plagulas per stml
     legebat, visus easdem per xmllint. Duo parseres, unum corpus,
     dissensus semper possibilis - et TACITUS, quia xp() errores
     in /dev/null mittebat (unde vitium gradus 0).
     Consilium ergo NON fuit 'xmllint per selectio substituere'.
     Onerator haec omnia IAM IN MEMORIA TENET; ergo tabulas
     EMITTIT (natura_examen -tabulae) et visus solam
     praesentationem agit. Una parsatio, una veritas.
     natura_visus.sh: DCCCX lineae -> DCLVII.

331. PORTA = DIFFERENTIA, ut in oneratore ipso. Ante mutationem
     tabulae XII, INDEX.md, HTML omnia servata sunt ut oraculum.
     Post: XI ex XII tabulis PRIMO CURSU octetim congruerunt
     (genera 177, res 384, arcus 519, glossae 177, resgen 384,
     citationes 26, assensus 26, fontes 13, transradices 9,
     umbrae 3, validitas 1). Duodecima (dubia) linea UNA discrepuit.

332. ET DISCREPANTIA ERAT VERA DIFFERENTIA SEMANTICA, non erratum:
     `stml_textus_internus` spatium album PRAECIDIT; xmllint
     `string()` non. Mensuratum in vitro: '<d>\n    alpha.\n  </d>'
     -> stml 'alpha.' (VI), xmllint ' alpha. ' quod sed ad
     'alpha. ' coartat. Unde campus unus (dubium functio_pura)
     spatio finali caret.
     ACCEPTUM ut MELIUS: spatium finale strepitus xmllint erat,
     quem visus in INDEX.md et HTML ferebat. INDEX.md et HTML
     ergo UNA LINEA differunt, eaque praedicta ANTE cursum.
     Differentia quae explicatur ante quam apparet non est
     defectus sed mensura.

333. REGULA XIV COACTA DIVIDENDA SOLVIT. Duae sedes xp() manebant
     quae SOLUM modulum radicis poscebant. Cum regula XIV
     (modulus= stirpi plagulae aequatur) ab oneratore nunc
     COGATUR, nomen plagulae fide dignum est: `basename` sufficit,
     et parsatio altera non opus. Porta cogens usum aliorum
     instrumentorum SIMPLICIOREM facit - lucrum quod portae raro
     numeratur.

334. GUARD TRANSITORIUS DELETUS eodem commisso quo natus est.
     Rota LXII eum aedificavit quia contractus (STML) et
     instrumentum (xmllint) dissentire poterant; xmllint abeunte,
     nihil custodiebat. Guard cum causa sua scriptus fuit et cum
     causa sua periit - quod est cur causas scribimus.

RESTAT: natura_quaere.sh XXII sedes xp() adhuc fert (corpus suum
G/P/M/A/R/S aedificans). Dependentia xmllint domus ergo nondum
tota abiit - visus liber est, quaere non.

MENSURATUM: 33 exemplaria / 177 genera / 384 res / 519 arcus /
0 vulnera / 24 monita; INDEX.md et HTML una linea praedicta
differunt; probationes natura 65/65, stml 577/577.

335. GRADUS II PERFECTUS: natura_quaere.sh quoque liberatus (XXII
     sedes). Domus naturae xmllint iam NUSQUAM exsequitur - tres
     parsationes eiusdem corporis (onerator per stml, visus per
     xmllint, quaere per xmllint) in UNAM redactae sunt.
     natura_quaere.sh: CCXVII lineae -> CLVII.

336. ET HIC DIFFERENTIA VITIUM VERUM INVENIT, non strepitum.
     Corpus quaerentis primo cursu CXXXVIII lineas fractas tulit:
     optiones electionis CONCATENATAE sine spatiis -
     'disciplinastructuracryptographica' pro 'disciplina structura
     cryptographica'.
     CAUSA: `stml_textus_internus` nodos textus INTER elementa
     (spatium album solum) OMITTIT, ergo textus optionum
     adiacentium coalescit; xmllint eos servabat. Eadem proprietas
     quae in dubiis strepitum tollebat hic CONTENTUM CORRUMPEBAT.
     Emendatum optiones SEPARATIM iungendo, non per textum
     internum generis.
     LEX: eadem differentia parseris in uno campo emendatio est et
     in alio corruptio. 'Melius' non est proprietas parseris sed
     proprietas USUS - unde differentia per campos currenda est,
     non semel iudicanda.

337. RESIDUUM POST NORMALIZATIONEM: DCCCLXXXVI lineae spatio
     finali solo differunt (stml praecidit, xmllint non) -
     acceptum ut supra. Normalizatione spatii finalis adhibita:
     ZERO.

338. PORTA APPARATUS CIRCULARIS FACTA EST, et id notandum antequam
     obliviscamur. `tools/natura_apparatus_umbra.sh` corpus
     quaerentis legit; corpus illud nunc ab ONERATORE venit, ergo
     differentia oneratorem contra se ipsum probaret. Migratio
     ipsa contra ORACULUM SERVATUM (corpus xmllint-genitum, ante
     mutationem captum) probata est: 1286 = 1286, ZERO.
     Deinceps porta illa nihil independens probat. Quod eam
     substituit: probationes stml/natura (642 assertiones) et
     differentia INDEX.md contra oraculum. Porta quae vim suam
     amisit et tacet peior est quam porta nulla - ideo hic scriptum.

MENSURATUM: visus 14 s -> 4.5 s; INDEX.md una linea praedicta
differt; corpus quaerentis ZERO post normalizationem; apparatus
1286 = 1286 contra oraculum independens.

## 2026-08-07 — round 64: the two things the generator found in natura

Both were surfaced by **building the projection**, not by reading the models.
That is the §3.3 rule of the natura→canon spec paying out: *if the generator
needs a distinction natura does not carry, that is evidence natura may be
missing a real one.*

339. `gerens=` HAD NO STATED DEFAULT, and the whole instance projection turns
     on it. A `machina_statuum` with `gerens="species"` describes states of
     the KIND and must not become an instance attribute; only
     `gerens="individuum"` projects. `natura.canon` declared the attribute as
     an optional `electio` with **no `ordinarius=`**, so the generator had to
     guess for every machine that omits it.
     Now stated: `ordinarius="individuum"`, with the reason in the `nota`.
     Which default is right was a natura question, not a generator question.

     THE CHECK IS THE DIFF, NOT THE EDIT. The generator already assumed
     `individuum`, so `./tools/natura_canones.sh -probare` had to come back
     `canones 34 recentes` — and did. Had output shifted, the assumption was
     wrong and the diff would have shown exactly where.

     AND THE CHECK DID NOT COVER MY MISTAKE. Writing the attribute I closed
     it `/>` and **deleted its two `<optio>` children**. All three of these
     stayed green: `canon_examen` on `natura.canon` (canon cannot express
     "electio implies optio" — a co-occurrence, explicitly out of scope per
     `canon.h`), the canon-freshness gate (the generator reads `.genera`, not
     `natura.canon`, so no output moved), and the test suite.
     What caught it was the **generated METAMODULUS catalog**, where
     `electio{species|individuum}` became a bare `electio=individuum`.
     LEX: a gate that watches the output of X does not watch X. The catalog
     earned its keep here precisely because it is a projection of a
     *different* axis.

340. `valor` MEANS TWO DIFFERENT THINGS, now recorded as a `dubium` on the
     `planta` genus. `rosa` fixing `habitus` to `frutex` **constrains** — a
     rose that is a tree is not a rose. `rosa_canina` carrying
     `altitudo_matura` 3 merely **typifies** — a particular bush may well be
     two metres. Same element, two readings.
     Both map safely to `ordinarius=`, because a stated default never
     rejects, so the first pass did not have to resolve it. But a genus
     author writing a `<valor>` should know which of the two they are doing,
     and nothing said so. Resolving it needs a mechanism that distinguishes
     constraint from exemplar, and that is an editorial judgment for Fran.

MENSURATUM: `natura_canones.sh -probare` → `canones 34 recentes` (proiectio
immota); `natura_visus.sh` → exit 1 cum tribus vulneribus stantibus solis
(pharmacon:185, vectura:144, plagula_computatralis:406), nullum quartum;
dubia 14 → 15.

## 2026-08-08 — glossae: vernacular documentation layer

The `<glossa lingua="en">` element landed (spec glossae): localized
reader-facing docs on genera, ISO 639-1 keys, the medieval glossa
arrangement — vernacular glosses on a canonical Latin text that
never rename it. Three findings worth keeping:

1. ZERO loader change. The hybrid-depth design (typed skeleton +
   retained StmlNodus) absorbed a new element exactly as its header
   comment promises — glossae reach every consumer via genus->nodus.
   Grammar admission was two data edits in natura/natura.canon
   (liberum under genus + elementum glossa with lingua necessarium);
   lib/canon.c needed nothing, the generic necessarium machinery
   enforces lingua.
2. The `la` column is definitio-OR-differentia, not definitio alone.
   First real-corpus run of natura_glossae showed la 123/177 with
   `animal` "missing" — but sub-genera carry only differentia BY
   DOCTRINE (definitio = genus proximum + differentia; natura_examen's
   derived gloss falls back the same way). Under the correct rule la
   is 177/177: the corpus was disciplined all along, the naive rule
   manufactured 54 false gaps. LEX (again): run every new
   filter/count on the REAL store before believing it — fixtures
   share your assumptions.
3. METAMODULUS §3 is GENERATED — the first draft of this change
   hand-edited the catalog table inside the GENERATUM span before
   noticing; the right move was editing natura.canon (already done in
   step 1) and running natura_metamodulus_generare.sh. The generated
   span said so at its top; read the span markers before editing any
   catalog.

MENSURATUM: la 177/177, en 1/177, fr 1/177 (planta seeded en+fr);
glossae flow genera -> planta.canon + individua.canon (blast radius
exactly 2 files); pagina natura/cocta/glossae.html deterministic,
-probare byte-compares.

## 2026-08-08 — the debt paid in an afternoon

Six parallel Opus glossators (partitioned BY FILE — no conflicts, no
worktrees needed) authored all 352 en/fr glossae in one pass:
2232 insertions, 0 deletions, both gates clean per agent, census
177/177 in all three languages, page free of every deest marker.
Each agent surfaced its translation judgments (universitas = corps
constitué not university; codex = FRBR copy not manuscript; iussum
kept apart from praescriptum per the file's own warning; tessera =
titre, with "au porteur" self-caught as contradicting the genus's
own modus_transferendi). One agent caught the verify-recipe flaw:
loose greps match genus NAMES across modules — filter on the
modulus COLUMN. Regeneration ran ONCE after all six landed (the
agents each independently declined to run it mid-flight — shared
state discipline held).

## 2026-08-08 — the documentation loop closes: annotatio, scriba, and the cheap batch

The genera-generation loop on "documentation" (descriptor's 15-miss
ledger, project-specs/documentatio-lacunae.md) resolved into corpus
work this day. Modeling-side record:

- ANNOTATIO minted (genus 178, iudicium): the corpus had NAMED its
  own missing genus in iudicium's header ("tres propositiones, tres
  probatores") years-in-corpus-time before any loop found it. The
  load-bearing reading: the header's axis is the PROBATOR, not the
  writer — so TOLERA inscriptions (human-written, tool-proven) and
  canon_examen verdicts (tool-derived, tool-proven) are ONE kind;
  inscripta/prolata species can come when content forces. Relation
  named `probator` (the header's own word — 'instrumentum' would
  shadow corporalia). gradus_assensus deliberately ABSENT: the
  absence is the differentia from iudicium. annotatio_machinae
  (commentarium species) is the INSCRIPTION; this is the
  PROPOSITION — the inscription carries it.
- SCRIBA beside auctor on opus_scriptum: auctor respondet, scriba
  exsequitur (Rom 16,22 — Tertius/Paulus). auctor stays
  persona-CLOSED and, since scoped-super, that closure has TEETH
  (probe: programma as auctor = CITATIO_IRRITA). scriba ad='*'
  multiplex — persons, workshops, machine ministers all scribe.
  The agent GENUS is deliberately parked (01KZFM3GW0) until a real
  census forces the tag; Varro's 'instrumentum vocale' is among
  the recorded candidates, Fran's call alone.
- Rulings healed the two oldest tolerated wounds the same day:
  dies takes '-YYYY' (HISTORICAL a.C.n., no year zero, ISO
  astronomical explicitly rejected); exsequitur targets
  versio_architecturae with x86_16 minted (the founding ISA that
  named the family — resident 385). semina.census judges PLANE
  SANA for the first time since it exists; tolerance apparatus
  deleted, omnia==0.
- Cheap batch: coquitur ad='*' (regula II taught the sanctioned
  open form via the hook); contentum across the intention
  quadrangle + gravitas on vitium; ordo on postulatum BORN with
  minimum="1" (fines dogfooded); auctoritativa on expressio;
  UNIVERSAL <nota> on every generated kind-tag (curatorial margin
  the document layer lacked — census explanations no longer hide
  in unqueryable XML comments).

Remaining from the loop: documentatio.genera itself — status and
the five open forks in nota 01KZFMTPR2.

## 2026-08-08 — documentatio.genera: the module the loop was for

The reader-task module shipped (spec project-specs/documentatio-spec.md,
plan documentatio-plan.md, decretum 01KZFPWPXH). The five forks all
resolved in one brainstorm with Fran:

1. **Genre = intent on the WORK; fit = verdict on the WALK.** A manual
   is authored as a manual (finis); "served this reader" is the walk's
   sentence. The same page can be intended-dictionary and
   measured-failed-manual without contradiction — which is literally
   what happened to glossae.html.
2. **defectus_documenti minted, NOT vitium widened.** Differentia
   deliberately parallel without inheritance: vitium = intention vs
   BEHAVIOR; this = owed vs what a READING yields. Species by force:
   deceptio (has a victim) > silentium > ambiguitas.
3. **The walk bears the failure sentence**, citing annotatio
   (agent-reader) / iudicium (person) / invenit → defectus.
4. **ambulatio_lectoris = cross-model species of probatio.** The
   enabling move was the L2 precedent replayed exactly: probatio's
   genus was already general, only exercet's closure wasn't → ad="*"
   at the parent, species narrows back to opus_scriptum.
5. **Parts are works.** pars_de ALREADY EXISTED on opus_scriptum
   ("pars Summae") — fork 5 needed zero mechanism. hortus.census is
   an exemplum now; a legend is a small work inside a host work.

Names: collision check BEFORE drafting paid again — bare `lectio` is
taken (elementa_programmatis state-read), `exemplar` is
format-reserved; hence ambulatio_lectoris and exemplum.

Proof: natura/exempla/documentatio.census — the peregrinus scenario
re-told in the new vocabulary, GREEN from disk on first contact. The
three facts the ledger opened with (L9/L10/L11's unsayables) are now
first-class tags. Suite section XVIII: exercet-aperture oracle
(verified red pre-change), disk-green census, four red probes — incl.
auctor's persona-teeth surviving inheritance into dictionarium.

Corpus after: 34 modules, 187 genera, glossae 187/187 ×3, 36 canons.
Deferred: minister machinalis still parked (01KZFM3GW0; lector/scriba
both ad="*" awaiting it), genre species beyond the measured four,
pars_de multiplex (no measured multi-host part).

## 2026-08-08 — the census arc: rhubarb.census, bin/natura, two chains

Fran's manifest vision (project-specs-adjacent, RM sententiae) landed
as a walk rather than a design: **coverage run 16, aimed at this
repository**. What it produced, in order:

**rhubarb.census** (a791638) — the repo's own census, KAOS-rooted:
one root, nothing floats. Green through five rewrites. It measured
what it was built to measure: the corpus can say "X uses Y" richly
(dependet-ex/adhibet wired 11 res on first pass) and "X exists for
the sake of Y" almost not at all — `causat` was single-valued, so
3 of 23 res carried justification and 19 were orphans-by-poverty.

**causat multiplex** (fd3c228) — the editio_de/L12 move exactly: one
finis, many effects. Orphans 19 → 0. The refused alternative is
recorded: a universal `propter` conflates *enrollment*-reason with
*existence*-reason (a rose in hortus.census serves no project goal),
so justification writes DOWNWARD from the spine. That also makes the
no-orphan check nearly free — all edges concentrate in one section.

**The audit is the finding** (111ed81). Four numbers in the census,
checked 40 minutes after writing: **two were wrong.** Nothing had
changed — I wrote them from memory instead of measuring. Worse, one
"correction" was itself wrong: I condemned `res 385` as unverifiable
because MY method (`grep -c '<res '`) found zero; the number was
right. **A bad anchor yields a FALSE NEGATIVE — it condemns a true
claim while wearing the look of verification.** Hence `<signum>`
entries carrying `via=` (the command), and `observatio.via` in the
model. A nota also rotted inside the same session: "propter
#perennitas (relation lacking)" was true when written and false
forty minutes later when I added the edge.

**The prose sweep** (1742f40) — L5 again, ×4: `causa`, `norma`,
`observatio`, `periculum` all lacked `contentum`, so the ROOT of the
census spine was a final cause whose statement lived in a comment.
All four fixed. Plus `causa.extra_ordinem` (a DECLARED terminus,
which killed the magic string the lint had been grepping for) and
**`veritas_sua` widened to `*` — L13 CLOSED**, four occurrences in
one census.

**Four candidates died on contact**, which is the better half of the
work: `cautio/laqueus` (periculum takes all four house traps; its
`minuitur_per` was already annotated "cautiones"), `mensuratio`
(already a species of fundamentum), `casus` (a forcing case IS an
observatio), universal `propter`. One genus minted: **`praeceptum`**
— a rule of an art, LEARNED and experience-corrected, vs `norma`
which is promulgated and cited by index. It carries `natum_ex`
because a maxim without its case is a platitude everyone nods at.
House doctrine now enrolled as #ars-naturae (peritia_communis) with
ten praecepta (e7aa93b).

**bin/natura** (f79d4d3, 6bb0b46, 02ceb7e) — born from a measured
failure: my collision check piped `natura_quaere.sh` through
`head -c 60`, which cut the `[species:...]` tag, so `mensuratio`
looked free while sitting under `fundamentum`. The natura gate caught
it AFTER I'd written the genus. The fix I first wrote to memory was
"read the whole row" — i.e. discipline, which is the shape of a
tool-shaped hole. So `quaere` ends with a **verdict line** that
survives truncation, and the verdict is **tri-state**: OCCUPATUM /
**UMBRATUM** (name used but undescribed — minting it FULFILLS the
shadow) / LIBERUM. natura_quaere.sh is now a forwarding shim.

**Two chains** (6800bc2, decretum 01KZH3Y1QH) — Fran asked whether
nesting is honored. It wasn't: `<species isbn>` holds isbn_10/13 and
`<species malus_domestica>` holds granny_smith, but the graph
flattened all res to the ambient genus. **The flattening was
deliberate and half-right** — the comment says "profunditas
Porphyriana est taxinomia, non catena apparatus", and that IS correct
for inheritance, since a species declares no apparatus (grammar
verified: no proprietates/relationes/machina/actiones/partes; it only
fixes values). The defect was that taxonomy was acknowledged and not
stored. So: ADDITIVE. `NaturaRes` gains `continens` + its own
`res_suae`; `genus_suum` and `genus->res_suae` keep their meanings,
the latter deliberately FLAT. **Regression proof = generated canons
byte-identical** after the change.

### What the shell lied about (three times, one day)
- awk state machine fooled TWICE by self-closing `<species .../>`
  (no closing tag → depth never resets) — invented nested genera
  that don't exist.
- `cut -c9-` mangled names because `uniq -c` pads its count field to
  variable width.
- `perl -pi -e` interpolated `$CORPUS` and `$(dirname ...)` as ITS
  variables, mangling a script into garbage.
Simple counts survived (tool and grep agree at 432 relations);
STRUCTURE did not. Hence `bin/natura census` + `relationes`.

### Open, with evidence attached (nothing decided)
- **Relation vocabulary is impoverished**: 432 relations, 325 names,
  **272 used exactly once (83%)**, and the singletons CLUSTER —
  12 names for origin/causation, 10 for part-whole, 4 each for
  dependency and succession. `<relatio>` carries a target and three
  booleans; `<proprietas>` can be typed, bounded, defaulted, given
  options. The corpus states what things ARE richly and how they
  STAND crudely. **89 open closures (`ad="*"`)** are the same bill.
- **Roles have no home**: round 50 already ruled GENERA NUDA MUNERA
  SUNT (cliens/servus ARE roles) — relation-ends have no names, so
  roles get reified into genera. `munus` and `potentia` both free;
  they are NOT synonyms (position vs capacity).
- **genus-vs-species IS the proprium question**, unmarked: a
  `<genus sub=>` ADDS apparatus, a `<species>` only DETERMINES what
  it inherited. Real distinction, carried by which tag you type.
  `isbn` is plausibly mis-filed (its definitio names four parts in
  prose it cannot declare).
- **finis**: 39/188 genera state their END in prose only, and the
  corpus's own rule says the end CONSTITUTES the kind ("quod ad finem
  ordinatur ab illo fine formatur"). Cross-cutting, not core.
- **`<valor>` binds or defaults?** — recorded `<dubium>` on planta;
  the projection silently answered "default" (`ordinarius=`).
- Latent: a `<genus>` nested in a `<species>` would lose its
  container (same gap, one level up) — legal, used ZERO times, filed
  01KZHAFCYZ with its trigger.

## 2026-08-10 — Necessitudines: relations become first-class (the classification, then the arc)

**The classification** (this is the part that must survive — raw
material was `bin/natura relationes -machina`, 432 usages, 325 names,
272 singletons/83%). Reading all 325 names against bearers and targets
produced ~17 families and five structural discoveries:

1. **Role-frames are the biggest family (~36 names)** — the singletons
   cluster ON act-genera as participant roles (venditio: emptor/
   venditor/res_vendita/solutum; iter: a_loco/ad_locum/per_viam;
   fiducia: fiduciarius/beneficiarius/res_fiduciaria). Terminus a quo
   / ad quem was independently reinvented THREE times (a_loco/ad_locum,
   missus_a/destinatus_ad, initium/finis). Roles were never
   impoverished vocabulary — they are case frames, and munus's home is
   the named end of a frame.
2. **Morphological fragmentation** — situm_in/sita_in/situs_in are ONE
   relation spelled by the bearer's gender; likewise statuta/statutum_per,
   custodita_a/custodita_per/custoditur_ab. String-keyed names fragment
   on Latin grammar — the sharpest ergonomics finding.
3. **Undeclared converse pairs everywhere** — pollinat/pollinatur_a,
   continet/continetur_in, legit/legitur_a, efficit/efficitur_a… —
   active/passive morphology doing what the format couldn't declare
   (the inversa wound at corpus scale).
4. **The _de pattern (~14 names)**: versio_de, pars_de, editio_de… —
   relation = bearer's kind + "of", a relational-noun shape.
5. **Reuse tracks generality**: de(9), continet(9), asseritur_per(8),
   parit(7), auctor(7) converge; causat appeared ONCE in the dictionary
   while being rhubarb.census's spine. Plus one plain synonym pair a
   dictionary would have prevented: dependet_ex/pendet_ex.

Families (names ≈): munera actuum 36 · norma 27 · iustificatio 25 ·
successio/FRBR 19 · exsecutio 17 · custodia 16 · origo 15 ·
dependentia 15 · locus 14 · mensura 14 · evulgatio 13 · communicatio
13 · causa/finis 12 · pars/totum 11 · mutatio 11 · auctoritas 7 ·
biologica 6.

**The design that fell out**: identity / label / spelling as three
layers; declaration-once (necessitudo with sub= into the family
spine, conversum=, scriptio children), use-sites light with
name-defaulting; frames as <termini> on act-genera with munus= ends;
symmetric with proprietas (a relation slot = named slot typed by a
relation-kind, as a property = named slot typed by a value-kind).

**The arc** (e49a61b..7da1869, 8 tasks): grammar → loader (discrimen
III, rules XX-XXIII+XXV) → binding above the inversa short-circuit →
apparatus chain (the canones fold read titulus from nomen= only, so a
terminus DROPPED SILENTLY — planted as a red test before the fix) →
quaere's fourth walk (pollinat was LIBERUM while alive in two modules
— measured before, OCCUPATUM after) → core module (16 families + 49
workhorses; promulgatio collision measured, evulgatio minted) →
migrations. Venditio's frame proved BYTE-IDENTICAL under the
canones gate (termini at the relationes position, munus = old nomen).
The two standing VULNERA (inversa-as-name: pharmacon
fabricatum_in/coquitur, plagula continetur_in/continet) died by
declaration — **corpus SANUM (VULNERA 0) for the first time**, and
the monolith gained 4 TRUE citations (pollinat + heirs) where an
invisible site had been. Burn-down baseline: 163/432 slots LIGATA
(38%) by name alone, 269 to converge, one aggregated monitum per
module counting them.

Traps for the next reader: the apparatus hoists EVERY element child
of a whitelisted container — a <nota> inside <termini> is a member
node (display paths skip it; the fold skips it via the nomen= guard).
Monitum XX must carry modulus= or -plagula prints it everywhere. The
canones driver skips genus-less modules (the generator refuses empty
canons with exit 2, correctly); the suite's reconciliation counts
COOKING modules + 1, derived not fixed.

## 2026-08-10 — ARBOR PORPHYRIANA: species sunt genera, signum `.` in nomine

The eight-commit arc (e825ce3..b6f5d5c) that unified the kind layer.
Decreta: species = nested-genus sugar (one NATURA_ENS_GENUS, element
name carries only the ROLE); the dot is PART of the element name
(`<.genus>`/`<.species>`/`<.cultivar>`); individuum stays unmarked
(the absolute universale/particulare line is dotted-vs-undotted); the
dot is an open registry for future kind-minters.

**Measured before**: 271 species + 1 cultivar of 385 "res" were kinds
stored as instances; loader had ONE branch for species/individuum/
cultivar; apis was `gradus="genus"` yet uninstantiable; currying-only
species could not recurse (isbn declares nothing, isbn_10 curries
nothing); 13 kind-level valores in planta authored, canon-blessed,
read by NO consumer. **After**: genera 460 / res 113 / VULNERA 0,
`quaere apis` → GENUS (sub animal), `apparatus rosa` shows
`= valor 'durata' -> perennis (determinatio)`.

Structure that made it safe: RENAME FIRST (T4: every literal flips,
species still load as res, gates prove counts UNCHANGED 188/385 and
cocta byte-identical) then UNIFY (T5: dispatch reroute, counts flip
460/113). Separating rename-risk from semantics-risk meant each phase
had a clean reconciliation gate.

Landmines hit and survived:
- **Rule VI vocabulary** read gradus_assensus's res_suae — members
  are now liberi. Missed = 26 certitudo vulnera; VULNERA 0 proves it.
- **strncmp-with-length on unterminated chorda** (quaesitor census):
  dotted names change lengths; missed = counts silently 0. Census
  271/113/1 exact proves all sites hit.
- **The T6/T7 coupling**: widening natura_apparatus with valor/relatum
  members leaked determinations into _apparatum_plicare — a valor
  SHADOWED the real proprietas (electio degraded to textus, 650 lines
  lost from generated canons). The -probare rancor gate caught it;
  the fold now skips determination members (its own path is
  _valores_applicare → ordinarius=). LESSON: apparatus is a shared
  API — widening it audits every consumer, and the generated-artifact
  byte-gate is the audit that cannot lie.
- Conservation proof for regenerated canons: glossae/ordinarius/
  element counts equal per file; only element ORDER moved (document
  order now interleaves former species). semina.census byte-identical
  (113 = individua, filter untouched).
- documentatio-probatio.census VITIA 7 measured PRE-EXISTING
  (judged against pre-arc canon: identical) → quaestio 01KZPSA3G1,
  not this arc's debt.

Deliberate deviation: examen -tabulae/-corpus stay RAW-partitioned
(genera.txt 188 rows, res.txt 385) — they are the "what the files
say" layer and visus is green/self-consistent on them; the graph
layer (examen summary, census, index) carries the new ontology
(460/113). Repartitioning the display layer is a contained follow-up
if wanted.

parens_etiam ported the dual-membership doctrine from res to kinds
(scriptum_conchae etiam=plagula_fontis — apparatus walks both
chains). Rule III monitum "proprietas ad rem solvit" evanuit
CONSULTO — species-typed proprietates are now clean kind references
(that was the point).

## 2026-08-10 — propagatio (36e205a..6d0cba7, same day as the arc)

Four consistency items, Fran-ordered:
1. natura.h DUAE CATENAE comment rewritten (was teaching the
   pre-porphyrian chains — the header everyone reads first).
2. Examen tables repartitioned: genera.txt = all 460 kinds with a
   9th MUNUS column (genus|species|cultivar) and nesting-parent in
   the sub column (PARENS table now full); res/resgen/S = individua
   only; ancestor walks accept any kind order (_kind_titulus_est) —
   an individuum under a species attributes to the species, which
   laika immediately exercised. visus rule-VI mirror moved to
   GENERA; rami() reads the new column; umbra's G-stream covers
   460. MONITA 25→23: arcs declared inside species now attribute
   correctly. Document layer and graph layer agree at last.
3. gerens="genus" (Fran ruled): the OS vita machine bears the
   KIND's lifecycle (macOS vigens, CP/M derelictum) — the canon's
   own nota had said "status GENERIS describit" all along.
4. Differentia exercised with real content: isbn DECLARES
   modulus_custodis + longitudo, isbn_10/isbn_13 CURRY them
   (11/10, 10/13), isbn_13 opens its own praefixum_ean (978|979) —
   the recursion the unification promised, visible in one
   apparatus view. And laika: first individuum under a species
   (canis), public record, res 114.

Trap note: <proprietates> is a bare container — no nota= attribute
(hook caught it twice; prose belongs on the proprietas rows).
Left open deliberately: glossae curation (177/460 hand-written),
quaestio 01KZPSA3G1 (pre-existing red census).
