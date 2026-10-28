/* communio_cliens.c - cliens communionis: semitae ascensus et
 * descensus in reactore (communio-plan-2 T6) */

#include "postulata_posix.h"
#include "communio_cliens.h"
#include "communio.h"
#include "communio_forma.h"
#include "veredarius.h"
#include "json.h"
#include "xar.h"
#include "filum.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define TESSERA_MAXIMA     XLIV
#define BASIS_MAXIMA       CCLVI
#define ERROR_MAXIMUS      CCLVI
#define CLAVIS_MAXIMA      CCCXX
#define VIA_MAXIMA         CCCXX
#define FASCICULI_OCTETI       ((i32)DXII * (i32)M)
#define QUAESTIO_MS        M             /* ascensus: acta nova */
#define REPETITIO_MS       ((i32)XXX * (i32)M)
#define MORA_ORDINARIA     XXV
#define MORA_MARGO_S       XV
#define TEMPUS_PETITIONIS  ((i32)XXX * (i32)M)
#define LECTIO_MAXIMA      ((i32)COMMUNIO_ACTA_MAXIMA * (i32)II)

nomen enumeratio {
    GRADUS_NULLUS = ZEPHYRUM,
    GRADUS_LIGAT,
    GRADUS_DESUNT,
    GRADUS_MASSAM_MITTIT,
    GRADUS_ACTA_MITTIT,
    GRADUS_TRAHIT,
    GRADUS_MASSAM_PETIT
} Gradus;

/* semita una: petitio una in itinere ad plurimum */
nomen structura {
    structura CommunioCliens* cliens;
                         b32  ascendens;
                     Piscina* piscina;       /* cyclus unus */
    /* plan-5 T1: petitio per veredarium - in_itinere a missione usque
     * ad responsum tractatum; respondit = finis venit */
                         b32 in_itinere;
                         b32 respondit;
                         i32 signum;
         VeredariusResponsum responsum;
                      Gradus gradus;
              ReactorTimerId hora;
                         b32 hora_viva;
                         b32 otiosa;
    /* ascensus */
                         Xar* fasciculus;         /* VolumenActum */
                         s64  finis_fasciculi;
                         b32  fasciculus_plenus;
                         Xar* desunt;        /* chorda */
                         i32  index;
    /* descensus */
                         Xar* tracta;        /* VolumenActum */
                         Xar* massae;        /* chorda petendae */
                         s64  cursor_novus;
                         b32  plura;
    /* recensio 3: epocha cycli - responsum epochae priori abicitur */
                         i32 epocha;
} Semita;

