"""silva.py - facies Pythonica instrumentorum silvae (dev-time SOLUM).

Bibliotheca stdlib sola; nihil navatum ab ea pendet. Veritas in C manet
(formator, differre, examen) - hic involucra subprocessus super CLIs
cum effusu -machina, ut scripta editionis codicem NOMINE, non textu,
tangant et post se ipsa iudicent.

Usus typicus in scripto editionis:

    import sys; sys.path.insert(0, 'pythonica'); import silva
    e = silva.Editio('lib/chorda.c')
    e.substituere('chorda_mensura', NOVUM)      # corpus functionis nomine
    e.replace('x  = I;', 'x = II;')             # ancora spatiis tolerans
    f = e.applicare()                           # omnia aut nihil; portae
    r = silva.Refactio()                         # trans plagulas, unum applicare
    r.editio('lib/x.c').substituere('f', NOVUM)
    r.prototypum_synchronizare('lib/x.c', 'include/x.h', 'f')
    fructus = r.applicare()                      # [Fructus] post omnes scriptas
    print(f)                                    # examen, forma, unitates, diff
    assert f.sana and f.unitates() == [('MUTATA', 'chorda_mensura', 'substantiva')]

Contractus:
  - Editio mutat textum IN MEMORIA; nihil in discum it ante applicare().
    Ancora fallens levat SilvaError - plagula intacta (omnia aut nihil).
  - applicare() = punctum scripturae unicum, ergo sedes portarum: custos
    lectionis rancidae, scriptura, forma (plagula tota), examen, differre
    contra textum ante editionem. Fructus refert; strictum revertit.
  - Ancorae tolerantes = SERIES LEXEMATUM (spatia ubique indifferentia:
    reordinatio formatoris ancoram non frangit). LEX FORMAE: novus PLANUS
    lexematibus paribus lexemata sola mutat - forma plagulae manet, etiam
    trans lineas; novus planus super extentum plurium linearum
    lexematibus imparibus REFUSATUR (lineae perderentur); novus cum
    lineis novis = forma auctoris verbatim. Ancora absens sedem PROXIMAM
    nominat (lexema divergens, linea).
  - Nomina (substituere/inserere) per formator -extenta resolvuntur in
    textu PRAESENTI (post editiones priores) - lineae labuntur, nomina
    manent.
"""
import difflib
import os
import glob
import re
import subprocess
import sys
import tempfile
from collections import namedtuple

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


class SilvaError(Exception):
    """Apparatus fractus aut ancora fallens - numquam tacitum."""


Extentum = namedtuple('Extentum',
                      'titulus linea_a linea_nodi linea_b definitio genus')
Divergentia = namedtuple('Divergentia',
                         'linea columna regula inventum exspectatum '
                         'nuntius emendationes')
Par = namedtuple('Par', 'genus titulus status classificatio additae '
                 'deletae')
Differentia = namedtuple('Differentia',
                         'paria verdictum cosmetica_solum rc')


def _curre(args, stdin=None, cwd=None, env=None):
    # errors='replace': effusum portae octetos non-UTF-8 ferre potest
    # (probatio octetos crudos imprimens) - decodificatio stricta
    # operarium umbrae 2026-09-02 tacite necavit, signum pendens mansit.
    # cwd: radix altera (clone photographiae) - ordinarius RADIX.
    # env: ambitus totus alter (oraculum: ORACULUM_OMNIA) - None = hereditas
    return subprocess.run(args, cwd=cwd or RADIX, capture_output=True,
                          text=True, errors='replace', input=stdin, env=env)


def _absoluta(via):
    return via if os.path.isabs(via) else os.path.join(RADIX, via)


def _capita(via):
    """directorium plagulae (-capita) - typi extra silvam visibiles"""
    d = os.path.dirname(_absoluta(via))
    return ['-capita', d] if os.path.isdir(d) else []


def _tsv(effusum):
    ordines = []
    for linea in effusum.splitlines():
        if not linea or linea.startswith('#'):
            continue
        ordines.append(linea.split('\t'))
    return ordines


# ---------------------------------------------------------------- extenta

_VEXILLA = None


def _vexilla():
    """vexilla domus (tools/vexilla.sh VEXILLA_C89) - semel lecta"""
    global _VEXILLA
    if _VEXILLA is None:
        r = _curre(['zsh', '-c',
                    'source tools/vexilla.sh && print -rl -- ${=VEXILLA_C89}'])
        _VEXILLA = [l for l in r.stdout.splitlines() if l.strip()]
        if not _VEXILLA:
            raise SilvaError('tools/vexilla.sh: VEXILLA_C89 vacua')
    return list(_VEXILLA)


def _inclusiones_clang(via):
    """-I pro probatione syntaxis: include/, directorium plagulae, et
    fontes/ suitae cuiusque (md fontes materiae includit)"""
    dirs = [os.path.join(RADIX, 'include'), os.path.dirname(_absoluta(via))]
    dirs += sorted(glob.glob(os.path.join(RADIX, '*', 'fontes')))
    return ['-I' + d for d in dirs]


def syntaxis(via, textus=None):
    """clang -fsyntax-only vexillis domus: None = sana, aliter linea prima
    'error:' (chorda). textus non-None = copia temporaria iuxta plagulam
    iudicatur (plagula viva intacta)."""
    if textus is None:
        via_t = _absoluta(via)
    else:
        d = os.path.dirname(_absoluta(via))
        fd, via_t = tempfile.mkstemp(prefix='.syntaxis_', suffix='.c', dir=d)
        with os.fdopen(fd, 'w') as f:
            f.write(textus)
    try:
        r = _curre(['clang', '-fsyntax-only'] + _vexilla()
                   + _inclusiones_clang(via_t) + [via_t])
        if r.returncode == 0:
            return None
        for l in (r.stderr or '').splitlines():
            if 'error:' in l:
                return l.replace(via_t, via)
        cauda = (r.stderr or '').strip().splitlines()
        return cauda[-1] if cauda else 'clang rc %d' % r.returncode
    finally:
        if textus is not None:
            os.unlink(via_t)


def probatio_addere(via, novus, ante='credo_imprimere_compendium',
                    **applicare):
    """casum probationis ante vocationem 'ante(' inserere et applicare
    (Fructus redditur; **applicare -> Editio.applicare, e.g. iudica=False)"""
    e = Editio(via)
    e.inserere_ante_vocationem(ante, novus)
    return e.applicare(**applicare)


Expansio = namedtuple('Expansio',
                      'successus vitium linea fragmentum loculus textus')


def expandere(fons):
    """stml expandere (bin/stml, aliter ~/.bin/stml) super viam aut TEXTUM
    STML (copia temporaria in build/pythonica): Expansio(successus,
    vitium (titulus, e.g. LOCULUS_IGNOTUS), linea, fragmentum, loculus,
    textus expansus pulcher). Sondae C pro numero vitii non iam
    necessariae (B1: tres scriptae, instrumento iam adstante)."""
    binarium = os.path.join(RADIX, 'bin', 'stml')
    if not os.path.exists(binarium):
        binarium = os.path.expanduser('~/.bin/stml')
    if not os.path.exists(binarium):
        raise SilvaError('stml absens - strue: ./tools/stml_struere.sh')
    temporaria = None
    if '<' in fons or '\n' in fons or not os.path.exists(_absoluta(fons)):
        d = os.path.join(RADIX, 'build', 'pythonica')
        os.makedirs(d, exist_ok=True)
        fd, temporaria = tempfile.mkstemp(prefix='expandere_', suffix='.stml',
                                          dir=d)
        with os.fdopen(fd, 'w') as f:
            f.write(fons)
        via = temporaria
    else:
        via = _absoluta(fons)
    try:
        r = _curre([binarium, 'expandere', via])
    finally:
        if temporaria:
            os.unlink(temporaria)
    if r.returncode == 0:
        return Expansio(True, None, 0, None, None, r.stdout)
    m = re.search(r":(\d+): ([A-Z_]+)(?: \(fragmentum '([^']*)'\))?"
                  r"(?: \(loculus '([^']*)'\))?", r.stderr or '')
    if not m:
        raise SilvaError('stml expandere rc %d sine vitio nominato: %s'
                         % (r.returncode, (r.stderr or '').strip()[-300:]))
    return Expansio(False, m.group(2), int(m.group(1)), m.group(3),
                    m.group(4), '')


# ---------------------------------------------------------------- oraculum md

FracturaOraculi = namedtuple('FracturaOraculi',
                             'numerus plagula sectio causa sperata nostra')
ExemplumOraculi = namedtuple('ExemplumOraculi',
                             'numerus plagula md sperata nostra')
Oraculum = namedtuple('Oraculum', 'praeterita totalis pinna sectiones '
                      'fracturae exempla ignoscentiae acta')
_ORACULI_FINIS = r'\n(?=  #\d|\n===|\n---|=== CREDO)'


def oraculum(exemplum=None):
    """probatio_md_oraculum uno vocamine (ambitus ORACULUM_OMNIA +
    ORACULUM_EXEMPLUM, grep non iam): Oraculum(praeterita, totalis,
    pinna, sectiones {titulus: (praeterita, totalis)} ordine, fracturae
    [FracturaOraculi] OMNES - causa = vitium programmatis aut None cum
    sperata/nostra NORMATAE (truncatae CXX) -, exempla [ExemplumOraculi]
    pro numero dato: md, sperata cruda, nostra cruda (linea nova finali
    dempta; plagula quaeque numerum suum habet - GFM post sectiones
    extensionum aliter numerat, ergo bina), ignoscentiae {titulus: n},
    acta. Suita md filtro 'oraculum' curritur (secunda pauca)."""
    ambitus = dict(os.environ)
    ambitus['ORACULUM_OMNIA'] = '1'
    if exemplum is not None:
        ambitus['ORACULUM_EXEMPLUM'] = str(int(exemplum))
    r = _curre(['./md/compile_probationes.sh', 'oraculum'], env=ambitus)
    acta = _ANSI.sub('', r.stdout + r.stderr)
    m = re.search(r'--- ORACULUM: (\d+)/(\d+) praeterita \(pinna (\d+)\) ---',
                  acta)
    if r.returncode == 2 or not m:
        raise SilvaError('oraculum non cucurrit (rc=%d): %s'
                         % (r.returncode, acta.strip()[-300:]))
    praeterita, totalis, pinna = (int(x) for x in m.groups())
    sectiones = {}
    ms = re.search(r'--- sectiones.*?---\n(.*?)\n--- ignoscentiae', acta, re.S)
    for o in re.finditer(r'(?m)^ +(\d+)/(\d+) +(.+?)\s*$', ms.group(1) if ms
                         else ''):
        sectiones[o.group(3)] = (int(o.group(1)), int(o.group(2)))
    ignoscentiae = {}
    mi = re.search(r'--- ignoscentiae: (.*?) ---', acta, re.S)
    for o in re.finditer(r"([\w' /<>]+?) (\d+)(?:,|$)", mi.group(1) if mi
                         else ''):
        ignoscentiae[o.group(1).strip()] = int(o.group(2))
    fracturae = []
    exempla = []
    partes = re.split(r'\n--- (\S+): \d+ exempla ---\n', acta)
    for k in range(1, len(partes) - 1, 2):
        plagula, textus = partes[k], partes[k + 1]
        for o in re.finditer(
                r'^  #(\d+) \[([^\]]*)\](?: FRACTA: ([^\n]*)|\n    sperata: (.*?)'
                r'\n    nostra:  (.*?))' + _ORACULI_FINIS, textus, re.S | re.M):
            fracturae.append(FracturaOraculi(
                int(o.group(1)), plagula, o.group(2), o.group(3),
                o.group(4), o.group(5)))
        for o in re.finditer(
                r'=== EXEMPLUM (\d+) \((\S+)\) ===\nmd: (.*?)\nsperata cruda: '
                r'(.*?)\nnostra cruda:  (.*?)' + _ORACULI_FINIS, textus, re.S):
            exempla.append(ExemplumOraculi(
                int(o.group(1)), o.group(2), o.group(3), o.group(4), o.group(5)))
    return Oraculum(praeterita, totalis, pinna, sectiones, fracturae, exempla,
                    ignoscentiae, acta)


def extenta(via):
    """Extenta functionum radicis (definitiones + prototypa) ordine fontis.
    Extentum = a linea post nodum priorem (commentarium ducens inclusum)
    ad lineam ultimam; linea_nodi = linea prima nodi ipsius."""
    r = _curre(['./silva/formator.sh', _absoluta(via), '-extenta']
               + _capita(via))
    if r.returncode == 2 or '# via' not in r.stdout:
        raise SilvaError('formator -extenta fractus: %s' % r.stderr.strip()
                         [-300:])
    exitus = []
    for o in _tsv(r.stdout):
        if len(o) < 6:
            continue
        exitus.append(Extentum(o[1], int(o[2]), int(o[3]), int(o[4]),
                               o[5] == 'definitio', o[5]))
    return exitus


TYPI = ('structura', 'unio', 'enumeratio')


def _extentum_nominis(lista, nomen, definitio=True, genus=None):
    """genus: 'definitio' | 'prototypum' | 'structura' | 'unio' |
    'enumeratio' | 'typus' (quilibet ex TYPI); definitio (bool) forma
    vetus pro functionibus"""
    if genus is None:
        genus = 'definitio' if definitio else 'prototypum'
    if genus == 'typus':
        candidati = [x for x in lista if x.titulus == nomen
                     and x.genus in TYPI]
    else:
        candidati = [x for x in lista if x.titulus == nomen
                     and x.genus == genus]
    if not candidati:
        alia = sorted(set(x.genus for x in lista if x.titulus == nomen))
        if alia:
            raise SilvaError("'%s' (%s) non inventum - hic ut %s adest"
                             % (nomen, genus, '/'.join(alia)))
        raise SilvaError("'%s' (%s) non inventum in hac plagula (vocatio, non"
                         " definitio? definitiones: %s ...)" % (nomen, genus,
                         ', '.join(x.titulus for x in lista if x.definitio)[:120]))
    if len(candidati) > 1:
        raise SilvaError("functio '%s' ambigua (%d extenta)"
                         % (nomen, len(candidati)))
    return candidati[0]


def corpus(via, nomen, definitio=True, genus=None):
    """Textus nodi (sine commentario ducente) functionis aut typi nomine
    (genus='typus' pro structura/unio/enumeratio)."""
    x = _extentum_nominis(extenta(via), nomen, definitio, genus)
    lineae = open(_absoluta(via)).read().splitlines(True)
    return ''.join(lineae[x.linea_nodi - 1:x.linea_b])


# ---------------------------------------------------------------- editio

def _exemplar_tolerans(vetus):
    verba = vetus.split()
    if not verba:
        raise SilvaError('ancora vacua')
    return re.compile(r'\s+'.join(re.escape(v) for v in verba))


def _exemplar_verborum(vetus):
    """tolerans='verba': verba ancorae spatiis quibuslibet ET marginibus
    ' * ' commentorum reflexorum separata - prosa in commentis (formator
    lineas commenti refringit: ancora exacta bis in B3 mortua)"""
    verba = vetus.split()
    if not verba:
        raise SilvaError('ancora vacua')
    return re.compile(r'(?:\s+(?:\*(?!/)[ \t]*)?)+'.join(
        re.escape(v) for v in verba))


# margo commenti: '*' ducens linearum sequentium (non '*/')
_MARGO = re.compile(r'(?m)^[ \t]*\*(?!/)')


# lexator C rudis pro ANCORIS (non pro veritate): litterae chordae et
# characteris, commenta, identificatores, numeri, interpunctiones
# longissimae primum. Ancora et plagula ambae lexantur; series
# lexematum ancorae in serie plagulae quaeritur - spatia ubique
# indifferentia (formator spatia addit, aufert, lineas findit: nihil
# horum seriem lexematum tangit).
_LEX = re.compile(r"""
    /\*.*?\*/                      # commentum
  | "(?:\\.|[^"\\])*"             # littera chordae
  | '(?:\\.|[^'\\])*'             # littera characteris
  | [A-Za-z_]\w*                   # identificator
  | 0[xX][0-9A-Fa-f]+[uUlL]*        # numerus hex
  | \d+(?:\.\d*)?(?:[eE][+-]?\d+)?[uUlLfF]*  # numerus
  | \.\.\.|<<=|>>=|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=
  | [-+*/%&|^~!<>=?:;,.(){}\[\]\#\\]
""", re.S | re.X)


def _lexemata(textus):
    """[(lexema, initium, finis)] - spatia omissa"""
    return [(m.group(0), m.start(), m.end()) for m in _LEX.finditer(textus)]


def _lexema_norma(t):
    """commentum = lexema unum: margines '*' linearum sequentium ablati
    (reflexio formatoris, 2026-09-03) et spatia INTRA collapsa (tabulae,
    lineae refractae, indentatio commenti ancoram non frangunt -
    2026-09-02); cetera exacta"""
    if t.startswith('/*'):
        return ' '.join(_MARGO.sub('', t).split())
    return t


def _series_lexematum(textus, vetus):
    """[[(a, b) lexematis cuiusque], ...] - sedes serierum lexematum
    ancorae in textu, lexema quodque cum spatio suo (substitutio in situ)"""
    anc = [_lexema_norma(t) for t, _, _ in _lexemata(vetus)]
    if not anc:
        raise SilvaError('ancora sine lexematis')
    lex = [(_lexema_norma(t), a, b) for t, a, b in _lexemata(textus)]
    n = len(anc)
    series = []
    for i in range(len(lex) - n + 1):
        if all(lex[i + k][0] == anc[k] for k in range(n)):
            series.append([(lex[i + k][1], lex[i + k][2]) for k in range(n)])
    return series


def _sedes_lexematum(textus, vetus):
    """spatia [initium, finis) ubi series lexematum ancorae in textu
    apparet"""
    return [(s[0][0], s[-1][1]) for s in _series_lexematum(textus, vetus)]


