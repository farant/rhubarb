#!/bin/bash

# tools/vitrea_spica.sh - specimen calibrationis WebKit compilare et
# currere (vide vitrea_spica.m; verdicta ->
# project-specs/vitrea-calibratio.md). Servum fixturae http (CORS
# permissivum) in 127.0.0.1:8747 ponit pro spicis IV/V.
#
# RE-CURRENDUM post renovationes macOS - WebKit cum systemate mutat.

set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p tools/build

clang -fobjc-arc -framework Cocoa -framework WebKit \
    -o tools/build/vitrea_spica tools/vitrea_spica.m

FIXDIR=$(mktemp -d)
cat > "$FIXDIR/serve.py" <<'PY'
import http.server, os, sys
class H(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        super().end_headers()
    def log_message(self, *a):
        pass
os.chdir(sys.argv[1])
http.server.HTTPServer(('127.0.0.1', 8747), H).serve_forever()
PY
printf '{"salve":"ex-http"}' > "$FIXDIR/data.json"
printf '<!doctype html><title>ats</title>specimen ats' > "$FIXDIR/index.html"
python3 "$FIXDIR/serve.py" "$FIXDIR" &
SERVUS=$!
trap 'kill $SERVUS 2>/dev/null || true; rm -rf "$FIXDIR"' EXIT
sleep 0.4

tools/build/vitrea_spica

echo "== repositoria WebKit recentia (situs persistentiae) =="
ls -dt ~/Library/WebKit/* 2>/dev/null | head -5 || true
