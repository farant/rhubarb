/* probatio_signatura_aws.c - AWS SigV4 contra vectores aws-c-auth
 * (horreum-plan T1). Vectores in probationes/fixa/signatura_aws/v4/
 * (84a26ea1, NOTICE); index.tsv dicit quos currere, quos excludere cum
 * causa. Lector request.txt hic, localis: linea petitionis ad spatium
 * primum et " HTTP/1.1" ultimum, capita "Titulus:valor", lineae spatio
 * incipientes valori priori adiunguntur, corpus post lineam vacuam. */
#include "latina.h"
#include "signatura_aws.h"
#include "sigillum.h"
#include "filum.h"
#include "iter_directoria.h"
#include "json.h"
#include "fasti.h"
#include "url.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_VECTORUM     "probationes/fixa/signatura_aws/v4"
#define VIA_INDICIS      "probationes/fixa/signatura_aws/index.tsv"
#define PARIA_MAXIMA     XXXII
#define SECRETUM_EXEMPLI "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY"

nomen structura {
    SignaturaPetitio petitio;
        SignaturaPar quaestio[PARIA_MAXIMA];
        SignaturaPar capita[PARIA_MAXIMA];
           character methodus[XVI];
} Vector;

interior chorda
_legere (
    constans character* titulus,
    constans character* filum,
               Piscina* p)
{
    character via[DXII];

    sprintf(via, "%s/%s/%s", VIA_VECTORUM, titulus, filum);
    redde filum_legere_totum(via, p);
}

/* literae in piscina sectionis ((i8*)"..." constans abicit:
 * -Wcast-qual) */
interior chorda
_literae (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

/* lineam proximam (sine '\n') ex *reliquum detrahit */
interior chorda
_linea (
     chorda* reliquum,
    Piscina* p)
{
    chorda linea;
       s32 i = chorda_invenire_index(*reliquum, _literae("\n", p));

    si (i < ZEPHYRUM)
    {
        linea              = *reliquum;
        reliquum->datum    = reliquum->datum + reliquum->mensura;
        reliquum->mensura  = ZEPHYRUM;
        redde linea;
    }
    linea = chorda_sectio(*reliquum, ZEPHYRUM, (i32)i);
    *reliquum = chorda_sectio(*reliquum, (i32)i + I, reliquum->mensura);
    redde linea;
}

interior chorda
_ante (
    chorda s,
       s32 i)
{
    redde chorda_sectio(s, ZEPHYRUM, (i32)i);
}

interior chorda
_post (
    chorda s,
       s32 i,
       i32 saltus)
{
    redde chorda_sectio(s, (i32)i + saltus, s.mensura);
}

interior chorda
_hex_corporis (
     chorda  corpus,
    Piscina* p)
{
     Sigillum s = sigillum_computare(corpus.datum,
                      (memoriae_index)corpus.mensura);
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, hex);
    redde chorda_ex_literis(hex, p);
}

/* quaestio "a=b&c" -> paria decodificata */
interior i32
_quaestionem_legere (
          chorda  q,
    SignaturaPar* paria,
         Piscina* p)
{
    i32 n = ZEPHYRUM;

    dum (q.mensura > ZEPHYRUM && n < PARIA_MAXIMA)
    {
           s32 et   = chorda_invenire_index(q, _literae("&", p));
        chorda par  = et < ZEPHYRUM ? q : _ante(q, et);
           s32 aeq  = chorda_invenire_index(par, _literae("=", p));

        si (aeq < ZEPHYRUM)
        {
            paria[n].titulus  = url_decodificare(par, p);
            paria[n].valor    = chorda_ex_literis("", p);
        }
        alioquin
        {
            paria[n].titulus  = url_decodificare(_ante(par, aeq), p);
            paria[n].valor    = url_decodificare(_post(par, aeq, I), p);
        }
        n++;
        si (et < ZEPHYRUM)
        {
            frange;
        }
        q = _post(q, et, I);
    }
    redde n;
}

