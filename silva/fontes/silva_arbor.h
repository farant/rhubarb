/* silva_arbor.h - vocabularium dialecti 'arbor' (STML canonicum)
 *
 * Arbor proicit arbores parsurae silvae in STML canonicum et eas
 * relegit. HOC caput stratum VOCABULARII solum fert (T2): sigillum
 * registri, quaesitiones nominum, tabulam orthographiae, mangulationem
 * tagorum. Scriptor (T3), comparator (T4), lector (T5) sequuntur.
 *
 * Consilium: project-specs/arbor-stml-spec-v2.md + arbor-stml-plan.md.
 *
 * CHARTA VOCABULARII (spec §2, contra registrum coctum mensurata):
 *   - genus nodi   -> tag elementi VERBATIM ex SilvaTabGenus.titulus
 *   - locus        -> tag elementi VERBATIM ex SilvaTabLocus.titulus
 *                     (nomina locorum NON globaliter unica sunt: LXII
 *                      nomina per CLXXVI ordines - locus significat
 *                      solum relative ad genus parentis)
 *   - genus lexematis -> tag PRAEFIXATUM 'lex-'
 *
 * CUR PRAEFIXUM: spatia nominum NON disiuncta sunt. 'assignatio' est
 * et genus nodi et SILVA_LEX_ASSIGNATIO; 'corpus' est et genus et
 * nomen loci. Praeterea genera lexematum in vocabulario communi
 * NUMQUAM fuerunt - machina quaestionis genera lexematum attingere
 * nequit (valorem TEXTUS comparat, non genus), et NOMINA_GENERUM
 * tabula separata est numquam in SilvaRegistrumCoctum nexa. Praefixum
 * ergo limitem spatii nominum DICIT, non celat.
 */

#ifndef SILVA_ARBOR_H
#define SILVA_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_expandere.h"
#include "silva_parsare.h"   /* SilvaParsura - superficies parsurae (M2) */

/* Praefixum tagorum lexematum (vide caput pro ratione) */
#define SILVA_ARBOR_PRAEFIXUM "lex-"

/* Capacitas buffer cuivis tago lexematis sufficiens: praefixum (IV)
 * + nomen longissimum ('SINISTRORSUM_ASSIGNATIO', XXIII) + terminator.
 * Rotundatum ad LXIV - probatio limitem verum custodit. */
#define SILVA_ARBOR_TAG_CAPACITAS 64

/* Longitudo sigilli in characteribus (nibbles hexadecimales) */
#define SILVA_ARBOR_SIGILLI_LONGITUDO 8


/* ==================================================
 * Sigillum registri
 * ================================================== */

/* Sigillum registri grammatici: friatio FNV-1a super genera + locos,
 * reddita ut chorda hexadecimalis minuscula VIII characterum
 * (semper VIII - zephyris ante-implita, ut sigilla oculo conferri
 * possint).
 *
 * TOTALE, non exemplar: SilvaRegistrumCoctum quattuor campos exacte
 * fert (duae series + duo numeri), ergo genera[] et loci[] ambulata
 * TOTAM structuram tegunt - nullus est status occultus quem sigillum
 * praeterire possit. Nullus campus versionis usquam exstat: CONTENTUM
 * tabularum solum signum versionis est, quod est prorsus quod
 * friandum volumus.
 *
 * Munus: documentum arboris sigillum fert; lector CONFERT et REICIT
 * si divergit. Arbor iudicata vocabulario falso mendacium est.
 *
 * XXXII bita sufficiunt quia comparatio UNA fit (sigillum documenti
 * contra sigillum registri currentis), ergo acceptio falsa 2^-32
 * constat - non terminus natalicius, qui comparationi binis inter
 * copiam applicaretur.
 *
 * Piscina/tabularium NIHIL -> chorda vacua (mensura ZEPHYRUM). */
chorda
silva_arbor_sigillum (
                           Piscina* piscina,
     constans SilvaRegistrumCoctum* tabularium);


/* ==================================================
 * Quaesitiones nominum registri
 * ================================================== */