def _proxima_ancorae(textus, vetus):
    """ancora absente: ubi series lexematum LONGISSIME congruit - lexema
    divergens exspectatum/inventum cum linea eius et textu lineae (grep
    post refusionem non iam: quinquies in B3, 2026-09-03)"""
    anc = [_lexema_norma(t) for t, _, _ in _lexemata(vetus)]
    if not anc:
        return ''
    lex = [(_lexema_norma(t), a, b) for t, a, b in _lexemata(textus)]
    optimum, sedes = 0, -1
    for i in range(len(lex)):
        if lex[i][0] != anc[0]:
            continue
        k = 1
        while k < len(anc) and i + k < len(lex) and lex[i + k][0] == anc[k]:
            k += 1
        if k > optimum:
            optimum, sedes = k, i
    if sedes < 0:
        return " - lexema primum ancorae %r in plagula absens" % anc[0]
    linea = textus.count('\n', 0, lex[sedes][1]) + 1
    j = sedes + optimum
    if optimum == len(anc) or j >= len(lex):
        return (" - proxima: lexemata %d/%d congruunt a linea %d, plagula"
                " finit" % (optimum, len(anc), linea))
    linea_div = textus.count('\n', 0, lex[j][1]) + 1
    lineae = textus.splitlines()
    textus_lineae = lineae[linea_div - 1].strip() if linea_div <= len(lineae) \
        else ''
    return (" - proxima: lexemata %d/%d congruunt a linea %d; lexema %d"
            " exspectatum %r, inventum %r (linea %d: %s)"
            % (optimum, len(anc), linea, optimum + 1, anc[optimum],
               lex[j][0], linea_div, textus_lineae[:80]))


def _nomen_planum(nomen, quid='nomen'):
    """nomen ut segmentum viae UNUM: litterae, numeri, . _ - ; nihil
    aliud (nec /, nec .., nec vacuum) - nomina in vias directoriorum
    iunguntur (imagines, clones umbrae, recepta) et deletio recursiva
    eas sequitur"""
    if not isinstance(nomen, str) or not re.fullmatch(r'[A-Za-z0-9._-]+', nomen) \
            or nomen in ('.', '..'):
        raise SilvaError('%s non planum (segmentum viae unum exspectatum): %r'
                         % (quid, nomen))
    return nomen


def _lineae_sedium(textus, sedes):
    """' - lineae [12, 340]' pro refusione ancorae: ubi ancora inventa
    sit, ut ancora longior sine grep eligatur; '' si nulla"""
    if not sedes:
        return ''
    return ' - lineae %s' % [textus.count('\n', 0, a) + 1 for a, _ in sedes]


class Editio(object):
    """Editiones unius plagulae in memoria; applicare() scribit semel.

    replace(vetus, novus, tolerans=True, numerus=1): ancora exacte
    numerus vicibus congruat (ordinarius semel), aliter SilvaError.
    substituere(nomen, novus, definitio=True): corpus nodi (linea_nodi..
    linea_b) nomine substituitur - commentarium ducens manet.
    inserere_post(nomen, novus) / inserere_ante(nomen, novus).
    diff(): textus unificatus; applicare(): scribit et diff reddit."""

    def __init__(self, via):
        self.via = via
        self.originalis = open(_absoluta(via)).read()
        self.textus = self.originalis
        self.acta = []
        self._extenta_originalia = None

    # -- ancorae textuales --
    def replace(self, vetus, novus, tolerans=True, numerus=1):
        """tolerans=True (ordinarius): ancora ut SERIES LEXEMATUM - spatia
        ubique indifferentia, etiam addita/ablata ('a(vacuum)' congruit
        'a (vacuum)', parametra in lineas fissa congruunt); litterae
        chordae exacte. LEX FORMAE (2026-09-03): novus PLANUS (sine linea
        nova) lexematibus paribus = substitutio lexematum IN SITU, forma
        plagulae servata (etiam trans lineas); novus planus lexematibus
        imparibus super extentum plurium linearum = REFUSIO (lineae
        perderentur - quattuor sedes in md_arbor.c ita contortae, quas
        formator sine regula 'sententia in linea sua' accepit); novus
        vacuus delet; novus cum lineis novis = forma auctoris verbatim.
        'spatia': cursus spatiorum solum, scriptura verbatim (forma
        vetus). 'verba': verba prosae trans margines ' * ' commentorum,
        verbatim. False: octeti exacti. Ancora absens: sedes proxima."""
        series = None
        if tolerans is True:
            series = _series_lexematum(self.textus, vetus)
            sedes = [(s[0][0], s[-1][1]) for s in series]
        elif tolerans == 'spatia':
            sedes = [m.span() for m in
                     _exemplar_tolerans(vetus).finditer(self.textus)]
        elif tolerans == 'verba':
            sedes = [m.span() for m in
                     _exemplar_verborum(vetus).finditer(self.textus)]
        else:
            sedes = []
            i = self.textus.find(vetus)
            while i >= 0:
                sedes.append((i, i + len(vetus)))
                i = self.textus.find(vetus, i + 1)
        if len(sedes) != numerus:
            raise SilvaError("ancora %d vicibus inventa (exspectatae %d)%s:"
                             " %r" % (len(sedes), numerus,
                                      _lineae_sedium(self.textus, sedes)
                                      or self._proxima(vetus, tolerans),
                                      vetus[:60]))
        if series is not None:
            self._substituere_lexemata(series, sedes, vetus, novus)
        else:
            for a, b in reversed(sedes):
                self.textus = self.textus[:a] + novus + self.textus[b:]
        self.acta.append('replace %r' % vetus[:40])
        return self

    def _proxima(self, vetus, tolerans):
        """refusio ancorae absentis: series lexematum proxima (tolerans)
        aut, ancora exacta absente, series lexematum inventa"""
        if tolerans is True:
            return _proxima_ancorae(self.textus, vetus)
        try:
            s = _sedes_lexematum(self.textus, vetus)
        except SilvaError:
            return ''
        if s:
            return (' - series lexematum congruit%s (tolerans=True)'
                    % _lineae_sedium(self.textus, s))
        return _proxima_ancorae(self.textus, vetus)

    def _substituere_lexemata(self, series, sedes, vetus, novus):
        """lex formae ancorarum tolerantium (vide replace)"""
        nova = [t for t, _, _ in _lexemata(novus)]
        planus = '\n' not in novus
        if planus and nova and len(nova) == len(series[0]):
            for s in reversed(series):
                for (a, b), t in reversed(list(zip(s, nova))):
                    self.textus = self.textus[:a] + t + self.textus[b:]
            return
        if planus and nova:
            multae = [(a, b) for a, b in sedes if '\n' in self.textus[a:b]]
            if multae:
                raise SilvaError(
                    "ancora lineas plures tenet%s, novus planus lexematibus"
                    " imparibus (%d pro %d): forma perderetur - novus cum"
                    " lineis novis scribatur (aut tolerans='spatia' pro"
                    " scriptura verbatim): %r"
                    % (_lineae_sedium(self.textus, multae), len(nova),
                       len(series[0]), vetus[:60]))
        for a, b in reversed(sedes):
            self.textus = self.textus[:a] + novus + self.textus[b:]

    def replace_inter(self, initium, finis, novus, tolerans=True,
                      inclusae=False):
        """spatium INTER ancoram 'initium' (semel) et ancoram 'finis'
        (primam POST initium) substituere - ambae breves, unicae,
        SERVATAE (novus inter eas stat); inclusae=True: ancorae ipsae
        quoque abeunt (mos ante 2026-09-02, qui caput 'si' devoravit).
        Pro blocis quae commenta tenent: ancora totius bloci commenta
        omnia ferre deberet (lexema unum quodque) - hic duae solae."""
        if tolerans is True:
            a = _sedes_lexematum(self.textus, initium)
        else:
            a = [(i, i + len(initium)) for i in
                 [j for j in range(len(self.textus))
                  if self.textus.startswith(initium, j)]]
        if len(a) != 1:
            raise SilvaError("ancora initii %d vicibus inventa%s: %r"
                             % (len(a), _lineae_sedium(self.textus, a)
                                or self._proxima(initium, tolerans),
                                initium[:60]))
        a0, a1 = a[0]
        cauda = self.textus[a1:]
        if tolerans is True:
            b = _sedes_lexematum(cauda, finis)
        else:
            b = [(i, i + len(finis)) for i in
                 [j for j in range(len(cauda)) if cauda.startswith(finis, j)]]
        if len(b) < 1:
            raise SilvaError("ancora finis post initium non inventa%s: %r"
                             % (_proxima_ancorae(cauda, finis)
                                if tolerans is True else '', finis[:60]))
        b0, b1 = a1 + b[0][0], a1 + b[0][1]
        if inclusae:
            self.textus = self.textus[:a0] + novus + self.textus[b1:]
        else:
            self.textus = self.textus[:a1] + novus + self.textus[b0:]
        self.acta.append('replace_inter %r..%r' % (initium[:20], finis[:20]))
        return self

    # -- nomina --
    def _extenta_praesentia(self):
        """extenta in textu PRAESENTI: copia temporaria iuxta plagulam
        (idem directorium -> eadem capita), deleta post."""
        d = os.path.dirname(_absoluta(self.via))
        fd, via_t = tempfile.mkstemp(prefix='.editio_', suffix='.c', dir=d)
        try:
            with os.fdopen(fd, 'w') as f:
                f.write(self.textus)
            return extenta(via_t)
        finally:
            os.unlink(via_t)

    def _extentum_praesens(self, nomen, definitio=True, genus=None):
        """extentum nominis in textu praesenti; nomine absente
        PARSATIONEM interrogat: textus praesens pauciora extenta quam
        originalis reddens = parsatio mortua (verbum latinum ut
        identificator - 'structura' = struct - lexema ineptum ...), et
        causa nominatur (clang -fsyntax-only linea prima, aliter examen),
        ne 'non inventum' de functione quaque posteriore mentiatur
        (2026-09-03, B1.1: hora perdita)."""
        ext = self._extenta_praesentia()
        try:
            return _extentum_nominis(ext, nomen, definitio, genus)
        except SilvaError as ex:
            if self._extenta_originalia is None:
                self._extenta_originalia = extenta(self.via)
            if len(ext) < len(self._extenta_originalia):
                raise SilvaError(
                    'parsatio textus praesentis MORTUA (%d extenta pro %d):'
                    ' %s\n  (%s)' % (len(ext), len(self._extenta_originalia),
                                     syntaxis(self.via, self.textus)
                                     or self._causa_examinis(), ex))
            raise

    def _causa_examinis(self):
        """violatio examinis prima super copiam temporariam (cum clang
        tacet); './silva/censor.sh <via>' pro macro in declaratore."""
        d = os.path.dirname(_absoluta(self.via))
        fd, via_t = tempfile.mkstemp(prefix='.editio_', suffix='.c', dir=d)
        try:
            with os.fdopen(fd, 'w') as f:
                f.write(self.textus)
            r = _curre(['./silva/examen.sh', via_t, '-machina'])
            for o in _tsv(r.stdout):
                if o and o[0] != 'VERDICTUM' and len(o) >= 7:
                    return 'examen: %s (linea %s)' % (o[6], o[1])
            return 'causa ignota - ./silva/censor.sh %s' % self.via
        finally:
            os.unlink(via_t)

    def _lineae(self):
        return self.textus.splitlines(True)

    def substituere(self, nomen, novus, definitio=True, genus=None):
        """corpus nodi nomine substituere (commentarium ducens manet);
        genus='typus' pro structura/unione/enumeratione (aut genus
        exactum: 'structura' ...) - sine genere functiones solae."""
        x = self._extentum_praesens(nomen, definitio, genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_nodi - 1]) + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('substituere %s' % nomen)
        return self

    def inserere_post(self, nomen, novus, definitio=True, genus=None):
        x = self._extentum_praesens(nomen, definitio, genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_b]) + '\n' + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('inserere_post %s' % nomen)
        return self

    def inserere_ante(self, nomen, novus, definitio=True, genus=None):
        x = self._extentum_praesens(nomen, definitio, genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        # ante nodum ipsum (post commentarium ducens functionis sequentis?
        # non: ante commentarium - extentum incipit post nodum priorem)
        self.textus = ''.join(lineae[:x.linea_a - 1]) + novus + '\n' \
            + ''.join(lineae[x.linea_a - 1:])
        self.acta.append('inserere_ante %s' % nomen)
        return self

    def commentum(self, nomen, novus, definitio=True, genus=None):
        """commentarium DUCENS nodi nominati substituere (substituere
        corpus solum tangit et commentarium servat - hic via ad ipsum):
        lineae a '/*' primo intra [linea_a, linea_nodi) usque ad lineam
        ante nodum; sine commentario novus ante nodum inseritur."""
        x = self._extentum_praesens(nomen, definitio, genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        initium = None
        for i in range(x.linea_a - 1, x.linea_nodi - 1):
            if lineae[i].lstrip().startswith('/*'):
                initium = i
                break
        if initium is None:
            initium = x.linea_nodi - 1
        self.textus = ''.join(lineae[:initium]) + novus \
            + ''.join(lineae[x.linea_nodi - 1:])
        self.acta.append('commentum %s' % nomen)
        return self

    def inserere_ante_vocationem(self, functio, novus):
        """novus ante lineam UNICAM quae 'functio(' vocat (e.g. casus
        probationis ante 'credo_imprimere_compendium();' - forma quater
        manu ancorata in B1); vocatio nulla aut plures = SilvaError."""
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        exemplar = re.compile(r'^\s*' + re.escape(functio) + r'\s*\(')
        sedes = [i for i, l in enumerate(lineae) if exemplar.match(l)]
        if len(sedes) != 1:
            raise SilvaError("vocatio '%s(' %d vicibus inventa (exspectata 1)"
                             % (functio, len(sedes)))
        i = sedes[0]
        self.textus = ''.join(lineae[:i]) + novus + ''.join(lineae[i:])
        self.acta.append('inserere_ante_vocationem %s' % functio)
        return self

    def membrum_addere(self, typus, textus, post=None):
        """membrum structurae/unionis/enumerationis nomine typi addere:
        ante lineam claudentem ('} Titulus;'), aut post membrum cuius
        linea 'post' (ancora tolerans intra typum) continet. Forma
        (applicare) columnas ordinat."""
        x = self._extentum_praesens(typus, genus='typus')
        if not textus.endswith('\n'):
            textus += '\n'
        lineae = self._lineae()
        a, b = x.linea_nodi - 1, x.linea_b      # [a, b) 0-basatae
        sedes = b - 1                           # linea claudens
        if post is not None:
            ex = _exemplar_tolerans(post)
            hits = [i for i in range(a, b) if ex.search(lineae[i])]
            if len(hits) != 1:
                raise SilvaError("membrum 'post' %d vicibus in %s: %r"
                                 % (len(hits), typus, post[:40]))
            sedes = hits[0] + 1
        self.textus = ''.join(lineae[:sedes]) + textus \
            + ''.join(lineae[sedes:])
        self.acta.append('membrum_addere %s' % typus)
        return self

    def replace_selecta(self, selector, novus, intra=None):
        """sedes structuralis (selecta) in textu PRAESENTI, exacte una,
        lineis [a, b] substitutis - ancora sine textu litterali:
        'redde' -intra f, 'si', 'vocatio' ..."""
        d = os.path.dirname(_absoluta(self.via))
        fd, via_t = tempfile.mkstemp(prefix='.editio_', suffix='.c', dir=d)
        try:
            with os.fdopen(fd, 'w') as f:
                f.write(self.textus)
            sedes = selecta(via_t, selector, intra)
        finally:
            os.unlink(via_t)
        if len(sedes) != 1:
            raise SilvaError("selector %r %d sedes (exspectata una)"
                             % (selector, len(sedes)))
        x = sedes[0]
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_a - 1]) + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('replace_selecta %r' % selector)
        return self

    # -- exitus --
    def diff(self):
        return ''.join(difflib.unified_diff(
            self.originalis.splitlines(True), self.textus.splitlines(True),
            'a/' + self.via, 'b/' + self.via))

    def mutata(self):
        return self.textus != self.originalis

    # -- phases (Refactio eas trans plagulas ordinat) --
    def _custodire(self):
        """custos lectionis rancidae: discus == textus lectus"""
        if open(_absoluta(self.via)).read() != self.originalis:
            raise SilvaError('plagula in disco mutata post lectionem'
                             ' (%s) - relege ante editionem' % self.via)

    def _scribere(self):
        """scriptura cruda; reddit textum ante"""
        ante = self.originalis
        with open(_absoluta(self.via), 'w') as f:
            f.write(self.textus)
        return ante

    def _formare(self):
        formata = formare(self.via)
        self.textus = open(_absoluta(self.via)).read()
        return formata

    def _restituere(self, ante):
        with open(_absoluta(self.via), 'w') as f:
            f.write(ante)
        self.textus = ante

    def _fructus(self, ante, formata, iudica):
        d = ''.join(difflib.unified_diff(
            ante.splitlines(True), self.textus.splitlines(True),
            'a/' + self.via, 'b/' + self.via))
        verd = examen(self.via) if iudica else None
        dif = differre(ante, self.via) if iudica else None
        return Fructus(self.via, d, verd, formata, dif)

    def applicare(self, forma=True, iudica=True, strictum=False):
        """Scribit SEMEL (omnia aut nihil) et Fructum reddit.

        Portae in ordine: (1) custos lectionis rancidae - discus a textu
        lecto differens = alius scripsit interea (uncus, processus
        alius): SilvaError, nihil scriptum; (2) scriptura; (3) forma:
        formator plagulae totius (diff redditus = quod in disco est);
        (4) iudica: examen + differre contra textum ante editionem
        (unitates mutatae nominatae - mea substantiva, formatoris
        cosmetica). strictum: examen non ACCIPE -> textus ante
        editionem restituitur et SilvaError. Ordinarius: refertur,
        numquam tacite revertitur (status medius refactionis licitus
        est; uncus commissionis porta manet)."""
        self._custodire()
        if not self.mutata():
            return Fructus(self.via, '', None, False, None)
        ante = self._scribere()
        formata = self._formare() if forma else False
        f = self._fructus(ante, formata, iudica)
        if strictum and not f.sana:
            self._restituere(ante)
            raise SilvaError('examen %s - scriptura restituta (strictum)'
                             % f.examen)
        self.originalis = self.textus
        return f


class Fructus(object):
    """exitus applicare: diff (post formam), examen, formata, differentia.
    str(fructus) = compendium quattuor linearum + diff."""

    def __init__(self, via, diff, examen_verdictum, formata, differentia):
        self.via = via
        self.diff = diff
        self.examen = examen_verdictum
        self.formata = formata
        self.differentia = differentia

    @property
    def sana(self):
        return self.examen in (None, 'ACCIPE')

    def unitates(self):
        if self.differentia is None:
            return []
        return [(p.status, p.titulus, p.classificatio)
                for p in self.differentia.paria]

    def __str__(self):
        lineae = ['applicare %s: %s' % (self.via, 'mutata' if self.diff
                                         else 'intacta')]
        if self.examen is not None:
            lineae.append('  examen: %s' % self.examen)
        lineae.append('  forma: %s' % ('scripta' if self.formata
                                        else 'nihil'))
        if self.differentia is not None:
            lineae.append('  differre: %s %s' % (
                self.differentia.verdictum, self.unitates()))
        return '\n'.join(lineae) + ('\n' + self.diff if self.diff else '')


