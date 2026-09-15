# reticulum_recensio.py - RECENSIO INDEPENDENS instrumenti reticuli partitionum (T35 e, T36 c): ex ordinibus machinae
# cum ordine COLUMNAE eadem DEFINITIONE (oratio-spec par. 7 "Design - T35", "Design - T36") ordines RETICULUM-COLUMNA,
# -SORS, -CATENA, -CONDICIO, -INFIMUM, -GREX, -GREX-SORS reddit - dictis et tuplis Pythonis, numquam instrumentum C
# vocans. Acceptatio: ordines horum generum utriusque (awk -F'\t' '$2 ~ /^RETICULUM-/') eadem esse debent (eadem
# vexilla, eadem via). TEGIT et DUPLEX (algebra) non recensentur: porta probatio_partitio ea tenet.
# Usus: reticulum_recensio.py <x.tsv> -genus K -aurum aurum-... (-sortes columna | -sortes-alternae N)
#       [-ubi titulus=v1,v2]... [-columnae t1,t2] [-praeter t1,t2] [-gradus t1,t2] [-limen N] [-lucrum N]
#       [-profunditas N] [-prima N] [-initium t1,t2] [-greges columna|catena|catena-libera|initium]   (-machina praeteritur)
import sys
from collections import defaultdict
from itertools import combinations

HABITUS_COACTUS, HABITUS_ORDINATUS, HABITUS_APERTUS = 'coactus', 'ordinatus', 'apertus'


def optiones_legere(argv):
    o = {'ubi': [], 'columnae': [], 'praeter': [], 'gradus': [], 'initium': [], 'limen': 20, 'lucrum': 0,
         'profunditas': 2, 'prima': 20, 'alternae': 0, 'greges': None, 'sortes': None}
    via, i = None, 0
    while i < len(argv):
        a = argv[i]
        if a == '-machina':
            i += 1
        elif not a.startswith('-'):
            via = a; i += 1
        else:
            v = argv[i + 1]
            if a in ('-genus', '-aurum', '-sortes', '-greges'):
                o[a[1:]] = v
            elif a == '-ubi':
                t, valores = v.split('=', 1); o['ubi'].append((t, set(valores.split(','))))
            elif a in ('-columnae', '-praeter', '-gradus', '-initium'):
                o[a[1:]] += v.split(',')
            elif a in ('-limen', '-lucrum', '-profunditas', '-prima'):
                o[a[1:]] = int(v)
            elif a == '-sortes-alternae':
                o['alternae'] = int(v)
            i += 2
    return via, o


def legere(via, genus):
    tituli, ordines = None, []
    for linea in open(via, encoding='utf-8', errors='surrogateescape'):
        linea = linea.rstrip('\n')
        if not linea or linea.startswith('#'):
            continue
        c = linea.split('\t')
        if len(c) > 2 and c[1] == 'COLUMNAE' and c[2] == genus:
            tituli = tituli or c[3:]
        elif len(c) > 1 and c[1] == genus:
            ordines.append(c[2:])
    return tituli, ordines


def ordo_primae_apparitionis(valores):
    visa = {}
    for v in valores:
        if v not in visa:
            visa[v] = len(visa)
    return list(visa)


