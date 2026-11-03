# nata_lux: Tallis, O nata lux (V voces, SATTB) ex MusicXML - editio
# Andrew Sims (CPDL #80317, 2024, sine lineis mensurae, licentia CPDL).
# Lectio MusicXML -> MML, verba, incisa (metrum liberum) per partem.
#   altitudo ex <step>/<alter>/<octave> (littera auctoris servatur);
#   valor ex <duration> (480 = quadrans); nota sine <lyric> = melisma;
#   mensura cum <repeat> bis scribitur (Nos membra ... corporis).
#   incisa: initia versuum in superiore (LINEAE); nota partis alterius
#   quae lineam incisi transit in duas ligatas ('&') dividitur.
# Usus: python3 -I nata_lux.py nata_lux_80317.mxl exitus.stml
#   (.mxl = MusicXML compressa, ut CPDL eam dat; .xml quoque legitur)
import sys
import zipfile
import xml.etree.ElementTree as ET

QUADRANS = 480
# syllabae quibus versus incipiunt, ordine (repetitio inclusa)
LINEAE = ['O', 'Je', 'di', 'lau', 'Qui', 'di', 'Nos', 'tu', 'Nos', 'tu']
VOCES = [('superius', 'superius', '-0.3'), ('altus', 'altus', '0.3'),
         ('tenor_primus', 'tenor', '-0.1'),
         ('tenor_secundus', 'tenor', '0.1'), ('bassus', 'bassus', '0')]
VALORES = [(4 * QUADRANS * 3 // 2, '1.'), (4 * QUADRANS, '1'),
           (3 * QUADRANS, '2.'), (2 * QUADRANS, '2'),
           (3 * QUADRANS // 2, '4.'), (QUADRANS, '4'),
           (3 * QUADRANS // 4, '8.'), (QUADRANS // 2, '8'),
           (QUADRANS // 4, '16')]
ALTERATIO = {-1: '-', 0: '', 1: '+'}


def partem_legere(pars):
    """-> [(initium, longitudo, nota|None, syllaba|None, syllabicum)]"""
    mensurae = []
    for m in pars.findall('measure'):
        mensurae.append(m)
        if any(r.get('direction') == 'backward'
               for r in m.iter('repeat')):
            # repetitio: a mensura cum repeat forward usque huc
            initium = max(i for i, mm in enumerate(mensurae)
                          if any(r.get('direction') == 'forward'
                                 for r in mm.iter('repeat')))
            mensurae.extend(mensurae[initium:])
    eventa, t = [], 0
    for m in mensurae:
        for n in m.findall('note'):
            assert n.find('chord') is None, 'accordum in parte'
            assert n.find('tie') is None, 'ligatura in fonte'
            d = int(n.findtext('duration'))
            p = n.find('pitch')
            nota = None
            if p is not None:
                nota = (p.findtext('step').lower(),
                        int(p.findtext('alter') or 0),
                        int(p.findtext('octave')))
            ly = n.find('lyric')
            syl = ly.findtext('text').strip() if ly is not None else None
            sylc = ly.findtext('syllabic') if ly is not None else None
            eventa.append((t, d, nota, syl, sylc))
            t += d
    return eventa


def lineas_invenire(superius):
    lineae, k = [], 0
    for t, d, nota, syl, sylc in superius:
        if (k < len(LINEAE) and syl == LINEAE[k]
                and sylc in ('begin', 'single')):
            lineae.append(t)
            k += 1
    assert k == len(LINEAE), 'versus non inventi: %d' % k
    return lineae


def valores(longitudo):
    """longitudo -> signa valorum ligandorum (maxima primum)"""
    signa = []
    for l, s in VALORES:
        while longitudo >= l:
            signa.append(s)
            longitudo -= l
    assert longitudo == 0, 'longitudo non scribenda'
    return signa


def mml(eventa, lineae):
    fines = set(lineae[1:])
    toks, octava = [], None
    for t, d, nota, syl, sylc in eventa:
        # partes notae inter lineas incisorum
        puncta = [t] + sorted(f for f in fines if t < f < t + d) + [t + d]
        for i in range(len(puncta) - 1):
            if puncta[i] in fines:
                toks.append('|')
            signa = valores(puncta[i + 1] - puncta[i])
            for j, s in enumerate(signa):
                if nota is None:
                    toks.append('r' + s)
                    continue
                littera, alt, o = nota
                if o != octava:
                    toks.append('o%d' % o)
                    octava = o
                ultima = i == len(puncta) - 2 and j == len(signa) - 1
                toks.append(littera + ALTERATIO[alt] + s
                            + ('' if ultima else ' &'))
    return ' '.join(toks)


def verba(eventa):
    v = ''
    for t, d, nota, syl, sylc in eventa:
        if nota is None:
            continue
        if syl is None:
            v += ' _'
            continue
        s = ''.join(c for c in syl if c.isalpha())
        v += ('-' if sylc in ('middle', 'end') else ' ') + s
    return v.strip()


COMMENTARIUM = '''
  <!-- nata_lux: Thomas Tallis, O nata lux de lumine (hymnus ad Laudes
       in Transfiguratione; Cantiones sacrae 1575), V voces SATTB a
       cappella. Fons: editio Andrew Sims (CPDL #80317, 2024, sine
       lineis mensurae, licentia CPDL), MusicXML. Incisa = versus
       hymni; versus ultimi (Nos membra ... corporis) repetuntur ut in
       editione. Finis in G maiore (B naturalis alti: tertia
       Picardiae). Generator: musica/fontes/nata_lux.py -->'''

INSTRUMENTA = ''.join(
    '\n    <instrumentum titulus="%s" basis="schola" vox="%s"/>' % (t, v)
    for t, v in sorted(set((v, v) for _, v, _ in VOCES)))

PARS = '''
    <pars titulus="%s" instrumentum="%s" lucrum="0.75" positio="%s"
          reverberatio="0.55">
      <frustum mensura="1"><textus!>%s</textus>
        <verba!>%s</verba></frustum>
    </pars>'''

CARMEN = '''
  <carmen titulus="nata_lux" tempus="72" metrum="liberum" clavis="g dorius">%s
    <reverberatio aula="0.9" mollities="0.6" latitudo="1" lucrum="0.5"/>%s
  </carmen>
'''

def radicem_legere(via):
    """.mxl: META-INF/container.xml nominat fontem intra zip"""
    if not via.endswith('.mxl'):
        return ET.parse(via).getroot()
    with zipfile.ZipFile(via) as z:
        cont = ET.fromstring(z.read('META-INF/container.xml'))
        fons = next(cont.iter('rootfile')).get('full-path')
        return ET.fromstring(z.read(fons))


if __name__ == '__main__':
    radix = radicem_legere(sys.argv[1])
    partes = [partem_legere(p) for p in radix.findall('part')]
    assert len(partes) == len(VOCES)
    longitudines = set(e[-1][0] + e[-1][1] for e in partes)
    assert len(longitudines) == 1, longitudines
    lineae = lineas_invenire(partes[0])
    corpus = ''.join(PARS % (t, v, pos, mml(e, lineae), verba(e))
                     for (t, v, pos), e in zip(VOCES, partes))
    open(sys.argv[2], 'w').write(COMMENTARIUM + CARMEN % (INSTRUMENTA,
                                                           corpus))
