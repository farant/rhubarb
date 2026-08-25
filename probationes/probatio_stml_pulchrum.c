/* probatio_stml_pulchrum.c - Portae M2 modi pulchri
 *
 * PORTA A - PUNCTUM FIXUM (spec triviae §5):
 *   pulchrum(legere(pulchrum(x))) == pulchrum(x) octetim.
 * PORTA B - AEQUALITAS ARBORUM (spec triviae §7.7):
 *   legere(pulchrum(x)) arbori legere(x) congruit modulo trivia
 *   (spatia_*, indentatio) et campos formae (captio_*,
 *   clausura_anonyma, attributi spatia_ante) - pulcher octetos
 *   movet, sensum numquam.
 *
 * CORPUS: viae ex probationes/fixa/stml_aurea/internus.aurum
 * (lineae 'F' - idem corpus quo pons aureus) + litterae authoratae
 * infra. Custodia teguminis: corpus vacuum RUBET (porta silens
 * ≡ porta mortua).
 *
 * NATA ANTE CHIRURGIAM (M2 T1, 2026-08-24): rubra prima = tabula
 * operum M2 (classes vitiorum TERMINI), non defectus probationis.
 */
#include "latina.h"
#include "stml.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

#define AUREUM_VIA "probationes/fixa/stml_aurea/internus.aurum"

/* Litterae authoratae - casus dispositionis interessantes; quisque
 * hodie parsat. */
nomen structura {
    constans character* titulus;
    constans character* fons;
} LitteraPulchra;

interior constans LitteraPulchra LITTERAE_PULCHRAE[] = {
    { "mixtus_inline",
      "<p>salve <b>munde</b> iterum</p>" },
    { "sep_spatia",
      "<sep>   </sep>" },
    { "sep_inter_elementa",
      "<radix><a/>   <b/></radix>" },
    { "vacuum_linea",
      "<a>\n</a>" },
    { "spatia_ubique",
      "<radix>\n  <a/>\n  <b>x</b>\n</radix>\n" },
    { "spina_nidificata",
      "<t1>\n  <t2>\n    <t3>\n      foo\n    </>\n  </>\n</t1>" },
    { "commentum_inter",
      "<radix>ante<!-- c -->post</radix>" },
    { "commentum_bloci",
      "<radix>\n  <!-- nota -->\n  <a/>\n</radix>" },
    { "entia",
      "<a>salve &lt;mundus&gt; &amp; amici</a>" },
    { "captio_ante_recta",
      "<radix><a (><b/><c/></radix>" },
    { "captio_retro",
      "<radix><b/>\n<) a></radix>" },
    { "farcimen",
      "<radix><x/>\n<= a =>\n<y/></radix>" },
    { "cruda_linea",
      "<radix><imp! (>crudus textus\n<post/></radix>" },
    { "crudus_plenus",
      "<code!>a < b && c > d</code>" },
    { "multilinea_versus",
      "<versus\\>\n  prima\n    altior\n  ultima\n</>" },
    { "attributa_multilinea",
      "<a\n  b=\"1\"\n  c=\"2\">x</a>" },
    { "processio_cauda",
      "<?xml version=\"1.0\"?>\n<radix><a/></radix>\n" },
    { "fragmentum",
      "<#pars attr=\"1\"><a/>textus</#>" },
    { "collapsus_auctoris",
      "<radix><t1(> <t2(> foo</radix>" },
    { "collapsus_mixtus",
      "<radix>\n  <par(> <filius/>\n  <b>x</b>\n</radix>" },
    { "verticalis_exemplar",
      "<a><b><c><d/><e/></c></b></a>" },
    { "verticalis_stackata",
      "<a(> <b(>\n  <c>\n    <d/>\n    <e/>\n  </>" },
    { "verticalis_auctoris",
      "<a (>\n<x/>" },
    { "verticalis_textus_multilineus",
      "<a><b><t>prima\nsecunda</t></b></a>" },
    { "verticalis_ultra_tectum",
      "<vinculumprimum><vinculumsecundum><vinculumtertium>"
      "<vinculumquartum>foo</vinculumquartum></vinculumtertium>"
      "</vinculumsecundum></vinculumprimum>" },
    { "lineae_vacuae",
      "<radix>\n  <a/>\n\n  <!-- nota -->\n  <b/>\n</radix>" },
    { "lineae_vacuae_multae",
      "<radix>\n\n  <a/>\n\n\n\n  <b/>\n\n</radix>" },
    { "lineae_vacuae_documenti",
      "<?xml version=\"1.0\"?>\n\n<radix><a/><b/></radix>" },
    { "refluxus_plenus",
      "<t>aaaa bbbb cccc dddd eeee ffff gggg hhhh\n"
      "iiii jjjj kkkk llll mmmm nnnn oooo pppp\n"
      "qqqq rrrr ssss tttt</t>" },
    { "refluxus_marginis_sordidi",
      "<radix>\n  <t>foo </t>\n  <u/>\n</radix>" }
};

