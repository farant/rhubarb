/* hospitium.c - Servus HTTP domesticus
 *
 * Medium super tcp_servus + http + router + reactor. Filum unum,
 * non-obstruens. Vita memoriae connexionis: piscina propria; basis
 * notata ANTE parserem - ad terminum petitionis piscina_reficere +
 * parser NOVUS (numquam http_parser_reset: post refectionem penderet).
 * Refectio DIFFERTUR donec cauda scriptionis percolata sit - responsum
 * in eadem piscina vivit.
 */

#include "hospitium.h"
#include "tcp.h"
#include "reactor.h"
#include "router.h"
#include "mimen.h"
#include "via.h"
#include "url.h"
#include "filum.h"
#include "iter_directoria.h"
#include "tempus.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define HOSPITIUM_CONNEXIONES_TECTUM  CXXVIII
#define HOSPITIUM_DIRECTORIA_MAXIMA   VIII
#define HOSPITIUM_CAPITA_MAXIMA       XVI
#define HOSPITIUM_BUFFER_LECTIONIS    MMMMXCVI
#define HOSPITIUM_PISCINA_CONNEXIONIS (LXIV * M)

/* Responsum plenitudinis - sine piscina, ante admissionem missum */
hic_manens constans character* RESPONSUM_PLENITUDINIS =
    "HTTP/1.1 503 Service Unavailable\r\n"
    "Content-Length: 0\r\n"
    "Connection: close\r\n"
    "\r\n";


/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* Exemplar CAPSAE - router indices obiectorum solos tenet (ISO C
 * indices functionum in vacuum* vetat) */
nomen structura {
    HospitiumTractator tractator;
    vacuum*            datum;
} CapsaTractatoris;

nomen enumeratio {
    CONNEXIO_LEGENS = 0,
    CONNEXIO_SCRIBENS
} ConnexioStatus;

nomen structura {
    b32            activa;
    Hospitium*     hospitium;
    TcpConnexio*   tcp;
    integer        fd;
    Piscina*       piscina_connexionis;
    PiscinaNotatio basis;          /* ante parserem notata */
    HttpParser*    parser;

    /* cauda scriptionis */
    chorda         scribenda;
    i32            offset;         /* offset <= scribenda.mensura invariat */

    ConnexioStatus status;
    b32            keep_alive;
    b32            claudenda_post; /* post caudam percolatam claudere */
    b32            ob_reliquias;   /* clausura propter bytes pipelinatos */

    /* termini (secunda f64 - tempus_nunc directe comparata) */
    f64            terminus_otii;
    f64            terminus_capitum;
    b32            capita_legens;
} ConnexioHospitii;

nomen structura {
    chorda praefixum;
    chorda directorium;
} DirectoriumServiendum;

structura Hospitium {
    Piscina*   piscina;
    TcpServus* servus;
    Reactor*   reactor;
    Router*    router;

    /* configuratio resoluta */
    i32 connexiones_maximae;
    i32 petitio_maxima;     /* 0 = defalta parseris */
    i32 uri_maxima;
    f64 otium_maximum;      /* secunda */
    f64 capita_maxima;      /* secunda */
    b32 acao;
    b32 acta_accessus;

    ConnexioHospitii connexiones[HOSPITIUM_CONNEXIONES_TECTUM];
    i32 connexiones_numerus;

    DirectoriumServiendum directoria[HOSPITIUM_DIRECTORIA_MAXIMA];
    i32 directoria_numerus;

    HospitiumFructus fructus;
    b32 sistere_petitum;
};

structura HospitiumColloquium {
    Hospitium*          hospitium;
    ConnexioHospitii*   connexio;
    HttpPetitioServeri* petitio;
    RoutaParams*        params;
    vacuum*             datum;
    b32                 est_caput;   /* HEAD - corpus omittendum */

    HttpCaput           capita[HOSPITIUM_CAPITA_MAXIMA];
    i32                 capita_numerus;
    b32                 respondit;
};


/* ========================================================================
 * PRAENUNTIATIONES INTERNAE
 * ======================================================================== */

