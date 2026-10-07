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
#include "stilus_terminalis.h"
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

/* effectus capti (B2): responsa appenduntur, tituli numerantur */
nomen structura {
     i8 responsa[CCLVI];
    i32 responsa_mensura;
    i32 tituli;
    i32 titulus_mensura;
     i8 titulus[CCLVI];
} Capta;

interior vacuum
responsum_capere (
             vacuum* datum,
        constans i8* octeti,
                i32  n)
{
    Capta* c;
      i32  k;

    c = (Capta*)datum;
    per (k = ZEPHYRUM; k < n && c->responsa_mensura < CCLVI; k++)
    {
        c->responsa[c->responsa_mensura++] = octeti[k];
    }
}

interior vacuum
titulum_capere (
     vacuum* datum,
     chorda  titulus)
{
    Capta* c;

    c = (Capta*)datum;
    c->tituli++;
    c->titulus_mensura = titulus.mensura;
    memcpy(c->titulus, titulus.datum,
        (memoriae_index)(titulus.mensura < CCLVI ? titulus.mensura
                                                 : CCLVI));
}

interior b32
responsum_est (
                 Capta* c,
    constans character* expectatum)
{
    b32 par;

    par = c->responsa_mensura == (i32)strlen(expectatum)
        && memcmp(c->responsa, expectatum,
              (memoriae_index)c->responsa_mensura) == ZEPHYRUM;
    c->responsa_mensura = ZEPHYRUM;
    redde par;
}

interior Aemulator*
capientem_creare (
                 Capta* c,
    constans character* titulus,
    constans character* versio)
{
    AemulatorConfiguratio cfg;

    memset(c, ZEPHYRUM, magnitudo(Capta));
    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo            = X;
    cfg.altitudo            = III;
    cfg.effectus.datum      = c;
    cfg.effectus.responsum  = responsum_capere;
    cfg.effectus.titulus    = titulum_capere;
    cfg.titulus             = titulus;
    cfg.versio              = versio;
    redde aemulator_creare(piscina, &cfg);
}

/* XV: responsa et effectus (B2) quae vectores non exprimunt */
interior vacuum
responsa_probare (vacuum)
{
                Aemulator* a;
                    Capta  c;
    AemulatorConfiguratio  cfg;
                character  identitas[XVI];
                character  titulus[II * MXXIV + XVI];
           memoriae_index  usus;
                      i32  i;

    imprimere("\n--- XV: responsa et effectus (B2) ---\n");
    /* identitas configurata; chordae in creatione copiatae */
    strcpy(identitas, "rhubarb");
    a = capientem_creare(&c, identitas, "7");
    CREDO_NON_NIHIL(a);
    identitas[ZEPHYRUM] = 'X';
    scribere(a, "\x1B[>q");
    CREDO_VERUM(responsum_est(&c, "\x1BP>|rhubarb 7\x1B\\"));
    /* NIHIL = defaltae */
    a = capientem_creare(&c, NIHIL, NIHIL);
    scribere(a, "\x1B[>q");
    CREDO_VERUM(responsum_est(&c, "\x1BP>|aemulator " AEMULATOR_VERSIO
                                  "\x1B\\"));
    /* initiare defaltas ponit */
    aemulator_configuratio_initiare(&cfg);
    CREDO_VERUM(strcmp(cfg.titulus, "aemulator") == ZEPHYRUM);
    CREDO_VERUM(strcmp(cfg.versio, AEMULATOR_VERSIO) == ZEPHYRUM);
    /* series trans vocationes scissae: responsum unum, titulus unus */
    scribere(a, "\x1B[2;3H\x1B[");
    CREDO_VERUM(responsum_est(&c, ""));
    scribere(a, "6n");
    CREDO_VERUM(responsum_est(&c, "\x1B[2;3R"));
    scribere(a, "\x1B]2;ab");
    CREDO_AEQUALIS_I32(c.tituli, ZEPHYRUM);
    scribere(a, "c\x07");
    CREDO_AEQUALIS_I32(c.tituli, I);
    CREDO_AEQUALIS_I32(c.titulus_mensura, III);
    CREDO_VERUM(memcmp(c.titulus, "abc", III) == ZEPHYRUM);
    /* sine effectibus: quaestiones et tituli tacite consumuntur */
    a = creare(X, III);
    scribere(a, "\x1B[c\x1B[5n\x1B[6n\x1B]2;t\x07\x1B[>q");
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
    CREDO_VERUM(textus_est(a, ""));
    /* titulus longus: II*MXXIV - II octeti (cum '2;') nuntiatur; ultra
     * limitem totus abicitur et numeratur (Ghostty osc
     * change_window_title.zig:36 "longer than buffer"). Divergentia
     * nominata: Ghostty usque ad MMXLVII accipit, nos MMXLVI. */
    a = capientem_creare(&c, NIHIL, NIHIL);
    strcpy(titulus, "\x1B]2;");
    memset(titulus + IV, 'a', II * MXXIV - II);
    strcpy(titulus + IV + II * MXXIV - II, "\x07");
    scribere(a, titulus);
    CREDO_AEQUALIS_I32(c.tituli, I);
    CREDO_AEQUALIS_I32(c.titulus_mensura, II * MXXIV - II);
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
    strcpy(titulus, "\x1B]2;");
    memset(titulus + IV, 'a', II * MXXIV + II);
    strcpy(titulus + IV + II * MXXIV + II, "\x07");
    scribere(a, titulus);
    CREDO_AEQUALIS_I32(c.tituli, I);
    CREDO_AEQUALIS_I32(aemulator_ignota(a), I);
    scribere(a, "ok");
    CREDO_VERUM(textus_est(a, "ok"));
    /* responsa et tituli status constans: nihil allocant */
    a     = capientem_creare(&c, NIHIL, NIHIL);
    usus  = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < M; i++)
    {
        scribere(a, "\x1B[c\x1B[>c\x1B[=c\x1B[5n\x1B[6n\x1B[>q"
                    "\x1B]0;titulus\x07\x1B]1;i\x07");
        c.responsa_mensura = ZEPHYRUM;
    }
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);
    CREDO_AEQUALIS_I32(c.tituli, M);
}