/* Nomen generis nodi -> index in tabularium->genera; -I si ignotum.
 * Titulus NON terminatur nullo (mensura data) - chordae documenti
 * ita veniunt. */
s32
silva_arbor_genus_index (
     constans SilvaRegistrumCoctum* tabularium,
                constans character* titulus,
                               i32  mensura);

/* Nomen loci INTRA genus datum -> index ABSOLUTUS in seriem planam
 * tabularium->loci; -I si ignotum aut genus_index extra fines.
 *
 * Scopus generis essentialis est: nomina locorum non globaliter
 * unica sunt (vide caput). Index absolutus redditur ut vocans
 * speciem statim legere possit: tabularium->loci[index].species. */
s32
silva_arbor_locus_index (
     constans SilvaRegistrumCoctum* tabularium,
                               s32  genus_index,
                constans character* titulus,
                               i32  mensura);


/* ==================================================
 * Orthographia lexematum
 * ================================================== */

/* Orthographia FIXA generis lexematis ('auto', '[', '->', ...);
 * NIHIL si genus orthographiam VARIAM fert (identificator, litterae,
 * trivia, genera robustitatis) aut si genus extra fines est.
 *
 * ARTIFICIUM NOVUM, PORTA OBLIGATA. Silva tabulam INVERSAM
 * (genus -> littera) nusquam habet: verba clausa tabulam ANTRORSUM
 * solam ferunt (VERBA_CLAUSA, silva_lexema.c) et interpunctiones
 * cascadam si-alioquin recognoscentem solam (_legere_interpunctionem).
 * Haec ergo fons veritatis SECUNDUS est, qui a lexatore divergere
 * potest. Porta ad nativitatem: probatio orthographiam CUIUSQUE
 * introitus lexat et genus redditum exspectat.
 *
 * Silva trigraphos NON implet, ergo orthographiae hodie vere I:I
 * sunt. Si trigraphi umquam addantur tabula I:I esse desinit et
 * valor portandus fit - dependentia nominata. */
constans character*
silva_arbor_orthographia (
    SilvaLexemaGenus genus);

/* An valor lexematis in documento PORTANDUS sit (orthographia varia).
 * FALSUM pro generibus orthographiae fixae ET pro EOF, cuius valor
 * semper vacuus est. */
b32
silva_arbor_valor_portandus (
    SilvaLexemaGenus genus);


/* ==================================================
 * Mangulatio tagorum lexematum
 * ================================================== */

/* Tag lexematis in buffer datum scribere: praefixum 'lex-' + nomen
 * generis minusculum, '_' in '-' mutatis. Nullo terminatur.
 * Reddit longitudinem SCRIPTAM (sine terminatore), aut ZEPHYRUM si
 * genus extra fines est aut capacitas non sufficit.
 *
 * Nomen ex silva_lexema_genus_nomen sumitur - NON tabula propria.
 * Tabula propria quartus fons veritatis esset. */
i32
silva_arbor_lexema_tag (
    SilvaLexemaGenus  genus,
           character* buffer,
                 i32  capacitas);

/* Tag ('lex-...') -> genus lexematis; SILVA_LEX_NUMERUS_GENERUM si
 * ignotum. Titulus NON terminatur nullo (mensura data).
 *
 * Implementum per mangulationem ANTRORSAM cuiusque generis et
 * comparationem - ergo directiones duae DIVERGERE NON POSSUNT. */
SilvaLexemaGenus
silva_arbor_lexema_ex_tag (
     constans character* tag,
                    i32  mensura);


/* ==================================================
 * Scriptor: arbor -> STML canonicum (T3)
 * ================================================== */

/* Tag involucri triviorum ante/post lexema */
#define SILVA_ARBOR_TAG_ANTE      "ante"
#define SILVA_ARBOR_TAG_POST      "post"
#define SILVA_ARBOR_TAG_SCISSURA  "scissura"
#define SILVA_ARBOR_TAG_ENVOLUCRI "arbor"

/* Fructus scripturae - forma SilvaScriptura aequata (fractura clara:
 * successus FALSUM + causa STATICA + sedes, numquam praetermissio
 * tacita). */
