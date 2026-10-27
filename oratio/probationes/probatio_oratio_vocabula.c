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
 * V.   PROSA markdown inlinearis (T15a): nodi TEXTUS soli - verba
 *      capituli, paragraphi, nexus (textus), cellarum, elementorum
 *      adsunt cum linea; verbatim, saeptum, destinatio nexus, html,
 *      praefatio ABSUNT; sagitta sola verbum non est; numeri exacti.
 * VI.  IUDICIUM ANGLICE: the determinans, parser substantivum, running
 *      adiectivum, quickly adverbium (Moby); worklog et offset permissa
 *      (glossarium anglice); tok LATINUM (contextus latinus solum);
 *      puella LATINUM (WORDS), piscina NOTUM (Moby eam novit); lexema,
 *      est (sum), silva LATINA (glossarium Latinum); xyzzy ignotum;
 *      ambiguum nullum.
 * VII. CORPUS prosae: plagulae md tractae quas iudex prosa_anglica
 *      (unitates.stml) iudicat, limina (plagulae > CCC, verba >
 *      V milia, nota + permissa + latina > tertia pars) - relatio diei
 *      primi in worklog; pinna post morphologiam.
 * Culpa plantata: limes CAPITALIS in scissione sublatus (MateriaNodus
 * integer manet); secunda: contextus glossarii inversus (e->anglice).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_vocabula.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_vocabularium_en.h"
#include "oratio_glossarium.h"
#include "unitates.h"
#include "unitates_arboris.h"
#include "filum.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 2958 -> 2964 (T14, 2026-09-05): tabula symbolorum (nexus) post
 * commissionem T13 renovata plagulas T13 demum indexavit - porta T13
 * ea numquam vidit (excubitor post successum renovat). Verba T13
 * novem in glossarium (ranga, permille, conllu, upos, misc, circse,
 * llct) aut renominata (treebank -> thesaurus arborum); residuum VI =
 * verba plagularum VETERUM nunc primum indexata (verba.c T14 nullum
 * verbum ignotum novum fert: quodque eius ignotum alibi iam sedet).
 * 2964 -> 2958 (2026-09-05): fusio rhubarb-pictor verba XXVIII nova
 * attulit (2987) - sectio glossarii TERMINI PICTOR (coinages Latina,
 * abbreviationes permissae, quaestio 01M1S6W8HF); pinna ad numerum
  * novum DEMISSA (solum cadens).
  * 2958 -> 2949 (2026-09-07): tabula symbolorum RESTITUTA (quaestio
 * 01M1TD1FMFT3 - fixtura briar latina.h decurtatum capita domus in
 * percursu umbrabat; tabula 0.96M -> 1.10M ordinum, verba 7598 ->
 * 7706): pinna 2958 super tabulam DEGRADATAM mensurata erat (percursus
 * vetus in arbore hodierna 2732 dat, tabula plena 2959); verba CCXXVII
 * restituta = constantes enumerorum et localia functionum quas caput
 * decurtatum fregerat. Octo tituli accidentium orationis (locativus,
 * superlativus, infinitivus, imperativus, plusquamperfectum, dualis,
   * numerale, pronomen; formae X cum infinitivum, pronominis) in
 * glossarium (TERMINI GRAMMATICI); pinna ad numerum novum DEMISSA.
 * 2949 -> COPIA (2026-09-07, decisio Frani): numerus 'solum cadens'
 * duas res non distinguebat - codicem meliorem et indicem
 * degradantem (fixtura latina.h: 2990 -> 2958 -> 2732 VIRIDIS) - et
 * attributionem manualem poscebat. Pinna nunc = COPIA COMMISSA
 * verborum ignotorum toleratorum (fixa/vocabula/ignota_symbolorum.txt)
 * quae solum minuitur; verbum novum rubrum cum sede prima; regeneratio
 * ORATIO_VOCABULA_SCRIBERE=1 causa nominata. Denominator (verba
 * identificatorum distincta) PINNATUS solum crescens - index degradans
 * ibi rubet ubi numerus legitur. Viae TRACTATAE solae (git ls-files):
 * opus alienae sessionis non commissum pinnam non ferit. */
#define VERBA_SYMBOLORUM_PINNA 7703

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

