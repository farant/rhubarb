/* hospitium.c - Servus HTTP domesticus
 *
 * Medium super tcp_servus + http + router + reactor. Filum unum,
 * non-obstruens. Vita memoriae connexionis: piscina propria; basis
 * notata ANTE parserem - ad terminum petitionis piscina_reficere +
 * parser NOVUS (numquam http_parser_reset: post refectionem penderet).
 * Refectio DIFFERTUR donec cauda scriptionis percolata sit - responsum
 * in eadem piscina vivit.
 *
 * Colloquium in loco connexionis INCLUSUM est (memoria stabilis, nulla
 * allocatio). Generatio loci auget in omni petitione et clausura:
 * manubrium dilatum (locus, generatio) sic numquam petitionem alienam
 * attingit. Connexio EXSPECTANS (responsum dilatum) legere pergit solum
 * ut abitum clientis videat (EOF -> clausura -> manubrium irritum).
 * Fons corporis: post caudam capitum, partes CAPSAE_FONTIS ex fonte
 * trahuntur dum fossa haurit.
 *
 * Corpus petitionis (T3): parser in modo capita_solum sistit post
 * capita; corpus hic per rivum ad 'sentinam' (memoria, filum
 * temporarium, parser multipartitus) usque ad Content-Length. Fila
 * temporaria in piscina petitionis nominata; _corpus_purgare ea delet
 * ANTE refectionem et ante destructionem piscinae (etiam cliente
 * abeunte, termino, sistere).
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
#include "multipartita.h"
#include "moneta.h"
#include "lectiones.h"

#include <stdio.h>
#include <string.h>

#define HOSPITIUM_CONNEXIONES_TECTUM  CXXVIII
#define HOSPITIUM_DIRECTORIA_MAXIMA   VIII
#define HOSPITIUM_CAPITA_MAXIMA       XVI
#define HOSPITIUM_BUFFER_LECTIONIS    (IV * MXXIV)
#define HOSPITIUM_PISCINA_CONNEXIONIS (LXIV * M)
#define HOSPITIUM_CAPSA_FONTIS        (XVI * MXXIV)
#define HOSPITIUM_CORPUS_MEMORIA      (MXXIV * MXXIV)
#define HOSPITIUM_PARTES_MAXIMAE      LXIV

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
    HospitiumTractator  tractator;
                vacuum* datum;
} CapsaTractatoris;

nomen enumeratio {
    CONNEXIO_LEGENS = 0,
    CONNEXIO_SCRIBENS,
    CONNEXIO_EXSPECTANS      /* responsum dilatum */
} ConnexioStatus;

/* sentina corporis petitionis */
nomen enumeratio {
    CORPUS_NULLUM = 0,
    CORPUS_MEMORIA,
    CORPUS_FILUM,
    CORPUS_MULTIPARTITUM
} CorpusModus;

nomen structura ConnexioHospitii ConnexioHospitii;

structura HospitiumColloquium {
             Hospitium* hospitium;
      ConnexioHospitii* connexio;
    HttpPetitioServeri* petitio;
           RoutaParams* params;
                vacuum* datum;
                   b32  est_caput;   /* HEAD - corpus omittendum */

    HttpCaput capita[HOSPITIUM_CAPITA_MAXIMA];
          i32 capita_numerus;
          b32 respondit;
          b32 differtur;
};

structura ConnexioHospitii {
               b32  activa;
               i32  locus;           /* index in tabula connexionum */
               i32  generatio;       /* + omni petitione et clausura */
         Hospitium* hospitium;
       TcpConnexio* tcp;
           integer  fd;
           Piscina* piscina_connexionis;
    PiscinaNotatio  basis;          /* ante parserem notata */
        HttpParser* parser;

    /* cauda scriptionis */
    chorda scribenda;
       i32 offset;         /* offset <= scribenda.mensura invariat */

    ConnexioStatus status;
               b32 keep_alive;
               b32 claudenda_post; /* post caudam percolatam claudere */
               b32 ob_reliquias;   /* clausura propter bytes pipelinatos */

    /* termini (secunda f64 - tempus_nunc directe comparata) */
    f64 terminus_otii;
    f64 terminus_capitum;
    b32 capita_legens;
    b32 octeti_accepti;    /* petitio incepta (sistere molliter) */

    /* responsum dilatum */
    HospitiumColloquium colloquium;
                    b32 dilatum;
                    f64 terminus_responsi;

    /* fons corporis */
              b32  fons_activus;
    HospitiumFons  fons;
               i8* capsa_fontis;
              i64  fons_missa;

    /* corpus petitionis per rivum (omnia in piscina petitionis) */
            CorpusModus  corpus_modus;
                    i64  corpus_reliquum;
                     i8* corpus_memoria;
                    i32  corpus_acceptum;
          FilumScriptor* corpus_scriptor;
     constans character* corpus_via;
                    b32  reliquiae_corporis;   /* octeti post corpus */
                    i32  corpus_status;        /* error sentinae */
     MultipartitaParser* multipartita;
         HospitiumFilum* fila;
                    b32* fila_retenta;
                    i32  fila_numerus;
          FilumScriptor* filum_scriptor;       /* pars fili currens */
      SigillumContextus  filum_sigillum;
        HospitiumCampus* campi;
                    i32  campi_numerus;
      ChordaAedificator* campus_aedificator;
                    b32  pars_est_filum;
};

nomen structura {
    chorda praefixum;
    chorda directorium;
} DirectoriumServiendum;

structura Hospitium {
      Piscina* piscina;
    TcpServus* servus;
      Reactor* reactor;
       Router* router;

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

    /* reactor communis, auscultator, vigilia */
               b32 reactor_proprius;
           integer auscultator_fd;
    ReactorTimerId vigilia;
               f64 responsum_maximum;   /* secunda */

    /* sistere molliter */
    b32 molliter;
    f64 molliter_terminus;
    b32 sistit;

    /* corpora per rivum */
                    i32  corpus_in_memoria_maximum;
     constans character* directorium_temporarium;
};


/* ========================================================================
 * PRAENUNTIATIONES INTERNAE
 * ======================================================================== */

interior vacuum
_connexio_callback (
    integer  fd,
        i32  eventus,
     vacuum* data);
interior vacuum
_auscultator_callback (
    integer  fd,
        i32  eventus,
     vacuum* data);
interior vacuum
_vigilia_callback (
    vacuum* data);
interior vacuum
_connexionem_claudere (
           Hospitium* h,
    ConnexioHospitii* conn);
interior vacuum
_scribere_tentare (
           Hospitium* h,
    ConnexioHospitii* conn);
interior vacuum
_paratus_novae (
           Hospitium* h,
    ConnexioHospitii* conn);
interior vacuum
_legere (
           Hospitium* h,
    ConnexioHospitii* conn);
interior vacuum
_petitionem_expedire (
           Hospitium* h,
    ConnexioHospitii* conn);
interior vacuum
_responsum_mittere (
           Hospitium* h,
    ConnexioHospitii* conn,
       HttpResponsum* responsum,
                 b32  sine_corpore);
