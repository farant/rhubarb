/* credentialia.c - tesserae per personam (vide include/credentialia.h)
 *
 * Loculus (CredentialiaOpus) vivit a verificatione coepta usque ad
 * reditum - aut, si re-hash debetur, usque ad tesseram novam positam:
 * tessera copiata tunc adhuc necessaria est (exemplar :796-801).
 * Liberatio tesseram delet (memset).
 */

#include "postulata_posix.h"
#include "credentialia.h"
#include "json.h"
#include "scrinium.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define CLAVIS_MAXIMA LXXX
#define PAR_MAXIMUM   LX

nomen enumeratio {
    OPUS_LIBERUM = ZEPHYRUM,
    OPUS_MORANS,        /* timer freni */
    OPUS_VERIFICANS,
    OPUS_RENOVANS       /* re-hash post bonam */
} OpusStatus;

nomen structura {
           Credentialia* credentialia;
             OpusStatus  status;
                     i8  tessera[CUSTODIA_TESSERA_MAXIMA];
                    i32  tn;
              character  nomen_usoris[CIVES_NOMEN_MAXIMUM + I];
                    i32  nn;
              character  clavis_paris[CLAVIS_MAXIMA];
              character  clavis_usoris[CLAVIS_MAXIMA];
              character  civis[CIVES_ID_MENSURA + I];
                    i32  cn;     /* 0 = nullus civis activus */
    CredentialiaReditus  reditus;
                 vacuum* datum;
} CredentialiaOpus;

structura Credentialia {
             Volumen* volumen;
            Scrinium* scrinium;
               Cives* cives;
            Custodia* custodia;
             Reactor* reactor;
                  s64  (*nunc_ms)(vacuum);
    CredentialiaOpus opera[CREDENTIALIA_OPERA];
};

hic_manens constans character* constans SCHEMA[] = {
    "CREATE TABLE IF NOT EXISTS credentialia"
    " (id TEXT PRIMARY KEY, civis TEXT NOT NULL, genus TEXT NOT NULL,"
    " phc TEXT NOT NULL, creatum_ms INTEGER NOT NULL,"
    " ultimus_usus_ms INTEGER NOT NULL)",
    "CREATE INDEX IF NOT EXISTS credentialia_civis"
    " ON credentialia (civis, genus)",
    NIHIL
};


/* ==================================================
 * auxilia
 * ================================================== */

interior s64
_tempus_verum_ms (vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

interior s64
_nunc (
    Credentialia* cr)
{
    redde cr->nunc_ms ? cr->nunc_ms() : _tempus_verum_ms();
}

interior chorda
_ch (
    constans character* s)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)s;
    c.mensura  = (i32)strlen(s);
    redde c;
}

interior vacuum
_opus_liberare (
    CredentialiaOpus* o)
{
    memset(o->tessera, ZEPHYRUM, magnitudo(o->tessera));
    o->tn      = ZEPHYRUM;
    o->status  = OPUS_LIBERUM;
}

/* phc tesserae civis (piscina) aut vacua */
interior chorda
_phc (
    Credentialia* cr,
          chorda  civis,
         Piscina* p)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(cr->scrinium,
        "SELECT phc FROM credentialia WHERE civis = ? AND genus = ?");
                 chorda phc = chorda_ex_literis("", p);

    si (!e)
    {
        redde phc;
    }
    scrinium_ligare_textum(e, I, civis);
    scrinium_ligare_textum(e, II, _ch(CREDENTIALE_TESSERA));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        phc = scrinium_columna_textus(e, ZEPHYRUM, p);
    }
    scrinium_finire(e);
    redde phc;
}


/* ==================================================
 * creatio, digestio, positio
 * ================================================== */

Credentialia*
credentialia_aperire (
     Volumen* volumen,
       Cives* cives,
    Custodia* custodia,
     Reactor* reactor,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina)
{
    Credentialia* cr;
             i32  k;

    si (!volumen || !cives || !custodia || !reactor || !piscina)
    {
        redde NIHIL;
    }
    cr = (Credentialia*)piscina_allocare(piscina,
        (i64)magnitudo(Credentialia));
    si (!cr)
    {
        redde NIHIL;
    }
    memset(cr, ZEPHYRUM, magnitudo(*cr));
    cr->volumen   = volumen;
    cr->scrinium  = volumen_scrinium(volumen);
    cr->cives     = cives;
    cr->custodia  = custodia;
    cr->reactor   = reactor;
    cr->nunc_ms   = nunc_ms;
    per (k = ZEPHYRUM; SCHEMA[k]; k++)
    {
        si (!cr->scrinium || !scrinium_exsequi(cr->scrinium, SCHEMA[k]))
        {
            redde NIHIL;
        }
    }
    redde cr;
}

b32
credentialia_digerere (
           Credentialia* credentialia,
                 chorda  tessera,
    CustodiaHashReditus  reditus,
                 vacuum* datum)
{
    si (!credentialia || !reditus)
    {
        redde FALSUM;
    }
    redde custodia_tesseram_digerere(credentialia->custodia, tessera,
        reditus, datum);
}

