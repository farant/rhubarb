import collections, sys
def lege(via):
    C = collections.Counter()
    for l in open(via, encoding='utf-8'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 2 and c[1] == 'ERRATUM-NOTA': C[(c[2], c[8], c[4], c[5], c[7] or '-')] += int(c[12])
    return C
A, B = lege(sys.argv[1]), lege(sys.argv[2])
for acc in sys.argv[3:]:
    D = collections.Counter()
    for k in set(A) | set(B):
        if k[0] == acc: D[k] = B.get(k, 0) - A.get(k, 0)
    print('=== %s errata nova' % acc)
    for k, v in sorted(D.items(), key=lambda kv: -kv[1])[:8]:
        if v > 0: print('   %+3d  %-12s %s -> %s  %s' % (v, k[1], k[2], k[3], k[4]))
    print('    sublata: %s' % ' '.join('%s %+d' % (k[1], v) for k, v in sorted(D.items(), key=lambda kv: kv[1])[:6] if v < 0))
