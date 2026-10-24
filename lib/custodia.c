/* custodia.c - custos applicationis (vide custodia.h)
 *
 * Opera tesserarum in tabula fixa (CUSTODIA_OPERA_MAXIMA): status
 * LIBERUM -> EXSPECTANS (ordine adventus) -> CURRENS (labor reactoris,
 * piscina propria cum memoria argon2id) -> LIBERUM. Opus finitum
 * memoriam reddit, tesseram delet, proximum incipit, DEINDE reditum
 * vocat (reditus opus novum addere potest). */
#include "postulata_posix.h"
#include "custodia.h"
#include "base64.h"
#include "filum.h"
#include "moneta.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define CUSTODIA_FRENA          MXXIV
#define CUSTODIA_MORA_BASIS     CCL      /* ms */
#define CUSTODIA_PHC_CAPSA      CLX

/* tecta parametrorum phc lectorum (phc ex copia nostra, sed numquam
 * memoria infinita ex datis) */
#define CUSTODIA_M_TECTUM       262144   /* 256 MiB */
#define CUSTODIA_T_TECTUM       X
#define CUSTODIA_P_TECTUM       VIII

nomen enumeratio {
    OPUS_LIBERUM = 0,
    OPUS_EXSPECTANS,
    OPUS_CURRENS
} OpusStatus;

nomen enumeratio {
    OPUS_HASH = 0,
    OPUS_VERIFICARE
} OpusGenus;

nomen structura {
                   Custodia* custodia;
                 OpusStatus  status;
                  OpusGenus  genus;
                        s64  ordo;
                         i8  tessera[CUSTODIA_TESSERA_MAXIMA];
                        i32  tn;
                         i8  sal[ARGON2ID_SAL_MAXIMUS];
                        i32  sn;
          Argon2idParametri  par;
                         i8  expectatum[ARGON2ID_DIGESTUM_MAXIMUM];
                        i32  dn;
                        b32  ficticium;
                    Piscina* piscina;
               Argon2idOpus* opus;
        CustodiaHashReditus  reditus_hash;
 CustodiaVerificatioReditus  reditus_verificationis;
                     vacuum* datum;
} CustodiaOpus;

nomen structura {
         b32 activum;
    Sigillum clavis;
         i32 defectus;
         s64 ultimus_ms;
} CustodiaFrenum;

structura Custodia {
    CustodiaConfiguratio  cfg;
                 Reactor* reactor;
                 Piscina* piscina;
            CustodiaOpus  opera[CUSTODIA_OPERA_MAXIMA];
                     i32  currentia;
                     s64  ordo_proximus;
                     s64  gradus_summa;
          CustodiaFrenum  frena[CUSTODIA_FRENA];
};


/* ==================================================
 * AUXILIA
 * ================================================== */

/* aequalitas temporis constantis */
interior b32
_aequale_constans (
    constans i8* a,
    constans i8* b,
            i32  n)
{
     i8 diff = 0;
    i32 i;

    per (i = 0; i < n; i++)
    {
        diff = (i8)(diff | (a[i] ^ b[i]));
    }
    redde diff == 0;
}

/* XXXII octeti -> XLIII signa base64url (sine '='), NUL */
interior vacuum
_signa_tuta (
    constans i8* o,
      character* exitus)
{
    i32 k = base64url_scribere(o, XXXII, exitus, XLIII);

    exitus[k] = '\0';
}

interior character
_minuscula (
    character c)
{
    redde (c >= 'A' && c <= 'Z') ? (character)(c - 'A' + 'a') : c;
}


/* ==================================================
 * CREARE
 * ================================================== */

