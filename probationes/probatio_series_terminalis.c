/* probatio_series_terminalis.c - lexemator fluminis terminalis
 * (eventus B1a): probationes Ghostty Parser.zig translatae (pin
 * 12752b2), divergentiae consultae, scissurae omnes (integrum,
 * bipartitum
 * ubique, octetum per octetum), initus hostilis (memoria finita). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "sors.h"
#include "series_terminalis.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* tituli_generum[] = {
    "NIHIL", "IMP", "EXS", "ESC", "CSI", "SS", "OSC", "DCS", "APC",
        "FUGA"
};

interior vacuum
_octetum (
    ChordaAedificator* a,
                   i8  c)
{
    hic_manens constans character notae[]  =
        "0123456789abcdef";
                              i32 u       =
                                  ((i32)c) & 0xFF;

    si (u > 0x20 && u < 0x7F && u != '\\')
    {
        chorda_aedificator_appendere_character(a, (character)u);
        redde;
    }
    chorda_aedificator_appendere_literis(a, "\\x");
    chorda_aedificator_appendere_character(a, notae[u
        >> IV]);
    chorda_aedificator_appendere_character(a,
        notae[u & 0xF]);
}

interior vacuum
_octetos (
    ChordaAedificator* a,
               chorda  c)
{
    i32 k;

    per (k = ZEPHYRUM; k < c.mensura; k++)
    {
        _octetum(a, c.datum[k]);
    }
}

interior vacuum
_lexema_reddere (
        ChordaAedificator* a,
    constans SeriesLexema* l,
                      b32  cum_crudo)
{
    i32 k;

    chorda_aedificator_appendere_literis(a, tituli_generum[l->genus]);
    si (l->introductor)
    {
        chorda_aedificator_appendere_literis(a, " i=");
        _octetum(a, l->introductor);
    }
    si (l->privatum)
    {
        chorda_aedificator_appendere_literis(a, " p=");
        _octetum(a, l->privatum);
    }
    si (l->numerus_parametrorum > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " P=");
        per (k = ZEPHYRUM; k < l->numerus_parametrorum; k++)
        {
            si (k > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a,
                    (l->separatores & ((i32)I << (k - I))) ? ':' : ';');
            }
            chorda_aedificator_appendere_s32(a, l->parametra[k]);
        }
    }
    si (l->numerus_intermediorum > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " I=");
        per (k = ZEPHYRUM; k < l->numerus_intermediorum; k++)
        {
            _octetum(a, l->intermedia[k]);
        }
    }
    si (l->finale)
    {
        chorda_aedificator_appendere_literis(a, " f=");
        _octetum(a, l->finale);
    }
    si (l->praefixum)
    {
        chorda_aedificator_appendere_literis(a, " PRAE");
    }
    si (   l->genus == SERIES_IMPRIMERE || l->genus == SERIES_OSC
        || l->genus == SERIES_DCS || l->genus == SERIES_APC)
    {
        chorda_aedificator_appendere_literis(a, " T=");
        _octetos(a, l->textus);
    }
    si (cum_crudo || l->genus == SERIES_FUGA)
    {
        chorda_aedificator_appendere_literis(a, " C=");
        _octetos(a, l->crudum);
    }
    si (l->truncatum)
    {
        chorda_aedificator_appendere_literis(a, " TRUNC");
    }
}

/* scissio: 0 integrum; > 0 bipartitum ibi; -1 octetum per octetum.
 * Cursus IMPRIMERE contigui iunguntur (scissura eos dividit). */
