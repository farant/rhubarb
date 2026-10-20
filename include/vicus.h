/* vicus.h - hospes applicationum: repositorium unum, tabulae,
 * montationes
 * (insula-rami-plan T1b)
 *
 * Vicus (via Romana insularum) repositorium UNUM possidet: radix
 * <vicus> (apps/vicus/organum.stml), liberi = montationes applicationum
 * (rami, canonibus suis iudicati). Volumen UNUM: documenta montationum
 * in spatiis suis (id), dispositio in plagula 'vicus/latera'
 * (configuratio, non historia: superscribitur). Vicus nullam
 * applicationem novit: principale genera registrat (titulus ->
 * magnitudo montationis + functio montandi).
 *
 * vicus-latera S2a: tabula = par laterum. SINISTRUM = editor (genus non
 * fixum), DEXTRUM = acervus cuius ultimus in fronte. Latus montatio
 * una est; id eius = "<tabula>_<latus>_<genus>" = id rami; spatium
 * arboris (actiones, figurae, ids intra resolvuntur) = via rami
 * "<genus>:<id>" (status-plan S8c). Ictus in latus non focatum id
 * focat (ictus primus solum focat).
 *
 * Genus ignotum in indice (volumen ab hospite recentiore): tabula
 * servatur, non montatur, causa nominatur. Addere generis ignoti
 * recusatur. */

#ifndef VICUS_H
#define VICUS_H

/* <aedilis corpus="lib/vicus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "organum_montatio.h"
#include "semita.h"

/* tabulae fixae (vicus-latera decisio IX): index ordinarius tot
 * dat; vicus plures non legit */
#define VICUS_TABULAE    X
#define VICUS_SINISTRUM  ZEPHYRUM
#define VICUS_DEXTRUM    I

/* linea tabularum (T2b): una, cellula domus VI x VIII */
#define VICUS_CELLULA_LATITUDO  VI
#define VICUS_CELLULA_ALTITUDO  VIII
#define VICUS_ALTITUDO_TABULARUM VICUS_CELLULA_ALTITUDO

/* montatio applicationis in sedem (magnitudinis registratae); ctx =
 * contextus generis (vicus_genus_addere; vicus-latera S2b: liber
 * paginarum scribae communis). argumentum (S3c): '$genus(argumentum)'
 * quo latus apertum est; NIHIL = nullum */
nomen b32 (*VicusMontator)(
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx);

/* gestum applicationis in Motum ponere (T3a) - e.g.
 * scriba_gestum_ponere */
nomen vacuum (*VicusGestor)(
     Motus* motus,
    vacuum* ctx);

/* pulsus montationis vivae (vicus-latera S1c): mutatum = quadrum
 * pingendum (si activa); finitus = vita exiit (concha), tabula
 * manet sed non iam pulsatur */
nomen structura {
    b32 mutatum;
    b32 finitus;
} VicusPulsus;

nomen VicusPulsus (*VicusPulsator)(
    vacuum* ctx);

/* facies montationis (T2a): quod hospes ab applicatione activa
 * accipit - registra, componere, fons imaginum, gestus (T3a), pulsus
 * (S1c). Ante describere tota nullatur: campi omissi = absentes. */
nomen structura {
      ActioRegistrum* actiones;
     FiguraRegistrum* figurae;
           Componere  componere;
              vacuum* componere_ctx;
           ImagoFons  fons;           /* NIHIL = nullae imagines */
              vacuum* fons_ctx;
         VicusGestor  gestum_ponere;  /* NIHIL = nullus gestus */
              vacuum* gestum_ctx;
       VicusPulsator  pulsare;        /* NIHIL = non vivit */
              vacuum* pulsare_ctx;
                 b32  vivit_in_fundo; /* pulsatur etiam non activa */
                 b32  focus_in_apertura; /* latus hoc focum capit cum
                                          * vicus aperitur (terminale:
                                          * programma statim scribendum;
                                          * Franus 2026-10-09) */
    /* ictus qui latus focat etiam agit, si latus vult (Franus: iussum
     * uno ictu). Post focum mutatum vocatur (motus iam lateri
     * aptatus); arbor = arbor composita. VERUM = actum. NIHIL = ictus
     * primus focat solum. */
                 b32  (*ictus_primus)(vacuum* ctx,
                          InsulaRepositorium* repo, Motus* motus,
                          Componens* arbor, constans Eventus* ev);
              vacuum* ictus_primus_ctx;
    /* T4d (colloquia-plan): Esc non consumptum in latere frontali
     * acervi latus tollit (vicus_acervo_redire) - latera sine
     * processu (admonitiones, colloquia); ordinarie FALSUM */
                 b32 effugium_redit;
    /* S3e: argumentum lateris mutatum (vicus_acervo_mutare): latus se
     * aptat (pictor: picturam aliam ostendit). NIHIL = non potest */
                 b32  (*argumentum_ponere)(vacuum* ctx,
                          constans character* argumentum);
              vacuum* argumentum_ponere_ctx;
    /* semitae R3: ramus lateris (semita.h) - historia lateris et
     * $via; NIHIL = latus sine semitis */
    constans InsulaRamus* ramus;
} VicusFacies;

