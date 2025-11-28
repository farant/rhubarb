#include "tabula_characterum.h"
#include <stdio.h>

/* ==================================================
 * Initiatio
 * ================================================== */

vacuum
tabula_initiare(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo)
{
    i32 linea;
    i32 columna;
    memoriae_index cellulae_magnitudo;
    memoriae_index indentatio_magnitudo;

    /* Salvare dimensiones */
    tabula->latitudo = latitudo;
    tabula->altitudo = altitudo;

    /* Allocare cellulae (row-major: altitudo * latitudo) */
    cellulae_magnitudo = (memoriae_index)altitudo * (memoriae_index)latitudo * magnitudo(character);
    tabula->cellulae = piscina_allocare(piscina, cellulae_magnitudo);

    /* Allocare indentatio */
    indentatio_magnitudo = (memoriae_index)altitudo * magnitudo(s32);
    tabula->indentatio = piscina_allocare(piscina, indentatio_magnitudo);

    /* Implere cum spatiis - permittit navigare libere */
    /* -1 significat indentatio non posita */
    per (linea = ZEPHYRUM; linea < altitudo; linea++)
    {
        per (columna = ZEPHYRUM; columna < latitudo; columna++)
        {
            tabula_cellula(tabula, linea, columna) = ' ';
        }
        tabula->indentatio[linea] = -I;
    }
}


/* ==================================================
 * Interrogationes (Query Functions)
 * ================================================== */

b32
tabula_est_cellula_vacua(
    character c)
{
    redde (c == '\0' || c == ' ' || c == '\t' || c == TAB_CONTINUATIO);
}

s32
tabula_invenire_finem_contenti(
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    s32 columna;

    si (linea >= tabula->altitudo)
    {
        redde -I;
    }

    /* Scandere retro ab ultima columna
     * Nota: Hic tabs tractantur ut contentum, NON ut whitespace.
     * Hoc differt ab tabula_est_cellula_vacua (quae tabs tractat
     * ut whitespace pro cushion comportamento). Pro finem contenti,
     * tabs sunt contentum preservandum (e.g. sticky indentation). */
    per (columna = (s32)tabula->latitudo - I; columna >= ZEPHYRUM; columna--)
    {
        character c;

        c = tabula_cellula(tabula, linea, (i32)columna);

        /* Solum '\0' et ' ' sunt trailing whitespace - tabs sunt contentum */
        si (c != '\0' && c != ' ')
        {
            redde columna;
        }
    }

    redde -I;  /* Linea vacua */
}

i32
tabula_invenire_initium_contenti(
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    i32 columna;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde tabula->latitudo;
    }

    /* Scandere ab initio */
    per (columna = ZEPHYRUM; columna < tabula->latitudo; columna++)
    {
        si (!tabula_est_cellula_vacua(tabula_cellula(tabula, linea, columna)))
        {
            redde columna;
        }
    }

    redde tabula->latitudo;  /* Linea vacua */
}

s32
tabula_invenire_obicem(
    constans TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna)
{
    i32 columna;

    si (linea >= tabula->altitudo)
    {
        redde -I;
    }

    /* Scandere dextram ad primum non-whitespace */
    per (columna = ab_columna; columna < tabula->latitudo; columna++)
    {
        si (!tabula_est_cellula_vacua(tabula_cellula(tabula, linea, columna)))
        {
            redde (s32)columna;
        }
    }

    redde -I;  /* Nulla contentum post columnam */
}

b32
tabula_est_plena(
    constans TabulaCharacterum* tabula)
{
    /* Tabula plena si ultima linea habet contentum in ultima columna */
    s32 finis;

    finis = tabula_invenire_finem_contenti(tabula, tabula->altitudo - I);

    redde (finis == (s32)tabula->latitudo - I);
}

