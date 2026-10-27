/* probatio_partitura.c - Probationes partiturae (include/partitura.h)
 *
 * Textus exactus: carmina parva STML, deinde partitura_scribere; liber
 * ordinarius (Korobeiniki) mensura prima aurea et totus sine ligaturis
 * (mensurae transcriptionis exacte implentur).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "partitura.h"
#include <stdio.h>
#include <string.h>

interior Carmen*
_carmen (
        CredoContextus* c,
    constans character* stml)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_legere(chorda_ex_literis(stml, c->piscina), c->piscina,
                        liber, &vitium) || liber->numerus < I)
    {
        imprimere("  carmen: %s\n", vitium.causa ? vitium.causa : "-");
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

/* textus partiturae; mensura 0 si FALSUM */
interior chorda
_scribere (
             CredoContextus* c,
            constans Carmen* k,
    constans PartituraForma* forma)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !partitura_scribere(k, forma, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

interior PartituraForma
_forma (
        CredoContextus* c,
                   i32  prima,
                   i32  ultima,
    constans character* pars)
{
    PartituraForma f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.prima         = prima;
    f.ultima        = ultima;
    f.pars.datum    = NIHIL;
    f.pars.mensura  = ZEPHYRUM;
    si (pars != NIHIL)
    {
        f.pars = chorda_ex_literis(pars, c->piscina);
    }
    redde f;
}

interior b32
_aequat (
    constans PartituraNota* n,
                 character  littera,
                       s32  alteratio,
                       s32  octava)
{
    si (   n->littera == littera && n->alteratio == alteratio
        && n->octava  == octava)
    {
        redde VERUM;
    }
    imprimere("  orthographia: %c %d %d pro %c %d %d\n", n->littera,
              (integer)n->alteratio, (integer)n->octava, littera,
              (integer)alteratio, (integer)octava);
    redde FALSUM;
}


    /* ==================================================
     * ORTHOGRAPHIA: clavis, extra scalam, littera auctoris
     * ================================================== */

interior vacuum
_sectio_orthographia (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"o\" clavis=\"a minor\">"
        "<clavis mensura=\"2\" valor=\"eb maior\"/>"
        "<clavis mensura=\"3\" valor=\"gb maior\"/>"
        "<clavis mensura=\"4\" valor=\"f# maior\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c</textus></frustum>"
        "</pars></carmen></carmina>");
       CarmenNota n;
    PartituraNota o;

    CREDO_NECESSE (k != NIHIL);
    memset(&n, ZEPHYRUM, magnitudo(n));
    /* A minor: G# extra scalam, '#' (clavis sine b) */
    n.nota  = LXVIII;
    o       = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'G', I, IV));
    n.nota  = LXIII;
    o       = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'D', I, IV));
    n.nota  = LXIV;
    o       = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'E', ZEPHYRUM, IV));
    /* Eb maior (mensura 2): gradus scalae b, naturalis, extra 'b' */
    n.nota  = LXIII;
    o       = partitura_orthographia(k, &n, MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'E', -I, IV));
    n.nota  = LXVIII;
    o       = partitura_orthographia(k, &n, MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'A', -I, IV));
    n.nota  = LXIV;
    o       = partitura_orthographia(k, &n, MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'E', ZEPHYRUM, IV));
    n.nota  = LXVI;
    o       = partitura_orthographia(k, &n, MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'G', -I, IV));
    /* signatura ipsa (non regula extra scalam): Gb maior Cb, F# maior
     * E# - naturales B et F aliter */
    n.nota  = LIX;
    o       = partitura_orthographia(k, &n, II * MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'C', -I, IV));
    n.nota  = LXV;
    o       = partitura_orthographia(k, &n, III * MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'E', I, IV));
    /* littera auctoris vincit; octava ex littera */
    n.nota     = LX;
    n.littera  = 'b';
    o          = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'B', I, III));
    n.nota     = LIX;
    n.littera  = 'c';
    o          = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'C', -I, IV));
    n.nota     = LXII;
    n.littera  = 'e';
    o          = partitura_orthographia(k, &n, MMMDCCCXL);
    CREDO_VERUM (_aequat(&o, 'E', -II, IV));
    /* MIDI 0..XI: octava -1 */
    n.nota     = I;
    n.littera  = '\0';
    o          = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'C', I, -I));
}


    /* ==================================================
     * VALORES: punctata, trioli, pulsus crudi
     * ================================================== */

