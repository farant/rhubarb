/* sarcina.c - arca ZIP solum legenda (include/sarcina.h) */
#include "sarcina.h"
#include "flatura.h"
#include "xar.h"

#include <stdio.h>
#include <string.h>

#define SIGNUM_FINIS       0x06054b50u
#define SIGNUM_CENTRALE   0x02014b50u
#define SIGNUM_LOCALE     0x04034b50u
#define FINIS_MENSURA      XXII
#define CENTRALE_MENSURA  XLVI
#define COMMENTUM_MAXIMUM 0xFFFFu
#define MENSURA_AMPLA      0xFFFFFFFFu

/* introitus cum vexillis privatis (bit 0 cryptographia, bit III
 * descriptor datorum); introitus publicus primus - monstrator idem */
nomen structura {
    SarcinaIntroitus publicus;
                 i32 vexilla;
} IntroitusInternus;

structura Sarcina {
    chorda  datum;
       Xar* introitus;   /* IntroitusInternus */
};

/* binos / quaternos octetos (little-endian) ad positionem, fines
 * probati; FALSUM si extra arcam */
interior b32
binos_legere (
    chorda  d,
       i32  positio,
       i32* valor)
{
    si (positio > d.mensura || d.mensura - positio < II)
    {
        redde FALSUM;
    }
    *valor = (i32)d.datum[positio] | ((i32)d.datum[positio + I]
        << VIII);
    redde VERUM;
}

interior b32
quaternos_legere (
    chorda  d,
       i32  positio,
       i32* valor)
{
    i32 a;
    i32 b;

    si (!binos_legere(d, positio, &a) || !binos_legere(d, positio + II,
            &b))
    {
        redde FALSUM;
    }
    *valor = a | (b << XVI);
    redde VERUM;
}

interior Sarcina*
recusare (
     constans character* nuntium,
                Piscina* piscina,
                 chorda* vitium)
{
    si (vitium)
    {
        *vitium = chorda_ex_literis(nuntium, piscina);
    }
    redde NIHIL;
}

/* EOCD retro quaeritur: ab ultimo loco possibili usque ad commentum
 * maximum; -1 (ut s32) si nullum */
interior s32
finem_directorii_quaerere (
    chorda d)
{
    i32 positio;
    i32 limes;
    i32 signum;

    si (d.mensura < FINIS_MENSURA)
    {
        redde -I;
    }
    positio  = d.mensura - FINIS_MENSURA;
    limes    = positio > COMMENTUM_MAXIMUM ? positio - COMMENTUM_MAXIMUM
             : ZEPHYRUM;
    per (;;)
    {
        si (   quaternos_legere(d, positio, &signum)
            && signum == SIGNUM_FINIS)
        {
            redde (s32)positio;
        }
        si (positio == limes)
        {
            frange;
        }
        positio--;
    }
    redde -I;
}

Sarcina*
sarcina_aperire (
      chorda  datum,
     Piscina* piscina,
      chorda* vitium)
{
              Sarcina* s;
    IntroitusInternus* e;
                  s32  finis;
                  i32  numerus;
                  i32  magnitudo_directorii;
                  i32  initium;
                  i32  positio;
                  i32  signum;
                  i32  longitudo_nominis;
                  i32  longitudo_extra;
                  i32  longitudo_commenti;
                  i32  i;

    si (!piscina || !datum.datum)
    {
        redde recusare("not a ZIP archive (empty)", piscina, vitium);
    }
    finis = finem_directorii_quaerere(datum);
    si (finis < ZEPHYRUM)
    {
        redde recusare("not a ZIP archive (no end of central "
            "directory)", piscina, vitium);
    }
    si (   !binos_legere(datum, (i32)finis + X, &numerus)
        || !quaternos_legere(datum, (i32)finis + XII,
        &magnitudo_directorii)
        || !quaternos_legere(datum, (i32)finis + XVI, &initium))
    {
        redde recusare("truncated end of central directory", piscina,
            vitium);
    }
    si (numerus == 0xFFFFu || initium == MENSURA_AMPLA)
    {
        redde recusare("ZIP64 archives are not supported", piscina,
            vitium);
    }
    si (initium > datum.mensura || magnitudo_directorii > datum.mensura
        - initium)
    {
        redde recusare("central directory lies outside the archive",
            piscina, vitium);
    }
    s = (Sarcina*)piscina_allocare(piscina, magnitudo(Sarcina));
    si (!s)
    {
        redde recusare("out of memory", piscina, vitium);
    }
    s->datum = datum;
    s->introitus = xar_creare(piscina,
        (i32)magnitudo(IntroitusInternus));
    positio = initium;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        e = (IntroitusInternus*)xar_addere(s->introitus);
        si (   !e || !quaternos_legere(datum, positio, &signum)
            || signum != SIGNUM_CENTRALE)
        {
            redde recusare("bad central directory record", piscina,
                vitium);
        }
        memset(e, ZEPHYRUM, magnitudo(IntroitusInternus));
        si (   !binos_legere(datum, positio + VIII, &e->vexilla)
            || !binos_legere(datum, positio + X, &e->publicus.modus)
            || !quaternos_legere(datum, positio + XVI, &e->publicus.crc)
            || !quaternos_legere(datum, positio + XX,
                   &e->publicus.mensura_compressa)
            || !quaternos_legere(datum, positio + XXIV,
                   &e->publicus.mensura)
            || !binos_legere(datum, positio + XXVIII,
            &longitudo_nominis)
            || !binos_legere(datum, positio + XXX, &longitudo_extra)
            || !binos_legere(datum, positio + XXXII,
            &longitudo_commenti)
            || !quaternos_legere(datum, positio + XLII,
                   &e->publicus.positio))
        {
            redde recusare("truncated central directory record",
                piscina,
                vitium);
        }
        si (   e->publicus.mensura_compressa == MENSURA_AMPLA
            || e->publicus.mensura           == MENSURA_AMPLA
            || e->publicus.positio           == MENSURA_AMPLA)
        {
            redde recusare("ZIP64 entries are not supported", piscina,
                vitium);
        }
        si (   positio + CENTRALE_MENSURA > datum.mensura
            || longitudo_nominis > datum.mensura - positio
               - CENTRALE_MENSURA)
        {
            redde recusare("entry name lies outside the archive",
                piscina,
                vitium);
        }
        e->publicus.titulus.datum    = datum.datum + positio
                                     + CENTRALE_MENSURA;
        e->publicus.titulus.mensura  = longitudo_nominis;
        e->publicus.directorium      = longitudo_nominis > ZEPHYRUM
            && e->publicus.titulus.datum[longitudo_nominis - I] == '/';
        positio += CENTRALE_MENSURA + longitudo_nominis
            + longitudo_extra
                 + longitudo_commenti;
    }
    redde s;
}

