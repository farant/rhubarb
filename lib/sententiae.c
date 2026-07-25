#include "sententiae.h"
#include "chorda_aedificator.h"

#include <string.h>

/* ====================================================================
 * SENTENTIAE - lector documentorum locis inscriptorum
 *
 * DUO GRADUS. Auxilia interiora VISIONES in textum incidentem reddunt
 * (nulla allocatio dum parsatur); functiones publicae in piscinam
 * TRANSCRIBUNT eo momento quo structura conditur. Vide sententiae.h
 * §VII: nulla visio in fontem superstes est.
 *
 * ORDO PIPAE FIXUS EST (decipula (a) in capite): continuationes
 * IUNGERE, deinde tags, deinde remissiones. Quicumque tags ante
 * iuncturam inspicit sex ex octo retractationibus amittit - et
 * TACITE.
 * ==================================================================== */

/* --------------------------------------------------------------------
 * I. AUXILIA MINIMA
 * -------------------------------------------------------------------- */

interior chorda
_ch (constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    /* -Wcast-qual: unio qualificatorem tollit sine calumnia */
    u.l = litterae;
    c.datum   = u.m;
    c.mensura = (i32)strlen(litterae);
    redde c;
}

interior chorda
_vacua (vacuum)
{
    chorda c;
    c.datum   = NIHIL;
    c.mensura = ZEPHYRUM;
    redde c;
}

interior chorda
_copia (chorda s, Piscina* piscina)
{
    chorda fructus;

    si (s.mensura == ZEPHYRUM || s.datum == NIHIL) { redde _vacua(); }

    fructus.datum = (i8*)piscina_allocare(piscina, (memoriae_index)s.mensura);
    si (fructus.datum == NIHIL) { redde _vacua(); }

    memcpy(fructus.datum, s.datum, (memoriae_index)s.mensura);
    fructus.mensura = s.mensura;
    redde fructus;
}

/* Probatio albi super i8 - NON <ctype.h>. Character signatus est in hac
 * machina, ergo octetus UTF-8 principalis negativus fieret et isspace()
 * indefinite ageret. Quattuor characteres explicite. */
interior b32
_album (i8 c)
{
    redde (b32)(c == (i8)' ' || c == (i8)'\t' ||
                c == (i8)'\r' || c == (i8)'\n');
}

interior b32
_numeralis (i8 c)
{
    redde (b32)(c >= (i8)'0' && c <= (i8)'9');
}

interior b32
_minuscula (i8 c)
{
    redde (b32)(c >= (i8)'a' && c <= (i8)'z');
}

interior b32
_maiuscula (i8 c)
{
    redde (b32)(c >= (i8)'A' && c <= (i8)'Z');
}

/* Cursor linearum: nulla allocatio, visio redditur. Lineam finalem sine
 * '\n' tractat; '\r' praecedens detrahit (decipula insertionis - plagula
 * ipsa nullum CRLF habet, sed pyxis transcriptionis eum inducere potest). */
interior b32
_linea_proxima (chorda totum, i32* positus, chorda* linea)
{
    i32 initium;
    i32 i;

    si (totum.datum == NIHIL || *positus >= totum.mensura) { redde FALSUM; }

    initium = *positus;
    i       = initium;
    dum (i < totum.mensura && totum.datum[i] != (i8)'\n') { i++; }

    linea->datum   = totum.datum + initium;
    linea->mensura = i - initium;

    si (linea->mensura > ZEPHYRUM &&
        linea->datum[linea->mensura - I] == (i8)'\r')
    { linea->mensura--; }

    *positus = (i < totum.mensura) ? (i + I) : totum.mensura;
    redde VERUM;
}

interior i32
_lineas_numerare (chorda s, i32 usque)
{
    i32 i;
    i32 n = I;

    per (i = ZEPHYRUM; i < usque && i < s.mensura; i++)
    { si (s.datum[i] == (i8)'\n') { n++; } }
    redde n;
}

/* Asteriscos, spatia, punctum finale detrahere - pro campis prooemii
 * ("**DI**" -> "DI", "DRAFT." -> "DRAFT"). */
interior chorda
_purgare (chorda s)
{
    chorda r;
    i32    a = ZEPHYRUM;
    i32    b;

    si (s.datum == NIHIL) { redde _vacua(); }
    b = s.mensura;

    dum (a < b && (_album(s.datum[a]) || s.datum[a] == (i8)'*')) { a++; }
    dum (b > a && (_album(s.datum[b - I]) || s.datum[b - I] == (i8)'*' ||
                   s.datum[b - I] == (i8)'.'))
    { b--; }

    r.datum   = s.datum + a;
    r.mensura = b - a;
    redde r;
}

/* --------------------------------------------------------------------
 * II. LOCUS
 *
 * locus := numerus ( "." segmentum )* "."
 *
 * PRIMUM SEGMENTUM NUMERICUM (decipula (b)): aliter quodlibet verbum
 * Anglicum puncto terminatum locus fit et prooemium CCLXXV congruentias
 * reddit ubi CCLXXIII sunt.
 *
 * Mensura REDDITA punctum finale EXCLUDIT: "4.obj.1.1", non
 * "4.obj.1.1." - forma canonica citationis (DI/4.obj.1.1).
 * -------------------------------------------------------------------- */

