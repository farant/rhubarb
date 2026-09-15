# reticulum_recensio.py - RECENSIO INDEPENDENS instrumenti reticuli partitionum (T35 e): ex ordinibus machinae cum ordine
# COLUMNAE eadem DEFINITIONE (oratio-spec par. 7 "Design - T35") ordines RETICULUM-COLUMNA, -SORS, -CATENA reddit - dictis
# et tuplis Pythonis, numquam instrumentum C vocans. Acceptatio: grep -P '\tRETICULUM-(COLUMNA|SORS|CATENA)\t' utriusque
# eadem esse debet (eadem vexilla, eadem via).
# Usus: reticulum_recensio.py <x.tsv> -genus K -aurum aurum-... -sortes columna [-ubi titulus=v1,v2]... [-columnae t1,t2]
#       [-praeter t1,t2] [-gradus t1,t2] [-limen N] [-lucrum N]   (-machina et -greges praetereuntur)
import sys
from collections import defaultdict


def optiones_legere(argv):
    o = {'ubi': [], 'columnae': [], 'praeter': [], 'gradus': [], 'limen': 20, 'lucrum': 0}
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
            elif a in ('-columnae', '-praeter', '-gradus'):
                o[a[1:]] += v.split(',')
            elif a in ('-limen', '-lucrum'):
                o[a[1:]] = int(v)
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
    n = len(claves)
    C, T, TS, TA = defaultdict(int), defaultdict(int), defaultdict(int), defaultdict(int)
    for g, a, s in zip(claves, aurum, sortes):
        C[g, a, s] += 1; T[g, a] += 1; TS[a, s] += 1; TA[a] += 1
    greges = ordo_primae_apparitionis(claves)
    per_sortem, suffragia = [], defaultdict(list)
    for s in sortium:
        basis = None
        for a in aurei:                                   # par -> index aurei minimus
            if basis is None or TA[a] - TS[a, s] > TA[basis] - TS[basis, s]:
                basis = a
        e = {'ordines': sum(TS[a, s] for a in aurei), 'recti': 0, 'basis': TS[basis, s], 'inaestimati': 0}
        for g in greges:
            disciplina = {a: T[g, a] - C[g, a, s] for a in aurei}
            in_sorte = sum(C[g, a, s] for a in aurei)
            if sum(disciplina.values()) >= limen:
                maxima = max(disciplina.values())
                victores = [a for a in aurei if disciplina[a] == maxima]
                votum = victores[0] if len(victores) == 1 else basis
                responsum = votum
                suffragia[g].append(votum)
            else:
                responsum = basis
                e['inaestimati'] += in_sorte
            e['recti'] += C[g, responsum, s]
        per_sortem.append(e)
    summa = {k: sum(e[k] for e in per_sortem) for k in ('ordines', 'recti', 'basis', 'inaestimati')}
    return {
        'greges': len(greges), 'sortes': per_sortem, 'summa': summa,
        'lucrum': summa['recti'] - summa['basis'],
        'intra': sum(max(T[g, a] for a in aurei) for g in greges) * 1000 // n if n else 0,
        'mutabiles': sum(1 for g in greges if len(set(suffragia[g])) > 1),
        'inaestimabilis': 2 * summa['inaestimati'] > summa['ordines'],
    }


def catena(valores_notarum, aurum, sortes, aurei, sortium, limen, lucrum_minimum, vetans):
    n, k = len(aurum), len(valores_notarum)
    currens = [()] * n
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
        gradus.append((accepta[0], accepta[2], aestimatio)); aestimatio = accepta[2]
    return initium, gradus


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
    if o['columnae']:
        notae = [index[t] for t in o['columnae'] if index[t] not in praeter]
    else:
        notae = [i for i, t in enumerate(tituli)
                 if i != index[o['sortes']] and not t.startswith('aurum-') and i not in praeter]
    aurum, sortes = valores(index[o['aurum']]), valores(index[o['sortes']])
    aurei, sortium = ordo_primae_apparitionis(aurum), ordo_primae_apparitionis(sortes)
    limen = o['limen']; lucrum_minimum = o['lucrum'] or limen
    valores_notarum = [valores(i) for i in notae]
    aestimationes = [aestimare(v, aurum, sortes, aurei, sortium, limen) for v in valores_notarum]
    ordo = sorted(range(len(notae)), key=lambda q: -aestimationes[q]['lucrum'])

    def scribe(genus, campi):
        print('\t'.join([via, genus] + [str(c) for c in campi]))

    print('\t'.join([via, 'COLUMNAE', 'RETICULUM-COLUMNA', 'columna', 'greges', 'ordines', 'recti', 'basis', 'lucrum',
                     'puritas', 'puritas-intra', 'inaestimati', 'mutabiles', 'cadentes', 'inaestimabilis']))
    for q in ordo:
        e = aestimationes[q]; sm = e['summa']
        scribe('RETICULUM-COLUMNA', [tituli[notae[q]], e['greges'], sm['ordines'], sm['recti'], sm['basis'], e['lucrum'],
                                     sm['recti'] * 1000 // sm['ordines'] if sm['ordines'] else 0, e['intra'],
                                     sm['inaestimati'], e['mutabiles'],
                                     sum(1 for x in e['sortes'] if x['recti'] < x['basis']), int(e['inaestimabilis'])])
    print('\t'.join([via, 'COLUMNAE', 'RETICULUM-SORS', 'columna', 'sors', 'ordines', 'recti', 'basis', 'inaestimati']))
    for q in ordo:
        for s, x in zip(sortium, aestimationes[q]['sortes']):
            scribe('RETICULUM-SORS', [tituli[notae[q]], s, x['ordines'], x['recti'], x['basis'], x['inaestimati']])
    print('\t'.join([via, 'COLUMNAE', 'RETICULUM-CATENA', 'vetans', 'gradus', 'columna', 'greges', 'recti', 'lucrum',
                     'cadentes']))
    for vetans in (1, 0):
        initium, gradus_catenae = catena(valores_notarum, aurum, sortes, aurei, sortium, limen, lucrum_minimum, vetans)
        scribe('RETICULUM-CATENA', [vetans, 0, '-', 1, initium['summa']['recti'], 0, 0])
        for g, (i, e, prior) in enumerate(gradus_catenae):
            scribe('RETICULUM-CATENA', [vetans, g + 1, tituli[notae[i]], e['greges'], e['summa']['recti'],
                                        e['summa']['recti'] - prior['summa']['recti'],
                                        sum(1 for x, y in zip(e['sortes'], prior['sortes']) if x['recti'] < y['recti'])])


principale()
