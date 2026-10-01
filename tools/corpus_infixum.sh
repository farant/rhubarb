#!/bin/bash
# tools/corpus_infixum.sh - corpus bibliothecarum INFIXUM: capsula
# build/capsula_corpus_silicis.{c,h} e lib/ include/ vendor/ canonibus.
#
# STAMPA (corpus.versio, in capsula infixa) = SIGILLUM contentorum
# corporis (fabrica 1b T6, Fran: optio a'): reproducibile - actio
# fabricae 'corpus_silicis' per regenerationem iudicatur - et clavis
# proiectorum briar (~/.rhubarb/briar/<titulus>-<clavis>) mutatur
# EXACTE cum contentis. Olim 'commit=... dies=...': omni regeneratione
# mutabatur (proiecta orbata) nec iudicari poterat. Identitas
# aedificationis: briar -versio (briar_aedificatio), -provenientia.
#
# Manu vocatum: regeneratur SOLUM cum fons recentior (mtime). Sub iudice
# (FABRICA_SCRIPTURA, radix umbra) et executore (FABRICA_AGIT) semper -
# fabrica stalitatem iudicat.
#
# Fons COMMUNIS duorum binariorum: tools/silex_struere.sh et
# tools/briar_struere.sh eum 'source' faciunt et
# corpus_infixum_regenerare vocant e radice arboris. Obiectum unum,
# stampa una - 'silex -versio' et 'briar -versio' eandem dicunt.
#
# Usus: ./tools/corpus_infixum.sh  (actio corpus_silicis), aut
#       (e radice) source tools/corpus_infixum.sh; corpus_infixum_regenerare

# shellcheck source=/dev/null
source "$(dirname "${BASH_SOURCE[0]}")/capsula_radicis.sh"

# tabula symbolum -> plagula domus: symbolum, genus
# (functio|macro|typedef|constans|variabile|parametrum), plagula - e
# nexu silvae (build/nexus.tsv, sedes solae). Ordines DUO generum:
#   include/*.h - briar inclusiones e USU symbolorum derivat (silva
#     symbola implicita et typos ignotos novit, haec tabula caput dicit);
#   lib/*.c - STATICA scopi plagulae (functio variabile typedef constans
#     macro profunditate 0 quorum nomen in NULLO capite sedet): briar
#     -amalgama ea per plagulam renominat (#define/#undef) ne fontes in
#     plagula una collidant. Nexus linkage non novit: 'sine sede in
#     capite' = staticum (functio publica sine prototypo capitis sub
#     -Wmissing-prototypes non compilat).
# Regenerata cum caput aut fons quivis recentior.
# RADICES CLIENTIUM materiae: caput et implementatio in eodem directorio
# (toml Q12/Q13). Idem ordo et eadem nomina ac SILEX_RADICES_CLIENTIUM
# in lib/silex.c - probatio_silex utrumque conferre cogit.
RADICES_CLIENTIUM=(materia/fontes toml/fontes)

