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

/* SCISSIO verbi (stratum V): verba inter verbum finitum prius et
 * alterum ad clausulam NOVAM (VERUM: ordo verbo finali - verba verbum
 * suum praecedunt) aut ad priorem (FALSUM: scissio ante verbum
 * alterum ipsum). Utraque mensurata 2026-09-08 (vide worklog). */
hic_manens constans b32 SCISSIO_ANTE_VERBA_MEDIA = VERUM;

/* VARIATIONES clausurae (T20a ter, 2026-09-08; ex indice erratorum
 * chartarum cum contextu): quaeque sola mensurata CONCORDIA PARIUM
 * (Rand; puritas sola segmentationem nimiam praemiat), deinde omnes.
 * Basis CIRCSE / LLCT dev / test 79.9 / 76.9 / 78.1.
 * QUI_SUPRA: 'qui supra' formula chartarum (= suprascriptus, sine
 * verbo, UD in clausula principali) relativum non seminat: = / +0.5
 * / +0.2.
 * COPULA_NON_CLAUDIT: verbum certum lemmate 'sum' clausulam
 * clausibilem sine verbo suo non claudit - praedicatum copulam
 * sequitur (qui fuit quondam Filippi), radix UD praedicatum: +0.2 /
 * +1.0 / +0.2.
 * COORDINANS_ANTE_SEMEN: coordinans non seminans quem semen statim
 * sequitur clausulae seminatae accedit (ut census limites definit):
 * = / +0.1 / +0.3.
 * CERTITUDO_LATINA: verbum CERTUM = lectiones LATINAE omnes finitae
 * (lectio Anglica Moby - tenet substantivum Anglicum - in sententia
 * Latina non suffragatur): +1.9 / +1.0 / +0.5.
 * OMNES: +2.1 / +4.3 / +1.8 (superadditivae chartis). Mensurata et
 * ABLATA: ubi/unde initio sententiae non seminans (unde conectivum,
 * census initium XV/XXV): sola -0.2 dev, in combinatione nihil. */
hic_manens constans b32 QUI_SUPRA_NON_SEMINAT  = VERUM;
hic_manens constans b32 COPULA_NON_CLAUDIT     = VERUM;
hic_manens constans b32 COORDINANS_ANTE_SEMEN  = VERUM;
hic_manens constans b32 CERTITUDO_LATINA       = VERUM;

/* VARIATIONES scissionis et clausurae II (T20a quater, 2026-09-08; ex
 * sententiis ostensis causae 'verbum' chartarum, -sententiae): quaeque
 * sola mensurata concordia (basis 82.0 / 81.2 / 79.9), deinde paria.
 * ID_EST: 'id est' formula fixa - est post id nec scindit nec claudit
 * (dedisti mihi id est res mea: obiectum post formulam pergit):
 * = / +0.4 / +0.2.
 * NOMEN_POST_VERBUM: clausura ad verbum differtur dum nomen proprium
 * sequitur (ubi vocitatur Iuveiano, qui dicitur Creta): = / +0.1 /
 * +0.1. Ambo: = / +0.5 / +0.4.
 * Mensurata et ABLATA: comma clausulam clausibilem claudens solum cum
 * verbo suo (comma appositivum: quia tu Gherardus, gratia Dei ...
 * episcopus, ... dedisti) = / -0.1 / +0.8 sola, cum ambobus -0.6 dev;
 * ambulatio coordinantis commata transiens (regitur per Urso, et
 * ille secunda regitur) = / +0.2 / -0.9. Seneca ab omnibus immota
 * (formulae chartarum). */
hic_manens constans b32 ID_EST             = VERUM;
hic_manens constans b32 NOMEN_POST_VERBUM  = VERUM;