structura CommunioCliens {
               Piscina* piscina;
               Reactor* reactor;
            Veredarius* veredarius;
               Volumen* volumen;
             character  basis[BASIS_MAXIMA];
             character  tessera[TESSERA_MAXIMA];
    CommunioAdvenerunt  advenerunt;
                vacuum* ctx;
                   i32  mora;
                   b32  currit;
                   b32  ligatum;
                   s64  ascensus;
                   s64  descensus;
                   i32  missa;
                   i32  recepta;
                   b32  connexus;
             character  error[ERROR_MAXIMUS];
             character* error_ipse;
                   i32  error_mensura;
    /* semita quae errorem posuit: alia eum non delet (recensio 5) */
       constans Semita* error_semita;
    /* recensio 3: replica voluminis servi cuius cursores sunt; mutata
     * = servus alius (volumen novum aut restitutum): cursores ad 0 */
             character servus[XXVIII];
                   i32 epocha;
                Semita sursum;
                Semita deorsum;
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior vacuum
_semita_cyclus (
    Semita* s);

interior vacuum
_clavem (
    CommunioCliens* c,
constans character* nomen_cursoris,
         character* exitus)
{
    sprintf(exitus, "communio/%s/%s", c->basis, nomen_cursoris);
}

interior s64
_cursorem_legere (
    CommunioCliens* c,
constans character* nomen_cursoris)
{
    character clavis[CLAVIS_MAXIMA];
       chorda valor;
          b32 inventum;
          s64 v;
          i32 i;

    _clavem(c, nomen_cursoris, clavis);
    valor = volumen_praesens_legere(c->volumen,
        chorda_ex_literis(clavis,
        c->piscina), c->piscina, &inventum);
    v = ZEPHYRUM;
    per (i = ZEPHYRUM; inventum && i < valor.mensura && i < XVIII; i++)
    {
        si (valor.datum[i] < '0' || valor.datum[i] > '9')
        {
            redde ZEPHYRUM;
        }
        v = v * X + (s64)(valor.datum[i] - '0');
    }
    redde v;
}

interior vacuum
_cursorem_scribere (
    CommunioCliens* c,
constans character* nomen_cursoris,
               s64  valor,
           Piscina* piscina)
{
    character clavis[CLAVIS_MAXIMA];
    character textus[XXIV];

    _clavem(c, nomen_cursoris, clavis);
    sprintf(textus, "%ld", (longus)valor);
    (vacuum)volumen_praesens_ponere(c->volumen,
        chorda_ex_literis(clavis,
        piscina), chorda_ex_literis(textus, piscina));
}

/* error (sine tessera umquam) */
interior vacuum
_errorem_ponere (
       CommunioCliens* c,
   constans character* quid,
                  i32  status,
      constans Semita* semita)
{
    si (status > ZEPHYRUM)
    {
        sprintf(c->error, "%.200s: HTTP %d", quid, (integer)status);
    }
    alioquin
    {
        sprintf(c->error, "%.200s: rete", quid);
    }
    c->error_mensura  = (i32)strlen(c->error);
    c->connexus       = FALSUM;
    c->error_semita   = semita;
}

/* successus semitae: error suus (aut nullius) deletur, alienus manet */
interior vacuum
_errorem_purgare (
      CommunioCliens* c,
     constans Semita* semita)
{
    c->connexus = VERUM;
    si (c->error_semita && c->error_semita != semita)
    {
        redde;
    }
    c->error[ZEPHYRUM]  = '\0';
    c->error_mensura    = ZEPHYRUM;
    c->error_semita     = NIHIL;
}

interior vacuum
_horam_tollere (
    Semita* s)
{
    si (s->hora_viva)
    {
        (vacuum)reactor_timer_cancellare(s->cliens->reactor, s->hora);
    }
    s->hora_viva = FALSUM;
}

interior vacuum
_horam_ponere (
                  Semita* s,
                     i32  ms,
    ReactorTimerCallback  functio)
{
    _horam_tollere(s);
    s->hora = reactor_timer_adicere(s->cliens->reactor, ms, FALSUM,
        functio, s);
    s->hora_viva = s->hora != (ReactorTimerId)-I;
}

interior vacuum
_semita_hora (
    vacuum* datum)
{
    Semita* s = (Semita*)datum;

    s->hora_viva = FALSUM;
    _semita_cyclus(s);
}

/* defectus: error notatur, semita post REPETITIO_MS iterum */
interior vacuum
_defectus (
             Semita* s,
constans character* quid,
                i32  status)
{
    _errorem_ponere(s->cliens, quid, status, s);
    s->otiosa = FALSUM;
    _horam_ponere(s, REPETITIO_MS, _semita_hora);
}

interior vacuum
_progredi_hora (
    vacuum* datum);

interior vacuum
_finis (
                          vacuum* datum,
    constans VeredariusResponsum* r)
{
    Semita* s = (Semita*)datum;

    s->responsum = *r;
    s->respondit = VERUM;
    si (!s->cliens->currit)
    {
        redde;
    }
    /* responsum in gyro proximo tractatur (numquam intra finem: piscina
     * vocationis vivere debet) */
    _horam_ponere(s, I, _progredi_hora);
}

/* petitio UNA (C7): omnis HTTP clientis hic */
interior b32
_mittere (
                  Semita* s,
      VeredariusMethodus  methodus,
      constans character* via,
                  chorda  corpus,
      constans character* mimen,
                     i32  tempus_ms)
{
       CommunioCliens* c;
    VeredariusPetitio  p;
            character  url[BASIS_MAXIMA + VIA_MAXIMA];
            character  auctoritas[LXIV];
                  b32  missa;

    c = s->cliens;
    sprintf(url, "%s%s", c->basis, via);
    sprintf(auctoritas, "Bearer %s", c->tessera);
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.methodus    = methodus;
    p.url         = url;
    p.auctoritas  = auctoritas;
    p.corpus      = corpus;
    p.mimen       = mimen;
    p.tempus_ms   = tempus_ms;
    s->respondit  = FALSUM;
    missa = c->veredarius->petere(c->veredarius->sui, &p, s->piscina,
        _finis, s, &s->signum);
    memset(auctoritas, ZEPHYRUM, magnitudo(auctoritas));
    s->in_itinere = missa;
    redde missa;
}

interior vacuum
_mittere_aut_deficere (
                  Semita* s,
      VeredariusMethodus  methodus,
      constans character* via,
                  chorda  corpus,
      constans character* mimen,
                     i32  tempus_ms)
{
    si (!_mittere(s, methodus, via, corpus, mimen, tempus_ms))
    {
        _defectus(s, via, ZEPHYRUM);
    }
}

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

/* recensio 3: servum agnoscere (replica voluminis eius, ex ligatione
 * aut tractione). Primus: notatur. Alius quam notatus: cursores ad 0,
 * replica iterum ligatur, epocha crescit (responsa in itinere ad servum
 * priorem abiciuntur) - VERUM redditur. */
interior b32
_servum_agnoscere (
    CommunioCliens* c,
            chorda  servus,
           Piscina* piscina)
{
    character clavis[CLAVIS_MAXIMA];
          i32 n;

    si (!communio_replica_valida(servus))
    {
        redde FALSUM;
    }
    n = (i32)strlen(c->servus);
    si (   n                                          == servus.mensura
        && memcmp(c->servus, servus.datum, (size_t)n) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    memcpy(c->servus, servus.datum, (size_t)servus.mensura);
    c->servus[servus.mensura] = '\0';
    _clavem(c, "servus", clavis);
    (vacuum)volumen_praesens_ponere(c->volumen,
        chorda_ex_literis(clavis,
        piscina), chorda_ex_literis(c->servus, piscina));
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    c->ascensus   = ZEPHYRUM;
    c->descensus  = ZEPHYRUM;
    _cursorem_scribere(c, "ascensus", ZEPHYRUM, piscina);
    _cursorem_scribere(c, "descensus", ZEPHYRUM, piscina);
    c->ligatum = FALSUM;
    c->epocha++;
    redde VERUM;
}


/* ==================================================
 * Ascensus
 * ================================================== */

interior vacuum
_acta_mittere (
    Semita* s)
{
    s->gradus = GRADUS_ACTA_MITTIT;
    _mittere_aut_deficere(s, VEREDARIUS_POST, "/communio/acta",
        communio_acta_scribere(s->fasciculus, s->piscina),
        "application/json",
        TEMPUS_PETITIONIS);
}

interior vacuum
_massam_mittere_proximam (
    Semita* s)
{
                chorda  hex;
                chorda  contentum;
                   b32  inventum;
             character  via[VIA_MAXIMA];
    VolumenMassaLector* lector;
                   i64  mensura;

    mensura = ZEPHYRUM;
    si (!s->desunt || s->index >= xar_numerus(s->desunt))
    {
        _acta_mittere(s);
        redde;
    }
    hex = *(chorda*)xar_obtinere(s->desunt, s->index);
    /* recensio 5: mensura PRIUS (massa magna non in memoriam) */
    lector = volumen_massam_aperire(s->cliens->volumen, hex, &mensura,
        s->piscina);
    si (lector)
    {
        volumen_massam_claudere(lector);
    }
    si (mensura > (i64)COMMUNIO_MASSA_MAXIMA)
    {
        _defectus(s, "massa nimia (supra XVI MB)", ZEPHYRUM);
        redde;
    }
    contentum = volumen_massam_promere(s->cliens->volumen, hex,
        s->piscina, &inventum);
    si (!inventum)
    {
        _defectus(s, "massa localis deest", ZEPHYRUM);
        redde;
    }
    sprintf(via, "/communio/massae/%.64s",
        (constans character*)hex.datum);
    s->gradus = GRADUS_MASSAM_MITTIT;
    _mittere_aut_deficere(s, VEREDARIUS_PUT, via, contentum,
        "application/octet-stream", TEMPUS_PETITIONIS);
}

interior vacuum
_ascensum_cyclus (
    Semita* s)
{
    CommunioCliens* c;
               Xar* acta;
               Xar* massae;
         JsonValor* o;
         JsonValor* t;
               i32  octeti;
               i32  i;
               i32  j;
               i32  k;
         character  corpus[XCVI];
            chorda  replica;

    c = s->cliens;
    piscina_vacare(s->piscina);
    s->fasciculus         = NIHIL;
    s->desunt             = NIHIL;
    s->index              = ZEPHYRUM;
    s->fasciculus_plenus  = FALSUM;
    s->epocha             = c->epocha;
    si (!c->ligatum)
    {
        sprintf(corpus, "{\"replica\":\"%.26s\"}",
            (constans character*)volumen_replica(c->volumen).datum);
        s->gradus = GRADUS_LIGAT;
        _mittere_aut_deficere(s, VEREDARIUS_POST, "/communio/apparatus",
            chorda_ex_literis(corpus, s->piscina), "application/json",
            TEMPUS_PETITIONIS);
        redde;
    }
    /* recensio 4: lectio limitata, non omnia post cursorem */
    replica  = volumen_replica(c->volumen);
    acta     = volumen_acta_replicarum_legere(c->volumen, c->ascensus,
        &replica, I, LECTIO_MAXIMA, s->piscina);
    s->fasciculus = xar_creare(s->piscina,
        (i32)magnitudo(VolumenActum));
    massae = xar_creare(s->piscina, (i32)magnitudo(chorda));
    si (!acta || !s->fasciculus || !massae)
    {
        _defectus(s, "volumen", ZEPHYRUM);
        redde;
    }
    s->finis_fasciculi  = c->ascensus;
    octeti              = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(acta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);
                 i32  mensura;

        si (communio_actum_derivatum(a, s->piscina))
        {
            s->finis_fasciculi = a->seq;
            perge;
        }
        mensura = a->datum.mensura + a->genus.mensura
                + a->momentum.mensura + CC;
        si (   xar_numerus(s->fasciculus) >= COMMUNIO_ACTA_MAXIMA
            || (xar_numerus(s->fasciculus) > ZEPHYRUM
                && octeti + mensura > FASCICULI_OCTETI))
        {
            s->fasciculus_plenus = VERUM;
            frange;
        }
        *(VolumenActum*)xar_addere(s->fasciculus)  = *a;
        octeti                                     += mensura;
        s->finis_fasciculi                         = a->seq;
    }
    si (xar_numerus(acta) >= LECTIO_MAXIMA)
    {
        s->fasciculus_plenus = VERUM;
    }
    si (xar_numerus(s->fasciculus) == ZEPHYRUM)
    {
        si (s->finis_fasciculi > c->ascensus)
        {
            c->ascensus = s->finis_fasciculi;
            _cursorem_scribere(c, "ascensus", c->ascensus, s->piscina);
        }
        si (s->fasciculus_plenus)
        {
            /* derivata sola in lectione plena: plura sequuntur */
            _horam_ponere(s, I, _semita_hora);
            redde;
        }
        s->otiosa = VERUM;
        _horam_ponere(s, QUAESTIO_MS, _semita_hora);
        redde;
    }
    s->otiosa = FALSUM;
    /* massae quas fasciculus nominat et hic sunt (unicae) */
    per (i = ZEPHYRUM; i < xar_numerus(s->fasciculus); i++)
    {
        Xar* n = communio_massas_nominatas(
            (VolumenActum*)xar_obtinere(s->fasciculus, i), s->piscina);

        per (j = ZEPHYRUM; n && j < xar_numerus(n); j++)
        {
            chorda h    = *(chorda*)xar_obtinere(n, j);
               b32 iam  = FALSUM;

            per (k = ZEPHYRUM; k < xar_numerus(massae) && !iam; k++)
            {
                iam = chorda_aequalis(*(chorda*)xar_obtinere(massae, k),
                    h);
            }
            si (!iam && volumen_massam_habet(c->volumen, h))
            {
                *(chorda*)xar_addere(massae) = h;
            }
        }
    }
    si (xar_numerus(massae) == ZEPHYRUM)
    {
        _acta_mittere(s);
        redde;
    }
    o = json_objectum_creare(s->piscina);
    t = json_tabulatum_creare(s->piscina);
    per (k = ZEPHYRUM; o && t && k < xar_numerus(massae); k++)
    {
        json_tabulatum_addere(t, json_chorda_creare(s->piscina,
            *(chorda*)xar_obtinere(massae, k)));
    }
    json_objectum_ponere(o, "massae", t);
    s->gradus = GRADUS_DESUNT;
    _mittere_aut_deficere(s, VEREDARIUS_POST, "/communio/massae/desunt",
        json_scribere(o, s->piscina), "application/json",
        TEMPUS_PETITIONIS);
}


/* ==================================================
 * Descensus
 * ================================================== */

interior vacuum
_descensum_cyclus (
    Semita* s)
{
    CommunioCliens* c;
         character  via[VIA_MAXIMA];

    c = s->cliens;
    piscina_vacare(s->piscina);
    s->tracta  = NIHIL;
    s->massae  = NIHIL;
    s->index   = ZEPHYRUM;
    s->epocha  = c->epocha;
    sprintf(via, "/communio/acta?post=%ld&mora=%d",
        (longus)c->descensus,
        (integer)c->mora);
    s->gradus = GRADUS_TRAHIT;
    _mittere_aut_deficere(s, VEREDARIUS_GET, via, _vacua(), NIHIL,
        (c->mora + MORA_MARGO_S) * (i32)M);
}

interior vacuum
_importare (
    Semita* s)
{
    CommunioCliens* c;
               i32  importata;
               i32  i;

    c          = s->cliens;
    importata  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(s->tracta); i++)
    {
        VolumenImportatio x = volumen_actum_importare(c->volumen,
            (VolumenActum*)xar_obtinere(s->tracta, i));

        si (x == VOLUMEN_IMPORTATUM)
        {
            importata++;
        }
        alioquin si (x != VOLUMEN_IAM_ADEST)
        {
            c->recepta += importata;
            si (importata > ZEPHYRUM && c->advenerunt)
            {
                c->advenerunt(c->ctx, importata);
            }
            _defectus(s, "importatio", ZEPHYRUM);
            redde;
        }
    }
    c->descensus  = s->cursor_novus;
    c->recepta    += importata;
    _cursorem_scribere(c, "descensus", c->descensus, s->piscina);
    _errorem_purgare(c, s);
    si (importata > ZEPHYRUM && c->advenerunt)
    {
        c->advenerunt(c->ctx, importata);
    }
    si (!c->currit)
    {
        redde;
    }
    si (s->plura || xar_numerus(s->tracta) > ZEPHYRUM)
    {
        s->otiosa = FALSUM;
        _descensum_cyclus(s);
        redde;
    }
    s->otiosa = VERUM;
    si (c->mora == ZEPHYRUM)
    {
        _horam_ponere(s, QUAESTIO_MS, _semita_hora);
    }
    alioquin
    {
        _descensum_cyclus(s);
    }
}

