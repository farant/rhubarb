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
    /* _nz: spatia COGERE ante comparationem.
     *
     * MENSURATUM 2026-08-13 (agens usor-probans): petitio
     * 'praevolatus 75.0s' fefellit dum textus in schermo EXACTE
     * ille esset. Causa: pagina 'nomen + "  " + valor' scribit -
     * DUO spatia - et navigatrum ea in UNUM cogit cum pingit.
     * Usor unum videt; textContent duo fert.
     *
     * Idem omni marcatione indentata accidit: '<button>\n  Salve\n
     * </button>' lineas et spatia fert quae nemo videt.
     *
     * Ergo utrumque latus cogitur. Petitio quod usor LEGIT
     * describit, non quod fons scribit - quae est tota ratio
     * petitionis textualis.
     *
     * (Agens 'nodos fratres' causam esse coniecit; verum latius
     *  est - spatia, non structura.) */ \
    "function _nz(s){s=String(s==null?'':s);" \
    "var o='',p=true,i,c;" \
    "for(i=0;i<s.length;i++){c=s.charAt(i);" \
    "if(c<=' '){if(!p){o+=' ';p=true;}}else{o+=c;p=false;}}" \
    "if(o.charAt(o.length-1)===' ')o=o.substring(0,o.length-1);" \
    "return o;}" \
    /* innerText ubi adest (visibilitatem servat; SVG eum non habet,
     * unde textContent superest). */ \
    /* VALOR SOLIS CAMPIS, non omnibus quod '.value' habent.
     *
     * MENSURATUM 2026-08-14 in laboratorio: '<button>' '.value'
     * HABET, et vacuam ('') nisi attributum ponas. Ergo _tx pro
     * OMNI pyxide '' reddebat, et inde DUO mendacia tacita:
     *   - 'manus textus <pyxis>' vacuum reddebat (responsum falsum,
     *     nullo signo)
     *   - 'premere-textum "condere"' pyxidem congruere NON poterat,
     *     ergo qt() FIELDSET ambientem eligebat (cuius innerText
     *     textum continet), eum premebat, et 'pressum' nuntiabat.
     *     Exitus ZEPHYRUM, nihil actum - defectus ille ipse quem
     *     haec bibliotheca tollere debet.
     *
     * Campi soli (INPUT/TEXTAREA/SELECT) valorem ut contentum
     * VISIBILE habent; ceteris textus visibilis est, et '.value'
     * res interna formae quam nemo videt. */ \
    "function _tx(e){var g=e.tagName;" \
    "if((g==='INPUT'||g==='TEXTAREA'||g==='SELECT')" \
    "&&e.value!==undefined&&e.value!==null)" \
    "return String(e.value);" \
    "return String((e.innerText===undefined||e.innerText===null)" \
    "?(e.textContent||''):e.innerText);}" \
    /* Petitio vacua NIHIL congruit, non OMNIA. Sine hac linea
     * asserta textus paginae de chorda vacua VIRIDIA fierent. */ \
    "function _tm(t){var l=document.querySelectorAll('*'),i,r=[]," \
    "q=_nz(t);if(q==='')return r;" \
    "for(i=0;i<l.length;i++){var e=l[i];" \
    "if(!v(e))continue;if(_nz(_tx(e)).indexOf(q)<0)continue;r.push(e);}" \
    "return r;}" \
    "function qt(t){var r=_tm(t),i,j,intus;" \
    "for(i=0;i<r.length;i++){intus=false;" \
    "for(j=0;j<r.length;j++){if(j!==i&&r[i].contains(r[j]))" \
    "{intus=true;break;}}" \
    "if(!intus)return r[i];}" \
    "return r.length?r[r.length-1]:null;}" \
    "function qtn(t){return _tm(t).length;}"

/* ENUMERATIO AFFORDANTIARUM - vide manus.h pro ratione.
 *
 * v() ADHIBETUR, non definitur: lex cohaerentiae poscit ut index
 * eandem visibilitatem intellegat quam actiones. Si hic aliam
 * definitionem scriberem, index affordantias ferret quas manus
 * premere recusaret - et divergentia tacite cresceret.
 *
 * IMPEDIMENTUM: iudicium act() supra RECIPITUR quantum purum est.
 * 'disabled' idem est verbatim. Obtectio autem scrollIntoView
 * poscit ut vera sit, quod paginam moveret - ergo extra prospectum
 * NON iudicamus (vacuum reddimus) et actio ipsa, quae volvere
 * DEBET, rursus iudicabit. Melius est tacere quam divinare.
 *
 * SELECTOR: '#id' si adest (stabilis), aliter semita nth-of-type
 * usque ad primum maiorem cum id, aut ad corpus. Semita fragilis
 * est si pagina mutatur - sed hic index INSTANTANEUS est, non
 * conditus, ergo fragilitas nullum tempus habet ad nocendum. */
/* FOCUS EX CLICU - quid clicus VERUS focet.
 *
 * MENSURATUM 2026-08-15 in laboratorio vivo, eventibus NATIVIS per
 * /imperium/mus missis (non fictis - fictus hic nihil probaret, quia
 * ipsa quaestio est quid textura ex eventu VERO faciat):
 *
 *   campus textus     -> FOCATUR
 *   textarea          -> FOCATUR
 *   contenteditable   -> FOCATUR
 *   pyxis (button)    -> NON focatur
 *   nexus (a)         -> NON focatur
 *   capsula (checkbox)-> NON focatur
 *   radius (radio)    -> NON focatur
 *
 * Custodiae: elementFromPoint quodque punctum ad elementum
 * intentum resolvit; pagina inter probationes intacta mansit.
 * 'select' NON probatum est - clicus nativus menu nativum aperit,
 * quod gyrum OBSTRUIT; regula infra eum inter regimina numerat.
 *
 * Haec mos macOS est (focus ex clicu solis superficiebus SCRIBENDI
 * datur), non casus. Ergo 'premere' eam sequitur: superficies
 * scribendi focat, regimina non. Si omnia focaremus, duo mala:
 * fidem MINUEREMUS in ipso systemate in quo currimus, et anulum
 * foci in imagines induceremus quas nulla regula ':focus' huius
 * domus exspectat (tres solae adsunt, omnes campos textus solos
 * petentes - MENSURATUM).
 *
 * ANTE 'click', non post: clicus verus in mousedown focat, eventum
 * 'click' in mouseup mittit. Auditor qui activeElement legit
 * elementum IAM focatum videre debet. */
