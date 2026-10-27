"""probatio_silva_arboris.py - pars ARBORIS VIVAE probationum pythonicae
(porta pythonica-arboris): greges qui arborem VIVAM legunt - corpus,
portas veras, bin/fabrica verum super repositorium. Pars pura (fictae,
mundi temporarii) in probatio_silva.py manet (porta pythonica).

Scissio (Franus 2026-10-23, desideratum …VXWK8, decretum …DC4A8Z):
auditus lectionum per greges (FABRICA_LECTIONES cum signis gregum)
greges hos solos arborem totam legere monstravit; ceteri instrumentorum
fontes solos legunt (legit portae pythonica). Greges verbatim translati;
probatio_communia.py praefationem communem tenet."""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probatio_communia  # noqa: E402
from probatio_communia import *  # noqa: E402,F401,F403

probatio_communia.SIGNUM = 'PYTHONICA ARBORIS'

_grex('lint latinus praevius (fail-fast ante portas)')
# functio vera: sine plagula C nihil iudicatum; plagula C sana -> lint
# VERE cucurrit (relatio unci 'nihil novi'); lintrum rubrum ipsum a
# fumo unci (gradus IX-XI) probatur - planta vera plagulam TRACTATAM
# poscit (index git-tractatas solas legit), probatio indicem git non
# tangit.
s_v, rel_v = silva.lint_latinus_praevium(['pythonica/README.md'])
credo(s_v and 'nihil iudicatum' in rel_v, 'lint praevius: sine plagula C nihil iudicatum')
s_c, rel_c = silva.lint_latinus_praevium(['lib/chorda.c', 'vendor/sqlite3.c'])
credo(s_c and 'nihil novi' in rel_c, 'lint praevius: plagula C sana -> lint vere cucurrit (nihil novi)')
# ambitus = iudex lint_latinus (unitates.stml, U4): alienum et fixtura
# non iudicantur; GENERATUM iudicatur (Franus 2026-10-15)
s_x, rel_x = silva.lint_latinus_praevium(['vendor/sqlite3.c', 'probationes/fixa/roundtrip/color.c'])
credo(s_x and 'nihil iudicatum' in rel_x, 'lint praevius: iudex excludit alienum et fixturam (vendor, fixa)')
s_g, rel_g = silva.lint_latinus_praevium(['silva/amalgama/silva.c'])
credo(s_g and 'nihil novi' in rel_g, 'lint praevius: generatum (amalgama) iudicatur')
# wiring: lint rubrum -> commissio ANTE portas refutat, NULLA porta currit
_lint_verus, _porta_vera = silva.lint_latinus_praevium, silva.porta
_cursae = []
silva.lint_latinus_praevium = lambda viae: (False, 'LINT LATINUS (uncus): NOVUM verbumfictum')
silva.porta = lambda *a, **k: _cursae.append(a) or _porta_vera(*a, **k)
try:
    for _nomen, _voca in (('commissio', lambda: silva.commissio('nihil', ['lib/chorda.c'], portae=['formator-intra'])),
                          ('commissio_umbra', lambda: silva.commissio_umbra('nihil', ['lib/chorda.c'], ['formator-intra'], siccum=True))):
        try:
            _voca()
            credo(False, '%s: lint praevius rubrum obstat' % _nomen)
        except silva.SilvaError as ex:
            credo('PRAEVIUS' in str(ex) and 'verbumfictum' in str(ex), '%s: refusio relationem lintris fert' % _nomen)
    credo(_cursae == [], 'lint praevius: NULLA porta cucurrit (fail-fast)')
finally:
    silva.lint_latinus_praevium, silva.porta = _lint_verus, _porta_vera

_grex('probatio_currere: probatio una sicut cursor')
credo(silva.probatio_suita('probatio_credo') == 'radix'
      and silva.probatio_suita('probatio_silva_lexema') == 'silva'
      and silva.probatio_suita('probatio_css_lexema') == 'css'
      if os.path.exists(os.path.join(silva.RADIX, 'css/probationes/probatio_css_lexema.c'))
      else silva.probatio_suita('probatio_silva_lexema') == 'silva',
      'probatio_suita: suita ex fonte')
try:
    silva.probatio_suita('probatio_nemo')
    credo(False, 'probatio ignota levat')
except silva.SilvaError as ex:
    credo('probatio ignota' in str(ex), 'probatio_suita: ignota nominata')
