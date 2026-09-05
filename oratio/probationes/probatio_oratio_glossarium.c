/* probatio_oratio_glossarium.c - Glossarium domus (T9)
 *
 * I.   CANON: oratio/glossarium.stml contra glossarium.canon vitia 0;
 *      copia cum classe ignota et copia sine lemma vitia > 0 (canon
 *      realis, non mutus).
 * II.  LECTOR: entria, formae (sum >= LXX), lemma ipsum forma nisi
 *      data; plicatio in quaestione (Est, EST, sum); permissa (offset,
 *      offsets, index); ignota (amat, xyzzy); fons malus refutatur cum
 *      linea (radix aliena, vocabulum sine classe, forma sine textu).
 * III. INTEGRATIO: glossarium in quaesitione Latina fons PRIMUS -
 *      'est' glossarium deinde edo; 'erat' 'sit' 'fuit' inventa (pinna
 *      T8 in tabula sola manet); 'virumque' immutatum; 'sine' immutatum.
 * IV.  CORPUS: fixturae Latinae sine et cum glossario - nota crescunt,
 *      numeri publicati.
 * Culpa plantata: plicatio clavis lectoris sublata ('Est' non invenit).
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "stml.h"
#include "internamentum.h"
#include "oratio_glossarium.h"
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

/* copia fontis cum substitutione literarum (prima occurrentia) */
interior chorda
_substituta (
               Piscina* piscina,
                chorda  fons,
    constans character* vetus,
    constans character* novus)
{
       i32 lv = (i32)strlen(vetus);
       i32 ln = (i32)strlen(novus);
       i32 i;
    chorda c;

    per (i = ZEPHYRUM; i + lv <= fons.mensura; i++)
    {
        si (memcmp(fons.datum + i, vetus, (size_t)lv) == ZEPHYRUM)
        {
            c.mensura = fons.mensura - lv + ln;
            c.datum   = (i8*)piscina_allocare(piscina,
                (memoriae_index)c.mensura + I);
            memcpy(c.datum, fons.datum, (size_t)i);
            memcpy(c.datum + i, novus, (size_t)ln);
            memcpy(c.datum + i + ln, fons.datum + i + lv,
                (size_t)(fons.mensura - i - lv));
            redde c;
        }
    }
    redde fons;
}

/* vitia canonis super fontem STML; -I = non iudicatum */
interior s32
_vitia_canonis (
               Piscina* piscina,
                 Canon* canon,
   InternamentumChorda* intern,
                chorda  fons,
    constans character* titulus)
{
    StmlResultus  res = stml_legere(fons, piscina, intern);
             Xar* vitia;
             i32  n;
             i32  i;

        si (!res.successus || res.elementum_radix == NIHIL)
        {
        imprimere("  %s: stml_legere fractum\n", titulus);
        redde (s32)-I;
        }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        redde (s32)-I;
    }
    n = xar_numerus(vitia);

    per (i = ZEPHYRUM; i < n && i < (i32)III; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: vitium %s", titulus, canon_nuntius(v->genus));
        si (v->detail != NIHIL)
        {
            imprimere(" (%.*s)", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
                imprimere("\n");
    }
    redde (s32)n;
}

/* analysis prima generis dati */
interior constans OratioAnalysis*
_prima (
                    Xar* x,
    OratioAnalysisGenus  genus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(x); i++)
    {
        constans OratioAnalysis* a =
            (constans OratioAnalysis*)xar_obtinere(x, i);

        si (a->genus == genus)
        {
            redde a;
        }
    }
    redde NIHIL;
}

