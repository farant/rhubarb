/* probatio_harmonia.c - Probationes harmoniae (include/harmonia.h)
 *
 * Classes: C 0, C# 1, D 2, D# 3, E 4, F 5, F# 6, G 7, G# 8, A 9, A# 10,
 * B 11. Nomina ex clave; quod sonat; linea per tempus; orthographia
 * auctoris (agnitio ipsa: probatio_acordum.c).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "partitura.h"
#include "harmonia.h"
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
        imprimere("  carmen: %s (offset %u)\n",
                  vitium.causa ? vitium.causa : "-", vitium.offset);
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

/* classes ex indice -1 terminato */
interior i32
_cl (
    constans s32* classes)
{
    i32 b = ZEPHYRUM;

    dum (*classes >= ZEPHYRUM)
    {
        b |= (i32)I << (i32)*classes;
        classes++;
    }
    redde b;
}

interior chorda
_titulus_harmoniae (
     CredoContextus* c,
    constans Carmen* k,
                i32  classes,
                i32  bassus)
{
     ChordaAedificator* a = chorda_aedificator_creare(c->piscina, LXIV);
               Acordum  h = acordum_ex_classibus(classes, bassus);
                chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !harmonia_scribere(k, &h, ZEPHYRUM, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_linea (
     CredoContextus* c,
    constans Carmen* k,
                s64  initium,
                s64  finis)
{
     ChordaAedificator* a = chorda_aedificator_creare(c->piscina, LXIV);
                chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   a == NIHIL
        || !harmonia_lineam_scribere(NIHIL, k, initium, finis,
                                               a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}


    /* ==================================================
     * NOMINA: litterae ex clave (D maior: F#; Gb maior: Gb)
     * ================================================== */

interior vacuum
_sectio_nomina (
    CredoContextus* c)
{
    hic_manens constans s32  G_B[]              = { 7, 11, 2, -1 };
    hic_manens constans s32  A_SEPTIMA[]        = { 9, 1, 4, 7, -1 };
    hic_manens constans s32  FIS_DIM[]          = { 6, 9, 0, -1 };
    hic_manens constans s32  E_[]               = { 4, -1 };
    hic_manens constans s32  IGN[]              = { 0, 4, 6, -1 };
    hic_manens constans s32  NIHIL_[]           = { -1 };
    hic_manens constans s32  SEMI[]             = { 11, 2, 5, 9, -1 };
    hic_manens constans s32  A_SEPTIMA_MINOR[]  = { 9, 0, 4, 7, -1 };
    hic_manens constans s32  D_TERTIA[]         = { 2, 6, -1 };
    hic_manens constans s32  D_TERTIA_MINOR[]   = { 2, 5, -1 };
    hic_manens constans s32  E_QUINTA[]         = { 4, 11, -1 };
            constans Carmen* d = _carmen(c,
                "<carmina><carmen titulus=\"d\" clavis=\"d maior\">"
                "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
                "mensura=\"1\"><textus!>c</textus></frustum></pars>"
                "</carmen></carmina>");
    constans Carmen* gb = _carmen(c,
        "<carmina><carmen titulus=\"g\" clavis=\"gb maior\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>c</textus></frustum></pars>"
        "</carmen></carmina>");

    CREDO_NECESSE (d != NIHIL && gb != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(G_B),
        XI),
        "G/B");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(A_SEPTIMA), IX),
        "A7");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(FIS_DIM), VI),
        "F#dim");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, gb,
        _cl(FIS_DIM), VI),
        "Gbdim");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(E_),
        IV),
        "(E)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(IGN),
        ZEPHYRUM),
        "[C E F#]");
    /* ignotum a basso sursum: basso E */
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(IGN),
        IV),
        "[E F# C]");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(NIHIL_),
        ZEPHYRUM), "r");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d, _cl(SEMI),
        XI),
        "Bm7b5");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(A_SEPTIMA_MINOR), ZEPHYRUM),
        "C6");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(D_TERTIA), II),
        "D(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(D_TERTIA_MINOR), II),
        "Dm(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_titulus_harmoniae(c, d,
        _cl(E_QUINTA), IV),
        "E(5)");
}


    /* ==================================================
     * SONANTES: mutus et percussio absunt; solus; silentium
     * ================================================== */

