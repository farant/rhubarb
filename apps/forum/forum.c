/* apps/forum/forum.c - forum: fenestra vitreae in mundum scriptorum
 * (F2 - app prima in apps/, conventiones nascuntur).
 *
 * Exemplar tabellae (ansa app-possessa, capsula, internuntius) +
 * cliens daemonis tabulariumd: connexio TCP PER-PETITIONEM
 * (contractus v1 spec-v2 par I), initialize non necessarium (daemon
 * se praeinitiat), start-if-absent ut fori. Tractatores IS:
 * pipata_legere (daemon "legere" genus=pipatum -> tabulatum
 * structuratum), pipatum_mittere {corpus} (titulus = praefixum XL
 * codicillorum; actor fran, sine signatura - decisio colloquii).
 *
 * Vexillum -fumus: pipata leguntur ANTE fenestram, numerus
 * impressus, exitus - porta scriptabilis sine oculis. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "tcp.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "capsula_forum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FORUM_PORTUS_ORDINARIUS 8753
/* legere C rerum cum corporibus - laxe (numerus romanus deest) */
#define RESPONSUM_CAPACITAS 262144

nomen structura {
    i32 portus;
    s64 petitio_index;   /* id JSON-RPC crescens */
} ForumStatus;

/* litterae -> chorda (sine copia; unio contra cast-qual) */
interior chorda
_ch_forum (constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l = litterae;
    c.datum = u.m;
    c.mensura = (i32)strlen(litterae);
    redde c;
}

/* ==================================================
 * cliens daemonis (per-petitionem, ut fori)
 * ================================================== */

interior TcpConnexio*
_conectere_aut_gignere (i32 portus, Piscina* pn)
{
    hic_manens b32 genitus = FALSUM;
    TcpResultus rc = tcp_connectere("127.0.0.1", portus, pn);
    integer k;

    si (rc.successus)
    {
        redde rc.connexio;
    }
    si (genitus)
    {
        /* iam genitum semel: defectus CITUS - anquisitio IS (2 s)
         * tractatorem obstruentem 5 s congelaret */
        redde NIHIL;
    }
    {
        character imperium[CXXVIII];

        genitus = VERUM;
        fprintf(stderr, "[forum] tabulariumd absens - gigno per"
            " launcher (portus %d)\n", (int)portus);
        sprintf(imperium, "./gesta/tabulariumd.sh -portus %d"
            " > /dev/null 2>&1 &", (int)portus);
        (vacuum)system(imperium);
    }
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        (vacuum)system("sleep 0.2");
        rc = tcp_connectere("127.0.0.1", portus, pn);
        si (rc.successus)
        {
            redde rc.connexio;
        }
    }
    redde NIHIL;
}

/* lineam mittere, responsum ad novam lineam legere (obstruens);
 * chorda vacua = defectus (culpa posita) */
interior chorda
_daemon_petere (i32 portus, Piscina* pn, chorda linea,
    chorda* culpa)
{
    chorda vacua;
    character* buf;
    s32 pos = ZEPHYRUM;
    TcpConnexio* conn;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    conn = _conectere_aut_gignere(portus, pn);
    si (conn == NIHIL)
    {
        *culpa = _ch_forum("tabulariumd absens -"
            " ./gesta/tabulariumd.sh incipe");
        redde vacua;
    }
    buf = (character*)piscina_allocare(pn,
        (memoriae_index)RESPONSUM_CAPACITAS);
    si (buf == NIHIL
        || !tcp_mittere_omnia(conn, (constans i8*)linea.datum,
               linea.mensura)
        || !tcp_mittere_omnia(conn, (constans i8*)"\n", I))
    {
        tcp_claudere(conn);
        *culpa = _ch_forum("missio ad daemonem fracta");
        redde vacua;
    }
    per (;;)
    {
        s32 n = tcp_recipere(conn, (i8*)(buf + pos),
            (i32)(RESPONSUM_CAPACITAS - I - pos));
        s32 j;
        b32 completum = FALSUM;

        si (n <= ZEPHYRUM)
        {
            frange;
        }
        per (j = pos; j < pos + n; j++)
        {
            si (buf[j] == '\n')
            {
                pos = j;
                completum = VERUM;
                frange;
            }
        }
        si (completum)
        {
            frange;
        }
        pos += n;
        si (pos >= (s32)(RESPONSUM_CAPACITAS - I))
        {
            frange;
        }
    }
    tcp_claudere(conn);
    si (pos == ZEPHYRUM)
    {
        *culpa = _ch_forum("responsum daemonis vacuum");
        redde vacua;
    }
    redde chorda_ex_buffer((i8*)buf, (i32)pos);
}