interior b32
_vectorem_legere (
    constans character* titulus,
                Vector* v,
               Piscina* p)
{
           chorda  contextus  = _legere(titulus, "context.json", p);
           chorda  petitio    = _legere(titulus, "request.txt", p);
     JsonResultus  j;
        JsonValor* cred;
           chorda  linea;
           chorda  uri;
              s32  i;
         DiesHora  dh;
           chorda  vacua  = chorda_ex_literis("", p);
              i32  n      = ZEPHYRUM;

    memset(v, ZEPHYRUM, magnitudo(*v));
    j = json_legere(contextus, p);
    si (!j.successus || petitio.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    cred = json_objectum_capere(j.radix, "credentials");
    v->petitio.clavis_id = json_capere_chorda(cred, "access_key_id",
        vacua);
    v->petitio.secretum  = json_capere_chorda(cred, "secret_access_key",
                               vacua);
    v->petitio.regio = json_capere_chorda(j.radix, "region", vacua);
    v->petitio.servitium = json_capere_chorda(j.radix, "service",
        vacua);
    v->petitio.corpus_capite = json_capere_boolean(j.radix, "sign_body",
                                   FALSUM);
    v->petitio.duratio = (i32)json_capere_integer(j.radix,
                               "expiration_in_seconds", ZEPHYRUM);
    si (!fasti_ex_iso(json_capere_chorda(j.radix, "timestamp", vacua),
        &dh))
    {
        redde FALSUM;
    }
    v->petitio.tempus = fasti_ad_unix(dh);

    /* linea petitionis */
    linea  = _linea(&petitio, p);
    i      = chorda_invenire_index(linea, _literae(" ", p));
    si (i < ZEPHYRUM || i >= (s32)magnitudo(v->methodus))
    {
        redde FALSUM;
    }
    memcpy(v->methodus, linea.datum, (size_t)i);
    v->methodus[i] = '\0';
    v->petitio.methodus = v->methodus;
    uri = _post(linea, i, I);
    i = chorda_invenire_ultimum_index(uri, _literae(" HTTP/1.1", p));
    si (i < ZEPHYRUM)
    {
        redde FALSUM;
    }
    uri  = _ante(uri, i);
    i    = chorda_invenire_index(uri, _literae("?", p));
    si (i < ZEPHYRUM)
    {
        v->petitio.via = url_decodificare(uri, p);
    }
    alioquin
    {
        v->petitio.via = url_decodificare(_ante(uri, i), p);
        v->petitio.quaestio_numerus = _quaestionem_legere(_post(uri, i,
            I),
            v->quaestio, p);
    }
    v->petitio.quaestio = v->quaestio;

    /* capita; lineae spatio incipientes = continuatio */
    dum (petitio.mensura > ZEPHYRUM)
    {
        linea = _linea(&petitio, p);
        si (linea.mensura == ZEPHYRUM)
        {
            frange;
        }
        si (   (linea.datum[ZEPHYRUM] == ' '
            || linea.datum[ZEPHYRUM] == '\t')
            && n > ZEPHYRUM)
        {
            ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);

            chorda_aedificator_appendere_chorda(a, v->capita[n
                - I].valor);
            chorda_aedificator_appendere_character(a, ' ');
            chorda_aedificator_appendere_chorda(a, linea);
            v->capita[n - I].valor = chorda_aedificator_finire(a);
            perge;
        }
        i = chorda_invenire_index(linea, _literae(":", p));
        si (i < ZEPHYRUM || n >= PARIA_MAXIMA)
        {
            redde FALSUM;
        }
        si (chorda_aequalis_case_insensitivus(_ante(linea, i),
                _literae("Host", p)))
        {
            v->petitio.hospes = _post(linea, i, I);
            perge;
        }
        v->capita[n].titulus  = _ante(linea, i);
        v->capita[n].valor    = _post(linea, i, I);
        n++;
    }
    v->petitio.capita           = v->capita;
    v->petitio.capita_numerus   = n;
    v->petitio.corpus_sigillum  = _hex_corporis(petitio, p);
    redde VERUM;
}

