/* md_arbor.c - Vide md_arbor.h.
 *
 * A3 bloci folia, A4 CONTINENTIA (CommonMark appendix A, forma
 * linearum). Acervus continentium apertorum (documentum, citationes,
 * listae, elementa); per lineam TRES gradus:
 *
 *   I.  continentia aperta ordine congruere: citatio marcam '>' poscit,
 *       elementum indentationem >= offset contenti aut lineam vacuam
 *       (nisi vacuum incepit et adhuc vacuum est), lista semper (eius
 *       elementum decernit). Primum non congruens sistit: quae infra
 *       sunt NON CONGRUUNT.
 *   II. initia continentium novorum in reliquo: '>' et marcae
 *       listarum (divisio vincit; elementum paragraphum interrumpit
 *       solum si non vacuum et, numeratum, initio I). Non congruentia
 *       clauduntur ANTE aperturam.
 *   III. reliquum folio: subductio (paragrapho aperto ET omnibus
 *       congruentibus), divisio, ATX, saeptum, html, aliter
 *       continuatio PIGRA paragraphi aperti (non congruentia manent
 *       aperta!) aut codex indentatus aut paragraphus novus.
 *
 * MARCAE CONTINENTIUM PRAEFIXA LINEAE SUNT (spec par. III): quidquid
 * gradus I et II consumunt in listam praefixorum cadit, quam folium
 * lineae possidens accipit (praefixa blocci, praefixa fracturae mollis,
 * praefixa lineae). Continentia octetos nullos possident.
 *
 * LINEAE VACUAE PENDENT: numquam statim ponuntur; linea non vacua
 * proxima continens congruens ostendit (lista -> elementum ultimum:
 * vacua elementa SEPARAT et laxitatem facit; codex indentatus si
 * pergit; finis documenti -> documentum). Laxitas listae in clausura
 * computatur: elementum non ultimum vacua desinens, aut duo bloci
 * elementi vacua separati. 'nudus' paragraphorum tunc ponitur
 * (semel scribitur), ceteri in fine ZEPHYRUM.
 *
 * TABULAE: tabula partim sumpta non scinditur (lexema unum, columnae
 * residuae ad contentum per md_scan_indentatio iterum numerantur -
 * excessus tabulae perit; corpus tabulas ducentes ZEPHYRUM habet).
 * '>' + tabula: tabula tota sumitur.
 */

#include "md_arbor.h"
#include "md_lexema.h"
#include "md_lexicon.h"
#include "md_registrum.h"
#include "md_decoctum.h"
#include "md_inlinea.h"
#include "materia_arbor.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>

#define MD_PROFUNDITAS_MAXIMA  ((i32)64)
#define MD_PRAEFIXA_MAXIMA     ((i32)160)


/* ==================================================
 * Praefixa lineae
 * ================================================== */

nomen structura {
    s32 genus;
    s32 ab;
    s32 ad;
} MdPraefixum;

nomen structura {
            i32 n;
    MdPraefixum v[MD_PRAEFIXA_MAXIMA];
} MdPraefixa;

interior vacuum
_praefixum (
    MdPraefixa* pf,
    MdLexGenus  genus,
           s32  ab,
           s32  ad)
{
    si (ad <= ab || pf->n >= MD_PRAEFIXA_MAXIMA)
    {
        redde;
    }
    pf->v[pf->n].genus  = (s32)genus;
    pf->v[pf->n].ab     = ab;
    pf->v[pf->n].ad     = ad;
    pf->n               = pf->n + I;
}


/* ==================================================
 * Continentia
 * ================================================== */

nomen structura {
          MdGenus  genus;
     MateriaNodus* nodus;
              i32  offset;           /* elementum: columnae contenti a
                                      * sede parentis */
              b32 initium_vacuum;   /* elementum: linea prima vacua */
              b32 habet_contentum;  /* elementum: blocus non vacuus */
              b32 numerata;         /* lista */
        character delimitator;      /* lista */
} MdContinens;


/* ==================================================
 * Status parsurae
 * ================================================== */

nomen structura {
          chorda  clavis;   /* titulus normalizatus (trim, spatia I, minusculae ASCII) */
    MateriaNodus* nodus;
} MdDefinitioClavis;

nomen structura {
                Piscina* piscina;
     constans character* fons;
               MdLineae  lineae;
              MdFabrica  fabrica;
           MateriaNodus* documentum;

            MdContinens acervus[MD_PROFUNDITAS_MAXIMA];
                    i32 profunditas;

    /* folium apertum (in continente summo) */
           MateriaNodus* paragraphus;
           MateriaNodus* inlinea;
                    i32  paragraphus_ultima;
           MateriaNodus* saeptum;
                    b32  saeptum_saeptus;
              character  saeptum_signum;
                    i32  saeptum_longitudo;
           MateriaNodus* html; i32 html_conditio;
    /* tabula aperta: ordinationes ex ordine separatore */
    MateriaNodus* tabula;
             i32  tabula_numerus;
             i32  tabula_ordinationes[MD_CELLAE_MAXIMAE];

    /* lineae vacuae pendentes */
                   Xar* vacuae;              /* Xar de MateriaNodus* */
    /* paragraphi omnes (nudus in fine) */
                   Xar* paragraphi;
    /* elementum in hac linea apertum (officium in fine lineae) */
          MateriaNodus* elementum_novus;
                   i32  officium_pendens;
    /* definitiones nexuum: clavis normalizata -> nodus (prima vincit) */
    Xar* definitiones;
} MdParsura;


/* ==================================================
 * Adiumenta constructionis
 * ================================================== */

interior MateriaNodus*
_nodus (
    MdParsura* p,
      MdGenus  genus)
{
    redde materia_nodus_creare(p->piscina, (s32)genus,
        MD_REGISTRUM.genera[genus].loci_numerus);
}

interior MateriaToken*
_lexema (
     MdParsura* p,
    MdLexGenus  genus,
           i32  linea,
           s32  ab,
           s32  ad)
{
    redde md_lexema_fabricare(&p->fabrica, (s32)genus, linea, ab, ad);
}

interior b32
_ponere_lexema (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* lexema)
{
    si (lexema == NIHIL)
    {
        redde VERUM;
    }
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_token(lexema),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_appendere_lexema (
       MdParsura* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* lexema)
{
    si (lexema == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_token(lexema), MATERIA_LOCUS_LISTA_TOKEN);
}

interior b32
_appendere_nodum (
       MdParsura* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_NODUS);
}

interior b32
_ponere_nodum (
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_nodus(filius),
        MATERIA_LOCUS_NODUS);
}

interior b32
_ponere_indicem (
    MateriaNodus* nodus,
             i32  locus,
             i32  index)
{
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_index((s32)index),
        MATERIA_LOCUS_INDEX);
}

