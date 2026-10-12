#!/usr/bin/env python3
"""tools/credo_convertere.py - suitam VETEREM (principale + blocci cum
titulis '--- Probans T ---') in suitam SECTIONUM credo v2 vertit (credo II
T8; spec project-specs/credo-spec-2.md II).

STRUCTURA PER SILVAM, textus verbatim: sedes nodorum (principale, sententiae
summae corporis eius, blocci, tituli, declarationes, redde, usus
identificatorum) per selectores silvae (silva.selecta) inveniuntur; textus
inter eas lineis integris transfertur - commenta et forma intacta. Formae
DUAE mechanicae solae; omnis alia RECUSATUR (nihil scribitur, causa
nominata):

  A  titulus INTRA quemque bloccum summum: '{ imprimere("--- Probans T
     ---"); ... }' (exemplar probatio_xar)
  B  titulus ANTE sectionem: 'imprimere("--- Probans T ---"); ...' usque ad
     titulum proximum (exemplar probatio_stml); titulus intra bloccum
     sectionis -> CREDO_GRADUS("T")

Recusationes: statutum inter blocos (A) aut ante titulum primum; bloccus
sine titulo aut cum pluribus (A); redde aut salta intra sectionem; tituli
duplices; variabilis principalis (praeter piscina/praeteritus/intern) in
sectione adhibita = status communis (fixura suitae manu facienda); plus
CXCIX sectiones. Nomina functionum: regula PROPRIA '_sectio_<ordinalis>'
(prima, secunda, ... sexagesima_septima) - tituli verba Anglica ferebant
(lint Latinus XVI recusavit, adoptio II).

Usus (ex radice):
  ./tools/credo_convertere.py [-probare] [-ad VIA] [-suita T] VIA
    -probare  nihil scribit: convertibilis? (nota commissionis, lint 5)
    -ad VIA   exitus alibi (ordinarium: in loco)
    -suita T  titulus suitae (ordinarium: nomen plagulae sine 'probatio_')
Exitus: 0 conversa/convertibilis | 1 recusata (causa) | 2 non applicabilis
(iam conversa, sine principale) aut usus."""
import os
import re
import sys

RADIX = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(RADIX, 'pythonica'))
import silva  # noqa: E402

PRINCIPALE = 'definitio-functionis:definit(main)'
SUMMA = PRINCIPALE + ' > corpus > '
TITULUS = ('sententia-expressionis:vocat(printf)'
           ':habet(folium-chorda[tok_valor*="--- Probans"])')
CAUDA_VOCATA = ('credo_imprimere_compendium', 'credo_claudere',
                'piscina_destruere', 'credo_omnia_praeterierunt')
LOCALES_LICITAE = ('piscina', 'praeteritus', 'intern')
UNITATES = ('', 'prima', 'secunda', 'tertia', 'quarta', 'quinta', 'sexta',
            'septima', 'octava', 'nona')
DECADES = ('', 'decima', 'vicesima', 'tricesima', 'quadragesima',
           'quinquagesima', 'sexagesima', 'septuagesima', 'octogesima',
           'nonagesima')


class Recusatio(Exception):
    pass


class NonApplicabilis(Exception):
    pass


