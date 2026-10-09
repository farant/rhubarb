#ifndef FABRICA_H
#define FABRICA_H

/* <aedilis corpus="lib/fabrica_genera.c"/> */
/* <aedilis corpus="lib/fabrica_declarationes.c"/> */
/* <aedilis corpus="lib/fabrica_ordo.c"/> */
/* <aedilis corpus="lib/fabrica_sanare.c"/> */
/* <aedilis corpus="lib/fabrica_gradus.c"/> */
/* <aedilis corpus="lib/fabrica_probationes_c.c"/> */

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "sigillum.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include "lectiones.h"


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
 * lib/fabrica_genera.c (strategia: lib/fabrica.c), numquam
 * enumeratio crescens. */
nomen structura FabricaGenus     FabricaGenus;
nomen structura FabricaStrategia FabricaStrategia;
nomen structura FabricaSanatio   FabricaSanatio;
nomen structura FabricaGradus    FabricaGradus;

nomen enumeratio {
    FABRICA_ACTIO_GENERATOR = ZEPHYRUM,
    FABRICA_ACTIO_FORMATIO,
    FABRICA_ACTIO_INSTITUTIO,
    /* IUDICIUM (spec 3 par. XII): porta ut actio - exitus = plagula
     * verdicti (provenientia 'verdictum'), clavis = vestigium libri
     * lectionum cursus TRANSEUNTIS; lectiones="verum" necessarium.
     * Iudicium numquam currit; sanare solum, et solum nominatum
     * (verritiones eam omittunt, numquam in unda simul). */
    FABRICA_ACTIO_IUDICIUM
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

/* attributum elementi gradus (fabrica-6 T5): exemplar, praeter... */
nomen structura {
    chorda titulus;
    chorda valor;
} FabricaAttributum;

/* CLAUSURA C scopi (fabrica-6 T6c, per sutura->clausura_c): quod
 * probatio C nectit et legit */
nomen structura {
    Xar* fontes;         /* chorda: nectendi, scopus primus (.c/.m) */
    Xar* capita;         /* chorda: capita clausurae (ingressus
                          * clavis) */
    Xar* vexilla_nexus;  /* chorda: e.g. "-framework Cocoa" (verba
                          * spatio separata) */
    Xar* facultates;     /* chorda: scopi solius (fenestra, rete,
                          * repositorium) */
} FabricaClausuraC;

/* MEMBRUM gradus (fabrica-6 T5): id stabilis = "<actio>/<titulus>" */
nomen structura {
    chorda titulus; /* intra actionem: "probatio_x" */
    chorda fons;    /* via fontis (arbori relativa); vacua licet */
} FabricaMembrum;

/* SECTIO (fabrica-7, credo v2): verdictum sectionis membri gradus, ex
 * plagula <area>/credo.tsv (CREDO_VERDICTA, lineae SECTIO). Suita non
 * conversa: probationes_c post cursum ipsa plagulam syntheticam
 * scribit - sectio una 'totum' (exitus ex codice, numeri ex compendio
 * effusionis) - ergo lector unus, forma una. */
nomen structura {
    chorda titulus;
    chorda exitus;      /* TRANSIIT | FRACTA | VACUA | ABORTA (credo);
                         * gradus II credo: OMISSA, NOTA_FRACTA,
                         * INOPINATA */
       i32 praeteriti;
       i32 totales;
       i32 ms;          /* tempus - NUMQUAM in verdicto sigillato
                         * (non deterministicum); cursus solum */
    chorda fractura;    /* "filum:versus genus expressio"; vacua si
                         * transiit */
} FabricaSectio;

nomen structura {
     Xar* sectiones;     /* FabricaSectio, ordine plagulae */
     b32  completa;      /* linea SUITA adest; FALSUM = ruina post
                         * sectionem ultimam notatam */
     b32 synthetica;    /* 'totum' a fabrica scriptum */
} FabricaSectiones;

nomen structura {
                  chorda  titulus;
    FabricaGenusActionis  genus;
                     Xar* mandatum;   /* chorda: argv fixum */
                     Xar* ingressus;  /* FabricaIngressus */
                     Xar* exitus;     /* FabricaExitus */
                  chorda  sedes;      /* "plagula:linea" */
                     b32  celer;       /* celer="verum": regeneratio
                                        * tam vilis ut sub iudicio
                                        * CELERI quoque currat (copiae
                                        * ~/.bin - uncus sessionis eas
                                        * videt; plan 1b T5) */
                     b32 memorabilis; /* memorabilis="verum": ingressus
                                        * PROBABILITER pleni (clausurae
                                        * manifestis derivatae) -
                                        * verificatio memorata
                                        * regenerationem supplet (T6).
                                        * Absens = FALSUM: regeneratur
                                        * semper sub -plenus. */
                     b32 iudex;        /* iudex="verum" (fabrica-6 T3):
                                        * STADIUM IUDICUM - iudicatur
                                        * primum; stalus = ceteri
                                        * recusantur, sanare eum primum
                                        * sanat */
                     b32 lectiones;   /* lectiones="verum" (plan 2 T2):
                                       * clavis = vestigium libri
                                       * lectionum (L A X D) ultimi
                                       * cursus congruentis */
                     Xar* praecondiciones; /* chorda: tituli actionum
                                            * REALIZANDARUM ante hanc -
                                            * ordo sine sigillo (plan 1b
                                            * T2). Exitus 'ignota' sola
                                            * hac via attinguntur. */
                     Xar* vestigia;   /* FabricaLocus: opera propria
                                       * (scriptura, directoria
                                       * vacuata) - plan 1b T4 */
                     Xar* communia;   /* FabricaLocus: area COMMUNIS
                                       * (cache idempotens, obiecta
                                       * per mtime): numquam simul */
                     Xar* dependentiae; /* chorda: tituli actionum
                                         * quarum exitus ingressus
                                         * 'enumerat' (clausurae
                                         * manifestorum quoque). NIHIL
                                         * = non computatae: ordo per
                                         * vias declaratas (T5) */
                  chorda signum;     /* iudicium (plan 5 T3): praefixum
                                       * litterale quod cursor portae
                                       * edit; fabrica verdictum IPSA
                                       * scribit ('<nomen>: <signum>
                                       * <verbum>'). Vacuum = mandatum
                                       * verdictum suum scribit */
    /* GRADUS (fabrica-6 T5) - omnia NIHIL in actione sine gradu */
    constans FabricaGradus* gradus;     /* genus gradus */
                       Xar* attributa;  /* FabricaAttributum: elementi
                                         * gradus (exemplar, praeter) */
                       Xar* ambitus;    /* chorda: NOMINA variabilium
                                         * declarata ultra basim */
    constans FabricaMembrum* membrum;   /* actio synthetica: membrum
                                         * suum (parens: NIHIL) */
                       Xar* post;       /* chorda: tituli actionum
                                         * PRODUCENDARUM ante hanc
                                         * (<post actio="X"/>; fabrica-6
                                         * T7) - ORDO solus (_pendet),
                                         * numquam clavis: membrum quod
                                         * productum legit per vestigium
                                         * suum clavatur, cetera non.
                                         * Membra gradus a parente
                                         * hereditant. NIHIL licet. */
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

/* Vestigium: plagula una in photographia arboris (plan 1b T4).
 * Scriptura detegitur per tempus aut mensuram - rescriptio octetis
 * eisdem quoque (tempus novum) nominatur. */
nomen structura {
    chorda via;
       s64 tempus_ns;   /* mtime */
       s64 mensura;
} FabricaVestigium;

/* Actum: exitus unius cursus mandati IN LOCO (sanare, plan 1b T3) */
nomen structura {
       s32 codex;        /* exitus processus; -1 = non incepit aut
                          * terminus excessus */
       i32 duratio_ms;
    chorda cauda;        /* ultimae lineae (erratum, aliter effusio) */
} FabricaActum;

/* Sutura: machina discum et processus per eam SOLAM tangit. */
/* lectio una vestigii (plan 2 T2): genus, via (arbori relativa),
 * sigillum status eius tempore cursus (L: contenta; A/X: praesentia;
 * D: nomina ordinata) */
nomen structura {
    LectioGenus genus;
         chorda via;
       Sigillum sigillum;
} FabricaLectio;

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
                   constans character* liber_via,  /* plan 2 T2:
                                     * FABRICA_LECTIONES; NIHIL = nullus */
                   Piscina* piscina, chorda* causa_out,
                   i32* duratio_ms_out);  /* parcum …AR15: tempus
                                           * regenerationis (cursus) */
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
    /* agere SIMUL (plan 2 T6): 'numerus' actiones TUTAE (lectiones=
     * "verum") parallele; redit cum omnes finitae. libri[k] = liber
     * lectionum filii k (FABRICA_LECTIONES ante incipere positus).
     * Fractura una: currentes finiunt, nova non incipiunt
     * (incepta_out[k] FALSUM, codex -I, cauda VACUA = consulto non
     * inceptum; cauda plena = incipi non potuit). NIHIL = seriatim
     * (FABRICA_FILA=1): via vetus intacta. Nucleus: tutae simul (scrip-
     * turae S contra vestigium membri; photographia una undae contra
     * unionem), non tutae SOLAE. */
    /* currere SIMUL (T6b, praevisio iudicii): regenerationes
     * 'numerus' actionum (lectiones="verum") parallele, quaeque in
     * scripturam suam VACUAM (FABRICA_SCRIPTURA) cum libro suo; redit
     * cum omnes finitae. NIHIL = praevisio nihil agit. */
    vacuum (*currere_simul)(vacuum* datum,
                            constans FabricaActio* constans* actiones,
                            i32 numerus,
                            constans character* constans* scripturae,
                            constans character* constans* libri,
                            Piscina* piscina, b32* felices_out,
                            chorda* causae_out, i32* durationes_out);
    /* praevisio (T6b, internum): non NIHIL = _regenerare petitiones
     * COLLIGIT, non currit (fabrica_regenerationes_praevidere solum) */
    Xar* praevisio;
    vacuum (*agere_simul)(vacuum* datum,
                          constans FabricaActio* constans* actiones,
                          i32 numerus,
                          constans character* constans* acta_viae,
                          constans character* constans* libri,
                          Piscina* piscina, FabricaActum* acta_out,
                          b32* incepta_out);
    /* photographia: arbor tota (sine .git) et loci declarati extra
     * arborem ('~/'), Xar de FabricaVestigium ORDINATA per viam.
     * NIHIL licet: sanare tum vestigia non probat (plan 1b T4). */
    b32 (*vestigium_capere)(vacuum* datum, Piscina* piscina,
                            Xar** vestigia_out);
    /* cursus (plan 1b T7): post quodque actum VERUM (sanatum,
     * fractum, praeparatum - non omissum, numquam siccum) sanatio
     * scribitur; legere = duratio cursus ULTIMI sanati tituli (FALSUM:
     * nullus). NIHIL licent. */
    vacuum (*cursum_inscribere)(vacuum* datum,
                                constans FabricaSanatio* sanatio);
    b32 (*cursum_legere)(vacuum* datum, constans character* titulus,
                         i32* duratio_ms_out);
    /* vestigia lectionum (plan 2 T2): clavis = (titulus, via exitus,
     * sigillum ingressuum declaratorum, sigillum artificii) - vestigium
     * ULTIMUM per (titulus, exitus): actio exituum multorum (fragmenta
     * silvae, XXII) cuique exitui suum servat;
     * legere FALSUM = nullum. NIHIL licent (sine vestigiis). */
    b32 (*lectiones_legere)(vacuum* datum, constans character* titulus,
                            constans character* exitus,
                            constans Sigillum* ingressus,
                            constans Sigillum* artificium,
                            Piscina* piscina, Xar** lectiones_out);
    vacuum (*lectiones_scribere)(vacuum* datum,
                                 constans character* titulus,
                                 constans character* exitus,
                                 constans Sigillum* ingressus,
                                 constans Sigillum* artificium,
                                 constans Xar* lectiones);
    /* vestigia ULTIMA tituli (omnes exitus, quaevis clavis) - ORDO:
     * viae lectae (L/X/A) et enumerata (D) actionis lectiones="verum"
     * arcus ad producentes dant, ut manifesta olim (plan 2 T2). FALSUM
     * = nullum vestigium (clonus recens: nullus arcus). NIHIL licet. */
    b32 (*lectiones_ultimae)(vacuum* datum, constans character* titulus,
                             Piscina* piscina, Xar** lectiones_out);
    /* PARTICULAE TRANSITUS (plan-5 T1): ingressus declarati singuli
     * (via + sigillum; et '<mandatum>', '<verdictum>') transitus ULTIMI
     * per (titulus, exitus) - iudex nominat QUIS mutatus est cum
     * clavis transitus non invenitur. NIHIL licent (causa vaga). */
    vacuum (*particulas_scribere)(vacuum* datum,
                                  constans character* titulus,
                                  constans character* exitus,
                                  constans Xar* particulae);
    b32 (*particulas_legere)(vacuum* datum, constans character* titulus,
                             constans character* exitus,
                             Piscina* piscina, Xar** particulae_out);
    /* VERDICTUM PONERE (plan 5 T3): actio iudicium cum signo - fabrica
     * verdictum ante cursum delet (contentum NIHIL) et post transitum
     * scribit (atomice). NIHIL: actio signi FRACTA ('sine
     * verdictum_ponere'). */
    b32 (*verdictum_ponere)(vacuum* datum, constans character* via,
                            constans chorda* contentum);
    /* praefixum absolutum arboris, ex viis libri demendum (instrumenta
     * vias absolutas scribere possunt); vacua = nihil demitur */
    chorda radix;
    /* AUDITUS MEMORIAE (plan 2 T2, Review Focus 5): ictus vestigii aut
     * memoriae sub -plenus regeneratur tamen et confertur. 0 = nullus;
     * I = omnes (iudicare -audit); N = unus ex N (specimen
     * determinatum: octetus primus clavis ingressuum modulo N) */
    i32 auditus;
    /* ==== IUDICIUM (spec 3 par. XII, T5b) - omnia NIHIL licent ==== */
    /* ambitus quem fabrica PORTAE dat: VERUM + valor si variabilis
     * adest. Lectio E cuius valor ab eo differt intra portam posita est
     * (cursor) et ex scriptis clavi inclusis pendet - omittitur;
     * aequalis = ingressus externus, clavatur. NIHIL: E non
     * verificabilis (vestigium nullum). */
    b32 (*ambitus)(vacuum* datum, constans character* titulus,
                   Piscina* piscina, chorda* valor_out);
    /* species viae (FabricaSpecies): FIFO, socket, machina = ALIA - in
     * vestigio portae non sigillabilis (IGNOTUM). NIHIL: non probatur. */
    i32 (*species)(vacuum* datum, constans character* via);
    /* exitus omnium actionum declaratarum (via -> NIHIL): lectio sub
     * build/ sine S in libro eodem DOMINUM declaratum habere debet */
    TabulaDispersa* exitus_noti;
    /* identitas compilatoris clang (genus ingressus 'identitas_clang'):
     * eadem ac bin/compilator (via vera, mensura, mtime, inodus;
     * FABRICA_CLANG: octeti eius). FALSUM + causa: ignota. */
    b32 (*identitas)(vacuum* datum, Piscina* piscina,
                     Sigillum* identitas_out, chorda* causa_out);
    /* effectus scripti (genus ingressus 'effectus', effectus-plan T7):
     * crusta/effectus.sh -clavis <via> - lineae clavis (genus, via,
     * extra; crusta/instrumenta/effectus.c) et codex exitus (0 sanum).
     * FALSUM: currere nequit. */
    b32 (*effectus)(vacuum* datum, constans character* via,
                    Piscina* piscina, chorda* effusio_out,
                    i32* codex_out);
    /* status repositorii (genus 'repositorium', fabrica-6 T2): clavis
     * "commissum" = sha HEAD (per lib/git, non per processum).
     * FALSUM: non repositorium aut clavis ignota. */
    b32 (*repositorium)(vacuum* datum, constans character* clavis,
                        Piscina* piscina, chorda* valor_out);
    /* AUDITUS TRANSITUS (spec 3 par. XIII): sub sanare, actio iudicium
     * RECENS electa (auditus I = omnes, N = unus ex N) tamen currit;
     * vestigium VETUS hic servatur (titulus -> Xar de FabricaLectio) ut
     * defectus lectiones novas nominet. NIHIL: auditus sine nominibus. */
    TabulaDispersa* audita;
    /* ==== GRADUS (fabrica-6 T5) - NIHIL: membra agi nequeunt ==== */
    /* aream parare: directorium (et <area>tmp/) creatum et VACUUM -
     * reliquiae cursus prioris membrum mutum celarent */
    b32 (*area_parare)(vacuum* datum, constans character* area);
    /* argv currere cum cwd = RADIX arboris (probationes viis radicis
     * utuntur - decisio Frani T6b) et ambitu EXACTO (Xar de "N=V";
     * nihil hereditatur) praeter FABRICA_LECTIONES = liber_via
     * (absoluta; liber vetus deletus et VACUUS creatus - processus
     * qui nihil notat vestigium vacuum, non absens, relinquit). area:
     * ubi scripturae cadunt. Acta (effusio, erratum) in acta_via.
     * FALSUM = incipi non potuit (cauda causam dicit). */
    b32 (*in_area_currere)(vacuum* datum, constans Xar* argv,
                           constans character* area,
                           constans Xar* ambitus,
                           constans character* liber_via,
                           constans character* acta_via,
                           Piscina* piscina, FabricaActum* actum_out);
    /* CLAUSURA C (fabrica-6 T6c): scopi (probationis .c) per
     * aedilis_derivare + aedilis_silva. FALSUM + causa: recusatio
     * aedilis (annotatio mala, facultas extra scopum...). */
    b32 (*clausura_c)(vacuum* datum, constans character* scopus,
                      Piscina* piscina, FabricaClausuraC* clausura_out,
                      chorda* causa_out);
    /* COMPILARE fontem in obiectum (via data) per thesaurum
     * compilatoris; vexilla domus (aedilis.stml: communia, per
     * fontem, vendor) ab sutura. FALSUM + erratum clang. */
    b32 (*compilare)(vacuum* datum, constans character* fons,
                     constans character* obiectum, Piscina* piscina,
                     chorda* erratum_out);
} FabricaSutura;

