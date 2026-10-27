/* probatio_recursus.c - schedula naturalia (pensa K1): corpus
 * lib/recursus.corpus.stml - exemplum legitur, forma canonica, dies in
 * fenestra, iter (forma canonica iterum lecta eadem); vitium recusatur
 * cum initio vitii dato. Sectio 'api' (Task 6): proximus, continet,
 * NIHIL, fenestra inversa, phrasis longa. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include "filum.h"
#include "fasti.h"
#include "recursus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_CORPORIS "lib/recursus.corpus.stml"

/* dies regulae in [ab, ad] ut "YYYY-MM-DD YYYY-MM-DD" */
interior chorda
dies_scribere (
    constans Recursus* r,
               chorda  ab,
               chorda  ad,
              Piscina* p)
{
                 Dies  prima;
                 Dies  ultima;
                  Xar* l;
    ChordaAedificator* a;
                  i32  i;

    l = xar_creare(p, (i32)magnitudo(Dies));
    a = chorda_aedificator_creare(p, (memoriae_index)CXXVIII);
    si (   !fasti_parsare_iso(ab, &prima)
        || !fasti_parsare_iso(ad, &ultima))
    {
        redde chorda_ex_literis("fenestra mala", p);
    }
    (vacuum)recursus_dies(r, prima, ultima, l);
    per (i = ZEPHYRUM; i < xar_numerus(l); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        chorda_aedificator_appendere_chorda(a, fasti_formare_diem(
            *(Dies*)xar_obtinere(l, i), FASTI_FORMA_ISO, p));
    }
    redde chorda_aedificator_finire(a);
}

interior b32
aequalis (
     constans character* quid,
                 chorda  receptum,
                 chorda  speratum)
{
    si (chorda_aequalis(receptum, speratum))
    {
        redde VERUM;
    }
    fprintf(stderr, "  %s: '%.*s' (speratum '%.*s')\n", quid,
        (int)receptum.mensura, (constans character*)receptum.datum,
        (int)speratum.mensura, (constans character*)speratum.datum);
    redde FALSUM;
}

/* exemplum: legere, canon, dies; forma canonica iterum: eadem */
interior b32
exemplum_probare (
    StmlNodus* n,
      Piscina* p)
{
        chorda* phrasis;
        chorda* canon;
        chorda* ab;
        chorda* ad;
        chorda* dies;
      Recursus* r;
      Recursus* iterum;
        chorda  vitium;
        chorda  c;

    phrasis  = stml_attributum_capere(n, "phrasis");
    canon    = stml_attributum_capere(n, "canon");
    ab       = stml_attributum_capere(n, "ab");
    ad       = stml_attributum_capere(n, "ad");
    dies     = stml_attributum_capere(n, "dies");
    si (!phrasis || !canon || !ab || !ad || !dies)
    {
        fprintf(stderr, "  exemplum sine attributo\n");
        redde FALSUM;
    }
    r = recursus_legere(*phrasis, p, &vitium);
    si (!r)
    {
        fprintf(stderr, "  '%.*s' recusata: %.*s\n",
            (int)phrasis->mensura,
            (constans character*)phrasis->datum, (int)vitium.mensura,
            (constans character*)vitium.datum);
        redde FALSUM;
    }
    c = recursus_scribere(r, p);
    si (   !aequalis("canon", c, *canon)
        || !aequalis("dies", dies_scribere(r, *ab, *ad, p), *dies))
    {
        redde FALSUM;
    }
    iterum = recursus_legere(c, p, &vitium);
    si (!iterum)
    {
        fprintf(stderr, "  canon '%.*s' non legitur: %.*s\n",
            (int)c.mensura, (constans character*)c.datum,
            (int)vitium.mensura, (constans character*)vitium.datum);
        redde FALSUM;
    }
    redde aequalis("canon iterum", recursus_scribere(iterum, p), *canon)
        && aequalis("dies iterum", dies_scribere(iterum, *ab, *ad, p),
               *dies);
}