interior vacuum
_massam_petere_proximam (
    Semita* s)
{
    character via[VIA_MAXIMA];
       chorda hex;

    si (!s->massae || s->index >= xar_numerus(s->massae))
    {
        _importare(s);
        redde;
    }
    hex = *(chorda*)xar_obtinere(s->massae, s->index);
    sprintf(via, "/communio/massae/%.64s",
        (constans character*)hex.datum);
    s->gradus = GRADUS_MASSAM_PETIT;
    _mittere_aut_deficere(s, VEREDARIUS_GET, via, _vacua(), NIHIL,
        TEMPUS_PETITIONIS);
}


/* ==================================================
 * Responsa
 * ================================================== */

interior vacuum
_tractum_legere (
    Semita* s,
    chorda  corpus)
{
    CommunioCliens* c;
      JsonResultus  r;
         JsonValor* m;
            chorda  causa;
               i32  i;

    c          = s->cliens;
    r          = json_legere(corpus, s->piscina);
    s->tracta  = communio_acta_legere(corpus, &causa, s->piscina);
    si (!r.successus || !s->tracta)
    {
        _defectus(s, "tractio mala", ZEPHYRUM);
        redde;
    }
    si (_servum_agnoscere(c, json_capere_chorda(r.radix, "servus",
            _vacua()), s->piscina))
    {
        /* servus alius: tractio haec cursore vetere facta - ab initio,
         * et ascensus statim (replica iterum ligatur) */
        s->otiosa         = FALSUM;
        c->sursum.otiosa  = FALSUM;
        _semita_cyclus(&c->sursum);
        _descensum_cyclus(s);
        redde;
    }
    s->cursor_novus = json_capere_integer(r.radix, "cursor",
        c->descensus);
    s->plura   = json_capere_boolean(r.radix, "plura", FALSUM);
    s->massae  = xar_creare(s->piscina, (i32)magnitudo(chorda));
    m          = json_objectum_capere(r.radix, "massae");
    per (i = ZEPHYRUM; m && s->massae && i < json_tabulatum_numerus(m);
         i++)
    {
        JsonValor* h = json_tabulatum_obtinere(m, i);

        si (   h && h->genus == JSON_CHORDA
            && h->datum.chorda_valor.mensura == LXIV
            && !volumen_massam_habet(c->volumen, h->datum.chorda_valor))
        {
            *(chorda*)xar_addere(s->massae) = h->datum.chorda_valor;
        }
    }
    _massam_petere_proximam(s);
}

