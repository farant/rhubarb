#!/bin/bash
# probationes/fixa/tls/generare.sh - certificata PROBATIONIS pro servo
# TLS locali (openssl s_server ut oraculum, probatio_tls_servus): CA
# probationis et quattuor certificata hospitis, claves EC P-256.
#
# IN TEMPORE PROBATIONIS generantur (in directorium datum), NON
# committuntur: macOS certificata hospitis TLS plus quam DCCCXXV dies
# valida reicit ("not standards compliant") - fixa committata intra
# biennium exspirarent et probationem frangerent. Nulla clavis
# privata in repositorio.
#
#   ca.pem / ca.der       CA probationis (ancora; DER pro SecCertificate)
#   localhost.{pem,key}   validum CCCLXV dies, SAN DNS:localhost IP:127.0.0.1
#   alienus.{pem,key}     ab eadem CA, sed SAN DNS:alienus.invalid
#   expiratus.{pem,key}   ab eadem CA, 2020-01-01 .. 2020-01-02
#   ignotus.{pem,key}     ab CA ALIA (in fixis non servata)
#   localhost_rsa.{pem,key}  ut localhost, clavis RSA-2048 (ECDHE_RSA;
#                         tls-bearssl R12)
#   serie_alta            series 0x8001 (DER: 00 80 01 - octetus 0x00
#                         servatus; fiducia-linux T1)
#   serie_longa           series XX octetorum
#   ip_solum              SAN IP:127.0.0.1,IP:::1 sine DNS
#   sine_san              CN=localhost, nulla extensio SAN
#   geminus               subiectum (CN=localhost) et clavis
#                         localhost.key IDEM, series alia - instar
#                         certificati transsignati (subiectum+clavis
#                         Mozillae eum quoque tangit)
#   sha1                  folium sha1WithRSA/ecdsa-with-SHA1 signatum
#   rsa_parva             folium RSA-1024
#   <titulus>.der         omne folium etiam DER (fiducia_iudicare)
#
# CLAVES SOLUM PROBATIONIS - nihil verum signant.
#
# Usus: probationes/fixa/tls/generare.sh <directorium>
set -euo pipefail
DEST="${1:?directorium destinationis}"
# openssl Homebrew in macOS, systematis in imaginibus Linux
OSSL_ORDINARIUS=/opt/homebrew/bin/openssl
[ -x "$OSSL_ORDINARIUS" ] || OSSL_ORDINARIUS="$(command -v openssl || true)"
OSSL="${OPENSSL:-$OSSL_ORDINARIUS}"
[ -n "$OSSL" ] || { echo "tls_fixa: openssl deest" >&2; exit 1; }
T="$(mktemp -d "${TMPDIR:-/tmp}/tls_fixa.XXXXXX")"
trap 'rm -rf "$T"' EXIT
mkdir -p "$DEST"

clavis () { "$OSSL" genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -out "$1" 2>/dev/null; }
clavis_rsa () { "$OSSL" genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -out "$1" 2>/dev/null; }

# CA probationis
clavis "$T/ca.key"
"$OSSL" req -x509 -new -key "$T/ca.key" -sha256 -days 36500 \
    -subj "/CN=rhubarb probatio CA" -out "$DEST/ca.pem" \
    -addext "basicConstraints=critical,CA:TRUE" \
    -addext "keyUsage=critical,keyCertSign,cRLSign"
"$OSSL" x509 -in "$DEST/ca.pem" -outform DER -out "$DEST/ca.der"

# configuratio minima 'openssl ca' (dies initii/finis arbitrarii)
mkdir -p "$T/ca"; : > "$T/ca/index.txt"; echo 1000 > "$T/ca/serial"
cat > "$T/ca.cnf" <<CNF
[ ca ]
default_ca = probatio
[ probatio ]
dir = $T/ca
database = \$dir/index.txt
serial = \$dir/serial
new_certs_dir = \$dir
certificate = $DEST/ca.pem
private_key = $T/ca.key
default_md = sha256
policy = laxa
copy_extensions = copy
unique_subject = no
[ laxa ]
commonName = supplied
CNF

