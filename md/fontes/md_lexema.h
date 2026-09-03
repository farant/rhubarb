/* md_lexema.h - Tabula linearum, scanners praefixorum, fabrica lexematum
 *
 * Markdown lexari sine contextu NON potest: an '>' marca sit an
 * textus ex indentatione et continentibus apertis pendet (CommonMark
 * par. V - algorithmus continentium). Ergo hic NON est lexator
 * fluxus totius (ut css_lexema) sed TRES res quas parsator blocorum
 * per lineam vocat, exemplar scanners.re cmark:
 *
 *  I.   TABULA LINEARUM: fons in lineas scissus (contentum +
 *       terminator), semel; parsator lineas ordine visitat.
 *  II.  SCANNERS: recognitores super [ab, ad) contenti lineae unius -
 *       indentatio, citatio, marca listae, officium, ATX, subductio,
 *       divisio, saeptum, separator tabulae. Extenta reddunt, lexemata
 *       NON creant: quis octetos possideat parsator decernit (una
 *       linea, unus possessor per octetum - spec par. III).
 *  III. FABRICA: lexema ex extento, linea/columna ex tabula linearum
 *       (I-basatae, columna = octetus in linea).
 *
 * md_lexare_crude = fluxus ingenuus ([INDENTATIO] [TEXTUS] [LINEA] per
 * lineam, FINIS) - porta A2 circuitus octetorum super corpus totum;
 * parsator eo NON utitur.
 *
 * TABULAE: columnae ad multiplum IV proximum expanduntur (CommonMark
 * par. 2.2). Tabula PARTIM sumpta (continens columnas II ex IV
 * poscit) octetum unum in possessores duos NON scindit: lexema
 * tabulam totam fert, parsator columnas residuas contento adnumerat
 * (differentia in valore DERIVATO solum apparet - codex indentatus
 * intra elementum). Corpus tabulas ducentes ZEPHYRUM habet; exempla
 * CommonMark paucas.
 */

#ifndef MD_LEXEMA_H
#define MD_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "materia_token.h"


/* ==================================================
 * Tabula linearum
 * ================================================== */

nomen structura {
    s32 offset;       /* initium contenti in fonte */
    i32 mensura;      /* octeti contenti, sine terminatore */
    i32 terminator;   /* octeti terminatoris: ZEPHYRUM (finis plagulae),
                       * I ("\n" aut "\r"), II ("\r\n") */
    i32 numerus;      /* I-basatus */
} MdLinea;

nomen structura {
    constans character* fons;
                   i32  mensura;
                   Xar* lineae;    /* Xar de MdLinea (per valorem) */
} MdLineae;

/* Fons in lineas scindere. Plagula vacua = lineae nullae; terminator
 * ultimus lineam vacuam ULTERIOREM non parit (CommonMark). FALSUM =
 * memoria deficit. */
b32
md_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
              MdLineae* exitus);

i32
md_lineae_numerus (
    constans MdLineae* lineae);

/* NIHIL si index extra. */
constans MdLinea*
md_linea (
    constans MdLineae* lineae,
                  i32  index);

/* Visus in fontem, nulla copia (chorda.datum i8* est; unio). */
chorda
md_chorda_fontis (
    constans character* fons,
                   s32  ab,
                   s32  ad);


/* ==================================================
 * Scanners - super [ab, ad) contenti lineae; ultra ad NUMQUAM legunt
 * ================================================== */

/* Spatia et tabulae ab 'ab'. Reddit octetum primum post; *columnae =
 * latitudo in columnis, 'columna' = columna (ZEPHYRUM-basata) octeti
 * ab, ad tabulas expandendas. */
s32
md_scan_indentatio (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32  columna,
                   i32* columnae);

/* '>' + spatium/tabula optionalis. Reddit finem marcae; ab si nulla. */
s32
md_scan_citatio (
    constans character* fons,
                   s32  ab,
                   s32  ad);

nomen structura {
          s32 marca_ad;      /* post marcam ipsam ('-' | '12.') */
          s32 ad;            /* post spatia sequentia OMNIA */
          b32 numerata;
          i32 initium;       /* numerus (numerata), aliter ZEPHYRUM */
    character delimitator;   /* '-' '+' '*' | '.' ')' */
          i32 latitudo;      /* columnae marcae ipsius */
          i32 spatia;        /* columnae spatiorum post marcam;
                               * ZEPHYRUM = finis lineae statim */
} MdMarcaListae;

/* '-' '+' '*' aut I..IX digiti + '.' | ')', deinde spatium/tabula aut
 * finis lineae. Quot spatia marcae pertineant (I..IV; V+ = I et
 * cetera codex indentatus) PARSATOR decernit - hic numerantur. */
b32
md_scan_marca_listae (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32  columna,
         MdMarcaListae* m);

nomen structura {
    s32 ad;          /* post ']' */
    b32 perfectum;   /* 'x' | 'X' */
} MdMarcaOfficii;

/* '[ ]' '[x]' '[X]' + spatium/tabula aut finis lineae (GFM par. 5.3). */
b32
md_scan_officium (
    constans character* fons,
                   s32  ab,
                   s32  ad,
        MdMarcaOfficii* o);

nomen structura {
    i32 gradus;         /* I..VI */
    s32 marca_ad;       /* post '#'+ et spatia sequentia */
    s32 contentum_ad;   /* finis contenti (ante clausuram et spatia eius) */
    s32 clausura_ab;    /* initium clausurae (spatia ducentia inclusa);
                         * == ad si nulla */
} MdAtx;

