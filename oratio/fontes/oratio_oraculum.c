/* oratio_oraculum.c - Vide oratio_oraculum.h. */

#include "oratio_oraculum.h"
#include "oratio_partes.h"
#include "oratio_lexicon.h"
#include "oratio_stml.h"
#include "materia_lexicon.h"
#include "internamentum.h"
#include "oratio_arbor.h"
#include "oratio_partes.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include <string.h>
#include <stdio.h>

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior b32
_aequalis (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, (size_t)a.mensura)
                == ZEPHYRUM));
}

interior b32
_literis (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

hic_manens constans character* constans UPOS[] = {
    "NOUN", "PROPN", "VERB", "AUX", "ADJ", "ADV", "PRON", "DET", "ADP",
    "NUM", "CCONJ", "SCONJ", "PART", "INTJ", "SYM", "PUNCT", "X"
};

OratioClassis
oratio_oraculum_classis_ex_upos (
    chorda upos)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        si (_literis(upos, UPOS[i]))
        {
            redde (OratioClassis)i;
        }
    }
    redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;
}

vacuum
oratio_oraculum_census_vacare (
    OratioOraculumCensus* census)
{
    memset(census, ZEPHYRUM, magnitudo(*census));
    census->lingua_documenti = (s32)-I;
}

/* T29 NOTAE: tabula accidentium contra aurum UD - clavis aurea, titulus
 * loci nostri, valores (chorda aurea -> index noster; alter = index
 * alter acceptus, -I nullus). Vox: deponens (WORDS) utrique acceptum -
 * conventio, non iudicium. Tempus compositum (Tense + Aspect) per
 * _tempus_aureum. */
nomen structura {
    constans character* valor_aureus;
                   s32  index;
                   s32  alter;
} ValorNotae;

nomen structura {
     constans character* clavis;      /* UD */
     constans character* accidens;    /* titulus loci */
    constans ValorNotae* valores;
                    i32  numerus;
    constans character* constans* tituli;   /* T30: tituli valorum nostrorum */
                    i32 numerus_titulorum;
} NotaOraculi;

hic_manens constans ValorNotae VALORES_NUMERI[] = {
    { "Sing", (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS, (s32)-I },
    { "Plur", (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS, (s32)-I }
};
/* T30: commune (WORDS 'C': masculinum aut femininum - dies, trux,
 * tu/vos, participia) utrique acceptum ut deponens voci - CONVENTIO
 * (census: tertia pars ad duas partes falsorum generis erat) */
hic_manens constans ValorNotae VALORES_GENERIS[] = {
    { "Masc", (s32)ORATIO_GENUS_GRAMMATICUM_MASCULINUM,
      (s32)ORATIO_GENUS_GRAMMATICUM_COMMUNE },
    { "Fem", (s32)ORATIO_GENUS_GRAMMATICUM_FEMININUM,
      (s32)ORATIO_GENUS_GRAMMATICUM_COMMUNE },
    { "Neut", (s32)ORATIO_GENUS_GRAMMATICUM_NEUTRUM, (s32)-I },
    { "Com", (s32)ORATIO_GENUS_GRAMMATICUM_COMMUNE, (s32)-I }
};
hic_manens constans ValorNotae VALORES_PERSONAE[] = {
    { "1", (s32)ORATIO_PERSONA_PRIMA, (s32)-I },
    { "2", (s32)ORATIO_PERSONA_SECUNDA, (s32)-I },
    { "3", (s32)ORATIO_PERSONA_TERTIA, (s32)-I }
};
hic_manens constans ValorNotae VALORES_MODI[] = {
    { "Ind", (s32)ORATIO_MODUS_INDICATIVUS, (s32)-I },
    { "Sub", (s32)ORATIO_MODUS_SUBIUNCTIVUS, (s32)-I },
    { "Imp", (s32)ORATIO_MODUS_IMPERATIVUS, (s32)-I }
};
hic_manens constans ValorNotae VALORES_VOCIS[] = {
    { "Act", (s32)ORATIO_VOX_ACTIVA, (s32)ORATIO_VOX_DEPONENS },
    { "Pass", (s32)ORATIO_VOX_PASSIVA, (s32)ORATIO_VOX_DEPONENS }
};
/* T30 d: gerundivum (WORDS VPAR FUT PASSIVE) pro auro Part acceptum -
 * chartae, Aquinas, Perseus, Dante formas -nd- omnes 'Part' notant
 * (CIRCSE et PROIEL solae Gdv/Ger) - et pro Ger: gerundium lectionem
 * propriam non habet (WORDS gerundivum neutrum) - CONVENTIONES, numero
 * 'conventione' visibiles */
hic_manens constans ValorNotae VALORES_FORMAE_VERBI[] = {
    { "Fin", (s32)ORATIO_FORMA_VERBI_FINITUM, (s32)-I },
    { "Inf", (s32)ORATIO_FORMA_VERBI_INFINITIVUM, (s32)-I },
    { "Part", (s32)ORATIO_FORMA_VERBI_PARTICIPIUM,
      (s32)ORATIO_FORMA_VERBI_GERUNDIVUM },
    { "Ger", (s32)ORATIO_FORMA_VERBI_GERUNDIUM,
      (s32)ORATIO_FORMA_VERBI_GERUNDIVUM },
    { "Gdv", (s32)ORATIO_FORMA_VERBI_GERUNDIVUM, (s32)-I },
    { "Sup", (s32)ORATIO_FORMA_VERBI_SUPINUM, (s32)-I }
};
hic_manens constans NotaOraculi NOTAE_ORACULI[ORATIO_ORACULUM_NOTAE] = {
    { "Number", "numerus", VALORES_NUMERI, (i32)II,
      ORATIO_TITULI_NUMERORUM,
          (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS },
    { "Gender", "genus", VALORES_GENERIS, (i32)IV,
      ORATIO_TITULI_GENERUM_GRAMMATICORUM,
      (i32)ORATIO_GENUS_GRAMMATICUM_NUMERUS },
    { "Person", "persona", VALORES_PERSONAE, (i32)III,
      ORATIO_TITULI_PERSONARUM, (i32)ORATIO_PERSONA_NUMERUS },
    { "Mood", "modus", VALORES_MODI, (i32)III,
      ORATIO_TITULI_MODORUM, (i32)ORATIO_MODUS_NUMERUS },
    { "Voice", "vox", VALORES_VOCIS, (i32)II,
      ORATIO_TITULI_VOCUM, (i32)ORATIO_VOX_NUMERUS },
    { "VerbForm", "forma-verbi", VALORES_FORMAE_VERBI, (i32)VI,
      ORATIO_TITULI_FORMARUM_VERBI, (i32)ORATIO_FORMA_VERBI_NUMERUS },
    { "Tense", "tempus", NIHIL, ZEPHYRUM,
      ORATIO_TITULI_TEMPORUM, (i32)ORATIO_TEMPUS_NUMERUS }
};

constans character*
oratio_oraculum_nota_titulus (
    i32 k)
{
    redde k >= ZEPHYRUM && k < ORATIO_ORACULUM_NOTAE
        ? NOTAE_ORACULI[k].accidens : NIHIL;
}

constans character*
oratio_oraculum_nota_valor_titulus (
    i32 k,
    s32 v)
{
    redde k >= ZEPHYRUM && k < ORATIO_ORACULUM_NOTAE && v >= ZEPHYRUM
        && v < (s32)NOTAE_ORACULI[k].numerus_titulorum
        ? NOTAE_ORACULI[k].tituli[v] : NIHIL;
}

/* discrepantiam notare (T19a): clavis 'aurea/nostra/forma plicata' in
 * indice; cella nova in Xar (cellae xar numquam moventur - monstrator
 * in indice stabilis) aut numerus cellae inventae auctus. Forma
 * monstrosa (> CC octeti) non tabulatur - numquam ruit. */
interior vacuum
_discrepantiam_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
           OratioClassis  aurea,
                  chorda  forma,
           OratioClassis  nostra)
{
                     character*  clavis_datum;
                        chorda   plicata;
                        chorda   clavis;
                        vacuum*  valor;
    OratioOraculumDiscrepantia*  d;
    OratioOraculumDiscrepantia** cella;
                           i32   praefixum;

    si (census->discrepantiae == NIHIL)
    {
        census->discrepantiae = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumDiscrepantia*));
        census->discrepantiae_index = tabula_dispersa_creare_chorda(
            piscina, (i32)1024);
        si (   census->discrepantiae       == NIHIL
            || census->discrepantiae_index == NIHIL)
        {
            census->discrepantiae = NIHIL;
            redde;
        }
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
    /* clavis binaria in piscina (forma quaelibet - URL aurea PROPN
     * CCXL octetorum lex summae bis fregit sub tabula stationaria):
     * octetus classis aureae, octetus nostrae, forma; chorda mensuram
     * fert, octeti nulli licent; sine stdio */
    praefixum    = (i32)II;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)(plicata.mensura + praefixum));
    si (plicata.datum == NIHIL || clavis_datum == NIHIL)
    {
        redde;
    }
    clavis_datum[ZEPHYRUM]  = (character)(i32)aurea;
    clavis_datum[I]         = (character)(i32)nostra;
    memcpy(clavis_datum + praefixum, plicata.datum,
        (size_t)plicata.mensura);
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = (i32)praefixum + plicata.mensura;
    si (tabula_dispersa_invenire(census->discrepantiae_index, clavis,
            &valor))
    {
        d           = (OratioOraculumDiscrepantia*)valor;
        d->numerus  = d->numerus + I;
        redde;
    }
    d = (OratioOraculumDiscrepantia*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*d));
    cella = (OratioOraculumDiscrepantia**)xar_addere(
        census->discrepantiae);
    si (d == NIHIL || cella == NIHIL)
    {
        redde;
    }
    d->aurea    = aurea;
    d->nostra   = nostra;
    d->forma    = plicata;
    d->numerus  = I;
    *cella      = d;
    (vacuum)tabula_dispersa_inserere(census->discrepantiae_index,
        clavis, d);
}

/* ordo: numerus maior prior, deinde forma octetim, deinde classis */
interior s32
_discrepantias_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans OratioOraculumDiscrepantia* x =
        *(OratioOraculumDiscrepantia* constans*)a;
    constans OratioOraculumDiscrepantia* y =
        *(OratioOraculumDiscrepantia* constans*)b;
    i32 minima;
    s32 c;

    si (x->numerus != y->numerus)
    {
        redde x->numerus > y->numerus ? (s32)-I : (s32)I;
    }
    minima = x->forma.mensura < y->forma.mensura ? x->forma.mensura
        : y->forma.mensura;
    c = (s32)memcmp(x->forma.datum, y->forma.datum, (size_t)minima);
    si (c != ZEPHYRUM)
    {
        redde c < ZEPHYRUM ? (s32)-I : (s32)I;
    }
    si (x->forma.mensura != y->forma.mensura)
    {
        redde x->forma.mensura < y->forma.mensura ? (s32)-I : (s32)I;
    }
    si ((i32)x->nostra != (i32)y->nostra)
    {
        redde (i32)x->nostra < (i32)y->nostra ? (s32)-I : (s32)I;
    }
    redde ZEPHYRUM;
}

Xar*
oratio_oraculum_discrepantiae (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                    OratioClassis  aurea)
{
    Xar* exitus;
    i32  i;
    i32  n;

    exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumDiscrepantia*));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->discrepantiae == NIHIL)
    {
        redde exitus;
    }
    n = xar_numerus(census->discrepantiae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        OratioOraculumDiscrepantia* d =
            *(OratioOraculumDiscrepantia**)xar_obtinere(
                census->discrepantiae, i);
        OratioOraculumDiscrepantia** cella;

        si (d == NIHIL || d->aurea != aurea)
        {
            perge;
        }
        cella = (OratioOraculumDiscrepantia**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = d;
    }
    xar_ordinare(exitus, _discrepantias_comparare);
    redde exitus;
}


/* ==================================================
 * Elementa arboris cum extentibus et classibus
 * ================================================== */

#define CLASSES_MAXIMAE 17
#define LEMMATA_MAXIMA  64

nomen structura {
    s32 a;                             /* extentum octetorum in textu */
    s32 b;
    chorda classes;                    /* lexema derivatum (vocabulum) aut titulus */
    i32 numerus_classium;
    OratioClassis classis[CLASSES_MAXIMAE];
        i32 numerus_lemmatum;
    chorda lemmata[LEMMATA_MAXIMA];    /* plicata */
    b32 ignotum;
        s32 decisio;                       /* OratioDecisio; -I = nemo (T19g) */
    i32 numerus_analysium;             /* lectiones (candidata) */
                chorda auctor;                     /* titulus regulae decidentis (T19g) */
    constans MateriaNodus* nodus;      /* pro socio ligationis (T19g bis) */
                s32 clausula;                      /* T20a: -I = aperta */
        s32 clausula_causa;
                s32 casus_primus;                  /* T23: casus lectionis primae Latinae; -I = nullus */
                s32 notae_primae[ORATIO_ORACULUM_NOTAE];   /* T29: accidentia lectionis primae Latinae; -I = nullum */
        i32 notae_praesentes[ORATIO_ORACULUM_NOTAE];   /* T30: mascula bitium valorum quos lectiones Latinae classis primae ferunt */
        s32 lexema;                        /* T26: index lexematis aurei alignati; -I */
} Elementum;

