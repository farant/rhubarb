/* xml_arbor.c - Aedificator xml (vide xml_arbor.h)
 *
 * Acervus graduum (Xar); gradus summus lexema tractat aut se claudit
 * et lexema gradui infra reddit (retractatio). Nodi parenti statim
 * appenduntur (listae materiae solum appenduntur; ordo octetorum =
 * ordo appendendi).
 */

#include "xml_arbor.h"
#include "xml_lector.h"
#include "xml_lexicon.h"
#include "xml_registrum.h"
#include "materia_diagnosticum.h"
#include "materia_token.h"
#include <string.h>


/* ==================================================
 * GradusArboris
 * ================================================== */

nomen enumeratio {
    GRADUS_DOCUMENTUM = 0,
    GRADUS_TAG,              /* tag initialis: attributa */
    GRADUS_CONTENTUM,        /* elementum apertum */
    GRADUS_TAG_FINALIS,      /* post '</titulus': '>' exspectat */
    GRADUS_DECLARATIO_XML,
    GRADUS_DOCTYPE,
    GRADUS_PARS_INTERNA,
    GRADUS_DECLARATIO,
    GRADUS_MALUM_TAG         /* tag finalis solus: usque ad '>' */
} GradusGenus;

/* status per genus:
 *   TAG, DECLARATIO_XML  attributum pendens: 0 nullum (aut perfectum),
 *                        1 '=' exspectat, 2 valorem exspectat
 *   DOCTYPE              0 titulum, 1 id externum, 2 post partem
 *                        internam */
nomen structura {
      GradusGenus  genus;
     MateriaNodus* nodus;
     MateriaNodus* attributum;   /* pendens (TAG, DECLARATIO_XML) */
     MateriaNodus* malum;        /* malum apertum huius loci */
              i32  status;
              i32  basis;       /* liberi pendentes ab hoc indice */
} GradusArboris;

nomen structura {
          Piscina* piscina;
        XmlLector  lector;
              Xar* gradus;      /* GradusArboris */
              Xar* pendentia;   /* MateriaToken* trivia */
              Xar* liberi;      /* MateriaValor pendentes (acervus) */
     MateriaToken* prior;       /* lexema non trivium ultimum */
     MateriaNodus* documentum;
       XmlParsura* relatio;
              Xar* diagnostica;
              b32  memoria_defecit;
              i32  mala;
              b32  fragmentum_partis; /* gradus imus PARS_INTERNA */
} Aedificatio;

interior GradusArboris*
_vertex (
    Aedificatio* p)
{
    i32 n = xar_numerus(p->gradus);

    redde n > ZEPHYRUM ? (GradusArboris*)xar_obtinere(p->gradus, n
        - I) : NIHIL;
}

interior s32
_locus_liberorum (
    constans GradusArboris* g);
interior b32
_ponere_listam (
     Aedificatio* p,
    MateriaNodus* nodus,
             s32  locus,
             Xar* lista);

/* liberi pendentes gradus in listam EXACTAM loci sui (ante mutationem
 * generis aut extollationem) */
interior vacuum
_figere (
      Aedificatio* p,
    GradusArboris* g)
{
    i32  n = xar_numerus(p->liberi) - g->basis;
    i32  k;
    Xar* x;

    si (n == ZEPHYRUM)
    {
        redde;
    }
    x = xar_creare_cum_magnitudine(p->piscina,
        (i32)magnitudo(MateriaValor), n);
    si (x == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        MateriaValor* d = (MateriaValor*)xar_addere(x);

        si (d == NIHIL)
        {
            p->memoria_defecit = VERUM;
            redde;
        }
        *d = *(MateriaValor*)xar_obtinere(p->liberi, g->basis + k);
    }
    (vacuum)_ponere_listam(p, g->nodus, _locus_liberorum(g), x);
    xar_truncare(p->liberi, g->basis);
}

interior vacuum
_extollere (
    Aedificatio* p)
{
    si (xar_numerus(p->gradus) > I)
    {
        _figere(p, (GradusArboris*)xar_obtinere(p->gradus,
            xar_numerus(p->gradus) - I));
        xar_removere_ultimum(p->gradus);
    }
}

