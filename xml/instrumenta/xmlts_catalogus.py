# xml/instrumenta/xmlts_catalogus.py - catalogus suitae W3C ->
# selectio casuum (planum xml-arbor P1; spec par. I et VII).
#
# Selectio (spec X1): RECOMMENDATION XML 1.0 (quodvis erratum) aut
# NS 1.0, VERSION non 1.1, EDITION absens aut V continens.
# Exclusiones intra selectionem, singulae cum causa:
#   codificatio      UTF-16 (BOM) aut EncName bene formatum non UTF-8
#                    (spec X8) - casus non iudicatur
#   sine-spatiis     NAMESPACE="no": casus pro processore sine spatiis
#                    nominum (nos spatia tractamus, X1) - non iudicatur
#   entitas-externa  not-wf cuius vitium solum in entitate externa
#                    latet: expat entitates externas non legens eum
#                    accipit (X3) - non iudicatur
#   exitus-externus  exitus canonicus pendet ex entitate externa non
#                    lecta: expat accipit sed exitus differt - acceptio
#                    iudicatur, exitus non
# TYPE 'error' (vitia optionalia) numquam iudicatur.
#
# Usus: python3 -I xmlts_catalogus.py RADIX            selectio (TSV)
#       python3 -I xmlts_catalogus.py RADIX -mensura   concordia
#                                                      expat/libxml2
# RADIX = xml/build/xmlts/xmlconf. Linea TSV:
#   ID TYPE via exitus|- entitates spatia exclusio|-
# (viae relativae ad RADIX).
import collections
import importlib.util
import os
import re
import subprocess
import sys
import xml.etree.ElementTree as ET

_s = importlib.util.spec_from_file_location(
    'expat_canonicum',
    os.path.join(os.path.dirname(os.path.abspath(__file__)),
                 'expat_canonicum.py'))
expat_canonicum = importlib.util.module_from_spec(_s)
_s.loader.exec_module(expat_canonicum)

RECOMMENDATIONES = ('XML1.0', 'NS1.0', 'XML1.0-errata2e',
                    'XML1.0-errata3e', 'XML1.0-errata4e',
                    'NS1.0-errata1e')
CODIFICATIONES_LICITAE = ('utf-8', 'us-ascii', 'ascii')


def casus_legere(radix):
    summum = open(os.path.join(radix, 'xmlconf.xml'),
                  encoding='utf-8').read()
    entitates = dict(re.findall(r'<!ENTITY\s+(\S+)\s+SYSTEM\s+"([^"]+)"',
                                summum))
    casus = []
    for m in re.finditer(r'<TESTCASES[^>]*>(.*?)</TESTCASES>', summum,
                         re.S):
        for e in re.findall(r'&([\w.-]+);', m.group(1)):
            via = entitates[e]
            txt = open(os.path.join(radix, via), 'rb').read()
            txt = txt.decode('utf-8')
            txt = re.sub(r'<\?xml[^>]*\?>', '', txt)
            txt = re.sub(r'<!DOCTYPE[^\[>]*(\[.*?\])?\s*>', '', txt,
                         flags=re.S)
            # catalogi Sun fragmenta sunt (radices plures): involvuntur
            for t in ET.fromstring('<R>' + txt + '</R>').iter('TEST'):
                a = dict(t.attrib)
                a['_dir'] = os.path.dirname(via)
                casus.append(a)
    return casus


def selectus(c):
    return (c.get('RECOMMENDATION', 'XML1.0') in RECOMMENDATIONES
            and c.get('VERSION', '1.0') != '1.1'
            and '5' in c.get('EDITION', '5').split())


def codificatio_aliena(octeti):
    if octeti[:2] in (b'\xff\xfe', b'\xfe\xff'):
        return True
    m = re.match(rb'(\xef\xbb\xbf)?<\?xml[^>]*?encoding\s*=\s*'
                 rb'["\']([A-Za-z][A-Za-z0-9._-]*)["\']', octeti)
    return bool(m) and m.group(2).decode().lower() not in \
        CODIFICATIONES_LICITAE


def via_casus(c, clavis):
    return os.path.normpath(os.path.join(c['_dir'], c[clavis]))


def exclusio(radix, c):
    octeti = open(os.path.join(radix, via_casus(c, 'URI')), 'rb').read()
    if codificatio_aliena(octeti):
        return 'codificatio'
    if c.get('NAMESPACE') == 'no':
        return 'sine-spatiis'
    if c.get('ENTITIES', 'none') != 'none':
        bene, forma = expat_canonicum.canonicum(
            octeti, c.get('NAMESPACE') != 'no')
        if c['TYPE'] == 'not-wf' and bene:
            return 'entitas-externa'
        if bene and c.get('OUTPUT'):
            exp = open(os.path.join(radix, via_casus(c, 'OUTPUT')),
                       'rb').read().decode('utf-8')
            if forma != exp:
                return 'exitus-externus'
    return '-'


def mensura(radix, casus):
    """concordia expat et libxml2 cum genere casus et exitu"""
    C = collections.Counter()
    dissensus = []
    for c, exc in casus:
        if c['TYPE'] == 'error' or exc in ('codificatio', 'sine-spatiis',
                                           'entitas-externa'):
            continue
        via = os.path.join(radix, via_casus(c, 'URI'))
        bene, forma = expat_canonicum.canonicum(
            open(via, 'rb').read(), c.get('NAMESPACE') != 'no')
        lint = subprocess.run(['xmllint', '--noout', '--nonet', via],
                              stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL).returncode == 0
        debet = c['TYPE'] != 'not-wf'
        C['casus'] += 1
        C['expat recte'] += bene == debet
        C['libxml2 recte'] += lint == debet
        if bene != debet:
            dissensus.append(('expat', c['ID'], c['TYPE'], forma))
        if lint != debet:
            dissensus.append(('libxml2', c['ID'], c['TYPE'], ''))
        if bene and c.get('OUTPUT') and exc != 'exitus-externus':
            exp = open(os.path.join(radix, via_casus(c, 'OUTPUT')),
                       'rb').read().decode('utf-8')
            C['exitus'] += 1
            C['exitus expat aequat'] += forma == exp
            if forma != exp:
                dissensus.append(('exitus', c['ID'], c['TYPE'],
                                  repr(forma[:80]) + ' / ' +
                                  repr(exp[:80])))
    for k, v in C.items():
        print('%-22s %d' % (k, v))
    for d in dissensus:
        print('\t'.join(d))


if __name__ == '__main__':
    radix = sys.argv[1]
    omnes = casus_legere(radix)
    sel = [c for c in omnes if selectus(c)]
    casus = [(c, exclusio(radix, c)) for c in sel]
    if len(sys.argv) > 2 and sys.argv[2] == '-mensura':
        mensura(radix, casus)
        sys.exit(0)
    print('# selectio.tsv - DATA: casus xmlts selecti (scribit '
          'xml/instrumenta/xmlts_catalogus.py)')
    print('# casus omnes %d, selecti %d; genera %s' % (
        len(omnes), len(sel),
        dict(collections.Counter(c['TYPE'] for c in sel))))
    print('# exclusiones %s' % dict(collections.Counter(
        e for _, e in casus if e != '-')))
    print('# ID\tTYPE\tvia\texitus\tentitates\tspatia\texclusio')
    for c, exc in casus:
        print('\t'.join([c['ID'], c['TYPE'], via_casus(c, 'URI'),
                         via_casus(c, 'OUTPUT') if c.get('OUTPUT')
                         else '-', c.get('ENTITIES', 'none'),
                         c.get('NAMESPACE', '-'), exc]))
