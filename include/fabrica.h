#ifndef FABRICA_H
#define FABRICA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "sigillum.h"
#include "internamentum.h"
#include "tabula_dispersa.h"


/* ==================================================
 * FABRICA - stratum aedificationis domus (iudex)
 *
 * Artificium (plagula producta: in build/, commissa, aut
 * installata) ex INGRESSIBUS per ACTIONEM fit. Iudex dicit quod
 * artificium ex ingressibus hodiernis NON factum sit, cur, et ordinem
 * sanationis - nihil aedificat. Vetus = sigilla ingressuum hodierna
 * != sigilla nota (relata aut regeneratione comparata). Numquam
 * mtime, numquam "exstat".
 *
 * Machina PURA: discum et processus per suturam solam (FabricaSutura)
 * tangit - probatio super tabulam in memoria currit, bin/fabrica
 * suturam veram praebet. NIHIL hic sqlite aut symbola extra lib/
 * nectit: quattuor installatores obiecta build/ CAECE nectunt.
 *
 * Spec: project-specs/fabrica-spec-v2.md; planum fabrica-plan-1a.md
 * (T1); parcum 01KZYN4VPZ.
 * ================================================== */

nomen enumeratio {
    FABRICA_INGRESSUS_FASCICULUS = ZEPHYRUM, /* octeti plagulae */
    FABRICA_INGRESSUS_MANIFESTUM,    /* viae manifesti aedilis */
    FABRICA_INGRESSUS_CONFIGURATIO,  /* aedilis.stml tota */
    FABRICA_INGRESSUS_INSTRUMENTUM,  /* binarium instrumenti */
    FABRICA_INGRESSUS_DIRECTORIUM,   /* nomina ordinata */
    FABRICA_INGRESSUS_PLAGULAE,      /* plagulae directorii, gradu 0,
                                      * suffixis filtratae (corpus
                                      * infixum briar/silicis) */
    FABRICA_INGRESSUS_MANIFESTA,     /* directorium manifestorum
                                      * (.stml, gradu 0): quodque
                                      * explicatur, nomina quoque
                                      * sigillantur (T6 fragmenta) */
    FABRICA_INGRESSUS_RADICES        /* configuratio aedilis: nomina
                                      * directoriorum inclusorum
                                      * OMNIUM quae nominat (caput
                                      * novum resolutionem mutat) */
} FabricaGenusIngressus;

nomen enumeratio {
    FABRICA_ACTIO_GENERATOR = ZEPHYRUM,
    FABRICA_ACTIO_FORMATIO,
    FABRICA_ACTIO_INSTITUTIO
} FabricaGenusActionis;

nomen enumeratio {
    FABRICA_PROVENIENTIA_REGENERATIO = ZEPHYRUM,
    FABRICA_PROVENIENTIA_RELATIO
} FabricaProvenientia;

nomen enumeratio {
    FABRICA_RECENS = ZEPHYRUM,
    FABRICA_STALUM,
    FABRICA_IGNOTUM,
    FABRICA_NON_IUDICATUM          /* celer: regeneratio omissa */
} FabricaStatus;

nomen structura {
    FabricaGenusIngressus genus;
                   chorda via;
                   chorda suffixa;   /* PLAGULAE: ".c .h" (spatio
                                      * separata); vacua = omnes */
} FabricaIngressus;

nomen structura {
                       chorda via;        /* artificium */
                       chorda scriptura;  /* via relativa intra
                                           * directorium scripturae
                                           * ubi regeneratio cadit */
    FabricaProvenientia provenientia;
} FabricaExitus;

nomen structura {
                  chorda  titulus;
    FabricaGenusActionis  genus;
                     Xar* mandatum;   /* chorda: argv fixum */
                     Xar* ingressus;  /* FabricaIngressus */
                     Xar* exitus;     /* FabricaExitus */
                  chorda  sedes;      /* "plagula:linea" */
                     b32  memorabilis; /* memorabilis="verum": ingressus
                                        * PROBABILITER pleni (clausurae
                                        * manifestis derivatae) -
                                        * verificatio memorata
                                        * regenerationem supplet (T6).
                                        * Absens = FALSUM: regeneratur
                                        * semper sub -plenus. */
} FabricaActio;

