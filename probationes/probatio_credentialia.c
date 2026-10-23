/* probatio_credentialia.c - credentialia (ianua T2): tesserae per
 * personam - digerere/ponere/verificare, persona ignota et non activa
 * (opus plenum ficticium, tempus idem), frenum (mora per timer,
 * numquam obstat; recusatio sine reditu), opera plena, tessera nimis
 * longa, re-hash parametrorum veterum, una tessera per personam, nulla
 * tessera nec digestum in actis.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "custodia.h"
#include "scrinium.h"
#include "volumen.h"
#include "xar.h"
#include "cives.h"
#include "credentialia.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define VICES_MAXIMAE (C * M)
#define TESSERA_BONA  "tessera-bona"

hic_manens s64 g_nunc = (s64)1760000000 * M;

interior s64
_nunc (vacuum)
{
    redde g_nunc;
}

interior s64
_ms_vera (vacuum)
{
    structura timeval tv;

    (vacuum)gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* ---- mundus probationis ---- */

nomen structura {
         Reactor* r;
        Custodia* cu;
         Volumen* v;
           Cives* cv;
    Credentialia* cr;
} Mundus;

interior Custodia*
_custodia (
    CredoContextus* c,
           Reactor* r,
               i32  transitus,
               i32  frenum_conatus)
{
    CustodiaConfiguratio cfg;
    /* parametri viles (transitus, KiB, lanae, longitudo): celeritas */
    hic_manens constans Argon2idParametri VILES_I   = { I, LXIV, I,
        XXXII, NIHIL, ZEPHYRUM, NIHIL, ZEPHYRUM };
    hic_manens constans Argon2idParametri VILES_II  = { II, LXIV, I,
        XXXII, NIHIL, ZEPHYRUM, NIHIL, ZEPHYRUM };

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    memset(cfg.secretum, VII, CUSTODIA_SECRETUM);
    cfg.tessera_parametri  = transitus == II ? VILES_II : VILES_I;
    cfg.frenum_conatus     = frenum_conatus;
    redde custodia_creare(&cfg, r, c->piscina);
}

interior b32
_mundus (
    CredoContextus* c,
            Mundus* m,
               i32  frenum_conatus)
{
    memset(m, ZEPHYRUM, magnitudo(*m));
    m->r   = reactor_creare(c->piscina);
    m->cu  = m->r ? _custodia(c, m->r, I, frenum_conatus) : NIHIL;
    m->v   = volumen_temporarium(c->piscina, "credentialia");
    m->cv  = m->v ? cives_aperire(m->v, _nunc, c->piscina) : NIHIL;
    m->cr  = m->cv && m->cu ? credentialia_aperire(m->v, m->cv, m->cu,
        m->r, _nunc, c->piscina) : NIHIL;
    redde m->cr != NIHIL;
}

interior vacuum
_mundum_claudere (
    Mundus* m)
{
    si (m->v)
    {
        volumen_claudere(m->v);
    }
    si (m->r)
    {
        reactor_destruere(m->r);
    }
}

/* ---- reditus ---- */

nomen structura {
          i32  hashes;
          i32  verificationes;
          b32  bona;
    character  civis[CIVES_ID_MENSURA + I];
       chorda  phc;
      Piscina* p;
} Exitus;

interior vacuum
_hash_venit (
     vacuum* datum,
     chorda  phc)
{
    Exitus* e = (Exitus*)datum;

    e->hashes++;
    e->phc.mensura  = phc.mensura;
    e->phc.datum    = (i8*)piscina_allocare(e->p, (i64)phc.mensura + I);
    si (phc.mensura > ZEPHYRUM)
    {
        memcpy(e->phc.datum, phc.datum, (size_t)phc.mensura);
    }
}

interior vacuum
_verificatio_venit (
     vacuum* datum,
        b32  bona,
     chorda  civis)
{
    Exitus* e = (Exitus*)datum;

    e->verificationes++;
    e->bona = bona;
    memset(e->civis, ZEPHYRUM, magnitudo(e->civis));
    si (civis.mensura > ZEPHYRUM && civis.mensura <= CIVES_ID_MENSURA)
    {
        memcpy(e->civis, civis.datum, (size_t)civis.mensura);
    }
}

