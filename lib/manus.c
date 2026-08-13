#include "postulata_posix.h"

#include "manus.h"

#include "chorda_aedificator.h"
#include "credo.h"
#include "http.h"
#include "json.h"
#include "processus.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/* manus.c - vide manus.h pro consilio et ratione.
 *
 * ORDO LEGENDI: _nunc/_quiescere (tempus) -> _petere (HTTP) ->
 * _tesseram_* (ansa imperii) -> _js_* (quid paginae mittitur) ->
 * superficies publica.
 */

/* Gratia super terminum paginae: si pagina intra suum terminum non
 * respondet, respondere OMNINO non potest (haeret, mortua est).
 * Vide manus.h 'TERMINUS DUPLEX'.
 *
 * CUR TAM LARGA: gratia non solum promissum exspectat sed ITER
 * REDITUS - responsum per internuntium in gyrum applicationis it, et
 * gyrus ille quotiens vult pulsat. Laboratorium 200 ms tictum habet;
 * D ms ergo duos tictus cum dimidio dabat, quod SUB ONERE perdidit.
 *
 * MENSURATUM 2026-08-13: assertum de pyxide impedita (unicum quod
 * terminum paginae PLENUM currit, nam pyxis clausa numquam aperitur)
 * intermittenter 'applicatio non respondit' de applicatione sana
 * nuntiabat - uno cursu ex tribus. Probatio FRAGILIS de bibliotheca
 * cuius munus est fragilitatem tollere; ridiculum et corrigendum.
 *
 * Manus tictum hospitis scire NON potest, ergo large datur. Nihil
 * constat in casu felici (gratia solum tunc valet cum pagina iam
 * terminum suum tetigit); mors vera paulo tardius nuntiatur. */
#define MANUS_GRATIA        ((Mora)(M + D))

/* Intervallum pulsationis tesserae: incipit angustum ut casus
 * FELIX (assertum quod statim verum est) statim redeat; crescit ut
 * mora vera gyrum non torreat. Nihil de VERITATE decernit - solum
 * quando eam discas. */
#define MANUS_PULSUS_PRIMUS ((Mora)II)
#define MANUS_PULSUS_MAXIMUS ((Mora)L)

/* Tempus unius petitionis HTTP. Non est terminus exspectationis -
 * petitiones ipsae statim redeunt (tessera aut status). */
#define MANUS_PETITIO_MS    ((MoraAngusta)(II * M))

#define MANUS_HOSPES_MAXIMUS LXIV
#define MANUS_VIA_MAXIMA     CCLVI

structura Manus {
    /* DUAE arenae, et distinctio non est commoditas.
     *
     * 'piscina' vocatoris est et DURAT. 'scriptorium' manus est et
     * post quamque petitionem REFICITUR - aliter gyrus pulsationis
     * duorum secundorum decies petitionum HTTP congereret.
     *
     * LEX: quod ex scriptorio exire debet per _servare in piscinam
     * transferatur ANTE reficere. Prima forma unam arenam habebat
     * et notationem in eadem sumebat quam _servare implebat: copia
     * ipsa reficiebatur, et 'ok' ex memoria mortua relegebatur -
     * assertum quod elementum suum VIDIT et tamen falsum dixit.
     * Probatio semitae FELICIS id invenit; semitae fallentes omnes
     * transierant. */
    Piscina*   piscina;
    Piscina*   scriptorium;

    character  hospes[MANUS_HOSPES_MAXIMUS];
    i32        portus;

    /* NIHIL nisi manus ipsa applicationem genuit. Praesentia eius
     * est quod gyrum haurire cogit. */
    Processus* processus;

    b32        fracta;
    chorda     causa;

    /* Dum paratitudinem probamus, defectus EXSPECTATUR - frangere
     * manum ob eum absurdum esset. */
    b32        tacens;

    /* Imago automatica cum manus frangitur; vacua = tacita.
     * 'in_imagine' recursionem vetat: si captura ipsa fallit,
     * _frangere iterum vocaretur. */
    character  via_culpae[MANUS_VIA_MAXIMA];
    b32        in_imagine;
};

/* Quid ansa tota rettulit. 'respondit' ab 'ok' DISTAT consulto:
 * primum de applicatione loquitur, alterum de facie. Confusa,
 * probatio fracta narrare non potest utrum res tua fracta sit an
 * abfuerit. */
nomen structura {
    b32    respondit;
    b32    ok;
    chorda visum;
} ManusVerdictum;

nomen structura {
    b32    successus;   /* connexio facta, responsum lectum */
    i32    status;
    chorda corpus;
} ManusResponsum;

/* ========================================================================
 * Tempus
 * ======================================================================== */

/* Computatio in s64 NUDO fit, deinde semel benedicitur: punctum x
 * punctum sensu caret, et scalaris Momentum non est. Vide fasti.h
 * 'Insignatum neutrum ... conversio explicita erasionem
 * benedicit'. */
interior Momentum
_nunc (
    vacuum)
{
    structura timeval tv;
    gettimeofday(&tv, NIHIL);
    redde (Momentum)((s64)tv.tv_sec * (s64)M + (s64)tv.tv_usec / (s64)M);
}

/* nanosleep, non usleep: XPG7 usleep sustulit, et sub postulatis
 * strictis in Linux omnino evanescit (uncus id monuit). */
interior vacuum
_quiescere (
    Mora ms)
{
    structura timespec ts;

    si (ms <= 0)
    {
        redde;
    }
    ts.tv_sec  = (time_t)((s64)ms / (s64)M);
    ts.tv_nsec = (longus)(((s64)ms % (s64)M) * (s64)(M * M));
    (vacuum)nanosleep(&ts, NIHIL);
}

/* ========================================================================
 * Chordae
 * ======================================================================== */

/* Chordam in arenam DURABILEM transferre. Necessarium quia gyrus
 * pulsationis arenam suam post quamque petitionem reficit - quod
 * inde exit et non servatur memoria mortua est. */
interior chorda
_servare (
    chorda   s,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, s.mensura + I);
    chorda_aedificator_appendere_chorda(a, s);
    redde chorda_aedificator_finire(a);
}

