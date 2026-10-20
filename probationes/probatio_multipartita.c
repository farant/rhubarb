/* probatio_multipartita.c - parser multipart/form-data (hospitium v3
 * T3): transcriptio reditus (initium, data, finis) comparatur.
 * Vestigium a fragmentatione non pendet: data contigue
 * appenduntur, ergo corpus ubivis scissum (et octetus singulus)
 * eandem transcriptionem reddere debet. Custodia regionis: praefixum
 * omne ad finem regionis mappatae (pagina sequens NON mappata) -
 * lectio ultra finem = SIGSEGV (tls13 R11). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "multipartita.h"
#include <string.h>
#include <sys/mman.h>

#define CUSTODIA_SPATIUM (LXIV * MXXIV)

/* ---- transcriptio ---- */

nomen structura {
    ChordaAedificator* aed;
                  i32  initia;
} Vestigium;

interior b32
_initium (
      vacuum* datum,
      chorda  campus,
      chorda  titulus_fili,
         b32  est_filum,
      chorda  mimen)
{
    Vestigium* t = (Vestigium*)datum;

    t->initia++;
    chorda_aedificator_appendere_literis(t->aed, "<");
    chorda_aedificator_appendere_chorda(t->aed, campus);
    chorda_aedificator_appendere_literis(t->aed, "|");
    chorda_aedificator_appendere_chorda(t->aed, titulus_fili);
    chorda_aedificator_appendere_literis(t->aed,
        est_filum ? "|F|" : "|C|");
    chorda_aedificator_appendere_chorda(t->aed, mimen);
    chorda_aedificator_appendere_literis(t->aed, ">");
    redde VERUM;
}

interior b32
_data (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
       Vestigium* t = (Vestigium*)datum;
             i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda_aedificator_appendere_character(t->aed,
            (character)octeti[i]);
    }
    redde VERUM;
}

interior b32
_finis (
    vacuum* datum)
{
    Vestigium* t = (Vestigium*)datum;

    chorda_aedificator_appendere_literis(t->aed, "</>");
    redde VERUM;
}

interior MultipartitaParser*
_parser (
                         Piscina* p,
                       Vestigium* t,
              constans character* limes,
    constans MultipartitaLimites* limites)
{
    MultipartitaReditus r;

    t->aed     = chorda_aedificator_creare(p, MXXIV);
    t->initia  = ZEPHYRUM;
    r.initium  = _initium;
    r.data     = _data;
    r.finis    = _finis;
    r.datum    = t;
    redde multipartita_creare(chorda_ex_literis(limes, p), limites, r,
        p);
}

interior b32
_aequalis (
                chorda  a,
    constans character* b)
{
    redde a.mensura == (i32)strlen(b)
        && memcmp(a.datum, b, (size_t)a.mensura) == ZEPHYRUM;
}

