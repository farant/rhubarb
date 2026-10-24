#!/usr/bin/env python3
"""bearssl_amalgamare.py - vendor/bearssl_amalgama.c + .h GENERATA
ex arbore upstream vendor/bearssl/ (tls-bearssl-plan T1, regula R1/R2).

CUR AMALGAMA (ut vendor/sqlite3.c): viae aedificationis domus (aedilis
X.h -> X.c, fabrica, compile_tests.sh VENDOR_SOURCES, ludus_tessera,
silex vendor/<basis>) par UNUM caput + UNUM fontem tractant.

COLLISIONES: fontes upstream nomina gradus plagulae (static, typedef,
tag, enumeratio) inter se iterant et macra propria definiunt. ORACULUM
clang: -ast-dump=json per fontem -> nomina in fonte IPSO definita;
'#define' textualia fontis -> macra eius. Nomen in pluribus fontibus ->
'#define n n__<fons>' ante fontem, '#undef' post; macra fontis post eum
'#undef'. Probatio: tools/bearssl_probare.sh (KAT upstream).

C89 (caput): 'static inline' -> sub C89 'inline' fit
'__attribute__((unused))' (silva __attribute__ ut vacuum legit; 'unused'
monitum functionis non adhibitae tacet). Fons ipse C99: nihil mutatur.

Usus: tools/bearssl_amalgamare.py [radix]
"""
import concurrent.futures
import hashlib
import json
import os
import re
import subprocess
import sys

RADIX = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else '.')
ARBOR = os.path.join(RADIX, 'vendor', 'bearssl')
# TITULUS PROPRIUS (non 'bearssl.h'): instrumenta silvae capita per
# BASENAME totius arboris resolvunt (primus in ambulatione vincit) -
# vendor/bearssl/inc/bearssl.h prior inveniebatur, limen C89 praeteritum
# (tls-bearssl R13)
# FABRICA_SCRIPTURA (bin/fabrica -plenus): exitus in scripturam, ingressus
# ex arbore (ut tools/runae_generare.sh)
EXITUS_RADIX = os.environ.get('FABRICA_SCRIPTURA') or RADIX
os.makedirs(os.path.join(EXITUS_RADIX, 'vendor'), exist_ok=True)
FONS_EXITUS = os.path.join(EXITUS_RADIX, 'vendor', 'bearssl_amalgama.c')
CAPUT_EXITUS = os.path.join(EXITUS_RADIX, 'vendor', 'bearssl_amalgama.h')
CLANG = ['clang', '-std=c99', '-I' + os.path.join(ARBOR, 'inc'),
         '-I' + os.path.join(ARBOR, 'src')]
DEFINE = re.compile(r'^[ \t]*#[ \t]*define[ \t]+([A-Za-z_][A-Za-z_0-9]*)',
                    re.M)
UNDEF = re.compile(r'^[ \t]*#[ \t]*undef[ \t]+([A-Za-z_][A-Za-z_0-9]*)',
                   re.M)
DEFINITIO = re.compile(r'^[ \t]*#[ \t]*define[ \t]+([A-Za-z_][A-Za-z_0-9]*)'
                       r'((?:[^\n]*\\\n)*[^\n]*)', re.M)
INCLUDE_INTERIUS = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"inner\.h"'
                              r'[^\n]*\n', re.M)
INCLUDE_CITATUM = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"([^"]+)"', re.M)
INCLUDE_CAPITIS = re.compile(r'^#include "(bearssl[a-z0-9_]*\.h)"\n',
                             re.M)
PRAEFATIO = ('/* {} - GENERATUM a tools/bearssl_amalgamare.py ex '
             'vendor/bearssl (BearSSL 7bea48e, MIT) - NE MANU EDITES */\n')


def fracta(nuntius):
    sys.stderr.write('bearssl_amalgamare: FRACTA: ' + nuntius + '\n')
    sys.exit(1)


def legere(via):
    with open(via, encoding='utf-8') as f:
        return f.read()


def definitiones(textus):
    """titulus macri -> corpora definitionum (spatia normata, lineae
    continuatae iunctae; variantes #if omnes)."""
    d = {}
    for n, corpus in DEFINITIO.findall(textus):
        d.setdefault(n, set()).add(' '.join(
            corpus.replace('\\\n', ' ').split()))
    return d


# PROMPTUARIUM (2026-10-23): nomina per fontem memorata - clang
# -ast-dump=json per CCXC fontes ~LXII s (auditus plenus generatorum:
# XLIII% totius). Clavis = octeti fontis + capita omnia arboris (macra
# definitiones creare possunt) + argumenta clang (viae RELATIVAE: arbores
# operis communicant) + versio clang. In arbore (non in scriptura), ut
# regenerationes in scriptura eo utantur; vestigium actionis.
PROMPTUARIUM = os.path.join(RADIX, 'build', 'bearssl_amalgama_promptuarium')
_CLAVIS_COMMUNIS = None