interior vacuum
_respondere_plene (
             Hospitium* h,
      ConnexioHospitii* conn,
                   i32  status,
    constans character* caput_titulus,
                chorda  caput_valor,
    constans character* mimen_typus,
                chorda  corpus,
                   b32  sine_corpore);
interior vacuum
_respondere_simplex (
           Hospitium* h,
    ConnexioHospitii* conn,
                 i32  status,
                 b32  sine_corpore);
interior b32
_directorium_tentare (
             Hospitium* h,
      ConnexioHospitii* conn,
    HttpPetitioServeri* petitio,
                   b32  est_caput);
interior vacuum
_corpus_purgare (
    ConnexioHospitii* conn);


/* ========================================================================
 * AUXILIA
 * ======================================================================== */

/* Copia NUL-terminata pro API systematis (filum/iter) */
interior constans character*
_literis (
     chorda  s,
    Piscina* piscina)
{
    character* l = (character*)piscina_allocare(piscina, (i64)(s.mensura
        + I));
    si (s.mensura > 0)
    {
        memcpy(l, s.datum, (size_t)s.mensura);
    }
    l[s.mensura] = '\0';
    redde l;
}

interior chorda
_chorda_vacua (vacuum)
{
    chorda vacua;
    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    redde vacua;
}


/* ========================================================================
 * CYCLUS VITAE CONNEXIONIS
 * ======================================================================== */

interior vacuum
_connexionem_claudere (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    si (!conn->activa)
    {
        redde;
    }

    si (conn->fons_activus)
    {
        si (conn->fons.claudere)
        {
            conn->fons.claudere(conn->fons.datum);
        }
        conn->fons_activus = FALSUM;
    }
    _corpus_purgare(conn);   /* fila temporaria ante piscinam */
    reactor_removere(h->reactor, conn->fd);
    tcp_claudere(conn->tcp);
    piscina_destruere(conn->piscina_connexionis);

    conn->generatio++;          /* manubria dilata irrita */
    conn->dilatum              = FALSUM;
    conn->activa               = FALSUM;
    conn->piscina_connexionis  = NIHIL;
    conn->parser               = NIHIL;
    conn->tcp                  = NIHIL;
    h->connexiones_numerus--;
}

/* Terminus petitionis: refectio piscinae + parser NOVUS.
 * Vocatur SOLUM post caudam scriptionis percolatam. */
interior vacuum
_paratus_novae (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    f64 nunc;

    _corpus_purgare(conn);   /* viae in piscina: ANTE refectionem */
    piscina_reficere(conn->piscina_connexionis, conn->basis);
    conn->parser = http_parser_creare_cum_limitibus(
        conn->piscina_connexionis, h->petitio_maxima, h->uri_maxima);
    http_parser_capita_solum(conn->parser, VERUM);

    conn->scribenda       = _chorda_vacua();
    conn->offset          = 0;
    conn->status          = CONNEXIO_LEGENS;
    conn->keep_alive      = VERUM;
    conn->claudenda_post  = FALSUM;
    conn->ob_reliquias    = FALSUM;
    conn->octeti_accepti  = FALSUM;
    conn->dilatum         = FALSUM;

    nunc                    = tempus_nunc();
    conn->capita_legens     = VERUM;
    conn->terminus_capitum  = nunc + h->capita_maxima;
    conn->terminus_otii     = nunc + h->otium_maximum;

    reactor_modificare(h->reactor, conn->fd, (i32)REACTOR_LEGERE);
}

interior vacuum
_scribere_tentare (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    per (;;)
    {
        s32 lecta;

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
                conn->status         = CONNEXIO_SCRIBENS;
                conn->terminus_otii  = tempus_nunc() + h->otium_maximum;
                reactor_modificare(h->reactor, conn->fd,
                    (i32)REACTOR_SCRIBERE);
                redde;
            }
            conn->offset += (i32)n;
        }

        /* cauda hausta: pars proxima fontis, si quis */
        si (!conn->fons_activus)
        {
            frange;
        }
        lecta = conn->fons.legere(conn->fons.datum, conn->capsa_fontis,
            HOSPITIUM_CAPSA_FONTIS);
        si (   lecta > 0 && conn->fons_missa + (i64)lecta
            <= conn->fons.mensura)
        {
            conn->fons_missa  += (i64)lecta;
            conn->scribenda    = chorda_ex_buffer(conn->capsa_fontis,
                (i32)lecta);
            conn->offset       = 0;
            perge;
        }
        si (conn->fons.claudere)
        {
            conn->fons.claudere(conn->fons.datum);
        }
        conn->fons_activus = FALSUM;
        si (lecta != 0 || conn->fons_missa != conn->fons.mensura)
        {
            /* fons fractus aut mensura mendax: clausura (cliens
             * corpus truncatum videt, non responsum falsum) */
            _connexionem_claudere(h, conn);
            redde;
        }
        frange;
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

/* ACAO + acta accessus, communia responsis plenis et fontibus */
interior vacuum
_responsum_parare (
           Hospitium* h,
    ConnexioHospitii* conn,
       HttpResponsum* responsum,
                 i64  longitudo)
{
    Piscina* p = conn->piscina_connexionis;

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
        nova[responsum->capita_numerus].valor = chorda_ex_literis("*",
            p);
        responsum->capita = nova;
        responsum->capita_numerus++;
    }

    /* acta accessus - ante missionem (via ex parsere adhuc vivit) */
    si (h->acta_accessus)
    {
        HttpPetitioServeri* petitio =
            http_parser_obtinere_petitio(conn->parser);
        si (petitio)
        {
            fprintf(stderr, "[hospitium] %s %.*s %u %luB\n",
                http_methodus_nomen(petitio->methodus),
                (int)petitio->via.mensura,
                (constans character*)petitio->via.datum,
                responsum->status, (insignatus longus)longitudo);
        }
    }
}

interior vacuum
_responsum_mittere (
           Hospitium* h,
    ConnexioHospitii* conn,
       HttpResponsum* responsum,
                 b32  sine_corpore)
{
    Piscina* p = conn->piscina_connexionis;
     chorda  serializata;

    _responsum_parare(h, conn, responsum,
        (i64)responsum->corpus.mensura);
    serializata = http_responsum_serialize_cum_conexione(
        responsum, conn->keep_alive, p);

    /* HEAD: capita serializata cum Content-Length vero, corpus omissum */
    si (sine_corpore)
    {
        i32 i;
        per (i = 0; i + III < serializata.mensura; i++)
        {
            si (   serializata.datum[i]       == '\r'
                && serializata.datum[i + I]   == '\n'
                && serializata.datum[i + II]  == '\r'
                && serializata.datum[i + III] == '\n')
            {
                serializata.mensura = i + IV;
                frange;
            }
        }
    }

    conn->scribenda  = serializata;
    conn->offset     = 0;
    _scribere_tentare(h, conn);
}

