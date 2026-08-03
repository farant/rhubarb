/* interpretare.c - INTERPRETARE (instrumenta prima #5): functionem
 * corporis cum argumentis datis in machinula currere - "ansa
 * hypothesis celeris" (venatio sputnik ~10 min -> vocatio una).
 *
 * Forma (sim-1 C3): generator MODULI INVOLUCRI - textus .medulla
 * parvus generatur (exemplar modulorum inline probationis
 * machinulae): data pro literis chordarum, functio $main quae
 * scopum vocat et fructum per printf imprimit (typo reditus
 * conscio). Tum: demissio TUs scopo datorum -> conexio -> machinula.
 * Involucrum TEXTUS est - inspectabile (-textus), reproducibile.
 *
 * Scopum (sim-1 C4): -plagulae a,b,c = TUs sub lib/ quorum titulus
 * substring continet (initium frigidum secundis, non minutis);
 * sine -plagulae = mundus totus lib/ (monitum honestum).
 *
 * v0 (DECISUS #5): scalaria + literae chordarum; aggregata =
 * dilatio nominata. Argumenta per typum PARAMETRI evaluantur
 * (machinula: signatura auctoritas ad limitem - lectio M2c);
 * literae fluitantes punctum accipiunt si param f32/f64.
 * Relatio halitus gratis in exitu non-BENE (recordator volatus
 * machinulae semper activus).
 *
 * Usus: ./officina/interpretare.sh [-plagulae a,b,c] [-textus]
 *           [-mora n] <functio> [argumenta...]
 * Exitus: 0 = BENE | 1 = halitus/functio absens | 2 = usus/mundus
 */

#include "postulata_posix.h"
#include "officina_demissio.h"
#include "officina_conexio.h"
#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define PLAGULAE_MAXIMAE XXXII
#define ARGUMENTA_MAXIMA VIII
#define INVOLUCRUM_MENSURA 8192

hic_manens i32 mensura_maxima = 4194304;

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;
hic_manens Piscina* piscina_officinae = NIHIL;
hic_manens Xar* moduli_mundi = NIHIL;
hic_manens Xar* lineae_mundi = NIHIL;    /* MedullaLineae*
                                          * parallelae modulis */

hic_manens constans character* plagulae_quaesitae[PLAGULAE_MAXIMAE];
hic_manens i32 numerus_quaesitarum = ZEPHYRUM;

/* ==================================================
 * demissio unius plagulae (cursor exscriptum, sine censu)
 * ================================================== */

