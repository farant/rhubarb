/* http2.c - connexio HTTP/2 sine IO (RFC 9113). Vide include/http2.h.
 *
 * T5: praefatio, positiones, petitiones et responsa, 1xx et
 * appendices, PING, GOAWAY, RST_STREAM, CONTINUATION cum limitibus,
 * errores connexionis et rivi. Fluxus (fenestrae) et concurrentia:
 * T6 - "antlia" (_mittere_pendentia) iam omnia mittenda per unum locum
 * fundit. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "hpack.h"
#include "http2_forma.h"
#include "http2.h"
#include <string.h>

/* http2-plan C4 */
#define TABULA_ORDINARIA          (MMXLVIII * II)
#define FENESTRA_RIVI_NOSTRA      0x100000            /* 1 MiB */
#define FENESTRA_CONNEXIONIS      0x1000000           /* 16 MiB */
#define FENESTRA_PROTOCOLLI       0xffff              /* 65535, 6.9.2 */
#define CAPITA_MAXIMA             0x10000             /* 65536 */
#define CONTINUATIONES_MAXIMAE    VIII
#define OCTETI_CAPITUM_MAXIMI     0x10000
#define EXITUS_CAPACITAS          (CCLVI * MXXIV)     /* 256 KiB */
#define SCHEDA_INITIALIS          (LXIV * MXXIV)
#define RIVUS_PISCINA             (IV * MXXIV)
#define SINE_LIMITE               0xffffffff

nomen enumeratio {
    RIVUS_VACUUS = ZEPHYRUM,
    RIVUS_EXSPECTANS,      /* supra MAX_CONCURRENT_STREAMS (C3) */
    RIVUS_APERTUS,         /* capita missa, corpus nondum totum */
    RIVUS_DIMIDIATUS       /* nos finivimus; responsum exspectatur */
} RivusStatus;

nomen structura {
            i32  id;
    RivusStatus  status;
        Piscina* piscina;
     HpackCaput* capita;
            i32  numerus;
         chorda  corpus;
            i32  missum;
            b32  capita_finalia;
            s64  fenestra_mittendi;
            s64  fenestra_recipiendi;
} Rivus;

structura Http2Connexio {
         Http2Munus  munus;
            Piscina* piscina;
            Piscina* scheda;          /* eventus nondum capti */
                Xar* eventus;
                i32  eventus_lecti;
    Http2Positiones  nostrae;
    /* positiones paris */
                i32  fenestra_paris;
                i32  forma_paris;
                i32  rivi_paris;
        HpackTabula* codificator;
        HpackTabula* decodator;
                 i8* introitus;
                i32  introitus_n;
                i32  introitus_capacitas;
                 i8* exitus;
                i32  exitus_initium;
                i32  exitus_finis;
                i32  exitus_capacitas;
    /* massa capitum per CONTINUATION */
                 i8* massa;
                i32  massa_n;
                b32  in_capitibus;
                i32  capitum_rivus;
                b32  capitum_finis_rivi;
                i32  continuationes;
                Xar* rivi;           /* Rivus */
                i32  rivus_proximus;
                i32  rivi_activi;
                b32  viva;
                b32  discessus_receptus;
                b32  discedens;
                b32  positiones_paris_receptae;
                s64  fenestra_mittendi;
                s64  fenestra_recipiendi;
};


/* ==================================================
 * Exitus et eventus
 * ================================================== */

/* spatium n octetorum in fine exitus (compactio si opus) */
interior b32
_spatium (
    Http2Connexio* c,
              i32  n)
{
    si (c->exitus_capacitas - c->exitus_finis >= n)
    {
        redde VERUM;
    }
    si (c->exitus_initium > ZEPHYRUM)
    {
        memmove(c->exitus, c->exitus + c->exitus_initium,
            (size_t)(c->exitus_finis - c->exitus_initium));
        c->exitus_finis    -= c->exitus_initium;
        c->exitus_initium  = ZEPHYRUM;
    }
    redde c->exitus_capacitas - c->exitus_finis >= n;
}

interior i32
_liberum (
    Http2Connexio* c)
{
    redde c->exitus_capacitas - c->exitus_finis + c->exitus_initium;
}

