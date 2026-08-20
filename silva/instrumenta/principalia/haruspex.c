/* haruspex.c - Oraculum formarum (M0a Chunk D)
 *
 * "Haruspex exta inspicit et contra clang pronuntiat." Pro quaque
 * plagula corporis: parsura (fistula plena - praebenda + systema +
 * oraculum praeoneratum) -> semantica -> emissio TU assertionum
 * temporis compilationis:
 *
 *   #include <stddef.h>
 *   #include "<plagula originalis>"    (etiam .c - inclusio textualis!)
 *   typedef char hx_N[(sizeof(T) == M) ? 1 : -1];
 *   typedef struct { char c; T t; } hx_N_s;
 *   typedef char hx_N_o[(offsetof(hx_N_s, t) == A) ? 1 : -1];
 *   ... offsetof per membrum directum; valores enumerorum ...
 *
 * COMPILATIO EST IUDICIUM: haruspex.sh quamque TU per
 * clang -fsyntax-only compilat; exitus non-zephyrus = forma nostra
 * a clang dissentit. Typi in ipsa .c definiti VISIBILES sunt (TU
 * eadem). Partitio provenientiae: typi ex_systemate (et typi qui
 * per valorem ex eis pendent) PRAETERMITTUNTUR - de systemate solum
 * opacitas asseritur, numquam interna (consilium par VIII).
 *
 * Praetermissa (numerata, alta voce in -v): incompleta, campos
 * ferentia (parca), anonyma, functiones, systemate-pendentia.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_commissio.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 tu_emissae = ZEPHYRUM;
hic_manens i32 assertiones_summa = ZEPHYRUM;
hic_manens i32 praetermissa_systemate = ZEPHYRUM;
hic_manens i32 praetermissa_incompleta = ZEPHYRUM;
hic_manens i32 praetermissa_campis = ZEPHYRUM;
hic_manens i32 capita_praebita = ZEPHYRUM;

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

/* ==================================================
 * Lectio plagularum + praebenda (exemplar percursus)
 * ================================================== */

