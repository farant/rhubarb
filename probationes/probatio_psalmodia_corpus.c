/* probatio_psalmodia_corpus.c - porta corporis 'psalmodia-oraculum'
 * (project-specs/psalmodia-plan-2.md T8; spec V)
 *
 * Corpus GregoBase (bacor v0.4, CC0, tools/gabc_arcessere.sh; in
 * build/, numquam commissum) per PSALMODIA_CORPUS=<directorium x>
 * (plagulae in x/gabc/NNNNN.gabc). Absens: sectio quaeque OMISSA
 * dicit et transit.
 *
 * SECTIONES:
 *   syllabae - verbum quodque corporis (syllabis fontis, acuto fontis)
 *     contra syllabae_latinae: numerus syllabarum et syllaba accentus
 *     (T1 decretum 4: stilus divisionis per exempla AR, non hic).
 *     Discordiae per verbum distinctum (numerus occurrentiarum).
 *   euouae - antiphona (office-part an) cum "E u o u a e" scripto et
 *     titulo resolubili (mode + _gregobase_mode_var): terminatio nostra
 *     super "sæculórum. Amen" contra notas scriptas, gradibus a do
 *     (clavis resoluta). Differentia constans = transposita (seorsum
 *     numerata, non discordia).
 *   psalmi - psalmi scripti (office-part ps) quorum titulus resolvitur
 *     (caput, annotation, nomen): textus fontis (sine notis et stilis,
 *     accentibus servatis) aptatur, notae et stili per versum
 *     comparantur, tenore utriusque ad tenorem nostrum translato.
 *
 * DISCORDIAE: quaeque sectio build/psalmodia/discordiae_<genus>.tsv
 * scribit (ordinatae; columnae: genus, id, titulus, nostra, eorum,
 * numerus) et cum manifesto commisso probationes/fixa/psalmodia/
 * discordiae.tsv (lineae eius generis) confert: linea nova = FRACTA
 * (discordia nova), linea manifesti absens = FRACTA (soluta: manifestum
 * minuendum). Tabula numquam ad corpus flectitur (decretum Frani 5).
 *
 * Notae ut litterae clavis c4 (gradus 0 = do = 'j'); syllabae spatio,
 * notae syllabae iunctae; " :" divisio maior, " ," minor; "<b>" "<i>"
 * stilus syllabae (stili soli).
 */

#include "latina.h"
#include "credo.h"
#include "psalmodia.h"
#include "psalmodia_tabula.h"
#include "syllabae_latinae.h"
#include "gabc.h"
#include "filum.h"
#include "lectiones.h"
#include "iter_directoria.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_TABULAE       "musica/toni_psalmorum.stml"
#define VIA_MANIFESTI     "probationes/fixa/psalmodia/discordiae.tsv"
#define DIRECTORIUM_ACTA  "build/psalmodia"
#define PLAGULAE_MAXIMAE  (X * M)
#define MONSTRANDAE       XXV


/* ==================================================
 * Corpus
 * ================================================== */

nomen structura {
      constans character* radix;     /* PSALMODIA_CORPUS */
                  chorda* tituli;   /* NNNNN.gabc, ordinati */
                     i32  numerus;
} Corpus;

interior integer
_titulos_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (integer)chorda_comparare(*(constans chorda*)a,
        *(constans chorda*)b);
}

/* VERUM si linea quae ad octetum finis desinit '#' incipit */
interior b32
_commentarium (
    chorda m,
       i32 finis)
{
    i32 initium = finis;

    dum (initium > ZEPHYRUM && m.datum[initium - I] != '\n')
    {
        initium--;
    }
    redde initium < finis && m.datum[initium] == '#';
}

/* data commissa, etiam sine corpore (porta radix): tabula sine
 * vitio, manifestum (si adest) sex columnis per lineam */
interior vacuum
_data_commissa (
    CredoContextus* ctx)
{
    PsalmodiaTabula* t = psalmodia_tabulam_legere(filum_legere_totum(
                             VIA_TABULAE, ctx->piscina), ctx->piscina);
             chorda m = filum_legere_totum(VIA_MANIFESTI,
                 ctx->piscina);
                i32 tabulae  = ZEPHYRUM;
                i32 malae    = ZEPHYRUM;
                i32 i;

    CREDO_VERUM (t != NIHIL && psalmodia_successus(t));
    per (i = ZEPHYRUM; i < m.mensura; i++)
    {
        si (m.datum[i] == '\t')
        {
            tabulae++;
        }
        alioquin si (m.datum[i] == '\n')
        {
            /* linea commentarii ('#') columnas non habet */
            si (tabulae != V && !_commentarium(m, i))
            {
                malae++;
            }
            tabulae = ZEPHYRUM;
        }
    }
    CREDO_AEQUALIS_I32 (malae, ZEPHYRUM);
}

/* FALSUM si corpus abest (OMISSA iam dictum) */
interior b32
_corpus_aperire (
    CredoContextus* ctx,
     constans character* sectio,
                 Corpus* corpus)
{
                 character  via[MXXIV];
       DirectoriumIterator* iter;
      DirectoriumIntroitus* e;

    _data_commissa(ctx);
    corpus->radix    = lectiones_ambitus("PSALMODIA_CORPUS");
    corpus->numerus  = ZEPHYRUM;
    si (corpus->radix == NIHIL || strlen(corpus->radix) > CM)
    {
        imprimere("PSALMODIA CORPUS: OMISSA (%s): PSALMODIA_CORPUS non "
            "datum\n", sectio);
        redde FALSUM;
    }
    sprintf(via, "%s/gabc", corpus->radix);
    iter = directorium_iterator_aperire(via, ctx->piscina);
    si (iter == NIHIL)
    {
        imprimere("PSALMODIA CORPUS: OMISSA (%s): %s abest\n", sectio,
            via);
        redde FALSUM;
    }
    corpus->tituli = (chorda*)piscina_allocare(ctx->piscina,
        (memoriae_index)(magnitudo(chorda) * PLAGULAE_MAXIMAE));
    dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
    {
        si (   e->genus == INTROITUS_FILUM
            && chorda_terminatur(e->titulus, chorda_ex_literis(".gabc",
                   ctx->piscina))
            && corpus->numerus < PLAGULAE_MAXIMAE)
        {
            corpus->tituli[corpus->numerus++] =
                chorda_transcribere(e->titulus, ctx->piscina);
        }
    }
    directorium_iterator_claudere(iter);
    qsort(corpus->tituli, (size_t)corpus->numerus, magnitudo(chorda),
        _titulos_comparare);
    redde corpus->numerus > ZEPHYRUM;
}

/* plagula k in piscina q; NIHIL si legi non potest */
interior GabcCantus*
_cantum_legere (
    constans Corpus* corpus,
                i32  k,
            Piscina* q)
{
    character via[MXXIV];

    sprintf(via, "%s/gabc/%.*s", corpus->radix,
        (integer)corpus->tituli[k].mensura,
        (constans character*)corpus->tituli[k].datum);
    redde gabc_legere(filum_legere_totum(via, q), q);
}

/* FALSUM si GregoBase linguam aliam signat (tag "Lingua Polonica";
 * "Lingua Latina" aut nullus = Latinus) */