interior vacuum _connexio_callback(integer fd, i32 eventus, vacuum* data);
interior vacuum _auscultator_callback(integer fd, i32 eventus, vacuum* data);
interior vacuum _vigilia_callback(vacuum* data);
interior vacuum _connexionem_claudere(Hospitium* h, ConnexioHospitii* conn);
interior vacuum _scribere_tentare(Hospitium* h, ConnexioHospitii* conn);
interior vacuum _paratus_novae(Hospitium* h, ConnexioHospitii* conn);
interior vacuum _legere(Hospitium* h, ConnexioHospitii* conn);
interior vacuum _petitionem_expedire(Hospitium* h, ConnexioHospitii* conn);
interior vacuum _responsum_mittere(Hospitium* h, ConnexioHospitii* conn,
    HttpResponsum* responsum, b32 sine_corpore);
interior vacuum _respondere_plene(Hospitium* h, ConnexioHospitii* conn,
    i32 status, constans character* caput_titulus, chorda caput_valor,
    constans character* mimen_typus, chorda corpus, b32 sine_corpore);
interior vacuum _respondere_simplex(Hospitium* h, ConnexioHospitii* conn,
    i32 status, b32 sine_corpore);
interior b32 _directorium_tentare(Hospitium* h, ConnexioHospitii* conn,
    HttpPetitioServeri* petitio, b32 est_caput);


/* ========================================================================
 * AUXILIA
 * ======================================================================== */

/* Copia NUL-terminata pro API systematis (filum/iter) */
interior constans character*
_literis(chorda s, Piscina* piscina)
{
    character* l = (character*)piscina_allocare(piscina, (i64)(s.mensura + I));
    si (s.mensura > 0)
    {
        memcpy(l, s.datum, (size_t)s.mensura);
    }
    l[s.mensura] = '\0';
    redde l;
}

interior chorda
_chorda_vacua(vacuum)
{
    chorda vacua;
    vacua.datum = NIHIL;
    vacua.mensura = 0;
    redde vacua;
}


/* ========================================================================
 * CYCLUS VITAE CONNEXIONIS
 * ======================================================================== */

interior vacuum
_connexionem_claudere(Hospitium* h, ConnexioHospitii* conn)
{
    si (!conn->activa)
    {
        redde;
    }

    reactor_removere(h->reactor, conn->fd);
    tcp_claudere(conn->tcp);
    piscina_destruere(conn->piscina_connexionis);

    conn->activa = FALSUM;
    conn->piscina_connexionis = NIHIL;
    conn->parser = NIHIL;
    conn->tcp = NIHIL;
    h->connexiones_numerus--;
}

/* Terminus petitionis: refectio piscinae + parser NOVUS.
 * Vocatur SOLUM post caudam scriptionis percolatam. */
interior vacuum
_paratus_novae(Hospitium* h, ConnexioHospitii* conn)
{
    f64 nunc;

    piscina_reficere(conn->piscina_connexionis, conn->basis);
    conn->parser = http_parser_creare_cum_limitibus(
        conn->piscina_connexionis, h->petitio_maxima, h->uri_maxima);

    conn->scribenda = _chorda_vacua();
    conn->offset = 0;
    conn->status = CONNEXIO_LEGENS;
    conn->keep_alive = VERUM;
    conn->claudenda_post = FALSUM;
    conn->ob_reliquias = FALSUM;

    nunc = tempus_nunc();
    conn->capita_legens = VERUM;
    conn->terminus_capitum = nunc + h->capita_maxima;
    conn->terminus_otii = nunc + h->otium_maximum;

    reactor_modificare(h->reactor, conn->fd, (i32)REACTOR_LEGERE);
}

interior vacuum
_scribere_tentare(Hospitium* h, ConnexioHospitii* conn)
{
    dum (conn->offset < conn->scribenda.mensura)
    {
        s32 n = tcp_mittere(conn->tcp,
            (constans i8*)conn->scribenda.datum + conn->offset,
            conn->scribenda.mensura - conn->offset);

        si (n < 0)
        {
            _connexionem_claudere(h, conn);
            redde;
        }
        si (n == 0)
        {
            /* socket plenus - caudam servare, SCRIBERE armare */
            conn->status = CONNEXIO_SCRIBENS;
            conn->terminus_otii = tempus_nunc() + h->otium_maximum;
            reactor_modificare(h->reactor, conn->fd, (i32)REACTOR_SCRIBERE);
            redde;
        }
        conn->offset += (i32)n;
    }

    /* cauda percolata */
    h->fructus.responsa_missa++;

    si (conn->claudenda_post)
    {
        si (conn->ob_reliquias)
        {
            h->fructus.clausae_reliquiis++;
        }
        _connexionem_claudere(h, conn);
        redde;
    }

    _paratus_novae(h, conn);
}


/* ========================================================================
 * RESPONSA
 * ======================================================================== */