constans character* constans ORATIO_ORACULUM_TITULI_PARTITIONIS[] = {
    "praelatio", "impletio", "umbra", "una", "aperta", "nulla", "ranga"
};

/* auctorem notare (T19g): clavis = titulus (copia in piscina iudicii
 * ut clavis stabilis sit - titulus elementi in piscina sententiae
 * vivit); cella nova aut numeri aucti. Ut _discrepantiam_notare. */
/* casus_iudicatus (T23): -I extra populum, 0 falsus, I rectus */
/* auctorem titulo capere aut creare (tabula per titulum); NIHIL =
 * memoria. T32 a: communis decisionibus verborum et arcubus umbrarum. */
interior OratioOraculumAuctor*
_auctorem_capere (
                 Piscina* piscina,
    OratioOraculumCensus* census,
                  chorda  titulus)
{
                   vacuum*  valor;
     OratioOraculumAuctor*  a;
     OratioOraculumAuctor** cella;
                   chorda   clavis;

    si (census->auctores == NIHIL)
    {
        census->auctores = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumAuctor*));
        census->auctores_index = tabula_dispersa_creare_chorda(piscina,
            (i32)256);
        si (   census->auctores       == NIHIL
            || census->auctores_index == NIHIL)
        {
            census->auctores = NIHIL;
            redde NIHIL;
        }
    }
    si (tabula_dispersa_invenire(census->auctores_index, titulus,
            &valor))
    {
        redde (OratioOraculumAuctor*)valor;
    }
    clavis = _copia(piscina, titulus);
    a      = (OratioOraculumAuctor*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*a));
    cella  = (OratioOraculumAuctor**)xar_addere(census->auctores);
    si (clavis.datum == NIHIL || a == NIHIL || cella == NIHIL)
    {
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->titulus  = clavis;
    *cella      = a;
    (vacuum)tabula_dispersa_inserere(census->auctores_index, clavis, a);
    redde a;
}

/* T32 a: arcum umbrae auctori notare (umbra lectionis primae impleta,
 * auctor = titulus regulae implentis) */
interior vacuum
_auctorem_ligationem_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
                  chorda  titulus,
                     b32  recta,
                     b32  vicina)
{
    OratioOraculumAuctor* a = _auctorem_capere(piscina, census,
        titulus);

    si (a == NIHIL)
    {
        redde;
    }
    a->ligationes = a->ligationes + I;
    si (recta)
    {
        a->ligationes_rectae = a->ligationes_rectae + I;
    }
    si (vicina)
    {
        a->ligationes_vicinae = a->ligationes_vicinae + I;
        si (recta)
        {
            a->ligationes_vicinae_rectae = a->ligationes_vicinae_rectae
                + I;
        }
    }
}

interior vacuum
_auctorem_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
                  chorda  titulus,
                     b32  primaria,
                     s32  distantia,
                     s32  casus_iudicatus)
{
     OratioOraculumAuctor* a;

    a = _auctorem_capere(piscina, census, titulus);
    si (a == NIHIL)
    {
        redde;
    }
        a->verba = a->verba + I;
    si (casus_iudicatus >= ZEPHYRUM)
    {
        a->casus_verba = a->casus_verba + I;
        a->casus_recti = a->casus_recti
            + (casus_iudicatus > ZEPHYRUM ? I : ZEPHYRUM);
    }
    si (primaria)
    {
        a->primaria = a->primaria + I;
    }
    si (distantia == (s32)I)
    {
        a->vicina = a->vicina + I;
        si (primaria)
        {
            a->vicina_primaria = a->vicina_primaria + I;
        }
    }
    alioquin si (distantia > (s32)I)
    {
        a->remota = a->remota + I;
        si (primaria)
        {
            a->remota_primaria = a->remota_primaria + I;
        }
    }
}

interior s32
_auctores_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumAuctor* x =
        *(constans OratioOraculumAuctor* constans*)p;
    constans OratioOraculumAuctor* y =
        *(constans OratioOraculumAuctor* constans*)q;

    si (x->verba != y->verba)
    {
        redde x->verba > y->verba ? (s32)-I : (s32)I;
    }
    redde chorda_comparare(x->titulus, y->titulus);
}

Xar*
oratio_oraculum_auctores (
                          Piscina* piscina,
    constans OratioOraculumCensus* census)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumAuctor*));
    i32 i;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->auctores == NIHIL)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(census->auctores); i++)
    {
        OratioOraculumAuctor** cella =
            (OratioOraculumAuctor**)xar_addere(exitus);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = *(OratioOraculumAuctor**)xar_obtinere(census->auctores,
            i);
    }
        xar_ordinare(exitus, _auctores_comparare);
    redde exitus;
}

interior vacuum
_extentum_listae (
    constans MateriaValor* v,
                      s32* a,
                      s32* b);

/* umbrae lectionis per titulum loci classis; NIHIL si absunt */
interior constans MateriaValor*
_umbrae_lectionis (
    constans MateriaNodus* analysis)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    locus = oratio_partes_locus(classis, "umbrae");
    si (   locus < ZEPHYRUM
        || analysis->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    redde &analysis->loci[locus];
}

/* index vocabuli implentis umbrae cuiusvis lectionis (lectio prima
 * sola si prima_sola); -I si nulla ligata */
interior s32
_ligatum (
    constans MateriaNodus* vocabulum,
                      b32  prima_sola,
                      s32  petitum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
                      i32 n;
                      i32 a;

    si (analyses->genus != MATERIA_VALOR_LISTA)
    {
        redde (s32)-I;
    }
    n = prima_sola ? I : materia_valor_lista_numerus(*analyses);
    per (a = ZEPHYRUM; a < n
        && a < materia_valor_lista_numerus(*analyses);
         a++)
    {
        constans MateriaValor* valor =
            materia_valor_lista_obtinere(*analyses, a);
        constans MateriaValor* umbrae;
                          i32  u;

        si (valor == NIHIL || valor->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        umbrae = _umbrae_lectionis(valor->datum.nodus);
        si (umbrae == NIHIL)
        {
            perge;
        }
        per (u = ZEPHYRUM; u
            < materia_valor_lista_numerus(*umbrae); u++)
        {
            constans MateriaValor* vu =
                materia_valor_lista_obtinere(*umbrae, u);
            constans MateriaValor* w;

            si (vu == NIHIL || vu->genus != MATERIA_VALOR_NODUS)
            {
                perge;
            }
            w = &vu->datum.nodus->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM];
            si (   w->genus == MATERIA_VALOR_INDEX
                && (petitum < ZEPHYRUM || w->datum.index == petitum))
            {
                redde w->datum.index;
            }
        }
    }
    redde (s32)-I;
}

/* SOCIUS ligationis (T19g bis): vocabulum quocum lectio PRIMA huius
 * ligata est - umbra impleta lectionis primae (carrier), aliter umbra
 * lectionis cuiusvis VICINI in hoc vocabulum spectans (implens); forma
 * ex textu per extentum partium; vacua si nullus aut sine patre */
interior chorda
_socius (
                   chorda  textus,
    constans MateriaNodus* vocabulum,
                      s32* distantia)
{
    constans MateriaNodus* sententia = vocabulum->pater;
    constans MateriaValor* elementa;
                      i32  ne;
                      i32  k;
                      s32  socius = (s32)-I;
                   chorda  vacua;

        vacua.datum  = NIHIL;
    vacua.mensura    = ZEPHYRUM;
    *distantia       = (s32)-I;
    si (   sententia        == NIHIL
        || sententia->genus != (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde vacua;
    }
    elementa = &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
    si (elementa->genus != MATERIA_VALOR_LISTA)
    {
        redde vacua;
    }
    ne = materia_valor_lista_numerus(*elementa);
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(*elementa,
            k);

        si (   v != NIHIL && v->genus == MATERIA_VALOR_NODUS
            && v->datum.nodus == vocabulum)
        {
            frange;
        }
    }
    si (k >= ne)
    {
        redde vacua;
    }
        socius = _ligatum(vocabulum, VERUM, (s32)-I);
    si (socius < ZEPHYRUM)
    {
        /* implens: carrier PROXIMUS cuius umbra huc spectat - regulae
         * sequentes cursu fratrum ultra vicinum ligant, distantia
         * relata */
        i32 d;
        i32 proximitas = ne + I;

        per (d = ZEPHYRUM; d < ne; d++)
        {
            constans MateriaValor* v;
                              i32  spatium = d > k ? d - k : k - d;

            si (d == k || spatium >= proximitas)
            {
                perge;
            }
            v = materia_valor_lista_obtinere(*elementa, d);
            si (   v != NIHIL && v->genus == MATERIA_VALOR_NODUS
                && v->datum.nodus->genus == (s32)ORATIO_GENUS_VOCABULUM
                && _ligatum(v->datum.nodus, FALSUM, (s32)k) == (s32)k)
            {
                socius      = (s32)d;
                proximitas  = spatium;
            }
        }
    }
    si (socius >= ZEPHYRUM && socius < (s32)ne)
    {
        *distantia = (s32)socius > (s32)k ? (s32)socius - (s32)k
            : (s32)k - (s32)socius;
    }

    si (socius >= ZEPHYRUM && socius < (s32)ne)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(*elementa,
            (i32)socius);
                          s32 a = (s32)-I;
                          s32 b = ZEPHYRUM;

        si (   v != NIHIL && v->genus == MATERIA_VALOR_NODUS
            && v->datum.nodus->genus == (s32)ORATIO_GENUS_VOCABULUM)
        {
            _extentum_listae(
                &v->datum.nodus->loci[ORATIO_VOCABULUM_PARTES], &a, &b);
            si (a >= ZEPHYRUM && b > a && b <= (s32)textus.mensura)
            {
                redde _chorda(textus.datum + a, (i32)(b - a));
            }
        }
    }
    redde vacua;
}

/* erratum notare (T19g bis): clavis binaria [aurea][nostra] auctor 0x01
 * forma 0x01 socius in piscina iudicii; cella nova aut numerus auctus */
interior vacuum
_erratum_notare (
                                   Piscina* piscina,
                      OratioOraculumCensus* census,
                                    chorda  auctor,
                                    chorda  forma,
                             OratioClassis  aurea,
                             OratioClassis  nostra,
                                    chorda  socius,
                                       s32  distantia)
{
                 character*  clavis_datum;
                    chorda   plicata;
                    chorda   clavis;
                    vacuum*  valor;
     OratioOraculumErratum*  d;
     OratioOraculumErratum** cella;
                       i32   n;

    si (census->errata == NIHIL)
    {
        census->errata = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumErratum*));
        census->errata_index = tabula_dispersa_creare_chorda(piscina,
            (i32)1024);
        si (census->errata == NIHIL || census->errata_index == NIHIL)
        {
            census->errata = NIHIL;
            redde;
        }
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
        n = (i32)III + auctor.mensura + I + plicata.mensura + I
            + socius.mensura;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)n);
    si (plicata.datum == NIHIL || clavis_datum == NIHIL)
    {
        redde;
    }
        clavis_datum[ZEPHYRUM]  = (character)(i32)aurea;
    clavis_datum[I]             = (character)(i32)nostra;
    clavis_datum[II]        = (character)(distantia < ZEPHYRUM
        ? (i32)255 : (distantia
            > (s32)254 ? (i32)254 : (i32)distantia));
    n = (i32)III;

    memcpy(clavis_datum + n, auctor.datum, (size_t)auctor.mensura);
    n                = n + auctor.mensura;
    clavis_datum[n]  = (character)I;
    n                = n + I;
    memcpy(clavis_datum + n, plicata.datum, (size_t)plicata.mensura);
    n                = n + plicata.mensura;
    clavis_datum[n]  = (character)I;
    n                = n + I;
    si (socius.mensura > ZEPHYRUM)
    {
        memcpy(clavis_datum + n, socius.datum, (size_t)socius.mensura);
        n = n + socius.mensura;
    }
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = n;
    si (tabula_dispersa_invenire(census->errata_index, clavis, &valor))
    {
        d           = (OratioOraculumErratum*)valor;
        d->numerus  = d->numerus + I;
        redde;
    }
    d = (OratioOraculumErratum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*d));
    cella = (OratioOraculumErratum**)xar_addere(census->errata);
    si (d == NIHIL || cella == NIHIL)
    {
        redde;
    }
    d->auctor  = _copia(piscina, auctor);
    d->forma   = plicata;
    d->aurea   = aurea;
    d->nostra  = nostra;
        d->socius   = socius.mensura > ZEPHYRUM ? _copia(piscina,
            socius)
            : socius;
    d->distantia  = distantia;
    d->numerus    = I;
    *cella        = d;
    (vacuum)tabula_dispersa_inserere(census->errata_index, clavis, d);
}

interior s32
_errata_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumErratum* x =
        *(constans OratioOraculumErratum* constans*)p;
    constans OratioOraculumErratum* y =
        *(constans OratioOraculumErratum* constans*)q;
    s32 c;

    si (x->numerus != y->numerus)
    {
        redde x->numerus > y->numerus ? (s32)-I : (s32)I;
    }
    c = chorda_comparare(x->auctor, y->auctor);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    c = chorda_comparare(x->forma, y->forma);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde chorda_comparare(x->socius, y->socius);
}

