/* probatio_http2.c - connexio HTTP/2, munus clientis (RFC 9113;
 * http2-plan T5). Probatio servum agit: formas per http2_forma et
 * capita per hpack scribit et octetos in http2_recipere fundit;
 * exitum clientis per http2_mittenda legit et decodat. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "hpack.h"
#include "http2_forma.h"
#include "http2.h"
#include "credo.h"
#include <string.h>

#define FORMA_ORDINARIA   (MMXLVIII * VIII)
#define TABULA_ORDINARIA  (MMXLVIII * II)

hic_manens Piscina* piscina;

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

/* servus fictus: codificator suus (ad clientem), decodator (a cliente),
 * formae clientis lectae */
nomen structura {
    Http2Connexio* c;
      HpackTabula* codificator;
      HpackTabula* decodator;
               i8  formae_octeti[XVI][CCLVI];
       Http2Forma  formae[XVI];
              i32  numerus;
              b32  praefatio_lecta;
} Servus;

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

/* exitum clientis legere: praefatio (semel) et formae in s->formae */
interior b32
exitum_legere (
    Servus* s)
{
    constans i8* d;
            i32  n;
            i32  pos;

    s->numerus  = ZEPHYRUM;
    n           = http2_mittenda(s->c, &d);
    pos         = ZEPHYRUM;
    si (!s->praefatio_lecta)
    {
        si (   n < HTTP2_PRAEFATIO_MENSURA
            || memcmp(d, HTTP2_PRAEFATIO, HTTP2_PRAEFATIO_MENSURA)
               != ZEPHYRUM)
        {
            redde FALSUM;
        }
        pos                 = HTTP2_PRAEFATIO_MENSURA;
        s->praefatio_lecta  = VERUM;
    }
    dum (pos < n && s->numerus < XVI)
    {
        Http2Iudicium j;
                  i32 k;

        k = http2_formam_legere(d + pos, n - pos, FORMA_ORDINARIA,
            &s->formae[s->numerus], &j);
        si (k == ZEPHYRUM || !j.bona || k > CCLVI)
        {
            redde FALSUM;
        }
        /* copia: exitus clientis post consumptionem reusatur */
        memcpy(s->formae_octeti[s->numerus], d + pos, (size_t)k);
        s->formae[s->numerus].contentum = s->formae_octeti[s->numerus]
            + (s->formae[s->numerus].contentum - (d + pos));
        s->numerus++;
        pos += k;
    }
    http2_mittenda_consumere(s->c, n);
    redde pos == n;
}