interior vacuum
_sectio_valores (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"v\">"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 c4. d8 e8 e8 e8 r8 | "
        "c4 c4 c4 c4 c4</textus></frustum>"
        "</pars></carmen></carmina>");
    CarmenNota* n;
           i32  i;

    CREDO_NECESSE (k != NIHIL);
    /* MML longitudines potentiae II solum: trioli et pulsus crudi per
     * editionem exemplaris (ut editor) */
    n = k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae;
    per (i = ZEPHYRUM; i < III; i++)
    {
        n[II + i].pulsus     = MCMXX + (s64)i * CCCXX;
        n[II + i].longitudo  = CCCXX;
    }
    per (i = ZEPHYRUM; i < V; i++)
    {
        n[V + i].pulsus     = MMMDCCCXL + (s64)i * DCCLXVIII;
        n[V + i].longitudo  = DCCLXVIII;
    }
    k->pulsus = II * MMMDCCCXL;
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen v: c maior, 4/4, 120\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  m  C4:4. D4:8 E4:8t E4:8t E4:8t r:4\n"
        "mensura 2\n"
        "  m  C4:~768 C4:~768 C4:~768 C4:~768 C4:~768\n");
}


    /* ==================================================
     * LIGATURAE ET PAUSAE: trans lineam, frustum serius, pausae maximae
     * ================================================== */

interior vacuum
_sectio_ligaturae (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"l\">"
        "<pars titulus=\"a\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 c2. d2 e4</textus></frustum>"
        "</pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\">"
        "<frustum mensura=\"2\"><textus!>o3 c2.</textus></frustum>"
        "</pars>"
        "<pars titulus=\"c\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 r4. c8 r2</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen l: c maior, 4/4, 120\n"
        "  pars a: aether, lucrum 1.00\n"
        "  pars b: aether, lucrum 1.00\n"
        "  pars c: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  a  C4:2. D4:4~\n"
        "  b  r:1\n"
        "  c  r:4. C4:8 r:2\n"
        "mensura 2\n"
        "  a  ~:4 E4:4 r:2\n"
        "  b  C3:2. r:4\n"
        "  c  r:1\n");
}


/* portamentum: '/' post valorem ubi nota desinit (trans lineam: post
 * residuum) */
interior vacuum
_sectio_illapsus (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"p\">"
        "<pars titulus=\"a\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 c4/ e4 g2/ | e1/8 | c2 e1/ g2</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen p: c maior, 4/4, 120\n"
        "  pars a: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  a  C4:4/ E4:4 G4:2/\n"
        "mensura 2\n"
        "  a  E4:1/\n"
        "mensura 3\n"
        "  a  C4:2 E4:2~\n"
        "mensura 4\n"
        "  a  ~:2/ G4:2\n");
}


    /* ==================================================
     * SIMUL: acervi, percussio, mutus et solus
     * ================================================== */

interior vacuum
_sectio_simul (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"a\" instrumentum=\"aether\" mutus>"
        "<frustum mensura=\"1\"><textus!>o4 c4 e4 r2</textus></frustum>"
        "</pars>"
        "<pars titulus=\"p\" instrumentum=\"aether\" solus>"
        "<frustum mensura=\"1\"><textus!>@punctum c8 r8 r2.</textus>"
        "</frustum></pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    /* e eodem pulsu quo c: acervus [C4 E4] (editor, non MML) */
    k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae[I].pulsus = ZEPHYRUM;
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen s: c maior, 4/4, 120\n"
        "  pars a: aether, lucrum 1.00 [mutus]\n"
        "  pars p: aether, lucrum 1.00 [solus]\n"
        "mensura 1\n"
        "  a  [C4 E4]:4 r:2.\n"
        "  p  @punctum:8 r:2. r:8\n");
}


    /* ==================================================
     * MUTATIONES: metrum, clavis, tempus (et intra mensuram)
     * ================================================== */

