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
    # errors='replace': effusum portae octetos non-UTF-8 ferre potest
    # (probatio octetos crudos imprimens) - decodificatio stricta
    # operarium umbrae 2026-09-02 tacite necavit, signum pendens mansit
    return subprocess.run(args, cwd=RADIX, capture_output=True,
                          text=True, errors='replace', input=stdin)


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
    """commentum = lexema unum: spatia INTRA collapsa (tabulae, lineae
    refractae, indentatio commenti ancoram non frangunt - 2026-09-02);
    cetera exacta"""
    if t.startswith('/*'):
        return ' '.join(t.split())
    return t


def _sedes_lexematum(textus, vetus):
    """spatia [initium, finis) ubi series lexematum ancorae in textu
    apparet"""
    anc = [_lexema_norma(t) for t, _, _ in _lexemata(vetus)]
    if not anc:
        raise SilvaError('ancora sine lexematis')
    lex = [(_lexema_norma(t), a, b) for t, a, b in _lexemata(textus)]
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

    def replace_inter(self, initium, finis, novus, tolerans=True):
        """spatium ab INITIO ancorae 'initium' ad FINEM ancorae 'finis'
        (post initium quaesitae) substituere - ambae breves, unicae;
        quidquid inter eas (commenta inclusa) abit. Pro blocis quae
        commenta tenent: ancora totius bloci commenta omnia ferre
        deberet (lexema unum quodque) - hic duo anchorae solae."""
        if tolerans is True:
            a = _sedes_lexematum(self.textus, initium)
        else:
            a = [(i, i + len(initium)) for i in
                 [j for j in range(len(self.textus)) if self.textus.startswith(initium, j)]]
        if len(a) != 1:
            raise SilvaError("ancora initii %d vicibus inventa: %r" % (len(a), initium[:60]))
        a0 = a[0][0]
        cauda = self.textus[a0:]
        if tolerans is True:
            b = _sedes_lexematum(cauda, finis)
        else:
            b = [(i, i + len(finis)) for i in
                 [j for j in range(len(cauda)) if cauda.startswith(finis, j)]]
        if len(b) < 1:
            raise SilvaError("ancora finis post initium non inventa: %r" % finis[:60])
        b1 = a0 + b[0][1]
        self.textus = self.textus[:a0] + novus + self.textus[b1:]
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
            raise SilvaError("ancora %d vicibus inventa (exspectatae %d):"
                             " %r" % (len(sedes), numerus, vetus[:60]))
        for a, b in reversed(sedes):
            self.textus = self.textus[:a] + novus + self.textus[b:]
        self.acta.append('replace %r' % vetus[:40])
        return self

    def inserere_post(self, vetus, novus, numerus=1):
        return self.replace(vetus, vetus + novus, numerus)

    def inserere_ante(self, vetus, novus, numerus=1):
        return self.replace(vetus, novus + vetus, numerus)

    def replace_inter(self, initium, finis, novus):
        """ab initio 'initium' (semel) ad finem 'finis' (prima post
        initium) substituere - exacte"""
        if self.textus.count(initium) != 1:
            raise SilvaError("ancora initii %d vicibus: %r" % (self.textus.count(initium), initium[:60]))
        a0 = self.textus.index(initium)
        b = self.textus.find(finis, a0 + len(initium))
        if b < 0:
            raise SilvaError("ancora finis post initium non inventa: %r" % finis[:60])
        self.textus = self.textus[:a0] + novus + self.textus[b + len(finis):]
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
    'aequivalentia': (['./silva/aequivalentia.sh', '-conferre'],
                      r'aequivalentia: (sana|FRACTA)'),
    'amalgamata': (['./tools/amalgamata_probare.sh'],
                   r'amalgamata: \d+ compilata, \d+ fracta'),
}
_ANSI = re.compile(r'\x1b\[[0-9;]*m')