/* XVI: DECRQCRA (sub vexillo lectio_schirmi) et DECSTR (B4b) */
interior vacuum
lectionem_probare (vacuum)
{
                Aemulator* a;
                    Capta  c;
    AemulatorConfiguratio  cfg;
         AemulatorCellula  prima;
         AemulatorCellula  secunda;
                      i32  i;

    imprimere("\n--- XVI: DECRQCRA et DECSTR (B4b) ---\n");
    aemulator_configuratio_initiare(&cfg);
    CREDO_FALSUM(cfg.lectio_schirmi);
    memset(&c, ZEPHYRUM, magnitudo(Capta));
    cfg.latitudo            = X;
    cfg.altitudo            = II;
    cfg.effectus.datum      = &c;
    cfg.effectus.responsum  = responsum_capere;
    cfg.lectio_schirmi      = VERUM;
    a                       = aemulator_creare(piscina, &cfg);
    scribere(a, "AB");
    /* cellula una; rectangulum; vacua = spatium (xterm >= 334) */
    scribere(a, "\x1B[7;0;1;1;1;1*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP7!~0041\x1B\\"));
    scribere(a, "\x1B[8;0;1;1;1;2*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP8!~0083\x1B\\"));
    scribere(a, "\x1B[8;0;1;5;1;5*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP8!~0020\x1B\\"));
    /* ordinaria: schirmum totum (0x83 + XVIII spatia) */
    scribere(a, "\x1B[9*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP9!~02C3\x1B\\"));
    /* rectangulum inversum: 0; limites praecisi ad schirmum */
    scribere(a, "\x1B[3;0;2;1;1;1*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP3!~0000\x1B\\"));
    scribere(a, "\x1B[4;0;1;1;9;999*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP4!~02C3\x1B\\"));
    /* lata: caput = punctum codicis, cauda = 0; summa XVI bitorum */
    scribere(a, "\x1B[H\x1B[2J\xE4\xB8\xAD");
    scribere(a, "\x1B[5;0;1;1;1;1*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP5!~4E2D\x1B\\"));
    scribere(a, "\x1B[5;0;1;2;1;2*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP5!~0000\x1B\\"));
    scribere(a, "\x1B[H");
    per (i = ZEPHYRUM; i < X; i++)
    {
        scribere(a, "\xE4\xB8\xAD");
    }
    scribere(a, "\x1B[6*y");
    CREDO_VERUM(responsum_est(&c, "\x1BP6!~0DC2\x1B\\"));
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);

    /* DECSTR: calamus nativus (SGR normalis) */
    a = creare(X, II);
    scribere(a, "b\x1B[1;31m\x1B[!pa");
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &prima));
    CREDO_VERUM(aemulator_cellula(a, I, ZEPHYRUM, &secunda));
    CREDO_VERUM(stilus_aequalis(&prima.stilus, &secunda.stilus));
    /* DECSTR in schirmo altero: servatus eius ad initium */
    a = creare(X, V);
    /* "\x1B" "7": effugium hex omnes digitos sequentes caperet */
    scribere(a, "\x1B[?1049h\x1B[3;3H\x1B" "7\x1B[!p\x1B[4;4H\x1B" "8");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_cursor(a).y, ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
}

/* lineas numeratas "ab".."ab+n-1" scribere, quamque cum \r\n */
interior vacuum
lineas_ab (
    Aemulator* a,
          i32  ab,
          i32  n)
{
    character b[XXIV];
          i32 i;

    per (i = ab; i < ab + n; i++)
    {
        sprintf(b, "%u\r\n", (unsigned)i);
        scribere(a, b);
    }
}

interior vacuum
lineas_numeratas (
    Aemulator* a,
          i32  n)
{
    lineas_ab(a, ZEPHYRUM, n);
}

interior Aemulator*
cum_historia (
    i32 latitudo,
    i32 altitudo,
    i32 octeti)
{
    AemulatorConfiguratio cfg;

    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo         = latitudo;
    cfg.altitudo         = altitudo;
    cfg.historia_octeti  = octeti;
    redde aemulator_creare(piscina, &cfg);
}

