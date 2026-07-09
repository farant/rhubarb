# Vocabularium Medullae — the sealed IR vocabulary

Date: 2026-07-09 (DECISUS, Fran — sim-3 + interview; see
project-specs/officina-m1-interview.md)
Standing: permanent reference, genera-c89.md precedent. The dump/parse
implementation cites THIS document. Op names, suffixes, and sigils are
CEREMONY-SEALED; §VII structural syntax (datum/attribute grammar) is
concrete-proposed and refinable during M1a Chunk C without ceremony
(structure, not names).

## I. Suffixes (typi operationum)

House convention, inherited from the monorepo typedefs:

    .i8 .i16 .i32 .i64    insignatus (unsigned)
    .s8 .s16 .s32 .s64    signatus   (signed)
    .f32 .f64             fluitans   (IEEE-754)

Registers hold 64-bit words; types live on OPS, not registers. Pointers are
.i64 values at the IR level. Conversions carry TWO suffixes: `.FROM.TO`.

## II. Sigils

    %titulus     registrum virtuale (register-resident locals keep their
                 source names; temporaries are %t1, %t2, ...)
    $titulus     symbolum (functio aut datum)
    @titulus     titulus blocci (block label)
    (nudum)      immediatum (bare literal operand)
    ;            commentarium (to end of line)

Internal-linkage names are source-derived: file statics `$<file-stem>.<name>`
(`$chorda_c.adiutor`), function-scope statics `$<functio>.<name>`
(`$legere_proximum.cursor`). Deterministic from source; stable across
program compositions.

## III. Operationes

**Motus**
    movere.T %d, src              copy register / load immediate

**Arithmetica** (all suffixes; residuum integer-only)
    addere.T  subtrahere.T  multiplicare.T  dividere.T  residuum.T  negare.T

**Bitalia** (integer suffixes only; classical logic set)
    et.T          & (and)
    vel.T         | (INCLUSIVE or)
    aut.T         ^ (EXCLUSIVE or — vel/aut is the classical distinction)
    complementum.T  ~
    sinistrorsum.T  <<
    dextrorsum.T    >>  (.s = arithmeticum, .i = logicum)

**Comparationes** (result: i32, 0 aut 1; signedness via suffix)
    aequalis.T  inaequalis.T  minor.T  minor_aequalis.T  maior.T  maior_aequalis.T

**Conversiones** (two suffixes: .FROM.TO; sign-/zero-extension decided by
SOURCE signedness — the M0b {naturalis, conversus} pair hands demissio
exactly this pair)
    extendere.F.T     integer widen        extendere.s32.s64 (sign)
                                           extendere.i32.i64 (zero)
    amputare.F.T      integer narrow (truncate)
    fluitare.F.T      integer -> fluitans
    integrare.F.T     fluitans -> integer
    dilatare.f32.f64  / artare.f64.f32     float width crossings

**Memoria**
    legere.T %d, %addr            typed load
    scribere.T %addr, src         typed store
    arca %d, magnitudo, ordinatio frame-slot allocation; returns the slot's
                                  address (QBE alloc lineage; slots for
                                  address-taken locals + aggregates; optional
                                  titulus attribute feeds indicium at M3)
    locus %d, $symbolum           address of datum/functio
    copia %dest, %src, magnitudo  aggregate copy (bytes)

**Fluxus**
    salire @b                                 unconditional jump
    ramus %c -> @verum | @falsum              branch: nonzero -> first target
    redde [src]                               return (operand absent = vacuum)
    vocare                                    call; forms in §V
    sistere "causa"                           TRAP: carries diagnostic +
                                              source position; halts the VM
                                              only IF EXECUTED. Emitted by
                                              demissio for unlowerable
                                              constructs (POSIX-incomplete
                                              layouts); the future
                                              enforcement layer's runtime
                                              seat.

## IV. Functio

    functio $titulus (T %param, T %param, ...) -> T
    functio $titulus (T %param, ...) -> T        ; variadica (callee is
                                                 ; systema-declared; ZERO
                                                 ; variadic bodies in corpus)
    -> vacuum                                    ; explicit void return type

Aggregate-by-value RETURN is explicit (sim C7): the destination address is a
visible first parameter in the medulla signature — officina's VM convention;
native ABIs map it at M5. Blocks follow; every block ends in exactly one
terminator (salire / ramus / redde / sistere).

## V. Vocare

    %d = vocare.T $f, args...     direct, valued
    vocare $f, args...            direct, vacuum
    %d = vocare.T %fp, args...    indirect (function-pointer register)

Arguments are pre-converted (M0b annotations); variadic call sites use the
same form (callee type carries est_variadica). Builtin routing is NOT marked
here — it is "definition absent at conexio time" (M2); est_implicitum is
enforcement-layer evidence only.

## VI. Modulus + datum

    modulus "lib/chorda.c"

    datum $titulus magnitudo N ordinatio A
        octeti  <hex byte runs>
        relocatio <offset>, $symbolum

Every string literal materializes as an anonymous datum (decoded bytes) +
relocatio at its use sites in static images. Census fact: string relocations
are the HIGH-VOLUME case (generated tables); function-address relocations
are the rare tail on the same mechanism.

## VII. Structural syntax (settled at M1a C — DECISUS 2026-07-09)

- Canonical form: THE DUMP DEFINES IT; the parser is strict;
  bar = dump(parse(t)) == t for canonical files.
- Float immediates: **%.17g decimal** (17 significant digits uniquely
  determine every IEEE double; strtod recovers exact bits; clean
  values stay clean, 0.1 shows its truth as 0.10000000000000001).
- Comments (`;` to end of line) and blank lines: **the lector skips
  them** — hand-written fixtures may carry them; such files are held
  to the IDEMPOTENCE bar (parse→dump→parse→dump identical) instead of
  byte-identity. Leading whitespace is skipped everywhere; content
  syntax is strict.
- Datum octeti: lowercase hex pairs, 4-byte groups space-separated,
  16 bytes per line. The lector accepts PARTIAL coverage (sequential
  from 0; rest stays zero) — a non-canonical convenience.
- Register names are an OPAQUE NAMESPACE to the lector (find-or-create
  by name); indices are internal and need not survive the round-trip —
  only names print.
- Provenance: in-memory instructions ALWAYS carry origo (vision pin);
  the textual form does NOT yet emit it — the optional trailing
  attribute (proposed `[fons linea:columna]`) is DEFERRED to M3
  (indicium is its consumer; the round-trip bar is unaffected).
- File extension: .medulla

## VIII. Exemplum (accepted at interview)

    functio $summa (s32 %n) -> s32
    @initium:
        %s = movere.s32 0
        %i = movere.s32 1
        salire @proba
    @proba:
        %c = maior.s32 %i, %n
        ramus %c -> @finis | @corpus
    @corpus:
        %s = addere.s32 %s, %i
        %i = addere.s32 %i, 1
        salire @proba
    @finis:
        redde %s