hic_manens character*
_systema_legere (Piscina* piscina, constans character* via,
    long* mensura_fructus)
{
    FILE* pl = fopen(via, "rb");
    long mensura;
    character* fons;

    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (fons == NIHIL
        || fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_fructus = mensura;
    redde fons;
}

hic_manens MedullaModulus*
_plagulam_demittere (constans SilvaContextus* ctx,
    constans character* via, MedullaLineae** lineae_exitus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    SilvaPiscina* piscina_arboris;
    i8* fons;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    chorda titulus_moduli;
    unio { constans character* c; i8* m; } u;

    si (lineae_exitus != NIHIL)
    {
        *lineae_exitus = NIHIL;
    }
    pl = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > (i32)mensura_maxima)
    {
        fprintf(stderr, "[praetermissa mensura] %s\n", via);
        fclose(pl);
        redde NIHIL;
    }

    piscina_arboris = silva_piscina_generare_dynamicum(
        "interpretare_arbor", 8388608);
    si (piscina_arboris == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    fons = (i8*)piscina_allocare(piscina_officinae,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    oraculum = silva_oraculum_creare(piscina_arboris);
    si (oraculum != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    u.m = fons;
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris, ctx,
        via, u.c, (unsigned int)mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr, "[parsura fracta] %s\n", via);
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(
        piscina_arboris, parsura, systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(
            piscina_arboris, parsura, systema_parsura);
    }
    si (sem == NIHIL)
    {
        fprintf(stderr, "[semantica fracta] %s\n", via);
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }

    u.c = via;
    titulus_moduli.datum = u.m;
    titulus_moduli.mensura = (i32)strlen(via);
    modulus = demissio_currere(piscina_officinae, parsura, sem,
        titulus_moduli);
    si (modulus == NIHIL)
    {
        fprintf(stderr, "[demissio ruit] %s\n", via);
    }
    alioquin si (lineae_exitus != NIHIL)
    {
        /* collige-ante-destrue (M3): catena originis solum dum
         * parsura vivit ambulabilis est; lineae in piscinam
         * officinae (vita moduli) */
        *lineae_exitus = demissio_lineas_colligere(
            piscina_officinae, modulus, parsura);
    }
    silva_piscina_destruere(piscina_arboris);
    redde modulus;
}

/* ==================================================
 * capita praeparare (exemplar cursor)
 * ================================================== */

hic_manens b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis)) redde;
    pl = fopen(via, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde;
    }
    fclose(pl);
    si (silva_contextus_praebere(ctx, titulus, textus,
            (unsigned int)mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

hic_manens vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(ctx, piscina, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_praebere(ctx, piscina, visa, via_plena,
                    introitus->d_name);
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * mundus scopo datus: plagulae .c sub lib/ filtro congruentes
 * ================================================== */

hic_manens integer
_comparare_vias (constans vacuum* a, constans vacuum* b)
{
    redde strcmp(*(character* constans*)a,
        *(character* constans*)b);
}

hic_manens b32
_plagula_congruit (constans character* titulus)
{
    i32 k;

    si (numerus_quaesitarum == ZEPHYRUM) redde VERUM;
    per (k = ZEPHYRUM; k < numerus_quaesitarum; k++)
    {
        si (strstr(titulus, plagulae_quaesitae[k]) != NIHIL)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

hic_manens vacuum
_bibliothecam_demittere (constans SilvaContextus* ctx)
{
    DIR* dir = opendir("lib");
    structura dirent* introitus;
    character* viae[512];
    integer numerus = 0;
    integer i;

    si (dir == NIHIL)
    {
        fprintf(stderr, "interpretare: lib/ deest (curre ex"
            " radice)\n");
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL && numerus < 512)
    {
        memoriae_index m = strlen(introitus->d_name);

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (m >= III && introitus->d_name[m - II] == '.'
            && introitus->d_name[m - I] == 'c'
            && _plagula_congruit(introitus->d_name))
        {
            character* via = piscina_allocare(piscina_officinae,
                m + V);

            sprintf(via, "lib/%s", introitus->d_name);
            viae[numerus] = via;
            numerus++;
        }
    }
    closedir(dir);
    qsort(viae, (memoriae_index)numerus, magnitudo(character*),
        _comparare_vias);
    per (i = 0; i < numerus; i++)
    {
        MedullaLineae* lineae = NIHIL;
        MedullaModulus* modulus = _plagulam_demittere(ctx, viae[i],
            &lineae);

        si (modulus != NIHIL)
        {
            MedullaModulus** locellus = xar_addere(moduli_mundi);
            MedullaLineae** locellus_l = xar_addere(lineae_mundi);

            *locellus = modulus;
            *locellus_l = lineae;
        }
    }
}

/* ==================================================
 * scopum invenire + involucrum generare
 * ================================================== */

hic_manens constans MedullaFunctio*
_functionem_invenire (constans character* titulus)
{
    i32 m;

    per (m = ZEPHYRUM; m < xar_numerus(moduli_mundi); m++)
    {
        MedullaModulus* modulus =
            *(MedullaModulus**)xar_obtinere(moduli_mundi, m);
        i32 f;

        per (f = ZEPHYRUM; f < xar_numerus(modulus->functiones);
            f++)
        {
            MedullaFunctio* functio =
                *(MedullaFunctio**)xar_obtinere(modulus->functiones,
                    f);

            si (chorda_aequalis_literis(functio->titulus, titulus))
            {
                redde functio;
            }
        }
    }
    redde NIHIL;
}

hic_manens b32
_typus_fluitans (s32 typus)
{
    redde (typus == (s32)MEDULLA_TYPUS_F32
        || typus == (s32)MEDULLA_TYPUS_F64) ? VERUM : FALSUM;
}

/* datum chordae: octeti hex in quaternionibus (exemplar goldens) */
hic_manens i32
_datum_chordae_scribere (character* involucrum, i32 cursor,
    i32 index, constans character* littera)
{
    i32 mensura_veri = (i32)strlen(littera) + I;   /* + NUL */
    i32 mensura_datis = ((mensura_veri + III) / IV) * IV;
    i32 k;

    cursor += (i32)sprintf(involucrum + cursor,
        "datum $arg_%u magnitudo %u ordinatio 1\n    octeti ",
        index, mensura_datis);
    per (k = ZEPHYRUM; k < mensura_datis; k++)
    {
        insignatus integer octetus = (k < mensura_veri - I)
            ? (insignatus integer)(insignatus character)littera[k]
            : 0U;

        si (k > ZEPHYRUM && k % IV == ZEPHYRUM)
        {
            cursor += (i32)sprintf(involucrum + cursor, " ");
        }
        cursor += (i32)sprintf(involucrum + cursor, "%02x",
            octetus);
    }
    cursor += (i32)sprintf(involucrum + cursor, "\n");
    redde cursor;
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    SilvaPiscina* piscina_silvae_ctx;
    SilvaContextus* ctx;
    constans character* titulus_functionis = NIHIL;
    constans character* argumenta[ARGUMENTA_MAXIMA];
    i32 numerus_argumentorum = ZEPHYRUM;
    b32 textus_petitus = FALSUM;
    insignatus integer mora_secunda = 60;
    integer a;

    per (a = I; a < argc; a++)
    {
        si (strcmp(argv[a], "-plagulae") == ZEPHYRUM
            && a + I < argc)
        {
            character* fragmentum;

            a++;
            fragmentum = strtok(argv[a], ",");
            dum (fragmentum != NIHIL
                && numerus_quaesitarum < PLAGULAE_MAXIMAE)
            {
                plagulae_quaesitae[numerus_quaesitarum] =
                    fragmentum;
                numerus_quaesitarum++;
                fragmentum = strtok(NIHIL, ",");
            }
        }
        alioquin si (strcmp(argv[a], "-textus") == ZEPHYRUM)
        {
            textus_petitus = VERUM;
        }
        alioquin si (strcmp(argv[a], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (strcmp(argv[a], "-mora") == ZEPHYRUM
            && a + I < argc)
        {
            a++;
            mora_secunda = (insignatus integer)atoi(argv[a]);
        }
        alioquin si (titulus_functionis == NIHIL)
        {
            titulus_functionis = argv[a];
        }
        alioquin si (numerus_argumentorum < ARGUMENTA_MAXIMA)
        {
            argumenta[numerus_argumentorum] = argv[a];
            numerus_argumentorum++;
        }
    }
    si (titulus_functionis == NIHIL)
    {
        fprintf(stderr, "usus: interpretare [-plagulae a,b,c]"
            " [-textus] [-mora n] <functio> [argumenta...]\n"
            "      (ex radice repositorii; scalaria + literae"
            " chordarum)\n");
        redde II;
    }

    piscina_ctx = piscina_generare_dynamicum("interpretare_ctx",
        8388608);
    piscina_silvae_ctx = silva_piscina_generare_dynamicum(
        "interpretare_silva", 8388608);
    piscina_officinae = piscina_generare_dynamicum(
        "interpretare_off", 16777216);
    si (piscina_ctx == NIHIL || piscina_silvae_ctx == NIHIL
        || piscina_officinae == NIHIL)
    {
        fprintf(stderr, "interpretare: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina_silvae_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "interpretare: contextus deest\n");
        redde II;
    }
    moduli_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaModulus*));
    lineae_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaLineae*));

    /* systema concatenatum ISO+POSIX (exemplar cursor) */
    {
        long m_iso = 0L;
        long m_posix = 0L;
        character* fons_iso = _systema_legere(piscina_ctx,
            "silva/fontes/systema_c89.h", &m_iso);
        character* fons_posix = _systema_legere(piscina_ctx,
            "silva/fontes/systema_posix.h", &m_posix);
        character* fons_sys;
        long mensura_sys;

        si (fons_iso == NIHIL || fons_posix == NIHIL)
        {
            fprintf(stderr, "interpretare: systema deest (curre"
                " ex radice repositorii)\n");
            redde II;
        }
        mensura_sys = m_iso + 1L + m_posix;
        fons_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(mensura_sys + 1L));
        si (fons_sys == NIHIL) redde II;
        memcpy(fons_sys, fons_iso, (memoriae_index)m_iso);
        fons_sys[m_iso] = '\n';
        memcpy(fons_sys + m_iso + 1L, fons_posix,
            (memoriae_index)m_posix);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (unsigned int)mensura_sys))
        {
            redde II;
        }
        systema_parsura = silva_c89_parsare(piscina_silvae_ctx,
            "systema_c89.h", fons_sys, (unsigned int)mensura_sys,
            NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "interpretare: systema non"
                " parsatum\n");
            redde II;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_silvae_ctx, systema_parsura);
        si (systema_semantica == NIHIL) redde II;
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL) redde II;
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    si (numerus_quaesitarum == ZEPHYRUM)
    {
        fprintf(stderr, "interpretare: sine -plagulae = mundus"
            " totus lib/ (~30s)...\n");
    }
    _bibliothecam_demittere(ctx);
    fprintf(stderr, "mundus: %d moduli\n",
        (int)xar_numerus(moduli_mundi));

    /* scopum invenire (signatura = auctoritas) */
    {
        constans MedullaFunctio* scopum =
            _functionem_invenire(titulus_functionis);
        character* involucrum;
        i32 cursor = ZEPHYRUM;
        i32 k;
        constans character* forma;

        si (scopum == NIHIL)
        {
            fprintf(stderr, "interpretare: functio non inventa:"
                " %s\n", titulus_functionis);
            si (numerus_quaesitarum > ZEPHYRUM)
            {
                fprintf(stderr, "  (in plagulis scopo datis solum"
                    " quaesita - amplia -plagulae;"
                    " ./silva/nexus.sh %s plagulam eius"
                    " invenit)\n", titulus_functionis);
            }
            redde I;
        }
        /* fructus aggregatus = parametrum primum syntheticum
         * %reditus (conventio M1 C7) - v0 scalaria solum (DECISUS
         * #5; sine hac custodia: scriptura fera per inscriptionem
         * purgamenti -> VITIUM - inventum die natali in
         * fasti_computus -> Dies) */
        si (xar_numerus(scopum->parametra) > ZEPHYRUM)
        {
            constans MedullaParametrum* primus =
                (constans MedullaParametrum*)xar_obtinere(
                    scopum->parametra, ZEPHYRUM);
            chorda* titulus_registri = (chorda*)xar_obtinere(
                scopum->registra, (i32)primus->index);

            si (titulus_registri != NIHIL
                && chorda_aequalis_literis(*titulus_registri,
                       "reditus"))
            {
                fprintf(stderr, "interpretare: fructus aggregatus"
                    " (%s conventionem %%reditus habet) - v0"
                    " scalaria solum, dilatio nominata\n",
                    titulus_functionis);
                redde I;
            }
        }
        si (xar_numerus(scopum->parametra)
            != numerus_argumentorum && !scopum->est_variadica)
        {
            fprintf(stderr, "CAUTIO: %d argumenta data,"
                " %d parametra exspectata\n",
                (int)numerus_argumentorum,
                (int)xar_numerus(scopum->parametra));
        }

        involucrum = (character*)piscina_allocare(piscina_ctx,
            INVOLUCRUM_MENSURA);
        si (involucrum == NIHIL) redde II;

        cursor += (i32)sprintf(involucrum + cursor,
            "modulus \"interpretare.medulla\"\n\nfunctio $main ()"
            " -> s32\n@initium:\n");

        /* literae chordarum -> loci */
        per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
        {
            constans MedullaParametrum* parametrum =
                (k < xar_numerus(scopum->parametra))
                    ? (constans MedullaParametrum*)xar_obtinere(
                          scopum->parametra, k)
                    : NIHIL;
            b32 est_chorda = FALSUM;

            /* littera chordae = non-numerus (nec '-' nec digitus
             * primus) AUT parametrum monstratoris cum littera
             * textuali */
            {
                character primus = argumenta[k][ZEPHYRUM];

                si (!((primus >= '0' && primus <= '9')
                    || primus == '-' || primus == '+'
                    || primus == '.'))
                {
                    est_chorda = VERUM;
                }
            }
            si (est_chorda)
            {
                cursor += (i32)sprintf(involucrum + cursor,
                    "    %%a%u = locus $arg_%u\n", k, k);
            }
            (vacuum)parametrum;
        }

        /* vocatio */
        si (scopum->typus_reditus == MEDULLA_TYPUS_NIHIL)
        {
            cursor += (i32)sprintf(involucrum + cursor,
                "    vocare $%s", titulus_functionis);
        }
        alioquin
        {
            cursor += (i32)sprintf(involucrum + cursor,
                "    %%fr = vocare.%s $%s",
                medulla_typum_titulus(scopum->typus_reditus),
                titulus_functionis);
        }
        per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
        {
            character primus = argumenta[k][ZEPHYRUM];
            b32 est_chorda = !((primus >= '0' && primus <= '9')
                || primus == '-' || primus == '+' || primus == '.');
            constans MedullaParametrum* parametrum =
                (k < xar_numerus(scopum->parametra))
                    ? (constans MedullaParametrum*)xar_obtinere(
                          scopum->parametra, k)
                    : NIHIL;

            si (est_chorda)
            {
                cursor += (i32)sprintf(involucrum + cursor,
                    ", %%a%u", k);
            }
            alioquin si (parametrum != NIHIL
                && _typus_fluitans(parametrum->typus)
                && strchr(argumenta[k], '.') == NIHIL)
            {
                /* littera fluitans sine puncto: punctum addere
                 * (lector formam fluitantem postulat) */
                cursor += (i32)sprintf(involucrum + cursor,
                    ", %s.0", argumenta[k]);
            }
            alioquin
            {
                cursor += (i32)sprintf(involucrum + cursor,
                    ", %s", argumenta[k]);
            }
        }
        cursor += (i32)sprintf(involucrum + cursor, "\n");

        /* impressio fructus per typum reditus */
        si (scopum->typus_reditus == MEDULLA_TYPUS_NIHIL)
        {
            forma = "(vacuum)\n";
            cursor += (i32)sprintf(involucrum + cursor,
                "    vocare $printf, $forma\n");
        }
        alioquin si (scopum->typus_reditus
            == (s32)MEDULLA_TYPUS_F32)
        {
            forma = "%g\n";
            /* dilatare, NON extendere: vocabularium f32->f64
             * (extendere = latitudo integralis; lector clemens
             * extendere.f32.f64 accepit et bits crudos servavit -
             * denormale 5e-315 = inventum die natali) */
            cursor += (i32)sprintf(involucrum + cursor,
                "    %%frd = dilatare.f32.f64 %%fr\n"
                "    vocare $printf, $forma, %%frd\n");
        }
        alioquin si (scopum->typus_reditus
            == (s32)MEDULLA_TYPUS_F64)
        {
            forma = "%g\n";
            cursor += (i32)sprintf(involucrum + cursor,
                "    vocare $printf, $forma, %%fr\n");
        }
        alioquin si (scopum->typus_reditus >= (s32)MEDULLA_TYPUS_S8
            && scopum->typus_reditus <= (s32)MEDULLA_TYPUS_S64)
        {
            forma = "%lld\n";
            cursor += (i32)sprintf(involucrum + cursor,
                "    vocare $printf, $forma, %%fr\n");
        }
        alioquin
        {
            forma = "%llu\n";
            cursor += (i32)sprintf(involucrum + cursor,
                "    vocare $printf, $forma, %%fr\n");
        }
        cursor += (i32)sprintf(involucrum + cursor,
            "    redde 0\n\n");

        /* datum formae (octeti cum NUL) */
        cursor = _datum_chordae_scribere(involucrum, cursor,
            999, forma);
        /* titulus $arg_999 -> $forma: substitutio simplex infra */
        {
            character* sedes = strstr(involucrum, "$arg_999");

            dum (sedes != NIHIL)
            {
                memcpy(sedes, "$forma  ", VIII);
                sedes = strstr(sedes, "$arg_999");
            }
        }

        /* data argumentorum chordarum */
        per (k = ZEPHYRUM; k < numerus_argumentorum; k++)
        {
            character primus = argumenta[k][ZEPHYRUM];
            b32 est_chorda = !((primus >= '0' && primus <= '9')
                || primus == '-' || primus == '+' || primus == '.');

            si (est_chorda)
            {
                cursor = _datum_chordae_scribere(involucrum,
                    cursor, (i32)k, argumenta[k]);
            }
        }

        si (textus_petitus)
        {
            fprintf(stderr, "--- involucrum ---\n%s"
                "--- finis ---\n", involucrum);
        }

        /* lector -> conexio -> machinula */
        {
            chorda textus_ch;
            i32 linea_erroris = ZEPHYRUM;
            MedullaModulus* involucrum_modulus;
            Regio* regio;
            Conexio* conexio;
            Machinula* machinula;
            MachinulaExitus fructus;
            unio { constans character* c; i8* m; } u2;

            u2.c = involucrum;
            textus_ch.datum = u2.m;
            textus_ch.mensura = cursor;
            involucrum_modulus = medulla_textum_legere(piscina_ctx,
                textus_ch, &linea_erroris);
            si (involucrum_modulus == NIHIL)
            {
                fprintf(stderr, "interpretare: involucrum non"
                    " lectum (linea %d) - VITIUM GENERATORIS:\n%s",
                    (int)linea_erroris, involucrum);
                redde II;
            }

            regio = regio_generare(piscina_ctx);
            conexio = conexio_creare(piscina_ctx, regio);
            per (k = ZEPHYRUM; k < xar_numerus(moduli_mundi); k++)
            {
                si (!conexio_modulum_addere(conexio,
                        *(MedullaModulus**)xar_obtinere(
                            moduli_mundi, k)))
                {
                    fprintf(stderr, "interpretare: modulus non"
                        " additus\n");
                    redde II;
                }
            }
            si (!conexio_modulum_addere(conexio,
                    involucrum_modulus)
                || !conexio_nectere(conexio))
            {
                constans chorda* symbolum =
                    conexio_querela_symbolum(conexio);

                fprintf(stderr, "interpretare: nexus fractus"
                    " (%.*s) - amplia -plagulae?\n",
                    (int)symbolum->mensura,
                    (constans character*)symbolum->datum);
                redde II;
            }
            machinula = machinula_creare(piscina_ctx, conexio,
                regio);
            si (machinula == NIHIL)
            {
                fprintf(stderr, "interpretare: machinula deest\n");
                redde II;
            }
            /* lineae semper (M3 DECISUS 9) - involucrum sine
             * lineis (textuale, origo NIHIL) */
            per (k = ZEPHYRUM; k < xar_numerus(lineae_mundi); k++)
            {
                MedullaLineae* lineae_k = *(MedullaLineae**)
                    xar_obtinere(lineae_mundi, k);

                si (lineae_k != NIHIL)
                {
                    machinula_lineas_praebere(machinula, (s32)k,
                        lineae_k);
                }
            }
            si (mora_secunda > 0U)
            {
                (vacuum)alarm(mora_secunda);
            }
            {
                chorda titulus_main;
                unio { constans character* c; i8* m; } um;

                um.c = "main";
                titulus_main.datum = um.m;
                titulus_main.mensura = IV;
                fructus = machinula_currere(machinula,
                    titulus_main);
            }
            (vacuum)alarm(0U);
            fflush(stdout);

            si (fructus.genus == MACHINULA_BENE)
            {
                fprintf(stderr, "[BENE  %llu instructiones]\n",
                    (insignatus longus longus)
                        machinula_numerus_instructionum(machinula));
                redde ZEPHYRUM;
            }
            {
                constans character* genera[] = { "BENE", "SISTERE",
                    "DECIPULA", "VITIUM", "PAUSA", "RECUSATIO" };

                fprintf(stderr, "[%s: %.*s]\n",
                    genera[fructus.genus],
                    (int)fructus.nuntius.mensura,
                    (constans character*)fructus.nuntius.datum);
            }
            redde I;
        }
    }
}