/* forma k generis g, aut -1 */
interior s32
forma_generis (
    Servus* s,
       i32  genus,
       i32  post)
{
    i32 k;

    per (k = post; k < s->numerus; k++)
    {
        si (s->formae[k].genus == genus)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

interior b32
fundere (
             Servus* s,
                i32  genus,
                i32  vexilla,
                i32  rivus,
        constans i8* contentum,
                i32  n)
{
     i8 b[CCLVI + XVI];
    i32 m;

    m = http2_formam_scribere(genus, vexilla, rivus, contentum, n, b,
        CCLVI + XVI);
    redde m > ZEPHYRUM && http2_recipere(s->c, b, m);
}

/* responsum capitum: HEADERS (FINIS_CAPITUM | vexilla) */
interior b32
capita_fundere (
                 Servus* s,
                    i32  rivus,
    constans HpackCaput* capita,
                    i32  numerus,
                    i32  vexilla)
{
     i8 massa[CCLVI];
    i32 n;

    n = hpack_codificare(s->codificator, capita, numerus, massa, CCLVI);
    redde n > ZEPHYRUM && fundere(s, HTTP2_CAPITA,
        HTTP2_FINIS_CAPITUM | vexilla, rivus, massa, n);
}

/* client novus, praefatio lecta, positiones servi vacuae et receptae */
interior b32
servum_parare (
                     Servus* s,
    constans Http2Positiones* positiones)
{
    memset(s, ZEPHYRUM, magnitudo(*s));
    s->c            = http2_connexionem_creare(HTTP2_CLIENS, positiones,
        piscina);
    s->codificator  = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    s->decodator    = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    redde s->c && exitum_legere(s)
        && fundere(s, HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, NIHIL,
               ZEPHYRUM)
        && fundere(s, HTTP2_POSITIONES, HTTP2_RECEPTUM, ZEPHYRUM, NIHIL,
               ZEPHYRUM)
        && exitum_legere(s);
}

interior i32
petere_get (
                Servus* s,
    constans character* via)
{
    HpackCaput h[IV];
        chorda vacuum_corpus;

    h[ZEPHYRUM] = caput(":method", "GET");
    h[I] = caput(":scheme", "https");
    h[II] = caput(":authority", "exemplum.example.org");
    h[III] = caput(":path", via);
    vacuum_corpus.datum = NIHIL;
    vacuum_corpus.mensura = ZEPHYRUM;
    redde http2_petere(s->c, h, IV, vacuum_corpus);
}

interior b32
eventum (
                Servus* s,
          Http2Eventus* ev,
     Http2EventusGenus  genus,
                   i32  rivus)
{
    redde http2_eventum_capere(s->c, ev, piscina) && ev->genus == genus
        && ev->rivus == rivus;
}

interior b32
status_est (
          Http2Eventus* ev,
    constans character* status)
{
    HpackCaput* h;

    si (!ev->capita || xar_numerus(ev->capita) < I)
    {
        redde FALSUM;
    }
    h = (HpackCaput*)xar_obtinere(ev->capita, ZEPHYRUM);
    redde chorda_aequalis_literis(h->titulus, ":status")
        && chorda_aequalis_literis(h->valor, status);
}

/* connexio mortua cum GOAWAY codicis 'codex' in exitu */
interior b32
mortua_cum (
    Servus* s,
       i32  codex)
{
             s32 k;
    Http2Eventus ev;
             b32 error;

    error = FALSUM;
    dum (http2_eventum_capere(s->c, &ev, piscina))
    {
        si (ev.genus == HTTP2_EVENTUS_ERROR && ev.codex == codex)
        {
            error = VERUM;
        }
    }
    si (!exitum_legere(s))
    {
        redde FALSUM;
    }
    k = forma_generis(s, HTTP2_DISCESSUS, ZEPHYRUM);
    redde error && !http2_viva(s->c) && k >= ZEPHYRUM
        && http2_u32_legere(s->formae[k].contentum + IV) == codex;
}

interior vacuum
_sectio_praefatio (
    CredoContextus* c)
{
    Servus s;
       s32 k;
       i32 id;
       i32 valor;
       i32 j;
       b32 impulsus_nullus;

    (vacuum)c;
    CREDO_GRADUS("praefatio, SETTINGS (ENABLE_PUSH 0), WINDOW_UPDATE");
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.c = http2_connexionem_creare(HTTP2_CLIENS, NIHIL, piscina);
    CREDO_NECESSE_NON_NIHIL(s.c);
    CREDO_VERUM(exitum_legere(&s));
    CREDO_VERUM(s.numerus >= II);
    CREDO_AEQUALIS_I32(s.formae[ZEPHYRUM].genus, HTTP2_POSITIONES);
    impulsus_nullus = FALSUM;
    per (j = ZEPHYRUM; http2_positionem_legere(&s.formae[ZEPHYRUM], j,
        &id,
             &valor); j++)
    {
        si (id == HTTP2_POSITIO_IMPULSUS && valor == ZEPHYRUM)
        {
            impulsus_nullus = VERUM;
        }
    }
    CREDO_VERUM(impulsus_nullus);
    k = forma_generis(&s, HTTP2_FENESTRA, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM && s.formae[k].rivus == ZEPHYRUM);

    CREDO_GRADUS("positiones servi -> RECEPTUM");
    CREDO_VERUM(fundere(&s, HTTP2_POSITIONES, ZEPHYRUM, ZEPHYRUM, NIHIL,
        ZEPHYRUM));
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_POSITIONES, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM
        && (s.formae[k].vexilla & HTTP2_RECEPTUM));
    CREDO_VERUM(fundere(&s, HTTP2_POSITIONES, HTTP2_RECEPTUM, ZEPHYRUM,
        NIHIL, ZEPHYRUM));
    CREDO_VERUM(http2_viva(s.c));

    CREDO_GRADUS("forma prima non SETTINGS: PROTOCOL_ERROR");
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.c = http2_connexionem_creare(HTTP2_CLIENS, NIHIL, piscina);
    CREDO_VERUM(exitum_legere(&s));
    CREDO_FALSUM(fundere(&s, HTTP2_PULSUS, ZEPHYRUM, ZEPHYRUM,
        (constans i8*)"12345678", VIII));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_PROTOCOLLI));

    CREDO_GRADUS("munus servi: nondum");
    CREDO_VERUM(http2_connexionem_creare(HTTP2_SERVUS, NIHIL, piscina)
        == NIHIL);
}

