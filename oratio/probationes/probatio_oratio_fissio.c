/* probatio_oratio_fissio.c - sententia_fissio contra oratio (T6)
 *
 * MENSURA, non pinna: initia sententiarum utriusque (post spatia
 * ducentia normata) super fixturas fixa/txt et paragraphos md
 * (specimen quinta quaeque plagula) conferuntur: communia, solum
 * fissionis, solum orationis; discrepantiae primae X cum via, linea et
 * contextu impressae, numeri publicati. Porta asserit utrumque
 * cucurrisse super > M sententias et concordiam > dimidium (limen
 * sanitatis, non pinna) - quid pinnetur post relationem lectam
 * decernitur (oratio-spec par. III: fissio mensurata, deinde
 * substituta post API eandem).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_sententiae.h"
#include "sententia_fissio.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
    s64 fissionis;
    s64 orationis;
    s64 communia;
    s64 solum_fissionis;
    s64 solum_orationis;
    i32 impressae;
} Collatio;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

interior s32
_normare (
    constans character* fons,
                   s32  i,
                   s32  ad)
{
    dum (i < ad && (fons[i] == ' ' || fons[i] == '\t' || fons[i] == '\n'
                    || fons[i] == '\r'))
    {
        i = i + I;
    }
    redde i;
}

interior i32
_linea_ad (
    constans character* fons,
                   s32  i)
{
    i32 l = I;
    s32 k;

    per (k = ZEPHYRUM; k < i; k++)
    {
        si (fons[k] == '\n')
        {
            l = l + I;
        }
    }
    redde l;
}

interior vacuum
_discrepantiam_imprimere (
    constans character* titulus,
    constans character* quis,
    constans character* fons,
                   i32  mensura,
                   s32  i,
              Collatio* c)
{
    i32 l;

    si (c->impressae >= X)
    {
        redde;
    }
    c->impressae++;
    l = ((s32)mensura - i) < (s32)XL ? (i32)((s32)mensura
        - i) : (i32)XL;
    imprimere("    %s:%d %s: \"%.*s\"\n", titulus,
        (integer)_linea_ad(fons, i),
        quis, (integer)l, fons + i);
}

/* initia utriusque conferre (ambo ordinati crescentes) */
interior vacuum
_conferre (
    constans character* titulus,
    constans character* fons,
                   i32  mensura,
               Piscina* piscina,
              Collatio* c)
{
    SententiaIndicesFructus f;
    MateriaNodus* radix;
    Xar* xs;
    chorda textus;
    i32 na;
    i32 nb;
    i32 a = ZEPHYRUM;
    i32 b = ZEPHYRUM;
    unio { constans character* c; i8* m; } u;

    u.c             = fons;
    textus.datum    = u.m;
    textus.mensura  = mensura;
    f               = sententia_fissio_indices(textus, piscina);
    radix           = oratio_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        redde;
    }
    xs = oratio_sententiae_extenta(piscina, radix);
    si (xs == NIHIL)
    {
        redde;
    }
    na            = f.numerus;
    nb            = xar_numerus(xs);
    c->fissionis  = c->fissionis + na;
    c->orationis  = c->orationis + nb;
    dum (a < na || b < nb)
    {
        s32 ia = a < na ? _normare(fons, (s32)f.indices[a].initium,
            (s32)mensura) : (s32)mensura + I;
        s32 ib = b < nb ? _normare(fons,
            ((OratioSententia*)xar_obtinere(xs, b))->initium,
            (s32)mensura) : (s32)mensura + I;

        si (ia == ib)
        {
            c->communia++;
            a++;
            b++;
        }
        alioquin si (ia < ib)
        {
            c->solum_fissionis++;
            _discrepantiam_imprimere(titulus, "fissio sola", fons,
                mensura, ia, c);
            a++;
        }
        alioquin
        {
            c->solum_orationis++;
            _discrepantiam_imprimere(titulus, "oratio sola", fons,
                mensura, ib, c);
            b++;
        }
    }
}

/* paragraphi md: extentum per arborem md (ut probatio_oratio_arbor) */
interior vacuum
_extentum (
    constans MateriaNodus* n,
                      s32* initium,
                      s32* finis)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];
                          i32  m;
                          i32  j;

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _extentum(v->datum.nodus, initium, finis);
            perge;
        }
        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            constans MateriaToken* t = v->datum.token;

            si (   t->fons_index  == MD_FONS_PLAGULAE
                && t->byte_offset >= ZEPHYRUM)
            {
                si (*initium < ZEPHYRUM || t->byte_offset < *initium)
                {
                    *initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->valor.mensura > *finis)
                {
                    *finis = t->byte_offset + (s32)t->valor.mensura;
                }
            }
            perge;
        }
        si (v->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        m = materia_valor_lista_numerus(*v);
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(*v,
                j);

            si (e == NIHIL)
            {
                perge;
            }
            si (e->genus == MATERIA_VALOR_NODUS)
            {
                _extentum(e->datum.nodus, initium, finis);
            }
            alioquin si (   e->genus == MATERIA_VALOR_TOKEN
                         && e->datum.token->fons_index
                             == MD_FONS_PLAGULAE
                         && e->datum.token->byte_offset >= ZEPHYRUM)
            {
                constans MateriaToken* t = e->datum.token;

                si (*initium < ZEPHYRUM || t->byte_offset < *initium)
                {
                    *initium = t->byte_offset;
                }
                si (t->byte_offset + (s32)t->valor.mensura > *finis)
                {
                    *finis = t->byte_offset + (s32)t->valor.mensura;
                }
            }
        }
    }
}

