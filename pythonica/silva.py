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
  - Ancorae tolerantes: spatia inter verba ancorae quaelibet spatia
    congruunt (reordinatio formatoris ancoram non frangit); ubi ancora
    spatium non habet, nullum admittitur.
  - Nomina (substituere/inserere) per formator -extenta resolvuntur in
    textu PRAESENTI (post editiones priores) - lineae labuntur, nomina
    manent.
"""
import difflib
import os
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


def _curre(args, stdin=None):
    return subprocess.run(args, cwd=RADIX, capture_output=True,
                          text=True, input=stdin)


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
        raise SilvaError("'%s' (%s) non inventum" % (nomen, genus))
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


def _sedes_lexematum(textus, vetus):
    """spatia [initium, finis) ubi series lexematum ancorae in textu
    apparet"""
    anc = [t for t, _, _ in _lexemata(vetus)]
    if not anc:
        raise SilvaError('ancora sine lexematis')
    lex = _lexemata(textus)
    n = len(anc)
    sedes = []
    for i in range(len(lex) - n + 1):
        if all(lex[i + k][0] == anc[k] for k in range(n)):
            sedes.append((lex[i][1], lex[i + n - 1][2]))
    return sedes


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

    # -- ancorae textuales --
    def replace(self, vetus, novus, tolerans=True, numerus=1):
        """tolerans=True (ordinarius): ancora ut SERIES LEXEMATUM - spatia
        ubique indifferentia, etiam addita/ablata ('a(vacuum)' congruit
        'a (vacuum)', parametra in lineas fissa congruunt); litterae
        chordae exacte. 'spatia': cursus spatiorum solum (forma vetus).
        False: octeti exacti."""
        if tolerans is True:
            sedes = _sedes_lexematum(self.textus, vetus)
        elif tolerans == 'spatia':
            sedes = [m.span() for m in
                     _exemplar_tolerans(vetus).finditer(self.textus)]
        else:
            sedes = []
            i = self.textus.find(vetus)
            while i >= 0:
                sedes.append((i, i + len(vetus)))
                i = self.textus.find(vetus, i + 1)
        if len(sedes) != numerus:
            raise SilvaError("ancora %d vicibus inventa (exspectatae %d):"
                             " %r" % (len(sedes), numerus, vetus[:60]))
        for a, b in reversed(sedes):
            self.textus = self.textus[:a] + novus + self.textus[b:]
        self.acta.append('replace %r' % vetus[:40])
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

    def _lineae(self):
        return self.textus.splitlines(True)

    def substituere(self, nomen, novus, definitio=True, genus=None):
        """corpus nodi nomine substituere (commentarium ducens manet);
        genus='typus' pro structura/unione/enumeratione (aut genus
        exactum: 'structura' ...) - sine genere functiones solae."""
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio,
                              genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_nodi - 1]) + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('substituere %s' % nomen)
        return self

    def inserere_post(self, nomen, novus, definitio=True, genus=None):
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio,
                              genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_b]) + '\n' + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('inserere_post %s' % nomen)
        return self

    def inserere_ante(self, nomen, novus, definitio=True, genus=None):
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio,
                              genus)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        # ante nodum ipsum (post commentarium ducens functionis sequentis?
        # non: ante commentarium - extentum incipit post nodum priorem)
        self.textus = ''.join(lineae[:x.linea_a - 1]) + novus + '\n' \
            + ''.join(lineae[x.linea_a - 1:])
        self.acta.append('inserere_ante %s' % nomen)
        return self

    def membrum_addere(self, typus, textus, post=None):
        """membrum structurae/unionis/enumerationis nomine typi addere:
        ante lineam claudentem ('} Titulus;'), aut post membrum cuius
        linea 'post' (ancora tolerans intra typum) continet. Forma
        (applicare) columnas ordinat."""
        x = _extentum_nominis(self._extenta_praesentia(), typus,
                              genus='typus')
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


# ---------------------------------------------------------------- portae

Porta = namedtuple('Porta', 'nomen cucurrit sana compendium rc acta')

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
    'amalgamata': (['./tools/amalgamata_probare.sh'],
                   r'amalgamata: \d+ compilata, \d+ fracta'),
}
_ANSI = re.compile(r'\x1b\[[0-9;]*m')


def porta_viae(via):
    """porta directorii plagulae: radix pro lib/include/probationes/tools"""
    v = via.replace('\\', '/')
    prima = v.split('/')[0]
    if prima in ('lib', 'include', 'probationes', 'tools'):
        return 'radix'
    if prima in PORTAE:
        return prima
    raise SilvaError('porta viae ignota: %s' % via)


def porta(nomen, filtrum=None):
    """portam currere: Porta(nomen, cucurrit, sana, compendium, rc,
    acta). sana SOLUM si cucurrit ET rc == 0 ET signum non fractum.
    NB: suites totae (radix, silva) minuta capiunt - in vocamine
    instrumenti tectum X minutorum: filtrum da aut in umbra curre."""
    if nomen not in PORTAE:
        raise SilvaError('porta ignota: %s (nota: %s)'
                         % (nomen, ', '.join(sorted(PORTAE))))
    imperium, signum = PORTAE[nomen]
    args = list(imperium) + ([filtrum] if filtrum else [])
    r = _curre(args)
    acta = _ANSI.sub('', r.stdout + r.stderr)
    m = re.search(signum, acta)
    cucurrit = m is not None
    compendium = m.group(0) if m else '(signum absens)'
    sana = cucurrit and r.returncode == 0 \
        and not re.search(r'FRACT|Fracti:\s*[1-9]|Failed:\s*[1-9]',
                          compendium)
    return Porta(nomen, cucurrit, sana, compendium, r.returncode, acta)


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
            f = receptum_validum(p)            # porta ex umbra
            nomen = f.nomen + ' (receptum)'
        else:
            nomen, filtrum = (p, None) if isinstance(p, str) else p
            f = porta(nomen, filtrum)
        if not f.sana:
            raise SilvaError('porta %s non sana (%s, rc=%d) - nihil'
                             ' commissum' % (nomen, f.compendium, f.rc))
    r = _curre(['git', 'add', '--'] + list(viae))
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
                      'compendium rc sigillum rancida finis')


def sigillum_arboris():
    """sigillum status arboris: HEAD + differentia plagularum tractarum
    (index et arbor) + plagulae novae non ignoratae cum contentu.
    Receptum portae huic sigillo ligatur - mutatio ulla = rancidum."""
    h = hashlib.sha256()
    h.update(_curre(['git', 'rev-parse', 'HEAD']).stdout.encode())
    h.update(_curre(['git', 'diff', 'HEAD', '--']).stdout.encode('utf-8',
                                                                   'replace'))
    novae = _curre(['git', 'ls-files', '--others', '--exclude-standard']
                   ).stdout.split()
    for v in sorted(novae):
        h.update(v.encode())
        try:
            with open(os.path.join(RADIX, v), 'rb') as f:
                h.update(hashlib.sha256(f.read()).digest())
        except (IOError, OSError):
            h.update(b'?')
    return h.hexdigest()[:16]


def _receptum_via(nomen, filtrum):
    return os.path.join(PORTAE_DIR, '%s%s.%d.json' % (
        nomen, '.' + filtrum if filtrum else '', int(time.time())))


def porta_umbra(nomen, filtrum=None):
    """portam in umbra currere (processus separatus, sessio propria):
    receptum JSON in build/portae/ scriptum in fine - sigillo arboris
    initii ligatum (rancidum si arbor mutata dum currit). Reddit viam
    recepti (pendens: '.pendens' iuxta eam dum currit). exspectare(via)
    manet; commissio(portae=[via]) receptum ut portam accipit."""
    if nomen not in PORTAE:
        raise SilvaError('porta ignota: %s' % nomen)
    os.makedirs(PORTAE_DIR, exist_ok=True)
    via = _receptum_via(nomen, filtrum)
    open(via + '.pendens', 'w').write(str(time.time()))
    subprocess.Popen(
        [sys.executable, os.path.abspath(__file__), '-umbra', nomen,
         filtrum or '', via], cwd=RADIX, start_new_session=True,
        stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL)
    return via


def _umbra_currere(nomen, filtrum, via):
    """corpus operarii umbrae (processus separatus)"""
    sig = sigillum_arboris()
    p = porta(nomen, filtrum or None)
    with open(via + '.acta', 'w') as f:
        f.write(p.acta)
    sig_finis = sigillum_arboris()
    d = {'nomen': nomen, 'filtrum': filtrum or None, 'sana': p.sana,
         'cucurrit': p.cucurrit, 'compendium': p.compendium, 'rc': p.rc,
         'sigillum': sig, 'rancida': sig != sig_finis,
         'finis': time.time()}
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
                    d['finis'])


def receptum_validum(via):
    """Porta ex recepto: sana SOLUM si receptum sanum, non rancidum, et
    sigillum arboris PRAESENS idem (nihil mutatum post cursum)"""
    r = receptum_legere(via)
    recens = r.sigillum == sigillum_arboris()
    sana = r.sana and r.cucurrit and not r.rancida and recens
    causa = r.compendium
    if r.rancida:
        causa += ' [arbor mutata DUM currebat]'
    elif not recens:
        causa += ' [arbor mutata POST cursum - receptum rancidum]'
    return Porta(r.nomen, r.cucurrit, sana, causa, r.rc, '')


def receptum_delere(via):
    """receptum cum actis et signo pendenti delere - post consumptionem
    (commissio) aut in fine probationis; absens = nihil"""
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
        time.sleep(intervallum)
    raise SilvaError('receptum non venit intra %ds: %s' % (tectum, via))


def portae_pendentes():
    """[(via, status)] status: pendens | sana | fracta | rancida -
    orientatio post compactionem: quid in cursu erat"""
    if not os.path.isdir(PORTAE_DIR):
        return []
    exitus = []
    sig = sigillum_arboris()
    for f in sorted(os.listdir(PORTAE_DIR)):
        via = os.path.join(PORTAE_DIR, f)
        if f.endswith('.pendens'):
            exitus.append((via[:-8], 'pendens'))
        elif f.endswith('.json'):
            r = receptum_legere(via)
            if r.rancida or r.sigillum != sig:
                exitus.append((via, 'rancida'))
            else:
                exitus.append((via, 'sana' if r.sana else 'fracta'))
    return exitus


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
    return rubra.compendium, viridis.compendium


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
        return fructus


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
        _umbra_currere(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        sys.stderr.write('usus: silva.py -umbra <porta> <filtrum|""> '
                         '<receptum.json>\n')
        sys.exit(2)
