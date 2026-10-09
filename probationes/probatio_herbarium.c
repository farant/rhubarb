/* probatio_herbarium.c - herbarium: captura, variantes, indices
 * admissi, secreta petitionis, defectus capturae, redditio
 * (vates-plan-2 T2; herbarium-spec par. VI). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "http.h"
#include "json.h"
#include "filum.h"
#include "herbarium.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* ---- vectura scripta: responsa ordine reddit ---- */

nomen structura {
                   i32  status;
    constans character* corpus;
    constans character* caput_titulus;   /* NIHIL = sine capite */
    constans character* caput_valor;
} ResponsumScriptum;

nomen structura {
    constans ResponsumScriptum* responsa;
                           i32  numerus;
                           i32  index;
} Scriptor;

interior HttpResultus
_scripta_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
                      Scriptor* s = (Scriptor*)datum;
                  HttpResultus  res;
                 HttpResponsum* resp;
    constans ResponsumScriptum* r;

    (vacuum)petitio;
    memset(&res, 0, magnitudo(res));
    si (s->index >= s->numerus)
    {
        res.error = HTTP_ERROR_CONNEXIO;
        redde res;
    }
    r = &s->responsa[s->index];
    s->index++;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = r->status;
    resp->corpus = chorda_ex_literis(r->corpus, piscina);
    resp->capita = (HttpCaput*)piscina_allocare(piscina,
        (i64)(III * magnitudo(HttpCaput)));
    resp->capita[0].titulus = chorda_ex_literis("Content-Type",
        piscina);
    resp->capita[0].valor = chorda_ex_literis("application/json",
        piscina);
    resp->capita[I].titulus = chorda_ex_literis("Set-Cookie", piscina);
    resp->capita[I].valor = chorda_ex_literis("sessio=SECRETUM_COOKIE",
        piscina);
    resp->capita_numerus = II;
    si (r->caput_titulus)
    {
        resp->capita[II].titulus = chorda_ex_literis(r->caput_titulus,
            piscina);
        resp->capita[II].valor = chorda_ex_literis(r->caput_valor,
            piscina);
        resp->capita_numerus = III;
    }
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

interior HttpVectura
_scripta (
    Scriptor* s)
{
    HttpVectura v;

    v.exsequi  = _scripta_exsequi;
    v.datum    = s;
    redde v;
}

interior HttpPetitio*
_petitio (
    Piscina* piscina)
{
    HttpPetitio* p = http_petitio_creare(piscina, HTTP_POST,
        "https://api.example.com/v1/messages");

    http_petitio_caput_addere(p, "x-api-key",
        "SECRETUM_CLAVIS_PROBATIONIS");
    http_petitio_corpus_ponere_chorda(p, chorda_ex_literis(
        "{\"model\":\"exemplar-x\",\"messages\":\"CORPUS_SECRETUM\"}",
        piscina));
    redde p;
}

/* numerus filorum in <dir>/specimina et linearum in <dir>/index.jsonl */
interior i32
_specimina_numerare (
    constans character* dir,
               Piscina* piscina)
{
               character  via[DXII];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;
                     i32  numerus = 0;

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    si (!it)
    {
        redde 0;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (introitus->genus == INTROITUS_FILUM)
        {
            numerus++;
        }
    }
    directorium_iterator_claudere(it);
    redde numerus;
}

interior i32
_lineae_indicis (
    constans character* dir,
               Piscina* piscina)
{
    character via[DXII];
       chorda textus;
          i32 i;
          i32 lineae = 0;

    sprintf(via, "%s/index.jsonl", dir);
    textus = filum_legere_totum(via, piscina);
    per (i = 0; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '\n')
        {
            lineae++;
        }
    }
    redde lineae;
}

