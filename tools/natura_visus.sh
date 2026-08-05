#!/bin/bash
# natura_visus.sh - proiectio visualis bibliothecae naturae
#
# Legit natura/*.stml, emittit build/natura_visus.html:
#   - silva Porphyriana (genera + sub-genera, numeri specierum)
#   - arcus trans exemplaria (modulus= relationes)
#   - UMBRAE: genera superficta (externum=) - agenda COMPUTATA
#   - VULNERA: citationes pendentes - semen validatoris
#
# Effectus PROIECTIO est (licentia delendi): deleri licet,
# regenerari potest. Instrumentum semen validatoris (LOADER=
# VALIDATOR) est - visus amicus, probatio severa, radix una.

set -u
cd "$(dirname "$0")/.." || exit 1

EXITUS=build/natura_visus.html
TMP=build/natura_visus_tmp
mkdir -p "$TMP"

GENERA="$TMP/genera.txt"     # mod|genus|sub|nSp|nIn|nDu|nMa
RES="$TMP/res.txt"           # mod|nomen (species+individua+cultivar)
ARCUS="$TMP/arcus.txt"       # fromMod|fromGenus|rel|toMod|target|genus
UMBRAE="$TMP/umbrae.txt"     # target|fromMod
DUBIA="$TMP/dubia.txt"       # mod|contextus
FONTES="$TMP/fontes.txt"     # mod|clavis
CITATIONES="$TMP/citationes.txt"  # mod|clavis|nomen
ASSENSUS="$TMP/assensus.txt"      # mod|gradus
TRANSRADICES="$TMP/transradices.txt"  # mod|genus|mod.parens
VALIDITAS="$TMP/validitas.txt"        # mod|nomen|a|ad
: > "$TRANSRADICES"; : > "$VALIDITAS"
: > "$GENERA"; : > "$RES"; : > "$ARCUS"; : > "$UMBRAE"; : > "$DUBIA"
: > "$FONTES"; : > "$CITATIONES"; : > "$ASSENSUS"

xp() { xmllint --xpath "$2" "$1" 2>/dev/null; }
num() { printf '%.0f' "${1:-0}" 2>/dev/null || printf '0'; }

MODULI=""