interior MateriaNodus*
_nodus (
      Aedificatio* p,
         XmlGenus  genus)
{
    MateriaNodus* n = materia_nodus_creare(p->piscina, (s32)genus,
        XML_REGISTRUM.genera[genus].loci_numerus);

    si (n == NIHIL)
    {
        p->memoria_defecit = VERUM;
    }
    redde n;
}

interior MateriaLocusSpecies
_species (
    s32 genus,
    i32 locus)
{
    redde (MateriaLocusSpecies)XML_REGISTRUM.loci[
        XML_REGISTRUM.genera[genus].loci_offset + locus].species;
}

interior vacuum
_ponere_token (
     Aedificatio* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    si (   nodus == NIHIL || !materia_nodus_ponere(nodus, locus,
        materia_valor_token(t), _species(nodus->genus, locus)))
    {
        p->memoria_defecit = VERUM;
    }
}

interior vacuum
_appendere_token (
     Aedificatio* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    si (   nodus == NIHIL || !materia_nodus_appendere(p->piscina, nodus,
        locus, materia_valor_token(t), MATERIA_LOCUS_LISTA_TOKEN))
    {
        p->memoria_defecit = VERUM;
    }
}

interior vacuum
_ponere_nodum (
     Aedificatio* p,
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (   nodus == NIHIL || filius == NIHIL
        || !materia_nodus_ponere(nodus, locus,
            materia_valor_nodus(filius), MATERIA_LOCUS_NODUS))
    {
        p->memoria_defecit = VERUM;
    }
}

