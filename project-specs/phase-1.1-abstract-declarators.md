# Phase 1.1: Abstract Declarators

Date: 2026-01-14
Status: Plan

---

## Goal

Add support for:
1. **Pointer casts**: `(int*)x`, `(char**)ptr`, `(void*)p`
2. **sizeof(type)**: `sizeof(int)`, `sizeof(int*)`, `sizeof(struct foo)`

---

## Current State

### Existing sizeof (P143)
```c
/* P143 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' factor" }
```
- State 335: after `sizeof` - expects factor tokens
- State 336: after `sizeof factor` - reduce P143
- Only handles `sizeof expr`, not `sizeof(type)`

### Existing casts (P144-P147)
```c
/* P144 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_CONVERSIO, "factor -> '(' INT ')' factor" }
/* P145 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_CONVERSIO, "factor -> '(' CHAR ')' factor" }
/* P146 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_CONVERSIO, "factor -> '(' VOID ')' factor" }
/* P147 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_CONVERSIO, "factor -> '(' ID ')' factor" }
```

State flow:
- State 6 (after `(`) shifts INT/CHAR/VOID to states 337/338/339
- States 337/338/339 only accept `)` -> states 340/341/342
- States 340/341/342 accept factor tokens -> states 343/344/345 (reduce)

**Problem**: States 337/338/339 don't accept `*` for pointer types.

---

## Design

### Approach A: Extend Existing Pattern (Chosen)

Rather than creating a full abstract_declarator non-terminal, extend the existing cast states to handle pointers:

```
After '( type' states (337/338/339):
  - On ')' -> go to cast-expects-factor state (existing)
  - On '*' -> go to new "type with pointer" state

After '( type *' state:
  - On ')' -> go to pointer-cast-expects-factor state
  - On '*' -> stay/loop for more pointers
  - On 'const' -> go to "type * const" state (optional, phase 2)
```

This keeps the state machine simpler and avoids full abstract declarator complexity.

### New Productions

```c
/* Pointer casts */
/* P154 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_CONVERSIO, "factor -> '(' INT '*' ')' factor" }
/* P155 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_CONVERSIO, "factor -> '(' CHAR '*' ')' factor" }
/* P156 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_CONVERSIO, "factor -> '(' VOID '*' ')' factor" }
/* P157 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_CONVERSIO, "factor -> '(' ID '*' ')' factor" }

/* Double pointer casts */
/* P158 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_CONVERSIO, "factor -> '(' INT '*' '*' ')' factor" }
/* P159 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_CONVERSIO, "factor -> '(' CHAR '*' '*' ')' factor" }
/* P160 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_CONVERSIO, "factor -> '(' VOID '*' '*' ')' factor" }
/* P161 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_CONVERSIO, "factor -> '(' ID '*' '*' ')' factor" }

/* sizeof(type) */
/* P162 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' INT ')'" }
/* P163 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' CHAR ')'" }
/* P164 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' VOID ')'" }
/* P165 */ { ARBOR2_NT_FACTOR, 4, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' ID ')'" }

/* sizeof(pointer type) */
/* P166 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' INT '*' ')'" }
/* P167 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' CHAR '*' ')'" }
/* P168 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' VOID '*' ')'" }
/* P169 */ { ARBOR2_NT_FACTOR, 5, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' ID '*' ')'" }

/* sizeof(double pointer) */
/* P170 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' INT '*' '*' ')'" }
/* P171 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' CHAR '*' '*' ')'" }
/* P172 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' VOID '*' '*' ')'" }
/* P173 */ { ARBOR2_NT_FACTOR, 6, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' '(' ID '*' '*' ')'" }
```

Total: 20 new productions (P154-P173)

---

## New States

### Pointer Cast States

#### State 364: after `( int *` - expects `)` or `*`
```c
hic_manens constans Arbor2TabulaActio STATUS_364_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 368, FALSUM },  /* -> (int*) expects factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 372, FALSUM }   /* -> (int**) */
};
```