i32
sarcina_numerus (
    constans Sarcina* s)
{
    redde s ? xar_numerus(s->introitus) : ZEPHYRUM;
}

constans SarcinaIntroitus*
sarcina_introitus (
    constans Sarcina* s,
                 i32  k)
{
    si (!s || k >= xar_numerus(s->introitus))
    {
        redde NIHIL;
    }
    redde &((constans IntroitusInternus*)xar_obtinere(s->introitus, k))
        ->publicus;
}

constans SarcinaIntroitus*
sarcina_invenire (
    constans Sarcina* s,
              chorda  titulus)
{
    constans SarcinaIntroitus* e;
                          i32  i;

    per (i = ZEPHYRUM; i < sarcina_numerus(s); i++)
    {
        e = sarcina_introitus(s, i);
        si (chorda_aequalis(e->titulus, titulus))
        {
            redde e;
        }
    }
    redde NIHIL;
}

/* data introitus: caput locale (signum, longitudines nominis et extra
 * LOCALES - possunt a centralibus differre), data post id; mensurae
 * et CRC SEMPER ex directorio (descriptor datorum: caput locale 0) */
b32
sarcina_legere (
             constans Sarcina* s,
    constans SarcinaIntroitus* e,
                      Piscina* piscina,
                       chorda* exitus,
                       chorda* vitium)
{
    constans IntroitusInternus* ii;
                           i32  signum;
                           i32  nominis;
                           i32  extra;
                           i32  initium;
                FlaturaFructus  f;
                     character  nuntium[LXIV];
                            i8* copia;

    si (!s || !e || !piscina || !exitus)
    {
        (vacuum)recusare("missing argument", piscina, vitium);
        redde FALSUM;
    }
    ii = (constans IntroitusInternus*)e;
    si (ii->vexilla & I)
    {
        (vacuum)recusare("encrypted entries are not supported", piscina,
            vitium);
        redde FALSUM;
    }
    si (e->modus != ZEPHYRUM && e->modus != VIII)
    {
        sprintf(nuntium, "unsupported compression method %u",
            (insignatus integer)e->modus);
        (vacuum)recusare(nuntium, piscina, vitium);
        redde FALSUM;
    }
    si (   !quaternos_legere(s->datum, e->positio, &signum)
        || signum != SIGNUM_LOCALE
        || !binos_legere(s->datum, e->positio + XXVI, &nominis)
        || !binos_legere(s->datum, e->positio + XXVIII, &extra))
    {
        (vacuum)recusare("bad local header", piscina, vitium);
        redde FALSUM;
    }
    initium = e->positio + XXX + nominis + extra;
    si (   initium > s->datum.mensura
        || e->mensura_compressa > s->datum.mensura - initium)
    {
        (vacuum)recusare("entry data lies outside the archive", piscina,
            vitium);
        redde FALSUM;
    }
    si (e->modus == ZEPHYRUM)
    {
        si (e->mensura_compressa != e->mensura)
        {
            (vacuum)recusare("stored entry size mismatch", piscina,
                vitium);
            redde FALSUM;
        }
        copia = (i8*)piscina_allocare(piscina, e->mensura + I);
        si (!copia)
        {
            (vacuum)recusare("out of memory", piscina, vitium);
            redde FALSUM;
        }
        memcpy(copia, s->datum.datum + initium, (size_t)e->mensura);
        exitus->datum    = copia;
        exitus->mensura  = e->mensura;
    }
    alioquin
    {
        f = flatura_inflare(s->datum.datum + initium,
            e->mensura_compressa, piscina);
        si (f.status != FLATURA_STATUS_OK)
        {
            (vacuum)recusare("corrupt DEFLATE data", piscina, vitium);
            redde FALSUM;
        }
        si (f.mensura != e->mensura)
        {
            (vacuum)recusare("inflated size mismatch", piscina, vitium);
            redde FALSUM;
        }
        exitus->datum    = f.datum;
        exitus->mensura  = f.mensura;
    }
    si (flatura_crc32(exitus->datum, exitus->mensura) != e->crc)
    {
        (vacuum)recusare("CRC mismatch", piscina, vitium);
        redde FALSUM;
    }
    redde VERUM;
}
