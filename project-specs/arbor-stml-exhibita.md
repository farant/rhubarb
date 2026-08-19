# Arbor ⇄ STML — Exhibita (simulatio parva, pre-interview)

2026-08-19. Stage 1 of the arbor-stml run-up (decreed: exhibits-first,
pattern-substrate as M1's scope driver). These are HAND-WRITTEN candidate
serializations of real subtrees, composed against the actual seam headers
(silva_nodus.h, silva_token.h, silva_tabulae.h, silva_scribere.h,
include/stml.h) and the actual baked registry (silva_tabulae_c89.c) —
not tool output, not compiled, not round-tripped. Their job is to make
the interview concrete: react to documents, not descriptions.
Sibling docs: arbor-stml-visio.md (the project), mutatio-visio.md §3
(the pattern consumer), stml-visio.md §2 (sigil law).

## 0. The vocabulary, measured

The registry (SILVA_C89_REGISTRUM) already IS the tag vocabulary:

- **58 genera**, kebab-case Latin: `definitio-functionis`, `declaratio`,
  `vocatio`, `binarium`, `folium-identificator`, `ambiguus`,
  `conditionalis`, `ramus-sumptus` …
- **176 loci**, named per genus, with a live convention: token loci are
  `tok_`-prefixed (`tok_apertum`, `tok_terminator`, `tok_verba`), node
  and list loci are bare (`specificatores`, `declaratores`, `conditio`,
  `argumenta`, `interpretationes`). Species per locus: NODUS / TOKEN /
  LISTA_NODUS / LISTA_TOKEN / LISTA_MIXTA / INDEX (the one INDEX in the
  c89 grammar: `ambiguus.canonica`; also `ramus-*.conditio_id`).
- **~96 lexeme genera** (NOMINA_GENERUM), uppercase English-ish:
  `INT`, `IDENTIFICATOR`, `PAREN_APERTA`, `SPATIA`, `COMMENTUM_CLAUSUM`.

**Finding 0 (the latina joke pays off):** node genera are Latin,
C-keyword lexemes are English — `si` the node never collides with
`IF`→`if` the lexeme; `structura` never collides with `struct`. The two
name spaces are disjoint BY LANGUAGE, so lexemes can be tags too, with
one mangling rule total: lowercase + `_`→`-`.

## I. Exhibitum primum — `int n = 0;` (purus, stratum 0)

Tree per registry: declaratio { specificatores:[typus-primitivus],
declaratores:[declarator-initiatus{declarator-titulus, `=`,
folium-integer}], tok_terminator:`;` }. Trivia measured convention
(silva_lexema.c:951): same-line trailing spaces → prior's spatia_post;
post-newline leading indentation → next token's spatia_ante.

### I.a Candidate: inline, positions carried

```stml
<arbor grammatica="c89" fons="minimum.c">
  <declaratio>
    <specificatores>
      <typus-primitivus>
        <tok_verba>
          <int b="0" linea="1" columna="1"><post><spatia n="1"/></post></int>
        </tok_verba>
      </typus-primitivus>
    </specificatores>
    <declaratores>
      <declarator-initiatus>
        <declarator>
          <declarator-titulus>
            <tok_titulus>
              <identificator b="4" linea="1" columna="5">n<post><spatia n="1"/></post></identificator>
            </tok_titulus>
          </declarator-titulus>
        </declarator>
        <tok_operator>
          <assignatio b="6" linea="1" columna="7"><post><spatia n="1"/></post></assignatio>
        </tok_operator>
        <initiator>
          <folium-integer>
            <tok_valor>
              <integer b="8" linea="1" columna="9">0</integer>
            </tok_valor>
          </folium-integer>
        </initiator>
      </declarator-initiatus>
    </declaratores>
    <tok_terminator>
      <semicolon b="9" linea="1" columna="10"><post><nova-linea/></post></semicolon>
    </tok_terminator>
  </declaratio>
</arbor>
```

Rules this candidate embodies:

- **Genus = element tag** (node and lexeme alike). **Locus = wrapper
  element**; the two alternate by depth, so `<vocatio><functio>` is
  locus-addressed exactly the way mutatio-visio §3 wants.
- **NIHIL locus = omitted element** (canonical absence). The registry
  (→ generated canon) makes reading unambiguous anyway.
- **Token valor = text content**, and ONLY for variable-spelling genera
  (identificator, litterae, comments, octetus-ignotus). Fixed-spelling
  genera (`int`, `=`, `;`) carry no text — spelling is the genus.
  Valor can NEVER be an attribute: `"` is unrepresentable in STML
  attribute values (measured, stml.h), and string literals contain it.
- **Trivia = `<ante>`/`<post>` wrappers** with compact lens forms:
  `<spatia n="4"/>`, `<tabulae n="1"/>`, `<nova-linea/>` (+`crlf`
  boolean when \r\n), `<continuatio/>`; comments as entity-escaped
  text (`<commentum-clausum>/* … */</commentum-clausum>`). The
  compact forms lose nothing — valor is fully determined by count.
  Comments cannot use raw `!` framing safely (a comment can contain
  the closing sequence); symmetric entity text is the safe default.

### I.b Variant: positions derived (nuda)

Same document with NO `b`/`linea`/`columna` anywhere, plus one anchor
on the envelope (`b-initium="0"` + starting linea/columna). For
stratum-0 tokens the positions are pure cumulative sums over the
emission walk — a load-time fixup recomputes them, exactly the class
pater and initium_lineae already live in (post-accept fixups).
The carried variant can LIE (positions disagreeing with content);
the derived variant cannot. But def-site tokens (exhibit II) have
positions in ANOTHER file, underivable from this document — those
must carry coordinates regardless.

### I.c Variant: merged token-loci

TOKEN-species loci collapse wrapper+token into one element:

```stml
<tok_terminator genus="semicolon"><post><nova-linea/></post></tok_terminator>
```

Halves the depth of token-heavy trees; costs the uniform
genus/locus alternation (a locus element sometimes IS the value).

## II. Exhibitum secundum — `    redde ZEPHYRUM;` (expansio, M2 preview)

Latinized line: `redde`→`return` and `ZEPHYRUM`→`0` are 1:1 object
macros (geometria fida). The TREE holds the EXPANDED tokens (lexical
fields from def-site); the invocation-site tokens exist only through
origo. The leading 4 spaces ride the invocation token (`invocatio
trivia fert` — silva_nodus.h). So an origo-bearing document needs a
home for tokens that are not in the tree: a pool.

```stml
<arbor grammatica="c89">
  <fontes>
    <fons index="0" via="lib/exemplum.c"/>
    <fons index="3" via="latina.h" lexicon/>
  </fontes>

  <redde>
    <tok_verbum>
      <return origo="#o1"/>
    </tok_verbum>
    <valor>
      <folium-integer>
        <tok_valor>
          <integer origo="#o2">0</integer>
        </tok_valor>
      </folium-integer>
    </valor>
    <tok_terminator>
      <semicolon b="1019" linea="42" columna="20"><post><nova-linea/></post></semicolon>
    </tok_terminator>
  </redde>

  <origines>
    <#o1><expansio macro="redde" corpus="#t-lat-1" invocatio="#t-inv-1"/></#o1>
    <#o2><expansio macro="ZEPHYRUM" corpus="#t-lat-2" invocatio="#t-inv-2"/></#o2>
    <#t-inv-1><identificator f="0" b="1004" linea="42" columna="5"><ante><spatia n="4"/></ante>redde</identificator></#t-inv-1>
    <#t-inv-2><identificator f="0" b="1010" linea="42" columna="11">ZEPHYRUM</identificator></#t-inv-2>
    <#t-lat-1><return f="3" b="612" linea="31" columna="21"/></#t-lat-1>
    <#t-lat-2><integer f="3" b="1101" linea="52" columna="22">0</integer></#t-lat-2>
  </origines>
</arbor>
```

- References are **attribute-valued `#id`** (document-space per
  decretum 01KZPEXT74), not `<<#id>>` transclusion — origo is an
  identity reference, not content-in-place. Transclusion stays
  reserved for compressed VIEWS (stml-visio §6 pipe-views).
- Origo genus as element name (`<expansio>`, `<pasta>`, `<chorda>`,
  `<api>`); FONS carries nothing = no origo attribute at all.
- Caecatio (hideset) not shown — M2 audit item (immutable shared-tail
  lists: pool-shaped too, `cauda="#c7"`).
- M1 posture on hitting a non-FONS token: **fractura clara** (same
  boundary silva_scribere draws), since the M1 corpus (roundtrip
  fixtures) is plain C. The format holds the door open via `origo=`.

## III. Exhibitum tertium — `i8 * t;` sine lexico (AMBIGUUS)

Two interpretationes SHARING the same four token objects (GLR arms
share; identity is the point — duplication would lie, §3c). Locus
`canonica` is the INDEX species.

```stml
<ambiguus>
  <interpretationes>
    <declaratio>
      <specificatores>
        <typus-nominatus>
          <tok_titulus>
            <#lex1><identificator b="0" linea="1" columna="1">i8<post><spatia n="1"/></post></identificator></#lex1>
          </tok_titulus>
        </typus-nominatus>
      </specificatores>
      <declaratores>
        <declarator-monstrator>
          <tok_stella><#lex2><star b="3" linea="1" columna="4"><post><spatia n="1"/></post></star></#lex2></tok_stella>
          <internum>
            <declarator-titulus>
              <tok_titulus><#lex3><identificator b="5" linea="1" columna="6">t</identificator></#lex3></tok_titulus>
            </declarator-titulus>
          </internum>
        </declarator-monstrator>
      </declaratores>
      <tok_terminator><#lex4><semicolon b="6" linea="1" columna="7"><post><nova-linea/></post></semicolon></#lex4></tok_terminator>
    </declaratio>
    <sententia-expressionis>
      <expressio>
        <binarium>
          <sinister><folium-identificator><tok_valor><<#lex1>></tok_valor></folium-identificator></sinister>
          <tok_operator><<#lex2>></tok_operator>
          <dexter><folium-identificator><tok_valor><<#lex3>></tok_valor></folium-identificator></dexter>
        </binarium>
      </expressio>
      <tok_terminator><<#lex4>></tok_terminator>
    </sententia-expressionis>
  </interpretationes>
  <canonica>0</canonica>
</ambiguus>
```

First use defines a named fragment (`<#lex1>…</#lex1>`), second use
transcludes (`<<#lex1>>`) — token identity survives serialization.
This is the first real pull on transclusion resolution in C, exactly
as stml-visio §6 anticipated. NOTE the asymmetry with exhibit II:
here shared CONTENT renders in place (transclusion is right); there
origo was an identity POINTER (attribute ref is right). One law:
**content shared → fragment+transclusion; identity referenced →
`#id` attribute.**

Alternative shown for contrast — token-table style: every token
serialized once in a flat `<lexemata>` pool, the whole tree
references by id. Sharing becomes trivial (both arms just point),
but every leaf of every UNAMBIGUOUS tree becomes an indirection
too. The more sharing a document has, the more the table wins;
M2's full parsura (parsura->lexemata IS a flat stream) is
table-shaped natively. The M1 choice is therefore really: one
format that starts table-shaped (no M1→M2 break, worse to read),
or inline-with-fragments for subtree documents (reads like the
code, pattern-friendly) with the full-parsura document as a second,
sibling form.

## IV. Inventiones (what the exhibits taught)

1. **The registry suffices.** No new vocabulary is needed for M1 —
   genus tags, locus wrappers, lexeme tags, all registry/name-table
   given. The generated canon (natura→canones lineage) can be
   projected from the same registry: locus wrapper order, species →
   content model, tok_ → token children. Canon judgment of tree
   documents is nearly free.
2. **The derivable-field inventory is the audit, started early:**
   pater (fixup), initium_lineae (fixup), longitudo (valor +
   scissurae), stratum-0 positions (emission walk from an anchor),
   prospectus/repositorium sharing (serialize the VIEW; the
   repositorium is engine-internal). Must-carry: genus, valor
   (variable-spelling only), trivia, scissurae, standard (when not
   C89), fons (when not the document default), def-site positions,
   origo (M2), caecatio (M2).
3. **Sharing appears at exactly three scales** (§3c called it):
   trivia (single-owner — plain nesting, no mechanism), AMBIGUUS
   arms (token identity — fragments/transclusion), origo chains
   (cross-stratum, cross-file — pool + attribute refs). Each scale
   got a different, fitting mechanism in the exhibits.
4. **Tokens force mixed content** (valor text + trivia children in
   one element) unless valor or trivia moves into its own wrapper.
   STML handles mixed content verbatim (stml_textus_internus), but
   the canonical form should decide deliberately.
5. **A canonical document should not be able to lie.** Carried
   stratum-0 positions are redundant with content; redundancy that
   can disagree is a liar's charter (the differre lesson: pairing
   keys must equal classification tiers — same law, new clothes).
   Deriving them makes the round-trip gate compare RECOMPUTED
   fields, which is precisely the representational audit running
   mechanically.