b32
tabula_est_vacua(
    constans TabulaCharacterum* tabula)
{
    i32 linea;

    per (linea = ZEPHYRUM; linea < tabula->altitudo; linea++)
    {
        si (tabula_invenire_finem_contenti(tabula, linea) >= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Mutationes (Mutation Functions)
 * ================================================== */

character
tabula_trudere_dextram(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna)
{
    i32 columna;
    character overflow;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde '\0';
    }

    si (ab_columna < ZEPHYRUM || ab_columna >= tabula->latitudo)
    {
        redde '\0';
    }

    /* Salvare character qui excidit */
    overflow = tabula_cellula(tabula, linea, tabula->latitudo - I);

    /* Trudere omnes characteres dextram ab columna */
    per (columna = tabula->latitudo - I; columna > ab_columna; columna--)
    {
        tabula_cellula(tabula, linea, columna) = tabula_cellula(tabula, linea, columna - I);
    }

    /* Cellula ab_columna fit vacua (spatium pro novo charactere) */
    tabula_cellula(tabula, linea, ab_columna) = ' ';

    redde overflow;
}

vacuum
tabula_trahere_sinistram(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna)
{
    i32 columna;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde;
    }

    si (ab_columna < ZEPHYRUM || ab_columna >= tabula->latitudo)
    {
        redde;
    }

    /* Trahere omnes characteres sinistram */
    per (columna = ab_columna; columna < tabula->latitudo - I; columna++)
    {
        tabula_cellula(tabula, linea, columna) = tabula_cellula(tabula, linea, columna + I);
    }

    /* Ultima columna fit vacua */
    tabula_cellula(tabula, linea, tabula->latitudo - I) = '\0';
}

b32
tabula_inserere_lineam(
    TabulaCharacterum* tabula,
    i32 ad_lineam)
{
    i32 linea;
    i32 columna;

    si (ad_lineam < ZEPHYRUM || ad_lineam >= tabula->altitudo)
    {
        redde FALSUM;
    }

    /* Verificare si ultima linea habet contentum */
    si (tabula_invenire_finem_contenti(tabula, tabula->altitudo - I) >= ZEPHYRUM)
    {
        redde FALSUM;  /* Non possumus trudere - ultima linea habet contentum */
    }

    /* Trudere lineas deorsum (cellulae et indentatio) */
    per (linea = tabula->altitudo - I; linea > ad_lineam; linea--)
    {
        per (columna = ZEPHYRUM; columna < tabula->latitudo; columna++)
        {
            tabula_cellula(tabula, linea, columna) = tabula_cellula(tabula, linea - I, columna);
        }
        tabula->indentatio[linea] = tabula->indentatio[linea - I];
    }

    /* Vacare lineam novam */
    per (columna = ZEPHYRUM; columna < tabula->latitudo; columna++)
    {
        tabula_cellula(tabula, ad_lineam, columna) = ' ';
    }
    tabula->indentatio[ad_lineam] = -I;  /* Non posita */

    redde VERUM;
}

vacuum
tabula_delere_lineam(
    TabulaCharacterum* tabula,
    i32 linea)
{
    i32 l;
    i32 columna;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde;
    }

    /* Trahere lineas sursum (cellulae et indentatio) */
    per (l = linea; l < tabula->altitudo - I; l++)
    {
        per (columna = ZEPHYRUM; columna < tabula->latitudo; columna++)
        {
            tabula_cellula(tabula, l, columna) = tabula_cellula(tabula, l + I, columna);
        }
        tabula->indentatio[l] = tabula->indentatio[l + I];
    }

    /* Vacare ultimam lineam */
    per (columna = ZEPHYRUM; columna < tabula->latitudo; columna++)
    {
        tabula_cellula(tabula, tabula->altitudo - I, columna) = ' ';
    }
    tabula->indentatio[tabula->altitudo - I] = -I;  /* Non posita */
}