# SUITAE GRADUUM (cursores ad gradus T3): suita conversa binarium in
# area membri habet (build/fabrica/area/<c>/<m>/<m>), non <sub>/build/ -
# ibi binaria cursoris veteris STALA manent (toml post T2)
credo(silva._binarium_probationis('toml', 'probatio_toml_api')
      == 'build/fabrica/area/probationes_toml/probatio_toml_api/probatio_toml_api'
      and silva._binarium_probationis('css', 'probatio_css_arbor')
      == 'build/fabrica/area/probationes_css/probatio_css_arbor/probatio_css_arbor'
      and silva._binarium_probationis('radix', 'probatio_piscina')
      == 'build/probationes/probatio_piscina',
      'probatio_currere: suita conversa -> binarium in area membri')
try:
    c = silva.probatio_currere('probatio_css_arbor', aedificare=True)
    credo(c.rc == 0 and c.suita == 'css'
          and 'Conditio: OMNIA PRAETERIERUNT' in c.acta,
          'probatio_currere: probatio css (gradus) aedificata per probare,'
          ' cursa (rc=%s)' % c.rc)
except silva.SilvaError as ex:
    credo(False, 'probatio_currere: probatio css (gradus) - %s' % ex)
# via binarii: scriptum rubrum fictum (exitus 1) - fracturae genericae
scriptum = os.path.join(silva.RADIX, 'pythonica', '.cursus_ruber.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\necho \"radix: $RHUBARB_RADIX\"\npwd\ncat <<'ACTA'\n"
                          + ACTA_RADIX + "ACTA\nexit 1\n")
os.chmod(scriptum, 0o755)
try:
    c = silva.probatio_currere(scriptum)
    credo(c.rc == 1 and c.suita == '?' and 'radix: ' + silva.RADIX in c.acta
          and c.acta.splitlines()[1].strip() == silva.RADIX,
          'probatio_currere: e radice, RHUBARB_RADIX praebita, rc lectus')
    credo(len(c.fracturae) == 1 and 'Totalis:    3' in c.fracturae[0].relatio
          and 'assertiones 3, fractae 1' in silva.cursus_textus(c),
          'probatio_currere: fracturae + cursus_textus')
finally:
    os.unlink(scriptum)
# binarium absens sine aedificare: refusio quae remedium nominat
try:
    silva.probatio_currere('probatio_silva_lexema') if not os.path.exists(
        os.path.join(silva.RADIX, silva._binarium_probationis(
            'silva', 'probatio_silva_lexema'))) else (_ for _ in ()).throw(
        silva.SilvaError('binarium absens (fictum)'))
    credo(False, 'binarium absens levat')
except silva.SilvaError as ex:
    credo('binarium absens' in str(ex), 'probatio_currere: binarium absens nominat aedificare')
# probatio vera radicis, aedificata per cursorem suitae (filtrum), deinde cursa
c = silva.probatio_currere('probatio_piscina', aedificare=True)
credo(c.rc == 0 and c.suita == 'radix' and 'Conditio: OMNIA PRAETERIERUNT' in c.acta
      and c.fracturae == [] and 'exitus 0' in silva.cursus_textus(c),
      'probatio_currere: probatio_piscina aedificata et viridis')
# partitio: processus vivus per sample (dormiens: folium syscall)
scriptum = os.path.join(silva.RADIX, 'pythonica', '.cursus_dormiens.tmp.sh')
open(scriptum, 'w').write("#!/bin/sh\nsleep 3\necho Totalis: 1\necho Conditio: OMNIA PRAETERIERUNT\n")
os.chmod(scriptum, 0o755)
try:
    c = silva.probatio_currere(scriptum, secunda=1, mora=0.2)
    credo(c.rc == 0 and c.partitio and sum(n for n, _, _ in c.partitio) > 0
          and 'partitio (folia' in silva.cursus_textus(c),
          'probatio_currere: partitio sumptum (%s)' % (c.partitio[0][1] if c.partitio else '-'))
    credo(os.path.exists(os.path.join(silva.RADIX, 'build', 'sample', '.cursus_dormiens.tmp.sh.probatio.txt')),
          'partitio: effusus crudus in build/sample')
