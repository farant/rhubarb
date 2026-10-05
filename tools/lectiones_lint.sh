#!/bin/bash
# tools/lectiones_lint.sh - LINT canalis lectionum (fabrica plan 2 T2):
# vocationes IO crudae extra canalem (filum, lectiones).
#
# CUR: liber lectionum videt solum quod per filum / via_existit /
# iter_directoria / lectiones_ambitus transit. Lectio cruda (fopen,
# stat, opendir, access, getenv) in instrumento cuius vestigium fabrica
# credit = ingressus invisibilis = memoria falsa RECENS (spec 2 par. X.1).
#
# Fons: nexus silvae (build/nexus.tsv, ordines 'usus' functionum
# systematis cum plagula et linea) - quaestio exacta, non grep.
# Via pilota = clausura aedilis instrumentorum quorum vestigia fabrica
# credit (VIA_PILOTA infra); ibi vocatio cruda OBSTAT (exitus 1);
# ceterae MONENTUR (numerus per plagulam).
#
# Usus: ./tools/lectiones_lint.sh [-omnes]   (-omnes: omnes sedes)
# Exitus: 0 via pilota munda · 1 vocatio cruda in via pilota · 2 usus
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
OMNES=0
[ "${1:-}" = "-omnes" ] && OMNES=1
[ -x bin/aedilis ] || { echo "lectiones_lint: bin/aedilis deest" >&2; exit 2; }
./silva/nexus.sh -renovare > /dev/null 2>&1 || { echo "lectiones_lint: nexus renovari nequit" >&2; exit 2; }

# canalis ipse: hae plagulae IO crudam iure faciunt. thesaurus: cache
# per sigilla ingressuum iam lectorum, non ingressus (iudex radicem eius
# e vestigiis eicit, plan 2 T3) - getenv THESAURUS_GENERATIO crudum
EXEMPTAE="lib/filum.c lib/lectiones.c lib/thesaurus.c"
# instrumenta quorum vestigia fabrica credit (lectiones="verum"), et
# probationes portae quae per vestigium iudicatur (actio 'iudicium',
# fabrica spec 3 T3: porta_toml) - IO cruda in clausura earum OBSTAT
VIA_PILOTA_RADICES="tools/aedilis.c $(ls toml/probationes/probatio_toml_*.c | tr '\n' ' ')"

T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
for r in $VIA_PILOTA_RADICES; do
    ./bin/aedilis "$r" --enumerare 2>/dev/null
    echo "$r"
done | sort -u > "$T/pilota"

awk -F'\t' '
    ($1=="fopen"||$1=="freopen"||$1=="opendir"||$1=="stat"||$1=="lstat"||
     $1=="access"||$1=="getenv"||$1=="open") && $2=="usus" {
        print $4 "\t" $5 "\t" $1 }' build/nexus.tsv | sort -u > "$T/crudae"

obstat=0
monita=0
notatae=0
while IFS='	' read -r via linea functio; do
    case " $EXEMPTAE " in *" $via "*) continue ;; esac
    # NOTATA: vocatio cruda cuius linea PRAECEDENS '/* lectiones:
    # notatur */' fert - eventum suum ipsa ad librum refert
    # (via_existit, ansae directoriorum aedilis - T1). Signum per
    # lineam, non vicinitas: vocatio cruda NOVA iuxta notatam olim
    # transibat (planta T2, heuristica XII linearum)
    if [ "$linea" -gt 1 ] && sed -n "$((linea - 1))p" "$via" 2>/dev/null \
            | grep -q 'lectiones: notatur'; then
        notatae=$((notatae + 1))
        continue
    fi
    if grep -qxF "$via" "$T/pilota"; then
        echo "OBSTAT: $via:$linea $functio (via pilota - per filum/lectiones)"
        obstat=$((obstat + 1))
    else
        monita=$((monita + 1))
        [ "$OMNES" -eq 1 ] && echo "monitum: $via:$linea $functio"
    fi
done < "$T/crudae"
echo "lectiones_lint: via pilota $(wc -l < "$T/pilota" | tr -d ' ') plagulae," \
     "$obstat obstantia, $notatae notatae; extra viam $monita vocationes crudae (monita; -omnes)"
[ "$obstat" -eq 0 ] && exit 0
exit 1
