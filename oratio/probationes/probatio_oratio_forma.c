/* probatio_oratio_forma.c - Forma textus paragraphi (T6b)
 *
 * I.   Tabula regularum sana (formae, indicia, terminatores) et
 *      iudicium super indicia manu facta.
 * II.  Casus inlineares: textus -> forma paragraphi primi et numerus
 *      unitatum lectionis (versus LF/CRLF, prosa involuta, tituli,
 *      tabulae per spatia/tabulas/virgulas, indices numerati/notati/
 *      Romani/paginati, prosa cum spatiis duplicibus, distichon,
 *      lexema infrangibile latitudinem non fingens).
 * III. Census formarum super fixturas fixa/txt (Propertius versus,
 *      ceteri prosa) - numeri publicati, limina sanitatis.
 * IV.  IUDICIA MANU FACTA (fixa/iudicia, notae '[[' ante initium
 *      unitatis): initia sententiarum orationis contra notas;
 *      congruentia per plagulam PINNATA, solum crescens; discrepantiae
 *      impressae (casus speciales = DATA).
 * V.   Census super paragraphos md (specimen quinta quaeque):
 *      prosa + titulus >= LXXX per centum; ORATIO_FORMA_SPECIMINA=1
 *      paragraphos non prosae imprimit (via, forma, regula, linea
 *      prima) - fons casuum specialium.
 * Culpa plantata: limen regulae 'versus voluntarius' 67 -> 167.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_forma.h"
#include "oratio_sententiae.h"
#include "oratio_registrum.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

interior i32
_numerus_listae (
    constans MateriaNodus* n,
                      i32  locus)
{
    constans MateriaValor* v = &n->loci[locus];

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(*v);
}

interior constans MateriaNodus*
_paragraphus (
    constans MateriaNodus* doc,
                      i32  i)
{
    redde materia_valor_lista_obtinere(
        doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI], i)->datum.nodus;
}

/* forma paragraphi primi, numerus unitatum, indicia recomputata */
interior OratioForma
_forma_textus (
               Piscina* piscina,
    constans character* textus,
                   i32* unitates,
         OratioIndicia* indicia)
{
    MateriaNodus* doc = oratio_arbor_parsare(piscina, textus,
        (i32)strlen(textus));
    constans MateriaNodus* par;

    *unitates = ZEPHYRUM;
    memset(indicia, ZEPHYRUM, magnitudo(*indicia));
    si (   doc == NIHIL
        || _numerus_listae(doc, (i32)ORATIO_DOCUMENTUM_PARAGRAPHI)
            == ZEPHYRUM)
    {
        redde ORATIO_FORMA_NUMERUS_FORMARUM;
    }
    par = _paragraphus(doc, ZEPHYRUM);
    *unitates = _numerus_listae(par,
        (i32)ORATIO_PARAGRAPHUS_SENTENTIAE);
    oratio_forma_indicia(oratio_forma_elementa(piscina, par),
        oratio_forma_initium_lineae(par), indicia);
    redde oratio_forma_paragraphi(par);
}

nomen structura {
    constans character* textus;
           OratioForma  forma;
                   i32  unitates;
    constans character* nota;
} CasusFormae;