/* species viae (sutura->species) */
nomen enumeratio {
    FABRICA_SPECIES_ABSENS = ZEPHYRUM,
    FABRICA_SPECIES_PLAGULA,
    FABRICA_SPECIES_DIRECTORIUM,
    FABRICA_SPECIES_ALIA          /* FIFO, socket, machina */
} FabricaSpecies;

/* via libri lectionum actionis (arbori relativa):
 * build/fabrica/lectiones/<titulus>.tsv - nucleus (iudicium,
 * regeneratio) et instrumentum (agere actionis iudicium) eandem
 * computant */
chorda
fabrica_liber_via (
     chorda  titulus,
    Piscina* piscina);

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

/* SUMPTUS (fabrica-6 T1, census chassis): quantum sigillatio costat -
 * VILIS (plagula una aut valor), MEDIUS (directorium, manifestum),
 * CARUS (processus externus: effectus). Ordo est, non mensura. */
nomen enumeratio {
    FABRICA_SUMPTUS_VILIS = ZEPHYRUM,
    FABRICA_SUMPTUS_MEDIUS,
    FABRICA_SUMPTUS_CARUS
} FabricaSumptus;

/* GENUS: quomodo artificium observetur. Verba NIHIL licent ubi
 * dictum. CHASSIS (fabrica-6 T1): omne genus registratum fixum
 * conformitatis habere DEBET (probatio_fabrica_genera, sectio
 * chassis, per fabrica_genera_numerus enumerat) - proprietates infra
 * in censu ('bin/fabrica census') nominantur. */
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
    /* CENSUS (fabrica-6 T1): VERUM si particulae per viam nominantur
     * (causae 'ingressus mutatus: X' dicere possunt); FALSUM =
     * sigillum crassum (particula una pro toto) */
               b32 particulae_nominatae;
    FabricaSumptus sumptus;
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