interior b32
historia_incipit (
             Aemulator* a,
    constans character* praefixum)
{
    chorda h;

    h = aemulator_historiam_effundere(a, piscina);
    redde h.mensura >= (i32)strlen(praefixum)
        && memcmp(h.datum, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* stilus cellulae visus (x, y) */
interior StilusTerminalis
stilus_visus (
    Aemulator* a,
          i32  x,
          i32  y)
{
    AemulatorCellula c;

    (vacuum)aemulator_visus_cellula(a, x, y, &c);
    redde c.stilus;
}

/* XVII: historia - paginae, evictio, latitudo, stili (C2) */
interior vacuum
historiam_probare (vacuum)
{
           Aemulator* a;
    AemulatorCellula  c;
    StilusTerminalis  st;
    StilusTerminalis  nativus;
           character  b[XXXII];
                 i32  i;

    imprimere("\n--- XVII: historia (C2) ---\n");
    stilus_nativus(&nativus);
    /* pagina una (latitudo XX: CCLXXIII lineae): DXCVI lineae abeunt
     * -> II paginae recyclatae, L manent (DXLVI..DXCV) */
    a = cum_historia(XX, V, I);
    lineas_numeratas(a, DC);
    CREDO_AEQUALIS_I32(aemulator_historia(a), L);
    CREDO_VERUM(historia_incipit(a, "546\n547\n"));
    /* paginae duae: CCLXXIII + L */
    a = cum_historia(XX, V, LXIV * MXXIV + I);
    lineas_numeratas(a, DC);
    CREDO_AEQUALIS_I32(aemulator_historia(a), CCCXXIII);
    CREDO_VERUM(historia_incipit(a, "273\n274\n"));
    /* sine historia: nihil */
    a = cum_historia(XX, V, ZEPHYRUM);
    lineas_numeratas(a, C);
    CREDO_AEQUALIS_I32(aemulator_historia(a), ZEPHYRUM);

    /* latitudo mutata: pagina nova; lineae angustiores (plenae)
     * legibiles, cellula ultra latitudinem suam vacua (non linea
     * proxima); linea latior in historia tota */
    a = cum_historia(XX, III, M * M);
    per (i = ZEPHYRUM; i < V; i++)
    {
        scribere(a, "aaaaaaaaaaaaaaaaaaaa\r");
        scribere(a, i + I < V ? "\n" : "");
    }
    CREDO_AEQUALIS_I32(aemulator_historia(a), II);
    CREDO_VERUM(aemulator_amplitudo(a, XXX, III));
    scribere(a, "\x1B[3;1H\x1B[2K");
    scribere(a, "latior linea quae XXV excedit\r\n\n\n");
    CREDO_AEQUALIS_I32(aemulator_historia(a), V);
    CREDO_VERUM(historia_incipit(a,
        "aaaaaaaaaaaaaaaaaaaa\naaaaaaaaaaaaaaaaaaaa\n"
        "aaaaaaaaaaaaaaaaaaaa\naaaaaaaaaaaaaaaaaaaa\n"
        "latior linea quae XXV excedit"));
    aemulator_visum_movere(a, M);
    CREDO_AEQUALIS_I32(aemulator_visus(a), V);
    CREDO_VERUM(aemulator_visus_cellula(a, XXV, ZEPHYRUM, &c));
    CREDO_AEQUALIS_I32(c.graphema.mensura, ZEPHYRUM);
    CREDO_VERUM(aemulator_visus_cellula(a, XIX, ZEPHYRUM, &c));
    CREDO_AEQUALIS_I32(c.graphema.mensura, I);

    /* involutio mollis in historia servatur */
    a = cum_historia(V, II, M * M);
    scribere(a, "abcdefg\r\n\n\n");
    CREDO_AEQUALIS_I32(aemulator_historia(a), III);
    aemulator_visum_movere(a, M);
    CREDO_VERUM(aemulator_visus_involuta(a, ZEPHYRUM));
    CREDO_FALSUM(aemulator_visus_involuta(a, I));

    /* schirmum alterum: historia nulla et visus ad imum */
    a = cum_historia(V, II, M * M);
    scribere(a, "1\r\n2\r\n3");
    aemulator_visum_movere(a, I);
    CREDO_AEQUALIS_I32(aemulator_visus(a), I);
    scribere(a, "\x1B[?1049hA");
    CREDO_AEQUALIS_I32(aemulator_visus(a), ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_historia(a), ZEPHYRUM);
    aemulator_visum_movere(a, I);
    CREDO_AEQUALIS_I32(aemulator_visus(a), ZEPHYRUM);
    scribere(a, "\x1B[?1049l");
    CREDO_AEQUALIS_I32(aemulator_historia(a), I);

    /* stili historiae collectionem schirmi superant */
    a = cum_historia(XX, V, M * M);
    scribere(a, "\x1B[38;2;255;0;0mR\x1B[0m\r\n\r\n\r\n\r\n\r\n");
    CREDO_AEQUALIS_I32(aemulator_historia(a), I);
    per (i = ZEPHYRUM; i < DC; i++)
    {
        sprintf(b, "\x1B[38;2;0;%u;%umX", (unsigned)(i / CCLVI),
            (unsigned)(i % CCLVI));
        scribere(a, b);
    }
    scribere(a, "\x1B[H\x1B[38;2;255;0;0mR");
    aemulator_visum_movere(a, M);
    st = stilus_visus(a, ZEPHYRUM, ZEPHYRUM);
    aemulator_visum_movere(a, -(s32)M);
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    CREDO_VERUM(stilus_aequalis(&st, &c.stilus));
    CREDO_FALSUM(stilus_aequalis(&st, &nativus));

    /* tabula paginae plena: linea CC stilorum - CXXVII servantur,
     * ceteri nativi (degradatio) */
    a = cum_historia(CC, III, M * M);
    per (i = ZEPHYRUM; i < CC; i++)
    {
        sprintf(b, "\x1B[38;2;1;%u;%umX", (unsigned)(i / CCLVI),
            (unsigned)(i % CCLVI));
        scribere(a, b);
    }
    scribere(a, "\x1B[0m\r\n\r\n\r\n");
    aemulator_visum_movere(a, M);
    st = stilus_visus(a, CXXVI, ZEPHYRUM);
    CREDO_FALSUM(stilus_aequalis(&st, &nativus));
    st = stilus_visus(a, CXXVII, ZEPHYRUM);
    CREDO_VERUM(stilus_aequalis(&st, &nativus));
    /* linea secunda quae cum prima non capitur: pagina nova, integra */
    a = cum_historia(CXX, III, M * M);
    per (i = ZEPHYRUM; i < C; i++)
    {
        sprintf(b, "\x1B[38;2;2;0;%umX", (unsigned)i);
        scribere(a, b);
    }
    scribere(a, "\x1B[0m\r\n");
    per (i = ZEPHYRUM; i < C; i++)
    {
        sprintf(b, "\x1B[38;2;3;0;%umX", (unsigned)i);
        scribere(a, b);
    }
    scribere(a, "\x1B[0m\r\n\r\n\r\n");
    CREDO_AEQUALIS_I32(aemulator_historia(a), II);
    aemulator_visum_movere(a, M);
    st = stilus_visus(a, XCIX, I);
    CREDO_FALSUM(stilus_aequalis(&st, &nativus));
}

/* XVIII: visus sub evictione et limes plenus (C3) */
interior vacuum
visum_probare (vacuum)
{
         Aemulator* a;
            chorda  v;
    memoriae_index  usus;
               i32  i;

    imprimere("\n--- XVIII: visus et limes plenus (C3) ---\n");
    /* paginae duae (CCLXXIII lineae): visus in linea 0; pagina eius
     * evicta -> visus ad historiam vetustissimam (273) praeciditur,
     * deinde ibi manet dum lineae novae veniunt (decisio XXI) */
    a = cum_historia(XX, V, LXIV * MXXIV + I);
    lineas_ab(a, ZEPHYRUM, C);
    aemulator_visum_movere(a, M);
    CREDO_AEQUALIS_I32(aemulator_visus(a), XCVI);
    lineas_ab(a, C, D);
    CREDO_AEQUALIS_I32(aemulator_historia(a), CCCXXIII);
    CREDO_AEQUALIS_I32(aemulator_visus(a), CCCXXIII);
    v = aemulator_visum_effundere(a, piscina);
    CREDO_VERUM(v.mensura > VIII
        && memcmp(v.datum, "273\n274\n", VIII) == ZEPHYRUM);
    /* pagina una: evictio totam historiam aufert, visus ad imum */
    a = cum_historia(XX, V, I);
    lineas_ab(a, ZEPHYRUM, C);
    aemulator_visum_movere(a, M);
    lineas_ab(a, C, CC);
    CREDO_VERUM(aemulator_visus(a) <= aemulator_historia(a));

    /* limes plenus (X MB ordinarii, LXXX x XXIV: CLX paginae x LXVIII
     * = X DCCCLXXX lineae): post impletionem effusio nihil allocat */
    a = creare(LXXX, XXIV);
    per (i = ZEPHYRUM; i < XI * M; i++)
    {
        scribere(a, "linea \x1B[1;31mX\x1B[0m \xE6\xA9\x8B\r\n");
    }
    CREDO_VERUM(aemulator_historia(a) > X * M);
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
    usus = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < III * M; i++)
    {
        scribere(a,
            "linea \x1B[1;32mX\x1B[0m \xE6\xA9\x8B volvitur\r\n");
    }
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);
    CREDO_VERUM(aemulator_historia(a) <= X * M + DCCCLXXX);
}