hic_manens constans CasusFormae CASUS[] = {
    { "  at uos, deductae quibus est fallacia lunae\n"
      "      et labor in magicis sacra piare focis,\n"
      "  en agedum dominae mentem conuertite nostrae,\n"
      "      et facite illa meo palleat ore magis!\n",
      ORATIO_FORMA_VERSUS, (i32)IV, "versus elegiaci LF" },
    { "  at uos, deductae quibus est fallacia lunae\r\n"
      "      et labor in magicis sacra piare focis,\r\n"
      "  en agedum dominae mentem conuertite nostrae,\r\n"
      "      et facite illa meo palleat ore magis!\r\n",
      ORATIO_FORMA_VERSUS, (i32)IV, "versus elegiaci CRLF" },
    { "Apprehension seems to exist among the people of the Southern States\n"
      "that by the accession of a Republican administration their property\n"
      "and their peace and personal security are to be endangered.\n"
      "There has never been any reasonable cause for such apprehension.\n",
      ORATIO_FORMA_PROSA, (i32)II, "prosa involuta" },
    { "Chapter I\n", ORATIO_FORMA_TITULUS, (i32)I, "titulus brevis" },
    { "SANCTI HILARII\n", ORATIO_FORMA_TITULUS, (i32)I,
        "titulus capitalis" },
    { "Contents:\n", ORATIO_FORMA_TITULUS, (i32)I, "titulus cum colo" },
    { "He left. She stayed.\n", ORATIO_FORMA_PROSA, (i32)II,
      "linea una, sententiae duae" },
    { "It rained.\n", ORATIO_FORMA_PROSA, (i32)I,
        "linea una terminata" },
    { "Brig. Gen. Thomas Farrell\n", ORATIO_FORMA_TITULUS, (i32)I,
      "attributio: puncta interna, unitas una" },
    { "Name      Age   City\n"
      "Alice     30    Rome\n"
      "Bob       41    Paris\n",
      ORATIO_FORMA_TABULA, (i32)III, "tabula spatiis" },
    { "a\tb\tc\n1\t2\t3\n", ORATIO_FORMA_TABULA, (i32)II,
        "tabula tabulis" },
    { "| a | b |\n| 1 | 2 |\n", ORATIO_FORMA_TABULA, (i32)II,
        "tabula virgulis" },
    { "It was late.  The house was dark.  Nobody\n"
      "came to the door.  We waited for an hour.\n",
      ORATIO_FORMA_PROSA, (i32)IV,
          "spatia duplicia post punctum: non tabula" },
    { "1. Introduction\n2. The Site\n3. The Test\n",
      ORATIO_FORMA_INDEX, (i32)III, "index numeratus" },
        { "- apples\n- pears\n- plums\n", ORATIO_FORMA_INDEX, (i32)III,
      "index notatus" },
    { "**Who:** Carter, Data.\n**Who:** Daniel alone.\n**Who:** Lucia.\n",
      ORATIO_FORMA_PROSA, (i32)I,
          "cursus markdown '**' non signum listae" },
    { "\"Resolved:  that the maintenance inviolate of\n"
      "the rights of the States, and especially the\n"
      "right of each State to order and control its\n"
      "own domestic institutions according to its\n"
      "own judgment exclusively, is essential to\n"
      "that balance of power on which the\n"
      "perfection and endurance of our political\n"
      "fabric depend.\"\n",
      ORATIO_FORMA_PROSA, (i32)I,
          "prosa columna angusta typista involuta" },

    { "I. Prima\nII. Secunda\nIII. Tertia\n", ORATIO_FORMA_INDEX,
        (i32)III,
      "index Romanus" },
    { "Introduction ........ 3\nThe Site ............ 9\nThe Test ........... 17\n",
      ORATIO_FORMA_INDEX, (i32)III, "index paginatus" },
    { "The quick brown fox jumps over the lazy dog near the riverbank today\n"
      "again.\n",
      ORATIO_FORMA_PROSA, (i32)I, "prosa duarum linearum" },
    { "Odi et amo. quare id faciam, fortasse requiris.\n"
      "    nescio, sed fieri sentio et excrucior.\n",
      ORATIO_FORMA_VERSUS, (i32)II, "distichon interpunctum" },
    { "See the page at http://example.com/a/very/long/path/here/x for details\n"
      "the cat sat on the mat and looked at the\n"
      "dog who sat on the other mat and looked\n"
      "back at the cat for a very long time.\n",
      ORATIO_FORMA_PROSA, (i32)I,
          "lexema infrangibile latitudinem non fingit" }
};

