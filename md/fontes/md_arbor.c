/* md_arbor.c - Vide md_arbor.h.
 *
 * A3: bloci folia. Ansa una super tabulam linearum; status = quod
 * apertum est (paragraphus, saeptum, html) et lineae vacuae pendentes
 * (intra codicem indentatum solae - vacuae ducentes et sequentes
 * codicis non sunt, CommonMark par. 4.4).
 *
 * ORDO IUDICII per lineam (CommonMark par. 4, forma foliorum):
 *   saeptum apertum -> clausum? aliter linea saepti
 *   html apertus    -> finis conditionis? aliter linea html
 *   codex indentatus apertus -> vacua pendet, indentata continuat,
 *                              cetera claudunt
 *   vacua           -> paragraphum claudit, blocus linea-vacua
 *   indentatio >= IV -> continuatio paragraphi, aliter codex indentatus
 *   praefatio (linea I sola) · subductio (paragrapho aperto) ·
 *   divisio · ATX · saeptum apertum · html initium · paragraphus
 *
 * Paragraphus in bloci NON appenditur dum aperitur sed dum CLAUDITUR:
 * subductio setext eum in capitulum vertit sine mutatione listae.
 */

#include "md_arbor.h"
#include "md_lexema.h"
#include "md_lexicon.h"
#include "md_registrum.h"
#include "materia_arbor.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Status parsurae
 * ================================================== */

nomen structura {
                Piscina* piscina;
     constans character* fons;
               MdLineae  lineae;
              MdFabrica  fabrica;
           MateriaNodus* documentum;

    /* paragraphus apertus */
          MateriaNodus* paragraphus;
          MateriaNodus* inlinea;
                   i32  paragraphus_ultima;   /* index lineae ultimae */

    /* saeptum apertum (saeptum aut indentatum) */
           MateriaNodus* saeptum;
                    b32  saeptum_saeptus;
              character  saeptum_signum;
                    i32  saeptum_longitudo;

    /* html apertus */
          MateriaNodus* html;
                   i32  html_conditio;

    /* lineae vacuae pendentes intra codicem indentatum */
                   Xar* vacuae;               /* Xar de MateriaNodus* */
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
        redde VERUM;   /* locus optionalis vacuus */
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

/* Praefixum INDENTATIO in locum LISTA_TOKEN (nihil si vacuum) */
interior b32
_praefixum_indentationis (
       MdParsura* p,
    MateriaNodus* nodus,
             i32  locus,
             i32  linea,
             s32  ab,
             s32  post)
{
    si (post <= ab)
    {
        redde VERUM;
    }
    redde _appendere_lexema(p, nodus, locus,
        _lexema(p, MD_LEX_INDENTATIO, linea, ab, post));
}

/* Nodus 'linea': praefixa [ab, post) ut INDENTATIO, contentum
 * [post, ad) generis dati, finis = terminator. */
interior MateriaNodus*
_linea (
     MdParsura* p,
           i32  linea,
           s32  ab,
           s32  post,
           s32  ad,
    MdLexGenus  genus_contenti)
{
    MateriaNodus* n = _nodus(p, MD_GENUS_LINEA);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (!_praefixum_indentationis(p, n, (i32)MD_LINEA_PRAEFIXA, linea,
        ab,
            post))
    {
        redde NIHIL;
    }
    si (   ad > post && !_ponere_lexema(n, (i32)MD_LINEA_CONTENTUM,
            _lexema(p, genus_contenti, linea, post, ad)))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(n, (i32)MD_LINEA_FINIS,
            md_lexema_terminator(&p->fabrica, linea)))
    {
        redde NIHIL;
    }
    redde n;
}

