/* toml_arbor.c - Aedificator toml (vide toml_arbor.h)
 *
 * Acervus graduum (Xar); gradus summus lexema tractat aut se claudit
 * et lexema gradui infra reddit (retractatio). Nodus parenti
 * traditur cum gradus eius CLAUDITUR (listae materiae solum
 * appenduntur; ordo octetorum ordo claudendi est).
 */

#include "toml_arbor.h"
#include "toml_lector.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "xar.h"


/* ==================================================
 * GradusAedificationis
 * ================================================== */

nomen enumeratio {
    GRADUS_DOCUMENTUM = 0,
    GRADUS_CAPUT,
    GRADUS_PAR,
    GRADUS_CLAVIS,
    GRADUS_SERIES,
    GRADUS_COMPACTA,
    GRADUS_MALUM
} GradusGenus;

/* status per genus:
 *   CAPUT    0 clavem exspectat, 1 claudentem, 2 finem lineae
 *   PAR      0 clavis aedificatur, 1 '=' exspectat, 2 valorem,
 *            3 finem (linea, ',' aut '}')
 *   CLAVIS   0 segmentum exspectat, 1 punctum aut finem
 *   SERIES   0 valorem aut ']', 1 ',' aut ']'
 *   COMPACTA 0 clavem aut '}', 1 ',' aut '}'
 *   MALUM    modus lectoris (0 CLAVIS, 1 VALOR) */
nomen structura {
     GradusGenus  genus;
    MateriaNodus* nodus;
             i32  status;
} GradusAedificationis;

nomen structura {
                Piscina* piscina;
             TomlLector  lector;
    MateriaLexiconRatum  lexicon;
                    Xar* gradus;      /* GradusAedificationis */
                    Xar* pendentia;   /* MateriaToken* trivia */
           MateriaToken* prior;
                    b32  post_lineam;
            TomlParsura* relatio;
                    b32  memoria_defecit;
                    /* series et tabulae compactae apertae in acervo:
                     * numerus CURRENS (Q11: numeratio per lexema totum
                     * acervum ambulabat - parsura quadratica in
                     * profunditate, C milia uncorum XXVIII s) */
                    i32 unci;
} Aedificatio;

interior GradusAedificationis*
_vertex (
    Aedificatio* p)
{
    i32 n = xar_numerus(p->gradus);

    redde n > ZEPHYRUM ? (GradusAedificationis*)xar_obtinere(p->gradus,
        n
        - I) : NIHIL;
}