interior vacuum
_sectio_petitio (
    CredoContextus* c)
{
          Servus  s;
    Http2Eventus  ev;
      HpackCaput  h[V];
             Xar* capita;
          chorda  corpus;
             s32  k;

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("GET: HEADERS rivo I cum FINIS_RIVI");
    CREDO_AEQUALIS_I32(petere_get(&s, "/salve"), I);
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_CAPITA, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM);
    CREDO_AEQUALIS_I32(s.formae[k].rivus, I);
    CREDO_AEQUALIS_I32(s.formae[k].vexilla
        & (HTTP2_FINIS_RIVI | HTTP2_FINIS_CAPITUM),
        HTTP2_FINIS_RIVI | HTTP2_FINIS_CAPITUM);
    capita = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    CREDO_AEQUALIS_I32((i32)hpack_decodere(s.decodator,
        s.formae[k].contentum, s.formae[k].contentum_n, ZEPHYRUM,
        piscina, capita), (i32)HPACK_BENE);
    CREDO_AEQUALIS_I32(xar_numerus(capita), IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(((HpackCaput*)xar_obtinere(capita,
        III))->valor, "/salve");

    CREDO_GRADUS("responsum: CAPITA, DATA, FINIS");
    h[ZEPHYRUM] = caput(":status", "200");
    CREDO_VERUM(capita_fundere(&s, I, h, I, ZEPHYRUM));
    CREDO_VERUM(fundere(&s, HTTP2_DATA, HTTP2_FINIS_RIVI, I,
        (constans i8*)"salve", V));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_VERUM(status_est(&ev, "200"));
    CREDO_FALSUM(ev.informativa);
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_DATA, I));
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.data, "salve");
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_FINIS, I));
    CREDO_FALSUM(http2_eventum_capere(s.c, &ev, piscina));

    CREDO_GRADUS("POST cum corpore: HEADERS, DATA FINIS_RIVI; rivus III");
    h[ZEPHYRUM]  = caput(":method", "POST");
    h[I]         = caput(":scheme", "https");
    h[II]        = caput(":authority", "exemplum.example.org");
    h[III]       = caput(":path", "/eco");
    h[IV]        = caput("content-type", "text/plain");
    corpus       = chorda_ex_literis("abc", piscina);
    CREDO_AEQUALIS_I32(http2_petere(s.c, h, V, corpus), III);
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_CAPITA, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM
        && !(s.formae[k].vexilla & HTTP2_FINIS_RIVI));
    k = forma_generis(&s, HTTP2_DATA, (i32)k);
    CREDO_VERUM(k >= ZEPHYRUM
        && (s.formae[k].vexilla & HTTP2_FINIS_RIVI));
    CREDO_AEQUALIS_I32(s.formae[k].contentum_n, III);

    CREDO_GRADUS("pseudo-capita desunt: 0");
    h[ZEPHYRUM] = caput("x-a", "b");
    CREDO_AEQUALIS_I32(http2_petere(s.c, h, I, corpus), ZEPHYRUM);
}