/* I..VI '#', deinde spatium/tabula aut finis. Clausura = spatia +
 * '#'+ + spatia in fine, spatio praecedente aut contento vacuo. */
b32
md_scan_atx (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                 MdAtx* r);

/* '='+ aut '-'+ deinde spatia sola. *gradus = I ('=') | II ('-'). */
b32
md_scan_subductio (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32* gradus);

/* III+ ex uno '-' '*' '_', spatiis/tabulis interpositis licitis. */
b32
md_scan_divisio (
    constans character* fons,
                   s32  ab,
                   s32  ad);

nomen structura {
    character signum;      /* '`' | '~' */
          i32 longitudo;   /* >= III */
          s32 signum_ad;
          s32 info_ab;     /* == signum_ad; info cruda spatia servat */
          s32 info_ad;     /* == ad */
          b32 info_adest;  /* info post trim non vacua */
} MdSaeptum;

/* III+ '`' aut '~' + chorda info; info saepti '`' gravem continere
 * NON potest (CommonMark par. 4.5). */
b32
md_scan_saeptum_apertum (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             MdSaeptum* r);

/* Cursus signi >= longitudo_min, deinde spatia sola. */
b32
md_scan_saeptum_clausum (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character  signum,
                   i32  longitudo_min,
                   s32* signum_ad);

/* Ordo separator tabulae GFM: cellae ':?-+:?' pipis divisae, pipa
 * ducens/finalis optionalis. Reddit numerum cellarum (ZEPHYRUM = non
 * separator); ordinationes[i] = MdOrdinatio (md_registrum.h) usque ad
 * capacitatem. */
i32
md_scan_tabula_separator (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32* ordinationes,
                   i32  capacitas);
#define MD_CELLAE_MAXIMAE ((i32)64)

nomen structura {
    s32 pipa_ab;   /* lexema PIPA ante cellam: spatia + '|' + spatia;
                    * vacuum (== pipa_ad) pro cella prima sine pipa */
    s32 pipa_ad;
    s32 ab;        /* contentum, trim */
    s32 ad;
} MdCella;

nomen structura {
        i32 numerus;
    MdCella cellae[MD_CELLAE_MAXIMAE];
        s32 clausum_ab;   /* post cellam ultimam usque ad finem: pipa
                           * finalis + spatia (vacuum licet) */
    s32 clausum_ad;
} MdOrdo;

/* Ordo tabulae GFM (par. 4.10): cellae pipis NON effugitis ('\|' manet
 * in contento) divisae; segmentum primum/ultimum cella est solum si non
 * vacuum (pipa ducens/finalis optionalis), interiora semper. Omnis
 * octetus [ab, ad) cellae aut clausum pertinet. Reddit numerum cellarum
 * (ZEPHYRUM = linea vacua). Ultra capacitatem contentum cellae ultimae
 * crescit. */
i32
md_scan_ordo (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                MdOrdo* o);


/* Conditiones initii blocci html (CommonMark par. 4.6), super contentum
 * post indentationem <= III: I script/pre/style/textarea · II '<!--' ·
 * III '<?' · IV '<!' + littera · V '<![CDATA[' · VI tag blocci (LXII
 * nomina) · VII tag apertum/clausum integrum solum in linea. Reddit
 * I..VII; ZEPHYRUM = nulla. Conditio VII paragraphum interrumpere NON
 * potest (paragraphus_apertus). */
i32
md_scan_html_initium (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   b32  paragraphus_apertus);

/* An linea blocum html conditionis datae CLAUDAT: I..V chordam finalem
 * continent ('</script>' etc., '-->', '?>', '>', ']]>' - linea ipsa
 * blocci pars est); VI et VII linea VACUA finiunt, quam vocans iudicat
 * (hic FALSUM). */
b32
md_scan_html_finis (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32  conditio);


/* ==================================================
 * Fabrica lexematum
 * ================================================== */

nomen structura {
              Piscina* piscina;
    constans MdLineae* lineae;
    MateriaTokenForma  forma;
} MdFabrica;

vacuum
md_fabrica_incipere (
            MdFabrica* fabrica,
              Piscina* piscina,
    constans MdLineae* lineae);

/* Lexema super [ab, ad) lineae linea_index (index in tabulam,
 * ZEPHYRUM-basatus). Positio ex tabula derivatur. NIHIL = memoria. */
MateriaToken*
md_lexema_fabricare (
    MdFabrica* fabrica,
          s32  genus,
          i32  linea_index,
          s32  ab,
          s32  ad);

/* Terminator lineae ut lexema LINEA aut LINEA_CR; NIHIL si linea
 * terminatorem non habet (ultima sine "\n"). */
MateriaToken*
md_lexema_terminator (
    MdFabrica* fabrica,
          i32  linea_index);

/* FINIS post octetum ultimum. */
MateriaToken*
md_lexema_finis (
    MdFabrica* fabrica);


/* ==================================================
 * Lexatio cruda - porta A2
 * ================================================== */

/* [INDENTATIO] [TEXTUS] [LINEA|LINEA_CR] per lineam, FINIS ultimum.
 * Xar de MateriaToken*. Valores concatenati == fons. */
Xar*
md_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* MD_LEXEMA_H */
