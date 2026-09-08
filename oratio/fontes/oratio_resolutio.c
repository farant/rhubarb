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

interior b32
_numerus_attributi (
             StmlNodus* nodus,
    constans character* titulus,
                   i32* exitus);

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
        /* gradus (T19d beta): attributum 'gradus' (>= I), absens = I */
        {
            i32 gradus;

            r->gradus = _numerus_attributi(n, "gradus", &gradus)
                && gradus > ZEPHYRUM ? gradus : I;
        }
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

i32
oratio_resolutio_lingua_censu_index (
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
    redde optima;
}

constans character*
oratio_resolutio_lingua_censu (
    constans i32* linguae)
{
    redde ORATIO_TITULI_LINGUARUM[
        oratio_resolutio_lingua_censu_index(linguae)];
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

/* IMPLETIO (T19d): umbra ligandum post permutationes - umbra nodus
 * (stabilis per permutationem listae), vocabulum implens w, analysis
 * implens b (index ANTE permutationem; remittitur) */
nomen structura {
    MateriaNodus* umbra;
             i32  w;
             i32  b;
} Impletio;

/* analysis 'a' vocabuli 'v' intra elementa adest? */
interior b32
_analysis_adest (
    MateriaValor elementa,
             i32 v,
             i32 a)
{
    constans MateriaNodus* vocabulum =
        materia_valor_lista_obtinere(elementa, v)->datum.nodus;
    constans MateriaValor* analyses;

    si (vocabulum->genus != (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde FALSUM;
    }
    analyses = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    redde analyses->genus == MATERIA_VALOR_LISTA
        && a < materia_valor_lista_numerus(*analyses);
}

/* umbra 'u' analysis 'a' vocabuli 'v' (locus 'umbrae' per titulum
 * classis analysis); NIHIL si absens */
interior MateriaNodus*
_umbram_invenire (
    MateriaValor elementa,
             i32 v,
             i32 a,
             i32 u)
{
    constans MateriaNodus* vocabulum;
    constans MateriaValor* analyses;
    constans MateriaValor* analysis_valor;
             MateriaNodus* analysis;
                      s32  locus;
    constans MateriaValor* umbrae;
    constans MateriaValor* umbra_valor;

    si (!_analysis_adest(elementa, v, a))
    {
        redde NIHIL;
    }
    vocabulum = materia_valor_lista_obtinere(elementa,
        v)->datum.nodus;
    analyses        = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    analysis_valor  = materia_valor_lista_obtinere(*analyses, a);
    si (   analysis_valor        == NIHIL
        || analysis_valor->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    analysis = analysis_valor->datum.nodus;
    locus    = oratio_partes_locus(oratio_genus_classis(
        (OratioGenus)analysis->genus), "umbrae");
    si (locus < ZEPHYRUM || (i32)locus >= analysis->numerus_locorum)
    {
        redde NIHIL;
    }
    umbrae = &analysis->loci[locus];
    si (   umbrae->genus != MATERIA_VALOR_LISTA
        || u             >= materia_valor_lista_numerus(*umbrae))
    {
        redde NIHIL;
    }
    umbra_valor = materia_valor_lista_obtinere(*umbrae, u);
    si (   umbra_valor        == NIHIL
        || umbra_valor->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde umbra_valor->datum.nodus;
}

/* sententiam unam resolvere GRADU uno: proiectio + regulae gradus ->
 * expansio -> consilia -> permutationes -> ligationes. vindicata =
 * vocabula gradibus prioribus vindicata (prima vincit TRANS gradus:
 * ordines in ea repetitae); post gradum vocabula praelata vindicantur.
 * FALSUM = memoria sola. */
interior b32
_sententiam_resolvere_gradu (
          Cursus* cursus,
    MateriaNodus* sententia,
             i32  gradus,
             b32* vindicata,
             b32* explicita)
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
                          Xar* impletiones;   /* Impletio (T19d) */

        si (   elementa->genus != MATERIA_VALOR_LISTA
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
        /* compositio: proiectio + regulae primae N HUIUS gradus, lineis
     * novis divisae */
    mensura = scriptura.textus.mensura + I;
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        si (r->gradus != gradus)
        {
            perge;
        }
        mensura = mensura + r->textus.mensura + I;
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

        si (r->gradus != gradus)
        {
            perge;
        }
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
    impletiones = xar_creare(scratch, (i32)magnitudo(Impletio));
    si (regulae == NIHIL || praelata == NIHIL || impletiones == NIHIL)
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
                        si (praelata[v] >= ZEPHYRUM || vindicata[v])
                        {
                si (cursus->census != NIHIL)
                {
                    cursus->census->repetitae =
                        cursus->census->repetitae + I;
                }
                perge;
                        }
            praelata[v]   = (s32)a;
            explicita[v]  = VERUM;   /* decisio regulae */

            si (   titulus != NIHIL
                && !_regulam_numerare(cursus, *titulus))
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
        }
        /* IMPLETIONES (T19d): umbra (vocabulum v, analysis a, umbra u)
         * a lectione (ad-vocabulum w, ad-analysis b) impletur. Ordo
         * validus notatur (ligatio POST permutationes, indicibus
         * remissis) et utramque lectionem praefert - carrier v/a et
         * implens w/b - prima regula per vocabulum vincente ut
         * praelatio. Ordo malus recusatur. */
        ordines = _liberi_titulo(scratch, consilium_nodus, "impletio");
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
                              i32  u;
                              i32  w;
                              i32  b;
                     MateriaNodus* umbra;
                         Impletio* cella;

            ordo = *(StmlNodus**)xar_obtinere(ordines, o);
            si (   !_numerus_attributi(ordo, "vocabulum", &v)
                || !_numerus_attributi(ordo, "analysis", &a)
                || !_numerus_attributi(ordo, "umbra", &u)
                || !_numerus_attributi(ordo, "ad-vocabulum", &w)
                || !_numerus_attributi(ordo, "ad-analysis", &b)
                || v >= ne || w >= ne
                || (umbra = _umbram_invenire(*elementa, v, a, u))
                    == NIHIL
                || !_analysis_adest(*elementa, w, b))
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
            /* vocabulum implens gradu priore vindicatum: veritas eius =
             * lectio PRIMA (n = 0 in proiectione huius gradus); ordo
             * lectionem aliam nominans repetita est (cum puella bona:
             * regula nominativi lectionem NOM 'puellae' iam ablativae
             * inveniret) */
            si (   (vindicata[w] && b != ZEPHYRUM)
                || (   explicita[w] && praelata[w] >= ZEPHYRUM
                    && praelata[w] != (s32)b))
            {
                /* T19e: idem INTRA gradum pro decisione EXPLICITA -
                 * vocabulum implens regula praelationis priore hoc
                 * gradu lectione ALIA decisum (a determinans per
                 * exceptionem;
                 * lectio verbi Moby 'a' umbram verbi 'have' impleret:
                 * have a car) non implet. Decisio STRUCTURALIS eiusdem
                 * gradus (ordo regularum accusativi/ablativi) non
                 * obstat: mensuratum, LLCT -0.2 cum obstaret */
                si (cursus->census != NIHIL)
                {
                    cursus->census->repetitae =
                        cursus->census->repetitae + I;
                }
                perge;
            }
            cella = (Impletio*)xar_addere(impletiones);

            si (cella == NIHIL)
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
            cella->umbra  = umbra;
            cella->w      = w;
            cella->b      = b;
                        /* praelationes ambae sub lege 'prima vincit per
             * vocabulum' (trans gradus quoque: vindicata) */
            si (praelata[v] < ZEPHYRUM && !vindicata[v])
            {
                praelata[v] = (s32)a;
            }
            si (praelata[w] < ZEPHYRUM && !vindicata[w])
            {
                praelata[w] = (s32)b;
            }

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
    /* LIGATIO umbrarum (T19d) post permutationes: index analysis
     * implentis in ordinem NOVUM remittitur (praelata prima, ceterae
     * ordine suo - lex permutationis supra); umbra iam ligata manet
     * (prima vincit: repetita). Lex 'semel scribere' ligationem primam
     * per ponere custodit. */
    per (k = ZEPHYRUM; k < xar_numerus(impletiones); k++)
    {
        Impletio* imp = (Impletio*)xar_obtinere(impletiones, k);
             i32  b_novus;

        si (imp->umbra->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM].genus
            != MATERIA_VALOR_NIHIL)
        {
            si (cursus->census != NIHIL)
            {
                cursus->census->repetitae =
                    cursus->census->repetitae + I;
            }
            perge;
        }
        b_novus = imp->b;
        si (praelata[imp->w] >= ZEPHYRUM)
        {
            si (imp->b == (i32)praelata[imp->w])
            {
                b_novus = ZEPHYRUM;
            }
            alioquin si (imp->b < (i32)praelata[imp->w])
            {
                b_novus = imp->b + I;
            }
        }
        si (   !materia_nodus_ponere(imp->umbra,
                (i32)ORATIO_UMBRA_IMPLETIO_VOCABULUM,
                materia_valor_index((s32)imp->w), MATERIA_LOCUS_INDEX)
            || !materia_nodus_ponere(imp->umbra,
                (i32)ORATIO_UMBRA_IMPLETIO_ANALYSIS,
                materia_valor_index((s32)b_novus), MATERIA_LOCUS_INDEX))
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
                si (cursus->census != NIHIL)
                {
            cursus->census->impletae = cursus->census->impletae + I;
                }
    }
    /* vindicatio: vocabula hoc gradu praelata (etiam iam prima)
     * gradibus sequentibus clausa */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        si (praelata[k] >= ZEPHYRUM)
        {
            vindicata[k] = VERUM;
        }
    }
        piscina_destruere(scratch);
    redde VERUM;
}

/* locus 'umbrae' lectionis: valor LISTA aut NIHIL (genus sine umbris,
 * locus non scriptus) */
interior constans MateriaValor*
_umbrae_lectionis (
    constans MateriaNodus* analysis)
{
    OratioClassis classis =
        oratio_genus_classis((OratioGenus)analysis->genus);
              s32 locus;

    si (classis >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    locus = oratio_partes_locus(classis, "umbrae");
    si (locus < ZEPHYRUM || (i32)locus >= analysis->numerus_locorum)
    {
        redde NIHIL;
    }
    si (analysis->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    redde &analysis->loci[locus];
}

/* GRADUS umbrarum lectionis vocabuli k (lex umbrarum, T19d gamma):
 * II = umbram fert et omnes a VICINO impletae (|w - k| = I), I =
 * aliter (sine umbris - lectio Anglica semper -, umbra vacua, implens
 * remotus). MENSURATUM 2026-09-07: depressio lectionum vacuarum sub
 * lectiones sine umbris adiectiva deprimit (implens ignotus aut sine
 * casu: LLCT -3, CIRCSE -4); impletio remota (regula praecedens ad
 * vocabulum PRIMUM congruens ligat, non proximum) substantiva Senecae
 * deprimit (-3) dum adiectiva chartarum tollit (+10): vicinitas sola
 * thesaurum nullum deprimit - ianua 'strictus'/'proximus' in machina
 * exemplarium legem laxaret. */
interior i32
_gradus_umbrarum (

    constans MateriaNodus* analysis,
                      i32  k)
{
    constans MateriaValor* umbrae = _umbrae_lectionis(analysis);
                      i32  n;
                      i32  u;

    si (umbrae == NIHIL)
    {
        redde I;
    }
    n = materia_valor_lista_numerus(*umbrae);
    si (n == ZEPHYRUM)
    {
        redde I;
    }
    per (u = ZEPHYRUM; u < n; u++)
    {
        constans MateriaValor* valor =
            materia_valor_lista_obtinere(*umbrae, u);

                s32 w;

        si (   valor        == NIHIL
            || valor->genus != MATERIA_VALOR_NODUS
            || valor->datum.nodus->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM]
                .genus == MATERIA_VALOR_NIHIL)
        {
            redde I;   /* vacua */
        }
        w = valor->datum.nodus->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM]
            .datum.index;
        si (w < ZEPHYRUM || ((i32)w > k ? (i32)w - k : k - (i32)w) != I)
        {
            redde I;   /* implens remotus */
        }

    }
    redde (i32)II;

}

/* LEX UMBRARUM (T19d gamma) post gradus omnes: in vocabulo quoque
 * lectiones ordine graduum umbrarum (II: umbrae a vicinis impletae,
 * I: ceterae), intra gradum ordine manente - lectio cuius umbrae
 * impleri non potuerunt lectioni cuius impletae sunt cedit (in bona
 * terra: adiectivum bonae, gradu I substantivo ut obiecto 'in'
 * praelato, per caput terram tollitur). Vocabula per PRAELATIONEM
 * decisa (regula classis, exceptio, regula linguae) intacta: ligatio
 * fortuita decisionem explicitam non vertit. Permutatio per
 * lista_permutare; ligationes quae in vocabulum permutatum spectant
 * per ordinem novum remissae (impletio-analysis reponere). Lex in C,
 * numquam arithmetica in STML (Fran). FALSUM = memoria sola. */
interior b32
_umbris_ordinare (
          Cursus* cursus,
    MateriaNodus* sententia,
    constans b32* explicita)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                 Piscina*  scratch;
                     i32** inversa;   /* per vocabulum: vetus -> novus;
                                      * NIHIL = immotum */
                      i32* numeri;    /* analyses per vocabulum */
                      i32  ne;
                      i32  k;

    ne       = materia_valor_lista_numerus(*elementa);
    scratch  = piscina_generare_dynamicum("oratio_lex_umbrarum", 65536);
    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    inversa = (i32**)piscina_allocare(scratch, (memoriae_index)ne
        * (memoriae_index)magnitudo(i32*));
    numeri  = (i32*)piscina_allocare(scratch, (memoriae_index)ne
        * (memoriae_index)magnitudo(i32));
    si (inversa == NIHIL || numeri == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaValor* elementum =
            materia_valor_lista_obtinere(*elementa, k);
                 MateriaNodus* vocabulum;
        constans MateriaValor* analyses;
                          i32* gradus_lectionum;
                          i32* ordo;
                          i32  n;
                          i32  i;
                          i32  j;
                          s32  g;
                          b32  immotum = VERUM;

                inversa[k]  = NIHIL;
        numeri[k]           = ZEPHYRUM;
        si (   elementum        == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM
            || explicita[k])
        {
            perge;   /* decisio regulae explicitae (praelatio) manet:
                      * ligatio fortuita eam non vertit (EWT 'a' + die
                      * ablativus, LLCT supra + accusativus) */
        }
        vocabulum = elementum->datum.nodus;

        analyses   = &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        n          = materia_valor_lista_numerus(*analyses);
        numeri[k]  = n;
        si (n < (i32)II)
        {
            perge;
        }
        gradus_lectionum = (i32*)piscina_allocare(scratch,
            (memoriae_index)n * (memoriae_index)magnitudo(i32));
        ordo             = (i32*)piscina_allocare(scratch,
            (memoriae_index)n * (memoriae_index)magnitudo(i32));
        si (gradus_lectionum == NIHIL || ordo == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            constans MateriaValor* valor =
                materia_valor_lista_obtinere(*analyses, i);

                        gradus_lectionum[i] =
                            valor != NIHIL
                                && valor->genus == MATERIA_VALOR_NODUS
                            ? _gradus_umbrarum(valor->datum.nodus,
                            k) : I;

        }
        j = ZEPHYRUM;
        per (g = (s32)II; g >= ZEPHYRUM; g--)
        {
            per (i = ZEPHYRUM; i < n; i++)
            {
                si (gradus_lectionum[i] == (i32)g)
                {
                    ordo[j]  = i;
                    j        = j + I;
                }
            }
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (ordo[i] != i)
            {
                immotum = FALSUM;
            }
        }
        si (immotum)
        {
            perge;
        }
        inversa[k] = (i32*)piscina_allocare(scratch,
            (memoriae_index)n * (memoriae_index)magnitudo(i32));
        si (inversa[k] == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            inversa[k][ordo[i]] = i;
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
            cursus->census->umbris_ordinata =
                cursus->census->umbris_ordinata + I;
        }
    }
    /* ligationes remittere: umbra quaeque in vocabulum permutatum
     * spectans indicem novum analysis implentis accipit */
    per (k = ZEPHYRUM; k < ne; k++)
    {
        constans MateriaValor* elementum =
            materia_valor_lista_obtinere(*elementa, k);
        constans MateriaValor* analyses;
                          i32  a;

        si (   elementum        == NIHIL
            || elementum->genus != MATERIA_VALOR_NODUS
            || elementum->datum.nodus->genus
                != (s32)ORATIO_GENUS_VOCABULUM)
        {
            perge;
        }
        analyses =
            &elementum->datum.nodus->loci[ORATIO_VOCABULUM_ANALYSES];
        si (analyses->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        per (a = ZEPHYRUM; a < materia_valor_lista_numerus(*analyses);
             a++)
        {
            constans MateriaValor* valor =
                materia_valor_lista_obtinere(*analyses, a);
            constans MateriaValor* umbrae;
                              i32  u;

            si (valor == NIHIL || valor->genus != MATERIA_VALOR_NODUS)
            {
                perge;
            }
            umbrae = _umbrae_lectionis(valor->datum.nodus);
            si (umbrae == NIHIL)
            {
                perge;
            }
            per (u = ZEPHYRUM; u < materia_valor_lista_numerus(*umbrae);
                 u++)
            {
                constans MateriaValor* valor_umbrae =
                    materia_valor_lista_obtinere(*umbrae, u);
                         MateriaNodus* umbra;
                                  s32  w;
                                  s32  b;

                si (   valor_umbrae        == NIHIL
                    || valor_umbrae->genus != MATERIA_VALOR_NODUS)
                {
                    perge;
                }
                umbra = valor_umbrae->datum.nodus;
                si (   umbra->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM]
                        .genus == MATERIA_VALOR_NIHIL
                    || umbra->loci[ORATIO_UMBRA_IMPLETIO_ANALYSIS]
                        .genus == MATERIA_VALOR_NIHIL)
                {
                    perge;
                }
                w = umbra->loci[ORATIO_UMBRA_IMPLETIO_VOCABULUM]
                    .datum.index;
                b = umbra->loci[ORATIO_UMBRA_IMPLETIO_ANALYSIS]
                    .datum.index;
                si (   w < ZEPHYRUM || (i32)w >= ne
                    || inversa[w] == NIHIL
                    || b < ZEPHYRUM || (i32)b >= numeri[w])
                {
                    perge;
                }
                si (!materia_nodus_reponere(umbra,
                        (i32)ORATIO_UMBRA_IMPLETIO_ANALYSIS,
                        materia_valor_index((s32)inversa[w][b]),
                        MATERIA_LOCUS_INDEX))
                {
                    piscina_destruere(scratch);
                    redde FALSUM;
                }
            }
        }
    }
    piscina_destruere(scratch);
    redde VERUM;
}

