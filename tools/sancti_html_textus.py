"""tools/sancti_html_textus.py - HTML (paginae Wikisource renderata) in
textum planum pro speculo fontium sanctorum (tools/sancti_fontes_capere.sh).
Data non fida: solum parsatur (html.parser), nihil exsequitur.

Usus: python3 -I tools/sancti_html_textus.py INTROITUS.json EXITUS.txt
(INTROITUS = responsum api.php?action=parse&prop=text&format=json)"""
import html.parser
import json
import sys

OMITTE = {'script', 'style', 'sup', 'table'}
CLAUSULAE = {'p', 'div', 'br', 'li', 'h1', 'h2', 'h3', 'h4', 'tr'}


class Lector(html.parser.HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.partes = []
        self.profunditas = 0

    def handle_starttag(self, tag, attrs):
        classes = dict(attrs).get('class') or ''
        if tag in OMITTE or 'mw-editsection' in classes \
                or 'reference' in classes or 'ws-noexport' in classes:
            self.profunditas += 1 if tag not in ('br',) else 0
            if tag in OMITTE or self.profunditas:
                return
        if tag in CLAUSULAE:
            self.partes.append('\n')

    def handle_endtag(self, tag):
        if self.profunditas and tag in OMITTE | {'span', 'div'}:
            self.profunditas -= 1
            return
        if tag in CLAUSULAE:
            self.partes.append('\n')

    def handle_data(self, data):
        if not self.profunditas:
            self.partes.append(data)


def textus_ex_html(fons):
    lector = Lector()
    lector.feed(fons)
    lineae = [' '.join(l.split()) for l in ''.join(lector.partes).split('\n')]
    return '\n'.join(l for l in lineae if l)


def main():
    with open(sys.argv[1], encoding='utf-8') as f:
        d = json.load(f)
    if 'parse' not in d:
        sys.stderr.write('sancti_html_textus: responsum sine parse\n')
        sys.exit(1)
    textus = textus_ex_html(d['parse']['text']['*'])
    with open(sys.argv[2], 'w', encoding='utf-8') as f:
        f.write(d['parse']['title'] + '\n\n' + textus + '\n')


if __name__ == '__main__':
    main()