6. **M1 refuses loud at the expansion boundary** (non-FONS token =
   fractura clara), mirroring silva_scribere — the format still
   reserves `origo=` so M2 is an extension, not a break.

## V. Quaestiones colloquio (the interview agenda)

1. **Table vs inline** (the fork with consequences): token-table from
   day one (M1==M2 format, one document kind) or inline subtree
   documents + table-shaped full-parsura as two forms of one dialect?
2. **Positions: derive or carry** for stratum-0 tokens? (I.a vs I.b —
   gate implications above; def-site tokens carry either way.)
3. **Locus wrappers: uniform or merged** for TOKEN loci (I.a vs I.c)?
4. **Lexeme genus as tag** (`<identificator>`) vs uniform `<lexema
   genus="identificator">`?
5. **Reference law** as proposed (content→transclusion,
   identity→attribute `#id`) — accept, or one mechanism only?
6. **Naming**: envelope `<arbor>` for subtree docs, `<parsura>` for
   full documents? What is the dialect called (canon file name,
   `silex iudicare` registration)? Registry-version attribute on the
   envelope (`registrum="c89"` + a sigillum?) so judgment can refuse
   version skew?
7. **Trivia lens forms** (`<spatia n="4"/>`) in the canonical form,
   or verbatim text with lenses left to projections?
8. **INDEX loci**: text content (`<canonica>0</canonica>`) or
   attribute? And should `canonica` reference the arm by fragment id
   instead of by position?
9. **Mixed content** (finding 4): keep, or wrap valor in `<valor>`?
10. **Which real corpus** anchors M1's gate: roundtrip fixtures only,
    or also pure-FONS subtrees mined from lib/ (est_fons_purus as the
    miner)?
