import sys
def lege(via):
    M = {}
    for l in open(via, encoding='utf-8'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 2 and c[1] == 'NOTA':
            M.setdefault(c[0].split('/')[-1][:14], {})[c[2]] = (int(c[3]), int(c[4]), int(c[5]))
    return M
A, B = lege(sys.argv[1]), lege(sys.argv[2])
pm = lambda r, v: 1000 * r // v if v else 0
print('%-16s %-14s %-14s %-14s %-14s %-14s' % ('file', 'forma-verbi', 'vox', 'genus', 'numerus', 'persona'))
for f in A:
    if 'ewt' in f: continue
    print('%-16s %s' % (f, '  '.join('%3d>%-3d %+4d' % (pm(A[f][k][1], A[f][k][0]), pm(B[f][k][1], B[f][k][0]), B[f][k][1] - A[f][k][1]) for k in ('forma-verbi', 'vox', 'genus', 'numerus', 'persona') if k in A[f])))
