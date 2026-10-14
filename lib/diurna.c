/*
 * diurna.c - acta diurna: taberna actorum (vide include/diurna.h)
 *
 * Index plicatur (lectio tota + tabula dispersa per clavem) in omni
 * lectione - index brevis manet quia purgatio eum compactat (gradus
 * proximus). Nulla sqlite: corpora plagulae, index TSV.
 */

#include "postulata_posix.h"

#include "latina.h"
#include "diurna.h"
#include "filum.h"
#include "flatura.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

structura Diurna {
    character  radix[DXII];
      Piscina* piscina;
};

/* forma corporis in plicatura (per sigillum) */
nomen structura {
    DiurnaForma forma;
            s64 momentum;
} DiurnaCorpus;

interior s64
_momentum_nunc (vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde (s64)ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * (s64)M + (s64)tv.tv_usec / (s64)M;
}

interior b32
_purum (
    chorda campus)
{
    i32 k;

    per (k = ZEPHYRUM; k < campus.mensura; k++)
    {
        si (   campus.datum[k] == '\t' || campus.datum[k] == '\n'
            || campus.datum[k] == '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* RADIX/corpora/II-hex/LXII-hex + suffixum */
interior vacuum
_via_corporis (
       constans Diurna* d,
    constans character* sigillum,
    constans character* suffixum,
             character* via)
{
    sprintf(via, "%s/corpora/%.2s/%s%s", d->radix, sigillum, sigillum
        + II,
        suffixum);
}

interior vacuum
_via_indicis (
     constans Diurna* d,
           character* via)
{
    sprintf(via, "%s/index.tsv", d->radix);
}

/* sera indicis (scriptores et purgatio): <radix>/index.sera, flock */
#define DIURNA_MORA_SERAE_MS (X * M)

interior FilumSera*
_seram_capere (
    constans Diurna* d)
{
    character via[DXII + XVI];

    sprintf(via, "%s/index.sera", d->radix);
    redde filum_seram_capere(via, (i32)DIURNA_MORA_SERAE_MS,
        d->piscina);
}

Diurna*
diurna_aperire (
    constans character* radix,
               Piscina* piscina)
{
       Diurna* d;
    character  corpora[DXII + XVI];

    si (   radix == NIHIL || piscina == NIHIL || radix[ZEPHYRUM] == '\0'
        || strlen(radix) >= (memoriae_index)CDLX)
    {
        redde NIHIL;
    }
    d = (Diurna*)piscina_allocare(piscina, magnitudo(Diurna));
    si (d == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(d->radix, "%s", radix);
    d->piscina = piscina;
    sprintf(corpora, "%s/corpora", radix);
    si (!filum_directorium_creare_cum_parentibus(corpora))
    {
        redde NIHIL;
    }
    redde d;
}


/* ==================================================
 * PLICATURA INDICIS
 * ================================================== */

/* lineam ad TAB scindere; numerus camporum (maxime 'maximum') */
interior i32
_campi (
    chorda  linea,
    chorda* campi,
       i32  maximum)
{
    i32 n        = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k <= linea.mensura; k++)
    {
        si (k == linea.mensura || linea.datum[k] == '\t')
        {
            si (n >= maximum)
            {
                redde maximum + I;
            }
            campi[n].datum    = linea.datum + initium;
            campi[n].mensura  = k - initium;
            n++;
            initium = k + I;
        }
    }
    redde n;
}

interior s64
_numerus (
    chorda campus)
{
    character tabula[XXXII];

    si (campus.mensura <= ZEPHYRUM || campus.mensura >= (i32)XXXII)
    {
        redde (s64)ZEPHYRUM;
    }
    memcpy(tabula, campus.datum, (size_t)campus.mensura);
    tabula[campus.mensura] = '\0';
    /* longus LXIV bitorum (LP64) */
    redde (s64)strtol(tabula, NIHIL, X);
}

/* clavis tabulae: clavis + '\x1f' + seq */
interior chorda
_clavis_seq (
     chorda  clavis,
        i32  seq,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(clavis.mensura + XVI));
            character numerus[XVI];

    chorda_aedificator_appendere_chorda(a, clavis);
    sprintf(numerus, "\x1f%d", (integer)seq);
    chorda_aedificator_appendere_literis(a, numerus);
    redde chorda_aedificator_finire(a);
}

/* index totus -> Xar introituum (ordine positionis primae); claves
 * tabula per (clavis, seq) in indicem Xar */
interior Xar*
_plicare (
    constans Diurna* d,
            Piscina* piscina)
{
          character  via[DXII + XVI];
             chorda  textus;
                Xar* introitus;
     TabulaDispersa* per_clavem;
     TabulaDispersa* per_corpus;
                i32  initium = ZEPHYRUM;
                i32  k;

    introitus   = xar_creare(piscina, (i32)magnitudo(DiurnaIntroitus));
    per_clavem  = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    per_corpus  = tabula_dispersa_creare_chorda(piscina, (i32)CXXVIII);
    si (   introitus  == NIHIL || per_clavem == NIHIL
        || per_corpus == NIHIL)
    {
        redde NIHIL;
    }
    _via_indicis(d, via);
    textus = filum_legere_totum(via, piscina);
    per (k = ZEPHYRUM; k <= textus.mensura; k++)
    {
        chorda linea;
        chorda campi[X];
           i32 n;

        si (k < textus.mensura && textus.datum[k] != '\n')
        {
            perge;
        }
        linea.datum    = textus.datum + initium;
        linea.mensura  = k - initium;
        initium        = k + I;
        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }
        n = _campi(linea, campi, (i32)X);
        si (   n == (i32)IX && chorda_aequalis_literis(campi[ZEPHYRUM],
                "positum") && campi[VI].mensura == (i32)LXIV)
        {
                 chorda clavis = _clavis_seq(campi[II],
                     (i32)_numerus(campi[III]), piscina);
                 vacuum* locus = NIHIL;
        DiurnaIntroitus* i;
           DiurnaCorpus* corpus = NIHIL;

            si (tabula_dispersa_invenire(per_clavem, clavis, &locus))
            {
                i = (DiurnaIntroitus*)xar_obtinere(introitus,
                    (i32)(memoriae_index)locus - I);
            }
            alioquin
            {
                i = (DiurnaIntroitus*)xar_addere(introitus);
                si (i == NIHIL)
                {
                    redde NIHIL;
                }
                memset(i, ZEPHYRUM, magnitudo(DiurnaIntroitus));
                (vacuum)tabula_dispersa_inserere(per_clavem, clavis,
                    (vacuum*)(memoriae_index)xar_numerus(introitus));
            }
            i->clavis    = campi[II];
            i->seq       = (i32)_numerus(campi[III]);
            i->momentum  = _numerus(campi[I]);
            i->titulus   = campi[IV];
            i->fractum   = chorda_aequalis_literis(campi[V], "1");
            memcpy(i->sigillum, campi[VI].datum, (size_t)LXIV);
            i->sigillum[LXIV]  = '\0';
            i->mensura         = _numerus(campi[VII]);
            i->originalis      = _numerus(campi[VIII]);
            /* positio iterata corpus RESCRIPSIT si deletum erat */
            si (   tabula_dispersa_invenire(per_corpus, campi[VI],
                       (vacuum**)&corpus)
                && corpus->forma == DIURNA_DELETUM)
            {
                corpus->forma     = DIURNA_CRUDUM;
                corpus->momentum  = (s64)ZEPHYRUM;
            }
        }
        alioquin si (   n == (i32)III && chorda_aequalis_literis(
                     campi[ZEPHYRUM], "fixum"))
        {
            vacuum* locus = NIHIL;

            si (tabula_dispersa_invenire(per_clavem,
                _clavis_seq(campi[II],
                    ZEPHYRUM, piscina), &locus))
            {
                ((DiurnaIntroitus*)xar_obtinere(introitus,
                    (i32)(memoriae_index)locus - I))->fixum = VERUM;
            }
        }
        alioquin si (   n == (i32)III && campi[II].mensura == (i32)LXIV
                     && (   chorda_aequalis_literis(campi[ZEPHYRUM],
                                "compressum")
                         || chorda_aequalis_literis(campi[ZEPHYRUM],
                                "deletum")))
        {
            DiurnaCorpus* corpus = NIHIL;

            si (!tabula_dispersa_invenire(per_corpus, campi[II],
                    (vacuum**)&corpus))
            {
                corpus = (DiurnaCorpus*)piscina_allocare(piscina,
                    magnitudo(DiurnaCorpus));
                si (corpus == NIHIL)
                {
                    redde NIHIL;
                }
                (vacuum)tabula_dispersa_inserere(per_corpus, campi[II],
                    corpus);
            }
            corpus->forma = chorda_aequalis_literis(campi[ZEPHYRUM],
                "deletum") ? DIURNA_DELETUM : DIURNA_COMPRESSUM;
            corpus->momentum = _numerus(campi[I]);
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(introitus); k++)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(introitus,
            k);
           DiurnaCorpus* corpus = NIHIL;
                 chorda  sigillum;

        sigillum.datum    = (i8*)i->sigillum;
        sigillum.mensura  = (i32)LXIV;
        si (tabula_dispersa_invenire(per_corpus, sigillum,
                (vacuum**)&corpus))
        {
            i->forma            = corpus->forma;
            i->momentum_formae  = corpus->momentum;
        }
    }
    redde introitus;
}

interior DiurnaIntroitus*
_invenire (
          Xar* introitus,
       chorda  clavis,
          i32  seq)
{
    s32 k;   /* SIGNATUS: i32 insignatus est, 'k >= 0' numquam falsum */

    per (k = (s32)xar_numerus(introitus) - I; k >= ZEPHYRUM; k--)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(introitus,
            (i32)k);

        si (i->seq == seq && chorda_aequalis(i->clavis, clavis))
        {
            redde i;
        }
    }
    redde NIHIL;
}


