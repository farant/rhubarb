/* probatio_explicatio.c - Probationes explicationis
 * (include/explicatio.h)
 *
 * Fenestrae (bassus tenens, octavae, linea secat, pausa, partes mutae
 * et '@' omissae), acorda (remotio minima, radix = bassus, septima
 * implicita ex tritono per clavem), genera singula in carminibus
 * parvis (C maior nisi aliter), linea partiturae.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "partitura.h"
#include "harmonia.h"
#include "explicatio.h"
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

/* carmen trium partium: bassus, media, melodia (ordine), clavis c
 * maior nisi data, 4/4 nisi datum */
interior Carmen*
_tres (
        CredoContextus* c,
    constans character* metrum,
    constans character* clavis,
    constans character* bassus,
    constans character* media,
    constans character* melodia)
{
    character* s = (character*)piscina_allocare(c->piscina, M);

    /* media "-": pars muta (index I servatur, nihil sonat) */
    sprintf(s, "<carmina><carmen titulus=\"p\" metrum=\"%s\" "
            "clavis=\"%s\">"
            "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "<pars titulus=\"media\" instrumentum=\"aether\"%s>"
            "<frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "<pars titulus=\"melodia\" instrumentum=\"aether\">"
            "<frustum "
            "mensura=\"1\"><textus!>%s</textus></frustum></pars>"
            "</carmen></carmina>", metrum, clavis, bassus,
            media[ZEPHYRUM] == '-' ? " mutus" : "",
            media[ZEPHYRUM] == '-' ? "c1" : media, melodia);
    redde _carmen(c, s);
}

interior Explicatio*
_explicare (
      CredoContextus* c,
     constans Carmen* k)
{
    Explicatio* e = (Explicatio*)piscina_allocare(c->piscina,
        magnitudo(Explicatio));

    si (k == NIHIL || !explicatio_carminis(k, c->piscina, e))
    {
        redde NIHIL;
    }
    redde e;
}

/* segmentum partis ad initium; NIHIL si nullum */
interior constans ExplicatioSegmentum*
_segmentum (
    constans Explicatio* e,
                    i32  pars,
                    s64  initium)
{
    i32 i;

    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        si (   e->segmenta[i].pars    == pars
            && e->segmenta[i].initium == initium)
        {
            redde &e->segmenta[i];
        }
    }
    imprimere("  segmentum nullum: pars %u initium %lld\n", pars,
              (long long)initium);
    redde NIHIL;
}

interior b32
_genus_est (
    constans Explicatio* e,
                    i32  pars,
                    s64  initium,
        ExplicatioGenus  genus,
                    b32  resoluta)
{
    constans ExplicatioSegmentum* s = _segmentum(e, pars, initium);

    si (s != NIHIL && s->genus == genus && s->resoluta == resoluta)
    {
        redde VERUM;
    }
    si (s != NIHIL)
    {
        imprimere("  pars %u @%lld: %s resoluta %u (sperabatur %s "
                  "%u)\n",
                  pars, (long long)initium,
                  explicatio_titulus(s->genus),
                  s->resoluta, explicatio_titulus(genus), resoluta);
    }
    redde FALSUM;
}

/* nomen acordi fenestrae */
interior chorda
_acordum (
         CredoContextus* c,
        constans Carmen* k,
    constans Explicatio* e,
                    i32  fenestra)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, XXXII);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   a == NIHIL || fenestra >= e->numerus_fenestrarum
        || !harmonia_scribere(k, &e->fenestrae[fenestra].harmonia,
                              e->fenestrae[fenestra].initium, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

/* linea 'explicatio' partiturae mensurae unius */
interior chorda
_linea (
      CredoContextus* c,
     constans Carmen* k,
          Explicatio* e,
                 i32  mensura)
{
       PartituraForma  f;
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);
               chorda  t;
                  i32  i;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.prima                       = mensura;
    f.ultima                      = mensura;
    f.additae[ZEPHYRUM].titulus   = "explicatio";
    f.additae[ZEPHYRUM].scribere  = explicatio_lineam_scribere;
    f.additae[ZEPHYRUM].ctx       = e;
    t.datum                       = NIHIL;
    t.mensura                     = ZEPHYRUM;
    si (a == NIHIL || !partitura_scribere(k, &f, a))
    {
        redde t;
    }
    t = chorda_aedificator_finire(a);
    dum (t.mensura > ZEPHYRUM && t.datum[t.mensura - I] == '\n')
    {
        t.mensura--;
    }
    i = t.mensura;
    dum (i > ZEPHYRUM && t.datum[i - I] != '\n')
    {
        i--;
    }
    i += (i32)strlen("  explicatio");
    dum (i < t.mensura && t.datum[i] == ' ')
    {
        i++;
    }
    t.datum    += i;
    t.mensura  -= i;
    redde t;
}


