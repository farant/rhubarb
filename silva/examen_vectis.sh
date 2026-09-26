#!/bin/bash

# silva/examen_vectis.sh - VECTIS DIFFERENTIALIS M4a (chunk D)
#
# Modus ordinarius: ① percursus fugarum oraculi (fixturae C99/GNU
#   contra chordam sigillatam - omnes REICI debent); ② corpus
#   invalidum: examen REICE ad pinnas EXSPECTA ET oraculum clang
#   consentiens.
# Modus -corpus: latus examinis super corpus verum - [verdictum
#   REICE] percursus contra tabulam exclusionum pinnatam.
# Exit: 0 = vectis tenet | 1 = discrepantia
#
# GRAMMATICA PINNARUM (STML annotationes, frustum E1 2026-07-22;
# ante: desideratum 01KXRD8JVS):
#   /* <exspecta codex="CODEX"/> */  - forma PRAELATA: linea
#     proxima non-pinna non-vacua flagrat; pinnae cumulantur;
#     editiones alibi eam numquam invalidant
#   /* <exspecta linea="13" codex="CODEX"/> */ - forma absoluta,
#     RECESSUS (diagnosticum in linea 1, supra quam nihil poni
#     potest - ordo_pravus_vendicati)
#   Resolutor = identitates --exspecta (ambulatio arboris; pinna
#   malformata CLAMAT - awk vetus tacite prosam faciebat).
#
# Chorda oraculi SIGILLATA (officina-m4a-spec.md §V); versio clang
# scribitur (linea acceptationis inter versiones movetur!).

set -u
SILVA_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RADIX_DIR="$(cd "$SILVA_DIR/.." && pwd)"
cd "$RADIX_DIR"

FIXA="$SILVA_DIR/probationes/fixa/examinis"
# <tolera codex="lint:vexilla-domus" (>clang ORACULUM est hic: vexilla EXPERIMENTI sunt, non aedificationis domus - tabulam domus fontare oraculum cum domo fluere faceret et comparationem ipsam perderet
declare -a ORACULUM=(
    clang -x c -std=c89 -pedantic-errors
    -Wno-long-long -Wno-overlength-strings
    -Werror=c99-designator
    -Wno-error=implicit-function-declaration
    -Wno-strict-prototypes
    -ferror-limit=0 -fno-caret-diagnostics -fsyntax-only
)

echo "oraculum: $(clang --version | head -1)"
fracta=0

# pinnae in paria "linea:CODEX" solutae (instrumentum
# annotationum; recentia binarii semel infra praestita)
"$SILVA_DIR/identitates.sh" -struere || exit 1
_pinnae_solvere () {
    "$SILVA_DIR/build/identitates" --exspecta "$1" || {
        echo "  PINNAE RECUSATAE: $1" >&2
        return 1
    }
}

