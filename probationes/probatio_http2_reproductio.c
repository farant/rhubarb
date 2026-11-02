/* probatio_http2_reproductio.c - tabulae HTTP/2 iteratae SINE Go
 * (http2-plan T9; spec D9). Tabulae ab tools/http2_tabulare.sh in
 * probationes/fixa/http2 (aut HTTP2_FIXA): pro quoque exemplo
 *   (a) formae servi decodantur et byte pro byte iterum scribuntur;
 *   (b) series formarum (genus, vexilla, rivus, longitudo) initium
 *       est seriei quam Go ipse se scripsisse tabulat (go.log);
 *   (c) octeti servi in connexionem novam fusi (petitionibus eisdem
 *       factis) responsa eadem reddunt ac cursus vivus;
 *   (d) octeti nostri cum praefatione et positionibus C4 incipiunt. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "lectiones.h"
#include "hpack.h"
#include "http2_forma.h"
#include "http2.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define FIXA_ORDINARIA   "probationes/fixa/http2"
#define FORMA_MAXIMA     0x1000000
#define MULTAE           XX
#define MAGNUM           (CCLVI * MXXIV)
#define REDDENDA         (C * MXXIV)
#define FRUSTUM          CMXCVII
#define PARS_ORACULI          (XVI * MXXIV)

hic_manens Piscina* piscina;

hic_manens constans character* constans EXEMPLA[V] = {
    "salve", "eco", "multae", "magnum", "abire"
};

nomen structura {
    i32 genus;
    i32 vexilla;
    i32 rivus;
    i32 longitudo;
} FormaBrevis;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior chorda
legere (
    constans character* exemplum,
    constans character* suffixum)
{
    constans character* fixa = lectiones_ambitus("HTTP2_FIXA");
             character  via[MXXIV];

    sprintf(via, "%s/%s.%s", (fixa && fixa[ZEPHYRUM]) ? fixa
        : FIXA_ORDINARIA, exemplum, suffixum);
    redde filum_legere_totum(via, piscina);
}

/* formae octetorum servi (sine praefatione: servus nullam mittit) */
interior Xar*
formae_octetorum (
    chorda octeti)
{
    Xar* l;
    i32  pos;

    l    = xar_creare(piscina, (i32)magnitudo(FormaBrevis));
    pos  = ZEPHYRUM;
    dum (pos < octeti.mensura)
    {
           Http2Forma  f;
        Http2Iudicium  j;
          FormaBrevis* b;
                  i32  k;

        k = http2_formam_legere(octeti.datum + pos, octeti.mensura
            - pos,
            FORMA_MAXIMA, &f, &j);
        si (k == ZEPHYRUM || !j.bona)
        {
            redde NIHIL;
        }
        b             = (FormaBrevis*)xar_addere(l);
        b->genus      = f.genus;
        b->vexilla    = f.vexilla;
        b->rivus      = f.rivus;
        b->longitudo  = f.longitudo;
        pos           += k;
    }
    redde l;
}

/* numerus post 'clavis' in linea (0 si abest) */
interior i32
numerus_post (
    constans character* linea,
    constans character* clavis)
{
    constans character* p = strstr(linea, clavis);
                   i32  n;

    n = ZEPHYRUM;
    si (!p)
    {
        redde ZEPHYRUM;
    }
    p += strlen(clavis);
    dum (*p >= '0' && *p <= '9')
    {
        n = n * X + (i32)(*p++ - '0');
    }
    redde n;
}

interior i32
genus_oraculi (
    constans character* titulus)
{
    hic_manens constans character* constans NOMINA[X] = {
        "DATA", "HEADERS", "PRIORITY", "RST_STREAM", "SETTINGS",
        "PUSH_PROMISE", "PING", "GOAWAY", "WINDOW_UPDATE",
            "CONTINUATION"
    };
    i32 k;

    per (k = ZEPHYRUM; k < X; k++)
    {
        size_t m = strlen(NOMINA[k]);

        si (   strncmp(titulus, NOMINA[k], m) == ZEPHYRUM
            && (titulus[m] == ' ' || titulus[m] == '\0'))
        {
            redde k;
        }
    }
    redde CCLV;
}

interior i32
vexilla_oraculi (
    constans character* linea)
{
    constans character* p = strstr(linea, "flags=");
                   i32  v;

    v = ZEPHYRUM;
    si (!p)
    {
        redde ZEPHYRUM;
    }
    p += VI;
    dum (*p && *p != ' ')
    {
        si (   strncmp(p, "END_STREAM", X) == ZEPHYRUM
            || strncmp(p, "ACK", III)      == ZEPHYRUM)
        {
            v |= HTTP2_FINIS_RIVI;
        }
        si (strncmp(p, "END_HEADERS", XI) == ZEPHYRUM)
        {
            v |= HTTP2_FINIS_CAPITUM;
        }
        si (strncmp(p, "PADDED", VI) == ZEPHYRUM)
        {
            v |= HTTP2_IMPLETUM;
        }
        si (strncmp(p, "PRIORITY", VIII) == ZEPHYRUM)
        {
            v |= HTTP2_CUM_PRIORITATE;
        }
        dum (*p && *p != '|' && *p != ' ')
        {
            p++;
        }
        si (*p == '|')
        {
            p++;
        }
    }
    redde v;
}