class FructusOmnes(list):
    """fructus transactionis (Refactio): lista Fructus per plagulam, cum
    .sana (omnes sanae) et str = compendia omnium - eadem assertio ac
    Fructus unius (f.sana), ne lista muta assertionem fallat"""

    @property
    def sana(self):
        return all(f.sana for f in self)

    def __str__(self):
        return '\n'.join(str(f) for f in self)


# ---------------------------------------------------------------- textus

class Textus(object):
    """Editio textus PLANI (sh, md, html, toml, py): ancorae exactae
    numero asserto (tolerans='spatia' cursus spatiorum), omnia aut
    nihil, custos lectionis rancidae, applicare() scribit SEMEL -
    sine formatore, sine examine: pro plagulis quas silva non legit.
    Idem mos ac Editio, ne 'lege, conta, substitue, scribe' sexies
    per diem manu iteretur."""

    def __init__(self, via):
        self.via = via
        self.originalis = open(_absoluta(via)).read()
        self.textus = self.originalis
        self.acta = []

    def replace(self, vetus, novus, numerus=1, tolerans=False):
        if tolerans == 'spatia':
            sedes = [m.span() for m in
                     _exemplar_tolerans(vetus).finditer(self.textus)]
        else:
            sedes = []
            i = self.textus.find(vetus)
            while i >= 0:
                sedes.append((i, i + len(vetus)))
                i = self.textus.find(vetus, i + 1)
        if len(sedes) != numerus:
            raise SilvaError("ancora %d vicibus inventa (exspectatae %d)%s:"
                             " %r" % (len(sedes), numerus,
                                      _lineae_sedium(self.textus, sedes),
                                      vetus[:60]))
        for a, b in reversed(sedes):
            self.textus = self.textus[:a] + novus + self.textus[b:]
        self.acta.append('replace %r' % vetus[:40])
        return self

    def inserere_post(self, vetus, novus, numerus=1):
        return self.replace(vetus, vetus + novus, numerus)

    def inserere_ante(self, vetus, novus, numerus=1):
        return self.replace(vetus, novus + vetus, numerus)

    def replace_inter(self, initium, finis, novus, inclusae=False):
        """spatium INTER 'initium' (semel) et 'finis' (primam post
        initium) substituere, exacte; ancorae SERVATAE nisi
        inclusae=True"""
        sedes = []
        i = self.textus.find(initium)
        while i >= 0:
            sedes.append((i, i + len(initium)))
            i = self.textus.find(initium, i + 1)
        if len(sedes) != 1:
            raise SilvaError("ancora initii %d vicibus%s: %r"
                             % (len(sedes), _lineae_sedium(self.textus, sedes),
                                initium[:60]))
        a0, a1 = sedes[0]
        b0 = self.textus.find(finis, a1)
        if b0 < 0:
            raise SilvaError("ancora finis post initium non inventa: %r"
                             % finis[:60])
        b1 = b0 + len(finis)
        if inclusae:
            self.textus = self.textus[:a0] + novus + self.textus[b1:]
        else:
            self.textus = self.textus[:a1] + novus + self.textus[b0:]
        self.acta.append('replace_inter %r..%r' % (initium[:20], finis[:20]))
        return self

    def appendere(self, novus):
        self.textus += novus
        self.acta.append('appendere')
        return self

    def diff(self):
        return ''.join(difflib.unified_diff(
            self.originalis.splitlines(True), self.textus.splitlines(True),
            'a/' + self.via, 'b/' + self.via))

    def mutata(self):
        return self.textus != self.originalis

    def applicare(self):
        """custos lectionis rancidae, scriptura semel; Fructus sine
        examine/forma/differentia"""
        if open(_absoluta(self.via)).read() != self.originalis:
            raise SilvaError('plagula in disco mutata post lectionem'
                             ' (%s) - relege ante editionem' % self.via)
        if not self.mutata():
            return Fructus(self.via, '', None, False, None)
        d = self.diff()
        with open(_absoluta(self.via), 'w') as f:
            f.write(self.textus)
        self.originalis = self.textus
        return Fructus(self.via, d, None, False, None)


# ---------------------------------------------------------------- prosa (md)

ProsaExtentum = namedtuple('ProsaExtentum',
                           'tag initium finis linea columna linea_finis versio'
                           ' caput_finis', defaults=(0,))


def prosa_extenta(via, selector, versio=0):
    """[ProsaExtentum] per md/extenta.sh (selectio super proiectionem md):
    OCTETI [initium, finis) in plagula, lineae/columnae I-basatae. rc 1 =
    nulla congruentia (lista vacua); rc 2 = fractura (selector, parsura)"""
    r = _curre(['./md/extenta.sh', _absoluta(via), selector])
    if r.returncode == 2:
        raise SilvaError('extenta fractus: %s' % r.stderr.strip()[-300:])
    exitus = []
    for linea in r.stdout.splitlines():
        if linea.startswith('#') or not linea.strip():
            continue
        p = linea.split('\t')
        exitus.append(ProsaExtentum(p[2], int(p[3]), int(p[4]), int(p[5]),
                                    int(p[6]), int(p[7]), versio))
    return exitus


class ProsaFructus(object):
    """fructus Prosa.applicare: diff, sana (parsura plagulae scriptae +
    extentum 'documentum' == plagula tota), ancorae {ancora: numerus post
    scripturam} - RELATAE, non assertae"""

    def __init__(self, via, diff, sana, ancorae, causa=''):
        self.via = via
        self.diff = diff
        self.sana = sana
        self.ancorae = ancorae
        self.causa = causa

    def __str__(self):
        lineae = ['applicare %s: %s' % (self.via, 'sana' if self.sana
                                        else 'NON SANA - ' + self.causa)]
        for k in sorted(self.ancorae):
            lineae.append('  ancora %s -> %d' % (k, self.ancorae[k]))
        return '\n'.join(lineae) + ('\n' + self.diff if self.diff else '')


class Prosa(object):
    """Editio markdown per ANCORAS STRUCTURALES (C1, spec md par. IX):
    Textus (custos lectionis rancidae, omnia aut nihil, scriptura semel,
    sine formatore) + extenta per md/extenta.sh. OCTETI, non characteres:
    plagula ut bytes tenetur (extenta octetim sunt), textus utf-8
    decodificatur. Extenta post editionem RANCESCUNT (offsets labuntur):
    quodque versionem editionis fert, methodus editionis extentum vetus
    refutat - reselige post editionem. sectio(titulus) = capitulum cum
    corpore usque ad capitulum proximum gradus aequalis aut superioris
    (lineae vacuae caudales exclusae, ut separatio post substitutionem
    maneat). applicare() scribit semel et IUDICAT: parsura super plagulam
    scriptam + extentum 'documentum' == plagula tota (lex octetorum); ancorae
    adhibitae iterum numerantur et REFERUNTUR (substitutio quae capitulum
    ipsum renominat licita est - relatio, non refusio)."""

    def __init__(self, via):
        self.via = via
        self.originalis = open(_absoluta(via), 'rb').read()
        self.octeti = self.originalis
        self.acta = []
        self.versio = 0
        self.ancorae = []

    # -- textus --
    @property
    def textus(self):
        return self.octeti.decode('utf-8', errors='replace')

    def corpus(self, x):
        """textus extenti (decodificatus)"""
        self._recens(x)
        return self.octeti[x.initium:x.finis].decode('utf-8',
                                                     errors='replace')

    # -- extenta --
    def _plagula_praesens(self):
        d = os.path.join(RADIX, 'build', 'pythonica')
        os.makedirs(d, exist_ok=True)
        fd, via_t = tempfile.mkstemp(prefix='.prosa_', suffix='.md', dir=d)
        with os.fdopen(fd, 'wb') as f:
            f.write(self.octeti)
        return via_t

    def selecta(self, selector):
        """[ProsaExtentum] per selectorem in textu PRAESENTI"""
        via_t = self._plagula_praesens()
        try:
            return prosa_extenta(via_t, selector, self.versio)
        finally:
            os.unlink(via_t)

    def _capitula(self):
        """[(extentum, gradus, titulus)] - titulus sine marcis (ATX: '#'
        ducentes et clausura; setext: subductio ablata)"""
        exitus = []
        for x in self.selecta('capitulum'):
            crudum = self.octeti[x.initium:x.finis]
            lineae = crudum.split(b'\n')
            prima = lineae[0].lstrip(b' ')
            if prima.startswith(b'#'):
                gradus = len(prima) - len(prima.lstrip(b'#'))
                t = prima[gradus:].strip()
                t = re.sub(rb'(^|\s)#+\s*$', b'', t).rstrip()
            else:
                plenae = [l for l in lineae if l.strip()]
                gradus = 1 if plenae[-1].strip().startswith(b'=') else 2
                t = b' '.join(l.strip() for l in plenae[:-1])
            exitus.append((x, gradus, t.decode('utf-8', errors='replace')))
        return exitus

    def _capitulum_index(self, titulus, gradus, incipit, continet):
        modi = [(k, v) for k, v in (('titulus', titulus), ('incipit', incipit),
                                    ('continet', continet)) if v is not None]
        if len(modi) != 1:
            raise SilvaError('capitulum: unum ex titulus / incipit / continet'
                             ' da (%d data)' % len(modi))
        modus, clavis = modi[0]
        congruit = {'titulus': lambda t: t == clavis,
                    'incipit': lambda t: t.startswith(clavis),
                    'continet': lambda t: clavis in t}[modus]
        c = self._capitula()
        hits = [i for i, (x, g, t) in enumerate(c)
                if congruit(t) and (gradus is None or g == gradus)]
        self.ancorae.append(('capitulum', clavis))
        if len(hits) != 1:
            if hits:
                praesentia = ', '.join('%r (h%d, linea %d)' % (c[i][2], c[i][1],
                                                              c[i][0].linea)
                                       for i in hits[:20])
            else:
                praesentia = ', '.join('%r (h%d, linea %d)' % (t, g, x.linea)
                                       for x, g, t in c[:40]) or '(nulla)'
            raise SilvaError('capitulum %s=%r %d vicibus inventum (exspectatum'
                             ' semel); %s: %s'
                             % (modus, clavis, len(hits),
                                'congruentia' if hits else 'praesentia',
                                praesentia))
        return hits[0], c

    def capitulum(self, titulus=None, gradus=None, incipit=None, continet=None):
        """extentum LINEAE capituli: titulus (textus sine marcis ==) aut
        incipit (praefixum) aut continet (pars); gradus filtrat (1..6);
        absens/ambiguum refutatur, capitula praesentia aut congruentia
        nominata"""
        i, c = self._capitulum_index(titulus, gradus, incipit, continet)
        return c[i][0]

    def sectio(self, titulus=None, gradus=None, incipit=None, continet=None):
        """capitulum CUM corpore: usque ad capitulum proximum gradus <=
        suo (ultimum: usque ad finem); lineae vacuae caudales exclusae;
        caput_finis = post lineam capituli (setext: post subductionem)"""
        i, c = self._capitulum_index(titulus, gradus, incipit, continet)
        x, g, _ = c[i]
        finis = len(self.octeti)
        for xj, gj, _ in c[i + 1:]:
            if gj <= g:
                finis = xj.initium
                break
        corpus = self.octeti[x.initium:finis]
        nudum = corpus.rstrip(b' \t\n')
        finis = x.initium + len(nudum) + (1 if corpus[len(nudum):]
                                          .startswith(b'\n') else 0)
        linea_finis = x.linea + self.octeti[x.initium:finis].count(b'\n') \
            - (1 if self.octeti[finis - 1:finis] == b'\n' else 0)
        return ProsaExtentum('sectio', x.initium, finis, x.linea, x.columna,
                             linea_finis, self.versio, x.finis)

    def paragraphum_addere(self, x, novus, ubi='finis'):
        """paragraphum sectioni (aut extento) addere cum separatione recta:
        ubi='finis' = post lineam contenti ultimam (linea vacua una inter,
        separatio ad capitulum proximum manet); ubi='initium' = post
        lineam capituli (setext: post subductionem), linea vacua ante et,
        si corpus statim sequitur, post"""
        self._recens(x)
        n = self._octeti_novi(novus)
        if not n.endswith(b'\n'):
            n += b'\n'
        if ubi == 'finis':
            sedes = x.finis
            n = b'\n' + n
        elif ubi == 'initium':
            sedes = x.caput_finis if x.caput_finis > x.initium \
                else self.octeti.find(b'\n', x.initium) + 1
            proxima = self.octeti[sedes:sedes + 1]
            n = b'\n' + n + (b'\n' if proxima not in (b'\n', b'') else b'')
        else:
            raise SilvaError("paragraphum_addere: ubi = 'finis' | 'initium'")
        self.octeti = self.octeti[:sedes] + n + self.octeti[sedes:]
        self.versio += 1
        self.acta.append('paragraphum_addere %s@%d %s' % (x.tag, x.linea, ubi))
        return self

    def _intra(self, xs, intra):
        if intra is None:
            return xs
        self._recens(intra)
        return [x for x in xs
                if x.initium >= intra.initium and x.finis <= intra.finis]

    def _n(self, quid, xs, n):
        if n < 0 or n >= len(xs):
            raise SilvaError('%s: %d praesentia, n=%d petitum'
                             % (quid, len(xs), n))
        return xs[n]

    def elementum(self, n=0, intra=None):
        """elementum listae n-tum (ordine documenti, nidificata inclusa);
        intra = extentum (sectio ...) quod ea continet"""
        self.ancorae.append(('elementum', n))
        return self._n('elementum', self._intra(self.selecta('elementum'),
                                                intra), n)

    def _lingua(self, x):
        prima = self.octeti[x.initium:x.finis].split(b'\n', 1)[0]
        info = prima.strip().lstrip(b'`~').strip()
        return info.split()[0].decode('utf-8', errors='replace') \
            if info else ''

    def saeptum(self, lingua=None, n=0, intra=None):
        """saeptum codicis n-tum (lingua = verbum primum info)"""
        self.ancorae.append(('saeptum', lingua))
        xs = self._intra(self.selecta('saeptum'), intra)
        if lingua is not None:
            xs = [x for x in xs if self._lingua(x) == lingua]
        return self._n('saeptum %r' % lingua, xs, n)

    def _sententiae(self, intra=None):
        """[ProsaExtentum 'sententia'] paragraphorum md intra extentum (aut
        omnium): paragraphi Orationi ut textus unus traditi (lineis vacuis
        separati), extenta in octetos plagulae remissa"""
        paragraphi = self._intra(self.selecta('paragraphus'), intra)
        partes, sedes, cursor = [], [], 0
        for x in paragraphi:
            corpus = self.octeti[x.initium:x.finis]
            if not corpus.endswith(b'\n'):
                corpus += b'\n'
            partes.append(corpus + b'\n')
            sedes.append((cursor, cursor + len(corpus), x))
            cursor += len(corpus) + 1
        xs = []
        if partes:
            for s in Oratio(b''.join(partes)).sententiae():
                for a, b, x in sedes:
                    if a <= s.initium and s.finis <= b:
                        initium = x.initium + (s.initium - a)
                        finis = x.initium + (s.finis - a)
                        linea = x.linea + self.octeti.count(b'\n', x.initium,
                                                            initium)
                        xs.append(ProsaExtentum(
                            'sententia', initium, finis, linea,
                            initium - self.octeti.rfind(b'\n', 0, initium),
                            linea + self.octeti.count(b'\n', initium, finis),
                            self.versio))
                        break
        return xs

    def sententia(self, n=0, intra=None):
        """sententia n-ta (ordine documenti) paragraphorum markdown intra
        extentum (sectio ...) aut totius plagulae - delegatio Orationi
        (T14): capitula, saepta, html numquam; elementum listae
        paragraphus est (nota '- ' intra sententiam primam); lineae novae
        intra sententiam manent; caudae exclusae - ancora pro
        substituere / inserere_post"""
        self.ancorae.append(('sententia', n))
        return self._n('sententia', self._sententiae(intra), n)

    # -- editiones --
    def _recens(self, x):
        if x.versio != self.versio:
            raise SilvaError('extentum rancidum (selectum in versione %d,'
                             ' praesens %d) - reselige post editionem'
                             % (x.versio, self.versio))

    def _octeti_novi(self, novus):
        return novus if isinstance(novus, bytes) else novus.encode('utf-8')

    def substituere(self, x, novus):
        """extentum substituere (octeti [initium, finis) -> novus)"""
        self._recens(x)
        self.octeti = self.octeti[:x.initium] + self._octeti_novi(novus) \
            + self.octeti[x.finis:]
        self.versio += 1
        self.acta.append('substituere %s@%d' % (x.tag, x.linea))
        return self

    def inserere_post(self, x, novus):
        self._recens(x)
        self.octeti = self.octeti[:x.finis] + self._octeti_novi(novus) \
            + self.octeti[x.finis:]
        self.versio += 1
        self.acta.append('inserere_post %s@%d' % (x.tag, x.linea))
        return self

    def inserere_ante(self, x, novus):
        self._recens(x)
        self.octeti = self.octeti[:x.initium] + self._octeti_novi(novus) \
            + self.octeti[x.initium:]
        self.versio += 1
        self.acta.append('inserere_ante %s@%d' % (x.tag, x.linea))
        return self

    def replace(self, vetus, novus, numerus=1):
        """ancora textualis EXACTA (ut Textus), numero asserto"""
        v = self._octeti_novi(vetus)
        sedes = []
        i = self.octeti.find(v)
        while i >= 0:
            sedes.append((i, i + len(v)))
            i = self.octeti.find(v, i + 1)
        if len(sedes) != numerus:
            raise SilvaError("ancora %d vicibus inventa (exspectatae %d)%s:"
                             " %r" % (len(sedes), numerus,
                                      _lineae_sedium(self.textus, [
                                          (len(self.octeti[:a].decode(
                                              'utf-8', errors='replace')), 0)
                                          for a, _ in sedes]),
                                      vetus[:60]))
        n = self._octeti_novi(novus)
        for a, b in reversed(sedes):
            self.octeti = self.octeti[:a] + n + self.octeti[b:]
        self.versio += 1
        self.acta.append('replace %r' % vetus[:40])
        return self

    def appendere(self, novus):
        self.octeti += self._octeti_novi(novus)
        self.versio += 1
        self.acta.append('appendere')
        return self

    # -- exitus --
    def diff(self):
        a = self.originalis.decode('utf-8', errors='replace')
        b = self.textus
        return ''.join(difflib.unified_diff(
            a.splitlines(True), b.splitlines(True),
            'a/' + self.via, 'b/' + self.via))

    def mutata(self):
        return self.octeti != self.originalis

    def _ancorae_numerare(self):
        numeri = {}
        capitula = None
        for genus, clavis in self.ancorae:
            if genus == 'capitulum':
                if capitula is None:
                    capitula = self._capitula()
                numeri['capitulum %r' % clavis] = sum(
                    1 for _, _, t in capitula if t == clavis)
            elif genus == 'sententia':
                numeri[genus] = len(self._sententiae())
            else:
                numeri[genus] = len(self.selecta(genus))
        return numeri

    def applicare(self):
        """custos lectionis rancidae, scriptura semel, iudex: parsura +
        extentum documenti == plagula tota; ancorae relatae"""
        if open(_absoluta(self.via), 'rb').read() != self.originalis:
            raise SilvaError('plagula in disco mutata post lectionem'
                             ' (%s) - relege ante editionem' % self.via)
        if not self.mutata():
            return ProsaFructus(self.via, '', True, {})
        d = self.diff()
        with open(_absoluta(self.via), 'wb') as f:
            f.write(self.octeti)
        self.originalis = self.octeti
        sana, causa = True, ''
        if self.octeti:
            try:
                doc = prosa_extenta(self.via, 'documentum')
                if len(doc) != 1 or doc[0].initium != 0 \
                        or doc[0].finis != len(self.octeti):
                    sana, causa = False, ('extentum documenti != plagula:'
                                          ' %r pro %d octetis'
                                          % (doc, len(self.octeti)))
            except SilvaError as ex:
                sana, causa = False, str(ex)
        return ProsaFructus(self.via, d, sana, self._ancorae_numerare(),
                            causa)