finally:
    os.unlink(scriptum)
    try:
        os.unlink(os.path.join(silva.RADIX, 'build', 'sample', '.cursus_dormiens.tmp.sh.probatio.txt'))
    except OSError:
        pass

_grex('oraculum md (ambitus ORACULUM_* involutus)')
# suita md gradus est (cursores ad gradus T4): oraculum cursum COGIT
# (-vis); defectus nominatur, numquam suitam abrumpit
try:
    o = silva.oraculum(exemplum=25)
    credo(o.totalis == 1324 and o.praeterita >= o.pinna >= 1270, 'oraculum: summa cum pinna')
    credo(o.sectiones.get('Tabs', (0, 0))[1] == 22 and len(o.sectiones) >= 30, 'oraculum: sectiones (Tabs 22, XXXI tituli)')
    credo(any(f.numerus == 5 and f.sectio == 'Tabs' and f.causa is None and f.sperata and f.nostra
              for f in o.fracturae), 'oraculum: fracturae omnes cum sperata/nostra normatis')
    credo(len(o.exempla) == 2 and {e.plagula.split('/')[-2] for e in o.exempla} == {'commonmark', 'gfm'}
          and o.exempla[0].md.startswith('&nbsp;') and o.exempla[0].nostra.endswith('</p>'),
          'oraculum: exemplum 25 ex plagula utraque, cruda')
    credo(o.ignoscentiae.get('inter', 0) > 0 and silva.oraculum().exempla == [], 'oraculum: ignoscentiae; sine exemplo nulla')
    # vocatio ITERATA ambitu eodem: clavis membri eadem (T4) - sine -vis
    # membrum servaretur, effusio nulla
    o2 = silva.oraculum()
    credo(o2.totalis == 1324, 'oraculum: iterum ambitu eodem (cursus cogitur)')
except silva.SilvaError as ex:
    credo(False, 'oraculum: cucurrit (%s)' % str(ex)[:160])

_grex('vocabula: recensio identificatorum (T10)')
vb = silva.vocabula('symbola')
credo(vb.numeri['verba'] > 5000 and vb.numeri['sedes'] > 100000, 'vocabula: symbola corporis (verba > V milia, sedes > C milia)')
per_verbum = dict((v.verbum, v) for v in vb.verba)
credo(per_verbum['offset'].status == 'permissum' and per_verbum['piscina'].status == 'notum' and per_verbum['piscina'].lemma == 'piscina' and per_verbum['piscina'].classis == 'N', 'vocabula: permissum (glossarium) et notum (WORDS) cum lemma et classe')
credo(vb.verba[0].sedes >= vb.verba[-1].sedes and len(vb.ignota) == vb.numeri['ignotum'] and vb.ignota[0].via and vb.ignota[0].linea > 0, 'vocabula: ordo sedium descendens, ignota cum sede prima')
try:
    silva.vocabula('alia'); credo(False, 'vocabula: fons ignotus recusatur')
except silva.SilvaError:
    credo(True, 'vocabula: fons ignotus recusatur')
vp = silva.vocabula('prosa')
per_verbum = dict((v.verbum, v) for v in vp.verba)
credo(vp.numeri['verba'] > 5000 and vp.numeri['ambiguum'] == 0 and vp.numeri['latinum'] > 100, 'vocabula: prosa corporis (verba > V milia, ambiguum nullum, latina > C)')
credo(per_verbum['the'].status == 'notum' and per_verbum['the'].classis == 'determinans' and per_verbum['the'].prosa == per_verbum['the'].sedes and per_verbum['the'].symbola == 0, 'vocabula: prosa - the notum determinans, sedes prosae solae')
credo(per_verbum['lexema'].status == 'latinum' and per_verbum['piscina'].status == 'notum' and per_verbum['worklog'].status == 'permissum', 'vocabula: prosa - lexema latinum (glossarium Latinum), piscina notum (Moby eam novit), worklog permissum (contextus anglicus)')
credo(per_verbum['values'].status == 'notum' and per_verbum['values'].lemma == 'value' and per_verbum['values'].regula == 'pluralis-s' and per_verbum['the'].regula == '', 'vocabula: prosa - values per regulam pluralis-s ad value, the sine regula')