/* ordo octetorum (copia deterministica, diff legibilis) */
interior s32
_chordae_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans chorda* ca = (constans chorda*)a;
    constans chorda* cb = (constans chorda*)b;
                i32  m  = ca->mensura < cb->mensura
                    ? ca->mensura : cb->mensura;
                int d  = memcmp(ca->datum, cb->datum, (size_t)m);

    si (d != ZEPHYRUM)
    {
        redde d < ZEPHYRUM ? -I : I;
    }
    redde ca->mensura < cb->mensura ? -I
        : (ca->mensura > cb->mensura ? I : ZEPHYRUM);
}

/* copiam ignotorum scribere: caput '#' + verba ordine octetorum */
interior vacuum
_copiam_scribere (
                    Piscina* piscina,
         constans character* via,
    constans OratioVocabula* vc,
                        Xar* ordo)
{
     Xar* verba = xar_creare(piscina, (i32)magnitudo(chorda));
    FILE* f;
     i32  k;

    si (verba == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(ordo); k++)
    {
        constans OratioVerbum* w = (constans OratioVerbum*)xar_obtinere(
            oratio_vocabula_verba(vc), (i32)*(s32*)xar_obtinere(ordo,
            k));
        chorda* s = (chorda*)xar_addere(verba);

        si (s != NIHIL)
        {
            *s = w->verbum;
        }
    }
    xar_ordinare(verba, _chordae_comparare);
    f = lectiones_fopen(via, "w");
    si (f == NIHIL)
    {
        redde;
    }
    fprintf(f, "# ignota_symbolorum.txt - verba identificatorum IGNOTA"
        " TOLERATA (pinna ut COPIA, 2026-09-07)\n"
        "# solum minuitur: glossarium (entrium aut ignotum-permissum)"
        " aut renominatio; verbum extra copiam = rubrum\n"
        "# regeneratio deliberata: ORATIO_VOCABULA_SCRIBERE=1"
        " ./oratio/compile_probationes.sh vocabula - causa in"
        " commissione nominanda\n");
    per (k = ZEPHYRUM; k < xar_numerus(verba); k++)
    {
        constans chorda* s = (constans chorda*)xar_obtinere(verba, k);

        fprintf(f, "%.*s\n", (integer)s->mensura,
            (constans character*)s->datum);
    }
    fclose(f);
}

/* mechanismus praefixorum (sectio I): knotapel/ in nexu ficto omittitur
 * - ambitus VERUS lintus in unitates.stml vivit (iudex lint_latinus) */
hic_manens constans character* constans EXCLUSA_FICTA[] = {
    "knotapel/", NIHIL
};

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

interior constans character*
_radix_viae (vacuum)
{
    constans character* radix_viae = getenv("RHUBARB_RADIX");

    redde radix_viae != NIHIL ? radix_viae : ".";
}

/* FIXURA SUITAE: tabula Latina cum glossario et Moby SEMEL oneratae
 * (assertiones olim principalis et sectionis VI); sectiones LEGUNT -
 * oratio_vocabula_creare(_anglice) tabulas constantes accipit
 * (2026-10-21), ergo vocabula quaeque sectionis propria est. */
nomen structura {
    constans OratioVocabulariumLa* voc;
    constans OratioVocabulariumEn* en;   /* NIHIL: Moby absens, VII
                                          * tacet */
} FixuraVocabulorum;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
           FixuraVocabulorum* f;
          constans character* radix  = _radix_viae();
                   character  via[1024];
                      chorda  tabula;
                      chorda  fons_glossarii;
                      chorda  fons_en;
        OratioVocabulariumLa* voc;
    OratioVocabulariumVitium  vitium;

    f = (FixuraVocabulorum*)piscina_allocare(c->piscina,
        (memoriae_index)magnitudo(FixuraVocabulorum));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    CREDO_VERUM (_plagulam_legere(c->piscina, via, &tabula));
    voc = oratio_vocabularium_la_onerare(c->piscina, tabula, &vitium);
    CREDO_NON_NIHIL (voc);
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    CREDO_VERUM (_plagulam_legere(c->piscina, via, &fons_glossarii));
    si (voc == NIHIL || fons_glossarii.datum == NIHIL)
    {
        redde;
    }
    {
        OratioGlossarium* gl = oratio_glossarium_legere(c->piscina,
            fons_glossarii,
            &vitium);

        CREDO_NON_NIHIL (gl);
        oratio_vocabularium_la_glossarium_ponere(voc, gl);
    }
    sprintf(via, "%s/oratio/vocabularium/en/mobypos.txt", radix);
    CREDO_VERUM (_plagulam_legere(c->piscina, via, &fons_en));
    f->voc  = voc;
    f->en   = oratio_vocabularium_en_onerare(c->piscina, fons_en,
        &vitium);
    CREDO_NON_NIHIL (f->en);
    c->datum = f;
}