/* ==================================================
 * GRADUS (fabrica-6 T5): genus gradus explicat declarationem actionis
 * in MEMBRA; quodque membrum ACTIO SYNTHETICA fit - titulus
 * "<actio>/<membrum>", genus iudicium, lectiones="verum", vestigium =
 * area membri, exitus = verdictum in area. Clavis per vestigium
 * lectionum, transitus servati, defectus iterum currunt, scripturae
 * extra aream nominatae: machina iudicii hodierna tota. Genus
 * nominationem possidet (nullum templum, nulla expressio).
 * ================================================== */

structura FabricaGradus {
    constans character* titulus;   /* nomen elementi in declaratione */
    /* membra ex attributis (per sutura->enumerare): Xar de
     * FabricaMembrum ordinata per nomen. FALSUM + causa: attributum
     * deest aut malum. */
    b32 (*membra)(constans FabricaSutura* sutura,
                  constans FabricaActio* actio, Piscina* piscina,
                  Xar* membra_out, chorda* causa_out);
    /* ingressus STATICI membri (FabricaIngressus) in clavem actionis
     * syntheticae (gradus 'nectere' T6: clausura, vexilla, identitas
     * clang). NIHIL = nulli (clavis = vestigium solum). */
    b32 (*ingressus)(constans FabricaSutura* sutura,
                     constans FabricaActio* actio,
                     constans FabricaMembrum* membrum, Piscina* piscina,
                     Xar* ingressus_out, chorda* causa_out);
    /* membrum agere in area (parata et vacua) cum ambitu dato, per
     * sutura->in_area_currere (liber: fabrica_liber_via, acta:
     * fabrica_acta_via - titulo actionis syntheticae). FALSUM = non
     * actum; codex 0 = transitus: cauda tum NOTA VERDICTI (vacua =
     * nulla; '<id>: transiit (nota)') - deterministica esse debet,
     * verdictum artificium sigillatum est. */
    b32 (*agere)(constans FabricaSutura* sutura,
                 constans FabricaActio* actio,
                 constans FabricaMembrum* membrum,
                 constans character* area, constans Xar* ambitus,
                 Piscina* piscina, FabricaActum* actum_out);
    /* CENSUS */
               b32 lectiones_dynamicae; /* clavis ex vestigio cursus */
    FabricaSumptus sumptus;
};