# ---------------------------------------------------------------- citata (md)

Citatum = namedtuple('Citatum', 'via linea textus genus verdictum sedes')
Citata = namedtuple('Citata', 'numeri absentia mota nuda')
_VIA_CITATA = re.compile(
    r'^(?:\./)?((?:\.\./)*[A-Za-z0-9_][A-Za-z0-9_./-]*\.(?:c|h|sh|md|py|stml|canon|tsv'
    r'|txt|json|toml|html|css|js))$')
_SYMBOLUM_CITATUM = re.compile(r'^([a-z_][a-z0-9_]*)\(\)?$')


def _nexus_definitiones():
    """symbola cum 'sedes' in build/nexus.tsv (tabula disponibilis nexus)"""
    via = os.path.join(RADIX, 'build', 'nexus.tsv')
    if not os.path.exists(via):
        raise SilvaError('build/nexus.tsv absens - ./silva/nexus.sh -renovare')
    sedes = set()
    with open(via, errors='replace') as f:
        for l in f:
            if l.startswith('#'):
                continue
            p = l.split('\t')
            if len(p) > 1 and p[1] == 'sedes':
                sedes.add(p[0])
    return sedes


def citata(via=None, praefixa=None):
    """CITATIONES in verbatim (code spans) corporis markdown contra arborem
    et tabulam symbolorum (md/census.sh -citata; putredo documentorum):
    VIAE (textus suffixo plagulae; relativae ad directorium plagulae md
    quoque) - adest | nudum (nomen sine directorio: plagulae eodem nomine
    in sedes nominatae) | motum (via cum directorio absens, plagulae eodem
    nomine alibi) | absens (nulla plagula eo nomine); SYMBOLA (nomen minusculum cum '('
    scriptum) - adest (sedes in nexus.tsv) | absens. via = plagula md una
    (etiam extra git); praefixa = filtrum viarum md corporis. Reddit
    Citata(numeri, absentia [Citatum], mota, nuda)."""
    args = ['./md/census.sh', '-citata'] + ([_absoluta(via)] if via else [])
    r = _curre(args)
    if r.returncode != 0:
        raise SilvaError('census -citata fractus: %s' % r.stderr.strip()[-200:])
    tractae = _curre(['git', 'ls-files']).stdout.splitlines()
    tractae_set = set(tractae)
    per_nomen = {}
    for t in tractae:
        per_nomen.setdefault(os.path.basename(t), []).append(t)
    definitiones = None
    absentia, mota, nuda = [], [], []
    numeri = dict((k, 0) for k in (
        'citata viae viae_adsunt viae_nudae viae_motae viae_absunt '
        'symbola symbola_adsunt symbola_absunt').split())
    for linea in r.stdout.splitlines():
        p = linea.split('\t', 2)
        if len(p) < 3:
            continue
        via_md, num, textus = p[0], int(p[1]), p[2].strip()
        if praefixa and not any(via_md.startswith(x) for x in praefixa):
            continue
        numeri['citata'] += 1
        m = _VIA_CITATA.match(textus)
        if m:
            numeri['viae'] += 1
            cand = m.group(1)
            relativa = os.path.normpath(os.path.join(os.path.dirname(via_md),
                                                     cand))
            if cand in tractae_set or relativa in tractae_set \
                    or os.path.exists(os.path.join(RADIX, cand)) \
                    or os.path.exists(os.path.join(RADIX, relativa)):
                numeri['viae_adsunt'] += 1
                continue
            alt = per_nomen.get(os.path.basename(cand), [])
            if not alt:
                numeri['viae_absunt'] += 1
                absentia.append(Citatum(via_md, num, textus, 'via', 'absens', ''))
            elif '/' not in cand:
                numeri['viae_nudae'] += 1
                nuda.append(Citatum(via_md, num, textus, 'via', 'nudum',
                                    ' '.join(alt[:3])))
            else:
                numeri['viae_motae'] += 1
                mota.append(Citatum(via_md, num, textus, 'via', 'motum',
                                    ' '.join(alt[:3])))
            continue
        m = _SYMBOLUM_CITATUM.match(textus)
        if m:
            if definitiones is None:
                definitiones = _nexus_definitiones()
            numeri['symbola'] += 1
            if m.group(1) in definitiones:
                numeri['symbola_adsunt'] += 1
            else:
                numeri['symbola_absunt'] += 1
                absentia.append(Citatum(via_md, num, textus, 'symbolum',
                                        'absens', ''))
    return Citata(numeri, absentia, mota, nuda)


def citata_textus(c, tectum=40):
    """compendium + absentia prima (via:linea textus)"""
    n = c.numeri
    lineae = ['citata %d: viae %d (adsunt %d, nudae %d, motae %d, ABSUNT %d);'
              ' symbola %d (adsunt %d, ABSUNT %d)'
              % (n['citata'], n['viae'], n['viae_adsunt'], n['viae_nudae'],
                 n['viae_motae'], n['viae_absunt'], n['symbola'],
                 n['symbola_adsunt'], n['symbola_absunt'])]
    per_dir = {}
    for x in c.absentia:
        d = x.via.split('/')[0] if '/' in x.via else '.'
        per_dir[d] = per_dir.get(d, 0) + 1
    if per_dir:
        lineae.append('absentia per directorium: ' + ', '.join(
            '%s %d' % kv for kv in sorted(per_dir.items(),
                                          key=lambda kv: -kv[1])[:8]))
    for x in c.absentia[:tectum]:
        lineae.append('  %s:%d  %s  (%s absens)' % (x.via, x.linea, x.textus,
                                                     x.genus))
    if len(c.absentia) > tectum:
        lineae.append('  ... %d cetera' % (len(c.absentia) - tectum))
    return '\n'.join(lineae)


# ---------------------------------------------------------------- portae

Porta = namedtuple('Porta', 'nomen cucurrit sana compendium rc acta fracturae',
                   defaults=(None,))
# fractura = probatio una fracta intra portam: nomen + lineae diagnosticae
# effusus SUI (FRACTA/Speratus/Receptus/compendium/error), non cauda actorum
Fractura = namedtuple('Fractura', 'nomen relatio')

# porta: (imperium, signum 'cucurrit' (regex)). 'cucurrit' = porta
# suam mensuram edidit - rc 0 sine signo = nihil cucurrit (exitus 2
# in cursoribus domus, sed signum id probat, non rc).
PORTAE = {
    'radix': (['./compile_tests.sh'],
              r'Tests Passed:|PROBATIONES: \d+/\d+'),
    'silva': (['./silva/compile_probationes.sh'],
              r'SILVA PROBATIONES: \d+/\d+ praeteritae'),
    'css': (['./css/compile_probationes.sh'], r'CSS PROBATIONES: \d+/\d+'),
    'materia': (['./materia/compile_probationes.sh'],
                r'MATERIA PROBATIONES: \d+/\d+'),
    'md': (['./md/compile_probationes.sh'], r'MD PROBATIONES: \d+/\d+'),
    'oratio': (['./oratio/compile_probationes.sh'],
               r'ORATIO PROBATIONES: \d+/\d+'),
    'officina': (['./officina/compile_probationes.sh'],
                 r'OFFICINA PROBATIONES: \d+/\d+'),
    'gesta': (['./gesta/compile_probationes.sh'],
              r'GESTA PROBATIONES: \d+/\d+'),
    'tessera': (['./tessera/compile_probationes.sh'],
                r'TESSERA PROBATIONES: \d+/\d+'),
    'saltuarius': (['./saltuarius/compile_probationes.sh'],
                   r'SALTUARIUS PROBATIONES: \d+/\d+'),
    'pythonica': (['./pythonica/probare.sh'], r'PYTHONICA: (sana|FRACTA)'),
    'formator-intra': (['./silva/formator_intra_fumus.sh'],
                       r'fumus intra: (sanum|FRACTUM)'),
    'formator-delta': (['./silva/formator_delta_fumus.sh'],
                       r'fumus delta: (sanum|FRACTUM)'),
    'differre': (['./silva/differre_fumus.sh'],
                 r'fumus differre: (sanum|FRACTUM)'),
    'unci': (['./tools/unci-git/fumus.sh'], r'fumus unci: (sanum|FRACTUM)'),
    'mensor-suitae': (['./tools/mensor_suitae_fumus.sh'],
                      r'fumus mensor_suitae: (sanum|FRACTUM)'),
    'vexilla': (['./tools/vexilla_fumus.sh'], r'fumus vexilla: (sanum|FRACTUM)'),
    'scribe': (['./silva/scribe_fumus.sh'], r'fumus scribe: (sanum|FRACTUM)'),
    'aequivalentia': (['./silva/aequivalentia.sh', '-conferre'],
                      r'aequivalentia: (sana|FRACTA)'),
    'amalgamata': (['./tools/amalgamata_probare.sh'],
                   r'amalgamata: \d+ compilata, \d+ fracta'),
    'aedilis': (['./tools/aedilis_porta.sh'],
                r'PORTA AEDILIS: \d+ probationes'),
    'materia-shim': (['./materia/shim_probare.sh'],
                     r'probatae \d+, fractae \d+'),
}
_ANSI = re.compile(r'\x1b\[[0-9;]*m')

# forma effusus portae: 'radix' (Testing: X ... ✗ TEST FAILED: X (t)),
# 'suita' (=== X === ... FRACTAE: X Y - silva et sub-suitae omnes),
# aliter 'generica' (porta tota = fractura una)
FORMAE = {'radix': 'radix', 'silva': 'suita', 'css': 'suita',
          'materia': 'suita', 'md': 'suita', 'oratio': 'suita',
          'officina': 'suita', 'gesta': 'suita',
          'tessera': 'suita', 'saltuarius': 'suita', 'aedilis': 'suita'}
_RELATIO_RE = re.compile(r'FRACTA|FRACTUM|FATALE|Speratus|Receptus|Totalis|'
                         r'Praeteriti|Fracti|Conditio|error:|Segmentation|'
                         r'Abort|exitus|FAILED|SINE VERDICTO')


def _relatio(lineae, tectum=14):
    """lineae diagnosticae ex effusu probationis unius; si nullae, cauda"""
    d = [l.rstrip() for l in lineae if _RELATIO_RE.search(l)]
    if not d:
        d = [l.rstrip() for l in lineae if l.strip()][-5:]
    if len(d) > tectum:
        d = d[:tectum - 1] + ['... (%d lineae plures)' % (len(d) - tectum + 1)]
    return '\n'.join(d)


def fracturae(acta, nomen='', forma=None):
    """probationes fractae ex actis portae: [Fractura(nomen, relatio)] -
    lectio actorum, non cursus. relatio = lineae diagnosticae effusus
    probationis IPSIUS (inter vexillum eius et verdictum), ut 'CXXIV
    assertiones, 0 fractae, exitus I' uno aspectu legatur."""
    forma = forma or FORMAE.get(nomen, 'generica')
    lineae = _ANSI.sub('', acta).splitlines()
    exitus = []
    if forma == 'radix':
        nomina = []
        for l in lineae:
            m = re.search(r'✗ (?:(?:TEST |COMPILATION |BUILD )?FAILED|'
                          r'SINE VERDICTO): (\S+)', l)
            if m and m.group(1) not in nomina:
                nomina.append(m.group(1))
        for n in nomina:
            a = b = None
            for i, l in enumerate(lineae):
                if a is None and l.strip() == 'Testing: ' + n:
                    a = i
                if re.search(r'FAILED: ' + re.escape(n) + r'\b', l):
                    b = i
                    break
            if a is None:
                a = max(-1, (b or 0) - 20)
            fin = len(lineae) if b is None else b + 1
            exitus.append(Fractura(n, _relatio(lineae[a + 1:fin])))
    elif forma == 'suita':
        m = re.search(r'^FRACTAE:(.*)$', '\n'.join(lineae), re.M)
        for n in (m.group(1).split() if m else []):
            a = None
            b = len(lineae)
            for i, l in enumerate(lineae):
                if a is None:
                    if l.strip() == '=== %s ===' % n:
                        a = i
                elif l.startswith('=== ') or l.startswith('====='):
                    b = i
                    break
            exitus.append(Fractura(n, _relatio(lineae[a + 1:b])
                                   if a is not None else '(effusus absens)'))
    else:
        if any(_RELATIO_RE.search(l) for l in lineae):
            exitus.append(Fractura(nomen or '?', _relatio(lineae)))
    return exitus


def relatio_fracturarum(fr, tectum=10):
    """textus relationis: nomina, deinde lineae diagnosticae per fracturam;
    '' si nullae. Hoc in errore commissionis et in recepto legitur."""
    if not fr:
        return ''
    partes = ['\nfractae (%d): %s' % (len(fr), ', '.join(f.nomen for f in fr))]
    for f in fr:
        ls = f.relatio.splitlines()
        partes.append('  --- ' + f.nomen)
        partes.extend('      ' + l for l in ls[:tectum])
        if len(ls) > tectum:
            partes.append('      ... (%d lineae plures)' % (len(ls) - tectum))
    return '\n'.join(partes)


def _summa_fracturae(f):
    """linea una quae fracturam nominat (FRACTA (...) / Conditio / error)"""
    for l in f.relatio.splitlines():
        if re.search(r'FRACTA \(|Conditio:|FATALE|error:|Segmentation', l):
            return l.strip()
    ls = f.relatio.splitlines()
    return ls[0].strip() if ls else '?'


def _portae_fictae():
    # portae fictae probationum: JSON {nomen: [imperium, signum, forma?]}
    # in ambitu - operarius umbrae (processus separatus, PORTAE suae)
    # easdem videt, aliter porta ficta ibi ignota est
    import json as _json
    try:
        d = _json.loads(os.environ.get('PYTHONICA_PORTAE_FICTAE', '{}'))
    except ValueError:
        return
    for k, v in d.items():
        PORTAE[k] = (list(v[0]), v[1])
        if len(v) > 2:
            FORMAE[k] = v[2]


_portae_fictae()


def porta_viae(via):
    """porta directorii plagulae: radix pro lib/include/probationes/tools"""
    v = via.replace('\\', '/')
    prima = v.split('/')[0]
    if prima in ('lib', 'include', 'probationes', 'tools'):
        return 'radix'
    if prima in PORTAE:
        return prima
    raise SilvaError('porta viae ignota: %s' % via)


def porta(nomen, filtrum=None, radix=None):
    """portam currere: Porta(nomen, cucurrit, sana, compendium, rc,
    acta). sana SOLUM si cucurrit ET rc == 0 ET signum non fractum.
    radix: directorium operis alterum (clone photographiae umbrae).
    NB: suites totae (radix, silva) minuta capiunt - in vocamine
    instrumenti tectum X minutorum: filtrum da aut in umbra curre."""
    if nomen not in PORTAE:
        raise SilvaError('porta ignota: %s (nota: %s)'
                         % (nomen, ', '.join(sorted(PORTAE))))
    imperium, signum = PORTAE[nomen]
    args = list(imperium) + ([filtrum] if filtrum else [])
    r = _curre(args, cwd=radix)
    acta = _ANSI.sub('', r.stdout + r.stderr)
    m = re.search(signum, acta)
    cucurrit = m is not None
    compendium = m.group(0) if m else '(signum absens)'
    sana = cucurrit and r.returncode == 0 \
        and not re.search(r'FRACT|Fracti:\s*[1-9]|Failed:\s*[1-9]',
                          compendium)
    fr = [] if sana else fracturae(acta, nomen)
    return Porta(nomen, cucurrit, sana, compendium, r.returncode, acta, fr)


VETITAE = ('FAQ.md', 'gesta/annales/tabula.md',
           'gesta/annales/tabularium.jsonl',
           'officina/instrumenta/legatus.worklog.md',
           'silva/grammatica/c89-formatted.stml')


def _trailer():
    r = _curre(['git', 'log', '-30', '--format=%B'])
    for linea in r.stdout.splitlines():
        if linea.startswith('Co-Authored-By:'):
            return linea
    return 'Co-Authored-By: Claude <noreply@anthropic.com>'


