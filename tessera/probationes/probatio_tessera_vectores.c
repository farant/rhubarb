/* probatio_tessera_vectores.c - Vectores initus per pontem frustorum
 *
 * Omnis vector (vectores_initus.h) in formis quattuor currit:
 *   INTEGRA   - scriptum totum una lectione
 *   BIPARTITA - omnis scissio in duo frusta, intra moram
 *   SINGULA   - octetus quisque suo frusto, intra moram
 *   SEQUENS   - + 'a' post (post moram si vector moram terminalem
 *               poscit): nihil post vectorem devoratur
 *
 * PONS FRUSTORUM: pons probationis proprius (TesseraPons publicus est
 * - sutura ut designata). Legere frustum proximum reddit; frustum
 * vacuum = mora exacta (ZEPHYRUM semel); frustis exhaustis semper
 * ZEPHYRUM. Pons memoriae NON mutatur (in capite amalgamatis
 * manuscripto vivit).
 *
 * DEBITA per formam (formae_debitae): forma debita congruens FRANGIT
 * ("debitum solutum - promove"); non congruens "debitum manet"
 * imprimit et numeratur.
 */
#include "latina.h"
#include "piscina.h"
#include "tessera_pons.h"
#include "tessera_eventum.h"
#include "credo.h"
#include "vectores_initus.h"
#include <stdio.h>
#include <string.h>

#define FRUSTA_MAXIMA            CCLVI
#define OCTETI_MAXIMI            CCLVI
#define EVENTA_OBSERVATA_MAXIMA  XVI
#define CIRCUITUS_MAXIMUS        CXXVIII


/* ================================================================
 * Pons frustorum
 * ================================================================ */

nomen structura {
    i32 initium;
    i32 longitudo;          /* ZEPHYRUM = mora exacta */
} Frustum;

nomen structura {
    TesseraPons pons;      /* pons.datum = haec structura */
             i8 octeti[OCTETI_MAXIMI];
            i32 mensura;
        Frustum frusta[FRUSTA_MAXIMA];
            i32 numerus_frustorum;
            i32 frustum_currens;
            i32 intra_frustum;    /* octeti frusti currentis iam redditi */
} PonsFrustorum;

interior s32
_frusta_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    PonsFrustorum* pf = (PonsFrustorum*)datum;
          Frustum* f;
              i32  reliqui;
              i32  n;

    (vacuum)mora_ms;  /* scriptum: tempus nullum */
    si (pf->frustum_currens >= pf->numerus_frustorum)
    {
        redde ZEPHYRUM;
    }
    f = &pf->frusta[pf->frustum_currens];
    si (f->longitudo == ZEPHYRUM)
    {
        pf->frustum_currens++;
        redde ZEPHYRUM;           /* mora exacta, semel */
    }
    reliqui  = f->longitudo - pf->intra_frustum;
    n        = (reliqui < capacitas) ? reliqui : capacitas;
    memcpy(buffer, pf->octeti + f->initium + pf->intra_frustum,
        (memoriae_index)n);
    pf->intra_frustum += n;
    si (pf->intra_frustum >= f->longitudo)
    {
        pf->frustum_currens++;
        pf->intra_frustum = ZEPHYRUM;
    }
    redde (s32)n;
}

interior b32
_frusta_scribere (
         vacuum* datum,
    constans i8* octeti,
            i32  numerus)
{
    (vacuum)datum;
    (vacuum)octeti;
    (vacuum)numerus;
    redde VERUM;
}

interior b32
_frusta_amplitudo (
    vacuum* datum,
       i32* latitudo_out,
       i32* altitudo_out)
{
    (vacuum)datum;
    *latitudo_out = LXXX;
    *altitudo_out = XXIV;
    redde VERUM;
}

interior b32
_frusta_status (
    vacuum* datum)
{
    (vacuum)datum;
    redde VERUM;
}

interior b32
_frusta_exhausta (
    constans PonsFrustorum* pf)
{
    redde pf->frustum_currens >= pf->numerus_frustorum;
}

interior vacuum
_frustum_addere (
    PonsFrustorum* pf,
              i32  initium,
              i32  longitudo)
{
    pf->frusta[pf->numerus_frustorum].initium    = initium;
    pf->frusta[pf->numerus_frustorum].longitudo  = longitudo;
    pf->numerus_frustorum++;
}

