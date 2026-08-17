# differre.c — worklog

## 2026-08-17 — natus (differentia semantica unitatum)

Third granularity of the differentia core in one day: the SAME
Myers-over-interned-identity that diffs lines diffs UNITS — lex
both versions, partition bytes by unit, sigillum each span, intern
the hashes, differentia_seriei over units. Silex diffs lines;
differre diffs the units you think in.

**MOTA semantics finding (measured at birth):** LCS anchoring
means "moved" is RELATIVE. A function that changed position while
its neighbors also changed stays IDEM in the LCS (it IS the
anchor); only units that fall out of the LCS with an equal-hash
partner report MOTA. Measured both ways: clean swap among
unchanged neighbors → "MOTA (contentum idem, sede nova)", zero
line churn (a line diff would show ±N noise); position change
amid changed neighbors → anchored, neighbors report. This is
correct, not a limitation — who moved is genuinely relative.

**Partition rule:** unit k's byte span = [prev end, own start),
own start = first token's leading trivia (attachment rules decide
comment ownership); unit 0 absorbs the file header, last unit
absorbs the tail. Every byte belongs to exactly one unit — the
differentia partition invariant philosophy at unit scale.

**Classification:** substantiva (code token sequences differ) /
documentaria (code identical, comment trivia differ) / cosmetica
(tokens and comments identical — whitespace only). Decidable
because trivia are first-class tokens.

**Prototype and definition are separate units** (top-level
semicolon vs brace rule) — a signature change shows twice, once
per unit. Honest; a symbol-level grouping pass is a named door.

**Known edges (at birth, per debrief practice):**
- Duplicate-content units: first-unmatched-wins in MOTA/MUTATA
  maps; a second identical pair may mis-pair. Rare, noted.
- Directive units are line-naive: #if/#endif are separate untitled
  units; conditional-ARM-aware diff (silva retains untaken arms —
  nothing else on earth diffs those) is the big named door.
- File-header comment edits attribute to the first unit (MUTATA
  documentaria on it) — honest but slightly odd.
- Titulus matching ignores genus: a typedef replacing a function
  of the same name reports MUTATA. Acceptable; genus is in the
  output either way.
- The -machina TSV is the composition surface: changed-symbol
  names pipe into nexus.sh / legati vocantes for blast radius —
  deliberately NOT built in.

Factoring: unit scanner extracted from the amalgamator into
instrumenta/silva_unitates.{h,c} (PORTA: amalgam byte-identical,
suite 39/39). silva_unitates_scandere = the enumeration wrapper;
directive titles for define/undef/ifdef/ifndef/include.
