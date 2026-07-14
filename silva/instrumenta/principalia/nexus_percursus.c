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
 * praetermittunt. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
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

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

/* receptor sweep: dedup per clavem plenam + fprintf (praefixum
 * "./" ambulationis detractum - cosmetica viae tabulae) */
hic_manens vacuum
_ordinem_recipere (vacuum* datum, constans SilvaChorda* titulus,
    constans character* species, constans character* genus,
    constans SilvaChorda* via, insignatus integer linea,
    insignatus integer columna, insignatus integer profunditas)
{
    character clavis_litterae[1024];
    chorda clavis;
    int scripti;
    constans insignatus character* via_datum;
    insignatus integer via_mensura;

    (vacuum)datum;
    si (effusio == NIHIL || titulus == NIHIL || via == NIHIL)
    {
        redde;
    }
    via_datum = via->datum;
    via_mensura = via->mensura;
    si (via_mensura > II && via_datum[ZEPHYRUM] == '.'
        && via_datum[I] == '/')
    {
        via_datum += II;
        via_mensura -= II;
    }
    si (titulus->mensura == ZEPHYRUM
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
_viam_solvere (constans SilvaChorda* via,
    constans character** datum_out, insignatus integer* mensura_out)
{
    constans insignatus character* datum;
    insignatus integer mensura;

    datum = via->datum;
    mensura = via->mensura;
    si (mensura > II && datum[ZEPHYRUM] == '.' && datum[I] == '/')
    {
        datum += II;
        mensura -= II;
    }
    si (capita_viae != NIHIL && mensura > ZEPHYRUM
        && memchr(datum, '/', (memoriae_index)mensura) == NIHIL)
    {
        chorda clavis;
        vacuum* valor;
        unio { constans insignatus character* c; i8* m; } u;

        u.c = datum;
        clavis.datum = u.m;
        clavis.mensura = (i32)mensura;
        si (tabula_dispersa_invenire(capita_viae, clavis, &valor)
            && valor != NIHIL)
        {
            constans chorda* plena = (constans chorda*)valor;

            *datum_out = (constans character*)plena->datum;
            *mensura_out = (insignatus integer)plena->mensura;
            redde;
        }
    }
    *datum_out = (constans character*)datum;
    *mensura_out = mensura;
}

/* graphus inclusionum TU huius fundere: ordo = ex TAB ad TAB status
 * (sumpta|praetermissa). Dedup clavi plena - visio prima et
 * praetermissa et sumpta eiusdem paris ambae supersunt (tabula =
 * database; consumptores ordines ut margines dedupant) */
hic_manens vacuum
_inclusiones_fundere (constans SilvaExpansio* exp)
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
        SilvaInclusioVista vista;
        constans SilvaChorda* via_ex;
        constans SilvaChorda* via_ad;
        constans character* ex_datum;
        insignatus integer ex_mensura;
        constans character* ad_datum;
        insignatus integer ad_mensura;
        character clavis_litterae[2304];
        chorda clavis;
        int scripti;

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
        si (ex_mensura == ZEPHYRUM || ad_mensura == ZEPHYRUM
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
_plagulam_percurrere (constans SilvaContextus* ctx,
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
    si (fons == NIHIL || (mensura > ZEPHYRUM
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
_est_fons_c (constans character* titulus)
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
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM
        /* flagrationes consulto plantatae censoris */
        || strcmp(titulus, "censoris") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_directorium_percurrere (constans SilvaContextus* ctx,
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
            _directorium_percurrere(ctx, via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            _plagulam_percurrere(ctx, via_plena);
        }
    }
    closedir(dir);
}

hic_manens vacuum
_caput_praebere (SilvaContextus* ctx, TabulaDispersa* visa,
    constans character* via, constans character* titulus)
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
    si (textus == NIHIL || (mensura > ZEPHYRUM
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

hic_manens vacuum
_capita_praeparare (SilvaContextus* ctx, TabulaDispersa* visa,
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
        memoriae_index m;

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
            _capita_praeparare(ctx, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_praebere(ctx, visa, via_plena,
                    introitus->d_name);
            }
        }
    }
    closedir(dir);
}

s32 principale (integer argc, character** argv)
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

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
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
    si (clavium_visa == NIHIL || inclusiones_visae == NIHIL
        || capita_viae == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: tabula deest\n");
        redde I;
    }

    /* systema semel parsatum (exemplar percursus -semantica) */
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long mensura_sys;
        character* fons_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "nexus_percursus: systema_c89.h deest"
                " (curre ex radice repositorii)\n");
            redde I;
        }
        fseek(pl_sys, 0L, SEEK_END);
        mensura_sys = ftell(pl_sys);
        fseek(pl_sys, 0L, SEEK_SET);
        fons_sys = (character*)piscina_allocare(piscina_textuum,
            (memoriae_index)(mensura_sys + 1L));
        si (fons_sys == NIHIL
            || fread(fons_sys, I, (memoriae_index)mensura_sys,
                   pl_sys) != (memoriae_index)mensura_sys)
        {
            fprintf(stderr,
                "nexus_percursus: systema non lectum\n");
            fclose(pl_sys);
            redde I;
        }
        fclose(pl_sys);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (insignatus integer)mensura_sys))
        {
            fprintf(stderr,
                "nexus_percursus: lexicon non additum\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_arboris_ctx,
            "systema_c89.h", fons_sys,
            (insignatus integer)mensura_sys, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr,
                "nexus_percursus: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_arboris_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr,
                "nexus_percursus: systema non analysatum\n");
            redde I;
        }
    }

    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_clavium, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "nexus_percursus: tabula deest\n");
            redde I;
        }
        _capita_praeparare(ctx, visa, ".");
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
    {
        time_t nunc = time(NIHIL);

        fprintf(effusio, "# nexus.tsv GENERATUM %ld"
            " - DISPONIBILE, noli committere;"
            " regenera: ./silva/nexus.sh -renovare\n",
            (long)nunc);
        fprintf(effusio_inclusionum, "# inclusiones.tsv GENERATUM %ld"
            " - DISPONIBILE, noli committere;"
            " regenera: ./silva/nexus.sh -renovare\n"
            "# ex\tad\tstatus(sumpta|praetermissa)\n",
            (long)nunc);
    }

    c0 = clock();
    _directorium_percurrere(ctx, radix);
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
    fprintf(stderr, "margines: %d scripti; %d duplicati\n",
        (int)inclusiones_scriptae, (int)inclusiones_duplicatae);
    fprintf(stderr, "sem2:     %d / %d plagulae (recanonicare"
        " mutavit)\n", (int)plagulae_sem2, (int)plagulae);
    fprintf(stderr, "tempus:   %.0f s\n",
        (duplex)(c1 - c0) / (duplex)CLOCKS_PER_SEC);
    fprintf(stderr, "tabula:   %s\n", via_finalis);
    fprintf(stderr, "graphus:  %s\n", via_incl_finalis);
    redde ZEPHYRUM;
}