#define MANUS_JS_FOCARE \
    "function _fscr(e){" \
    "var g=String(e.tagName||'').toUpperCase(),y;" \
    "if(e.isContentEditable)return true;" \
    "if(g==='TEXTAREA')return true;" \
    "if(g!=='INPUT')return false;" \
    "y=String(e.type||'text').toLowerCase();" \
    "return !(y==='button'||y==='submit'||y==='reset'" \
    "||y==='checkbox'||y==='radio'||y==='image'||y==='file'" \
    "||y==='hidden'||y==='color'||y==='range');}" \
    "if(_fscr(e)&&typeof e.focus==='function'){e.focus();}"

/* ACTIO PREMENDI - UNA forma, duo verba (premere, premere-textum).
 *
 * Prius bis scripta erat, verbatim. Nihil aliud quam tempus opus
 * erat ut discreparent - id ipsum quod _tx fecit (vide
 * manus_textus infra: duo exemplaria de <select> iam dissentiebant
 * antequam quisquam animadverteret). Focus additus utrique
 * simul additur quia forma UNA est. */
#define MANUS_JS_PREMERE \
    MANUS_JS_FOCARE \
    "if(typeof e.click==='function'){e.click();}" \
    /* SVG 'click' non habet (HTMLElement solus): elementa \
     * picturae - virgae flammae, puncta tendentiae - aliter \
     * omnino premi non possent. Eventus VERUS mittitur, ergo \
     * pagina eum eodem modo audit. */ \
    "else{e.dispatchEvent(new MouseEvent('click'," \
    "{bubbles:true,cancelable:true,view:window}));}" \
    "return{ok:true,visum:\"pressum\"};"

/* SEMITA ET TITULUS - communia inter 'affordantiae' et 'focus'.
 *
 * SEORSUM STANT, non bis scripta, et hoc caput causam portat: _tx
 * bis scriptum est (vide manus_textus infra) et duo exemplaria IAM
 * DISCREPABANT de <select> antequam quisquam animadverteret. Verbum
 * 'focus' selectorem eiusdem formae reddere DEBET quam affordantiae
 * imprimunt - aliter nomen quod unum instrumentum dat alteri ignotum
 * est, quod totam rem inutilem facit. Forma una ex fonte uno venit.
 *
 * _esc/_via: selector stabilis ('#id' si adest, aliter semita
 * nth-of-type). _sp/_lab: titulus quem HOMO videt. */
#define MANUS_JS_SEMITA \
    "function _esc(s){return (window.CSS&&CSS.escape)?CSS.escape(s):s;}" \
    "function _via(e){if(e.id)return '#'+_esc(e.id);" \
    "var p=[],n,s;" \
    "while(e&&e.nodeType===1&&e!==document.body){" \
    "if(e.id){p.unshift('#'+_esc(e.id));break;}" \
    "n=1;s=e.previousElementSibling;" \
    "for(;s;s=s.previousElementSibling){if(s.tagName===e.tagName)n++;}" \
    "p.unshift(e.tagName.toLowerCase()+':nth-of-type('+n+')');" \
    "e=e.parentNode;}" \
    "return p.join('>');}" \
    "function _sp(t){var o='',pr=true,i,c;t=String(t||'');" \
    "for(i=0;i<t.length;i++){c=t.charAt(i);" \
    "if(c<=' '){if(!pr){o+=' ';pr=true;}}else{o+=c;pr=false;}}" \
    "if(o.charAt(o.length-1)===' ')o=o.substring(0,o.length-1);" \
    "return o.length>80?o.substring(0,80):o;}" \
    "function _lab(e){var t=e.getAttribute?" \
    "(e.getAttribute('aria-label')||''):'';" \
    "if(!t&&e.labels&&e.labels.length)t=e.labels[0].textContent||'';" \
    "if(!t)t=e.placeholder||'';" \
    "if(!t&&e.tagName!=='INPUT'&&e.tagName!=='SELECT'" \
    "&&e.tagName!=='TEXTAREA')t=e.textContent||'';" \
    "if(!t)t=e.name||'';" \
    "return _sp(t);}"

#define MANUS_JS_AFFORDANTIAE \
    "var S='a[href],button,input,textarea,select,summary," \
    "[role=\"button\"],[role=\"link\"],[contenteditable=\"true\"]';" \
    MANUS_JS_SEMITA \
    "function _gen(e){var g=e.tagName.toLowerCase()," \
    "y=(e.type||'').toLowerCase(),r;" \
    "if(g==='select')return 3;" \
    "if(g==='textarea')return 2;" \
    "if(g==='input'){if(y==='checkbox'||y==='radio')return 4;" \
    "if(y==='submit'||y==='button'||y==='reset')return 1;" \
    "if(y==='hidden')return 0;return 2;}" \
    "if(g==='a'||g==='button'||g==='summary')return 1;" \
    "r=(e.getAttribute&&e.getAttribute('role'))||'';" \
    "if(r==='button'||r==='link')return 1;" \
    "if(e.isContentEditable)return 2;" \
    "return 0;}" \
    "function _imp(e,rc){" \
    "if(e.disabled||(e.matches&&e.matches(':disabled')))" \
    "return 'elementum impeditum (disabled)';" \
    "if(rc.bottom<0||rc.right<0||rc.top>(window.innerHeight||0)" \
    "||rc.left>(window.innerWidth||0))return '';" \
    "var sup=document.elementFromPoint(rc.left+rc.width/2," \
    "rc.top+rc.height/2);" \
    "if(sup&&sup!==e&&!e.contains(sup)&&!sup.contains(e))" \
    "return 'elementum obtectum a <'+sup.tagName.toLowerCase()+" \
    "(sup.id?'#'+sup.id:'')+'>';" \
    "return '';}" \
    "var l=document.querySelectorAll(S),i,e,g,rc,r=[];" \
    "for(i=0;i<l.length;i++){e=l[i];if(!v(e))continue;" \
    "g=_gen(e);if(!g)continue;rc=e.getBoundingClientRect();" \
    "r.push({genus:g,selector:_via(e),titulus:_lab(e)," \
    "valor:(e.value===undefined||e.value===null)?'':String(e.value)," \
    "impedimentum:_imp(e,rc),x:Math.round(rc.left)," \
    "y:Math.round(rc.top),latitudo:Math.round(rc.width)," \
    "altitudo:Math.round(rc.height)});}" \
    "return r;"

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