# ① percursus fugarum: C99/GNU contra oraculum - omnes REICI
echo "--- percursus fugarum oraculi ---"
# ...ET ab examine (2026-09-01): ante hoc fugae oraculum solum
# calibrabant - examen V ex XII accipiebat dum vectis tenebat
# (indagatio 01KZBYEHJP). Assertio quae deerat: iudex noster ipse.
for f in "$FIXA"/fugae/*.fuga; do
    if "${ORACULUM[@]}" "$f" >/dev/null 2>&1; then
        echo "  FUGA: $(basename "$f") oraculo ACCEPTUM (C99/GNU!)"
        fracta=1
    fi
    verdictum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null \
        | awk -F'\t' '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "REICE" ]; then
        echo "  FUGA: $(basename "$f") EXAMINI accepta (verdictum ${verdictum:-nullum})"
        fracta=1
    fi
done
[ "$fracta" -eq 0 ] && echo "  fugae: 0 (omnes reiectae - oraculo ET examine)"

# tabula nominum codicum ex capite (numerus = ordo enumerationis)
MAPPA="$SILVA_DIR/build/codices_mappa.txt"
grep -o 'EXAMEN_CODEX_[A-Z_]*' "$SILVA_DIR/fontes/silva_c89_semantica.h" \
    | awk '!visa[$0]++ && $0 != "EXAMEN_CODEX_NUMERUS" {print NR-1"\t"$0}' \
    > "$MAPPA"

# ② corpus invalidum: examen ad pinnas + oraculum consentiens
echo "--- corpus invalidum (pinnae EXSPECTA) ---"
for f in "$FIXA"/*.invalidum; do
    basis="$(basename "$f")"
    exspecta="$(_pinnae_solvere "$f")" || fracta=1

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "REICE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum (non REICE)"
        fracta=1
        continue
    fi
    # quaeque pinna: (linea, codex-nomen) in effuso -machina
    for pinna in $exspecta; do
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $5==c {inventum=1} END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
    done
    # oraculum consentiens (exclusiones nominatae infra)
    case "$basis" in
        *) if "${ORACULUM[@]}" "$f" >/dev/null 2>&1; then
               echo "  DISCREPANTIA: $basis oraculum ACCIPIT," \
                    "examen REICIT"
               fracta=1
           fi ;;
    esac
done

# ②b corpus domesticum: legale C89 (verdictum ACCIPE manet) sed
#   ordines DOMESTICUM ad pinnas; oraculum clang easdem lineas monet
#   ET numerus monitorum == numerus pinnarum (aequalitas honesta).
#   Vexillum oraculi per fixturam: directivum "ORACULUM -W..." in
#   commentario; ordinarius -Wsign-conversion.
echo "--- corpus domesticum (pinnae EXSPECTA + oraculum) ---"
# <tolera codex="lint:vexilla-domus" (>clang ORACULUM est hic: vexilla EXPERIMENTI sunt, non aedificationis domus - tabulam domus fontare oraculum cum domo fluere faceret et comparationem ipsam perderet
declare -a ORACULUM_BASIS_D=(
    clang -x c -std=c89 -pedantic
    -Wno-long-long -fno-caret-diagnostics -fsyntax-only
)
for f in "$FIXA"/*.domesticum; do
    [ -e "$f" ] || continue
    basis="$(basename "$f")"
    exspecta="$(_pinnae_solvere "$f")" || fracta=1
    vexillum="$(grep -o 'ORACULUM -W[a-z-]*' "$f" | head -1 \
        | sed 's/ORACULUM //')"
    [ -z "$vexillum" ] && vexillum="-Wsign-conversion"
    exemplar="${vexillum#-W}"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "ACCIPE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum" \
             "(non ACCIPE - legale C89!)"
        fracta=1
        continue
    fi
    monita="$("${ORACULUM_BASIS_D[@]}" "$vexillum" "$f" 2>&1)"
    for pinna in $exspecta; do
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $4=="domesticum" && $5==c {inventum=1}
             END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
        if ! printf '%s\n' "$monita" | grep -q ":$linea:.*$exemplar"; then
            echo "  ORACULUM DISSENTIT: $basis linea $linea" \
                 "(clang non monet)"
            fracta=1
        fi
    done
    n_pinnae="$(printf '%s\n' "$exspecta" | grep -c . || true)"
    # solum ordines "warning:" - notae clang exemplar repetere
    # possunt ("uninitialized use occurs here" sub -Wuninitialized)
    n_monita="$(printf '%s\n' "$monita" \
        | grep -c "warning:.*$exemplar" || true)"
    if [ "$n_pinnae" != "$n_monita" ]; then
        echo "  DISCREPANTIA NUMERI: $basis pinnae=$n_pinnae" \
             "monita oraculi=$n_monita"
        fracta=1
    fi
done

# ②b′ corpus suspectum: ut domesticum sed gradu SUSPECTUM (paritas
#   monitorum clang extra vexilla domus - e.g. -Wreturn-type).
#   Eadem mechanica: pinnae + oraculum easdem lineas monet + numeri
#   aequales. Functiones uni-lineae in fixturis (clang ad uncum
#   clausum monet).
echo "--- corpus suspectum (pinnae EXSPECTA + oraculum) ---"
for f in "$FIXA"/*.suspectum; do
    [ -e "$f" ] || continue
    basis="$(basename "$f")"
    exspecta="$(_pinnae_solvere "$f")" || fracta=1
    vexillum="$(grep -o 'ORACULUM -W[a-z-]*' "$f" | head -1 \
        | sed 's/ORACULUM //')"
    [ -z "$vexillum" ] && vexillum="-Wsign-conversion"
    exemplar="${vexillum#-W}"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "ACCIPE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum" \
             "(non ACCIPE - legale C89!)"
        fracta=1
        continue
    fi
    monita="$("${ORACULUM_BASIS_D[@]}" "$vexillum" "$f" 2>&1)"
    for pinna in $exspecta; do
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $4=="suspectum" && $5==c {inventum=1}
             END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
        if ! printf '%s\n' "$monita" | grep -q ":$linea:.*$exemplar"; then
            echo "  ORACULUM DISSENTIT: $basis linea $linea" \
                 "(clang non monet)"
            fracta=1
        fi
    done
    n_pinnae="$(printf '%s\n' "$exspecta" | grep -c . || true)"
    # solum ordines "warning:" - notae clang exemplar repetere
    # possunt ("uninitialized use occurs here" sub -Wuninitialized)
    n_monita="$(printf '%s\n' "$monita" \
        | grep -c "warning:.*$exemplar" || true)"
    if [ "$n_pinnae" != "$n_monita" ]; then
        echo "  DISCREPANTIA NUMERI: $basis pinnae=$n_pinnae" \
             "monita oraculi=$n_monita"
        fracta=1
    fi
done

# ②c corpus severum: gradus SEVERI (supra oraculum - clang SILET ad
#   pinnas, id ipsum proprietas est); verdictum ACCIPE; ordines
#   codicum pinnatorum == pinnae exacte (lineae toleratae QUIETAE);
#   IRRITUM 0. Vexillum oraculi per directivum "ORACULUM -W..." ut
#   supra.
echo "--- corpus severum (pinnae + oraculum inversum) ---"
for f in "$FIXA"/*.severum; do
    [ -e "$f" ] || continue
    basis="$(basename "$f")"
    exspecta="$(_pinnae_solvere "$f")" || fracta=1
    vexillum="$(grep -o 'ORACULUM -W[a-z-]*' "$f" | head -1 \
        | sed 's/ORACULUM //')"
    [ -z "$vexillum" ] && vexillum="-Wsign-conversion"
    exemplar="${vexillum#-W}"

    effusum="$("$SILVA_DIR/build/examen" "$f" -machina 2>/dev/null)"
    verdictum="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$1=="VERDICTUM"{print $2}')"
    if [ "$verdictum" != "ACCIPE" ]; then
        echo "  DISCREPANTIA: $basis examen=$verdictum (non ACCIPE)"
        fracta=1
        continue
    fi
    monita="$("${ORACULUM_BASIS_D[@]}" "$vexillum" "$f" 2>&1)"
    n_pinnae=0
    codices_pinnati=""
    for pinna in $exspecta; do
        n_pinnae=$((n_pinnae + 1))
        linea="${pinna%%:*}"
        nomen="EXAMEN_CODEX_${pinna#*:}"
        numerus="$(awk -F'\t' -v n="$nomen" '$2==n{print $1}' "$MAPPA")"
        if [ -z "$numerus" ]; then
            echo "  PINNA IGNOTA: $basis $nomen"
            fracta=1
            continue
        fi
        codices_pinnati="$codices_pinnati $numerus"
        if ! printf '%s\n' "$effusum" | awk -F'\t' \
            -v l="$linea" -v c="$numerus" \
            '$2==l && $4=="domesticum" && $5==c {inventum=1}
             END{exit !inventum}'; then
            echo "  PINNA DEEST: $basis linea $linea $nomen"
            fracta=1
        fi
        if printf '%s\n' "$monita" | grep -q ":$linea:.*$exemplar"; then
            echo "  ORACULUM FLAGRAT: $basis linea $linea" \
                 "(gradus severus supra oraculum esse debet!)"
            fracta=1
        fi
    done
    n_severa="$(printf '%s\n' "$effusum" | awk -F'\t' \
        -v cs="$codices_pinnati" \
        'BEGIN{n=split(cs,a," "); for(i=1;i<=n;i++) cc[a[i]]=1}
         ($5 in cc){k++} END{print k+0}')"
    if [ "$n_severa" != "$n_pinnae" ]; then
        echo "  DISCREPANTIA NUMERI: $basis severa=$n_severa" \
             "pinnae=$n_pinnae (tolerata flagrant?)"
        fracta=1
    fi
    n_irrita="$(printf '%s\n' "$effusum" | awk -F'\t' \
        '$5==56' | wc -l | tr -d ' ')"
    if [ "$n_irrita" != "0" ]; then
        echo "  IRRITUM: $basis $n_irrita TOLERA irrita"
        fracta=1
    fi
done

# ④ LOCA ET EXCERPTA (silva-migratio T15b): syntaxis fracta ordinem
#    cum linea/columna VERA fert (mors GLR, non 'nodi erroris N' linea
#    0 - uncus post-editionem locum agentibus nunc dat), modus humanus
#    excerptum sub diagnostico (relata 'hic coepit' + primaria 'hic
#    exspectatur'); diagnosticum semanticum excerptum suum quoque.
echo "--- loca syntaxis et excerpta (T15b) ---"
LOCA="build/vectis_loca"
mkdir -p "$SILVA_DIR/$LOCA"
printf '#include "latina.h"\n\ninterior integer\nbona (vacuum)\n{\n    redde I;\n}\n\ninterior integer\nmala (vacuum)\n{\n    redde I +;\n}\n' \
    > "$SILVA_DIR/$LOCA/mala.c"
printf '#include "latina.h"\n\ninterior integer\nf (vacuum)\n{\n    integer x;\n    redde y;\n}\n' \
    > "$SILVA_DIR/$LOCA/sem.c"
_loca () {  # $1 = descriptio, $2 = exspectatum, $3 = factum
    if [ "$2" = "$3" ]; then echo "  ok   $1"
    else echo "  FRACTUM $1: exspectatum [$2] factum [$3]"; fracta=1; fi
}
( cd "$RADIX_DIR" && ./silva/examen.sh "silva/$LOCA/mala.c" ) \
    > "$SILVA_DIR/$LOCA/mala.out" 2>/dev/null
( cd "$RADIX_DIR" && ./silva/examen.sh "silva/$LOCA/mala.c" -machina ) \
    > "$SILVA_DIR/$LOCA/mala.tsv" 2>/dev/null
( cd "$RADIX_DIR" && ./silva/examen.sh "silva/$LOCA/sem.c" ) \
    > "$SILVA_DIR/$LOCA/sem.out" 2>/dev/null
_loca "syntaxis: linea prima ad mortem" \
    "silva/$LOCA/mala.c:12:14: [violatio] lexema quod grammatica hic non accipit" \
    "$(sed -n 1p "$SILVA_DIR/$LOCA/mala.out")"
_loca "syntaxis: relata 'hic coepit' ad IX:1" \
    "     | ^ hic coepit" "$(sed -n 3p "$SILVA_DIR/$LOCA/mala.out")"
_loca "syntaxis: primaria 'hic exspectatur' sub columna XIV" \
    "     |              ^ hic exspectatur" \
    "$(sed -n 5p "$SILVA_DIR/$LOCA/mala.out")"
_loca "syntaxis -machina: linea/columna/causa (uncus eas legit)" \
    "12:14:lexema quod grammatica hic non accipit" \
    "$(awk -F'\t' '$4=="violatio"{print $2":"$3":"$7}' "$SILVA_DIR/$LOCA/mala.tsv")"
_loca "semantica: excerptum sub diagnostico" \
    "   7 |     redde y;" "$(sed -n 2p "$SILVA_DIR/$LOCA/sem.out")"
_loca "semantica: signum sub columna XI" \
    "     |           ^" "$(sed -n 3p "$SILVA_DIR/$LOCA/sem.out")"
# ⑤ MACRA SYNTACTICA SYSTEMATIS (silva-migratio T16a; lapide bugs/009):
#    va_arg/offsetof TYPUM accipiunt - sine macro in systema_c89.h
#    ERROR syntaxis erant; va_start/va_end/assert 'vocatio implicita'
#    et 'variabilis ininitiata' falsa dabant. Plagula omnia quinque
#    utens: ACCIPE, ordo nullus ullius gradus.
printf "#include <stdarg.h>\n#include <stddef.h>\n#include <assert.h>\n\nstruct par { int a; int b; };\n\nint\nf (int n, ...)\n{\n    va_list va;\n    char* s;\n    size_t o = offsetof(struct par, b);\n    va_start(va, n);\n    s = va_arg(va, char*);\n    va_end(va);\n    assert(s != 0);\n    return (int)o + (s != 0);\n}\n" > "$SILVA_DIR/$LOCA/va.c"
( cd "$RADIX_DIR" && ./silva/examen.sh "silva/$LOCA/va.c" -machina ) \
    > "$SILVA_DIR/$LOCA/va.tsv" 2>/dev/null
_loca "systema: va_arg/offsetof/va_start/va_end/assert ACCIPE" \
    "ACCIPE" "$(awk -F'\t' '$1=="VERDICTUM"{print $2}' "$SILVA_DIR/$LOCA/va.tsv")"
_loca "systema: nullus ordo (nec suspectum falsum)" \
    "0" "$(awk -F'\t' '$1!="VERDICTUM" && $1!~/^#/' "$SILVA_DIR/$LOCA/va.tsv" | wc -l | tr -d ' ')"
rm -rf "$SILVA_DIR/$LOCA"

# ⑥ EXTENSIO COMPILATORIS (silva-migratio T17b; lapide bugs/010):
#    silva __attribute__ accipit (T17a), examen usum in codice domus
#    VIOLATIONE nominat (91) - in codice, in '#define', in ramo non
#    sumpto ('#ifdef __GNUC__'); '#if 0' praeteritur. Excusatio
#    declarata (<tolera codex="EXTENSIO_COMPILATORIS" (>causa>) per
#    lineam absorbet - etiam intra ramum non sumptum (collector
#    annotationum laminas crudas nunc ambulat); tolera quae nihil
#    absorbet TOLERA_IRRITUM manet.
echo "--- extensio compilatoris (T17b) ---"
mkdir -p "$SILVA_DIR/$LOCA"
cat > "$SILVA_DIR/$LOCA/attr.c" <<'FIXA'
struct s { int x; } __attribute__((packed));
int f (int a) __attribute__((unused));
#ifdef __GNUC__
#define NON_REDIT __attribute__((noreturn))
#else
#define NON_REDIT
#endif
#define FORMA(a, b) __attribute__((__format__(printf, a, b)))
#if 0
int g (void) __attribute__((cold));
#endif
int f (int a) { return a; }
FIXA
cat > "$SILVA_DIR/$LOCA/tolerata.c" <<'FIXA'
/* <tolera codex="EXTENSIO_COMPILATORIS" (>forma protocolli retis */
struct s { int x; } __attribute__((packed));
int f (int a) __attribute__((unused)); /* <tolera codex="EXTENSIO_COMPILATORIS" (>probatio */
#ifdef __GNUC__
/* <tolera codex="EXTENSIO_COMPILATORIS" (>clang fluxum noscit */
#define NON_REDIT __attribute__((noreturn))
#else
#define NON_REDIT
#endif
/* <tolera codex="EXTENSIO_COMPILATORIS" (>clang formam custodit */
#define FORMA(a, b) __attribute__((__format__(printf, a, b)))
int f (int a) { return a; }
FIXA
cat > "$SILVA_DIR/$LOCA/irrita.c" <<'FIXA'
/* <tolera codex="EXTENSIO_COMPILATORIS" (>nihil hic absorbetur */
int x;
FIXA
for f in attr tolerata irrita; do
    ( cd "$RADIX_DIR" && ./silva/examen.sh "silva/$LOCA/$f.c" -machina ) \
        > "$SILVA_DIR/$LOCA/$f.tsv" 2>/dev/null
