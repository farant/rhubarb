/* vicus_applicatio.c - compositio communis hospitis (T4) */

#include "vicus_applicatio.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "terminale.h"
#include "kalendarium.h"
#include "nomenclator.h"
#include "iussum.h"
#include "admonitiones_visus.h"
#include "admonitiones_volantes.h"
#include "multiplexor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/* dispositio ordinaria (vicus-latera decisio IX, Franus 2026-10-08):
 * decem tabulae - I scriba | terminale, II scriba | pictor, III-X
 * scriba | scriba */
#define TABULA_ORDINARIA(id, dextrum)                               \
    "<tabula id=\"" id "\" focus=\"sinistrum\">"                    \
    "<latus genus=\"scriba\"/>"                                     \
    "<acervus><latus genus=\"" dextrum "\"/></acervus></tabula>"

#define INDEX_ORDINARIUS                                            \
    "<tabulae activa=\"1\">"                                        \
    TABULA_ORDINARIA("1", "terminale")                              \
    TABULA_ORDINARIA("2", "pictor")                                 \
    TABULA_ORDINARIA("3", "scriba")                                 \
    TABULA_ORDINARIA("4", "scriba")                                 \
    TABULA_ORDINARIA("5", "scriba")                                 \
    TABULA_ORDINARIA("6", "scriba")                                 \
    TABULA_ORDINARIA("7", "scriba")                                 \
    TABULA_ORDINARIA("8", "scriba")                                 \
    TABULA_ORDINARIA("9", "scriba")                                 \
    TABULA_ORDINARIA("10", "scriba")                                \
    "</tabulae>"


/* ==================================================
 * Genera (VicusMontator, VicusDescriptor)
 * ================================================== */

/* S2b/S3b: contextus generis scribae - liber paginarum et iussa,
 * communia omnibus visibus */
nomen structura {
        ScribaLiber* liber;
    IussumRegistrum* iussa;
    /* status-plan S6b: systemata repositorii vici, communia cum
     * contextu pictoris (S7b: vicus_aperire ea creat) */
    StatusSystemata* systemata;
} ContextusScribae;

/* S3b: '$dies' -> dies hodiernus "MM/DD/YYYY" (forma concha vetus;
 * consumens, ut prunifex). S3b-2: '$dies(N)' = N dies ab hodie
 * (signatus: -1 heri, 7 hebdomas post); mktime menses et annos
 * normat. Argumentum non numerus aut plura: error. */
interior b32
dies_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
          time_t nunc;
    structura tm* tm;
    structura tm  dies;
             s32 gradus;
       character textus[XXXII];

    (vacuum)ctx;
    gradus = ZEPHYRUM;
    si (iussum->numerus_argumentorum > I)
    {
        effectus->error = chorda_ex_literis("dies: unum argumentum",
            piscina);
        redde VERUM;
    }
    si (   iussum->numerus_argumentorum == I
        && !chorda_ut_s32(iussum->argumenta[ZEPHYRUM], &gradus))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            "dies: numerus dierum non intellegitur: ", piscina),
            iussum->argumenta[ZEPHYRUM], piscina);
        redde VERUM;
    }
    nunc  = time(NIHIL);
    tm    = localtime(&nunc);
    si (!tm)
    {
        redde FALSUM;
    }
    dies           = *tm;
    dies.tm_mday   += gradus;
    dies.tm_isdst  = -I;
    si (mktime(&dies) == (time_t)-I)
    {
        redde FALSUM;
    }
    sprintf(textus, "%02d/%02d/%04d", dies.tm_mon + I, dies.tm_mday,
        dies.tm_year + MCM);
    effectus->textus = chorda_ex_literis(textus, piscina);
    redde VERUM;
}


/* ==================================================
 * $question<>...</> (Franus 2026-10-13): quaestio ad exemplar per
 * portam (applicatio fenestrae: vates); responsum in lineas albas
 * finales paginae, numquam ultra
 * ================================================== */

#define QUAESTIO_EXEMPLAR_NOMEN "claude-opus-5-5"
#define QUAESTIO_INITIUM        "<answer>"
#define QUAESTIO_FINIS          "</>"

nomen structura {
    VicusPortaQuaestionis porta;
                      b32 posita;   /* porta data (fenestra) */
} ContextusQuaestionis;

#define QUAESTIO_MARGO II   /* columnae vacuae utrimque (Franus) */

/* status dispositionis: exitus (NIHIL = numerare solum) */
nomen structura {
        i8* exitus;
       s32  o;
       s32  columna;
       s32  lineae;
       b32  nuda;      /* linea sine octeto adhuc (margo nondum) */
       s32  margo;
} Dispositio;

interior vacuum
dispositionem_scribere (
    Dispositio* d,
   constans i8* datum,
           s32  n)
{
    si (d->nuda && n > ZEPHYRUM)
    {
        s32 k;
        per (k = ZEPHYRUM; k < d->margo; k++)
        {
            si (d->exitus)
            {
                d->exitus[d->o] = (i8)' ';
            }
            d->o++;
        }
        d->columna  = d->margo;
        d->nuda     = FALSUM;
    }
    si (d->exitus && n > ZEPHYRUM)
    {
        memcpy(d->exitus + d->o, datum, (memoriae_index)n);
    }
    d->o        += n;
    d->columna  += n;
}

interior vacuum
dispositionis_linea_nova (
    Dispositio* d)
{
    si (d->exitus)
    {
        d->exitus[d->o] = (i8)'\n';
    }
    d->o++;
    d->lineae++;
    d->columna  = ZEPHYRUM;
    d->nuda     = VERUM;
}

/* marca lineae logicae post indentationem: "- " "* " "+ " "> " "# " aut
 * "N. " "N) "; redde longitudinem (0 = nulla) */
interior s32
marca_longitudo (
    constans i8* t,
            s32  n)
{
    s32 k = ZEPHYRUM;

    si (   n >= II && t[I] == (i8)' '
        && (t[0] == (i8)'-' || t[0] == (i8)'*' || t[0] == (i8)'+'
            || t[0] == (i8)'>' || t[0] == (i8)'#'))
    {
        redde II;
    }
    dum (k < n && k < IX && t[k] >= (i8)'0' && t[k] <= (i8)'9')
    {
        k++;
    }
    si (   k > ZEPHYRUM && k + I < n
        && (t[k] == (i8)'.' || t[k] == (i8)')') && t[k + I] == (i8)' ')
    {
        redde k + II;
    }
    redde ZEPHYRUM;
}

/* lineae logicae (quaestio.h: '\t' ducens = verbatim; aliter indentatio
 * + marca + verba refluenda; linea vacua = separatio) in lineas paginae
 * latitudinis W cum margine QUAESTIO_MARGO utrimque: "<answer>" ante
 * primam, "</>" post ultimam; textus refluit cum indentatione pendente
 * post marcam, verbatim numquam refluit (longior frangitur). ellipsis:
 * "..." post textum. Redde numerum linearum; exitus (si datus) eas '\n'
 * iunctas accipit */