b32
manus_reficere (
    Manus* manus)
{
    si (manus == NIHIL)
    {
        redde FALSUM;
    }

    /* Iter PLENUM probare ANTE refectionem - et 'tacens' hic
     * necessarium est, quia manus adhuc FRACTA est et sonda per
     * _iussum it, quod aliter causam ORIGINALEM superscriberet
     * antequam sciremus an reficienda sit. */
    manus->tacens = VERUM;
    {
        chorda valor;
        b32    vivit = _iussum(manus,
                               chorda_ex_literis("1", manus->piscina),
                               MANUS_MORA_BREVIS, &valor);
        manus->tacens = FALSUM;
        si (!vivit)
        {
            /* Cadaver non reficitur: causa prima manet, et manus
             * fracta manet ut sectiones sequentes TACEANT - quod
             * hic rectum est. */
            redde FALSUM;
        }
    }

    manus->fracta        = FALSUM;
    manus->causa.mensura = 0;
    manus->causa.datum   = NIHIL;

    /* Acervus errorum quoque purgandus: terminus sectionis utrumque
     * vult, et qui reficere meminit purgare oblivisceretur. */
    manus_errores_purgare(manus);
    redde VERUM;
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
    /* ABSENTIA ANTE AGIBILITATEM, et suo nuntio.
     *
     * Prius act(NIHIL) 'nullum elementum visibile' pro utroque
     * itinere reddebat. Pro petitione TEXTUALI id fallit: agens
     * probans nuntium 'praevolatus 75.0s - nullum elementum
     * visibile' accepit dum textus in schermo PLANE esset, et
     * intellexit 'hoc in pagina non est' - cum vera causa esset
     * 'nullus textus ita scriptus'. Duo genera defectus unum
     * nomen ferebant. */
    chorda_aedificator_appendere_literis(a, ");if(!e)return{ok:false,visum:");
    _appendere_litteras_js(a,
        (resolutor[0] == 'q' && resolutor[1] == 't')
        ? "nullum elementum VISIBILE hunc textum fert (spatia coacta"
          " comparantur; vide 'affordantiae')"
        : "nullum elementum VISIBILE huic selectori congruit");
    chorda_aedificator_appendere_literis(a, "};");

    /* Porta agibilitatis ANTE opus. Causa reddita NOMINATUR, ergo
     * 'pyxis impedita' a 'pyxis abest' et ab 'pyxis obtecta'
     * distinguitur - tria vitia valde diversa quae omnia olim
     * 'pressum' nuntiabant. */
    chorda_aedificator_appendere_literis(a, "var c=act(e);"
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
    redde _agere(manus, "q(", selector, MANUS_JS_PREMERE,
                 "manus_premere");
}

b32
manus_premere_textum (
    Manus*              manus,
    constans character* textus)
{
    /* PETITIO VACUA RECUSATUR.
     *
     * MENSURATUM in me ipso 2026-08-13: chorda vacua OMNIBUS
     * congruit, qt() imum eligit, et 'premere-textum ""' corpus
     * pressit exitu ZEPHYRO. Bis hodie viridem vacuum ita peperi -
     * aestimare meum 'null' reddidit, quod huc traditum 'operatum
     * est'. Culpam meam esse credidi; dimidium instrumenti fuit,
     * quod petitionem sine sensu admisit.
     *
     * Spatia sola idem faciunt (post _nz vacua fiunt), ergo hic
     * quoque reiciuntur. */
    si (textus != NIHIL)
    {
        constans character* p = textus;

        dum (*p != '\0' && *p <= ' ')
        {
            p++;
        }
        si (*p == '\0')
        {
            _frangere(manus,
                "petitio textualis VACUA - quidlibet congrueret");
            redde FALSUM;
        }
    }

    /* Sola differentia a manus_premere: qt() pro q(). Cetera -
     * porta, mora, nuntius, ACTIO IPSA - communia sunt. */
    redde _agere(manus, "qt(", textus, MANUS_JS_PREMERE,
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
     * nihil omnino sentiret.
     *
     * ASSIGNATIO PROBANDA EST - vitium MENSURATUM 2026-08-13 ab
     * agente qui applicationem ut usor tractabat:
     *
     *   bin/manus scribere '#comparanda' '1 · 64d3dbf0'
     *   exit=0, et NIHIL mutatum est.
     *
     * '.value' in <select> optionem CONGRUENTEM quaerit; chorda
     * quae nulli optioni par est TACITE abicitur. Prior forma
     * 'ok:true' semper reddebat - viride mendax, genus pessimum.
     *
     * Ego IPSE eandem functionem hodie 'operari' demonstravi, sed
     * VALOREM optionis tradideram quem prius per aestimare
     * extraxeram - probatio privilegiata, quae scientiam
     * postulabat quam instrumentum non ostendit.
     *
     * Ergo TRIA: (1) <select> et per valorem et per TEXTUM VISIBILEM
     * congruit - textus enim est quod usor legit; (2) elementum sine
     * .value RECUSAT potius quam nihil agit; (3) post assignationem
     * valor RELECTUS est - campus numericus qui litteras reicit, aut
     * maxlength qui truncat, eodem silentio mentiretur. */
    a = chorda_aedificator_creare(manus->piscina, DXII);
    chorda_aedificator_appendere_literis(a, "var T=");
    _appendere_litteras_js(a, textus);
    chorda_aedificator_appendere_literis(a,
        ";function _t(s){s=String(s==null?'':s);"
        "var i=0,j=s.length;"
        "while(i<j&&s.charAt(i)<=' ')i++;"
        "while(j>i&&s.charAt(j-1)<=' ')j--;"
        "return s.substring(i,j);}"
        "if(e.tagName==='SELECT'){"
        "var o=null,i,d=[];"
        "for(i=0;i<e.options.length;i++){d.push(_t(e.options[i].text));}"
        "for(i=0;i<e.options.length&&!o;i++){"
        "if(e.options[i].value===T)o=e.options[i];}"
        "for(i=0;i<e.options.length&&!o;i++){"
        "if(_t(e.options[i].text)===_t(T))o=e.options[i];}"
        "if(!o)return{ok:false,visum:\"nulla optio congruit; praesto: \""
        "+d.join(\" | \")};"
        "e.selectedIndex=o.index;"
        "}else if(e.value===undefined||e.value===null){"
        "return{ok:false,visum:'<'+e.tagName.toLowerCase()+"
        "'> valorem non habet - scribere hic nihil ageret'};"
        "}else{e.value=T;}"
        "e.dispatchEvent(new Event('input',{bubbles:true}));"
        "e.dispatchEvent(new Event('change',{bubbles:true}));"
        "if(e.tagName!=='SELECT'&&String(e.value)!==String(T)){"
        "return{ok:false,visum:'valor non mansit: petitum \"'+T+"
        "'\", remansit \"'+e.value+'\"'};}"
        "return{ok:true,visum:String(e.value)};");

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
manus_movere (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    si (selector == NIHIL || selector[0] == '\0')
    {
        _frangere(manus, "manus_movere: selector VACUUS");
        redde FALSUM;
    }

    /* EVENTUS JS, NON NATIVI - et hoc consulto (2026-08-15).
     *
     * QUOD AMITTITUR: CSS ':hover' non congruet. Eventus per
     * dispatchEvent missus isTrusted=false fert, et textura eum ad
     * statum ':hover' non admittit. Eadem est limitatio quam Cypress
     * fatetur (nullum '.hover()' habet, eandem ob causam).
     *
     * QUOD NON AMITTITUR, et cur hoc satis est: regulae ':hover'
     * huius domus OMNES ornatoriae sunt - color limbi, fundus,
     * linea subducta. MENSURATUM in foro, villa, mensore: nulla
     * regula 'display', 'visibility', 'opacity' mutat. Nihil
     * APPARET quod ante latebat, ergo nihil quod assertum meretur.
     * Indicia autem quae vere informant (tituli volantes, menus)
     * per JS fiunt, et ea haec via EXCITAT.
     *
     * CUR NON NATIVI: eventus nativus ':hover' verum daret, sed
     * cursorem SYSTEMATIS implicat - et Fran in eadem machina
     * laborat dum probationes currunt. Instrumentum quod indicem
     * rapit dum ille scribit instrumentum est quod nemo curret.
     * Primitivum nativum manet (fenestra_murem_immittere) si
     * aliquando ':hover' verum poposcerimus.
     *
     * EXITUS ET INTROITUS PARES: titulus volans qui in 'mouseenter'
     * apparet in 'mouseleave' abire DEBET. Sine pari, elementum
     * prius libratum in aeternum apertum maneret et imago sequens
     * mentiretur. Ergo elementum ultimum servamus. */
    a = chorda_aedificator_creare(manus->piscina, M);
    chorda_aedificator_appendere_literis(a, "var e=q(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");if(!e)return{ok:false,visum:"
        "\"nullum elementum VISIBILE huic selectori congruit\"};"
        "e.scrollIntoView({block:'center',inline:'center'});"
        "var r=e.getBoundingClientRect();"
        "if(r.width<=0||r.height<=0)return{ok:false,visum:"
        "\"elementum sine mensura (nihil librari potest)\"};"
        "var cx=r.left+r.width/2,cy=r.top+r.height/2;"
        /* enter/leave NON bulliunt (norma DOM); over/out bulliunt */
        "function mk(g,rel){return new MouseEvent(g,{bubbles:"
        "(g!=='mouseenter'&&g!=='mouseleave'),cancelable:true,"
        "view:window,clientX:cx,clientY:cy,relatedTarget:rel||null});}"
        "var ante=window.__manus_libratum||null;"
        "if(ante&&ante!==e&&document.contains(ante)){"
        "ante.dispatchEvent(mk('mouseout',e));"
        "ante.dispatchEvent(mk('mouseleave',e));}"
        "if(ante!==e){e.dispatchEvent(mk('mouseover',ante));"
        "e.dispatchEvent(mk('mouseenter',ante));}"
        "e.dispatchEvent(mk('mousemove',null));"
        "window.__manus_libratum=e;"
        "return{ok:true,visum:\"libratum\"};");

    v = _exspectare(manus,
                    _js_exspectare(manus, (constans character*)
                        _litterae(chorda_aedificator_finire(a),
                                  manus->piscina),
                        MANUS_MORA_ORDINARIA),
                    MANUS_MORA_ORDINARIA);
    si (!v.respondit)
    {
        redde FALSUM;   /* _iussum iam fregit */
    }
    si (!v.ok)
    {
        _frangere(manus, _litterae(v.visum, manus->piscina));
        redde FALSUM;
    }
    redde VERUM;
}

b32
manus_clavem (
    Manus*              manus,
    constans character* clavis)
{
    PiscinaNotatio nota;
    ManusResponsum r;
    i32            status;
    b32            ok;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    si (clavis == NIHIL || clavis[0] == '\0')
    {
        _frangere(manus, "manus_clavem: clavis VACUA");
        redde FALSUM;
    }

    nota = piscina_notare(manus->scriptorium);
    r = _petere(manus, HTTP_POST, "/imperium/clavis",
                chorda_ex_literis(clavis, manus->scriptorium),
                manus->scriptorium);
    status = r.successus ? r.status : ZEPHYRUM;
    ok = r.successus && r.status >= CC && r.status < CCC;
    piscina_reficere(manus->scriptorium, nota);

    si (!ok)
    {
        /* Causae DISTINCTAE. Prius omnes 'clavis non immissa'
         * essent, et 'applicatio hanc semitam non habet' ab
         * 'hanc clavem non novi' indiscretum - duo vitia valde
         * diversa quae ambo idem nomen ferrent. */
        si (status == (i32)CDIV)
        {
            _frangere(manus,
                "manus_clavem: applicatio claviarium non praebuit"
                " (atrium eum ponit; app manu structa"
                " imperium_claviarium_ponere vocet)");
        }
        alioquin si (status == (i32)CD)
        {
            _frangere(manus,
                "manus_clavem: clavis IGNOTA - nomina posita sunt"
                " (Enter Tab Escape Space Backspace Delete"
                " ArrowUp/Down/Left/Right Home End PageUp PageDown"
                " F1-F12), praefixis Cmd+ Ctrl+ Shift+ Alt+."
                " Textum per 'scribere' pone, non per claves");
        }
        alioquin
        {
            _frangere(manus, "manus_clavem: applicatio non respondit");
        }
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
        /* TEXTUM VISIBILEM reddere, non textContent.
         *
         * MENSURATUM 2026-08-13: 'manus textus body' XXXII milia
         * octetorum reddidit - fontem <script> paginae TOTUM,
         * commentariis inclusis. textContent enim omnia legit,
         * etiam quae usor videre NON POTEST.
         *
         * Id legem huius bibliothecae ipsam frangebat: omnia hic
         * 'VISIBILE, NON PRAESENS' significant (vide caput
         * MANUS_JS_VISUS supra). innerText solus eam servat -
         * scripta, styli, occulta omnia excludit.
         *
         * <select>: TEXTUS optionis electae, non valor. Valor
         * saepe clavis interna est ('019ffd86e206'); usor verba
         * legit. Prior forma chordam VACUAM reddebat cum optio
         * prima valorem vacuum haberet - quod 'selector nihil
         * invenit' mentiebatur. */
        "if(e.tagName==='SELECT'){var o=e.options[e.selectedIndex];"
        "return{ok:true,visum:o?String(o.text):\"\"};}"
        /* VALOR SOLIS CAMPIS - eadem custodia quam _tx fert.
         *
         * MENSURATUM 2026-08-14: '<button>' '.value' habet, vacuam
         * nisi attributum ponas. Sine custodia 'textus <pyxis>'
         * vacuum reddebat - responsum falsum sine ullo signo, dum
         * pyxis 'condere' plane in schermo diceret.
         *
         * DUAE FORMAE EIUSDEM IUDICII hic vivunt (haec et _tx), et
         * IAM discrepabant: haec SELECT ut textum optionis legit,
         * illa ut valorem. Quod ipsum est cur duplicatio nocet. */
        "if((e.tagName==='INPUT'||e.tagName==='TEXTAREA')"
        "&&e.value!==undefined&&e.value!==null)"
        "return{ok:true,visum:String(e.value)};"
        "return{ok:true,visum:String("
        "(e.innerText===undefined||e.innerText===null)"
        "?e.textContent:e.innerText)};");

    v = _interrogare(manus,
                     _litterae(chorda_aedificator_finire(a), manus->piscina));
    si (!v.ok)
    {
        redde vacua;
    }
    redde v.visum;
}

/* ========================================================================
 * Volvere
 * ======================================================================== */

b32
manus_volvere_ad (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    /* NON per _agere: porta agibilitatis hic falsa esset. Elementum
     * impeditum aut obtectum VIDERI tamen potest, et videre est
     * quod hic petimus. Volvere ad pyxidem impeditam legitimum est;
     * eam PREMERE non est. */
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var e=q(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");if(!e)return{ok:false,"
        "visum:\"nullum elementum VISIBILE huic selectori congruit\"};"
        "if(e.scrollIntoView)e.scrollIntoView({block:'center'});"
        "return{ok:true,visum:String(Math.round(window.pageYOffset||0))};");

    redde _interrogare(manus,
        _litterae(chorda_aedificator_finire(a), manus->piscina)).ok;
}

b32
manus_volvere (
    Manus* manus,
    s32    pixela)
{
    ChordaAedificator* a;
    character          numerus[XXXII];

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    sprintf(numerus, "%d", (integer)pixela);

    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "window.scrollBy(0,");
    chorda_aedificator_appendere_literis(a, numerus);
    chorda_aedificator_appendere_literis(a,
        ");return{ok:true,"
        "visum:String(Math.round(window.pageYOffset||0))};");

    redde _interrogare(manus,
        _litterae(chorda_aedificator_finire(a), manus->piscina)).ok;
}

/* ========================================================================
 * Exspectatio
 * ======================================================================== */

b32
manus_exspectare (
    Manus*              manus,
    constans character* selector,
    b32                 adesse,
    Mora                mora)
{
    ChordaAedificator* a;
    ManusVerdictum     v;

    si (manus == NIHIL || manus->fracta || selector == NIHIL)
    {
        redde FALSUM;
    }

    /* qn() numerat VISIBILIA solum - eadem lex quae actionibus et
     * assertis praeest, ergo 'adest' hic idem significat quod illic. */
    a = chorda_aedificator_creare(manus->piscina, CXXVIII);
    chorda_aedificator_appendere_literis(a, "var n=qn(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a, ");return{ok:n");
    chorda_aedificator_appendere_literis(a, adesse ? ">0" : "===0");
    chorda_aedificator_appendere_literis(a, ",visum:String(n)};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a),
                                  manus->piscina),
                        mora),
                    mora);

    si (!v.ok)
    {
        ChordaAedificator* b = chorda_aedificator_creare(manus->piscina,
                                                         CCLVI);

        chorda_aedificator_appendere_literis(b,
            adesse ? "exspectatio defecit: nihil VISIBILE congruit "
                   : "exspectatio defecit: adhuc adest ");
        chorda_aedificator_appendere_literis(b, selector);
        /* Numerus visus quoque: 'nihil apparuit' et 'tria manserunt'
         * diversa sunt, et alterum sine altero horam furatur. */
        chorda_aedificator_appendere_literis(b, " (visa: ");
        chorda_aedificator_appendere_chorda(b, v.visum);
        chorda_aedificator_appendere_literis(b, ")");

        _frangere(manus, _litterae(chorda_aedificator_finire(b),
                                   manus->piscina));
    }

    redde v.ok;
}

/* ========================================================================
 * Lectio
 * ======================================================================== */

Lectio
manus_legere (
    Manus*              manus,
    constans character* selector,
    Piscina*            piscina)
{
    ChordaAedificator* a;
    Lectio             fructus;
    chorda             valor;
    JsonResultus       lectio;
    i32                i;
    i32                n;

    fructus.lineae  = NIHIL;
    fructus.numerus = ZEPHYRUM;

    si (manus == NIHIL || manus->fracta || piscina == NIHIL
        || selector == NIHIL)
    {
        redde fructus;
    }

    /* Selector INTERPOLATUR, ergo macro purum esse non potest ut
     * affordantiarum. v/_nz/_tx ex praeambulo veniunt - eadem lex
     * visibilitatis et eadem coactio spatiorum quae petitioni
     * textuali praeest. */
    a = chorda_aedificator_creare(manus->piscina, (memoriae_index)(II * M));
    chorda_aedificator_appendere_literis(a, "(function(){");
    chorda_aedificator_appendere_literis(a, MANUS_JS_VISUS);
    chorda_aedificator_appendere_literis(a, "var L=document.querySelectorAll(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        "),i,j,r=[];"
        "for(i=0;i<L.length;i++){var e=L[i];if(!v(e))continue;"
        "var cel=[],ch=e.children;"
        "for(j=0;j<ch.length;j++){if(v(ch[j]))cel.push(_nz(_tx(ch[j])));}"
        /* Sine filiis elementaribus: textus SUUS una cellula. */
        "if(cel.length===0)cel.push(_nz(_tx(e)));"
        "r.push(cel);}"
        "return r;})()");

    /* Selector pravus hic IACTAT, et _iussum manum cum nuntio
     * navigatri frangit ("'' is not a valid selector") - quod
     * melius est quam index vacuus, qui 'nihil adest' mentiretur. */
    si (!_iussum(manus, chorda_aedificator_finire(a),
                 MANUS_MORA_BREVIS, &valor))
    {
        redde fructus;
    }

    lectio = json_legere(valor, piscina);
    si (!lectio.successus || !json_est_tabulatum(lectio.radix))
    {
        _frangere(manus, "lectio: responsum tabulatum non est");
        redde fructus;
    }

    n = json_tabulatum_numerus(lectio.radix);
    si (n == ZEPHYRUM)
    {
        redde fructus;
    }

    fructus.lineae = (LineaLecta*)piscina_allocare(
        piscina, (memoriae_index)n * magnitudo(LineaLecta));

    per (i = ZEPHYRUM; i < n; i++)
    {
        JsonValor* ordo = json_tabulatum_obtinere(lectio.radix, i);
        i32        quot = json_tabulatum_numerus(ordo);
        i32        j;

        fructus.lineae[i].numerus  = quot;
        fructus.lineae[i].cellulae = (quot > ZEPHYRUM)
            ? (chorda*)piscina_allocare(piscina,
                  (memoriae_index)quot * magnitudo(chorda))
            : NIHIL;

        per (j = ZEPHYRUM; j < quot; j++)
        {
            fructus.lineae[i].cellulae[j] = chorda_transcribere(
                json_ad_chorda(json_tabulatum_obtinere(ordo, j)), piscina);
        }
    }

    fructus.numerus = n;
    redde fructus;
}