interior b32
_latinus (
    constans GabcCantus* g)
{
    i32 i;

    per (i = ZEPHYRUM; i < g->numerus_capitum; i++)
    {
        chorda clavis  = g->capita[i].clavis;
        chorda valor   = g->capita[i].valor;

        si (   clavis.mensura > XV
            && memcmp(clavis.datum, "_gregobase_tag_", XV) == ZEPHYRUM
            && valor.mensura > VII
            && memcmp(valor.datum, "Lingua ", VII)         == ZEPHYRUM
            && !(valor.mensura >= XIII
                 && memcmp(valor.datum, "Lingua Latina", XIII)
                    == ZEPHYRUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* id = titulus sine ".gabc" */
interior chorda
_id (
    constans Corpus* corpus,
                i32  k)
{
    chorda t = corpus->tituli[k];

    redde chorda_sectio(t, ZEPHYRUM, t.mensura - V);
}

interior chorda
_caput (
    constans GabcCantus* g,
     constans character* clavis)
{
    chorda vacua;
       i32 i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < g->numerus_capitum; i++)
    {
        si (chorda_aequalis_literis(g->capita[i].clavis, clavis))
        {
            redde g->capita[i].valor;
        }
    }
    redde vacua;
}

interior PsalmodiaTabula*
_tabula (
    CredoContextus* ctx)
{
    redde psalmodia_tabulam_legere(filum_legere_totum(VIA_TABULAE,
        ctx->piscina), ctx->piscina);
}


/* ==================================================
 * Discordiae
 * ================================================== */

nomen structura {
    chorda linea;      /* genus, id, titulus, nostra, eorum */
    chorda id;
       i32 numerus;
} Discordia;

nomen structura {
               Piscina* piscina;
        TabulaDispersa* tabula;  /* clavis -> Discordia */
    constans character* genus;  /* syllabae, euouae, psalmus */
} Discordiae;

interior vacuum
_discordias_creare (
        CredoContextus* ctx,
    constans character* genus,
            Discordiae* d)
{
    d->piscina  = ctx->piscina;
    d->tabula   = tabula_dispersa_creare_chorda(ctx->piscina, MXXIV);
    d->genus    = genus;
}

/* aggregare: clavis sine id (verba distincta), id prima servatur */
interior vacuum
_discordiam_addere (
            Discordiae* d,
    constans character* genus,
                chorda  id,
                chorda  titulus,
                chorda  nostra,
                chorda  eorum,
                   b32  aggregare)
{
     ChordaAedificator* a;
                chorda  clavis;
             Discordia* res;
                vacuum* valor;

    a = chorda_aedificator_creare(d->piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, genus);
    chorda_aedificator_appendere_character(a, '\t');
    si (!aggregare)
    {
        chorda_aedificator_appendere_chorda(a, id);
    }
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_chorda(a, titulus);
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_chorda(a, nostra);
    chorda_aedificator_appendere_character(a, '\t');
    chorda_aedificator_appendere_chorda(a, eorum);
    clavis = chorda_aedificator_finire(a);
    si (tabula_dispersa_invenire(d->tabula, clavis, &valor))
    {
        res = (Discordia*)valor;
        res->numerus++;
        redde;
    }
    res = (Discordia*)piscina_allocare(d->piscina,
        (memoriae_index)magnitudo(Discordia));
    res->linea    = clavis;
    res->id       = chorda_transcribere(id, d->piscina);
    res->numerus  = I;
    (vacuum)tabula_dispersa_inserere(d->tabula, clavis, res);
}

interior integer
_lineas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde (integer)chorda_comparare(*(constans chorda*)a,
        *(constans chorda*)b);
}

/* linea plena: id in columnam secundam, numerus in sextam */
interior chorda
_lineam_plenam (
               Piscina* piscina,
    constans Discordia* res)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, CCLVI);
                  i32  tab;
            character  numerus[XXIV];

    per (tab = ZEPHYRUM; tab < res->linea.mensura
        && res->linea.datum[tab] != '\t'; tab++)
    {
    }
    chorda_aedificator_appendere_chorda(a, chorda_sectio(res->linea,
        ZEPHYRUM, tab + I));
    chorda_aedificator_appendere_chorda(a, res->id);
    /* clavis aggregata id vacuum habet ("genus\t\t..."); non
     * aggregata id suum, quod hic omittitur */
    si (   tab + I < res->linea.mensura
        && res->linea.datum[tab + I] == '\t')
    {
        chorda_aedificator_appendere_chorda(a, chorda_sectio(res->linea,
            tab + I, res->linea.mensura));
    }
    alioquin
    {
        i32 secunda;

        per (secunda = tab + I; secunda < res->linea.mensura
            && res->linea.datum[secunda] != '\t'; secunda++)
        {
        }
        chorda_aedificator_appendere_chorda(a, chorda_sectio(res->linea,
            secunda, res->linea.mensura));
    }
    sprintf(numerus, "\t%u", (insignatus integer)res->numerus);
    chorda_aedificator_appendere_literis(a, numerus);
    redde chorda_aedificator_finire(a);
}

/* scribit build/psalmodia/discordiae_<genus>.tsv et cum manifesto
 * confert; reddit numerum discordiarum */
interior i32
_discordias_conferre (
    CredoContextus* ctx,
        Discordiae* d)
{
            TabulaIterator it  = tabula_dispersa_iterator_initium(
                                      d->tabula);
                    chorda* lineae;
                    chorda  clavis;
                    vacuum* valor;
         ChordaAedificator* a;
            TabulaDispersa* nostrae;
                    chorda  manifestum;
                 character  via[CCLVI];
                 character  praefixum[XXXII];
                       i32  n        = ZEPHYRUM;
                       i32  novae    = ZEPHYRUM;
                       i32  solutae  = ZEPHYRUM;
                       i32  i;
                       i32  initium;

    lineae = (chorda*)piscina_allocare(ctx->piscina,
        (memoriae_index)(magnitudo(chorda)
        * (size_t)(tabula_dispersa_numerus(d->tabula) + I)));
    dum (tabula_dispersa_iterator_proximum(&it, &clavis, &valor))
    {
        lineae[n++] = _lineam_plenam(ctx->piscina, (Discordia*)valor);
    }
    qsort(lineae, (size_t)n, magnitudo(chorda), _lineas_comparare);
    a        = chorda_aedificator_creare(ctx->piscina, MXXIV * XVI);
    nostrae  = tabula_dispersa_creare_chorda(ctx->piscina, MXXIV);
    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda_aedificator_appendere_chorda(a, lineae[i]);
        chorda_aedificator_appendere_character(a, '\n');
        (vacuum)tabula_dispersa_inserere(nostrae, lineae[i], lineae);
    }
    (vacuum)filum_directorium_creare_si_necesse(DIRECTORIUM_ACTA);
    sprintf(via, "%s/discordiae_%s.tsv", DIRECTORIUM_ACTA, d->genus);
    CREDO_VERUM (filum_scribere(via, chorda_aedificator_finire(a)));

    manifestum = filum_legere_totum(VIA_MANIFESTI, ctx->piscina);
    si (manifestum.mensura == ZEPHYRUM)
    {
        imprimere("  manifestum abest (%s): %u discordiae in %s\n",
            VIA_MANIFESTI, (insignatus integer)n, via);
        CREDO_VERUM (FALSUM);
        redde n;
    }
    sprintf(praefixum, "%s", d->genus);
    /* lineae manifesti huius generis: absentes in nostris = solutae */
    {
        TabulaDispersa* manifesti =
            tabula_dispersa_creare_chorda(ctx->piscina, MXXIV);

        initium = ZEPHYRUM;
        per (i = ZEPHYRUM; i <= manifestum.mensura; i++)
        {
            chorda linea;

            si (i < manifestum.mensura && manifestum.datum[i] != '\n')
            {
                perge;
            }
            linea    = chorda_sectio(manifestum, initium, i);
            initium  = i + I;
            si (   linea.mensura         == ZEPHYRUM
                || linea.datum[ZEPHYRUM] == '#'
                || !chorda_incipit(linea, chorda_ex_literis(praefixum,
                       ctx->piscina)))
            {
                perge;
            }
            (vacuum)tabula_dispersa_inserere(manifesti, linea, lineae);
            si (!tabula_dispersa_continet(nostrae, linea))
            {
                si (solutae++ < MONSTRANDAE)
                {
                    imprimere("  SOLUTA (manifestum minuendum): %.*s\n",
                        (integer)linea.mensura,
                        (constans character*)linea.datum);
                }
            }
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (!tabula_dispersa_continet(manifesti, lineae[i]))
            {
                si (novae++ < MONSTRANDAE)
                {
                    imprimere("  NOVA: %.*s\n",
                        (integer)lineae[i].mensura,
                        (constans character*)lineae[i].datum);
                }
            }
        }
    }
    imprimere("  %s: discordiae %u, novae %u, solutae %u (%s)\n",
        d->genus, (insignatus integer)n, (insignatus integer)novae,
        (insignatus integer)solutae, via);
    CREDO_AEQUALIS_I32 (novae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (solutae, ZEPHYRUM);
    redde n;
}


