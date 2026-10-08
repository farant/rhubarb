#!/bin/bash
# tools/rete_fumus.sh - fumus retis (vates-plan-1 T1): TLS, badssl,
# tempora HTTPS, API Anthropic. NON PORTA: rete poscit, manu curritur.
# Usus: ./tools/rete_fumus.sh [-tls] [-badssl] [-mora] [-anthropic]
#       (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/rete_fumus.c >&2 || exit 1
bash build/aedilis/rete_fumus/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/rete_fumus/rete_fumus "$@"
