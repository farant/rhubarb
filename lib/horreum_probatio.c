/* horreum_probatio.c - probatio viva provisoris (vide horreum_probatio.h;
 * horreum-plan T6)
 *
 * Gradus ordine fixo, quisque lineam unam reddit; nullus gradus alium
 * sistit (relatio tota semper). Opera per aedificatores nucleii et
 * horreum_exsequi (iterationes T3) ut responsa cruda servari possint;
 * praesignatae per vecturam ipsam sine Authorization. Purgatio ultima:
 * enumeratio sub praefixo -> delere (plura, aut singula si plura
 * deficit), onera pendentia -> abicere, deinde numeratio relictorum. */
#include "latina.h"
#include "horreum_probatio.h"
#include "horreum_cliens.h"
#include "chorda_aedificator.h"
#include "base64.h"
#include "filum.h"

#include <string.h>
#include <time.h>

#define ETAG_VACUI "d41d8cd98f00b204e9800998ecf8427e"

nomen structura {
                     Horreum* h;
                      chorda  situla;
                      chorda  radix;        /* praefixum + id + "/" */
          constans character* responsa;
                 HttpVectura  vectura;
             HorreumProbatio* r;
                     Piscina* p;
} Probans;

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior chorda
_clavis (
                Probans* pr,
     constans character* nomen_relativum)
{
    redde chorda_concatenare(pr->radix, _l(nomen_relativum, pr->p),
        pr->p);
}

interior vacuum
_linea (
                Probans* pr,
     constans character* titulus,
                    i32  capacitas,
       HorreumVerdictum  verdictum,
                 chorda  causa)
{
    HorreumProbatioLinea* l;

    si (pr->r->numerus >= HORREUM_PROBATIO_LINEAE)
    {
        redde;
    }
    l             = &pr->r->lineae[pr->r->numerus];
    l->titulus    = titulus;
    l->capacitas  = capacitas;
    l->verdictum  = verdictum;
    l->causa      = causa;
    pr->r->numerus++;
}

/* "gradus: codex (status) nuntius" */
interior chorda
_causa (
                    Probans* pr,
         constans character* gradus,
      constans HorreumError* e)
{
    ChordaAedificator* a = chorda_aedificator_creare(pr->p, CXXVIII);

    chorda_aedificator_appendere_literis(a, gradus);
    chorda_aedificator_appendere_literis(a, ": ");
    si (e->codex.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, e->codex);
        chorda_aedificator_appendere_literis(a, " ");
    }
    si (e->status > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "(");
        chorda_aedificator_appendere_i32(a, e->status);
        chorda_aedificator_appendere_literis(a, ") ");
    }
    chorda_aedificator_appendere_chorda(a, e->nuntius);
    redde chorda_aedificator_finire(a);
}

interior vacuum
_caput_petitionis (
          Probans* pr,
    HttpResponsum* r)
{
    constans character* nomina[] = { "x-amz-request-id", "cf-ray",
        "x-request-id", "x-amz-id-2" };
    i32 i;

    si (!r || pr->r->caput_petitionis.mensura > ZEPHYRUM)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        si (http_responsum_caput(r, nomina[i]).mensura > ZEPHYRUM)
        {
            pr->r->caput_petitionis = _l(nomina[i], pr->p);
            redde;
        }
    }
}

interior vacuum
_servare (
                Probans* pr,
     constans character* nomen_fili,
                 chorda  corpus)
{
    chorda via;

    si (!pr->responsa || !nomen_fili || corpus.mensura == ZEPHYRUM)
    {
        redde;
    }
    via = chorda_concatenare(_l(pr->responsa, pr->p), _l("/", pr->p),
        pr->p);
    via = chorda_concatenare(via, _l(nomen_fili, pr->p), pr->p);
    (vacuum)filum_scribere(chorda_ut_cstr(via, pr->p),
        horreum_responsum_purgare(corpus, pr->p));
}

/* exsequi cum responso servato (si nomen) et capite id notato */
interior b32
_exsequi (
                 Probans* pr,
    constans HorreumOpus* o,
           HorreumExitus* x,
         HorreumObjectum* ob,
            HorreumError* e,
      constans character* nomen_fili)
{
    HorreumExitus x_locus;
              b32 bene;

    si (!x)
    {
        x = &x_locus;
    }
    bene = horreum_exsequi(pr->h, o, x, ob, e, pr->p);
    _caput_petitionis(pr, x->responsum);
    si (x->responsum)
    {
        _servare(pr, nomen_fili, x->responsum->corpus);
    }
    redde bene;
}

interior b32
_ponere (
                    Probans* pr,
                     chorda  clavis,
                     chorda  corpus,
    constans HorreumPonendi* ponendi,
            HorreumObjectum* ob,
               HorreumError* e,
         constans character* nomen_fili)
{
    HorreumOpus o;

    redde horreum_opus_ponere(pr->h, pr->situla, clavis, corpus,
        ponendi,
        &o, e, pr->p)
        && _exsequi(pr, &o, NIHIL, ob, e, nomen_fili);
}