/* Registra: fasciculus, configuratio, instrumentum (octeti plagulae;
 * nomina tria, implementatio una), directorium, manifestum, plagulae,
 * manifesta, radices, binarium. NIHIL si titulus ignotus. */
constans FabricaGenus*
fabrica_genus_invenire (
    chorda titulus);

/* REGISTRUM ENUMERATUM (fabrica-6 T1): numerus generum et genus ad
 * indicem (0 .. numerus-1; NIHIL extra) - chassis et census per haec,
 * numquam per indicem manu scriptum. */
i32
fabrica_genera_numerus (vacuum);

/* LOCI LECTI generis (census, '-tacta'): "ex particulis" (enumerare
 * NIHIL: plagulae sigillatae ipsae), "nulli" (enumerare explicite
 * nihil - genus quaestionibus inversis INVISIBILE), "proprii"
 * (enumeratio propria) */
constans character*
fabrica_genus_loci (
    constans FabricaGenus* genus);

/* AXES DUO (fabrica-6 T2): genus ex pari (res, clavis) - NIHIL si par
 * nullum genus habet (recusatio nominat: signum designi). Et inverse:
 * par generis ("res/clavis") aut NIHIL (genus alias solum: configuratio,
 * binarium, radices). */
constans FabricaGenus*
fabrica_genus_ex_pari (
    chorda res,
    chorda clavis);