nomen vacuum (*VicusDescriptor)(
         vacuum* montatio,
    VicusFacies* facies);

nomen structura {
             chorda  titulus;
     memoriae_index  mensura;     /* montationis (sedes) */
      VicusMontator  montare;
    VicusDescriptor  describere;
             vacuum* ctx;        /* montatori datur (S2b) */
} VicusGenus;

/* latus (S2a): montatio una. id = "<tabula>_<latus>_<genus>"
 * ("3_sinistrum_scriba"; canon 'nomen' puncta non admittit) = id rami
 * repositorii; spatium arboris = via rami ("scriba:3_sinistrum_scriba",
 * status-plan S8c) */
nomen structura {
                 chorda id;
                 chorda genus;
                 chorda argumentum;   /* S3c: identitas = genus +
                                        * argumentum; vacuum = nullum */
    constans VicusGenus* descriptio;   /* NIHIL = genus ignotum */
                 vacuum* montatio;       /* sedes montationis */
                    b32  montata;
            VicusFacies  facies;         /* si montata */
                    b32  finita;         /* concha exiit: titulus
                                          * "[exitus]", non iam
                                          * pulsatur */
    /* semitae R3 (vicus privatum): historia lateris - semitae plenae
     * ut textus STML (cum signo camporum 'impelle'); cursor = locus
     * currens */
                    Xar* historia;
                    i32  historia_cursor;
} VicusLatus;

nomen structura {
        chorda  id;          /* "1".."10" */
    VicusLatus  sinistrum;
           Xar* acervus;     /* Xar de VicusLatus (dextrum); ultimus =
                              * in fronte; numquam vacuus (decisio
                              * VIII) */
           i32 focus;       /* VICUS_SINISTRUM aut VICUS_DEXTRUM */
           i32 focus_domus; /* T4f: focus cum latus cycli domum texit
                             * (ordinarium VICUS_DEXTRUM) */
} VicusTabula;

/* S4 (Franus 2026-10-09): horologium in linea tabularum, ad dextrum
 * ("7:52 PM"); ictus in eo diem ostendit ("Friday, October 9th,
 * 2026"), iterum horam (Franus 2026-10-10). NIHIL (ordinarium) =
 * nullum horologium - probationes et specimina constant; applicationes
 * vicus_horologium_locale ponunt. vicus_pulsare tempus legit et VERUM
 * reddit cum mutatur (minutum aut dies). S7a: lectio in ramo radicis
 * vivit (tempus_*, pila_*; absens = nulla), systema 'horologium' eam
 * scribit; horologium_dies ibidem (vicus.radix). */

/* S4: tempus horologii (locale) */
nomen structura {
    s32 annus;            /* 2026 */
    s32 mensis;           /* 1-12 */
    s32 dies;             /* 1-31 */
    s32 dies_hebdomadis;  /* 0 dominica - 6 sabbatum */
    s32 hora;             /* 0-23 */
    s32 minutum;          /* 0-59 */
} VicusTempus;

nomen vacuum (*VicusHorologium) (vacuum* ctx, VicusTempus* tempus);

/* S5: pila in linea tabularum, ad dextrum horologii ("87%", "87%+"
 * cum oneratur). NIHIL = nulla (probationes); applicationes portam
 * per energia_status ponunt. Legitur cum horologio (minuto). */