b32
tabula_tractare_overflow(
    TabulaCharacterum* tabula,
    i32 linea,
    character overflow)
{
    i32 linea_seq;
    i32 initium_seq;
    character overflow_seq;

    si (overflow == '\0')
    {
        redde VERUM;  /* Nihil tractandum */
    }

    si (linea < ZEPHYRUM || linea >= tabula->altitudo - I)
    {
        redde FALSUM;  /* Non possumus tractare overflow ex ultima linea */
    }

    linea_seq = linea + I;
    initium_seq = tabula_invenire_initium_contenti(tabula, linea_seq);

    /* Si linea sequens habet indentationem, inserere lineam novam */
    si (initium_seq > ZEPHYRUM && initium_seq < tabula->latitudo)
    {
        /* Linea sequens habet indentationem - inserere lineam novam */
        si (!tabula_inserere_lineam(tabula, linea_seq))
        {
            redde FALSUM;  /* Pagina plena */
        }

        /* Ponere overflow in nova linea */
        tabula_cellula(tabula, linea_seq, ZEPHYRUM) = overflow;
    }
    alioquin
    {
        /* Linea sequens non habet indentationem - trudere et cascade */
        overflow_seq = tabula_trudere_dextram(tabula, linea_seq, ZEPHYRUM);
        tabula_cellula(tabula, linea_seq, ZEPHYRUM) = overflow;

        /* Recursive: tractare overflow ex linea sequente */
        si (overflow_seq != '\0' && !tabula_est_cellula_vacua(overflow_seq))
        {
            redde tabula_tractare_overflow(tabula, linea_seq, overflow_seq);
        }
    }

    redde VERUM;
}


/* ==================================================
 * Operationes Principales
 * ================================================== */

b32
tabula_inserere_characterem(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna,
    character c)
{
    s32 obex;
    character overflow;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde FALSUM;
    }

    si (columna < ZEPHYRUM || columna >= tabula->latitudo)
    {
        redde FALSUM;
    }

    /* Invenire obicem (primum non-whitespace post columnam) */
    obex = tabula_invenire_obicem(tabula, linea, columna);

    si (obex < ZEPHYRUM)
    {
        /* Nulla contentum post columnam - implere spatia et pingere */
        s32 finis_contenti;
        i32 col;

        finis_contenti = tabula_invenire_finem_contenti(tabula, linea);

        /* Implere spatia inter finem contenti et columnam novam */
        per (col = (i32)(finis_contenti + I); col < columna; col++)
        {
            /* Si implendo super '\t', convertere TAB_CONTINUATIO quoque */
            si (tabula_cellula(tabula, linea, col) == '\t' && col + I < tabula->latitudo)
            {
                tabula_cellula(tabula, linea, col + I) = ' ';
            }
            /* Si implendo super TAB_CONTINUATIO, convertere '\t' quoque */
            si (tabula_cellula(tabula, linea, col) == TAB_CONTINUATIO && col > ZEPHYRUM)
            {
                tabula_cellula(tabula, linea, col - I) = ' ';
            }
            tabula_cellula(tabula, linea, col) = ' ';
        }

        tabula_cellula(tabula, linea, columna) = c;
        redde VERUM;
    }

    /* Whitespace inter cursor et contentum?
     * Si sic, solum pingere (consumere whitespace).
     * Si cursor ad obicem (immediate ante contentum), trudere. */
    si ((s32)columna < obex - I)
    {
        /* Whitespace cushion existit - solum pingere */

        /* Si pingimus super '\t', convertere TAB_CONTINUATIO sequens ad spatium */
        si (tabula_cellula(tabula, linea, columna) == '\t' && columna + I < tabula->latitudo)
        {
            tabula_cellula(tabula, linea, columna + I) = ' ';
        }
        /* Si pingimus super TAB_CONTINUATIO, convertere '\t' praecedens ad spatium */
        si (tabula_cellula(tabula, linea, columna) == TAB_CONTINUATIO && columna > ZEPHYRUM)
        {
            tabula_cellula(tabula, linea, columna - I) = ' ';
        }

        tabula_cellula(tabula, linea, columna) = c;
        redde VERUM;
    }

    /* Ad obicem vel supra contentum - trudere dextram */
    overflow = tabula_trudere_dextram(tabula, linea, columna);

    /* Pingere novum characterem */
    tabula_cellula(tabula, linea, columna) = c;

    /* Tractare overflow si necessarium */
    si (overflow != '\0' && !tabula_est_cellula_vacua(overflow))
    {
        si (linea < tabula->altitudo - I)
        {
            redde tabula_tractare_overflow(tabula, linea, overflow);
        }
        alioquin
        {
            /* Ultima linea - character perditus, pagina plena */
            redde FALSUM;
        }
    }

    redde VERUM;
}

