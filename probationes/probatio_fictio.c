/* probatio_fictio.c - fictio: determinismus, fines, formae, corpus,
 * scrinium difficilium (norma-plan-2 N1) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "sors.h"
#include "utf8.h"
#include "fasti.h"
#include "fictio.h"

#include <stdio.h>
#include <string.h>

interior b32
_utf8_validum (
    chorda c)
{
    constans i8* p      = c.datum;
    constans i8* finis  = c.datum + c.mensura;

    dum (p < finis)
    {
        si (utf8_decodere(&p, finis) < 0)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
       Sors  a;
       Sors  b;

    piscina = c->piscina;
    sors_seminare(&a, XLII, 0);
    sors_seminare(&b, XLII, 0);
    CREDO_CHORDA_AEQUALIS(fictio_nomen(&a, piscina), fictio_nomen(&b,
        piscina));
    CREDO_CHORDA_AEQUALIS(fictio_email(&a, piscina), fictio_email(&b,
        piscina));
    CREDO_CHORDA_AEQUALIS(fictio_uuid(&a, piscina), fictio_uuid(&b,
        piscina));
    CREDO_CHORDA_AEQUALIS(fictio_textus_latinus(&a, III, piscina),
                          fictio_textus_latinus(&b, III, piscina));
    CREDO_CHORDA_AEQUALIS(fictio_textus_difficilis(&a, piscina),
                          fictio_textus_difficilis(&b, piscina));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Sors s;
     i32 i;
     b32 imum    = FALSUM;
     b32 summum  = FALSUM;

    (vacuum)c;
    sors_seminare(&s, VII, 0);
    per (i = 0; i < M; i++)
    {
        s64 n = fictio_integer(&s, -V, V);
        f64 f = fictio_numerus(&s, 0.5, 2.5);

        CREDO_VERUM(n >= -V && n <= V);
        CREDO_VERUM(f >= 0.5 && f < 2.5);
        si (n == -V) imum = VERUM;
        si (n == V)  summum = VERUM;
    }
    CREDO_VERUM(imum && summum);
    /* minimum > maximum -> minimum; spatium totum s64 sine ruina */
    CREDO_AEQUALIS_S64(fictio_integer(&s, IX, III), IX);
    CREDO_NON_RUIT((vacuum)fictio_integer(&s, (s64)((i64)I << LXIII),
                                             (s64)(((i64)I << LXIII)
                                                 - I)));
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
     Piscina* piscina;
        Sors  s;
         i32  i;
    DiesHora  dh;

    piscina = c->piscina;
    sors_seminare(&s, XIII, 0);
    per (i = 0; i < C; i++)
    {
        chorda e = fictio_email(&s, piscina);
        chorda u = fictio_uuid(&s, piscina);
        chorda t = fictio_tempus(&s, 0, (s64)4102444800, piscina);
        chorda x = fictio_textus(&s, II, V,
            "\xce\xb1\xce\xb2\xce\xb3", piscina);   /* alpha beta gamma */
        s32 runae = utf8_numerare_runas(x.datum, (s32)x.mensura);
        i32 k;
        i32 ad = 0;

        per (k = 0; k < e.mensura; k++)
        {
            si (e.datum[k] == '@') ad++;
        }
        CREDO_AEQUALIS_I32(ad, I);
        CREDO_VERUM(chorda_terminatur(e,
            chorda_ex_literis("@example.org", piscina))
                 || chorda_terminatur(e,
                 chorda_ex_literis("@example.com", piscina))
                 || chorda_terminatur(e,
                 chorda_ex_literis("@example.net", piscina)));
        CREDO_AEQUALIS_I32(u.mensura, XXXVI);
        CREDO_VERUM(u.datum[VIII] == '-' && u.datum[XIII] == '-'
                 && u.datum[XVIII] == '-' && u.datum[XXIII] == '-');
        CREDO_VERUM(u.datum[XIV] == '4');
        CREDO_VERUM(strchr("89ab", u.datum[XIX]) != NIHIL);
        CREDO_VERUM(fasti_ex_iso(t, &dh));
        CREDO_VERUM(runae >= II && runae <= V);
    }
    /* epocha fixa */
    CREDO_CHORDA_AEQUALIS_LITERIS(fictio_tempus(&s, 0, 0, piscina),
                                  "1970-01-01T00:00:00Z");
    CREDO_CHORDA_AEQUALIS_LITERIS(fictio_tempus(&s, 951782400,
        951782400, piscina),
                                  "2000-02-29T00:00:00Z");
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
       Sors  s;
     chorda  l;
        i32  i;
        i32  fines = 0;

    piscina = c->piscina;
    sors_seminare(&s, III, 0);
    l = fictio_textus_latinus(&s, III, piscina);
    per (i = 0; i < l.mensura; i++)
    {
        si (l.datum[i] == '.' || l.datum[i] == '?' || l.datum[i] == '!')
        {
            fines++;
        }
    }
    CREDO_MAIOR_AUT_AEQUALIS_I32(fines, III);
    CREDO_MAIOR_AUT_AEQUALIS_I32(fictio_difficilia_numerus(), IX);
    per (i = 0; i < fictio_difficilia_numerus(); i++)
    {
        CREDO_VERUM(_utf8_validum(fictio_difficile(i, piscina)));
    }
    /* scrinium continet U+0000 et chordam vacuam */
    {
        b32 nul    = FALSUM;
        b32 vacua  = FALSUM;

        per (i = 0; i < fictio_difficilia_numerus(); i++)
        {
            chorda d = fictio_difficile(i, piscina);

            si (d.mensura == 0) vacua = VERUM;
            si (   d.mensura > 0
                && memchr(d.datum,
                                                                                                                          0,
                                                                                                                                                     (size_t)d.mensura)) nul =
                                                                                                                                                                             VERUM;
        }
        CREDO_VERUM(nul && vacua);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "determinismus", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fictio_integer / fictio_numerus", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "email, uuid, tempus, textus", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "corpus Latinum et scrinium difficilium", _sectio_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fictio", SECTIONES);
}