#### State 365: after `( char *` - expects `)` or `*`
```c
hic_manens constans Arbor2TabulaActio STATUS_365_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 369, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 373, FALSUM }
};
```

#### State 366: after `( void *` - expects `)` or `*`
```c
hic_manens constans Arbor2TabulaActio STATUS_366_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 370, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 374, FALSUM }
};
```

#### State 367: after `( ID *` - expects `)` or `*`
```c
hic_manens constans Arbor2TabulaActio STATUS_367_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 371, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 375, FALSUM }
};
```

#### States 368-371: after `( type * )` - expects factor
These are like states 340-342 but for pointer casts.

#### States 372-375: after `( type * *` - expects `)` or `*`

#### States 376-379: after `( type * * )` - expects factor

#### States 380-383: after `( type * ) factor` - reduce P154-P157

#### States 384-387: after `( type * * ) factor` - reduce P158-P161

### sizeof(type) States

#### State 388: after `sizeof (` - expects type
```c
hic_manens constans Arbor2TabulaActio STATUS_388_ACTIONES[] = {
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 389, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 390, FALSUM },
    { ARBOR2_LEXEMA_VOID,           ARBOR2_ACTIO_SHIFT, 391, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 392, FALSUM },
    /* Also allow expression for sizeof(expr) ambiguity - GLR fork */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, VERUM },  /* conflict - fork */
    /* ... other expr-start tokens ... */
};
```

#### States 389-392: after `sizeof ( type` - expects `)` or `*`
#### States 393-396: after `sizeof ( type )` - reduce P162-P165
#### States 397-400: after `sizeof ( type *` - expects `)` or `*`
#### States 401-404: after `sizeof ( type * )` - reduce P166-P169
#### States 405-408: after `sizeof ( type * *` - expects `)`
#### States 409-412: after `sizeof ( type * * )` - reduce P170-P173

---

## State Modifications

### State 6 (after `(`)
No change needed - already shifts INT/CHAR/VOID to 337/338/339.

### States 337-339 (after `( type`)
Add `*` shift action:
```c
/* State 337: after '( int' - expects ')' or '*' */
hic_manens constans Arbor2TabulaActio STATUS_337_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 340, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 364, FALSUM }  /* NEW */
};
```

### State 335 (after `sizeof`)
Add `(` shift to new sizeof-paren state:
```c
/* State 335: after 'sizeof' - expects factor or '(' */
hic_manens constans Arbor2TabulaActio STATUS_335_ACTIONES[] = {
    /* existing factor-start tokens... */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 388, VERUM },  /* fork: sizeof( could be type or expr */
};
```

---

## Summary of New States

| State Range | Description | Count |
|-------------|-------------|-------|
| 364-367 | after `( type *` | 4 |
| 368-371 | after `( type * )` expects factor | 4 |
| 372-375 | after `( type * *` | 4 |
| 376-379 | after `( type * * )` expects factor | 4 |
| 380-383 | after `( type * ) factor` reduce | 4 |
| 384-387 | after `( type * * ) factor` reduce | 4 |
| 388 | after `sizeof (` | 1 |
| 389-392 | after `sizeof ( type` | 4 |
| 393-396 | after `sizeof ( type )` reduce | 4 |
| 397-400 | after `sizeof ( type *` | 4 |
| 401-404 | after `sizeof ( type * )` reduce | 4 |
| 405-408 | after `sizeof ( type * *` | 4 |
| 409-412 | after `sizeof ( type * * )` reduce | 4 |

**Total new states: ~49**

---

## AST Representation

### Existing CONVERSIO node
```c
structura {
    Arbor2Nodus*        typus;      /* Type node (IDENTIFICATOR) */
    Arbor2Nodus*        expressio;  /* Expression being cast */
} conversio;
```

For pointer casts, we need to represent the pointer depth. Options:

**Option A: Extend typus node** (simpler)
Store pointer depth in the type node's declarator field:
```c
/* In AST handler, create a DECLARATOR node with num_stellae set */
nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
nodus_typus->datum.declarator.num_stellae = pointer_depth;
nodus_typus->datum.declarator.titulus = type_name;  /* "int", "char", etc. */
```

