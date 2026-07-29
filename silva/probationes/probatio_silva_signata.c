/* probatio_silva_signata.c - GENERA SIGNATA (gradus 3, 01KYNXXVX9):
 * typedef nominales cum algebra affini/lineari.
 *
 * SPECIMINA ANTE MACHINAM (ritus formarum): exspectationes hae ante
 * praedicatum scriptae sunt. Codex 82 SIGNATUM_COMMIXTUM.
 *
 * Algebra: genus LINEARE (sine differentia - vector): idem+idem
 * licet, trans genera vetitum. Genus AFFINE (cum differentia -
 * punctum): punctum+punctum VETITUM etiam eodem genere,
 * punctum-punctum = differentia, punctum +/- differentia = punctum.
 * Insignatum NEUTRUM (graduale); CONVERSIO explicita = benedictio.
 * Ambulatio generum PURA - emissio ad hamum BINARIUM (semel per
 * nodum typatum) et ad limites, numquam in ambulatione ipsa. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_c89_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), NIHIL);
}

/* fons -> codex exspectatus n-ies */
interior vacuum
_codicem_probare (Piscina* piscina, constans character* fons,
    s32 codex, i32 numerus, constans character* descriptio)
{
    SilvaParsura* parsura;
    SilvaSemantica* sem;
    i32 inventi = ZEPHYRUM;
    i32 i;
    i32 m;

    imprimere("  %s\n", descriptio);
    parsura = _parsare(piscina, fons);
    CREDO_NON_NIHIL(parsura);
    si (parsura == NIHIL)
    {
        redde;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    CREDO_NON_NIHIL(sem);
    si (sem == NIHIL)
    {
        redde;
    }
    m = (i32)silva_c89_diagnostica_numerus(sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            inventi++;
        }
    }
    si (inventi != numerus)
    {
        imprimere("    [FONS FRACTUS] inventi=%ld"
            " exspectati=%ld\n%s\n",
            (longus)inventi, (longus)numerus, fons);
    }
    CREDO_AEQUALIS_I32((i32)inventi, (i32)numerus);
}

/* praefatio communis: Momentum (affine, differentia Mora) + Mora
 * (lineare) + Denarii (lineare) - omnia signata, subiecta longa */
#define PRAEFATIO \
    "/* <contractus signatum=\"Momentum\" differentia=\"Mora\"/> */\n" \
    "typedef long Momentum;\n" \
    "/* <contractus signatum=\"Mora\"/> */\n" \
    "typedef long Mora;\n" \
    "/* <contractus signatum=\"Denarii\"/> */\n" \
    "typedef long Denarii;\n"

#define FONTIS_CAPACITAS 2048

interior vacuum
_signatum_probare (Piscina* piscina, constans character* corpus,
    i32 numerus, constans character* descriptio)
{
    character fons[FONTIS_CAPACITAS];

    si (strlen(corpus) + strlen(PRAEFATIO)
        >= (memoriae_index)FONTIS_CAPACITAS)
    {
        CREDO_VERUM(FALSUM);
        redde;
    }
    strcpy(fons, PRAEFATIO);
    strcat(fons, corpus);
    _codicem_probare(piscina, fons,
        (s32)EXAMEN_CODEX_SIGNATUM_COMMIXTUM, numerus, descriptio);
}