interior chorda
_fluxus (
       SeriesLector* lx,
        constans i8* fons,
                i32  n,
                s32  scissio,
                b32  cum_crudo,
            Piscina* p)
{
     ChordaAedificator* a;
     ChordaAedificator* cursus;
          SeriesLexema  l;
                   b32  primus = VERUM;
                   i32  ab;
                   i32  ad;

    a       = chorda_aedificator_creare(p, CCLVI);
    cursus  = chorda_aedificator_creare(p, LXIV);
    series_lectorem_purgare(lx);
    ab = ZEPHYRUM;
    dum (ab < n)
    {
        constans i8* ptr;
        constans i8* finis;

        ad = (scissio == ZEPHYRUM) ? n
            : (scissio < ZEPHYRUM) ? ab + I
            : (ab < (i32)scissio) ? (i32)scissio : n;
        ptr    = fons + ab;
        finis  = fons + ad;
        dum (ptr < finis)
        {
            SeriesGenus g = series_lexema_proximum(lx, &ptr, finis, &l);

            si (g == SERIES_NIHIL)
            {
                frange;
            }
            si (g == SERIES_IMPRIMERE)
            {
                chorda_aedificator_appendere_chorda(cursus, l.textus);
                perge;
            }
            si (chorda_aedificator_longitudo(cursus) > ZEPHYRUM)
            {
                si (!primus)
                {
                    chorda_aedificator_appendere_literis(a, " | ");
                }
                primus = FALSUM;
                chorda_aedificator_appendere_literis(a, "IMP T=");
                _octetos(a, chorda_aedificator_spectare(cursus));
                chorda_aedificator_reset(cursus);
            }
            si (!primus)
            {
                chorda_aedificator_appendere_literis(a, " | ");
            }
            primus = FALSUM;
            _lexema_reddere(a, &l, cum_crudo);
        }
        ab = ad;
    }
    si (chorda_aedificator_longitudo(cursus) > ZEPHYRUM)
    {
        si (!primus)
        {
            chorda_aedificator_appendere_literis(a, " | ");
        }
        chorda_aedificator_appendere_literis(a, "IMP T=");
        _octetos(a, chorda_aedificator_spectare(cursus));
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_videre (
          SeriesLector* lx,
               Piscina* p,
    constans character* fons,
    constans character* exspectatum)
{
    chorda r;

    r = _fluxus(lx, (constans i8*)fons, (i32)strlen(fons), ZEPHYRUM,
        FALSUM, p);
    si (!chorda_aequalis_literis(r, exspectatum))
    {
        imprimere("  fons:        (%u octeti)\n  exspectatum: %s\n"
            "  actuale:     %.*s\n", (unsigned)strlen(fons),
            exspectatum,
            (int)r.mensura, (constans character*)r.datum);
        redde FALSUM;
    }
    redde VERUM;
}

/* fons + repetitio * n + cauda */
interior constans character*
_repetere (
    constans character* caput,
    constans character* frustum,
                   i32  n,
    constans character* cauda,
               Piscina* p)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, CCLVI);
                   i32  k;

    chorda_aedificator_appendere_literis(a, caput);
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_literis(a, frustum);
    }
    chorda_aedificator_appendere_literis(a, cauda);
    redde chorda_ut_cstr(chorda_aedificator_finire(a), p);
}

/* Corpus scissurarum: fluxus deterministici (sine mora). */
/* Corpus modi initus (B1b): responsa vera et claves alterum */
hic_manens constans character* corpus_initus[] = {
    "\033 \033!\0335",
    "\x1bNa\x1bPa\x1b]a\x07\x1b_x\x1b\\",
    "\x1b\r\x1b\x7f\x1b\x01",
    "\x1bP>|kitty(0.40.1)\x1b\\",
    "\x1b]4;0;#fff\x07y\x1b]11;rgb:0/0/0\x1b\\",
    "\x1b_Gi=1;OK\x1b\\x",
    "\x1bOA\x1b[1;5A\x1b\x1b[A"
};