/* nexus.tsv (CLXXII MB 2026-10-21, cum arbore crescens) PER PARTES
 * fluit: lineae integrae partis cuiusque in ambas vocabulas.
 * oratio_vocabula_symbola nihil ex nexu retinet (verbum, via,
 * identificator copiantur), ergo capsa una sufficit - plagula tota
 * numquam in memoria (olim CC MiB arenae sectionis IV). */
#define CAPSA_NEXUS ((i32)16777216)

interior vacuum
_nexum_fluere (
    FilumOctetorumLector* nexus,
          OratioVocabula* corpus,
          OratioVocabula* symbola_sola,
                  chorda  tractatae,
                     b32* bene_corporis,
                     b32* bene_solorum)
{
    Piscina* capsae = piscina_generare_dynamicum("vocabula_nexus",
        (memoriae_index)CAPSA_NEXUS + 65536);
          i8* capsa;
         i32  reliquum = ZEPHYRUM;   /* linea imperfecta in capite */
         s32  lecti;

    *bene_corporis  = FALSUM;
    *bene_solorum   = FALSUM;
    si (capsae == NIHIL)
    {
        redde;
    }
    capsa = (i8*)piscina_allocare(capsae, (memoriae_index)CAPSA_NEXUS);
    si (capsa == NIHIL)
    {
        piscina_destruere(capsae);
        redde;
    }
    *bene_corporis  = VERUM;
    *bene_solorum   = symbola_sola != NIHIL;
    fac
    {
           i32 finis;
           i32 ultima;
        chorda pars;

        lecti = filum_octeti_legere(nexus, capsa + reliquum,
            CAPSA_NEXUS - reliquum);
        si (lecti < ZEPHYRUM)
        {
            *bene_corporis  = FALSUM;
            *bene_solorum   = FALSUM;
            frange;
        }
        finis   = reliquum + (i32)lecti;
        ultima  = finis;
        si (lecti > ZEPHYRUM)
        {
            dum (ultima > ZEPHYRUM && capsa[ultima - I] != '\n')
            {
                ultima = ultima - I;
            }
            si (ultima == ZEPHYRUM)
            {
                /* linea capsa longior: fracta, numquam tacite */
                *bene_corporis  = FALSUM;
                *bene_solorum   = FALSUM;
                frange;
            }
        }
        pars.datum    = capsa;
        pars.mensura  = ultima;
        *bene_corporis = *bene_corporis
            && oratio_vocabula_symbola(corpus, pars, NIHIL, tractatae);
        si (symbola_sola != NIHIL)
        {
            *bene_solorum = *bene_solorum
                && oratio_vocabula_symbola(symbola_sola, pars, NIHIL,
                    tractatae);
        }
        reliquum = finis - ultima;
        memmove(capsa, capsa + ultima, (size_t)reliquum);
    } dum (lecti > ZEPHYRUM);
    piscina_destruere(capsae);
}