def commissio(nuntius, viae, portae=(), verificare=True):
    """gate, deinde commissio - uno vocamine, in Pythone (crusta 'set -e'
    non honorat; pipestatus fallit). portae: nomina aut (nomen,
    filtrum); OMNES sanae esse debent (cucurrit ET rc 0) aliter
    SilvaError et nihil commissum. Viae explicitae solae (numquam -A);
    VETITAE (plagulae Frani in cursu) refutantur. verificare=False =
    --no-verify (commissiones formae solae). Reddit hash brevem."""
    for v in viae:
        if v in VETITAE:
            raise SilvaError('via vetita commissioni: %s' % v)
    for p in portae:
        if isinstance(p, str) and p.endswith('.json'):
            f = receptum_validum(p, viae)      # porta ex umbra
            nomen = f.nomen + ' (receptum)'
        else:
            nomen, filtrum = (p, None) if isinstance(p, str) else p
            f = porta(nomen, filtrum)
        if not f.sana:
            # acta portae foris currentis nusquam servata erant: fractura
            # sine causa visibili (2026-09-03, porta pythonica sub md)
            os.makedirs(PORTAE_DIR, exist_ok=True)
            via_acta = os.path.join(PORTAE_DIR, re.sub(r'[^A-Za-z0-9_.-]+', '_', nomen)
                                    + '.fracta.acta')
            with open(via_acta, 'w') as fa:
                fa.write(f.acta or '')
            raise SilvaError('porta %s non sana (%s, rc=%d) - nihil'
                             ' commissum; acta: %s%s'
                             % (nomen, f.compendium, f.rc, via_acta,
                                relatio_fracturarum(f.fracturae)))
    # renominationes/deletiones per git mv/rm iam in indice: via absens
    # in disco licet si deletio eius in indice stat (viae NOVAE dantur)
    deletae = set(_curre(['git', 'diff', '--cached', '--name-only',
                          '--no-renames', '--diff-filter=D']).stdout.split())
    addendae = []
    for v in viae:
        if os.path.exists(_absoluta(v)):
            addendae.append(v)
        elif v not in deletae:
            raise SilvaError('via nec in disco nec deleta in indice: %s' % v)
    if addendae:
        r = _curre(['git', 'add', '--'] + addendae)
        if r.returncode != 0:
            raise SilvaError('git add: %s' % r.stderr.strip())
    args = ['git', 'commit', '-q']
    if not verificare:
        args.append('--no-verify')
    args += ['-F', '-']
    r = _curre(args, stdin=nuntius.rstrip('\n') + '\n\n' + _trailer() + '\n')
    if r.returncode != 0:
        raise SilvaError('git commit rc=%d: %s'
                         % (r.returncode, (r.stdout + r.stderr).strip()[-600:]))
    return _curre(['git', 'rev-parse', '--short', 'HEAD']).stdout.strip()


# ---------------------------------------------------------------- umbra

import hashlib
import time

PORTAE_DIR = os.path.join(RADIX, 'build', 'portae')
Receptum = namedtuple('Receptum', 'via nomen filtrum sana cucurrit '
                      'compendium rc sigillum rancida finis fracturae '
                      'photographia', defaults=([], None))


def sigillum_arboris():
    """sigillum status arboris: HEAD + differentia plagularum tractarum
    (index et arbor) + plagulae novae non ignoratae cum contentu.
    Receptum portae huic sigillo ligatur - mutatio ulla = rancidum."""
    h = hashlib.sha256()
    h.update(_curre(['git', 'rev-parse', 'HEAD']).stdout.encode())
    # VETITAE (plagulae Frani in cursu, numquam commissae per hoc) non
    # sigillantur: acta tabularii tracta sunt et per notas mutantur -
    # sine hoc nota inter portam et commissionem receptum rancidum
    # faciebat (2026-09-02)
    h.update(_curre(['git', 'diff', 'HEAD', '--', '.']
                    + [':(exclude)' + v for v in VETITAE]
                    ).stdout.encode('utf-8', 'replace'))
    novae = [v for v in _curre(['git', 'ls-files', '--others',
                                '--exclude-standard']).stdout.split()
             if v not in VETITAE]
    for v in sorted(novae):
        h.update(v.encode())
        try:
            with open(os.path.join(RADIX, v), 'rb') as f:
                h.update(hashlib.sha256(f.read()).digest())
        except (IOError, OSError):
            h.update(b'?')
    return h.hexdigest()[:16]


# ---------------------------------------------------------------- photographia

# Umbra PHOTOGRAPHICA: porta contra statum operis CAPTUM currit, non
# contra arborem vivam - editio pergit dum porta currit (2026-09-02
# vesperae VI portae ~XX minuta otiosa). Photographia = arbor git status
# operis (index temporarius + add -A + write-tree), materializata ut
# clone localis sine checkout (.git verum, obiecta hardlinked:
# bibliotheca git domus et mensor HEAD legunt; worktree git '.git'
# plagulam dat, clone '-s' alternates quas bibliotheca ignorat) + read-tree
# arboris + clonatio (copy-on-write, APFS) rerum ignoratarum (bin/,
# build/, book_assets/...). Receptum sigillum = arbor photographiae;
# commissio plagulas SUAS contra blobs arboris confert, non arborem
# totam: quod committitur est quod probatum est, cetera libera.
UMBRAE_DIR = os.path.expanduser('~/.rhubarb/umbrae')
Photographia = namedtuple('Photographia', 'arbor basis via')
_IGNORATA_NON_CLONANDA = ('.superpowers', '.claude', '.DS_Store',
                          'tabularium.db', 'tabularium.db-wal',
                          'tabularium.db-shm')


def photographia_capere():
    """status operis (tractae mutatae + novae non ignoratae; VETITAE
    inclusae - photographia non est commissio) ut arbor git, arbore viva
    intacta. Reddit Photographia(arbor, basis=HEAD, via=None)."""
    import shutil
    index = _absoluta(_curre(['git', 'rev-parse', '--git-path',
                              'index']).stdout.strip())
    tmp = tempfile.NamedTemporaryFile(prefix='index.umbra.', delete=False)
    tmp.close()
    try:
        if os.path.exists(index):
            shutil.copyfile(index, tmp.name)
        else:
            os.unlink(tmp.name)
        ambitus = dict(os.environ, GIT_INDEX_FILE=tmp.name)
        r = subprocess.run(['git', 'add', '-A', '--', '.'], cwd=RADIX,
                           env=ambitus, capture_output=True, text=True)
        if r.returncode != 0:
            raise SilvaError('photographia: git add: %s'
                             % r.stderr.strip()[-300:])
        r = subprocess.run(['git', 'write-tree'], cwd=RADIX, env=ambitus,
                           capture_output=True, text=True)
        if r.returncode != 0:
            raise SilvaError('photographia: write-tree: %s'
                             % r.stderr.strip()[-300:])
        arbor = r.stdout.strip()
    finally:
        try:
            os.unlink(tmp.name)
        except OSError:
            pass
    basis = _curre(['git', 'rev-parse', 'HEAD']).stdout.strip()
    return Photographia(arbor, basis, None)


def photographia_continet(arbor, via):
    """VERUM si contentum PRAESENS plagulae 'via' idem est ac blob eius
    in arbore photographiae (absens utrimque = VERUM): 'quod committo
    est quod probatum est'"""
    r = _curre(['git', 'ls-tree', arbor, '--', via])
    in_arbore = r.stdout.split('\t')[0].split()[2] if r.stdout.strip() \
        else None
    if not os.path.exists(_absoluta(via)):
        return in_arbore is None
    if in_arbore is None:
        return False
    nunc = _curre(['git', 'hash-object', '--', via]).stdout.strip()
    return nunc == in_arbore


def _ignorata():
    r = _curre(['git', 'ls-files', '--others', '--ignored',
                '--exclude-standard', '--directory'])
    exitus = []
    for l in r.stdout.splitlines():
        if not l:
            continue
        primum = l.rstrip('/').split('/')[0]
        if primum in _IGNORATA_NON_CLONANDA or l.endswith('.DS_Store'):
            continue
        exitus.append(l)
    return exitus


def _clonare_ignorata(ad):
    """res ignoratae (bin/, build/, silva/build/, book_assets/*.txt ...)
    in clone: cp -c (clonefile, copy-on-write) - spatium nullum donec
    scribantur; directoria singula, plagulae fascibus per directorium"""
    per_dir = {}
    for l in _ignorata():
        if l.endswith('/'):
            d = l.rstrip('/')
            os.makedirs(os.path.join(ad, os.path.dirname(d)), exist_ok=True)
            r = subprocess.run(['cp', '-c', '-R', os.path.join(RADIX, d),
                                os.path.join(ad, d)], capture_output=True,
                               text=True)
            if r.returncode != 0:
                raise SilvaError('clonatio %s: %s' % (d, r.stderr.strip()[-200:]))
        else:
            per_dir.setdefault(os.path.dirname(l), []).append(l)
    for d, plagulae in per_dir.items():
        dest = os.path.join(ad, d) if d else ad
        os.makedirs(dest, exist_ok=True)
        for i in range(0, len(plagulae), 500):
            r = subprocess.run(['cp', '-c'] + [os.path.join(RADIX, f) for f
                                                in plagulae[i:i + 500]]
                               + [dest], capture_output=True, text=True)
            if r.returncode != 0:
                raise SilvaError('clonatio %s: %s' % (d, r.stderr.strip()[-200:]))


def _tempora_speculari(ad, arbor):
    """tempora (mtime) plagularum tractarum arboris vivae in clone
    speculari. read-tree omnes 'nunc' stampat, res ignoratae (bin/,
    build/ per clonefile) tempora sua servant - ordo temporum in clone
    INVERSUS fit et custodes binariorum ('fons recentior binario':
    probatio_natura_glossae/canones) in umbra RUBRI mentiuntur
    (2026-09-03, porta radicis umbrae B1.1 md). Speculum ordinem vivum
    reddit; plagula viva absens (nata post captum, deleta) tempus
    checkout servat."""
    r = _curre(['git', 'ls-tree', '-r', '-z', '--name-only', arbor])
    for p in r.stdout.split('\0'):
        if not p:
            continue
        try:
            st = os.stat(os.path.join(RADIX, p))
            os.utime(os.path.join(ad, p), ns=(st.st_atime_ns, st.st_mtime_ns))
        except OSError:
            pass


def photographia_materializare(ph, nomen='umbra'):
    """photographiam ut directorium operis vivum: clone localis sine
    checkout (obiecta hardlinked, .git verum, HEAD = basis), read-tree
    arboris, res ignoratae clonatae. Reddit ph cum via."""
    _nomen_planum(nomen, 'nomen photographiae')
    os.makedirs(UMBRAE_DIR, exist_ok=True)
    via = os.path.join(UMBRAE_DIR, '%s.%d' % (nomen, int(time.time() * 1000)))
    # clone LOCALIS (obiecta per hardlinks), non '-s' (alternates):
    # bibliotheca git domus alternates non sequitur - in clone
    # communi HEAD vacuum videbat, differre_git plagulam totam
    # ADDITAM dicebat (porta prima rubra, 2026-09-02)
    r = _curre(['git', 'clone', '-q', '-n', RADIX, via])
    if r.returncode != 0:
        raise SilvaError('photographia: clone: %s' % r.stderr.strip()[-300:])
    r = subprocess.run(['git', 'read-tree', '--reset', '-u', ph.arbor],
                       cwd=via, capture_output=True, text=True)
    if r.returncode != 0:
        raise SilvaError('photographia: read-tree: %s'
                         % r.stderr.strip()[-300:])
    _clonare_ignorata(via)
    _tempora_speculari(via, ph.arbor)
    return ph._replace(via=via)


def photographia_delere(ph_aut_via):
    import shutil
    via = ph_aut_via.via if hasattr(ph_aut_via, 'via') else ph_aut_via
    if via and via.startswith(UMBRAE_DIR) and os.path.isdir(via):
        shutil.rmtree(via, ignore_errors=True)


def _receptum_via(nomen, filtrum):
    _nomen_planum(nomen, 'nomen portae')
    if filtrum:
        _nomen_planum(filtrum, 'filtrum')
    return os.path.join(PORTAE_DIR, '%s%s.%d.json' % (
        nomen, '.' + filtrum if filtrum else '', int(time.time())))


def porta_umbra(nomen, filtrum=None, photographica=True):
    """portam in umbra currere (processus separatus, sessio propria):
    receptum JSON in build/portae/ scriptum in fine. photographica
    (ordinarius): porta contra PHOTOGRAPHIAM status operis currit (clone
    in ~/.rhubarb/umbrae) - editio pergere licet; commissio plagulas
    suas contra blobs photographiae confert. photographica=False: mos
    vetus, sigillum arboris vivae (rancidum si quid mutatum). Reddit
    viam recepti (pendens: '.pendens' iuxta eam dum currit).
    exspectare(via) manet; commissio(portae=[via]) receptum accipit."""
    if nomen not in PORTAE:
        raise SilvaError('porta ignota: %s' % nomen)
    os.makedirs(PORTAE_DIR, exist_ok=True)
    via = _receptum_via(nomen, filtrum)      # nomina plana hic iudicata
    extra = []
    if photographica:
        ph = photographia_materializare(photographia_capere(), nomen)
        extra = [ph.via, ph.arbor, ph.basis]
        # sera processus: clone vivus dum PID vivit (lanceans nunc,
        # operarius post furcam) - umbrae_purgare ex quovis contextu
        # eum servat
        open(os.path.join(ph.via, '.umbra.pid'), 'w').write(str(os.getpid()))
    p = subprocess.Popen(
        [sys.executable, os.path.abspath(__file__), '-umbra', nomen,
         filtrum or '', via] + extra, cwd=RADIX, start_new_session=True,
        stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL)
    # signum pendens = PID operarii (linea I; operarius mortuus sine
    # recepto nominatur - portae_pendentes 'mortua', exspectare levat
    # statim) + via clonis (linea II; umbrae_purgare eum vivum tenet)
    open(via + '.pendens', 'w').write('%d\n%s\n' % (p.pid, extra[0] if extra else ''))
    if extra:
        open(os.path.join(extra[0], '.umbra.pid'), 'w').write(str(p.pid))
    return via


def _pendens_mortua(via):
    """VERUM si signum pendens PID mortuum fert (operarius abiit sine
    recepto); FALSUM si vivus aut signum vetus sine PID"""
    try:
        pid = int(open(via + '.pendens').read().strip().splitlines()[0])
    except (IOError, OSError, ValueError, IndexError):
        return False
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return True
    except OSError:
        return False
    return False


def _umbra_currere(nomen, filtrum, via, via_operis=None, arbor=None,
                   basis=None):
    """corpus operarii umbrae (processus separatus). Ruina QUAELIBET
    receptum FRACTUM scribit (traceback in .acta) et signum pendens
    tollit - operarius numquam tacite abit. via_operis/arbor/basis:
    umbra photographica (porta in clone, sigillum = arbor)."""
    import traceback
    photo = ({'via_operis': via_operis, 'arbor': arbor, 'basis': basis}
             if via_operis else None)
    sig = '?'
    try:
        sig = arbor if photo else sigillum_arboris()
        p = porta(nomen, filtrum or None, radix=via_operis)
        acta = p.acta
        d = {'nomen': nomen, 'filtrum': filtrum or None, 'sana': p.sana,
             'cucurrit': p.cucurrit, 'compendium': p.compendium,
             'rc': p.rc, 'sigillum': sig,
             'rancida': False if photo else sig != sigillum_arboris(),
             'finis': time.time(),
             'fracturae': [list(f) for f in (p.fracturae or [])],
             'photographia': photo}
    except Exception:
        acta = traceback.format_exc()
        ultima = acta.strip().splitlines()[-1][:200]
        d = {'nomen': nomen, 'filtrum': filtrum or None, 'sana': False,
             'cucurrit': False, 'compendium': 'UMBRA FRACTA: ' + ultima,
             'rc': -1, 'sigillum': sig, 'rancida': False,
             'finis': time.time(), 'fracturae': [], 'photographia': photo}
    with open(via + '.acta', 'w') as f:
        f.write(acta)
    with open(via + '.tmp', 'w') as f:
        json.dump(d, f)
    os.rename(via + '.tmp', via)
    try:
        os.unlink(via + '.pendens')
    except OSError:
        pass


def receptum_legere(via):
    if not os.path.exists(via):
        if os.path.exists(via + '.pendens'):
            raise SilvaError('receptum pendens (porta adhuc currit): %s'
                             % via)
        raise SilvaError('receptum absens: %s' % via)
    with open(via) as f:
        d = json.load(f)
    return Receptum(via, d['nomen'], d['filtrum'], d['sana'], d['cucurrit'],
                    d['compendium'], d['rc'], d['sigillum'], d['rancida'],
                    d['finis'],
                    [Fractura(*f) for f in d.get('fracturae', [])],
                    d.get('photographia'))


def receptum_validum(via, viae=None):
    """Porta ex recepto. Photographicum: sana si receptum sanum, HEAD =
    basis, et plagulae 'viae' (quae committentur) contentu idem ac in
    arbore photographiae - cetera libera. Vetus: sigillum arboris vivae
    idem (nihil mutatum post cursum)."""
    r = receptum_legere(via)
    causa = r.compendium
    if r.photographia:
        basis_eadem = _curre(['git', 'rev-parse', 'HEAD']).stdout.strip() \
            == r.photographia['basis']
        mutatae = [v for v in (viae or [])
                   if not photographia_continet(r.photographia['arbor'], v)]
        recens = basis_eadem and not mutatae
        if not basis_eadem:
            causa += ' [HEAD mutatum post photographiam]'
        if mutatae:
            causa += ' [plagulae post photographiam mutatae aut extra eam:'
            causa += ' %s]' % ', '.join(mutatae[:5])
    else:
        recens = r.sigillum == sigillum_arboris()
        if r.rancida:
            causa += ' [arbor mutata DUM currebat]'
        elif not recens:
            causa += ' [arbor mutata POST cursum - receptum rancidum]'
    sana = r.sana and r.cucurrit and not r.rancida and recens
    return Porta(r.nomen, r.cucurrit, sana, causa, r.rc, '', r.fracturae)


def receptum_relatio(via):
    """relatio recepti, etiam diu post cursum: nomen, compendium,
    probationes fractae cum lineis diagnosticis suis - lectio sola"""
    r = receptum_legere(via)
    caput = '%s: %s [%s]' % (r.nomen, r.compendium,
                             'sana' if r.sana else 'FRACTA')
    return caput + relatio_fracturarum(r.fracturae)


def receptum_delere(via):
    """receptum cum actis et signo pendenti delere - post consumptionem
    (commissio) aut in fine probationis; absens = nihil. Clone
    photographiae (si adest) quoque sublatus."""
    try:
        r = receptum_legere(via)
        if r.photographia and r.photographia.get('via_operis'):
            photographia_delere(r.photographia['via_operis'])
    except SilvaError:
        pass
    for suffixum in ('', '.acta', '.pendens', '.tmp'):
        try:
            os.unlink(via + suffixum)
        except OSError:
            pass


def exspectare(via, tectum=1800, intervallum=2.0):
    """receptum manere (secunda); SilvaError post tectum"""
    finis = time.time() + tectum
    while time.time() < finis:
        if os.path.exists(via):
            return receptum_legere(via)
        if _pendens_mortua(via):
            raise SilvaError('operarius umbrae mortuus sine recepto: %s'
                             % via)
        time.sleep(intervallum)
    raise SilvaError('receptum non venit intra %ds: %s' % (tectum, via))


