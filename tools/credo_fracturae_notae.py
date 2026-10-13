#!/usr/bin/env python3
"""tools/credo_fracturae_notae.py - CENSUS fracturarum exspectatarum
(credo II T2): sectiones quae 'exspectata_fracta' ferunt, ex REPOSITORIO
solo (principium …RBA4W73: quod exitum mutat in repositorio stat).

Ordines tabularum CredoSectio per silvam (./silva/selecta.sh: declaratio
generis CredoSectio, congeries interior = ordo) - commenta et macra
silva solvit; campus sextus = exspectata_fracta (chorda = causa).

Usus (ex radice):
  ./tools/credo_fracturae_notae.py [-tabularium] [VIA...]
    VIA       plagulae .c (ordinarium: probationes/ et */probationes/
              quae credo_suitam_currere vocant)
    -tabularium  RELATIO solum, numquam porta: id tabularii in causa
              (…XXXXXX aut ULID) per ./gesta/frigida.sh -res quaeritur;
              res clausa -> "fortasse sanata", ignota -> "id ignotum"
Effusio: '<via>:<linea>\\t<sectio>\\t<causa>' per fracturam, deinde
'fracturae exspectatae: N'. Exitus 0 (relatio), 2 usus."""
import glob
import os
import re
import subprocess
import sys

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SELECTOR = ('declaratio:habet(typus-nominatus[tok_titulus="CredoSectio"])'
            ' congeries congeries')


def campi(ordo):
    """ordo '{ a, "b" "c", NIHIL }' -> campi (chordae iunctae, verba
    nuda) - litterae adiacentes iunguntur ut C facit"""
    textus = ordo.strip()
    textus = textus[textus.index('{') + 1:textus.rindex('}')]
    out, cur, i = [], [], 0
    chorda = None
    while i < len(textus):
        ch = textus[i]
        if ch == '"':
            j = i + 1
            val = []
            while textus[j] != '"':
                if textus[j] == '\\':
                    val.append(textus[j:j + 2])
                    j += 2
                    continue
                val.append(textus[j])
                j += 1
            chorda = (chorda or '') + ''.join(val)
            i = j + 1
            continue
        if ch == ',':
            out.append(chorda if chorda is not None else ''.join(cur).strip())
            cur, chorda = [], None
            i += 1
            continue
        if chorda is None:
            cur.append(ch)
        i += 1
    if chorda is not None or ''.join(cur).strip():
        out.append(chorda if chorda is not None else ''.join(cur).strip())
    return out


def ordines(via):
    r = subprocess.run(['./silva/selecta.sh', via, SELECTOR], cwd=RADIX,
                       capture_output=True, text=True, errors='replace')
    for m in re.finditer(r'^\[([^\]\n]*):(\d+)-\d+\]\n(.*?)(?=^\[|\Z)', r.stdout,
                         re.S | re.M):
        yield m.group(1), int(m.group(2)), m.group(3)


def status_tabularii(ident):
    r = subprocess.run(['./gesta/frigida.sh', '-res', ident], cwd=RADIX,
                       capture_output=True, text=True, errors='replace')
    if 'res ignota' in r.stdout + r.stderr:
        return 'id ignotum'
    m = re.search(r'\(([a-z]+), ([a-z]+)\)', r.stdout)
    if not m:
        return 'status nescitus'
    if m.group(2) in ('clausum', 'impletum', 'relictum'):
        return '%s %s - fortasse sanata' % (m.group(1), m.group(2))
    return '%s %s' % (m.group(1), m.group(2))


def principale(argv):
    tabularium = '-tabularium' in argv
    viae = [a for a in argv if not a.startswith('-')]
    if any(a.startswith('-') and a != '-tabularium' for a in argv):
        print(__doc__.split('Usus')[1].split('Effusio')[0], file=sys.stderr)
        return 2
    if not viae:
        viae = sorted(glob.glob(os.path.join(RADIX, 'probationes', '*.c'))
                      + glob.glob(os.path.join(RADIX, '*', 'probationes',
                                               '*.c')))
        viae = [v for v in viae if 'credo_suitam_currere' in
                open(v, errors='replace').read()]
    n = 0
    for via in viae:
        for fons, linea, ordo in ordines(via):
            c = campi(ordo)
            if len(c) < 6 or c[0] in ('', 'NIHIL') or c[5] in ('', 'NIHIL'):
                continue
            n += 1
            rel = os.path.relpath(fons, RADIX) if fons.startswith(RADIX) \
                else fons
            print('%s:%d\t%s\t%s' % (rel, linea, c[0], c[5]))
            if tabularium:
                for ident in re.findall(r'(?:…|\.\.\.)([0-9A-Z]{6,26})', c[5]):
                    print('\t  tabularium %s: %s' % (ident,
                                                    status_tabularii(ident)))
    print('fracturae exspectatae: %d' % n)
    return 0


if __name__ == '__main__':
    sys.exit(principale(sys.argv[1:]))
