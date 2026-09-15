#!/usr/bin/env python3
# pondera.py - tabula ponderum decretoris (T38 b, 2026-09-15; spec par. 2
# decisiones LVII-LVIII): ex ordinibus LIS (metire.sh x.tsv -lites)
# plagularum EDITIBILIUM solarum (la_circse, la_llct) contentiones et
# rectae per (regula, dialectus, forma, folliculus, valor).
#
#   python3 oratio/census/pondera.py <lites.tsv>                 -> stdout
#   python3 oratio/census/pondera.py <lites.tsv> -scribere [out] -> fixa/pondera.tsv
#
# LEX CREDITI: contentio iudicata = ordo LIS cuius aurum-arcus non ignotum
# (nec arcus idem cum lectione ignota: contentio lectionis iniudicabilis);
# latus utrumque (victor, victa) contentionem unam accipit; latus RECTUM si
# arcus eius aureus (aurum-arcus latus nominat aut 'ambo', aut 'idem' cum
# aurum-caput-idem 1) ET lectio eius non falsa (aurum-lectio latus nominat,
# 'ambo' aut 'ignotum'). Claves: folliculi (contextus declarati + '-'),
# regula plana (contextus IV: ambo, dialectus, forma, '-'), structura
# '#relatio+ante|post+proximo|remoto' ex columnis lateris ipsius.
import sys, os, datetime, subprocess

EDITIBILES = ('la_circse', 'la_llct')
CAPUT = ('regula', 'dialectus', 'forma', 'folliculus', 'valor',
         'contentiones', 'rectae', 'permille')
CAPUT_LIS = ('victor', 'victa', 'ante', 'aurum-caput-idem',
             'caput-victoris-idem', 'numerus-capitis', 'numerus-victae',
             'numerus-victoris', 'thesaurus', 'aurum-arcus', 'aurum-lectio',
             'relatio-victae', 'relatio-victoris', 'ante-victoris',
             'distantia-victae', 'distantia-victoris', 'dialectus', 'forma')

# FOLLICULI = DATA (decisio LVIII): nomen, valor(latus) -> chorda aut None,
# relationes, contextus quibus condicionatur, causa cum numeris reticuli.
def _valor_numeri_capitis(l):
    if not l['caput_idem']:
        return None
    return l['numerus_capitis'] if l['numerus_capitis'] != '-' else None

def _valor_positionis_numeri(l):
    if l['numerus'] == '-':
        return None
    return '%s+%s' % (l['ante'], l['numerus'])

def _valor_directionis_distantiae(l):
    if l['distantia'] <= 0:
        return None
    return '%s+%s' % ('ante' if l['ante'] == '1' else 'post',
                      gradus_distantiae(l['distantia']))

FOLLICULI = [
    # (nomen, valor, relationes, contextus, activus, causa) - VERDICTA
    # RETICULI 2026-09-15 super lites_t38a (columna forma adest):
    ('numerus-capitis', _valor_numeri_capitis,
     ('subiectum', 'obiectum-verbi'), ('dialectus',), False,
     'T35 e / T36 d: catena vetans sortibus alternis +75, la_llct cadit 12 -> dialecto condicionatus; T38 d RECUSATUS: cursu decretoris ordo classicus = sors CIRCSE (subiectum 294 contra obiectum 189) prosae classicae impositus - eo omisso casus sursum omnibus plagulis Latinis (CIRCSE +13 contra +5 verba, ITTB +34 contra +14, Perseus +23 contra +6, PROIEL +12 contra 0, LLCT test +42 contra +32); folliculus verus, tabula falsa: prosa classica editibilis deest (decisio LVII)'),
    ('positio-numerus', _valor_positionis_numeri,
     ('subiectum', 'obiectum-verbi'), ('dialectus',), True,
     'T38 b: par ante x numerus-victae infimum +79 retentum, lucrantes 5, cadit 1 (la_llct 1+pluralis 2/8 contra victorem) -> dialecto condicionatus ut numerus verbi; FORMA RECUSATA: intra versum CIRCSE et Perseus in triplice flectunt (1+pluralis+singularis CIRCSE 47:29 victor, Perseus 18:7 victa) - forma corpora non separat'),
    ('directio-distantia', _valor_directionis_distantiae,
     ('caput',), ('forma',), False,
     'T38 b RECUSATUS: arcus capitis (2,061 contentiones, relatio-victoris=caput) par ante-victoris x distantia-victoris cadit 3 ex 6 sortibus, greges 1+1 et 0+plures aperti MUTABILES (CIRCSE contra ceteros, la_llct contra ceteros) - nullus contextus declaratus ambos separat; columna forma ipsa -20'),
]

def gradus_distantiae(d):
    return '1' if d == 1 else 'plures'   # lex -gradus reticuli

def legere(via):
    tituli = None
    ordines = []
    with open(via, encoding='utf-8') as f:
        for linea in f:
            c = linea.rstrip('\n').split('\t')
            if len(c) < 3:
                continue
            if c[1] == 'COLUMNAE' and c[2] == 'LIS':
                if tituli is None:
                    tituli = c[3:]
                continue
            if c[1] == 'LIS':
                if tituli is None:
                    sys.stderr.write('pondera: ordo LIS ante caput COLUMNAE\n'); sys.exit(2)
                if len(c) - 2 != len(tituli):
                    sys.stderr.write('pondera: latitudo ordinis LIS %d pro %d\n' % (len(c) - 2, len(tituli))); sys.exit(2)
                ordines.append(dict(zip(tituli, c[2:])))
    if tituli is None:
        sys.stderr.write('pondera: caput COLUMNAE LIS absens\n'); sys.exit(2)
    for t in CAPUT_LIS:
        if t not in tituli:
            sys.stderr.write('pondera: columna absens: %s\n' % t); sys.exit(2)
    return ordines