interior b32
_inspicere (
            Probans* pr,
             chorda  clavis,
    HorreumObjectum* ob,
       HorreumError* e)
{
    HorreumOpus o;

    redde horreum_opus_inspicere(pr->h, pr->situla, clavis, &o, e,
        pr->p)
        && _exsequi(pr, &o, NIHIL, ob, e, NIHIL);
}

interior b32
_capere (
                Probans* pr,
                 chorda  clavis,
                 chorda* corpus,
           HorreumError* e,
     constans character* nomen_fili)
{
      HorreumOpus o;
    HorreumExitus x;
  HorreumObjectum ob;

    *corpus = chorda_ex_buffer(NIHIL, ZEPHYRUM);
    si (   !horreum_opus_capere(pr->h, pr->situla, clavis, NIHIL, &o, e,
                pr->p)
        || !_exsequi(pr, &o, &x, &ob, e, nomen_fili))
    {
        redde FALSUM;
    }
    *corpus = x.responsum ? x.responsum->corpus
                          : chorda_ex_buffer(NIHIL, ZEPHYRUM);
    redde VERUM;
}

interior b32
_delere (
         Probans* pr,
          chorda  clavis,
    HorreumError* e)
{
    HorreumOpus o;

    redde horreum_opus_delere(pr->h, pr->situla, clavis, &o, e, pr->p)
        && _exsequi(pr, &o, NIHIL, NIHIL, e, NIHIL);
}

/* HEAD -> ABEST? */
interior b32
_abest (
    Probans* pr,
     chorda  clavis)
{
    HorreumObjectum ob;
       HorreumError e;

    redde !_inspicere(pr, clavis, &ob, &e) && e.genus == HORREUM_ABEST;
}

/* ===== iter ===== */
interior vacuum
_gradus_iter (
    Probans* pr)
{
        Piscina* p      = pr->p;
         chorda  mira   = _clavis(pr, "a b+c%d?e#f&g/\xE1\x88\xB4//x/");
         chorda  vacua  = _clavis(pr, "vacuum");
         chorda  corpus;
         chorda  etag;
HorreumObjectum  ob;
   HorreumError  e;

    pr->r->clavis_difficilis = mira;
    si (!_ponere(pr, mira, _l("salve", p), NIHIL, &ob, &e, NIHIL))
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "PUT clavis difficilis", &e));
        redde;
    }
    etag = ob.etag;
    si (   !_capere(pr, mira, &corpus, &e, NIHIL)
        || !chorda_aequalis_literis(corpus, "salve"))
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "GET clavis difficilis", &e));
        redde;
    }
    si (   !_inspicere(pr, mira, &ob, &e) || ob.mensura != V
        || !chorda_aequalis(ob.etag, etag))
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "HEAD clavis difficilis", &e));
        redde;
    }
    si (   !_ponere(pr, vacua, chorda_ex_buffer(NIHIL, ZEPHYRUM), NIHIL,
                &ob, &e, NIHIL)
        || !_capere(pr, vacua, &corpus, &e, NIHIL)
        || corpus.mensura != ZEPHYRUM
        || !_inspicere(pr, vacua, &ob, &e) || ob.mensura != ZEPHYRUM
        || !chorda_aequalis_literis(ob.etag, ETAG_VACUI))
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "obiectum vacuum", &e));
        redde;
    }
    si (!_delere(pr, vacua, &e) || !_abest(pr, vacua))
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "DELETE", &e));
        redde;
    }
    /* responsum 404 servatur */
    (vacuum)_capere(pr, _clavis(pr, "nusquam"), &corpus, &e,
        "error_abest.xml");
    si (e.genus != HORREUM_ABEST)
    {
        _linea(pr, "iter", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "GET absentis", &e));
        redde;
    }
    _linea(pr, "iter", ZEPHYRUM, HORREUM_PROBATUM, _l(
        "PUT/GET/HEAD/DELETE: clavis difficilis et obiectum vacuum",
        p));
}

/* ===== condiciones ===== */
interior vacuum
_gradus_solum_si_abest (
    Probans* pr)
{
        Piscina* p       = pr->p;
         chorda  clavis  = _clavis(pr, "conditio");
 HorreumPonendi  ponendi;
HorreumObjectum  ob;
   HorreumError  e;

    si (!_ponere(pr, clavis, _l("primum", p), NIHIL, &ob, &e, NIHIL))
    {
        _linea(pr, "solum_si_abest", HORREUM_CAP_SOLUM_SI_ABEST,
            HORREUM_NON_PROBATUM, _causa(pr, "PUT primum", &e));
        redde;
    }
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest = VERUM;
    si (_ponere(pr, clavis, _l("secundum", p), &ponendi, &ob, &e,
            "error_conditio.xml"))
    {
        _linea(pr, "solum_si_abest", HORREUM_CAP_SOLUM_SI_ABEST,
            HORREUM_REFUTATUM, _l(
            "If-None-Match: * neglectum - obiectum exsistens superscriptum",
            p));
        redde;
    }
    si (e.genus != HORREUM_CONDITIO)
    {
        _linea(pr, "solum_si_abest", HORREUM_CAP_SOLUM_SI_ABEST,
            HORREUM_REFUTATUM, _causa(pr, "If-None-Match: * (adest)",
            &e));
        redde;
    }
    si (!_ponere(pr, _clavis(pr, "absens"), _l("novum", p), &ponendi,
            &ob, &e, NIHIL))
    {
        _linea(pr, "solum_si_abest", HORREUM_CAP_SOLUM_SI_ABEST,
            HORREUM_REFUTATUM, _causa(pr, "If-None-Match: * (abest)",
            &e));
        redde;
    }
    _linea(pr, "solum_si_abest", HORREUM_CAP_SOLUM_SI_ABEST,
        HORREUM_PROBATUM, _l("412 si adest, 200 si abest", p));
}

