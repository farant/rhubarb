/* oratio_resolutio.c - Resolutio ambiguitatis per contextum (T17) */

#include "oratio_resolutio.h"
#include "oratio_registrum.h"
#include "oratio_stml.h"
#include "oratio_partes.h"
#include "materia_arbor.h"
#include "stml.h"
#include "stml_macros.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                       Piscina* piscina;
           InternamentumChorda* intern;
  constans MateriaLexiconRatum* ratum;
      constans OratioProgramma* programma;
                           s32  regulae_numerus;
            constans character* lingua;
         OratioResolutioCensus* census;
} Cursus;

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura == l
        && (l == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM));
}

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    f                = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde FALSUM;
    }
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = (i8*)memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

/* liberi elementi titulo dato ordine (Xar de StmlNodus*) */
interior Xar*
_liberi_titulo (
               Piscina* piscina,
             StmlNodus* nodus,
    constans character* titulus)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    i32  k;

    si (exitus == NIHIL || nodus == NIHIL || nodus->liberi == NIHIL)
    {
        redde exitus;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(nodus->liberi, k);

        si (   l          != NIHIL && l->genus == STML_NODUS_ELEMENTUM
            && l->titulus != NIHIL && _aequalis(*l->titulus, titulus))
        {
            StmlNodus** s = (StmlNodus**)xar_addere(exitus);

            si (s == NIHIL)
            {
                redde NIHIL;
            }
            *s = l;
        }
    }
    redde exitus;
}

