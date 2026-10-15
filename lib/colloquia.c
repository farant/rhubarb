/* colloquia.c - sessiones Claude Code (include/colloquia.h) */
#include "postulata_posix.h"
#include "colloquia.h"
#include "praeconium.h"
#include "json.h"
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLLOQUIA_CAUDA_TRANSCRIPTI (CCLVI * MXXIV)

/* status internus sessionis: Colloquium et tempora linearum */
nomen structura {
    Colloquium c;
           s64 tempus_prompti;     /* UserPromptSubmit ultimus */
           s64 tempus_explicitae;  /* "praeco status" ultimus */
        chorda linea_derivata;
        chorda linea_explicita;
} Status;

/* panellus -> sessio ultima (per eventus cum sessione) */
nomen structura {
    chorda panellus;
    chorda sessio;
} Panellus;

interior b32
eventus_est (
                 chorda  eventus,
     constans character* titulus)
{
    redde chorda_aequalis_literis(eventus, titulus);
}

interior Status*
statum_invenire (
       Xar* status,
    chorda  sessio)
{
    Status* s;
       i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(status); i++)
    {
        s = (Status*)xar_obtinere(status, i);
        si (chorda_aequalis(s->c.sessio, sessio))
        {
            redde s;
        }
    }
    s = (Status*)xar_addere(status);
    si (s)
    {
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->c.sessio = sessio;
        s->c.status = COLLOQUIUM_IGNOTUM;
    }
    redde s;
}

interior vacuum
panellum_notare (
       Xar* panelli,
    chorda  panellus,
    chorda  sessio)
{
    Panellus* p;
         i32  i;

    si (panellus.mensura == ZEPHYRUM)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(panelli); i++)
    {
        p = (Panellus*)xar_obtinere(panelli, i);
        si (chorda_aequalis(p->panellus, panellus))
        {
            p->sessio = sessio;
            redde;
        }
    }
    p = (Panellus*)xar_addere(panelli);
    si (p)
    {
        p->panellus  = panellus;
        p->sessio    = sessio;
    }
}

interior chorda
sessio_panelli (
       Xar* panelli,
    chorda  panellus)
{
    Panellus* p;
      chorda  vacua;
         i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(panelli); i++)
    {
        p = (Panellus*)xar_obtinere(panelli, i);
        si (chorda_aequalis(p->panellus, panellus))
        {
            redde p->sessio;
        }
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* prima linea textus */
interior chorda
linea_prima (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura && c.datum[i] != '\n'; i++)
    {
    }
    c.mensura = i;
    redde c;
}

/* "genus: nuntius" (instrumentum) */
interior chorda
linea_instrumenti (
      chorda  genus,
      chorda  nuntius,
     Piscina* piscina)
{
    si (genus.mensura == ZEPHYRUM)
    {
        redde nuntius;
    }
    si (nuntius.mensura == ZEPHYRUM)
    {
        redde genus;
    }
    redde chorda_concatenare(chorda_concatenare(genus,
        chorda_ex_literis(": ", piscina), piscina), nuntius, piscina);
}

interior b32
eget_te (
    chorda genus)
{
    redde chorda_aequalis_literis(genus, "permission_prompt")
        || chorda_aequalis_literis(genus, "agent_needs_input")
        || chorda_incipit(genus, chorda_ex_literis("elicitation",
        NIHIL));
}