# forma effusus portae: 'radix' (Testing: X ... ✗ TEST FAILED: X (t)),
# 'suita' (=== X === ... FRACTAE: X Y - silva et sub-suitae omnes),
# aliter 'generica' (porta tota = fractura una)
FORMAE = {'radix': 'radix', 'silva': 'suita', 'css': 'suita',
          'materia': 'suita', 'officina': 'suita', 'gesta': 'suita',
          'tessera': 'suita', 'saltuarius': 'suita'}
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
            m = re.search(r'✗ (?:TEST |COMPILATION |BUILD )?FAILED: (\S+)', l)
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
            f = receptum_validum(p)            # porta ex umbra
            nomen = f.nomen + ' (receptum)'
        else:
            nomen, filtrum = (p, None) if isinstance(p, str) else p
            f = porta(nomen, filtrum)
        if not f.sana:
            raise SilvaError('porta %s non sana (%s, rc=%d) - nihil'
                             ' commissum%s' % (nomen, f.compendium, f.rc,
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
                      'compendium rc sigillum rancida finis fracturae',
                      defaults=([],))


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
    p = subprocess.Popen(
        [sys.executable, os.path.abspath(__file__), '-umbra', nomen,
         filtrum or '', via], cwd=RADIX, start_new_session=True,
        stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL)
    # signum pendens = PID operarii: operarius mortuus sine recepto
    # nominatur (portae_pendentes 'mortua', exspectare levat statim)
    open(via + '.pendens', 'w').write(str(p.pid))
    return via


def _pendens_mortua(via):
    """VERUM si signum pendens PID mortuum fert (operarius abiit sine
    recepto); FALSUM si vivus aut signum vetus sine PID"""
    try:
        pid = int(open(via + '.pendens').read().strip())
    except (IOError, OSError, ValueError):
        return False
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return True
    except OSError:
        return False
    return False


def _umbra_currere(nomen, filtrum, via):
    """corpus operarii umbrae (processus separatus). Ruina QUAELIBET
    receptum FRACTUM scribit (traceback in .acta) et signum pendens
    tollit - operarius numquam tacite abit."""
    import traceback
    sig = '?'
    try:
        sig = sigillum_arboris()
        p = porta(nomen, filtrum or None)
        acta = p.acta
        d = {'nomen': nomen, 'filtrum': filtrum or None, 'sana': p.sana,
             'cucurrit': p.cucurrit, 'compendium': p.compendium,
             'rc': p.rc, 'sigillum': sig,
             'rancida': sig != sigillum_arboris(), 'finis': time.time(),
             'fracturae': [list(f) for f in (p.fracturae or [])]}
    except Exception:
        acta = traceback.format_exc()
        ultima = acta.strip().splitlines()[-1][:200]
        d = {'nomen': nomen, 'filtrum': filtrum or None, 'sana': False,
             'cucurrit': False, 'compendium': 'UMBRA FRACTA: ' + ultima,
             'rc': -1, 'sigillum': sig, 'rancida': False,
             'finis': time.time(), 'fracturae': []}
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
                    [Fractura(*f) for f in d.get('fracturae', [])])


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
    for f in sorted(os.listdir(PORTAE_DIR)):
        via = os.path.join(PORTAE_DIR, f)
        if f.endswith('.pendens'):
            exitus.append((via[:-8], 'mortua (operarius abest, receptum'
                           ' nullum)' if _pendens_mortua(via[:-8])
                           else 'pendens'))
        elif f.endswith('.json'):
            r = receptum_legere(via)
            if r.rancida or r.sigillum != sig:
                exitus.append((via, 'rancida'))
            else:
                exitus.append((via, 'sana' if r.sana else 'fracta'))
    return exitus


def _totum_actorum(acta):
    m = re.search(r'Total Time:\s*\S*?([\d.]+)s', acta)
    return float(m.group(1)) if m else None


def commissio_umbra(nuntius, viae, portae, verificare=True, tectum=1800,
                    siccum=False):
    """Portae umbrae SERIATIM (tempora non contendunt - mensurae suitae
    fidae manent), deinde commissio contra recepta OMNIA, recepta
    deleta. Porta fracta: SilvaError cum cauda actorum, recepta
    servata ad inspectionem. siccum: portae solae, nihil commissum.
    Reddit (hash | None, [(nomen, compendium, totum_secunda)])."""
    recepta = []
    for p in portae:
        nomen, filtrum = (p, None) if isinstance(p, str) else p
        via = porta_umbra(nomen, filtrum)
        r = exspectare(via, tectum)
        acta = open(via + '.acta').read()
        tot = _totum_actorum(acta)
        if tot is None:
            praef = {'radix': '', 'silva': 'silva.'}.get(nomen)
            if praef is not None:
                ss = mensurae(praef, 1, plenae=False)
                if ss and (time.time() - float(ss[0].mensurae.get('suita.tempus.totum', 0)) > 0):
                    tot = ss[0].mensurae.get('suita.tempus.totum')
        recepta.append((via, nomen, r, tot))
        if not r.sana:
            rel = relatio_fracturarum(r.fracturae) or '\n' + acta[-800:]
            raise SilvaError('porta umbrae %s non sana: %s%s\n(acta: %s.acta)'
                             % (nomen, r.compendium, rel, via))
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


Mensura = namedtuple('Mensura', 'via parsare_ms lexare_ms phases allocationes usus ordo')


def metiri(via, n=7, nudum=False):
    """computus min-of-n (singuli +-X%): parsare/lexare ms, phases
    (lex/expansio/glr/commissio), allocationes, usus - pro A/B
    optimizationum. ordo = ordo TSV integer cursus optimi."""
    best = None
    for _ in range(n):
        args = ['./silva/computus.sh', _absoluta(via), '-machina'] + (['-nudum'] if nudum else [])
        out = _curre(args).stdout.splitlines()
        rows = [l.split('\t') for l in out if l and not l.startswith('#')]
        if not rows:
            raise SilvaError('computus sine ordine: %s' % via)
        r = rows[0]
        v = float(r[7])
        if best is None or v < best[0]:
            best = (v, r)
    r = best[1]
    phases = {}
    if len(r) >= 20:
        phases = {'lex': float(r[16]), 'expansio': float(r[17]),
                  'glr': float(r[18]), 'commissio': float(r[19])}
    return Mensura(via, best[0], float(r[6]), phases, int(r[13]), int(r[8]), r)


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
