#ifndef CALENDARIUM_LITURGICUM_H
#define CALENDARIUM_LITURGICUM_H

/* <aedilis corpus="lib/sanctorale.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"


/* ==================================================
 * CALENDARIUM LITURGICUM
 * Bibliotheca Calendarii Liturgici Romani (Forma Ordinaria)
 *
 * Secundum Calendarium Romanum Generale (1969)
 * Novus Ordo Missae Pauli VI
 *
 * Requiret: fasti.h (pro Dies, fasti_computus)
 * ================================================== */


/* ==================================================
 * Constantae - Tempora Liturgica
 *
 * Sex tempora anni liturgici.
 * Annus liturgicus incipit Dominica I Adventus.
 * ================================================== */

nomen enumeratio {
    TEMPUS_ADVENTUS        = ZEPHYRUM,  /* Dominica I Adventus - 24 Dec */
    TEMPUS_NATIVITATIS     = I,         /* 25 Dec - Baptisma Domini */
    TEMPUS_PER_ANNUM_I     = II,        /* Post Baptisma - ante Quadragesimam */
    TEMPUS_QUADRAGESIMAE   = III,       /* Feria IV Cinerum - Feria V Sancta */
    TEMPUS_TRIDUUM_SACRUM  = IV,        /* Feria V vespere - Dominica Paschae */
    TEMPUS_PASCHALE        = V,         /* Dominica Paschae - Pentecoste */
    TEMPUS_PER_ANNUM_II    = VI         /* Post Pentecosten - ante Adventum */
} TempusLiturgicum;


/* ==================================================
 * Constantae - Gradus Celebrationis
 *
 * GENUS celebrationis ad ostendendum (sollemnitas, festum, memoria,
 * dominica, feria). Praecedentiam NON dat - vide OrdoTabulae
 * (liturgia-plan L1). Valores priores servati.
 * ================================================== */

nomen enumeratio {
    GRADUS_FERIA               = ZEPHYRUM,  /* Dies ferialis simplex */
    GRADUS_MEMORIA_AD_LIBITUM  = I,         /* Memoria optionalis */
    GRADUS_MEMORIA             = II,        /* Memoria obligatoria */
    GRADUS_FESTUM              = III,       /* Festum */
    GRADUS_SOLLEMNITAS         = IV,        /* Sollemnitas */
    GRADUS_DOMINICA            = V,         /* Dominica */
    GRADUS_DIES_PECULIARIS     = VI,        /* Cineres, Hebdomada Sancta,
                                             * Triduum, Octava Paschae */
    GRADUS_DEFUNCTORUM         = VII        /* Commemoratio omnium
                                             * fidelium defunctorum */
} GradusCelebrationis;


/* ==================================================
 * Constantae - Ordo Tabulae Dierum Liturgicorum
 *
 * Normae Universales de Anno Liturgico 59: tabula dierum secundum
 * praecedentiam. MINOR numerus = MAIOR praecedentia. Celebratio
 * principalis diei = ordo minimus inter celebrationes nec ad libitum
 * nec commemoratas; ordine aequali temporale vincit.
 * ================================================== */

nomen enumeratio {
    ORDO_TRIDUUM                = I,     /* Triduum Paschale */
    ORDO_PRIVILEGIATUS          = II,    /* Nativitas, Epiphania, Ascensio,
                                          * Pentecoste; Dominicae Adventus,
                                          * Quadragesimae, Paschae; Cineres;
                                          * Hebdomada Sancta II-V; Octava
                                          * Paschae */
    ORDO_SOLLEMNITAS_GENERALIS  = III,   /* + Commemoratio Defunctorum */
    ORDO_SOLLEMNITAS_PROPRIA    = IV,
    ORDO_FESTUM_DOMINI          = V,
    ORDO_DOMINICA_COMMUNIS      = VI,    /* Dominicae Nativitatis et per
                                          * annum */
    ORDO_FESTUM_GENERALE        = VII,
    ORDO_FESTUM_PROPRIUM        = VIII,
    ORDO_FERIA_PRIVILEGIATA     = IX,    /* 17-24 XII, octava Nativitatis,
                                          * feriae Quadragesimae */
    ORDO_MEMORIA_GENERALIS      = X,
    ORDO_MEMORIA_PROPRIA        = XI,
    ORDO_MEMORIA_AD_LIBITUM     = XII,
    ORDO_FERIA                  = XIII
} OrdoTabulae;


/* ==================================================
 * Constantae - Colores Liturgici
 * ================================================== */

