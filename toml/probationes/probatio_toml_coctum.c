/* probatio_toml_coctum.c - Coctio documenti toml: tabulae et
 * structura (Q7b)
 *
 * Valida: valores per viam ('a.b#1.c': '#k' elementum seriei) post
 * claves punctatas, capita, series tabularum, tabulas inlineas;
 * ordo clavium = ordo fontis; modus tabularum. Vitia structurae:
 * codex, sedes PRIMARIA (segmentum clavis offendens), sedes RELATA
 * (definitio prima) ubi est. Omnia errata simul, per initium ordinata
 * (syntaxis + scalaria + structura). Corpora: toml-test valida omnia
 * sana, invalida OMNIA cum diagnostico (CDLXXIV); domus sana.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_coctum.h"
#include "toml_scalaris.h"
#include "toml_corpus_ambulare.h"
#include "tabula_dispersa.h"
#include "materia_diagnosticum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Adiumenta
 * ================================================== */

interior TomlCoctum
_coquere (
               Piscina* piscina,
    constans character* litterae)
{
            s32  n    = (s32)strlen(litterae);
      character* fons = (character*)piscina_allocare(piscina,
                            (i64)n + I);
    MateriaNodus* radix;
     TomlParsura  r;

    memcpy(fons, litterae, (size_t)n + I);
    radix = toml_arbor_parsare(piscina, fons, n, &r);
    redde toml_coquere(piscina, radix, &r);
}

/* via 'a.b#1.c': segmenta per '.', '#k' elementum seriei */
interior constans TomlValor*
_quaerere (
    constans TomlValor* radix,
    constans character* via)
{
    constans TomlValor* v = radix;
    constans character* p = via;

    dum (v != NIHIL)
    {
         constans character* finis = p;
                     chorda  seg;
                     vacuum* inventum = NIHIL;
                  character  alveus[CCLVI];

        dum (*finis != '\0' && *finis != '.' && *finis != '#')
        {
            finis++;
        }
        si (v->genus != TOML_VALOR_TABULA)
        {
            redde NIHIL;
        }
        memcpy(alveus, p, (size_t)(finis - p));
        seg.datum    = (i8*)alveus;
        seg.mensura  = (i32)(finis - p);
        si (seg.mensura == ZEPHYRUM)
        {
            i32 k;

            /* clavis vacua extra indicem (toml_coctum.c _invenire) */
            per (k = ZEPHYRUM; k < xar_numerus(v->datum.tabula.claves);
                 k++)
            {
                si (((chorda*)xar_obtinere(v->datum.tabula.claves,
                         k))->mensura == ZEPHYRUM)
                {
                    inventum = *(TomlValor**)xar_obtinere(
                        v->datum.tabula.valores, k);
                }
            }
            si (inventum == NIHIL)
            {
                redde NIHIL;
            }
        }
        alioquin si (!tabula_dispersa_invenire(v->datum.tabula.index,
                         seg, &inventum))
        {
            redde NIHIL;
        }
        v = (constans TomlValor*)inventum;
        p = finis;
        dum (*p == '#' && v != NIHIL)
        {
            i32 k = (i32)atoi(p + I);

            si (   v->genus != TOML_VALOR_SERIES
                || k        >= xar_numerus(v->datum.series))
            {
                redde NIHIL;
            }
            v = *(TomlValor**)xar_obtinere(v->datum.series, k);
            p++;
            dum (*p >= '0' && *p <= '9')
            {
                p++;
            }
        }
        si (*p == '\0')
        {
            redde v;
        }
        p++;   /* '.' */
    }
    redde NIHIL;
}

interior constans MateriaDiagnosticum*
_diagnosticum (
    Xar* dd,
    i32  k)
{
    redde (constans MateriaDiagnosticum*)xar_obtinere(dd, k);
}

interior vacuum
_diagnostica_imprimere (
    constans TomlCoctum* c)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(c->diagnostica); k++)
    {
        constans MateriaDiagnosticum* d = _diagnosticum(c->diagnostica,
            k);

        imprimere("      %s @%d: %s", d->codex,
            (integer)d->tractus.initium,
            d->causa);
        si (d->numerus_relatorum > ZEPHYRUM)
        {
            imprimere(" [relata @%d]",
                (integer)d->relata[ZEPHYRUM].tractus.initium);
        }
        imprimere("\n");
    }
}


/* ==================================================
 * Tabulae casuum
 * ================================================== */