for f in natura/*.stml; do
    mod=$(xp "$f" 'string(/natura/@modulus)')
    ver=$(xp "$f" 'string(/natura/@versio)')
    MODULI="$MODULI $mod:$ver"

    n=$(num "$(xp "$f" 'count(//genus)')")
    i=1
    while [ "$i" -le "$n" ]; do
        g=$(xp "$f" "string((//genus)[$i]/@nomen)")
        s=$(xp "$f" "string((//genus)[$i]/@sub)")
        sm=$(xp "$f" "string((//genus)[$i]/@modulus)")
        # subordinatio TRANS EXEMPLARIA (mechanismus rotae XVII):
        # sub= cum modulo alieno = arcus, et radix in silva sua
        if [ -n "$s" ] && [ -n "$sm" ] && [ "$sm" != "$mod" ]; then
            echo "$mod|$g|sub|$sm|$s" >> "$ARCUS"
            echo "$mod|$g|$sm.$s" >> "$TRANSRADICES"
            s=""
        fi
        nsp=$(num "$(xp "$f" "count((//genus)[$i]/species)")")
        nin=$(num "$(xp "$f" "count((//genus)[$i]/individuum)")")
        ndu=$(num "$(xp "$f" "count((//genus)[$i]//dubium)")")
        nma=$(num "$(xp "$f" "count((//genus)[$i]/machina_statuum)")")
        echo "$mod|$g|$s|$nsp|$nin|$ndu|$nma" >> "$GENERA"
        i=$((i + 1))
    done

    # res dictionarii (species/individua/cultivares nominati)
    for gradus in species individuum cultivar; do
        nr=$(num "$(xp "$f" "count(//$gradus)")")
        i=1
        while [ "$i" -le "$nr" ]; do
            r=$(xp "$f" "string((//$gradus)[$i]/@nomen)")
            [ -n "$r" ] && echo "$mod|$r" >> "$RES"
            i=$((i + 1))
        done
    done

    # arcus: relatio/relatum cum ad= aut a= (praeter aperta/externa)
    for elem in relatio relatum; do
        for attr in ad a; do
            sel="//${elem}[@${attr}][not(@externum)][@${attr}!='*']"
            ne=$(num "$(xp "$f" "count($sel)")")
            i=1
            while [ "$i" -le "$ne" ]; do
                rel=$(xp "$f" "string(($sel)[$i]/@nomen)")
                tgt=$(xp "$f" "string(($sel)[$i]/@${attr})")
                tm=$(xp "$f" "string(($sel)[$i]/@modulus)")
                fg=$(xp "$f" "string(($sel)[$i]/ancestor::genus/@nomen)")
                [ -z "$tm" ] && tm="$mod"
                echo "$mod|$fg|$rel|$tm|$tgt" >> "$ARCUS"
                i=$((i + 1))
            done
        done
    done

    # proprietates generibus bibliothecae typatae (mechanismus
    # novus: genus= + modulus= pro paletta nuda) - arcus quoque
    np=$(num "$(xp "$f" "count(//proprietas[@modulus])")")
    i=1
    while [ "$i" -le "$np" ]; do
        pn=$(xp "$f" "string((//proprietas[@modulus])[$i]/@nomen)")
        pg=$(xp "$f" "string((//proprietas[@modulus])[$i]/@genus)")
        pm=$(xp "$f" "string((//proprietas[@modulus])[$i]/@modulus)")
        pf=$(xp "$f" "string((//proprietas[@modulus])[$i]/ancestor::genus[1]/@nomen)")
        echo "$mod|$pf|proprietas:$pn|$pm|$pg" >> "$ARCUS"
        i=$((i + 1))
    done

    # fontes declarati (claves) + citationes fons= et certitudo=
    nfd=$(num "$(xp "$f" "count(//fontes/fons)")")
    i=1
    while [ "$i" -le "$nfd" ]; do
        fc=$(xp "$f" "string((//fontes/fons)[$i]/@clavis)")
        echo "$mod|$fc" >> "$FONTES"
        i=$((i + 1))
    done
    nfc=$(num "$(xp "$f" "count(//*[@fons])")")
    i=1
    while [ "$i" -le "$nfc" ]; do
        fu=$(xp "$f" "string((//*[@fons])[$i]/@fons)")
        fn=$(xp "$f" "string((//*[@fons])[$i]/@nomen)")
        echo "$mod|$fu|$fn" >> "$CITATIONES"
        i=$((i + 1))
    done
    ncert=$(num "$(xp "$f" "count(//*[@certitudo])")")
    i=1
    while [ "$i" -le "$ncert" ]; do
        cv=$(xp "$f" "string((//*[@certitudo])[$i]/@certitudo)")
        echo "$mod|$cv" >> "$ASSENSUS"
        i=$((i + 1))
    done

    # tempus validitatis (mechanismus rotae XVIII):
    # forma dierum + ordo (a <= ad) - intervallum inversum error
    nvt=$(num "$(xp "$f" "count(//*[@valens_a or @valens_ad])")")
    i=1
    while [ "$i" -le "$nvt" ]; do
        sel="(//*[@valens_a or @valens_ad])[$i]"
        va=$(xp "$f" "string($sel/@valens_a)")
        vd=$(xp "$f" "string($sel/@valens_ad)")
        vn=$(xp "$f" "string($sel/@nomen)")
        echo "$mod|$vn|$va|$vd" >> "$VALIDITAS"
        i=$((i + 1))
    done

    # umbrae: externum="verum" - superficta declarata
    nu=$(num "$(xp "$f" "count(//*[@externum='verum'])")")
    i=1
    while [ "$i" -le "$nu" ]; do
        u=$(xp "$f" "string((//*[@externum='verum'])[$i]/@ad)")
        [ -n "$u" ] && echo "$u|$mod" >> "$UMBRAE"
        i=$((i + 1))
    done

    # dubia cum contextu
    ndub=$(num "$(xp "$f" 'count(//dubium)')")
    i=1
    while [ "$i" -le "$ndub" ]; do
        ctx=$(xp "$f" "string((//dubium)[$i]/ancestor-or-self::*[@nomen][1]/@nomen)")
        [ -z "$ctx" ] && ctx="(modulus)"
        echo "$mod|$ctx" >> "$DUBIA"
        i=$((i + 1))
    done
done

# ---- validatio: arcus contra genera+res ----
VULNERA="$TMP/vulnera.txt"
: > "$VULNERA"
while IFS='|' read -r fm fg rel tm tgt; do
    if ! grep -q "^$tm|$tgt|" "$GENERA" && \
       ! grep -q "^$tm|$tgt\$" "$RES"; then
        echo "$fm.$fg --$rel--> $tm.$tgt" >> "$VULNERA"
    fi
done < "$ARCUS"

# ---- regula VIII: vocabularium clausum (METAMODULUS §3/§4) ----
# Spec et corpus se invicem custodiunt: elementum aut attributum
# novum sine emendatione METAMODULI = vulnus. Ideo documentum
# rancidum fieri non potest.
# normalizatio spatiorum OBLIGATORIA: catalogi lineas plures
# habent, et `case " $x " in *" $e "*` spatia poscit, non lineas
_elementa="natura fontes fons genus definitio differentia
proprietates proprietas optio partes pars machina_statuum status
transitus actiones actio relationes relatio species individuum
cultivar valor relatum historia eventum nota dubium"
_attributa="nomen modulus versio lingua sub gradus etiam genus
ad a per multiplex ordinarius necessaria externum inversa gerens
quando actio clavis verificatus certitudo fons valens_a valens_ad
nota version encoding"
# expansio NON QUOTATA verba findit (lineas quoque) et echo ea
# spatiis singulis reiungit - aliter linea nova mandatum novum
# intra $(...) esset
ELEMENTA_NOTA=" $(echo $_elementa) "
ATTRIBUTA_NOTA=" $(echo $_attributa) "

# CAVE: vocabularium ex TITULIS solis legendum, non ex textu
# crudo - prosa (dubium) syntaxin attributi continere potest
# ('puritas="verum"' ut exemplum disputatum). Grep textum crudum
# legens parser non est.
for f in natura/*.stml; do
    mod=$(xp "$f" 'string(/natura/@modulus)')
    perl -0ne 's/<!--.*?-->//gs;
               while (/<([a-z_]+)((?:"[^"]*"|[^<>])*)>/g) {
                 my ($e,$at)=($1,$2); print "E $e\n";
                 while ($at =~ /([a-z_]+)\s*=\s*"[^"]*"/g) {
                   print "A $1\n" } }' "$f" | sort -u | \
    while read -r genusnotae vox; do
        if [ "$genusnotae" = "E" ]; then
            case "$ELEMENTA_NOTA" in
                *" $vox "*) ;;
                *) echo "$mod --elementum--> <$vox> (extra METAMODULUM §3)" \
                       >> "$VULNERA" ;;
            esac
        else
            case "$ATTRIBUTA_NOTA" in
                *" $vox "*) ;;
                *) echo "$mod --attributum--> $vox= (extra METAMODULUM §4)" \
                       >> "$VULNERA" ;;
            esac
        fi
    done
done

# ---- validatio fidei: fons= solvatur, certitudo= vera sit ----
while IFS='|' read -r m clavis nomen; do
    if ! grep -q "^$m|$clavis\$" "$FONTES"; then
        echo "$m.$nomen --fons--> '$clavis' (clavis non declarata)" \
            >> "$VULNERA"
    fi
done < "$CITATIONES"
while IFS='|' read -r m gradus; do
    if ! grep -q "^iudicium|$gradus\$" "$RES"; then
        echo "$m --certitudo--> '$gradus' (gradus assensus ignotus)" \
            >> "$VULNERA"
    fi
done < "$ASSENSUS"
while IFS='|' read -r m nomen va vd; do
    for d in "$va" "$vd"; do
        [ -z "$d" ] && continue
        case "$d" in
            [0-9][0-9][0-9][0-9]|\
            [0-9][0-9][0-9][0-9]-[0-9][0-9]|\
            [0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]) ;;
            *) echo "$m.$nomen --validitas--> '$d' (forma diei prava)" \
                   >> "$VULNERA" ;;
        esac
    done
    # intervallum inversum: finis ante initium
    if [ -n "$va" ] && [ -n "$vd" ]; then
        if [ "$(printf '%s\n%s\n' "$va" "$vd" | sort | head -1)" != "$va" ]; then
            echo "$m.$nomen --validitas--> $va..$vd (intervallum INVERSUM)" \
                >> "$VULNERA"
        fi
    fi
done < "$VALIDITAS"
nVulnera=$(wc -l < "$VULNERA" | tr -d ' ')
nValiditas=$(wc -l < "$VALIDITAS" | tr -d ' ')

# ---- numeri ----
nModuli=$(ls natura/*.stml | wc -l | tr -d ' ')
nGenera=$(wc -l < "$GENERA" | tr -d ' ')
nRes=$(wc -l < "$RES" | tr -d ' ')
nArcus=$(wc -l < "$ARCUS" | tr -d ' ')
nUmbrae=$(sort -u -t'|' -k1,1 "$UMBRAE" | wc -l | tr -d ' ')
nDubia=$(wc -l < "$DUBIA" | tr -d ' ')
nVulnera=$(wc -l < "$VULNERA" | tr -d ' ')

# ---- emissio HTML ----
{
cat <<'CAPUT'
<!DOCTYPE html>
<html lang="la"><head><meta charset="utf-8">
<title>Natura - Visus Bibliothecae</title>
<style>
 body { font-family: Menlo, monospace; margin: 2rem auto;
        max-width: 72rem; background: #14120f; color: #d8d0c0;
        line-height: 1.5; }
 h1 { color: #e8c878; font-size: 1.4rem; }
 h2 { color: #b8a878; border-bottom: 1px solid #3a352c;
      padding-bottom: .3rem; margin-top: 2rem; font-size: 1.1rem; }
 .numeri { color: #888070; margin-bottom: 1.5rem; }
 .numeri b { color: #e8c878; }
 .modulus { margin: 1rem 0; }
 .modulus > .titulus { color: #a8c8a0; font-weight: bold; }
 ul { list-style: none; padding-left: 1.4rem; margin: .2rem 0; }
 li { margin: .1rem 0; }
 .genus { color: #d8d0c0; }
 .insignia { color: #787060; font-size: .85em; }
 .machina { color: #c89078; }
 .dubium-i { color: #c8b878; }
 .arcus td { padding: .1rem .8rem .1rem 0; color: #a8a090; }
 .arcus .rel { color: #88a8c0; }
 .umbra { color: #9088a8; }
 .vulnus { color: #d88078; }
 .sanum { color: #a8c8a0; }
 table { border-collapse: collapse; }
</style></head><body>
<h1>NATURA - visus bibliothecae semanticae</h1>
CAPUT

echo "<p class='numeri'>exemplaria <b>$nModuli</b> &middot; genera <b>$nGenera</b> &middot; res dictionarii <b>$nRes</b> &middot; arcus <b>$nArcus</b> &middot; umbrae <b>$nUmbrae</b> &middot; dubia <b>$nDubia</b> &middot; vulnera <b>$nVulnera</b></p>"

# ---- silva ----
echo "<h2>SILVA PORPHYRIANA</h2>"

rami() { # mod parens
    local mod="$1" parens="$2" linea g s nsp nin ndu nma insig
    echo "<ul>"
    while IFS='|' read -r _ g s nsp nin ndu nma; do
        [ "$s" != "$parens" ] && continue
        insig=""
        [ "$nsp" -gt 0 ] && insig="$insig species:$nsp"
        [ "$nin" -gt 0 ] && insig="$insig individua:$nin"
        printf '<li><span class="genus">%s</span>' "$g"
        supra=$(grep "^$mod|$g|" "$TRANSRADICES" | cut -d'|' -f3)
        [ -n "$supra" ] && printf ' <span class="umbra">&sub; %s</span>' "$supra"
        [ -n "$insig" ] && printf ' <span class="insignia">%s</span>' "$insig"
        [ "$nma" -gt 0 ] && printf ' <span class="machina">&#9881;machina</span>'
        [ "$ndu" -gt 0 ] && printf ' <span class="dubium-i">dubia:%s</span>' "$ndu"
        rami "$mod" "$g"
        echo "</li>"
    done < <(grep "^$mod|" "$GENERA")
    echo "</ul>"
}

for mv in $MODULI; do
    mod="${mv%%:*}"; ver="${mv##*:}"
    echo "<div class='modulus'><span class='titulus'>$mod</span> <span class='insignia'>v$ver</span>"
    rami "$mod" ""
    echo "</div>"
done

# ---- arcus trans exemplaria ----
echo "<h2>ARCUS TRANS EXEMPLARIA</h2><table class='arcus'>"
while IFS='|' read -r fm fg rel tm tgt; do
    [ "$fm" = "$tm" ] && continue
    echo "<tr><td>$fm.$fg</td><td class='rel'>--$rel--&gt;</td><td>$tm.$tgt</td></tr>"
done < <(sort -u "$ARCUS")
echo "</table>"

# ---- umbrae ----
echo "<h2>UMBRAE (superficta, nondum descripta - agenda computata)</h2><ul>"
sort -t'|' -k1,1 "$UMBRAE" | sort -u -t'|' -k1,1 | \
while IFS='|' read -r u fm; do
    echo "<li class='umbra'>$u <span class='insignia'>(a $fm superfectum)</span></li>"
done
echo "</ul>"

# ---- dubia ----
echo "<h2>DUBIA APERTA</h2><ul>"
while IFS='|' read -r m c; do
    echo "<li><span class='insignia'>$m /</span> $c</li>"
done < "$DUBIA"
echo "</ul>"

# ---- vulnera ----
echo "<h2>VULNERA (citationes pendentes)</h2>"
if [ "$nVulnera" -eq 0 ]; then
    echo "<p class='sanum'>nulla - omnes citationes solvuntur</p>"
else
    echo "<ul>"
    while IFS= read -r v; do
        echo "<li class='vulnus'>$v</li>"
    done < "$VULNERA"
    echo "</ul>"
fi

echo "<p class='insignia'>proiectio generata a tools/natura_visus.sh - deleri licet, regenerari potest</p>"
echo "</body></html>"
} > "$EXITUS"

# ---- relatio terminalis ----
echo "natura_visus: $EXITUS scriptum"
echo "  exemplaria $nModuli / genera $nGenera / res $nRes / arcus $nArcus"
echo "  umbrae $nUmbrae / dubia $nDubia / tempus-validitatis $nValiditas / VULNERA $nVulnera"
if [ "$nVulnera" -gt 0 ]; then
    echo "  --- vulnera: ---"
    cat "$VULNERA" | sed 's/^/  /'
    exit 1
fi
exit 0
