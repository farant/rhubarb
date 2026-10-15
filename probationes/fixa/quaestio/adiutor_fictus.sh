#!/bin/sh
# adiutor_fictus.sh - bin/quaestio fictum (probatio_vicus_quaestio_porta,
# quaestio-rivus T8): tabulae ut adiutor verus, scaena per -quaestio.
# Argumenta ut bin/quaestio (-exemplar X -lineae L -latitudo W
# -quaestio <scaena>); nulla clavis, nullum rete.
q=""
while [ $# -gt 0 ]; do
    if [ "$1" = "-quaestio" ]; then
        q="$2"
    fi
    shift
done
case "$q" in
    bonum)
        printf 'T 6\nSalve '
        printf 'T 6\nmunde.'
        printf 'U 12 34\nF end_turn\n'
        ;;
    mors)
        printf 'T 4\nPars'
        echo "adiutor fictus moritur" >&2
        exit 3
        ;;
    pravum)
        printf 'Q zz\n'
        sleep 5
        ;;
    error)
        printf 'E 10\nOverloadedF error\n'
        ;;
    lente)
        printf 'T 5\nPrima'
        sleep 30
        printf 'F end_turn\n'
        ;;
esac