interior b32
_locus_capere (chorda linea, chorda* locus, i32* profunditas)
{
    i32 i        = ZEPHYRUM;
    i32 segmenta = ZEPHYRUM;

    si (linea.datum == NIHIL || linea.mensura == ZEPHYRUM) { redde FALSUM; }

    dum (VERUM)
    {
        i32 initium_seg = i;

        si (segmenta == ZEPHYRUM)
        {
            dum (i < linea.mensura && _numeralis(linea.datum[i])) { i++; }
        }
        alioquin si (i < linea.mensura && _numeralis(linea.datum[i]))
        {
            dum (i < linea.mensura && _numeralis(linea.datum[i])) { i++; }
        }
        alioquin
        {
            dum (i < linea.mensura && _minuscula(linea.datum[i])) { i++; }
        }

        si (i == initium_seg) { redde FALSUM; }
        si (i >= linea.mensura || linea.datum[i] != (i8)'.') { redde FALSUM; }

        i++;
        segmenta++;
        si (segmenta > (i32)SENTENTIAE_PROFUNDITAS_MAXIMA) { redde FALSUM; }

        si (i < linea.mensura && linea.datum[i] == (i8)' ')
        {
            locus->datum   = linea.datum;
            locus->mensura = i - I;      /* punctum finale exclusum */
            *profunditas   = segmenta;
            redde VERUM;
        }
        si (i >= linea.mensura) { redde FALSUM; }
    }
}

b32
sententiae_parentem_capere (chorda locus, chorda* parens)
{
    s32 index;

    si (locus.datum == NIHIL || locus.mensura == ZEPHYRUM || parens == NIHIL)
    { redde FALSUM; }

    index = chorda_invenire_ultimum_index(locus, _ch("."));
    si (index < (s32)ZEPHYRUM) { redde FALSUM; }

    parens->datum   = locus.datum;
    parens->mensura = (i32)index;
    redde VERUM;
}

interior b32
_omnia_numeralia (chorda locus)
{
    i32 i;

    per (i = ZEPHYRUM; i < locus.mensura; i++)
    {
        si (!_numeralis(locus.datum[i]) && locus.datum[i] != (i8)'.')
        { redde FALSUM; }
    }
    redde VERUM;
}

/* --------------------------------------------------------------------
 * III. GRADUS
 * -------------------------------------------------------------------- */

interior s32
_gradus_invenire (constans Xar* vocabularium, chorda contentum, chorda* onus)
{
    i32 k;

    si (onus != NIHIL) { *onus = _vacua(); }
    si (vocabularium == NIHIL) { redde (s32)(-I); }

    per (k = ZEPHYRUM; k < xar_numerus(vocabularium); k++)
    {
        Gradus* g = (Gradus*)xar_obtinere(vocabularium, (i32)k);

        si (g == NIHIL) { perge; }

        si (chorda_aequalis(g->vocabulum, contentum)) { redde (s32)k; }

        si (contentum.mensura > g->vocabulum.mensura &&
            chorda_incipit(contentum, g->vocabulum) &&
            contentum.datum[g->vocabulum.mensura] == (i8)':')
        {
            si (onus != NIHIL)
            {
                chorda o;
                o.datum   = contentum.datum + g->vocabulum.mensura + I;
                o.mensura = contentum.mensura - g->vocabulum.mensura - I;
                *onus = chorda_praecidere(o);
            }
            redde (s32)k;
        }
    }
    redde (s32)(-I);
}

/* Vocabularium ex PROOEMIO colligere - positionaliter, non per phrasin
 * (decipula (c) et §III capitis). Formam nexus markdown [x](y)
 * praeterit; uncina quae verbo minusculo non incipiunt praeterit. */
interior vacuum
_vocabularium_colligere (chorda prooemium, Xar* vocabularium, Piscina* piscina)
{
    i32 i = ZEPHYRUM;

    dum (i < prooemium.mensura)
    {
        i32    apertum;
        i32    clausum;
        i32    j;
        i32    k;
        chorda contentum;
        chorda vocabulum;
        b32    onus = FALSUM;
        b32    iam  = FALSUM;

        si (prooemium.datum[i] != (i8)'[') { i++; perge; }

        apertum = i;
        clausum = apertum + I;
        dum (clausum < prooemium.mensura &&
             prooemium.datum[clausum] != (i8)']')
        { clausum++; }

        si (clausum >= prooemium.mensura) { frange; }

        si (clausum + I < prooemium.mensura &&
            prooemium.datum[clausum + I] == (i8)'(')
        { i = clausum + I; perge; }

        contentum.datum   = prooemium.datum + apertum + I;
        contentum.mensura = clausum - apertum - I;

        j = ZEPHYRUM;
        dum (j < contentum.mensura && _minuscula(contentum.datum[j])) { j++; }
        si (j == ZEPHYRUM) { i = clausum + I; perge; }

        vocabulum.datum   = contentum.datum;
        vocabulum.mensura = j;

        si (j < contentum.mensura)
        {
            si (contentum.datum[j] == (i8)':') { onus = VERUM; }
            alioquin { i = clausum + I; perge; }
        }

        per (k = ZEPHYRUM; k < xar_numerus(vocabularium); k++)
        {
            Gradus* g = (Gradus*)xar_obtinere(vocabularium, (i32)k);
            si (g != NIHIL && chorda_aequalis(g->vocabulum, vocabulum))
            { iam = VERUM; frange; }
        }

        si (!iam)
        {
            Gradus* g = (Gradus*)xar_addere(vocabularium);
            si (g != NIHIL)
            {
                g->vocabulum = _copia(vocabulum, piscina);
                g->fert_onus = onus;
            }
        }

        i = clausum + I;
    }
}

