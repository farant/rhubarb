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

/* VOCABULARIUM (spec 1b par. II): GENUS artificii dicit quomodo
 * OBSERVETUR (enumerare, sigillare, locare); STRATEGIA dicit quomodo
 * exitus RECENS esse sciatur (iudicare). Registra nominibus quaeruntur
 * (fabrica_genus_invenire, fabrica_strategia_invenire) - machina
 * numquam super genus commutat. Genus novum = structura nova in
 * lib/fabrica.c, numquam enumeratio crescens. */
nomen structura FabricaGenus     FabricaGenus;
nomen structura FabricaStrategia FabricaStrategia;

nomen enumeratio {
    FABRICA_ACTIO_GENERATOR = ZEPHYRUM,
    FABRICA_ACTIO_FORMATIO,
    FABRICA_ACTIO_INSTITUTIO
} FabricaGenusActionis;

nomen enumeratio {
    FABRICA_RECENS = ZEPHYRUM,
    FABRICA_STALUM,
    FABRICA_IGNOTUM,
    FABRICA_NON_IUDICATUM          /* celer: regeneratio omissa */
} FabricaStatus;

/* particula: quod ingressus ad sigillum actionis confert - via
 * (plagula, aut directorium cum '/' finali, ne cum plagula eiusdem
 * viae confundatur) et sigillum eius */
nomen structura {
      chorda via;
    Sigillum octeti;
} FabricaParticula;

/* locus: ubi artificium habitat (locare) aut quod ingressus legit et
 * custodit (enumerare) - forma geometrica, non genus */
nomen enumeratio {
    FABRICA_LOCUS_PLAGULA = ZEPHYRUM,  /* via ipsa */
    FABRICA_LOCUS_PLAGULAE,            /* plagulae directorii gradu 0,
                                        * suffixis filtratae */
    FABRICA_LOCUS_ARBOR                /* arbor tota sub via */
} FabricaFormaLoci;

nomen structura {
    FabricaFormaLoci forma;
              chorda via;
              chorda suffixa;   /* PLAGULAE: ".c .h" (spatio
                                 * separata); vacua = omnes */
} FabricaLocus;

nomen structura {
     constans FabricaGenus* genus;
                    chorda  via;
                    chorda  suffixa;   /* plagulae: ".c .h" (spatio
                                       * separata); vacua = omnes */
} FabricaIngressus;

nomen structura {
                        chorda via;        /* artificium */
                        chorda scriptura;  /* via relativa intra
                                             * directorium scripturae
                                             * ubi regeneratio cadit */
        constans FabricaGenus* genus;      /* absens in declaratione:
                                             * genus ordinarium
                                             * strategiae */
    constans FabricaStrategia* strategia;  /* attributum
                                             * 'provenientia' */
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
                     Xar* praecondiciones; /* chorda: tituli actionum
                                            * REALIZANDARUM ante hanc -
                                            * ordo sine sigillo (plan 1b
                                            * T2). Exitus 'ignota' sola
                                            * hac via attinguntur. */
} FabricaActio;

/* COMPOSITUM (spec 1b par. II.3): artificium ex artificiis - lista
 * plana partium nominatarum (nulla expressio: decretum Canonis).
 * Iudicium = pessimum partium. */
nomen enumeratio {
    FABRICA_PARS_ARTIFICIUM = ZEPHYRUM, /* via exitus */
    FABRICA_PARS_ACTIO,                 /* omnes exitus actionis */
    FABRICA_PARS_COMPOSITUM             /* compositum aliud */
} FabricaFormaPartis;

nomen structura {
    FabricaFormaPartis forma;
                chorda titulus;
                chorda sedes;
} FabricaPars;

nomen structura {
    chorda  titulus;
       Xar* partes;   /* FabricaPars */
    chorda  sedes;
} FabricaCompositum;

/* Actum: exitus unius cursus mandati IN LOCO (sanare, plan 1b T3) */
nomen structura {
       s32 codex;        /* exitus processus; -1 = non incepit aut
                          * terminus excessus */
       i32 duratio_ms;
    chorda cauda;        /* ultimae lineae (erratum, aliter effusio) */
} FabricaActum;

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
     * mandati (radix nova in mandato verificationem veterem solvit).
     * NIHIL licet (sine memoria). Consulitur pro actionibus
     * memorabilibus SOLIS. */
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
    /* mandatum actionis IN LOCO currere (FABRICA_SCRIPTURA nulla:
     * generatores in arbore scribunt), effusionem in acta_via
     * scribere. FALSUM = incipi non potuit aut terminus excessus
     * (causa in cauda). NIHIL licet: sanare tum siccum solum potest
     * (plan 1b T3). */
    b32 (*agere)(vacuum* datum, constans FabricaActio* actio,
                 constans character* acta_via, Piscina* piscina,
                 FabricaActum* actum_out);
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