/* omnia fila directorii concatenata (pro inquisitione secretorum) */
interior chorda
_omnia_legere (
    constans character* dir,
               Piscina* piscina)
{
               character  via[DXII];
               character  plena[DXII];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;
                  chorda  summa = chorda_ex_literis("",
                      piscina);

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    dum (it && (introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        chorda pars;
        chorda iuncta;

        sprintf(plena, "%s/%.*s", via,
            (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        pars            = filum_legere_totum(plena, piscina);
        iuncta.mensura  = summa.mensura + pars.mensura;
        iuncta.datum = (i8*)piscina_allocare(piscina,
            (i64)iuncta.mensura + I);
        memcpy(iuncta.datum, summa.datum, (size_t)summa.mensura);
        memcpy(iuncta.datum + summa.mensura, pars.datum,
            (size_t)pars.mensura);
        summa = iuncta;
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    redde summa;
}

interior vacuum
_purgare (
    constans character* dir,
               Piscina* piscina)
{
               character  via[DXII];
               character  plena[DXII];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    dum (it && (introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        sprintf(plena, "%s/%.*s", via,
            (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        (vacuum)unlink(plena);
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    (vacuum)rmdir(via);
    sprintf(via, "%s/index.jsonl", dir);
    (vacuum)unlink(via);
    (vacuum)rmdir(dir);
}

interior vacuum
_directorium (
             character* fructus,
    constans character* titulus)
{
    sprintf(fructus, "/tmp/probatio_herbarium_%s_%ld", titulus,
            (longus)getpid());
}

/* ---- probationes ---- */

interior vacuum
probatio_sceleti(Piscina* piscina)
{
    HttpResponsum a;
    HttpResponsum b;
    HttpResponsum c;
    HttpResponsum d;

    imprimere("\n--- Probans herbarium_clavis_sceleti ---\n");
    memset(&a, 0, magnitudo(a));
    memset(&b, 0, magnitudo(b));
    memset(&c, 0, magnitudo(c));
    memset(&d, 0, magnitudo(d));
    a.status = CDXXIX;
    a.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"x\",\"message\":\"m1\"}}",
        piscina);
    b = a;
    b.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"y\",\"message\":\"aliud\"}}",
        piscina);
    c = a;
    c.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"x\",\"message\":\"m\"},\"novum\":1}",
        piscina);
    d.status = DII;
    d.corpus = chorda_ex_literis("<html>bad gateway</html>", piscina);

    /* valores diversi, forma eadem -> clavis eadem */
    CREDO_CHORDA_AEQUALIS(herbarium_clavis_sceleti(NIHIL, &a, piscina,
        NIHIL),
                          herbarium_clavis_sceleti(NIHIL, &b, piscina,
                          NIHIL));
    /* campus novus -> clavis alia */
    CREDO_FALSUM(chorda_aequalis(herbarium_clavis_sceleti(NIHIL, &a,
        piscina, NIHIL),
                                 herbarium_clavis_sceleti(NIHIL, &c,
                                 piscina, NIHIL)));
    /* non JSON: status + classis longitudinis */
    CREDO_CHORDA_INCIPIT(herbarium_clavis_sceleti(NIHIL, &d, piscina,
        NIHIL),
                         chorda_ex_literis("502:crudum:", piscina));
}

interior vacuum
probatio_captura(Piscina* piscina)
{
            character  dir[CCLVI];
    ResponsumScriptum  responsa[VI];
             Scriptor  s;
    HerbariumOptiones  o;
            Herbarium* h;
          HttpVectura  v;
         HttpResultus  r;
               chorda  omnia;
    constans character* constans campi[II] = { "model", NIHIL };
    i32 i;

    imprimere("\n--- Probans herbarium captura ---\n");
    _directorium(dir, "captura");
    _purgare(dir, piscina);

    /* 0,1: idem genus, numeri soli diversi -> una variantis
     * 2: verba alia -> variantis II
     * 3: verba tertia, sed variantes_maximae II -> non servatur
     * 4: status CC sub limine -> nihil
     * 5: genus aliud (529) -> specimen novum */
    responsa[0].status = CDXXIX;
    responsa[0].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"limit 50 reached\"}}";
    responsa[0].caput_titulus  = "retry-after";
    responsa[0].caput_valor    = "2";
    responsa[I]                = responsa[0];
    responsa[I].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"limit 99 reached\"}}";
    responsa[II] = responsa[0];
    responsa[II].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"slow down please\"}}";
    responsa[III] = responsa[0];
    responsa[III].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"tertia verba\"}}";
    responsa[IV].status         = CC;
    responsa[IV].corpus         = "{\"ok\":true}";
    responsa[IV].caput_titulus  = NIHIL;
    responsa[IV].caput_valor    = NIHIL;
    responsa[V].status          = DXXIX;
    responsa[V].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";
    responsa[V].caput_titulus  = NIHIL;
    responsa[V].caput_valor    = NIHIL;
    s.responsa                 = responsa;
    s.numerus                  = VI;
    s.index                    = 0;

    o                    = herbarium_optiones_ordinariae();
    o.directorium        = dir;
    o.variantes_maximae  = II;
    o.campi_petitionis   = campi;
    h                    = herbarium_aperire(piscina, &o);
    CREDO_NON_NIHIL(h);
    v = herbarium_vectura(h, _scripta(&s));

    per (i = 0; i < VI; i++)
    {
        r = http_vectura_exsequi(v, _petitio(piscina), piscina);
        /* responsum TRANSMITTITUR immutatum */
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(r.responsum->status, responsa[i].status);
        CREDO_CHORDA_AEQUALIS_LITERIS(r.responsum->corpus,
            responsa[i].corpus);
    }

    /* 429: variantes II; 529: I; 200: nulla -> III fila */
    CREDO_AEQUALIS_I32(_specimina_numerare(dir, piscina), III);
    /* visiones: V (omnes supra limen, etiam non servatae) */
    CREDO_AEQUALIS_I32(_lineae_indicis(dir, piscina), V);

    omnia = _omnia_legere(dir, piscina);
    /* indices admissi: content-type et retry-after, NON set-cookie */
    CREDO_CHORDA_CONTINET(omnia, chorda_ex_literis("retry-after",
        piscina));
    CREDO_FALSUM(chorda_continet(omnia,
        chorda_ex_literis("SECRETUM_COOKIE", piscina)));
    /* capita et corpus petitionis numquam */
    CREDO_FALSUM(chorda_continet(omnia,
        chorda_ex_literis("SECRETUM_CLAVIS", piscina)));
    CREDO_FALSUM(chorda_continet(omnia,
        chorda_ex_literis("CORPUS_SECRETUM", piscina)));
    /* variantes per textum LARVATUM: "limit 99" eadem ac "limit 50"
     * (numeri soli), ergo variantis II = "slow down please" - numerus
     * fasciculorum solus hoc non discernit (planta T2-2 viridis manebat) */
    CREDO_CHORDA_CONTINET(omnia, chorda_ex_literis("slow down please",
        piscina));
    CREDO_FALSUM(chorda_continet(omnia,
        chorda_ex_literis("limit 99 reached", piscina)));
    /* summarium: campus 'model' petitionis */
    CREDO_CHORDA_CONTINET(omnia, chorda_ex_literis("exemplar-x",
        piscina));

    _purgare(dir, piscina);
}

interior vacuum
probatio_premere_explicite(Piscina* piscina)
{
            character  dir[CCLVI];
    HerbariumOptiones  o;
            Herbarium* h;
        HttpResponsum  resp;
                  Xar* s;
    HerbariumSpecimen* sp;

    imprimere("\n--- Probans herbarium_premere (novitas) ---\n");
    _directorium(dir, "premere");
    _purgare(dir, piscina);
    o              = herbarium_optiones_ordinariae();
    o.directorium  = dir;
    h              = herbarium_aperire(piscina, &o);
    memset(&resp, 0, magnitudo(resp));
    resp.status = CC;
    resp.corpus =
        chorda_ex_literis("{\"content\":[{\"type\":\"novum_genus\"}]}",
        piscina);
    herbarium_premere(h, _petitio(piscina), &resp,
                      chorda_ex_literis("blocus ignotus: novum_genus",
                      piscina));
    s = herbarium_enumerare(piscina, dir);
    CREDO_AEQUALIS_I32(xar_numerus(s), I);
    sp = (HerbariumSpecimen*)xar_obtinere(s, 0);
    CREDO_CHORDA_AEQUALIS_LITERIS(sp->causa,
        "blocus ignotus: novum_genus");
    CREDO_AEQUALIS_I32(sp->status, CC);
    /* herbarium NIHIL: nihil fit, nulla ruina */
    herbarium_premere(NIHIL, _petitio(piscina), &resp, sp->causa);
    _purgare(dir, piscina);
}

interior vacuum
probatio_captura_defectus(Piscina* piscina)
{
            character  dir[CCLVI];
            character  spec[CCLVI];
    ResponsumScriptum  responsa[I];
             Scriptor  s;
    HerbariumOptiones  o;
            Herbarium* h;
         HttpResultus  r;

    imprimere("\n--- Probans defectus capturae (responsum intactum) ---\n");
    /* aperire in loco impossibili -> NIHIL; vectura NIHIL transmittit */
    o              = herbarium_optiones_ordinariae();
    o.directorium  = "/dev/null/herbarium";
    CREDO_NIHIL(herbarium_aperire(piscina, &o));

    _directorium(dir, "defectus");
    _purgare(dir, piscina);
    responsa[0].status = D;
    responsa[0].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"api_error\",\"message\":\"x\"}}";
    responsa[0].caput_titulus  = NIHIL;
    responsa[0].caput_valor    = NIHIL;
    s.responsa                 = responsa;
    s.numerus                  = I;
    s.index                    = 0;
    o.directorium              = dir;
    h                          = herbarium_aperire(piscina, &o);
    CREDO_NON_NIHIL(h);
    /* directorium post aperitionem non scribendum */
    sprintf(spec, "%s/specimina", dir);
    (vacuum)chmod(spec, 0500);
    (vacuum)chmod(dir, 0500);
    r = http_vectura_exsequi(herbarium_vectura(h, _scripta(&s)),
                             _petitio(piscina), piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.responsum->status, D);
    CREDO_CHORDA_CONTINET(r.responsum->corpus,
        chorda_ex_literis("api_error", piscina));
    (vacuum)chmod(dir, 0755);
    (vacuum)chmod(spec, 0755);

    /* herbarium NIHIL in vectura: interior ipsa redditur */
    s.index = 0;
    r = http_vectura_exsequi(herbarium_vectura(NIHIL, _scripta(&s)),
                             _petitio(piscina), piscina);
    CREDO_VERUM(r.successus);
    _purgare(dir, piscina);
}

interior vacuum
probatio_reddere(Piscina* piscina)
{
            character  dir[CCLVI];
    ResponsumScriptum  responsa[II];
             Scriptor  s;
    HerbariumOptiones  o;
            Herbarium* h;
          HttpVectura  reddens;
         HttpResultus  r;
                  Xar* specimina;
                  i32  i;

    imprimere("\n--- Probans herbarium_enumerare + herbarium_reddens ---\n");
    _directorium(dir, "redditio");
    _purgare(dir, piscina);
    responsa[0].status = CDXXIX;
    responsa[0].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"a\"}}";
    responsa[0].caput_titulus  = "retry-after";
    responsa[0].caput_valor    = "7";
    responsa[I].status         = DXXIX;
    responsa[I].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";
    responsa[I].caput_titulus  = NIHIL;
    responsa[I].caput_valor    = NIHIL;
    s.responsa                 = responsa;
    s.numerus                  = II;
    s.index                    = 0;
    o                          = herbarium_optiones_ordinariae();
    o.directorium              = dir;
    h                          = herbarium_aperire(piscina, &o);
    per (i = 0; i < II; i++)
    {
        (vacuum)http_vectura_exsequi(herbarium_vectura(h, _scripta(&s)),
                                     _petitio(piscina), piscina);
    }

    specimina = herbarium_enumerare(piscina, dir);
    CREDO_AEQUALIS_I32(xar_numerus(specimina), II);
    reddens = herbarium_reddens(piscina, specimina);
    per (i = 0; i < II; i++)
    {
        HerbariumSpecimen* sp =
            (HerbariumSpecimen*)xar_obtinere(specimina, i);

        r = http_vectura_exsequi(reddens, _petitio(piscina), piscina);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(r.responsum->status, sp->status);
        CREDO_CHORDA_AEQUALIS(r.responsum->corpus, sp->corpus);
    }
    /* retry-after servatum et redditum */
    {
        b32 inventum = FALSUM;

        s.index = 0;
        reddens = herbarium_reddens(piscina, specimina);
        per (i = 0; i < II; i++)
        {
            r = http_vectura_exsequi(reddens, _petitio(piscina),
                piscina);
            si (http_responsum_caput(r.responsum, "retry-after").mensura
                > 0)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS(
                    http_responsum_caput(r.responsum, "retry-after"),
                    "7");
                inventum = VERUM;
            }
        }
        CREDO_VERUM(inventum);
    }
    /* exhaustum -> error nominatus */
    r = http_vectura_exsequi(reddens, _petitio(piscina), piscina);
    CREDO_FALSUM(r.successus);
    CREDO_VERUM(r.error == HTTP_ERROR_CONNEXIO);
    /* directorium absens -> Xar vacuum */
    CREDO_AEQUALIS_I32(xar_numerus(herbarium_enumerare(piscina,
        "/tmp/probatio_herbarium_numquam_creatum")), 0);
    _purgare(dir, piscina);
}

/* ---- herbarium-spec-2 H1: iudex ante usum, sedes ordinaria ---- */

nomen structura {
    i32 vocationes;
} IudexProbationis;

/* iudex probationis: corpus 'x' continens = inexspectatum */
interior chorda
_iudex_x (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    IudexProbationis* probans = (IudexProbationis*)datum;

    (vacuum)petitio;
    probans->vocationes++;
    si (chorda_continet(responsum->corpus, chorda_ex_literis("x",
            piscina)))
    {
        redde chorda_ex_literis("habet x", piscina);
    }
    redde chorda_ex_literis("", piscina);
}

interior vacuum
probatio_iudex(Piscina* piscina)
{
            character  dir[CCLVI];
    ResponsumScriptum  responsa[III];
             Scriptor  s;
    HerbariumOptiones  o;
     IudexProbationis  probans;
            Herbarium* h;
          HttpVectura  v;
         HttpResultus  r;
                  i32  i;

    imprimere("\n--- Probans iudicem ante usum ---\n");
    _directorium(dir, "iudex");
    _purgare(dir, piscina);
    /* 0: CC cum 'x' -> premitur causa iudicis; 1: CC sine 'x' -> nihil;
     * 2: CDXXIX -> premitur 'status', iudex NON vocatur */
    responsa[0].status         = CC;
    responsa[0].corpus         = "{\"ok\":\"x\"}";
    responsa[0].caput_titulus  = NIHIL;
    responsa[0].caput_valor    = NIHIL;
    responsa[I]                = responsa[0];
    responsa[I].corpus         = "{\"ok\":true}";
    responsa[II]               = responsa[0];
    responsa[II].status        = CDXXIX;
    responsa[II].corpus =
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\"}}";
    s.responsa          = responsa;
    s.numerus           = III;
    s.index             = 0;
    probans.vocationes  = 0;
    o                   = herbarium_optiones_ordinariae();
    o.directorium       = dir;
    o.iudex             = _iudex_x;
    o.iudex_datum       = &probans;
    h                   = herbarium_aperire(piscina, &o);
    CREDO_NON_NIHIL(h);
    v = herbarium_vectura(h, _scripta(&s));
    per (i = 0; i < III; i++)
    {
        r = http_vectura_exsequi(v, _petitio(piscina), piscina);
        CREDO_VERUM(r.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(r.responsum->corpus,
            responsa[i].corpus);
        si (i == 0)
        {
            /* ANTE redditionem: specimen iam in disco */
            CREDO_AEQUALIS_I32(_specimina_numerare(dir, piscina), I);
        }
    }
    CREDO_AEQUALIS_I32(probans.vocationes, II);
    CREDO_AEQUALIS_I32(_specimina_numerare(dir, piscina), II);
    CREDO_CHORDA_CONTINET(_omnia_legere(dir, piscina),
        chorda_ex_literis("habet x", piscina));
    _purgare(dir, piscina);
}

/* ambitus temporarius: valorem servat, ponit (NIHIL = removet) */
interior character*
_ambitum_ponere (
    constans character* titulus,
    constans character* valor,
               Piscina* piscina)
{
    character* vetus = getenv(titulus);
    character* copia = NIHIL;

    si (vetus)
    {
        copia = chorda_ut_cstr(chorda_ex_literis(vetus, piscina),
            piscina);
    }
    si (valor)
    {
        (vacuum)setenv(titulus, valor, I);
    }
    alioquin
    {
        (vacuum)unsetenv(titulus);
    }
    redde copia;
}

interior vacuum
probatio_sedes_ordinaria(Piscina* piscina)
{
    character  dir[CCLVI];
    character  exspectata[DXII];
    character* herbarium_vetus;
    character* domus_vetus;
       chorda  causa;
       chorda  sedes;

    imprimere("\n--- Probans sedem ordinariam ---\n");
    _directorium(dir, "sedes");
    (vacuum)mkdir(dir, 0755);
    /* $RHUBARB_HERBARIUM directorium exstans -> <dir>/<hospes> */
    herbarium_vetus = _ambitum_ponere("RHUBARB_HERBARIUM", dir,
        piscina);
    sedes = herbarium_sedes_ordinaria("api.example.com", piscina,
        &causa);
    sprintf(exspectata, "%s/api.example.com", dir);
    CREDO_CHORDA_AEQUALIS_LITERIS(sedes, exspectata);
    /* nomen non exstans -> vacua + causa, numquam tacite alio */
    (vacuum)_ambitum_ponere("RHUBARB_HERBARIUM",
        "/tmp/probatio_herbarium_numquam_creatum", piscina);
    sedes = herbarium_sedes_ordinaria("api.example.com", piscina,
        &causa);
    CREDO_AEQUALIS_I32(sedes.mensura, 0);
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("RHUBARB_HERBARIUM",
        piscina));
    /* sine ambitu: $HOME/.rhubarb/herbarium/<hospes> */
    (vacuum)_ambitum_ponere("RHUBARB_HERBARIUM", NIHIL, piscina);
    domus_vetus = _ambitum_ponere("HOME", dir, piscina);
    sedes = herbarium_sedes_ordinaria("api.example.com", piscina,
        &causa);
    sprintf(exspectata, "%s/.rhubarb/herbarium/api.example.com", dir);
    CREDO_CHORDA_AEQUALIS_LITERIS(sedes, exspectata);
    /* $HOME deest */
    (vacuum)_ambitum_ponere("HOME", NIHIL, piscina);
    sedes = herbarium_sedes_ordinaria("api.example.com", piscina,
        &causa);
    CREDO_AEQUALIS_I32(sedes.mensura, 0);
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("HOME", piscina));
    (vacuum)_ambitum_ponere("HOME", dir, piscina);
    /* hospes pravus */
    CREDO_AEQUALIS_I32(herbarium_sedes_ordinaria("a/b", piscina,
        &causa).mensura, 0);
    CREDO_AEQUALIS_I32(herbarium_sedes_ordinaria("..", piscina,
        &causa).mensura, 0);
    CREDO_AEQUALIS_I32(herbarium_sedes_ordinaria("", piscina,
        &causa).mensura, 0);
    CREDO_AEQUALIS_I32(herbarium_sedes_ordinaria(NIHIL, piscina,
        &causa).mensura, 0);
    /* ambitus restitutus */
    (vacuum)_ambitum_ponere("HOME", domus_vetus, piscina);
    (vacuum)_ambitum_ponere("RHUBARB_HERBARIUM", herbarium_vetus,
        piscina);
    (vacuum)rmdir(dir);
}

s32
principale (vacuum)
{
     Piscina* piscina;
         b32  successus;

    piscina = piscina_generare_dynamicum("probatio_herbarium", M * M);
    credo_aperire(piscina);

    probatio_sceleti(piscina);
    probatio_captura(piscina);
    probatio_premere_explicite(piscina);
    probatio_captura_defectus(piscina);
    probatio_reddere(piscina);
    /* herbarium-spec-2 H1 */
    probatio_iudex(piscina);
    probatio_sedes_ordinaria(piscina);

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