hic_manens character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* datum;

    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    datum = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l + 1L));
    si (datum == NIHIL
        || (mensura_l > 0L
            && fread(datum, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    datum[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde datum;
}

hic_manens b32
_est_caput (constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    redde m > II && titulus[m - II] == '.' && titulus[m - I] == 'h';
}

hic_manens b32
_est_fons_c (constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    redde m > II && titulus[m - II] == '.' && titulus[m - I] == 'c';
}

hic_manens vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* radix)
{
    DIR* dir = opendir(radix);
    structura dirent* introitus;
    character via[2048];

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        sprintf(via, "%s/%s", radix, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(ctx, piscina, visa, via);
        }
        alioquin si (_est_caput(introitus->d_name))
        {
            chorda clavis;
            unio { constans character* c; i8* m; } u;

            u.c = introitus->d_name;
            clavis.datum = u.m;
            clavis.mensura = (i32)strlen(introitus->d_name);
            si (tabula_dispersa_continet(visa, clavis))
            {
                perge;   /* primus vicit */
            }
            {
                chorda copia = chorda_transcribere(clavis, piscina);

                (vacuum)tabula_dispersa_inserere(visa, copia,
                    (vacuum*)VERUM);
            }
            {
                i32 mensura = ZEPHYRUM;
                character* textus = _plagulam_legere(piscina, via,
                    &mensura);

                si (textus != NIHIL)
                {
                    (vacuum)silva_contextus_praebere(ctx,
                        introitus->d_name, textus, mensura);
                    capita_praebita++;
                }
            }
        }
    }
    closedir(dir);
}

/* ==================================================
 * Assertabilitas
 * ================================================== */

/* Penditne typus PER VALOREM ex typo systematis? (monstratores
 * opaci licent - partitio provenientiae) */
hic_manens b32
_pendet_ex_systemate (constans TypusC89* typus, i32 profunditas)
{
    si (typus == NIHIL || profunditas > XXXII)
    {
        redde VERUM;   /* cautus */
    }
    commutatio (typus->genus)
    {
        casus TYPUS_C89_PRIMITIVUS:
        casus TYPUS_C89_MONSTRATOR:
            redde FALSUM;
        casus TYPUS_C89_QUALIFICATUS:
            redde _pendet_ex_systemate(
                typus->datum.qualificatus.internum, profunditas + I);
        casus TYPUS_C89_ACIES:
            redde _pendet_ex_systemate(typus->datum.acies.elementum,
                profunditas + I);
        casus TYPUS_C89_ENUMERATUS:
            redde typus->ex_systemate;
        casus TYPUS_C89_STRUCTURA:
        casus TYPUS_C89_UNIO:
        {
            i32 k;

            si (typus->ex_systemate)
            {
                redde VERUM;
            }
            per (k = ZEPHYRUM;
                 k < (i32)typus->datum.tag.numerus_membrorum; k++)
            {
                si (_pendet_ex_systemate(
                        typus->datum.tag.membra[k].typus,
                        profunditas + I))
                {
                    redde VERUM;
                }
            }
            redde FALSUM;
        }
        ordinarius:
            redde VERUM;   /* FUNCTIO/ERROR - non assertanda */
    }
}

hic_manens b32
_assertabilis (SilvaSemantica* sem, TypusC89* typus)
{
    si (typus == NIHIL || typus->genus == TYPUS_C89_ERROR
        || typus->genus == TYPUS_C89_FUNCTIO)
    {
        redde FALSUM;
    }
    si (_pendet_ex_systemate(typus, ZEPHYRUM))
    {
        praetermissa_systemate++;
        redde FALSUM;
    }
    {
        constans TypusC89* t = typus;

        dum (t->genus == TYPUS_C89_QUALIFICATUS)
        {
            t = t->datum.qualificatus.internum;
        }
        si ((t->genus == TYPUS_C89_STRUCTURA
             || t->genus == TYPUS_C89_UNIO)
            && t->datum.tag.habet_campos)
        {
            praetermissa_campis++;
            redde FALSUM;
        }
    }
    si (!silva_c89_formam_computare(sem, typus))
    {
        praetermissa_incompleta++;
        redde FALSUM;
    }
    redde VERUM;
}

/* ==================================================
 * Emissio assertionum
 * ================================================== */

/* Nomen typi in textum C ("Titulus" aut "struct Titulus") */
hic_manens vacuum
_nomen_typi_imprimere (FILE* pl, constans character* praefixum,
    chorda titulus)
{
    fprintf(pl, "%s%.*s", praefixum, (int)titulus.mensura,
        (constans character*)titulus.datum);
}

/* Assertiones pro typo uno (sizeof + ordinatio + offsetof
 * membrorum si tag) - numerus assertionum redditur */
hic_manens i32
_typum_asserere (FILE* pl, SilvaSemantica* sem, TypusC89* typus,
    constans character* praefixum, chorda titulus, i32* n)
{
    i32 emissae = ZEPHYRUM;
    constans TypusC89* nudus = typus;

    (vacuum)sem;
    dum (nudus->genus == TYPUS_C89_QUALIFICATUS)
    {
        nudus = nudus->datum.qualificatus.internum;
    }

    /* sizeof */
    fprintf(pl, "typedef char hx_%d_m[(sizeof(", (int)*n);
    _nomen_typi_imprimere(pl, praefixum, titulus);
    fprintf(pl, ") == %d) ? 1 : -1];\n",
        (int)typus->magnitudo_octetorum);
    emissae++;

    /* ordinatio (dolus structurae: offset post char unum) */
    fprintf(pl, "typedef struct { char hx_c; ");
    _nomen_typi_imprimere(pl, praefixum, titulus);
    fprintf(pl, " hx_t; } hx_%d_s;\n", (int)*n);
    fprintf(pl,
        "typedef char hx_%d_o[(offsetof(hx_%d_s, hx_t) == %d)"
        " ? 1 : -1];\n",
        (int)*n, (int)*n, (int)typus->ordinatio);
    emissae++;

    /* offsetof membrorum directorum (tags completi) */
    si (nudus->genus == TYPUS_C89_STRUCTURA
        || nudus->genus == TYPUS_C89_UNIO)
    {
        i32 k;

        per (k = ZEPHYRUM;
             k < (i32)nudus->datum.tag.numerus_membrorum; k++)
        {
            TypusC89Membrum* membrum = &nudus->datum.tag.membra[k];

            si (membrum->titulus.mensura == ZEPHYRUM)
            {
                perge;
            }
            fprintf(pl, "typedef char hx_%d_%d[(offsetof(",
                (int)*n, (int)k);
            _nomen_typi_imprimere(pl, praefixum, titulus);
            fprintf(pl, ", %.*s) == %d) ? 1 : -1];\n",
                (int)membrum->titulus.mensura,
                (constans character*)membrum->titulus.datum,
                (int)membrum->offset);
            emissae++;
        }
    }
    (*n)++;
    redde emissae;
}

/* ==================================================
 * Plagula una
 * ================================================== */

/* Estne plagula LEXICON silvae (systema_*.h)? Basis nominis
 * inspicitur, non via tota - lexicon ubicumque situm idem manet.
 * CLASSIS, non nomen: lexicon quodlibet futurum se sponte excludet. */
hic_manens b32
_lexicon_est (constans character* via)
{
    constans character* basis;
    constans character* solidus;

    basis   = via;
    solidus = strrchr(via, '/');
    si (solidus != NIHIL)
    {
        basis = solidus + I;
    }
    redde (strncmp(basis, "systema_", magnitudo("systema_") - I) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_plagulam_inspicere (constans SilvaContextus* ctx,
    constans character* via)
{
    Piscina* piscina = piscina_generare_dynamicum("haruspex",
        8388608);
    i32 mensura = ZEPHYRUM;
    character* fons;
    SilvaOraculum* oraculum;
    SilvaParsura* parsura;
    SilvaSemantica* sem;
    character via_tu[2048];
    FILE* pl = NIHIL;
    i32 n = ZEPHYRUM;
    i32 assertiones = ZEPHYRUM;
    i32 i;

    si (piscina == NIHIL)
    {
        redde;
    }
    /* LEXICA (systema_*.h) numquam a clang compilantur: typi
     * synthetici consulto sunt - partitio provenientiae - et
     * plagulae ipsae stare solae nequeunt (systema_posix.h 'time_t'
     * nominat quem nullum caput vero praebet). Haruspex eas praeterit.
     *
     * Prius 'systema_c89.h' SOLUM nominabatur. Cum systema_posix.h
     * lexicon SECUNDUM factum est (28502f2), nemo condicionem
     * extendit: haruspex ex illo die ruber stetit, et nemo vidit
     * quia nemo eum currebat. Nunc CLASSIS excluditur, non nomen -
     * lexicon tertium se sponte excludet. */
    si (_lexicon_est(via))
    {
        piscina_destruere(piscina);
        redde;
    }
    fons = _plagulam_legere(piscina, via, &mensura);
    si (fons == NIHIL)
    {
        piscina_destruere(piscina);
        redde;
    }
    plagulae++;

    oraculum = silva_oraculum_creare(piscina);
    si (oraculum != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        fons, mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("[SINE ARBORE] %s\n", via);
        piscina_destruere(piscina);
        redde;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(piscina,
        parsura, systema_parsura);
    si (sem == NIHIL)
    {
        imprimere("[SINE SEMANTICA] %s\n", via);
        piscina_destruere(piscina);
        redde;
    }

    /* TU aperire pigre (solum si assertiones adsunt) */
    {
        constans character* basis = strrchr(via, '/');
        character nudum[512];
        memoriae_index m;

        basis = (basis != NIHIL) ? basis + I : via;
        m = strlen(basis);
        si (m > II && basis[m - II] == '.')
        {
            m = m - II;   /* extensionem .c/.h demere */
        }
        si (m > (memoriae_index)(magnitudo(nudum) - I))
        {
            m = magnitudo(nudum) - I;
        }
        memcpy(nudum, basis, m);
        nudum[m] = '\0';
        sprintf(via_tu, "silva/build/exta/hx_%04d_%s.c",
            (int)tu_emissae, nudum);
    }

    /* typedefs plagulae IPSIUS (fons_princeps) */
    per (i = ZEPHYRUM; i < silva_c89_symbola_numerus(sem); i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, i);

        si (symbolum->ex_systemate
            || symbolum->profunditas != ZEPHYRUM
            || symbolum->lexema == NIHIL
            || symbolum->lexema->fons_index
                != parsura->fons_princeps)
        {
            perge;
        }
        si (symbolum->genus == (s32)SYMBOLUM_TYPEDEF)
        {
            si (!_assertabilis(sem, symbolum->typus))
            {
                perge;
            }
            si (pl == NIHIL)
            {
                pl = fopen(via_tu, "wb");
                si (pl == NIHIL)
                {
                    frange;
                }
                fprintf(pl, "/* GENERATUM - haruspex (%s) */\n"
                    "#include <stddef.h>\n#include \"%s\"\n\n",
                    via, via);
            }
            assertiones += _typum_asserere(pl, sem, symbolum->typus,
                "", symbolum->titulus, &n);
        }
        alioquin si (symbolum->genus == (s32)SYMBOLUM_CONSTANS)
        {
            si (pl == NIHIL)
            {
                pl = fopen(via_tu, "wb");
                si (pl == NIHIL)
                {
                    frange;
                }
                fprintf(pl, "/* GENERATUM - haruspex (%s) */\n"
                    "#include <stddef.h>\n#include \"%s\"\n\n",
                    via, via);
            }
            fprintf(pl, "typedef char hx_%d_v[(%.*s == %ldL)"
                " ? 1 : -1];\n", (int)n,
                (int)symbolum->titulus.mensura,
                (constans character*)symbolum->titulus.datum,
                (long)symbolum->valor);
            n++;
            assertiones++;
        }
    }

    /* tags nominati plagulae ipsius */
    {
        TabulaIterator iter = tabula_dispersa_iterator_initium(
            sem->scopus_summus->tags);
        chorda clavis;
        vacuum* valor;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
                &valor))
        {
            TypusC89* typus = (TypusC89*)valor;
            constans character* praefixum;
            chorda titulus;
            s32 fons_tag = -I;

            si (typus->ex_systemate)
            {
                perge;
            }
            si (typus->genus == TYPUS_C89_STRUCTURA)
            {
                praefixum = "struct ";
                titulus = typus->datum.tag.titulus;
            }
            alioquin si (typus->genus == TYPUS_C89_UNIO)
            {
                praefixum = "union ";
                titulus = typus->datum.tag.titulus;
            }
            alioquin si (typus->genus == TYPUS_C89_ENUMERATUS)
            {
                praefixum = "enum ";
                titulus = typus->datum.enumeratus.titulus;
            }
            alioquin
            {
                perge;
            }
            si (titulus.mensura == ZEPHYRUM)
            {
                perge;
            }
            /* fons tag per lexema verbi (declarans nodus) */
            {
                constans SilvaNodus* declarans =
                    (typus->genus == TYPUS_C89_ENUMERATUS)
                        ? typus->datum.enumeratus.declarans
                        : typus->datum.tag.declarans;

                si (declarans != NIHIL)
                {
                    SilvaValor verbum =
                        (typus->genus == TYPUS_C89_ENUMERATUS)
                        ? silva_c89_enumeratio_tok_verbum(declarans)
                        : ((typus->genus == TYPUS_C89_UNIO)
                            ? silva_c89_unio_tok_verbum(declarans)
                            : silva_c89_structura_tok_verbum(
                                  declarans));

                    si (verbum.genus == SILVA_VALOR_TOKEN)
                    {
                        fons_tag = verbum.datum.token->fons_index;
                    }
                }
            }
            si (fons_tag != parsura->fons_princeps)
            {
                perge;
            }
            si (!_assertabilis(sem, typus))
            {
                perge;
            }
            si (pl == NIHIL)
            {
                pl = fopen(via_tu, "wb");
                si (pl == NIHIL)
                {
                    frange;
                }
                fprintf(pl, "/* GENERATUM - haruspex (%s) */\n"
                    "#include <stddef.h>\n#include \"%s\"\n\n",
                    via, via);
            }
            assertiones += _typum_asserere(pl, sem, typus,
                praefixum, titulus, &n);
        }
    }

    si (pl != NIHIL)
    {
        fclose(pl);
        tu_emissae++;
        assertiones_summa += assertiones;
        si (verbosa)
        {
            imprimere("[hx %d assertiones] %s\n", (int)assertiones,
                via);
        }
    }
    piscina_destruere(piscina);
}

hic_manens vacuum
_directorium_inspicere (constans SilvaContextus* ctx,
    constans character* radix)
{
    DIR* dir = opendir(radix);
    structura dirent* introitus;
    character via[2048];

    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        sprintf(via, "%s/%s", radix, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _directorium_inspicere(ctx, via);
        }
        alioquin si (_est_fons_c(introitus->d_name)
            || _est_caput(introitus->d_name))
        {
            _plagulam_inspicere(ctx, via);
        }
    }
    closedir(dir);
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    SilvaContextus* ctx;
    integer k;

    piscina_ctx = piscina_generare_dynamicum("haruspex_ctx",
        16777216);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "haruspex: piscina deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "haruspex: contextus deest\n");
        redde I;
    }
    ctx->fines.lexemata = ZEPHYRUM;

    /* systema (fistula plena) */
    {
        i32 mensura = ZEPHYRUM;
        character* fons = _plagulam_legere(piscina_ctx,
            "silva/fontes/systema_c89.h", &mensura);

        si (fons == NIHIL)
        {
            fprintf(stderr, "haruspex: systema deest (curre ex"
                " radice repositorii)\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_ctx,
            "systema_c89.h", fons, mensura, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "haruspex: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "haruspex: systema non analysatum\n");
            redde I;
        }
    }

    /* praebenda ex radice (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            redde I;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    imprimere("=== HARUSPEX (formae contra clang) ===\n");
    imprimere("capita: %d praebita\n", (int)capita_praebita);

    {
        b32 ulla = FALSUM;

        per (k = I; k < argc; k++)
        {
            si (strcmp(argv[k], "-v") == ZEPHYRUM)
            {
                verbosa = VERUM;
                perge;
            }
            ulla = VERUM;
            _directorium_inspicere(ctx, argv[k]);
        }
        si (!ulla)
        {
            _directorium_inspicere(ctx, "include");
            _directorium_inspicere(ctx, "lib");
            _directorium_inspicere(ctx, "silva/fontes");
            _directorium_inspicere(ctx, "tessera/fontes");
            _directorium_inspicere(ctx, "saltuarius/fontes");
        }
    }

    imprimere("plagulae: %d; TU emissae: %d; assertiones: %d\n",
        (int)plagulae, (int)tu_emissae, (int)assertiones_summa);
    imprimere("praetermissa: %d systemate, %d incompleta/functio,"
        " %d campis\n",
        (int)praetermissa_systemate, (int)praetermissa_incompleta,
        (int)praetermissa_campis);
    redde ZEPHYRUM;
}