/* VARIATIONES III (T20a quinquies, ex visu causae 'extentum'
 * chartarum; basis concordiae 82.0 / 81.7 / 80.3):
 * UNA_CUM: cum post una/simul (una cum omnibus rebus) adpositio -
 * CXII casus aurei chartarum, limes numquam - non seminat (sola
 * = / +0.3 / -0.1, cum ceteris nihil deprimit).
 * CUM_SUBIUNCTIVUM: cum corroboratur verbo finito CERTO aut capaci
 * cuius lectio finita subiunctiva est (cum venisset Senecae:
 * homographum subiunctivum) - chartis cum adpositio XCVIII %, verbum
 * capax quodvis homographum est. Mensurata et ABLATA: corroboratio
 * verbo CERTO solo (-0.1 Senecae, +1.0 / +0.9 chartis).
 * quantus in ORATIO_SEMINA_RELATIVA (quantum ... est pertenentes):
 * = / +0.2 / +0.3. Omnes tres: = / +0.9 / +0.7. */
hic_manens constans b32 UNA_CUM           = VERUM;
hic_manens constans b32 CUM_SUBIUNCTIVUM  = VERUM;
/* CUM_ABLATIVO: exigentia certi/subiunctivi solum cum verbum proximum
 * post cum lectionem ablativam fert (cum patre, cum omnibus rebus);
 * aliter (cum venit, cum ille ...) verbum capax quodvis corroborat ut
 * basis - Senecae XXXIX paria reddit. */
hic_manens constans b32 CUM_ABLATIVO      = VERUM;
/* CUM_ABLATIVO_SOLO (ex censu legis II clausulae, 2026-09-08): cum
 * cuius verbum proximum lectionem ablativam fert et NULLAM nominativam
 * nec ipsum verbum finitum capax est adpositio est et numquam seminat
 * (cum uno lato tenet, cum eo legiones; cum ipse domnus seminat).
 * Census seminum falsorum cum: chartae XII / XXIX, Seneca VII ->
 * III / XV, III, sed SCONJ IV Senecae amissa. MENSURATUM paribus
 * concordiae: Seneca +CLVII, chartae test +MXXI, chartae dev -CCCXII
 * (439845 -> 439533) - thesaurum deprimit, ABLATUM (numeri manent).
 * Lex II ipsa (cum seminans -> lectio coniunctionis prima) censu
 * REFUTATA utraque stampa: Seneca +XVII/-VII (+XIII/-III), chartae
 * -XI / -XXVII (-II / -XIV): decisio seminis chartis adpositionem
 * seminat; custodia sine capsa (verbum capax sequitur) Senecae
 * paene aequalis, chartis -CIII / -CXVI. */
hic_manens constans b32 CUM_ABLATIVO_SOLO = FALSUM;

/* STRATUM IV CATENA (T20b, 2026-09-08): MENSURATUM concordia parium
 * exacta (CIRCSE / LLCT dev / test, basis 102551 / 438519 / 444139):
 * vicini strata debilia -33 / +17 / -6 (mota V / I / II); vicini
 * quodvis -39 / +17 / -6; quaevis distantia debilia -1142 / -7363 /
 * -4926 (mota CCLXV / DCXVII / CDXCV); quaevis omnia -1203 / -7419 /
 * -4980. Ligationes vicinae limitem fere numquam transeunt (extenta
 * contigua), remotae saepe (discordiae DXLVII / MXVI / CMLVI) et eas
 * sequi ubique nocet: catena extentum NON superat - discordia
 * inventum de LIGATIONE est, non de clausula (lex III T20c: caput
 * laxum intra capsam). Numeratio sola manet.
 * CATENA_DISTANTIA: distantia maxima socii ut verbum moveatur (0 =
 * numquam, discordiae solum numerantur; I = vicini soli; magna =
 * quaevis). CATENA_DEBILES_SOLAE: verba stratorum clausura/verbum
 * sola moventur (VERUM) aut quodvis debilius socio (FALSUM). */

hic_manens constans i32 CATENA_DISTANTIA = ZEPHYRUM;
hic_manens constans b32 CATENA_DEBILES_SOLAE = VERUM;
/* robur strati per puritatem mensuratam (semen 943-967, unica
 * 920-999, extentum 900-943, verbum 712-763, clausura 659-738);
 * index = OratioClausulaCausa; catena numquam iterum movetur */
hic_manens constans i32 ROBUR_CAUSAE[ORATIO_CLAUSULA_CAUSA_NUMERUS] = {
    (i32)5,   /* semen */
    (i32)3,   /* extentum */
    (i32)1,   /* clausura */
    ZEPHYRUM, /* catena */
    (i32)4,   /* unica */
    (i32)2    /* verbum */
};