nomen enumeratio {
    COLLIT_ALBUS     = ZEPHYRUM,  /* Nativitas, Pascha, BVM, Confessores */
    COLLIT_RUBER     = I,         /* Passio, Martyres, Pentecoste */
    COLLIT_VIRIDIS   = II,        /* Tempus per Annum */
    COLLIT_VIOLACEUS = III,       /* Adventus, Quadragesima */
    COLLIT_ROSACEUS  = IV,        /* Gaudete, Laetare */
    COLLIT_NIGER     = V          /* Defunctorum (optionalis) */
} ColorLiturgicus;


/* ==================================================
 * Constantae - Cyclus Lectionum
 * ================================================== */

nomen enumeratio {
    CYCLUS_A = ZEPHYRUM,  /* Matthaeus */
    CYCLUS_B = I,         /* Marcus */
    CYCLUS_C = II         /* Lucas */
} CyclusDominicalis;

nomen enumeratio {
    CYCLUS_I  = ZEPHYRUM,  /* Annus impar */
    CYCLUS_II = I          /* Annus par */
} CyclusQuotidianus;


/* ==================================================
 * Constantae - Genus Celebrationis
 * ================================================== */

nomen enumeratio {
    GENUS_TEMPORALE  = ZEPHYRUM,  /* Ex tempore (Dominicae, feriae) */
    GENUS_SANCTORALE = I,         /* Ex calendario sanctorum */
    GENUS_MOBILE     = II         /* Festa mobilia (ex anchoris: Pascha,
                                   * Adventus, Nativitas, Epiphania) */
} GenusCelebrationis;


/* ==================================================
 * Typi Structurarum
 * ================================================== */

/* Celebratio - Una celebratio liturgica */
nomen structura {
                 chorda titulus;        /* Titulus plenus */
                 chorda titulus_brevis; /* Titulus abbreviatus */
    GradusCelebrationis gradus;         /* genus ad ostendendum */
            OrdoTabulae ordo;           /* praecedentia */
        ColorLiturgicus color;
     GenusCelebrationis genus;
                    b32 ad_libitum;     /* hodie ad libitum (etiam memoriae
                                         * obligatoriae duae eodem die) -
                                         * numquam principalis */
                    b32 commemoratio;   /* memoria in feria privilegiata:
                                         * collecta sola - numquam
                                         * principalis, colorem non dat */
                    b32 translata;      /* sollemnitas impedita in hunc
                                         * diem translata */
                   Dies dies_originis;  /* dies propria (translata: dies
                                         * impedita; aliter dies ipsa) */
                    s32 lectionarium;   /* Numerus in Lectionario (-1 si
                                         * ignotus) */
} Celebratio;


/* InformatioCycli - Cyclus lectionum */
nomen structura {
    CyclusDominicalis cyclus_dominicalis;  /* A, B, C */
    CyclusQuotidianus cyclus_quotidianus;  /* I, II */
                  s32 hebdomada_psalterii; /* 1-4 */
} InformatioCycli;


/* InformatioTemporis - Positio in tempore liturgico */
nomen structura {
    TempusLiturgicum tempus;
                 s32 hebdomada;          /* Numerus hebdomadae (1+) */
                 s32 dies_in_hebdomada;  /* 0=Dominica, 6=Sabbatum */
                 b32 est_dominica;
} InformatioTemporis;


/* InformatioDiei - Informationes completae pro una die
 *
 * celebrationes = celebratio DIEI primum (temporalis, aut mobilis
 * quae eam substituit - Pascha, Palmae, Pentecoste, Trinitas ...),
 * deinde sanctorales et memoriae mobiles quae celebrari possunt
 * (victae omittuntur; memoriae in feria privilegiata cum
 * commemoratio; memoriae obligatoriae duae eodem die ambae ad
 * libitum). celebratio_principalis = ordo
 * minimus inter non ad libitum nec commemoratas (numquam NIHIL);
 * color_diei = color principalis. */
nomen structura {
                  Dies  dies;
    InformatioTemporis  tempus_info;
            Celebratio* celebrationes;
                   s32  numerus_celebrationum;
            Celebratio* celebratio_principalis;
       ColorLiturgicus  color_diei;
       InformatioCycli  cyclus;
                chorda  titulus_diei;
} InformatioDiei;


/* AnchoresMobiles - Dies movebiles pro anno CIVILI (calendarium USA:
 * Epiphania dominica 2-8 I, Ascensio et Corpus Christi dominica) */