/* ==================================================
 * Fenestrae
 * ================================================== */

interior vacuum
_sectio_fenestrae (
    CredoContextus* c)
{
    hic_manens constans s64 LIMITES[IX] = {
        0, 3840, 5760, 7680, 11520, 15360, 19200, 23040, 26880
    };
    constans Carmen* k = _carmen(c,
        "<carmina><carmen titulus=\"f\">"
        "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o5 g1 | e1 | g1 | g1 | g1 | r1 | g1"
        "</textus></frustum></pars>"
        "<pars titulus=\"bassus\" instrumentum=\"aether\"><frustum "
        "mensura=\"1\"><textus!>o2 l8 e >e <e >e <e >e <e >e | "
        "l2 a >c | r1 | c1 | c1 | r1 | c1</textus></frustum></pars>"
        "<pars titulus=\"muta\" instrumentum=\"aether\" mutus><frustum "
        "mensura=\"1\"><textus!>o1 c1</textus></frustum></pars>"
        "<pars titulus=\"tympanum\" instrumentum=\"aether\"><frustum "
        "mensura=\"2\"><textus!>@punctum o1 c1</textus></frustum>"
        "</pars>"
        "</carmen></carmina>");
    Explicatio* e = _explicare(c, k);
           i32  i;

    CREDO_NECESSE (e != NIHIL);
    /* E octavis: una; A, C; G melodiae (bassus tacet); C | C (linea
     * secat); silentium; C */
    CREDO_NECESSE (e->numerus_fenestrarum == VIII);
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        CREDO_AEQUALIS_S64 (e->fenestrae[i].initium, LIMITES[i]);
        CREDO_AEQUALIS_S64 (e->fenestrae[i].finis, LIMITES[i + I]);
    }
    CREDO_AEQUALIS_I32 (e->fenestrae[ZEPHYRUM].harmonia.bassus, IV);
    CREDO_AEQUALIS_I32 (e->fenestrae[III].harmonia.bassus, VII);
    CREDO_AEQUALIS_I32 ((i32)e->fenestrae[VI].harmonia.genus,
                        (i32)HARMONIA_NULLA);
    /* segmenta ordine (initium, pars); muta et '@' nulla */
    per (i = ZEPHYRUM; i < e->numerus_segmentorum; i++)
    {
        CREDO_VERUM (e->segmenta[i].pars < II);
        si (i > ZEPHYRUM)
        {
            CREDO_VERUM (   e->segmenta[i - I].initium
                         <= e->segmenta[i].initium);
        }
    }
}


/* ==================================================
 * Acorda
 * ================================================== */