constans character*
fabrica_genus_par (
    constans FabricaGenus* genus);

constans FabricaGenus*
fabrica_genus_obtinere (
    i32 index);

/* Registra: regeneratio (memoria ante eam, actionibus memorabilibus
 * solis), relatio, ignota (praecondicio: numquam iudicatur), verdictum
 * (actionis iudicium solius: plagula verdicti + vestigium transitus,
 * numquam currit). NIHIL si titulus ignotus. */
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

/* Dependentiae per LOCOS (plan 1b T5): pro quaque actione, actiones
 * quarum exitus aliquis in loco ingressus eius cadit - verbum
 * 'enumerare' cuiusque generis, ergo clausurae manifestorum quoque
 * (latina.h, silva.c, capsulae in clausuris). Ingressus qui enumerari
 * nequit (manifestum absens, clonus recens): via declarata. Genus
 * 'instrumentum' nihil enumerat: instrumentum ADHIBETUR, non
 * consumitur (cyclus bootstrap aedilis -> amalgama -> fontes ->
 * aedilis sic frangitur). Actio se ipsam numquam (praelatio).
 * actio->dependentiae ponitur; fabrica_ordinare, sanare, undae eas
 * sequuntur. FALSUM solum si memoria deficit. */
b32
fabrica_dependentias_computare (
    constans FabricaSutura* sutura,
                       Xar* actiones,   /* FabricaActio (valore) */
                   Piscina* piscina,
                    chorda* causa_out);