vacuum
tabula_delere_characterem(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna)
{
    s32 finis;

    si (linea >= tabula->altitudo)
    {
        redde;
    }

    si (columna >= tabula->latitudo)
    {
        redde;
    }

    /* Verificare si contentum existit ad dextram */
    finis = tabula_invenire_finem_contenti(tabula, linea);

    si (finis >= (s32)columna)
    {
        /* Contentum ad dextram - trahere sinistram */
        tabula_trahere_sinistram(tabula, linea, columna);
    }
    /* Si nulla contentum ad dextram, nihil facere */
}

vacuum
tabula_invenire_contentum_connexum(
    TabulaCharacterum* tabula,
    i32 linea_initium,
    i32 columna_initium,
    i32* linea_finis,
    i32* columna_finis)
{
    i32 linea;
    i32 columna;
    i32 spatia_consecutiva;
    b32 in_contentu;

    *linea_finis = linea_initium;
    *columna_finis = columna_initium;

    linea = linea_initium;
    columna = columna_initium;
    spatia_consecutiva = ZEPHYRUM;
    in_contentu = FALSUM;

    /* Scandere per contentum connexum */
    dum (linea < tabula->altitudo)
    {
        dum (columna < tabula->latitudo)
        {
            character c;

            c = tabula_cellula(tabula, linea, columna);

            si (tabula_est_cellula_vacua(c))
            {
                si (c == ' ')
                {
                    spatia_consecutiva++;
                    si (spatia_consecutiva >= II)
                    {
                        /* Duo spatia - finis connexionis */
                        redde;
                    }
                }
                alioquin
                {
                    /* '\0' vel '\t' post contentum - finis connexionis */
                    si (in_contentu)
                    {
                        redde;
                    }
                    spatia_consecutiva = ZEPHYRUM;
                }
            }
            alioquin
            {
                /* Contentum inventum */
                in_contentu = VERUM;
                spatia_consecutiva = ZEPHYRUM;
                *linea_finis = linea;
                *columna_finis = columna;
            }

            columna++;
        }

        /* Nova linea - continuare si contentum connexum */
        linea++;
        columna = ZEPHYRUM;

        /* Si linea nova incipit cum plus quam uno spatio, finis */
        si (linea < tabula->altitudo)
        {
            i32 initium;

            initium = tabula_invenire_initium_contenti(tabula, linea);

            si (initium > I || initium == tabula->latitudo)
            {
                /* Indentatio vel linea vacua - finis connexionis */
                redde;
            }
        }
    }
}


/* ==================================================
 * Tab Handling (Tab = 2 cellulae)
 * ================================================== */

b32
tabula_inserere_tab(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna)
{
    s32 obex;
    character overflow1;
    character overflow2;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde FALSUM;
    }

    /* Tab requirit 2 cellulas, ergo columna maxima est latitudo - 2 */
    si (columna < ZEPHYRUM || columna >= tabula->latitudo - I)
    {
        redde FALSUM;
    }

    /* Invenire obicem (primum non-whitespace post columnam) */
    obex = tabula_invenire_obicem(tabula, linea, columna);

    si (obex < ZEPHYRUM)
    {
        /* Nulla contentum post columnam - solum pingere tab */
        tabula_cellula(tabula, linea, columna) = '\t';
        tabula_cellula(tabula, linea, columna + I) = TAB_CONTINUATIO;
        redde VERUM;
    }

    /* Contentum existit - trudere dextram bis (pro 2 cellulis) */
    overflow1 = tabula_trudere_dextram(tabula, linea, columna);
    overflow2 = tabula_trudere_dextram(tabula, linea, columna);

    /* Pingere tab */
    tabula_cellula(tabula, linea, columna) = '\t';
    tabula_cellula(tabula, linea, columna + I) = TAB_CONTINUATIO;

    /* Tractare overflow si necessarium */
    si (overflow1 != '\0' && !tabula_est_cellula_vacua(overflow1))
    {
        si (linea < tabula->altitudo - I)
        {
            si (!tabula_tractare_overflow(tabula, linea, overflow1))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde FALSUM;  /* Pagina plena */
        }
    }

    si (overflow2 != '\0' && !tabula_est_cellula_vacua(overflow2))
    {
        si (linea < tabula->altitudo - I)
        {
            si (!tabula_tractare_overflow(tabula, linea, overflow2))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde FALSUM;  /* Pagina plena */
        }
    }

    redde VERUM;
}

