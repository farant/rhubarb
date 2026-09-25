# silva/materia_substratum.sh - substratum MATERIAE in aedificatione
# silvae (phasis V, silva-migratio-plan T6a) - FONTANDUM, non currendum
#
# Usus (post RADIX_DIR cognitum):
#   source "$RADIX_DIR/silva/materia_substratum.sh"
#   materia_substratum_struere "$BUILD_DIR" "${GCC_FLAGS[@]}" || exit 1
#   obj_files="$obj_files $MATERIA_OBIECTA"
#
# CUR. Cursor silvae et instrumenta eius XX (examen, formator, nexus,
# renominare, selecta...) OMNEM silva/fontes/*.c compilant et nectunt -
# consulto, ne modulus novus tacite omittatur (tools/silva_fontes_
# generare.sh). Frons C89 (silva_frons, T6b) et deinde switch
# modulorum (T7-T11) materiam vocant, ergo quisque nexus silvae
# substratum materiae ferre debet. Tabula UNA hic, ne XXIII copiae
# divergant.
#
# Moduli = substratum quod silva consumit (idem ac shim_probare.sh):
# ceteri materiae moduli (coctor, sedes, diagnostica...) clientium
# sunt, non silvae - addantur cum consumptor eos poscit.
#
# Custodia capitum: obiectum recompilatur si fons eius recentior est,
# aut si caput quodlibet in include/ vel materia/fontes/ (forma
# '! [ obj -nt caput ]' - aequalitas temporis recompilat, lex nt-
# aequalitas).

MATERIA_SUBSTRATUM=(
    materia_token
    materia_nodus
    materia_scribere
    materia_arbor
    materia_arbor_aequalitas
    materia_lexicon
)

materia_substratum_struere () {   # <directorium obiectorum> <vexilla...>
    local dir="$1"
    shift
    local radix m src obj caput recens
    radix="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
    MATERIA_OBIECTA=""
    for m in "${MATERIA_SUBSTRATUM[@]}"; do
        src="$radix/materia/fontes/$m.c"
        obj="$dir/$m.o"
        recens=1
        if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
            recens=0
        else
            for caput in "$radix"/include/*.h "$radix"/materia/fontes/*.h; do
                if ! [ "$obj" -nt "$caput" ]; then
                    recens=0
                    break
                fi
            done
        fi
        if [ "$recens" -eq 0 ]; then
            echo "  [materia] $m.c" >&2
            clang "$@" -I"$radix/include" -I"$radix/materia/fontes" \
                -c "$src" -o "$obj" || return 1
        fi
        MATERIA_OBIECTA="$MATERIA_OBIECTA $obj"
    done
    return 0
}
