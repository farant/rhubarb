#!/bin/bash
# natura_visus.sh - proiectio visualis bibliothecae naturae
#
# Legit natura/*.genera, emittit build/natura_visus.html:
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
GLOSSAE="$TMP/glossae.txt"            # mod|genus|glossa
RESGEN="$TMP/resgen.txt"              # mod|genus|res
: > "$TRANSRADICES"; : > "$VALIDITAS"; : > "$GLOSSAE"; : > "$RESGEN"
: > "$GENERA"; : > "$RES"; : > "$ARCUS"; : > "$UMBRAE"; : > "$DUBIA"
: > "$FONTES"; : > "$CITATIONES"; : > "$ASSENSUS"

num() { printf '%.0f' "${1:-0}" 2>/dev/null || printf '0'; }

# ================= PORTA PARSATIONIS (gradus 0) =================
# CUR: plagula quae parsari nequit hoc instrumentum TACITE
# corrumpit. xp() errores in /dev/null mittit, ergo plagula fracta
# chordas vacuas ubique reddit: genera eius evanescunt, VULNERA
# nulla nuntiantur, exitus ZEPHYRUM est - et 'exemplaria' eam tamen
# NUMERAT, unde processa videtur. MENSURATUM 2026-08-06: plagula
# vere malformata per visum transiit cum 'VULNERA 0'.
#
# Iudicium NON duplicatur: auctoritas contractus est
# bin/natura_examen (onerator ipse), quem visus ROGAT. Una res, una
# sedes. Sola regula I hic obstat - cetera vulnera analysin visus
# non corrumpunt, ergo eum non morantur.

PORTA=bin/natura_examen

if [ ! -x "$PORTA" ]; then
    echo "natura_visus: $PORTA ABEST - porta parsationis TACET." >&2
    echo "  Strue: ./tools/natura_struere.sh" >&2
    exit 2
fi

# porta STALA idem mentitur ac porta absens (lectio legati)
for _f in lib/natura.c include/natura.h tools/natura_examen.c; do
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_visus: $PORTA STALUS est ($_f recentior)." >&2
        echo "  Strue: ./tools/natura_struere.sh" >&2
        exit 2
    fi
done

_fracta=$("$PORTA" -machina 2>/dev/null | awk -F'\t' '$1=="VULNUS" && $2==1')
if [ -n "$_fracta" ]; then
    echo "natura_visus: PARSATIO FRACTA - analysis fieri NEQUIT" >&2
    printf '%s\n' "$_fracta" | sed 's/^/  /' >&2
    exit 1
fi

# GUARD TRANSITORIUS DELETUS (gradus II, 2026-08-06): xmllint
# hinc abiit, ergo plagula STML-valida-sed-XML-invalida hoc
# instrumentum frangere non potest amplius. Contractus (STML) et
# instrumentum tandem CONSENTIUNT, unde guard nihil custodiebat.

# ================= EXTRACTIO (gradus II) =================
# XLVIII vocationes xmllint hic stabant. Erant PARSATIO ALTERA
# eiusdem corporis cum interpretatione altera: onerator plagulas
# per stml legebat, visus easdem per xmllint - duo parseres, unum
# corpus, dissensus semper possibilis (et TACITUS, quia xp()
# errores in /dev/null mittebat).
#
# Nunc onerator, qui haec omnia iam in memoria tenet, tabulas
# EMITTIT; visus solam praesentationem agit. Una parsatio, una
# veritas. Schemata tabularum immutata manent, ergo quod sequitur
# intactum est.
#
# DIFFERENTIA MENSURATA in migratione: stml_textus_internus
# spatium album PRAECIDIT, xmllint string() non - unde campus
# unus (dubium functio_pura) spatio finali caret quod xmllint
# ferebat. Melius, non peius: spatium finale strepitus erat.

if ! "$PORTA" -tabulae "$TMP" >/dev/null 2>&1; then
    echo "natura_visus: tabulae scribi nequeunt (porta $PORTA)" >&2
    exit 1
fi

MODULI=$(tr '\n' ' ' < "$TMP/moduli.txt")

