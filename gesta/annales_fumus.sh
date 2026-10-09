#!/bin/bash
# gesta/annales_fumus.sh - porta SEDIS ANNALIUM (gesta/fontes/annales_sedes.h)
#
# CUR: acta tabularii domus totius sunt, non arboris. Olim omnis
# launcher annales ex arbore sua legebat: residens aut via frigida in
# arbore secunda scrinium VACUUM incipiebat et seq 1 annalibus
# commissis appendebat (quaestiones …MKMD2, …VSY50E). Haec porta
# sedem, custodem et scriptores SIMUL probat - semper in sedibus
# temporariis, numquam in tabulario vivo.
#
#   I    sedes: $RHUBARB_ANNALES vincit; directorium absens recusatur
#   II   custos: sedes vacua, annales sine scrinio, scrinium sine
#        annalibus - omnia recusata, NIHIL creatum
#   III  genesis expressa: sedes vacua -> tabularium novum; iterum
#        recusatur
#   IV   scriptores SIMUL: II processus x N notae - seq unica et densa,
#        annales ordine seq, annales == acta (numerus et ultima)
#   V    restitutio: scrinium ex annalibus solis == scrinium scriptum
#   VI   residens MCP et daemon fori in sede mala: RECUSATUM, exitus 1
#   VII  arbor recens (legatum): annales commissi sine scrinio -
#        residens RECUSAT (non incipit vacuum)
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
./gesta/frigida.sh >/dev/null 2>&1      # aedificat (usus: exitus 2)
./gesta/tabulariumd.sh -struere </dev/null >/dev/null 2>&1 || exit 2
NF="$RADIX/gesta/build/nota_frigida"
TB="$RADIX/gesta/build/tabularium"
TD="$RADIX/gesta/build/tabulariumd"
for b in "$NF" "$TB" "$TD"; do [ -x "$b" ] || { echo "annales_fumus: $b deest"; exit 2; }; done
T="$(mktemp -d "${TMPDIR:-/tmp}/annales_fumus.XXXXXX")" || exit 2
trap 'rm -rf "$T"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
N=25
# processus cum TECTO (V s): residens aut daemon qui NON recusat
# currit et numquam exit - porta pendens = porta mortua. Exitus 124
# = tectum attactum (processus occisus); effusus in "$T/effusus".
tecto () {
    local p k=0
    "$@" </dev/null > "$T/effusus" 2>&1 &
    p=$!
    while kill -0 "$p" 2>/dev/null; do
        sleep 0.2; k=$((k + 1))
        if [ "$k" -ge 25 ]; then kill "$p" 2>/dev/null; wait "$p" 2>/dev/null; return 124; fi
    done
    wait "$p"
}

# I. sedes
mkdir -p "$T/i"
o=$(RHUBARB_ANNALES="$T/i" "$NF" -sedes); rc=$?
[ "$rc" -eq 0 ] && echo "$o" | grep -q "^origo	ambitus$" \
    && echo "$o" | grep -q "^annales	$T/i/tabularium.jsonl$"; credo $? "I RHUBARB_ANNALES vincit (origo ambitus, annales in sede)"
RHUBARB_ANNALES="$T/nusquam" "$NF" -res x >/dev/null 2>&1; rc=$?
[ "$rc" -eq 1 ] && [ ! -e "$T/nusquam" ]; credo $? "I directorium absens: exitus 1, nihil creatum (erat $rc)"

# II. custos
mkdir -p "$T/vacua"
e=$(RHUBARB_ANNALES="$T/vacua" "$NF" -res x 2>&1); rc=$?
[ "$rc" -eq 1 ] && echo "$e" | grep -q 'annales absentes' \
    && [ -z "$(ls -A "$T/vacua")" ]; credo $? "II sedes vacua: recusata, nihil creatum (erat $rc)"
mkdir -p "$T/sine_scrinio"; : > "$T/sine_scrinio/tabularium.jsonl"
e=$(RHUBARB_ANNALES="$T/sine_scrinio" "$NF" -crea nota x 2>&1); rc=$?
[ "$rc" -eq 1 ] && echo "$e" | grep -q 'annales sine scrinio' \
    && [ ! -e "$T/sine_scrinio/tabularium.db" ]; credo $? "II annales sine scrinio: recusata, restitutio nominata"
mkdir -p "$T/sine_annalibus"; : > "$T/sine_annalibus/tabularium.db"
e=$(RHUBARB_ANNALES="$T/sine_annalibus" "$NF" -crea nota x 2>&1); rc=$?
[ "$rc" -eq 1 ] && echo "$e" | grep -q 'scrinium sine annalibus' \
    && [ ! -e "$T/sine_annalibus/tabularium.jsonl" ]; credo $? "II scrinium sine annalibus: recusata, annales non creati"

# III. genesis expressa
mkdir -p "$T/s"
RHUBARB_ANNALES="$T/s" "$NF" -genesis >/dev/null 2>&1; rc=$?
[ "$rc" -eq 0 ] && [ -f "$T/s/tabularium.jsonl" ] && [ -f "$T/s/tabularium.db" ]; credo $? "III genesis: annales + scrinium (erat $rc)"
RHUBARB_ANNALES="$T/s" "$NF" -genesis >/dev/null 2>&1; rc=$?
[ "$rc" -eq 1 ]; credo $? "III genesis iterata recusata (erat $rc)"