/* ==================================================
 * Syllabae
 * ================================================== */

/* acutus in octetis (praecompositus aut U+0301), ut
 * probatio_syllabae_latinae */
interior b32
_acutum_habet (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i + I < s.mensura; i++)
    {
        insignatus character p = (insignatus character)s.datum[i];
        insignatus character q = (insignatus character)s.datum[i + I];

        si (   (p == 0xC3 && (q == 0xA1 || q == 0xA9 || q == 0xAD
                    || q == 0xB3 || q == 0xBA || q == 0xBD || q == 0x81
                    || q == 0x89 || q == 0x8D || q == 0x93 || q == 0x9A
                    || q == 0x9D))
            || (p == 0xC7 && (q == 0xBD || q == 0xBC))
            || (p == 0xCC && q == 0x81))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus cantatus syllabae iunctus (vacuus si nullus) */
interior chorda
_textus_syllabae (
    constans GabcSyllaba* sy,
                 Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);
                  i32  k;

    per (k = ZEPHYRUM; k < sy->numerus_textuum; k++)
    {
        si (sy->textus[k].genus == GABC_TEXTUS_CANTATUS)
        {
            chorda_aedificator_appendere_chorda(a,
                sy->textus[k].textus);
        }
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_neumam_habet (
    constans GabcSyllaba* sy)
{
    i32 k;

    per (k = ZEPHYRUM; k < sy->numerus_elementorum; k++)
    {
        si (sy->elementa[k].genus == GABC_EL_NEUMA)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* interpunctio ASCII et « » in extremis praeciditur */
interior chorda
_praecidere (
    chorda s)
{
    dum (s.mensura > ZEPHYRUM)
    {
        insignatus character primus =
            (insignatus character)s.datum[ZEPHYRUM];

        si (   primus < 0x80
            && !((primus | 0x20) >= 'a' && (primus | 0x20) <= 'z'))
        {
            s = chorda_sectio(s, I, s.mensura);
        }
        alioquin si (   s.mensura >= II && primus == 0xC2
                     && ((insignatus character)s.datum[I] == 0xAB
                         || (insignatus character)s.datum[I] == 0xBB))
        {
            s = chorda_sectio(s, II, s.mensura);
        }
        alioquin
        {
            frange;
        }
    }
    dum (s.mensura > ZEPHYRUM)
    {
        insignatus character postremus =
            (insignatus character)s.datum[s.mensura - I];

        si (   postremus < 0x80
            && !((postremus | 0x20) >= 'a'
            && (postremus | 0x20) <= 'z'))
        {
            s.mensura--;
        }
        alioquin si (   s.mensura >= II
                     && (insignatus character)s.datum[s.mensura - II]
                        == 0xC2
                     && (postremus == 0xAB || postremus == 0xBB))
        {
            s.mensura -= II;
        }
        alioquin
        {
            frange;
        }
    }
    redde s;
}

interior chorda
_cum_accentu (
                Piscina* p,
                 chorda  divisio,
                    s32  accentus)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);
            character  n[XVI];

    chorda_aedificator_appendere_chorda(a, divisio);
    si (accentus >= ZEPHYRUM)
    {
        sprintf(n, " @%d", (integer)(accentus + I));
    }
    alioquin
    {
        sprintf(n, " @-");
    }
    chorda_aedificator_appendere_literis(a, n);
    redde chorda_aedificator_finire(a);
}

nomen structura {
    Discordiae discordiae;
           i32 verba;
           i32 numeri;
           i32 accentus;
} SyllabaeCensus;

#define SYLLABAE_VERBI_MAXIMAE  LXIV

/* verbum fontis ex syllabis indices[0..n) contra nostrum; omittitur
 * (ut in specimine T2) syllaba sine vocali, sine notis, spatium aut
 * signum intra, elisio */
interior vacuum
_verbum_iudicare (
     constans GabcCantus* g,
            constans i32* indices,
                     i32  n,
                  chorda  id,
                 Piscina* p,
         SyllabaeCensus* census)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);
     ChordaAedificator* b = chorda_aedificator_creare(p, LXIV);
                chorda  verbum;
                chorda  nostra;
                   s32  acc_fons = -I;
         VerbumLatinum  divisum;
                   i32  s;
                   i32  k;
                   i32  j;

    si (n == ZEPHYRUM)
    {
        redde;
    }
    per (s = ZEPHYRUM; s < n; s++)
    {
        constans GabcSyllaba* sy       = &g->syllabae[indices[s]];
                         b32  vocalis  = FALSUM;

        si (sy->numerus_elementorum == ZEPHYRUM)
        {
            redde;
        }
        si (s > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(b, '-');
        }
        per (k = ZEPHYRUM; k < sy->numerus_textuum; k++)
        {
            chorda t;

            si (sy->textus[k].genus != GABC_TEXTUS_CANTATUS)
            {
                perge;
            }
            si (sy->textus[k].stilus & GABC_STILUS_ELISIO)
            {
                redde;
            }
            t = sy->textus[k].textus;
            per (j = ZEPHYRUM; j < t.mensura; j++)
            {
                insignatus character ch =
                    (insignatus character)t.datum[j];

                si (ch == ' ' || ch == '*' || ch == '+' || ch == '-')
                {
                    redde;
                }
                si (syllabae_latinae_vocalis(t, j, NIHIL))
                {
                    vocalis = VERUM;
                }
            }
            si (_acutum_habet(t))
            {
                acc_fons = (s32)s;
            }
            chorda_aedificator_appendere_chorda(a, t);
            chorda_aedificator_appendere_chorda(b, t);
        }
        si (!vocalis)
        {
            redde;
        }
    }
    verbum = chorda_aedificator_finire(a);
    /* signa repetitionis (ij, iij) verba non sunt */
    {
        chorda nudum   = _praecidere(verbum);
           b32 signum  = nudum.mensura > ZEPHYRUM;

        per (k = ZEPHYRUM; k < nudum.mensura; k++)
        {
            si (   (nudum.datum[k] | 0x20) != 'i'
                && (nudum.datum[k] | 0x20) != 'j')
            {
                signum = FALSUM;
            }
        }
        si (signum)
        {
            redde;
        }
    }
    CREDO_NECESSE (syllabae_latinae_dividere(verbum, p, &divisum));
    census->verba++;
    si (   divisum.numerus == n
        && (acc_fons < ZEPHYRUM || divisum.accentus == acc_fons))
    {
        redde;
    }
    a = chorda_aedificator_creare(p, LXIV);
    per (s = ZEPHYRUM; s < divisum.numerus; s++)
    {
        si (s > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '-');
        }
        chorda_aedificator_appendere_chorda(a, chorda_sectio(verbum,
            divisum.syllabae[s].initium, divisum.syllabae[s].finis));
    }
    nostra = _cum_accentu(p, _praecidere(chorda_aedificator_finire(a)),
        divisum.accentus);
    si (divisum.numerus != n)
    {
        census->numeri++;
    }
    alioquin
    {
        census->accentus++;
    }
    _discordiam_addere(&census->discordiae,
        divisum.numerus != n ? "syllabae:numerus" : "syllabae:accentus",
        id,
        _praecidere(verbum), nostra,
        _cum_accentu(p, _praecidere(chorda_aedificator_finire(b)),
            acc_fons),
        VERUM);
}

