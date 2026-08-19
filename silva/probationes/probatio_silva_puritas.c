/* probatio_silva_puritas.c - puritas fontis + geometria fida
 * (quaestiones provenientiae, 01M0D4QV5S)
 *
 * SPECIMINA ANTE PRAEDICATUM (ritus formarum): exspectationes hae
 * ANTE machinam scriptae sunt.
 *
 * Lattice quaestionum duarum:
 *   est_fons_purus: lexemata OMNIA origine FONS (stratum 0).
 *     Severa - in codice latinizato sententiae fere numquam purae
 *     (si->if expansum est), sed subarbores purae geometriam
 *     ipsam ferunt: extensio relexata seriem lexematum EXACTE
 *     reddit (proprietas relexationis, sectio VII).
 *   geometria_fida: sedes radicum DISTINCTAE. Expansio 1:1 fida
 *     (lexema unum ad sedem unam veram); expansio 1:N collabitur
 *     (lexemata plura, sedes una) et mentitur - classis venationum
 *     formatoris (flatura, coloratio).
 *
 * Exspectationes per sectiones:
 *   I.   declaratio pura:        purus VERUM,  fida VERUM
 *   II.  macro 1:1 (si->if):     purus FALSUM, fida VERUM
 *   III. macro 1:N (obiectum):   purus FALSUM, fida FALSUM
 *   IV.  argumentum bis (x*x):   purus FALSUM, fida FALSUM
 *   V.   NIHIL + fons alienus:   vacue VERUM (nihil vindicatum)
 *   VII. proprietas relexationis super radices puras
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_lexema.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior SilvaParsura*
_parsare (
               Piscina* piscina,
    constans character* fons)
{
    redde silva_c89_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), NIHIL);
}

/* elementum k radicis (lista valorum) ut nodus; NIHIL si absens */
interior SilvaNodus*
_radix_nodus (
    constans SilvaParsura* parsura,
                      i32  k)
{
    SilvaValor* e;

    si (parsura == NIHIL || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    e = silva_valor_lista_obtinere(parsura->commissio->radix, k);
    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}


/* ==================================================
 * collectio lexematum subarboris (pro relexatione)
 * ================================================== */

interior vacuum
_lexemata_valoris (
    SilvaValor  v,
           Xar* effecta);

interior vacuum
_lexemata_nodi (
    constans SilvaNodus* n,
                    Xar* effecta)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        _lexemata_valoris(n->loci[k], effecta);
    }
}

interior vacuum
_lexemata_valoris (
    SilvaValor  v,
           Xar* effecta)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                SilvaToken** locus =
                    (SilvaToken**)xar_addere(effecta);

                *locus = v.datum.token;
            }
            frange;
        casus SILVA_VALOR_NODUS:
            _lexemata_nodi(v.datum.nodus, effecta);
            frange;
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    _lexemata_valoris(*elem, effecta);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

interior s32
_lexema_byte_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans SilvaToken* ta = *(SilvaToken* constans*)a;
    constans SilvaToken* tb = *(SilvaToken* constans*)b;

    si (ta->byte_offset != tb->byte_offset)
    {
        redde (ta->byte_offset < tb->byte_offset) ? -I : I;
    }
    redde ZEPHYRUM;
}

/* PROPRIETAS RELEXATIONIS: subarboris PURAE extensio relexata
 * seriem lexematum (genus + valor) exacte reddit. Oraculum
 * classificationis quod fidelitas byte-exacta praetermittit -
 * arbor male classificata bytes intactos emittere potest, sed
 * extensionem mendacem relexatio detegit. */