# IV. scriptores SIMUL: II processus, N notae singuli
scribere () {   # $1 = praefixum
    local k f=0
    for k in $(seq 1 "$N"); do
        RHUBARB_ANNALES="$T/s" "$NF" -crea nota "fumus $1 $k" "corpus $1 $k" >/dev/null 2>&1 || f=$((f + 1))
    done
    echo "$f" > "$T/fracta_$1"
}
scribere A & pa=$!
scribere B & pb=$!
wait "$pa" "$pb"
fa=$(cat "$T/fracta_A"); fb=$(cat "$T/fracta_B")
[ "$fa" -eq 0 ] && [ "$fb" -eq 0 ]; credo $? "IV scripturae omnes successerunt (fractae A=$fa B=$fb)"
lineae=$(wc -l < "$T/s/tabularium.jsonl" | tr -d ' ')
acta=$(sqlite3 "$T/s/tabularium.db" 'select count(*) from tessellae')
ultima=$(sqlite3 "$T/s/tabularium.db" 'select max(seq) from tessellae')
[ "$lineae" -eq "$acta" ] && [ "$acta" -ge $((2 * N)) ]; credo $? "IV annales == acta ($lineae lineae, $acta acta, >= $((2 * N)))"
seqs="$(grep -o '"seq":[0-9]*' "$T/s/tabularium.jsonl" | cut -d: -f2)"
ordo=$(printf '%s\n' "$seqs" | awk 'NR > 1 && $1 != prior + 1 { f++ } { prior = $1 } END { print f + 0 }')
[ "$ordo" -eq 0 ] && [ "$(printf '%s\n' "$seqs" | tail -1)" = "$ultima" ]; credo $? "IV seq densa ordine in annalibus, ultima == scrinii ($ultima; saltus $ordo)"
duplices=$(printf '%s\n' "$seqs" | sort | uniq -d | wc -l | tr -d ' ')
[ "$duplices" -eq 0 ]; credo $? "IV seq unica ($duplices duplices)"
na=$(grep -c '"fumus A ' "$T/s/tabularium.jsonl"); nb=$(grep -c '"fumus B ' "$T/s/tabularium.jsonl")
[ "$na" -eq "$N" ] && [ "$nb" -eq "$N" ]; credo $? "IV utriusque scriptoris omnes notae (A=$na B=$nb)"

# V. restitutio ex annalibus solis
mkdir -p "$T/r"; cp "$T/s/tabularium.jsonl" "$T/r/"
RHUBARB_ANNALES="$T/r" "$NF" -restituere >/dev/null 2>&1; rc=$?
ra=$(sqlite3 "$T/r/tabularium.db" 'select count(*), max(seq) from tessellae' 2>/dev/null)
[ "$rc" -eq 0 ] && [ "$ra" = "$acta|$ultima" ]; credo $? "V restitutum == scriptum ($ra vs $acta|$ultima)"
RHUBARB_ANNALES="$T/r" "$NF" -restituere >/dev/null 2>&1; rc=$?
[ "$rc" -eq 1 ]; credo $? "V restitutio super scrinium exstans recusata (erat $rc)"

# VI. residens MCP et daemon in sede mala
RHUBARB_ANNALES="$T/sine_scrinio" tecto "$TB" -mcp -radix "$RADIX"; rc=$?
[ "$rc" -eq 1 ] && grep -q 'RECUSATUM' "$T/effusus"; credo $? "VI residens MCP recusat (erat $rc; 124 = non recusavit, currebat)"
mkdir -p "$T/forum_vacuum"
RHUBARB_ANNALES="$T/forum_vacuum" tecto "$TD" -portus 0 -radix "$RADIX"; rc=$?
[ "$rc" -eq 1 ] && grep -q 'RECUSATUM' "$T/effusus" \
    && [ -z "$(ls -A "$T/forum_vacuum")" ]; credo $? "VI daemon fori recusat, nihil creatum (erat $rc; 124 = non recusavit, currebat)"

# VII. arbor recens (legatum): annales commissi, scrinium nullum
mkdir -p "$T/arbor/gesta/annales" "$T/domus"
head -5 gesta/annales/tabularium.jsonl > "$T/arbor/gesta/annales/tabularium.jsonl"
tecto env -u RHUBARB_ANNALES HOME="$T/domus" "$TB" -mcp -radix "$T/arbor"; rc=$?
[ "$rc" -eq 1 ] && grep -q 'annales sine scrinio' "$T/effusus" \
    && [ ! -e "$T/arbor/tabularium.db" ] \
    && [ "$(wc -l < "$T/arbor/gesta/annales/tabularium.jsonl" | tr -d ' ')" -eq 5 ]; credo $? "VII arbor recens: residens recusat, annales intacti, scrinium non creatum (erat $rc)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus annalium: sanum"; exit 0; fi
echo "fumus annalium: FRACTUM ($fracta)"; exit 1
