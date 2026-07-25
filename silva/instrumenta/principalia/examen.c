/* examen.c - effusor iudicii unius plagulae (M4a chunk D)
 *
 * Recipe percursus FIDELE: contextus + capita repositorii praebita
 * (inclusio vera) + lexicon systematis (canalis macrorum) + oraculum
 * praeseminatum + BIS-ANALYSIS post recanonicare (sem2
 * auctoritativum - typatio relativa canonicae).
 *
 * Usus:  examen <plagula.c> [-posix|-nudum] [-machina] [-v]
 * Exitus: 0 ACCIPE | 1 REICE | 2 INFRA
 *
 * Verdictum: REICE = violatio non-provisionalis ULLA aut nodi
 * erroris (syntaxis fracta = reiectio C89); INFRA = apparatus
 * solum (plagula illegibilis, systema deest).
 *
 * Geminus tertius oneratorum (percursus + vindex_onerator + hic) -
 * migratio ad fontem communem = parca nominata, tactu proximo.
 */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "vigilia.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens b32 machina = FALSUM;
hic_manens b32 cum_posix = FALSUM;
hic_manens b32 nudum = FALSUM;

hic_manens i32 capita_praebita = ZEPHYRUM;

interior constans character*
_severitatis_titulus (s32 severitas)
{
    commutatio (severitas)
    {
        casus (s32)EXAMEN_VIOLATIO:   redde "violatio";
        casus (s32)EXAMEN_SUSPECTUM:  redde "suspectum";
        casus (s32)EXAMEN_DOMESTICUM: redde "domesticum";
        ordinarius:                   redde "infra";
    }
}

interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
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
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura_l;
    redde textus;
}

interior b32
_praetermittendum (constans character* titulus)
{
    redde strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    chorda clavis;
    character* textus;
    i32 mensura;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;   /* collisio basename - primus vicit */
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde;
    }
    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
        capita_praebita++;
    }
}

interior vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via)
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
 * DERIVATIO EX INCLUSIONIBUS (varians (a), parcum 01KYB3NNR4)
 *
 * Plagula dependentias suas IAM declarat: '#include <sys/select.h>'
 * EST dictum "superficie select utor". Ergo tabula routing nulla
 * opus est - informatio in plagula ipsa habitat, ab auctore eo
 * momento scripta quo necessitatem creat, et cum plagula movetur
 * aut renominatur eam SEQUITUR.
 *
 * ISO SEMPER INTEGRUM MANET (decretum 01KYDMTS5N): derivatio POSIX
 * SOLUM tangit. Derivatio ISO diagnostica VERA quoque pareret
 * ('strlen adhibes sed <string.h> non includis') sed motum
 * baselineae per repositorium TOTUM efficeret - cursus proprius,
 * benedictio propria.
 *
 * '-posix' MANET ut escape: totum lexicon onerat, mos vetus exacte.
 *
 * Sectiones in systema_posix.h notantur:
 *   <sectio caput="fcntl.h"/>              - una capiti respondet
 *   <sectio caput="a.h,b.h"/>              - pluribus
 *   <sectio semper="verum"/>               - basis (typi communes)
 *   <sectio caput="x.h" poscit="y.h"/>     - dependentia inter
 *                                            sectiones
 *
 * 'poscit' NECESSARIUM est quia sectiones se invicem tangunt:
 * select() struct timeval ex sys/time.h poscit, struct stat
 * struct timespec. Sine eo derivatio lexicon FRACTUM pareret.
 * Clausura transitiva computatur; sectiones ORDINE FONTIS
 * emittuntur, ergo ordo declarationum per constructionem rectus est.
 * ================================================== */

#define SECTIONES_MAXIMAE 64

nomen structura {
    i32 initium;     /* contentum: post '\*\/' commentarii notantis */
    i32 finis;
    i32 caput_i;
    i32 caput_m;
    i32 poscit_i;
    i32 poscit_m;
    b32 semper;
    b32 petita;
} SectioLexici;