/* chorda -> litterae NIHIL-terminatae (credo litteras poscit;
 * chorda nostra terminum non fert). */
interior constans character*
_litterae (
    chorda   s,
    Piscina* piscina)
{
    character* b = (character*)piscina_allocare(
        piscina, (memoriae_index)s.mensura + I);
    si (s.mensura > 0)
    {
        memcpy(b, s.datum, (memoriae_index)s.mensura);
    }
    b[s.mensura] = '\0';
    redde b;
}

interior b32
_imaginem (
    Manus*               manus,
    constans character*  via,
    Mora                 mora,
    constans character** causa_out);

interior vacuum
_frangere (
    Manus*              manus,
    constans character* causa)
{
    si (manus->tacens)
    {
        redde;
    }
    /* Prima causa sola servatur: sequentes eius sequelae sunt. */
    si (!manus->fracta)
    {
        manus->fracta = VERUM;
        manus->causa  = chorda_ex_literis(causa, manus->piscina);

        /* Imago in ipso momento fracturae - ANTE quam quicquam
         * aliud paginam moveat. Mora BREVIS consulto: si
         * applicatio mortua est (causa frequens), longa hic decem
         * secunda cuique fracturae adderet.
         *
         * 'in_imagine' recursionem vetat: captura fallens
         * _frangere iterum vocaret. */
        si (manus->via_culpae[0] != '\0' && !manus->in_imagine)
        {
            constans character* ignorata = NIHIL;

            manus->in_imagine = VERUM;
            (vacuum)_imaginem(manus, manus->via_culpae,
                              MANUS_MORA_BREVIS, &ignorata);
            manus->in_imagine = FALSUM;
        }
    }
}

/* ========================================================================
 * HTTP
 * ======================================================================== */

interior ManusResponsum
_petere (
    Manus*              manus,
    HttpMethodus        methodus,
    constans character* via,
    chorda              corpus,
    Piscina*            arena)
{
    ManusResponsum r;
    character      url[CCLVI];
    HttpPetitio*   pet;
    HttpResultus   res;

    r.successus = FALSUM;
    r.status    = 0;
    r.corpus.mensura = 0;
    r.corpus.datum   = NIHIL;

    sprintf(url, "http://%s:%d%s", manus->hospes, (integer)manus->portus, via);

    pet = http_petitio_creare(arena, methodus, url);
    si (pet == NIHIL)
    {
        redde r;
    }
    http_petitio_tempus_ponere(pet, MANUS_PETITIO_MS);
    si (corpus.mensura > 0)
    {
        http_petitio_corpus_ponere_chorda(pet, corpus);
    }

    res = http_exsequi(pet, arena);
    si (!res.successus || res.responsum == NIHIL)
    {
        redde r;
    }

    r.successus = VERUM;
    r.status    = res.responsum->status;
    r.corpus    = res.responsum->corpus;
    redde r;
}

/* ========================================================================
 * Ansa imperii: tesseram petere, tesseram pulsare
 * ======================================================================== */

/* POST quod tesseram reddit. -1 si applicatio non respondit. */
/* Nihil hinc exit nisi numerus, ergo scriptorium totum reficitur. */
interior s64
_tesseram_petere (
    Manus*              manus,
    constans character* via,
    chorda              corpus)
{
    PiscinaNotatio nota = piscina_notare(manus->scriptorium);
    ManusResponsum r;
    JsonResultus   j;
    JsonValor*     t;
    s64            tessera = -I;

    /* CCII 'Acceptum', non CC: imperium tesseram reddit, non
     * effectum - iussum enim nondum factum est. Primum hic CC
     * SOLUM accipiebam, et simulacrum probationis CC reddebat quia
     * ego id scripseram - XXIV asserta viridia mecum consentiebant,
     * non cum servo vero. Ergo omnis species II. */
    r = _petere(manus, HTTP_POST, via, corpus, manus->scriptorium);
    si (r.successus && r.status >= CC && r.status < CCC)
    {
        j = json_legere(r.corpus, manus->scriptorium);
        si (j.successus && j.radix != NIHIL)
        {
            t = json_objectum_capere(j.radix, "tessera");
            si (t != NIHIL && json_est_integer(t))
            {
                tessera = json_ad_integer(t);
            }
        }
    }

    piscina_reficere(manus->scriptorium, nota);
    redde tessera;
}

/* Tesseram pulsare donec perficiatur, fallat, aut terminus
 * exterior transeat. Valor JSON crudus in arenam manus servatur. */
interior b32
_tesseram_pulsare (
    Manus*   manus,
    s64      tessera,
    Momentum terminus,
    chorda*  valor,
    chorda*  culpa)
{
    character via[LXIV];
    Mora      intervallum = MANUS_PULSUS_PRIMUS;
    chorda    vacuum_corpus;

    vacuum_corpus.mensura = 0;
    vacuum_corpus.datum   = NIHIL;

    sprintf(via, "/imperium/%ld", (longus)tessera);

    dum (VERUM)
    {
        PiscinaNotatio nota = piscina_notare(manus->scriptorium);
        ManusResponsum r;
        JsonResultus   j;
        JsonValor*     status;
        b32            perfectum = FALSUM;
        b32            fallitum  = FALSUM;

        /* Fistulas haurire. Valor redditus consulto abicitur:
         * PARATUS solum post exitum infantis redditur, et infans
         * noster currere DEBET. Vocamus propter effectum lateralem
         * - fistula plena scriptorem obstrueret et applicationem
         * viventem mortuam VIDERI faceret. */
        si (manus->processus != NIHIL)
        {
            (vacuum)processus_pulsare(manus->processus);
        }

        r = _petere(manus, HTTP_GET, via, vacuum_corpus, manus->scriptorium);
        si (r.successus && r.status == CC)
        {
            j = json_legere(r.corpus, manus->scriptorium);
            si (j.successus && j.radix != NIHIL)
            {
                status = json_objectum_capere(j.radix, "status");
                si (status != NIHIL && json_est_chorda(status))
                {
                    chorda s = json_ad_chorda(status);
                    si (chorda_aequalis_literis(s, "perfectum"))
                    {
                        JsonValor* v = json_objectum_capere(j.radix, "valor");
                        si (v != NIHIL)
                        {
                            /* Scribitur in scriptorio, SERVATUR in
                             * piscina: hoc transitum facit. */
                            chorda textus = json_scribere(v, manus->scriptorium);
                            *valor    = _servare(textus, manus->piscina);
                            perfectum = VERUM;
                        }
                    }
                    alioquin si (chorda_aequalis_literis(s, "culpa"))
                    {
                        JsonValor* n = json_objectum_capere(j.radix, "nuntius");
                        si (n != NIHIL && json_est_chorda(n))
                        {
                            *culpa = _servare(json_ad_chorda(n), manus->piscina);
                        }
                        fallitum = VERUM;
                    }
                }
            }
        }

        /* Quod _servare transtulit in piscina est; reliqua huius
         * gyri hic moriuntur. */
        piscina_reficere(manus->scriptorium, nota);

        si (perfectum || fallitum)
        {
            redde perfectum;
        }

        si (_nunc() > terminus)
        {
            redde FALSUM;
        }

        _quiescere(intervallum);
        intervallum *= II;
        si (intervallum > MANUS_PULSUS_MAXIMUS)
        {
            intervallum = MANUS_PULSUS_MAXIMUS;
        }
    }
}

