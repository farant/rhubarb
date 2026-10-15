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
#   <titulus>.der         omne folium etiam DER (fiducia_iudicare)
#
# CLAVES SOLUM PROBATIONIS - nihil verum signant.
#
# Usus: probationes/fixa/tls/generare.sh <directorium>
set -euo pipefail
DEST="${1:?directorium destinationis}"
OSSL="${OPENSSL:-/opt/homebrew/bin/openssl}"
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
