/* tools/natura_glossae.c - census glossarum (spec glossae par. 5)
 *
 * Copertura documentationis per corpus generum: 'la' per
 * <definitio> exsistentem, ceterae linguae per <glossa lingua=>.
 * Relatio ordinaria numquam obstat (index operum est dum corpus
 * rubet); -porta obstat. Denominator = genera propria (non
 * species/umbrae/individua).
 *
 * Divisio laboris (spec par. 3): canon FORMAM iudicat (lingua
 * necessaria - natura.canon), hic COPERTURA et GEMINA.
 *
 * -html VIA: encyclopaedia et lacunae FUSAE - plagula una sine ope
 * externa, DETERMINISTICA (nulla tempora: porta output confert,
 * numquam tempora - -probare crustae byte confert).
 *
 * NB 'glossae' in natura_examen/visus aliud est (sententia prima
 * definitionis, glossae.txt) - stratum vetus, non hoc.
 *
 * Exitus: 0 relatio/porta-serena, 1 porta cum lacunis, 2 NIHIL
 * CURSUM (argumenta mala, corpus vacuum) - numquam sanitas tacita.
 */
#include "postulata_posix.h"

#include "latina.h"
#include "natura.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO           ".genera"
#define EXTENSIO_LONGA     VII
#define NG_LINGUAE_MAXIMAE VIII
#define NG_CODEX_LONGUS    VIII
#define NG_COLUMNA         XXIV

nomen structura {
    character  codices[NG_LINGUAE_MAXIMAE][NG_CODEX_LONGUS];
    i32        numerus;
} NgLinguae;

interior b32    _extensionem_habet(constans chorda* titulus);
interior vacuum _stirpem_scribere(constans chorda* titulus,
                                  character* exitus, i32 capacitas);
interior b32    _linguas_legere(constans character* lista,
                                NgLinguae* linguae);
interior i32    _genus_linguae_quot(NaturaGenus* g,
                                    constans character* codex);
interior vacuum _columnam_scribere(FILE* f, constans chorda* t,
                                   i32 latitudo);

interior b32
_extensionem_habet(
    constans chorda*  titulus)
{
    si (titulus->mensura <= (i32)EXTENSIO_LONGA)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(titulus->datum +
                    (titulus->mensura - (i32)EXTENSIO_LONGA),
                    EXTENSIO, (size_t)EXTENSIO_LONGA) == ZEPHYRUM);
}

interior vacuum
_stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               capacitas)
{
    i32 longitudo;
    i32 i;

    longitudo = titulus->mensura - (i32)EXTENSIO_LONGA;
    si (longitudo >= capacitas)
    {
        longitudo = capacitas - I;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        exitus[i] = (character)titulus->datum[i];
    }
    exitus[longitudo] = '\0';
}

/* 'la,en,fr' -> codices; FALSUM si vacuus/longus/nimii */
interior b32
_linguas_legere(
    constans character*  lista,
    NgLinguae*           linguae)
{
    i32 n;
    i32 j;

    linguae->numerus = ZEPHYRUM;
    n = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (VERUM)
    {
        character c;

        c = lista[n];
        si (c == ',' || c == '\0')
        {
            si (j == ZEPHYRUM ||
                linguae->numerus >= (i32)NG_LINGUAE_MAXIMAE)
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = '\0';
            linguae->numerus++;
            j = ZEPHYRUM;
            si (c == '\0')
            {
                frange;
            }
        }
        alioquin
        {
            si (j >= (i32)(NG_CODEX_LONGUS - I))
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = c;
            j++;
        }
        n++;
    }
    redde VERUM;
}

/* quot vices genus linguam ferat: 'la' = definitio AUT differentia
 * exsistens (0/1) - sub-genera differentia sola consulto utuntur
 * (definitio = genus proximum + differentia; mos natura_examen,
 * qui glossam derivatam eodem cadente petit), ceterae = glossae
 * congruentes numeratae (>1 = GEMINA) */
interior i32
_genus_linguae_quot(
    NaturaGenus*         g,
    constans character*  codex)
{
    i32 numerus;
    i32 i;
    i32 quot;

    si (strcmp(codex, "la") == ZEPHYRUM)
    {
        redde (stml_invenire_liberum(g->nodus, "definitio") ||
               stml_invenire_liberum(g->nodus, "differentia"))
                  ? I : ZEPHYRUM;
    }

    quot    = ZEPHYRUM;
    numerus = stml_numerus_liberorum(g->nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    lingua;

        l = stml_liberum_ad_indicem(g->nodus, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "glossa"))
        {
            perge;
        }
        lingua = stml_attributum_capere(l, "lingua");
        si (lingua && chorda_aequalis_literis(*lingua, codex))
        {
            quot++;
        }
    }
    redde quot;
}

