#!/bin/bash
# natura_quaere.sh - VECTIS bibliothecae: terminum invenire, et
# apparatum eius in QUAESTIONES vertere.
#
#   ./tools/natura_quaere.sh <terminus>
#       quae genera huic rei conveniant (nomina, definitiones,
#       differentiae, species, umbrae)
#
#   ./tools/natura_quaere.sh -a [modulus.]genus
#       APPARATUS: quid genus applicatum te ROGARE faciat -
#       proprietates, machinas, relationes, partes, species,
#       cum hereditate per sub= (etiam trans exemplaria).
#
# CUR HOC INSTRUMENTUM: genus titulus non est sed apparatum
# fert (machinam, proprietates, relationes). Ideo genus
# applicare quaestiones proximas TRADIT - quod peritia
# extrinsecus visa est. Hoc instrumentum eas explicat.
#
# INDEPENDENS CONSULTO: natura_visus.sh PORTA est (regulae XVII)
# et labefactari non debet; hoc legit tantum et consulit. Si
# quando dissonent, PORTA VINCIT.

set -u
cd "$(dirname "$0")/.." || exit 1
TMP=build/natura_quaere_tmp; mkdir -p "$TMP"
xp() { xmllint --xpath "$2" "$1" 2>/dev/null; }
num() { printf '%.0f' "${1:-0}" 2>/dev/null || printf '0'; }

CORPUS="$TMP/corpus.txt"
: > "$CORPUS"