/* eventum unum in statum sessionis suae */
interior vacuum
eventum_applicare (
    constans PraeconiumEventus* e,
                        Status* s,
                       Piscina* piscina)
{
    s->c.tempus = e->tempus;
    si (e->via.mensura > ZEPHYRUM)
    {
        s->c.via = e->via;
    }
    si (e->transcriptum.mensura > ZEPHYRUM)
    {
        s->c.transcriptum = e->transcriptum;
    }
    si (e->panellus.mensura > ZEPHYRUM)
    {
        s->c.panellus = e->panellus;
        s->c.servitor = e->servitor;
    }
    si (eventus_est(e->eventus, "SessionStart"))
    {
        s->c.status = COLLOQUIUM_OTIOSUM;
    }
    alioquin si (eventus_est(e->eventus, "UserPromptSubmit"))
    {
        s->c.status        = COLLOQUIUM_LABORAT;
        s->tempus_prompti  = e->tempus;
        s->linea_derivata  = e->nuntius;
    }
    alioquin si (eventus_est(e->eventus, "PreToolUse"))
    {
        s->c.status        = COLLOQUIUM_LABORAT;
        s->linea_derivata  = linea_instrumenti(e->genus, e->nuntius,
            piscina);
    }
    alioquin si (eventus_est(e->eventus, "Notification"))
    {
        si (eget_te(e->genus))
        {
            s->c.status        = COLLOQUIUM_EXSPECTAT;
            s->linea_derivata  = e->nuntius;
        }
        alioquin si (chorda_aequalis_literis(e->genus, "idle_prompt"))
        {
            s->c.status = COLLOQUIUM_OTIOSUM;
        }
    }
    alioquin si (eventus_est(e->eventus, "Stop"))
    {
        s->c.status        = COLLOQUIUM_PERFECIT;
        s->linea_derivata  = linea_prima(e->nuntius);
    }
    alioquin si (eventus_est(e->eventus, "StopFailure"))
    {
        s->c.status        = COLLOQUIUM_EXSPECTAT;
        s->linea_derivata  = e->nuntius;
    }
    alioquin si (eventus_est(e->eventus, "SessionEnd"))
    {
        s->c.status = COLLOQUIUM_CLAUSUM;
    }
}

interior integer
ordo_recentium (
    constans vacuum* a,
    constans vacuum* b)
{
    s64 x;
    s64 y;

    x = ((constans Colloquium*)a)->tempus;
    y = ((constans Colloquium*)b)->tempus;
    redde x > y ? -I : x < y ? I : ZEPHYRUM;
}

Xar*
colloquia_ex_eventibus (
         Xar* eventus,
     Piscina* piscina)
{
                           Xar* status;
                           Xar* panelli;
                           Xar* exitus;
                    Colloquium* plani;
    constans PraeconiumEventus* e;
                        Status* s;
                        chorda  sessio;
                           i32  i;
                           i32  n;

    status   = xar_creare(piscina, (i32)magnitudo(Status));
    panelli  = xar_creare(piscina, (i32)magnitudo(Panellus));
    exitus   = xar_creare(piscina, (i32)magnitudo(Colloquium));
    si (!status || !panelli || !exitus)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; eventus && i < xar_numerus(eventus); i++)
    {
        e = (constans PraeconiumEventus*)xar_obtinere(eventus, i);
        si (   eventus_est(e->eventus, "status") && e->sessio.mensura
            == ZEPHYRUM)
        {
            /* status explicitus: sessio ultima eius panelli */
            sessio = sessio_panelli(panelli, e->panellus);
            si (sessio.mensura == ZEPHYRUM)
            {
                perge;
            }
            s = statum_invenire(status, sessio);
            si (s)
            {
                s->c.tempus           = e->tempus;
                s->tempus_explicitae  = e->tempus;
                s->linea_explicita    = e->nuntius;
            }
            perge;
        }
        si (e->sessio.mensura == ZEPHYRUM)
        {
            perge;
        }
        panellum_notare(panelli, e->panellus, e->sessio);
        s = statum_invenire(status, e->sessio);
        si (s)
        {
            eventum_applicare(e, s, piscina);
        }
    }
    n = xar_numerus(status);
    si (n == ZEPHYRUM)
    {
        redde exitus;
    }
    plani = (Colloquium*)piscina_allocare(piscina, magnitudo(Colloquium)
        * (memoriae_index)n);
    si (!plani)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        s         = (Status*)xar_obtinere(status, i);
        s->c.linea = (s->linea_explicita.mensura > ZEPHYRUM
            && s->tempus_explicitae > s->tempus_prompti)
            ? s->linea_explicita : s->linea_derivata;
        plani[i]  = s->c;
    }
    qsort(plani, (size_t)n, magnitudo(Colloquium), ordo_recentium);
    per (i = ZEPHYRUM; i < n; i++)
    {
        *(Colloquium*)xar_addere(exitus) = plani[i];
    }
    redde exitus;
}

/* valor chordae clavis in linea JSON (vacuus si non) */
interior chorda
clavis_lineae (
                 chorda  linea,
     constans character* clavis,
                Piscina* piscina)
{
    JsonResultus  r;
       JsonValor* v;
          chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    r              = json_legere(linea, piscina);
    si (!r.successus || !r.radix || !json_est_objectum(r.radix))
    {
        redde vacua;
    }
    v = json_objectum_capere(r.radix, clavis);
    redde v && json_est_chorda(v) ? json_ad_chorda(v) : vacua;
}

