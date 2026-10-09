/* vates.c - provisor neuter exemplarium linguae (vates-spec;
 * vates-plan-2). Partes: A exemplar neutrum + corpus Anthropic (T3),
 * B lectio (T4), C provisores + mittere + rationarium + fictus (T5). */
#include "postulata_posix.h"
#include "vates.h"
#include "herbarium.h"
#include "norma.h"
#include "vates_responsum_norma.h"
#include "filum.h"
#include "fasti.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define VATES_SIGNA_ORDINARIA        (XVI * M)   /* max_tokens */
#define VATES_PUNCTA_THESAURI_MAXIMA IV

nomen structura {
        chorda  provisor;
        chorda  clavis;      /* campus corporis aut titulus capitis */
     JsonValor* valor;       /* extra */
        chorda  textus;      /* valor capitis */
} VatesEffugium;

structura VatesPetitio {
    Piscina* piscina;
     chorda  exemplar;
        s32  signa_maxima;
     chorda  conatus;
        b32  cogitatio_monstranda;
        b32  cauda_signata;
        Xar* systema;       /* VatesBlocus */
        Xar* instrumenta;   /* VatesInstrumentum */
        Xar* nuntii;        /* VatesNuntius */
        Xar* extra;         /* VatesEffugium */
        Xar* capita;        /* VatesEffugium */
};


/* ======================================================================
 * A. EXEMPLAR NEUTRUM
 * ====================================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = 0;
    redde v;
}

VatesPetitio*
vates_petitio_creare (
     Piscina* piscina,
      chorda  exemplar)
{
    VatesPetitio* p;

    si (!piscina)
    {
        redde NIHIL;
    }
    p = (VatesPetitio*)piscina_allocare(piscina,
        (i64)magnitudo(VatesPetitio));
    memset(p, 0, magnitudo(*p));
    p->piscina   = piscina;
    p->exemplar  = exemplar;
    p->systema   = xar_creare(piscina, (i32)magnitudo(VatesBlocus));
    p->instrumenta = xar_creare(piscina,
        (i32)magnitudo(VatesInstrumentum));
    p->nuntii  = xar_creare(piscina, (i32)magnitudo(VatesNuntius));
    p->extra   = xar_creare(piscina, (i32)magnitudo(VatesEffugium));
    p->capita  = xar_creare(piscina, (i32)magnitudo(VatesEffugium));
    redde p;
}

vacuum
vates_petitio_maxima_ponere (
    VatesPetitio* petitio,
             s32  signa_maxima)
{
    si (petitio)
    {
        petitio->signa_maxima = signa_maxima;
    }
}

vacuum
vates_petitio_conatum_ponere (
    VatesPetitio* petitio,
          chorda  conatus)
{
    si (petitio)
    {
        petitio->conatus = conatus;
    }
}

vacuum
vates_petitio_cogitationem_monstrare (
    VatesPetitio* petitio,
             b32  monstrare)
{
    si (petitio)
    {
        petitio->cogitatio_monstranda = monstrare;
    }
}

vacuum
vates_petitio_caudam_signare (
    VatesPetitio* petitio,
             b32  signare)
{
    si (petitio)
    {
        petitio->cauda_signata = signare;
    }
}

vacuum
vates_systema_addere (
    VatesPetitio* petitio,
          chorda  textus,
             b32  signum_thesauri)
{
    VatesBlocus* b;

    si (!petitio)
    {
        redde;
    }
    b = (VatesBlocus*)xar_addere(petitio->systema);
    memset(b, 0, magnitudo(*b));
    b->genus            = VATES_TEXTUS;
    b->textus           = textus;
    b->signum_thesauri  = signum_thesauri;
}

vacuum
vates_instrumentum_addere (
                  VatesPetitio* petitio,
    constans VatesInstrumentum* instrumentum)
{
    si (petitio && instrumentum)
    {
        *(VatesInstrumentum*)xar_addere(petitio->instrumenta) =
            *instrumentum;
    }
}

VatesNuntius*
vates_nuntium_addere (
     VatesPetitio* petitio,
       VatesMunus  munus)
{
    VatesNuntius* n;

    si (!petitio)
    {
        redde NIHIL;
    }
    n         = (VatesNuntius*)xar_addere(petitio->nuntii);
    n->munus  = munus;
    n->bloci = xar_creare(petitio->piscina,
        (i32)magnitudo(VatesBlocus));
    redde n;
}

VatesBlocus*
vates_textum_addere (
     VatesNuntius* nuntius,
           chorda  textus)
{
    VatesBlocus* b;

    si (!nuntius)
    {
        redde NIHIL;
    }
    b = (VatesBlocus*)xar_addere(nuntius->bloci);
    memset(b, 0, magnitudo(*b));
    b->genus   = VATES_TEXTUS;
    b->textus  = textus;
    redde b;
}

VatesBlocus*
vates_effectum_addere (
     VatesNuntius* nuntius,
           chorda  id,
           chorda  contentum,
              b32  erratum)
{
    VatesBlocus* b = vates_textum_addere(nuntius, contentum);

    si (b)
    {
        b->genus    = VATES_INSTRUMENTI_EFFECTUS;
        b->id       = id;
        b->erratum  = erratum;
    }
    redde b;
}

vacuum
vates_extra_ponere (
          VatesPetitio* petitio,
    constans character* provisor,
    constans character* clavis,
             JsonValor* valor)
{
    VatesEffugium* e;

    si (!petitio || !provisor || !clavis)
    {
        redde;
    }
    e = (VatesEffugium*)xar_addere(petitio->extra);
    memset(e, 0, magnitudo(*e));
    e->provisor  = chorda_ex_literis(provisor, petitio->piscina);
    e->clavis    = chorda_ex_literis(clavis, petitio->piscina);
    e->valor     = valor;
}

vacuum
vates_caput_addere (
          VatesPetitio* petitio,
    constans character* provisor,
    constans character* titulus,
    constans character* valor)
{
    VatesEffugium* e;

    si (!petitio || !provisor || !titulus || !valor)
    {
        redde;
    }
    e = (VatesEffugium*)xar_addere(petitio->capita);
    memset(e, 0, magnitudo(*e));
    e->provisor  = chorda_ex_literis(provisor, petitio->piscina);
    e->clavis    = chorda_ex_literis(titulus, petitio->piscina);
    e->textus    = chorda_ex_literis(valor, petitio->piscina);
}

vacuum
vates_responsum_addere (
               VatesPetitio* petitio,
    constans VatesResponsum* responsum)
{
    VatesNuntius* n;
             i32  i;

    si (   !petitio || !responsum || !responsum->bloci
        || xar_numerus(responsum->bloci) == 0)
    {
        redde;
    }
    n = vates_nuntium_addere(petitio, VATES_ASSISTENS);
    per (i = 0; i < xar_numerus(responsum->bloci); i++)
    {
        *(VatesBlocus*)xar_addere(n->bloci) =
            *(VatesBlocus*)xar_obtinere(responsum->bloci, i);
    }
}


/* ======================================================================
 * A. CORPUS ANTHROPIC
 * ====================================================================== */