**Option B: Create new NOMEN_TYPI node type**
Add ARBOR2_NODUS_NOMEN_TYPI with:
```c
structura {
    Arbor2LexemaGenus   base_type;  /* INT, CHAR, VOID, or IDENTIFICATOR */
    chorda*             type_name;  /* For typedef names */
    s32                 num_stellae;
} nomen_typi;
```

**Recommendation: Option A** - reuse existing DECLARATOR, less code change.

### sizeof node
Existing sizeof_expr already supports both forms:
```c
structura {
    b32                 est_typus;      /* VERUM for sizeof(type) */
    Arbor2Nodus*        operandum;      /* Type or expression */
} sizeof_expr;
```

For `sizeof(type)`:
- `est_typus = VERUM`
- `operandum` = DECLARATOR node with type info and num_stellae

---

## Implementation Steps

### Step 1: Add Productions (lib/arbor2_glr_tabula.c)
Add P154-P173 to REGULAE array.

### Step 2: Add State Arrays
Create STATUS_364_ACTIONES through STATUS_412_ACTIONES.

### Step 3: Modify Existing States
- State 337: add `*` shift to 364
- State 338: add `*` shift to 365
- State 339: add `*` shift to 366
- State 335: add `(` shift to 388 (with VERUM conflict flag for GLR fork)

### Step 4: Add STATUS_INFO Entries
Add descriptions for all new states.

### Step 5: Add GOTO Entries
Add GOTO table entries for new states that need them.

### Step 6: Update AST Handler (lib/arbor2_glr.c)
Handle P154-P173 in _exequi_reduce:
- For casts: create DECLARATOR node with num_stellae, use as typus in CONVERSIO
- For sizeof: create DECLARATOR node, set est_typus = VERUM

### Step 7: Update glr_debug.c
Add print cases for new node patterns.

### Step 8: Add Tests
Test cases:
```c
/* Pointer casts */
"(int*)x"
"(char*)ptr"
"(void*)p"
"(int**)pp"
"(char**)argv"

/* sizeof(type) */
"sizeof(int)"
"sizeof(char)"
"sizeof(void*)"
"sizeof(int*)"
"sizeof(int**)"

/* Combined */
"(int*)sizeof(int)"
"sizeof((int*)x)"
```

---

## GLR Ambiguity Handling

The key ambiguity is `sizeof(x)` where `x` could be:
1. A type name (typedef) -> sizeof(type)
2. A variable -> sizeof(expr)

With GLR:
- State 388 (after `sizeof (`) has conflict flag on IDENTIFICATOR
- Parser forks both interpretations
- One fork dies when `)` doesn't match expected syntax
- If both survive (x is both typedef and variable), create ambiguity node

For now, we'll handle this by:
1. Type keywords (int, char, void) always go sizeof(type) path
2. Identifiers fork both paths
3. Use existing arbor2_glr_est_probabiliter_typus() to guide merging

---

## Files to Modify

1. **lib/arbor2_glr_tabula.c**
   - Add P154-P173 to REGULAE
   - Add ~49 new state arrays
   - Modify states 335, 337, 338, 339
   - Add STATUS_INFO entries
   - Add GOTO entries

2. **lib/arbor2_glr.c**
   - Add AST handlers for P154-P173

3. **tools/glr_debug.c**
   - Update print handlers if needed

4. **probationes/probatio_arbor2_glr.c**
   - Add test cases

---

## Estimated Effort

- Productions: 20 new
- States: ~49 new
- State modifications: 4
- LOC estimate: ~1500 new lines in tabula.c, ~100 in glr.c

---

## Future Extensions (Not This Phase)

- Triple+ pointers: `(int***)`
- Const pointers: `(int * const)`
- Function pointers: `(int (*)(void))`
- Array types: `sizeof(int[10])`
- Struct/union in casts: `(struct foo*)`

These require full abstract declarator grammar, which is more complex.
