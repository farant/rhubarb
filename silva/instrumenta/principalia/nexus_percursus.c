/* nexus_percursus.c - NEXUS PERCURSUS (instrumenta prima #4a):
 * corpus totum semel parsatur + analysatur, omnis symboli sedes
 * declarationis et omnis sedes usus in build/nexus.tsv funduntur.
 * CLI nexus (nexus.c) tabulam millisecundis legit.
 *
 * Tabula DISPONIBILIS consulto (DECISUS sim-2 #3): numquam
 * committitur, scaffolding prae-acta; usus cibi canini schema
 * actorum alet. Regeneratio: ./silva/nexus.sh -renovare.
 *
 * Forma ordinis (septem campi, tab separati):
 *   titulus  species(sedes|usus)  genus  via  linea  columna  prof
 * prof = profunditas symboli (0 = scopus fili); usus systematis
 * (printf, malloc...) INCLUSI - "quis vocat malloc" quaestio vera
 * officinae erat (auditus aedificatorum M2).
 *
 * Duplicata (capita a multis TU inclusa) tabula clavium deduplicata.
 * Duo-passus percursus exemplar: sem2 post recanonicare AUCTORITAS
 * (symbolum_nodi canonicae-RELATIVUM est).
 *
 * MUNDUS AMALGAMATIS (LEGATUS v0.1b): silva.h solum; logica ordinum
 * in instrumenta/nexus_ordines.{h,c} communis cum legato - geminus
 * sepultus, barra migrationis = paritas octetim tsv. Piscinae
 * divisae: SilvaPiscina pro arboribus silvae, Piscina bibliothecae
 * pro textibus/clavibus (silva_piscina_allocare non publica -
 * exemplar praeparatoris).
 *
 * EXCUBITOR chunk 1: idem percursus build/inclusiones.tsv fundit -
 * graphus inclusionum (ex TAB ad TAB status) pro custode staleness.
 * Capita praebita basename tantum sunt; capita_viae (impleta in
 * _caput_praebere, primus-vincit = semantica praebere ipsius) vias
 * plenas reddit - ordo viam CUIUS OCTETI VERE PARSATI SUNT nominat.
 * Viae externae (stdio.h...) basename manent; consumptores eas
 * praetermittunt.
 *
 * ORDO CANONICUS + ATTRIBUTIO PER PLAGULAM (rung 5 (1), 2026-07-14):
 * ambulatio vias COLLIGIT, ordinatio (strcmp), deinde iudicia -
 * readdir non deterministicum erat (ordo tabulae = fortuna APFS).
 * Ordines nexus IAM sui-attributi erant (fundere vias sine '/'
 * omittit; 796/796 cursus contigui verificati) - filtrum
 * via==plagula rete tutelae est. Margines inclusionum RE-PINNATI
 * ad attributionem per-ex (antea primus-TU-vincit interpositus:
 * 1150 cursus / 723 ex). Consecutio: tabulae per viam aggregatae
 * ordine canonico stabili - praecursor fusionis incrementalis
 * (INTENTIO rung 5, silva/phase-log.md). */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include "silva.h"
#include "nexus_ordines.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

hic_manens FILE* effusio = NIHIL;
hic_manens TabulaDispersa* clavium_visa = NIHIL;
hic_manens Piscina* piscina_clavium = NIHIL;
hic_manens Piscina* piscina_textuum = NIHIL;

hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 ordines_scripti = ZEPHYRUM;
hic_manens i32 ordines_duplicati = ZEPHYRUM;
hic_manens i32 praetermissae = ZEPHYRUM;
hic_manens i32 plagulae_sem2 = ZEPHYRUM;   /* recanonicare > 0 ->
                                            * transitus alter (rung 3
                                            * scalae: calibratio) */

/* EXCUBITOR chunk 1: effusio graphi inclusionum */
hic_manens FILE* effusio_inclusionum = NIHIL;
hic_manens TabulaDispersa* inclusiones_visae = NIHIL;
hic_manens TabulaDispersa* capita_viae = NIHIL;  /* basename -> chorda* via plena */
hic_manens i32 inclusiones_scriptae = ZEPHYRUM;
hic_manens i32 inclusiones_duplicatae = ZEPHYRUM;

/* rung 5 (1): ordo canonicus + attributio per plagulam */
hic_manens Xar* viae_omnes = NIHIL;                /* character* elementa */
hic_manens constans character* via_activa = NIHIL; /* plagula sub iudicio */
hic_manens i32 ordines_alieni = ZEPHYRUM;          /* rete tutelae (0 exspectatum) */
hic_manens i32 inclusiones_alienae = ZEPHYRUM;     /* margines non-sui omissi */

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

/* receptor sweep: dedup per clavem plenam + fprintf (praefixum
 * "./" ambulationis detractum - cosmetica viae tabulae) */
hic_manens vacuum
_ordinem_recipere (
                  vacuum* datum,
    constans SilvaChorda* titulus,
      constans character* species,
      constans character* genus,
    constans SilvaChorda* via,
      insignatus integer  linea,
      insignatus integer  columna,
      insignatus integer  profunditas)
{
                        character  clavis_litterae[1024];
                           chorda  clavis;
                              int  scripti;
    constans insignatus character* via_datum;
               insignatus integer  via_mensura;

    (vacuum)datum;
    si (effusio == NIHIL || titulus == NIHIL || via == NIHIL)
    {
        redde;
    }
    via_datum    = via->datum;
    via_mensura  = via->mensura;
    si (   via_mensura > II && via_datum[ZEPHYRUM] == '.'
        && via_datum[I] == '/')
    {
        via_datum    += II;
        via_mensura  -= II;
    }
    /* attributio per plagulam (rung 5 (1)): ordines viae alienae
     * omissi. Rete tutelae - fundere basenames iam omittit, ergo 0
     * exspectatum; si accendit, praesumptio attributionis fracta
     * (vide commentarium capitis). */
    si (via_activa != NIHIL)
    {
        memoriae_index m_activae = strlen(via_activa);

        si (   (memoriae_index)via_mensura              != m_activae
            || memcmp(via_datum, via_activa, m_activae) != ZEPHYRUM)
        {
            ordines_alieni++;
            redde;
        }
    }
    si (   titulus->mensura == ZEPHYRUM
        || (memoriae_index)titulus->mensura
            + (memoriae_index)via_mensura + LXIV
            >= magnitudo(clavis_litterae))
    {
        redde;
    }
    scripti = sprintf(clavis_litterae, "%.*s\t%s\t%.*s\t%u\t%u",
        (int)titulus->mensura,
        (constans character*)titulus->datum,
        species,
        (int)via_mensura, (constans character*)via_datum,
        linea, columna);
    si (scripti <= ZEPHYRUM)
    {
        redde;
    }
    /* chorda_ex_literis COPIAT in piscinam (clavis stabilis -
     * buffer stivae reusatur) */
    clavis = chorda_ex_literis(clavis_litterae, piscina_clavium);
    si (tabula_dispersa_continet(clavium_visa, clavis))
    {
        ordines_duplicati++;
        redde;
    }
    (vacuum)tabula_dispersa_inserere(clavium_visa, clavis, NIHIL);

    fprintf(effusio, "%.*s\t%s\t%s\t%.*s\t%u\t%u\t%u\n",
        (int)titulus->mensura,
        (constans character*)titulus->datum,
        species, genus,
        (int)via_mensura, (constans character*)via_datum,
        linea, columna, profunditas);
    ordines_scripti++;
}