interior vacuum
_paragraphos_conferre (
       constans character* titulus,
    constans MateriaNodus* n,
       constans character* fons,
                  Piscina* piscina,
                 Collatio* c)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    si (n->genus == (s32)MD_GENUS_PARAGRAPHUS)
    {
        s32 a = (s32)-I;
        s32 b = ZEPHYRUM;

        _extentum(n, &a, &b);
        si (a >= ZEPHYRUM && b > a)
        {
            _conferre(titulus, fons + a, (i32)(b - a), piscina, c);
        }
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _paragraphos_conferre(titulus, v->datum.nodus, fons,
                piscina, c);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    _paragraphos_conferre(titulus, e->datum.nodus, fons,
                        piscina, c);
                }
            }
        }
    }
}

hic_manens constans character* FIXTURAE[] = {
    "oratio/probationes/fixa/txt/hilarius.txt",
    "oratio/probationes/fixa/txt/propertius.txt",
    "oratio/probationes/fixa/txt/cicero.txt",
    "oratio/probationes/fixa/txt/lincoln.txt",
    "oratio/probationes/fixa/txt/trinity.txt"
};

s32
principale (vacuum)
{
               Piscina* piscina;
    constans character* radix_viae;
              Collatio  c;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_fissio",
        8388608);
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    memset(&c, ZEPHYRUM, magnitudo(c));

    imprimere("\n--- Collatio super fixturas (fixa/txt) ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXTURAE)
        / magnitudo(FIXTURAE[0])); i++)
    {
        character  via[DXII];
          Piscina* p = piscina_generare_dynamicum("fissio_fixtura",
              33554432);
        character* textus;
              i32  mensura = ZEPHYRUM;
         Collatio  una;

        memset(&una, ZEPHYRUM, magnitudo(una));
        sprintf(via, "%s/%s", radix_viae, FIXTURAE[i]);
        textus = _plagulam_legere(p, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus != NIHIL)
        {
            _conferre(FIXTURAE[i], textus, mensura, p, &una);
            imprimere("  %-44s fissio %4ld  oratio %4ld  communia %4ld  sola f %3ld  sola o %3ld\n",
                FIXTURAE[i], (longus)una.fissionis,
                (longus)una.orationis,
                (longus)una.communia, (longus)una.solum_fissionis,
                (longus)una.solum_orationis);
            c.fissionis        += una.fissionis;
            c.orationis        += una.orationis;
            c.communia         += una.communia;
            c.solum_fissionis  += una.solum_fissionis;
            c.solum_orationis  += una.solum_orationis;
        }
        piscina_destruere(p);
    }

    imprimere("\n--- Collatio super paragraphos md (specimen quinta quaeque) ---\n");
    {
        character via[DXII];
        FILE* lista;
        i32 numerus_lineae = ZEPHYRUM;
        i32 plagulae = ZEPHYRUM;
        Collatio md;

        memset(&md, ZEPHYRUM, magnitudo(md));
        md.impressae = X;   /* discrepantiae md non imprimuntur (fixturae satis) */
        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                  Piscina* p;
                character* textus;
                      i32  mensura  = ZEPHYRUM;
                   size_t  l        = strlen(linea);
                MateriaNodus* arbor;

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                numerus_lineae++;
                si (   getenv("ORATIO_CORPUS_TOTUS") == NIHIL
                    && (numerus_lineae % V)          != ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("fissio_corpus",
                    33554432);
                sprintf(via, "%s/%s", radix_viae, linea);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus != NIHIL)
                {
                    plagulae++;
                    arbor = md_arbor_parsare(p, textus, mensura);
                    _paragraphos_conferre(linea, arbor, textus, p, &md);
                }
                piscina_destruere(p);
            }
            fclose(lista);
        }
        imprimere("  plagulae %d: fissio %ld  oratio %ld  communia %ld  sola f %ld  sola o %ld\n",
            (integer)plagulae, (longus)md.fissionis,
            (longus)md.orationis,
            (longus)md.communia, (longus)md.solum_fissionis,
            (longus)md.solum_orationis);
        c.fissionis        += md.fissionis;
        c.orationis        += md.orationis;
        c.communia         += md.communia;
        c.solum_fissionis  += md.solum_fissionis;
        c.solum_orationis  += md.solum_orationis;
    }

    imprimere("\n--- SUMMA: fissio %ld, oratio %ld, communia %ld (%.1f%% fissionis, %.1f%% orationis),"
              " sola fissionis %ld, sola orationis %ld ---\n",
        (longus)c.fissionis, (longus)c.orationis, (longus)c.communia,
        c.fissionis > ZEPHYRUM ? 100.0 * (duplex)c.communia
            / (duplex)c.fissionis : 0.0,
        c.orationis > ZEPHYRUM ? 100.0 * (duplex)c.communia
            / (duplex)c.orationis : 0.0,
        (longus)c.solum_fissionis, (longus)c.solum_orationis);
    CREDO_VERUM (c.fissionis > (s64)1000 && c.orationis > (s64)1000);
    CREDO_VERUM (c.communia * (s64)II > c.fissionis);
    CREDO_VERUM (c.communia * (s64)II > c.orationis);

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