interior vacuum
_columnam_scribere(
    FILE*             f,
    constans chorda*  t,
    i32               latitudo)
{
    i32 i;

    fprintf(f, "%.*s", (integer)t->mensura,
            (constans character*)t->datum);
    per (i = t->mensura; i < latitudo; i++)
    {
        putc(' ', f);
    }
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*              piscina;
    NaturaBibliotheca*    bib;
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    constans character*   radix;
    constans character*   lista_linguarum;
    constans character*   via_html;
    NgLinguae             linguae;
    b32                   modus_porta;
    b32                   modus_machina;
    s32                   i;
    i32                   onerata;
    i32                   lacunae;
    i32                   m;
    i32                   g_i;
    i32                   l_i;
    i32                   summa_habentia[NG_LINGUAE_MAXIMAE];
    i32                   summa_omnia;

    radix           = "natura";
    lista_linguarum = "la,en,fr";
    via_html        = NIHIL;
    modus_porta     = FALSUM;
    modus_machina   = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-porta") == ZEPHYRUM)
        {
            modus_porta = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            modus_machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-linguae") == ZEPHYRUM &&
                     i + I < numerus)
        {
            lista_linguarum = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-html") == ZEPHYRUM &&
                     i + I < numerus)
        {
            via_html = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_glossae [-radix DIR] "
                "[-linguae la,en,fr] [-porta] [-machina] "
                "[-html VIA]\n");
            redde II;
        }
    }

    si (!_linguas_legere(lista_linguarum, &linguae))
    {
        fprintf(stderr,
            "natura_glossae: -linguae '%s' legi nequit\n",
            lista_linguarum);
        redde II;
    }

    piscina = piscina_generare_dynamicum("natura_glossae", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr,
            "natura_glossae: bibliotheca creari nequit\n");
        redde II;
    }

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_glossae: '%s' aperiri nequit\n",
                radix);
        redde II;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
        {
            perge;
        }
        si ((size_t)introitus->titulus.mensura + strlen(radix) + II >
            magnitudo(via))
        {
            perge;
        }
        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        _stirpem_scribere(&introitus->titulus, stirps,
                          (i32)magnitudo(stirps));
        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_glossae: '%s' legi nequit\n",
                    via);
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: NULLUM exemplar in '%s' - nihil "
            "cursum\n", radix);
        redde II;
    }

    /* vulnera corporis lint alterius sunt (natura_examen) -
     * copertura etiam super corpore vulnerato numeratur */
    natura_nectere(bib);

    /* ---- tabula coperturae ---- */

    summa_omnia = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        summa_habentia[l_i] = ZEPHYRUM;
    }

    si (!modus_machina)
    {
        printf("%-24s", "GLOSSAE");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("%-10s", linguae.codices[l_i]);
        }
        printf("\n");
    }

    per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
    {
        NaturaExemplar* ex;
        i32             omnia_moduli;
        i32             habentia[NG_LINGUAE_MAXIMAE];

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, m);
        omnia_moduli = ZEPHYRUM;
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            habentia[l_i] = ZEPHYRUM;
        }

        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (!chorda_aequalis(*g->modulus, *ex->stirps))
            {
                perge;
            }
            omnia_moduli++;
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                si (_genus_linguae_quot(g,
                        linguae.codices[l_i]) >= (i32)I)
                {
                    habentia[l_i]++;
                }
            }
        }

        si (omnia_moduli == ZEPHYRUM)
        {
            perge;   /* exemplar sine generibus propriis */
        }
        summa_omnia += omnia_moduli;

        si (modus_machina)
        {
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                _columnam_scribere(stdout, ex->stirps, ZEPHYRUM);
                printf("\t%s\t%u\t%u\n", linguae.codices[l_i],
                       habentia[l_i], omnia_moduli);
                summa_habentia[l_i] += habentia[l_i];
            }
        }
        alioquin
        {
            _columnam_scribere(stdout, ex->stirps,
                               (i32)NG_COLUMNA);
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                character fractio[XXXII];

                sprintf(fractio, "%u/%u", habentia[l_i],
                        omnia_moduli);
                printf("%-10s", fractio);
                summa_habentia[l_i] += habentia[l_i];
            }
            printf("\n");
        }
    }

    si (modus_machina)
    {
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("SUMMA\t%s\t%u\t%u\n", linguae.codices[l_i],
                   summa_habentia[l_i], summa_omnia);
        }
    }
    alioquin
    {
        printf("%-24s", "SUMMA");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            character fractio[XXXII];

            sprintf(fractio, "%u/%u", summa_habentia[l_i],
                    summa_omnia);
            printf("%-10s", fractio);
        }
        printf("\n");
    }

    /* ---- carentia et gemina nominatim (index operum) ---- */

    lacunae = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
        {
            NaturaExemplar* ex;

            ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                                 m);
            per (g_i = ZEPHYRUM;
                 g_i < xar_numerus(bib->genera_omnia); g_i++)
            {
                NaturaGenus* g;
                i32          quot;

                g = *(NaturaGenus**)xar_obtinere(
                        bib->genera_omnia, g_i);
                si (!chorda_aequalis(*g->modulus, *ex->stirps))
                {
                    perge;
                }
                quot = _genus_linguae_quot(g,
                           linguae.codices[l_i]);
                si (quot == ZEPHYRUM)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("CARENS\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("CARENS %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
                alioquin si (quot > (i32)I)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("GEMINA\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("GEMINA %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
            }
        }
    }

    /* via_html: opus V (_paginam_scribere) - hoc loco nondum */
    (vacuum)via_html;

    si (modus_porta && lacunae > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: PORTA - lacunae %u\n", lacunae);
        redde I;
    }
    redde ZEPHYRUM;
}