/* capita cum Content-Length fontis, deinde corpus per partes */
interior vacuum
_fontem_mittere (
           Hospitium* h,
    ConnexioHospitii* conn,
       HttpResponsum* responsum,
        HospitiumFons fons,
                 b32  sine_corpore)
{
    Piscina* p = conn->piscina_connexionis;

    _responsum_parare(h, conn, responsum, fons.mensura);
    conn->scribenda  = http_responsum_capita_serialize(responsum,
        conn->keep_alive, fons.mensura, p);
    conn->offset     = 0;
    si (sine_corpore || fons.mensura == 0)
    {
        si (fons.claudere)
        {
            fons.claudere(fons.datum);
        }
    }
    alioquin
    {
        conn->fons          = fons;
        conn->fons_activus  = VERUM;
        conn->fons_missa    = 0;
        conn->capsa_fontis  = (i8*)piscina_allocare(p,
            (i64)HOSPITIUM_CAPSA_FONTIS);
    }
    _scribere_tentare(h, conn);
}

/* fons ex filo (filum_octeti) */
interior s32
_fons_fili_legere (
    vacuum* datum,
        i8* capsa,
       i32  n)
{
    redde filum_octeti_legere((FilumOctetorumLector*)datum, capsa, n);
}

interior vacuum
_fons_fili_claudere (
    vacuum* datum)
{
    filum_octeti_claudere((FilumOctetorumLector*)datum);
}

/* fons fili paratus; FALSUM si aperiri nequit */
interior b32
_fontem_fili_parare (
    constans character* via,
               Piscina* p,
         HospitiumFons* fons)
{
    FilumOctetorumLector* l = filum_octeti_aperire(via, p);

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    fons->mensura   = (i64)filum_mensura(via);
    fons->legere    = _fons_fili_legere;
    fons->claudere  = _fons_fili_claudere;
    fons->datum     = l;
    redde VERUM;
}

interior vacuum
_respondere_plene (
             Hospitium* h,
      ConnexioHospitii* conn,
                   i32  status,
    constans character* caput_titulus,
                chorda  caput_valor,
    constans character* mimen_typus,
                chorda  corpus,
                   b32  sine_corpore)
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
            chorda_ex_literis("Content-Type",
            conn->piscina_connexionis);
        capita_localia[numerus].valor =
            chorda_ex_literis(mimen_typus, conn->piscina_connexionis);
        numerus++;
    }

    memset(&responsum, 0, magnitudo(responsum));
    responsum.status          = status;
    responsum.capita          = capita_localia;
    responsum.capita_numerus  = numerus;
    responsum.corpus          = corpus;

    _responsum_mittere(h, conn, &responsum, sine_corpore);
}

interior vacuum
_respondere_simplex (
           Hospitium* h,
    ConnexioHospitii* conn,
                 i32  status,
                 b32  sine_corpore)
{
    _respondere_plene(h, conn, status, NIHIL, _chorda_vacua(), NIHIL,
                      _chorda_vacua(), sine_corpore);
}


/* ========================================================================
 * SERVITIO DIRECTORII
 * ======================================================================== */