/* formae relativae: lemma lectionis pronominis aut determinantis */
constans character* constans ORATIO_SEMINA_RELATIVA[] = {
    "qui", "quicumque", "quisquis",
    "quantus",   /* T20a quinquies: quantum ... est pertenentes */
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
             b32  subiunctivum;   /* lectio finita subiunctiva ulla */

                                   b32  seminat;        /* post corroborationem */
                          b32  copula_certa;   /* certum et lemmate 'sum' (copula) */
                                       b32  supra;          /* lectio lemmate 'supra' (qui supra) */
                          b32  id;             /* forma 'id' (id est) */
                          b32  cum;            /* lectio lemmate 'cum' */
                          b32  ablativus;      /* lectio Latina casu ablativo ulla */
             b32  nominativus;    /* lectio Latina casu nominativo ulla (T24) */

             b32 una_simul;      /* forma una | simul (una cum) */

             b32 nomen_proprium; /* lectio nominis proprii ulla */
             b32 differtur;      /* clausura post hoc verbum differtur */


             s32 clausula;       /* -I = aperta */
             s32 causa;
} Membrum;

nomen structura {
    s32 species;
    s32 semen;    /* ordinalis elementi; -I */
    s32 pater;    /* -I = summa */
        i32 membra;
    s32 verbum;        /* ordinalis verbi finiti certi sui; -I nondum */
    b32 clausibilis;   /* verbo finito certo aut signo clauditur
                        * (subordinata, relativa, soror earum);
                        * principalis numquam, parenthetica signo
                        * suo solo */
} Clausula;

