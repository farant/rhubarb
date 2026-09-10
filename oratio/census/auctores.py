# auctores.py - tabula fiduciae ex ordinibus AUCTOR unius tsv (metire.sh): per auctorem, thesauris Latinis summatis,
# verba decisa / primaria, ARCUS / rectae (T32 a: columnae XI-XIV), vicinae, plagula pessima; ordine praecisionis arcuum
import sys, collections
A = collections.defaultdict(lambda: collections.Counter()); W = {}
for l in open(sys.argv[1], encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) < 15 or c[1] != 'AUCTOR' or 'en_ewt' in c[0]: continue
    t = c[2]; f = c[0].split('/')[-1].replace('la_', '').replace('-ud-', ' ').replace('.conllu', '')
    n = [int(x) for x in c[3:15]]
    a = A[t]; a['verba'] += n[0]; a['primaria'] += n[1]; a['arcus'] += n[8]; a['rectae'] += n[9]; a['vicinae'] += n[10]; a['vicinae_rectae'] += n[11]
    p = 1000 * n[9] // n[8] if n[8] else None
    if p is not None and (t not in W or p < W[t][0]): W[t] = (p, f)
def pm(a, b): return 1000 * a // b if b else 0
print('%-46s %6s %5s  %6s %5s  %6s %5s  %s' % ('auctor', 'verba', 'prim', 'arcus', 'prae', 'vicin', 'prae', 'pessima'))
for t, a in sorted(A.items(), key=lambda kv: -pm(kv[1]['rectae'], kv[1]['arcus'])):
    print('%-46s %6d %5d  %6d %5d  %6d %5d  %s' % (t, a['verba'], pm(a['primaria'], a['verba']), a['arcus'], pm(a['rectae'], a['arcus']), a['vicinae'], pm(a['vicinae_rectae'], a['vicinae']), ('%d %s' % W[t]) if t in W else '-'))