interior s32
responsum_disponere (
    constans i8* t,
            s32  n,
            s32  w,
            b32  ellipsis,
             i8* exitus,
            s32* longitudo)
{
    Dispositio d;
           s32 dextra;
           s32 i              = ZEPHYRUM;
           b32 sola_marca     = VERUM;   /* linea prima: "<answer>" solum */
           b32 vacua_pendens  = FALSUM;

    d.exitus   = exitus;
    d.o        = ZEPHYRUM;
    d.columna  = ZEPHYRUM;
    d.lineae   = I;
    d.nuda     = VERUM;
    d.margo = (w - II * QUAESTIO_MARGO
        >= XX) ? QUAESTIO_MARGO : ZEPHYRUM;
    dextra = w - d.margo;
    dispositionem_scribere(&d, (constans i8*)QUAESTIO_INITIUM,
        (s32)strlen(QUAESTIO_INITIUM));
    dum (i < n)
    {
        s32 f = i;
        s32 k;
        s32 m;

        dum (f < n && t[f] != (i8)'\n')
        {
            f++;
        }
        si (f == i)
        {
            vacua_pendens  = !sola_marca;
            i              = f + I;
            perge;
        }
        si (!sola_marca || t[i] == (i8)'\t')
        {
            dispositionis_linea_nova(&d);
            si (vacua_pendens)
            {
                dispositionis_linea_nova(&d);
            }
        }
        sola_marca     = FALSUM;
        vacua_pendens  = FALSUM;
        si (t[i] == (i8)'\t')
        {
            /* verbatim: frusta latitudinis interioris */
            s32 a      = i + I;
            s32 latus  = dextra - d.margo;
            b32 prima  = VERUM;
            dum (prima || a < f)
            {
                s32 q = f - a < latus ? f - a : latus;
                si (!prima)
                {
                    dispositionis_linea_nova(&d);
                }
                dispositionem_scribere(&d, t + a, q);
                a      += q;
                prima  = FALSUM;
            }
            i = f + I;
            perge;
        }
        /* textus: indentatio + marca, deinde verba */
        k = ZEPHYRUM;
        dum (i + k < f && t[i + k] == (i8)' ')
        {
            k++;
        }
        m = marca_longitudo(t + i + k, f - i - k);
        dispositionem_scribere(&d, t + i, k + m);
        {
            s32 pendens  = k + m;
            s32 a        = i + k + m;
            b32 initium  = VERUM;   /* nullum verbum adhuc post marcam */

            dum (a < f)
            {
                s32 b;
                si (t[a] == (i8)' ')
                {
                    a++;
                    perge;
                }
                b = a;
                dum (b < f && t[b] != (i8)' ')
                {
                    b++;
                }
                dum (a < b)
                {
                    s32 spatium  = initium ? ZEPHYRUM : I;
                    s32 locus    = dextra - d.columna - spatium;
                    s32 q        = b - a;
                    si (   q > locus && (q <= dextra - d.margo - pendens
                        || locus <= ZEPHYRUM))
                    {
                        s32 r;
                        dispositionis_linea_nova(&d);
                        per (r = ZEPHYRUM; r < pendens; r++)
                        {
                            dispositionem_scribere(&d,
                                (constans i8*)" ", I);
                        }
                        si (pendens == ZEPHYRUM)
                        {
                            dispositionem_scribere(&d, (constans i8*)"",
                                ZEPHYRUM);
                        }
                        spatium = ZEPHYRUM;
                        locus = dextra - (d.nuda ? d.margo : d.columna);
                    }
                    si (q > locus)
                    {
                        q = locus;   /* verbum longius quam linea */
                    }
                    si (spatium)
                    {
                        dispositionem_scribere(&d, (constans i8*)" ",
                            I);
                    }
                    dispositionem_scribere(&d, t + a, q);
                    a        += q;
                    initium  = FALSUM;
                }
            }
        }
        i = f + I;
    }
    si (ellipsis)
    {
        si (d.columna + III > dextra)
        {
            dispositionis_linea_nova(&d);
        }
        dispositionem_scribere(&d, (constans i8*)"...", III);
    }
    si (d.nuda || d.columna + (s32)strlen(QUAESTIO_FINIS) > dextra)
    {
        dispositionis_linea_nova(&d);
    }
    dispositionem_scribere(&d, (constans i8*)QUAESTIO_FINIS,
        (s32)strlen(QUAESTIO_FINIS));
    *longitudo = d.o;
    redde d.lineae;
}

/* responsum in L lineas: totum si capit; aliter praefixum longissimum
 * quod ad finem lineae logicae aut sententiae ('.', '!', '?') desinit;
 * aliter ad verbum cum "..." */
interior chorda
responsum_aptare (
    chorda  textus,
       s32  w,
       s32  l,
   Piscina* piscina)
{
    chorda  c;
        i8* q;
       s32  n;
       s32  k;

    q = (i8*)piscina_allocare(piscina,
        (memoriae_index)((s32)textus.mensura
        * II + w * IV + LXIV));
    c.datum = q;
    si (responsum_disponere(textus.datum, (s32)textus.mensura, w,
        FALSUM,
            NIHIL, &n) <= l)
    {
        (vacuum)responsum_disponere(textus.datum, (s32)textus.mensura,
            w,
            FALSUM, q, &n);
        c.mensura = (i32)n;
        redde c;
    }
    per (k = (s32)textus.mensura - I; k > ZEPHYRUM; k--)
    {
         i8 x          = textus.datum[k];
        s32 longitudo  = -I;
        si (x == (i8)'\n')
        {
            longitudo = k;
        }
        alioquin si (   (x == (i8)'.' || x == (i8)'!' || x == (i8)'?')
                     && (k + I == (s32)textus.mensura
                         || textus.datum[k + I] == (i8)' '
                         || textus.datum[k + I] == (i8)'\n'))
        {
            longitudo = k + I;
        }
        si (   longitudo > ZEPHYRUM
            && responsum_disponere(textus.datum, longitudo, w, FALSUM,
            NIHIL,
                   &n) <= l)
        {
            (vacuum)responsum_disponere(textus.datum, longitudo, w,
                FALSUM, q,
                &n);
            c.mensura = (i32)n;
            redde c;
        }
    }
    per (k = (s32)textus.mensura - I; k > ZEPHYRUM; k--)
    {
        si (   (textus.datum[k] == (i8)' '
            || textus.datum[k] == (i8)'\n')
            && responsum_disponere(textus.datum, k, w, VERUM, NIHIL, &n)
                <= l)
        {
            (vacuum)responsum_disponere(textus.datum, k, w, VERUM, q,
                &n);
            c.mensura = (i32)n;
            redde c;
        }
    }
    (vacuum)responsum_disponere(textus.datum, ZEPHYRUM, w, VERUM, q,
        &n);
    c.mensura = (i32)n;
    redde c;
}

chorda
vicus_quaestio_disponere (
      chorda  planatum,
         s32  w,
         s32  l,
     Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis("\n", piscina),
        responsum_aptare(planatum, w, l, piscina), piscina);
}

/* opus $question (quaestio-rivus T8): porta pulsatur, textus planatus
 * disponitur; pagina plena -> porta abrumpitur; piscina propria in fine
 * destruitur (textus redditi in piscina pulsus) */
nomen structura {
               IussumOpus  opus;        /* primum */
    VicusPortaQuaestionis  porta;
                   vacuum* manubrium;
                  Piscina* piscina;     /* propria */
                   chorda  textus;      /* planatum ultimum */
                      s32  w;
                      s32  l;
} OpusQuaestionis;

interior chorda
quaestionis_nuntius (
     constans character* praefixum,
                 chorda  textus,
                Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        textus, piscina);
}

/* textus + paragraphus ultimus (marca) */
interior chorda
cum_marca (
                chorda  textus,
    constans character* marca,
               Piscina* piscina)
{
    chorda c = chorda_concatenare(chorda_ex_literis("", piscina),
        textus,
        piscina);

    si (c.mensura > ZEPHYRUM)
    {
        c = chorda_concatenare(c, chorda_ex_literis("\n\n", piscina),
            piscina);
    }
    redde chorda_concatenare(c, chorda_ex_literis(marca, piscina),
        piscina);
}

interior vacuum
opus_quaestionis_finire (
    OpusQuaestionis* opera)
{
    Piscina* p = opera->piscina;

    opera->piscina = NIHIL;
    si (p)
    {
        piscina_destruere(p);
    }
}

interior IussumOpusStatus
opus_quaestionis_pulsare (
    IussumOpus* opus,
       Piscina* piscina,
        chorda* textus,
        chorda* nuntius,
           b32* mutatum)
{
           OpusQuaestionis* opera = (OpusQuaestionis*)opus->datum;
    VicusPulsusQuaestionis  p;
                       s32  n;
                    chorda  planatum;
                 character  usus[XCVI];

    p = opera->porta.pulsare(opera->manubrium, opera->piscina);
    si (p.mutatum)
    {
        opera->textus = chorda_concatenare(chorda_ex_literis("",
            opera->piscina), p.textus, opera->piscina);
    }
    planatum = chorda_concatenare(chorda_ex_literis("", piscina),
        opera->textus, piscina);
    si (p.error.mensura > ZEPHYRUM)
    {
        *nuntius = quaestionis_nuntius("question: ", p.error, piscina);
        si (planatum.mensura == ZEPHYRUM)
        {
            opus_quaestionis_finire(opera);
            redde IUSSUM_OPUS_IRRITUM;
        }
        *textus = vicus_quaestio_disponere(cum_marca(planatum,
            chorda_ut_cstr(quaestionis_nuntius("[error: ",
            chorda_concatenare(
            p.error, chorda_ex_literis("]", piscina), piscina),
            piscina),
            piscina), piscina), opera->w, opera->l, piscina);
        opus_quaestionis_finire(opera);
        redde IUSSUM_OPUS_FINITUM;
    }
    si (p.mutatum)
    {
        /* pagina plena: rivus sistitur, praecisio ad finem
         * sententiae */
        si (responsum_disponere(planatum.datum, (s32)planatum.mensura,
                opera->w, FALSUM, NIHIL, &n) > opera->l)
        {
            opera->porta.abrumpere(opera->manubrium);
            *textus = vicus_quaestio_disponere(planatum, opera->w,
                opera->l,
                piscina);
            *nuntius = chorda_ex_literis("question: page full",
                piscina);
            opus_quaestionis_finire(opera);
            redde IUSSUM_OPUS_FINITUM;
        }
        *textus = vicus_quaestio_disponere(planatum, opera->w, opera->l,
            piscina);
        *mutatum = VERUM;
    }
    si (p.finitus)
    {
        si (planatum.mensura == ZEPHYRUM)
        {
            *nuntius = chorda_ex_literis("question: empty answer",
                piscina);
            opus_quaestionis_finire(opera);
            redde IUSSUM_OPUS_IRRITUM;
        }
        sprintf(usus, "question: %ld in, %ld out", (longus)p.input,
            (longus)p.output);
        *textus = vicus_quaestio_disponere(planatum, opera->w, opera->l,
            piscina);
        *nuntius = chorda_ex_literis(usus, piscina);
        opus_quaestionis_finire(opera);
        redde IUSSUM_OPUS_FINITUM;
    }
    redde IUSSUM_OPUS_CURRIT;
}