interior JsonValor*
_signum_cache (
    Piscina* piscina)
{
    JsonValor* cc = json_objectum_creare(piscina);

    json_objectum_ponere(cc, "type", json_chorda_creare_literis(piscina,
        "ephemeral"));
    redde cc;
}

interior b32
_ab_anthropic (
    constans VatesBlocus* b)
{
    redde b->crudum != NIHIL
        && chorda_aequalis_literis(b->provisor, "anthropic");
}

/* copia levis objecti + cache_control (crudum ipsum numquam mutatur) */
interior JsonValor*
_copia_signata (
    JsonValor* crudum,
      Piscina* piscina)
{
               JsonValor* o   = json_objectum_creare(piscina);
    JsonObjectumIterator  it  = json_objectum_iterator(crudum);
                  chorda  clavis;
               JsonValor* valor;

    dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
    {
        json_objectum_ponere_chorda(o, clavis, valor);
    }
    json_objectum_ponere(o, "cache_control", _signum_cache(piscina));
    redde o;
}

interior JsonValor*
_blocum_anthropic (
    constans VatesBlocus* b,
                 Piscina* piscina)
{
    JsonValor* o = NIHIL;

    si (b->genus == VATES_OPACUM)
    {
        redde _ab_anthropic(b) ? b->crudum : NIHIL;
    }
    si (_ab_anthropic(b))
    {
        redde b->signum_thesauri ? _copia_signata(b->crudum,
            piscina) : b->crudum;
    }
    o = json_objectum_creare(piscina);
    commutatio (b->genus)
    {
        casus VATES_TEXTUS:
            json_objectum_ponere(o, "type",
                json_chorda_creare_literis(piscina, "text"));
            json_objectum_ponere(o, "text", json_chorda_creare(piscina,
                b->textus));
            frange;
        casus VATES_INSTRUMENTUM_PETITUM:
            json_objectum_ponere(o, "type",
                json_chorda_creare_literis(piscina, "tool_use"));
            json_objectum_ponere(o, "id", json_chorda_creare(piscina,
                b->id));
            json_objectum_ponere(o, "name", json_chorda_creare(piscina,
                b->titulus));
            json_objectum_ponere(o, "input",
                b->input ? b->input : json_objectum_creare(piscina));
            frange;
        casus VATES_INSTRUMENTI_EFFECTUS:
            json_objectum_ponere(o, "type",
                json_chorda_creare_literis(piscina, "tool_result"));
            json_objectum_ponere(o, "tool_use_id",
                json_chorda_creare(piscina, b->id));
            json_objectum_ponere(o, "content",
                json_chorda_creare(piscina, b->textus));
            si (b->erratum)
            {
                json_objectum_ponere(o, "is_error",
                    json_boolean_creare(piscina, VERUM));
            }
            frange;
        ordinarius:
            redde NIHIL;
    }
    si (b->signum_thesauri)
    {
        json_objectum_ponere(o, "cache_control",
            _signum_cache(piscina));
    }
    redde o;
}

interior JsonValor*
_contentum_anthropic (
        Xar* bloci,
    Piscina* piscina)
{
    JsonValor* arr = json_tabulatum_creare(piscina);
          i32  i;

    per (i = 0; i < xar_numerus(bloci); i++)
    {
        JsonValor* o =
            _blocum_anthropic((VatesBlocus*)xar_obtinere(bloci, i),
                                         piscina);

        si (o)
        {
            json_tabulatum_addere(arr, o);
        }
    }
    redde arr;
}

interior JsonValor*
_corpus_anthropic (constans VatesPetitio* pe, Piscina* piscina)
{
    JsonValor* radix = json_objectum_creare(piscina);
    JsonValor* nuntii;
          i32  i;

    json_objectum_ponere(radix, "model", json_chorda_creare(piscina,
        pe->exemplar));
    json_objectum_ponere(radix, "max_tokens",
        json_integer_creare(piscina,
        pe->signa_maxima
            > 0 ? (s64)pe->signa_maxima : (s64)VATES_SIGNA_ORDINARIA));
    si (pe->cogitatio_monstranda)
    {
        JsonValor* c = json_objectum_creare(piscina);

        json_objectum_ponere(c, "type",
            json_chorda_creare_literis(piscina, "adaptive"));
        json_objectum_ponere(c, "display",
            json_chorda_creare_literis(piscina, "summarized"));
        json_objectum_ponere(radix, "thinking", c);
    }
    si (pe->conatus.mensura > 0)
    {
        JsonValor* c = json_objectum_creare(piscina);

        json_objectum_ponere(c, "effort", json_chorda_creare(piscina,
            pe->conatus));
        json_objectum_ponere(radix, "output_config", c);
    }
    si (pe->cauda_signata)
    {
        json_objectum_ponere(radix, "cache_control",
            _signum_cache(piscina));
    }
    si (xar_numerus(pe->systema) > 0)
    {
        json_objectum_ponere(radix, "system",
            _contentum_anthropic(pe->systema, piscina));
    }
    si (xar_numerus(pe->instrumenta) > 0)
    {
        JsonValor* arr = json_tabulatum_creare(piscina);

        per (i = 0; i < xar_numerus(pe->instrumenta); i++)
        {
            VatesInstrumentum* t =
                (VatesInstrumentum*)xar_obtinere(pe->instrumenta, i);
                    JsonValor* o =
                        json_objectum_creare(piscina);

            json_objectum_ponere(o, "name", json_chorda_creare(piscina,
                t->titulus));
            json_objectum_ponere(o, "description",
                json_chorda_creare(piscina, t->descriptio));
            json_objectum_ponere(o, "input_schema",
                t->schema ? t->schema : json_objectum_creare(piscina));
            json_tabulatum_addere(arr, o);
        }
        json_objectum_ponere(radix, "tools", arr);
    }
    nuntii = json_tabulatum_creare(piscina);
    per (i = 0; i < xar_numerus(pe->nuntii); i++)
    {
        VatesNuntius* n = (VatesNuntius*)xar_obtinere(pe->nuntii,
            i);
           JsonValor* content = _contentum_anthropic(n->bloci,
               piscina);
           JsonValor* o;

        si (json_tabulatum_numerus(content) == 0)
        {
            perge;   /* nuntius vacuus omittitur (Review focus 5) */
        }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "role",
            json_chorda_creare_literis(piscina,
            n->munus == VATES_USOR ? "user" : "assistant"));
        json_objectum_ponere(o, "content", content);
        json_tabulatum_addere(nuntii, o);
    }
    json_objectum_ponere(radix, "messages", nuntii);
    per (i = 0; i < xar_numerus(pe->extra); i++)
    {
        VatesEffugium* e = (VatesEffugium*)xar_obtinere(pe->extra, i);

        si (chorda_aequalis_literis(e->provisor, "anthropic"))
        {
            json_objectum_ponere_chorda(radix, e->clavis, e->valor);
        }
    }
    redde radix;
}

