/* multipart_credo_drafts.c - schedae sectionum credo pro inventis
 * multipart (recensio sceptica). Omnes hodie FRANGI debent. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "multipart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* boundary ex "multipart/form-data; boundary=" (XXX litterae) */
interior chorda
_boundary_ex (
    MultipartAedificator* aed)
{
    chorda ct;

    ct = multipart_content_type(aed);
    redde chorda_sectio(ct, XXX, ct.mensura);
}

/* quoties "--<B>" in corpore occurrit */
interior i32
_delimitatores_numerare (
          chorda  corpus,
          chorda  boundary,
        Piscina*  p)
{
    redde chorda_numerare_occurrentia(corpus,
        chorda_concatenare(chorda_ex_literis("--", p), boundary, p));
}

/* ---- I: valor qui boundary continet partem falsam fingere non potest */
interior vacuum
_probare_boundary_in_valore (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  b0;
                  chorda  b1;
                  chorda  corpus;
                  chorda  falsum;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    b0 = _boundary_ex(aed);
    falsum = chorda_concatenare(chorda_ex_literis("hello\r\n--", c->piscina),
        chorda_concatenare(b0, chorda_ex_literis(
            "\r\nContent-Disposition: form-data; name=\"admin\"\r\n\r\ntrue",
            c->piscina), c->piscina), c->piscina);
    multipart_addere_textum(aed, "comment", falsum);
    corpus = multipart_corpus_finire(aed);
    b1 = _boundary_ex(aed);   /* boundary FINALIS (mutari licet) */
    /* aperiens + claudens solum: valor boundary finalem non continet */
    CREDO_AEQUALIS_I32(_delimitatores_numerare(corpus, b1, c->piscina), II);
}

/* ---- II: duo aedificatores eodem secundo boundary distinctos habent */
interior vacuum
_probare_boundary_distincti (
    CredoContextus* c)
{
    MultipartAedificator* a;
    MultipartAedificator* b;

    a = multipart_creare(c->piscina);
    b = multipart_creare(c->piscina);
    CREDO_NECESSE(a != NIHIL && b != NIHIL);
    CREDO_FALSUM(chorda_aequalis(_boundary_ex(a), _boundary_ex(b)));
}

/* ---- III: boundary entropiam plenam habet (non XXV 'a' fixae) */
interior vacuum
_probare_boundary_entropia (
    CredoContextus* c)
{
    MultipartAedificator* a;
                  chorda  b;

    a = multipart_creare(c->piscina);
    CREDO_NECESSE(a != NIHIL);
    b = _boundary_ex(a);
    CREDO_AEQUALIS_I32(b.mensura, XXXII);
    CREDO_FALSUM(chorda_terminatur(b,
        chorda_ex_literis("aaaaaaaaaaaaaaaaaaaaaaaaa", c->piscina)));
}

/* ---- IV: corpus formae A ut fasciculus in forma B: B tres delimitatores */
interior vacuum
_probare_forma_in_forma (
    CredoContextus* c)
{
    MultipartAedificator* a;
    MultipartAedificator* b;
                  chorda  corpus_a;
                  chorda  corpus_b;

    a = multipart_creare(c->piscina);
    b = multipart_creare(c->piscina);
    CREDO_NECESSE(a != NIHIL && b != NIHIL);
    multipart_addere_textum(a, "inner", chorda_ex_literis("secret",
        c->piscina));
    corpus_a = multipart_corpus_finire(a);
    multipart_addere_textum(b, "outer", chorda_ex_literis("x", c->piscina));
    multipart_addere_fasciculum(b, "file", "form.txt", "text/plain",
        corpus_a.datum, corpus_a.mensura);
    corpus_b = multipart_corpus_finire(b);
    /* II aperientes + I claudens; corpus_a intra nullum fert */
    CREDO_AEQUALIS_I32(_delimitatores_numerare(corpus_b, _boundary_ex(b),
        c->piscina), III);
}

/* ---- V: '"' in nomine campi partem fasciculi fingere non potest */
interior vacuum
_probare_nomen_cum_virgulis (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    multipart_addere_textum(aed, "x\"; filename=\"evil.exe",
        chorda_ex_literis("v", c->piscina));
    corpus = multipart_corpus_finire(aed);
    CREDO_FALSUM(chorda_continet(corpus, chorda_ex_literis(
        "name=\"x\"; filename=\"evil.exe\"", c->piscina)));
}

/* ---- VI: CRLF in nomine fasciculi caput novum non incipit */
interior vacuum
_probare_crlf_in_nomine_fasciculi (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    multipart_addere_fasciculum(aed, "file", "a\r\nContent-Type: text/html",
        "image/png", (constans i8*)"DATA", IV);
    corpus = multipart_corpus_finire(aed);
    CREDO_FALSUM(chorda_continet(corpus, chorda_ex_literis(
        "\r\nContent-Type: text/html", c->piscina)));
    CREDO_MINOR_AUT_AEQUALIS_I32(chorda_numerare_occurrentia(corpus,
        chorda_ex_literis("Content-Type:", c->piscina)), I);
}

/* ---- VII: CRLF in content_type caput non addit nec data praefigit */
interior vacuum
_probare_crlf_in_content_type (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    multipart_addere_fasciculum(aed, "file", "a.png",
        "image/png\r\nX-Injected: 1", (constans i8*)"DATA", IV);
    corpus = multipart_corpus_finire(aed);
    CREDO_FALSUM(chorda_continet(corpus, chorda_ex_literis(
        "\r\nX-Injected: 1", c->piscina)));

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    multipart_addere_fasciculum(aed, "file", "a.png",
        "image/png\r\n\r\nPREFIX", (constans i8*)"DATA", IV);
    corpus = multipart_corpus_finire(aed);
    CREDO_FALSUM(chorda_continet(corpus, chorda_ex_literis(
        "\r\n\r\nPREFIX", c->piscina)));
}

