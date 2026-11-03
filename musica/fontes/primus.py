# primus: cantus modi I (d dorius) - schola super ison D, metrum liberum.
# Rhythmus Solesmensis: punctum = octava; nota ante divisionem minorem
# quarta, ante maiorem quarta punctata + spiritus (r8); ultima d2.
# Incisa ('|') verbis Ave Maria respondent (ave.py eadem melodia).
import re, sys

PHRASES = [
    # Ave Maria | gratia plena - D -> A, b molle super A
    ["o4 l8 d e f g a a g4", "a b- a g f g a4. r8"],
    # Dominus | tecum - ad C superius, redit ad A
    ["a o5 c d c o4 a g4", "a o5 c o4 b a g a a4. r8"],
    # benedicta | tu in mulieribus - descensus ad F
    ["f g a g4", "f e d e f g f e f4. r8"],
    # et benedictus fructus | ventris tui | Iesus - subtonium C, D
    ["d e f g f e d4", "c d e f4", "e d2."],
]

VALORES = {'1': 8, '2': 4, '4': 2, '8': 1}

def octavae(incisum):
    """longitudo incisi in octavis (l8 ordinaria)"""
    n = 0
    for t in incisum.split():
        m = re.fullmatch(r'([a-gr])[-+#]?(\d+)?(\.)?', t)
        if not m:
            continue
        v = VALORES[m.group(2) or '8']
        n += v * 3 // 2 if m.group(3) else v
    return n

def valores(n):
    """n octavae ut notae ligatae ('&'), maximae primum"""
    partes = []
    for v, s in ((8, '1'), (6, '2.'), (4, '2'), (3, '4.'), (2, '4'),
                 (1, '8')):
        while n >= v:
            partes.append(s)
            n -= v
    return partes

def ison():
    """per phrasin nota D una, ligata trans incisa (spiritus chori non
    interrumpit; phrasis nova iterum incipit)"""
    phrases = []
    for ph in PHRASES:
        incisa = []
        for inc in ph:
            incisa.append(' & '.join('d' + v for v in valores(octavae(inc))))
        phrases.append(' & | '.join(incisa))
    return 'o3 ' + ' | '.join(phrases)

def textus():
    return ' | '.join(' | '.join(ph) for ph in PHRASES)

CAPUT = '''
  <carmen titulus="%s" tempus="80" metrum="liberum" clavis="d dorius">
    <reverberatio aula="0.9" mollities="0.6" latitudo="1" lucrum="0.5"/>
    <instrumentum titulus="ison_d" basis="schola" vocalis="o"
        vox="bassus" amplitudo="0.5" ambitus="36..60"/>
    <pars titulus="cantus" instrumentum="schola" lucrum="1"
          reverberatio="0.55">
      <frustum mensura="1"><textus!>%s</textus>%s</frustum>
    </pars>
    <pars titulus="ison" instrumentum="ison_d" lucrum="1"
          reverberatio="0.55">
      <frustum mensura="1"><textus!>%s</textus></frustum>
    </pars>
  </carmen>
'''

def carmen(titulus, commentarium, verba=None):
    v = '\n        <verba!>%s</verba>' % verba if verba else ''
    return commentarium + (CAPUT % (titulus, textus(), v, ison()))

COMMENTARIUM = '''
  <!-- primus: cantus modi I (d dorius) sine textu, schola super ison
       D, metrum liberum (incisa '|', punctum octava; nota ante
       divisionem longior, spiritus r8 in fine phrasis). Initium D -> A
       (finalis -> repercussio), b molle (Bb) super A, phrases in A, A,
       F, D cadunt; subtonium C ante finem. Generator:
       musica/fontes/primus.py -->'''

if __name__ == '__main__':
    open(sys.argv[1], 'w').write(carmen('primus', COMMENTARIUM))