interior b32
_currere (
    Reactor* r,
        i32* signum,
        i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < VICES_MAXIMAE && *signum < n; k++)
    {
        reactor_poll(r, I);
    }
    redde *signum >= n;
}

/* civis cum tessera (transactiones propriae); id in piscina */
interior b32
_civem (
                 Mundus* m,
     constans character* nomen_usoris,
     constans character* tessera,
                 chorda* id,
                Piscina* p)
{
    Exitus e;
       b32 bene;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = p;
    si (   !volumen_transactionem_incipere(m->v)
        || !cives_creare(m->cv, _c(nomen_usoris, p), _c(nomen_usoris,
               p), id, p)
        || !volumen_transactionem_committere(m->v))
    {
        redde FALSUM;
    }
    si (   !credentialia_digerere(m->cr, _c(tessera, p), _hash_venit,
        &e)
        || !_currere(m->r, &e.hashes, I) || e.phc.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!volumen_transactionem_incipere(m->v))
    {
        redde FALSUM;
    }
    bene = credentialia_tesseram_ponere(m->cr, *id, e.phc);
    redde (bene && volumen_transactionem_committere(m->v)) || (
        volumen_transactionem_revolvere(m->v) && FALSUM);
}

/* verificatio tota: initium + reditus */
interior CredentialiaInitium
_verificare (
                 Mundus* m,
     constans character* nomen_usoris,
     constans character* tessera,
                 Exitus* e,
                Piscina* p)
{
    CredentialiaInitium i;

    memset(e, ZEPHYRUM, magnitudo(*e));
    e->p  = p;
    i     = credentialia_verificare(m->cr, _c(nomen_usoris, p),
        _c(tessera, p), _c("10.0.0.1", p), _verificatio_venit, e);
    si (i == CREDENTIALIA_COEPTUM)
    {
        (vacuum)_currere(m->r, &e->verificationes, I);
    }
    redde i;
}

/* numerus actorum generis dati */
interior i32
_acta_numerare (
                Volumen* v,
     constans character* genus,
                Piscina* p)
{
    Xar* acta  = volumen_acta_legere(v, ZEPHYRUM, p);
    i32  n     = ZEPHYRUM;
    i32  k;

    per (k = ZEPHYRUM; acta && k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        si (chorda_aequalis_literis(a->genus, genus))
        {
            n++;
        }
    }
    redde n;
}

/* textus unus ex quaestione (piscina) */
interior chorda
_textus (
                Volumen* v,
     constans character* sql,
                Piscina* p)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(volumen_scrinium(v),
        sql);
                 chorda t = chorda_ex_literis("", p);

    si (e && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        t = scrinium_columna_textus(e, ZEPHYRUM, p);
    }
    si (e)
    {
        scrinium_finire(e);
    }
    redde t;
}


/* ==================================================
 * sectiones
 * ================================================== */

interior vacuum
_sectio_ponere_verificare (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      Exitus  e;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    CREDO_AEQUALIS_S32((s32)_verificare(&m, "fran", TESSERA_BONA, &e,
        p),
        (s32)CREDENTIALIA_COEPTUM);
    CREDO_AEQUALIS_I32(e.verificationes, I);
    CREDO_VERUM(e.bona);
    CREDO_CHORDA_AEQUALIS(_c(e.civis, p), id);
    CREDO_AEQUALIS_S32((s32)_verificare(&m, "fran", "tessera-mala", &e,
        p), (s32)CREDENTIALIA_COEPTUM);
    CREDO_AEQUALIS_I32(e.verificationes, I);
    CREDO_FALSUM(e.bona);
    CREDO_AEQUALIS_I32((i32)strlen(e.civis), ZEPHYRUM);
    /* usus ultimus notatus */
    CREDO_CHORDA_AEQUALIS_LITERIS(_textus(m.v,
        "SELECT CAST(ultimus_usus_ms AS TEXT) FROM credentialia", p),
        "1760000000000");
    _mundum_claudere(&m);
}