nomen structura {
    constans character* fons;
    constans character* via;
                   s64  valor;
} CasusValidus;

hic_manens constans CasusValidus VALIDA[] = {
    { "a.b.c = 1\na.b.d = 2",                        "a.b.c",   I },
    { "a.b.c = 1\na.b.d = 2",                        "a.b.d",   II },
    { "[a.b]\nx = 1\n[a]\ny = 2",                    "a.b.x",   I },
    { "[a.b]\nx = 1\n[a]\ny = 2",                    "a.y",     II },
    { "[fruit]\napple.color = 1\napple.taste.sweet = 2\n"
      "[fruit.apple.texture]\nsmooth = 3",
      "fruit.apple.texture.smooth", III },
    { "[fruit]\napple.color = 1\napple.taste.sweet = 2\n"
      "[fruit.apple.texture]\nsmooth = 3",
      "fruit.apple.taste.sweet", II },
    { "[[f]]\nn = 1\n[f.p]\nc = 2\n[[f.v]]\nn = 3\n[[f]]\nn = 4\n"
      "[[f.v]]\nn = 5",                              "f#0.p.c",  II },
    { "[[f]]\nn = 1\n[f.p]\nc = 2\n[[f.v]]\nn = 3\n[[f]]\nn = 4\n"
      "[[f.v]]\nn = 5",                              "f#0.v#0.n", III },
    { "[[f]]\nn = 1\n[f.p]\nc = 2\n[[f.v]]\nn = 3\n[[f]]\nn = 4\n"
      "[[f.v]]\nn = 5",                              "f#1.v#0.n", V },
    { "[[f]]\nn = 1\n[f.p]\nc = 2\n[[f.v]]\nn = 3\n[[f]]\nn = 4\n"
      "[[f.v]]\nn = 5",                              "f#1.n",    IV },
    { "3.14159 = 1",                                 "3.14159",  I },
    { "a = {x = 1, y.z = 2}",                        "a.y.z",    II },
    { "a = [{x = 1}, {x = 2}]",                      "a#1.x",    II },
    { "[a]\n[a.b]\n[a.c]\nd = 1",                    "a.c.d",    I },
    { "[a.b.c]\n[a]\nb.d = 1",                       "a.b.d",    I },
    { "[[a]]\n[a.b]\nx = 1\n[[a]]\n[a.b]\nx = 2",    "a#1.b.x",  II },
    { "[[a]]\n[a.b]\nx = 1\n[[a]]\n[a.b]\nx = 2",    "a#0.b.x",  I },
    { "\"\" = 1",                                    "",         I },
    { "\"k\\u0041\" = 1",                            "kA",       I },
    { "t = {a = [1,\n2]}",                           "t.a#1",    II },
    { "[a]\nb.c = 1\nb.d = 2",                       "a.b.d",    II },
    { "a = 1 # c\n[b] # c\nc = 2",                   "b.c",      II },
    { NIHIL,                                         NIHIL,
        ZEPHYRUM }
};

nomen structura {
    constans character* fons;
    constans character* codex;
                   s32  initium;
                   s32  relatum;    /* -I: sine sede relata */
} CasusVitii;

