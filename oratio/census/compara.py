import sys
def lege(via):
    M = {}
    for l in open(via, encoding='utf-8'):
        c = l.rstrip('\n').split('\t')
        if len(c) < 3 or c[0].startswith('#'): continue
        f = c[0].split('/')[-1].replace('la_', '').replace('en_', '').replace('-ud-', ' ').replace('.conllu', '')
        d = M.setdefault(f, {})
        if c[1] == 'SUMMA': d['summa'] = [int(x) for x in c[2:]]
        elif c[1] == 'NOTA': d['nota:' + c[2]] = [int(x) for x in c[3:]]
        elif c[1] == 'LIGATIO': d['ligatio'] = [int(x) for x in c[2:]]
        elif c[1] == 'PARTITIO': d.setdefault('partitio', {})[c[2]] = (int(c[3]), int(c[4]))
    return M
def pm(a, b): return 1000 * a // b if b else 0
def coactae(d):
    p = d.get('partitio', {}); v = sum(p[k][0] for k in ('impletio', 'umbra') if k in p); r = sum(p[k][1] for k in ('impletio', 'umbra') if k in p); return r, v
A, B = lege(sys.argv[1]), lege(sys.argv[2])
print('%-14s %-9s %-9s %-9s %-9s %-9s %-13s %-9s' % ('file', 'primaria', 'coactae', 'casus', 'numerus', 'genus', 'genus(conv)', 'ligatio'))
for f in [x for x in A if x in B]:
    a, b = A[f], B[f]
    sa, sb = a['summa'], b['summa']
    ca, cb = coactae(a), coactae(b)
    ga, gb = a.get('nota:genus', [0, 0, 0]), b.get('nota:genus', [0, 0, 0])
    na, nb = a.get('nota:numerus', [0, 0, 0]), b.get('nota:numerus', [0, 0, 0])
    la, lb = a['ligatio'], b['ligatio']
    print('%-14s %3d>%-3d   %3d>%-3d   %3d>%-3d   %3d>%-3d   %3d>%-3d   %4d>%-4d      %3d>%-3d' % (f,
        pm(sa[2], sa[0]), pm(sb[2], sb[0]), pm(ca[0], ca[1]), pm(cb[0], cb[1]),
        pm(sa[7], sa[6]), pm(sb[7], sb[6]), pm(na[1], na[0]), pm(nb[1], nb[0]),
        pm(ga[1], ga[0]), pm(gb[1], gb[0]), ga[2] if len(ga) > 2 else 0, gb[2] if len(gb) > 2 else 0,
        pm(la[1], la[0]), pm(lb[1], lb[0])))
    print('%-14s   words: primaria %+d casus %+d numerus %+d genus %+d ligatio %+d/%+d' % ('', sb[2]-sa[2], sb[7]-sa[7], nb[1]-na[1], gb[1]-ga[1], lb[1]-la[1], lb[0]-la[0]))
