#!/usr/bin/env python3
# tools/musicxml_oraculum.py - oraculum music21: partitura -> tabula canonica
#
# AEDIFICATOR AURI SEMEL (spec musicxml M11, Franus): music21 in venv
# abiciendo (build/musicxml/oraculum/) currit, tabulas scribit; nos
# discordantias manu recensemus et aurum NOSTRUM committimus. Nulla
# porta, nulla probatio hoc vocat. Usus manualis et rarus:
#
#   python3 -m venv build/musicxml/oraculum
#   build/musicxml/oraculum/bin/python -I -m pip install music21==10.5.2
#   build/musicxml/oraculum/bin/python -I tools/musicxml_oraculum.py \
#       EXITUS_DIR via.musicxml [via.mxl ...]
#
# Forma = tabula lectoris (musicxml_tabulam_scribere, plan P2):
#   DATA musicxml tabula 1
#   # pars initium longitudo midi littera alteratio gratia versus
# una linea per notam sonantem post ligaturas iunctas, ordine
# exsecutionis (expandRepeats), initium et longitudo in quartis ut
# fractiones reductae, altitudo SONANS (toSoundingPitch), lineae
# ordinatae (pars, initium, midi). Linea temporis communis (M15):
# initium mensurae k = summa maximarum longitudinum mensurarum
# priorum inter partes. Fallit -> '# vitium oraculi: ...' et linea
# nulla. Fasciculi externi = data non fida: via ut argumentum, -I.

import sys
import os
from fractions import Fraction


def quartae(x):
    f = Fraction(x).limit_denominator(1 << 20)
    if f.denominator == 1:
        return str(f.numerator)
    return '%d/%d' % (f.numerator, f.denominator)


def alteratio(p):
    a = p.accidental.alter if p.accidental is not None else 0.0
    if float(a) == int(a):
        return str(int(a))
    s = ('%.4f' % a).rstrip('0').rstrip('.')
    return s


def clavis_ordinis(pars, ortus, midi, dur, gratia, versus_textus):
    # ordo canonicus = lector (_lineas_conferre): pars, initium, midi,
    # longitudo, gratia, numerus syllabarum, textus syllabae primae
    syl = [] if versus_textus == '-' else versus_textus.split('|')
    prima = syl[0].split(':', 2)[2].encode('utf-8') if syl else b''
    return (pars, ortus, midi, dur, gratia, len(syl), prima)


def versus(n):
    partes = []
    for ly in sorted(n.lyrics, key=lambda y: (y.number or 0)):
        comps = ly.components if getattr(ly, 'isComposite', False) else [ly]
        for y in comps:
            text = (y.text or '').replace('\t', ' ').replace('\n', ' ') \
                .replace('\r', ' ').replace('|', ' ')
            syll = y.syllabic or 'single'
            if syll == 'composite':
                syll = 'single'
            partes.append('%d:%s:%s' % (ly.number or 1, syll, text))
    return '|'.join(partes) if partes else '-'


