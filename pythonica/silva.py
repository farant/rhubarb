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
    print(e.applicare())                        # omnia aut nihil; diff
    print(silva.verdictum('lib/chorda.c'))      # examen + forma
    assert silva.differre(vetus, novus).cosmetica_solum

Contractus:
  - Editio mutat textum IN MEMORIA; nihil in discum it ante applicare().
    Ancora fallens levat SilvaError - plagula intacta (omnia aut nihil).
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
                      'titulus linea_a linea_nodi linea_b definitio')
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
                               o[5] == 'definitio'))
    return exitus


def _extentum_nominis(lista, nomen, definitio=True):
    candidati = [x for x in lista if x.titulus == nomen
                 and x.definitio == definitio]
    if not candidati:
        genus = 'definitio' if definitio else 'prototypum'
        raise SilvaError("functio '%s' (%s) non inventa" % (nomen, genus))
    if len(candidati) > 1:
        raise SilvaError("functio '%s' ambigua (%d extenta)"
                         % (nomen, len(candidati)))
    return candidati[0]


def corpus(via, nomen, definitio=True):
    """Textus nodi (sine commentario ducente) functionis nomine."""
    x = _extentum_nominis(extenta(via), nomen, definitio)
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

    # -- exitus --
    def diff(self):
        return ''.join(difflib.unified_diff(
            self.originalis.splitlines(True), self.textus.splitlines(True),
            'a/' + self.via, 'b/' + self.via))

    def mutata(self):
        return self.textus != self.originalis

    def applicare(self):
        """scribit semel (omnia aut nihil); diff reddit ('' si intacta)"""
        if not self.mutata():
            return ''
        d = self.diff()
        with open(_absoluta(self.via), 'w') as f:
            f.write(self.textus)
        self.originalis = self.textus
        return d


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
