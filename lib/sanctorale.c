/* ==================================================
 * SANCTORALE
 * Calendarium Sanctorum Universale (Forma Ordinaria)
 *
 * Secundum Calendarium Romanum Generale (1969)
 * Cum diebus obligationis pro Civitatibus Foederatis
 *
 * Nomina Anglice pro usu in calendario
 * ================================================== */

#include "latina.h"
#include "calendarium_liturgicum.h"


/* ==================================================
 * Tabulae Sanctorale
 *
 * Ordinatae per mensem et diem.
 * Plures celebrationes in eadem die possibiles.
 * ================================================== */

interior constans SanctoraleDatum SANCTORALE[] = {

    /* ============================================
     * IANUARIUS
     * ============================================ */

    /* 1 Ian - Sollemnitas */
    { FASTI_IANUARIUS, I,
      "Mary, Mother of God", "Mary, Mother of God",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, VERUM },

    /* 2 Ian - Memoria */
    { FASTI_IANUARIUS, II,
      "Saints Basil the Great and Gregory Nazianzen",
      "Ss. Basil & Gregory",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 3 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, III,
      "The Most Holy Name of Jesus", "Holy Name of Jesus",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 4 Ian - Memoria (US) */
    { FASTI_IANUARIUS, IV,
      "Saint Elizabeth Ann Seton", "St. Elizabeth Ann Seton",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 5 Ian - Memoria (US) */
    { FASTI_IANUARIUS, V,
      "Saint John Neumann", "St. John Neumann",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 6 Ian - Sollemnitas (si non transfertur ad Dominicam) */
    { FASTI_IANUARIUS, VI,
      "The Epiphany of the Lord", "Epiphany",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, FALSUM },

    /* 7 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, VII,
      "Saint Raymond of Penyafort", "St. Raymond",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 13 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, XIII,
      "Saint Hilary", "St. Hilary",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 17 Ian - Memoria */
    { FASTI_IANUARIUS, XVII,
      "Saint Anthony", "St. Anthony",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 20 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, XX,
      "Saint Fabian", "St. Fabian",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_IANUARIUS, XX,
      "Saint Sebastian", "St. Sebastian",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 21 Ian - Memoria */
    { FASTI_IANUARIUS, XXI,
      "Saint Agnes", "St. Agnes",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 22 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, XXII,
      "Saint Vincent", "St. Vincent",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 24 Ian - Memoria */
    { FASTI_IANUARIUS, XXIV,
      "Saint Francis de Sales", "St. Francis de Sales",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 25 Ian - Festum */
    { FASTI_IANUARIUS, XXV,
      "The Conversion of Saint Paul the Apostle", "Conv. of St. Paul",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 26 Ian - Memoria */
    { FASTI_IANUARIUS, XXVI,
      "Saints Timothy and Titus", "Ss. Timothy & Titus",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 27 Ian - Memoria ad libitum */
    { FASTI_IANUARIUS, XXVII,
      "Saint Angela Merici", "St. Angela Merici",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 28 Ian - Memoria */
    { FASTI_IANUARIUS, XXVIII,
      "Saint Thomas Aquinas", "St. Thomas Aquinas",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 31 Ian - Memoria */
    { FASTI_IANUARIUS, XXXI,
      "Saint John Bosco", "St. John Bosco",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * FEBRUARIUS
     * ============================================ */

    /* 2 Feb - Festum */
    { FASTI_FEBRUARIUS, II,
      "The Presentation of the Lord", "Presentation",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 3 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, III,
      "Saint Blaise", "St. Blaise",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_FEBRUARIUS, III,
      "Saint Ansgar", "St. Ansgar",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 5 Feb - Memoria */
    { FASTI_FEBRUARIUS, V,
      "Saint Agatha", "St. Agatha",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 6 Feb - Memoria */
    { FASTI_FEBRUARIUS, VI,
      "Saint Paul Miki and Companions", "St. Paul Miki",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 8 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, VIII,
      "Saint Jerome Emiliani", "St. Jerome Emiliani",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_FEBRUARIUS, VIII,
      "Saint Josephine Bakhita", "St. Josephine Bakhita",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 10 Feb - Memoria */
    { FASTI_FEBRUARIUS, X,
      "Saint Scholastica", "St. Scholastica",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 11 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, XI,
      "Our Lady of Lourdes", "Our Lady of Lourdes",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 14 Feb - Memoria */
    { FASTI_FEBRUARIUS, XIV,
      "Saints Cyril and Methodius", "Ss. Cyril & Methodius",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 17 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, XVII,
      "The Seven Holy Founders of the Servite Order",
      "Seven Servite Founders",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 21 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, XXI,
      "Saint Peter Damian", "St. Peter Damian",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 22 Feb - Festum */
    { FASTI_FEBRUARIUS, XXII,
      "The Chair of Saint Peter the Apostle", "Chair of St. Peter",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 23 Feb - Memoria ad libitum */
    { FASTI_FEBRUARIUS, XXIII,
      "Saint Polycarp", "St. Polycarp",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },


    /* ============================================
     * MARTIUS
     * ============================================ */

    /* 4 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, IV,
      "Saint Casimir", "St. Casimir",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 7 Mar - Memoria */
    { FASTI_MARTIUS, VII,
      "Saints Perpetua and Felicity", "Ss. Perpetua & Felicity",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 8 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, VIII,
      "Saint John of God", "St. John of God",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 9 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, IX,
      "Saint Frances of Rome", "St. Frances of Rome",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 17 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, XVII,
      "Saint Patrick", "St. Patrick",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 18 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, XVIII,
      "Saint Cyril of Jerusalem", "St. Cyril of Jerusalem",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 19 Mar - Sollemnitas */
    { FASTI_MARTIUS, XIX,
      "Saint Joseph, Spouse of the Blessed Virgin Mary",
      "St. Joseph",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, FALSUM },

    /* 23 Mar - Memoria ad libitum */
    { FASTI_MARTIUS, XXIII,
      "Saint Turibius of Mogrovejo", "St. Turibius",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 25 Mar - Sollemnitas */
    { FASTI_MARTIUS, XXV,
      "The Annunciation of the Lord", "Annunciation",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * APRILIS
     * ============================================ */

    /* 2 Apr - Memoria ad libitum */
    { FASTI_APRILIS, II,
      "Saint Francis of Paola", "St. Francis of Paola",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 4 Apr - Memoria ad libitum */
    { FASTI_APRILIS, IV,
      "Saint Isidore", "St. Isidore",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 5 Apr - Memoria ad libitum */
    { FASTI_APRILIS, V,
      "Saint Vincent Ferrer", "St. Vincent Ferrer",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 7 Apr - Memoria */
    { FASTI_APRILIS, VII,
      "Saint John Baptist de la Salle", "St. J.B. de la Salle",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 11 Apr - Memoria */
    { FASTI_APRILIS, XI,
      "Saint Stanislaus", "St. Stanislaus",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 13 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XIII,
      "Saint Martin I", "St. Martin I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 21 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XXI,
      "Saint Anselm", "St. Anselm",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 23 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XXIII,
      "Saint George", "St. George",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_APRILIS, XXIII,
      "Saint Adalbert", "St. Adalbert",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 24 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XXIV,
      "Saint Fidelis of Sigmaringen", "St. Fidelis",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 25 Apr - Festum */
    { FASTI_APRILIS, XXV,
      "Saint Mark the Evangelist", "St. Mark",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 28 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XXVIII,
      "Saint Peter Chanel", "St. Peter Chanel",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_APRILIS, XXVIII,
      "Saint Louis Grignion de Montfort", "St. Louis de Montfort",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 29 Apr - Memoria */
    { FASTI_APRILIS, XXIX,
      "Saint Catherine of Siena", "St. Catherine of Siena",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 30 Apr - Memoria ad libitum */
    { FASTI_APRILIS, XXX,
      "Saint Pius V", "St. Pius V",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * MAIUS
     * ============================================ */

    /* 1 May - Memoria ad libitum */
    { FASTI_MAIUS, I,
      "Saint Joseph the Worker", "St. Joseph Worker",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 2 May - Memoria */
    { FASTI_MAIUS, II,
      "Saint Athanasius", "St. Athanasius",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 3 May - Festum */
    { FASTI_MAIUS, III,
      "Saints Philip and James, Apostles", "Ss. Philip & James",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 10 May - Memoria ad libitum (US) */
    { FASTI_MAIUS, X,
      "Saint Damien de Veuster", "St. Damien",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 12 May - Memoria ad libitum */
    { FASTI_MAIUS, XII,
      "Saints Nereus and Achilleus", "Ss. Nereus & Achilleus",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_MAIUS, XII,
      "Saint Pancras", "St. Pancras",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 13 May - Memoria ad libitum */
    { FASTI_MAIUS, XIII,
      "Our Lady of Fatima", "Our Lady of Fatima",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 14 May - Festum */
    { FASTI_MAIUS, XIV,
      "Saint Matthias the Apostle", "St. Matthias",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 15 May - Memoria (US) */
    { FASTI_MAIUS, XV,
      "Saint Isidore the Farmer", "St. Isidore Farmer",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 18 May - Memoria ad libitum */
    { FASTI_MAIUS, XVIII,
      "Saint John I", "St. John I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 20 May - Memoria ad libitum */
    { FASTI_MAIUS, XX,
      "Saint Bernardine of Siena", "St. Bernardine",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 21 May - Memoria ad libitum */
    { FASTI_MAIUS, XXI,
      "Saint Christopher Magallanes and Companions",
      "St. Christopher Magallanes",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 22 May - Memoria ad libitum */
    { FASTI_MAIUS, XXII,
      "Saint Rita of Cascia", "St. Rita",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 25 May - Memoria ad libitum */
    { FASTI_MAIUS, XXV,
      "Saint Bede the Venerable", "St. Bede",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_MAIUS, XXV,
      "Saint Gregory VII", "St. Gregory VII",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_MAIUS, XXV,
      "Saint Mary Magdalene de' Pazzi", "St. Mary Magdalene de' Pazzi",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 26 May - Memoria */
    { FASTI_MAIUS, XXVI,
      "Saint Philip Neri", "St. Philip Neri",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 27 May - Memoria ad libitum */
    { FASTI_MAIUS, XXVII,
      "Saint Augustine of Canterbury", "St. Augustine of Canterbury",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 31 May - Festum */
    { FASTI_MAIUS, XXXI,
      "The Visitation of the Blessed Virgin Mary", "Visitation",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * IUNIUS
     * ============================================ */

    /* 1 Jun - Memoria */
    { FASTI_IUNIUS, I,
      "Saint Justin", "St. Justin",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 2 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, II,
      "Saints Marcellinus and Peter", "Ss. Marcellinus & Peter",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 3 Jun - Memoria */
    { FASTI_IUNIUS, III,
      "Saints Charles Lwanga and Companions", "St. Charles Lwanga",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 5 Jun - Memoria */
    { FASTI_IUNIUS, V,
      "Saint Boniface", "St. Boniface",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 6 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, VI,
      "Saint Norbert", "St. Norbert",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 9 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, IX,
      "Saint Ephrem", "St. Ephrem",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 11 Jun - Memoria */
    { FASTI_IUNIUS, XI,
      "Saint Barnabas the Apostle", "St. Barnabas",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 13 Jun - Memoria */
    { FASTI_IUNIUS, XIII,
      "Saint Anthony of Padua", "St. Anthony of Padua",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 19 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, XIX,
      "Saint Romuald", "St. Romuald",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 21 Jun - Memoria */
    { FASTI_IUNIUS, XXI,
      "Saint Aloysius Gonzaga", "St. Aloysius Gonzaga",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 22 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, XXII,
      "Saint Paulinus of Nola", "St. Paulinus of Nola",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_IUNIUS, XXII,
      "Saints John Fisher and Thomas More", "Ss. Fisher & More",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 24 Jun - Sollemnitas */
    { FASTI_IUNIUS, XXIV,
      "The Nativity of Saint John the Baptist", "Birth of St. John Baptist",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, FALSUM },

    /* 27 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, XXVII,
      "Saint Cyril of Alexandria", "St. Cyril of Alexandria",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 28 Jun - Memoria */
    { FASTI_IUNIUS, XXVIII,
      "Saint Irenaeus", "St. Irenaeus",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 29 Jun - Sollemnitas */
    { FASTI_IUNIUS, XXIX,
      "Saints Peter and Paul, Apostles", "Ss. Peter & Paul",
      GRADUS_SOLLEMNITAS, COLLIT_RUBER, FALSUM },

    /* 30 Jun - Memoria ad libitum */
    { FASTI_IUNIUS, XXX,
      "The First Martyrs of the Holy Roman Church", "First Martyrs of Rome",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },


    /* ============================================
     * IULIUS
     * ============================================ */

    /* 1 Jul - Memoria (US) */
    { FASTI_IULIUS, I,
      "Saint Junipero Serra", "St. Junipero Serra",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 3 Jul - Festum */
    { FASTI_IULIUS, III,
      "Saint Thomas the Apostle", "St. Thomas",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 4 Jul - Memoria ad libitum (US) */
    { FASTI_IULIUS, IV,
      "Independence Day", "Independence Day",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 5 Jul - Memoria ad libitum */
    { FASTI_IULIUS, V,
      "Saint Anthony Zaccaria", "St. Anthony Zaccaria",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_IULIUS, V,
      "Saint Elizabeth of Portugal", "St. Elizabeth of Portugal",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 6 Jul - Memoria ad libitum */
    { FASTI_IULIUS, VI,
      "Saint Maria Goretti", "St. Maria Goretti",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 9 Jul - Memoria ad libitum */
    { FASTI_IULIUS, IX,
      "Saint Augustine Zhao Rong and Companions", "St. Augustine Zhao Rong",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 11 Jul - Memoria */
    { FASTI_IULIUS, XI,
      "Saint Benedict", "St. Benedict",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 13 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XIII,
      "Saint Henry", "St. Henry",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 14 Jul - Memoria (US) */
    { FASTI_IULIUS, XIV,
      "Saint Kateri Tekakwitha", "St. Kateri Tekakwitha",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 15 Jul - Memoria */
    { FASTI_IULIUS, XV,
      "Saint Bonaventure", "St. Bonaventure",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 16 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XVI,
      "Our Lady of Mount Carmel", "Our Lady of Mt. Carmel",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 18 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XVIII,
      "Saint Camillus de Lellis", "St. Camillus de Lellis",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 20 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XX,
      "Saint Apollinaris", "St. Apollinaris",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 21 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XXI,
      "Saint Lawrence of Brindisi", "St. Lawrence of Brindisi",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 22 Jul - Festum */
    { FASTI_IULIUS, XXII,
      "Saint Mary Magdalene", "St. Mary Magdalene",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 23 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XXIII,
      "Saint Bridget", "St. Bridget",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 24 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XXIV,
      "Saint Sharbel Makhluf", "St. Sharbel",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 25 Jul - Festum */
    { FASTI_IULIUS, XXV,
      "Saint James the Apostle", "St. James",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 26 Jul - Memoria */
    { FASTI_IULIUS, XXVI,
      "Saints Joachim and Anne", "Ss. Joachim & Anne",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 29 Jul - Memoria */
    { FASTI_IULIUS, XXIX,
      "Saint Martha", "St. Martha",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 30 Jul - Memoria ad libitum */
    { FASTI_IULIUS, XXX,
      "Saint Peter Chrysologus", "St. Peter Chrysologus",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 31 Jul - Memoria */
    { FASTI_IULIUS, XXXI,
      "Saint Ignatius of Loyola", "St. Ignatius of Loyola",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * AUGUSTUS
     * ============================================ */

    /* 1 Aug - Memoria */
    { FASTI_AUGUSTUS, I,
      "Saint Alphonsus Liguori", "St. Alphonsus Liguori",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 2 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, II,
      "Saint Eusebius of Vercelli", "St. Eusebius of Vercelli",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_AUGUSTUS, II,
      "Saint Peter Julian Eymard", "St. Peter Julian Eymard",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 4 Aug - Memoria */
    { FASTI_AUGUSTUS, IV,
      "Saint John Vianney", "St. John Vianney",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 5 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, V,
      "The Dedication of the Basilica of Saint Mary Major",
      "Dedication of St. Mary Major",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 6 Aug - Festum */
    { FASTI_AUGUSTUS, VI,
      "The Transfiguration of the Lord", "Transfiguration",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 7 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, VII,
      "Saint Sixtus II and Companions", "St. Sixtus II",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_AUGUSTUS, VII,
      "Saint Cajetan", "St. Cajetan",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 8 Aug - Memoria */
    { FASTI_AUGUSTUS, VIII,
      "Saint Dominic", "St. Dominic",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 9 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, IX,
      "Saint Teresa Benedicta of the Cross", "St. Teresa Benedicta",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 10 Aug - Festum */
    { FASTI_AUGUSTUS, X,
      "Saint Lawrence", "St. Lawrence",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 11 Aug - Memoria */
    { FASTI_AUGUSTUS, XI,
      "Saint Clare", "St. Clare",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 12 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XII,
      "Saint Jane Frances de Chantal", "St. Jane Frances de Chantal",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 13 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XIII,
      "Saints Pontian and Hippolytus", "Ss. Pontian & Hippolytus",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 14 Aug - Memoria */
    { FASTI_AUGUSTUS, XIV,
      "Saint Maximilian Kolbe", "St. Maximilian Kolbe",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 15 Aug - Sollemnitas */
    { FASTI_AUGUSTUS, XV,
      "The Assumption of the Blessed Virgin Mary", "Assumption",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, VERUM },

    /* 16 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XVI,
      "Saint Stephen of Hungary", "St. Stephen of Hungary",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 19 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XIX,
      "Saint John Eudes", "St. John Eudes",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 20 Aug - Memoria */
    { FASTI_AUGUSTUS, XX,
      "Saint Bernard", "St. Bernard",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 21 Aug - Memoria */
    { FASTI_AUGUSTUS, XXI,
      "Saint Pius X", "St. Pius X",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 22 Aug - Memoria */
    { FASTI_AUGUSTUS, XXII,
      "The Queenship of the Blessed Virgin Mary", "Queenship of Mary",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 23 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XXIII,
      "Saint Rose of Lima", "St. Rose of Lima",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 24 Aug - Festum */
    { FASTI_AUGUSTUS, XXIV,
      "Saint Bartholomew the Apostle", "St. Bartholomew",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 25 Aug - Memoria ad libitum */
    { FASTI_AUGUSTUS, XXV,
      "Saint Louis", "St. Louis",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_AUGUSTUS, XXV,
      "Saint Joseph Calasanz", "St. Joseph Calasanz",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 27 Aug - Memoria */
    { FASTI_AUGUSTUS, XXVII,
      "Saint Monica", "St. Monica",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 28 Aug - Memoria */
    { FASTI_AUGUSTUS, XXVIII,
      "Saint Augustine", "St. Augustine",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 29 Aug - Memoria */
    { FASTI_AUGUSTUS, XXIX,
      "The Passion of Saint John the Baptist", "Beheading of St. John Baptist",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },


    /* ============================================
     * SEPTEMBER
     * ============================================ */

    /* 3 Sep - Memoria */
    { FASTI_SEPTEMBER, III,
      "Saint Gregory the Great", "St. Gregory the Great",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 8 Sep - Festum */
    { FASTI_SEPTEMBER, VIII,
      "The Nativity of the Blessed Virgin Mary", "Birth of Mary",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 9 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, IX,
      "Saint Peter Claver", "St. Peter Claver",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 12 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XII,
      "The Most Holy Name of Mary", "Holy Name of Mary",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 13 Sep - Memoria */
    { FASTI_SEPTEMBER, XIII,
      "Saint John Chrysostom", "St. John Chrysostom",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 14 Sep - Festum */
    { FASTI_SEPTEMBER, XIV,
      "The Exaltation of the Holy Cross", "Exaltation of Holy Cross",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 15 Sep - Memoria */
    { FASTI_SEPTEMBER, XV,
      "Our Lady of Sorrows", "Our Lady of Sorrows",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 16 Sep - Memoria */
    { FASTI_SEPTEMBER, XVI,
      "Saints Cornelius and Cyprian", "Ss. Cornelius & Cyprian",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 17 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XVII,
      "Saint Robert Bellarmine", "St. Robert Bellarmine",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 19 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XIX,
      "Saint Januarius", "St. Januarius",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 20 Sep - Memoria */
    { FASTI_SEPTEMBER, XX,
      "Saints Andrew Kim Tae-gon and Paul Chong Ha-sang and Companions",
      "Ss. Andrew Kim & Paul Chong",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 21 Sep - Festum */
    { FASTI_SEPTEMBER, XXI,
      "Saint Matthew, Apostle and Evangelist", "St. Matthew",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 23 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XXIII,
      "Saint Pius of Pietrelcina", "St. Padre Pio",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 26 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XXVI,
      "Saints Cosmas and Damian", "Ss. Cosmas & Damian",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 27 Sep - Memoria */
    { FASTI_SEPTEMBER, XXVII,
      "Saint Vincent de Paul", "St. Vincent de Paul",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 28 Sep - Memoria ad libitum */
    { FASTI_SEPTEMBER, XXVIII,
      "Saint Wenceslaus", "St. Wenceslaus",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_SEPTEMBER, XXVIII,
      "Saint Lawrence Ruiz and Companions", "St. Lawrence Ruiz",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 29 Sep - Festum */
    { FASTI_SEPTEMBER, XXIX,
      "Saints Michael, Gabriel, and Raphael, Archangels", "Ss. Michael, Gabriel, Raphael",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 30 Sep - Memoria */
    { FASTI_SEPTEMBER, XXX,
      "Saint Jerome", "St. Jerome",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },


    /* ============================================
     * OCTOBER
     * ============================================ */

    /* 1 Oct - Memoria */
    { FASTI_OCTOBER, I,
      "Saint Therese of the Child Jesus", "St. Therese of Lisieux",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 2 Oct - Memoria */
    { FASTI_OCTOBER, II,
      "The Holy Guardian Angels", "Guardian Angels",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 4 Oct - Memoria */
    { FASTI_OCTOBER, IV,
      "Saint Francis of Assisi", "St. Francis of Assisi",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 5 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, V,
      "Saint Faustina Kowalska", "St. Faustina",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 6 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, VI,
      "Saint Bruno", "St. Bruno",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_OCTOBER, VI,
      "Blessed Marie Rose Durocher", "Bl. Marie Rose Durocher",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 7 Oct - Memoria */
    { FASTI_OCTOBER, VII,
      "Our Lady of the Rosary", "Our Lady of the Rosary",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 9 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, IX,
      "Saint Denis and Companions", "St. Denis",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_OCTOBER, IX,
      "Saint John Leonardi", "St. John Leonardi",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 11 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XI,
      "Saint John XXIII", "St. John XXIII",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 14 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XIV,
      "Saint Callistus I", "St. Callistus I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 15 Oct - Memoria */
    { FASTI_OCTOBER, XV,
      "Saint Teresa of Jesus", "St. Teresa of Avila",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 16 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XVI,
      "Saint Hedwig", "St. Hedwig",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_OCTOBER, XVI,
      "Saint Margaret Mary Alacoque", "St. Margaret Mary Alacoque",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 17 Oct - Memoria */
    { FASTI_OCTOBER, XVII,
      "Saint Ignatius of Antioch", "St. Ignatius of Antioch",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 18 Oct - Festum */
    { FASTI_OCTOBER, XVIII,
      "Saint Luke the Evangelist", "St. Luke",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 19 Oct - Memoria (US) */
    { FASTI_OCTOBER, XIX,
      "Saints John de Brebeuf and Isaac Jogues and Companions",
      "Ss. John de Brebeuf & Isaac Jogues",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    { FASTI_OCTOBER, XIX,
      "Saint Paul of the Cross", "St. Paul of the Cross",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 22 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XXII,
      "Saint John Paul II", "St. John Paul II",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 23 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XXIII,
      "Saint John of Capistrano", "St. John of Capistrano",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 24 Oct - Memoria ad libitum */
    { FASTI_OCTOBER, XXIV,
      "Saint Anthony Mary Claret", "St. Anthony Mary Claret",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 28 Oct - Festum */
    { FASTI_OCTOBER, XXVIII,
      "Saints Simon and Jude, Apostles", "Ss. Simon & Jude",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },


    /* ============================================
     * NOVEMBER
     * ============================================ */

    /* 1 Nov - Sollemnitas */
    { FASTI_NOVEMBER, I,
      "All Saints", "All Saints",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, VERUM },

    /* 2 Nov - Commemoratio omnium fidelium defunctorum */
    { FASTI_NOVEMBER, II,
      "The Commemoration of All the Faithful Departed", "All Souls",
      GRADUS_DIES_PECULIARIS, COLLIT_VIOLACEUS, FALSUM },

    /* 3 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, III,
      "Saint Martin de Porres", "St. Martin de Porres",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 4 Nov - Memoria */
    { FASTI_NOVEMBER, IV,
      "Saint Charles Borromeo", "St. Charles Borromeo",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 9 Nov - Festum */
    { FASTI_NOVEMBER, IX,
      "The Dedication of the Lateran Basilica", "Lateran Basilica",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 10 Nov - Memoria */
    { FASTI_NOVEMBER, X,
      "Saint Leo the Great", "St. Leo the Great",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 11 Nov - Memoria */
    { FASTI_NOVEMBER, XI,
      "Saint Martin of Tours", "St. Martin of Tours",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 12 Nov - Memoria */
    { FASTI_NOVEMBER, XII,
      "Saint Josaphat", "St. Josaphat",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 13 Nov - Memoria (US) */
    { FASTI_NOVEMBER, XIII,
      "Saint Frances Xavier Cabrini", "St. Frances Cabrini",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 15 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, XV,
      "Saint Albert the Great", "St. Albert the Great",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 16 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, XVI,
      "Saint Margaret of Scotland", "St. Margaret of Scotland",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_NOVEMBER, XVI,
      "Saint Gertrude", "St. Gertrude",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 17 Nov - Memoria */
    { FASTI_NOVEMBER, XVII,
      "Saint Elizabeth of Hungary", "St. Elizabeth of Hungary",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 18 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, XVIII,
      "The Dedication of the Basilicas of Saints Peter and Paul",
      "Ss. Peter & Paul Basilicas",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_NOVEMBER, XVIII,
      "Saint Rose Philippine Duchesne", "St. Rose Philippine Duchesne",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 21 Nov - Memoria */
    { FASTI_NOVEMBER, XXI,
      "The Presentation of the Blessed Virgin Mary", "Presentation of Mary",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 22 Nov - Memoria */
    { FASTI_NOVEMBER, XXII,
      "Saint Cecilia", "St. Cecilia",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 23 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, XXIII,
      "Saint Clement I", "St. Clement I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    { FASTI_NOVEMBER, XXIII,
      "Saint Columban", "St. Columban",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    { FASTI_NOVEMBER, XXIII,
      "Blessed Miguel Agustin Pro", "Bl. Miguel Pro",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 24 Nov - Memoria */
    { FASTI_NOVEMBER, XXIV,
      "Saint Andrew Dung-Lac and Companions", "St. Andrew Dung-Lac",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 25 Nov - Memoria ad libitum */
    { FASTI_NOVEMBER, XXV,
      "Saint Catherine of Alexandria", "St. Catherine of Alexandria",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 30 Nov - Festum */
    { FASTI_NOVEMBER, XXX,
      "Saint Andrew the Apostle", "St. Andrew",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },


    /* ============================================
     * DECEMBER
     * ============================================ */

    /* 3 Dec - Memoria */
    { FASTI_DECEMBER, III,
      "Saint Francis Xavier", "St. Francis Xavier",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 4 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, IV,
      "Saint John Damascene", "St. John Damascene",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 6 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, VI,
      "Saint Nicholas", "St. Nicholas",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 7 Dec - Memoria */
    { FASTI_DECEMBER, VII,
      "Saint Ambrose", "St. Ambrose",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 8 Dec - Sollemnitas */
    { FASTI_DECEMBER, VIII,
      "The Immaculate Conception of the Blessed Virgin Mary",
      "Immaculate Conception",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, VERUM },

    /* 9 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, IX,
      "Saint Juan Diego", "St. Juan Diego",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 10 Dec - Memoria ad libitum (US) */
    { FASTI_DECEMBER, X,
      "Our Lady of Loreto", "Our Lady of Loreto",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 11 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, XI,
      "Saint Damasus I", "St. Damasus I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 12 Dec - Festum (US) */
    { FASTI_DECEMBER, XII,
      "Our Lady of Guadalupe", "Our Lady of Guadalupe",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 13 Dec - Memoria */
    { FASTI_DECEMBER, XIII,
      "Saint Lucy", "St. Lucy",
      GRADUS_MEMORIA, COLLIT_RUBER, FALSUM },

    /* 14 Dec - Memoria */
    { FASTI_DECEMBER, XIV,
      "Saint John of the Cross", "St. John of the Cross",
      GRADUS_MEMORIA, COLLIT_ALBUS, FALSUM },

    /* 21 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, XXI,
      "Saint Peter Canisius", "St. Peter Canisius",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 23 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, XXIII,
      "Saint John of Kanty", "St. John of Kanty",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

    /* 25 Dec - Sollemnitas */
    { FASTI_DECEMBER, XXV,
      "The Nativity of the Lord", "Christmas",
      GRADUS_SOLLEMNITAS, COLLIT_ALBUS, VERUM },

    /* 26 Dec - Festum */
    { FASTI_DECEMBER, XXVI,
      "Saint Stephen, First Martyr", "St. Stephen",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 27 Dec - Festum */
    { FASTI_DECEMBER, XXVII,
      "Saint John, Apostle and Evangelist", "St. John",
      GRADUS_FESTUM, COLLIT_ALBUS, FALSUM },

    /* 28 Dec - Festum */
    { FASTI_DECEMBER, XXVIII,
      "The Holy Innocents, Martyrs", "Holy Innocents",
      GRADUS_FESTUM, COLLIT_RUBER, FALSUM },

    /* 29 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, XXIX,
      "Saint Thomas Becket", "St. Thomas Becket",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_RUBER, FALSUM },

    /* 31 Dec - Memoria ad libitum */
    { FASTI_DECEMBER, XXXI,
      "Saint Sylvester I", "St. Sylvester I",
      GRADUS_MEMORIA_AD_LIBITUM, COLLIT_ALBUS, FALSUM },

};


/* Numerus totalis celebrationum in sanctorale */
interior constans s32 SANCTORALE_NUMERUS =
    (s32)(magnitudo(SANCTORALE) / magnitudo(SANCTORALE[0]));


/* ==================================================
 * Functiones Lookup
 * ================================================== */

/*
 * sanctorale_obtinere
 *
 * Obtinere celebrationes pro die dato.
 * Redit indicem primae celebrationis vel NIHIL.
 * Ponit numerum celebrationum in *numerus_celebrationum.
 */
constans SanctoraleDatum*
sanctorale_obtinere(
    s32  mensis,
    s32  dies,
    s32* numerus_celebrationum)
{
    s32 i;
    s32 primus = -I;
    s32 numerus = ZEPHYRUM;

    /* Quaerere per tabulam (ordinata est) */
    per (i = ZEPHYRUM; i < SANCTORALE_NUMERUS; i++) {
        si (SANCTORALE[i].mensis == mensis && SANCTORALE[i].dies == dies) {
            si (primus < ZEPHYRUM) {
                primus = i;
            }
            numerus++;
        } alioquin si (SANCTORALE[i].mensis > mensis ||
                      (SANCTORALE[i].mensis == mensis && SANCTORALE[i].dies > dies)) {
            /* Praeter diem quaesitam */
            frange;
        }
    }

    si (numerus_celebrationum != NIHIL) {
        *numerus_celebrationum = numerus;
    }

    si (primus >= ZEPHYRUM) {
        redde &SANCTORALE[primus];
    }

    redde NIHIL;
}


/*
 * sanctorale_numerus
 *
 * Redit numerum totalem celebrationum in sanctorale.
 */
s32
sanctorale_numerus(vacuum)
{
    redde SANCTORALE_NUMERUS;
}
