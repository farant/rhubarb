#!/usr/bin/env python3
"""tools/credo_convertere_sectiones.py - suitam credo v1 formae 'vexillum +
bloccus' in sectiones v2 vertit (migratio subsystematum, gradus II: css,
toml, saltuarius; ledger 01M5B3BT9R71). Frater credo_convertere.py: ille
formas A/B mechanicas solas accipit et praeludium ante blocos recusat;
hic praeludium usitatum tractat.

FORMA ACCEPTA: in principale post praeludium blocci '    {' ... '    }'
quisque post vexillum suum ('    /* ====' ... ' * ==== */'). Bloccus
quisque sectio fit, titulo et nomine functionis datis. Intra bloccum:
  - titulus primus 'imprimere("\\n--- X ---\\n")' tollitur (credo
    titulum sectionis imprimit; examen 95), sequentes -> CREDO_GRADUS
  - '&ratum' -> monstrator constans in fixuram
  - 'c' localis (umbra parametri) -> 'compositus' (chordae intactae)
  - declarationes sectionis ex usu: ratum, piscina (= c->piscina),
    opus, radix (RHUBARB_RADIX), nexus; nihil horum -> (vacuum)c

OPTIONES:
  LEXICON (argumentum tertium): e.g. TOML_LEXICON - lineae
    'CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, ...))' (et
    assertio iudicii sequens) ex praeludio aut bloco in parare_suitae
    moventur; fixura FixuraLexici. '-' = sine fixura.
  -opus=TITULUS,MENSURA: piscina 'opus' per sectionem propria
    (creata et destructa intra sectionem) ubi bloccus 'opus' adhibet.
  -nexus: sectiones quae 'nexus' adhibent nexum saltuarii suum creant;
    parare_suitae creationem semel asserit (CREDO_NON_NIHIL).

QUAE NON TRACTAT (manu, vide lib/credo.worklog.md 2026-10-21):
  status inter blocos (numeratores, statica scripta, fixa disci),
  assertiones aggregatae trans blocos, mundus mutabilis praeter nexum,
  blocci ex prioribus pendentes. Scaenica: credo_convertere_scaenam.py.

POST CONVERSIONEM SEMPER: ./silva/formator.sh -scribere VIA, deinde
./tools/credo_conversionem_probare.py VIA (assertiones et Totalis ==
HEAD, ordo mixtus) et VOCABULA_VIAE_ADDITAE=VIA ./oratio/vocabula.sh
-nova (nomina functionum Latina).

Usus (ex radice):
  ./tools/credo_convertere_sectiones.py VIA SUITA LEXICON|- [-opus=T,N]
      [-nexus] 'functio|titulus' ...   (par unum per bloccum, ordine)
Exitus: 0 conversa | 1 recusata (causa nominata, nihil scriptum) | 2 usus
"""
import re
import sys

VEXILLUM_BLOCCUS = re.compile(
    r'\n(    /\* =+\n(?:     \*[^\n]*\n)*?     \* =+ \*/)\n\n(    \{\n.*?\n    \})(?=\n)',
    re.S)
RATUM_FACERE = re.compile(
    r'\n *CREDO_VERUM \(materia_lexicon_ratum_facere\(&ratum, &[A-Z_]+,\n? *&iudicium\)\);\n'
    r'(?: *CREDO_AEQUALIS_S32 \(iudicium\.vitium, \(s32\)MATERIA_LEX_SANUM\);\n)?')
TITULUS = re.compile(r'\n( *)imprimere\("\\n--- (.*?) ---\\n"\);\n')
CHORDA = re.compile(r'("(?:[^"\\]|\\.)*")')


class Recusatio(Exception):
    pass


def _dedentare(textus):
    return '\n'.join(l[4:] if l.startswith('    ') else l
                     for l in textus.split('\n'))