interior vacuum
_sectio_informativa (
    CredoContextus* c)
{
          Servus s;
    Http2Eventus ev;
      HpackCaput h[I];

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("103, 200, DATA, appendices cum FINIS_RIVI");
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    h[ZEPHYRUM] = caput(":status", "103");
    CREDO_VERUM(capita_fundere(&s, I, h, I, ZEPHYRUM));
    h[ZEPHYRUM] = caput(":status", "200");
    CREDO_VERUM(capita_fundere(&s, I, h, I, ZEPHYRUM));
    CREDO_VERUM(fundere(&s, HTTP2_DATA, ZEPHYRUM, I,
        (constans i8*)"x", I));
    h[ZEPHYRUM] = caput("x-summa", "1");
    CREDO_VERUM(capita_fundere(&s, I, h, I, HTTP2_FINIS_RIVI));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_VERUM(ev.informativa && status_est(&ev, "103"));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_VERUM(!ev.informativa && status_est(&ev, "200"));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_DATA, I));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_FALSUM(ev.informativa);
    CREDO_CHORDA_AEQUALIS_LITERIS(((HpackCaput*)xar_obtinere(ev.capita,
        ZEPHYRUM))->titulus, "x-summa");
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_FINIS, I));
}

interior vacuum
_sectio_pulsus (
    CredoContextus* c)
{
          Servus s;
    Http2Eventus ev;
             s32 k;

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("PING servi -> PING RECEPTUM idem");
    CREDO_VERUM(fundere(&s, HTTP2_PULSUS, ZEPHYRUM, ZEPHYRUM,
        (constans i8*)"abcdefgh", VIII));
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_PULSUS, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM
        && (s.formae[k].vexilla & HTTP2_RECEPTUM));
    CREDO_VERUM(memcmp(s.formae[k].contentum, "abcdefgh", VIII)
        == ZEPHYRUM);

    CREDO_GRADUS("PING noster -> PULSUS in RECEPTO");
    CREDO_VERUM(http2_pulsare(s.c, (constans i8*)"12345678"));
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_PULSUS, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM
        && !(s.formae[k].vexilla & HTTP2_RECEPTUM));
    CREDO_VERUM(fundere(&s, HTTP2_PULSUS, HTTP2_RECEPTUM, ZEPHYRUM,
        (constans i8*)"12345678", VIII));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_PULSUS, ZEPHYRUM));
    CREDO_VERUM(ev.data.mensura == VIII
        && memcmp(ev.data.datum, "12345678", VIII) == ZEPHYRUM);
}

interior vacuum
_sectio_discessus (
    CredoContextus* c)
{
          Servus s;
    Http2Eventus ev;
      HpackCaput h[I];
              i8 g[VIII];
             b32 tertius;
             b32 conn;

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("GOAWAY ultimus I: rivus III retentabilis, petere 0");
    CREDO_AEQUALIS_I32(petere_get(&s, "/a"), I);
    CREDO_AEQUALIS_I32(petere_get(&s, "/b"), III);
    CREDO_VERUM(exitum_legere(&s));
    http2_u32_scribere(I, g);
    http2_u32_scribere(HTTP2_ERROR_NULLUS, g + IV);
    CREDO_VERUM(fundere(&s, HTTP2_DISCESSUS, ZEPHYRUM, ZEPHYRUM, g,
        VIII));
    tertius  = FALSUM;
    conn     = FALSUM;
    dum (http2_eventum_capere(s.c, &ev, piscina))
    {
        si (ev.genus == HTTP2_EVENTUS_DISCESSUS && ev.rivus == III)
        {
            tertius = VERUM;
        }
        si (   ev.genus == HTTP2_EVENTUS_DISCESSUS
            && ev.rivus == ZEPHYRUM && ev.rivus_ultimus == I)
        {
            conn = VERUM;
        }
    }
    CREDO_VERUM(tertius && conn);
    CREDO_AEQUALIS_I32(petere_get(&s, "/c"), ZEPHYRUM);

    CREDO_GRADUS("rivus I adhuc perficitur");
    h[ZEPHYRUM] = caput(":status", "200");
    CREDO_VERUM(capita_fundere(&s, I, h, I, HTTP2_FINIS_RIVI));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_FINIS, I));
}