chorda
vates_anthropic_corpus (
    constans VatesPetitio* petitio,
                  Piscina* piscina)
{
    si (!petitio || !piscina)
    {
        redde _vacua();
    }
    redde json_scribere(_corpus_anthropic(petitio, piscina), piscina);
}


/* ======================================================================
 * B. LECTIO ANTHROPIC
 * ====================================================================== */

/* prima novitas sola servatur */
interior vacuum
_novitas (
                chorda* novitas,
    constans character* praefixum,
                chorda  quid,
               Piscina* piscina)
{
    character buffer[CCLVI];

    si (novitas->mensura > 0)
    {
        redde;
    }
    sprintf(buffer, "%s%.*s", praefixum,
            (integer)(quid.mensura > CC ? CC : quid.mensura),
            (constans character*)quid.datum);
    *novitas = chorda_ex_literis(buffer, piscina);
}

/* "$.content[N].type" -> typus blocu N (nota VARIATIO); aliter via ipsa */
interior chorda
_typum_ex_via (
    JsonValor* radix,
       chorda  via,
      Piscina* p)
{
    constans character* praefixum             = "$.content[";
                   i32  lp     =
                       (i32)strlen(praefixum);
                   i32  index  = 0;
                   i32  i;
             JsonValor* content;
             JsonValor* blocus;

    (vacuum)p;
    si (   via.mensura                              <= lp
        || memcmp(via.datum, praefixum, (size_t)lp) != 0)
    {
        redde via;
    }
    per (i = lp; i < via.mensura && via.datum[i] >= '0'
        && via.datum[i] <= '9'; i++)
    {
        index = index * X + (i32)(via.datum[i] - '0');
    }
    content = json_objectum_capere(radix, "content");
    blocus = content ? json_tabulatum_obtinere(content, index) : NIHIL;
    redde json_capere_chorda(blocus, "type", via);
}

/* novitas = notae (et vitia) normae, OMNES, '; ' iunctae. Nota VARIATIO
 * in content -> "blocus ignotus: <type>" (forma veterum probationum). */
interior vacuum
_novitates_ex_norma (
    JsonValor* radix,
       chorda* novitas,
      Piscina* p)
{
        NormaIudicium j = norma_iudicare(vates_norma_responsum(p),
            radix,
            p);
    ChordaAedificator* aed = chorda_aedificator_creare(p, CCLVI);
                  i32  i;

    si (novitas->mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, *novitas);
    }
    per (i = 0; i < xar_numerus(j.notae) + xar_numerus(j.vitia); i++)
    {
                b32  nota = i < xar_numerus(j.notae);
        NormaVitium* v =
            (NormaVitium*)xar_obtinere(nota ? j.notae : j.vitia,
                             nota ? i : i - xar_numerus(j.notae));

        si (chorda_aedificator_longitudo(aed) > 0)
        {
            chorda_aedificator_appendere_literis(aed, "; ");
        }
        si (nota && v->causa == NORMA_CAUSA_VARIATIO)
        {
            chorda_aedificator_appendere_literis(aed,
                "blocus ignotus: ");
            chorda_aedificator_appendere_chorda(aed,
                _typum_ex_via(radix, v->via, p));
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aed,
                nota ? "campus ignotus: " : "forma fracta: ");
            chorda_aedificator_appendere_chorda(aed, v->via);
        }
    }
    *novitas = chorda_aedificator_finire(aed);
}

interior VatesCausaFinis
_causa_finis (
     chorda  s,
     chorda* novitas,
    Piscina* piscina)
{
    si (   chorda_aequalis_literis(s, "end_turn")
        || chorda_aequalis_literis(s, "stop_sequence"))
    {
        redde VATES_FINIS;
    }
    si (chorda_aequalis_literis(s, "tool_use"))
    {
        redde VATES_FINIS_INSTRUMENTUM;
    }
    si (chorda_aequalis_literis(s, "max_tokens"))
    {
        redde VATES_FINIS_MAXIMUM;
    }
    si (chorda_aequalis_literis(s, "refusal"))
    {
        redde VATES_FINIS_RECUSATIO;
    }
    si (chorda_aequalis_literis(s, "pause_turn"))
    {
        redde VATES_FINIS_PAUSA;
    }
    _novitas(novitas, "stop_reason ignota: ", s, piscina);
    redde VATES_FINIS_ALIA;
}

interior VatesResponsum*
_responsum_vacuum (
    Piscina* piscina)
{
    VatesResponsum* r = (VatesResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(VatesResponsum));

    memset(r, 0, magnitudo(*r));
    r->bloci         = xar_creare(piscina, (i32)magnitudo(VatesBlocus));
    r->usus.pretium  = -I;
    redde r;
}

