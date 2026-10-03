"""derivare_bibliothecas.py - lentes DERIVATAE inventarii 'bibliothecae (include)'.

Usus: python3 derivare_bibliothecas.py <radix arboris> > cellae.json

Pro omni include/*.h reddit cellas JSON (ad instrumentum tabularii
'inventarium', actus cellae, fons derivatum):
  plagulae - fontes implementationis: regula aedilis (lib/<basis>.c et
             lib/<basis>_macos|_posix.(c|m)) PLUS omnis plagula lib/ quae
             functionem a capite declaratam definit (build/nexus.tsv);
             definitiones PRIVATAE (linea ante nomen: interior, hic_manens,
             static) excluduntur - nexus.tsv nexum non notat, ergo homonyma
             privata alioquin ut definitiones capitis numerarentur.
             Plures plagulae: numerus functionum definitarum in parenthesi,
             ordine numeri descendente.
             Nulla: '(header only)'.
  mutatum  - 'AAAA-MM-DD hash' commissionis ultimae quae caput aut
             plagulam eius tangit (git log ad HEAD arboris).
Ordines novi (capita sine ordine in inventario) in stderr nominantur;
addendi sunt actu 'ordines' ante cellas.
"""
import json, os, re, subprocess, sys

radix = sys.argv[1]
os.chdir(radix)
capita = sorted('include/' + f for f in os.listdir('include') if f.endswith('.h'))

declaratae = {}   # caput -> set(functionum)
definitiones = {} # functio -> [plagulae lib/]
for linea in open('build/nexus.tsv'):
    p = linea.rstrip('\n').split('\t')
    if len(p) < 6 or p[1] != 'sedes' or p[2] != 'functio':
        continue
    titulus, via, linea_n = p[0], p[3], p[4]
    if via.startswith('include/') and via.endswith('.h'):
        declaratae.setdefault(via, set()).add(titulus)
    elif via.startswith('lib/') and (via.endswith('.c') or via.endswith('.m')):
        definitiones.setdefault(titulus, []).append((via, int(linea_n) if linea_n.isdigit() else 0))

memoria_linearum = {}
def privata(via, n):
    """Linea ante nomen (aut ipsa) 'interior'/'hic_manens'/'static' fert."""
    if via not in memoria_linearum:
        try:
            memoria_linearum[via] = open(via, errors='replace').read().split('\n')
        except OSError:
            memoria_linearum[via] = []
    L = memoria_linearum[via]
    prope = ' '.join(L[max(0, n - 3):n])
    return re.search(r'\b(interior|hic_manens|static)\b', prope) is not None

cellae = []
for caput in capita:
    basis = caput[len('include/'):-2]
    regula = [v for v in ('lib/%s.c' % basis, 'lib/%s_macos.c' % basis, 'lib/%s_macos.m' % basis,
                          'lib/%s_posix.c' % basis, 'lib/%s_posix.m' % basis) if os.path.exists(v)]
    numeri = {}
    for f in declaratae.get(caput, ()):
        for via, n in definitiones.get(f, ()):
            if not privata(via, n):
                numeri[via] = numeri.get(via, 0) + 1
    plagulae = sorted(set(regula) | set(numeri), key=lambda v: (-numeri.get(v, 0), v))
    if not plagulae:
        textus = '(header only)'
    elif len(plagulae) == 1:
        textus = plagulae[0]
    else:
        textus = ' '.join('%s (%d)' % (v, numeri.get(v, 0)) for v in plagulae)
    log = subprocess.run(['git', 'log', '-1', '--format=%ad %h', '--date=short', '--', caput] + plagulae,
                         capture_output=True, text=True).stdout.strip()
    cellae.append({'ordo': caput, 'lens': 'plagulae', 'valor': {'genus': 'textus', 'valor': textus}})
    if log:
        cellae.append({'ordo': caput, 'lens': 'mutatum', 'valor': {'genus': 'textus', 'valor': log}})

json.dump(cellae, sys.stdout, ensure_ascii=False)
sys.stderr.write('derivare_bibliothecas: %d capita, %d cellae\n' % (len(capita), len(cellae)))