/* Pontem pro vectore et forma parare; scissio solum pro BIPARTITA */
interior vacuum
_pontem_parare (
            PonsFrustorum* pf,
    constans VectorInitus* v,
                      i32  forma,
                      i32  scissio)
{
    i32 k;

    memset(pf, ZEPHYRUM, magnitudo(PonsFrustorum));
    pf->pons.datum      = pf;
    pf->pons.legere     = _frusta_legere;
    pf->pons.scribere   = _frusta_scribere;
    pf->pons.amplitudo  = _frusta_amplitudo;
    pf->pons.intrare    = _frusta_status;
    pf->pons.egredi     = _frusta_status;
    pf->pons.resumptum  = NIHIL;

    memcpy(pf->octeti, v->octeti, (memoriae_index)v->mensura);
    pf->mensura = v->mensura;

    si (forma == FORMA_INTEGRA)
    {
        _frustum_addere(pf, ZEPHYRUM, v->mensura);
    }
    alioquin si (forma == FORMA_BIPARTITA)
    {
        _frustum_addere(pf, ZEPHYRUM, scissio);
        _frustum_addere(pf, scissio, v->mensura - scissio);
    }
    alioquin si (forma == FORMA_SINGULA)
    {
        per (k = ZEPHYRUM; k < v->mensura; k++)
        {
            _frustum_addere(pf, k, I);
        }
    }
    alioquin  /* SEQUENS */
    {
        pf->octeti[v->mensura]  = (i8)'a';
        pf->mensura             = v->mensura + I;
        si (v->mora_terminalis)
        {
            _frustum_addere(pf, ZEPHYRUM, v->mensura);
            _frustum_addere(pf, ZEPHYRUM, ZEPHYRUM);   /* mora */
            _frustum_addere(pf, v->mensura, I);
        }
        alioquin
        {
            _frustum_addere(pf, ZEPHYRUM, v->mensura + I);
        }
    }
}


/* ================================================================
 * Cursus et comparatio
 * ================================================================ */

/* Eventa legere usque ad frusta exhausta et lectorem vacuum.
 * *ruptum = VERUM si eventa nimia aut circuitus sine fine. */
interior i32
_currere (
     PonsFrustorum* pf,
           Piscina* piscina,
    TesseraEventum* observata,
               b32* ruptum)
{
    TesseraLector* lector;
              i32  n = ZEPHYRUM;
              i32  circuitus;

    *ruptum  = FALSUM;
    lector   = tessera_lector_creare(piscina, &pf->pons);
    si (lector == NIHIL)
    {
        *ruptum = VERUM;
        redde ZEPHYRUM;
    }
    per (circuitus = ZEPHYRUM; circuitus
        < CIRCUITUS_MAXIMUS; circuitus++)
    {
        TesseraEventum ev;

        si (tessera_eventum_expectare(lector, &ev, X)
                == TESSERA_EVENTUM_NIHIL)
        {
            si (_frusta_exhausta(pf) && lector->mensura == ZEPHYRUM)
            {
                redde n;
            }
            perge;
        }
        si (n >= EVENTA_OBSERVATA_MAXIMA)
        {
            *ruptum = VERUM;
            redde n;
        }
        observata[n++] = ev;
    }
    *ruptum = VERUM;
    redde n;
}

/* Eventa exspectata pro forma (SEQUENS addit 'a') */
interior i32
_exspectata_colligere (
    constans VectorInitus* v,
                      i32  forma,
       EventumExspectatum* exspectata)
{
    i32 n = ZEPHYRUM;

    dum (   n < VECTOR_EVENTA_MAXIMA
         && v->eventa[n].genus != TESSERA_EVENTUM_NIHIL)
    {
        exspectata[n] = v->eventa[n];
        n++;
    }
    si (forma == FORMA_SEQUENS)
    {
        EventumExspectatum a = EX_RUNA('a', ZEPHYRUM);

        exspectata[n++] = a;
    }
    redde n;
}

interior b32
_eventum_congruit (
        constans TesseraEventum* o,
    constans EventumExspectatum* e)
{
    redde o->genus == e->genus
        && o->clavis == e->clavis
        && o->runa == e->runa
        && o->modificatores == e->modificatores
        && o->numerus == e->numerus
        && o->mus_genus == e->mus_genus
        && o->mus_x == e->mus_x
        && o->mus_y == e->mus_y
        && o->mus_pulsus == e->mus_pulsus;
}


/* ================================================================
 * Impressio (solum in fractura)
 * ================================================================ */

