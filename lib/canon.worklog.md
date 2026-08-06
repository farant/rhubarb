# canon.worklog.md

## 2026-08-06 — nativitas: schema STML generale (gradus II)

CUR: domus quinque dialectos STML scribit - natura (33 plagulae),
librarium (3, ~CXL milia linearum), grammatica (4, c89.stml solum
MCCXVII lineae quae parserem C regunt), aedilis (2), quaestiones
(1). UNA sola custodem habebat. Vitium schematis in c89.stml aut
aedilis.stml hodie ut mos mirus infra apparet, non ut diagnosticum.

FORMAE CONTENTUS DERIVATAE, NON CONIECTATAE. Instrumentum scriptum
quod corpus ambulat et pro quoque elemento liberos cum min/max et
attributa cum frequentia refert. Duo momenti:
  - CARDINALITAS SINE ORDINE SUFFICIT. Nullus ordo in natura
    postulatur, ergo NULLA opus est forma contentus regularis
    (DTD-stylus). Saccus inordinatus cum numeris omnia tegit -
    lingua multo minor.
  - OBSERVATUM NON EST POSTULATUM. cultivar semel apparet, ergo
    liberi eius {1,1} videntur; fons/@verificatus in XIII ex XIII
    adest sed METAMODULUS 'ordinarius falsum' dicit. Canon ex uno
    exemplo GENERATUS corpus fossilizat - ideo manu SCRIPTUS est,
    derivatione ut teste, non ut auctore.

LINGUA EXPRESSIONUM NON AEDIFICATA, CONSULTO. Assertiones
('valens_a <= valens_ad', 'ad AUT a') linguam expressionum poscunt,
et illa via schemata in linguas programmandi vertit (XSD testis).
Canon gradum II tenet; cetera vocanti relinquuntur.

PRIMA CONTACTIO CORPORIS TRIA INVENIT:
  1-2. 'inversa' DUOBUS MODIS adhibetur: 'verum' (booleanum, ut
       METAMODULUS par.3 dicit) et nomen relationis quam invertit
       ('coquitur' in pharmacon, 'continet' in plagula_
       computatralis). Corpus SIBI dissentit; forma nomen ferens
       plus dicit, sed spec booleanum poscit. QUAESTIO DOCTRINAE.
  3.   quando="-312" (via Appia, CCCXII a.C.) formam 'dies' non
       implet, quae annos ante Christum exprimere NON POTEST.
       ET GRAVIUS: onerator 'quando' NUMQUAM probavit - regula VII
       valens_a/valens_ad sola tegit. Canon probationem invenit
       quam onerator non habebat, primo cursu.

Id est quod stratum schematis praestare debet: non minus codicis in
natura (regulae IX-XIII manent, et illae sunt gemmae), sed
FUNDAMENTUM ubi nullum erat, et vocabularium quod in tribus locis
manu servatum non amplius est.

## 2026-08-06 — dialectus SECUNDUS (aedilis): duo defectus linguae

Aedilis scriptus est ut PROBATIO GENERALITATIS, non ut lucrum. Duo
defectus statim invenit quos natura sola numquam ostendisset - quod
est tota ratio dialecti secundi ante fidem.

I. FORMAE CONTEXTU DEPENDENTES. <regula> duas formas fert:
     intra <nexus>        obiectum= poscit, <vexillum> continet
     intra <irregularia>  caput= poscit, <obiectum> continet
   Declarationes GLOBALES (mos DTD) unionem solam dicere possunt,
   unde '<regula obiectum="x"><obiectum/></regula>' transiret.
   Attributum 'intra=' additum (mos RELAX NG); clavis tabulae fit
   "parens/nomen", et specialius vincit. Natura eo caruit quia
   nullum nomen bis diverse adhibet.

II. REGISTRUM PER EXTENSIONEM SOLAM FALSUM EST. Primo scripsi
    '.stml -> aedilis.canon'. Sed '.stml' QUATTUOR dialectos fert
    (librarium, grammatica, aedilis, quaestiones): illa linea tres
    alios dialectos canone alieno iudicasset. Extensio contractum
    nominat SOLUM ubi extensio dialecto propria est - quod '.genera'
    est et '.stml' non.
    Additum: clavis per ELEMENTUM RADICIS ('<aedilis>'), quae
    dialectum semper nominat quia documentum se ipsum profitetur.
    RADIX VINCIT, extensio cadit.

ET UNCUS BINARII STALI RURSUS. Mutationem 'intra' per
`clang -c ... -o /dev/null` probavi - quod SYNTAXIN probat et
BINARIUM NON SCRIBIT - deinde bin/canon_examen VETUS cursus est.
Scopus tacite fallebat et causam in tabula dispersa quaerebam.
Aedificatio depurationis (flagra pauciora) rem statim solvit, quia
illa RE VERA compilata est.
Eadem lectio quam natura_visus porta stala custodit; canon_examen
eam nondum habet. '-o /dev/null' probat te compilare posse, non te
compilavisse.

MENSURATUM: natura 33 plagulae / 3 vitia (vera, supra); aedilis
2 plagulae / 0 vitia.