nomen b32 (*VicusPila) (vacuum* ctx, s32* centesimae, b32* oneratur);

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
                    i32  latitudo;
                    i32  altitudo;
     InsulaRepositorium* repo;
                    Xar* genera;      /* Xar de VicusGenus */
                    Xar* tabulae;     /* Xar de VicusTabula */
                 chorda  activa;      /* copia indicis (vicus/latera) */
                 chorda  causa;
         ActioRegistrum* actiones;    /* hospitis + activae (T2a) */
        FiguraRegistrum* figurae;
                  Motus* motus;       /* ligatus (T3a); NIHIL nullus */
                    Xar* petitiones;  /* S3c: aperitiones in acervo
                                       * pendentes (vicus_pulsare) */
        VicusHorologium  horologium;  /* S4: NIHIL = nullum */
                 vacuum* horologium_ctx;
              VicusPila  pila;        /* S5: NIHIL = nulla */
                 vacuum* pila_ctx;
        StatusSystemata* systemata;   /* S7b: NIHIL = nulla */
                    /* T4a: vicus_systema_addere (privatum) */
                    Xar* systemata_hospitis;
             VicusLatus* stratum;     /* T4b: NIHIL = nullum */
                    Xar* cyclus;      /* T4f: latera cycli (genus,
                                       * argumentum; privatum); NIHIL
                                       * aut vacuus = nullus */
          SemitaOrganum  organa;      /* semitae R3: NIHIL = nullae */
                 vacuum* organa_ctx;
                Piscina* scheda_semitarum;   /* per pulsum vacatur */
} Vicus;

/* T4a (colloquia-plan): systema hospitis - in omni vicus_pulsare
 * vocatur (post horologium); campos radicis per vicus_radicem_scribere
 * sub titulo suo scribit (actio systema="verum" in manifesto radicis).
 * VERUM = mutatum (quadrum pingendum). */
nomen b32 (*VicusSystema)(Vicus* v, vacuum* ctx);

Vicus*
vicus_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo);

b32
vicus_genus_addere (
                  Vicus* v,
     constans character* titulus,
         memoriae_index  mensura,
          VicusMontator  montare,
        VicusDescriptor  describere,
                 vacuum* ctx);

/* T4b (colloquia-plan): stratum supra latera - montatio una generis
 * registrati (ramus "<genus>", id "stratum_<genus>"), in OMNI tabula
 * post latera composita (supra pingitur, ictus prius accipit) et
 * semper pulsata; superficies tota (linea tabularum inclusa). Arbor in
 * coordinatis superficiei; fines radicis stratum non tegunt: ictus
 * extra liberos ad latera transeunt (radix 0 x 0). Registra in spatio
 * rami miscentur. Numquam focum capit. FALSUM si genus ignotum aut iam
 * stratum. Insigne (T4b): campus radicis admonitiones_non_lectae > 0
 * -> signum et numerus sinistrorsum horologii; ictus latus
 * 'admonitiones' in acervo aperit. */
b32
vicus_stratum_addere (
                  Vicus* v,
     constans character* genus);

/* T4a: titulus iteratus, systema aut titulus NIHIL: FALSUM */
b32
vicus_systema_addere (
                  Vicus* v,
     constans character* titulus,
           VicusSystema  systema,
                 vacuum* ctx);

/* campum radicis scribere scriptore dato (valor NIHIL = tollere);
 * FALSUM si recusatum (domini) */
b32
vicus_radicem_scribere (
                  Vicus* v,
     constans character* scriptor,
     constans character* titulus,
     constans character* valor);

/* S7b: systemata status (organum_montatio.h) - vicus_aperire ea creat
 * (conservatio, relatio, dimissio) post repositorium et radicem cum eis
 * parat (clavis "vicus"); latera eadem a hospite accipiunt. ANTE
 * vicus_aperire vocandum; NIHIL (ordinarium) = nulla (probationes). */
vacuum
vicus_systemata_ponere (
               Vicus* v,
     StatusSystemata* systemata);