interior b32
_continet (
                chorda  a,
    constans character* b)
{
    i32 n = (i32)strlen(b);
    i32 i;

    per (i = ZEPHYRUM; i + n <= a.mensura; i++)
    {
        si (memcmp(a.datum + i, b, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ---- corpus exemplar ---- */

#define LIMES "limesXYZ"

/* duo campi + filum cum octetis 0..255 et pseudo-delimitatore */
interior chorda
_corpus_simplex (
    Piscina* p,
     chorda* expectata)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, MXXIV);
    ChordaAedificator* e = chorda_aedificator_creare(p, MXXIV);
                   i8  octeti[CCLVI];
                  i32  i;

    per (i = ZEPHYRUM; i < CCLVI; i++)
    {
        octeti[i] = (i8)i;
    }
    chorda_aedificator_appendere_literis(a,
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"titulus\"\r\n"
        "\r\n"
        "Salve\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"nota\"\r\n"
        "\r\n"
        "linea una\r\nlinea altera\r\n--\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"filum\"; "
        "filename=\"a.bin\"\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n");
    chorda_aedificator_appendere_chorda(a, chorda_ex_buffer(octeti,
        CCLVI));
    chorda_aedificator_appendere_literis(a,
        "\r\n--" LIMES "--\r\n");

    chorda_aedificator_appendere_literis(e,
        "<titulus||C|text/plain>Salve</>"
        "<nota||C|text/plain>linea una\r\nlinea altera\r\n--</>"
        "<filum|a.bin|F|application/octet-stream>");
    chorda_aedificator_appendere_chorda(e, chorda_ex_buffer(octeti,
        CCLVI));
    chorda_aedificator_appendere_literis(e, "</>");
    *expectata = chorda_aedificator_finire(e);
    redde chorda_aedificator_finire(a);
}

/* ---- sectiones ---- */

interior vacuum
_sectio_simplex (CredoContextus* c)
{
                  Vestigium t;
                     chorda expectata;
                     chorda corpus = _corpus_simplex(c->piscina,
                                 &expectata);
         MultipartitaParser* p = _parser(c->piscina, &t, LIMES, NIHIL);

    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(multipartita_adicere(p, corpus.datum, corpus.mensura));
    CREDO_VERUM(multipartita_finire(p));
    CREDO_AEQUALIS_I32(t.initia, III);
    CREDO_VERUM(chorda_aequalis(chorda_aedificator_finire(t.aed),
        expectata));
    CREDO_AEQUALIS_I32(multipartita_error(p).mensura, ZEPHYRUM);
}

/* scissio in omni positione (duo fragmenta) et octetus singulus */
interior vacuum
_sectio_secta (CredoContextus* c)
{
    chorda expectata;
    chorda corpus = _corpus_simplex(c->piscina, &expectata);
       i32 k;
       i32 malae = ZEPHYRUM;

    per (k = ZEPHYRUM; k <= corpus.mensura; k++)
    {
        Piscina* p = piscina_generare_dynamicum("secta",
                                    XVI * MXXIV);
                 Vestigium  t;
        MultipartitaParser* m = _parser(p, &t, LIMES, NIHIL);
                       b32  bene;

        bene = multipartita_adicere(m, corpus.datum, k)
            && multipartita_adicere(m, corpus.datum + k,
                corpus.mensura - k)
            && multipartita_finire(m)
            && chorda_aequalis(chorda_aedificator_finire(t.aed),
                expectata);
        si (!bene)
        {
            malae++;
        }
        piscina_destruere(p);
    }
    CREDO_AEQUALIS_I32(malae, ZEPHYRUM);

    {
                 Vestigium  t;
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);
                       b32  bene = VERUM;

        per (k = ZEPHYRUM; k < corpus.mensura; k++)
        {
            bene = bene && multipartita_adicere(m, corpus.datum + k, I);
        }
        CREDO_VERUM(bene);
        CREDO_VERUM(multipartita_finire(m));
        CREDO_VERUM(chorda_aequalis(chorda_aedificator_finire(t.aed),
            expectata));
    }
}

interior i8*
_custodiam_creare (vacuum)
{
    i8* sedes = (i8*)mmap(NIHIL, (size_t)(II * CUSTODIA_SPATIUM),
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -I, ZEPHYRUM);

    si ((vacuum*)sedes == MAP_FAILED)
    {
        redde NIHIL;
    }
    (vacuum)munmap(sedes + CUSTODIA_SPATIUM, (size_t)CUSTODIA_SPATIUM);
    redde sedes;
}

/* omne praefixum ad finem regionis: reddit numerum praefixorum ante
 * limitem finalem quae finire tamen acceptat (debet 0) */
interior i32
_verrere (
    constans i8* integrum,
            i32  n,
            i32  finis_clausurae)
{
    i8* custodia  = _custodiam_creare();
   i32  mala      = ZEPHYRUM;
   i32  k;

    si (custodia == NIHIL || n > CUSTODIA_SPATIUM)
    {
        redde n;
    }
    per (k = ZEPHYRUM; k <= n; k++)
    {
        Piscina* p = piscina_generare_dynamicum("custodia",
                                     XVI * MXXIV);
                 Vestigium  t;
        MultipartitaParser* m = _parser(p, &t, LIMES, NIHIL);
                        i8* o = custodia
                            + CUSTODIA_SPATIUM - k;
                       b32 clausum;

        memcpy(o, integrum, (size_t)k);
        clausum = multipartita_adicere(m, o, k)
            && multipartita_finire(m);
        si (clausum != (k >= finis_clausurae))
        {
            mala++;
        }
        piscina_destruere(p);
    }
    (vacuum)munmap(custodia, (size_t)CUSTODIA_SPATIUM);
    redde mala;
}

interior vacuum
_sectio_custodia_regionis (CredoContextus* c)
{
    chorda expectata;
    chorda corpus           = _corpus_simplex(c->piscina, &expectata);
       /* clausura ante CRLF ultimum */
       i32 finis_clausurae = corpus.mensura - II;

    CREDO_NON_RUIT((vacuum)_verrere(corpus.datum, corpus.mensura,
        finis_clausurae));
    CREDO_AEQUALIS_I32(_verrere(corpus.datum, corpus.mensura,
        finis_clausurae), ZEPHYRUM);
}

/* proprietates navigatorum reales (Review focus 2) */
interior vacuum
_sectio_navigator (CredoContextus* c)
{
    constans character* corpus =
        "Hic praeambulum est, ignorandum.\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"euro\"; "
        "filename=\"fallback.txt\"; filename*=UTF-8''%E2%82%AC.txt\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "nummi\r\n"
        "--" LIMES "\r\n"
        "content-disposition: form-data; name=\"a;b\\\"c\"\r\n"
        "\r\n"
        "sine typo\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"fur\"; "
        "filename=\"C:\\\\Users\\\\x\\\\malum.exe\"\r\n"
        "\r\n"
        "A\r\n--" LIMES "X non limes\r\n--" LIMES "-ne hoc quidem\r\n"
        "--" LIMES "\r\n"
        "Content-Disposition: form-data; name=\"via\"; "
        "filename=\"../../etc/passwd\"\r\n"
        "Content-Type: application/x-nihil\r\n"
        "\r\n"
        "B\r\n"
        "--" LIMES "  \t\r\n"
        "Content-Disposition: form-data; name=\"punctum\"; "
        "filename=\"..\"\r\n"
        "\r\n"
        "\r\n"
        "--" LIMES "--\r\n"
        "epilogus quoque ignorandus\r\n--" LIMES "\r\n";
    constans character* expectata =
        "<euro|\xE2\x82\xAC.txt|F|text/plain>nummi</>"
        "<a;b\"c||C|text/plain>sine typo</>"
        "<fur|malum.exe|F|text/plain>A\r\n--" LIMES "X non limes\r\n--"
        LIMES "-ne hoc quidem</>"
        "<via|passwd|F|application/x-nihil>B</>"
        "<punctum||F|text/plain></>";
             Vestigium  t;
    MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);

    CREDO_VERUM(multipartita_adicere(m, (constans i8*)corpus,
        (i32)strlen(corpus)));
    CREDO_VERUM(multipartita_finire(m));
    CREDO_VERUM(_aequalis(chorda_aedificator_finire(t.aed),
        expectata));

    /* LF solum: RFC 7578 CRLF postulat - error nominatus */
    {
        constans character* lf =
            "--" LIMES "\n"
            "Content-Disposition: form-data; name=\"x\"\n"
            "\n"
            "v\n"
            "--" LIMES "--\n";
                 Vestigium  u;
        MultipartitaParser* n = _parser(c->piscina, &u, LIMES, NIHIL);

        CREDO_FALSUM(multipartita_adicere(n, (constans i8*)lf,
            (i32)strlen(lf)));
        CREDO_VERUM(_continet(multipartita_error(n), "LF"));
        CREDO_FALSUM(multipartita_finire(n));
    }
    /* CRLF post limitem sed LF solum in capitibus */
    {
        constans character* lf =
            "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"x\"\n"
            "\r\n"
            "v\r\n"
            "--" LIMES "--\r\n";
                 Vestigium  u;
        MultipartitaParser* n = _parser(c->piscina, &u, LIMES, NIHIL);

        CREDO_FALSUM(multipartita_adicere(n, (constans i8*)lf,
            (i32)strlen(lf)));
        CREDO_VERUM(_continet(multipartita_error(n), "LF"));
    }
}