/* via ad emissionem: praefixum "./" detractum; basename sine '/'
 * per capita_viae ad viam plenam resolutum (primus-vincit); ignotum
 * (caput externum) ut est manet */
hic_manens vacuum
_viam_solvere (
    constans SilvaChorda*  via,
      constans character** datum_out,
      insignatus integer*  mensura_out)
{
    constans insignatus character* datum;
               insignatus integer  mensura;

    datum    = via->datum;
    mensura  = via->mensura;
    si (mensura > II && datum[ZEPHYRUM] == '.' && datum[I] == '/')
    {
        datum    += II;
        mensura  -= II;
    }
    si (   capita_viae != NIHIL && mensura > ZEPHYRUM
        && memchr(datum, '/', (memoriae_index)mensura) == NIHIL)
    {
        chorda  clavis;
        vacuum* valor;
        unio { constans insignatus character* c; i8* m; } u;

        u.c             = datum;
        clavis.datum    = u.m;
        clavis.mensura  = (i32)mensura;
        si (   tabula_dispersa_invenire(capita_viae, clavis, &valor)
            && valor != NIHIL)
        {
            constans chorda* plena = (constans chorda*)valor;

            *datum_out    = (constans character*)plena->datum;
            *mensura_out  = (insignatus integer)plena->mensura;
            redde;
        }
    }
    *datum_out    = (constans character*)datum;
    *mensura_out  = mensura;
}

/* graphus inclusionum TU huius fundere: ordo = ex TAB ad TAB status
 * (sumpta|praetermissa). Dedup clavi plena - visio prima et
 * praetermissa et sumpta eiusdem paris ambae supersunt (tabula =
 * database; consumptores ordines ut margines dedupant) */
hic_manens vacuum
_inclusiones_fundere (
    constans SilvaExpansio* exp)
{
    insignatus integer n;
    insignatus integer k;

    si (effusio_inclusionum == NIHIL || exp == NIHIL)
    {
        redde;
    }
    n = silva_inclusiones_numerus(exp);
    per (k = ZEPHYRUM; k < n; k++)
    {
          SilvaInclusioVista  vista;
        constans SilvaChorda* via_ex;
        constans SilvaChorda* via_ad;
          constans character* ex_datum;
          insignatus integer  ex_mensura;
          constans character* ad_datum;
          insignatus integer  ad_mensura;
                   character  clavis_litterae[2304];
                      chorda  clavis;
                         int  scripti;

        si (!silva_inclusio_vista(exp, k, &vista))
        {
            perge;
        }
        via_ex = silva_fons_via(exp, vista.fons_ex);
        si (via_ex == NIHIL)
        {
            perge;
        }
        via_ad = NIHIL;
        si (vista.fons_ad >= ZEPHYRUM)
        {
            via_ad = silva_fons_via(exp, vista.fons_ad);
        }
        si (via_ad == NIHIL)
        {
            via_ad = vista.via;
        }
        si (via_ad == NIHIL)
        {
            perge;
        }
        _viam_solvere(via_ex, &ex_datum, &ex_mensura);
        _viam_solvere(via_ad, &ad_datum, &ad_mensura);
        /* attributio per-ex (rung 5 (1)): quaeque plagula ambulata
         * margines SUOS solum emittit - margines nidificati (ex =
         * caput inclusum) ab ea ipsa plagula, directe ambulata,
         * emittuntur. Antea primus-TU-vincit interpositus. */
        si (via_activa != NIHIL)
        {
            memoriae_index m_activae = strlen(via_activa);

            si (   (memoriae_index)ex_mensura != m_activae
                || memcmp(ex_datum, via_activa, m_activae)
                    != ZEPHYRUM)
            {
                inclusiones_alienae++;
                perge;
            }
        }
        si (   ex_mensura == ZEPHYRUM || ad_mensura == ZEPHYRUM
            || (memoriae_index)ex_mensura + (memoriae_index)ad_mensura
                + XXXII >= magnitudo(clavis_litterae))
        {
            perge;
        }
        scripti = sprintf(clavis_litterae, "%.*s\t%.*s\t%s",
            (int)ex_mensura, ex_datum,
            (int)ad_mensura, ad_datum,
            vista.est_praetermissa ? "praetermissa" : "sumpta");
        si (scripti <= ZEPHYRUM)
        {
            perge;
        }
        clavis = chorda_ex_literis(clavis_litterae, piscina_clavium);
        si (tabula_dispersa_continet(inclusiones_visae, clavis))
        {
            inclusiones_duplicatae++;
            perge;
        }
        (vacuum)tabula_dispersa_inserere(inclusiones_visae, clavis,
            NIHIL);
        fprintf(effusio_inclusionum, "%s\n", clavis_litterae);
        inclusiones_scriptae++;
    }
}

/* --------------------------------------------------
 * per plagulam: duo-passus percursus exemplar
 * -------------------------------------------------- */