interior vacuum
_sectio_ignotus (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      Exitus  e;
         s64  a;
         s64  opus_noti;
         s64  opus_ignoti;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    a = custodia_gradus_summa(m.cu);
    (vacuum)_verificare(&m, "fran", "tessera-mala", &e, p);
    opus_noti  = custodia_gradus_summa(m.cu) - a;
    a          = custodia_gradus_summa(m.cu);
    CREDO_AEQUALIS_S32((s32)_verificare(&m, "nemo", TESSERA_BONA, &e,
        p),
        (s32)CREDENTIALIA_COEPTUM);
    opus_ignoti = custodia_gradus_summa(m.cu) - a;
    CREDO_AEQUALIS_I32(e.verificationes, I);
    CREDO_FALSUM(e.bona);
    CREDO_MAIOR_S32((s32)opus_noti, ZEPHYRUM);
    CREDO_AEQUALIS_S64(opus_ignoti, opus_noti);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_status_non_activus (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      chorda  id_amandae;
      Exitus  e;
         s64  a;
         s64  opus_boni;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    CREDO_NECESSE(_civem(&m, "amanda", TESSERA_BONA, &id_amandae, p));
    a = custodia_gradus_summa(m.cu);
    (vacuum)_verificare(&m, "amanda", TESSERA_BONA, &e, p);
    opus_boni = custodia_gradus_summa(m.cu) - a;
    CREDO_VERUM(e.bona);
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_NECESSE(cives_statum_ponere(m.cv, id, CIVIS_SUSPENSUS));
    CREDO_NECESSE(cives_delere(m.cv, id_amandae));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    /* suspensa et deleta: tessera recta, bona FALSUM, opus idem */
    a = custodia_gradus_summa(m.cu);
    (vacuum)_verificare(&m, "fran", TESSERA_BONA, &e, p);
    CREDO_AEQUALIS_I32(e.verificationes, I);
    CREDO_FALSUM(e.bona);
    CREDO_AEQUALIS_S64(custodia_gradus_summa(m.cu) - a, opus_boni);
    a = custodia_gradus_summa(m.cu);
    (vacuum)_verificare(&m, "amanda", TESSERA_BONA, &e, p);
    CREDO_AEQUALIS_I32(e.verificationes, I);
    CREDO_FALSUM(e.bona);
    CREDO_AEQUALIS_S64(custodia_gradus_summa(m.cu) - a, opus_boni);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_frenum (CredoContextus* c)
{
                Piscina* p = c->piscina;
                 Mundus  m;
                 chorda  id;
                 Exitus  e;
    CredentialiaInitium  i;
                    s64  ante;
                    s64  post_vocationem;
                    s32  k;

    CREDO_NECESSE(_mundus(c, &m, II));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    per (k = ZEPHYRUM; k < II; k++)
    {
        CREDO_AEQUALIS_S32((s32)_verificare(&m, "fran", "mala", &e, p),
            (s32)CREDENTIALIA_COEPTUM);
        CREDO_FALSUM(e.bona);
    }
    /* mora: vocatio statim redit, reditus post timer (CCL ms) */
    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p   = p;
    ante  = _ms_vera();
    i     = credentialia_verificare(m.cr, _c("fran", p),
        _c(TESSERA_BONA,
        p), _c("10.0.0.1", p), _verificatio_venit, &e);
    post_vocationem = _ms_vera() - ante;
    CREDO_AEQUALIS_S32((s32)i, (s32)CREDENTIALIA_COEPTUM);
    CREDO_AEQUALIS_I32(e.verificationes, ZEPHYRUM);
    CREDO_MINOR_S32((s32)post_vocationem, L);
    CREDO_VERUM(_currere(m.r, &e.verificationes, I));
    CREDO_MAIOR_AUT_AEQUALIS_S32((s32)(_ms_vera() - ante), CCXL);
    /* successus frenum purgat; deinde falsa usque ad recusationem */
    CREDO_VERUM(e.bona);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        (vacuum)_verificare(&m, "fran", "mala", &e, p);
    }
    memset(&e, ZEPHYRUM, magnitudo(e));
    e.p = p;
    CREDO_AEQUALIS_S32((s32)credentialia_verificare(m.cr, _c("fran", p),
        _c(TESSERA_BONA, p), _c("10.0.0.1", p), _verificatio_venit, &e),
        (s32)CREDENTIALIA_FRENATUM);
    per (k = ZEPHYRUM; k < C; k++)
    {
        reactor_poll(m.r, ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32(e.verificationes, ZEPHYRUM);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_occupatum (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      Exitus  e[CREDENTIALIA_OPERA];
      Exitus  ultimum;
         i32  k;
         i32  venerunt = ZEPHYRUM;
   character  longa[MXXV + I];

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    per (k = ZEPHYRUM; k < CREDENTIALIA_OPERA; k++)
    {
        memset(&e[k], ZEPHYRUM, magnitudo(e[k]));
        e[k].p = p;
        CREDO_AEQUALIS_S32((s32)credentialia_verificare(m.cr, _c("fran",
            p), _c("mala", p), _c("10.0.0.2", p), _verificatio_venit,
            &e[k]), (s32)CREDENTIALIA_COEPTUM);
    }
    memset(&ultimum, ZEPHYRUM, magnitudo(ultimum));
    CREDO_AEQUALIS_S32((s32)credentialia_verificare(m.cr, _c("fran", p),
        _c(TESSERA_BONA, p), _c("10.0.0.2", p), _verificatio_venit,
        &ultimum), (s32)CREDENTIALIA_OCCUPATUM);
    /* omnia exhauriuntur */
    per (k = ZEPHYRUM; k < VICES_MAXIMAE
        && venerunt < CREDENTIALIA_OPERA;
         k++)
    {
        i32 j;

        reactor_poll(m.r, I);
        venerunt = ZEPHYRUM;
        per (j = ZEPHYRUM; j < CREDENTIALIA_OPERA; j++)
        {
            venerunt += e[j].verificationes;
        }
    }
    CREDO_AEQUALIS_I32(venerunt, CREDENTIALIA_OPERA);
    CREDO_AEQUALIS_I32(ultimum.verificationes, ZEPHYRUM);
    /* tessera nimis longa */
    memset(longa, 'a', MXXV);
    longa[MXXV] = '\0';
    CREDO_AEQUALIS_S32((s32)credentialia_verificare(m.cr, _c("fran", p),
        _c(longa, p), _c("10.0.0.3", p), _verificatio_venit, &ultimum),
        (s32)CREDENTIALIA_PRAVUM);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_renovanda (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
    Custodia* vetus;
      chorda  id;
      Exitus  e;
      Exitus  h;
         i32  k;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    vetus = _custodia(c, m.r, II, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(vetus);
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_NECESSE(cives_creare(m.cv, _c("fran", p), _c("Fran", p), &id,
        p));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    memset(&h, ZEPHYRUM, magnitudo(h));
    h.p = p;
    CREDO_NECESSE(custodia_tesseram_digerere(vetus, _c(TESSERA_BONA, p),
        _hash_venit, &h));
    CREDO_NECESSE(_currere(m.r, &h.hashes, I));
    CREDO_CHORDA_CONTINET(h.phc, _c(",t=2,", p));
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_NECESSE(credentialia_tesseram_ponere(m.cr, id, h.phc));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    CREDO_AEQUALIS_I32(_acta_numerare(m.v, "credentiale.positum", p),
        I);
    (vacuum)_verificare(&m, "fran", TESSERA_BONA, &e, p);
    CREDO_VERUM(e.bona);
    /* re-hash post responsum: actum secundum, parametri novi */
    per (k = ZEPHYRUM; k < VICES_MAXIMAE
         && _acta_numerare(m.v, "credentiale.positum", p) < II; k++)
    {
        reactor_poll(m.r, I);
    }
    CREDO_AEQUALIS_I32(_acta_numerare(m.v, "credentiale.positum", p),
        II);
    CREDO_CHORDA_CONTINET(_textus(m.v, "SELECT phc FROM credentialia",
        p),
        _c(",t=1,", p));
    (vacuum)_verificare(&m, "fran", TESSERA_BONA, &e, p);
    CREDO_VERUM(e.bona);
    _mundum_claudere(&m);
}

interior vacuum
_sectio_una_per_personam (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      Exitus  h;
      Exitus  e;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    memset(&h, ZEPHYRUM, magnitudo(h));
    h.p = p;
    CREDO_NECESSE(credentialia_digerere(m.cr, _c("tessera-nova", p),
        _hash_venit, &h));
    CREDO_NECESSE(_currere(m.r, &h.hashes, I));
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_VERUM(credentialia_tesseram_ponere(m.cr, id, h.phc));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    CREDO_CHORDA_AEQUALIS_LITERIS(_textus(m.v,
        "SELECT CAST(count(*) AS TEXT) FROM credentialia", p), "1");
    (vacuum)_verificare(&m, "fran", TESSERA_BONA, &e, p);
    CREDO_FALSUM(e.bona);
    (vacuum)_verificare(&m, "fran", "tessera-nova", &e, p);
    CREDO_VERUM(e.bona);
    /* civis ignotus aut phc vacua: refutatur */
    CREDO_NECESSE(volumen_transactionem_incipere(m.v));
    CREDO_FALSUM(credentialia_tesseram_ponere(m.cr,
        _c("01AAAAAAAAAAAAAAAAAAAAAAAA", p), h.phc));
    CREDO_FALSUM(credentialia_tesseram_ponere(m.cr, id, _c("", p)));
    CREDO_NECESSE(volumen_transactionem_committere(m.v));
    _mundum_claudere(&m);
}

interior vacuum
_sectio_acta_sine_secretis (CredoContextus* c)
{
     Piscina* p = c->piscina;
      Mundus  m;
      chorda  id;
      Exitus  e;
         Xar* acta;
         i32  k;
      chorda  columnae;

    CREDO_NECESSE(_mundus(c, &m, ZEPHYRUM));
    CREDO_NECESSE(_civem(&m, "fran", TESSERA_BONA, &id, p));
    (vacuum)_verificare(&m, "fran", TESSERA_BONA, &e, p);
    (vacuum)_verificare(&m, "fran", "tessera-mala", &e, p);
    acta = volumen_acta_legere(m.v, ZEPHYRUM, p);
    CREDO_NECESSE_NON_NIHIL(acta);
    per (k = ZEPHYRUM; k < xar_numerus(acta); k++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, k);

        CREDO_FALSUM(chorda_continet(a->datum, _c("$argon2", p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c(TESSERA_BONA, p)));
        CREDO_FALSUM(chorda_continet(a->datum, _c("tessera-mala", p)));
    }
    CREDO_AEQUALIS_I32(_acta_numerare(m.v, "credentiale.positum", p),
        I);
    columnae = _textus(m.v, "SELECT group_concat(id || civis || genus"
        " || creatum_ms || ultimus_usus_ms) FROM credentialia", p);
    CREDO_CHORDA_NON_VACUA(columnae);
    CREDO_FALSUM(chorda_continet(columnae, _c("$argon2", p)));
    CREDO_FALSUM(chorda_continet(columnae, _c(TESSERA_BONA, p)));
    _mundum_claudere(&m);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ponere_verificare", _sectio_ponere_verificare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "ignotus", _sectio_ignotus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "status_non_activus", _sectio_status_non_activus, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "frenum", _sectio_frenum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "occupatum", _sectio_occupatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "renovanda", _sectio_renovanda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "una_per_personam", _sectio_una_per_personam, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "acta_sine_secretis", _sectio_acta_sine_secretis, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("credentialia", SECTIONES);
}
