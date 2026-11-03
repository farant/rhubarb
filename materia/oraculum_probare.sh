#!/bin/bash
# materia/oraculum_probare.sh - PORTA ORACULI: silva viva contra silvam
# ad pignus (phasis V, silva-migratio-plan T3)
#
# Usage:
#   ./materia/oraculum_probare.sh              # corpus ordinarium
#   ./materia/oraculum_probare.sh lib/xar.c    # plagulae nominatae
#
# Contractus exitus: 0 omnis differentia NOMINATA et omnis nominata
# adhuc differt · 1 differentia innominata, aut linea nominata STALA,
# aut silva viva non compilat · 2 NIHIL CURSUM (binarium absens,
# corpus vacuum, lineae numero discrepantes).
#
# CUR. Migratio silvam vivam in materiam vertit; iudex eius est silva
# ad commissum pignoratum (materia/oraculum_silvae_struere.sh, PIGNUS),
# processus separatus. Quaeque columna lineae (octeti, circuitus,
# emissio, stml, errores, semantica) inter pignus et vivum confertur;
# vivum praeterea documentum pignoris legit (lectio=idem) et cum suo
# confert (comparator=aequales).
#
# DISPARES NOMINATI (materia/oraculum_silvae.dispares): divergentia
# CONSULTA (sanatio CR in T9, bugs/009-010 in phasis 5.x) ibi linea
# stat - via<TAB>columna<TAB>causa. Numquam numerantur: linea quae
# iam NON differt STALA est et rubet (ut dispar crustae nominatus):
# aliter sanatio quae aliud frangit post totalem immutatum lateret.
#
# SILVA VIVA: binarium vivum ex clausura SUA (oraculum_silvae_struere.sh,
# cursor_communis) struitur, per thesaurum - porta quae obiecta rancida
# iudicat codicem heri, non hodie, iudicat (olim obiecta silva/build per
# cursorem silvae, cursores ad gradus T5).
# <porta titulus="oraculum-silvae" gradus="fusio" tegit="materia/fontes/* silva/fontes/* silva/grammatica/*"/>

set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BASIS="$RADIX/build/oraculum_silvae"
DISPARES="$RADIX/materia/oraculum_silvae.dispares"
mkdir -p "$BASIS"
source "$RADIX/tools/sera.sh"
# sera EADEM ac struere: filius eam tenentem invenit et nihil facit
sera_capere "$BASIS/cursor.sera" || exit 2
PIGNUS=$(sed -n 's/^PIGNUS=//p' "$RADIX/materia/oraculum_silvae_struere.sh")
T0=$(date +%s)

# ---- 1. silva viva: ex clausura sua in struere (gradu 2) - olim
# obiecta per cursorem silvae (STRUERE_SOLUM) hic struebantur
# (cursores ad gradus T5: cursor nunc gradus est) ----

# ---- 2. binaria ----
"$RADIX/materia/oraculum_silvae_struere.sh" > "$BASIS/struere.effusio" 2>&1
RC=$?
if [ "$RC" -ne 0 ]; then
    cat "$BASIS/struere.effusio"
    if [ "$RC" -eq 1 ]; then
        echo "VERDICTUM: ORACULUM SILVAE FRACTUM (aedificatio)"
        exit 1
    fi
    echo "VERDICTUM: ORACULUM SILVAE NIHIL CURSUM (aedificatio)"
    exit 2
fi

# ---- 3. corpus ----
# shim (lib, include, silva/fontes) + corpus roundtrip portarum M3
if [ "$#" -gt 0 ]; then
    printf '%s\n' "$@" > "$BASIS/corpus.txt"
