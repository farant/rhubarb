"""status_visionum.py - notae status in folium visionum operum.

Usus: python3 status_visionum.py <inventarium.tsv> <opera.html>

Inventarium = 'visiones operum (status)' in tabulario, exportatum per
'./gesta/frigida.sh -inventarium' (lineae: ordo, lens, genus, valor).
Folium curatum manu est; hoc instrumentum solum notas status et notas
'pivot' inserit. Idempotens: notae priores removentur ante novas.
"""
import html, re, sys
from collections import Counter

via_tsv, via_paginae = sys.argv[1], sys.argv[2]

cellae = {}
for linea in open(via_tsv):
    partes = linea.rstrip('\n').split('\t')
    if len(partes) == 4:
        cellae.setdefault(partes[0], {})[partes[1]] = partes[3].replace('\\t', '\t').replace('\\n', ' ')

NOMINA_STATUS = {'aedificatum': 'built', 'partim': 'partly built', 'nondum': 'not started'}
RE_ID = r'class="id" href="[a-z]+\.html">([A-Z][\d.]*)</a>'

pagina = open(via_paginae).read()
pagina = re.sub(r' <span class="statera[^"]*">.*?</span><!--/statera-->', '', pagina)


def nota_reddere(ordo):
    cella = cellae.get(ordo, {})
    status = cella.get('status')
    if status:
        signum = '<span class="status-mark s-%s">%s</span>' % (status, NOMINA_STATUS[status])
    else:
        signum = '<span class="status-mark s-ignotum">unknown</span>'
    nota = cella.get('nota')
    if nota:
        pivot = nota.startswith('PIVOT')
        nota = html.escape(nota)
        if pivot:
            nota = '<strong>pivot</strong>' + nota[len('PIVOT'):]
        signum += '<span class="nota-visio">%s</span>' % nota
    return ' <span class="statera">%s</span><!--/statera-->' % signum


lineae = []
for linea in pagina.split('\n'):
    m = re.search(RE_ID, linea)
    if m:
        if linea.endswith('</li>'):
            linea = linea[:-5] + nota_reddere(m.group(1)) + '</li>'
        else:
            linea = linea + nota_reddere(m.group(1))
    lineae.append(linea)
pagina = '\n'.join(lineae)

ordines = re.findall(RE_ID, pagina)
numeri = Counter(cellae.get(o, {}).get('status', 'ignotum') for o in ordines)
pivota = sum(1 for o in ordines if cellae.get(o, {}).get('nota', '').startswith('PIVOT'))
summa = ('<p class="nota statera-summa">Status from the ledger inventory <em>visiones operum (status)</em>: '
         '%d built, %d partly built, %d not started, %d unknown; %d with a pivot note.</p>' % (
             numeri['aedificatum'], numeri['partim'], numeri['nondum'], numeri['ignotum'], pivota))
pagina = re.sub(r'<p class="nota statera-summa">.*?</p>\n\n', '', pagina)
pagina = pagina.replace('<h2>Network</h2>', summa + '\n\n<h2>Network</h2>', 1)
open(via_paginae, 'w').write(pagina)
print(dict(numeri), 'pivot', pivota)