/* --------------------------------------------------------------------
 * IV. PROOEMIUM
 * -------------------------------------------------------------------- */

interior i32
_prooemium_finis (chorda fons)
{
    i32    positus = ZEPHYRUM;
    chorda linea;

    dum (VERUM)
    {
        i32 initium = positus;

        si (!_linea_proxima(fons, &positus, &linea)) { frange; }
        si (chorda_incipit(linea, _ch("```")) ||
            chorda_incipit(linea, _ch("## ")))
        { redde initium; }
    }
    redde fons.mensura;
}

interior b32
_cella_separatrix (chorda c)
{
    i32 i;
    i32 signa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] == (i8)'-' || c.datum[i] == (i8)':') { signa++; }
        alioquin si (!_album(c.datum[i])) { redde FALSUM; }
    }
    redde (b32)(signa > ZEPHYRUM);
}

interior vacuum
_auctoritates_colligere (chorda prooemium, Xar* auctoritates, Piscina* piscina)
{
    i32    positus = ZEPHYRUM;
    chorda linea;

    dum (_linea_proxima(prooemium, &positus, &linea))
    {
        chorda_fissio_fructus fissio;
        chorda                siglum;
        chorda                descriptio;
        Auctoritas*           a;

        si (!chorda_incipit(chorda_praecidere(linea), _ch("|"))) { perge; }

        fissio = chorda_fissio(linea, '|', piscina);
        si (fissio.numerus < (i32)IV) { perge; }

        siglum     = chorda_praecidere(fissio.elementa[I]);
        descriptio = chorda_praecidere(fissio.elementa[II]);

        si (siglum.mensura == ZEPHYRUM) { perge; }
        si (_cella_separatrix(siglum)) { perge; }
        si (chorda_aequalis_literis(siglum, "Siglum")) { perge; }

        a = (Auctoritas*)xar_addere(auctoritates);
        si (a == NIHIL) { perge; }
        a->siglum     = _copia(siglum, piscina);
        a->descriptio = _copia(descriptio, piscina);
    }
}

interior vacuum
_prooemium_legere (chorda prooemium, Liber* liber, Piscina* piscina)
{
    i32    positus = ZEPHYRUM;
    chorda linea;

    liber->titulus = _vacua();
    liber->siglum  = _vacua();
    liber->status  = _vacua();

    dum (_linea_proxima(prooemium, &positus, &linea))
    {
        s32 index;

        si (liber->titulus.mensura == ZEPHYRUM &&
            chorda_incipit(linea, _ch("# ")))
        {
            chorda t;
            t.datum   = linea.datum + II;
            t.mensura = linea.mensura - II;
            liber->titulus = _copia(chorda_praecidere(t), piscina);
            perge;
        }

        si (liber->siglum.mensura == ZEPHYRUM)
        {
            index = chorda_invenire_index(linea, _ch("Siglum:"));
            si (index >= (s32)ZEPHYRUM)
            {
                chorda r;
                r.datum   = linea.datum + (i32)index + (i32)VII;
                r.mensura = linea.mensura - (i32)index - (i32)VII;
                liber->siglum = _copia(_purgare(r), piscina);
                perge;
            }
        }

        si (liber->status.mensura == ZEPHYRUM)
        {
            index = chorda_invenire_index(linea, _ch("Status:"));
            si (index >= (s32)ZEPHYRUM)
            {
                chorda r;
                i32    fin;

                r.datum   = linea.datum + (i32)index + (i32)VII;
                r.mensura = linea.mensura - (i32)index - (i32)VII;
                r = chorda_praecidere(r);

                fin = ZEPHYRUM;
                dum (fin < r.mensura && r.datum[fin] != (i8)'.' &&
                     r.datum[fin] != (i8)'*')
                { fin++; }
                r.mensura = fin;
                liber->status = _copia(chorda_praecidere(r), piscina);
            }
        }
    }

    _vocabularium_colligere(prooemium, liber->vocabularium, piscina);
    _auctoritates_colligere(prooemium, liber->auctoritates, piscina);
}