corpus_symbola_generare () {
    local TABULA=corpus.symbola.tsv
    local RADIX="${CAPSULA_RADIX:-$(pwd)}"
    if [ "${CAPSULA_SEMPER:-0}" != 1 ] && [ -f "$TABULA" ] && [ -z "$(find include "${RADICES_CLIENTIUM[@]}" -name '*.h' -newer "$TABULA" -print -quit 2>/dev/null)" ] \
        && [ -z "$(find lib "${RADICES_CLIENTIUM[@]}" -name '*.c' -newer "$TABULA" -print -quit 2>/dev/null)" ]; then
        return 0
    fi
    echo "  [symbola] corpus.symbola.tsv (nexus incrementalis)"
    # nexus = cache arboris VERAE (build/nexus.tsv), etiam sub umbra
    ( cd "$RADIX" && ./silva/nexus.sh -renovare > /dev/null 2>&1 ) || return 1
    [ -f "$RADIX/build/nexus.tsv" ] || return 1
    {
        echo "# corpus.symbola.tsv GENERATUM a tools/corpus_infixum.sh (sedes include/*.h et <radix>/*.h = capita; sedes lib/*.c et <radix>/*.c profunditate 0 sine capite = statica; radices: ${RADICES_CLIENTIUM[*]}) e build/nexus.tsv - NE MANU EDITES"
        awk -F'\t' -v radices="${RADICES_CLIENTIUM[*]}" '
            BEGIN { nr = split(radices, R, " ") }
            function in_radice(via, suffixum,    k) {
                for (k = 1; k <= nr; k++)
                    if (index(via, R[k] "/") == 1 && via ~ ("\\" suffixum "$") \
                        && substr(via, length(R[k]) + 2) !~ /\//) return 1
                return 0
            }
            $2=="sedes" && ($4 ~ /^include\// || in_radice($4, ".h")) { publica[$1]=1; print $1"\t"$3"\t"$4 }
            $2=="sedes" && ($4 ~ /^lib\/[^\/]*\.c$/ || in_radice($4, ".c")) && $7=="0" \
                && ($3=="functio" || $3=="variabile" || $3=="typedef" || $3=="constans" || $3=="macro") {
                n=n+1; statica[n]=$1"\t"$3"\t"$4; nomina[n]=$1
            }
            END { for (k=1; k<=n; k++) if (!(nomina[k] in publica)) print statica[k] }
        ' "$RADIX/build/nexus.tsv" | sort -u
    } > "$TABULA" || return 1
    return 0
}

# plagulae corporis (iidem globi ac corpus_silicis_files), ordine C
_corpus_plagulae () {
    local radix
    (
        export LC_ALL=C
        shopt -s nullglob
        local globi=(lib/*.c lib/*.m include/*.h)
        for radix in "${RADICES_CLIENTIUM[@]}"; do
            globi+=("$radix"/*.c "$radix"/*.h)
        done
        globi+=(vendor/* tools/capsula_generare.c corpus.symbola.tsv
            natura/cocta/*.canon natura/cocta/semina.census
            canones.registrum natura/natura.canon aedilis.canon canon.canon
            silva/grammatica/*.canon silva/quaestiones.canon
            silva/fontes/systema_c89.h silva/fontes/systema_posix.h)
        for f in "${globi[@]}"; do
            [ -f "$f" ] && printf '%s\n' "$f"
        done
    )
}

# sigillum contentorum: via + SHA-256 cuiusque, deinde SHA-256 totius
corpus_sigillum () {
    local f
    _corpus_plagulae | while IFS= read -r f; do
        printf '%s\t%s\n' "$f" "$(shasum -a 256 < "$f" | cut -c1-64)"
    done | shasum -a 256 | cut -c1-64
}

corpus_infixum_regenerare () {
    : "${CAPSULA_RADIX:=$(pwd)}"
    corpus_symbola_generare || return 1
    local CORPUS_C=build/capsula_corpus_silicis.c
    local regen=0
    if [ "${CAPSULA_SEMPER:-0}" = 1 ] || [ ! -f "$CORPUS_C" ]; then
        regen=1
    elif [ -n "$(find lib include vendor tools/capsula_generare.c \
            "${RADICES_CLIENTIUM[@]}" natura/cocta canones.registrum natura/natura.canon \
            aedilis.canon canon.canon silva/grammatica/grammatica.canon \
            silva/quaestiones.canon corpus.symbola.tsv \
            silva/fontes/systema_c89.h silva/fontes/systema_posix.h \
            -newer "$CORPUS_C" -print -quit 2>/dev/null)" ]; then
        regen=1
    fi
    if [ "$regen" = 1 ]; then
        echo "  [corpus] sigillum + capsula (tardum semel)"
        printf 'sigillum=%s\n' "$(corpus_sigillum)" > corpus.versio
        local globi_radicum="" radix
        for radix in "${RADICES_CLIENTIUM[@]}"; do
            globi_radicum="$globi_radicum \"$radix/*.c\", \"$radix/*.h\","
        done
        {
            echo "# GENERATUM a tools/corpus_infixum.sh - NE MANU EDITES (gitignoratum)"
            echo "# radices clientium (RADICES_CLIENTIUM): instrumentum capsulae clientem"
            echo "# toml trahit (toml Q12) - vide lib/silex.c SILEX_RADICES_CLIENTIUM"
            echo "corpus_silicis_files = [\"lib/*.c\", \"lib/*.m\", \"include/*.h\",$globi_radicum \"vendor/*\", \"tools/capsula_generare.c\", \"corpus.versio\", \"corpus.symbola.tsv\", \"natura/cocta/*.canon\", \"natura/cocta/semina.census\", \"canones.registrum\", \"natura/natura.canon\", \"aedilis.canon\", \"canon.canon\", \"silva/grammatica/*.canon\", \"silva/quaestiones.canon\", \"silva/fontes/systema_c89.h\", \"silva/fontes/systema_posix.h\"]"
            echo "corpus_silicis_compress = true"
        } > corpus_silicis.toml
        capsula_radicis_generare corpus_silicis || return 1
    fi
    return 0
}

# actio fabricae 'corpus_silicis' (plan 1b T6): scriptum directe vocatum
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    capsula_radicis_parare lib include vendor materia toml tools natura \
        silva canones.registrum aedilis.canon canon.canon || exit 1
    corpus_infixum_regenerare || exit 1
fi