interior vacuum
opus_quaestionis_abrumpere (
    IussumOpus* opus,
       Piscina* piscina,
        chorda* textus,
        chorda* nuntius)
{
    OpusQuaestionis* opera = (OpusQuaestionis*)opus->datum;
             chorda  planatum;

    opera->porta.abrumpere(opera->manubrium);
    planatum = chorda_concatenare(chorda_ex_literis("", piscina),
        opera->textus, piscina);
    *textus = vicus_quaestio_disponere(cum_marca(planatum,
        "[cancelled]",
        piscina), opera->w, opera->l, piscina);
    *nuntius = chorda_ex_literis("question: cancelled", piscina);
    opus_quaestionis_finire(opera);
}

interior b32
quaestionem_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    ContextusQuaestionis* cq  = (ContextusQuaestionis*)ctx;
                     s32  w   = iussum->latitudo_tabulae;
                     s32  l   = iussum->lineae_liberae;
                     s32  lineae;
                     s32  latitudo;
                 Piscina* propria;
         OpusQuaestionis* opera;
                  chorda  error;

    si (!iussum->corpus || iussum->numerus_argumentorum != I)
    {
        effectus->error = chorda_ex_literis(
            "question: write $question<>...</>", piscina);
        redde VERUM;
    }
    si (l < I)
    {
        effectus->error = chorda_ex_literis(
            "question: no free lines below", piscina);
        redde VERUM;
    }
    si (!cq || !cq->posita)
    {
        effectus->error = chorda_ex_literis(
            "question: no API port (window app only)", piscina);
        redde VERUM;
    }
    si (!iussum->iterum)
    {
        effectus->differre = VERUM;
        effectus->nuntius = chorda_ex_literis(
            "question: asking " QUAESTIO_EXEMPLAR_NOMEN "...", piscina);
        redde VERUM;
    }
    /* R8: linea "<answer>" non textus; margines non textus */
    lineae    = l - I >= I ? l - I : I;
    latitudo  = w - II * QUAESTIO_MARGO >= XX ? w - II * QUAESTIO_MARGO
                                            : w;
    propria   = piscina_generare_dynamicum("quaestio_opus", LXIV * M);
    opera        = propria ? (OpusQuaestionis*)piscina_allocare(propria,
        magnitudo(OpusQuaestionis)) : NIHIL;
    si (!opera)
    {
        si (propria)
        {
            piscina_destruere(propria);
        }
        effectus->error = chorda_ex_literis("question: out of memory",
            piscina);
        redde VERUM;
    }
    memset(opera, ZEPHYRUM, magnitudo(OpusQuaestionis));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    opera->manubrium  = cq->porta.incipere(cq->porta.ctx,
        iussum->argumenta[ZEPHYRUM], lineae, latitudo, propria, &error);
    si (!opera->manubrium)
    {
        effectus->error = quaestionis_nuntius("question: ",
            chorda_concatenare(chorda_ex_literis("", piscina), error,
            piscina), piscina);
        piscina_destruere(propria);
        redde VERUM;
    }
    opera->porta           = cq->porta;
    opera->piscina         = propria;
    opera->textus          = chorda_ex_literis("", propria);
    opera->w               = w;
    opera->l               = l;
    opera->opus.datum      = opera;
    opera->opus.pulsare    = opus_quaestionis_pulsare;
    opera->opus.abrumpere  = opus_quaestionis_abrumpere;
    effectus->opus         = &opera->opus;
    /* "<answered>" (X) brevius quam "$question<>" (XI): linea numquam
     * crescit, ergo numquam transfluit (Franus 2026-10-14) */
    effectus->caput     = chorda_ex_literis("<answered>", piscina);
    effectus->nuntius   = chorda_ex_literis(
        "question: streaming... (Esc cancels)", piscina);
    redde VERUM;
}

/* S3c: verba aperientia ('$terminale', '$scriba(nomen)',
 * '$pictor(nomen)') - latus in acervo tabulae activae (petitio,
 * pulsu proximo applicata); signum manet (non consumunt) */
nomen structura {
                  Vicus* vicus;
     constans character* genus;
     /* A4: '$agenda' = genus calendarium cum "agenda"; NIHIL =
      * argumentum ab usore */
     constans character* argumentum_fixum;
} ContextusAperiendi;

/* S3e: contextus generis pictoris - memoria picturarum COMMUNIS omnium
 * laterum (documentum unum per spatium: latus quod picturam relinquit
 * et postea redit documentum alterius lateris recens videt, non
 * copiam veterem) */
nomen structura {
    TabulaDispersa* documenta;
    /* L5: porta imaginis (applicatio ponit; NIHIL = nulla) */
      ImagoFructus (*porta)(vacuum* ctx, Piscina* piscina);
            vacuum* porta_ctx;
    /* status-plan S3-S6: systemata repositorii vici, communia cum
     * contextu scribae (S7b: vicus_aperire ea creat) */
    StatusSystemata* systemata;
} ContextusPictoris;

/* L5: porta lateris pictoris - ad portam communem (tempore vocationis
 * lecta: latera ante portam positam montata eam quoque vident) */
interior ImagoFructus
imaginem_capere_communem (
     vacuum* ctx,
    Piscina* piscina)
{
    ContextusPictoris* cp;
         ImagoFructus  f;

    cp = (ContextusPictoris*)ctx;
    si (cp && cp->porta)
    {
        redde cp->porta(cp->porta_ctx, piscina);
    }
    memset(&f, ZEPHYRUM, magnitudo(f));
    redde f;
}

/* S3e: $pictor-next / $pictor-prev */
nomen structura {
                  Vicus* vicus;
                    s32  directio;     /* I next, -I prev */
     constans character* verbum;
} ContextusCycli;

interior b32
nomen_paginae_validum (
    chorda c)
{
          i32 i;
    character x;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        x = (character)c.datum[i];
        si (!(   (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')
              || x == '_' || x == '-'))
        {
            redde FALSUM;
        }
    }
    redde c.mensura > ZEPHYRUM;
}

interior b32
aperire_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusAperiendi* ca;
                         chorda  arg;

    ca = (constans ContextusAperiendi*)ctx;
    si (iussum->numerus_argumentorum > I)
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            ca->genus, piscina), chorda_ex_literis(": unum argumentum",
            piscina), piscina);
        redde VERUM;
    }
    arg.datum    = NIHIL;
    arg.mensura  = ZEPHYRUM;
    si (iussum->numerus_argumentorum == I)
    {
        arg = iussum->argumenta[ZEPHYRUM];
    }
    si (   arg.mensura > ZEPHYRUM
        && strcmp(ca->genus, "scriba") == ZEPHYRUM
        && !nomen_paginae_validum(arg))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            "scriba: nomen paginae invalidum: ", piscina), arg,
            piscina);
        redde VERUM;
    }
    si (!vicus_acervo_aperire(ca->vicus, ca->genus,
            ca->argumentum_fixum ? ca->argumentum_fixum
            : arg.mensura > ZEPHYRUM ? chorda_ut_cstr(arg, piscina)
                                     : NIHIL))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            ca->genus, piscina), chorda_ex_literis(
            ": aperiri non potest", piscina), piscina);
    }
    redde VERUM;
}

interior b32
aperiens_registrare (
        IussumRegistrum* r,
                  Vicus* v,
     constans character* genus,
                Piscina* piscina)
{
    ContextusAperiendi* ca;

    ca = (ContextusAperiendi*)piscina_allocare(piscina,
        magnitudo(ContextusAperiendi));
    si (!ca)
    {
        redde FALSUM;
    }
    ca->vicus             = v;
    ca->genus             = genus;
    ca->argumentum_fixum  = NIHIL;
    redde iussum_registrare(r, genus, FALSUM, aperire_iussum, ca);
}