interior vacuum
_acta_missa (
    Semita* s,
    chorda  corpus)
{
    CommunioCliens* c;
      JsonResultus  r;
         JsonValor* refusum;
               s64  accepta;

    c = s->cliens;
    r = json_legere(corpus, s->piscina);
    accepta = r.successus ? json_capere_integer(r.radix, "accepta", -I)
                          : -I;
    si (accepta < ZEPHYRUM || accepta > (s64)xar_numerus(s->fasciculus))
    {
        _defectus(s, "acta missa", ZEPHYRUM);
        redde;
    }
    c->missa  += (i32)accepta;
    refusum   = json_objectum_capere(r.radix, "refusum");
    si (accepta == (s64)xar_numerus(s->fasciculus))
    {
        c->ascensus = s->finis_fasciculi;
    }
    alioquin si (accepta > ZEPHYRUM)
    {
        c->ascensus = ((VolumenActum*)xar_obtinere(s->fasciculus,
            (i32)accepta - I))->seq;
    }
    _cursorem_scribere(c, "ascensus", c->ascensus, s->piscina);
    si (refusum && refusum->genus == JSON_OBJECTUM)
    {
        _errorem_ponere(c, "refusum", ZEPHYRUM, s);
        s->otiosa = FALSUM;
        _horam_ponere(s, REPETITIO_MS, _semita_hora);
        redde;
    }
    _errorem_purgare(c, s);
    _ascensum_cyclus(s);
}