done
CODEX_EXT="$(awk -F'\t' '$2=="EXAMEN_CODEX_EXTENSIO_COMPILATORIS"{print $1}' "$MAPPA")"
CODEX_IRR="$(awk -F'\t' '$2=="EXAMEN_CODEX_TOLERA_IRRITUM"{print $1}' "$MAPPA")"
_loca "extensio: verdictum REICE" \
    "REICE" "$(awk -F'\t' '$1=="VERDICTUM"{print $2}' "$SILVA_DIR/$LOCA/attr.tsv")"
_loca "extensio: sedes (codex, #define, ramus non sumptus; #if 0 non)" \
    "1:21 2:15 4:19 8:21" \
    "$(awk -F'\t' -v c="$CODEX_EXT" '$5==c && $4=="violatio"{print $2":"$3}' "$SILVA_DIR/$LOCA/attr.tsv" | sort -n | tr '\n' ' ' | sed 's/ $//')"
_loca "extensio: attributum nominatum (__format__ -> format)" \
    "1" "$(awk -F'\t' -v c="$CODEX_EXT" '$5==c && $2=="8" && $7 ~ /__attribute__\(\(format\)\)/' "$SILVA_DIR/$LOCA/attr.tsv" | wc -l | tr -d ' ')"
_loca "extensio: formae/fluxus modulum caecum nominat (packed, noreturn; unused non)" \
    "1 4" \
    "$(awk -F'\t' -v c="$CODEX_EXT" '$5==c && $7 ~ /caecus/{print $2}' "$SILVA_DIR/$LOCA/attr.tsv" | sort -n | tr '\n' ' ' | sed 's/ $//')"