interior vacuum
_gradus_solum_si_etag (
    Probans* pr)
{
        Piscina* p       = pr->p;
         chorda  clavis  = _clavis(pr, "conditio");
 HorreumPonendi  ponendi;
HorreumObjectum  ob;
   HorreumError  e;

    si (!_inspicere(pr, clavis, &ob, &e))
    {
        _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
            HORREUM_NON_PROBATUM, _causa(pr, "HEAD", &e));
        redde;
    }
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_etag = _l("00000000000000000000000000000000", p);
    si (_ponere(pr, clavis, _l("tertium", p), &ponendi, &ob, &e, NIHIL))
    {
        _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
            HORREUM_REFUTATUM, _l(
            "If-Match falsum neglectum - obiectum superscriptum", p));
        redde;
    }
    si (e.genus != HORREUM_CONDITIO)
    {
        _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
            HORREUM_REFUTATUM, _causa(pr, "If-Match falsum", &e));
        redde;
    }
    si (!_inspicere(pr, clavis, &ob, &e))
    {
        _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
            HORREUM_NON_PROBATUM, _causa(pr, "HEAD", &e));
        redde;
    }
    ponendi.solum_si_etag = ob.etag;
    si (!_ponere(pr, clavis, _l("quartum", p), &ponendi, &ob, &e,
        NIHIL))
    {
        _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
            HORREUM_REFUTATUM, _causa(pr, "If-Match verum", &e));
        redde;
    }
    _linea(pr, "solum_si_etag", HORREUM_CAP_SOLUM_SI_ETAG,
        HORREUM_PROBATUM, _l("412 si ETag falsum, 200 si verum", p));
}

interior b32
_petitio_url (
          Probans* pr,
     HttpMethodus  methodus,
           chorda  url,
           chorda  corpus,
           chorda* reddita,
     HorreumError* e);

interior chorda
_url_praesignata (
          Probans* pr,
     HttpMethodus  methodus,
           chorda  clavis,
     HorreumError* e);

/* ===== acl ===== */
interior vacuum
_gradus_acl (
    Probans* pr)
{
        Piscina* p       = pr->p;
         chorda  clavis  = _clavis(pr, "acl");
 HorreumPonendi  ponendi;
HorreumObjectum  ob;
   HorreumError  e;
   HorreumError  e_delendi;
         chorda  url;
         chorda  corpus;
            s32  signum;
            b32  publicum;

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.acl = HORREUM_ACL_PUBLICA_LECTIO;
    si (!_ponere(pr, clavis, _l("acl", p), &ponendi, &ob, &e, NIHIL))
    {
        _linea(pr, "acl", HORREUM_CAP_ACL, HORREUM_REFUTATUM, _causa(pr,
            "x-amz-acl public-read", &e));
        redde;
    }
    /* effectus: GET per URL nudam (praesignata sine quaestione) */
    url     = _url_praesignata(pr, HTTP_GET, clavis, &e);
    signum  = chorda_invenire_index(url, _l("?", p));
    si (signum > ZEPHYRUM)
    {
        url = chorda_sectio(url, ZEPHYRUM, (i32)signum);
    }
    publicum = url.mensura > ZEPHYRUM
        && _petitio_url(pr, HTTP_GET, url, chorda_ex_buffer(NIHIL,
            ZEPHYRUM), &corpus, &e)
        && chorda_aequalis_literis(corpus, "acl");
    /* obiectum publicum statim deletur */
    (vacuum)_delere(pr, clavis, &e_delendi);
    si (!publicum)
    {
        _linea(pr, "acl", HORREUM_CAP_ACL, HORREUM_REFUTATUM, _causa(pr,
            "x-amz-acl public-read acceptum sed GET sine signatura",
            &e));
        redde;
    }
    _linea(pr, "acl", HORREUM_CAP_ACL, HORREUM_PROBATUM, _l(
        "public-read: GET sine signatura legit", p));
}

/* ===== summae ===== */
interior SignaturaPar*
_caput_operis (
           HorreumOpus* o,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->capita_numerus; i++)
    {
        si (chorda_aequalis_literis(o->capita[i].titulus, titulus))
        {
            redde &o->capita[i];
        }
    }
    redde NIHIL;
}