/* --------------------------------------------------------------------
 * V. TAGS ET REMISSIONES - post iuncturam SEMPER
 * -------------------------------------------------------------------- */

interior vacuum
_anomaliam_ponere (Xar* anomaliae, GenusAnomaliae genus, i32 linea,
                   chorda textus, constans character* causa, Piscina* piscina)
{
    Anomalia* a;

    si (anomaliae == NIHIL) { redde; }
    a = (Anomalia*)xar_addere(anomaliae);
    si (a == NIHIL) { redde; }

    a->genus  = genus;
    a->linea  = linea;
    a->textus = _copia(textus, piscina);
    a->causa  = _copia(_ch(causa), piscina);
}

interior CulpaStructurae
_tags_extrahere (chorda textus, constans Xar* vocabularium, Sententia* s,
                 Xar* anomaliae, Piscina* piscina)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        i32    clausum;
        i32    j;
        chorda contentum;
        chorda onus;
        s32    gradus;

        si (textus.datum[i] != (i8)'[') { i++; perge; }

        clausum = i + I;
        dum (clausum < textus.mensura && textus.datum[clausum] != (i8)']')
        { clausum++; }

        si (clausum >= textus.mensura) { redde SENTENTIAE_UNCINUS_APERTUS; }

        contentum.datum   = textus.datum + i + I;
        contentum.mensura = clausum - i - I;

        gradus = _gradus_invenire(vocabularium, contentum, &onus);
        si (gradus >= (s32)ZEPHYRUM)
        {
            s->gradus      = gradus;
            s->onus_gradus = _copia(onus, piscina);
            i = clausum + I;
            perge;
        }

        /* non gradus: annotatio libera. Si tamen gradum SIMULAT -
         * verbum minusculum solum - mendum probabile est. */
        j = ZEPHYRUM;
        dum (j < contentum.mensura && _minuscula(contentum.datum[j])) { j++; }

        si (j == contentum.mensura && j > ZEPHYRUM)
        {
            _anomaliam_ponere(anomaliae, ANOMALIA_GRADUS_IGNOTUS, s->linea,
                              contentum, "uncinum gradui simile sed in "
                              "vocabulario prooemii non declaratum", piscina);
        }
        alioquin
        {
            chorda* a = (chorda*)xar_addere(s->annotationes);
            si (a != NIHIL) { *a = _copia(contentum, piscina); }
        }

        i = clausum + I;
    }
    redde SENTENTIAE_BENE;
}

interior b32
_locum_simulat (chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM || !_numeralis(c.datum[ZEPHYRUM]))
    { redde FALSUM; }

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (!_numeralis(c.datum[i]) && !_minuscula(c.datum[i]) &&
            c.datum[i] != (i8)'.')
        { redde FALSUM; }
    }
    redde VERUM;
}

interior vacuum
_remissiones_extrahere (chorda textus, Sententia* s, Piscina* piscina)
{
    i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
        i32    clausum;
        chorda contentum;
        chorda scopus;

        si (textus.datum[i] != (i8)'(') { i++; perge; }

        clausum = i + I;
        dum (clausum < textus.mensura && textus.datum[clausum] != (i8)')')
        { clausum++; }
        si (clausum >= textus.mensura) { frange; }

        contentum.datum   = textus.datum + i + I;
        contentum.mensura = clausum - i - I;

        scopus = _vacua();
        si (chorda_incipit(contentum, _ch("see ")))
        {
            scopus.datum   = contentum.datum + IV;
            scopus.mensura = contentum.mensura - IV;
            scopus = _purgare(scopus);
        }
        alioquin si (_locum_simulat(chorda_praecidere(contentum)))
        {
            scopus = _purgare(contentum);
        }

        si (scopus.mensura > ZEPHYRUM && _locum_simulat(scopus))
        {
            Remissio* r = (Remissio*)xar_addere(s->remissiones);
            si (r != NIHIL)
            {
                r->genus    = REMISSIO_INTERNA;
                r->siglum   = _vacua();
                r->scopus   = _copia(scopus, piscina);
                r->textus   = _copia(contentum, piscina);
                r->resoluta = FALSUM;
            }
        }

        i = clausum + I;
    }
}

/* Sigla externa: verbum integrum, sensibile ad capitalia. Scopus tantum
 * capitur si character post siglum maiusculus aut numeralis est - ita
 * "ST I q.21 a.3" scopum fert, "NO calls it configuration" non (prosa
 * est, non citatio). */
