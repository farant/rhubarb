/* oratio_vocabula.c - Vide oratio_vocabula.h. */

#include "oratio_vocabula.h"
#include "oratio_glossarium.h"
#include "oratio_arbor.h"
#include "oratio_registrum.h"
#include "silva_lexema.h"
#include "silva_token.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "tabula_dispersa.h"
#include <string.h>

nomen structura {
    s32 index;
} Sedes;

structura OratioVocabula {
                 Piscina* piscina;
    OratioVocabulariumLa* voc;
    OratioVocabulariumEn* en;          /* contextus Anglicus: Moby (NIHIL Latine) */
                     b32  anglice;     /* VERUM = iudicium contextus Anglici */
                     Xar* verba;                  /* OratioVerbum */
          TabulaDispersa* per_verbum;  /* verbum minusculum -> Sedes */
                     i32  sedes;
                     b32  iudicata;
};

hic_manens constans character* TITULI_STATUUM[] = {
    "notum", "ambiguum", "permissum", "ignotum", "latinum"
};

constans character* constans ORATIO_VOCABULA_EXCLUSA[] = {
    "knotapel/", "vendor/", "archivum/", NIHIL
};

constans character* constans ORATIO_PROSA_EXCLUSA[] = {
    "vendor/", "archivum/", "gesta/annales/tabula.md", "md/CENSUS.md",
    NIHIL
};