nomen structura {
                     b32  successus;
                  chorda  textus;  /* octeti STML; vacua in fractura */
      constans character* causa;   /* diagnosticum staticum; NIHIL si bene */
     constans SilvaNodus* sedes;   /* nodus fracturae; NIHIL licet */
} SilvaArborScriptura;

/* Subarborem in documentum STML canonicum scribere.
 *
 * grammatica: nomen grammaticae in involucrum scriptum (e.g. "c89").
 *   PARAMETRUM EST, non derivatum: registrum coctum nomen SUUM non
 *   fert (nullus campus versionis aut identitatis in tabulis - vide
 *   silva_arbor_sigillum). Sigillum grammaticas cryptographice
 *   distinguit; hoc nomen homini est.
 *
 * expansio: NECESSARIA, non optionalis - tabula fontium in
 *   SilvaExpansio.fontes vivit et nodus nudus fons_index solvere
 *   nequit. NIHIL licet solum si nullum lexema fontem nominat.
 *
 * intern: NIHIL licet -> per internamentum_creare pigre creatur.
 *
 * QUAE SCRIBUNTUR: genus nodi = tag; locus = involucrum (NIHIL
 * omissus); lexema = tag praefixatum 'lex-'; valor ut TEXTUS et
 * solum generibus orthographiae variae; trivia in <ante>/<post> per
 * lentes compactas; scissurae ut <scissura offset="N"/>; 'standard'
 * et 'f' solum cum non-ordinaria.
 *
 * QUAE NON SCRIBUNTUR: positiones (b/linea/columna). Involucrum
 * ANCORAM solam fert; cetera ambulatione emissionis derivantur.
 * Lex: documentum canonicum MENTIRI non possit - positio portata
 * cum contento dissentire potest, derivata non potest.
 *
 * AMBIGUUS: arbor a silva_scribere CONSULTO divergit. Scribere solam
 * interpretationem canonicam emittit (octeti unam lectionem solam
 * scribere possunt); arbor OMNES emittit, quia hoc est quod circuitus
 * octetorum structuraliter servare non potest. Nulla causa specialis
 * in codice: ambulatio generica listam 'interpretationes' totam
 * emittit, et hoc IPSUM est divergentia.
 *
 * COMMUNICATIO LEXEMATUM: bracchia ambigua lexemata EADEM communicant
 * (identitas res est - duplicatio mentiretur). Usus primus fragmentum
 * nominatum scribit (<#lexN>), usus sequentes transclusionem
 * (<<#lexN>>). Duo passus: primus usus numerat, secundus emittit -
 * aliter usus primus scire non posset an fragmentum opus esset.
 *
 * REFUTAT CLARE: lexema non-FONS (limes expansionis, sicut scribere),
 * genus ignotum, forma corrupta, elementum listae mixtae illicitum,
 * valor textui non tutus. */
SilvaArborScriptura
silva_arbor_scribere_nodum (
                          Piscina* piscina,
              constans SilvaNodus* nodus,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
           constans SilvaExpansio* expansio,
              InternamentumChorda* intern);


/* ==================================================
 * Comparator arborum (T4)
 *
 * API PUBLICA fontium, NON adiutor probationis: probationes silvae
 * communia NULLA habent per constructionem (quaeque unitas
 * translationis propria est), et consumptor proximus iam visibilis
 * est - portae mutationis id ipsum rogant, 'an haec transformatio
 * arborem servaverit'.
 * ================================================== */

/* Capacitas semitae diagnosticae (tags '>' iunctis) */
#define SILVA_ARBOR_VIA_CAPACITAS 256