interior vacuum
_sigla_extrahere (chorda textus, constans Xar* auctoritates, Sententia* s,
                  Piscina* piscina)
{
    i32 k;

    si (auctoritates == NIHIL) { redde; }

    per (k = ZEPHYRUM; k < xar_numerus(auctoritates); k++)
    {
        Auctoritas* a = (Auctoritas*)xar_obtinere(auctoritates, (i32)k);
        i32         i;

        si (a == NIHIL || a->siglum.mensura == ZEPHYRUM) { perge; }

        per (i = ZEPHYRUM; i + a->siglum.mensura <= textus.mensura; i++)
        {
            chorda    candidatus;
            chorda    scopus;
            Remissio* r;
            i32       post;

            candidatus.datum   = textus.datum + i;
            candidatus.mensura = a->siglum.mensura;

            si (!chorda_aequalis(candidatus, a->siglum)) { perge; }

            si (i > ZEPHYRUM &&
                (_minuscula(textus.datum[i - I]) ||
                 _maiuscula(textus.datum[i - I]) ||
                 _numeralis(textus.datum[i - I])))
            { perge; }

            post = i + a->siglum.mensura;
            si (post < textus.mensura &&
                (_minuscula(textus.datum[post]) ||
                 _maiuscula(textus.datum[post]) ||
                 _numeralis(textus.datum[post])))
            { perge; }

            scopus = _vacua();
            si (post + I < textus.mensura && textus.datum[post] == (i8)' ' &&
                (_maiuscula(textus.datum[post + I]) ||
                 _numeralis(textus.datum[post + I])))
            {
                i32 fin = post + I;
                dum (fin < textus.mensura && textus.datum[fin] != (i8)')' &&
                     textus.datum[fin] != (i8)';' &&
                     textus.datum[fin] != (i8)',')
                { fin++; }
                scopus.datum   = textus.datum + post + I;
                scopus.mensura = fin - post - I;
                scopus = chorda_praecidere(scopus);
            }

            r = (Remissio*)xar_addere(s->remissiones);
            si (r != NIHIL)
            {
                r->genus    = REMISSIO_EXTERNA;
                r->siglum   = a->siglum;
                r->scopus   = _copia(scopus, piscina);
                r->textus   = _copia(candidatus, piscina);
                r->resoluta = FALSUM;
            }

            i = post - I;
        }
    }
}

/* --------------------------------------------------------------------
 * VI. NORMALIZATIO ET SIGILLUM
 * -------------------------------------------------------------------- */

chorda
sententiae_normalizare (chorda textus, constans Xar* vocabularium,
                        Piscina* piscina)
{
    ChordaAedificator* aed;
    chorda             locus;
    i32                profunditas = ZEPHYRUM;
    i32                i           = ZEPHYRUM;
    b32                spatium     = FALSUM;
    b32                aliquid     = FALSUM;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    { redde _vacua(); }

    aed = chorda_aedificator_creare(piscina,
                                    (memoriae_index)(textus.mensura + XVI));
    si (aed == NIHIL) { redde _vacua(); }

    si (_locus_capere(textus, &locus, &profunditas))
    { i = locus.mensura + I; }

    dum (i < textus.mensura)
    {
        i8 c = textus.datum[i];

        si (_album(c)) { spatium = VERUM; i++; perge; }

        si (c == (i8)'[')
        {
            i32    clausum = i + I;
            chorda contentum;

            dum (clausum < textus.mensura && textus.datum[clausum] != (i8)']')
            { clausum++; }

            si (clausum < textus.mensura)
            {
                contentum.datum   = textus.datum + i + I;
                contentum.mensura = clausum - i - I;

                si (_gradus_invenire(vocabularium, contentum, NIHIL) >=
                    (s32)ZEPHYRUM)
                {
                    i = clausum + I;
                    spatium = VERUM;
                    perge;
                }
            }
        }

        si (spatium && aliquid)
        { chorda_aedificator_appendere_character(aed, ' '); }
        spatium = FALSUM;
        aliquid = VERUM;

        chorda_aedificator_appendere_character(aed, (character)c);
        i++;
    }

    redde chorda_aedificator_finire(aed);
}

Sigillum
sententiae_sigillare (chorda textus_normatus)
{
    hic_manens constans i8 nullum[I] = { (i8)0 };

    si (textus_normatus.datum == NIHIL || textus_normatus.mensura == ZEPHYRUM)
    { redde sigillum_computare(nullum, (memoriae_index)ZEPHYRUM); }

    redde sigillum_computare(textus_normatus.datum,
                             (memoriae_index)textus_normatus.mensura);
}

/* --------------------------------------------------------------------
 * VII. QUAESTIO ET ANCORAE
 * -------------------------------------------------------------------- */

Sententia*
sententiae_per_locum (constans Liber* liber, chorda locus)
{
    i32 k;

    si (liber == NIHIL || liber->sententiae == NIHIL) { redde NIHIL; }

    per (k = ZEPHYRUM; k < xar_numerus(liber->sententiae); k++)
    {
        Sententia* s = (Sententia*)xar_obtinere(liber->sententiae, (i32)k);
        si (s != NIHIL && chorda_aequalis(s->locus, locus)) { redde s; }
    }
    redde NIHIL;
}