Custodia*
custodia_creare (
    constans CustodiaConfiguratio* configuratio,
                          Reactor* reactor,
                          Piscina* piscina)
{
    Custodia* c;

    si (!configuratio || !reactor || !piscina)
    {
        redde NIHIL;
    }
    c = (Custodia*)piscina_allocare(piscina, (i64)magnitudo(Custodia));
    si (!c)
    {
        redde NIHIL;
    }
    memset(c, 0, magnitudo(*c));
    c->cfg      = *configuratio;
    c->reactor  = reactor;
    c->piscina  = piscina;
    si (c->cfg.tessera_parametri.t_cost == 0)
    {
        memset(&c->cfg.tessera_parametri, 0,
            magnitudo(c->cfg.tessera_parametri));
        c->cfg.tessera_parametri.t_cost      = II;
        c->cfg.tessera_parametri.m_cost_kib  = 19456;
        c->cfg.tessera_parametri.lanae       = I;
        c->cfg.tessera_parametri.longitudo   = XXXII;
    }
    si (c->cfg.opera_concurrentia == 0)
    {
        c->cfg.opera_concurrentia = II;
    }
    si (c->cfg.caudices_per_gradum == 0)
    {
        c->cfg.caudices_per_gradum = XXXII;
    }
    si (c->cfg.frenum_conatus == 0)
    {
        c->cfg.frenum_conatus = V;
    }
    si (c->cfg.frenum_fenestra_s == 0)
    {
        c->cfg.frenum_fenestra_s = CM;
    }
    redde c;
}


/* ==================================================
 * OPERA TESSERARUM
 * ================================================== */

interior vacuum
_opera_incipere (
    Custodia* c);

interior b32
_labor (
    vacuum* datum)
{
                  CustodiaOpus* o = (CustodiaOpus*)datum;
                      Custodia* c = o->custodia;
                            i8  digestum[ARGON2ID_DIGESTUM_MAXIMUM];
                     character  phc[CUSTODIA_PHC_CAPSA];
                           i32  phc_n = 0;
                           b32  bene;
                           b32  bona       = FALSUM;
                           b32  renovanda  = FALSUM;
                     OpusGenus  genus;
           CustodiaHashReditus  reditus_digesti;
    CustodiaVerificatioReditus  reditus_verificationis;
                        vacuum* d;

    c->gradus_summa++;
    si (   o->opus
        && !argon2id_gradus(o->opus, c->cfg.caudices_per_gradum))
    {
        redde FALSUM;   /* gyrus proximus */
    }
    bene = o->opus != NIHIL;
    si (bene)
    {
        argon2id_digestum(o->opus, digestum);
    }
    si (bene && o->genus == OPUS_HASH)
    {
        chorda s = argon2id_phc_scribere(&o->par, o->sal, o->sn,
            digestum,
            o->piscina);

        si (s.mensura > 0 && s.mensura < CUSTODIA_PHC_CAPSA)
        {
            memcpy(phc, s.datum, (size_t)s.mensura);
            phc_n = s.mensura;
        }
    }
    si (bene && o->genus == OPUS_VERIFICARE)
    {
        constans Argon2idParametri* q = &c->cfg.tessera_parametri;

        bona = !o->ficticium && o->dn == o->par.longitudo
            && _aequale_constans(digestum, o->expectatum, o->dn);
        renovanda = bona && (o->par.t_cost != q->t_cost
            || o->par.m_cost_kib != q->m_cost_kib
            || o->par.lanae != q->lanae
            || o->par.longitudo != q->longitudo);
    }

    /* memoria reddita, tessera deleta, locus liber - ante reditum */
    piscina_destruere(o->piscina);
    memset(digestum, 0, magnitudo(digestum));
    memset(o->tessera, 0, magnitudo(o->tessera));
    memset(o->expectatum, 0, magnitudo(o->expectatum));
    genus                   = o->genus;
    reditus_digesti         = o->reditus_hash;
    reditus_verificationis  = o->reditus_verificationis;
    d                       = o->datum;
    o->piscina              = NIHIL;
    o->opus                 = NIHIL;
    o->status               = OPUS_LIBERUM;
    c->currentia--;
    _opera_incipere(c);

    si (genus == OPUS_HASH && reditus_digesti)
    {
        chorda r;

        r.datum    = phc_n > 0 ? (i8*)phc : NIHIL;
        r.mensura  = phc_n;
        reditus_digesti(d, r);
    }
    alioquin si (genus == OPUS_VERIFICARE && reditus_verificationis)
    {
        reditus_verificationis(d, bona, renovanda);
    }
    redde VERUM;
}