/* XIX: ED 3 et retractio historiae (C4) */
interior vacuum
retrahere_probare (vacuum)
{
           Aemulator* a;
    AemulatorCellula  c;
    StilusTerminalis  rubrum;
    StilusTerminalis  nativus;
      memoriae_index  usus;
                 i32  i;

    imprimere("\n--- XIX: ED 3 et retractio (C4) ---\n");
    stilus_nativus(&nativus);
    /* stilus et involutio cum linea redeunt. Flavus primum in tabula
     * schirmi (index I) deletus: rubrum in pagina index I, in schirmo
     * II - retractio sine re-internatione flavum redderet */
    a = cum_historia(V, II, M * M);
    scribere(a, "\x1B[33mY\x1B[0m\x1B[2K\r");
    scribere(a, "\x1B[31mabcdefg\x1B[0m\r\n\n\n");
    CREDO_AEQUALIS_I32(aemulator_historia(a), III);
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    CREDO_VERUM(aemulator_amplitudo(a, V, V));
    CREDO_AEQUALIS_I32(aemulator_historia(a), ZEPHYRUM);
    CREDO_VERUM(textus_est(a, "abcde\nfg"));
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    rubrum = c.stilus;
    CREDO_FALSUM(stilus_aequalis(&rubrum, &nativus));
    CREDO_VERUM(aemulator_cellula(a, I, I, &c));
    CREDO_VERUM(stilus_aequalis(&c.stilus, &rubrum));
    /* rubrum vere rubrum: cellula viva SGR 31 comparatur */
    scribere(a, "\x1B[5;5H\x1B[31mR\x1B[0m");
    CREDO_VERUM(aemulator_cellula(a, IV, IV, &c));
    CREDO_VERUM(stilus_aequalis(&c.stilus, &rubrum));
    CREDO_VERUM(aemulator_visus_involuta(a, ZEPHYRUM));
    CREDO_FALSUM(aemulator_visus_involuta(a, I));
    /* linea latior in schirmum angustius: praeciditur */
    a = cum_historia(XX, II, M * M);
    scribere(a, "abcdefghijklmnop\r\n\r\n");
    CREDO_VERUM(aemulator_amplitudo(a, VI, II));
    CREDO_VERUM(aemulator_amplitudo(a, VI, III));
    CREDO_VERUM(textus_est(a, "abcdef"));
    /* ED 3 in schirmo altero historiam primarii non tangit */
    a = cum_historia(V, II, M * M);
    scribere(a, "1\r\n2\r\n3\x1B[?1049h\x1B[3J\x1B[?1049l");
    CREDO_AEQUALIS_I32(aemulator_historia(a), I);
    /* ED 3 deinde impletio: pagina reutitur, nihil allocatur */
    a = cum_historia(XX, V, I);
    lineas_numeratas(a, CCC);
    scribere(a, "\x1B[3J");
    usus = piscina_summa_usus(piscina);
    lineas_numeratas(a, CCC);
    scribere(a, "\x1B[3J");
    lineas_numeratas(a, CCC);
    CREDO_VERUM(piscina_summa_usus(piscina) == usus);
    CREDO_VERUM(aemulator_historia(a) > ZEPHYRUM);
    /* visus post retractionem intra historiam */
    a = cum_historia(V, II, M * M);
    per (i = ZEPHYRUM; i < X; i++)
    {
        scribere(a, "x\r\n");
    }
    aemulator_visum_movere(a, M);
    CREDO_VERUM(aemulator_amplitudo(a, V, VIII));
    CREDO_VERUM(aemulator_visus(a) <= aemulator_historia(a));
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
}