/* Iussum unum: JS mittere, valorem JSON exspectare.
 *
 * 'mora_paginae' quantum pagina ipsa exspectare iussa est; hic
 * gratia additur (vide MANUS_GRATIA). */
interior b32
_iussum (
    Manus*   manus,
    chorda   js,
    Mora     mora_paginae,
    chorda*  valor)
{
    s64      tessera;
    Momentum terminus;
    chorda   culpa;

    culpa.mensura = 0;
    culpa.datum   = NIHIL;

    valor->mensura = 0;
    valor->datum   = NIHIL;

    tessera = _tesseram_petere(manus, "/imperium", js);
    si (tessera < 0)
    {
        _frangere(manus, "applicatio iussum non accepit "
                         "(currit? imperium sub -vivum praebetur?)");
        redde FALSUM;
    }

    terminus = _nunc() + mora_paginae + MANUS_GRATIA;
    si (!_tesseram_pulsare(manus, tessera, terminus, valor, &culpa))
    {
        si (culpa.mensura > 0)
        {
            /* JS ipsum iactavit - vitium in selectore aut pagina,
             * non in tempore. */
            _frangere(manus, _litterae(culpa, manus->piscina));
        }
        alioquin
        {
            _frangere(manus, "applicatio non respondit intra terminum "
                             "(haeret, aut gyrus eius hospitium non pulsat)");
        }
        redde FALSUM;
    }
    redde VERUM;
}

/* ========================================================================
 * JS: adiutores communes
 * ======================================================================== */

/* VISIBILE, NON PRAESENS - et haec distinctio consulto sumpta est.
 *
 * 'hidden', 'display:none', pyxis clausa: elementum in DOM MANET.
 * Ergo assertum quod praesentiam solam quaerit VIRIDE fit de re quam
 * usor videre non potest - genus mendacii quod haec bibliotheca
 * tollere debet.
 *
 * MENSURATUM 2026-08-13: probatio experimenti 0001 'forma abiit'
 * asseruit; forma autem tantum occulta erat, et assertum sine
 * visibilitate transiisset. Transitus scaenarum - id ipsum quod
 * probandum erat - probari non poterat.
 *
 * Ergo OMNIA hic (asserta, interrogationes, actiones) idem
 * significant: quod USOR videt. Pro interrogatione crudi DOM adest
 * manus_aestimare.
 *
 * v() = visibile; q() = primum visibile congruens (NIHIL si nullum).
 * Definitio: neque 'display:none' neque 'visibility:hidden', et
 * area non vacua. */
#define MANUS_JS_VISUS \
    "function v(e){if(!e)return false;" \
    "var s=window.getComputedStyle(e);" \
    "if(s.display==='none'||s.visibility==='hidden')return false;" \
    "return !!(e.offsetWidth||e.offsetHeight||e.getClientRects().length);}" \
    "function q(s){var l=document.querySelectorAll(s),i;" \
    "for(i=0;i<l.length;i++){if(v(l[i]))return l[i];}return null;}" \
    "function qn(s){var l=document.querySelectorAll(s),i,n=0;" \
    "for(i=0;i<l.length;i++){if(v(l[i]))n++;}return n;}" \
    MANUS_JS_AGIBILE

/* AGIBILITAS: quod usor facere POTEST, non quod JS facere potest.
 *
 * MENSURATUM in pictore vero 2026-08-13 - duo silentia:
 *   pyxis 'disabled': e.click() nullum eventum parit (ictus 0),
 *     et tamen redit ut si egisset;
 *   pyxis OBTECTA velo pleno: e.click() eventum parit (ictus 2) -
 *     JS enim scrutinium loci non facit, usor autem velum tangeret.
 *
 * Utrumque manus 'pressum' nuntiabat. Duo VIRIDIA MENDACIA: probatio
 * transibat ubi usor omnino agere non posset. Doctrina a Cypress
 * sumpta (actionability), causa nostra propria.
 *
 * Reddit CAUSAM litteralem si non agibile, NIHIL si agibile - ut
 * nuntius fracturae dicat CUR, non solum 'falsum'. Et quia _agere
 * per gyrum exspectationis it, condicio ITERUM ATQUE ITERUM
 * temptatur: pyxis quae post iter pontis expedita fit sponte
 * exspectatur. */
#define MANUS_JS_AGIBILE \
    "function act(e){" \
    "if(!e)return 'nullum elementum visibile';" \
    "if(e.disabled||(e.matches&&e.matches(':disabled')))" \
    "return 'elementum impeditum (disabled)';" \
    "if(e.scrollIntoView)e.scrollIntoView({block:'center'});" \
    "var rc=e.getBoundingClientRect();" \
    "var sup=document.elementFromPoint(rc.left+rc.width/2," \
    "rc.top+rc.height/2);" \
    "if(sup&&sup!==e&&!e.contains(sup)&&!sup.contains(e))" \
    "return 'elementum obtectum a <'+sup.tagName.toLowerCase()+" \
    "(sup.id?'#'+sup.id:'')+'>';" \
    "return null;}" \
    MANUS_JS_TEXTUS

