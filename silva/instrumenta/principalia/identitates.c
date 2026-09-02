/* identitates.c - index identitatum nid + mintatio + solutio
 * (parcum 01KY3D7EJP frustum B; spec = project-specs/
 * annotationes-stml-spec.md par 4)
 *
 * Usus (per silva/identitates.sh):
 *   identitates --renovare <exitus.tsv> <fons...>
 *   identitates --solvere <praefixum> <index.tsv>
 *   identitates --mintare [--scribere] <fons...>
 *
 * Recipe parsationis = examen (contextus + capita repositorii
 * praebita); sine lexico/semantica - collectio, non iudicium.
 * Mintatio: sicca ORDINARIA; --scribere splissa ordine ascendente
 * applicat, in plagulam temporalem scribit, RE-COLLIGIT et
 * verificat (petitiones -> 0, identitates auctae), tum renominat -
 * defectus = original intactum + clamor. Cusio = moneta_ulid
 * (puteus tabularii ipsius - spatium identitatis UNUM). */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "stml.h"
#include "moneta.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_contextus.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_annotationes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>


/* ==================================================
 * Lectio plagularum + praeparatio capitum (exemplar examinis)
 * ================================================== */

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
              FILE* plagula = fopen(via, "rb");
            longus  magnitudo_plagulae;
         character* textus;
    memoriae_index  lecta;

    *mensura_out = ZEPHYRUM;
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(plagula, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(plagula);
        redde NIHIL;
    }
    magnitudo_plagulae = ftell(plagula);
    si (magnitudo_plagulae < 0L)
    {
        fclose(plagula);
        redde NIHIL;
    }
    rewind(plagula);
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)magnitudo_plagulae + I);
    si (textus == NIHIL)
    {
        fclose(plagula);
        redde NIHIL;
    }
    lecta = fread(textus, I, (memoriae_index)magnitudo_plagulae,
        plagula);
    fclose(plagula);
    si (lecta != (memoriae_index)magnitudo_plagulae)
    {
        redde NIHIL;
    }
    textus[magnitudo_plagulae]  = '\0';
    *mensura_out                = (i32)magnitudo_plagulae;
    redde textus;
}