/* XX: lucra parva (D1) quae vectores non exprimunt */
interior vacuum
lucra_probare (vacuum)
{
           Aemulator* a;
    AemulatorCellula  c;
    StilusTerminalis  rubrum_fundus;
    StilusTerminalis  nativus;

    imprimere("\n--- XX: lucra parva (D1) ---\n");
    stilus_nativus(&nativus);
    /* DECALN: fundus calami servatur, crassum tollitur; calamus manet
     * redactus (Ghostty decaln) */
    a = creare(V, II);
    scribere(a, "\x1B[1;48;2;255;0;0m\x1B#8");
    CREDO_VERUM(textus_est(a, "EEEEE\nEEEEE"));
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    CREDO_VERUM((c.stilus.ornamenta & STILUS_CRASSUM) == ZEPHYRUM);
    CREDO_VERUM(c.stilus.color_fundi.genus == STILUS_COLOR_RGB);
    rubrum_fundus = c.stilus;
    scribere(a, "x");
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    CREDO_VERUM(stilus_aequalis(&c.stilus, &rubrum_fundus));
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, I);
    /* RIS: calamus nativus, servatus oblitus (DECRC -> initium) */
    a = creare(X, V);
    scribere(a, "\x1B[31m\x1B[3;4H\x1B" "7\x1B" "c\x1B" "8x");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, I);
    CREDO_AEQUALIS_I32(aemulator_cursor(a).y, ZEPHYRUM);
    CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM, &c));
    CREDO_VERUM(stilus_aequalis(&c.stilus, &nativus));
    /* REP saturatum non pendet (terminus: bis area schirmi) */
    a = creare(X, V);
    CREDO_NON_PENDET(scribere(a, "A\x1B[2147483647b"), MM);
    scribere(a, "A\x1B[2147483647b");
    CREDO_AEQUALIS_I32(aemulator_ignota(a), ZEPHYRUM);
}

/* XXI: modi initus (D2) - instantanea aemulator_modi; bita cruda per
 * DECRQM in vectoribus, summaria muris solum hic videntur */