interior VatesResponsum*
_legere (
       i32  status,
    chorda  corpus,
   Piscina* piscina,
    chorda* novitas)
{
    VatesResponsum* r      = _responsum_vacuum(piscina);
      JsonResultus  j      = json_legere(corpus, piscina);
            chorda  vacua  = chorda_ex_literis("", piscina);
         JsonValor* content;
         JsonValor* usus;
               i32  i;

    *novitas        = _vacua();
    r->status_http  = status;
    r->conatus      = I;
    si (status != CC)
    {
        r->error = VATES_ERROR_STATUS;
        si (j.successus)
        {
            JsonValor* e = json_objectum_capere(j.radix, "error");

            r->error_genus    = json_capere_chorda(e, "type", vacua);
            r->error_nuntius  = json_capere_chorda(e, "message", vacua);
        }
        alioquin
        {
            r->error_nuntius.datum = corpus.datum;
            r->error_nuntius.mensura = corpus.mensura
                > CCLVI ? CCLVI : corpus.mensura;
        }
        redde r;
    }
    si (!j.successus || !json_est_objectum(j.radix))
    {
        r->error         = VATES_ERROR_PARSE;
        r->error_nuntius =
            j.successus ? chorda_ex_literis("radix non objectum",
            piscina)
                                       : j.error;
        redde r;
    }
    r->id        = json_capere_chorda(j.radix, "id", vacua);
    r->exemplar  = json_capere_chorda(j.radix, "model", vacua);
    r->causa_finis_cruda = json_capere_chorda(j.radix, "stop_reason",
        vacua);
    r->causa_finis = _causa_finis(r->causa_finis_cruda, novitas,
        piscina);
    r->recusatio_categoria = json_capere_chorda(
        json_objectum_capere(j.radix, "stop_details"), "category",
        vacua);

    content = json_objectum_capere(j.radix, "content");
    per (i = 0; content && i < json_tabulatum_numerus(content); i++)
    {
          JsonValor* e      = json_tabulatum_obtinere(content, i);
             chorda  typus  = json_capere_chorda(e, "type", vacua);
        VatesBlocus* b      = (VatesBlocus*)xar_addere(r->bloci);

        memset(b, 0, magnitudo(*b));
        b->crudum    = e;
        b->provisor  = chorda_ex_literis("anthropic", piscina);
        si (chorda_aequalis_literis(typus, "text"))
        {
            b->genus   = VATES_TEXTUS;
            b->textus  = json_capere_chorda(e, "text", vacua);
        }
        alioquin si (chorda_aequalis_literis(typus, "tool_use"))
        {
            b->genus    = VATES_INSTRUMENTUM_PETITUM;
            b->id       = json_capere_chorda(e, "id", vacua);
            b->titulus  = json_capere_chorda(e, "name", vacua);
            b->input    = json_objectum_capere(e, "input");
        }
        alioquin
        {
            b->genus = VATES_OPACUM;
        }
    }

    /* novitates per schema: IUDEX in vectura (herbarium-spec-2), ante
     * hanc lectionem - non hic */

    usus           = json_objectum_capere(j.radix, "usage");
    r->usus.input  = json_capere_integer(usus, "input_tokens", 0);
    r->usus.output = json_capere_integer(usus, "output_tokens",
        0);
    r->usus.cache_lectum = json_capere_integer(usus,
        "cache_read_input_tokens", 0);
    si (json_objectum_capere(usus, "cache_creation"))
    {
        JsonValor* cc = json_objectum_capere(usus, "cache_creation");

        r->usus.cache_scriptum_5m = json_capere_integer(cc,
            "ephemeral_5m_input_tokens", 0);
        r->usus.cache_scriptum_1h = json_capere_integer(cc,
            "ephemeral_1h_input_tokens", 0);
    }
    alioquin
    {
        r->usus.cache_scriptum_5m = json_capere_integer(usus,
            "cache_creation_input_tokens", 0);
    }
    r->successus  = VERUM;
    r->error      = VATES_OK;
    redde r;
}

VatesResponsum*
vates_anthropic_legere (
       i32  status_http,
    chorda  corpus,
   Piscina* piscina)
{
    chorda novitas;

    redde _legere(status_http, corpus, piscina, &novitas);
}


/* ======================================================================
 * C. PROVISORES, MITTERE, RATIONARIUM, FICTUS
 * ====================================================================== */

#define VATES_URL_ANTHROPIC     "https://api.anthropic.com/v1/messages"
#define VATES_VERSIO_ANTHROPIC  "2023-06-01"

nomen structura {
       i32 status;
    chorda corpus;
} VatesFictumResponsum;

structura Vates {
           Piscina* piscina;
            chorda  provisor;
         character* clavis;               /* NUL-terminata, UNA copia */
     VatesOptiones  optiones;
       HttpVectura  vectura;
         Herbarium* herbarium;
               b32  rationarium_defectus;
               Xar* fictus_responsa;      /* VatesFictumResponsum */
               i32  fictus_index;
               Xar* fictus_petitiones;    /* chorda */
               i32  fictus_numerus;
};

nomen Vates* (*VatesConstructor)(Piscina* piscina, chorda clavis,
                                 constans VatesOptiones* optiones);

nomen structura {
     constans character* titulus;
       VatesConstructor  constructor;
} VatesProvisorInscriptus;

hic_manens constans character* constans _campi_anthropic[] = { "model",
    NIHIL };

interior chorda
_chorda_copia (
     chorda  s,
    Piscina* piscina)
{
    chorda c;

    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(piscina, (i64)s.mensura + I);
    si (s.mensura > 0)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    redde c;
}

