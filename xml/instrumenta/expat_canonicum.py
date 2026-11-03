# xml/instrumenta/expat_canonicum.py - oraculum expat (planum xml-arbor
# P1): documentum -> forma canonica suitae W3C, aut INVALIDUM.
#
# Forma canonica (James Clark; lecta ex xmlts out/, PROVENIENTIA.md):
#   nulla declaratio XML, nulla commentaria; elementum vacuum = <a></a>;
#   attributa ordine titulorum (punctis codicis); in textu et valoribus
#   & < > " -> &amp; &lt; &gt; &quot;, TAB LF CR -> &#9; &#10; &#13;;
#   instructio = <?scopus data?>. FORMA SECUNDA: si DTD notationes
#   declarat, praecedit '<!DOCTYPE radix [\n' + notationes ordine
#   titulorum ('<!NOTATION n PUBLIC 'p' 's'>' / PUBLIC 'p' / SYSTEM 's')
#   + ']>\n'.
#   DOCTYPE formae secundae stat ubi in documento stetit (instructio
#   ante DOCTYPE ante eum scribitur: ibm29v01).
# Spatia nominum (NS 1.0): documentum iterum legitur a parsatore cum
# spatiis (separator ' '); vitium spatiorum = INVALIDUM. Forma ex
# parsatore sine spatiis (tituli ut scripti).
# DIVERGENTIA NOTA: expat (2.7.4) regulas titulorum editionum I-IV
# servat, non editionis V - casus IBM P84-P89 et similes recusat quos
# editio V accipit (PROVENIENTIA.md).
# Entitates externae NUMQUAM leguntur (spec X3): nec subsetum externum
# nec entitates parametri nec generales.
#
# Usus: python3 -I expat_canonicum.py via.xml
#   exitus 0: forma canonica in stdout; exitus 1: 'INVALIDUM: l:c causa'
import sys
import xml.parsers.expat as expat

EFFUGIA = {'&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;',
           '\t': '&#9;', '\n': '&#10;', '\r': '&#13;'}


# str.translate (C) pro characteribus singulis per generatorem
# (budget B2 X1: ~CXCI milliones characterum per aurum silvestre, LX s)
_TABULA_EFFUGIORUM = str.maketrans(EFFUGIA)


def _effugere(t):
    return t.translate(_TABULA_EFFUGIORUM)


def _spatia_probare(octeti):
    p = expat.ParserCreate(namespace_separator=' ')
    p.SetParamEntityParsing(expat.XML_PARAM_ENTITY_PARSING_NEVER)
    p.Parse(octeti, True)


def canonicum(octeti, spatia=True):
    """-> (VERUM, forma) aut (FALSUM, 'l:c causa'); spatia = FALSUM
    pro casibus NAMESPACE="no" """
    p = expat.ParserCreate()
    p.SetParamEntityParsing(expat.XML_PARAM_ENTITY_PARSING_NEVER)
    p.buffer_text = True
    corpus, notationes, radix, locus = [], {}, [None], [0]

    def initium(titulus, attributa):
        corpus.append('<' + titulus)
        for k in sorted(attributa):
            corpus.append(' %s="%s"' % (k, _effugere(attributa[k])))
        corpus.append('>')

    def finis(titulus):
        corpus.append('</%s>' % titulus)

    def textus(t):
        corpus.append(_effugere(t))

    def instructio(scopus, data):
        corpus.append('<?%s %s?>' % (scopus, data))

    def notatio(titulus, basis, systema, publicum):
        notationes[titulus] = (publicum, systema)

    def doctype(titulus, systema, publicum, subsetum):
        radix[0] = titulus

    def doctype_finis():
        locus[0] = len(corpus)

    p.StartElementHandler = initium
    p.EndElementHandler = finis
    p.CharacterDataHandler = textus
    p.ProcessingInstructionHandler = instructio
    p.NotationDeclHandler = notatio
    p.StartDoctypeDeclHandler = doctype
    p.EndDoctypeDeclHandler = doctype_finis
    try:
        p.Parse(octeti, True)
        if spatia:
            _spatia_probare(octeti)
    except expat.ExpatError as e:
        return False, '%d:%d %s' % (e.lineno, e.offset + 1,
                                    expat.ErrorString(e.code))
    except (ValueError, LookupError) as e:  # codificatio non tractata
        return False, '1:1 %s' % e
    caput = ''
    if notationes:
        caput = '<!DOCTYPE %s [\n' % radix[0]
        for n in sorted(notationes):
            pub, sys_ = notationes[n]
            if pub is not None and sys_ is not None:
                caput += "<!NOTATION %s PUBLIC '%s' '%s'>\n" % (n, pub, sys_)
            elif pub is not None:
                caput += "<!NOTATION %s PUBLIC '%s'>\n" % (n, pub)
            else:
                caput += "<!NOTATION %s SYSTEM '%s'>\n" % (n, sys_)
        caput += ']>\n'
    return True, (''.join(corpus[:locus[0]]) + caput
                  + ''.join(corpus[locus[0]:]))


if __name__ == '__main__':
    bene, exitus = canonicum(open(sys.argv[1], 'rb').read())
    if bene:
        sys.stdout.write(exitus)
        sys.exit(0)
    print('INVALIDUM: ' + exitus)
    sys.exit(1)
