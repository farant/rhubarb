/* Probatio pro pagina - editor textus */
#include "latina.h"
#include "fenestra.h"
#include "pagina.h"
#include "piscina.h"
#include <stdio.h>

/* Modi editoris (vim-style) */
nomen enumeratio {
    MODO_NORMAL = ZEPHYRUM,
    MODO_INSERT
} ModoEditor;

int
main (
    void)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
                 Eventus  eventus;
                     b32  currens;
                  Pagina  pagina;
             ModoEditor  modo;
                    character clavis_praecedens;

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("pagina", M * M);
    si (piscina == NIHIL)
    {
        imprimere("Errore: non possum creare piscinam\n");
        return 1;
    }

    /* Configurare fenestram */
    configuratio.titulus  = "Probatio Pagina";
    configuratio.x        = C;
    configuratio.y        = C;
    configuratio.latitudo = DCCC;
    configuratio.altitudo = CDLXXX;
    configuratio.vexilla  = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        imprimere("Errore: non possum creare fenestram\n");
        piscina_destruere(piscina);
        return 1;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    si (tabula == NIHIL)
    {
        imprimere("Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        return 1;
    }

    /* Initiare paginam */
    pagina_initiare(&pagina, "page:test");

    /* Initiare modum vim */
    modo = MODO_NORMAL;
    clavis_praecedens = '\0';

    /* Inserere textum initialem */
    pagina_inserere_chordam(&pagina, "Salve! Hoc est probatio paginae.\n");
    pagina_inserere_chordam(&pagina, "Preme EFFUGIUM ut claudas.\n\n");
    pagina_inserere_chordam(&pagina, "i = insert mode, ESC = normal mode\n");
    pagina_inserere_chordam(&pagina, "Normal: hjkl, wb, o/O, a/i, g/G, dd, dG, d$\n");

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    imprimere("Probatio Pagina - Vim Mode\n");
    imprimere("Normal mode: hjkl navigation, w/b word nav, o/O new line, a/i insert\n");
    imprimere("Insert mode: ESC to normal\n");
    imprimere("Commands: g/G jump, dd/dG/d$ delete\n");
    imprimere("Premere EFFUGIUM ut claudas\n\n");

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Perscrutari eventus ex systemate */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            commutatio (eventus.genus)
            {
                casus EVENTUS_CLAUDERE:
                    currens = FALSUM;
                    frange;

                casus EVENTUS_CLAVIS_DEPRESSUS:
                {
                    clavis_t clavis;
                    character c;

                    clavis = eventus.datum.clavis.clavis;
                    c = eventus.datum.clavis.typus;

                    si (modo == MODO_INSERT)
                    {
                        /* Modo Insert - tractare ut editor textus normalis */
                        si (clavis == CLAVIS_EFFUGIUM || c == XXVII)
                        {
                            /* ESC vel Ctrl-[ - redire ad modum normalem */
                            modo = MODO_NORMAL;
                            clavis_praecedens = '\0';
                        }
                        alioquin si (clavis == CLAVIS_RETRORSUM)
                        {
                            pagina_delere_characterem(&pagina);
                        }
                        alioquin si (clavis == CLAVIS_DELERE)
                        {
                            pagina_delere_characterem_ante(&pagina);
                        }
                        alioquin si (c == '\n' || c == '\r')
                        {
                            character indentatio[C];
                            i32 longitudo_indentationis;
                            i32 i;

                            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                                &pagina, indentatio, C);

                            pagina_inserere_characterem(&pagina, '\n');

                            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                                pagina_inserere_characterem(&pagina, indentatio[i]);
                            }
                        }
                        alioquin si (c == '\t')
                        {
                            pagina_inserere_characterem(&pagina, '\t');
                        }
                        alioquin si (c != '\0' && c >= XXXII && c <= CXXVI)
                        {
                            pagina_inserere_characterem(&pagina, c);
                        }
                    }
                    alioquin si (modo == MODO_NORMAL)
                    {
                        /* Modo Normal - tractare vim commands */
                        si (clavis == CLAVIS_EFFUGIUM)
                        {
                            currens = FALSUM;
                        }
                        /* Movimento hjkl */
                        alioquin si (c == 'h')
                        {
                            pagina_movere_cursor_sinistram(&pagina);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'j')
                        {
                            pagina_movere_cursor_deorsum(&pagina);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'k')
                        {
                            pagina_movere_cursor_sursum(&pagina);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'l')
                        {
                            pagina_movere_cursor_dextram(&pagina);
                            clavis_praecedens = '\0';
                        }
                        /* Navigatio verbi */
                        alioquin si (c == 'w')
                        {
                            pagina_movere_ad_verbum_proximum(&pagina);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'b')
                        {
                            pagina_movere_ad_verbum_praecedens(&pagina);
                            clavis_praecedens = '\0';
                        }
                        /* Intrare modum insert */
                        alioquin si (c == 'i')
                        {
                            modo = MODO_INSERT;
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'a')
                        {
                            pagina_movere_cursor_dextram(&pagina);
                            modo = MODO_INSERT;
                            clavis_praecedens = '\0';
                        }
                        /* Commandos multi-clavis */
                        alioquin si (c == 'd')
                        {
                            si (clavis_praecedens == 'd')
                            {
                                /* dd - delere lineam */
                                i32 initium;
                                i32 finis;

                                initium = pagina_invenire_initium_lineae(&pagina, (i32)pagina.cursor);
                                finis = pagina_invenire_finem_lineae(&pagina, (i32)pagina.cursor);

                                /* Si non ultima linea, includere newline */
                                si (finis < pagina.longitudo) {
                                    finis++;
                                }

                                pagina_ponere_selectionem(&pagina, initium, finis);
                                pagina_delere_selectionem(&pagina);
                                clavis_praecedens = '\0';
                            }
                            alioquin si (clavis_praecedens == '\0')
                            {
                                clavis_praecedens = 'd';
                            }
                        }
                        alioquin si (c == 'G')
                        {
                            si (clavis_praecedens == 'd')
                            {
                                /* dG - delere ad finem */
                                pagina_ponere_selectionem(&pagina, pagina.cursor, pagina.longitudo);
                                pagina_delere_selectionem(&pagina);
                                clavis_praecedens = '\0';
                            }
                            alioquin
                            {
                                /* G - saltare ad finem */
                                pagina_ponere_cursor(&pagina, pagina.longitudo);
                                clavis_praecedens = '\0';
                            }
                        }
                        alioquin si (c == '$')
                        {
                            si (clavis_praecedens == 'd')
                            {
                                /* d$ - delere ad finem lineae */
                                i32 finis;

                                finis = pagina_invenire_finem_lineae(&pagina, (i32)pagina.cursor);
                                pagina_ponere_selectionem(&pagina, pagina.cursor, (i32)finis);
                                pagina_delere_selectionem(&pagina);
                                clavis_praecedens = '\0';
                            }
                            alioquin
                            {
                                /* $ - movere ad finem lineae */
                                pagina_movere_cursor_finis(&pagina);
                                clavis_praecedens = '\0';
                            }
                        }
                        alioquin si (c == 'g')
                        {
                            /* g - saltare ad initium */
                            pagina_ponere_cursor(&pagina, ZEPHYRUM);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == '^')
                        {
                            /* ^ - saltare ad primum characterem non-spatium lineae */
                            i32 initium;
                            s32 i;

                            initium = pagina_invenire_initium_lineae(&pagina, (i32)pagina.cursor);

                            /* Invenire primum characterem non-spatium */
                            per (i = (s32)initium; i < (s32)pagina.longitudo; i++) {
                                character c_temp;

                                c_temp = pagina.buffer[(i32)i];
                                si (c_temp != ' ' && c_temp != '\t') {
                                    frange;
                                }
                            }

                            pagina_ponere_cursor(&pagina, (i32)i);
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'o')
                        {
                            /* o - nova linea post */
                            character indentatio[C];
                            i32 longitudo_indentationis;
                            i32 i;

                            pagina_movere_cursor_finis(&pagina);

                            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                                &pagina, indentatio, C);

                            pagina_inserere_characterem(&pagina, '\n');

                            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                                pagina_inserere_characterem(&pagina, indentatio[i]);
                            }

                            modo = MODO_INSERT;
                            clavis_praecedens = '\0';
                        }
                        alioquin si (c == 'O')
                        {
                            /* O - nova linea ante */
                            character indentatio[C];
                            i32 longitudo_indentationis;
                            i32 i;

                            pagina_movere_cursor_domus(&pagina);

                            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                                &pagina, indentatio, C);

                            pagina_inserere_characterem(&pagina, '\n');
                            pagina_movere_cursor_sinistram(&pagina);

                            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                                pagina_inserere_characterem(&pagina, indentatio[i]);
                            }

                            modo = MODO_INSERT;
                            clavis_praecedens = '\0';
                        }
                        alioquin
                        {
                            /* Clavis non recognita - vacare clavis praecedens */
                            clavis_praecedens = '\0';
                        }
                    }

                    frange;
                }

                ordinarius:
                    frange;
            }
        }

        /* Purgare tabulam pixelorum */
        tabula_pixelorum_vacare(tabula, RGB(XX, XX, XXX));

        /* Reddere paginam */
        pagina_reddere(tabula, &pagina, X, X, LXX, LV, I);

        /* Monstrare indicatorem modi */
        {
            chorda textus_modi;

            si (modo == MODO_INSERT) {
                textus_modi = chorda_ex_literis("-- INSERT --", piscina);
                tabula_pixelorum_pingere_chordam(tabula, X, CDLX,
                    textus_modi, RGB(C, CCLV, C));
            } alioquin {
                textus_modi = chorda_ex_literis("-- NORMAL --", piscina);
                tabula_pixelorum_pingere_chordam(tabula, X, CDLX,
                    textus_modi, RGB(CLXXX, CLXXX, CCLV));
            }
        }

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