nomen structura {
    s64 vocabula;
    s64 nota;
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
                forma.datum   = fons.datum + a->byte_offset;
                forma.mensura = (i32)(b->byte_offset
                    + (s32)b->valor.mensura
                    - a->byte_offset);
                analyses = oratio_vocabularium_la_quaerere(piscina, voc,
                    forma);
                c->vocabula = c->vocabula + I;
                si (   analyses != NIHIL
                    && xar_numerus(analyses) > ZEPHYRUM)
                {
                    c->nota = c->nota + I;
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
                   chorda  fons_glossarii;
                   chorda  fons_canonis;
                   chorda  tabula;
         OratioGlossarium* gl;
     OratioVocabulariumLa* voc;
 OratioVocabulariumVitium vitium;
      InternamentumChorda* intern;
                    Canon* canon;
                   chorda  causa;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_glossarium",
        268435456);
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    intern = internamentum_creare(piscina);

    imprimere("\n--- I. Canon ---\n");
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fons_glossarii));
    sprintf(via, "%s/oratio/grammatica/glossarium.canon", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &fons_canonis));
    causa.datum = NIHIL;
    causa.mensura = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL || fons_glossarii.datum == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_AEQUALIS_S32 (_vitia_canonis(piscina, canon, intern,
        fons_glossarii,
        "glossarium.stml"), ZEPHYRUM);
    CREDO_MAIOR_S32 (_vitia_canonis(piscina, canon, intern,
        _substituta(piscina, fons_glossarii, "classis=\"verbum\"",
            "classis=\"verbumm\""), "classis ignota"), ZEPHYRUM);
    CREDO_MAIOR_S32 (_vitia_canonis(piscina, canon, intern,
        _substituta(piscina, fons_glossarii, " lemma=\"sum\"", ""),
        "lemma absens"), ZEPHYRUM);
    CREDO_MAIOR_S32 (_vitia_canonis(piscina, canon, intern,
        _substituta(piscina, fons_glossarii, "persona=\"III\"",
            "persona=\"IV\""), "persona IV"), ZEPHYRUM);

    imprimere("\n--- II. Lector ---\n");
    gl = oratio_glossarium_legere(piscina, fons_glossarii, &vitium);
    CREDO_NON_NIHIL (gl);
    si (gl == NIHIL)
    {
        imprimere("  glossarium non legitur: %s:%d %s\n",
            vitium.plagula,
            (integer)vitium.linea, vitium.causa);
        credo_imprimere_compendium();
        redde I;
    }
    imprimere("  entria %d  formae %d\n",
        (integer)oratio_glossarium_numerus_entriorum(gl),
        (integer)oratio_glossarium_numerus_formarum(gl));
        CREDO_VERUM (oratio_glossarium_numerus_entriorum(gl)
            >= (i32)XVIII);
    CREDO_VERUM (oratio_glossarium_numerus_formarum(gl) >= (i32)100);

    {
        constans OratioGlossariumEntrium* e =
            oratio_glossarium_entrium(gl,
            ZEPHYRUM);

        CREDO_VERUM (_aequalis(e->lemma, "sum"));
        CREDO_VERUM (_aequalis(e->lingua, "latina"));
        CREDO_VERUM (_aequalis(e->classis, "verbum"));
        CREDO_FALSUM (e->permissum);
        CREDO_VERUM (e->formae_numerus >= (i32)70);
        CREDO_VERUM (_aequalis(e->sensus, "be; exist; (copula)"));
    }
    {
                                   Xar* x;
        constans OratioGlossariumForma* f;

        x = oratio_glossarium_quaerere(piscina, gl, _l("est"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        f = oratio_glossarium_forma(gl, *(s32*)xar_obtinere(x,
            ZEPHYRUM));
        CREDO_VERUM (_aequalis(f->textus, "est"));
        CREDO_VERUM (_aequalis(f->persona, "III"));
        CREDO_VERUM (_aequalis(f->numerus, "singularis"));
        CREDO_VERUM (_aequalis(f->tempus, "praesens"));
        CREDO_VERUM (_aequalis(f->modus, "indicativus"));
        CREDO_VERUM (_aequalis(f->vox, "activa"));
        CREDO_VERUM (_aequalis(f->casus_grammaticus, ""));
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("Est"))), I);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("EST"))), I);
        /* esto: II et III S imperativus futurum - formae duae */
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("esto"))), (i32)II);
        /* fueris: futurum-exactum IND et perfectum SUB */
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("fueris"))), (i32)II);
        x = oratio_glossarium_quaerere(piscina, gl, _l("esse"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        f = oratio_glossarium_forma(gl, *(s32*)xar_obtinere(x,
            ZEPHYRUM));
        CREDO_VERUM (_aequalis(f->modus, "infinitivus"));
        CREDO_VERUM (_aequalis(f->forma_verbi, "infinitivum"));
        /* lemma ipsum forma: offset (cum forma offsets), index (sine forma) */
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("offset"), VERUM));
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("offsets"), VERUM));
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("index"), VERUM));
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("STML"), VERUM));
        CREDO_FALSUM (oratio_glossarium_permissum(piscina, gl,
            _l("sum"), VERUM));
        CREDO_FALSUM (oratio_glossarium_permissum(piscina, gl,
            _l("amat"), VERUM));
        /* contextus: tok latinus, worklog anglicus solum, offset ambo */
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl, _l("tok"),
            VERUM));
        CREDO_FALSUM (oratio_glossarium_permissum(piscina, gl,
            _l("tok"), FALSUM));
        CREDO_FALSUM (oratio_glossarium_permissum(piscina, gl,
            _l("worklog"), VERUM));
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("worklog"), FALSUM));
        CREDO_VERUM (oratio_glossarium_permissum(piscina, gl,
            _l("offset"), FALSUM));
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("amat"))), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("xyzzy"))), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l(""))), ZEPHYRUM);
                /* plicatio UTRIMQUE (clavis et quaestio): Vergilius cum V et capitali -
         * VERGILIVS, uergilius, Vergilius unum idemque; Vergilio bis (DAT ABL) */
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("VERGILIVS"))), I);
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("uergilius"))), I);
        x = oratio_glossarium_quaerere(piscina, gl, _l("Vergilius"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        f = oratio_glossarium_forma(gl, *(s32*)xar_obtinere(x,
            ZEPHYRUM));
        CREDO_VERUM (_aequalis(f->casus_grammaticus, "nominativus"));
        CREDO_VERUM (_aequalis(oratio_glossarium_entrium(gl,
            f->entrium)->classis,
            "nomen-proprium"));
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("Vergilio"))), (i32)II);
        /* 'sum' semel: lemma non iteratum cum forma 'sum' data */

        CREDO_AEQUALIS_I32 (xar_numerus(oratio_glossarium_quaerere(piscina,
            gl,
            _l("sum"))), I);
    }
    /* fontes mali refutantur cum linea */
    CREDO_NIHIL (oratio_glossarium_legere(piscina,
        _l("<lexicon versio=\"1\"/>"), &vitium));
    CREDO_MAIOR_I32 (vitium.linea, ZEPHYRUM);
    CREDO_NIHIL (oratio_glossarium_legere(piscina,
        _l("<glossarium versio=\"1\">\n  <vocabulum lemma=\"x\" lingua=\"latina\"/>\n</glossarium>"),
        &vitium));
    CREDO_AEQUALIS_I32 (vitium.linea, (i32)II);
    CREDO_NIHIL (oratio_glossarium_legere(piscina,
        _l("<glossarium versio=\"1\">\n  <vocabulum lemma=\"x\" lingua=\"latina\" classis=\"verbum\">\n    <forma/>\n  </vocabulum>\n</glossarium>"),
        &vitium));
    CREDO_AEQUALIS_I32 (vitium.linea, (i32)III);
    imprimere("  fontes mali: %s:%d %s\n", vitium.plagula,
        (integer)vitium.linea,
        vitium.causa);

    imprimere("\n--- III. Integratio: fons primus ---\n");
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    CREDO_VERUM (_plagulam_legere(piscina, via, &tabula));
    voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
    CREDO_NON_NIHIL (voc);
    si (voc == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_NIHIL (oratio_vocabularium_la_glossarium(voc));
    CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabularium_la_quaerere(piscina,
        voc,
        _l("erat"))), ZEPHYRUM);
    oratio_vocabularium_la_glossarium_ponere(voc, gl);
    CREDO_VERUM (oratio_vocabularium_la_glossarium(voc) == gl);
    {
                            Xar* x;
        constans OratioAnalysis* a;

        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("est"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), (i32)II);
        a = (constans OratioAnalysis*)xar_obtinere(x, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)a->genus,
            (s32)ORATIO_ANALYSIS_GLOSSARIUM);
        CREDO_VERUM (a->glossarium >= ZEPHYRUM
            && _aequalis(oratio_glossarium_entrium(gl,
                oratio_glossarium_forma(gl,
                a->glossarium)->entrium)->lemma,
                "sum"));
        a = (constans OratioAnalysis*)xar_obtinere(x, I);
        CREDO_AEQUALIS_S32 ((s32)a->genus, (s32)ORATIO_ANALYSIS_STIRPS);
        CREDO_VERUM (_aequalis(oratio_vocabularium_la_stirps(voc,
            a->stirps)
            ->stirpes[ZEPHYRUM], "ed"));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("erat"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_NON_NIHIL (_prima(x, ORATIO_ANALYSIS_GLOSSARIUM));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("fuit"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("sit"));
        CREDO_NON_NIHIL (_prima(x, ORATIO_ANALYSIS_GLOSSARIUM));
        /* inventa: tackons non tentantur ('sumque' -> sum + que tamen) */
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("estque"));
        CREDO_VERUM (xar_numerus(x) >= (i32)II);
        CREDO_AEQUALIS_S32 ((s32)((constans OratioAnalysis*)xar_obtinere(x,
            ZEPHYRUM))->genus, (s32)ORATIO_ANALYSIS_TACKON);
        /* WORDS immutata */
        x = oratio_vocabularium_la_quaerere(piscina, voc,
            _l("virumque"));
        CREDO_AEQUALIS_S32 ((s32)((constans OratioAnalysis*)xar_obtinere(x,
            ZEPHYRUM))->genus, (s32)ORATIO_ANALYSIS_TACKON);
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("amat"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_NIHIL (_prima(x, ORATIO_ANALYSIS_GLOSSARIUM));
        x = oratio_vocabularium_la_quaerere(piscina, voc, _l("offset"));
        CREDO_AEQUALIS_I32 (xar_numerus(x), I);
        CREDO_NON_NIHIL (_prima(x, ORATIO_ANALYSIS_GLOSSARIUM));
    }

    imprimere("\n--- IV. Corpus: sine et cum glossario ---\n");
    {
        Coverage sine;
        Coverage cum;

        memset(&sine, ZEPHYRUM, magnitudo(sine));
        memset(&cum, ZEPHYRUM, magnitudo(cum));
        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXTURAE_LATINAE)
            / magnitudo(FIXTURAE_LATINAE[0])); i++)
        {
            Piscina* p = piscina_generare_dynamicum("gl_corpus",
                268435456);
             chorda textus;
            MateriaNodus* doc;
            Coverage a;
            Coverage b;

            memset(&a, ZEPHYRUM, magnitudo(a));
            memset(&b, ZEPHYRUM, magnitudo(b));
            sprintf(via, "%s/%s", radix, FIXTURAE_LATINAE[i]);
            CREDO_VERUM (_plagulam_legere(p, via, &textus));
            doc = oratio_arbor_parsare(p,
                (constans character*)textus.datum,
                textus.mensura);
            CREDO_NON_NIHIL (doc);
            si (doc != NIHIL)
            {
                oratio_vocabularium_la_glossarium_ponere(voc, NIHIL);
                _vocabula_quaerere(p, voc, doc, textus, &a);
                oratio_vocabularium_la_glossarium_ponere(voc, gl);
                _vocabula_quaerere(p, voc, doc, textus, &b);
            }
            imprimere("  %-44s vocabula %6ld  nota sine %6ld (%.1f%%)  cum %6ld (%.1f%%)\n",
                FIXTURAE_LATINAE[i], (longus)a.vocabula, (longus)a.nota,
                a.vocabula > ZEPHYRUM ? 100.0 * (duplex)a.nota
                    / (duplex)a.vocabula : 0.0,
                (longus)b.nota,
                b.vocabula > ZEPHYRUM ? 100.0 * (duplex)b.nota
                    / (duplex)b.vocabula : 0.0);
            CREDO_VERUM (b.nota >= a.nota);
            sine.vocabula  += a.vocabula;
            sine.nota      += a.nota;
            cum.vocabula   += b.vocabula;
            cum.nota       += b.nota;
            piscina_destruere(p);
        }
        imprimere("  SUMMA vocabula %ld  sine %ld (%.1f%%)  cum %ld (%.1f%%)  lucrum %ld\n",
            (longus)sine.vocabula, (longus)sine.nota,
            100.0 * (duplex)sine.nota / (duplex)sine.vocabula,
            (longus)cum.nota, 100.0 * (duplex)cum.nota
                / (duplex)cum.vocabula,
            (longus)(cum.nota - sine.nota));
        CREDO_VERUM (cum.nota > sine.nota);
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