interior b32
_praetermittendum (
    constans character* titulus)
{
    redde strcmp(titulus, "vendor") == ZEPHYRUM
        || strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_caput_praebere (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via,
    constans character* titulus)
{
       chorda  clavis;
    character* textus;
          i32  mensura;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde;
    }
    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

interior vacuum
_capita_praeparare (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
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
            _capita_praeparare(ctx, piscina, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (   m >= III && introitus->d_name[m - II] == '.'
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
 * Adiutores communes
 * ================================================== */

interior constans character*
_genus_titulus (
    s32 genus)
{
    si (   genus >= ZEPHYRUM
        && (i32)genus < SILVA_C89_REGISTRUM.numerus_generum)
    {
        redde SILVA_C89_REGISTRUM.genera[genus].titulus;
    }
    redde "?";
}

interior constans character*
_modus_titulus (
    SilvaAnnotatioModus modus)
{
    commutatio (modus)
    {
        casus SILVA_ANNOTATIO_SUPRA:    redde "supra";
        casus SILVA_ANNOTATIO_INTERIOR: redde "interior";
        ordinarius:                     frange;
    }
    redde "plagula";
}

interior SilvaParsura*
_parsare (
               Piscina*  piscina,
        SilvaContextus*  ctx,
    constans character*  via,
             character** fons_out,
                   i32*  mensura_out)
{
    character* fons = _plagulam_legere(piscina, via, mensura_out);

    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    si (fons_out != NIHIL)
    {
        *fons_out = fons;
    }
    redde silva_c89_parsare_cum_contextu(piscina, ctx, via, fons,
        *mensura_out, NIHIL);
}


/* ==================================================
 * --renovare: index scribere (+ citationes res= ex arboribus -
 * frustum D: materia cruda nexuum reversorum tabulae)
 * ================================================== */

interior vacuum
_citationes_colligere (
                       FILE* cit,
         constans character* via,
    constans SilvaAnnotatio* a,
                  StmlNodus* nodus,
                        i32* summa)
{
    i32 k;

    si (nodus == NIHIL || cit == NIHIL)
    {
        redde;
    }
    si (   (s32)nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->attributa  != NIHIL)
    {
        per (k = ZEPHYRUM; k < xar_numerus(nodus->attributa); k++)
        {
            constans StmlAttributum* attr =
                (constans StmlAttributum*)xar_obtinere(
                    nodus->attributa, k);

            si (   attr != NIHIL && attr->titulus != NIHIL
                && attr->titulus->mensura == III
                && memcmp(attr->titulus->datum, "res", III)
                    == ZEPHYRUM
                && attr->valor != NIHIL
                && attr->valor->mensura > ZEPHYRUM)
            {
                fprintf(cit, "%.*s\t%s\t%u\n",
                    (int)attr->valor->mensura,
                    (constans character*)attr->valor->datum,
                    via, a->linea);
                (*summa)++;
            }
        }
    }
    si (nodus->liberi != NIHIL)
    {
        per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
        {
            _citationes_colligere(cit, via, a,
                *(StmlNodus**)xar_obtinere(nodus->liberi, k),
                summa);
        }
    }
}

interior s32
_renovare (
               Piscina*  piscina,
        SilvaContextus*  ctx,
    constans character*  exitus,
    constans character*  exitus_cit,
               integer   numerus,
             character** viae)
{
    FILE* ex = fopen(exitus, "w");
    FILE* cit = exitus_cit != NIHIL ? fopen(exitus_cit, "w")
                                    : NIHIL;
                    i32  citationes  = ZEPHYRUM;
    InternamentumChorda* intern      = internamentum_creare(piscina);
         TabulaDispersa* nides_visae = tabula_dispersa_creare_chorda(
             piscina, DXII);
        i32 summa       = ZEPHYRUM;
        i32 petitiones  = ZEPHYRUM;
        i32 duplicata   = ZEPHYRUM;
    integer f;

    si (ex == NIHIL || intern == NIHIL)
    {
        fprintf(stderr, "identitates: exitus non scribilis: %s\n",
            exitus);
        redde I;
    }
    fprintf(ex, "# identitates.tsv GENERATUM %ld - DISPONIBILE,"
        " noli committere; regenera: ./silva/identitates.sh"
        " -renovare\n", (longus)time(NIHIL));
    si (cit != NIHIL)
    {
        fprintf(cit, "# citationes.tsv GENERATUM %ld -"
            " DISPONIBILE, noli committere; regenera:"
            " ./silva/identitates.sh -renovare\n",
            (longus)time(NIHIL));
    }

    per (f = ZEPHYRUM; f < numerus; f++)
    {
                 i32  mensura = ZEPHYRUM;
        SilvaParsura* parsura = _parsare(piscina, ctx, viae[f],
            NIHIL, &mensura);
        Xar* annotationes;
        i32  k;

        si (parsura == NIHIL)
        {
            fprintf(stderr, "identitates: CAUTIO %s non parsata\n",
                viae[f]);
            perge;
        }
        annotationes = silva_annotationes_colligere(piscina,
            parsura, intern);
        si (annotationes == NIHIL)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(annotationes); k++)
        {
            constans SilvaAnnotatio* a = (constans SilvaAnnotatio*)
                xar_obtinere(annotationes, k);
            Xar* identitates;
            i32  j;

            si (a->fons_index != parsura->fons_princeps)
            {
                perge;   /* capita praebita suam vicem accipiunt */
            }
            identitates = silva_annotationes_identitates(piscina,
                a);
            si (identitates == NIHIL)
            {
                perge;   /* malformata - res lint */
            }
            _citationes_colligere(cit, viae[f], a, a->documentum,
                &citationes);
            per (j = ZEPHYRUM; j < xar_numerus(identitates); j++)
            {
                constans SilvaIdentitas* id =
                    (constans SilvaIdentitas*)xar_obtinere(
                        identitates, j);
                               s32 u_min = -I;
                               s32 u_max = -I;
                               i32 linea_unitatis   =
                                   ZEPHYRUM;
                constans character* genus_unitatis  = "-";

                si (id->petitio)
                {
                    petitiones++;
                    perge;
                }
                /* duplicata TRANS plagulas (codex 75 = plagula una;
                 * indicis vicis = veritas globalis) */
                si (nides_visae != NIHIL)
                {
                    si (tabula_dispersa_continet(nides_visae,
                            id->valor))
                    {
                        fprintf(stderr, "identitates: CAUTIO nid"
                            " duplicatum trans plagulas: %.*s"
                            " (%s:%u)\n",
                            (int)id->valor.mensura,
                            (constans character*)id->valor.datum,
                            viae[f], a->linea);
                        duplicata++;
                    }
                    alioquin
                    {
                        (vacuum)tabula_dispersa_inserere(
                            nides_visae, id->valor, NIHIL);
                    }
                }
                si (a->unitas != NIHIL)
                {
                    i32 columna_a;
                    i32 linea_b;
                    i32 columna_b;

                    /* CONTRACTUS: vocator initiat (accumulator) */
                    u_min = -I;
                    u_max = ZEPHYRUM;
                    silva_nodus_extensionem(a->unitas,
                        a->fons_index, &u_min, &u_max);
                    silva_nodus_extensionem_lineis(a->unitas,
                        a->fons_index, &linea_unitatis, &columna_a,
                        &linea_b, &columna_b);
                    genus_unitatis = _genus_titulus(
                        a->unitas->genus);
                }
                fprintf(ex,
                    "%.*s\t%s\t%u\t%s\t%.*s\t%s\t%u\t%d\t%d\t%d\t%d\n",
                    (int)id->valor.mensura,
                    (constans character*)id->valor.datum,
                    viae[f],
                    a->linea,
                    _modus_titulus(a->modus),
                    (int)(id->elementum->titulus != NIHIL
                        ? id->elementum->titulus->mensura
                        : ZEPHYRUM),
                    (constans character*)(id->elementum->titulus
                            != NIHIL
                        ? id->elementum->titulus->datum : NIHIL),
                    genus_unitatis,
                    linea_unitatis,
                    (int)a->byte_offset,
                    (int)(a->byte_offset
                        + (s32)a->crudum.mensura),
                    (int)u_min,
                    (int)u_max);
                summa++;
            }
        }
    }
    fclose(ex);
    si (cit != NIHIL)
    {
        fclose(cit);
    }
    fprintf(stderr, "identitates: %d identitates in %s",
        (int)summa, exitus);
    si (citationes > ZEPHYRUM)
    {
        fprintf(stderr, " (citationes %d)", (int)citationes);
    }
    si (petitiones > ZEPHYRUM)
    {
        fprintf(stderr, " (petitiones %d - minta per -mintare)",
            (int)petitiones);
    }
    si (duplicata > ZEPHYRUM)
    {
        fprintf(stderr, " (CAUTIO: duplicata trans plagulas %d)",
            (int)duplicata);
    }
    fprintf(stderr, "\n");
    redde ZEPHYRUM;
}


/* ==================================================
 * --solvere: praefixum -> ordo + fons
 * ================================================== */

interior vacuum
_partem_imprimere (
    constans character* via,
                longus  a,
                longus  b)
{
    FILE* plagula;
    longus k;

    si (a < 0L || b <= a)
    {
        redde;
    }
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde;
    }
    si (fseek(plagula, a, SEEK_SET) == ZEPHYRUM)
    {
        per (k = a; k < b; k++)
        {
            integer c = fgetc(plagula);

            si (c == EOF)
            {
                frange;
            }
            fputc(c, stdout);
        }
    }
    fclose(plagula);
    fputc('\n', stdout);
}

interior s32
_solvere (
    constans character* praefixum,
    constans character* index_via)
{
    FILE* index;
    character linea[4096];
    character inventa[4096];
    i32 numerus_inventarum = ZEPHYRUM;
    memoriae_index m;

    si (praefixum[ZEPHYRUM] == '#' && praefixum[I] == '#')
    {
        praefixum += II;
    }
    m = strlen(praefixum);
    si (m < VI)
    {
        fprintf(stderr, "identitates: praefixum breve (>= VI"
            " characteres): %s\n", praefixum);
        redde I;
    }
    index = fopen(index_via, "r");
    si (index == NIHIL)
    {
        fprintf(stderr, "identitates: index deest: %s (curre"
            " -renovare)\n", index_via);
        redde I;
    }
    inventa[ZEPHYRUM] = '\0';
    dum (fgets(linea, (int)magnitudo(linea), index) != NIHIL)
    {
        si (linea[ZEPHYRUM] == '#')
        {
            perge;
        }
        si (strncmp(linea, praefixum, m) == ZEPHYRUM)
        {
            si (numerus_inventarum == ZEPHYRUM)
            {
                strcpy(inventa, linea);
            }
            numerus_inventarum++;
        }
    }
    fclose(index);
    si (numerus_inventarum == ZEPHYRUM)
    {
        fprintf(stderr, "identitates: non inventum: %s\n",
            praefixum);
        redde I;
    }
    si (numerus_inventarum > I)
    {
        fprintf(stderr, "identitates: praefixum ambiguum (%d"
            " ordines): %s\n", (int)numerus_inventarum, praefixum);
        redde I;
    }
    /* ordo unus: columnae nid via linea modus elementum genus
     * linea_u a_min a_max u_min u_max */
    {
        character* partes[11];
          integer  n = ZEPHYRUM;
        character* p = inventa;

        dum (n < 11 && p != NIHIL)
        {
            partes[n] = p;
            n++;
            p = strchr(p, '\t');
            si (p != NIHIL)
            {
                *p = '\0';
                p++;
            }
        }
        si (n < 11)
        {
            fprintf(stderr, "identitates: ordo corruptus\n");
            redde I;
        }
        {
            character* caudae = strchr(partes[10], '\n');

            si (caudae != NIHIL)
            {
                *caudae = '\0';
            }
        }
        printf("%s\n%s:%s  %s  <%s>  unitas %s:%s\n",
            partes[ZEPHYRUM], partes[I], partes[II], partes[III],
            partes[IV], partes[V], partes[VI]);
        printf("--- annotatio ---\n");
        _partem_imprimere(partes[I], atol(partes[VII]),
            atol(partes[VIII]));
        si (atol(partes[IX]) >= 0L)
        {
            printf("--- unitas ---\n");
            _partem_imprimere(partes[I], atol(partes[IX]),
                atol(partes[X]));
        }
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * --mintare [--scribere]
 * ================================================== */

nomen structura {
                   s32 offset;
    SilvaInsertioGenus genus;
} Splissum;

interior s32
_mintare (
           Piscina*  piscina,
    SilvaContextus*  ctx,
               b32   scribere,
           integer   numerus,
         character** viae)
{
    InternamentumChorda* intern = internamentum_creare(piscina);
                    i32  summa_mintatae = ZEPHYRUM;
                integer  f;

    si (intern == NIHIL)
    {
        redde I;
    }
    per (f = ZEPHYRUM; f < numerus; f++)
    {
                 i32  mensura  = ZEPHYRUM;
           character* fons     = NIHIL;
        SilvaParsura* parsura = _parsare(piscina, ctx, viae[f],
            &fons, &mensura);
        Xar* annotationes;
        Xar* splissa = xar_creare(piscina,
            (i32)magnitudo(Splissum));
        i32 identitates_priores = ZEPHYRUM;
        i32 k;

        si (parsura == NIHIL || splissa == NIHIL)
        {
            fprintf(stderr, "identitates: CAUTIO %s non parsata\n",
                viae[f]);
            perge;
        }
        annotationes = silva_annotationes_colligere(piscina,
            parsura, intern);
        si (annotationes == NIHIL)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(annotationes); k++)
        {
            constans SilvaAnnotatio* a = (constans SilvaAnnotatio*)
                xar_obtinere(annotationes, k);
            Xar* identitates;
            i32  j;

            si (a->fons_index != parsura->fons_princeps)
            {
                perge;
            }
            identitates = silva_annotationes_identitates(piscina,
                a);
            si (identitates == NIHIL)
            {
                perge;
            }
            per (j = ZEPHYRUM; j < xar_numerus(identitates); j++)
            {
                constans SilvaIdentitas* id =
                    (constans SilvaIdentitas*)xar_obtinere(
                        identitates, j);

                si (!id->petitio)
                {
                    identitates_priores++;
                    perge;
                }
                si (id->insertio_offset < ZEPHYRUM)
                {
                    fprintf(stderr, "identitates: sedes incerta"
                        " in %s:%u - mintatio recusata\n",
                        viae[f], a->linea);
                    redde I;
                }
                {
                    Splissum* s = (Splissum*)xar_addere(splissa);

                    si (s != NIHIL)
                    {
                        s->offset  = id->insertio_offset;
                        s->genus   = id->insertio_genus;
                    }
                }
                si (!scribere)
                {
                    printf("%s:%u  petitio (%s)\n", viae[f],
                        a->linea,
                        id->insertio_genus
                                == SILVA_INSERTIO_POST_TITULUM
                            ? "elementum nudum" : "attributum");
                }
            }
        }
        si (xar_numerus(splissa) == ZEPHYRUM || !scribere)
        {
            perge;
        }
        /* applicare: ordine ascendente copiare + inserere */
        {
                       i32 n = xar_numerus(splissa);
            memoriae_index nova_capacitas = (memoriae_index)mensura
                + (memoriae_index)n * 40;
            character* novus = (character*)piscina_allocare(piscina,
                nova_capacitas + I);
                  s32 scripti  = ZEPHYRUM;
                  s32 prior    = ZEPHYRUM;
            character via_temp[1100];
                  i32 s_index;

            si (   novus               == NIHIL
                || strlen(viae[f]) + X >= magnitudo(via_temp))
            {
                redde I;
            }
            per (s_index = ZEPHYRUM; s_index < n; s_index++)
            {
                constans Splissum* s = (constans Splissum*)
                    xar_obtinere(splissa, s_index);
                character u[MONETA_ULID_MENSURA];
                      s32 pars = s->offset - prior;

                si (pars < ZEPHYRUM)
                {
                    fprintf(stderr, "identitates: splissa"
                        " inordinata in %s\n", viae[f]);
                    redde I;
                }
                memcpy(novus + scripti, fons + prior,
                    (memoriae_index)pars);
                scripti  += pars;
                prior    = s->offset;
                moneta_ulid(u);
                si (s->genus == SILVA_INSERTIO_POST_TITULUM)
                {
                    scripti += sprintf(novus + scripti,
                        " v=\"%s\"", u);
                }
                alioquin
                {
                    scripti += sprintf(novus + scripti,
                        "=\"%s\"", u);
                }
            }
            memcpy(novus + scripti, fons + prior,
                (memoriae_index)((s32)mensura - prior));
            scripti         += (s32)mensura - prior;
            novus[scripti]  = '\0';

            sprintf(via_temp, "%s.mintatur", viae[f]);
            {
                FILE* ex = fopen(via_temp, "wb");

                si (   ex == NIHIL
                    || fwrite(novus, I, (memoriae_index)scripti,
                           ex) != (memoriae_index)scripti)
                {
                    si (ex != NIHIL)
                    {
                        fclose(ex);
                    }
                    fprintf(stderr, "identitates: scriptio fracta:"
                        " %s\n", via_temp);
                    redde I;
                }
                fclose(ex);
            }
            /* VERIFICATIO: re-colligere - petitiones 0, identitates
             * auctae numero splissorum */
            {
                         i32  mensura_novi = ZEPHYRUM;
                SilvaParsura* parsura_novi = _parsare(piscina, ctx,
                    via_temp, NIHIL, &mensura_novi);
                Xar* annotationes_novi;
                i32  identitates_novae  = ZEPHYRUM;
                i32  petitiones_novae   = ZEPHYRUM;
                i32  kk;

                si (   parsura_novi      == NIHIL
                    || (s32)mensura_novi != scripti)
                {
                    remove(via_temp);
                    fprintf(stderr, "identitates: verificatio"
                        " fracta (parsura): %s\n", viae[f]);
                    redde I;
                }
                annotationes_novi = silva_annotationes_colligere(
                    piscina, parsura_novi, intern);
                per (kk = ZEPHYRUM; annotationes_novi != NIHIL
                    && kk < xar_numerus(annotationes_novi); kk++)
                {
                    constans SilvaAnnotatio* a =
                        (constans SilvaAnnotatio*)xar_obtinere(
                            annotationes_novi, kk);
                    Xar* ids;
                    i32  jj;

                    si (a->fons_index
                        != parsura_novi->fons_princeps)
                    {
                        perge;
                    }
                    ids = silva_annotationes_identitates(piscina,
                        a);
                    si (ids == NIHIL)
                    {
                        perge;
                    }
                    per (jj = ZEPHYRUM; jj < xar_numerus(ids); jj++)
                    {
                        constans SilvaIdentitas* id =
                            (constans SilvaIdentitas*)xar_obtinere(
                                ids, jj);

                        si (id->petitio)
                        {
                            petitiones_novae++;
                        }
                        alioquin
                        {
                            identitates_novae++;
                        }
                    }
                }
                si (   petitiones_novae != ZEPHYRUM
                    || identitates_novae != identitates_priores
                        + xar_numerus(splissa))
                {
                    remove(via_temp);
                    fprintf(stderr, "identitates: verificatio"
                        " fracta (numeri %d/%d): %s - original"
                        " intactum\n", (int)petitiones_novae,
                        (int)identitates_novae, viae[f]);
                    redde I;
                }
            }
            si (rename(via_temp, viae[f]) != ZEPHYRUM)
            {
                remove(via_temp);
                fprintf(stderr, "identitates: renominatio fracta:"
                    " %s\n", viae[f]);
                redde I;
            }
            printf("%s: %d mintatae\n", viae[f],
                (int)xar_numerus(splissa));
            summa_mintatae += xar_numerus(splissa);
        }
    }
    si (!scribere)
    {
        fprintf(stderr, "identitates: sicca (applica per"
            " -scribere)\n");
    }
    alioquin
    {
        fprintf(stderr, "identitates: %d mintatae\n",
            (int)summa_mintatae);
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * --exspecta: pinnae vectis ex annotationibus (frustum E1)
 *
 * <exspecta codex="X"/>            - proxima: linea sequens
 *   non-vacua non-pinnata flagrat (pinnae cumulantur)
 * <exspecta linea="13" codex="X"/> - absoluta (recessus)
 *
 * Emissio "linea:CODEX" per pinnam - forma quam examen_vectis.sh
 * consumit. Ancoratum malformatum aut pinna sine destinatione =
 * RECUSATIO CLAMOSA: pinna tacite lapsa est classis quam
 * grammatica haec necat.
 * ================================================== */

nomen structura {
       b32  absoluta;
       i32  linea_data;   /* absoluta: ex attributo linea= */
       i32  finis;        /* linea ultima commentarii (proxima) */
    chorda* codex;
} PinnaExspecta;

interior b32
_numerus_ex_chorda (
    constans chorda* c,
                i32* numerus_out)
{
    i32 n = ZEPHYRUM;
    i32 i;

    si (c == NIHIL || c->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c->mensura; i++)
    {
        si (c->datum[i] < '0' || c->datum[i] > '9')
        {
            redde FALSUM;
        }
        n = n * X + (i32)(c->datum[i] - '0');
    }
    *numerus_out = n;
    redde VERUM;
}

interior s32
_exspecta (
               Piscina* piscina,
        SilvaContextus* ctx,
    constans character* via)
{
             i32  mensura  = ZEPHYRUM;
       character* fons     = NIHIL;
    SilvaParsura* parsura = _parsare(piscina, ctx, via, &fons,
        &mensura);
    Xar* annotationes;
    Xar* pinnae;
    b32* vacuae;     /* [1..numerus_linearum] sola spatia/tabs */
    b32* pinnatae;   /* [1..numerus_linearum] pinnam ferentes */
    i32  numerus_linearum;
    i32  i;
    i32  l;
    b32  vacua_currens;
    i32  k;

    si (parsura == NIHIL)
    {
        fprintf(stderr, "identitates --exspecta: %s non legibilis"
            " aut non parsata\n", via);
        redde I;
    }
    numerus_linearum = I;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (fons[i] == '\n')
        {
            numerus_linearum++;
        }
    }
    vacuae = (b32*)piscina_allocare(piscina,
        (memoriae_index)(numerus_linearum + II) * magnitudo(b32));
    pinnatae = (b32*)piscina_allocare(piscina,
        (memoriae_index)(numerus_linearum + II) * magnitudo(b32));
    pinnae = xar_creare(piscina, magnitudo(PinnaExspecta));
    si (vacuae == NIHIL || pinnatae == NIHIL || pinnae == NIHIL)
    {
        fprintf(stderr, "identitates --exspecta: memoria deest\n");
        redde I;
    }
    memset(pinnatae, ZEPHYRUM,
        (memoriae_index)(numerus_linearum + II) * magnitudo(b32));
    l              = I;
    vacua_currens  = VERUM;
    per (i = ZEPHYRUM; i <= mensura; i++)
    {
        si (i == mensura || fons[i] == '\n')
        {
            vacuae[l] = vacua_currens;
            l++;
            vacua_currens = VERUM;
        }
        alioquin si (fons[i] != ' ' && fons[i] != '\t')
        {
            vacua_currens = FALSUM;
        }
    }

    annotationes = silva_annotationes_colligere(piscina, parsura,
        NIHIL);
    per (k = ZEPHYRUM;
         annotationes != NIHIL && k < xar_numerus(annotationes);
         k++)
    {
        constans SilvaAnnotatio* a = (constans SilvaAnnotatio*)
            xar_obtinere(annotationes, k);
        i32 finis;
        i32 j;

        si (a->fons_index != parsura->fons_princeps)
        {
            perge;
        }
        si (!a->parsata)
        {
            fprintf(stderr, "identitates --exspecta: ANNOTATIO"
                " MALFORMATA %s:%u - pinna fortasse tacite"
                " lapsa\n", via, a->linea);
            redde I;
        }
        finis = a->linea;
        per (j = ZEPHYRUM; j < a->crudum.mensura; j++)
        {
            si (a->crudum.datum[j] == '\n')
            {
                finis++;
            }
        }
        per (j = ZEPHYRUM;
             a->documentum != NIHIL
                 && a->documentum->liberi != NIHIL
                 && j < xar_numerus(a->documentum->liberi);
             j++)
        {
            StmlNodus* nodus = *(StmlNodus**)xar_obtinere(
                a->documentum->liberi, j);
            PinnaExspecta* pinna;
                   chorda* codex;
                   chorda* linea_attr;

            si (   nodus                   == NIHIL
                || (s32)nodus->genus       != STML_NODUS_ELEMENTUM
                || nodus->titulus          == NIHIL
                || nodus->titulus->mensura != VIII
                || memcmp(nodus->titulus->datum, "exspecta", VIII)
                    != ZEPHYRUM)
            {
                perge;
            }
            codex = stml_attributum_capere(nodus, "codex");
            si (codex == NIHIL || codex->mensura == ZEPHYRUM)
            {
                fprintf(stderr, "identitates --exspecta: exspecta"
                    " sine codice %s:%u\n", via, a->linea);
                redde I;
            }
            pinna = (PinnaExspecta*)xar_addere(pinnae);
            si (pinna == NIHIL)
            {
                fprintf(stderr, "identitates --exspecta: memoria"
                    " deest\n");
                redde I;
            }
            pinna->codex  = codex;
            pinna->finis  = finis;
            linea_attr    = stml_attributum_capere(nodus, "linea");
            si (linea_attr != NIHIL)
            {
                si (!_numerus_ex_chorda(linea_attr,
                        &pinna->linea_data))
                {
                    fprintf(stderr, "identitates --exspecta:"
                        " linea non numerica %s:%u\n", via,
                        a->linea);
                    redde I;
                }
                pinna->absoluta = VERUM;
            }
            alioquin
            {
                pinna->absoluta    = FALSUM;
                pinna->linea_data  = ZEPHYRUM;
            }
            per (l = a->linea;
                 l <= finis && l <= numerus_linearum; l++)
            {
                pinnatae[l] = VERUM;
            }
        }
    }

    per (k = ZEPHYRUM; k < xar_numerus(pinnae); k++)
    {
        constans PinnaExspecta* pinna = (constans PinnaExspecta*)
            xar_obtinere(pinnae, k);

        si (pinna->absoluta)
        {
            imprimere("%u:%.*s\n", pinna->linea_data,
                (int)pinna->codex->mensura,
                (constans character*)pinna->codex->datum);
            perge;
        }
        l = pinna->finis + I;
        dum (l <= numerus_linearum && (vacuae[l] || pinnatae[l]))
        {
            l++;
        }
        si (l > numerus_linearum)
        {
            fprintf(stderr, "identitates --exspecta: pinna sine"
                " linea destinata %s (finis %u)\n", via,
                pinna->finis);
            redde I;
        }
        imprimere("%u:%.*s\n", l,
            (int)pinna->codex->mensura,
            (constans character*)pinna->codex->datum);
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * principale
 * ================================================== */

s32
principale (
      integer   argc,
    character** argv)
{
           Piscina* piscina;
    SilvaContextus* ctx;

    si (   argc                         >= IV
        && strcmp(argv[I], "--solvere") == ZEPHYRUM)
    {
        redde _solvere(argv[II], argv[III]);
    }
    si (argc < III)
    {
        fprintf(stderr, "usus: identitates --renovare <ex.tsv>"
            " <fons...> | --solvere <praefixum> <index.tsv> |"
            " --mintare [--scribere] <fons...>\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("identitates", 536870912);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "identitates: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "identitates: contextus deest\n");
        redde II;
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina, DXII);

        si (visa != NIHIL)
        {
            _capita_praeparare(ctx, piscina, visa, ".");
        }
    }
    si (strcmp(argv[I], "--renovare") == ZEPHYRUM)
    {
        si (argc < V)
        {
            fprintf(stderr, "identitates: --renovare <index.tsv>"
                " <citationes.tsv> <fons...>\n");
            redde II;
        }
        redde _renovare(piscina, ctx, argv[II], argv[III],
            argc - IV, argv + IV);
    }
    si (argc == III && strcmp(argv[I], "--exspecta") == ZEPHYRUM)
    {
        redde _exspecta(piscina, ctx, argv[II]);
    }
    si (strcmp(argv[I], "--mintare") == ZEPHYRUM)
    {
        b32 scribere = argc >= III
            && strcmp(argv[II], "--scribere") == ZEPHYRUM;
        integer initium = scribere ? III : II;

        si (argc <= initium)
        {
            fprintf(stderr, "identitates: fontes desunt\n");
            redde II;
        }
        redde _mintare(piscina, ctx, scribere, argc - initium,
            argv + initium);
    }
    fprintf(stderr, "identitates: optio ignota: %s\n", argv[I]);
    redde II;
}
