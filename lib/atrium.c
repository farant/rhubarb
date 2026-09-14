/* atrium.c - applicatio vitreae PLENA. Consilium et rationes in
 * atrium.h; hic sola executio.
 *
 * NOTA DE FORMA: haec plagula ASSEMBLATIO est, non machinatio.
 * Quod hic vides fere totum ex forum.c/villa.c/mensor_ui.c
 * verbatim venit - id est ipsum argumentum. Nihil novi inventum
 * est; una sedes tribus substituta. */

#include "atrium.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ATRIUM_LATITUDO_ORDINARIA  1000
#define ATRIUM_ALTITUDO_ORDINARIA   900
#define ATRIUM_SIGNUM_VISIO         VII
#define ATRIUM_VISIO_LATITUDO      1100
#define ATRIUM_VISIO_ALTITUDO       860

structura Atrium {
           Piscina* piscina;      /* vita atrii (vocantis) */
           Piscina* arena;        /* per-tictum; PROPRIA */
    PiscinaNotatio  arena_nota;   /* punctum ad quod reficitur */

        Fenestra* fenestra;
         Capsula* capsula;
          Vitrea* vitrea;
    Internuntius* internuntius;
        Speculum* speculum;     /* NIHIL = sine fontibus */
        Vivarium  vivarium;     /* successus FALSUM = sine -vivum */

      Mora tictus_ms;
    chorda iniectio;     /* copiata in piscinam atrii */
       b32 parata;       /* pagina se paratam nuntiavit */

    AtriumEventor  eventor;
           vacuum* datum;

    /* --- VISIO (fenestra altera, briar-spec par. 4.9) --- */
                   b32  retro;             /* Visio vexillum heres */
    constans character* titulus;           /* copia: titulus Visionis */
    constans character* visio;             /* copia; NIHIL = sine */
               Piscina* visio_piscina;     /* vita fenestrae Visio */
              Fenestra* visio_fenestra;    /* NIHIL = clausa */
                Vitrea* visio_vitrea;
          Internuntius* visio_internuntius;  /* sine methodis */
};


/* ==================================================
 * Discretio nuntii - PURA (probabilis sine fenestra)
 * ================================================== */

AtriumNuntiusGenus
atrium_nuntium_discernere (
    chorda nuntium)
{
    si (nuntium.datum == NIHIL || nuntium.mensura == ZEPHYRUM)
    {
        redde ATRIUM_NUNTIUS_IGNOTUS;
    }

    /* Internuntius OBIECTA sola accipit. Prima littera sufficit:
     * quod non incipit '{' JSON-RPC esse NON POTEST, et nos hic
     * discernimus, non parsamus. */
    si (nuntium.datum[ZEPHYRUM] == '{')
    {
        redde ATRIUM_NUNTIUS_RPC;
    }

    si (   nuntium.mensura == (i32)VII
        && memcmp(nuntium.datum, "paratus", (memoriae_index)VII)
            == ZEPHYRUM)
    {
        redde ATRIUM_NUNTIUS_PARATUS;
    }

    redde ATRIUM_NUNTIUS_IGNOTUS;
}


/* ==================================================
 * Vexilla domus ex argv
 * ================================================== */

