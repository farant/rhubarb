/* probatio_oratio_vocabula.c - Recensio vocabulorum (T10)
 *
 * I.   SCISSIO identificatorum: '_' et limes minuscula->CAPITALIS
 *      (piscina_allocare, MateriaNodus, STMLNodus, ORATIO_PARS_N,
 *      _capsula_forum_data, utf8_legere, s32 omissum, x omissum);
 *      sedes et sedes prima ex tabula nexus inlineari.
 * II.  COMMENTARIA plagulae C inlinearis per silvam et orationem:
 *      verba, lineae (commentum clausum multilineare, commentum lineae).
 * III. IUDICIUM cum tabula et glossario: notum (piscina), notum per
 *      glossarium (est -> sum verbum), permissum (offset buffer index),
 *      ambiguum (virum: lemmata III), ignotum (the xyzzy).
 * IV.  CORPUS: nexus.tsv symbola + commentaria corporis C (corpus_c.txt):
 *      numeri publicati, ignota prima XV, limina sanitatis (verba > V
 *      milia, nota + permissa + ambigua > tertia pars) - relatio, non
 *      pinna: numerus ignotorum 'solum cadens' post relationem lectam.
 * Culpa plantata: limes CAPITALIS in scissione sublatus (MateriaNodus
 * integer manet).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_vocabula.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IGNOTA_SYMBOLORUM_PINNA 3022

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior chorda
_l (
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;
    chorda c;

    u.c        = literae;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literae);
    redde c;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

interior constans OratioVerbum*
_verbum (
    constans OratioVocabula* vc,
         constans character* verbum)
{
    Xar* verba = oratio_vocabula_verba(vc);
    i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(verba); i++)
    {
        constans OratioVerbum* v =
            (constans OratioVerbum*)xar_obtinere(verba, i);

        si (_aequalis(v->verbum, verbum))
        {
            redde v;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix;
                character  via[1024];
                   chorda  tabula;
                   chorda  fons_glossarii;
     OratioVocabulariumLa* voc;
 OratioVocabulariumVitium vitium;
           OratioVocabula* vc;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_vocabula",
        536870912);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &tabula));
    voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
    CREDO_NON_NIHIL (voc);
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fons_glossarii));
    si (voc == NIHIL || fons_glossarii.datum == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    {
        OratioGlossarium* gl = oratio_glossarium_legere(piscina,
            fons_glossarii,
            &vitium);

        CREDO_NON_NIHIL (gl);
        oratio_vocabularium_la_glossarium_ponere(voc, gl);
    }

    imprimere("\n--- I. Scissio identificatorum ---\n");
    vc = oratio_vocabula_creare(piscina, voc);
    CREDO_NON_NIHIL (vc);
        CREDO_VERUM (oratio_vocabula_symbola(vc, _l(
            "# caput\n"
            "knotapel_res\tsedes\tfunctio\tknotapel/demo_01/main.c\t1\t1\t0\n"

            "piscina_allocare\tsedes\tfunctio\tlib/piscina.c\t12\t1\t0\n"
            "MateriaNodus\tsedes\ttypedef\tmateria/fontes/materia_nodus.h\t40\t1\t0\n"
            "STMLNodus\tsedes\ttypedef\tinclude/stml.h\t9\t1\t0\n"
            "ORATIO_PARS_N\tsedes\tconstans\toratio/fontes/oratio_vocabularium.h\t50\t5\t0\n"
            "_capsula_forum_data\tsedes\tvariabile\tapps/forum/capsula.c\t5\t24\t0\n"
            "utf8_legere\tsedes\tfunctio\tlib/utf8.c\t30\t1\t0\n"
            "s32\tsedes\ttypedef\tinclude/latina.h\t3\t1\t0\n"
            "x\tsedes\tvariabile\tlib/piscina.c\t99\t1\t0\n"
            "piscina\tvocatio\tfunctio\tlib/alia.c\t1\t1\t0\n"
                "piscina_destruere\tsedes\tfunctio\tlib/piscina.c\t80\t1\t0\n"),
            ORATIO_VOCABULA_EXCLUSA));

    {
                          Xar* verba = oratio_vocabula_verba(vc);
        constans OratioVerbum* v;

        imprimere("  verba %d sedes %d\n", (integer)xar_numerus(verba),
            (integer)oratio_vocabula_sedes(vc));
        CREDO_AEQUALIS_I32 (xar_numerus(verba), (i32)XIII);
        /* piscina allocare materia nodus stml oratio pars capsula forum
         * data utf8 legere destruere */
                CREDO_AEQUALIS_I32 (oratio_vocabula_sedes(vc), (i32)XV);

        v = _verbum(vc, "piscina");
        CREDO_NON_NIHIL (v);
        CREDO_AEQUALIS_I32 (v->sedes, (i32)II);
        CREDO_AEQUALIS_I32 (v->sedes_symbolorum, (i32)II);
        CREDO_VERUM (_aequalis(v->via_prima, "lib/piscina.c"));
        CREDO_AEQUALIS_I32 (v->linea_prima, (i32)XII);
        CREDO_FALSUM (v->ex_commento_prima);
        CREDO_NON_NIHIL (_verbum(vc, "materia"));
        CREDO_NON_NIHIL (_verbum(vc, "nodus"));
        CREDO_NIHIL (_verbum(vc, "materianodus"));
        CREDO_NON_NIHIL (_verbum(vc, "stml"));
        CREDO_NON_NIHIL (_verbum(vc, "oratio"));
        CREDO_NON_NIHIL (_verbum(vc, "pars"));
        CREDO_NIHIL (_verbum(vc, "n"));
        CREDO_NON_NIHIL (_verbum(vc, "capsula"));
        CREDO_NON_NIHIL (_verbum(vc, "utf8"));
        CREDO_NIHIL (_verbum(vc, "s32"));
        CREDO_NIHIL (_verbum(vc, "x"));
                CREDO_NIHIL (_verbum(vc, "vocatio"));
        CREDO_NIHIL (_verbum(vc, "knotapel"));   /* via exclusa */

        v = _verbum(vc, "nodus");
        CREDO_VERUM (v != NIHIL && _aequalis(v->via_prima,
            "materia/fontes/materia_nodus.h")
                && v->linea_prima == (i32)40);
    }

    imprimere("\n--- II. Commentaria per silvam et orationem ---\n");
    CREDO_VERUM (oratio_vocabula_commenta(vc, _l(
        "/* Piscinam allocare; the buffer est offset.\n"
        " * iterum piscinam, virum. */\n"
        "int x; /* lineae tertiae verbum */\n"
        "int y;\n"), "lib/probatio.c"));
    {
        constans OratioVerbum* v;

        v = _verbum(vc, "piscinam");
        CREDO_NON_NIHIL (v);
        CREDO_AEQUALIS_I32 (v->sedes, (i32)II);
        CREDO_AEQUALIS_I32 (v->sedes_commentorum, (i32)II);
        CREDO_VERUM (_aequalis(v->via_prima, "lib/probatio.c"));
        CREDO_AEQUALIS_I32 (v->linea_prima, I);
        CREDO_VERUM (v->ex_commento_prima);
        v = _verbum(vc, "iterum");
        CREDO_VERUM (v != NIHIL && v->linea_prima == (i32)II);
        v = _verbum(vc, "tertiae");
        CREDO_VERUM (v != NIHIL && v->linea_prima == (i32)III);
        CREDO_NON_NIHIL (_verbum(vc, "the"));
        CREDO_NON_NIHIL (_verbum(vc, "buffer"));
        CREDO_NON_NIHIL (_verbum(vc, "virum"));
        CREDO_NIHIL (_verbum(vc, "int"));   /* codex, non commentum */
        v = _verbum(vc, "allocare");
        CREDO_VERUM (v != NIHIL && v->sedes_symbolorum == I
            && v->sedes_commentorum == I);
    }

    imprimere("\n--- III. Iudicium ---\n");
    CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("xyzzy"),
        "lib/x.c", I,
        FALSUM));
    CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("Index"),
        "lib/x.c", (i32)II,
        FALSUM));
    CREDO_VERUM (oratio_vocabula_iudicare(vc));
    {
        constans OratioVerbum* v;

        v = _verbum(vc, "piscina");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "N"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "piscina"));
        v = _verbum(vc, "est");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_AMBIGUUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "verbum"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "sum"));
        CREDO_VERUM (v != NIHIL && v->lemmata == (i32)II);   /* sum, edo */
        v = _verbum(vc, "offset");
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);
        v = _verbum(vc, "buffer");
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);
        v = _verbum(vc, "index");
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);
        v = _verbum(vc, "virum");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_AMBIGUUM);
                CREDO_VERUM (v != NIHIL && v->lemmata >= (i32)III);   /* vir virus virum vis */

        v = _verbum(vc, "the");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_IGNOTUM);
        CREDO_VERUM (v != NIHIL && v->analyses == ZEPHYRUM);
        v = _verbum(vc, "xyzzy");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_IGNOTUM);
        imprimere("  nota %d ambigua %d permissa %d ignota %d\n",
            (integer)oratio_vocabula_numerus(vc, ORATIO_VERBUM_NOTUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_AMBIGUUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_PERMISSUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_IGNOTUM));
                CREDO_AEQUALIS_I32 (oratio_vocabula_numerus(vc,
                    ORATIO_VERBUM_PERMISSUM),
                    (i32)V);   /* offset buffer index utf8 stml */

        /* ordo: sedes descendentes */
        {
            Xar* ordo = oratio_vocabula_ordinata(piscina, vc, (s32)-I);
            i32  k;
            i32  prior     = (i32)1000000;
            b32  ordinata  = VERUM;

            CREDO_AEQUALIS_I32 (xar_numerus(ordo),
                xar_numerus(oratio_vocabula_verba(vc)));
            per (k = ZEPHYRUM; k < xar_numerus(ordo); k++)
            {
                constans OratioVerbum* w =
                    (constans OratioVerbum*)xar_obtinere(
                    oratio_vocabula_verba(vc),
                    (i32)*(s32*)xar_obtinere(ordo, k));

                si (w->sedes > prior)
                {
                    ordinata = FALSUM;
                }
                prior = w->sedes;
            }
            CREDO_VERUM (ordinata);
            ordo = oratio_vocabula_ordinata(piscina, vc,
                (s32)ORATIO_VERBUM_IGNOTUM);
            CREDO_AEQUALIS_I32 (xar_numerus(ordo),
                oratio_vocabula_numerus(vc, ORATIO_VERBUM_IGNOTUM));
        }
    }

    imprimere("\n--- IV. Corpus: symbola + commentaria ---\n");
    {
        OratioVocabula* corpus = oratio_vocabula_creare(piscina, voc);
                chorda  nexus;
        FILE* lista;
        character linea[512];
        i32 plagulae = ZEPHYRUM;
        clock_t ante = clock();
        i32 n;
        i32 nota;
        i32 ambigua;
        i32 permissa;
        i32 ignota;

        CREDO_NON_NIHIL (corpus);
        sprintf(via, "%s/build/nexus.tsv", radix);
        si (!_plagulam_legere(piscina, via, &nexus))
        {
            CREDO_CULPA ("build/nexus.tsv absens - ./silva/nexus.sh -renovare");
        }
                alioquin
        {
            OratioVocabula* symbola_sola =
                oratio_vocabula_creare(piscina, voc);
                       i32 ignota_symbolorum;

                        CREDO_VERUM (oratio_vocabula_symbola(corpus,
                            nexus,
                            ORATIO_VOCABULA_EXCLUSA));
            /* PINNA 'solum cadens' super identificatores DOMUS (knotapel/
             * vendor/ archivum/ exclusa, decisio Frani 2026-09-04; cursus
             * glossarii I: MMMMMDXV -> MMMMMCDIX omnibus viis). Rubra = verba
             * nova ignota: glossarium (vox domus, terminus permissus) aut
             * renominatio; pinna movetur causa nominata. */
            CREDO_NON_NIHIL (symbola_sola);
            CREDO_VERUM (oratio_vocabula_symbola(symbola_sola, nexus,
                ORATIO_VOCABULA_EXCLUSA));

            CREDO_VERUM (oratio_vocabula_iudicare(symbola_sola));
            ignota_symbolorum = oratio_vocabula_numerus(symbola_sola,
                ORATIO_VERBUM_IGNOTUM);
            imprimere("  identificatores: verba %d  ignota %d  (pinna %d, solum cadens)\n",
                (integer)xar_numerus(oratio_vocabula_verba(symbola_sola)),
                (integer)ignota_symbolorum,
                (integer)IGNOTA_SYMBOLORUM_PINNA);
            si (ignota_symbolorum > (i32)IGNOTA_SYMBOLORUM_PINNA)
            {
                imprimere("  IGNOTA CREVERUNT: glossarium (oratio/glossarium.stml) aut renominatio; ./oratio/vocabula.sh -symbola\n");
            }
            CREDO_VERUM (ignota_symbolorum
                <= (i32)IGNOTA_SYMBOLORUM_PINNA);
        }
        sprintf(via, "%s/oratio/build/corpus_c.txt", radix);

        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_c.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                 size_t  l = strlen(linea);
                Piscina* p;
                 chorda  fons;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("vocabula_plagula",
                    33554432);
                sprintf(via, "%s/%s", radix, linea);
                si (_plagulam_legere(p, via, &fons))
                {
                    plagulae = plagulae + I;
                    si (!oratio_vocabula_commenta(corpus, fons, linea))
                    {
                        CREDO_CULPA ("commenta fracta");
                    }
                }
                piscina_destruere(p);
            }
            fclose(lista);
        }
        CREDO_VERUM (oratio_vocabula_iudicare(corpus));
        n     = xar_numerus(oratio_vocabula_verba(corpus));
        nota  = oratio_vocabula_numerus(corpus, ORATIO_VERBUM_NOTUM);
        ambigua = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_AMBIGUUM);
        permissa = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_PERMISSUM);
        ignota = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_IGNOTUM);
        imprimere("  plagulae C %d  verba distincta %d  sedes %d  %.0f ms\n",
            (integer)plagulae, (integer)n,
            (integer)oratio_vocabula_sedes(corpus),
            1000.0 * (duplex)(clock() - ante) / (duplex)CLOCKS_PER_SEC);
        imprimere("  nota %d  ambigua %d  permissa %d  IGNOTA %d (%.1f%%)\n",
            (integer)nota, (integer)ambigua, (integer)permissa,
            (integer)ignota,
            n > ZEPHYRUM ? 100.0 * (duplex)ignota / (duplex)n : 0.0);
        {
            Xar* ordo = oratio_vocabula_ordinata(piscina, corpus,
                (s32)ORATIO_VERBUM_IGNOTUM);

            per (i = ZEPHYRUM; i < xar_numerus(ordo)
                && i < (i32)XV; i++)
            {
                constans OratioVerbum* w =
                    (constans OratioVerbum*)xar_obtinere(
                    oratio_vocabula_verba(corpus),
                    (i32)*(s32*)xar_obtinere(ordo, i));

                imprimere("    %-20.*s %6d  %.*s:%d\n",
                    (integer)w->verbum.mensura,
                    (constans character*)w->verbum.datum,
                    (integer)w->sedes,
                    (integer)w->via_prima.mensura,
                    (constans character*)w->via_prima.datum,
                    (integer)w->linea_prima);
            }
        }
        CREDO_VERUM (plagulae > (i32)100);
        CREDO_VERUM (n > (i32)5000);
        CREDO_VERUM ((nota + ambigua + permissa) * (i32)III > n);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