/* ---- VIII: datum ad tempus additionis copiatur (non indicatur) */
interior vacuum
_probare_datum_copiatum (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;
                  chorda  val;
               character  alveus[VIII];

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    memcpy(alveus, "ORIGINAL", VIII);
    multipart_addere_fasciculum(aed, "f", "f.bin", "text/plain",
        (constans i8*)alveus, VIII);
    val.datum    = (i8*)alveus;
    val.mensura  = VIII;
    multipart_addere_textum(aed, "t", val);
    memcpy(alveus, "CHANGED!", VIII);
    corpus = multipart_corpus_finire(aed);
    CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(corpus,
        chorda_ex_literis("ORIGINAL", c->piscina)), II);
    CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(corpus,
        chorda_ex_literis("CHANGED!", c->piscina)), ZEPHYRUM);
}

/* ---- IX: datum NIHIL cum mensura > 0 reicitur, finire non ruit */
interior vacuum
_finire_datum_nihil (
    vacuum)
{
               Piscina* p;
    MultipartAedificator* aed;

    p = piscina_generare_dynamicum("datum_nihil", M * LXIV);
    aed = multipart_creare(p);
    multipart_addere_fasciculum(aed, "f", "f.bin", "text/plain", NIHIL, V);
    (vacuum)multipart_corpus_finire(aed);
    piscina_destruere(p);
}

interior vacuum
_probare_datum_nihil (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_RUIT(_finire_datum_nihil());
}

/* ---- X: plus quam XXXII partes non silenter perduntur */
interior vacuum
_probare_campi_ultra_xxxii (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;
               character  titulus[XVI];
                     i32  k;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    per (k = ZEPHYRUM; k < XL; k++)
    {
        sprintf(titulus, "f%02u", (insignatus integer)k);
        multipart_addere_textum(aed, titulus, chorda_ex_literis("v",
            c->piscina));
    }
    corpus = multipart_corpus_finire(aed);
    CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(corpus,
        chorda_ex_literis("Content-Disposition", c->piscina)), XL);
    CREDO_CHORDA_CONTINET(corpus, chorda_ex_literis("name=\"f39\"",
        c->piscina));
}

/* ---- XI/XII: piscina certa: NIHIL / corpus, non exitus processus.
 * Filius SIGUSR1 mittit SI eventus rectus est: exitus(1) piscinae
 * fractae signum non habet, ergo CREDO_RUIT_CUM tunc frangitur. */
interior vacuum
_creare_in_piscina_parva (
    vacuum)
{
    Piscina* p;

    p = piscina_generare_certae_magnitudinis("certa", CCLVI);
    si (multipart_creare(p) == NIHIL)
    {
        raise(SIGUSR1);
    }
}

interior vacuum
_finire_in_piscina_certa (
    vacuum)
{
               Piscina* p;
    MultipartAedificator* aed;
                  chorda  corpus;

    p = piscina_generare_certae_magnitudinis("certa", M * IV + CC);
    aed = multipart_creare(p);
    si (aed == NIHIL)
    {
        redde;
    }
    multipart_addere_textum(aed, "t", chorda_ex_literis("val", p));
    corpus = multipart_corpus_finire(aed);
    si (corpus.mensura == CXXIII)
    {
        raise(SIGUSR1);
    }
}

interior vacuum
_probare_piscina_certa (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_RUIT_CUM(_creare_in_piscina_parva(), SIGUSR1);
    CREDO_RUIT_CUM(_finire_in_piscina_certa(), SIGUSR1);
}

/* ---- XIII: fasciculus sine nomine fasciculi data non perdit */
interior vacuum
_probare_fasciculus_sine_nomine (
    CredoContextus* c)
{
    MultipartAedificator* aed;
                  chorda  corpus;

    aed = multipart_creare(c->piscina);
    CREDO_NECESSE(aed != NIHIL);
    multipart_addere_fasciculum(aed, "file", NIHIL, "image/png",
        (constans i8*)"PNGDATA", VII);
    corpus = multipart_corpus_finire(aed);
    CREDO_CHORDA_CONTINET(corpus, chorda_ex_literis("PNGDATA", c->piscina));
    CREDO_CHORDA_CONTINET(corpus, chorda_ex_literis("name=\"file\"",
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "boundary_in_valore", _probare_boundary_in_valore, NIHIL, NIHIL, NIHIL, NIHIL },
    { "boundary_distincti", _probare_boundary_distincti, NIHIL, NIHIL, NIHIL, NIHIL },
    { "boundary_entropia", _probare_boundary_entropia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma_in_forma", _probare_forma_in_forma, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomen_cum_virgulis", _probare_nomen_cum_virgulis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "crlf_nomen_fasciculi", _probare_crlf_in_nomine_fasciculi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "crlf_content_type", _probare_crlf_in_content_type, NIHIL, NIHIL, NIHIL, NIHIL },
    { "datum_copiatum", _probare_datum_copiatum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "datum_nihil", _probare_datum_nihil, NIHIL, NIHIL, NIHIL, NIHIL },
    { "campi_ultra_xxxii", _probare_campi_ultra_xxxii, NIHIL, NIHIL, NIHIL, NIHIL },
    { "piscina_certa", _probare_piscina_certa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasciculus_sine_nomine", _probare_fasciculus_sine_nomine, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("multipart_schedae", SECTIONES);
}