/* GENUS: quomodo artificium observetur. Verba NIHIL licent ubi
 * dictum. */
structura FabricaGenus {
    constans character* titulus;   /* nomen in declaratione */
    /* sigillare: particulas (FabricaParticula) ingressus addere -
     * quod sigillum actionis confert. FALSUM + causa: absens,
     * malformatum, incompletum. Exclusa (Xar de chorda, NIHIL licet)
     * praetermittuntur. */
    b32 (*sigillare)(constans FabricaSutura* sutura,
                     constans FabricaIngressus* ingressus,
                     constans Xar* exclusa, Piscina* piscina,
                     Xar* particulae, chorda* causa_out);
    /* enumerare: loci (FabricaLocus) quos ingressus legit aut custodit
     * (commissio '-tacta'). NIHIL = ex particulis sigillandi:
     * plagula -> PLAGULA, 'dir/' -> PLAGULAE sine suffixis. */
    b32 (*enumerare)(constans FabricaSutura* sutura,
                     constans FabricaIngressus* ingressus,
                     Piscina* piscina, Xar* loci,
                     chorda* causa_out);
    /* locare: loci quos exitus huius generis scribit. NIHIL = genus
     * ingressus solum (exitus esse nequit). */
    b32 (*locare)(constans FabricaExitus* exitus, Piscina* piscina,
                  Xar* loci);
    /* VERUM: octeti ex ingressibus determinati (regeneratio licet).
     * Binarium FALSUM: LC_UUID et signatura (mensuratum, 1a T6). */
    b32 reproducibile;
};

/* STRATEGIA: quomodo exitus RECENS esse sciatur, genere et actione
 * producente datis (regeneratio = 'curre productorem meum'). */
structura FabricaStrategia {
    constans character* titulus;          /* 'provenientia' */
    constans character* genus_ordinarium; /* genus exitus sine
                                           * attributo genus */
    /* VERUM: artificium octetis comparatur - '-tacta' id solum
     * iudicat (commissa generata); genus reproducibile postulat */
    b32 octetis_comparat;
    /* FALSUM: 'ignota' - numquam iudicatur; exitus eius praecondicio
     * sola est (numquam pars compositi, numquam ingressus) */
    b32 iudicatur;
    /* iudicium exitus, ingressibus actionis IAM sigillatis. plenus
     * FALSUM = celer. */
    FabricaIudicium (*iudicare)(constans FabricaSutura* sutura,
                                constans FabricaActio* actio,
                                constans FabricaExitus* exitus,
                                constans Sigillum* ingressus,
                                b32 plenus, Piscina* piscina);
};

/* Registra: fasciculus, configuratio, instrumentum (octeti plagulae;
 * nomina tria, implementatio una), directorium, manifestum, plagulae,
 * manifesta, radices, binarium. NIHIL si titulus ignotus. */
constans FabricaGenus*
fabrica_genus_invenire (
    chorda titulus);

/* Registra: regeneratio (memoria ante eam, actionibus memorabilibus
 * solis), relatio, ignota (praecondicio: numquam iudicatur). NIHIL si
 * titulus ignotus. */
constans FabricaStrategia*
fabrica_strategia_invenire (
    chorda titulus);

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
 * alterius est, aut quae eam praecondicionem nominat, post eam;
 * ceteroquin ordo datus (stabilis). Xar de FabricaActio* in actiones
 * datas. NIHIL + causa in cyclo, titulis nominatis. */
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

/* Loci (FabricaLocus) omnium ingressuum actionis: verbum 'enumerare'
 * cuiusque generis (aut ex particulis eius). FALSUM + causa si
 * ingressus explicari nequit. */
b32
fabrica_actionem_enumerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* loci,
                    chorda* causa_out);