interior vacuum
_syllabae_corporis (
    CredoContextus* ctx)
{
            Corpus corpus;
    SyllabaeCensus census;
               i32 k;
               i32 lecti    = ZEPHYRUM;
               i32 alienae  = ZEPHYRUM;

    si (!_corpus_aperire(ctx, "syllabae", &corpus))
    {
        redde;
    }
    memset(&census, ZEPHYRUM, magnitudo(census));
    _discordias_creare(ctx, "syllabae", &census.discordiae);
    per (k = ZEPHYRUM; k < corpus.numerus; k++)
    {
        Piscina* q = piscina_generare_dynamicum("psalmodia_corpus",
                         (memoriae_index)(CCLVI * MXXIV));
        GabcCantus* g;
               i32  indices[SYLLABAE_VERBI_MAXIMAE];
               i32  n = ZEPHYRUM;
               i32  s;

        CREDO_NECESSE (q != NIHIL);
        g = _cantum_legere(&corpus, k, q);
        si (g != NIHIL)
        {
            lecti++;
        }
        si (g != NIHIL && !_latinus(g))
        {
            alienae++;
        }
        alioquin si (g != NIHIL)
        {
            per (s = ZEPHYRUM; s < g->numerus_syllabarum; s++)
            {
                si (_textus_syllabae(&g->syllabae[s], q).mensura
                    == ZEPHYRUM)
                {
                    perge;
                }
                si (   g->syllabae[s].verbum_incipit
                    || n == SYLLABAE_VERBI_MAXIMAE)
                {
                    _verbum_iudicare(g, indices, n, _id(&corpus, k), q,
                        &census);
                    n = ZEPHYRUM;
                }
                indices[n++] = s;
            }
            _verbum_iudicare(g, indices, n, _id(&corpus, k), q,
                &census);
        }
        piscina_destruere(q);
    }
    imprimere("  syllabae: plagulae %u (lectae %u, linguae alienae "
        "%u), verba %u, numerus alius %u, accentus alius %u\n",
        (insignatus integer)corpus.numerus, (insignatus integer)lecti,
        (insignatus integer)alienae,
        (insignatus integer)census.verba,
        (insignatus integer)census.numeri,
        (insignatus integer)census.accentus);
    CREDO_AEQUALIS_I32 (lecti, corpus.numerus);
    (vacuum)_discordias_conferre(ctx, &census.discordiae);
}


/* ==================================================
 * Notae ut litterae
 * ================================================== */

/* gradus (a do) + translatio -> littera clavis c4 (ultra 'm'
 * litterae sequentes, non gabc); '?' extra a..z */
interior character
_littera (
    s32 gradus)
{
    s32 locus = gradus + IX;

    redde (locus >= ZEPHYRUM && locus <= XXV)
        ? (character)('a' + locus) : '?';
}

/* gradus frequentissimus (tenor) cantus */
interior s32
_tenor (
    constans GabcCantus* g)
{
    i32 numeri[LXIV];
    i32 i;
    i32 optimus = ZEPHYRUM;

    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    per (i = ZEPHYRUM; i < g->numerus_notarum; i++)
    {
        s32 locus = g->notae[i].gradus + XXXII;

        si (locus >= ZEPHYRUM && locus < LXIV)
        {
            numeri[locus]++;
        }
    }
    per (i = I; i < LXIV; i++)
    {
        si (numeri[i] > numeri[optimus])
        {
            optimus = i;
        }
    }
    redde (s32)optimus - XXXII;
}

/* notae syllabae (neumae omnes) ut litterae, translatae */
interior vacuum
_notas_appendere (
     constans GabcCantus* g,
    constans GabcSyllaba* sy,
                     s32  translatio,
       ChordaAedificator* a)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < sy->numerus_elementorum; k++)
    {
        constans GabcElementum* el = &sy->elementa[k];

        si (el->genus != GABC_EL_NEUMA)
        {
            perge;
        }
        per (j = el->prima; j < el->prima + el->numerus
            && j < g->numerus_notarum; j++)
        {
            chorda_aedificator_appendere_character(a,
                _littera(g->notae[j].gradus + translatio));
        }
    }
}


/* ==================================================
 * Signa (tituli toni)
 * ================================================== */

/* modus + differentia ut GregoBase scribit -> signum: "p" =
 * peregrinus; "-star" = '*'; modus vacuus = differentia sola */
interior chorda
_signum_formare (
          Piscina* p,
           chorda  modus,
           chorda  differentia)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);
                  i32  i;

    modus        = chorda_praecidere(modus);
    differentia  = chorda_praecidere(differentia);
    si (chorda_aequalis_literis(modus, "p"))
    {
        redde chorda_ex_literis("peregrinus", p);
    }
    chorda_aedificator_appendere_chorda(a, modus);
    si (modus.mensura > ZEPHYRUM && differentia.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, ' ');
    }
    per (i = ZEPHYRUM; i < differentia.mensura; i++)
    {
        si (   i + V <= differentia.mensura
            && chorda_aequalis_case_insensitivus(chorda_sectio(
                   differentia, i, i + V), chorda_ex_literis("-star",
                   p)))
        {
            chorda_aedificator_appendere_character(a, '*');
            i += IV;
            perge;
        }
        chorda_aedificator_appendere_character(a,
            (character)differentia.datum[i]);
    }
    redde chorda_aedificator_finire(a);
}

