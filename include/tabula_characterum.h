#ifndef TABULA_CHARACTERUM_H
#define TABULA_CHARACTERUM_H

#include "latina.h"
#include "piscina.h"
#include "credo.h"

/* ==================================================
 * TABULA CHARACTERUM - 2D Character Grid
 *
 * Pure 2D character grid for text editing with
 * barrier-based insertion and intelligent content pushing.
 *
 * PHILOSOPHIA:
 * - Grid dimensions are configurable at creation time
 * - Default: 68 columns × 56 rows
 * - Typing "paints" into cells
 * - Barrier = last whitespace before content
 * - Typing into barrier pushes content forward
 * - Indentation-aware overflow handling
 *
 * EXEMPLUM:
 *   TabulaCharacterum tabula;
 *   tabula_initiare(&tabula, piscina, 68, 56);
 *   tabula_inserere_characterem(&tabula, 0, 0, 'H');
 *   tabula_inserere_characterem(&tabula, 0, 1, 'i');
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

/* Dimensiones defaltae - usantur si non specificatae */
#define TABULA_LATITUDO_DEFALTA   LXVIII    /* 68 columns */
#define TABULA_ALTITUDO_DEFALTA   LVI       /* 56 rows */

/* Backwards compatibility - mapped to defaults */
#define TABULA_LATITUDO   TABULA_LATITUDO_DEFALTA
#define TABULA_ALTITUDO   TABULA_ALTITUDO_DEFALTA

/* Tab occupat duas cellulas: '\t' sequitur TAB_CONTINUATIO
 * Hoc permittit tabs reddere ut duo spatia et delere uno backspace */
#define TAB_CONTINUATIO   ((character)0x01)


/* ==================================================
 * Typi
 * ================================================== */

/* Pure 2D character grid - no cursor, no UI state
 * Cellulae allocantur dynamice ex piscina */
nomen structura {
    i32 latitudo;       /* Numerus columnarum */
    i32 altitudo;       /* Numerus linearum */
    character* cellulae;  /* [altitudo * latitudo], row-major */
    s32* indentatio;      /* [altitudo] - Sticky indentation per linea (-1 = non posita) */
} TabulaCharacterum;

/* Macro pro accessu cellulae - row-major layout */
#define tabula_cellula(tabula, linea, columna) \
    ((tabula)->cellulae[(linea) * (tabula)->latitudo + (columna)])


/* ==================================================
 * Initiatio
 * ================================================== */

/* Initiare tabulam cum dimensionibus specificatis
 * Allocat cellulae et indentatio ex piscina
 * Omnes cellulae initiantur ad '\0'
 *
 * tabula: tabula initianda
 * piscina: piscina pro allocatione
 * latitudo: numerus columnarum
 * altitudo: numerus linearum
 */
vacuum
tabula_initiare(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo);


/* ==================================================
 * Interrogationes (Query Functions)
 * ================================================== */

/* Invenire finem contenti in linea
 * Reddit columnam ultimi non-whitespace, vel -1 si linea vacua
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 */
s32
tabula_invenire_finem_contenti(
    constans TabulaCharacterum* tabula,
    i32 linea);

/* Invenire initium contenti in linea (post indentationem)
 * Reddit columnam primi non-whitespace, vel latitudo si vacua
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 */
i32
tabula_invenire_initium_contenti(
    constans TabulaCharacterum* tabula,
    i32 linea);

/* Invenire obicem (barrier) in linea ab columna data
 * Obex est columna primi non-whitespace post columnam datam
 * Reddit columnam obicis, vel -1 si nulla contentum post columnam
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * ab_columna: columna ab qua quaerere (0 ad latitudo-1)
 */
s32
tabula_invenire_obicem(
    constans TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna);

/* Verificare si cellula est vacua (whitespace vel '\0')
 *
 * c: character verificandus
 *
 * Reddit: VERUM si vacua
 */
b32
tabula_est_cellula_vacua(
    character c);

/* Verificare si tabula est plena
 * Tabula est plena si ultima linea habet contentum in ultima columna
 *
 * tabula: tabula
 *
 * Reddit: VERUM si plena
 */
b32
tabula_est_plena(
    constans TabulaCharacterum* tabula);

/* Verificare si tabula est vacua (omnes cellulae vacuae)
 *
 * tabula: tabula
 *
 * Reddit: VERUM si vacua
 */
b32
tabula_est_vacua(
    constans TabulaCharacterum* tabula);


/* ==================================================
 * Mutationes (Mutation Functions)
 * ================================================== */

/* Trudere contentum dextram in linea ab columna data
 * Reddit characterem qui excidit (overflow) ex columna ultima, vel '\0'
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * ab_columna: columna ab qua trudere (0 ad latitudo-1)
 *
 * Reddit: character qui excidit, vel '\0' si nihil
 */
character
tabula_trudere_dextram(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna);

/* Trahere contentum sinistram in linea ab columna data
 * Cellulae post contentum fiunt '\0'
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * ab_columna: columna ab qua trahere (0 ad latitudo-1)
 */
vacuum
tabula_trahere_sinistram(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 ab_columna);

/* Inserere lineam vacuam ad positionem, trudendo sequentes deorsum
 * Reddit FALSUM si ultima linea habet contentum (non potest trudere)
 *
 * tabula: tabula
 * ad_lineam: linea ubi inserere (0 ad altitudo-1)
 *
 * Reddit: VERUM si successus
 */
b32
tabula_inserere_lineam(
    TabulaCharacterum* tabula,
    i32 ad_lineam);