hospes () {   # titulus SAN [initium finis] - sine diebus: CCCLXV ab hodie;
              # GENUS_CLAVIS=rsa ante vocationem: folium RSA-2048
    if [ "${GENUS_CLAVIS:-}" = rsa ]; then clavis_rsa "$DEST/$1.key"; else clavis "$DEST/$1.key"; fi
    "$OSSL" req -new -key "$DEST/$1.key" -subj "/CN=$1" \
        -addext "subjectAltName=$2" \
        -addext "extendedKeyUsage=serverAuth" -out "$T/$1.csr"
    if [ $# -ge 4 ]; then
        "$OSSL" ca -batch -config "$T/ca.cnf" -in "$T/$1.csr" \
            -startdate "$3" -enddate "$4" -out "$T/$1.crt" 2>/dev/null
    else
        "$OSSL" ca -batch -config "$T/ca.cnf" -in "$T/$1.csr" \
            -days 365 -out "$T/$1.crt" 2>/dev/null
    fi
    "$OSSL" x509 -in "$T/$1.crt" -out "$DEST/$1.pem"
    "$OSSL" x509 -in "$DEST/$1.pem" -outform DER -out "$DEST/$1.der"
}

hospes localhost "DNS:localhost,IP:127.0.0.1"
hospes alienus   "DNS:alienus.invalid"
hospes expiratus "DNS:localhost,IP:127.0.0.1" 20200101000000Z 20200102000000Z
GENUS_CLAVIS=rsa hospes localhost_rsa "DNS:localhost,IP:127.0.0.1"

# folia cum serie data aut digestione data (x509 -req: 'openssl ca'
# seriem ex tabula sua sumit) - titulus SAN serie [md] [clavis_data]
signare () {
    local titulus="$1" san="$2" serie="$3" md="${4:-sha256}" clavis_data="${5:-}"
    if [ -n "$clavis_data" ]; then
        k="$clavis_data"
    else
        k="$DEST/$titulus.key"
        if [ "${GENUS_CLAVIS:-}" = rsa1024 ]; then
            "$OSSL" genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:1024 -out "$k" 2>/dev/null
        else
            clavis "$k"
        fi
    fi
    if [ -n "$san" ]; then
        "$OSSL" req -new -key "$k" -subj "/CN=${CN:-$titulus}" \
            -addext "subjectAltName=$san" \
            -addext "extendedKeyUsage=serverAuth" -out "$T/$titulus.csr"
    else
        "$OSSL" req -new -key "$k" -subj "/CN=localhost" \
            -addext "extendedKeyUsage=serverAuth" -out "$T/$titulus.csr"
    fi
    "$OSSL" x509 -req -in "$T/$titulus.csr" -CA "$DEST/ca.pem" -CAkey "$T/ca.key" \
        -set_serial "$serie" -days 365 "-$md" -copy_extensions copy \
        -out "$DEST/$titulus.pem" 2>/dev/null
    "$OSSL" x509 -in "$DEST/$titulus.pem" -outform DER -out "$DEST/$titulus.der"
}

signare serie_alta  "DNS:localhost" 0x8001
signare serie_longa "DNS:localhost" 0x7F0102030405060708090A0B0C0D0E0F10111213
signare ip_solum    "IP:127.0.0.1,IP:::1" 0x2001
signare sine_san    "" 0x2002
CN=localhost signare geminus "DNS:localhost" 0x2003 sha256 "$DEST/localhost.key"
signare sha1        "DNS:localhost" 0x2004 sha1
GENUS_CLAVIS=rsa1024 signare rsa_parva "DNS:localhost" 0x2005

# ignotus: CA alia (non servata)
clavis "$T/alia.key"
"$OSSL" req -x509 -new -key "$T/alia.key" -sha256 -days 36500 \
    -subj "/CN=CA ignota" -out "$T/alia.pem" \
    -addext "basicConstraints=critical,CA:TRUE"
clavis "$DEST/ignotus.key"
"$OSSL" req -new -key "$DEST/ignotus.key" -subj "/CN=ignotus" \
    -addext "subjectAltName=DNS:localhost,IP:127.0.0.1" -out "$T/ignotus.csr"
"$OSSL" x509 -req -in "$T/ignotus.csr" -CA "$T/alia.pem" -CAkey "$T/alia.key" \
    -CAcreateserial -days 365 -copy_extensions copy \
    -out "$DEST/ignotus.pem" 2>/dev/null
"$OSSL" x509 -in "$DEST/ignotus.pem" -outform DER -out "$DEST/ignotus.der"

echo "tls_fixa: scripta in $DEST"