/* titulus resolutus: "8 G" (modus tabulae + differentia) */
interior chorda
_signum_resolutum (
                        Piscina* p,
        constans PsalmodiaTonus* tonus,
     constans PsalmodiaClausula* terminatio)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, XXXII);

    chorda_aedificator_appendere_chorda(a, tonus->modus);
    si (terminatio->differentia.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, terminatio->differentia);
    }
    si (tonus->positio_altera)
    {
        chorda_aedificator_appendere_literis(a, " (altera)");
    }
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * EUOUAE
 * ================================================== */

/* littera syllabae sine accentu, minuscula; 0 si non littera
 * (octetus unus aut UTF-8 vocalis acuta C3 xx) */
interior i32
_litteras_euouae (
       chorda  t,
    character* exitus,
          i32  capacitas)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < t.mensura && n < capacitas; i++)
    {
        insignatus character ch = (insignatus character)t.datum[i];

        si ((ch | 0x20) >= 'a' && (ch | 0x20) <= 'z')
        {
            exitus[n++] = (character)(ch | 0x20);
        }
        alioquin si (ch == 0xC3 && i + I < t.mensura)
        {
            insignatus character q = (insignatus character)t.datum[++i];

            q = (insignatus character)(q | 0x20);
            exitus[n++] = (character)(q == 0xA1 ? 'a' : q == 0xA9 ? 'e'
                        : q == 0xB3 ? 'o' : q == 0xBA ? 'u' : '?');
        }
    }
    redde n;
}