interior vacuum
_progredi_hora (
    vacuum* datum)
{
              Semita* s;
                 b32  respondit;
                 i32  status;
              chorda  corpus;
           character  novum[SIGILLUM_HEX_MENSURA];
              chorda  hex;
           JsonValor* d;
        JsonResultus  r;
                 i32  i;

    s              = (Semita*)datum;
    s->hora_viva   = FALSUM;
    respondit      = s->in_itinere && s->respondit;
    s->in_itinere  = FALSUM;
    s->respondit   = FALSUM;
    si (!respondit || !s->cliens->currit)
    {
        redde;
    }
    si (s->epocha != s->cliens->epocha)
    {
        /* recensio 3: responsum servi prioris - cyclus de novo */
        _semita_cyclus(s);
        redde;
    }
    status = s->responsum.successus ? s->responsum.status : ZEPHYRUM;
    corpus = s->responsum.successus ? s->responsum.corpus : _vacua();
    commutatio (s->gradus)
    {
        casus GRADUS_LIGAT:
            si (status != CC)
            {
                _defectus(s, "ligare", status);
                frange;
            }
            r = json_legere(corpus, s->piscina);
            si (r.successus)
            {
                (vacuum)_servum_agnoscere(s->cliens, json_capere_chorda(
                    r.radix, "servus", _vacua()), s->piscina);
            }
            s->cliens->ligatum = VERUM;
            _errorem_purgare(s->cliens, s);
            _ascensum_cyclus(s);
            frange;
        casus GRADUS_DESUNT:
            r = json_legere(corpus, s->piscina);
            d = (status == CC && r.successus)
                ? json_objectum_capere(r.radix, "desunt") : NIHIL;
            si (!d)
            {
                _defectus(s, "massae desunt", status);
                frange;
            }
            s->desunt = xar_creare(s->piscina, (i32)magnitudo(chorda));
            per (i = ZEPHYRUM; s->desunt
                && i < json_tabulatum_numerus(d);
                 i++)
            {
                JsonValor* h = json_tabulatum_obtinere(d, i);

                si (h && h->genus == JSON_CHORDA)
                {
                    *(chorda*)xar_addere(s->desunt) =
                        h->datum.chorda_valor;
                }
            }
            s->index = ZEPHYRUM;
            _massam_mittere_proximam(s);
            frange;
        casus GRADUS_MASSAM_MITTIT:
            si (status != CCI)
            {
                _defectus(s, "massa missa", status);
                frange;
            }
            s->index++;
            _massam_mittere_proximam(s);
            frange;
        casus GRADUS_ACTA_MITTIT:
            si (status != CC)
            {
                _defectus(s, "acta", status);
                frange;
            }
            _acta_missa(s, corpus);
            frange;
        casus GRADUS_TRAHIT:
            si (status != CC)
            {
                _defectus(s, "tractio", status);
                frange;
            }
            _tractum_legere(s, corpus);
            frange;
        casus GRADUS_MASSAM_PETIT:
            hex = *(chorda*)xar_obtinere(s->massae, s->index);
            si (   status                                 != CC
                || !volumen_massam_condere(s->cliens->volumen, corpus,
                       novum)
                || memcmp(novum, hex.datum, (size_t)LXIV) != ZEPHYRUM)
            {
                _defectus(s, "massa petita", status);
                frange;
            }
            s->index++;
            _massam_petere_proximam(s);
            frange;
        ordinarius:
            frange;
    }
}