nomen structura {
    s32 annus;

    /* Pascha et derivatae */
    Dies pascha;
    Dies feria_iv_cinerum;
    Dies dominica_i_quadragesimae;
    Dies dominica_palmarum;
    Dies feria_v_in_cena_domini;
    Dies feria_vi_in_passione;
    Dies sabbatum_sanctum;
    Dies ascensio;               /* USA: Dominica VII Paschae
                                  * (Pascha + XLII) */
    Dies pentecoste;
    Dies maria_mater_ecclesiae;  /* feria II post Pentecosten */
    Dies ss_trinitas;
    Dies corpus_christi;         /* USA: dominica post Trinitatem
                                  * (Pascha + LXIII) */
    Dies cor_iesu;
    Dies cor_immaculatum;        /* sabbatum post Cor Iesu */

    /* Adventus et Nativitas */
    Dies dominica_i_adventus;
    Dies christus_rex;
    Dies epiphania;              /* USA: dominica 2-8 I */
    Dies baptisma_domini;        /* dominica post Epiphaniam; feria II
                                  * si Epiphania 7 aut 8 I */
    Dies sancta_familia;         /* dominica intra octavam Nativitatis,
                                  * aut 30 XII si nulla */

    /* Proprium USA */
    Dies gratiarum_actio;        /* Thanksgiving: feria V quarta
                                  * Novembris */
} AnchoresMobiles;


/* CalendariumLiturgicum - Contexus principalis (opacus) */
nomen structura CalendariumLiturgicum CalendariumLiturgicum;


/* ==================================================
 * Creatio et Destructio
 * ================================================== */

CalendariumLiturgicum*
calendarium_creare (
    Piscina* piscina);

vacuum
calendarium_destruere (
    CalendariumLiturgicum* cal);


/* ==================================================
 * Quaestio Principalis
 * ================================================== */

/* Annus validus calendarii (Forma Ordinaria ab Adventu 1969):
 * extra - aut die invalida, aut cal NIHIL - obtinere_diem NIHIL
 * reddit */
#define CALENDARIUM_ANNUS_PRIMUS    MCMLXX
#define CALENDARIUM_ANNUS_ULTIMUS   (IX * M + CMXCIX)

/* Obtinere informationes completas pro die (NIHIL extra dominium) */
InformatioDiei*
calendarium_obtinere_diem (
    CalendariumLiturgicum* cal,
                     Dies  dies,
                  Piscina* piscina);


/* ==================================================
 * Quaestiones Temporis
 * ================================================== */

TempusLiturgicum
calendarium_tempus (
    CalendariumLiturgicum* cal,
                     Dies  dies);

InformatioTemporis
calendarium_tempus_info (
    CalendariumLiturgicum* cal,
                     Dies  dies);

ColorLiturgicus
calendarium_color_temporis (
    CalendariumLiturgicum* cal,
                     Dies  dies);


/* ==================================================
 * Limites Temporum
 * ================================================== */

AnchoresMobiles*
calendarium_anchorae (
    CalendariumLiturgicum* cal,
                      s32  annus);

Dies
calendarium_initium_temporis (
    CalendariumLiturgicum* cal,
         TempusLiturgicum  tempus,
                      s32  annus);

Dies
calendarium_finis_temporis (
    CalendariumLiturgicum* cal,
         TempusLiturgicum  tempus,
                      s32  annus);


/* ==================================================
 * Quaestiones Cycli Lectionum
 * ================================================== */

/* annus: annus LITURGICUS, id est annus civilis in quo finit
 * (Dominica I Adventus 2025 annum 2026 incipit). Pro die
 * calendarium_obtinere_diem eum computat. */
CyclusDominicalis
calendarium_cyclus_dominicalis (
    s32 annus);

CyclusQuotidianus
calendarium_cyclus_quotidianus (
    s32 annus);

s32
calendarium_hebdomada_psalterii (
    CalendariumLiturgicum* cal,
                     Dies  dies);


/* ==================================================
 * Dies Peculiares
 * ================================================== */

b32
calendarium_est_feria_cinerum (
    CalendariumLiturgicum* cal,
                     Dies  dies);

b32
calendarium_est_hebdomada_sancta (
    CalendariumLiturgicum* cal,
                     Dies  dies);

b32
calendarium_est_triduum (
    CalendariumLiturgicum* cal,
                     Dies  dies);

b32
calendarium_est_octava_paschae (
    CalendariumLiturgicum* cal,
                     Dies  dies);

b32
calendarium_est_octava_nativitatis (
    CalendariumLiturgicum* cal,
                     Dies  dies);


/* ==================================================
 * Invenire
 * ================================================== */

Dies
calendarium_proxima_dominica (
    Dies dies);