hic_manens constans character* FIXTURAE[] = {
    "oratio/probationes/fixa/txt/hilarius.txt",
    "oratio/probationes/fixa/txt/propertius.txt",
    "oratio/probationes/fixa/txt/cicero.txt",
    "oratio/probationes/fixa/txt/lincoln.txt",
    "oratio/probationes/fixa/txt/trinity.txt"
};

nomen structura {
    constans character* via;
                   i32  pinna;     /* congruentia minima (solum crescens) */
} Iudicium;

hic_manens constans Iudicium IUDICIA[] = {
        { "oratio/probationes/fixa/iudicia/propertius-i-1.txt",
            (i32)VIII },
    { "oratio/probationes/fixa/iudicia/lincoln-inauguratio.txt",
        (i32)VIII },
    { "oratio/probationes/fixa/iudicia/cicero-catilina.txt",
        (i32)X },
    { "oratio/probationes/fixa/iudicia/trinity-caput.txt",
        (i32)XXV }

};

nomen structura {
    s64 formae[ORATIO_FORMA_NUMERUS_FORMARUM];
    s64 paragraphi;
    s64 sententiae;
    b32 specimina;
} Census;

interior vacuum
_censere (
    constans MateriaNodus* doc,
                   Census* c,
       constans character* titulus,
       constans character* textus,
                  Piscina* piscina)
{
    i32 np = _numerus_listae(doc, (i32)ORATIO_DOCUMENTUM_PARAGRAPHI);
    i32 i;

    per (i = ZEPHYRUM; i < np; i++)
    {
        constans MateriaNodus* par    = _paragraphus(doc, i);
                  OratioForma  forma  = oratio_forma_paragraphi(par);

        c->paragraphi = c->paragraphi + I;
        c->sententiae = c->sententiae
            + _numerus_listae(par, (i32)ORATIO_PARAGRAPHUS_SENTENTIAE);
        c->formae[forma] = c->formae[forma] + I;
        si (c->specimina && forma != ORATIO_FORMA_PROSA)
        {
             constans OratioRegulaFormae* regula = NIHIL;
                           OratioIndicia  indicia;
                                     s32  ab;
                                     s32  k;

            oratio_forma_indicia(oratio_forma_elementa(piscina, par),
                oratio_forma_initium_lineae(par), &indicia);
            oratio_forma_iudicare(&indicia, &regula);
            ab = oratio_forma_initium_lineae(par);
            imprimere("    %-8s %-24s %s: ",
                oratio_forma_titulus(forma),
                regula != NIHIL ? regula->titulus : "-", titulus);
            per (k = ab; k < ab + (s32)70 && textus[k] != '\0'
                && textus[k] != '\n' && textus[k] != '\r'; k++)
            {
                putchar(textus[k]);
            }
            putchar('\n');
        }
    }
}

/* paragraphi md: extentum per arborem md (ut probatio_oratio_fissio) */
interior vacuum
_extentum (
    constans MateriaNodus* n,
                      s32* initium,
                      s32* finis)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];
                          i32  m;
                          i32  j;

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _extentum(v->datum.nodus, initium, finis);
            perge;
        }
        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            constans MateriaToken* t = v->datum.token;

            si (   t->fons_index  == MD_FONS_PLAGULAE
                && t->byte_offset >= ZEPHYRUM)
            {
                si (*initium < ZEPHYRUM || t->byte_offset < *initium)
                {
                    *initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->valor.mensura > *finis)
                {
                    *finis = t->byte_offset + (s32)t->valor.mensura;
                }
            }
            perge;
        }
        si (v->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        m = materia_valor_lista_numerus(*v);
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);

            si (e == NIHIL)
            {
                perge;
            }
            si (e->genus == MATERIA_VALOR_NODUS)
            {
                _extentum(e->datum.nodus, initium, finis);
            }
            alioquin si (   e->genus == MATERIA_VALOR_TOKEN
                         && e->datum.token->fons_index
                             == MD_FONS_PLAGULAE
                         && e->datum.token->byte_offset >= ZEPHYRUM)
            {
                constans MateriaToken* t = e->datum.token;

                si (*initium < ZEPHYRUM || t->byte_offset < *initium)
                {
                    *initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->valor.mensura > *finis)
                {
                    *finis = t->byte_offset + (s32)t->valor.mensura;
                }
            }
        }
    }
}

