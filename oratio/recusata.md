# oratio — recusata: the refusal registry

*Born 2026-09-09 (T31 d), after reading knotapel's wall registry. A
refusal is a fact about the configuration it was measured against, not
an absolute. Every entry names the pipeline COMPONENT the wall sat in,
so that when that component changes the refusal is re-measured rather
than remembered. The components, in the order DKC found them to matter:*

- **annotatio** — what readings carry and which slots exist. If a law
  cannot be expressed because the feature is not on the reading, no
  rule or policy will reach it (the encoding wall).
- **exsecutor** — how orders are read out: first-wins, pair policy,
  vindication, refusals, the passes. The readout wall: the reading is
  right and the owner is wrong.
- **regulae** — the rule file: which patterns exist and in what order.
- **substratum** — the dictionary and the gold: the reading is not in
  the list, or the census says the law is false.
- **oraculum** — the measurement itself (conventions, pins).

Status: *manet* (stands), *retractandum* (re-measure when the named
component changes), *clausum* (closed by a later adoption).

| id | tranche | refused | measured | component | status / retry when |
|---|---|---|---|---|---|
| R01 | T19j | tail laws: strong heads, bind-only, twin, loose noun-first | each depresses a treebank; a 70 % distant binding beats crude order | exsecutor | manet |
| R02 | T19h/l | demotion of unpromoted readings, four forms | primary down every time; the alternative readings are junk | substratum | manet — do not retry |
| R03 | T20c | loose head rules inside the clause box, seven variants | Seneca primary down; a seed-between control without the box equals it | exsecutor | manet — for boundary laws the box is the seeds |
| R04 | T20c | clause laws 1, 2 (`cum`), 4 (subject) | refuted by census | substratum | manet |
| R05 | T24 | the case prior before the evidence rules | wrecked primary | exsecutor (order) | manet — a prior may not change what evidence rules see |
| R06 | T27 | unified loose tier; first reading per case | attachment down under every ordering | exsecutor (retry + order) | retractandum when claims are ranked by trust rather than rule order |
| R07 | T30 c | known-form variants unfiltered by dialect | primary down everywhere: over-generated readings win spurious agreement | annotatio | clausum by the dialect gate on the annotator |
| R08 | T30 e | ablative-first loose order; gen-acc-nom | case up but gender and attachment down everywhere | regulae (order) | manet |
| R09 | T31 a | nouns-only subject candidates | worse everywhere | regulae | manet |
| R10 | T31 a | LEX CAPITIS on any partner / distant heads | charters and Dante down (relative pronouns are subjects) | exsecutor | retractandum with a determiner-aware head law |
| R11 | T31 b | loose object tier | attachment −49/−50 charters, −14..−24 shelf: distant accusatives are somebody else's | exsecutor (no contest between claims) | retractandum after one head per word with best-wins |
| R12 | T31 b | partner refusals: class change, adposition member, own subject | −1..−4, and the subject one inert (slots fill in rule order) | exsecutor (order of filling) | retractandum after certainty-ordered filling |
| R13 | T31 b | excluding unknown-kind (X) verbs from the object slot | recall to a sixth | substratum (WORDS kind) | manet |
| R14 | T31 c | object slot on participles | case +4..+13 but attachment −5/−3/−2: right case, wrong owner (the main verb across the participle) | exsecutor (no contest between claims) | retractandum after one head per word with best-wins |
| R15 | T31 d | naive second resolution pass | inert classical, charters attachment −5/−6: new arcs 2–19 % right, from greedy rules on reordered readings | annotatio (decisions are not features) + exsecutor (greedy capture) | retractandum as a strict-decided tier with a trust table |
| R16 | T31 d | prior only in the last pass | byte-identical to R15 | — | inert |
