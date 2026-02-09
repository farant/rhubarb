# lapifex_c89_grammatica.c Worklog

## 2026-02-09 — Initial Implementation (M2.3)

### Overview
C89 expression grammar for the Lapifex GLR parser. Contains:
- STML grammar string with 94 productions covering all C89 expression precedence levels
- Reduction callback (switch on production index) that builds Arbor2Nodus* AST nodes
- `lapifex_c89_expressionem_parsare()` pipeline: lex -> adapt -> GLR parse -> AST
- Static table caching for performance

### Grammar Structure
Productions P0-P93 (+ augmented P94 added automatically):
- P0-P1: comma (virga)
- P2-P13: assignment operators (=, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=)
- P14-P15: ternary (?:)
- P16-P43: binary operators by precedence (||, &&, |, ^, &, ==, !=, <, >, <=, >=, <<, >>, +, -, *, /, %)
- P44-P55: unary/cast/sizeof
- P56-P63: postfix (call, subscript, member, increment)
- P64-P69: primary (identifiers, literals, parenthesized)
- P70-P71: function call arguments
- P72-P93: type names for casts and sizeof (keyword types only, no typedef ambiguity)

### Key Design Decisions
1. **Minimal type_name**: Only keyword types (void, char, int, etc.) and struct/union/enum tags. No IDENTIFICATOR in type_name to avoid cast ambiguity with parenthesized expressions. Typedef resolution deferred to M2.6.

2. **Table caching**: Grammar/LALR table built once into static variables on first call. Subsequent parses skip the expensive ~2s table construction.

3. **Uses LALR(1)** instead of canonical LR(1) due to state explosion with the full grammar. No LALR conflicts found — the expression grammar is unambiguous with the precedence hierarchy encoded directly in the grammar.

### Terminal Names
All terminal names in the STML must match NOMINA_GENERUM[] in arbor2_lexema.c exactly. Unused C89 keywords (AUTO, BREAK, CASE, etc.) are omitted from the grammar since they don't appear in expressions.

### Production Numbering
Productions are numbered 0-based in the order they appear in the STML `<regulae>`. The augmented production is added as P94 by `lapifex_grammaticam_legere`. The reduction callback uses a switch on production_index with roman numeral case labels (ZEPHYRUM through XCIII).