def tabula(via):
    import music21
    from music21 import note, chord, harmony
    s = music21.converter.parse(via, forceSource=True)
    try:
        s = s.toSoundingPitch()
    except Exception:
        pass
    try:
        s = s.expandRepeats()
    except Exception as e:
        print('# monitum oraculi: expandRepeats: %s' % e, file=sys.stderr)
    partes = list(s.parts) if hasattr(s, 'parts') and len(s.parts) else [s]
    mensurae = [list(p.getElementsByClass('Measure')) for p in partes]
    n = max((len(m) for m in mensurae), default=0)
    longitudines = [Fraction(0)] * n
    for ms in mensurae:
        for k, m in enumerate(ms):
            longitudines[k] = max(longitudines[k],
                                  Fraction(m.highestTime).limit_denominator(1 << 20))
    initia = []
    acc = Fraction(0)
    for k in range(n):
        initia.append(acc)
        acc += longitudines[k]
    lineae = []
    ordo_partium = {}
    for pi, (p, ms) in enumerate(zip(partes, mensurae)):
        # id MusicXML: music21 id partis mutat (nomen instrumenti) et
        # partes plurium baculorum in PartStaff '<id>-StaffN' scindit;
        # id verum in p.groups servat - baculi in partem unam iunguntur
        pid = None
        for g in getattr(p, 'groups', []):
            if isinstance(g, str) and g:
                pid = g
                break
        if pid is None:
            pid = p.id if isinstance(p.id, str) else 'P%d' % (pi + 1)
        if '-Staff' in pid:
            pid = pid[:pid.index('-Staff')]
        ordo_partium.setdefault(pid, len(ordo_partium))
        apertae = []   # indices linearum cum ligatura aperta
        for k, m in enumerate(ms):
            for el in m.recurse().notes:
                # symbola acordorum (<harmony>) non sunt notae
                if isinstance(el, harmony.Harmony):
                    continue
                ortus = initia[k] + Fraction(el.getOffsetInHierarchy(m)).limit_denominator(1 << 20)
                gratia = el.duration.isGrace
                dur = Fraction(0) if gratia else \
                    Fraction(el.duration.quarterLength).limit_denominator(1 << 20)
                if isinstance(el, chord.Chord):
                    # verba in nota prima acordi solum (ut lector noster)
                    soni = [(pp, el if i == 0 else None)
                            for i, pp in enumerate(el.pitches)]
                elif isinstance(el, note.Note):
                    soni = [(el.pitch, el)]
                elif isinstance(el, note.Unpitched):
                    dp = el.displayPitch()
                    soni = [(dp, el)]
                else:
                    continue
                for pp, src in soni:
                    # ligatura PER SONUM: acordum ligaturam suam per notam
                    # habet (el.tie acordi totius membra non ligata
                    # falso iungebat - corpus silvestre 2026-10-31)
                    if isinstance(el, chord.Chord):
                        tt = el.getTie(pp)
                    else:
                        tt = el.tie
                    tie = tt.type if tt is not None else None
                    midi = pp.midi
                    if tie in ('stop', 'continue'):
                        found = None
                        for i in apertae:
                            L = lineae[i]
                            if L['midi'] == midi and L['ortus'] + L['dur'] == ortus:
                                found = i
                                break
                        if found is not None:
                            lineae[found]['dur'] += dur
                            if tie == 'stop':
                                apertae.remove(found)
                            continue
                    lineae.append({'pars': ordo_partium[pid], 'pid': pid,
                                   'ortus': ortus,
                                   'dur': dur, 'midi': midi,
                                   'littera': pp.step,
                                   'alt': alteratio(pp),
                                   'gratia': 1 if gratia else 0,
                                   'versus': versus(src) if src is not None else '-'})
                    if tie in ('start', 'continue'):
                        apertae.append(len(lineae) - 1)
    lineae.sort(key=lambda L: clavis_ordinis(L['pars'], L['ortus'], L['midi'],
                                             L['dur'], L['gratia'], L['versus']))
    out = ['DATA musicxml tabula 1',
           '# pars\tinitium\tlongitudo\tmidi\tlittera\talteratio\tgratia\tversus']
    for L in lineae:
        out.append('\t'.join([L['pid'], quartae(L['ortus']), quartae(L['dur']),
                              str(L['midi']), L['littera'], L['alt'],
                              str(L['gratia']), L['versus']]))
    return '\n'.join(out) + '\n'


def main():
    if len(sys.argv) < 3:
        print(__doc__ or 'usus: musicxml_oraculum.py EXITUS_DIR via...', file=sys.stderr)
        return 2
    exitus = sys.argv[1]
    os.makedirs(exitus, exist_ok=True)
    mala = 0
    for via in sys.argv[2:]:
        nomen_fasciculi = os.path.basename(via) + '.tsv'
        # iam scriptum: praetermittitur (cursus a custode occisus pergit)
        if os.path.exists(os.path.join(exitus, nomen_fasciculi)):
            continue
        try:
            t = tabula(via)
        except Exception as e:
            mala += 1
            t = 'DATA musicxml tabula 1\n# vitium oraculi: %s: %s\n' % (
                type(e).__name__, str(e).replace('\n', ' ')[:200])
        with open(os.path.join(exitus, nomen_fasciculi), 'w') as f:
            f.write(t)
    print('ORACULUM: %d fasciculi, %d fracti' % (len(sys.argv) - 2, mala))
    return 0


if __name__ == '__main__':
    sys.exit(main())