/* MODUS: quid POSITIO significet pendet ab eo unde arbor venerit.
 *
 *   STRUCTURALIS - positiones IGNORANTUR. Modus mutationis: subarbor
 *     mota octetos suos alibi habet, et arbor tamen EADEM est. Sine
 *     hoc modo comparator consumptori quem planum nominat inutilis
 *     esset.
 *
 *   FIDELITAS - positiones CONFERUNTUR. Modus circuitus (T6 oraculum
 *     A): documentum positiones non fert, ergo lector eas ambulatione
 *     emissionis DERIVAT; nisi eas conferamus, derivatio tota
 *     inprobata manet.
 *
 * PROVENIENTIA tamen SEMPER confertur, utroque modo: an lexema
 * sedem VERAM habeat (byte_offset >= 0) an syntheticum sit. Temptatio
 * erat positiones conferre 'solum cum ambo lexemata sedem habeant' -
 * quae est prorsus custodia quae transit quia subiectum eius abest
 * (vitium derivationis omnia ad -I ponens taceret). Provenientia
 * factum STRUCTURALE de lexemate est, non ordinata; nullus modus eam
 * celare debet. */
nomen enumeratio {
    SILVA_ARBOR_COMPARATIO_STRUCTURALIS = 0,
    SILVA_ARBOR_COMPARATIO_FIDELITAS
} SilvaArborComparatioModus;

/* Divergentia PRIMA nominata. Booleanum solum in porta super LXXVIII
 * fixturas inutile est - 'inaequales' sine campo et sede bisectionem
 * manualem petit. */
nomen structura {
      constans character* campus;    /* nomen campi divergentis */
     constans SilvaNodus* nodus_a;   /* nodi continentes */
     constans SilvaNodus* nodus_b;
     constans SilvaToken* lexema_a;  /* NIHIL nisi divergentia lexicalis */
     constans SilvaToken* lexema_b;
                     s32  locus;     /* index loci; -I si non pertinet */
                     s32  index;     /* index in lista; -I si non pertinet */
               character  via[SILVA_ARBOR_VIA_CAPACITAS];
} SilvaArborDifferentia;

/* Arbores duas conferre. VERUM si aequales.
 *
 * differentia: NIHIL licet; alioquin in inaequalitate impletur
 * (in aequalitate campus = NIHIL ponitur).
 *
 * QUAE CONFERUNTUR: genus nodi, numerus locorum, per locum genus
 * valoris; listae per mensuram PROSPECTUS (numquam xar_numerus
 * repositorii - repositorium inter furcas GLR commune et
 * append-only est, ergo longitudo eius statum MACHINAE reddit, non
 * visum huius arboris); lexemata per genus, octetos valoris,
 * standard, fons_index, initium_lineae, provenientiam, seriem
 * triviorum (genus + valor, ORDINE, ambae listae), scissuras.
 *
 * PATER: nullitas sola confertur (a->pater NIHIL si et solum si
 * b->pater NIHIL). Comparatio monstratorum trans arbores duas nihil
 * significat, et custodia fortior - 'pater aequat nodum unde
 * descendimus' - super subarbores COMMUNICATAS frangitur, quia
 * committere ex visitatione ULTIMA ordine ambulationis assignat
 * (bis parcatum: phase-log:1356-1359, :5288-5295). Nullitas tamen
 * capit quod capiendum est: lectorem patrem omnino non fixisse.
 *
 * QUOD NON VIDET, CONSULTO: dominium GEMINUM triviorum. Si lexema
 * idem in duabus listis spatia_* apparet, ambo domini LOCALITER
 * recti videntur et comparatio structuralis nihil dicit. Hoc NON
 * est vitium hic sanandum - est ipsa ratio cur T6 oraculum SECUNDUM
 * habet (emissio octetorum, ubi id ut octeti GEMINI apparet). Noli
 * hunc comparatorem 'emendare' ut id capiat: oracula duo classes
 * defectuum DISIUNCTAS tegunt, et in unum collapsa alteram
 * perderent. */
b32
silva_arbor_aequalis (
          constans SilvaNodus* a,
          constans SilvaNodus* b,
    SilvaArborComparatioModus  modus,
        SilvaArborDifferentia* differentia);


/* ==================================================
 * Lector: STML canonicum -> arbor (T5a)
 *
 * ASYMMETRIA cum scriptore, consulta: scriptor arborem a parsatore
 * structam accipit, ergo bene-formatam praesumere potest et ad
 * limitem expansionis solum recusat. Lector octetos QUOSLIBET
 * accipit et OMNIA statuere debet. Pars maior huius stratum
 * VALIDATIONIS est, non constructionis.
 * ================================================== */

