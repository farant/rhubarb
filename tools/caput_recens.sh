#!/bin/bash
# tools/caput_recens.sh - CAPUT RECENTISSIMUM SEMEL per vocationem
# (sourcetur ab involucris instrumentorum: silva/<x>.sh, officina/<x>.sh,
# gesta, materia, saltuarius, tessera, tools/*_struere.sh)
#
# Olim quodque involucrum 'newest_header' per OBIECTUM vocabat:
#   [ -n "$(newest_header "$obj")" ]   ->  subshell + find + head
# id est tres processus et ambulatio directoriorum per obiectum (examen
# XLIV obiecta: ~0.5 s per vocationem sub onere; CCCXXX vocationes
# involucrorum in commissione fusionis - budget B1 E2, 2026-10-28).
# Nunc (exemplar cursorum probationum, "CAPUT RECENTISSIMUM SEMEL"):
#   - caput recentissimum SEMEL quaeritur, PIGRE (vocatione prima
#     caput_recentius, non ad sourcing - involucrum quod caput generat
#     ante compilationem id videt);
#   - obiectum stalum est nisi STRICTE recentius capite recentissimo
#     ('-nt' bash secundas solum comparat: aequalitas = recompila -
#     directio cauta, numquam caeca; 'aliquod caput obiecto recentius'
#     <=> 'caput recentissimum obiecto recentius');
#   - caput_recentius STATUM reddit, non effusionem: vocatio sine
#     subshell (in $(...) memoria pigra in subshell periret et ambulatio
#     per obiectum rediret).
# Custos: directoria sine ullo capite = viae pravae = custodia MORTUA -
# CAUTIO in stderr (ut cursores probationum).
#
# Usus:  source "$RADIX_DIR/tools/caput_recens.sh"
#        caput_recens_directoria DIR...      (directoria capitum *.h)
#        if ... || caput_recentius "$obj"; then <recompila>; fi

caput_recens_directoria () {
    CAPUT_RECENS_DIRECTORIA=("$@")
    CAPUT_RECENS=""
    CAPUT_RECENS_PARATUM=""
}

_caput_recens_parare () {
    local caput_via
    [ -n "${CAPUT_RECENS_PARATUM:-}" ] && return 0
    CAPUT_RECENS_PARATUM=1
    CAPUT_RECENS=""
    while IFS= read -r caput_via; do
        # <tolera codex="lint:nt-aequalitas" (>maximum quaeritur, non comparatio duorum: in aequalitate utrumlibet aeque valet
        if [ -z "$CAPUT_RECENS" ] || [ "$caput_via" -nt "$CAPUT_RECENS" ]; then
            CAPUT_RECENS="$caput_via"
        fi
    done < <(find "${CAPUT_RECENS_DIRECTORIA[@]}" -name '*.h' 2> /dev/null)
    if [ -z "$CAPUT_RECENS" ]; then
        echo "CAUTIO: nullum caput inventum (${CAPUT_RECENS_DIRECTORIA[*]}) - custodia recompilationis capitum MORTUA" >&2
    fi
}

# exitus 0 = caput aliquod obiecto non antiquius: recompilandum
caput_recentius () {
    _caput_recens_parare
    [ -n "$CAPUT_RECENS" ] && ! [ "$1" -nt "$CAPUT_RECENS" ]
}
