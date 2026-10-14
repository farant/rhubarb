/* probatio_http_lector.c - lector responsi HTTP incrementalis
 * (quaestio-rivus-spec II.1)
 *
 * Casus quisque TOTUS, OCTETO SINGULO, et ad omnem divisionem binam
 * adicitur: status, caput, corpus et quod rivus accepit eadem esse
 * debent. Longitudo, chunked (extensio, postrema), clausura, non-2xx
 * (rivus tacet), HEAD/204 (nullum corpus), prava (hex, truncatum,
 * capita nimia), octeti post finem neglecti.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "http.h"
#include <string.h>

nomen structura {
    ChordaAedificator* aed;
                  i32  vocationes;
} Collector;

interior vacuum
_colligere (
          vacuum* datum,
     constans i8* octeti,
             i32  n)
{
    Collector* c = (Collector*)datum;
          i32  k;

    c->vocationes++;
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(c->aed,
            (character)octeti[k]);
    }
}

/* modus: 0 totus, 1 octetus singulus, k > 1 = divisio ad k */
interior HttpLector*
_currere (
    constans character* crudum,
                   s32  modus,
          HttpMethodus  methodus,
                   b32  finire,
             Collector* c,
               Piscina* p)
{
    HttpLector* l;
           i32  n = (i32)strlen(crudum);
           i32  k;

    c->aed         = chorda_aedificator_creare(p, LXIV);
    c->vocationes  = ZEPHYRUM;
    l              = http_lector_creare(p, methodus, _colligere, c);
    si (modus == ZEPHYRUM)
    {
        (vacuum)http_lector_adicere(l, (constans i8*)crudum, n);
    }
    alioquin si (modus == I)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            (vacuum)http_lector_adicere(l, (constans i8*)crudum + k, I);
        }
    }
    alioquin
    {
        (vacuum)http_lector_adicere(l, (constans i8*)crudum,
            (i32)modus);
        (vacuum)http_lector_adicere(l, (constans i8*)crudum + modus,
            n - (i32)modus);
    }
    si (finire)
    {
        (vacuum)http_lector_finire(l);
    }
    redde l;
}

/* omnes divisiones: status finalis, status HTTP, corpus, rivus */
interior vacuum
_omnes (
     CredoContextus* c,
 constans character* crudum,
        HttpMethodus methodus,
                 b32 finire,
    HttpLectorStatus status_exspectatus,
                 i32 status_http,
 constans character* corpus,
                 b32 rivus_loquitur)
{
    s32 modus;
    s32 n     = (s32)strlen(crudum);
    i32 mala  = ZEPHYRUM;

    per (modus = ZEPHYRUM; modus < n; modus++)
    {
         Collector  col;
        HttpLector* l = _currere(crudum, modus, methodus, finire, &col,
            c->piscina);
        HttpResponsum* r      = http_lector_responsum(l);
               chorda  rivus  = chorda_aedificator_finire(col.aed);

        si (http_lector_status(l) != status_exspectatus)
        {
            mala++;
            perge;
        }
        si (status_exspectatus == HTTP_LECTOR_ERROR)
        {
            perge;
        }
        si (   !r || r->status != status_http
            || !chorda_aequalis_literis(r->corpus, corpus)
            || (rivus_loquitur
                ? !chorda_aequalis_literis(rivus, corpus)
                : (col.vocationes != ZEPHYRUM)))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}

interior vacuum
_sectio_longitudo (
    CredoContextus* c)
{
     Collector  col;
    HttpLector* l;

    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 11\r\nX-Nota: b\r\n"
        "\r\nsalve munde", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS, CC,
        "salve munde", VERUM);
    /* caput legitur; octeti post finem neglecti */
    l = _currere("HTTP/1.1 200 OK\r\nContent-Length: 2\r\nX-Nota: b\r\n"
        "\r\nokXYZ", ZEPHYRUM, HTTP_GET, FALSUM, &col, c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_FINITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_responsum_caput(
        http_lector_responsum(l), "X-Nota"), "b");
    CREDO_CHORDA_AEQUALIS_LITERIS(http_lector_responsum(l)->corpus,
        "ok");
}

interior vacuum
_sectio_chunked (
    CredoContextus* c)
{
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "5;nomen=v\r\nsalve\r\n6\r\n munde\r\n0\r\nX-Postrema: q\r\n"
        "\r\n", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS, CC,
        "salve munde", VERUM);
    /* magnitudo hex maiuscula et minuscula */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "A\r\n0123456789\r\nb\r\nabcdefghijk\r\n0\r\n\r\n", HTTP_GET,
        FALSUM, HTTP_LECTOR_FINITUS, CC, "0123456789abcdefghijk",
        VERUM);
}

interior vacuum
_sectio_clausura (
    CredoContextus* c)
{
    /* sine longitudine: corpus usque ad EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"
        "usque ad finem", HTTP_GET, VERUM, HTTP_LECTOR_FINITUS, CC,
        "usque ad finem", VERUM);
}

interior vacuum
_sectio_non_ducenti (
    CredoContextus* c)
{
    /* corpus colligitur, rivus tacet */
    _omnes(c, "HTTP/1.1 529 Overloaded\r\nContent-Length: 15\r\n\r\n"
        "{\"error\":\"o\"}xx", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS,
        DXXIX, "{\"error\":\"o\"}xx", FALSUM);
}

interior vacuum
_sectio_sine_corpore (
    CredoContextus* c)
{
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n", HTTP_HEAD,
        FALSUM, HTTP_LECTOR_FINITUS, CC, "", FALSUM);
    _omnes(c, "HTTP/1.1 204 No Content\r\n\r\n", HTTP_GET, FALSUM,
        HTTP_LECTOR_FINITUS, CCIV, "", FALSUM);
}

interior vacuum
_sectio_prava (
    CredoContextus* c)
{
     Collector  col;
    HttpLector* l;
     character* ingens;
           i32  k;

    /* hex pravum */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "zz\r\nsalve\r\n", HTTP_GET, FALSUM, HTTP_LECTOR_ERROR, CC, "",
        FALSUM);
    /* chunked truncatum + EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "5\r\nsal", HTTP_GET, VERUM, HTTP_LECTOR_ERROR, CC, "", FALSUM);
    /* longitudo non impleta + EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nsalve",
        HTTP_GET, VERUM, HTTP_LECTOR_ERROR, CC, "", FALSUM);
    /* linea status prava */
    l = _currere("garbage\r\n\r\n", ZEPHYRUM, HTTP_GET, FALSUM, &col,
        c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_ERROR);
    CREDO_VERUM(http_lector_error(l).mensura > ZEPHYRUM);
    /* capita nimia */
    ingens = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(HTTP_LECTOR_CAPITA_MAXIMA + C));
    memcpy(ingens, "HTTP/1.1 200 OK\r\nX: ", XX);
    per (k = XX; k < HTTP_LECTOR_CAPITA_MAXIMA + L; k++)
    {
        ingens[k] = 'a';
    }
    ingens[k] = '\0';
    l = _currere(ingens, ZEPHYRUM, HTTP_GET, FALSUM, &col, c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_ERROR);
    /* EOF ante capita: vacuum */
    l = http_lector_creare(c->piscina, HTTP_GET, NIHIL, NIHIL);
    CREDO_AEQUALIS_I32((i32)http_lector_finire(l),
        (i32)HTTP_LECTOR_ERROR);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "longitudo", _sectio_longitudo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chunked", _sectio_chunked, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausura", _sectio_clausura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non 2xx", _sectio_non_ducenti, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine corpore", _sectio_sine_corpore, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "prava", _sectio_prava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http_lector", SECTIONES);
}