/* ========================================================================
 * Affordantiae
 * ======================================================================== */

interior chorda
_campus_chorda (
    JsonValor*          obiectum,
    constans character* clavis,
    Piscina*            piscina)
{
    redde chorda_transcribere(
        json_ad_chorda(json_objectum_capere(obiectum, clavis)), piscina);
}

interior s32
_campus_numerus (
    JsonValor*          obiectum,
    constans character* clavis)
{
    redde (s32)json_ad_integer(json_objectum_capere(obiectum, clavis));
}

Affordantiae
manus_affordantiae (
    Manus*   manus,
    Piscina* piscina)
{
    ChordaAedificator* a;
    Affordantiae       fructus;
    chorda             valor;
    JsonResultus       lectio;
    i32                i;
    i32                n;

    fructus.res     = NIHIL;
    fructus.numerus = ZEPHYRUM;

    si (manus == NIHIL || manus->fracta || piscina == NIHIL)
    {
        redde fructus;
    }

    a = chorda_aedificator_creare(manus->piscina,
                                  (memoriae_index)(IV * M));
    chorda_aedificator_appendere_literis(a, "(function(){");
    chorda_aedificator_appendere_literis(a, MANUS_JS_VISUS);
    chorda_aedificator_appendere_literis(a, MANUS_JS_AFFORDANTIAE);
    chorda_aedificator_appendere_literis(a, "})()");

    /* _iussum manum FRANGIT et causam nominat si defecit - ergo hic
     * tacite redimus; vocans manus_fracta interroget. */
    si (!_iussum(manus, chorda_aedificator_finire(a),
                 MANUS_MORA_BREVIS, &valor))
    {
        redde fructus;
    }

    lectio = json_legere(valor, piscina);
    si (!lectio.successus || !json_est_tabulatum(lectio.radix))
    {
        /* Pagina respondit sed non tabulato. Hoc vitium NOSTRUM est
         * (JS supra), non usoris - ergo manum frangimus potius quam
         * indicem vacuum reddere, qui 'nihil hic est' mentiretur. */
        _frangere(manus,
            "affordantiae: responsum tabulatum non est");
        redde fructus;
    }

    n = json_tabulatum_numerus(lectio.radix);
    si (n == ZEPHYRUM)
    {
        redde fructus;
    }

    fructus.res = (Affordantia*)piscina_allocare(
        piscina, (memoriae_index)n * magnitudo(Affordantia));

    per (i = ZEPHYRUM; i < n; i++)
    {
        JsonValor*   o = json_tabulatum_obtinere(lectio.radix, i);
        Affordantia* d = &fructus.res[i];

        d->genus        = (AffordantiaGenus)_campus_numerus(o, "genus");
        d->selector     = _campus_chorda(o, "selector", piscina);
        d->titulus      = _campus_chorda(o, "titulus", piscina);
        d->valor        = _campus_chorda(o, "valor", piscina);
        d->impedimentum = _campus_chorda(o, "impedimentum", piscina);
        d->x            = _campus_numerus(o, "x");
        d->y            = _campus_numerus(o, "y");
        d->latitudo     = _campus_numerus(o, "latitudo");
        d->altitudo     = _campus_numerus(o, "altitudo");
    }

    fructus.numerus = n;
    redde fructus;
}