/* larvare: copia herbarii (caput probatum non exportat) - Ruling T5 */
interior b32
_cifra (
    i8 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_vocis (
    i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || _cifra(c) || c == '_' || c == '-';
}

interior chorda
_larvare (
     chorda  s,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)s.mensura + I);
    i32 i = 0;

    dum (i < s.mensura)
    {
        si (_vocis(s.datum[i]))
        {
            i32 initium  = i;
            b32 cifra    = FALSUM;

            dum (i < s.mensura && _vocis(s.datum[i]))
            {
                si (_cifra(s.datum[i]))
                {
                    cifra = VERUM;
                }
                i++;
            }
            si (cifra && i - initium >= XII)
            {
                chorda_aedificator_appendere_character(aed, '@');
            }
            alioquin
            {
                i32 k;
                b32 in_numeris = FALSUM;

                per (k = initium; k < i; k++)
                {
                    si (_cifra(s.datum[k]))
                    {
                        si (!in_numeris)
                        {
                            chorda_aedificator_appendere_character(aed,
                                '#');
                        }
                        in_numeris = VERUM;
                    }
                    alioquin
                    {
                        chorda_aedificator_appendere_character(aed,
                            (character)s.datum[k]);
                        in_numeris = FALSUM;
                    }
                }
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed,
                (character)s.datum[i]);
            i++;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_clavis_anthropic (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
         JsonResultus  j = json_legere(responsum->corpus, piscina);
            JsonValor* e =
                j.successus ? json_objectum_capere(j.radix,
                "error") : NIHIL;
    ChordaAedificator* aed;
            character  numerus[XXXII];
               chorda  vacua = chorda_ex_literis("",
                   piscina);

    si (!e)
    {
        redde herbarium_clavis_sceleti(petitio, responsum, piscina,
            datum);
    }
    aed = chorda_aedificator_creare(piscina, CXXVIII);
    sprintf(numerus, "%u:", responsum->status);
    chorda_aedificator_appendere_literis(aed, numerus);
    chorda_aedificator_appendere_chorda(aed, json_capere_chorda(e,
        "type", vacua));
    chorda_aedificator_appendere_character(aed, ':');
    chorda_aedificator_appendere_chorda(aed,
        _larvare(json_capere_chorda(e, "message", vacua), piscina));
    redde chorda_aedificator_finire(aed);
}

VatesOptiones
vates_optiones_ordinariae (vacuum)
{
    VatesOptiones o;

    memset(&o, 0, magnitudo(o));
    redde o;
}

interior Vates*
_anthropic_struere (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones,
        constans character* provisor)
{
    Vates* v;

    si (!piscina)
    {
        redde NIHIL;
    }
    v = (Vates*)piscina_allocare(piscina, (i64)magnitudo(Vates));
    memset(v, 0, magnitudo(*v));
    v->piscina   = piscina;
    v->provisor  = chorda_ex_literis(provisor, piscina);
    v->clavis = (character*)piscina_allocare(piscina,
        (i64)clavis.mensura + I);
    si (clavis.mensura > 0)
    {
        memcpy(v->clavis, clavis.datum, (size_t)clavis.mensura);
    }
    v->clavis[clavis.mensura] = '\0';
    v->optiones = optiones ? *optiones : vates_optiones_ordinariae();
    si (v->optiones.tempus_ms <= 0)
    {
        v->optiones.tempus_ms = DC * M;           /* X minuta */
    }
    si (v->optiones.conatus_maximi == 0)
    {
        v->optiones.conatus_maximi = III;
    }
    si (v->optiones.mora_iterandi_ms <= 0)
    {
        v->optiones.mora_iterandi_ms = II * M;
    }
    si (v->optiones.mora_iterandi_maxima_ms <= 0)
    {
        v->optiones.mora_iterandi_maxima_ms = LX * M;
    }
    v->vectura = v->optiones.vectura.exsequi ? v->optiones.vectura
                                             : http_vectura_ordinaria();
    redde v;
}

/* novitas corporis Anthropic: stop_reason ignota, deinde notae et vitia
 * schematis declarati (lib/vates_responsum.norma) */
interior chorda
_novitates_corporis (
    JsonValor* radix,
      Piscina* piscina)
{
    chorda novitas = _vacua();

    (vacuum)_causa_finis(json_capere_chorda(radix, "stop_reason",
        _vacua()), &novitas, piscina);
    _novitates_ex_norma(radix, &novitas, piscina);
    redde novitas;
}

/* IUDEX (herbarium-spec-2 §III): ante usum, ex statu et octetis solis,
 * codice hostili introitu tuto (json_legere, norma_iudicare). Responsa
 * >= CD herbarium ipsum 'status' premit; hic sub CD. */
interior chorda
_iudex_anthropic (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    JsonResultus j;
       character nuntius[LXIV];

    (vacuum)petitio;
    (vacuum)datum;
    si (responsum->status != CC)
    {
        sprintf(nuntius, "status inexspectatus: %u",
            (insignatus integer)responsum->status);
        redde chorda_ex_literis(nuntius, piscina);
    }
    j = json_legere(responsum->corpus, piscina);
    si (!j.successus)
    {
        redde chorda_ex_literis("corpus non JSON", piscina);
    }
    si (!json_est_objectum(j.radix))
    {
        redde chorda_ex_literis("radix non objectum", piscina);
    }
    redde _novitates_corporis(j.radix, piscina);
}

/* hospes ex URL ('https://hospes/...'): acervus per hospitem */
interior constans character*
_hospes_ex_url (
    constans character* url,
               Piscina* piscina)
{
    constans character* initium = strstr(url, "://");
    constans character* finis;
             character* hospes;
                size_t  longitudo;

    initium    = initium ? initium + III : url;
    finis      = strchr(initium, '/');
    longitudo  = finis ? (size_t)(finis - initium) : strlen(initium);
    hospes     = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    memcpy(hospes, initium, longitudo);
    hospes[longitudo] = '\0';
    redde hospes;
}

/* captura ORDINARIA (herbarium-spec-2 §II.b): herbarium_via, aliter
 * sedes ordinaria + hospes; sine_herbario expresse exstinguit. Sedes
 * irrita: sine captura, semel in stderr - vocatio numquam frangitur */
interior vacuum
_herbarium_adiungere (
                 Vates* v,
    constans character* hospes)
{
    HerbariumOptiones optiones_herbarii;
               chorda sedes;
               chorda causa;

    si (v->optiones.sine_herbario)
    {
        redde;
    }
    optiones_herbarii = herbarium_optiones_ordinariae();
    si (v->optiones.herbarium_via)
    {
        optiones_herbarii.directorium = v->optiones.herbarium_via;
    }
    alioquin
    {
        sedes = herbarium_sedes_ordinaria(hospes, v->piscina, &causa);
        si (sedes.mensura == 0)
        {
            fprintf(stderr, "vates: herbarium sine sede (%.*s) - sine"
                " captura\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde;
        }
        optiones_herbarii.directorium = chorda_ut_cstr(sedes,
            v->piscina);
    }
    optiones_herbarii.clavis = _clavis_anthropic;
    optiones_herbarii.campi_petitionis = _campi_anthropic;
    optiones_herbarii.iudex = _iudex_anthropic;
    v->herbarium = herbarium_aperire(v->piscina, &optiones_herbarii);
    v->vectura = herbarium_vectura(v->herbarium, v->vectura);
}

Vates*
vates_anthropic_aperire (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones)
{
    Vates* v = _anthropic_struere(piscina, clavis, optiones,
        "anthropic");

    si (v)
    {
        _herbarium_adiungere(v, _hospes_ex_url(VATES_URL_ANTHROPIC,
            v->piscina));
    }
    redde v;
}

interior HttpResultus
_fictus_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
                   Vates* v = (Vates*)datum;
            HttpResultus  res;
           HttpResponsum* resp;
    VatesFictumResponsum* f;

    *(chorda*)xar_addere(v->fictus_petitiones) =
        _chorda_copia(http_petitio_visus(petitio).corpus, v->piscina);
    memset(&res, 0, magnitudo(res));
    si (v->fictus_index >= xar_numerus(v->fictus_responsa))
    {
        res.error = HTTP_ERROR_CONNEXIO;
        res.error_descriptio = chorda_ex_literis("fictus exhaustum",
            piscina);
        redde res;
    }
    f = (VatesFictumResponsum*)xar_obtinere(v->fictus_responsa,
        v->fictus_index);
    v->fictus_index++;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status   = f->status;
    resp->corpus   = f->corpus;
    res.successus  = VERUM;
    res.responsum  = resp;
    redde res;
}

Vates*
vates_fictus_aperire (
                   Piscina* piscina,
    constans VatesOptiones* optiones)
{
    Vates* v = _anthropic_struere(piscina, chorda_ex_literis("fictus",
        piscina),
                                  optiones, "fictus");

    si (!v)
    {
        redde NIHIL;
    }
    v->fictus_responsa = xar_creare(piscina,
        (i32)magnitudo(VatesFictumResponsum));
    v->fictus_petitiones  = xar_creare(piscina, (i32)magnitudo(chorda));
    v->vectura.exsequi    = _fictus_exsequi;
    v->vectura.datum      = v;
    /* acervus proprius: responsa ficta numquam inter vera */
    _herbarium_adiungere(v, "fictus");
    redde v;
}

interior Vates*
_fictus_constructor (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* o)
{
    (vacuum)clavis;
    redde vates_fictus_aperire(piscina, o);
}

hic_manens constans VatesProvisorInscriptus _provisores[] = {
    { "anthropic", vates_anthropic_aperire },
    { "fictus",    _fictus_constructor },
    { NIHIL,       NIHIL }
};

Vates*
vates_aperire (
                   Piscina* piscina,
                    chorda  provisor,
                    chorda  clavis,
    constans VatesOptiones* optiones)
{
    i32 i;

    per (i = 0; _provisores[i].titulus; i++)
    {
        si (chorda_aequalis_literis(provisor, _provisores[i].titulus))
        {
            redde _provisores[i].constructor(piscina, clavis, optiones);
        }
    }
    fprintf(stderr, "vates: provisor ignotus '%.*s' - noti:",
            (integer)provisor.mensura,
            (constans character*)provisor.datum);
    per (i = 0; _provisores[i].titulus; i++)
    {
        fprintf(stderr, " %s", _provisores[i].titulus);
    }
    fprintf(stderr, "\n");
    redde NIHIL;
}

chorda
vates_provisor (
    Vates* vates)
{
    redde vates ? vates->provisor : _vacua();
}

constans character*
vates_error_descriptio (
    VatesError error)
{
    commutatio (error)
    {
        casus VATES_OK:            redde "OK";
        casus VATES_ERROR_RETE:    redde "rete (connexio aut TLS)";
        casus VATES_ERROR_TEMPUS:  redde "tempus excessum";
        casus VATES_ERROR_STATUS:  redde "status HTTP provisoris";
        casus VATES_ERROR_PARSE:   redde "responsum non legibile";
        casus VATES_ERROR_LIMES:   redde "limes localis (ante missionem)";
        ordinarius:                redde "error ignotus";
    }
}

/* ---- fictus: responsa in cauda ---- */

interior vacuum
_fictus_ponere (
     Vates* v,
       i32  status,
    chorda  corpus)
{
    VatesFictumResponsum* f;

    si (!v || !v->fictus_responsa)
    {
        redde;
    }
    f          = (VatesFictumResponsum*)xar_addere(v->fictus_responsa);
    f->status  = status;
    f->corpus  = corpus;
}

interior JsonValor*
_fictum_nuntium (
                     Vates* v,
        constans character* causa_finis,
                 JsonValor* content,
                 VatesUsus  usus)
{
      Piscina* p   = v->piscina;
    JsonValor* r   = json_objectum_creare(p);
    JsonValor* u   = json_objectum_creare(p);
    JsonValor* cc  = json_objectum_creare(p);
    character  id[LXIV];

    v->fictus_numerus++;
    sprintf(id, "msg_fictus_%u", v->fictus_numerus);
    json_objectum_ponere(r, "id", json_chorda_creare_literis(p, id));
    json_objectum_ponere(r, "type", json_chorda_creare_literis(p,
        "message"));
    json_objectum_ponere(r, "role", json_chorda_creare_literis(p,
        "assistant"));
    json_objectum_ponere(r, "model", json_chorda_creare_literis(p,
        "fictus"));
    json_objectum_ponere(r, "content", content);
    json_objectum_ponere(r, "stop_reason", json_chorda_creare_literis(p,
        causa_finis));
    json_objectum_ponere(r, "stop_sequence", json_nullum_creare(p));
    json_objectum_ponere(u, "input_tokens", json_integer_creare(p,
        usus.input));
    json_objectum_ponere(u, "cache_read_input_tokens",
                         json_integer_creare(p, usus.cache_lectum));
    json_objectum_ponere(u, "cache_creation_input_tokens",
        json_integer_creare(p,
        usus.cache_scriptum_5m + usus.cache_scriptum_1h));
    json_objectum_ponere(cc, "ephemeral_5m_input_tokens",
                         json_integer_creare(p,
                         usus.cache_scriptum_5m));
    json_objectum_ponere(cc, "ephemeral_1h_input_tokens",
                         json_integer_creare(p,
                         usus.cache_scriptum_1h));
    json_objectum_ponere(u, "cache_creation", cc);
    json_objectum_ponere(u, "output_tokens", json_integer_creare(p,
        usus.output));
    json_objectum_ponere(r, "usage", u);
    redde r;
}

vacuum
vates_fictus_textum (
        Vates* vates,
       chorda  textus,
    VatesUsus  usus)
{
    JsonValor* content;
    JsonValor* b;

    si (!vates || !vates->fictus_responsa)
    {
        redde;
    }
    content  = json_tabulatum_creare(vates->piscina);
    b        = json_objectum_creare(vates->piscina);
    json_objectum_ponere(b, "type",
        json_chorda_creare_literis(vates->piscina, "text"));
    json_objectum_ponere(b, "text", json_chorda_creare(vates->piscina,
        textus));
    json_tabulatum_addere(content, b);
    _fictus_ponere(vates, CC, json_scribere(
        _fictum_nuntium(vates, "end_turn", content, usus),
        vates->piscina));
}

vacuum
vates_fictus_instrumentum (
                   Vates* vates,
                  chorda  id,
                  chorda  titulus,
      constans character* input_json)
{
       JsonValor* content;
       JsonValor* b;
    JsonResultus  input;
       VatesUsus  nullus;

    si (!vates || !vates->fictus_responsa)
    {
        redde;
    }
    memset(&nullus, 0, magnitudo(nullus));
    input = json_legere_literis(input_json ? input_json : "{}",
        vates->piscina);
    content  = json_tabulatum_creare(vates->piscina);
    b        = json_objectum_creare(vates->piscina);
    json_objectum_ponere(b, "type",
        json_chorda_creare_literis(vates->piscina, "tool_use"));
    json_objectum_ponere(b, "id", json_chorda_creare(vates->piscina,
        id));
    json_objectum_ponere(b, "name", json_chorda_creare(vates->piscina,
        titulus));
    json_objectum_ponere(b, "input", input.successus ? input.radix
                                     : json_objectum_creare(vates->piscina));
    json_tabulatum_addere(content, b);
    _fictus_ponere(vates, CC, json_scribere(
        _fictum_nuntium(vates, "tool_use", content, nullus),
        vates->piscina));
}

vacuum
vates_fictus_crudum (
                 Vates* vates,
                   i32  status_http,
    constans character* corpus_json)
{
    si (!vates || !corpus_json)
    {
        redde;
    }
    _fictus_ponere(vates, status_http, chorda_ex_literis(corpus_json,
        vates->piscina));
}

i32
vates_fictus_petitiones_numerus (
    Vates* vates)
{
    redde (vates
        && vates->fictus_petitiones) ? xar_numerus(vates->fictus_petitiones) : 0;
}

chorda
vates_fictus_petitio (
    Vates* vates,
      i32  index)
{
    si (   !vates || !vates->fictus_petitiones
        || index >= xar_numerus(vates->fictus_petitiones))
    {
        redde _vacua();
    }
    redde *(chorda*)xar_obtinere(vates->fictus_petitiones, index);
}

/* ---- mittere ---- */

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_dormire (
    s64 ms)
{
    structura timespec pausa;

    si (ms <= 0)
    {
        redde;
    }
    pausa.tv_sec   = (time_t)(ms / M);
    pausa.tv_nsec  = (longus)((ms % M) * M * M);
    (vacuum)nanosleep(&pausa, NIHIL);
}

interior b32
_status_iterandus (
    i32 status)
{
    redde status == CDVIII || status == CDIX || status == CDXXIX
        || status >= D;
}

interior s64
_mora_iterandi (
    constans Vates* v,
      HttpResultus  resultus_http)
{
    s64 mora = (s64)v->optiones.mora_iterandi_ms;

    si (resultus_http.successus && resultus_http.responsum)
    {
        chorda ra = http_responsum_caput(resultus_http.responsum,
            "retry-after");
           s64 secunda;

        si (   ra.mensura > 0 && chorda_ut_s64(ra, &secunda)
            && secunda >= 0)
        {
            mora = secunda * M;
        }
    }
    si (mora > (s64)v->optiones.mora_iterandi_maxima_ms)
    {
        mora = (s64)v->optiones.mora_iterandi_maxima_ms;
    }
    redde mora;
}

interior s64
_pretium (
        constans Vates* v,
                chorda  exemplar,
    constans VatesUsus* u)
{
    i32 i;

    per (i = 0; v->optiones.pretia
        && i < v->optiones.pretia_numerus; i++)
    {
        constans VatesPretium* p = &v->optiones.pretia[i];

        si (chorda_aequalis_literis(exemplar, p->exemplar))
        {
            redde (u->input * p->input + u->output * p->output
                   + u->cache_lectum * p->cache_lectum
                   + u->cache_scriptum_5m * p->cache_5m
                   + u->cache_scriptum_1h * p->cache_1h) / ((s64)M * M);
        }
    }
    redde -I;
}

interior vacuum
_usus_addere (
             VatesUsus* summa,
    constans VatesUsus* u,
                   b32* pretium_notum)
{
    summa->input              += u->input;
    summa->cache_lectum       += u->cache_lectum;
    summa->cache_scriptum_5m  += u->cache_scriptum_5m;
    summa->cache_scriptum_1h  += u->cache_scriptum_1h;
    summa->output             += u->output;
    summa->mora_ms            += u->mora_ms;
    si (u->pretium >= 0)
    {
        summa->pretium += u->pretium;
        *pretium_notum = VERUM;
    }
}

interior HttpPetitio*
_petitionem_http (
           constans Vates* v,
    constans VatesPetitio* pe,
                   chorda  corpus,
                  Piscina* piscina)
{
    HttpPetitio* hp = http_petitio_creare(piscina, HTTP_POST,
        VATES_URL_ANTHROPIC);
            i32 i;

    http_petitio_caput_addere(hp, "content-type", "application/json");
    http_petitio_caput_addere(hp, "x-api-key", v->clavis);
    http_petitio_caput_addere(hp, "anthropic-version",
        VATES_VERSIO_ANTHROPIC);
    per (i = 0; i < xar_numerus(pe->capita); i++)
    {
        VatesEffugium* e = (VatesEffugium*)xar_obtinere(pe->capita, i);

        si (chorda_aequalis_literis(e->provisor, "anthropic"))
        {
            http_petitio_caput_addere(hp, chorda_ut_cstr(e->clavis,
                piscina),
                                      chorda_ut_cstr(e->textus,
                                      piscina));
        }
    }
    http_petitio_corpus_ponere_chorda(hp, corpus);
    http_petitio_tempus_ponere(hp, v->optiones.tempus_ms);
    redde hp;
}

interior vacuum
_rationarium_scribere (
                      Vates* v,
      constans VatesPetitio* pe,
                     chorda  propositum,
                        i32  conatus,
    constans VatesResponsum* r,
                    Piscina* piscina)
{
    JsonValor* o;
    JsonValor* u;
       chorda  textus;
       chorda  linea;

    si (!v->optiones.rationarium_via)
    {
        redde;
    }
    o = json_objectum_creare(piscina);
    u = json_objectum_creare(piscina);
    json_objectum_ponere(o, "tempus",
        json_chorda_creare(piscina, fasti_ad_iso(fasti_nunc(),
        piscina)));
    json_objectum_ponere(o, "provisor", json_chorda_creare(piscina,
        v->provisor));
    json_objectum_ponere(o, "exemplar", json_chorda_creare(piscina,
        pe->exemplar));
    json_objectum_ponere(o, "propositum", json_chorda_creare(piscina,
        propositum));
    json_objectum_ponere(o, "conatus", json_integer_creare(piscina,
        (s64)conatus));
    json_objectum_ponere(o, "status", json_integer_creare(piscina,
        (s64)r->status_http));
    json_objectum_ponere(o, "causa_finis", json_chorda_creare(piscina,
        r->causa_finis_cruda));
    si (r->error != VATES_OK)
    {
        json_objectum_ponere(o, "error",
            json_chorda_creare_literis(piscina,
            vates_error_descriptio(r->error)));
        json_objectum_ponere(o, "error_genus",
            json_chorda_creare(piscina, r->error_genus));
    }
    json_objectum_ponere(u, "input", json_integer_creare(piscina,
        r->usus.input));
    json_objectum_ponere(u, "cache_lectum", json_integer_creare(piscina,
        r->usus.cache_lectum));
    json_objectum_ponere(u, "cache_5m", json_integer_creare(piscina,
        r->usus.cache_scriptum_5m));
    json_objectum_ponere(u, "cache_1h", json_integer_creare(piscina,
        r->usus.cache_scriptum_1h));
    json_objectum_ponere(u, "output", json_integer_creare(piscina,
        r->usus.output));
    json_objectum_ponere(o, "usus", u);
    json_objectum_ponere(o, "pretium", json_integer_creare(piscina,
        r->usus.pretium));
    json_objectum_ponere(o, "mora_ms", json_integer_creare(piscina,
        r->usus.mora_ms));
    json_objectum_ponere(o, "id", json_chorda_creare(piscina, r->id));
    textus         = json_scribere(o, piscina);
    linea.mensura  = textus.mensura + I;
    linea.datum    = (i8*)piscina_allocare(piscina, (i64)linea.mensura);
    memcpy(linea.datum, textus.datum, (size_t)textus.mensura);
    linea.datum[textus.mensura] = '\n';
    si (   !filum_appendere_firmiter(v->optiones.rationarium_via, linea)
        && !v->rationarium_defectus)
    {
        fprintf(stderr, "vates: rationarium scribi non potest: %s\n",
                v->optiones.rationarium_via);
        v->rationarium_defectus = VERUM;
    }
}

interior s32
_signa_numerare (
    constans VatesPetitio* p)
{
    s32 numerus = p->cauda_signata ? I : 0;
    i32 i;
    i32 k;

    per (i = 0; i < xar_numerus(p->systema); i++)
    {
        si (((VatesBlocus*)xar_obtinere(p->systema,
            i))->signum_thesauri)
        {
            numerus++;
        }
    }
    per (i = 0; i < xar_numerus(p->nuntii); i++)
    {
        VatesNuntius* n = (VatesNuntius*)xar_obtinere(p->nuntii, i);

        per (k = 0; k < xar_numerus(n->bloci); k++)
        {
            VatesBlocus* b = (VatesBlocus*)xar_obtinere(n->bloci, k);

            si (b->signum_thesauri && b->genus != VATES_OPACUM)
            {
                numerus++;
            }
        }
    }
    redde numerus;
}

VatesResponsum*
vates_mittere (
            Vates* vates,
     VatesPetitio* petitio,
           chorda  propositum,
          Piscina* piscina)
{
    VatesResponsum* r = NIHIL;
         VatesUsus  summa;
            chorda  corpus;
               i32  conatus;
               b32  pretium_notum = FALSUM;

    si (!piscina)
    {
        redde NIHIL;
    }
    si (!vates || !petitio)
    {
        r         = _responsum_vacuum(piscina);
        r->error  = VATES_ERROR_LIMES;
        r->error_nuntius = chorda_ex_literis("vates aut petitio NIHIL",
            piscina);
        redde r;
    }
    si (_signa_numerare(petitio) > VATES_PUNCTA_THESAURI_MAXIMA)
    {
        r         = _responsum_vacuum(piscina);
        r->error  = VATES_ERROR_LIMES;
        r->error_nuntius = chorda_ex_literis(
            "plus quam IV puncta cache (API: IV maxime)", piscina);
        redde r;
    }
    memset(&summa, 0, magnitudo(summa));
    corpus = vates_anthropic_corpus(petitio, piscina);
    per (conatus = I; ; conatus++)
    {
        HttpPetitio* hp = _petitionem_http(vates, petitio, corpus,
            piscina);
                 s64 initium = _ms_nunc();
        HttpResultus resultus_http =
            http_vectura_exsequi(vates->vectura, hp, piscina);
        b32 iterandum;

        si (!resultus_http.successus || !resultus_http.responsum)
        {
            r = _responsum_vacuum(piscina);
            r->error = (resultus_http.error
                == HTTP_ERROR_TIMEOUT) ? VATES_ERROR_TEMPUS
                                                        : VATES_ERROR_RETE;
            r->error_nuntius = resultus_http.error_descriptio;
            iterandum = (r->error == VATES_ERROR_RETE)
                || (conatus == I);
        }
        alioquin
        {
            chorda novitas;

            /* inexspectata iam in disco: iudex in vectura capiente
             * (herbarium-spec-2) ante hanc lectionem premit */
            r = _legere(resultus_http.responsum->status,
                resultus_http.responsum->corpus, piscina, &novitas);
            iterandum = _status_iterandus(r->status_http);
        }
        r->usus.mora_ms = _ms_nunc() - initium;
        r->usus.pretium = _pretium(vates,
            r->exemplar.mensura > 0 ? r->exemplar : petitio->exemplar,
            &r->usus);
        _rationarium_scribere(vates, petitio, propositum, conatus, r,
            piscina);
        _usus_addere(&summa, &r->usus, &pretium_notum);
        si (!iterandum || conatus >= vates->optiones.conatus_maximi)
        {
            frange;
        }
        _dormire(_mora_iterandi(vates, resultus_http));
    }
    r->usus = summa;
    si (!pretium_notum)
    {
        r->usus.pretium = -I;
    }
    r->conatus = conatus;
    redde r;
}
