/* villa_lectores.h - Lectores villae: aedificator mandati ssh +
 * parsatores PURI effusionum servi remoti.
 *
 * NIHIL HIC RETE TANGIT. Omnis functio huius plagulae est functio
 * pura: chorda intrat, structura exit. Quod ssh vere currat, quod
 * processus generetur, quod eventus scribatur - alibi fit
 * (processus.h, apps/villa). Ergo omnis lector hic per fixa vera
 * probari potest sine servo, sine rete, sine daemone.
 *
 * FIXA SUNT SPECIFICATIO. probationes/fixa/villa/ capturas veras ex
 * droplet Debian tenet (bis captas, octetim comparatas, manu
 * expurgatas). Quod parsator praestare debet = quod fixa continent,
 * non quod nos formam esse putamus. Vide fixa/villa/README.md.
 *
 * VITA CHORDARUM: omnis chorda in structuris redditis TRANSCRIBITUR
 * in piscinam traditam. Nulla vista in effusionem intrantem manet.
 * Hoc consulto: effusio processus in arena vocationis vivit (quae
 * singulis tictibus reficitur) dum photographiae probationum per
 * tictus manent. Vista supervivens ruinam silentem pareret.
 *
 * IMPERIA REMOTA POSTULATA: parsatores formas praesumunt quas
 * vexilla certa solae pariunt. Vexillum omissum = forma alia =
 * parsator falsus. Ergo hic nominantur, iuxta lectorem quisque.
 */

#ifndef VILLA_LECTORES_H
#define VILLA_LECTORES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"


/* ========================================================================
 * I. AEDIFICATOR MANDATI SSH
 * ======================================================================== */

/* Terminus viae socketi moderantis. macOS sun_path C III octetos
 * fert; ssh '%C' in XL characteres (friatio) expandit. Supra hunc
 * terminum ssh ligationem tacite deserit et RECIDIT ad manus
 * plenas - quod solum ut mora inexplicata apparet, quia ssh
 * defectum ligationis nonnisi sub '-v' nuntiat. Ergo RECUSAMUS
 * clare potius quam lente degeneremus. */
#define VILLA_MODERANDI_TERMINUS   C     /* octeti viae expansae */
#define VILLA_MODERANDI_SIGILLUM   XL    /* '%C' -> XL characteres */

/* Tectum vectoris argumentorum (forma plena = XVIII elementa). */
#define VILLA_SSH_ARGUMENTA_MAXIMA XXIV

nomen structura {
    /* IDENTITAS PRIMA: alias in ~/.ssh/config. Fran servum ut
	 * 'ssh smaragda' attingit; ssh ipse claves, agentem,
	 * ProxyJump, blocos Match, et Include resolvit. Ergo villa
	 * configurationem MUTUATUR sicut auctoritatem mutuatur, et
	 * configurationem ssh ipsa non parsat (quod vere durum est:
	 * wildcards, Match, Include). */
    chorda alias;

    /* RECIDIVUM pro capsis extra ~/.ssh/config. Adhibentur solum
	 * si alias vacuus est. */
    chorda hospes;
    chorda usor;
       i32 portus;              /* ZEPHYRUM = tacitum (ssh decernit) */
    chorda clavis;              /* -i; vacua = tacitum */

    /* Directorium socketi moderantis, SINE '/%C' terminali.
	 * Vacuum = nulla multiplicatio (semita probationis id facit).
	 * Sedes recta = $HOME/.rhubarb/cm - NON sub build/, quod
	 * speculum_generare et fumus 'rm -rf' faciunt: socketus sub
	 * ssh deletus omne imperium posterum ad manum plenam tacite
	 * degenerat. */
    chorda via_moderandi;

    i32 mora_conexionis;        /* ConnectTimeout; ZEPHYRUM = V */
    i32 perseverantia;          /* ControlPersist; ZEPHYRUM = CCC */

    /* Sutura binarii: 'ssh' nisi VILLA_SSH positum. Semel in
	 * configuratione legitur, ergo semita configurationis et
	 * semita probationis SUTURA UNA sunt, non duae. */
    constans character* via_ssh;
} ConfiguratioSsh;

