#!/bin/bash
# tools/musicxml_arcessere.sh - fontes externi MusicXML (plan musicxml P7/P8)
#
#   -suita     suita W3C (MIT) ad commissum fixum in build/musicxml/suite_tar/
#              arcessit, sha256 probat, cum probationes/fixa/musicxml/suite/
#              (INDEX) confert - fasciculi commissi iam adsunt; hoc
#              provenientiam renovat aut probat.
#   -silvestria  corpus silvestre (CC0): OpenScore Lieder et
#              StringQuartets, solum .mxl, ad commissa fixa, in
#              build/musicxml/silvestria/<repositorium>/<via>; manifestum
#              commissum (probationes/fixa/musicxml/silvestria.manifestum:
#              'sha256  repositorium/via') viam et sigillum dat - fasciculus
#              discrepans = FRACTUM. Fasciculi NUMQUAM committuntur.
#   -silvestria-novum  idem, sed index ex API GitHub et manifestum
#              (re)scribitur (semel, aut cum commissa mutantur).
#
# Fasciculi externi = data non fida: directorium proprium, nihil
# exsequitur. Exitus 0 sanum, 1 fractum, 2 usus.
set -euo pipefail
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SUITA_COMMISSUM=77c19f7e819154c70ca1a1992e80dcda8ff82fea
SUITA_SHA=5717b897bbc64e7f3b882618f0b60f421b36fe008f83867da42adeb32d620cd4

suita () {
    local d="$RADIX/build/musicxml/suite_tar"
    rm -rf "$d" && mkdir -p "$d/x"
    curl -sS -L -m 120 -o "$d/suite.tar.gz" \
        "https://codeload.github.com/w3c-cg/musicxmlTestSuite/tar.gz/$SUITA_COMMISSUM"
    local h
    h=$(shasum -a 256 "$d/suite.tar.gz" | cut -d' ' -f1)
    if [ "$h" != "$SUITA_SHA" ]; then
        echo "MUSICXML ARCESSERE: sha256 tarballi discrepat ($h)" >&2
        return 1
    fi
    tar -xzf "$d/suite.tar.gz" -C "$d/x"
    local fixa="$RADIX/probationes/fixa/musicxml/suite"
    if ( cd "$d"/x/*/xmlFiles && ls | LC_ALL=C sort | tr '\n' '\0' \
            | xargs -0 shasum -a 256 ) | diff -q - <(sed 1d "$fixa/INDEX") >/dev/null; then
        echo "MUSICXML ARCESSERE: suita congruit cum fixis commissis (183)"
    else
        echo "MUSICXML ARCESSERE: suita a fixis commissis DIFFERT" >&2
        return 1
    fi
}

LIEDER_COMMISSUM=38c5db510224d9facdc4b08d741fc788cfb58ea8
QUARTETA_COMMISSUM=2614b0b2958dd1ce3f52e172a43e5f4d0e29570f

silvestria () {
    local novum="$1"
    python3 -I - "$RADIX" "$novum" "$LIEDER_COMMISSUM" "$QUARTETA_COMMISSUM" <<'PY'
import sys, os, json, hashlib, urllib.request, urllib.parse
from concurrent.futures import ThreadPoolExecutor
radix, novum, lieder, quarteta = sys.argv[1], sys.argv[2] == '1', sys.argv[3], sys.argv[4]
repos = {'Lieder': lieder, 'StringQuartets': quarteta}
dest = os.path.join(radix, 'build/musicxml/silvestria')
manifestum = os.path.join(radix, 'probationes/fixa/musicxml/silvestria.manifestum')
def get(url):
    with urllib.request.urlopen(url, timeout=120) as r:
        return r.read()
if novum:
    items = []
    for repo, sha in repos.items():
        t = json.loads(get('https://api.github.com/repos/OpenScore/%s/git/trees/%s?recursive=1' % (repo, sha)))
        assert not t.get('truncated'), 'arbor truncata'
        items += [(repo, x['path'], None) for x in t['tree']
                  if x['type'] == 'blob' and x['path'].endswith('.mxl')]
else:
    items = []
    for line in open(manifestum):
        if line.startswith(('DATA', '#')) or not line.strip():
            continue
        h, rel = line.rstrip('\n').split('  ', 1)
        repo, path = rel.split('/', 1)
        items.append((repo, path, h))
def fetch(it):
    repo, path, h = it
    out = os.path.join(dest, repo, path)
    if not (os.path.exists(out) and h and hashlib.sha256(open(out, 'rb').read()).hexdigest() == h):
        url = 'https://raw.githubusercontent.com/OpenScore/%s/%s/%s' % (repo, repos[repo], urllib.parse.quote(path))
        data = get(url)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        with open(out, 'wb') as f:
            f.write(data)
    d = hashlib.sha256(open(out, 'rb').read()).hexdigest()
    return (repo + '/' + path, d, h is None or d == h)
with ThreadPoolExecutor(max_workers=8) as ex:
    res = list(ex.map(fetch, items))
mala = [r for r in res if not r[2]]
if novum:
    with open(manifestum, 'w') as f:
        f.write('DATA musicxml silvestria manifestum 1 (OpenScore Lieder %s, StringQuartets %s; CC0; sha256 et via; fasciculi in build/musicxml/silvestria/, numquam commissi)\n' % (lieder[:8], quarteta[:8]))
        for rel, d, _ in sorted(res):
            f.write('%s  %s\n' % (d, rel))
print('MUSICXML ARCESSERE: silvestria %d fasciculi, %d discrepantes' % (len(res), len(mala)))
for m in mala[:10]:
    print('  DISCREPAT ' + m[0])
sys.exit(1 if mala else 0)
PY
}

case "${1:-}" in
    -suita) suita ;;
    -silvestria) silvestria 0 ;;
    -silvestria-novum) silvestria 1 ;;
    *) echo "usus: $0 -suita | -silvestria | -silvestria-novum" >&2; exit 2 ;;
esac
