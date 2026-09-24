/* compendium.c - vide compendium.h */
#include "compendium.h"
#include <string.h>

/* titulum in piscinam transcribere cum NUL post (consumptores
 * legati eo ut C-chorda utuntur) */
interior chorda
_transcribere (
    Piscina* piscina,
     chorda  fons)
{
    chorda  c;
        i8* d = (i8*)piscina_allocare(piscina,
            (memoriae_index)fons.mensura + I);

    c.datum    = d;
    c.mensura  = ZEPHYRUM;
    si (d == NIHIL)
    {
        redde c;
    }
    si (fons.mensura > ZEPHYRUM && fons.datum != NIHIL)
    {
        memcpy(d, fons.datum, (memoriae_index)fons.mensura);
    }
    d[fons.mensura]  = (i8)'\0';
    c.mensura        = fons.mensura;
    redde c;
}

/* radix declarationis continentis (nodus sine patre) */
interior constans SilvaNodus*
_radix (
    constans SilvaNodus* nodus)
{
    dum (nodus->pater != NIHIL)
    {
        nodus = nodus->pater;
    }
    redde nodus;
}

/* extenta ex nodo radicis ponere. PRIMUS (radix declarationis):
 * lineae si inventae, commentarium (aut -1), corpus (aut -1).
 * Definiens typedefi (repunctio): NIHIL mutatur nisi corpus in
 * plagula stat; commentarium typedefi MANET nisi definitio suum
 * fert (regula legati, excussio 2026-07-29). */
interior vacuum
_extenta_ponere (
    CompendiumDeclaratio* d,
     constans SilvaNodus* radix,
                     s32  fons,
                     b32  primus)
{
        insignatus integer la;
        insignatus integer ca;
        insignatus integer lb;
        insignatus integer cb;
                       int min_c = -I;
                       int max_c = ZEPHYRUM;
    SilvaCommentariumVista cv;
                       b32 commentarium;

    silva_nodus_extensionem(radix, (int)fons, &min_c, &max_c);
    si (!primus && min_c < ZEPHYRUM)
    {
        redde;
    }
    silva_nodus_extensionem_lineis(radix, (int)fons, &la, &ca, &lb,
        &cb);
    si (la != ZEPHYRUM)
    {
        d->linea_a = la;
        d->linea_b = lb;
    }
    commentarium = silva_commentarium_ducens(radix, (int)fons, &cv)
        == I;
    si (commentarium)
    {
        d->commentarium_initium  = (s32)cv.initium;
        d->commentarium_finis    = (s32)cv.finis;
    }
    alioquin si (primus)
    {
        d->commentarium_initium  = (s32)-I;
        d->commentarium_finis    = (s32)-I;
    }
    d->corpus_initium  = (s32)min_c;
    d->corpus_finis    = (min_c >= ZEPHYRUM) ? (s32)max_c : (s32)-I;
}

/* typedef structurae/unionis/enumerationis: nodus DEFINIENS tag
 * (NIHIL si nullus aut idem) - idioma typedef-opacum (desideratum
 * 01KXS3EXS6): charta typedefi nudi ad definitionem repungitur */
interior constans SilvaNodus*
_definiens_typi_nominati (
    constans SemanticaSymbolum* s)
{
    constans TypusC89* t = s->typus;

    si (s->genus != (int)SYMBOLUM_TYPEDEF || t == NIHIL)
    {
        redde NIHIL;
    }
    dum (t != NIHIL && t->genus == (s32)TYPUS_C89_QUALIFICATUS)
    {
        t = t->datum.qualificatus.internum;
    }
    si (   t != NIHIL
        && (   t->genus == (s32)TYPUS_C89_STRUCTURA
            || t->genus == (s32)TYPUS_C89_UNIO)
        && t->datum.tag.completa)
    {
        redde t->datum.tag.declarans;
    }
    si (   t        != NIHIL
        && t->genus == (s32)TYPUS_C89_ENUMERATUS
        && t->datum.enumeratus.completa)
    {
        redde t->datum.enumeratus.declarans;
    }
    redde NIHIL;
}

