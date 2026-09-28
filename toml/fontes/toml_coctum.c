/* toml_coctum.c - Coctio documenti toml (vide toml_coctum.h)
 *
 * Ambulatio una super liberos documenti cum tabula CURRENTI (capite
 * mutata). Caput quod recusatur tabulam ORPHANAM dat (non insertam):
 * paria sectionis eius tamen coquuntur et inter se iudicantur, sed
 * errata consequentia non gignunt.
 *
 * Profunditas: valores inclusi (series, tabulae inlineae) per
 * recursionem coquuntur - profunditas C acervi = profunditas uncorum
 * fontis. Aedificator iterativus est; hic nondum (Q11 totalitas).
 */

#include "toml_coctum.h"
#include "toml_scalaris.h"
#include "toml_registrum.h"
#include "toml_lexicon.h"
#include "tabula_dispersa.h"
#include "materia_diagnostica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
                   chorda  textus;     /* decoditum */
    constans MateriaToken* lexema;
} Segmentum;

nomen structura {
      Piscina* piscina;
          Xar* diagnostica;   /* structurae */
          s32  sectio;
    TomlValor* radix;
          Xar* opera;         /* Opus: continentes implendi */
} Coctor;

hic_manens constans character* NOMINA_GENERUM[] = {
    "tabula", "series", "chorda", "integer", "fluitans", "boolean",
    "tempus"
};


/* ==================================================
 * Valores et tabulae
 * ================================================== */

interior TomlValor*
_valor_novus (
                   Coctor* c,
         TomlGenusValoris  genus,
    constans MateriaNodus* nodus,
    constans MateriaNodus* definitio)
{
    TomlValor* v = (TomlValor*)piscina_allocare(c->piscina,
                       (i64)magnitudo(TomlValor));

    si (v == NIHIL)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(TomlValor));
    v->genus      = genus;
    v->nodus      = nodus;
    v->definitio  = definitio;
    redde v;
}

interior TomlValor*
_tabula_nova (
                   Coctor* c,
         TomlModusTabulae  modus,
    constans MateriaNodus* nodus,
    constans MateriaNodus* definitio)
{
    TomlValor* v = _valor_novus(c, TOML_VALOR_TABULA, nodus, definitio);

    si (v == NIHIL)
    {
        redde NIHIL;
    }
    v->datum.tabula.claves  = xar_creare(c->piscina,
        (i32)magnitudo(chorda));
    v->datum.tabula.valores = xar_creare(c->piscina,
        (i32)magnitudo(TomlValor*));
    v->datum.tabula.index   = tabula_dispersa_creare_chorda(c->piscina,
        VIII);
    v->datum.tabula.modus   = modus;
    v->datum.tabula.sectio  = c->sectio;
    si (   v->datum.tabula.claves  == NIHIL
        || v->datum.tabula.valores == NIHIL
        || v->datum.tabula.index   == NIHIL)
    {
        redde NIHIL;
    }
    redde v;
}

interior TomlValor*
_series_nova (
                   Coctor* c,
    constans MateriaNodus* nodus,
    constans MateriaNodus* definitio)
{
    TomlValor* v = _valor_novus(c, TOML_VALOR_SERIES, nodus, definitio);

    si (v == NIHIL)
    {
        redde NIHIL;
    }
    v->datum.series = xar_creare(c->piscina,
        (i32)magnitudo(TomlValor*));
    redde v->datum.series != NIHIL ? v : NIHIL;
}

interior b32
_series_appendere (
    TomlValor* series,
    TomlValor* elementum)
{
    TomlValor** locus = (TomlValor**)xar_addere(series->datum.series);

    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    *locus = elementum;
    redde VERUM;
}

TomlValor*
toml_tabulae_filius (
     constans TomlValor* tabula,
                 chorda  clavis)
{
    vacuum* inventum = NIHIL;
       i32  k;

    si (tabula == NIHIL || tabula->genus != TOML_VALOR_TABULA)
    {
        redde NIHIL;
    }
    /* clavis vacua in indicem non intrat - vide _inserere */
    si (clavis.mensura == ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < xar_numerus(tabula->datum.tabula.claves);
             k++)
        {
            si (((chorda*)xar_obtinere(tabula->datum.tabula.claves,
                     k))->mensura == ZEPHYRUM)
            {
                redde *(TomlValor**)xar_obtinere(
                    tabula->datum.tabula.valores, k);
            }
        }
        redde NIHIL;
    }
    si (tabula_dispersa_invenire(tabula->datum.tabula.index, clavis,
            &inventum))
    {
        redde (TomlValor*)inventum;
    }
    redde NIHIL;
}