/* lineae ": wrote <GENUS> ..." tabulae Go */
interior Xar*
formae_oraculi (
    chorda tabula)
{
    Xar* l;
    i32  initium;
    i32  i;

    l        = xar_creare(piscina, (i32)magnitudo(FormaBrevis));
    initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= tabula.mensura; i++)
    {
                 character  linea[CCLVI];
        constans character* p;
                       i32  m;

        si (i < tabula.mensura && tabula.datum[i] != '\n')
        {
            perge;
        }
        m = i - initium;
        si (m >= CCLVI)
        {
            m = CCLV;
        }
        memcpy(linea, tabula.datum + initium, (size_t)m);
        linea[m]  = '\0';
        initium   = i + I;
        p         = strstr(linea, ": wrote ");
        si (p)
        {
            FormaBrevis* b = (FormaBrevis*)xar_addere(l);

            p             += VIII;
            b->genus      = genus_oraculi(p);
            b->vexilla    = vexilla_oraculi(p);
            b->rivus      = numerus_post(p, "stream=");
            b->longitudo  = numerus_post(p, "len=");
        }
    }
    redde l;
}

interior vacuum
_sectio_rotundum (
    CredoContextus* c)
{
    i32 e;

    (vacuum)c;
    per (e = ZEPHYRUM; e < V; e++)
    {
        chorda octeti = legere(EXEMPLA[e], "servus.bin");
           i32 pos;
           i32 formae;
           b32 bene;

        CREDO_GRADUS(EXEMPLA[e]);
        CREDO_NECESSE(octeti.mensura > ZEPHYRUM);
        pos     = ZEPHYRUM;
        formae  = ZEPHYRUM;
        bene    = VERUM;
        dum (bene && pos < octeti.mensura)
        {
               Http2Forma  f;
            Http2Iudicium  j;
                       i8* iterum;
                      i32  k;
                      i32  m;

            k = http2_formam_legere(octeti.datum + pos,
                octeti.mensura - pos, FORMA_MAXIMA, &f, &j);
            bene = k > ZEPHYRUM && j.bona;
            si (!bene)
            {
                frange;
            }
            iterum = (i8*)piscina_allocare(piscina, (memoriae_index)k);
            m = http2_formam_scribere(f.genus, f.vexilla, f.rivus,
                octeti.datum + pos + HTTP2_CAPUT_FORMAE, f.longitudo,
                iterum, k);
            bene = m == k && memcmp(iterum, octeti.datum + pos,
                (size_t)k) == ZEPHYRUM;
            pos += k;
            formae++;
        }
        CREDO_VERUM(bene);
        CREDO_AEQUALIS_I32(pos, octeti.mensura);
        CREDO_VERUM(formae >= III);
    }
}

interior vacuum
_sectio_oraculi (
    CredoContextus* c)
{
    i32 e;

    (vacuum)c;
    per (e = ZEPHYRUM; e < V; e++)
    {
        Xar* nostrae;
        Xar* oraculum;
        i32  i;
        b32  bene;

        CREDO_GRADUS(EXEMPLA[e]);
        nostrae   = formae_octetorum(legere(EXEMPLA[e], "servus.bin"));
        oraculum  = formae_oraculi(legere(EXEMPLA[e], "go.log"));
        CREDO_NECESSE(nostrae && oraculum);
        CREDO_VERUM(xar_numerus(nostrae) > ZEPHYRUM);
        CREDO_VERUM(xar_numerus(oraculum) >= xar_numerus(nostrae));
        bene = VERUM;
        per (i = ZEPHYRUM; bene && i < xar_numerus(nostrae)
             && i < xar_numerus(oraculum); i++)
        {
            FormaBrevis* a = (FormaBrevis*)xar_obtinere(nostrae, i);
            FormaBrevis* b = (FormaBrevis*)xar_obtinere(oraculum, i);

            bene = a->genus == b->genus && a->vexilla == b->vexilla
                && a->rivus == b->rivus && a->longitudo == b->longitudo;
            si (!bene)
            {
                imprimere("  forma %u: nostra %u/%u/%u/%u, Go %u/%u/%u/%u\n",
                    (insignatus integer)i, (insignatus integer)a->genus,
                    (insignatus integer)a->vexilla,
                    (insignatus integer)a->rivus,
                    (insignatus integer)a->longitudo,
                    (insignatus integer)b->genus,
                    (insignatus integer)b->vexilla,
                    (insignatus integer)b->rivus,
                    (insignatus integer)b->longitudo);
            }
        }
        CREDO_VERUM(bene);
    }
}