/* ========================================================================
 * Focus
 * ======================================================================== */

b32
manus_focus_ponere (
    Manus*              manus,
    constans character* selector)
{
    ChordaAedificator* a;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    a = chorda_aedificator_creare(manus->piscina, CCLVI);
    /* NULLA custodia generis hic - et hoc consulto.
     *
     * 'premere' clicum FINGIT, ergo morem clici sequi debet
     * (superficies scribendi solae). Hoc verbum nihil fingit: focum
     * PETIS. Petitio explicita regimen focare licite vult - ordinem
     * Tab probare, anulum foci in imagine videre, auditorem 'focus'
     * excitare. Instrumentum quod petitionem apertam recusaret
     * usorem ad 'aestimare' remitteret, quod est ipsum foramen quod
     * hoc verbum claudit.
     *
     * VERIFICATIO tamen OBLIGATORIA: '.focus()' in <div> sine
     * tabindex NIHIL agit et NIHIL dicit. Sine relectione verbum
     * 'factum' redderet dum focus alibi maneret - eadem forma
     * mendacii quam 'scribere' in <select> olim habuit. Ergo post
     * vocationem activeElement comparatur, et nomen eius in nuntio
     * fracturae apparet ut causa NOMINETUR. */
    chorda_aedificator_appendere_literis(a,
        "if(typeof e.focus!=='function')"
        "return{ok:false,visum:'<'+e.tagName.toLowerCase()+"
        "'> focum capere non potest'};"
        "e.focus();"
        "if(document.activeElement!==e){"
        "var b=document.activeElement;"
        "return{ok:false,visum:'focus non mansit - tenet '+"
        "(b?('<'+b.tagName.toLowerCase()+(b.id?'#'+b.id:'')+'>')"
        ":'nihil')+' (tabindex deest?)'};}"
        "return{ok:true,visum:\"focatum\"};");

    redde _agere(manus, "q(", selector,
                 _litterae(chorda_aedificator_finire(a), manus->piscina),
                 "manus_focus_ponere");
}