interior vacuum
_paragraphos_censere (
       constans character* titulus,
    constans MateriaNodus* n,
       constans character* fons,
                  Piscina* piscina,
                   Census* c)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    si (n->genus == (s32)MD_GENUS_PARAGRAPHUS)
    {
        s32 a = (s32)-I;
        s32 b = ZEPHYRUM;

        _extentum(n, &a, &b);
        si (a >= ZEPHYRUM && b > a)
        {
            character* copia = (character*)piscina_allocare(piscina,
                (memoriae_index)(b - a) + I);
            MateriaNodus* doc;

            memcpy(copia, fons + a, (size_t)(b - a));
            copia[b - a] = '\0';
            doc = oratio_arbor_parsare(piscina, copia, (i32)(b - a));
            si (doc != NIHIL)
            {
                _censere(doc, c, titulus, copia, piscina);
            }
        }
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _paragraphos_censere(titulus, v->datum.nodus, fons, piscina,
                c);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    _paragraphos_censere(titulus, e->datum.nodus, fons,
                        piscina, c);
                }
            }
        }
    }
}

interior vacuum
_censum_imprimere (
    constans character* titulus,
       constans Census* c)
{
    i32 f;

    imprimere("  %-44s paragraphi %6ld  unitates %6ld ", titulus,
        (longus)c->paragraphi, (longus)c->sententiae);
    per (f = ZEPHYRUM; f < (i32)ORATIO_FORMA_NUMERUS_FORMARUM; f++)
    {
        imprimere(" %s %ld", oratio_forma_titulus((OratioForma)f),
            (longus)c->formae[f]);
    }
    imprimere("\n");
}

/* notas '[[' tollere; offsets initiorum in Xar de s32 */
interior character*
_iudicia_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura,
                   Xar* initia)
{
           i32  crudi   = ZEPHYRUM;
     character* crudum  = _plagulam_legere(piscina, via, &crudi);
     character* purum;
           i32  i;
           i32  o       = ZEPHYRUM;

    si (crudum == NIHIL)
    {
        redde NIHIL;
    }
    purum = (character*)piscina_allocare(piscina,
        (memoriae_index)crudi + I);
    per (i = ZEPHYRUM; i < crudi; i++)
    {
        si (i + I < crudi && crudum[i] == '[' && crudum[i + I] == '[')
        {
            s32* locus = (s32*)xar_addere(initia);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus  = (s32)o;
            i       = i + I;
            perge;
        }
        purum[o]  = crudum[i];
        o         = o + I;
    }
    purum[o] = '\0';
    *mensura = o;
    redde purum;
}

