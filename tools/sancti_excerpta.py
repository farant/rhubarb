"""tools/sancti_excerpta.py - excerpta plagularum .sanctus in fontibus
probare (project-specs/sancti-plan.md par. I.1-3). NON porta: speculum
fontium extra repositorium (../sancti-fontes) postulat.

COMPARATIO PER VERBA: minusculae, accentus remoti, lineae in fine
divisae ('be-\\ntween') iunctae, deinde litterae et digiti SOLI (spatia,
interpunctio, signa OCR errantia omissa). Excerptum verba fontis
reddere debet - errores OCR inclusi ('AT Home' pro 'At Rome'); prosa
descriptionis recte scribitur.

Fontes: mr1916 butler1812 (locus 'vNN...') ce1913 (locus = titulus
articuli; capitur si deest) usccb2026 (fixtura repositorii) sedes (url
capitur, HTML in textum).

Usus:
  python3 -I tools/sancti_excerpta.py probare PLAGULA...
  python3 -I tools/sancti_excerpta.py quaerere OPUS LOCUS "verba..."
Exitus: 0 omnia inventa · 1 excerptum aliquod absens aut fons absens ·
        2 usus pravus
Ambitus: SANCTI_FONTES (speculum; ordinarium ../sancti-fontes)
"""
import hashlib
import html
import os
import re
import subprocess
import sys
import unicodedata

RADIX = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
SPECULUM = os.environ.get('SANCTI_FONTES') or os.path.join(
    RADIX, '..', 'sancti-fontes')
MINIMUM = 20   # litterae normatae: excerptum brevius nihil probat
FONS_RE = re.compile(r'<fons\b([^>]*)>(.*?)</fons>', re.S)
ATTR_RE = re.compile(r'([a-z_-]+)="([^"]*)"')
EXC_RE = re.compile(r'<excerptum>(.*?)</excerptum>', re.S)
_textus = {}


def normare(t):
    t = re.sub(r'-\s*\n\s*', '', t)
    t = unicodedata.normalize('NFKD', t)
    t = ''.join(c for c in t if not unicodedata.combining(c))
    return re.sub(r'[^a-z0-9]', '', t.lower())


def capere_ce(locus):
    via = os.path.join(SPECULUM, 'ce1913',
                       locus.replace(' ', '_').replace('/', '~') + '.txt')
    if not os.path.exists(via):
        subprocess.run([os.path.join(RADIX, 'tools',
                                     'sancti_fontes_capere.sh'), 'ce',
                        locus], capture_output=True, cwd=RADIX)
    return via


def capere_url(url):
    d = os.path.join(SPECULUM, 'sedes')
    via = os.path.join(d, hashlib.sha1(url.encode()).hexdigest()[:16]
                       + '.txt')
    if not os.path.exists(via):
        os.makedirs(d, exist_ok=True)
        r = subprocess.run(['curl', '-sfL', '-m', '120', url],
                           capture_output=True)
        if r.returncode != 0 or not r.stdout:
            return None
        sys.path.insert(0, os.path.join(RADIX, 'tools'))
        from sancti_html_textus import textus_ex_html
        with open(via, 'w', encoding='utf-8') as f:
            f.write(url + '\n\n' + textus_ex_html(
                r.stdout.decode('utf-8', 'replace')) + '\n')
    return via


def via_fontis(opus, locus, url):
    if opus == 'mr1916':
        return os.path.join(SPECULUM, 'mr1916', 'mr1916.txt')
    if opus == 'butler1812':
        m = re.match(r'v(\d\d)', locus)
        return m and os.path.join(SPECULUM, 'butler1812',
                                  'v%s.txt' % m.group(1))
    if opus == 'ce1913':
        return capere_ce(locus)
    if opus == 'usccb2026':
        return os.path.join(RADIX, 'probationes', 'fixa', 'liturgia',
                            'usccb_sanctorale.tsv')
    if opus == 'sedes' and url:
        return capere_url(url)
    return None


def textus_normatus(via):
    if via not in _textus:
        with open(via, encoding='utf-8', errors='replace') as f:
            _textus[via] = normare(f.read())
    return _textus[via]


def probare(plagulae):
    fracta = 0
    numerus = 0
    for p in plagulae:
        with open(p, encoding='utf-8') as f:
            textus = f.read()
        for attrs, corpus in FONS_RE.findall(textus):
            a = dict(ATTR_RE.findall(attrs))
            opus, locus = a.get('opus', ''), a.get('locus', '')
            via = via_fontis(opus, locus, html.unescape(a.get('url', '')))
            for e in EXC_RE.findall(corpus):
                numerus += 1
                e = html.unescape(' '.join(e.split()))
                n = normare(e)
                titulus = '%s [%s %s]' % (p, a.get('id', '?'), opus)
                if opus == 'sedes' and not a.get('url'):
                    continue   # fixtura sine url: non probatur
                if not via or not os.path.exists(via):
                    print('FONS ABSENS\t%s\t%s' % (titulus, locus))
                    fracta += 1
                elif len(n) < MINIMUM and opus != 'usccb2026':
                    print('BREVIUS\t%s\t%s' % (titulus, e[:80]))
                    fracta += 1
                elif n not in textus_normatus(via):
                    print('ABSENS\t%s\t%s' % (titulus, e[:100]))
                    fracta += 1
    print('excerpta: %d probata, %d fracta' % (numerus, fracta))
    return 1 if fracta else 0


def quaerere(opus, locus, verba):
    via = via_fontis(opus, locus, locus if opus == 'sedes' else '')
    if not via or not os.path.exists(via):
        print('FONS ABSENS: %s %s' % (opus, locus))
        return 1
    with open(via, encoding='utf-8', errors='replace') as f:
        lineae = f.read().split('\n')
    acus = normare(verba)
    inventa = 0
    for i in range(len(lineae)):
        fenestra = '\n'.join(lineae[i:i + 6])
        if acus in normare(fenestra) and (
                i == 0 or acus not in normare('\n'.join(lineae[i - 1:i + 5]))):
            inventa += 1
            print('--- %s:%d' % (via, i + 1))
            print('\n'.join(lineae[max(0, i - 2):i + 8]))
            if inventa >= 5:
                break
    if not inventa:
        print('nihil inventum')
    return 0 if inventa else 1


def main():
    if len(sys.argv) >= 3 and sys.argv[1] == 'probare':
        sys.exit(probare(sys.argv[2:]))
    if len(sys.argv) == 5 and sys.argv[1] == 'quaerere':
        sys.exit(quaerere(sys.argv[2], sys.argv[3], sys.argv[4]))
    sys.stderr.write(__doc__)
    sys.exit(2)


main()
