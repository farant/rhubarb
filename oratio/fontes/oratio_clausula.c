/* oratio_clausula.c - Vide oratio_clausula.h. */

#include "oratio_clausula.h"
#include "oratio_partes.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * DATA seminum (census thesaurorum UD 2026-09-08, scratchpad semina2)
 * ================================================== */

/* SCONJ sine exceptione in thesauris Latinis tribus (aut in lista
 * SUBORDINANTES_CERTAE T19b cum testimonio parvo) et limes clausulae
 * altus: si (CIRCSE 73 %, LLCT 99/98), quia (100 ubique), qualiter
 * (100/100), dum (85/86/90), quod (70/91/88 - forma etiam relativa
 * 'qui', utrumque semen), sicut (LLCT 100, Senecae n=I), donec (100,
 * n=IX), quamquam (100, n=IV), quando (100, n=III); ceterae ex T19b
 * (SCONJ certae, limes n<III 100 %). */
constans character* constans ORATIO_SEMINA_CERTA[] = {
    "si", "quia", "qualiter", "dum", "quod", "sicut", "sicuti", "donec",
    "quamquam", "quando", "quoniam", "antequam", "priusquam",
        "postquam",
    "etsi", "quamvis", "quatenus", "quomodo", "quotiens", "quin",
    NIHIL
};

/* Semen solum cum verbo finito CAPACI inter se et semen proximum:
 * ut (Senecae SCONJ 94 % sed limes 57 % - comparativum 'ut leo';
 * chartis 98/92), cum (SCONJ 48 % Senecae, limes 80 %; chartis
 * adpositio 98 %), nisi (Senecae 44 % - 'nisi forte' sine verbo;
 * chartis coordinans 'nisi' limes 100 %), ubi (adverbium relativum:
 * 90 / 71 / 71 %), unde (100 %, n parvus). Mensurata et OMISSA T20a:
 * quam (limes 21-58 %, comparativum), ne (particula 75 %), licet
 * (verbum 55 %), quo/qua (adverbia). */
constans character* constans ORATIO_SEMINA_CORROBORANDA[] = {
    "ut", "cum", "nisi", "ubi", "unde",
    NIHIL
};

/* formae relativae: lemma lectionis pronominis aut determinantis */
constans character* constans ORATIO_SEMINA_RELATIVA[] = {
    "qui", "quicumque", "quisquis",
    NIHIL
};

/* coordinantia inter verba finita capacia utrimque (limes 76-100 %
 * inter verba finita aurea, 22-31 % aliter); -que/-ve enclitica
 * omissa (limes 0-3 % - post verbum hospitem stant); vel/seu/sive
 * omissa (57-62 % ff, 15 % aliter, n parvus) */
constans character* constans ORATIO_SEMINA_COORDINANTIA[] = {
    "et", "atque", "ac", "sed", "aut", "nec", "neque", "at",
    NIHIL
};


/* ==================================================
 * Adiumenta
 * ================================================== */