s32
principale (vacuum)
{
               Piscina* piscina;
    constans character* radix_viae;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_forma",
        8388608);
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }

    imprimere("\n--- I. Tabula regularum ---\n");
    {
                      OratioIndicia  indicia;
        constans OratioRegulaFormae* regula = NIHIL;

        CREDO_VERUM (ORATIO_REGULAE_FORMAE_NUMERUS >= (i32)V);
        per (i = ZEPHYRUM; i < ORATIO_REGULAE_FORMAE_NUMERUS; i++)
        {
            constans OratioRegulaFormae* r = &ORATIO_REGULAE_FORMAE[i];
                                    i32  c;
                                    b32  terminata = FALSUM;

            CREDO_NON_NIHIL (r->titulus);
            CREDO_MINOR_S32 ((s32)r->forma,
                (s32)ORATIO_FORMA_NUMERUS_FORMARUM);
            CREDO_VERUM (r->forma != ORATIO_FORMA_PROSA);   /* prosa = ordinaria */
            per (c = ZEPHYRUM; c < (i32)ORATIO_CONDICIONES_MAXIMAE; c++)
            {
                si (r->condiciones[c].indicium == ORATIO_INDICIUM_NIHIL)
                {
                    terminata = VERUM;
                    frange;
                }
                CREDO_MINOR_S32 ((s32)r->condiciones[c].indicium,
                    (s32)ORATIO_INDICIUM_NUMERUS_INDICIORUM);
            }
            CREDO_VERUM (terminata);
        }
        CREDO_VERUM (strcmp(oratio_forma_titulus(ORATIO_FORMA_PROSA),
            "prosa") == ZEPHYRUM);
        CREDO_VERUM (strcmp(oratio_forma_titulus(ORATIO_FORMA_INDEX),
            "index") == ZEPHYRUM);
        CREDO_VERUM (strcmp(oratio_forma_indicii_titulus(
            ORATIO_INDICIUM_VOLUNTARIAE_PCT), "voluntariae-pct")
            == ZEPHYRUM);
        memset(&indicia, ZEPHYRUM, magnitudo(indicia));
        CREDO_AEQUALIS_S32 ((s32)oratio_forma_iudicare(&indicia,
            &regula),
            (s32)ORATIO_FORMA_PROSA);
        CREDO_NIHIL (regula);
        indicia.lineae               = I;
        indicia.elementa             = (i32)III;
        indicia.capitales_omnes_pct  = (i32)100;
        CREDO_AEQUALIS_S32 ((s32)oratio_forma_iudicare(&indicia,
            &regula),
            (s32)ORATIO_FORMA_TITULUS);
        CREDO_NON_NIHIL (regula);
        CREDO_VERUM (regula != NIHIL
            && strcmp(regula->titulus, "titulus capitalis")
                == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (oratio_forma_indicium(&indicia,
            ORATIO_INDICIUM_ELEMENTA), (i32)III);
        CREDO_AEQUALIS_I32 (oratio_forma_indicium(&indicia,
            ORATIO_INDICIUM_NIHIL), ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)oratio_forma_paragraphi(NIHIL),
            (s32)ORATIO_FORMA_PROSA);
    }

    imprimere("\n--- II. Casus inlineares ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(CASUS)
        / magnitudo(CASUS[0]));
        i++)
    {
        Piscina* p = piscina_generare_dynamicum("forma_casus", 4194304);
        OratioIndicia indicia;
        i32 unitates;
        OratioForma forma = _forma_textus(p, CASUS[i].textus, &unitates,
            &indicia);

        si (   forma    != CASUS[i].forma
            || unitates != CASUS[i].unitates)
        {
            imprimere("  %s: forma %s (sperata %s), unitates %d (speratae %d)"
                      " [lineae %d latitudo %d vol%% %d interp%% %d not%% %d col%% %d]\n",
                CASUS[i].nota, oratio_forma_titulus(forma),
                oratio_forma_titulus(CASUS[i].forma), (integer)unitates,
                (integer)CASUS[i].unitates, (integer)indicia.lineae,
                (integer)indicia.latitudo,
                (integer)indicia.voluntariae_pct,
                (integer)indicia.interpunctae_pct,
                (integer)indicia.notatae_pct,
                (integer)indicia.columnatae_pct);
        }
        CREDO_AEQUALIS_S32 ((s32)forma, (s32)CASUS[i].forma);
        CREDO_AEQUALIS_I32 (unitates, CASUS[i].unitates);
        piscina_destruere(p);
    }
    {
                /* indicia versuum LF: lineae IV, stropha aequalis (~XLV) - regula
         * interpuncta eam capit, non voluntaria (nihil 'cepisset') */

        Piscina* p = piscina_generare_dynamicum("forma_indicia",
            4194304);
        OratioIndicia indicia;
                  i32 unitates;

        _forma_textus(p, CASUS[ZEPHYRUM].textus, &unitates, &indicia);
        CREDO_AEQUALIS_I32 (indicia.lineae, (i32)IV);
        CREDO_AEQUALIS_I32 (indicia.fracturae, (i32)III);
                CREDO_VERUM (indicia.interpunctae_pct >= (i32)75);

        CREDO_AEQUALIS_I32 (indicia.indentatio_maxima
            - indicia.indentatio_minima, (i32)IV);
        CREDO_VERUM (indicia.latitudo <= (i32)80);
        /* tabula spatiis: columnatae omnes */
        _forma_textus(p, CASUS[IX].textus, &unitates, &indicia);
        CREDO_AEQUALIS_I32 (indicia.columnatae_pct, (i32)100);
        piscina_destruere(p);
    }

    imprimere("\n--- III. Census formarum super fixturas ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXTURAE)
        / magnitudo(FIXTURAE[0])); i++)
    {
        character  via[DXII];
          Piscina* p = piscina_generare_dynamicum("forma_fixtura",
              33554432);
        character* textus;
              i32  mensura = ZEPHYRUM;
           Census  c;
     MateriaNodus* doc;

        memset(&c, ZEPHYRUM, magnitudo(c));
        sprintf(via, "%s/%s", radix_viae, FIXTURAE[i]);
        textus = _plagulam_legere(p, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus != NIHIL)
        {
            doc = oratio_arbor_parsare(p, textus, mensura);
            CREDO_NON_NIHIL (doc);
            si (doc != NIHIL)
            {
                _censere(doc, &c, FIXTURAE[i], textus, p);
                _censum_imprimere(FIXTURAE[i], &c);
                si (strstr(FIXTURAE[i], "propertius") != NIHIL)
                {
                    CREDO_VERUM (c.formae[ORATIO_FORMA_VERSUS]
                        > c.formae[ORATIO_FORMA_PROSA]);
                    CREDO_VERUM (c.sententiae >= (s64)200);
                }
                alioquin si (strstr(FIXTURAE[i], "trinity") == NIHIL)
                {
                    CREDO_VERUM (c.formae[ORATIO_FORMA_PROSA]
                        > c.formae[ORATIO_FORMA_VERSUS]);
                    CREDO_VERUM (c.formae[ORATIO_FORMA_PROSA]
                        + c.formae[ORATIO_FORMA_TITULUS]
                        >= c.paragraphi * (s64)IV / (s64)V);
                }
            }
        }
        piscina_destruere(p);
    }

    imprimere("\n--- IV. Iudicia manu facta (fixa/iudicia) ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(IUDICIA)
        / magnitudo(IUDICIA[0])); i++)
    {
        character  via[DXII];
          Piscina* p = piscina_generare_dynamicum("forma_iudicium",
              8388608);
              Xar* initia = xar_creare(p, (i32)magnitudo(s32));
        character* textus;
              i32  mensura = ZEPHYRUM;
     MateriaNodus* doc;
              Xar* sententiae;
              i32  congruentia = ZEPHYRUM;
              i32  iudicata;
              i32  orationis   = ZEPHYRUM;
              i32  a;
              i32  impressae   = ZEPHYRUM;

        sprintf(via, "%s/%s", radix_viae, IUDICIA[i].via);
        textus = _iudicia_legere(p, via, &mensura, initia);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            piscina_destruere(p);
            perge;
        }
        iudicata = xar_numerus(initia);
        CREDO_MAIOR_I32 (iudicata, ZEPHYRUM);
        doc = oratio_arbor_parsare(p, textus, mensura);
        CREDO_NON_NIHIL (doc);
        sententiae = doc != NIHIL ? oratio_sententiae_extenta(p, doc)
                                  : NIHIL;
        si (sententiae != NIHIL)
        {
            orationis = xar_numerus(sententiae);
            per (a = ZEPHYRUM; a < iudicata; a++)
            {
                s32 initium = *(s32*)xar_obtinere(initia, a);
                i32 s;
                b32 inventum = FALSUM;

                per (s = ZEPHYRUM; s < orationis; s++)
                {
                    constans OratioSententia* x =
                        (constans OratioSententia*)xar_obtinere(sententiae,
                        s);

                    si (x->initium == initium)
                    {
                        inventum = VERUM;
                        frange;
                    }
                }
                si (inventum)
                {
                    congruentia = congruentia + I;
                }
                alioquin si (impressae < (i32)VI)
                {
                    s32 k;

                    imprimere("    absens @%d: ", (integer)initium);
                    per (k = initium; k < initium + (s32)50
                        && k < (s32)mensura
                        && textus[k] != '\n' && textus[k] != '\r'; k++)
                    {
                        putchar(textus[k]);
                    }
                    putchar('\n');
                    impressae = impressae + I;
                }
            }
        }
                /* superflua: initia orationis non iudicata (scissiones falsae) */
        si (sententiae != NIHIL)
        {
            i32 s;

            impressae = ZEPHYRUM;
            per (s = ZEPHYRUM; s < orationis
                && impressae < (i32)VI; s++)
            {
                constans OratioSententia* x =
                    (constans OratioSententia*)xar_obtinere(sententiae,
                    s);
                                     b32 iudicatum = FALSUM;

                per (a = ZEPHYRUM; a < iudicata; a++)
                {
                    si (*(s32*)xar_obtinere(initia, a) == x->initium)
                    {
                        iudicatum = VERUM;
                        frange;
                    }
                }
                si (!iudicatum)
                {
                    s32 k;

                    imprimere("    superfluum @%d: ",
                        (integer)x->initium);
                    per (k = x->initium; k < x->initium + (s32)50
                        && k < (s32)mensura && textus[k] != '\n'
                        && textus[k] != '\r'; k++)
                    {
                        putchar(textus[k]);
                    }
                    putchar('\n');
                    impressae = impressae + I;
                }
            }
        }
        imprimere("  %-52s iudicata %3d  oratio %3d  congruentia %3d  (pinna %d)\n",

            IUDICIA[i].via, (integer)iudicata, (integer)orationis,
            (integer)congruentia, (integer)IUDICIA[i].pinna);
        CREDO_VERUM (congruentia >= IUDICIA[i].pinna);
        piscina_destruere(p);
    }

    imprimere("\n--- V. Census super paragraphos md (specimen quinta quaeque) ---\n");
    {
        character via[DXII];
        FILE* lista;
        i32 numerus_lineae = ZEPHYRUM;
        i32 plagulae = ZEPHYRUM;
        Census md;

        memset(&md, ZEPHYRUM, magnitudo(md));
        md.specimina = (b32)(getenv("ORATIO_FORMA_SPECIMINA") != NIHIL);
        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                  Piscina* p;
                character* textus;
                      i32  mensura  = ZEPHYRUM;
                   size_t  l        = strlen(linea);
                MateriaNodus* arbor;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                numerus_lineae++;
                si (   getenv("ORATIO_CORPUS_TOTUS") == NIHIL
                    && (numerus_lineae % V)          != ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("forma_corpus",
                    33554432);
                sprintf(via, "%s/%s", radix_viae, linea);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus != NIHIL)
                {
                    plagulae++;
                    arbor = md_arbor_parsare(p, textus, mensura);
                    _paragraphos_censere(linea, arbor, textus, p, &md);
                }
                piscina_destruere(p);
            }
            fclose(lista);
        }
        imprimere("  plagulae %d:", (integer)plagulae);
        _censum_imprimere("md", &md);
        CREDO_VERUM (plagulae > (i32)100);
        CREDO_VERUM (md.paragraphi > (s64)1000);
        CREDO_VERUM (md.formae[ORATIO_FORMA_PROSA]
            + md.formae[ORATIO_FORMA_TITULUS]
            >= md.paragraphi * (s64)IV / (s64)V);
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