/* attributum="valor" intra tagum invenire */
hic_manens b32
_attributum_capere (constans character* fons, i32 initium, i32 finis,
    constans character* titulus, i32* valor_i, i32* valor_m)
{
    i32 n = (i32)strlen(titulus);
    i32 i;

    *valor_i = ZEPHYRUM;
    *valor_m = ZEPHYRUM;
    per (i = initium; i + n + II < finis; i++)
    {
        si (strncmp(fons + i, titulus, (memoriae_index)n) != ZEPHYRUM)
        {
            perge;
        }
        si (fons[i + n] != '=' || fons[i + n + I] != '"')
        {
            perge;
        }
        {
            i32 v = i + n + II;
            i32 f = v;

            dum (f < finis && fons[f] != '"') f++;
            si (f >= finis) redde FALSUM;
            *valor_i = v;
            *valor_m = f - v;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* an nomen in lista commatibus separata stet (comparatio TOTA) */
hic_manens b32
_in_lista_nominum (constans character* lista, i32 lista_m,
    constans character* titulus, i32 titulus_m)
{
    i32 i = ZEPHYRUM;

    si (titulus_m == ZEPHYRUM) redde FALSUM;
    dum (i < lista_m)
    {
        i32 initium = i;
        i32 longitudo;

        dum (i < lista_m && lista[i] != ',') i++;
        longitudo = i - initium;
        si (longitudo == titulus_m
            && strncmp(lista + initium, titulus,
                (memoriae_index)longitudo) == ZEPHYRUM)
        {
            redde VERUM;
        }
        si (i < lista_m) i++;
    }
    redde FALSUM;
}

/* nomina ex lineis '#include <...>' colligere, commatibus iuncta.
 * Inclusiones '"locales"' CONSULTO praetermittuntur: eae per
 * ambulationem capitum repositorii iam resolvuntur. */
hic_manens i32
_inclusa_colligere (constans character* fons, i32 mensura,
    character* effusio, i32 capacitas)
{
    i32 longitudo = ZEPHYRUM;
    i32 i = ZEPHYRUM;

    dum (i + IX < mensura)
    {
        i32 j;
        i32 initium;

        si (strncmp(fons + i, "#include", VIII) != ZEPHYRUM)
        {
            i++;
            perge;
        }
        j = i + VIII;
        dum (j < mensura && (fons[j] == ' ' || fons[j] == '\t')) j++;
        si (j >= mensura || fons[j] != '<')
        {
            i = (j > i) ? j : i + I;
            perge;
        }
        j++;
        initium = j;
        dum (j < mensura && fons[j] != '>' && fons[j] != '\n') j++;
        si (j < mensura && fons[j] == '>' && j > initium)
        {
            i32 n = j - initium;

            si (longitudo + n + I < capacitas)
            {
                si (longitudo > ZEPHYRUM)
                {
                    effusio[longitudo] = ',';
                    longitudo++;
                }
                memcpy(effusio + longitudo, fons + initium,
                    (memoriae_index)n);
                longitudo = longitudo + n;
            }
        }
        i = j;
    }
    redde longitudo;
}

hic_manens character*
_posix_derivare (constans character* fons_px, i32 mensura_px,
    constans character* fons_pl, i32 mensura_pl, Piscina* piscina,
    i32* mensura_out)
{
    SectioLexici sectiones[SECTIONES_MAXIMAE];
    i32 numerus = ZEPHYRUM;
    i32 capacitas = mensura_pl + mensura_px + CC;
    character* petita;
    i32 petita_m;
    character* effusio;
    i32 longitudo = ZEPHYRUM;
    i32 i;
    b32 mutatum;

    *mensura_out = ZEPHYRUM;
    petita = (character*)piscina_allocare(piscina,
        (memoriae_index)capacitas);
    effusio = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_px + SECTIONES_MAXIMAE + I));
    si (petita == NIHIL || effusio == NIHIL) redde NIHIL;
    petita_m = _inclusa_colligere(fons_pl, mensura_pl, petita,
        capacitas);

    /* sectiones scrutari. Contentum post '*''/' commentarii notantis
     * incipit (non post tagum ipsum): alioquin cauda commentarii
     * ' ... *''/' in textum emissum flueret et parsationem
     * frangeret. Sectio praecedens ad '/''*' huius commentarii
     * desinit, eadem de causa. */
    i = ZEPHYRUM;
    dum (i + VII < mensura_px)
    {
        i32 tag_finis;
        i32 com_initium;
        i32 com_finis;
        i32 vi;
        i32 vm;

        si (strncmp(fons_px + i, "<sectio", VII) != ZEPHYRUM)
        {
            i++;
            perge;
        }
        tag_finis = i;
        dum (tag_finis < mensura_px && fons_px[tag_finis] != '>')
        {
            tag_finis++;
        }
        com_initium = i;
        dum (com_initium > ZEPHYRUM
            && !(fons_px[com_initium] == '/'
                 && com_initium + I < mensura_px
                 && fons_px[com_initium + I] == '*'))
        {
            com_initium--;
        }
        com_finis = tag_finis;
        dum (com_finis + I < mensura_px
            && !(fons_px[com_finis] == '*'
                 && fons_px[com_finis + I] == '/'))
        {
            com_finis++;
        }
        com_finis = com_finis + II;
        si (com_finis > mensura_px) com_finis = mensura_px;

        si (numerus > ZEPHYRUM)
        {
            sectiones[numerus - I].finis = com_initium;
        }
        si (numerus >= SECTIONES_MAXIMAE)
        {
            fprintf(stderr, "examen: sectiones lexici plus quam"
                " %d\n", (int)SECTIONES_MAXIMAE);
            redde NIHIL;
        }
        sectiones[numerus].initium = com_finis;
        sectiones[numerus].finis = mensura_px;
        sectiones[numerus].caput_i = ZEPHYRUM;
        sectiones[numerus].caput_m = ZEPHYRUM;
        sectiones[numerus].poscit_i = ZEPHYRUM;
        sectiones[numerus].poscit_m = ZEPHYRUM;
        sectiones[numerus].semper = FALSUM;
        sectiones[numerus].petita = FALSUM;
        si (_attributum_capere(fons_px, i, tag_finis, "caput",
                &vi, &vm))
        {
            sectiones[numerus].caput_i = vi;
            sectiones[numerus].caput_m = vm;
        }
        si (_attributum_capere(fons_px, i, tag_finis, "poscit",
                &vi, &vm))
        {
            sectiones[numerus].poscit_i = vi;
            sectiones[numerus].poscit_m = vm;
        }
        si (_attributum_capere(fons_px, i, tag_finis, "semper",
                &vi, &vm))
        {
            sectiones[numerus].semper = VERUM;
        }
        numerus++;
        i = tag_finis;
    }

    /* clausura transitiva sub 'poscit' */
    mutatum = VERUM;
    dum (mutatum)
    {
        mutatum = FALSUM;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            b32 volo;

            si (sectiones[i].petita) perge;
            volo = sectiones[i].semper;
            si (!volo && sectiones[i].caput_m > ZEPHYRUM)
            {
                constans character* lista = fons_px
                    + sectiones[i].caput_i;
                i32 lista_m = sectiones[i].caput_m;
                i32 p = ZEPHYRUM;

                dum (p < lista_m && !volo)
                {
                    i32 ini = p;
                    i32 lon;

                    dum (p < lista_m && lista[p] != ',') p++;
                    lon = p - ini;
                    si (_in_lista_nominum(petita, petita_m,
                            lista + ini, lon))
                    {
                        volo = VERUM;
                    }
                    si (p < lista_m) p++;
                }
            }
            si (!volo) perge;
            sectiones[i].petita = VERUM;
            mutatum = VERUM;
            si (sectiones[i].poscit_m > ZEPHYRUM
                && petita_m + sectiones[i].poscit_m + I < capacitas)
            {
                si (petita_m > ZEPHYRUM)
                {
                    petita[petita_m] = ',';
                    petita_m++;
                }
                memcpy(petita + petita_m,
                    fons_px + sectiones[i].poscit_i,
                    (memoriae_index)sectiones[i].poscit_m);
                petita_m = petita_m + sectiones[i].poscit_m;
            }
        }
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i32 n;

        si (!sectiones[i].petita) perge;
        n = sectiones[i].finis - sectiones[i].initium;
        si (n <= ZEPHYRUM) perge;
        memcpy(effusio + longitudo, fons_px + sectiones[i].initium,
            (memoriae_index)n);
        longitudo = longitudo + n;
        effusio[longitudo] = '\n';
        longitudo++;
    }
    si (longitudo == ZEPHYRUM) redde NIHIL;
    effusio[longitudo] = '\0';
    *mensura_out = longitudo;
    redde effusio;
}