hic_manens constans character* corpus[] = {
    "\x9E\x9C" "a\x19",
    "\x1b(B",
    "\x1b[H",
    "\x1b[1;4H",
    "\x1b[38:2m",
    "\x1b[48:2m\x1b[H",
    "\x1b[38:5:1;48:5:0m",
    "\x1b[48:2:240:143:104m",
    "\x1b[58:2::240:143:104m",
    "\x1b[;4:3;38;2;175;175;215;58:2::190:80:70m",
    "\x1b[38:2h",
    "\x1b[?2026$p",
    "\x1b[3 q",
    "\x1b]0;abc\x07",
    "\x1b]0;abc\x1b\\",
    "\x1bP+q544e\x1b\\",
    "\x1bP1000p\x1b\\",
    "ab\x1b[A" "cd",
    "\r\x7f\x03",
    "\x1bOA\x1bO2P",
    "\x1b\x1b[A",
    "\x1b" "a",
    "\x1b[1\x08;2H",
    "\x1b[1\x18x",
    "\x1b[1\x1b[A",
    "\x1b\xc3\xa9",
    "\x1b_Gi=1;OK\x1b\\",
    "\x1b[!\"#$%&p",
    "\x1b[99999999999A",
    "\x1b[97:65;2u",
    "\x1b[>1u",
    "\x1b^pm\x1b\\",
    "\x1b]0;t\x1b[A",
    "\x1b]0;t\x18",
    "\x1b]0;a\nb\x07",
    "\x1b_a\x07" "b\x1b\\",
    "x\x1b[<0;10;5Mhello\x1b[<0;10;5m"
};