/* responsum unius rivi in iteratione */
nomen structura {
                  i32  rivus;
                  i32  status;
    ChordaAedificator* corpus;
                  b32  finis;
} Reditum;

interior HpackCaput
caput (
    constans character* titulus,
    constans character* valor)
{
    HpackCaput h;

    h.titulus  = chorda_ex_literis(titulus, piscina);
    h.valor    = chorda_ex_literis(valor, piscina);
    h.numquam  = FALSUM;
    redde h;
}

interior i32
petere (
          Http2Connexio* c,
     constans character* methodus,
     constans character* via,
                 chorda  corpus)
{
    HpackCaput h[IV];

    h[ZEPHYRUM]  = caput(":method", methodus);
    h[I]         = caput(":scheme", "https");
    h[II]        = caput(":authority", "localhost");
    h[III]       = caput(":path", via);
    redde http2_petere(c, h, IV, corpus);
}

interior i32
_status_legere (
    chorda valor)
{
    i32 s;
    i32 i;

    s = ZEPHYRUM;
    per (i = ZEPHYRUM; i < valor.mensura; i++)
    {
        s = s * X + (i32)(valor.datum[i] - '0');
    }
    redde s;
}

/* octeti servi in connexionem fusi per frusta; eventus in reditis */
interior b32
iterare (
    Http2Connexio* c,
           chorda  octeti,
          Reditum* redita,
              i32  numerus,
              b32* discessus)
{
    i32 pos;

    pos = ZEPHYRUM;
    dum (pos < octeti.mensura)
    {
        Http2Eventus  ev;
         constans i8* d;
                 i32  n;
                 i32  k;

        n = octeti.mensura - pos;
        si (n > FRUSTUM)
        {
            n = FRUSTUM;
        }
        si (!http2_recipere(c, octeti.datum + pos, n))
        {
            redde FALSUM;
        }
        pos += n;
        dum (http2_eventum_capere(c, &ev, piscina))
        {
            Reditum* r = NIHIL;

            per (k = ZEPHYRUM; k < numerus; k++)
            {
                si (redita[k].rivus == ev.rivus)
                {
                    r = &redita[k];
                }
            }
            si (ev.genus == HTTP2_EVENTUS_DISCESSUS)
            {
                *discessus = VERUM;
            }
            si (!r)
            {
                perge;
            }
            si (   ev.genus  == HTTP2_EVENTUS_CAPITA
                && r->status == ZEPHYRUM
                && ev.capita && xar_numerus(ev.capita) > ZEPHYRUM)
            {
                r->status = _status_legere(((HpackCaput*)xar_obtinere(
                    ev.capita, ZEPHYRUM))->valor);
            }
            si (ev.genus == HTTP2_EVENTUS_DATA)
            {
                (vacuum)chorda_aedificator_appendere_chorda(r->corpus,
                    ev.data);
            }
            si (ev.genus == HTTP2_EVENTUS_FINIS)
            {
                r->finis = VERUM;
            }
        }
        n = http2_mittenda(c, &d);
        http2_mittenda_consumere(c, n);
    }
    redde VERUM;
}

interior chorda
corpus_redditum (
    i32 n)
{
    chorda s;
       i32 i;

    s.mensura = n;
    s.datum = (i8*)piscina_allocare(piscina, (memoriae_index)(n
        + I));
    per (i = ZEPHYRUM; i < n; i++)
    {
        s.datum[i] = (i8)('A' + i % XXVI);
    }
    redde s;
}