def aestimare(claves, aurum, sortes, aurei, sortium, limen):
    """Iudicium retentum (T35 b) cum testimonio per gregem (T36 a): captivus, habitus, margo, retenti, recti."""
    n = len(claves)
    C, T, TS, TA = defaultdict(int), defaultdict(int), defaultdict(int), defaultdict(int)
    for g, a, s in zip(claves, aurum, sortes):
        C[g, a, s] += 1; T[g, a] += 1; TS[a, s] += 1; TA[a] += 1
    greges = ordo_primae_apparitionis(claves)
    totales = {g: sum(T[g, a] for a in aurei) for g in greges}
    per_sortem = []
    gregum = {g: {'captivus': None, 'sortes': []} for g in greges}
    for s in sortium:
        basis = None
        for a in aurei:                                   # par -> index aurei minimus
            if basis is None or TA[a] - TS[a, s] > TA[basis] - TS[basis, s]:
                basis = a
        e = {'ordines': sum(TS[a, s] for a in aurei), 'recti': 0, 'basis': TS[basis, s], 'inaestimati': 0}
        for g in greges:
            disciplina = {a: T[g, a] - C[g, a, s] for a in aurei}
            in_sorte = sum(C[g, a, s] for a in aurei)
            valores = sorted(disciplina.values(), reverse=True)
            margo = valores[0] - (valores[1] if len(valores) > 1 else 0)
            if in_sorte > 0 and in_sorte == totales[g]:
                gregum[g]['captivus'] = s
            if sum(disciplina.values()) >= limen:
                maxima = max(disciplina.values())
                victores = [a for a in aurei if disciplina[a] == maxima]
                votum = victores[0] if len(victores) == 1 else basis
                responsum = votum
            else:
                votum = None
                responsum = basis
                e['inaestimati'] += in_sorte
            recti_gregis = C[g, responsum, s]
            e['recti'] += recti_gregis
            gregum[g]['sortes'].append({'sors': s, 'disciplina': sum(disciplina.values()), 'suffragium': votum,
                                        'margo': margo, 'retenti': in_sorte, 'recti': recti_gregis})
        per_sortem.append(e)
    summa = {k: sum(e[k] for e in per_sortem) for k in ('ordines', 'recti', 'basis', 'inaestimati')}
    captivus_limine = False
    for g in greges:
        ps = gregum[g]['sortes']
        vota = [x['suffragium'] for x in ps if x['suffragium'] is not None]
        mutabilis = len(set(vota)) > 1
        apertus = mutabilis or any(x['suffragium'] is None or x['margo'] == 0 for x in ps)
        unanimis = all(x['margo'] == x['disciplina'] for x in ps)
        gregum[g]['mutabilis'] = mutabilis
        gregum[g]['habitus'] = HABITUS_APERTUS if apertus else (HABITUS_COACTUS if unanimis else HABITUS_ORDINATUS)
        gregum[g]['totales'] = totales[g]
        gregum[g]['numeri'] = {a: T[g, a] for a in aurei}
        if gregum[g]['captivus'] is not None and totales[g] >= limen:
            captivus_limine = True
    captivi = [g for g in greges if gregum[g]['captivus'] is not None]
    return {
        'greges': greges, 'gregum': gregum, 'sortes': per_sortem, 'summa': summa,
        'lucrum': summa['recti'] - summa['basis'],
        'intra': sum(max(T[g, a] for a in aurei) for g in greges) * 1000 // n if n else 0,
        'mutabiles': sum(1 for g in greges if gregum[g]['mutabilis']),
        'inaestimabilis': 2 * summa['inaestimati'] > summa['ordines'] or captivus_limine,
        'greges_captivi': len(captivi), 'ordines_captivi': sum(totales[g] for g in captivi),
    }


def catena(valores_notarum, aurum, sortes, aurei, sortium, limen, lucrum_minimum, vetans, semen):
    """Catena avida a semine (claves gradus 0) - vetans aut libera; gradus = (index columnae, claves, aestimatio, prior)."""
    n, k = len(aurum), len(valores_notarum)
    currens = list(semen)
    aestimatio = aestimare(currens, aurum, sortes, aurei, sortium, limen)
    initium, gradus, adhibitae = aestimatio, [], set()
    while len(adhibitae) < k:
        candidatae = []
        for i in range(k):
            if i in adhibitae:
                continue
            claves = [currens[r] + (valores_notarum[i][r],) for r in range(n)]
            candidatae.append((i, claves, aestimare(claves, aurum, sortes, aurei, sortium, limen)))
        candidatae.sort(key=lambda t: -t[2]['summa']['recti'])          # stabilis: par -> index minimus
        accepta = None
        for i, claves, e in candidatae:
            if e['summa']['recti'] - aestimatio['summa']['recti'] < lucrum_minimum:
                break
            if vetans and any(e['sortes'][s]['recti'] < aestimatio['sortes'][s]['recti'] for s in range(len(sortium))):
                continue
            accepta = (i, claves, e)
            break
        if accepta is None:
            break
        adhibitae.add(accepta[0]); currens = accepta[1]
        gradus.append((accepta[0], accepta[2], aestimatio, currens)); aestimatio = accepta[2]
    return initium, gradus, currens