ManusFocus
manus_focus (
    Manus*   manus,
    Piscina* piscina)
{
    ChordaAedificator* a;
    ManusFocus         fructus;
    chorda             valor;
    JsonResultus       lectio;

    fructus.habet           = FALSUM;
    fructus.selector.mensura = 0;
    fructus.selector.datum   = NIHIL;
    fructus.titulus.mensura  = 0;
    fructus.titulus.datum    = NIHIL;
    fructus.tag.mensura      = 0;
    fructus.tag.datum        = NIHIL;

    si (manus == NIHIL || manus->fracta || piscina == NIHIL)
    {
        redde fructus;
    }

    a = chorda_aedificator_creare(manus->piscina, (memoriae_index)M);
    chorda_aedificator_appendere_literis(a, "(function(){");
    chorda_aedificator_appendere_literis(a, MANUS_JS_SEMITA);
    /* CORPUS ET documentElement AMBO 'nihil' sunt. activeElement
     * corpus reddit cum nemo focum tenet, sed post 'blur()' aut in
     * pagina nondum tacta documentElement reddere potest - idem
     * significant, ergo idem nuntiant. */
    chorda_aedificator_appendere_literis(a,
        "var e=document.activeElement;"
        "if(!e||e===document.body||e===document.documentElement)"
        "return{habet:0,selector:\"\",titulus:\"\",tag:\"\"};"
        "return{habet:1,selector:_via(e),titulus:_lab(e),"
        "tag:String(e.tagName).toLowerCase()};");
    chorda_aedificator_appendere_literis(a, "})()");

    si (!_iussum(manus, chorda_aedificator_finire(a),
                 MANUS_MORA_BREVIS, &valor))
    {
        redde fructus;
    }

    lectio = json_legere(valor, piscina);
    si (!lectio.successus)
    {
        _frangere(manus, "focus: responsum JSON non est");
        redde fructus;
    }

    fructus.habet    = _campus_numerus(lectio.radix, "habet") != ZEPHYRUM;
    fructus.selector = _campus_chorda(lectio.radix, "selector", piscina);
    fructus.titulus  = _campus_chorda(lectio.radix, "titulus", piscina);
    fructus.tag      = _campus_chorda(lectio.radix, "tag", piscina);
    redde fructus;
}