interior vacuum
_sectio_abrogatio (
    CredoContextus* c)
{
          Servus s;
    Http2Eventus ev;
              i8 g[IV];
             s32 k;

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("RST_STREAM servi -> ABROGATUS cum codice");
    CREDO_AEQUALIS_I32(petere_get(&s, "/a"), I);
    CREDO_AEQUALIS_I32(petere_get(&s, "/b"), III);
    CREDO_VERUM(exitum_legere(&s));
    http2_u32_scribere(HTTP2_ERROR_ABROGATUM, g);
    CREDO_VERUM(fundere(&s, HTTP2_ABROGATIO, ZEPHYRUM, I, g, IV));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_ABROGATUS, I));
    CREDO_AEQUALIS_I32(ev.codex, HTTP2_ERROR_ABROGATUM);

    CREDO_GRADUS("abrogare nostrum -> RST_STREAM CANCEL; DATA postea tacet");
    CREDO_VERUM(http2_rivum_abrogare(s.c, III));
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_ABROGATIO, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM && s.formae[k].rivus == III
        && http2_u32_legere(s.formae[k].contentum)
           == HTTP2_ERROR_ABROGATUM);
    CREDO_VERUM(fundere(&s, HTTP2_DATA, HTTP2_FINIS_RIVI, III,
        (constans i8*)"x", I));
    CREDO_FALSUM(http2_eventum_capere(s.c, &ev, piscina));
    CREDO_VERUM(http2_viva(s.c));

    CREDO_GRADUS("iudicium rivi (WINDOW_UPDATE 0) -> RST_STREAM, ABROGATUS");
    CREDO_AEQUALIS_I32(petere_get(&s, "/c"), V);
    CREDO_VERUM(exitum_legere(&s));
    http2_u32_scribere(ZEPHYRUM, g);
    CREDO_VERUM(fundere(&s, HTTP2_FENESTRA, ZEPHYRUM, V, g, IV));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_ABROGATUS, V));
    CREDO_AEQUALIS_I32(ev.codex, HTTP2_ERROR_PROTOCOLLI);
    CREDO_VERUM(exitum_legere(&s));
    k = forma_generis(&s, HTTP2_ABROGATIO, ZEPHYRUM);
    CREDO_VERUM(k >= ZEPHYRUM && s.formae[k].rivus == V);
    CREDO_VERUM(http2_viva(s.c));
}