for f in natura/*.genera; do
    mod=$(xp "$f" 'string(/natura/@modulus)')
    ng=$(num "$(xp "$f" 'count(//genus)')")
    i=1
    while [ "$i" -le "$ng" ]; do
        sel="(//genus)[$i]"
        g=$(xp "$f" "string($sel/@nomen)")
        sb=$(xp "$f" "string($sel/@sub)")
        sm=$(xp "$f" "string($sel/@modulus)")
        [ -z "$sm" ] && sm="$mod"
        [ -z "$sb" ] && sm=""
        de=$(xp "$f" "string($sel/definitio)" | tr '\n' ' ' | sed 's/  */ /g;s/^ //')
        di=$(xp "$f" "string($sel/differentia)" | tr '\n' ' ' | sed 's/  */ /g;s/^ //')
        printf 'G\t%s\t%s\t%s\t%s\t%s\t%s\n' "$mod" "$g" "$sm" "$sb" "$de" "$di" >> "$CORPUS"

        # CAVE: machina_statuum filius DIRECTUS generis est;
        # proprietas/pars/relatio in involucris habitant
        for k in proprietas machina_statuum pars relatio; do
            case "$k" in
              proprietas) base="$sel/proprietates/proprietas" ;;
              machina_statuum) base="$sel/machina_statuum" ;;
              pars) base="$sel/partes/pars" ;;
              relatio) base="$sel/relationes/relatio" ;;
            esac
            nk=$(num "$(xp "$f" "count($base)")")
            j=1
            while [ "$j" -le "$nk" ]; do
                s2="$base[$j]"
                n=$(xp "$f" "string($s2/@nomen)")
                case "$k" in
                  proprietas)
                    ty=$(xp "$f" "string($s2/@genus)")
                    tm=$(xp "$f" "string($s2/@modulus)")
                    op=$(xp "$f" "string($s2)" | tr '\n' ' ' | sed 's/  */ /g;s/^ //;s/ $//')
                    printf 'P\t%s\t%s\t%s\t%s\t%s\t%s\n' "$mod" "$g" "$n" "$ty" "$tm" "$op" >> "$CORPUS" ;;
                  machina_statuum)
                    # CAVE: xmllint nodos attributorum LINEIS
                    # separat - iungendum ANTE quam in TSV eat,
                    # aliter campus lineas plures occupat et
                    # tabulam frangit
                    st=$(xp "$f" "$s2/status/@nomen" | tr '\n' ' ' \
                         | sed 's/ *nomen="/, /g;s/"//g;s/^, //;s/ *$//')
                    printf 'M\t%s\t%s\t%s\t%s\n' "$mod" "$g" "$n" "$st" >> "$CORPUS" ;;
                  pars)
                    ne=$(xp "$f" "string($s2/@necessaria)")
                    printf 'A\t%s\t%s\t%s\t%s\n' "$mod" "$g" "$n" "$ne" >> "$CORPUS" ;;
                  relatio|relatum)
                    ad=$(xp "$f" "string($s2/@ad)")
                    rm=$(xp "$f" "string($s2/@modulus)")
                    [ -z "$rm" ] && rm="$mod"
                    [ -n "$n" ] && printf 'R\t%s\t%s\t%s\t%s\t%s\n' "$mod" "$g" "$n" "$rm" "$ad" >> "$CORPUS" ;;
                esac
                j=$((j + 1))
            done
        done
        i=$((i + 1))
    done

    for gr in species individuum; do
        nr=$(num "$(xp "$f" "count(//$gr)")")
        i=1
        while [ "$i" -le "$nr" ]; do
            r=$(xp "$f" "string((//$gr)[$i]/@nomen)")
            rg=$(xp "$f" "string((//$gr)[$i]/ancestor::genus[1]/@nomen)")
            rd=$(xp "$f" "string((//$gr)[$i]/definitio)" | tr '\n' ' ' | sed 's/  */ /g;s/^ //')
            [ -n "$r" ] && printf 'S\t%s\t%s\t%s\t%s\t%s\n' "$mod" "$rg" "$gr" "$r" "$rd" >> "$CORPUS"
            i=$((i + 1))
        done
    done

    nu=$(num "$(xp "$f" "count(//*[@externum='verum'])")")
    i=1
    while [ "$i" -le "$nu" ]; do
        u=$(xp "$f" "string((//*[@externum='verum'])[$i]/@ad)")
        [ -n "$u" ] && printf 'U\t%s\t%s\n' "$mod" "$u" >> "$CORPUS"
        i=$((i + 1))
    done
done

# ---- maiores: catena sub= (etiam trans exemplaria) ----
maiores() {   # mod genus -> lineas "mod\tgenus" a se ad radicem
    local m="$1" g="$2" n=0 line pm pg
    while [ -n "$g" ] && [ "$n" -lt 12 ]; do
        printf '%s\t%s\n' "$m" "$g"
        line=$(awk -F'\t' -v m="$m" -v g="$g" \
              '$1=="G"&&$2==m&&$3==g{print $4"\t"$5; exit}' "$CORPUS")
        pm=$(printf '%s' "$line" | cut -f1); pg=$(printf '%s' "$line" | cut -f2)
        [ -z "$pg" ] && break
        m="$pm"; g="$pg"; n=$((n + 1))
    done
}

# ================= MODUS APPARATUS =================
if [ "${1:-}" = "-a" ]; then
    [ $# -lt 2 ] && { echo "usus: $0 -a [modulus.]genus"; exit 2; }
    q="$2"; qm=""; qg="$q"
    case "$q" in *.*) qm="${q%%.*}"; qg="${q#*.}";; esac
    hit=$(awk -F'\t' -v m="$qm" -v g="$qg" \
          '$1=="G"&&$3==g&&(m==""||$2==m){print $2"\t"$3; exit}' "$CORPUS")
    [ -z "$hit" ] && { echo "genus '$q' non inventum. Quaere: $0 $qg"; exit 1; }
    m=$(printf '%s' "$hit" | cut -f1); g=$(printf '%s' "$hit" | cut -f2)

    echo "=================================================="
    echo "  /$m/$g"
    echo "=================================================="
    awk -F'\t' -v m="$m" -v g="$g" '$1=="G"&&$2==m&&$3==g{
        if($6!="") print "\n" $6; if($7!="") print "\nDIFFERENTIA: " $7 }' "$CORPUS"

    CHAIN="$TMP/chain.txt"; maiores "$m" "$g" > "$CHAIN"
    if [ "$(wc -l < "$CHAIN")" -gt 1 ]; then
        printf '\nIN ARBORE: '
        { tac "$CHAIN" 2>/dev/null || tail -r "$CHAIN"; } \
          | awk -F'\t' '{printf "%s%s.%s", (NR>1?" > ":""), $1, $2} END{print ""}'
    fi

    echo
    echo "--- QUAE ROGARE DEBEAS (apparatus, cum hereditate) ---"
    while IFS=$'\t' read -r cm cg; do
        pfx=""; [ "$cm/$cg" != "$m/$g" ] && pfx="  [a $cm.$cg] "
        awk -F'\t' -v m="$cm" -v g="$cg" -v p="$pfx" '$1=="P"&&$2==m&&$3==g{
            o=$7; gsub(/^ +| +$/,"",o)
            printf "  ? %s%s", p, $4
            if($5=="electio"&&o!="") printf "  (%s)", o
            else if($5!="") printf "  [%s%s]", ($6!=""? $6 ".":""), $5
            printf "\n" }' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v p="$pfx" '$1=="M"&&$2==m&&$3==g{
            printf "  ? %squo statu (%s): %s\n", p, $4, $5 }' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v p="$pfx" '$1=="A"&&$2==m&&$3==g{
            printf "  ? %sadestne pars '\''%s'\''%s\n", p, $4,
                   ($5=="verum" ? "  [NECESSARIA]" : "  (optio)") }' "$CORPUS"
        awk -F'\t' -v m="$cm" -v g="$cg" -v p="$pfx" '$1=="R"&&$2==m&&$3==g{
            printf "  ? %s%s -> %s\n", p, $4, ($6=="*" ? "(quidlibet)" : $5 "." $6) }' "$CORPUS"
    done < "$CHAIN"

    nsp=$(awk -F'\t' -v m="$m" -v g="$g" '$1=="S"&&$2==m&&$3==g' "$CORPUS" | wc -l)
    if [ "$nsp" -gt 0 ]; then
        echo
        echo "--- QUAE SPECIES SIT? ---"
        awk -F'\t' -v m="$m" -v g="$g" '$1=="S"&&$2==m&&$3==g{
            mark = ($4=="individuum" ? ":" : "")
            d=$6; if(length(d)>92) d=substr(d,1,92) "..."
            printf "  %s%-26s %s\n", mark, $5, d }' "$CORPUS"
    fi
    exit 0