interior constans character* CLAVIUM_TITULI[] = {
    "nulla", "sursum", "deorsum", "dextra", "sinistra", "domus",
    "finis", "pagina_sursum", "pagina_deorsum", "insertio",
    "deletio", "fuga", "reditus", "tabula", "retrorsum", "functio"
};

interior constans character* MURIUM_TITULI[] = {
    "pressus", "solutus", "rota_sursum", "rota_deorsum"
};

interior vacuum
_octetos_imprimere (
    constans i8* o,
            i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        si (o[k] == 0x1B)
        {
            imprimere("\\033");
        }
        alioquin si (o[k] >= 0x20 && o[k] < 0x7F && o[k] != '\\')
        {
            imprimere("%c", (integer)o[k]);
        }
        alioquin
        {
            imprimere("\\x%02X", (integer)o[k]);
        }
    }
}

interior vacuum
_campos_imprimere (
    TesseraEventumGenus genus,
          TesseraClavis clavis,
                    s32 runa,
                    i32 modificatores,
                    i32 numerus,
        TesseraMusGenus mus_genus,
                    s32 mus_x,
                    s32 mus_y,
                    i32 mus_pulsus)
{
    si (genus == TESSERA_EVENTUM_MUS)
    {
        imprimere("MUS(%s %d,%d p%u)", MURIUM_TITULI[mus_genus],
            (integer)mus_x, (integer)mus_y,
            (insignatus integer)mus_pulsus);
    }
    alioquin si (genus != TESSERA_EVENTUM_CLAVIS)
    {
        imprimere("GENUS(%d)", (integer)genus);
    }
    alioquin si (clavis == TESSERA_CLAVIS_NULLA)
    {
        imprimere("RUNA(U+%04X mod=%u)", (insignatus integer)runa,
            (insignatus integer)modificatores);
    }
    alioquin si (clavis == TESSERA_CLAVIS_FUNCTIO)
    {
        imprimere("F%u(mod=%u)", (insignatus integer)numerus,
            (insignatus integer)modificatores);
    }
    alioquin
    {
        imprimere("CLAVIS(%s mod=%u)", CLAVIUM_TITULI[clavis],
            (insignatus integer)modificatores);
    }
}

interior vacuum
_fracturam_imprimere (
              constans VectorInitus* v,
                 constans character* forma_titulus,
                                s32  scissio,
             constans PonsFrustorum* pf,
        constans EventumExspectatum* exspectata,
                                i32  numerus_exspectatorum,
            constans TesseraEventum* observata,
                                i32  numerus_observatorum,
                                b32  ruptum)
{
    i32 k;

    imprimere("  FRACTA: \"%s\" [%s", v->titulus, forma_titulus);
    si (scissio >= ZEPHYRUM)
    {
        imprimere(" %d", (integer)scissio);
    }
    imprimere("]%s\n    octeti:     ", ruptum ? " (RUPTUM)" : "");
    _octetos_imprimere(pf->octeti, pf->mensura);
    imprimere("\n    exspectata: ");
    per (k = ZEPHYRUM; k < numerus_exspectatorum; k++)
    {
        constans EventumExspectatum* e = &exspectata[k];

        _campos_imprimere(e->genus, e->clavis, e->runa,
            e->modificatores,
            e->numerus, e->mus_genus, e->mus_x, e->mus_y,
            e->mus_pulsus);
        imprimere(" ");
    }
    imprimere("\n    observata:  ");
    per (k = ZEPHYRUM; k < numerus_observatorum; k++)
    {
        constans TesseraEventum* o = &observata[k];

        _campos_imprimere(o->genus, o->clavis, o->runa,
            o->modificatores,
            o->numerus, o->mus_genus, o->mus_x, o->mus_y,
            o->mus_pulsus);
        imprimere(" ");
    }
    imprimere("\n");
}