def portae_pendentes():
    """[(via, status)] status: pendens | sana | fracta | rancida -
    orientatio post compactionem: quid in cursu erat"""
    if not os.path.isdir(PORTAE_DIR):
        return []
    exitus = []
    sig = sigillum_arboris()
    caput = _curre(['git', 'rev-parse', 'HEAD']).stdout.strip()
    for f in sorted(os.listdir(PORTAE_DIR)):
        via = os.path.join(PORTAE_DIR, f)
        if f.endswith('.pendens'):
            exitus.append((via[:-8], 'mortua (operarius abest, receptum'
                           ' nullum)' if _pendens_mortua(via[:-8])
                           else 'pendens'))
        elif f.endswith('.json'):
            r = receptum_legere(via)
            if r.photographia:
                rancida = r.rancida or r.photographia.get('basis') != caput
            else:
                rancida = r.rancida or r.sigillum != sig
            if rancida:
                exitus.append((via, 'rancida'))
            else:
                exitus.append((via, 'sana' if r.sana else 'fracta'))
    for via in umbrae_orphanae():
        exitus.append((via, 'clone orphanus'))
    return exitus


def _pid_vivus(pid):
    try:
        os.kill(int(pid), 0)
    except (ProcessLookupError, ValueError, TypeError):
        return False
    except OSError:
        return True
    return True


def _clones_vivi():
    """clones quos receptum, signum pendens, aut PROCESSUS VIVUS
    (<clone>/.umbra.pid) tenet - trans contextus: porta in clone
    currens sua recepta non videt (build/portae eius alter est), sed
    PID eius vivus est ubique (porta pythonicae in clone se ipsam
    delevit, 2026-09-02)"""
    vivi = set()
    if os.path.isdir(PORTAE_DIR):
        for f in os.listdir(PORTAE_DIR):
            via = os.path.join(PORTAE_DIR, f)
            if f.endswith('.json'):
                try:
                    r = receptum_legere(via)
                    if r.photographia and r.photographia.get('via_operis'):
                        vivi.add(r.photographia['via_operis'])
                except (SilvaError, ValueError, KeyError):
                    pass
            elif f.endswith('.pendens'):
                lineae = open(via).read().splitlines()
                if len(lineae) > 1 and lineae[1]:
                    vivi.add(lineae[1])
    if os.path.isdir(UMBRAE_DIR):
        for f in os.listdir(UMBRAE_DIR):
            via = os.path.join(UMBRAE_DIR, f)
            sera = os.path.join(via, '.umbra.pid')
            if os.path.exists(sera):
                try:
                    if _pid_vivus(open(sera).read().strip()):
                        vivi.add(via)
                except (IOError, OSError):
                    pass
    return vivi


def umbrae_orphanae():
    """clones sine recepto, sine signo pendenti, sine processu vivo
    (lectio sola)"""
    if not os.path.isdir(UMBRAE_DIR):
        return []
    vivi = _clones_vivi()
    return [os.path.join(UMBRAE_DIR, f) for f in sorted(os.listdir(UMBRAE_DIR))
            if os.path.isdir(os.path.join(UMBRAE_DIR, f))
            and os.path.join(UMBRAE_DIR, f) not in vivi]


def umbrae_purgare():
    """clones orphanos delere (umbrae_orphanae) - porta fracta clonem
    servat donec receptum deleatur, oblivio eos cumulat (~C MB veri
    quisque). Reddit vias deletas."""
    deletae = []
    for via in umbrae_orphanae():
        photographia_delere(via)
        deletae.append(via)
    return deletae


def _totum_actorum(acta):
    m = re.search(r'Total Time:\s*\S*?([\d.]+)s', acta)
    return float(m.group(1)) if m else None


def commissio_umbra(nuntius, viae, portae, verificare=True, tectum=1800,
                    siccum=False):
    """Portae umbrae SERIATIM (tempora non contendunt - mensurae suitae
    fidae manent), quaeque in clone photographiae suae (editio pergit),
    deinde commissio contra recepta OMNIA (plagulae viae contra blobs
    photographiae), recepta et clones deleti. Porta fracta: SilvaError
    cum probationibus fractis et lineis diagnosticis earum (fracturae);
    receptum et clone servantur ad inspectionem (receptum_relatio,
    receptum_delere; umbrae_purgare orphanos). siccum: portae solae,
    nihil commissum. Reddit (hash | None, [(nomen, compendium,
    totum_secunda)])."""
    recepta = []
    for p in portae:
        nomen, filtrum = (p, None) if isinstance(p, str) else p
        via = porta_umbra(nomen, filtrum)
        r = exspectare(via, tectum)
        acta = open(via + '.acta').read()
        tot = _totum_actorum(acta)
        if tot is None:
            praef = {'radix': '', 'silva': 'silva.', 'css': 'css.',
                     'materia': 'materia.', 'md': 'md.',
                     'oratio': 'oratio.'}.get(nomen)
            if praef is not None:
                ss = mensurae(praef, 1, plenae=False)
                if ss and (time.time() - float(ss[0].mensurae.get('suita.tempus.totum', 0)) > 0):
                    tot = ss[0].mensurae.get('suita.tempus.totum')
        recepta.append((via, nomen, r, tot))
        if not r.sana:
            rel = relatio_fracturarum(r.fracturae) or '\n' + acta[-800:]
            raise SilvaError('porta umbrae %s non sana: %s%s\n(acta: %s.acta;'
                             ' receptum et clone servata - post inspectionem'
                             ' silva.receptum_delere(%r))'
                             % (nomen, r.compendium, rel, via, via))
    h = None
    if not siccum:
        h = commissio(nuntius, viae, portae=[v for v, _, _, _ in recepta],
                      verificare=verificare)
    for v, _, _, _ in recepta:
        receptum_delere(v)
    return h, [(n, r.compendium, tot) for _, n, r, tot in recepta]


# ---------------------------------------------------------------- planta

def planta(via, vetus, novus, porta_nomen, filtrum=None, tolerans=True):
    """ritus culpae plantatae uno vocamine: ancora vetus -> novus, porta
    RUBRA esse debet (et cucurrisse, et aedificatio non fracta),
    reversio, porta VIRIDIS esse debet - reversio in 'finally' etiam
    per errorem. porta_nomen: nomen portae aut functio (via) -> Porta.
    Reddit (rubra, viridis) compendia."""
    e = Editio(via)
    e.replace(vetus, novus, tolerans=tolerans)
    ante = e.originalis
    plantata = e.textus
    # praevolatus: planta compilans quaeritur - clang -fsyntax-only super
    # copiam (plagula viva intacta, porta non cursa) ante ritum; planta
    # aedificationem frangens (parametrum inutile sub -Werror, B1.1)
    # cursum totum portae olim perdebat
    causa = syntaxis(via, plantata) if via.endswith(('.c', '.h')) else None
    if causa:
        raise SilvaError('planta non compilat (praevolatus, nihil cursum):'
                         ' %s' % causa)

    def currere():
        if callable(porta_nomen):
            return porta_nomen(via)
        return porta(porta_nomen, filtrum)

    with open(_absoluta(via), 'w') as f:
        f.write(plantata)
    try:
        rubra = currere()
        if re.search(r'\berror:', rubra.acta or ''):
            raise SilvaError('planta AEDIFICATIONEM fregit (error:) - '
                             'nihil probat; planta compilans quaeritur')
        if not rubra.cucurrit:
            raise SilvaError('porta cum planta non cucurrit (%s)'
                             % rubra.compendium)
        if rubra.sana:
            raise SilvaError('porta cum planta VIRIDIS - porta muta:'
                             ' %s' % rubra.compendium)
    finally:
        with open(_absoluta(via), 'w') as f:
            f.write(ante)
    viridis = currere()
    if not viridis.sana:
        raise SilvaError('porta post reversionem non viridis: %s'
                         % viridis.compendium)
    # testimonium rubrum servatur (acta rubra a cursu viridi obteruntur):
    # compendium + quae probationes ruberint et qua linea
    rubrum = rubra.compendium
    if rubra.fracturae:
        rubrum += ' | fractae: ' + ', '.join(
            '%s (%s)' % (f.nomen, _summa_fracturae(f)) for f in rubra.fracturae)
    print('planta rubra: ' + rubrum)
    return rubrum, viridis.compendium


# ---------------------------------------------------------------- probatio una

# suita -> (directorium fontium, porta, via binarii (% nomen)). Cursores
# omnes probationem e RADICE incipiunt; sub-suitae radicem per
# RHUBARB_RADIX praebent (defaltum in probationibus '..' = cursus manualis
# ex silva/ - e radice sine ambitu 'corpus non apertum' fallit, 2026-09-02);
# radix binaria in build/probationes/<nomen> aedificat (olim /tmp/<nomen>
# fixum - collisio inter arbores et clones umbrae, 2026-09-04).
SUITAE = {
    'radix': ('probationes', 'build/probationes/%s'),
    'silva': ('silva/probationes', 'silva/build/%s'),
    'css': ('css/probationes', 'css/build/%s'),
    'materia': ('materia/probationes', 'materia/build/%s'),
    'md': ('md/probationes', 'md/build/%s'),
    'oratio': ('oratio/probationes', 'oratio/build/%s'),
    'officina': ('officina/probationes', 'officina/build/%s'),
    'gesta': ('gesta/probationes', 'gesta/build/%s'),
    'tessera': ('tessera/probationes', 'tessera/build/%s'),
    'saltuarius': ('saltuarius/probationes', 'saltuarius/build/%s'),
}
Cursus = namedtuple('Cursus', 'nomen suita rc secunda acta fracturae profilum'
                    ' via_profili', defaults=(None,))


def probatio_suita(nomen):
    """suita cui probatio nominata pertinet (fons <dir>/<nomen>.c)"""
    for suita, (fontes, _) in SUITAE.items():
        if os.path.exists(os.path.join(RADIX, fontes, nomen + '.c')):
            return suita
    raise SilvaError('probatio ignota: %s (fons in nulla suita: %s)'
                     % (nomen, ', '.join(sorted(SUITAE))))


def _profilum(pid, secunda, via_effusus):
    """sample <pid> <secunda> -> [(numerus, functio, bibliotheca)] ex tabula
    'Sort by top of stack' (folia: ubi tempus consumitur), ordine ponderis;
    effusus crudus in via_effusus (arbor vocationum tota)"""
    r = _curre(['sample', str(pid), str(secunda), '-mayDie', '-file',
                via_effusus])
    if r.returncode != 0 or not os.path.exists(via_effusus):
        raise SilvaError('sample fractum (rc=%d): %s'
                         % (r.returncode, (r.stdout + r.stderr).strip()[-300:]))
    folia = []
    f = False
    for l in open(via_effusus, errors='replace'):
        if l.startswith('Sort by top of stack'):
            f = True
            continue
        if not f:
            continue
        m = re.match(r'\s+(\S+)\s+\(in ([^)]+)\)\s+(\d+)\s*$', l)
        if m:
            folia.append((int(m.group(3)), m.group(1), m.group(2)))
        elif folia and not l.strip():
            break
    return folia


def profilum_textus(profilum, tectum=15):
    """tabula foliorum: pars centesima, numerus, functio (bibliotheca)"""
    summa = sum(n for n, _, _ in profilum) or 1
    return '\n'.join('  %5.1f%%  %6d  %s  (%s)' % (100.0 * n / summa, n, fn, bib)
                     for n, fn, bib in profilum[:tectum])


def probatio_currere(nomen, aedificare=False, secunda=0, mora=2.0,
                     tectum=1800):
    """probationem UNAM currere sicut cursor eius: e radice, RHUBARB_RADIX
    praebita, binarium suitae. nomen = nomen probationis (suita ex fonte
    invenitur) aut via binarii exsecutabilis. aedificare: cursorem suitae
    cum filtro primum currere (aedificat ET currit semel - pretium
    acceptum; error aedificationis SilvaError cum relatione). secunda > 0:
    post moram (s) processum vivum per 'sample' secunda profilare ->
    Cursus.profilum = folia [(numerus, functio, bibliotheca)], effusus
    crudus build/sample/<nomen>.probatio.txt. Reddit Cursus(nomen, suita,
    rc, secunda cursus, acta, fracturae (generica, si rc != 0), profilum)."""
    if '/' in nomen and os.path.exists(nomen):
        suita, binarium = '?', os.path.abspath(nomen)
        titulus = os.path.basename(nomen)
    else:
        suita = probatio_suita(nomen)
        titulus = nomen
        if aedificare:
            p = porta(suita, nomen)
            if not p.cucurrit or re.search(r'\berror:|COMPILATION FAILED|'
                                           r'FRACTA \(compilatio\)', p.acta):
                raise SilvaError('aedificatio %s fracta (%s)%s' % (
                    nomen, p.compendium,
                    relatio_fracturarum(p.fracturae) or '\n' + p.acta[-800:]))
        binarium = os.path.join(RADIX, SUITAE[suita][1] % nomen)
        if not os.path.exists(binarium):
            raise SilvaError('binarium absens: %s (aedificare=True ut cursor'
                             ' suitae id aedificet)' % binarium)
    ambitus = dict(os.environ)
    ambitus['RHUBARB_RADIX'] = RADIX
    via_acta = tempfile.NamedTemporaryFile(prefix='cursus.', suffix='.log',
                                           delete=False)
    via_acta.close()
    t0 = time.time()
    with open(via_acta.name, 'wb') as effusus:
        proc = subprocess.Popen([binarium], cwd=RADIX, env=ambitus,
                                stdin=subprocess.DEVNULL, stdout=effusus,
                                stderr=subprocess.STDOUT)
        profilum = []
        via_p = None
        if secunda > 0:
            finis_morae = time.time() + mora
            while time.time() < finis_morae and proc.poll() is None:
                time.sleep(0.05)
            if proc.poll() is None:
                os.makedirs(os.path.join(RADIX, 'build', 'sample'),
                            exist_ok=True)
                via_p = os.path.join(RADIX, 'build', 'sample',
                                     titulus + '.probatio.txt')
                profilum = _profilum(proc.pid, secunda, via_p)
        try:
            rc = proc.wait(timeout=tectum)
        except subprocess.TimeoutExpired:
            proc.kill()
            rc = proc.wait()
            raise SilvaError('probatio %s tectum %ds excessit (occisa)'
                             % (titulus, tectum))
    t1 = time.time()
    acta = _ANSI.sub('', open(via_acta.name, errors='replace').read())
    os.unlink(via_acta.name)
    fr = [] if rc == 0 else fracturae(acta, titulus, forma='generica')
    return Cursus(titulus, suita, rc, t1 - t0, acta, fr, profilum, via_p)


def _profilum_arbor(via):
    """arbor vocationum ex effusu 'sample' (sectio 'Call graph'):
    [(altitudo, numerus, functio)] ordine plagulae; numerus = exempla
    INCLUSIVA subarboris illius"""
    frusta = []
    f = False
    for l in open(via, errors='replace'):
        if l.startswith('Call graph'):
            f = True
            continue
        if l.startswith('Total number in stack'):
            break
        if not f:
            continue
        m = re.match(r'^([\s+!:|]*)(\d+) (\S+)', l)
        if m:
            frusta.append((len(m.group(1)), int(m.group(2)), m.group(3)))
    return frusta


def profilum_inclusivum(via, tectum=30):
    """tempus INCLUSIVUM per functionem ex effusu sample - recursione
    COLLAPSA: occurrentia functionis sub se ipsa non numeratur (summa
    ingenua per gradus recursionis ambulatores recursivos septies
    inflabat, 2026-09-02). [(pars, numerus, functio)] ordine ponderis;
    fila/start/main omissa."""
    frusta = _profilum_arbor(via)
    summae = {}
    acervus = []
    totum = frusta[0][1] if frusta else 1
    for altitudo, n, fn in frusta:
        while acervus and acervus[-1][0] >= altitudo:
            acervus.pop()
        if fn not in [s[1] for s in acervus]:
            summae[fn] = summae.get(fn, 0) + n
        acervus.append((altitudo, fn))
    ordo = sorted(summae.items(), key=lambda kv: -kv[1])
    exitus = []
    for fn, n in ordo:
        if fn.startswith('Thread') or fn in ('start', 'main'):
            continue
        exitus.append((100.0 * n / totum, n, fn))
        if len(exitus) >= tectum:
            break
    return exitus


def profilum_viae(via, functio, minimum=0, tectum=12):
    """semitae vocationum quae in 'functio' desinunt, a main deorsum:
    [(numerus, 'a > b×3 > functio')] ordine ponderis - cursus eiusdem
    functionis (recursio) in unum 'fn×k' comprimitur, occurrentia
    functionis sub se ipsa omissa (numeratur semel, in summo)."""
    frusta = _profilum_arbor(via)
    acervus = []
    summae = {}
    for altitudo, n, fn in frusta:
        while acervus and acervus[-1][0] >= altitudo:
            acervus.pop()
        catena = [s[1] for s in acervus]
        if fn == functio and functio not in catena:
            if 'main' in catena:
                catena = catena[catena.index('main') + 1:]
            catena = catena + [fn]
            compressa = []
            for c in catena:
                if compressa and compressa[-1][0] == c:
                    compressa[-1][1] += 1
                else:
                    compressa.append([c, 1])
            clavis = ' > '.join(c if k == 1 else '%s×%d' % (c, k)
                                for c, k in compressa)
            summae[clavis] = summae.get(clavis, 0) + n
        acervus.append((altitudo, fn))
    ordo = sorted(summae.items(), key=lambda kv: -kv[1])
    return [(n, s) for s, n in ordo if n >= minimum][:tectum]


def profilum_inclusivum_textus(inclusivum, tectum=15):
    return '\n'.join('  %5.1f%%  %6d  %s' % (p, n, fn)
                     for p, n, fn in inclusivum[:tectum])


def cursus_textus(c, tectum=15):
    """compendium cursus: verdictum, tempus, compendium credo, fracturae,
    profilum (si sumptum)"""
    m = re.search(r'Totalis:\s*(\d+).*?Fracti:\s*(\d+).*?Conditio: ([^\n]*)',
                  c.acta, re.S)
    credo_ = ('assertiones %s, fractae %s, %s' % m.groups()) if m \
        else '(compendium credo absens)'
    lineae = ['%s (%s): exitus %d, %.2f s - %s'
              % (c.nomen, c.suita, c.rc, c.secunda, credo_)]
    if c.fracturae:
        lineae.append(relatio_fracturarum(c.fracturae).lstrip('\n'))
    if c.profilum:
        lineae.append('profilum (folia, %d exempla):'
                      % sum(n for n, _, _ in c.profilum))
        lineae.append(profilum_textus(c.profilum, tectum))
    if c.via_profili and os.path.exists(c.via_profili):
        lineae.append('profilum (inclusivum, recursione collapsa):')
        lineae.append(profilum_inclusivum_textus(
            profilum_inclusivum(c.via_profili), tectum))
        lineae.append('  (semitae: profilum_viae(%r, functio))' % c.via_profili)
    return '\n'.join(lineae)


