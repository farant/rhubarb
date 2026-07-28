/* probatio_silva_intervalla.c - FLUXUS INTERVALLA (gradus 2,
 * 01KYMBTDGP): analysis intervallorum [imum, summum] super CFG.
 *
 * SPECIMINA ANTE PRAEDICATUM (ritus formarum): exspectationes hae
 * ANTE machinam scriptae sunt. Superficies proba = quaestio publica
 * silva_c89_fluxus_intervallum_ad_punctum (sedes SEVERAE porta
 * heuristica clausae fixpunctum videre nequeunt - addendum
 * INTENTIONIS 2026-07-28).
 *
 * Lattice: IGNOTUM (fundus) < VALIDUM [imum,summum] < OMNIA (extra
 * fines s64 - insignatum 64-bitorum, non-integrale). Summum
 * tractabile = fines TYPI ut VALIDUM. Latificatio V=3 ad extrema
 * typi; refinatio marginum VERUS/FALSUS post latificationem
 * praecisionem custoditam recuperat (specimen IX); custos post usum
 * = imprecisio nominata (specimen X, angustatio v1 omissa). */

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

/* folia identificatoris nomine - collectio recursiva ordine fontis */
interior vacuum
_folia_colligere (Piscina* piscina, constans SilvaNodus* nodus,
    constans character* titulus, Xar* effecta)
{
    Xar* liberi;
    i32 i;
    i32 m;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        SilvaValor tok_v = silva_c89_folium_identificator_tok_valor(
            nodus);

        si (tok_v.genus == SILVA_VALOR_TOKEN)
        {
            chorda valor = tok_v.datum.token->valor;
            memoriae_index mt = strlen(titulus);

            si (valor.mensura == (i32)mt && valor.datum != NIHIL
                && memcmp(valor.datum, titulus, mt) == ZEPHYRUM)
            {
                constans SilvaNodus** locus =
                    (constans SilvaNodus**)xar_addere(effecta);

                *locus = nodus;
            }
        }
    }
    liberi = silva_nodus_liberi(piscina, nodus);
    m = xar_numerus(liberi);
    per (i = ZEPHYRUM; i < m; i++)
    {
        _folia_colligere(piscina,
            *(constans SilvaNodus**)xar_obtinere(liberi, i),
            titulus, effecta);
    }
}

/* k-tum folium (0-basatum) tituli in arbore parsurae */
interior constans SilvaNodus*
_folium (Piscina* piscina, constans SilvaParsura* parsura,
    constans character* titulus, i32 k)
{
    Xar* effecta = xar_creare(piscina,
        (i32)magnitudo(constans SilvaNodus*));

    /* radix = lista valorum, non nodus - ambula elementa */
    {
        i32 i;
        i32 m = (i32)silva_valor_lista_numerus(
            parsura->commissio->radix);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* e = silva_valor_lista_obtinere(
                parsura->commissio->radix, i);

            si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
            {
                _folia_colligere(piscina, e->datum.nodus, titulus,
                    effecta);
            }
        }
    }
    si (k < ZEPHYRUM || k >= xar_numerus(effecta))
    {
        redde NIHIL;
    }
    redde *(constans SilvaNodus**)xar_obtinere(effecta, k);
}