interior vacuum
modos_probare (vacuum)
{
             Aemulator* a;
         AemulatorModi  m;
       AemulatorCursor  prope;
       AemulatorCursor  longe;

    imprimere("\n--- XXI: modi initus (D2) ---\n");
    a = creare(X, V);
    m = aemulator_modi(a);
    CREDO_FALSUM(m.sagittae_applicationis);
    CREDO_FALSUM(m.tabula_applicationis);
    CREDO_AEQUALIS_I32((i32)m.mus, (i32)AEMULATOR_MUS_NULLUS);
    CREDO_AEQUALIS_I32((i32)m.mus_forma, (i32)AEMULATOR_MUS_FORMA_X10);
    CREDO_FALSUM(m.glutinum);
    CREDO_FALSUM(m.focus);
    CREDO_FALSUM(m.lnm);
    CREDO_AEQUALIS_I32(m.kitty_vexilla, ZEPHYRUM);
    CREDO_VERUM(m.rotula_sagittis);
    scribere(a, "\x1B[?1007l");
    CREDO_FALSUM(aemulator_modi(a).rotula_sagittis);
    scribere(a, "\x1B[?1007h");
    /* kitty (D4): vexilla schirmi ACTIVI */
    scribere(a, "\x1B[>13u");
    CREDO_AEQUALIS_I32(aemulator_modi(a).kitty_vexilla, XIII);
    scribere(a, "\x1B[?1049h");
    CREDO_AEQUALIS_I32(aemulator_modi(a).kitty_vexilla, ZEPHYRUM);
    scribere(a, "\x1B[?1049l\x1B[<u");
    CREDO_AEQUALIS_I32(aemulator_modi(a).kitty_vexilla, ZEPHYRUM);
    /* extractio saturata non pendet (Ghostty: >= VIII purgat) */
    CREDO_NON_PENDET(scribere(a, "\x1B[>1u\x1B[<2147483647u"), MM);
    CREDO_AEQUALIS_I32(aemulator_modi(a).kitty_vexilla, ZEPHYRUM);
    scribere(a, "\x1B[?1h\x1B=\x1B[?2004h\x1B[?1004h\x1B[20h");
    m = aemulator_modi(a);
    CREDO_VERUM(m.sagittae_applicationis);
    CREDO_VERUM(m.tabula_applicationis);
    CREDO_VERUM(m.glutinum);
    CREDO_VERUM(m.focus);
    CREDO_VERUM(m.lnm);
    /* mus: positum ultimum vincit */
    scribere(a, "\x1B[?1000h\x1B[?1003h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus,
                       (i32)AEMULATOR_MUS_OMNIS);
    scribere(a, "\x1B[?9h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus,
                       (i32)AEMULATOR_MUS_X10);
    scribere(a, "\x1B[?1000h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus,
                       (i32)AEMULATOR_MUS_PRESSIO);
    scribere(a, "\x1B[?1002h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus,
                       (i32)AEMULATOR_MUS_TRACTUS);
    /* remotum quodlibet -> nullus, etiam alio posito (Ghostty) */
    scribere(a, "\x1B[?1000l");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus,
                       (i32)AEMULATOR_MUS_NULLUS);
    /* forma: positum ultimum vincit, remotum -> X10 */
    scribere(a, "\x1B[?1006h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus_forma,
                       (i32)AEMULATOR_MUS_FORMA_SGR);
    scribere(a, "\x1B[?1015h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus_forma,
                       (i32)AEMULATOR_MUS_FORMA_URXVT);
    scribere(a, "\x1B[?1005h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus_forma,
                       (i32)AEMULATOR_MUS_FORMA_UTF8);
    scribere(a, "\x1B[?1016h");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus_forma,
                       (i32)AEMULATOR_MUS_FORMA_SGR_PIXELA);
    scribere(a, "\x1B[?1006l");
    CREDO_AEQUALIS_I32((i32)aemulator_modi(a).mus_forma,
                       (i32)AEMULATOR_MUS_FORMA_X10);
    /* DECSTR: sagittae et tabula remota; mus et glutinum manent */
    scribere(a, "\x1B[?1003h\x1B[?1006h\x1B[!p");
    m = aemulator_modi(a);
    CREDO_FALSUM(m.sagittae_applicationis);
    CREDO_FALSUM(m.tabula_applicationis);
    CREDO_AEQUALIS_I32((i32)m.mus, (i32)AEMULATOR_MUS_OMNIS);
    CREDO_AEQUALIS_I32((i32)m.mus_forma, (i32)AEMULATOR_MUS_FORMA_SGR);
    CREDO_VERUM(m.glutinum);
    /* RIS: omnia ordinaria */
    scribere(a, "\x1B" "c");
    m = aemulator_modi(a);
    CREDO_AEQUALIS_I32((i32)m.mus, (i32)AEMULATOR_MUS_NULLUS);
    CREDO_AEQUALIS_I32((i32)m.mus_forma, (i32)AEMULATOR_MUS_FORMA_X10);
    CREDO_FALSUM(m.glutinum);
    CREDO_FALSUM(m.focus);
    CREDO_FALSUM(m.lnm);
    CREDO_VERUM(aemulator_modus(a, VII, VERUM));
    CREDO_VERUM(aemulator_cursor(a).visibilis);
    /* MXLV cyclus: residuum exactum - n et n + M * (V x III) idem
     * (Ghostty "extended reverse wrap bottom wraparound", n = VII) */
    a = creare(V, III);
    scribere(a, "\x1B[?1045hABCDE\r\n1\x1B[7D");
    prope = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(prope.x, IV);
    CREDO_AEQUALIS_I32(prope.y, II);
    a = creare(V, III);
    scribere(a, "\x1B[?1045hABCDE\r\n1\x1B[15007D");
    longe = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(longe.x, prope.x);
    CREDO_AEQUALIS_I32(longe.y, prope.y);
    /* saturatum in columna una non pendet */
    a = creare(I, V);
    CREDO_NON_PENDET(scribere(a, "\x1B[?1045hA\r\nB\x1B[2147483647D"),
                     MM);
}