/* valor lineae "Authorization:" in petitione signata */
interior chorda
_auctoritas_expectata (
      chorda  signata,
     Piscina* p)
{
    dum (signata.mensura > ZEPHYRUM)
    {
        chorda linea = _linea(&signata, p);

        si (chorda_incipit(linea, _literae("Authorization:", p)))
        {
            redde _post(linea, (s32)XIV, ZEPHYRUM);
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

/* quaestio lineae petitionis signatae ("GET /?q HTTP/1.1") */
interior chorda
_quaestio_expectata (
      chorda  signata,
     Piscina* p)
{
    chorda linea  = _linea(&signata, p);
       s32 q      = chorda_invenire_index(linea, _literae("?", p));
       s32 h     = chorda_invenire_ultimum_index(linea,
                       _literae(" HTTP/1.1", p));

    si (q < ZEPHYRUM || h < q)
    {
        redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    redde chorda_sectio(linea, (i32)q + I, (i32)h);
}

/* paria "a&b&c" ut multitudo: eadem paria, eodem numero */
interior b32
_paria_aequalia (
      chorda  a,
      chorda  b,
     Piscina* p)
{
    i32 na = chorda_numerare_occurrentia(a, _literae("&", p));
    i32 nb = chorda_numerare_occurrentia(b, _literae("&", p));

    si (na != nb)
    {
        redde FALSUM;
    }
    dum (a.mensura > ZEPHYRUM)
    {
           s32 et        = chorda_invenire_index(a, _literae("&", p));
        chorda par       = et < ZEPHYRUM ? a : _ante(a, et);
        chorda reliquum  = b;
           b32 inventum  = FALSUM;

        dum (reliquum.mensura > ZEPHYRUM)
        {
               s32 e2 = chorda_invenire_index(reliquum, _literae("&",
                   p));
            chorda p2 = e2 < ZEPHYRUM ? reliquum : _ante(reliquum, e2);

            /* decodificata comparantur: request.txt signatae "ሴ"
             * crudum scribit, nos %E1%88%B4 */
            si (chorda_aequalis(url_decodificare(par, p),
                    url_decodificare(p2, p)))
            {
                inventum = VERUM;
                frange;
            }
            si (e2 < ZEPHYRUM)
            {
                frange;
            }
            reliquum = _post(reliquum, e2, I);
        }
        si (!inventum)
        {
            redde FALSUM;
        }
        si (et < ZEPHYRUM)
        {
            frange;
        }
        a = _post(a, et, I);
    }
    redde VERUM;
}

/* per index.tsv: vocat f pro omni vectore 'curre' */
nomen vacuum (*VectorFunctio)(CredoContextus* c, constans character* t);

interior i32
_per_indicem (
    CredoContextus* c,
     VectorFunctio  f)
{
    chorda index  = filum_legere_totum(VIA_INDICIS, c->piscina);
       i32 n      = ZEPHYRUM;

    CREDO_VERUM(index.mensura > ZEPHYRUM);
    dum (index.mensura > ZEPHYRUM)
    {
           chorda linea = _linea(&index, c->piscina);
              s32 tab = chorda_invenire_index(linea, _literae("\t",
                  c->piscina));
        character titulus[CXXVIII];

        si (tab <= ZEPHYRUM || tab >= (s32)magnitudo(titulus))
        {
            perge;
        }
        si (!chorda_incipit(_post(linea, tab, I), _literae("curre",
            c->piscina)))
        {
            perge;
        }
        memcpy(titulus, linea.datum, (size_t)tab);
        titulus[tab] = '\0';
        f(c, titulus);
        n++;
    }
    redde n;
}

interior vacuum
_vector_capitum (
         CredoContextus* c,
    constans character* t)
{
               Vector v;
    SignaturaResultus r;

    imprimere("  [%s]\n", t);
    CREDO_VERUM(_vectorem_legere(t, &v, c->piscina));
    r = signatura_aws_capitibus(&v.petitio, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS(r.petitio_canonica,
        _legere(t, "header-canonical-request.txt", c->piscina));
    CREDO_CHORDA_AEQUALIS(r.chorda_signanda,
        _legere(t, "header-string-to-sign.txt", c->piscina));
    CREDO_CHORDA_AEQUALIS(r.signatura,
        _legere(t, "header-signature.txt", c->piscina));
    CREDO_CHORDA_AEQUALIS(r.auctoritas, _auctoritas_expectata(
        _legere(t, "header-signed-request.txt", c->piscina),
        c->piscina));
}

interior vacuum
_vector_quaestionis (
         CredoContextus* c,
    constans character* t)
{
               Vector v;
    SignaturaResultus r;

    imprimere("  [%s]\n", t);
    CREDO_VERUM(_vectorem_legere(t, &v, c->piscina));
    r = signatura_aws_quaestione(&v.petitio, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS(r.petitio_canonica,
        _legere(t, "query-canonical-request.txt", c->piscina));
    CREDO_CHORDA_AEQUALIS(r.chorda_signanda,
        _legere(t, "query-string-to-sign.txt", c->piscina));
    CREDO_CHORDA_AEQUALIS(r.signatura,
        _legere(t, "query-signature.txt", c->piscina));
    CREDO_VERUM(_paria_aequalia(r.quaestio_signata, _quaestio_expectata(
        _legere(t, "query-signed-request.txt", c->piscina), c->piscina),
        c->piscina));
}

interior vacuum
_sectio_vectores_capitum (
    CredoContextus* c)
{
    CREDO_MAIOR_I32(_per_indicem(c, _vector_capitum), ZEPHYRUM);
}

interior vacuum
_sectio_vectores_quaestionis (
    CredoContextus* c)
{
    CREDO_MAIOR_I32(_per_indicem(c, _vector_quaestionis), ZEPHYRUM);
}

/* omne directorium v4/ in index.tsv nominatur (curre aut exclude) */
interior vacuum
_sectio_index_integer (
    CredoContextus* c)
{
                  chorda index = filum_legere_totum(VIA_INDICIS,
                                      c->piscina);
     DirectoriumIterator* it =
         directorium_iterator_aperire(VIA_VECTORUM,
                                   c->piscina);
    DirectoriumIntroitus* e;
                     i32  n = ZEPHYRUM;

    CREDO_NON_NIHIL(it);
    dum ((e = directorium_iterator_proximum(it)) != NIHIL)
    {
        ChordaAedificator* a;

        si (   e->genus != INTROITUS_DIRECTORIUM
            || chorda_incipit(e->titulus, _literae(".", c->piscina)))
        {
            perge;
        }
        a = chorda_aedificator_creare(c->piscina, LXIV);
        chorda_aedificator_appendere_character(a, '\n');
        chorda_aedificator_appendere_chorda(a, e->titulus);
        chorda_aedificator_appendere_character(a, '\t');
        n++;
        /* "\ntitulus\t" aut index ab eo incipiens */
        CREDO_VERUM(chorda_continet(index,
            chorda_aedificator_spectare(a))
            || chorda_incipit(index,
            _post(chorda_aedificator_spectare(a),
                   ZEPHYRUM, I)));
    }
    directorium_iterator_claudere(it);
    CREDO_AEQUALIS_I32(n, XXXVIII);
}

interior vacuum
_sectio_codificatio (
    CredoContextus* c)
{
    Piscina* p = c->piscina;

    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("a b", c->piscina), FALSUM,
        p), "a%20b");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("/x/y", c->piscina), VERUM,
        p), "/x/y");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("/x/y", c->piscina), FALSUM,
        p),
        "%2Fx%2Fy");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("~-_.AZaz09", c->piscina),
        FALSUM, p),
        "~-_.AZaz09");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("+", c->piscina), FALSUM, p),
        "%2B");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("%", c->piscina), FALSUM, p),
        "%25");
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_codificare(_literae("\xE1\x88\xB4", c->piscina),
        FALSUM, p),
        "%E1%88%B4");
}