interior chorda
_partes (
    Piscina* p,
        i32  numerus)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, MXXIV);
                  i32  i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(a,
            "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"c\"\r\n"
            "\r\n"
            "v\r\n");
    }
    chorda_aedificator_appendere_literis(a, "--" LIMES "--\r\n");
    redde chorda_aedificator_finire(a);
}

interior vacuum
_sectio_limites (CredoContextus* c)
{
    /* LXIV partes licent, LXV non */
    {
                 Vestigium  t;
                    chorda  b = _partes(c->piscina, LXIV);
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);

        CREDO_VERUM(multipartita_adicere(m, b.datum, b.mensura));
        CREDO_VERUM(multipartita_finire(m));
    }
    {
                 Vestigium  t;
                    chorda  b = _partes(c->piscina, LXV);
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);

        CREDO_FALSUM(multipartita_adicere(m, b.datum, b.mensura));
        CREDO_VERUM(_continet(multipartita_error(m), "partes"));
        CREDO_AEQUALIS_I32(t.initia, LXIV);
    }
    /* caput partis super 8 KB */
    {
        ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
                                   MXXIV);
                 Vestigium  t;
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);
                       i32  i;

        chorda_aedificator_appendere_literis(a, "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"c\"\r\nX-Longum: ");
        per (i = ZEPHYRUM; i < VIII * MXXIV; i++)
        {
            chorda_aedificator_appendere_literis(a, "a");
        }
        chorda_aedificator_appendere_literis(a, "\r\n\r\nv\r\n--" LIMES
            "--\r\n");
        {
            chorda b = chorda_aedificator_finire(a);

            CREDO_FALSUM(multipartita_adicere(m, b.datum, b.mensura));
            CREDO_VERUM(_continet(multipartita_error(m),
                "caput partis"));
            CREDO_AEQUALIS_I32(t.initia, ZEPHYRUM);
        }
    }
    /* campus (non filum) super 64 KB; filum aeque magnum licet */
    {
        ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
                                   MXXIV);
        ChordaAedificator* f = chorda_aedificator_creare(c->piscina,
                                   MXXIV);
                 Vestigium  t;
                 Vestigium  u;
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);
        MultipartitaParser* n = _parser(c->piscina, &u, LIMES, NIHIL);
                       i32  i;

        chorda_aedificator_appendere_literis(a, "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"c\"\r\n\r\n");
        chorda_aedificator_appendere_literis(f, "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"c\"; "
            "filename=\"f\"\r\n\r\n");
        per (i = ZEPHYRUM; i <= LXIV * MXXIV; i++)
        {
            chorda_aedificator_appendere_literis(a, "a");
            chorda_aedificator_appendere_literis(f, "a");
        }
        chorda_aedificator_appendere_literis(a,
            "\r\n--" LIMES "--\r\n");
        chorda_aedificator_appendere_literis(f,
            "\r\n--" LIMES "--\r\n");
        {
            chorda b = chorda_aedificator_finire(a);
            chorda g = chorda_aedificator_finire(f);

            CREDO_FALSUM(multipartita_adicere(m, b.datum, b.mensura));
            CREDO_VERUM(_continet(multipartita_error(m), "campus"));
            CREDO_VERUM(multipartita_adicere(n, g.datum, g.mensura));
            CREDO_VERUM(multipartita_finire(n));
        }
    }
    /* limes finalis absens */
    {
        constans character* b =
            "--" LIMES "\r\n"
            "Content-Disposition: form-data; name=\"c\"\r\n"
            "\r\n"
            "v\r\n"
            "--" LIMES "\r\n";
                 Vestigium  t;
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);

        CREDO_VERUM(multipartita_adicere(m, (constans i8*)b,
            (i32)strlen(b)));
        CREDO_FALSUM(multipartita_finire(m));
        CREDO_VERUM(_continet(multipartita_error(m), "finis"));
    }
    /* Content-Disposition absens */
    {
        constans character* b =
            "--" LIMES "\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "v\r\n"
            "--" LIMES "--\r\n";
                 Vestigium  t;
        MultipartitaParser* m = _parser(c->piscina, &t, LIMES, NIHIL);

        CREDO_FALSUM(multipartita_adicere(m, (constans i8*)b,
            (i32)strlen(b)));
        CREDO_VERUM(_continet(multipartita_error(m),
            "Content-Disposition"));
    }
}

