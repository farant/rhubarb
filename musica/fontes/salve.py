# salve: Salve Regina (tonus simplex, modus V) ex gabc - Liber Usualis
# (1961) p. 279, GregoBase 2435 (Andrew Hinkley). Lectio gabc -> MML,
# verba, incisa (metrum liberum). Regulae Solesmenses:
#   punctum = octava (II sextae decimae); '.' mora x2; '_' episema x1.5;
#   nota ante quilisma ('w') x1.5; unisoni intra syllabam ligati
#   (bivirga, tristropha, pressus).
#   divisiones: ',' ';' '`' -> '|' + spiritus r16; ':' '::' -> '|' + r8.
# Clavis c4: do in linea IV (j); do = F (modus V, finalis F; fa = Bb).
import re, sys, unicodedata

GRADUS = 'abcdefghijklm'
# c4: j = do = F4 (MIDI 65); scala do maior super F: F G A Bb C D E
SCALA = [0, 2, 4, 5, 7, 9, 11]
DO_MIDI, DO_GRADUS = 65, GRADUS.index('j')
NOMINA = ['c', 'c+', 'd', 'e-', 'e', 'f', 'f+', 'g', 'a-', 'a', 'b-', 'b']

def midi(littera):
    g = GRADUS.index(littera) - DO_GRADUS
    octava, gradus = divmod(g, 7)
    return DO_MIDI + 12 * octava + SCALA[gradus]

def notae_neumatis(s):
    """notae gabc unius syllabae -> [(midi, sextae decimae)]"""
    s = re.sub(r'\[[^\]]*\]', '', s)          # [oh:h] [ll:1] ...
    notae = []
    i = 0
    while i < len(s):
        c = s[i]
        if c.lower() in GRADUS:
            m = midi(c.lower())
            longitudo = 2
            j = i + 1
            quilisma = False
            while j < len(s) and s[j] in "._'wvos~<>-01234":
                if s[j] == '.':
                    longitudo = 4
                elif s[j] == '_' and longitudo == 2:
                    longitudo = 3
                elif s[j] == 'w':
                    quilisma = True
                j += 1
            if quilisma and notae:
                p, l = notae[-1]
                notae[-1] = (p, max(l, 3))
            if notae and notae[-1][0] == m:      # unisonus ligatus
                notae[-1] = (m, notae[-1][1] + longitudo)
            else:
                notae.append((m, longitudo))
            i = j
        else:
            i += 1
    return notae

def syllaba_munda(t):
    t = t.replace('{', '').replace('}', '')
    t = unicodedata.normalize('NFD', t)
    t = ''.join(ch for ch in t if not unicodedata.combining(ch))
    t = re.sub(r'[^A-Za-z]', '', t)
    return t[:1] + t[1:].lower() if t else t

def legere(gabc):
    """-> (incisa [(notae, spiritus)], verba): syllabae verbi '-',
    verba ' ', melismata ' _' post syllabam"""
    corpus = gabc.split('%%', 1)[1]
    incisa, notae_incisi = [], []
    verba = ''
    for m in re.finditer(r'([^()]*)\(([^)]*)\)', corpus):
        textus, n = m.group(1), m.group(2).strip()
        if re.fullmatch(r'[cf]b?\d', n):
            continue                          # clavis
        if n in (',', ';', ':', '::', '`', ';1', ';2'):
            incisa.append((notae_incisi, 'r8' if n in (':', '::')
                           else 'r16'))
            notae_incisi = []
            continue
        nn = notae_neumatis(n)
        if not nn:
            continue
        s = syllaba_munda(textus)
        if s:
            novum_verbum = textus.startswith(' ') or not verba
            verba += (' ' if novum_verbum and verba else
                      '' if not verba else '-') + s
            verba += ' _' * (len(nn) - 1)
        else:                                 # sine textu: melisma
            verba += ' _' * len(nn)
        notae_incisi.extend(nn)
    if notae_incisi:
        incisa.append((notae_incisi, ''))
    incisa[-1] = (incisa[-1][0], '')          # finis: sine spiritu
    return incisa, verba.strip()

VALORES = {2: '8', 3: '8.', 4: '4', 6: '4.', 8: '2', 12: '2.'}

def mml(incisa):
    partes, octava = [], None
    for notae, spiritus in incisa:
        if not notae:
            continue
        toks = []
        for p, l in notae:
            o = p // 12 - 1
            if o != octava:
                toks.append('o%d' % o)
                octava = o
            toks.append(NOMINA[p % 12] + VALORES[l])
        if spiritus:
            toks.append(spiritus)
        partes.append(' '.join(toks))
    return ' | '.join(partes)

COMMENTARIUM = '''
  <!-- salve: Salve Regina, tonus simplex (modus V, f lydius cum b
       molle: clavis c4, do = F). Fons: Liber Usualis (1961) p. 279,
       GregoBase 2435 (transcriptio Andrew Hinkley). Rhythmus
       Solesmensis: punctum octava, mora x2, episema x1.5, nota ante
       quilisma x1.5, unisoni ligati; divisiones '|' cum spiritu (minor
       r16, maior r8). Schola sola. Generator:
       musica/fontes/salve.py -->'''

CARMEN = '''
  <carmen titulus="salve" tempus="80" metrum="liberum" clavis="f lydius">
    <reverberatio aula="0.9" mollities="0.6" latitudo="1" lucrum="0.5"/>
    <pars titulus="cantus" instrumentum="schola" lucrum="1"
          reverberatio="0.55">
      <frustum mensura="1"><textus!>%s</textus>
        <verba!>%s</verba></frustum>
    </pars>
  </carmen>
'''

if __name__ == '__main__':
    incisa, verba = legere(open(sys.argv[1], encoding='utf-8').read())
    open(sys.argv[2], 'w').write(COMMENTARIUM + CARMEN % (mml(incisa), verba))