interior TomlValor*
_invenire (
     TomlValor* tabula,
        chorda  clavis)
{
    redde toml_tabulae_filius(tabula, clavis);
}

interior b32
_inserere (
     TomlValor* tabula,
        chorda  clavis,
     TomlValor* valor)
{
       chorda*  c = (chorda*)xar_addere(tabula->datum.tabula.claves);
    TomlValor** v =
        (TomlValor**)xar_addere(tabula->datum.tabula.valores);

    si (c == NIHIL || v == NIHIL)
    {
        redde FALSUM;
    }
    *c = clavis;
    *v = valor;
    si (clavis.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* vide _invenire */
    }
    redde tabula_dispersa_inserere(tabula->datum.tabula.index, clavis,
        valor);
}

/* elementum ultimum seriei tabularum */
interior TomlValor*
_ultimum (
    TomlValor* series)
{
    i32 n = xar_numerus(series->datum.series);

    redde n > ZEPHYRUM
        ? *(TomlValor**)xar_obtinere(series->datum.series, n
            - I) : NIHIL;
}


/* ==================================================
 * Diagnostica
 * ================================================== */

/* via clavium 'a.b.c' segmentorum [0, usque] */
interior constans character*
_via (
                Coctor* c,
    constans Segmentum* seg,
                   i32  usque)
{
          i32  k;
          i32  m = ZEPHYRUM;
    character* b;

    per (k = ZEPHYRUM; k <= usque; k++)
    {
        m += seg[k].textus.mensura + I;
    }
    b = (character*)piscina_allocare(c->piscina, (i64)(m + I));
    si (b == NIHIL)
    {
        redde "?";
    }
    m = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= usque; k++)
    {
        si (k > ZEPHYRUM)
        {
            b[m++] = '.';
        }
        memcpy(b + m, seg[k].textus.datum,
            (size_t)seg[k].textus.mensura);
        m += seg[k].textus.mensura;
    }
    b[m] = '\0';
    redde b;
}

/* causa: forma cum argumentis (via ad CXXVIII octetos abscisa) */
interior constans character*
_causa (
                Coctor* c,
    constans character* forma,
    constans character* primum,
    constans character* secundum)
{
    character* b = (character*)piscina_allocare(c->piscina, (i64)CCCLX);

    si (b == NIHIL)
    {
        redde forma;
    }
    sprintf(b, forma, primum, secundum != NIHIL ? secundum : "");
    redde b;
}

interior vacuum
_culpa (
                   Coctor* c,
    constans MateriaToken* lexema,
    constans MateriaNodus* nodus,
    constans MateriaNodus* relatum,
       constans character* codex,
       constans character* causa)
{
    MateriaDiagnosticum* d;

    d = (MateriaDiagnosticum*)xar_addere(c->diagnostica);
    si (d == NIHIL)
    {
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(MateriaDiagnosticum));
    d->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex     = codex;
    d->causa     = causa;
    d->nodus     = nodus;
    materia_tractus_lexematis(NIHIL, lexema, &d->tractus);
    si (relatum != NIHIL)
    {
        MateriaSedesRelata* r = (MateriaSedesRelata*)piscina_allocare(
                                    c->piscina,
                                    (i64)magnitudo(MateriaSedesRelata));

        si (   r != NIHIL
            && materia_tractus_nodi(NIHIL, relatum, &r->tractus))
        {
            r->nota               = "prima definitio";
            d->relata             = r;
            d->numerus_relatorum  = I;
        }
    }
}


/* ==================================================
 * Claves
 * ================================================== */

/* segmenta clavis (decodita, sine punctis) in Xar NOVUM; NIHIL =
 * clavis absens aut segmentum vacuum (nominatum) */