/* ==================================================
 * PONERE
 * ================================================== */

/* SUTURA UNA corporis: praecisio (cauda), sigillum, scriptura atomica,
 * linea indicis. Redactio postea HIC (ante sigillum). */
b32
diurna_ponere (
               Diurna* diurna,
               chorda  clavis,
               chorda  titulus,
                  b32  fractum,
               chorda  corpus,
      DiurnaIntroitus* positum_out)
{
             chorda  servatum = corpus;
           Sigillum  sigillum;
          character  hex[SIGILLUM_HEX_MENSURA];
          character  via[DXII + C];
          character  via_compressa[DXII + C];
          character  directorium[DXII + XVI];
          character  temporaria[DXII + C];
          character  linea_numeri[CXXVIII];
  ChordaAedificator* linea;
                s64  momentum;
          FilumSera* sera;

    si (   diurna == NIHIL || clavis.mensura == ZEPHYRUM
        || !_purum(clavis)
        || !_purum(titulus))
    {
        redde FALSUM;
    }
    si (corpus.mensura > (i32)DIURNA_CORPUS_MAXIMUM)
    {
        ChordaAedificator* a =
            chorda_aedificator_creare(diurna->piscina,
            (memoriae_index)DIURNA_CORPUS_MAXIMUM
                + (memoriae_index)LXIV);
                character caput[LXIV];
                   chorda cauda;

        si (a == NIHIL)
        {
            redde FALSUM;
        }
        sprintf(caput, "[... %ld octeti praecisi]\n",
            (longus)(corpus.mensura - (i32)DIURNA_CORPUS_MAXIMUM));
        cauda.datum    = corpus.datum + corpus.mensura
            - (i32)DIURNA_CORPUS_MAXIMUM;
        cauda.mensura  = (i32)DIURNA_CORPUS_MAXIMUM;
        chorda_aedificator_appendere_literis(a, caput);
        chorda_aedificator_appendere_chorda(a, cauda);
        servatum = chorda_aedificator_finire(a);
    }
    sigillum = sigillum_computare(servatum.datum,
        (memoriae_index)servatum.mensura);
    sigillum_hex(&sigillum, hex);
    _via_corporis(diurna, hex, "", via);
    _via_corporis(diurna, hex, ".gz", via_compressa);
    /* SERA: "corpus adest?" + scriptura + linea sub sera UNA quam
     * purgare tenet - aliter purgare corpus inter probationem et lineam
     * deleret (linea ad corpus absens) */
    sera = _seram_capere(diurna);
    si (sera == NIHIL)
    {
        redde FALSUM;
    }
    /* corpus praesens (crudum aut compressum) non rescribitur */
    si (!filum_existit(via) && !filum_existit(via_compressa))
    {
        sprintf(directorium, "%s/corpora/%.2s", diurna->radix, hex);
        sprintf(temporaria, "%s.%ld.tmp", via, (longus)getpid());
        si (   !filum_directorium_creare_cum_parentibus(directorium)
            || !filum_scribere(temporaria, servatum)
            || !filum_movere(temporaria, via))
        {
            (vacuum)filum_delere(temporaria);
            filum_seram_liberare(sera);
            redde FALSUM;
        }
    }
    momentum = _momentum_nunc();
    linea = chorda_aedificator_creare(diurna->piscina,
        (memoriae_index)(clavis.mensura + titulus.mensura + CC));
    si (linea == NIHIL)
    {
        filum_seram_liberare(sera);
        redde FALSUM;
    }
    sprintf(linea_numeri, "positum\t%ld\t", (longus)momentum);
    chorda_aedificator_appendere_literis(linea, linea_numeri);
    chorda_aedificator_appendere_chorda(linea, clavis);
    chorda_aedificator_appendere_literis(linea, "\t0\t");
    chorda_aedificator_appendere_chorda(linea, titulus);
    sprintf(linea_numeri, "\t%d\t%s\t%ld\t%ld\n",
        fractum ? I : ZEPHYRUM,
        hex, (longus)servatum.mensura, (longus)corpus.mensura);
    chorda_aedificator_appendere_literis(linea, linea_numeri);
    _via_indicis(diurna, via);
    si (!filum_appendere_firmiter(via,
        chorda_aedificator_finire(linea)))
    {
        filum_seram_liberare(sera);
        redde FALSUM;
    }
    filum_seram_liberare(sera);
    si (positum_out != NIHIL)
    {
        memset(positum_out, ZEPHYRUM, magnitudo(DiurnaIntroitus));
        positum_out->clavis    = clavis;
        positum_out->seq       = ZEPHYRUM;
        positum_out->momentum  = momentum;
        positum_out->titulus   = titulus;
        positum_out->fractum   = fractum;
        memcpy(positum_out->sigillum, hex,
            (size_t)SIGILLUM_HEX_MENSURA);
        positum_out->mensura     = (s64)servatum.mensura;
        positum_out->originalis  = (s64)corpus.mensura;
        positum_out->forma       = DIURNA_CRUDUM;
    }
    redde VERUM;
}