interior b32
_formam_addere (
    Http2Connexio* c,
              i32  genus,
              i32  vexilla,
              i32  rivus,
      constans i8* contentum,
              i32  n)
{
    i32 k;

    si (!_spatium(c, HTTP2_CAPUT_FORMAE + n))
    {
        redde FALSUM;
    }
    k = http2_formam_scribere(genus, vexilla, rivus, contentum, n,
        c->exitus + c->exitus_finis, c->exitus_capacitas
            - c->exitus_finis);
    c->exitus_finis += k;
    redde k > ZEPHYRUM;
}

interior Http2Eventus*
_eventum (
              Http2Connexio* c,
          Http2EventusGenus  genus,
                        i32  rivus)
{
    Http2Eventus* ev;

    ev = (Http2Eventus*)xar_addere(c->eventus);
    si (ev)
    {
        memset(ev, ZEPHYRUM, magnitudo(Http2Eventus));
        ev->genus = genus;
        ev->rivus = rivus;
    }
    redde ev;
}

interior chorda
_copia (
    constans i8* datum,
            i32  n,
        Piscina* piscina)
{
    chorda r;

    r.mensura = n;
    r.datum = (i8*)piscina_allocare(piscina, (memoriae_index)(n
        + I));
    si (r.datum && n > ZEPHYRUM)
    {
        memcpy(r.datum, datum, (size_t)n);
    }
    redde r;
}

interior vacuum
_discessum_mittere (
    Http2Connexio* c,
              i32  codex)
{
    i8 g[VIII];

    /* cliens rivos paris numquam accepit: ultimus 0 */
    http2_u32_scribere(ZEPHYRUM, g);
    http2_u32_scribere(codex, g + IV);
    (vacuum)_formam_addere(c, HTTP2_DISCESSUS, ZEPHYRUM, ZEPHYRUM, g,
        VIII);
}

/* 5.4.1: GOAWAY, connexio mortua, eventus ERROR */
interior vacuum
_error_connexionis (
    Http2Connexio* c,
              i32  codex)
{
    Http2Eventus* ev;

    si (!c->viva)
    {
        redde;
    }
    c->viva = FALSUM;
    _discessum_mittere(c, codex);
    ev = _eventum(c, HTTP2_EVENTUS_ERROR, ZEPHYRUM);
    si (ev)
    {
        ev->codex = codex;
    }
}


/* ==================================================
 * Rivi
 * ================================================== */

interior Rivus*
_rivum_invenire (
    Http2Connexio* c,
              i32  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(c->rivi); i++)
    {
        Rivus* r = (Rivus*)xar_obtinere(c->rivi, i);

        si (r->status != RIVUS_VACUUS && r->id == id)
        {
            redde r;
        }
    }
    redde NIHIL;
}

interior Rivus*
_rivum_novum (
    Http2Connexio* c)
{
    Rivus* r;
      i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(c->rivi); i++)
    {
        r = (Rivus*)xar_obtinere(c->rivi, i);
        si (r->status == RIVUS_VACUUS)
        {
            redde r;
        }
    }
    redde (Rivus*)xar_addere(c->rivi);
}

interior vacuum
_mittere_pendentia (
    Http2Connexio* c);

interior vacuum
_rivum_claudere (
    Http2Connexio* c,
            Rivus* r)
{
    si (r->status == RIVUS_APERTUS || r->status == RIVUS_DIMIDIATUS)
    {
        c->rivi_activi--;
    }
    si (r->piscina)
    {
        piscina_destruere(r->piscina);
    }
    memset(r, ZEPHYRUM, magnitudo(Rivus));
}

/* capita rivi codificata et in formas HEADERS + CONTINUATION fissa;
 * FALSUM (nihil mutatum praeter codificatorem? numquam: spatium prius
 * probatur) si exitus non capit */
