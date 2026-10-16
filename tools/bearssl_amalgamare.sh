#!/bin/bash
# tools/bearssl_amalgamare.sh - vendor/bearssl_amalgama.{c,h} GENERATA ex
# vendor/bearssl (actio fabricae 'bearssl_amalgama'; python declaratus ut
# instrumentum, ut entitates_html_generare.sh)
set -euo pipefail
cd "$(dirname "$0")/.."
exec /opt/homebrew/bin/python3 -I tools/bearssl_amalgamare.py "$PWD"