_loca "extensio: tolera per lineam (etiam in ramo non sumpto) - ACCIPE" \
    "ACCIPE" "$(awk -F'\t' '$1=="VERDICTUM"{print $2}' "$SILVA_DIR/$LOCA/tolerata.tsv")"
_loca "extensio: tolerata - ordo nullus (nec irritum)" \
    "0" "$(awk -F'\t' '$1!="VERDICTUM" && $1!~/^#/' "$SILVA_DIR/$LOCA/tolerata.tsv" | wc -l | tr -d ' ')"
_loca "extensio: tolera sine invento -> TOLERA_IRRITUM" \
    "1:$CODEX_IRR" \
    "$(awk -F'\t' '$1!="VERDICTUM" && $1!~/^#/{print $2":"$5}' "$SILVA_DIR/$LOCA/irrita.tsv")"
rm -rf "$SILVA_DIR/$LOCA"

# ③ -corpus: columna verdicti percursus contra exclusiones pinnatas
#    + plagulae INFRA (annotatio/compositio/praeparatio fracta):
#    praefixo FRACTA in tabulam intrant - annotatio fracta oraculo
#    INVISIBILIS esse non debet (nec REICE nec exclusio = tacita)
if [ "${1:-}" = "-corpus" ]; then
    echo "--- corpus verum (percursus REICE vs exclusiones) ---"
    ./silva/percursus.sh -semantica 2>&1 \
        | awk '/\[verdictum REICE\] / {
                   sub(/.*\[verdictum REICE\] /, ""); print; next }
               /\[(externa|compositio|praeparatio) fracta\] / {
                   sub(/.*fracta\] /, ""); print "FRACTA " $0 }' \
        | sort > "$SILVA_DIR/build/reice_currentes.txt"
    # forma pinnae: via<TAB>causa (lineae '#' = commentaria) -
    # columna prima sola contra sweep diffatur; causa documentum est
    if ! grep -v '^#' "$FIXA/exclusiones.txt" | cut -f1 \
            | diff -u - "$SILVA_DIR/build/reice_currentes.txt"; then
        echo "  DISCREPANTIA: REICE corporis != exclusiones pinnatae"
        fracta=1
    else
        echo "  exclusiones: $(grep -cv '^#' "$FIXA/exclusiones.txt" \
            | tr -d ' ') pinnatae, omnes notae"
    fi
fi

if [ "$fracta" -eq 0 ]; then
    echo "VECTIS TENET"
    exit 0
fi
echo "VECTIS FRACTUS"
exit 1
