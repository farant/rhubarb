#!/bin/bash
# tools/psalmi_arcessere.sh - psalmi I-CL ex Divinum Officium in
# musica/psalmi/ (spec psalmodia VI q1, decretum tabularii
# 01M7BHHT1PHJTTDVQX1X5V70Y9)
#
# Fons: github.com/DivinumOfficium/divinum-officium, commissio fixa
# (COMMISSIO infra), web/www/horas/Latin/Psalterium/Psalmorum/
# PsalmN.txt - psalterium Vulgatae (Gallicanum), non Bea. Verba:
# Vulgata Sixto-Clementina (1592), publica; plagulae digitales sub
# licentia MIT proiecti - LICENSE eorum ut musica/psalmi/LICENTIA
# servatur.
#
#   (sine vexillo)  arcessit, convertit, cum musica/psalmi/ confert.
#                   Exitus 0 congruunt, 1 differt.
#   -scribere       musica/psalmi/N.txt (I-CL) et LICENTIA scribit.
#
# Conversio (forma eorum -> textus punctatus, include/psalmodia.h):
#   "N:V[a-z] " initio lineae omittitur (N = numerus plagulae);
#   numeri versuum Vulgatae intra lineam "(8)", "(2a)" omittuntur;
#   '‡' -> '†' (eorum horasscripts.pl: '‡' mediatio Breviarii recitati
#   sed flexa Antiphonalis - nos canimus); "(Aleph)" etc. (Ps 118) ->
#   linea commentarii "% Aleph" ante versum; "(fit reverentia)" ->
#   "% (fit reverentia)" ante versum; lineae "$ant" (Ps 94,
#   invitatorium) omittuntur. Linea alia ignota, aut signa reliqua
#   ( ) $ cifrae, aut '*' non semel, aut '†' post '*' = vitium, nihil
#   scribitur.
# Correctiones (CORRECTIONES infra): textus versus mutatus, linea
#   "% correctio: vetus -> novum (causa)" ante versum; locus qui non
#   semel invenitur = vitium (fons mutatus).
#
# Fasciculi externi = data non fida: directorium proprium in
# build/psalmi/, scriptum Python per 'python3 -I', nihil exsequitur.
set -euo pipefail
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
COMMISSIO=31d9e8b2e46aa38c63f7639e53582ffea8bd3a84
URL=https://raw.githubusercontent.com/DivinumOfficium/divinum-officium/$COMMISSIO
VIA=web/www/horas/Latin/Psalterium/Psalmorum
D="$RADIX/build/psalmi/do_${COMMISSIO:0:12}"