s32 principale (vacuum)
{
          Piscina* piscina;
          Piscina* piscina_hostilis;
     SeriesLector* lx;
     SeriesLector* lector_hostilis;
     SeriesLector* li;
     SeriesLexema  l;
              i32  k;
              s32  n;
              i32  discrepantiae;

    piscina = piscina_generare_dynamicum("probatio_series_terminalis",
        M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    lx = series_lectorem_creare(piscina);
    CREDO_NON_NIHIL (lx);

    imprimere("\n--- I. Ghostty Parser.zig (translata) ---\n");
    /* C1 NON agnita (Franus): 0x9E 0x9C imprimuntur */
    CREDO_VERUM (_videre(lx, piscina, "\x9E\x9C" "a\x19",
        "IMP T=\\x9e\\x9ca | EXS f=\\x19"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b(B", "ESC I=( f=B"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[H", "CSI i=[ f=H"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[1;4H",
        "CSI i=[ P=1;4 f=H"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[38:2m",
        "CSI i=[ P=38:2 f=m"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[48:2m\x1b[H",
        "CSI i=[ P=48:2 f=m | CSI i=[ f=H"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[38:5:1;48:5:0m",
        "CSI i=[ P=38:5:1;48:5:0 f=m"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[48:2:240:143:104m",
        "CSI i=[ P=48:2:240:143:104 f=m"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[4:3m",
        "CSI i=[ P=4:3 f=m"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[58:2::240:143:104m",
        "CSI i=[ P=58:2:0:240:143:104 f=m"));
    CREDO_VERUM (_videre(lx, piscina,
        "\x1b[;4:3;38;2;175;175;215;58:2::190:80:70m",
        "CSI i=[ P=0;4:3;38;2;175;175;215;58:2:0:190:80:70 f=m"));
    CREDO_VERUM (_videre(lx, piscina,
        "\x1b[4:3;38;2;51;51;51;48;2;170;170;170;58;2;255;97;136m",
        "CSI i=[ P=4:3;38;2;51;51;51;48;2;170;170;170;58;2;255;97;136 "
        "f=m"));
    /* DIVERGENTIA: Ghostty ':' extra 'm' abicit; nos servamus */
    CREDO_VERUM (_videre(lx, piscina, "\x1b[38:2h",
        "CSI i=[ P=38:2 f=h"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[?2026$p",
        "CSI i=[ p=? P=2026 I=$ f=p"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[3 q",
        "CSI i=[ P=3 I=\\x20 f=q"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]0;abc\x07",
        "OSC i=] T=0;abc"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]0;abc\x1b\\",
        "OSC i=] T=0;abc"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]112\x07", "OSC i=] T=112"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]104\x07", "OSC i=] T=104"));
    /* parametra nimia: series tota abicitur; proximum integrum */
    CREDO_VERUM (_videre(lx, piscina,
        _repetere("\x1b[", "1;", C, "1Cx", piscina), "IMP T=x"));
    per (n = I; n <= SERIES_PARAMETRA_MAXIMA; n++)
    {
        constans character* f;
               constans i8* ptr;
               constans i8* finis;

        f      = _repetere("\x1b[", "1;", (i32)(n - I), "2H", piscina);
        ptr    = (constans i8*)f;
        finis  = ptr + strlen(f);
        series_lectorem_purgare(lx);
        CREDO_VERUM (series_lexema_proximum(lx, &ptr, finis, &l)
            == SERIES_CSI);
        CREDO_AEQUALIS_I32 (l.numerus_parametrorum, (i32)n);
        CREDO_AEQUALIS_S32 (l.parametra[n - I], II);
    }
    CREDO_VERUM (_videre(lx, piscina,
        _repetere("\x1b[", "1;", SERIES_PARAMETRA_MAXIMA + I, "2H",
            piscina), ""));
    CREDO_VERUM (_videre(lx, piscina, "\x1bP+q544e\x1b\\",
        "DCS i=P I=+ f=q T=544e"));
    CREDO_VERUM (_videre(lx, piscina, "\x1bP1000p\x1b\\",
        "DCS i=P P=1000 f=p T="));
    CREDO_VERUM (_videre(lx, piscina,
        _repetere("\x1bP6", ";", SERIES_PARAMETRA_MAXIMA,
            "7pdata\x1b\\", piscina), ""));

    imprimere("\n--- II. divergentiae consultae (initus) ---\n");
    {
        constans character* f    = "ab\x1b[A";
               constans i8* ptr  = (constans i8*)f;

        series_lectorem_purgare(lx);
        CREDO_VERUM (series_lexema_proximum(lx, &ptr,
            (constans i8*)f + V, &l) == SERIES_IMPRIMERE);
        /* visus in initum, non copia */
        CREDO_VERUM (l.textus.datum == (constans i8*)f);
        CREDO_AEQUALIS_I32 (l.textus.mensura, II);
    }
    CREDO_VERUM (_videre(lx, piscina, "ab\x1b[A",
        "IMP T=ab | CSI i=[ f=A"));
    CREDO_VERUM (_videre(lx, piscina, "\r\x7f\x03",
        "EXS f=\\x0d | EXS f=\\x7f | EXS f=\\x03"));
    CREDO_VERUM (_videre(lx, piscina, "\x1bOA", "SS i=O f=A"));
    CREDO_VERUM (_videre(lx, piscina, "\x1bO2P", "SS i=O P=2 f=P"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b\x1b[A",
        "CSI i=[ f=A PRAE"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b" "a", "ESC f=a"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[1\x08;2H",
        "EXS f=\\x08 | CSI i=[ P=1;2 f=H"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[1\x18x",
        "FUGA i=[ C=\\x1b[1 | EXS f=\\x18 | IMP T=x"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[1\x1b[A",
        "FUGA i=[ C=\\x1b[1 | CSI i=[ f=A"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b\xc3\xa9",
        "FUGA C=\\x1b | IMP T=\\xc3\\xa9"));
    /* SS + octetus altus: FUGA, octetus NON consumptus (aemulator D3 -
     * olim octeti alti in statu SS tacite peribant) */
    CREDO_VERUM (_videre(lx, piscina, "\x1bN\xc3\xa9",
        "FUGA i=N C=\\x1bN | IMP T=\\xc3\\xa9"));
    CREDO_VERUM (_videre(lx, piscina, "\x1bO2\xc3\xa9",
        "FUGA i=O C=\\x1bO2 | IMP T=\\xc3\\xa9"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b_Gi=1;OK\x1b\\",
        "APC i=_ T=Gi=1;OK"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[!\"#$%&p",
        "CSI i=[ I=!\"#$ f=p TRUNC"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[99999999999A",
        "CSI i=[ P=2147483647 f=A"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[97:65;2u",
        "CSI i=[ P=97:65;2 f=u"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b[>1u",
        "CSI i=[ p=> P=1 f=u"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b^pm\x1b\\", "APC i=^ T=pm"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]0;t\x1b[A",
        "OSC i=] T=0;t | CSI i=[ f=A"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]0;t\x18",
        "FUGA i=] C=\\x1b]0;t | EXS f=\\x18"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b]0;a\nb\x07",
        "OSC i=] T=0;ab"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b_a\x07" "b\x1b\\",
        "APC i=_ T=a\\x07b"));
    /* corpus chordae ultra SERIES_CHORDA_MAXIMA: praecisum, notatum */
    {
        constans character* f;
               constans i8* ptr;

        f    = _repetere("\x1b]", "x", III * M, "\x07", piscina);
        ptr  = (constans i8*)f;
        series_lectorem_purgare(lx);
        CREDO_VERUM (series_lexema_proximum(lx, &ptr,
            ptr + strlen(f), &l) == SERIES_OSC);
        CREDO_AEQUALIS_I32 (l.textus.mensura, SERIES_CHORDA_MAXIMA);
        CREDO_VERUM (l.truncatum);
    }

    imprimere("\n--- III. pendet / evacuare (mora vocantis) ---\n");
    {
         constans character* casus_fugae[V];
         constans character* exspectata[V];
                        i32  c;

        casus_fugae[0]  = "\x1b";
        exspectata[0]   = "FUGA C=\\x1b";
        casus_fugae[1]  = "\x1b[";
        exspectata[1]   = "FUGA i=[ C=\\x1b[";
        casus_fugae[2]  = "\x1bO";
        exspectata[2]   = "FUGA i=O C=\\x1bO";
        casus_fugae[3]  = "\x1b[<0;1";
        exspectata[3]   = "FUGA i=[ C=\\x1b[<0;1";
        casus_fugae[4]  = "\x1b\x1b";
        exspectata[4]   = "FUGA PRAE C=\\x1b\\x1b";
        per (c = ZEPHYRUM; c < V; c++)
        {
                  constans i8* ptr =
                      (constans i8*)casus_fugae[c];
            ChordaAedificator* a = chorda_aedificator_creare(piscina,
                LXIV);

            series_lectorem_purgare(lx);
            CREDO_VERUM (series_lexema_proximum(lx, &ptr,
                ptr + strlen(casus_fugae[c]), &l) == SERIES_NIHIL);
            CREDO_VERUM (series_lector_pendet(lx));
            CREDO_VERUM (series_lectorem_evacuare(lx, &l));
            _lexema_reddere(a, &l, FALSUM);
            CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a),
                exspectata[c]);
            CREDO_FALSUM (series_lector_pendet(lx));
            CREDO_FALSUM (series_lectorem_evacuare(lx, &l));
        }
        /* post chordam per ESC clausam: solum '\' exspectatur - mora
         * nihil reddit */
        {
            constans character* f    = "\x1b]0;t\x1b";
                   constans i8* ptr  = (constans i8*)f;

            series_lectorem_purgare(lx);
            CREDO_VERUM (series_lexema_proximum(lx, &ptr,
                ptr + strlen(f), &l) == SERIES_OSC);
            CREDO_VERUM (series_lexema_proximum(lx, &ptr,
                (constans i8*)f + strlen(f), &l) == SERIES_NIHIL);
            CREDO_FALSUM (series_lectorem_evacuare(lx, &l));
            CREDO_FALSUM (series_lector_pendet(lx));
        }
    }

    imprimere("\n--- IV. scissurae omnes ---\n");
    discrepantiae = ZEPHYRUM;
    per (k = ZEPHYRUM; k < (i32)(magnitudo(corpus)
        / magnitudo(corpus[0]));
         k++)
    {
         constans i8* f = (constans i8*)corpus[k];
                 i32  m = (i32)strlen(corpus[k]);
              chorda  integrum;
              chorda  alterum;
                 s32  s;

        integrum = _fluxus(lx, f, m, ZEPHYRUM, VERUM, piscina);
        per (s = -I; s < (s32)m; s++)
        {
            si (s == ZEPHYRUM)
            {
                perge;
            }
            alterum = _fluxus(lx, f, m, s, VERUM, piscina);
            si (!chorda_aequalis(integrum, alterum))
            {
                si (discrepantiae == ZEPHYRUM)
                {
                    imprimere("  corpus[%u] scissio %d:\n    integrum: "
                        "%.*s\n    scissum:  %.*s\n", (unsigned)k,
                        (int)s,
                        (int)integrum.mensura,
                        (constans character*)integrum.datum,
                        (int)alterum.mensura,
                        (constans character*)alterum.datum);
                }
                discrepantiae++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);

    imprimere("\n--- VI. modus initus (B1b) ---\n");
    li = series_lectorem_creare(piscina);
    CREDO_NON_NIHIL (li);
    series_lectorem_initus_ponere(li, VERUM);
    CREDO_VERUM (_videre(li, piscina, "\x1b ", "ESC f=\\x20"));
    CREDO_VERUM (_videre(li, piscina, "\x1b!", "ESC f=!"));
    CREDO_VERUM (_videre(li, piscina, "\x1b(B", "ESC f=( | IMP T=B"));
    CREDO_VERUM (_videre(li, piscina, "\x1bNa", "ESC f=N | IMP T=a"));
    CREDO_VERUM (_videre(li, piscina, "\x1bXa", "ESC f=X | IMP T=a"));
    CREDO_VERUM (_videre(li, piscina, "\x1b^a", "ESC f=^ | IMP T=a"));
    CREDO_VERUM (_videre(li, piscina, "\x1bPa", "ESC f=P | IMP T=a"));
    /* OSC, APC series manent etiam in modo initus */
    CREDO_VERUM (_videre(li, piscina, "\x1b]a\x07", "OSC i=] T=a"));
    CREDO_VERUM (_videre(li, piscina, "\x1b_x\x1b\\", "APC i=_ T=x"));
    CREDO_VERUM (_videre(li, piscina, "\x1b\r",
        "FUGA C=\\x1b | EXS f=\\x0d"));
    CREDO_VERUM (_videre(li, piscina, "\x1b\x7f",
        "FUGA C=\\x1b | EXS f=\\x7f"));
    /* responsa vera manent series */
    CREDO_VERUM (_videre(li, piscina, "\x1bP>|kitty(0.40.1)\x1b\\",
        "DCS i=P p=> f=| T=kitty(0.40.1)"));
    CREDO_VERUM (_videre(li, piscina, "\x1bP1+r544e\x1b\\",
        "DCS i=P P=1 I=+ f=r T=544e"));
    CREDO_VERUM (_videre(li, piscina, "\x1b]4;0;#fff\x07",
        "OSC i=] T=4;0;#fff"));
    CREDO_VERUM (_videre(li, piscina, "\x1b_Gi=1;OK\x1b\\",
        "APC i=_ T=Gi=1;OK"));
    CREDO_VERUM (_videre(li, piscina, "\x1bOA", "SS i=O f=A"));
    CREDO_VERUM (_videre(li, piscina, "\x1bO\xc3\xa9",
        "FUGA i=O C=\\x1bO | IMP T=\\xc3\\xa9"));
    CREDO_VERUM (_videre(li, piscina, "\x1b\x1b[A",
        "CSI i=[ f=A PRAE"));
    /* modus scriptionis immutatus: ESC N = SS2, ESC SP = intermedium */
    CREDO_VERUM (_videre(lx, piscina, "\x1bNa", "SS i=N f=a"));
    CREDO_VERUM (_videre(lx, piscina, "\x1b(B", "ESC I=( f=B"));
    /* purgatio modum servat */
    series_lectorem_purgare(li);
    CREDO_VERUM (_videre(li, piscina, "\x1bNa", "ESC f=N | IMP T=a"));
    /* scissurae in modo initus */
    discrepantiae = ZEPHYRUM;
    per (k = ZEPHYRUM;
         k < (i32)(magnitudo(corpus_initus)
             / magnitudo(corpus_initus[0]));
         k++)
    {
         constans i8* f = (constans i8*)corpus_initus[k];
                 i32  m = (i32)strlen(corpus_initus[k]);
              chorda  integrum;
                 s32  s;

        integrum = _fluxus(li, f, m, ZEPHYRUM, VERUM, piscina);
        per (s = -I; s < (s32)m; s++)
        {
            si (   s != ZEPHYRUM && !chorda_aequalis(integrum,
                    _fluxus(li, f, m, s, VERUM, piscina)))
            {
                si (discrepantiae == ZEPHYRUM)
                {
                    imprimere("  corpus_initus[%u] scissio %d\n",
                        (unsigned)k, (int)s);
                }
                discrepantiae++;
            }
        }
    }
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);

    imprimere("\n--- V. initus hostilis: memoria finita ---\n");
    piscina_hostilis = piscina_generare_dynamicum("series_hostilis",
        LXIV * M);
    CREDO_NON_NIHIL (piscina_hostilis);
    lector_hostilis = series_lectorem_creare(piscina_hostilis);
    {
            memoriae_index  usus;
        constans character* f;
               constans i8* ptr;
               constans i8* finis;
                        i8* aleae;
                      Sors  sors;
                       i32  lexemata = ZEPHYRUM;

        /* constructio initus in piscina ALTERA: usus lectoris solus */
        f      = _repetere("\x1b[", "1;", X * M, "m", piscina);
        aleae  = (i8*)piscina_allocare(piscina, LXIV * MXXIV);
        sors_seminare(&sors, (i64)MMXXVI, ZEPHYRUM);
        per (k = ZEPHYRUM; k < LXIV * MXXIV; k++)
        {
            aleae[k] = (i8)(sors_proximum(&sors) & 0xFF);
        }
        usus   = piscina_summa_usus(piscina_hostilis);

        ptr    = (constans i8*)f;
        finis  = ptr + strlen(f);
        dum (series_lexema_proximum(lector_hostilis, &ptr, finis, &l)
            != SERIES_NIHIL)
        {
            lexemata++;
        }
        CREDO_AEQUALIS_I32 (lexemata, ZEPHYRUM);    /* X M parametra */
        CREDO_FALSUM (series_lector_pendet(lector_hostilis));

        f      = _repetere("\x1b]", "y", M * M, "", piscina);
        ptr    = (constans i8*)f;
        finis  = ptr + strlen(f);
        CREDO_VERUM (series_lexema_proximum(lector_hostilis, &ptr,
            finis, &l)
            == SERIES_NIHIL);
        CREDO_VERUM (series_lector_pendet(lector_hostilis));
        CREDO_VERUM (series_lectorem_evacuare(lector_hostilis, &l));
        CREDO_AEQUALIS_I32 (l.crudum.mensura, SERIES_CRUDUM_MAXIMUM);

        ptr    = (constans i8*)aleae;
        finis  = ptr + LXIV * MXXIV;
        dum (ptr < finis)
        {
            constans i8* frustum_finis;

            frustum_finis = ptr + I + sors_intra(&sors, XXXI);
            si (frustum_finis > finis)
            {
                frustum_finis = finis;
            }
            dum (series_lexema_proximum(lector_hostilis, &ptr,
                frustum_finis, &l)
                != SERIES_NIHIL)
            {
                lexemata++;
            }
        }
        CREDO_VERUM (lexemata > C);
        /* nihil post creationem allocatum */
        CREDO_VERUM (piscina_summa_usus(piscina_hostilis) == usus);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
