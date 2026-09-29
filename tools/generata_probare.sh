#!/bin/bash
# tools/generata_probare.sh - PORTA GENERATORUM: artificia derivata
# commissa == regeneratio hodierna (silva-migratio T19a)
#
# Usus: ./tools/generata_probare.sh
# Exitus: 0 omnia recentia · 1 stalum aliquod (nominatur) · 2 usus.
#
# CUR. Instrumenta quae nemo currit mortua sunt: T13a invenit aedilem
# materiam non videre (aedilis.stml radicem 'materia/fontes' non
# habebat) - manifestum silvae corpus TACITE amiserat, et aedilis.stml
# a NULLA porta tegebatur (planta T19a: radice deleta porta aedilis
# CLXXXVII bonae manebat). Porta haec producta ipsa iudicat:
#   I.   amalgamata tria: caput vivum == caput VERBATIM intra .c
#        (amalgama dimidiata, ...XHAW3H) - tools/amalgama_caput.sh;
#   II.  lexicon C89 generatum (silva_lexicon_c89.{c,h}) == generator
#        hodiernus (spatia neglecta: formator plagulam commissam
#        post generationem format);
#   III. snippets RADIX_FONTES (*_fontes_generata.sh, ab aedile) ==
#        regeneratio per mandatum 'regeneratio:' cuiusque. Generatores
#        IN LOCO scribunt - copiae ante servantur et SEMPER (trap)
#        restituuntur; comparatio contra copias;
#   IV.  manifesta amalgamatoris (fontes_generata.h silvae, officinae,
#        tesserae) per tools/porta_vetustatis.sh - hic aedilis.stml
#        VERE tegitur;
#   V.   sectio numerorum latina.h (ZEPHYRUM-MMMCMXCIX) == generator
#        (tools/latina_numeri.sh; fons numerus_romanus_scribere);
#   VI.  tabulae runarum (lib/runae_tabulae.c) == generator ex datis
#        Unicode fixis (tools/runae_generare.sh -probare, spatia
#        neglecta);
#   VII. amalgamata .c TOTA == regeneratio hodierna (AMALGAMA_COMPARARE=1
#        ./X/amalgamare.sh: generatio sola, sine verificationibus) -
#        commenta capitum quoque (quaestio ...G73P, 2026-09-29).
# Stala per nomen; nihil tacite.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
TMP="$(mktemp -d)"
fracta=0

# ---- I. amalgamata: caput vivum == caput intra .c ----
for p in silva officina tessera; do
    ./tools/amalgama_caput.sh "$p/amalgama/$p.c" "$p/amalgama/$p.h" \
        -comparare 2>"$TMP/caput.err"
    rc=$?
    if [ "$rc" -eq 0 ]; then
        echo "  ok   amalgama $p: caput congruit"
    else
        echo "  STALUM amalgama $p (rc $rc): $(head -1 "$TMP/caput.err")"
        fracta=1
    fi
done

# ---- II. lexicon C89 generatum ----
mkdir -p "$TMP/lexicon"
if ./silva/instrumenta/lexicon_c89_generare.sh "$TMP/lexicon" \
        >"$TMP/lexicon.log" 2>&1; then
    for f in silva_lexicon_c89.c silva_lexicon_c89.h; do
        if [ "$(tr -s ' \t\n' ' ' < "silva/fontes/$f")" \
             = "$(tr -s ' \t\n' ' ' < "$TMP/lexicon/$f")" ]; then
            echo "  ok   lexicon: silva/fontes/$f recens"
        else
            echo "  STALUM lexicon: silva/fontes/$f != generator (./silva/instrumenta/lexicon_c89_generare.sh, deinde formator -scribere)"
            fracta=1
        fi
    done
else
    echo "  FRACTUM lexicon: generator: $(tail -1 "$TMP/lexicon.log")"
    fracta=1
fi

# ---- III. snippets aedilis ----
# bash 3.2 (macOS): mapfile abest
SNIPPETS=()
while IFS= read -r s; do SNIPPETS+=("$s"); done \
    < <(git ls-files '*fontes_generata.sh')
if [ "${#SNIPPETS[@]}" -eq 0 ]; then
    echo "  FRACTUM snippets: nulli inventi (git ls-files)"
    echo "generata: FRACTA"
    exit 1