/* qt(t) = elementum IMUM visibile quod textum 't' continet.
 *
 * 'Imum' cardo est: <body> omnia continet, <main> fere omnia. Sine
 * hac lege quaeque petitio textualis corpus inveniret et premere
 * corpus premeret - viride et inutile. Ergo ex congruentibus id
 * eligitur quod nullum aliud congruens INTRA se habet.
 *
 * qtn(t) = num quid omnino congruat (pro asserto textus paginae). */
#define MANUS_JS_TEXTUS \
    "function _tx(e){return (e.value!==undefined&&e.value!==null)" \
    "?String(e.value):String(e.textContent||'');}" \
    "function _tm(t){var l=document.querySelectorAll('*'),i,r=[];" \
    "for(i=0;i<l.length;i++){var e=l[i];" \
    "if(!v(e))continue;if(_tx(e).indexOf(t)<0)continue;r.push(e);}" \
    "return r;}" \
    "function qt(t){var r=_tm(t),i,j,intus;" \
    "for(i=0;i<r.length;i++){intus=false;" \
    "for(j=0;j<r.length;j++){if(j!==i&&r[i].contains(r[j]))" \
    "{intus=true;break;}}" \
    "if(!intus)return r[i];}" \
    "return r.length?r[r.length-1]:null;}" \
    "function qtn(t){return _tm(t).length;}"

/* COLLECTOR ERRORUM - in paginam positus cum manus aperitur.
 *
 * Tres fontes, quia tres semitae DIVERSAE sunt et nulla alteram
 * capit: 'error' exceptiones non captas fert; 'unhandledrejection'
 * promissa reiecta (semita quae in hac domo maxime refert - pons
 * TOTUS promissis agit, et promissum reiectum sine captura nihil
 * omnino in consolam scribit); console.error involutum quod codex
 * ipse nuntiat.
 *
 * Idempotens (vexillum __manus_errores): manus altera eundem
 * collectorem non duplicat.
 *
 * Terminus C nuntiorum: pagina in gyro cadens memoriam aliter
 * exhauriret, et centesimus error primo nihil addit. */
#define MANUS_JS_ERRORES \
    "(function(){if(window.__manus_errores)return 'iam';" \
    "window.__manus_errores=[];" \
    "function n(g,m){var a=window.__manus_errores;" \
    "if(a.length<100)a.push(g+': '+m);}" \
    "window.addEventListener('error',function(ev){" \
    "n('exceptio',(ev.message||String(ev.error))+" \
    "(ev.filename?(' @ '+ev.filename+':'+ev.lineno):''));});" \
    "window.addEventListener('unhandledrejection',function(ev){" \
    "var r=ev.reason;n('promissum reiectum'," \
    "(r&&r.message)?r.message:String(r));});" \
    "var ce=console.error;console.error=function(){" \
    "n('console.error',Array.prototype.slice.call(arguments)" \
    ".join(' '));if(ce)ce.apply(console,arguments);};" \
    "return 'positum';})()"

/* ========================================================================
 * JS: gyrus exspectationis IN PAGINA
 * ======================================================================== */

/* Corpus functionis quod {ok,visum} reddit involvitur promisso quod
 * ipsum tempus gerit. Condicio ~LXII vicibus per secundum
 * aestimatur - independenter a rete, quod est totum consilium
 * (vide manus.h). */
interior chorda
_js_exspectare (
    Manus*              manus,
    constans character* corpus_functionis,
    Mora                mora)
{
    ChordaAedificator* a = chorda_aedificator_creare(manus->piscina, DXII);
    character          numerus[XXXII];

    sprintf(numerus, "%ld", (longus)mora);

    chorda_aedificator_appendere_literis(a, "new Promise(function(R){var d=Date.now()+");
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_literis(a, ";");
    chorda_aedificator_appendere_literis(a, MANUS_JS_VISUS);
    chorda_aedificator_appendere_literis(a, "function f(){");
    chorda_aedificator_appendere_literis(a, corpus_functionis);
    chorda_aedificator_appendere_literis(a,
        "}(function c(){var r=f();"
        "if(r.ok){R(r);return;}"
        "if(Date.now()>d){R(r);return;}"
        "setTimeout(c,16);})();})");

    redde chorda_aedificator_finire(a);
}

/* Selectorem (aut textum quemlibet) ut litteras JS effugere. */
interior vacuum
_appendere_litteras_js (
    ChordaAedificator*  a,
    constans character* s)
{
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_literis_evasus_json(a, s);
    chorda_aedificator_appendere_character(a, '"');
}

/* Verdictum ex valore JSON {ok,visum} legere. */
interior ManusVerdictum
_verdictum (
    Manus* manus,
    chorda valor,
    b32    respondit)
{
    ManusVerdictum v;
    JsonResultus   j;

    v.respondit    = respondit;
    v.ok           = FALSUM;
    v.visum.mensura = 0;
    v.visum.datum   = NIHIL;

    si (!respondit)
    {
        redde v;
    }

    j = json_legere(valor, manus->piscina);
    si (j.successus && j.radix != NIHIL)
    {
        JsonValor* ok    = json_objectum_capere(j.radix, "ok");
        JsonValor* visum = json_objectum_capere(j.radix, "visum");
        si (ok != NIHIL && json_est_boolean(ok))
        {
            v.ok = json_ad_boolean(ok);
        }
        si (visum != NIHIL && json_est_chorda(visum))
        {
            v.visum = json_ad_chorda(visum);
        }
    }
    redde v;
}

interior ManusVerdictum
_exspectare (
    Manus* manus,
    chorda js,
    Mora   mora)
{
    chorda valor;
    b32    respondit = _iussum(manus, js, mora, &valor);
    redde _verdictum(manus, valor, respondit);
}

/* ========================================================================
 * Cyclus vitae
 * ======================================================================== */

/* Applicatio vivit? Responsum QUODLIBET sufficit - etiam CDIV
 * ('tessera ignota') probat hospitium currere et imperium
 * inscriptum esse. */