/* <externa caput="X.h"> ... </externa> - superficies externae in
 * COMMENTARIO declaratae: .d.ts pro C, ad limitem platformae.
 *
 * ORDO PORTANS - cur scrutinium TEXTUS et non ambulatio arboris:
 * hoc ANTE parsationem legi debet, quia typos definit quibus
 * parsatio ipsa eget. Arbor quae hinc penderet nondum exsistit cum
 * opus est. Ergo textum crudum scrutamur, ante omnia.
 *
 * 'caput=' REQUIRITUR et clavis promotionis est: nominat sectionem
 * lexici globalis pro qua hic blocus vicarius stat. Cum lexicon
 * illam sectionem acceperit, blocus deleri potest - et duplicatio
 * detegi. Sine eo, derivatio-ex-inclusionibus futura scire non
 * posset an sectio globalis hunc blocum superet.
 *
 * Praefixum ' * ' cuiusque lineae tollitur (mos commentarii C).
 *
 * PERICULUM ACCEPTUM: scrutinium crudum est, ergo '<externa' intra
 * litteram chordae captum iri posset. Pretium falsum est monitio
 * amissa, numquam mendum missum (vide infra), ergo v1 id fert.
 *
 * ASYMMETRIA PERICULI (ratio huius rei): declaratio errans efficere
 * potest SOLUM ut examen codicem ACCIPIAT quem clang postea REICIT.
 * examen monitio praevia est; clang cum vexillis plenis porta vera
 * manet. Ergo mendacium locale monitionem priorem constat - dissimile
 * EXCLUSIONI, quae omnia silet et plagulam CAECAM relinquit. */