interior vacuum
_sectio_mutationes (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"m\" clavis=\"a minor\">"
        "<metrum mensura=\"2\" valor=\"3/4\" "
        "agogica=\"vindobonensis\"/>"
        "<clavis mensura=\"2\" valor=\"eb maior\"/>"
        "<tempus mensura=\"2\" valor=\"160\"/>"
        "<tempus pulsus=\"4800\" valor=\"90\" usque=\"4\"/>"
        "<fermata mensura=\"3\" ictus=\"2\" factor=\"1.5\"/>"
        "<fermata mensura=\"4\"/>"
        "<dynamica mensura=\"1\" valor=\"mf\"/>"
        "<dynamica pars=\"m\" mensura=\"3\" valor=\"ff\" usque=\"4\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 c1 | e-2. | c2. | c2.</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen m: a minor, 4/4, 120\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1  dynamica mf\n"
        "  m  C4:1\n"
        "mensura 2  metrum 3/4 agogica vindobonensis  clavis eb maior  "
        "tempus 160  "
        "tempus 90 usque 4 (+960)\n"
        "  m  Eb4:2.\n"
        "mensura 3  fermata x1.5 (+960)  dynamica m ff usque 4\n"
        "  m  C4:2.\n"
        "mensura 4  fermata x2\n"
        "  m  C4:2.\n");
    /* clivus ab initio: titulus tempus SCRIPTUM (C) nominat */
    k = _carmen(c,
        "<carmina><carmen titulus=\"r\" tempus=\"100\" "
        "agogica=\"2 1 2 1 2 1 2 1\">"
        "<tempus mensura=\"1\" valor=\"60\" usque=\"2\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 c1 | c1</textus></frustum>"
        "</pars></carmen></carmina>");
    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen r: c maior, 4/4 agogica 2 1 2 1 2 1 2 1, 100\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1  tempus 60 usque 2\n"
        "  m  C4:1\n"
        "mensura 2\n"
        "  m  C4:1\n");
}


    /* ==================================================
     * FORMA: mensurae, pars, vitia
     * ================================================== */

#define CARMEN_TRIUM                                                  \
    "<carmina><carmen titulus=\"t\">"                                 \
    "<pars titulus=\"x\" instrumentum=\"aether\">"                    \
    "<frustum mensura=\"1\"><textus!>o4 c1 d1 e1</textus></frustum>"  \
    "</pars>"                                                         \
    "<pars titulus=\"yy\" instrumentum=\"triangulum\" lucrum=\"0.5\">" \
    "<frustum mensura=\"1\"><textus!>o2 c1 d1 e1</textus></frustum>"  \
    "</pars></carmen></carmina>"

interior vacuum
_sectio_forma (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, CARMEN_TRIUM);
     PartituraForma  f;

    CREDO_NECESSE (k != NIHIL);
    f = _forma(c, II, II, "yy");
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen t: c maior, 4/4, 120\n"
        "  pars yy: triangulum, lucrum 0.50\n"
        "mensura 2\n"
        "  yy  D2:1\n");
    /* 0 = prima / ultima carminis */
    f = _forma(c, ZEPHYRUM, ZEPHYRUM, NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen t: c maior, 4/4, 120\n"
        "  pars x: aether, lucrum 1.00\n"
        "  pars yy: triangulum, lucrum 0.50\n"
        "mensura 1\n"
        "  x   C4:1\n"
        "  yy  C2:1\n"
        "mensura 2\n"
        "  x   D4:1\n"
        "  yy  D2:1\n"
        "mensura 3\n"
        "  x   E4:1\n"
        "  yy  E2:1\n");
    f = _forma(c, III, ZEPHYRUM, "x");
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen t: c maior, 4/4, 120\n"
        "  pars x: aether, lucrum 1.00\n"
        "mensura 3\n"
        "  x  E4:1\n");
    /* vitia: pars ignota, extra carmen, ultima < prima */
    f = _forma(c, I, I, "nulla");
    CREDO_VERUM (_scribere(c, k, &f).mensura == ZEPHYRUM);
    f = _forma(c, IV, ZEPHYRUM, NIHIL);
    CREDO_VERUM (_scribere(c, k, &f).mensura == ZEPHYRUM);
    f = _forma(c, I, IX, NIHIL);
    CREDO_VERUM (_scribere(c, k, &f).mensura == ZEPHYRUM);
    f = _forma(c, III, II, NIHIL);
    CREDO_VERUM (_scribere(c, k, &f).mensura == ZEPHYRUM);
}


    /* ==================================================
     * KOROBEINIKI: mensura prima aurea; totum sine ligaturis
     * ================================================== */