/* exspectantia ordine adventus incipere usque ad tectum */
interior vacuum
_opera_incipere (
    Custodia* c)
{
    dum (c->currentia < c->cfg.opera_concurrentia)
    {
        CustodiaOpus* o = NIHIL;
                 i32  i;

        per (i = 0; i < CUSTODIA_OPERA_MAXIMA; i++)
        {
            CustodiaOpus* x = &c->opera[i];

            si (   x->status == OPUS_EXSPECTANS
                && (!o || x->ordo < o->ordo))
            {
                o = x;
            }
        }
        si (!o)
        {
            redde;
        }
        o->piscina = piscina_generare_dynamicum("custodia_opus",
            LXIV * MXXIV);
        o->opus = o->piscina ? argon2id_incipere(&o->par, o->tessera,
            o->tn, o->sal, o->sn, o->piscina) : NIHIL;
        o->status = OPUS_CURRENS;
        c->currentia++;
        /* opus NIHIL (memoria deest): labor gyro primo errorem
         * nuntiat - via una */
        (vacuum)reactor_laborem_adicere(c->reactor, _labor, o);
    }
}

interior CustodiaOpus*
_opus_capere (
      Custodia* c,
        chorda  tessera)
{
    i32 i;

    si (   tessera.mensura > CUSTODIA_TESSERA_MAXIMA
        || (tessera.mensura > 0 && !tessera.datum))
    {
        redde NIHIL;
    }
    per (i = 0; i < CUSTODIA_OPERA_MAXIMA; i++)
    {
        CustodiaOpus* o = &c->opera[i];

        si (o->status == OPUS_LIBERUM)
        {
            memset(o, 0, magnitudo(*o));
            o->custodia  = c;
            o->ordo      = c->ordo_proximus++;
            o->tn        = tessera.mensura;
            si (tessera.mensura > 0)
            {
                memcpy(o->tessera, tessera.datum,
                    (size_t)tessera.mensura);
            }
            redde o;
        }
    }
    redde NIHIL;   /* cauda plena */
}

b32
custodia_tessera_valida (
    chorda tessera)
{
    redde tessera.datum != NIHIL
        && tessera.mensura >= CUSTODIA_TESSERA_MINIMA
        && tessera.mensura <= CUSTODIA_TESSERA_MAXIMA;
}

b32
custodia_tesseram_digerere (
               Custodia* custodia,
                 chorda  tessera,
    CustodiaHashReditus  reditus,
                 vacuum* datum)
{
    CustodiaOpus* o;

    si (!custodia || !custodia_tessera_valida(tessera))
    {
        redde FALSUM;
    }
    o = _opus_capere(custodia, tessera);
    si (!o)
    {
        redde FALSUM;
    }
    o->sn = XVI;
    si (!moneta_octeti_fortuiti(o->sal, o->sn))
    {
        memset(o->tessera, 0, magnitudo(o->tessera));
        redde FALSUM;   /* status LIBERUM manet */
    }
    o->genus         = OPUS_HASH;
    o->par           = custodia->cfg.tessera_parametri;
    o->reditus_hash  = reditus;
    o->datum         = datum;
    o->status        = OPUS_EXSPECTANS;
    _opera_incipere(custodia);
    redde VERUM;
}

/* sal ficticius fixus: hash ficticium = opus plenum, numquam bonum */
hic_manens constans character* CUSTODIA_SAL_FICTICIUS =
    "custodia:fictum";

b32
custodia_tesseram_verificare (
                      Custodia* custodia,
                        chorda  phc,
                        chorda  tessera,
    CustodiaVerificatioReditus  reditus,
                        vacuum* datum)
{
    CustodiaOpus* o;

    si (!custodia || tessera.mensura > CUSTODIA_TESSERA_MAXIMA)
    {
        redde FALSUM;
    }
    o = _opus_capere(custodia, tessera);
    si (!o)
    {
        redde FALSUM;
    }
    o->genus                   = OPUS_VERIFICARE;
    o->reditus_verificationis  = reditus;
    o->datum                   = datum;
    si (   phc.mensura == 0
        || !argon2id_phc_legere(phc, &o->par, o->sal, &o->sn,
               o->expectatum, &o->dn)
        || o->par.m_cost_kib > CUSTODIA_M_TECTUM
        || o->par.t_cost > CUSTODIA_T_TECTUM
        || o->par.lanae > CUSTODIA_P_TECTUM)
    {
        /* usor ignotus aut phc mala: opus plenum, responsum FALSUM */
        o->ficticium  = VERUM;
        o->par        = custodia->cfg.tessera_parametri;
        o->sn         = (i32)strlen(CUSTODIA_SAL_FICTICIUS);
        memcpy(o->sal, CUSTODIA_SAL_FICTICIUS, (size_t)o->sn);
        o->dn         = o->par.longitudo;
        memset(o->expectatum, 0, magnitudo(o->expectatum));
    }
    o->status = OPUS_EXSPECTANS;
    _opera_incipere(custodia);
    redde VERUM;
}