interior vacuum
_semita_cyclus (
    Semita* s)
{
    si (!s->cliens->currit || s->in_itinere)
    {
        redde;
    }
    _horam_tollere(s);
    si (s->ascendens)
    {
        _ascensum_cyclus(s);
    }
    alioquin
    {
        _descensum_cyclus(s);
    }
}


/* ==================================================
 * Interfacies
 * ================================================== */

interior b32
_semitam_parare (
    CommunioCliens* c,
            Semita* s,
               b32  ascendens)
{
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->cliens     = c;
    s->ascendens  = ascendens;
    s->piscina    = piscina_generare_dynamicum("communio_cliens",
        (memoriae_index)LXIV * (memoriae_index)M);
    redde s->piscina != NIHIL;
}

CommunioCliens*
communio_clientem_creare (
               Piscina* piscina,
               Reactor* reactor,
            Veredarius* veredarius,
               Volumen* volumen,
    constans character* basis,
                chorda  tessera,
    CommunioAdvenerunt  advenerunt,
                vacuum* ctx)
{
    CommunioCliens* c;

    si (   !piscina || !reactor || !veredarius || !volumen || !basis
        || strlen(basis) >= BASIS_MAXIMA || !tessera.datum
        || tessera.mensura <= ZEPHYRUM
        || tessera.mensura >= TESSERA_MAXIMA
        || volumen_replica(volumen).mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    c = (CommunioCliens*)piscina_allocare(piscina,
        magnitudo(CommunioCliens));
    si (!c)
    {
        redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->piscina     = piscina;
    c->reactor     = reactor;
    c->veredarius  = veredarius;
    c->volumen     = volumen;
    c->advenerunt  = advenerunt;
    c->ctx         = ctx;
    c->mora        = MORA_ORDINARIA;
    c->error_ipse  = c->error;
    strcpy(c->basis, basis);
    memcpy(c->tessera, tessera.datum, (size_t)tessera.mensura);
    c->tessera[tessera.mensura] = '\0';
    si (   !_semitam_parare(c, &c->sursum, VERUM)
        || !_semitam_parare(c, &c->deorsum, FALSUM))
    {
        redde NIHIL;
    }
    c->ascensus   = _cursorem_legere(c, "ascensus");
    c->descensus  = _cursorem_legere(c, "descensus");
    {
        character clavis[CLAVIS_MAXIMA];
           chorda valor;
              b32 inventum;

        _clavem(c, "servus", clavis);
        valor = volumen_praesens_legere(c->volumen, chorda_ex_literis(
            clavis, c->piscina), c->piscina, &inventum);
        si (   inventum && valor.mensura > ZEPHYRUM
            && valor.mensura < (i32)magnitudo(c->servus))
        {
            memcpy(c->servus, valor.datum, (size_t)valor.mensura);
        }
    }
    redde c;
}

vacuum
communio_cliens_moram_ponere (
    CommunioCliens* cliens,
               i32  secundae)
{
    si (cliens)
    {
        cliens->mora = secundae > MORA_ORDINARIA ? MORA_ORDINARIA
                                                 : secundae;
    }
}

vacuum
communio_cliens_incipere (
    CommunioCliens* cliens)
{
    si (!cliens || cliens->currit)
    {
        redde;
    }
    cliens->currit = VERUM;
    _semita_cyclus(&cliens->sursum);
    _semita_cyclus(&cliens->deorsum);
}

vacuum
communio_cliens_excitare (
    CommunioCliens* cliens)
{
    si (!cliens || !cliens->currit)
    {
        redde;
    }
    cliens->sursum.otiosa   = FALSUM;
    cliens->deorsum.otiosa  = FALSUM;
    _semita_cyclus(&cliens->sursum);
    _semita_cyclus(&cliens->deorsum);
}

interior vacuum
_semitam_sistere (
    Semita* s)
{
    Veredarius* v;

    _horam_tollere(s);
    v = s->cliens->veredarius;
    si (s->in_itinere && !s->respondit)
    {
        v->abrumpere(v->sui, s->signum);
    }
    s->in_itinere  = FALSUM;
    s->respondit   = FALSUM;
    _horam_tollere(s);
}

vacuum
communio_cliens_sistere (
    CommunioCliens* cliens)
{
    si (!cliens)
    {
        redde;
    }
    cliens->currit = FALSUM;
    _semitam_sistere(&cliens->sursum);
    _semitam_sistere(&cliens->deorsum);
}

vacuum
communio_clientem_destruere (
    CommunioCliens* cliens)
{
    si (!cliens)
    {
        redde;
    }
    communio_cliens_sistere(cliens);
    memset(cliens->tessera, ZEPHYRUM, magnitudo(cliens->tessera));
    si (cliens->sursum.piscina)
    {
        piscina_destruere(cliens->sursum.piscina);
        cliens->sursum.piscina = NIHIL;
    }
    si (cliens->deorsum.piscina)
    {
        piscina_destruere(cliens->deorsum.piscina);
        cliens->deorsum.piscina = NIHIL;
    }
}

CommunioStatus
communio_cliens_status (
    constans CommunioCliens* cliens)
{
    CommunioStatus s;

    memset(&s, ZEPHYRUM, magnitudo(s));
    si (!cliens)
    {
        redde s;
    }
    s.ascensus       = cliens->ascensus;
    s.descensus      = cliens->descensus;
    s.missa          = cliens->missa;
    s.recepta        = cliens->recepta;
    s.connexus       = cliens->connexus;
    s.error.datum    = (i8*)cliens->error_ipse;
    s.error.mensura  = cliens->error_mensura;
    redde s;
}

b32
communio_cliens_otiosus (
    constans CommunioCliens* cliens)
{
    redde cliens && cliens->currit
        && cliens->sursum.otiosa && cliens->deorsum.otiosa
        && !cliens->sursum.in_itinere && !cliens->deorsum.in_itinere;
}


/* ==================================================
 * Tessera in filo (T7)
 * ================================================== */

b32
communio_tesseram_scribere (
     constans character* via,
              character  digestum[LXV])
{
     character tessera[XLIV];
       integer fd;
       ssize_t n;
           b32 bene;

    si (   !via || !digestum
        || !communio_tesseram_facere(tessera, digestum))
    {
        redde FALSUM;
    }
    /* <tolera codex="CANALIS_PRAETERITUS" (>tessera secreta */
    fd = open(via, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
    si (fd < 0)
    {
        memset(tessera, ZEPHYRUM, magnitudo(tessera));
        fprintf(stderr,
            "communio: tessera scribi nequit (iam exsistit?):"
            " %s\n", via);
        redde FALSUM;
    }
    n     = write(fd, tessera, (size_t)COMMUNIO_TESSERA_MENSURA);
    bene  = n == (ssize_t)COMMUNIO_TESSERA_MENSURA;
    bene  = close(fd) == ZEPHYRUM && bene;
    memset(tessera, ZEPHYRUM, magnitudo(tessera));
    si (!bene)
    {
        (vacuum)filum_delere(via);
        memset(digestum, ZEPHYRUM, (size_t)LXV);
        fprintf(stderr, "communio: tessera scribi nequit: %s\n", via);
    }
    redde bene;
}

b32
communio_tesseram_legere (
     constans character* via,
              character  exitus[XLIV])
{
    structura stat st;
         integer fd;
         ssize_t n;

    si (!via || !exitus)
    {
        redde FALSUM;
    }
    /* <tolera codex="CANALIS_PRAETERITUS" (>tessera secreta */
    fd = open(via, O_RDONLY | O_NOFOLLOW);
    si (fd < 0)
    {
        fprintf(stderr, "communio: tessera legi nequit: %s\n", via);
        redde FALSUM;
    }
    si (fstat(fd, &st) != ZEPHYRUM || !S_ISREG(st.st_mode))
    {
        (vacuum)close(fd);
        fprintf(stderr, "communio: tessera non filum regulare: %s\n",
            via);
        redde FALSUM;
    }
    si ((st.st_mode & 077) != ZEPHYRUM)
    {
        (vacuum)close(fd);
        fprintf(stderr,
            "communio: tessera modus latior quam 0600 (chmod"
            " 600 %s)\n", via);
        redde FALSUM;
    }
    n = read(fd, exitus, (size_t)(XLIV - I));
    (vacuum)close(fd);
    si (n > ZEPHYRUM && exitus[n - I] == '\n')
    {
        n--;
    }
    si (n != (ssize_t)COMMUNIO_TESSERA_MENSURA)
    {
        memset(exitus, ZEPHYRUM, (size_t)XLIV);
        fprintf(stderr, "communio: tessera mala (XLIII signa): %s\n",
            via);
        redde FALSUM;
    }
    exitus[n] = '\0';
    redde VERUM;
}


/* ==================================================
 * Inscriptio per epistulam (plan-3 T6)
 * ================================================== */

/* petitio una sincrona: finis hic notatur */
nomen structura {
                    b32 finita;
    VeredariusResponsum responsum;
} Semel;

interior vacuum
_finis_semel (
                          vacuum* datum,
    constans VeredariusResponsum* r)
{
    Semel* m = (Semel*)datum;

    m->responsum  = *r;
    m->finita     = VERUM;
}

/* petitio una sincrona per veredarium (instrumentum; plan-5 T2 eam
 * removet); status 0 = rete aut tempus */
interior i32
_vocare_semel (
               Reactor* reactor,
            Veredarius* veredarius,
    VeredariusMethodus  methodus,
    constans character* url,
    constans character* tessera,
                chorda  corpus,
                chorda* responsum,
               Piscina* piscina)
{
    VeredariusPetitio p;
                Semel m;
            character auctoritas[LXIV];
                  i32 signum;
                  i32 gyri;
                  b32 missa;

    responsum->datum    = NIHIL;
    responsum->mensura  = ZEPHYRUM;
    memset(&p, ZEPHYRUM, magnitudo(p));
    memset(&m, ZEPHYRUM, magnitudo(m));
    p.methodus   = methodus;
    p.url        = url;
    p.tempus_ms  = TEMPUS_PETITIONIS;
    si (tessera)
    {
        sprintf(auctoritas, "Bearer %.43s", tessera);
        p.auctoritas = auctoritas;
    }
    si (corpus.mensura > ZEPHYRUM)
    {
        p.corpus  = corpus;
        p.mimen   = "application/json";
    }
    missa = veredarius->petere(veredarius->sui, &p, piscina,
        _finis_semel, &m, &signum);
    memset(auctoritas, ZEPHYRUM, magnitudo(auctoritas));
    per (gyri = ZEPHYRUM; missa && !m.finita && gyri < (i32)MMM; gyri++)
    {
        (vacuum)reactor_poll(reactor, X);
    }
    si (!missa)
    {
        redde ZEPHYRUM;
    }
    si (!m.finita)
    {
        veredarius->abrumpere(veredarius->sui, signum);
        redde ZEPHYRUM;
    }
    si (!m.responsum.successus)
    {
        redde ZEPHYRUM;
    }
    *responsum = m.responsum.corpus;
    redde m.responsum.status;
}

/* campus chordae ex corpore JSON in buffer (mensura exacta) */
interior b32
_campum_copiare (
                chorda  corpus,
    constans character* clavis,
                   i32  mensura,
             character* exitus,
               Piscina* piscina)
{
    JsonResultus r;
          chorda c;
          chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    r              = json_legere(corpus, piscina);
    c = r.successus ? json_capere_chorda(r.radix, clavis,
        vacua) : vacua;
    si (c.mensura != mensura)
    {
        redde FALSUM;
    }
    memcpy(exitus, c.datum, (size_t)mensura);
    exitus[mensura] = '\0';
    redde VERUM;
}

b32
communio_inscriptionem_petere (
               Reactor* reactor,
            Veredarius* veredarius,
    constans character* basis,
                chorda  epistula,
    constans character* via_tesserae,
                chorda  titulus,
             character  aditus_exitus[MONETA_ULID_MENSURA],
               Piscina* piscina)
{
     JsonValor* o;
     character  digestum[LXV];
     character  url[BASIS_MAXIMA + VIA_MAXIMA];
        chorda  responsum;
           i32  status;

    si (   !reactor || !veredarius || !basis || !via_tesserae
        || !aditus_exitus
        || strlen(basis) >= BASIS_MAXIMA)
    {
        redde FALSUM;
    }
    si (!communio_tesseram_scribere(via_tesserae, digestum))
    {
        redde FALSUM;
    }
    o = json_objectum_creare(piscina);
    si (!o)
    {
        (vacuum)filum_delere(via_tesserae);
        redde FALSUM;
    }
    json_objectum_ponere(o, "epistula", json_chorda_creare(piscina,
        epistula));
    json_objectum_ponere(o, "digestum",
        json_chorda_creare_literis(piscina,
        digestum));
    json_objectum_ponere(o, "titulus", json_chorda_creare(piscina,
        titulus));
    sprintf(url, "%s/communio/intrare", basis);
    status = _vocare_semel(reactor, veredarius, VEREDARIUS_POST, url,
        NIHIL,
        json_scribere(o,
        piscina), &responsum, piscina);
    si (   status != CCII
        || !_campum_copiare(responsum, "aditus", XXVI, aditus_exitus,
               piscina))
    {
        (vacuum)filum_delere(via_tesserae);
        fprintf(stderr,
            "communio: inscriptio petita deficit (HTTP %d)\n",
            (integer)status);
        redde FALSUM;
    }
    redde VERUM;
}

s32
communio_inscriptionem_confirmare (
               Reactor* reactor,
            Veredarius* veredarius,
    constans character* basis,
                chorda  aditus,
                chorda  codex,
             character  apparatus_exitus[MONETA_ULID_MENSURA],
               Piscina* piscina)
{
     JsonValor* o;
     character  url[BASIS_MAXIMA + VIA_MAXIMA];
        chorda  responsum;
           i32  status;

    si (   !reactor || !veredarius || !basis || !apparatus_exitus
        || strlen(basis) >= BASIS_MAXIMA)
    {
        redde -I;
    }
    o = json_objectum_creare(piscina);
    si (!o)
    {
        redde -I;
    }
    json_objectum_ponere(o, "aditus", json_chorda_creare(piscina,
        aditus));
    json_objectum_ponere(o, "codex", json_chorda_creare(piscina,
        codex));
    sprintf(url, "%s/communio/confirmare", basis);
    status = _vocare_semel(reactor, veredarius, VEREDARIUS_POST, url,
        NIHIL,
        json_scribere(o,
        piscina), &responsum, piscina);
    si (status == ZEPHYRUM)
    {
        redde -II;
    }
    si (   status != CC
        || !_campum_copiare(responsum, "apparatus", XXVI,
               apparatus_exitus, piscina))
    {
        redde -I;
    }
    redde ZEPHYRUM;
}

b32
communio_tessera_valet (
               Reactor* reactor,
            Veredarius* veredarius,
    constans character* basis,
    constans character* via_tesserae,
               Piscina* piscina)
{
    character tessera[XLIV];
    character url[BASIS_MAXIMA + VIA_MAXIMA];
       chorda responsum;
       chorda vacua;
          i32 status;

    si (   !reactor || !veredarius || !basis
        || strlen(basis) >= BASIS_MAXIMA
        || !communio_tesseram_legere(via_tesserae, tessera))
    {
        redde FALSUM;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    sprintf(url, "%s/communio/apparatus", basis);
    status = _vocare_semel(reactor, veredarius, VEREDARIUS_GET, url,
        tessera, vacua,
        &responsum, piscina);
    memset(tessera, ZEPHYRUM, magnitudo(tessera));
    redde status == CC;
}