interior b32
_respondet (
    Manus* manus)
{
    PiscinaNotatio nota = piscina_notare(manus->scriptorium);
    chorda         vacuum_corpus;
    ManusResponsum r;

    vacuum_corpus.mensura = 0;
    vacuum_corpus.datum   = NIHIL;

    r = _petere(manus, HTTP_GET, "/imperium/0", vacuum_corpus,
                manus->scriptorium);
    piscina_reficere(manus->scriptorium, nota);
    redde r.successus;
}

/* PORTUS LIGATUS NON EST PAGINA PARATA.
 *
 * Applicatio hospitium ligat multo ANTE quam vitrea paginam suam
 * onerat - et modo vivo pagina per idem hospitium venit. Inter
 * illud et hoc portus respondet dum pagina non est.
 *
 * Iussum in illam rimam missum in VACUUM abit: in documento
 * inonerato 'internuntius' non est, ergo involucrum imperii iactat;
 * sed semita culpae eius internuntium ipsum vocat, unde nec
 * responsum nec culpa umquam redit. Tessera in AETERNUM pendet -
 * et nihil in applicatione fractum est.
 *
 * Ergo paratitudo iter PLENUM probet, non portum: iussum minimum
 * quod redire DEBET. MENSURATUM 2026-08-13: manus_incipere portu
 * contenta erat, et probatio fumi 'applicatio non respondit' de
 * applicatione perfecte sana nuntiabat. */
interior b32
_paratus (
    Manus* manus)
{
    chorda valor;
    b32    fructus;

    manus->tacens = VERUM;
    fructus = _iussum(manus, chorda_ex_literis("1", manus->piscina),
                      MANUS_MORA_BREVIS, &valor);
    manus->tacens = FALSUM;

    /* Conatus defectus manum non maculet. */
    manus->fracta        = FALSUM;
    manus->causa.mensura = 0;
    manus->causa.datum   = NIHIL;
    redde fructus;
}

/* Collectorem in paginam ponere. Defectus hic manum NON frangit:
 * applicatio sine collectore probabilis manet, et defectus ipse in
 * asserto errorum apparebit. */
interior vacuum
_errores_instituere (
    Manus* manus)
{
    chorda valor;

    manus->tacens = VERUM;
    (vacuum)_iussum(manus,
                    chorda_ex_literis(MANUS_JS_ERRORES, manus->piscina),
                    MANUS_MORA_BREVIS, &valor);
    manus->tacens = FALSUM;
    manus->fracta        = FALSUM;
    manus->causa.mensura = 0;
    manus->causa.datum   = NIHIL;
}

interior Manus*
_manus_creare (
    Piscina*            piscina,
    constans character* hospes,
    i32                 portus)
{
    Manus* manus;
    i32    longitudo;

    si (piscina == NIHIL || hospes == NIHIL)
    {
        redde NIHIL;
    }
    longitudo = (i32)strlen(hospes);
    si (longitudo == 0 || longitudo >= MANUS_HOSPES_MAXIMUS)
    {
        redde NIHIL;
    }

    manus = (Manus*)piscina_allocare(piscina, (memoriae_index)magnitudo(Manus));

    /* Arena manus PROPRIA - unica res quam manus possidet, et ideo
     * unica quam claudere destruere debet. */
    manus->scriptorium = piscina_generare_dynamicum("manus_scriptorium",
                                                    LXIV * M);
    si (manus->scriptorium == NIHIL)
    {
        redde NIHIL;
    }

    manus->piscina   = piscina;
    manus->portus    = portus;
    manus->processus = NIHIL;
    manus->fracta    = FALSUM;
    manus->tacens     = FALSUM;
    manus->in_imagine = FALSUM;
    manus->via_culpae[0] = '\0';
    manus->causa.mensura = 0;
    manus->causa.datum   = NIHIL;
    memcpy(manus->hospes, hospes, (memoriae_index)longitudo);
    manus->hospes[longitudo] = '\0';

    redde manus;
}

Manus*
manus_aperire (
    Piscina*            piscina,
    constans character* hospes,
    i32                 portus)
{
    Manus* manus = _manus_creare(piscina, hospes, portus);

    si (manus == NIHIL)
    {
        redde NIHIL;
    }
    /* NIHIL potius quam manus muta: manus quae nihil tangit omnia
     * asserta silentio praeterire faceret - suita viridis contra
     * applicationem quae numquam cucurrit.
     *
     * Iter PLENUM probatur, non portus (vide _paratus): manus quae
     * portum tangit sed paginam non attingit aeque inutilis est. */
    si (!_respondet(manus) || !_paratus(manus))
    {
        redde NIHIL;
    }
    _errores_instituere(manus);
    redde manus;
}

Manus*
manus_incipere (
    Piscina*                      piscina,
    constans character* constans* argumenta,
    i32                           portus,
    Mora                          mora)
{
    Manus*     manus;
    Processus* processus;
    Momentum   terminus;

    manus = _manus_creare(piscina, "127.0.0.1", portus);
    si (manus == NIHIL || argumenta == NIHIL || argumenta[0] == NIHIL)
    {
        redde NIHIL;
    }

    /* Terminus ZEPHYRUS (infinitus): applicatio sub probatione
     * currere DEBET donec probatio finiat. Occisio manus_claudere
     * pertinet, non horologio processus. */
    processus = processus_incipere(argumenta, 0, piscina);
    si (processus == NIHIL)
    {
        redde NIHIL;
    }
    manus->processus = processus;

    terminus = _nunc() + mora;
    dum (_nunc() <= terminus)
    {
        (vacuum)processus_pulsare(processus);
        si (_respondet(manus) && _paratus(manus))
        {
            _errores_instituere(manus);
            redde manus;
        }
        _quiescere(XX);
    }

    processus_abrumpere(processus);
    redde NIHIL;
}

vacuum
manus_claudere (
    Manus* manus)
{
    si (manus == NIHIL)
    {
        redde;
    }
    si (manus->processus != NIHIL)
    {
        processus_abrumpere(manus->processus);
        manus->processus = NIHIL;
    }
    si (manus->scriptorium != NIHIL)
    {
        piscina_destruere(manus->scriptorium);
        manus->scriptorium = NIHIL;
    }
}