hic_manens vacuum
_plagulam_percurrere (
    constans SilvaContextus* ctx,
         constans character* via)
{
    Piscina* piscina_textus;
    SilvaPiscina* piscina_arboris;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum_plagulae = NIHIL;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        fprintf(stderr, "[praetermissa mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    /* rung 2 (piscinae communes + vacare) TEMPTATUM ET REVERSUM
     * 2026-07-14: piscina_vacare alvei OMNES memset-purgat
     * (contractus - allocare in memoriam pool-purgatam confidit),
     * ergo reusus = purgatio catenae maximae x800 plagulas = 52s ->
     * 306s PESSIMATIO. Creatio/destructio per plagulam (~16s sys
     * toto) pretium iustum est. */
    piscina_textus = piscina_generare_dynamicum("nexus_textus",
        8388608);
    si (piscina_textus == NIHIL)
    {
        fclose(pl);
        redde;
    }
    fons = (i8*)piscina_allocare(piscina_textus,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (   fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        piscina_destruere(piscina_textus);
        redde;
    }
    fclose(pl);

    piscina_arboris = silva_piscina_generare_dynamicum(
        "nexus_arbor", 8388608);
    si (piscina_arboris == NIHIL)
    {
        piscina_destruere(piscina_textus);
        redde;
    }

    plagulae++;

    oraculum_plagulae = silva_oraculum_creare(piscina_arboris);
    si (oraculum_plagulae != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum_plagulae);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris, ctx,
        via, (constans character*)fons,
        (insignatus integer)mensura, oraculum_plagulae);

    /* graphus inclusionum etiam sine successu semantico validus */
    si (parsura != NIHIL)
    {
        _inclusiones_fundere(parsura->expansio);
    }

    si (parsura != NIHIL && parsura->successus)
    {
        SilvaSemantica* sem =
            silva_c89_semantica_analysare_cum_systemate(
                piscina_arboris, parsura, systema_parsura);

        si (sem != NIHIL && oraculum_plagulae != NIHIL)
        {
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum_plagulae);
            silva_oraculum_responsa_vacare(oraculum_plagulae);
            /* rung 3 scalae: recanonicare numerum mutationum
             * reddit - 0 = arbor intacta, sem1 IAM auctoritas
             * (semantics-praeservans constructione; barra paritatis
             * octetim id probat). sem2 solum post mutationes
             * (canonicae-relativum: symbolum_nodi contra arborem
             * versam). */
            si (silva_recanonicare(parsura->commissio,
                    oraculum_plagulae, silva_c89_resolutor, NIHIL)
                > ZEPHYRUM)
            {
                sem = silva_c89_semantica_analysare_cum_systemate(
                    piscina_arboris, parsura, systema_parsura);
                plagulae_sem2++;
            }
        }
        si (sem != NIHIL)
        {
            nexus_ordines_fundere(parsura, sem, piscina_textus,
                _ordinem_recipere, NIHIL);
        }
        alioquin
        {
            fprintf(stderr, "[SEMANTICA FRACTA] %s\n", via);
        }
    }
    alioquin
    {
        fprintf(stderr, "[SINE ARBORE] %s\n", via);
    }
    /* arbores ANTE textum (arbores in fontem monstrant) */
    silva_piscina_destruere(piscina_arboris);
    piscina_destruere(piscina_textus);
}

/* --------------------------------------------------
 * ambulatio + capita (exemplar percursus/censor)
 * -------------------------------------------------- */

hic_manens b32
_est_fons_c (
    constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    si (m < III)
    {
        redde FALSUM;
    }
    si (titulus[m - II] != '.')
    {
        redde FALSUM;
    }
    redde (titulus[m - I] == 'c' || titulus[m - I] == 'h')
        ? VERUM : FALSUM;
}

hic_manens b32
_praetermittendum (
    constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM
        /* flagrationes consulto plantatae censoris */
        || strcmp(titulus, "censoris") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* rung 5 (1): ambulatio vias COLLIGIT (iudicium separatum) - viae
 * praefixu "./" detracto in piscinam duplicatae, deinde ordinatae */
hic_manens constans character*
_viam_dupare (
    constans character* via)
{
    memoriae_index  m = strlen(via);
         character* nova;

    nova = (character*)piscina_allocare(piscina_textuum, m + I);
    si (nova == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(nova, via, m + I);
    redde nova;
}

hic_manens vacuum
_vias_colligere (
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];

        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        si (_praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);

        si (introitus->d_type == DT_DIR)
        {
            _vias_colligere(via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            constans character* v = via_plena;
            constans character* dup;

            si (v[ZEPHYRUM] == '.' && v[I] == '/')
            {
                v += II;
            }
            dup = _viam_dupare(v);
            si (dup != NIHIL && viae_omnes != NIHIL)
            {
                constans character** sedes =
                    (constans character**)xar_addere(viae_omnes);

                si (sedes != NIHIL)
                {
                    *sedes = dup;
                }
            }
        }
    }
    closedir(dir);
}

hic_manens s32
_viae_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans character* va  = *(constans character* constans*)a;
    constans character* vb  = *(constans character* constans*)b;
               integer  d   = strcmp(va, vb);

    redde (d < ZEPHYRUM) ? -I : (d > ZEPHYRUM ? I : ZEPHYRUM);
}


/* ==================================================
 * rung 5 (2)(3): percursus incrementalis
 * ==================================================
 *
 * Aqua-signum = stampa GENERATUM INTERNA tabulae veteris (numquam
 * mtime tabulae ipsius - lectio venenationis debounce: mtime per
 * touch falsificabilis, stampa sola cum regeneratione vera mutatur).
 *
 * Copia laboris: (a) plagulae quarum mtime >= stampa (aequaevum =
 * suspectum - polaritas _via_recens, non vigiliae); (b) CLAUSURA
 * REVERSA super graphum inclusionum VETEREM. Argumentum sanitatis
 * graphi veteris: plagula G quae caput novum I per caput vetus H
 * trahit - H mutatum est (mtime >= stampa), ergo includentes H per
 * graphum VETEREM G continent (margo G->H vetus exstat). Plagula
 * quae includes SUOS mutavit mtime proprio recentior est - in (a).
 * Casus omnes tecti. (c) deletae = viae veteres non ambulatae;
 * (d) novae = ambulatae sine grege veteri.
 *
 * Fusio: tabula vetus in memoriam onerata, greges per viam
 * (contigui post ordinem canonicum - non-contiguitas = tabula
 * MALFORMATA -> plenus). Emissio ordine canonico: via in copia ->
 * iudicium recens; via retenta -> octeti veteres verbatim; via
 * deleta -> nihil. Dedup tutum: claves ordinum viam continent -
 * collisio trans plagulas impossibilis (verificatum 2026-07-14),
 * dedup intra-copiam sufficit.
 *
 * LIMES NOMINATUS (re-pin (1)): margines ex-attributi = visio
 * plagulae SOLIUS (praeprocessore standalone); margo conditionalis
 * qui solum intra TU alienum accenditur (exemplar stb_image
 * IMPLEMENTATION) e grapho cadit. Auditum ad re-pin: 8/8 margines
 * evanidi ad EXTERNA (stdio.h...) - clausurae innocui, quia externa
 * numquam copiam seminant. Caput repositorii sub condicione alieni
 * TU inclusum limitem hunc excitaret - exemplar quod repositorium
 * non adhibet (worklog).
 *
 * CUSTOS SYSTEMATIS: systema_c89.h in graphum non intrat (canalis
 * lexici, non inclusio - inventum M4b) sed iudicia OMNIA afficit ->
 * mtime eius >= stampa = plenus coactum. Instrumentum ipsum novum
 * (obiecta celer recompilata) = plenus coactum in nexus.sh - tabula
 * mixta duorum parsatorum numquam nascitur. */