def _clavis_communis():
    """capita arboris + argumenta + versio clang, semel per cursum"""
    global _CLAVIS_COMMUNIS
    if _CLAVIS_COMMUNIS is None:
        h = hashlib.sha256()
        for d, sub, fs in sorted(os.walk(ARBOR)):
            sub.sort()
            for f in sorted(fs):
                if f.endswith('.h'):
                    via = os.path.join(d, f)
                    h.update(os.path.relpath(via, ARBOR).encode())
                    with open(via, 'rb') as fh:
                        h.update(fh.read())
        h.update(' '.join(['-std=c99', '-Iinc', '-Isrc', '-fsyntax-only',
                           '-ast-dump=json']).encode())
        r = subprocess.run(['clang', '--version'], capture_output=True,
                           text=True)
        h.update(r.stdout.encode())
        _CLAVIS_COMMUNIS = h.hexdigest()
    return _CLAVIS_COMMUNIS


def nomina_interiora(via):
    """nomina_interiora_clang per promptuarium (fons et capita immutata
    -> sine clang)"""
    with open(via, 'rb') as fh:
        h = hashlib.sha256(fh.read())
    h.update(_clavis_communis().encode())
    plagula = os.path.join(PROMPTUARIUM, h.hexdigest() + '.json')
    try:
        with open(plagula, encoding='utf-8') as fh:
            return set(json.load(fh))
    except (OSError, ValueError):
        pass
    nomina = nomina_interiora_clang(via)
    os.makedirs(PROMPTUARIUM, exist_ok=True)
    temporaria = '%s.%d.%d' % (plagula, os.getpid(),
                               abs(hash(via)) % 1000000)
    with open(temporaria, 'w', encoding='utf-8') as fh:
        json.dump(sorted(nomina), fh)
    os.replace(temporaria, plagula)
    return nomina


def nomina_interiora_clang(via):
    """nomina gradus plagulae in fonte IPSO definita (clang)."""
    r = subprocess.run(CLANG + ['-fsyntax-only', '-Xclang',
                                '-ast-dump=json', via],
                       capture_output=True, text=True)
    if r.returncode != 0:
        fracta('clang ' + via + ': ' + r.stderr[:400])
    radix = json.loads(r.stdout)
    vera = os.path.realpath(via)
    currens = None
    nomina = set()
    for d in radix.get('inner', []):
        loc = d.get('loc', {})
        # ordo: loc, spellingLoc, expansionLoc - expansio vincit
        for k in (loc, loc.get('spellingLoc', {}),
                  loc.get('expansionLoc', {})):
            if 'file' in k:
                currens = k['file']
        inclusum = ('includedFrom' in loc
                    or 'includedFrom' in loc.get('expansionLoc', {}))
        if inclusum or currens is None \
                or os.path.realpath(currens) != vera:
            continue
        genus = d.get('kind')
        if genus in ('FunctionDecl', 'VarDecl') \
                and d.get('storageClass') == 'static':
            nomina.add(d['name'])
        if genus in ('TypedefDecl', 'RecordDecl', 'EnumDecl') \
                and d.get('name'):
            nomina.add(d['name'])
        if genus == 'EnumDecl':
            for e in d.get('inner', []):
                if e.get('kind') == 'EnumConstantDecl':
                    nomina.add(e['name'])
    return nomina


def caput_planum():
    """inc/bearssl.h cum capitibus suis inlineatis (semel quodque)."""
    visa = set()

    def explicare(titulus):
        if titulus in visa:
            return ''
        visa.add(titulus)
        return INCLUDE_CAPITIS.sub(
            lambda m: explicare(m.group(1)),
            legere(os.path.join(ARBOR, 'inc', titulus)))
    textus = explicare('bearssl.h')
    if '#include "' in textus:
        fracta('caput: include citatum residuum')
    return textus, visa


# subdirectoria src in aedificatio.stml singula declarata (plagulae
# gradu 0 tegunt; actio memorabilis): novum aut absens = declaratio
# vetus, clavis fabricae contenta eius non videret (2026-10-17)
SUBDIRECTORIA = {'aead', 'codec', 'ec', 'hash', 'int', 'kdf', 'mac',
                 'rand', 'rsa', 'ssl', 'symcipher', 'x509'}