interior vacuum
_responsum_mittere(Hospitium* h, ConnexioHospitii* conn,
    HttpResponsum* responsum, b32 sine_corpore)
{
    Piscina* p = conn->piscina_connexionis;
    chorda serializata;

    /* ACAO uniformis in omni responso si petitum */
    si (h->acao)
    {
        HttpCaput* nova = (HttpCaput*)piscina_allocare(p,
            (i64)((responsum->capita_numerus + I)
                  * (i32)magnitudo(HttpCaput)));
        si (responsum->capita_numerus > 0)
        {
            memcpy(nova, responsum->capita,
                   (size_t)responsum->capita_numerus
                       * magnitudo(HttpCaput));
        }
        nova[responsum->capita_numerus].titulus =
            chorda_ex_literis("Access-Control-Allow-Origin", p);
        nova[responsum->capita_numerus].valor = chorda_ex_literis("*", p);
        responsum->capita = nova;
        responsum->capita_numerus++;
    }

    /* acta accessus - ante missionem (via ex parsere adhuc vivit) */
    si (h->acta_accessus)
    {
        HttpPetitioServeri* petitio = http_parser_obtinere_petitio(conn->parser);
        si (petitio)
        {
            fprintf(stderr, "[hospitium] %s %.*s %u %uB\n",
                http_methodus_nomen(petitio->methodus),
                (int)petitio->via.mensura,
                (constans character*)petitio->via.datum,
                responsum->status, responsum->corpus.mensura);
        }
    }

    serializata = http_responsum_serialize_cum_conexione(
        responsum, conn->keep_alive, p);

    /* HEAD: capita serializata cum Content-Length vero, corpus omissum */
    si (sine_corpore)
    {
        i32 i;
        per (i = 0; i + III < serializata.mensura; i++)
        {
            si (serializata.datum[i] == '\r'
                && serializata.datum[i + I] == '\n'
                && serializata.datum[i + II] == '\r'
                && serializata.datum[i + III] == '\n')
            {
                serializata.mensura = i + IV;
                frange;
            }
        }
    }

    conn->scribenda = serializata;
    conn->offset = 0;
    _scribere_tentare(h, conn);
}

interior vacuum
_respondere_plene(Hospitium* h, ConnexioHospitii* conn,
    i32 status, constans character* caput_titulus, chorda caput_valor,
    constans character* mimen_typus, chorda corpus, b32 sine_corpore)
{
    HttpResponsum responsum;
    HttpCaput capita_localia[II];
    i32 numerus = 0;

    si (caput_titulus)
    {
        capita_localia[numerus].titulus =
            chorda_ex_literis(caput_titulus, conn->piscina_connexionis);
        capita_localia[numerus].valor = caput_valor;
        numerus++;
    }
    si (mimen_typus)
    {
        capita_localia[numerus].titulus =
            chorda_ex_literis("Content-Type", conn->piscina_connexionis);
        capita_localia[numerus].valor =
            chorda_ex_literis(mimen_typus, conn->piscina_connexionis);
        numerus++;
    }

    memset(&responsum, 0, magnitudo(responsum));
    responsum.status = status;
    responsum.capita = capita_localia;
    responsum.capita_numerus = numerus;
    responsum.corpus = corpus;

    _responsum_mittere(h, conn, &responsum, sine_corpore);
}

interior vacuum
_respondere_simplex(Hospitium* h, ConnexioHospitii* conn,
    i32 status, b32 sine_corpore)
{
    _respondere_plene(h, conn, status, NIHIL, _chorda_vacua(), NIHIL,
                      _chorda_vacua(), sine_corpore);
}


/* ========================================================================
 * SERVITIO DIRECTORII
 * ======================================================================== */