hic_manens character*
_externa_excerpere (constans character* fons, i32 mensura,
    Piscina* piscina, i32* mensura_out, constans character* via,
    b32* fractum)
{
    character* effusio;
    i32 longitudo = ZEPHYRUM;
    i32 i = ZEPHYRUM;

    /* 'fractum' ab 'absente' DISCERNENDUM: NIHIL sine fracto =
     * nullus blocus (casus communis, tacitus); NIHIL cum fracto =
     * annotatio prava, quae iudicium SISTERE debet. Sine hac
     * distinctione blocus pravus verdictum ACCIPE tacite pareret -
     * id est ipsum genus vitii quod haec annotatio vitare debet. */
    *fractum = FALSUM;
    *mensura_out = ZEPHYRUM;
    si (fons == NIHIL || mensura == ZEPHYRUM) redde NIHIL;
    effusio = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    si (effusio == NIHIL) redde NIHIL;

    dum (i + VIII < mensura)
    {
        i32 apertura;
        i32 initium;
        i32 finis;
        b32 caput_adest = FALSUM;
        i32 k;

        si (!(fons[i] == '<'
              && strncmp(fons + i, "<externa", VIII) == ZEPHYRUM
              && (fons[i + VIII] == ' ' || fons[i + VIII] == '>'
                  || fons[i + VIII] == '\n')))
        {
            i++;
            perge;
        }
        apertura = i + VIII;
        dum (apertura < mensura && fons[apertura] != '>')
        {
            apertura++;
        }
        si (apertura >= mensura)
        {
            fprintf(stderr, "examen: <externa> non clausum (%s)\n",
                via);
            *fractum = VERUM;
            redde NIHIL;
        }
        per (k = i; k + VI < apertura; k++)
        {
            si (strncmp(fons + k, "caput=", VI) == ZEPHYRUM)
            {
                caput_adest = VERUM;
                frange;
            }
        }
        si (!caput_adest)
        {
            fprintf(stderr, "examen: <externa> sine 'caput=' (%s) -"
                " caput nominandum est (clavis promotionis)\n", via);
            *fractum = VERUM;
            redde NIHIL;
        }
        initium = apertura + I;
        finis = initium;
        dum (finis + X <= mensura
            && strncmp(fons + finis, "</externa>", X) != ZEPHYRUM)
        {
            finis++;
        }
        si (finis + X > mensura)
        {
            fprintf(stderr, "examen: </externa> abest (%s)\n", via);
            *fractum = VERUM;
            redde NIHIL;
        }
        {
            i32 p = initium;
            b32 initium_lineae = VERUM;

            dum (p < finis)
            {
                si (initium_lineae)
                {
                    dum (p < finis
                        && (fons[p] == ' ' || fons[p] == '\t'))
                    {
                        p++;
                    }
                    si (p < finis && fons[p] == '*'
                        && !(p + I < finis && fons[p + I] == '/'))
                    {
                        p++;
                        si (p < finis && fons[p] == ' ') p++;
                    }
                    initium_lineae = FALSUM;
                    perge;
                }
                si (fons[p] == '\n') initium_lineae = VERUM;
                effusio[longitudo] = fons[p];
                longitudo++;
                p++;
            }
            effusio[longitudo] = '\n';
            longitudo++;
        }
        i = finis + X;
    }
    si (longitudo == ZEPHYRUM) redde NIHIL;
    effusio[longitudo] = '\0';
    *mensura_out = longitudo;
    redde effusio;
}

