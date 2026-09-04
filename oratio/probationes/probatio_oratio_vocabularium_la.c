/* probatio_oratio_vocabularium_la.c - Quaesitio Latina (T8)
 *
 * I.   PLICATIO: minusculae, v->u, j->i, ligaturae ae/oe, macra/breves
 *      sublata, octeti ceteri ut sunt.
 * II.  ONUS: tabula commissa oneratur (census, tackons III, ms).
 * III. QUAESITIO contra WORDS (words_engine transcriptus): nomina
 *      (vir, virum, puellam - genus C, rex/regis - finis vacuus et
 *      clavis II), verba (amat, amavit clavis III, amatus VPAR clavis
 *      IV, amare INF), adiectiva (bonus, melior COMP clavis III,
 *      optime SUPER), adverbia (amare X semel, amariter POS, bene),
 *      indeclinabilia (et, in ABL+ACC, a V analyses), tackon
 *      (virumque: -que ante hospites; sine: nihil tentatur), unicum
 *      (agantur), plicatio in quaesitione (VIRVM == virum, Jam == iam),
 *      ignota (xyzzy, Troiae - WORDS Troiam non habet, forma vacua,
 *      forma XL litterarum) = inventa, non vitia.
 * IV.  LEMMA v1 (forma prima): vir amo puella bonus rex ago loquor
 *      licet amare et.
 * V.   CORPUS: vocabula fixturarum Latinarum (hilarius, propertius,
 *      cicero) quaesita - nota / ignota numerata, ignota prima
 *      impressa, ms; limen sanitatis: nota > dimidium.
 * Culpa plantata: declinatio 0 0 flexionis non iam quaelibet (VPAR).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_arbor.h"
#include "oratio_registrum.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    exitus->datum    = memoria;
    exitus->mensura  = (i32)longitudo;
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

interior b32
_incipit (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura >= l
        && memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM);
}

/* an analysis STIRPS adsit cum stirpe prima, parte, accidentiis
 * (initium), clave */
interior b32
_adest (
    constans OratioVocabulariumLa* voc,
                              Xar* analyses,
               constans character* stirps_prima,
                       OratioPars  pars,
               constans character* accidentia,
                              i32  clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(analyses); i++)
    {
        constans OratioAnalysis* a =
            (constans OratioAnalysis*)xar_obtinere(analyses, i);
        constans OratioStirps* st;
        constans OratioFlexio* fl;

        si (a->genus != ORATIO_ANALYSIS_STIRPS)
        {
            perge;
        }
        st = oratio_vocabularium_la_stirps(voc, a->stirps);
        fl = oratio_vocabularium_la_flexio(voc, a->flexio);
        si (   st->pars == pars
            && _aequalis(st->stirpes[ZEPHYRUM], stirps_prima)
            && _incipit(fl->accidentia, accidentia)
            && (clavis == ZEPHYRUM || a->clavis == clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i32
_numerus_partis (
    constans OratioVocabulariumLa* voc,
                              Xar* analyses,
                       OratioPars  pars)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(analyses); i++)
    {
        constans OratioAnalysis* a =
            (constans OratioAnalysis*)xar_obtinere(analyses, i);

        si (   a->genus == ORATIO_ANALYSIS_STIRPS
            && oratio_vocabularium_la_stirps(voc, a->stirps)->pars
                == pars)
        {
            n = n + I;
        }
    }
    redde n;
}

/* stirps analysis primae STIRPS cum stirpe prima data (index) aut -I */
interior s32
_stirps_analysis (
    constans OratioVocabulariumLa* voc,
                              Xar* analyses,
               constans character* stirps_prima,
                       OratioPars  pars)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(analyses); i++)
    {
        constans OratioAnalysis* a =
            (constans OratioAnalysis*)xar_obtinere(analyses, i);

        si (   a->genus == ORATIO_ANALYSIS_STIRPS
            && oratio_vocabularium_la_stirps(voc, a->stirps)->pars
                == pars
            && _aequalis(oratio_vocabularium_la_stirps(voc, a->stirps)
                ->stirpes[ZEPHYRUM], stirps_prima))
        {
            redde a->stirps;
        }
    }
    redde (s32)-I;
}