def ordinalis(n):
    """1 -> prima, 21 -> vicesima_prima, 100 -> centesima (CXCIX ad summum)"""
    if n >= 100:
        return 'centesima' + ('_' + ordinalis(n - 100) if n > 100 else '')
    if n < 10:
        return UNITATES[n]
    return DECADES[n // 10] + ('_' + UNITATES[n % 10] if n % 10 else '')


def sedes(via, selector):
    """[(linea_a, linea_b, textus)] - lineae ab I, inclusivae"""
    return [(s.linea_a, s.linea_b, s.textus)
            for s in silva.selecta(via, selector)]


def titulus_ex(textus):
    """'--- Probans T ---' ex litteris (adiacentes iunctae) - T crudum"""
    litterae = ''.join(re.findall(r'"((?:[^"\\]|\\.)*)"', textus))
    m = re.match(r'^(?:\\n)*--- Probans (.*?) ---(?:\\n)*$', litterae)
    return m.group(1) if m else None


def intra(linea, ambitus):
    return any(a <= linea <= b for a, b in ambitus)


def _commentum(linea, in_commento):
    """(est pars commenti, in_commento post lineam) - lineae triviae"""
    st = linea.strip()
    if in_commento:
        return True, '*/' not in linea
    if st.startswith('/*'):
        return True, '*/' not in linea
    return False, False


_licita = {}


def _cauda_licita(via, nodi_cauda):
    """nodi caudae ILLICITI (vacuum = omnes licitae): compendium,
    claudere, destruere, praeteritus, si (praeteritus), redde,
    imprimere("\\n") - copia semel per plagulam computata"""
    if via in _licita:
        return [n for n in nodi_cauda if (n[0], n[1]) not in _licita[via]]
    licita = set()
    for v in CAUDA_VOCATA:
        licita |= {(a, b) for a, b, _ in sedes(via, SUMMA + 'sententia-expressionis:vocat(%s)' % v)}
    licita |= {(a, b) for a, b, _ in sedes(via, SUMMA + 'si:utitur(praeteritus)')}
    licita |= {(a, b) for a, b, _ in sedes(via, SUMMA + 'redde')}
    for a, b, t in sedes(via, SUMMA + 'sententia-expressionis:vocat(printf)'):
        if re.match(r'^\s*imprimere\("\\n"\);\s*$', t):
            licita.add((a, b))
    _licita[via] = licita
    return [n for n in nodi_cauda if (n[0], n[1]) not in licita]


def _prologus_functionis(usus_intern, usus_piscinae):
    if usus_intern:
        return ['                Piscina* piscina;',
                '    InternamentumChorda* intern;',
                '',
                '    piscina = c->piscina;',
                '    intern  = internamentum_creare(piscina);',
                '    CREDO_NECESSE_NON_NIHIL(intern);']
    if usus_piscinae:
        return ['    Piscina* piscina;', '', '    piscina = c->piscina;']
    return ['    (vacuum)c;']


def convertere(via, suita):
    """reddit (textus_novus, forma, numerus_sectionum); Recusatio /
    NonApplicabilis"""
    textus = open(via).read()
    if 'credo_suitam_currere' in textus:
        raise NonApplicabilis('iam conversa (credo_suitam_currere adest)')
    lineae = textus.split('\n')
    pr = sedes(via, PRINCIPALE)
    if len(pr) != 1:
        raise NonApplicabilis('principale non inventum (%d)' % len(pr))
    pa, pb, _ = pr[0]
    nodi = sorted((a, b, t) for a, b, t in sedes(via, SUMMA + '*'))
    blocci = {(a, b) for a, b, _ in sedes(via, SUMMA + 'corpus')}
    tituli_summi = {(a, b): titulus_ex(t)
                    for a, b, t in sedes(via, SUMMA + TITULUS)}
    tituli_profundi = [(a, b, titulus_ex(t))
                       for a, b, t in sedes(via, SUMMA + 'corpus ' + TITULUS)]
    aperire = [b for a, b, _ in sedes(via, SUMMA + 'sententia-expressionis:vocat(credo_aperire)')]
    if len(aperire) != 1:
        raise Recusatio('credo_aperire in principale non semel (%d)'
                        % len(aperire))
    finis_prologi = aperire[0]
    post = [n for n in nodi if n[0] > finis_prologi]

    # locales principalis: nomina ex declarationibus (nodus silvae
    # declarationem delimitat; nomen = identificator ante ; = , [)
    locales = []
    for a, b, t in sedes(via, SUMMA + 'declaratio'):
        locales += re.findall(r'(\w+)\s*(?:\[[^\]]*\])?\s*(?:=[^,;]*)?[,;]', t)

    if tituli_summi:
        forma = 'B'
        primi = [n for n in post if (n[0], n[1]) in tituli_summi]
        ante = [n for n in post if n[0] < primi[0][0]]
        if ante:
            raise Recusatio('statutum ante titulum primum linea %d: %s'
                            % (ante[0][0], ante[0][2].strip().split('\n')[0]))
        # cauda = suffixum maximum nodorum licitorum post titulum ultimum
        post_ultimum = [n for n in post if n[0] > primi[-1][1]]
        k = len(post_ultimum)
        while k > 0 and not _cauda_licita(via, [post_ultimum[k - 1]]):
            k -= 1
        cauda = post_ultimum[k:]
        initium_caudae = cauda[0][0] if cauda else pb
        sectiones = []
        for i, n in enumerate(primi):
            finis = (primi[i + 1][0] - 1) if i + 1 < len(primi) \
                else initium_caudae - 1
            sectiones.append((tituli_summi[(n[0], n[1])], n[0], n[1], finis))
    elif tituli_profundi:
        forma = 'A'
        sectiones = []
        cauda_vista = False
        for n in post:
            if (n[0], n[1]) in blocci and not cauda_vista:
                directi_summi = [t for t in sedes_titulorum_bloccorum(via)
                                 if n[0] < t[0] and t[1] < n[1]]
                if len(directi_summi) != 1:
                    raise Recusatio('bloccus linea %d: tituli %d'
                                    % (n[0], len(directi_summi)))
                ta, tb, tt = directi_summi[0]
                sectiones.append((tt, ta, tb, n[1], n[0]))
                continue
            if not sectiones:
                raise Recusatio('statutum ante blocos linea %d: %s'
                                % (n[0], n[2].strip().split('\n')[0]))
            if _cauda_licita(via, [n]):
                raise Recusatio('statutum inter blocos linea %d: %s'
                                % (n[0], n[2].strip().split('\n')[0]))
            cauda_vista = True
        if not sectiones:
            raise Recusatio('nulla sectio')
    else:
        raise Recusatio("tituli '--- Probans T ---' desunt (forma nulla)")

    if len(sectiones) > 199:
        raise Recusatio('sectiones %d > CXCIX' % len(sectiones))
    tituli = [s[0] for s in sectiones]
    if None in tituli:
        raise Recusatio('titulus non legibilis')
    if len(set(tituli)) != len(tituli):
        raise Recusatio('tituli duplices')
    ambitus = ([(s[1], s[3]) for s in sectiones] if forma == 'B'
               else [(s[4], s[3]) for s in sectiones])
    for a, b, _ in sedes(via, SUMMA + '* redde') + sedes(via, SUMMA + '* salta'):
        if intra(a, ambitus):
            raise Recusatio('redde/salta intra sectionem linea %d' % a)
    usus = {}
    for titulus in set(locales) | set(LOCALES_LICITAE):
        usus[titulus] = [a for a, b, _ in sedes(
            via, SUMMA + '* folium-identificator[tok_valor="%s"]' % titulus)]
    for titulus in locales:
        if titulus in LOCALES_LICITAE:
            continue
        for a in usus[titulus]:
            if intra(a, ambitus):
                raise Recusatio("variabilis principalis '%s' in sectione"
                                " (linea %d) - status communis: fixura suitae"
                                " manu" % (titulus, a))

    def usus_in(titulus, a, b):
        return any(a <= x <= b for x in usus.get(titulus, []))

    out = []
    nomina = ['_sectio_' + ordinalis(i + 1) for i in range(len(sectiones))]
    if forma == 'A':
        vexillum = []
        in_com = False
        prior = finis_prologi
        for (t, ta, tb, bb, ba), nomen_f in zip(sectiones, nomina):
            vexillum = []
            in_com = False
            for l in lineae[prior:ba - 1]:
                pars, in_com = _commentum(l, in_com)
                if pars:
                    vexillum.append(l)
            if any(re.search(r'\*\s*Compendium\s*$', v) for v in vexillum):
                vexillum = []
            bloccus = lineae[ba - 1:bb]
            k = ta - ba
            del bloccus[k:k + (tb - ta + 1)]
            if k < len(bloccus) and bloccus[k].strip() == '':
                del bloccus[k]
            out += vexillum + [''] + _caput_functionis(nomen_f)
            out += _prologus_functionis(usus_in('intern', ba, bb),
                                        usus_in('piscina', ba, bb))
            out += bloccus + ['}', '']
            prior = bb
    else:
        primus = sectiones[0][1]
        vexillum = [l for l in lineae[finis_prologi:primus - 1] if l.strip()]
        for (t, ta, tb, fin), nomen_f in zip(sectiones, nomina):
            corpus = lineae[tb:fin]
            e = len(corpus)
            while e > 0 and (corpus[e - 1].strip() == ''
                             or corpus[e - 1].lstrip().startswith(('/*', '*'))):
                e -= 1
            proximum = [l for l in corpus[e:] if l.strip()]
            corpus = corpus[:e]
            while corpus and corpus[0].strip() == '':
                corpus = corpus[1:]
            # titulus intra bloccum sectionis -> CREDO_GRADUS (gradus)
            for pa_, pb_, pt in sorted(tituli_profundi, reverse=True):
                if tb < pa_ and pb_ <= fin:
                    i0 = _indicem_invenire(corpus, lineae[pa_ - 1])
                    if i0 >= 0:
                        spatia = re.match(r'^(\s*)', corpus[i0]).group(1)
                        corpus[i0:i0 + (pb_ - pa_ + 1)] = [
                            '%sCREDO_GRADUS("%s");' % (spatia, pt)]
            out += vexillum + [''] + _caput_functionis(nomen_f)
            out += _prologus_functionis(
                any(re.search(r'\bintern\b', x) for x in corpus)
                and usus_in('intern', ta, fin),
                any(re.search(r'\bpiscina\b', x) for x in corpus)
                and usus_in('piscina', ta, fin))
            out += corpus + ['}', '']
            vexillum = proximum
    out.append('hic_manens constans CredoSectio SECTIONES[] = {')
    for t, nomen_f in zip(tituli, nomina):
        out.append('    { "%s", %s, NIHIL, NIHIL, NIHIL, NIHIL },'
                   % (t, nomen_f))
    out += ['    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }', '}' + ';', '',
            's32', 'principale (vacuum)', '{',
            '    redde credo_suitam_currere("%s", SECTIONES);' % suita, '}']
    novus = '\n'.join(lineae[:pa - 1] + out + lineae[pb:])
    return novus, forma, len(sectiones)


def _caput_functionis(nomen_f):
    return ['interior vacuum', '%s (' % nomen_f, '    CredoContextus* c)',
            '{']


def _indicem_invenire(corpus, linea):
    for i, l in enumerate(corpus):
        if l == linea:
            return i
    return -1


_titulorum_bloccorum = {}


def sedes_titulorum_bloccorum(via):
    """tituli DIRECTI bloccorum summorum (forma A): corpus > corpus > T"""
    if via not in _titulorum_bloccorum:
        _titulorum_bloccorum[via] = [
            (a, b, titulus_ex(t))
            for a, b, t in sedes(via, SUMMA + 'corpus > ' + TITULUS)]
    return _titulorum_bloccorum[via]


def principale_(argv):
    probare, ad, suita, viae = False, None, None, []
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == '-probare':
            probare = True
        elif a in ('-ad', '-suita') and i + 1 < len(argv):
            if a == '-ad':
                ad = argv[i + 1]
            else:
                suita = argv[i + 1]
            i += 1
        elif a.startswith('-') or viae:
            print(__doc__.split('Usus')[1].split('Exitus')[0],
                  file=sys.stderr)
            return 2
        else:
            viae.append(a)
        i += 1
    if not viae:
        print('usus: credo_convertere.py [-probare] [-ad VIA] [-suita T]'
              ' VIA', file=sys.stderr)
        return 2
    via = viae[0]
    if suita is None:
        suita = re.sub(r'^probatio_', '', os.path.basename(via)
                       ).rsplit('.', 1)[0]
    try:
        novus, forma, n = convertere(via, suita)
    except NonApplicabilis as e:
        print('non applicabilis: %s - %s' % (via, e))
        return 2
    except Recusatio as e:
        print('recusata: %s - %s' % (via, e))
        return 1
    if probare:
        print('convertibilis: %s (forma %s, sectiones %d)' % (via, forma, n))
        return 0
    with open(ad or via, 'w') as f:
        f.write(novus)
    print('conversa: %s -> %s (forma %s, sectiones %d)'
          % (via, ad or via, forma, n))
    return 0


if __name__ == '__main__':
    sys.exit(principale_(sys.argv[1:]))