nomen enumeratio {
    VILLA_SSH_OK = 0,
    VILLA_SSH_SINE_DESTINATIONE,  /* nec alias nec hospes */
    VILLA_SSH_SINE_IMPERIO,       /* imperium remotum vacuum */
    VILLA_SSH_VIA_LONGA,          /* ControlPath supra terminum */
    VILLA_SSH_NIMIS_ARGUMENTORUM  /* vector excessus (custos) */
} VillaSshError;

nomen structura {
              b32 successus;
    VillaSshError error;
           chorda causa;        /* recusatio NOMINATA si !successus */

    /* Vector NIHIL-terminatus, paratus pro processus_exsequi.
	 * NULLA concha: spatia, virgulae, lineae novae in imperio
	 * remoto TUTA sunt, quia nemo ea interpretatur. */
    constans character** argumenta;
                   i32   numerus;   /* sine NIHIL terminali */
} MandatumSsh;

/* Mandatum ssh aedificare. Recusatio semper causam NOMINAT. */
MandatumSsh
villa_mandatum_ssh (
    constans ConfiguratioSsh* configuratio,
                      chorda  imperium,
                     Piscina* piscina);

/* Descriptio erroris ut litterae staticae (pro nuntiis). */
constans character*
villa_ssh_error_nomen (
    VillaSshError error);


/* ========================================================================
 * I-bis. DISCRETIO EXITUS SSH - cur defecit?
 * ========================================================================
 *
 * 'known_hosts trust is a precondition; villa refuses with a NAMED
 * CAUSE' - hoc est quod causam nominat. Sine eo facies unum 'ssh
 * defecit' pro omnibus ostenderet, quod nihil dicit: clavis ignota,
 * conexio recusata, permissio negata, et imperium REMOTUM absens
 * remedia prorsus diversa postulant.
 *
 * LEX FUNDAMENTALIS (ex fixis vera): ssh codicem CCLV pro erroribus
 * SUIS adhibet, et codicem imperii REMOTI aliter transmittit. Ergo
 * CCLV = 'ssh ipse defecit' (erratum causam nominat); quidquid
 * aliud = imperium remotum cucurrit et hunc codicem reddidit.
 *
 * AMBIGUITAS INSANABILIS: imperium remotum quod IPSUM CCLV reddit
 * ab errore ssh distingui NON POTEST. Nulla sutura hoc solvit sine
 * signo (e.g. imperium in 'echo MARCA$?' involvere). v1 id accipit
 * et hic nominat; si quando morsum dederit, signum additur.
 *
 * PRAESUMIT PROCESSUM CUCURRISSE. Si processus_exsequi successus =
 * FALSUM reddidit (binarium ssh absens, furca fracta, mora nostra
 * excessa), ille error PRIOR est et hanc functionem non attingit.
 *
 * LINEA DECISIVA, NON PRIMA: fixum 'clavis_negata' duas lineas
 * fert, quarum PRIMA fallax est ('Load key "/dev/null": invalid
 * format') et SECUNDA decernit ('Permission denied (publickey)').
 * Ergo erratum TOTUM scrutamur; parsator lineae primae falsum
 * responsum daret.
 */

nomen enumeratio {
    VILLA_EXITUS_SUCCESSUS = 0,      /* codex ZEPHYRUM */
    /* --- ssh ipse defecit (codex CCLV) --- */
    VILLA_EXITUS_HOSPES_IGNOTUS,     /* nomen resolvi non potest */
    VILLA_EXITUS_CONEXIO_RECUSATA,   /* portus clausus */
    VILLA_EXITUS_TEMPUS,             /* ConnectTimeout ssh */
    VILLA_EXITUS_CLAVIS_HOSPITIS,    /* known_hosts non consentit */
    VILLA_EXITUS_PERMISSIO,          /* clavis recusata */
    VILLA_EXITUS_SSH_ALIUS,          /* CCLV, causa ignota */
    /* --- imperium remotum cucurrit --- */
    VILLA_EXITUS_IMPERIUM_ABSENS,    /* CXXVII */
    VILLA_EXITUS_IMPERIUM_FRACTUM    /* codex alius non-zerus */
} VillaExitus;

nomen structura {
    VillaExitus genus;
         chorda causa;      /* linea DECISIVA erratii, verbatim */
            i32 codex;      /* codex exitus crudus */
            b32 ssh_ipse;   /* VERUM = defectus ssh, non imperii */
} CausaExitus;

