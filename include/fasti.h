#ifndef FASTI_H
#define FASTI_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"

/* ==================================================
 * FASTI - Bibliotheca Calendarii et Temporis
 *
 * Fasti erat calendarium Romanum antiquum.
 * Haec bibliotheca tractat:
 * - Dies (annus, mensis, dies)
 * - Horae (hora, minutum, secundum)
 * - Intervalla inter dies
 * - Computationes calendariales
 * - Computus Paschalis
 * ================================================== */


/* ==================================================
 * Constantae - Dies Hebdomadis
 *
 * Dominica = 0, more Romano et liturgico.
 * Hebdomada incipit die Domini.
 * ================================================== */

#define FASTI_DOMINICA      ZEPHYRUM
#define FASTI_FERIA_II      I
#define FASTI_FERIA_III     II
#define FASTI_FERIA_IV      III
#define FASTI_FERIA_V       IV
#define FASTI_FERIA_VI      V
#define FASTI_SABBATUM      VI


/* ==================================================
 * Constantae - Menses
 *
 * Menses sunt 1-indicati (Ianuarius = 1).
 * ================================================== */

#define FASTI_IANUARIUS     I
#define FASTI_FEBRUARIUS    II
#define FASTI_MARTIUS       III
#define FASTI_APRILIS       IV
#define FASTI_MAIUS         V
#define FASTI_IUNIUS        VI
#define FASTI_IULIUS        VII
#define FASTI_AUGUSTUS      VIII
#define FASTI_SEPTEMBER     IX
#define FASTI_OCTOBER       X
#define FASTI_NOVEMBER      XI
#define FASTI_DECEMBER      XII


/* ==================================================
 * Constantae - Linguae
 * ================================================== */

#define FASTI_LINGUA_LATINA     ZEPHYRUM
#define FASTI_LINGUA_ANGLICA    I


/* ==================================================
 * Constantae - Formae
 * ================================================== */

#define FASTI_FORMA_ISO             I       /* 2024-03-15 */
#define FASTI_FORMA_EUROPAEA        II      /* 15/03/2024 */
#define FASTI_FORMA_AMERICANA       III     /* 03/15/2024 */
#define FASTI_FORMA_LATINA_LONGA    IV      /* XV Martii MMXXIV */
#define FASTI_FORMA_LATINA_BREVIS   V       /* 15 Mar 2024 */
#define FASTI_FORMA_LITURGICA       VI      /* Dominica, 15 Martii 2024 */


/* ==================================================
 * Typi Fundamentales
 * ================================================== */

/* Dies - Data calendarii Gregoriani
 *
 * annus: Annus (negativus = ante Christum)
 * mensis: Mensis (1-12)
 * dies: Dies mensis (1-31)
 */
nomen structura {
    s32 annus;
    s32 mensis;
    s32 dies;
} Dies;

/* Hora - Tempus intra diem
 *
 * hora: Hora (0-23)
 * minutum: Minutum (0-59)
 * secundum: Secundum (0-59)
 */
nomen structura {
    s32 hora;
    s32 minutum;
    s32 secundum;
} Hora;

/* DiesHora - Dies et hora coniuncta */
nomen structura {
    Dies dies;
    Hora hora;
} DiesHora;

/* Intervallum - Distantia inter duas dies */
nomen structura {
    s64 dies_totales;
} Intervallum;


/* ==================================================
 * Constructores
 * ================================================== */

/* Creare diem ex componentibus (non validat) */
Dies
fasti_dies(
    s32 annus,
    s32 mensis,
    s32 dies);

/* Creare horam ex componentibus (non validat) */
Hora
fasti_hora(
    s32 hora,
    s32 minutum,
    s32 secundum);

/* Obtinere diem hodiernam ex systemate */
Dies
fasti_dies_hodie(
    vacuum);

/* Obtinere diem et horam nunc ex systemate */
DiesHora
fasti_nunc(
    vacuum);


/* ==================================================
 * Validatio
 * ================================================== */

/* Verificare si dies valida */
b32
fasti_dies_valida(
    Dies dies);

/* Verificare si hora valida */
b32
fasti_hora_valida(
    Hora hora);


/* ==================================================
 * Quaestiones Calendarii
 * ================================================== */

/* Est annus bissextilis? (leap year) */
b32
fasti_est_bissextilis(
    s32 annus);

/* Numerus dierum in mense (28-31) */
s32
fasti_dies_in_mense(
    s32 mensis,
    s32 annus);