interior Xar*
_clavem_legere (
                   Coctor* c,
    constans MateriaValor* locus)
{
    constans MateriaNodus* clavis;
             MateriaValor  partes;
                      Xar* segmenta;
                      i32  k;
                      i32  n;
    constans MateriaToken* ultimum = NIHIL;

    si (   locus->genus       != MATERIA_VALOR_NODUS
        || locus->datum.nodus == NIHIL)
    {
        redde NIHIL;
    }
    clavis = locus->datum.nodus;
    partes = clavis->loci[TOML_CLAVIS_PARTES];
    si (partes.genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    segmenta  = xar_creare(c->piscina, (i32)magnitudo(Segmentum));
    n         = materia_valor_lista_numerus(partes);
    per (k = ZEPHYRUM; segmenta != NIHIL && k < n; k++)
    {
        MateriaValor* e = materia_valor_lista_obtinere(partes, k);
        MateriaToken* t;

        si (e->genus != MATERIA_VALOR_TOKEN)
        {
            perge;
        }
        t        = e->datum.token;
        ultimum  = t;
        si (t->genus != (s32)TOML_LEX_PUNCTUM)
        {
            Segmentum* s = (Segmentum*)xar_addere(segmenta);

            si (s == NIHIL)
            {
                redde NIHIL;
            }
            s->lexema = t;
            toml_chordam_coquere(c->piscina, t, &s->textus, NIHIL);
        }
    }
    si (ultimum != NIHIL && ultimum->genus == (s32)TOML_LEX_PUNCTUM)
    {
        _culpa(c, ultimum, clavis, NIHIL, TOML_CODEX_CLAVIS_VACUA,
            "segmentum clavis post '.' deest");
        redde NIHIL;
    }
    si (segmenta == NIHIL || xar_numerus(segmenta) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde segmenta;
}

interior constans character*
_genus_nomen (
    constans TomlValor* v)
{
    si (v->genus == TOML_VALOR_SERIES && v->series_tabularum)
    {
        redde "series tabularum";
    }
    si (   v->genus              == TOML_VALOR_TABULA
        && v->datum.tabula.modus == TOML_TABULA_INLINEA)
    {
        redde "tabula inlinea";
    }
    redde NOMINA_GENERUM[v->genus];
}


/* ==================================================
 * Valores - ITERATIVE (Q11: recursio per profunditatem uncorum C
 * acervum consumebat; C milia '[' SIGSEGV)
 *
 * Continens (series, tabula inlinea) VACUUM nascitur et statim in
 * parentem inseritur; OPUS eius in acervum pellitur et liberi deinde
 * ordine fontis coquuntur - continens interior opus suum supra pellit,
 * ergo ordo profundus-primus sine recursione C. Ordo insertionis non
 * mutatur: clavis parentis ante liberos eius, ut antea.
 * ================================================== */

interior vacuum
_par (
                   Coctor* c,
                TomlValor* tabula,
    constans MateriaNodus* par);

nomen structura {
    constans MateriaNodus* nodus;       /* series aut tabula-compacta */
                TomlValor* valor;       /* continens implendum */
    constans MateriaNodus* definitio;
                      i32  index;       /* liber proximus */
    constans MateriaNodus* ultimus;     /* liber nodalis ultimus */
} Opus;

/* valorem incipere: scalare TOTUM; continens vacuum + opus pulsum */
interior TomlValor*
_valorem_incipere (
                   Coctor* c,
    constans MateriaNodus* nodus,
    constans MateriaNodus* definitio)
{
    TomlValor* v;
         Opus* o;

    commutatio (nodus->genus)
    {
        casus TOML_GENUS_CHORDA:
        casus TOML_GENUS_NUMERUS:
        casus TOML_GENUS_BOOLEAN:
        casus TOML_GENUS_TEMPUS:
            v = _valor_novus(c, TOML_VALOR_CHORDA, nodus, definitio);
            si (v == NIHIL)
            {
                redde NIHIL;
            }
            /* vitia scalaria toml_scalaria_iudicare nuntiat */
            si (!toml_scalarem_coquere(c->piscina, nodus, v, NIHIL))
            {
                v->genus = nodus->genus == (s32)TOML_GENUS_NUMERUS
                    ? (v->genus == TOML_VALOR_FLUITANS
                        ? TOML_VALOR_FLUITANS : TOML_VALOR_INTEGER)
                    : nodus->genus == (s32)TOML_GENUS_TEMPUS
                    ? TOML_VALOR_TEMPUS
                    : nodus->genus == (s32)TOML_GENUS_BOOLEAN
                    ? TOML_VALOR_BOOLEAN : TOML_VALOR_CHORDA;
            }
            v->definitio = definitio;
            redde v;
        casus TOML_GENUS_SERIES:
            v = _series_nova(c, nodus, definitio);
            frange;
        casus TOML_GENUS_TABULA_COMPACTA:
            v = _tabula_nova(c, TOML_TABULA_INLINEA, nodus, definitio);
            frange;
        ordinarius:
            redde NIHIL;
    }
    si (v == NIHIL)
    {
        redde NIHIL;
    }
    o = (Opus*)xar_addere(c->opera);
    si (o == NIHIL)
    {
        redde NIHIL;
    }
    o->nodus      = nodus;
    o->valor      = v;
    o->definitio  = definitio;
    o->index      = ZEPHYRUM;
    o->ultimus    = NIHIL;
    redde v;
}

/* opera omnia implere (acervus vacuus in fine) */
interior vacuum
_opera_exhaurire (
    Coctor* c)
{
    dum (xar_numerus(c->opera) > ZEPHYRUM)
    {
                 i32  summum  = xar_numerus(c->opera) - I;
                Opus* o       = (Opus*)xar_obtinere(c->opera, summum);
                Opus  copia;
        MateriaValor  l = o->nodus->loci[TOML_INCLUSA_LIBERI];
        MateriaValor* e;

        si (   l.genus  != MATERIA_VALOR_LISTA
            || o->index >= materia_valor_lista_numerus(l))
        {
            copia = *o;
            xar_removere_ultimum(c->opera);
            si (   copia.nodus->genus == (s32)TOML_GENUS_TABULA_COMPACTA
                && copia.ultimus != NIHIL
                && copia.ultimus->genus == (s32)TOML_GENUS_COMMA
                && copia.ultimus->loci[TOML_LEXEMA_TOK].genus
                    == MATERIA_VALOR_TOKEN)
            {
                _culpa(c,
                    copia.ultimus->loci[TOML_LEXEMA_TOK].datum.token,
                    copia.nodus, NIHIL, TOML_CODEX_VERSIO_NOVIOR,
                    "comma caudale in tabula inlinea: TOML 1.1, "
                    "non 1.0");
            }
            perge;
        }
        e = materia_valor_lista_obtinere(l, o->index);
        o->index++;
        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        /* o post vocationem invalidum esse potest (acervus crescit):
         * copia ante vocationem */
        copia = *o;
        si (copia.nodus->genus == (s32)TOML_GENUS_SERIES)
        {
            TomlValor* elementum;

            si (   e->datum.nodus->genus == (s32)TOML_GENUS_COMMA
                || e->datum.nodus->genus == (s32)TOML_GENUS_MALUM)
            {
                perge;
            }
            elementum = _valorem_incipere(c, e->datum.nodus,
                copia.definitio);
            si (elementum != NIHIL)
            {
                _series_appendere(copia.valor, elementum);
            }
        }
        alioquin
        {
            o->ultimus = e->datum.nodus;
            si (e->datum.nodus->genus == (s32)TOML_GENUS_PAR)
            {
                _par(c, copia.valor, e->datum.nodus);
            }
        }
    }
}


/* ==================================================
 * Paria et capita
 * ================================================== */

interior vacuum
_par (
                   Coctor* c,
                TomlValor* tabula,
    constans MateriaNodus* par)
{
                      Xar* segmenta;
                Segmentum* seg;
                      i32  n;
                      i32  k;
                TomlValor* currens = tabula;
                TomlValor* v;
    constans MateriaNodus* clavis;

    si (par->loci[TOML_PAR_VALOR].genus != MATERIA_VALOR_NODUS)
    {
        redde;
    }
    segmenta = _clavem_legere(c, &par->loci[TOML_PAR_CLAVIS]);
    si (segmenta == NIHIL)
    {
        redde;
    }
    clavis  = par->loci[TOML_PAR_CLAVIS].datum.nodus;
    seg     = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
    n       = xar_numerus(segmenta);

    /* continentes per claves punctatas */
    per (k = ZEPHYRUM; k + I < n; k++)
    {
        Segmentum* s       = (Segmentum*)xar_obtinere(segmenta, k);
        TomlValor* filius  = _invenire(currens, s->textus);

        seg = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
        si (filius == NIHIL)
        {
            filius = _tabula_nova(c, TOML_TABULA_PUNCTATA, par, clavis);
            si (   filius == NIHIL
                || !_inserere(currens, s->textus, filius))
            {
                redde;
            }
        }
        alioquin si (filius->genus == TOML_VALOR_TABULA)
        {
            TomlModusTabulae modus = filius->datum.tabula.modus;

            si (modus == TOML_TABULA_INLINEA)
            {
                _culpa(c, s->lexema, par, filius->definitio,
                    TOML_CODEX_CLAUSA, _causa(c,
                    "tabula inlinea '%s' clausa est: post scriptionem "
                    "nihil ei addi potest%s", _via(c, seg, k), NIHIL));
                redde;
            }
            si (   modus == TOML_TABULA_EXPLICITA
                || (modus == TOML_TABULA_PUNCTATA
                    && filius->datum.tabula.sectio != c->sectio))
            {
                _culpa(c, s->lexema, par, filius->definitio,
                    TOML_CODEX_EXTRA_SECTIONEM, _causa(c,
                    "tabula '%s' %s: claves punctatae eam extra "
                    "sectionem suam non extendunt", _via(c, seg, k),
                    modus == TOML_TABULA_EXPLICITA
                    ? "per caput definita est"
                    : "in sectione priore per claves punctatas "
                      "definita est"));
                redde;
            }
            si (modus == TOML_TABULA_IMPLICITA)
            {
                filius->datum.tabula.modus   = TOML_TABULA_PUNCTATA;
                filius->datum.tabula.sectio  = c->sectio;
            }
        }
        alioquin si (filius->genus == TOML_VALOR_SERIES)
        {
            si (filius->series_tabularum)
            {
                _culpa(c, s->lexema, par, filius->definitio,
                    TOML_CODEX_EXTRA_SECTIONEM, _causa(c,
                    "'%s' series tabularum est ([[%s]]): claves "
                    "punctatae eam extra sectionem suam non extendunt",
                    _via(c, seg, k), _via(c, seg, k)));
            }
            alioquin
            {
                _culpa(c, s->lexema, par, filius->definitio,
                    TOML_CODEX_CLAUSA, _causa(c,
                    "series statica '%s' clausa est%s", _via(c, seg, k),
                    NIHIL));
            }
            redde;
        }
        alioquin
        {
            _culpa(c, s->lexema, par, filius->definitio,
                TOML_CODEX_GENUS_ALIENUM, _causa(c,
                "'%s' iam %s est, non tabula", _via(c, seg, k),
                _genus_nomen(filius)));
            redde;
        }
        currens = filius;
    }

    /* clavis ultima */
    {
        Segmentum* s      = (Segmentum*)xar_obtinere(segmenta, n - I);
        TomlValor* prior  = _invenire(currens, s->textus);

        seg = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
        si (prior != NIHIL)
        {
            _culpa(c, s->lexema, par, prior->definitio,
                TOML_CODEX_CLAVIS_ITERATA, _causa(c,
                "clavis '%s' iam definita (%s)", _via(c, seg, n - I),
                _genus_nomen(prior)));
            redde;
        }
        v = _valorem_incipere(c, par->loci[TOML_PAR_VALOR].datum.nodus,
            clavis);
        si (v != NIHIL)
        {
            _inserere(currens, s->textus, v);
        }
    }
}

/* [x] aut [[x]]: tabula nova currens, aut NIHIL (nominatum) */
interior TomlValor*
_caput (
                   Coctor* c,
    constans MateriaNodus* caput,
                      b32  seriei)
{
                      Xar* segmenta;
                Segmentum* seg;
                      i32  n;
                      i32  k;
                TomlValor* currens = c->radix;
                TomlValor* prior;
                Segmentum* s;
    constans MateriaNodus* clavis;

    segmenta = _clavem_legere(c, &caput->loci[TOML_CAPUT_CLAVIS]);
    si (segmenta == NIHIL)
    {
        redde NIHIL;
    }
    clavis  = caput->loci[TOML_CAPUT_CLAVIS].datum.nodus;
    seg     = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
    n       = xar_numerus(segmenta);

    per (k = ZEPHYRUM; k + I < n; k++)
    {
        TomlValor* filius;

        seg     = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
        s       = (Segmentum*)xar_obtinere(segmenta, k);
        filius  = _invenire(currens, s->textus);
        si (filius == NIHIL)
        {
            filius = _tabula_nova(c, TOML_TABULA_IMPLICITA, caput,
                clavis);
            si (   filius == NIHIL
                || !_inserere(currens, s->textus, filius))
            {
                redde NIHIL;
            }
        }
        alioquin si (filius->genus == TOML_VALOR_TABULA)
        {
            si (filius->datum.tabula.modus == TOML_TABULA_INLINEA)
            {
                _culpa(c, s->lexema, caput, filius->definitio,
                    TOML_CODEX_CLAUSA, _causa(c,
                    "tabula inlinea '%s' clausa est: post scriptionem "
                    "nihil ei addi potest%s", _via(c, seg, k), NIHIL));
                redde NIHIL;
            }
        }
        alioquin si (   filius->genus    == TOML_VALOR_SERIES
                     && filius->series_tabularum
                     && _ultimum(filius) != NIHIL)
        {
            filius = _ultimum(filius);
        }
        alioquin si (filius->genus == TOML_VALOR_SERIES)
        {
            _culpa(c, s->lexema, caput, filius->definitio,
                TOML_CODEX_CLAUSA, _causa(c,
                "series statica '%s' clausa est%s", _via(c, seg, k),
                NIHIL));
            redde NIHIL;
        }
        alioquin
        {
            _culpa(c, s->lexema, caput, filius->definitio,
                TOML_CODEX_GENUS_ALIENUM, _causa(c,
                "'%s' iam %s est, non tabula", _via(c, seg, k),
                _genus_nomen(filius)));
            redde NIHIL;
        }
        currens = filius;
    }

    seg    = (Segmentum*)xar_obtinere(segmenta, ZEPHYRUM);
    s      = (Segmentum*)xar_obtinere(segmenta, n - I);
    prior  = _invenire(currens, s->textus);

    si (!seriei)
    {
        TomlValor* t;

        si (prior == NIHIL)
        {
            t = _tabula_nova(c, TOML_TABULA_EXPLICITA, caput, clavis);
            si (t == NIHIL || !_inserere(currens, s->textus, t))
            {
                redde NIHIL;
            }
            redde t;
        }
        si (prior->genus == TOML_VALOR_TABULA)
        {
            commutatio (prior->datum.tabula.modus)
            {
                casus TOML_TABULA_IMPLICITA:
                    prior->datum.tabula.modus = TOML_TABULA_EXPLICITA;
                    prior->definitio          = clavis;
                    prior->nodus              = caput;
                    redde prior;
                casus TOML_TABULA_EXPLICITA:
                    _culpa(c, s->lexema, caput, prior->definitio,
                        TOML_CODEX_TABULA_ITERATA, _causa(c,
                        "tabula [%s] iam definita%s", _via(c, seg, n
                            - I),
                        NIHIL));
                    redde NIHIL;
                casus TOML_TABULA_PUNCTATA:
                    _culpa(c, s->lexema, caput, prior->definitio,
                        TOML_CODEX_TABULA_PUNCTATA, _causa(c,
                        "tabula [%s] iam per claves punctatas "
                        "definita%s",
                        _via(c, seg, n - I), NIHIL));
                    redde NIHIL;
                casus TOML_TABULA_INLINEA:
                ordinarius:
                    _culpa(c, s->lexema, caput, prior->definitio,
                        TOML_CODEX_CLAUSA, _causa(c,
                        "tabula inlinea '%s' clausa est: post "
                        "scriptionem "
                        "nihil ei addi potest%s", _via(c, seg, n - I),
                        NIHIL));
                    redde NIHIL;
            }
        }
        si (   prior->genus == TOML_VALOR_SERIES
            && prior->series_tabularum)
        {
            _culpa(c, s->lexema, caput, prior->definitio,
                TOML_CODEX_TABULA_ITERATA, _causa(c,
                "tabula [%s] iam definita ut series tabularum [[%s]]",
                _via(c, seg, n - I), _via(c, seg, n - I)));
            redde NIHIL;
        }
        si (prior->genus == TOML_VALOR_SERIES)
        {
            _culpa(c, s->lexema, caput, prior->definitio,
                TOML_CODEX_CLAUSA, _causa(c,
                "series statica '%s' clausa est%s", _via(c, seg, n - I),
                NIHIL));
            redde NIHIL;
        }
        _culpa(c, s->lexema, caput, prior->definitio,
            TOML_CODEX_GENUS_ALIENUM, _causa(c,
            "'%s' iam %s est, non tabula", _via(c, seg, n - I),
            _genus_nomen(prior)));
        redde NIHIL;
    }

    /* [[x]] */
    si (prior == NIHIL)
    {
        prior = _series_nova(c, caput, clavis);
        si (prior == NIHIL || !_inserere(currens, s->textus, prior))
        {
            redde NIHIL;
        }
        prior->series_tabularum = VERUM;
    }
    alioquin si (   prior->genus == TOML_VALOR_SERIES
                 && !prior->series_tabularum)
    {
        _culpa(c, s->lexema, caput, prior->definitio, TOML_CODEX_CLAUSA,
            _causa(c, "series statica '%s' clausa est: [[%s]] eam "
            "extendere non potest", _via(c, seg, n - I),
            _via(c, seg, n - I)));
        redde NIHIL;
    }
    alioquin si (prior->genus != TOML_VALOR_SERIES)
    {
        si (   prior->genus              == TOML_VALOR_TABULA
            && prior->datum.tabula.modus == TOML_TABULA_INLINEA)
        {
            _culpa(c, s->lexema, caput, prior->definitio,
                TOML_CODEX_CLAUSA, _causa(c,
                "tabula inlinea '%s' clausa est: post "
                "scriptionem nihil "
                "ei addi potest%s", _via(c, seg, n - I), NIHIL));
            redde NIHIL;
        }
        _culpa(c, s->lexema, caput, prior->definitio,
            TOML_CODEX_GENUS_ALIENUM, _causa(c,
            "'%s' iam %s est, non series tabularum",
            _via(c, seg, n - I), _genus_nomen(prior)));
        redde NIHIL;
    }
    {
        TomlValor* elementum = _tabula_nova(c, TOML_TABULA_EXPLICITA,
                                   caput, clavis);

        si (elementum == NIHIL || !_series_appendere(prior, elementum))
        {
            redde NIHIL;
        }
        redde elementum;
    }
}


/* ==================================================
 * Lineae intra tabulas inlineas (TOML 1.1)
 * ================================================== */

nomen structura {
      Coctor* coctor;
         Xar* acervus;    /* constans MateriaNodus* uncorum apertorum */
         s32  cacumen;
} Unci;

interior b32
_clausura_adest (
    constans MateriaNodus* nodus)
{
    redde nodus->loci[TOML_INCLUSA_TOK_CLAUSURA].genus
        == MATERIA_VALOR_TOKEN
        && nodus->loci[TOML_INCLUSA_TOK_CLAUSURA].datum.token != NIHIL;
}

interior vacuum
_uncos_sequi (
                    vacuum* datum,
     constans MateriaToken* t,
     constans MateriaNodus* pater,
                       b32  in_malo,
                       b32  trivium)
{
    Unci* u = (Unci*)datum;

    (vacuum)in_malo;
    si (trivium)
    {
        constans MateriaNodus** summum;

        si (t->genus != (s32)TOML_LEX_LINEA || u->cacumen == ZEPHYRUM)
        {
            redde;
        }
        summum = (constans MateriaNodus**)xar_obtinere(u->acervus,
            (i32)(u->cacumen - I));
        si ((*summum)->genus == (s32)TOML_GENUS_TABULA_COMPACTA)
        {
            _culpa(u->coctor, t, *summum, NIHIL,
                TOML_CODEX_VERSIO_NOVIOR,
                "linea nova intra tabulam inlineam: TOML 1.1, non 1.0");
        }
        redde;
    }
    si (   pater == NIHIL
        || (pater->genus != (s32)TOML_GENUS_TABULA_COMPACTA
            && pater->genus != (s32)TOML_GENUS_SERIES)
        || !_clausura_adest(pater))
    {
        redde;
    }
    si (t == pater->loci[TOML_INCLUSA_TOK_APERTURA].datum.token)
    {
        constans MateriaNodus** locus;

        si (u->cacumen < (s32)xar_numerus(u->acervus))
        {
            locus = (constans MateriaNodus**)xar_obtinere(u->acervus,
                (i32)u->cacumen);
        }
        alioquin
        {
            locus = (constans MateriaNodus**)xar_addere(u->acervus);
        }
        si (locus != NIHIL)
        {
            *locus = pater;
            u->cacumen++;
        }
    }
    alioquin si (   t
                 == pater->loci[TOML_INCLUSA_TOK_CLAUSURA].datum.token
                 && u->cacumen > ZEPHYRUM)
    {
        u->cacumen--;
    }
}


/* ==================================================
 * Ordo diagnosticorum
 * ================================================== */

nomen structura {
    MateriaDiagnosticum d;
                    i32 ordo;
} Ordinandum;

interior integer
_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Ordinandum* x = (constans Ordinandum*)a;
    constans Ordinandum* y = (constans Ordinandum*)b;

    si (x->d.tractus.initium != y->d.tractus.initium)
    {
        redde x->d.tractus.initium < y->d.tractus.initium ? -I : I;
    }
    redde x->ordo < y->ordo ? -I : (x->ordo > y->ordo ? I : ZEPHYRUM);
}

interior Xar*
_ordinare (
    Piscina* piscina,
        Xar* fontes[III])
{
           i32  summa = ZEPHYRUM;
           i32  k;
           i32  j;
           i32  m = ZEPHYRUM;
    Ordinandum* omnia;
           Xar* exitus = xar_creare(piscina,
                            (i32)magnitudo(MateriaDiagnosticum));

    per (k = ZEPHYRUM; k < III; k++)
    {
        summa += fontes[k] != NIHIL ? xar_numerus(fontes[k]) : ZEPHYRUM;
    }
    si (exitus == NIHIL || summa == ZEPHYRUM)
    {
        redde exitus;
    }
    omnia = (Ordinandum*)piscina_allocare(piscina,
        (i64)summa * (i64)magnitudo(Ordinandum));
    si (omnia == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        per (j = ZEPHYRUM; fontes[k] != NIHIL
            && j < xar_numerus(fontes[k]);
             j++)
        {
            omnia[m].d =
                *(MateriaDiagnosticum*)xar_obtinere(fontes[k], j);
            omnia[m].ordo = m;
            m++;
        }
    }
    qsort(omnia, (size_t)summa, magnitudo(Ordinandum), _comparare);
    per (k = ZEPHYRUM; k < summa; k++)
    {
        MateriaDiagnosticum* d =
            (MateriaDiagnosticum*)xar_addere(exitus);

        si (d == NIHIL)
        {
            redde NIHIL;
        }
        *d = omnia[k].d;
    }
    redde exitus;
}


/* ==================================================
 * Documentum
 * ================================================== */

TomlCoctum
toml_coquere (
                  Piscina* piscina,
    constans MateriaNodus* documentum,
     constans TomlParsura* parsura)
{
    TomlCoctum  exitus;
        Coctor  c;
           Xar* fontes[III];
     TomlValor* currens;

    memset(&exitus, ZEPHYRUM, magnitudo(exitus));
    si (piscina == NIHIL)
    {
        redde exitus;
    }
    c.piscina  = piscina;
    c.sectio   = ZEPHYRUM;
    c.diagnostica = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    c.radix       = _tabula_nova(&c, TOML_TABULA_EXPLICITA, documentum,
        NIHIL);
    c.opera       = xar_creare(piscina, (i32)magnitudo(Opus));
    exitus.radix  = c.radix;
    si (   documentum == NIHIL || c.diagnostica == NIHIL
        || c.radix    == NIHIL || c.opera == NIHIL)
    {
        redde exitus;
    }

    /* structura: sententiae ordine */
    currens = c.radix;
    {
        MateriaValor l = documentum->loci[TOML_DOCUMENTUM_LIBERI];
                 i32 k;

        per (k = ZEPHYRUM; l.genus == MATERIA_VALOR_LISTA
             && k < materia_valor_lista_numerus(l); k++)
        {
              MateriaValor* e = materia_valor_lista_obtinere(l, k);
     constans MateriaNodus* s;

            si (e->genus != MATERIA_VALOR_NODUS)
            {
                perge;
            }
            s = e->datum.nodus;
            si (s->genus == (s32)TOML_GENUS_PAR)
            {
                _par(&c, currens, s);
                _opera_exhaurire(&c);
            }
            alioquin si (   s->genus == (s32)TOML_GENUS_CAPUT_TABULAE
                         || s->genus == (s32)TOML_GENUS_CAPUT_SERIEI)
            {
                TomlValor* t;

                c.sectio++;
                t = _caput(&c, s,
                    s->genus == (s32)TOML_GENUS_CAPUT_SERIEI);
                currens = t != NIHIL ? t
                    : _tabula_nova(&c, TOML_TABULA_EXPLICITA, s, NIHIL);
                si (currens == NIHIL)
                {
                    redde exitus;
                }
            }
        }
    }

    /* lineae novae intra tabulas inlineas */
    {
        Unci u;

        u.coctor  = &c;
        u.acervus = xar_creare(piscina,
            (i32)magnitudo(constans MateriaNodus*));
        u.cacumen = ZEPHYRUM;
        si (u.acervus != NIHIL)
        {
            toml_lexemata_ambulare(piscina, documentum, _uncos_sequi,
                &u);
        }
    }

    /* syntaxis: declaratio derivat quod aedificator numeravit. Parsura
     * SANA (mala, clausurae, absentiae nulla) nihil derivandum habet -
     * et derivatio in profunditate quadratica est (materia, Q11: XL
     * milia uncorum IV s). Aequivalentia in porta totalitatis per omnem
     * casum fortuitum asseritur. Parsura NIHIL = derivatur semper. */
    fontes[ZEPHYRUM] = (parsura != NIHIL && parsura->sana) ? NIHIL
        : materia_diagnostica_derivare(piscina, documentum,
            &TOML_REGISTRUM, &TOML_DIAGNOSTICA, NIHIL, NIHIL);
    fontes[I]           = toml_scalaria_iudicare(piscina, documentum);
    fontes[II]          = c.diagnostica;
    exitus.diagnostica  = _ordinare(piscina, fontes);
    exitus.sanum = exitus.diagnostica != NIHIL
        && xar_numerus(exitus.diagnostica) == ZEPHYRUM
        && (parsura == NIHIL || parsura->sana);
    redde exitus;
}