Xar*
oratio_oraculum_errata (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                           chorda  auctor)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumErratum*));
    i32 i;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->errata == NIHIL)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(census->errata); i++)
    {
        OratioOraculumErratum* d =
            *(OratioOraculumErratum**)xar_obtinere(census->errata, i);
        OratioOraculumErratum** cella;

        si (   auctor.mensura > ZEPHYRUM
            && chorda_comparare(d->auctor, auctor) != ZEPHYRUM)
        {
            perge;
        }
        cella = (OratioOraculumErratum**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = d;
    }
    xar_ordinare(exitus, _errata_comparare);
    redde exitus;
}

/* erratum notae notare (T30): clavis binaria [k][classis][aurea][nostra]
 * [distantia][attingibile][decisio + I] auctor 0x01 forma 0x01 socius in
 * piscina iudicii; cella nova aut numerus auctus */
interior vacuum
_erratum_notae_notare (
                                   Piscina* piscina,
                      OratioOraculumCensus* census,
                                       i32  nota,
                             OratioClassis  classis,
                                       s32  aurea,
                                       s32  nostra,
                                       s32  decisio,
                                    chorda  auctor,
                                    chorda  forma,
                                    chorda  socius,
                                       s32  distantia,
                                       b32  attingibile)
{
                      character*  clavis_datum;
                         chorda   plicata;
                         chorda   clavis;
                         vacuum*  valor;
     OratioOraculumErratumNotae*  d;
     OratioOraculumErratumNotae** cella;
                            i32   n;

    si (census->errata_notarum == NIHIL)
    {
        census->errata_notarum = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumErratumNotae*));
        census->errata_notarum_index = tabula_dispersa_creare_chorda(
            piscina, (i32)1024);
        si (   census->errata_notarum       == NIHIL
            || census->errata_notarum_index == NIHIL)
        {
            census->errata_notarum = NIHIL;
            redde;
        }
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
    n = (i32)VII + auctor.mensura + I + plicata.mensura + I
        + socius.mensura;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)n);
    si (plicata.datum == NIHIL || clavis_datum == NIHIL)
    {
        redde;
    }
    clavis_datum[ZEPHYRUM]  = (character)(i32)nota;
    clavis_datum[I]         = (character)(i32)classis;
    clavis_datum[II]       = (character)(aurea < ZEPHYRUM
        ? (i32)255 : (i32)aurea);
    clavis_datum[III]      = (character)(nostra < ZEPHYRUM
        ? (i32)255 : (i32)nostra);
    clavis_datum[IV]       = (character)(distantia < ZEPHYRUM
        ? (i32)255 : (distantia
            > (s32)254 ? (i32)254 : (i32)distantia));
    clavis_datum[V]        =
        (character)(attingibile ? (i32)I : ZEPHYRUM);
    clavis_datum[VI]       = (character)(decisio < ZEPHYRUM
        ? ZEPHYRUM : (i32)decisio + I);
    n = (i32)VII;
    memcpy(clavis_datum + n, auctor.datum, (size_t)auctor.mensura);
    n                = n + auctor.mensura;
    clavis_datum[n]  = (character)I;
    n                = n + I;
    memcpy(clavis_datum + n, plicata.datum, (size_t)plicata.mensura);
    n                = n + plicata.mensura;
    clavis_datum[n]  = (character)I;
    n                = n + I;
    si (socius.mensura > ZEPHYRUM)
    {
        memcpy(clavis_datum + n, socius.datum, (size_t)socius.mensura);
        n = n + socius.mensura;
    }
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = n;
    si (tabula_dispersa_invenire(census->errata_notarum_index, clavis,
            &valor))
    {
        d           = (OratioOraculumErratumNotae*)valor;
        d->numerus  = d->numerus + I;
        redde;
    }
    d = (OratioOraculumErratumNotae*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*d));
    cella = (OratioOraculumErratumNotae**)xar_addere(
        census->errata_notarum);
    si (d == NIHIL || cella == NIHIL)
    {
        redde;
    }
    d->nota     = nota;
    d->classis  = classis;
    d->aurea    = aurea;
    d->nostra   = nostra;
    d->decisio  = decisio;
    d->auctor       = auctor.mensura > ZEPHYRUM ? _copia(piscina,
        auctor)
        : auctor;
    d->forma        = plicata;
    d->socius       = socius.mensura > ZEPHYRUM ? _copia(piscina,
        socius)
        : socius;
    d->distantia    = distantia;
    d->attingibile  = attingibile;
    d->numerus      = I;
    *cella          = d;
    (vacuum)tabula_dispersa_inserere(census->errata_notarum_index,
        clavis,
        d);
}

interior s32
_errata_notarum_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumErratumNotae* x =
        *(constans OratioOraculumErratumNotae* constans*)p;
    constans OratioOraculumErratumNotae* y =
        *(constans OratioOraculumErratumNotae* constans*)q;
    s32 c;

    si (x->numerus != y->numerus)
    {
        redde x->numerus > y->numerus ? (s32)-I : (s32)I;
    }
    si (x->nota != y->nota)
    {
        redde x->nota < y->nota ? (s32)-I : (s32)I;
    }
    si (x->classis != y->classis)
    {
        redde (i32)x->classis < (i32)y->classis ? (s32)-I : (s32)I;
    }
    c = chorda_comparare(x->forma, y->forma);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde chorda_comparare(x->socius, y->socius);
}

Xar*
oratio_oraculum_errata_notarum (
                          Piscina* piscina,
    constans OratioOraculumCensus* census,
                              s32  nota)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumErratumNotae*));
    i32 i;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->errata_notarum == NIHIL)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(census->errata_notarum); i++)
    {
        OratioOraculumErratumNotae* d =
            *(OratioOraculumErratumNotae**)xar_obtinere(
            census->errata_notarum, i);
        OratioOraculumErratumNotae** cella;

        si (nota >= ZEPHYRUM && (s32)d->nota != nota)
        {
            perge;
        }
        cella = (OratioOraculumErratumNotae**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = d;
    }
    xar_ordinare(exitus, _errata_notarum_comparare);
    redde exitus;
}

/* extentum lexematum listae loci */
interior vacuum
_extentum_listae (
    constans MateriaValor* v,
                      s32* a,
                      s32* b)
{
    i32 n;
    i32 k;

    si (v->genus == MATERIA_VALOR_TOKEN && v->datum.token != NIHIL)
    {
        constans MateriaToken* t = v->datum.token;

        si (t->byte_offset >= ZEPHYRUM)
        {
            si (*a < ZEPHYRUM || t->byte_offset < *a)
            {
                *a = t->byte_offset;
            }
            si (t->byte_offset + (s32)t->valor.mensura > *b)
            {
                *b = t->byte_offset + (s32)t->valor.mensura;
            }
        }
        redde;
    }
    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    n = materia_valor_lista_numerus(*v);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*v, k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            _extentum_listae(e, a, b);
        }
    }
}

interior b32
_elementum_addere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
            constans MateriaNodus* n,
                              Xar* elementa)
{
    Elementum* e = (Elementum*)xar_addere(elementa);
          i32  k;

    si (e == NIHIL)
    {
        redde FALSUM;
    }
                        memset(e, ZEPHYRUM, magnitudo(*e));
        e->a         = (s32)-I;
    e->b             = ZEPHYRUM;
    e->decisio       = (s32)-I;
        e->nodus     = n;
    e->casus_primus  = (s32)-I;
    {
        i32 k;

        per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
        {
            e->notae_primae[k] = (s32)-I;
        }
    }
    e->lexema        = (s32)-I;
    /* T20a: clausula elementi (locus per genus) */
    e->clausula        = (s32)-I;
    e->clausula_causa  = (s32)-I;
    {
        s32 locus = oratio_locus_clausulae((OratioGenus)n->genus,
            FALSUM);
        s32 locus_causae = oratio_locus_clausulae(
            (OratioGenus)n->genus, VERUM);

        si (   locus                >= ZEPHYRUM
            && n->loci[locus].genus == MATERIA_VALOR_INDEX)
        {
            e->clausula = n->loci[locus].datum.index;
        }
        si (   locus_causae                >= ZEPHYRUM
            && n->loci[locus_causae].genus == MATERIA_VALOR_INDEX)
        {
            e->clausula_causa = n->loci[locus_causae].datum.index;
        }
    }


    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        constans MateriaValor* classes =
            &n->loci[ORATIO_VOCABULUM_CLASSES];
        constans MateriaValor* analyses =
            &n->loci[ORATIO_VOCABULUM_ANALYSES];
        constans MateriaValor* decisio =
            &n->loci[ORATIO_VOCABULUM_DECISIO];

                /* T19g: decisio, auctor et numerus lectionum pro partitione */
        si (decisio->genus == MATERIA_VALOR_INDEX)
        {
            constans MateriaValor* auctor =
                &n->loci[ORATIO_VOCABULUM_AUCTOR];

            e->decisio = decisio->datum.index;
            si (   auctor->genus       == MATERIA_VALOR_TOKEN
                && auctor->datum.token != NIHIL)
            {
                e->auctor = auctor->datum.token->valor;
            }
        }

                si (analyses->genus == MATERIA_VALOR_LISTA)
                {
            e->numerus_analysium =
                materia_valor_lista_numerus(*analyses);
            /* T23: casus lectionis primae, Latinae solum */
            si (e->numerus_analysium > ZEPHYRUM)
            {
                constans MateriaNodus* prima =
                    materia_valor_lista_obtinere(
                    *analyses, ZEPHYRUM)->datum.nodus;
                OratioClassis cl = oratio_genus_classis(
                    (OratioGenus)prima->genus);
                s32 locus = (i32)cl
                    < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                    ? oratio_partes_locus(cl, "casus") : (s32)-I;

                si (   locus                    >= ZEPHYRUM
                    && prima->loci[ORATIO_ANALYSIS_LINGUA].genus
                        == MATERIA_VALOR_INDEX
                    && prima->loci[ORATIO_ANALYSIS_LINGUA].datum.index
                        == (s32)ORATIO_LINGUA_LATINA
                    && prima->loci[locus].genus == MATERIA_VALOR_INDEX)
                {
                    e->casus_primus = prima->loci[locus].datum.index;
                }
                /* T29: accidentia tabulae notarum, lectio prima Latina */
                si (   prima->loci[ORATIO_ANALYSIS_LINGUA].genus
                        == MATERIA_VALOR_INDEX
                    && prima->loci[ORATIO_ANALYSIS_LINGUA].datum.index
                        == (s32)ORATIO_LINGUA_LATINA
                    && (i32)cl < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
                {
                    i32 k;

                    per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
                    {
                        s32 locus_k = oratio_partes_locus(cl,
                            NOTAE_ORACULI[k].accidens);
                        i32 j;

                        si (locus_k < ZEPHYRUM)
                        {
                            perge;
                        }
                        si (prima->loci[locus_k].genus
                                == MATERIA_VALOR_INDEX)
                        {
                            e->notae_primae[k] =
                                prima->loci[locus_k].datum.index;
                        }
                        /* T30: valores attingibiles - lectiones Latinae
                         * eiusdem classis ac prima, accidens ferentes */
                        per (j = ZEPHYRUM; j
                            < e->numerus_analysium; j++)
                        {
                            constans MateriaNodus* aj =
                                materia_valor_lista_obtinere(*analyses,
                                j)->datum.nodus;
                            s32 v;

                            si (   oratio_genus_classis(
                                        (OratioGenus)aj->genus) != cl
                                || aj->loci[ORATIO_ANALYSIS_LINGUA].genus
                                    != MATERIA_VALOR_INDEX
                                || aj->loci[ORATIO_ANALYSIS_LINGUA]
                                    .datum.index
                                    != (s32)ORATIO_LINGUA_LATINA
                                || aj->loci[locus_k].genus
                                    != MATERIA_VALOR_INDEX)
                            {
                                perge;
                            }
                            v = aj->loci[locus_k].datum.index;
                            si (v >= ZEPHYRUM && v < (s32)31)
                            {
                                e->notae_praesentes[k] =
                                    e->notae_praesentes[k]
                                    | ((i32)I << (i32)v);
                            }
                        }
                    }
                }
            }
                }


        _extentum_listae(&n->loci[ORATIO_VOCABULUM_PARTES], &e->a,
            &e->b);
        si (   classes->genus       == MATERIA_VALOR_TOKEN
            && classes->datum.token != NIHIL)
        {
            chorda c = classes->datum.token->valor;
               i32 i = ZEPHYRUM;

            e->classes = c;
            dum (   i < c.mensura
                 && e->numerus_classium < (i32)CLASSES_MAXIMAE)
            {
                          i32 a = i;
                OratioClassis cl;

                dum (i < c.mensura && c.datum[i] != ' ')
                {
                    i = i + I;
                }
                cl =
                    oratio_classis_ex_titulo((constans character*)c.datum
                        + a,
                    i - a);
                si ((i32)cl < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
                {
                    e->classis[e->numerus_classium] = cl;
                    e->numerus_classium = e->numerus_classium + I;
                }
                i = i + I;
            }
        }
        e->ignotum = (b32)(e->numerus_classium == I
            && e->classis[ZEPHYRUM] == ORATIO_CLASSIS_IGNOTUM);
        si (analyses->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*analyses);

            per (k = ZEPHYRUM; k < m
                && e->numerus_lemmatum < (i32)LEMMATA_MAXIMA;
                 k++)
            {
                constans MateriaNodus* an =
                    materia_valor_lista_obtinere(
                    *analyses, k)->datum.nodus;
                constans MateriaValor* l =
                    &an->loci[ORATIO_ANALYSIS_LEMMA];

                si (   l->genus       == MATERIA_VALOR_TOKEN
                    && l->datum.token != NIHIL)
                {
                    chorda plicata =
                        oratio_vocabularium_la_plicare(piscina,
                        l->datum.token->valor);
                    i32 j;
                    b32 nova = VERUM;

                    per (j = ZEPHYRUM; j < e->numerus_lemmatum; j++)
                    {
                        si (_aequalis(e->lemmata[j], plicata))
                        {
                            nova = FALSUM;
                            frange;
                        }
                    }
                    si (nova)
                    {
                        e->lemmata[e->numerus_lemmatum] = plicata;
                        e->numerus_lemmatum = e->numerus_lemmatum + I;
                    }
                }
            }
        }
        (vacuum)voc;
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        /* signum = interpunctio aut symbolum (UD PUNCT / SYM: '-'
         * utrumque, '$' '/' ':-)' SYM): candidata ambo, interpunctio
         * prima (T16) */
        hic_manens character signa[] = "interpunctio symbolum";

        _extentum_listae(&n->loci[ORATIO_INTERPUNCTIO_SIGNUM], &e->a,
            &e->b);

                e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_INTERPUNCTIO;
        e->classis[I]                 = ORATIO_CLASSIS_SYMBOLUM;
        e->numerus_classium           = II;
        e->numerus_analysium          = II;   /* candidata duo, nemo decidit */

        e->classes = _copia(piscina, _chorda((i8*)signa,
            (i32)(magnitudo(signa) - I)));
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        _extentum_listae(&n->loci[ORATIO_NUMERUS_CRUDUM], &e->a, &e->b);
                e->classis[ZEPHYRUM]  = ORATIO_CLASSIS_NUMERALE;
        e->numerus_classium           = I;
        e->numerus_analysium          = I;

        redde VERUM;
    }
    xar_removere_ultimum(elementa);
    redde VERUM;
}

