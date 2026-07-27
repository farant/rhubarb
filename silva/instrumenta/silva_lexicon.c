/* silva_lexicon.c - compositio lexici systematis per-plagulam
 *
 * Doctrina tota in capite (silva_lexicon.h). Hic mechanica sola.
 *
 * CAVE SCRIPTORI HUIUS PLAGULAE: sequentia aperiens tagi externa
 * (uncus angularis + verbum + spatium/uncus/linea nova) in
 * commentario aut in littera chordae NON scribatur - excerptor
 * plagulam iudicatam TEXTUALITER scrutatur, ergo plagula haec (et
 * examen.c ante extractionem - vitium verum, exitus II in se ipsum)
 * se ipsam falleret. Litterae chordarum infra tutae sunt quia
 * character sequens ('"' aut '/') custodiam non transit; nuntii
 * erroris verbo nudo 'externa' CONSULTO utuntur, sine uncis.
 */

#include "latina.h"
#include "piscina.h"
#include "silva_lexicon.h"
#include <stdio.h>
#include <string.h>

#define SECTIONES_MAXIMAE 64

nomen structura {
    i32 initium;     /* contentum: post finem commentarii notantis */
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

character*
silva_lexicon_posix_derivare (constans character* fons_px,
    i32 mensura_px, constans character* fons_pl, i32 mensura_pl,
    Piscina* piscina, i32* mensura_out)
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
    si (fons_px == NIHIL || fons_pl == NIHIL) redde NIHIL;
    petita = (character*)piscina_allocare(piscina,
        (memoriae_index)capacitas);
    effusio = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_px + SECTIONES_MAXIMAE + I));
    si (petita == NIHIL || effusio == NIHIL) redde NIHIL;
    petita_m = _inclusa_colligere(fons_pl, mensura_pl, petita,
        capacitas);

    /* sectiones scrutari. Contentum post FINEM commentarii notantis
     * incipit (non post tagum ipsum): alioquin cauda commentarii in
     * textum emissum flueret et parsationem frangeret. Sectio
     * praecedens ad INITIUM eiusdem commentarii desinit, eadem de
     * causa. */
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
            fprintf(stderr, "lexicon: sectiones plus quam %d\n",
                (int)SECTIONES_MAXIMAE);
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
        /* <tolera codex="SUBTRACTIO_COMPARATA" (>sectio structuraliter: finis post initium a scansore posita */
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

character*
silva_lexicon_externa_excerpere (constans character* fons,
    i32 mensura, Piscina* piscina, i32* mensura_out,
    constans character* via, b32* fractum)
{
    character* effusio;
    i32 longitudo = ZEPHYRUM;
    i32 i = ZEPHYRUM;

    /* 'fractum' ab 'absente' discernitur - contractus in capite */
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
            fprintf(stderr, "lexicon: tagus externa non clausus"
                " (%s)\n", via);
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
            fprintf(stderr, "lexicon: blocus externa sine 'caput='"
                " (%s) - caput nominandum est (clavis"
                " promotionis)\n", via);
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
            fprintf(stderr, "lexicon: clausura bloci externa abest"
                " (%s)\n", via);
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

character*
silva_lexicon_componere (constans character* fons_iso,
    i32 mensura_iso, constans character* fons_px, i32 mensura_px,
    constans character* fons_pl, i32 mensura_pl, b32 totum_posix,
    Piscina* piscina, i32* mensura_out, constans character* via,
    b32* fractum)
{
    constans character* pars = NIHIL;
    i32 pars_m = ZEPHYRUM;
    character* ext = NIHIL;
    i32 ext_m = ZEPHYRUM;
    character* effusio;
    i32 longitudo;

    *fractum = FALSUM;
    *mensura_out = ZEPHYRUM;
    si (fons_iso == NIHIL) redde NIHIL;

    si (totum_posix)
    {
        pars = fons_px;
        pars_m = mensura_px;
    }
    alioquin si (fons_px != NIHIL && fons_pl != NIHIL)
    {
        pars = silva_lexicon_posix_derivare(fons_px, mensura_px,
            fons_pl, mensura_pl, piscina, &pars_m);
    }
    si (fons_pl != NIHIL)
    {
        ext = silva_lexicon_externa_excerpere(fons_pl, mensura_pl,
            piscina, &ext_m, via, fractum);
        si (*fractum) redde NIHIL;
    }

    effusio = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_iso + pars_m + ext_m + III));
    si (effusio == NIHIL) redde NIHIL;
    memcpy(effusio, fons_iso, (memoriae_index)mensura_iso);
    longitudo = mensura_iso;
    si (pars != NIHIL && pars_m > ZEPHYRUM)
    {
        effusio[longitudo] = '\n';
        longitudo++;
        memcpy(effusio + longitudo, pars, (memoriae_index)pars_m);
        longitudo = longitudo + pars_m;
    }
    si (ext != NIHIL && ext_m > ZEPHYRUM)
    {
        effusio[longitudo] = '\n';
        longitudo++;
        memcpy(effusio + longitudo, ext, (memoriae_index)ext_m);
        longitudo = longitudo + ext_m;
    }
    effusio[longitudo] = '\0';
    *mensura_out = longitudo;
    redde effusio;
}