/* Plagulam totam in piscinam legere */
interior b32
_fasciculum_legere (
    constans character* via,
               Piscina* piscina,
                chorda* exitus)
{
      FILE* f;
    longus  mensura;
        i8* buffer;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }

    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }
    mensura = ftell(f);
    si (mensura < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    si (fseek(f, 0L, SEEK_SET) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    si (mensura == 0L)
    {
        fclose(f);
        redde VERUM;
    }

    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (buffer == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    si (fread(buffer, I, (size_t)mensura, f) != (size_t)mensura)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);

    exitus->datum    = buffer;
    exitus->mensura  = (i32)mensura;
    redde VERUM;
}

/* Vias fontium ex aureo commisso extrahere (lineae 'F <via> ...') */
interior i32
_vias_ex_aureo (
                 chorda    aureum,
                Piscina*   piscina,
     constans character*** viae_exitus)
{
     constans character** viae;
                    i32   numerus;
                    i32   i;
                    i32   transitus;

    numerus  = ZEPHYRUM;
    viae     = NIHIL;

    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        i32 n;

        n = ZEPHYRUM;
        per (i = ZEPHYRUM; i < aureum.mensura; i++)
        {
            b32 initium_lineae;

            initium_lineae = (b32)(i == ZEPHYRUM
                || (character)aureum.datum[i - I] == '\n');
            si (   initium_lineae
                && i + I < aureum.mensura
                && (character)aureum.datum[i]     == 'F'
                && (character)aureum.datum[i + I] == ' ')
            {
                si (transitus == I)
                {
                          i32  initium;
                          i32  finis;
                    character* copia;

                    initium  = i + II;
                    finis    = initium;
                    dum (   finis < aureum.mensura
                         && (character)aureum.datum[finis] != ' '
                         && (character)aureum.datum[finis] != '\n')
                    {
                        finis++;
                    }
                    copia = (character*)piscina_allocare(piscina,
                        (memoriae_index)(finis - initium + I));
                    si (copia != NIHIL)
                    {
                        memcpy(copia, aureum.datum + initium,
                               (size_t)(finis - initium));
                        copia[finis - initium]  = '\0';
                        viae[n]                 = copia;
                    }
                }
                n++;
            }
        }

        si (transitus == ZEPHYRUM)
        {
            numerus = n;
            si (numerus == ZEPHYRUM)
            {
                frange;
            }
            viae = (constans character**)piscina_allocare(piscina,
                (memoriae_index)numerus
                    * (memoriae_index)magnitudo(character*));
            si (viae == NIHIL)
            {
                redde ZEPHYRUM;
            }
        }
    }

    *viae_exitus = viae;
    redde numerus;
}


/* ==================================================
 * Porta B - comparatio arborum modulo trivia + formam
 * ================================================== */

interior b32
_veritas (
    b32 x)
{
    redde (x != ZEPHYRUM) ? VERUM : FALSUM;
}

/* NIHIL ≡ chorda vacua */
interior b32
_chordae_pares (
    constans chorda* a,
    constans chorda* b)
{
    i32 ma;
    i32 mb;

    ma = (a != NIHIL) ? a->mensura : ZEPHYRUM;
    mb = (b != NIHIL) ? b->mensura : ZEPHYRUM;
    si (ma != mb)
    {
        redde FALSUM;
    }
    si (ma == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(a->datum, b->datum, (size_t)ma) == ZEPHYRUM);
}

/* aequalitas valorum modulo fluxum (§4 re-involutio, M3): in
 * textu FLUMINIS cursus albi lineam ferentes utrimque spatio UNI
 * aequivalent - pulcher prosam possidet, octetos movet, fluxum
 * numquam. Ceteri octeti exacti; genera marcata ('\'/'!') hac via
 * numquam veniunt (byte-aequalitas manet). */