interior vacuum
_gradus_summa (
                Probans* pr,
     constans character* titulus,
                    i32  capacitas,
        HorreumChecksum  genus,
     constans character* caput)
{
          Piscina* p       = pr->p;
           chorda  clavis  = _clavis(pr, titulus);
   HorreumPonendi  ponendi;
      HorreumOpus  o;
  HorreumObjectum  ob;
     HorreumError  e;
     SignaturaPar* c;
           chorda  missa;
           chorda  reddita;
               i8  nulla[XXXII];
ChordaAedificator* a;

    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.checksum = genus;
    si (!horreum_opus_ponere(pr->h, pr->situla, clavis, _l("summa", p),
            &ponendi, &o, &e, p))
    {
        _linea(pr, titulus, capacitas, HORREUM_NON_PROBATUM, _causa(pr,
            "aedificare", &e));
        redde;
    }
    c      = _caput_operis(&o, caput);
    missa  = c ? c->valor : chorda_ex_buffer(NIHIL, ZEPHYRUM);
    si (!_exsequi(pr, &o, NIHIL, &ob, &e, NIHIL))
    {
        _linea(pr, titulus, capacitas, HORREUM_REFUTATUM, _causa(pr,
            "summa vera", &e));
        redde;
    }
    reddita = genus == HORREUM_CHECKSUM_CRC32 ? ob.checksum_crc32
                                              : ob.checksum_sha256;
    /* summa FALSA: recusari debet */
    si (   !horreum_opus_ponere(pr->h, pr->situla, clavis, _l("summa",
        p),
                &ponendi, &o, &e, p)
        || !(c = _caput_operis(&o, caput)))
    {
        _linea(pr, titulus, capacitas, HORREUM_NON_PROBATUM, _causa(pr,
            "aedificare", &e));
        redde;
    }
    memset(nulla, ZEPHYRUM, magnitudo(nulla));
    c->valor = base64_codificare(nulla, genus == HORREUM_CHECKSUM_CRC32
        ? IV : XXXII, p);
    si (chorda_aequalis(c->valor, missa))
    {
        nulla[ZEPHYRUM] = I;
        c->valor = base64_codificare(nulla, genus
            == HORREUM_CHECKSUM_CRC32
            ? IV : XXXII, p);
    }
    si (_exsequi(pr, &o, NIHIL, &ob, &e, NIHIL))
    {
        _linea(pr, titulus, capacitas, HORREUM_REFUTATUM, _l(
            "summa falsa accepta - caput neglectum", p));
        redde;
    }
    si (e.genus == HORREUM_TRANSITORIUM)
    {
        _linea(pr, titulus, capacitas, HORREUM_NON_PROBATUM, _causa(pr,
            "summa falsa", &e));
        redde;
    }
    a = chorda_aedificator_creare(p, CXXVIII);
    chorda_aedificator_appendere_literis(a, "vera accepta (");
    chorda_aedificator_appendere_literis(a, chorda_aequalis(reddita,
        missa)
        ? "reddita" : "non reddita");
    chorda_aedificator_appendere_literis(a, "), falsa recusata (");
    chorda_aedificator_appendere_chorda(a, e.codex);
    chorda_aedificator_appendere_literis(a, ")");
    _linea(pr, titulus, capacitas, HORREUM_PROBATUM,
        chorda_aedificator_finire(a));
}

/* ===== delere_plura ===== */
interior vacuum
_gradus_delere_plura (
    Probans* pr)
{
        Piscina* p = pr->p;
         chorda  claves[II];
    HorreumOpus  o;
  HorreumExitus  x;
HorreumObjectum  ob;
   HorreumError  e;
 HorreumDeletio* exitus;
            i32  numerus;
            i32  i;

    claves[ZEPHYRUM]  = _clavis(pr, "plura/unum");
    claves[I]         = _clavis(pr, "plura/duo");
    per (i = ZEPHYRUM; i < II; i++)
    {
        si (!_ponere(pr, claves[i], _l("plura", p), NIHIL, &ob, &e,
            NIHIL))
        {
            _linea(pr, "delere_plura", HORREUM_CAP_DELERE_PLURA,
                HORREUM_NON_PROBATUM, _causa(pr, "PUT", &e));
            redde;
        }
    }
    si (   !horreum_opus_delere_plura(pr->h, pr->situla, claves, II, &o,
        &e,
                p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, "delere_plura.xml")
        || !horreum_deletiones_legere(x.responsum->corpus, &exitus,
            &numerus, &e, p))
    {
        _linea(pr, "delere_plura", HORREUM_CAP_DELERE_PLURA,
            HORREUM_REFUTATUM, _causa(pr, "DeleteObjects", &e));
        redde;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!exitus[i].deletum)
        {
            _linea(pr, "delere_plura", HORREUM_CAP_DELERE_PLURA,
                HORREUM_REFUTATUM, _causa(pr, "clavis non deleta",
                &exitus[i].error));
            redde;
        }
    }
    si (   numerus != II || !_abest(pr, claves[ZEPHYRUM])
        || !_abest(pr, claves[I]))
    {
        _linea(pr, "delere_plura", HORREUM_CAP_DELERE_PLURA,
            HORREUM_REFUTATUM, _l("claves manent post DeleteObjects",
            p));
        redde;
    }
    _linea(pr, "delere_plura", HORREUM_CAP_DELERE_PLURA,
        HORREUM_PROBATUM,
        _l("DeleteObjects cum Content-MD5: II deletae", p));
}