interior vacuum
_indicem_generare(Hospitium* h, ConnexioHospitii* conn,
    constans character* via_disci, chorda via_paginae, b32 est_caput)
{
    Piscina* p = conn->piscina_connexionis;
    ChordaAedificator* aed = chorda_aedificator_creare(p, DXII);
    DirectoriumIterator* iter;
    DirectoriumIntroitus* intro;
    chorda corpus;

    chorda_aedificator_appendere_literis(aed, "<html><body><h1>");
    chorda_aedificator_appendere_chorda(aed, via_paginae);
    chorda_aedificator_appendere_literis(aed, "</h1><ul>");

    iter = directorium_iterator_aperire(via_disci, p);
    si (iter)
    {
        /* introitus vivit usque ad proximum SOLUM - intra ansam emittere */
        dum ((intro = directorium_iterator_proximum(iter)) != NIHIL)
        {
            chorda_aedificator_appendere_literis(aed, "<li><a href=\"");
            chorda_aedificator_appendere_chorda(aed, intro->titulus);
            si (intro->genus == INTROITUS_DIRECTORIUM)
            {
                chorda_aedificator_appendere_character(aed, '/');
            }
            chorda_aedificator_appendere_literis(aed, "\">");
            chorda_aedificator_appendere_chorda(aed, intro->titulus);
            si (intro->genus == INTROITUS_DIRECTORIUM)
            {
                chorda_aedificator_appendere_character(aed, '/');
            }
            chorda_aedificator_appendere_literis(aed, "</a></li>");
        }
        directorium_iterator_claudere(iter);
    }

    chorda_aedificator_appendere_literis(aed, "</ul></body></html>");
    corpus = chorda_aedificator_finire(aed);

    _respondere_plene(h, conn, CC, NIHIL, _chorda_vacua(), "text/html",
                      corpus, est_caput);
}

interior vacuum
_filum_regulare_servire(Hospitium* h, ConnexioHospitii* conn,
    constans character* via_disci, chorda via_typi, b32 est_caput)
{
    chorda contentum = filum_legere_totum(via_disci,
                                          conn->piscina_connexionis);

    _respondere_plene(h, conn, CC, NIHIL, _chorda_vacua(),
                      mimen_pro_via_chorda(via_typi), contentum, est_caput);
}

interior vacuum
_filum_servire(Hospitium* h, ConnexioHospitii* conn,
    DirectoriumServiendum* ds, chorda rel, chorda via_paginae, b32 est_caput)
{
    Piscina* p = conn->piscina_connexionis;
    chorda partes[II];
    chorda iuncta;
    chorda normalizata;
    constans character* via_disci;
    FilumStatus status_fili;

    si (rel.mensura > 0)
    {
        partes[0] = ds->directorium;
        partes[I] = rel;
        iuncta = via_iungere(partes, II, p);
    }
    alioquin
    {
        iuncta = ds->directorium;
    }
    normalizata = via_normalizare(iuncta, p);

    /* defensio traversalis: radix praefixum byteorum normalizatae sit,
     * cum limite segmenti ("/tmp/x" non praefixum "/tmp/xy") */
    si (!chorda_incipit(normalizata, ds->directorium)
        || (normalizata.mensura > ds->directorium.mensura
            && normalizata.datum[ds->directorium.mensura] != '/'))
    {
        _respondere_simplex(h, conn, CDIII, est_caput);
        redde;
    }

    via_disci = _literis(normalizata, p);

    si (!filum_status(via_disci, &status_fili))
    {
        _respondere_simplex(h, conn, CDIV, est_caput);
        redde;
    }

    si (status_fili.est_directorium)
    {
        chorda partes_indicis[II];
        chorda via_indicis;
        constans character* index_disci;

        partes_indicis[0] = normalizata;
        partes_indicis[I] = chorda_ex_literis("index.html", p);
        via_indicis = via_iungere(partes_indicis, II, p);
        index_disci = _literis(via_indicis, p);

        si (filum_existit(index_disci))
        {
            _filum_regulare_servire(h, conn, index_disci, via_indicis,
                                    est_caput);
            redde;
        }

        _indicem_generare(h, conn, via_disci, via_paginae, est_caput);
        redde;
    }

    si (status_fili.est_filum)
    {
        _filum_regulare_servire(h, conn, via_disci, normalizata, est_caput);
        redde;
    }

    _respondere_simplex(h, conn, CDIV, est_caput);
}

interior b32
_directorium_tentare(Hospitium* h, ConnexioHospitii* conn,
    HttpPetitioServeri* petitio, b32 est_caput)
{
    chorda decodificata;
    i32 d;

    si (h->directoria_numerus == 0)
    {
        redde FALSUM;
    }

    /* decodificatio PRIMA - %2e%2e traversalem celare non potest */
    decodificata = url_decodificare(petitio->via, conn->piscina_connexionis);

    per (d = 0; d < h->directoria_numerus; d++)
    {
        DirectoriumServiendum* ds = &h->directoria[d];
        chorda rel;

        si (!chorda_incipit(decodificata, ds->praefixum))
        {
            perge;
        }

        rel.datum = decodificata.datum + ds->praefixum.mensura;
        rel.mensura = decodificata.mensura - ds->praefixum.mensura;

        /* limes praefixi viae: "/staticx" non sub "/static" */
        si (rel.mensura > 0 && rel.datum[0] != '/')
        {
            perge;
        }

        _filum_servire(h, conn, ds, rel, decodificata, est_caput);
        redde VERUM;
    }

    redde FALSUM;
}