/* textum instrumenti ex involucro MCP extrahere:
 * {"result":{"content":[{"type":"text","text":...}]}} aut
 * {"error":{"message":...}} -> *est_error */
interior chorda
_textus_ex_responso (chorda responsum, Piscina* pn, b32* est_error)
{
    JsonResultus r = json_legere(responsum, pn);
    JsonValor* v;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    *est_error = FALSUM;
    si (!r.successus || !json_est_objectum(r.radix))
    {
        *est_error = VERUM;
        redde vacua;
    }
    v = json_objectum_capere(r.radix, "error");
    si (v != NIHIL)
    {
        *est_error = VERUM;
        redde json_ad_chorda(json_objectum_capere(v, "message"));
    }
    v = json_objectum_capere(r.radix, "result");
    si (v != NIHIL)
    {
        JsonValor* contentum = json_objectum_capere(v, "content");
        JsonValor* isError = json_objectum_capere(v, "isError");

        si (isError != NIHIL && json_ad_boolean(isError))
        {
            *est_error = VERUM;
        }
        si (contentum != NIHIL && json_est_tabulatum(contentum)
            && json_tabulatum_numerus(contentum) > ZEPHYRUM)
        {
            redde json_ad_chorda(json_objectum_capere(
                json_tabulatum_obtinere(contentum, ZEPHYRUM),
                "text"));
        }
    }
    *est_error = VERUM;
    redde vacua;
}

/* praefixum XL codicillorum ut titulus (limes UTF-8 servatus) */
interior chorda
_titulus_ex_corpore (chorda corpus)
{
    i32 codicilli = ZEPHYRUM;
    i32 i = ZEPHYRUM;
    chorda t;

    dum (i < corpus.mensura && codicilli < XL)
    {
        i++;
        dum (i < corpus.mensura
            && ((insignatus character)corpus.datum[i] & 0xC0u)
                == 0x80u)
        {
            i++;
        }
        codicilli++;
    }
    t.datum = corpus.datum;
    t.mensura = i;
    redde t;
}

/* pipata a daemone: tabulatum JSON parsatum (NIHIL = culpa) */
interior JsonValor*
_pipata_capere (ForumStatus* forum, Piscina* pn, chorda* culpa)
{
    chorda linea = chorda_ex_literis(
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"legere\",\"arguments\":{"
        "\"genus\":\"pipatum\",\"quantum\":100}}}", pn);
    chorda responsum = _daemon_petere(forum->portus, pn, linea,
        culpa);
    chorda textus;
    b32 est_error = FALSUM;
    JsonResultus r;

    si (responsum.mensura == ZEPHYRUM)
    {
        redde NIHIL;   /* culpa iam posita */
    }
    textus = _textus_ex_responso(responsum, pn, &est_error);
    si (est_error || textus.mensura == ZEPHYRUM)
    {
        *culpa = textus.mensura > ZEPHYRUM ? textus
            : _ch_forum("responsum daemonis ininterpretabile");
        redde NIHIL;
    }
    r = json_legere(textus, pn);
    si (!r.successus || !json_est_tabulatum(r.radix))
    {
        *culpa = _ch_forum("tabulatum legere fracta");
        redde NIHIL;
    }
    redde r.radix;
}

/* ==================================================
 * tractatores internuntii
 * ================================================== */

interior JsonValor*
_pipata_legere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    JsonValor* fructus;
    JsonValor* pipata;

    (vacuum)argumenta;
    pipata = _pipata_capere(forum, piscina, culpa);
    si (pipata == NIHIL)
    {
        redde NIHIL;   /* culpa posita */
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "pipata", pipata);
    redde fructus;
}