# ---- classificatio umbrarum: DUAE agendae, non una ----
# Umbra a relatione citata GENUS poscit: opus consilii, quaestio
# ontologica, iudicium Franis. Umbra a relato SOLO citata
# INDIVIDUUM poscit sub genere iam exsistente: opus dictionarii,
# nullum consilium. Utrumque sub uno titulo ponere agendam facit
# quae difficilior videtur quam est - et laborem facilem, quem
# quivis quovis momento capere potest, invisibilem reddit.
# EURISTICA, non lex: relatum genus attingere POTEST. Nomen
# 'coniectata' hoc fatetur.
UMBCLS="$TMP/umbrae_cls.txt"   # target|classis|nCit|moduli
: > "$UMBCLS"
for u in $(cut -d'|' -f1 "$UMBRAE" | sort -u); do
    lineae=$(grep "^$u|" "$UMBRAE")
    ncit=$(echo "$lineae" | wc -l | tr -d ' ')
    mods=$(echo "$lineae" | cut -d'|' -f2 | sort -u | paste -sd, - \
           | sed 's/,/, /g')
    # relatio VINCIT: si utroque modo citatur, genus prius est,
    # quia individuum sub genere nondum nato poni non potest
    if echo "$lineae" | cut -d'|' -f3 | grep -q '^relatio$'; then
        cls="genus"
    else
        cls="individuum"
    fi
    echo "$u|$cls|$ncit|$mods" >> "$UMBCLS"
done
nUmbGen=$(grep -c '|genus|' "$UMBCLS" || true)
nUmbInd=$(grep -c '|individuum|' "$UMBCLS" || true)