interior vacuum
_scissio_probare (
    CredoContextus* c)
{
    constans FixuraVocabulorum* f =
        (constans FixuraVocabulorum*)c->suita;
                               Piscina* piscina  = c->piscina;
         constans OratioVocabulariumLa* voc;
               OratioVocabula* vc;

    si (f == NIHIL)
    {
        redde;   /* fixura absens: sectio VACUA, suita FRACTA */
    }
    voc = f->voc;
    CREDO_GRADUS ("I. Scissio identificatorum");
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
            /* numeralia latina.h tota maiuscula: nulla verba;
             * 'MIX' ut segmentum verbum manet */
            "CCLXIX\tsedes\tconstans\tinclude/latina.h\t300\t9\t0\n"
            "MIX\tsedes\tconstans\tinclude/latina.h\t1060\t9\t0\n"
            "COLOR_MIX\tsedes\tconstans\tlib/pictor.c\t7\t9\t0\n"
            "piscina\tvocatio\tfunctio\tlib/alia.c\t1\t1\t0\n"
                                "piscina_destruere\tsedes\tfunctio\tlib/piscina.c\t80\t1\t0\n"),
            EXCLUSA_FICTA, _l("")));

    {
                          Xar* verba = oratio_vocabula_verba(vc);
        constans OratioVerbum* v;

        imprimere("  verba %d sedes %d\n", (integer)xar_numerus(verba),
            (integer)oratio_vocabula_sedes(vc));
        CREDO_AEQUALIS_I32 (xar_numerus(verba), (i32)XV);
        /* piscina allocare materia nodus stml oratio pars capsula forum
         * data utf8 legere destruere */
                CREDO_AEQUALIS_I32 (oratio_vocabula_sedes(vc),
                    (i32)XVII);

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
        /* numeralia tota: nihil; segmentum 'MIX' verbum */
        CREDO_NIHIL (_verbum(vc, "cclxix"));
        CREDO_NON_NIHIL (_verbum(vc, "color"));
        CREDO_NON_NIHIL (_verbum(vc, "mix"));
        CREDO_NIHIL (_verbum(vc, "x"));
                CREDO_NIHIL (_verbum(vc, "vocatio"));
        CREDO_NIHIL (_verbum(vc, "knotapel"));   /* via exclusa */

        v = _verbum(vc, "nodus");
        CREDO_VERUM (v != NIHIL && _aequalis(v->via_prima,
            "materia/fontes/materia_nodus.h")
                && v->linea_prima == (i32)40);
    }

    CREDO_GRADUS ("II. Commentaria per silvam et orationem");
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

    CREDO_GRADUS ("III. Iudicium");
    CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("xyzzy"),
        "lib/x.c", I,
        FALSUM));
        CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("Index"),
            "lib/x.c", (i32)II,
            FALSUM));
    CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("worklog"),
        "lib/x.c", (i32)III,
        FALSUM));   /* contextus anglicus solum: in identificatore IGNOTUM */
    CREDO_VERUM (oratio_vocabula_verbum_addere(vc, _l("tok"), "lib/x.c",
        (i32)IV,
        FALSUM));   /* abbreviatio permissa (latinus) */

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
        v = _verbum(vc, "worklog");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_IGNOTUM);
        v = _verbum(vc, "tok");
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);

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
                            (i32)VI);   /* offset buffer index utf8 stml tok */


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
}

