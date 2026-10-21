#!/usr/bin/env python3
"""tools/credo_convertere_scaenam.py - suitam credo v1 SCAENICAM in
sectionem v2 UNAM vertit (migratio subsystematum, gradus II: saltuarius
quaestio, claves, visum, columnae, limes, structura; ledger
01M5B3BT9R71).

SCAENICA: blocci statum priorum EX PROPOSITO continuant (sessio
interactiva - quaestio commissa in bloco I, n/N in bloco II). Divisio
sectiones per replay sine assertione postularet - prolixum et fragile;
sectio una fidelis est, ordine trivialiter libera.

Quod facit: corpus principale -> sectio '_scaenam_probare'; piscina =
c->piscina (si adhibetur, aliter (vacuum)c); creatio piscinae,
credo_aperire, cauda (compendium, destructio, redde) tolluntur; tituli
'--- X ---' (etiam bilineares) -> CREDO_GRADUS ("X") ut fractura
gradum nominet; 'redde I;' (custodiae praeparationis) -> 'redde;'
(sectio VACUA, suita FRACTA - sine assertione nova).

Ubi blocci INDEPENDENTES sunt, credo_convertere_sectiones.py praefer
(fractura per sectionem, ordo mixtus aliquid probat).

POST CONVERSIONEM SEMPER: ./silva/formator.sh -scribere VIA, deinde
./tools/credo_conversionem_probare.py VIA et VOCABULA_VIAE_ADDITAE=VIA
./oratio/vocabula.sh -nova.

RECUSAT (exitus I, nihil scriptum): cauda post compendium aliud quam
compendium/destructionem/redde continens (_purgare() frigidae: in
purgare transfer manu); 'c' localis parametrum umbrans sine -c=NOMEN.

Usus (ex radice):
  ./tools/credo_convertere_scaenam.py [-c=NOMEN] VIA SUITA TITULUS_SECTIONIS
Exitus: 0 conversa | 1 recusata (causa nominata, nihil scriptum) | 2 usus
"""
import re
import sys

TITULUS = re.compile(
    r'\n( *)imprimere\("\\n--- ((?:[^"\\]|\\.)*?)"((?:\s*"(?:[^"\\]|\\.)*")*)\);\n')


CHORDA = re.compile(r'("(?:[^"\\]|\\.)*")')
CAUDA_NOTA = [
    r'^\s*$', r'^\s*imprimere\("\\n"\);$', r'^\s*credo_imprimere_compendium\(\);$',
    r'^\s*(b32\s+)?praeteritus = credo_omnia_praeterierunt\(\);$',
    r'^\s*piscina_destruere\(piscina\);$', r'^\s*redde praeteritus \? ZEPHYRUM : I;$',
    r'^\s*si \(praeteritus\)$', r'^\s*alioquin$', r'^\s*[{}]$',
    r'^\s*redde (ZEPHYRUM|I);$',
]


class Recusatio(Exception):
    pass


def _gradus(m):
    """titulus solum si textus iunctus '---\\n' finit; aliter impressio
    datorum (e.g. '\\n--- intrati:' ante ansam numerorum, tessera modi)
    intacta manet - output tacite mutatum alioquin"""
    pars = m.group(2) + ''.join(re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(3)))
    if not re.search(r'---\\n$', pars) or '%' in pars:
        return m.group(0)
    pars = re.sub(r'\s*---\\n$', '', pars).replace('Probans ', '')
    return '\n%sCREDO_GRADUS ("%s");\n' % (m.group(1), pars)