_grex('portae debitae: involucrum bin/fabrica portae - arbor VIVA')
_fb_portae = silva.FABRICA_BIN
_portae_seposita = {}
try:
    # arbor VIVA (invariantes): cursor ipse, manu, intectum, normatio.
    # portae FICTAE probationum priorum (cursores extra arborem, sine
    # annotatione) seponuntur - registrum verum solum
    silva.FABRICA_BIN = os.path.join(RADIX, 'bin', 'fabrica')
    for _k in [k for k in silva.PORTAE if k.startswith('ficta')]:
        _portae_seposita[_k] = silva.PORTAE.pop(_k)
    debita, intecta = silva.portae_debitae(['gesta/compile_probationes.sh', 'README.md',
                                            './tools/../apps/forum/fumus.sh'])
    credo(silva.Debitum('gesta', 'gesta/compile_probationes.sh (cursor ipse)', False) in debita,
          'VIVUM: cursor mutatus -> porta sua', causa=repr(debita))
    credo([d.porta for d in debita if d.manu] == ['apps/forum/fumus.sh'],
          'VIVUM: porta manu nominatur (via cursoris)', causa=repr(debita))
    credo(intecta == ['README.md'], 'VIVUM: README.md nullius portae', causa=repr(intecta))
    rel = silva.portae_debitae_relatio(['README.md'])
    credo(rel == 'PORTAE DEBITAE (1 via):\n  (nulla)\nINTECTA (nulla porta):\n  README.md\n',
          'VIVUM: relatio hominis forma fabricae', causa=repr(rel))
finally:
    silva.FABRICA_BIN = _fb_portae
    silva.PORTAE.update(_portae_seposita)

_grex('portae debitae: tools/portae_debitae.sh (arbor VIVA)')
_pdcli = os.path.join(RADIX, 'tools', 'portae_debitae.sh')
credo(os.access(_pdcli, os.X_OK), 'tools/portae_debitae.sh exsistit et exsecutabilis')
if os.access(_pdcli, os.X_OK):
    r = subprocess.run([_pdcli, 'gesta/fontes/tabularium.c', 'README.md'], cwd=RADIX,
                       capture_output=True, text=True)
    credo(r.returncode == 0, 'CLI: exitus 0 cum iudicavit (erat %d)' % r.returncode)
    credo('\n  radix\n' in r.stdout and '\n  gesta\n' in r.stdout
          and 'tabulariumd' in r.stdout,
          'CLI VIVUM: tabularium.c -> radix (per tabulariumd) et gesta')
    credo('INTECTA (nulla porta):\n  README.md' in r.stdout, 'CLI VIVUM: README.md intectum')
    # TABULARIUM NON LEGITUR: annales absentes nihil mutant (clone frigidus)
    e = dict(os.environ, RHUBARB_ANNALES=os.path.join(T, 'annales_nusquam'))
    r2 = subprocess.run([_pdcli, 'gesta/fontes/tabularium.c', 'README.md'], cwd=RADIX,
                        capture_output=True, text=True, env=e)
    credo(r2.returncode == 0 and r2.stdout == r.stdout,
          'CLI: sine annalibus idem responsum (tabularium non legitur)')