interior b32
_ponere_listam (
     Aedificatio* p,
    MateriaNodus* nodus,
             s32  locus,
             Xar* lista)
{
    si (   nodus == NIHIL || locus < ZEPHYRUM
        || !materia_nodus_ponere(nodus, (i32)locus,
        materia_valor_lista(lista),
            MATERIA_LOCUS_LISTA_NODUS))
    {
        p->memoria_defecit = VERUM;
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_impellere (
     Aedificatio* p,
     GradusGenus  genus,
    MateriaNodus* nodus,
             i32  status)
{
     GradusArboris* g = (GradusArboris*)xar_addere(p->gradus);
               i32  n;

    si (g == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    g->genus       = genus;
    g->nodus       = nodus;
    g->attributum  = NIHIL;
    g->malum       = NIHIL;
    g->status      = status;
    g->basis       = xar_numerus(p->liberi);
    n              = xar_numerus(p->gradus);
    si (p->relatio != NIHIL && n > p->relatio->profunditas_maxima)
    {
        p->relatio->profunditas_maxima = n;
    }
}

/* lexema in locum liberorum gradus (malum apertum aut novum). -I si
 * gradus liberos non habet: s32, quia i32 insignatum est (ibi '< 0'
 * numquam verum) */
interior s32
_locus_liberorum (
    constans GradusArboris* g)
{
    commutatio (g->genus)
    {
        casus GRADUS_DOCUMENTUM:
            redde (s32)XML_DOCUMENTUM_LIBERI;
        casus GRADUS_CONTENTUM:
            redde (s32)XML_ELEMENTUM_LIBERI;
        casus GRADUS_TAG:
            redde (s32)XML_ELEMENTUM_ATTRIBUTA;
        casus GRADUS_DECLARATIO_XML:
            redde (s32)XML_DECLARATIO_XML_ATTRIBUTA;
        casus GRADUS_PARS_INTERNA:
            redde (s32)XML_PARS_INTERNA_LIBERI;
        ordinarius:
            redde -(s32)I;
    }
}

/* nodus in liberos gradus; malum apertum clauditur */
interior vacuum
_liberum (
           Aedificatio* p,
         GradusArboris* g,
          MateriaNodus* filius)
{
    s32 locus = _locus_liberorum(g);
    MateriaValor* d;

    g->malum = NIHIL;
    si (locus < ZEPHYRUM || filius == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    d = (MateriaValor*)xar_addere(p->liberi);
    si (d == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    *d = materia_valor_nodus(filius);
}

/* lexema in malum loci gradus (contigua in malum unum) */
interior vacuum
_malum (
           Aedificatio* p,
         GradusArboris* g,
          MateriaToken* t)
{
    si (g->malum == NIHIL)
    {
        MateriaNodus* m = _nodus(p, XML_GENUS_MALUM);

        _liberum(p, g, m);
        g->malum = m;
        p->mala++;
    }
    _appendere_token(p, g->malum, (i32)XML_MALUM_TOKENS, t);
}

interior vacuum
_lexema_solum (
            Aedificatio* p,
          GradusArboris* g,
               XmlGenus  genus,
           MateriaToken* t)
{
    MateriaNodus* n = _nodus(p, genus);

    _ponere_token(p, n, (i32)XML_LEXEMA_TOK, t);
    _liberum(p, g, n);
}


/* ==================================================
 * Diagnostica emissa
 * ================================================== */

interior vacuum
_culpa (
              Aedificatio* p,
    constans MateriaToken* lexema,
    constans MateriaNodus* nodus,
       constans character* codex,
       constans character* causa)
{
    MateriaDiagnosticum* d;

    d = (MateriaDiagnosticum*)xar_addere(p->diagnostica);
    si (d == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(MateriaDiagnosticum));
    d->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex     = codex;
    d->causa     = causa;
    d->nodus     = nodus;
    d->lexema    = lexema;
    si (lexema != NIHIL)
    {
        materia_tractus_lexematis(NIHIL, lexema, &d->tractus);
    }
    alioquin si (   nodus == NIHIL
                 || !materia_tractus_nodi(NIHIL, nodus, &d->tractus))
    {
        d->tractus.initium = (s32)-I;
    }
}


/* ==================================================
 * Ligator
 * ================================================== */

interior vacuum
_cumulare (
     Aedificatio* p,
    MateriaToken* t)
{
    MateriaToken** sedes = (MateriaToken**)xar_addere(p->pendentia);

    si (sedes == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    *sedes = t;
}

/* valor quem scriptor STML cum triviis ferre nequit (materia_arbor.c
 * _textus_tutus): NUL, album solum, aut margo alba cum linea nova */
interior b32
_tutus (
    chorda v)
{
    s32 primus   = (s32)-I;
    s32 ultimus  = (s32)-I;
    i32 i;

    si (v.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < v.mensura; i++)
    {
        character c = (character)v.datum[i];

        si (c == '\0')
        {
            redde FALSUM;
        }
        si (   c != ' ' && c != '\t' && c != '\n' && c != '\r'
            && c != '\f' && c != '\v')
        {
            si (primus < ZEPHYRUM)
            {
                primus = (s32)i;
            }
            ultimus = (s32)i;
        }
    }
    si (primus < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < (i32)primus; i++)
    {
        si (v.datum[i] == '\n')
        {
            redde FALSUM;
        }
    }
    per (i = (i32)ultimus + I; i < v.mensura; i++)
    {
        si (v.datum[i] == '\n')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* trivia pendentia omnia ANTE sequentis - nisi valor sequentis STML
 * cum triviis non fert (valor citatus apertus usque ad finem fontis,
 * linea nova in margine): tum POST prioris (si prior tutus est) */
interior vacuum
_solvere (
     Aedificatio* p,
    MateriaToken* sequens)
{
              i32 numerus = xar_numerus(p->pendentia);
    MateriaToken** plana;
              i32  j;
    MateriaToken*  locale[VIII];

    si (numerus == ZEPHYRUM)
    {
        redde;
    }
    /* series temporaria: trivia_*_ponere seriem exactam ipsa copiat */
    plana = numerus <= VIII ? locale
        : (MateriaToken**)piscina_allocare_ordinatum(p->piscina,
        (memoriae_index)magnitudo(MateriaToken*)
            * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(p->pendentia, j);
    }
    si (   !_tutus(sequens->valor) && p->prior != NIHIL
        && _tutus(p->prior->valor)
        && p->prior->numerus_post == ZEPHYRUM)
    {
        si (!materia_token_trivia_post_ponere(p->prior, p->piscina,
            plana, numerus))
        {
            p->memoria_defecit = VERUM;
        }
    }
    alioquin si (!materia_token_trivia_ante_ponere(sequens, p->piscina,
                 plana, numerus))
    {
        p->memoria_defecit = VERUM;
    }
    xar_vacare(p->pendentia);
}


/* ==================================================
 * Tituli elementorum
 * ================================================== */

/* titulus elementi ex '<titulus' */
interior chorda
_titulus_elementi (
    constans MateriaNodus* elementum)
{
    constans MateriaValor* v =
        &elementum->loci[XML_ELEMENTUM_TOK_APERTURA];
                   chorda c;

    c = v->datum.token->valor;
    c.datum++;
    c.mensura--;
    redde c;
}

/* titulus ex '</titulus' aequat titulum elementi? */
interior b32
_titulus_aequat (
    constans MateriaNodus* elementum,
    constans MateriaToken* clausura)
{
    chorda a = _titulus_elementi(elementum);

    redde clausura->valor.mensura == a.mensura + II
        && memcmp(clausura->valor.datum + II, a.datum,
               (size_t)a.mensura) == ZEPHYRUM;
}


/* ==================================================
 * GradusArboris per genus
 * ================================================== */

/* lexema tractatum (VERUM) aut gradus clausus et lexema gradui infra
 * reddendum (FALSUM) */

interior b32
_contentum (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_TEXTUS:
            _lexema_solum(p, g, XML_GENUS_TEXTUS, t);
            redde VERUM;
        casus XML_LEX_REFERENTIA:
            _lexema_solum(p, g, XML_GENUS_REFERENTIA, t);
            redde VERUM;
        casus XML_LEX_CDATA:
            _lexema_solum(p, g, XML_GENUS_CDATA, t);
            redde VERUM;
        casus XML_LEX_COMMENTARIUM:
            _lexema_solum(p, g, XML_GENUS_COMMENTARIUM, t);
            redde VERUM;
        casus XML_LEX_INSTRUCTIO:
            _lexema_solum(p, g, XML_GENUS_INSTRUCTIO, t);
            redde VERUM;
        casus XML_LEX_SIGNUM_ORDINIS:
            si (g->genus == GRADUS_DOCUMENTUM)
            {
                _lexema_solum(p, g, XML_GENUS_SIGNUM_ORDINIS, t);
                redde VERUM;
            }
            frange;
        casus XML_LEX_TAG_APERTURA:
        {
            MateriaNodus* e = _nodus(p, XML_GENUS_ELEMENTUM);

            _ponere_token(p, e, (i32)XML_ELEMENTUM_TOK_APERTURA, t);
            _liberum(p, g, e);
            _impellere(p, GRADUS_TAG, e, ZEPHYRUM);
            redde VERUM;
        }
        casus XML_LEX_TAG_CLAUSURA:
        {
            s32 k;

            /* elementum apertum eiusdem tituli in acervo (gradus
             * contenti contigui supra documentum) */
            per (k = (s32)xar_numerus(p->gradus) - I; k
                >= ZEPHYRUM; k--)
            {
                GradusArboris* h =
                    (GradusArboris*)xar_obtinere(p->gradus, (i32)k);

                si (h->genus != GRADUS_CONTENTUM)
                {
                    k = (s32)-I;
                    frange;
                }
                si (_titulus_aequat(h->nodus, t))
                {
                    frange;
                }
            }
            si (k < ZEPHYRUM)
            {
                MateriaNodus* m = _nodus(p, XML_GENUS_MALUM);

                _liberum(p, g, m);
                p->mala++;
                _appendere_token(p, m, (i32)XML_MALUM_TOKENS, t);
                _culpa(p, t, m, XML_CODEX_TAG_FINALIS_SOLUS,
                    "tag finalis sine tag initiali eiusdem tituli");
                _impellere(p, GRADUS_MALUM_TAG, m, ZEPHYRUM);
                redde VERUM;
            }
            /* elementa intermedia sine tag finali */
            dum ((s32)xar_numerus(p->gradus) - I > k)
            {
                GradusArboris* h = _vertex(p);

                _culpa(p, h->nodus->loci[XML_ELEMENTUM_TOK_APERTURA]
                    .datum.token, h->nodus, XML_CODEX_TAG_FINALIS_DEEST,
                    "tag finalis deest (WFC: Element Type Match)");
                _extollere(p);
            }
            g = _vertex(p);
            _ponere_token(p, g->nodus, (i32)XML_ELEMENTUM_TOK_CLAUSURA,
                t);
            _figere(p, g);
            g->genus = GRADUS_TAG_FINALIS;
            redde VERUM;
        }
        casus XML_LEX_DECLARATIO_XML:
        {
            MateriaNodus* d = _nodus(p, XML_GENUS_DECLARATIO_XML);

            _ponere_token(p, d, (i32)XML_DECLARATIO_XML_TOK_APERTURA,
                t);
            _liberum(p, g, d);
            _impellere(p, GRADUS_DECLARATIO_XML, d, ZEPHYRUM);
            redde VERUM;
        }
        casus XML_LEX_DOCTYPE:
        {
            MateriaNodus* d = _nodus(p, XML_GENUS_DOCTYPE);

            _ponere_token(p, d, (i32)XML_DOCTYPE_TOK_APERTURA, t);
            _liberum(p, g, d);
            _impellere(p, GRADUS_DOCTYPE, d, ZEPHYRUM);
            redde VERUM;
        }
        casus XML_LEX_FINIS:
            si (g->genus == GRADUS_DOCUMENTUM)
            {
                _ponere_token(p, g->nodus, (i32)XML_DOCUMENTUM_CAUDA,
                    t);
                redde VERUM;
            }
            _culpa(p, g->nodus->loci[XML_ELEMENTUM_TOK_APERTURA]
                .datum.token, g->nodus, XML_CODEX_TAG_FINALIS_DEEST,
                "tag finalis deest ante finem fontis");
            redde FALSUM;
        ordinarius:
            frange;
    }
    _malum(p, g, t);
    redde VERUM;
}

/* attributa (TAG et DECLARATIO_XML) */
interior b32
_attributa (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_TITULUS:
        {
            MateriaNodus* a = _nodus(p, XML_GENUS_ATTRIBUTUM);

            _ponere_token(p, a, (i32)XML_ATTRIBUTUM_TOK_TITULUS, t);
            _liberum(p, g, a);
            g->attributum  = a;
            g->status      = I;
            redde VERUM;
        }
        casus XML_LEX_AEQUALE:
            si (g->attributum != NIHIL && g->status == I)
            {
                _ponere_token(p, g->attributum,
                    (i32)XML_ATTRIBUTUM_TOK_AEQUALE, t);
                g->status = II;
                redde VERUM;
            }
            frange;
        casus XML_LEX_VALOR:
            si (g->attributum != NIHIL && g->status == II)
            {
                _ponere_token(p, g->attributum,
                    (i32)XML_ATTRIBUTUM_TOK_VALOR, t);
                g->status      = ZEPHYRUM;
                g->attributum  = NIHIL;
                redde VERUM;
            }
            frange;
        casus XML_LEX_IGNOTUM:
            frange;
        casus XML_LEX_TAG_FINIS:
            si (g->genus == GRADUS_TAG)
            {
                _ponere_token(p, g->nodus, (i32)XML_ELEMENTUM_TOK_FINIS,
                    t);
                _figere(p, g);
                g->genus       = GRADUS_CONTENTUM;
                g->attributum  = NIHIL;
                g->malum       = NIHIL;
                redde VERUM;
            }
            frange;
        casus XML_LEX_TAG_FINIS_SOLUS:
            si (g->genus == GRADUS_TAG)
            {
                _ponere_token(p, g->nodus, (i32)XML_ELEMENTUM_TOK_FINIS,
                    t);
                _extollere(p);
                redde VERUM;
            }
            frange;
        casus XML_LEX_INSTRUCTIO_FINIS:
            si (g->genus == GRADUS_DECLARATIO_XML)
            {
                _ponere_token(p, g->nodus,
                    (i32)XML_DECLARATIO_XML_TOK_CLAUSURA, t);
                _extollere(p);
                redde VERUM;
            }
            frange;
        ordinarius:
            /* lexema structurale (post '<' relectum, aut FINIS): tag
             * non clausus - elementum apertum manet */
            si (g->genus == GRADUS_TAG)
            {
                _figere(p, g);
                g->genus       = GRADUS_CONTENTUM;
                g->attributum  = NIHIL;
                g->malum       = NIHIL;
                redde _contentum(p, g, t);
            }
            redde FALSUM;
    }
    g->attributum  = NIHIL;
    g->status      = ZEPHYRUM;
    _malum(p, g, t);
    redde VERUM;
}

interior b32
_tag_finalis (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    si (t->genus == (s32)XML_LEX_TAG_FINIS)
    {
        _ponere_token(p, g->nodus,
            (i32)XML_ELEMENTUM_TOK_CLAUSURA_FINIS,
            t);
        _extollere(p);
        redde VERUM;
    }
    _culpa(p, g->nodus->loci[XML_ELEMENTUM_TOK_CLAUSURA].datum.token,
        g->nodus, XML_CODEX_TAG_FINALIS_FINIS,
        "'>' deest post tag finalem");
    redde FALSUM;
}

interior b32
_malum_tag (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_TITULUS:
        casus XML_LEX_AEQUALE:
        casus XML_LEX_VALOR:
        casus XML_LEX_IGNOTUM:
            _appendere_token(p, g->nodus, (i32)XML_MALUM_TOKENS, t);
            redde VERUM;
        casus XML_LEX_TAG_FINIS:
        casus XML_LEX_TAG_FINIS_SOLUS:
            _appendere_token(p, g->nodus, (i32)XML_MALUM_TOKENS, t);
            _extollere(p);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
_doctype (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_TITULUS:
            si (g->status == ZEPHYRUM)
            {
                _ponere_token(p, g->nodus, (i32)XML_DOCTYPE_TOK_TITULUS,
                    t);
                g->status = I;
                redde VERUM;
            }
            si (g->status == I)
            {
                _appendere_token(p, g->nodus,
                    (i32)XML_DOCTYPE_ID_EXTERNUM, t);
                redde VERUM;
            }
            frange;
        casus XML_LEX_VALOR:
            si (g->status <= I)
            {
                _appendere_token(p, g->nodus,
                    (i32)XML_DOCTYPE_ID_EXTERNUM, t);
                g->status = I;
                redde VERUM;
            }
            frange;
        casus XML_LEX_PARS_INTERNA_APERTURA:
            si (g->status <= I)
            {
                MateriaNodus* pi = _nodus(p, XML_GENUS_PARS_INTERNA);

                _ponere_token(p, pi, (i32)XML_PARS_INTERNA_TOK_APERTURA,
                    t);
                _ponere_nodum(p, g->nodus,
                    (i32)XML_DOCTYPE_PARS_INTERNA,
                    pi);
                g->status = II;
                _impellere(p, GRADUS_PARS_INTERNA, pi, ZEPHYRUM);
                redde VERUM;
            }
            frange;
        casus XML_LEX_TAG_FINIS:
            _ponere_token(p, g->nodus, (i32)XML_DOCTYPE_TOK_CLAUSURA,
                t);
            _extollere(p);
            redde VERUM;
        ordinarius:
            frange;
    }
    redde FALSUM;
}

interior b32
_pars_interna (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_DECLARATIO:
        {
            MateriaNodus* d = _nodus(p, XML_GENUS_DECLARATIO);

            _ponere_token(p, d, (i32)XML_DECLARATIO_TOK_APERTURA, t);
            _liberum(p, g, d);
            _impellere(p, GRADUS_DECLARATIO, d, ZEPHYRUM);
            redde VERUM;
        }
        casus XML_LEX_REFERENTIA_PARAMETRI:
            _lexema_solum(p, g, XML_GENUS_REFERENTIA_PARAMETRI, t);
            redde VERUM;
        casus XML_LEX_COMMENTARIUM:
            _lexema_solum(p, g, XML_GENUS_COMMENTARIUM, t);
            redde VERUM;
        casus XML_LEX_INSTRUCTIO:
            _lexema_solum(p, g, XML_GENUS_INSTRUCTIO, t);
            redde VERUM;
        casus XML_LEX_PARS_INTERNA_CLAUSURA:
            _ponere_token(p, g->nodus,
                (i32)XML_PARS_INTERNA_TOK_CLAUSURA,
                t);
            _extollere(p);
            redde VERUM;
        casus XML_LEX_FINIS:
            /* fragmentum partis internae: gradus imus FINIS accipit */
            redde p->fragmentum_partis && xar_numerus(p->gradus) == I;
        ordinarius:
            frange;
    }
    _malum(p, g, t);
    redde VERUM;
}

interior b32
_declaratio (
            Aedificatio* p,
          GradusArboris* g,
           MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus XML_LEX_TAG_FINIS:
            _ponere_token(p, g->nodus, (i32)XML_DECLARATIO_TOK_CLAUSURA,
                t);
            _extollere(p);
            redde VERUM;
        casus XML_LEX_DECLARATIO:
        casus XML_LEX_PARS_INTERNA_CLAUSURA:
        casus XML_LEX_COMMENTARIUM:
        casus XML_LEX_INSTRUCTIO:
        casus XML_LEX_FINIS:
            redde FALSUM;
        ordinarius:
            _appendere_token(p, g->nodus, (i32)XML_DECLARATIO_PARTES,
                t);
            redde VERUM;
    }
}

interior vacuum
_tractare (
     Aedificatio* p,
    MateriaToken* t)
{
    dum (!p->memoria_defecit)
    {
         GradusArboris* g = _vertex(p);
                   b32  tractatum;

        si (g == NIHIL)
        {
            p->memoria_defecit = VERUM;
            redde;
        }
        commutatio (g->genus)
        {
            casus GRADUS_DOCUMENTUM:
            casus GRADUS_CONTENTUM:
                tractatum = _contentum(p, g, t);
                frange;
            casus GRADUS_TAG:
            casus GRADUS_DECLARATIO_XML:
                tractatum = _attributa(p, g, t);
                frange;
            casus GRADUS_TAG_FINALIS:
                tractatum = _tag_finalis(p, g, t);
                frange;
            casus GRADUS_MALUM_TAG:
                tractatum = _malum_tag(p, g, t);
                frange;
            casus GRADUS_DOCTYPE:
                tractatum = _doctype(p, g, t);
                frange;
            casus GRADUS_PARS_INTERNA:
                tractatum = _pars_interna(p, g, t);
                frange;
            casus GRADUS_DECLARATIO:
                tractatum = _declaratio(p, g, t);
                frange;
            ordinarius:
                tractatum = VERUM;
                frange;
        }
        si (tractatum)
        {
            redde;
        }
        /* gradus clausus (gradus documenti numquam - FINIS ibi
         * tractatur): lexema gradui infra */
        _extollere(p);
    }
}


/* ==================================================
 * Ordo documenti
 * ================================================== */

interior b32
_spatia_sola (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character b = (character)c.datum[i];

        si (b != ' ' && b != '\t' && b != '\n' && b != '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_ordinem_iudicare (
    Aedificatio* p)
{
    MateriaValor liberi  = p->documentum->loci[XML_DOCUMENTUM_LIBERI];
             i32 n       = materia_valor_lista_numerus(liberi);
             i32 k;
             i32 radices             = ZEPHYRUM;
             i32 declarationes_typi  = ZEPHYRUM;

    per (k = ZEPHYRUM; k < n; k++)
    {
        MateriaNodus* f = materia_valor_lista_obtinere(liberi, k)
                              ->datum.nodus;

        commutatio (f->genus)
        {
            casus XML_GENUS_ELEMENTUM:
                radices++;
                si (radices > I)
                {
                    _culpa(p, f->loci[XML_ELEMENTUM_TOK_APERTURA]
                        .datum.token, f, XML_CODEX_RADIX_ALTERA,
                        "elementum radicis alterum (documentum radicem"
                        " unam habet)");
                }
                frange;
            casus XML_GENUS_DOCTYPE:
                declarationes_typi++;
                si (radices > ZEPHYRUM || declarationes_typi > I)
                {
                    _culpa(p, f->loci[XML_DOCTYPE_TOK_APERTURA]
                        .datum.token, f, XML_CODEX_DOCTYPE_LOCUS,
                        "declaratio typi semel et ante elementum"
                        " radicis");
                }
                frange;
            casus XML_GENUS_TEXTUS:
                si (!_spatia_sola(f->loci[XML_LEXEMA_TOK].datum.token
                    ->valor))
                {
                    _culpa(p, f->loci[XML_LEXEMA_TOK].datum.token, f,
                        XML_CODEX_TEXTUS_EXTRA,
                        "textus extra elementum radicis");
                }
                frange;
            casus XML_GENUS_REFERENTIA:
            casus XML_GENUS_CDATA:
                _culpa(p, f->loci[XML_LEXEMA_TOK].datum.token, f,
                    XML_CODEX_CONTENTUM_EXTRA,
                    "contentum extra elementum radicis");
                frange;
            ordinarius:
                frange;
        }
    }
    si (radices == ZEPHYRUM)
    {
        _culpa(p, p->documentum->loci[XML_DOCUMENTUM_CAUDA].datum.token,
            p->documentum, XML_CODEX_RADIX_DEEST,
            "elementum radicis deest");
    }
}


/* ==================================================
 * Publica
 * ================================================== */

/* fragmentum: -I documentum; aliter XmlFragmentum */
interior MateriaNodus*
_parsare (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
                   s32  fragmentum,
            XmlParsura* relatio)
{
    Aedificatio p;
            b32 lector_bonus;

    memset(&p, ZEPHYRUM, magnitudo(p));
    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    p.piscina    = piscina;
    p.relatio    = relatio;
    p.gradus     = xar_creare(piscina, (i32)magnitudo(GradusArboris));
    p.pendentia  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    p.liberi     = xar_creare(piscina, (i32)magnitudo(MateriaValor));
    p.diagnostica = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    si (relatio != NIHIL)
    {
        memset(relatio, ZEPHYRUM, magnitudo(*relatio));
    }
    lector_bonus = fragmentum < ZEPHYRUM
        ? xml_lector_incipere(&p.lector, piscina, fons, mensura)
        : xml_lector_incipere_fragmentum(&p.lector, piscina, fons,
            mensura, fragmentum == (s32)XML_FRAGMENTUM_PARS_INTERNA
                ? XML_MODUS_PARS_INTERNA : XML_MODUS_CONTENTUM);
    si (   p.gradus      == NIHIL || p.pendentia == NIHIL
        || p.diagnostica == NIHIL || p.liberi == NIHIL || !lector_bonus)
    {
        redde NIHIL;
    }
    si (fragmentum == (s32)XML_FRAGMENTUM_PARS_INTERNA)
    {
        p.fragmentum_partis  = VERUM;
        p.documentum         = _nodus(&p, XML_GENUS_PARS_INTERNA);
        _impellere(&p, GRADUS_PARS_INTERNA, p.documentum, ZEPHYRUM);
    }
    alioquin
    {
        p.documentum = _nodus(&p, XML_GENUS_DOCUMENTUM);
        _impellere(&p, GRADUS_DOCUMENTUM, p.documentum, ZEPHYRUM);
    }

    dum (!p.memoria_defecit)
    {
        MateriaToken* t = xml_lector_proximum(&p.lector);

        si (t == NIHIL)
        {
            p.memoria_defecit = VERUM;
            frange;
        }
        si (t->genus == (s32)XML_LEX_SPATIA)
        {
            _cumulare(&p, t);
            perge;
        }
        _solvere(&p, t);
        p.prior = t;
        _tractare(&p, t);
        si (t->genus == (s32)XML_LEX_FINIS)
        {
            frange;
        }
    }
    /* gradus aperti (documentum semper, elementa sine tag finali) */
    dum (!p.memoria_defecit && xar_numerus(p.gradus) > ZEPHYRUM)
    {
        _figere(&p, _vertex(&p));
        xar_removere_ultimum(p.gradus);
    }
    si (p.memoria_defecit)
    {
        redde NIHIL;
    }
    si (fragmentum < ZEPHYRUM)
    {
        _ordinem_iudicare(&p);
    }
    si (relatio != NIHIL)
    {
        relatio->diagnostica  = p.diagnostica;
        relatio->mala         = p.mala;
        relatio->sana        = p.mala == ZEPHYRUM
            && xar_numerus(p.diagnostica) == ZEPHYRUM;
    }
    redde p.memoria_defecit ? NIHIL : p.documentum;
}

MateriaNodus*
xml_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
            XmlParsura* relatio)
{
    redde _parsare(piscina, fons, mensura, (s32)-I, relatio);
}

MateriaNodus*
xml_arbor_parsare_fragmentum (
                Piscina* piscina,
     constans character* fons,
                    s32  mensura,
          XmlFragmentum  genus,
             XmlParsura* relatio)
{
    redde _parsare(piscina, fons, mensura, (s32)genus, relatio);
}