/* A4: verbum quod genus cum argumento fixo aperit ('$agenda' =
 * calendarium "agenda") */
interior b32
aperiens_fixum_registrare (
        IussumRegistrum* r,
                  Vicus* v,
     constans character* verbum,
     constans character* genus,
     constans character* argumentum,
                Piscina* piscina)
{
    ContextusAperiendi* ca;

    ca = (ContextusAperiendi*)piscina_allocare(piscina,
        magnitudo(ContextusAperiendi));
    si (!ca)
    {
        redde FALSUM;
    }
    ca->vicus             = v;
    ca->genus             = genus;
    ca->argumentum_fixum  = argumentum;
    redde iussum_registrare(r, verbum, FALSUM, aperire_iussum, ca);
}

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusScribae* cs;
      ScribaMontatio* m;

    /* S2b/S3b: ctx = contextus communis (aedificare) */
    cs  = (ContextusScribae*)ctx;
    m   = (ScribaMontatio*)sedes;
    /* S3c: '$scriba(nomen)' - pagina nominata (condita si deest) est
     * pagina PRIMA visus; visus iam servatus (reapertura, navigatio)
     * suam servat */
    si (argumentum && cs->liber)
    {
        chorda visus;
           b32 inventum;

        visus = chorda_concatenare(chorda_ex_literis("scriba/visus/",
            piscina), chorda_ex_literis(id, piscina), piscina);
        (vacuum)volumen_plagulam_promere(volumen, visus, piscina,
            &inventum);
        si (!inventum)
        {
            si (!scriba_liber_paginam_condere(cs->liber,
                    chorda_ex_literis(argumentum, piscina)))
            {
                redde FALSUM;
            }
            (vacuum)volumen_plagulam_condere(volumen, visus,
                chorda_ex_literis(argumentum, piscina), "scriba:visus");
        }
    }
    si (!scriba_montare(m, piscina, intern, volumen, repo, id,
            latitudo, altitudo, cs->liber, cs->systemata))
    {
        redde FALSUM;
    }
    m->actiones_ctx.iussa = cs->iussa;
    /* S7b: registrum vim unum applicationis - radix hospitis */
    m->actiones_ctx.ramus_capsae = insula_ramus_radix(repo);
    redde VERUM;
}

/* focus advenit (S2b): visus stalus PRIMUM reficitur - aliter clavis
 * prima super folium vetus scriberet et mutationem alterius visus
 * reverteret; deinde gestus */
interior vacuum
scribae_gestum (
     Motus* motus,
    vacuum* ctx)
{
    ScribaMontatio* m;

    m = (ScribaMontatio*)ctx;
    (vacuum)scriba_reficere(m);
    scriba_gestum_ponere(motus, &m->actiones_ctx);
}

/* pulsus (S2b): visus stalus reficitur (pagina ab alio visu mutata) */
interior VicusPulsus
scribam_pulsare (
    vacuum* ctx)
{
    VicusPulsus p;

    p.mutatum = scriba_reficere((ScribaMontatio*)ctx);
    p.finitus = FALSUM;
    redde p;
}

/* ictus primus (Franus): ictus qui latus scribae focat iussum sub se
 * statim currit - ut si latus iam focatum esset. Pagina in spatio
 * lateris quaeritur (arbor composita); ictus extra iussum focat
 * solum */
interior b32
scribae_ictus_primus (
                 vacuum* ctx,
     InsulaRepositorium* repo,
                  Motus* motus,
              Componens* arbor,
       constans Eventus* ev)
{
    ScribaMontatio* m;
         Componens* pagina;
           Punctum  p;
            chorda  prior;
               b32  actum;

    m = (ScribaMontatio*)ctx;
    si (   !m || !arbor || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    pagina = componens_invenire_in_spatio(arbor, m->ramus.via,
        chorda_ex_literis("pagina", m->actiones_ctx.doc->piscina));
    p.x = ev->datum.mus.x;
    p.y = ev->datum.mus.y;
    si (   !pagina
        || !scriba_iussum_ad_punctum(&m->actiones_ctx, pagina, p))
    {
        redde FALSUM;
    }
    /* scriptor = actio paginae: domini status scribae (cursor, modus,
     * nuntius) 'pagina.clavis' solum admittunt - intra tractatorem
     * radicis vici scriptor alius est et scripturae tacite
     * recusarentur */
    prior = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("pagina.clavis",
        m->actiones_ctx.doc->piscina));
    actum = scriba_pagina_clavis(repo, motus, NIHIL, pagina, ev,
        &m->actiones_ctx);
    insula_scriptorem_ponere(repo, prior);
    redde actum;
}

interior vacuum
scribam_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    ScribaMontatio* m;

    m                    = (ScribaMontatio*)montatio;
    f->actiones          = m->actiones;
    f->figurae           = m->figurae;
    f->componere         = scriba_componere;
    f->componere_ctx     = &m->compositio;
    f->gestum_ponere     = scribae_gestum;
    f->gestum_ctx        = m;
    f->pulsare           = scribam_pulsare;
    f->pulsare_ctx       = m;
    f->ictus_primus      = scribae_ictus_primus;
    f->ictus_primus_ctx  = m;
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
     PictorMontatio* m;
  ContextusPictoris* cp;
             vacuum* valor;
             chorda  clavis;

    m   = (PictorMontatio*)sedes;
    cp  = (ContextusPictoris*)ctx;
    si (!pictor_montare(m, piscina, intern, volumen, repo, id,
            latitudo, altitudo, cp ? cp->systemata : NIHIL))
    {
        redde FALSUM;
    }
    /* L5: porta imaginis communis */
    m->actiones_ctx.imago_capere  = imaginem_capere_communem;
    m->actiones_ctx.imago_ctx     = cp;
    /* S3e: memoria communis - documentum spatii huius, si iam
     * apertum, idem fit */
    si (cp && cp->documenta && id && id[ZEPHYRUM])
    {
        clavis  = chorda_ex_literis(id, piscina);
        valor   = NIHIL;
        si (tabula_dispersa_invenire(cp->documenta, clavis, &valor))
        {
            m->documenta = cp->documenta;
            (vacuum)pictor_picturam_ponere(m, id);
        }
        alioquin
        {
            (vacuum)tabula_dispersa_inserere(cp->documenta, clavis,
                m->doc);
            m->documenta = cp->documenta;
        }
    }
    /* S3e: argumentum picturam bibliothecae nominans ostenditur;
     * aliter (S3c, identitas sola) pictura lateris ipsius */
    si (argumentum && argumentum[ZEPHYRUM])
    {
        (vacuum)pictor_picturam_ponere(m, argumentum);
    }
    redde VERUM;
}

/* S3e: facies.argumentum_ponere pictoris */
interior b32
pictoris_argumentum_ponere (
                 vacuum* ctx,
     constans character* argumentum)
{
    /* S3f: pictura ignota ($pictor-new) conditur */
    redde pictor_picturam_ponere((PictorMontatio*)ctx, argumentum)
        || pictor_picturam_condere((PictorMontatio*)ctx, argumentum);
}

interior vacuum
pictorem_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    PictorMontatio* m;

    m                         = (PictorMontatio*)montatio;
    f->actiones               = m->actiones;
    f->figurae                = m->figurae;
    f->componere              = pictor_componere;
    f->componere_ctx          = &m->compositio;
    f->fons                   = pictor_imago_fons;
    f->fons_ctx               = &m->figurae_ctx;
    f->argumentum_ponere      = pictoris_argumentum_ponere;
    f->argumentum_ponere_ctx  = m;
}

/* S3e: pictura lateris pictoris: argumentum si picturam bibliothecae
 * nominat, aliter id lateris */
interior chorda
pictura_lateris (
       constans VicusLatus* l,
              constans Xar* bibliotheca)
{
    i32 i;

    per (i = ZEPHYRUM; !chorda_vacua(l->argumentum)
                       && i < xar_numerus(bibliotheca); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(bibliotheca, i),
            l->argumentum))
        {
            redde l->argumentum;
        }
    }
    redde l->id;
}

/* S3e: $pictor-next / $pictor-prev (Franus): frons pictoris tabulae
 * activae picturam proximam bibliothecae (ordine viae, circulo)
 * ostendit - etiam in latere alio ostensam (documentum unum, memoria
 * communis: latera plura tuta; Franus regulam praeteritionis
 * removit). Nullus pictor in acervo: latus novum. PETITIO. */