/* ========================================================================
 * Magnitudo
 * ======================================================================== */

b32
manus_magnitudinem_ponere (
    Manus* manus,
    i32    latitudo,
    i32    altitudo,
    i32*   latitudo_facta,
    i32*   altitudo_facta)
{
    PiscinaNotatio nota;
    ManusResponsum r;
    JsonResultus   lectio;
    i32            status;
    b32            ok;
    character      corpus[LXIV];

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }
    si (latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        _frangere(manus,
            "manus_magnitudinem_ponere: mensura non positiva");
        redde FALSUM;
    }

    sprintf(corpus, "%d %d", (integer)latitudo, (integer)altitudo);

    nota = piscina_notare(manus->scriptorium);
    r = _petere(manus, HTTP_POST, "/imperium/magnitudo",
                chorda_ex_literis(corpus, manus->scriptorium),
                manus->scriptorium);
    status = r.successus ? r.status : ZEPHYRUM;
    ok = r.successus && r.status >= CC && r.status < CCC;

    /* FACTA ANTE REFECTIONEM legere: corpus responsi in scriptorio
     * vivit, quod infra reficitur. Lectio post refectionem memoriam
     * mortuam legeret - genus erroris quod fere semper 'operatur'
     * donec piscina aliud eodem loco ponat. */
    si (ok)
    {
        lectio = json_legere(r.corpus, manus->scriptorium);
        si (lectio.successus)
        {
            si (latitudo_facta != NIHIL)
            {
                *latitudo_facta = (i32)_campus_numerus(lectio.radix,
                                                       "latitudo");
            }
            si (altitudo_facta != NIHIL)
            {
                *altitudo_facta = (i32)_campus_numerus(lectio.radix,
                                                       "altitudo");
            }
        }
    }
    piscina_reficere(manus->scriptorium, nota);

    si (!ok)
    {
        si (status == (i32)CDIV)
        {
            _frangere(manus,
                "manus_magnitudinem_ponere: applicatio"
                " magnitudinatorem non praebuit (atrium eum ponit;"
                " app manu structa imperium_magnitudinatorem_ponere"
                " vocet)");
        }
        alioquin si (status == (i32)CD)
        {
            _frangere(manus,
                "manus_magnitudinem_ponere: mensura recusata"
                " (positiva esse debet)");
        }
        alioquin
        {
            _frangere(manus,
                "manus_magnitudinem_ponere: applicatio non respondit");
        }
        redde FALSUM;
    }
    redde VERUM;
}