/* sententiam unam resolvere: gradus I .. maximus regularum primarum N,
 * quisque proiectione nova (permutationes gradus prioris visae),
 * vocabulis vindicatis trans gradus; post gradus omnes lex umbrarum.
 * FALSUM = memoria sola. */
interior b32
_sententiam_resolvere (

          Cursus* cursus,
    MateriaNodus* sententia)
{
    constans MateriaValor* elementa =
        &sententia->loci[ORATIO_SENTENTIA_ELEMENTA];
                      i32  ne;
                      i32  regulae_numerus;
                      i32  gradus_maximus = I;
                      i32  gradus;
                      i32  k;
                      b32* vindicata;
                      b32* explicita;

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
    per (k = ZEPHYRUM; k < regulae_numerus; k++)
    {
        constans OratioRegula* r =
            (constans OratioRegula*)xar_obtinere(
            cursus->programma->regulae, k);

        si (r->gradus > gradus_maximus)
        {
            gradus_maximus = r->gradus;
        }
    }
        vindicata = (b32*)piscina_allocare(cursus->piscina,
            (memoriae_index)ne * (memoriae_index)magnitudo(b32));
    explicita = (b32*)piscina_allocare(cursus->piscina,
        (memoriae_index)ne * (memoriae_index)magnitudo(b32));
    si (vindicata == NIHIL || explicita == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ne; k++)
    {
        vindicata[k] = FALSUM;
        explicita[k] = FALSUM;
    }
    per (gradus = I; gradus <= gradus_maximus; gradus++)
    {
        si (!_sententiam_resolvere_gradu(cursus, sententia, gradus,
                vindicata, explicita))
        {
            redde FALSUM;
        }
    }
    redde _umbris_ordinare(cursus, sententia, explicita);
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