interior b32
cyclus_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusCycli* cc;
                VicusTabula* t;
                 VicusLatus* l;
                 VicusLatus* frons;
                        Xar* bibliotheca;
                     chorda  currens;
                     chorda  pictura;
                        s32  n;
                        s32  initium;
                        s32  k;
                        s32  index;
                        i32  i;

    cc = (constans ContextusCycli*)ctx;
    si (iussum->numerus_argumentorum > ZEPHYRUM)
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": nulla argumenta", piscina),
            piscina);
        redde VERUM;
    }
    bibliotheca  = pictor_documenta_enumerare(cc->vicus->volumen,
        piscina);
    t      = vicus_activa(cc->vicus);
    frons  = NIHIL;
    per (i = ZEPHYRUM; t && i < xar_numerus(t->acervus); i++)
    {
        l = (VicusLatus*)xar_obtinere(t->acervus, i);
        si (chorda_aequalis_literis(l->genus, "pictor"))
        {
            frons = l;
        }
    }
    currens.datum    = NIHIL;
    currens.mensura  = ZEPHYRUM;
    si (frons && bibliotheca)
    {
        currens = pictura_lateris(frons, bibliotheca);
    }
    n        = bibliotheca ? (s32)xar_numerus(bibliotheca) : ZEPHYRUM;
    initium  = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(bibliotheca, (i32)k),
            currens))
        {
            initium = k;
        }
    }
    per (k = I; k <= n; k++)
    {
        index = initium < ZEPHYRUM
            ? (cc->directio > ZEPHYRUM ? k - I : n - k)
            : ((initium + cc->directio * k) % n + n) % n;
        pictura = *(chorda*)xar_obtinere(bibliotheca, (i32)index);
        si (chorda_vacua(pictura) || chorda_aequalis(pictura, currens))
        {
            perge;
        }
        si (!vicus_acervo_mutare(cc->vicus, "pictor",
                chorda_ut_cstr(pictura, piscina)))
        {
            effectus->error = chorda_concatenare(chorda_ex_literis(
                cc->verbum, piscina), chorda_ex_literis(
                ": mutari non potest", piscina), piscina);
        }
        redde VERUM;
    }
    effectus->error = chorda_concatenare(chorda_ex_literis(cc->verbum,
        piscina), chorda_ex_literis(": nulla alia pictura", piscina),
        piscina);
    redde VERUM;
}

/* S3f: $pictor-new (Franus): pictura nova vacua "pictura_<n>" (n
 * primum liberum ab I) in fronte pictoris tabulae activae; nullus
 * pictor: latus novum. PETITIO. */
interior b32
novum_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusCycli* cc;
                        Xar* bibliotheca;
                  character  titulus[XXXII];
                        s32  n;
                        i32  i;
                        b32  liberum;

    cc = (constans ContextusCycli*)ctx;
    si (iussum->numerus_argumentorum > ZEPHYRUM)
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": nulla argumenta", piscina),
            piscina);
        redde VERUM;
    }
    bibliotheca = pictor_documenta_enumerare(cc->vicus->volumen,
        piscina);
    per (n = I; n < M; n++)
    {
        sprintf(titulus, "pictura_%d", (integer)n);
        liberum = VERUM;
        per (i = ZEPHYRUM; bibliotheca && i < xar_numerus(bibliotheca);
             i++)
        {
            si (chorda_aequalis_literis(*(chorda*)xar_obtinere(
                bibliotheca, i), titulus))
            {
                liberum = FALSUM;
            }
        }
        si (liberum)
        {
            frange;
        }
    }
    si (!vicus_acervo_mutare(cc->vicus, "pictor", titulus))
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": condi non potest", piscina),
            piscina);
    }
    redde VERUM;
}

interior b32
cyclum_registrare (
        IussumRegistrum* r,
                  Vicus* v,
     constans character* verbum,
                    s32  directio,
                Piscina* piscina)
{
    ContextusCycli* cc;

    cc = (ContextusCycli*)piscina_allocare(piscina,
        magnitudo(ContextusCycli));
    si (!cc)
    {
        redde FALSUM;
    }
    cc->vicus     = v;
    cc->directio  = directio;
    cc->verbum    = verbum;
    redde iussum_registrare(r, verbum, FALSUM, directio == ZEPHYRUM
        ? novum_iussum : cyclus_iussum, cc);
}

/* T4c: argumentum lateris terminalis "tmux <servitor|-> <sessio>" ->
 * "tmux -u [-S servitor] attach -t =<sessio>" (sessio ultima: spatia
 * licet); aliud aut nullum -> NIHIL (concha). Servitor "-" =
 * ordinarius. */
interior constans character* constans*
argumenta_terminalis (
    constans character* argumentum,
               Piscina* piscina)
{
           Multiplexor  mx;
             character* copia;
             character* servitor;
             character* sessio;

    si (!argumentum || strncmp(argumentum, "tmux ", V) != ZEPHYRUM)
    {
        redde NIHIL;
    }
    copia = (character*)piscina_allocare(piscina, (memoriae_index)
        strlen(argumentum) + I);
    si (!copia)
    {
        redde NIHIL;
    }
    strcpy(copia, argumentum + V);
    servitor  = copia;
    sessio    = strchr(copia, ' ');
    si (!sessio || !sessio[I])
    {
        redde NIHIL;
    }
    *sessio++ = '\0';
    memset(&mx, ZEPHYRUM, magnitudo(mx));
    si (strcmp(servitor, "-") != ZEPHYRUM)
    {
        mx.socket = servitor;
    }
    redde multiplexor_argumenta_adhaerendi(&mx,
        chorda_ex_literis(sessio,
        piscina), piscina);
}

/* terminale (vicus-latera S1c): concha nova in omni apertura (decisio
 * aemulatoris: nihil durabile praeter ramum); volumen non legitur.
 * T4c: argumentum "tmux <servitor> <sessio>" sessioni adhaeret (et
 * post restitutionem iterum). ctx = systemata vici (S7c-3: organum,
 * ut scriba et pictor) */
interior b32
terminale_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)volumen;
    redde terminale_montare((TerminaleApplicatio*)sedes, piscina,
        intern, repo, id, argumenta_terminalis(argumentum, piscina),
        (StatusSystemata*)ctx, latitudo, altitudo);
}

/* pulsus sine mora: ansa hospitis XVI ms ipsa exspectat */
interior VicusPulsus
terminale_pulsare_in_vico (
    vacuum* ctx)
{
    AemulatorHospesPulsus ph;
              VicusPulsus p;

    ph         = terminale_pulsare((TerminaleApplicatio*)ctx, ZEPHYRUM);
    p.mutatum  = ph.mutatum;
    p.finitus  = ph.finitus;
    redde p;
}

interior vacuum
terminale_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    TerminaleApplicatio* m;

    m                  = (TerminaleApplicatio*)montatio;
    f->actiones        = m->actiones;
    f->figurae         = m->figurae;
    f->componere       = terminale_componere;
    f->componere_ctx   = m;
    f->pulsare         = terminale_pulsare_in_vico;
    f->pulsare_ctx     = m;
    f->vivit_in_fundo  = VERUM;
    /* Franus: 'tmux a' statim post aperturam ad concham, non scribam */
    f->focus_in_apertura = VERUM;
}

/* agenda-plan A2: calendarium - documentum agendae UNUM per volumen
 * vici (spatium "agenda"), pigre apertum, commune omnibus lateribus
 * calendarii; hodie ex horologio vici (probationes fictum ponunt) */
nomen structura {
                  Vicus* vicus;
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
       AgendaDocumentum* documentum;
            FastiFontes* fontes;     /* P6: natales nomenclatoris */
        StatusSystemata* systemata;
} ContextusKalendarii;

/* porta 'hodie' calendarii: horologium vici (fons ipse lectionis
 * tempus_* radicis); nullum = FALSUM */
interior b32
hodie_vici (
    vacuum* ctx,
      Dies* hodie)
{
          Vicus* v;
    VicusTempus  t;

    v = (Vicus*)ctx;
    si (!v || !v->horologium)
    {
        redde FALSUM;
    }
    memset(&t, ZEPHYRUM, magnitudo(t));
    v->horologium(v->horologium_ctx, &t);
    *hodie = fasti_dies(t.annus, t.mensis, t.dies);
    redde fasti_dies_valida(*hodie);
}

interior b32
kalendarium_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusKalendarii* ck;

    ck = (ContextusKalendarii*)ctx;
    si (!ck)
    {
        redde FALSUM;
    }
    si (!ck->documentum)
    {
        ck->documentum = agenda_aperire(ck->piscina, ck->intern,
            ck->volumen, "agenda", ZEPHYRUM);
    }
    redde kalendarium_montare((KalendariumMontatio*)sedes, piscina,
        intern, repo, id, (argumentum && argumentum[ZEPHYRUM])
        ? argumentum : NIHIL, ck->documentum, ck->fontes, volumen,
        latitudo,
        altitudo, hodie_vici,
        ck->vicus,
        ck->systemata);
}