vacuum
tabula_delere_tab(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna)
{
    i32 columna_tab;

    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde;
    }

    si (columna < ZEPHYRUM || columna >= tabula->latitudo)
    {
        redde;
    }

    /* Si ad TAB_CONTINUATIO, invenire '\t' (columna - 1) */
    si (tabula_cellula(tabula, linea, columna) == TAB_CONTINUATIO && columna > ZEPHYRUM)
    {
        columna_tab = columna - I;
    }
    alioquin si (tabula_cellula(tabula, linea, columna) == '\t')
    {
        columna_tab = columna;
    }
    alioquin
    {
        /* Non est tab - nihil facere */
        redde;
    }

    /* Delere ambas cellulas: trahere sinistram bis */
    tabula_trahere_sinistram(tabula, linea, columna_tab);
    tabula_trahere_sinistram(tabula, linea, columna_tab);
}

b32
tabula_est_tab_continuatio(
    constans TabulaCharacterum* tabula,
    i32 linea,
    i32 columna)
{
    si (linea < ZEPHYRUM || linea >= tabula->altitudo)
    {
        redde FALSUM;
    }

    si (columna < ZEPHYRUM || columna >= tabula->latitudo)
    {
        redde FALSUM;
    }

    redde (tabula_cellula(tabula, linea, columna) == TAB_CONTINUATIO);
}


/* ==================================================
 * Test Helpers
 * ================================================== */

vacuum
tabula_ex_literis_cum_dimensionibus(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo,
    constans character* literae)
{
    i32 linea;
    i32 columna;
    i32 i;
    b32 indentatio_posita;

    /* Primo initiare cum dimensionibus datis */
    tabula_initiare(tabula, piscina, latitudo, altitudo);

    linea = ZEPHYRUM;
    columna = ZEPHYRUM;
    indentatio_posita = FALSUM;

    per (i = ZEPHYRUM; literae[i] != '\0'; i++)
    {
        si (literae[i] == '\n')
        {
            linea++;
            columna = ZEPHYRUM;
            indentatio_posita = FALSUM;

            si (linea >= tabula->altitudo)
            {
                frange;
            }
        }
        alioquin
        {
            si (columna < tabula->latitudo && linea < tabula->altitudo)
            {
                /* Tractare tab ut duo cellulae: '\t' + TAB_CONTINUATIO */
                si (literae[i] == '\t')
                {
                    tabula_cellula(tabula, linea, columna) = '\t';
                    columna++;
                    si (columna < tabula->latitudo)
                    {
                        tabula_cellula(tabula, linea, columna) = TAB_CONTINUATIO;
                        columna++;
                    }
                }
                alioquin
                {
                    tabula_cellula(tabula, linea, columna) = literae[i];

                    /* Ponere sticky indentatio ad primum non-spatium */
                    si (!indentatio_posita && literae[i] != ' ')
                    {
                        tabula->indentatio[linea] = (s32)columna;
                        indentatio_posita = VERUM;
                    }

                    columna++;
                }
            }
        }
    }
}

vacuum
tabula_ex_literis(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    constans character* literae)
{
    tabula_ex_literis_cum_dimensionibus(
        tabula,
        piscina,
        TABULA_LATITUDO_DEFALTA,
        TABULA_ALTITUDO_DEFALTA,
        literae);
}