integer
principale (vacuum)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIO SILVA SIGNATA (gradus 3)\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_signata",
        16777216);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("--- I-VIII: limites (initiator/argumentum/redde/"
        "membrum) ---\n");

    /* I. initiator trans genera -> violatio */
    _signatum_probare(piscina,
        "void f(Momentum m) { Denarii d = m; }",
        I, "I. Denarii d = m (Momentum) -> violatio");

    /* II. insignatum in signatum: graduale, constantes adoptant */
    _signatum_probare(piscina,
        "void f(void) { Momentum m = 99; }",
        ZEPHYRUM, "II. Momentum m = 99 -> TACET (neutrum)");

    /* III. signatum in insignatum: erasio licita */
    _signatum_probare(piscina,
        "void f(Momentum m) { long x = m; }",
        ZEPHYRUM, "III. long x = m -> TACET (erasio)");

    /* IV. idem genus -> TACET */
    _signatum_probare(piscina,
        "void f(Momentum a) { Momentum b = a; }",
        ZEPHYRUM, "IV. Momentum b = a -> TACET");

    /* V. benedictio conversionis explicitae */
    _signatum_probare(piscina,
        "void f(Momentum m) { Denarii d = (Denarii)m; }",
        ZEPHYRUM, "V. (Denarii)m -> TACET (benedictio)");

    /* VI. argumentum trans genera -> violatio */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m) { g(m); }",
        I, "VI. g(m) ubi g(Denarii) -> violatio");

    /* VII. redde trans genera -> violatio */
    _signatum_probare(piscina,
        "Momentum f(Denarii d) { return d; }",
        I, "VII. return d (Denarii) ex Momentum -> violatio");

    /* VIII. assignatio membro trans genera -> violatio */
    _signatum_probare(piscina,
        "struct S { Denarii pretium; };\n"
        "void f(struct S s, Momentum m) { s.pretium = m; }",
        I, "VIII. s.pretium = m -> violatio");

    imprimere("--- IX-XX: algebra affinis/linearis ---\n");

    /* IX. punctum + punctum -> violatio (etiam eodem genere) */
    _signatum_probare(piscina,
        "void f(Momentum a, Momentum b) { long x = a + b; }",
        I, "IX. Momentum + Momentum -> violatio");

    /* X. punctum - punctum = differentia -> in Moram TACET */
    _signatum_probare(piscina,
        "void f(Momentum a, Momentum b) { Mora r = a - b; }",
        ZEPHYRUM, "X. Mora r = a - b -> TACET (differentia)");

    /* XI. punctum - punctum in Momentum -> limes capit */
    _signatum_probare(piscina,
        "void f(Momentum a, Momentum b) { Momentum r = a - b; }",
        I, "XI. Momentum r = a - b -> violatio (Mora est)");

    /* XII. punctum + differentia = punctum -> TACET */
    _signatum_probare(piscina,
        "void f(Momentum a, Mora r) { Momentum b = a + r; }",
        ZEPHYRUM, "XII. Momentum b = a + r -> TACET");

    /* XIII. differentia - punctum -> violatio (spatium-punctum) */
    _signatum_probare(piscina,
        "void f(Momentum a, Mora r) { long x = r - a; }",
        I, "XIII. r - a (Mora - Momentum) -> violatio");

    /* XIV. lineare + lineare idem genus -> TACET */
    _signatum_probare(piscina,
        "void f(Mora a, Mora b) { Mora c = a + b; }",
        ZEPHYRUM, "XIV. Mora + Mora -> TACET");

    /* XV. comparatio trans genera -> violatio */
    _signatum_probare(piscina,
        "int f(Momentum m, Denarii d) { return (m < d) ? 1 : 0; }",
        I, "XV. m < d trans genera -> violatio");

    /* XVI. scalatio linearis: una parte vacua -> TACET */
    _signatum_probare(piscina,
        "void f(Mora a) { Mora b = a * 2; }",
        ZEPHYRUM, "XVI. Mora * 2 -> TACET (scalatio)");

    /* XVII. lineare * lineare -> violatio (algebra unitatum nulla) */
    _signatum_probare(piscina,
        "void f(Mora a, Mora b) { long x = a * b; }",
        I, "XVII. Mora * Mora -> violatio");

    /* XVIII. ratio: idem lineare / -> vacuum, TACET */
    _signatum_probare(piscina,
        "void f(Mora a, Mora b) { long x = a / b; }",
        ZEPHYRUM, "XVIII. Mora / Mora -> TACET (ratio)");

    /* XIX. phasis: affine % differentia = differentia -> TACET */
    _signatum_probare(piscina,
        "void f(Momentum m, Mora p) { Mora r = m % p; }",
        ZEPHYRUM, "XIX. Mora r = m % p -> TACET (phasis)");

    /* XX. comparatio eodem genere -> TACET (puncta comparabilia) */
    _signatum_probare(piscina,
        "int f(Momentum a, Momentum b) { return (a < b) ? 1 : 0; }",
        ZEPHYRUM, "XX. a < b eodem genere -> TACET");

    imprimere("--- XXI: genus per effectum vocationis ---\n");

    /* XXI. genus reditus vocati per orthographiam declarationis */
    _signatum_probare(piscina,
        "Momentum nunc(void);\n"
        "void f(void) { Denarii d = nunc(); }",
        I, "XXI. Denarii d = nunc() -> violatio");

    imprimere("--- XXII-XXIV: grammatica STALUS ---\n");

    /* XXII. signatum + intra eodem elemento: cura una */
    _codicem_probare(piscina,
        "/* <contractus signatum=\"X\" intra=\"0,5\"/> */\n"
        "typedef long X;\n"
        "void f(void) { X x = 0; }",
        (s32)EXAMEN_CODEX_CONTRACTUS_STALUS, I,
        "XXII. signatum + intra -> STALUS");

    /* XXIII. differentia non registrata -> STALUS */
    _codicem_probare(piscina,
        "/* <contractus signatum=\"P\" differentia=\"Ignota\"/> */\n"
        "typedef long P;\n"
        "void f(void) { P x = 0; }",
        (s32)EXAMEN_CODEX_CONTRACTUS_STALUS, I,
        "XXIII. differentia ignota -> STALUS");

    /* XXIV. differentia typo insignato subiecta -> STALUS (lex
     * morarum signatarum; symbolum hic resolubile) */
    _codicem_probare(piscina,
        "/* <contractus signatum=\"U\"/> */\n"
        "typedef unsigned long U;\n"
        "/* <contractus signatum=\"P\" differentia=\"U\"/> */\n"
        "typedef long P;\n"
        "void f(void) { P x = 0; }",
        (s32)EXAMEN_CODEX_CONTRACTUS_STALUS, I,
        "XXIV. differentia insignata -> STALUS");

    imprimere("--- XXV-XXXVIII: vestigatio fluminis (arcus fluxus"
        " 01KYPZ3XPW) ---\n");

    /* XXV. lavatio simplex ad argumentum (LACUNA banked) */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m) { long t = m; g(t); }",
        I, "XXV. long t = m; g(t) ubi g(Denarii) -> violatio");

    /* XXVI. lavatio ad argumentum genere pari -> TACET */
    _signatum_probare(piscina,
        "void g(Momentum d);\n"
        "void f(Momentum m) { long t = m; g(t); }",
        ZEPHYRUM, "XXVI. g(t) ubi g(Momentum) -> TACET (par)");

    /* XXVII. lavatio in binarium trans genera */
    _signatum_probare(piscina,
        "void f(Momentum m, Denarii d) {\n"
        "    long t = m;\n"
        "    long x = t + d;\n"
        "}",
        I, "XXVII. t (fluxu Momentum) + d (Denarii) -> violatio");

    /* XXVIII. lavatio punctum + punctum (alter declaratus) */
    _signatum_probare(piscina,
        "void f(Momentum m, Momentum b) {\n"
        "    long t = m;\n"
        "    long x = t + b;\n"
        "}",
        I, "XXVIII. t (fluxu) + b -> punctum+punctum violatio");

    /* XXIX. iunctio ramorum dissentiens = AMISSA -> TACET */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m, Denarii p, int c) {\n"
        "    long t;\n"
        "    if (c) { t = m; } else { t = p; }\n"
        "    g(t);\n"
        "}",
        ZEPHYRUM, "XXIX. iunctio dissentiens -> TACET (amissa)");

    /* XXX. iunctio ramorum consentiens -> stirps servatur */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m, int c) {\n"
        "    long t;\n"
        "    if (c) { t = m; } else { t = m; }\n"
        "    g(t);\n"
        "}",
        I, "XXX. iunctio consentiens -> violatio (servata)");

    /* XXXI. erasio conversione consulta -> TACET (littera una
     * lavationis honestae) */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m) { long t = (long)m; g(t); }",
        ZEPHYRUM, "XXXI. long t = (long)m; g(t) -> TACET (erasio)");

    /* XXXII. effugium = stirps amissa -> TACET */
    _signatum_probare(piscina,
        "void h(long* p);\n"
        "void g(Denarii d);\n"
        "void f(Momentum m) { long t = m; h(&t); g(t); }",
        ZEPHYRUM, "XXXII. h(&t) -> TACET (effugium)");

    /* XXXIII. redefinitio littera = NEUTRA delet -> TACET */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m) { long t = m; t = 5; g(t); }",
        ZEPHYRUM, "XXXIII. t = 5 post t = m -> TACET (deleta)");

    /* XXXIV. redde flumine-signatum contra genus reditus */
    _signatum_probare(piscina,
        "Denarii f(Momentum m) { long t = m; return t; }",
        I, "XXXIV. return t (fluxu Momentum) ex Denarii ->"
            " violatio");

    /* XXXV. initiator flumine-signatus contra genus positionis */
    _signatum_probare(piscina,
        "void f(Momentum m) { long t = m; { Denarii d = t; } }",
        I, "XXXV. Denarii d = t (fluxu Momentum) -> violatio");

    /* XXXVI. assignatio flumine-signata in positionem signatam */
    _signatum_probare(piscina,
        "void f(Momentum m, Denarii d) { long t = m; d = t; }",
        I, "XXXVI. d = t (fluxu Momentum) -> violatio");

    /* XXXVII. ansa sui-referens genere PARI: v1 conservative
     * delebat, v2 (classificatio ambitu-conscia) servat ET par
     * est - silentium utroque modo, ratione recta nunc */
    _signatum_probare(piscina,
        "void g(Momentum d);\n"
        "void f(Momentum m, Mora r, int n) {\n"
        "    long t = m;\n"
        "    int i;\n"
        "    for (i = 0; i < n; i++) { t = t - r; }\n"
        "    g(t);\n"
        "}",
        ZEPHYRUM, "XXXVII. ansa t = t - r, g(Momentum) -> TACET"
            " (servatum ET par)");

    /* XXXVIII. t + t fluxu: punctum+punctum UNA emissione (dedup
     * sedium - folia duo eiusdem BINARII) */
    _signatum_probare(piscina,
        "void f(Momentum m) { long t = m; long x = t + t; }",
        I, "XXXVIII. t + t (fluxu) -> violatio UNA (dedup)");

    imprimere("--- XXXIX-XLVII: v2 fluxus (ansae/catenae/composita/"
        "monstratores) ---\n");

    /* XXXIX. ansa sui-referens TRANS genera: v1 conservativum
     * (AMISSA) nunc in vim conversum - provenientia servata
     * flagrat */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m, Mora r, int n) {\n"
        "    long t = m;\n"
        "    int i;\n"
        "    for (i = 0; i < n; i++) { t = t - r; }\n"
        "    g(t);\n"
        "}",
        I, "XXXIX. ansa t = t - r, g(Denarii) -> violatio"
            " (servatum)");

    /* XL. catena assignationis: valor = dextrum internum */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m) {\n"
        "    long t;\n"
        "    long u;\n"
        "    t = u = m;\n"
        "    g(t);\n"
        "}",
        I, "XL. t = u = m; g(t) ubi g(Denarii) -> violatio"
            " (catena)");

    /* XLI. compositum genus servat: t -= r punctum - spatium */
    _signatum_probare(piscina,
        "void g(Denarii d);\n"
        "void f(Momentum m, Mora r) {\n"
        "    long t = m;\n"
        "    t -= r;\n"
        "    g(t);\n"
        "}",
        I, "XLI. t -= r (Momentum - Mora); g(Denarii) ->"
            " violatio");

    /* XLII. compositum typatione: punctum += punctum */
    _signatum_probare(piscina,
        "void f(Momentum a, Momentum b) { a += b; }",
        I, "XLII. a += b (puncta) -> violatio (typatio)");

    /* XLIII. compositum typatione trans genera */
    _signatum_probare(piscina,
        "void f(Denarii d, Momentum m) { d -= m; }",
        I, "XLIII. d -= m -> violatio (typatio)");

    /* XLIV. compositum fluxu: t += b puncta ambo (alter fluxu) */
    _signatum_probare(piscina,
        "void f(Momentum m, Momentum b) {\n"
        "    long t = m;\n"
        "    t += b;\n"
        "}",
        I, "XLIV. t (fluxu Momentum) += b -> violatio");

    /* XLV. compositum licitum genus servat: punctum += spatium */
    _signatum_probare(piscina,
        "void g(Momentum d);\n"
        "void f(Momentum m, Mora r) {\n"
        "    long t = m;\n"
        "    t += r;\n"
        "    g(t);\n"
        "}",
        ZEPHYRUM, "XLV. t += r; g(Momentum) -> TACET (servatum"
            " et par)");

    /* XLVI. effectus compositi contra positionem: m -= m2 =
     * differentia in punctum */
    _signatum_probare(piscina,
        "void f(Momentum a, Momentum b) { a -= b; }",
        I, "XLVI. a -= b (differentia in punctum) -> violatio");

    /* XLVII. custos monstratorum: differentia monstratorum in
     * genus signatum - orthographia specificatorum genus NON fert
     * (typus signabilis) -> TACET */
    _signatum_probare(piscina,
        "void f(Momentum* p, Momentum* q) { Momentum r = q - p; }",
        ZEPHYRUM, "XLVII. Momentum r = q - p (monstratores) ->"
            " TACET (custos)");

    imprimere("--- XLVIII-LIII: stirpes membrorum (v2b) ---\n");

    /* XLVIII. lavatio per campum insignatum ad argumentum */
    _signatum_probare(piscina,
        "struct S { long t; };\n"
        "void g(Denarii d);\n"
        "void f(Momentum m) {\n"
        "    struct S s;\n"
        "    s.t = m;\n"
        "    g(s.t);\n"
        "}",
        I, "XLVIII. s.t = m; g(s.t) ubi g(Denarii) -> violatio");

    /* XLIX. lavatio membri genere pari -> TACET */
    _signatum_probare(piscina,
        "struct S { long t; };\n"
        "void g(Momentum d);\n"
        "void f(Momentum m) {\n"
        "    struct S s;\n"
        "    s.t = m;\n"
        "    g(s.t);\n"
        "}",
        ZEPHYRUM, "XLIX. g(s.t) ubi g(Momentum) -> TACET (par)");

    /* L. copia totius stirpem membri delet -> TACET */
    _signatum_probare(piscina,
        "struct S { long t; };\n"
        "void g(Denarii d);\n"
        "void f(Momentum m, struct S s2) {\n"
        "    struct S s;\n"
        "    s.t = m;\n"
        "    s = s2;\n"
        "    g(s.t);\n"
        "}",
        ZEPHYRUM, "L. s = s2 post s.t = m -> TACET (deleta)");

    /* LI. effugium basis stirpes membrorum delet -> TACET */
    _signatum_probare(piscina,
        "struct S { long t; };\n"
        "void h(struct S* p);\n"
        "void g(Denarii d);\n"
        "void f(Momentum m) {\n"
        "    struct S s;\n"
        "    s.t = m;\n"
        "    h(&s);\n"
        "    g(s.t);\n"
        "}",
        ZEPHYRUM, "LI. h(&s) -> TACET (effugium basis)");

    /* LII. membrum flumine-signatum in algebra */
    _signatum_probare(piscina,
        "struct S { long t; };\n"
        "void f(Momentum m, Momentum b) {\n"
        "    struct S s;\n"
        "    long x;\n"
        "    s.t = m;\n"
        "    x = s.t + b;\n"
        "}",
        I, "LII. s.t (fluxu Momentum) + b -> violatio");

    /* LIII. scriptio FRATRIS stirpem delet (dele conservativum -
     * exceptio structurae = refinatio nominata; pin ianuae) */
    _signatum_probare(piscina,
        "struct S { long a; long b; };\n"
        "void g(Denarii d);\n"
        "void f(Momentum m) {\n"
        "    struct S s;\n"
        "    s.a = m;\n"
        "    s.b = 5;\n"
        "    g(s.a);\n"
        "}",
        ZEPHYRUM, "LIII. s.b = 5 post s.a = m -> TACET"
            " (conservativum)");

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
