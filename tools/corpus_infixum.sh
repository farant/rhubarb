#!/bin/bash
# tools/corpus_infixum.sh - corpus bibliothecarum INFIXUM: capsula
# build/capsula_corpus_silicis.{c,h} e lib/ include/ vendor/ canonibus
# (regenerata SOLUM cum fons recentior; stampa = commit eius temporis,
# SORDIDUM si arbor sordida in contentis corporis).
#
# Fons COMMUNIS duorum binariorum: tools/silex_struere.sh et
# tools/briar_struere.sh eum 'source' faciunt et
# corpus_infixum_regenerare vocant e radice arboris. Obiectum unum,
# stampa una - 'silex -versio' et 'briar -versio' eandem dicunt.
#
# Usus (e radice): source tools/corpus_infixum.sh; corpus_infixum_regenerare

corpus_infixum_regenerare () {
    local CORPUS_C=build/capsula_corpus_silicis.c
    local regen=0
    if [ ! -f "$CORPUS_C" ]; then
        regen=1
    elif [ -n "$(find lib include vendor tools/capsula_generare.c \
            natura/cocta canones.registrum natura/natura.canon \
            aedilis.canon canon.canon silva/grammatica/grammatica.canon \
            silva/quaestiones.canon \
            -newer "$CORPUS_C" -print -quit 2>/dev/null)" ]; then
        regen=1
    fi
    if [ "$regen" = 1 ]; then
        echo "  [corpus] stampa + capsula (tardum semel)"
        local STAMPA="commit=$(git rev-parse --short HEAD 2>/dev/null \
            || echo ignotum)"
        # sorditia SCOPATA ad contenta corporis - plagulae aliae (FAQ,
        # gesta) semper mutatae sunt nec in capsulam eunt
        if [ -n "$(git status --porcelain -- lib include vendor \
                tools/capsula_generare.c natura/cocta canones.registrum \
                natura/natura.canon aedilis.canon canon.canon \
                silva/grammatica/grammatica.canon \
                silva/quaestiones.canon 2>/dev/null)" ]; then
            STAMPA="$STAMPA SORDIDUM"
        fi
        STAMPA="$STAMPA dies=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
        printf '%s\n' "$STAMPA" > corpus.versio
        cat > corpus_silicis.toml <<'TOML'
# GENERATUM a tools/corpus_infixum.sh - NE MANU EDITES (gitignoratum)
corpus_silicis_files = ["lib/*.c", "lib/*.m", "include/*.h", "vendor/*", "tools/capsula_generare.c", "corpus.versio", "natura/cocta/*.canon", "natura/cocta/semina.census", "canones.registrum", "natura/natura.canon", "aedilis.canon", "canon.canon", "silva/grammatica/*.canon", "silva/quaestiones.canon"]
corpus_silicis_compress = true
TOML
        if [ ! -x bin/capsula_generare ]; then
            ./compile_tools.sh capsula_generare >/dev/null || return 1
        fi
        ./bin/capsula_generare corpus_silicis.toml || return 1
        mv capsula_corpus_silicis.h capsula_corpus_silicis.c build/ \
            || return 1
    fi
    return 0
}
