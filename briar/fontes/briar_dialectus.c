/* briar_dialectus.c - vide briar_dialectus.h */
#include "briar_dialectus.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  LXXII
#define MARGO     XV

/* '#define verbum valor' latina.h; grex = numerus gregis (lineae
 * vacuae greges separant) */
nomen structura {
    chorda verbum;
    chorda valor;
       i32 grex;
} BriarDialectiPar;

/* 'nomen <typus...> titulus;' - typus crudus, post resolvitur */
nomen structura {
    chorda titulus;
    chorda typus;
} BriarDialectiTypus;

nomen structura {
    ChordaAedificator* aed;
              Piscina* piscina;
                  i32  columna;
} BriarScriptor;

/* titulus gregis ex verbo PRIMO eius; ceteri -> 'cetera' */
interior constans character* TITULI_GREGUM[][II] = {
    { "character",  "typi:" },
    { "vacuum",     "qualitates:" },
    { "si",         "imperium:" },
    { "structura",  "aggregata:" },
    { "NIHIL",      "constantes:" },
    { "imprimere",  "bibliotheca:" },
    { "interior",   "staticus:" },
    { NIHIL,        NIHIL }
};

/* laquei C89: lingua congelata, ergo manu (lineae <= LXXII) */
interior constans character* LAQUEI[] = {
    "LAQUEI C89",
    "  - declarationes solum in capite blocki; nulla commentaria '//';",
    "    nullum 'inline'; nullum <stdint.h> (typi supra)",
    "  - initiator aggregati constantes SOLAS accipit: '{ \"x\", via,",
    "    NIHIL }' recusatur. Ordinem tempore cursus imple:",
    "        constans character* argumenta[IV];",
    "        argumenta[ZEPHYRUM] = \"pdfinfo\";",
    "        argumenta[I]        = via;",
    "        argumenta[II]       = NIHIL;",
    "        r = processus_exsequi(argumenta, 5000, piscina);",
    "  - principale (vacuum) aut principale (integer argc, character**",
    "    argv): integer aut s32, NUMQUAM i32 (insignatum)",
    "  - functio interior non adhibita = error (-Wunused-function)",
    "  - conversio signi tacita = error (-Wsign-conversion): i32, s32,",
    "    memoriae_index inter se solum cum iactu explicito",
    "  - verba supra VETITA ut identificatores (nomen=typedef,",
    "    casus=case, per=for, duplex=double); omne numerale",
    "    Romanum 0-3999 macrum est (DI, MIX, CIV...); ultra: IV * M",
    "  - chorda {i32 mensura; i8* datum} sine NUL finali: imprime per",
    "    \"%.*s\", (integer)c.mensura, (constans character*)c.datum",
    "  - atoi/strtol(c.datum) ULTRA mensuram legunt (sine NUL) et",
    "    sordes tacent: chorda_ut_s32 / chorda_ut_s64 (FALSUM si",
    "    vacua, sordes, superfluitas)",
    "  - adiutores: briar prototypos generat in regione principali ET",
    "    probationis - 'staticus' non opus. staticus usus a principale",
    "    solo -probatio frangit (principale abest: non adhibitus)",
    "  - elementum bibliotheca via=\"x.thistle\" (columna 0): regiones",
    "    C planae eius praesto (sine #include); via contra plagulam;",
    "    interior/staticus privata; circulus et nomen publicum bis",
    "    refutantur",
    NIHIL
};

interior b32
_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\r';
}

interior chorda
_frustum (
    chorda textus,
       i32 initium,
       i32 finis)
{
    chorda c;

    c.datum    = textus.datum + initium;
    c.mensura  = (i32)(finis - initium);
    redde c;
}

/* VERUM si linea [k, finis) cum praefixo incipit */
interior b32
_incipit (
                 chorda  textus,
                    i32  k,
                    i32  finis,
     constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);

    redde finis - k >= n
        && memcmp(textus.datum + k, praefixum, (size_t)n) == ZEPHYRUM;
}

/* verbum proximum ab *k (spatia praetermissa); vacua si nullum */
interior chorda
_verbum_proximum (
    chorda  textus,
       i32* k,
       i32  finis)
{
    i32 initium;

    dum (*k < finis && _spatium(textus.datum[*k]))
    {
        (*k)++;
    }
    initium = *k;
    dum (*k < finis && !_spatium(textus.datum[*k]))
    {
        (*k)++;
    }
    redde _frustum(textus, initium, *k);
}