else
    ( cd "$RADIX" && ls lib/*.c include/*.h silva/fontes/*.c \
          probationes/fixa/roundtrip/*.c probationes/fixa/roundtrip/*.h \
          2>/dev/null ) | sort -u > "$BASIS/corpus.txt"
fi
N=$(wc -l < "$BASIS/corpus.txt" | tr -d ' ')
if [ "$N" -eq 0 ]; then
    echo "VERDICTUM: ORACULUM SILVAE NIHIL CURSUM (corpus vacuum)"
    exit 2
fi
echo "oraculum silvae: plagulae $N, pignus $PIGNUS"

# ---- 4. cursus duo ----
DOC="$BASIS/documenta"
rm -rf "$DOC"; mkdir -p "$DOC"
CORPUS=()
while IFS= read -r v; do CORPUS+=("$v"); done < "$BASIS/corpus.txt"
# PIGNUS PER MEMORIAM (budget B1 A6): silva ad pignus pura est in
# (binarium, via, octeti) - nullum IO praeter plagulam, contextus
# latinus in binario - ergo linea et documentum plagulae immutatae
# ex memoria veniunt; pignus solum in plagulas novas aut mutatas
# currit. Clavis = sha256 obiectorum pignoris (oraculum.o + amalgama.o:
# compilatio deterministica, nexus NON - binarium ipsum cursu quoque
# alio sigillo nascitur) + sha256 plagulae + via (in linea et
# documento). Documenta in $DOC per nexum durum (DLXX
# MB, copia tempus caperet); $DOC quoque cursu proximo deletur, memoria
# manet. Corpus ordinarium memoriam ad usus huius cursus putat.
BIN_SHA=$(cat "$BASIS/$PIGNUS/oraculum.o" "$BASIS/$PIGNUS/amalgama.o" \
          | shasum -a 256 | cut -c1-16)
MEM="$BASIS/memoria/$BIN_SHA"
NOVA="$BASIS/pignus_nova"
mkdir -p "$MEM"
rm -rf "$NOVA"; mkdir -p "$NOVA"
( cd "$RADIX" && shasum -a 256 "${CORPUS[@]}" ) > "$BASIS/corpus.sha"
DESUNT=()
CLAVES=()
while read -r sha via; do
    k="$MEM/$sha.${via//\//%}"
    CLAVES+=("$k")
    [ -f "$k.linea" ] || DESUNT+=("$via")
done < "$BASIS/corpus.sha"
RC_P=0
: > "$BASIS/pignus.err"
if [ "${#DESUNT[@]}" -gt 0 ]; then
    ( cd "$RADIX" && "$BASIS/$PIGNUS/oraculum" -stml "$NOVA" "${DESUNT[@]}" ) \
        > "$BASIS/pignus_nova.txt" 2> "$BASIS/pignus.err"
    RC_P=$?
    if [ "$RC_P" -eq 0 ]; then
        # linea pignoris -> clavis (sha ex corpus.sha, awk semel)
        awk 'NR == FNR { sha[$2] = $1; next }
             { split($0, c, "\t"); v = c[1]; n = v; gsub("/", "%", n)
               print sha[v] "\t" n }' "$BASIS/corpus.sha" \
            "$BASIS/pignus_nova.txt" > "$BASIS/pignus_nova.claves"
        exec 3< "$BASIS/pignus_nova.claves"
        while IFS= read -r linea; do
            IFS=$'\t' read -r sha nom <&3
            k="$MEM/$sha.$nom"
            [ -f "$NOVA/$nom.stml" ] && mv "$NOVA/$nom.stml" "$k.stml"
            printf '%s\n' "$linea" > "$k.linea.tmp" && mv "$k.linea.tmp" "$k.linea"
        done < "$BASIS/pignus_nova.txt"
        exec 3<&-
    fi
fi
: > "$BASIS/pignus.txt"
if [ "$RC_P" -eq 0 ]; then
    LINEAE=()
    for k in "${CLAVES[@]}"; do
        [ -f "$k.linea" ] && LINEAE+=("$k.linea")
        [ -f "$k.stml" ] && ln "$k.stml" "$DOC/${k#"$MEM"/*.}.stml"
    done
    [ "${#LINEAE[@]}" -gt 0 ] && cat "${LINEAE[@]}" > "$BASIS/pignus.txt"
    if [ "$#" -eq 0 ]; then
        printf '%s\n' "${CLAVES[@]}" | sed 's|$|.linea|' > "$BASIS/memoria.usae"
        printf '%s\n' "${CLAVES[@]}" | sed 's|$|.stml|' >> "$BASIS/memoria.usae"
        find "$MEM" -type f | sort > "$BASIS/memoria.omnes"
        sort "$BASIS/memoria.usae" | comm -23 "$BASIS/memoria.omnes" - \
            | while IFS= read -r v; do rm -f "$v"; done
        find "$BASIS/memoria" -mindepth 1 -maxdepth 1 -type d \
            ! -name "$BIN_SHA" -exec rm -rf {} +
    fi
fi
echo "pignus: ex memoria $(( N - ${#DESUNT[@]} )), cursa ${#DESUNT[@]}"
( cd "$RADIX" && "$BASIS/vivum" -legere "$DOC" "${CORPUS[@]}" ) \
    > "$BASIS/vivum.txt" 2> "$BASIS/vivum.err"
RC_V=$?
NP=$(wc -l < "$BASIS/pignus.txt" | tr -d ' ')
NV=$(wc -l < "$BASIS/vivum.txt" | tr -d ' ')
if [ "$RC_P" -ne 0 ] || [ "$RC_V" -ne 0 ] || [ "$NP" -ne "$N" ] || [ "$NV" -ne "$N" ]; then
    cat "$BASIS/pignus.err" "$BASIS/vivum.err"
    echo "VERDICTUM: ORACULUM SILVAE NIHIL CURSUM (exitus pignus $RC_P vivum $RC_V; lineae $NP/$NV pro $N)"
    exit 2
fi

# ---- 5. collatio: via<TAB>columna<TAB>pignus<TAB>vivum ----
paste "$BASIS/pignus.txt" "$BASIS/vivum.txt" | awk -F'\t' '
    {
        np = split($0, c, "\t")
        # pignus: campi 1..k, vivum: k+1..np (via primum in utroque)
        k = 0
        for (i = 2; i <= np; i++) if (c[i] !~ /=/) { k = i - 1; break }
        via_p = c[1]; via_v = c[k + 1]
        if (via_p != via_v) { print via_p "\tvia\t" via_p "\t" via_v; next }
        delete P; delete V
        for (i = 2; i <= k; i++)  { split(c[i], kv, "="); P[kv[1]] = substr(c[i], length(kv[1]) + 2) }
        for (i = k + 2; i <= np; i++) { split(c[i], kv, "="); V[kv[1]] = substr(c[i], length(kv[1]) + 2) }
        for (col in P) if (P[col] != V[col]) print via_p "\t" col "\t" P[col] "\t" V[col]
        for (col in V) if (!(col in P) && col != "lectio" && col != "comparator")
            print via_p "\t" col "\t-\t" V[col]
        if (V["lectio"] != "idem")         print via_p "\tlectio\tidem\t" V["lectio"]
        if (V["comparator"] != "aequales") print via_p "\tcomparator\taequales\t" V["comparator"]
    }' | sort > "$BASIS/differentiae.txt"

# ---- 6. contra dispares nominatos ----
grep -v '^#' "$DISPARES" 2>/dev/null | awk -F'\t' 'NF >= 2 { print $1 "\t" $2 }' \
    | sort -u > "$BASIS/nominati.txt"
cut -f1,2 "$BASIS/differentiae.txt" | sort -u > "$BASIS/differentes.txt"
INNOMINATI=$(comm -23 "$BASIS/differentes.txt" "$BASIS/nominati.txt")
STALI=$(comm -13 "$BASIS/differentes.txt" "$BASIS/nominati.txt")
K=$(comm -12 "$BASIS/differentes.txt" "$BASIS/nominati.txt" | grep -c . )
U=$(printf '%s' "$INNOMINATI" | grep -c .)
S=$(printf '%s' "$STALI" | grep -c .)

if [ "$U" -gt 0 ]; then
    echo "INNOMINATI (primi XX):"
    printf '%s\n' "$INNOMINATI" > "$BASIS/innominati.txt"
    head -20 "$BASIS/innominati.txt" | while IFS=$'\t' read -r via col; do
        awk -F'\t' -v v="$via" -v c="$col" '$1 == v && $2 == c {
            printf "  %s  %s  pignus=%s  vivum=%s\n", $1, $2, $3, $4 }' \
            "$BASIS/differentiae.txt"
    done
fi
if [ "$S" -gt 0 ]; then
    echo "STALI (nominati sed iam non differunt):"
    printf '%s\n' "$STALI" | sed 's/^/  /'
fi
echo "murus: $(( $(date +%s) - T0 ))s"
if [ "$U" -gt 0 ] || [ "$S" -gt 0 ]; then
    echo "VERDICTUM: ORACULUM SILVAE FRACTUM (nominati $K, innominati $U, stali $S)"
    exit 1
fi
echo "VERDICTUM: ORACULUM SILVAE sanum (nominati $K, innominati 0, stali 0)"
exit 0