/* fons -> sem analysatum + folium k tituli + quaestio intervalli */
interior b32
_quaerere (Piscina* piscina, constans character* fons,
    constans character* titulus, i32 k, SemanticaIntervallum* iv)
{
    SilvaParsura* parsura = _parsare(piscina, fons);
    SilvaSemantica* sem;
    constans SilvaNodus* folium;

    CREDO_NON_NIHIL(parsura);
    si (parsura == NIHIL)
    {
        redde FALSUM;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    CREDO_NON_NIHIL(sem);
    si (sem == NIHIL)
    {
        redde FALSUM;
    }
    folium = _folium(piscina, parsura, titulus, k);
    CREDO_NON_NIHIL(folium);
    si (folium == NIHIL)
    {
        redde FALSUM;
    }
    redde silva_c89_fluxus_intervallum_ad_punctum(sem, folium, iv);
}

/* nodi ACCESSUS titulo membri - collectio recursiva ordine fontis
 * (membra ut pseudo-variabiles, 01KYMYW75S: quaestio ad nodum
 * ACCESSUS ipsum - eventa membrorum eum ferunt) */
interior vacuum
_accessus_colligere (Piscina* piscina, constans SilvaNodus* nodus,
    constans character* titulus, Xar* effecta)
{
    Xar* liberi;
    i32 i;
    i32 m;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_ACCESSUS)
    {
        SilvaValor tok_v = silva_c89_accessus_tok_titulus(nodus);

        si (tok_v.genus == SILVA_VALOR_TOKEN)
        {
            chorda valor = tok_v.datum.token->valor;
            memoriae_index mt = strlen(titulus);

            si (valor.mensura == (i32)mt && valor.datum != NIHIL
                && memcmp(valor.datum, titulus, mt) == ZEPHYRUM)
            {
                constans SilvaNodus** locus =
                    (constans SilvaNodus**)xar_addere(effecta);

                *locus = nodus;
            }
        }
    }
    liberi = silva_nodus_liberi(piscina, nodus);
    m = xar_numerus(liberi);
    per (i = ZEPHYRUM; i < m; i++)
    {
        _accessus_colligere(piscina,
            *(constans SilvaNodus**)xar_obtinere(liberi, i),
            titulus, effecta);
    }
}

/* k-tus nodus ACCESSUS (0-basatus) tituli membri in arbore */
interior constans SilvaNodus*
_accessus (Piscina* piscina, constans SilvaParsura* parsura,
    constans character* titulus, i32 k)
{
    Xar* effecta = xar_creare(piscina,
        (i32)magnitudo(constans SilvaNodus*));
    i32 i;
    i32 m = (i32)silva_valor_lista_numerus(
        parsura->commissio->radix);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* e = silva_valor_lista_obtinere(
            parsura->commissio->radix, i);

        si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
        {
            _accessus_colligere(piscina, e->datum.nodus, titulus,
                effecta);
        }
    }
    si (k < ZEPHYRUM || k >= xar_numerus(effecta))
    {
        redde NIHIL;
    }
    redde *(constans SilvaNodus**)xar_obtinere(effecta, k);
}

/* fons -> sem analysatum + accessus k membri + quaestio intervalli */
interior b32
_quaerere_membrum (Piscina* piscina, constans character* fons,
    constans character* titulus, i32 k, SemanticaIntervallum* iv)
{
    SilvaParsura* parsura = _parsare(piscina, fons);
    SilvaSemantica* sem;
    constans SilvaNodus* nodus;

    CREDO_NON_NIHIL(parsura);
    si (parsura == NIHIL)
    {
        redde FALSUM;
    }
    sem = silva_c89_semantica_analysare(piscina, parsura);
    CREDO_NON_NIHIL(sem);
    si (sem == NIHIL)
    {
        redde FALSUM;
    }
    nodus = _accessus(piscina, parsura, titulus, k);
    CREDO_NON_NIHIL(nodus);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    redde silva_c89_fluxus_intervallum_ad_punctum(sem, nodus, iv);
}

/* adfirmatio membri: VALIDUM [imum, summum] exspectatum ad nodum
 * ACCESSUS k-tum tituli */