def convertere(via, suita, titulus, c_novum=None):
    fons = open(via).read()
    if 'credo_suitam_currere' in fons:
        raise Recusatio('iam v2')
    if 'principale (vacuum)' not in fons:
        raise Recusatio('principale absens')
    i = fons.index('principale (vacuum)')
    i = fons.rindex('\n', 0, i - 1) + 1
    caput = re.sub(r'\n(s32|integer)\n$', '\n', fons[:i])
    corpus = fons[i:]
    interius = corpus[corpus.index('{\n') + 2:]
    m = re.match(r'((?:[^\n]*\n)*?)\n', interius)
    if not m:
        raise Recusatio('declarationes principalis non separatae')
    declarationes = '\n'.join(
        l for l in m.group(1).split('\n')
        if not re.search(r'\bpraeteritus\b|Piscina\*\s+piscina;', l))
    reliquum = interius[m.end():]
    reliquum = re.sub(r'    piscina = piscina_generare_dynamicum\([^;]*\);\n', '',
                      reliquum, flags=re.S)
    # custodia piscinae, quaevis forma nuntii ('FRACTA: piscina...')
    reliquum = re.sub(
        r'    si \(!piscina\)\n    \{\n        imprimere\("FRACTA: piscina[^"]*"\);\n'
        r'        redde I;\n    \}\n', '', reliquum)
    reliquum = reliquum.replace('    credo_aperire(piscina);\n', '')
    cauda = re.search(r'\n *(imprimere\("\\n"\);\n *)?credo_imprimere_compendium\(\);.*$',
                      reliquum, re.S)
    if not cauda:
        raise Recusatio('cauda (credo_imprimere_compendium) non inventa')
    reliqua_caudae = [l for l in re.sub(r'/\*.*?\*/', '', reliquum[cauda.start():],
                                         flags=re.S).split('\n')
                      if not any(re.match(n, l) for n in CAUDA_NOTA)]
    if reliqua_caudae:
        raise Recusatio('cauda post compendium continet (manu: purgare sectionis '
                        'aut suitae): %s' % ' | '.join(l.strip() for l in reliqua_caudae[:4]))
    reliquum = reliquum[:cauda.start()] + '\n'
    reliquum = TITULUS.sub(_gradus, reliquum)
    if re.search(r'imprimere\("\\\\n--- [^"%]*---\\\\n"\);', reliquum):
        raise Recusatio('titulus formae ignotae relictus')
    sine_chordis = CHORDA.sub('""', re.sub(r'/\*.*?\*/', '', reliquum + declarationes,
                                         flags=re.S))
    if re.search(r'(?<![\w>.])c\b(?!\s*\()', sine_chordis):
        if c_novum is None:
            raise Recusatio("localis 'c' parametrum sectionis umbrat - da -c=NOMEN "
                            "(nomen typo aptum)")
        def _renominare(textus):
            return ''.join(x if x.startswith('"') else re.sub(
                r'(?<![\w>.\'])\bc\b(?=\s*[-;,)=.\[]|->)', c_novum, x)
                for x in CHORDA.split(textus))
        reliquum = _renominare(reliquum)
        declarationes = _renominare(declarationes)
    nuda = re.sub(r'/\*.*?\*/', '', reliquum + declarationes, flags=re.S)
    utitur_piscina = re.search(r'\bpiscina\b', nuda) is not None
    piscina = '            Piscina* piscina = c->piscina;' if utitur_piscina else ''
    reliquum = re.sub(r'\bredde I;', 'redde;   /* sine assertione: sectio VACUA */',
                      reliquum)
    reliquum = '\n' + reliquum.strip('\n') + '\n'
    if not utitur_piscina:
        reliquum = '\n    (vacuum)c;' + reliquum
    novum = caput.rstrip('\n') + '''

/* Sectio UNA: probatio scaenica - blocci statum priorum ex proposito
 * continuant; gradus (CREDO_GRADUS) fracturam nominant. */
interior vacuum
_scaenam_probare (
    CredoContextus* c)
{
%s
%s
%s}

hic_manens constans CredoSectio SECTIONES[] = {
    { "%s", _scaenam_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("%s", SECTIONES);
}
''' % (piscina, declarationes.rstrip('\n'), reliquum, titulus, suita)
    novum = re.sub(r'(CredoContextus\* c\)\n\{\n)\n', r'\1', novum)
    open(via, 'w').write(novum)


def main(argumenta):
    c_novum = None
    if argumenta and argumenta[0].startswith('-c='):
        c_novum = argumenta[0][3:]
        argumenta = argumenta[1:]
    if len(argumenta) != 3:
        sys.stderr.write(__doc__)
        return 2
    try:
        convertere(argumenta[0], argumenta[1], argumenta[2], c_novum)
    except Recusatio as e:
        print('recusata: %s - %s' % (argumenta[0], e))
        return 1
    print('conversa (sectio una): %s' % argumenta[0])
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