/* ===== enumerare ===== */
interior b32
_praefixum_adest (
    constans HorreumPagina* pg,
                    chorda  quaesitum)
{
    i32 i;

    per (i = ZEPHYRUM; i < pg->praefixa_numerus; i++)
    {
        si (chorda_aequalis(pg->praefixa_communia[i], quaesitum))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_gradus_enumerare (
    Probans* pr)
{
          Piscina* p = pr->p;
      HorreumOpus  o;
    HorreumExitus  x;
  HorreumObjectum  ob;
     HorreumError  e;
HorreumEnumerandi  en;
    HorreumPagina  pg;

    si (   !_ponere(pr, _clavis(pr, "index/unum.txt"), _l("i", p),
        NIHIL,
                &ob, &e, NIHIL)
        || !_ponere(pr, _clavis(pr, "index/duo.txt"), _l("ii", p),
        NIHIL,
            &ob, &e, NIHIL))
    {
        _linea(pr, "enumerare", ZEPHYRUM, HORREUM_NON_PROBATUM,
            _causa(pr,
            "PUT", &e));
        redde;
    }
    memset(&en, ZEPHYRUM, magnitudo(en));
    en.praefixum    = pr->radix;
    en.delimitator  = _l("/", p);
    si (   !horreum_opus_enumerare(pr->h, pr->situla, &en, &o, &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, "enumerare.xml")
        || !horreum_paginam_legere(x.responsum->corpus, &pg, &e, p))
    {
        _linea(pr, "enumerare", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "ListObjectsV2", &e));
        redde;
    }
    si (   !_praefixum_adest(&pg, _clavis(pr, "index/"))
        || !_praefixum_adest(&pg, _clavis(pr, "a b+c%d?e#f&g/")))
    {
        _linea(pr, "enumerare", ZEPHYRUM, HORREUM_REFUTATUM, _l(
            "praefixa communia desunt (delimitator)", p));
        redde;
    }
    memset(&en, ZEPHYRUM, magnitudo(en));
    en.praefixum = _clavis(pr, "a b+c");
    si (   !horreum_opus_enumerare(pr->h, pr->situla, &en, &o, &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, NIHIL)
        || !horreum_paginam_legere(x.responsum->corpus, &pg, &e, p))
    {
        _linea(pr, "enumerare", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "ListObjectsV2 praefixi difficilis", &e));
        redde;
    }
    si (   pg.obiecta_numerus != I
        || !chorda_aequalis(pg.obiecta[ZEPHYRUM].clavis,
            pr->r->clavis_difficilis))
    {
        _linea(pr, "enumerare", ZEPHYRUM, HORREUM_REFUTATUM, _l(
            "clavis difficilis non eadem in enumeratione", p));
        redde;
    }
    _linea(pr, "enumerare", ZEPHYRUM, HORREUM_PROBATUM, _l(
        "delimitator / et clavis difficilis integra", p));
}

/* ===== multipars ===== */
/* onus pendens per CLAVEM quaeritur: R2 in ListMultipartUploads id
 * ALIUM reddit quam CreateMultipartUpload (utrumque abicere valet) */
interior b32
_pendens (
    constans HorreumOnusPendens* onera,
                            i32  numerus,
                         chorda  clavis)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (chorda_aequalis(onera[i].clavis, clavis))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_pendentes (
                Probans*  pr,
     HorreumOnusPendens** onera,
                    i32*  numerus,
           HorreumError*  e,
     constans character*  nomen_fili)
{
      HorreumOpus o;
    HorreumExitus x;

    redde horreum_opus_mp_pendentes(pr->h, pr->situla, pr->radix, &o, e,
        pr->p)
        && _exsequi(pr, &o, &x, NIHIL, e, nomen_fili)
        && horreum_pendentes_legere(x.responsum->corpus, onera, numerus,
        e,
            pr->p);
}