def principale():
    via, o = optiones_legere(sys.argv[1:])
    tituli, ordines = legere(via, o['genus'])
    index = {t: i for i, t in enumerate(tituli)}
    ordines = [r for r in ordines if all(r[index[t]] in valores for t, valores in o['ubi'])]
    gradus = set(index[t] for t in o['gradus'])

    def valores(i):
        if i not in gradus:
            return [r[i] for r in ordines]
        return [v if v in ('0', '1') else 'plures' for v in (r[i] for r in ordines)]

    praeter = set(index[t] for t in o['praeter'])
    index_sortium = index[o['sortes']] if o['sortes'] is not None else None
    if o['columnae']:
        notae = [index[t] for t in o['columnae'] if index[t] not in praeter]
    else:
        notae = [i for i, t in enumerate(tituli)
                 if i != index_sortium and not t.startswith('aurum-') and i not in praeter]
    aurum = valores(index[o['aurum']])
    if o['alternae']:
        sortes = ['alterna-%d' % (r % o['alternae']) for r in range(len(ordines))]
    else:
        sortes = valores(index_sortium)
    aurei, sortium = ordo_primae_apparitionis(aurum), ordo_primae_apparitionis(sortes)
    limen = o['limen']; lucrum_minimum = o['lucrum'] or limen
    valores_notarum = [valores(i) for i in notae]
    aestimationes = [aestimare([(v,) for v in vals], aurum, sortes, aurei, sortium, limen) for vals in valores_notarum]
    ordo = sorted(range(len(notae)), key=lambda q: -aestimationes[q]['lucrum'])
    constantes = [q for q in range(len(notae)) if len(set(valores_notarum[q])) <= 1]
    liberae = [q for q in range(len(notae)) if q not in constantes]
    semen_columnae = [notae.index(index[t]) for t in o['initium']]
    semen = [tuple(valores_notarum[q][r] for q in semen_columnae) for r in range(len(ordines))]

    def scribe(genus, campi):
        print('\t'.join([via, genus] + [str(c) for c in campi]))

    def caput(genus, tituli_generis):
        print('\t'.join([via, 'COLUMNAE', genus] + tituli_generis))

    def titulus(q):
        return tituli[notae[q]]

    caput('RETICULUM-COLUMNA', ['columna', 'greges', 'ordines', 'recti', 'basis', 'lucrum', 'puritas', 'puritas-intra',
                                'inaestimati', 'mutabiles', 'cadentes-basi', 'inaestimabilis', 'greges-captivi',
                                'ordines-captivi'])
    for q in ordo:
        e = aestimationes[q]; sm = e['summa']
        scribe('RETICULUM-COLUMNA', [titulus(q), len(e['greges']), sm['ordines'], sm['recti'], sm['basis'], e['lucrum'],
                                     sm['recti'] * 1000 // sm['ordines'] if sm['ordines'] else 0, e['intra'],
                                     sm['inaestimati'], e['mutabiles'],
                                     sum(1 for x in e['sortes'] if x['recti'] < x['basis']), int(e['inaestimabilis']),
                                     e['greges_captivi'], e['ordines_captivi']])
    caput('RETICULUM-SORS', ['columna', 'sors', 'ordines', 'recti', 'basis', 'inaestimati'])
    for q in ordo:
        for s, x in zip(sortium, aestimationes[q]['sortes']):
            scribe('RETICULUM-SORS', [titulus(q), s, x['ordines'], x['recti'], x['basis'], x['inaestimati']])
    caput('RETICULUM-CATENA', ['vetans', 'gradus', 'columna', 'greges', 'recti', 'lucrum', 'cadentes-gradu'])
    catenae = {}
    for vetans in (1, 0):
        initium, gradus_catenae, ultimae = catena(valores_notarum, aurum, sortes, aurei, sortium, limen,
                                                  lucrum_minimum, vetans, semen)
        catenae[vetans] = (initium, gradus_catenae, ultimae)
        scribe('RETICULUM-CATENA', [vetans, 0, '+'.join(titulus(q) for q in semen_columnae) or '-',
                                    len(initium['greges']), initium['summa']['recti'], 0, 0])
        for g, (i, e, prior, _) in enumerate(gradus_catenae):
            scribe('RETICULUM-CATENA', [vetans, g + 1, titulus(i), len(e['greges']), e['summa']['recti'],
                                        e['summa']['recti'] - prior['summa']['recti'],
                                        sum(1 for x, y in zip(e['sortes'], prior['sortes']) if x['recti'] < y['recti'])])
    caput('RETICULUM-CONDICIO', ['gradus', 'columna', 'sors', 'recti', 'priores'])
    for g, (i, e, prior, _) in enumerate(catenae[0][1]):
        for s, x, y in zip(sortium, e['sortes'], prior['sortes']):
            if x['recti'] < y['recti']:
                scribe('RETICULUM-CONDICIO', [g + 1, titulus(i), s, x['recti'], y['recti']])

    # infima: subseta II..profunditas notarum non constantium; incrementum contra optimum serierum minorum
    caput('RETICULUM-INFIMUM', ['profunditas', 'columnae', 'greges', 'recti', 'incrementum', 'cadentes-basi',
                                'inaestimati'])
    recti_seriei = {(q,): aestimationes[q]['summa']['recti'] for q in range(len(notae))}
    infima = []
    for d in range(2, o['profunditas'] + 1):
        for combo in combinations(liberae, d):
            claves = [tuple(valores_notarum[q][r] for q in combo) for r in range(len(ordines))]
            e = aestimare(claves, aurum, sortes, aurei, sortium, limen)
            recti_seriei[combo] = e['summa']['recti']
            optimum = max(recti_seriei[combo[:j] + combo[j + 1:]] for j in range(d))
            infima.append((d, combo, len(e['greges']), e['summa']['recti'], e['summa']['recti'] - optimum,
                           sum(1 for x in e['sortes'] if x['recti'] < x['basis']), e['summa']['inaestimati']))
    for d in range(2, o['profunditas'] + 1):
        eius = [x for x in infima if x[0] == d]
        for x in sorted(eius, key=lambda x: -x[4])[:o['prima']]:      # stabilis: par -> ordo enumerationis
            scribe('RETICULUM-INFIMUM', [d, '+'.join(titulus(q) for q in x[1]), x[2], x[3], x[4], x[5], x[6]])

    # testimonium gregum (T36 c)
    caput('RETICULUM-GREX', ['partitio', 'grex', 'ordines', 'aurum-maximum', 'aurum-maximum-ordines', 'puritas',
                             'captivus', 'habitus', 'mutabilis', 'sortes-suffragantes', 'retenti', 'recti'])
    electa = None
    if o['greges'] in ('catena', 'catena-libera', 'initium'):
        initium, gradus_catenae, ultimae = catenae[1 if o['greges'] != 'catena-libera' else 0]
        if o['greges'] == 'initium' or not gradus_catenae:
            electa = (semen, initium)
        else:
            electa = (ultimae, gradus_catenae[-1][1])
    elif o['greges'] is not None:
        q = notae.index(index[o['greges']])
        electa = ([(v,) for v in valores_notarum[q]], aestimationes[q])
    ordines_gregum = []
    if electa is not None:
        claves, e = electa
        for g in e['greges']:
            gr = e['gregum'][g]
            ordines_gregum.append((g, gr))
            aureus = max(aurei, key=lambda a: (gr['numeri'][a], -aurei.index(a)))
            scribe('RETICULUM-GREX', [o['greges'], '+'.join(g), gr['totales'], aureus, gr['numeri'][aureus],
                                      gr['numeri'][aureus] * 1000 // gr['totales'],
                                      gr['captivus'] if gr['captivus'] is not None else '-', gr['habitus'],
                                      int(gr['mutabilis']), sum(1 for x in gr['sortes'] if x['suffragium'] is not None),
                                      sum(x['retenti'] for x in gr['sortes']), sum(x['recti'] for x in gr['sortes'])])
    caput('RETICULUM-GREX-SORS', ['partitio', 'grex', 'sors', 'disciplina', 'suffragium', 'margo', 'retenti', 'recti'])
    for g, gr in ordines_gregum:
        for x in gr['sortes']:
            scribe('RETICULUM-GREX-SORS', [o['greges'], '+'.join(g), x['sors'], x['disciplina'],
                                           x['suffragium'] if x['suffragium'] is not None else '-', x['margo'],
                                           x['retenti'], x['recti']])


principale()
