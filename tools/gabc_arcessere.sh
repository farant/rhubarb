#!/bin/bash
# tools/gabc_arcessere.sh - specimen GregoBase (plan gabc T9)
#
# Fons: GregoBaseCorpus v0.4 (bacor, 2020-07-22; exportatio GregoBase
# 2019-10-24), zip ad sha256 fixum. CC0: "All chant transcriptions are
# released under CC0" (gregobase.selapa.net, pagina 'About'); corpus
# quoque CC0. Editiones ipsae (Solesmes) iura sibi vindicant - ergo
# solum cantus quorum fontes OMNES anno <= MCMXXX impressi sunt.
#
#   (sine vexillo)  zip arcessit, sha256 probat, fasciculos manifesti
#                   (probationes/fixa/gabc/gregobase.manifestum) cum zip
#                   et cum commissis (probationes/fixa/gabc/gregobase/)
#                   confert. Exitus 0 congruunt, 1 differt.
#   -eligere        electionem iterat (deterministica: ordo
#                   sha256('gregobase-'+id) intra partem officii, quotae,
#                   deinde modus quisque >= V, clavis f >= VIII) et
#                   manifestum fasciculosque scribit.
#
# Fasciculi externi = data non fida: directorium proprium in
# build/gabc/, scriptum Python per 'python3 -I', nihil exsequitur.
set -euo pipefail
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
URL=https://github.com/bacor/gregobasecorpus/releases/download/v0.4/gregobasecorpus-v0.4.zip
ZIP_SHA=d417a98471ba7dfdf988c27ced8a6cf3ef95110d82b6f05b11296e935e9b5bf4
D="$RADIX/build/gabc/bacor_v0.4"

arcessere () {
    rm -rf "$D" && mkdir -p "$D/x"
    curl -sS -L -m 300 -o "$D/corpus.zip" "$URL"
    local h
    h=$(shasum -a 256 "$D/corpus.zip" | cut -d' ' -f1)
    if [ "$h" != "$ZIP_SHA" ]; then
        echo "GABC ARCESSERE: sha256 zip discrepat ($h)" >&2
        return 1
    fi
    if unzip -Z1 "$D/corpus.zip" | grep -qE '^/|(^|/)\.\.(/|$)'; then
        echo "GABC ARCESSERE: via periculosa in zip" >&2
        return 1
    fi
    unzip -q "$D/corpus.zip" 'csv/*' 'gabc/*' -d "$D/x"
}

modus=probare
[ "${1:-}" = "-eligere" ] && modus=eligere
[ $# -gt 0 ] && [ "$modus" != eligere ] && { echo "usus: $0 [-eligere]" >&2; exit 2; }
arcessere
python3 -I - "$RADIX" "$D/x" "$modus" "$ZIP_SHA" <<'PY'
import csv, sys, os, re, hashlib, collections, shutil
radix, X, modus, zsha = sys.argv[1:5]
MAN = os.path.join(radix, 'probationes/fixa/gabc/gregobase.manifestum')
FIX = os.path.join(radix, 'probationes/fixa/gabc/gregobase')
def via(i): return os.path.join(X, 'gabc', '%05d.gabc' % int(i))
def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()
if modus == 'probare':
    n = 0; mala = []
    for l in open(MAN, encoding='utf-8'):
        if l.startswith('DATA') or not l.strip(): continue
        f = l.rstrip('\n').split('\t'); nomen, h = f[0], f[1]
        z = os.path.join(X, 'gabc', nomen); c = os.path.join(FIX, nomen)
        if not os.path.exists(z) or sha(z) != h: mala.append(nomen + ' (zip)')
        elif not os.path.exists(c) or sha(c) != h: mala.append(nomen + ' (commissum)')
        n += 1
    if mala:
        print('GABC ARCESSERE: DIFFERT', len(mala), 'ex', n, ':', ', '.join(mala[:10])); sys.exit(1)
    print('GABC ARCESSERE: congruunt', n); sys.exit(0)
csv.field_size_limit(10**8)
src = {r['id']: r for r in csv.DictReader(open(os.path.join(X,'csv/sources.csv'), encoding='utf-8'))}
ch = {r['id']: r for r in csv.DictReader(open(os.path.join(X,'csv/chants.csv'), encoding='utf-8'))}
cs = collections.defaultdict(set)
for r in csv.DictReader(open(os.path.join(X,'csv/chant_sources.csv'), encoding='utf-8')):
    cs[r['chant_id']].add(r['source'])
def clef(g):
    m = re.search(r'\(([cf])b?[1-4]\)', g); return m.group(1) if m else '?'
def anni(i): return [int(src[s]['year']) for s in cs[i] if src[s]['year'].isdigit()]
pool = [i for i in ch if cs[i] and anni(i) and max(anni(i)) <= 1930 and os.path.exists(via(i))]
key = lambda i: hashlib.sha256(('gregobase-' + i).encode()).hexdigest()
QUOTAE = {'in': 8, 'gr': 8, 'al': 8, 'tr': 4, 'of': 8, 'co': 8, 'se': 3,
          'an': 12, 'hy': 8, 're': 4, 'va': 3, 'rb': 2, 'ps': 1}
sel, causa = [], {}
for op, q in QUOTAE.items():
    for i in sorted([i for i in pool if ch[i]['office_part'] == op], key=key)[:q]:
        sel.append(i); causa[i] = 'quota ' + op
def topup(pred, n, why):
    have = [i for i in sel if pred(i)]
    for i in sorted([i for i in pool if pred(i) and i not in sel], key=key)[:max(0, n - len(have))]:
        sel.append(i); causa[i] = why
for m in '12345678':
    topup(lambda i, m=m: ch[i]['mode'] == m, 5, 'modus ' + m)
topup(lambda i: clef(ch[i]['gabc']) == 'f', 8, 'clavis f')
os.makedirs(FIX, exist_ok=True)
for f in os.listdir(FIX):
    if f.endswith('.gabc'): os.remove(os.path.join(FIX, f))
with open(MAN, 'w', encoding='utf-8') as out:
    out.write('DATA gabc gregobase manifestum 1 (GregoBaseCorpus v0.4 bacor, zip sha256 %s; '
              'exportatio GregoBase 2019-10-24; CC0 1.0; fontes omnes <= 1930; tools/gabc_arcessere.sh; '
              'columnae: plagula sha256 pars modus clavis fontes causa incipit)\n' % zsha)
    for i in sorted(sel, key=int):
        nomen = '%05d.gabc' % int(i)
        shutil.copyfile(via(i), os.path.join(FIX, nomen))
        inc = re.sub(r'[\t\n\r]', ' ', ch[i]['incipit'])
        out.write('\t'.join([nomen, sha(via(i)), ch[i]['office_part'] or '-', ch[i]['mode'] or '-',
            clef(ch[i]['gabc']), '; '.join(src[s]['title'] + ' ' + src[s]['year'] for s in sorted(cs[i])),
            causa[i], inc]) + '\n')
print('GABC ARCESSERE: electi', len(sel), '-> manifestum et fasciculi scripti')
PY