interior b32
kalendarii_argumentum_ponere (
                 vacuum* ctx,
     constans character* argumentum)
{
    redde kalendarium_argumentum_ponere((KalendariumMontatio*)ctx,
        argumentum);
}

/* A6e: pulsus lateris calendarii - notae refectae */
interior VicusPulsus
kalendarii_pulsare (
    vacuum* ctx)
{
    VicusPulsus p;

    p.mutatum = kalendarium_reficere((KalendariumMontatio*)ctx);
    p.finitus = FALSUM;
    redde p;
}

interior vacuum
kalendarium_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    KalendariumMontatio* m;

    m                         = (KalendariumMontatio*)montatio;
    f->actiones               = m->actiones;
    f->figurae                = m->figurae;
    f->componere              = kalendarium_componere;
    f->componere_ctx          = m;
    f->argumentum_ponere      = kalendarii_argumentum_ponere;
    f->argumentum_ponere_ctx  = m;
    /* A6d: notae diei - gestus scribae nidificatae */
    f->gestum_ponere  = kalendarium_gestum_ponere;
    f->gestum_ctx     = m;
    /* A6e: pulsus - indicium paginae notarum */
    f->pulsare      = kalendarii_pulsare;
    f->pulsare_ctx  = m;
}

/* nomenclator-plan P2: nomenclator - documentum personarum UNUM per
 * volumen vici (spatium "personae"), pigre apertum, commune omnibus
 * lateribus nomenclatoris */
nomen structura {
                  Vicus* vicus;
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
     PersonaeDocumentum* documentum;
        StatusSystemata* systemata;
} ContextusNomenclatoris;

/* P6c: porta diei nomenclatoris - latus calendarii in frontem acervi,
 * visus diei in eo die (vicus_acervo_mutare "YYYY-MM-DD"; petitio
 * pulsus proximi); ctx = ContextusNomenclatoris* */
interior b32
diem_in_calendario (
     vacuum* ctx,
       Dies  dies)
{
    ContextusNomenclatoris* cn;
                 character  iso[XVI];

    cn = (ContextusNomenclatoris*)ctx;
    si (!cn || !fasti_dies_valida(dies))
    {
        redde FALSUM;
    }
    sprintf(iso, "%04d-%02d-%02d", (integer)dies.annus,
        (integer)dies.mensis, (integer)dies.dies);
    redde vicus_acervo_mutare(cn->vicus, "calendarium", iso);
}

interior b32
nomenclatorem_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusNomenclatoris* cn;

    cn = (ContextusNomenclatoris*)ctx;
    si (!cn)
    {
        redde FALSUM;
    }
    si (!cn->documentum)
    {
        cn->documentum = personae_aperire(cn->piscina, cn->intern,
            cn->volumen, "personae", ZEPHYRUM);
    }
    si (   !cn->documentum
        || !nomenclator_montare((NomenclatorMontatio*)sedes, piscina,
               intern, repo, id, cn->documentum, volumen, hodie_vici,
               cn->vicus, latitudo, altitudo, cn->systemata))
    {
        redde FALSUM;
    }
    nomenclator_portam_diei_ponere((NomenclatorMontatio*)sedes,
        diem_in_calendario, cn);
    /* P6b: argumentum "<id>" personam aperit (malum: index manet) */
    si (argumentum && argumentum[ZEPHYRUM])
    {
        (vacuum)nomenclator_argumentum_ponere(
            (NomenclatorMontatio*)sedes, argumentum);
    }
    redde VERUM;
}

/* P6b: argumentum lateris mutatum (vicus_acervo_mutare) */
interior b32
nomenclatoris_argumentum_ponere (
                 vacuum* ctx,
     constans character* argumentum)
{
    redde nomenclator_argumentum_ponere((NomenclatorMontatio*)ctx,
        argumentum);
}

/* P5b: pulsus lateris nomenclatoris - notae refectae */
interior VicusPulsus
nomenclatoris_pulsare (
    vacuum* ctx)
{
    VicusPulsus p;

    p.mutatum = nomenclator_reficere((NomenclatorMontatio*)ctx);
    p.finitus = FALSUM;
    redde p;
}

interior vacuum
nomenclatorem_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    NomenclatorMontatio* m;

    m                 = (NomenclatorMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = nomenclator_componere;
    f->componere_ctx  = m;
    /* P5b: notae personae - gestus scribae nidificatae, pulsus */
    f->gestum_ponere  = nomenclator_gestum_ponere;
    f->gestum_ctx     = m;
    f->pulsare        = nomenclatoris_pulsare;
    f->pulsare_ctx    = m;
    /* P6b: "<id>" personam aperit (natalis ex calendario) */
    f->argumentum_ponere      = nomenclatoris_argumentum_ponere;
    f->argumentum_ponere_ctx  = m;
}

/* P6: fons dierum calendarii - natales ex documento personarum
 * communi (pigre apertum, ut nomenclatorem_montare_in_vico); ctx =
 * ContextusNomenclatoris* */
interior b32
natales_legere (
      vacuum* ctx,
        Dies  prima,
        Dies  ultima,
         Xar* exitus,
     Piscina* piscina)
{
    ContextusNomenclatoris* cn;

    cn = (ContextusNomenclatoris*)ctx;
    si (!cn)
    {
        redde FALSUM;
    }
    si (!cn->documentum)
    {
        cn->documentum = personae_aperire(cn->piscina, cn->intern,
            cn->volumen, "personae", ZEPHYRUM);
    }
    redde cn->documentum && personae_fasti_legere(cn->documentum, prima,
        ultima, exitus, piscina);
}

/* P6b: introitum natalis aperire - latus nomenclatoris in frontem
 * acervi cum persona (vicus_acervo_mutare; petitio pulsus proximi) */