# CONSILIUM (consilium S3c, S5c2): forma quae binaria stala fecit -
# consilium sanationis per bin/fabrica VERUM iudicatum: forma FACTUM
# (iudicatur, non agitur), iudex ipse primus (fabrica_struere), ceteri
# 'bin/fabrica sanare X'; portae extra consilium; domus in arbore
# secundaria VETITA, in principali ROGANDA, externa ROGANDA - classis a
# lectore C inferta, pythonica vias non classificat
_fb_ficta_consilii = silva.FABRICA_BIN
silva.FABRICA_BIN = os.path.join(silva.RADIX, 'bin', 'fabrica')
if os.path.exists(silva.FABRICA_BIN):
    _t = silva._consilium_formae(['lib/aedilis.c'],
                                 ['bin/aedilis', 'bin/fabrica'], True)
    _rc, _o, _v = silva._consilium_iudicare(_t, 'probatio_forma')
    _undae = dict((x[2], int(x[1])) for x in _o if x[0] == 'UNDA')
    credo(_rc == 0 and _undae.get('forma') == 1
          and _undae.get('sana bin/fabrica') == 2
          and _undae.get('sana bin/aedilis') == 3
          and 'portae' not in _undae,
          'consilium formae: forma, iudex ipse, ceteri; portae extra', _o)
    # parcum …AG7: sanatio bin/aedilis tabulae_silvae (supra eam) quoque
    # sanat, quae silva/build scribit - vestigium DECLARATUM gradui adest
    credo('<vestigium exemplar="silva/build/*"/>'
          in _t.split('titulus="sana bin/aedilis"')[-1].split('</gradus>')[0],
          'consilium formae: vestigium declaratum ambitus (silva/build ab'
          ' tabulae_silvae) in gradu sana bin/aedilis', _t)
    credo('factum="verum"' in _t
          and '<argumentum valor="tools/fabrica_struere.sh"/>' in _t
          and '<argumentum valor="bin/fabrica"/><argumentum valor="sanare"/>'
              '<argumentum valor="bin/aedilis"/>' in _t,
          'consilium formae: forma factum, mandata iudicis et sanationis', _t)

    def _sententia(t):
        _r, _x, _ = silva._consilium_iudicare(t, 'probatio_forma')
        return _r, [(x[4], x[5]) for x in _x if x[0] == 'IUDICIUM']
    credo(_sententia(silva._consilium_formae(['lib/aedilis.c'],
                                             ['~/.bin/aedilis'], True))
          == (1, [('VETITUM', 'arbor_secundaria')]),
          'consilium formae: in arbore secundaria domus VETITA')
    credo(_sententia(silva._consilium_formae(['lib/aedilis.c'],
                                             ['~/.bin/aedilis'], False))
          == (1, [('ROGANDUM', 'domus_roganda')]),
          'consilium formae: in arbore principali domus ROGANDA')
    credo(_sententia(silva._consilium_formae(['lib/aedilis.c'],
                                             ['/usr/local/x'], False))
          == (1, [('ROGANDUM', 'externa_roganda')]),
          'consilium formae: externa ROGANDA')
    # S3d, S5c2: producentes - fabrica ficta 'sanare -siccum' facta dat
    # (SANATIO EXITUS UNDA), 'consilium' ad binarium VERUM transit;
    # politica (non pythonica) institutionem domus omittit
    _fb_vera = silva.FABRICA_BIN
    _facta = ('SANATIO\tAGENDUM\tnatura\t100\tbin/natura_canones stalum\n'
              'EXITUS\tnatura\tbin/natura_canones\n'
              'EXITUS\tnatura\tbin/natura_glossae\n'
              'SANATIO\tAGENDUM\tcanones_cocti\t50\tgenerator fractus\n'
              'EXITUS\tcanones_cocti\tnatura/cocta/a.canon\n'
              'SANATIO\tAGENDUM\tinstitue\t5\tdomus\n'
              'EXITUS\tinstitue\t~/.bin/natura\n'
              'UNDA\t1\tnatura\nUNDA\t2\tcanones_cocti\nUNDA\t1\tinstitue\n')
    _ficta_s3d = os.path.join(T, 'fabrica_s3d')
    with open(_ficta_s3d, 'w') as _f:
        _f.write('#!/bin/bash\nif [ "$2" = -vestigium ]; then printf'
                 ' "bin/natura_canones\\tnatura/aedificium/*\\n"; exit 0; fi\n'
                 'if [ "$1" = sanare ]; then printf %%s %s; exit 0;'
                 ' fi\nexec %s "$@"\n' % (repr(_facta).replace('\\t', '\t')
                                            .replace('\\n', '\n'), _fb_vera))
    os.chmod(_ficta_s3d, 0o755)
    _vc_vera = silva.VIAE_COMMISSAE
    silva.VIAE_COMMISSAE = lambda viae: set(v for v in viae
                                            if v.startswith('natura/'))
    silva.FABRICA_BIN = _ficta_s3d
    _c = silva._consilium_producentium(['natura/cocta/a.canon'])
    silva.FABRICA_BIN = _fb_vera
    _cr = silva._consilium_iudicare(_c, 'probatio_producentes') \
        if _c is not None else (None, [], None)
    credo(_c is not None and _cr[0] == 0
          and [x[2] for x in _cr[1] if x[0] == 'UNDA']
          == ['sana bin/natura_canones']
          and 'res="bin/natura_glossae"' in _c
          and '<argumentum valor="sanare"/><argumentum'
              ' valor="bin/natura_canones"/>' in _c
          and '<vestigium exemplar="natura/aedificium/*"/>' in _c,
          'consilium producentium: instrumentum sanatur (exitus omnes),'
          ' regenerator commissorum NON, institutio domus a politica'
          ' omissa', _c)
    silva.FABRICA_BIN = _ficta_s3d
    silva.VIAE_COMMISSAE = lambda viae: set(viae)
    _c = silva._consilium_producentium(['natura/cocta/a.canon'])
    silva.FABRICA_BIN = _fb_vera
    silva.VIAE_COMMISSAE = _vc_vera
    credo(_c is None, 'consilium producentium: exitus commissi = nullus'
          ' producens (recusatio antiqua valet)')
    # AG7: '-vestigium' fractum = SilvaError, numquam bin/* build/* tacite
    _ficta_ag7 = os.path.join(T, 'fabrica_ag7')
    with open(_ficta_ag7, 'w') as _f:
        _f.write('#!/bin/bash\necho "vexillum ignotum -vestigium" >&2; exit 2\n')
    os.chmod(_ficta_ag7, 0o755)
    silva.FABRICA_BIN = _ficta_ag7
    try:
        silva._vestigia_sanandi(['bin/x'])
        _ag7 = None
    except silva.SilvaError as _e:
        _ag7 = str(_e)
    silva.FABRICA_BIN = _fb_vera
    credo(_ag7 is not None and 'vexillum ignotum' in _ag7,
          'vestigia sanandi: fabrica fracta -> SilvaError nominata', _ag7)
    # S5c2: exsecutio VERA per '-agere' (arbor viva, build/ solum):
    # scriptura proposita ACTUM, non proposita FRACTUM nominata (3)
    _d = 'build/consilium/probatio_agere'

    def _doc_agere(scribit, scriptum):
        return ('<consilium>\n  <gradus titulus="sana %s/x">\n'
                '    <mandatum><argumentum valor="sh"/><argumentum'
                ' valor="-c"/><argumentum valor="mkdir -p %s; echo x > %s"/>'
                '</mandatum>\n    <effectus verbum="scribere" res="%s"/>\n'
                '  </gradus>\n</consilium>\n' % (_d, _d, scriptum, scribit))
    _rc, _a, _e, _v = silva._consilium_agere(
        _doc_agere(_d + '/x', _d + '/x'), 'probatio_agere')
    credo(_rc == 0 and silva._consilium_acta(_a) == [_d + '/x'],
          'consilium -agere: scriptura proposita ACTUM', (_rc, _a, _e))
    _rc, _a, _e, _v = silva._consilium_agere(
        _doc_agere(_d + '/x', _d + '/y'), 'probatio_agere')
    credo(_rc == 3 and silva._consilium_acta(_a) == []
          and 'EXTRA sana %s/x %s/y' % (_d, _d)
          in silva._consilium_fracturae(_a, _e),
          'consilium -agere: scriptura non proposita FRACTUM nominata',
          (_rc, _a, _e))