interior b32
_elementa_colligere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
            constans MateriaNodus* n,
                              Xar* elementa)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (   n->genus == (s32)ORATIO_GENUS_VOCABULUM
        || n->genus == (s32)ORATIO_GENUS_INTERPUNCTIO
        || n->genus == (s32)ORATIO_GENUS_NUMERUS)
    {
        redde _elementum_addere(piscina, voc, n, elementa);
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_elementa_colligere(piscina, voc, v->datum.nodus,
                elementa))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_elementa_colligere(piscina, voc,
                    e->datum.nodus,
                        elementa))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Clausulae aureae (T20a)
 * ================================================== */

/* numerus decimalis chordae; -I si non numerus */
interior s32
_numerus_chordae (
    chorda c)
{
    s32 v = ZEPHYRUM;
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde (s32)-I;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde (s32)-I;
        }
        v = v * (s32)X + (s32)(c.datum[i] - '0');
    }
    redde v;
}

interior b32
_continet (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);
    i32 i;

    per (i = ZEPHYRUM; i + l <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (size_t)l) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* relatio UD auxiliaris aut copulae (aux | cop, praefixum ante ':') */
interior b32
_auxiliaris_aut_copula (
    chorda relatio)
{
    redde (b32)(   (relatio.mensura >= (i32)III
                    && memcmp(relatio.datum, "aux", (size_t)III)
                        == ZEPHYRUM
                    && (   relatio.mensura == (i32)III
                        || relatio.datum[III] == ':'))
                || (relatio.mensura >= (i32)III
                    && memcmp(relatio.datum, "cop", (size_t)III)
                        == ZEPHYRUM
                    && (   relatio.mensura == (i32)III
                        || relatio.datum[III] == ':')));
}

/* clausulae aureae sententiae totius: per positionem lexematis
 * (rangae -I) positio radicis; finitae per positionem radicis.
 * Tabulae in piscina. FALSUM = memoria. */
interior b32
_clausulas_aureas (
                           Piscina*  piscina,
    constans OratioConlluSententia*  s,
                               s32** clausulae,
                               b32** finitae)
{
    i32  n           = xar_numerus(s->lexemata);
    s32  id_maximus  = ZEPHYRUM;
    s32* positio;     /* id -> positio; -I */
    s32* caput;       /* positio -> positio capitis; -I radix */
    b32* radix;
    b32* finita;
    s32* cl;
    i32  k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

        si (!t->ranga && (s32)t->a > id_maximus)
        {
            id_maximus = (s32)t->a;
        }
    }
    positio = (s32*)piscina_allocare(piscina,
        (memoriae_index)(id_maximus
            + I) * (memoriae_index)magnitudo(s32));
    caput   = (s32*)piscina_allocare(piscina, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    radix   = (b32*)piscina_allocare(piscina, (memoriae_index)n
        * (memoriae_index)magnitudo(b32));
    finita  = (b32*)piscina_allocare(piscina, (memoriae_index)n
        * (memoriae_index)magnitudo(b32));
    cl      = (s32*)piscina_allocare(piscina, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    si (   positio == NIHIL || caput == NIHIL || radix == NIHIL
        || finita  == NIHIL || cl == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k <= (i32)id_maximus; k++)
    {
        positio[k] = (s32)-I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

        caput[k]   = (s32)-I;
        radix[k]   = FALSUM;
        finita[k]  = FALSUM;
        cl[k]      = (s32)-I;
        si (!t->ranga)
        {
            positio[t->a] = (s32)k;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
        s32 h;

        si (t->ranga)
        {
            perge;
        }
        h = _numerus_chordae(t->head);
        si (h > ZEPHYRUM && h <= id_maximus && positio[h] >= ZEPHYRUM)
        {
            caput[k] = positio[h];
        }
        si (_literis(t->deprel, "root") || h == ZEPHYRUM)
        {
            radix[k] = VERUM;
        }
                si (   _continet(t->feats, "VerbForm=Fin")
                    && !_auxiliaris_aut_copula(t->deprel))
                {
            radix[k]   = VERUM;
            finita[k]  = VERUM;
                }
    }
    /* praedicatum cum aux/cop finito: radix finita */
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

                si (   !t->ranga && caput[k] >= ZEPHYRUM
                    && _auxiliaris_aut_copula(t->deprel)
                    && _continet(t->feats, "VerbForm=Fin"))
                {
            radix[caput[k]]   = VERUM;
            finita[caput[k]]  = VERUM;
                }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
        s32 p       = (s32)k;
        i32 gradus  = ZEPHYRUM;

        si (t->ranga)
        {
            perge;
        }
        dum (   p >= ZEPHYRUM && !radix[p] && caput[p] >= ZEPHYRUM
             && gradus < n)
        {
            p       = caput[p];
            gradus  = gradus + I;
        }
        cl[k] = p;
    }
    *clausulae  = cl;
    *finitae    = finita;
    redde VERUM;
}

s32
oratio_oraculum_clausula_aurea (
                           Piscina* piscina,
    constans OratioConlluSententia* sententia,
                               i32  k,
                               b32* finita)
{
    s32* cl;
    b32* fin;

    si (   k >= xar_numerus(sententia->lexemata)
        || !_clausulas_aureas(piscina, sententia, &cl, &fin))
    {
        redde (s32)-I;
    }
    si (finita != NIHIL)
    {
        *finita = cl[k] >= ZEPHYRUM ? fin[cl[k]] : FALSUM;
    }
    redde cl[k];
}

/* erratum clausulae notare: clavis binaria [causa][species] forma
 * 0x01 radix */
interior vacuum
_erratum_clausulae_notare (
                 Piscina* piscina,
    OratioOraculumCensus* census,
                     s32  causa,
                  chorda  forma,
                     s32  species,
                  chorda  radix)
{
                         character*  clavis_datum;
                            chorda   plicata;
                            chorda   radix_plicata;
                            chorda   clavis;
                            vacuum*  valor;
    OratioOraculumErratumClausulae*  d;
    OratioOraculumErratumClausulae** cella;
                               i32   n;

    si (census->errata_clausularum == NIHIL)
    {
        census->errata_clausularum = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumErratumClausulae*));
        census->errata_clausularum_index =
            tabula_dispersa_creare_chorda(piscina, (i32)1024);
        si (   census->errata_clausularum       == NIHIL
            || census->errata_clausularum_index == NIHIL)
        {
            census->errata_clausularum = NIHIL;
            redde;
        }
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
    radix_plicata = oratio_vocabularium_la_plicare(piscina, radix);
    n = (i32)II + plicata.mensura + I + radix_plicata.mensura;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)n);
    si (   plicata.datum == NIHIL || radix_plicata.datum == NIHIL
        || clavis_datum  == NIHIL)
    {
        redde;
    }
    clavis_datum[ZEPHYRUM]  = (character)(causa + I);
    clavis_datum[I]         = (character)(species + I);
    n                       = (i32)II;
    memcpy(clavis_datum + n, plicata.datum, (size_t)plicata.mensura);
    n                = n + plicata.mensura;
    clavis_datum[n]  = (character)I;
    n                = n + I;
    memcpy(clavis_datum + n, radix_plicata.datum,
        (size_t)radix_plicata.mensura);
    n               = n + radix_plicata.mensura;
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = n;
    si (tabula_dispersa_invenire(census->errata_clausularum_index,
            clavis, &valor))
    {
        d           = (OratioOraculumErratumClausulae*)valor;
        d->numerus  = d->numerus + I;
        redde;
    }
    d = (OratioOraculumErratumClausulae*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*d));
    cella = (OratioOraculumErratumClausulae**)xar_addere(
        census->errata_clausularum);
    si (d == NIHIL || cella == NIHIL)
    {
        redde;
    }
    d->causa    = causa;
    d->forma    = plicata;
    d->species  = species;
    d->radix    = radix_plicata;
    d->numerus  = I;
    *cella      = d;
    (vacuum)tabula_dispersa_inserere(census->errata_clausularum_index,
        clavis, d);
}

interior s32
_errata_clausularum_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumErratumClausulae* x =
        *(constans OratioOraculumErratumClausulae* constans*)p;
    constans OratioOraculumErratumClausulae* y =
        *(constans OratioOraculumErratumClausulae* constans*)q;
    s32 c;

    si (x->numerus != y->numerus)
    {
        redde x->numerus > y->numerus ? (s32)-I : (s32)I;
    }
    si (x->causa != y->causa)
    {
        redde x->causa < y->causa ? (s32)-I : (s32)I;
    }
    c = chorda_comparare(x->forma, y->forma);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde chorda_comparare(x->radix, y->radix);
}

Xar*
oratio_oraculum_errata_clausularum (
                          Piscina* piscina,
    constans OratioOraculumCensus* census)
{
    Xar* exitus = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumErratumClausulae*));
    i32 i;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (census->errata_clausularum == NIHIL)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(census->errata_clausularum);
         i++)
    {
        OratioOraculumErratumClausulae** cella =
            (OratioOraculumErratumClausulae**)xar_addere(exitus);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = *(OratioOraculumErratumClausulae**)xar_obtinere(
            census->errata_clausularum, i);
    }
    xar_ordinare(exitus, _errata_clausularum_comparare);
    redde exitus;
}