def _vexillum(textus):
    textus = _dedentare(textus)
    textus = re.sub(r'^/\* =+$', '/* ' + '=' * 50, textus, flags=re.M)
    return re.sub(r'^ \* =+ \*/$', ' * ' + '=' * 50 + ' */', textus,
                  flags=re.M)


def _sine_commentis(textus):
    return re.sub(r'/\*.*?\*/', '', textus, flags=re.S)


def _sine_chordis(textus):
    return CHORDA.sub('""', _sine_commentis(textus))


def convertere(via, suita, lexicon, opus_titulus, opus_mensura,
               nexus_mundus, paria):
    fons = open(via).read()
    if 'credo_suitam_currere' in fons:
        raise Recusatio('iam v2')
    if 'principale (vacuum)' not in fons:
        raise Recusatio('principale absens')
    i = fons.index('principale (vacuum)')
    i = fons.rindex('\n', 0, i - 1) + 1
    caput = re.sub(r'\n(s32|integer)\n$', '\n', fons[:i])
    corpus = fons[i:]
    blocci = VEXILLUM_BLOCCUS.findall(corpus)
    if len(blocci) != len(paria):
        raise Recusatio('blocci %d, paria data %d' % (len(blocci), len(paria)))
    exitus = [caput.rstrip('\n')]

    if lexicon != '-':
        motae = ''
        for k, (vex, bloccus) in enumerate(blocci):
            m = RATUM_FACERE.search(bloccus)
            if m:
                motae = m.group(0)
                blocci[k] = (vex, bloccus.replace(m.group(0), '\n', 1))
        if not motae:
            m = RATUM_FACERE.search(corpus[:corpus.index('/* ====')])
            if not m:
                raise Recusatio('ratum_facere non inventum (LEXICON datum)')
            motae = m.group(0)
        motae = motae.replace('&ratum', '&f->ratum').strip('\n')
        motae = '\n'.join(('    ' if l.strip().startswith('CREDO') else '        ')
                          + l.strip() for l in motae.split('\n'))
        exitus.append('''

/* FIXURA SUITAE: ratum lexici semel factum; sectiones id per
 * monstratorem constantem LEGUNT. */
nomen structura {
    MateriaLexiconRatum ratum;
} FixuraLexici;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
          FixuraLexici* f;
    MateriaLexIudicium  iudicium;

    f = (FixuraLexici*)piscina_allocare(c->piscina,
        magnitudo(FixuraLexici));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
%s
    c->datum = f;
}''' % motae)
    elif nexus_mundus:
        exitus.append('''

/* creatio nexus semel asserta (ut olim ante blocos); sectiones nexum
 * suum quaeque creant - status nexus inter sectiones non communis */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    CREDO_NON_NIHIL (saltuarius_nexus_creare(c->piscina));
}''')

    for (vex, bloccus), (functio, _titulus) in zip(blocci, paria):
        primus = [True]

        def _titulum(m):
            if primus[0]:
                primus[0] = False
                return '\n'
            return '\n%sCREDO_GRADUS ("%s");\n' % (
                m.group(1), m.group(2).replace('Probans ', ''))
        bloccus = TITULUS.sub(_titulum, bloccus)
        bloccus = re.sub(r'\{\n\n', '{\n', bloccus, count=1)
        bloccus = bloccus.replace('&ratum', 'ratum')
        if re.search(r'(?<![\w>.])c\b(?!\s*\()', _sine_chordis(bloccus)):
            partes = CHORDA.split(bloccus)
            bloccus = ''.join(
                x if x.startswith('"') else re.sub(
                    r'(?<![\w>.\'])\bc\b(?=\s*[-;,)=.\[]|->)', 'compositus', x)
                for x in partes)
        nuda = _sine_commentis(bloccus)
        utitur_ratum = re.search(r'\bratum\b', nuda) is not None
        utitur_piscina = re.search(r'\bpiscina\b', nuda) is not None
        utitur_nexus = nexus_mundus and re.search(r'\bnexus\b', nuda) is not None
        utitur_opus = opus_titulus is not None and re.search(r'\bopus\b', nuda) is not None
        utitur_radix = (re.search(r'\bradix\b', nuda) is not None
                        and 'radix =' not in nuda and 'radix=' not in nuda)
        declarationes, custodiae = [], ''
        if utitur_ratum:
            declarationes.append('    constans MateriaLexiconRatum* ratum;')
            custodiae = '''
    si (c->suita == NIHIL)
    {
        redde;
    }
    ratum = &((constans FixuraLexici*)c->suita)->ratum;'''
        if utitur_piscina:
            declarationes.append(('                         ' if utitur_ratum else '    ')
                                 + 'Piscina* piscina = c->piscina;')
        if utitur_nexus:
            declarationes.append('    SaltuariusNexus* nexus;')
            custodiae += '''
    nexus = saltuarius_nexus_creare(%s);
    si (nexus == NIHIL)
    {
        redde;   /* creatio in parare_suitae asserta */
    }''' % ('piscina' if utitur_piscina else 'c->piscina')
        if utitur_opus:
            declarationes.append('    Piscina* opus;')
        if utitur_radix:
            declarationes.append(
                '    constans character* radix = lectiones_ambitus("RHUBARB_RADIX");')
            custodiae += '''
    si (radix == NIHIL)
    {
        radix = ".";
    }'''
        if utitur_opus:
            custodiae += '''
    opus = piscina_generare_dynamicum("%s", %s);
    si (opus == NIHIL)
    {
        redde;
    }''' % (opus_titulus, opus_mensura)
            bloccus = bloccus + '\n    piscina_destruere(opus);'
        if not (utitur_piscina or utitur_ratum
                or re.search(r'(?<![\w>.])c\b', _sine_commentis(bloccus))):
            declarationes.append('    (vacuum)c;')
        exitus.append('\n\n' + _vexillum(vex) + '''

interior vacuum
%s (
    CredoContextus* c)
{
%s
%s
%s
}''' % (functio, '\n'.join(declarationes), custodiae, bloccus))

    exitus.append('\nhic_manens constans CredoSectio SECTIONES[] = {')
    for functio, titulus in paria:
        exitus.append('    { "%s", %s,\n        NIHIL, NIHIL, NIHIL, NIHIL },'
                      % (titulus, functio))
    cum_paratione = lexicon != '-' or nexus_mundus
    exitus.append('''    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde %s;
}
''' % ('credo_suitam_currere_cum("%s", SECTIONES,\n        _suitam_parare, NIHIL)' % suita
       if cum_paratione else 'credo_suitam_currere("%s", SECTIONES)' % suita))
    open(via, 'w').write('\n'.join(exitus))
    return len(paria)


def main(argumenta):
    if len(argumenta) < 4:
        sys.stderr.write(__doc__)
        return 2
    via, suita, lexicon = argumenta[0], argumenta[1], argumenta[2]
    reliqua = argumenta[3:]
    opus_titulus, opus_mensura, nexus_mundus = None, None, False
    while reliqua and reliqua[0].startswith('-'):
        if reliqua[0].startswith('-opus='):
            opus_titulus, opus_mensura = reliqua[0][6:].split(',')
        elif reliqua[0] == '-nexus':
            nexus_mundus = True
        else:
            sys.stderr.write('optio ignota: %s\n' % reliqua[0])
            return 2
        reliqua = reliqua[1:]
    paria = []
    for a in reliqua:
        if '|' not in a:
            sys.stderr.write('par sine "|": %s\n' % a)
            return 2
        paria.append(a.split('|', 1))
    try:
        n = convertere(via, suita, lexicon, opus_titulus, opus_mensura,
                       nexus_mundus, paria)
    except Recusatio as e:
        print('recusata: %s - %s' % (via, e))
        return 1
    print('conversa: %s (%d sectiones%s)' % (
        via, n, ', fixura' if lexicon != '-' else ''))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