interior vacuum
_sectio_liber (
    CredoContextus* c)
{
            Carmina  liber;
     CarminisVitium  vitium;
    constans Carmen* k;
     PartituraForma  f;
             chorda  t;
                i32  mensurae  = ZEPHYRUM;
                i32  i;

    CREDO_NECESSE (carmina_ordinaria(c->piscina, &liber, &vitium));
    k = carmina_invenire(&liber, chorda_ex_literis("korobeiniki",
        c->piscina));
    CREDO_NECESSE (k != NIHIL);
    f = _forma(c, I, I, NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen korobeiniki: a minor, 4/4, 144\n"
        "  pars melodia: quadrans, lucrum 0.70\n"
        "  pars bassus: triangulum, lucrum 0.80\n"
        "mensura 1\n"
        "  melodia  E5:4 B4:8 C5:8 D5:4 C5:8 B4:8\n"
        "  bassus   E2:8 E3:8 E2:8 E3:8 E2:8 E3:8 E2:8 E3:8\n");
    /* totum: XVI mensurae, nulla ligatura nec pulsus crudus */
    t = _scribere(c, k, NIHIL);
    CREDO_NECESSE (t.mensura > ZEPHYRUM);
    per (i = ZEPHYRUM; i + VIII <= t.mensura; i++)
    {
        si (   memcmp(t.datum + i, "mensura ", VIII) == ZEPHYRUM
            && (i == ZEPHYRUM || t.datum[i - I] == '\n'))
        {
            mensurae++;
        }
    }
    CREDO_AEQUALIS_I32 (mensurae, XVI);
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '~')
        {
            imprimere("  '~' ad octetum %u\n", i);
            frange;
        }
    }
    CREDO_VERUM (i == t.mensura);
}

/* linea addita ficta: mensura quaeque "[initium,finis)" */
interior b32
_addita (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus)
{
    (vacuum)carmen;
    (vacuum)ctx;
    redde chorda_aedificator_appendere_character(textus, '[')
        && chorda_aedificator_appendere_s32(textus, (s32)initium)
        && chorda_aedificator_appendere_character(textus, ',')
        && chorda_aedificator_appendere_s32(textus, (s32)finis)
        && chorda_aedificator_appendere_character(textus, ')');
}

/* linea addita: post partes, titulus in columna (latitudo eius) */
interior vacuum
_sectio_addita (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c, CARMEN_TRIUM);
     PartituraForma  f;

    CREDO_NECESSE (k != NIHIL);
    f                             = _forma(c, II, III, NIHIL);
    f.additae[ZEPHYRUM].titulus   = "analysis";
    f.additae[ZEPHYRUM].scribere  = _addita;
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen t: c maior, 4/4, 120\n"
        "  pars x: aether, lucrum 1.00\n"
        "  pars yy: triangulum, lucrum 0.50\n"
        "mensura 2\n"
        "  x         D4:1\n"
        "  yy        D2:1\n"
        "  analysis  [3840,7680)\n"
        "mensura 3\n"
        "  x         E4:1\n"
        "  yy        E2:1\n"
        "  analysis  [7680,11520)\n");
    /* lineae duae ordine; columna ad titulum longissimum; titulus
     * NIHIL finit (tertia cum scriptore sed sine titulo omittitur) */
    f.additae[I].titulus     = "analysis_altera";
    f.additae[I].scribere    = _addita;
    f.additae[III].titulus   = "nunquam";
    f.additae[III].scribere  = _addita;
    f.ultima                 = II;
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, &f),
        "carmen t: c maior, 4/4, 120\n"
        "  pars x: aether, lucrum 1.00\n"
        "  pars yy: triangulum, lucrum 0.50\n"
        "mensura 2\n"
        "  x                D4:1\n"
        "  yy               D2:1\n"
        "  analysis         [3840,7680)\n"
        "  analysis_altera  [3840,7680)\n");
}