interior b32
_capita_mittere (
    Http2Connexio* c,
            Rivus* r)
{
     i8* massa;
    i32  capacitas;
    i32  n;
    i32  i;
    i32  pos;
    i32  formae;
    b32  finis_rivi;

    capacitas = XXXII;
    per (i = ZEPHYRUM; i < r->numerus; i++)
    {
        capacitas += r->capita[i].titulus.mensura
                   + r->capita[i].valor.mensura + XVI;
    }
    formae = capacitas / c->forma_paris + I;
    si (_liberum(c) < capacitas + formae * HTTP2_CAPUT_FORMAE)
    {
        redde FALSUM;
    }
    massa = (i8*)piscina_allocare(r->piscina,
        (memoriae_index)capacitas);
    n = massa ? hpack_codificare(c->codificator, r->capita, r->numerus,
        massa, capacitas) : ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        _error_connexionis(c, HTTP2_ERROR_INTERNUS);
        redde FALSUM;
    }
    finis_rivi  = r->corpus.mensura == ZEPHYRUM;
    pos         = ZEPHYRUM;
    fac
    {
        i32 pars = n - pos;
        i32 vexilla;

        si (pars > c->forma_paris)
        {
            pars = c->forma_paris;
        }
        vexilla = (pos + pars == n) ? HTTP2_FINIS_CAPITUM : ZEPHYRUM;
        si (pos == ZEPHYRUM && finis_rivi)
        {
            vexilla |= HTTP2_FINIS_RIVI;
        }
        (vacuum)_formam_addere(c, pos == ZEPHYRUM ? HTTP2_CAPITA
            : HTTP2_CONTINUATIO, vexilla, r->id, massa + pos, pars);
        pos += pars;
    } dum (pos < n);
    r->status = finis_rivi ? RIVUS_DIMIDIATUS : RIVUS_APERTUS;
    c->rivi_activi++;
    redde VERUM;
}

/* corpus rivi quantum exitus (et fenestrae, T6) sinunt */
interior vacuum
_data_mittere (
    Http2Connexio* c,
            Rivus* r)
{
    dum (r->status == RIVUS_APERTUS && r->missum < r->corpus.mensura)
    {
        i32 pars = r->corpus.mensura - r->missum;
        i32 liberum;
        b32 ultima;

        si (_liberum(c) <= HTTP2_CAPUT_FORMAE)
        {
            redde;
        }
        liberum = _liberum(c) - HTTP2_CAPUT_FORMAE;
        si (pars > c->forma_paris)
        {
            pars = c->forma_paris;
        }
        si (pars > liberum)
        {
            pars = liberum;
        }
        ultima = r->missum + pars == r->corpus.mensura;
        (vacuum)_formam_addere(c, HTTP2_DATA,
            ultima ? HTTP2_FINIS_RIVI : ZEPHYRUM, r->id,
            r->corpus.datum + r->missum, pars);
        r->missum += pars;
        si (ultima)
        {
            r->status = RIVUS_DIMIDIATUS;
        }
    }
}

/* antlia: corpora pendentia, deinde petitiones exspectantes ordine
 * rivorum (C3) */