interior vacuum
_adfirma_membrum (Piscina* piscina, constans character* fons,
    constans character* titulus, i32 k, s64 imum, s64 summum,
    constans character* descriptio)
{
    SemanticaIntervallum iv;
    b32 inventum;

    imprimere("  %s\n", descriptio);
    memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
    inventum = _quaerere_membrum(piscina, fons, titulus, k, &iv);
    CREDO_VERUM(inventum);
    si (!inventum)
    {
        redde;
    }
    CREDO_AEQUALIS_I32((i32)iv.status,
        (i32)SEMANTICA_INTERVALLUM_VALIDUM);
    si (iv.imum != imum || iv.summum != summum)
    {
        imprimere("    EXSPECTATUM [%ld, %ld] RECEPTUM [%ld, %ld]\n",
            (longus)imum, (longus)summum, (longus)iv.imum,
            (longus)iv.summum);
    }
    CREDO_VERUM(iv.imum == imum);
    CREDO_VERUM(iv.summum == summum);
}

/* adfirmatio compendiaria: VALIDUM [imum, summum] exspectatum */
interior vacuum
_adfirma (Piscina* piscina, constans character* fons,
    constans character* titulus, i32 k, s64 imum, s64 summum,
    constans character* descriptio)
{
    SemanticaIntervallum iv;
    b32 inventum;

    imprimere("  %s\n", descriptio);
    memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
    inventum = _quaerere(piscina, fons, titulus, k, &iv);
    CREDO_VERUM(inventum);
    si (!inventum)
    {
        redde;
    }
    CREDO_AEQUALIS_I32((i32)iv.status,
        (i32)SEMANTICA_INTERVALLUM_VALIDUM);
    si (iv.imum != imum || iv.summum != summum)
    {
        imprimere("    EXSPECTATUM [%ld, %ld] RECEPTUM [%ld, %ld]\n",
            (longus)imum, (longus)summum, (longus)iv.imum,
            (longus)iv.summum);
    }
    CREDO_VERUM(iv.imum == imum);
    CREDO_VERUM(iv.summum == summum);
}

#define INT_IMUM   (-2147483647L - 1L)
#define INT_SUMMUM 2147483647L
#define U32_SUMMUM 4294967295L