/* corpus /magnum: Go eandem partem XVI KiB iterat */
interior b32
magnum_rectum (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] != (i8)('a' + (i % PARS_ORACULI) % XXVI))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_reproductio (
    CredoContextus* c)
{
    i32 e;

    (vacuum)c;
    per (e = ZEPHYRUM; e < V; e++)
    {
        Http2Connexio* h2;
              Reditum  redita[MULTAE];
               chorda  vacuum_corpus;
               chorda  corpus;
                  i32  numerus;
                  i32  k;
                  b32  discessus;

        CREDO_GRADUS(EXEMPLA[e]);
        h2 = http2_connexionem_creare(HTTP2_CLIENS, NIHIL, piscina);
        CREDO_NECESSE_NON_NIHIL(h2);
        vacuum_corpus.datum    = NIHIL;
        vacuum_corpus.mensura  = ZEPHYRUM;
        memset(redita, ZEPHYRUM, magnitudo(redita));
        numerus = (e == II) ? MULTAE : I;
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            character via[XXXII];

            redita[k].corpus = chorda_aedificator_creare(piscina,
                MXXIV);
            si (e == ZEPHYRUM || e == II)
            {
                redita[k].rivus = petere(h2, "GET", "/salve",
                    vacuum_corpus);
            }
            alioquin si (e == I)
            {
                redita[k].rivus = petere(h2, "POST", "/eco",
                    corpus_redditum(REDDENDA));
            }
            alioquin si (e == III)
            {
                sprintf(via, "/magnum?n=%u",
                    (insignatus integer)MAGNUM);
                redita[k].rivus = petere(h2, "GET", via, vacuum_corpus);
            }
            alioquin
            {
                redita[k].rivus = petere(h2, "GET", "/abire",
                    vacuum_corpus);
            }
        }
        discessus = FALSUM;
        CREDO_VERUM(iterare(h2, legere(EXEMPLA[e], "servus.bin"),
            redita,
            numerus, &discessus));
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            CREDO_AEQUALIS_I32(redita[k].status, CC);
            CREDO_VERUM(redita[k].finis);
        }
        corpus = chorda_aedificator_finire(redita[ZEPHYRUM].corpus);
        si (e == ZEPHYRUM || e == II)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "salve");
        }
        alioquin si (e == I)
        {
            CREDO_AEQUALIS_I32(corpus.mensura, REDDENDA);
            CREDO_VERUM(corpus.mensura == REDDENDA
                && memcmp(corpus.datum, corpus_redditum(REDDENDA).datum,
                       REDDENDA) == ZEPHYRUM);
        }
        alioquin si (e == III)
        {
            CREDO_AEQUALIS_I32(corpus.mensura, MAGNUM);
            CREDO_VERUM(magnum_rectum(corpus));
        }
        alioquin
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(corpus, "vale");
        }
        CREDO_VERUM(http2_viva(h2));
    }
}

interior vacuum
_sectio_cliens (
    CredoContextus* c)
{
    i32 e;

    (vacuum)c;
    per (e = ZEPHYRUM; e < V; e++)
    {
               chorda octeti = legere(EXEMPLA[e], "cliens.bin");
           Http2Forma f;
        Http2Iudicium j;
                  i32 k;
                  i32 q;
                  i32 id;
                  i32 valor;
                  i32 visa;

        CREDO_GRADUS(EXEMPLA[e]);
        CREDO_NECESSE(octeti.mensura > HTTP2_PRAEFATIO_MENSURA);
        CREDO_VERUM(memcmp(octeti.datum, HTTP2_PRAEFATIO,
            HTTP2_PRAEFATIO_MENSURA) == ZEPHYRUM);
        k = http2_formam_legere(octeti.datum + HTTP2_PRAEFATIO_MENSURA,
            octeti.mensura - HTTP2_PRAEFATIO_MENSURA, FORMA_MAXIMA, &f,
            &j);
        CREDO_VERUM(k > ZEPHYRUM && j.bona);
        CREDO_AEQUALIS_I32(f.genus, HTTP2_POSITIONES);
        visa = ZEPHYRUM;
        per (q = ZEPHYRUM; http2_positionem_legere(&f, q, &id,
            &valor); q++)
        {
            si (id == HTTP2_POSITIO_IMPULSUS && valor == ZEPHYRUM)
            {
                visa |= I;
            }
            si (id == HTTP2_POSITIO_TABULA && valor == (MMXLVIII * II))
            {
                visa |= II;
            }
            si (id == HTTP2_POSITIO_FENESTRA && valor == 0x100000)
            {
                visa |= IV;
            }
            si (id == HTTP2_POSITIO_CAPITA && valor == 0x10000)
            {
                visa |= VIII;
            }
        }
        CREDO_AEQUALIS_I32(visa, XV);
        q = http2_formam_legere(octeti.datum + HTTP2_PRAEFATIO_MENSURA
            + k,
            octeti.mensura - HTTP2_PRAEFATIO_MENSURA - k, FORMA_MAXIMA,
            &f,
            &j);
        CREDO_VERUM(q > ZEPHYRUM && j.bona);
        CREDO_AEQUALIS_I32(f.genus, HTTP2_FENESTRA);
        CREDO_AEQUALIS_I32(f.rivus, ZEPHYRUM);
        CREDO_AEQUALIS_I32(http2_u32_legere(f.contentum),
            0x1000000 - 0xffff);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "rotundum (a)", _sectio_rotundum, _parare, _purgare, NIHIL,
        NIHIL },
    { "series Go (b)", _sectio_oraculi, _parare, _purgare, NIHIL,
        NIHIL },
    { "reproductio (c)", _sectio_reproductio, _parare, _purgare, NIHIL,
        NIHIL },
    { "praefatio nostra (d)", _sectio_cliens, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http2_reproductio", SECTIONES);
}
