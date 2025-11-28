#include "vim.h"

/* ==================================================
 * Constantae Internae
 * ================================================== */

#define FD_TIMEOUT 0.3  /* 300ms pro fd escape */


/* ==================================================
 * Initiatio
 * ================================================== */

VimStatus
vim_initiare(
    TabulaCharacterum* tabula)
{
    VimStatus status;

    status.tabula = tabula;
    status.cursor_linea = ZEPHYRUM;
    status.cursor_columna = ZEPHYRUM;
    status.modo = MODO_VIM_NORMALIS;
    status.selectio_initium_linea = -I;
    status.selectio_initium_columna = -I;
    status.clavis_praecedens = '\0';
    status.esperans_fd = FALSUM;
    status.tempus_f = 0.0;
    status.debet_claudere = FALSUM;
    status.mutatus = FALSUM;

    redde status;
}


/* ==================================================
 * Utilitas Interna
 * ================================================== */

/* Verificare si character est pars verbi (alphanumericus vel underscore) */
hic_manens b32
_est_character_verbi(
    character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

/* Invenire ultimam lineam cum contentu in tabula */
hic_manens i32
_ultima_linea_cum_contentu(
    TabulaCharacterum* tabula)
{
    s32 linea;

    per (linea = (s32)TABULA_ALTITUDO - I; linea >= ZEPHYRUM; linea--)
    {
        si (tabula_invenire_finem_contenti(tabula, (i32)linea) >= ZEPHYRUM)
        {
            redde (i32)linea;
        }
    }

    redde ZEPHYRUM;
}

/* Invenire finem contenti in linea currenti (pro cursor clamping) */
hic_manens s32
_finis_lineae(
    TabulaCharacterum* tabula,
    i32 linea)
{
    s32 finis;

    finis = tabula_invenire_finem_contenti(tabula, linea);

    si (finis < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    redde finis;
}

/* Clamp cursor ad limites tabulae (2D grid - libera navigatio) */
hic_manens VimStatus
_clamp_cursor(
    VimStatus status)
{
    /* Clamp linea */
    si (status.cursor_linea < ZEPHYRUM)
    {
        status.cursor_linea = ZEPHYRUM;
    }
    si (status.cursor_linea >= (s32)TABULA_ALTITUDO)
    {
        status.cursor_linea = TABULA_ALTITUDO - I;
    }

    /* Clamp columna - 2D grid permittit navigare libere */
    si (status.cursor_columna < ZEPHYRUM)
    {
        status.cursor_columna = ZEPHYRUM;
    }
    si (status.cursor_columna >= (s32)TABULA_LATITUDO)
    {
        status.cursor_columna = TABULA_LATITUDO - I;
    }

    redde status;
}


/* ==================================================
 * Motus Cursoris
 * ================================================== */

hic_manens VimStatus
_movere_sinistram(
    VimStatus status)
{
    si (status.cursor_columna > ZEPHYRUM)
    {
        status.cursor_columna--;

        /* Si landed on TAB_CONTINUATIO, skip to '\t' */
        si (tabula_est_tab_continuatio(status.tabula, status.cursor_linea, status.cursor_columna))
        {
            si (status.cursor_columna > ZEPHYRUM)
            {
                status.cursor_columna--;
            }
        }
    }

    redde status;
}

hic_manens VimStatus
_movere_dextram(
    VimStatus status)
{
    /* 2D grid - libera navigatio */
    si (status.cursor_columna < TABULA_LATITUDO - I)
    {
        status.cursor_columna++;

        /* Si landed on TAB_CONTINUATIO, skip past it */
        si (tabula_est_tab_continuatio(status.tabula, status.cursor_linea, status.cursor_columna))
        {
            si (status.cursor_columna < TABULA_LATITUDO - I)
            {
                status.cursor_columna++;
            }
        }
    }

    redde status;
}

hic_manens VimStatus
_movere_sursum(
    VimStatus status)
{
    si (status.cursor_linea > ZEPHYRUM)
    {
        status.cursor_linea--;
        status = _clamp_cursor(status);
    }

    redde status;
}

hic_manens VimStatus
_movere_deorsum(
    VimStatus status)
{
    si (status.cursor_linea < TABULA_ALTITUDO - I)
    {
        status.cursor_linea++;
        status = _clamp_cursor(status);
    }

    redde status;
}

hic_manens VimStatus
_movere_initium_lineae(
    VimStatus status)
{
    status.cursor_columna = ZEPHYRUM;

    redde status;
}

hic_manens VimStatus
_movere_finem_lineae(
    VimStatus status)
{
    s32 finis;

    finis = _finis_lineae(status.tabula, status.cursor_linea);

    si (status.modo == MODO_VIM_NORMALIS)
    {
        status.cursor_columna = (i32)finis;
    }
    alioquin
    {
        /* In insert mode, move past last character */
        status.cursor_columna = (i32)(finis + I);
        si (status.cursor_columna >= (s32)TABULA_LATITUDO)
        {
            status.cursor_columna = TABULA_LATITUDO - I;
        }
    }

    redde status;
}

hic_manens VimStatus
_movere_initium_contenti(
    VimStatus status)
{
    i32 initium;

    initium = tabula_invenire_initium_contenti(status.tabula, status.cursor_linea);

    si (initium < TABULA_LATITUDO)
    {
        status.cursor_columna = initium;
    }

    redde status;
}

/* w - movere ad initium verbi proximi */
hic_manens VimStatus
_movere_verbum_proximum(
    VimStatus status)
{
    s32 finis;
    character c;
    b32 in_verbo;

    finis = _finis_lineae(status.tabula, status.cursor_linea);

    /* Si cursor ultra contentum, movere ad proximam lineam */
    si ((s32)status.cursor_columna >= finis)
    {
        si (status.cursor_linea < _ultima_linea_cum_contentu(status.tabula))
        {
            status.cursor_linea++;
            status.cursor_columna = tabula_invenire_initium_contenti(
                status.tabula, status.cursor_linea);
        }
        redde status;
    }

    c = status.tabula->cellulae[status.cursor_linea][status.cursor_columna];
    in_verbo = _est_character_verbi(c);

    /* Saltare characteres similes (verbum vel non-verbum) */
    dum ((s32)status.cursor_columna < finis)
    {
        c = status.tabula->cellulae[status.cursor_linea][status.cursor_columna];

        si (_est_character_verbi(c) != in_verbo)
        {
            frange;
        }

        /* Si spatium, saltare et invenire proximum non-spatium */
        si (c == ' ' || c == '\t')
        {
            frange;
        }

        status.cursor_columna++;
    }

    /* Saltare spatia */
    dum ((s32)status.cursor_columna <= finis)
    {
        c = status.tabula->cellulae[status.cursor_linea][status.cursor_columna];

        si (c != ' ' && c != '\t' && c != '\0')
        {
            frange;
        }

        status.cursor_columna++;
    }

    /* Si ultra finem lineae, movere ad proximam lineam */
    si ((s32)status.cursor_columna > finis)
    {
        si (status.cursor_linea < _ultima_linea_cum_contentu(status.tabula))
        {
            status.cursor_linea++;
            status.cursor_columna = tabula_invenire_initium_contenti(
                status.tabula, status.cursor_linea);
        }
        alioquin
        {
            status.cursor_columna = (i32)finis;
        }
    }

    redde status;
}

/* b - movere ad initium verbi praecedentis */
hic_manens VimStatus
_movere_verbum_praecedens(
    VimStatus status)
{
    s32 finis;
    character c;

    /* Si ad initium lineae, movere ad lineam superiorem */
    si (status.cursor_columna == ZEPHYRUM)
    {
        si (status.cursor_linea > ZEPHYRUM)
        {
            status.cursor_linea--;
            finis = _finis_lineae(status.tabula, status.cursor_linea);
            status.cursor_columna = (i32)finis;
        }
        redde status;
    }

    /* Movere sinistram unum */
    status.cursor_columna--;

    /* Saltare spatia retro */
    dum (status.cursor_columna > ZEPHYRUM)
    {
        c = status.tabula->cellulae[status.cursor_linea][status.cursor_columna];

        si (c != ' ' && c != '\t' && c != '\0')
        {
            frange;
        }

        status.cursor_columna--;
    }

    /* Invenire initium verbi */
    dum (status.cursor_columna > ZEPHYRUM)
    {
        character c_prior;

        c = status.tabula->cellulae[status.cursor_linea][status.cursor_columna];
        c_prior = status.tabula->cellulae[status.cursor_linea][status.cursor_columna - I];

        /* Si character prior est spatium vel typus differens, sumus ad initium */
        si (c_prior == ' ' || c_prior == '\t' || c_prior == '\0')
        {
            frange;
        }

        si (_est_character_verbi(c) != _est_character_verbi(c_prior))
        {
            frange;
        }

        status.cursor_columna--;
    }

    redde status;
}

/* g - movere ad initium tabulae */
hic_manens VimStatus
_movere_initium_tabulae(
    VimStatus status)
{
    status.cursor_linea = ZEPHYRUM;
    status.cursor_columna = ZEPHYRUM;

    redde status;
}

/* G - movere ad finem tabulae */
hic_manens VimStatus
_movere_finem_tabulae(
    VimStatus status)
{
    i32 ultima;
    s32 finis;

    ultima = _ultima_linea_cum_contentu(status.tabula);
    status.cursor_linea = ultima;

    finis = _finis_lineae(status.tabula, status.cursor_linea);
    status.cursor_columna = (i32)finis;

    redde status;
}


/* ==================================================
 * Operationes Editandi
 * ================================================== */

hic_manens VimStatus
_inserere_characterem(
    VimStatus status,
    character c)
{
    b32 successus;
    i32 linea;
    i32 columna;

    linea = status.cursor_linea;
    columna = status.cursor_columna;

    successus = tabula_inserere_characterem(
        status.tabula,
        linea,
        columna,
        c);

    si (successus)
    {
        /* Actualizare sticky indentatio si character non-spatium */
        /* -1 significat "non posita", 0+ est indentatio valida */
        si (c != ' ' && c != '\t')
        {
            /* Solum ponere si nondum posita (prima character non-spatium) */
            si (status.tabula->indentatio[linea] < ZEPHYRUM)
            {
                status.tabula->indentatio[linea] = (s32)columna;
            }
        }

        status.cursor_columna++;
        status.mutatus = VERUM;
    }

    redde status;
}

hic_manens VimStatus
_delere_characterem_sub_cursore(
    VimStatus status)
{
    s32 finis;

    finis = _finis_lineae(status.tabula, status.cursor_linea);

    si (finis >= ZEPHYRUM && (s32)status.cursor_columna <= finis)
    {
        tabula_delere_characterem(
            status.tabula,
            status.cursor_linea,
            status.cursor_columna);

        status.mutatus = VERUM;

        /* Clamp cursor si necessarium */
        status = _clamp_cursor(status);
    }

    redde status;
}

hic_manens VimStatus
_delere_characterem_ante_cursorem(
    VimStatus status)
{
    si (status.cursor_columna > ZEPHYRUM)
    {
        character c_ante;

        c_ante = status.tabula->cellulae[status.cursor_linea][status.cursor_columna - I];

        /* Si character ante est TAB_CONTINUATIO, delere totum tab */
        si (c_ante == TAB_CONTINUATIO && status.cursor_columna >= II)
        {
            tabula_delere_tab(status.tabula, status.cursor_linea, status.cursor_columna - I);
            status.cursor_columna -= II;  /* Movere cursor retro per 2 */
            status.mutatus = VERUM;
        }
        alioquin si (c_ante == '\t')
        {
            /* Cursor est ad TAB_CONTINUATIO, delere totum tab */
            tabula_delere_tab(status.tabula, status.cursor_linea, status.cursor_columna - I);
            status.cursor_columna--;
            status.mutatus = VERUM;
        }
        alioquin
        {
            /* Normal backspace */
            status.cursor_columna--;

            tabula_delere_characterem(
                status.tabula,
                status.cursor_linea,
                status.cursor_columna);

            status.mutatus = VERUM;
        }
    }
    alioquin si (status.cursor_linea > ZEPHYRUM)
    {
        /* Ad initium lineae - jungere cum linea superiore */
        s32 finis_superioris;

        status.cursor_linea--;
        finis_superioris = _finis_lineae(status.tabula, status.cursor_linea);
        status.cursor_columna = (i32)(finis_superioris + I);

        /* TODO: trahere contentum lineae inferioris sursum */
        status.mutatus = VERUM;
    }

    redde status;
}

hic_manens VimStatus
_delere_lineam(
    VimStatus status)
{
    tabula_delere_lineam(status.tabula, status.cursor_linea);

    status.mutatus = VERUM;

    /* Clamp cursor */
    status = _clamp_cursor(status);

    redde status;
}

/* d$ - delere ad finem lineae */
hic_manens VimStatus
_delere_ad_finem_lineae(
    VimStatus status)
{
    s32 finis;
    s32 col;

    finis = _finis_lineae(status.tabula, status.cursor_linea);

    /* Delere omnes characteres a cursore ad finem */
    per (col = finis; col >= (s32)status.cursor_columna; col--)
    {
        tabula_delere_characterem(status.tabula, status.cursor_linea, (i32)col);
    }

    status.mutatus = VERUM;
    status = _clamp_cursor(status);

    redde status;
}

/* dG - delere ad finem tabulae */
hic_manens VimStatus
_delere_ad_finem_tabulae(
    VimStatus status)
{
    i32 ultima;
    s32 linea;

    ultima = _ultima_linea_cum_contentu(status.tabula);

    /* Delere omnes lineas a cursore ad finem */
    per (linea = (s32)ultima; linea >= (s32)status.cursor_linea; linea--)
    {
        tabula_delere_lineam(status.tabula, (i32)linea);
    }

    status.mutatus = VERUM;
    status = _clamp_cursor(status);

    redde status;
}

/* >> - augere indentationem lineae */
hic_manens VimStatus
_augere_indentationem(
    VimStatus status)
{
    s32 finis;
    s32 col;

    finis = _finis_lineae(status.tabula, status.cursor_linea);

    /* Si linea habet contentum, trudere dextram et inserere tab/spatia */
    si (finis >= ZEPHYRUM)
    {
        /* Trudere totam lineam dextram */
        per (col = finis; col >= ZEPHYRUM; col--)
        {
            character c;

            c = status.tabula->cellulae[status.cursor_linea][(i32)col];
            si ((i32)col + II < TABULA_LATITUDO)
            {
                status.tabula->cellulae[status.cursor_linea][(i32)col + II] = c;
            }
        }

        /* Inserere duo spatia ad initium */
        status.tabula->cellulae[status.cursor_linea][ZEPHYRUM] = ' ';
        status.tabula->cellulae[status.cursor_linea][I] = ' ';

        status.mutatus = VERUM;
    }

    redde status;
}

/* << - minuere indentationem lineae */
hic_manens VimStatus
_minuere_indentationem(
    VimStatus status)
{
    i32 initium;
    s32 finis;
    i32 numerus_deletorum;
    s32 col;

    initium = tabula_invenire_initium_contenti(status.tabula, status.cursor_linea);
    finis = _finis_lineae(status.tabula, status.cursor_linea);

    /* Computare quantum indentationis delere (maximum II) */
    numerus_deletorum = ZEPHYRUM;
    si (initium >= II)
    {
        numerus_deletorum = II;
    }
    alioquin si (initium >= I)
    {
        numerus_deletorum = I;
    }

    si (numerus_deletorum > ZEPHYRUM && finis >= ZEPHYRUM)
    {
        /* Trahere contentum sinistram */
        per (col = (s32)initium; col <= finis; col++)
        {
            status.tabula->cellulae[status.cursor_linea][(i32)col - numerus_deletorum] =
                status.tabula->cellulae[status.cursor_linea][(i32)col];
        }

        /* Vacare cellulas ad finem */
        per (col = finis - (s32)numerus_deletorum + I; col <= finis; col++)
        {
            status.tabula->cellulae[status.cursor_linea][(i32)col] = '\0';
        }

        /* Adjustare cursor si necessarium */
        si (status.cursor_columna >= numerus_deletorum)
        {
            status.cursor_columna -= numerus_deletorum;
        }
        alioquin
        {
            status.cursor_columna = ZEPHYRUM;
        }

        status.mutatus = VERUM;
    }

    redde status;
}

/* Copiare indentationem ex linea ad lineam novam (usus sticky metadata) */
hic_manens VimStatus
_copiare_indentationem(
    VimStatus status,
    i32 linea_fons)
{
    s32 indentatio;
    i32 col;
    i32 linea_scan;
    s32 linea_cum_indentatio;

    indentatio = -I;
    linea_cum_indentatio = -I;

    /* Primo: verificare si linea_fons habet contentum et whitespace ad initium
     * Hoc permittit user adjustare indentationem (backspace tabs)
     * et Enter respectat adjustationem, non metadata antiquam
     *
     * NOTA: tabula_initiare implet cellulas cum spatiis, ergo linea "vacua"
     * habet 68 spatia. Solum scandimus whitespace si linea habet contentum
     * (aliquid non-whitespace) */
    {
        s32 finis_contenti;

        finis_contenti = tabula_invenire_finem_contenti(status.tabula, linea_fons);

        /* Solum scandere whitespace si linea habet contentum */
        si (finis_contenti >= ZEPHYRUM)
        {
            i32 indent_finis;

            indent_finis = ZEPHYRUM;
            col = ZEPHYRUM;

            /* Scandere whitespace (tabs et spatia) ad initium lineae
             * Scanndimus usque ad finem contenti, non usque ad finem lineae
             * (quia tabula_initiare implet cum spatiis) */
            dum (col <= (i32)finis_contenti)
            {
                character c;

                c = status.tabula->cellulae[linea_fons][col];

                si (c == '\t')
                {
                    /* Tab inventa - saltare TAB_CONTINUATIO quoque */
                    indent_finis = col + II;
                    col += II;
                }
                alioquin si (c == ' ')
                {
                    /* Spatium inventa */
                    indent_finis = col + I;
                    col++;
                }
                alioquin
                {
                    frange;
                }
            }

            /* Si whitespace inventum ante contentum, usare illud */
            si (indent_finis > ZEPHYRUM)
            {
                indentatio = (s32)indent_finis;
                linea_cum_indentatio = (s32)linea_fons;
            }
        }
    }

    /* Si nulla tabs in linea_fons, quaerere retro pro metadata
     * Hoc est "sticky" behavior pro spatiis */
    si (indentatio < ZEPHYRUM)
    {
        per (linea_scan = linea_fons; linea_scan >= ZEPHYRUM; linea_scan--)
        {
            si (status.tabula->indentatio[linea_scan] >= ZEPHYRUM)
            {
                indentatio = status.tabula->indentatio[linea_scan];
                linea_cum_indentatio = (s32)linea_scan;
                frange;
            }
        }
    }

    /* Si inventa indentatio valida, copiare ad lineam currentem */
    si (indentatio > ZEPHYRUM && indentatio < TABULA_LATITUDO && linea_cum_indentatio >= ZEPHYRUM)
    {
        /* Copiare characteres indentationis (preservat tabs!) */
        per (col = ZEPHYRUM; col < (i32)indentatio; col++)
        {
            status.tabula->cellulae[status.cursor_linea][col] =
                status.tabula->cellulae[(i32)linea_cum_indentatio][col];
        }

        /* NON ponere indentatio metadata - permittere typing determinare
         * Hoc permittit user typare plus spatia et obtinere majorem indentationem
         * indentatio[linea] manet -1 usque ad primum non-spatium characterem */
        status.cursor_columna = (i32)indentatio;
    }

    redde status;
}

/* o - inserere lineam novam infra cum auto-indent */
hic_manens VimStatus
_inserere_lineam_novam_infra(
    VimStatus status)
{
    b32 successus;
    i32 linea_fons;

    linea_fons = status.cursor_linea;
    successus = tabula_inserere_lineam(status.tabula, status.cursor_linea + I);

    si (successus)
    {
        status.cursor_linea++;
        status.cursor_columna = ZEPHYRUM;
        status.modo = MODO_VIM_INSERERE;
        status.mutatus = VERUM;

        /* Auto-indent: copiare indentationem ex linea superiore */
        status = _copiare_indentationem(status, linea_fons);
    }

    redde status;
}

/* O - inserere lineam novam supra cum auto-indent */
hic_manens VimStatus
_inserere_lineam_novam_supra(
    VimStatus status)
{
    b32 successus;
    i32 linea_fons;

    linea_fons = status.cursor_linea;
    successus = tabula_inserere_lineam(status.tabula, status.cursor_linea);

    si (successus)
    {
        status.cursor_columna = ZEPHYRUM;
        status.modo = MODO_VIM_INSERERE;
        status.mutatus = VERUM;

        /* Auto-indent: copiare indentationem ex linea inferiore (quae nunc est linea_fons + 1) */
        status = _copiare_indentationem(status, linea_fons + I);
    }

    redde status;
}

/* Enter in insert mode - nova linea cum auto-indent */
hic_manens VimStatus
_inserere_novam_lineam_in_inserere(
    VimStatus status)
{
    b32 successus;
    i32 linea_fons;

    linea_fons = status.cursor_linea;
    successus = tabula_inserere_lineam(status.tabula, status.cursor_linea + I);

    si (successus)
    {
        status.cursor_linea++;
        status.cursor_columna = ZEPHYRUM;
        status.mutatus = VERUM;

        /* Auto-indent: copiare indentationem ex linea superiore */
        status = _copiare_indentationem(status, linea_fons);
    }

    redde status;
}


/* ==================================================
 * Tractare Modus Normalis
 * ================================================== */

hic_manens VimStatus
_tractare_normalis(
    VimStatus status,
    s32 clavis)
{
    /* Multi-key commands */
    si (status.clavis_praecedens == 'd')
    {
        status.clavis_praecedens = '\0';

        si (clavis == 'd')
        {
            /* dd - delete line */
            redde _delere_lineam(status);
        }
        alioquin si (clavis == '$')
        {
            /* d$ - delete to end of line */
            redde _delere_ad_finem_lineae(status);
        }
        alioquin si (clavis == 'G')
        {
            /* dG - delete to end of file */
            redde _delere_ad_finem_tabulae(status);
        }

        redde status;
    }

    si (status.clavis_praecedens == '>')
    {
        status.clavis_praecedens = '\0';

        si (clavis == '>')
        {
            /* >> - indent line */
            redde _augere_indentationem(status);
        }

        redde status;
    }

    si (status.clavis_praecedens == '<')
    {
        status.clavis_praecedens = '\0';

        si (clavis == '<')
        {
            /* << - dedent line */
            redde _minuere_indentationem(status);
        }

        redde status;
    }

    /* Single key commands */
    commutatio (clavis)
    {
        /* Motus */
        casus 'h':
        casus VIM_CLAVIS_SINISTRAM:
            redde _movere_sinistram(status);

        casus 'l':
        casus VIM_CLAVIS_DEXTRAM:
            redde _movere_dextram(status);

        casus 'k':
        casus VIM_CLAVIS_SURSUM:
            redde _movere_sursum(status);

        casus 'j':
        casus VIM_CLAVIS_DEORSUM:
            redde _movere_deorsum(status);

        casus '0':
        casus VIM_CLAVIS_HOME:
            redde _movere_initium_lineae(status);

        casus '$':
        casus VIM_CLAVIS_END:
            redde _movere_finem_lineae(status);

        casus '^':
            redde _movere_initium_contenti(status);

        /* Word motion */
        casus 'w':
            redde _movere_verbum_proximum(status);

        casus 'b':
            redde _movere_verbum_praecedens(status);

        /* File motion */
        casus 'g':
            redde _movere_initium_tabulae(status);

        casus 'G':
            redde _movere_finem_tabulae(status);

        /* Modus inserere */
        casus 'i':
            status.modo = MODO_VIM_INSERERE;
            redde status;

        casus 'a':
            status.modo = MODO_VIM_INSERERE;
            redde _movere_dextram(status);

        casus 'A':
            status.modo = MODO_VIM_INSERERE;
            redde _movere_finem_lineae(status);

        casus 'I':
            status.modo = MODO_VIM_INSERERE;
            redde _movere_initium_contenti(status);

        casus 'o':
            redde _inserere_lineam_novam_infra(status);

        casus 'O':
            redde _inserere_lineam_novam_supra(status);

        /* Deletio */
        casus 'x':
        casus VIM_CLAVIS_DELETE:
            redde _delere_characterem_sub_cursore(status);

        casus 'd':
            status.clavis_praecedens = 'd';
            redde status;

        /* Indentation */
        casus '>':
            status.clavis_praecedens = '>';
            redde status;

        casus '<':
            status.clavis_praecedens = '<';
            redde status;

        /* Escape */
        casus VIM_CLAVIS_ESCAPE:
            status.debet_claudere = VERUM;
            redde status;

        ordinarius:
            redde status;
    }

    redde status;
}


/* ==================================================
 * Tractare Modus Inserere
 * ================================================== */

hic_manens VimStatus
_tractare_inserere(
    VimStatus status,
    s32 clavis,
    f64 tempus)
{
    /* fd escape sequence */
    si (status.esperans_fd)
    {
        si (clavis == 'd' && (tempus - status.tempus_f) < FD_TIMEOUT)
        {
            /* fd detectum - redire ad modum normalem */
            status.esperans_fd = FALSUM;

            /* Delere 'f' quod insertum est */
            status = _delere_characterem_ante_cursorem(status);

            status.modo = MODO_VIM_NORMALIS;
            status = _clamp_cursor(status);

            redde status;
        }
        alioquin
        {
            status.esperans_fd = FALSUM;
        }
    }

    commutatio (clavis)
    {
        casus VIM_CLAVIS_ESCAPE:
            status.modo = MODO_VIM_NORMALIS;
            status = _clamp_cursor(status);
            redde status;

        casus 'f':
            status.esperans_fd = VERUM;
            status.tempus_f = tempus;
            redde _inserere_characterem(status, 'f');

        casus VIM_CLAVIS_BACKSPACE:
            redde _delere_characterem_ante_cursorem(status);

        casus VIM_CLAVIS_DELETE:
            redde _delere_characterem_sub_cursore(status);

        casus VIM_CLAVIS_ENTER:
            redde _inserere_novam_lineam_in_inserere(status);

        casus VIM_CLAVIS_TAB:
            /* Tab occupat 2 cellulas: '\t' + TAB_CONTINUATIO */
            si (tabula_inserere_tab(status.tabula, status.cursor_linea, status.cursor_columna))
            {
                status.cursor_columna += II;  /* Movere cursor per 2 */
                status.mutatus = VERUM;
            }
            redde status;

        casus VIM_CLAVIS_SINISTRAM:
            redde _movere_sinistram(status);

        casus VIM_CLAVIS_DEXTRAM:
            redde _movere_dextram(status);

        casus VIM_CLAVIS_SURSUM:
            redde _movere_sursum(status);

        casus VIM_CLAVIS_DEORSUM:
            redde _movere_deorsum(status);

        ordinarius:
            /* Inserere character normalis */
            si (clavis >= ' ' && clavis <= '~')
            {
                redde _inserere_characterem(status, (character)clavis);
            }
            redde status;
    }

    redde status;
}


/* ==================================================
 * Tractare Claves (Publicum)
 * ================================================== */

VimStatus
vim_tractare_clavem(
    VimStatus status,
    s32 clavis)
{
    redde vim_tractare_clavem_cum_tempore(status, clavis, 0.0);
}

VimStatus
vim_tractare_clavem_cum_tempore(
    VimStatus status,
    s32 clavis,
    f64 tempus)
{
    status.mutatus = FALSUM;
    status.debet_claudere = FALSUM;

    commutatio (status.modo)
    {
        casus MODO_VIM_NORMALIS:
            redde _tractare_normalis(status, clavis);

        casus MODO_VIM_INSERERE:
            redde _tractare_inserere(status, clavis, tempus);

        casus MODO_VIM_VISUALIS:
            /* TODO: implementare */
            redde status;

        ordinarius:
            redde status;
    }

    redde status;
}


/* ==================================================
 * Interrogationes
 * ================================================== */

constans character*
vim_nomen_modi(
    ModoVim modo)
{
    commutatio (modo)
    {
        casus MODO_VIM_NORMALIS:
            redde "NORMAL";

        casus MODO_VIM_INSERERE:
            redde "INSERT";

        casus MODO_VIM_VISUALIS:
            redde "VISUAL";

        ordinarius:
            redde "?";
    }
}


/* ==================================================
 * Utilitas
 * ================================================== */

VimStatus
vim_ponere_cursor(
    VimStatus status,
    i32 linea,
    i32 columna)
{
    status.cursor_linea = linea;
    status.cursor_columna = columna;

    redde _clamp_cursor(status);
}

VimStatus
vim_ponere_modum(
    VimStatus status,
    ModoVim modo)
{
    status.modo = modo;

    redde status;
}