/* syllabae [*prima, *ultima] = "E u o u a e"; FALSUM si nullae */
interior b32
_euouae_invenire (
    constans GabcCantus* g,
                Piscina* q,
                    i32* prima,
                    i32* ultima)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < g->numerus_syllabarum; i++)
    {
        character acc[XVI];
              i32 n = ZEPHYRUM;

        si (_litteras_euouae(_textus_syllabae(&g->syllabae[i], q), acc,
            XVI) == ZEPHYRUM)
        {
            perge;
        }
        per (j = i; j < g->numerus_syllabarum && n < VI; j++)
        {
            n += _litteras_euouae(_textus_syllabae(&g->syllabae[j], q),
                acc + n, XVI - n);
            si (n > VI || memcmp(acc, "euouae", (size_t)n) != ZEPHYRUM)
            {
                frange;
            }
        }
        si (n == VI && memcmp(acc, "euouae", VI) == ZEPHYRUM)
        {
            *prima   = i;
            *ultima  = j - I;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* gradus syllabarum cum neumis in [prima, ultima]: "jj i hg" et
 * numerus syllabarum; gradus in *gradus (ad translationem) */
interior chorda
_gradus_scribere (
    constans GabcCantus* g,
                    i32  prima,
                    i32  ultima,
                Piscina* q,
                    s32* gradus,
                    i32* numerus,
                    i32* syllabae)
{
    ChordaAedificator* a = chorda_aedificator_creare(q, LXIV);
                  i32  s;
                  i32  k;
                  i32  j;

    *numerus   = ZEPHYRUM;
    *syllabae  = ZEPHYRUM;
    per (s = prima; s <= ultima && s < g->numerus_syllabarum; s++)
    {
        constans GabcSyllaba* sy = &g->syllabae[s];

        si (!_neumam_habet(sy))
        {
            perge;
        }
        si (*syllabae > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, ' ');
        }
        (*syllabae)++;
        _notas_appendere(g, sy, ZEPHYRUM, a);
        per (k = ZEPHYRUM; k < sy->numerus_elementorum; k++)
        {
            constans GabcElementum* el = &sy->elementa[k];

            si (el->genus != GABC_EL_NEUMA)
            {
                perge;
            }
            per (j = el->prima; j < el->prima + el->numerus
                && j < g->numerus_notarum && *numerus < XXXII; j++)
            {
                gradus[(*numerus)++] = g->notae[j].gradus;
            }
        }
    }
    redde chorda_aedificator_finire(a);
}

nomen structura {
    i32 concordes;
    i32 vicariae;       /* differentia vicaria tabulae (A* -> A) */
    i32 transpositae;
    i32 discordes;
} Suffragia;

nomen structura {
        Discordiae  discordiae;
    TabulaDispersa* tituli;   /* signum resolutum -> Suffragia */
               i32  antiphonae;
               i32  scriptae;
               i32  sine_titulo;
               i32  irresolutae;
               i32  comparatae;
} EuouaeCensus;

/* terminatio super "sæculórum. Amen" -> cantus gabc relectus */
interior GabcCantus*
_euouae_nostrum (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
                       Piscina* q)
{
     PsalmodiaPsalmus* ps;
    ChordaAedificator* a = chorda_aedificator_creare(q, CCLVI);

    ps = psalmodia_partem_aptare(tonus, terminatio,
        PSALMODIA_FINIS_TERMINATIO, FALSUM,
        chorda_ex_literis("s\xC3\xA6" "cul\xC3\xB3rum. Amen", q), q);
    si (ps == NIHIL || !psalmodia_gabc_scribere(ps, NIHIL, a))
    {
        redde NIHIL;
    }
    redde gabc_legere(chorda_aedificator_finire(a), q);
}

interior vacuum
_euouae_cantus (
    CredoContextus* ctx,
    constans PsalmodiaTabula* tabula,
       constans GabcCantus* g,
                    chorda  id,
                  Piscina* q,
             EuouaeCensus* census)
{
       constans PsalmodiaTonus* tonus = NIHIL;
    constans PsalmodiaClausula* terminatio;
                    GabcCantus* noster;
                        chorda  differentia;
                        chorda  signum;
                        chorda  resolutum;
                        chorda  titulus;
                        chorda  nostra;
                        chorda  eorum;
                           s32  gradus_n[XXXII];
                           s32  gradus_e[XXXII];
                           i32  numerus_n;
                           i32  numerus_e;
                           i32  syllabae_n;
                           i32  syllabae_e;
                           i32  prima;
                           i32  ultima;
                           i32  i;
                     Suffragia* suff;
                        vacuum* valor;
                           b32  aequales;
                           b32  translata;
                           b32  vicaria;

    si (!chorda_aequalis_literis(_caput(g, "office-part"), "an"))
    {
        redde;
    }
    census->antiphonae++;
    si (!_euouae_invenire(g, q, &prima, &ultima))
    {
        redde;
    }
    census->scriptae++;
    differentia = _caput(g, "_gregobase_mode_var");
    si (chorda_praecidere(differentia).mensura == ZEPHYRUM)
    {
        census->sine_titulo++;
        redde;
    }
    signum = _signum_formare(q, g->modus, differentia);
    terminatio = psalmodia_signum_invenire(tabula, signum, &tonus,
        NIHIL);
    si (terminatio == NIHIL)
    {
        census->irresolutae++;
        redde;
    }
    census->comparatae++;
    noster = _euouae_nostrum(tonus, terminatio, q);
    CREDO_NECESSE (noster != NIHIL);
    nostra = _gradus_scribere(noster, ZEPHYRUM,
        noster->numerus_syllabarum - I, q, gradus_n, &numerus_n,
        &syllabae_n);
    eorum = _gradus_scribere(g, prima, ultima, q, gradus_e, &numerus_e,
        &syllabae_e);
    aequales   = chorda_aequalis(nostra, eorum);
    translata  = FALSUM;
    vicaria    = FALSUM;
    /* terminatio cum vicaria (A* vicaria A, p. 13*: "potest ad libitum
     * adhiberi"): antiphona sic notata vicariam quoque canere potest */
    si (!aequales && terminatio->vicaria.mensura > ZEPHYRUM)
    {
        constans PsalmodiaClausula* altera;
                        GabcCantus* alter;

        altera = psalmodia_terminationem_invenire(tabula, tonus->modus,
            terminatio->vicaria, NIHIL, NIHIL);
        alter = altera != NIHIL ? _euouae_nostrum(tonus, altera, q)
                                : NIHIL;
        si (alter != NIHIL)
        {
            s32 gradus_v[XXXII];
            i32 numerus_v;
            i32 syllabae_v;

            vicaria = chorda_aequalis(_gradus_scribere(alter, ZEPHYRUM,
                alter->numerus_syllabarum - I, q, gradus_v, &numerus_v,
                &syllabae_v), eorum);
        }
    }
    si (   !aequales && numerus_n == numerus_e
        && syllabae_n         == syllabae_e
        && numerus_n > ZEPHYRUM
        && gradus_n[ZEPHYRUM] != gradus_e[ZEPHYRUM])
    {
        /* forma eadem (spatia), differentia constans */
        translata = VERUM;
        per (i = ZEPHYRUM; i < numerus_n; i++)
        {
            si (   gradus_e[i] - gradus_e[ZEPHYRUM]
                != gradus_n[i] - gradus_n[ZEPHYRUM])
            {
                translata = FALSUM;
            }
        }
        per (i = ZEPHYRUM; translata && i < nostra.mensura; i++)
        {
            si ((nostra.datum[i] == ' ') != (eorum.datum[i] == ' '))
            {
                translata = FALSUM;
            }
        }
    }
    resolutum = _signum_resolutum(ctx->piscina, tonus, terminatio);
    si (!tabula_dispersa_invenire(census->tituli, resolutum, &valor))
    {
        suff = (Suffragia*)piscina_allocare(ctx->piscina,
            (memoriae_index)magnitudo(Suffragia));
        memset(suff, ZEPHYRUM, magnitudo(Suffragia));
        (vacuum)tabula_dispersa_inserere(census->tituli, resolutum,
            suff);
    }
    alioquin
    {
        suff = (Suffragia*)valor;
    }
    si (aequales)
    {
        suff->concordes++;
        redde;
    }
    si (vicaria)
    {
        suff->vicariae++;
        redde;
    }
    si (translata)
    {
        suff->transpositae++;
        redde;
    }
    suff->discordes++;
    {
        ChordaAedificator* a = chorda_aedificator_creare(q, LXIV);

        chorda_aedificator_appendere_chorda(a, resolutum);
        chorda_aedificator_appendere_literis(a, " <- ");
        chorda_aedificator_appendere_chorda(a, signum);
        titulus = chorda_aedificator_finire(a);
    }
    _discordiam_addere(&census->discordiae, "euouae", id, titulus,
        nostra, eorum, FALSUM);
}

interior vacuum
_euouae (
    CredoContextus* ctx)
{
             Corpus  corpus;
       EuouaeCensus  census;
    PsalmodiaTabula* tabula;
     TabulaIterator  it;
             chorda  clavis;
             vacuum* valor;
             chorda* tituli;
                i32  n = ZEPHYRUM;
                i32  k;

    si (!_corpus_aperire(ctx, "euouae", &corpus))
    {
        redde;
    }
    tabula = _tabula(ctx);
    CREDO_NECESSE (tabula != NIHIL && psalmodia_successus(tabula));
    memset(&census, ZEPHYRUM, magnitudo(census));
    _discordias_creare(ctx, "euouae", &census.discordiae);
    census.tituli = tabula_dispersa_creare_chorda(ctx->piscina,
        CXXVIII);
    per (k = ZEPHYRUM; k < corpus.numerus; k++)
    {
        Piscina* q = piscina_generare_dynamicum("psalmodia_corpus",
                         (memoriae_index)(CCLVI * MXXIV));
        GabcCantus* g;

        CREDO_NECESSE (q != NIHIL);
        g = _cantum_legere(&corpus, k, q);
        si (g != NIHIL)
        {
            si (_latinus(g))
            {
                _euouae_cantus(ctx, tabula, g, _id(&corpus, k), q,
                    &census);
            }
        }
        piscina_destruere(q);
    }
    imprimere("  euouae: antiphonae %u, cum EUOUAE scripto %u, sine "
        "titulo %u, titulus non resolutus %u, comparatae %u\n",
        (insignatus integer)census.antiphonae,
        (insignatus integer)census.scriptae,
        (insignatus integer)census.sine_titulo,
        (insignatus integer)census.irresolutae,
        (insignatus integer)census.comparatae);
    tituli = (chorda*)piscina_allocare(ctx->piscina,
        (memoriae_index)(magnitudo(chorda)
        * (size_t)(tabula_dispersa_numerus(census.tituli) + I)));
    it = tabula_dispersa_iterator_initium(census.tituli);
    dum (tabula_dispersa_iterator_proximum(&it, &clavis, &valor))
    {
        tituli[n++] = clavis;
    }
    qsort(tituli, (size_t)n, magnitudo(chorda), _lineas_comparare);
    imprimere("  %-16s %9s %8s %12s %9s\n", "terminatio", "concordes",
        "vicariae", "transpositae", "discordes");
    per (k = ZEPHYRUM; k < n; k++)
    {
        Suffragia* suff;

        (vacuum)tabula_dispersa_invenire(census.tituli, tituli[k],
            &valor);
        suff = (Suffragia*)valor;
        imprimere("  %-16.*s %9u %8u %12u %9u\n",
            (integer)tituli[k].mensura,
            (constans character*)tituli[k].datum,
            (insignatus integer)suff->concordes,
            (insignatus integer)suff->vicariae,
            (insignatus integer)suff->transpositae,
            (insignatus integer)suff->discordes);
    }
    CREDO_MAIOR_I32 (census.comparatae, ZEPHYRUM);
    (vacuum)_discordias_conferre(ctx, &census.discordiae);
}


/* ==================================================
 * Psalmi scripti
 * ================================================== */

/* signum ex nomine: post "Mode ", aut in parenthesi ultima post
 * "Ps"/"Ps."/numerum ("Beatus vir (Ps 111 8 G)" -> "8 G") */
interior chorda
_signum_ex_titulo (
    chorda titulus)
{
     chorda vacua;
        s32 i;
        s32 aperta = -I;
        s32 clausa = -I;
        i32 k;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; (i32)i + V <= titulus.mensura; i++)
    {
        si (memcmp(titulus.datum + i, "Mode ", V) == ZEPHYRUM)
        {
            redde chorda_praecidere(chorda_sectio(titulus, (i32)i + V,
                titulus.mensura));
        }
    }
    per (i = (s32)titulus.mensura - I; i >= ZEPHYRUM; i--)
    {
        si (titulus.datum[i] == ')' && clausa < ZEPHYRUM)
        {
            clausa = i;
        }
        si (titulus.datum[i] == '(' && clausa >= ZEPHYRUM)
        {
            aperta = i;
            frange;
        }
    }
    si (aperta < ZEPHYRUM)
    {
        redde vacua;
    }
    k = (i32)aperta + I;
    /* "Ps" aut "Ps." aut "Psalmus", deinde numerus */
    si (   k + II                              <= (i32)clausa
        && memcmp(titulus.datum + k, "Ps", II) == ZEPHYRUM)
    {
        dum (   k < (i32)clausa && titulus.datum[k] != ' '
             && !(titulus.datum[k] >= '0' && titulus.datum[k] <= '9'))
        {
            k++;
        }
        dum (k < (i32)clausa && titulus.datum[k] == ' ')
        {
            k++;
        }
        dum (   k < (i32)clausa && titulus.datum[k] >= '0'
             && titulus.datum[k] <= '9')
        {
            k++;
        }
        dum (k < (i32)clausa && titulus.datum[k] == ' ')
        {
            k++;
        }
        si (   k < (i32)clausa && titulus.datum[k] >= '1'
            && titulus.datum[k] <= '8')
        {
            redde chorda_praecidere(chorda_sectio(titulus, k,
                (i32)clausa));
        }
    }
    redde vacua;
}