/* ==================================================
 * LEGERE, FIGERE, INTROITUS
 * ================================================== */

chorda
diurna_legere (
          Diurna* diurna,
          chorda  clavis,
         Piscina* piscina,
    DiurnaStatus* status_out)
{
              Xar* introitus;
  DiurnaIntroitus* i;
           chorda  vacua;
           chorda  corpus;
         Sigillum  sigillum;
        character  hex[SIGILLUM_HEX_MENSURA];
        character  via[DXII + C];
     DiurnaStatus  status = DIURNA_IGNOTUM;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (status_out != NIHIL)
    {
        *status_out = DIURNA_IGNOTUM;
    }
    si (diurna == NIHIL || piscina == NIHIL)
    {
        redde vacua;
    }
    introitus = _plicare(diurna, piscina);
    i = (introitus != NIHIL) ? _invenire(introitus, clavis, ZEPHYRUM)
        : NIHIL;
    si (i == NIHIL)
    {
        redde vacua;
    }
    si (i->forma == DIURNA_DELETUM)
    {
        status = DIURNA_ABLATUM;
        corpus = vacua;
    }
    alioquin si (i->forma == DIURNA_COMPRESSUM)
    {
                chorda compressum_crudum;
        FlaturaFructus f;

        _via_corporis(diurna, i->sigillum, ".gz", via);
        compressum_crudum = filum_legere_totum(via, piscina);
        f = flatura_gzip_inflare(compressum_crudum.datum,
            compressum_crudum.mensura, piscina);
        corpus.datum = f.datum;
        corpus.mensura  = (f.status == FLATURA_STATUS_OK) ? f.mensura
            : ZEPHYRUM;
        status = (f.status == FLATURA_STATUS_OK) ? DIURNA_ADEST
            : DIURNA_CORRUPTUM;
    }
    alioquin
    {
        _via_corporis(diurna, i->sigillum, "", via);
        si (!filum_existit(via))
        {
            status = DIURNA_CORRUPTUM;
            corpus = vacua;
        }
        alioquin
        {
            corpus = filum_legere_totum(via, piscina);
            status = DIURNA_ADEST;
        }
    }
    /* sigillum verificatur: corpus mutatum numquam ut verum redditur */
    si (status == DIURNA_ADEST)
    {
        sigillum = sigillum_computare(corpus.datum,
            (memoriae_index)corpus.mensura);
        sigillum_hex(&sigillum, hex);
        si (strcmp(hex, i->sigillum) != ZEPHYRUM)
        {
            status = DIURNA_CORRUPTUM;
        }
    }
    si (status_out != NIHIL)
    {
        *status_out = status;
    }
    redde (status == DIURNA_ADEST) ? corpus : vacua;
}