interior vacuum
_sectio_sonantes (
    CredoContextus* c)
{
    Carmen* k = (Carmen*)(vacuum*)_carmen(c,
        "<carmina><carmen titulus=\"s\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 e1</textus></frustum></pars>"
        "<pars titulus=\"medius\" instrumentum=\"aether\" mutus>"
        "<frustum "
        "mensura=\"1\"><textus!>o4 g1</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c1</textus></frustum></pars>"
        "<pars titulus=\"p\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>@punctum o2 d1</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
    Acordum h;

    CREDO_NECESSE (k != NIHIL);
    /* E_QUINTA + C3 (G mutus, D percussio): C(3) */
    h = harmonia_ad_pulsum(k, CMLX);
    CREDO_VERUM (h.genus == ACORDUM_TERTIA_MAIOR);
    CREDO_AEQUALIS_I32 (h.radix, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (h.bassus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (h.classes, (I << ZEPHYRUM) | (I << IV));
    /* post finem: nihil */
    h = harmonia_ad_pulsum(k, MMMDCCCXL);
    CREDO_VERUM (h.genus == ACORDUM_NULLA);
    /* melodia sola: E tantum */
    k->partes[ZEPHYRUM].solus  = VERUM;
    h                          = harmonia_ad_pulsum(k, ZEPHYRUM);
    CREDO_VERUM (h.genus == ACORDUM_CLASSIS_UNA);
    CREDO_AEQUALIS_I32 (h.radix, IV);
}


    /* ==================================================
     * LINEA: per tempus, aequalia iuncta; inversio; pausa; 3/4; 6/8;
     * in partitura
     * ================================================== */

interior vacuum
_sectio_linea (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"l\" clavis=\"g maior\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 g4 g4 f#4 f#4 | r1</textus>"
        "</frustum></pars>"
        "<pars titulus=\"medius\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c2 a2 | r1</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 e2 d2 | r2 c2</textus></frustum>"
        "</pars></carmen></carmina>");
    constans Carmen* tres = _carmen(c,
        "<carmina><carmen titulus=\"t\" metrum=\"3/4\">"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c4 e4 g4</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* sex = _carmen(c,
        "<carmina><carmen titulus=\"x\" metrum=\"6/8\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 e8 f8 e8 d4.</textus></frustum>"
        "</pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g4. b4.</textus></frustum></pars>"
        "<pars titulus=\"c\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o3 c4. g4.</textus></frustum></pars>"
        "</carmen></carmina>");
       PartituraForma  f;
    ChordaAedificator* a;

    CREDO_NECESSE (k != NIHIL && tres != NIHIL && sex != NIHIL);
    /* C/E (G5 C4 E3) bis, deinde D (F#5 A4 D_TERTIA) bis */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, ZEPHYRUM, MMMDCCCXL),
        "C/E:2 D:2");
    /* pausa: tempora 1-2 tacent, 3-4 C3 solum */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, MMMDCCCXL,
        II * MMMDCCCXL), "r:2 (C):2");
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, tres, ZEPHYRUM,
        MMDCCCLXXX),
        "(C):4 (E):4 (G):4");
    /* 6/8: tempora tres octavae - F5 transiens (octava secunda) non
     * legitur */
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, sex, ZEPHYRUM, MMDCCCLXXX),
        "C:4. G:4.");
    /* in partitura per lineam additam */
    memset(&f, ZEPHYRUM, magnitudo(f));
    f.prima = I;
    f.ultima = I;
    f.additae[ZEPHYRUM].titulus = "harmonia";
    f.additae[ZEPHYRUM].scribere = harmonia_lineam_scribere;
    a = chorda_aedificator_creare(c->piscina, CCLVI);
    CREDO_NECESSE (a != NIHIL && partitura_scribere(k, &f, a));
    CREDO_CHORDA_CONTINET (chorda_aedificator_finire(a),
        chorda_ex_literis("\n  harmonia  C/E:2 D:2\n", c->piscina));
}