fi
mkdir -p "$TMP/copiae"
for s in "${SNIPPETS[@]}"; do
    mkdir -p "$TMP/copiae/$(dirname "$s")"
    cp "$s" "$TMP/copiae/$s"
done
_restituere () {
    for s in "${SNIPPETS[@]}"; do
        cp "$TMP/copiae/$s" "$s"
    done
    rm -rf "$TMP"
}
trap _restituere EXIT

# mandata regenerationis (linea '# regeneratio:'), unumquodque semel
git grep -h '^# regeneratio: ' -- '*fontes_generata.sh' \
    | sed 's/^# regeneratio: //' | sort -u > "$TMP/mandata"
while IFS= read -r mandatum; do
    # shellcheck disable=SC2086
    if ! ( eval "$mandatum" ) >"$TMP/mandatum.log" 2>&1; then
        echo "  FRACTUM regeneratio: $mandatum"
        tail -3 "$TMP/mandatum.log" | sed 's/^/      /'
        fracta=1
    fi
done < "$TMP/mandata"

# ---- IV. manifesta amalgamatoris (porta vetustatis) ----
# tools/porta_vetustatis.sh (2026-08-20, plantata) intra amalgamare.sh
# SOLUM currebat - nulla commissio eam debebat. Haec aedilis.stml
# vere tegit: radice 'materia/fontes' deleta manifestum silvae capita
# materiae omnia amittit (planta T19a) - snippets III ea non vident
# (lib/*.c solum enumerant).
for p in silva officina tessera; do
    if ./tools/porta_vetustatis.sh "$p" >"$TMP/vetustas.log" 2>&1; then
        echo "  ok   manifestum amalgamatoris $p: recens"
    else
        echo "  STALUM manifestum amalgamatoris $p (regenera: ./tools/amalgama_fontes_generare.sh $p)"
        grep -E '^[<>]' "$TMP/vetustas.log" | head -4 | sed 's/^/      /'
        fracta=1
    fi
done

# ---- V. numeri latina.h ----
if ./tools/latina_numeri.sh >"$TMP/numeri.log" 2>&1; then
    echo "  ok   numeri latina.h: sectio recens"
else
    echo "  STALUM numeri latina.h: $(tail -1 "$TMP/numeri.log")"
    fracta=1
fi

# ---- VI. tabulae runarum ----
if ./tools/runae_generare.sh -probare >"$TMP/runae.log" 2>&1; then
    echo "  ok   tabulae runarum: lib/runae_tabulae.c recens"
else
    echo "  STALUM tabulae runarum: $(tail -1 "$TMP/runae.log")"
    fracta=1
fi

# ---- VII. amalgamata .c == regeneratio hodierna ----
# I caput solum confert; hic corpus TOTUM (commenta capitum quoque:
# a3632f55 descriptiones XXV capitum mutavit, amalgamata tria rancida
# commissa, nemo vidit). Obiecta et amalgamator incrementaliter, deinde
# generatio sola (~0,2 s) - verificationes amalgamare.sh non currunt.
for p in silva tessera officina; do
    AMALGAMA_COMPARARE=1 "./$p/amalgamare.sh" > "$TMP/amalgama_$p.log" 2>&1
    rc=$?
    if [ "$rc" -eq 0 ]; then
        echo "  ok   $(tail -1 "$TMP/amalgama_$p.log")"
    elif [ "$rc" -eq 1 ]; then
        echo "  STALUM $(tail -1 "$TMP/amalgama_$p.log")"
        fracta=1
    else
        echo "  FRACTUM amalgama $p (exitus $rc): $(tail -1 "$TMP/amalgama_$p.log")"
        fracta=1
    fi
done

stala=0
for s in "${SNIPPETS[@]}"; do
    if ! cmp -s "$s" "$TMP/copiae/$s"; then
        echo "  STALUM snippet: $s"
        diff "$TMP/copiae/$s" "$s" | head -6 | sed 's/^/      /'
        stala=$((stala + 1))
        fracta=1
    fi
done
echo "  snippets: ${#SNIPPETS[@]} inspecti, $stala stala"

if [ "$fracta" -eq 0 ]; then
    echo "generata: sana"
    exit 0
fi
echo "generata: FRACTA"
exit 1