b32
credentialia_tesseram_ponere (
    Credentialia* credentialia,
          chorda  civis,
          chorda  phc)
{
               Piscina* p;
                 Civis  x;
             character  ulid[SCRINIUM_ULID_MENSURA];
    ScriniumEnuntiatum* e;
             JsonValor* d;
                   b32  bene = FALSUM;

    si (!credentialia || !civis.datum || phc.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    p = piscina_generare_dynamicum("credentialia_ponere", MXXIV);
    si (!p)
    {
        redde FALSUM;
    }
    si (!cives_per_id(credentialia->cives, civis, &x, p))
    {
        piscina_destruere(p);
        redde FALSUM;
    }
    e = scrinium_praeparare(credentialia->scrinium,
        "DELETE FROM credentialia WHERE civis = ? AND genus = ?");
    si (e)
    {
        scrinium_ligare_textum(e, I, civis);
        scrinium_ligare_textum(e, II, _ch(CREDENTIALE_TESSERA));
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    e = bene ? scrinium_praeparare(credentialia->scrinium,
        "INSERT INTO credentialia (id, civis, genus, phc, creatum_ms,"
        " ultimus_usus_ms) VALUES (?, ?, ?, ?, ?, 0)") : NIHIL;
    bene = FALSUM;
    si (e)
    {
        scrinium_ulid(ulid);
        scrinium_ligare_textum(e, I, _ch(ulid));
        scrinium_ligare_textum(e, II, civis);
        scrinium_ligare_textum(e, III, _ch(CREDENTIALE_TESSERA));
        scrinium_ligare_textum(e, IV, phc);
        scrinium_ligare_numerum(e, V, _nunc(credentialia));
        bene = scrinium_gradi(e) == SCRINIUM_FACTUM;
        scrinium_finire(e);
    }
    si (bene)
    {
        d = json_objectum_creare(p);
        json_objectum_ponere(d, "civis", json_chorda_creare(p, civis));
        json_objectum_ponere(d, "genus", json_chorda_creare(p,
            _ch(CREDENTIALE_TESSERA)));
        bene = volumen_actum_appendere(credentialia->volumen,
            "credentiale.positum", json_scribere(d, p)) > ZEPHYRUM;
    }
    piscina_destruere(p);
    redde bene;
}


/* ==================================================
 * verificatio
 * ================================================== */

/* re-hash factum: tessera nova in transactione propria */
interior vacuum
_renovatio_facta (
    vacuum* datum,
    chorda  phc)
{
    CredentialiaOpus* o   = (CredentialiaOpus*)datum;
        Credentialia* cr  = o->credentialia;

    si (   phc.mensura > ZEPHYRUM && volumen_transactionem_incipere(
        cr->volumen))
    {
        si (credentialia_tesseram_ponere(cr, chorda_ex_buffer(
            (i8*)o->civis, o->cn), phc))
        {
            (vacuum)volumen_transactionem_committere(cr->volumen);
        }
        alioquin
        {
            (vacuum)volumen_transactionem_revolvere(cr->volumen);
        }
    }
    _opus_liberare(o);
}

interior vacuum
_usum_notare (
    Credentialia* cr,
          chorda  civis)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(cr->scrinium,
        "UPDATE credentialia SET ultimus_usus_ms = ?"
        " WHERE civis = ? AND genus = ?");

    si (e)
    {
        scrinium_ligare_numerum(e, I, _nunc(cr));
        scrinium_ligare_textum(e, II, civis);
        scrinium_ligare_textum(e, III, _ch(CREDENTIALE_TESSERA));
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
}

interior vacuum
_verificatio_facta (
    vacuum* datum,
       b32  bona,
       b32  renovanda)
{
     CredentialiaOpus* o     = (CredentialiaOpus*)datum;
         Credentialia* cr    = o->credentialia;
                  s64  nunc  = _nunc(cr);
               chorda  civis;

    /* sine civi activo phc vacua fuit: bona iam FALSUM (ficticium) */
    bona   = bona && o->cn > ZEPHYRUM;
    civis  = bona ? chorda_ex_buffer((i8*)o->civis, o->cn)
        : chorda_ex_buffer((i8*)o->civis, ZEPHYRUM);
    custodia_frenum_notare(cr->custodia, _ch(o->clavis_paris), bona,
        nunc);
    custodia_frenum_notare(cr->custodia, _ch(o->clavis_usoris), bona,
        nunc);
    si (bona)
    {
        _usum_notare(cr, civis);
    }
    o->reditus(o->datum, bona, civis);
    si (   bona && renovanda && custodia_tesseram_digerere(cr->custodia,
        chorda_ex_buffer(o->tessera, o->tn), _renovatio_facta, o))
    {
        o->status = OPUS_RENOVANS;
        redde;   /* loculus manet usque ad renovationem */
    }
    _opus_liberare(o);
}

/* civem quaerere et verificationem incipere; FALSUM = cauda plena */
interior b32
_incipere (
    CredentialiaOpus* o)
{
    Credentialia* cr = o->credentialia;
         Piscina* p  = piscina_generare_dynamicum("credentialia_verif",
                           DXII);
           Civis x;
          chorda phc;
             b32 bene;

    si (!p)
    {
        redde FALSUM;
    }
    o->cn  = ZEPHYRUM;
    phc    = chorda_ex_literis("", p);
    si (   cives_per_nomen(cr->cives, chorda_ex_buffer(
               (i8*)o->nomen_usoris, o->nn), &x, p)
        && x.status     == CIVIS_ACTIVUS
        && x.id.mensura <= CIVES_ID_MENSURA)
    {
        memcpy(o->civis, x.id.datum, (size_t)x.id.mensura);
        o->civis[x.id.mensura]  = '\0';
        o->cn                   = x.id.mensura;
        phc                     = _phc(cr, x.id, p);
    }
    o->status  = OPUS_VERIFICANS;
    bene       = custodia_tesseram_verificare(cr->custodia, phc,
        chorda_ex_buffer(o->tessera, o->tn), _verificatio_facta, o);
    piscina_destruere(p);
    redde bene;
}

/* timer freni finitus: verificatio nunc incipit */
interior vacuum
_mora_finita (
    vacuum* datum)
{
    CredentialiaOpus* o = (CredentialiaOpus*)datum;

    si (!_incipere(o))
    {
        /* cauda custodiae plena post moram: refutatio ut falsa (rara) */
        o->reditus(o->datum, FALSUM, chorda_ex_buffer((i8*)o->civis,
            ZEPHYRUM));
        _opus_liberare(o);
    }
}

CredentialiaInitium
credentialia_verificare (
           Credentialia* credentialia,
                 chorda  nomen_usoris,
                 chorda  tessera,
                 chorda  clavis_paris,
    CredentialiaReditus  reditus,
                 vacuum* datum)
{
     CredentialiaOpus* o = NIHIL;
            character  clavis_paris_l[CLAVIS_MAXIMA];
            character  clavis_usoris_l[CLAVIS_MAXIMA];
                  s64  nunc;
                  s32  mora_a;
                  s32  mora_b;
                  i32  k;

    si (   !credentialia || !reditus
        || tessera.mensura > CUSTODIA_TESSERA_MAXIMA
        || nomen_usoris.mensura > CIVES_NOMEN_MAXIMUM
        || clavis_paris.mensura > PAR_MAXIMUM)
    {
        redde CREDENTIALIA_PRAVUM;
    }
    sprintf(clavis_paris_l, "par:%.*s", (integer)clavis_paris.mensura,
        clavis_paris.datum ? (constans character*)clavis_paris.datum : "");
    sprintf(clavis_usoris_l, "usor:%.*s", (integer)nomen_usoris.mensura,
        nomen_usoris.datum ? (constans character*)nomen_usoris.datum : "");
    nunc    = _nunc(credentialia);
    mora_a  = custodia_frenum(credentialia->custodia,
        _ch(clavis_paris_l),
        nunc);
    mora_b  = custodia_frenum(credentialia->custodia,
        _ch(clavis_usoris_l), nunc);
    si (mora_a < ZEPHYRUM || mora_b < ZEPHYRUM)
    {
        redde CREDENTIALIA_FRENATUM;
    }
    per (k = ZEPHYRUM; k < CREDENTIALIA_OPERA; k++)
    {
        si (credentialia->opera[k].status == OPUS_LIBERUM)
        {
            o = &credentialia->opera[k];
            frange;
        }
    }
    si (!o)
    {
        redde CREDENTIALIA_OCCUPATUM;
    }
    memset(o, ZEPHYRUM, magnitudo(*o));
    o->credentialia  = credentialia;
    o->reditus       = reditus;
    o->datum         = datum;
    o->tn            = tessera.mensura;
    si (tessera.mensura > ZEPHYRUM)
    {
        memcpy(o->tessera, tessera.datum, (size_t)tessera.mensura);
    }
    o->nn = nomen_usoris.mensura;
    si (nomen_usoris.mensura > ZEPHYRUM)
    {
        memcpy(o->nomen_usoris, nomen_usoris.datum,
            (size_t)nomen_usoris.mensura);
    }
    strcpy(o->clavis_paris, clavis_paris_l);
    strcpy(o->clavis_usoris, clavis_usoris_l);
    si (mora_a > ZEPHYRUM || mora_b > ZEPHYRUM)
    {
        /* mora DIFFERTUR (timer), numquam obstat */
        o->status = OPUS_MORANS;
        (vacuum)reactor_timer_adicere(credentialia->reactor,
            (i32)(mora_a > mora_b ? mora_a : mora_b), FALSUM,
            _mora_finita,
            o);
        redde CREDENTIALIA_COEPTUM;
    }
    si (!_incipere(o))
    {
        _opus_liberare(o);
        redde CREDENTIALIA_OCCUPATUM;
    }
    redde CREDENTIALIA_COEPTUM;
}