/* Dies hebdomadis (0=Dominica, 6=Sabbatum) */
s32
fasti_dies_hebdomadis(
    Dies dies);

/* Dies anni (1-366) */
s32
fasti_dies_anni(
    Dies dies);

/* Numerus hebdomadis ISO (1-53) */
s32
fasti_hebdomada_anni(
    Dies dies);

/* Prima dies mensis */
Dies
fasti_prima_mensis(
    s32 mensis,
    s32 annus);

/* Ultima dies mensis */
Dies
fasti_ultima_mensis(
    s32 mensis,
    s32 annus);

/* Dies hebdomadis primae diei mensis (pro layout calendarii) */
s32
fasti_primus_dies_hebdomadis_mensis(
    s32 mensis,
    s32 annus);


/* ==================================================
 * Arithmetica Dierum
 * ================================================== */

/* Addere dies (negativus = subtrahere) */
Dies
fasti_addere_dies(
    Dies dies,
    s32  numerus);

/* Addere menses (cohibet si dies invalidus) */
Dies
fasti_addere_menses(
    Dies dies,
    s32  numerus);

/* Addere annos (cohibet si dies invalidus) */
Dies
fasti_addere_annos(
    Dies dies,
    s32  numerus);

/* Differentia inter duas dies */
Intervallum
fasti_differentia(
    Dies prima,
    Dies secunda);


/* ==================================================
 * Comparatio
 * ================================================== */

/* Comparare: -1 si prima < secunda, 0 si =, +1 si > */
s32
fasti_comparare(
    Dies prima,
    Dies secunda);

/* Est prima ante secundam? */
b32
fasti_ante(
    Dies prima,
    Dies secunda);

/* Est prima post secundam? */
b32
fasti_post(
    Dies prima,
    Dies secunda);

/* Sunt dies aequales? */
b32
fasti_aequalis(
    Dies prima,
    Dies secunda);

/* Est dies inter duas dies (inclusive)? */
b32
fasti_inter(
    Dies dies,
    Dies ab,
    Dies ad);


/* ==================================================
 * Conversio Iuliana
 *
 * Numeri Iuliani sunt utiles pro computationibus.
 * ================================================== */

/* Convertere diem ad numerum Iulianum */
s64
fasti_ad_iulianum(
    Dies dies);

/* Convertere numerum Iulianum ad diem */
Dies
fasti_ex_iuliano(
    s64 iulianus);


/* ==================================================
 * Conversio Unix
 * ================================================== */

/* Convertere DiesHora ad Unix timestamp (UTC) */
s64
fasti_ad_unix(
    DiesHora dh);

/* Convertere Unix timestamp ad DiesHora (UTC) */
DiesHora
fasti_ex_unix(
    s64 timestamp);


/* ==================================================
 * Formatatio (requiret Piscina)
 * ================================================== */

/* Nomen mensis */
chorda
fasti_nomen_mensis(
    s32      mensis,
    b32      abbreviatum,
    s32      lingua,
    Piscina* piscina);

/* Nomen diei hebdomadis */
chorda
fasti_nomen_diei_hebdomadis(
    s32      dies_hebdomadis,
    b32      abbreviatum,
    s32      lingua,
    Piscina* piscina);

/* Formare diem */
chorda
fasti_formare_diem(
    Dies      dies,
    s32       forma,
    Piscina*  piscina);

/* Formare horam */
chorda
fasti_formare_horam(
    Hora     hora,
    b32      cum_secundis,
    b32      xxiv_horae,
    Piscina* piscina);

/* Scribere diem ad aedificatorem */
vacuum
fasti_scribere_diem(
    ChordaAedificator* aedificator,
    Dies               dies,
    s32                forma);

/* Scribere horam ad aedificatorem */
vacuum
fasti_scribere_horam(
    ChordaAedificator* aedificator,
    Hora               hora,
    b32                cum_secundis,
    b32                xxiv_horae);


/* ==================================================
 * Parsing
 * ================================================== */

/* Parsare diem ISO (YYYY-MM-DD) */
b32
fasti_parsare_iso(
    chorda  s,
    Dies*   fructus);

/* Parsare diem (tentat varios formatos) */
b32
fasti_parsare_diem(
    chorda  s,
    Dies*   fructus);


/* ==================================================
 * Computus Paschalis
 *
 * Computus est algorithmus pro determinando
 * diem Paschatis. Basis calendarii liturgici.
 * ================================================== */

/* Computare diem Paschatis (Dominica Resurrectionis) */
Dies
fasti_computus(
    s32 annus);


#endif /* FASTI_H */