def main():
    src = os.path.join(ARBOR, 'src')
    for d, sub, _ in os.walk(src):
        if d != src and sub:
            fracta('src gradu altiore quam I: ' + os.path.relpath(d, src))
    praesentia = {e for e in os.listdir(src)
                  if os.path.isdir(os.path.join(src, e))}
    if praesentia != SUBDIRECTORIA:
        fracta('subdirectoria src ab aedificatio.stml differunt: '
               + ' '.join(sorted(praesentia ^ SUBDIRECTORIA))
               + ' (declara in actione bearssl_amalgama)')
    fontes = sorted(
        os.path.relpath(os.path.join(d, f), os.path.join(ARBOR, 'src'))
        for d, _, fs in os.walk(os.path.join(ARBOR, 'src'))
        for f in fs if f.endswith('.c'))
    if not fontes:
        fracta('vendor/bearssl/src vacuum')
    viae = [os.path.join(ARBOR, 'src', s) for s in fontes]
    _clavis_communis()   # semel, ante fila
    with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        nomina = dict(zip(fontes, ex.map(nomina_interiora, viae)))
    textus = {s: legere(v) for s, v in zip(fontes, viae)}
    macra = {s: set(DEFINE.findall(textus[s])) for s in fontes}

    caput, capita = caput_planum()
    interius = legere(os.path.join(ARBOR, 'src', 'inner.h'))
    configuratio = legere(os.path.join(ARBOR, 'src', 'config.h'))
    # macra capitum (inner.h, config.h, bearssl.h) toti amalgamae
    # communia sunt: fons qui unum IDEM redefinit (ARSH in ec_c25519_m15
    # = inner.h:1030, C licet) id post se NON '#undef' - aliter fontes
    # posteriores id amitterent. Redefinitio DIVERSA aut '#undef' macri
    # capitum -> FRACTA (amalgama mentiretur).
    def_capitum = definitiones(caput + interius + configuratio)
    for s in fontes:
        for m in INCLUDE_CITATUM.findall(textus[s]):
            if m != 'inner.h':
                fracta(s + ': include citatum ignotum "' + m + '"')
        for n, corpora in definitiones(textus[s]).items():
            if n in def_capitum and not corpora <= def_capitum[n]:
                fracta(s + ': macrum capitum ' + n + ' aliter redefinit')
        deleta = set(UNDEF.findall(textus[s])) & set(def_capitum)
        if deleta:
            fracta(s + ': macra capitum delet: ' + ' '.join(sorted(deleta)))
        macra[s] -= set(def_capitum)

    numerus = {}
    for s in fontes:
        for n in nomina[s] | macra[s]:
            numerus[n] = numerus.get(n, 0) + 1

    # caput: C89 limen + capita planata
    with open(CAPUT_EXITUS, 'w', encoding='utf-8', newline='\n') as o:
        o.write(PRAEFATIO.format('bearssl_amalgama.h'))
        o.write('/* capita: ' + ' '.join(sorted(capita)) + ' */\n')
        o.write('#if !defined(__STDC_VERSION__) '
                '|| __STDC_VERSION__ < 199901L\n'
                '#define inline __attribute__((unused))'
                ' /* <tolera codex="EXTENSIO_COMPILATORIS" (>limen C89 '
                '(tls-bearssl R2): inline -> unused; silva __attribute__ '
                'ut vacuum legit */\n'
                '#define BEARSSL_INLINE_C89\n#endif\n')
        o.write(caput)
        o.write('#ifdef BEARSSL_INLINE_C89\n#undef inline\n'
                '#undef BEARSSL_INLINE_C89\n#endif\n')

    # fons: inner.h (config.h inlineatum, bearssl.h -> caput generatum
    # eodem directorio) + fontes renominati
    interius = re.sub(r'^#include "config\.h"\n',
                      lambda m: configuratio, interius, flags=re.M)
    interius, n = re.subn(r'^#include "bearssl\.h"$',
                          '#include "bearssl_amalgama.h"', interius,
                          flags=re.M)
    if n != 1:
        fracta('inner.h: #include "bearssl.h" non semel (%d)' % n)
    renominata = 0
    with open(FONS_EXITUS, 'w', encoding='utf-8', newline='\n') as o:
        o.write(PRAEFATIO.format('bearssl_amalgama.c'))
        o.write('#line 1 "vendor/bearssl/src/inner.h"\n')
        o.write(interius)
        for s in fontes:
            basis = os.path.basename(s)[:-2]
            rn = sorted(n for n in nomina[s] if numerus[n] > 1)
            renominata += len(rn)
            for n in rn:
                o.write('#define %s %s__%s\n' % (n, n, basis))
            o.write('#line 1 "vendor/bearssl/src/%s"\n' % s)
            o.write(INCLUDE_INTERIUS.sub('', textus[s]))
            if not textus[s].endswith('\n'):
                o.write('\n')
            for n in rn:
                o.write('#undef %s\n' % n)
            for n in sorted(macra[s]):
                o.write('#undef %s\n' % n)
    sys.stderr.write('bearssl_amalgamare: fontes %d, renominata %d, '
                     'macra %d\n' % (len(fontes), renominata,
                                     sum(len(v) for v in macra.values())))


main()
