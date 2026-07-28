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

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
