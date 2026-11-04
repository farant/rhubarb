/* praeconium.c - acta colloquiorum (include/praeconium.h) */
#include "postulata_posix.h"
#include "praeconium.h"
#include "filum.h"
#include "chorda_aedificator.h"
#include "json.h"
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constans character*
praeconium_via (
    Piscina* piscina)
{
    constans character* directorium;
    constans character* domus;
             character* via;

    directorium  = getenv("RHUBARB_COLLOQUIA");
    domus        = getenv("HOME");
    via          = (character*)piscina_allocare(piscina,
        (memoriae_index)
        (directorium ? strlen(directorium) : (domus ? strlen(domus)
        : ZEPHYRUM)) + LXIV);
    si (!via)
    {
        redde NIHIL;
    }
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/eventa.jsonl", directorium);
    }
    alioquin
    {
        sprintf(via, "%s/.rhubarb/colloquia/eventa.jsonl", domus ? domus
            : ".");
    }
    redde via;
}

/* campus chordae objecti (vacuus si abest aut non chorda) */
interior chorda
campus (
             JsonValor* objectum,
    constans character* clavis)
{
    JsonValor* v;
       chorda  vacua;

    v = objectum ? json_objectum_capere(objectum, clavis) : NIHIL;
    si (v && json_est_chorda(v))
    {
        redde json_ad_chorda(v);
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* primus campus non vacuus inter claves datas (NIHIL finitas) */
interior chorda
campus_primus (
                  JsonValor* objectum,
    constans character* constans* claves)
{
    chorda c;
       i32 i;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; claves[i]; i++)
    {
        c = campus(objectum, claves[i]);
        si (c.mensura > ZEPHYRUM)
        {
            redde c;
        }
    }
    redde c;
}

/* praecidere ad maximum, numquam intra characterem UTF-8: si octetus
 * primus exclusus continuatio est, character ad initium suum redit */
interior chorda
praecidere (
    chorda c)
{
    i32 n;

    si (c.mensura <= PRAECONIUM_NUNTIUS_MAXIMUS)
    {
        redde c;
    }
    n = PRAECONIUM_NUNTIUS_MAXIMUS;
    dum (   n > ZEPHYRUM && (((insignatus character)c.datum[n]) & CXCII)
        == CXXVIII)
    {
        n--;
    }
    c.mensura = n;
    redde c;
}

interior chorda
literae (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s ? s : "", piscina);
}

interior b32
eventus_est (
                 chorda  eventus,
     constans character* titulus)
{
    redde chorda_aequalis_literis(eventus, titulus);
}