interior vacuum
_sectio_acorda (
    CredoContextus* c)
{
    constans Carmen* k;
         Explicatio* e;

    /* Bb5 A5 G5 super C3 E4: A removenda (C7) - non Bb (Am7/C):
     * radix = bassus vincit */
    k = _tres(c, "3/4", "d minor", "o3 c2.", "o4 e2.", "o5 b-4 a4 g4");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL && e->numerus_fenestrarum == I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, ZEPHYRUM), "C7");
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_TRANSITUS, FALSUM));
    CREDO_VERUM (_genus_est(e, II, ZEPHYRUM, EXPLICATIO_SEPTIMA,
        VERUM));
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_ACORDI, FALSUM));
    /* nihil removendum: trias plena */
    k = _tres(c, "4/4", "c maior", "o3 c1", "o4 e1", "o5 g1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, ZEPHYRUM), "C");
    CREDO_FALSUM (e->fenestrae[ZEPHYRUM].implicitum);
    /* bassus transiens: D sub C E tentis - acordum prius servatur
     * (C/D), D transitus; superiores tenti suspensiones non sunt */
    k = _tres(c, "4/4", "c maior", "o3 c4 d4 e2", "o4 e1", "o5 c1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL && e->numerus_fenestrarum == III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, I), "C(3)/D");
    CREDO_VERUM (_genus_est(e, ZEPHYRUM, CMLX, EXPLICATIO_TRANSITUS,
                            FALSUM));
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_ACORDI, FALSUM));
    /* bassus gradu intrat, saltu exit: non transiens - et bassus
     * numquam removetur: fenestra D "(D)", non "C(3)/D" */
    k = _tres(c, "4/4", "c maior", "o3 c4 d4 g2", "o4 e1", "o5 c1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL && e->numerus_fenestrarum == III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, I), "(D)");
    /* remotio aequalis magnitudinis: plures notae manentes vincunt -
     * Eb (una) removetur, non E (quattuor): C(3), non Cm(3) */
    k = _tres(c, "4/4", "c maior", "o3 c1", "o4 e2 e2", "o5 e-4 e4 e2");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, ZEPHYRUM), "C(3)");
    /* superiores mutati (I V6 I): acordum novum, non transiens */
    k = _tres(c, "4/4", "c maior", "o3 c4 <b4 >c2", "o4 e4 d4 e2",
              "o5 g4 g4 g2");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL && e->numerus_fenestrarum == III);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, I), "G/B");
    CREDO_VERUM (_genus_est(e, ZEPHYRUM, CMLX, EXPLICATIO_ACORDI,
                            FALSUM));
    /* tritonus solus: septima implicita, radix ex clave */
    k = _tres(c, "4/4", "a minor", "o2 g#1", "-", "o5 d1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, ZEPHYRUM),
        "E7/G#");
    CREDO_VERUM (e->fenestrae[ZEPHYRUM].implicitum);
    k = _tres(c, "4/4", "bb maior", "o2 g#1", "-", "o5 d1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_acordum(c, k, e, ZEPHYRUM),
        "Bb7/Ab");
    CREDO_VERUM (e->fenestrae[ZEPHYRUM].implicitum);
}


/* ==================================================
 * Genera
 * ================================================== */

interior vacuum
_sectio_genera (
    CredoContextus* c)
{
    Explicatio* e;

    /* transitus: C5 D5 E5 */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 e1",
                            "o5 c4 d4 e4 g4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_TRANSITUS, FALSUM));
    CREDO_VERUM (_genus_est(e, II, ZEPHYRUM, EXPLICATIO_ACORDI,
        FALSUM));
    /* vicinus: E5 F5 E5 */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 c1",
                            "o5 e4 f4 e4 g4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_VICINUS, FALSUM));
    /* appoggiatura: C5 -> F5 (saltus) -> E5 */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 e1",
                            "o5 c4 f4 e4 g4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_INNIXA,
                            VERUM));
    /* escapata: C5 D5 (gradus sursum) G4 (saltus deorsum) */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 e1",
                            "o5 c4 d4 <g4 >c4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_EFFUGIENS, FALSUM));
    /* anticipatio: F5 in tempore IV mensurae C, F5 in mensura F */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1 | f1",
                            "o4 e1 | a1", "o5 e2 g4 f4 | f1"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, MMDCCCLXXX, EXPLICATIO_ANTICIPATIO,
                            FALSUM));
    /* pausa motum frangit: D5 post pausam */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 e1",
                            "o5 c4 r4 d4 e4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_IGNOTA, FALSUM));
    /* ignota: C#6 saltu intrat et exit; fenestra sine nomine */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 c1", "o4 e1",
                            "o5 g4 >c#4 <e4 g4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_IGNOTA, FALSUM));
}

/* suspensio (tenta et repetita), non resoluta, retardatio */
interior vacuum
_sectio_suspensio (
    CredoContextus* c)
{
    Explicatio* e;

    /* C5 tenta per F -> G: segmentum secundum suspensio, B4 resolvit */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 f2 g2", "o4 a2 d2",
                            "o5 c2. <b4"));
    CREDO_NECESSE (e != NIHIL && e->numerus_fenestrarum == II);
    CREDO_VERUM (_genus_est(e, II, ZEPHYRUM, EXPLICATIO_ACORDI,
        FALSUM));
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_SUSPENSIO, VERUM));
    /* altitudo repetita praeparat */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 f2 g2", "o4 a2 d2",
                            "o5 c2 c4 <b4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_SUSPENSIO, VERUM));
    /* non resoluta: G4 saltu */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 f2 g2", "o4 a2 b2",
                            "o5 c2. <g4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_SUSPENSIO,
        FALSUM));
    /* retardatio: D5 tenta per G -> C, E5 sursum */
    e = _explicare(c, _tres(c, "4/4", "c maior", "o3 g2 c2", "o4 b2 e2",
                            "o5 d2. e4"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, MCMXX, EXPLICATIO_RETARDATIO,
        VERUM));
}