OratioProgramma*
oratio_resolutio_programma_legere (
                     Piscina* piscina,
         InternamentumChorda* intern,
                      chorda  textus,
    OratioVocabulariumVitium* vitium)
{
     OratioProgramma* p;
        StmlResultus  lectio;
                 Xar* regulae;
                 i32  k;

    memset(vitium, ZEPHYRUM, magnitudo(*vitium));
    vitium->plagula = "resolutio.stml";
    p = (OratioProgramma*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(*p));
    si (p == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    p->textus   = _copia(piscina, textus);
    p->regulae  = xar_creare(piscina, (i32)magnitudo(OratioRegula));
    si (p->textus.datum == NIHIL || p->regulae == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    lectio = stml_legere(p->textus, piscina, intern);
    si (!lectio.successus || lectio.radix == NIHIL)
    {
        vitium->linea = lectio.linea_erroris;
        vitium->causa = "programma non legitur";
        redde NIHIL;
    }
    regulae = _liberi_titulo(piscina, lectio.radix, "regula");
    si (regulae == NIHIL)
    {
        vitium->causa = "memoria";
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
           StmlNodus* n        = *(StmlNodus**)xar_obtinere(regulae, k);
              chorda* titulus  = stml_attributum_capere(n, "titulus");
        OratioRegula* r;

        si (titulus == NIHIL || titulus->mensura == ZEPHYRUM)
        {
            vitium->linea = n->linea;
            vitium->causa = "regula sine titulo";
            redde NIHIL;
        }
        si (   n->positus_finis <= n->positus_initium
            || n->positus_finis > p->textus.mensura)
        {
            vitium->linea = n->linea;
            vitium->causa = "regula sine extento";
            redde NIHIL;
        }
        r = (OratioRegula*)xar_addere(p->regulae);
        si (r == NIHIL)
        {
            vitium->causa = "memoria";
            redde NIHIL;
        }
        r->titulus  = _copia(piscina, *titulus);
        r->textus   = _chorda(p->textus.datum + n->positus_initium,
            n->positus_finis - n->positus_initium);
    }
    redde p;
}

OratioProgramma*
oratio_resolutio_programma_onerare (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* radix,
    OratioVocabulariumVitium* vitium)
{
    character via[1024];
       chorda fons;

    si (radix == NIHIL)
    {
        radix = ".";
    }
    sprintf(via, "%s/oratio/partes/resolutio.stml", radix);
    si (!_plagulam_legere(piscina, via, &fons))
    {
        memset(vitium, ZEPHYRUM, magnitudo(*vitium));
        vitium->plagula  = "oratio/partes/resolutio.stml";
        vitium->causa    = "plagula absens";
        redde NIHIL;
    }
    redde oratio_resolutio_programma_legere(piscina, intern, fons,
        vitium);
}

vacuum
oratio_resolutio_census_vacare (
    OratioResolutioCensus* census)
{
    memset(census, ZEPHYRUM, magnitudo(*census));
}

constans character*
oratio_resolutio_lingua_censu (
    constans i32* linguae)
{
    i32 k;
    i32 optima = ZEPHYRUM;

    per (k = I; k < (i32)ORATIO_LINGUA_NUMERUS; k++)
    {
        si (linguae[k] > linguae[optima])
        {
            optima = k;
        }
    }
    redde ORATIO_TITULI_LINGUARUM[optima];
}

/* regulae numerum in censu augere (titulus copiatur in piscina) */
interior b32
_regulam_numerare (
    Cursus* cursus,
    chorda  titulus)
{
          OratioResolutioCensus* census = cursus->census;
                            i32  k;
    OratioResolutioRegulaCensus* r;

    si (census == NIHIL)
    {
        redde VERUM;
    }
    si (census->per_regulam == NIHIL)
    {
        census->per_regulam = xar_creare(cursus->piscina,
            (i32)magnitudo(OratioResolutioRegulaCensus));
        si (census->per_regulam == NIHIL)
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(census->per_regulam); k++)
    {
        r =
            (OratioResolutioRegulaCensus*)xar_obtinere(
                census->per_regulam, k);
        si (   r->titulus.mensura == titulus.mensura
            && memcmp(r->titulus.datum, titulus.datum,
                (size_t)titulus.mensura) == ZEPHYRUM)
        {
            r->numerus = r->numerus + I;
            redde VERUM;
        }
    }
    r = (OratioResolutioRegulaCensus*)xar_addere(census->per_regulam);
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    r->titulus = _copia(cursus->piscina, titulus);
    r->numerus = I;
    redde VERUM;
}

interior b32
_numerus_attributi (
             StmlNodus* nodus,
    constans character* titulus,
                   i32* exitus)
{
    chorda* v = stml_attributum_capere(nodus, titulus);

    si (v == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_arbor_numerus_ex_chorda(v, exitus);
}

/* sententiam unam resolvere: proiectio + programma -> expansio ->
 * consilia -> permutationes. FALSUM = memoria sola. */
interior b32
_sententiam_resolvere (
          Cursus* cursus,
    MateriaNodus* sententia)
{
                      Piscina* scratch;
            OratioStmlOrnatus  ornatus;
        MateriaArborConsilium  consilium;
        MateriaArborScriptura  scriptura;
        constans MateriaValor* elementa =
            &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                          i32  ne;
                          i32  regulae_numerus;
                          i32  mensura;
                          i32  k;
                    character* textus;
                       chorda  fons;
                 StmlResultus  lectio;
         StmlExpansioResultus  expansio;
                          Xar* regulae;
                          s32* praelata;

    si (cursus->census != NIHIL)
    {
        cursus->census->sententiae = cursus->census->sententiae + I;
    }
    si (   elementa->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*elementa) == ZEPHYRUM)
    {
        redde VERUM;
    }
    ne               = materia_valor_lista_numerus(*elementa);
    regulae_numerus  = xar_numerus(cursus->programma->regulae);
    si (   cursus->regulae_numerus >= ZEPHYRUM
        && cursus->regulae_numerus < (s32)regulae_numerus)
    {
        regulae_numerus = (i32)cursus->regulae_numerus;
    }
    si (regulae_numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    scratch = piscina_generare_dynamicum("oratio_resolutio_sententia",
        16777216);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    oratio_stml_consilium_ornatum(&consilium, cursus->ratum, &ornatus,
        cursus->lingua);
    scriptura = materia_arbor_scribere_nodum(scratch, sententia,
        &consilium);
    si (!scriptura.successus)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
    /* compositio: proiectio + regulae primae N, lineis novis divisae */
    mensura = scriptura.textus.mensura + I;
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        mensura = mensura + ((constans OratioRegula*)xar_obtinere(
                cursus->programma->regulae, k))->textus.mensura + I;
    }
    textus = (character*)piscina_allocare(scratch,
        (memoriae_index)mensura
        + I);
    si (textus == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    memcpy(textus, scriptura.textus.datum,
        (size_t)scriptura.textus.mensura);
    mensura          = scriptura.textus.mensura;
    textus[mensura]  = '\n';
    mensura          = mensura + I;
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        memcpy(textus + mensura, r->textus.datum,
            (size_t)r->textus.mensura);
        mensura          = mensura + r->textus.mensura;
        textus[mensura]  = '\n';
        mensura          = mensura + I;
    }
    textus[mensura]  = ZEPHYRUM;
    fons             = _chorda((i8*)textus, mensura);
    lectio           = stml_legere(fons, scratch, cursus->intern);
    si (!lectio.successus || lectio.radix == NIHIL)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
    expansio = stml_expandere(lectio.radix, scratch, cursus->intern);
    si (!expansio.successus || expansio.radix_expansa == NIHIL)
    {
        si (cursus->census != NIHIL)
        {
            cursus->census->fractae = cursus->census->fractae + I;
        }
        piscina_destruere(scratch);
        redde VERUM;
    }
    regulae = _liberi_titulo(scratch, expansio.radix_expansa, "regula");
    praelata = (s32*)piscina_allocare(scratch, (memoriae_index)ne
        * (memoriae_index)magnitudo(s32));
    si (regulae == NIHIL || praelata == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        praelata[k] = (s32)-I;
    }
    /* ordines consilii cuiusque regulae ordine programmatis: prima
     * vincit */
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
        StmlNodus* regula = *(StmlNodus**)xar_obtinere(regulae, k);
        StmlNodus* consilium_nodus = stml_invenire_liberum(regula,
            "consilium");
           chorda* titulus = stml_attributum_capere(regula, "titulus");
              Xar* ordines;
              i32  o;

        si (consilium_nodus == NIHIL)
        {
            perge;
        }
        ordines = _liberi_titulo(scratch, consilium_nodus, "praelatio");
        si (ordines == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (o = ZEPHYRUM; o < xar_numerus(ordines); o++)
        {
                        StmlNodus* ordo;
                              i32  v;
                              i32  a;
            constans MateriaNodus* vocabulum;
            constans MateriaValor* analyses;

            ordo = *(StmlNodus**)xar_obtinere(ordines, o);
            si (   !_numerus_attributi(ordo, "vocabulum", &v)
                || !_numerus_attributi(ordo, "analysis", &a)
                || v >= ne)
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            vocabulum = materia_valor_lista_obtinere(*elementa,
                v)->datum.nodus;
            analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
            si (   vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM
                || analyses->genus != MATERIA_VALOR_LISTA
                || a >= materia_valor_lista_numerus(*analyses))
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->recusatae =
                        cursus->census->recusatae + I;
                }
                perge;
            }
            si (cursus->census != NIHIL)
            {
                cursus->census->ordines = cursus->census->ordines + I;
            }
            si (praelata[v] >= ZEPHYRUM)
            {
                si (cursus->census != NIHIL)
                {
                    cursus->census->repetitae =
                        cursus->census->repetitae + I;
                }
                perge;
            }
            praelata[v] = (s32)a;
            si (   titulus != NIHIL
                && !_regulam_numerare(cursus, *titulus))
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
        }
    }
    /* permutationes: analysis praelata prima, ceterae ordine suo */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        MateriaNodus* vocabulum;
        constans MateriaValor* analyses;
                          i32  n;
                          i32* ordo;
                          i32  i;
                          i32  j;

        si (praelata[k] <= ZEPHYRUM)
        {
            perge;   /* nihil aut iam prima */
        }
        vocabulum = materia_valor_lista_obtinere(*elementa,
            k)->datum.nodus;
        analyses  = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
        n         = materia_valor_lista_numerus(*analyses);
        ordo      = (i32*)piscina_allocare(scratch, (memoriae_index)n
            * (memoriae_index)magnitudo(i32));
        si (ordo == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        ordo[ZEPHYRUM]  = (i32)praelata[k];
        j               = I;
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (i != (i32)praelata[k])
            {
                ordo[j]  = i;
                j        = j + I;
            }
        }
        si (   !materia_nodus_lista_permutare(cursus->piscina,
            vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES, ordo, n)
            || !oratio_partes_compendia_reponere(cursus->piscina,
                vocabulum))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        si (cursus->census != NIHIL)
        {
            cursus->census->applicatae = cursus->census->applicatae + I;
        }
    }
    piscina_destruere(scratch);
    redde VERUM;
}

