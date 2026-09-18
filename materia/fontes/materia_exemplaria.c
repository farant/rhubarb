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
    si (xar_numerus(series) == ZEPHYRUM)
    {
        /* ORDO SINE SEDE TRANSIT, TRACTU VACUO. Inventum locari non
         * potest (regula nodum nullum cepit, e.g. '<situs/>'), sed
         * TACITE CADERE id ipsum est quod hic arcus prohibere vult:
         * regula quae bis congruit et nihil reddit a regula quae nihil
         * invenit non differret. Ergo ordo superest et consumptor eum
         * NOMINAT - materia_diagnostica_plena eum RECUSAT, ut
         * extractor prior faciebat.
         *
         * Mensuratum 2026-09-18: forma prior hic 'redde' habebat dum
         * commentarium hoc ipsum promittebat - commentarium consilium
         * servavit quod codex numquam accepit. */
        d->tractus.initium  = (s32)-I;
        d->tractus.finis    = (s32)-I;
        redde;
    }
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
 * Compositio documenti
 * ================================================== */

/* TRANSPARENTIA ordinaria clientium materiae. Per SUBSTRATUM est, non
 * per clientem: involucra triviae ('ante' 'post') et attributa
 * visionis sedium ('sedes' 'octeti') littera congruentiae fierent
 * aliter, et capturae iteratae sub visione sedium numquam
 * congruerent. */
#define TRANS_TAGS       "ante post"
#define TRANS_ATTRIBUTA  "sedes octeti"