interior vacuum
_sectio_continuatio (
    CredoContextus* c)
{
             Servus s;
       Http2Eventus ev;
         HpackCaput h[II];
    Http2Positiones p;
                 i8 massa[CCLVI];
                i32 n;
                i32 i;

    (vacuum)c;
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_GRADUS("HEADERS + CONTINUATION: unum CAPITA");
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    h[ZEPHYRUM]  = caput(":status", "200");
    h[I]         = caput("x-longum", "abcdefghijklmnopqrstuvwxyz");
    n            = hpack_codificare(s.codificator, h, II, massa, CCLVI);
    CREDO_VERUM(n > X);
    CREDO_VERUM(fundere(&s, HTTP2_CAPITA, ZEPHYRUM, I, massa, V));
    CREDO_FALSUM(http2_eventum_capere(s.c, &ev, piscina));
    CREDO_VERUM(fundere(&s, HTTP2_CONTINUATIO, HTTP2_FINIS_CAPITUM, I,
        massa + V, n - V));
    CREDO_VERUM(eventum(&s, &ev, HTTP2_EVENTUS_CAPITA, I));
    CREDO_AEQUALIS_I32(xar_numerus(ev.capita), II);

    CREDO_GRADUS("plus VIII CONTINUATIONES: connexio moritur");
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), III);
    CREDO_VERUM(exitum_legere(&s));
    CREDO_VERUM(fundere(&s, HTTP2_CAPITA, ZEPHYRUM, III, massa, I));
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        CREDO_VERUM(fundere(&s, HTTP2_CONTINUATIO, ZEPHYRUM, III, massa,
            I));
    }
    CREDO_FALSUM(fundere(&s, HTTP2_CONTINUATIO, ZEPHYRUM, III, massa,
        I));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_MODERATIO));

    CREDO_GRADUS("massa supra octetos maximos: connexio moritur");
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.octeti_capitum_maximi = XVI;
    CREDO_NECESSE(servum_parare(&s, &p));
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    CREDO_VERUM(fundere(&s, HTTP2_CAPITA, ZEPHYRUM, I, massa, X));
    CREDO_FALSUM(fundere(&s, HTTP2_CONTINUATIO, HTTP2_FINIS_CAPITUM, I,
        massa, X));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_MODERATIO));

    CREDO_GRADUS("continuatio interrupta (Review Focus 3)");
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    CREDO_VERUM(fundere(&s, HTTP2_CAPITA, ZEPHYRUM, I, massa, V));
    CREDO_FALSUM(fundere(&s, HTTP2_DATA, ZEPHYRUM, I,
        (constans i8*)"x", I));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_PROTOCOLLI));
    CREDO_FALSUM(http2_recipere(s.c, massa, I));
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), ZEPHYRUM);
}

interior vacuum
_sectio_errores (
    CredoContextus* c)
{
    Servus s;
        i8 b[VIII];

    (vacuum)c;
    CREDO_GRADUS("PUSH_PROMISE: PROTOCOL_ERROR");
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    http2_u32_scribere(II, b);
    b[IV] = (i8)0x82;
    CREDO_FALSUM(fundere(&s, HTTP2_PROMISSIO, HTTP2_FINIS_CAPITUM, I, b,
        V));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_PROTOCOLLI));

    CREDO_GRADUS("massa HPACK mala: COMPRESSION_ERROR");
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    CREDO_AEQUALIS_I32(petere_get(&s, "/"), I);
    CREDO_VERUM(exitum_legere(&s));
    b[ZEPHYRUM] = (i8)0x80;
    CREDO_FALSUM(fundere(&s, HTTP2_CAPITA, HTTP2_FINIS_CAPITUM, I, b,
        I));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_COMPRESSIO));

    CREDO_GRADUS("HEADERS in rivo numquam aperto: PROTOCOL_ERROR");
    CREDO_NECESSE(servum_parare(&s, NIHIL));
    b[ZEPHYRUM] = (i8)0x88;
    CREDO_FALSUM(fundere(&s, HTTP2_CAPITA, HTTP2_FINIS_CAPITUM, VII, b,
        I));
    CREDO_VERUM(mortua_cum(&s, HTTP2_ERROR_PROTOCOLLI));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "praefatio", _sectio_praefatio, _parare, _purgare, NIHIL, NIHIL },
    { "petitio", _sectio_petitio, _parare, _purgare, NIHIL, NIHIL },
    { "informativa et appendices", _sectio_informativa, _parare,
        _purgare, NIHIL, NIHIL },
    { "pulsus", _sectio_pulsus, _parare, _purgare, NIHIL, NIHIL },
    { "discessus", _sectio_discessus, _parare, _purgare, NIHIL, NIHIL },
    { "abrogatio", _sectio_abrogatio, _parare, _purgare, NIHIL, NIHIL },
    { "continuatio", _sectio_continuatio, _parare, _purgare, NIHIL,
        NIHIL },
    { "errores", _sectio_errores, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http2", SECTIONES);
}