b32
oratio_vocabula_via_exclusa (
                          chorda  via,
    constans character* constans* exclusa)
{
    i32 i;

    si (exclusa == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; exclusa[i] != NIHIL; i++)
    {
        i32 l = (i32)strlen(exclusa[i]);

        si (   via.mensura                              >= l
            && memcmp(via.datum, exclusa[i], (size_t)l) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

constans character*
oratio_verbum_status_titulus (
    OratioVerbumStatus status)
{
    si ((i32)status >= (i32)ORATIO_VERBUM_NUMERUS_STATUUM)
    {
        redde "?";
    }
    redde TITULI_STATUUM[status];
}

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
    i8* d = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);

    si (d == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    memcpy(d, c.datum, (size_t)c.mensura);
    d[c.mensura] = ZEPHYRUM;
    redde _chorda(d, c.mensura);
}

interior chorda
_copia_literarum (
               Piscina* piscina,
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;

    u.c = literae;
    redde _copia(piscina, _chorda(u.m, (i32)strlen(literae)));
}

interior b32
_littera_est (
    i8 c)
{
    redde (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c >= 0x80);
}

interior b32
_capitalis_est (
    i8 c)
{
    redde (b32)(c >= 'A' && c <= 'Z');
}

interior b32
_minuscula_est (
    i8 c)
{
    redde (b32)(c >= 'a' && c <= 'z');
}

interior b32
_digitus_est (
    i8 c)
{
    redde (b32)(c >= '0' && c <= '9');
}

OratioVocabula*
oratio_vocabula_creare_anglice (
                 Piscina* piscina,
    OratioVocabulariumLa* voc,
    OratioVocabulariumEn* en)
{
    OratioVocabula* vc = oratio_vocabula_creare(piscina, voc);

    si (vc == NIHIL)
    {
        redde NIHIL;
    }
    vc->en       = en;
    vc->anglice  = VERUM;
    redde vc;
}

OratioVocabula*
oratio_vocabula_creare (
                 Piscina* piscina,
    OratioVocabulariumLa* voc)
{
    OratioVocabula* vc = (OratioVocabula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(OratioVocabula));

    si (vc == NIHIL)
    {
        redde NIHIL;
    }
    memset(vc, ZEPHYRUM, magnitudo(*vc));
    vc->piscina = piscina;
    vc->voc = voc;
    vc->verba = xar_creare(piscina, (i32)magnitudo(OratioVerbum));
    vc->per_verbum = tabula_dispersa_creare_chorda(piscina, (i32)32768);
    si (vc->verba == NIHIL || vc->per_verbum == NIHIL)
    {
        redde NIHIL;
    }
    redde vc;
}

b32
oratio_vocabula_verbum_addere (
        OratioVocabula* vc,
                chorda  verbum,
    constans character* via,
                   i32  linea,
                   b32  ex_commento)
{
    redde oratio_vocabula_verbum_addere_sede(vc, verbum, via, linea,
        ex_commento ? ORATIO_SEDES_COMMENTUM : ORATIO_SEDES_SYMBOLUM);
}

b32
oratio_vocabula_verbum_addere_sede (
        OratioVocabula* vc,
                chorda  verbum,
    constans character* via,
                   i32  linea,
      OratioSedesGenus  genus)
{
              i8  minusculum[64];
          chorda  clavis;
          vacuum* valor = NIHIL;
    OratioVerbum* v;
             i32  i;
             i32  litterae = ZEPHYRUM;

    si (verbum.mensura < (i32)II || verbum.mensura > (i32)63)
    {
        redde VERUM;   /* nihil: nimis breve aut longum */
    }
    per (i = ZEPHYRUM; i < verbum.mensura; i++)
    {
        i8 c = verbum.datum[i];

        si (_capitalis_est(c))
        {
            c = (i8)(c - 'A' + 'a');
        }
                /* littera = ASCII aut octetus DUCENS UTF-8 (>= 0xC0): sequentia
         * multi-octeta una littera est, ergo sagitta sola (E2 86 92) aut
         * littera Graeca sola verbum non facit */
        si (_littera_est(c) && (c < 0x80 || c >= 0xC0))
        {
            litterae = litterae + I;
        }
        minusculum[i] = c;
    }
    si (litterae < (i32)II)
    {
        redde VERUM;   /* s32, b32, 42: non verba */
    }
    clavis     = _chorda(minusculum, verbum.mensura);
    vc->sedes  = vc->sedes + I;
    si (tabula_dispersa_invenire(vc->per_verbum, clavis, &valor))
    {
        v = (OratioVerbum*)xar_obtinere(vc->verba,
            (i32)((Sedes*)valor)->index);
    }
    alioquin
    {
        Sedes* s = (Sedes*)piscina_allocare(vc->piscina,
            (memoriae_index)magnitudo(Sedes));

        v = (OratioVerbum*)xar_addere(vc->verba);
        si (s == NIHIL || v == NIHIL)
        {
            redde FALSUM;
        }
        memset(v, ZEPHYRUM, magnitudo(*v));
        v->verbum             = _copia(vc->piscina, clavis);
        v->status             = ORATIO_VERBUM_IGNOTUM;
        v->via_prima          = _copia_literarum(vc->piscina, via);
        v->linea_prima        = linea;
        v->ex_commento_prima  = (b32)(genus == ORATIO_SEDES_COMMENTUM);
        v->ex_prosa_prima     = (b32)(genus == ORATIO_SEDES_PROSA);
        s->index              = (s32)xar_numerus(vc->verba) - I;
        si (!tabula_dispersa_inserere(vc->per_verbum, v->verbum, s))
        {
            redde FALSUM;
        }
    }
        v->sedes = v->sedes + I;
    commutatio (genus)
    {
        casus ORATIO_SEDES_COMMENTUM:
            v->sedes_commentorum = v->sedes_commentorum + I;
            frange;
        casus ORATIO_SEDES_PROSA:
            v->sedes_prosae = v->sedes_prosae + I;
            frange;
        ordinarius:
            v->sedes_symbolorum = v->sedes_symbolorum + I;
            frange;
    }
    redde VERUM;
}

b32
oratio_vocabula_identificatorem_addere (
        OratioVocabula* vc,
                chorda  identificator,
    constans character* via,
                   i32  linea)
{
    i32 i = ZEPHYRUM;
    i32 n = identificator.mensura;

    dum (i < n)
    {
        i32 a;
        i32 j;

        /* separatores ('_', cetera non alphanumerica) praeterire */
        dum (   i < n && !_littera_est(identificator.datum[i])
             && !_digitus_est(identificator.datum[i]))
        {
            i = i + I;
        }
        si (i >= n)
        {
            frange;
        }
        a = i;
        j = i + I;
        dum (j < n)
        {
            i8 c = identificator.datum[j];
            i8 p = identificator.datum[j - I];

            si (!_littera_est(c) && !_digitus_est(c))
            {
                frange;
            }
            /* limes minuscula/digitus -> CAPITALIS (materiaNodus) aut
             * CAPITALIS CAPITALIS minuscula (STMLNodus -> STML Nodus) */
            si (   _capitalis_est(c)
                && (_minuscula_est(p) || _digitus_est(p)
                    || (_capitalis_est(p) && j + I < n
                        && _minuscula_est(identificator.datum[j + I]))))
            {
                frange;
            }
            j = j + I;
        }
        si (!oratio_vocabula_verbum_addere(vc,
                _chorda(identificator.datum + a, j - a), via, linea,
                FALSUM))
        {
            redde FALSUM;
        }
        i = j;
    }
    redde VERUM;
}

/* campus n (0-basatus) lineae TSV */
interior chorda
_campus (
    chorda linea,
       i32 n)
{
    i32 i = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (i <= linea.mensura)
    {
        i32 a = i;

        dum (i < linea.mensura && linea.datum[i] != '\t')
        {
            i = i + I;
        }
        si (k == n)
        {
            redde _chorda(linea.datum + a, i - a);
        }
        k = k + I;
        i = i + I;
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

interior i32
_numerus_ex (
    chorda c)
{
    i32 v = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura && _digitus_est(c.datum[i]); i++)
    {
        v = v * (i32)X + (i32)(c.datum[i] - '0');
    }
    redde v;
}

b32
oratio_vocabula_symbola (
                  OratioVocabula* vc,
                          chorda  nexus_tsv,
    constans character* constans* exclusa)
{

          i32 cursor = ZEPHYRUM;
    character via[512];

    dum (cursor < nexus_tsv.mensura)
    {
           i32 a = cursor;
           i32 b = a;
        chorda linea;
        chorda genus;

        dum (b < nexus_tsv.mensura && nexus_tsv.datum[b] != '\n')
        {
            b = b + I;
        }
        cursor  = b + I;
        linea   = _chorda(nexus_tsv.datum + a, b - a);
        si (linea.mensura == ZEPHYRUM || linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }
        genus = _campus(linea, I);
        si (   genus.mensura != (i32)V
            || memcmp(genus.datum, "sedes", (size_t)V)
            != ZEPHYRUM)
        {
            perge;
        }
                {
            chorda v = _campus(linea, (i32)III);
               i32 m = v.mensura < (i32)511 ? v.mensura : (i32)511;

            si (oratio_vocabula_via_exclusa(v, exclusa))
            {
                perge;
            }
            memcpy(via, v.datum, (size_t)m);
            via[m] = '\0';
        }

        si (!oratio_vocabula_identificatorem_addere(vc, _campus(linea,
                ZEPHYRUM), via, _numerus_ex(_campus(linea, (i32)IV))))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* vocabula arboris orationis (commentarium) addere; linea = basis +
 * lineae novae ante vocabulum intra commentarium */
interior b32
_vocabula_arboris (
               OratioVocabula* vc,
        constans MateriaNodus* doc,
                       chorda  fons,
           constans character* via,
                          i32  linea_basis,
             OratioSedesGenus  genus)
{
    constans MateriaValor* paragraphi =
        &doc->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
                      i32 np;
                      i32 p;
                      i32 cursor  = ZEPHYRUM;
                      i32 linea   = linea_basis;

    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    np = materia_valor_lista_numerus(*paragraphi);
    per (p = ZEPHYRUM; p < np; p++)
    {
        constans MateriaNodus* par =
            materia_valor_lista_obtinere(*paragraphi, p)->datum.nodus;
        constans MateriaValor* sententiae =
            &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
                          i32 ns;
                          i32 s;

        si (sententiae->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        ns = materia_valor_lista_numerus(*sententiae);
        per (s = ZEPHYRUM; s < ns; s++)
        {
            constans MateriaNodus* sen =
                materia_valor_lista_obtinere(*sententiae,
                s)->datum.nodus;
            constans MateriaValor* elementa =
                &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
                              i32 ne;
                              i32 k;

            si (elementa->genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            ne = materia_valor_lista_numerus(*elementa);
            per (k = ZEPHYRUM; k < ne; k++)
            {
                constans MateriaNodus* e =
                    materia_valor_lista_obtinere(*elementa,
                    k)->datum.nodus;
                constans MateriaValor* partes;
                constans MateriaToken* a;
                constans MateriaToken* b;
                                  i32  ab;

                si (e->genus != (s32)ORATIO_GENUS_VOCABULUM)
                {
                    perge;
                }
                partes = &e->loci[ORATIO_VOCABULUM_PARTES];
                si (   partes->genus != MATERIA_VALOR_LISTA
                    || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
                {
                    perge;
                }
                a = materia_valor_lista_obtinere(*partes,
                    ZEPHYRUM)->datum.token;
                b = materia_valor_lista_obtinere(*partes,
                    materia_valor_lista_numerus(*partes)
                        - I)->datum.token;
                ab = (i32)a->byte_offset;
                dum (cursor < ab && cursor < fons.mensura)
                {
                    si (fons.datum[cursor] == '\n')
                    {
                        linea = linea + I;
                    }
                    cursor = cursor + I;
                }
                si (!oratio_vocabula_verbum_addere_sede(vc,
                        _chorda(fons.datum + ab,
                            (i32)(b->byte_offset
                                + (s32)b->valor.mensura) - ab),
                        via, linea, genus))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

b32
oratio_vocabula_commenta (
        OratioVocabula* vc,
                chorda  fons,
    constans character* via)
{
    Piscina* scratch = piscina_generare_dynamicum("vocabula_commenta",
        67108864);
    Xar* lexemata;
    i32  i;
    b32  sanum = VERUM;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    lexemata = silva_lexare_cruda(scratch,
        (constans character*)fons.datum,
        fons.mensura, ZEPHYRUM);
    si (lexemata == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(lexemata) && sanum; i++)
    {
        SilvaToken* t = *(SilvaToken**)xar_obtinere(lexemata, i);
        MateriaNodus* doc;

        si (   t->genus != SILVA_LEX_COMMENTUM_CLAUSUM
            && t->genus != SILVA_LEX_COMMENTUM_LINEA)
        {
            perge;
        }
        doc = oratio_arbor_parsare(scratch,
            (constans character*)t->valor.datum,
            t->valor.mensura);
        si (doc == NIHIL)
        {
            sanum = FALSUM;
            frange;
        }
        sanum = _vocabula_arboris(vc, doc, t->valor, via, t->linea,
            ORATIO_SEDES_COMMENTUM);
    }
        piscina_destruere(scratch);
    redde sanum;
}

/* nodos TEXTUS arboris md ambulare: extentum lexematum crudorum
 * (plagulae, non syntheticorum) per arborem orationis legitur, linea =
 * lexematis primi; saepta, verbatim, destinationes nexuum, html,
 * praefatio genera ALIA sunt et numquam tanguntur */
interior b32
_prosa_arboris (
           OratioVocabula* vc,
                  Piscina* scratch,
    constans MateriaNodus* n,
                   chorda  fons,
       constans character* via)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (n->genus == (s32)MD_GENUS_TEXTUS)
    {
        constans MateriaValor* crudum  = &n->loci[MD_TEXTUS_CRUDUM];
                          s32  a       = (s32)-I;
                          s32  b       = ZEPHYRUM;
                          i32  linea   = I;
                          i32  m;
                          i32  k;

        si (crudum->genus != MATERIA_VALOR_LISTA)
        {
            redde VERUM;
        }
        m = materia_valor_lista_numerus(*crudum);
        per (k = ZEPHYRUM; k < m; k++)
        {
            constans MateriaValor* e =
                materia_valor_lista_obtinere(*crudum, k);
            constans MateriaToken* t;

            si (e == NIHIL || e->genus != MATERIA_VALOR_TOKEN)
            {
                perge;
            }
            t = e->datum.token;
            si (   t->fons_index != MD_FONS_PLAGULAE
                || t->byte_offset < ZEPHYRUM)
            {
                perge;
            }
            si (a < ZEPHYRUM || t->byte_offset < a)
            {
                a      = t->byte_offset;
                linea  = t->linea;
            }
            si (t->byte_offset + (s32)t->valor.mensura > b)
            {
                b = t->byte_offset + (s32)t->valor.mensura;
            }
        }
        si (a >= ZEPHYRUM && b > a && b <= (s32)fons.mensura)
        {
            MateriaNodus* doc = oratio_arbor_parsare(scratch,
                (constans character*)fons.datum + a, (i32)(b - a));

            si (doc == NIHIL)
            {
                redde FALSUM;
            }
            redde _vocabula_arboris(vc, doc,
                _chorda(fons.datum + a, (i32)(b - a)), via, linea,
                ORATIO_SEDES_PROSA);
        }
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_prosa_arboris(vc, scratch, v->datum.nodus, fons, via))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 nl = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < nl; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_prosa_arboris(vc, scratch, e->datum.nodus,
                    fons,
                        via))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

b32
oratio_vocabula_prosa (
        OratioVocabula* vc,
                chorda  fons,
    constans character* via)
{
    Piscina* scratch = piscina_generare_dynamicum("vocabula_prosa",
        67108864);
    MateriaNodus* doc;
    b32 sanum;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    doc = md_arbor_parsare(scratch, (constans character*)fons.datum,
        fons.mensura);
    si (doc == NIHIL)
    {
        piscina_destruere(scratch);
        redde FALSUM;
    }
    sanum = _prosa_arboris(vc, scratch, doc, fons, via);
    piscina_destruere(scratch);
    redde sanum;
}

/* lemma analysis (in scratch) */
interior chorda
_lemma_analysis (
                          Piscina* scratch,
    constans OratioVocabulariumLa* voc,
          constans OratioAnalysis* a,
                           chorda* classis)
{
    commutatio (a->genus)
    {
        casus ORATIO_ANALYSIS_GLOSSARIUM:
        {
            constans OratioGlossarium* gl =
                oratio_vocabularium_la_glossarium(voc);
            constans OratioGlossariumEntrium* e =
                oratio_glossarium_entrium(gl,
                oratio_glossarium_forma(gl, a->glossarium)->entrium);

            *classis = e->classis;
            redde e->lemma;
        }
        casus ORATIO_ANALYSIS_STIRPS:
        {
            constans character* t = oratio_pars_titulus(
                oratio_vocabularium_la_stirps(voc, a->stirps)->pars);

            *classis = _copia_literarum(scratch, t);
            redde oratio_vocabularium_la_lemma(scratch, voc, a->stirps);
        }
        casus ORATIO_ANALYSIS_UNICUM:
            *classis = _copia_literarum(scratch, "unicum");
            redde oratio_vocabularium_la_unicum(voc, a->unicum)->forma;
        ordinarius:
            *classis = _copia_literarum(scratch, "tackon");
            redde oratio_vocabularium_la_additamentum(voc,
                a->tackon)->forma;
    }
}

/* contextus ANGLICUS (prosa): glossarium (entria anglice licita)
 * primum, Moby deinde (forma exacta - morphologia gradus IV), tabula
 * Latina tertia -> LATINUM (notum Latine, non inventum: nomina rerum
 * domus in prosa aperte scripta); ambiguum numquam (Moby lemmata non
 * fert) */
interior b32
_iudicare_anglice (
    OratioVocabula* vc)
{
    Piscina* scratch =
        piscina_generare_dynamicum("vocabula_iudicium_en",
        67108864);
    constans OratioGlossarium* gl;
                          i32  i;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    gl = oratio_vocabularium_la_glossarium(vc->voc);
    per (i = ZEPHYRUM; i < xar_numerus(vc->verba); i++)
    {
        OratioVerbum* v = (OratioVerbum*)xar_obtinere(vc->verba, i);
                 Xar* x;
                 i32  k;

        v->analyses  = ZEPHYRUM;
        v->lemmata   = ZEPHYRUM;
        v->status    = ORATIO_VERBUM_IGNOTUM;
        /* I. glossarium: entria in prosa Anglica licita (anglicus, ambo) */
        si (gl != NIHIL)
        {
            Xar* formae = oratio_glossarium_quaerere(scratch, gl,
                v->verbum);

            si (formae == NIHIL)
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
            per (k = ZEPHYRUM; k < xar_numerus(formae); k++)
            {
                constans OratioGlossariumEntrium* e =
                    oratio_glossarium_entrium(gl,
                    oratio_glossarium_forma(gl,
                        *(s32*)xar_obtinere(formae, k))->entrium);

                si (!e->anglice)
                {
                    perge;
                }
                                v->analyses = v->analyses + I;
                si (v->analyses == I)
                {
                    v->classis  = _copia(vc->piscina, e->classis);
                    v->lemma    = _copia(vc->piscina, e->lemma);
                    v->lemmata  = I;
                    /* entrium Latinum (lexema, silva, sum) in prosa
                     * Anglica = LATINUM, non notum: status linguam refert */
                    v->status   = e->permissum ? ORATIO_VERBUM_PERMISSUM
                        : (e->lingua.mensura == (i32)VI
                            && memcmp(e->lingua.datum, "latina",
                                (size_t)VI) == ZEPHYRUM)
                            ? ORATIO_VERBUM_LATINUM
                            : ORATIO_VERBUM_NOTUM;
                }
            }
        }
        /* II. Moby: forma exacta */
        si (v->analyses == ZEPHYRUM && vc->en != NIHIL)
        {
            x = oratio_vocabularium_en_quaerere(scratch, vc->en,
                v->verbum);
            si (x == NIHIL)
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
            si (xar_numerus(x) > ZEPHYRUM)
            {
                constans OratioVocabulumEn* r =
                    oratio_vocabularium_en_recordum(vc->en,
                    *(s32*)xar_obtinere(x, ZEPHYRUM));
                constans character* classis =
                    oratio_vocabularium_en_classis(
                    r->codices.datum[ZEPHYRUM]);

                v->analyses  = xar_numerus(x);
                v->lemmata   = I;
                v->classis   = _copia_literarum(vc->piscina,
                    classis != NIHIL ? classis : "ignotum");
                v->lemma   = _copia(vc->piscina, v->verbum);
                v->status  = ORATIO_VERBUM_NOTUM;
            }
        }
        /* III. tabula Latina (glossarium totum inclusum): LATINUM */
        si (v->analyses == ZEPHYRUM)
        {
            x = oratio_vocabularium_la_quaerere(scratch, vc->voc,
                v->verbum);
            si (x == NIHIL)
            {
                piscina_destruere(scratch);
                redde FALSUM;
            }
            si (xar_numerus(x) > ZEPHYRUM)
            {
                chorda classis;
                chorda lemma = _lemma_analysis(scratch, vc->voc,
                    (constans OratioAnalysis*)xar_obtinere(x, ZEPHYRUM),
                    &classis);

                v->analyses  = xar_numerus(x);
                v->lemmata   = I;
                v->classis   = _copia(vc->piscina, classis);
                v->lemma     = _copia(vc->piscina, lemma);
                v->status    = ORATIO_VERBUM_LATINUM;
            }
        }
        si ((i & 0x3FF) == 0x3FF)
        {
            piscina_destruere(scratch);
            scratch = piscina_generare_dynamicum("vocabula_iudicium_en",
                67108864);
            si (scratch == NIHIL)
            {
                redde FALSUM;
            }
        }
    }
    piscina_destruere(scratch);
    vc->iudicata = VERUM;
    redde VERUM;
}

b32
oratio_vocabula_iudicare (
    OratioVocabula* vc)
{
    Piscina* scratch = piscina_generare_dynamicum("vocabula_iudicium",
        67108864);
    i32 i;

        si (scratch == NIHIL)
        {
        redde FALSUM;
        }
    si (vc->anglice)
    {
        piscina_destruere(scratch);
        redde _iudicare_anglice(vc);
    }
    per (i = ZEPHYRUM; i < xar_numerus(vc->verba); i++)
    {
        OratioVerbum* v = (OratioVerbum*)xar_obtinere(vc->verba, i);
        Xar* x = oratio_vocabularium_la_quaerere(scratch, vc->voc,
            v->verbum);
           b32 permissum = FALSUM;
        chorda lemmata[64];
           i32 distincta = ZEPHYRUM;
           i32 k;

        si (x == NIHIL)
        {
            piscina_destruere(scratch);
            redde FALSUM;
        }
                v->analyses = ZEPHYRUM;
        per (k = ZEPHYRUM; k < xar_numerus(x); k++)
        {
            constans OratioAnalysis* a =
                (constans OratioAnalysis*)xar_obtinere(x, k);
            chorda classis;
            chorda lemma;
               i32 j;
               b32 nova = VERUM;

            si (a->genus == ORATIO_ANALYSIS_GLOSSARIUM)
            {
                constans OratioGlossarium* gl =
                    oratio_vocabularium_la_glossarium(vc->voc);
                constans OratioGlossariumEntrium* en =
                    oratio_glossarium_entrium(
                    gl, oratio_glossarium_forma(gl,
                    a->glossarium)->entrium);

                /* contextus LATINUS (identificatores, commentaria): entria
                 * contextus anglici hic non valent - nec permissa nec nota */
                si (!en->latine)
                {
                    perge;
                }
                si (en->permissum)
                {
                    permissum = VERUM;
                }
            }
            lemma = _lemma_analysis(scratch, vc->voc, a, &classis);
            v->analyses = v->analyses + I;
            si (v->analyses == I)
            {
                v->classis  = _copia(vc->piscina, classis);
                v->lemma    = _copia(vc->piscina, lemma);
            }

            si (a->genus == ORATIO_ANALYSIS_TACKON)
            {
                perge;   /* tackon lemma non est */
            }
            per (j = ZEPHYRUM; j < distincta; j++)
            {
                si (   lemmata[j].mensura == lemma.mensura
                    && memcmp(lemmata[j].datum, lemma.datum,
                        (size_t)lemma.mensura) == ZEPHYRUM)
                {
                    nova = FALSUM;
                    frange;
                }
            }
            si (nova && distincta < (i32)64)
            {
                lemmata[distincta]  = lemma;
                distincta           = distincta + I;
            }
        }
        v->lemmata = distincta;
        si (permissum)
        {
            v->status = ORATIO_VERBUM_PERMISSUM;
        }
        alioquin si (v->analyses == ZEPHYRUM)
        {
            v->status = ORATIO_VERBUM_IGNOTUM;
        }
        alioquin si (distincta > I)
        {
            v->status = ORATIO_VERBUM_AMBIGUUM;
        }
        alioquin
        {
            v->status = ORATIO_VERBUM_NOTUM;
        }
        si ((i & 0x3FF) == 0x3FF)
        {
            /* scratch renovare ne cresceret: lemmata iam copiata */
            piscina_destruere(scratch);
            scratch = piscina_generare_dynamicum("vocabula_iudicium",
                67108864);
            si (scratch == NIHIL)
            {
                redde FALSUM;
            }
        }
    }
    piscina_destruere(scratch);
    vc->iudicata = VERUM;
    redde VERUM;
}

Xar*
oratio_vocabula_verba (
    constans OratioVocabula* vc)
{
    redde vc->verba;
}

Xar*
oratio_vocabula_ordinata (
                    Piscina* piscina,
    constans OratioVocabula* vc,
                        s32  status)
{
        Xar* exitus  = xar_creare(piscina, (i32)magnitudo(s32));
        i32  n       = xar_numerus(vc->verba);
        i32  maxima  = ZEPHYRUM;
        i32* numeri;
        i32  i;
        s32  s;


    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans OratioVerbum* v = (constans OratioVerbum*)xar_obtinere(
            vc->verba, i);

        si (v->sedes > maxima)
        {
            maxima = v->sedes;
        }
    }
        /* ordinatio per numerationem (O(n + maxima), non per cursus n x
     * maxima - super prosam 62 milia verborum x 100 milia sedium XX s
     * erant): numerus verborum cuiusque sedis, deinde initia descendentia
     * (sedes maiores primae), deinde verba ordine primi adventus in
     * loca sua */
    numeri = (i32*)piscina_allocare(piscina,
        (memoriae_index)(maxima + I) * (memoriae_index)magnitudo(i32));
    si (numeri == NIHIL)
    {
        redde NIHIL;
    }
    memset(numeri, ZEPHYRUM, (size_t)(maxima + I) * magnitudo(i32));
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans OratioVerbum* v =
            (constans OratioVerbum*)xar_obtinere(vc->verba, i);

        si (status < ZEPHYRUM || (s32)v->status == status)
        {
            numeri[v->sedes] = numeri[v->sedes] + I;
        }
    }
    {
        i32 initium = ZEPHYRUM;

        per (s = (s32)maxima; s >= ZEPHYRUM; s--)
        {
            i32 t = numeri[s];

            numeri[s]  = initium;
            initium    = initium + t;
        }
        per (i = ZEPHYRUM; i < initium; i++)
        {
            s32* locus = (s32*)xar_addere(exitus);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus = (s32)-I;
        }
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans OratioVerbum* v =
            (constans OratioVerbum*)xar_obtinere(vc->verba, i);

        si (status < ZEPHYRUM || (s32)v->status == status)
        {
            *(s32*)xar_obtinere(exitus, numeri[v->sedes]) = (s32)i;
            numeri[v->sedes] = numeri[v->sedes] + I;
        }
    }
    redde exitus;
}

i32
oratio_vocabula_numerus (
    constans OratioVocabula* vc,
         OratioVerbumStatus  status)
{
    i32 n = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(vc->verba); i++)
    {
        si (((constans OratioVerbum*)xar_obtinere(vc->verba, i))->status
            == status)
        {
            n = n + I;
        }
    }
    redde n;
}

i32
oratio_vocabula_sedes (
    constans OratioVocabula* vc)
{
    redde vc->sedes;
}