nomen structura {
    s64 vocabula;
    s64 nota;
    s64 ignota;
    i32 impressa;
} Coverage;

interior vacuum
_vocabula_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
            constans MateriaNodus* doc,
                           chorda  fons,
                         Coverage* c)
{

    constans MateriaValor* paragraphi =
        &doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
                      i32 np;
                      i32 p;

    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    np = materia_valor_lista_numerus(*paragraphi);
    per (p = ZEPHYRUM; p < np; p++)
    {
        constans MateriaNodus* par =
            materia_valor_lista_obtinere(*paragraphi, p)->datum.nodus;
        constans MateriaValor* sententiae =
            &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
                          i32 ns;
                          i32 s;

        si (sententiae->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        ns = materia_valor_lista_numerus(*sententiae);
        per (s = ZEPHYRUM; s < ns; s++)
        {
            constans MateriaNodus* sen =
                materia_valor_lista_obtinere(*sententiae,
                s)->datum.nodus;
            constans MateriaValor* elementa =
                &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
                              i32 ne;
                              i32 k;

            si (elementa->genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            ne = materia_valor_lista_numerus(*elementa);
            per (k = ZEPHYRUM; k < ne; k++)
            {
                constans MateriaNodus* e =
                    materia_valor_lista_obtinere(*elementa,
                    k)->datum.nodus;
                 constans MateriaValor* partes;
                 constans MateriaToken* a;
                 constans MateriaToken* b;
                                chorda  forma;
                                   Xar* analyses;

                si (e->genus != (s32)ORATIO_GENUS_VOCABULUM)
                {
                    perge;
                }
                partes = &e->loci[ORATIO_VOCABULUM_PARTES];
                si (   partes->genus != MATERIA_VALOR_LISTA
                    || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
                {
                    perge;
                }
                a = materia_valor_lista_obtinere(*partes,
                    ZEPHYRUM)->datum.token;
                b = materia_valor_lista_obtinere(*partes,
                    materia_valor_lista_numerus(*partes)
                        - I)->datum.token;
                                forma.datum   = fons.datum
                                    + a->byte_offset;

                forma.mensura = (i32)(b->byte_offset
                    + (s32)b->valor.mensura
                    - a->byte_offset);
                /* memoria per vocabulum: piscina propria parva */
                analyses = oratio_vocabularium_la_quaerere(piscina, voc,
                    forma);
                c->vocabula = c->vocabula + I;
                si (   analyses != NIHIL
                    && xar_numerus(analyses) > ZEPHYRUM)
                {
                    c->nota = c->nota + I;
                }
                alioquin
                {
                    c->ignota = c->ignota + I;
                    si (c->impressa < (i32)XII)
                    {
                        imprimere("    ignotum: %.*s\n",
                            (integer)forma.mensura,
                            (constans character*)forma.datum);
                        c->impressa = c->impressa + I;
                    }
                }
            }
        }
    }
}

hic_manens constans character* FIXTURAE_LATINAE[] = {
    "oratio/probationes/fixa/txt/hilarius.txt",
    "oratio/probationes/fixa/txt/propertius.txt",
    "oratio/probationes/fixa/txt/cicero.txt"
};

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix;
                character  via[1024];
                   chorda  tabula;
    OratioVocabulariumLa* voc;
 OratioVocabulariumVitium vitium;
                      i32  i;

    piscina =
        piscina_generare_dynamicum("probatio_oratio_vocabularium_la",
        268435456);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- I. Plicatio ---\n");
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("VIRVM")),
        "uirum"));
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("Jam")),
        "iam"));
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("\xC3\x86quus")), "aequus"));                 /* Æquus */
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("R\xC5\x8Dma")), "roma"));                    /* Rōma */
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("c\xC5\x93lum")), "coelum"));                 /* cœlum */
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("\xC4\x81m\xC4\x83")), "ama"));               /* āmă */
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("caf\xC3\xA9")), "caf\xC3\xA9"));             /* é manet */
    CREDO_VERUM (_aequalis(oratio_vocabularium_la_plicare(piscina,
        _l("")), ""));

    imprimere("\n--- II. Onus ---\n");
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &tabula));
    {
                         clock_t ante = clock();
                         clock_t post;
        OratioVocabulariumCensus c;

        voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
        post = clock();
        CREDO_NON_NIHIL (voc);
        si (voc == NIHIL)
        {
            imprimere("  onus fractum: %s\n", vitium.causa);
            credo_imprimere_compendium();
            redde I;
        }
        c = oratio_vocabularium_la_census(voc);
        imprimere("  stirpes %d flexiones %d additamenta %d unica %d tackons %d  onus %.1f ms\n",
            (integer)c.stirpes, (integer)c.flexiones,
            (integer)c.additamenta,
            (integer)c.unica,
            (integer)oratio_vocabularium_la_tackones(voc),
            1000.0 * (duplex)(post - ante) / (duplex)CLOCKS_PER_SEC);
        CREDO_AEQUALIS_I32 (c.stirpes, (i32)39335);
        CREDO_AEQUALIS_I32 (c.flexiones, (i32)1797);
        CREDO_AEQUALIS_I32 (oratio_vocabularium_la_tackones(voc),
            (i32)III);
    }

    imprimere("\n--- III. Quaesitio contra WORDS ---\n");
    {
        Xar* x;

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("vir"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_adest(voc, x, "vir", ORATIO_PARS_N, "NOM S", I));
        CREDO_VERUM (_adest(voc, x, "vir", ORATIO_PARS_N, "VOC S", I));

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("virum"));
        CREDO_VERUM (_adest(voc, x, "vir", ORATIO_PARS_N, "ACC S",
            (i32)II));
        CREDO_VERUM (_adest(voc, x, "vir", ORATIO_PARS_N, "GEN P",
            (i32)II));
        CREDO_VERUM (_adest(voc, x, "vis", ORATIO_PARS_N, "GEN P",
            (i32)II));
        imprimere("  virum: %d analyses\n", (integer)xar_numerus(x));
        {
            Xar* y = oratio_vocabularium_la_quaerere(piscina, voc,
                _l("VIRVM"));

            CREDO_AEQUALIS_I32 (xar_numerus(y), xar_numerus(x));
        }

        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("puellam"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_adest(voc, x, "puell", ORATIO_PARS_N, "ACC S",
            (i32)II));

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("rex"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_adest(voc, x, "rex", ORATIO_PARS_N, "NOM S", I));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("regis"));
        CREDO_VERUM (_adest(voc, x, "rex", ORATIO_PARS_N, "GEN S",
            (i32)II));
        CREDO_VERUM (_adest(voc, x, "reg", ORATIO_PARS_V,
            "PRES ACTIVE IND 2 S", (i32)II));

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amat"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_adest(voc, x, "am", ORATIO_PARS_V,
            "PRES ACTIVE IND 3 S", (i32)II));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amavit"));
        CREDO_VERUM (_adest(voc, x, "am", ORATIO_PARS_V,
            "PERF ACTIVE IND 3 S",
            (i32)III));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amatus"));
        CREDO_VERUM (_adest(voc, x, "am", ORATIO_PARS_V,
            "NOM S M PERF PASSIVE PPL",
            (i32)IV));
        CREDO_VERUM (_adest(voc, x, "amat", ORATIO_PARS_ADJ,
            "NOM S M POS", I));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amare"));
        CREDO_VERUM (_adest(voc, x, "am", ORATIO_PARS_V,
            "PRES ACTIVE INF", (i32)II));
        CREDO_AEQUALIS_I32 (_numerus_partis(voc, x, ORATIO_PARS_ADV),
            I);
        imprimere("  amare: %d analyses (ADV semel)\n",
            (integer)xar_numerus(x));

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("bonus"));
        CREDO_VERUM (_adest(voc, x, "bon", ORATIO_PARS_ADJ,
            "NOM S M POS", I));
        CREDO_VERUM (_adest(voc, x, "bon", ORATIO_PARS_N, "NOM S", I));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("melior"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_adest(voc, x, "bon", ORATIO_PARS_ADJ,
            "NOM S C COMP", (i32)III));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("optime"));
        CREDO_VERUM (_adest(voc, x, "bon", ORATIO_PARS_ADJ,
            "VOC S M SUPER", (i32)IV));
        CREDO_VERUM (_adest(voc, x, "bene", ORATIO_PARS_ADV, "X",
            (i32)III));
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("amariter"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_adest(voc, x, "amariter", ORATIO_PARS_ADV, "POS",
            I));

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("et"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_VERUM (_adest(voc, x, "et", ORATIO_PARS_CONJ, "", I));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("in"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_VERUM (_adest(voc, x, "in", ORATIO_PARS_PREP, "ABL", I));
        CREDO_VERUM (_adest(voc, x, "in", ORATIO_PARS_PREP, "ACC", I));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("a"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)V);
        CREDO_VERUM (_adest(voc, x, "a", ORATIO_PARS_PREP, "ABL", I));
        CREDO_VERUM (_adest(voc, x, "a", ORATIO_PARS_INTERJ, "", I));

        /* tackon: -que ante hospites, hospites tackon ferunt */
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("virumque"));
        CREDO_VERUM (xar_numerus(x) >= (i32)III);
        si (xar_numerus(x) >= (i32)II)
        {
            constans OratioAnalysis* t =
                (constans OratioAnalysis*)xar_obtinere(x, ZEPHYRUM);
            constans OratioAnalysis* h =
                (constans OratioAnalysis*)xar_obtinere(x, I);

            CREDO_AEQUALIS_S32 ((s32)t->genus,
                (s32)ORATIO_ANALYSIS_TACKON);
            CREDO_VERUM (_aequalis(oratio_vocabularium_la_additamentum(voc,
                t->tackon)->forma, "que"));
            CREDO_AEQUALIS_S32 ((s32)h->genus,
                (s32)ORATIO_ANALYSIS_STIRPS);
            CREDO_AEQUALIS_S32 (h->tackon, t->tackon);
            CREDO_AEQUALIS_I32 (h->mensura_stirpis + (i32)II, (i32)V);
        }
                CREDO_VERUM (_adest(voc, x, "vir", ORATIO_PARS_N,
                    "ACC S", (i32)II));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("sine"));
        CREDO_VERUM (_adest(voc, x, "sine", ORATIO_PARS_PREP, "ABL",
            I));
        per (i = ZEPHYRUM; i < xar_numerus(x); i++)
        {
            CREDO_AEQUALIS_S32 (((constans OratioAnalysis*)xar_obtinere(x,
                i))
                ->tackon, (s32)-I);   /* nihil tentatum: directae adsunt */
        }

                                /* INVENTUM (T8): 'sum' in DICTLINE.GEN venditae DEEST - V 5 1 lineae
         * omnes composita (absum adsum ...), INFLECTS V 5 1 XIX formae
         * solae (essem forem es este esto esse sunto). In TABULA SOLA
         * 'erat' 'fuit' IGNOTA manent (pinna huius plagulae); glossarium
         * domus (T9, oratio/glossarium.stml) paradigma fert - porta
         * probatio_oratio_glossarium ea cum glossario appenso probat.
         * Stirpes vacuae internae (Bdl WORDS) paratae. */

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("erat"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("fuit"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), ZEPHYRUM);
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("esse"));
        CREDO_VERUM (_adest(voc, x, "ed", ORATIO_PARS_V,
            "PRES ACTIVE INF", (i32)II));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("absunt"));
        CREDO_VERUM (_adest(voc, x, "abs", ORATIO_PARS_V,
            "PRES ACTIVE IND 3 P", I));


        /* unicum */
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("agantur"));

        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        CREDO_AEQUALIS_S32 ((s32)((constans OratioAnalysis*)xar_obtinere(x,
            ZEPHYRUM))->genus, (s32)ORATIO_ANALYSIS_UNICUM);
        CREDO_VERUM (_adest(voc, x, "ag", ORATIO_PARS_V,
            "PRES PASSIVE SUB 3 P", I));

        /* ordo: stirpes ordine plagulae, finis longior prior */
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("regis"));
        {
            i32 k;
            s32 ultima    = (s32)-I;
            b32 ordinata  = VERUM;

            per (k = ZEPHYRUM; k < xar_numerus(x); k++)
            {
                constans OratioAnalysis* a =
                    (constans OratioAnalysis*)xar_obtinere(x, k);

                si (a->stirps < ultima)
                {
                    ordinata = FALSUM;
                }
                ultima = a->stirps;
            }
            CREDO_VERUM (ordinata);
        }

        /* ignota = inventa */
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
            voc, _l("xyzzy"))), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
            voc, _l("Troiae"))), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
            voc, _l(""))), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
            voc, _l("abcdefghijklmnopqrstuvwxyzabcdefghijklmn"))),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
            voc, _l("\xE2\x80\x94"))), ZEPHYRUM);
    }

    imprimere("\n--- IV. Lemma v1 (forma prima) ---\n");
    {
        Xar* x;
        s32  s;

                x = oratio_vocabularium_la_quaerere(piscina, voc,
                    _l("vir"));
        s = _stirps_analysis(voc, x, "vir", ORATIO_PARS_N);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "vir"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amat"));
        s = _stirps_analysis(voc, x, "am", ORATIO_PARS_V);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "amo"));
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("puellam"));
        s = _stirps_analysis(voc, x, "puell", ORATIO_PARS_N);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "puella"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("melior"));
        s = _stirps_analysis(voc, x, "bon", ORATIO_PARS_ADJ);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "bonus"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("regis"));
        s = _stirps_analysis(voc, x, "rex", ORATIO_PARS_N);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "rex"));
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("agantur"));
        s = _stirps_analysis(voc, x, "ag", ORATIO_PARS_V);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "ago"));
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("loquitur"));
        s = _stirps_analysis(voc, x, "loqu", ORATIO_PARS_V);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "loquor"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amare"));
        s = _stirps_analysis(voc, x, "amare", ORATIO_PARS_ADV);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "amare"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("et"));
        s = _stirps_analysis(voc, x, "et", ORATIO_PARS_CONJ);
        CREDO_VERUM (s >= ZEPHYRUM
            && _aequalis(oratio_vocabularium_la_lemma(
            piscina, voc, s), "et"));
    }

    imprimere("\n--- V. Corpus: vocabula fixturarum Latinarum ---\n");
    {
        Coverage summa;

        memset(&summa, ZEPHYRUM, magnitudo(summa));
        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXTURAE_LATINAE)
            / magnitudo(FIXTURAE_LATINAE[0])); i++)
        {
            Piscina* p = piscina_generare_dynamicum("la_corpus",
                268435456);
              chorda textus;
            Coverage c;
            MateriaNodus* doc;
            clock_t ante;
            clock_t post;

            memset(&c, ZEPHYRUM, magnitudo(c));
            sprintf(via, "%s/%s", radix, FIXTURAE_LATINAE[i]);
            CREDO_VERUM (_plagulam_legere(p, via, &textus));
            doc = oratio_arbor_parsare(p,
                (constans character*)textus.datum,
                textus.mensura);
            CREDO_NON_NIHIL (doc);
            ante = clock();
            si (doc != NIHIL)
            {
                                _vocabula_quaerere(p, voc, doc, textus,
                                    &c);

            }
            post = clock();
            imprimere("  %-44s vocabula %6ld  nota %6ld  ignota %5ld  (%.1f%%)  %.1f ms\n",
                FIXTURAE_LATINAE[i], (longus)c.vocabula, (longus)c.nota,
                (longus)c.ignota,
                c.vocabula > ZEPHYRUM ? 100.0 * (duplex)c.nota
                    / (duplex)c.vocabula
                    : 0.0,
                1000.0 * (duplex)(post - ante)
                    / (duplex)CLOCKS_PER_SEC);
            CREDO_VERUM (c.vocabula > (s64)1000);
            CREDO_VERUM (c.nota * (s64)II > c.vocabula);
            summa.vocabula  += c.vocabula;
            summa.nota      += c.nota;
            summa.ignota    += c.ignota;
            piscina_destruere(p);
        }
        imprimere("  SUMMA vocabula %ld  nota %ld  ignota %ld  (%.1f%%)\n",
            (longus)summa.vocabula, (longus)summa.nota,
            (longus)summa.ignota,
            100.0 * (duplex)summa.nota / (duplex)summa.vocabula);
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