/* Sutura: machina discum et processus per eam SOLAM tangit. */
nomen structura {
    vacuum* datum;
    /* FALSUM = plagula absens */
    b32 (*legere)(vacuum* datum, constans character* via,
                  Piscina* piscina, chorda* contentum_out);
    /* nomina (chorda) ordinata; FALSUM = directorium absens */
    b32 (*enumerare)(vacuum* datum, constans character* via,
                     Piscina* piscina, Xar** nomina_out);
    /* mandatum currere (argv fixum) cum directorio scripturae
     * VACUO (sutura id ante cursum vacuat - reliquiae cursus prioris
     * generatorem mutum celarent); FABRICA_SCRIPTURA = scriptura_dir.
     * FALSUM + causa (ultima linea) si fractum. */
    b32 (*currere)(vacuum* datum, constans Xar* mandatum,
                   constans character* scriptura_dir,
                   Piscina* piscina, chorda* causa_out);
    /* binarium '-provenientia' rogare; FALSUM = nulla relatio */
    b32 (*rogare)(vacuum* datum, constans character* via,
                  Piscina* piscina, chorda* relatio_out);
    /* memoria: VERUM si verificatio (titulus, clavis, artificium)
     * iam scripta; 'ingressus' = CLAVIS: sigillum ingressuum et
     * mandati (radix nova in mandato verificationem veterem solvit). NIHIL licet (sine memoria). Consulitur pro
     * actionibus memorabilibus SOLIS. */
    b32 (*meminisse)(vacuum* datum, constans character* titulus,
                     constans Sigillum* ingressus,
                     constans Sigillum* artificium);
    /* verificationem scribere: vocatur SOLUM post RECENS per
     * regenerationem actionis memorabilis (numquam post relationem aut
     * memoriam ipsam). NIHIL licet. */
    vacuum (*inscribere)(vacuum* datum, constans character* titulus,
                         constans Sigillum* ingressus,
                         constans Sigillum* artificium);
    /* memoria sigillorum PER CURSUM (via -> Sigillum*): plagula quae
     * multis ingressibus communis est semel legitur et sigillatur
     * (capsula corporis XLIX MB tribus binariis; ingressus actionis
     * per exitum). NIHIL licet. Arbor intra cursum immota ponitur. */
    TabulaDispersa* sigilla;
    /* regenerationes PER CURSUM (titulus actionis -> chorda* causa;
     * mensura 0 = cursus felix): generator semel per actionem currit,
     * etsi exitus multos habet (silva: XXII fragmenta ex generatore
     * uno). NIHIL licet (tum per exitum currit). */
    TabulaDispersa* regenerationes;
    /* sigilla ingressuum PER CURSUM (titulus actionis -> sigillum aut
     * causa): actio exituum multorum semel explicatur (silva: XXII
     * exitus, LIV manifesta). NIHIL licet. */
    TabulaDispersa* digesta;
} FabricaSutura;

/* Suturam vacuam parare: OMNIA membra NIHIL. Vocans deinde quae
 * praebet ponit - membrum novum postea additum sic tutum manet
 * (T7: 'sigilla' additum, sutura instrumenti membrum non posuit,
 * monstrator purgamenti -> Bus error). */
vacuum
fabrica_suturam_parare (
    FabricaSutura* sutura);

nomen structura {
           chorda artificium;
    FabricaStatus status;
           chorda causa;
} FabricaIudicium;

/* Viae manifesti aedilis: obiecta, capita, vendores (systemata NON -
 * plagulae nostrae non sunt) et inresolutae (sectio citatarum nusquam
 * inventarum, 982fec44). Xar de chorda. FALSUM + causa si
 * malformatum. */
b32
fabrica_manifestum_legere (
                 chorda   contentum,
                Piscina*  piscina,
                    Xar** viae_out,
                    Xar** inresolutae_out,
                 chorda*  causa_out);

/* Sigillum copiae ingressuum: pro quoque ingressu ordine viae,
 * "via NUL sigillum(octetorum)"; manifestum in viae suas explicatur
 * (manifestum ipsum NON - generatum= et commissum= in eo mutantur),
 * directorium in nomina sua (via cum '/' finali, ne cum plagula
 * eiusdem viae confundatur). Exclusa (Xar de chorda, NIHIL licet)
 * praetermittuntur. FALSUM + causa: ingressus absens, manifestum
 * incompletum (inresoluta nominata). */
b32
fabrica_ingressus_sigillare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
               constans Xar* exclusa,
                    Piscina* piscina,
                   Sigillum* sigillum_out,
                     chorda* causa_out);

/* Iudicium unius exitus actionis. plenus FALSUM = celer
 * (regeneratio numquam; relatio et memoria licent). Directorium
 * scripturae: build/fabrica/scriptura/TITULUS (actionis). */
FabricaIudicium
fabrica_iudicare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
     constans FabricaExitus* exitus,
                        b32  plenus,
                    Piscina* piscina);

/* Actiones ordine dependentiae: actio cuius ingressus exitus
 * alterius est post eam; ceteroquin ordo datus (stabilis). Xar de
 * FabricaActio* in actiones datas. NIHIL + causa in cyclo, titulis
 * nominatis. */
Xar*
fabrica_ordinare (
    constans Xar* actiones,   /* FabricaActio (valore) */
         Piscina* piscina,
          chorda* causa_out);

/* Declarationes subsystematis legere (dialectus aedificatio,
 * aedificatio.canon): Xar de FabricaActio (valore), sedes
 * "via:linea". Recusat (NIHIL + causa cum "via:linea"): radix
 * aliena, genus actionis aut ingressus aut provenientiae ignotum,
 * actio sine titulo, sine ingressu, sine exitu, titulus duplex
 * (directoria scripturae colliderent). scriptura absens = via
 * exitus. */
Xar*
fabrica_declarationes_legere (
                 chorda  contentum,
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out);

/* Radix fabrica.stml (dialectus fabrica v2): viae subsystematum
 * (Xar de chorda), ordine documenti. NIHIL + causa si radix non
 * fabrica est aut subsystema sine via. */
Xar*
fabrica_subsystemata_legere (
                 chorda  contentum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out);

/* Via plagulae provenientiae actionis (conventio T7):
 * build/fabrica/provenientia/TITULUS.c - installator eam scribit
 * (tools/provenientia_scribere.sh), binarium eam nectit. */
chorda
fabrica_provenientia_via (
     chorda  titulus,
    Piscina* piscina);

/* Sigillum actionis UT iudex et 'bin/fabrica digestum' id computant:
 * ingressus MINUS plagula provenientiae eius (quae digestum ipsum fert
 * - sine exclusione omne binarium statim post institutionem stalum
 * esset). Functio UNA pro ambobus: scriptum et iudex dissentire
 * nequeunt. */
b32
fabrica_actionem_sigillare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out);

#endif /* FABRICA_H */