b32
diurna_figere (
    Diurna* diurna,
    chorda  clavis)
{
               Xar* introitus;
         character  via[DXII + XVI];
         character  numeri[LXIV];
 ChordaAedificator* linea;
         FilumSera* sera;
               b32  bene;

    si (diurna == NIHIL || !_purum(clavis))
    {
        redde FALSUM;
    }
    /* sub sera: compactio clavem inter probationem et lineam tollere
     * non potest */
    sera = _seram_capere(diurna);
    si (sera == NIHIL)
    {
        redde FALSUM;
    }
    introitus = _plicare(diurna, diurna->piscina);
    linea = chorda_aedificator_creare(diurna->piscina,
        (memoriae_index)(clavis.mensura + LXIV));
    si (   introitus == NIHIL || linea == NIHIL
        || _invenire(introitus, clavis, ZEPHYRUM) == NIHIL)
    {
        filum_seram_liberare(sera);
        redde FALSUM;
    }
    sprintf(numeri, "fixum\t%ld\t", (longus)_momentum_nunc());
    chorda_aedificator_appendere_literis(linea, numeri);
    chorda_aedificator_appendere_chorda(linea, clavis);
    chorda_aedificator_appendere_character(linea, '\n');
    _via_indicis(diurna, via);
    bene = filum_appendere_firmiter(via,
        chorda_aedificator_finire(linea));
    filum_seram_liberare(sera);
    redde bene;
}