/* septima resoluta, non resoluta, implicita resoluta */
interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Explicatio* e;

    e = _explicare(c, _tres(c, "4/4", "c maior", "o2 g1 | o3 c1",
                            "o4 f1 | e1", "o4 b2 >d2 | c1"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, I, ZEPHYRUM, EXPLICATIO_SEPTIMA, VERUM));
    e = _explicare(c, _tres(c, "4/4", "c maior", "o2 g1 | o3 c1",
                            "o4 f1 | g1", "o4 b2 >d2 | c1"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, I, ZEPHYRUM, EXPLICATIO_SEPTIMA,
        FALSUM));
    /* septima transiens sursum: C5 D5 E5 super E7/G# (korobeiniki
     * mensura 3) - resoluta; saltu sursum non */
    e = _explicare(c, _tres(c, "4/4", "a minor", "o2 g#1", "o4 b1",
                            "o5 c4 d4 e2"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_SEPTIMA, VERUM));
    e = _explicare(c, _tres(c, "4/4", "a minor", "o2 g#1", "o4 b1",
                            "o5 c4 d4 >e2"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, CMLX, EXPLICATIO_SEPTIMA, FALSUM));
    /* korobeiniki: D5 super G#2 (E7 implicita) -> C5 */
    e = _explicare(c, _tres(c, "4/4", "a minor", "o2 g#1 | a1", "-",
                            "o5 d1 | c1"));
    CREDO_NECESSE (e != NIHIL);
    CREDO_VERUM (_genus_est(e, II, ZEPHYRUM, EXPLICATIO_SEPTIMA,
        VERUM));
}


/* ==================================================
 * Linea partiturae
 * ================================================== */

interior vacuum
_sectio_linea (
    CredoContextus* c)
{
    constans Carmen* k;
         Explicatio* e;

    k = _tres(c, "3/4", "d minor", "o3 c2.", "o4 e2.", "o5 b-4 a4 g4");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, e, I),
        "C7:2. (A5 tr.)");
    /* implicita, septima sine resolutione */
    k = _tres(c, "4/4", "a minor", "o2 g#1", "-", "o5 d1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, e, I),
        "E7/G#*:1 (D5 sept.!)");
    /* fenestrae duae; suspensio in secunda */
    k = _tres(c, "4/4", "c maior", "o3 f2 g2", "o4 a2 d2",
        "o5 c2. <b4");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, e, I),
        "F:2 G:2 (C5 susp.)");
    /* non resoluta: '!' */
    k = _tres(c, "4/4", "c maior", "o3 f2 g2", "o4 a2 b2",
        "o5 c2. <g4");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, e, I),
        "F:2 G(3):2 (C5 susp.!)");
    /* plures alienae, ordine */
    k = _tres(c, "4/4", "c maior", "o3 c1", "o4 g1",
        "o5 e4 f4 e4 d4 | c1");
    e = _explicare(c, k);
    CREDO_NECESSE (e != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_linea(c, k, e, I),
        "C:1 (F5 vic., D5 tr.)");
}

interior vacuum
_sectio_tituli (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_LITERAE_AEQUALES (explicatio_titulus(EXPLICATIO_ACORDI),
                            "acordi");
    CREDO_LITERAE_AEQUALES (explicatio_titulus(EXPLICATIO_TRANSITUS),
                            "transitus");
    CREDO_LITERAE_AEQUALES (explicatio_titulus(EXPLICATIO_IGNOTA),
                            "ignota");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fenestrae", _sectio_fenestrae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acorda", _sectio_acorda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "genera", _sectio_genera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "suspensio", _sectio_suspensio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "septima", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea", _sectio_linea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tituli", _sectio_tituli, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("explicatio", SECTIONES);
}