interior vacuum
_gradus_partium (
    Probans* pr)
{
           Piscina* p       = pr->p;
            chorda  clavis  = _clavis(pr, "multipars");
            chorda  id;
            chorda  id_abiciendi;
       HorreumOpus  o;
     HorreumExitus  x;
   HorreumObjectum  ob;
      HorreumError  e;
       HorreumPars  partes[III];
       HorreumPars* lectae;
HorreumOnusPendens* onera;
               i32  numerus;
               i32  n;
                i8* d;

    si (   !horreum_opus_mp_incipere(pr->h, pr->situla, clavis, NIHIL,
        &o,
                &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, "mp_incipere.xml")
        || !horreum_id_oneris_legere(x.responsum->corpus, &id, &e, p))
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "CreateMultipartUpload", &e));
        redde;
    }
    per (n = ZEPHYRUM; n < III; n++)
    {
        d = (i8*)piscina_allocare(p,
            (memoriae_index)HORREUM_PARS_MINIMA);
        memset(d, (integer)n + 'a',
            (memoriae_index)HORREUM_PARS_MINIMA);
        si (   !horreum_opus_mp_pars(pr->h, pr->situla, clavis, id, n
            + I,
                    chorda_ex_buffer(d, HORREUM_PARS_MINIMA), &o, &e, p)
            || !_exsequi(pr, &o, NIHIL, &ob, &e, NIHIL))
        {
            _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM,
                _causa(pr,
                "UploadPart", &e));
            redde;
        }
        partes[n].numerus  = n + I;
        partes[n].etag     = ob.etag;
        partes[n].mensura  = (i64)HORREUM_PARS_MINIMA;
    }
    si (   !horreum_opus_mp_partes(pr->h, pr->situla, clavis, id, &o,
        &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, "mp_partes.xml")
        || !horreum_partes_legere(x.responsum->corpus, &lectae,
        &numerus,
            &e, p)
        || numerus != III)
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "ListParts", &e));
        redde;
    }
    si (   !_pendentes(pr, &onera, &numerus, &e, "mp_pendentes.xml")
        || !_pendens(onera, numerus, clavis))
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "ListMultipartUploads", &e));
        redde;
    }
    si (   !horreum_opus_mp_perficere(pr->h, pr->situla, clavis, id,
        partes,
                III, &o, &e, p)
        || !_exsequi(pr, &o, NIHIL, &ob, &e, "mp_perficere.xml")
        || !chorda_terminatur(ob.etag, _l("-3", p)))
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "CompleteMultipartUpload", &e));
        redde;
    }
    si (   !_inspicere(pr, clavis, &ob, &e)
        || ob.mensura != (i64)III * (i64)HORREUM_PARS_MINIMA)
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "HEAD post Complete", &e));
        redde;
    }
    /* abortus */
    si (   !horreum_opus_mp_incipere(pr->h, pr->situla,
                _clavis(pr, "abiciendum"), NIHIL, &o, &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, NIHIL)
        || !horreum_id_oneris_legere(x.responsum->corpus, &id_abiciendi,
        &e,
            p)
        || !horreum_opus_mp_abicere(pr->h, pr->situla,
            _clavis(pr, "abiciendum"), id_abiciendi, &o, &e, p)
        || !_exsequi(pr, &o, NIHIL, NIHIL, &e, NIHIL)
        || !_pendentes(pr, &onera, &numerus, &e, NIHIL)
        || _pendens(onera, numerus, _clavis(pr, "abiciendum"))
        || _pendens(onera, numerus, clavis))
    {
        _linea(pr, "multipars", ZEPHYRUM, HORREUM_REFUTATUM, _causa(pr,
            "AbortMultipartUpload", &e));
        redde;
    }
    _linea(pr, "multipars", ZEPHYRUM, HORREUM_PROBATUM, _l(
        "III x V MiB, ETag -3, ListParts, pendentes, abortus", p));
}

/* ===== petitio per URL (sine Authorization) ===== */
interior b32
_petitio_url (
          Probans* pr,
     HttpMethodus  methodus,
           chorda  url,
           chorda  corpus,
           chorda* reddita,
     HorreumError* e)
{
          Piscina* p = pr->p;
      HttpPetitio* pet;
     HttpResultus  res;
ChordaAedificator* a;
              i32  portus;

    pet = http_petitio_creare(p, methodus, chorda_ut_cstr(url, p));
    si (!pet)
    {
        redde FALSUM;
    }
    /* Host ut in filo (vectura simulacri capita sola videt) */
    a       = chorda_aedificator_creare(p, CXXVIII);
    portus  = http_petitio_portus(pet);
    chorda_aedificator_appendere_literis(a, http_petitio_hospes(pet));
    si (   (http_petitio_est_https(pet) && portus != CDXLIII)
        || (!http_petitio_est_https(pet) && portus != LXXX))
    {
        chorda_aedificator_appendere_character(a, ':');
        chorda_aedificator_appendere_i32(a, portus);
    }
    http_petitio_caput_addere(pet, "Host", chorda_ut_cstr(
        chorda_aedificator_finire(a), p));
    si (corpus.mensura > ZEPHYRUM)
    {
        http_petitio_corpus_ponere_chorda(pet, corpus);
    }
    res = http_vectura_exsequi(pr->vectura, pet, p);
    memset(e, ZEPHYRUM, magnitudo(*e));
    si (!res.successus || !res.responsum)
    {
        e->genus    = HORREUM_TRANSITORIUM;
        e->nuntius  = res.error_descriptio;
        redde FALSUM;
    }
    e->status = res.responsum->status;
    si (res.responsum->status < CC || res.responsum->status >= CCC)
    {
        e->genus    = HORREUM_MALUM;
        e->nuntius  = _l("status non 2xx", p);
        redde FALSUM;
    }
    si (reddita)
    {
        *reddita = res.responsum->corpus;
    }
    redde VERUM;
}

/* URL praesignata (CCC s) */
interior chorda
_url_praesignata (
          Probans* pr,
     HttpMethodus  methodus,
           chorda  clavis,
     HorreumError* e)
{
    redde horreum_praesignare(pr->h, methodus, pr->situla, clavis, CCC,
        chorda_ex_buffer(NIHIL, ZEPHYRUM), (MomentumSec)time(NIHIL), e,
        pr->p);
}

