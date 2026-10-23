#!/bin/bash

# tools/exemplar_mcp_fumus.sh - fumus vivus servi MCP exemplaris
#
# Usus: tools/exemplar_mcp_fumus.sh <basis>
#       e.g. tools/exemplar_mcp_fumus.sh https://exemplar.golden.earth
#            tools/exemplar_mcp_fumus.sh http://127.0.0.1:3010
#
# NUMQUAM intrat, nihil grave scribit (mcp-remotus T8):
#   POST /mcp sine tessera -> 401 cum WWW-Authenticate OCTETIM
#   documenta PRM (II viae) et auctoritatis: JSON, resource et issuer
#     inter se et cum basi congruunt
#   GET /mcp -> 405
#   POST /oauth/register (redirectio loopback probationis) -> 201 -
#     cliens sine concessione post diem putatur
# Exitus: 0 omnia bona | 1 aliquid fractum | 2 usus
set -u

[ $# -eq 1 ] || { echo "usus: $0 <basis>" >&2; exit 2; }
BASIS="${1%/}"
TMP="$(mktemp -d "${TMPDIR:-/tmp}/exemplar_mcp_fumus.XXXXXX")" || exit 2
trap 'rm -rf "$TMP"' EXIT
FRACTA=0

bene ()    { echo "BENE     $*"; }
fractum () { echo "FRACTUM  $*"; FRACTA=1; }

# petitio: methodus via [corpus] -> status in stdout; capita/corpus in TMP
petere () {
    local methodus="$1" via="$2" corpus="${3:-}"
    if [ -n "$corpus" ]; then
        curl -s -o "$TMP/corpus" -D "$TMP/capita" -w '%{http_code}' \
            -X "$methodus" -H 'Content-Type: application/json' \
            --data-binary "$corpus" "$BASIS$via"
    else
        curl -s -o "$TMP/corpus" -D "$TMP/capita" -w '%{http_code}' \
            -X "$methodus" "$BASIS$via"
    fi
}

# valor JSON per viam ("a.0.b") ex TMP/corpus; "" si deest
json () {
    python3 -I -c '
import json, sys
try:
    v = json.load(open(sys.argv[1]))
    for k in sys.argv[2].split("."):
        v = v[int(k)] if isinstance(v, list) else v[k]
    print(v if isinstance(v, str) else json.dumps(v))
except Exception:
    print("")
' "$TMP/corpus" "$1"
}

# 1. /mcp sine tessera
INITIUM='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-11-25","capabilities":{},"clientInfo":{"name":"fumus","version":"0"}}}'
CAPUT="WWW-Authenticate: Bearer resource_metadata=\"$BASIS/.well-known/oauth-protected-resource\", scope=\"notae:legere notae:scribere\""
s="$(petere POST /mcp "$INITIUM")"
if [ "$s" = 401 ] && tr -d '\r' < "$TMP/capita" | grep -qFx "$CAPUT"; then
    bene "POST /mcp sine tessera: 401 cum capite octetim"
else
    fractum "POST /mcp sine tessera: status $s, caput: $(tr -d '\r' < "$TMP/capita" | grep -i '^www-authenticate' || echo absens)"
fi

# 2. documenta inventionis
RESOURCE=""
for via in /.well-known/oauth-protected-resource \
           /.well-known/oauth-protected-resource/mcp; do
    s="$(petere GET "$via")"
    r="$(json resource)"
    a="$(json authorization_servers.0)"
    if [ "$s" = 200 ] && [ "$r" = "$BASIS/mcp" ] && [ "$a" = "$BASIS" ]; then
        bene "GET $via: resource et auctoritas congruunt"
        RESOURCE="$r"
    else
        fractum "GET $via: status $s, resource '$r', auctoritas '$a'"
    fi
done
s="$(petere GET /.well-known/oauth-authorization-server)"
i="$(json issuer)"
reg="$(json registration_endpoint)"
if [ "$s" = 200 ] && [ "$i" = "$BASIS" ] && [ "$reg" = "$BASIS/oauth/register" ] \
    && [ "$(json code_challenge_methods_supported.0)" = S256 ]; then
    bene "GET /.well-known/oauth-authorization-server: issuer $i, S256"
else
    fractum "documentum auctoritatis: status $s, issuer '$i', register '$reg'"
fi
[ -n "$RESOURCE" ] || fractum "resource nullum"

# 3. GET /mcp -> 405
s="$(petere GET /mcp)"
if [ "$s" = 405 ] && tr -d '\r' < "$TMP/capita" | grep -qix 'allow: POST'; then
    bene "GET /mcp: 405 Allow: POST"
else
    fractum "GET /mcp: status $s"
fi

# 4. inscriptio (cliens probationis, putabitur)
s="$(petere POST /oauth/register '{"redirect_uris":["http://127.0.0.1:1/fumus"],"client_name":"fumus","token_endpoint_auth_method":"none"}')"
id="$(json client_id)"
if [ "$s" = 201 ] && [ -n "$id" ]; then
    bene "POST /oauth/register: 201 (cliens fumi, post diem putatur)"
else
    fractum "POST /oauth/register: status $s"
fi

if [ "$FRACTA" = 0 ]; then
    echo "exemplar_mcp_fumus: $BASIS - omnia bona"
fi
exit "$FRACTA"