i32
custodia_opera_currentia (
    constans Custodia* custodia)
{
    redde custodia ? custodia->currentia : 0;
}

s64
custodia_gradus_summa (
    constans Custodia* custodia)
{
    redde custodia ? custodia->gradus_summa : 0;
}


/* ==================================================
 * SESSIONES ET CSRF
 * ================================================== */

b32
custodia_sessionem_creare (
    CustodiaSessio* exitus)
{
    i8 o[XXXII];

    si (!exitus || !moneta_octeti_fortuiti(o, XXXII))
    {
        redde FALSUM;
    }
    _signa_tuta(o, exitus->signum);
    exitus->sigillum = sigillum_computare(exitus->signum, XLIII);
    memset(o, 0, magnitudo(o));
    redde VERUM;
}

Sigillum
custodia_signi_sigillum (
    chorda signum)
{
    redde sigillum_computare(signum.datum,
        (memoriae_index)signum.mensura);
}

interior vacuum
_csrf_computare (
             Custodia* custodia,
    constans Sigillum* sessio,
            character* exitus)
{
    Sigillum mac;

    mac = sigillum_hmac(custodia->cfg.secretum, CUSTODIA_SECRETUM,
        sessio->octeti, SIGILLUM_OCTETI);

    _signa_tuta(mac.octeti, exitus);
}

chorda
custodia_csrf (
             Custodia* custodia,
    constans Sigillum* sessio,
              Piscina* piscina)
{
    character t[CUSTODIA_SIGNUM_MENSURA];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    si (!custodia || !sessio || !piscina)
    {
        redde vacua;
    }
    _csrf_computare(custodia, sessio, t);
    redde chorda_ex_literis(t, piscina);
}

b32
custodia_csrf_verificare (
             Custodia* custodia,
    constans Sigillum* sessio,
               chorda  valor)
{
    character t[CUSTODIA_SIGNUM_MENSURA];

    si (!custodia || !sessio || !valor.datum || valor.mensura != XLIII)
    {
        redde FALSUM;
    }
    _csrf_computare(custodia, sessio, t);
    redde _aequale_constans((constans i8*)t, valor.datum, XLIII);
}