/* Actiones ordine dependentiae: actio cuius ingressus exitus
 * alterius est (aut, computatis dependentiis, quam ingressus eius
 * enumerat), aut quae eam praecondicionem nominat, post eam;
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

/* Idem cum FAMILIIS (plan 2 T4): elementum <familia titulus via
 * praefixum? suffixum?> templum STML unum fert (<#@id basis="@basis"
 * fons="@fons"> corpus = actio una </#>); per plagulam directorii
 * 'via' (sutura->enumerare) congruentem vocatio synthetica
 * <<#@id basis="..." fons="via/plagula">> per stml_expandere impletur
 * (argumenta sola quae templum declarat; nulla lingua expressionum).
 * Instantia: titulus 'familia:basis' (templum titulum ferre nequit),
 * fons ingressus fasciculus. basis = nomen sine suffixo. Recusat:
 * familia sine titulo/via, templum non unum, directorium absens,
 * vitium expansionis, corpus non actio una. Sutura NIHIL = lector
 * purus: familia recusatur (fabrica_declarationes_legere). */
Xar*
fabrica_declarationes_legere_cum_sutura (
                    chorda  contentum,
        constans character* via,
    constans FabricaSutura* sutura,
                   Piscina* piscina,
       InternamentumChorda* intern,
                    chorda* causa_out);

