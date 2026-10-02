#!/bin/bash
# tools/briar_struere.sh - bin/briar aedificare et in ~/.bin instituere
#
# Obiecta: briar/build/*.o (cursor briar/compile_probationes.sh ea
# struit: clausura silicis + sqlite + amalgama silvae + materia + md +
# briar) + corpus infixum build/capsula_corpus_silicis.c (bloccus
# COMMUNIS cum silex_struere.sh: tools/corpus_infixum.sh). ORDO: suite
# briar prius (obiecta), deinde hoc. Corpus e radice arboris legitur -
# in arbore secundaria (worktree) stampa eius commissum dicit.
#
# Usus: ./tools/briar_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ -z "$(ls -A briar/build/briar_*.o 2>/dev/null)" ]; then
    echo "briar_struere: briar/build/ vacuum - ./briar/compile_probationes.sh registrum prius" >&2
    exit 1
fi
if [ ! -x bin/capsula_generare ] && [ -z "$(ls -A build/*.o 2>/dev/null)" ]; then
    echo "briar_struere: build/ vacuum et bin/capsula_generare abest - ./compile_tests.sh registrum prius" >&2
    exit 1
fi

# capsulae: actiones fabricae suae (1b T6) - sub executore
# (FABRICA_AGIT) hoc scriptum SOLUM nectit (sanare eas ante ordinat);
# manu vocatum eas, si recentiores fontes, regenerat
if [ -z "${FABRICA_AGIT:-}" ]; then
    source "$SCRIPT_DIR/corpus_infixum.sh"
    corpus_infixum_regenerare || exit 1
    source "$SCRIPT_DIR/briar_facies_capsula.sh"
    briar_facies_capsula_regenerare || exit 1
    source "$SCRIPT_DIR/briar_icon_capsula.sh"
    briar_icon_capsula_regenerare || exit 1
    # charta mutationum: versio binarii EX ea legitur (briar -versio)
    source "$SCRIPT_DIR/briar_mutationes_capsula.sh"
    briar_mutationes_capsula_regenerare || exit 1
fi
# identitas aedificationis (bugs/011 lapidis): tempus, sigillum
# fontium briar IPSIUS (instrumentum, fontes, charta, compendium) et
# commissum - SORDIDUM si hi fontes mutationes non commissas ferunt.
# Stampa corporis (corpus_infixum.sh) corpus solum nominat; binaria
# diversa eandem ferebant. Generatur OMNI aedificatione (tempus).
FONTES_BRIAR=(tools/briar.c briar/fontes/*.c briar/fontes/*.h
    silva/instrumenta/silva_lexicon.c silva/instrumenta/silva_lexicon.h
    briar/MUTATIONES.md officina/instrumenta/compendium.c
    officina/instrumenta/compendium.h)
AED_TEMPUS="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
AED_FONTES="$(cat "${FONTES_BRIAR[@]}" | shasum -a 256 | cut -c1-8)"
AED_COMMISSUM="$(git rev-parse --short=8 HEAD 2>/dev/null || echo ignotum)"
if [ -n "$(git status --porcelain -- "${FONTES_BRIAR[@]}" 2>/dev/null)" ]; then
    AED_COMMISSUM="$AED_COMMISSUM SORDIDUM"
fi
mkdir -p build
cat > build/briar_aedificatio.c <<AEDIFICATIO
/* build/briar_aedificatio.c - GENERATUM a tools/briar_struere.sh -
 * NE MANU EDITES (briar -versio, linea 'aedificatum:') */
const char briar_aedificatio_tempus[] = "$AED_TEMPUS";
const char briar_aedificatio_fontes[] = "$AED_FONTES";
const char briar_aedificatio_commissum[] = "$AED_COMMISSUM";
AEDIFICATIO

# decodificator (stb_image per lib/imago.c) SOLUM in binario (spec par.
# 4.8 A1): nulla porta briar eum nectit, ergo obiectum RADICIS sumitur
if [ ! -f build/imago.o ]; then
    echo "briar_struere: build/imago.o abest - ./compile_tests.sh prius" >&2
    exit 1
fi

# obiecta: omnia briar/build/*.o praeter probationes (obiecta
# probationum ibi non cadunt, sed custodia eadem quae silex_struere)
OBJ=""
for o in briar/build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done

mkdir -p bin
# provenientia (fabrica T7): '-provenientia' respondetur; digestum
# clausuram aedilis sequitur (bin/aedilis tools/briar.c)
PROV_OBJ="$("$SCRIPT_DIR/provenientia_obiectum.sh" briar bin/briar tools/briar.c)" || exit 1
echo "  [briar] tools/briar.c + corpus"
clang "${GCC_FLAGS[@]}" -Iinclude -Imateria/fontes -Imd/fontes \
    -Ibriar/fontes -Isilva/amalgama -Iofficina/instrumenta \
    tools/briar.c lib/provenientia.c "$PROV_OBJ" build/capsula_corpus_silicis.c \
    build/capsula_icon_briar.c build/capsula_mutationes_briar.c \
    build/briar_aedificatio.c \
    build/imago.o $OBJ \
    -o bin/briar || exit 1
echo "aedificatum: bin/briar"

# institutio: exemplum in ~/.bin (in PATH) - '#!/usr/bin/env briar'
# eum ibi invenit; rm ante cp (macOS binarium superscriptum SIGKILL
# dat - inodus recens obligatorius)
# institutio in ~/.bin: actio fabricae sua ('institutio_briar', 1b T5) -
# sub executore (FABRICA_AGIT) hoc scriptum SOLUM nectit; manu vocatum
# etiam instituit
if [ -z "${FABRICA_AGIT:-}" ]; then
    ./tools/instituere.sh bin/briar || exit 1
fi