interior vacuum
_sectio_dies (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(
        signatura_aws_dies((MomentumSec)1440938160L, c->piscina),
        "20150830T123600Z");
}

interior vacuum
_sectio_duratio (
    CredoContextus* c)
{
    Vector v;

    CREDO_VERUM(_vectorem_legere("get-vanilla", &v, c->piscina));
    v.petitio.duratio = ZEPHYRUM;
    CREDO_FALSUM(signatura_aws_quaestione(&v.petitio, c->piscina)
                     .successus);
    v.petitio.duratio = SIGNATURA_AWS_DURATIO_MAXIMA + I;
    CREDO_FALSUM(signatura_aws_quaestione(&v.petitio, c->piscina)
                     .successus);
    v.petitio.duratio = SIGNATURA_AWS_DURATIO_MAXIMA;
    CREDO_VERUM(signatura_aws_quaestione(&v.petitio, c->piscina)
                    .successus);
}

/* nullum campum resultus secretum continet */
interior b32
_resultus_tacitus (
    constans SignaturaResultus* r,
                       Piscina* p)
{
    chorda s = _literae(SECRETUM_EXEMPLI, p);
       i32 k;

    si (   chorda_continet(r->error, s)
        || chorda_continet(r->petitio_canonica, s)
        || chorda_continet(r->chorda_signanda, s)
        || chorda_continet(r->signatura, s)
        || chorda_continet(r->auctoritas, s)
        || chorda_continet(r->quaestio_signata, s))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < r->capita_addita_numerus; k++)
    {
        si (chorda_continet(r->capita_addita[k].valor, s))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_secretum_tacitum (
    CredoContextus* c)
{
               Vector v;
    SignaturaResultus r;

    CREDO_VERUM(_vectorem_legere("get-vanilla", &v, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(v.petitio.secretum, SECRETUM_EXEMPLI);
    r = signatura_aws_capitibus(&v.petitio, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_VERUM(_resultus_tacitus(&r, c->piscina));
    r = signatura_aws_quaestione(&v.petitio, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_VERUM(_resultus_tacitus(&r, c->piscina));

    v.petitio.regio  = chorda_ex_literis("", c->piscina);
    r                = signatura_aws_capitibus(&v.petitio, c->piscina);
    CREDO_FALSUM(r.successus);
    CREDO_CHORDA_NON_VACUA(r.error);
    CREDO_VERUM(_resultus_tacitus(&r, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vectores_capitum", _sectio_vectores_capitum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vectores_quaestionis", _sectio_vectores_quaestionis, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "index_integer", _sectio_index_integer, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "codificatio", _sectio_codificatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dies", _sectio_dies, NIHIL, NIHIL, NIHIL, NIHIL },
    { "duratio", _sectio_duratio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secretum_tacitum", _sectio_secretum_tacitum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("signatura_aws", SECTIONES);
}