#include <sys/stat.h>   /* instrumentum solum - mtime copiae */

nomen structura
{
    constans character* initium;   /* in tabulam veterem oneratam */
        memoriae_index  mensura;   /* octeti gregis (lineis plenis) */
                   i32  lineae;
} GrexVetus;

hic_manens b32  modus_plenus = FALSUM;          /* -plenus */
hic_manens b32  modus_incrementalis = FALSUM;
hic_manens long stampa_vetus = 0L;

hic_manens TabulaDispersa* greges_ordinum = NIHIL;    /* via -> GrexVetus* */
hic_manens TabulaDispersa* greges_marginum = NIHIL;   /* ex  -> GrexVetus* */
hic_manens TabulaDispersa* copia_iudicanda = NIHIL;   /* via -> NIHIL */
hic_manens TabulaDispersa* adjacentia_inversa = NIHIL; /* ad -> Xar*(chorda ex) */
hic_manens TabulaDispersa* affecta = NIHIL;           /* clausura */
hic_manens Xar* frons_clausurae = NIHIL;              /* chorda */
hic_manens Xar* ads_visae = NIHIL;                    /* chorda (pro umbris basename) */

hic_manens i32 stat_directae = ZEPHYRUM;
hic_manens i32 stat_clausura = ZEPHYRUM;
hic_manens i32 stat_deletae = ZEPHYRUM;
hic_manens i32 stat_novae = ZEPHYRUM;
hic_manens i32 ordines_retenti = ZEPHYRUM;
hic_manens i32 margines_retenti = ZEPHYRUM;

/* chorda quae in litteras exsistentes monstrat (sine copia) */
hic_manens chorda
_chordam_alligare (
    constans character* datum,
        memoriae_index  mensura)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l        = datum;
    c.datum    = u.m;
    c.mensura  = (i32)mensura;
    redde c;
}

/* plagula tota in piscinam PROPRIAM mensurae plenae onerata
 * (piscina vita processus manet - consulto; tabula vetus 48MB) */