# ---------------------------------------------------------------- imagines

# Oraculum identitatis octetorum: effusus imperii per plagulam ANTE
# mutationem servatus, POST conlatus - methodus probationis domus
# (aequivalentia parsurae, clausurae aedilis, lineae census) quae ter
# uno die manu gyro crustae scribebatur (2026-09-02).
IMAGINES_DIR = os.path.join(RADIX, 'build', 'imagines')
Imago = namedtuple('Imago', 'nomen via imperium plagulae numerus')


class Collatio(object):
    """fructus imago_conferre: eaedem / diversae / absentes (in imagine,
    non in lista) / novae (in lista, sine imagine); .sana = eaedem
    solae"""

    def __init__(self, nomen, eaedem, diversae, absentes, novae):
        self.nomen = nomen
        self.eaedem = eaedem
        self.diversae = diversae
        self.absentes = absentes
        self.novae = novae

    @property
    def sana(self):
        return not (self.diversae or self.absentes or self.novae)

    def __str__(self):
        s = 'imago %s: %d eaedem, %d diversae, %d absentes, %d novae' % (
            self.nomen, len(self.eaedem), len(self.diversae),
            len(self.absentes), len(self.novae))
        for titulus, lista in (('diversae', self.diversae),
                               ('absentes', self.absentes),
                               ('novae', self.novae)):
            for v in lista[:20]:
                s += '\n  %s: %s' % (titulus[:-1], v)
            if len(lista) > 20:
                s += '\n  ... (%d plures)' % (len(lista) - 20)
        return s


def _imago_nomen_plagulae(via):
    return via.replace('/', '__')


def _imago_currere(imperium, via):
    args = ([via if a == '{}' else a for a in imperium]
            if '{}' in imperium else list(imperium) + [via])
    r = subprocess.run(args, cwd=RADIX, stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)
    return r.returncode, r.stdout


def imago_capere(nomen, imperium, plagulae):
    """imaginem effusus imperii per plagulam servare: build/imagines/
    <nomen>/<via>.out (octeti stdout) + .rc, manifestum.json (imperium,
    plagulae, HEAD, momentum). imperium: lista argumentorum, '{}' = via
    plagulae (absens: via appenditur). Imago prior eiusdem nominis
    deletur."""
    import json as _json
    import shutil
    _nomen_planum(nomen, 'nomen imaginis')
    d = os.path.join(IMAGINES_DIR, nomen)
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    for via in plagulae:
        rc, octeti = _imago_currere(imperium, via)
        basis = os.path.join(d, _imago_nomen_plagulae(via))
        with open(basis + '.out', 'wb') as f:
            f.write(octeti)
        with open(basis + '.rc', 'w') as f:
            f.write(str(rc))
    with open(os.path.join(d, 'manifestum.json'), 'w') as f:
        _json.dump({'imperium': list(imperium), 'plagulae': list(plagulae),
                    'commissio': _curre(['git', 'rev-parse', '--short',
                                         'HEAD']).stdout.strip(),
                    'momentum': time.time()}, f)
    return Imago(nomen, d, list(imperium), list(plagulae), len(plagulae))


def imago_manifestum(nomen):
    import json as _json
    _nomen_planum(nomen, 'nomen imaginis')
    via = os.path.join(IMAGINES_DIR, nomen, 'manifestum.json')
    if not os.path.exists(via):
        raise SilvaError('imago absens: %s (imago_capere primum)' % nomen)
    with open(via) as f:
        return _json.load(f)


def imago_conferre(nomen, imperium=None, plagulae=None):
    """imaginem cum effusu PRAESENTI conferre (idem imperium, eaedem
    plagulae nisi datae): Collatio; .sana = octeti et rc idem ubique.
    Effusus novus in <nomen>.post/ servatur (imago_differentia)."""
    m = imago_manifestum(nomen)
    imperium = list(imperium) if imperium is not None else m['imperium']
    plagulae = list(plagulae) if plagulae is not None else m['plagulae']
    d = os.path.join(IMAGINES_DIR, nomen)
    d_post = d + '.post'
    import shutil
    shutil.rmtree(d_post, ignore_errors=True)
    os.makedirs(d_post)
    eaedem, diversae, novae = [], [], []
    for via in plagulae:
        rc, octeti = _imago_currere(imperium, via)
        basis = os.path.join(d, _imago_nomen_plagulae(via))
        basis_post = os.path.join(d_post, _imago_nomen_plagulae(via))
        with open(basis_post + '.out', 'wb') as f:
            f.write(octeti)
        with open(basis_post + '.rc', 'w') as f:
            f.write(str(rc))
        if not os.path.exists(basis + '.out'):
            novae.append(via)
            continue
        with open(basis + '.out', 'rb') as f:
            antea = f.read()
        rc_antea = open(basis + '.rc').read().strip()
        if antea == octeti and rc_antea == str(rc):
            eaedem.append(via)
        else:
            diversae.append(via)
    absentes = [v for v in m['plagulae'] if v not in plagulae]
    return Collatio(nomen, eaedem, diversae, absentes, novae)


def imago_differentia(nomen, via, contextus=3):
    """differentia unificata effusus imaginis contra effusum ultimae
    conlationis (<nomen>.post) pro plagula una"""
    _nomen_planum(nomen, 'nomen imaginis')
    d = os.path.join(IMAGINES_DIR, nomen)
    a = os.path.join(d, _imago_nomen_plagulae(via) + '.out')
    b = os.path.join(d + '.post', _imago_nomen_plagulae(via) + '.out')
    for v in (a, b):
        if not os.path.exists(v):
            raise SilvaError('effusus absens: %s' % v)
    ta = open(a, errors='replace').read().splitlines(True)
    tb = open(b, errors='replace').read().splitlines(True)
    return ''.join(difflib.unified_diff(ta, tb, 'imago/' + via,
                                        'nunc/' + via, n=contextus))


# ---------------------------------------------------------------- mensurae

VOLUMEN_MENSURARUM = os.path.expanduser('~/.rhubarb/mensurae.volumen')
Sessio = namedtuple('Sessio', 'clavis momentum commissio mensurae')


def mensurae(praefixum='', quantum=1, plenae=True, via=None):
    """Sessiones suitae ex volumine mensoris, recentissimae primum:
    praefixum '' = radix, 'silva.' = silva (tools/mensor_suitae.sh).
    Tituli praefixo EXUTI (suita.tempus.totum, probatio.cursus.X ...)
    ut claves suitarum omnium eaedem sint. plenae: solae sessiones cum
    suita.probationes.totae >= dimidium maximi (cursus filtrati
    exclusi, ut facies). [] si volumen abest."""
    import sqlite3
    v = via or VOLUMEN_MENSURARUM
    if not os.path.exists(v):
        return []
    db = sqlite3.connect(v)
    rows = db.execute("select momentum, genus, datum from acta"
                      " where genus in ('sessio','mensura') order by seq"
                      ).fetchall()
    db.close()
    meta = {}
    mens = {}
    ordo = []
    for mom, g, d in rows:
        p = d.split('\t')
        if g == 'sessio':
            meta[p[0]] = (mom, p[2][:8] if len(p) > 2 else '')
            continue
        if len(p) < 6 or not p[3].startswith(praefixum):
            continue
        cauda = p[3][len(praefixum):]
        if cauda.split('.')[0] not in ('suita', 'probatio'):
            continue
        if p[0] not in mens:
            mens[p[0]] = {}
            ordo.append(p[0])
        try:
            mens[p[0]][cauda] = float(p[4])
        except ValueError:
            pass
    sessiones = [s for s in ordo if 'suita.tempus.totum' in mens[s]]
    if plenae and sessiones:
        maximum = max(mens[s].get('suita.probationes.totae', 0)
                      for s in sessiones)
        sessiones = [s for s in sessiones
                     if mens[s].get('suita.probationes.totae', 0) * 2
                     >= maximum]
    exitus = []
    for s in reversed(sessiones[-quantum:]):
        mom, comm = meta.get(s, ('', ''))
        exitus.append(Sessio(s, mom, comm, mens[s]))
    return exitus


def compendium_mensurae(sessio, quantum=5):
    """textus: totum/cursus/compilatio/praevolatus + probationes
    tardissimae"""
    m = sessio.mensurae
    lineae = ['sessio %s %s %s: totum %.1fs cursus %.1fs compilatio %.1fs'
              ' praevolatus %.1fs probationes %d' % (
                  sessio.clavis, sessio.momentum[:16], sessio.commissio,
                  m.get('suita.tempus.totum', 0),
                  m.get('suita.tempus.cursus', 0),
                  m.get('suita.tempus.compilatio', 0),
                  m.get('suita.tempus.praevolatus', 0),
                  int(m.get('suita.probationes.totae', 0)))]
    tarda = sorted(((v, k) for k, v in m.items()
                    if k.startswith('probatio.cursus.')), reverse=True)
    for v, k in tarda[:quantum]:
        lineae.append('  %8.2fs  %s' % (v, k[len('probatio.cursus.'):]))
    return '\n'.join(lineae)


Mensura = namedtuple('Mensura', 'via parsare_ms lexare_ms phases allocationes usus ordo'
                    ' campi', defaults=(None,))


def metiri(via, n=7, nudum=False):
    """computus min-of-n (singuli +-X%): parsare/lexare ms, phases,
    allocationes, usus - pro A/B optimizationum. Instrumentum ex
    suffixo: .css -> css/computus.sh, .md -> md/computus.sh, .txt ->
    oratio/computus.sh (semita
    materiae; phases emittendi/arbor_scribendi/arbor_legendi/comparandi),
    aliter silva/computus.sh (lex/expansio/glr/commissio). Columnae per
    TITULOS capitis '#' lectae (campi = dict cursus optimi), ordo =
    ordo TSV crudus."""
    cliens = ('css' if via.endswith('.css')
              else 'md' if via.endswith('.md')
              else 'oratio' if via.endswith('.txt') else None)
    best = None
    for _ in range(n):
        if cliens:
            args = ['./%s/computus.sh' % cliens, _absoluta(via), '-machina']
        else:
            args = ['./silva/computus.sh', _absoluta(via), '-machina'] \
                + (['-nudum'] if nudum else [])
        out = _curre(args).stdout.splitlines()
        caput = [l[1:].strip().split('\t') for l in out if l.startswith('#')]
        rows = [l.split('\t') for l in out if l and not l.startswith('#')]
        if not rows or not caput:
            raise SilvaError('computus sine ordine: %s' % via)
        campi = dict(zip(caput[0], rows[0]))
        v = float(campi['ms_parsandi'])
        if best is None or v < best[0]:
            best = (v, rows[0], campi)
    v, r, campi = best
    if cliens:
        phases = dict((k[3:], float(campi[k])) for k in
                      ('ms_emittendi', 'ms_arbor_scribendi',
                       'ms_arbor_legendi', 'ms_comparandi') if k in campi)
        lexare = 0.0
    else:
        phases = dict((k[3:], float(campi[k])) for k in
                      ('ph_lexandi', 'ph_expandendi', 'ph_glr',
                       'ph_committendi') if k in campi)
        phases = {'lex': phases.get('lexandi', 0.0),
                  'expansio': phases.get('expandendi', 0.0),
                  'glr': phases.get('glr', 0.0),
                  'commissio': phases.get('committendi', 0.0)}
        lexare = float(campi.get('ms_lexandi', 0))
    return Mensura(via, v, lexare, phases, int(campi['allocationes']),
                   int(campi['usus']), r, campi)


# ---------------------------------------------------------------- selecta

Selectum = namedtuple('Selectum', 'linea_a linea_b textus')


def selecta(via, selector, intra=None):
    """sedes structurales per selectorem (selecta.sh): [Selectum] cum
    lineis et textu byte-exacto - ancorae sine textu litterali"""
    args = ['./silva/selecta.sh', _absoluta(via), selector, '-omnia']
    if intra:
        args += ['-intra', intra]
    r = _curre(args)
    if r.returncode == 2:
        raise SilvaError('selecta fractus: %s' % r.stderr.strip()[-200:])
    exitus = []
    caput = re.compile(r'^\[(.+):(\d+)-(\d+)\]$')
    cur = None
    corpus_lineae = []
    for linea in r.stdout.splitlines():
        m = caput.match(linea)
        if m:
            if cur is not None:
                exitus.append(Selectum(cur[0], cur[1],
                                       '\n'.join(corpus_lineae).rstrip('\n')
                                       + '\n'))
            cur = (int(m.group(2)), int(m.group(3)))
            corpus_lineae = []
        elif cur is not None:
            corpus_lineae.append(linea)
    if cur is not None:
        exitus.append(Selectum(cur[0], cur[1],
                               '\n'.join(corpus_lineae).rstrip('\n') + '\n'))
    return exitus


def origo (via, linea, columna=None):
    """historia expansionis ad sedem (origo.sh) - textus"""
    sedes = '%d:%d' % (linea, columna) if columna else str(linea)
    return _curre(['./silva/origo.sh', _absoluta(via), sedes,
                   '-omnia']).stdout


def arbor(via, nudum=False):
    """documentum STML canonicum plagulae (arbor.sh) - textus"""
    args = ['./silva/arbor.sh', _absoluta(via), '-tacitus']
    if nudum:
        args.append('-nudum')
    r = _curre(args)
    if r.returncode != 0 and not r.stdout:
        raise SilvaError('arbor fractus: %s' % r.stderr.strip()[-200:])
    return r.stdout


def differre_git(via, ref_vetus='HEAD', ref_novum=None,
                 gradus='cosmetica'):
    """differentia unitatum contra historiam git (arbor operis
    ordinaria) cum verdicto"""
    args = ['./silva/differre.sh', '-git', via, ref_vetus]
    if ref_novum:
        args.append(ref_novum)
    args += ['-machina', '-verdictum', gradus]
    r = _curre(args)
    paria = []
    verd = None
    for o in _tsv(r.stdout):
        if o and o[0] == 'VERDICTUM' and len(o) > 1:
            verd = o[1]
        elif len(o) >= 6:
            paria.append(Par(o[0], o[1], o[2], o[3], o[4], o[5]))
    if verd is None:
        raise SilvaError('differre -git sine verdicto (rc=%d): %s'
                         % (r.returncode, r.stderr.strip()[-200:]))
    return Differentia(paria, verd, r.returncode == 0, r.returncode)


# ---------------------------------------------------------------- legati

import atexit
import json


class Legati(object):
    """Pons ad legatum (servum MCP silvae) per stdio JSON-RPC (lineae):
    idem apparatus quem Claude per MCP videt - vocantes/vocata/
    inclusiones/caput/symbolum/corpus/diagnostica/quaestio - scriptis
    patens. Processus proprius (non residens sessionis), pigre natus,
    in exitu clausus. Effusum = textus fragmenti primi."""

    def __init__(self):
        self.proc = None
        self.id = 0

    def _nasci(self):
        if self.proc is not None:
            return
        self.proc = subprocess.Popen(
            ['./officina/legatus.sh', '-mcp'], cwd=RADIX,
            stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL, text=True, bufsize=1)
        atexit.register(self.claudere)
        self._petere('initialize', {
            'protocolVersion': '2024-11-05', 'capabilities': {},
            'clientInfo': {'name': 'pythonica', 'version': '0'}})
        self._mittere({'jsonrpc': '2.0',
                       'method': 'notifications/initialized'})

    def _mittere(self, nuntius):
        self.proc.stdin.write(json.dumps(nuntius) + '\n')
        self.proc.stdin.flush()

    def _petere(self, methodus, params):
        self.id += 1
        self._mittere({'jsonrpc': '2.0', 'id': self.id,
                       'method': methodus, 'params': params})
        while True:
            linea = self.proc.stdout.readline()
            if not linea:
                raise SilvaError('legatus tacuit (processus mortuus?)')
            try:
                n = json.loads(linea)
            except ValueError:
                continue
            if n.get('id') == self.id:
                if 'error' in n:
                    raise SilvaError('legatus: %s' % n['error'])
                return n.get('result')

    def voca(self, instrumentum, **argumenta):
        self._nasci()
        r = self._petere('tools/call', {'name': instrumentum,
                                        'arguments': argumenta})
        partes = (r or {}).get('content') or []
        return '\n'.join(p.get('text', '') for p in partes)

    def claudere(self):
        if self.proc is not None:
            try:
                self.proc.stdin.close()
                self.proc.wait(timeout=5)
            except Exception:
                self.proc.kill()
            self.proc = None


_legati = Legati()


def legati(instrumentum, **argumenta):
    """vocatio cruda instrumenti legati (textus)"""
    return _legati.voca(instrumentum, **argumenta)


def vocantes(titulus):
    return legati('vocantes', titulus=titulus)


def vocata(titulus):
    return legati('vocata', titulus=titulus)


def inclusiones(via):
    return legati('inclusiones', via=via)


def caput(via):
    return legati('caput', via=via)


def symbolum(titulus, via=None):
    return legati('symbolum', titulus=titulus, **({'via': via} if via
                                                     else {}))


def diagnostica(via):
    return legati('diagnostica', via=via)


# ---------------------------------------------------------------- refactio

def _caput_definitionis(textus_nodi):
    """caput definitionis (specificatores + declarator) sine corpore:
    lineae ante primam '{' solam in columna I"""
    lineae = textus_nodi.splitlines(True)
    for i, l in enumerate(lineae):
        if l.rstrip('\n') == '{':
            return ''.join(lineae[:i]).rstrip()
    raise SilvaError('caput definitionis: brachium apertum in columna I'
                     ' non inventum (forma domus?)')