interior MateriaNodus*
_nodus (
    Aedificatio* p,
      TomlGenus  genus)
{
    MateriaNodus* n = materia_nodus_creare(p->piscina, (s32)genus,
        TOML_REGISTRUM.genera[genus].loci_numerus);

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
    redde (MateriaLocusSpecies)TOML_REGISTRUM.loci[
        TOML_REGISTRUM.genera[genus].loci_offset + locus].species;
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
_appendere_nodum (
    Aedificatio* p,
   MateriaNodus* nodus,
            i32  locus,
   MateriaNodus* filius)
{
    si (   nodus == NIHIL || filius == NIHIL
        || !materia_nodus_appendere(p->piscina, nodus, locus,
            materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_NODUS))
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

interior vacuum
_impellere (
     Aedificatio* p,
     GradusGenus  genus,
    MateriaNodus* nodus,
             i32  status)
{
     GradusAedificationis* g =
         (GradusAedificationis*)xar_addere(p->gradus);
                      i32 n;

    si (g == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde;
    }
    g->genus   = genus;
    g->nodus   = nodus;
    g->status  = status;
    n          = xar_numerus(p->gradus);
    si (genus == GRADUS_SERIES || genus == GRADUS_COMPACTA)
    {
        p->unci++;
    }
    si (p->relatio != NIHIL && n > p->relatio->profunditas_maxima)
    {
        p->relatio->profunditas_maxima = n;
    }
}


/* ==================================================
 * Ligator et recordatio
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
    si (t->genus == (s32)TOML_LEX_LINEA)
    {
        p->post_lineam = VERUM;
    }
}

/* regula toml (toml_arbor.h): post lineam terminantem aut in initio
 * omnia ANTE sequentis; aliter usque ad LINEA primam POST prioris */
interior vacuum
_solvere (
     Aedificatio* p,
    MateriaToken* sequens)
{
              i32 numerus = xar_numerus(p->pendentia);
    MateriaToken** plana;
              i32  divisio;
              i32  j;

    materia_token_initium_lineae_ponere(sequens, p->post_lineam);
    p->post_lineam = FALSUM;
    si (numerus == ZEPHYRUM)
    {
        p->prior = sequens;
        redde;
    }
    plana = (MateriaToken**)piscina_allocare_ordinatum(p->piscina,
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
    si (   p->prior        == NIHIL
        || p->prior->genus == (s32)TOML_LEX_LINEA_FINIS)
    {
        divisio = ZEPHYRUM;
    }
    alioquin
    {
        divisio = numerus;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (plana[j]->genus == (s32)TOML_LEX_LINEA)
            {
                divisio = j + I;
                frange;
            }
        }
    }
    si (   divisio > ZEPHYRUM
        && !materia_token_trivia_post_ponere(p->prior,
        p->piscina, plana, divisio))
    {
        p->memoria_defecit = VERUM;
    }
    si (divisio < numerus && !materia_token_trivia_ante_ponere(sequens,
        p->piscina, plana + divisio, numerus - divisio))
    {
        p->memoria_defecit = VERUM;
    }
    xar_vacare(p->pendentia);
    p->prior = sequens;
}


/* ==================================================
 * Classes lexematum
 * ================================================== */

interior b32
_segmentum (
    s32 g)
{
    redde g == (s32)TOML_LEX_CLAVIS_NUDA
        || g == (s32)TOML_LEX_CLAVIS_GEMINA
        || g == (s32)TOML_LEX_CLAVIS_SIMPLEX;
}

/* genus nodi valoris scalaris, aut -I */
interior s32
_scalaris (
    s32 g)
{
    commutatio (g)
    {
        casus TOML_LEX_CHORDA_GEMINA:
        casus TOML_LEX_CHORDA_GEMINA_MULTA:
        casus TOML_LEX_CHORDA_SIMPLEX:
        casus TOML_LEX_CHORDA_SIMPLEX_MULTA:
            redde (s32)TOML_GENUS_CHORDA;
        casus TOML_LEX_NUMERUS:
            redde (s32)TOML_GENUS_NUMERUS;
        casus TOML_LEX_TEMPUS:
            redde (s32)TOML_GENUS_TEMPUS;
        casus TOML_LEX_VERUM:
        casus TOML_LEX_FALSUM:
            redde (s32)TOML_GENUS_BOOLEAN;
        ordinarius:
            frange;
    }
    redde (s32)-I;
}

/* chorda (valor aut clavis) sine claudente: fines per lectorem
 * inventi, hic iudicati */
interior b32
_chorda_aperta (
    constans MateriaToken* t)
{
    constans i8* v = t->valor.datum;
            i32  n = t->valor.mensura;
            s32  retro;
            i32  k;

    commutatio (t->genus)
    {
        casus TOML_LEX_CHORDA_GEMINA:
        casus TOML_LEX_CLAVIS_GEMINA:
            si (n < II || v[n - I] != '"')
            {
                redde VERUM;
            }
            /* '\"' finale effugium est si retroversi impares */
            retro = ZEPHYRUM;
            per (k = n - II; k > ZEPHYRUM && v[k] == '\\'; k--)
            {
                retro++;
            }
            redde (retro % II) == I;
        casus TOML_LEX_CHORDA_SIMPLEX:
        casus TOML_LEX_CLAVIS_SIMPLEX:
            redde n < II || v[n - I] != '\'';
        casus TOML_LEX_CHORDA_GEMINA_MULTA:
            redde n < VI || v[n - I] != '"' || v[n - II] != '"'
                || v[n - III] != '"';
        casus TOML_LEX_CHORDA_SIMPLEX_MULTA:
            redde n < VI || v[n - I] != '\'' || v[n - II] != '\''
                || v[n - III] != '\'';
        ordinarius:
            frange;
    }
    redde FALSUM;
}

interior vacuum
_clausura_absens (
    Aedificatio* p)
{
    si (p->relatio != NIHIL)
    {
        p->relatio->clausurae_absentes++;
    }
}

interior vacuum
_absentia (
    Aedificatio* p,
            i32  numerus)
{
    si (p->relatio != NIHIL)
    {
        p->relatio->absentiae += numerus;
    }
}


/* ==================================================
 * Traditio: gradus summus clauditur, nodus parenti
 * ================================================== */

interior vacuum
_tradere (
     Aedificatio* p,
    MateriaNodus* filius)
{
    GradusAedificationis* g = _vertex(p);

    si (g == NIHIL)
    {
        redde;
    }
    commutatio (g->genus)
    {
        casus GRADUS_DOCUMENTUM:
            _appendere_nodum(p, g->nodus, (i32)TOML_DOCUMENTUM_LIBERI,
                filius);
            frange;
        casus GRADUS_CAPUT:
            _ponere_nodum(p, g->nodus, (i32)TOML_CAPUT_CLAVIS, filius);
            g->status = I;
            frange;
        casus GRADUS_PAR:
            si (g->status == ZEPHYRUM)
            {
                _ponere_nodum(p, g->nodus, (i32)TOML_PAR_CLAVIS,
                    filius);
                g->status = I;
            }
            alioquin
            {
                _ponere_nodum(p, g->nodus, (i32)TOML_PAR_VALOR, filius);
                g->status = III;
            }
            frange;
        casus GRADUS_SERIES:
        casus GRADUS_COMPACTA:
            _appendere_nodum(p, g->nodus, (i32)TOML_INCLUSA_LIBERI,
                filius);
            g->status = I;
            frange;
        ordinarius:
            frange;
    }
}

/* gradum summum removere; numerus uncorum sequitur (via UNA qua
 * acervus decrescit) */
interior vacuum
_depellere (
    Aedificatio* p)
{
    GradusAedificationis* g = _vertex(p);

    si (   g != NIHIL
        && (g->genus == GRADUS_SERIES || g->genus == GRADUS_COMPACTA)
        && p->unci > ZEPHYRUM)
    {
        p->unci--;
    }
    xar_removere_ultimum(p->gradus);
}

/* gradum summum claudere et nodum eius tradere; absentiae ad finem
 * (EOF aut abruptio) numerantur */
interior vacuum
_claudere (
    Aedificatio* p)
{
    GradusAedificationis g = *_vertex(p);

    _depellere(p);
    commutatio (g.genus)
    {
        casus GRADUS_CAPUT:
            si (g.status == ZEPHYRUM)
            {
                _absentia(p, I);
                _clausura_absens(p);
            }
            alioquin si (g.status == I)
            {
                _clausura_absens(p);
            }
            frange;
        casus GRADUS_PAR:
            si (g.status == I)
            {
                _absentia(p, II);
            }
            alioquin si (g.status == II)
            {
                _absentia(p, I);
            }
            frange;
        casus GRADUS_SERIES:
        casus GRADUS_COMPACTA:
            _clausura_absens(p);
            frange;
        ordinarius:
            frange;
    }
    _tradere(p, g.nodus);
}

/* malum novum in gradu summo; lexema primum recipit */
interior vacuum
_malum (
     Aedificatio* p,
    MateriaToken* t,
             i32  modus)
{
    MateriaNodus* m = _nodus(p, TOML_GENUS_MALUM);

    _appendere_token(p, m, (i32)TOML_MALUM_TOKENS, t);
    si (p->relatio != NIHIL)
    {
        p->relatio->mala++;
    }
    _impellere(p, GRADUS_MALUM, m, modus);
}

/* valor in gradu summo (PAR status II aut SERIES): VERUM si acceptus */
interior b32
_valorem_incipere (
     Aedificatio* p,
    MateriaToken* t)
{
    s32 g = _scalaris(t->genus);

    si (g >= ZEPHYRUM)
    {
        MateriaNodus* n = _nodus(p, (TomlGenus)g);

        _ponere_token(p, n, (i32)TOML_LEXEMA_TOK, t);
        si (_chorda_aperta(t))
        {
            _clausura_absens(p);
        }
        _tradere(p, n);
        redde VERUM;
    }
    si (t->genus == (s32)TOML_LEX_SERIES_APERTURA)
    {
        MateriaNodus* n = _nodus(p, TOML_GENUS_SERIES);

        _ponere_token(p, n, (i32)TOML_INCLUSA_TOK_APERTURA, t);
        _impellere(p, GRADUS_SERIES, n, ZEPHYRUM);
        redde VERUM;
    }
    si (t->genus == (s32)TOML_LEX_COMPACTA_APERTURA)
    {
        MateriaNodus* n = _nodus(p, TOML_GENUS_TABULA_COMPACTA);

        _ponere_token(p, n, (i32)TOML_INCLUSA_TOK_APERTURA, t);
        _impellere(p, GRADUS_COMPACTA, n, ZEPHYRUM);
        redde VERUM;
    }
    redde FALSUM;
}

/* par novum cum clave nova (lexema segmentum retractandum) */
interior vacuum
_par_incipere (
    Aedificatio* p)
{
    _impellere(p, GRADUS_PAR, _nodus(p, TOML_GENUS_PAR), ZEPHYRUM);
    _impellere(p, GRADUS_CLAVIS, _nodus(p, TOML_GENUS_CLAVIS),
        ZEPHYRUM);
}

/* in uncis? (malum usque ad ',' aut claudentem) */
interior b32
_in_uncis (
    Aedificatio* p)
{
    i32 n = xar_numerus(p->gradus);
    i32 k;

    per (k = n - I; k >= ZEPHYRUM && k < n; k--)
    {
        GradusAedificationis* g =
            (GradusAedificationis*)xar_obtinere(p->gradus, k);

        si (g->genus == GRADUS_SERIES || g->genus == GRADUS_COMPACTA)
        {
            redde VERUM;
        }
        si (g->genus == GRADUS_DOCUMENTUM)
        {
            redde FALSUM;
        }
        si (k == ZEPHYRUM)
        {
            frange;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Tractatio: VERUM = lexema acceptum; FALSUM = retractandum (gradus
 * summus clausus est)
 * ================================================== */

interior b32
_tractare (
     Aedificatio* p,
    MateriaToken* t)
{
     GradusAedificationis* g          = _vertex(p);
                      s32  genus_lex  = t->genus;

    commutatio (g->genus)
    {
        casus GRADUS_DOCUMENTUM:
            si (_segmentum(genus_lex))
            {
                _par_incipere(p);
                redde FALSUM;
            }
            si (   genus_lex == (s32)TOML_LEX_TABULA_APERTURA
                || genus_lex == (s32)TOML_LEX_SERIES_TABULARUM_APERTURA)
            {
                MateriaNodus* n = _nodus(p,
                    genus_lex == (s32)TOML_LEX_TABULA_APERTURA
                        ? TOML_GENUS_CAPUT_TABULAE
                        : TOML_GENUS_CAPUT_SERIEI);

                _ponere_token(p, n, (i32)TOML_CAPUT_TOK_APERTURA, t);
                _impellere(p, GRADUS_CAPUT, n, ZEPHYRUM);
                redde VERUM;
            }
            si (genus_lex == (s32)TOML_LEX_LINEA_FINIS)
            {
                MateriaNodus* n = _nodus(p, TOML_GENUS_LINEA);

                _ponere_token(p, n, (i32)TOML_LEXEMA_TOK, t);
                _appendere_nodum(p, g->nodus,
                    (i32)TOML_DOCUMENTUM_LIBERI,
                    n);
                redde VERUM;
            }
            _malum(p, t, ZEPHYRUM);
            redde VERUM;

        casus GRADUS_CLAVIS:
            si (g->status == ZEPHYRUM && _segmentum(genus_lex))
            {
                _appendere_token(p, g->nodus, (i32)TOML_CLAVIS_PARTES,
                    t);
                si (_chorda_aperta(t))
                {
                    _clausura_absens(p);
                }
                g->status = I;
                redde VERUM;
            }
            si (g->status == I && genus_lex == (s32)TOML_LEX_PUNCTUM)
            {
                _appendere_token(p, g->nodus, (i32)TOML_CLAVIS_PARTES,
                    t);
                g->status = ZEPHYRUM;
                redde VERUM;
            }
            _claudere(p);
            redde FALSUM;

        casus GRADUS_CAPUT:
            si (g->status == ZEPHYRUM && _segmentum(genus_lex))
            {
                _impellere(p, GRADUS_CLAVIS, _nodus(p,
                    TOML_GENUS_CLAVIS),
                    ZEPHYRUM);
                redde FALSUM;
            }
            si (g->status == ZEPHYRUM)
            {
                _absentia(p, I);
                g->status = I;
            }
            si (g->status == I)
            {
                s32 claudens = g->nodus->genus
                        == (s32)TOML_GENUS_CAPUT_TABULAE
                    ? (s32)TOML_LEX_TABULA_CLAUSURA
                    : (s32)TOML_LEX_SERIES_TABULARUM_CLAUSURA;

                si (genus_lex == claudens)
                {
                    _ponere_token(p, g->nodus,
                        (i32)TOML_CAPUT_TOK_CLAUSURA, t);
                    g->status = II;
                    redde VERUM;
                }
            }
            /* finis lineae aut aliud: caput clauditur */
            _claudere(p);
            si (genus_lex == (s32)TOML_LEX_LINEA_FINIS)
            {
                redde FALSUM;
            }
            _malum(p, t, ZEPHYRUM);
            redde VERUM;

        casus GRADUS_PAR:
            si (g->status == I && genus_lex == (s32)TOML_LEX_SIGNUM)
            {
                _ponere_token(p, g->nodus, (i32)TOML_PAR_TOK_SIGNUM, t);
                g->status = II;
                redde VERUM;
            }
            si (g->status == II)
            {
                si (_valorem_incipere(p, t))
                {
                    redde VERUM;
                }
                si (   genus_lex == (s32)TOML_LEX_LINEA_FINIS
                    || genus_lex == (s32)TOML_LEX_COMMA
                    || genus_lex == (s32)TOML_LEX_COMPACTA_CLAUSURA)
                {
                    _claudere(p);
                    redde FALSUM;
                }
                /* valor pravus: malum ut valor */
                _malum(p, t, I);
                redde VERUM;
            }
            /* status I (sine '=') aut III (post valorem) */
            _claudere(p);
            si (   genus_lex == (s32)TOML_LEX_LINEA_FINIS
                || (_in_uncis(p) && (genus_lex == (s32)TOML_LEX_COMMA
                    || genus_lex == (s32)TOML_LEX_COMPACTA_CLAUSURA)))
            {
                redde FALSUM;
            }
            _malum(p, t, ZEPHYRUM);
            redde VERUM;

        casus GRADUS_SERIES:
            si (genus_lex == (s32)TOML_LEX_SERIES_CLAUSURA)
            {
                _ponere_token(p, g->nodus,
                    (i32)TOML_INCLUSA_TOK_CLAUSURA,
                    t);
                {
                    GradusAedificationis s = *g;

                    _depellere(p);
                    _tradere(p, s.nodus);
                }
                redde VERUM;
            }
            si (g->status == I && genus_lex == (s32)TOML_LEX_COMMA)
            {
                MateriaNodus* n = _nodus(p, TOML_GENUS_COMMA);

                _ponere_token(p, n, (i32)TOML_LEXEMA_TOK, t);
                _appendere_nodum(p, g->nodus, (i32)TOML_INCLUSA_LIBERI,
                    n);
                g->status = ZEPHYRUM;
                redde VERUM;
            }
            si (g->status == ZEPHYRUM && _valorem_incipere(p, t))
            {
                redde VERUM;
            }
            _malum(p, t, I);
            redde VERUM;

        casus GRADUS_COMPACTA:
            si (genus_lex == (s32)TOML_LEX_COMPACTA_CLAUSURA)
            {
                _ponere_token(p, g->nodus,
                    (i32)TOML_INCLUSA_TOK_CLAUSURA,
                    t);
                {
                    GradusAedificationis s = *g;

                    _depellere(p);
                    _tradere(p, s.nodus);
                }
                redde VERUM;
            }
            si (g->status == I && genus_lex == (s32)TOML_LEX_COMMA)
            {
                MateriaNodus* n = _nodus(p, TOML_GENUS_COMMA);

                _ponere_token(p, n, (i32)TOML_LEXEMA_TOK, t);
                _appendere_nodum(p, g->nodus, (i32)TOML_INCLUSA_LIBERI,
                    n);
                g->status = ZEPHYRUM;
                redde VERUM;
            }
            si (g->status == ZEPHYRUM && _segmentum(genus_lex))
            {
                _par_incipere(p);
                redde FALSUM;
            }
            _malum(p, t, ZEPHYRUM);
            redde VERUM;

        casus GRADUS_MALUM:
            si (_in_uncis(p))
            {
                si (   genus_lex == (s32)TOML_LEX_COMMA
                    || genus_lex == (s32)TOML_LEX_SERIES_CLAUSURA
                    || genus_lex == (s32)TOML_LEX_COMPACTA_CLAUSURA)
                {
                    _claudere(p);
                    redde FALSUM;
                }
            }
            alioquin si (genus_lex == (s32)TOML_LEX_LINEA_FINIS)
            {
                _claudere(p);
                redde FALSUM;
            }
            _appendere_token(p, g->nodus, (i32)TOML_MALUM_TOKENS, t);
            redde VERUM;

        ordinarius:
            frange;
    }
    redde VERUM;
}


/* ==================================================
 * Modus et status lectoris ex acervo
 * ================================================== */

interior TomlModus
_modus (
    constans GradusAedificationis* g)
{
    commutatio (g->genus)
    {
        casus GRADUS_PAR:
            redde g->status
                == II ? TOML_MODUS_VALOR : TOML_MODUS_CLAVIS;
        casus GRADUS_SERIES:
            redde TOML_MODUS_VALOR;
        casus GRADUS_MALUM:
            redde g->status == I ? TOML_MODUS_VALOR : TOML_MODUS_CLAVIS;
        ordinarius:
            frange;
    }
    redde TOML_MODUS_CLAVIS;
}

interior i32
_unci_aperti (
    Aedificatio* p)
{
    redde p->unci;
}


/* ==================================================
 * Publica
 * ================================================== */

MateriaNodus*
toml_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   s32  mensura,
           TomlParsura* relatio)
{
            Aedificatio p;
     MateriaLexIudicium iudicium;
          MateriaNodus*  documentum;
            TomlParsura  vacua;

    si (relatio == NIHIL)
    {
        relatio = &vacua;
    }
    relatio->mala                = ZEPHYRUM;
    relatio->clausurae_absentes  = ZEPHYRUM;
    relatio->absentiae           = ZEPHYRUM;
    relatio->profunditas_maxima  = ZEPHYRUM;
    relatio->sana                = FALSUM;

    p.piscina          = piscina;
    p.relatio          = relatio;
    p.prior            = NIHIL;
    p.post_lineam      = VERUM;
    p.memoria_defecit  = FALSUM;
    p.unci             = ZEPHYRUM;
    p.gradus = xar_creare(piscina,
        (i32)magnitudo(GradusAedificationis));
    p.pendentia = xar_creare(piscina,
        (i32)magnitudo(MateriaToken*));
    si (   p.gradus == NIHIL || p.pendentia == NIHIL
        || !toml_lector_incipere(&p.lector, piscina, fons, mensura)
        || !materia_lexicon_ratum_facere(&p.lexicon, &TOML_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }
    documentum = _nodus(&p, TOML_GENUS_DOCUMENTUM);
    _impellere(&p, GRADUS_DOCUMENTUM, documentum, ZEPHYRUM);

    dum (!p.memoria_defecit)
    {
        GradusAedificationis* g = _vertex(&p);
        MateriaToken* t;

        toml_lector_profunditatem_ponere(&p.lector, _unci_aperti(&p));
        toml_lector_sententiam_ponere(&p.lector,
            g->genus != GRADUS_DOCUMENTUM);
        t = toml_lector_proximum(&p.lector, _modus(g));
        si (t == NIHIL)
        {
            redde NIHIL;
        }
        si (materia_lexicon_trivium_est(&p.lexicon, t->genus))
        {
            _cumulare(&p, t);
            perge;
        }
        _solvere(&p, t);
        si (t->genus == (s32)TOML_LEX_FINIS)
        {
            dum (xar_numerus(p.gradus) > I)
            {
                _claudere(&p);
            }
            _ponere_token(&p, documentum, (i32)TOML_DOCUMENTUM_CAUDA,
                t);
            frange;
        }
        /* retractatio: gradus claudit et lexema infra reddit; semper
         * finit quia documentum omne lexema accipit */
        dum (!_tractare(&p, t) && !p.memoria_defecit)
        {
        }
    }
    si (p.memoria_defecit)
    {
        redde NIHIL;
    }
    relatio->sana = relatio->mala == ZEPHYRUM
        && relatio->clausurae_absentes == ZEPHYRUM
        && relatio->absentiae == ZEPHYRUM;
    redde documentum;
}