interior b32
_in_lista (
    constans character* constans* lista,
                           chorda  lemma)
{
    i32 i;

    per (i = ZEPHYRUM; lista[i] != NIHIL; i++)
    {
        i32 l = (i32)strlen(lista[i]);

        si (   l                                        == lemma.mensura
            && memcmp(lista[i], lemma.datum, (size_t)l) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior s32
_accidens (
    constans MateriaNodus* analysis,
       constans character* titulus)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    locus = oratio_partes_locus(classis, titulus);
    si (   locus < ZEPHYRUM || (i32)locus >= analysis->numerus_locorum
        || analysis->loci[locus].genus != MATERIA_VALOR_INDEX)
    {
        redde (s32)-I;
    }
    redde analysis->loci[locus].datum.index;
}

interior chorda
_lemma (
    constans MateriaNodus* analysis)
{
    constans MateriaValor* l = &analysis->loci[ORATIO_ANALYSIS_LEMMA];
                   chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (l->genus != MATERIA_VALOR_TOKEN || l->datum.token == NIHIL)
    {
        redde vacua;
    }
    redde l->datum.token->valor;
}

/* forma superficiei vocabuli (pars una) == literae, minusculis ASCII
 * (Quis initiale); FALSUM si partes plures aut absentes */
interior b32
_forma_est (
    constans MateriaNodus* vocabulum,
       constans character* literae)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
    constans MateriaValor* pars;
                   chorda  forma;
                      i32  l = (i32)strlen(literae);
                      i32  i;

    si (   partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) != I)
    {
        redde FALSUM;
    }
    pars = materia_valor_lista_obtinere(*partes, ZEPHYRUM);
    si (   pars == NIHIL || pars->genus != MATERIA_VALOR_TOKEN
        || pars->datum.token == NIHIL)
    {
        redde FALSUM;
    }
    forma = pars->datum.token->valor;
    si (forma.mensura != l)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < l; i++)
    {
        i8 c = forma.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (i8)(c + ('a' - 'A'));
        }
        si (c != (i8)literae[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_latina (
    constans MateriaNodus* analysis)
{
    constans MateriaValor* l = &analysis->loci[ORATIO_ANALYSIS_LINGUA];

    redde (b32)(l->genus == MATERIA_VALOR_INDEX
        && l->datum.index == (s32)ORATIO_LINGUA_LATINA);
}

b32
oratio_clausula_lectio_finita (
    constans MateriaNodus* analysis)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 forma;
              s32 modus;

    si (   classis != ORATIO_CLASSIS_VERBUM
        && classis != ORATIO_CLASSIS_AUXILIARE)
    {
        redde FALSUM;
    }
    si (!_latina(analysis))
    {
        redde FALSUM;
    }
    forma = _accidens(analysis, "forma-verbi");
    si (forma >= ZEPHYRUM)
    {
        redde (b32)(forma == (s32)ORATIO_FORMA_VERBI_FINITUM);
    }
    modus = _accidens(analysis, "modus");
    redde (b32)(   modus == (s32)ORATIO_MODUS_INDICATIVUS
                || modus == (s32)ORATIO_MODUS_SUBIUNCTIVUS
                || modus == (s32)ORATIO_MODUS_IMPERATIVUS);
}

/* genera seminis candidati elementi */
nomen enumeratio {
    SEMEN_NULLUM = 0,
    SEMEN_CERTUM,
    SEMEN_CORROBORANDUM,
    SEMEN_RELATIVUM,
    SEMEN_COORDINANS,
    SEMEN_SIGNUM,         /* , ; : (signum claudens) */
    SEMEN_APERIENS,       /* ( */
    SEMEN_CLAUDENS        /* ) */
} GenusSeminis;

nomen structura {
    MateriaNodus* nodus;   /* mutabilis: loci clausulae scribuntur */
             s32  semen;          /* GenusSeminis candidatum */
                      b32  finita_capax;   /* lectio finita ulla */
                      b32  finita_certa;   /* lectiones omnes finitae */
                      b32  seminat;        /* post corroborationem */
                      s32  clausula;       /* -I = aperta */
                      s32  causa;
} Membrum;

nomen structura {
    s32 species;
    s32 semen;    /* ordinalis elementi; -I */
    s32 pater;    /* -I = summa */
    i32 membra;
    b32 clausibilis;   /* verbo finito certo aut signo clauditur
                        * (subordinata, relativa, soror earum);
                        * principalis numquam, parenthetica signo
                        * suo solo */
} Clausula;

interior vacuum
_membrum_describere (
    MateriaNodus* nodus,
         Membrum* m)
{
    constans MateriaValor* analyses;
                      i32  n;
                      i32  a;
                      i32  finitae = ZEPHYRUM;

    m->nodus         = nodus;
    m->semen         = (s32)SEMEN_NULLUM;
    m->finita_capax  = FALSUM;
    m->finita_certa  = FALSUM;
    m->seminat       = FALSUM;
    m->clausula      = (s32)-I;
    m->causa         = (s32)-I;
    si (nodus->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        constans MateriaValor* signum =
            &nodus->loci[ORATIO_INTERPUNCTIO_SIGNUM];

        si (   signum->genus                      == MATERIA_VALOR_TOKEN
            && signum->datum.token                != NIHIL
            && signum->datum.token->valor.mensura == I)
        {
            i8 c = signum->datum.token->valor.datum[ZEPHYRUM];

            si (c == ',' || c == ';' || c == ':')
            {
                m->semen = (s32)SEMEN_SIGNUM;
            }
            alioquin si (c == '(')
            {
                m->semen = (s32)SEMEN_APERIENS;
            }
            alioquin si (c == ')')
            {
                m->semen = (s32)SEMEN_CLAUDENS;
            }
        }
        redde;
    }
    si (nodus->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde;
    }
    analyses = &nodus->loci[ORATIO_VOCABULUM_ANALYSES];
    si (analyses->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    n = materia_valor_lista_numerus(*analyses);
    per (a = ZEPHYRUM; a < n; a++)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(*analyses,
            a);
        constans MateriaNodus* lectio;
                OratioClassis  classis;
                       chorda  lemma;

        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        lectio = v->datum.nodus;
        si (oratio_clausula_lectio_finita(lectio))
        {
            m->finita_capax  = VERUM;
            finitae          = finitae + I;
        }
        si (!_latina(lectio))
        {
            perge;
        }
        classis  = oratio_genus_classis((OratioGenus)lectio->genus);
        lemma    = _lemma(lectio);
        si (lemma.mensura == ZEPHYRUM)
        {
            perge;
        }
        /* prioritas: certum > relativum > corroborandum > coordinans */
        si (_in_lista(ORATIO_SEMINA_CERTA, lemma))
        {
            m->semen = (s32)SEMEN_CERTUM;
        }
                alioquin si (   (   classis == ORATIO_CLASSIS_PRONOMEN
                             || classis == ORATIO_CLASSIS_DETERMINANS)
                             && _in_lista(ORATIO_SEMINA_RELATIVA, lemma)
                             && m->semen != (s32)SEMEN_CERTUM
                             && !_forma_est(nodus, "quis")
                             && !_forma_est(nodus, "quid"))
        {
            /* quis/quid: indefinitum post si/ne/num (si quis
             * chartarum), interrogativum - relativum nominativus
             * qui/quae/quod est; WORDS lemma unum 'qui' omnibus dat,
             * UD lemma 'quis' seorsum (census: qui PRON 94-97 %
             * limes chartis sine his formis) */
            m->semen = (s32)SEMEN_RELATIVUM;
        }
        alioquin si (   _in_lista(ORATIO_SEMINA_CORROBORANDA, lemma)
                     && m->semen == (s32)SEMEN_NULLUM)
        {
            m->semen = (s32)SEMEN_CORROBORANDUM;
        }
        alioquin si (   classis  == ORATIO_CLASSIS_CONIUNCTIO_COORDINANS
                     && _in_lista(ORATIO_SEMINA_COORDINANTIA, lemma)
                     && m->semen == (s32)SEMEN_NULLUM)
        {
            m->semen = (s32)SEMEN_COORDINANS;
        }
    }
    m->finita_certa = (b32)(n > ZEPHYRUM && finitae == n);
}

/* stratum I: corroboratio - verbum finitum capax inter k et semen
 * proximum (certum, relativum, signum claudens, finis); coordinans: utrimque
 * intra segmentum a candidato quovis terminatum */
interior b32
_terminus_corroborationis (
    constans Membrum* m)
{
    redde (b32)(   m->semen == (s32)SEMEN_CERTUM
                || m->semen == (s32)SEMEN_RELATIVUM
                || m->semen == (s32)SEMEN_SIGNUM
                || m->semen == (s32)SEMEN_APERIENS
                || m->semen == (s32)SEMEN_CLAUDENS);
}

interior vacuum
_semina_iudicare (
                 Membrum* membra,
                     i32  n,
    OratioClausulaCensus* census)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
                Membrum* m = &membra[k];
                    i32  j;
                    s32  h;   /* signatus: ambulatio sinistrorsum */
                    b32  sinistra  = FALSUM;
                    b32  dextra    = FALSUM;

        si (   m->semen == (s32)SEMEN_CERTUM
            || m->semen == (s32)SEMEN_RELATIVUM)
        {
            m->seminat = VERUM;
            perge;
        }
        si (m->semen == (s32)SEMEN_CORROBORANDUM)
        {
            per (j = k + I; j < n
                && !_terminus_corroborationis(&membra[j]);
                 j++)
            {
                si (membra[j].finita_capax)
                {
                    dextra = VERUM;
                    frange;
                }
            }
            m->seminat = dextra;
            si (census != NIHIL)
            {
                si (dextra)
                {
                    census->corroborata = census->corroborata + I;
                }
                alioquin
                {
                    census->reiecta = census->reiecta + I;
                }
            }
            perge;
        }
        si (m->semen == (s32)SEMEN_COORDINANS)
        {
                        per (h = (s32)k - (s32)I; h >= ZEPHYRUM
                            && membra[h].semen
                                == (s32)SEMEN_NULLUM; h--)
                        {
                si (membra[h].finita_capax)
                {
                    sinistra = VERUM;
                    frange;
                }
                        }
            per (j = k + I; j < n
                && membra[j].semen == (s32)SEMEN_NULLUM; j++)
            {
                si (membra[j].finita_capax)
                {
                    dextra = VERUM;
                    frange;
                }
            }
            m->seminat = (b32)(sinistra && dextra);
            si (census != NIHIL)
            {
                si (m->seminat)
                {
                    census->corroborata = census->corroborata + I;
                }
                alioquin
                {
                    census->reiecta = census->reiecta + I;
                }
            }
        }
    }
}