interior vacuum
_corpus_symbolorum_probare (
    CredoContextus* c)
{
    constans FixuraVocabulorum* f =
        (constans FixuraVocabulorum*)c->suita;
                               Piscina* piscina  = c->piscina;
                    constans character* radix    = _radix_viae();
                             character  via[1024];
         constans OratioVocabulariumLa* voc;
                                   i32  i;

    si (f == NIHIL)
    {
        redde;   /* fixura absens: sectio VACUA, suita FRACTA */
    }
    voc = f->voc;
    {
                        OratioVocabula* corpus =
                            oratio_vocabula_creare(piscina, voc);
                  FilumOctetorumLector* nexus;
                                   b32  bene_corporis  = FALSUM;
                                   b32  bene_solorum   = FALSUM;
                                chorda  tractatae;
                        OratioVocabula* symbola_sola      = NIHIL;
                                chorda  copia;
                             character  via_copiae[512];
                             character  culpa[256];
                                   i32  ignota_symbolorum  = ZEPHYRUM;
                                   i32  verba_symbolorum   = ZEPHYRUM;
                                   b32  scribere          =
                                       getenv("ORATIO_VOCABULA_SCRIBERE")
                                           != NIHIL;
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
        tractatae.datum    = NIHIL;
        tractatae.mensura  = ZEPHYRUM;
        copia.datum        = NIHIL;
        copia.mensura      = ZEPHYRUM;
        nexus              = filum_octeti_aperire(via, piscina);
        si (nexus == NIHIL)
        {
            CREDO_CULPA ("build/nexus.tsv absens - ./silva/nexus.sh -renovare");
        }
        alioquin
        {
            symbola_sola = oratio_vocabula_creare(piscina, voc);

            /* viae tractatae (cursor scribit): sine ea porta rubet,
             * numquam tacite omnes vias numerat. AMBITUS = iudex
             * lint_latinus (unitates.stml) - idem filtrum ac
             * instrumentum vocabula, ne pinna et lint divergant */
            sprintf(via, "%s/oratio/build/corpus_tractatae.txt", radix);
            si (!_plagulam_legere(piscina, via, &tractatae))
            {
                CREDO_CULPA ("oratio/build/corpus_tractatae.txt absens - e radice per cursorem curre");
            }
            alioquin
            {
                  chorda  causa_u;
                     b32  ignotus = FALSUM;
                Unitates* u = unitates_legere(unitates_sutura_arboris(
                    radix, piscina), piscina, &causa_u, NIHIL);

                CREDO_NON_NIHIL (u);
                si (u != NIHIL)
                {
                    tractatae = unitates_viae_iudicatae(u, _l(
                        "lint_latinus"), tractatae, piscina, &ignotus);
                }
                CREDO_FALSUM (ignotus);
                CREDO_MAIOR_I32 (tractatae.mensura, ZEPHYRUM);
            }
            _nexum_fluere(nexus, corpus, symbola_sola, tractatae,
                &bene_corporis, &bene_solorum);
            filum_octeti_claudere(nexus);
            CREDO_VERUM (bene_corporis);
            /* PINNA = COPIA COMMISSA super identificatores DOMUS (knotapel/
             * vendor/ archivum/ exclusa, decisio Frani 2026-09-04; viae
             * tractatae solae 2026-09-07). Verbum ignotum extra copiam =
             * NOVUM = rubrum cum sede prima et EXITIBUS nominatis
             * (oratio_vocabula_nova); copia solum minuitur; regeneratio
             * deliberata ORATIO_VOCABULA_SCRIBERE=1 causa nominata.
             * Verdictum et relatio POST commentaria (IV b). */
            CREDO_NON_NIHIL (symbola_sola);
            CREDO_VERUM (bene_solorum);
            CREDO_VERUM (oratio_vocabula_iudicare(symbola_sola));
            ignota_symbolorum = oratio_vocabula_numerus(symbola_sola,
                ORATIO_VERBUM_IGNOTUM);
            verba_symbolorum  = xar_numerus(
                oratio_vocabula_verba(symbola_sola));
            sprintf(via_copiae,
                "%s/oratio/probationes/fixa/vocabula/ignota_symbolorum.txt",
                radix);
            si (   !_plagulam_legere(piscina, via_copiae, &copia)
                && !scribere)
            {
                CREDO_CULPA ("copia ignotorum absens - ORATIO_VOCABULA_SCRIBERE=1 eam genera, causa nominata");
            }
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
                        (integer)nota, (integer)ambigua,
                        (integer)permissa,
            (integer)ignota,
            n > ZEPHYRUM ? 100.0 * (duplex)ignota / (duplex)n : 0.0);
        imprimere("  commentaria: IGNOTA relata (non pinnata), XV primae:\n");
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

        /* VERDICTUM IDENTIFICATORUM post relationem commentorum (quae
         * ignota RELATA fert - 'after', 'const' - ne cum findings
         * confundantur). Fractura verba ipsa fert (CREDO_CULPA cum
         * nuntio): consumptor quivis lineae 'FRACTA' (commissio,
         * relatio_fracturarum) verba et exitus videt, non numerum
         * lineae solum. Denominator item cum causis. */
        CREDO_GRADUS ("IV b. identificatores: verdictum contra copiam toleratam");
        si (symbola_sola != NIHIL)
        {
                                       i32 tolerata_n  =
                                           ZEPHYRUM;
                                       i32 evanida_n   =
                                           ZEPHYRUM;
                        constans character* nuntius     = NIHIL;
                                       s32  nova_n;

            si (scribere)
            {
                Xar* ordo = oratio_vocabula_ordinata(piscina,
                    symbola_sola,
                    (s32)ORATIO_VERBUM_IGNOTUM);

                _copiam_scribere(piscina, via_copiae, symbola_sola,
                    ordo);
                imprimere("  COPIA SCRIPTA: %s (%d verba) - CAUSA in commissione nominanda\n",
                    via_copiae, (integer)ignota_symbolorum);
                si (!_plagulam_legere(piscina, via_copiae, &copia))
                {
                    CREDO_CULPA ("copia scripta non legibilis");
                }
            }
            nova_n = oratio_vocabula_nova(piscina, symbola_sola, copia,
                VERUM, &tolerata_n, &evanida_n, &nuntius);
            imprimere("  identificatores: verba %d (pinna %d, solum crescens)  ignota %d  tolerata %d  NOVA %d  evanida %d\n",
                (integer)verba_symbolorum,
                (integer)VERBA_SYMBOLORUM_PINNA,
                (integer)ignota_symbolorum, (integer)tolerata_n,
                (integer)nova_n, (integer)evanida_n);
            si (nova_n != ZEPHYRUM)
            {
                CREDO_CULPA (nova_n > ZEPHYRUM ? nuntius
                    : "relatio novorum fracta (memoria)");
            }
            si (verba_symbolorum < (i32)VERBA_SYMBOLORUM_PINNA)
            {
                sprintf(culpa, "VERBA DECREVERUNT %d < pinna %d: index"
                    " (build/nexus.tsv) degradatus? ./silva/nexus.sh"
                    " -renovare -plenus; plagulae deletae -> pinna"
                    " movenda causa nominata",
                    (integer)verba_symbolorum,
                    (integer)VERBA_SYMBOLORUM_PINNA);
                CREDO_CULPA (culpa);
            }
        }
    }
}