/* orthographia auctoris: C#+E in D minor = C#m(3) (non Dbm); d- + f
 * scripta = Db(3); linea acordorum eadem */
interior chorda
_ad_initium (
     CredoContextus* c,
    constans Carmen* k)
{
     ChordaAedificator* a = chorda_aedificator_creare(c->piscina, LXIV);
               Acordum  h = harmonia_ad_pulsum(k, ZEPHYRUM);
                chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !harmonia_scribere(k, &h, ZEPHYRUM, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

interior vacuum
_sectio_orthographia (
    CredoContextus* c)
{
    constans Carmen* acuta = _carmen(c,
        "<carmina><carmen titulus=\"x\" clavis=\"d minor\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c#1</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* mollis = _carmen(c,
        "<carmina><carmen titulus=\"x\" clavis=\"d minor\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 d-1</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 f1</textus></frustum></pars>"
        "</carmen></carmina>");

    /* G# non sensibilis D minoris: solum littera scripta G# dat (regula
     * clavis Ab daret); prima littera vincit (c# ante d-) */
    constans Carmen* extranea = _carmen(c,
        "<carmina><carmen titulus=\"x\" clavis=\"d minor\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 g#1</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 b1</textus></frustum></pars>"
        "</carmen></carmina>");
    constans Carmen* prima = _carmen(c,
        "<carmina><carmen titulus=\"x\" clavis=\"d minor\">"
        "<pars titulus=\"a\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 c#1</textus></frustum></pars>"
        "<pars titulus=\"b\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 d-1</textus></frustum></pars>"
        "<pars titulus=\"d\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1</textus></frustum></pars>"
        "</carmen></carmina>");

    CREDO_NECESSE (acuta != NIHIL && mollis != NIHIL);
    CREDO_NECESSE (extranea != NIHIL && prima != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_ad_initium(c, extranea), "G#m(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_ad_initium(c, prima), "C#m(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_ad_initium(c, acuta), "C#m(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_ad_initium(c, mollis), "Db(3)");
    CREDO_CHORDA_INCIPIT (_linea(c, acuta, ZEPHYRUM, acuta->pulsus),
        chorda_ex_literis("C#m(3)", c->piscina));
}

/* linea 'acorda': declarata ut scripta per mensuram (C#m auctoris, non
 * Dbm), 'r' nullum, '-' extra spatia */
interior vacuum
_sectio_acorda (
    CredoContextus* c)
{
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"a\" metrum=\"4/4\" "
        "clavis=\"e maior\">"
        "<acorda mensura=\"1\" ictus=\"3\" textus=\"A:2\"/>"
        "<acorda mensura=\"2\" textus=\"C#m:2 B7:2 | r | E\"/>"
        "<pars titulus=\"m\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o4 e1 | e1 | e1 | e1 | e1</textus>"
        "</frustum></pars></carmen></carmina>");
          ChordaAedificator* a;
                        s64  M1 = (s64)MMMDCCCXL;
                        i32  m;
    constans character* constans sperata[V] = {
        "-:2 A:2", "C#m:2 B7:2", "r:1", "E:1", "-:1"
    };

    CREDO_NECESSE (k != NIHIL);
    per (m = ZEPHYRUM; m < V; m++)
    {
        a = chorda_aedificator_creare(c->piscina, LXIV);
        CREDO_NECESSE (a != NIHIL
            && harmonia_acorda_lineam_scribere(NIHIL,
            k, (s64)m * M1, (s64)(m + I) * M1, a));
        CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a),
            sperata[m]);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nomina", _sectio_nomina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sonantes", _sectio_sonantes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea", _sectio_linea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acorda", _sectio_acorda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "orthographia", _sectio_orthographia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("harmonia", SECTIONES);
}