IudiciumAncorae
sententiae_ancoram_iudicare (constans Liber* liber, chorda locus,
                             Sigillum sigillum)
{
    IudiciumAncorae iudicium;
    Sententia*      s;
    i32             k;

    iudicium.status      = ANCORA_ORBA;
    iudicium.locus_novus = _vacua();

    si (liber == NIHIL || liber->sententiae == NIHIL) { redde iudicium; }

    s = sententiae_per_locum(liber, locus);
    si (s != NIHIL)
    {
        iudicium.status = sigillum_aequale(&s->sigillum, &sigillum)
                            ? ANCORA_INTEGRA : ANCORA_MUTATA;
        redde iudicium;
    }

    /* Locus abest. Sigillum alibi quaerimus: MOTUM deprehendimus, sed
     * consulto NON sequimur - ancora mota ANNUNTIATUR, non migrat.
     * Coniectura falsa ancoram MENTIENTEM pareret, quae peior est quam
     * fracta quia nihil eam signat. */
    per (k = ZEPHYRUM; k < xar_numerus(liber->sententiae); k++)
    {
        Sententia* t = (Sententia*)xar_obtinere(liber->sententiae, (i32)k);

        si (t == NIHIL) { perge; }
        si (sigillum_aequale(&t->sigillum, &sigillum))
        {
            iudicium.status      = ANCORA_MOTA;
            iudicium.locus_novus = t->locus;
            redde iudicium;
        }
    }

    redde iudicium;
}

/* --------------------------------------------------------------------
 * VIII. LECTIO
 * -------------------------------------------------------------------- */

interior Liber
_recusare (Liber liber, CulpaStructurae culpa, i32 linea,
           constans character* causa, Piscina* piscina)
{
    liber.successus    = FALSUM;
    liber.culpa        = culpa;
    liber.linea_culpae = linea;
    liber.causa        = _copia(_ch(causa), piscina);
    redde liber;
}

/* Sententiam pendentem condere. TRIA loca eam claudunt - saeptum
 * clausum, linea vacua, locus novus - ergo UNUM corpus, non tria.
 * Villa hoc docuit: duo exemplaria codicis subtilis divergunt, et
 * divergentia hic ancoram MENTIENTEM pareret.
 *
 * `pendens` et `aed` hic purgantur, ideo vocator nihil meminisse debet. */
interior CulpaStructurae
_pendentem_condere (Liber* liber, b32* pendens, ChordaAedificator** aed,
                    chorda locus, i32 profunditas, i32 linea,
                    Piscina* piscina)
{
    Sententia* s;
    chorda     textus;

    si (!*pendens) { redde SENTENTIAE_BENE; }

    textus   = chorda_aedificator_finire(*aed);
    *aed     = NIHIL;
    *pendens = FALSUM;

    s = (Sententia*)xar_addere(liber->sententiae);
    si (s == NIHIL) { redde SENTENTIAE_BENE; }

    s->locus           = locus;
    s->profunditas     = profunditas;
    s->textus          = textus;
    s->linea           = linea;
    s->pars            = xar_numerus(liber->partes) - I;
    s->gradus          = (s32)(-I);
    s->onus_gradus     = _vacua();
    s->annotationes    = xar_creare(piscina, (i32)magnitudo(chorda));
    s->remissiones     = xar_creare(piscina, (i32)magnitudo(Remissio));
    s->retractum       = FALSUM;
    s->inresolutum     = FALSUM;
    s->textus_normatus = _vacua();

    redde _tags_extrahere(textus, liber->vocabularium, s,
                          liber->anomaliae, piscina);
}

interior vacuum
_partem_claudere (Liber* liber)
{
    i32 n = xar_numerus(liber->partes);

    si (n > ZEPHYRUM)
    {
        Pars* p = (Pars*)xar_obtinere(liber->partes, (i32)(n - I));
        si (p != NIHIL)
        { p->numerus = xar_numerus(liber->sententiae) - p->prima; }
    }
}