interior b32
natalem_aperire (
                     vacuum* ctx,
    constans FastiIntroitus* introitus)
{
    ContextusNomenclatoris* cn;
                 character  numerus[XVI];

    cn = (ContextusNomenclatoris*)ctx;
    si (!cn || !introitus || introitus->referentia <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    sprintf(numerus, "%d", (integer)introitus->referentia);
    redde vicus_acervo_mutare(cn->vicus, "nomenclator", numerus);
}

/* folium paginae novae (S2b): lateris sinistri cellulae (dimidium
 * cellulis rotundatum) minus margines; altitudo minus linea tabularum,
 * status, margines - minima ut scriba (XX x X) */
interior i32
folii_columnae (
    i32 latitudo)
{
    s32 n;

    n = ((s32)latitudo / II) / VICUS_CELLULA_LATITUDO - II;
    redde (i32)(n < XX ? XX : n);
}

interior i32
folii_lineae (
    i32 altitudo)
{
    s32 n;

    n = ((s32)altitudo - VICUS_ALTITUDO_TABULARUM)
        / VICUS_CELLULA_ALTITUDO - III;
    redde (i32)(n < X ? X : n);
}


/* ==================================================
 * Admonitiones (colloquia-plan T4a)
 * ================================================== */

/* index communis (centra omnia), sequax (solum post
 * vicus_applicatio_admonitiones_aperire), systema */
nomen structura {
          Admonitiones* index;
    AdmonitionumSequax  sequax;
                   b32  sequitur;     /* sequax initiatus */
                   b32  applicatae;   /* lectae_ad radicis applicatae */
                   s32  non_lectae;   /* scriptum ultimum; -1 nullum */
       StatusSystemata* systemata;
               Piscina* piscina;
                 Vicus* vicus;      /* T4c: saltus ad latus */
               Piscina* scheda;     /* T4c: per saltum vacatur */
} ContextusAdmonitionum;

interior s64
nunc_vici (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde (s64)time(NIHIL) * (s64)M;
}

/* T4c: latus terminalis cuius tty cliens sessionis est; NIHIL si
 * nullum */
interior constans VicusLatus*
latus_adhaerens (
                Vicus* v,
                  Xar* clientes,
               chorda  sessio)
{
                   VicusTabula* t;
                    VicusLatus* l;
    constans MultiplexorCliens* c;
                        chorda  tty;
                           i32  i;
                           s32  k;
                           i32  j;

    per (i = ZEPHYRUM; i < (i32)vicus_numerus_tabularum(v); i++)
    {
        t = vicus_tabula(v, i);
        /* k -1 = sinistrum, deinde acervus */
        per (k = -I; t && k < (s32)xar_numerus(t->acervus); k++)
        {
            l = k < ZEPHYRUM ? &t->sinistrum
                : (VicusLatus*)xar_obtinere(t->acervus, (i32)k);
            si (   !l->montata
                || !chorda_aequalis_literis(l->genus, "terminale"))
            {
                perge;
            }
            tty = terminale_tty((constans TerminaleApplicatio*)
                l->montatio);
            per (j = ZEPHYRUM; tty.mensura > ZEPHYRUM && clientes
                               && j < xar_numerus(clientes); j++)
            {
                c = (constans MultiplexorCliens*)xar_obtinere(clientes,
                    j);
                si (   chorda_aequalis(c->tty, tty)
                    && chorda_aequalis(c->sessio, sessio))
                {
                    redde l;
                }
            }
        }
    }
    redde NIHIL;
}

/* porta actionis: "panellus" -> argumentum "<panellus> <servitor>".
 * T4c: panellus in sessione sua eligitur (select-window +
 * select-pane); latus terminalis vici sessioni adhaerens (quavis
 * tabula) ostenditur et focatur; nullum -> terminale novum in acervo
 * tabulae activae "tmux <servitor> <sessio>" (petitio). */
interior b32
admonitionem_agere (
                vacuum* ctx,
    constans Admonitio* x)
{
           ContextusAdmonitionum* ca;
                     Multiplexor  mx;
    constans MultiplexorPanellus* panellus;
             constans VicusLatus* l;
                       character  argumentum[CCLVI];
                       character* spatium;
                          chorda  sessio;
                          chorda  aperiendum;

    ca = (ContextusAdmonitionum*)ctx;
    si (   !ca || !x || !ca->scheda
        || !chorda_aequalis_literis(x->actio, "panellus")
        || x->argumentum.mensura == ZEPHYRUM
        || x->argumentum.mensura >= CCLVI)
    {
        redde FALSUM;
    }
    piscina_vacare(ca->scheda);
    memcpy(argumentum, x->argumentum.datum, (size_t)
        x->argumentum.mensura);
    argumentum[x->argumentum.mensura] = '\0';
    memset(&mx, ZEPHYRUM, magnitudo(mx));
    spatium = strchr(argumentum, ' ');
    si (spatium)
    {
        *spatium = '\0';
        si (spatium[I])
        {
            mx.socket = spatium + I;
        }
    }
    panellus = multiplexor_panellum_invenire(multiplexor_panelli(&mx,
        ca->scheda), chorda_ex_literis(argumentum, ca->scheda));
    si (!panellus || !ca->vicus)
    {
        redde FALSUM;
    }
    sessio = panellus->sessio;
    (vacuum)multiplexor_panellum_eligere(&mx, panellus->id, ca->scheda);
    l = latus_adhaerens(ca->vicus, multiplexor_clientes(&mx,
        ca->scheda), sessio);
    si (l)
    {
        redde vicus_latus_focare(ca->vicus, chorda_ut_cstr(l->id,
            ca->scheda));
    }
    aperiendum = chorda_concatenare(chorda_ex_literis(mx.socket
        ? "tmux " : "tmux - ", ca->scheda), chorda_ex_literis(mx.socket
        ? mx.socket : "", ca->scheda), ca->scheda);
    si (mx.socket)
    {
        aperiendum = chorda_concatenare(aperiendum, chorda_ex_literis(
            " ", ca->scheda), ca->scheda);
    }
    aperiendum = chorda_concatenare(aperiendum, sessio, ca->scheda);
    redde vicus_acervo_aperire(ca->vicus, "terminale", chorda_ut_cstr(
        aperiendum, ca->scheda));
}

/* systema "admonitiones" (pulsus vici): lectae ex campo radicis semel,
 * liber omni pulsu (legere_ab: fopen et ftell solum nisi crevit), campi
 * radicis cum mutantur */
interior b32
systema_admonitionum (
     Vicus* v,
    vacuum* ctx)
{
    ContextusAdmonitionum* ca;
                   chorda* a;
                   chorda  recentissima;
                      s64  lectae_ad;
                      s32  u;
                character  numerus[XVI];
                      b32  mutatum;

    ca       = (ContextusAdmonitionum*)ctx;
    mutatum  = FALSUM;
    si (!ca->sequitur)
    {
        redde FALSUM;
    }
    si (!ca->applicatae)
    {
        a = insula_attributum(v->repo, "admonitiones_lectae_ad");
        si (a && chorda_ut_s64(*a, &lectae_ad))
        {
            (vacuum)admonitiones_lectas_notare_ad(ca->index, lectae_ad);
        }
        ca->applicatae = VERUM;
    }
    si (   admonitiones_sequi(&ca->sequax, &recentissima) > ZEPHYRUM
        && recentissima.mensura > ZEPHYRUM)
    {
        (vacuum)vicus_radicem_scribere(v, "admonitiones",
            "admonitio_nova", chorda_ut_cstr(recentissima,
            ca->sequax.scheda));
        mutatum = VERUM;
    }
    u = (s32)admonitiones_non_lectae(ca->index);
    si (u != ca->non_lectae)
    {
        sprintf(numerus, "%d", (integer)u);
        (vacuum)vicus_radicem_scribere(v, "admonitiones",
            "admonitiones_non_lectae", numerus);
        ca->non_lectae  = u;
        mutatum         = VERUM;
    }
    redde mutatum;
}

interior b32
admonitiones_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusAdmonitionum* ca;
     AdmonitionumMontatio* m;

    (vacuum)volumen;
    (vacuum)argumentum;
    ca  = (ContextusAdmonitionum*)ctx;
    m   = (AdmonitionumMontatio*)sedes;
    si (   !ca
        || !admonitiones_montare(m, piscina, intern, repo, id,
               ca->index, admonitionem_agere, ca, latitudo, altitudo,
               ca->systemata))
    {
        redde FALSUM;
    }
    m->nunc = nunc_vici;
    redde VERUM;
}

interior vacuum
admonitiones_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    AdmonitionumMontatio* m;

    m                 = (AdmonitionumMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = admonitiones_componere;
    f->componere_ctx  = m;
}

/* T4b: volantes (toasts) - stratum vici, index et porta communes */
interior b32
volantes_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusAdmonitionum* ca;
        VolantiumMontatio* m;

    (vacuum)volumen;
    (vacuum)argumentum;
    ca  = (ContextusAdmonitionum*)ctx;
    m   = (VolantiumMontatio*)sedes;
    si (   !ca
        || !volantes_montare(m, piscina, intern, repo, id, ca->index,
               admonitionem_agere, ca, latitudo, altitudo,
               ca->systemata))
    {
        redde FALSUM;
    }
    m->nunc = nunc_vici;
    redde VERUM;
}

interior VicusPulsus
volantes_pulsare_in_vico (
    vacuum* ctx)
{
    VicusPulsus p;

    p.mutatum = volantes_pulsare((VolantiumMontatio*)ctx);
    p.finitus = FALSUM;
    redde p;
}

interior vacuum
volantes_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    VolantiumMontatio* m;

    m                 = (VolantiumMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = volantes_componere;
    f->componere_ctx  = m;
    f->pulsare        = volantes_pulsare_in_vico;
    f->pulsare_ctx    = m;
}

b32
vicus_applicatio_admonitiones_aperire (
        VicusApplicatio* app,
     constans character* via)
{
    ContextusAdmonitionum* ca;

    si (!app || !app->admonitionum || !via)
    {
        redde FALSUM;
    }
    ca = (ContextusAdmonitionum*)app->admonitionum;
    si (ca->sequitur)
    {
        redde FALSUM;
    }
    ca->sequitur = admonitiones_sequacem_initiare(&ca->sequax,
        ca->piscina, ca->index, via, CCLVI * M);
    redde ca->sequitur;
}


/* ==================================================
 * Applicatio
 * ================================================== */

Volumen*
vicus_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus)
{
    constans character* via_voluminis = NIHIL;
                   s32  i;

    *fumus = FALSUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            *fumus = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-volumen") == ZEPHYRUM
                     && i + I < argc)
        {
            i++;
            via_voluminis = argv[i];
        }
    }
    si (*fumus)
    {
        redde volumen_temporarium(piscina, "vicus_fumus");
    }
    redde volumen_aperire_aut_creare(piscina,
        via_voluminis ? via_voluminis : "vicus.volumen");
}

vacuum
vicus_applicatio_quaestionis_portam_ponere (
                   VicusApplicatio* app,
    constans VicusPortaQuaestionis* porta)
{
    ContextusQuaestionis* cq;

    si (!app || !app->quaestionis)
    {
        redde;
    }
    cq = (ContextusQuaestionis*)app->quaestionis;
    si (   !porta || !porta->incipere || !porta->pulsare
        || !porta->abrumpere)
    {
        memset(cq, ZEPHYRUM, magnitudo(ContextusQuaestionis));
        redde;
    }
    cq->porta   = *porta;
    cq->posita  = VERUM;
}

