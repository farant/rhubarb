# contentiones.py - census contentionum (T32 b): ex ordinibus CONTENTIO unius tsv (metire.sh), per par (victor, victa)
# thesauris Latinis summatis: alternae, victae rectae (permille) - iudicis contentionum materia (quae regula cui cedere
# debeat); ordine numeri. Usus: contentiones.py <mensura.tsv> [plagula-pars]
import sys, collections
C = collections.defaultdict(lambda: [0, 0])
pars = sys.argv[2] if len(sys.argv) > 2 else None
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 6 or c[1] != 'CONTENTIO' or 'en_ewt' in c[0] or (pars and pars not in c[0]): continue
    C[(c[2], c[3])][0] += int(c[4]); C[(c[2], c[3])][1] += int(c[5])
def pm(a, b): return 1000 * a // b if b else 0
print('%-44s > %-44s %6s %6s' % ('victor', 'victa', 'n', 'victa'))
for (v, w), (n, r) in sorted(C.items(), key=lambda kv: -kv[1][0])[:60]:
    print('%-44s > %-44s %6d %6d' % (v, w, n, pm(r, n)))
