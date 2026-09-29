/* probatio_tessera_folium.c - folium: involutio et pictura (runae U7)
 *
 * I.   Casus parvi: spatium, CJK (inter lata), Thai (limes), unitas
 *      latior quam latitudo, paragraphi vacui, CR LF, latitudo nulla.
 * II.  Invarianta per corpus (XXXV linguae x politicae II x latitudines
 *      XL et XVII), INDEPENDENTER ex runae computata: lineae textum
 *      sine iactura reddunt; latitudo lineae vera et <= limitem (nisi
 *      unitas sola); limites ad limites unitatum; omnis ruptura
 *      licita (post spatium, inter NATURA lata - runa prima II -,
 *      aut linea plena);
 *      statisticae = summae.
 * III. Pictura: ordo quisque relectus == linea sua (imperia ' ',
 *      unitates latitudinis 0 non pictae); aurea paginae primae ja,
 *      hi, yo ad XL x X (probationes/fixa/folium/; absentia rubra -
 *      FOLIUM_AURUM_SCRIBERE=1 scribit, oculo Frani probanda).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "runae.h"
#include "utf8.h"
#include "folium_pagina.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CORPUS_MAXIMUM 65536
#define ORDO_MAXIMUS   1024

interior constans character* constans LINGUAE[] = {
    "ar", "bn", "ceb", "de", "el", "en", "es", "fa", "fr",
    "gu", "he", "hi", "hu", "id", "ig", "it", "ja", "ko",
    "la", "ml", "nl", "pl", "pt", "ro", "ru", "rw", "sv",
    "sw", "ta", "th", "tl", "tr", "vi", "yo", "zh"
};

/* Fasciculum legere (RHUBARB_RADIX a cursore datur); 0 si absens */
interior i32
_legere (
    constans character* via_relativa,
                    i8* alveus)
{
             character  via[DXII];
    constans character* radix = getenv("RHUBARB_RADIX");
         FILE* f;
         i32  lecti;

    sprintf(via, "%s/%s", radix != NIHIL ? radix : "..", via_relativa);
    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde ZEPHYRUM;
    }
    lecti = (i32)fread(alveus, I, CORPUS_MAXIMUM, f);
    fclose(f);
    redde lecti;
}

/* Casum parvum involvere et latitudines linearum conferre */
interior vacuum
_casus (
                Piscina* piscina,
     constans character* textus,
                    i32  latitudo,
                    i32  numerus,
     constans      i32* latitudines,
     constans character* causa)
{
    FoliumInvolutio inv;
                i32 k;
                b32 bona;

    CREDO_VERUM (folium_involvere(piscina,
        chorda_ex_literis(textus, piscina), latitudo,
        RUNAE_POLITICA_GRAPHEMATUM, &inv));
    bona = (b32)(inv.numerus == numerus);
    per (k = ZEPHYRUM; bona && k < numerus; k++)
    {
        bona = (b32)(inv.lineae[k].latitudo == latitudines[k]);
    }
    si (!bona)
    {
        imprimere("  FRACTA: %s: lineae %u:", causa,
            (insignatus integer)inv.numerus);
        per (k = ZEPHYRUM; k < inv.numerus; k++)
        {
            imprimere(" [%u-%u lat %u]",
                (insignatus integer)inv.lineae[k].initium,
                (insignatus integer)inv.lineae[k].finis,
                (insignatus integer)inv.lineae[k].latitudo);
        }
        imprimere("\n");
    }
    CREDO_VERUM (bona);
}

/* Runa prima unitatis natura lata (II)? */
interior b32
_lata (
    constans i8* cursor,
    constans i8* finis)
{
    constans i8* post = cursor;
            s32  runa = utf8_decodere(&post, finis);

    redde (b32)(runa >= ZEPHYRUM && runae_latitudo(runa) == II);
}

/* Unitatem ultimam [initium, finis) quaerere: initium eius */
interior constans i8*
_ultima (
      constans i8* initium,
      constans i8* finis,
    RunaePolitica  politica)
{
    constans i8* cursor = initium;
    constans i8* ultima = initium;
            i32  lat;

    dum (cursor < finis)
    {
        ultima = cursor;
        cursor = runae_unitas_proxima(cursor, finis, politica, &lat);
    }
    redde ultima;
}