interior vacuum
_membrum_describere (
    MateriaNodus* nodus,
         Membrum* m,
             b32  latina)
{
        constans MateriaValor* analyses;
                          i32  n;
                          i32  a;
                          i32  finitae     = ZEPHYRUM;
                          i32  auxiliares  = ZEPHYRUM;
            hic_manens constans character* constans SUPRA[] = { "supra",
                NIHIL };
        hic_manens constans character* constans ESSE[] = { "sum",
            NIHIL };
    hic_manens constans character* constans CUM[] = { "cum", NIHIL };

                      i32 latinae  = ZEPHYRUM;
                      b32 copula   = FALSUM;


    m->nodus                 = nodus;
    m->semen                 = (s32)SEMEN_NULLUM;
            m->finita_capax  = FALSUM;
    m->finita_certa          = FALSUM;
    m->subiunctivum          = FALSUM;
    m->seminat               = FALSUM;

    m->copula_certa   = FALSUM;
            m->supra  = FALSUM;
        m->id         = FALSUM;
        m->cum        = FALSUM;
        m->ablativus  = FALSUM;
    m->nominativus    = FALSUM;

    m->una_simul      = FALSUM;

    m->nomen_proprium  = FALSUM;
    m->differtur       = FALSUM;


    m->clausula  = (s32)-I;
    m->causa     = (s32)-I;
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
        si (nodus->genus != (s32)ORATIO_GENUS_VOCABULUM || !latina)
        {
        redde;   /* documentum non Latinum: nec semen nec verbum */
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
            si (_accidens(lectio, "modus")
                == (s32)ORATIO_MODUS_SUBIUNCTIVUS)
            {
                m->subiunctivum = VERUM;
            }

            si (oratio_genus_classis((OratioGenus)lectio->genus)
                == ORATIO_CLASSIS_AUXILIARE)
            {
                auxiliares = auxiliares + I;
            }
                }
        si (!_latina(lectio))
        {
            perge;
        }
                latinae = latinae + I;
        classis = oratio_genus_classis((OratioGenus)lectio->genus);
        si (classis == ORATIO_CLASSIS_NOMEN_PROPRIUM)
        {
            m->nomen_proprium = VERUM;
        }

        lemma    = _lemma(lectio);
        si (lemma.mensura == ZEPHYRUM)
        {
            perge;
        }
                si (_in_lista(ESSE, lemma))
                {
            copula = VERUM;
                }
                si (_in_lista(CUM, lemma))
                {
            m->cum = VERUM;
                }
                si (_accidens(lectio, "casus")
                    == (s32)ORATIO_CASUS_ABLATIVUS)
                {
            m->ablativus = VERUM;
                }
        si (_accidens(lectio, "casus") == (s32)ORATIO_CASUS_NOMINATIVUS)
        {
            m->nominativus = VERUM;
        }


                si (_in_lista(SUPRA, lemma))
                {
            m->supra = VERUM;
                }
        /* prioritas: certum > relativum > corroborandum > coordinans */
        si (_in_lista(ORATIO_SEMINA_CERTA, lemma))
        {
            m->semen = (s32)SEMEN_CERTUM;
        }
                        alioquin si (   (   classis
                                     == ORATIO_CLASSIS_PRONOMEN
                                     || classis
                                         == ORATIO_CLASSIS_DETERMINANS)
                                     && _in_lista(ORATIO_SEMINA_RELATIVA,
                                                          lemma)
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
                    m->id         = _forma_est(nodus, "id");
    m->una_simul  = (b32)(_forma_est(nodus, "una")
        || _forma_est(nodus, "simul"));

    si (CERTITUDO_LATINA)
    {
        m->finita_certa = (b32)(latinae > ZEPHYRUM
            && finitae == latinae);
    }
    alioquin
    {
        m->finita_certa = (b32)(n > ZEPHYRUM && finitae == n);
    }
    m->copula_certa  = (b32)(m->finita_certa && copula);
    (vacuum)auxiliares;
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

        /* 'id est': est post id formula fixa - nec certum nec capax */
        si (   ID_EST && k > ZEPHYRUM && membra[k - I].id
            && m->copula_certa)
        {
            m->finita_certa = FALSUM;
            m->finita_capax = FALSUM;
            m->copula_certa = FALSUM;
        }
        si (   QUI_SUPRA_NON_SEMINAT && m->semen == (s32)SEMEN_RELATIVUM
            && k + I < n && membra[k + I].supra)
        {
            /* 'qui supra': formula chartarum sine verbo */
            m->semen = (s32)SEMEN_NULLUM;
            perge;
        }
        si (   m->semen == (s32)SEMEN_CERTUM
            || m->semen == (s32)SEMEN_RELATIVUM)
        {
            m->seminat = VERUM;
            perge;
        }

                si (   UNA_CUM && m->semen == (s32)SEMEN_CORROBORANDUM
                    && m->cum
                    && k > ZEPHYRUM && membra[k - I].una_simul)
                {
            m->semen = (s32)SEMEN_NULLUM;   /* una cum: adpositio */
            perge;
                }
                        si (   CUM_ABLATIVO_SOLO
                            && m->semen == (s32)SEMEN_CORROBORANDUM
                            && m->cum && k + I < n
                            && membra[k + I].ablativus
                            && !membra[k + I].nominativus
                            && !membra[k + I].finita_capax)
                        {
            m->semen = (s32)SEMEN_NULLUM;   /* cum + ablativo solo: adpositio */
            perge;
                        }
                si (m->semen == (s32)SEMEN_CORROBORANDUM)
                {
            b32 exigens = (b32)(m->cum && CUM_SUBIUNCTIVUM
                && (   !CUM_ABLATIVO
                    || (k + I < n && membra[k + I].ablativus)));

            per (j = k + I; j < n
                && !_terminus_corroborationis(&membra[j]);
                 j++)
            {
                si (   membra[j].finita_capax
                    && (   !exigens
                        || membra[j].finita_certa
                        || membra[j].subiunctivum))
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
                                                per (h = (s32)k
                                                    - (s32)I; h
                                                    >= ZEPHYRUM
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
                            && membra[j].semen
                                == (s32)SEMEN_NULLUM; j++)
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
        c->membra   = ZEPHYRUM;
    c->verbum       = (s32)-I;
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
    i32 altitudo            = ZEPHYRUM;
    s32 summa               = (s32)-I;   /* clausula principalis; -I nondum */
        b32 post_clausuram  = FALSUM;
    b32 post_verbum         = FALSUM;   /* post scissionem verbi */
        b32 semen_ullum     = FALSUM;
    b32 scissa              = FALSUM;
    s32 pendens             = (s32)-I;   /* coordinans ante semen */
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
                        m->clausula  = nova;
            m->causa                 = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram           = FALSUM;
            post_verbum              = FALSUM;
        }
                alioquin si (m->seminat)
        {
            s32 species = m->semen == (s32)SEMEN_RELATIVUM
                ? (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA
                : (s32)ORATIO_SPECIES_CLAUSULAE_SUBORDINATA;
            s32 vetus_top = top;

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
                                    acervus[altitudo] = nova;
            altitudo = altitudo + I;
            m->clausula = nova;
            m->causa = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram = FALSUM;
            post_verbum = FALSUM;
            /* coordinans pendens ante hoc semen: clausulae seminatae */
            si (   COORDINANS_ANTE_SEMEN && pendens == (s32)k - (s32)I
                && vetus_top                >= ZEPHYRUM
                && membra[pendens].clausula == vetus_top)
            {
                membra[pendens].clausula = nova;
                ((Clausula*)xar_obtinere(clausulae, (i32)vetus_top))
                    ->membra = ((Clausula*)xar_obtinere(clausulae,
                        (i32)vetus_top))->membra - I;
                ((Clausula*)xar_obtinere(clausulae, (i32)nova))->membra
                    = ((Clausula*)xar_obtinere(clausulae,
                        (i32)nova))->membra + I;
            }
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
                        acervus[altitudo] = nova;
            altitudo = altitudo + I;
            m->clausula = nova;
            m->causa = (s32)ORATIO_CLAUSULA_CAUSA_SEMEN;
            post_clausuram = FALSUM;
            post_verbum = FALSUM;
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
            m->causa     = post_verbum
                ? (s32)ORATIO_CLAUSULA_CAUSA_VERBUM
                : post_clausuram
                ? (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA
                : (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM;
                                    si (   altitudo > ZEPHYRUM
                                        && ((Clausula*)xar_obtinere(clausulae,
                                        (i32)top))->clausibilis)
                                    {
                altitudo        = altitudo - I;
                post_clausuram  = VERUM;
                post_verbum     = FALSUM;
                si (census != NIHIL)
                {
                    census->clausae_signo = census->clausae_signo + I;
                }
                                    }
        }
                alioquin
        {
            Clausula* cl = (Clausula*)xar_obtinere(clausulae, (i32)top);

            m->clausula  = top;
            m->causa     = post_verbum
                ? (s32)ORATIO_CLAUSULA_CAUSA_VERBUM
                : post_clausuram
                ? (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA
                : (s32)ORATIO_CLAUSULA_CAUSA_EXTENTUM;
                        si (   m->semen == (s32)SEMEN_COORDINANS
                            && !m->seminat)
                        {
                pendens = (s32)k;
                        }
                        si (   (   m->finita_certa && cl->clausibilis
                            && !(COPULA_NON_CLAUDIT && m->copula_certa
                            && cl->verbum < ZEPHYRUM))
                            || (k > ZEPHYRUM
                                && membra[k - I].differtur))
                        {
                /* stratum III: verbum finitum certum clausulam
                 * clausibilem claudit (prior verbi finalis); nomen
                 * proprium sequens clausuram differt (ubi vocitatur
                 * Iuveiano) */
                si (   m->finita_certa && !(k > ZEPHYRUM
                        && membra[k - I].differtur))
                {
                    cl->verbum = (s32)k;
                }
                si (   NOMEN_POST_VERBUM && k + I < n
                    && membra[k + I].nomen_proprium
                    && membra[k + I].semen == (s32)SEMEN_NULLUM
                    && !membra[k + I].finita_certa)
                {
                    m->differtur = VERUM;   /* claudetur post nomen */
                }
                alioquin
                {
                    altitudo        = altitudo - I;
                    post_clausuram  = VERUM;
                    post_verbum     = FALSUM;
                    si (census != NIHIL)
                    {
                        census->clausae_verbo = census->clausae_verbo
                            + I;
                    }
                }
                        }
            alioquin si (m->finita_certa && cl->verbum < ZEPHYRUM)
                        {
                cl->verbum = (s32)k;   /* verbum suum */
                        }
            alioquin si (m->finita_certa)
                        {
                /* stratum V: SCISSIO - verbum finitum certum alterum in
                 * clausula quae suum iam habet: semen absens
                 * (asyndeton); clausula coordinata nova sorore
                 * substituente, verba media ad novam (variatio) */
                s32 nova = _clausulam_addere(clausulae,
                    (s32)ORATIO_SPECIES_CLAUSULAE_COORDINATA, (s32)k,
                    cl->pater);
                Clausula* cn;

                si (nova < ZEPHYRUM)
                {
                    redde FALSUM;
                }
                cl = (Clausula*)xar_obtinere(clausulae, (i32)top);
                cn = (Clausula*)xar_obtinere(clausulae, (i32)nova);
                cn->clausibilis = cl->clausibilis;
                cn->verbum = (s32)k;
                si (altitudo > ZEPHYRUM)
                {
                    acervus[altitudo - I] = nova;
                }
                si (top == summa)
                {
                    summa = nova;
                }
                si (SCISSIO_ANTE_VERBA_MEDIA)
                {
                    i32 j;

                    per (j = (i32)cl->verbum + I; j < k; j++)
                    {
                        si (membra[j].clausula == top)
                        {
                            membra[j].clausula  = nova;
                            membra[j].causa     =
                                (s32)ORATIO_CLAUSULA_CAUSA_VERBUM;
                            cl->membra = cl->membra - I;
                            cn->membra = cn->membra + I;
                        }
                    }
                }
                m->clausula     = nova;
                m->causa        = (s32)ORATIO_CLAUSULA_CAUSA_VERBUM;
                post_verbum     = VERUM;
                post_clausuram  = FALSUM;
                scissa          = VERUM;
                si (census != NIHIL)
                {
                    census->scissae = census->scissae + I;
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
        /* sententia sine semine et sine scissione: clausula una, causa
     * UNICA ubique (stratum suum in oraculo) */
    si (!semen_ullum && !scissa)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (membra[k].clausula >= ZEPHYRUM)
            {
                membra[k].causa = (s32)ORATIO_CLAUSULA_CAUSA_UNICA;
            }
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
                     b32  latina,
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
                _membrum_describere(v->datum.nodus, &membra[k], latina);
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

/* umbrae lectionis PRIMAE vocabuli: ordinales impletionum (w) in
 * exitus (usque ad maximum); numerus */
interior i32
_impletiones_lectionis_primae (
    constans MateriaNodus* vocabulum,
                      s32* exitus,
                      i32  maximum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaValor* prima;
    constans MateriaNodus* lectio;
            OratioClassis  classis;
                      s32  locus;
    constans MateriaValor* umbrae;
                      i32  n;
                      i32  u;
                      i32  numerus = ZEPHYRUM;

    si (   analyses->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*analyses) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    prima = materia_valor_lista_obtinere(*analyses, ZEPHYRUM);
    si (prima == NIHIL || prima->genus != MATERIA_VALOR_NODUS)
    {
        redde ZEPHYRUM;
    }
    lectio   = prima->datum.nodus;
    classis  = oratio_genus_classis((OratioGenus)lectio->genus);
    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde ZEPHYRUM;
    }
    locus = oratio_partes_locus(classis, "umbrae");
    si (   locus < ZEPHYRUM || (i32)locus >= lectio->numerus_locorum
        || lectio->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    umbrae  = &lectio->loci[locus];
    n       = materia_valor_lista_numerus(*umbrae);
    per (u = ZEPHYRUM; u < n && numerus < maximum; u++)
    {
        constans MateriaValor* v = materia_valor_lista_obtinere(*umbrae,
            u);
        constans MateriaValor* w;

        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        w = &v->datum.nodus->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM];
        si (   w->genus       == MATERIA_VALOR_INDEX
            && w->datum.index >= ZEPHYRUM)
        {
            exitus[numerus]  = w->datum.index;
            numerus          = numerus + I;
        }
    }
    redde numerus;
}

/* an elementum semen clausulae relativae sit (ligatio ad antecedens
 * limitem iure transit) */
interior b32
_relativum_semen (
    constans MateriaNodus* sententia,
                      s32  clausula,
                      s32  causa,
                      i32  k)
{
    constans MateriaValor* clausulae =
        &sententia->loci[ORATIO_SENTENTIA_CLAUSULAE];
    constans MateriaValor* v;
    constans MateriaNodus* nodus;

    si (   causa            != (s32)ORATIO_CLAUSULA_CAUSA_SEMEN
        || clausula < ZEPHYRUM
        || clausulae->genus != MATERIA_VALOR_LISTA
        || (i32)clausula    >= materia_valor_lista_numerus(*clausulae))
    {
        redde FALSUM;
    }
    v = materia_valor_lista_obtinere(*clausulae, (i32)clausula);
    si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
    {
        redde FALSUM;
    }
    nodus = v->datum.nodus;
    redde (b32)(   nodus->loci[ORATIO_CLAUSULA_SPECIES].genus
                    == MATERIA_VALOR_INDEX
                && nodus->loci[ORATIO_CLAUSULA_SPECIES].datum.index
                    == (s32)ORATIO_SPECIES_CLAUSULAE_RELATIVA
                && nodus->loci[ORATIO_CLAUSULA_SEMEN].genus
                    == MATERIA_VALOR_INDEX
                && nodus->loci[ORATIO_CLAUSULA_SEMEN].datum.index
                    == (s32)k);
}

#define IMPLETIONES_MAXIMAE 8

interior b32
_sententiam_propagare (
                 Piscina* piscina,
            MateriaNodus* sententia,
    OratioClausulaCensus* census)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                 Piscina* scratch;
                     s32* clausula;
                     s32* causa;
            MateriaNodus** nodi;
                     i32  n;
                     i32  k;

    si (   sententia->loci[ORATIO_SENTENTIA_CLAUSULAE].genus
            == MATERIA_VALOR_NIHIL
        || elementa->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;   /* non stampata */
    }
    n = materia_valor_lista_numerus(*elementa);
    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    scratch = piscina_generare_dynamicum("oratio_clausula_catena",
        262144);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    clausula = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    causa    = (s32*)piscina_allocare(scratch, (memoriae_index)n
        * (memoriae_index)magnitudo(s32));
    nodi     = (MateriaNodus**)piscina_allocare(scratch,
        (memoriae_index)n * (memoriae_index)magnitudo(MateriaNodus*));
    si (clausula == NIHIL || causa == NIHIL || nodi == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(*elementa,
            k);
        s32 locus;
        s32 locus_causae;

        clausula[k]  = (s32)-I;
        causa[k]     = (s32)-I;
        nodi[k]      = NIHIL;
        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        nodi[k]       = v->datum.nodus;
        locus         =
            oratio_locus_clausulae((OratioGenus)nodi[k]->genus,
            FALSUM);
        locus_causae  =
            oratio_locus_clausulae((OratioGenus)nodi[k]->genus,
            VERUM);
        si (   locus                      >= ZEPHYRUM
            && nodi[k]->loci[locus].genus == MATERIA_VALOR_INDEX)
        {
            clausula[k] = nodi[k]->loci[locus].datum.index;
        }
        si (   locus_causae                      >= ZEPHYRUM
            && nodi[k]->loci[locus_causae].genus == MATERIA_VALOR_INDEX)
        {
            causa[k] = nodi[k]->loci[locus_causae].datum.index;
        }
    }
    /* margines: lectio prima v -> socius w */
    per (k = ZEPHYRUM; k < n; k++)
    {
        s32 socii[IMPLETIONES_MAXIMAE];
        i32 numerus;
        i32 s;

        si (   nodi[k]        == NIHIL
            || nodi[k]->genus != (s32)ORATIO_GENUS_VOCABULUM
            || clausula[k] < ZEPHYRUM)
        {
            perge;
        }
        numerus = _impletiones_lectionis_primae(nodi[k], socii,
            (i32)IMPLETIONES_MAXIMAE);
        per (s = ZEPHYRUM; s < numerus; s++)
        {
            i32 w = (i32)socii[s];
            i32 distantia;
            i32 debile;
            i32 forte;

            si (   w           >= n || clausula[w] < ZEPHYRUM
                || clausula[w] == clausula[k])
            {
                perge;
            }
            /* relativum ad antecedens: limes iure */
            si (   _relativum_semen(sententia, clausula[k], causa[k], k)
                || _relativum_semen(sententia, clausula[w], causa[w],
                w))
            {
                perge;
            }
            si (census != NIHIL)
            {
                census->discordiae = census->discordiae + I;
            }
            distantia = w > k ? w - k : k - w;
            si (   CATENA_DISTANTIA == ZEPHYRUM
                || distantia > (i32)CATENA_DISTANTIA)
            {
                perge;
            }
            /* debilius movetur ad fortius; paria non moventur */
            {
                i32 robur_k = causa[k] >= ZEPHYRUM
                    && causa[k] < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                    ? ROBUR_CAUSAE[causa[k]] : ZEPHYRUM;
                i32 robur_w = causa[w] >= ZEPHYRUM
                    && causa[w] < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                    ? ROBUR_CAUSAE[causa[w]] : ZEPHYRUM;

                si (   robur_k == robur_w || robur_k == ZEPHYRUM
                    || robur_w == ZEPHYRUM)
                {
                    perge;
                }
                debile  = robur_k < robur_w ? k : w;
                forte   = robur_k < robur_w ? w : k;
            }
            si (   CATENA_DEBILES_SOLAE
                && causa[debile] != (s32)ORATIO_CLAUSULA_CAUSA_CLAUSURA
                && causa[debile] != (s32)ORATIO_CLAUSULA_CAUSA_VERBUM)
            {
                perge;
            }
            {
                s32 locus = oratio_locus_clausulae(
                    (OratioGenus)nodi[debile]->genus, FALSUM);
                s32 locus_causae = oratio_locus_clausulae(
                    (OratioGenus)nodi[debile]->genus, VERUM);

                si (   !materia_nodus_reponere(nodi[debile], (i32)locus,
                        materia_valor_index(clausula[forte]),
                        MATERIA_LOCUS_INDEX)
                    || !materia_nodus_reponere(nodi[debile],
                        (i32)locus_causae,
                        materia_valor_index(
                            (s32)ORATIO_CLAUSULA_CAUSA_CATENA),
                        MATERIA_LOCUS_INDEX))
                {
                    piscina_destruere(scratch);
                    redde FALSUM;
                }
                clausula[debile]  = clausula[forte];
                causa[debile]     = (s32)ORATIO_CLAUSULA_CAUSA_CATENA;
                si (census != NIHIL)
                {
                    census->catenatae = census->catenatae + I;
                }
            }
        }
    }
    (vacuum)piscina;
    piscina_destruere(scratch);
    redde VERUM;
}

b32
oratio_clausulas_propagare (
                 Piscina* piscina,
            MateriaNodus* radix,
      constans character* lingua,
    OratioClausulaCensus* census)
{
    constans MateriaValor* lista;
                      i32  n;
                      i32  k;

    si (   radix == NIHIL
        || (lingua != NIHIL && strcmp(lingua, "anglica") == ZEPHYRUM))
    {
        redde VERUM;   /* documentum Anglicum: sine catena (unicum) */
    }
    si (radix->genus == (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde _sententiam_propagare(piscina, radix, census);
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
            && !oratio_clausulas_propagare(piscina, e->datum.nodus,
                lingua, census))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
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
      constans character* lingua,
    OratioClausulaCensus* census)
{
    constans MateriaValor* lista;
                      i32  n;
                      i32  k;
                      b32  latina = (b32)(lingua == NIHIL
                          || strcmp(lingua, "anglica") != ZEPHYRUM);

    si (radix == NIHIL)
    {
        redde VERUM;
    }
    si (radix->genus == (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde _sententiam_seminare(piscina, radix, latina, census);
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
                    && !oratio_clausulas_seminare(piscina,
                    e->datum.nodus,
                    lingua, census))
                {
            redde FALSUM;
                }
    }
    redde VERUM;
}
