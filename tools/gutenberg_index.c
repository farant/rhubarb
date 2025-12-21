/* gutenberg_index.c - Creare indicem librorum Gutenberg in STML */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "iter_directoria.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include "stml.h"
#include "internamentum.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>

/* Informationes de filo */
nomen structura {
    chorda via;
    b32    in_old;
    b32    duplicatum;  /* Habet -N suffix */
} FilumInfo;

/* Informationes de libro */
nomen structura {
    chorda numerus;       /* "12345" */
    Xar*   fila;          /* Xar de FilumInfo */
    chorda titulus;       /* Si parsatum */
    chorda auctor;        /* Si parsatum */
    b32    parsatum;
} LiberInfo;

/* Contextus pro ambulatore */
nomen structura {
    TabulaDispersa* libri;    /* numerus -> LiberInfo* */
    Piscina*        piscina;
    i32             fila_totalia;
} IndexContextus;

/* ============================================================
 * Functiones auxiliares (ex gutenberg_census.c)
 * ============================================================ */

hic_manens chorda
extrahere_numerum_libri(chorda titulus, Piscina* piscina)
{
    chorda fructus;
    i32 finis;
    i32 i;
    character c;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    si (titulus.mensura < 5) {
        redde fructus;
    }

    c = (character)titulus.datum[0];
    si (c < '0' || c > '9') {
        redde fructus;
    }

    finis = 0;
    per (i = 0; i < titulus.mensura; i++) {
        c = (character)titulus.datum[i];
        si (c >= '0' && c <= '9') {
            finis = i + 1;
        } alioquin {
            frange;
        }
    }

    si (finis == 0) {
        redde fructus;
    }

    fructus = chorda_sectio(titulus, 0, finis);
    fructus = chorda_transcribere(fructus, piscina);

    redde fructus;
}

hic_manens b32
est_duplicatum(chorda titulus)
{
    i32 i;

    per (i = 2; i < titulus.mensura - 5; i++) {
        character c = (character)titulus.datum[i];
        si (c == '-') {
            si (i + 1 < titulus.mensura) {
                character next = (character)titulus.datum[i + 1];
                si (next >= '0' && next <= '9') {
                    redde VERUM;
                }
            }
        }
    }

    redde FALSUM;
}

