#!/bin/bash
# tools/specimina_colligere.sh - figuras API Anthropic veras colligere
# (omne responsum in herbarium; petitiones tools/specimina_anthropic).
# NON PORTA: API vera (centesimae). Usus:
#   ./tools/specimina_colligere.sh [-ad <acervus>] [<directorium petitionum>]
#   (AEDIFICARE_SOLUM=1: struere sine cursu)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis tools/specimina_colligere.c >&2 || exit 1
bash build/aedilis/specimina_colligere/struere.sh >&2 || exit 1
[ -n "${AEDIFICARE_SOLUM:-}" ] && exit 0
exec build/aedilis/specimina_colligere/specimina_colligere "$@"