/* ========================================================================
 * EXPEDITIO PETITIONIS
 * ======================================================================== */

interior vacuum
_tractatorem_vocare(Hospitium* h, ConnexioHospitii* conn,
    HttpPetitioServeri* petitio, RoutaResultus* routa, b32 est_caput)
{
    HospitiumColloquium colloquium;
    CapsaTractatoris* capsa = (CapsaTractatoris*)routa->datum;

    memset(&colloquium, 0, magnitudo(colloquium));
    colloquium.hospitium = h;
    colloquium.connexio = conn;
    colloquium.petitio = petitio;
    colloquium.params = &routa->params;
    colloquium.datum = capsa->datum;
    colloquium.est_caput = est_caput;

    capsa->tractator(&colloquium);

    si (!colloquium.respondit)
    {
        /* tractator tacuit - error servi */
        _respondere_simplex(h, conn, D, est_caput);
    }
}

interior vacuum
_mittere_non_permissum(Hospitium* h, ConnexioHospitii* conn,
    i32 larva, b32 est_caput)
{
    ChordaAedificator* aed =
        chorda_aedificator_creare(conn->piscina_connexionis, LXIV);
    chorda allow;
    b32 primus = VERUM;
    i32 m;

    per (m = 0; m <= (i32)HTTP_OPTIONS; m++)
    {
        si (larva & ROUTA_METHODUS_BIT(m))
        {
            si (!primus)
            {
                chorda_aedificator_appendere_literis(aed, ", ");
            }
            chorda_aedificator_appendere_literis(aed,
                http_methodus_nomen((HttpMethodus)m));
            primus = FALSUM;
        }
    }
    si ((larva & ROUTA_METHODUS_BIT(HTTP_GET))
        && !(larva & ROUTA_METHODUS_BIT(HTTP_HEAD)))
    {
        chorda_aedificator_appendere_literis(aed, ", HEAD");
    }
    si (!(larva & ROUTA_METHODUS_BIT(HTTP_OPTIONS)))
    {
        chorda_aedificator_appendere_literis(aed, ", OPTIONS");
    }
    allow = chorda_aedificator_finire(aed);

    _respondere_plene(h, conn, CDV, "Allow", allow, NIHIL,
                      _chorda_vacua(), est_caput);
}

interior vacuum
_petitionem_expedire(Hospitium* h, ConnexioHospitii* conn)
{
    HttpPetitioServeri* petitio = http_parser_obtinere_petitio(conn->parser);
    HttpMethodus methodus_efficax;
    b32 est_caput;
    RoutaResultus routa;
    i32 reliquiae;

    conn->capita_legens = FALSUM;

    /* reliquiae = pipelining differtur: servire, respondere, claudere */
    reliquiae = http_parser_reliquiae(conn->parser);
    conn->keep_alive = petitio->keep_alive;
    si (reliquiae > 0)
    {
        conn->keep_alive = FALSUM;
        conn->ob_reliquias = VERUM;
    }
    conn->claudenda_post = !conn->keep_alive;

    est_caput = (petitio->methodus == HTTP_HEAD);
    methodus_efficax = est_caput ? HTTP_GET : petitio->methodus;

    routa = router_matching(h->router, methodus_efficax, petitio->via,
                            conn->piscina_connexionis);

    h->fructus.petitiones_tractatae++;

    si (routa.invenit)
    {
        _tractatorem_vocare(h, conn, petitio, &routa, est_caput);
        redde;
    }

    si (routa.via_inventa)
    {
        _mittere_non_permissum(h, conn, routa.methodi_permissae, est_caput);
        redde;
    }

    si (methodus_efficax == HTTP_GET
        && _directorium_tentare(h, conn, petitio, est_caput))
    {
        redde;
    }

    _respondere_simplex(h, conn, CDIV, est_caput);
}


/* ========================================================================
 * VOCAMINA REACTORIS
 * ======================================================================== */