/* II. Invarianta unius involutionis; reddit numerum fracturarum */
interior i32
_proprietates (
                      chorda  textus,
    constans FoliumInvolutio* inv,
                         i32  latitudo,
               RunaePolitica  politica,
          constans character* lingua)
{
    constans i8* t       = textus.datum;
            i32  cursor  = ZEPHYRUM;   /* offset post lineam priorem */
            i32  k;
            i32  fracturae  = ZEPHYRUM;
            i32  summa      = ZEPHYRUM;

    per (k = ZEPHYRUM; k < inv->numerus; k++)
    {
        constans FoliumLinea* l = &inv->lineae[k];
                         i32  lat_vera;
                 constans i8* ambulans;
                         i32  lat;
                         b32  sola;

        /* sine iactura: inter lineas nihil, aut \n, aut \r\n */
        si (   !(l->initium == cursor)
            && !(l->initium == cursor + I && t[cursor] == '\n')
            && !(l->initium == cursor + II && t[cursor] == '\r'
                 && t[cursor + I] == '\n'))
        {
            si (fracturae < III)
            {
                imprimere("  FRACTA: %s/%u: linea %u incipit %u, "
                    "exspectatum post %u\n", lingua,
                    (insignatus integer)latitudo,
                    (insignatus integer)k,
                    (insignatus integer)l->initium,
                    (insignatus integer)cursor);
            }
            fracturae++;
        }
        /* latitudo vera et limes */
        lat_vera = runae_latitudo_textus(t + l->initium, t + l->finis,
            politica);
        sola = (b32)(runae_unitas_proxima(t + l->initium, t + l->finis,
            politica, &lat) == t + l->finis);
        si (   lat_vera != l->latitudo
            || (l->latitudo > latitudo && !sola))
        {
            si (fracturae < III)
            {
                imprimere("  FRACTA: %s/%u: linea %u latitudo %u "
                    "(vera %u)\n", lingua,
                    (insignatus integer)latitudo,
                    (insignatus integer)k,
                    (insignatus integer)l->latitudo,
                    (insignatus integer)lat_vera);
            }
            fracturae++;
        }
        summa += l->latitudo;
        /* limes ad limitem unitatis */
        ambulans = t + l->initium;
        dum (ambulans < t + l->finis)
        {
            ambulans = runae_unitas_proxima(ambulans, t
                + textus.mensura,
                politica, &lat);
        }
        si (ambulans != t + l->finis)
        {
            si (fracturae < III)
            {
                imprimere("  FRACTA: %s/%u: linea %u finis %u intra "
                    "unitatem\n", lingua, (insignatus integer)latitudo,
                    (insignatus integer)k,
                    (insignatus integer)l->finis);
            }
            fracturae++;
        }
        /* ruptura licita (linea sequens eodem octeto incipit) */
        si (   k + I < inv->numerus
            && inv->lineae[k + I].initium == l->finis
            && l->finis > l->initium)
        {
            constans i8* ultima = _ultima(t + l->initium, t + l->finis,
                politica);
                    i32 lat_sequentis;

            (vacuum)runae_unitas_proxima(t + l->finis, t
                + textus.mensura, politica, &lat_sequentis);
            si (   t[l->finis - I]             != ' '
                && !_lata(ultima, t + l->finis)
                && !_lata(t + l->finis, t + textus.mensura)
                && l->latitudo + lat_sequentis <= latitudo)
            {
                si (fracturae < III)
                {
                    imprimere("  FRACTA: %s/%u: ruptura illicita post "
                        "lineam %u (lat %u, sequens %u)\n", lingua,
                        (insignatus integer)latitudo,
                        (insignatus integer)k,
                        (insignatus integer)l->latitudo,
                        (insignatus integer)lat_sequentis);
                }
                fracturae++;
            }
        }
        cursor = l->finis;
    }
    /* cauda: nihil nisi \r?\n post lineam ultimam */
    si (   !(cursor == textus.mensura)
        && !(cursor + I == textus.mensura && t[cursor] == '\n')
        && !(cursor + II == textus.mensura && t[cursor] == '\r'))
    {
        imprimere("  FRACTA: %s/%u: cauda %u ex %u non reddita\n",
            lingua,
            (insignatus integer)latitudo, (insignatus integer)cursor,
            (insignatus integer)textus.mensura);
        fracturae++;
    }
    si (summa != inv->columnae)
    {
        imprimere("  FRACTA: %s/%u: columnae %u, summa linearum %u\n",
            lingua, (insignatus integer)latitudo,
            (insignatus integer)inv->columnae,
            (insignatus integer)summa);
        fracturae++;
    }
    redde fracturae;
}