/* XXII: colores (D5) - configuratio hospitis et lectio vivorum */
interior vacuum
colores_probare (vacuum)
{
                Aemulator* a;
    AemulatorConfiguratio  cfg;
                      i32  tabula[CCLVI];
                      i32  i;

    imprimere("\n--- XXII: colores (D5) ---\n");
    /* ordinarii: tabula xterm, litterae albae, fundus niger, cursor
     * litteras sequitur */
    a = creare(X, V);
    CREDO_AEQUALIS_I32(aemulator_color(a, I), 0xCD0000);
    CREDO_AEQUALIS_I32(aemulator_color(a, CCLV), 0xEEEEEE);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_LITTERAE),
                       0xFFFFFF);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_FUNDI),
                       ZEPHYRUM);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_CURSORIS),
                       0xFFFFFF);
    CREDO_AEQUALIS_I32(aemulator_color(a, CCLIX), ZEPHYRUM);
    /* programma mutat: lectio vivum videt; cursor sequitur */
    scribere(a, "\x1B]4;1;#102030\x07\x1B]10;#405060\x07");
    CREDO_AEQUALIS_I32(aemulator_color(a, I), 0x102030);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_CURSORIS),
                       0x405060);
    /* configuratio hospitis: tabula propria, thema, cursor proprius;
     * restitutio ad configurationem redit */
    per (i = ZEPHYRUM; i < CCLVI; i++)
    {
        tabula[i] = (i32)(i * 0x010101);
    }
    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo         = X;
    cfg.altitudo         = V;
    cfg.historia_octeti  = ZEPHYRUM;
    cfg.color_litterae   = 0x112233;
    cfg.color_fundi      = 0x445566;
    cfg.color_cursoris   = 0x778899;
    cfg.tabula_colorum   = tabula;
    a                    = aemulator_creare(piscina, &cfg);
    tabula[VII]          = 0xABCDEF;
    CREDO_AEQUALIS_I32(aemulator_color(a, VII), 0x070707);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_FUNDI),
                       0x445566);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_CURSORIS),
                       0x778899);
    scribere(a, "\x1B]4;7;#000001\x07\x1B]12;#000002\x07"
                "\x1B]11;#000003\x07");
    CREDO_AEQUALIS_I32(aemulator_color(a, VII), I);
    scribere(a, "\x1B]104;7\x07\x1B]112\x07\x1B]111\x07");
    CREDO_AEQUALIS_I32(aemulator_color(a, VII), 0x070707);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_CURSORIS),
                       0x778899);
    CREDO_AEQUALIS_I32(aemulator_color(a, AEMULATOR_COLOR_FUNDI),
                       0x445566);
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
    scribere(a, "\x1B[99n\x1B#3\x1B]777;x\x07" "Z");
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

    imprimere("\n--- X: motus et series (A2) ---\n");
    a = creare(X, VI);
    scribere(a, "\x1B[3;5H\x1B[2G");
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, I);
    CREDO_AEQUALIS_I32(c.y, II);
    scribere(a, "\x1B[4d");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).y, III);
    scribere(a, "\x1B[E");
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(c.y, IV);
    scribere(a, "\x1B[7`\x1B[2F");
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(c.y, II);
    scribere(a, "\x1B[99;99f");
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, IX);
    CREDO_AEQUALIS_I32(c.y, V);
    /* intermedia (DECRQPSR - DECRQM iam noster, D2), ':' extra SGR,
     * privatum ignotum, SGR ignotum */
    a = creare(X, III);
    scribere(a, "\x1B[1$w\x1B[0 q\x1B[2:3H\x1B[>5c\x1B[99m\x1B[3;3 H");
    CREDO_AEQUALIS_I32(aemulator_ignota(a), VI);
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, ZEPHYRUM);
    /* SGR cum ':' licet */
    scribere(a, "\x1B[4:3mU");
    {
        AemulatorCellula cellula_lecta;

        CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM,
            &cellula_lecta));
        CREDO_VERUM(cellula_lecta.stilus.sublinea
            == STILUS_SUBLINEA_UNDULATA);
    }

    imprimere("\n--- XI: deletio super latas ---\n");
    a = creare(X, III);
    scribere(a, "\xE6\xA9\x8BZ\x1B[1;2H\x1B[K");
    CREDO_VERUM(cellula_est(a, ZEPHYRUM, ZEPHYRUM, "",
        AEMULATOR_ANGUSTA));
    CREDO_VERUM(textus_est(a, ""));
    a = creare(X, III);
    scribere(a, "AB\xE6\xA9\x8BZ\x1B[1;3H\x1B[1K");
    CREDO_VERUM(cellula_est(a, III, ZEPHYRUM, "", AEMULATOR_ANGUSTA));
    CREDO_VERUM(textus_est(a, "    Z"));

    imprimere("\n--- XII: DECSC calamum servat; 1049 limites ---\n");
    a = creare(X, III);
    scribere(a, "\x1B[1m\x1B" "7\x1B[0m\x1B" "8X");
    {
        AemulatorCellula cellula_lecta;

        CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM,
            &cellula_lecta));
        CREDO_VERUM((cellula_lecta.stilus.ornamenta & STILUS_CRASSUM)
            != ZEPHYRUM);
    }
    /* exitus sine servatione: initium */
    a = creare(X, III);
    scribere(a, "\x1B[3;3H\x1B[?1049l");
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(c.y, ZEPHYRUM);
    /* mutatio magnitudinis in altero: primarium servatum (cursor
     * primarii intra altitudinem novam - aliter lineae summae abeunt,
     * ut in VIII) */
    a = creare(X, V);
    scribere(a, "prima\x1B[2;6H\x1B[?1049haltera");
    CREDO_VERUM(aemulator_amplitudo(a, VI, III));
    CREDO_VERUM(aemulator_alterum(a));
    scribere(a, "\x1B[?1049l");
    CREDO_FALSUM(aemulator_alterum(a));
    CREDO_VERUM(textus_est(a, "prima"));
    c = aemulator_cursor(a);
    CREDO_AEQUALIS_I32(c.x, V);
    CREDO_AEQUALIS_I32(c.y, I);

    imprimere("\n--- XIII: stili colliguntur; saturatio ---\n");
    a = creare(X, III);
    /* stilus moriturus (rubrum, index I) ante viridem (index II) in
     * schirmo: collectio viridem ad I movet - cellula renumeranda */
    scribere(a, "\x1B[2;1H\x1B[31mr\x1B[32mg\x1B[2;1H\x1B[0my\x1B[H");
    scribere(a, "calefactio\r");
    usus = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < MM; i++)
    {
        character sgr[XLVIII];

        sprintf(sgr, "\x1B[38;2;%d;%d;0mX\r", (integer)(i % CCLVI),
            (integer)(i / CCLVI));
        scribere(a, sgr);
    }
    post = piscina_summa_usus(piscina);
    CREDO_VERUM(post == usus);
    {
        AemulatorCellula cellula_lecta;

        CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM,
            &cellula_lecta));
        CREDO_VERUM(cellula_lecta.stilus.color_litterae.genus
            == STILUS_COLOR_RGB);
        CREDO_AEQUALIS_I32(cellula_lecta.stilus.color_litterae.valor,
            (i32)((((MM - I) % CCLVI) << XVI) | (((MM - I) / CCLVI)
            << VIII)));
    }
    {
        AemulatorCellula cellula_lecta;

        CREDO_VERUM(aemulator_cellula(a, I, I, &cellula_lecta));
        CREDO_VERUM(chorda_aequalis_literis(cellula_lecta.graphema,
            "g"));
        CREDO_VERUM(cellula_lecta.stilus.color_litterae.genus
            == STILUS_COLOR_TABULA);
        CREDO_AEQUALIS_I32(cellula_lecta.stilus.color_litterae.valor,
            II);
    }
    /* DC stili simul visibiles: tabula plena manet, degradatio */
    a = creare(XL, XX);
    per (i = ZEPHYRUM; i < DC; i++)
    {
        character sgr[XLVIII];

        sprintf(sgr, "\x1B[38;2;%d;%d;1mY", (integer)(i % CCLVI),
            (integer)(i / CCLVI));
        scribere(a, sgr);
    }
    {
        AemulatorCellula cellula_lecta;

        /* ultima: tabula plena -> nativus (numquam ruina) */
        CREDO_VERUM(aemulator_cellula(a, XIX, XIV, &cellula_lecta));
        CREDO_VERUM(chorda_aequalis_literis(cellula_lecta.graphema,
            "Y"));
        CREDO_VERUM(cellula_lecta.stilus.color_litterae.genus
            == STILUS_COLOR_NATIVUS);
        /* post purgationem stili iterum valent */
        scribere(a, "\x1B[0m\x1B[2J\x1B[H\x1B[38;2;1;2;3mW");
        CREDO_VERUM(aemulator_cellula(a, ZEPHYRUM, ZEPHYRUM,
            &cellula_lecta));
        CREDO_AEQUALIS_I32(cellula_lecta.stilus.color_litterae.valor,
            0x010203);
    }

    imprimere("\n--- XIV: regio et sistae (B1) ---\n");
    /* mutatio magnitudinis regionem et sistas ad ordinem reddit */
    a = creare(X, V);
    scribere(a, "\x1B[2;3r\x1B[3g\x1B[1;5H\x1BH");
    CREDO_VERUM(aemulator_amplitudo(a, X, V));
    scribere(a, "\x1B[5;1HA\nB");
    CREDO_VERUM(textus_est(a, "\n\n\nA\n B"));
    scribere(a, "\x1B[1;1H\tT");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, IX);
    /* TBC 0: sista una tollitur */
    a = creare(XX, II);
    scribere(a, "\x1B[1;9H\x1B[0g\x1B[1;1H\t");
    CREDO_AEQUALIS_I32(aemulator_cursor(a).x, XVI);
    /* SD cum pluribus parametris: non SD (xterm), ignotum */
    a = creare(X, III);
    scribere(a, "A\x1B[1;2;3;4;5T");
    CREDO_AEQUALIS_I32(aemulator_ignota(a), I);
    CREDO_VERUM(textus_est(a, "A"));
    /* LNM legitur */
    CREDO_FALSUM(aemulator_modus(a, XX, FALSUM));
    scribere(a, "\x1B[20h");
    CREDO_VERUM(aemulator_modus(a, XX, FALSUM));
    CREDO_FALSUM(aemulator_modus(a, XX, VERUM));
    /* volutio regionis status constans: nihil allocat */
    a = creare(XX, X);
    scribere(a, "\x1B[3;7r\x1B[7;1H\x1B[44m");
    usus = piscina_summa_usus(piscina);
    per (i = ZEPHYRUM; i < M; i++)
    {
        scribere(a, "linea\r\n\x1BM\x1B[2L\x1B[2M\x1B[S\x1B[T");
    }
    post = piscina_summa_usus(piscina);
    CREDO_VERUM(post == usus);

    responsa_probare();
    lectionem_probare();
    historiam_probare();
    visum_probare();
    retrahere_probare();
    lucra_probare();
    modos_probare();
    colores_probare();

    imprimere("\n--- IX: status constans nihil allocat ---\n");
    /* historia pagina una (limes I octeti): calefactio lineas in
     * historiam mittit, pagina allocatur; deinde solum recyclatur
     * (decisio XX) */
    aemulator_configuratio_initiare(&cfg);
    cfg.latitudo         = XX;
    cfg.altitudo         = V;
    cfg.historia_octeti  = I;
    a                    = aemulator_creare(piscina, &cfg);
    scribere(a, "calefactio\r\n\r\n\r\n\r\n\r\n\r\n");
    CREDO_VERUM(aemulator_historia(a) > ZEPHYRUM);
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