interior vacuum
_legere(Hospitium* h, ConnexioHospitii* conn)
{
    i8 buffer[HOSPITIUM_BUFFER_LECTIONIS];

    per (;;)
    {
        HttpParseResultus res;
        s32 n = tcp_recipere(conn->tcp, buffer, HOSPITIUM_BUFFER_LECTIONIS);

        si (n == TCP_ITERUM)
        {
            redde;
        }
        si (n <= 0)
        {
            /* EOF (0) aut error (-1) */
            _connexionem_claudere(h, conn);
            redde;
        }

        conn->terminus_otii = tempus_nunc() + h->otium_maximum;

        res = http_parser_adicere(conn->parser,
                                  (constans character*)buffer, (i32)n);

        si (!res.successus)
        {
            h->fructus.petitiones_reiectae++;
            conn->keep_alive = FALSUM;
            conn->claudenda_post = VERUM;
            _respondere_simplex(h, conn,
                res.status_suggestus ? res.status_suggestus : (i32)CD,
                FALSUM);
            redde;
        }

        si (res.completa)
        {
            _petitionem_expedire(h, conn);
            redde;
        }
    }
}

interior vacuum
_connexio_callback(integer fd, i32 eventus, vacuum* data)
{
    ConnexioHospitii* conn = (ConnexioHospitii*)data;
    Hospitium* h = conn->hospitium;

    (vacuum)fd;

    si (!conn->activa)
    {
        redde;
    }

    si (eventus & (i32)REACTOR_ERROR)
    {
        _connexionem_claudere(h, conn);
        redde;
    }

    si (conn->status == CONNEXIO_SCRIBENS)
    {
        si (eventus & (i32)REACTOR_SCRIBERE)
        {
            _scribere_tentare(h, conn);
        }
        alioquin si (eventus & (i32)REACTOR_CLAUSUM)
        {
            _connexionem_claudere(h, conn);
        }
        redde;
    }

    si (eventus & (i32)REACTOR_LEGERE)
    {
        /* etiam cum CLAUSUM: lectio EOF honeste tractat */
        _legere(h, conn);
        redde;
    }

    si (eventus & (i32)REACTOR_CLAUSUM)
    {
        _connexionem_claudere(h, conn);
    }
}

interior ConnexioHospitii*
_slot_liberum(Hospitium* h)
{
    i32 i;
    per (i = 0; i < h->connexiones_maximae; i++)
    {
        si (!h->connexiones[i].activa)
        {
            redde &h->connexiones[i];
        }
    }
    redde NIHIL;
}

interior vacuum
_auscultator_callback(integer fd, i32 eventus, vacuum* data)
{
    Hospitium* h = (Hospitium*)data;

    (vacuum)fd;
    (vacuum)eventus;

    /* percolare usque ad TCP_ERROR_ITERUM */
    per (;;)
    {
        Piscina* p;
        TcpResultus acceptum;
        ConnexioHospitii* conn;
        f64 nunc;

        p = piscina_generare_dynamicum("hospitium_connexio",
                                       HOSPITIUM_PISCINA_CONNEXIONIS);
        acceptum = tcp_servus_accipere(h->servus, p);

        si (!acceptum.successus)
        {
            piscina_destruere(p);
            redde;  /* TCP_ERROR_ITERUM = percolatum; alii = relinquere */
        }

        conn = _slot_liberum(h);
        si (conn == NIHIL)
        {
            /* plenum: DIII visibile, non tergum SYN tacitum */
            h->fructus.connexiones_plenae_recusatae++;
            tcp_mittere(acceptum.connexio,
                (constans i8*)RESPONSUM_PLENITUDINIS,
                (i32)strlen(RESPONSUM_PLENITUDINIS));
            tcp_claudere(acceptum.connexio);
            piscina_destruere(p);
            perge;
        }

        memset(conn, 0, magnitudo(*conn));
        conn->activa = VERUM;
        conn->hospitium = h;
        conn->tcp = acceptum.connexio;
        conn->fd = (integer)tcp_obtinere_fd(acceptum.connexio);
        conn->piscina_connexionis = p;
        conn->basis = piscina_notare(p);   /* TcpConnexio ANTE basim */
        conn->parser = http_parser_creare_cum_limitibus(
            p, h->petitio_maxima, h->uri_maxima);
        conn->status = CONNEXIO_LEGENS;
        conn->keep_alive = VERUM;

        nunc = tempus_nunc();
        conn->capita_legens = VERUM;
        conn->terminus_capitum = nunc + h->capita_maxima;
        conn->terminus_otii = nunc + h->otium_maximum;

        h->connexiones_numerus++;
        h->fructus.connexiones_acceptae++;

        reactor_adicere(h->reactor, conn->fd, (i32)REACTOR_LEGERE,
                        _connexio_callback, conn);
    }
}

