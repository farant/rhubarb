/* probatio_aemulator.c - nucleus emulatoris, quod vectores non tegunt
 * (aemulator-plan A1)
 *
 * Configuratio et limites; UTF-8 scissa trans vocationes, invalida ->
 * U+FFFD; series ignotae numeratae; campana per effectum; tabulatio ad
 * finem; lata in columna ultima (caput) sine CUP; notae iungentes
 * (graphemata v2: abiciuntur); mutatio magnitudinis (praecidere,
 * implere, cursor in schirmo, capacitas geometrica, intra capacitatem
 * nihil allocat); status constans nihil allocat. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

interior Aemulator*
creare (
    i32 latitudo,
    i32 altitudo)
{
    AemulatorConfiguratio cfg;

    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo = latitudo;
    cfg.altitudo = altitudo;
    redde aemulator_creare(piscina, &cfg);
}

interior vacuum
scribere (
             Aemulator* a,
    constans character* textus)
{
    aemulator_scribere(a, (constans i8*)textus, (i32)strlen(textus));
}

interior b32
textus_est (
             Aemulator* a,
    constans character* expectatum)
{
    redde chorda_aequalis_literis(aemulator_textum_effundere(a,
        piscina),
                                  expectatum);
}

interior b32
cellula_est (
              Aemulator* a,
                    i32  x,
                    i32  y,
     constans character* graphema,
      AemulatorLatitudo  latitudo)
{
    AemulatorCellula c;

    redde aemulator_cellula(a, x, y, &c)
        && chorda_aequalis_literis(c.graphema, graphema)
        && c.latitudo == latitudo;
}

interior vacuum
campanam_numerare (
    vacuum* datum)
{
    (*(i32*)datum)++;
}

s32 principale (vacuum)
{
                Aemulator* a;
    AemulatorConfiguratio  cfg;
          AemulatorCursor  c;
                      i32  campanae;
           memoriae_index  usus;
           memoriae_index  post;
                      b32  mutatae;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_aemulator",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I: configuratio et limites ---\n");
    aemulator_configuratio_initiare(&cfg);
    CREDO_AEQUALIS_I32(cfg.latitudo, LXXX);
    CREDO_AEQUALIS_I32(cfg.altitudo, XXIV);
    CREDO_NIHIL(cfg.effectus.campana);
    CREDO_NIHIL(creare(ZEPHYRUM, X));
    CREDO_NIHIL(creare(X, ZEPHYRUM));
    CREDO_NIHIL(creare(IV * MXXIV + I, X));
    a = creare(X, III);
    CREDO_NON_NIHIL(a);
    CREDO_AEQUALIS_I32(aemulator_latitudo(a), X);
    CREDO_AEQUALIS_I32(aemulator_altitudo(a), III);
    c = aemulator_cursor(a);
    CREDO_VERUM(c.visibilis);
    CREDO_FALSUM(c.pendens);
    CREDO_VERUM(aemulator_modus(a, VII, VERUM));
    CREDO_FALSUM(aemulator_cellula(a, X, ZEPHYRUM, NIHIL));

    imprimere("\n--- II: UTF-8 scissa et invalida ---\n");
    a = creare(X, III);
    scribere(a, "\xE6");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, ZEPHYRUM);
    scribere(a, "\xA9");
    scribere(a, "\x8B");
    CREDO_VERUM(cellula_est(a, ZEPHYRUM, ZEPHYRUM, "\xE6\xA9\x8B",
        AEMULATOR_LATA));
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, II);
    a = creare(X, III);
    scribere(a, "\xFF" "A" "\x80");
    CREDO_VERUM(textus_est(a, "\xEF\xBF\xBD" "A" "\xEF\xBF\xBD"));
    /* runa scissa quam series interrumpit: substituta, deinde series */
    a = creare(X, III);
    scribere(a, "\xE6");
    scribere(a, "\x1B[mB");
    CREDO_VERUM(textus_est(a, "\xEF\xBF\xBD" "B"));

    imprimere("\n--- III: series ignotae numerantur ---\n");
    a = creare(X, III);
    scribere(a, "\x1B[5n\x1B" "7\x1B]0;titulus\x07" "Z");
    CREDO_AEQUALIS_I32(aemulator_ignota(a), III);
    CREDO_VERUM(textus_est(a, "Z"));

    imprimere("\n--- IV: campana per effectum ---\n");
    campanae = ZEPHYRUM;
    aemulator_configuratio_initiare(&cfg);
    cfg.effectus.datum    = &campanae;
    cfg.effectus.campana  = campanam_numerare;
    a                     = aemulator_creare(piscina, &cfg);
    scribere(a, "\x07x\x07");
    CREDO_AEQUALIS_I32(campanae, II);
    /* sine effectu: nihil, nulla ruina */
    a = creare(X, III);
    scribere(a, "\x07");

    imprimere("\n--- V: tabulatio ad finem ---\n");
    a = creare(X, III);
    scribere(a, "\t\t\tQ");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, IX);
    CREDO_VERUM(aemulator_cursor(a).pendens);
    CREDO_VERUM(textus_est(a, "         Q"));

    imprimere("\n--- VI: lata in columna ultima ---\n");
    a = creare(III, III);
    scribere(a, "ab\xF0\x9F\x98\x80");
    CREDO_VERUM(cellula_est(a, II, ZEPHYRUM, "", AEMULATOR_CAPUT));
    CREDO_VERUM(cellula_est(a, ZEPHYRUM, I, "\xF0\x9F\x98\x80",
        AEMULATOR_LATA));
    CREDO_VERUM(cellula_est(a, I, I, "", AEMULATOR_CAUDA));
    CREDO_VERUM(textus_est(a, "ab\n\xF0\x9F\x98\x80"));
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, II);
    CREDO_AEQUALIS_I32(c.y, I);

    /* super latam scribere (sine CUP: retrorsum) - cauda vacatur */
    a = creare(X, III);
    scribere(a, "\xE6\xA9\x8B\x08\x08X");
    CREDO_VERUM(cellula_est(a, ZEPHYRUM, ZEPHYRUM, "X",
        AEMULATOR_ANGUSTA));
    CREDO_VERUM(cellula_est(a, I, ZEPHYRUM, "", AEMULATOR_ANGUSTA));
    CREDO_VERUM(textus_est(a, "X"));
    /* super caudam: lata vacatur */
    a = creare(X, III);
    scribere(a, "\xE6\xA9\x8B\x08Y");
    CREDO_VERUM(cellula_est(a, ZEPHYRUM, ZEPHYRUM, "",
        AEMULATOR_ANGUSTA));
    CREDO_VERUM(textus_est(a, " Y"));

    imprimere("\n--- VII: notae iungentes (graphemata v2) ---\n");
    a = creare(X, III);
    scribere(a, "o\xCC\x80" "p");
    CREDO_VERUM(textus_est(a, "op"));
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, II);

    imprimere("\n--- VIII: mutatio magnitudinis ---\n");
    a = creare(X, III);
    scribere(a, "ABCDEFGHIJ");
    CREDO_VERUM(aemulator_cursor(a).pendens);
    CREDO_VERUM(aemulator_amplitudo(a, V, III));
    CREDO_VERUM(textus_est(a, "ABCDE"));
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, IV);
    CREDO_FALSUM(c.pendens);
    /* latior iterum: columnae novae vacuae (refluxus dilatus) */
    CREDO_VERUM(aemulator_amplitudo(a, X, III));
    CREDO_VERUM(textus_est(a, "ABCDE"));
    /* cursor in ima linea: lineae summae abeunt */
    a = creare(V, III);
    scribere(a, "a\r\nb\r\nc");
    CREDO_VERUM(aemulator_amplitudo(a, V, II));
    CREDO_VERUM(textus_est(a, "b\nc"));
    CREDO_AEQUALIS_I32(aemulator_cursor(a).y, I);
    /* altior: lineae novae vacuae infra */
    CREDO_VERUM(aemulator_amplitudo(a, V, IV));
    scribere(a, "\r\n\r\nz");
    CREDO_VERUM(textus_est(a, "b\nc\n\nz"));
    /* magnitudo mala: FALSUM, status integer */
    CREDO_FALSUM(aemulator_amplitudo(a, ZEPHYRUM, IV));
    CREDO_FALSUM(aemulator_amplitudo(a, V, IV * MXXIV + I));
    CREDO_AEQUALIS_I32(aemulator_latitudo(a), V);
    CREDO_VERUM(textus_est(a, "b\nc\n\nz"));
    /* capacitas geometrica: X -> XI capit XV; intra eam nihil
     * allocatur */
    a = creare(X, III);
    CREDO_VERUM(aemulator_amplitudo(a, XI, III));
    /* credo in eadem piscina scribit: mensura sine CREDO intermedio */
    usus     = piscina_summa_usus(piscina);
    mutatae  = aemulator_amplitudo(a, XV, III);
    mutatae  &= aemulator_amplitudo(a, II, I);
    mutatae  &= aemulator_amplitudo(a, XV, III);
    post     = piscina_summa_usus(piscina);
    CREDO_VERUM(mutatae);
    CREDO_VERUM(post == usus);
    usus     = piscina_summa_usus(piscina);
    mutatae  = aemulator_amplitudo(a, XVI, III);
    post     = piscina_summa_usus(piscina);
    CREDO_VERUM(mutatae);
    CREDO_VERUM(post > usus);

    /* volutio: linea summa abit, linea ima nova VACUA */
    a = creare(V, III);
    scribere(a, "a\r\nb\r\nc\r\n");
    CREDO_VERUM(textus_est(a, "b\nc"));
    CREDO_AEQUALIS_I32(aemulator_cursor(a).y, II);

    imprimere("\n--- IX: status constans nihil allocat ---\n");
    a = creare(XX, V);
    scribere(a, "calefactio\r\n");
    usus = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < M; i++)
    {
        scribere(a, "linea \xE6\xA9\x8B volvitur\r\n\x1B[1mX\x07");
    }
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