/* clausulam novam addere; index eius */
interior s32
_clausulam_addere (
    Xar* clausulae,
    s32  species,
    s32  semen,
    s32  pater)
{
    Clausula* c = (Clausula*)xar_addere(clausulae);

    si (c == NIHIL)
    {
        redde (s32)-I;
    }
        c->species  = species;
    c->semen        = semen;
    c->pater        = pater;
    c->membra       = ZEPHYRUM;
    c->clausibilis  = (b32)(
           species == (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA
        || species == (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA);
    redde (s32)(xar_numerus(clausulae) - I);
}

/* strata II et III: acervus */
interior b32
_extentum_stampare (
                 Piscina* scratch,
                 Membrum* membra,
                     i32  n,
                     Xar* clausulae,
    OratioClausulaCensus* census)
{
    s32* acervus = (s32*)piscina_allocare(scratch,
        (memoriae_index)(n + I) * (memoriae_index)magnitudo(s32));
    i32 altitudo        = ZEPHYRUM;
    s32 summa           = (s32)-I;   /* clausula principalis; -I nondum */
    b32 post_clausuram  = FALSUM;
    b32 semen_ullum     = FALSUM;
    i32 k;

    si (acervus == NIHIL)
    {
        redde FALSUM;
    }
        per (k = ZEPHYRUM; k < n; k++)
        {
        si (   membra[k].seminat
            || membra[k].semen == (s32)SEMEN_APERIENS)
        {
            semen_ullum = VERUM;
        }
        }
    si (!semen_ullum)
    {
        /* sententia sine semine: clausula una, causa UNICA */
        summa = _clausulam_addere(clausulae,
            (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS, (s32)-I,
            (s32)-I);
        si (summa < ZEPHYRUM)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            membra[k].clausula  = summa;
            membra[k].causa     = (s32)ORATIO_CLAUSULA_CAUSA_UNICA;
            ((Clausula*)xar_obtinere(clausulae, (i32)summa))->membra
                = ((Clausula*)xar_obtinere(clausulae,
                (i32)summa))->membra
                + I;
        }
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        Membrum* m   = &membra[k];
            s32  top = altitudo > ZEPHYRUM ? acervus[altitudo - I]
                : (s32)-I;
            s32 nova;

        /* principalis pigre: verbum summi ordinis sine acervo aut
         * clausura ad parentem absentem */
        si (   top < ZEPHYRUM && !m->seminat
            && m->semen != (s32)SEMEN_APERIENS)
        {
            si (summa < ZEPHYRUM)
            {
                i32 c;

                summa = _clausulam_addere(clausulae,
                    (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS, (s32)-I,
                    (s32)-I);
                si (summa < ZEPHYRUM)
                {
                    redde FALSUM;
                }
                /* clausulae iam natae sine parente: principalis pater */
                per (c = ZEPHYRUM; c < (i32)summa; c++)
                {
                    Clausula* cl = (Clausula*)xar_obtinere(clausulae,
                        c);

                    si (cl->pater < ZEPHYRUM)
                    {
                        cl->pater = summa;
                    }
                }
            }
            acervus[altitudo]  = summa;
            altitudo           = altitudo + I;
            top                = summa;
        }
        si (m->seminat && m->semen == (s32)SEMEN_COORDINANS)
        {
            s32 pater = top >= ZEPHYRUM
                ? ((Clausula*)xar_obtinere(clausulae, (i32)top))->pater
                : (s32)-I;

            nova = _clausulam_addere(clausulae,
                (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA, (s32)k,
                pater);
            si (nova < ZEPHYRUM)
            {
                redde FALSUM;
            }
                        si (altitudo > ZEPHYRUM)
                        {
                /* soror substituit; clausibilis ut substituta */
                ((Clausula*)xar_obtinere(clausulae, (i32)nova))
                    ->clausibilis = ((Clausula*)xar_obtinere(
                        clausulae, (i32)top))->clausibilis;
                acervus[altitudo - I] = nova;
                        }
            alioquin
                        {
                acervus[altitudo]  = nova;
                altitudo           = altitudo + I;
                        }
            si (top == summa)
            {
                summa = nova;   /* coordinata principalis: summa nova */
            }
            m->clausula     = nova;
            m->causa        = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram  = FALSUM;
        }
        alioquin si (m->seminat)
        {
            s32 species = m->semen == (s32)SEMEN_RELATIVUM
                ? (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA
                : (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA;

            /* relativum initiale = relativum conectens: clausulam
             * principalem aperit (Qui cum venisset ...) */
            si (   m->semen == (s32)SEMEN_RELATIVUM && k == ZEPHYRUM
                && summa < ZEPHYRUM)
            {
                species = (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS;
            }
            nova = _clausulam_addere(clausulae, species, (s32)k, top);
            si (nova < ZEPHYRUM)
            {
                redde FALSUM;
            }
            si (species == (s32)ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS)
            {
                summa = nova;
            }
            acervus[altitudo]  = nova;
            altitudo           = altitudo + I;
            m->clausula        = nova;
            m->causa           = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram     = FALSUM;
        }
        alioquin si (m->semen == (s32)SEMEN_APERIENS)
        {
            nova = _clausulam_addere(clausulae,
                (s32)ORATIO_SPECIES_CLAUSULAE_PARENTHETICA, (s32)k,
                top);
            si (nova < ZEPHYRUM)
            {
                redde FALSUM;
            }
            acervus[altitudo]  = nova;
            altitudo           = altitudo + I;
            m->clausula        = nova;
            m->causa           = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram     = FALSUM;
        }
        alioquin si (m->semen == (s32)SEMEN_CLAUDENS)
        {
            m->clausula  = top;
            m->causa     = (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM;
            si (   altitudo > I
                && ((Clausula*)xar_obtinere(clausulae,
                (i32)top))->species
                    == (s32)ORATIO_SPECIES_CLAUSULAE_PARENTHETICA)
            {
                altitudo        = altitudo - I;
                post_clausuram  = VERUM;
                si (census != NIHIL)
                {
                    census->clausae_signo = census->clausae_signo + I;
                }
            }
        }
        alioquin si (m->semen == (s32)SEMEN_SIGNUM)
        {
            /* interpunctio membrum clausulae quam claudit */
                        m->clausula  = top;
            m->causa     = post_clausuram
                ? (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA
                : (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM;
            si (   altitudo > ZEPHYRUM
                && ((Clausula*)xar_obtinere(clausulae,
                (i32)top))->clausibilis)
            {
                altitudo        = altitudo - I;
                post_clausuram  = VERUM;
                si (census != NIHIL)
                {
                    census->clausae_signo = census->clausae_signo + I;
                }
            }
        }
        alioquin
        {
            m->clausula  = top;
            m->causa     = post_clausuram
                ? (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA
                : (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM;
            /* stratum III: verbum finitum certum clausulam non basalem
             * claudit (prior verbi finalis) */
                        si (   m->finita_certa && altitudo > ZEPHYRUM
                            && ((Clausula*)xar_obtinere(clausulae,
                            (i32)top))->clausibilis)
                        {
                altitudo        = altitudo - I;
                post_clausuram  = VERUM;
                si (census != NIHIL)
                {
                    census->clausae_verbo = census->clausae_verbo + I;
                }
                        }
        }
        si (m->clausula >= ZEPHYRUM)
        {
            Clausula* cl = (Clausula*)xar_obtinere(clausulae,
                (i32)m->clausula);

            cl->membra = cl->membra + I;
        }
    }
    /* summa nondum nata (sententia tota subordinata): patres absentes
     * manent non scripti (inventum: clausula principalis deest) */
    redde VERUM;
}

/* umbra clausulae e schemate: relatio + condicio una */
interior MateriaNodus*
_umbram_struere (
    Piscina* piscina,
        s32  relatio,
        i32  locus_condicionis,
        s32  valor)
{
    MateriaNodus* umbra = materia_nodus_creare(piscina,
        (s32)ORATIO_GENUS_UMBRA,
        ORATIO_REGISTRUM.genera[ORATIO_GENUS_UMBRA].loci_numerus);

    si (   umbra == NIHIL
        || !materia_nodus_ponere(umbra, (i32)ORATIO_UMBRA_RELATIO,
            materia_valor_index(relatio), MATERIA_LOCUS_INDEX)
        || !materia_nodus_ponere(umbra, locus_condicionis,
            materia_valor_index(valor), MATERIA_LOCUS_INDEX))
    {
        redde NIHIL;
    }
    redde umbra;
}

interior b32
_clausulas_scribere (
                 Piscina* piscina,
            MateriaNodus* sententia,
                 Membrum* membra,
                     i32  n,
                     Xar* clausulae,
    OratioClausulaCensus* census)
{
    i32 c;
    i32 k;

    per (c = ZEPHYRUM; c < xar_numerus(clausulae); c++)
    {
        constans Clausula* cl = (constans Clausula*)xar_obtinere(
            clausulae, c);
            MateriaNodus* nodus = materia_nodus_creare(piscina,
                (s32)ORATIO_GENUS_CLAUSULA,
                ORATIO_REGISTRUM.genera[ORATIO_GENUS_CLAUSULA].loci_numerus);
            MateriaNodus* verbum;
            MateriaNodus* subiectum;

        si (nodus == NIHIL)
        {
            redde FALSUM;
        }
        si (   cl->semen >= ZEPHYRUM
            && !materia_nodus_ponere(nodus, (i32)ORATIO_CLAUSULA_SEMEN,
                materia_valor_index(cl->semen), MATERIA_LOCUS_INDEX))
        {
            redde FALSUM;
        }
        si (!materia_nodus_ponere(nodus, (i32)ORATIO_CLAUSULA_SPECIES,
                materia_valor_index(cl->species), MATERIA_LOCUS_INDEX))
        {
            redde FALSUM;
        }
        si (   cl->pater >= ZEPHYRUM
            && !materia_nodus_ponere(nodus, (i32)ORATIO_CLAUSULA_PATER,
                materia_valor_index(cl->pater), MATERIA_LOCUS_INDEX))
        {
            redde FALSUM;
        }
        verbum = _umbram_struere(piscina,
            (s32)ORATIO_RELATIO_VERBUM_FINITUM,
            (i32)ORATIO_UMBRA_FORMA_VERBI,
            (s32)ORATIO_FORMA_VERBI_FINITUM);
        subiectum = _umbram_struere(piscina,
            (s32)ORATIO_RELATIO_SUBIECTUM,
            (i32)ORATIO_UMBRA_CASUS, (s32)ORATIO_CASUS_NOMINATIVUS);
                si (   verbum == NIHIL || subiectum == NIHIL
                    || !materia_nodus_appendere(piscina, nodus,
                    (i32)ORATIO_CLAUSULA_UMBRAE,
                    materia_valor_nodus(verbum),
                    MATERIA_LOCUS_LISTA_NODUS)
                    || !materia_nodus_appendere(piscina, nodus,
                    (i32)ORATIO_CLAUSULA_UMBRAE,
                    materia_valor_nodus(subiectum),
                    MATERIA_LOCUS_LISTA_NODUS)
                    || !materia_nodus_appendere(piscina, sententia,
                    (i32)ORATIO_SENTENTIA_CLAUSULAE,
                    materia_valor_nodus(nodus),
                    MATERIA_LOCUS_LISTA_NODUS))
                {
            redde FALSUM;
                }
        /* patres figendi (ordinalis 'n' proiectionis et socius
         * oraculi per patrem): appendere patrem non ponit */
        nodus->pater      = sententia;
        verbum->pater     = nodus;
        subiectum->pater  = nodus;
        si (census != NIHIL)
        {
            census->clausulae = census->clausulae + I;
            si (   cl->species >= ZEPHYRUM
                && cl->species < (s32)ORATIO_SPECIES_CLAUSULAE_NUMERUS)
            {
                census->species[cl->species] =
                    census->species[cl->species] + I;
            }
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
                MateriaNodus* nodus = membra[k].nodus;
                         s32  locus = oratio_locus_clausulae(
                             (OratioGenus)nodus->genus, FALSUM);
                 s32 locus_causae = oratio_locus_clausulae(
                     (OratioGenus)nodus->genus, VERUM);

        si (membra[k].clausula < ZEPHYRUM || locus < ZEPHYRUM)
        {
            si (census != NIHIL)
            {
                census->apertae = census->apertae + I;
            }
            perge;
        }
        si (   !materia_nodus_ponere(nodus, (i32)locus,
                materia_valor_index(membra[k].clausula),
                MATERIA_LOCUS_INDEX)
            || !materia_nodus_ponere(nodus, (i32)locus_causae,
                materia_valor_index(membra[k].causa),
                MATERIA_LOCUS_INDEX))
        {
            redde FALSUM;
        }
        si (   census != NIHIL && membra[k].causa >= ZEPHYRUM
            && membra[k].causa < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS)
        {
            census->causae[membra[k].causa] =
                census->causae[membra[k].causa] + I;
        }
    }
    redde VERUM;
}

interior b32
_sententiam_seminare (
                 Piscina* piscina,
            MateriaNodus* sententia,
    OratioClausulaCensus* census)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                 Piscina* scratch;
                 Membrum* membra;
                     Xar* clausulae;
                     i32  n;
                     i32  k;
                     b32  bene;

    si (sententia->loci[ORATIO_SENTENTIA_CLAUSULAE].genus
        != MATERIA_VALOR_NIHIL)
    {
        redde VERUM;   /* iam stampata (idempotens) */
    }
    si (   elementa->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*elementa) == ZEPHYRUM)
    {
        redde VERUM;
    }
    n        = materia_valor_lista_numerus(*elementa);
    scratch  = piscina_generare_dynamicum("oratio_clausula_sententia",
        1048576);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    membra = (Membrum*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(Membrum));
    clausulae = xar_creare(scratch, (i32)magnitudo(Clausula));
    si (membra == NIHIL || clausulae == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(*elementa,
            k);

        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            piscina_destruere(scratch);
            redde VERUM;   /* sententia aliena: nihil */
        }
        _membrum_describere(v->datum.nodus, &membra[k]);
    }
    _semina_iudicare(membra, n, census);
    bene = _extentum_stampare(scratch, membra, n, clausulae, census)
        && _clausulas_scribere(piscina, sententia, membra, n, clausulae,
            census);
    si (bene && census != NIHIL)
    {
        census->sententiae = census->sententiae + I;
    }
    piscina_destruere(scratch);
    redde bene;
}

vacuum
oratio_clausula_census_vacare (
    OratioClausulaCensus* census)
{
    memset(census, ZEPHYRUM, magnitudo(*census));
}

b32
oratio_clausulas_seminare (
                 Piscina* piscina,
            MateriaNodus* radix,
    OratioClausulaCensus* census)
{
    constans MateriaValor* lista;
                      i32  n;
                      i32  k;

    si (radix == NIHIL)
    {
        redde VERUM;
    }
    si (radix->genus == (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde _sententiam_seminare(piscina, radix, census);
    }
    si (radix->genus == (s32)ORATIO_GENUS_DOCUMENTUM)
    {
        lista = &radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    }
    alioquin si (radix->genus == (s32)ORATIO_GENUS_PARAGRAPHUS)
    {
        lista = &radix->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    }
    alioquin
    {
        redde VERUM;
    }
    si (lista->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(*lista);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*lista,
            k);

        si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
            && !oratio_clausulas_seminare(piscina, e->datum.nodus,
                census))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