interior vacuum
_prosam_probare (
    CredoContextus* c)
{
    constans FixuraVocabulorum* f =
        (constans FixuraVocabulorum*)c->suita;
                               Piscina* piscina  = c->piscina;
         constans OratioVocabulariumLa* voc;

    si (f == NIHIL)
    {
        redde;   /* fixura absens: sectio VACUA, suita FRACTA */
    }
    voc = f->voc;
    {
        OratioVocabula* pr = oratio_vocabula_creare(piscina, voc);
        constans OratioVerbum* v;

        CREDO_NON_NIHIL (pr);
        CREDO_VERUM (oratio_vocabula_prosa(pr, _l(
            "---\n"
            "title: frontmatter\n"
            "---\n"
            "# Heading words\n"
            "\n"
            "Prose alpha with `verbatim beta` and [linktext gamma](http://urlword.example/delta) here \xe2\x86\x92 also.\n"
            "Second epsilon line.\n"
            "\n"
            "```c\n"
            "int fenced_zeta;\n"
            "```\n"
            "\n"
            "| cell eta | cell theta |\n"
            "|---|---|\n"
            "| iota | kappa |\n"
            "\n"
            "<div>htmlword lambda</div>\n"
            "\n"
            "- item mu\n"
            "- item nu\n"), "docs/probatio.md"));
        imprimere("  verba %d sedes %d\n",
            (integer)xar_numerus(oratio_vocabula_verba(pr)),
            (integer)oratio_vocabula_sedes(pr));
        v = _verbum(pr, "alpha");
        CREDO_NON_NIHIL (v);
        CREDO_VERUM (v != NIHIL && v->linea_prima == (i32)VI);
        CREDO_VERUM (v != NIHIL && v->sedes_prosae == I
            && v->sedes_symbolorum == ZEPHYRUM
            && v->sedes_commentorum == ZEPHYRUM);
        CREDO_VERUM (v != NIHIL && v->ex_prosa_prima
            && !v->ex_commento_prima);
        CREDO_VERUM (v != NIHIL && _aequalis(v->via_prima,
            "docs/probatio.md"));
        v = _verbum(pr, "heading");
        CREDO_VERUM (v != NIHIL && v->linea_prima == (i32)IV);
        CREDO_NON_NIHIL (_verbum(pr, "words"));
        CREDO_NIHIL (_verbum(pr, "verbatim"));   /* code span */
        CREDO_NIHIL (_verbum(pr, "beta"));
        CREDO_NON_NIHIL (_verbum(pr, "linktext"));
        CREDO_NON_NIHIL (_verbum(pr, "gamma"));
        CREDO_NIHIL (_verbum(pr, "urlword"));    /* destinatio nexus */
        CREDO_NIHIL (_verbum(pr, "delta"));
        CREDO_NIHIL (_verbum(pr, "http"));
        CREDO_NON_NIHIL (_verbum(pr, "here"));
        v = _verbum(pr, "epsilon");
        CREDO_VERUM (v != NIHIL && v->linea_prima == (i32)VII);
        CREDO_NIHIL (_verbum(pr, "fenced"));     /* saeptum */
        CREDO_NIHIL (_verbum(pr, "zeta"));
        CREDO_NIHIL (_verbum(pr, "int"));
        v = _verbum(pr, "cell");
        CREDO_VERUM (v != NIHIL && v->sedes == (i32)II);   /* tabula */
        CREDO_NON_NIHIL (_verbum(pr, "eta"));
        CREDO_NON_NIHIL (_verbum(pr, "theta"));
        CREDO_NON_NIHIL (_verbum(pr, "iota"));
        CREDO_NON_NIHIL (_verbum(pr, "kappa"));
        CREDO_NIHIL (_verbum(pr, "htmlword"));   /* blocus html */
        CREDO_NIHIL (_verbum(pr, "lambda"));
        CREDO_NIHIL (_verbum(pr, "div"));
        v = _verbum(pr, "item");
        CREDO_VERUM (v != NIHIL && v->sedes == (i32)II);   /* lista */
        CREDO_NON_NIHIL (_verbum(pr, "mu"));
        CREDO_NON_NIHIL (_verbum(pr, "nu"));
        CREDO_NIHIL (_verbum(pr, "frontmatter"));   /* praefatio */
                CREDO_NIHIL (_verbum(pr, "title"));
        CREDO_NON_NIHIL (_verbum(pr, "also"));
        CREDO_NIHIL (_verbum(pr, "\xe2\x86\x92"));   /* sagitta: signum, non verbum */
        CREDO_AEQUALIS_I32 (xar_numerus(oratio_vocabula_verba(pr)),
            (i32)XXI);
        CREDO_AEQUALIS_I32 (oratio_vocabula_sedes(pr), (i32)XXIII);
    }
}