Xar*
compendium_declarationes (
      constans SilvaParsura* parsura,
    constans SilvaSemantica* semantica,
                    Piscina* piscina)
{
    Xar* declarationes = xar_creare(piscina,
        (i32)magnitudo(CompendiumDeclaratio));
    insignatus integer n;
    insignatus integer i;
                   s32 fons;

    si (   declarationes == NIHIL || parsura == NIHIL
        || semantica     == NIHIL)
    {
        redde declarationes;
    }
    fons  = parsura->fons_princeps;
    n     = silva_c89_symbola_numerus(semantica);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(semantica, i);
         constans SilvaNodus* radix;
         constans SilvaNodus* definiens;
        CompendiumDeclaratio* d;
          insignatus integer  la;
          insignatus integer  ca;
          insignatus integer  lb;
          insignatus integer  cb;
                      chorda  titulus;

        /* profunditas 0 sola; systema, implicita, sine declarante
         * exclusa */
        si (   s            == NIHIL || s->profunditas != ZEPHYRUM
            || s->ex_systemate || s->est_implicitum
            || s->declarans == NIHIL)
        {
            perge;
        }
        /* in plagula principali? (lineae declaratoris) */
        silva_nodus_extensionem_lineis(s->declarans, fons, &la, &ca,
            &lb, &cb);
        si (la == ZEPHYRUM)
        {
            perge;
        }
        d = (CompendiumDeclaratio*)xar_addere(declarationes);
        si (d == NIHIL)
        {
            redde NIHIL;
        }
        titulus.datum    = (i8*)s->titulus.datum;
        titulus.mensura  = (i32)s->titulus.mensura;
        d->symbolum      = s;
        d->titulus       = _transcribere(piscina, titulus);
        d->genus         = (s32)s->genus;
        d->linea_a       = la;
        d->linea_b       = lb;
        /* definitio? accessor generis alieni SILVA_VALOR_NIHIL
         * reddit (contractus silva.h) */
        d->est_definitio = (   s->genus == (int)SYMBOLUM_FUNCTIO
                            && silva_c89_definitio_functionis_corpus(
                                   s->declarans).genus
                               != SILVA_VALOR_NIHIL) ? VERUM : FALSUM;
        d->commentarium_initium  = (s32)-I;
        d->commentarium_finis    = (s32)-I;
        d->corpus_initium        = (s32)-I;
        d->corpus_finis          = (s32)-I;
        /* ASCENSUS PATRIS: declarans prototypi = nodus declaratoris
         * (a titulo incipit - trivia in specificatoribus EXTRA);
         * radix = declaratio continens (lineae, commentarium,
         * corpus ex RADICE - excussio legati 2026-07-29) */
        radix = _radix(s->declarans);
        _extenta_ponere(d, radix, fons, VERUM);
        definiens = _definiens_typi_nominati(s);
        si (definiens != NIHIL && _radix(definiens) != radix)
        {
            _extenta_ponere(d, _radix(definiens), fons, FALSUM);
        }
    }
    redde declarationes;
}

Xar*
compendium_ordo (
         Xar* declarationes,
     Piscina* piscina)
{
    Xar* omnes;
    Xar* ordo;
    i32  n;
    i32  i;
    s32  initium_prius = (s32)-I;

    si (declarationes == NIHIL)
    {
        redde NIHIL;
    }
    n      = xar_numerus(declarationes);
    omnes  = xar_creare(piscina, (i32)magnitudo(i32));
    ordo   = xar_creare(piscina, (i32)magnitudo(i32));
    si (omnes == NIHIL || ordo == NIHIL)
    {
        redde NIHIL;
    }
    /* insertio (n parvum): linea prior aut eadem cum genere NON
     * constantis - enumeratores eundem corpus quam typus ferunt:
     * typus primus */
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans CompendiumDeclaratio* e =
            (constans CompendiumDeclaratio*)xar_obtinere(declarationes,
                i);
        i32* locus = (i32*)xar_addere(omnes);
        i32  j;

        si (locus == NIHIL)
        {
            redde NIHIL;
        }
        j = xar_numerus(omnes) - I;
        dum (j > ZEPHYRUM)
        {
            i32 index_prioris = *(i32*)xar_obtinere(omnes, j - I);

            constans CompendiumDeclaratio* prior =
                (constans CompendiumDeclaratio*)xar_obtinere(
                    declarationes, index_prioris);

            si (   prior->linea_a < e->linea_a
                || (   prior->linea_a == e->linea_a
                    && (   prior->genus != (s32)SYMBOLUM_CONSTANS
                        || e->genus == (s32)SYMBOLUM_CONSTANS)))
            {
                frange;
            }
            *(i32*)xar_obtinere(omnes, j) = index_prioris;
            j--;
        }
        *(i32*)xar_obtinere(omnes, j) = i;
    }
    /* declaratio eadem semel; corpus absens omittitur */
    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 index = *(i32*)xar_obtinere(omnes, i);

        constans CompendiumDeclaratio* e =
            (constans CompendiumDeclaratio*)xar_obtinere(declarationes,
                index);

        si (   e->corpus_initium < ZEPHYRUM
            || e->corpus_initium == initium_prius)
        {
            perge;
        }
        initium_prius = e->corpus_initium;
        {
            i32* locus = (i32*)xar_addere(ordo);

            si (locus == NIHIL)
            {
                redde NIHIL;
            }
            *locus = index;
        }
    }
    redde ordo;
}