def _numerus(c):
    try:
        return int(c)
    except ValueError:
        return 0

def latera(r):
    """latus utrumque contentionis iudicatae: dict cum regula, relatio, ante,
    distantia, numerus (lectionis lateris), numerus_capitis, caput_idem,
    rectum; [] si contentio non iudicata (aurum-arcus ignotum)"""
    arcus = r['aurum-arcus']; lectio = r['aurum-lectio']
    # non iudicata: arcus ignotus, aut arcus idem cum lectione ignota
    # (contentio lectionis quam aurum iudicare nequit)
    if arcus == 'ignotum' or (arcus == 'idem' and lectio == 'ignotum'):
        return []
    exitus = []
    for latus, suf, ante, dist, num in (('victor', 'victoris', 'ante-victoris', 'distantia-victoris', 'numerus-victoris'),
                                        ('victa', 'victae', 'ante', 'distantia-victae', 'numerus-victae')):
        arcus_aureus = arcus in (latus, 'ambo') or (arcus == 'idem' and r['aurum-caput-idem'] == '1')
        lectio_bona = lectio in (latus, 'ambo', 'ignotum')
        exitus.append({
            'regula': r[latus], 'relatio': r['relatio-' + suf], 'ante': r[ante],
            'distantia': _numerus(r[dist]), 'numerus': r[num],
            'numerus_capitis': r['numerus-capitis'],
            'caput_idem': latus == 'victa' or r['caput-victoris-idem'] == '1',
            'rectum': arcus_aureus and lectio_bona})
    return exitus

def structura(l):
    if l['relatio'] == '-' or l['distantia'] <= 0:
        return None
    return '@%s+%s+%s' % (l['relatio'], 'ante' if l['ante'] == '1' else 'post',
                          'proximo' if l['distantia'] == 1 else 'remoto')

def variantes(d, f, dimensiones):
    v = [('-', '-')]
    if 'dialectus' in dimensiones:
        v.append((d, '-'))
    if 'forma' in dimensiones:
        v.append(('-', f))
    if 'dialectus' in dimensiones and 'forma' in dimensiones:
        v.append((d, f))
    return v

def numerare(ordines):
    summae = {}
    def credere(clavis, rectum):
        s = summae.setdefault(clavis, [0, 0])
        s[0] += 1
        if rectum:
            s[1] += 1
    for r in ordines:
        d, f = r['dialectus'], r['forma']
        for l in latera(r):
            for dc, fc in variantes(d, f, ('dialectus', 'forma')):
                credere((l['regula'], dc, fc, '-', '-'), l['rectum'])
                st = structura(l)
                if st is not None:
                    credere((st, dc, fc, '-', '-'), l['rectum'])
            for nomen, valor, relationes, dimensiones, activus, causa in FOLLICULI:
                if not activus or l['relatio'] not in relationes:
                    continue
                v = valor(l)
                if v is None:
                    continue
                for dc, fc in variantes(d, f, dimensiones):
                    credere((l['regula'], dc, fc, nomen, v), l['rectum'])
    return summae

def lineae(summae):
    exitus = ['\t'.join(CAPUT)]
    for clavis in sorted(summae):
        n, rectae = summae[clavis]
        exitus.append('\t'.join(clavis) + '\t%d\t%d\t%d' % (n, rectae, rectae * 1000 // n))
    return exitus

def principale(argv):
    if len(argv) < 2:
        sys.stderr.write('usus: pondera.py <lites.tsv> [-scribere [out]]\n'); return 2
    via = argv[1]
    scribere = len(argv) > 2 and argv[2] == '-scribere'
    exitus_via = argv[3] if scribere and len(argv) > 3 else 'oratio/probationes/fixa/pondera.tsv'
    ordines = [r for r in legere(via) if r['thesaurus'] in EDITIBILES]
    praesentes = set(r['thesaurus'] for r in ordines)
    absentes = [t for t in EDITIBILES if t not in praesentes]
    if absentes:
        sys.stderr.write('pondera: plagulae editibiles absunt: %s\n' % ' '.join(absentes)); return 2
    textus = lineae(numerare(ordines))
    if scribere:
        try:
            fons = subprocess.run(['git', 'rev-parse', '--short', 'HEAD'], capture_output=True, text=True).stdout.strip()
        except OSError:
            fons = '?'
        caput = ('# pondera.tsv - tabula ponderum decretoris (T38 b, %s): contentiones et rectae per (regula, dialectus, forma, '
                 'folliculus, valor) ex ordinibus LIS plagularum EDITIBILIUM (la_circse, la_llct) solis; scrinium numquam; '
                 'regeneratio: python3 oratio/census/pondera.py <lites.tsv> -scribere (causa nominata); fons %s %s'
                 % (datetime.date.today().isoformat(), fons, os.path.basename(via)))
        with open(exitus_via, 'w', encoding='utf-8') as f:
            f.write(caput + '\n' + '\n'.join(textus) + '\n')
        sys.stderr.write('pondera: %s scripta (%d ordines)\n' % (exitus_via, len(textus) - 1))
    else:
        sys.stdout.write('\n'.join(textus) + '\n')
    return 0

if __name__ == '__main__':
    sys.exit(principale(sys.argv))