s32 principale (integer argc, character** argv)
{
    constans character* via = NIHIL;
    Piscina* piscina;
    SilvaContextus* ctx;
    SilvaParsura* systema_parsura = NIHIL;
    SilvaSemantica* systema_semantica = NIHIL;
    character* fons_plagulae = NIHIL;
    i32 mensura_plagulae = ZEPHYRUM;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-v") == ZEPHYRUM)
        {
            verbosa = VERUM;
        }
        alioquin si (strcmp(argv[k], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[k], "-posix") == ZEPHYRUM)
        {
            cum_posix = VERUM;
        }
        alioquin si (strcmp(argv[k], "-nudum") == ZEPHYRUM)
        {
            nudum = VERUM;
        }
        alioquin
        {
            via = argv[k];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: examen <plagula.c> [-posix|-nudum]"
            " [-machina] [-v]\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("examen", 268435456);
    si (piscina != NIHIL)
    {
        /* vigilia sui (2026-07-17): binarium hoc SOLUM examen.sh
         * religat - vocatio directa (circuli celeres) post
         * editionem fontium binarium VETUSTUM currit tacite
         * (decipula 2026-07-16: "custos alienitatis non operatur"
         * = custos in binario numquam erat). Manifestum involucrum
         * scribit (invarians aedificatoris); absentia = quieta.
         * Cautio in STDERR solum - effusum -machina intactum. */
        VigiliaConfiguratio vc;
        Vigilia* vigilia;

        vc.signum = NIHIL;
        vc.via_binarii = argv[ZEPHYRUM];
        vc.via_manifesti = "silva/build/examen.manifestum";
        vigilia = vigilia_creare(piscina, &vc);
        si (vigilia != NIHIL
            && vigilia_inspicere(vigilia, piscina)
                == VIGILIA_FONTES_SUPERANT)
        {
            /* nuntius proprius (cautio bibliothecae residentibus
             * scripta est - "/mcp reconnect" hic fallax) */
            fprintf(stderr, "CAUTIO examen: binarium vetustius"
                " fontibus (%s) - per ./silva/examen.sh recurre\n",
                vigilia_causa(vigilia));
        }
    }
    si (piscina == NIHIL)
    {
        fprintf(stderr, "examen: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "examen: contextus deest\n");
        redde II;
    }

    /* plagula PRIMUM legitur (non ubi iudicatur): blocos <externa>
     * ante compositionem lexici excerpere oportet */
    fons_plagulae = _plagulam_legere(piscina, via, &mensura_plagulae);
    si (fons_plagulae == NIHIL)
    {
        fprintf(stderr, "examen: plagula illegibilis: %s\n", via);
        redde II;
    }

    /* systema (lexicon): ISO ordinarius; -posix concatenat
     * (exemplar vindex_onerator); -nudum = sine (LEXICON_DEEST
     * probandus) */
    si (!nudum)
    {
        character* fons_sys;
        i32 mensura_sys = ZEPHYRUM;

        fons_sys = _plagulam_legere(piscina,
            "silva/fontes/systema_c89.h", &mensura_sys);
        si (fons_sys == NIHIL)
        {
            fprintf(stderr, "examen: systema_c89.h deest (curre"
                " ex radice repositorii)\n");
            redde II;
        }
        /* POSIX: '-posix' totum onerat (mos vetus, escape); alioquin
         * sectiones EX INCLUSIONIBUS plagulae derivantur. Plagula
         * quae nullum caput POSIX includit nihil accipit - ISO purum,
         * ut antea. */
        {
            i32 mensura_px = ZEPHYRUM;
            character* fons_px = _plagulam_legere(piscina,
                "silva/fontes/systema_posix.h", &mensura_px);
            character* pars = NIHIL;
            i32 pars_m = ZEPHYRUM;

            si (fons_px == NIHIL)
            {
                fprintf(stderr,
                    "examen: systema_posix.h deest\n");
                redde II;
            }
            si (cum_posix)
            {
                pars = fons_px;
                pars_m = mensura_px;
            }
            alioquin
            {
                pars = _posix_derivare(fons_px, mensura_px,
                    fons_plagulae, mensura_plagulae, piscina,
                    &pars_m);
            }
            si (pars != NIHIL && pars_m > ZEPHYRUM)
            {
                character* iunctum = (character*)piscina_allocare(
                    piscina, (memoriae_index)(mensura_sys
                        + pars_m + I));

                si (iunctum == NIHIL)
                {
                    redde II;
                }
                memcpy(iunctum, fons_sys,
                    (memoriae_index)mensura_sys);
                iunctum[mensura_sys] = '\n';
                memcpy(iunctum + mensura_sys + I, pars,
                    (memoriae_index)pars_m);
                fons_sys = iunctum;
                mensura_sys = mensura_sys + I + pars_m;
                si (verbosa)
                {
                    fprintf(stderr, "examen: POSIX %d octeti (%s)\n",
                        (int)pars_m,
                        cum_posix ? "-posix, totum" : "derivatum");
                }
            }
        }
        /* <externa> plagulae: lexicon LOCALE, eodem canali quo
         * systema globale - superficies quas lexicon nondum fert
         * (sockets, poll) hic a plagula ipsa portantur, pro
         * exclusione quae plagulam TOTAM caecam faceret */
        {
            i32 mensura_ext = ZEPHYRUM;
            b32 ext_fractum = FALSUM;
            character* fons_ext = _externa_excerpere(fons_plagulae,
                mensura_plagulae, piscina, &mensura_ext, via,
                &ext_fractum);

            /* annotatio prava = INFRA (apparatus), numquam ACCIPE:
             * verdictum mundum ex annotatione fracta est mendacium */
            si (ext_fractum)
            {
                redde II;
            }
            si (fons_ext != NIHIL && mensura_ext > ZEPHYRUM)
            {
                character* iunctum = (character*)piscina_allocare(
                    piscina, (memoriae_index)(mensura_sys
                        + mensura_ext + I));

                si (iunctum == NIHIL)
                {
                    redde II;
                }
                memcpy(iunctum, fons_sys,
                    (memoriae_index)mensura_sys);
                iunctum[mensura_sys] = '\n';
                memcpy(iunctum + mensura_sys + I, fons_ext,
                    (memoriae_index)mensura_ext);
                fons_sys = iunctum;
                mensura_sys = mensura_sys + I + mensura_ext;
                si (verbosa)
                {
                    fprintf(stderr, "examen: <externa> %d octeti ex"
                        " %s\n", (int)mensura_ext, via);
                }
            }
        }
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, mensura_sys))
        {
            fprintf(stderr, "examen: lexicon non additum\n");
            redde II;
        }
        systema_parsura = silva_c89_parsare(piscina,
            "systema_c89.h", fons_sys, mensura_sys, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "examen: systema non parsatum\n");
            redde II;
        }
        systema_semantica = silva_c89_semantica_analysare(piscina,
            systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "examen: systema non analysatum\n");
            redde II;
        }
    }

    /* capita repositorii (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina, DXII);

        si (visa != NIHIL)
        {
            _capita_praeparare(ctx, piscina, visa, ".");
        }
        si (verbosa)
        {
            fprintf(stderr, "examen: capita %d praebita\n",
                (int)capita_praebita);
        }
    }

    /* plagula ipsa: parsare + bis-analysis (recipe percursus) */
    {
        i32 mensura = mensura_plagulae;
        character* fons = fons_plagulae;   /* supra lectum */
        SilvaOraculum* oraculum;
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        i32 violationes = ZEPHYRUM;
        i32 suspecta = ZEPHYRUM;
        i32 domestica = ZEPHYRUM;
        i32 infra = ZEPHYRUM;
        i32 provisionalia = ZEPHYRUM;
        b32 reice = FALSUM;

        si (fons == NIHIL)
        {
            fprintf(stderr, "examen: plagula illegibilis: %s\n",
                via);
            redde II;
        }
        oraculum = silva_oraculum_creare(piscina);
        si (oraculum != NIHIL && systema_semantica != NIHIL)
        {
            (vacuum)silva_c89_semantica_oraculum_augere(
                systema_semantica, oraculum);
        }
        parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
            fons, mensura, oraculum);
        si (parsura == NIHIL || !parsura->successus
            || parsura->commissio == NIHIL)
        {
            fprintf(stderr, "examen: apparatus fractus: %s\n", via);
            redde II;
        }
        sem = silva_c89_semantica_analysare_cum_systemate(piscina,
            parsura, systema_parsura);
        si (sem != NIHIL && oraculum != NIHIL)
        {
            /* clausura: symbola plagulae in oraculum ->
             * recanonicare -> analysis auctoritativa */
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum);
            silva_oraculum_responsa_vacare(oraculum);
            (vacuum)silva_recanonicare(parsura->commissio, oraculum,
                silva_c89_resolutor, NIHIL);
            sem = silva_c89_semantica_analysare_cum_systemate(
                piscina, parsura, systema_parsura);
        }
        si (sem == NIHIL)
        {
            fprintf(stderr, "examen: semantica fracta: %s\n", via);
            redde II;
        }

        si (parsura->numerus_errorum > ZEPHYRUM)
        {
            reice = VERUM;   /* syntaxis fracta = reiectio C89 */
            si (machina)
            {
                imprimere("%s\t0\t0\tviolatio\t-1\t0\t"
                    "nodi erroris (syntaxis) %d\n", via,
                    (int)parsura->numerus_errorum);
            }
            alioquin
            {
                imprimere("%s: [violatio] nodi erroris (syntaxis)"
                    " %d\n", via, (int)parsura->numerus_errorum);
            }
        }

        {
            i32 i;
            i32 m = (i32)silva_c89_diagnostica_numerus(sem);

            per (i = ZEPHYRUM; i < m; i++)
            {
                constans SemanticaDiagnosticum* d =
                    silva_c89_diagnosticum_per_indicem(sem, i);

                si (d == NIHIL)
                {
                    perge;
                }
                si (d->provisionale)
                {
                    provisionalia++;
                }
                commutatio (d->severitas)
                {
                    casus (s32)EXAMEN_VIOLATIO:
                        violationes++;
                        si (!d->provisionale)
                        {
                            reice = VERUM;
                        }
                        frange;
                    casus (s32)EXAMEN_SUSPECTUM:
                        suspecta++;
                        frange;
                    casus (s32)EXAMEN_DOMESTICUM:
                        domestica++;
                        frange;
                    ordinarius:
                        infra++;
                        frange;
                }
                si (machina)
                {
                    imprimere("%.*s\t%d\t%d\t%s\t%d\t%d\t%s\n",
                        (int)d->via.mensura,
                        (constans character*)d->via.datum,
                        (int)d->linea, (int)d->columna,
                        _severitatis_titulus(d->severitas),
                        (int)d->codex,
                        (int)(d->provisionale ? I : ZEPHYRUM),
                        d->causa);
                }
                alioquin
                {
                    imprimere("%.*s:%d:%d: [%s] %s%s\n",
                        (int)d->via.mensura,
                        (constans character*)d->via.datum,
                        (int)d->linea, (int)d->columna,
                        _severitatis_titulus(d->severitas),
                        d->causa,
                        d->provisionale ? " (provisionale)" : "");
                }
            }
        }

        si (machina)
        {
            imprimere("VERDICTUM\t%s\n",
                reice ? "REICE" : "ACCIPE");
        }
        alioquin
        {
            imprimere("verdictum: %s  (violationes %d, suspecta %d,"
                " domestica %d, infra %d; provisionalia %d)\n",
                reice ? "REICE" : "ACCIPE", (int)violationes,
                (int)suspecta, (int)domestica, (int)infra,
                (int)provisionalia);
        }
        piscina_destruere(piscina);
        redde reice ? I : ZEPHYRUM;
    }
}