interior vacuum
_sectio_limes (CredoContextus* c)
{
                Piscina* p = c->piscina;
                 chorda  l;
              character  prolixus[CXXVIII];
    MultipartitaReditus  r;

    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "multipart/form-data; boundary=abc", p), p);
    CREDO_VERUM(_aequalis(l, "abc"));
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "multipart/form-data; charset=utf-8; boundary=\"a b:c\"", p),
        p);
    CREDO_VERUM(_aequalis(l, "a b:c"));
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "Multipart/Form-Data;BOUNDARY=----WebKitFormBoundaryQ7", p), p);
    CREDO_VERUM(_aequalis(l, "----WebKitFormBoundaryQ7"));
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "multipart/form-data", p), p);
    CREDO_AEQUALIS_I32(l.mensura, ZEPHYRUM);
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "text/plain; boundary=abc", p), p);
    CREDO_AEQUALIS_I32(l.mensura, ZEPHYRUM);
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "multipart/form-data; boundary=", p), p);
    CREDO_AEQUALIS_I32(l.mensura, ZEPHYRUM);
    l = multipartita_limes_ex_typo(chorda_ex_literis(
        "multipart/form-data; boundary=a<b", p), p);
    CREDO_AEQUALIS_I32(l.mensura, ZEPHYRUM);

    /* LXX signa licent, LXXI non */
    strcpy(prolixus, "multipart/form-data; boundary=");
    memset(prolixus + XXX, 'q', LXX);
    prolixus[XXX + LXX] = '\0';
    l = multipartita_limes_ex_typo(chorda_ex_literis(prolixus, p), p);
    CREDO_AEQUALIS_I32(l.mensura, LXX);
    prolixus[XXX + LXX] = 'q';
    prolixus[XXX + LXXI] = '\0';
    l = multipartita_limes_ex_typo(chorda_ex_literis(prolixus, p), p);
    CREDO_AEQUALIS_I32(l.mensura, ZEPHYRUM);

    memset(&r, ZEPHYRUM, magnitudo(r));
    CREDO_NIHIL(multipartita_creare(chorda_ex_literis("", p), NIHIL, r,
        p));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "simplex", _sectio_simplex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secta", _sectio_secta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "custodia_regionis", _sectio_custodia_regionis, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "navigator", _sectio_navigator, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _sectio_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limes", _sectio_limes, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("multipartita", SECTIONES);
}