/* semen notare (census aureus): clavis upos 0x01 textus */
interior OratioOraculumSemen*
_semen_capere (
           Piscina* piscina,
               Xar* semina,
    TabulaDispersa* index,
            chorda  upos,
            chorda  textus)
{
              character*  clavis_datum;
                 chorda   clavis;
                 chorda   plicata;
                 vacuum*  valor;
    OratioOraculumSemen*  s;
    OratioOraculumSemen** cella;
                    i32   n;

    plicata  = oratio_vocabularium_la_plicare(piscina, textus);
    n        = upos.mensura + I + plicata.mensura;
    clavis_datum = (character*)piscina_allocare(piscina,
        (memoriae_index)n);
    si (plicata.datum == NIHIL || clavis_datum == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(clavis_datum, upos.datum, (size_t)upos.mensura);
    clavis_datum[upos.mensura] = (character)I;
    memcpy(clavis_datum + upos.mensura + I, plicata.datum,
        (size_t)plicata.mensura);
    clavis.datum    = (i8*)clavis_datum;
    clavis.mensura  = n;
    si (tabula_dispersa_invenire(index, clavis, &valor))
    {
        redde (OratioOraculumSemen*)valor;
    }
    s = (OratioOraculumSemen*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*s));
    cella = (OratioOraculumSemen**)xar_addere(semina);
    si (s == NIHIL || cella == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->textus  = plicata;
    s->upos    = _copia(piscina, upos);
    *cella     = s;
    (vacuum)tabula_dispersa_inserere(index, clavis, s);
    redde s;
}

interior s32
_semina_comparare (
    constans vacuum* p,
    constans vacuum* q)
{
    constans OratioOraculumSemen* x =
        *(constans OratioOraculumSemen* constans*)p;
    constans OratioOraculumSemen* y =
        *(constans OratioOraculumSemen* constans*)q;
    s32 c;

    si (x->n != y->n)
    {
        redde x->n > y->n ? (s32)-I : (s32)I;
    }
    c = chorda_comparare(x->textus, y->textus);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde chorda_comparare(x->upos, y->upos);
}

interior b32
_semen_candidatum (
    chorda upos)
{
    redde (b32)(   _literis(upos, "SCONJ") || _literis(upos, "CCONJ")
                || _literis(upos, "PRON") || _literis(upos, "DET")
                || _literis(upos, "ADV") || _literis(upos, "PUNCT"));
}

Xar*
oratio_oraculum_semina (
    Piscina* piscina,
        Xar* sententiae)
{
               Xar* semina = xar_creare(piscina,
                   (i32)magnitudo(OratioOraculumSemen*));
    TabulaDispersa* index  = tabula_dispersa_creare_chorda(piscina,
        (i32)4096);
                i32 i;

    si (semina == NIHIL || index == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        constans OratioConlluSententia* s =
            (constans OratioConlluSententia*)xar_obtinere(sententiae,
            i);
        Piscina* scratch = piscina_generare_dynamicum(
            "oraculum_semina", 4194304);
            s32* cl;
            b32* fin;
            i32  n;
            i32  k;

        si (scratch == NIHIL)
        {
            redde NIHIL;
        }
        si (!_clausulas_aureas(scratch, s, &cl, &fin))
        {
            piscina_destruere(scratch);
            redde NIHIL;
        }
        n = xar_numerus(s->lexemata);
        per (k = ZEPHYRUM; k < n; k++)
        {
            constans OratioConlluLexema* t =
                (constans OratioConlluLexema*)xar_obtinere(s->lexemata,
                k);
                        s32 prior  = (s32)-I;   /* non PUNCT non CCONJ */
            s32 praecedens         = (s32)-I;   /* non PUNCT */
            s32 proximum           = (s32)-I;
            s32 j;
            b32 limes;
            OratioOraculumSemen* sm;

            si (t->ranga || !_semen_candidatum(t->upos))
            {
                perge;
            }
            per (j = (s32)k - (s32)I; j >= ZEPHYRUM; j--)
            {
                constans OratioConlluLexema* u =
                    (constans OratioConlluLexema*)xar_obtinere(
                    s->lexemata, (i32)j);

                si (u->ranga || _literis(u->upos, "PUNCT"))
                {
                    perge;
                }
                                si (praecedens < ZEPHYRUM)
                                {
                    praecedens = j;
                                }
                si (!_literis(u->upos, "CCONJ"))
                {
                    prior = j;
                    frange;
                }
            }
            per (j = (s32)k + (s32)I; j < (s32)n; j++)
            {
                constans OratioConlluLexema* u =
                    (constans OratioConlluLexema*)xar_obtinere(
                    s->lexemata, (i32)j);

                si (!u->ranga && !_literis(u->upos, "PUNCT"))
                {
                    proximum = j;
                    frange;
                }
            }
            si (_literis(t->upos, "PUNCT"))
            {
                                limes = (b32)(   praecedens >= ZEPHYRUM
                                    && proximum >= ZEPHYRUM
                                    && cl[praecedens] != cl[proximum]);
                sm = _semen_capere(piscina, semina, index, t->upos,
                    t->forma);
            }
            alioquin
            {
                limes = (b32)(prior < ZEPHYRUM || cl[prior] != cl[k]);
                sm = _semen_capere(piscina, semina, index, t->upos,
                    t->lemma);
            }
            si (sm == NIHIL)
            {
                piscina_destruere(scratch);
                redde NIHIL;
            }
            sm->n = sm->n + I;
            si (limes)
            {
                sm->limes = sm->limes + I;
            }
            si (prior < ZEPHYRUM && !_literis(t->upos, "PUNCT"))
            {
                sm->initia = sm->initia + I;
            }
            si (cl[k] >= ZEPHYRUM && fin[cl[k]])
            {
                sm->finita = sm->finita + I;
            }
            si (_literis(t->upos, "CCONJ"))
            {
                b32 sinistra  = FALSUM;
                b32 dextra    = FALSUM;

                per (j = (s32)k - (s32)I; j >= ZEPHYRUM; j--)
                {
                    constans OratioConlluLexema* u =
                        (constans OratioConlluLexema*)xar_obtinere(
                        s->lexemata, (i32)j);

                    si (   _literis(u->upos, "SCONJ")
                        || _literis(u->upos, "CCONJ")
                        || _literis(u->forma, ",")
                        || _literis(u->forma, ";"))
                    {
                        frange;
                    }
                    si (   !u->ranga && fin[j] && cl[j] == j)
                    {
                        sinistra = VERUM;
                    }
                }
                per (j = (s32)k + (s32)I; j < (s32)n; j++)
                {
                    constans OratioConlluLexema* u =
                        (constans OratioConlluLexema*)xar_obtinere(
                        s->lexemata, (i32)j);

                    si (   _literis(u->upos, "SCONJ")
                        || _literis(u->upos, "CCONJ")
                        || _literis(u->forma, ",")
                        || _literis(u->forma, ";"))
                    {
                        frange;
                    }
                    si (   !u->ranga && fin[j] && cl[j] == j)
                    {
                        dextra = VERUM;
                    }
                }
                si (sinistra && dextra)
                {
                    sm->ff = sm->ff + I;
                    si (limes)
                    {
                        sm->ff_limes = sm->ff_limes + I;
                    }
                }
            }
        }
        piscina_destruere(scratch);
    }
    xar_ordinare(semina, _semina_comparare);
    redde semina;
}


/* ==================================================
 * Iudicium
 * ================================================== */

/* PAR clausulae (T20a): verbum aureum alignatum (non PUNCT) - clausula
 * nostra elementi primi (-I aperta), causa, clausula aurea (positio
 * radicis), forma aurea; post sententiam totam iudicatur (maior
 * clausulae nostrae cuiusque) */
nomen structura {
       s32 nostra;
       s32 causa;
       s32 aurea;
    chorda forma;
} ParClausulae;


interior vacuum
_exemplum (
    OratioOraculumClassis* c,
                  Piscina* piscina,
                   chorda  forma,
                   chorda  classes,
                   chorda  lemma)
{
    OratioOraculumExemplum* e;

    si (c->numerus_exemplorum >= (i32)ORATIO_ORACULUM_EXEMPLA)
    {
        redde;
    }
    e         = &c->exempla[c->numerus_exemplorum];
    e->forma  = _copia(piscina, forma);
    e->classes  = _copia(piscina, classes.mensura > ZEPHYRUM ? classes
        : _chorda(NIHIL, ZEPHYRUM));
    e->lemma               = _copia(piscina, lemma);
    c->numerus_exemplorum  = c->numerus_exemplorum + I;
}

/* formam in textu a cursore quaerere (spatia praeterita); -I si absens */
interior s32
_formam_invenire (
    chorda textus,
       i32 cursor,
    chorda forma)
{
    i32 i = cursor;

    si (forma.mensura == ZEPHYRUM)
    {
        redde (s32)-I;
    }
    dum (i + forma.mensura <= textus.mensura)
    {
        si (memcmp(textus.datum + i, forma.datum, (size_t)forma.mensura)
            == ZEPHYRUM)
        {
            redde (s32)i;
        }
        i = i + I;
    }
    redde (s32)-I;
}

/* T29: tempus aureum ex Tense + Aspect (UD Latinum: Past+Perf =
 * perfectum, Past+Imp = imperfectum, Fut+Perf = futurum exactum) */
interior s32
_tempus_aureum (
    chorda notae)
{
    si (_continet(notae, "Tense=Pqp"))
    {
        redde (s32)ORATIO_TEMPUS_PLUSQUAMPERFECTUM;
    }
    si (_continet(notae, "Tense=Pres"))
    {
        redde (s32)ORATIO_TEMPUS_PRAESENS;
    }
    si (_continet(notae, "Tense=Past"))
    {
        redde _continet(notae, "Aspect=Perf")
            ? (s32)ORATIO_TEMPUS_PERFECTUM
            : (s32)ORATIO_TEMPUS_IMPERFECTUM;
    }
    si (_continet(notae, "Tense=Fut"))
    {
        redde _continet(notae, "Aspect=Perf")
            ? (s32)ORATIO_TEMPUS_FUTURUM_EXACTUM
            : (s32)ORATIO_TEMPUS_FUTURUM;
    }
    redde (s32)-I;
}

/* T29: valor aureus accidentis k ex notis UD ('Clavis=Valor'); alter =
 * index alter acceptus aut -I; -I = absens aut extra tabulam */
interior s32
_nota_aurea (
    chorda  notae,
       i32  k,
       s32* alter)
{
    constans NotaOraculi* nota = &NOTAE_ORACULI[k];
               character  quaesitum[32];
                     i32  i;

    *alter = (s32)-I;
    si (nota->valores == NIHIL)
    {
        redde _tempus_aureum(notae);
    }
    per (i = ZEPHYRUM; i < nota->numerus; i++)
    {
        sprintf(quaesitum, "%s=%s", nota->clavis,
            nota->valores[i].valor_aureus);
        si (_continet(notae, quaesitum))
        {
            *alter = nota->valores[i].alter;
            redde nota->valores[i].index;
        }
    }
    redde (s32)-I;
}

/* T30: valores aurei accidentis k ut MASCULA bitium indicum nostrorum -
 * campus 'Clavis=Valor,Valor' (initium aut post '|'), valor quisque
 * per commata bitium suum, alter acceptus quoque (UD Latinum genus
 * plures scribit: 'Gender=Fem,Masc' - substringa 'Gender=Masc' eum
 * non invenit, T29 femininum solum accipiebat); ZEPHYRUM = absens aut
 * extra tabulam */
interior i32
_nota_aurea_valores (
    chorda notae,
       i32 k,
       b32 alteri)
{
    constans NotaOraculi* nota     = &NOTAE_ORACULI[k];
                     i32  mascula  = ZEPHYRUM;
                     i32  l        = (i32)strlen(nota->clavis);
                     i32  i;

    si (nota->valores == NIHIL)
    {
        s32 t = _tempus_aureum(notae);

        redde t >= ZEPHYRUM ? (i32)I << (i32)t : ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i + l + I <= notae.mensura; i++)
    {
        i32 initium;
        i32 j;

        si (   (i != ZEPHYRUM && notae.datum[i - I] != '|')
            || memcmp(notae.datum + i, nota->clavis, (size_t)l)
                != ZEPHYRUM
            || notae.datum[i + l] != '=')
        {
            perge;
        }
        initium  = i + l + I;
        j        = initium;
        dum (j <= notae.mensura)
        {
            si (   j == notae.mensura || notae.datum[j] == '|'
                || notae.datum[j] == ',')
            {
                i32 v;

                per (v = ZEPHYRUM; v < nota->numerus; v++)
                {
                    i32 lv = (i32)strlen(nota->valores[v].valor_aureus);

                    si (   j - initium == lv
                        && memcmp(notae.datum + initium,
                            nota->valores[v].valor_aureus, (size_t)lv)
                            == ZEPHYRUM)
                    {
                        mascula = mascula
                            | ((i32)I << (i32)nota->valores[v].index);
                        si (   alteri
                            && nota->valores[v].alter >= ZEPHYRUM)
                        {
                            mascula = mascula
                                | ((i32)I
                                    << (i32)nota->valores[v].alter);
                        }
                    }
                }
                si (j == notae.mensura || notae.datum[j] == '|')
                {
                    frange;
                }
                initium = j + I;
            }
            j = j + I;
        }
        frange;
    }
    redde mascula;
}

/* casus aureus ex 'Case=Nom|...' (UD): OratioCasus aut -I */
interior s32
_casus_aureus (
    chorda notae)
{
    si (_continet(notae, "Case=Nom"))
    {
        redde (s32)ORATIO_CASUS_NOMINATIVUS;
    }
    si (_continet(notae, "Case=Gen"))
    {
        redde (s32)ORATIO_CASUS_GENITIVUS;
    }
    si (_continet(notae, "Case=Dat"))
    {
        redde (s32)ORATIO_CASUS_DATIVUS;
    }
    si (_continet(notae, "Case=Acc"))
    {
        redde (s32)ORATIO_CASUS_ACCUSATIVUS;
    }
    si (_continet(notae, "Case=Abl"))
    {
        redde (s32)ORATIO_CASUS_ABLATIVUS;
    }
    si (_continet(notae, "Case=Loc"))
    {
        redde (s32)ORATIO_CASUS_LOCATIVUS;
    }
    si (_continet(notae, "Case=Voc"))
    {
        redde (s32)ORATIO_CASUS_VOCATIVUS;
    }
    redde (s32)-I;
}

/* elementum unum inter [e0, e1) cuius textus formam octetim aequat;
 * -I si nullum aut plura (T21) */
interior s32
_elementum_formae (
    chorda  textus,
       Xar* elementa,
       i32  e0,
       i32  e1,
    chorda  forma)
{
    s32 inventum = (s32)-I;
    i32 k;

    per (k = e0; k < e1; k++)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa, k);

        si (   e->a                >= ZEPHYRUM && e->b > e->a
            && (i32)(e->b - e->a)  == forma.mensura
            && (s32)textus.mensura >= e->b
            && memcmp(textus.datum + e->a, forma.datum,
                (size_t)forma.mensura) == ZEPHYRUM)
        {
                        si (inventum >= ZEPHYRUM)
                        {
                redde (s32)-I;   /* plura: ambiguum */
                        }
            inventum = (s32)k;
        }
    }
    redde inventum;
}