/* Ordinem y relegere: continuationes omissae, cellula vacua ' ',
 * spatia caudae praecisa */
interior i32
_ordo (
    TesseraOpus* opus,
            s32  y,
             i8* exitus)
{
    s32 x;
    i32 n = ZEPHYRUM;

    per (x = ZEPHYRUM; x < (s32)tessera_latitudo(opus); x++)
    {
        TesseraCellula c = tessera_cellulam_legere(opus, x, y);
                   i32 m;

        si (c.ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
        {
            perge;
        }
        m = tessera_cellulae_octeti(opus, x, y, exitus + n,
            ORDO_MAXIMUS - n);
        si (m == ZEPHYRUM)
        {
            exitus[n++] = ' ';
        }
        n += m;
    }
    dum (n > ZEPHYRUM && exitus[n - I] == ' ')
    {
        n--;
    }
    redde n;
}

/* Linea exspectata: unitates latitudinis 0 omissae, imperia ' ',
 * spatia caudae praecisa */
interior i32
_linea_exspectata (
                  chorda  textus,
    constans FoliumLinea* l,
           RunaePolitica  politica,
                      i8* exitus)
{
    constans i8* cursor  = textus.datum + l->initium;
    constans i8* finis   = textus.datum + l->finis;
            i32  n       = ZEPHYRUM;

    dum (cursor < finis)
    {
                i32  lat;
        constans i8* post = runae_unitas_proxima(cursor, finis,
            politica,
            &lat);

        si (*cursor < 0x20 || *cursor == 0x7F)
        {
            exitus[n++] = ' ';
        }
        alioquin si (lat > ZEPHYRUM)
        {
            memcpy(exitus + n, cursor, (memoriae_index)(post - cursor));
            n += (i32)(post - cursor);
        }
        cursor = post;
    }
    dum (n > ZEPHYRUM && exitus[n - I] == ' ')
    {
        n--;
    }
    redde n;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                     i8* alveus;
                    i32  k;

    piscina = piscina_generare_dynamicum("probatio_tessera_folium",
        67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    alveus = (i8*)piscina_allocare(piscina, CORPUS_MAXIMUM);
    CREDO_NON_NIHIL (alveus);

    imprimere("\n--- I. Casus ---\n");
    {
        hic_manens constans i32 SPATIUM[]    = { VI, X };
        hic_manens constans i32 SINICA[]     = { IV, IV, II };
        hic_manens constans i32 SIAMENSIS[]  = { III, III, I };
        hic_manens constans i32 LATUM[]      = { I, II, I };
        hic_manens constans i32 PARAGRAPHI[] = { II, ZEPHYRUM,
            II };
        hic_manens constans i32 REDITUS_LINEAE[]  = { II, II };
        hic_manens constans i32 MIXTUM[]          = { III, III };
        hic_manens constans i32 SIGNUM[]          = { III, IV, II };
        hic_manens constans i32 POST_LATUM[]      = { II, III };
                FoliumInvolutio inv;

        _casus(piscina, "alpha beta gamma", X, II, SPATIUM,
            "post spatium");
        _casus(piscina, "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"
            "\xE4\xB8\xAD\xE6\x96\x87", IV, III, SINICA,
            "inter lata (CJK)");
        /* sa wa-ai sa do-ii kho ra-a bo: VII unitates I columnae */
        _casus(piscina, "\xE0\xB8\xAA\xE0\xB8\xA7\xE0\xB8\xB1"
            "\xE0\xB8\xAA\xE0\xB8\x94\xE0\xB8\xB5\xE0\xB8\x84"
            "\xE0\xB8\xA3\xE0\xB8\xB1\xE0\xB8\x9A", III, III, SIAMENSIS,
            "Thai: ad limitem");
        _casus(piscina, "a\xE5\xBA\x83" "b", I, III, LATUM,
            "latum latius quam latitudo: solum");
        _casus(piscina, "ab\n\ncd", X, III, PARAGRAPHI,
            "paragraphus vacuus");
        _casus(piscina, "ab\r\ncd", X, II, REDITUS_LINEAE,
            "CR LF tonsum");
        _casus(piscina, "ab \xE5\xBA\x83" "c", IV, II, MIXTUM,
            "spatium deinde latum");
        /* ka + i (Mc): unitas II columnarum sed NATURA non lata -
         * nulla ruptura intra verbum Hindi (mensuratum in aureo hi:
         * 'kornelius' fractum) */
        _casus(piscina, "ab \xE0\xA4\x95\xE0\xA4\xBF"
            "\xE0\xA4\x95\xE0\xA4\xBF\xE0\xA4\x95\xE0\xA4\xBF", V,
            III, SIGNUM, "signum spatians non latum natura");
        /* ruptura post latum, NON ad limitem: 広|abc, non 広a|bc */
        _casus(piscina, "\xE5\xBA\x83" "abc", III, II, POST_LATUM,
            "ruptura post latum");
        CREDO_FALSUM (folium_involvere(piscina,
            chorda_ex_literis("ab", piscina), ZEPHYRUM,
            RUNAE_POLITICA_GRAPHEMATUM, &inv));
    }

    imprimere("\n--- II. Invarianta per corpus ---\n");
    {
        hic_manens constans i32 LATITUDINES[]  = { XL, XVII };
                            i32 fracturae      =
                                ZEPHYRUM;
                            i32 involutiones   =
                                ZEPHYRUM;

        per (k = ZEPHYRUM; k < XXXV; k++)
        {
            character via[CXXVIII];
                  i32 mensura;
                  i32 j;
                  i32 p;

            sprintf(via, "probationes/fixa/runae/corpus/%s.txt",
                LINGUAE[k]);
            mensura = _legere(via, alveus);
            CREDO_VERUM (mensura > ZEPHYRUM);
            per (j = ZEPHYRUM; j < II; j++)
            {
                per (p = ZEPHYRUM; p < II; p++)
                {
                    RunaePolitica politica = (p == ZEPHYRUM)
                        ? RUNAE_POLITICA_GRAPHEMATUM
                        : RUNAE_POLITICA_SIMPLEX;
                             chorda textus;
                    FoliumInvolutio inv;

                    textus.datum    = alveus;
                    textus.mensura  = mensura;
                    si (!folium_involvere(piscina, textus,
                            LATITUDINES[j], politica, &inv))
                    {
                        fracturae++;
                        perge;
                    }
                    fracturae += _proprietates(textus, &inv,
                        LATITUDINES[j], politica, LINGUAE[k]);
                    si (inv.numerus == ZEPHYRUM)
                    {
                        fracturae++;
                    }
                    involutiones++;
                }
            }
        }
        imprimere("  involutiones: %u, fracturae: %u\n",
            (insignatus integer)involutiones,
            (insignatus integer)fracturae);
        CREDO_AEQUALIS_I32 (involutiones, (i32)CXL);
        CREDO_AEQUALIS_I32 (fracturae, ZEPHYRUM);
    }

    imprimere("\n--- III. Pictura et aurea ---\n");
    /* imperium (tabula) pingitur ut spatium */
    {
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina,
            X, II);
            TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);
        FoliumInvolutio  inv;
                     i8  ordo[ORDO_MAXIMUS];
                 chorda  textus = chorda_ex_literis("a\tb", piscina);
                    i32  n;

        CREDO_VERUM (folium_involvere(piscina, textus, X,
            RUNAE_POLITICA_GRAPHEMATUM, &inv));
        folium_paginam_pingere(opus, textus, &inv, ZEPHYRUM, II,
            tessera_stilus_nativus());
        n = _ordo(opus, ZEPHYRUM, ordo);
        CREDO_AEQUALIS_I32 (n, III);
        CREDO_VERUM (memcmp(ordo, "a b", III) == ZEPHYRUM);
    }
    {
        hic_manens constans character* constans AUREAE[] = {
            "ja", "hi", "yo"
        };
        b32 scribere = (b32)(getenv("FOLIUM_AURUM_SCRIBERE") != NIHIL);
         i8 ordo[ORDO_MAXIMUS];
         i8 exspectata[ORDO_MAXIMUS];

        per (k = ZEPHYRUM; k < III; k++)
        {
            TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(
                piscina, XL, X);
                    TesseraOpus* opus = tessera_aperire(piscina,
                        &pm->pons);
                  character  via[CXXVIII];
                        i32  mensura;
                     chorda  textus;
            FoliumInvolutio  inv;
          ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                (memoriae_index)(IV * M));
                        i32 y;
                        i32 congrui = ZEPHYRUM;

            CREDO_NON_NIHIL (opus);
            sprintf(via, "probationes/fixa/runae/corpus/%s.txt",
                AUREAE[k]);
            mensura         = _legere(via, alveus);
            textus.datum    = alveus;
            textus.mensura  = mensura;
            CREDO_VERUM (folium_involvere(piscina, textus, XL,
                RUNAE_POLITICA_GRAPHEMATUM, &inv));
            tessera_purgare(opus, tessera_stilus_nativus());
            folium_paginam_pingere(opus, textus, &inv, ZEPHYRUM, X,
                tessera_stilus_nativus());
            per (y = ZEPHYRUM; y < X; y++)
            {
                i32 n = _ordo(opus, (s32)y, ordo);
             chorda relectus;
                i32 m = (y < inv.numerus)
                    ? _linea_exspectata(textus, &inv.lineae[y],
                          RUNAE_POLITICA_GRAPHEMATUM, exspectata)
                    : ZEPHYRUM;

                si (   n == m
                    && memcmp(ordo, exspectata, (memoriae_index)n)
                           == ZEPHYRUM)
                {
                    congrui++;
                }
                relectus.datum    = ordo;
                relectus.mensura  = n;
                chorda_aedificator_appendere_chorda(aed, relectus);
                chorda_aedificator_appendere_character(aed, '\n');
            }
            imprimere("  %s: ordines congrui %u/X\n", AUREAE[k],
                (insignatus integer)congrui);
            CREDO_AEQUALIS_I32 (congrui, X);
            {
                   chorda pagina = chorda_aedificator_finire(aed);
                character via_auri[CXXVIII];
                      i32 m;

                sprintf(via_auri,
                    "probationes/fixa/folium/%s.40x10.txt", AUREAE[k]);
                si (scribere)
                {
                    character via_plena[DXII];
                         FILE* f;
                    constans character* radix = getenv("RHUBARB_RADIX");

                    sprintf(via_plena, "%s/%s",
                        radix != NIHIL ? radix : "..", via_auri);
                    f = fopen(via_plena, "wb");
                    CREDO_NON_NIHIL (f);
                    si (f != NIHIL)
                    {
                        (vacuum)fwrite(pagina.datum, I,
                            (memoriae_index)pagina.mensura, f);
                        fclose(f);
                        imprimere("  aurum scriptum: %s\n", via_auri);
                    }
                }
                m = _legere(via_auri, alveus);
                si (   m != pagina.mensura
                    || memcmp(alveus, pagina.datum,
                           (memoriae_index)m) != ZEPHYRUM)
                {
                    imprimere("  FRACTA: %s: aurum %s (%u octeti, "
                        "pagina %u)\n", AUREAE[k], via_auri,
                        (insignatus integer)m,
                        (insignatus integer)pagina.mensura);
                }
                CREDO_AEQUALIS_I32 (m, pagina.mensura);
                CREDO_VERUM (m == pagina.mensura
                    && memcmp(alveus, pagina.datum,
                           (memoriae_index)m) == ZEPHYRUM);
            }
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
