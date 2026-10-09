#!/usr/bin/env python3
"""tools/uuidtext_census.py - census /private/var/db/uuidtext (macOS
log string cache) et scriptum purgationis.

CUR (2026-10-09): macOS pro OMNI aedificatione distincta processus qui
in systema log scribit plagulam in uuidtext servat - sectionem
__cstring binarii (~3.3 MB pro probatione radicis: compile_tests.sh
omnia obiecta + Cocoa/Security/WebKit in omnem probationem nectit;
framework onerata log scribit; tabulae nodorum ut chordae). 37 GB uno
die per quinque arbores, umbras, frigida. Lineae log harum probationum
nobis inutiles sunt (credo per stdout et plagulas loquitur).

Sine sudo: plagulae legibiles; via binarii = chorda ULTIMA (NUL
terminata) plagulae, ergo cauda sola legitur. NOSTRA = binaria ex
arboribus rhubarb*, ~/.rhubarb (umbrae), plicis temporariis fumorum et
frigidae, scratch Claudii. Cetera numquam tanguntur.

Usus: ./tools/uuidtext_census.py [-scriptum VIA]
  imprimit census; scribit scriptum purgationis (ordinarium
  build/uuidtext_purgare.sh) quod Franus per 'sudo sh VIA' currit.
Exitus 0."""
import os
import re
import sys
import time

RADIX_CACHE = '/private/var/db/uuidtext'
DOMUS = os.path.expanduser('~')
PRAEFIXA_NOSTRA = (
    DOMUS + '/Documents/projects/rhubarb',     # rhubarb, rhubarb-secunda...
    DOMUS + '/.rhubarb/',                       # umbrae (commissio_umbra)
    '/private/tmp/claude-',                     # scratch Claudii
)
# plicae temporariae (mktemp sub $TMPDIR): fumi et frigida
TEMPORARIA = re.compile(r'^/private/var/folders/[^/]+/[^/]+/T/'
                        r'(frigida|credo_fumus|annales_fumus|tmp)\.[^/]+/')


def via_binarii(plagula):
    """chorda ultima NUL-terminata caudae (via imaginis); '' si nulla"""
    try:
        with open(plagula, 'rb') as h:
            h.seek(0, 2)
            n = h.tell()
            h.seek(max(0, n - 4096))
            cauda = h.read()
    except OSError:
        return ''
    partes = cauda.rstrip(b'\x00').split(b'\x00')
    if not partes:
        return ''
    ultima = partes[-1].decode('utf-8', 'replace')
    return ultima if ultima.startswith('/') else ''


def est_nostra(via):
    if via.startswith(PRAEFIXA_NOSTRA):
        return True
    return bool(TEMPORARIA.match(via))


def arbor(via):
    """clavis summarii: arbor rhubarb, umbrae, temporaria, scratch"""
    m = re.match(re.escape(DOMUS) + r'/Documents/projects/(rhubarb[^/]*)', via)
    if m:
        return m.group(1)
    if via.startswith(DOMUS + '/.rhubarb/'):
        return '~/.rhubarb (umbrae)'
    if via.startswith('/private/tmp/claude-'):
        return 'scratch Claudii'
    return 'temporaria (fumi, frigida)'


def gb(n):
    return '%.1f GB' % (n / 1e9)


def principale():
    scriptum = 'build/uuidtext_purgare.sh'
    if '-scriptum' in sys.argv:
        scriptum = sys.argv[sys.argv.index('-scriptum') + 1]
    nunc = time.time()
    omnia_n = omnia_b = 0
    nostra = []                 # (via plagulae, magnitudo, mtime)
    per_arborem = {}
    for d in sorted(os.listdir(RADIX_CACHE)):
        plica = os.path.join(RADIX_CACHE, d)
        if not os.path.isdir(plica):
            continue
        for f in os.listdir(plica):
            p = os.path.join(plica, f)
            try:
                st = os.stat(p)
            except OSError:
                continue
            omnia_n += 1
            omnia_b += st.st_size
            via = via_binarii(p)
            if via and est_nostra(via):
                nostra.append((p, st.st_size, st.st_mtime))
                a = per_arborem.setdefault(arbor(via), [0, 0])
                a[0] += 1
                a[1] += st.st_size
    nostra_b = sum(x[1] for x in nostra)
    hora = [x for x in nostra if nunc - x[2] < 3600]
    dies = [x for x in nostra if nunc - x[2] < 86400]
    print('UUIDTEXT: %s in %d plagulis; NOSTRA %s in %d (%.0f%%)'
          % (gb(omnia_b), omnia_n, gb(nostra_b), len(nostra),
             100.0 * nostra_b / omnia_b if omnia_b else 0))
    print('incrementum nostrum: hora ultima %s (%d), dies ultimus %s (%d)'
          % (gb(sum(x[1] for x in hora)), len(hora),
             gb(sum(x[1] for x in dies)), len(dies)))
    for a, (n, b) in sorted(per_arborem.items(), key=lambda kv: -kv[1][1]):
        print('  %-28s %9s  %6d plagulae' % (a, gb(b), n))
    os.makedirs(os.path.dirname(scriptum) or '.', exist_ok=True)
    with open(scriptum, 'w') as s:
        s.write('#!/bin/sh\n')
        s.write('# GENERATUM a tools/uuidtext_census.py %s - NE EDITA MANU\n'
                % time.strftime('%Y-%m-%d %H:%M'))
        s.write('# Delet %d plagulas uuidtext (%s) quarum binaria NOSTRA\n'
                % (len(nostra), gb(nostra_b)))
        s.write('# sunt (arbores rhubarb*, ~/.rhubarb, fumi, frigida,\n'
                '# scratch). Cetera intacta. Usus: sudo sh %s\n' % scriptum)
        s.write('set -u\n')
        s.write('[ "$(id -u)" -eq 0 ] || { echo "sudo sh $0"; exit 2; }\n')
        s.write('ante=$(du -sk %s | cut -f1)\n' % RADIX_CACHE)
        for p, _, _ in nostra:
            s.write("rm -f '%s'\n" % p)
        s.write('post=$(du -sk %s | cut -f1)\n' % RADIX_CACHE)
        s.write('echo "uuidtext: $((ante / 1048576)) GB -> '
                '$((post / 1048576)) GB"\n')
    print('scriptum: %s (%d plagulae, %s) - currere: sudo sh %s'
          % (scriptum, len(nostra), gb(nostra_b), scriptum))
    return 0


if __name__ == '__main__':
    sys.exit(principale())