b32
manus_fracta (
    constans Manus* manus)
{
    redde (manus == NIHIL) ? VERUM : manus->fracta;
}

chorda
manus_causa (
    constans Manus* manus)
{
    chorda vacua;
    si (manus != NIHIL)
    {
        redde manus->causa;
    }
    vacua.mensura = 0;
    vacua.datum   = NIHIL;
    redde vacua;
}

/* ========================================================================
 * Actiones
 * ======================================================================== */

chorda
manus_aestimare (
    Manus*              manus,
    constans character* js,
    Mora                mora)
{
    chorda valor;
    chorda vacua;

    vacua.mensura = 0;
    vacua.datum   = NIHIL;

    si (manus == NIHIL || manus->fracta)
    {
        redde vacua;
    }
    si (!_iussum(manus, chorda_ex_literis(js, manus->piscina), mora, &valor))
    {
        redde vacua;
    }
    redde valor;
}

/* ACTIO: unica semita omnium actionum.
 *
 * Actiones tres (premere, scribere, premere_textum) in uno solo
 * differunt - QUOMODO elementum inveniatur:
 *   "q("  selector CSS  -> primum VISIBILE congruens
 *   "qt(" textus        -> elementum IMUM textum ferens
 * Cetera - porta agibilitatis, gyrus exspectationis, nuntius
 * fracturae - eadem sunt.
 *
 * CUR HIC UNA, NON TER: premere_textum olim hanc functionem totam
 * describebat, portam inclusam. Porta ergo bis emittebatur, et
 * actio QUARTA (eligere? purgare? submittere?) tertiam occasionem
 * eam OMITTENDI ferret - silentio, quia actio sine porta
 * perfecte operatur donec elementum impeditum aut obtectum
 * occurrat. Nunc omittere eam impossibile est: quicumque 'opus'
 * tradit portam iam supra se habet.
 *
 * Elementum EXSPECTATUR: clic in id quod nondum apparuit vitium
 * temporis est - id ipsum quod haec bibliotheca tollere debet. */
interior b32
_agere (
    Manus*              manus,
    constans character* resolutor,   /* "q(" aut "qt(" */
    constans character* argumentum,  /* selector aut textus */
    constans character* opus,
    constans character* nomen_actionis)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, "var e=");
    chorda_aedificator_appendere_literis(a, resolutor);
    _appendere_litteras_js(a, argumentum);
    /* Porta agibilitatis ANTE opus. Causa reddita NOMINATUR, ergo
     * 'pyxis impedita' a 'pyxis abest' et ab 'pyxis obtecta'
     * distinguitur - tria vitia valde diversa quae omnia olim
     * 'pressum' nuntiabant. */
    chorda_aedificator_appendere_literis(a, ");var c=act(e);"
        "if(c)return{ok:false,visum:c};");
    chorda_aedificator_appendere_literis(a, opus);

    v = _exspectare(manus,
                    _js_exspectare(manus, (constans character*)
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        MANUS_MORA_ORDINARIA),
                    MANUS_MORA_ORDINARIA);

    si (!v.respondit)
    {
        redde FALSUM;   /* _iussum iam fregit */
    }
    si (!v.ok)
    {
        ChordaAedificator* n = chorda_aedificator_creare(manus->piscina, CXXVIII);
        chorda_aedificator_appendere_literis(n, nomen_actionis);
        chorda_aedificator_appendere_literis(n, " fefellit: ");
        chorda_aedificator_appendere_literis(n, argumentum);
        chorda_aedificator_appendere_literis(n, " - ");
        chorda_aedificator_appendere_chorda(n, v.visum);
        _frangere(manus, _litterae(chorda_aedificator_finire(n), manus->piscina));
        redde FALSUM;
    }
    redde VERUM;
}

b32
manus_premere (
    Manus*              manus,
    constans character* selector)
{
    redde _agere(manus, "q(", selector,
                 "e.click();return{ok:true,visum:\"pressum\"};",
                 "manus_premere");
}

b32
manus_premere_textum (
    Manus*              manus,
    constans character* textus)
{
    /* Sola differentia a manus_premere: qt() pro q(). Cetera -
     * porta, mora, nuntius - ex _agere veniunt. */
    redde _agere(manus, "qt(", textus,
                 "e.click();return{ok:true,visum:\"pressum\"};",
                 "manus_premere_textum");
}

b32
manus_scribere (
    Manus*              manus,
    constans character* selector,
    constans character* textus)
{
    ChordaAedificator* a;
    b32                fructus;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    /* Eventus 'input'/'change' manu immittendi sunt: assignatio
     * .value eos NON parit, et facies reactiva quae eos audit
     * nihil omnino sentiret. */
    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, "e.value=");
    _appendere_litteras_js(a, textus);
    chorda_aedificator_appendere_literis(a,
        ";e.dispatchEvent(new Event('input',{bubbles:true}));"
        "e.dispatchEvent(new Event('change',{bubbles:true}));"
        "return{ok:true,visum:e.value};");

    fructus = _agere(manus, "q(", selector,
                     _litterae(chorda_aedificator_finire(a), manus->piscina),
                     "manus_scribere");
    redde fructus;
}

/* Captura nuda: nec statum manus inspicit nec frangit. Necessaria
 * quia _frangere ipsum eam vocat - manus iam fracta imaginem suam
 * capere DEBET, quod publica forma (quae fractam recusat) vetaret. */
interior b32
_imaginem (
    Manus*              manus,
    constans character* via,
    Mora                mora,
    constans character** causa_out)
{
    s64      tessera;
    Momentum terminus;
    chorda   valor;
    chorda   culpa;

    valor.mensura = 0;
    valor.datum   = NIHIL;
    culpa.mensura = 0;
    culpa.datum   = NIHIL;
    *causa_out    = NIHIL;

    tessera = _tesseram_petere(manus, "/imperium/imago",
                               chorda_ex_literis(via, manus->piscina));
    si (tessera < 0)
    {
        *causa_out = "manus_imaginem: applicatio imaginatorem non praebuit "
                     "(imperium_imaginatorem_ponere vocatum est?)";
        redde FALSUM;
    }

    terminus = _nunc() + mora;
    si (!_tesseram_pulsare(manus, tessera, terminus, &valor, &culpa))
    {
        *causa_out = "manus_imaginem: imago non scripta";
        redde FALSUM;
    }
    redde VERUM;
}