fi

# ================= MODUS QUAERENDI =================
[ $# -lt 1 ] && { echo "usus: $0 <terminus> | $0 -a [modulus.]genus"; exit 2; }
Q="$1"
echo "quaesitum: '$Q'"
echo

awk -F'\t' -v q="$(printf '%s' "$Q" | tr 'A-Z' 'a-z')" '
function low(s){return tolower(s)}
$1=="G" {
  sc=0; where=""
  if(index(low($3),q)) { sc+=6; where="NOMEN" }
  if(index(low($6),q)) { sc+=3; where=(where?where"+":"")"definitio" }
  if(index(low($7),q)) { sc+=3; where=(where?where"+":"")"differentia" }
  if(sc>0){ key=$2"\t"$3; score[key]=sc; wh[key]=where
            d=($6!=""?$6:$7); if(length(d)>96) d=substr(d,1,96)"..."; gl[key]=d }
}
$1=="S" {
  if(index(low($5),q)||index(low($6),q)){
    key=$2"\t"$3; score[key]+=4
    wh[key]=(wh[key]?wh[key]"+":"")"species:"$5
    if(!($2"\t"$3 in gl)) gl[key]="(vide genus)"
  }
}
END{
  n=0
  for(k in score){ printf "%d\t%s\t%s\t%s\n", score[k], k, wh[k], gl[k]; n++ }
  if(n==0) print "0\t\t\t\t"
}' "$CORPUS" | sort -rn | while IFS=$'\t' read -r sc mod gen wh gl; do
    [ "$sc" = "0" ] && { echo "  (nullum genus congruit)"; break; }
    printf '  /%s/%s\n      %s\n      [%s]\n\n' "$mod" "$gen" "$gl" "$wh"
done

echo "--- UMBRAE congruentes (superfecta, nondum descripta) ---"
found=$(awk -F'\t' -v q="$(printf '%s' "$Q" | tr 'A-Z' 'a-z')" \
  '$1=="U" && index(tolower($3),q){print "  " $3 "  (a " $2 " superfectum)"}' \
  "$CORPUS" | sort -u)
[ -n "$found" ] && echo "$found" || echo "  (nullae)"
echo
echo "apparatum vide: $0 -a <modulus.genus>"