hic_manens character*
_plagulam_totam_legere (
    constans character* via,
    constans character* titulus_piscinae,
        memoriae_index* mensura_out)
{
    FILE* pl;
    long m_l;
    memoriae_index m;
    Piscina* p;
    character* datum;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    m_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (m_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    m = (memoriae_index)m_l;
    p = piscina_generare_dynamicum(titulus_piscinae, m + 4096);
    si (p == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    datum = (character*)piscina_allocare(p, m + I);
    si (   datum == NIHIL
        || (m > ZEPHYRUM && fread(datum, I, m, pl) != m))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    datum[m]      = '\0';
    *mensura_out  = m;
    redde datum;
}

/* greges contigui campo clave (0-basato, tab separato) aedificare.
 * FALSUM = tabula vetus malformata (clavis non contigua = forma
 * ante ordinem canonicum, aut campus deest) -> plenus. Claves in
 * tabulam oneratam monstrant. */
hic_manens b32
_greges_aedificare (
    constans character* datum,
        memoriae_index  mensura,
                   i32  lineae_praefationis,
                   i32  campus,
        TabulaDispersa* greges,
                   Xar* claves)
{
    constans character* cursor  = datum;
    constans character* finis   = datum + mensura;
                chorda  clavis_aperta;
    constans character* initium_gregis  = NIHIL;
                   i32  lineae_gregis   = ZEPHYRUM;
                   i32  n_praef          =
                       lineae_praefationis;

    clavis_aperta.datum    = NIHIL;
    clavis_aperta.mensura  = ZEPHYRUM;

    dum (cursor < finis)
    {
        constans character* linea = cursor;
        constans character* nl = (constans character*)memchr(cursor,
            '\n', (memoriae_index)(finis - cursor));
        constans character* finis_lineae = (nl != NIHIL) ? nl : finis;
        constans character* c;
        constans character* campus_init;
                       i32  k;
                    chorda  clavis;

        cursor = (nl != NIHIL) ? nl + I : finis;
        si (n_praef > ZEPHYRUM)
        {
            n_praef--;
            perge;
        }
        si (linea == finis_lineae)
        {
            perge;
        }
        si (*linea == '#')
        {
            /* lapis "# vacua <via>": plagula iudicata quae nihil
             * emisit - grex nullarum linearum, ne nova aeterna
             * fiat (fixturae roundtrip invalidae). Linea aliena
             * '#' silenter praetermittitur. */
            si (   (memoriae_index)(finis_lineae - linea) > VIII
                && memcmp(linea, "# vacua ", VIII) == ZEPHYRUM)
            {
                chorda clavis_vacua = _chordam_alligare(
                    linea + VIII,
                    (memoriae_index)(finis_lineae
                        - (linea + VIII)));

                si (clavis_aperta.datum != NIHIL)
                {
                    GrexVetus* g = (GrexVetus*)piscina_allocare(
                        piscina_clavium, magnitudo(GrexVetus));

                    si (g == NIHIL)
                    {
                        redde FALSUM;
                    }
                    g->initium = initium_gregis;
                    g->mensura =
                        (memoriae_index)(linea - initium_gregis);
                    g->lineae = lineae_gregis;
                    (vacuum)tabula_dispersa_inserere(greges,
                        clavis_aperta, g);
                    clavis_aperta.datum    = NIHIL;
                    clavis_aperta.mensura  = ZEPHYRUM;
                }
                si (tabula_dispersa_continet(greges, clavis_vacua))
                {
                    redde FALSUM;
                }
                {
                    GrexVetus* g = (GrexVetus*)piscina_allocare(
                        piscina_clavium, magnitudo(GrexVetus));

                    si (g == NIHIL)
                    {
                        redde FALSUM;
                    }
                    g->initium  = linea;
                    g->mensura  = (memoriae_index)(cursor - linea);
                    g->lineae   = ZEPHYRUM;
                    (vacuum)tabula_dispersa_inserere(greges,
                        clavis_vacua, g);
                }
                si (claves != NIHIL)
                {
                    chorda* s = (chorda*)xar_addere(claves);

                    si (s != NIHIL)
                    {
                        *s = clavis_vacua;
                    }
                }
            }
            perge;
        }
        c            = linea;
        campus_init  = linea;
        k            = ZEPHYRUM;
        dum (k < campus && c < finis_lineae)
        {
            si (*c == '\t')
            {
                k++;
                campus_init = c + I;
            }
            c++;
        }
        si (k < campus)
        {
            redde FALSUM;
        }
        c = campus_init;
        dum (c < finis_lineae && *c != '\t')
        {
            c++;
        }
        clavis = _chordam_alligare(campus_init,
            (memoriae_index)(c - campus_init));

        si (   clavis_aperta.datum != NIHIL
            && clavis.mensura      == clavis_aperta.mensura
            && memcmp(clavis.datum, clavis_aperta.datum,
                   (memoriae_index)clavis.mensura) == ZEPHYRUM)
        {
            lineae_gregis++;
            perge;
        }
        /* grex prior clauditur (mensura = initium lineae huius) */
        si (clavis_aperta.datum != NIHIL)
        {
            GrexVetus* g = (GrexVetus*)piscina_allocare(
                piscina_clavium, magnitudo(GrexVetus));

            si (g == NIHIL)
            {
                redde FALSUM;
            }
            g->initium  = initium_gregis;
            g->mensura  = (memoriae_index)(linea - initium_gregis);
            g->lineae   = lineae_gregis;
            (vacuum)tabula_dispersa_inserere(greges, clavis_aperta,
                g);
        }
        si (tabula_dispersa_continet(greges, clavis))
        {
            redde FALSUM;   /* NON CONTIGUA */
        }
        clavis_aperta   = clavis;
        initium_gregis  = linea;
        lineae_gregis   = I;
        si (claves != NIHIL)
        {
            chorda* s = (chorda*)xar_addere(claves);

            si (s != NIHIL)
            {
                *s = clavis;
            }
        }
    }
    si (clavis_aperta.datum != NIHIL)
    {
        GrexVetus* g = (GrexVetus*)piscina_allocare(piscina_clavium,
            magnitudo(GrexVetus));

        si (g == NIHIL)
        {
            redde FALSUM;
        }
        g->initium  = initium_gregis;
        g->mensura  = (memoriae_index)(finis - initium_gregis);
        g->lineae   = lineae_gregis;
        (vacuum)tabula_dispersa_inserere(greges, clavis_aperta, g);
    }
    redde VERUM;
}

/* adjacentia inversa graphi veteris: ad -> Xar exium (clausura
 * reversa ab ea currit). Status (sumpta|praetermissa) AMBO numerant
 * - coniunctio conservativa. */
hic_manens vacuum
_adjacentiam_aedificare (
    constans character* datum,
        memoriae_index  mensura)
{
    constans character* cursor   = datum;
    constans character* finis    = datum + mensura;
                   i32  n_praef  = II;

    dum (cursor < finis)
    {
        constans character* linea = cursor;
        constans character* nl = (constans character*)memchr(cursor,
            '\n', (memoriae_index)(finis - cursor));
        constans character* finis_lineae = (nl != NIHIL) ? nl : finis;
        constans character* t1;
        constans character* t2;
                    chorda  ex;
                    chorda  ad;
                    vacuum* valor;
                       Xar* exs;

        cursor = (nl != NIHIL) ? nl + I : finis;
        si (n_praef > ZEPHYRUM)
        {
            n_praef--;
            perge;
        }
        si (linea == finis_lineae)
        {
            perge;
        }
        t1 = (constans character*)memchr(linea, '\t',
            (memoriae_index)(finis_lineae - linea));
        si (t1 == NIHIL)
        {
            perge;
        }
        t2 = (constans character*)memchr(t1 + I, '\t',
            (memoriae_index)(finis_lineae - (t1 + I)));
        si (t2 == NIHIL)
        {
            perge;
        }
        ex = _chordam_alligare(linea, (memoriae_index)(t1 - linea));
        ad = _chordam_alligare(t1 + I, (memoriae_index)(t2 - (t1 + I)));
        si (ex.mensura == ZEPHYRUM || ad.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (   tabula_dispersa_invenire(adjacentia_inversa, ad, &valor)
            && valor != NIHIL)
        {
            exs = (Xar*)valor;
        }
        alioquin
        {
            exs = xar_creare(piscina_clavium, (i32)magnitudo(chorda));
            si (exs == NIHIL)
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(adjacentia_inversa, ad,
                exs);
            si (ads_visae != NIHIL)
            {
                chorda* s = (chorda*)xar_addere(ads_visae);

                si (s != NIHIL)
                {
                    *s = ad;
                }
            }
        }
        {
            chorda* s = (chorda*)xar_addere(exs);

            si (s != NIHIL)
            {
                *s = ex;
            }
        }
    }
}

/* semen clausurae (idempotens): in affecta + frontem */
hic_manens vacuum
_afficere (
    chorda via)
{
    chorda* s;

    si (   affecta == NIHIL || via.mensura == ZEPHYRUM
        || tabula_dispersa_continet(affecta, via))
    {
        redde;
    }
    (vacuum)tabula_dispersa_inserere(affecta, via, NIHIL);
    s = (chorda*)xar_addere(frons_clausurae);
    si (s != NIHIL)
    {
        *s = via;
    }
}

hic_manens chorda
_basis_chordae (
    chorda via)
{
    i32 k = via.mensura;

    dum (k > ZEPHYRUM && via.datum[k - I] != '/')
    {
        k--;
    }
    redde _chordam_alligare((constans character*)via.datum + k,
        (memoriae_index)(via.mensura - k));
}

/* praeparatio incrementalis (viae_omnes iam collectae/ordinatae).
 * FALSUM cum causa in stderr -> plenus (numquam tacite). VERUM ->
 * greges_*, copia_iudicanda, stat_* impleta. */
hic_manens b32
_incrementale_praeparare (vacuum)
{
        memoriae_index  m_ord   = ZEPHYRUM;
        memoriae_index  m_marg  = ZEPHYRUM;
    constans character* vetus_ord;
    constans character* vetus_marg;
        TabulaDispersa* ambulatae;
        TabulaDispersa* directae;
        TabulaDispersa* deletae_visae;
                   Xar* claves_ordinum;
                   Xar* claves_marginum;
                   i32  n;
                   i32  j;

    vetus_ord = _plagulam_totam_legere("build/nexus.tsv",
        "nexus_vetus_ordines", &m_ord);
    si (vetus_ord == NIHIL)
    {
        fprintf(stderr, "incrementale non possibile: nexus.tsv"
            " vetus deest -> plenus\n");
        redde FALSUM;
    }
    si (   sscanf(vetus_ord, "# nexus.tsv GENERATUM %ld",
            &stampa_vetus) != I
        || stampa_vetus <= 0L)
    {
        fprintf(stderr, "incrementale non possibile: stampa"
            " GENERATUM illegibilis -> plenus\n");
        redde FALSUM;
    }
    vetus_marg = _plagulam_totam_legere("build/inclusiones.tsv",
        "nexus_vetus_margines", &m_marg);
    si (vetus_marg == NIHIL)
    {
        fprintf(stderr, "incrementale non possibile:"
            " inclusiones.tsv vetus deest -> plenus\n");
        redde FALSUM;
    }

    /* custos systematis: canalis lexici extra graphum, iudicia
     * OMNIA afficit */
    {
        structura stat st;

        si (   stat("silva/fontes/systema_c89.h", &st) != ZEPHYRUM
            || (long)st.st_mtime                       >= stampa_vetus)
        {
            fprintf(stderr, "incrementale non possibile: systema"
                " mutatum -> plenus\n");
            redde FALSUM;
        }
    }

    greges_ordinum = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    greges_marginum = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    adjacentia_inversa = tabula_dispersa_creare_chorda(
        piscina_clavium, DXII);
    affecta = tabula_dispersa_creare_chorda(piscina_clavium, DXII);
    copia_iudicanda = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    ambulatae  = tabula_dispersa_creare_chorda(piscina_clavium, DXII);
    directae   = tabula_dispersa_creare_chorda(piscina_clavium, DXII);
    deletae_visae = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    claves_ordinum = xar_creare(piscina_clavium,
        (i32)magnitudo(chorda));
    claves_marginum = xar_creare(piscina_clavium,
        (i32)magnitudo(chorda));
    ads_visae = xar_creare(piscina_clavium, (i32)magnitudo(chorda));
    frons_clausurae = xar_creare(piscina_clavium,
        (i32)magnitudo(chorda));
    si (   greges_ordinum     == NIHIL || greges_marginum == NIHIL
        || adjacentia_inversa == NIHIL || affecta == NIHIL
        || copia_iudicanda    == NIHIL || ambulatae == NIHIL
        || directae           == NIHIL || deletae_visae == NIHIL
        || claves_ordinum     == NIHIL || claves_marginum == NIHIL
        || ads_visae          == NIHIL || frons_clausurae == NIHIL)
    {
        fprintf(stderr, "incrementale non possibile: memoria"
            " -> plenus\n");
        redde FALSUM;
    }

    si (!_greges_aedificare(vetus_ord, m_ord, I, III,
            greges_ordinum, claves_ordinum))
    {
        fprintf(stderr, "incrementale non possibile: nexus.tsv"
            " vetus non contiguus (forma ante ordinem canonicum?)"
            " -> plenus\n");
        redde FALSUM;
    }
    si (!_greges_aedificare(vetus_marg, m_marg, II, ZEPHYRUM,
            greges_marginum, claves_marginum))
    {
        fprintf(stderr, "incrementale non possibile:"
            " inclusiones.tsv vetus non contiguus -> plenus\n");
        redde FALSUM;
    }
    _adjacentiam_aedificare(vetus_marg, m_marg);

    n = xar_numerus(viae_omnes);
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans character* v =
            *(constans character**)xar_obtinere(viae_omnes, j);

        (vacuum)tabula_dispersa_inserere(ambulatae,
            _chordam_alligare(v, strlen(v)), NIHIL);
    }

    /* (a) directae per mtime (>= : aequaevum suspectum, polaritas
     * _via_recens) + (d) novae */
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans character* v =
            *(constans character**)xar_obtinere(viae_omnes, j);
        chorda cv = _chordam_alligare(v, strlen(v));
        structura stat st;

        si (stat(v, &st) != ZEPHYRUM)
        {
            perge;   /* evanuit inter collectionem et hic - cursus
                      * proximus deletam videt */
        }
        si ((long)st.st_mtime >= stampa_vetus)
        {
            (vacuum)tabula_dispersa_inserere(directae, cv, NIHIL);
            _afficere(cv);
        }
        alioquin si (   !tabula_dispersa_continet(greges_ordinum, cv)
                     && !tabula_dispersa_continet(greges_marginum, cv))
        {
            /* sine grege veteri = nova - NISI mensura eam
             * praetermitteret (regula eadem qua iudicium) */
            si (   mensura_maxima > ZEPHYRUM
                && (long)st.st_size > (long)mensura_maxima)
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(copia_iudicanda, cv,
                NIHIL);
            stat_novae++;
            /* umbra basename: caput novum resolutiones praebere
             * mutare potest (primus-vincit) - semina basename ipsum
             * (margines ad non resoluti) + ads veteres eiusdem
             * basename */
            {
                chorda basis  = _basis_chordae(cv);
                   i32 na     = xar_numerus(ads_visae);
                   i32 ka;

                _afficere(basis);
                per (ka = ZEPHYRUM; ka < na; ka++)
                {
                    chorda ad = *(chorda*)xar_obtinere(ads_visae,
                        ka);
                    chorda basis_ad = _basis_chordae(ad);

                    si (   basis_ad.mensura == basis.mensura
                        && memcmp(basis_ad.datum, basis.datum,
                               (memoriae_index)basis.mensura)
                            == ZEPHYRUM)
                    {
                        _afficere(ad);
                    }
                }
            }
        }
    }

    /* (c) deletae: claves veteres non ambulatae - semina quoque
     * (includentes capitis deleti re-iudicandi; umbratio inversa
     * per easdem margines tecta) */
    {
        i32 nk = xar_numerus(claves_ordinum);
        i32 kk;

        per (kk = ZEPHYRUM; kk < nk; kk++)
        {
            chorda cl = *(chorda*)xar_obtinere(claves_ordinum, kk);

            si (   !tabula_dispersa_continet(ambulatae, cl)
                && !tabula_dispersa_continet(deletae_visae, cl))
            {
                (vacuum)tabula_dispersa_inserere(deletae_visae, cl,
                    NIHIL);
                stat_deletae++;
                _afficere(cl);
            }
        }
        nk = xar_numerus(claves_marginum);
        per (kk = ZEPHYRUM; kk < nk; kk++)
        {
            chorda cl = *(chorda*)xar_obtinere(claves_marginum, kk);

            si (   !tabula_dispersa_continet(ambulatae, cl)
                && !tabula_dispersa_continet(deletae_visae, cl))
            {
                (vacuum)tabula_dispersa_inserere(deletae_visae, cl,
                    NIHIL);
                stat_deletae++;
                _afficere(cl);
            }
        }
    }

    /* clausura reversa (BFS super graphum VETEREM - argumentum
     * sanitatis in commentario sectionis) */
    {
        i32 caput = ZEPHYRUM;

        dum (caput < xar_numerus(frons_clausurae))
        {
            chorda ad = *(chorda*)xar_obtinere(frons_clausurae,
                caput);
            vacuum* valor;

            caput++;
            si (   tabula_dispersa_invenire(adjacentia_inversa, ad,
                    &valor)
                && valor != NIHIL)
            {
                Xar* exs  = (Xar*)valor;
                i32  ne   = xar_numerus(exs);
                i32  ke;

                per (ke = ZEPHYRUM; ke < ne; ke++)
                {
                    _afficere(*(chorda*)xar_obtinere(exs, ke));
                }
            }
        }
    }

    /* copia = (affecta ∩ ambulatae) ∪ novae (iam insertae) */
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans character* v =
            *(constans character**)xar_obtinere(viae_omnes, j);
        chorda cv = _chordam_alligare(v, strlen(v));

        si (   tabula_dispersa_continet(affecta, cv)
            && !tabula_dispersa_continet(copia_iudicanda, cv))
        {
            (vacuum)tabula_dispersa_inserere(copia_iudicanda, cv,
                NIHIL);
            si (tabula_dispersa_continet(directae, cv))
            {
                stat_directae++;
            }
            alioquin
            {
                stat_clausura++;
            }
        }
    }
    redde VERUM;
}

