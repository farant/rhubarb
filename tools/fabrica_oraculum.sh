#!/bin/bash
# tools/fabrica_oraculum.sh - PORTA ORACULI: bin/fabrica iudicare -plenus
# cum porta generata (tools/generata_probare.sh) CONSENTIT per
# artificium (fabrica plan 1a T8, spec v1 Q15: oraculum, deinde
# deletio - generata manet donec consensus in historia vera stetit).
#
# Consensus = ambo 'recens' aut ambo 'non recens'. Mappatio verdictorum:
# generata ok <-> fabrica RECENS; generata STALUM/FRACTUM <-> fabrica
# STALUM/IGNOTUM (generata IV fracturam compilationis STALUM vocat,
# fabrica generatorem fractum IGNOTUM - ambo 'non recens', nulla
# discordia). Artificium quod generata iudicat sed de quo fabrica
# TACET = discordia (tegmen amissum: declaratio deleta/omissa).
# Artificia quae fabrica sola iudicat (tabulae silvae, capsulae,
# excludenda) numerantur, non comparantur - oraculum nullum habent.
#
# Usus: ./tools/fabrica_oraculum.sh
# Exitus: 0 consensus · 1 discordia (nominata, verdicta ambo) ·
#         2 porta currere nequit (bin/fabrica deest, generata fracta
#         sine linea finali).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
[ -x bin/fabrica ] || { echo "oraculum fabricae: bin/fabrica deest (./tools/fabrica_struere.sh)"; exit 2; }
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT

# ---- I. generata (sequentialiter: generatores eadem clausurarum
# directoria scribunt ac iudex plenus - numquam simul) ----
# fabrica plan 5 T7a: porta 'generata' nunc PER FABRICAM iudicat
# (tools/generata_iudicare.sh) - iudex ALTER non est et lineas
# stadiorum non fert; olim receptum vivum eius hic adhibebatur
# (comparatio vacua = consensus falsus). Oraculum generata_probare.sh
# ipsum currit donec T7b id retiret.
./tools/generata_probare.sh > "$T/generata" 2>&1
if ! grep -qE '^generata: (sana|FRACTA)$' "$T/generata"; then
    echo "oraculum fabricae: generata sine linea finali"
    tail -5 "$T/generata" | sed 's/^/    /'
    exit 2
fi

# ---- II. fabrica plena, omnia verdicta ----
bin/fabrica iudicare -plenus -omnia > "$T/fabrica" 2>&1
rc=$?
if [ "$rc" -eq 2 ]; then
    echo "oraculum fabricae: bin/fabrica nihil iudicavit (exitus 2)"
    tail -5 "$T/fabrica" | sed 's/^/    /'
    exit 2
fi

# fabrica: 'VERDICTUM via - causa' -> 'via<TAB>recens|non'
sed -nE 's/^(RECENS|STALUM|IGNOTUM|NON IUDICATUM) ([^ ]+) - .*/\1	\2/p' \
    "$T/fabrica" | awk -F'\t' '{ print $2 "\t" ($1 == "RECENS" ? "recens" : "non") }' \
    | sort -u > "$T/f.tsv"

# generata: lineae per stadium -> 'via<TAB>recens|non'
{
    # I (caput: 'caput congruit') et VII (corpus: 'recens' /
    # 'RANCIDA' / FRACTUM cum exitu): amalgama X -> X/amalgama/X.c
    sed -nE 's/^  ok   amalgama ([a-z]+): .*/\1\/amalgama\/\1.c	recens/p' "$T/generata"
    sed -nE 's/^  (STALUM|FRACTUM) amalgama ([a-z]+)[ :].*/\2\/amalgama\/\2.c	non/p' "$T/generata"
    # II lexicon
    sed -nE 's/^  ok   lexicon: ([^ ]+) recens$/\1	recens/p' "$T/generata"
    sed -nE 's/^  STALUM lexicon: ([^ ]+) != .*/\1	non/p' "$T/generata"
    if grep -q '^  FRACTUM lexicon:' "$T/generata"; then
        printf 'silva/fontes/silva_lexicon_c89.c\tnon\nsilva/fontes/silva_lexicon_c89.h\tnon\n'
    fi
    # IV manifesta amalgamatoris
    sed -nE 's/^  ok   manifestum amalgamatoris ([a-z]+): recens$/\1\/instrumenta\/principalia\/fontes_generata.h	recens/p' "$T/generata"
    sed -nE 's/^  STALUM manifestum amalgamatoris ([a-z]+) .*/\1\/instrumenta\/principalia\/fontes_generata.h	non/p' "$T/generata"
    # V, VI, VIII
    grep -q '^  ok   numeri latina.h' "$T/generata" && printf 'include/latina.h\trecens\n'
    grep -q '^  STALUM numeri latina.h' "$T/generata" && printf 'include/latina.h\tnon\n'
    grep -q '^  ok   tabulae runarum' "$T/generata" && printf 'lib/runae_tabulae.c\trecens\n'
    grep -q '^  STALUM tabulae runarum' "$T/generata" && printf 'lib/runae_tabulae.c\tnon\n'
    grep -q '^  ok   entitates' "$T/generata" && printf 'lib/entitates_html_tabula.c\trecens\n'
    grep -q '^  STALUM entitates' "$T/generata" && printf 'lib/entitates_html_tabula.c\tnon\n'
    # III snippets: STALUM snippet: via; regeneratio fracta -> omnia
    # fragmenta eiusdem mandati 'non'
    git ls-files '*fontes_generata.sh' | while IFS= read -r s; do
        if grep -qxF "  STALUM snippet: $s" "$T/generata"; then
            printf '%s\tnon\n' "$s"
        else
            m="$(sed -n 's/^# regeneratio: //p' "$s")"
            if grep -qxF "  FRACTUM regeneratio: $m" "$T/generata"; then
                printf '%s\tnon\n' "$s"
            else
                printf '%s\trecens\n' "$s"
            fi
        fi
    done
} > "$T/g.raw"
# 'non' vincit (I aut VII fractum -> amalgama non recens)
sort -t'	' -k1,1 -k2,2 "$T/g.raw" | awk -F'\t' '
    { if (!($1 in v) || $2 == "non") v[$1] = $2 }
    END { for (k in v) print k "\t" v[k] }' | sort > "$T/g.tsv"

# ---- III. comparatio ----
discordiae=0
while IFS='	' read -r via g; do
    f="$(awk -F'\t' -v v="$via" '$1 == v { print $2 }' "$T/f.tsv")"
    if [ -z "$f" ]; then
        echo "  DISCORDIA $via: generata '$g', fabrica TACET (declaratio abest?)"
        discordiae=$((discordiae + 1))
    elif [ "$f" != "$g" ]; then
        linea="$(grep -E "^[A-Z ]+ $via - " "$T/fabrica" | head -1)"
        echo "  DISCORDIA $via: generata '$g', fabrica '$f' ($linea)"
        discordiae=$((discordiae + 1))
    fi
done < "$T/g.tsv"

communia="$(wc -l < "$T/g.tsv" | tr -d ' ')"
solae="$(cut -f1 "$T/g.tsv" | sort | comm -13 - <(cut -f1 "$T/f.tsv" | grep -v '^bin/' | sort) | wc -l | tr -d ' ')"
echo "  artificia communia: $communia comparata, $discordiae discordiae"
echo "  fabrica sola (sine oraculo): $solae generata"
if [ "$discordiae" -eq 0 ]; then
    echo "oraculum fabricae: consensus"
    exit 0
fi
echo "oraculum fabricae: DISCORDIA"
exit 1