b32
manus_imaginem (
    Manus*              manus,
    constans character* via)
{
    constans character* causa = NIHIL;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    si (!_imaginem(manus, via, MANUS_MORA_LONGA, &causa))
    {
        _frangere(manus, causa);
        redde FALSUM;
    }
    redde VERUM;
}

vacuum
manus_imaginem_culpae_ponere (
    Manus*              manus,
    constans character* via)
{
    i32 longitudo;

    si (manus == NIHIL)
    {
        redde;
    }
    si (via == NIHIL)
    {
        manus->via_culpae[0] = '\0';
        redde;
    }
    longitudo = (i32)strlen(via);
    si (longitudo >= MANUS_VIA_MAXIMA)
    {
        longitudo = MANUS_VIA_MAXIMA - I;
    }
    memcpy(manus->via_culpae, via, (memoriae_index)longitudo);
    manus->via_culpae[longitudo] = '\0';
}

/* ========================================================================
 * Exploratio - TACITA
 *
 * Condicio falsa manum NON frangit: interrogatio est, non assertum.
 * Applicatio muta tamen frangit - id enim non est responsum
 * 'falsum', sed nullum responsum.
 * ======================================================================== */

interior ManusVerdictum
_interrogare (
    Manus*              manus,
    constans character* corpus_functionis)
{
    ChordaAedificator* a;
    chorda             js;
    chorda             valor;
    b32                respondit;
    ManusVerdictum     mutum;

    mutum.respondit     = FALSUM;
    mutum.ok            = FALSUM;
    mutum.visum.mensura = 0;
    mutum.visum.datum   = NIHIL;

    si (manus == NIHIL || manus->fracta)
    {
        redde mutum;
    }

    /* Sine promisso: interrogatio statim respondet. */
    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, "(function(){");
    chorda_aedificator_appendere_literis(a, MANUS_JS_VISUS);
    chorda_aedificator_appendere_literis(a, corpus_functionis);
    chorda_aedificator_appendere_literis(a, "})()");
    js = chorda_aedificator_finire(a);

    respondit = _iussum(manus, js, MANUS_MORA_BREVIS, &valor);
    redde _verdictum(manus, valor, respondit);
}

b32
manus_existit (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var n=qn(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");return{ok:n>0,visum:String(n)};");

    v = _interrogare(manus,
                     _litterae(chorda_aedificator_finire(a), manus->piscina));
    redde v.ok;
}

i32
manus_numerus (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;
    ManusVerdictum     v;
    s32                n = 0;

    si (manus == NIHIL || manus->fracta)
    {
        redde 0;
    }
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var n=qn(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");return{ok:true,visum:String(n)};");

    v = _interrogare(manus,
                     _litterae(chorda_aedificator_finire(a), manus->piscina));
    si (!v.respondit || !chorda_ut_s32(v.visum, &n))
    {
        redde 0;
    }
    redde (i32)n;
}

chorda
manus_textus (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;
    ManusVerdictum     v;
    chorda             vacua;

    vacua.mensura = 0;
    vacua.datum   = NIHIL;

    si (manus == NIHIL || manus->fracta)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var e=q(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");if(!e)return{ok:false,visum:\"\"};"
        "return{ok:true,visum:String(e.value!==undefined?e.value:e.textContent)};");

    v = _interrogare(manus,
                     _litterae(chorda_aedificator_finire(a), manus->piscina));
    si (!v.ok)
    {
        redde vacua;
    }
    redde v.visum;
}

/* ========================================================================
 * Asserta
 * ======================================================================== */

/* Fractum notare cum AMBOBUS lateribus. Assertum quod 'falsum'
 * dicit et tacet horam furatur - ergo hic semper et exspectatum et
 * visum nominantur. */
interior b32
_notare (
    Manus*              manus,
    constans character* genus,
    chorda              expressio,
    constans character* exspectatum,
    ManusVerdictum      v,
    constans character* filum,
    s32                 versus)
{
    constans character* visum;

    si (!v.respondit)
    {
        /* Applicatio muta: causa iam in manu est, et eam nominare
         * utilius est quam 'visum: ""'. */
        visum = _litterae(manus->causa, manus->piscina);
    }
    alioquin
    {
        visum = _litterae(v.visum, manus->piscina);
    }

    /* _credo_notare, NON _credo_notare_chorda: illud conditionem
     * EXPLICITAM accipit, hoc duos valores inter se comparat (est
     * enim primitivum CREDO_CHORDAE_AEQUALES). Cum illo usus eram:
     * "1" contra "I aut plura elementa" numquam par est, ergo omne
     * assertum fallebat quidquid pagina dixisset - et semitae
     * FALLENTES id celabant, quia fallere iam debebant.
     *
     * ORDO: valor_primus 'Receptus' imprimitur, secundus
     * 'Speratus' - actuale ANTE exspectatum. */
    _credo_notare(
        genus,
        _litterae(expressio, manus->piscina),
        visum,
        exspectatum,
        filum,
        versus,
        v.ok);

    si (!v.ok)
    {
        _frangere(manus, "assertum fractum");
    }
    redde v.ok;
}

/* Descriptio pro compendio credo: 'manus[".tessera"]'. */
interior chorda
_expressio (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "manus[");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a, "]");
    redde chorda_aedificator_finire(a);
}

b32
_manus_credo_existere (
    Manus*              manus,
    constans character* selector,
    b32                 adesse,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var n=qn(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a, ");return{ok:n");
    chorda_aedificator_appendere_literis(a, adesse ? ">0" : "===0");
    chorda_aedificator_appendere_literis(a, ",visum:String(n)};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    redde _notare(manus,
                  adesse ? "credo_manus_existit" : "credo_manus_abest",
                  _expressio(manus, selector),
                  adesse ? "I aut plura elementa" : "nulla elementa",
                  v, filum, versus);
}