interior vacuum
_vigilia_callback(vacuum* data)
{
    Hospitium* h = (Hospitium*)data;
    f64 nunc = tempus_nunc();
    i32 i;

    per (i = 0; i < h->connexiones_maximae; i++)
    {
        ConnexioHospitii* conn = &h->connexiones[i];

        si (!conn->activa)
        {
            perge;
        }

        si ((conn->capita_legens && nunc > conn->terminus_capitum)
            || nunc > conn->terminus_otii)
        {
            h->fructus.clausae_otio++;
            _connexionem_claudere(h, conn);
        }
    }
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - CYCLUS VITAE
 * ======================================================================== */

Hospitium*
hospitium_creare(
    Piscina*                        piscina,
    constans HospitiumConfiguratio* configuratio)
{
    Hospitium* h;
    HospitiumConfiguratio cfg;
    TcpServusOptiones opt;
    TcpServusResultus servus_res;
    integer auscultator_fd;

    si (!piscina)
    {
        redde NIHIL;
    }

    si (configuratio)
    {
        cfg = *configuratio;
    }
    alioquin
    {
        memset(&cfg, 0, magnitudo(cfg));
    }

    h = (Hospitium*)piscina_allocare(piscina, (i64)magnitudo(Hospitium));
    si (!h)
    {
        redde NIHIL;
    }
    memset(h, 0, magnitudo(*h));
    h->piscina = piscina;

    h->connexiones_maximae = cfg.connexiones_maximae
        ? cfg.connexiones_maximae : CXXVIII;
    si (h->connexiones_maximae > HOSPITIUM_CONNEXIONES_TECTUM)
    {
        h->connexiones_maximae = HOSPITIUM_CONNEXIONES_TECTUM;
    }
    h->petitio_maxima = cfg.petitio_maxima;
    h->uri_maxima = cfg.uri_maxima;
    h->otium_maximum = (cfg.otium_maximum_ms > 0)
        ? ((f64)cfg.otium_maximum_ms / (f64)M) : (f64)XXX;
    h->capita_maxima = (cfg.capita_maxima_ms > 0)
        ? ((f64)cfg.capita_maxima_ms / (f64)M) : (f64)X;
    h->acao = cfg.acao;
    h->acta_accessus = cfg.acta_accessus;

    opt = tcp_servus_optiones_default();  /* non_blocans VERUM */
    servus_res = tcp_servus_creare_cum_optionibus(
        cfg.hospes ? cfg.hospes : "127.0.0.1",
        cfg.portus, &opt, piscina);
    si (!servus_res.successus)
    {
        redde NIHIL;
    }
    h->servus = servus_res.servus;

    si (tcp_servus_auscultare(h->servus, 0) != TCP_OK)
    {
        tcp_servus_claudere(h->servus);
        redde NIHIL;
    }

    h->reactor = reactor_creare(piscina);
    h->router = router_creare(piscina);
    si (!h->reactor || !h->router)
    {
        tcp_servus_claudere(h->servus);
        redde NIHIL;
    }

    auscultator_fd = tcp_servus_obtinere_fd(h->servus);
    reactor_adicere(h->reactor, auscultator_fd, (i32)REACTOR_LEGERE,
                    _auscultator_callback, h);
    reactor_timer_adicere(h->reactor, M, VERUM, _vigilia_callback, h);

    redde h;
}

vacuum
hospitium_destruere(Hospitium* hospitium)
{
    i32 i;

    si (!hospitium)
    {
        redde;
    }

    per (i = 0; i < HOSPITIUM_CONNEXIONES_TECTUM; i++)
    {
        si (hospitium->connexiones[i].activa)
        {
            _connexionem_claudere(hospitium, &hospitium->connexiones[i]);
        }
    }

    tcp_servus_claudere(hospitium->servus);
    reactor_destruere(hospitium->reactor);
}

b32
hospitium_praebere(
    Hospitium*          hospitium,
    HttpMethodus        methodus,
    constans character* via,
    HospitiumTractator  tractator,
    vacuum*             datum)
{
    CapsaTractatoris* capsa;

    si (!hospitium || !via || !tractator)
    {
        redde FALSUM;
    }

    capsa = (CapsaTractatoris*)piscina_allocare(hospitium->piscina,
        (i64)magnitudo(CapsaTractatoris));
    capsa->tractator = tractator;
    capsa->datum = datum;

    redde router_adicere(hospitium->router, methodus, via, capsa);
}

b32
hospitium_directorium_servire(
    Hospitium*          hospitium,
    constans character* praefixum,
    constans character* directorium)
{
    DirectoriumServiendum* ds;

    si (!hospitium || !praefixum || !directorium)
    {
        redde FALSUM;
    }
    si (hospitium->directoria_numerus >= HOSPITIUM_DIRECTORIA_MAXIMA)
    {
        redde FALSUM;
    }

    ds = &hospitium->directoria[hospitium->directoria_numerus];
    ds->praefixum = chorda_ex_literis(praefixum, hospitium->piscina);
    ds->directorium = chorda_ex_literis(directorium, hospitium->piscina);
    hospitium->directoria_numerus++;

    redde VERUM;
}

b32
hospitium_gressus(
    Hospitium* hospitium,
    s32        ms_maximae)
{
    si (!hospitium || hospitium->sistere_petitum)
    {
        redde FALSUM;
    }

    reactor_poll(hospitium->reactor,
                 (ms_maximae > 0) ? (i32)ms_maximae : 0);

    redde !hospitium->sistere_petitum;
}

vacuum
hospitium_currere(Hospitium* hospitium)
{
    si (!hospitium)
    {
        redde;
    }

    dum (hospitium_gressus(hospitium, (s32)M))
    {
        ;
    }
}

vacuum
hospitium_sistere(Hospitium* hospitium)
{
    si (!hospitium)
    {
        redde;
    }
    hospitium->sistere_petitum = VERUM;
}

i32
hospitium_portus(constans Hospitium* hospitium)
{
    si (!hospitium)
    {
        redde 0;
    }
    redde tcp_servus_obtinere_portum(hospitium->servus);
}

HospitiumFructus
hospitium_fructus(constans Hospitium* hospitium)
{
    HospitiumFructus vacuus;

    si (!hospitium)
    {
        memset(&vacuus, 0, magnitudo(vacuus));
        redde vacuus;
    }
    redde hospitium->fructus;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - COLLOQUIUM
 * ======================================================================== */

constans HttpPetitioServeri*
colloquium_petitio(HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->petitio;
}

chorda
colloquium_param(
    HospitiumColloquium* colloquium,
    constans character*  titulus)
{
    si (!colloquium)
    {
        redde _chorda_vacua();
    }
    redde router_param_obtinere(colloquium->params, titulus);
}

Piscina*
colloquium_piscina(HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->connexio->piscina_connexionis;
}

vacuum*
colloquium_datum(HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->datum;
}

vacuum
colloquium_caput_addere(
    HospitiumColloquium* colloquium,
    constans character*  titulus,
    constans character*  valor)
{
    Piscina* p;

    si (!colloquium || colloquium->respondit || !titulus || !valor)
    {
        redde;
    }
    si (colloquium->capita_numerus >= HOSPITIUM_CAPITA_MAXIMA)
    {
        redde;
    }

    p = colloquium->connexio->piscina_connexionis;
    colloquium->capita[colloquium->capita_numerus].titulus =
        chorda_ex_literis(titulus, p);
    colloquium->capita[colloquium->capita_numerus].valor =
        chorda_ex_literis(valor, p);
    colloquium->capita_numerus++;
}

vacuum
colloquium_respondere(
    HospitiumColloquium* colloquium,
    i32                  status,
    constans character*  mimen_typus,
    chorda               corpus)
{
    HttpResponsum responsum;
    Hospitium* h;
    ConnexioHospitii* conn;
    Piscina* p;

    si (!colloquium || colloquium->respondit)
    {
        redde;  /* uno ictu - vocatio prima vincit */
    }

    h = colloquium->hospitium;
    conn = colloquium->connexio;
    p = conn->piscina_connexionis;

    si (mimen_typus && colloquium->capita_numerus < HOSPITIUM_CAPITA_MAXIMA)
    {
        colloquium->capita[colloquium->capita_numerus].titulus =
            chorda_ex_literis("Content-Type", p);
        colloquium->capita[colloquium->capita_numerus].valor =
            chorda_ex_literis(mimen_typus, p);
        colloquium->capita_numerus++;
    }

    colloquium->respondit = VERUM;

    memset(&responsum, 0, magnitudo(responsum));
    responsum.status = status;
    responsum.capita = colloquium->capita;
    responsum.capita_numerus = colloquium->capita_numerus;
    responsum.corpus = corpus;

    _responsum_mittere(h, conn, &responsum, colloquium->est_caput);
}