/* textus punctatus ex psalmo scripto: syllabae iunctae (spatium ante
 * verbum), linea nova post divisionem finalem, " *" ad divisionem
 * maiorem si versus asteriscum non habet */
interior chorda
_textum_psalmi (
    constans GabcCantus* g,
                Piscina* q)
{
    ChordaAedificator* a = chorda_aedificator_creare(q, MXXIV);
                  b32  linea_vacua = VERUM;
                  b32  asteriscus = FALSUM;
                  i32  s;
                  i32  k;

    per (s = ZEPHYRUM; s < g->numerus_syllabarum; s++)
    {
        constans GabcSyllaba* sy  = &g->syllabae[s];
                      chorda  t   = _textus_syllabae(sy, q);

        si (t.mensura > ZEPHYRUM)
        {
            si (!linea_vacua && sy->verbum_incipit)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            chorda_aedificator_appendere_chorda(a, t);
            linea_vacua = FALSUM;
            si (chorda_continet(t, chorda_ex_literis("*", q)))
            {
                asteriscus = VERUM;
            }
        }
        per (k = ZEPHYRUM; k < sy->numerus_elementorum; k++)
        {
            constans GabcElementum* el = &sy->elementa[k];

            si (el->genus != GABC_EL_DIVISIO)
            {
                perge;
            }
            si (el->divisio == GABC_DIVISIO_FINALIS)
            {
                chorda_aedificator_appendere_character(a, '\n');
                linea_vacua  = VERUM;
                asteriscus   = FALSUM;
            }
            alioquin si (   (   el->divisio == GABC_DIVISIO_MAIOR
                             || el->divisio
                                 == GABC_DIVISIO_MAIOR_PUNCTATA)
                         && !asteriscus && !linea_vacua)
            {
                chorda_aedificator_appendere_literis(a, " *");
                asteriscus = VERUM;
            }
        }
    }
    redde chorda_aedificator_finire(a);
}

/* versus ut chordae: notae (translatae) et, si stili, "<b>"/"<i>" */
interior i32
_versus_scribere (
    constans GabcCantus* g,
                    s32  translatio,
                    b32  stili,
                Piscina* q,
                 chorda* versus,
                    i32  capacitas)
{
    ChordaAedificator* a      = chorda_aedificator_creare(q, CCLVI);
                  i32  n      = ZEPHYRUM;
                  b32  plena  = FALSUM;
                  i32  s;
                  i32  k;

    per (s = ZEPHYRUM; s < g->numerus_syllabarum && n < capacitas; s++)
    {
        constans GabcSyllaba* sy = &g->syllabae[s];

        si (_neumam_habet(sy))
        {
            i32 stilus = ZEPHYRUM;

            per (k = ZEPHYRUM; k < sy->numerus_textuum; k++)
            {
                stilus |= sy->textus[k].stilus;
            }
            si (plena)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            si (stili && (stilus & GABC_STILUS_CRASSUS))
            {
                chorda_aedificator_appendere_literis(a, "<b>");
            }
            alioquin si (stili && (stilus & GABC_STILUS_ITALICUS))
            {
                chorda_aedificator_appendere_literis(a, "<i>");
            }
            _notas_appendere(g, sy, translatio, a);
            plena = VERUM;
        }
        per (k = ZEPHYRUM; k < sy->numerus_elementorum; k++)
        {
            constans GabcElementum* el = &sy->elementa[k];

            si (el->genus != GABC_EL_DIVISIO)
            {
                perge;
            }
            si (el->divisio == GABC_DIVISIO_FINALIS)
            {
                si (plena)
                {
                    versus[n++]  = chorda_aedificator_finire(a);
                    a            = chorda_aedificator_creare(q, CCLVI);
                }
                plena = FALSUM;
            }
            alioquin si (plena)
            {
                chorda_aedificator_appendere_literis(a,
                    (   el->divisio == GABC_DIVISIO_MAIOR
                     || el->divisio == GABC_DIVISIO_MAIOR_PUNCTATA)
                    ? " :" : " ,");
            }
        }
    }
    si (plena && n < capacitas)
    {
        versus[n++] = chorda_aedificator_finire(a);
    }
    redde n;
}

#define VERSUS_MAXIMI  CXXVIII

nomen structura {
    Discordiae discordiae;
           i32 scripti;
           i32 resoluti;
} PsalmiCensus;

/* discordiae versuum (notae, stili) unius aptationis */
interior i32
_psalmum_conferre (
     constans GabcCantus* noster,
     constans GabcCantus* eorum_cantus,
                 Piscina* q,
                     b32  addere,
                  chorda  id,
                  chorda  titulus,
            PsalmiCensus* census)
{
    chorda n_notae[VERSUS_MAXIMI];
    chorda e_notae[VERSUS_MAXIMI];
    chorda n_stili[VERSUS_MAXIMI];
    chorda e_stili[VERSUS_MAXIMI];
       s32 translatio = _tenor(noster) - _tenor(eorum_cantus);
       i32 nn;
       i32 ne;
       i32 v;
       i32 discordes = ZEPHYRUM;
 character numerus[XXXII];

    nn = _versus_scribere(noster, ZEPHYRUM, FALSUM, q, n_notae,
        VERSUS_MAXIMI);
    ne = _versus_scribere(eorum_cantus, translatio, FALSUM, q, e_notae,
        VERSUS_MAXIMI);
    (vacuum)_versus_scribere(noster, ZEPHYRUM, VERUM, q, n_stili,
        VERSUS_MAXIMI);
    (vacuum)_versus_scribere(eorum_cantus, translatio, VERUM, q,
        e_stili, VERSUS_MAXIMI);
    si (nn != ne)
    {
        discordes++;
        si (addere)
        {
            character a[XXXII];

            sprintf(a, "versus %u", (insignatus integer)nn);
            sprintf(numerus, "versus %u", (insignatus integer)ne);
            _discordiam_addere(&census->discordiae, "psalmus:versus",
                id,
                titulus, chorda_ex_literis(a, q),
                chorda_ex_literis(numerus, q), FALSUM);
        }
    }
    per (v = ZEPHYRUM; v < nn && v < ne; v++)
    {
        constans character* genus = NIHIL;

        si (!chorda_aequalis(n_notae[v], e_notae[v]))
        {
            genus = "psalmus:notae";
        }
        alioquin si (!chorda_aequalis(n_stili[v], e_stili[v]))
        {
            genus = "psalmus:stili";
        }
        si (genus == NIHIL)
        {
            perge;
        }
        discordes++;
        si (addere)
        {
            ChordaAedificator* a = chorda_aedificator_creare(q, XXXII);

            chorda_aedificator_appendere_chorda(a, id);
            sprintf(numerus, ":%u", (insignatus integer)(v + I));
            chorda_aedificator_appendere_literis(a, numerus);
            _discordiam_addere(&census->discordiae, genus,
                chorda_aedificator_finire(a), titulus,
                genus[VIII] == 'n' ? n_notae[v] : n_stili[v],
                genus[VIII] == 'n' ? e_notae[v] : e_stili[v], FALSUM);
        }
    }
    redde discordes;
}

