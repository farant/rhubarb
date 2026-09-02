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
        if tolerans:
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

    def substituere(self, nomen, novus, definitio=True):
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_nodi - 1]) + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('substituere %s' % nomen)
        return self

    def inserere_post(self, nomen, novus, definitio=True):
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio)
        if not novus.endswith('\n'):
            novus += '\n'
        lineae = self._lineae()
        self.textus = ''.join(lineae[:x.linea_b]) + '\n' + novus \
            + ''.join(lineae[x.linea_b:])
        self.acta.append('inserere_post %s' % nomen)
        return self

    def inserere_ante(self, nomen, novus, definitio=True):
        x = _extentum_nominis(self._extenta_praesentia(), nomen, definitio)
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