b32
custodia_origo_congruit (
    chorda origo,
    chorda schema,
    chorda hospes)
{
    i32 n = schema.mensura + III + hospes.mensura;
    i32 i;

    si (   !origo.datum || origo.mensura != n || schema.mensura == 0
        || hospes.mensura == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < n; i++)
    {
        character e;

        si (i < schema.mensura)
        {
            e = (character)schema.datum[i];
        }
        alioquin si (i < schema.mensura + III)
        {
            e = "://"[i - schema.mensura];
        }
        alioquin
        {
            e = (character)hospes.datum[i - schema.mensura - III];
        }
        si (_minuscula((character)origo.datum[i]) != _minuscula(e))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * FRENUM
 * ================================================== */

interior CustodiaFrenum*
_frenum_invenire (
     Custodia* c,
       chorda  clavis,
          s64  nunc_ms,
          b32  creare)
{
          Sigillum s = sigillum_computare(clavis.datum,
                          (memoriae_index)clavis.mensura);
               s64  fenestra       = (s64)c->cfg.frenum_fenestra_s * M;
    CustodiaFrenum* liberum        = NIHIL;
    CustodiaFrenum* vetustissimum  = NIHIL;
               i32  i;

    per (i = 0; i < CUSTODIA_FRENA; i++)
    {
        CustodiaFrenum* f = &c->frena[i];

        si (f->activum && nunc_ms - f->ultimus_ms >= fenestra)
        {
            f->activum = FALSUM;   /* fenestra praeterita */
        }
        si (!f->activum)
        {
            si (!liberum)
            {
                liberum = f;
            }
            perge;
        }
        si (sigillum_aequale(&f->clavis, &s))
        {
            redde f;
        }
        si (!vetustissimum || f->ultimus_ms < vetustissimum->ultimus_ms)
        {
            vetustissimum = f;
        }
    }
    si (!creare)
    {
        redde NIHIL;
    }
    si (!liberum)
    {
        liberum = vetustissimum;   /* plena: vetustissimum cedit */
    }
    memset(liberum, 0, magnitudo(*liberum));
    liberum->activum     = VERUM;
    liberum->clavis      = s;
    liberum->ultimus_ms  = nunc_ms;
    redde liberum;
}

s32
custodia_frenum (
     Custodia* custodia,
       chorda  clavis,
          s64  nunc_ms)
{
    CustodiaFrenum* f;
               i32  n;

    si (!custodia)
    {
        redde 0;
    }
    f = _frenum_invenire(custodia, clavis, nunc_ms, FALSUM);
    n = (i32)custodia->cfg.frenum_conatus;
    si (!f || f->defectus < n)
    {
        redde 0;
    }
    si (f->defectus >= II * n)
    {
        redde -I;
    }
    redde (s32)(CUSTODIA_MORA_BASIS << (f->defectus - n));
}

vacuum
custodia_frenum_notare (
     Custodia* custodia,
       chorda  clavis,
          b32  successus,
          s64  nunc_ms)
{
    CustodiaFrenum* f;

    si (!custodia)
    {
        redde;
    }
    f = _frenum_invenire(custodia, clavis, nunc_ms, !successus);
    si (!f)
    {
        redde;
    }
    si (successus)
    {
        f->activum = FALSUM;
        redde;
    }
    f->defectus++;
    f->ultimus_ms = nunc_ms;
}


/* ==================================================
 * SECRETUM SERVI
 * ================================================== */

b32
custodia_secretum_parare (
    constans character* via,
                    i8* secretum)
{
       integer fd;
    structura stat st;
            i8 capsa[CUSTODIA_SECRETUM + I];
           i32 lecta = 0;

    si (!via || !secretum)
    {
        redde FALSUM;
    }
    /* secretum tempore cursus: per librum lectionum fabrica contentum
     * sigillaret - ergo lectio cruda (exemplar anthropic.clavis) */
    /* <tolera codex="CANALIS_PRAETERITUS" (>secretum servi */
    fd = open(via, O_RDONLY | O_NOFOLLOW);
    si (fd >= 0)
    {
        b32 bonum = fstat(fd, &st) == 0 && S_ISREG(st.st_mode)
            && (st.st_mode & 077) == 0
            && st.st_size == (off_t)CUSTODIA_SECRETUM;

        dum (bonum && lecta < CUSTODIA_SECRETUM + I)
        {
            ssize_t n = read(fd, capsa + lecta,
                (size_t)(CUSTODIA_SECRETUM + I - lecta));

            si (n <= 0)
            {
                frange;
            }
            lecta += (i32)n;
        }
        close(fd);
        si (!bonum || lecta != CUSTODIA_SECRETUM)
        {
            memset(capsa, 0, magnitudo(capsa));
            redde FALSUM;
        }
        memcpy(secretum, capsa, CUSTODIA_SECRETUM);
        memset(capsa, 0, magnitudo(capsa));
        redde VERUM;
    }
    si (errno != ENOENT)
    {
        redde FALSUM;
    }
    si (!moneta_octeti_fortuiti(capsa, CUSTODIA_SECRETUM))
    {
        redde FALSUM;
    }
    /* <tolera codex="CANALIS_PRAETERITUS" (>secretum servi novum */
    fd = open(via, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
    si (fd < 0)
    {
        memset(capsa, 0, magnitudo(capsa));
        redde FALSUM;
    }
    dum (lecta < CUSTODIA_SECRETUM)
    {
        ssize_t n = write(fd, capsa + lecta,
            (size_t)(CUSTODIA_SECRETUM - lecta));

        si (n <= 0)
        {
            frange;
        }
        lecta += (i32)n;
    }
    si (lecta != CUSTODIA_SECRETUM || fsync(fd) != 0)
    {
        close(fd);
        (vacuum)filum_delere(via);
        memset(capsa, 0, magnitudo(capsa));
        redde FALSUM;
    }
    close(fd);
    memcpy(secretum, capsa, CUSTODIA_SECRETUM);
    memset(capsa, 0, magnitudo(capsa));
    redde VERUM;
}
