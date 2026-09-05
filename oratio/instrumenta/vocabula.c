/* vocabula.c - Instrumentum recensionis vocabulorum (T10)
 *
 * Usus: vocabula [-symbola | -commenta | -omnia] [-machina] [-omnes]
 *                [-tectum N]
 *   -symbola   identificatores ex build/nexus.tsv (sedes)
 *   -commenta  commentaria plagularum C in oratio/build/corpus_c.txt
 *   -omnia     utraque (ordinarium)
 *   -machina   TSV omnium verborum cum capite
 *   -omnes     lista ignotorum tota (plane: tectum LX)
 * Glossarium oratio/glossarium.stml appenditur si adest. Radix per
 * RHUBARB_RADIX. Exitus: 0 relatio scripta (ignota inventa sunt, non
 * vitia) | 2 usus/tabula/nexus absens
 */

#include "latina.h"
#include "oratio_vocabula.h"
#include "oratio_vocabularium.h"
#include "oratio_vocabularium_la.h"
#include "oratio_glossarium.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior vacuum
_c (
    chorda c)
{
    imprimere("%.*s", (integer)c.mensura, (constans character*)c.datum);
}

/* directorium SUMMUM viae (lib, silva, knotapel, ...) */
interior chorda
_directorium (
    chorda via)
{
    i32 i;

    per (i = ZEPHYRUM; i < via.mensura; i++)
    {
        si (via.datum[i] == '/')
        {
            via.mensura = i;
            redde via;
        }
    }
    redde via;
}

integer
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
     constans character* radix;
              character  via[1024];
                 chorda  tabula;
   OratioVocabulariumLa* voc;