/* repositorium creatur, dispositio e volumine legitur (absens:
 * ordinaria, quae scribitur), latera montantur. Forma:
 * <tabulae activa="1"><tabula id="1" focus="sinistrum"><latus
 * genus="scriba"/><acervus><latus genus="terminale"/></acervus>
 * </tabula>...</tabulae>; acervus absens = latus sinistrum iteratum.
 * S3c: latus 'id' et 'argumentum' optionalia (absens id = ordinarium
 * "<tabula>_<latus>_<genus>").
 * FALSUM si dispositio mala aut repositorium deficit; latus generis
 * ignoti praeteritur (causa). S7a: radix ex manifesto infixo (genus
 * vicus) paratur (canon, domini). */
b32
vicus_aperire (
                  Vicus* v,
     constans character* index_ordinarius);

i32
vicus_numerus_tabularum (
    constans Vicus* v);

VicusTabula*
vicus_tabula (
    constans Vicus* v,
               i32  index);

/* tabula activa; NIHIL si nulla */
VicusTabula*
vicus_activa (
    constans Vicus* v);

/* latus tabulae: VICUS_SINISTRUM aut VICUS_DEXTRUM (= frons
 * acervi); NIHIL si tabula NIHIL */
VicusLatus*
vicus_latus (
    VicusTabula* t,
            i32  latus);

/* latus focatum tabulae activae; NIHIL si nulla */
VicusLatus*
vicus_latus_focatum (
    constans Vicus* v);

/* focum tabulae activae ad latus ponere (ictus idem facit): Motus
 * ramum, spatium, gestum sequitur; pendentia relinquentis effunduntur
 * ut in commutatione tabularum. Dispositio servatur. */
b32
vicus_focum_ponere (
    Vicus* v,
      i32  latus);

/* T6 (colloquia-plan): argumentum lateris servatum mutare SINE
 * remontatione - montatio currit intacta (facies.argumentum_ponere
 * non vocatur); dispositio scribitur, restitutio proxima eo montat.
 * Identitas (genus + argumentum) sequitur: vicus_acervo_aperire cum
 * eodem argumento hoc latus invenit. argumentum NIHIL aut "" =
 * nullum. FALSUM si id ignotum. */
b32
vicus_latus_argumentum_ponere (
                  Vicus* v,
     constans character* id,
     constans character* argumentum);

/* T4c (colloquia-plan): latus per id ostendere: tabula eius activa,
 * in frontem acervi (si ibi), focus in eo; dispositio servatur.
 * FALSUM si id ignotum aut latus non montatum. */
b32
vicus_latus_focare (
                  Vicus* v,
     constans character* id);

/* activam ponere (insula et index servatus); FALSUM si id
 * ignotum */
b32
vicus_activam_ponere (
                  Vicus* v,
     constans character* id);

/* registra hospitis (T2a): dispensatori et glutino SEMEL dantur;
 * in aperire et commutatione vacantur et ex activa implentur -
 * indices numquam mutantur */
ActioRegistrum*
vicus_actiones (
    constans Vicus* v);

FiguraRegistrum*
vicus_figurae (
    constans Vicus* v);

/* ImagoFons compositus: ad fontem tabulae activae delegat (ctx =
 * Vicus*) */
constans Imago*
vicus_imago_fons (
     chorda  provenientia,
     vacuum* ctx);

/* Componere-formata (dispensator.h), ctx = Vicus*: radix (actio
 * 'vicus.radix'), linea tabularum (PARTES_INDEX, figura hospitis ex
 * Vicus legit), arbores laterum VISIBILIUM tabulae activae (componere
 * cuiusque) infra lineam translatae, sinistrum ad x 0, dextrum post
 * dimidium (cellulis rotundatum), radix cuiusque spatio lateris
 * signata; divisor post latera. Applicatio superficiem suam ex ramo
 * legit - hospes eam scribit (aperire, mutatio magnitudinis). */
Componens*
vicus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* Dispensatorem ligare (T3a, T3b) - post vicus_aperire, semel:
 * - Motus eius: ramus = ramus activae (focus, effusio pan/zoom),
 *   gestus = gestus activae. In commutatione: gestus relinquentis et
 *   pan/zoom in ramum RELINQUENTIS effunduntur (gestus non effusus
 *   commutationem recusat), captura et ictus pendens abiciuntur,
 *   deinde ramus et gestus advenientis ponuntur.
 * - destinatio hospitis: Cmd+1..9 / Cmd+0 ad radicem (tabula;
 *   applicatio eas numquam videt; Franus 2026-10-09 - olim praefixum
 *   Ctrl-A, quod nunc ad latus focatum it, tmux eo utitur); cetera
 *   geometrica. */