interior vacuum
_iudicium_anglicum_probare (
    CredoContextus* c)
{
    constans FixuraVocabulorum* f =
        (constans FixuraVocabulorum*)c->suita;
                               Piscina* piscina  = c->piscina;
         constans OratioVocabulariumLa* voc;
         constans OratioVocabulariumEn* en;

    si (f == NIHIL)
    {
        redde;   /* fixura absens: sectio VACUA, suita FRACTA */
    }
    voc  = f->voc;
    en   = f->en;
    {
        OratioVocabula* an = oratio_vocabula_creare_anglice(piscina,
            voc,
            en);
        constans OratioVerbum* v;

        CREDO_NON_NIHIL (an);
        CREDO_VERUM (oratio_vocabula_prosa(an, _l(
                        "The parser is running quickly; a worklog notes offset tok\n"
            "xyzzy puella lexema est silva piscina; values tested.\n"),
            "docs/a.md"));
        CREDO_VERUM (oratio_vocabula_iudicare(an));
        v = _verbum(an, "the");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->classis, "determinans"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "the"));
        CREDO_VERUM (v != NIHIL && v->analyses == I && v->lemmata == I);
        v = _verbum(an, "parser");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->classis, "substantivum"));
        v = _verbum(an, "running");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "adiectivum"));
        v = _verbum(an, "quickly");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "adverbium"));
        v = _verbum(an, "worklog");   /* contextus anglicus */
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);
        v = _verbum(an, "offset");    /* ambo */
        CREDO_VERUM (v != NIHIL
            && v->status == ORATIO_VERBUM_PERMISSUM);
        v = _verbum(an, "tok");       /* contextus latinus solum -> Latine notum */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_LATINUM);
        v = _verbum(an, "xyzzy");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_IGNOTUM);
        CREDO_VERUM (v != NIHIL && v->analyses == ZEPHYRUM);
                v = _verbum(an, "puella");    /* WORDS solum */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_LATINUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "N"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "puella"));
        v = _verbum(an, "piscina");   /* Moby eam novit (Anglice): notum */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->classis, "substantivum"));
        v = _verbum(an, "lexema");    /* glossarium, lingua latina */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_LATINUM);
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->classis, "substantivum"));
        v = _verbum(an, "est");       /* glossarium primum: sum, non edo */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_LATINUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "sum"));
                v = _verbum(an, "silva");     /* nomen proprium domus, Latinum */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_LATINUM);
        v = _verbum(an, "values");    /* regula morphologica (T15b) */
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "value"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->regula, "pluralis-s"));
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->classis, "substantivum"));
        v = _verbum(an, "tested");
        CREDO_VERUM (v != NIHIL && v->status == ORATIO_VERBUM_NOTUM);
        CREDO_VERUM (v != NIHIL && _aequalis(v->lemma, "test"));
        CREDO_VERUM (v != NIHIL
            && _aequalis(v->regula, "praeteritum-ed"));
        CREDO_VERUM (v != NIHIL && _aequalis(v->classis, "verbum"));
        v = _verbum(an, "the");
        CREDO_VERUM (v != NIHIL && v->regula.mensura == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (oratio_vocabula_numerus(an,
            ORATIO_VERBUM_AMBIGUUM), ZEPHYRUM);
        imprimere("  nota %d permissa %d latina %d ignota %d\n",
            (integer)oratio_vocabula_numerus(an, ORATIO_VERBUM_NOTUM),
            (integer)oratio_vocabula_numerus(an,
            ORATIO_VERBUM_PERMISSUM),
            (integer)oratio_vocabula_numerus(an,
            ORATIO_VERBUM_LATINUM),
            (integer)oratio_vocabula_numerus(an,
            ORATIO_VERBUM_IGNOTUM));
        CREDO_VERUM (strcmp(oratio_verbum_status_titulus(
            ORATIO_VERBUM_LATINUM), "latinum") == ZEPHYRUM);
    }
}