/* Nodus 'linea-vacua': spatia [ab, post) ut INDENTATIO, finis. */
interior MateriaNodus*
_vacua (
    MdParsura* p,
          i32  linea,
          s32  ab,
          s32  post)
{
    MateriaNodus* n = _nodus(p, MD_GENUS_LINEA_VACUA);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (!_praefixum_indentationis(p, n, (i32)MD_VACUA_PRAEFIXA, linea,
        ab,
            post))
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

/* Vacuas pendentes in locum datum effundere (ordine) */
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

interior b32
_blocum_addere (
       MdParsura* p,
    MateriaNodus* blocus)
{
    si (!_vacuas_effundere(p, p->documentum, (i32)MD_DOCUMENTUM_BLOCI))
    {
        redde FALSUM;
    }
    redde _appendere_nodum(p, p->documentum, (i32)MD_DOCUMENTUM_BLOCI,
        blocus);
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

    si (t == NIHIL)
    {
        redde NIHIL;
    }
    si (!_appendere_lexema(p, t, (i32)MD_TEXTUS_CRUDUM,
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
          s32  ab,
          s32  post,
          s32  ad)
{
    MateriaNodus* par = _nodus(p, MD_GENUS_PARAGRAPHUS);
    MateriaNodus* inl = _nodus(p, MD_GENUS_INLINEA);

    si (par == NIHIL || inl == NIHIL)
    {
        redde FALSUM;
    }
    si (!_praefixum_indentationis(p, par, (i32)MD_PARAGRAPHUS_PRAEFIXA,
            linea, ab, post))
    {
        redde FALSUM;
    }
    si (!_ponere_indicem(par, (i32)MD_PARAGRAPHUS_NUDUS, ZEPHYRUM))
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
    p->paragraphus         = par;
    p->inlinea             = inl;
    p->paragraphus_ultima  = linea;
    redde VERUM;
}

/* Linea sequens: fractura-mollis (terminator lineae prioris + praefixa
 * huius) deinde textus crudus. */
interior b32
_paragraphum_continuare (
    MdParsura* p,
          i32  linea,
          s32  ab,
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
    si (!_praefixum_indentationis(p, fr, (i32)MD_MOLLIS_PRAEFIXA, linea,
        ab,
            post))
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

interior b32
_paragraphum_claudere (
    MdParsura* p)
{
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

/* Subductio setext: paragraphus apertus fit capitulum (praefixa et
 * inlinea migrant), subductio = linea huius. */
interior b32
_setext (
    MdParsura* p,
          i32  gradus,
          i32  linea,
          s32  ab,
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
            _linea(p, linea, ab, post, ad, MD_LEX_SUBDUCTIO)))
    {
        redde FALSUM;
    }
    p->paragraphus  = NIHIL;
    p->inlinea      = NIHIL;
    redde _blocum_addere(p, cap);
}


/* ==================================================
 * Capitulum ATX, divisio, saeptum, html, praefatio
 * ================================================== */

interior b32
_atx (
         MdParsura* p,
    constans MdAtx* a,
               i32  linea,
               s32  ab,
               s32  post,
               s32  ad)
{
    MateriaNodus* cap = _nodus(p, MD_GENUS_CAPITULUM);
    MateriaNodus* inl = _nodus(p, MD_GENUS_INLINEA);

    si (cap == NIHIL || inl == NIHIL)
    {
        redde FALSUM;
    }
    si (!_praefixum_indentationis(p, cap, (i32)MD_CAPITULUM_PRAEFIXA,
        linea,
            ab, post))
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
          s32  ab,
          s32  post,
          s32  ad)
{
    MateriaNodus* d = _nodus(p, MD_GENUS_DIVISIO);

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(d, (i32)MD_DIVISIO_LINEA,
            _linea(p, linea, ab, post, ad, MD_LEX_DIVISIO)))
    {
        redde FALSUM;
    }
    redde _blocum_addere(p, d);
}

/* Linea limitis saepti: praefixa, signum [post, signum_ad), info
 * [signum_ad, ad) si non vacua, finis. */
interior MateriaNodus*
_limes (
    MdParsura* p,
          i32  linea,
          s32  ab,
          s32  post,
          s32  signum_ad,
          s32  ad)
{
    MateriaNodus* l = _nodus(p, MD_GENUS_LIMES);

    si (l == NIHIL)
    {
        redde NIHIL;
    }
    si (!_praefixum_indentationis(p, l, (i32)MD_LIMES_PRAEFIXA, linea,
        ab,
            post))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(l, (i32)MD_LIMES_SIGNUM,
            _lexema(p, MD_LEX_SAEPTUM, linea, post, signum_ad)))
    {
        redde NIHIL;
    }
    si (   ad > signum_ad
        && !_ponere_lexema(l, (i32)MD_LIMES_INFO,
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
                   s32  ab,
                   s32  post,
                   s32  ad)
{
    MateriaNodus* sa = _nodus(p, MD_GENUS_SAEPTUM);

    si (sa == NIHIL)
    {
        redde FALSUM;
    }
    si (!_ponere_indicem(sa, (i32)MD_SAEPTUM_FORMA,
            (i32)MD_SAEPTUM_SAEPTUS))
    {
        redde FALSUM;
    }
    si (!_ponere_nodum(sa, (i32)MD_SAEPTUM_APERTUM,
            _limes(p, linea, ab, post, s->signum_ad, ad)))
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
          s32  ab,
          s32  post,
          s32  ad)
{
    MateriaNodus* sa = _nodus(p, MD_GENUS_SAEPTUM);

    si (sa == NIHIL)
    {
        redde FALSUM;
    }
    si (!_ponere_indicem(sa, (i32)MD_SAEPTUM_FORMA,
            (i32)MD_SAEPTUM_INDENTATUS))
    {
        redde FALSUM;
    }
    si (!_appendere_nodum(p, sa, (i32)MD_SAEPTUM_LINEAE,
            _linea(p, linea, ab, post, ad, MD_LEX_TEXTUS)))
    {
        redde FALSUM;
    }
    p->saeptum          = sa;
    p->saeptum_saeptus  = FALSUM;
    redde _blocum_addere(p, sa);
}

/* Linea html cruda tota (indentatio inclusa) ut lexema HTML */
interior MateriaNodus*
_linea_html (
    MdParsura* p,
          i32  linea,
          s32  ab,
          s32  ad)
{
    redde _linea(p, linea, ab, ab, ad, MD_LEX_HTML);
}

interior b32
_html_aperire (
    MdParsura* p,
          i32  conditio,
          i32  linea,
          s32  ab,
          s32  post,
          s32  ad)
{
    MateriaNodus* h = _nodus(p, MD_GENUS_HTML);

    si (h == NIHIL)
    {
        redde FALSUM;
    }
    si (!_appendere_nodum(p, h, (i32)MD_HTML_LINEAE,
            _linea_html(p, linea, ab, ad)))
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

/* Praefatio: linea I '---', clausum '---' proxima; FALSUM = nulla
 * (vocans pergit). *proxima = index post praefationem. */
interior b32
_praefatio (
    MdParsura* p,
          i32* proxima)
{
    i32 n = md_lineae_numerus(&p->lineae);
    i32 j;
    MateriaNodus* pr;

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
                _linea(p, ZEPHYRUM, l->offset, l->offset,
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
                MateriaNodus* ln = (post == ad) ? _vacua(p, k, ab, post)
                                 : _linea(p, k, ab, ab, ad,
                                 MD_LEX_TEXTUS);

            si (!_appendere_nodum(p, pr, (i32)MD_PRAEFATIO_LINEAE, ln))
            {
                redde FALSUM;
            }
        }
    }
    {
        constans MdLinea* l = md_linea(&p->lineae, j);

        si (!_ponere_nodum(pr, (i32)MD_PRAEFATIO_CLAUSUM,
                _linea(p, j, l->offset, l->offset,
                    l->offset + (s32)l->mensura, MD_LEX_TEXTUS)))
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


/* ==================================================
 * Ansa
 * ================================================== */

interior b32
_lineam_parsare (
    MdParsura* p,
          i32  i)
{
     constans MdLinea* l    = md_linea(&p->lineae, i);
                  s32  ab   = l->offset;
                  s32  ad   = ab + (s32)l->mensura;
                  i32  col  = ZEPHYRUM;
                  s32  post = md_scan_indentatio(p->fons, ab, ad,
                      ZEPHYRUM,
                                 &col);
                  b32 vacua = (b32)(post == ad);

    /* saeptum apertum */
    si (p->saeptum != NIHIL && p->saeptum_saeptus)
    {
        s32 sig_ad;

        si (   col <= III
            && md_scan_saeptum_clausum(p->fons, post, ad,
            p->saeptum_signum,
                   p->saeptum_longitudo, &sig_ad))
        {
            si (!_ponere_nodum(p->saeptum, (i32)MD_SAEPTUM_CLAUSUM,
                    _limes(p, i, ab, post, sig_ad, ad)))
            {
                redde FALSUM;
            }
            p->saeptum = NIHIL;
            redde VERUM;
        }
        redde _appendere_nodum(p, p->saeptum, (i32)MD_SAEPTUM_LINEAE,
            vacua ? _vacua(p, i, ab, post)
                  : _linea(p, i, ab, post, ad, MD_LEX_TEXTUS));
    }

    /* html apertus */
    si (p->html != NIHIL)
    {
        si (p->html_conditio >= VI && vacua)
        {
            p->html = NIHIL;   /* vacua ipsa blocci NON est */
        }
        alioquin
        {
            si (!_appendere_nodum(p, p->html, (i32)MD_HTML_LINEAE,
                    _linea_html(p, i, ab, ad)))
            {
                redde FALSUM;
            }
            si (   p->html_conditio <= V
                && md_scan_html_finis(p->fons, ab, ad,
                p->html_conditio))
            {
                p->html = NIHIL;
            }
            redde VERUM;
        }
    }

    /* codex indentatus apertus */
    si (p->saeptum != NIHIL && !p->saeptum_saeptus)
    {
        si (vacua)
        {
            MateriaNodus** locus =
                (MateriaNodus**)xar_addere(p->vacuae);

            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = _vacua(p, i, ab, post);
            redde (b32)(*locus != NIHIL);
        }
        si (col >= IV)
        {
            si (!_vacuas_effundere(p, p->saeptum,
                (i32)MD_SAEPTUM_LINEAE))
            {
                redde FALSUM;
            }
            redde _appendere_nodum(p, p->saeptum,
                (i32)MD_SAEPTUM_LINEAE,
                _linea(p, i, ab, post, ad, MD_LEX_TEXTUS));
        }
        p->saeptum = NIHIL;   /* vacuae pendentes documento cedunt */
    }

    /* linea vacua */
    si (vacua)
    {
        si (!_paragraphum_claudere(p))
        {
            redde FALSUM;
        }
        redde _blocum_addere(p, _vacua(p, i, ab, post));
    }

    /* indentatio >= IV */
    si (col >= IV)
    {
        si (p->paragraphus != NIHIL)
        {
            redde _paragraphum_continuare(p, i, ab, post, ad);
        }
        redde _indentatum_aperire(p, i, ab, post, ad);
    }

    /* contentum [post, ad), indentatio <= III */
    {
              i32 gradus;
            MdAtx atx;
        MdSaeptum s;
              i32 conditio;

        si (   p->paragraphus != NIHIL
            && md_scan_subductio(p->fons, post, ad, &gradus))
        {
            redde _setext(p, gradus, i, ab, post, ad);
        }
        si (md_scan_divisio(p->fons, post, ad))
        {
            redde _paragraphum_claudere(p)
                && _divisio(p, i, ab, post, ad);
        }
        si (md_scan_atx(p->fons, post, ad, &atx))
        {
            redde _paragraphum_claudere(p)
                && _atx(p, &atx, i, ab, post, ad);
        }
        si (md_scan_saeptum_apertum(p->fons, post, ad, &s))
        {
            redde _paragraphum_claudere(p)
                && _saeptum_aperire(p, &s, i, ab, post, ad);
        }
        conditio = md_scan_html_initium(p->fons, post, ad,
            (b32)(p->paragraphus != NIHIL));
        si (conditio != ZEPHYRUM)
        {
            redde _paragraphum_claudere(p)
                && _html_aperire(p, conditio, i, ab, post, ad);
        }
        si (p->paragraphus != NIHIL)
        {
            redde _paragraphum_continuare(p, i, ab, post, ad);
        }
        redde _paragraphum_aperire(p, i, ab, post, ad);
    }
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
    p.documentum  = _nodus(&p, MD_GENUS_DOCUMENTUM);
    si (p.vacuae == NIHIL || p.documentum == NIHIL)
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
        si (!_lineam_parsare(&p, i))
        {
            redde NIHIL;
        }
    }
    si (!_paragraphum_claudere(&p))
    {
        redde NIHIL;
    }
    si (!_vacuas_effundere(&p, p.documentum, (i32)MD_DOCUMENTUM_BLOCI))
    {
        redde NIHIL;
    }
    si (!_ponere_lexema(p.documentum, (i32)MD_DOCUMENTUM_FINIS,
            md_lexema_finis(&p.fabrica)))
    {
        redde NIHIL;
    }
    materia_arbor_patres_figere(piscina, p.documentum);
    redde p.documentum;
}