else:
    credo(False, 'consilium formae: bin/fabrica deest (nihil iudicatum)')
silva.FABRICA_BIN = _fb_ficta_consilii

_grex('translatio receptorum: lector contra bin/fabrica portae translatio veram')
_tu_fb = silva.FABRICA_BIN
silva.FABRICA_BIN = os.path.join(RADIX, 'bin', 'fabrica')
try:
    # lector contractus contra bin/fabrica portae translatio veram
    # (registrum sine portis fictis - C eas recusaret)
    _tu_portae = silva.PORTAE
    try:
        silva.PORTAE = dict((k2, v2) for k2, v2 in _tu_portae.items()
                            if not k2.startswith('ficta'))
        arb, ins, dec = silva._translationes_umbrae(['vexilla', 'radix'], {})
        credo(re.match(r'^[0-9a-f]{40}$', arb or '') is not None
              and dec.get('vexilla', ('',))[0] == 'nullum'
              and dec.get('radix', ('',))[0] == 'numquam'
              and len(ins.get('vexilla', '')) == 64,
              'umbra: lector contractus contra C veram (ARBOR, INSTRUMENTA, TRANSLATIO)',
              causa=repr((arb, ins, dec)))
    finally:
        silva.PORTAE = _tu_portae
finally:
    silva.FABRICA_BIN = _tu_fb

_grex_claudere()
print()
if fracta:
    print('PYTHONICA ARBORIS: FRACTA %d' % len(fracta))
    sys.exit(1)
print('PYTHONICA ARBORIS: sana')