/* iudicium plagulae unius + lapis vacuae: si iudicata nihil emisit
 * (nec mensura praetermissa - illa regula sua excluditur ut -omnia
 * postea eas capiat), "# vacua <via>" scribitur ne nova aeterna
 * fiat. Ambobus modis (plenus quoque - tabula plena lapides fert
 * quos cursus incrementalis sequens legit). */
hic_manens vacuum
_plagulam_iudicare (
    constans SilvaContextus* ctx,
         constans character* via)
{
    i32 ord0    = ordines_scripti;
    i32 marg0   = inclusiones_scriptae;
    i32 praet0  = praetermissae;

    via_activa = via;
    _plagulam_percurrere(ctx, via);
    via_activa = NIHIL;
    si (   ordines_scripti == ord0 && inclusiones_scriptae == marg0
        && praetermissae   == praet0 && effusio != NIHIL)
    {
        fprintf(effusio, "# vacua %s\n", via);
    }
}

/* systema semel parsatum (exemplar percursus -semantica) - solum
 * quando iudicia futura sunt (transitus nulla-mutatione hoc
 * praeterit: solum floor ambulationis + stat) */
hic_manens b32
_systema_praeparare (
    SilvaContextus* ctx,
      SilvaPiscina* piscina_arboris_ctx)
{
         FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
         long  mensura_sys;
    character* fons_sys;

    si (pl_sys == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: systema_c89.h deest"
            " (curre ex radice repositorii)\n");
        redde FALSUM;
    }
    fseek(pl_sys, 0L, SEEK_END);
    mensura_sys = ftell(pl_sys);
    fseek(pl_sys, 0L, SEEK_SET);
    fons_sys = (character*)piscina_allocare(piscina_textuum,
        (memoriae_index)(mensura_sys + 1L));
    si (   fons_sys == NIHIL
        || fread(fons_sys, I, (memoriae_index)mensura_sys,
               pl_sys) != (memoriae_index)mensura_sys)
    {
        fprintf(stderr,
            "nexus_percursus: systema non lectum\n");
        fclose(pl_sys);
        redde FALSUM;
    }
    fclose(pl_sys);
    si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
            fons_sys, (insignatus integer)mensura_sys))
    {
        fprintf(stderr,
            "nexus_percursus: lexicon non additum\n");
        redde FALSUM;
    }
    systema_parsura = silva_c89_parsare(piscina_arboris_ctx,
        "systema_c89.h", fons_sys,
        (insignatus integer)mensura_sys, NIHIL);
    si (   systema_parsura == NIHIL
        || systema_parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr,
            "nexus_percursus: systema non parsatum\n");
        redde FALSUM;
    }
    systema_semantica = silva_c89_semantica_analysare(
        piscina_arboris_ctx, systema_parsura);
    si (systema_semantica == NIHIL)
    {
        fprintf(stderr,
            "nexus_percursus: systema non analysatum\n");
        redde FALSUM;
    }
    redde VERUM;
}