interior b32
_est_albus_octetus (
    character c)
{
    redde (b32)(c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

interior s32
_fluxu_normatum (
    constans chorda* s,
                i32* i)
{
    si (*i >= s->mensura)
    {
        redde -I;
    }
    si (_est_albus_octetus((character)s->datum[*i]))
    {
        i32 finis;
        b32 fert;

        finis  = *i;
        fert   = FALSUM;
        dum (   finis < s->mensura
             && _est_albus_octetus((character)s->datum[finis]))
        {
            si ((character)s->datum[finis] == '\n')
            {
                fert = VERUM;
            }
            finis++;
        }
        si (fert)
        {
            *i = finis;
            redde (s32)' ';
        }
    }
    {
        s32 c;

        c = (s32)s->datum[*i];
        (*i)++;
        redde c;
    }
}

interior b32
_fluxu_pares (
    constans chorda* a,
    constans chorda* b)
{
    i32 ia;
    i32 ib;

    si (a == NIHIL || b == NIHIL)
    {
        redde (b32)(a == b);
    }
    ia = ZEPHYRUM;
    ib = ZEPHYRUM;
    dum (VERUM)
    {
        s32 ca;
        s32 cb;

        ca = _fluxu_normatum(a, &ia);
        cb = _fluxu_normatum(b, &ib);
        si (ca != cb)
        {
            redde FALSUM;
        }
        si (ca == -I)
        {
            redde VERUM;
        }
    }
}

interior b32
_textus_fluminis (
    constans StmlNodus* n)
{
    redde (b32)(n->genus == STML_NODUS_TEXTUS
        && (   n->parens == NIHIL
            || (!n->parens->crudus && !n->parens->multilinea)));
}

interior vacuum
_incongruentiam_nuntiare (
    constans  character* campus,
    constans  StmlNodus* a,
    constans  StmlNodus* b,
                    i32  profunditas)
{
    imprimere("    INCONGRUENTIA (%s) ad profunditatem %lu:\n",
        campus, (insignatus longus)profunditas);
    si (a != NIHIL && a->titulus != NIHIL)
    {
        imprimere("      a: <%.*s> genus %lu\n",
            (integer)a->titulus->mensura,
            (constans character*)a->titulus->datum,
            (insignatus longus)a->genus);
    }
    alioquin si (a != NIHIL)
    {
        imprimere("      a: (sine titulo) genus %lu\n",
            (insignatus longus)a->genus);
    }
    si (b != NIHIL && b->titulus != NIHIL)
    {
        imprimere("      b: <%.*s> genus %lu\n",
            (integer)b->titulus->mensura,
            (constans character*)b->titulus->datum,
            (insignatus longus)b->genus);
    }
    alioquin si (b != NIHIL)
    {
        imprimere("      b: (sine titulo) genus %lu\n",
            (insignatus longus)b->genus);
    }
}

interior b32
_arbores_congruae (
    constans StmlNodus* a,
    constans StmlNodus* b,
                   i32  profunditas)
{
    i32 num_a;
    i32 num_b;
    i32 i;

    si (a == NIHIL && b == NIHIL)
    {
        redde VERUM;
    }
    si (a == NIHIL || b == NIHIL)
    {
        _incongruentiam_nuntiare("nodus absens", a, b, profunditas);
        redde FALSUM;
    }
    si (a->genus != b->genus)
    {
        _incongruentiam_nuntiare("genus", a, b, profunditas);
        redde FALSUM;
    }
    si (!_chordae_pares(a->titulus, b->titulus))
    {
        _incongruentiam_nuntiare("titulus", a, b, profunditas);
        redde FALSUM;
    }
    si (   !_chordae_pares(a->valor, b->valor)
        && !(   _textus_fluminis(a)
             && _textus_fluminis(b)
             && _fluxu_pares(a->valor, b->valor)))
    {
        _incongruentiam_nuntiare("valor", a, b, profunditas);
        si (a->valor != NIHIL)
        {
            imprimere("      valor a (%lu oct.): \"%.*s\"\n",
                (insignatus longus)a->valor->mensura,
                (integer)a->valor->mensura,
                (constans character*)a->valor->datum);
        }
        si (b->valor != NIHIL)
        {
            imprimere("      valor b (%lu oct.): \"%.*s\"\n",
                (insignatus longus)b->valor->mensura,
                (integer)b->valor->mensura,
                (constans character*)b->valor->datum);
        }
        redde FALSUM;
    }
    si (_veritas(a->crudus) != _veritas(b->crudus))
    {
        _incongruentiam_nuntiare("crudus", a, b, profunditas);
        redde FALSUM;
    }
    si (_veritas(a->multilinea) != _veritas(b->multilinea))
    {
        _incongruentiam_nuntiare("multilinea", a, b, profunditas);
        redde FALSUM;
    }
    si (_veritas(a->fragmentum) != _veritas(b->fragmentum))
    {
        _incongruentiam_nuntiare("fragmentum", a, b, profunditas);
        redde FALSUM;
    }
    si (!_chordae_pares(a->fragmentum_id, b->fragmentum_id))
    {
        _incongruentiam_nuntiare("fragmentum_id", a, b, profunditas);
        redde FALSUM;
    }
    si (!_chordae_pares(a->augmentum_clavis, b->augmentum_clavis))
    {
        _incongruentiam_nuntiare("augmentum_clavis", a, b,
                                 profunditas);
        redde FALSUM;
    }

    /* attributa: numerus + tituli/valores ordine (spatia_ante
     * attributi = trivia, ignoratur) */
    num_a = (a->attributa != NIHIL)
        ? xar_numerus(a->attributa) : ZEPHYRUM;
    num_b = (b->attributa != NIHIL)
        ? xar_numerus(b->attributa) : ZEPHYRUM;
    si (num_a != num_b)
    {
        _incongruentiam_nuntiare("numerus attributorum", a, b,
                                 profunditas);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < num_a; i++)
    {
        StmlAttributum* at_a;
        StmlAttributum* at_b;

        at_a = (StmlAttributum*)xar_obtinere(a->attributa, i);
        at_b = (StmlAttributum*)xar_obtinere(b->attributa, i);
        si (   at_a == NIHIL || at_b == NIHIL
            || !_chordae_pares(at_a->titulus, at_b->titulus)
            || !_chordae_pares(at_a->valor, at_b->valor))
        {
            _incongruentiam_nuntiare("attributum", a, b,
                                     profunditas);
            redde FALSUM;
        }
    }

    /* liberi: numerus + recursio */
    num_a = (a->liberi != NIHIL) ? xar_numerus(a->liberi) : ZEPHYRUM;
    num_b = (b->liberi != NIHIL) ? xar_numerus(b->liberi) : ZEPHYRUM;
    si (num_a != num_b)
    {
        _incongruentiam_nuntiare("numerus liberorum", a, b,
                                 profunditas);
        imprimere("      a: %lu liberi, b: %lu liberi\n",
            (insignatus longus)num_a, (insignatus longus)num_b);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < num_a; i++)
    {
        StmlNodus** cella_a;
        StmlNodus** cella_b;

        cella_a = (StmlNodus**)xar_obtinere(a->liberi, i);
        cella_b = (StmlNodus**)xar_obtinere(b->liberi, i);
        si (!_arbores_congruae(
                (cella_a != NIHIL) ? *cella_a : NIHIL,
                (cella_b != NIHIL) ? *cella_b : NIHIL,
                profunditas + I))
        {
            imprimere("      (via: liberum %lu)\n",
                (insignatus longus)i);
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Portae ambae super fontem unum
 * ================================================== */

interior vacuum
_divergentiam_octetorum_imprimere (
    chorda p1,
    chorda p2)
{
    i32 minima;
    i32 primus;
    i32 i;
    i32 initium;
    i32 finis1;
    i32 finis2;

    minima = (p1.mensura < p2.mensura) ? p1.mensura : p2.mensura;
    primus = minima;
    per (i = ZEPHYRUM; i < minima; i++)
    {
        si (p1.datum[i] != p2.datum[i])
        {
            primus = i;
            frange;
        }
    }

    initium  = (primus > XL) ? primus - XL : ZEPHYRUM;
    finis1   = (primus + XL < p1.mensura) ? primus + XL : p1.mensura;
    finis2   = (primus + XL < p2.mensura) ? primus + XL : p2.mensura;

    imprimere("    DIVERGENTIA ad octetum %lu "
              "(mensurae %lu / %lu):\n",
        (insignatus longus)primus,
        (insignatus longus)p1.mensura,
        (insignatus longus)p2.mensura);
    imprimere("      p1: \"%.*s\"\n",
        (integer)(finis1 - initium),
        (constans character*)(p1.datum + initium));
    imprimere("      p2: \"%.*s\"\n",
        (integer)(finis2 - initium),
        (constans character*)(p2.datum + initium));
}

/* recusatio_licet: corpus plagulas recusantes fert (WORKSHOP -
 * generatio praecedens; aureum 'successus:0' testatur et mutationem
 * recusationis vigilat) - praetermissio CLARA, non silens. Litterae
 * parsare DEBENT. */
interior vacuum
_portas_probare (
    constans character* titulus,
                chorda  fons,
               Piscina* piscina,
                   b32  recusatio_licet)
{
     InternamentumChorda* intern;
            StmlResultus  r1;
            StmlResultus  r2;
                  chorda  p1;
                  chorda  p2;
                     b32  punctum_fixum;
                     b32  congruens;

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        imprimere("  FRACTA: internamentum (%s)\n", titulus);
        CREDO_VERUM (FALSUM);
        redde;
    }

    r1 = stml_legere(fons, piscina, intern);
    si (!r1.successus)
    {
        si (recusatio_licet)
        {
            imprimere("  PRAETERMISSUM (parsatio recusata): %s\n",
                      titulus);
        }
        alioquin
        {
            imprimere("  PARSATIO FRACTA: %s\n", titulus);
            CREDO_VERUM (FALSUM);
        }
        redde;
    }

    p1 = stml_scribere(r1.radix, piscina, VERUM);

    r2 = stml_legere(p1, piscina, intern);
    si (!r2.successus)
    {
        imprimere("  RELECTIO PULCHRI FRACTA: %s\n", titulus);
        imprimere("    emissum: \"%.*s\"\n",
            (integer)((p1.mensura < LXXX) ? p1.mensura : LXXX),
            (constans character*)p1.datum);
        CREDO_VERUM (FALSUM);
        redde;
    }

    /* PORTA A - punctum fixum */
    p2 = stml_scribere(r2.radix, piscina, VERUM);
    punctum_fixum = (b32)(p1.mensura == p2.mensura
        && (p1.mensura == ZEPHYRUM
            || memcmp(p1.datum, p2.datum, (size_t)p1.mensura)
                == ZEPHYRUM));
    si (!punctum_fixum)
    {
        imprimere("  PUNCTUM FIXUM FRACTUM: %s\n", titulus);
        _divergentiam_octetorum_imprimere(p1, p2);
    }
    CREDO_VERUM (punctum_fixum);

    /* PORTA B - aequalitas arborum modulo trivia + formam */
    congruens = _arbores_congruae(r1.radix, r2.radix, ZEPHYRUM);
    si (!congruens)
    {
        imprimere("  ARBORES INCONGRUAE: %s\n", titulus);
    }
    CREDO_VERUM (congruens);
}

s32
principale (vacuum)
{
    Piscina* piscina;
        i32  i;
        i32  numerus_litterarum;

    piscina = piscina_generare_dynamicum("probatio_stml_pulchrum",
                                         4194304);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Litterae authoratae
     * ================================================== */

    imprimere("\n--- Probans litteras (portae A + B) ---\n");
    numerus_litterarum = (i32)(magnitudo(LITTERAE_PULCHRAE)
        / magnitudo(LITTERAE_PULCHRAE[ZEPHYRUM]));
    per (i = ZEPHYRUM; i < numerus_litterarum; i++)
    {
         Piscina* p;
          chorda  fons;

        p = piscina_generare_dynamicum("pulchrum_littera", 1048576);
        CREDO_NON_NIHIL (p);
        si (p == NIHIL)
        {
            perge;
        }
        imprimere("  littera: %s\n", LITTERAE_PULCHRAE[i].titulus);
        fons = chorda_ex_literis(LITTERAE_PULCHRAE[i].fons, p);
        _portas_probare(LITTERAE_PULCHRAE[i].titulus, fons, p,
                        FALSUM);
        piscina_destruere(p);
    }


    /* ==================================================
     * Corpus (viae ex aureo)
     * ================================================== */

    imprimere("\n--- Probans corpus (portae A + B) ---\n");
    {
                        chorda   aureum;
            constans character** viae;
                           i32   numerus_viarum;

        si (!_fasciculum_legere(AUREUM_VIA, piscina, &aureum))
        {
            imprimere("  FRACTA: aureum abest (%s) - corpus sine "
                      "indice\n", AUREUM_VIA);
            CREDO_VERUM (FALSUM);
        }
        alioquin
        {
            viae            = NIHIL;
            numerus_viarum  = _vias_ex_aureo(aureum, piscina, &viae);

            /* custodia teguminis: corpus vacuum = porta mortua */
            CREDO_VERUM (numerus_viarum > ZEPHYRUM);
            imprimere("  corpus: %lu plagulae\n",
                (insignatus longus)numerus_viarum);

            per (i = ZEPHYRUM; i < numerus_viarum; i++)
            {
                 Piscina* p;
                  chorda  fons;

                p = piscina_generare_dynamicum("pulchrum_plagula",
                                               4194304);
                CREDO_NON_NIHIL (p);
                si (p == NIHIL)
                {
                    perge;
                }
                si (!_fasciculum_legere(viae[i], p, &fons))
                {
                    imprimere("  FRACTA: legere %s\n", viae[i]);
                    CREDO_VERUM (FALSUM);
                    piscina_destruere(p);
                    perge;
                }
                _portas_probare(viae[i], fons, p, VERUM);
                piscina_destruere(p);
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