hic_manens constans CasusVitii VITIA[] = {
    { "a = 1\na = 2",              TOML_CODEX_CLAVIS_ITERATA,  VI,
        ZEPHYRUM },
    { "a.b = 1\na.b = 2",          TOML_CODEX_CLAVIS_ITERATA,  X,
        ZEPHYRUM },
    { "\"a\" = 1\na = 2",          TOML_CODEX_CLAVIS_ITERATA,  VIII,
        ZEPHYRUM },
    { "a = {b = 1, b = 2}",        TOML_CODEX_CLAVIS_ITERATA,  XII, V },
    { "a = {b.c = 1, b = 2}",      TOML_CODEX_CLAVIS_ITERATA,  XIV, V },
    { "[[p.arr]]\n[p]\narr = 2",   TOML_CODEX_CLAVIS_ITERATA,  XIV,
        II },
    { "[a]\n[a]",                  TOML_CODEX_TABULA_ITERATA,  V,   I },
    { "[x]\n[x.y]\n[x]",           TOML_CODEX_TABULA_ITERATA,  XI,  I },
    { "[[a]]\n[a]",                TOML_CODEX_TABULA_ITERATA,  VII,
        II },
    { "a.b = 1\n[a]",              TOML_CODEX_TABULA_PUNCTATA, IX,
        ZEPHYRUM },
    { "[a]\nb.c = 1\n[a.b]",       TOML_CODEX_TABULA_PUNCTATA, XV,
        IV },
    { "[t1]\nt2.t3.v = 0\n[t1.t2]", TOML_CODEX_TABULA_PUNCTATA, XXI,
        V },
    { "a = {x = 1}\na.y = 2",      TOML_CODEX_CLAUSA,          XII,
        ZEPHYRUM },
    { "a = []\n[[a]]",             TOML_CODEX_CLAUSA,          IX,
        ZEPHYRUM },
    { "a = [{b = 1}]\n[a.c]",      TOML_CODEX_CLAUSA,          XV,
        ZEPHYRUM },
    { "a = {x = {y = 1}, x.z = 2}", TOML_CODEX_CLAUSA,         XVIII,
        V },
    { "a = {}\n[a.b]",             TOML_CODEX_CLAUSA,          VIII,
        ZEPHYRUM },
    { "[a.b]\n[[a]]",              TOML_CODEX_GENUS_ALIENUM,   VIII,
        I },
    { "[a]\n[[a]]",                TOML_CODEX_GENUS_ALIENUM,   VI,  I },
    { "a = 1\n[a.b]",              TOML_CODEX_GENUS_ALIENUM,   VII,
        ZEPHYRUM },
    { "a = 1\na.b = 2",            TOML_CODEX_GENUS_ALIENUM,   VI,
        ZEPHYRUM },
    { "a.b = 1\na.b.c = 2",        TOML_CODEX_GENUS_ALIENUM,   X,
        ZEPHYRUM },
    { "a = true\n[[a]]",           TOML_CODEX_GENUS_ALIENUM,   XI,
        ZEPHYRUM },
    { "[[albums.songs]]\n[[albums]]", TOML_CODEX_GENUS_ALIENUM, XIX,
        II },
    { "[a.b.c]\nz = 9\n[a]\nb.c.t = 1", TOML_CODEX_EXTRA_SECTIONEM,
      XX, I },
    { "[[a.b]]\n[a]\nb.y = 2",     TOML_CODEX_EXTRA_SECTIONEM, XII,
        II },
    { "[a]\nb.c = 1\n[x]\n[a]\nb.d = 2", TOML_CODEX_TABULA_ITERATA,
      XVII, I },
    { "[a.]",                      TOML_CODEX_CLAVIS_VACUA,    II,
        -I },
    { "a. = 1",                    TOML_CODEX_CLAVIS_VACUA,    I,
        -I },
    { "t = {a = 1,\nb = 2}",       TOML_CODEX_VERSIO_NOVIOR,   XI,
        -I },
    { "t = {a = 1\n}",             TOML_CODEX_VERSIO_NOVIOR,   X,
        -I },
    { "t = {a = 1, }",             TOML_CODEX_VERSIO_NOVIOR,   X,
        -I },
    { NIHIL,                       NIHIL,                      ZEPHYRUM,
      ZEPHYRUM }
};


/* ==================================================
 * Corpora
 * ================================================== */

