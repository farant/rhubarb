#!/usr/bin/env python3
"""generare.py - fixa icones: fons_256.png et sips_16.png.

fons_256.png: NOSTRA, determinata, sine dependentia (zlib et struct
bibliothecae normae). sips_16.png: ORACULUM APPLE CONGELATUM -
'sips -z 16 16' super fontem. Oraculum congelari potest quia exitus
sips fontis SOLIUS functio est, non codicis nostri: codex noster
mutari potest sine hoc fixo renovando. Eadem disciplina ac constantes
per Python pinnatae (imago_png CRC IEND, imago_opus 40/200).

Fons ad FALSIFICANDUM designatus, non ad placendum:

  - discus opacus cum gradu (r ex x, g ex y), in fundo PELLUCIDO
    ALBO: pixelum pellucidum colorem in OMNI canali fert, ergo halo
    (media sine pondere alphae) in quolibet canali apparet - forma
    quarta legis plantae, 2026-09-12 (fixum cuius axis constans
    assertionem infalsificabilem facit).
  - lineae nigrae UNIUS pixeli omni VIII versu (y % 8 == 0): ad XVI
    (gradus XVI) resamplator PROXIMUS in linea SEMPER legit (0, 16,
    32 ... omnes = 0 mod 8), media areae eas in umbram 1/8 miscet.
    Ergo 'proximus pro area per errorem' differentiam MAGNAM dat -
    id ipsum quod I7 capere debet.

Usus (e radice arboris):
    python3 probationes/fixa/icones/generare.py

Regeneratio solum cum causa nominata: fixa pinnata sunt, et
probatio_icones.c limitem MENSURATUM contra ea tenet.
"""

import os
import struct
import subprocess
import sys
import zlib

LATUS = 256
RADIUS = 110.0
DIR = os.path.dirname(os.path.abspath(__file__))


def pixelum(x, y):
    centrum = (LATUS - 1) / 2.0
    dx = x - centrum
    dy = y - centrum
    if dx * dx + dy * dy > RADIUS * RADIUS:
        return (255, 255, 255, 0)
    if y % 8 == 0:
        return (0, 0, 0, 255)
    return ((x * 255) // LATUS, (y * 255) // LATUS, 64, 255)


def pars(genus, datum):
    corpus = genus + datum
    return (struct.pack('>I', len(datum)) + corpus
            + struct.pack('>I', zlib.crc32(corpus) & 0xFFFFFFFF))


def png(latitudo, altitudo, functio):
    crudum = bytearray()
    for y in range(altitudo):
        crudum.append(0)
        for x in range(latitudo):
            crudum.extend(functio(x, y))
    ihdr = struct.pack('>IIBBBBB', latitudo, altitudo, 8, 6, 0, 0, 0)
    return (b'\x89PNG\r\n\x1a\n' + pars(b'IHDR', ihdr)
            + pars(b'IDAT', zlib.compress(bytes(crudum), 9))
            + pars(b'IEND', b''))


def main():
    fons = os.path.join(DIR, 'fons_256.png')
    oraculum = os.path.join(DIR, 'sips_16.png')

    with open(fons, 'wb') as f:
        f.write(png(LATUS, LATUS, pixelum))
    print('scriptum: %s' % fons)

    if not os.path.exists('/usr/bin/sips'):
        print('sips abest: oraculum NON generatum', file=sys.stderr)
        return 2
    r = subprocess.run(['/usr/bin/sips', '-z', '16', '16', fons,
                        '--out', oraculum],
                       capture_output=True, text=True)
    if r.returncode != 0:
        print('sips defecit:\n%s%s' % (r.stdout, r.stderr),
              file=sys.stderr)
        return 1
    print('scriptum: %s (sips -z 16 16)' % oraculum)
    return 0


if __name__ == '__main__':
    sys.exit(main())