interior vacuum
_prosam_domus_probare (
    CredoContextus* c)
{
    constans FixuraVocabulorum* f =
        (constans FixuraVocabulorum*)c->suita;
                               Piscina* piscina  = c->piscina;
                    constans character* radix    = _radix_viae();
                             character  via[1024];
         constans OratioVocabulariumLa* voc;
         constans OratioVocabulariumEn* en;
                                   i32  i;

    si (f == NIHIL)
    {
        redde;   /* fixura absens: sectio VACUA, suita FRACTA */
    }
    voc  = f->voc;
    en   = f->en;
    si (en != NIHIL)
    {
        OratioVocabula* corpus = oratio_vocabula_creare_anglice(piscina,
            voc, en);
        FILE* lista;
        character linea[512];
        i32 plagulae = ZEPHYRUM;
        i32 exclusae = ZEPHYRUM;
        clock_t ante = clock();
        i32 n;
        i32 nota;
        i32 permissa;
        i32 latina;
        i32 ignota;
        chorda    causa_u;
        Unitates* u_prosae = unitates_legere(
            unitates_sutura_arboris(radix, piscina), piscina, &causa_u,
            NIHIL);

        CREDO_NON_NIHIL (corpus);
        CREDO_NECESSE_NON_NIHIL (u_prosae);
        sprintf(via, "%s/oratio/build/corpus_md.txt", radix);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
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
                si (unitates_iudicat(u_prosae, _l("prosa_anglica"),
                        _l(linea), piscina, NIHIL) != UNITATES_IUDICAT)
                {
                    exclusae = exclusae + I;
                    perge;
                }
                p = piscina_generare_dynamicum("vocabula_plagula",
                    33554432);
                sprintf(via, "%s/%s", radix, linea);
                si (_plagulam_legere(p, via, &fons))
                {
                    plagulae = plagulae + I;
                    si (!oratio_vocabula_prosa(corpus, fons, linea))
                    {
                        CREDO_CULPA ("prosa fracta");
                    }
                }
                piscina_destruere(p);
            }
            fclose(lista);
        }
        CREDO_VERUM (oratio_vocabula_iudicare(corpus));
        n     = xar_numerus(oratio_vocabula_verba(corpus));
        nota  = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_NOTUM);
        permissa  = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_PERMISSUM);
        latina    = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_LATINUM);
        ignota    = oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_IGNOTUM);
        imprimere("  plagulae md %d (exclusae %d)  verba distincta %d  sedes %d  %.0f ms\n",
            (integer)plagulae, (integer)exclusae, (integer)n,
            (integer)oratio_vocabula_sedes(corpus),
            1000.0 * (duplex)(clock() - ante) / (duplex)CLOCKS_PER_SEC);
        imprimere("  nota %d  permissa %d  latina %d  IGNOTA %d (%.1f%%)\n",
            (integer)nota, (integer)permissa, (integer)latina,
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
        CREDO_VERUM (plagulae > (i32)300);
        CREDO_VERUM (exclusae > ZEPHYRUM);
        CREDO_VERUM (n > (i32)5000);
        CREDO_AEQUALIS_I32 (oratio_vocabula_numerus(corpus,
            ORATIO_VERBUM_AMBIGUUM), ZEPHYRUM);
                        /* limina prosae (T15b, 2026-09-04): nota 73.8 % post regulas
         * morphologicas (58 % -> 26.2 % ignota) et cursum glossarii II;
         * pavimentum LXX % nota, TECTUM XXIX % ignota - tectum pinna
         * prosae est (numerus 'solum cadens' identificatorum hic nimis
         * fragilis: prosa cotidie verba nova fert); rubrum = regula aut
         * glossarium fractum, non verbum novum unum. Causa nominata
         * movet. */
        CREDO_VERUM ((nota + permissa + latina) * (i32)X
            > n * (i32)VII);
        CREDO_VERUM (ignota * (i32)C <= n * (i32)XXIX);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I-III. Scissio identificatorum, commentaria, iudicium",
        _scissio_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "IV. Corpus: symbola + commentaria", _corpus_symbolorum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "V. Prosa markdown: nodi TEXTUS soli", _prosam_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "VI. Iudicium Anglice", _iudicium_anglicum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "VII. Corpus: prosa markdown domus", _prosam_domus_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("oratio_vocabula", SECTIONES,
        _suitam_parare, NIHIL);
}
