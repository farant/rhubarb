#ifndef ATRIUM_H
#define ATRIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "capsula.h"
#include "fenestra.h"
#include "vitrea.h"
#include "internuntius.h"
#include "speculum.h"
#include "imperium.h"


/* atrium.h - applicatio vitreae PLENA (baculi inclusi)
 *
 * PROBLEMA MENSURATUM: forum, villa, mensor_ui eandem structuram
 * ~LXV linearum manu scribunt - figuram fenestrae, capsulam,
 * figuram vitreae (in tribus IDENTICAM, litteram ad litteram),
 * internuntium, gyrum duplicem, destructionem ORDINE RECTO. Nemo
 * eam elegit; unaquaeque app eam a priore descripsit.
 *
 * PEIUS: quaeque app SUBSETUM baculorum accepit, casu non consilio.
 * villa et forum speculum habent, imperium non; mensor_ui imperium
 * habet, speculum non. Neutrum decretum est - utrumque accidit.
 * Id est ipsum vitium quod atrium tollit.
 *
 * CUR NON IN VITREA: vitrea.h finem PERPETUUM suum declarat -
 * "adhaesio texturae + origo assetorum + transportus chordarum, IN
 * AETERNUM". Atrium hospitium, speculum, imperium trahit; ea in
 * vitream ponere OMNEM applicationem vitreae servitorem HTTP ligare
 * cogeret, etiam eam quae nihil servit. Idem argumentum quo
 * vivarium in imperio et non in vitrea positum est.
 *
 * PRETIUM APERTUM: qui atrium includit hospitium/speculum/imperium
 * LIGAT, adhibeat necne. Id est pactum baculorum inclusorum. Cui
 * WebKit nudum et nihil aliud opus est, vitream solam adhibeat -
 * quae PRIMITIVUM manet et manebit.
 *
 * ==========================================================
 * GYRUS APPLICATIONIS EST, NON ATRII
 * ==========================================================
 *
 * Atrium gyrum NON possidet. App suum servat - ergo 'frange' agit,
 * exitus proprios reddit, printf in gyro ponit, punctum interruptionis
 * figit. Nulla inversio regiminis, nulla sutura per-tictum quam
 * recte scribere debeas.
 *
 *   Atrium* atrium = atrium_creare(piscina, &figura, &causa);
 *   si (atrium == NIHIL) { ... causa NOMINAT quid defecerit ... }
 *
 *   internuntius_praebere(atrium_internuntius(atrium),
 *                         "villa_status", _villa_status, &villa);
 *   ... ceterae methodi ...
 *
 *   atrium_monstrare(atrium);
 *
 *   dum (atrium_currendum(atrium))
 *   {
 *       (vacuum)atrium_gressus(atrium);
 *
 *       si (villa_agens_pulsare(villa.agens) > ZEPHYRUM)
 *       {
 *           villa.cycli++;
 *       }
 *       si (villa.fumus_perfectus_est) { frange; }
 *   }
 *
 *   atrium_destruere(atrium);
 *
 * QUOD ATRIUM TENET quia solum semel recte scribi debet:
 *   - ORDO DESTRUCTIONIS: vitrea_destruere ANTE fenestra_destruere.
 *     vitrea.h eum nominat quia facile invertitur; hic semel tenetur.
 *   - ARENA per-tictum (vide infra).
 *   - INTERITUS processus interretialis -> recargatio. Tres apps eum
 *     eodem modo tractabant; nunc consilium domus est.
 *   - Discretio nuntii pontis (vide atrium_nuntium_discernere).
 *
 * ==========================================================
 * ARENA: REFICITUR INITIO, NON FINE
 * ==========================================================
 *
 * villa et forum hodie piscina_notare initio tictus et
 * piscina_reficere FINE vocant - par quod oblivisci potes, et quod
 * memoriam app statim post usum necat (forum JSON in ea struit).
 *
 * Atrium arenam INITIO cuiusque gressus reficit. Ergo:
 *   - nihil est quod paretur; par evanuit
 *   - quod ex arena post gressum sumis usque ad gressum PROXIMUM
 *     valet - vita intuitiva, et LAXIOR quam hodie
 *
 *   Piscina* arena = atrium_arena(atrium);
 *
 * DECIPULA UNICA HUIUS CONSILII: atrium_gressus BIS in uno tictu
 * vocare arenam bis reficit - quidquid inter duas vocationes ex ea
 * sumpsisti stale fit. Semel per tictum voca. (Eam omittere innocuum
 * est comparate: fenestra pumpare desinit et app palam pendet.)
 *
 * ========================================================== */

nomen structura Atrium Atrium;