integer
principale (vacuum)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIO SILVA INTERVALLA (fluxus gradus 2)\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_intervalla",
        16777216);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("--- I-V: definitiones, catenae, iunctio ---\n");

    /* I. recta: constans per definitionem */
    _adfirma(piscina,
        "void f(void) { int a = 10; int u = a; }",
        "a", ZEPHYRUM, 10L, 10L,
        "I. recta: a = 10 -> [10,10]");

    /* II. catena: def per expressionem cum ambitu */
    _adfirma(piscina,
        "void f(void) { int a = 10; int b = a + 5; int u = b; }",
        "b", ZEPHYRUM, 15L, 15L,
        "II. catena: b = a + 5 -> [15,15]");

    /* III. ternarius: involucrum bracchiorum */
    _adfirma(piscina,
        "void f(int c) { int t = c ? 3 : 7; int u = t; }",
        "t", ZEPHYRUM, 3L, 7L,
        "III. ternarius -> [3,7]");

    /* IV. parametrum: fines typi ut VALIDUM (summum tractabile) */
    _adfirma(piscina,
        "void f(int p) { int u = p; }",
        "p", ZEPHYRUM, INT_IMUM, INT_SUMMUM,
        "IV. parametrum -> fines int");

    /* V. iunctio ramorum: involucrum */
    _adfirma(piscina,
        "void f(int c) { int x; if (c) { x = 3; } else { x = 10; }"
        " { int u = x; } }",
        "x", II, 3L, 10L,
        "V. iunctio -> [3,10]");

    imprimere("--- VI-VII: refinatio marginum ---\n");

    /* VI. refinatio VERUS: p >= 5 */
    _adfirma(piscina,
        "void f(int p) { if (p >= 5) { int u = p; } }",
        "p", I, 5L, INT_SUMMUM,
        "VI. refinatio VERUS p >= 5 -> [5,max]");

    /* VII. refinatio FALSUS: !(p >= 5) => p <= 4 */
    _adfirma(piscina,
        "void f(int p) { if (p >= 5) { ; } else { int u = p; } }",
        "p", I, INT_IMUM, 4L,
        "VII. refinatio FALSUS -> [min,4]");

    imprimere("--- VIII-X: ansae, latificatio, recuperatio ---\n");

    /* VIII. ansa numerans contra parametrum: refinatio contra
     * summum finium tegit sine latificatione (simulatio) */
    _adfirma(piscina,
        "void f(int n) { int i; for (i = 0; i < n; i++)"
        " { int u = i; } }",
        "i", III, 0L, INT_SUMMUM - 1L,
        "VIII. ansa i < n -> [0, max-1]");

    /* IX. ansa constans: latificatio (V=3) deinde refinatio VERUS
     * praecisionem in corpore recuperat; post ansam FALSUS */
    _adfirma(piscina,
        "void f(void) { int i; for (i = 0; i < 10; i++)"
        " { int u = i; } { int post = i; } }",
        "i", III, 0L, 9L,
        "IX-a. corpus post latificationem: refinatio -> [0,9]");
    _adfirma(piscina,
        "void f(void) { int i; for (i = 0; i < 10; i++)"
        " { int u = i; } { int post = i; } }",
        "i", IV, 10L, INT_SUMMUM,
        "IX-b. post ansam: FALSUS -> [10,max]");

    /* X. custos POST usum: latificatio sine angustatione -
     * imprecisio NOMINATA (angustatio v1 omissa; refinatio
     * marginis usum praecedentem iuvare nequit) */
    _adfirma(piscina,
        "void f(void) { int i = 0; while (1) { int u = i;"
        " i = i + 1; if (i >= 5) break; } }",
        "i", ZEPHYRUM, 0L, INT_SUMMUM,
        "X. custos post usum -> [0,max] (limes nominatus)");

    imprimere("--- XI-XV: effugium, involutio, larva ---\n");

    /* XI. effugium: &a -> fines typi perpetuo */
    _adfirma(piscina,
        "void g(int* q); void f(void) { int a = 5; g(&a);"
        " { int u = a; } }",
        "a", I, INT_IMUM, INT_SUMMUM,
        "XI. effugium &a -> fines int");

    /* XII. involutio insignata additionis: a+1 excedit -> fines */
    _adfirma(piscina,
        "void f(unsigned a) { unsigned u = a + 1u;"
        " { unsigned w = u; } }",
        "u", ZEPHYRUM, 0L, U32_SUMMUM,
        "XII. involutio a+1u -> fines u32");

    /* XIII. subtractio insignata sub zephyro -> fines (sana) */
    _adfirma(piscina,
        "void f(void) { unsigned a = 1u; unsigned d = a - 3u;"
        " { unsigned w = d; } }",
        "d", ZEPHYRUM, 0L, U32_SUMMUM,
        "XIII. involutio 1u-3u -> fines u32");

    /* XIV. larva: x & 255 -> [0,255] */
    _adfirma(piscina,
        "void f(int x) { int m = x & 255; { int u = m; } }",
        "m", ZEPHYRUM, 0L, 255L,
        "XIV. larva & 255 -> [0,255]");

    /* XV. redefinitio: definitio ultima vincit */
    _adfirma(piscina,
        "void f(void) { int a = 10; a = -3; { int u = a; } }",
        "a", I, -3L, -3L,
        "XV. redefinitio -> [-3,-3]");

    imprimere("--- XVI-XVII: OMNIA et non-inventum ---\n");

    /* XVI. insignatum 64-bitorum: extra fines s64 -> OMNIA */
    {
        SemanticaIntervallum iv;
        b32 inventum;

        imprimere("  XVI. unsigned long -> OMNIA\n");
        memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
        inventum = _quaerere(piscina,
            "void f(unsigned long p) { unsigned long u = p; }",
            "p", ZEPHYRUM, &iv);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32((i32)iv.status,
            (i32)SEMANTICA_INTERVALLUM_OMNIA);
    }

    /* XVII. folium extra eventa (titulus functionis) -> FALSUM */
    {
        SemanticaIntervallum iv;
        SilvaParsura* parsura = _parsare(piscina,
            "void g(void); void f(void) { g(); }");
        SilvaSemantica* sem;
        constans SilvaNodus* folium;

        imprimere("  XVII. titulus functionis -> non inventum\n");
        CREDO_NON_NIHIL(parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL(sem);
        folium = _folium(piscina, parsura, "g", ZEPHYRUM);
        CREDO_NON_NIHIL(folium);
        si (folium != NIHIL && sem != NIHIL)
        {
            memset(&iv, ZEPHYRUM,
                magnitudo(SemanticaIntervallum));
            CREDO_FALSUM(silva_c89_fluxus_intervallum_ad_punctum(
                sem, folium, &iv));
        }
    }

    imprimere("--- XVIII-XXIX: membra ut pseudo-variabiles ---\n");

    /* XVIII. definitio membri recta: s.m = 5 -> lectio [5,5]
     * (accessus k=1: k=0 scriptio est) */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void f(void) { struct S s;"
        " s.m = 5u; { unsigned u = s.m; } }",
        "m", I, 5L, 5L,
        "XVIII. s.m = 5 -> [5,5]");

    /* XIX. assignatio totius delet membra: s = g() -> fines typi
     * membri (regula v1: def basis = membra ad fines) */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; struct S g(void);"
        " void f(void) { struct S s; s.m = 5u; s = g();"
        " { unsigned u = s.m; } }",
        "m", I, 0L, U32_SUMMUM,
        "XIX. s = g() delet -> fines u32");

    /* XX. refinatio membri custodita (CASUS MATRIS: chorda.mensura):
     * parametrum basis - membrum fines typi in introitu, custos
     * refinat */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void f(struct S s)"
        " { if (s.m >= 2u) { unsigned x = s.m; } }",
        "m", I, 2L, U32_SUMMUM,
        "XX. custos s.m >= 2 -> [2, u32max]");

    /* XXI. crementum membri: pater accessus operatorem dat */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void f(void) { struct S s;"
        " s.m = 3u; s.m++; { unsigned u = s.m; } }",
        "m", II, 4L, 4L,
        "XXI. s.m++ -> [4,4]");

    /* XXII. effugium basis: &s -> membra fines typi perpetuo */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void g(struct S* p);"
        " void f(void) { struct S s; s.m = 5u; g(&s);"
        " { unsigned u = s.m; } }",
        "m", I, 0L, U32_SUMMUM,
        "XXII. effugium &s -> fines u32");

    /* XXIII. assignatio composita membri: imprecisio v1 (fons =
     * nodus assignationis, aestimator ordinarius -> fines typi;
     * mores localium speculati) */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void f(void) { struct S s;"
        " s.m = 10u; s.m -= 3u; { unsigned u = s.m; } }",
        "m", II, 0L, U32_SUMMUM,
        "XXIII. s.m -= 3 -> fines (imprecisio nominata)");

    /* XXIV. unio: scriptio fratris delet (ordo dele-deinde-pone
     * sanitatem aliasing praestat - deviatio ab pinna rei, INTENTIO
     * 2026-07-28) */
    _adfirma_membrum(piscina,
        "union U { unsigned a; int b; }; void f(void) { union U u;"
        " u.a = 5u; u.b = -1; { unsigned x = u.a; } }",
        "a", I, 0L, U32_SUMMUM,
        "XXIV. unio: u.b scriptum -> u.a fines (non [5,5])");

    /* XXV. membrum parametri sine custode: fines typi membri ut
     * VALIDUM (proprietas fundi - non OMNIA) */
    _adfirma_membrum(piscina,
        "struct S { unsigned m; }; void f(struct S s)"
        " { unsigned u = s.m; }",
        "m", ZEPHYRUM, 0L, U32_SUMMUM,
        "XXV. parametri membrum -> fines u32 VALIDUM");

    imprimere("--- XXVI-XXIX: fines tractationis membrorum ---\n");

    /* XXVI. profunditas 2 invisibilis: t.in.m lectio nullum eventum
     * membri fert -> quaestio FALSUM (honeste extra scopum) */
    {
        SemanticaIntervallum iv;
        SilvaParsura* parsura = _parsare(piscina,
            "struct S { unsigned m; }; struct T { struct S in; };"
            " void f(void) { struct T t; t.in.m = 5u;"
            " { unsigned u = t.in.m; } }");
        SilvaSemantica* sem;
        constans SilvaNodus* nodus;

        imprimere("  XXVI. t.in.m -> non inventum (profunditas 2)\n");
        CREDO_NON_NIHIL(parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL(sem);
        nodus = _accessus(piscina, parsura, "m", I);
        CREDO_NON_NIHIL(nodus);
        si (nodus != NIHIL && sem != NIHIL)
        {
            memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
            CREDO_FALSUM(silva_c89_fluxus_intervallum_ad_punctum(
                sem, nodus, &iv));
        }
    }

    /* XXVII. membrum non-integrale: ordo exsistit sed OMNIA */
    {
        SemanticaIntervallum iv;
        b32 inventum;

        imprimere("  XXVII. membrum duplex -> OMNIA\n");
        memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
        inventum = _quaerere_membrum(piscina,
            "struct D { double d; }; void f(void) { struct D s;"
            " s.d = 1.5; { double x = s.d; } }",
            "d", I, &iv);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32((i32)iv.status,
            (i32)SEMANTICA_INTERVALLUM_OMNIA);
    }

    /* XXVIII. sagitta exclusa: p->m basis monstrator - nullum
     * eventum membri (pointee extra scopum v1) */
    {
        SemanticaIntervallum iv;
        SilvaParsura* parsura = _parsare(piscina,
            "struct S { unsigned m; }; void f(struct S* p)"
            " { unsigned u = p->m; }");
        SilvaSemantica* sem;
        constans SilvaNodus* nodus;

        imprimere("  XXVIII. p->m -> non inventum (sagitta)\n");
        CREDO_NON_NIHIL(parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL(sem);
        nodus = _accessus(piscina, parsura, "m", ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        si (nodus != NIHIL && sem != NIHIL)
        {
            memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
            CREDO_FALSUM(silva_c89_fluxus_intervallum_ad_punctum(
                sem, nodus, &iv));
        }
    }

    /* XXIX. campus bitorum: truncatio def-claims insana faceret -
     * ordo numquam praecisus (est_campus -> OMNIA semper) */
    {
        SemanticaIntervallum iv;
        b32 inventum;

        imprimere("  XXIX. campus bitorum -> OMNIA (non [200,200])\n");
        memset(&iv, ZEPHYRUM, magnitudo(SemanticaIntervallum));
        inventum = _quaerere_membrum(piscina,
            "struct B { unsigned bits : 3; }; void f(void)"
            " { struct B b; b.bits = 200u; { unsigned u = b.bits; } }",
            "bits", I, &iv);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32((i32)iv.status,
            (i32)SEMANTICA_INTERVALLUM_OMNIA);
    }

    imprimere("--- XXX-XXXVII: contractus intervallorum "
        "(01KYN533VY) ---\n");

    /* XXX. typedef refinatum: contractus supra typedef - parametrum
     * eo typo declaratum semen angustum accipit */
    _adfirma(piscina,
        "/* <contractus intra=\"1,16\"/> */\n"
        "typedef unsigned NBits;\n"
        "void f(NBits n) { unsigned u = n; }",
        "n", ZEPHYRUM, 1L, 16L,
        "XXX. typedef refinatum -> [1,16]");

    /* XXXI. contractus param inline (frons altera, machina eadem) */
    _adfirma(piscina,
        "/* <contractus param=\"n\" intra=\"0,255\"/> */\n"
        "void f(unsigned n) { unsigned u = n; }",
        "n", ZEPHYRUM, 0L, 255L,
        "XXXI. param intra inline -> [0,255]");

    /* XXXII. contractus solum angustat: latior typo -> typus vincit */
    _adfirma(piscina,
        "/* <contractus param=\"n\" intra=\"0,999\"/> */\n"
        "void f(unsigned char n) { unsigned u = n; }",
        "n", ZEPHYRUM, 0L, 255L,
        "XXXII. contractus latior -> fines typi vincunt");

    /* XXXIII. insignatum 64-bitorum cum contractu: fiducia trahit
     * ubi fundus nequibat (ante: OMNIA, specimen XVI) */
    _adfirma(piscina,
        "/* <contractus param=\"n\" intra=\"1,16\"/> */\n"
        "void f(unsigned long n) { unsigned long u = n; }",
        "n", ZEPHYRUM, 1L, 16L,
        "XXXIII. u64 cum contractu -> VALIDUM [1,16]");

    /* XXXIV. membrum typo refinato declaratum: semen membri
     * angustum (compositio cum ordinibus membrorum) */
    _adfirma_membrum(piscina,
        "/* <contractus intra=\"1,16\"/> */\n"
        "typedef unsigned NBits;\n"
        "struct S { NBits n; };\n"
        "void f(struct S s) { unsigned u = s.n; }",
        "n", ZEPHYRUM, 1L, 16L,
        "XXXIV. membrum typo refinato -> [1,16]");

    /* XXXIV-b. honestas effugii: post &s lectio membri ad fines
     * TYPI cadit - refinatio numquam in reservis conservativis
     * (furca sanitatis INTENTIONIS) */
    _adfirma_membrum(piscina,
        "/* <contractus intra=\"1,16\"/> */\n"
        "typedef unsigned NBits;\n"
        "struct S { NBits n; };\n"
        "void f(struct S s) { g2(&s); { unsigned u = s.n; } }",
        "n", ZEPHYRUM, 0L, U32_SUMMUM,
        "XXXIV-b. effugium basis -> fines typi (non [1,16])");

    /* XXXV. typedef sine contractu immutatum */
    _adfirma(piscina,
        "typedef unsigned U;\n"
        "void f(U n) { unsigned u = n; }",
        "n", ZEPHYRUM, 0L, U32_SUMMUM,
        "XXXV. typedef sine contractu -> fines typi");

    /* XXXVI. fluctus variabilis (extensio aestimatoris): larva
     * (1 << n) - 1 cum n intra [1,16] -> [1, 65535] */
    _adfirma(piscina,
        "/* <contractus param=\"n\" intra=\"1,16\"/> */\n"
        "void f(unsigned n) { unsigned m = (1u << n) - 1u;"
        " { unsigned w = m; } }",
        "m", ZEPHYRUM, 1L, 65535L,
        "XXXVI. (1 << [1,16]) - 1 -> [1,65535]");

    /* XXXVII. localis typo refinato NON seminatur (exclusio v1:
     * purgamentum ininitiatum in [1,16] asserere INSANUM esset) */
    _adfirma(piscina,
        "/* <contractus intra=\"1,16\"/> */\n"
        "typedef unsigned NBits;\n"
        "void f(void) { NBits v; { unsigned u = v; } }",
        "v", ZEPHYRUM, 0L, U32_SUMMUM,
        "XXXVII. localis refinatus ininitiatus -> fines typi");

    /* XXXVII-b. effugium parametri: fiducia aliasing non superest */
    _adfirma(piscina,
        "/* <contractus param=\"n\" intra=\"1,16\"/> */\n"
        "void f(unsigned n) { g2(&n); { unsigned u = n; } }",
        "n", I, 0L, U32_SUMMUM,
        "XXXVII-b. effugium parametri -> fines typi");

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