/* ===== praesignare ===== */
interior b32
_praesignatum (
          Probans* pr,
     HttpMethodus  methodus,
           chorda  clavis,
           chorda  corpus,
           chorda* reddita,
     HorreumError* e)
{
    chorda url = _url_praesignata(pr, methodus, clavis, e);

    redde url.mensura > ZEPHYRUM
        && _petitio_url(pr, methodus, url, corpus, reddita, e);
}

interior vacuum
_gradus_praesignare (
    Probans* pr)
{
     Piscina* p       = pr->p;
      chorda  clavis  = _clavis(pr, "praesignatum");
      chorda  corpus;
HorreumError  e;

    si (!_praesignatum(pr, HTTP_PUT, clavis, _l("praesignatum", p),
        NIHIL,
            &e))
    {
        _linea(pr, "praesignare", ZEPHYRUM, HORREUM_REFUTATUM,
            _causa(pr,
            "PUT praesignatum", &e));
        redde;
    }
    si (   !_praesignatum(pr, HTTP_GET, clavis, chorda_ex_buffer(NIHIL,
                ZEPHYRUM), &corpus, &e)
        || !chorda_aequalis_literis(corpus, "praesignatum"))
    {
        _linea(pr, "praesignare", ZEPHYRUM, HORREUM_REFUTATUM,
            _causa(pr,
            "GET praesignatum", &e));
        redde;
    }
    _linea(pr, "praesignare", ZEPHYRUM, HORREUM_PROBATUM, _l(
        "PUT et GET per URL sine Authorization", p));
}

/* ===== purgatio ===== */
/* obiecta sub radice deleta (pagina una per gyrum); numerus inventorum,
 * -I si enumeratio deficit */
interior s32
_obiecta_delere (
    Probans* pr,
        b32  delere)
{
          Piscina* p = pr->p;
      HorreumOpus  o;
    HorreumExitus  x;
     HorreumError  e;
HorreumEnumerandi  en;
    HorreumPagina  pg;
   HorreumDeletio* exitus;
           chorda* claves;
              i32  numerus;
              i32  i;

    memset(&en, ZEPHYRUM, magnitudo(en));
    en.praefixum = pr->radix;
    si (   !horreum_opus_enumerare(pr->h, pr->situla, &en, &o, &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, NIHIL)
        || !horreum_paginam_legere(x.responsum->corpus, &pg, &e, p))
    {
        redde -I;
    }
    si (!delere || pg.obiecta_numerus == ZEPHYRUM)
    {
        redde (s32)pg.obiecta_numerus;
    }
    claves = (chorda*)piscina_allocare(p, (memoriae_index)
        ((memoriae_index)pg.obiecta_numerus * magnitudo(chorda)));
    per (i = ZEPHYRUM; i < pg.obiecta_numerus; i++)
    {
        claves[i] = pg.obiecta[i].clavis;
    }
    si (   !horreum_opus_delere_plura(pr->h, pr->situla, claves,
                pg.obiecta_numerus, &o, &e, p)
        || !_exsequi(pr, &o, &x, NIHIL, &e, NIHIL)
        || !horreum_deletiones_legere(x.responsum->corpus, &exitus,
            &numerus, &e, p))
    {
        per (i = ZEPHYRUM; i < pg.obiecta_numerus; i++)
        {
            (vacuum)_delere(pr, claves[i], &e);
        }
    }
    redde (s32)pg.obiecta_numerus;
}

interior vacuum
_gradus_purgatio (
    Probans* pr)
{
           Piscina* p = pr->p;
HorreumOnusPendens* onera;
       HorreumOpus  o;
      HorreumError  e;
               i32  numerus = ZEPHYRUM;
               i32  i;
               i32  gyri;
               s32  reliqua;
 ChordaAedificator* a;

    per (gyri = ZEPHYRUM; gyri < VIII && _obiecta_delere(pr, VERUM)
        > ZEPHYRUM; gyri++)
    {
    }
    si (_pendentes(pr, &onera, &numerus, &e, NIHIL))
    {
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            si (horreum_opus_mp_abicere(pr->h, pr->situla,
                onera[i].clavis,
                    onera[i].id_oneris, &o, &e, p))
            {
                (vacuum)_exsequi(pr, &o, NIHIL, NIHIL, &e, NIHIL);
            }
        }
    }
    reliqua = _obiecta_delere(pr, FALSUM);
    numerus = ZEPHYRUM;
    si (   !_pendentes(pr, &onera, &numerus, &e, NIHIL)
        || reliqua < ZEPHYRUM)
    {
        pr->r->relicta = -I;
        _linea(pr, "purgatio", ZEPHYRUM, HORREUM_NON_PROBATUM,
            _causa(pr,
            "enumeratio post purgationem", &e));
        redde;
    }
    pr->r->relicta = reliqua + (s32)numerus;
    si (pr->r->relicta > ZEPHYRUM)
    {
        a = chorda_aedificator_creare(p, LXIV);
        chorda_aedificator_appendere_i32(a, (i32)reliqua);
        chorda_aedificator_appendere_literis(a, " obiecta et ");
        chorda_aedificator_appendere_i32(a, numerus);
        chorda_aedificator_appendere_literis(a, " onera relicta");
        _linea(pr, "purgatio", ZEPHYRUM, HORREUM_REFUTATUM,
            chorda_aedificator_finire(a));
        redde;
    }
    _linea(pr, "purgatio", ZEPHYRUM, HORREUM_PROBATUM, _l(
        "nihil relictum sub praefixo", p));
}

