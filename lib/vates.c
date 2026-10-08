/* vates.c - provisor neuter exemplarium linguae (vates-spec;
 * vates-plan-2). Partes: A exemplar neutrum + corpus Anthropic (T3),
 * B lectio (T4), C provisores + mittere + rationarium + fictus (T5). */
#include "postulata_posix.h"
#include "vates.h"
#include "herbarium.h"
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