# ---- validatio: arcus contra genera+res ----
VULNERA="$TMP/vulnera.txt"
: > "$VULNERA"
while IFS='|' read -r fm fg rel tm tgt fr; do
    if ! grep -q "^$tm|$tgt|" "$GENERA" && \
       ! grep -q "^$tm|$tgt\$" "$RES"; then
        # fons precisus in nuntio: diagnostica se nominantia
        echo "$fm.${fr:-$fg} --$rel--> $tm.$tgt" >> "$VULNERA"
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
for f in natura/*.genera; do
    # modulus EX NOMINE PLAGULAE: regula XIV (modulus= stirpi
    # aequatur) ab oneratore nunc COGITUR, ergo nomen plagulae
    # fide dignum est et parsatio altera non opus
    mod=$(basename "$f" .genera)
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

# ---- regula XV: nomina intra modulum UNICA (METAMODULUS §4b)
# Allocutio /modulus/nomen ab hac unicitate PENDET - ergo non
# munditia sed fundamentum. Genera et res unum spatium nominum
# communicant, quia allocutio gradum non fert.
sort "$GENERA" | cut -d'|' -f1,2 > "$TMP/nomina.txt"
cut -d'|' -f1,2 "$RES" >> "$TMP/nomina.txt"
sort "$TMP/nomina.txt" | uniq -d | while IFS='|' read -r m n; do
    echo "$m.$n --nomen--> bis definitum (allocutio ambigua)" \
        >> "$VULNERA"
done

# ---- regula XVI: UMBRAE RANCIDAE (externum sed iam descriptum)
# Agenda computata est: umbra rancida agendam MENTIENTEM facit,
# quod peius est quam agendam nullam. Quater manu capta.
while IFS='|' read -r u cls ncit mods; do
    if grep -q "|$u|" "$GENERA" || grep -q "|$u\$" "$RES"; then
        echo "$mods --umbra--> '$u' RANCIDA (iam descriptum)" \
            >> "$VULNERA"
    fi
done < "$UMBCLS"

# ---- regula XVII: versio attributi cum capite congruat ----
for f in natura/*.genera; do
    m=$(basename "$f" .genera)
    va=$(grep "^$m:" "$TMP/moduli.txt" | cut -d: -f2)
    vc=$(grep -o 'versio [0-9]*, PLASTICUM' "$f" | head -1 \
         | sed 's/versio //; s/, PLASTICUM//')
    if [ -n "$vc" ] && [ "$va" != "$vc" ]; then
        echo "$m --versio--> elementum v$va, caput v$vc (dissona)" \
            >> "$VULNERA"
    fi
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
nElementa=$(echo $_elementa | wc -w | tr -d " ")

# ---- MONITA: nomina cognata sine cognatione (ADVISORIUM) ----
# LEX PROPOSITA: stirps communis cognationem SIGNIFICAT. Si 'x_y'
# et 'x' ambo nomina sunt, sed 'x_y' sub 'x' non stat, aut nomen
# fallit aut arbor arcum amisit. NEUTRUM vulnus est - unde
# MONITUM, quod portam non frangit et exitum non mutat.
# Hoc porta 'monitum' quam quaerebamus (collisio nominum propinquorum)
# in forma quae MENSURARI potest antequam severa fiat.
MONITA="$TMP/monita.txt"
PARENS="$TMP/parens.txt"       # nomen|parens
NOMINA="$TMP/nomina_omnia.txt"
NEXUS="$TMP/nexus_nom.txt"     # fons|meta (nomina nuda, utroque ordine)
: > "$MONITA"; : > "$PARENS"
# nexus nominales ex arcubus: fons PRECISUS (campus VI) ad metam
cut -d'|' -f5,6 "$ARCUS" | awk -F'|' 'NF==2 && $1 && $2 {print $2"|"$1}' \
    | sort -u > "$NEXUS"
while IFS='|' read -r m g s _ _ _ _ _; do
    # radix transmodularis parens quoque est (sub= cum modulo alieno)
    [ -z "$s" ] && s=$(grep "^$m|$g|" "$TRANSRADICES" | cut -d'|' -f3 \
                       | sed 's/^.*\.//')
    echo "$g|$s" >> "$PARENS"
done < "$GENERA"
while IFS='|' read -r m g r; do
    echo "${r#:}|$g" >> "$PARENS"
done < "$RESGEN"

maior() {  # maior <stirps> <nomen> -> 0 si stirps maiorem nominis est
    local quaesitum="$1" cur="$2" i=0
    while [ -n "$cur" ] && [ "$i" -lt 12 ]; do
        cur=$(grep -m1 "^$cur|" "$PARENS" | cut -d'|' -f2)
        [ "$cur" = "$quaesitum" ] && return 0
        i=$((i + 1))
    done
    return 1
}

{ cut -d'|' -f2 "$GENERA"; cut -d'|' -f3 "$RESGEN" | sed 's/^://'; } \
    | sort -u > "$NOMINA"
while read -r b; do
    case "$b" in *_*) ;; *) continue ;; esac
    stirps="${b%_*}"
    # stirps LONGISSIMA quae nomen est: 'res_sacra_mobilis' contra
    # 'res_sacra', non contra 'res'
    while [ -n "$stirps" ]; do
        if grep -qx "$stirps" "$NOMINA"; then
            # RELATIO DECLARATA stirpem AEQUE EXPLICAT: familia
            # versionis/editionis/expressionis hic vivit -
            # ':macos_14_sonoma' versio est de ':macos', et doctrina
            # domus (FRBR) NEGAT editionem speciem operis esse.
            # Cognatio vera est ubi sub= falsum esset. Ergo arcus
            # monitum tacet, non sola maioritas.
            if ! maior "$stirps" "$b" && \
               ! grep -qxE "$b\|$stirps|$stirps\|$b" "$NEXUS"; then
                echo "'$b' stirpem '$stirps' fert sed sub ea non stat" \
                    >> "$MONITA"
            fi
            break
        fi
        case "$stirps" in *_*) stirps="${stirps%_*}" ;; *) stirps="" ;; esac
    done
done < "$NOMINA"

# ---- MONITUM: HOMONYMIA TRANS EXEMPLARIA (regula XIX, dormiens)
# Regula XV unicitatem INTRA exemplar poscit; trans exemplaria
# nomen idem iam LICET - ianua aperta est, numquam adhibita
# (mensuratum 2026-08-05: nomina CDXCIII, homonyma NULLA). Ergo
# unicitas globalis CONSUETUDO est, non lex - et sessio futura id
# scire debet, ne suffixa ponat (substantia_chemica,
# numerus_dea) ubi exemplar alienum sufficeret.
# Hoc monitum ianuam CUSTODIT, non claudit: primo homonymo dicit
# 'declara aut renomina', ne tacite fiat. Nulla portae severitas
# ante casum primum - quod nondum accidit probari non potest.
HOMO="$TMP/homonyma.txt"
: > "$HOMO"
cut -d'|' -f1,2 "$GENERA" > "$HOMO"
awk -F'|' 'NF>=3 {print $1"|"$3}' "$RESGEN" | sed 's/|:/|/' >> "$HOMO"
cut -d'|' -f2 "$HOMO" | sort | uniq -d | while read -r hn; do
    hmods=$(grep "|$hn\$" "$HOMO" | cut -d'|' -f1 | sort -u \
            | paste -sd, - | sed 's/,/, /g')
    echo "'$hn' in exemplaribus pluribus ($hmods) - homonymia: nota utrimque DECLARANDA (cur non idem genus), et citationes modulum= ferre debent" \
        >> "$MONITA"
done

# ---- MONITUM: RATIO.md contra worklogum (defluxus prosae) ----
# RATIO.md rota XLII scripta est NE materia orientationis
# deflueret, et rota L septem cursibus retro iacebat - documentum
# quod contra defluxum exsistit et ipsum defluit, quia prosam
# nulla porta custodit. Rota LIX iterum septem retro.
# Hoc minimum quod custodiri POTEST custodit: numerum cursuum
# circuli. Worklogus eos nominat ('coverage loop, run N'), RATIO
# eos tabula enumerat. Discrepantia = monitum.
_wl="natura/natura.worklog.md"
_rt="natura/RATIO.md"
if [ -r "$_wl" ] && [ -r "$_rt" ]; then
    cursusWl=$(grep -o 'coverage loop, run [0-9]*' "$_wl" \
               | grep -o '[0-9]*' | sort -n | tail -1)
    cursusRt=$(sed -n '/| run | scenario/,/^$/p' "$_rt" \
               | grep -o '^| [0-9]* ' | grep -o '[0-9]*' \
               | sort -n | tail -1)
    if [ -n "$cursusWl" ] && [ -n "$cursusRt" ] \
       && [ "$cursusWl" != "$cursusRt" ]; then
        echo "RATIO.md cursus $cursusRt enumerat, worklogus $cursusWl - prosa orientationis RANCIDA" \
            >> "$MONITA"
    fi
fi

nMonita=$(wc -l < "$MONITA" | tr -d ' ')

# ---- numeri ----
nModuli=$(ls natura/*.genera | wc -l | tr -d ' ')
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
    local mod="$1" parens="$2" linea g s nsp nin ndu nma npa insig
    echo "<ul>"
    while IFS='|' read -r _ g s nsp nin ndu nma npa; do
        [ "$s" != "$parens" ] && continue
        insig=""
        [ "$nsp" -gt 0 ] && insig="$insig species:$nsp"
        [ "$nin" -gt 0 ] && insig="$insig individua:$nin"
        printf '<li><span class="genus">%s</span>' "$g"
        supra=$(grep "^$mod|$g|" "$TRANSRADICES" | cut -d'|' -f3)
        [ -n "$supra" ] && printf ' <span class="umbra">&sub; %s</span>' "$supra"
        [ -n "$insig" ] && printf ' <span class="insignia">%s</span>' "$insig"
        [ "$nma" -gt 0 ] && printf ' <span class="machina">&#9881;machina</span>'
        [ "$npa" -gt 0 ] && printf ' <span class="machina">&#9635;partes:%s</span>' "$npa"
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
while IFS='|' read -r fm fg rel tm tgt fr; do
    [ "$fm" = "$tm" ] && continue
    ostensum="$fg"
    [ -n "$fr" ] && [ "$fr" != "$fg" ] && ostensum="$fg/$fr"
    echo "<tr><td>$fm.$ostensum</td><td class='rel'>--$rel--&gt;</td><td>$tm.$tgt</td></tr>"
done < <(sort -u "$ARCUS")
echo "</table>"

# ---- umbrae: duae agendae ----
echo "<h2>UMBRAE - GENERA DESIDERATA ($nUmbGen)</h2>"
echo "<p class='insignia'>a relatione citata: opus consilii</p><ul>"
sort -t'|' -k3,3nr "$UMBCLS" | while IFS='|' read -r u cls ncit mods; do
    [ "$cls" = "genus" ] || continue
    echo "<li class='umbra'>$u <span class='insignia'>(citatum ${ncit}x: $mods)</span></li>"
done
echo "</ul>"
echo "<h2>UMBRAE - INDIVIDUA DESIDERATA ($nUmbInd)</h2>"
echo "<p class='insignia'>a relato solo citata: opus dictionarii, nullum consilium</p><ul>"
sort -t'|' -k3,3nr "$UMBCLS" | while IFS='|' read -r u cls ncit mods; do
    [ "$cls" = "individuum" ] || continue
    echo "<li class='umbra'>$u <span class='insignia'>(citatum ${ncit}x: $mods)</span></li>"
done
echo "</ul>"

# ---- dubia ----
echo "<h2>DUBIA APERTA</h2><ul>"
while IFS='|' read -r m c t; do
    echo "<li><span class='insignia'>$m /</span> $c &mdash; <span class='insignia'>$t</span></li>"
done < "$DUBIA"
echo "</ul>"

# ---- monita ----
echo "<h2>MONITA (advisoria - portam non frangunt)</h2>"
echo "<p class='insignia'>stirps communis sine cognatione: aut nomen fallit, aut arbor arcum amisit</p>"
if [ "$nMonita" -eq 0 ]; then
    echo "<p class='sanum'>nulla</p>"
else
    echo "<ul>"
    while IFS= read -r mo; do
        echo "<li class='dubium-i'>$mo</li>"
    done < "$MONITA"
    echo "</ul>"
fi

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

# ---- INDEX.md: proiectio ad oculos AGENTIS (non hominis) ----
# HTML homini servit; sessio nova textum brevem greppabilem
# poscit. Semper scribitur, numquam per vexillum: quod oblivioni
# dari potest, dabitur - et git indicem rancidum statim ostendit.
{
echo "# NATURA — INDEX GENERUM"
echo
echo "**GENERATUM** a \`tools/natura_visus.sh\` — noli manu emendare."
echo "Regenera: \`./tools/natura_visus.sh\` (idem cursus qui portam custodit)."
echo
echo "Exemplaria **$nModuli** · genera **$nGenera** · res dictionarii **$nRes** · arcus **$nArcus**"
echo
echo "Cur haec exsistat, cui serviat, quomodo laboremus: \`natura/RATIO.md\`."
echo "Forma ipsa (elementa, attributa, regulae): \`natura/METAMODULUS.md\`."
echo "Historia et doctrina: \`natura/natura.worklog.md\` (LEGE PRIMUM)."
echo
echo "## I. Quaestio usitatissima: an genus iam exsistat?"
echo
echo '```'
echo "grep -i '<terminus>' natura/INDEX.md      # genus aut res"
echo "grep -n 'nomen=\"<genus>\"' natura/*.genera   # sedes definitionis"
echo "./tools/natura_quaere.sh <terminus>        # QUAERE (definitiones+species)"
echo "./tools/natura_quaere.sh -a mod.genus      # APPARATUS: quid rogare debeas"
echo '```'
echo
echo "## II. Genera per exemplar"
echo
echo "Signa apparatus — quid genus applicanti PRAESTET (RATIO §3):"
echo "\`⚙\` machinam statuum fert · \`▣N\` partes N declaratas fert"
echo "(partes = index quo ABSENTIA visibilis fit)."
echo
for mv in $MODULI; do
    mod="${mv%%:*}"; ver="${mv##*:}"
    ng=$(grep -c "^$mod|" "$GENERA")
    echo "### $mod (v$ver, genera $ng)"
    echo
    while IFS='|' read -r _ g gl; do
        linea=$(grep "^$mod|$g|" "$GENERA")
        supra=$(grep "^$mod|$g|" "$TRANSRADICES" | cut -d'|' -f3)
        parens=$(echo "$linea" | cut -d'|' -f3)
        nma=$(echo "$linea" | cut -d'|' -f7)
        npa=$(echo "$linea" | cut -d'|' -f8)
        marca=""
        [ -n "$supra" ] && marca=" ⊂ $supra"
        [ -n "$parens" ] && marca=" ⊂ $parens"
        app=""
        [ "${nma:-0}" -gt 0 ] && app="$app ⚙"
        [ "${npa:-0}" -gt 0 ] && app="$app ▣$npa"
        echo "- **$g**$marca$app — $gl"
    done < <(grep "^$mod|" "$GLOSSAE")
    echo
done
echo "## III. Index alphabeticus (genera)"
echo
echo "| genus | exemplar |"
echo "|---|---|"
sort -t'|' -k2,2 "$GLOSSAE" | while IFS='|' read -r m g _; do
    echo "| $g | $m |"
done
echo
echo "## IV. Dictionarium (species et individua descripta)"
echo
for mv in $MODULI; do
    mod="${mv%%:*}"
    for g in $(grep "^$mod|" "$RESGEN" | cut -d'|' -f2 | sort -u); do
        lista=$(grep "^$mod|$g|" "$RESGEN" | cut -d'|' -f3 \
                | sort | paste -sd, - | sed 's/,/, /g')
        [ -n "$lista" ] && echo "- \`$mod.$g\` — $lista"
    done
done
echo
echo "## V. Umbrae — superficta, nondum descripta"
echo
echo "Agenda COMPUTATA (non memorata): quod aliquod exemplar citat sed"
echo "nemo describit. DUAE agendae sunt, non una — ordo intra utramque"
echo "per numerum citationum (quod plura exemplaria poscunt, prius)."
echo
echo "### V.a Genera desiderata ($nUmbGen) — opus consilii"
echo
echo "A \`relatio\` citata: relationes in GENERA tendunt, ergo quaestio"
echo "ontologica et iudicium poscitur."
echo
sort -t'|' -k3,3nr "$UMBCLS" | while IFS='|' read -r u cls ncit mods; do
    [ "$cls" = "genus" ] || continue
    echo "- **$u** — ${ncit}x, a \`$mods\`"
done
echo
echo "### V.b Individua desiderata ($nUmbInd) — opus dictionarii"
echo
echo "A \`relato\` SOLO citata: res singularis nominata sub genere iam"
echo "exsistente. Nullum consilium — describi potest quovis momento."
echo "(EURISTICA, non lex: relatum genus attingere potest.)"
echo
sort -t'|' -k3,3nr "$UMBCLS" | while IFS='|' read -r u cls ncit mods; do
    [ "$cls" = "individuum" ] || continue
    echo "- **$u** — ${ncit}x, a \`$mods\`"
done
echo
echo "## VI. Dubia aperta"
echo
while IFS='|' read -r m c t; do
    echo "- \`$m\` / **$c** — $t"
done < "$DUBIA"
echo
echo "## VII. Monita — stirps communis sine cognatione ($nMonita)"
echo
echo "Regula XVIII (ADVISORIA, portam non frangit). Nomen quod nomen"
echo "aliud ut stirpem fert, sed nec sub eo stat nec relationem ad id"
echo "declarat. **Pleraque LEGITIMA sunt** — homonyma honesta:"
echo "\`fons_c\` plagula est, \`fons\` propositionis origo. Pretium"
echo "regulae non est quod defectus inveniat sed quod nulla talis"
echo "coincidentia INVISA transeat."
echo
if [ "$nMonita" -eq 0 ]; then
    echo "- nulla"
else
    sed 's/^/- /' "$MONITA"
fi
echo
echo "## VIII. Vocabularium formae (omnes tituli licentes)"
echo
echo "**Elementa (${nElementa}):** $(echo $_elementa | sed 's/ /, /g')"
echo
echo "**Attributa:** $(echo $_attributa | sed 's/ version encoding//' | sed 's/ /, /g')"
echo
echo "Vocabularium CLAUSUM est (METAMODULUS regula VIII): titulus novus"
echo "sine emendatione specificationis portam frangit."
} > natura/INDEX.md

# ---- relatio terminalis ----
echo "natura_visus: $EXITUS scriptum"
echo "  exemplaria $nModuli / genera $nGenera / res $nRes / arcus $nArcus"
echo "  umbrae $nUmbrae (genera $nUmbGen, individua $nUmbInd) / dubia $nDubia"
echo "  tempus-validitatis $nValiditas / MONITA $nMonita / VULNERA $nVulnera"
if [ "$nVulnera" -gt 0 ]; then
    echo "  --- vulnera: ---"
    cat "$VULNERA" | sed 's/^/  /'
    exit 1
fi
exit 0