/* linea contracta: spatia alba in unum, nullum post '(' nec ante
 * ')'; ultra tectum praecisa */
nomen structura {
     i8* datum;
    i32  n;
    i32  tectum;
    b32  spatium;    /* spatium pendens (nondum scriptum) */
    b32  praecisa;
} CompendiumLinea;

interior vacuum
_scribere (
    CompendiumLinea* lin,
                 i8  c)
{
    si (lin->n >= lin->tectum)
    {
        lin->praecisa = VERUM;
        redde;
    }
    lin->datum[lin->n] = c;
    lin->n++;
}

interior vacuum
_contracte_appendere (
    CompendiumLinea* lin,
             chorda  c)
{
    i32 k;

    per (k = ZEPHYRUM; k < (i32)c.mensura && !lin->praecisa; k++)
    {
        i8 ch = c.datum[k];

        si (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            lin->spatium = lin->n > ZEPHYRUM;
            perge;
        }
        /* spatium pendens scribitur nisi post '(' aut ante ')' */
        si (   lin->spatium && ch != (i8)')'
            && lin->datum[lin->n - I] != (i8)'(')
        {
            _scribere(lin, (i8)' ');
        }
        lin->spatium = FALSUM;
        _scribere(lin, ch);
    }
}

chorda
compendium_contrahere (
     chorda  corpus,
        i32  tectum,
    Piscina* piscina)
{
    CompendiumLinea lin;
             chorda linea;
                s32 a = (s32)-I;
                s32 b = (s32)-I;
                i32 k;

    linea.datum    = NIHIL;
    linea.mensura  = ZEPHYRUM;
    lin.datum      = (i8*)piscina_allocare(piscina,
        (memoriae_index)tectum + V);
    si (lin.datum == NIHIL)
    {
        redde linea;
    }
    lin.n         = ZEPHYRUM;
    lin.tectum    = tectum;
    lin.spatium   = FALSUM;
    lin.praecisa  = FALSUM;
    per (k = ZEPHYRUM; k < (i32)corpus.mensura; k++)
    {
        si (corpus.datum[k] == (i8)'{' && a < ZEPHYRUM)
        {
            a = (s32)k;
        }
        si (corpus.datum[k] == (i8)'}')
        {
            b = (s32)k;
        }
    }
    si (a >= ZEPHYRUM && b > a)
    {
        chorda pars;

        pars          = corpus;
        pars.mensura  = (i32)a;
        _contracte_appendere(&lin, pars);
        _contracte_appendere(&lin, chorda_ex_literis(" {...} ",
            piscina));
        pars.datum    = corpus.datum + (i32)b + I;
        pars.mensura  = corpus.mensura - (i32)b - I;
        _contracte_appendere(&lin, pars);
    }
    alioquin
    {
        _contracte_appendere(&lin, corpus);
    }
    si (lin.praecisa)
    {
        dum (lin.n > ZEPHYRUM && lin.datum[lin.n - I] == (i8)' ')
        {
            lin.n--;
        }
        memcpy(lin.datum + lin.n, " ...", (memoriae_index)IV);
        lin.n = lin.n + IV;
    }
    linea.datum    = lin.datum;
    linea.mensura  = lin.n;
    redde linea;
}