/* LIGATIONES (T26): umbrae impletae lectionis primae cuiusque elementi
 * alignati contra capita aurea; arcus aurei = lexemata alignata non
 * interpuncta capite alignato (revocatio). FALSUM = memoria */
interior b32
_ligationes_iudicare (
                           Piscina* piscina,
                           Piscina* scratch,
              OratioOraculumCensus* census,
    constans OratioConlluSententia* s,
                               Xar* elementa)
{
    i32  n           = xar_numerus(s->lexemata);
    i32  ne          = xar_numerus(elementa);
    s32  id_maximus  = ZEPHYRUM;
    s32* positio;      /* id aureum -> index lexematis */
    s32* caput;        /* index lexematis -> index capitis; -I radix */
    s32* elementum;    /* index lexematis -> index elementi; -I */
    i32* petitiones;   /* T32 a: petitiones dependentis per elementum */
    i32* petitiones_rectae;
    i32  k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

        si (!t->ranga && (s32)t->a > id_maximus)
        {
            id_maximus = (s32)t->a;
        }
    }
    positio   = (s32*)piscina_allocare(scratch,
        (memoriae_index)(id_maximus
        + I) * (memoriae_index)magnitudo(s32));
    caput     = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    elementum = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    petitiones = (i32*)piscina_allocare(scratch,
        (memoriae_index)(ne + I) * (memoriae_index)magnitudo(i32));
    petitiones_rectae = (i32*)piscina_allocare(scratch,
        (memoriae_index)(ne + I) * (memoriae_index)magnitudo(i32));
    si (   positio    == NIHIL || caput == NIHIL || elementum == NIHIL
        || petitiones == NIHIL || petitiones_rectae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        petitiones[k]         = ZEPHYRUM;
        petitiones_rectae[k]  = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k <= (i32)id_maximus; k++)
    {
        positio[k] = (s32)-I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

        caput[k]      = (s32)-I;
        elementum[k]  = (s32)-I;
        si (!t->ranga)
        {
            positio[t->a] = (s32)k;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
        s32 h = t->ranga ? ZEPHYRUM : _numerus_chordae(t->head);

        si (h > ZEPHYRUM && h <= id_maximus)
        {
            caput[k] = positio[h];
        }
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa,
            k);

        si (e->lexema >= ZEPHYRUM && e->lexema < (s32)n)
        {
            elementum[e->lexema] = (s32)k;
        }
    }
    /* arcus aurei: lexema alignatum non interpunctum, caput alignatum */
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);

        si (   !t->ranga && !_literis(t->upos, "PUNCT")
            && elementum[k]        >= ZEPHYRUM && caput[k] >= ZEPHYRUM
            && elementum[caput[k]] >= ZEPHYRUM)
        {
            census->arcus_aurei = census->arcus_aurei + I;
            /* T31 a: arcus subiecti aurei (nsubj, nsubj:pass) */
            si (   t->deprel.mensura >= (i32)V
                && memcmp(t->deprel.datum, "nsubj", (size_t)V)
                    == ZEPHYRUM)
            {
                census->arcus_aurei_subiecti =
                    census->arcus_aurei_subiecti + I;
            }
            si (_literis(t->deprel, "obj"))
            {
                census->arcus_aurei_obiecti =
                    census->arcus_aurei_obiecti + I;   /* T31 b */
            }
        }
    }
    /* ligationes nostrae */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa,
            k);
        constans MateriaValor* analyses;
        constans MateriaNodus* prima;
        constans MateriaValor* umbrae;
                          i32  u;

        si (   e->nodus        == NIHIL || e->lexema < ZEPHYRUM
            || e->nodus->genus != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        analyses = &e->nodus->loci[ORATIO_VOCABULUM_ANALYSES];
        si (   analyses->genus != MATERIA_VALOR_LISTA
            || materia_valor_lista_numerus(*analyses) == ZEPHYRUM)
        {
            perge;
        }
        prima = materia_valor_lista_obtinere(*analyses, ZEPHYRUM)
            ->datum.nodus;
        {
            OratioClassis cl =
                oratio_genus_classis((OratioGenus)prima->genus);
            s32 locus = (i32)cl < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                ? oratio_partes_locus(cl, "umbrae") : (s32)-I;

            si (   locus < ZEPHYRUM
                || prima->loci[locus].genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            umbrae = &prima->loci[locus];
        }
        per (u = ZEPHYRUM; u
            < materia_valor_lista_numerus(*umbrae); u++)
        {
            constans MateriaNodus* umbra = materia_valor_lista_obtinere(
                *umbrae, u)->datum.nodus;
            constans MateriaValor* w =
                &umbra->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM];
            constans Elementum* socius;
                           b32  capitis;
                           b32  recta;
                           b32  pendet;
                           i32  distantia;

            si (   w->genus != MATERIA_VALOR_INDEX || w->datum.index
                < ZEPHYRUM || w->datum.index >= (s32)ne)
            {
                perge;
            }
            socius = (constans Elementum*)xar_obtinere(elementa,
                (i32)w->datum.index);
            si (socius->lexema < ZEPHYRUM)
            {
                perge;
            }
                        capitis =
                            (b32)(   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                            == MATERIA_VALOR_INDEX
                            && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                            == (s32)ORATIO_RELATIO_CAPUT);
            /* directio UD: caput adiectivi = nomen (amod); adpositio,
             * particula (to) et auxiliare (have) a regimine suo pendent
             * (case, mark, aux) - socius caput carrier; obiectum verbi
             * a verbo pendet (obj) - carrier caput socii */
            {
                OratioClassis cc = oratio_genus_classis(
                    (OratioGenus)prima->genus);
                pendet = (b32)(capitis
                    || cc == ORATIO_CLASSIS_ADPOSITIO
                    || cc == ORATIO_CLASSIS_PARTICULA
                    || cc == ORATIO_CLASSIS_AUXILIARE);

                recta = pendet
                    ? (b32)(caput[e->lexema] == socius->lexema)
                    : (b32)(caput[socius->lexema] == e->lexema);
            }
            distantia = (i32)w->datum.index > k ? (i32)w->datum.index
                - k
                : k - (i32)w->datum.index;
            census->ligationes_nostrae = census->ligationes_nostrae + I;
            si (recta)
            {
                census->ligationes_rectae = census->ligationes_rectae
                    + I;
            }
            /* T32 a: per AUCTOREM impletionis; dependens petitus (carrier
             * si pendet, socius aliter) numeratur - contentio capitis */
            {
                constans MateriaValor* auctor =
                    &umbra->loci[ORATIO_UMBRA_AUCTOR];
                i32 dependens = pendet ? k : (i32)w->datum.index;

                si (   auctor->genus       == MATERIA_VALOR_TOKEN
                    && auctor->datum.token != NIHIL)
                {
                    _auctorem_ligationem_notare(piscina, census,
                        auctor->datum.token->valor, recta,
                        (b32)(distantia == I));
                }
                petitiones[dependens] = petitiones[dependens] + I;
                si (recta)
                {
                    petitiones_rectae[dependens] =
                        petitiones_rectae[dependens] + I;
                }
            }
            /* T31 a: per relationem */
            si (   umbra->loci[ORATIO_UMBRA_RELATIO].genus
                    == MATERIA_VALOR_INDEX
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    >= ZEPHYRUM
                && umbra->loci[ORATIO_UMBRA_RELATIO].datum.index
                    < (s32)ORATIO_RELATIO_NUMERUS)
            {
                i32 r =
                    (i32)umbra->loci[ORATIO_UMBRA_RELATIO].datum.index;

                census->ligationes_nostrae_relationum[r] =
                    census->ligationes_nostrae_relationum[r] + I;
                si (recta)
                {
                    census->ligationes_rectae_relationum[r] =
                        census->ligationes_rectae_relationum[r] + I;
                }
            }
            si (capitis)
            {
                census->ligationes_capitis = census->ligationes_capitis
                    + I;
                si (recta)
                {
                    census->ligationes_capitis_rectae =
                        census->ligationes_capitis_rectae + I;
                }
            }
            si (distantia == I)
            {
                census->ligationes_vicinae = census->ligationes_vicinae
                    + I;
                si (recta)
                {
                    census->ligationes_vicinae_rectae =
                        census->ligationes_vicinae_rectae + I;
                }
            }
        }
    }
    /* T32 a: dependentes contesti = elementa petita bis aut pluries */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        si (petitiones[k] >= (i32)II)
        {
            census->dependentes_contesti = census->dependentes_contesti
                + I;
            census->petitiones_contestae = census->petitiones_contestae
                + petitiones[k];
            census->petitiones_contestae_rectae =
                census->petitiones_contestae_rectae
                    + petitiones_rectae[k];
        }
    }
    redde VERUM;
}