/* PRAEVISIO (T6b): regenerationes quas iudicium (plenus) actionum
 * 'actiones' (FabricaActio*) posceret - SOLAE actiones lectiones="verum"
 * (scripturae notae, ut sanare simul) - per currere_simul SIMUL currit
 * et in sutura->regenerationes memorat; iudicium sequens (ordine suo,
 * immutatum) eas memoratas invenit. Iudicia praevisionis abiciuntur.
 * Nihil agit si currere_simul aut regenerationes NIHIL. */
vacuum
fabrica_regenerationes_praevidere (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                       b32  plenus,
                   Piscina* piscina);

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
 * ingressus MINUS directorium provenientiae TOTUM (build/fabrica/
 * provenientia/: plagulae digestum ipsum ferunt; familia binariorum
 * plagulam per binarium habet - 1b T5)
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
 * VESTIGIA (plan 1b T4): actio scribit SOLUM intra vestigium suum
 * ================================================== */

/* Viae (Xar de chorda) novae, deletae aut mutatae (tempus aut
 * mensura) inter photographias 'ante' et 'post' (ordinatae) EXTRA
 * vestigium actionis: 'locare' exituum + vestigia + communia +
 * involucrum (build/fabrica/acta/TITULUS.log,
 * build/fabrica/provenientia/TITULUS.{c,o},
 * build/fabrica/scriptura/TITULUS/, build/fabrica.db{,-wal,-shm},
 * build/fabrica/sera). Vacua = sanum. */
Xar*
fabrica_vestigia_comparare (
    constans FabricaActio* actio,
             constans Xar* ante,
             constans Xar* post,
                  Piscina* piscina);

/* Undae: actiones (Xar de FabricaActio*, ordinatae) quae SIMUL
 * currere possent - nulla dependentia inter eas, vestigia disiuncta,
 * neutra communia tangit. Xar de Xar de FabricaActio*, ordine undarum.
 * Monstratur solum (Q41: sanare seriatim currit). */
Xar*
fabrica_undas_formare (
    constans Xar* ordo,
         Piscina* piscina);


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
    FABRICA_FORTASSE,           /* siccum: post actionem agendam */
    FABRICA_AUDITUM_DISCORS,    /* auditus: memoria/vestigium RECENS
                                 * dicebat, regeneratio differt (plan
                                 * 2 T2) */
    FABRICA_IUDICIUM            /* regeneratio iudicis (cursus solum,
                                 * numquam sanatio; parcum …AR15) */
} FabricaEventus;

structura FabricaSanatio {
     constans FabricaActio* actio;
            FabricaEventus  eventus;
                    chorda  causa;
    /* CUR ACTA (fabrica-plan-5 T1): causa iudicii ante actum (exitus
     * stalus, transitus mutatus: 'lectio transitus mutata: <via>');
     * vacua = non acta aut causa ignota */
                    chorda stalum;
                       i32 duratio_ms;   /* siccum: AESTIMATIO ex
                                           * cursu ultimo (si notum) */
                       b32 tempus_notum; /* FALSUM: siccum sine
                                           * cursu priore (1b T7) */
};

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


/* ==================================================
 * GRADUS (fabrica-6 T5): registrum, areae, ambitus, explicatio
 * ================================================== */