interior b32
_digiti (
    chorda c)
{
    i32 k;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < (i32)c.mensura; k++)
    {
        si (c.datum[k] < '0' || c.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior i32
_numerus (
    chorda c)
{
    i32 n = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < (i32)c.mensura; k++)
    {
        n = n * X + (i32)(c.datum[k] - '0');
    }
    redde n;
}

/* latina.h legere: definitiones (cum gregibus) et typi 'nomen' */
interior vacuum
_legere (
     chorda  latina,
        Xar* paria,
        Xar* typi)
{
    i32 grex        = ZEPHYRUM;
    b32 grex_novus  = VERUM;
    i32 initium     = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k <= (i32)latina.mensura; k++)
    {
        i32 a;

        si (k < (i32)latina.mensura && latina.datum[k] != '\n')
        {
            perge;
        }
        a        = initium;
        initium  = k + I;
        dum (a < k && _spatium(latina.datum[a]))
        {
            a++;
        }
        si (a == k)
        {
            grex_novus = VERUM;
            perge;
        }
        si (_incipit(latina, a, k, "#define"))
        {
            chorda verbum;
            chorda valor;
               i32 v;

            a       += VII;
            verbum  = _verbum_proximum(latina, &a, k);
            dum (a < k && _spatium(latina.datum[a]))
            {
                a++;
            }
            v = k;
            dum (v > a && _spatium(latina.datum[v - I]))
            {
                v--;
            }
            valor = _frustum(latina, a, v);
            si (valor.mensura > ZEPHYRUM && verbum.mensura > ZEPHYRUM)
            {
                BriarDialectiPar* p;

                si (grex_novus)
                {
                    grex++;
                    grex_novus = FALSUM;
                }
                p = (BriarDialectiPar*)xar_addere(paria);
                si (p != NIHIL)
                {
                    p->verbum  = verbum;
                    p->valor   = valor;
                    p->grex    = grex;
                }
            }
            perge;
        }
        grex_novus = VERUM;
        si (_incipit(latina, a, k, "nomen "))
        {
            i32 f = k;
            i32 t;

            /* 'nomen <typus> titulus;' - titulus ante ';' ultimum */
            dum (   f > a && (   _spatium(latina.datum[f - I])
                           || latina.datum[f - I] == ';'))
            {
                f--;
            }
            t = f;
            dum (t > a && !_spatium(latina.datum[t - I]))
            {
                t--;
            }
            si (t > a + VI)
            {
                BriarDialectiTypus* ty =
                    (BriarDialectiTypus*)xar_addere(typi);

                si (ty != NIHIL)
                {
                    ty->titulus  = _frustum(latina, t, f);
                    ty->typus    = _frustum(latina, a + VI, t);
                }
            }
        }
    }
}

interior chorda
_resolvere (
       Xar* paria,
    chorda  verbum)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(paria); i++)
    {
        BriarDialectiPar* p = (BriarDialectiPar*)xar_obtinere(paria, i);

        si (chorda_aequalis(p->verbum, verbum))
        {
            redde p->valor;
        }
    }
    redde verbum;
}

interior vacuum
_scribere (
          BriarScriptor* s,
     constans character* t)
{
    constans character* q;

    chorda_aedificator_appendere_literis(s->aed, t);
    per (q = t; *q != '\0'; q++)
    {
        s->columna = *q == '\n' ? ZEPHYRUM : s->columna + I;
    }
}

interior vacuum
_scribere_chordam (
     BriarScriptor* s,
            chorda  c)
{
    chorda_aedificator_appendere_chorda(s->aed, c);
    s->columna += (i32)c.mensura;
}

/* '  titulus:    ' usque ad MARGO */
interior vacuum
_titulum (
          BriarScriptor* s,
     constans character* titulus)
{
    character linea[LXIV];

    sprintf(linea, "  %-12s ", titulus);
    _scribere(s, linea);
}

/* verbum (a + sep + b) cum involutione ad LATITUDO */
interior vacuum
_verbum (
          BriarScriptor* s,
                 chorda  a,
     constans character* sep,
                 chorda  b)
{
    i32 longitudo = (i32)(a.mensura + strlen(sep) + b.mensura);

    si (s->columna > MARGO && s->columna + I + longitudo > LATITUDO)
    {
        i32 k;

        _scribere(s, "\n");
        per (k = ZEPHYRUM; k < MARGO; k++)
        {
            _scribere(s, " ");
        }
    }
    alioquin si (s->columna > MARGO)
    {
        _scribere(s, " ");
    }
    _scribere_chordam(s, a);
    _scribere(s, sep);
    _scribere_chordam(s, b);
}