/* ==================================================
 * Nuntius pontis: quid pagina misit
 * ==================================================
 *
 * Pagina chordam NUDAM mittere potest ('paratus'), quae JSON-RPC
 * non est. Sine discrimine internuntius de ea queritur ('culpa
 * orba') in consola omni onere - strepitus qui culpas VERAS
 * obtegeret.
 *
 * STATUS MENSURATUS (ne plus dicamus quam verum est): SOLA pagina
 * mensoris postMessage crudum vocat, ergo sola mensor_ui custodiam
 * hanc habet. Paginae villae et fori per pontem a vitrea iniectum
 * loquuntur et NIHIL nisi JSON mittunt - ergo custodia eis non
 * deest, sed opus non fuit.
 *
 * CUR TAMEN HIC: quaeque app quae iniectionem vult paginam suam
 * paratam nuntiare debet, et tunc custodiam eandem rursus deduceret.
 * Semel hic scripta, conventio domus fit pro omnibus quae sequuntur.
 *
 * PURA - probabilis sine fenestra. */

nomen enumeratio {
    ATRIUM_NUNTIUS_IGNOTUS = 0,  /* nec JSON nec verbum notum */
    ATRIUM_NUNTIUS_PARATUS,      /* chorda nuda "paratus" */
    ATRIUM_NUNTIUS_RPC           /* JSON-RPC (internuntio tradendus) */
} AtriumNuntiusGenus;

AtriumNuntiusGenus
atrium_nuntium_discernere (
    chorda nuntium);


/* ==================================================
 * Quid hoc tictu accidit (vexilla ex atrium_gressus)
 * ================================================== */

nomen enumeratio {
    ATRIUM_ACTUM_NIHIL   = 0,
    ATRIUM_ACTUM_PARATA  = 1,  /* pagina se paratam nuntiavit */
    ATRIUM_ACTUM_RPC     = 2,  /* nuntius internuntio traditus */
    ATRIUM_ACTUM_INTERITUS = 4 /* processus interretialis obiit;
                                * atrium IAM recargavit */
} AtriumActum;


/* ==================================================
 * Configuratio
 * ==================================================
 *
 * memset ZEPHYRO, deinde quod opus est pone. Omnia ZEPHYRUM/NIHIL
 * ordinaria sana significant - EXCEPTIS titulo et capsula, sine
 * quibus atrium_creare RECUSAT et causam NOMINAT.
 */

/* Eventus fenestrae quos atrium non consumpsit (speculum eos PRIMUM
 * videt; quod ille rapit hic non apparet).
 *
 * HAEC SOLA SUTURA INVERSA EST, et de industria: pumpa eventuum
 * intra atrium_gressus vivit, ergo app ad eventus crudos aliter
 * pervenire NON POTEST. Tres apps hodiernae nullam adhibent - sed
 * foramen in API relinquere quod post refici non possit peius est
 * quam campus optivus quem nemo implet. */
nomen vacuum (*AtriumEventor)(
    vacuum*           datum,
    constans Eventus* eventus);

nomen structura {
    /* --- FENESTRA --- */
    constans character* titulus;        /* OBLIGATORIUS */
                   i32  latitudo;       /* ZEPHYRUM = 1000 */
                   i32  altitudo;       /* ZEPHYRUM = 900 */
                   i32  vexilla;        /* ZEPHYRUM = CLAUDIBILIS
                                         * | MUTABILIS | CENTRATA */
    b32 retro;          /* VERUM = focum systematis
                                         * NON rapere (FENESTRA_RETRO;
                                         * vexillum domus '-retro' -
                                         * probationes fumi sine
                                         * interpellatione usoris) */

    /* --- PAGINA --- */
    constans CapsulaEmbed* capsula;       /* OBLIGATORIA */
       constans character* via_initialis; /* NIHIL = "index.html" */
    /* RADIX: non-NIHIL = assetum e DIRECTORIO legere, non e capsula
     * infixa. Vexillum domus '-radix <via>' hoc implet.
     *
     * SCHEMA, URL, ORIGO MANENT EADEM (capsula://...): pagina discernere
     * NON POTEST. Ergo modus evolutionis a re vecta uno solo modo
     * differt - QUAE PLAGULAE EXSTENT - non transportu, portu, origine.
     * Vide capsula.h; hospitium hic nullum est et nihil auscultat.
     *
     * 'capsula' OBLIGATORIA MANET: id est quod vehitur. Radix fontem
     * mutat, non pactum - ergo binarium sine vexillo semper id agit
     * quod vectum est. */
    constans character* capsula_radix;
    /* VERUM = inspector Safari CELATUS. Tres apps eum volunt, ergo
     * ordinarium est adesse; capsula vecta eum claudat.
     *
     * NOMEN WEBKIT NOMINAT DE INDUSTRIA: hic campus UNAM rem regit -
     * an Safari se paginae adiungere possit. Si inspectorem DOMUS
     * aliquando aedificamus, is baculum SEPARATUM erit cum capsula
     * propria (sicut 'fontes' speculo), non valor alius huius campi.
     * Campum 'sine_inspectore' nominare regionem occuparet quam
     * nondum decrevimus. */
    b32 sine_inspectore_webkit;

    /* --- SPECULUM (Cmd+Shift+D) --- */
    /* Capsula FONTIUM, non assetorum - a bin/capsula_generare
     * distincta. NIHIL = sine speculo (et app hoc ELEGIT, non
     * oblita est: campus hic in structura visibilis sedet). */
    constans CapsulaEmbed* fontes;

    /* --- IMPERIUM (modus evolutionis) --- */
    /* PERICULUM: JS arbitrarium exsequitur. Post vexillum sedeat,
     * in capsula vecta NUMQUAM. Vide imperium.h. */
    b32 vivum;
    i32 portus;                         /* ZEPHYRUM = auto-selectus */

    /* --- GYRUS --- */
              Mora  tictus_ms;           /* ZEPHYRUM = CC */
    memoriae_index  arena_mensura;       /* ZEPHYRUM = M * M */
     AtriumEventor  eventor;             /* NIHIL = eventus absorbentur */
            vacuum* datum;               /* eventori traditum */
} AtriumConfiguratio;