interior vacuum
_mittere_pendentia (
    Http2Connexio* c)
{
    i32 i;

    si (!c->viva)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->rivi); i++)
    {
        _data_mittere(c, (Rivus*)xar_obtinere(c->rivi, i));
    }
    dum (c->rivi_activi < c->rivi_paris && !c->discessus_receptus)
    {
        Rivus* minimus = NIHIL;

        per (i = ZEPHYRUM; i < xar_numerus(c->rivi); i++)
        {
            Rivus* r = (Rivus*)xar_obtinere(c->rivi, i);

            si (   r->status == RIVUS_EXSPECTANS
                && (!minimus || r->id < minimus->id))
            {
                minimus = r;
            }
        }
        si (!minimus || !_capita_mittere(c, minimus))
        {
            redde;
        }
        _data_mittere(c, minimus);
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

interior i32
_ordinarium (
    i32 valor,
    i32 ordinarium)
{
    redde valor > ZEPHYRUM ? valor : ordinarium;
}

Http2Connexio*
http2_connexionem_creare (
                 Http2Munus  munus,
    constans Http2Positiones* positiones,
                    Piscina* piscina)
{
      Http2Connexio* c;
    Http2Positiones  vacuae;
                 i8  p[XXX];
                i32  n;
                 i8  g[IV];

    si (munus != HTTP2_CLIENS || !piscina)
    {
        redde NIHIL;
    }
    memset(&vacuae, ZEPHYRUM, magnitudo(vacuae));
    si (!positiones)
    {
        positiones = &vacuae;
    }
    c = (Http2Connexio*)piscina_allocare(piscina,
        magnitudo(Http2Connexio));
    si (!c)
    {
        redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(Http2Connexio));
    c->munus    = munus;
    c->piscina  = piscina;
    c->nostrae.tabula_capitum = _ordinarium(positiones->tabula_capitum,
        TABULA_ORDINARIA);
    c->nostrae.fenestra_initialis = _ordinarium(
        positiones->fenestra_initialis, FENESTRA_RIVI_NOSTRA);
    c->nostrae.forma_maxima = _ordinarium(positiones->forma_maxima,
        HTTP2_FORMA_MINIMA);
    c->nostrae.capita_maxima = _ordinarium(positiones->capita_maxima,
        CAPITA_MAXIMA);
    c->nostrae.continuationes_maximae = _ordinarium(
        positiones->continuationes_maximae, CONTINUATIONES_MAXIMAE);
    c->nostrae.octeti_capitum_maximi = _ordinarium(
        positiones->octeti_capitum_maximi, OCTETI_CAPITUM_MAXIMI);
    c->fenestra_paris       = FENESTRA_PROTOCOLLI;
    c->forma_paris          = HTTP2_FORMA_MINIMA;
    c->rivi_paris           = SINE_LIMITE;
    c->fenestra_mittendi    = FENESTRA_PROTOCOLLI;
    c->fenestra_recipiendi  = FENESTRA_CONNEXIONIS;
    c->scheda = piscina_generare_dynamicum("http2_scheda",
        SCHEDA_INITIALIS);
    c->eventus = c->scheda ? xar_creare(c->scheda,
        (i32)magnitudo(Http2Eventus)) : NIHIL;
    c->codificator = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    c->decodator = hpack_tabulam_creare(piscina,
        c->nostrae.tabula_capitum);
    c->introitus_capacitas = HTTP2_CAPUT_FORMAE
        + c->nostrae.forma_maxima;
    c->introitus = (i8*)piscina_allocare(piscina,
        (memoriae_index)c->introitus_capacitas);
    c->exitus_capacitas = EXITUS_CAPACITAS;
    c->exitus = (i8*)piscina_allocare(piscina,
        (memoriae_index)c->exitus_capacitas);
    c->massa = (i8*)piscina_allocare(piscina,
        (memoriae_index)c->nostrae.octeti_capitum_maximi);
    c->rivi = xar_creare(piscina, (i32)magnitudo(Rivus));
    si (   !c->eventus || !c->codificator || !c->decodator
        || !c->introitus || !c->exitus || !c->massa || !c->rivi)
    {
        redde NIHIL;
    }
    c->rivus_proximus  = I;
    c->viva            = VERUM;
    /* 3.4 praefatio, 6.5 SETTINGS, 6.9 WINDOW_UPDATE connexionis */
    memcpy(c->exitus, HTTP2_PRAEFATIO, HTTP2_PRAEFATIO_MENSURA);
    c->exitus_finis = HTTP2_PRAEFATIO_MENSURA;
    n = http2_positionem_scribere(HTTP2_POSITIO_IMPULSUS, ZEPHYRUM, p);
    n += http2_positionem_scribere(HTTP2_POSITIO_TABULA,
        c->nostrae.tabula_capitum, p + n);
    n += http2_positionem_scribere(HTTP2_POSITIO_FENESTRA,
        c->nostrae.fenestra_initialis, p + n);
    n += http2_positionem_scribere(HTTP2_POSITIO_CAPITA,
        c->nostrae.capita_maxima, p + n);
    si (c->nostrae.forma_maxima != HTTP2_FORMA_MINIMA)
    {
        n += http2_positionem_scribere(HTTP2_POSITIO_FORMA,
            c->nostrae.forma_maxima, p + n);
    }
    (vacuum)_formam_addere(c, HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, p,
        n);
    http2_u32_scribere(FENESTRA_CONNEXIONIS - FENESTRA_PROTOCOLLI, g);
    (vacuum)_formam_addere(c, HTTP2_FENESTRA, ZEPHYRUM, ZEPHYRUM, g,
        IV);
    redde c;
}


/* ==================================================
 * Receptio
 * ================================================== */

interior vacuum
_positiones_applicare (
               Http2Connexio* c,
         constans Http2Forma* f)
{
    i32 k;
    i32 id;
    i32 valor;

    per (k = ZEPHYRUM; http2_positionem_legere(f, k, &id, &valor); k++)
    {
        commutatio (id)
        {
            casus HTTP2_POSITIO_TABULA:
                hpack_magnitudinem_ponere(c->codificator, valor);
                frange;
            casus HTTP2_POSITIO_RIVI:
                c->rivi_paris = valor;
                frange;
            casus HTTP2_POSITIO_FENESTRA:
                c->fenestra_paris = valor;
                frange;
            casus HTTP2_POSITIO_FORMA:
                c->forma_paris = valor;
                frange;
            ordinarius:
                frange;
        }
    }
}

interior b32
_massam_addere (
    Http2Connexio* c,
      constans i8* datum,
              i32  n)
{
    si (c->massa_n + n > c->nostrae.octeti_capitum_maximi)
    {
        _error_connexionis(c, HTTP2_ERROR_MODERATIO);
        redde FALSUM;
    }
    memcpy(c->massa + c->massa_n, datum, (size_t)n);
    c->massa_n += n;
    redde VERUM;
}

interior b32
_status_informativus (
    Xar* capita)
{
    HpackCaput* h;

    si (xar_numerus(capita) < I)
    {
        redde FALSUM;
    }
    h = (HpackCaput*)xar_obtinere(capita, ZEPHYRUM);
    redde chorda_aequalis_literis(h->titulus, ":status")
        && h->valor.mensura == III && h->valor.datum[ZEPHYRUM] == '1';
}

/* massa tota: decodatio SEMPER (tabula concors), deinde eventus si
 * rivus vivit */
interior vacuum
_capita_perficere (
    Http2Connexio* c)
{
           Xar* capita;
    HpackError  e;
         Rivus* r;
  Http2Eventus* ev;

    c->in_capitibus = FALSUM;
    capita = xar_creare(c->scheda, (i32)magnitudo(HpackCaput));
    e = capita ? hpack_decodere(c->decodator, c->massa, c->massa_n,
        c->nostrae.capita_maxima, c->scheda, capita)
        : HPACK_ERROR_NIMIA;
    si (e != HPACK_BENE)
    {
        _error_connexionis(c, HTTP2_ERROR_COMPRESSIO);
        redde;
    }
    r = _rivum_invenire(c, c->capitum_rivus);
    si (!r)
    {
        redde;
    }
    ev = _eventum(c, HTTP2_EVENTUS_CAPITA, r->id);
    si (ev)
    {
        ev->capita       = capita;
        ev->informativa  = !r->capita_finalia
                        && _status_informativus(capita);
        si (!ev->informativa)
        {
            r->capita_finalia = VERUM;
        }
    }
    si (c->capitum_finis_rivi)
    {
        (vacuum)_eventum(c, HTTP2_EVENTUS_FINIS, r->id);
        _rivum_claudere(c, r);
    }
}

interior vacuum
_discessum_tractare (
               Http2Connexio* c,
         constans Http2Forma* f)
{
    Http2Eventus* ev;
             i32  ultimus;
             i32  i;

    ultimus = http2_u32_legere(f->contentum) & HTTP2_FENESTRA_SUPREMA;
    c->discessus_receptus = VERUM;
    per (i = ZEPHYRUM; i < xar_numerus(c->rivi); i++)
    {
        Rivus* r = (Rivus*)xar_obtinere(c->rivi, i);

        si (r->status != RIVUS_VACUUS && r->id > ultimus)
        {
            (vacuum)_eventum(c, HTTP2_EVENTUS_DISCESSUS, r->id);
            _rivum_claudere(c, r);
        }
    }
    ev = _eventum(c, HTTP2_EVENTUS_DISCESSUS, ZEPHYRUM);
    si (ev)
    {
        ev->codex          = http2_u32_legere(f->contentum + IV);
        ev->rivus_ultimus  = ultimus;
        ev->data = _copia(f->contentum + VIII, f->contentum_n - VIII,
            c->scheda);
    }
}

/* rivus quem cliens numquam aperuit (par aut >= proximum) */
interior b32
_rivus_otiosus (
    Http2Connexio* c,
              i32  id)
{
    redde (id & I) == ZEPHYRUM || id >= c->rivus_proximus;
}

interior vacuum
_formam_tractare (
                   Http2Connexio* c,
             constans Http2Forma* f,
          constans Http2Iudicium* j)
{
           Rivus* r;
    Http2Eventus* ev;
              i8  g[IV];

    si (!j->bona)
    {
        si (j->connexionis)
        {
            _error_connexionis(c, j->codex);
            redde;
        }
        /* 5.4.2 error rivi */
        http2_u32_scribere(j->codex, g);
        (vacuum)_formam_addere(c, HTTP2_ABROGATIO, ZEPHYRUM, f->rivus,
            g,
            IV);
        r = _rivum_invenire(c, f->rivus);
        si (r)
        {
            ev = _eventum(c, HTTP2_EVENTUS_ABROGATUS, r->id);
            si (ev)
            {
                ev->codex = j->codex;
            }
            _rivum_claudere(c, r);
        }
        redde;
    }
    /* 3.4: forma prima servi = SETTINGS */
    si (   !c->positiones_paris_receptae
        && (f->genus != HTTP2_POSITIONES
            || (f->vexilla & HTTP2_RECEPTUM)))
    {
        _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
        redde;
    }
    /* 6.10: inter HEADERS et CONTINUATION nihil aliud */
    si (c->in_capitibus && f->genus != HTTP2_CONTINUATIO)
    {
        _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
        redde;
    }
    commutatio (f->genus)
    {
        casus HTTP2_POSITIONES:
            si (f->vexilla & HTTP2_RECEPTUM)
            {
                frange;
            }
            _positiones_applicare(c, f);
            c->positiones_paris_receptae = VERUM;
            (vacuum)_formam_addere(c, HTTP2_POSITIONES, HTTP2_RECEPTUM,
                ZEPHYRUM, NIHIL, ZEPHYRUM);
            frange;
        casus HTTP2_PULSUS:
            si (f->vexilla & HTTP2_RECEPTUM)
            {
                ev = _eventum(c, HTTP2_EVENTUS_PULSUS, ZEPHYRUM);
                si (ev)
                {
                    ev->data = _copia(f->contentum, VIII, c->scheda);
                }
                frange;
            }
            (vacuum)_formam_addere(c, HTTP2_PULSUS, HTTP2_RECEPTUM,
                ZEPHYRUM, f->contentum, VIII);
            frange;
        casus HTTP2_DISCESSUS:
            _discessum_tractare(c, f);
            frange;
        casus HTTP2_ABROGATIO:
            r = _rivum_invenire(c, f->rivus);
            si (r)
            {
                ev = _eventum(c, HTTP2_EVENTUS_ABROGATUS, r->id);
                si (ev)
                {
                    ev->codex = http2_u32_legere(f->contentum);
                }
                _rivum_claudere(c, r);
            }
            alioquin si (_rivus_otiosus(c, f->rivus))
            {
                _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
            }
            frange;
        casus HTTP2_CAPITA:
            si (_rivus_otiosus(c, f->rivus))
            {
                _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
                frange;
            }
            c->capitum_rivus       = f->rivus;
            c->capitum_finis_rivi  = (f->vexilla & HTTP2_FINIS_RIVI)
                                  != ZEPHYRUM;
            c->massa_n             = ZEPHYRUM;
            c->continuationes      = ZEPHYRUM;
            si (!_massam_addere(c, f->contentum, f->contentum_n))
            {
                frange;
            }
            si (f->vexilla & HTTP2_FINIS_CAPITUM)
            {
                _capita_perficere(c);
            }
            alioquin
            {
                c->in_capitibus = VERUM;
            }
            frange;
        casus HTTP2_CONTINUATIO:
            si (!c->in_capitibus || f->rivus != c->capitum_rivus)
            {
                _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
                frange;
            }
            si (++c->continuationes > c->nostrae.continuationes_maximae)
            {
                _error_connexionis(c, HTTP2_ERROR_MODERATIO);
                frange;
            }
            si (!_massam_addere(c, f->contentum, f->contentum_n))
            {
                frange;
            }
            si (f->vexilla & HTTP2_FINIS_CAPITUM)
            {
                _capita_perficere(c);
            }
            frange;
        casus HTTP2_DATA:
            r = _rivum_invenire(c, f->rivus);
            si (!r)
            {
                si (_rivus_otiosus(c, f->rivus))
                {
                    _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
                }
                frange;
            }
            si (f->contentum_n > ZEPHYRUM)
            {
                ev = _eventum(c, HTTP2_EVENTUS_DATA, r->id);
                si (ev)
                {
                    ev->data = _copia(f->contentum, f->contentum_n,
                        c->scheda);
                }
            }
            si (f->vexilla & HTTP2_FINIS_RIVI)
            {
                (vacuum)_eventum(c, HTTP2_EVENTUS_FINIS, r->id);
                _rivum_claudere(c, r);
            }
            frange;
        casus HTTP2_PROMISSIO:
            /* ENABLE_PUSH 0 misimus */
            _error_connexionis(c, HTTP2_ERROR_PROTOCOLLI);
            frange;
        ordinarius:
            /* PRIORITY neglecta; WINDOW_UPDATE: T6; ignota praetereunt */
            frange;
    }
}

b32
http2_recipere (
    Http2Connexio* c,
      constans i8* datum,
              i32  n)
{
    si (!c || !c->viva || (n > ZEPHYRUM && !datum))
    {
        redde FALSUM;
    }
    dum (c->viva)
    {
        i32 copia;
        i32 pos;

        copia = c->introitus_capacitas - c->introitus_n;
        si (copia > n)
        {
            copia = n;
        }
        memcpy(c->introitus + c->introitus_n, datum, (size_t)copia);
        c->introitus_n  += copia;
        datum           += copia;
        n               -= copia;
        pos             = ZEPHYRUM;
        dum (c->viva)
        {
               Http2Forma f;
            Http2Iudicium j;
                      i32 k;

            k = http2_formam_legere(c->introitus + pos,
                c->introitus_n - pos, c->nostrae.forma_maxima, &f, &j);
            si (k == ZEPHYRUM)
            {
                frange;
            }
            _formam_tractare(c, &f, &j);
            pos += k;
        }
        memmove(c->introitus, c->introitus + pos,
            (size_t)(c->introitus_n - pos));
        c->introitus_n -= pos;
        si (n == ZEPHYRUM || (copia == ZEPHYRUM && pos == ZEPHYRUM))
        {
            frange;
        }
    }
    _mittere_pendentia(c);
    redde c->viva;
}


/* ==================================================
 * Exitus, eventus, petitiones
 * ================================================== */

i32
http2_mittenda (
    Http2Connexio*  c,
      constans i8** datum)
{
    si (!c || !datum)
    {
        redde ZEPHYRUM;
    }
    *datum = c->exitus + c->exitus_initium;
    redde c->exitus_finis - c->exitus_initium;
}

vacuum
http2_mittenda_consumere (
    Http2Connexio* c,
              i32  n)
{
    si (!c)
    {
        redde;
    }
    si (n > c->exitus_finis - c->exitus_initium)
    {
        n = c->exitus_finis - c->exitus_initium;
    }
    c->exitus_initium += n;
    si (c->exitus_initium == c->exitus_finis)
    {
        c->exitus_initium  = ZEPHYRUM;
        c->exitus_finis    = ZEPHYRUM;
    }
    _mittere_pendentia(c);
}

b32
http2_eventum_capere (
    Http2Connexio* c,
     Http2Eventus* eventus,
          Piscina* piscina)
{
    Http2Eventus* ev;
             i32  i;

    si (!c || !eventus || !piscina)
    {
        redde FALSUM;
    }
    si (c->eventus_lecti >= xar_numerus(c->eventus))
    {
        piscina_vacare(c->scheda);
        c->eventus = xar_creare(c->scheda,
            (i32)magnitudo(Http2Eventus));
        c->eventus_lecti = ZEPHYRUM;
        redde FALSUM;
    }
    ev = (Http2Eventus*)xar_obtinere(c->eventus,
        c->eventus_lecti++);
    *eventus       = *ev;
    eventus->data  = _copia(ev->data.datum, ev->data.mensura, piscina);
    si (ev->capita)
    {
        eventus->capita = xar_creare(piscina,
            (i32)magnitudo(HpackCaput));
        per (i = ZEPHYRUM; eventus->capita
            && i < xar_numerus(ev->capita);
             i++)
        {
            HpackCaput* fons = (HpackCaput*)xar_obtinere(ev->capita, i);
            HpackCaput* h = (HpackCaput*)xar_addere(eventus->capita);

            si (h)
            {
                h->titulus  = _copia(fons->titulus.datum,
                    fons->titulus.mensura, piscina);
                h->valor    = _copia(fons->valor.datum,
                    fons->valor.mensura, piscina);
                h->numquam  = fons->numquam;
            }
        }
    }
    redde VERUM;
}

i32
http2_petere (
         Http2Connexio* c,
    constans HpackCaput* capita,
                    i32  numerus,
                 chorda  corpus)
{
    hic_manens constans character* PSEUDO[IV] = {
        ":method", ":scheme", ":authority", ":path"
    };
    Rivus* r;
      i32  i;

    si (   !c || !c->viva || c->discessus_receptus || c->discedens
        || numerus < IV || !capita
        || c->rivus_proximus > HTTP2_FENESTRA_SUPREMA)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        si (!chorda_aequalis_literis(capita[i].titulus, PSEUDO[i]))
        {
            redde ZEPHYRUM;
        }
    }
    r = _rivum_novum(c);
    si (!r)
    {
        redde ZEPHYRUM;
    }
    memset(r, ZEPHYRUM, magnitudo(Rivus));
    r->piscina = piscina_generare_dynamicum("http2_rivus",
        RIVUS_PISCINA);
    r->capita  = r->piscina ? (HpackCaput*)piscina_allocare(r->piscina,
        (memoriae_index)numerus * magnitudo(HpackCaput)) : NIHIL;
    si (!r->capita)
    {
        si (r->piscina)
        {
            piscina_destruere(r->piscina);
        }
        memset(r, ZEPHYRUM, magnitudo(Rivus));
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        r->capita[i].titulus  = _copia(capita[i].titulus.datum,
            capita[i].titulus.mensura, r->piscina);
        r->capita[i].valor    = _copia(capita[i].valor.datum,
            capita[i].valor.mensura, r->piscina);
        r->capita[i].numquam  = capita[i].numquam;
    }
    r->numerus = numerus;
    r->corpus = _copia(corpus.datum, corpus.mensura, r->piscina);
    r->id = c->rivus_proximus;
    r->status = RIVUS_EXSPECTANS;
    r->fenestra_mittendi = c->fenestra_paris;
    r->fenestra_recipiendi = c->nostrae.fenestra_initialis;
    c->rivus_proximus += II;
    _mittere_pendentia(c);
    redde r->id;
}