Liber
sententiae_legere (chorda fons, Piscina* piscina)
{
    Liber              liber;
    chorda             prooemium;
    chorda             linea;
    ChordaAedificator* aed          = NIHIL;
    chorda             locus_pendens = { ZEPHYRUM, NIHIL };
    i32                prof_pendens  = ZEPHYRUM;
    i32                linea_pendens = ZEPHYRUM;
    b32                pendens       = FALSUM;
    b32                intra         = FALSUM;
    CulpaStructurae    culpa;
    i32                positus;
    i32                numerus_lineae;
    i32                finis_prooemii;
    i32                k;

    liber.successus      = VERUM;
    liber.culpa          = SENTENTIAE_BENE;
    liber.linea_culpae   = ZEPHYRUM;
    liber.columna_culpae = ZEPHYRUM;
    liber.causa          = _vacua();
    liber.titulus        = _vacua();
    liber.siglum         = _vacua();
    liber.status         = _vacua();
    liber.prooemium      = _vacua();
    liber.vocabularium   = xar_creare(piscina, (i32)magnitudo(Gradus));
    liber.auctoritates   = xar_creare(piscina, (i32)magnitudo(Auctoritas));
    liber.partes         = xar_creare(piscina, (i32)magnitudo(Pars));
    liber.sententiae     = xar_creare(piscina, (i32)magnitudo(Sententia));
    liber.anomaliae      = xar_creare(piscina, (i32)magnitudo(Anomalia));

    si (liber.vocabularium == NIHIL || liber.auctoritates == NIHIL ||
        liber.partes == NIHIL || liber.sententiae == NIHIL ||
        liber.anomaliae == NIHIL)
    { redde _recusare(liber, SENTENTIAE_LOCUS_MALEFORMATUS, ZEPHYRUM,
                      "piscina exhausta", piscina); }

    si (fons.datum == NIHIL || fons.mensura == ZEPHYRUM) { redde liber; }

    finis_prooemii    = _prooemium_finis(fons);
    prooemium.datum   = fons.datum;
    prooemium.mensura = finis_prooemii;
    liber.prooemium   = _copia(prooemium, piscina);

    _prooemium_legere(prooemium, &liber, piscina);

    si (xar_numerus(liber.vocabularium) == ZEPHYRUM)
    {
        _anomaliam_ponere(liber.anomaliae, ANOMALIA_VOCABULARIUM_ABEST,
                          I, _vacua(),
                          "prooemium nullum gradum declarat - uncina "
                          "ante primam sententiam nulla inventa", piscina);
    }

    positus        = finis_prooemii;
    numerus_lineae = _lineas_numerare(fons, finis_prooemii);

    dum (_linea_proxima(fons, &positus, &linea))
    {
        chorda locus;
        i32    profunditas = ZEPHYRUM;
        chorda praecisa    = chorda_praecidere(linea);

        /* --- saeptum --- */
        si (chorda_incipit(linea, _ch("```")))
        {
            si (intra)
            {
                culpa = _pendentem_condere(&liber, &pendens, &aed,
                                           locus_pendens, prof_pendens,
                                           linea_pendens, piscina);
                si (culpa != SENTENTIAE_BENE)
                { redde _recusare(liber, culpa, linea_pendens,
                                  "uncinum apertum in sententia", piscina); }
            }
            intra = (b32)(!intra);
            numerus_lineae++;
            perge;
        }

        si (!intra)
        {
            si (chorda_incipit(linea, _ch("## ")))
            {
                Pars*  p;
                chorda t;

                _partem_claudere(&liber);
                p = (Pars*)xar_addere(liber.partes);
                si (p != NIHIL)
                {
                    t.datum     = linea.datum + III;
                    t.mensura   = linea.mensura - III;
                    p->titulus  = _copia(chorda_praecidere(t), piscina);
                    p->prima    = xar_numerus(liber.sententiae);
                    p->numerus  = ZEPHYRUM;
                }
            }
            numerus_lineae++;
            perge;
        }

        /* --- intra saeptum --- */
        si (praecisa.mensura == ZEPHYRUM)
        {
            culpa = _pendentem_condere(&liber, &pendens, &aed, locus_pendens,
                                       prof_pendens, linea_pendens, piscina);
            si (culpa != SENTENTIAE_BENE)
            { redde _recusare(liber, culpa, linea_pendens,
                              "uncinum apertum in sententia", piscina); }
            numerus_lineae++;
            perge;
        }

        si (_locus_capere(linea, &locus, &profunditas))
        {
            chorda reliquum;

            culpa = _pendentem_condere(&liber, &pendens, &aed, locus_pendens,
                                       prof_pendens, linea_pendens, piscina);
            si (culpa != SENTENTIAE_BENE)
            { redde _recusare(liber, culpa, linea_pendens,
                              "uncinum apertum in sententia", piscina); }

            aed = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
            si (aed == NIHIL)
            { redde _recusare(liber, SENTENTIAE_LOCUS_MALEFORMATUS,
                              numerus_lineae, "piscina exhausta", piscina); }

            reliquum.datum   = linea.datum + locus.mensura + II;
            reliquum.mensura = linea.mensura - locus.mensura - II;
            chorda_aedificator_appendere_chorda(aed,
                                                chorda_praecidere(reliquum));

            locus_pendens = _copia(locus, piscina);
            prof_pendens  = profunditas;
            linea_pendens = numerus_lineae;
            pendens       = VERUM;
        }
        alioquin si (_album(linea.datum[ZEPHYRUM]) && pendens)
        {
            chorda_aedificator_appendere_character(aed, ' ');
            chorda_aedificator_appendere_chorda(aed, praecisa);
        }
        alioquin
        {
            _anomaliam_ponere(liber.anomaliae, ANOMALIA_LINEA_VAGA,
                              numerus_lineae, praecisa,
                              "linea intra saeptum quae nec locus nec "
                              "continuatio est", piscina);
        }

        numerus_lineae++;
    }

    si (intra)
    { redde _recusare(liber, SENTENTIAE_SAEPTUM_IMPAR, numerus_lineae,
                      "saeptum apertum ad finem plagulae", piscina); }

    _partem_claudere(&liber);

    /* --- gressus posteriores: normalizatio, sigillum, iudicia, nexus --- */
    per (k = ZEPHYRUM; k < xar_numerus(liber.sententiae); k++)
    {
        Sententia* s = (Sententia*)xar_obtinere(liber.sententiae, (i32)k);
        Gradus*    g;
        chorda     parens;
        i32        j;

        si (s == NIHIL) { perge; }

        s->textus_normatus = sententiae_normalizare(s->textus,
                                                    liber.vocabularium,
                                                    piscina);
        s->sigillum = sententiae_sigillare(s->textus_normatus);

        _remissiones_extrahere(s->textus, s, piscina);
        _sigla_extrahere(s->textus, liber.auctoritates, s, piscina);

        /* IUDICIA DERIVATA - semel hic, ne consumptores dissentiant */
        si (s->gradus >= (s32)ZEPHYRUM)
        {
            g = (Gradus*)xar_obtinere(liber.vocabularium, (i32)s->gradus);
            si (g != NIHIL)
            {
                s->retractum   = chorda_aequalis_literis(g->vocabulum,
                                                         "retracted");
                s->inresolutum = chorda_aequalis_literis(g->vocabulum,
                                                         "unresolved");
            }
        }

        /* locus duplex - ancora MENTIENS aliter oreretur */
        per (j = ZEPHYRUM; j < k; j++)
        {
            Sententia* t = (Sententia*)xar_obtinere(liber.sententiae, (i32)j);
            si (t != NIHIL && chorda_aequalis(t->locus, s->locus))
            { redde _recusare(liber, SENTENTIAE_LOCUS_DUPLEX, s->linea,
                              "locus bis definitus - duae sententiae unam "
                              "ancoram vindicarent", piscina); }
        }

        /* parens absens: ANOMALIA, non culpa, et solum si omnia segmenta
         * parentis numerica sunt (decipula (e) - rami nominati ut
         * "4.obj" punctum possidens legitime non habent) */
        si (sententiae_parentem_capere(s->locus, &parens) &&
            _omnia_numeralia(parens) &&
            sententiae_per_locum(&liber, parens) == NIHIL)
        {
            _anomaliam_ponere(liber.anomaliae, ANOMALIA_PARENS_ABEST,
                              s->linea, s->locus,
                              "parens numericus huius loci non exstat",
                              piscina);
        }
    }

    /* remissiones internas resolvere - post omnes sententias */
    per (k = ZEPHYRUM; k < xar_numerus(liber.sententiae); k++)
    {
        Sententia* s = (Sententia*)xar_obtinere(liber.sententiae, (i32)k);
        i32        j;

        si (s == NIHIL || s->remissiones == NIHIL) { perge; }

        per (j = ZEPHYRUM; j < xar_numerus(s->remissiones); j++)
        {
            Remissio* r = (Remissio*)xar_obtinere(s->remissiones, (i32)j);

            si (r == NIHIL || r->genus != REMISSIO_INTERNA) { perge; }

            r->resoluta = (b32)(sententiae_per_locum(&liber, r->scopus)
                                != NIHIL);
            si (!r->resoluta)
            {
                _anomaliam_ponere(liber.anomaliae,
                                  ANOMALIA_REMISSIO_INRESOLUTA, s->linea,
                                  r->scopus,
                                  "remissio interna ad locum qui non exstat",
                                  piscina);
            }
        }
    }

    redde liber;
}