/* nota scripta (partitura_notam_scribere); mensura 0 si FALSUM */
interior chorda
_nota (
          CredoContextus* c,
         constans Carmen* k,
     constans CarmenNota* n,
                     s64  pulsus)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, XVI);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !partitura_notam_scribere(k, n, pulsus, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

/* ambitus: aequales (prima vincit), sonitus '@' omissi, pulsus carminis
 * (frustum + nota) ad orthographiam clavis */
interior vacuum
_sectio_ambitus (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"a\" clavis=\"c maior\">"
        "<clavis mensura=\"2\" valor=\"eb maior\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 a#4 >d4 <b-4 >c4 | f1"
        "</textus></frustum></pars>"
        "<pars titulus=\"p\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>@punctum c4 g4</textus>"
        "</frustum></pars>"
        "<pars titulus=\"q\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>c4 @punctum g4</textus>"
        "</frustum></pars>"
        "<pars titulus=\"s\" instrumentum=\"aether\">"
        "<frustum mensura=\"2\"><textus!>c4 d#4 e-4</textus></frustum>"
        "</pars></carmen></carmina>");
    constans CarmenNota* m;
             CarmenNota* s;
       PartituraAmbitus  a;

    CREDO_NECESSE (k != NIHIL && k->numerus_partium == IV);
    /* A#4 et Bb4 aequales: prima (pulsus 0) vincit, littera sua */
    m = k->partes[ZEPHYRUM].frusta[ZEPHYRUM].notae;
    a = partitura_ambitus(&k->partes[ZEPHYRUM]);
    CREDO_AEQUALIS_I32 (a.numerus, V);
    CREDO_VERUM (a.infima == &m[ZEPHYRUM]);
    CREDO_AEQUALIS_S64 (a.pulsus_infimae, ZEPHYRUM);
    CREDO_VERUM (a.summa == &m[IV]);
    CREDO_AEQUALIS_S64 (a.pulsus_summae, MMMDCCCXL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k, a.infima,
        a.pulsus_infimae), "A#4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k, &m[II], MCMXX), "Bb4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k, a.summa,
        a.pulsus_summae), "F5");
    /* sonitus soli: nulla altitudo; scriptus '@titulus' */
    a = partitura_ambitus(&k->partes[I]);
    CREDO_AEQUALIS_I32 (a.numerus, ZEPHYRUM);
    CREDO_VERUM (a.infima == NIHIL && a.summa == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k,
        &k->partes[I].frusta[ZEPHYRUM].notae[ZEPHYRUM], ZEPHYRUM),
        "@punctum");
    /* mixta: G '@' altior omittitur - C4 infima et summa */
    m = k->partes[II].frusta[ZEPHYRUM].notae;
    a = partitura_ambitus(&k->partes[II]);
    CREDO_AEQUALIS_I32 (a.numerus, I);
    CREDO_VERUM (a.infima == &m[ZEPHYRUM] && a.summa == &m[ZEPHYRUM]);
    /* sine littera (editio exemplaris): clavis ad pulsum CARMINIS -
     * frustum in mensura 2 (Eb maior) -> Eb4; pulsus intra frustum
     * solus (C maior) D#4 daret. Summa aequalis (D# scripta, Eb):
     * prima vincit */
    s             = k->partes[III].frusta[ZEPHYRUM].notae;
    s[I].littera  = '\0';
    a             = partitura_ambitus(&k->partes[III]);
    CREDO_AEQUALIS_I32 (a.numerus, III);
    CREDO_VERUM (a.infima == &s[ZEPHYRUM]);
    CREDO_AEQUALIS_S64 (a.pulsus_infimae, MMMDCCCXL);
    CREDO_VERUM (a.summa == &s[I]);
    CREDO_AEQUALIS_S64 (a.pulsus_summae, MMMDCCCXL + CMLX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k, a.summa,
        a.pulsus_summae), "Eb4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_nota(c, k, a.summa, CMLX), "D#4");
}

/* lectio notae: inversa scriptionis; littera dat octavam (Cb4 = LIX,
 * B#3 = LX); fines MIDI; formae malae */
interior b32
_lecta (
         CredoContextus* c,
     constans character* textus,
                    i32  altitudo,
              character  littera,
                    s32  alteratio,
                    s32  octava)
{
    PartituraNota n;
              i32 a = XCIX;

    si (   partitura_notam_legere(chorda_ex_literis(textus, c->piscina),
        &n,
                                  &a)
        && a           == altitudo && n.littera == littera
        && n.alteratio == alteratio && n.octava == octava)
    {
        redde VERUM;
    }
    imprimere("  %s: altitudo %u %c %d %d\n", textus, a, n.littera,
              (integer)n.alteratio, (integer)n.octava);
    redde FALSUM;
}