/* Delere lineam, trahendo sequentes sursum
 * Ultima linea fit vacua
 *
 * tabula: tabula
 * linea: linea delenda (0 ad altitudo-1)
 */
vacuum
tabula_delere_lineam(
    TabulaCharacterum* tabula,
    i32 linea);

/* Tractare overflow ex linea in lineam sequentem
 * Considerat indentationem: si linea sequens habet indentationem,
 * inserit lineam novam pro overflow
 *
 * tabula: tabula
 * linea: linea ex qua overflow venit (0 ad altitudo-2)
 * overflow: character qui excidit
 *
 * Reddit: VERUM si successus, FALSUM si pagina plena
 */
b32
tabula_tractare_overflow(
    TabulaCharacterum* tabula,
    i32 linea,
    character overflow);


/* ==================================================
 * Operationes Principales
 * ================================================== */

/* Inserere characterem ad positionem cum barrier/push logic
 *
 * Si nulla contentum post positionem: pingere characterem
 * Si contentum existit: trudere contentum, deinde pingere
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * columna: columna (0 ad latitudo-1)
 * c: character inserendus
 *
 * Reddit: VERUM si successus, FALSUM si pagina plena
 */
b32
tabula_inserere_characterem(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna,
    character c);

/* Delere characterem ad positionem, trahens contentum sinistram
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * columna: columna (0 ad latitudo-1)
 */
vacuum
tabula_delere_characterem(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna);

/* Invenire extensionem contenti connexi (pro backspace pull-up)
 * Contentum connexum = verba separata per singulum spatium
 * Duo spatia vel cellulae vacuae frangunt connexionem
 *
 * tabula: tabula
 * linea_initium: linea initii (0 ad altitudo-1)
 * columna_initium: columna initii (0 ad latitudo-1)
 * linea_finis: [exitus] linea finis contenti connexi
 * columna_finis: [exitus] columna finis contenti connexi
 */
vacuum
tabula_invenire_contentum_connexum(
    TabulaCharacterum* tabula,
    i32 linea_initium,
    i32 columna_initium,
    i32* linea_finis,
    i32* columna_finis);


/* ==================================================
 * Tab Handling (Tab = 2 cellulae)
 * ================================================== */

/* Inserere tab ad positionem (occupat 2 cellulas: '\t' + TAB_CONTINUATIO)
 * Si contentum existit ad dextram, trudere dextram per 2
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * columna: columna (0 ad latitudo-2, tab requirit 2 cellulas)
 *
 * Reddit: VERUM si successus, FALSUM si non potest inserere
 */
b32
tabula_inserere_tab(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna);

/* Delere tab ad positionem (vel ad TAB_CONTINUATIO)
 * Si columna habet TAB_CONTINUATIO, delere totum tab (ambas cellulas)
 * Si columna habet '\t', delere ambas cellulas
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * columna: columna (0 ad latitudo-1)
 */
vacuum
tabula_delere_tab(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna);

/* Verificare si cellula est TAB_CONTINUATIO (pars secunda tab)
 *
 * tabula: tabula
 * linea: linea (0 ad altitudo-1)
 * columna: columna (0 ad latitudo-1)
 *
 * Reddit: VERUM si TAB_CONTINUATIO
 */
b32
tabula_est_tab_continuatio(
    constans TabulaCharacterum* tabula,
    i32 linea,
    i32 columna);


/* ==================================================
 * Test Helpers
 * ================================================== */

/* Creare tabula ex string literal cum dimensionibus specificatis
 * Initiat tabulam cum dimensionibus datis et implet ex literis
 * '\n' = nova linea, alii characteres = cellulae
 * Trailing whitespace in lineis preservatur
 *
 * tabula: tabula initianda et implenda
 * piscina: piscina pro allocatione
 * latitudo: numerus columnarum
 * altitudo: numerus linearum
 * literae: string cum '\n' pro lineis novis
 */
vacuum
tabula_ex_literis_cum_dimensionibus(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo,
    constans character* literae);

/* Creare tabula ex string literal (cum dimensionibus defaltis)
 * Initiat tabulam cum dimensionibus defaltis et implet ex literis
 * '\n' = nova linea, alii characteres = cellulae
 * Trailing whitespace in lineis preservatur
 *
 * tabula: tabula initianda et implenda
 * piscina: piscina pro allocatione
 * literae: string cum '\n' pro lineis novis
 */
vacuum
tabula_ex_literis(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    constans character* literae);

/* Comparare tabula ad string expectatum
 * Format: lineae separatae per '\n', trailing whitespace significat
 *
 * tabula: tabula comparanda
 * expectatum: string expectatum
 *
 * Reddit: VERUM si aequalis
 */
b32
tabula_aequalis_literis(
    TabulaCharacterum* tabula,
    constans character* expectatum);

/* Imprimere tabula ad stdout (pro debugging)
 * Monstrat grid cum '.' pro cellulis vacuis ('\0')
 * Solum imprimit usque ad ultimam lineam cum contentu
 *
 * tabula: tabula imprimenda
 */
vacuum
tabula_imprimere(
    TabulaCharacterum* tabula);

/* Assertio helper - comparat et imprimit si non aequalis
 * Usus cum CREDO framework
 *
 * tabula: tabula verificanda
 * piscina: piscina pro tabula temporaria (pro imprimendo expectatum)
 * expectatum: string expectatum
 * descriptio: descriptio assertionis
 */
vacuum
tabula_asserere(
    TabulaCharacterum* tabula,
    Piscina* piscina,
    constans character* expectatum,
    constans character* descriptio);

#endif /* TABULA_CHARACTERUM_H */