b32
http2_rivum_abrogare (
    Http2Connexio* c,
              i32  rivus)
{
    Rivus* r;
       i8  g[IV];

    r = c ? _rivum_invenire(c, rivus) : NIHIL;
    si (!r)
    {
        redde FALSUM;
    }
    si (r->status != RIVUS_EXSPECTANS && c->viva)
    {
        http2_u32_scribere(HTTP2_ERROR_ABROGATUM, g);
        (vacuum)_formam_addere(c, HTTP2_ABROGATIO, ZEPHYRUM, rivus, g,
            IV);
    }
    _rivum_claudere(c, r);
    _mittere_pendentia(c);
    redde VERUM;
}

b32
http2_pulsare (
    Http2Connexio* c,
      constans i8* octo)
{
    si (!c || !c->viva || !octo)
    {
        redde FALSUM;
    }
    redde _formam_addere(c, HTTP2_PULSUS, ZEPHYRUM, ZEPHYRUM, octo,
        VIII);
}

vacuum
http2_discedere (
    Http2Connexio* c,
              i32  codex)
{
    si (!c || !c->viva || c->discedens)
    {
        redde;
    }
    c->discedens = VERUM;
    _discessum_mittere(c, codex);
}

b32
http2_viva (
    constans Http2Connexio* c)
{
    redde c && c->viva;
}