interior vacuum
_sectio_lectio (
    CredoContextus* c)
{
    hic_manens constans character* MALAE[] = {
        "", "H4", "C", "Bb", "@punctum", "C4x", "C###4", "C#b4",
        "C-", "Cb-1", "G#9", "C10", "#4", " C4", "C4 "
    };
    PartituraNota n;
              i32 a;
              i32 i;
              s32 o;
        character t[VIII];

    CREDO_VERUM (_lecta(c, "C4", LX, 'C', ZEPHYRUM, IV));
    CREDO_VERUM (_lecta(c, "Bb4", LXX, 'B', -I, IV));
    CREDO_VERUM (_lecta(c, "F#5", LXXVIII, 'F', I, V));
    CREDO_VERUM (_lecta(c, "C##3", L, 'C', II, III));
    CREDO_VERUM (_lecta(c, "Ebb2", XXXVIII, 'E', -II, II));
    CREDO_VERUM (_lecta(c, "Cb4", LIX, 'C', -I, IV));
    CREDO_VERUM (_lecta(c, "B#3", LX, 'B', I, III));
    CREDO_VERUM (_lecta(c, "C-1", ZEPHYRUM, 'C', ZEPHYRUM, -I));
    CREDO_VERUM (_lecta(c, "G9", CXXVII, 'G', ZEPHYRUM, IX));
    /* litterae parvae (agens "g2" scribit): 'bb4' = Bb4 */
    CREDO_VERUM (_lecta(c, "g2", XLIII, 'G', ZEPHYRUM, II));
    CREDO_VERUM (_lecta(c, "bb4", LXX, 'B', -I, IV));
    /* C per octavas omnes */
    per (o = -I; o <= IX; o++)
    {
        sprintf(t, "C%d", (integer)o);
        CREDO_VERUM (_lecta(c, t, (i32)((o + I) * XII), 'C', ZEPHYRUM,
            o));
    }
    per (i = ZEPHYRUM; i < (i32)(magnitudo(MALAE)
        / magnitudo(MALAE[0]));
         i++)
    {
        si (partitura_notam_legere(chorda_ex_literis(MALAE[i],
            c->piscina),
                                   &n, &a))
        {
            imprimere("  mala accepta: '%s'\n", MALAE[i]);
            CREDO_CULPA ("forma mala accepta");
        }
    }
}

/* sine littera: sensibilis clavis minoris = gradus VII elevatus (C# in
 * D minor, non Db); chroma aliud regulam priorem servat (Eb) */
interior vacuum
_sectio_sensibilis (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"x\" clavis=\"d minor\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");
       CarmenNota n;
    PartituraNota o;

    CREDO_NECESSE (k != NIHIL);
    memset(&n, ZEPHYRUM, magnitudo(n));
    n.nota  = LXI;
    o       = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'C', I, IV));
    n.nota  = LXIII;
    o       = partitura_orthographia(k, &n, ZEPHYRUM);
    CREDO_VERUM (_aequat(&o, 'E', -I, IV));
}

/* ligatura trans lineam: nota una, '~' ad lineam; trioli '8t' */
interior vacuum
_sectio_ligatura (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"l\">"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 c2 d2 & | d2 e12 f12 g12 a4</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_AEQUALIS_I32 (k->partes[ZEPHYRUM].frusta[ZEPHYRUM].numerus,
                        VI);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen l: c maior, 4/4, 120\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  m  C4:2 D4:2~\n"
        "mensura 2\n"
        "  m  ~:2 E4:8t F4:8t G4:8t A4:4\n");
}

/* repetitio: titulus mensurae metae nominat sectionem et semitonia;
 * transpositae litteras ex clave accipiunt (E4 - 1 in C maior = D#4) */
interior vacuum
_sectio_repetitio (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"r\">"
        "<sectio titulus=\"a\" mensura=\"1\" usque=\"2\"/>"
        "<repetitio sectio=\"a\" mensura=\"2\" semitonia=\"2\"/>"
        "<repetitio sectio=\"a\" mensura=\"3\" semitonia=\"-1\"/>"
        "<repetitio sectio=\"a\" mensura=\"4\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\"><textus!>o4 c2 e2</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen r: c maior, 4/4, 120\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  m  C4:2 E4:2\n"
        "mensura 2  repetitio a +2\n"
        "  m  D4:2 F#4:2\n"
        "mensura 3  repetitio a -1\n"
        "  m  B3:2 D#4:2\n"
        "mensura 4  repetitio a\n"
        "  m  C4:2 E4:2\n");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "orthographia", _sectio_orthographia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "sensibilis", _sectio_sensibilis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "valores", _sectio_valores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligaturae", _sectio_ligaturae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "portamentum", _sectio_illapsus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simul", _sectio_simul, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mutationes", _sectio_mutationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "addita", _sectio_addita, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lectio", _sectio_lectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ambitus", _sectio_ambitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liber", _sectio_liber, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligatura", _sectio_ligatura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "repetitio", _sectio_repetitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("partitura", SECTIONES);
}