nomen structura {
    i32 valida;
    i32 valida_sana;
    i32 invalida;
    i32 invalida_cum_diagnostico;
    i32 domus;
    i32 domus_sana;
    i32 silvestria;
    i32 silvestria_sana;
    i32 ordo_ruptus;
    i32 nominati;
} Status;

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
         Status* st = (Status*)datum;
    TomlParsura  r;
    MateriaNodus* radix;
    TomlCoctum c;
           i32 n;
           i32 k;

    radix = toml_arbor_parsare(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r);
    c = toml_coquere(opus, radix, &r);
    n = c.diagnostica != NIHIL ? xar_numerus(c.diagnostica) : ZEPHYRUM;
    per (k = I; k < n; k++)
    {
        si (   _diagnosticum(c.diagnostica, k)->tractus.initium
            < _diagnosticum(c.diagnostica, k - I)->tractus.initium)
        {
            st->ordo_ruptus++;
        }
    }
    si (fons == TOML_CORPUS_TOML_TEST)
    {
        si (strncmp(via, "valid/", VI) == ZEPHYRUM)
        {
            st->valida++;
            si (c.sanum)
            {
                st->valida_sana++;
            }
            alioquin si (st->nominati++ < X)
            {
                imprimere("    validum non sanum: %s\n", via);
                _diagnostica_imprimere(&c);
            }
        }
        alioquin
        {
            st->invalida++;
            si (n > ZEPHYRUM && !c.sanum)
            {
                st->invalida_cum_diagnostico++;
            }
            alioquin si (st->nominati++ < X)
            {
                imprimere("    invalidum sine diagnostico: %s\n", via);
            }
        }
    }
    alioquin si (fons == TOML_CORPUS_DOMUS)
    {
        st->domus++;
        si (c.sanum)
        {
            st->domus_sana++;
        }
        alioquin si (st->nominati++ < X)
        {
            imprimere("    domus non sana: %s\n", via);
            _diagnostica_imprimere(&c);
        }
    }
    alioquin
    {
        st->silvestria++;
        si (c.sanum)
        {
            st->silvestria_sana++;
        }
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                Piscina* opus;
     constans character* radix = getenv("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_coctum",
        262144);
    opus    = piscina_generare_dynamicum("probatio_toml_coctum_opus",
        1048576);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Valida: valores per viam
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans valida ---\n");
        per (k = ZEPHYRUM; VALIDA[k].fons != NIHIL; k++)
        {
                    TomlCoctum  c = _coquere(opus, VALIDA[k].fons);
            constans TomlValor* v = _quaerere(c.radix, VALIDA[k].via);
                           b32  bene = c.sanum && v != NIHIL
                               && v->genus == TOML_VALOR_INTEGER
                               && v->datum.integer_valor
                                 == VALIDA[k].valor;

            si (!bene)
            {
                imprimere("    casus %u (%s): sanum %d, valor %s\n", k,
                    VALIDA[k].via, (integer)c.sanum,
                    v == NIHIL ? "NIHIL" : "alius");
                _diagnostica_imprimere(&c);
            }
            CREDO_VERUM (bene);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Modi, genera, definitiones
     * ================================================== */

    {
                TomlCoctum  c;
        constans TomlValor* v;

        imprimere("\n--- Probans modos tabularum ---\n");
        c = _coquere(opus, "a.b = 1\n[x.y]\n[[s]]\ni = {k = 1}\n");
        CREDO_VERUM (c.sanum);
        v = _quaerere(c.radix, "a");
        CREDO_NON_NIHIL (v);
        CREDO_AEQUALIS_S32 ((s32)v->datum.tabula.modus,
            (s32)TOML_TABULA_PUNCTATA);
        v = _quaerere(c.radix, "x");
        CREDO_AEQUALIS_S32 ((s32)v->datum.tabula.modus,
            (s32)TOML_TABULA_IMPLICITA);
        v = _quaerere(c.radix, "x.y");
        CREDO_AEQUALIS_S32 ((s32)v->datum.tabula.modus,
            (s32)TOML_TABULA_EXPLICITA);
        v = _quaerere(c.radix, "s");
        CREDO_AEQUALIS_S32 ((s32)v->genus, (s32)TOML_VALOR_SERIES);
        CREDO_VERUM (v->series_tabularum);
        v = _quaerere(c.radix, "s#0.i");
        CREDO_AEQUALIS_S32 ((s32)v->datum.tabula.modus,
            (s32)TOML_TABULA_INLINEA);
        CREDO_NON_NIHIL (v->nodus);
        CREDO_NON_NIHIL (v->definitio);
        v = _quaerere(c.radix, "a.b");
        CREDO_NON_NIHIL (v->nodus);
        CREDO_NON_NIHIL (v->definitio);
        piscina_vacare(opus);
    }


    /* ==================================================
     * Ordo clavium = ordo fontis
     * ================================================== */

    {
                TomlCoctum  c;
        constans character* EXSPECTATAE[] = { "z", "a", "m", "a.b" };
                       i32  k;

        imprimere("\n--- Probans ordinem clavium ---\n");
        c = _coquere(opus, "z = 1\na = 2\nm = 3\n\"a.b\" = 4\n");
        CREDO_VERUM (c.sanum);
        CREDO_AEQUALIS_I32 (xar_numerus(c.radix->datum.tabula.claves),
            IV);
        per (k = ZEPHYRUM; k < IV
             && k < xar_numerus(c.radix->datum.tabula.claves); k++)
        {
            chorda* cl = (chorda*)xar_obtinere(
                c.radix->datum.tabula.claves, k);

            CREDO_AEQUALIS_I32 (cl->mensura,
                (i32)strlen(EXSPECTATAE[k]));
            CREDO_VERUM (memcmp(cl->datum, EXSPECTATAE[k],
                cl->mensura) == ZEPHYRUM);
        }
        piscina_vacare(opus);
    }


    /* ==================================================
     * Vitia structurae: codex, sedes, sedes relata
     * ================================================== */

    {
        i32 k;

        imprimere("\n--- Probans vitia structurae ---\n");
        per (k = ZEPHYRUM; VITIA[k].fons != NIHIL; k++)
        {
             constans CasusVitii* cv  = &VITIA[k];
                      TomlCoctum  c   = _coquere(opus, cv->fons);
                             i32  n   = xar_numerus(c.diagnostica);
                             b32  bene;

            bene = !c.sanum && n == I
                && strcmp(_diagnosticum(c.diagnostica, ZEPHYRUM)->codex,
                    cv->codex) == ZEPHYRUM
                && _diagnosticum(c.diagnostica,
                ZEPHYRUM)->tractus.initium
                    == cv->initium;
            si (bene && cv->relatum >= ZEPHYRUM)
            {
                constans MateriaDiagnosticum* d =
                    _diagnosticum(c.diagnostica, ZEPHYRUM);

                bene = d->numerus_relatorum == I
                    && d->relata[ZEPHYRUM].tractus.initium
                        == cv->relatum;
            }
            si (!bene)
            {
                imprimere("    casus %u '%s' (%s @%d, relatum %d):\n",
                    k,
                    cv->fons, cv->codex, (integer)cv->initium,
                    (integer)cv->relatum);
                _diagnostica_imprimere(&c);
            }
            CREDO_VERUM (bene);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * Omnia errata simul, per initium ordinata
     * ================================================== */

    {
                TomlCoctum  c;
        constans character* CODICES[] = { TOML_CODEX_CLAVIS_ITERATA,
            TOML_CODEX_EXTRA_FINES, "malum" };
        s32 INITIA[III];
        i32 k;

        INITIA[ZEPHYRUM]  = VI;
        INITIA[I]         = XVI;
        INITIA[II]        = XL;
        imprimere("\n--- Probans errata omnia ---\n");
        c = _coquere(opus,
            "a = 1\na = 2\nb = 9223372036854775808\nc = ?\n");
        CREDO_FALSUM (c.sanum);
        CREDO_AEQUALIS_I32 (xar_numerus(c.diagnostica), III);
        si (xar_numerus(c.diagnostica) != III)
        {
            _diagnostica_imprimere(&c);
        }
        per (k = ZEPHYRUM; k < III
            && k < xar_numerus(c.diagnostica); k++)
        {
            CREDO_VERUM (strcmp(_diagnosticum(c.diagnostica, k)->codex,
                CODICES[k]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (
                _diagnosticum(c.diagnostica, k)->tractus.initium,
                INITIA[k]);
        }
        /* documentum cum vitiis tamen coquitur */
        CREDO_NON_NIHIL (_quaerere(c.radix, "a"));
        piscina_vacare(opus);
    }


    /* ==================================================
     * Corpora
     * ================================================== */

    {
                  Status st;
        TomlCorpusNumeri nn;

        imprimere("\n--- Probans corpora ---\n");
        memset(&st, ZEPHYRUM, magnitudo(st));
        toml_corpus_ambulare(piscina, opus, radix
            != NIHIL ? radix : ".",
            _visor, &st, &nn);
        imprimere("  toml-test valida: %u, sana %u; invalida: %u, cum "
            "diagnostico %u\n", st.valida, st.valida_sana, st.invalida,
            st.invalida_cum_diagnostico);
        imprimere("  domus: %u, sana %u; silvestria: %u, sana %u; ordo "
            "ruptus %u\n", st.domus, st.domus_sana, st.silvestria,
            st.silvestria_sana, st.ordo_ruptus);
        CREDO_VERUM (nn.indices_lecti);
        CREDO_AEQUALIS_I32 (st.valida, (i32)CCV);
        CREDO_AEQUALIS_I32 (st.valida_sana, st.valida);
        CREDO_AEQUALIS_I32 (st.invalida, (i32)CDLXXIV);
        CREDO_AEQUALIS_I32 (st.invalida_cum_diagnostico, st.invalida);
        CREDO_MAIOR_I32 (st.domus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (st.domus_sana, st.domus);
        CREDO_AEQUALIS_I32 (st.ordo_ruptus, ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