/* REGISTRUM GRADUUM (ut generum T1): probationes_c (T6c) - genus
 * ludicrum probationis T5 in probatione ipsa vivit (actio->gradus ad
 * tabulam localem), non registratur. */
i32
fabrica_graduum_numerus (vacuum);

constans FabricaGradus*
fabrica_gradus_obtinere (
    i32 index);

constans FabricaGradus*
fabrica_gradus_invenire (
    chorda titulus);

/* build/fabrica/area/<actio>/<membrum>/ */
chorda
fabrica_area_via (
     chorda  actio,
     chorda  membrum,
    Piscina* piscina);

/* build/fabrica/acta/<titulus>.log (acta cursus actionis) */
chorda
fabrica_acta_via (
     chorda  titulus,
    Piscina* piscina);

/* SECTIONES (fabrica-7 T1): plagula 'via' (CREDO_VERDICTA - lineae
 * "SECTIO\t<titulus>\t<exitus>\t<praeteriti>\t<totales>\t<ms>\t
 * <fractura>" et "SUITA\t..."; aliae ignorantur) per sutura->legere ->
 * sectiones_out. Plagula absens aut linea SECTIO deformis -> FALSUM +
 * causa (linea nominata). synthetica = sectio unica 'totum'. */
b32
fabrica_sectiones_legere (
    constans FabricaSutura* sutura,
        constans character* via,
                   Piscina* piscina,
          FabricaSectiones* sectiones_out,
                    chorda* causa_out);

/* AMBITUS BASIS: PATH fixum (/usr/bin:/bin:/usr/sbin:/sbin - systema
 * solum, ~/.bin ingressus non declaratus esset), HOME (sutura->
 * ambitus), TMPDIR = <radix>/<area>tmp, RHUBARB_RADIX = sutura->radix;
 * deinde declarata actionis (valor per sutura->ambitus; absens = non
 * positum). Xar de chorda "N=V" ordinata per nomen. */
Xar*
fabrica_ambitum_basis (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
        constans character* area,
                   Piscina* piscina);

/* actiones (valore) -> eaedem + actiones syntheticae membrorum (post
 * parentem, ordine nominum). Parens manet (compositum membrorum T7).
 * NIHIL + causa: membra recusata (sedes parentis nominata), id duplex.
 * Actio sine gradu immutata transit. */
Xar*
fabrica_gradus_explicare (
    constans FabricaSutura* sutura,
              constans Xar* actiones,   /* FabricaActio (valore) */
                   Piscina* piscina,
                    chorda* causa_out);

/* ORPHANA: areae sub build/fabrica/area/ sine actione gradus aut sine
 * membro hodierno (Xar de chorda, viae cum '/' finali) - nuntiantur,
 * numquam deletae (ut build/aedilis). actiones = post explicationem. */
Xar*
fabrica_areas_orphanas (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                   Piscina* piscina);

/* COMPOSITA GRADUUM (fabrica-6 T7): pro quaque actione gradus
 * compositum eiusdem tituli, partes = membra (FABRICA_PARS_ACTIO),
 * ordine membrorum. Xar de FabricaCompositum (actiones = post
 * explicationem). */
Xar*
fabrica_gradus_composita (
    constans Xar* actiones,
         Piscina* piscina);

/* VERDICTUM COMPOSITI (spec 6 par. V): "<titulus>: N/M" (N partes
 * RECENS ex M), et si N < M " - non recentia: " + nominata (III,
 * deinde "+K"); verdictum membri gradus nominatur id suo
 * ('actio/membrum'), cetera artificio. Signum nullum, grep nullum. */
chorda
fabrica_compositum_verdictum (
    constans Xar* iudicia,     /* FabricaIudicium partium */
          chorda  titulus,
         Piscina* piscina);

/* RECUSATIO AMBITUS: nomina lectionum E (FabricaLectio, genus
 * LECTIO_AMBITUS, via = nomen) quae nec in basi nec declarata sunt -
 * Xar de chorda sine duplicibus; vacua = sanum. Membrum cuius cursus
 * talem legit FRACTUM: 'ambitus non declaratus: X'. */
Xar*
fabrica_ambitum_non_declaratum (
    constans FabricaActio* actio,
             constans Xar* lectiones,
                  Piscina* piscina);

#endif /* FABRICA_H */