/* Vitium lectionis: causa STATICA + LINEA documenti. Linea non
 * ornamentum est - vitium sine linea in documento magno venatio
 * est, non diagnosticum (StmlNodus.linea ob hoc ipsum exstat). */
nomen structura {
     constans character* causa;   /* NIHIL si bene */
                    i32  linea;   /* 1-basata; ZEPHYRUM si ignota */
} SilvaArborVitium;

/* Documentum arboris in arborem silvae relegere.
 *
 * VALIDAT ANTE CONSTRUCTIONEM: involucrum adest; 'grammatica'
 * congruit; SIGILLUM congruit aut RECUSAT (arbor vocabulario falso
 * iudicata mendacium est); genus et locus quisque registro noti;
 * species loci contento congruit; valor adest si et solum si genus
 * orthographiam variam fert.
 *
 * CONSTRUIT PER SEMITAM CUSTODITAM: silva_nodus_creare +
 * silva_nodus_ponere (S32 - species probata, semel-tantum scribere),
 * ergo custodia constructionis gratis. SED lector elementa LISTAE
 * MIXTAE ipse custodire DEBET: silva_nodus_appendere speciem loci
 * solam probat, numquam quid intus eat.
 *
 * VALOR VERBATIM per stml_textus_internus - NUMQUAM
 * stml_textus_normalizatus, quod transformat.
 *
 * QUOD T5a NONDUM FACIT (T5b): positiones/pater/longitudinem
 * derivare, et transclusiones resolvere. Ergo lexemata SYNTHETICA
 * redeunt (byte_offset -I) et documentum transclusionem ferens
 * CLARE RECUSATUR. Arbor sine sedibus hoc modo legitima est - id
 * est prorsus casus arboris AUCTORATAE, quae fontem non habet.
 *
 * NB comparator talem arborem contra parsatam AEQUALEM NON dicet,
 * ne modo structurali quidem, quia PROVENIENTIA utroque modo
 * confertur. Hoc rectum est: T5a circuitum plenum simulare NON
 * potest, et custodia id DICIT potius quam celet.
 *
 * Reddit NIHIL + vitium nominatum in recusatione. */
SilvaNodus*
silva_arbor_legere (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
     constans SilvaRegistrumCoctum* tabularium,
                constans character* grammatica,
                  SilvaArborVitium* vitium);


/* ==================================================
 * Parsura: plagula INTEGRA <-> STML canonicum (M2 §2)
 *
 * Documentum <parsura> proiectio PLAGULAE est, non parsurae:
 * fert CLAUSURAM EMISSIONIS - id est prorsus quod
 * silva_scribere_fontem legit - et nihil aliud. Sex res: arborem
 * nodorum, directivas, arborem regionum (rami non sumpti),
 * trivia caudae (lexema EOF), tabulam fontium, extenta.
 *
 * QUAE EXCLUDUNTUR ET CUR: strata, acta, numeratores, vexilla
 * salutis (est_intermissa, fines_tactae, numerus_errorum, ...).
 * Ratio non est 'derivabilia sunt' sed FORTIOR: STML forma
 * MUTABILIS est editionibus tractabilis, et strata EXITUS
 * expansionis sunt - ergo stratum mutare incohaerens est.
 * Documentum quod plagulam EDITAM iuxta stratum NON EDITUM ferre
 * posset MENTIRI posset. Eadem lex quae M1 sedes derivare fecit.
 * <strata> nomen RESERVATUM manet, sicut 'origo=' in M1.
 *
 * NULLA ANCORA: subarbor ancoram fert quia quid ante eam stet
 * scire nequit. Plagula ipsa INITIUM est - ergo sedes omnes ab
 * offset ZEPHYRUM, linea I derivantur.
 *
 * Consilium: project-specs/arbor-parsura-spec.md.
 * ================================================== */