/* --------------------------------------------------------------------
 * IX. NOMINA
 * -------------------------------------------------------------------- */

constans character*
sententiae_culpae_nomen (CulpaStructurae culpa)
{
    commutatio (culpa)
    {
        casus SENTENTIAE_BENE:               redde "bene";
        casus SENTENTIAE_LOCUS_DUPLEX:       redde "locus duplex";
        casus SENTENTIAE_UNCINUS_APERTUS:    redde "uncinum apertum";
        casus SENTENTIAE_LOCUS_MALEFORMATUS: redde "locus maleformatus";
        casus SENTENTIAE_SAEPTUM_IMPAR:      redde "saeptum impar";
        casus SENTENTIAE_PROFUNDITAS_NIMIA:  redde "profunditas nimia";
        ordinarius:                          frange;
    }
    redde "ignotum";
}

constans character*
sententiae_anomaliae_nomen (GenusAnomaliae genus)
{
    commutatio (genus)
    {
        casus ANOMALIA_GRADUS_IGNOTUS:      redde "gradus ignotus";
        casus ANOMALIA_REMISSIO_INRESOLUTA: redde "remissio inresoluta";
        casus ANOMALIA_LINEA_VAGA:          redde "linea vaga";
        casus ANOMALIA_VOCABULARIUM_ABEST:  redde "vocabularium abest";
        casus ANOMALIA_PARENS_ABEST:        redde "parens abest";
        ordinarius:                         frange;
    }
    redde "ignotum";
}

constans character*
sententiae_ancorae_nomen (StatusAncorae status)
{
    commutatio (status)
    {
        casus ANCORA_INTEGRA: redde "integra";
        casus ANCORA_MOTA:    redde "mota";
        casus ANCORA_MUTATA:  redde "mutata";
        casus ANCORA_ORBA:    redde "orba";
        ordinarius:           frange;
    }
    redde "ignotum";
}