/* vitium: recusata, vitium incipit textu dato */
interior b32
vitium_probare (
    StmlNodus* n,
      Piscina* p)
{
     chorda* phrasis;
     chorda* incipit;
     chorda  vitium;

    phrasis = stml_attributum_capere(n, "phrasis");
    incipit = stml_attributum_capere(n, "incipit");
    si (!phrasis || !incipit)
    {
        fprintf(stderr, "  vitium sine attributo\n");
        redde FALSUM;
    }
    si (recursus_legere(*phrasis, p, &vitium))
    {
        fprintf(stderr, "  '%.*s' accepta (recusanda)\n",
            (int)phrasis->mensura, (constans character*)phrasis->datum);
        redde FALSUM;
    }
    si (   vitium.mensura < incipit->mensura
        || memcmp(vitium.datum, incipit->datum,
        (size_t)incipit->mensura)
           != ZEPHYRUM)
    {
        fprintf(stderr, "  vitium '%.*s' (incipere debet '%.*s')\n",
            (int)vitium.mensura, (constans character*)vitium.datum,
            (int)incipit->mensura, (constans character*)incipit->datum);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
    InternamentumChorda* intern;
                 chorda  textus;
           StmlResultus  res;
              StmlNodus* n;
                    i32  i;
                    i32  exempla;
                    i32  vitia;

    intern = internamentum_creare(c->piscina);
    textus = filum_legere_totum(VIA_CORPORIS, c->piscina);
    CREDO_NECESSE(textus.mensura > ZEPHYRUM);
    res = stml_legere_ex_literis(chorda_ut_cstr(textus, c->piscina),
        c->piscina, intern);
    CREDO_NECESSE(res.successus && res.elementum_radix);
    exempla  = ZEPHYRUM;
    vitia    = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(res.elementum_radix);
         i++)
    {
        n = stml_liberum_ad_indicem(res.elementum_radix, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        CREDO_GRADUS(chorda_ut_cstr(*stml_attributum_capere(n,
            "phrasis"),
            c->piscina));
        si (chorda_aequalis_literis(*n->titulus, "exemplum"))
        {
            exempla++;
            CREDO_VERUM(exemplum_probare(n, c->piscina));
        }
        alioquin si (chorda_aequalis_literis(*n->titulus, "vitium"))
        {
            vitia++;
            CREDO_VERUM(vitium_probare(n, c->piscina));
        }
        alioquin
        {
            fprintf(stderr, "  elementum ignotum in corpore\n");
            CREDO_VERUM(FALSUM);
        }
    }
    /* porta tacita = porta mortua: corpus vere legitur */
    CREDO_VERUM(exempla > ZEPHYRUM);
    CREDO_VERUM(vitia > ZEPHYRUM);
}

interior Recursus*
regula (
    constans character* phrasis,
               Piscina* p)
{
    redde recursus_legere(chorda_ex_literis(phrasis, p), p, NIHIL);
}

interior b32
dies_est (
                  Dies  d,
    constans character* iso,
               Piscina* p)
{
    redde chorda_aequalis_literis(fasti_formare_diem(d, FASTI_FORMA_ISO,
        p), iso);
}

interior vacuum
_sectio_api (
    CredoContextus* c)
{
     Recursus* r;
         Dies  d;
          Xar* l;
       chorda  vitium;
    character  longa[CCC];
          i32  i;

    CREDO_GRADUS("proximus: post ancoram, ante ancoram, semel praeteritum");
    r = regula("every other monday from oct 26 2026", c->piscina);
    CREDO_NECESSE(r != NIHIL);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXVII), &d));
    CREDO_VERUM(dies_est(d, "2026-11-09", c->piscina));
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, I), &d));
    CREDO_VERUM(dies_est(d, "2026-10-26", c->piscina));
    r = regula("2026-10-30", c->piscina);
    CREDO_FALSUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXXI), &d));

    CREDO_GRADUS("ancora longinqua: phasis servatur");
    r = regula("every other monday from jan 1 1990", c->piscina);
    CREDO_NECESSE(r != NIHIL);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXXVI, X, XXVI), &d));
    CREDO_VERUM(dies_est(d, "2026-11-02", c->piscina));

    CREDO_GRADUS("29 Feb trans annum 2100 (non bissextilem)");
    r = regula("every year on feb 29", c->piscina);
    CREDO_VERUM(recursus_proximus(r, fasti_dies(MMXCVII, III, I), &d));
    CREDO_VERUM(dies_est(d, "2104-02-29", c->piscina));

    CREDO_GRADUS("continet");
    r = regula("weekends", c->piscina);
    CREDO_VERUM(recursus_continet(r, fasti_dies(MMXXVI, X, XXIV)));
    CREDO_FALSUM(recursus_continet(r, fasti_dies(MMXXVI, X, XXVI)));
    CREDO_FALSUM(recursus_continet(r, fasti_dies(MMXXVI, II, XXIX)));

    CREDO_GRADUS("NIHIL et fenestra inversa");
    l = xar_creare(c->piscina, (i32)magnitudo(Dies));
    CREDO_FALSUM(recursus_continet(NIHIL, fasti_dies(MMXXVI, X, XXIV)));
    CREDO_AEQUALIS_I32(recursus_dies(NIHIL, fasti_dies(MMXXVI, X, I),
        fasti_dies(MMXXVI, X, XXXI), l), ZEPHYRUM);
    CREDO_AEQUALIS_I32(recursus_dies(r, fasti_dies(MMXXVI, X, XXXI),
        fasti_dies(MMXXVI, X, I), l), ZEPHYRUM);
    CREDO_FALSUM(recursus_proximus(NIHIL, fasti_dies(MMXXVI, X, I),
        &d));
    CREDO_VERUM(recursus_scribere(NIHIL, c->piscina).mensura
        == ZEPHYRUM);
    CREDO_VERUM(recursus_legere(chorda_ex_literis("", c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(chorda_aequalis_literis(vitium, "empty schedule"));
    CREDO_VERUM(recursus_legere(chorda_ex_literis("daily", c->piscina),
        NIHIL, &vitium) == NIHIL);

    CREDO_GRADUS("phrasis longa: vitium nominatum, nullus exitus");
    memset(longa, ZEPHYRUM, magnitudo(longa));
    per (i = ZEPHYRUM; i < LXX; i++)
    {
        strcat(longa, "mon ");
    }
    CREDO_VERUM(recursus_legere(chorda_ex_literis(longa, c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(strstr(chorda_ut_cstr(vitium, c->piscina),
        "schedule too long") != NIHIL);
    memset(longa, 'x', XL);
    longa[XL] = '\0';
    CREDO_VERUM(recursus_legere(chorda_ex_literis(longa, c->piscina),
        c->piscina, &vitium) == NIHIL);
    CREDO_VERUM(strstr(chorda_ut_cstr(vitium, c->piscina),
        "word too long") != NIHIL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "corpus", _sectio_corpus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "api", _sectio_api, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("recursus", SECTIONES);
}