/* VERUM si via quaevis 'viae' (Xar de chorda, e.g. plagulae
 * commissionis) actionem TANGIT: locus ingressus eius (PLAGULA ipsa;
 * plagula nova/deleta in PLAGULIS suffixo congruens; via sub ARBORE)
 * aut locus exitus eius ('locare'). Ingressus explicari nequeunt ->
 * VERUM (conservativum: iudex IGNOTUM nominabit). T8: commissio
 * iudicat sola tacta. */
b32
fabrica_actio_tacta (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
              constans Xar* viae,
                   Piscina* piscina);

/* Composita plagulae declarationum (elementa 'compositum' cum
 * partibus 'pars': artificium=, actio= aut compositum=, unum
 * exacte). NIHIL + causa "via:linea": compositum sine titulo aut sine
 * parte, pars sine nomine aut nominibus pluribus, titulus duplex.
 * Xar de FabricaCompositum (fortasse vacua). */
Xar*
fabrica_composita_legere (
                 chorda  contentum,
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out);

/* Artificia compositi 'titulus', plana et sine duplicibus (Xar de
 * chorda, ordine primae apparitionis). NIHIL + causa: compositum
 * ignotum, pars ignota, cyclus (tituli nominati), pars cuius strategia
 * non iudicatur (praecondicio). Pessimum associativum est: planum ==
 * nidificatum. */
Xar*
fabrica_compositum_explicare (
     constans Xar* composita,  /* FabricaCompositum */
     constans Xar* actiones,   /* FabricaActio */
           chorda  titulus,
          Piscina* piscina,
           chorda* causa_out);

/* Pessimum iudiciorum (Xar de FabricaIudicium) ordine RECENS <
 * NON_IUDICATUM < IGNOTUM < STALUM; causa nominat artificia pessima
 * (III, deinde "+N"). artificium = titulus. */
FabricaIudicium
fabrica_iudicia_coniungere (
     constans Xar* iudicia,
           chorda  titulus,
          Piscina* piscina);

/* Praecondiciones omnium actionum: actio nominata exstat; exitus
 * strategiae non iudicatae numquam ingressus alterius (praecondicio
 * sola licet). FALSUM + causa "sedes: ...". */
b32
fabrica_praecondiciones_probare (
    constans Xar* actiones,
         Piscina* piscina,
          chorda* causa_out);

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


/* ==================================================
 * SANARE (plan 1b T3): iudicare, agere, iterum iudicare
 * ================================================== */

nomen enumeratio {
    FABRICA_SANATUM = ZEPHYRUM, /* actum, exitus RECENS post */
    FABRICA_PRAEPARATUM,        /* praecondicio (ignota) acta,
                                 * exitus 0 */
    FABRICA_FRACTUM,            /* codex != 0, terminus, aut post-
                                 * condicio non RECENS */
    FABRICA_OMISSUM,            /* dependentia fracta aut omissa */
    FABRICA_AGENDUM,            /* siccum: stalum/ignotum nunc */
    FABRICA_FORTASSE            /* siccum: post actionem agendam */
} FabricaEventus;

nomen structura {
     constans FabricaActio* actio;
            FabricaEventus  eventus;
                    chorda  causa;
                       i32  duratio_ms;
} FabricaSanatio;

/* Sanare. 'electa': viae artificiorum (Xar de chorda; NIHIL = omnia);
 * actiones earum et omnes supra eas (ingressus, praecondiciones) in
 * ambitu. Per ordinem: exitus actionis NUNC iudicantur (plenus);
 * omnes RECENS -> nihil (non relatum); dependentia FRACTUM/OMISSUM ->
 * OMISSUM; aliter praecondiciones ignotae (semel per cursum), agere,
 * memoriae per cursum (sigilla, digesta, regenerationes) VACANTUR,
 * exitus iterum iudicantur -> SANATUM aut FRACTUM. Actiones quarum
 * exitus omnes 'ignota' sunt praecondicione SOLA realizantur. siccum:
 * nihil agitur (AGENDUM, FORTASSE). Acta:
 * build/fabrica/acta/TITULUS.log.
 * Xar de FabricaSanatio (vacua = nihil agendum); NIHIL + causa si via
 * electa a nulla actione producitur. */
Xar*
fabrica_sanare (
    constans FabricaSutura* sutura,
              constans Xar* ordo,      /* FabricaActio*, ordinatae */
              constans Xar* electa,
                       b32  siccum,
                   Piscina* piscina,
                    chorda* causa_out);

#endif /* FABRICA_H */
