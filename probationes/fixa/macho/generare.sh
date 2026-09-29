#!/bin/bash
# generare.sh - fixa Mach-O probationum ex fontibus suis regenerare
#
# probationes/fixa/simplex et probationes/fixa/cum_functione sunt
# binaria COMMISSA (fixa congelata: probationes structuram legunt, non
# octetos, sed fixum non mutatur nisi hoc scripto cum causa nominata).
# Olim manu facta (2025-11-11) et gitignorata - in clone quavis
# probatio_macho, macho_deps, sectio, symbola rubrae (quaestio …QY4,
# 2026-09-28). Fontes: probationes/fixa/macho/{simplex,cum_functione}.c.
#
# -g0: vexilla domus -g ferunt; nexus tunc STABS (FUN _main, OSO cum
# via ABSOLUTA machinae) in tabula symbolorum relinquit - symbolum_invenire
# stab primum invenit (_main "debug", non functio) et binarium viam
# domesticam commissam ferret.
#
# Usus (ex radice): ./probationes/fixa/macho/generare.sh
set -e
cd "$(dirname "$0")/../../.."
source tools/vexilla.sh
# Obiectum in via FIXA, deinde nexus: clang solus obiectum in via
# temporaria fortuita ponit, et ld64 LC_UUID (ergo signaturam) ex eo
# derivat - binarium quoque cursu differret.
mkdir -p build/fixa_macho
for f in simplex cum_functione; do
    clang "${VEXILLA_C89[@]}" -Iinclude -O0 -g0 -c \
        "probationes/fixa/macho/$f.c" -o "build/fixa_macho/$f.o"
    clang "build/fixa_macho/$f.o" -o "probationes/fixa/$f"
    echo "generatum: probationes/fixa/$f"
done