CausaExitus
villa_exitum_discernere (
        i32  codex_exitus,
     chorda  erratum,
    Piscina* piscina);

/* Nomen breve ut litterae staticae (pro faciebus et nuntiis). */
constans character*
villa_exitus_nomen (
    VillaExitus genus);


/* ========================================================================
 * II. SYSTEMCTL SHOW - status servitiorum
 * ======================================================================== */

/* IMPERIUM: systemctl show <unitates...> --property=<claves>
 *                     --no-pager
 * CLAVES: Id,Description,LoadState,ActiveState,SubState,
 *         UnitFileState,ActiveEnterTimestamp,MainPID,NRestarts,
 *         Result,ExecMainStatus
 *
 * CAUTIO CLAVIUM: systemd proprietates ORDINE SUO reddit, non
 * ordine petito (MainPID praecedit Id praecedit Description). Ergo
 * hic lector CLAVE ducitur; lector positione ductus falsus esset.
 *
 * Valores vacui NORMALES sunt: unitas ignota 'UnitFileState=' et
 * 'ActiveEnterTimestamp=' nudas reddit.
 *
 * ActiveEnterTimestamp chorda HUMANA est ('Fri 2025-09-12 19:42:15
 * UTC'), non epocha. Verbatim servatur et monstratur. Si quando
 * numerus 'per N dies currens' volueris, PETE
 * ActiveEnterTimestampMonotonic - noli hanc parsare.
 *
 * DUAE INSIDIAE EX FIXO 'fracta' (captura vera 2026-07-24, unitate
 * temporaria in droplet consulto fracta):
 *
 *  (a) 'ActiveEnterTimestamp=' VACUUM est in unitate FRACTA, in
 *      unitate INACTIVA, ET in unitate IGNOTA pariter. Ergo campus
 *      vacuus NIHIL de causa dicit - eo ut indicio 'non inventa'
 *      uti falsum esset.
 *  (b) 'Result=success' redditur in unitate MORTUA et in unitate
 *      IGNOTA. Ergo causa_finis SOLUM cum 'fracta' VERUM est
 *      significat quicquam; alioquin monstrata mendacium ostenderet
 *      ('Result: success' iuxta servitium quod non currit). Valor
 *      verbatim servatur - lector systemd non mentiatur - sed
 *      vexillum 'fracta' iudicium portat. Probatio §XVII insidiam
 *      ipsam adserit, ne commentarius solus eam custodiat.
 */

nomen structura {
    chorda id;                  /* Id */
    chorda descriptio;          /* Description */
    chorda status_oneris;       /* LoadState */
    chorda status_vitae;        /* ActiveState */
    chorda sub_status;          /* SubState */
    chorda status_plagulae;     /* UnitFileState (potest vacuus) */
    chorda tempus_initii;       /* ActiveEnterTimestamp (verbatim) */

    /* Result: success|exit-code|signal|timeout|oom-kill|core-dump|
	 * watchdog|start-limit-hit|resources|protocol.
	 * SIGNIFICAT NIHIL nisi 'fracta' - vide (b) supra. */
    chorda causa_finis;
       i32 codex_exitus;        /* ExecMainStatus */
       i32 pid;                 /* MainPID; ZEPHYRUM = nullus */
       i32 restitutiones;       /* NRestarts */

    /* Iudicia derivata - unum locum, ne quisque consumptor
	 * chordas iterum comparet (et alius aliter). */
    b32 inventa;                /* LoadState != 'not-found' */
    b32 currit;                 /* ActiveState == 'active' */
    b32 fracta;                 /* ActiveState == 'failed' */
} StatusServitii;

/* Redde Xar de StatusServitii (ordine effusionis). Tabulata vacua
 * intrante vacua. */
Xar*
villa_systemctl_legere (
      chorda  effusio,
     Piscina* piscina);


/* ========================================================================
 * III. SYSTEMCTL LIST-UNITS - inventio currentium
 * ======================================================================== */

