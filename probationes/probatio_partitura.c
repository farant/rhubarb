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
        "<metrum mensura=\"2\" valor=\"3/4\"/>"
        "<clavis mensura=\"2\" valor=\"eb maior\"/>"
        "<tempus mensura=\"2\" valor=\"160\"/>"
        "<tempus pulsus=\"4800\" valor=\"90\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\">"
        "<frustum mensura=\"1\">"
        "<textus!>o4 c1 | e-2.</textus></frustum>"
        "</pars></carmen></carmina>");

    CREDO_NECESSE (k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_scribere(c, k, NIHIL),
        "carmen m: a minor, 4/4, 120\n"
        "  pars m: aether, lucrum 1.00\n"
        "mensura 1\n"
        "  m  C4:1\n"
        "mensura 2  metrum 3/4  clavis eb maior  tempus 160  "
        "tempus 90 (+960)\n"
        "  m  Eb4:2.\n");
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
    f                  = _forma(c, II, III, NIHIL);
    f.additae_titulus  = "analysis";
    f.addita           = _addita;
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
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "orthographia", _sectio_orthographia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "valores", _sectio_valores, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligaturae", _sectio_ligaturae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simul", _sectio_simul, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mutationes", _sectio_mutationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma", _sectio_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "addita", _sectio_addita, NIHIL, NIHIL, NIHIL, NIHIL },
    { "liber", _sectio_liber, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("partitura", SECTIONES);
}
