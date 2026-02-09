# 1x10 "Lingua Nova Structurae" — Historian Pitch

## Overview

Episode 1x10 extends STML with six new features: raw content tags, capture operators, fragments, labels with namespaces, transclusion, and deep clone. Each has deep Roman and classical precedent. This pitch provides historical grounding, Latin vocabulary audit, production design notes, character knowledge mapping, and Earth-side parallels.

---

## 1. RAW CONTENT TAGS (`<code!>literal text</code>`)

### The Problem
Content that contains angle brackets, ampersands, or other markup characters must be preserved *exactly as written*. The parser must not interpret them.

### Roman Precedent: *Verba Certa* and *Ipsissima Verba*

Roman law distinguished sharply between paraphrase and verbatim quotation:

- **Verba certa** ("fixed words") — Legal formulae that had to be recited *exactly* or the legal act was void. The *legis actiones* (actions at law) of early Roman procedure required plaintiffs to speak precise words; one wrong syllable and the case was lost. Gaius (Institutes 4.11) records a famous case: a man suing for destroyed *vines* lost because the statute said *trees* (*arbores*), and his lawyer used the word "vines" instead. The formula was the formula. No interpretation. No substitution.

- **Ipsissima verba** ("the very words themselves") — The practice of preserving exact quotations. Roman historians prided themselves on this. Cato the Elder's *Origines* included verbatim speeches from the Senate. Gellius (*Noctes Atticae*) frequently insists on quoting sources "in their own words" (*ipsis verbis*), specifically contrasting this with paraphrase.

- **Sacred formulae:** The *carmen* (ritual formula) had to be recited without alteration. Livy (1.24, 1.32) records the fetial formulae for declaring war and making treaties — the words were considered effective precisely because they were unmodified. The pontifex who garbled a formula had to start over.

- **Diplomatic correspondence:** Letters between Rome and foreign powers were preserved verbatim in the *aerarium* (treasury archive). Polybius (3.22-26) transcribes the Carthaginian treaties from the original bronze tablets, noting the archaic Latin was already difficult for Romans of his era to understand — but the words were preserved *because they were the words*.

### Dramatic Parallel
Raw content tags are *verba certa* for machines. The parser sees `<code!>` and knows: do not interpret. Preserve. The content inside is a *carmen* — its power comes from its exact form.

### Latin Vocabulary
- **Crudus** (raw, unprocessed) — already used in the API: `crudus` field, `stml_elementum_crudum_creare()`. Good Latin. *Crudus* literally means "raw/undigested" — meat that hasn't been cooked, food that hasn't been processed. Perfect for content the parser shouldn't "digest."
- **Verbatim** — "word for word." From *verbum* (word) + *-atim* (in the manner of). Romans would understand this instantly.
- **Intactus** (untouched) — another option, though *crudus* is better because it implies the content *could* be processed but hasn't been.

### Character Knowledge Map
- **Pliny** would know the *verba certa* tradition immediately — he's a lawyer and encyclopedist who has spent his career preserving exact quotations. His Naturalis Historia quotes sources verbatim constantly.
- **Daniel** would know the *ipsissima verba* scholarly tradition from Gellius and from his archaeological training (preserving inscription text exactly).
- **Marcus** would know the legal formula tradition from Roman engineering contracts — specifications that had to be exact.
- **Lucia** would recognize the linguistic principle: some language is performative (it does what it says) vs. descriptive (it describes what it means).

---

## 2. CAPTURE OPERATORS (`<div (>`, `<) section>`, `<= wrapper =>`)

### The Problem
Restructuring a document tree by having elements absorb their siblings — a node reaches out and makes adjacent content its children.

### Roman Precedent: Legal Adoption, Military Incorporation, *Evocatio*

Rome was a civilization that *restructured social trees constantly*:

- **Adrogatio** (adoption of a *sui iuris* person — someone who was their own legal head of household) — The most dramatic form of Roman adoption. An independent person was absorbed into another family. They lost their old family name, their old household gods, their old legal identity. Their entire *familia* (including dependents, property, and sacra) was absorbed. This is **backward capture**: the adopting family reaches backward and takes in everything that belonged to the adoptee. Gaius, Inst. 1.107: "Adrogation is the adoption of someone who is in their own power... they pass under the *patria potestas* of the adopter with all their dependents."

- **Adoptio** (adoption of someone still under *patria potestas*) — Simpler: a person is transferred from one father's power to another's. This is more like **forward capture** — the new parent reaches forward and claims the child.

- **Military incorporation:** When Rome conquered allied cities, the allied military units were incorporated into the Roman army structure. The *socii* (allies) were reorganized into *alae* (wings) flanking the Roman legions. The unit was captured into a new parent structure while retaining its internal organization. The *formula togatorum* listed which allies contributed what units — a document describing the capture structure.

- ***Evocatio* (calling out the enemy's gods):** Before besieging a city, Roman generals performed *evocatio* — a ritual inviting the enemy's patron deity to abandon them and join Rome. The promise: "You will have a temple. You will have worship. You will have a better home." Macrobius (*Saturnalia* 3.9) preserves the formula. This is **theological capture** — restructuring the divine tree by absorbing another civilization's gods. The most famous case: Juno was *evocated* from Veii in 396 BC (Livy 5.21-22). The goddess literally changed parents.

- **Sandwich capture** parallels **confarreatio** (the most solemn form of Roman marriage) — two independent legal entities (husband and wife) are wrapped together under a new shared structure. The ceremony required witnesses on both sides (the "bread" and the "filling" of the sandwich).

### Dramatic Parallel
Capture operators are *adrogatio* for document trees. A node reaches out and says: "You are now my children. Your identity is preserved, but your position in the tree changes." The backward capture (`<) section>`) is particularly Roman — the section declares itself and *retroactively claims* what came before. Roman law did this constantly: a will could retroactively restructure property that had been held in trust for decades.

### Latin Vocabulary
- **Captio** (a taking, a capture) — already used in the API: `StmlCaptioDirectio`. Excellent Latin. In legal language, *captio* had a specific meaning: a clause in a will or contract that "captured" (bound) property or persons to a new disposition.
- **Adrogare** (to adopt by *adrogatio*) — the most dramatic form. Would resonate with Roman characters.
- **Absorbere** (to swallow, absorb) — more aggressive than *captio*. Could be used for the drama of it.
- **Farcimen** (stuffing, sausage filling) — already used for sandwich capture (`STML_CAPTIO_FARCIMEN`). This is wonderfully earthy Latin — a *farcimen* is literally stuffed meat. Romans would find this hilarious and memorable.

### Character Knowledge Map
- **Marcus** would understand capture operators through the military lens — reorganizing units, incorporating allies.
- **Pliny** would immediately think of *adrogatio* — he documents Roman family law extensively.
- **Methos** might flash briefly on *evocatio* — he's seen gods captured. A 15-second beat at most.
- **Lucia** would note the legal framework: capture has *rules*. You can't capture arbitrarily. Forward capture has a count. Backward capture has a count. Sandwich capture is always one-and-one. This is *lex*, not chaos.

---

## 3. FRAGMENTS (`<#>`, `<#id>`)

### The Problem
Grouping content without adding semantic weight. A container that means nothing except "these things go together."

### Roman Precedent: *Excerpta*, *Florilegia*, and Commonplace Books

The Romans were prolific collectors of fragments:

- **Excerpta** (excerpts) — Roman scholars compiled *excerpta* from larger works. These were collections organized by topic, not by authorship or chronology. The *excerpta* had no "meaning" of their own — they were a grouping mechanism. Pliny's own working method (described by his nephew, Ep. 3.5) was to have a reader dictate passages, which Pliny would annotate and file into *excerpta* organized by subject. The excerpt collection was a fragment — a semantic-free container.

- **Florilegia** ("flower-gatherings") — Collections of notable passages from various authors. The metaphor is gardening: you pick flowers from different gardens and arrange them in a bouquet. The bouquet has no "meaning" beyond "these are beautiful things I chose to put together." The tradition runs from late antiquity through the Middle Ages, but the Roman origin is in the *loci communes* (commonplace books) of rhetorical training.

- **The Digest of Justinian** (533 AD, but drawing on centuries of Roman jurisprudential practice) — Organized Roman legal opinions into *tituli* (title headings) that grouped related fragments (*fragmenta*) from different jurists. The fragment groupings were explicitly non-semantic: a *titulus* about water rights might contain fragments from jurists separated by 400 years who never read each other. The grouping was functional, not authorial.

- **Anthologiae** — The Greek term (*anthologia*, "flower-gathering") was adopted by Roman literary culture. Martial's *Epigrammata* are essentially a curated fragment collection — each epigram stands alone, the book is a container.

### Dramatic Parallel
Fragments are *excerpta* for the DOM tree. They say "these things belong together for now" without asserting anything about *why*. Pliny would find this intimately familiar — his entire working method is fragment-based. His *adversaria* (daily notes) are fragments waiting to be organized into the encyclopedia.

### Latin Vocabulary
- **Fragmentum** — already used in the API (`fragmentum` field, `STML_TOKEN_FRAGMENTUM_*`). Perfect Latin. *Fragmentum* literally means "a broken piece" — from *frangere* (to break). The implication is that the fragment was once part of something larger, or will become part of something larger. It exists in transit.
- **Excerptum** (excerpt, something pulled out) — alternative that emphasizes selection.
- **Fasciculus** (a small bundle) — a physical metaphor: tied together with string, no inherent order, just a bundle.

### Character Knowledge Map
- **Pliny** — fragments are *his life*. His entire working method is fragment-based. He would explain this concept better than anyone because he lives it.
- **Joe** — Watcher records are fragments. Individual observations from different eras, grouped by subject but not semantically unified.
- **Daniel** — archaeological fragments. Pottery sherds grouped in a bag. The bag means "these came from the same context." The bag itself has no semantic content.

---

## 4. LABELS / NAMESPACES (`labels="ui::button state::active"`)

### The Problem
Multi-axis classification. A single element needs to belong to multiple independent classification systems simultaneously.

### Roman Precedent: *Tria Nomina*, Guild Membership, *Cursus Honorum*

Romans lived under multiple simultaneous classification systems:

- **Tria nomina** (three names) — Every Roman citizen had (at minimum) three names operating on independent axes:
  - *Praenomen* (personal name): Gaius, Lucius, Marcus — identified you within your family.
  - *Nomen* (family name): Julius, Cornelius, Aurelius — identified your *gens* (clan). This is your primary namespace.
  - *Cognomen* (branch name): Caesar, Scipio, Crassus — identified your branch of the gens. A sub-namespace.
  - Some had a fourth: *agnomen* — an additional distinguishing name. Scipio *Africanus*. Fabius *Maximus* *Cunctator*. These are labels stacking.

  So "Publius Cornelius Scipio Africanus" is `labels="personal::publius gens::cornelius cognomen::scipio honor::africanus"`. Four independent namespaces, one person.

- **Cursus honorum** (the course of honors) — The sequence of political offices. A senator might simultaneously hold labels from multiple classification axes: `labels="ordo::senatorius magistratus::consul provincia::africa military::imperator"`. The *cursus* was a namespace: it classified you by political rank independently of family, wealth, or military command.

- **Guild membership (*collegia*):** A Roman citizen could belong to multiple *collegia* simultaneously — a *collegium fabrorum* (builders' guild), a *collegium mercatorum* (merchants' guild), a *sodalitas* (religious brotherhood). Each was an independent classification axis. A man could be `labels="collegium::fabri collegium::mercatores sodalitas::mithrae tribus::palatina"`.

- **Census classification:** The Servian classes (*classis*) classified citizens by wealth for military and voting purposes. This was *another* independent axis layered on top of tribe, family, and guild.

### Dramatic Parallel
The `::` namespace separator is the digital *tria nomina*. Roman engineers would understand multi-axis classification instantly because they live in a society that classifies them on at least four independent axes simultaneously. `labels="census::classis-prima gens::aurelia collegium::fabri munus::architectus"` — a first-class citizen of the Aurelian gens who belongs to the builders' guild and works as an architect. Four namespaces. One person. The Romans invented this.

### Latin Vocabulary
- **Titulus** (label, inscription, title) — already used in the API (`stml_titulum_addere`, `stml_titulum_habet`). Excellent. A *titulus* in Roman architecture was the inscription on a building naming its builder, dedicatee, and purpose — literally a label. Honorific *tituli* on statues classified the subject by name, office, and deeds.
- **Spatium nominum** (namespace — "space of names") — a possible Latin coinage for the `::` concept.
- **Genus/species** — the Aristotelian/Porphyrian classification. `ui::button` is genus *ui*, species *button*. The Tree of Porphyry (introduced in 1x09's historical notes) is the direct ancestor.

### Character Knowledge Map
- **Marcus** — the *tria nomina* expert. He lives in this system. He'd explain it through the census: "Every citizen is classified by name, by gens, by tribe, by census class, by guild. Five axes. One person. Your labels do the same for documents."
- **Lucia** — the linguistic precision. "The `::` separator is a genus-species relationship. `ui::button` — the genus is interface, the species is button. This is Aristotelian classification made operational."
- **Pliny** — his encyclopedia IS multi-axis classification. Volume 37 about gemstones classifies each stone by color, hardness, origin, use, and value — five independent axes. He's been doing namespaced labels for decades, by hand.

---

## 5. TRANSCLUSION (`<<selector>>`)

### The Problem
Referencing content that exists elsewhere in the document (or in another document) without copying it. The reference points to the source; if the source changes, the transclusion reflects the change.

### Roman Precedent: *Cento* Poetry, Legal Cross-Reference, *Citare*

- ***Cento* poetry** — A *cento* (literally "patchwork quilt") was a poem assembled entirely from lines of existing poetry, typically Virgil. Ausonius's *Cento Nuptialis* (4th century) builds an entire epithalamium from fragments of the *Aeneid* and *Georgics*. The key: the lines are *references* to Virgil, not independent text. A reader who knows Virgil reads the cento on two levels — the new poem AND the original context. This is transclusion: the referenced content carries its source meaning with it.

  Proba's *Cento Vergilianus* (c. 360 AD) goes further: she builds a Christian narrative of salvation history entirely from Virgilian lines. Genesis, the Gospels, Revelation — all in Virgil's words. The transclusion transcends the original author's intent.

- **Legal cross-reference (*citare*, *referre ad*):** Roman jurists constantly referred to earlier opinions. Papinian cites Julianus. Ulpian cites Papinian. The citation is a reference, not a copy — the original opinion's authority flows through the reference. If Julianus's opinion was later modified (by a rescript or a new interpretation), the modification flowed through to everyone who cited him. This is live transclusion.

- **The *commentarii* tradition:** Roman magistrates maintained *commentarii* (official records) that subsequent magistrates could reference. A governor's *commentarii* might say "Follow the procedure established in the *commentarii* of my predecessor Gaius Verres, year 73, regarding grain collection." The reference pointed to the source. If someone discovered Verres's procedure was corrupt (as Cicero eventually proved), the corruption flowed through to everyone who had transcluded it.

- **Intertextuality in Roman historiography:** Livy references Polybius. Tacitus references Pliny the Elder's histories (now lost). These aren't just citations — they're structural references that depend on the source remaining available. When the source is lost (as Pliny the Elder's histories were), the transclusion breaks. The reference exists but the content is gone.

### Dramatic Parallel
Transclusion is the digital *cento*. The `<<selector>>` tag points to content elsewhere and says: "The canonical version lives there. I am a window into it." Pliny would understand this through his index — Volume I of the Naturalis Historia is essentially a transclusion document, pointing into the other 36 volumes. Daniel would understand it through the *cento* tradition — scholarly composition by reference.

### Latin Vocabulary
- **Transclusio** — already used in the API (`STML_NODUS_TRANSCLUSIO`). A neologism, but well-formed Latin: *trans-* (across) + *clusio* (a closing/enclosure, from *claudere*). "An enclosure that reaches across." Good.
- **Referre** (to carry back, to refer) — the standard Latin for cross-reference. *Refertur ad* = "it refers to."
- **Citare** (to summon, to cite) — stronger than *referre*. In legal Latin, *citare* meant to summon someone or something to appear. A transclusion *summons* the content to appear.

### Character Knowledge Map
- **Pliny** — his index. Volume I is a transclusion document. "I built this. I built this forty years ago. With ink."
- **Daniel** — the *cento* tradition. "Ausonius built entire poems from Virgil's lines. The lines aren't copies — they're references. The reader needs access to Virgil to understand the cento fully."
- **Joe** — Watcher cross-references. "Subject file: Duncan MacLeod. See also: Connor MacLeod (File WC-1118). See also: Darius (File WC-0031, CLOSED)."
- **Data** — the formal guarantee: "A transclusion is a pointer. If the source changes, the transclusion reflects the change. If the source is deleted, the transclusion breaks. This is not a defect. It is a constraint that preserves truth."

---

## 6. DEEP CLONE (`stml_duplicare`)

### The Problem
Creating a perfect, independent copy of a document subtree. The clone is identical to the original at the moment of copying but evolves independently thereafter.

### Roman Precedent: Scribal Copying, *Exemplum*, *Apographum*

Rome was a civilization of copies:

- ***Exemplum* (the authorized copy):** In Roman law, an *exemplum* was an official copy of a document — a will, a decree, a treaty. The *exemplum* had legal authority equal to the original. The *tabularii* (record-keepers) maintained *exempla* of all public documents. Crucially, the *exemplum* was independent: if the original was destroyed (as happened when the Gauls sacked Rome in 390 BC), the *exempla* held in other archives were the surviving authority.

- ***Apographum* (the transcript):** A copy made from an original, often of artwork or inscriptions. Cicero uses the word for copies of Greek statues (*Ad Att.* 1.8). The *apographum* was understood to be a perfect duplicate at the moment of creation but was clearly distinct from the original.

- **The *scriptoria* (copying rooms):** Every major Roman household and temple had facilities for copying manuscripts. Atticus, Cicero's friend, ran what amounted to a publishing house — enslaved scribes copying manuscripts for distribution. Each copy was a *deep clone*: every word, every punctuation mark, every marginal note reproduced. The copy then lived its own life — it could be annotated, corrected, damaged, or lost independently of the original.

- **The Roman practice of *instauratio* (re-performance):** If a religious ritual was interrupted or performed incorrectly, the entire ritual had to be cloned from the beginning — *instauratio*. The re-performance was a fresh copy of the ritual, identical in form but independent in time. Livy records games being *instaurated* up to seven times due to technical violations.

- **Wax seal impressions:** A signet ring (*anulus*) pressed into wax created a *deep clone* of the seal design. The impression was an independent object — it could be broken, melted, or preserved. It was identical to the original design at the moment of impression but was physically distinct.

### Dramatic Parallel
Deep clone is the digital *exemplum*. The `stml_duplicare()` call creates a copy with the same authority as the original. Like a scribal copy of a Senate decree, it is complete and independent from the moment of creation. The original can change; the copy preserves the state at the moment of cloning.

### Latin Vocabulary
- **Duplicare** (to double, to make a copy) — already used in the API. Standard Latin. *Duplex* = double, *duplicare* = to make double.
- **Exemplum** (authorized copy, model, example) — the legal/archival term. Carries authority.
- **Apographum** (transcript) — from Greek, but used in Latin. More scholarly than *exemplum*.
- **Profundus** (deep) — `stml_duplicare` is deep clone. The API comments use "profunde" for this.
- **Superficialis** (shallow/surface-level) — `stml_duplicare_superficialiter` for shallow clone. Good Latin.

### Character Knowledge Map
- **Pliny** — "I have made thousands of copies. An excerpt from my encyclopedia, sent to a colleague — it is a copy. If I revise the encyclopedia, his copy preserves what I wrote before the revision. This is not a flaw. This is how knowledge propagates."
- **Marcus** — architectural blueprints. "We copy a plan for a building. The builders in the north have one copy. The builders in the south have another. They diverge as each team adapts to local conditions. But both started from the same plan."
- **Joe** — Watcher protocol. "We maintain copies of critical files at multiple sites. If the Paris archive burns, the Geneva copy survives."

---

## EARTH-SIDE: GILFOYLE/DINESH INTRODUCTION — HISTORICAL PARALLELS

### Roman *Publicani* and the Proto-Corporation

The season outline places Gilfoyle and Dinesh's introduction in 1x10. Their AI coding firm — independent, uncompromised, refusing to attend the weird retreats — has deep Roman parallels:

- **Societates publicanorum** (companies of *publicani*) — The *publicani* were Roman tax farmers: private citizens who bid on government contracts (tax collection, construction, mining, provisioning armies). They formed *societates* (companies) with shared capital, distributed risk, and — critically — **transferable shares** (*partes*). This is arguably the first joint-stock corporation in history. Polybius (6.17) marvels at the scale.

- **The *publicanus* as government contractor:** Gilfoyle and Dinesh's company mirrors the *publicani* dynamic — private tech built for government use, but at arm's length. O'Neill approaching them about the compromised hardware is structurally identical to a Roman general approaching a *publicanus* consortium about supplying his legions.

- **Corruption and independence:** The *publicani* were notorious for corruption (Cicero's Verrine Orations are essentially a prosecution of corrupt government contractors). But the *honest* publicani — and they existed — were essential to the Republic. The state couldn't function without them. Gilfoyle and Dinesh are the honest *publicani*: they won't attend the retreats, they won't take the money, they write honest code because the alternative disgusts them.

- **The refusal pattern:** Cato the Elder was famous for refusing bribes and luxury. When offered a lucrative position, he reportedly said: "What you offer is more than I need. What I have is less than I deserve. The distance between them is called integrity." Gilfoyle deleting investor emails is the same gesture — refusal as identity.

### Production Design for Gilfoyle/Dinesh Scene
- **Office:** Cluttered, productive, real. Servers visible. No motivational posters. No ping-pong table. The antithesis of VC-funded startup aesthetic. Think: a workshop, not an "office." Wires, hardware, stickers on laptops, food wrappers. The environment of people who work rather than perform.
- **Contrast with the "retreats":** We never see the retreats in 1x10 (save that for later). But the emails describing them should feel glossy, performative, buzzword-laden. The tonal contrast sells itself.

---

## PRODUCTION DESIGN — NOVA ROMA SCENES

### The Workshop/Tabularium

For the STML extension teaching scenes:

- **Writing surfaces:** The Tabularium should now show *layers* of notation. Wax tablets (1x07), parchment scrolls, and now the screen. The evolution of media is visible in the room.
- **Stemmata on walls:** By 1x10, Marcus or a Roman engineer should have painted a *stemma* (family tree) on a tabularium wall — the dependency tree from 1x09. This should be visible in the background, a physical version of the digital tree.
- **Pliny's *adversaria*:** Pliny should have his working notebooks visible — the *adversaria* (daily notes) that his nephew describes (Ep. 3.5). Small wax tablets covered in cramped writing. These ARE fragments — individual observations waiting to be organized.
- **The cursor and stylus:** Roman engineers working at terminals should have a *stilus* (stylus) nearby even when typing — the habit of annotation is ingrained. Some should be making margin notes on wax while looking at screen output.

### Props for Specific Scenes

- **Raw content demonstration:** Have Pliny's original encyclopedia text visible — with angle brackets and special characters that break the parser. The motivation is physical: he's trying to encode a passage about metallurgy that includes chemical notation with `<` and `>` symbols.
- **Capture operators:** A physical demonstration aid — Marcus could have wooden blocks on a table. Move one block next to others, then slide a larger block around them to "capture" them. The physical metaphor before the code.
- **Fragments:** Pliny's *excerpta* — physical bundles of notes tied with string. He holds up a bundle: "These are from volume seven. They have no order yet. They are a... group. A collection without a reason except that I chose to collect them." Fragment.
- **Transclusion:** A wax tablet with a pointer: "See Volume XII, Chapter III, Observation 7." The tablet itself contains no content — just a reference. The content lives elsewhere.

### Lighting and Time of Day
- **Morning for teaching scenes** — bright, active, productive.
- **Afternoon for complexity** — as the extensions get more sophisticated (capture operators, transclusion), the light should shift. Not dark, but warmer. The difficulty is increasing.
- **Evening for the Earth-side scene** — Gilfoyle/Dinesh in artificial light. The contrast with Nova Roma's natural light.

---

## LATIN VOCABULARY AUDIT — COMPLETE GLOSSARY

### Terms Already in the API (Approved)
| Term | Meaning | Usage |
|------|---------|-------|
| *crudus* | raw, unprocessed | Raw content tags |
| *captio* | a taking, capture | Capture operators |
| *farcimen* | stuffing, sausage | Sandwich capture |
| *fragmentum* | a broken piece | Fragment tags |
| *titulus* | label, inscription | Labels system |
| *transclusio* | across-enclosure | Transclusion |
| *duplicare* | to double, copy | Deep clone |
| *profunde* | deeply | Deep clone qualifier |
| *superficialiter* | shallowly | Shallow clone qualifier |

### Proposed New Terms for Dialogue
| Term | Meaning | Context |
|------|---------|---------|
| *verba certa* | fixed words | Raw content motivation (legal) |
| *ipsissima verba* | the very words | Raw content scholarly parallel |
| *adrogatio* | full adoption | Backward capture metaphor |
| *excerpta* | excerpts | Fragment parallel |
| *fasciculus* | small bundle | Physical fragment metaphor |
| *spatium nominum* | namespace | Latin coinage for `::` concept |
| *cento* | patchwork (poem) | Transclusion parallel |
| *exemplum* | authorized copy | Deep clone parallel |
| *instauratio* | re-performance | Deep clone ritual parallel |

### Terms to AVOID
| Term | Reason |
|------|--------|
| *nomen* | Already a `typedef` macro in the codebase — NEVER use as variable name |
| *copia* (for "copy") | Ambiguous — *copia* primarily means "abundance" in classical Latin. *Exemplum* or *duplicare* are better. |
| *inclusio* (for transclusion) | Too close to "inclusion" — transclusion is specifically *cross-referential*, not just "putting inside." |

---

## CHARACTER KNOWLEDGE MAP — WHO KNOWS WHAT

| Character | Raw Content | Capture | Fragments | Labels/NS | Transclusion | Deep Clone |
|-----------|------------|---------|-----------|-----------|--------------|------------|
| **Pliny** | *Verba certa* (legal), *ipsissima verba* (scholarly) | *Adrogatio* (family law) | *Excerpta* (his daily method!) | Multi-axis encyclopedia classification | His index (Vol. I) | *Exempla* from archives |
| **Marcus** | Engineering specs (exact) | Military incorporation | Construction material bundles | Census classification, *tria nomina* | Architectural cross-refs | Blueprint copies |
| **Daniel** | *Ipsissima verba*, inscription preservation | Translation as restructuring | Archaeological fragment bags | Linguistic classification systems | *Cento* poetry | Manuscript copying |
| **Lucia** | Performative vs. descriptive language | Legal framework of capture | Rhetorical *loci communes* | Aristotelian genus/species via `::` | Intertextuality | The copy's independence |
| **Casey** | Sacred formulae (prayers, liturgy) | — | — | — | Scripture cross-reference | — |
| **Carter** | — | — | — | — | — | Scientific replication |
| **Data** | Formal: "content that must not be interpreted" | "Restructuring the tree without rewriting the source" | "A container whose only property is containment" | "Orthogonal classification axes" | "A pointer to canonical content" | "An independent copy with shared origin" |
| **Methos** | *Evocatio* (brief flash) | *Evocatio* as theological capture | — | — | — | — |
| **Joe** | Verbatim Watcher records | — | Individual observation records | Subject/era/observer classification | Cross-reference files | Archive backup copies |
| **Midas** | Persian diplomatic archives | Persian administrative absorption | — | — | — | — |
| **Teal'c** | Jaffa military orders (exact) | — | — | — | — | — |

---

## HISTORICAL ACCURACY NOTES

### Things to Get Right
1. **Adrogatio required a vote of the *comitia curiata*** — it was a public act, not a private contract. This matters if Marcus or Pliny references it: capture operators are public restructuring, not secret.
2. **The fetial formulae** (for *evocatio* etc.) were among the most closely guarded sacred texts. If Methos references *evocatio*, he's referencing something most Romans wouldn't casually know. His knowledge dates him — he was there when these rituals were still performed.
3. **Pliny's working method** is documented by his nephew (Ep. 3.5): a reader, wax tablets, *excerpta* organized by topic, constant annotation. This is NOT fictional — the show can use it directly.
4. **The Digest of Justinian** is technically 6th century, but the jurisprudential practice of organizing *fragmenta* under *tituli* goes back to the late Republic. Pliny's era would know the concept even if the specific compilation is centuries later.
5. **Cento poetry** — Ausonius (4th century) is the most famous practitioner, but the form existed earlier. The practice of quotation-as-composition was understood in Pliny's era through rhetorical training.

### Things the Characters Might Get Wrong (Productively)
- A Roman engineer might compare fragments to *fasciculi* (bundles) — tied with string, no order. Daniel could gently correct: "A *fasciculus* implies physical proximity. A fragment implies logical grouping. You might fragment things that are stored in different places." The correction deepens understanding.
- Marcus might initially resist capture operators: "This is too much power. A node that can reorganize its siblings? That is a dictator, not a citizen." Lucia's response: "A dictator was a legal office with defined powers and duration. So is a capture operator." Beautiful Roman constitutional argument.

---

## OPEN QUESTIONS FOR THE SHOWRUNNER

1. **How much Methos in 1x10?** The *evocatio* parallel is very strong for capture operators but Methos had a major scene in 1x09's Tag. Recommend: ONE beat, 15-20 seconds. When someone explains capture operators, Methos says: "The Romans had a word for capturing another city's gods. *Evocatio*. You promised the god a better temple. Sometimes it worked." Then he leaves. The implication: he saw it done.

2. **Should Pliny's *excerpta* be physically present?** Recommend yes. He should have his working notebooks (*adversaria*) on his desk — bundles of wax note-tablets. When fragments are introduced, he holds up a bundle: "I have been doing this for sixty years. You have given it a name." The physical prop grounds the concept.

3. **How technical should the *tria nomina* / namespace explanation be?** Recommend: Marcus explains the *tria nomina* in 30 seconds, then Lucia extends it to genus/species. Total: 60 seconds. The audience doesn't need a Roman history lecture — they need to see that multi-axis classification is *natural* to Roman thinking.

4. **Gilfoyle/Dinesh scene length?** Per the season outline, this is their introduction. Recommend 90-120 seconds max. Enough to establish: (a) they exist, (b) they're independent, (c) the weird emails, (d) Gilfoyle's refusal. Don't over-explain. Let the audience wonder who these people are.

5. **Should we see a *cento* performed?** Daniel explaining transclusion through Ausonius is strong, but actually hearing a few lines of a *cento* — Virgil's words rearranged into new meaning — could be powerful. Pliny or Lucia could recite from memory. Recommend: 2-3 lines maximum. The concept lands faster through demonstration than explanation.