interior b32
_praefixa_ponere (
              MdParsura* p,
           MateriaNodus* nodus,
                    i32  locus,
                    i32  linea,
    constans MdPraefixa* pf)
{
    i32 i;

    per (i = ZEPHYRUM; i < pf->n; i++)
    {
        si (!_appendere_lexema(p, nodus, locus,
                _lexema(p, (MdLexGenus)pf->v[i].genus, linea,
                pf->v[i].ab,
                    pf->v[i].ad)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Nodus 'linea': praefixa, contentum [post, ad), finis */
interior MateriaNodus*
_linea (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad,
             MdLexGenus  genus_contenti)
{
    MateriaNodus* n = _nodus(p, MD_GENUS_LINEA);

    si (   n == NIHIL
        || !_praefixa_ponere(p, n, (i32)MD_LINEA_PRAEFIXA, linea, pf))
    {
        redde NIHIL;
    }
    {
        s32 i = post;

        dum (i < ad)
        {
            s32 fin = md_scissio_proxima(p->fons, i, ad);

            si (!_appendere_lexema(p, n, (i32)MD_LINEA_CONTENTUM,
                    _lexema(p, genus_contenti, linea, i, fin)))
            {
                redde NIHIL;
            }
            i = fin;
        }
    }
    si (!_ponere_lexema(n, (i32)MD_LINEA_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde NIHIL;
    }
    redde n;
}

interior MateriaNodus*
_vacua (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf)
{
    MateriaNodus* n = _nodus(p, MD_GENUS_LINEA_VACUA);

    si (   n == NIHIL
        || !_praefixa_ponere(p, n, (i32)MD_VACUA_PRAEFIXA, linea, pf))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(n, (i32)MD_VACUA_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde NIHIL;
    }
    redde n;
}


/* ==================================================
 * Acervus et positio blocorum
 * ================================================== */

interior MdContinens*
_summum (
    MdParsura* p)
{
    redde &p->acervus[p->profunditas - I];
}

interior i32
_locus_blocorum (
    constans MdContinens* c)
{
    commutatio (c->genus)
    {
    casus MD_GENUS_CITATIO:   redde (i32)MD_CITATIO_BLOCI;
    casus MD_GENUS_ELEMENTUM: redde (i32)MD_ELEMENTUM_BLOCI;
    casus MD_GENUS_LISTA:     redde (i32)MD_LISTA_ELEMENTA;
    ordinarius:               redde (i32)MD_DOCUMENTUM_BLOCI;
    }
}

interior MateriaNodus*
_elementum_ultimum (
    constans MateriaNodus* lista)
{
    i32 n = materia_valor_lista_numerus(lista->loci[MD_LISTA_ELEMENTA]);
    MateriaValor* v;

    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(lista->loci[MD_LISTA_ELEMENTA], n
        - I);
    redde (v != NIHIL
        && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus
                                                          : NIHIL;
}

interior b32
_vacuas_effundere (
       MdParsura* p,
    MateriaNodus* nodus,
             i32  locus)
{
    i32 i;
    i32 n = xar_numerus(p->vacuae);

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* v = *(MateriaNodus**)xar_obtinere(p->vacuae, i);

        si (!_appendere_nodum(p, nodus, locus, v))
        {
            redde FALSUM;
        }
    }
    xar_vacare(p->vacuae);
    redde VERUM;
}

/* Vacuas pendentes in continens summum effundere: lista -> elementum
 * eius ultimum (vacua elementa separat), aliter bloci eius. */
interior b32
_vacuas_in_summum (
    MdParsura* p)
{
    MdContinens* c = _summum(p);

    si (xar_numerus(p->vacuae) == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (c->genus == MD_GENUS_LISTA)
    {
        MateriaNodus* ultimum = _elementum_ultimum(c->nodus);

        si (ultimum == NIHIL)
        {
            redde FALSUM;
        }
        redde _vacuas_effundere(p, ultimum, (i32)MD_ELEMENTUM_BLOCI);
    }
    redde _vacuas_effundere(p, c->nodus, _locus_blocorum(c));
}

/* Blocum in continens summum ponere (vacuae pendentes ante) */
interior b32
_blocum_addere (
       MdParsura* p,
    MateriaNodus* blocus)
{
    MdContinens* c = _summum(p);

    si (!_vacuas_in_summum(p))
    {
        redde FALSUM;
    }
    si (   c->genus      == MD_GENUS_ELEMENTUM
        && blocus->genus != (s32)MD_GENUS_LINEA_VACUA)
    {
        c->habet_contentum = VERUM;
    }
    redde _appendere_nodum(p, c->nodus, _locus_blocorum(c), blocus);
}


/* ==================================================
 * Paragraphus
 * ================================================== */

interior MateriaNodus*
_textus_crudus (
    MdParsura* p,
          i32  linea,
          s32  ab,
          s32  ad)
{
    MateriaNodus* t = _nodus(p, MD_GENUS_TEXTUS);

    si (   t == NIHIL || !_appendere_lexema(p, t, (i32)MD_TEXTUS_CRUDUM,
            _lexema(p, MD_LEX_TEXTUS, linea, ab, ad)))
    {
        redde NIHIL;
    }
    redde t;
}

interior b32
_paragraphum_aperire (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* par = _nodus(p, MD_GENUS_PARAGRAPHUS);
    MateriaNodus* inl = _nodus(p, MD_GENUS_INLINEA);
    MateriaNodus** locus;

    si (par == NIHIL || inl == NIHIL)
    {
        redde FALSUM;
    }
    si (!_praefixa_ponere(p, par, (i32)MD_PARAGRAPHUS_PRAEFIXA, linea,
        pf))
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(par, (i32)MD_PARAGRAPHUS_INLINEA, inl))
    {
        redde FALSUM;
    }
    si (!_appendere_nodum(p, inl, (i32)MD_INLINEA_LIBERI,
            _textus_crudus(p, linea, post, ad)))
    {
        redde FALSUM;
    }
    locus = (MateriaNodus**)xar_addere(p->paragraphi);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    *locus                 = par;
    p->paragraphus         = par;
    p->inlinea             = inl;
    p->paragraphus_ultima  = linea;
    redde VERUM;
}

interior b32
_paragraphum_continuare (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* fr = _nodus(p, MD_GENUS_FRACTURA_MOLLIS);

    si (fr == NIHIL)
    {
        redde FALSUM;
    }
    si (!_ponere_lexema(fr, (i32)MD_MOLLIS_FINIS,
            md_lexema_terminator(&p->fabrica, p->paragraphus_ultima)))
    {
        redde FALSUM;
    }
    si (!_praefixa_ponere(p, fr, (i32)MD_MOLLIS_PRAEFIXA, linea, pf))
    {
        redde FALSUM;
    }
    si (!_appendere_nodum(p, p->inlinea, (i32)MD_INLINEA_LIBERI, fr))
    {
        redde FALSUM;
    }
    si (!_appendere_nodum(p, p->inlinea, (i32)MD_INLINEA_LIBERI,
            _textus_crudus(p, linea, post, ad)))
    {
        redde FALSUM;
    }
    p->paragraphus_ultima = linea;
    redde VERUM;
}


/* ==================================================
 * Definitiones nexuum (par. 4.7) - in initio paragraphi, ad clausuram
 * ================================================== */

nomen structura {
    s32 la;
    s32 lb;            /* titulus intra '[' ']' */
    s32 da;
    s32 db;            /* destinatio (sine '<' '>') */
    s32 ta;
    s32 tb;            /* descriptio (sine citationibus) */
    b32 descriptio_adest;
    i32 lineae;        /* lineae consumptae */
} MdDefinitio;

interior s32
_lexema_ab (
    constans MateriaToken* t)
{
    redde t->byte_offset;
}

interior s32
_lexema_ad (
    constans MateriaToken* t)
{
    redde t->byte_offset + (s32)t->valor.mensura;
}

/* Citationem clausam eiusdem lineae invenire: reddit index clausurae
 * aut -I. Effugia '\' saltantur. */
interior s32
_clausuram_invenire (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character  clausura)
{
    s32 i;

    per (i = ab; i < ad; i++)
    {
        si (fons[i] == '\\' && i + I < ad)
        {
            i = i + I;
            perge;
        }
        si (fons[i] == clausura)
        {
            redde i;
        }
    }
    redde (s32)-I;
}

interior b32
_spatia_sola (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    s32 i;

    per (i = ab; i < ad; i++)
    {
        si (fons[i] != ' ' && fons[i] != '\t')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Definitionem a linea k paragraphi scandere (lexemata = contentum
 * linearum). Formae: '[t]: d "s"' una linea; '[t]:' + d in linea
 * sequenti; d + descriptio in linea sequenti. Descriptio lineam suam
 * claudit (multi-lineae NON sustinentur); titulus lineam non transit. */
interior b32
_definitionem_scandere (
          MdParsura*  p,
       MateriaToken** lineae,
                i32   n,
                i32   k,
        MdDefinitio*  d)
{
    constans character* fons  = p->fons;
                   s32  ab    = _lexema_ab(lineae[k]);
                   s32  ad    = _lexema_ad(lineae[k]);
                   s32  i;
                   i32  linea       = k;
                   b32  non_vacuus  = FALSUM;

    memset(d, ZEPHYRUM, magnitudo(*d));
    si (ab >= ad || fons[ab] != '[')
    {
        redde FALSUM;
    }
    per (i = ab + I; i < ad; i++)
    {
        si (fons[i] == '\\' && i + I < ad)
        {
            i           = i + I;
            non_vacuus  = VERUM;
            perge;
        }
        si (fons[i] == '[')
        {
            redde FALSUM;
        }
        si (fons[i] == ']')
        {
            frange;
        }
        si (fons[i] != ' ' && fons[i] != '\t')
        {
            non_vacuus = VERUM;
        }
    }
    si (i >= ad || !non_vacuus || i - (ab + I) > (s32)999)
    {
        redde FALSUM;
    }
    d->la  = ab + I;
    d->lb  = i;
    i      = i + I;
    si (i >= ad || fons[i] != ':')
    {
        redde FALSUM;
    }
    i = i + I;
    dum (i < ad && (fons[i] == ' ' || fons[i] == '\t'))
    {
        i = i + I;
    }
    si (i >= ad)
    {
        si (linea + I >= n)
        {
            redde FALSUM;
        }
        linea  = linea + I;
        ab     = _lexema_ab(lineae[linea]);
        ad     = _lexema_ad(lineae[linea]);
        i      = ab;
        dum (i < ad && (fons[i] == ' ' || fons[i] == '\t'))
        {
            i = i + I;
        }
        si (i >= ad)
        {
            redde FALSUM;
        }
    }
    si (fons[i] == '<')
    {
        s32 j = _clausuram_invenire(fons, i + I, ad, '>');

        si (j < ZEPHYRUM)
        {
            redde FALSUM;
        }
        d->da  = i + I;
        d->db  = j;
        i      = j + I;
    }
    alioquin
    {
        s32 j            = i;
        i32 profunditas  = ZEPHYRUM;

        dum (   j < ad && fons[j] != ' ' && fons[j] != '\t'
             && (insignatus character)fons[j]
                   > (insignatus character)0x1F)
        {
            si (fons[j] == '\\' && j + I < ad)
            {
                j = j + II;
                perge;
            }
            si (fons[j] == '(')
            {
                profunditas = profunditas + I;
            }
            alioquin si (fons[j] == ')')
            {
                si (profunditas == ZEPHYRUM)
                {
                    frange;
                }
                profunditas = profunditas - I;
            }
            j = j + I;
        }
        si (j == i || profunditas != ZEPHYRUM)
        {
            redde FALSUM;
        }
        d->da  = i;
        d->db  = j;
        i      = j;
    }
    {
        s32 post = i;

        dum (post < ad && (fons[post] == ' ' || fons[post] == '\t'))
        {
            post = post + I;
        }
        si (post >= ad)
        {
            d->lineae = linea - k + I;
            si (linea + I < n)
            {
                s32 tab  = _lexema_ab(lineae[linea + I]);
                s32 tad  = _lexema_ad(lineae[linea + I]);
                s32 t    = tab;

                dum (t < tad && (fons[t] == ' ' || fons[t] == '\t'))
                {
                    t = t + I;
                }
                si (   t < tad
                    && (fons[t] == '"' || fons[t] == '\''
                    || fons[t] == '('))
                {
                    character cl = (fons[t] == '(') ? ')' : fons[t];
                    s32 j = _clausuram_invenire(fons, t + I, tad, cl);

                    si (j >= ZEPHYRUM && _spatia_sola(fons, j + I, tad))
                    {
                        d->ta                = t + I;
                        d->tb                = j;
                        d->descriptio_adest  = VERUM;
                        d->lineae            = linea - k + II;
                    }
                }
            }
            redde VERUM;
        }
        si (post == i)
        {
            redde FALSUM;
        }
        si (   fons[post] == '"' || fons[post] == '\''
            || fons[post] == '(')
        {
            character cl  = (fons[post] == '(') ? ')' : fons[post];
                  s32 j   = _clausuram_invenire(fons, post + I,
                      ad, cl);

            si (j >= ZEPHYRUM && _spatia_sola(fons, j + I, ad))
            {
                d->ta                = post + I;
                d->tb                = j;
                d->descriptio_adest  = VERUM;
                d->lineae            = linea - k + I;
                redde VERUM;
            }
        }
        redde FALSUM;
    }
}

interior b32
_derivatum_ponere (
                MdParsura* p,
             MateriaNodus* nodus,
                      i32  locus,
                      s32  ab,
                      s32  ad,
    constans MateriaToken* origo)
{
       b32 mutatus;
    chorda valor = md_decoquere(p->piscina, p->fons + ab, (i32)(ad
        - ab),
        &mutatus);

    si (valor.datum == NIHIL && ad > ab)
    {
        redde FALSUM;
    }
    redde _ponere_lexema(nodus, locus, md_lexema_derivatum(&p->fabrica,
        (s32)MD_LEX_DERIVATUM, valor, origo));
}

/* Definitiones a fronte paragraphi aperti extrahere: nodi
 * definitio-nexus in continens; reliquum paragraphus NOVUS (lineae
 * migrant) aut nihil. */
interior b32
_definitiones_extrahere (
    MdParsura* p)
{
    MateriaNodus* inl       = p->inlinea;
    MateriaValor  lv        = inl->loci[MD_INLINEA_LIBERI];
             i32  n         = lv.datum.lista.mensura;
             i32  n_lineae  = (n + I) / II;
    MateriaToken** lineae;
    i32            k;
    i32            j;
    i32            consumptae = ZEPHYRUM;

    si (n == ZEPHYRUM || n_lineae > (i32)256)
    {
        redde VERUM;
    }
    {
        constans MateriaValor* v0 = materia_valor_lista_obtinere(lv,
            ZEPHYRUM);
                 MateriaNodus* t0 = v0->datum.nodus;

        si (   t0->genus != (s32)MD_GENUS_TEXTUS
            || p->fons[_lexema_ab(materia_valor_lista_obtinere(
                   t0->loci[MD_TEXTUS_CRUDUM], ZEPHYRUM)->datum.token)]
                       != '[')
        {
            redde VERUM;
        }
    }
    lineae = (MateriaToken**)piscina_allocare(p->piscina,
        (memoriae_index)n_lineae * (memoriae_index)magnitudo(MateriaToken*));
    si (lineae == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n_lineae; k++)
    {
        MateriaNodus* t = materia_valor_lista_obtinere(lv,
            k * II)->datum.nodus;

        lineae[k] =
            materia_valor_lista_obtinere(t->loci[MD_TEXTUS_CRUDUM],
            ZEPHYRUM)->datum.token;
    }
    dum (consumptae < n_lineae)
    {
        MdDefinitio d;
        MateriaNodus* def;
        chorda        clavis;
        b32           nova = VERUM;

        si (!_definitionem_scandere(p, lineae, n_lineae, consumptae,
            &d))
        {
            frange;
        }
        def = _nodus(p, MD_GENUS_DEFINITIO_NEXUS);
        si (def == NIHIL)
        {
            redde FALSUM;
        }
        per (j = consumptae; j < consumptae + d.lineae; j++)
        {
            MateriaNodus* ln = _nodus(p, MD_GENUS_LINEA);
            MateriaValor  praefixa = (j == ZEPHYRUM)
                ? p->paragraphus->loci[MD_PARAGRAPHUS_PRAEFIXA]
                : materia_valor_lista_obtinere(lv, j * II
                    - I)->datum.nodus
                      ->loci[MD_MOLLIS_PRAEFIXA];
            MateriaToken* finis = (j + I < n_lineae)
                ? materia_valor_lista_obtinere(lv, j * II
                    + I)->datum.nodus
                      ->loci[MD_MOLLIS_FINIS].datum.token
                : md_lexema_terminator(&p->fabrica, lineae[j]->linea
                    - I);

            si (ln == NIHIL)
            {
                redde FALSUM;
            }
            si (   praefixa.genus != MATERIA_VALOR_NIHIL
                && !materia_nodus_ponere(ln, (i32)MD_LINEA_PRAEFIXA,
                praefixa,
                       MATERIA_LOCUS_LISTA_TOKEN))
            {
                redde FALSUM;
            }
            {
                s32 la  = _lexema_ab(lineae[j]);
                s32 lz  = _lexema_ad(lineae[j]);
                s32 q   = la;

                dum (q < lz)
                {
                    s32 fin = md_scissio_proxima(p->fons, q, lz);

                    si (!_appendere_lexema(p, ln,
                        (i32)MD_LINEA_CONTENTUM,
                            _lexema(p, MD_LEX_TEXTUS, lineae[j]->linea
                                - I, q, fin)))
                    {
                        redde FALSUM;
                    }
                    q = fin;
                }
            }
            si (   !_ponere_lexema(ln, (i32)MD_LINEA_FINIS, finis)
                || !_appendere_nodum(p, def, (i32)MD_DEFINITIO_LINEAE,
                ln))
            {
                redde FALSUM;
            }
        }
        clavis = md_clavem_normalizare(p->piscina, p->fons, d.la, d.lb);
        si (clavis.datum == NIHIL)
        {
            redde FALSUM;
        }
        si (   !_ponere_lexema(def, (i32)MD_DEFINITIO_TITULUS,
                md_lexema_derivatum(&p->fabrica, (s32)MD_LEX_DERIVATUM,
                clavis,
                    lineae[consumptae]))
            || !_derivatum_ponere(p, def, (i32)MD_DEFINITIO_URL, d.da,
            d.db,
                   lineae[consumptae])
            || (d.descriptio_adest
                && !_derivatum_ponere(p, def,
                (i32)MD_DEFINITIO_DESCRIPTIO,
                       d.ta, d.tb, lineae[consumptae])))
        {
            redde FALSUM;
        }
        {
            i32 m = xar_numerus(p->definitiones);
            i32 q;

            per (q = ZEPHYRUM; q < m; q++)
            {
                MdDefinitioClavis* dc =
                    (MdDefinitioClavis*)xar_obtinere(
                    p->definitiones, q);

                si (   dc->clavis.mensura == clavis.mensura
                    && memcmp(dc->clavis.datum, clavis.datum,
                           (size_t)clavis.mensura) == ZEPHYRUM)
                {
                    nova = FALSUM;
                    frange;
                }
            }
            si (nova)
            {
                MdDefinitioClavis* dc = (MdDefinitioClavis*)xar_addere(
                    p->definitiones);

                si (dc == NIHIL)
                {
                    redde FALSUM;
                }
                dc->clavis  = clavis;
                dc->nodus   = def;
            }
        }
        si (!_blocum_addere(p, def))
        {
            redde FALSUM;
        }
        consumptae = consumptae + d.lineae;
    }
    si (consumptae == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (consumptae >= n_lineae)
    {
        p->paragraphus  = NIHIL;
        p->inlinea      = NIHIL;
        redde VERUM;
    }
    {
        MateriaNodus* par  = _nodus(p, MD_GENUS_PARAGRAPHUS);
        MateriaNodus* ni   = _nodus(p, MD_GENUS_INLINEA);
        MateriaValor  praefixa = materia_valor_lista_obtinere(lv,
            consumptae * II - I)->datum.nodus->loci[MD_MOLLIS_PRAEFIXA];
        MateriaNodus** locus;

        si (par == NIHIL || ni == NIHIL)
        {
            redde FALSUM;
        }
        si (   praefixa.genus != MATERIA_VALOR_NIHIL
            && !materia_nodus_ponere(par, (i32)MD_PARAGRAPHUS_PRAEFIXA,
            praefixa,
                   MATERIA_LOCUS_LISTA_TOKEN))
        {
            redde FALSUM;
        }
        per (j = consumptae * II; j < n; j++)
        {
            si (!_appendere_nodum(p, ni, (i32)MD_INLINEA_LIBERI,
                    materia_valor_lista_obtinere(lv, j)->datum.nodus))
            {
                redde FALSUM;
            }
        }
        si (!_ponere_nodum(par, (i32)MD_PARAGRAPHUS_INLINEA, ni))
        {
            redde FALSUM;
        }
        locus = (MateriaNodus**)xar_addere(p->paragraphi);
        si (locus == NIHIL)
        {
            redde FALSUM;
        }
        *locus          = par;
        p->paragraphus  = par;
        p->inlinea      = ni;
    }
    redde VERUM;
}

interior b32
_paragraphum_claudere (
    MdParsura* p)
{
    si (p->paragraphus == NIHIL)
    {
        redde VERUM;
    }
    si (!_definitiones_extrahere(p))
    {
        redde FALSUM;
    }
    si (p->paragraphus == NIHIL)
    {
        redde VERUM;
    }
    si (!_ponere_lexema(p->paragraphus, (i32)MD_PARAGRAPHUS_FINIS,
            md_lexema_terminator(&p->fabrica, p->paragraphus_ultima)))
    {
        redde FALSUM;
    }
    si (!_blocum_addere(p, p->paragraphus))
    {
        redde FALSUM;
    }
    p->paragraphus  = NIHIL;
    p->inlinea      = NIHIL;
    redde VERUM;
}

interior b32
_setext (
              MdParsura* p,
                    i32  gradus,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* cap = _nodus(p, MD_GENUS_CAPITULUM);
    MateriaNodus* par = p->paragraphus;

    si (cap == NIHIL)
    {
        redde FALSUM;
    }
    si (par->loci[MD_PARAGRAPHUS_PRAEFIXA].genus != MATERIA_VALOR_NIHIL
        && !materia_nodus_ponere(cap, (i32)MD_CAPITULUM_PRAEFIXA,
               par->loci[MD_PARAGRAPHUS_PRAEFIXA],
               MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde FALSUM;
    }
    si (!_ponere_indicem(cap, (i32)MD_CAPITULUM_GRADUS, gradus))
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(cap, (i32)MD_CAPITULUM_INLINEA, p->inlinea))
    {
        redde FALSUM;
    }
    si (!_ponere_lexema(cap, (i32)MD_CAPITULUM_FINIS,
            md_lexema_terminator(&p->fabrica, p->paragraphus_ultima)))
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(cap, (i32)MD_CAPITULUM_SUBDUCTIO,
            _linea(p, linea, pf, post, ad, MD_LEX_SUBDUCTIO)))
    {
        redde FALSUM;
    }
    /* paragraphus in listam paragraphorum manet: nudus in fine ZEPHYRUM
     * (capitulum eum non ostendit - innocuum) */
    p->paragraphus  = NIHIL;
    p->inlinea      = NIHIL;
    redde _blocum_addere(p, cap);
}


/* ==================================================
 * Folia cetera
 * ================================================== */

interior b32
_atx (
              MdParsura* p,
         constans MdAtx* a,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* cap = _nodus(p, MD_GENUS_CAPITULUM);
    MateriaNodus* inl = _nodus(p, MD_GENUS_INLINEA);

    si (cap == NIHIL || inl == NIHIL)
    {
        redde FALSUM;
    }
    si (!_praefixa_ponere(p, cap, (i32)MD_CAPITULUM_PRAEFIXA, linea,
        pf))
    {
        redde FALSUM;
    }
    si (!_ponere_indicem(cap, (i32)MD_CAPITULUM_GRADUS, a->gradus))
    {
        redde FALSUM;
    }
    si (!_ponere_lexema(cap, (i32)MD_CAPITULUM_MARCA,
            _lexema(p, MD_LEX_MARCA_ATX, linea, post, a->marca_ad)))
    {
        redde FALSUM;
    }
    si (   a->contentum_ad > a->marca_ad
        && !_appendere_nodum(p, inl, (i32)MD_INLINEA_LIBERI,
               _textus_crudus(p, linea, a->marca_ad, a->contentum_ad)))
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(cap, (i32)MD_CAPITULUM_INLINEA, inl))
    {
        redde FALSUM;
    }
    si (   a->clausura_ab < ad
        && !_ponere_lexema(cap, (i32)MD_CAPITULUM_CLAUSUM,
               _lexema(p, MD_LEX_CLAUSURA_ATX, linea, a->clausura_ab,
               ad)))
    {
        redde FALSUM;
    }
    si (!_ponere_lexema(cap, (i32)MD_CAPITULUM_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde FALSUM;
    }
    redde _blocum_addere(p, cap);
}

interior b32
_divisio (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* d = _nodus(p, MD_GENUS_DIVISIO);

    si (   d == NIHIL || !_ponere_nodum(d, (i32)MD_DIVISIO_LINEA,
            _linea(p, linea, pf, post, ad, MD_LEX_DIVISIO)))
    {
        redde FALSUM;
    }
    redde _blocum_addere(p, d);
}

interior MateriaNodus*
_limes (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  signum_ad,
                    s32  ad)
{
    MateriaNodus* l = _nodus(p, MD_GENUS_LIMES);

    si (   l == NIHIL
        || !_praefixa_ponere(p, l, (i32)MD_LIMES_PRAEFIXA, linea, pf))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(l, (i32)MD_LIMES_SIGNUM,
            _lexema(p, MD_LEX_SAEPTUM, linea, post, signum_ad)))
    {
        redde NIHIL;
    }
    si (   ad > signum_ad && !_ponere_lexema(l, (i32)MD_LIMES_INFO,
            _lexema(p, MD_LEX_INFO, linea, signum_ad, ad)))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(l, (i32)MD_LIMES_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde NIHIL;
    }
    redde l;
}

interior b32
_saeptum_aperire (
               MdParsura* p,
      constans MdSaeptum* s,
                     i32  linea,
     constans MdPraefixa* pf,
                     s32  post,
                     s32  ad)
{
    MateriaNodus* sa = _nodus(p, MD_GENUS_SAEPTUM);

    si (   sa == NIHIL
        || !_ponere_indicem(sa, (i32)MD_SAEPTUM_FORMA,
        (i32)MD_SAEPTUM_SAEPTUS)
        || !_ponere_nodum(sa, (i32)MD_SAEPTUM_APERTUM,
               _limes(p, linea, pf, post, s->signum_ad, ad)))
    {
        redde FALSUM;
    }
    p->saeptum            = sa;
    p->saeptum_saeptus    = VERUM;
    p->saeptum_signum     = s->signum;
    p->saeptum_longitudo  = s->longitudo;
    redde _blocum_addere(p, sa);
}

interior b32
_indentatum_aperire (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* sa = _nodus(p, MD_GENUS_SAEPTUM);

    si (   sa == NIHIL
        || !_ponere_indicem(sa, (i32)MD_SAEPTUM_FORMA,
        (i32)MD_SAEPTUM_INDENTATUS)
        || !_appendere_nodum(p, sa, (i32)MD_SAEPTUM_LINEAE,
               _linea(p, linea, pf, post, ad, MD_LEX_TEXTUS)))
    {
        redde FALSUM;
    }
    p->saeptum          = sa;
    p->saeptum_saeptus  = FALSUM;
    redde _blocum_addere(p, sa);
}

interior b32
_html_aperire (
              MdParsura* p,
                    i32  conditio,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  cursor,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* h = _nodus(p, MD_GENUS_HTML);

    si (   h == NIHIL || !_appendere_nodum(p, h, (i32)MD_HTML_LINEAE,
            _linea(p, linea, pf, cursor, ad, MD_LEX_HTML)))
    {
        redde FALSUM;
    }
    si (!(conditio <= V
        && md_scan_html_finis(p->fons, post, ad, conditio)))
    {
        p->html           = h;
        p->html_conditio  = conditio;
    }
    redde _blocum_addere(p, h);
}


/* ==================================================
 * Tabulae GFM (par. 4.10)
 * ================================================== */

interior b32
_continet_pipam (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    s32 i;

    per (i = ab; i < ad; i++)
    {
        si (fons[i] == '|')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Ordo: praefixa ex pf aut ex valore listae dato (linea capitis, cuius
 * praefixa in paragrapho vixerunt), cellae ad numerum ordinationum
 * SUPPLETAE (cella sine octetis - transformatio numerare nequit),
 * clausum, finis. */
interior MateriaNodus*
_ordo (
                MdParsura* p,
                      i32  linea,
      constans MdPraefixa* pf,
    constans MateriaValor* praefixa_valor,
                      s32  ab,
                      s32  ad,
             constans i32* ordinationes,
                      i32  n_ord)
{
          MdOrdo  o;
    MateriaNodus* ordo = _nodus(p, MD_GENUS_ORDO);
             i32  n;
             i32  k;
             i32  summa;

    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    n = md_scan_ordo(p->fons, ab, ad, &o);
    si (   pf != NIHIL
        && !_praefixa_ponere(p, ordo, (i32)MD_ORDO_PRAEFIXA, linea, pf))
    {
        redde NIHIL;
    }
    si (   praefixa_valor        != NIHIL
        && praefixa_valor->genus != MATERIA_VALOR_NIHIL
        && !materia_nodus_ponere(ordo, (i32)MD_ORDO_PRAEFIXA,
        *praefixa_valor,
               MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde NIHIL;
    }
    summa = (n > n_ord) ? n : n_ord;
    per (k = ZEPHYRUM; k < summa; k++)
    {
        MateriaNodus* cella  = _nodus(p, MD_GENUS_CELLA);
        MateriaNodus* inl    = _nodus(p, MD_GENUS_INLINEA);

        si (   cella == NIHIL || inl == NIHIL
            || !_ponere_indicem(cella, (i32)MD_CELLA_ORDINATIO,
                   (k
                       < n_ord) ? ordinationes[k] : (i32)MD_ORDINATIO_NULLA))
        {
            redde NIHIL;
        }
        si (k < n)
        {
            constans MdCella* c = &o.cellae[k];

            si (   c->pipa_ad > c->pipa_ab
                && !_ponere_lexema(cella, (i32)MD_CELLA_APERTUM,
                       _lexema(p, MD_LEX_PIPA, linea, c->pipa_ab,
                       c->pipa_ad)))
            {
                redde NIHIL;
            }
            si (   c->ad > c->ab
                && !_appendere_nodum(p, inl, (i32)MD_INLINEA_LIBERI,
                       _textus_crudus(p, linea, c->ab, c->ad)))
            {
                redde NIHIL;
            }
        }
        si (   !_ponere_nodum(cella, (i32)MD_CELLA_INLINEA, inl)
            || !_appendere_nodum(p, ordo, (i32)MD_ORDO_CELLAE, cella))
        {
            redde NIHIL;
        }
    }
    si (   o.clausum_ad > o.clausum_ab
        && !_ponere_lexema(ordo, (i32)MD_ORDO_CLAUSUM,
               _lexema(p, MD_LEX_PIPA, linea, o.clausum_ab,
               o.clausum_ad)))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(ordo, (i32)MD_ORDO_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde NIHIL;
    }
    redde ordo;
}

/* Linea separator sub paragrapho aperto: linea ULTIMA paragraphi fit
 * caput si cellae eius numero ordinationum aequant. Paragraphus unius
 * lineae evanescit; longior TRUNCATUR (prospectus listae liberorum -
 * mensura sua, materia_nodus.h) et clauditur. Reddit I tabula facta,
 * ZEPHYRUM non tabula, -I memoria. */
interior i32
_tabulam_incipere (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad,
           constans i32* ordinationes,
                    i32  n_ord)
{
    MateriaNodus* inl  = p->inlinea;
    MateriaValor* lv   = &inl->loci[MD_INLINEA_LIBERI];
             i32  n    = lv->datum.lista.mensura;
    MateriaNodus*  ultimus;
    MateriaToken*  t;
    MdOrdo         o;
    MateriaValor   praefixa_capitis;
    MateriaNodus*  caput;
    MateriaNodus*  tabula;
    i32            k;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    ultimus = materia_valor_lista_obtinere(*lv, n - I)->datum.nodus;
    si (ultimus->genus != (s32)MD_GENUS_TEXTUS)
    {
        redde ZEPHYRUM;
    }
    t = materia_valor_lista_obtinere(ultimus->loci[MD_TEXTUS_CRUDUM],
        ZEPHYRUM)->datum.token;
    si (md_scan_ordo(p->fons, t->byte_offset,
            t->byte_offset + (s32)t->valor.mensura, &o) != n_ord)
    {
        redde ZEPHYRUM;
    }
    si (   !_continet_pipam(p->fons, post, ad)
        && !_continet_pipam(p->fons, t->byte_offset,
               t->byte_offset + (s32)t->valor.mensura))
    {
        redde ZEPHYRUM;   /* sine pipa ulla: subductio/divisio vincunt */
    }

    /* praefixa lineae capitis: paragraphi (unius lineae) aut fracturae
     * mollis praecedentis */
    si (n == I)
    {
        praefixa_capitis =
            p->paragraphus->loci[MD_PARAGRAPHUS_PRAEFIXA];
        p->paragraphus  = NIHIL;
        p->inlinea      = NIHIL;
    }
    alioquin
    {
        MateriaNodus* mollis = materia_valor_lista_obtinere(*lv, n
            - II)->datum.nodus;

        praefixa_capitis         = mollis->loci[MD_MOLLIS_PRAEFIXA];
        lv->datum.lista.mensura  = n - II;
        p->paragraphus_ultima    = p->paragraphus_ultima - I;
        si (!_paragraphum_claudere(p))
        {
            redde (i32)-I;
        }
    }

    caput = _ordo(p, linea - I, NIHIL, &praefixa_capitis,
        t->byte_offset,
        t->byte_offset + (s32)t->valor.mensura, ordinationes, n_ord);
    tabula = _nodus(p, MD_GENUS_TABULA);
    si (   caput == NIHIL || tabula == NIHIL
        || !_ponere_nodum(tabula, (i32)MD_TABULA_CAPUT, caput)
        || !_ponere_nodum(tabula, (i32)MD_TABULA_SEPARATOR,
               _linea(p, linea, pf, post, ad, MD_LEX_TEXTUS)))
    {
        redde (i32)-I;
    }
    per (k = ZEPHYRUM; k < n_ord; k++)
    {
        p->tabula_ordinationes[k] = ordinationes[k];
    }
    p->tabula_numerus  = n_ord;
    p->tabula          = tabula;
    redde _blocum_addere(p, tabula) ? I : (i32)-I;
}

interior b32
_ordinem_addere (
              MdParsura* p,
                    i32  linea,
    constans MdPraefixa* pf,
                    s32  post,
                    s32  ad)
{
    MateriaNodus* ordo = _ordo(p, linea, pf, NIHIL, post, ad,
        p->tabula_ordinationes, p->tabula_numerus);

    si (ordo == NIHIL)
    {
        redde FALSUM;
    }
    redde _appendere_nodum(p, p->tabula, (i32)MD_TABULA_ORDINES, ordo);
}


/* ==================================================
 * Clausura folii, listarum, continentium
 * ================================================== */

interior b32
_folium_claudere (
    MdParsura* p)
{
    p->saeptum  = NIHIL;
    p->html     = NIHIL;
    p->tabula   = NIHIL;
    redde _paragraphum_claudere(p);
}

/* Laxitas listae et nudus paragraphorum filiorum directorum */
interior b32
_listam_claudere (
    MateriaNodus* lista)
{
    MateriaValor elementa  = lista->loci[MD_LISTA_ELEMENTA];
             i32 n         = materia_valor_lista_numerus(elementa);
             i32 i;
             b32 laxa = FALSUM;

    per (i = ZEPHYRUM; i < n && !laxa; i++)
    {
        MateriaValor* ev  = materia_valor_lista_obtinere(elementa, i);
        MateriaNodus* e   = ev->datum.nodus;
        MateriaValor  bl  = e->loci[MD_ELEMENTUM_BLOCI];
                 i32  m   = materia_valor_lista_numerus(bl);
                 i32  j;
                 b32  post_contentum  = FALSUM;   /* blocus non vacuus visus */
                 b32  vacua_post      = FALSUM;   /* vacua post contentum */

        per (j = ZEPHYRUM; j < m; j++)
        {
            MateriaNodus* b = materia_valor_lista_obtinere(bl,
                j)->datum.nodus;

            si (b->genus == (s32)MD_GENUS_LINEA_VACUA)
            {
                si (post_contentum)
                {
                    vacua_post = VERUM;
                }
            }
            alioquin
            {
                si (vacua_post)
                {
                    laxa = VERUM;   /* duo bloci vacua separati */
                }
                post_contentum  = VERUM;
                vacua_post      = FALSUM;
            }
        }
        si (vacua_post && i < n - I)
        {
            laxa = VERUM;   /* elementa vacua separata */
        }
    }
    si (!_ponere_indicem(lista, (i32)MD_LISTA_LAXA,
        laxa ? I : ZEPHYRUM))
    {
        redde FALSUM;
    }
    /* nudus: paragraphi filii directi elementorum listae STRICTAE */
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* e = materia_valor_lista_obtinere(elementa,
            i)->datum.nodus;
        MateriaValor bl  = e->loci[MD_ELEMENTUM_BLOCI];
                 i32 m   = materia_valor_lista_numerus(bl);
                 i32 j;

        per (j = ZEPHYRUM; j < m; j++)
        {
            MateriaNodus* b = materia_valor_lista_obtinere(bl,
                j)->datum.nodus;

            si (   b->genus == (s32)MD_GENUS_PARAGRAPHUS
                && b->loci[MD_PARAGRAPHUS_NUDUS].genus
                    == MATERIA_VALOR_NIHIL
                && !_ponere_indicem(b, (i32)MD_PARAGRAPHUS_NUDUS,
                       laxa ? ZEPHYRUM : I))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* Continentia supra 'usque' (index acervi) claudere, folio primum */
interior b32
_claudere_usque (
    MdParsura* p,
          i32  usque)
{
    si (!_folium_claudere(p))
    {
        redde FALSUM;
    }
    dum (p->profunditas - I > usque)
    {
        MdContinens* c = _summum(p);

        si (c->genus == MD_GENUS_LISTA && !_listam_claudere(c->nodus))
        {
            redde FALSUM;
        }
        p->profunditas = p->profunditas - I;
    }
    redde VERUM;
}

/* Ante blocum NON-elementum: continentia non congruentia claudere, deinde
 * listam in summo (lista elementa sola accipit) */
interior b32
_claudere_pro_bloco (
    MdParsura* p,
          i32  usque)
{
    si (!_claudere_usque(p, usque))
    {
        redde FALSUM;
    }
    dum (_summum(p)->genus == MD_GENUS_LISTA)
    {
        si (!_listam_claudere(_summum(p)->nodus))
        {
            redde FALSUM;
        }
        p->profunditas = p->profunditas - I;
    }
    redde VERUM;
}

interior b32
_aperire (
       MdParsura* p,
         MdGenus  genus,
    MateriaNodus* nodus)
{
    MdContinens* c;

    si (p->profunditas >= MD_PROFUNDITAS_MAXIMA)
    {
        redde FALSUM;
    }
    c = &p->acervus[p->profunditas];
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->genus        = genus;
    c->nodus        = nodus;
    p->profunditas  = p->profunditas + I;
    redde VERUM;
}


/* ==================================================
 * Columnae
 * ================================================== */

/* Spatia/tabulae a cursor consumere donec 'petitae' columnae coopertae
 * sint (tabula ultra excedere potest). Reddit post; *coopertae. */
interior s32
_columnas_consumere (
    constans character* fons,
                   s32  cursor,
                   s32  ad,
                   i32  columna,
                   i32  petitae,
                   i32* coopertae)
{
    i32 c = columna;
    s32 i = cursor;

    dum (i < ad && (c - columna) < petitae)
    {
        si (fons[i] == '\t')
        {
            c = c + (IV - (c % IV));
        }
        alioquin si (fons[i] == ' ')
        {
            c = c + I;
        }
        alioquin
        {
            frange;
        }
        i = i + I;
    }
    *coopertae = c - columna;
    redde i;
}


/* ==================================================
 * Ansa
 * ================================================== */

/* Elementum novum in continente summo (lista existens aut nova) */
interior b32
_elementum_aperire (
                 MdParsura* p,
    constans MdMarcaListae* m,
                       i32  offset)
{
    MdContinens* c = _summum(p);
    MateriaNodus* lista;
    MateriaNodus* elementum;

    si (!(c->genus == MD_GENUS_LISTA && c->numerata == m->numerata
          && c->delimitator == m->delimitator))
    {
        /* lista nova (lista aperta incompatibilis prius clausa) */
        si (   c->genus == MD_GENUS_LISTA
            && !_claudere_usque(p, p->profunditas - II))
        {
            redde FALSUM;
        }
        lista = _nodus(p, MD_GENUS_LISTA);
        si (   lista == NIHIL
            || !_ponere_indicem(lista, (i32)MD_LISTA_GENUS,
                   m->numerata ? (i32)MD_LISTA_NUMERATA : (i32)MD_LISTA_PUNCTATA)
            || !_ponere_indicem(lista, (i32)MD_LISTA_INITIUM,
            m->initium)
            || !_blocum_addere(p, lista)
            || !_aperire(p, MD_GENUS_LISTA, lista))
        {
            redde FALSUM;
        }
        c               = _summum(p);
        c->numerata     = m->numerata;
        c->delimitator  = m->delimitator;
    }
    elementum = _nodus(p, MD_GENUS_ELEMENTUM);
    si (   elementum == NIHIL || !_blocum_addere(p, elementum)
        || !_aperire(p, MD_GENUS_ELEMENTUM, elementum))
    {
        redde FALSUM;
    }
    c                    = _summum(p);
    c->offset            = offset;
    c->initium_vacuum    = (b32)(m->spatia == ZEPHYRUM);
    p->elementum_novus   = elementum;
    p->officium_pendens  = (i32)MD_OFFICIUM_NULLUM;
    redde VERUM;
}

interior b32
_lineam_parsare (
    MdParsura* p,
          i32  i)
{
    constans MdLinea* l            = md_linea(&p->lineae, i);
                  s32 ab           = l->offset;
                  s32 ad           = ab + (s32)l->mensura;
                  s32 cursor       = ab;
                  i32 columna      = ZEPHYRUM;
                  i32 congruentia  = ZEPHYRUM;   /* index acervi ultimi congruentis */
                  i32 k;
                  s32 post;
                  i32 col;
                  b32 vacua;
           MdPraefixa pf;

    pf.n                = ZEPHYRUM;
    p->elementum_novus  = NIHIL;

    /* ---- I. continentia aperta congruere ---- */
    per (k = I; k < p->profunditas; k++)
    {
        MdContinens* c = &p->acervus[k];

        post   = md_scan_indentatio(p->fons, cursor, ad, columna, &col);
        vacua  = (b32)(post == ad);

        si (c->genus == MD_GENUS_CITATIO)
        {
            s32 m_ad;

            si (vacua || col > III || p->fons[post] != '>')
            {
                frange;
            }
            m_ad = md_scan_citatio(p->fons, post, ad);
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            _praefixum(&pf, MD_LEX_MARCA_CITATIONIS, post, m_ad);
            columna = columna + col + I
                    + ((m_ad - post > I) ? I : ZEPHYRUM);
            cursor  = m_ad;
        }
        alioquin si (c->genus == MD_GENUS_ELEMENTUM)
        {
            si (vacua)
            {
                si (c->initium_vacuum && !c->habet_contentum)
                {
                    frange;   /* elementum vacuum: vacua secunda claudit */
                }
            }
            alioquin
            {
                i32 coopertae;
                s32 fin;

                si (col < c->offset)
                {
                    frange;
                }
                fin = _columnas_consumere(p->fons, cursor, ad, columna,
                    c->offset, &coopertae);
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, fin);
                columna  = columna + coopertae;
                cursor   = fin;
            }
        }
        /* LISTA semper congruit: elementum eius decernit */
        congruentia = k;
    }

    /* ---- folia quae lineam totam sumunt (si omnia congruunt) ---- */
    si (congruentia == p->profunditas - I)
    {
        post   = md_scan_indentatio(p->fons, cursor, ad, columna, &col);
        vacua  = (b32)(post == ad);

        si (p->saeptum != NIHIL && p->saeptum_saeptus)
        {
            s32 sig_ad;

            si (col <= III && md_scan_saeptum_clausum(p->fons, post, ad,
                    p->saeptum_signum, p->saeptum_longitudo, &sig_ad))
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                si (!_ponere_nodum(p->saeptum, (i32)MD_SAEPTUM_CLAUSUM,
                        _limes(p, i, &pf, post, sig_ad, ad)))
                {
                    redde FALSUM;
                }
                p->saeptum = NIHIL;
                redde VERUM;
            }
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            redde _appendere_nodum(p, p->saeptum,
                (i32)MD_SAEPTUM_LINEAE,
                vacua ? _vacua(p, i, &pf)
                      : _linea(p, i, &pf, post, ad, MD_LEX_TEXTUS));
        }
        si (p->html != NIHIL)
        {
            si (p->html_conditio >= VI && vacua)
            {
                p->html = NIHIL;   /* vacua ipsa blocci NON est */
            }
            alioquin
            {
                si (!_appendere_nodum(p, p->html, (i32)MD_HTML_LINEAE,
                        _linea(p, i, &pf, cursor, ad, MD_LEX_HTML)))
                {
                    redde FALSUM;
                }
                si (   p->html_conditio <= V
                    && md_scan_html_finis(p->fons, cursor, ad,
                    p->html_conditio))
                {
                    p->html = NIHIL;
                }
                redde VERUM;
            }
        }
        si (p->saeptum != NIHIL && !p->saeptum_saeptus)
        {
            si (vacua)
            {
                MateriaNodus** locus =
                    (MateriaNodus**)xar_addere(p->vacuae);

                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                si (locus == NIHIL)
                {
                    redde FALSUM;
                }
                *locus = _vacua(p, i, &pf);
                redde (b32)(*locus != NIHIL);
            }
            si (col >= IV)
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                redde _vacuas_effundere(p, p->saeptum,
                    (i32)MD_SAEPTUM_LINEAE)
                    && _appendere_nodum(p, p->saeptum,
                    (i32)MD_SAEPTUM_LINEAE,
                           _linea(p, i, &pf, post, ad, MD_LEX_TEXTUS));
            }
            p->saeptum = NIHIL;
        }
    }
    alioquin si (p->saeptum != NIHIL || p->html != NIHIL)
    {
        /* folium in continente non congruente: clauditur cum eo */
        si (!_claudere_usque(p, congruentia))
        {
            redde FALSUM;
        }
    }

    /* ---- II. initia continentium novorum ---- */
    dum (VERUM)
    {
        MdMarcaListae m;

        post   = md_scan_indentatio(p->fons, cursor, ad, columna, &col);
        vacua  = (b32)(post == ad);
        si (vacua || col > III)
        {
            frange;
        }
        si (p->fons[post] == '>')
        {
            s32 m_ad;

            si (!_claudere_pro_bloco(p, congruentia))
            { redde FALSUM;
            } m_ad = md_scan_citatio(p->fons, post, ad);
            {
                MateriaNodus* q = _nodus(p, MD_GENUS_CITATIO);

                si (   q == NIHIL || !_blocum_addere(p, q)
                    || !_aperire(p, MD_GENUS_CITATIO, q))
                {
                    redde FALSUM;
                }
            }
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            _praefixum(&pf, MD_LEX_MARCA_CITATIONIS, post, m_ad);
            columna = columna + col + I + ((m_ad - post
                > I) ? I : ZEPHYRUM);
            cursor       = m_ad;
            congruentia  = p->profunditas - I;
            perge;
        }
        si (md_scan_divisio(p->fons, post, ad))
        {
            frange;   /* divisio marcam listae vincit */
        }
        si (md_scan_marca_listae(p->fons, post, ad, columna + col, &m))
        {
            i32 n_spatia;
            i32 coopertae;
            s32 marca_ad;

            /* paragraphum interrumpere: non vacuum et (punctata aut I) */
            {
                MdContinens* cc = &p->acervus[congruentia];
                        b32  soror = (b32)(cc->genus == MD_GENUS_LISTA
                                  && cc->numerata == m.numerata
                                  && cc->delimitator == m.delimitator);

                si (   !soror && p->paragraphus != NIHIL
                    && (m.spatia == ZEPHYRUM
                    || (m.numerata && m.initium != I)))
                {
                    frange;
                }
            }
            n_spatia = (m.spatia == ZEPHYRUM
                || m.spatia >= V) ? I : m.spatia;
            marca_ad = (m.spatia == ZEPHYRUM) ? m.marca_ad
                     : _columnas_consumere(p->fons, m.marca_ad, ad,
                           columna + col + m.latitudo, n_spatia,
                           &coopertae);
            si (!_claudere_usque(p, congruentia))
            {
                redde FALSUM;
            }
            si (!_elementum_aperire(p, &m, col + m.latitudo + n_spatia))
            {
                redde FALSUM;
            }
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            _praefixum(&pf,
                m.numerata ? MD_LEX_MARCA_NUMERI : MD_LEX_MARCA_PUNCTI,
                post, marca_ad);
            columna      = columna + col + m.latitudo + n_spatia;
            cursor       = marca_ad;
            congruentia  = p->profunditas - I;
            perge;
        }
        frange;
    }

    /* ---- III. reliquum folio ---- */
    post   = md_scan_indentatio(p->fons, cursor, ad, columna, &col);
    vacua  = (b32)(post == ad);

    si (vacua)
    {
        MateriaNodus** locus;

        /* linea marcae elementi vacui: vacua ELEMENTI est (marca eius
         * praefixum fert), non pendens */
        si (p->elementum_novus != NIHIL)
        {
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            redde _blocum_addere(p, _vacua(p, i, &pf));
        }
        si (!_claudere_usque(p, congruentia))
        {
            redde FALSUM;
        }
        _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
        locus = (MateriaNodus**)xar_addere(p->vacuae);
        si (locus == NIHIL)
        {
            redde FALSUM;
        }
        *locus = _vacua(p, i, &pf);
        redde (b32)(*locus != NIHIL);
    }

    {
              i32 gradus;
            MdAtx atx;
        MdSaeptum s;
              i32 conditio;
              b32 omnia = (b32)(congruentia == p->profunditas
                  - I);

        si (col <= III)
        {
            si (omnia && p->paragraphus != NIHIL)
            {
                i32 ord[MD_CELLAE_MAXIMAE];
                i32 n_sep = md_scan_tabula_separator(p->fons, post, ad,
                    ord,
                    MD_CELLAE_MAXIMAE);

                si (n_sep > ZEPHYRUM && n_sep <= MD_CELLAE_MAXIMAE)
                {
                    i32 r;

                    _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                    r = _tabulam_incipere(p, i, &pf, post, ad, ord,
                        n_sep);
                    si (r != ZEPHYRUM)
                    {
                        redde (b32)(r > ZEPHYRUM);
                    }
                    pf.n = pf.n - ((post > cursor) ? I : ZEPHYRUM);
                }
            }
            si (   omnia && p->paragraphus != NIHIL
                && md_scan_subductio(p->fons, post, ad, &gradus))
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                redde _setext(p, gradus, i, &pf, post, ad);
            }
            si (md_scan_divisio(p->fons, post, ad))
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                redde _claudere_pro_bloco(p, congruentia)
                    && _divisio(p, i, &pf, post, ad);
            }
            si (md_scan_atx(p->fons, post, ad, &atx))
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                redde _claudere_pro_bloco(p, congruentia)
                    && _atx(p, &atx, i, &pf, post, ad);
            }
            si (md_scan_saeptum_apertum(p->fons, post, ad, &s))
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                redde _claudere_pro_bloco(p, congruentia)
                    && _saeptum_aperire(p, &s, i, &pf, post, ad);
            }
            conditio = md_scan_html_initium(p->fons, post, ad,
                (b32)(p->paragraphus != NIHIL));
            si (conditio != ZEPHYRUM)
            {
                redde _claudere_pro_bloco(p, congruentia)
                    && _html_aperire(p, conditio, i, &pf, cursor, post,
                    ad);
            }
        }
        /* continuatio pigra: paragraphus apertus, textus purus */
        si (p->tabula != NIHIL && omnia)
        {
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            redde _ordinem_addere(p, i, &pf, post, ad);
        }
        si (p->paragraphus != NIHIL)
        {
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            redde _paragraphum_continuare(p, i, &pf, post, ad);
        }
        si (!_claudere_pro_bloco(p, congruentia))
        { redde FALSUM;
        } si (col >= IV)
          {
            _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
            redde _indentatum_aperire(p, i, &pf, post, ad);
          }
        /* officium: blocus primus elementi in hac linea aperti */
        si (p->elementum_novus != NIHIL)
        {
            MdMarcaOfficii o;

            si (md_scan_officium(p->fons, post, ad, &o))
            {
                s32 fin = (o.ad < ad) ? o.ad + I : o.ad;

                _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
                _praefixum(&pf, MD_LEX_MARCA_OFFICII, post, fin);
                p->officium_pendens =
                    o.perfectum ? (i32)MD_OFFICIUM_PERFECTUM
                                                  : (i32)MD_OFFICIUM_APERTUM;
                redde _paragraphum_aperire(p, i, &pf, fin, ad);
            }
        }
        _praefixum(&pf, MD_LEX_INDENTATIO, cursor, post);
        redde _paragraphum_aperire(p, i, &pf, post, ad);
    }
}

/* officium elementi in hac linea aperti - semel, in fine lineae */
interior b32
_officium_finire (
    MdParsura* p)
{
    si (p->elementum_novus == NIHIL)
    {
        redde VERUM;
    }
    si (!_ponere_indicem(p->elementum_novus, (i32)MD_ELEMENTUM_OFFICIUM,
            p->officium_pendens))
    {
        redde FALSUM;
    }
    p->elementum_novus = NIHIL;
    redde VERUM;
}

/* Linea '---' exacte (praefatio) */
interior b32
_est_lineola (
    MdParsura* p,
          i32  linea)
{
    constans MdLinea* l = md_linea(&p->lineae, linea);

    redde (b32)(l->mensura == III
                && memcmp(p->fons + l->offset, "---", III) == ZEPHYRUM);
}

interior b32
_praefatio (
    MdParsura* p,
          i32* proxima)
{
    i32 n = md_lineae_numerus(&p->lineae);
    i32 j;
    MateriaNodus* pr;
    MdPraefixa nulla;

    nulla.n = ZEPHYRUM;
    si (n < II || !_est_lineola(p, ZEPHYRUM))
    {
        redde FALSUM;
    }
    per (j = I; j < n; j++)
    {
        si (_est_lineola(p, j))
        {
            frange;
        }
    }
    si (j >= n)
    {
        redde FALSUM;
    }
    pr = _nodus(p, MD_GENUS_PRAEFATIO);
    si (pr == NIHIL)
    {
        redde FALSUM;
    }
    {
        constans MdLinea* l = md_linea(&p->lineae, ZEPHYRUM);

        si (!_ponere_nodum(pr, (i32)MD_PRAEFATIO_APERTUM,
                _linea(p, ZEPHYRUM, &nulla, l->offset,
                    l->offset + (s32)l->mensura, MD_LEX_TEXTUS)))
        {
            redde FALSUM;
        }
    }
    {
        i32 k;

        per (k = I; k < j; k++)
        {
            constans MdLinea* l   = md_linea(&p->lineae, k);
                          s32 ab  = l->offset;
                          s32 ad  = ab + (s32)l->mensura;
                          i32 col;
                          s32 post = md_scan_indentatio(p->fons, ab, ad,
                                         ZEPHYRUM, &col);
                   MdPraefixa pf;
                MateriaNodus* ln;

            pf.n = ZEPHYRUM;
            si (post == ad)
            {
                _praefixum(&pf, MD_LEX_INDENTATIO, ab, post);
                ln = _vacua(p, k, &pf);
            }
            alioquin
            {
                ln = _linea(p, k, &nulla, ab, ad, MD_LEX_TEXTUS);
            }
            si (!_appendere_nodum(p, pr, (i32)MD_PRAEFATIO_LINEAE, ln))
            {
                redde FALSUM;
            }
        }
    }
    {
        constans MdLinea* l = md_linea(&p->lineae, j);

        si (!_ponere_nodum(pr, (i32)MD_PRAEFATIO_CLAUSUM,
                _linea(p, j, &nulla, l->offset, l->offset
                    + (s32)l->mensura,
                    MD_LEX_TEXTUS)))
        {
            redde FALSUM;
        }
    }
    si (!_ponere_nodum(p->documentum, (i32)MD_DOCUMENTUM_PRAEFATIO, pr))
    {
        redde FALSUM;
    }
    *proxima = j + I;
    redde VERUM;
}

/* nudus ZEPHYRUM omnibus paragraphis quibus nondum positus (extra
 * listas strictas) */
interior b32
_paragraphos_finire (
    MdParsura* p)
{
    i32 i;
    i32 n = xar_numerus(p->paragraphi);

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* par = *(MateriaNodus**)xar_obtinere(p->paragraphi,
            i);

        si (par->loci[MD_PARAGRAPHUS_NUDUS].genus == MATERIA_VALOR_NIHIL
            && !_ponere_indicem(par, (i32)MD_PARAGRAPHUS_NUDUS,
            ZEPHYRUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * A7: arbor inlinea post blocos
 * ================================================== */

interior MateriaNodus*
_definitio_quaerere (
    vacuum* datum,
    chorda  clavis)
{
    MdParsura* p = (MdParsura*)datum;
          i32  m = xar_numerus(p->definitiones);
          i32  q;

    per (q = ZEPHYRUM; q < m; q++)
    {
        MdDefinitioClavis* dc =
            (MdDefinitioClavis*)xar_obtinere(p->definitiones, q);

        si (   dc->clavis.mensura == clavis.mensura
            && memcmp(dc->clavis.datum, clavis.datum,
            (size_t)clavis.mensura) == ZEPHYRUM)
        {
            redde dc->nodus;
        }
    }
    redde NIHIL;
}

/* Omnes nodos 'inlinea' arboris visitare (paragraphus, capitulum, cella) */
interior b32
_inlineas_construere (
    MdInlineaContextus* c,
          MateriaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde VERUM;
    }
    si (nodus->genus == (s32)MD_GENUS_INLINEA)
    {
        redde md_inlinea_construere(c, nodus);
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_inlineas_construere(c, v->datum.nodus))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < n; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_inlineas_construere(c, e->datum.nodus))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

MateriaNodus*
md_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MdParsura p;
          i32 i;
          i32 n;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.piscina  = piscina;
    p.fons     = fons;
    si (!md_lineae_scindere(piscina, fons, mensura, &p.lineae))
    {
        redde NIHIL;
    }
    md_fabrica_incipere(&p.fabrica, piscina, &p.lineae);
    p.vacuae      = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    p.paragraphi  = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    p.definitiones = xar_creare(piscina,
        (i32)magnitudo(MdDefinitioClavis));
    p.documentum = _nodus(&p, MD_GENUS_DOCUMENTUM);
    si (   p.vacuae       == NIHIL || p.paragraphi == NIHIL
        || p.definitiones == NIHIL || p.documentum == NIHIL
        || !_aperire(&p, MD_GENUS_DOCUMENTUM, p.documentum))
    {
        redde NIHIL;
    }

    i = ZEPHYRUM;
    n = md_lineae_numerus(&p.lineae);
    si (!_praefatio(&p, &i))
    {
        i = ZEPHYRUM;
    }
    per (; i < n; i++)
    {
        si (!_lineam_parsare(&p, i) || !_officium_finire(&p))
        {
            redde NIHIL;
        }
    }
    si (!_claudere_usque(&p, ZEPHYRUM))
    {
        redde NIHIL;
    }
    si (!_vacuas_effundere(&p, p.documentum, (i32)MD_DOCUMENTUM_BLOCI))
    {
        redde NIHIL;
    }
    si (!_paragraphos_finire(&p))
    {
        redde NIHIL;
    }
    {
        MdInlineaContextus c;

        c.piscina             = piscina;
        c.fons                = fons;
        c.fabrica             = &p.fabrica;
        c.definitio_quaerere  = _definitio_quaerere;
        c.datum               = &p;
        si (!_inlineas_construere(&c, p.documentum))
        {
            redde NIHIL;
        }
    }
    si (!_ponere_lexema(p.documentum, (i32)MD_DOCUMENTUM_FINIS,
            md_lexema_finis(&p.fabrica)))
    {
        redde NIHIL;
    }
    materia_arbor_patres_figere(piscina, p.documentum);
    redde p.documentum;
}