hic_manens b32
est_in_old(chorda via)
{
    i32 i;
    per (i = 0; i < via.mensura - 4; i++) {
        si (via.datum[i] == '/' &&
            via.datum[i+1] == 'o' &&
            via.datum[i+2] == 'l' &&
            via.datum[i+3] == 'd' &&
            via.datum[i+4] == '/') {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ============================================================
 * Parsatio metadata
 * ============================================================ */

/* Invenire lineam quae incipit cum praefixo
 * Redde: chorda post praefixum (trimmed), vel vacua si non inventum
 */
hic_manens chorda
invenire_lineam_cum_praefixo(
    chorda              textus,
    constans character* praefixum,
    Piscina*            piscina)
{
    chorda praef;
    i32 i;
    i32 linea_initium;
    chorda fructus;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    praef = chorda_ex_literis(praefixum, piscina);

    linea_initium = 0;
    per (i = 0; i < textus.mensura; i++) {
        si (textus.datum[i] == '\n') {
            linea_initium = i + 1;
        } alioquin si (i == linea_initium || (i > 0 && textus.datum[i-1] == '\n')) {
            /* Initium lineae - verificare praefixum */
            si (i + praef.mensura <= textus.mensura) {
                chorda sectio = chorda_sectio(textus, i, i + praef.mensura);
                si (chorda_aequalis(sectio, praef)) {
                    /* Inventum! Extrahere residuum lineae */
                    i32 finis = i + praef.mensura;
                    dum (finis < textus.mensura && textus.datum[finis] != '\n') {
                        finis++;
                    }
                    fructus = chorda_sectio(textus, i + praef.mensura, finis);
                    /* Trimmare spatia (space, tab, newline, carriage return) */
                    dum (fructus.mensura > 0 &&
                         (fructus.datum[0] == ' ' ||
                          fructus.datum[0] == '\t' ||
                          fructus.datum[0] == '\n' ||
                          fructus.datum[0] == '\r')) {
                        fructus.datum++;
                        fructus.mensura--;
                    }
                    dum (fructus.mensura > 0 &&
                         (fructus.datum[fructus.mensura - 1] == ' ' ||
                          fructus.datum[fructus.mensura - 1] == '\t' ||
                          fructus.datum[fructus.mensura - 1] == '\n' ||
                          fructus.datum[fructus.mensura - 1] == '\r')) {
                        fructus.mensura--;
                    }
                    fructus = chorda_transcribere(fructus, piscina);
                    redde fructus;
                }
            }
        }
    }

    redde fructus;
}

/* Parsare caput fili et extrahere metadata */
hic_manens vacuum
parsare_caput_libri(LiberInfo* liber, chorda via, Piscina* piscina)
{
    chorda contentum;
    chorda caput;
    chorda titulus;
    chorda auctor;
    character via_cstr[1024];
    i32 lineae;
    i32 i;
    i32 finis;

    /* Convertere via ad C string */
    si (via.mensura >= 1024) {
        redde;
    }
    per (i = 0; i < via.mensura; i++) {
        via_cstr[i] = (character)via.datum[i];
    }
    via_cstr[via.mensura] = '\0';

    /* Legere filum */
    contentum = filum_legere_totum(via_cstr, piscina);
    si (contentum.mensura == 0) {
        redde;
    }

    /* Capere primas ~50 lineas */
    lineae = 0;
    finis = 0;
    per (i = 0; i < contentum.mensura && lineae < 50; i++) {
        si (contentum.datum[i] == '\n') {
            lineae++;
        }
        finis = i;
    }
    caput = chorda_sectio(contentum, 0, finis);

    /* Quaerere "Title:" */
    titulus = invenire_lineam_cum_praefixo(caput, "Title:", piscina);
    si (titulus.datum != NIHIL && titulus.mensura > 0) {
        liber->titulus = titulus;
        liber->parsatum = VERUM;
    }

    /* Quaerere "Author:" */
    auctor = invenire_lineam_cum_praefixo(caput, "Author:", piscina);
    si (auctor.datum != NIHIL && auctor.mensura > 0) {
        liber->auctor = auctor;
    }
}

/* ============================================================
 * Callback pro ambulatore
 * ============================================================ */

hic_manens s32
processare_filum(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus)
{
    IndexContextus* ctx = (IndexContextus*)contextus;
    chorda numerus;
    LiberInfo* liber;
    FilumInfo* filum_info;

    /* Solum fila */
    si (introitus->genus != INTROITUS_FILUM) {
        redde 0;
    }

    /* Solum .txt */
    si (!chorda_terminatur(introitus->titulus,
                           chorda_ex_literis(".txt", ctx->piscina))) {
        redde 0;
    }

    ctx->fila_totalia++;

    /* Extrahere numerum libri */
    numerus = extrahere_numerum_libri(introitus->titulus, ctx->piscina);
    si (numerus.datum == NIHIL) {
        /* Non numericum - ignorare pro nunc */
        redde 0;
    }

    /* Capere vel creare LiberInfo */
    {
        vacuum* valor_temp = NIHIL;
        si (tabula_dispersa_invenire(ctx->libri, numerus, &valor_temp)) {
            liber = (LiberInfo*)valor_temp;
        } alioquin {
            liber = NIHIL;
        }
    }
    si (liber == NIHIL) {
        liber = (LiberInfo*)piscina_allocare(ctx->piscina,
                                              magnitudo(LiberInfo));
        liber->numerus = numerus;
        liber->fila = xar_creare(ctx->piscina, magnitudo(FilumInfo));
        liber->titulus.datum = NIHIL;
        liber->titulus.mensura = 0;
        liber->auctor.datum = NIHIL;
        liber->auctor.mensura = 0;
        liber->parsatum = FALSUM;
        tabula_dispersa_inserere(ctx->libri, numerus, liber);
    }

    /* Addere filum ad librum */
    filum_info = (FilumInfo*)xar_addere(liber->fila);
    filum_info->via = chorda_transcribere(via_plena, ctx->piscina);
    filum_info->in_old = est_in_old(via_plena);
    filum_info->duplicatum = est_duplicatum(introitus->titulus);

    redde 0;
}

/* ============================================================
 * Creare STML
 * ============================================================ */

hic_manens StmlNodus*
creare_stml_index(
    TabulaDispersa*      libri,
    Piscina*             piscina,
    InternamentumChorda* intern,
    i32*                 parsata_extra,
    i32*                 indiget_llm_extra)
{
    StmlNodus* radix;
    TabulaIterator iter;
    chorda clavis;
    vacuum* valor;
    i32 parsata = 0;
    i32 indiget_llm = 0;

    radix = stml_elementum_creare(piscina, intern, "librarium");

    iter = tabula_dispersa_iterator_initium(libri);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor)) {
        LiberInfo* liber = (LiberInfo*)valor;
        StmlNodus* nodus_liber;
        i32 i;
        i32 num_fila;

        /* Si nondum parsatum, probare nunc */
        si (!liber->parsatum && liber->fila != NIHIL) {
            num_fila = xar_numerus(liber->fila);
            /* Probare primum filum quod non est in old/ */
            per (i = 0; i < num_fila; i++) {
                FilumInfo* fi = (FilumInfo*)xar_obtinere(liber->fila, i);
                si (!fi->in_old) {
                    parsare_caput_libri(liber, fi->via, piscina);
                    si (liber->parsatum) {
                        frange;
                    }
                }
            }
            /* Si adhuc non parsatum, probare quodlibet */
            si (!liber->parsatum && num_fila > 0) {
                FilumInfo* fi = (FilumInfo*)xar_obtinere(liber->fila, 0);
                parsare_caput_libri(liber, fi->via, piscina);
            }
        }

        /* Creare nodum */
        nodus_liber = stml_elementum_creare(piscina, intern, "liber");

        /* Numerus attributum */
        stml_attributum_addere_chorda(nodus_liber, piscina, intern,
                                       "numerus", liber->numerus);

        /* Status */
        si (liber->parsatum) {
            stml_attributum_addere(nodus_liber, piscina, intern,
                                   "status", "parsatum");
            parsata++;
        } alioquin {
            stml_attributum_addere(nodus_liber, piscina, intern,
                                   "status", "indiget_llm");
            indiget_llm++;
        }

        /* Titulus si habemus */
        si (liber->titulus.datum != NIHIL && liber->titulus.mensura > 0) {
            StmlNodus* nodus_tit = stml_elementum_creare(piscina, intern, "titulus");
            {
                StmlNodus* textus_nodus = stml_textum_creare_ex_chorda(
                    piscina, intern, liber->titulus);
                stml_liberum_addere(nodus_tit, textus_nodus);
            }
            stml_liberum_addere(nodus_liber, nodus_tit);
        }

        /* Auctor si habemus */
        si (liber->auctor.datum != NIHIL && liber->auctor.mensura > 0) {
            StmlNodus* nodus_auc = stml_elementum_creare(piscina, intern, "auctor");
            {
                StmlNodus* textus_nodus = stml_textum_creare_ex_chorda(
                    piscina, intern, liber->auctor);
                stml_liberum_addere(nodus_auc, textus_nodus);
            }
            stml_liberum_addere(nodus_liber, nodus_auc);
        }

        /* Fila */
        si (liber->fila != NIHIL) {
            num_fila = xar_numerus(liber->fila);
            per (i = 0; i < num_fila; i++) {
                FilumInfo* fi = (FilumInfo*)xar_obtinere(liber->fila, i);
                StmlNodus* nodus_filum = stml_elementum_creare(
                    piscina, intern, "filum");
                stml_attributum_addere_chorda(nodus_filum, piscina, intern,
                                              "via", fi->via);
                si (fi->in_old) {
                    stml_attributum_boolean_addere(nodus_filum, piscina, intern,
                                                   "in_old");
                }
                si (fi->duplicatum) {
                    stml_attributum_boolean_addere(nodus_filum, piscina, intern,
                                                   "duplicatum");
                }
                stml_liberum_addere(nodus_liber, nodus_filum);
            }
        }

        stml_liberum_addere(radix, nodus_liber);
    }

    *parsata_extra = parsata;
    *indiget_llm_extra = indiget_llm;
    redde radix;
}

/* ============================================================
 * Principale
 * ============================================================ */

s32 principale(s32 argc, character** argv)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    IndexContextus ctx;
    DirectoriumFiltrum filtrum;
    constans character* via_input;
    constans character* via_output;
    StmlNodus* stml_radix;
    chorda stml_textus;
    b32 scriptura_successus;
    i32 libri_totales;
    i32 parsata;
    i32 indiget_llm;

    /* Argumenta */
    si (argc > 1) {
        via_input = argv[1];
    } alioquin {
        via_input = "../gutenberg-mirror";
    }

    si (argc > 2) {
        via_output = argv[2];
    } alioquin {
        via_output = "gutenberg_index.stml";
    }

    /* Initiare */
    piscina = piscina_generare_dynamicum("index", 64 * 1024 * 1024);
    si (!piscina) {
        imprimere("Error: non possum allocare memoriam\n");
        redde 1;
    }

    intern = internamentum_creare(piscina);

    ctx.libri = tabula_dispersa_creare_chorda(piscina, 4096);
    ctx.piscina = piscina;
    ctx.fila_totalia = 0;

    imprimere("Ambulans per: %s\n", via_input);
    imprimere("...\n");

    /* Ambulare */
    filtrum = directorium_filtrum_omnia();
    filtrum.includere_occultos = FALSUM;

    si (directorium_ambulare(via_input, &filtrum, processare_filum,
                              &ctx, piscina) < 0) {
        imprimere("Error: non possum aperire '%s'\n", via_input);
        piscina_destruere(piscina);
        redde 1;
    }

    libri_totales = tabula_dispersa_numerus(ctx.libri);
    imprimere("Inventa: %u fila, %u libri unici\n",
              ctx.fila_totalia, libri_totales);
    imprimere("Parsans metadata...\n");

    /* Creare STML */
    stml_radix = creare_stml_index(ctx.libri, piscina, intern,
                                    &parsata, &indiget_llm);

    /* Scribere ad filum */
    stml_textus = stml_scribere(stml_radix, piscina, VERUM);
    scriptura_successus = filum_scribere(via_output, stml_textus);

    si (!scriptura_successus) {
        imprimere("Error: non possum scribere '%s'\n", via_output);
        piscina_destruere(piscina);
        redde 1;
    }

    /* Summa */
    imprimere("\n");
    imprimere("=== INDEX GUTENBERG ===\n");
    imprimere("Libri totales:  %u\n", libri_totales);
    imprimere("Parsata:        %u\n", parsata);
    imprimere("Indiget LLM:    %u\n", indiget_llm);
    imprimere("Scriptum:       %s\n", via_output);
    imprimere("=======================\n");

    piscina_destruere(piscina);
    redde 0;
}
