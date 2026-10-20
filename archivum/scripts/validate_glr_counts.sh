#!/bin/bash
#
# validate_glr_counts.sh
# Validates that STATE_*_COUNT macros match actual entry counts in arbor2_glr_tabula.c
#

TABULA="lib/arbor2_glr_tabula.c"

if [[ ! -f "$TABULA" ]]; then
    echo "Error: $TABULA not found"
    exit 1
fi

echo "Validating GLR state counts..."
echo ""

# Create temp files
MACRO_FILE=$(mktemp)
ACTUAL_FILE=$(mktemp)
ACTIONES_FILE=$(mktemp)
trap "rm -f $MACRO_FILE $ACTUAL_FILE $ACTIONES_FILE" EXIT

# Extract STATE_*_COUNT macro definitions
grep -E '#define[[:space:]]+STATE_[0-9]+_COUNT[[:space:]]+[0-9]+' "$TABULA" | \
    sed -E 's/#define[[:space:]]+STATE_([0-9]+)_COUNT[[:space:]]+([0-9]+)/\1 \2/' | \
    sort -n > "$MACRO_FILE"

# Extract just the ACTIONES array (between "ACTIONES[] = {" and "NUM_ACTIONES")
sed -n '/^hic_manens Arbor2TabulaActio ACTIONES\[\]/,/^hic_manens i32 NUM_ACTIONES/p' "$TABULA" > "$ACTIONES_FILE"

# Count actual entries per state within the ACTIONES array only
awk '
    /\/\* State [0-9]+:/ {
        if (current_state != "") {
            print current_state, count
        }
        # Extract state number using gsub
        line = $0
        gsub(/.*State /, "", line)
        gsub(/:.*/, "", line)
        current_state = line
        count = 0
        next
    }
    /ARBOR2_ACTIO_(SHIFT|REDUCE|ACCEPT|ERROR)/ {
        count++
    }
    END {
        if (current_state != "") {
            print current_state, count
        }
    }
' "$ACTIONES_FILE" | sort -n > "$ACTUAL_FILE"

# Compare
errors=0
max_state=$(tail -1 "$MACRO_FILE" | cut -d' ' -f1)

echo "State | Macro | Actual | Status"
echo "------|-------|--------|-------"

for s in $(seq 0 $max_state); do
    macro=$(grep "^$s " "$MACRO_FILE" | awk '{print $2}')
    actual=$(grep "^$s " "$ACTUAL_FILE" | awk '{print $2}')

    macro=${macro:-0}
    actual=${actual:-0}

    if [[ "$macro" != "$actual" ]]; then
        printf "%5d | %5d | %6d | MISMATCH\n" "$s" "$macro" "$actual"
        ((errors++))
    fi
done

echo ""
echo "----------------------------------------"
echo "States checked: $((max_state + 1))"
echo "Mismatches: $errors"

if (( errors > 0 )); then
    echo ""
    echo "FIX: Update STATE_*_COUNT macros to match actual counts"
    exit 1
else
    echo ""
    echo "All state counts valid."
    exit 0
fi