/* Vexilla domus ex argv legere: -vivum, -portus N.
 *
 * Configurationem IMPLET solum (nihil aliud legit) - app cetera sua
 * ipsa legat. Optivum de industria: argv tacite legere miraculum
 * esset, sed ternis apps easdem duas lineas scribere cogere
 * duplicatio est quam atrium tollere debet. */
vacuum
atrium_vexilla_legere (
    AtriumConfiguratio*  figura,
               integer   argc,
             character** argv);


/* ==================================================
 * Cyclus vitae
 * ================================================== */

/*
 * atrium_creare - thema, fenestram, capsulam, vitream, internuntium,
 * speculum (si fontes), imperium (si vivum) - ordine recto.
 *
 * Redde NIHIL cum culpa; *causa (si non NIHIL) NOMINAT quid
 * defecerit - numquam vacua cum NIHIL redditur.
 */
Atrium*
atrium_creare (
                        Piscina* piscina,
    constans AtriumConfiguratio* figura,
                         chorda* causa);

/* Fenestram monstrare. Post methodos praebitas vocanda: pagina
 * onerari incipit, et methodus quae nondum praebita est vocari
 * potest. */
vacuum
atrium_monstrare (
    Atrium* atrium);

/* Condicio gyri: FALSUM cum fenestra claudi debet. */
b32
atrium_currendum (
    constans Atrium* atrium);

/*
 * atrium_gressus - Gradus unus: arena reficitur, eventa fenestrae
 * exhauriuntur (speculum primum, deinde eventor), cauda pontis
 * exhauritur, imperium pulsatur, iniectio curritur si pagina
 * modo parata est.
 *
 * Redde vexilla AtriumActum (OR-iuncta). Ignorare tutum est.
 *
 * SEMEL PER TICTUM (vide decipulam arenae supra).
 */
i32
atrium_gressus (
    Atrium* atrium);

/* Ordo rectus intus: vitrea ANTE fenestram. */
vacuum
atrium_destruere (
    Atrium* atrium);


/* ==================================================
 * Iniectio: JS semel post 'paratus'
 * ==================================================
 *
 * mensor_ui hoc manu agebat: nuntium 'paratus' exspectare, deinde
 * window.MENSURAE iniicere. Sine exspectatione iniectio cum onere
 * certat et interdum paginam vacuam invenit.
 *
 * PACTUM PAGINAE - PARS TUA: pagina se paratam nuntiare DEBET, aut
 * iniectio numquam currit (silentium, non culpa - atrium paginam
 * cogere non potest). Una linea in onere:
 *
 *   var h = window.webkit && window.webkit.messageHandlers;
 *   if (h && h.internuntius) { h.internuntius.postMessage("paratus"); }
 *
 * ORDO NON REFERT: si iniectionem POST paginam paratam ponis, statim
 * currit. Ergo eam ante gyrum aut in eo ponere utrumque valet.
 *
 * POST RECARGATIONEM ITERUM CURRIT. Pagina nova status novus est -
 * quod post interitum processus interretialis recargatum est
 * iniectionem suam amisit. mensor_ui hoc hodie NON tractat (datum
 * eius numquam mutatur, ergo nemo id vidit); atrium id gratis dat.
 *
 * Chorda COPIATUR in piscinam atrii - aedificator vocantis post hoc
 * liberari aut reficii potest.
 */

vacuum
atrium_iniectionem_ponere (
    Atrium* atrium,
    chorda  javascript);


/* ==================================================
 * Accessus ad partes
 * ==================================================
 *
 * Atrium partes non celat: app methodos praebere, JS aestimare,
 * imagines petere debet. Celare eas ansam magis rigidam faceret
 * sine lucro - atrium ASSEMBLATIO est, non abstractio.
 */

/* Ubi methodos praebeas (ANTE atrium_monstrare). */
Internuntius*
atrium_internuntius (
    Atrium* atrium);

/* Pro vitrea_aestimare, vitrea_imaginem_petere. */
Vitrea*
atrium_vitrea (
    Atrium* atrium);

Fenestra*
atrium_fenestra (
    Atrium* atrium);

/* Arena per-tictum: REFICITUR INITIO cuiusque gressus. Quod hinc
 * sumis usque ad gressum proximum valet. */
Piscina*
atrium_arena (
    Atrium* atrium);

/* Portus imperii VERUS (auto-selectus resolutus), aut ZEPHYRUM si
 * atrium sine -vivum currit. */
i32
atrium_portus (
    constans Atrium* atrium);

#endif /* ATRIUM_H */
