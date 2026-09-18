/* materia_exemplaria.c - Ordines inventorum ex documento expanso
 *
 * Ambulatio RECURSIVA per subarborem ordinis. Python idem per fluxum
 * SAX agebat (acervus notarum, 'gradus_capti', elementa vacua HTML) -
 * fulcimenta quae arbor vera non poscit: recursio ACERVUS EST, et
 * 'noli descendere' REDDE est.
 */

#include "materia_exemplaria.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Adiumenta arboris
 * ================================================== */

interior b32
_elementum_est (
    constans StmlNodus* n)
{
    redde (b32)(n != NIHIL && n->genus == STML_NODUS_ELEMENTUM);
}

/* attributum ut litterae NUL-terminatae in piscina (MateriaDiagnosticum
 * 'constans character*' fert, chorda non est) */
interior constans character*
_attributum (
           Piscina* piscina,
         StmlNodus* n,
constans character* titulus)
{
       chorda* v = stml_attributum_capere(n, titulus);
    character* c;

    si (v == NIHIL || v->mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    c = (character*)piscina_allocare(piscina,
        (memoriae_index)v->mensura + I);
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(c, v->datum, (memoriae_index)v->mensura);
    c[v->mensura] = '\0';
    redde c;
}

/* 'L:C-L:C' + 'B-B' -> tractus. FALSUM = sedes abest aut prava:
 * ordo sine sede inventum LOCATUM fieri non potest. */
interior b32
_tractus_legere (
     StmlNodus* n,
MateriaTractus* t)
{
       chorda* sedes   = stml_attributum_capere(n, "sedes");
       chorda* octeti  = stml_attributum_capere(n, "octeti");
    character  copia[LXIV];
      integer  linea = 0, columna = 0, linea_f = 0, columna_f = 0;
      integer  initium = 0, finis = 0;

    si (sedes == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    memset(t, ZEPHYRUM, magnitudo(*t));
    si (sedes->mensura >= (i32)LXIV || octeti->mensura >= (i32)LXIV)
    {
        redde FALSUM;
    }
    memcpy(copia, sedes->datum, (memoriae_index)sedes->mensura);
    copia[sedes->mensura] = '\0';
    si (sscanf(copia, "%d:%d-%d:%d", &linea, &columna, &linea_f,
            &columna_f) != IV)
    {
        redde FALSUM;
    }
    memcpy(copia, octeti->datum, (memoriae_index)octeti->mensura);
    copia[octeti->mensura] = '\0';
    si (sscanf(copia, "%d-%d", &initium, &finis) != II)
    {
        redde FALSUM;
    }
    t->linea          = (i32)linea;
    t->columna        = (i32)columna;
    t->linea_finis    = (i32)linea_f;
    t->columna_finis  = (i32)columna_f;
    t->initium        = (s32)initium;
    t->finis          = (s32)finis;
    t->est_fons       = VERUM;
    redde VERUM;
}


/* ==================================================
 * Sedes colligere - LEX DESCENSUS TACENTIS
 * ================================================== */

nomen structura {
    MateriaTractus  tractus;
constans character* nota;
} SedesLecta;

/* Sedes SUMMAS colligere: nodus qui sedem fert eam confert et
 * descensus ILLIC TACET - subarbor capta tota unum locum est.
 * 'nota_ambiens' involucrum proximum nominat quod notam habuit. */
interior vacuum
_sedes_colligere (
            Piscina* piscina,
          StmlNodus* n,
 constans character* nota_ambiens,
                Xar* series)
{
    MateriaTractus  t;
constans character* nota;
               i32  k;

    si (!_elementum_est(n))
    {
        redde;
    }
    nota = _attributum(piscina, n, "nota");
    si (nota == NIHIL)
    {
        nota = nota_ambiens;
    }
    si (_tractus_legere(n, &t))
    {
        SedesLecta* s = (SedesLecta*)xar_addere(series);

        si (s != NIHIL)
        {
            s->tractus  = t;
            s->nota     = nota;
        }
        redde;   /* DESCENSUS TACET - lex tota */
    }
    per (k = ZEPHYRUM; k < xar_numerus(n->liberi); k++)
    {
        _sedes_colligere(piscina, *(StmlNodus**)xar_obtinere(n->liberi,
            k), nota, series);
    }
}


/* ==================================================
 * Ordo unus
 * ================================================== */

interior vacuum
_ordinem_addere (
            Piscina* piscina,
                Xar* exitus,
          StmlNodus* liberum,
 constans character* lint,
 constans character* gravitas,
 constans character* causa)
{
                  Xar* series;
  MateriaDiagnosticum* d;
   MateriaSedesRelata* relata;
            character* codex;
                  i32  k;

    series = xar_creare(piscina, (i32)magnitudo(SedesLecta));
    si (series == NIHIL)
    {
        redde;
    }
    _sedes_colligere(piscina, liberum, NIHIL, series);
    si (xar_numerus(series) == ZEPHYRUM)
    {
        /* ordo sine sede: inventum LOCATUM fieri non potest. Tacite
         * cadere id est quod arcus diagnosticorum prohibere vult,
         * ergo ordo cum tractu vacuo transit et consumptor eum
         * NOMINARE potest. */
        redde;
    }
    d = (MateriaDiagnosticum*)xar_addere(exitus);
    si (d == NIHIL)
    {
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(*d));
    codex = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(lint) + VI);
    si (codex == NIHIL)
    {
        redde;
    }
    strcpy(codex, "lint:");
    strcat(codex, lint);
    d->codex = codex;
    d->causa = causa != NIHIL ? causa : lint;
    d->gravitas  = (gravitas != NIHIL
                    && strcmp(gravitas, "monitum") == ZEPHYRUM)
        ? (s32)MATERIA_GRAVITAS_MONITUM
        : (s32)MATERIA_GRAVITAS_ERRATUM;
    d->tractus   = ((SedesLecta*)xar_obtinere(series,
                        ZEPHYRUM))->tractus;
    d->nota      = ((SedesLecta*)xar_obtinere(series,
                        ZEPHYRUM))->nota;
    si (xar_numerus(series) <= (i32)I)
    {
        redde;
    }
    relata = (MateriaSedesRelata*)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(series) - I)
            * magnitudo(MateriaSedesRelata));
    si (relata == NIHIL)
    {
        redde;
    }
    per (k = (i32)I; k < xar_numerus(series); k++)
    {
        SedesLecta* s = (SedesLecta*)xar_obtinere(series, k);

        relata[k - I].tractus  = s->tractus;
        relata[k - I].nota     = s->nota;
    }
    d->relata             = relata;
    d->numerus_relatorum  = xar_numerus(series) - I;
}