class Refactio(object):
    """Transactio trans plagulas: Editio una per viam (editio(via)),
    applicare() unum - custos lectionis rancidae in OMNIBUS ante
    scripturam ullam, tum scriptura omnium, forma omnium, iudicium
    omnium (examen post omnes scriptas: plagula A functionem in B
    additam vocans B scriptam requirit). strictum: examen ullum non
    ACCIPE -> omnes restitutae. Reddit [Fructus]."""

    def __init__(self):
        self.editiones = {}
        self.ordo = []

    def editio(self, via):
        if via not in self.editiones:
            self.editiones[via] = Editio(via)
            self.ordo.append(via)
        return self.editiones[via]

    def prototypum_synchronizare(self, via_c, via_h, nomen,
                                 si_absens='error'):
        """prototypum functionis in via_h ex capite definitionis in via_c
        (textus PRAESENS transactionis) regenerare. via_h == via_c =
        acervus prototyporum localium. si_absens: 'error' | 'finis'
        (ante '#endif' ultimum inseritur, capitibus solis)."""
        ec = self.editio(via_c)
        x = _extentum_nominis(ec._extenta_praesentia(), nomen, True)
        lineae = ec._lineae()
        caput = _caput_definitionis(''.join(lineae[x.linea_nodi - 1:
                                                   x.linea_b]))
        prototypum = caput + ';\n'
        eh = self.editio(via_h)
        try:
            px = _extentum_nominis(eh._extenta_praesentia(), nomen, False)
        except SilvaError:
            if si_absens != 'finis':
                raise
            k = eh.textus.rfind('#endif')
            if k < 0:
                raise SilvaError('prototypum absens et #endif absens: %s'
                                 % via_h)
            eh.textus = eh.textus[:k] + prototypum + '\n' + eh.textus[k:]
            eh.acta.append('prototypum %s insertum' % nomen)
            return self
        lh = eh._lineae()
        eh.textus = ''.join(lh[:px.linea_nodi - 1]) + prototypum \
            + ''.join(lh[px.linea_b:])
        eh.acta.append('prototypum %s synchronizatum' % nomen)
        return self

    def applicare(self, forma=True, iudica=True, strictum=False):
        for via in self.ordo:
            self.editiones[via]._custodire()
        mutatae = [v for v in self.ordo if self.editiones[v].mutata()]
        antea = {}
        for via in mutatae:
            antea[via] = self.editiones[via]._scribere()
        formatae = {}
        for via in mutatae:
            formatae[via] = self.editiones[via]._formare() if forma \
                else False
        fructus = []
        for via in self.ordo:
            e = self.editiones[via]
            if via not in antea:
                fructus.append(Fructus(via, '', None, False, None))
                continue
            fructus.append(e._fructus(antea[via], formatae[via], iudica))
        if strictum and any(not f.sana for f in fructus):
            for via in mutatae:
                self.editiones[via]._restituere(antea[via])
            raise SilvaError('examen non ACCIPE in %s - omnes restitutae'
                             ' (strictum)' % [f.via for f in fructus
                                              if not f.sana])
        for via in mutatae:
            self.editiones[via].originalis = self.editiones[via].textus
        return FructusOmnes(fructus)


def prototypum_synchronizare(via_c, via_h, nomen, si_absens='error',
                             **optiones):
    """forma brevis: transactio unius operationis"""
    r = Refactio()
    r.prototypum_synchronizare(via_c, via_h, nomen, si_absens)
    return r.applicare(**optiones)


# ---------------------------------------------------------------- iudicia

def examen(via):
    """verdictum examinis C89: 'ACCIPE' | 'REICE' | 'RECUSO'"""
    r = _curre(['./silva/examen.sh', _absoluta(via), '-machina'])
    for o in _tsv(r.stdout):
        if o and o[0] == 'VERDICTUM' and len(o) > 1:
            return o[1]
    raise SilvaError('examen sine verdicto (rc=%d): %s'
                     % (r.returncode, r.stderr.strip()[-200:]))


def lint(via):
    """divergentiae formae (regulae omnes) - lista Divergentia"""
    r = _curre(['./silva/formator.sh', _absoluta(via), '-machina']
               + _capita(via))
    if r.returncode == 2:
        raise SilvaError('formator fractus: %s' % r.stderr.strip()[-200:])
    exitus = []
    for o in _tsv(r.stdout):
        if len(o) >= 8:
            exitus.append(Divergentia(int(o[1]), int(o[2]), o[3],
                                      int(o[4]), int(o[5]), o[6], int(o[7])))
    return exitus


def forma_delta(via, ref='HEAD'):
    """divergentiae formae NOVAE contra ref (lex basis): (novae, evanidae)"""
    r = _curre(['./silva/formator.sh', _absoluta(via), '-delta', ref,
                '-machina'])
    for o in _tsv(r.stdout):
        if o and o[0] == 'DELTA' and len(o) >= 4:
            return int(o[2].lstrip('+')), int(o[3].lstrip('-'))
    raise SilvaError('formator -delta sine ordine DELTA (rc=%d)'
                     % r.returncode)


def formare(via, nomina=None):
    """-scribere (tota, aut -intra nomina); VERUM si scripta"""
    args = ['./silva/formator.sh', _absoluta(via), '-scribere'] + _capita(via)
    for n in nomina or []:
        args += ['-intra', n]
    r = _curre(args)
    if r.returncode == 2:
        raise SilvaError('formator recusavit: %s' % r.stderr.strip()[-300:])
    return 'formator: scriptum' in r.stderr


def verdictum(via, ref='HEAD'):
    """examen + forma uno vocamine: dict(examen, forma_novae, forma_evanidae)"""
    novae, evanidae = forma_delta(via, ref)
    return {'examen': examen(via), 'forma_novae': novae,
            'forma_evanidae': evanidae}


# ---------------------------------------------------------------- symbola

Sedes = namedtuple('Sedes', 'via linea columna genus')
Usus = namedtuple('Usus', 'sedes usus')   # sedes: [Sedes]; usus: {via: [lineae]}


def usus(symbolum):
    """sedes definitionum + usus per plagulam (nexus.sh, tabula se-sanans);
    symbolum ignotum = Usus([], {})"""
    r = _curre(['./silva/nexus.sh', symbolum])
    if r.returncode == 2:
        raise SilvaError('nexus fractus: %s' % r.stderr.strip()[-200:])
    sedes = []
    per_viam = {}
    for linea in r.stdout.splitlines():
        if linea.startswith('SEDES '):
            partes = linea.split()
            via, l, c = partes[1].rsplit(':', 2)
            sedes.append(Sedes(via, int(l), int(c),
                               partes[2] if len(partes) > 2 else ''))
        elif linea.startswith('  ') and '(' in linea and '):' in linea:
            via = linea.split()[0]
            lineae = [int(x) for x in linea.split('):', 1)[1].split()]
            per_viam[via] = lineae
    return Usus(sedes, per_viam)


def renominare(vetus, novum, viae=None, scribere=False, intra_via=None):
    """renominatio semantica (renominare.sh): planum ordinarium, scribere
    applicat. Reddit textum plani/relationis; refusio (1) et recusatio
    (3) levant SilvaError cum causa; apparatus (2) et fractum (4)
    quoque."""
    args = ['./silva/renominare.sh', vetus, novum] + list(viae or [])
    if intra_via:
        args += ['-via', intra_via]
    if scribere:
        args.append('-scribere')
    r = _curre(args)
    if r.returncode != 0:
        raise SilvaError('renominare rc=%d: %s' % (
            r.returncode, (r.stdout + r.stderr).strip()[-400:]))
    return r.stdout


def _ut_plagula(x, praefixum):
    """x = via aut textus (cum '\\n'): via temporaria pro textu"""
    if '\n' not in x and os.path.exists(_absoluta(x)):
        return _absoluta(x), None
    fd, via_t = tempfile.mkstemp(prefix=praefixum, suffix='.c',
                                 dir=os.path.join(RADIX, 'build'))
    with os.fdopen(fd, 'w') as f:
        f.write(x)
    return via_t, via_t


def differre(vetus, novus, gradus='cosmetica'):
    """differentia unitatum (viae aut textus). verdictum: 'cosmetica' |
    'documentaria' | 'substantiva'; cosmetica_solum = verdictum intra
    gradum datum (gradus 'documentaria' commentaria quoque tolerat)."""
    va, ta = _ut_plagula(vetus, '.differre_a_')
    vb, tb = _ut_plagula(novus, '.differre_b_')
    try:
        r = _curre(['./silva/differre.sh', va, vb, '-machina',
                    '-verdictum', gradus])
    finally:
        for t in (ta, tb):
            if t:
                os.unlink(t)
    paria = []
    verd = None
    for o in _tsv(r.stdout):
        if o and o[0] == 'VERDICTUM' and len(o) > 1:
            verd = o[1]
        elif len(o) >= 6:
            paria.append(Par(o[0], o[1], o[2], o[3], o[4], o[5]))
    if verd is None:
        raise SilvaError('differre sine verdicto (rc=%d): %s'
                         % (r.returncode, r.stderr.strip()[-200:]))
    return Differentia(paria, verd, r.returncode == 0, r.returncode)


if __name__ == '__main__':
    if len(sys.argv) >= 5 and sys.argv[1] == '-umbra':
        if len(sys.argv) >= 8:
            _umbra_currere(sys.argv[2], sys.argv[3], sys.argv[4],
                           sys.argv[5], sys.argv[6], sys.argv[7])
        else:
            _umbra_currere(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        sys.stderr.write('usus: silva.py -umbra <porta> <filtrum|""> '
                         '<receptum.json> [<clone> <arbor> <basis>]\n')
        sys.exit(2)


# ---------------------------------------------------------------- vocabula (oratio)
Verbum = namedtuple('Verbum', 'verbum status sedes symbola commenta prosa classis '
                    'lemma analyses lemmata via linea regula')
Vocabula = namedtuple('Vocabula', 'numeri verba ignota ambigua permissa')


def vocabula(fons='omnia', omnes_viae=False):
    """RECENSIO VOCABULORUM (./oratio/vocabula.sh -machina; T10):
    identificatores (build/nexus.tsv, sedes; scissi ad '_' et ad limitem
    minuscula->CAPITALIS) et/aut commentaria (lib/*.c, silva/fontes/*.c
    per lexatorem silvae et arborem orationis), verbum quodque SEMEL
    quaesitum - glossarium oratio/glossarium.stml primum, tabula WORDS
    deinde: status 'notum' | 'ambiguum' (lemmata plura) | 'permissum'
    (glossarium classis ignotum-permissum: offset, index ...) | 'ignotum'
    (INVENTUM, non vitium: Anglica gradus IV, nomina propria,
    abbreviationes, vocabula domus nondum in glossario). fons = 'symbola'
    | 'commenta' | 'omnia'. Reddit Vocabula(numeri {verba sedes notum
    ambiguum permissum ignotum}, verba [Verbum ordine sedium
    descendentium: verbum status sedes symbola commenta classis lemma
    analyses lemmata via linea (sedes prima)], ignota, ambigua,
    permissa). Relatio glossarium alit (termini technici) et T8b metitur.
    Viae knotapel/ vendor/ archivum/ EXCLUSAE (lint Latinus domus, decisio
    Frani 2026-09-04) nisi omnes_viae=True. fons='prosa' (T15a) = CONTEXTUS
    ANGLICUS: nodi TEXTUS plagularum markdown tractarum (saepta, verbatim,
    nexus, html numquam), glossarium (entria anglice licita) primum, Moby
    deinde, tabula Latina tertia -> status 'latinum' (Latine notum, NON
    inventum); 'ambiguum' numquam; viae vendor/ archivum/ et plagulae
    generatae (gesta/annales/tabula.md, md/CENSUS.md) exclusae, knotapel/
    INCLUSUM. Verbum.prosa = sedes in prosa; Verbum.regula = regula
    morphologica (T15b: pluralis-s, praeteritum-ed, participium-ing,
    possessivum, adverbium-ly, comparativus-er, compositum ...) qua forma
    non listata ad basin (lemma) redacta est, vacua = forma exacta."""
    if fons not in ('symbola', 'commenta', 'omnia', 'prosa'):
        raise SilvaError("fons: 'symbola' | 'commenta' | 'omnia' | 'prosa'")
    r = _curre(['./oratio/vocabula.sh', '-' + fons, '-machina']
               + (['-omnes-viae'] if omnes_viae else []))
    if r.returncode != 0:
        raise SilvaError('vocabula.sh fractus: %s' % r.stderr.strip()[-200:])
    verba = []
    for linea in r.stdout.splitlines():
        if not linea or linea.startswith('#'):
            continue
        p = linea.split('\t')
        if len(p) < 13:
            continue
        verba.append(Verbum(p[0], p[1], int(p[2]), int(p[3]), int(p[4]), int(p[5]),
                            p[6], p[7], int(p[8]), int(p[9]), p[10], int(p[11]), p[12]))
    numeri = {'verba': len(verba), 'sedes': sum(v.sedes for v in verba)}
    for st in ('notum', 'ambiguum', 'permissum', 'ignotum', 'latinum'):
        numeri[st] = sum(1 for v in verba if v.status == st)
    return Vocabula(numeri, verba,
                    [v for v in verba if v.status == 'ignotum'],
                    [v for v in verba if v.status == 'ambiguum'],
                    [v for v in verba if v.status == 'permissum'])


# ---------------------------------------------------------------- oratio (T14)
OratioSententia = namedtuple('OratioSententia',
                             'index initium finis linea forma textus')
OratioVocabulum = namedtuple('OratioVocabulum',
                             'index initium finis linea paragraphus sententia'
                             ' forma classes linguae lemma analyses')
OratioAnalysis = namedtuple('OratioAnalysis',
                            'index classis lemma lingua fons nativum sensus'
                            ' accidentia')
ORATIO_ACCIDENTIA = ('casus', 'numerus', 'genus', 'persona', 'tempus', 'modus',
                     'vox', 'forma-verbi', 'gradus', 'species', 'declinatio',
                     'coniugatio')


class Oratio(object):
    """QUAESTIONES super orationem (textum planum; T14): sententiae
    (./oratio/sententiae.sh), vocabula annotata et analyses
    (./oratio/verba.sh - arbor orationis + tabula Latina WORDS + glossarium
    oratio/glossarium.stml, gradus III). Argumentum: via plagulae
    exsistentis aut TEXTUS ipse (str aut bytes; in build/pythonica per
    vocamen scriptus et deletus). LECTIO SOLA - nihil hic editur (Prosa
    editiones markdown fert et Prosa.sententia huc delegat). Extenta
    OCTETIM [initium, finis) in fonte, caudae exclusae; index =
    ordinalis vocabuli in documento (clavis analysium); sententia =
    ordinalis sententiae ut in sententiae(); classes/linguae = tuplae
    ordine analysium (loci compendiarii vocabuli), ('ignotum',) sine
    analysi (INVENTUM, non vitium); lemma = analysis primae (ordo FONTIS,
    non gradus V: 'virum' -> virus, 'rosam' -> rodo). Analysis: classis
    (substantivum verbum ... ignotum), lemma, lingua (latina anglica),
    fons (vocabularium-la glossarium regula ...), nativum (codex fontis
    verbatim 'N 1 1 NOM S C' | 'capitalis'), sensus, accidentia {titulus:
    optio} - casus numerus genus persona tempus modus vox forma-verbi
    gradus species declinatio coniugatio, absentia OMISSA, declinatio/
    coniugatio numeri ut chordae. Instrumentum semel per quaestionem
    cursum, memoriter."""

    def __init__(self, via_aut_textus):
        t = via_aut_textus
        if isinstance(t, bytes):
            self.via, self.octeti = None, t
        elif t and '\n' not in t and os.path.isfile(_absoluta(t)):
            self.via, self.octeti = _absoluta(t), None
        elif t.endswith('.txt') and ' ' not in t and '\n' not in t:
            raise SilvaError('Oratio: plagula absens: %s' % t)
        else:
            self.via, self.octeti = None, t.encode('utf-8')
        self._sententiae = None
        self._vocabula = None
        self._analyses = None

    @property
    def textus(self):
        """fons decodificatus (utf-8)"""
        if self.octeti is None:
            self.octeti = open(self.via, 'rb').read()
        return self.octeti.decode('utf-8', errors='replace')

    def _cum_plagula(self, f):
        if self.via:
            return f(self.via)
        d = os.path.join(RADIX, 'build', 'pythonica')
        os.makedirs(d, exist_ok=True)
        fd, via_t = tempfile.mkstemp(prefix='.oratio_', suffix='.txt', dir=d)
        with os.fdopen(fd, 'wb') as h:
            h.write(self.octeti)
        try:
            return f(via_t)
        finally:
            os.unlink(via_t)

    def _machina(self, imperium, optiones=()):
        """lineae TSV instrumenti (-machina): rc 1 = nihil (lista vacua),
        rc 2 = fractura"""
        def f(via):
            r = _curre([imperium, via, '-machina'] + list(optiones))
            lineae = r.stdout.splitlines()
            # caput '#' = instrumentum cucurrit; rc 1 sine capite =
            # involucrum fractum (compilatio), non 'nihil inventum'
            if r.returncode == 2 or not any(l.startswith('#') for l in lineae):
                raise SilvaError('%s fractus (rc %d): %s'
                                 % (imperium, r.returncode,
                                    r.stderr.strip()[-300:]))
            return [l.split('\t') for l in lineae
                    if l and not l.startswith('#')]
        return self._cum_plagula(f)

    def sententiae(self):
        """[OratioSententia] ordine documenti (forma paragraphi: prosa
        versus titulus tabula index; textus lineis novis in spatia versis)"""
        if self._sententiae is None:
            self._sententiae = [
                OratioSententia(i, int(p[0]), int(p[1]), int(p[2]), p[3], p[4])
                for i, p in enumerate(self._machina('./oratio/sententiae.sh'))]
        return list(self._sententiae)

    def vocabula(self, classis=None, lingua=None, ignota=False, sententia=None):
        """[OratioVocabulum] ordine documenti; classis = quae eam inter
        classes ferunt (candidatum quodque, non primum solum); lingua item;
        ignota = sine analysi; sententia = ordinalis sententiae"""
        if self._vocabula is None:
            self._vocabula = [
                OratioVocabulum(int(p[1]), int(p[2]), int(p[3]), int(p[4]),
                                int(p[5]), int(p[6]), p[7],
                                tuple(p[8].split()), tuple(p[9].split()),
                                p[10], int(p[11]))
                for p in self._machina('./oratio/verba.sh')]
        xs = self._vocabula
        if classis is not None:
            xs = [v for v in xs if classis in v.classes]
        if lingua is not None:
            xs = [v for v in xs if lingua in v.linguae]
        if ignota:
            xs = [v for v in xs if 'ignotum' in v.classes]
        if sententia is not None:
            xs = [v for v in xs if v.sententia == sententia]
        return list(xs)

    def ignota(self):
        """vocabula sine analysi (inventa)"""
        return self.vocabula(ignota=True)

    def analyses(self, vocabulum):
        """[OratioAnalysis] vocabuli (OratioVocabulum aut index) ordine
        fontis; [] sine analysi"""
        if self._analyses is None:
            self._analyses = {}
            for p in self._machina('./oratio/verba.sh', ['-analyses']):
                i = int(p[1])
                acc = dict((k, v) for k, v in zip(ORATIO_ACCIDENTIA, p[9:])
                           if v)
                self._analyses.setdefault(i, []).append(
                    OratioAnalysis(i, p[3], p[4], p[5], p[6], p[7], p[8], acc))
        i = vocabulum.index if isinstance(vocabulum, OratioVocabulum) \
            else int(vocabulum)
        return list(self._analyses.get(i, []))