hic_manens vacuum
_caput_praebere (
        SilvaContextus* ctx,
        TabulaDispersa* visa,
    constans character* via,
    constans character* titulus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina_textuum);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;
    }

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;

    textus = (character*)piscina_allocare(piscina_textuum,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (   textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde;
    }
    fclose(pl);

    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);

        /* EXCUBITOR: basename -> via plena ("./" detractum) - primus
         * vincit, semantica praebere ipsius speculatur */
        si (capita_viae != NIHIL)
        {
                        chorda* via_plena;
            constans character* v;

            via_plena = (chorda*)piscina_allocare(piscina_textuum,
                magnitudo(chorda));
            si (via_plena != NIHIL)
            {
                v = via;
                si (v[ZEPHYRUM] == '.' && v[I] == '/')
                {
                    v += II;
                }
                *via_plena = chorda_ex_literis(v, piscina_textuum);
                (vacuum)tabula_dispersa_inserere(capita_viae, clavis,
                    via_plena);
            }
        }
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
    SilvaPiscina* piscina_arboris_ctx;
    SilvaContextus* ctx;
    constans character* radix = ".";
    /* scriptura ATOMICA: .nova + rename - lectores (nexus CLI,
     * legatus ad initialize, uncus post-commissionem in fundo)
     * numquam tabulam truncatam vident */
    constans character* via_effusionis = "build/nexus.tsv.nova";
    constans character* via_finalis = "build/nexus.tsv";
    constans character* via_incl_effusionis =
        "build/inclusiones.tsv.nova";
    constans character* via_incl_finalis = "build/inclusiones.tsv";
    integer k;
    clock_t c0;
    clock_t c1;
    time_t nunc_stampa;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (strcmp(argv[k], "-plenus") == ZEPHYRUM)
        {
            modus_plenus = VERUM;
        }
        alioquin
        {
            radix = argv[k];
        }
    }

    piscina_arboris_ctx = silva_piscina_generare_dynamicum(
        "nexus_ctx", 8388608);
    si (piscina_arboris_ctx == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: piscina deest\n");
        redde I;
    }
    piscina_clavium = piscina_generare_dynamicum("nexus_claves",
        8388608);
    piscina_textuum = piscina_generare_dynamicum("nexus_textus_ctx",
        8388608);
    si (piscina_clavium == NIHIL || piscina_textuum == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: piscina clavium deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_arboris_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: contextus deest\n");
        redde I;
    }
    clavium_visa = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    inclusiones_visae = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    capita_viae = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    si (   clavium_visa == NIHIL || inclusiones_visae == NIHIL
        || capita_viae  == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: tabula deest\n");
        redde I;
    }

    /* stampa nova ANTE ambulationem capta (conservativum: editiones
     * durante cursu proximo cursui cadunt, numquam perduntur) */
    nunc_stampa = time(NIHIL);

    /* rung 5 (1): collectio + ordinatio canonica ANTE praebitionem
     * capitum et iudicia - primus-vincit basename duplicatorum nunc
     * deterministicus (include/ ante probationes/, quinque paria
     * fixturarum roundtrip) */
    viae_omnes = xar_creare(piscina_clavium,
        (i32)magnitudo(constans character*));
    si (viae_omnes == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: xar deest\n");
        redde I;
    }
    _vias_colligere(radix);
    xar_ordinare(viae_omnes, _viae_comparare);

    /* rung 5 (2): modus incrementalis temptatur (radix "." solum -
     * viae tabulae veteris radici-relativae sunt) */
    si (!modus_plenus && strcmp(radix, ".") == ZEPHYRUM)
    {
        modus_incrementalis = _incrementale_praeparare();
    }
    si (   modus_incrementalis
        && stat_directae + stat_clausura + stat_novae + stat_deletae
            == ZEPHYRUM)
    {
        fprintf(stderr, "incrementale: nulla mutatio - tabulae"
            " intactae\n");
        redde ZEPHYRUM;
    }

    /* systema + capita solum quando iudicia futura (cursus
     * deletione-sola eas praeterit) */
    si (   !modus_incrementalis
        || stat_directae + stat_clausura + stat_novae > ZEPHYRUM)
    {
        si (!_systema_praeparare(ctx, piscina_arboris_ctx))
        {
            redde I;
        }
        {
            TabulaDispersa* visa = tabula_dispersa_creare_chorda(
                piscina_clavium, DXII);
            i32 n = xar_numerus(viae_omnes);
            i32 j;

            si (visa == NIHIL)
            {
                fprintf(stderr, "nexus_percursus: tabula deest\n");
                redde I;
            }
            per (j = ZEPHYRUM; j < n; j++)
            {
                constans character* v =
                    *(constans character**)xar_obtinere(viae_omnes,
                        j);
                memoriae_index m = strlen(v);

                si (v[m - I] == 'h')
                {
                    constans character* basis = strrchr(v, '/');

                    _caput_praebere(ctx, visa, v,
                        basis != NIHIL ? basis + I : v);
                }
            }
        }
    }

    effusio = fopen(via_effusionis, "w");
    si (effusio == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: %s non aperta"
            " (build/ adest?)\n", via_effusionis);
        redde I;
    }
    effusio_inclusionum = fopen(via_incl_effusionis, "w");
    si (effusio_inclusionum == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: %s non aperta\n",
            via_incl_effusionis);
        redde I;
    }
    fprintf(effusio, "# nexus.tsv GENERATUM %ld"
        " - DISPONIBILE, noli committere;"
        " regenera: ./silva/nexus.sh -renovare\n",
        (long)nunc_stampa);
    fprintf(effusio_inclusionum, "# inclusiones.tsv GENERATUM %ld"
        " - DISPONIBILE, noli committere;"
        " regenera: ./silva/nexus.sh -renovare\n"
        "# ex\tad\tstatus(sumpta|praetermissa)\n",
        (long)nunc_stampa);

    c0 = clock();
    {
        i32 n = xar_numerus(viae_omnes);
        i32 j;

        per (j = ZEPHYRUM; j < n; j++)
        {
            constans character* v =
                *(constans character**)xar_obtinere(viae_omnes, j);
            chorda  cv;
            vacuum* valor;

            si (!modus_incrementalis)
            {
                _plagulam_iudicare(ctx, v);
                perge;
            }
            /* rung 5 (3): fusio fluminis ordine canonico - via in
             * copia iudicatur recens, via retenta octetim vetus,
             * via deleta (non ambulata) numquam attingitur */
            cv = _chordam_alligare(v, strlen(v));
            si (tabula_dispersa_continet(copia_iudicanda, cv))
            {
                _plagulam_iudicare(ctx, v);
                perge;
            }
            si (   tabula_dispersa_invenire(greges_ordinum, cv, &valor)
                && valor != NIHIL)
            {
                constans GrexVetus* g = (constans GrexVetus*)valor;

                fwrite(g->initium, I, g->mensura, effusio);
                ordines_retenti += g->lineae;
            }
            si (   tabula_dispersa_invenire(greges_marginum, cv,
                    &valor)
                && valor != NIHIL)
            {
                constans GrexVetus* g = (constans GrexVetus*)valor;

                fwrite(g->initium, I, g->mensura,
                    effusio_inclusionum);
                margines_retenti += g->lineae;
            }
        }
        via_activa = NIHIL;
    }
    c1 = clock();
    fclose(effusio);
    fclose(effusio_inclusionum);
    si (rename(via_effusionis, via_finalis) != ZEPHYRUM)
    {
        fprintf(stderr, "nexus_percursus: rename %s -> %s fractum\n",
            via_effusionis, via_finalis);
        redde I;
    }
    si (rename(via_incl_effusionis, via_incl_finalis) != ZEPHYRUM)
    {
        fprintf(stderr, "nexus_percursus: rename %s -> %s fractum\n",
            via_incl_effusionis, via_incl_finalis);
        redde I;
    }

    fprintf(stderr, "\n=== NEXUS PERCURSUS ===\n");
    fprintf(stderr, "plagulae: %d", (int)plagulae);
    si (praetermissae > ZEPHYRUM)
    {
        fprintf(stderr, "  [+%d praetermissae]",
            (int)praetermissae);
    }
    fprintf(stderr, "\nordines:  %d scripti; %d duplicati"
        " (capita inclusa)\n", (int)ordines_scripti,
        (int)ordines_duplicati);
    fprintf(stderr, "margines: %d scripti; %d duplicati;"
        " %d alieni omissi\n", (int)inclusiones_scriptae,
        (int)inclusiones_duplicatae, (int)inclusiones_alienae);
    si (ordines_alieni > ZEPHYRUM)
    {
        fprintf(stderr, "CAUTIO: %d ordines alieni omissi"
            " (praesumptio attributionis fracta!)\n",
            (int)ordines_alieni);
    }
    fprintf(stderr, "sem2:     %d / %d plagulae (recanonicare"
        " mutavit)\n", (int)plagulae_sem2, (int)plagulae);
    si (modus_incrementalis)
    {
        fprintf(stderr, "incrementale: %d re-iudicatae (%d per"
            " clausuram capitum), %d deletae, %d novae;"
            " ordines retenti %d, margines retenti %d\n",
            (int)(stat_directae + stat_clausura),
            (int)stat_clausura, (int)stat_deletae,
            (int)stat_novae, (int)ordines_retenti,
            (int)margines_retenti);
    }
    fprintf(stderr, "tempus:   %.0f s\n",
        (duplex)(c1 - c0) / (duplex)CLOCKS_PER_SEC);
    fprintf(stderr, "tabula:   %s\n", via_finalis);
    fprintf(stderr, "graphus:  %s\n", via_incl_finalis);
    redde ZEPHYRUM;
}