interior vacuum
_indicem_generare (
             Hospitium* h,
      ConnexioHospitii* conn,
    constans character* via_disci,
                chorda  via_paginae,
                   b32  est_caput)
{
                 Piscina* p    = conn->piscina_connexionis;
       ChordaAedificator* aed  = chorda_aedificator_creare(p, DXII);
     DirectoriumIterator* iter;
    DirectoriumIntroitus* intro;
                  chorda  corpus;

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
_filum_regulare_servire (
             Hospitium* h,
      ConnexioHospitii* conn,
    constans character* via_disci,
                chorda  via_typi,
                   b32  est_caput)
{
    HttpResponsum responsum;
        HttpCaput caput;
    HospitiumFons fons;

    si (!_fontem_fili_parare(via_disci, conn->piscina_connexionis,
        &fons))
    {
        _respondere_simplex(h, conn, CDIV, est_caput);
        redde;
    }
    caput.titulus = chorda_ex_literis("Content-Type",
        conn->piscina_connexionis);
    caput.valor = chorda_ex_literis(mimen_pro_via_chorda(via_typi),
        conn->piscina_connexionis);
    memset(&responsum, 0, magnitudo(responsum));
    responsum.status          = CC;
    responsum.capita          = &caput;
    responsum.capita_numerus  = I;
    _fontem_mittere(h, conn, &responsum, fons, est_caput);
}

interior vacuum
_filum_servire (
                Hospitium* h,
         ConnexioHospitii* conn,
    DirectoriumServiendum* ds,
                   chorda  rel,
                   chorda  via_paginae,
                      b32  est_caput)
{
               Piscina* p = conn->piscina_connexionis;
                chorda  partes[II];
                chorda  iuncta;
                chorda  normalizata;
    constans character* via_disci;
           FilumStatus  status_fili;

    si (rel.mensura > 0)
    {
        partes[0]  = ds->directorium;
        partes[I]  = rel;
        iuncta     = via_iungere(partes, II, p);
    }
    alioquin
    {
        iuncta = ds->directorium;
    }
    normalizata = via_normalizare(iuncta, p);

    /* defensio traversalis: radix praefixum byteorum normalizatae sit,
     * cum limite segmenti ("/tmp/x" non praefixum "/tmp/xy") */
    si (   !chorda_incipit(normalizata, ds->directorium)
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
                    chorda  partes_indicis[II];
                    chorda  via_indicis;
        constans character* index_disci;

        partes_indicis[0]  = normalizata;
        partes_indicis[I]  = chorda_ex_literis("index.html", p);
        via_indicis        = via_iungere(partes_indicis, II, p);
        index_disci        = _literis(via_indicis, p);

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
        _filum_regulare_servire(h, conn, via_disci, normalizata,
            est_caput);
        redde;
    }

    _respondere_simplex(h, conn, CDIV, est_caput);
}

interior b32
_directorium_tentare (
             Hospitium* h,
      ConnexioHospitii* conn,
    HttpPetitioServeri* petitio,
                   b32  est_caput)
{
    chorda decodificata;
       i32 d;

    si (h->directoria_numerus == 0)
    {
        redde FALSUM;
    }

    /* decodificatio PRIMA - %2e%2e traversalem celare non potest */
    decodificata = url_decodificare(petitio->via,
        conn->piscina_connexionis);

    per (d = 0; d < h->directoria_numerus; d++)
    {
        DirectoriumServiendum* ds = &h->directoria[d];
                       chorda  rel;

        si (!chorda_incipit(decodificata, ds->praefixum))
        {
            perge;
        }

        rel.datum    = decodificata.datum + ds->praefixum.mensura;
        rel.mensura  = decodificata.mensura - ds->praefixum.mensura;

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
_tractatorem_vocare (
             Hospitium* h,
      ConnexioHospitii* conn,
    HttpPetitioServeri* petitio,
         RoutaResultus* routa,
                   b32  est_caput)
{
    HospitiumColloquium* colloquium  = &conn->colloquium;
       CapsaTractatoris* capsa       = (CapsaTractatoris*)routa->datum;
            RoutaParams* params;

    /* params in piscina petitionis (routa est in acervo vocantis) */
    params = (RoutaParams*)piscina_allocare(conn->piscina_connexionis,
        (i64)magnitudo(RoutaParams));
    *params = routa->params;
    conn->generatio++;      /* petitio nova: manubria priora irrita */
    memset(colloquium, 0, magnitudo(*colloquium));
    colloquium->hospitium  = h;
    colloquium->connexio   = conn;
    colloquium->petitio    = petitio;
    colloquium->params     = params;
    colloquium->datum      = capsa->datum;
    colloquium->est_caput  = est_caput;

    capsa->tractator(colloquium);

    si (!colloquium->respondit)
    {
        si (colloquium->differtur)
        {
            /* responsum dilatum: legere solum ut abitus videatur */
            conn->status   = CONNEXIO_EXSPECTANS;
            conn->dilatum  = VERUM;
            conn->terminus_responsi  = tempus_nunc()
                + h->responsum_maximum;
            reactor_modificare(h->reactor, conn->fd,
                (i32)REACTOR_LEGERE);
            redde;
        }
        /* tractator tacuit - error servi */
        _respondere_simplex(h, conn, D, est_caput);
    }
}

interior vacuum
_mittere_non_permissum (
           Hospitium* h,
    ConnexioHospitii* conn,
                 i32  larva,
                 b32  est_caput)
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
    si (   (larva & ROUTA_METHODUS_BIT(HTTP_GET))
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
_petitionem_expedire (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    HttpPetitioServeri* petitio =
        http_parser_obtinere_petitio(conn->parser);
          HttpMethodus methodus_efficax;
                   b32 est_caput;
         RoutaResultus routa;
                   i32 reliquiae;

    conn->capita_legens = FALSUM;

    /* reliquiae = pipelining differtur: servire, respondere, claudere */
    reliquiae         = http_parser_reliquiae(conn->parser);
    conn->keep_alive  = petitio->keep_alive;
    si (reliquiae > 0 || conn->reliquiae_corporis)
    {
        conn->keep_alive    = FALSUM;
        conn->ob_reliquias  = VERUM;
    }
    si (h->molliter)
    {
        conn->keep_alive = FALSUM;   /* sistere molliter: ultima */
    }
    conn->claudenda_post = !conn->keep_alive;

    est_caput         = (petitio->methodus == HTTP_HEAD);
    methodus_efficax  = est_caput ? HTTP_GET : petitio->methodus;

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
        _mittere_non_permissum(h, conn, routa.methodi_permissae,
            est_caput);
        redde;
    }

    si (   methodus_efficax == HTTP_GET
        && _directorium_tentare(h, conn, petitio, est_caput))
    {
        redde;
    }

    _respondere_simplex(h, conn, CDIV, est_caput);
}


/* ========================================================================
 * CORPUS PETITIONIS PER RIVUM
 * ======================================================================== */

/* Caput petitionis (titulus sine casu); vacua si absens */
interior chorda
_caput_petitionis (
    constans HttpPetitioServeri* petitio,
             constans character* titulus)
{
    i32 n = (i32)strlen(titulus);
    i32 i;
    i32 k;

    per (i = 0; i < petitio->capita_numerus; i++)
    {
        chorda t = petitio->capita[i].titulus;

        si (t.mensura != n)
        {
            perge;
        }
        per (k = 0; k < n; k++)
        {
            character a = (character)t.datum[k];
            character b = titulus[k];

            si (a >= 'A' && a <= 'Z')
            {
                a = (character)(a - 'A' + 'a');
            }
            si (b >= 'A' && b <= 'Z')
            {
                b = (character)(b - 'A' + 'a');
            }
            si (a != b)
            {
                frange;
            }
        }
        si (k == n)
        {
            redde petitio->capita[i].valor;
        }
    }
    redde _chorda_vacua();
}

/* praefixum (minusculis datum) sine casu */
interior b32
_incipit_ignora_casus (
                chorda  s,
    constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);
    i32 i;

    si (s.mensura < n)
    {
        redde FALSUM;
    }
    per (i = 0; i < n; i++)
    {
        character a = (character)s.datum[i];

        si (a >= 'A' && a <= 'Z')
        {
            a = (character)(a - 'A' + 'a');
        }
        si (a != praefixum[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Directorium temporarium resolutum (NIHIL -> $TMPDIR -> /tmp), sine
 * '/' finali, in piscina servi */
interior constans character*
_directorium_temporarium (
                Piscina* piscina,
     constans character* datum)
{
     constans character* dir = datum;
              character* copia;
                 size_t  n;

    si (dir == NIHIL)
    {
        dir = lectiones_ambitus("TMPDIR");
    }
    si (dir == NIHIL || dir[0] == '\0')
    {
        dir = "/tmp";
    }
    n      = strlen(dir);
    copia  = (character*)piscina_allocare(piscina, (i64)(n + I));
    strcpy(copia, dir);
    si (n > I && copia[n - I] == '/')
    {
        copia[n - I] = '\0';   /* $TMPDIR macOS '/' finit */
    }
    redde copia;
}

/* Via fili temporarii nova (nanoid) in piscina petitionis */
interior constans character*
_via_temporaria (
           Hospitium* h,
    ConnexioHospitii* conn)
{
     character  signa[MONETA_NANOID_MENSURA];
     character* via;

    si (!moneta_nanoid(signa))
    {
        redde NIHIL;
    }
    via = (character*)piscina_allocare(conn->piscina_connexionis,
        (i64)(strlen(h->directorium_temporarium) + XLVIII));
    sprintf(via, "%s/hospitium-%s.tmp", h->directorium_temporarium,
        signa);
    redde via;
}

/* Fila temporaria delere (nisi retenta), scriptores claudere, statum
 * sentinae purgare. Tuta iterum. */
interior vacuum
_corpus_purgare (
    ConnexioHospitii* conn)
{
    i32 i;

    si (conn->corpus_scriptor)
    {
        filum_scriptor_claudere(conn->corpus_scriptor);
    }
    si (conn->filum_scriptor)
    {
        filum_scriptor_claudere(conn->filum_scriptor);
    }
    si (conn->corpus_via)
    {
        (vacuum)filum_delere(conn->corpus_via);
    }
    per (i = 0; i < conn->fila_numerus; i++)
    {
        si (!conn->fila_retenta[i] && conn->fila[i].via)
        {
            (vacuum)filum_delere(conn->fila[i].via);
        }
    }
    conn->corpus_modus        = CORPUS_NULLUM;
    conn->corpus_reliquum     = 0;
    conn->corpus_memoria      = NIHIL;
    conn->corpus_acceptum     = 0;
    conn->corpus_scriptor     = NIHIL;
    conn->corpus_via          = NIHIL;
    conn->reliquiae_corporis  = FALSUM;
    conn->corpus_status       = 0;
    conn->multipartita        = NIHIL;
    conn->fila                = NIHIL;
    conn->fila_retenta        = NIHIL;
    conn->fila_numerus        = 0;
    conn->filum_scriptor      = NIHIL;
    conn->campi               = NIHIL;
    conn->campi_numerus       = 0;
    conn->campus_aedificator  = NIHIL;
    conn->pars_est_filum      = FALSUM;
}

/* Error sentinae: responsum + clausura; lectio corporis desinit */
interior vacuum
_corpus_errare (
           Hospitium* h,
    ConnexioHospitii* conn,
                 i32  status)
{
    h->fructus.petitiones_reiectae++;
    conn->corpus_modus    = CORPUS_NULLUM;
    conn->keep_alive      = FALSUM;
    conn->claudenda_post  = VERUM;
    _respondere_simplex(h, conn, status, FALSUM);
}

/* ---- reditus multipartiti ---- */

interior b32
_mp_initium (
     vacuum* datum,
     chorda  campus,
     chorda  titulus_fili,
        b32  est_filum,
     chorda  mimen)
{
    ConnexioHospitii* conn  = (ConnexioHospitii*)datum;
           Hospitium* h     = conn->hospitium;

    conn->pars_est_filum = est_filum;
    si (est_filum)
    {
             HospitiumFilum* f;
         constans character* via;

        si (conn->fila_numerus >= HOSPITIUM_PARTES_MAXIMAE)
        {
            redde FALSUM;
        }
        via = _via_temporaria(h, conn);
        si (via == NIHIL)
        {
            conn->corpus_status = D;
            redde FALSUM;
        }
        f = &conn->fila[conn->fila_numerus];
        memset(f, 0, magnitudo(*f));
        f->campus                               = campus;
        f->titulus_fili                         = titulus_fili;
        f->mimen                                = mimen;
        f->via                                  = via;
        conn->fila_retenta[conn->fila_numerus]  = FALSUM;
        conn->fila_numerus++;   /* ante aperturam: purgare videt */
        conn->filum_scriptor = filum_scriptor_aperire(via,
            FILUM_MODUS_CREARE, conn->piscina_connexionis);
        si (conn->filum_scriptor == NIHIL)
        {
            conn->corpus_status = D;
            redde FALSUM;
        }
        sigillum_incipere(&conn->filum_sigillum);
        redde VERUM;
    }
    si (conn->campi_numerus >= HOSPITIUM_PARTES_MAXIMAE)
    {
        redde FALSUM;
    }
    conn->campi[conn->campi_numerus].campus  = campus;
    conn->campi[conn->campi_numerus].valor   = _chorda_vacua();
    conn->campi_numerus++;
    conn->campus_aedificator = chorda_aedificator_creare(
        conn->piscina_connexionis, LXIV);
    redde conn->campus_aedificator != NIHIL;
}

interior b32
_mp_data (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    ConnexioHospitii* conn = (ConnexioHospitii*)datum;
                 i32  i;

    si (conn->pars_est_filum)
    {
        HospitiumFilum* f = &conn->fila[conn->fila_numerus - I];

        si (!filum_scriptor_octetos_scribere(conn->filum_scriptor,
            octeti, n))
        {
            conn->corpus_status = D;
            redde FALSUM;
        }
        sigillum_addere(&conn->filum_sigillum, octeti,
            (memoriae_index)n);
        f->mensura += (i64)n;
        redde VERUM;
    }
    per (i = 0; i < n; i++)
    {
        si (!chorda_aedificator_appendere_character(
            conn->campus_aedificator, (character)octeti[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_mp_finis (
    vacuum* datum)
{
    ConnexioHospitii* conn = (ConnexioHospitii*)datum;

    si (conn->pars_est_filum)
    {
        HospitiumFilum* f = &conn->fila[conn->fila_numerus - I];
              Sigillum  s;

        filum_scriptor_claudere(conn->filum_scriptor);
        conn->filum_scriptor  = NIHIL;
        s                     = sigillum_finire(&conn->filum_sigillum);
        sigillum_hex(&s, f->sigillum_hex);
        redde VERUM;
    }
    conn->campi[conn->campi_numerus - I].valor =
        chorda_aedificator_finire(conn->campus_aedificator);
    conn->campus_aedificator = NIHIL;
    redde VERUM;
}

/* Capita completa: sentinam eligere. FALSUM = responsum iam missum
 * (error) - lectio desinit. */
interior b32
_corpus_incipere (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    HttpPetitioServeri* pet =
        http_parser_obtinere_petitio(conn->parser);
                Piscina* p = conn->piscina_connexionis;
                 chorda  typus = _caput_petitionis(pet, "Content-Type");
                 chorda  expectatio = _caput_petitionis(pet, "Expect");

    conn->capita_legens    = FALSUM;   /* corpus: otium solum */
    conn->corpus_reliquum  = (i64)pet->content_length;

    si (_incipit_ignora_casus(typus, "multipart/form-data"))
    {
                     chorda limes =
                         multipartita_limes_ex_typo(typus,
                         p);
        MultipartitaReditus r;

        si (limes.mensura == 0)
        {
            _corpus_errare(h, conn, CD);
            redde FALSUM;
        }
        conn->fila = (HospitiumFilum*)piscina_allocare(p,
            (i64)(HOSPITIUM_PARTES_MAXIMAE
                  * (i32)magnitudo(HospitiumFilum)));
        conn->fila_retenta = (b32*)piscina_allocare(p,
            (i64)(HOSPITIUM_PARTES_MAXIMAE * (i32)magnitudo(b32)));
        conn->campi = (HospitiumCampus*)piscina_allocare(p,
            (i64)(HOSPITIUM_PARTES_MAXIMAE
                  * (i32)magnitudo(HospitiumCampus)));
        r.initium           = _mp_initium;
        r.data              = _mp_data;
        r.finis             = _mp_finis;
        r.datum             = conn;
        conn->multipartita  = multipartita_creare(limes, NIHIL, r, p);
        si (   !conn->fila || !conn->fila_retenta || !conn->campi
            || !conn->multipartita)
        {
            _corpus_errare(h, conn, D);
            redde FALSUM;
        }
        conn->corpus_modus = CORPUS_MULTIPARTITUM;
    }
    alioquin si (conn->corpus_reliquum
                 <= (i64)h->corpus_in_memoria_maximum)
    {
        conn->corpus_memoria = (i8*)piscina_allocare(p,
            conn->corpus_reliquum);
        si (!conn->corpus_memoria)
        {
            _corpus_errare(h, conn, D);
            redde FALSUM;
        }
        conn->corpus_modus = CORPUS_MEMORIA;
    }
    alioquin
    {
        conn->corpus_via = _via_temporaria(h, conn);
        conn->corpus_scriptor = conn->corpus_via
            ? filum_scriptor_aperire(conn->corpus_via,
            FILUM_MODUS_CREARE,
                  p)
            : NIHIL;
        si (!conn->corpus_scriptor)
        {
            _corpus_errare(h, conn, D);
            redde FALSUM;
        }
        conn->corpus_modus = CORPUS_FILUM;
    }

    /* cliens (curl > 1 MB) exspectat: statim 100 */
    si (   pet->versio >= XI
        && _incipit_ignora_casus(expectatio, "100-continue"))
    {
        constans character* centum = "HTTP/1.1 100 Continue\r\n\r\n";

        (vacuum)tcp_mittere(conn->tcp, (constans i8*)centum,
            (i32)strlen(centum));
    }
    redde VERUM;
}

/* Corpus totum acceptum: sentinam claudere, petitionem expedire */
interior vacuum
_corpus_finire (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    HttpPetitioServeri* pet =
        http_parser_obtinere_petitio(conn->parser);

    commutatio (conn->corpus_modus)
    {
        casus CORPUS_MEMORIA:
            pet->corpus.datum      = conn->corpus_memoria;
            pet->corpus.mensura    = conn->corpus_acceptum;
            pet->corpus_longitudo  = conn->corpus_acceptum;
            frange;
        casus CORPUS_FILUM:
            filum_scriptor_claudere(conn->corpus_scriptor);
            conn->corpus_scriptor  = NIHIL;
            pet->corpus_via        = conn->corpus_via;
            frange;
        casus CORPUS_MULTIPARTITUM:
            si (!multipartita_finire(conn->multipartita))
            {
                _corpus_errare(h, conn, CD);
                redde;
            }
            frange;
        casus CORPUS_NULLUM:
            frange;
    }
    conn->corpus_modus = CORPUS_NULLUM;
    _petitionem_expedire(h, conn);
}

/* Octeti corporis ad sentinam (ad Content-Length). VERUM = lege
 * plura; FALSUM = petitio expedita aut error. */
interior b32
_corpus_adicere (
           Hospitium* h,
    ConnexioHospitii* conn,
         constans i8* octeti,
                 i32  n)
{
    i32 m = n;

    si ((i64)m > conn->corpus_reliquum)
    {
        m                         = (i32)conn->corpus_reliquum;
        conn->reliquiae_corporis  = VERUM;   /* pipelining: claudere post */
    }
    commutatio (conn->corpus_modus)
    {
        casus CORPUS_MEMORIA:
            memcpy(conn->corpus_memoria + conn->corpus_acceptum, octeti,
                (size_t)m);
            conn->corpus_acceptum += m;
            frange;
        casus CORPUS_FILUM:
            si (!filum_scriptor_octetos_scribere(conn->corpus_scriptor,
                octeti, m))
            {
                _corpus_errare(h, conn, D);
                redde FALSUM;
            }
            frange;
        casus CORPUS_MULTIPARTITUM:
            si (!multipartita_adicere(conn->multipartita, octeti, m))
            {
                _corpus_errare(h, conn, conn->corpus_status
                    ? conn->corpus_status : (i32)CD);
                redde FALSUM;
            }
            frange;
        casus CORPUS_NULLUM:
            redde FALSUM;
    }
    conn->corpus_reliquum -= (i64)m;
    si (conn->corpus_reliquum == 0)
    {
        _corpus_finire(h, conn);
        redde FALSUM;
    }
    redde VERUM;
}


/* ========================================================================
 * VOCAMINA REACTORIS
 * ======================================================================== */

interior vacuum
_legere (
           Hospitium* h,
    ConnexioHospitii* conn)
{
    i8 buffer[HOSPITIUM_BUFFER_LECTIONIS];

    per (;;)
    {
        HttpParseResultus res;
                      s32 n = tcp_recipere(conn->tcp, buffer,
                          HOSPITIUM_BUFFER_LECTIONIS);

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

        conn->terminus_otii   = tempus_nunc() + h->otium_maximum;
        conn->octeti_accepti  = VERUM;

        si (conn->corpus_modus != CORPUS_NULLUM)
        {
            si (!_corpus_adicere(h, conn, buffer, (i32)n))
            {
                redde;
            }
            perge;
        }

        res = http_parser_adicere(conn->parser,
                                  (constans character*)buffer, (i32)n);

        si (!res.successus)
        {
            h->fructus.petitiones_reiectae++;
            conn->keep_alive      = FALSUM;
            conn->claudenda_post  = VERUM;
            _respondere_simplex(h, conn,
                res.status_suggestus ? res.status_suggestus : (i32)CD,
                FALSUM);
            redde;
        }

        si (res.capita_completa)
        {
            chorda initium;

            si (!_corpus_incipere(h, conn))
            {
                redde;
            }
            initium = http_parser_corpus_initium(conn->parser);
            si (   initium.mensura > 0
                && !_corpus_adicere(h, conn, initium.datum,
                    initium.mensura))
            {
                redde;
            }
            perge;
        }

        si (res.completa)
        {
            _petitionem_expedire(h, conn);
            redde;
        }
    }
}

interior vacuum
_connexio_callback (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    ConnexioHospitii* conn  = (ConnexioHospitii*)data;
           Hospitium* h     = conn->hospitium;

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

    si (conn->status == CONNEXIO_EXSPECTANS)
    {
        /* responsum dilatum: solum abitum clientis quaerere */
         i8 capsa[CCLVI];
        s32 n = tcp_recipere(conn->tcp, capsa, CCLVI);

        si (n == TCP_ITERUM)
        {
            redde;
        }
        si (n <= 0)
        {
            _connexionem_claudere(h, conn);   /* manubrium irritum */
            redde;
        }
        /* octeti ante responsum = pipelining: clausura honesta post */
        conn->keep_alive      = FALSUM;
        conn->claudenda_post  = VERUM;
        conn->ob_reliquias    = VERUM;
        reactor_modificare(h->reactor, conn->fd, 0);
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
_slot_liberum (
    Hospitium* h)
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
_auscultator_callback (
    integer  fd,
        i32  eventus,
     vacuum* data)
{
    Hospitium* h = (Hospitium*)data;

    (vacuum)fd;
    (vacuum)eventus;

    /* percolare usque ad TCP_ERROR_ITERUM */
    per (;;)
    {
                 Piscina* p;
             TcpResultus  acceptum;
        ConnexioHospitii* conn;
                     f64  nunc;

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

        {
            i32 generatio = conn->generatio;

            memset(conn, 0, magnitudo(*conn));
            conn->generatio = generatio + I;
        }
        conn->locus = (i32)(conn - h->connexiones);
        conn->activa = VERUM;
        conn->hospitium = h;
        conn->tcp = acceptum.connexio;
        conn->fd = (integer)tcp_obtinere_fd(acceptum.connexio);
        conn->piscina_connexionis = p;
        conn->basis = piscina_notare(p);   /* TcpConnexio ANTE basim */
        conn->parser = http_parser_creare_cum_limitibus(
            p, h->petitio_maxima, h->uri_maxima);
        http_parser_capita_solum(conn->parser, VERUM);
        conn->status      = CONNEXIO_LEGENS;
        conn->keep_alive  = VERUM;

        nunc                    = tempus_nunc();
        conn->capita_legens     = VERUM;
        conn->terminus_capitum  = nunc + h->capita_maxima;
        conn->terminus_otii     = nunc + h->otium_maximum;

        h->connexiones_numerus++;
        h->fructus.connexiones_acceptae++;

        reactor_adicere(h->reactor, conn->fd, (i32)REACTOR_LEGERE,
                        _connexio_callback, conn);
    }
}

interior vacuum
_vigilia_callback (
    vacuum* data)
{
    Hospitium* h     = (Hospitium*)data;
          f64  nunc  = tempus_nunc();
          i32  i;

    per (i = 0; i < h->connexiones_maximae; i++)
    {
        ConnexioHospitii* conn = &h->connexiones[i];

        si (!conn->activa)
        {
            perge;
        }

        si (conn->status == CONNEXIO_EXSPECTANS)
        {
            si (nunc > conn->terminus_responsi)
            {
                /* nemo respondit: 503, manubrium irritum */
                conn->generatio++;
                conn->dilatum               = FALSUM;
                conn->colloquium.respondit  = VERUM;
                conn->keep_alive            = FALSUM;
                conn->claudenda_post        = VERUM;
                _respondere_simplex(h, conn, DIII,
                    conn->colloquium.est_caput);
            }
            perge;
        }

        si (   (conn->capita_legens && nunc > conn->terminus_capitum)
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
hospitium_creare (
                           Piscina* piscina,
    constans HospitiumConfiguratio* configuratio)
{
                Hospitium* h;
    HospitiumConfiguratio  cfg;
        TcpServusOptiones  opt;
        TcpServusResultus  servus_res;
                  integer  auscultator_fd;

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

    h = (Hospitium*)piscina_allocare(piscina,
        (i64)magnitudo(Hospitium));
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
    h->petitio_maxima  = cfg.petitio_maxima;
    h->uri_maxima      = cfg.uri_maxima;
    h->otium_maximum = (cfg.otium_maximum_ms > 0)
        ? ((f64)cfg.otium_maximum_ms / (f64)M) : (f64)XXX;
    h->capita_maxima = (cfg.capita_maxima_ms > 0)
        ? ((f64)cfg.capita_maxima_ms / (f64)M) : (f64)X;
    h->acao           = cfg.acao;
    h->acta_accessus  = cfg.acta_accessus;
    h->responsum_maximum = (cfg.responsum_maximum_ms > 0)
        ? ((f64)cfg.responsum_maximum_ms / (f64)M) : (f64)XXX;
    h->corpus_in_memoria_maximum = cfg.corpus_in_memoria_maximum
        ? cfg.corpus_in_memoria_maximum : HOSPITIUM_CORPUS_MEMORIA;
    h->directorium_temporarium = _directorium_temporarium(piscina,
        cfg.directorium_temporarium);

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

    h->reactor_proprius  = (cfg.reactor == NIHIL);
    h->reactor           = cfg.reactor ? cfg.reactor
                                       : reactor_creare(piscina);
    h->router            = router_creare(piscina);
    si (!h->reactor || !h->router)
    {
        tcp_servus_claudere(h->servus);
        redde NIHIL;
    }

    auscultator_fd     = tcp_servus_obtinere_fd(h->servus);
    h->auscultator_fd  = auscultator_fd;
    reactor_adicere(h->reactor, auscultator_fd, (i32)REACTOR_LEGERE,
                    _auscultator_callback, h);
    h->vigilia = reactor_timer_adicere(h->reactor, M, VERUM,
        _vigilia_callback, h);

    redde h;
}

vacuum
hospitium_destruere (
    Hospitium* hospitium)
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
            _connexionem_claudere(hospitium,
                &hospitium->connexiones[i]);
        }
    }

    /* reactor communis vivit: registrationes nostras tollere */
    si (!hospitium->molliter)
    {
        reactor_removere(hospitium->reactor, hospitium->auscultator_fd);
    }
    reactor_timer_cancellare(hospitium->reactor, hospitium->vigilia);
    tcp_servus_claudere(hospitium->servus);
    si (hospitium->reactor_proprius)
    {
        reactor_destruere(hospitium->reactor);
    }
}

b32
hospitium_praebere (
             Hospitium* hospitium,
          HttpMethodus  methodus,
    constans character* via,
    HospitiumTractator  tractator,
                vacuum* datum)
{
    CapsaTractatoris* capsa;

    si (!hospitium || !via || !tractator)
    {
        redde FALSUM;
    }

    capsa = (CapsaTractatoris*)piscina_allocare(hospitium->piscina,
        (i64)magnitudo(CapsaTractatoris));
    capsa->tractator  = tractator;
    capsa->datum      = datum;

    redde router_adicere(hospitium->router, methodus, via, capsa);
}

b32
hospitium_directorium_servire (
             Hospitium* hospitium,
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
    ds->directorium = chorda_ex_literis(directorium,
        hospitium->piscina);
    hospitium->directoria_numerus++;

    redde VERUM;
}

/* sistere molliter: connexiones otiosae clauduntur; cum nulla petitio
 * restat (aut terminus praeteriit) omnia clauduntur et sistit */
interior vacuum
_molliter_procedere (
    Hospitium* h)
{
    f64 nunc      = tempus_nunc();
    b32 terminus  = nunc > h->molliter_terminus;
    i32 restant   = 0;
    i32 i;

    per (i = 0; i < h->connexiones_maximae; i++)
    {
        ConnexioHospitii* conn = &h->connexiones[i];

        si (!conn->activa)
        {
            perge;
        }
        si (   terminus
            || (   conn->status == CONNEXIO_LEGENS
                && !conn->octeti_accepti))
        {
            _connexionem_claudere(h, conn);
            perge;
        }
        restant++;
    }
    si (restant == 0)
    {
        h->sistit = VERUM;
    }
}

b32
hospitium_gressus (
    Hospitium* hospitium,
          s32  ms_maximae)
{
    si (!hospitium || hospitium->sistere_petitum || hospitium->sistit)
    {
        redde FALSUM;
    }

    reactor_poll(hospitium->reactor,
                 (ms_maximae > 0) ? (i32)ms_maximae : 0);

    si (hospitium->molliter)
    {
        _molliter_procedere(hospitium);
    }

    redde !hospitium->sistere_petitum && !hospitium->sistit;
}

vacuum
hospitium_currere (
    Hospitium* hospitium)
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
hospitium_sistere (
    Hospitium* hospitium)
{
    si (!hospitium)
    {
        redde;
    }
    hospitium->sistere_petitum = VERUM;
}

i32
hospitium_portus (
    constans Hospitium* hospitium)
{
    si (!hospitium)
    {
        redde 0;
    }
    redde tcp_servus_obtinere_portum(hospitium->servus);
}

HospitiumFructus
hospitium_fructus (
    constans Hospitium* hospitium)
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
colloquium_petitio (
    HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->petitio;
}

chorda
colloquium_param (
    HospitiumColloquium* colloquium,
     constans character* titulus)
{
    si (!colloquium)
    {
        redde _chorda_vacua();
    }
    redde router_param_obtinere(colloquium->params, titulus);
}

Piscina*
colloquium_piscina (
    HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->connexio->piscina_connexionis;
}

vacuum*
colloquium_datum (
    HospitiumColloquium* colloquium)
{
    si (!colloquium)
    {
        redde NIHIL;
    }
    redde colloquium->datum;
}

vacuum
colloquium_caput_addere (
    HospitiumColloquium* colloquium,
     constans character* titulus,
     constans character* valor)
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
colloquium_respondere (
    HospitiumColloquium* colloquium,
                    i32  status,
     constans character* mimen_typus,
                 chorda  corpus)
{
       HttpResponsum  responsum;
           Hospitium* h;
    ConnexioHospitii* conn;
             Piscina* p;

    si (!colloquium || colloquium->respondit)
    {
        redde;  /* uno ictu - vocatio prima vincit */
    }

    h     = colloquium->hospitium;
    conn  = colloquium->connexio;
    p     = conn->piscina_connexionis;

    si (   mimen_typus
        && colloquium->capita_numerus < HOSPITIUM_CAPITA_MAXIMA)
    {
        colloquium->capita[colloquium->capita_numerus].titulus =
            chorda_ex_literis("Content-Type", p);
        colloquium->capita[colloquium->capita_numerus].valor =
            chorda_ex_literis(mimen_typus, p);
        colloquium->capita_numerus++;
    }

    colloquium->respondit  = VERUM;
    conn->dilatum          = FALSUM;

    memset(&responsum, 0, magnitudo(responsum));
    responsum.status          = status;
    responsum.capita          = colloquium->capita;
    responsum.capita_numerus  = colloquium->capita_numerus;
    responsum.corpus          = corpus;

    _responsum_mittere(h, conn, &responsum, colloquium->est_caput);
}

Reactor*
hospitium_reactor (
    Hospitium* hospitium)
{
    redde hospitium ? hospitium->reactor : NIHIL;
}

vacuum
hospitium_sistere_molliter (
    Hospitium* hospitium,
          i32  mora_ms)
{
    si (!hospitium || hospitium->molliter)
    {
        redde;
    }
    reactor_removere(hospitium->reactor, hospitium->auscultator_fd);
    tcp_servus_claudere(hospitium->servus);
    hospitium->molliter           = VERUM;
    hospitium->molliter_terminus  = tempus_nunc()
        + (f64)mora_ms / (f64)M;
}

b32
hospitium_sistit (
    constans Hospitium* hospitium)
{
    redde hospitium ? hospitium->sistit : FALSUM;
}

vacuum
colloquium_respondere_fontem (
     HospitiumColloquium* colloquium,
                     i32  status,
      constans character* mimen_typus,
           HospitiumFons  fons)
{
    HttpResponsum  responsum;
          Piscina* p;

    si (!colloquium || colloquium->respondit)
    {
        si (fons.claudere)
        {
            fons.claudere(fons.datum);
        }
        redde;
    }
    p = colloquium->connexio->piscina_connexionis;
    si (   mimen_typus
        && colloquium->capita_numerus < HOSPITIUM_CAPITA_MAXIMA)
    {
        colloquium->capita[colloquium->capita_numerus].titulus =
            chorda_ex_literis("Content-Type", p);
        colloquium->capita[colloquium->capita_numerus].valor =
            chorda_ex_literis(mimen_typus, p);
        colloquium->capita_numerus++;
    }
    colloquium->respondit          = VERUM;
    colloquium->connexio->dilatum  = FALSUM;
    memset(&responsum, 0, magnitudo(responsum));
    responsum.status          = status;
    responsum.capita          = colloquium->capita;
    responsum.capita_numerus  = colloquium->capita_numerus;
    _fontem_mittere(colloquium->hospitium, colloquium->connexio,
        &responsum, fons, colloquium->est_caput);
}

b32
colloquium_respondere_filum (
    HospitiumColloquium* colloquium,
                    i32  status,
     constans character* mimen_typus,
     constans character* via)
{
    HospitiumFons fons;

    si (!colloquium || colloquium->respondit || !via)
    {
        redde FALSUM;
    }
    si (!_fontem_fili_parare(via,
        colloquium->connexio->piscina_connexionis, &fons))
    {
        colloquium_respondere(colloquium, CDIV, NIHIL, _chorda_vacua());
        redde FALSUM;
    }
    colloquium_respondere_fontem(colloquium, status, mimen_typus, fons);
    redde VERUM;
}

HospitiumDilatum
colloquium_differre (
    HospitiumColloquium* colloquium)
{
    HospitiumDilatum d;

    memset(&d, 0, magnitudo(d));
    si (!colloquium || colloquium->respondit)
    {
        redde d;
    }
    colloquium->differtur  = VERUM;
    d.hospitium            = colloquium->hospitium;
    d.locus                = colloquium->connexio->locus;
    d.generatio            = colloquium->connexio->generatio;
    redde d;
}

b32
dilatum_valet (
    HospitiumDilatum dilatum)
{
    ConnexioHospitii* conn;

    si (   !dilatum.hospitium
        || dilatum.locus >= dilatum.hospitium->connexiones_maximae)
    {
        redde FALSUM;
    }
    conn = &dilatum.hospitium->connexiones[dilatum.locus];
    redde conn->activa && conn->dilatum
        && conn->generatio == dilatum.generatio
        && !conn->colloquium.respondit;
}

HospitiumColloquium*
dilatum_colloquium (
    HospitiumDilatum dilatum)
{
    redde dilatum_valet(dilatum)
        ? &dilatum.hospitium->connexiones[dilatum.locus].colloquium
        : NIHIL;
}

i32
colloquium_fila (
         HospitiumColloquium*  colloquium,
     constans HospitiumFilum** fila)
{
    si (fila)
    {
        *fila = NIHIL;
    }
    si (!colloquium || !colloquium->connexio)
    {
        redde 0;
    }
    si (fila)
    {
        *fila = colloquium->connexio->fila;
    }
    redde colloquium->connexio->fila_numerus;
}

b32
colloquium_filum_retinere (
        HospitiumColloquium* colloquium,
    constans HospitiumFilum* filum,
         constans character* via_nova)
{
    ConnexioHospitii* conn;
                 s64  index;
           character* copia;

    si (!colloquium || !filum || !via_nova || !colloquium->connexio)
    {
        redde FALSUM;
    }
    conn   = colloquium->connexio;
    si (!conn->fila)
    {
        redde FALSUM;
    }
    index  = (s64)(filum - conn->fila);
    si (index < 0 || index >= (s64)conn->fila_numerus)
    {
        redde FALSUM;
    }
    si (!filum_movere(conn->fila[index].via, via_nova))
    {
        redde FALSUM;
    }
    copia = (character*)piscina_allocare(conn->piscina_connexionis,
        (i64)(strlen(via_nova) + I));
    strcpy(copia, via_nova);
    conn->fila[index].via      = copia;
    conn->fila_retenta[index]  = VERUM;
    redde VERUM;
}

i32
colloquium_campi (
          HospitiumColloquium*  colloquium,
     constans HospitiumCampus** campi)
{
    si (campi)
    {
        *campi = NIHIL;
    }
    si (!colloquium || !colloquium->connexio)
    {
        redde 0;
    }
    si (campi)
    {
        *campi = colloquium->connexio->campi;
    }
    redde colloquium->connexio->campi_numerus;
}