interior b32
_transparentiam_habet (
    StmlNodus* regula)
{
    i32 k;

    si (regula == NIHIL)
    {
        redde FALSUM;
    }
    si (   _elementum_est(regula) && regula->titulus != NIHIL
        && chorda_aequalis_literis(*regula->titulus, "TRANSPARENTIA"))
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(regula->liberi); k++)
    {
        si (_transparentiam_habet(
                *(StmlNodus**)xar_obtinere(regula->liberi, k)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_liberos_transferre (
    StmlNodus* ad,
    StmlNodus* ex)
{
    i32 k;

    si (ex == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(ex->liberi); k++)
    {
        si (!stml_liberum_addere(ad,
                *(StmlNodus**)xar_obtinere(ex->liberi, k)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

StmlNodus*
materia_exemplaria_componere (
              Piscina* piscina,
            StmlNodus* proiectio,
            StmlNodus* regula,
  InternamentumChorda* intern)
{
    StmlNodus* documentum;

    si (   piscina == NIHIL || proiectio == NIHIL || regula == NIHIL
        || intern  == NIHIL)
    {
        redde NIHIL;
    }
    /* NODUS DOCUMENTI, non elementum: EXEMPLAR/CATENA/relatum GRADU
     * DOCUMENTI agnoscuntur (documenta stml par. XIX), ergo intra
     * elementum inclusa expansori INVISIBILIA sunt et regula nihil
     * invenit. Constructor documenti non exstat, ergo documentum
     * VACUUM parsatur - via una quae nodum recti generis dat.
     * (Mensuratum: prima forma elementum 'documentum' creabat et
     * gradus II ordines ZERO reddebat, proiectione recta.) */
    {
        StmlResultus r = stml_legere_ex_literis("<!--documentum-->",
            piscina, intern);

        si (!r.successus || r.radix == NIHIL)
        {
            redde NIHIL;
        }
        documentum = r.radix;
    }
    /* PROIECTIO PRIMA, deinde regula - ordo quem expansor poscit:
     * regula proiectionem SUPRA se quaerit.
     *
     * LIMES RELATORUM HIC NON EST: Python marcam
     * '<exemplaria-limes-relatorum/>' interponit quia textum EXPANSUM
     * scindit et partem posteriorem solam ad HTML vertit (arbor ipsa
     * attributa sine valore fert quae forma HTML recusat). Extractio
     * ex ARBORE elementa <relatum> directe invenit, ergo marca cum
     * causa sua evanescit. */
    si (!_liberos_transferre(documentum, proiectio))
    {
        redde NIHIL;
    }
    si (!_transparentiam_habet(regula))
    {
        StmlNodus* t = stml_elementum_creare(piscina, intern,
                           "TRANSPARENTIA");

        si (   t == NIHIL
            || !stml_attributum_addere(t, piscina, intern, "tags",
                   TRANS_TAGS)
            || !stml_attributum_addere(t, piscina, intern,
                   "attributa", TRANS_ATTRIBUTA)
            || !stml_liberum_addere(documentum, t))
        {
            redde NIHIL;
        }
    }
    si (!_liberos_transferre(documentum, regula))
    {
        redde NIHIL;
    }
    redde documentum;
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


/* ==================================================
 * Subtractio declarata
 * ================================================== */

nomen structura {
    constans character* auxiliaris;   /* lint bracchii ipsius */
    constans character* destinatum;   /* quod 'minuit' nominat */
} SubtractioDeclarata;

/* an codex ('lint:X') lintrem nominatam ferat */
interior b32
_codex_lintris (
    constans character* codex,
    constans character* lint)
{
    si (codex == NIHIL || lint == NIHIL)
    {
        redde FALSUM;
    }
    si (strncmp(codex, "lint:", V) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde (b32)(strcmp(codex + V, lint) == ZEPHYRUM);
}

/* an ordo ullus lintris nominatae sedem hanc teneat */
interior b32
_sedes_adest (
        constans Xar* ordines,
  constans character* lint,
                 s32  initium)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(ordines, k);

        si (   _codex_lintris(d->codex, lint)
            && d->tractus.initium == initium)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Bracchia auxiliaria declarata colligere ('<relatum minuit="X">'). */
interior vacuum
_subtractiones_quaerere (
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
        constans character* lint    = _attributum(piscina, n, "lint");
        constans character* minuit  = _attributum(piscina, n, "minuit");

        si (lint != NIHIL && minuit != NIHIL)
        {
            SubtractioDeclarata* s =
                (SubtractioDeclarata*)xar_addere(exitus);

            si (s != NIHIL)
            {
                s->auxiliaris = lint;
                s->destinatum = minuit;
            }
        }
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(n->liberi); k++)
    {
        _subtractiones_quaerere(piscina, exitus,
            *(StmlNodus**)xar_obtinere(n->liberi, k));
    }
}

/* Causa refutationis, sede prima vaga nominata. */
interior constans character*
_causa_vaga (
                      Piscina* piscina,
 constans SubtractioDeclarata* s,
                          s32  initium)
{
    character* c = (character*)piscina_allocare(piscina, CCLVI);

    si (c == NIHIL)
    {
        redde NIHIL;
    }
    sprintf(c, "bracchium '%s' sedem %d extra '%s' tenet - bracchia "
        "sedes diversas capiunt, differentia nihil significat",
        s->auxiliaris, (integer)initium, s->destinatum);
    redde c;
}

Xar*
materia_exemplaria_minuere (
               Piscina*  piscina,
             StmlNodus*  expansum,
          constans Xar*  ordines,
    constans character** causa)
{
    Xar* subtractiones;
    Xar* exitus;
    i32  k;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    si (piscina == NIHIL || expansum == NIHIL || ordines == NIHIL)
    {
        redde NIHIL;
    }
    subtractiones = xar_creare(piscina,
        (i32)magnitudo(SubtractioDeclarata));
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (subtractiones == NIHIL || exitus == NIHIL)
    {
        redde NIHIL;
    }
    _subtractiones_quaerere(piscina, subtractiones, expansum);
    /* CUSTODIA ANTE OMNIA: sedes bracchii auxiliaris quae in destinato
     * non est regulam FRACTAM nominat, non numerum minorem. */
    per (k = ZEPHYRUM; k < xar_numerus(subtractiones); k++)
    {
        constans SubtractioDeclarata* s =
            (constans SubtractioDeclarata*)xar_obtinere(subtractiones,
                k);
        i32 j;

        per (j = ZEPHYRUM; j < xar_numerus(ordines); j++)
        {
            constans MateriaDiagnosticum* d =
                (constans MateriaDiagnosticum*)xar_obtinere(ordines, j);

            si (   _codex_lintris(d->codex, s->auxiliaris)
                && !_sedes_adest(ordines, s->destinatum,
                       d->tractus.initium))
            {
                si (causa != NIHIL)
                {
                    *causa = _causa_vaga(piscina, s,
                                 d->tractus.initium);
                }
                redde NIHIL;
            }
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(ordines, k);
        b32 tollendus = FALSUM;
        i32 j;

        per (j = ZEPHYRUM;
             j < xar_numerus(subtractiones) && !tollendus; j++)
        {
            constans SubtractioDeclarata* s =
                (constans SubtractioDeclarata*)xar_obtinere(
                    subtractiones, j);

            si (_codex_lintris(d->codex, s->auxiliaris))
            {
                /* bracchium auxiliare inventum non est */
                tollendus = VERUM;
            }
            alioquin si (   _codex_lintris(d->codex, s->destinatum)
                         && _sedes_adest(ordines, s->auxiliaris,
                                d->tractus.initium))
            {
                tollendus = VERUM;
            }
        }
        si (!tollendus)
        {
            MateriaDiagnosticum* cella =
                (MateriaDiagnosticum*)xar_addere(exitus);

            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = *d;
        }
    }
    redde exitus;
}


/* ==================================================
 * Nomina lintrium declaratarum
 * ================================================== */

interior vacuum
_lintres_quaerere (
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
        constans character* lint = _attributum(piscina, n, "lint");

        si (lint != NIHIL)
        {
            i32 j;
            b32 iam = FALSUM;

            per (j = ZEPHYRUM; j < xar_numerus(exitus) && !iam; j++)
            {
                si (strcmp(*(constans character**)xar_obtinere(exitus,
                        j), lint) == ZEPHYRUM)
                {
                    iam = VERUM;
                }
            }
            si (!iam)
            {
                constans character** cella =
                    (constans character**)xar_addere(exitus);

                si (cella != NIHIL)
                {
                    *cella = lint;
                }
            }
        }
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(n->liberi); k++)
    {
        _lintres_quaerere(piscina, exitus,
            *(StmlNodus**)xar_obtinere(n->liberi, k));
    }
}

Xar*
materia_exemplaria_lintres (
         Piscina* piscina,
    constans Xar* regulae)
{
    Xar* exitus;
    i32  k;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(constans character*));
    si (exitus == NIHIL || regulae == NIHIL)
    {
        redde exitus;
    }
    per (k = ZEPHYRUM; k < xar_numerus(regulae); k++)
    {
        _lintres_quaerere(piscina, exitus,
            *(StmlNodus**)xar_obtinere(regulae, k));
    }
    redde exitus;
}