/* ===== aditus ===== */
b32
horreum_probare (
                     Horreum* h,
    constans HorreumProbandi* o,
             HorreumProbatio* r,
                HorreumError* e,
                     Piscina* p)
{
                    Probans pr;
    HorreumClientisOptiones co;
                     chorda praefixum;
                     chorda id;
               HorreumError e_locus;

    si (!e)
    {
        e = &e_locus;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    si (!h || !r || !p)
    {
        e->genus = HORREUM_MALUM;
        e->nuntius = chorda_ex_literis("horreum: probare sine horreo",
            p);
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(*r));
    memset(&pr, ZEPHYRUM, magnitudo(pr));
    pr.h       = h;
    pr.r       = r;
    pr.p       = p;
    pr.situla  = horreum_situla_ordinaria(h);
    si (pr.situla.mensura == ZEPHYRUM)
    {
        e->genus    = HORREUM_MALUM;
        e->nuntius  = _l("horreum: probare sine situla ordinaria", p);
        redde FALSUM;
    }
    si (o)
    {
        pr.vectura   = o->vectura;
        pr.responsa  = o->responsa;
    }
    si (!pr.vectura.exsequi)
    {
        pr.vectura = http_vectura_ordinaria();
    }
    si (   pr.responsa
        && !filum_directorium_creare_si_necesse(pr.responsa))
    {
        e->genus    = HORREUM_MALUM;
        e->nuntius  =
            _l("horreum: directorium responsorum creari nequit",
            p);
        redde FALSUM;
    }
    praefixum = o && o->praefixum.mensura > ZEPHYRUM
                ? o->praefixum : _l("horreum-probatio/", p);
    id        = o && o->id.mensura > ZEPHYRUM
                ? o->id : signatura_aws_dies((MomentumSec)time(NIHIL),
                p);
    pr.radix  = chorda_concatenare(chorda_concatenare(praefixum, id, p),
        _l("/", p), p);
    r->praefixum = pr.radix;

    horreum_omnia_permittere(h);
    memset(&co, ZEPHYRUM, magnitudo(co));
    co.vectura = pr.vectura;
    horreum_cliens_parare(h, &co, p);

    _gradus_iter(&pr);
    _gradus_solum_si_abest(&pr);
    _gradus_solum_si_etag(&pr);
    _gradus_acl(&pr);
    _gradus_summa(&pr, "checksum_crc32", HORREUM_CAP_CHECKSUM_CRC32,
        HORREUM_CHECKSUM_CRC32, "x-amz-checksum-crc32");
    _gradus_summa(&pr, "checksum_sha256", HORREUM_CAP_CHECKSUM_SHA256,
        HORREUM_CHECKSUM_SHA256, "x-amz-checksum-sha256");
    _gradus_delere_plura(&pr);
    _gradus_enumerare(&pr);
    _gradus_partium(&pr);
    _gradus_praesignare(&pr);
    _gradus_purgatio(&pr);
    redde VERUM;
}

/* textus post <ID>, <DisplayName> et <Location> (R2: rationarium in
 * hospite) usque ad '<' proximum */
chorda
horreum_responsum_purgare (
     chorda  corpus,
    Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p,
        (i32)corpus.mensura + LXIV);
                   i32  i = ZEPHYRUM;
                   i32  k;
                   i32  t;
    constans character* tegenda[] = { "<ID>", "<DisplayName>",
        "<Location>" };

    dum (i < (i32)corpus.mensura)
    {
        per (t = ZEPHYRUM; t < III; t++)
        {
            i32 n = (i32)strlen(tegenda[t]);

            si (   i + n <= (i32)corpus.mensura
                && memcmp(corpus.datum + i, tegenda[t],
                    (memoriae_index)n) == ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a, tegenda[t]);
                per (k = i + n; k < (i32)corpus.mensura
                    && corpus.datum[k] != '<'; k++)
                {
                }
                chorda_aedificator_appendere_literis(a, "SCRUBBED");
                i = k;
                frange;
            }
        }
        si (t < III)
        {
            perge;
        }
        chorda_aedificator_appendere_character(a,
            (character)corpus.datum[i]);
        i++;
    }
    redde chorda_aedificator_finire(a);
}

constans character*
horreum_verdictum_nomen (
    HorreumVerdictum v)
{
    commutatio (v)
    {
        casus HORREUM_PROBATUM:   redde "probatum";
        casus HORREUM_REFUTATUM:  redde "refutatum";
        ordinarius:               redde "non probatum";
    }
}