interior b32
_sententias_resolvere (
          Cursus* cursus,
    MateriaNodus* nodus)
{
    constans MateriaValor* lista;
                      i32  n;
                      i32  k;

    si (nodus->genus == (s32)ORATIO_GENUS_SENTENTIA)
    {
        redde _sententiam_resolvere(cursus, nodus);
    }
    si (nodus->genus == (s32)ORATIO_GENUS_DOCUMENTUM)
    {
        lista = &nodus->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    }
    alioquin si (nodus->genus == (s32)ORATIO_GENUS_PARAGRAPHUS)
    {
        lista = &nodus->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
    }
    alioquin
    {
        redde VERUM;
    }
    si (lista->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(*lista);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*lista,
            k);

        si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
            && !_sententias_resolvere(cursus, e->datum.nodus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
oratio_resolutio_applicare (
                       Piscina* piscina,
           InternamentumChorda* intern,
  constans MateriaLexiconRatum* ratum,
      constans OratioProgramma* programma,
                           s32  regulae_numerus,
            constans character* lingua,
                  MateriaNodus* radix,
         OratioResolutioCensus* census)
{
    Cursus cursus;

    si (programma == NIHIL || radix == NIHIL)
    {
        redde VERUM;
    }
    cursus.piscina          = piscina;
    cursus.intern           = intern;
    cursus.ratum            = ratum;
    cursus.programma        = programma;
    cursus.regulae_numerus  = regulae_numerus;
    cursus.lingua           = lingua;
    cursus.census           = census;
    redde _sententias_resolvere(&cursus, radix);
}