/* verbum aureum unum contra classes elementorum [e0, e1) iudicare */
interior vacuum
_verbum_iudicare (
                        Piscina* piscina,
           OratioOraculumCensus* census,
                         chorda  textus,
                            Xar* elementa,
                            i32  e0,
                            i32  e1,
    constans OratioConlluLexema* verbum,
                            b32  primum,
                            Xar* paria,
                            s32  clausula_aurea)
{
                                  OratioClassis  aurea;
                          OratioOraculumClassis* c;
                                            b32  tectum    =
                                                FALSUM;
                                            b32 primaria  =
                                                FALSUM;
                                            b32 lemma     =
                                                FALSUM;
                                            b32 ignotum   = VERUM;
                                         chorda classes;
                                         chorda lemma_plicatum;
                                            i32 k;
                                            s32 casus_iudicatus;

    aurea            = oratio_oraculum_classis_ex_upos(verbum->upos);
    c                = &census->classes[aurea];
    census->verba    = census->verba + I;
    c->verba         = c->verba + I;
    classes.datum    = NIHIL;
    classes.mensura  = ZEPHYRUM;
    lemma_plicatum = oratio_vocabularium_la_plicare(piscina,
        verbum->lemma);
    per (k = e0; k < e1; k++)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa, k);
                       i32 j;

        si (k == e0)
        {
            classes = e->classes;
        }
        si (!e->ignotum)
        {
            ignotum = FALSUM;
        }
        per (j = ZEPHYRUM; j < e->numerus_classium; j++)
        {
            si (e->classis[j] == aurea)
            {
                tectum = VERUM;
                si (j == ZEPHYRUM && k == e0 && primum)
                {
                    primaria = VERUM;
                }
            }
        }
        per (j = ZEPHYRUM; j < e->numerus_lemmatum; j++)
        {
            si (_aequalis(e->lemmata[j], lemma_plicatum))
            {
                lemma = VERUM;
            }
        }
    }
    si (e1 <= e0)
    {
        ignotum = FALSUM;
    }
    si (tectum)
    {
        census->tecta  = census->tecta + I;
        c->tecta       = c->tecta + I;
    }
    alioquin
    {
        _exemplum(c, piscina, verbum->forma, classes, verbum->lemma);
    }
                si (primaria)
                {
        census->primaria  = census->primaria + I;
        c->primaria       = c->primaria + I;
                }
    /* T23 CASUS: classis recta, aurum casum fert, lectio prima Latina
     * casum fert -> populus; rectus si aequales */
    casus_iudicatus = (s32)-I;
    si (primaria && e1 > e0)
    {
        constans Elementum* e_primum =
            (constans Elementum*)xar_obtinere(elementa, e0);
        s32 casus_aureus = _casus_aureus(verbum->feats);

        si (   casus_aureus           >= ZEPHYRUM
            && e_primum->casus_primus >= ZEPHYRUM)
        {
            casus_iudicatus = (s32)(e_primum->casus_primus
                == casus_aureus);
            census->casus_verba  = census->casus_verba + I;
            c->casus_verba       = c->casus_verba + I;
            si (casus_iudicatus > ZEPHYRUM)
            {
                census->casus_recti  = census->casus_recti + I;
                c->casus_recti       = c->casus_recti + I;
            }
        }
        /* T29 NOTAE: accidens quodque tabulae, eadem condicio */
        {
            i32 k;

            per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
            {
                /* T30: valores aurei plures quivis acceptus; falsum
                 * tabulatum cum socio et attingibilitate */
                i32 aurei = _nota_aurea_valores(verbum->feats, k,
                    VERUM);
                s32 nostra = e_primum->notae_primae[k];

                si (aurei != ZEPHYRUM && nostra >= ZEPHYRUM)
                {
                    census->notae_verba[k] = census->notae_verba[k] + I;
                    si (((aurei >> (i32)nostra) & (i32)I) != ZEPHYRUM)
                    {
                        i32 stricti = _nota_aurea_valores(verbum->feats,
                            k, FALSUM);

                        census->notae_recti[k] =
                            census->notae_recti[k] + I;
                        si (((stricti >> (i32)nostra) & (i32)I)
                            == ZEPHYRUM)
                        {
                            census->notae_conventione[k] =
                                census->notae_conventione[k] + I;
                        }
                    }
                    alioquin
                    {
                           s32 distantia = (s32)-I;
                           s32 alter;
                        chorda socius;

                        socius.datum    = NIHIL;
                        socius.mensura  = ZEPHYRUM;
                        si (e_primum->nodus != NIHIL)
                        {
                            socius = _socius(textus, e_primum->nodus,
                                &distantia);
                        }
                        _erratum_notae_notare(piscina, census, k,
                            e_primum->classis[ZEPHYRUM],
                            _nota_aurea(verbum->feats, k, &alter),
                            nostra,
                            e_primum->decisio, e_primum->auctor,
                            verbum->forma, socius, distantia,
                            (b32)((e_primum->notae_praesentes[k] & aurei)
                                != ZEPHYRUM));
                    }
                }
            }
        }
    }
        /* T19g: partitio decisionum - verbum aureum QUODQUE per genus
     * decisionis elementi primi sui (nulla sine elemento, ranga = verbum
     * rangae non primum); auctor per regulam. Lex summae in porta:
     * partitio tota == verba - inalignata. */
    {
        constans Elementum* e = e1 > e0
            ? (constans Elementum*)xar_obtinere(elementa, e0) : NIHIL;
                       i32 p;

                si (e == NIHIL || e->ignotum)
                {
            /* T19k: lectio ignoti = cellula sine candidato noto (nulla),
             * non 'una' */
            p = ORATIO_ORACULUM_PARTITIO_NULLA;
                }
        alioquin si (!primum)

                {
            p = ORATIO_ORACULUM_PARTITIO_RANGA;
                }
        alioquin si (   e->decisio >= ZEPHYRUM
                     && e->decisio < (s32)ORATIO_DECISIO_NUMERUS)
                {
                                    p = (i32)e->decisio;
            si (e->auctor.mensura > ZEPHYRUM)
            {
                /* T19g bis: socius ligationis et distantia pro auctore
                 * (vicina | remota); decisio falsa tabulata cum socio */
                   s32 distantia = (s32)-I;
                chorda socius;

                socius.datum    = NIHIL;
                socius.mensura  = ZEPHYRUM;
                si (e->nodus != NIHIL)
                {
                    socius = _socius(textus, e->nodus, &distantia);
                }
                                _auctorem_notare(piscina, census,
                                    e->auctor, primaria,
                                    distantia, casus_iudicatus);
                si (!primaria)
                {
                    _erratum_notare(piscina, census, e->auctor,
                        verbum->forma, aurea,
                        e->numerus_classium > ZEPHYRUM
                            ? e->classis[ZEPHYRUM]
                            : ORATIO_CLASSIS_NUMERUS_CLASSIUM,
                        socius, distantia);
                }
            }


                }
        alioquin si (e->numerus_analysium == I)
                {
            p = ORATIO_ORACULUM_PARTITIO_UNA;
                }
        alioquin si (e->numerus_analysium > I)
                {
            p = ORATIO_ORACULUM_PARTITIO_APERTA;
                }
        alioquin
                {
            p = ORATIO_ORACULUM_PARTITIO_NULLA;
                }
        census->partitio_verba[p] = census->partitio_verba[p] + I;
        si (primaria)
        {
            census->partitio_primaria[p] =
                census->partitio_primaria[p] + I;
        }
    }


    /* T19a: tectum sed non primum = discrepantia (classis nostra prima
     * elementi primi); lex summae: tecta - primaria per classem */
    si (tectum && !primaria && e1 > e0)
    {
        constans Elementum* primum_elementum =
            (constans Elementum*)xar_obtinere(elementa, e0);

        _discrepantiam_notare(piscina, census, aurea, verbum->forma,
            primum_elementum->numerus_classium > ZEPHYRUM
                ? primum_elementum->classis[ZEPHYRUM]
                : ORATIO_CLASSIS_NUMERUS_CLASSIUM);
    }
        si (lemma)
        {
        census->lemmata  = census->lemmata + I;
        c->lemmata       = c->lemmata + I;
        }
    si (ignotum)
    {
        census->ignota  = census->ignota + I;
        c->ignota       = c->ignota + I;
    }
    /* T20a: par clausulae (verbum alignatum primum, non PUNCT,
     * clausula aurea nota) */
    si (   paria          != NIHIL && primum && e1 > e0
        && clausula_aurea >= ZEPHYRUM
        && aurea          != ORATIO_CLASSIS_INTERPUNCTIO)
    {
        constans Elementum* e =
            (constans Elementum*)xar_obtinere(elementa, e0);
        ParClausulae* p = (ParClausulae*)xar_addere(paria);

        si (p != NIHIL)
        {
            p->nostra  = e->clausula;
            p->causa   = e->clausula_causa;
            p->aurea   = clausula_aurea;
            p->forma   = verbum->forma;
        }
    }
}