/* IMPERIUM: systemctl list-units --type=service --state=running
 *                     --no-pager --plain
 *
 * '--plain' NON OMITTENDUM: sine eo systemctl notas ('*', globulum)
 * ante unitates defectas ponit et lineam primam frangit.
 *
 * CAUTIO LEGENDAE: effusio LEGENDAM in calce fert post lineam
 * vacuam ('Legend: LOAD ...' + 'N loaded units listed.'). Lector
 * ad lineam vacuam PRIMAM desinit; alioquin prosam ut unitates
 * legeret.
 *
 * DESCRIPTIO spatia continet et ultima columna est - ergo quattuor
 * verba prima sola finduntur, reliquum verbatim manet. Columnae
 * spatiis variis ordinantur (fixa nostra id probant: expurgatio
 * ordinationem in tribus lineis fregit et parsator recte manet),
 * ergo findendum est per CURSUS spatii albi, non per columnas
 * fixas.
 */

nomen structura {
    chorda unitas;              /* UNIT */
    chorda onus;                /* LOAD */
    chorda vita;                /* ACTIVE */
    chorda sub;                 /* SUB */
    chorda descriptio;          /* DESCRIPTION (spatia continet) */
} UnitasCursoria;

Xar*
villa_unitates_legere (
      chorda  effusio,
     Piscina* piscina);


/* ========================================================================
 * IV. NGINX -T - situs procurati
 * ======================================================================== */

/* IMPERIUM: sudo nginx -T (configuratio RESOLUTA tota, cum
 *                          inclusis expansis)
 *
 * Hic parsator maximus est et fixum nostrum quinque insidias fert
 * quas forma FICTA numquam ostendisset:
 *
 *  (a) TABULATURAE ET SPATIA MISCENTUR intra plagulam unam. Omnis
 *      praesumptio de indentatione falsa est.
 *  (b) 'server {' IN COMMENTARIIS APPARET (bloci mail exemplares,
 *      hospes virtualis exemplaris in sites-enabled/default).
 *      Scrutator qui commentaria non tollit eos NUMERAT.
 *  (c) 'if (...) { }' intra blocum server profunditatem auget -
 *      ergo profunditas uncorum vera numeranda est, non lineae.
 *  (d) 'proxy_pass' intra 'location' vivit, UNO UNCO ALTIUS quam
 *      'server_name'. Scrutator planus eum non videt.
 *  (e) Certbot bloca GEMINA parit: unum HTTPS cum proxy_pass +
 *      ssl_certificate, alterum HTTP quod solum 'return 301' agit.
 *      Qui blocum unum per situm praesumit, DUPLICITER numerat.
 *
 * Praeterea 'server_tokens' cum 'server' NON confundendum est
 * (comparatio verbi tota, non praefixum), et 'ssl_certificate_key'
 * cum 'ssl_certificate' non item.
 */

/* Clausula = blocus 'server { }' unus, crudus, ut in plagula
 * iacet. */
nomen structura {
    chorda hospes;              /* server_name primum */
    chorda hospites;            /* server_name omnia, spatio iuncta */
    chorda destinatio;          /* proxy_pass PRIMUM (vacua si nulla) */

    /* QUOT 'proxy_pass' in bloco fuerint. 'destinatio' PRIMUM
	 * solum tenet - blocum cum locationibus pluribus ad upstreams
	 * diversos aliter TACITE unum ostenderet ut si solus esset.
	 * Numerus terminum VISIBILEM facit: facies 'unum ex tribus'
	 * dicere potest potius quam mentiri. Droplet nostrum unam
	 * locationem per servum habet, ergo fixa hoc non exercent -
	 * id est ratio propter quam numerus adest, non contra. */
    i32 destinationes;

    chorda certificatum;        /* ssl_certificate (vacuum si nullum) */
    chorda radix;               /* root (vacua si nulla) */
    chorda plagula;             /* '# configuration file X:' proximum */
       i32 linea;               /* linea ubi 'server {' (I-basata) */
       b32 ssl;                 /* aliquod 'listen ... ssl' */
       b32 redirigit;           /* 'return 30x' praesens */
} ClausulaNginx;

/* Situs = clausulae eiusdem nominis PLICATAE (gemina Certbot in
 * unum). */
nomen structura {
    chorda hospes;
    chorda hospites;
    chorda destinatio;           /* primum inventum */
       i32 destinationes;        /* summa per bloca plicata */
    chorda certificatum;
    chorda radix;
    chorda plagula;
       b32 ssl;
       b32 habet_redirectionem;  /* clausula socia http->https adest */
       i32 clausulae;            /* quot bloca in hunc situm plicata */
} SitusNginx;