interior GabcCantus*
_aptatum (
       constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* terminatio,
                        chorda  textus,
                           b32  canticum,
                       Piscina* q,
                           i32* admonitiones)
{
    PsalmodiaOptiones  optiones;
     PsalmodiaPsalmus* ps;
    ChordaAedificator* a = chorda_aedificator_creare(q, MXXIV);

    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.sine_gloria = VERUM;
    optiones.canticum = canticum;
    ps = psalmodia_aptare(tonus, terminatio, textus, &optiones, q);
    si (ps == NIHIL || !psalmodia_gabc_scribere(ps, &optiones, a))
    {
        redde NIHIL;
    }
    *admonitiones = ps->numerus_admonitionum;
    redde gabc_legere(chorda_aedificator_finire(a), q);
}

interior vacuum
_psalmum_iudicare (
    constans PsalmodiaTabula* tabula,
       constans GabcCantus* g,
                    chorda  id,
                  Piscina* q,
             PsalmiCensus* census)
{
       constans PsalmodiaTonus* tonus       = NIHIL;
    constans PsalmodiaClausula* terminatio  = NIHIL;
                        chorda  signum;
                        chorda  textus;
                    GabcCantus* simplex;
                    GabcCantus* canticum;
                           i32  ds;
                           i32  dc;
                           i32  admonitiones = ZEPHYRUM;
                           i32  k;
             ChordaAedificator* causa = chorda_aedificator_creare(q,
                                     CCLVI);

    si (!chorda_aequalis_literis(_caput(g, "office-part"), "ps"))
    {
        redde;
    }
    census->scripti++;
    /* caput (mode + mode_var), annotation, nomen, mode solum */
    signum = _signum_formare(q, g->modus,
        _caput(g, "_gregobase_mode_var"));
    si (chorda_praecidere(_caput(g, "_gregobase_mode_var")).mensura
        > ZEPHYRUM)
    {
        terminatio = psalmodia_signum_invenire(tabula, signum, &tonus,
            causa);
    }
    per (k = ZEPHYRUM; terminatio == NIHIL
        && k < g->numerus_annotationum; k++)
    {
        signum = g->annotationes[k];
        terminatio = psalmodia_signum_invenire(tabula, signum, &tonus,
            causa);
    }
    si (terminatio == NIHIL)
    {
        signum = _signum_formare(q, chorda_ex_literis("", q),
            _signum_ex_titulo(g->titulus));
        si (signum.mensura > ZEPHYRUM)
        {
            terminatio = psalmodia_signum_invenire(tabula, signum,
                &tonus, causa);
        }
    }
    si (terminatio == NIHIL && g->modus.mensura > ZEPHYRUM)
    {
        signum = _signum_formare(q, g->modus, chorda_ex_literis("", q));
        terminatio = psalmodia_signum_invenire(tabula, signum, &tonus,
            causa);
    }
    si (terminatio == NIHIL)
    {
        chorda c2 = chorda_aedificator_finire(causa);

        imprimere("  psalmus %.*s (%.*s): titulus non resolvitur "
            "(ultimum \"%.*s\": %.*s)\n",
            (integer)id.mensura, (constans character*)id.datum,
            (integer)g->titulus.mensura,
            (constans character*)g->titulus.datum,
            (integer)signum.mensura, (constans character*)signum.datum,
            (integer)c2.mensura, (constans character*)c2.datum);
        redde;
    }
    census->resoluti++;
    textus    = _textum_psalmi(g, q);
    simplex   = _aptatum(tonus, terminatio, textus, FALSUM, q,
        &admonitiones);
    canticum  = _aptatum(tonus, terminatio, textus, VERUM, q,
        &admonitiones);
    CREDO_NECESSE (simplex != NIHIL && canticum != NIHIL);
    ds = _psalmum_conferre(simplex, g, q, FALSUM, id, signum, census);
    dc = _psalmum_conferre(canticum, g, q, FALSUM, id, signum, census);
    {
        chorda titulus = chorda_concatenare(
            _signum_resolutum(q, tonus, terminatio),
            chorda_ex_literis(dc < ds ? " (intonatio omni versu)" : "",
                q), q);
        i32 versus;

        versus = _psalmum_conferre(dc < ds ? canticum : simplex, g, q,
            VERUM, id, titulus, census);
        imprimere("  psalmus %.*s %-24.*s versus discordes %u, "
            "admonitiones %u\n", (integer)id.mensura,
            (constans character*)id.datum, (integer)titulus.mensura,
            (constans character*)titulus.datum,
            (insignatus integer)versus,
            (insignatus integer)admonitiones);
    }
}

interior vacuum
_psalmi_scripti (
    CredoContextus* ctx)
{
             Corpus  corpus;
       PsalmiCensus  census;
    PsalmodiaTabula* tabula;
                i32  k;

    si (!_corpus_aperire(ctx, "psalmi", &corpus))
    {
        redde;
    }
    tabula = _tabula(ctx);
    CREDO_NECESSE (tabula != NIHIL && psalmodia_successus(tabula));
    memset(&census, ZEPHYRUM, magnitudo(census));
    _discordias_creare(ctx, "psalmus", &census.discordiae);
    per (k = ZEPHYRUM; k < corpus.numerus; k++)
    {
        Piscina* q = piscina_generare_dynamicum("psalmodia_corpus",
                         (memoriae_index)(CCLVI * MXXIV));
        GabcCantus* g;

        CREDO_NECESSE (q != NIHIL);
        g = _cantum_legere(&corpus, k, q);
        si (g != NIHIL)
        {
            _psalmum_iudicare(tabula, g, _id(&corpus, k), q,
                &census);
        }
        piscina_destruere(q);
    }
    imprimere("  psalmi: scripti %u, titulus resolutus %u\n",
        (insignatus integer)census.scripti,
        (insignatus integer)census.resoluti);
    CREDO_MAIOR_I32 (census.resoluti, ZEPHYRUM);
    (vacuum)_discordias_conferre(ctx, &census.discordiae);
}


/* ==================================================
 * Sectiones
 * ================================================== */

interior constans CredoSectio SECTIONES[] = {
    { "syllabae corporis: numerus et accentus per verbum",
        _syllabae_corporis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "euouae: terminationes contra antiphonas scriptas", _euouae,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "psalmi scripti: notae, stili, divisiones", _psalmi_scripti,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("psalmodia_corpus", SECTIONES, NIHIL,
        NIHIL);
}