/* VERUM si vexillum v inter verba textus stat */
interior b32
_vexillum_inest (
    chorda v,
    chorda textus)
{
    i32 k = ZEPHYRUM;

    dum (k < (i32)textus.mensura)
    {
        si (chorda_aequalis(v, _verbum_proximum(textus, &k,
            (i32)textus.mensura)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* vexilla ut verba involuta; basis non-NIHIL = 'plana +' et sola
 * vexilla quae basis non habet. '|' = finis (post eum vexilla
 * fontium venditorum, e.g. sqlite - scriptori nihil ad rem) */
interior vacuum
_vexilla_scribere (
          BriarScriptor* s,
     constans character* titulus,
     constans character* vexilla,
     constans character* basis,
                Piscina* piscina)
{
    chorda textus  = chorda_ex_literis(vexilla, piscina);
    chorda fundus  = chorda_ex_literis(basis != NIHIL ? basis : "",
        piscina);
    chorda vacua  = _frustum(textus, ZEPHYRUM, ZEPHYRUM);
       i32 k      = ZEPHYRUM;

    _titulum(s, titulus);
    si (basis != NIHIL)
    {
        _verbum(s, chorda_ex_literis("plana +", piscina), "", vacua);
    }
    dum (k < (i32)textus.mensura)
    {
        chorda v = _verbum_proximum(textus, &k, (i32)textus.mensura);

        si (chorda_aequalis_literis(v, "|"))
        {
            frange;
        }
        si (v.mensura > ZEPHYRUM && !_vexillum_inest(v, fundus))
        {
            _verbum(s, v, "", vacua);
        }
    }
    _scribere(s, "\n");
}

interior constans character*
_titulus_gregis (
    chorda primum)
{
    i32 i;

    per (i = ZEPHYRUM; TITULI_GREGUM[i][ZEPHYRUM] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(primum, TITULI_GREGUM[i][ZEPHYRUM]))
        {
            redde TITULI_GREGUM[i][I];
        }
    }
    redde NIHIL;
}

/* Numerus maximus N ut omnes valores 0..N in grege [a, b) definiti
 * sint; -1 si 0 abest. latina.h numeros SELECTOS habet (0-214 omnes,
 * supra CCCXX sed non CCCXIX) - charta regulam dicit, non seriem
 * integram simulat (lapide briar-feedback documentation-ideas/014). */
interior s32
_continui_usque (
    Xar* paria,
    i32  a,
    i32  b)
{
    s32 n         = -I;
    b32 inventum  = VERUM;

    dum (inventum)
    {
        i32 i;

        inventum = FALSUM;
        per (i = a; i < b && !inventum; i++)
        {
            inventum = _numerus(((BriarDialectiPar*)xar_obtinere(paria,
                i))->valor) == (i32)(n + I);
        }
        si (inventum)
        {
            n++;
        }
    }
    redde n;
}

/* grex [a, b) paria; numeri contracti si omnes digiti et >= X */
interior vacuum
_gregem_scribere (
          BriarScriptor* s,
                    Xar* paria,
                    i32  a,
                    i32  b,
     constans character* titulus)
{
    chorda vacua;
       i32 i;
       b32 numeri = b - a >= X;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    per (i = a; i < b && numeri; i++)
    {
        numeri = _digiti(((BriarDialectiPar*)xar_obtinere(paria,
            i))->valor);
    }
    _titulum(s, numeri ? "numeri:" : titulus);
    si (numeri)
    {
         BriarDialectiPar* maximum = NIHIL;
                character  numerus[XXXII];

        per (i = a; i < b; i++)
        {
            BriarDialectiPar* p = (BriarDialectiPar*)xar_obtinere(paria,
                i);

            si (i < a + IV)
            {
                _verbum(s, p->verbum, "=", p->valor);
            }
            si (   maximum == NIHIL
                || _numerus(p->valor) > _numerus(maximum->valor))
            {
                maximum = p;
            }
        }
        _verbum(s, chorda_ex_literis("...", s->piscina), "",
            vacua);
        _verbum(s, maximum->verbum, "=", maximum->valor);
        sprintf(numerus, "(%u)", (insignatus integer)(b - a));
        _verbum(s, chorda_ex_literis(numerus, s->piscina), "",
            vacua);
        {
            s32 continui = _continui_usque(paria, a, b);

            /* regula in linea propria sub numeris, si series non
             * integra est: 'numerus N definitus?' scriptor quaerit */
            si ((i32)(continui + I) < (b - a))
            {
                _scribere(s, "\n");
                _titulum(s, "");
                si (continui >= ZEPHYRUM)
                {
                    sprintf(numerus, "0-%d;", (integer)continui);
                    _verbum(s, chorda_ex_literis("omnes", s->piscina),
                        "", vacua);
                    _verbum(s, chorda_ex_literis(numerus, s->piscina),
                        "", vacua);
                }
                _verbum(s, chorda_ex_literis("supra", s->piscina), "",
                    vacua);
                _verbum(s, chorda_ex_literis("selecti", s->piscina), "",
                    vacua);
                _verbum(s, chorda_ex_literis("tantum", s->piscina), "",
                    vacua);
                _verbum(s, chorda_ex_literis("(ceteri decimales)",
                    s->piscina), "", vacua);
            }
        }
    }
    alioquin
    {
        per (i = a; i < b; i++)
        {
            BriarDialectiPar* p = (BriarDialectiPar*)xar_obtinere(paria,
                i);

            _verbum(s, p->verbum, "=", p->valor);
        }
    }
    _scribere(s, "\n");
}

chorda
briar_dialectus_charta (
                 chorda  latina,
     constans character* vexilla_plana,
     constans character* vexilla_vitrea,
                Piscina* piscina)
{
    BriarScriptor  s;
              Xar* paria;
              Xar* typi;
              Xar* cetera;
              i32  i;
              i32  initium;

    s.aed = chorda_aedificator_creare(piscina,
        (memoriae_index)8192);
    s.piscina  = piscina;
    s.columna  = ZEPHYRUM;
    paria      = xar_creare(piscina, (i32)magnitudo(BriarDialectiPar));
    typi = xar_creare(piscina,
        (i32)magnitudo(BriarDialectiTypus));
    cetera = xar_creare(piscina, (i32)magnitudo(BriarDialectiPar));
    si (   s.aed  == NIHIL || paria == NIHIL || typi == NIHIL
        || cetera == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    _legere(latina, paria, typi);

    _scribere(&s,
        "DIALECTUS briar - C89 per latina.h; clang cum -Werror\n"
        "(monitio = error)\n\n");

    _scribere(&s, "TYPI INTEGRORUM (latina.h)\n");
    per (i = ZEPHYRUM; i < xar_numerus(typi); i++)
    {
        BriarDialectiTypus* ty = (BriarDialectiTypus*)xar_obtinere(typi,
            i);
                  character linea[LXIV];
                        i32 k       = ZEPHYRUM;
                        b32 primum  = VERUM;

        sprintf(linea, "  %-15.*s = ", (integer)ty->titulus.mensura,
            (constans character*)ty->titulus.datum);
        _scribere(&s, linea);
        dum (k < (i32)ty->typus.mensura)
        {
            chorda v = _verbum_proximum(ty->typus, &k,
                (i32)ty->typus.mensura);

            si (v.mensura == ZEPHYRUM)
            {
                perge;
            }
            si (!primum)
            {
                _scribere(&s, " ");
            }
            _scribere_chordam(&s, _resolvere(paria, v));
            primum = FALSUM;
        }
        _scribere(&s, "\n");
    }
    _scribere(&s, "  CAVE: iN INSIGNATI sunt, sN signati - i32 NON est"
        " 'int'.\n\n");

    _scribere(&s, "VEXILLA CLANG\n");
    _vexilla_scribere(&s, "plana:", vexilla_plana, NIHIL, piscina);
    _vexilla_scribere(&s, "vitrea:", vexilla_vitrea, vexilla_plana,
        piscina);
    _scribere(&s, "\n");

    _scribere(&s, "VERBA latina.h (verbum=C; omnia VETITA ut"
        " identificatores)\n");
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= xar_numerus(paria); i++)
    {
          BriarDialectiPar* primum;
        constans character* titulus;
                       i32  k;

        si (   i < xar_numerus(paria)
            && ((BriarDialectiPar*)xar_obtinere(paria, i))->grex
                == ((BriarDialectiPar*)xar_obtinere(paria,
                    initium))->grex)
        {
            perge;
        }
        primum   = (BriarDialectiPar*)xar_obtinere(paria, initium);
        titulus  = _titulus_gregis(primum->verbum);
        si (titulus != NIHIL || i - initium >= X)
        {
            _gregem_scribere(&s, paria, initium, i,
                titulus != NIHIL ? titulus : "cetera:");
        }
        alioquin
        {
            per (k = initium; k < i; k++)
            {
                BriarDialectiPar* c = (BriarDialectiPar*)xar_addere(
                    cetera);

                si (c != NIHIL)
                {
                    *c = *(BriarDialectiPar*)xar_obtinere(paria, k);
                }
            }
        }
        initium = i;
    }
    si (xar_numerus(cetera) > ZEPHYRUM)
    {
        _gregem_scribere(&s, cetera, ZEPHYRUM, xar_numerus(cetera),
            "cetera:");
    }
    _scribere(&s, "\n");

    per (i = ZEPHYRUM; LAQUEI[i] != NIHIL; i++)
    {
        _scribere(&s, LAQUEI[i]);
        _scribere(&s, "\n");
    }
    redde chorda_aedificator_finire(s.aed);
}