b32
praeconium_ex_hamo (
                 chorda  json,
     constans character* panellus,
     constans character* tmux,
                    s64  tempus,
      PraeconiumEventus* e,
                Piscina* piscina)
{
    constans character* claves_instrumenti[] = { "description",
        "command", "file_path", "pattern", "url", NIHIL };
    constans character* claves_initii[] = { "trigger", "source",
        NIHIL };
           JsonResultus  r;
              JsonValor* radix;
                 chorda  tmux_c;
                    i32  i;

    si (!e)
    {
        redde FALSUM;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    r = json_legere(json, piscina);
    si (!r.successus || !r.radix || !json_est_objectum(r.radix))
    {
        redde FALSUM;
    }
    radix            = r.radix;
    e->tempus        = tempus;
    e->sessio        = campus(radix, "session_id");
    e->eventus       = campus(radix, "hook_event_name");
    e->via           = campus(radix, "cwd");
    e->transcriptum  = campus(radix, "transcript_path");
    e->panellus      = literae(panellus, piscina);
    /* TMUX = "socket,pid,sessio": socket solum */
    tmux_c           = literae(tmux, piscina);
    per (i = ZEPHYRUM; i < tmux_c.mensura
        && tmux_c.datum[i] != ','; i++)
    {
    }
    e->servitor          = tmux_c;
    e->servitor.mensura  = i;

    si (eventus_est(e->eventus, "Notification"))
    {
        e->genus    = campus(radix, "notification_type");
        e->nuntius  = campus(radix, "message");
    }
    alioquin si (   eventus_est(e->eventus, "PreToolUse")
                 || eventus_est(e->eventus, "PostToolUse")
                 || eventus_est(e->eventus, "PostToolUseFailure")
                 || eventus_est(e->eventus, "PermissionRequest"))
    {
        e->genus    = campus(radix, "tool_name");
        e->nuntius  = campus_primus(json_objectum_capere(radix,
            "tool_input"), claves_instrumenti);
    }
    alioquin si (eventus_est(e->eventus, "UserPromptSubmit"))
    {
        e->nuntius = campus(radix, "prompt");
    }
    alioquin si (   eventus_est(e->eventus, "Stop")
                 || eventus_est(e->eventus, "SubagentStop"))
    {
        e->nuntius = campus(radix, "last_assistant_message");
    }
    alioquin si (eventus_est(e->eventus, "StopFailure"))
    {
        e->nuntius = campus(radix, "error");
    }
    alioquin si (eventus_est(e->eventus, "SessionStart"))
    {
        e->genus = campus_primus(radix, claves_initii);
    }
    alioquin si (eventus_est(e->eventus, "SessionEnd"))
    {
        e->genus = campus(radix, "reason");
    }
    e->nuntius = praecidere(e->nuntius);
    redde VERUM;
}

b32
praeconium_scribere (
            constans character* via,
    constans PraeconiumEventus* e,
                       Piscina* piscina)
{
    JsonValor* o;
       chorda  linea;
    character* directorium;
    character* finis;

    si (!via || !e)
    {
        redde FALSUM;
    }
    o = json_objectum_creare(piscina);
    si (!o)
    {
        redde FALSUM;
    }
    json_objectum_ponere(o, "tempus", json_integer_creare(piscina,
        e->tempus));
    json_objectum_ponere(o, "sessio", json_chorda_creare(piscina,
        e->sessio));
    json_objectum_ponere(o, "eventus", json_chorda_creare(piscina,
        e->eventus));
    json_objectum_ponere(o, "panellus", json_chorda_creare(piscina,
        e->panellus));
    json_objectum_ponere(o, "servitor", json_chorda_creare(piscina,
        e->servitor));
    json_objectum_ponere(o, "via", json_chorda_creare(piscina, e->via));
    json_objectum_ponere(o, "transcriptum", json_chorda_creare(piscina,
        e->transcriptum));
    json_objectum_ponere(o, "genus", json_chorda_creare(piscina,
        e->genus));
    json_objectum_ponere(o, "nuntius", json_chorda_creare(piscina,
        praecidere(e->nuntius)));
    linea = chorda_concatenare(json_scribere(o, piscina),
        chorda_ex_literis("\n", piscina), piscina);
    /* directorium libri */
    directorium = (character*)piscina_allocare(piscina, (memoriae_index)
        strlen(via) + I);
    si (!directorium)
    {
        redde FALSUM;
    }
    strcpy(directorium, via);
    finis = strrchr(directorium, '/');
    si (finis && finis != directorium)
    {
        *finis = '\0';
        (vacuum)filum_directorium_creare_cum_parentibus(directorium);
    }
    redde filum_appendere_firmiter(via, linea);
}

/* eventus ex linea nostra (FALSUM si mala) */
interior b32
ex_linea (
                chorda  linea,
     PraeconiumEventus* e,
               Piscina* piscina)
{
    JsonResultus  r;
       JsonValor* t;

    r = json_legere(linea, piscina);
    si (!r.successus || !r.radix || !json_est_objectum(r.radix))
    {
        redde FALSUM;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    t = json_objectum_capere(r.radix, "tempus");
    si (!t || !json_est_integer(t))
    {
        redde FALSUM;
    }
    e->tempus        = json_ad_integer(t);
    e->sessio        = campus(r.radix, "sessio");
    e->eventus       = campus(r.radix, "eventus");
    e->panellus      = campus(r.radix, "panellus");
    e->servitor      = campus(r.radix, "servitor");
    e->via           = campus(r.radix, "via");
    e->transcriptum  = campus(r.radix, "transcriptum");
    e->genus         = campus(r.radix, "genus");
    e->nuntius       = campus(r.radix, "nuntius");
    redde VERUM;
}

Xar*
praeconium_caudam_legere (
    constans character* via,
                   i32  octeti,
               Piscina* piscina)
{
                      Xar* l;
                     FILE* f;
                      long  mensura;
                      long  initium;
                 character* alveus;
                    size_t  lecti;
                    chorda  textus;
    chorda_fissio_fructus  lineae;
         PraeconiumEventus  e;
                       i32  i;

    l = xar_creare(piscina, (i32)magnitudo(PraeconiumEventus));
    f = via ? lectiones_fopen(via, "rb") : NIHIL;
    si (!l || !f)
    {
        si (f)
        {
            fclose(f);
        }
        redde l;
    }
    (vacuum)fseek(f, 0L, SEEK_END);
    mensura = ftell(f);
    initium = mensura > (long)octeti ? mensura - (long)octeti : 0L;
    alveus   = (character*)piscina_allocare(piscina, (memoriae_index)
        (mensura - initium) + I);
    si (!alveus || mensura < 0L || fseek(f, initium, SEEK_SET) != 0)
    {
        fclose(f);
        redde l;
    }
    lecti = fread(alveus, I, (size_t)(mensura - initium), f);
    fclose(f);
    textus.datum    = (i8*)alveus;
    textus.mensura  = (i32)lecti;
    lineae          = chorda_fissio(textus, '\n', piscina);
    /* linea prima fracta (initium intra lineam) ut JSON deficit et
     * omittitur; initium IN limite lineae: linea integra servatur */
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (   lineae.elementa[i].mensura > ZEPHYRUM
            && ex_linea(lineae.elementa[i], &e, piscina))
        {
            *(PraeconiumEventus*)xar_addere(l) = e;
        }
    }
    redde l;
}

Xar*
praeconium_legere_ab (
    constans character* via,
                   s64* positio,
               Piscina* piscina)
{
                      Xar* l;
                     FILE* f;
                      long  mensura;
                 character* alveus;
                    size_t  lecti;
                       s64  finis;
                    chorda  textus;
    chorda_fissio_fructus  lineae;
         PraeconiumEventus  e;
                       i32  i;

    l = xar_creare(piscina, (i32)magnitudo(PraeconiumEventus));
    si (!l || !positio)
    {
        redde l;
    }
    f = via ? lectiones_fopen(via, "rb") : NIHIL;
    si (!f)
    {
        *positio = ZEPHYRUM;
        redde l;
    }
    (vacuum)fseek(f, 0L, SEEK_END);
    mensura = ftell(f);
    /* liber brevior quam positio (deletus, rotatus): ab initio */
    si (mensura < 0L || *positio < ZEPHYRUM || *positio > (s64)mensura)
    {
        *positio = ZEPHYRUM;
    }
    si ((s64)mensura == *positio)
    {
        fclose(f);
        redde l;
    }
    alveus = (character*)piscina_allocare(piscina, (memoriae_index)
        ((s64)mensura - *positio) + I);
    si (!alveus || fseek(f, (long)*positio, SEEK_SET) != 0)
    {
        fclose(f);
        redde l;
    }
    lecti = fread(alveus, I, (size_t)((s64)mensura - *positio), f);
    fclose(f);
    /* solum usque ad lineam integram ultimam */
    finis = (s64)lecti;
    dum (finis > ZEPHYRUM && alveus[finis - I] != '\n')
    {
        finis--;
    }
    si (finis == ZEPHYRUM)
    {
        redde l;
    }
    textus.datum    = (i8*)alveus;
    textus.mensura  = (i32)finis;
    lineae          = chorda_fissio(textus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (   lineae.elementa[i].mensura > ZEPHYRUM
            && ex_linea(lineae.elementa[i], &e, piscina))
        {
            *(PraeconiumEventus*)xar_addere(l) = e;
        }
    }
    *positio += finis;
    redde l;
}

/* ===== comites P8: hami tmux ===== */

hic_manens constans character* constans hami_tmux[] = {
    "session-window-changed", "client-session-changed",
    "window-pane-changed", NIHIL };

hic_manens constans character signum_initii[] =
    "# praeco tmux: initium\n";
hic_manens constans character signum_finis[] =
    "# praeco tmux: finis\n";

chorda
praeconium_tmux_configuratio (
    constans character* praeco,
               Piscina* piscina)
{
    ChordaAedificator* a;
                  i32  i;

    a = chorda_aedificator_creare(piscina, DXII);
    si (   !a || !praeco || strchr(praeco, '\'')
        || strchr(praeco, '"'))
    {
        redde chorda_ex_literis("", piscina);
    }
    chorda_aedificator_appendere_literis(a, "# praeco tmux (comites P8)"
        " - generatum a 'praeco tmux instituere'\n");
    per (i = ZEPHYRUM; hami_tmux[i]; i++)
    {
        chorda_aedificator_appendere_literis(a, "set-hook -g ");
        chorda_aedificator_appendere_literis(a, hami_tmux[i]);
        chorda_aedificator_appendere_literis(a, " \"run-shell -b '");
        chorda_aedificator_appendere_literis(a, praeco);
        chorda_aedificator_appendere_literis(a, " tmux "
            "\\\"#{socket_path}\\\" \\\"#{pane_id}\\\"'\"\n");
    }
    redde chorda_aedificator_finire(a);
}

/* initium lineae 'signum' (cum '\n') in textu ab 'ab'; -1 si nulla */
interior s32
lineam_invenire (
                 chorda  textus,
                    i32  ab,
     constans character* signum,
                Piscina* piscina)
{
    chorda acus;
       s32 k;

    acus = chorda_ex_literis(signum, piscina);
    dum (ab <= textus.mensura)
    {
        k = chorda_invenire_index(chorda_sectio(textus, ab,
            textus.mensura), acus);
        si (k < ZEPHYRUM)
        {
            redde -I;
        }
        si (   ab + (i32)k                   == ZEPHYRUM
            || textus.datum[ab + (i32)k - I] == '\n')
        {
            redde (s32)(ab + (i32)k);
        }
        ab += (i32)k + I;
    }
    redde -I;
}

chorda
praeconium_tmux_conf (
                 chorda  conf,
     constans character* fasciculus,
                Piscina* piscina)
{
    ChordaAedificator* a;
                  s32  initium;
                  s32  finis;

    a = chorda_aedificator_creare(piscina, (memoriae_index)
        conf.mensura + CCLVI);
    si (!a || !fasciculus)
    {
        redde conf;
    }
    initium  = lineam_invenire(conf, ZEPHYRUM, signum_initii, piscina);
    finis    = initium < ZEPHYRUM ? -I : lineam_invenire(conf,
        (i32)initium, signum_finis, piscina);
    si (finis >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, chorda_sectio(conf,
            ZEPHYRUM, (i32)initium));
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(a, conf);
        si (   conf.mensura > ZEPHYRUM
            && conf.datum[conf.mensura - I] != '\n')
        {
            chorda_aedificator_appendere_literis(a, "\n");
        }
    }
    chorda_aedificator_appendere_literis(a, signum_initii);
    chorda_aedificator_appendere_literis(a, "source-file -q \"");
    chorda_aedificator_appendere_literis(a, fasciculus);
    chorda_aedificator_appendere_literis(a, "\"\n");
    chorda_aedificator_appendere_literis(a, signum_finis);
    si (finis >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, chorda_sectio(conf,
            (i32)finis + (i32)strlen(signum_finis), conf.mensura));
    }
    redde chorda_aedificator_finire(a);
}