interior JsonValor*
_pipatum_mittere (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    ForumStatus* forum = (ForumStatus*)datum;
    chorda corpus;
    JsonValor* petitio;
    JsonValor* params;
    JsonValor* arg_obj;
    chorda responsum;
    chorda textus;
    b32 est_error = FALSUM;
    JsonValor* fructus;

    corpus.mensura = ZEPHYRUM;
    corpus.datum = NIHIL;
    si (argumenta != NIHIL)
    {
        corpus = json_ad_chorda(json_objectum_capere(argumenta,
            "corpus"));
    }
    si (corpus.mensura == ZEPHYRUM)
    {
        *culpa = _ch_forum("corpus vacuum");
        redde NIHIL;
    }
    /* petitio per json aedificata - effugia gratis */
    petitio = json_objectum_creare(piscina);
    params = json_objectum_creare(piscina);
    arg_obj = json_objectum_creare(piscina);
    forum->petitio_index++;
    json_objectum_ponere(petitio, "jsonrpc",
        json_chorda_creare_literis(piscina, "2.0"));
    json_objectum_ponere(petitio, "id",
        json_integer_creare(piscina, forum->petitio_index));
    json_objectum_ponere(petitio, "method",
        json_chorda_creare_literis(piscina, "tools/call"));
    json_objectum_ponere(arg_obj, "genus",
        json_chorda_creare_literis(piscina, "pipatum"));
    json_objectum_ponere(arg_obj, "titulus",
        json_chorda_creare(piscina, _titulus_ex_corpore(corpus)));
    json_objectum_ponere(arg_obj, "corpus",
        json_chorda_creare(piscina, corpus));
    json_objectum_ponere(arg_obj, "actor",
        json_chorda_creare_literis(piscina, "fran"));
    json_objectum_ponere(params, "name",
        json_chorda_creare_literis(piscina, "addere"));
    json_objectum_ponere(params, "arguments", arg_obj);
    json_objectum_ponere(petitio, "params", params);

    responsum = _daemon_petere(forum->portus, piscina,
        json_scribere(petitio, piscina), culpa);
    si (responsum.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    textus = _textus_ex_responso(responsum, piscina, &est_error);
    si (est_error)
    {
        *culpa = textus.mensura > ZEPHYRUM ? textus
            : _ch_forum("missio recusata");
        redde NIHIL;
    }
    imprimere("[forum] pipatum missum: %.*s\n",
        (int)(corpus.mensura > XL ? XL : corpus.mensura),
        (constans character*)corpus.datum);
    fflush(stdout);
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "bene",
        json_boolean_creare(piscina, VERUM));
    redde fructus;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("forum",
        16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "forum_vocationes", 8388608);
    ForumStatus forum;
    b32 fumus = FALSUM;
    integer k;
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio figura_vitreae;
    Fenestra* fenestra;
    Capsula* capsula;
    Vitrea* vitrea;
    Internuntius* inx;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    forum.portus = FORUM_PORTUS_ORDINARIUS;
    forum.petitio_index = (s64)I;
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-portus") == ZEPHYRUM && k + I < argc)
        {
            forum.portus = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
        {
            fumus = VERUM;
        }
    }
    si (fumus)
    {
        /* porta sine oculis: lectio contra daemonem verum ante
         * fenestram ullam */
        chorda culpa;
        JsonValor* pipata;

        culpa.mensura = ZEPHYRUM;
        culpa.datum = NIHIL;
        pipata = _pipata_capere(&forum, piscina, &culpa);
        si (pipata == NIHIL)
        {
            imprimere("[forum] fumus FRACTUS: %.*s\n",
                (int)culpa.mensura,
                (constans character*)culpa.datum);
            redde I;
        }
        imprimere("[forum] fumus: %d pipata lecta\n",
            (int)json_tabulatum_numerus(pipata));
        piscina_destruere(piscina_vocationis);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    figura_fenestrae.titulus = "forum";
    figura_fenestrae.x = CC;
    figura_fenestrae.y = CC;
    figura_fenestrae.latitudo = 1000;
    figura_fenestrae.altitudo = 900;
    figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_forum, piscina);
    si (capsula == NIHIL)
    {
        imprimere("FRACTA: capsula\n");
        redde I;
    }
    figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url = NIHIL;
    figura_vitreae.inspectabilis = VERUM;
    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        imprimere("FRACTA: vitrea\n");
        redde I;
    }
    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        imprimere("FRACTA: internuntius\n");
        redde I;
    }
    (vacuum)internuntius_praebere(inx, "pipata_legere",
        _pipata_legere, &forum);
    (vacuum)internuntius_praebere(inx, "pipatum_mittere",
        _pipatum_mittere, &forum);

    imprimere("[forum] fenestra aperta (daemon portus %d)\n",
        (int)forum.portus);
    fflush(stdout);

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus eventus;
        chorda nuntium;
        VitreaNuntiusGenus genus;
        PiscinaNotatio nota;

        fenestra_expectare_eventus(fenestra, CC);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* eventa fenestralia sola (livratio duplex) */
        }
        nota = piscina_notare(piscina_vocationis);
        dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
        {
            si (genus == VITREA_NUNTIUS_PONS)
            {
                internuntius_tractare(inx, nuntium,
                    piscina_vocationis);
            }
            alioquin
            {
                vitrea_recargare(vitrea);
            }
        }
        piscina_reficere(piscina_vocationis, nota);
    }

    imprimere("[forum] finis\n");
    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