/* Bloca 'server' cruda, ordine plagulae. */
Xar*
villa_nginx_clausulas_legere (
      chorda  effusio,
     Piscina* piscina);

/* Clausulas in situs plicare (gemina Certbot conflantur). */
Xar*
villa_nginx_situs_plicare (
    constans Xar* clausulae,
         Piscina* piscina);

/* Commoditas: ambo gradus uno vocamine. */
Xar*
villa_nginx_legere (
      chorda  effusio,
     Piscina* piscina);


/* ========================================================================
 * IV-bis. SECTIONES PROBATIONIS - effusio composita findere
 * ========================================================================
 *
 * Probatio UNA quinque lectiones fert (systemctl, nginx, df,
 * uptime, meminfo). Vocatio ssh per lectionem quinque itinera et
 * quinque processus posceret; imperium COMPOSITUM unum sufficit.
 * (ControlMaster itinera posteriora vilia facit, non gratuita.)
 *
 * FORMA, quam scriptum remotum emittit:
 *     ##VILLA##<nomen>##
 *     <effusio imperii, cum errato mixto>
 *     ##VILLA##fin##<codex>##
 *
 * Codex POST effusionem venit quia ante eam ignotus est. Ergo
 * sectio quae finem suum non invenit CLAUSA NON EST - et id est
 * signum quod duas res simul prodit: imperium remotum abscissum
 * (conexio cecidit medio) AUT collisionem marcae (plagula
 * configurationis lineam nostrae similem ferens). Utrumque
 * 'clausa=FALSUM' nominat; consumptor sectionem non clausam
 * FIDERE NON DEBET.
 */

#define VILLA_MARCA_SECTIONIS "##VILLA##"

nomen structura {
    chorda titulus;       /* nomen sectionis ('nomen' = macro!) */
    chorda contentum;     /* effusio, sine lineis marcae */
       i32 codex;         /* exitus imperii (si clausa) */
       b32 clausa;        /* finem suum invenit? */
} SectioProbationis;

/* Effusionem compositam in sectiones findere (ordine emissionis). */
Xar*
villa_sectiones_legere (
      chorda  effusio,
     Piscina* piscina);

/* Sectionem nomine invenire (NIHIL si abest). */
constans SectioProbationis*
villa_sectionem_invenire (
     constans      Xar* sectiones,
    constans character* titulus);


/* ========================================================================
 * V. LECTORES PARVI - discus, memoria, tempus activum
 * ======================================================================== */

/* Hi tres ex fixis VARIANTIBUS veniunt: numeri inter capturas
 * mutant, forma non. Ergo probationes FORMAM figunt et numerum
 * NUMQUAM. */

/* IMPERIUM: df -P /
 * '-P' (POSIX) non omittendum: sine eo df nomina longa in lineam
 * alteram frangit et columnae vagantur. */
nomen structura {
    chorda systema;             /* Filesystem */
    chorda punctum;             /* Mounted on */
       i64 frusta;              /* 1024-blocks */
       i64 usa;                 /* Used */
       i64 praesto;             /* Available */
       i32 capacitas;           /* Capacity, sine '%' */
} StatusDisci;

b32
villa_discum_legere (
             chorda  effusio,
        StatusDisci* fructus,
            Piscina* piscina);

/* IMPERIUM: head -5 /proc/meminfo
 * /proc adhibetur, non 'free', quia forma /proc per versiones
 * stabilis manet dum prosa 'free' mutat. */
nomen structura {
    i64 summa_kb;               /* MemTotal */
    i64 libera_kb;              /* MemFree */
    i64 praesto_kb;             /* MemAvailable */
    i64 interposita_kb;         /* Buffers */
    i64 condita_kb;             /* Cached */
} StatusMemoriae;

b32
villa_memoriam_legere (
              chorda  effusio,
      StatusMemoriae* fructus);

/* IMPERIUM: cat /proc/uptime
 * Forma: '<activum>.<cc> <otiosum>.<cc>'. Pars integra sola
 * sumitur - nullum punctum fluitans in villa. */
b32
villa_tempus_activum_legere (
    chorda  effusio,
       i64* secunda);

#endif /* VILLA_LECTORES_H */