/* Unum cursum (forma + scissio) probare */
interior b32
_cursum_probare (
                  Piscina* piscina,
    constans VectorInitus* v,
                      i32  forma,
       constans character* forma_titulus,
                      s32  scissio,
                      b32  loquax)
{
        PonsFrustorum pf;
       TesseraEventum observata[EVENTA_OBSERVATA_MAXIMA];
   EventumExspectatum exspectata[VECTOR_EVENTA_MAXIMA + I];
                  i32 numerus_observatorum;
                  i32 numerus_exspectatorum;
                  b32 ruptum;
                  b32 congruit;
                  i32 k;

    _pontem_parare(&pf, v, forma,
        (scissio >= ZEPHYRUM) ? (i32)scissio : ZEPHYRUM);
    numerus_observatorum = _currere(&pf, piscina, observata, &ruptum);
    numerus_exspectatorum = _exspectata_colligere(v, forma, exspectata);

    congruit = !ruptum && numerus_observatorum == numerus_exspectatorum;
    per (k = ZEPHYRUM; congruit && k < numerus_exspectatorum; k++)
    {
        congruit = _eventum_congruit(&observata[k], &exspectata[k]);
    }
    si (!congruit && loquax)
    {
        _fracturam_imprimere(v, forma_titulus, scissio, &pf,
            exspectata, numerus_exspectatorum, observata,
            numerus_observatorum, ruptum);
    }
    redde congruit;
}

/* Formam totam probare (BIPARTITA: omnes scissiones; loquax = prima
 * fractura imprimitur) */
interior b32
_formam_probare (
                  Piscina* piscina,
    constans VectorInitus* v,
                      i32  forma,
       constans character* forma_titulus,
                      b32  loquax)
{
    i32 k;

    si (forma != FORMA_BIPARTITA)
    {
        redde _cursum_probare(piscina, v, forma, forma_titulus, -I,
            loquax);
    }
    per (k = I; k < v->mensura; k++)
    {
        si (!_cursum_probare(piscina, v, forma, forma_titulus, (s32)k,
                loquax))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ================================================================
 * Tabulam currere
 * ================================================================ */

interior constans i32 FORMAE[] = {
    FORMA_INTEGRA, FORMA_BIPARTITA, FORMA_SINGULA, FORMA_SEQUENS
};

interior constans character* FORMARUM_TITULI[] = {
    "integra", "bipartita", "singula", "sequens"
};

interior i32 numerus_vectorum;
interior i32 numerus_formarum;
interior i32 numerus_debitorum;

interior vacuum
_tabulam_currere (
                  Piscina* piscina,
       constans character* titulus_tabulae,
    constans VectorInitus* tabula,
                      i32  numerus)
{
    i32 j;
    i32 f;

    imprimere("\n--- Vectores: %s (%u) ---\n", titulus_tabulae,
        (insignatus integer)numerus);
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        constans VectorInitus* v = &tabula[j];

        numerus_vectorum++;
        /* debitum sine causa = error tabulae */
        CREDO_VERUM (v->formae_debitae == VECTOR_VALET
                     || v->causa != NIHIL);
        CREDO_VERUM (v->mensura + I < OCTETI_MAXIMI);

        per (f = ZEPHYRUM; f < IV; f++)
        {
            i32 forma   = FORMAE[f];
            b32 debita  = (v->formae_debitae & forma) != ZEPHYRUM;
            b32 congruit;

            /* scissio nulla in vectore unius octeti */
            si (   (forma == FORMA_BIPARTITA || forma == FORMA_SINGULA)
                && v->mensura < II)
            {
                perge;
            }
            numerus_formarum++;
            congruit = _formam_probare(piscina, v, forma,
                FORMARUM_TITULI[f], !debita);
            si (debita)
            {
                si (congruit)
                {
                    imprimere("  DEBITUM SOLUTUM - promove: \"%s\" [%s]\n",
                        v->titulus, FORMARUM_TITULI[f]);
                    CREDO_CULPA ("debitum solutum - promove");
                }
                alioquin
                {
                    imprimere("  debitum manet: \"%s\" [%s] (%s)\n",
                        v->titulus, FORMARUM_TITULI[f], v->causa);
                    numerus_debitorum++;
                }
            }
            alioquin
            {
                CREDO_VERUM (congruit);
            }
        }
    }
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_tessera_vectores",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    _tabulam_currere(piscina, "semen",
        VECTORES_SEMEN,
        (i32)(magnitudo(VECTORES_SEMEN)
            / magnitudo(VECTORES_SEMEN[0])));
    _tabulam_currere(piscina, "claves (OpenTUI parse.keypress)",
        VECTORES_CLAVIUM,
        (i32)(magnitudo(VECTORES_CLAVIUM)
            / magnitudo(VECTORES_CLAVIUM[0])));

    imprimere("\nvectores %u, formae probatae %u, debita manentia %u\n",
        (insignatus integer)numerus_vectorum,
        (insignatus integer)numerus_formarum,
        (insignatus integer)numerus_debitorum);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