b32
tabula_aequalis_literis(
    TabulaCharacterum* tabula,
    constans character* expectatum)
{
    i32 linea;
    i32 columna;
    i32 i;
    character c_tabula;
    character c_expect;

    linea = ZEPHYRUM;
    columna = ZEPHYRUM;
    i = ZEPHYRUM;

    dum (VERUM)
    {
        c_expect = expectatum[i];

        si (c_expect == '\0')
        {
            /* Finis expectati - verificare si tabula habet plus contenti */
            dum (linea < tabula->altitudo)
            {
                dum (columna < tabula->latitudo)
                {
                    c_tabula = tabula_cellula(tabula, linea, columna);
                    si (!tabula_est_cellula_vacua(c_tabula))
                    {
                        redde FALSUM;  /* Tabula habet extra contentum */
                    }
                    columna++;
                }
                linea++;
                columna = ZEPHYRUM;
            }
            redde VERUM;
        }

        si (c_expect == '\n')
        {
            /* Nova linea in expectato - verificare si reliquum lineae est vacuum */
            dum (columna < tabula->latitudo)
            {
                c_tabula = tabula_cellula(tabula, linea, columna);
                si (!tabula_est_cellula_vacua(c_tabula))
                {
                    redde FALSUM;
                }
                columna++;
            }
            linea++;
            columna = ZEPHYRUM;

            si (linea >= tabula->altitudo)
            {
                /* Verificare si expectatum habet plus linearum */
                i++;
                redde (expectatum[i] == '\0');
            }
        }
        alioquin si (c_expect == '\t')
        {
            /* Tab in expectato - verificare '\t' + TAB_CONTINUATIO in tabula */
            si (linea >= tabula->altitudo || columna >= tabula->latitudo - I)
            {
                redde FALSUM;
            }

            si (tabula_cellula(tabula, linea, columna) != '\t')
            {
                redde FALSUM;
            }
            columna++;

            si (tabula_cellula(tabula, linea, columna) != TAB_CONTINUATIO)
            {
                redde FALSUM;
            }
            columna++;
        }
        alioquin
        {
            si (linea >= tabula->altitudo || columna >= tabula->latitudo)
            {
                redde FALSUM;  /* Expectatum excedit limites */
            }

            c_tabula = tabula_cellula(tabula, linea, columna);

            /* Tractare spatia: ' ' in expectato debet match ' ' vel '\0' in tabula? */
            /* Non - debemus esse stricti */
            si (c_tabula != c_expect)
            {
                redde FALSUM;
            }

            columna++;
        }

        i++;
    }
}

vacuum
tabula_imprimere(
    TabulaCharacterum* tabula)
{
    s32 linea;
    s32 ultima_linea_cum_contentu;

    /* Invenire ultimam lineam cum contentu */
    ultima_linea_cum_contentu = -I;
    per (linea = ZEPHYRUM; linea < (s32)tabula->altitudo; linea++)
    {
        si (tabula_invenire_finem_contenti(tabula, (i32)linea) >= ZEPHYRUM)
        {
            ultima_linea_cum_contentu = linea;
        }
    }

    /* Imprimere lineas usque ad ultimam cum contentu */
    per (linea = ZEPHYRUM; linea <= ultima_linea_cum_contentu; linea++)
    {
        s32 finis;
        s32 col;

        finis = tabula_invenire_finem_contenti(tabula, (i32)linea);

        printf("%2d: |", linea);

        /* Si linea vacua (finis < 0), imprimere solum '.' */
        si (finis < ZEPHYRUM)
        {
            printf(".");
        }
        alioquin
        {
            per (col = ZEPHYRUM; col <= finis; col++)
            {
                character c;

                c = tabula_cellula(tabula, (i32)linea, (i32)col);

                si (c == '\0')
                {
                    printf(".");
                }
                alioquin si (c == ' ')
                {
                    printf("_");
                }
                alioquin si (c == '\t')
                {
                    printf(">");
                }
                alioquin
                {
                    printf("%c", c);
                }
            }
        }

        printf("|\n");
    }

    si (ultima_linea_cum_contentu < ZEPHYRUM)
    {
        printf("(tabula vacua)\n");
    }
}

vacuum
tabula_asserere(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    constans character* expectatum,
    constans character* descriptio)
{
    b32 aequalis;

    aequalis = tabula_aequalis_literis(tabula, expectatum);

    si (!aequalis)
    {
        printf("\n=== ASSERTIO FRACTA: %s ===\n", descriptio);
        printf("Expectatum:\n");
        {
            TabulaCharacterum temp;
            tabula_ex_literis(&temp, piscina, expectatum);
            tabula_imprimere(&temp);
        }
        printf("\nActuale:\n");
        tabula_imprimere(tabula);
        printf("===========================\n\n");
    }

    CREDO_VERUM(aequalis);
    (vacuum)descriptio;  /* Usatur pro debug output supra */
}