b32
_manus_credo_textum (
    Manus*              manus,
    constans character* selector,
    constans character* exspectatum,
    b32                 pars_sufficit,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, "var e=q(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");if(!e)return{ok:false,visum:\"(nullum elementum)\"};"
        "var v=String(e.value!==undefined?e.value:e.textContent);return{ok:");
    si (pars_sufficit)
    {
        chorda_aedificator_appendere_literis(a, "v.indexOf(");
        _appendere_litteras_js(a, exspectatum);
        chorda_aedificator_appendere_literis(a, ")>=0");
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, "v===");
        _appendere_litteras_js(a, exspectatum);
    }
    chorda_aedificator_appendere_literis(a, ",visum:v};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    redde _notare(manus,
                  pars_sufficit ? "credo_manus_textus_continet"
                                : "credo_manus_textus",
                  _expressio(manus, selector),
                  exspectatum, v, filum, versus);
}

b32
_manus_credo_numerum (
    Manus*              manus,
    constans character* selector,
    i32                 quot,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    ChordaAedificator* a;
    ManusVerdictum     v;
    character          numerus[XXXII];

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    sprintf(numerus, "%lu", (insignatus longus)quot);

    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var n=qn(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a, ");return{ok:n===");
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_literis(a, ",visum:String(n)};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    redde _notare(manus, "credo_manus_numerus",
                  _expressio(manus, selector),
                  numerus, v, filum, versus);
}

/* ========================================================================
 * Textus paginae, praesentia cruda, errores
 * ======================================================================== */

b32
_manus_credo_textum_paginae (
    Manus*              manus,
    constans character* textus,
    b32                 adesse,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, "var n=qtn(");
    _appendere_litteras_js(a, textus);
    chorda_aedificator_appendere_literis(a, ");return{ok:n");
    chorda_aedificator_appendere_literis(a, adesse ? ">0" : "===0");
    chorda_aedificator_appendere_literis(a, ",visum:String(n)};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "pagina continet \"");
    chorda_aedificator_appendere_literis(a, textus);
    chorda_aedificator_appendere_literis(a, "\"");

    redde _notare(manus,
                  adesse ? "credo_manus_textum" : "credo_manus_textum_abest",
                  chorda_aedificator_finire(a),
                  adesse ? "I aut plura elementa visibilia"
                         : "nulla elementa visibilia",
                  v, filum, versus);
}

b32
_manus_credo_omnino (
    Manus*              manus,
    constans character* selector,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    /* querySelectorAll CRUDUM consulto - hoc solum assertum de DOM
     * ipso loquitur, non de eo quod usor videt. */
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a,
        "var n=document.querySelectorAll(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ").length;return{ok:n===0,visum:String(n)};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    redde _notare(manus, "credo_manus_abest_omnino",
                  _expressio(manus, selector),
                  "nulla elementa in DOM (ne occulta quidem)",
                  v, filum, versus);
}

i32
manus_errores (
    Manus*  manus,
    chorda* primus)
{
    ManusVerdictum v;
    s32            quot = 0;

    si (primus != NIHIL)
    {
        primus->mensura = 0;
        primus->datum   = NIHIL;
    }
    si (manus == NIHIL || manus->fracta)
    {
        redde 0;
    }

    /* 'visum' primum nuntium fert, 'ok' numerum nullum esse. Ergo
     * una interrogatio et numerum et causam reddit - nam numerus
     * sine nuntio nihil docet. */
    v = _interrogare(manus,
        "var a=window.__manus_errores;"
        "if(!a)return{ok:true,visum:\"-1|collector abest\"};"
        "return{ok:a.length===0,visum:String(a.length)+\"|\"+"
        "(a.length?a[0]:\"\")};");

    si (!v.respondit || v.visum.mensura == 0)
    {
        redde 0;
    }
    {
        i32 i = 0;
        dum (i < v.visum.mensura && v.visum.datum[i] != '|')
        {
            i++;
        }
        {
            chorda numerus;
            numerus.datum   = v.visum.datum;
            numerus.mensura = i;
            si (!chorda_ut_s32(numerus, &quot))
            {
                quot = 0;
            }
        }
        si (primus != NIHIL && i + I < v.visum.mensura)
        {
            primus->datum   = v.visum.datum + i + I;
            primus->mensura = v.visum.mensura - (i + I);
        }
    }
    si (quot < 0)
    {
        redde 0;   /* collector abest - nihil de erroribus dicere possumus */
    }
    redde (i32)quot;
}

vacuum
manus_errores_purgare (
    Manus* manus)
{
    si (manus == NIHIL || manus->fracta)
    {
        redde;
    }
    /* 'length = 0' MUTAT acervum in loco - non novum ponit. Custodes
     * enim ad ILLUM acervum iam ligati sunt; eum reponere illos ad
     * relictum scribere sineret, et errores postea deleti
     * viderentur. */
    (vacuum)_interrogare(manus,
        "var a=window.__manus_errores;"
        "if(a)a.length=0;"
        "return{ok:true,visum:\"purgatum\"};");
}

b32
_manus_credo_sine_erroribus (
    Manus*              manus,
    constans character* filum,
    s32                 versus)
{
    chorda    primus;
    i32       quot;
    character numerus[XXXII];

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    quot = manus_errores(manus, &primus);

    sprintf(numerus, "%lu", (insignatus longus)quot);
    {
        ChordaAedificator* a = chorda_aedificator_creare(manus->piscina, CCLVI);
        chorda_aedificator_appendere_literis(a, numerus);
        si (primus.mensura > 0)
        {
            chorda_aedificator_appendere_literis(a, " (primus: ");
            chorda_aedificator_appendere_chorda(a, primus);
            chorda_aedificator_appendere_literis(a, ")");
        }
        _credo_notare("credo_manus_sine_erroribus",
                      "errores paginae",
                      _litterae(chorda_aedificator_finire(a), manus->piscina),
                      "0",
                      filum, versus, quot == 0);
    }
    si (quot != 0)
    {
        _frangere(manus, "pagina errores iactavit");
    }
    redde (quot == 0);
}