chorda
colloquia_titulum_legere (
    constans character* transcriptum,
              Piscina*  piscina)
{
                      FILE* f;
                      long  mensura;
                      long  initium;
                 character* alveus;
                    size_t  lecti;
                    chorda  textus;
                    chorda  ai;
                    chorda  t;
    chorda_fissio_fructus  lineae;
                       s32  i;

    ai.datum    = NIHIL;
    ai.mensura  = ZEPHYRUM;
    f = transcriptum && transcriptum[ZEPHYRUM]
        ? lectiones_fopen(transcriptum, "rb") : NIHIL;
    si (!f)
    {
        redde ai;
    }
    (vacuum)fseek(f, 0L, SEEK_END);
    mensura  = ftell(f);
    initium  = mensura > (long)COLLOQUIA_CAUDA_TRANSCRIPTI
        ? mensura - (long)COLLOQUIA_CAUDA_TRANSCRIPTI : 0L;
    alveus   = mensura >= 0L ? (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura - initium) + I) : NIHIL;
    si (!alveus || fseek(f, initium, SEEK_SET) != 0)
    {
        fclose(f);
        redde ai;
    }
    lecti = fread(alveus, I, (size_t)(mensura - initium), f);
    fclose(f);
    textus.datum    = (i8*)alveus;
    textus.mensura  = (i32)lecti;
    lineae          = chorda_fissio(textus, '\n', piscina);
    /* a fine: custom-title primus inventus vincit; ai-title ultimus
     * servatur ut subsidium */
    per (i = (s32)lineae.numerus - I; i >= ZEPHYRUM; i--)
    {
        si (chorda_continet(lineae.elementa[i], chorda_ex_literis(
                "\"type\":\"custom-title\"", piscina)))
        {
            t = clavis_lineae(lineae.elementa[i], "customTitle",
                piscina);
            si (t.mensura > ZEPHYRUM)
            {
                redde t;
            }
        }
        si (   ai.mensura == ZEPHYRUM
            && chorda_continet(lineae.elementa[i], chorda_ex_literis(
                "\"type\":\"ai-title\"", piscina)))
        {
            ai = clavis_lineae(lineae.elementa[i], "aiTitle", piscina);
        }
    }
    redde ai;
}

vacuum
colloquia_panellos_applicare (
    Xar* colloquia,
    Xar* panelli)
{
    Colloquium* c;
           i32  i;

    per (i = ZEPHYRUM; colloquia && i < xar_numerus(colloquia); i++)
    {
        c = (Colloquium*)xar_obtinere(colloquia, i);
        c->panellus_vivit = c->panellus.mensura > ZEPHYRUM && panelli
            && multiplexor_panellum_invenire(panelli, c->panellus);
        si (!c->panellus_vivit)
        {
            c->status = COLLOQUIUM_CLAUSUM;
        }
    }
}

/* nomen directorii ultimum ("/w/gamma" -> "gamma") */
interior chorda
nomen_directorii (
    chorda via)
{
    i32 i;

    per (i = via.mensura; i > ZEPHYRUM && via.datum[i - I] != '/'; i--)
    {
    }
    via.datum    += i;
    via.mensura  -= i;
    redde via;
}

Xar*
colloquia_legere (
      constans character* via_libri,
    constans Multiplexor* m,
                     i32  octeti,
                 Piscina* piscina)
{
            Xar* l;
            Xar* panelli;
     Colloquium* c;
            i32  i;

    l = colloquia_ex_eventibus(praeconium_caudam_legere(via_libri,
        octeti, piscina), piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c = (Colloquium*)xar_obtinere(l, i);
        c->titulus = colloquia_titulum_legere(chorda_ut_cstr(
            c->transcriptum, piscina), piscina);
        si (c->titulus.mensura == ZEPHYRUM)
        {
            c->titulus = nomen_directorii(c->via);
        }
    }
    si (l && m)
    {
        panelli = multiplexor_panelli(m, piscina);
        si (panelli)
        {
            colloquia_panellos_applicare(l, panelli);
        }
    }
    redde l;
}
