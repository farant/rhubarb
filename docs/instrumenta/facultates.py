"""facultates.py - tabulae communes paginarum facultatum (briar, fabrica, ...).

Inventarium tabularii 'X facultates' (exportatum per ./gesta/frigida.sh
-inventarium EX ARBORE PRINCIPALI): ordo = facultas (slug), lentes area,
status, versio, descriptio, fons. Hic: lectio TSV, iudicium valorum
(ignotum = RECUSATIO), et tres tabulae (per aream, statum, versionem)
ordinatae sine JavaScript (input.ordo + :checked in stilus.css).
"""
import html, re, sys
from collections import Counter

STATUS = [
    ('aedificatum', 'built', 'Built and gated: a test, a plant or a smoke stage exercises it.'),
    ('partim', 'partial', 'Built in part; the missing part is a named limit or work in progress.'),
    ('dilatum', 'deferred', 'Named in a spec as deliberately not built yet, with its reason.'),
    ('refutatum', 'refused', 'Decided against by decree: written down so nobody builds it by accident.'),
]


def legere(via_tsv):
    ordines, cellae = [], {}
    for linea in open(via_tsv):
        p = linea.rstrip('\n').split('\t')
        if len(p) < 4:
            continue
        if p[0] not in cellae:
            ordines.append(p[0])
            cellae[p[0]] = {}
        cellae[p[0]][p[1]] = p[3]
    return ordines, cellae


def iudicare(nomen, ordines, cellae, areae, versiones):
    status_noti = [s for s, _, _ in STATUS]
    for o in ordines:
        c = cellae[o]
        if c.get('area') not in areae or c.get('status') not in status_noti or c.get('versio') not in versiones:
            sys.exit('%s: ordo %s: area/status/versio ignota (%r) - recuso' % (nomen, o, c))


def codex_involvere(signa):
    """Functio textus: effugit et signa (regex signa, super textum effugitum) in <code> involvit."""
    r = re.compile(signa)
    return lambda t: r.sub(lambda m: '<code>%s</code>' % m.group(1), html.escape(t, quote=False))


def tabulae(ordines, cellae, AREAE, VERSIONES, TITULI_VERSIONUM, textus):
    """Reddit (tabulae_html, status_html, index_arearum, numeri)."""
    areae = dict(AREAE)

    def linea(o):
        c = cellae[o]
        return ('<tr><td><code>%s</code></td><td><span class="status-%s">%s</span></td><td>%s</td>'
                '<td>%s<br><span class="nota">%s · %s</span></td></tr>') % (
            o, c['status'], c['status'], html.escape(c['versio']), textus(c['descriptio']),
            areae[c['area']], html.escape(c['fons']))

    def grex(titulus, membra):
        return (['<tr class="grex-gradus"><td colspan="4">%s <span class="nota">%d</span></td></tr>' % (titulus, len(membra))]
                + [linea(o) for o in membra])

    per_aream, per_statum, per_versionem = [], [], []
    for k, t in AREAE:
        m = [o for o in ordines if cellae[o]['area'] == k]
        if m:
            per_aream += grex(t, m)
    for k, angl, _ in STATUS:
        m = [o for o in ordines if cellae[o]['status'] == k]
        if m:
            per_statum += grex('<span class="status-%s">%s</span> %s' % (k, k, angl), m)
    for v in VERSIONES:
        m = [o for o in ordines if cellae[o]['versio'] == v]
        if m:
            per_versionem += grex(TITULI_VERSIONUM[v], m)

    def tabula(clavis, lineae):
        sig = lambda k: ' &#9650;' if k == clavis else ''
        return ('<table class="facultates ordo-%s">\n<thead><tr>'
                '<th><label for="ordo-area" class="ordinator">Feature%s</label></th>'
                '<th><label for="ordo-status" class="ordinator">Status%s</label></th>'
                '<th><label for="ordo-versio" class="ordinator">Since%s</label></th>'
                '<th>What it does</th></tr></thead>\n<tbody>\n%s\n</tbody>\n</table>') % (
            clavis, sig('area'), sig('status'), sig('versio'), '\n'.join(lineae))

    tabulae_html = ('<input type="radio" name="ordo-facultatum" id="ordo-area" class="ordo" checked>\n'
                    '<input type="radio" name="ordo-facultatum" id="ordo-status" class="ordo">\n'
                    '<input type="radio" name="ordo-facultatum" id="ordo-versio" class="ordo">\n\n'
                    + tabula('area', per_aream) + '\n\n' + tabula('status', per_statum) + '\n\n'
                    + tabula('versio', per_versionem))
    numeri = Counter(cellae[o]['status'] for o in ordines)
    status_html = '\n'.join('<tr><td><span class="status-%s">%s</span></td><td>%s</td><td>%d</td><td>%s</td></tr>' % (
        k, k, angl, numeri[k], html.escape(d)) for k, angl, d in STATUS if numeri[k])
    index_arearum = ' · '.join('%s <span class="nota">%d</span>' % (t, sum(1 for o in ordines if cellae[o]['area'] == k))
                               for k, t in AREAE)
    return tabulae_html, status_html, index_arearum, numeri


def compendium(nomen, ordines, numeri):
    return '%s: %d facultates (%s)' % (nomen, len(ordines), ', '.join('%s %d' % (k, numeri[k]) for k, _, _ in STATUS if numeri[k]))