/* species clausulae c sententiae (nodus); -I si absens */
interior s32
_species_clausulae (
    constans MateriaNodus* sententia,
                      s32  c)
{
    constans MateriaValor* clausulae;
    constans MateriaValor* v;

    si (   sententia        == NIHIL
        || sententia->genus != (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde (s32)-I;
    }
    clausulae = &sententia->loci[ORATIO_SENTENTIA_CLAUSULAE];
    si (   clausulae->genus != MATERIA_VALOR_LISTA || c < ZEPHYRUM
        || (i32)c           >= materia_valor_lista_numerus(*clausulae))
    {
        redde (s32)-I;
    }
    v = materia_valor_lista_obtinere(*clausulae, (i32)c);
    si (   v == NIHIL || v->genus != MATERIA_VALOR_NODUS
        || v->datum.nodus->loci[ORATIO_CLAUSULA_SPECIES].genus
            != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde v->datum.nodus->loci[ORATIO_CLAUSULA_SPECIES].datum.index;
}

/* numerus clausularum nostrarum sententiae (0 sine stampa) */
interior i32
_numerus_clausularum (
    constans MateriaNodus* sententia)
{
    constans MateriaValor* clausulae;

    si (   sententia        == NIHIL
        || sententia->genus != (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde ZEPHYRUM;
    }
    clausulae = &sententia->loci[ORATIO_SENTENTIA_CLAUSULAE];
    redde clausulae->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*clausulae) : ZEPHYRUM;
}

/* PURITAS (decisio XLVII): per clausulam nostram c clausula aurea
 * MAIOR inter paria; par rectum si aurea == maior; per causam. Numeri
 * clausularum: nostrae (nodi) contra aureae (radices). */
/* sententiam ostendere (T20a quater): si quota manet et verbum causae
 * petitae male positum adest, textus et verba in census->ostensae */
interior vacuum
_sententiam_ostendere (
                           Piscina* piscina,
              OratioOraculumCensus* census,
    constans OratioConlluSententia* s,
                            chorda  textus,
                               Xar* paria,
                      constans s32* maior,
                               i32  nc)
{
                               i32 np = xar_numerus(paria);
                               i32 nl =
                                   xar_numerus(s->lexemata);
                               b32  malum_petitum = FALSUM;
    OratioOraculumSententiaOstensa* o;
                               i32  k;

    si (census->ostendendae <= ZEPHYRUM)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < np; k++)
    {
        constans ParClausulae* p = (constans ParClausulae*)xar_obtinere(
            paria, k);
        b32 malum = (b32)(p->nostra < ZEPHYRUM || (i32)p->nostra >= nc
            || maior[p->nostra] != p->aurea);

        si (   malum
            && (   census->causa_ostendenda < ZEPHYRUM
                || census->causa_ostendenda == p->causa))
        {
            malum_petitum = VERUM;
        }
    }
    si (!malum_petitum)
    {
        redde;
    }
    si (census->ostensae == NIHIL)
    {
        census->ostensae = xar_creare(piscina,
            (i32)magnitudo(OratioOraculumSententiaOstensa));
        si (census->ostensae == NIHIL)
        {
            redde;
        }
    }
    o = (OratioOraculumSententiaOstensa*)xar_addere(census->ostensae);
    si (o == NIHIL)
    {
        redde;
    }
    o->textus  = _copia(piscina, textus);
    o->verba   = xar_creare(piscina,
        (i32)magnitudo(OratioOraculumVerbumOstensum));
    si (o->verba == NIHIL)
    {
        xar_removere_ultimum(census->ostensae);
        redde;
    }
    per (k = ZEPHYRUM; k < np; k++)
    {
        constans ParClausulae* p = (constans ParClausulae*)xar_obtinere(
            paria, k);
        OratioOraculumVerbumOstensum* v =
            (OratioOraculumVerbumOstensum*)xar_addere(o->verba);

        si (v == NIHIL)
        {
            redde;
        }
        v->forma   = _copia(piscina, p->forma);
        v->nostra  = p->nostra;
        v->causa   = p->causa;
        v->malum   = (b32)(p->nostra < ZEPHYRUM || (i32)p->nostra >= nc
            || maior[p->nostra] != p->aurea);
        v->radix.datum    = NIHIL;
        v->radix.mensura  = ZEPHYRUM;
        si (p->aurea >= ZEPHYRUM && (i32)p->aurea < nl)
        {
            v->radix = _copia(piscina, ((constans OratioConlluLexema*)
                xar_obtinere(s->lexemata, (i32)p->aurea))->forma);
        }
    }
    census->ostendendae = census->ostendendae - I;
}

interior b32
_clausulas_iudicare (
                           Piscina* scratch,
                           Piscina* piscina,
              OratioOraculumCensus* census,
    constans OratioConlluSententia* s,
             constans MateriaNodus* sententia,
                            chorda  textus,
                               Xar* paria,
                      constans s32* cl)
{
    i32  np = xar_numerus(paria);
    i32  nc = _numerus_clausularum(sententia);
    i32  nl = xar_numerus(s->lexemata);
    s32* maior;
    i32* tabula;   /* nc x nl */
    i32  aureae = ZEPHYRUM;
    i32  c;
    i32  k;

    si (np == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < nl; k++)
    {
        si (cl[k] == (s32)k)
        {
            aureae = aureae + I;
        }
    }
    census->clausulae_sententiae  = census->clausulae_sententiae + I;
    census->clausulae_nostrae     = census->clausulae_nostrae + nc;
    census->clausulae_aureae      = census->clausulae_aureae + aureae;
    si (nc == aureae)
    {
        census->clausulae_pares = census->clausulae_pares + I;
    }
    maior  = (s32*)piscina_allocare(scratch, (memoriae_index)(nc + I)
        * (memoriae_index)magnitudo(s32));
    tabula = (i32*)piscina_allocare(scratch, (memoriae_index)(nc + I)
        * (memoriae_index)(nl + I) * (memoriae_index)magnitudo(i32));
    si (maior == NIHIL || tabula == NIHIL)
    {
        redde FALSUM;
    }
    memset(tabula, ZEPHYRUM, (size_t)((nc + I) * (nl + I))
        * magnitudo(i32));
    per (k = ZEPHYRUM; k < np; k++)
    {
        constans ParClausulae* p = (constans ParClausulae*)xar_obtinere(
            paria, k);

        si (   p->nostra >= ZEPHYRUM && (i32)p->nostra < nc
            && p->aurea  >= ZEPHYRUM && (i32)p->aurea < nl)
        {
            i32 cella = (i32)p->nostra * (nl + I) + (i32)p->aurea;

            tabula[cella] = tabula[cella] + I;
        }
    }
        per (c = ZEPHYRUM; c < nc; c++)
        {
        i32 optimum = ZEPHYRUM;

        maior[c] = (s32)-I;
        per (k = ZEPHYRUM; k < nl; k++)
        {
            si (tabula[c * (nl + I) + k] > optimum)
            {
                optimum   = tabula[c * (nl + I) + k];
                maior[c]  = (s32)k;
            }
        }
        }
    _sententiam_ostendere(piscina, census, s, textus, paria, maior, nc);

        /* concordia parium: verba posita ambo, par idem iudicatum */
    per (k = ZEPHYRUM; k < np; k++)
    {
        constans ParClausulae* p = (constans ParClausulae*)xar_obtinere(
            paria, k);
        i32 j;

        per (j = k + I; j < np; j++)
        {
            constans ParClausulae* q =
                (constans ParClausulae*)xar_obtinere(paria, j);
            b32 nostrum = (b32)(p->nostra >= ZEPHYRUM
                && p->nostra == q->nostra);
            b32 aureum  = (b32)(p->aurea == q->aurea);

            census->clausulae_paria = census->clausulae_paria + I;
            si (nostrum == aureum)
            {
                census->clausulae_paria_concordia =
                    census->clausulae_paria_concordia + I;
            }
        }
    }
    per (k = ZEPHYRUM; k < np; k++)
    {
        constans ParClausulae* p = (constans ParClausulae*)xar_obtinere(
            paria, k);
        i32 causa = (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS;   /* aperta */
        b32 recta = FALSUM;


        census->clausulae_iudicata = census->clausulae_iudicata + I;
        si (   p->nostra >= ZEPHYRUM && (i32)p->nostra < nc
            && p->causa  >= ZEPHYRUM
            && p->causa < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS)
        {
            causa = (i32)p->causa;
            recta = (b32)(maior[p->nostra] == p->aurea);
        }
        census->clausulae_verba[causa] = census->clausulae_verba[causa]
            + I;
        si (recta)
        {
            census->clausulae_rectae[causa] =
                census->clausulae_rectae[causa] + I;
        }
        alioquin si (causa < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS)
        {
            chorda radix;

            radix.datum    = NIHIL;
            radix.mensura  = ZEPHYRUM;
            si (p->aurea >= ZEPHYRUM && (i32)p->aurea < nl)
            {
                radix = ((constans OratioConlluLexema*)xar_obtinere(
                    s->lexemata, (i32)p->aurea))->forma;
            }
            _erratum_clausulae_notare(piscina, census, p->causa,
                p->forma, _species_clausulae(sententia, p->nostra),
                radix);
        }
    }
    redde VERUM;
}

/* resolutio optionalis (T17): programma + regulae + intern + ratum */
nomen structura {
       constans OratioProgramma* programma;
                            s32  regulae_numerus;
            InternamentumChorda* intern;
   constans MateriaLexiconRatum* ratum;
             constans character* lingua_documenti;   /* NIHIL = census
                                                  * sententiae solius */
} Resolutio;

interior b32
_sententiam_iudicare (
                           Piscina* piscina,
        constans OratioVocabularia* vocabularia,
    constans OratioConlluSententia* s,
              OratioOraculumCensus* census,
                constans Resolutio* resolutio)
{
    Piscina* scratch = piscina_generare_dynamicum("oraculum_sententia",
        16777216);
    chorda textus;
    MateriaNodus* doc;
    Xar* elementa;
    i32 cursor = ZEPHYRUM;
    i32 k;
    i32 n;
        i32 e_proximum = ZEPHYRUM;
    i32 lingua_index;
            OratioPartesCensus census_partium;
 OratioResolutioCensus census_resolutionis;   /* T20b: catena */
                  Xar* paria;   /* T20a: ParClausulae */

                  s32* cl;
                  b32* fin;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    census->sententiae  = census->sententiae + I;
    textus              = oratio_conllu_textus(scratch, s);
    doc = textus.datum != NIHIL
        ? oratio_arbor_parsare(scratch,
        (constans character*)textus.datum,
            textus.mensura)
        : NIHIL;
    elementa = xar_creare(scratch, (i32)magnitudo(Elementum));
        si (   doc == NIHIL || elementa == NIHIL
            || !oratio_partes_annotare_dialecto(scratch, vocabularia,
            doc,
                &census_partium, (OratioDialectus)census->dialectus))
        {
        census->sententiae_fractae = census->sententiae_fractae + I;
        piscina_destruere(scratch);
        redde VERUM;
        }
    /* census linguae sententiae (T18): relatus per thesaurum - regula
     * linguae solum sententias Anglicas iudicatas tangit */
        lingua_index = oratio_resolutio_lingua_censu_index(
            census_partium.vocabula_linguarum);
                census->sententiae_linguae[lingua_index] =
                    census->sententiae_linguae[lingua_index] + I;
                oratio_resolutio_census_vacare(&census_resolutionis);
        si (   (   resolutio != NIHIL && resolutio->programma != NIHIL
            && !oratio_resolutio_applicare(scratch, resolutio->intern,
                resolutio->ratum, resolutio->programma,
                resolutio->regulae_numerus,
                resolutio->lingua_documenti != NIHIL
                    ? resolutio->lingua_documenti
                    : ORATIO_TITULI_LINGUARUM[lingua_index], doc,
                &census_resolutionis))
            || !_elementa_colligere(scratch, vocabularia->la, doc,
            elementa))
        {
        census->sententiae_fractae = census->sententiae_fractae + I;
        piscina_destruere(scratch);
        redde VERUM;
        }
        /* T20a: clausulae aureae (radices finitae) et paria pro puritate;
     * resolutio absens (crudus) = paria sine clausula nostra (apertae) */
    paria = xar_creare(scratch, (i32)magnitudo(ParClausulae));
    si (paria == NIHIL || !_clausulas_aureas(scratch, s, &cl, &fin))
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    n = xar_numerus(s->lexemata);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans OratioConlluLexema* t =
            (constans OratioConlluLexema*)xar_obtinere(s->lexemata, k);
        s32 ga;
        s32 gb;
        i32 e0;
        i32 e1;
        i32 verba_rangae = t->ranga ? t->b - t->a + I : ZEPHYRUM;
        i32 w;


        si (t->ranga)
        {
            census->rangae = census->rangae + I;
        }
        ga = _formam_invenire(textus, cursor, t->forma);
        si (ga < ZEPHYRUM)
        {
            /* forma in textu absens: verba inalignata, cursor manet */
            per (w = ZEPHYRUM; w <= verba_rangae; w++)
            {
                constans OratioConlluLexema* v =
                    (constans OratioConlluLexema*)
                    xar_obtinere(s->lexemata, k + (t->ranga ? w
                        + I : ZEPHYRUM));

                si (!t->ranga && w > ZEPHYRUM)
                {
                    frange;
                }
                si (t->ranga && w == verba_rangae)
                {
                    frange;
                }
                si (k + (t->ranga ? w + I : ZEPHYRUM) < n)
                {
                    OratioClassis aurea =
                        oratio_oraculum_classis_ex_upos(
                        v->upos);

                    census->verba       = census->verba + I;
                    census->inalignata  = census->inalignata + I;
                    census->classes[aurea].verba =
                        census->classes[aurea].verba + I;
                    census->classes[aurea].inalignata =
                        census->classes[aurea].inalignata + I;
                }
            }
            k = k + verba_rangae;
            perge;
        }
        gb      = ga + (s32)t->forma.mensura;
        cursor  = (i32)gb;
        /* elementa quae extentum [ga, gb) tangunt */
        dum (   e_proximum < xar_numerus(elementa)
             && ((constans Elementum*)xar_obtinere(elementa,
            e_proximum))->b
                <= ga)
        {
            e_proximum = e_proximum + I;
        }
        e0 = e_proximum;
        e1 = e0;
        dum (   e1 < xar_numerus(elementa)
             && ((constans Elementum*)xar_obtinere(elementa, e1))->a
                < gb)
        {
            e1 = e1 + I;
        }
                                si (t->ranga)
                                {
            /* T21: pars rangae cuius forma elementum unum inter
             * [e0, e1) octetim aequat (encliticum scissum: arma + que)
             * contra id solum ut verbum primum iudicatur; aliter ut
             * ante (prima contra elementa omnia, ceterae 'ranga') */
            per (w = ZEPHYRUM; w < verba_rangae && k + I + w < n; w++)
            {
                constans OratioConlluLexema* pars =
                    (constans OratioConlluLexema*)xar_obtinere(
                    s->lexemata, k + I + w);
                                s32 ep = _elementum_formae(textus,
                                    elementa, e0, e1,
                                    pars->forma);

                                si (ep >= ZEPHYRUM)
                                {
                    ((Elementum*)xar_obtinere(elementa,
                        (i32)ep))->lexema =
                        (s32)(k + I + w);
                    _verbum_iudicare(piscina, census, textus, elementa,
                        (i32)ep, (i32)ep + I, pars, VERUM, paria,
                        cl[k + I + w]);
                                }
                alioquin
                                {
                    si (w == ZEPHYRUM && e1 > e0)
                    {
                        ((Elementum*)xar_obtinere(elementa,
                            e0))->lexema =
                            (s32)(k + I);
                    }
                    _verbum_iudicare(piscina, census, textus, elementa,
                        e0, e1, pars, (b32)(w == ZEPHYRUM), paria,
                        cl[k + I + w]);
                                }
            }
            k = k + verba_rangae;
                                }
                alioquin
                                {
            si (e1 > e0)
            {
                ((Elementum*)xar_obtinere(elementa, e0))->lexema =
                    (s32)k;
            }
                        _verbum_iudicare(piscina, census, textus,
                            elementa, e0, e1,
                            t, VERUM, paria, cl[k]);
                                }
    }
    /* T26: ligationes contra capita aurea */
    si (!_ligationes_iudicare(piscina, scratch, census, s, elementa))
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
        /* T20b: discordiae et catenatae ex censu resolutionis */
    census->catena_discordiae = census->catena_discordiae
        + census_resolutionis.clausulae.discordiae;
    census->catena_catenatae = census->catena_catenatae
        + census_resolutionis.clausulae.catenatae;
    /* T20a: puritas clausularum sententiae (sententia = pater elementi
     * primi) */
    si (xar_numerus(elementa) > ZEPHYRUM)
    {
        constans Elementum* e0_elementum =
            (constans Elementum*)xar_obtinere(elementa, ZEPHYRUM);

                si (!_clausulas_iudicare(scratch, piscina, census, s,
                    e0_elementum->nodus != NIHIL
                    ? e0_elementum->nodus->pater : NIHIL,
                    textus, paria, cl))
                {
            piscina_destruere(scratch);
            redde FALSUM;
                }
    }
    (vacuum)fin;
    piscina_destruere(scratch);
    redde VERUM;
}

b32
oratio_oraculum_iudicare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                              Xar* sententiae,
             OratioOraculumCensus* census)
{
    redde oratio_oraculum_iudicare_resolutum(piscina, vocabularia,
        NIHIL,
        (s32)-I, sententiae, census);
}

/* census linguae DOCUMENTI (T18): thesaurus totus annotatur semel ante
 * iudicium, suffragia vocabulorum summantur, lingua = plurima (Latina
 * in aequalitate). Sententia sola fallit (EWT: CXXXIV sententiae 'I
 * have.', nomina sola, lineae sine verbis Latinae iudicatae); documentum
 * totum non fallit - et ita instrumenta (arbor, verba) censent. Pretium:
 * parsura + annotatio bis per sententiam (~I s per thesaurum). */
interior i32
_linguam_documenti_censere (
                       Piscina* piscina,
    constans OratioVocabularia* vocabularia,
                           Xar* sententiae,
          OratioOraculumCensus* census)
{
    i32 i;
    i32 k;

    (vacuum)piscina;
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        constans OratioConlluSententia* s =
            (constans OratioConlluSententia*)xar_obtinere(sententiae,
            i);
        Piscina* scratch = piscina_generare_dynamicum(
            "oraculum_census_linguae", 16777216);
        chorda textus;
        MateriaNodus* doc;
        OratioPartesCensus census_partium;

        si (scratch == NIHIL)
        {
            redde ZEPHYRUM;
        }
        textus = oratio_conllu_textus(scratch, s);
        doc = textus.datum != NIHIL
            ? oratio_arbor_parsare(scratch,
            (constans character*)textus.datum, textus.mensura)
            : NIHIL;
        si (   doc != NIHIL
            && oratio_partes_annotare(scratch, vocabularia, doc,
                &census_partium))
        {
            /* T22 b: census dialecti in eodem circuitu */
            census->dialectus_recuperata = census->dialectus_recuperata
                + census_partium.orthographia;
            census->dialectus_verba = census->dialectus_verba
                + census_partium.vocabula;
            per (k = ZEPHYRUM; k < (i32)ORATIO_LINGUA_NUMERUS; k++)
            {
                census->suffragia_linguarum[k] =
                    census->suffragia_linguarum[k]
                    + census_partium.vocabula_linguarum[k];
            }
        }
        piscina_destruere(scratch);
    }
    redde oratio_resolutio_lingua_censu_index(
        census->suffragia_linguarum);
}

b32
oratio_oraculum_iudicare_resolutum (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
         constans OratioProgramma* programma,
                              s32  regulae_numerus,
                              Xar* sententiae,
             OratioOraculumCensus* census)
{
              Resolutio resolutio;
    MateriaLexiconRatum ratum;
     MateriaLexIudicium iudicium;
                    i32 i;

    resolutio.programma         = programma;
    resolutio.regulae_numerus   = regulae_numerus;
    resolutio.intern            = NIHIL;
    resolutio.ratum             = NIHIL;
    resolutio.lingua_documenti  = NIHIL;
    si (programma != NIHIL)
    {
        resolutio.intern = internamentum_creare(piscina);
        si (   resolutio.intern == NIHIL
            || !materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
                &iudicium))
        {
            redde FALSUM;
        }
        resolutio.ratum = &ratum;
        census->lingua_documenti = (s32)_linguam_documenti_censere(
            piscina, vocabularia, sententiae, census);
        resolutio.lingua_documenti =
            ORATIO_TITULI_LINGUARUM[(i32)census->lingua_documenti];
        census->dialectus = (s32)oratio_partes_dialectus_censu(
            census->dialectus_recuperata, census->dialectus_verba);
    }
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        si (!_sententiam_iudicare(piscina, vocabularia,
                (constans OratioConlluSententia*)xar_obtinere(sententiae,
                i),
                census, programma != NIHIL ? &resolutio : NIHIL))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