vacuum
atrium_vexilla_legere (
              AtriumConfiguratio*  figura,
                         integer   argc,
                       character** argv)
{
    integer i;

    si (figura == NIHIL || argv == NIHIL)
    {
        redde;
    }

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-vivum") == ZEPHYRUM)
        {
            figura->vivum = VERUM;
        }
        /* -retro: focum systematis non rapere - fumus XII
         * applicationes generat et usor scribens non interpellandus. */
        alioquin si (strcmp(argv[i], "-retro") == ZEPHYRUM)
        {
            figura->retro = VERUM;
        }
        /* -radix <via>: assetum e disco. INDEPENDENS a -vivum de
         * industria - laboratorium ambo sub uno vexillo ligavit, et
         * ita duas figuras rationabiles inattingibiles fecit: imperium
         * contra capsulam VECTAM (id quod probare vis), et assetum e
         * disco SINE canali exsecutionis aperto. */
        alioquin si (   strcmp(argv[i], "-radix") == ZEPHYRUM
                     && (i + I) < argc)
        {
            i++;
            figura->capsula_radix = argv[i];
        }
        alioquin si (   strcmp(argv[i], "-portus") == ZEPHYRUM
                     && (i + I) < argc)
        {
            /* SIGNATUM de industria: portus est i32 (INSIGNATUS),
             * ergo atoi("-5") sine hac custodia in numerum immanem
             * verteretur et ligatio causa ignota falleret. */
            s32 lectus = (s32)atoi(argv[i + I]);

            si (lectus > ZEPHYRUM)
            {
                figura->portus = (i32)lectus;
            }
        }
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

interior Atrium*
_frangere (
                chorda* causa,
    constans character* nuntius,
               Piscina* piscina)
{
    si (causa != NIHIL)
    {
        *causa = chorda_ex_literis(nuntius, piscina);
    }
    redde NIHIL;
}

Atrium*
atrium_creare (
                        Piscina* piscina,
    constans AtriumConfiguratio* figura,
                         chorda* causa)
{
                  Atrium* atrium;
    FenestraConfiguratio  figura_fenestrae;
      VitreaConfiguratio  figura_vitreae;
          memoriae_index  arena_mensura;

    si (piscina == NIHIL)
    {
        /* Sine piscina causam ipsam nuntiare non possumus - chorda
         * sedem poscit. Vacuam ponimus ne vocans stalum legat. */
        si (causa != NIHIL)
        {
            causa->datum    = NIHIL;
            causa->mensura  = ZEPHYRUM;
        }
        redde NIHIL;
    }

    si (figura == NIHIL)
    {
        redde _frangere(causa, "Configuratio deest", piscina);
    }
    si (figura->titulus == NIHIL)
    {
        redde _frangere(causa,
            "Titulus deest - fenestra nomen poscit", piscina);
    }
    /* Visio VACUA ante capsulam: recusatio sine AppKit probabilis */
    si (figura->visio != NIHIL && figura->visio[0] == '\0')
    {
        redde _frangere(causa,
            "Visio vacua - via paginae intra capsulam aut NIHIL",
            piscina);
    }
    si (figura->capsula == NIHIL)
    {
        redde _frangere(causa,
            "Capsula deest - unde pagina oneretur?", piscina);
    }

    atrium = (Atrium*)piscina_allocare(piscina, magnitudo(Atrium));
    memset(atrium, 0, magnitudo(Atrium));

    atrium->piscina   = piscina;
    atrium->tictus_ms = (figura->tictus_ms != ZEPHYRUM)
                      ? figura->tictus_ms : (Mora)CC;
    atrium->eventor  = figura->eventor;
    atrium->datum    = figura->datum;
    atrium->retro    = figura->retro;
    atrium->titulus  = chorda_ut_cstr(chorda_ex_literis(figura->titulus,
        piscina), piscina);
    atrium->visio    = (figura->visio != NIHIL)
                     ? chorda_ut_cstr(chorda_ex_literis(figura->visio,
                         piscina), piscina)
                     : NIHIL;

    /* --- ARENA PER-TICTUM ---
     * Piscina PROPRIA, non sectio piscinae vocantis: reficitur
     * omni gressu, et id super allocationes vocantis facere eas
     * necaret. */
    arena_mensura = (figura->arena_mensura != ZEPHYRUM)
                  ? figura->arena_mensura : (memoriae_index)(M * M);

    atrium->arena = piscina_generare_dynamicum("atrium/arena",
                                               arena_mensura);
    si (atrium->arena == NIHIL)
    {
        redde _frangere(causa,
            "Arena per-tictum creari non potuit", piscina);
    }
    atrium->arena_nota = piscina_notare(atrium->arena);

    /* --- FENESTRA --- */
    thema_initiare();

    memset(&figura_fenestrae, 0, magnitudo(figura_fenestrae));
    figura_fenestrae.titulus  = figura->titulus;
    figura_fenestrae.x        = CC;
    figura_fenestrae.y        = CC;
    figura_fenestrae.latitudo = (figura->latitudo != ZEPHYRUM)
                              ? figura->latitudo
                              : (i32)ATRIUM_LATITUDO_ORDINARIA;
    figura_fenestrae.altitudo = (figura->altitudo != ZEPHYRUM)
                              ? figura->altitudo
                              : (i32)ATRIUM_ALTITUDO_ORDINARIA;
    figura_fenestrae.vexilla  = (figura->vexilla != ZEPHYRUM)
                              ? figura->vexilla
                              : (i32)(FENESTRA_CLAUDIBILIS
                                    | FENESTRA_MUTABILIS
                                    | FENESTRA_CENTRATA);
    si (figura->retro)
    {
        figura_fenestrae.vexilla = figura_fenestrae.vexilla
                                 | (i32)FENESTRA_RETRO;
    }

    atrium->fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (atrium->fenestra == NIHIL)
    {
        redde _frangere(causa, "Fenestra creari non potuit", piscina);
    }

    /* --- CAPSULA + VITREA --- */
    /* Radix data = fons discus, ansa eadem. Vitrea nihil de hoc scit:
     * capsula_legere unica vox est qua eam tangit. */
    si (   figura->capsula_radix    != NIHIL
        && figura->capsula_radix[0] != '\0')
    {
        atrium->capsula = capsula_aperire_e_disco(
            figura->capsula_radix, piscina);
        si (atrium->capsula == NIHIL)
        {
            /* Radix prava TACITE ad capsulam infixam NON labitur: qui
             * '-radix' poscit modum evolutionis poscit, et lapsus
             * tacitus eum assetum VETUS videre sineret dum se sua
             * videre credit. */
            redde _frangere(causa,
                "Radix capsulae directorium non est (-radix)", piscina);
        }
    }
    alioquin
    {
        atrium->capsula = capsula_aperire(figura->capsula, piscina);
        si (atrium->capsula == NIHIL)
        {
            redde _frangere(causa,
                "Capsula aperiri non potuit (assets fracta?)", piscina);
        }
    }

    memset(&figura_vitreae, 0, magnitudo(figura_vitreae));
    figura_vitreae.origo    = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula  = atrium->capsula;
    figura_vitreae.via_initialis = (figura->via_initialis != NIHIL)
                                 ? figura->via_initialis
                                 : "index.html";
    figura_vitreae.url           = NIHIL;
    figura_vitreae.inspectabilis = figura->sine_inspectore_webkit
                                 ? FALSUM : VERUM;

    atrium->vitrea = vitrea_creare(piscina, atrium->fenestra,
                                   &figura_vitreae);
    si (atrium->vitrea == NIHIL)
    {
        redde _frangere(causa, "Vitrea creari non potuit", piscina);
    }

    /* --- INTERNUNTIUS ---
     * Semper creatus, etiam si app nullam methodum praebet:
     * speculum eum poscit, et pagina paratum suum per eum mittit. */
    atrium->internuntius = internuntius_creare(piscina, vitrea_missor,
                                               atrium->vitrea);
    si (atrium->internuntius == NIHIL)
    {
        redde _frangere(causa, "Internuntius creari non potuit",
                        piscina);
    }

    /* --- VISIO (optiva): res menu; fenestra altera in gressu --- */
    si (   atrium->visio != NIHIL
        && !fenestra_menu_addere(atrium->fenestra, "Visio",
               "Cmd+Shift+v", (i32)ATRIUM_SIGNUM_VISIO))
    {
        redde _frangere(causa, "Visio: res menu addi non potuit",
            piscina);
    }

    /* --- SPECULUM (optivum) --- */
    si (figura->fontes != NIHIL)
    {
        atrium->speculum = speculum_creare(piscina, figura->fontes,
                                           atrium->internuntius,
                                           vitrea_aestimator,
                                           atrium->vitrea);
        si (atrium->speculum == NIHIL)
        {
            redde _frangere(causa,
                "Speculum creari non potuit (capsula fontium fracta,"
                " aut methodus iam praebita)", piscina);
        }
    }

    /* --- IMPERIUM (optivum, modus evolutionis) --- */
    si (figura->vivum)
    {
        VivariumConfiguratio figura_vivarii;

        memset(&figura_vivarii, 0, magnitudo(figura_vivarii));
        figura_vivarii.portus      = figura->portus;
        figura_vivarii.aestimator  = vitrea_aestimator;
        figura_vivarii.imaginator  = vitrea_imaginator;
        figura_vivarii.missor      = vitrea_missor;
        figura_vivarii.datum       = atrium->vitrea;
        /* Claviarius FENESTRAM poscit (eventus nativus ad fenestram
         * clavem it), ceterae suturae VITREAM - ergo datum proprium.
         * Vide imperium.h. */
        figura_vivarii.claviarius            = fenestra_claviarius;
        figura_vivarii.claviarius_datum      = atrium->fenestra;
        figura_vivarii.musarius              = fenestra_musarius;
        figura_vivarii.musarius_datum        = atrium->fenestra;
        figura_vivarii.magnitudinator        = fenestra_magnitudinator;
        figura_vivarii.magnitudinator_datum  = atrium->fenestra;
        figura_vivarii.fenestra     =
            fenestra_numerus_nativus(atrium->fenestra);
        /* Internuntium NOSTRUM tradimus: pons unus imperium unum
         * fert (imperium.h id nominat), ergo vivarium alterum
         * creare sinere registrationem geminam pareret. */
        figura_vivarii.internuntius = atrium->internuntius;

        atrium->vivarium = imperium_vivarium(piscina, &figura_vivarii);
        si (!atrium->vivarium.successus)
        {
            si (causa != NIHIL)
            {
                *causa = atrium->vivarium.causa;
            }
            redde NIHIL;
        }
    }

    redde atrium;
}


/* ==================================================
 * Visio: fenestra altera (briar-spec par. 4.9)
 * ================================================== */

/* ordo: vitrea ANTE fenestram, deinde piscina; partes absentes
 * praetereuntur, ergo et apertio fracta hic purgatur */
interior vacuum
_visionem_claudere (
    Atrium* atrium)
{
    si (atrium->visio_vitrea != NIHIL)
    {
        vitrea_destruere(atrium->visio_vitrea);
    }
    si (atrium->visio_fenestra != NIHIL)
    {
        fenestra_destruere(atrium->visio_fenestra);
    }
    si (atrium->visio_piscina != NIHIL)
    {
        piscina_destruere(atrium->visio_piscina);
    }
    atrium->visio_vitrea        = NIHIL;
    atrium->visio_fenestra      = NIHIL;
    atrium->visio_internuntius  = NIHIL;
    atrium->visio_piscina       = NIHIL;
}

/* Aperire (VERUM) aut apertam ante ponere (FALSUM). Piscina PROPRIA:
 * clausa destruitur, ergo apertio iterata nihil cumulat. Eadem capsula
 * (infixa aut -radix): pagina eodem modo legitur ac principalis. */
interior b32
_visionem_aperire (
    Atrium* atrium)
{
    FenestraConfiguratio figura_fenestrae;
      VitreaConfiguratio figura_vitreae;
               character titulus[CCLVI];

    si (atrium->visio_fenestra != NIHIL)
    {
        fenestra_monstrare(atrium->visio_fenestra);
        redde FALSUM;
    }
    atrium->visio_piscina = piscina_generare_dynamicum("atrium/visio",
        (memoriae_index)(M * M));
    si (atrium->visio_piscina == NIHIL)
    {
        redde FALSUM;
    }
    /* lineola longa UTF-8 (e2 80 94) inter titulum et 'visio' */
    sprintf(titulus, "%.200s \xe2\x80\x94 visio", atrium->titulus);
    memset(&figura_fenestrae, 0, magnitudo(figura_fenestrae));
    figura_fenestrae.titulus   = titulus;
    figura_fenestrae.x         = CC;
    figura_fenestrae.y         = CC;
    figura_fenestrae.latitudo  = (i32)ATRIUM_VISIO_LATITUDO;
    figura_fenestrae.altitudo  = (i32)ATRIUM_VISIO_ALTITUDO;
    figura_fenestrae.vexilla   = (i32)(FENESTRA_CLAUDIBILIS
                               | FENESTRA_MUTABILIS
                               | FENESTRA_CENTRATA);
    si (atrium->retro)
    {
        figura_fenestrae.vexilla = figura_fenestrae.vexilla
                                 | (i32)FENESTRA_RETRO;
    }
    atrium->visio_fenestra = fenestra_creare(atrium->visio_piscina,
        &figura_fenestrae);
    si (atrium->visio_fenestra == NIHIL)
    {
        _visionem_claudere(atrium);
        redde FALSUM;
    }
    memset(&figura_vitreae, 0, magnitudo(figura_vitreae));
    figura_vitreae.origo          = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula        = atrium->capsula;
    figura_vitreae.via_initialis  = atrium->visio;
    figura_vitreae.url            = NIHIL;
    figura_vitreae.inspectabilis  = VERUM;
    atrium->visio_vitrea = vitrea_creare(atrium->visio_piscina,
        atrium->visio_fenestra, &figura_vitreae);
    /* internuntius SINE methodis: vocationes paginae 'methodus ignota'
     * statim accipiunt et pagina ad insulam suam labitur (V4) */
    atrium->visio_internuntius = (atrium->visio_vitrea != NIHIL)
        ? internuntius_creare(atrium->visio_piscina, vitrea_missor,
              atrium->visio_vitrea)
        : NIHIL;
    si (atrium->visio_internuntius == NIHIL)
    {
        _visionem_claudere(atrium);
        redde FALSUM;
    }
    fenestra_monstrare(atrium->visio_fenestra);
    redde VERUM;
}

/* per tictum: clausuram videre, pontem exhaurire (nihil cumulatur).
 * Pumpa fenestrae eventus TOTIUS applicationis in caudam principalem
 * trahit; cauda Visionis vacua manet et legenda non est. */
interior vacuum
_visionem_pulsare (
    Atrium* atrium)
{
                chorda nuntium;
    VitreaNuntiusGenus genus;

    si (atrium->visio_fenestra == NIHIL)
    {
        redde;
    }
    si (fenestra_debet_claudere(atrium->visio_fenestra))
    {
        _visionem_claudere(atrium);
        redde;
    }
    dum (vitrea_obtinere_nuntium(atrium->visio_vitrea, &nuntium,
            &genus))
    {
        si (genus == VITREA_NUNTIUS_INTERITUS)
        {
            vitrea_recargare(atrium->visio_vitrea);
            perge;
        }
        si (atrium_nuntium_discernere(nuntium) == ATRIUM_NUNTIUS_RPC)
        {
            internuntius_tractare(atrium->visio_internuntius, nuntium,
                atrium->arena);
        }
    }
}


/* ==================================================
 * Gyrus
 * ================================================== */

vacuum
atrium_monstrare (
    Atrium* atrium)
{
    si (atrium == NIHIL)
    {
        redde;
    }
    fenestra_monstrare(atrium->fenestra);
}

b32
atrium_currendum (
    constans Atrium* atrium)
{
    si (atrium == NIHIL)
    {
        redde FALSUM;
    }
    /* fenestra_debet_claudere constantem accipit; atrium ipsum hic
     * non mutamus, ergo constantia vera est. */
    redde !fenestra_debet_claudere(atrium->fenestra);
}

i32
atrium_gressus (
    Atrium* atrium)
{
                   i32 actum = ZEPHYRUM;
               Eventus eventus;
                chorda nuntium;
    VitreaNuntiusGenus genus;

    si (atrium == NIHIL)
    {
        redde ZEPHYRUM;
    }

    /* ARENA PRIMUM. Vide decipulam in atrium.h: quod app ex arena
     * post gressum sumpsit usque huc valuit, et hic moritur. */
    piscina_reficere(atrium->arena, atrium->arena_nota);

    fenestra_expectare_eventus(atrium->fenestra, atrium->tictus_ms);

    dum (fenestra_obtinere_eventus(atrium->fenestra, &eventus))
    {
        /* VISIO PRIMUM: res menu atrii, non applicationis. Ceterae
         * res menu (app eas ipsa addere potest) eventori transeunt. */
        si (   eventus.genus             == EVENTUS_MENU
            && atrium->visio             != NIHIL
            && eventus.datum.menu.signum == (i32)ATRIUM_SIGNUM_VISIO)
        {
            si (_visionem_aperire(atrium))
            {
                actum |= (i32)ATRIUM_ACTUM_VISIO;
            }
            perge;
        }
        /* Speculum PRIMUM: Cmd+Shift+D rapit et VERUM reddit.
         * Quod rapuit eventori non ostenditur - aliter app
         * compendium domus ut suum tractare posset. */
        si (   atrium->speculum != NIHIL
            && speculum_tangere(atrium->speculum, &eventus))
        {
            perge;
        }
        si (atrium->eventor != NIHIL)
        {
            atrium->eventor(atrium->datum, &eventus);
        }
    }

    dum (vitrea_obtinere_nuntium(atrium->vitrea, &nuntium, &genus))
    {
        si (genus == VITREA_NUNTIUS_INTERITUS)
        {
            /* Processus interretialis obiit. Tres apps idem
             * agebant; nunc consilium domus est. */
            vitrea_recargare(atrium->vitrea);
            atrium->parata  = FALSUM;   /* pagina nova, iniectio rursus */
            actum           |= (i32)ATRIUM_ACTUM_INTERITUS;
            perge;
        }

        commutatio (atrium_nuntium_discernere(nuntium))
        {
        casus ATRIUM_NUNTIUS_RPC:
            internuntius_tractare(atrium->internuntius, nuntium,
                                  atrium->arena);
            actum |= (i32)ATRIUM_ACTUM_RPC;
            frange;

        casus ATRIUM_NUNTIUS_PARATUS:
            atrium->parata = VERUM;
            actum |= (i32)ATRIUM_ACTUM_PARATA;
            si (atrium->iniectio.mensura != ZEPHYRUM)
            {
                vitrea_aestimare(atrium->vitrea, atrium->iniectio);
            }
            frange;

        ordinarius:
            /* Nec JSON nec verbum notum. TACEMUS de industria:
             * internuntio tradere 'culpam orbam' in consola omni
             * onere pareret - strepitum qui culpas VERAS obtegeret,
             * qui ipse defectus erat quem mensor_ui manu vitabat. */
            frange;
        }
    }

    /* Visio: clausura videtur, pons exhauritur */
    _visionem_pulsare(atrium);

    /* Imperium: innocuum si vivarium non successit (gressus
     * successum intus probat). */
    vivarium_gressus(&atrium->vivarium);

    redde actum;
}

vacuum
atrium_destruere (
    Atrium* atrium)
{
    si (atrium == NIHIL)
    {
        redde;
    }

    /* Visio PRIMUM: fenestra altera ante principalem */
    _visionem_claudere(atrium);

    /* ORDO: vitrea ANTE fenestram (vitrea.h id nominat quia facile
     * invertitur). Semel hic, non in quaque app. */
    vitrea_destruere(atrium->vitrea);
    fenestra_destruere(atrium->fenestra);

    /* Arena NOSTRA est; piscina vocantis eius est. */
    piscina_destruere(atrium->arena);
    atrium->arena = NIHIL;
}


/* ==================================================
 * Iniectio
 * ================================================== */

vacuum
atrium_iniectionem_ponere (
    Atrium* atrium,
    chorda  javascript)
{
    si (atrium == NIHIL || javascript.mensura == ZEPHYRUM)
    {
        redde;
    }

    /* COPIA: aedificator vocantis post hoc liberari aut reficii
     * potest, et iniectio post quamque recargationem rursus
     * currit - ergo eam vitam atrii vivere oportet. */
    atrium->iniectio = chorda_transcribere(javascript,
                                           atrium->piscina);

    /* Si pagina IAM parata est, nunc curre. Sine hoc ordo
     * ponendi/parandi refert, et defectus esset silens. */
    si (atrium->parata)
    {
        vitrea_aestimare(atrium->vitrea, atrium->iniectio);
    }
}


/* ==================================================
 * Accessus ad partes
 * ================================================== */

Internuntius*
atrium_internuntius (
    Atrium* atrium)
{
    redde (atrium != NIHIL) ? atrium->internuntius : NIHIL;
}

Vitrea*
atrium_vitrea (
    Atrium* atrium)
{
    redde (atrium != NIHIL) ? atrium->vitrea : NIHIL;
}

Fenestra*
atrium_fenestra (
    Atrium* atrium)
{
    redde (atrium != NIHIL) ? atrium->fenestra : NIHIL;
}

Piscina*
atrium_arena (
    Atrium* atrium)
{
    redde (atrium != NIHIL) ? atrium->arena : NIHIL;
}

i32
atrium_portus (
    constans Atrium* atrium)
{
    si (atrium == NIHIL || !atrium->vivarium.successus)
    {
        redde ZEPHYRUM;
    }
    redde atrium->vivarium.portus;
}