/* titulus (plenus aut brevis, exactus) celebrationis mobilis aut
 * sanctoralis: dies eius anno dato (translata: dies nova); non
 * inventa: dies 0-0-0 */
Dies
calendarium_invenire_festum (
    CalendariumLiturgicum* cal,
                   chorda  titulus,
                      s32  annus);

/* Dies festus de praecepto USA (dominicis exceptis): Nativitas;
 * Immaculata (non translata in 9 XII); 1 I, 15 VIII, 1 XI nisi
 * sabbato aut feria II (norma complementaria ad can. 1246 §2).
 * Ascensio in USA dominica est. */
b32
calendarium_dies_obligationis (
    CalendariumLiturgicum* cal,
                     Dies  dies);


/* ==================================================
 * Formatatio
 * ================================================== */

chorda
calendarium_nomen_temporis (
    TempusLiturgicum  tempus,
                 b32  abbreviatum,
             Piscina* piscina);

chorda
calendarium_nomen_gradus (
    GradusCelebrationis  gradus,
                    b32  abbreviatum,
                Piscina* piscina);

chorda
calendarium_nomen_coloris (
    ColorLiturgicus  color,
            Piscina* piscina);

chorda
calendarium_formare_titulum (
    CalendariumLiturgicum* cal,
                     Dies  dies,
                  Piscina* piscina);

/* nomina celebrationum diei ex calendarium_obtinere_diem (F14):
 * celebrationes non temporales (principalis, ad libitum,
 * commemoratae) per ", "; vacua si nulla */
chorda
calendarium_nomen_celebrationum (
    CalendariumLiturgicum* cal,
                     Dies  dies,
                  Piscina* piscina);


/* ==================================================
 * Sanctorale - Data Structure
 * ================================================== */

/* Sanctorale vigens ad: calendarium USA (USCCB 2026) et decretum
 * ultimum inscriptum - S. Ioannes Henricus Newman (9 X; Dicasterium
 * de Cultu Divino, 9 XI 2025). Fons et excerpta:
 * probationes/fixa/liturgia/usccb_sanctorale.tsv (liturgia-plan L4) */
#define CALENDARIUM_SANCTORALE_VIGENS "2026-02"

/* SanctoraleDatum - Entry in the sanctorale table */
nomen structura {
                    s32  mensis;
                    s32  dies;
     constans character* titulus;         /* English name */
     constans character* titulus_brevis;  /* Abbreviated name */
    GradusCelebrationis  gradus;
            OrdoTabulae  ordo;              /* praecedentia (UNLY 59) */
        ColorLiturgicus  color;
                    b32  dies_obligationis;  /* US Holy Day of Obligation */
} SanctoraleDatum;


/* ==================================================
 * Sanctorale - Lookup Functions
 * ================================================== */

/* Get sanctorale entries for a given date */
constans SanctoraleDatum*
sanctorale_obtinere (
    s32  mensis,
    s32  dies,
    s32* numerus_celebrationum);

/* Get number of entries in sanctorale */
s32
sanctorale_numerus (vacuum);


/* ==================================================
 * Constantae Sanctorale - Dies Fixi Principales
 * ================================================== */

/* Sollemnitates */
#define CAL_MARIA_MATER_DEI_M        FASTI_IANUARIUS
#define CAL_MARIA_MATER_DEI_D        I

#define CAL_EPIPHANIA_M              FASTI_IANUARIUS
#define CAL_EPIPHANIA_D              VI

#define CAL_IOSEPH_M                 FASTI_MARTIUS
#define CAL_IOSEPH_D                 XIX

#define CAL_ANNUNTIATIO_M            FASTI_MARTIUS
#define CAL_ANNUNTIATIO_D            XXV

#define CAL_NATIVITAS_IOANNIS_M      FASTI_IUNIUS
#define CAL_NATIVITAS_IOANNIS_D      XXIV

#define CAL_PETRUS_PAULUS_M          FASTI_IUNIUS
#define CAL_PETRUS_PAULUS_D          XXIX

#define CAL_ASSUMPTIO_M              FASTI_AUGUSTUS
#define CAL_ASSUMPTIO_D              XV

#define CAL_OMNES_SANCTI_M           FASTI_NOVEMBER
#define CAL_OMNES_SANCTI_D           I

#define CAL_IMMACULATA_M             FASTI_DECEMBER
#define CAL_IMMACULATA_D             VIII

#define CAL_NATIVITAS_M              FASTI_DECEMBER
#define CAL_NATIVITAS_D              XXV


#endif /* CALENDARIUM_LITURGICUM_H */