vacuum
vicus_dispensatorem_ligare (
          Vicus* v,
    Dispensator* d);

/* Pulsus unus (vicus-latera S1c, S2a): latera VISIBILIA tabulae
 * activae (sinistrum, frons acervi) semper, cetera (acervi, tabulae
 * aliae) solum si vivit_in_fundo (decisio VI: in fundo legitur, non
 * pingitur); finita numquam. VERUM si quadrum pingendum: latus
 * visibile mutatum, aut latus nunc finitum (titulus in linea
 * mutatur). Dispositio servata (vicus/latera) non tangitur. */
b32
vicus_pulsare (
    Vicus* v);

vacuum
vicus_horologium_ponere (
              Vicus* v,
    VicusHorologium  horologium,
             vacuum* ctx);

vacuum
vicus_pilam_ponere (
          Vicus* v,
      VicusPila  pila,
         vacuum* ctx);

/* hora locali systematis (time.h: time, localtime) */
vacuum
vicus_horologium_locale (
         vacuum* ctx,
    VicusTempus* tempus);

/* S3e: latus frontale generis in acervo tabulae activae argumentum
 * novum accipit (facies.argumentum_ponere) et in frontem venit; focus
 * manet; dispositio servatur. Nullum eius generis: latus montatur
 * (ut vicus_acervo_aperire), deinde facies.argumentum_ponere
 * vocatur. PETITIO (vicus_pulsare proximo). FALSUM si
 * genus ignotum. */
b32
vicus_acervo_mutare (
                 Vicus* v,
    constans character* genus,
    constans character* argumentum);

/* T4f (colloquia-plan): cyclus lateris dextri - Cmd+] (directio +I)
 * aut Cmd+[ (-I) in tabula activa: domus -> latus[0] -> ... ->
 * latus[n-1] -> domus. Latus cycli = genera[k] cum argumenta[k]
 * (identitas ut vicus_acervo_aperire, S3c; argumenta NIHIL aut
 * argumenta[k] NIHIL = sine argumento). Domus = latus summum acervi
 * quod in cyclo non est (nullum: imum). Gradus ad latus = ut
 * vicus_acervo_aperire (focus dexter); gradus ad domum = latera cycli
 * supra domum tolluntur (ut Esc), focus redit quo erat cum latus cycli
 * domum texit. Copiantur; numerus ZEPHYRUM = cyclus nullus. FALSUM si
 * genus ignotum. */
b32
vicus_cyclum_ponere (
                           Vicus* v,
    constans character* constans* genera,
    constans character* constans* argumenta,
                             i32  numerus);

/* gradus cycli (+I aut -I): PETITIO, applicatur in vicus_pulsare
 * proximo (ut vicus_acervo_aperire). FALSUM si cyclus nullus. */
b32
vicus_cyclo_gradi (
    Vicus* v,
      s32  directio);

/* T4d: latus frontale acervi tabulae activae tollitur, prior in
 * frontem venit, focus ad dextrum; dispositio servatur. FALSUM si
 * acervus unum solum habet (numquam vacuus, decisio VIII). */
b32
vicus_acervo_redire (
    Vicus* v);

/* S3c: latus (genus, argumentum) in acervo tabulae activae: iam
 * praesens in frontem venit, aliter montatur (id novum: id
 * ordinarium, deinde '_2', '_3'...) et frons fit; focus ad dextrum;
 * dispositio servatur (id et argumentum cuiusque lateris). PETITIO:
 * applicatur in vicus_pulsare proximo, numquam intra tractationem
 * eventus (iussum ex scriba inter tractationem currit). FALSUM si
 * genus ignotum. */
b32
vicus_acervo_aperire (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum);

/* semitae R3: organa (genus -> manifestum) pro semitis laterum;
 * applicatio ponit (organa infixa, cache). NIHIL = nullae (historia
 * nulla) */
vacuum
vicus_organa_ponere (
              Vicus* v,
      SemitaOrganum  organa,
             vacuum* ctx);

chorda
vicus_causa (
    constans Vicus* v);

#endif /* VICUS_H */