b32
manus_magnitudo (
    Manus* manus,
    i32*   latitudo,
    i32*   altitudo)
{
    chorda       valor;
    JsonResultus lectio;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    si (!_iussum(manus,
            chorda_ex_literis(
                "(function(){return{latitudo:window.innerWidth,"
                "altitudo:window.innerHeight};})()",
                manus->piscina),
            MANUS_MORA_BREVIS, &valor))
    {
        redde FALSUM;
    }

    lectio = json_legere(valor, manus->piscina);
    si (!lectio.successus)
    {
        _frangere(manus, "magnitudo: responsum JSON non est");
        redde FALSUM;
    }
    si (latitudo != NIHIL)
    {
        *latitudo = (i32)_campus_numerus(lectio.radix, "latitudo");
    }
    si (altitudo != NIHIL)
    {
        *altitudo = (i32)_campus_numerus(lectio.radix, "altitudo");
    }
    redde VERUM;
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
_manus_credo_focum (
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

    a = chorda_aedificator_creare(manus->piscina, (memoriae_index)M);
    chorda_aedificator_appendere_literis(a, MANUS_JS_SEMITA);
    chorda_aedificator_appendere_literis(a, "var e=document.activeElement,t=");
    /* querySelector, NON q(): elementum focum tenens visibile esse
     * DEBET ut focum utile habeat, sed si occultum est id ipsum est
     * quod probatio nosse vult. q() nihil inveniret et nuntius
     * 'focus alibi' diceret ubi verum est 'focus hic, sed latet'. */
    chorda_aedificator_appendere_literis(a, "document.querySelector(");
    _appendere_litteras_js(a, selector);
    chorda_aedificator_appendere_literis(a,
        ");"
        /* VISUM semper focum CURRENTEM nominat, etiam cum ok.
         * Nuntius fracturae qui solum 'falsum' dicit probatorem ad
         * aestimare manu scriptum remittit - id ipsum quod hoc
         * verbum tollere debet. */
        "var u=(!e||e===document.body||e===document.documentElement)"
        "?\"(nihil)\":_via(e);"
        "return{ok:!!(t&&e===t),visum:u};");

    v = _exspectare(manus,
                    _js_exspectare(manus,
                        _litterae(chorda_aedificator_finire(a), manus->piscina),
                        mora),
                    mora);

    redde _notare(manus, "credo_manus_focus",
                  _expressio(manus, selector),
                  selector, v, filum, versus);
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

chorda
manus_effusio (
    Manus* manus)
{
    chorda vacua;

    vacua.datum   = NIHIL;
    vacua.mensura = 0;

    si (manus == NIHIL || manus->processus == NIHIL)
    {
        redde vacua;
    }
    /* Pulsare PRIMUM: quod in fistula sedet nondum in sacculo est,
     * et vocator lineam modo scriptam quaerit. */
    (vacuum)processus_pulsare(manus->processus);
    redde processus_effusio_hactenus(manus->processus);
}

b32
_manus_credo_effusio (
    Manus*              manus,
    constans character* textus,
    Mora                mora,
    constans character* filum,
    s32                 versus)
{
    Momentum  terminus;
    b32       inventum = FALSUM;
    chorda    ultima;
    character quot[XXXII];

    ultima.datum   = NIHIL;
    ultima.mensura = 0;

    si (manus == NIHIL || manus->fracta)
    {
        redde FALSUM;
    }

    /* EXSPECTAT, sicut cetera: linea per fistulam venit quando
     * applicatio pulsat, non quando nos poscimus. Somnus hic idem
     * mendacium esset quod in DOM. */
    terminus = _nunc() + mora;
    dum (VERUM)
    {
        ultima = manus_effusio(manus);
        si (ultima.mensura > 0
            && chorda_continet(ultima,
                   chorda_ex_literis(textus, manus->piscina)))
        {
            inventum = VERUM;
            frange;
        }
        si (_nunc() > terminus)
        {
            frange;
        }
        _quiescere(MANUS_PULSUS_MAXIMUS);
    }

    sprintf(quot, "%lu octeti effusionis",
            (insignatus longus)ultima.mensura);
    _credo_notare("credo_manus_effusio",
                  "effusio applicationis",
                  quot, textus, filum, versus, inventum);
    si (!inventum)
    {
        _frangere(manus, "effusio applicationis textum non tulit");
    }
    redde inventum;
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