modus=probare
[ "${1:-}" = "-scribere" ] && modus=scribere
[ $# -gt 0 ] && [ "$modus" != scribere ] && { echo "usus: $0 [-scribere]" >&2; exit 2; }

rm -rf "$D" && mkdir -p "$D/x"
curl -sS -f -L -m 60 -o "$D/x/LICENSE" "$URL/LICENSE"
for n in $(seq 1 150); do
    curl -sS -f -L -m 60 -o "$D/x/Psalm$n.txt" "$URL/$VIA/Psalm$n.txt"
done

python3 -I - "$RADIX" "$D/x" "$modus" "$COMMISSIO" <<'PY'
import os, re, sys
radix, X, modus, commissio = sys.argv[1:5]
DEST = os.path.join(radix, 'musica/psalmi')
VERSUS = re.compile(r'^(\d+):(\d+)[a-z]? (.*)$')
HEBR = re.compile(r'^\(([A-Z][a-z]+)\) ')
NUM = re.compile(r'\s*\(\d+[a-z]?\)\s*')
vitia = []
# (psalmus, vetus, novum, causa) - textus versus convertendi
CORRECTIONES = [
    (104, 'ad Isaac:', 'ad Ísaac:',
     'vox hebraica sine acuto; ut Divinum Officium Latin-gabc canit,'
     ' non abrupte - Franus 2026-11-13, quaestio 01M7BJCHC94WEVPABP4Y5GVC4Q'),
]
usae = set()

def convertere(n, linea_cruda):
    exitus = []
    for k, l in enumerate(linea_cruda, 1):
        l = l.rstrip('\r\n')
        if l == '$ant' or not l.strip():
            continue
        m = VERSUS.match(l)
        if not m or int(m.group(1)) != n:
            vitia.append('Psalm%d.txt:%d: linea ignota: %s' % (n, k, l))
            continue
        t = m.group(3)
        h = HEBR.match(t)
        if h:
            exitus.append('% ' + h.group(1))
            t = t[h.end():]
        if '(fit reverentia)' in t:
            exitus.append('% (fit reverentia)')
            t = t.replace('(fit reverentia)', ' ')
        t = NUM.sub(' ', t).replace('‡', '†')
        t = re.sub(r'\s+', ' ', t).strip()
        if re.search(r'[()$0-9]', t) or t.count('*') != 1 \
                or t.count('†') > 1 or ('†' in t and t.index('†') > t.index('*')):
            vitia.append('Psalm%d.txt:%d: post conversionem: %s' % (n, k, t))
        for c, (ps, vetus, novum, causa) in enumerate(CORRECTIONES):
            if ps == n and vetus in t:
                if c in usae or t.count(vetus) != 1:
                    vitia.append('Psalm%d.txt:%d: correctio non semel: %s' % (n, k, vetus))
                usae.add(c)
                exitus.append('%% correctio: %s -> %s (%s)' % (vetus, novum, causa))
                t = t.replace(vetus, novum)
        exitus.append(t)
    versus = [v for v in exitus if not v.startswith('%')]
    incipit = versus[0].split('*')[0].strip().rstrip(',:;†').strip() if versus else ''
    caput = [
        '%% Psalmus %d (Vulgata) - %s' % (n, incipit),
        '%% Fons: Divinum Officium (commissio %s),' % commissio[:12],
        '%% %s/Psalm%d.txt; licentia MIT, vide LICENTIA.' % (
            'web/www/horas/Latin/Psalterium/Psalmorum', n),
        '% Per tools/psalmi_arcessere.sh conversum: textus punctatus',
        '% (include/psalmodia.h); Gloria Patri ab aptatore additur.',
    ]
    return '\n'.join(caput + exitus) + '\n'

plagulae = {}
for n in range(1, 151):
    with open(os.path.join(X, 'Psalm%d.txt' % n), encoding='utf-8') as f:
        plagulae['%d.txt' % n] = convertere(n, f.readlines())
with open(os.path.join(X, 'LICENSE'), encoding='utf-8') as f:
    plagulae['LICENTIA'] = f.read()
for c, (ps, vetus, novum, causa) in enumerate(CORRECTIONES):
    if c not in usae:
        vitia.append('Psalm%d.txt: correctio non inventa: %s' % (ps, vetus))
if vitia:
    for v in vitia:
        print('PSALMI ARCESSERE: ' + v, file=sys.stderr)
    sys.exit(1)
if modus == 'scribere':
    os.makedirs(DEST, exist_ok=True)
    for titulus_plagulae, textus in plagulae.items():
        with open(os.path.join(DEST, titulus_plagulae), 'w', encoding='utf-8') as f:
            f.write(textus)
    print('PSALMI ARCESSERE: %d plagulae scriptae in musica/psalmi/' % len(plagulae))
    sys.exit(0)
diversae = []
for titulus_plagulae, textus in plagulae.items():
    p = os.path.join(DEST, titulus_plagulae)
    if not os.path.exists(p) or open(p, encoding='utf-8').read() != textus:
        diversae.append(titulus_plagulae)
if diversae:
    print('PSALMI ARCESSERE: differunt %d: %s' % (len(diversae), ' '.join(diversae[:10])))
    sys.exit(1)
print('PSALMI ARCESSERE: congruunt (%d plagulae)' % len(plagulae))
PY
