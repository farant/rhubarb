# xml/instrumenta/silvestria_indicem.py - manifestum corporis silvestris
# clientis xml (planum xml-arbor P1 gradus 5; regula Frani 2026-09-28:
# plagulae alienae in repositorio non ponuntur - manifestum solum).
#
# Radices: ~/Documents/projects, /opt/homebrew (extensiones .xml .svg
# .plist .xhtml .xsd .xsl), /System/Library/CoreServices (.plist).
# Omittuntur: directoria build/, node_modules/, .git/; plagulae quae
# (post BOM optionalem et spatia) non a '<' incipiunt (plist binaria,
# alia). Unicae per contentum: sigillum sha256 semel (via prima ordine
# alphabetico). Mensurae in stderr: numerus, octeti, maxima, non UTF-8.
#
# Usus: python3 -I silvestria_indicem.py > manifestum
#   linea: sha256<TAB>via ('~' = domus)
import hashlib
import os
import re
import sys

DOMUS = os.path.expanduser('~')
RADICES = [(os.path.join(DOMUS, 'Documents/projects'),
            ('.xml', '.svg', '.plist', '.xhtml', '.xsd', '.xsl')),
           ('/opt/homebrew', ('.xml', '.svg', '.plist', '.xhtml', '.xsd',
                              '.xsl')),
           ('/System/Library/CoreServices', ('.plist',))]
OMISSA = {'build', 'node_modules', '.git'}


def codificatio(octeti):
    if octeti[:2] in (b'\xff\xfe', b'\xfe\xff'):
        return 'utf-16'
    m = re.match(rb'(\xef\xbb\xbf)?\s*<\?xml[^>]*?encoding\s*=\s*'
                 rb'["\']([^"\']*)["\']', octeti)
    if m and m.group(2).decode('latin-1').lower() not in ('utf-8',
                                                          'us-ascii'):
        return m.group(2).decode('latin-1').lower()
    try:
        octeti.decode('utf-8')
    except UnicodeDecodeError:
        return 'non-utf-8'
    return None


def main():
    visa = {}
    for radix, extensiones in RADICES:
        for d, subd, plagulae in os.walk(radix):
            subd[:] = sorted(s for s in subd if s not in OMISSA)
            for p in sorted(plagulae):
                if not p.lower().endswith(extensiones):
                    continue
                via = os.path.join(d, p)
                try:
                    if not os.path.isfile(via) or os.path.islink(via):
                        continue
                    octeti = open(via, 'rb').read()
                except OSError:
                    continue
                if octeti[:2] in (b'\xff\xfe', b'\xfe\xff'):
                    initium = b'<'
                else:
                    initium = octeti.lstrip(b'\xef\xbb\xbf').lstrip()[:1]
                if initium != b'<':
                    continue
                s = hashlib.sha256(octeti).hexdigest()
                if s not in visa or via < visa[s][0]:
                    visa[s] = (via, len(octeti), codificatio(octeti))
    octeti = sum(v[1] for v in visa.values())
    maxima = max(visa.values(), key=lambda v: v[1])
    alienae = {}
    for v in visa.values():
        if v[2]:
            alienae[v[2]] = alienae.get(v[2], 0) + 1
    sys.stderr.write('plagulae %d, octeti %d, maxima %d (%s)\n'
                     % (len(visa), octeti, maxima[1], maxima[0]))
    sys.stderr.write('codificationes non UTF-8: %s\n' % alienae)
    print('# silvestria.manifestum - DATA: corpus silvestre clientis xml '
          '(planum xml-arbor P1)')
    print('# Scribit xml/instrumenta/silvestria_indicem.py. '
          'MANIFESTUM SOLUM (regula Frani')
    print('# 2026-09-28): porta plagulas vivas legit; sigillo mutato '
          'aut plagula absente')
    print('# OMITTIT CUM NUMERO. Linea: sha256<TAB>via (\'~\' = domus).')
    for s, v in sorted(visa.items(), key=lambda t: t[1][0]):
        via = v[0]
        if via.startswith(DOMUS + '/'):
            via = '~' + via[len(DOMUS):]
        print('%s\t%s' % (s, via))


if __name__ == '__main__':
    main()