vacuum
vicus_applicatio_imaginis_portam_ponere (
     VicusApplicatio* app,
        ImagoFructus (*porta)(vacuum* ctx, Piscina* piscina),
              vacuum* ctx)
{
    ContextusPictoris* cp;

    si (!app || !app->pictoris)
    {
        redde;
    }
    cp             = (ContextusPictoris*)app->pictoris;
    cp->porta      = porta;
    cp->porta_ctx  = ctx;
}

b32
vicus_applicatio_aedificare (
        VicusApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo)
{
                chorda  causa;
      ContextusScribae* cs;
     ContextusPictoris* cp;
   ContextusKalendarii* ck;
ContextusNomenclatoris* cn;
  ContextusQuaestionis* cq;
 ContextusAdmonitionum* cad;
       StatusSystemata* systemata;

    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(VicusApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;
    app->vicus    = vicus_creare(piscina, intern, volumen,
        latitudo, altitudo);
    /* S6b: systemata repositorii, communia radici et omnibus
     * lateribus; S7b: vicus_aperire ea creat */
    systemata = (StatusSystemata*)piscina_allocare(piscina,
        magnitudo(StatusSystemata));
    memset(systemata, ZEPHYRUM, magnitudo(StatusSystemata));
    vicus_systemata_ponere(app->vicus, systemata);
    /* S2b: liber paginarum UNUS pro omnibus visibus scribae; paginae
     * novae magnitudine lateris (dimidium cellulis rotundatum, minus
     * margines et status - ut folium S2c) */
    /* S3e: memoria picturarum communis */
    cp = (ContextusPictoris*)piscina_allocare(piscina,
        magnitudo(ContextusPictoris));
    si (cp)
    {
        cp->documenta  = tabula_dispersa_creare_chorda(piscina, XVI);
        cp->porta      = NIHIL;
        cp->porta_ctx  = NIHIL;
        cp->systemata  = systemata;
    }
    app->pictoris = cp;
    /* A2: calendarium - documentum agendae commune */
    ck = (ContextusKalendarii*)piscina_allocare(piscina,
        magnitudo(ContextusKalendarii));
    si (ck)
    {
        memset(ck, ZEPHYRUM, magnitudo(ContextusKalendarii));
        ck->vicus      = app->vicus;
        ck->volumen    = volumen;
        ck->piscina    = piscina;
        ck->intern     = intern;
        ck->systemata  = systemata;
    }
    /* P2: nomenclator - documentum personarum commune */
    cn = (ContextusNomenclatoris*)piscina_allocare(piscina,
        magnitudo(ContextusNomenclatoris));
    si (cn)
    {
        memset(cn, ZEPHYRUM, magnitudo(ContextusNomenclatoris));
        cn->vicus      = app->vicus;
        cn->volumen    = volumen;
        cn->piscina    = piscina;
        cn->intern     = intern;
        cn->systemata  = systemata;
    }
    /* P6: fontes dierum calendarii (natales nomenclatoris) */
    si (ck)
    {
        ck->fontes = fasti_fontes_creare(piscina);
    }
    /* $question: porta nulla donec applicatio fenestrae ponit */
    cq = (ContextusQuaestionis*)piscina_allocare(piscina,
        magnitudo(ContextusQuaestionis));
    si (cq)
    {
        memset(cq, ZEPHYRUM, magnitudo(ContextusQuaestionis));
    }
    app->quaestionis = cq;
    /* T4a: admonitiones - index communis; sequax post
     * vicus_applicatio_admonitiones_aperire */
    cad = (ContextusAdmonitionum*)piscina_allocare(piscina,
        magnitudo(ContextusAdmonitionum));
    si (cad)
    {
        memset(cad, ZEPHYRUM, magnitudo(ContextusAdmonitionum));
        cad->index       = admonitiones_creare(piscina, CC);
        cad->non_lectae  = -I;
        cad->systemata   = systemata;
        cad->piscina     = piscina;
        cad->vicus       = app->vicus;
        cad->scheda      = piscina_generare_dynamicum(
            "admonitiones_saltus", XVI * M);
    }
    app->admonitionum = cad;
    cs = (ContextusScribae*)piscina_allocare(piscina,
        magnitudo(ContextusScribae));
    cs->systemata = systemata;
    cs->liber = scriba_liber_aperire(piscina, intern, volumen,
        folii_columnae(latitudo), folii_lineae(altitudo));
    /* S3b: iussa omnium visuum scribae */
    cs->iussa = iussum_registrum_creare(piscina);
    si (   !app->vicus || !cs->liber || !cs->iussa
        || !cq
        || !iussum_registrare(cs->iussa, "question", FALSUM,
               quaestionem_iussum, cq)
        || !iussum_registrare(cs->iussa, "dies", VERUM, dies_iussum,
               NIHIL)
        || !aperiens_registrare(cs->iussa, app->vicus, "terminale",
               piscina)
        || !aperiens_registrare(cs->iussa, app->vicus, "scriba",
               piscina)
        || !aperiens_registrare(cs->iussa, app->vicus, "pictor",
               piscina)
        || !aperiens_registrare(cs->iussa, app->vicus, "calendarium",
               piscina)
        || !aperiens_fixum_registrare(cs->iussa, app->vicus, "agenda",
               "calendarium", "agenda", piscina)
        || !aperiens_fixum_registrare(cs->iussa, app->vicus, "contacts",
               "nomenclator", NIHIL, piscina)
        || !cad || !cad->index || !cad->scheda
        || !aperiens_fixum_registrare(cs->iussa, app->vicus,
               "notifications", "admonitiones", NIHIL, piscina)
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-next", I,
               piscina)
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-prev", -I,
               piscina)
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-new",
               ZEPHYRUM, piscina)
        || !vicus_genus_addere(app->vicus, "scriba",
               magnitudo(ScribaMontatio), scribam_montare,
               scribam_describere, cs)
        || !vicus_genus_addere(app->vicus, "pictor",
               magnitudo(PictorMontatio), pictorem_montare,
               pictorem_describere, cp)
        || !vicus_genus_addere(app->vicus, "terminale",
               magnitudo(TerminaleApplicatio),
               terminale_montare_in_vico, terminale_describere,
               systemata)
        || !ck
        || !vicus_genus_addere(app->vicus, "calendarium",
               magnitudo(KalendariumMontatio),
               kalendarium_montare_in_vico, kalendarium_describere, ck)
        || !cn
        || !ck->fontes
        || !fasti_fontes_addere(ck->fontes, "nomenclator",
               natales_legere, natalem_aperire, cn)
        || !vicus_genus_addere(app->vicus, "nomenclator",
               magnitudo(NomenclatorMontatio),
               nomenclatorem_montare_in_vico, nomenclatorem_describere,
               cn)
        || !vicus_genus_addere(app->vicus, "admonitiones",
               magnitudo(AdmonitionumMontatio),
               admonitiones_montare_in_vico, admonitiones_describere,
               cad)
        || !vicus_genus_addere(app->vicus, "volantes",
               magnitudo(VolantiumMontatio), volantes_montare_in_vico,
               volantes_describere, cad)
        || !vicus_systema_addere(app->vicus, "admonitiones",
               systema_admonitionum, cad)
        || !vicus_aperire(app->vicus, INDEX_ORDINARIUS)
        || !vicus_stratum_addere(app->vicus, "volantes"))
    {
        causa = app->vicus ? vicus_causa(app->vicus)
                           : chorda_ex_literis("vicus_creare", piscina);
        fprintf(stderr, "vicus: aperiri non potuit: %.*s\n",
            (int)causa.mensura, causa.datum);
        redde FALSUM;
    }
    /* tabula praeterita (genus ignotum, montatio deficiens): nominatur,
     * ceterae currunt */
    causa = vicus_causa(app->vicus);
    si (causa.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "vicus: %.*s\n", (int)causa.mensura,
            causa.datum);
    }
    app->d = dispensator_creare(piscina, intern, app->vicus->repo,
        vicus_actiones(app->vicus), vicus_componere, app->vicus, CCC);
    si (!app->d)
    {
        redde FALSUM;
    }
    vicus_dispensatorem_ligare(app->vicus, app->d);
    /* S5c: dimissio in via eventus */
    si (systemata->dimissio)
    {
        dimissio_dispensatori_ligare(systemata->dimissio, app->d);
    }
    redde VERUM;
}