OratioVocabulariumVitium vitium;
         OratioVocabula* vc;
                    b32  symbola  = VERUM;
                    b32  commenta = VERUM;
                                        b32  machina  = FALSUM;
                    b32  omnes_viae = FALSUM;
                    i32  tectum   = (i32)60;

                integer  i;
                clock_t  ante;
                    i32  plagulae = ZEPHYRUM;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-symbola") == ZEPHYRUM)
        {
            symbola   = VERUM;
            commenta  = FALSUM;
        }
        alioquin si (strcmp(argv[i], "-commenta") == ZEPHYRUM)
        {
            symbola   = FALSUM;
            commenta  = VERUM;
        }
        alioquin si (strcmp(argv[i], "-omnia") == ZEPHYRUM)
        {
            symbola   = VERUM;
            commenta  = VERUM;
        }
        alioquin si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
                alioquin si (strcmp(argv[i], "-omnes") == ZEPHYRUM)
        {
            tectum = (i32)1000000;
        }
        alioquin si (strcmp(argv[i], "-omnes-viae") == ZEPHYRUM)
        {
            omnes_viae = VERUM;   /* etiam knotapel/ vendor/ archivum/ */
        }

        alioquin si (   strcmp(argv[i], "-tectum") == ZEPHYRUM
                     && i + I < argc)
        {
            i       = i + I;
            tectum  = (i32)atoi(argv[i]);
        }
        alioquin
        {
                        fprintf(stderr,
                            "usus: vocabula [-symbola | -commenta | -omnia] [-machina] [-omnes] [-omnes-viae] [-tectum N]\n");

            redde II;
        }
    }
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina = piscina_generare_dynamicum("oratio_vocabula", 536870912);
    sprintf(via, "%s/oratio/vocabularium/la.bin", radix);
    si (!_plagulam_legere(piscina, via, &tabula))
    {
        fprintf(stderr, "vocabula: tabula absens: %s\n", via);
        redde II;
    }
    voc = oratio_vocabularium_la_onerare(piscina, tabula, &vitium);
    si (voc == NIHIL)
    {
        fprintf(stderr, "vocabula: onus fractum: %s\n", vitium.causa);
        redde II;
    }
    sprintf(via, "%s/oratio/glossarium.stml", radix);
    {
        chorda fons_glossarii;

        si (_plagulam_legere(piscina, via, &fons_glossarii))
        {
            OratioGlossarium* gl = oratio_glossarium_legere(piscina,
                fons_glossarii, &vitium);

            si (gl == NIHIL)
            {
                fprintf(stderr,
                    "vocabula: glossarium non legitur: %s:%d %s\n",
                    vitium.plagula, (integer)vitium.linea,
                    vitium.causa);
                redde II;
            }
            oratio_vocabularium_la_glossarium_ponere(voc, gl);
        }
    }
    vc = oratio_vocabula_creare(piscina, voc);
    si (vc == NIHIL)
    {
        redde II;
    }
    ante = clock();
    si (symbola)
    {
        chorda nexus;

        sprintf(via, "%s/build/nexus.tsv", radix);
        si (!_plagulam_legere(piscina, via, &nexus))
        {
            fprintf(stderr,
                "vocabula: nexus absens: %s (./silva/nexus.sh -renovare)\n",
                via);
            redde II;
        }
                si (!oratio_vocabula_symbola(vc, nexus,
                    omnes_viae ? NIHIL : ORATIO_VOCABULA_EXCLUSA))
                {

            fprintf(stderr, "vocabula: symbola fracta\n");
            redde II;
                }
    }
    si (commenta)
    {
        FILE* lista;
        character linea[512];

        sprintf(via, "%s/oratio/build/corpus_c.txt", radix);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            fprintf(stderr,
                "vocabula: corpus_c.txt absens: %s (cursor eam scribit)\n",
                via);
            redde II;
        }
        dum (fgets(linea, (integer)magnitudo(linea), lista) != NIHIL)
        {
             size_t  l = strlen(linea);
            Piscina* p;
             chorda  fons;

            dum (   l > ZEPHYRUM
                 && (linea[l - I] == '\n' || linea[l - I] == '\r'))
            {
                linea[--l] = '\0';
            }
            si (l == ZEPHYRUM)
            {
                perge;
            }
            p = piscina_generare_dynamicum("vocabula_plagula",
                33554432);
            sprintf(via, "%s/%s", radix, linea);
            si (_plagulam_legere(p, via, &fons))
            {
                plagulae = plagulae + I;
                si (!oratio_vocabula_commenta(vc, fons, linea))
                {
                    fprintf(stderr, "vocabula: commenta fracta: %s\n",
                        linea);
                    redde II;
                }
            }
            piscina_destruere(p);
        }
        fclose(lista);
    }
    si (!oratio_vocabula_iudicare(vc))
    {
        fprintf(stderr, "vocabula: iudicium fractum\n");
        redde II;
    }
    {
           Xar* verba  = oratio_vocabula_verba(vc);
           i32  n      = xar_numerus(verba);
        duplex  ms = 1000.0 * (duplex)(clock() - ante)
            / (duplex)CLOCKS_PER_SEC;

        si (machina)
        {
            Xar* ordo = oratio_vocabula_ordinata(piscina, vc, (s32)-I);

            imprimere("# verbum\tstatus\tsedes\tsymbola\tcommenta\tclassis\tlemma\tanalyses\tlemmata\tvia\tlinea\n");
            per (i = ZEPHYRUM; i < (integer)xar_numerus(ordo); i++)
            {
                constans OratioVerbum* v =
                    (constans OratioVerbum*)xar_obtinere(
                    verba, (i32)*(s32*)xar_obtinere(ordo, (i32)i));

                _c(v->verbum);
                imprimere("\t%s\t%d\t%d\t%d\t",
                    oratio_verbum_status_titulus(v->status),
                    (integer)v->sedes, (integer)v->sedes_symbolorum,
                    (integer)v->sedes_commentorum);
                _c(v->classis);
                putchar('\t');
                _c(v->lemma);
                imprimere("\t%d\t%d\t", (integer)v->analyses,
                    (integer)v->lemmata);
                _c(v->via_prima);
                imprimere("\t%d\n", (integer)v->linea_prima);
            }
            redde ZEPHYRUM;
        }
                imprimere("--- vocabula: %s%s%s%s ---\n",
                    symbola ? "symbola" : "",
                    symbola
                    && commenta ? " + " : "",
                    commenta ? "commenta" : "",
                    symbola
                    && !omnes_viae ? " (sine knotapel/ vendor/ archivum/)" : "");

        imprimere("  verba distincta %d  sedes %d  plagulae C %d  %.0f ms\n",
            (integer)n, (integer)oratio_vocabula_sedes(vc),
            (integer)plagulae, ms);
        imprimere("  nota %d  ambigua %d  permissa %d  IGNOTA %d\n",
            (integer)oratio_vocabula_numerus(vc, ORATIO_VERBUM_NOTUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_AMBIGUUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_PERMISSUM),
            (integer)oratio_vocabula_numerus(vc,
            ORATIO_VERBUM_IGNOTUM));
        /* ignota per directorium sedis primae */
        {
            Xar* ordo = oratio_vocabula_ordinata(piscina, vc,
                (s32)ORATIO_VERBUM_IGNOTUM);
            chorda dirs[64];
               i32 numeri[64];
               i32 nd = ZEPHYRUM;
               i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(ordo); k++)
            {
                constans OratioVerbum* v =
                    (constans OratioVerbum*)xar_obtinere(
                    verba, (i32)*(s32*)xar_obtinere(ordo, k));
                chorda d = _directorium(v->via_prima);
                   i32 j;
                   b32 inventum = FALSUM;

                per (j = ZEPHYRUM; j < nd; j++)
                {
                    si (   dirs[j].mensura == d.mensura
                        && memcmp(dirs[j].datum, d.datum,
                        (size_t)d.mensura)
                            == ZEPHYRUM)
                    {
                        numeri[j]  = numeri[j] + I;
                        inventum   = VERUM;
                        frange;
                    }
                }
                si (!inventum && nd < (i32)64)
                {
                    dirs[nd]    = d;
                    numeri[nd]  = I;
                    nd          = nd + I;
                }
            }
                        /* ordinare per numerum descendentem (nd <= LXIV) */
            {
                i32 a;
                i32 b;

                per (a = ZEPHYRUM; a < nd; a++)
                {
                    per (b = a + I; b < nd; b++)
                    {
                        si (numeri[b] > numeri[a])
                        {
                            chorda dt = dirs[a];
                               i32 nt = numeri[a];

                            dirs[a]    = dirs[b];
                            numeri[a]  = numeri[b];
                            dirs[b]    = dt;
                            numeri[b]  = nt;
                        }
                    }
                }
            }
            imprimere("  ignota per directorium summum sedis primae:");
            per (k = ZEPHYRUM; k < nd; k++)
            {
                imprimere(" ");
                _c(dirs[k]);
                imprimere("=%d", (integer)numeri[k]);
            }

            imprimere("\n\n--- IGNOTA (sedes descendentes; tectum %d) ---\n",
                (integer)tectum);
            per (k = ZEPHYRUM; k < xar_numerus(ordo) && k < tectum; k++)
            {
                constans OratioVerbum* v =
                    (constans OratioVerbum*)xar_obtinere(
                    verba, (i32)*(s32*)xar_obtinere(ordo, k));

                imprimere("  %-24.*s %6d  ", (integer)v->verbum.mensura,
                    (constans character*)v->verbum.datum,
                    (integer)v->sedes);
                _c(v->via_prima);
                imprimere(":%d%s\n", (integer)v->linea_prima,
                    v->ex_commento_prima ? " (commentum)" : "");
            }
        }
        {
            Xar* ordo = oratio_vocabula_ordinata(piscina, vc,
                (s32)ORATIO_VERBUM_AMBIGUUM);
            i32 k;

            imprimere("\n--- AMBIGUA (prima XX) ---\n");
            per (k = ZEPHYRUM; k < xar_numerus(ordo)
                && k < (i32)XX; k++)
            {
                constans OratioVerbum* v =
                    (constans OratioVerbum*)xar_obtinere(
                    verba, (i32)*(s32*)xar_obtinere(ordo, k));

                imprimere("  %-24.*s %6d  lemmata %d  primum ",
                    (integer)v->verbum.mensura,
                    (constans character*)v->verbum.datum,
                    (integer)v->sedes,
                    (integer)v->lemmata);
                _c(v->classis);
                putchar(' ');
                _c(v->lemma);
                putchar('\n');
            }
        }
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