/* ==================================================
 * Extractio
 * ================================================== */

interior vacuum
_relatum_tractare (
       Piscina* piscina,
           Xar* exitus,
     StmlNodus* relatum)
{
    constans character* lint     = _attributum(piscina, relatum,
                                       "lint");
    constans character* gravitas = _attributum(piscina, relatum,
                                       "gravitas");
    constans character* causa    = _attributum(piscina, relatum,
                                       "causa");
                    i32 k;

    si (lint == NIHIL)
    {
        redde;
    }
    /* ORDO UNUS PER LIBERUM: filii bini inventa DUO sunt, non
     * inventum unum sedibus binis - electio auctoris regulae. */
    per (k = ZEPHYRUM; k < xar_numerus(relatum->liberi); k++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(relatum->liberi, k);

        si (_elementum_est(l))
        {
            _ordinem_addere(piscina, exitus, l, lint, gravitas, causa);
        }
    }
}

interior vacuum
_quaerere (
       Piscina* piscina,
           Xar* exitus,
     StmlNodus* n)
{
    i32 k;

    si (!_elementum_est(n) && n->genus != STML_NODUS_DOCUMENTUM)
    {
        redde;
    }
    si (   _elementum_est(n) && n->titulus != NIHIL
        && chorda_aequalis_literis(*n->titulus, "relatum"))
    {
        _relatum_tractare(piscina, exitus, n);
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(n->liberi); k++)
    {
        _quaerere(piscina, exitus,
            *(StmlNodus**)xar_obtinere(n->liberi, k));
    }
}

Xar*
materia_exemplaria_extrahere (
    Piscina* piscina,
  StmlNodus* expansum)
{
    Xar* exitus;

    si (piscina == NIHIL || expansum == NIHIL)
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    _quaerere(piscina, exitus, expansum);
    redde exitus;
}
