"""pythonica/lectiones.py - liber lectionum pythonis (fabrica plan 5 T10,
spec 5 par. V.2, A4): idem liber ac lib/lectiones.c, lineae eiusdem
formae ('L\\tvia', 'E\\ttitulus\\tvalor' ...) in plagulam quam
FABRICA_LECTIONES nominat (omni nota relecta, ut in C).

Canales:
  auditus (sys.addaudithook): 'open' - lectio L aut A (exstantia per
      stat VERUM ante apertionem; auditus ante eam currit), scriptura S;
      'os.listdir' / 'os.scandir' - D.
  involucra (CPython ea non audit): os.stat et os.lstat (X aut A) - et
      familia os.path (exists, isfile, isdir, getmtime ...) quae os.stat
      tempore vocationis quaerit; os.environ per dictionarium
      notans: get, [], in -> E. Copia tota (dict(os.environ),
      os.environ.copy()) NIHIL notat - ut environ in C, getenv solum
      (variabiles terminalis, TERM_SESSION_ID, verdictum aliter
      irritarent).
Viae sub /dev/ non notantur (machina, non ingressus - ut C). Via ut
data notatur (relativa aut absoluta), ut C.

instituere() semel; auditus removeri nequit (sys.addaudithook) - hinc
probationes eum in FILIO ponunt.
"""
import os
import sys

_stat_verum = os.stat
_lstat_verum = os.lstat
_environ_verum = os.environ
_descriptor = None
_via_aperta = None
_intus = False
_institutum = False


def _liber():
    return _environ_verum.get('FABRICA_LECTIONES')


def notare(littera, via):
    """lineam unam scribere: 'littera<TAB>via' (via iam cum valore pro E)"""
    global _descriptor, _via_aperta, _intus
    if _intus:
        return
    liber = _liber()
    if not liber:
        if _descriptor is not None:
            os.close(_descriptor)
            _descriptor = None
        return
    _intus = True
    try:
        if _descriptor is None or liber != _via_aperta:
            if _descriptor is not None:
                os.close(_descriptor)
                _descriptor = None
            try:
                _descriptor = os.open(liber, os.O_WRONLY | os.O_APPEND
                                      | os.O_CREAT, 0o644)
            except OSError:
                return
            _via_aperta = liber
        os.write(_descriptor, ('%s\t%s\n' % (littera, via)).encode(
            'utf-8', 'surrogateescape'))
    finally:
        _intus = False


def _via(res):
    """via ut chorda, aut None (descriptor, aliud)"""
    if isinstance(res, int):
        return None
    try:
        res = os.fspath(res)
    except TypeError:
        return None
    if isinstance(res, bytes):
        res = res.decode('utf-8', 'surrogateescape')
    return res


def _machina(via):
    return via.startswith('/dev/')


def _exstat(via):
    try:
        _stat_verum(via)
        return True
    except (OSError, ValueError):
        return False


def _auditus(eventus, argumenta):
    if _intus or eventus not in ('open', 'os.listdir', 'os.scandir'):
        return
    if not _liber():
        return
    if eventus == 'open':
        via = _via(argumenta[0])
        if via is None or _machina(via):
            return
        modus, vexilla = argumenta[1], argumenta[2]
        if modus is not None:
            lectio = 'r' in modus and '+' not in modus
        else:
            vexilla = vexilla or 0
            lectio = (vexilla & (os.O_WRONLY | os.O_RDWR)) == 0
        if lectio:
            notare('L' if _exstat(via) else 'A', via)
        else:
            notare('S', via)
        return
    via = _via(argumenta[0]) if argumenta and argumenta[0] is not None else '.'
    if via is not None and not _machina(via):
        notare('D', via)


def _stat_notans(verum, littera_absentis='A'):
    def stat(via, *reliqua, **optiones):
        try:
            responsum = verum(via, *reliqua, **optiones)
        except OSError:
            textus = _via(via)
            if textus is not None and not _machina(textus):
                notare(littera_absentis, textus)
            raise
        textus = _via(via)
        if textus is not None and not _machina(textus):
            notare('X', textus)
        return responsum
    stat.__name__ = verum.__name__
    stat.__doc__ = verum.__doc__
    return stat


class _EnvironNotans(dict):
    """os.environ notans: lectiones explicitae (get, [], in) E notant;
    mutationes per os.environ verum (putenv) transeunt et hic
    speculantur. __iter__/keys non superscripta: dict(hoc) per viam
    celerem CPython copiat sine __getitem__ - copia nihil notat."""

    def __getitem__(self, titulus):
        try:
            valor = dict.__getitem__(self, titulus)
        except KeyError:
            notare('E', titulus)
            raise
        notare('E', '%s\t%s' % (titulus, valor))
        return valor

    def get(self, titulus, ordinarium=None):
        if dict.__contains__(self, titulus):
            valor = dict.__getitem__(self, titulus)
            notare('E', '%s\t%s' % (titulus, valor))
            return valor
        notare('E', titulus)
        return ordinarium

    def __contains__(self, titulus):
        if dict.__contains__(self, titulus):
            notare('E', '%s\t%s' % (titulus, dict.__getitem__(self, titulus)))
            return True
        notare('E', titulus)
        return False

    def __setitem__(self, titulus, valor):
        _environ_verum[titulus] = valor
        dict.__setitem__(self, titulus, valor)

    def __delitem__(self, titulus):
        del _environ_verum[titulus]
        dict.__delitem__(self, titulus)

    def pop(self, titulus, *ordinarium):
        _environ_verum.pop(titulus, None)
        return dict.pop(self, titulus, *ordinarium)

    def popitem(self):
        titulus, valor = dict.popitem(self)
        _environ_verum.pop(titulus, None)
        return titulus, valor

    def setdefault(self, titulus, ordinarium=None):
        if not dict.__contains__(self, titulus):
            self[titulus] = ordinarium
        return dict.__getitem__(self, titulus)

    def update(self, *alia, **optiones):
        for titulus, valor in dict(*alia, **optiones).items():
            self[titulus] = valor

    def clear(self):
        _environ_verum.clear()
        dict.clear(self)

    def copy(self):
        return dict(self)


def instituere():
    """auditum et involucra ponere (semel; idempotens)"""
    global _institutum
    if _institutum:
        return
    _institutum = True
    sys.addaudithook(_auditus)
    os.stat = _stat_notans(_stat_verum)
    os.lstat = _stat_notans(_lstat_verum)
    os.environ = _EnvironNotans(_environ_verum)