/* Tags sectionum documenti parsurae */
#define SILVA_ARBOR_TAG_PARSURA "parsura"
#define SILVA_ARBOR_TAG_FONTES  "fontes"
#define SILVA_ARBOR_TAG_FONS    "fons"
#define SILVA_ARBOR_TAG_CAUDA   "cauda"
#define SILVA_ARBOR_TAG_DIRECTIVA "directiva"
#define SILVA_ARBOR_TAG_REGIO_DIRECTIVA "regio-directiva"
#define SILVA_ARBOR_TAG_REGIO_CRUDA     "regio-cruda"
#define SILVA_ARBOR_TAG_REGIO_FINIS     "regio-finis"

/* Plagulam integram ex parsura in documentum <parsura> scribere.
 *
 * fons_index: cuius plagulae octeti petuntur (parsura plures fert).
 *   Plerumque parsura->fons_princeps. -I = quaelibet.
 *
 * grammatica: ut in silva_arbor_scribere_nodum - PARAMETRUM, quia
 *   registrum nomen suum non fert.
 *
 * intern: NIHIL licet -> pigre creatur.
 *
 * UNUS scriptor totum documentum scribit, CONSULTO: numeratio
 * fragmentorum (<#lexN>) DOCUMENTO-scopata est, et
 * silva_arbor_scribere_nodum per nodum vocatum numeratorem suum ad
 * zephyrum omni vocamine reponeret - ergo duo nodi supremi ambo
 * '<#lex1>' emitterent et documentum identitates GEMINAS ferret.
 *
 * REFUTAT CLARE, ut scriptor nodorum. */
SilvaArborScriptura
silva_arbor_scribere_parsuram (
                          Piscina* piscina,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
                              s32  fons_index,
              InternamentumChorda* intern);

/* Documentum <parsura> in parsuram silvae relegere.
 *
 * VALIDAT ANTE CONSTRUCTIONEM ut lector nodorum: involucrum
 * <parsura>; 'grammatica' congruit; 'registrum-sigillum' congruit
 * aut RECUSAT.
 *
 * REFICIT: SilvaExpansio novam cum tabula fontium (scriptor eam
 * REPETIT - silva_arbor_scribere_nodum expansionem non-NIHIL
 * postulat ut fons_index solvat, et silva_scribere_fontem
 * expansio->regiones legit); commissionem per silva_committere;
 * lexema_finis; fons_princeps ex involucro.
 *
 * CAMPI EXCLUSI (vide caput sectionis) ZEPHYRO EXPLICITE
 * ponuntur - numquam non-initializati relinquuntur.
 *
 * Reddit NIHIL + vitium nominatum in recusatione. */
/* Parsuras duas conferre. VERUM si aequales.
 *
 * MUNUS: DIAGNOSIS, non verdictum. Verdictum portae M2 aequalitas
 * OCTETORUM contra plagulam in disco est (valor expectatus EXTRA
 * silvam - vitium symmetricum inter scriptorem et lectorem in eo
 * latere nequit). Cum ille cadit, HAEC functio dicit QUIS CAMPUS
 * divergat - quod in M1 omnem divergentiam uno cursu solvit, ter,
 * post ter falsam divinationem.
 *
 * QUAE CONFERUNTUR: radix (per nodum supremum, delegando), lineae
 * directivarum, arbor regionum (rami, laminae crudae, linea finis,
 * filiae), lexema caudae cum triviis.
 *
 * QUAE NON, ET CUR:
 *   est_sumptum  - STRUCTURALE est (ramus sine lamina cruda sumptus
 *                  est), ergo comparatio eius rem bis diceret; et
 *                  VISIO tenet viam defaltam unam inter multas esse.
 *   expressio    - documentum eam NON fert (octeti eius in
 *                  'directiva' iam sunt; _regiones_colligere eam
 *                  numquam reinserit). Lacuna REPRAESENTATIONALIS
 *                  nominata, non tacita - recensio T5 eam numerabit.
 */
b32
silva_arbor_parsurae_aequales (
          constans SilvaParsura* a,
          constans SilvaParsura* b,
      SilvaArborComparatioModus  modus,
          SilvaArborDifferentia* differentia);

SilvaParsura*
silva_arbor_legere_parsuram (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
     constans SilvaRegistrumCoctum* tabularium,
                constans character* grammatica,
                  SilvaArborVitium* vitium);

#endif /* SILVA_ARBOR_H */