interior b32
_relexatio_congruit (
                Piscina* piscina,
     constans character* fons,
    constans SilvaNodus* nodus,
                    s32  fons_princeps)
{
    s32  minimum = -I;
    s32  maximum = ZEPHYRUM;
    Xar* arboris;
    Xar* relexata;
    i32  numerus_arboris;
    i32  numerus_relexatae;
    i32  k;

    silva_nodus_extensionem(nodus, fons_princeps, &minimum,
        &maximum);
    si (minimum < (s32)ZEPHYRUM || maximum <= minimum)
    {
        redde FALSUM;
    }
    arboris = xar_creare(piscina, (i32)magnitudo(SilvaToken*));
    _lexemata_nodi(nodus, arboris);
    xar_ordinare(arboris, _lexema_byte_comparare);

    relexata = silva_lexare(piscina, fons + minimum,
        (i32)(maximum - minimum), ZEPHYRUM);
    si (relexata == NIHIL)
    {
        redde FALSUM;
    }
    numerus_arboris    = xar_numerus(arboris);
    numerus_relexatae  = xar_numerus(relexata);
    /* EOF ultimum relexatae non numeratur */
    si (   numerus_relexatae < I
        || numerus_arboris != numerus_relexatae - I)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus_arboris; k++)
    {
        constans SilvaToken* arboreum =
            *(SilvaToken**)xar_obtinere(arboris, k);
        constans SilvaToken* relexatum =
            *(SilvaToken**)xar_obtinere(relexata, k);

        si (arboreum->genus != relexatum->genus)
        {
            redde FALSUM;
        }
        si (   arboreum->valor.mensura != relexatum->valor.mensura
            || (arboreum->valor.mensura > (i32)ZEPHYRUM
                && memcmp(arboreum->valor.datum,
                       relexatum->valor.datum,
                       arboreum->valor.mensura) != ZEPHYRUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

integer
principale (vacuum)
{
    Piscina* piscina;

    imprimere("\n========================================\n");
    imprimere("PROBATIO SILVA PURITAS (provenientia)\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_puritas",
        16777216);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("--- I: declaratio pura ---\n");
    {
        SilvaParsura* parsura = _parsare(piscina, "int x;\n");
          SilvaNodus* nodus;

        CREDO_NON_NIHIL(parsura);
        CREDO_VERUM(parsura->successus);
        nodus = _radix_nodus(parsura, ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        CREDO_VERUM(silva_nodus_est_fons_purus(nodus,
            parsura->fons_princeps));
        CREDO_VERUM(silva_nodus_geometria_fida(piscina, nodus,
            parsura->fons_princeps));
        /* fons quilibet (-1): eadem verdicta */
        CREDO_VERUM(silva_nodus_est_fons_purus(nodus, -I));
        CREDO_VERUM(silva_nodus_geometria_fida(piscina, nodus,
            -I));
    }

    imprimere("--- II: macro 1:1 (si->if) - impurum sed fidum ---\n");
    {
        SilvaParsura* parsura = _parsare(piscina,
            "#define si if\n"
            "void f(int a) { si (a) { a = 1; } }\n");
        SilvaNodus* nodus;

        CREDO_NON_NIHIL(parsura);
        CREDO_VERUM(parsura->successus);
        nodus = _radix_nodus(parsura, ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        CREDO_FALSUM(silva_nodus_est_fons_purus(nodus,
            parsura->fons_princeps));
        CREDO_VERUM(silva_nodus_geometria_fida(piscina, nodus,
            parsura->fons_princeps));
    }

    imprimere("--- III: macro 1:N (obiectum) - collapsus ---\n");
    {
        SilvaParsura* parsura = _parsare(piscina,
            "#define SUMMA (1 + 2)\n"
            "int y = SUMMA;\n");
        SilvaNodus* nodus;

        CREDO_NON_NIHIL(parsura);
        CREDO_VERUM(parsura->successus);
        nodus = _radix_nodus(parsura, ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        CREDO_FALSUM(silva_nodus_est_fons_purus(nodus,
            parsura->fons_princeps));
        CREDO_FALSUM(silva_nodus_geometria_fida(piscina, nodus,
            parsura->fons_princeps));
    }

    imprimere("--- IV: argumentum bis adhibitum - collapsus ---\n");
    {
        SilvaParsura* parsura = _parsare(piscina,
            "#define GEMINUS(x) ((x) * (x))\n"
            "int z = GEMINUS(3);\n");
        SilvaNodus* nodus;

        CREDO_NON_NIHIL(parsura);
        CREDO_VERUM(parsura->successus);
        nodus = _radix_nodus(parsura, ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        CREDO_FALSUM(silva_nodus_est_fons_purus(nodus,
            parsura->fons_princeps));
        CREDO_FALSUM(silva_nodus_geometria_fida(piscina, nodus,
            parsura->fons_princeps));
    }

    imprimere("--- V: NIHIL + fons alienus - vacue verum ---\n");
    {
        SilvaParsura* parsura = _parsare(piscina, "int x;\n");
          SilvaNodus* nodus;

        CREDO_VERUM(silva_nodus_est_fons_purus(NIHIL, ZEPHYRUM));
        CREDO_VERUM(silva_nodus_geometria_fida(piscina, NIHIL,
            ZEPHYRUM));
        CREDO_NON_NIHIL(parsura);
        nodus = _radix_nodus(parsura, ZEPHYRUM);
        CREDO_NON_NIHIL(nodus);
        /* fons alienus: purus FALSUM (lexemata alibi), fida VERUM
         * (nihil in plagula illa vindicatum) */
        CREDO_FALSUM(silva_nodus_est_fons_purus(nodus,
            parsura->fons_princeps + I));
        CREDO_VERUM(silva_nodus_geometria_fida(piscina, nodus,
            parsura->fons_princeps + I));
    }

    imprimere("--- VII: proprietas relexationis ---\n");
    {
        constans character* fons =
            "int a;\n"
            "int b = 5;\n"
            "#define TRIA (1 + 2)\n"
            "int c = TRIA;\n"
            "void f(int p) { return; }\n";
        SilvaParsura* parsura = _parsare(piscina, fons);
                 i32  m;
                 i32  k;
                 i32  purae     = ZEPHYRUM;
                 i32  congruae  = ZEPHYRUM;

        CREDO_NON_NIHIL(parsura);
        CREDO_VERUM(parsura->successus);
        m = (i32)silva_valor_lista_numerus(
            parsura->commissio->radix);
        per (k = ZEPHYRUM; k < m; k++)
        {
            SilvaNodus* nodus = _radix_nodus(parsura, k);

            si (nodus == NIHIL)
            {
                perge;
            }
            si (silva_nodus_est_fons_purus(nodus,
                    parsura->fons_princeps))
            {
                purae = purae + I;
                si (_relexatio_congruit(piscina, fons, nodus,
                        parsura->fons_princeps))
                {
                    congruae = congruae + I;
                }
            }
        }
        /* a, b, f purae (c impura - TRIA); omnes purae congruunt */
        CREDO_AEQUALIS_I32(purae, (i32)3);
        CREDO_AEQUALIS_I32(congruae, purae);
    }

    credo_imprimere_compendium();
    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde omnia ? ZEPHYRUM : I;
    }
}