Xar*
diurna_introitus (
     Diurna* diurna,
    Piscina* piscina)
{
    si (diurna == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    redde _plicare(diurna, piscina);
}

constans character*
diurna_status_nomen (
    DiurnaStatus status)
{
    commutatio (status)
    {
        casus DIURNA_ADEST:      redde "adest";
        casus DIURNA_IGNOTUM:    redde "ignotum";
        casus DIURNA_ABLATUM:    redde "deletum";
        casus DIURNA_CORRUPTUM:  redde "corruptum";
        ordinarius:              redde "?";
    }
}


/* ==================================================
 * PURGARE (analytica T4 2c)
 * ================================================== */

/* corpus in plicatura purgationis */
nomen structura {
    constans character* sigillum;
           DiurnaForma  forma;          /* praesens */
                   s64  momentum;       /* formae praesentis */
           DiurnaForma  necessaria;     /* recentissima poscita */
                   b32  referenda;      /* clavis superstes spectat */
} DiurnaPurgatum;

/* forma quam clavis poscit (CRUDUM < COMPRESSUM < DELETUM) */
interior DiurnaForma
_forma_necessaria (
    constans DiurnaIntroitus* i,
       constans DiurnaRegula* regula,
                         s64  nunc_ms,
                         b32  ultimum_tituli)
{
    s64 aetas = nunc_ms - i->momentum;
    s64 crudum =
        i->fractum ? regula->crudum_fracti_ms : regula->crudum_ms;
    s64 compressum = i->fractum ? regula->compressum_fracti_ms
        : regula->compressum_ms;

    si (i->fixum || (regula->ultimum_servare && ultimum_tituli))
    {
        redde DIURNA_CRUDUM;
    }
    si (aetas < crudum)
    {
        redde DIURNA_CRUDUM;
    }
    si (aetas < compressum)
    {
        redde DIURNA_COMPRESSUM;
    }
    redde DIURNA_DELETUM;
}

interior vacuum
_lineam_positi_addere (
           ChordaAedificator* a,
    constans DiurnaIntroitus* i)
{
    character numeri[CXXVIII];

    sprintf(numeri, "positum\t%ld\t", (longus)i->momentum);
    chorda_aedificator_appendere_literis(a, numeri);
    chorda_aedificator_appendere_chorda(a, i->clavis);
    sprintf(numeri, "\t%d\t", (integer)i->seq);
    chorda_aedificator_appendere_literis(a, numeri);
    chorda_aedificator_appendere_chorda(a, i->titulus);
    sprintf(numeri, "\t%d\t%s\t%ld\t%ld\n", i->fractum ? I : ZEPHYRUM,
        i->sigillum, (longus)i->mensura, (longus)i->originalis);
    chorda_aedificator_appendere_literis(a, numeri);
}

/* corpus in formam compressam: .gz atomice, crudum deletum; octeti
 * liberati redduntur (siccum: computati, nihil scriptum) */
interior s64
_comprimere (
       constans Diurna* d,
    constans character* sigillum,
                   b32  siccum,
               Piscina* piscina,
                   b32* bene_out)
{
         character via[DXII + C];
         character via_compressa[DXII + C];
         character temporaria[DXII + C];
            chorda crudum;
    FlaturaFructus f;
            chorda compressum;

    *bene_out = FALSUM;
    _via_corporis(d, sigillum, "", via);
    _via_corporis(d, sigillum, ".gz", via_compressa);
    crudum = filum_legere_totum(via, piscina);
    si (   crudum.datum == NIHIL && crudum.mensura == ZEPHYRUM
        && !filum_existit(via))
    {
        redde (s64)ZEPHYRUM;
    }
    f = flatura_gzip_deflare(crudum.datum, crudum.mensura,
        FLATURA_COMPRESSIO_ORDINARIA, piscina);
    si (f.status != FLATURA_STATUS_OK)
    {
        redde (s64)ZEPHYRUM;
    }
    *bene_out = VERUM;
    si (!siccum)
    {
        compressum.datum    = f.datum;
        compressum.mensura  = f.mensura;
        sprintf(temporaria, "%s.%ld.tmp", via_compressa,
            (longus)getpid());
        si (   !filum_scribere(temporaria, compressum)
            || !filum_movere(temporaria, via_compressa))
        {
            (vacuum)filum_delere(temporaria);
            *bene_out = FALSUM;
            redde (s64)ZEPHYRUM;
        }
        (vacuum)filum_delere(via);
    }
    redde (s64)crudum.mensura - (s64)f.mensura;
}

b32
diurna_purgare (
                   Diurna* diurna,
    constans DiurnaRegula* regula,
                      s64  nunc_ms,
                      b32  siccum,
            DiurnaRelatio* relatio_out)
{
           DiurnaRelatio  relatio;
               FilumSera* sera;
                     Xar* introitus;
                     Xar* corpora;
          TabulaDispersa* per_corpus;
          TabulaDispersa* ultima;
               character  via[DXII + C];
                  chorda  textus;
       ChordaAedificator* novus;
                     s32  k;
                     i32  j;
                     b32  bene = VERUM;

    memset(&relatio, ZEPHYRUM, magnitudo(relatio));
    si (relatio_out != NIHIL)
    {
        *relatio_out = relatio;
    }
    si (diurna == NIHIL || regula == NIHIL)
    {
        redde FALSUM;
    }
    sera = _seram_capere(diurna);
    si (sera == NIHIL)
    {
        redde FALSUM;
    }
    _via_indicis(diurna, via);
    textus = filum_legere_totum(via, diurna->piscina);
    per (j = ZEPHYRUM; j < textus.mensura; j++)
    {
        si (textus.datum[j] == '\n')
        {
            relatio.lineae_ante++;
        }
    }
    introitus = _plicare(diurna, diurna->piscina);
    corpora = xar_creare(diurna->piscina,
        (i32)magnitudo(DiurnaPurgatum));
    per_corpus  = tabula_dispersa_creare_chorda(diurna->piscina,
        (i32)CXXVIII);
    ultima      = tabula_dispersa_creare_chorda(diurna->piscina,
        (i32)CXXVIII);
    novus       = chorda_aedificator_creare(diurna->piscina,
        (memoriae_index)(textus.mensura + LXIV));
    si (   introitus == NIHIL || corpora == NIHIL || per_corpus == NIHIL
        || ultima    == NIHIL || novus == NIHIL)
    {
        filum_seram_liberare(sera);
        redde FALSUM;
    }
    relatio.introitus = (i32)xar_numerus(introitus);
    /* ultimum cuiusque tituli: positum recentissimum */
    per (j = ZEPHYRUM; j < xar_numerus(introitus); j++)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(introitus,
            j);
                 vacuum* locus = NIHIL;

        si (   !tabula_dispersa_invenire(ultima, i->titulus, &locus)
            || ((DiurnaIntroitus*)locus)->momentum <= i->momentum)
        {
            (vacuum)tabula_dispersa_inserere(ultima, i->titulus, i);
        }
    }
    /* per clavem forma necessaria, per corpus recentissima */
    per (j = ZEPHYRUM; j < xar_numerus(introitus); j++)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(introitus,
            j);
                  chorda  sigillum;
          DiurnaPurgatum* corpus  = NIHIL;
                  vacuum* locus   = NIHIL;
             DiurnaForma  forma;

        (vacuum)tabula_dispersa_invenire(ultima, i->titulus, &locus);
        forma = _forma_necessaria(i, regula, nunc_ms,
            (DiurnaIntroitus*)locus == i);
        sigillum.datum    = (i8*)i->sigillum;
        sigillum.mensura  = (i32)LXIV;
        si (!tabula_dispersa_invenire(per_corpus, sigillum,
                (vacuum**)&corpus))
        {
            corpus = (DiurnaPurgatum*)xar_addere(corpora);
            si (corpus == NIHIL)
            {
                filum_seram_liberare(sera);
                redde FALSUM;
            }
            corpus->sigillum    = i->sigillum;
            corpus->forma       = i->forma;
            corpus->momentum    = i->momentum_formae;
            corpus->necessaria  = DIURNA_DELETUM;
            corpus->referenda   = FALSUM;
            (vacuum)tabula_dispersa_inserere(per_corpus, sigillum,
                corpus);
        }
        si (forma < corpus->necessaria)
        {
            corpus->necessaria = forma;
        }
    }
    relatio.corpora = (i32)xar_numerus(corpora);
    /* actiones per corpus - solum progrediuntur */
    per (j = ZEPHYRUM; j < xar_numerus(corpora); j++)
    {
        DiurnaPurgatum* corpus = (DiurnaPurgatum*)xar_obtinere(corpora,
            j);
             character via_cruda[DXII + C];
             character via_compressa[DXII + C];
                   s64 octeti;

        _via_corporis(diurna, corpus->sigillum, "", via_cruda);
        _via_corporis(diurna, corpus->sigillum, ".gz", via_compressa);
        octeti = (s64)filum_mensura(corpus->forma == DIURNA_COMPRESSUM
            ? via_compressa : via_cruda);
        relatio.octeti_ante += octeti;
        si (   corpus->necessaria == DIURNA_DELETUM
            && corpus->forma      != DIURNA_DELETUM)
        {
            relatio.deleta++;
            relatio.octeti_liberati += octeti;
            si (!siccum)
            {
                (vacuum)filum_delere(via_cruda);
                (vacuum)filum_delere(via_compressa);
            }
            corpus->forma     = DIURNA_DELETUM;
            corpus->momentum  = nunc_ms;
        }
        alioquin si (   corpus->necessaria == DIURNA_COMPRESSUM
                     && corpus->forma      == DIURNA_CRUDUM)
        {
            b32 compressum = FALSUM;

            relatio.octeti_liberati += _comprimere(diurna,
                corpus->sigillum,
                siccum, diurna->piscina, &compressum);
            si (compressum)
            {
                relatio.compressa++;
                corpus->forma     = DIURNA_COMPRESSUM;
                corpus->momentum  = nunc_ms;
            }
        }
    }
    /* COMPACTIO: positum ultimum per clavem (+ fixum), deinde formae
     * corporum - lineae formae POST positum (plicatura ordinem legit).
     * Oblivio: clavis cuius corpus diutius quam oblivio_ms deletum est
     * tollitur; linea corporis sine clave superstite quoque */
    per (j = ZEPHYRUM; j < xar_numerus(introitus); j++)
    {
        DiurnaIntroitus* i = (DiurnaIntroitus*)xar_obtinere(introitus,
            j);
                  chorda  sigillum;
          DiurnaPurgatum* corpus = NIHIL;

        sigillum.datum    = (i8*)i->sigillum;
        sigillum.mensura  = (i32)LXIV;
        (vacuum)tabula_dispersa_invenire(per_corpus, sigillum,
            (vacuum**)&corpus);
        si (   corpus != NIHIL && corpus->forma == DIURNA_DELETUM
            && regula->oblivio_ms > (s64)ZEPHYRUM
            && nunc_ms - corpus->momentum > regula->oblivio_ms)
        {
            relatio.oblita++;
            perge;
        }
        si (corpus != NIHIL)
        {
            corpus->referenda = VERUM;
        }
        _lineam_positi_addere(novus, i);
        relatio.lineae_post++;
        si (i->fixum)
        {
            character numeri[LXIV];

            sprintf(numeri, "fixum\t%ld\t", (longus)i->momentum);
            chorda_aedificator_appendere_literis(novus, numeri);
            chorda_aedificator_appendere_chorda(novus, i->clavis);
            chorda_aedificator_appendere_character(novus, '\n');
            relatio.lineae_post++;
        }
    }
    per (k = ZEPHYRUM; k < (s32)xar_numerus(corpora); k++)
    {
        DiurnaPurgatum* corpus = (DiurnaPurgatum*)xar_obtinere(corpora,
            (i32)k);
             character linea[CXXVIII];

        si (!corpus->referenda || corpus->forma == DIURNA_CRUDUM)
        {
            perge;
        }
        sprintf(linea, "%s\t%ld\t%s\n", corpus->forma == DIURNA_DELETUM
            ? "deletum" : "compressum", (longus)corpus->momentum,
            corpus->sigillum);
        chorda_aedificator_appendere_literis(novus, linea);
        relatio.lineae_post++;
    }
    si (!siccum)
    {
        character temporaria[DXII + C];

        sprintf(temporaria, "%s.%ld.tmp", via, (longus)getpid());
        bene =    filum_scribere(temporaria,
            chorda_aedificator_finire(novus))
               && filum_movere(temporaria, via);
        si (!bene)
        {
            (vacuum)filum_delere(temporaria);
        }
    }
    filum_seram_liberare(sera);
    si (relatio_out != NIHIL)
    {
        *relatio_out = relatio;
    }
    redde bene;
}
