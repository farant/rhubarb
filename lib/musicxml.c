/* musicxml.c - lector MusicXML (caput include/musicxml.h)
 *
 * Super xml_documentum.h (visio cocta). Duo transitus: primus omnes
 * <divisions> colligit et divisionem operis computat (minimus
 * communis multiplex, M4); secundus partes per mensuras ambulat cum
 * cursore in divisionibus operis. Deinde linea temporis communis
 * (M15). Arcae .mxl per sarcina.h (META-INF/container.xml, rootfile
 * primus). Spec: project-specs/musicxml-spec.md.
 */

#include "musicxml.h"
#include "xml_documentum.h"
#include "sarcina.h"
#include "materia_diagnosticum.h"
#include "xar.h"
#include <stdlib.h>
#include <string.h>

#define DIVISIONES_MAXIMAE ((s64)2147483647)


/* ==================================================
 * Lector et vitia
 * ================================================== */

nomen structura {
    Piscina* piscina;
        Xar* vitia;          /* MusicxmlVitium */
        s64  divisiones;     /* divisio operis */
        b32  concentus;      /* concert-score: numquam transponitur */
} Lector;

interior vacuum
_vitium_locatum (
                  Lector* l,
        MusicxmlGravitas  gravitas,
     MusicxmlVitiumGenus  genus,
                     i32  linea,
                     i32  columna,
      constans character* causa,
                  chorda  adiectum)
{
       MusicxmlVitium* v = (MusicxmlVitium*)xar_addere(l->vitia);
    ChordaAedificator* a;

    si (v == NIHIL)
    {
        redde;
    }
    memset(v, ZEPHYRUM, magnitudo(MusicxmlVitium));
    v->gravitas  = gravitas;
    v->genus     = genus;
    v->linea     = linea;
    v->columna   = columna;
    a            = chorda_aedificator_creare(l->piscina, CXXVIII);
    si (a == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, causa);
    si (adiectum.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, adiectum);
    }
    v->causa = chorda_aedificator_finire(a);
}

interior vacuum
_vitium (
                  Lector* l,
        MusicxmlGravitas  gravitas,
     MusicxmlVitiumGenus  genus,
      constans XmlCoctus* nodus,
      constans character* causa,
                  chorda  adiectum)
{
    i32 linea    = ZEPHYRUM;
    i32 columna  = ZEPHYRUM;

    (vacuum)xml_locus(nodus, &linea, &columna);
    _vitium_locatum(l, gravitas, genus, linea, columna, causa,
        adiectum);
}

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}


/* ==================================================
 * Textus et numeri
 * ================================================== */

interior b32
_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* spatia utrimque abscissa (sine allocatione) */
interior chorda
_praecidere (
    chorda c)
{
    dum (c.mensura > ZEPHYRUM && _spatium((character)c.datum[ZEPHYRUM]))
    {
        c.datum++;
        c.mensura--;
    }
    dum (   c.mensura > ZEPHYRUM
         && _spatium((character)c.datum[c.mensura - I]))
    {
        c.mensura--;
    }
    redde c;
}

/* [+-]?cifrae; FALSUM si non numerus aut nimius */
interior b32
_integer (
    chorda  c,
      s64* n)
{
    s64 v      = ZEPHYRUM;
    s64 limes  = DIVISIONES_MAXIMAE * (s64)M;
    b32 minus  = FALSUM;
    i32 i      = ZEPHYRUM;

    c = _praecidere(c);
    si (   c.mensura > ZEPHYRUM
        && (c.datum[ZEPHYRUM] == '+' || c.datum[ZEPHYRUM] == '-'))
    {
        minus  = c.datum[ZEPHYRUM] == '-';
        i      = I;
    }
    si (i >= c.mensura)
    {
        redde FALSUM;
    }
    per (; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9' || v > limes)
        {
            redde FALSUM;
        }
        v = v * X + (s64)(c.datum[i] - '0');
    }
    *n = minus ? -v : v;
    redde VERUM;
}

/* [+-]?cifrae[.cifrae]; FALSUM si non numerus */
interior b32
_decimalis (
    chorda  c,
       f64* n)
{
    f64 v        = 0.0;
    f64 scala    = 1.0;
    b32 minus    = FALSUM;
    b32 punctum  = FALSUM;
    i32 cifrae   = ZEPHYRUM;
    i32 i        = ZEPHYRUM;

    c = _praecidere(c);
    si (   c.mensura > ZEPHYRUM
        && (c.datum[ZEPHYRUM] == '+' || c.datum[ZEPHYRUM] == '-'))
    {
        minus  = c.datum[ZEPHYRUM] == '-';
        i      = I;
    }
    per (; i < c.mensura; i++)
    {
        si (c.datum[i] == '.' && !punctum)
        {
            punctum = VERUM;
        }
        alioquin si (   c.datum[i] >= '0' && c.datum[i] <= '9'
                     && cifrae < XVIII)
        {
            v = v * 10.0 + (f64)(c.datum[i] - '0');
            si (punctum)
            {
                scala *= 10.0;
            }
            cifrae++;
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    si (cifrae == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *n = (minus ? -v : v) / scala;
    redde VERUM;
}

/* textus filii (praecisus); FALSUM si filius abest */
interior b32
_textus_filii (
                 Lector* l,
     constans XmlCoctus* e,
     constans character* titulus,
                 chorda* t)
{
    constans XmlCoctus* f = xml_filius(e, titulus);

    *t = _vacua();
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    si (!xml_textus_totus(f, l->piscina, t))
    {
        *t = _vacua();
    }
    *t = _praecidere(*t);
    redde VERUM;
}

/* numerus filii: VERUM si adest et legitur; monitum NUMERUS_MALUS si
 * adest sed non legitur */
interior b32
_integer_filii (
                 Lector* l,
     constans XmlCoctus* e,
     constans character* titulus,
                    s64* n)
{
    chorda t;

    si (!_textus_filii(l, e, titulus, &t))
    {
        redde FALSUM;
    }
    si (!_integer(t, n))
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS,
            xml_filius(e, titulus), "numerus illegibilis: ", t);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_attributum (
     constans XmlCoctus* e,
     constans character* titulus)
{
    chorda v;

    si (!xml_attributum(e, titulus, &v))
    {
        redde _vacua();
    }
    redde _praecidere(v);
}

interior vacuum*
_tabula (
              Lector* l,
        constans Xar* x,
                 i32  magnitudo_elementi,
                 i32* numerus)
{
    vacuum* t;
       i32  n = x != NIHIL ? xar_numerus(x) : ZEPHYRUM;

    *numerus = ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    t = piscina_allocare(l->piscina,
            (memoriae_index)(n * magnitudo_elementi));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)xar_copiare_ad_tabulam(x, t, ZEPHYRUM, n);
    *numerus = n;
    redde t;
}

interior s64
_divisor_communis (
    s64 a,
    s64 b)
{
    dum (b != ZEPHYRUM)
    {
        s64 t = a % b;

        a = b;
        b = t;
    }
    redde a < ZEPHYRUM ? -a : a;
}

/* gradus diatonicus litterae in semitoniis supra C; -I si non littera */
interior s32
_gradus (
    character littera)
{
    commutatio (littera)
    {
        casus 'C': redde ZEPHYRUM;
        casus 'D': redde II;
        casus 'E': redde IV;
        casus 'F': redde V;
        casus 'G': redde VII;
        casus 'A': redde IX;
        casus 'B': redde XI;
        ordinarius: redde -I;
    }
}

/* nota MIDI: alteratio rotundata (dimidium sursum) */
interior s32
_notam_computare (
    character littera,
          f64 alteratio,
          s32 octava)
{
    f64 a = alteratio + 0.5;
    s32 r = (s32)a;

    si (a < 0.0 && (f64)r != a)
    {
        r--;
    }
    redde XII * (octava + I) + _gradus(littera) + r;
}


/* ==================================================
 * Transitus primus: divisio operis
 * ================================================== */

/* FALSUM = multiplex nimius (vitium scriptum) */
interior b32
_divisiones_colligere (
                 Lector* l,
     constans XmlCoctus* radix)
{
     constans XmlCoctus* p;
     constans XmlCoctus* m;
     constans XmlCoctus* a;
     constans XmlCoctus* d;
                    s64  multiplex = I;

    /* partwise: part/measure; timewise: measure/part */
    per (p = xml_filius(radix, NIHIL); p != NIHIL;
         p = xml_frater(p, NIHIL))
    {
        per (m = xml_filius(p, NIHIL); m != NIHIL;
             m = xml_frater(m, NIHIL))
        {
            per (a = xml_filius(m, "attributes"); a != NIHIL;
                 a = xml_frater(a, "attributes"))
            {
                per (d = xml_filius(a, "divisions"); d != NIHIL;
                     d = xml_frater(d, "divisions"))
                {
                    chorda t;
                       s64 v;
                       s64 g;

                    si (   !xml_textus_totus(d, l->piscina, &t)
                        || !_integer(t, &v) || v <= ZEPHYRUM)
                    {
                        _vitium(l, MUSICXML_MONITUM,
                            MUSICXML_NUMERUS_MALUS, d,
                            "divisions illegibiles (praeteritae): ",
                            _praecidere(t));
                        perge;
                    }
                    g = _divisor_communis(multiplex, v);
                    si (multiplex / g > DIVISIONES_MAXIMAE / v)
                    {
                        _vitium(l, MUSICXML_VITIUM,
                            MUSICXML_DIVISIONES_NIMIAE, d,
                            "divisio operis ultra 2^31: ", t);
                        redde FALSUM;
                    }
                    multiplex = multiplex / g * v;
                }
            }
        }
    }
    l->divisiones = multiplex;
    redde VERUM;
}


/* ==================================================
 * Attributa
 * ================================================== */

nomen structura {
                chorda  vox;
                   s32  nota;
    constans XmlCoctus* nodus;
} LigaturaAperta;

nomen structura {
                         s64 factor;     /* divisio operis /
                                           * divisiones scriptae */
                          Xar* ligaturae;  /* LigaturaAperta */
                          b32  transpositio_adest;
         MusicxmlTranspositio  transpositio;   /* vigens */
constans MusicxmlInstrumentum* instrumenta;   /* partis (percussio) */
                          i32  numerus_instrumentorum;
} StatusPartis;

interior vacuum
_clavem_legere (
                 Lector* l,
     constans XmlCoctus* k,
         MusicxmlClavis* clavis)
{
     constans XmlCoctus* f;
                 chorda  t;
                    s64  v;
                    Xar* litterae;
                    Xar* alterationes;

    memset(clavis, ZEPHYRUM, magnitudo(MusicxmlClavis));
    si (_integer_filii(l, k, "fifths", &v))
    {
        clavis->tradita = VERUM;
        clavis->quintae = (s32)v;
        (vacuum)_textus_filii(l, k, "mode", &clavis->modus);
        redde;
    }
    litterae      = xar_creare(l->piscina, (i32)magnitudo(character));
    alterationes  = xar_creare(l->piscina, (i32)magnitudo(f64));
    si (litterae == NIHIL || alterationes == NIHIL)
    {
        redde;
    }
    per (f = xml_filius(k, NIHIL); f != NIHIL; f = xml_frater(f, NIHIL))
    {
        si (chorda_aequalis_literis(xml_titulus(f), "key-step"))
        {
            character* c = (character*)xar_addere(litterae);
                  f64* a = (f64*)xar_addere(alterationes);

            si (c == NIHIL || a == NIHIL)
            {
                redde;
            }
            (vacuum)xml_textus_totus(f, l->piscina, &t);
            t = _praecidere(t);
            *c = t.mensura > ZEPHYRUM ? (character)t.datum[ZEPHYRUM]
                                      : '\0';
            *a = 0.0;
        }
        alioquin si (   chorda_aequalis_literis(xml_titulus(f),
                     "key-alter")
                     && xar_numerus(alterationes) > ZEPHYRUM)
        {
            f64* a = (f64*)xar_obtinere(alterationes,
                         xar_numerus(alterationes) - I);

            (vacuum)xml_textus_totus(f, l->piscina, &t);
            si (!_decimalis(t, a))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, f,
                    "key-alter illegibile: ", _praecidere(t));
            }
        }
    }
    clavis->litterae = (character*)_tabula(l, litterae,
        (i32)magnitudo(character), &clavis->numerus_graduum);
    clavis->alterationes = (f64*)_tabula(l, alterationes,
        (i32)magnitudo(f64), &clavis->numerus_graduum);
}

interior vacuum
_metrum_legere (
                 Lector* l,
     constans XmlCoctus* tm,
         MusicxmlMetrum* metrum)
{
     constans XmlCoctus* f;
                    Xar* partes;

    memset(metrum, ZEPHYRUM, magnitudo(MusicxmlMetrum));
    metrum->symbolum = _attributum(tm, "symbol");
    si (xml_filius(tm, "senza-misura") != NIHIL)
    {
        metrum->liberum = VERUM;
        redde;
    }
    partes = xar_creare(l->piscina, (i32)magnitudo(MusicxmlMetriPars));
    si (partes == NIHIL)
    {
        redde;
    }
    per (f = xml_filius(tm, NIHIL); f != NIHIL; f = xml_frater(f,
        NIHIL))
    {
        chorda t;

        si (chorda_aequalis_literis(xml_titulus(f), "beats"))
        {
            MusicxmlMetriPars* p =
                (MusicxmlMetriPars*)xar_addere(partes);
                          i32 i;
                          i32 initium = ZEPHYRUM;

            si (p == NIHIL)
            {
                redde;
            }
            memset(p, ZEPHYRUM, magnitudo(MusicxmlMetriPars));
            (vacuum)xml_textus_totus(f, l->piscina, &t);
            p->pulsus = _praecidere(t);
            /* summa partium '+' (metrum additivum) */
            per (i = ZEPHYRUM; i <= p->pulsus.mensura; i++)
            {
                si (i == p->pulsus.mensura || p->pulsus.datum[i] == '+')
                {
                    s64 v;

                    si (_integer(chorda_sectio(p->pulsus, initium, i),
                            &v) && v > ZEPHYRUM)
                    {
                        p->summa += (i32)v;
                    }
                    alioquin
                    {
                        _vitium(l, MUSICXML_MONITUM,
                            MUSICXML_NUMERUS_MALUS, f,
                            "beats illegibile: ", p->pulsus);
                        frange;
                    }
                    initium = i + I;
                }
            }
        }
        alioquin si (   chorda_aequalis_literis(xml_titulus(f),
                     "beat-type")
                     && xar_numerus(partes) > ZEPHYRUM)
        {
            MusicxmlMetriPars* p = (MusicxmlMetriPars*)xar_obtinere(
                partes, xar_numerus(partes) - I);
                          s64 v;

            (vacuum)xml_textus_totus(f, l->piscina, &t);
            si (_integer(t, &v) && v > ZEPHYRUM)
            {
                p->unitas = (i32)v;
            }
            alioquin
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, f,
                    "beat-type illegibile: ", _praecidere(t));
            }
        }
    }
    metrum->partes = (MusicxmlMetriPars*)_tabula(l, partes,
        (i32)magnitudo(MusicxmlMetriPars), &metrum->numerus_partium);
}

interior b32
_attributa_legere (
                 Lector* l,
     constans XmlCoctus* e,
                    s64  positio,
           StatusPartis* sp,
      MusicxmlAttributa* t)
{
     constans XmlCoctus* f;
                    s64  v;
                    Xar* claviculae;

    memset(t, ZEPHYRUM, magnitudo(MusicxmlAttributa));
    t->positio  = positio;
    f           = xml_filius(e, "divisions");
    si (f != NIHIL)
    {
        chorda c;

        /* illegibiles iam in transitu primo nominatae */
        si (   xml_textus_totus(f, l->piscina, &c) && _integer(c, &v)
            && v > ZEPHYRUM)
        {
            t->divisiones  = v;
            sp->factor     = l->divisiones / v;
        }
    }
    f = xml_filius(e, "key");
    si (f != NIHIL)
    {
        t->clavis_adest = VERUM;
        _clavem_legere(l, f, &t->clavis);
    }
    f = xml_filius(e, "time");
    si (f != NIHIL)
    {
        t->metrum_adest = VERUM;
        _metrum_legere(l, f, &t->metrum);
    }
    si (_integer_filii(l, e, "staves", &v) && v > ZEPHYRUM)
    {
        t->baculi = (i32)v;
    }
    claviculae = xar_creare(l->piscina,
        (i32)magnitudo(MusicxmlClavicula));
    si (claviculae == NIHIL)
    {
        redde FALSUM;
    }
    per (f = xml_filius(e, "clef"); f != NIHIL; f = xml_frater(f,
        "clef"))
    {
        MusicxmlClavicula* k =
            (MusicxmlClavicula*)xar_addere(claviculae);
                   chorda s;

        si (k == NIHIL)
        {
            redde FALSUM;
        }
        memset(k, ZEPHYRUM, magnitudo(MusicxmlClavicula));
        k->baculum = I;
        si (_integer(_attributum(f, "number"), &v) && v > ZEPHYRUM)
        {
            k->baculum = (i32)v;
        }
        si (_textus_filii(l, f, "sign", &s) && s.mensura > ZEPHYRUM)
        {
            k->signum = (character)s.datum[ZEPHYRUM];
            si (chorda_aequalis_literis(s, "percussion"))
            {
                k->signum = 'P';
            }
            alioquin si (chorda_aequalis_literis(s, "TAB"))
            {
                k->signum = 'T';
            }
            alioquin si (chorda_aequalis_literis(s, "none"))
            {
                k->signum = 'N';
            }
        }
        si (_integer_filii(l, f, "line", &v))
        {
            k->linea = (i32)v;
        }
        si (_integer_filii(l, f, "clef-octave-change", &v))
        {
            k->octava_mutata = (s32)v;
        }
    }
    t->claviculae = (MusicxmlClavicula*)_tabula(l, claviculae,
        (i32)magnitudo(MusicxmlClavicula), &t->numerus_clavicularum);
    f = xml_filius(e, "transpose");
    si (f != NIHIL)
    {
        t->transpositio_adest = VERUM;
        si (_integer_filii(l, f, "diatonic", &v))
        {
            t->transpositio.diatonica = (s32)v;
        }
        si (_integer_filii(l, f, "chromatic", &v))
        {
            t->transpositio.chromatica = (s32)v;
        }
        si (_integer_filii(l, f, "octave-change", &v))
        {
            t->transpositio.octava_mutata = (s32)v;
        }
        t->transpositio.duplicata  = xml_filius(f, "double") != NIHIL;
        sp->transpositio_adest     = VERUM;
        sp->transpositio           = t->transpositio;
    }
    redde VERUM;
}


/* ==================================================
 * Notae
 * ================================================== */

nomen structura {
     s64 cursor;
     s64 maximum;
     b32 prior_adest;        /* nota prior (non acordi) in mensura */
     s64 prior_positio;
  chorda prior_vox;
} StatusMensurae;

/* duratio in divisionibus operis; FALSUM si abest aut illegibilis */
interior b32
_duratio (
                 Lector* l,
     constans XmlCoctus* e,
           StatusPartis* sp,
                    s64* d)
{
    s64 v;

    si (!_integer_filii(l, e, "duration", &v))
    {
        redde FALSUM;
    }
    si (v < ZEPHYRUM)
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, e,
            "duration negativa", _vacua());
        redde FALSUM;
    }
    *d = v * sp->factor;
    redde VERUM;
}

interior vacuum
_ligaturam_tractare (
                 Lector* l,
           StatusPartis* sp,
     constans XmlCoctus* e,
           MusicxmlNota* n)
{
    i32 i;

    si (n->genus != MUSICXML_NOTA_SONANS)
    {
        redde;
    }
    si (n->ligatura_finis)
    {
        b32 inventa = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(sp->ligaturae); i++)
        {
            LigaturaAperta* a = (LigaturaAperta*)xar_obtinere(
                sp->ligaturae, i);

            si (a->nota == n->nota && chorda_aequalis(a->vox, n->vox))
            {
                /* ultima in locum eius, deinde ultima tollitur */
                *a = *(LigaturaAperta*)xar_obtinere(sp->ligaturae,
                    xar_numerus(sp->ligaturae) - I);
                (vacuum)xar_removere_ultimum(sp->ligaturae);
                inventa = VERUM;
                frange;
            }
        }
        si (!inventa)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_LIGATURA_PENDENS, e,
                "tie stop sine initio", _vacua());
        }
    }
    si (n->ligatura_initium)
    {
        LigaturaAperta* a = (LigaturaAperta*)xar_addere(sp->ligaturae);

        si (a != NIHIL)
        {
            a->vox    = n->vox;
            a->nota   = n->nota;
            a->nodus  = e;
        }
    }
}

/* steal-time: centesimae (decimales rotundatae); 0 si absens */
interior s32
_centesimae (
    chorda c)
{
    f64 v;

    si (c.mensura == ZEPHYRUM || !_decimalis(c, &v))
    {
        redde ZEPHYRUM;
    }
    redde (s32)(v < 0.0 ? v - 0.5 : v + 0.5);
}

/* index litterae in "CDEFGAB"; -I si non littera */
interior s32
_ordo_litterae (
    character littera)
{
     constans character* litterae = "CDEFGAB";
                    s32  i;

    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (litterae[i] == littera)
        {
            redde i;
        }
    }
    redde -I;
}

/* sonans ex scripta: littera movetur per diatonic (+ VII octavae),
 * nota MIDI per chromatic (+ XII octavae); alteratio sonans = quod
 * reliquum est. concert-score: notae iam sonantes (numquam bis).
 * Percussa: nota ex instrumento (midi-unpitched - I) si notum. */
interior vacuum
_sonantem_computare (
    constans StatusPartis* sp,
                      b32  concentus,
             MusicxmlNota* n)
{
    s32 diatonica;
    s32 chromatica;
    s32 summa;
    s32 octava;
    s32 ordo;
    s32 naturale;
    i32 i;

    n->littera_sonans    = n->littera;
    n->alteratio_sonans  = n->alteratio;
    n->octava_sonans     = n->octava;
    si (n->littera == '\0' || n->genus == MUSICXML_NOTA_PAUSA)
    {
        redde;
    }
    si (n->genus == MUSICXML_NOTA_PERCUSSA)
    {
        n->nota = _notam_computare(n->littera, n->alteratio, n->octava);
        per (i = ZEPHYRUM; i < sp->numerus_instrumentorum; i++)
        {
            constans MusicxmlInstrumentum* x = &sp->instrumenta[i];

            si (   x->percussio > ZEPHYRUM
                && (chorda_aequalis(x->id, n->instrumentum)
                    || (n->instrumentum.mensura == ZEPHYRUM
                        && sp->numerus_instrumentorum == I)))
            {
                n->nota = (s32)x->percussio - I;
                frange;
            }
        }
        redde;
    }
    si (concentus || !sp->transpositio_adest)
    {
        n->nota = _notam_computare(n->littera, n->alteratio, n->octava);
        redde;
    }
    diatonica   = sp->transpositio.diatonica
                  + VII * sp->transpositio.octava_mutata;
    chromatica  = sp->transpositio.chromatica
                  + XII * sp->transpositio.octava_mutata;
    summa = n->octava * VII + _ordo_litterae(n->littera) + diatonica;
    octava = summa >= ZEPHYRUM ? summa / VII : -((VI - summa) / VII);
    ordo = summa - VII * octava;
    n->littera_sonans = "CDEFGAB"[ordo];
    n->octava_sonans = octava;
    naturale = (XII * octava + _gradus(n->littera_sonans))
               - (XII * n->octava + _gradus(n->littera));
    n->alteratio_sonans = n->alteratio + (f64)(chromatica - naturale);
    n->nota = _notam_computare(n->littera_sonans, n->alteratio_sonans,
        n->octava_sonans);
}

interior MusicxmlLocusSyllabae
_locum_syllabae_legere (
                 Lector* l,
     constans XmlCoctus* f)
{
    chorda t;

    (vacuum)xml_textus_totus(f, l->piscina, &t);
    t = _praecidere(t);
    si (chorda_aequalis_literis(t, "single"))
    {
        redde MUSICXML_SYLLABA_SINGULA;
    }
    si (chorda_aequalis_literis(t, "begin"))
    {
        redde MUSICXML_SYLLABA_INITIUM;
    }
    si (chorda_aequalis_literis(t, "middle"))
    {
        redde MUSICXML_SYLLABA_MEDIA;
    }
    si (chorda_aequalis_literis(t, "end"))
    {
        redde MUSICXML_SYLLABA_FINIS;
    }
    _vitium(l, MUSICXML_MONITUM, MUSICXML_VALOR_IGNOTUS, f,
        "syllabic ignotum: ", t);
    redde MUSICXML_SYLLABA_IGNOTA;
}

/* verba (lyric): syllaba per <text>; <elision> iungit sequentem in
 * eadem nota; <extend> ultimam notat. Versus absens = I;
 * illegibilis = 0.
 * Ordo: versus (stabilis - ordo documenti intra versum). */
interior vacuum
_verba_legere (
                 Lector* l,
     constans XmlCoctus* e,
           MusicxmlNota* n)
{
     constans XmlCoctus* ly;
     constans XmlCoctus* f;
                    Xar* syllabae;
                    i32  i;
                    i32  j;

    si (xml_filius(e, "lyric") == NIHIL)
    {
        redde;
    }
    syllabae = xar_creare(l->piscina, (i32)magnitudo(MusicxmlSyllaba));
    si (syllabae == NIHIL)
    {
        redde;
    }
    per (ly = xml_filius(e, "lyric"); ly != NIHIL;
         ly = xml_frater(ly, "lyric"))
    {
                   chorda numerus  = _attributum(ly, "number");
                      i32 versus   = I;
                      s64 v;
    MusicxmlLocusSyllabae locus   = MUSICXML_SYLLABA_SINGULA;
                      b32 iuncta  = FALSUM;
                      s32 ultima  = -I;

        si (numerus.mensura > ZEPHYRUM)
        {
            versus = (_integer(numerus, &v) && v > ZEPHYRUM) ? (i32)v
                                                            : ZEPHYRUM;
        }
        per (f = xml_filius(ly, NIHIL); f != NIHIL;
             f = xml_frater(f, NIHIL))
        {
            chorda titulus = xml_titulus(f);

            si (chorda_aequalis_literis(titulus, "syllabic"))
            {
                locus = _locum_syllabae_legere(l, f);
            }
            alioquin si (chorda_aequalis_literis(titulus, "text"))
            {
                MusicxmlSyllaba* y = (MusicxmlSyllaba*)xar_addere(
                    syllabae);

                si (y == NIHIL)
                {
                    redde;
                }
                memset(y, ZEPHYRUM, magnitudo(MusicxmlSyllaba));
                y->versus         = versus;
                y->versus_textus  = numerus.mensura > ZEPHYRUM
                                        ? numerus
                                        : chorda_ex_literis("1",
                                              l->piscina);
                y->locus   = locus;
                y->iuncta  = iuncta;
                si (!xml_textus_totus(f, l->piscina, &y->textus))
                {
                    y->textus = _vacua();
                }
                y->textus  = _praecidere(y->textus);
                ultima     = (s32)xar_numerus(syllabae) - I;
                locus      = MUSICXML_SYLLABA_SINGULA;
                iuncta     = FALSUM;
            }
            alioquin si (chorda_aequalis_literis(titulus, "elision"))
            {
                iuncta = VERUM;
            }
            alioquin si (   chorda_aequalis_literis(titulus, "extend")
                         && ultima >= ZEPHYRUM)
            {
                ((MusicxmlSyllaba*)xar_obtinere(syllabae,
                    (i32)ultima))->extensio = VERUM;
            }
        }
    }
    n->syllabae = (MusicxmlSyllaba*)_tabula(l, syllabae,
        (i32)magnitudo(MusicxmlSyllaba), &n->numerus_syllabarum);
    /* ordo versuum stabilis (insertio) */
    per (i = I; i < n->numerus_syllabarum; i++)
    {
        MusicxmlSyllaba y = n->syllabae[i];

        per (j = i; j > ZEPHYRUM
            && n->syllabae[j - I].versus > y.versus;
             j--)
        {
            n->syllabae[j] = n->syllabae[j - I];
        }
        n->syllabae[j] = y;
    }
}

/* signa notarum (notations): genus et titulus elementi; tied et
 * tuplet praetereuntur (ligatura ex <tie>, tempus ex duratione) */
interior vacuum
_signum_addere (
                  Lector* l,
                     Xar* signa,
     MusicxmlSignumGenus  genus,
      constans XmlCoctus* x)
{
    MusicxmlSignum* g = (MusicxmlSignum*)xar_addere(signa);
            chorda  t;
               s64  v;

    si (g == NIHIL)
    {
        redde;
    }
    memset(g, ZEPHYRUM, magnitudo(MusicxmlSignum));
    g->genus    = genus;
    g->titulus  = xml_titulus(x);
    si (   chorda_aequalis_literis(g->titulus, "other-dynamics")
        && xml_textus_totus(x, l->piscina, &t))
    {
        g->titulus = _praecidere(t);
    }
    si (genus == MUSICXML_SIGNUM_ARCUS)
    {
        g->numerus = I;
        si (_integer(_attributum(x, "number"), &v) && v > ZEPHYRUM)
        {
            g->numerus = (i32)v;
        }
        t           = _attributum(x, "type");
        g->initium  = chorda_aequalis_literis(t, "start");
        g->finis    = chorda_aequalis_literis(t, "stop");
    }
}

interior vacuum
_signa_legere (
                 Lector* l,
     constans XmlCoctus* e,
           MusicxmlNota* n)
{
     constans XmlCoctus* nt;
     constans XmlCoctus* x;
     constans XmlCoctus* y;
                    Xar* signa;

    si (xml_filius(e, "notations") == NIHIL)
    {
        redde;
    }
    signa = xar_creare(l->piscina, (i32)magnitudo(MusicxmlSignum));
    si (signa == NIHIL)
    {
        redde;
    }
    per (nt = xml_filius(e, "notations"); nt != NIHIL;
         nt = xml_frater(nt, "notations"))
    {
        per (x = xml_filius(nt, NIHIL); x != NIHIL;
             x = xml_frater(x, NIHIL))
        {
                         chorda titulus  = xml_titulus(x);
            MusicxmlSignumGenus genus    = MUSICXML_SIGNUM_ALIUD;
                            b32 grex     = FALSUM;

            si (   chorda_aequalis_literis(titulus, "tied")
                || chorda_aequalis_literis(titulus, "tuplet"))
            {
                perge;
            }
            si (chorda_aequalis_literis(titulus, "fermata"))
            {
                genus = MUSICXML_SIGNUM_CORONA;
            }
            alioquin si (chorda_aequalis_literis(titulus, "slur"))
            {
                genus = MUSICXML_SIGNUM_ARCUS;
            }
            alioquin si (   chorda_aequalis_literis(titulus,
                         "arpeggiate")
                         || chorda_aequalis_literis(titulus,
                                "non-arpeggiate"))
            {
                genus = MUSICXML_SIGNUM_ACORDUM_FRACTUM;
            }
            alioquin si (chorda_aequalis_literis(titulus,
                         "articulations"))
            {
                genus  = MUSICXML_SIGNUM_ARTICULATIO;
                grex   = VERUM;
            }
            alioquin si (chorda_aequalis_literis(titulus, "ornaments"))
            {
                genus  = MUSICXML_SIGNUM_ORNAMENTUM;
                grex   = VERUM;
            }
            alioquin si (chorda_aequalis_literis(titulus, "technical"))
            {
                genus  = MUSICXML_SIGNUM_TECHNICUM;
                grex   = VERUM;
            }
            alioquin si (chorda_aequalis_literis(titulus, "dynamics"))
            {
                genus  = MUSICXML_SIGNUM_DYNAMICA;
                grex   = VERUM;
            }
            si (!grex)
            {
                _signum_addere(l, signa, genus, x);
                perge;
            }
            per (y = xml_filius(x, NIHIL); y != NIHIL;
                 y = xml_frater(y, NIHIL))
            {
                _signum_addere(l, signa, genus, y);
            }
        }
    }
    n->signa = (MusicxmlSignum*)_tabula(l, signa,
        (i32)magnitudo(MusicxmlSignum), &n->numerus_signorum);
}

/* VERUM = nota addenda in *n */
interior b32
_notam_legere (
                 Lector* l,
     constans XmlCoctus* e,
           StatusPartis* sp,
         StatusMensurae* sm,
           MusicxmlNota* n)
{
     constans XmlCoctus* f;
                 chorda  t;
                    s64  v;
                    s64  d = ZEPHYRUM;
                    b32  acordum;

    memset(n, ZEPHYRUM, magnitudo(MusicxmlNota));
    (vacuum)xml_locus(e, &n->linea, &n->columna);
    n->gratia  = xml_filius(e, "grace") != NIHIL;
    n->parva   = xml_filius(e, "cue") != NIHIL;
    acordum    = xml_filius(e, "chord") != NIHIL;
    (vacuum)_textus_filii(l, e, "voice", &n->vox);
    si (_integer_filii(l, e, "staff", &v) && v > ZEPHYRUM)
    {
        n->baculum = (i32)v;
    }

    f = xml_filius(e, "pitch");
    si (f != NIHIL)
    {
        n->genus = MUSICXML_NOTA_SONANS;
        si (   !_textus_filii(l, f, "step", &t) || t.mensura != I
            || _gradus((character)t.datum[ZEPHYRUM]) < ZEPHYRUM)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                "pitch sine step legibili (pausa facta)", _vacua());
            n->genus = MUSICXML_NOTA_PAUSA;
        }
        alioquin
        {
            n->littera = (character)t.datum[ZEPHYRUM];
        }
        si (   _textus_filii(l, f, "alter", &t)
            && !_decimalis(t, &n->alteratio))
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, e,
                "alter illegibile: ", t);
            n->alteratio = 0.0;
        }
        si (n->genus == MUSICXML_NOTA_SONANS)
        {
            si (   !_integer_filii(l, f, "octave", &v) || v < ZEPHYRUM
                || v > IX)
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                    "pitch sine octave legibili (pausa facta)",
                    _vacua());
                n->genus    = MUSICXML_NOTA_PAUSA;
                n->littera  = '\0';
            }
            alioquin
            {
                n->octava = (s32)v;
            }
        }
    }
    alioquin si ((f = xml_filius(e, "rest")) != NIHIL)
    {
        n->genus = MUSICXML_NOTA_PAUSA;
        n->mensura_tota = chorda_aequalis_literis(
            _attributum(f, "measure"), "yes");
    }
    alioquin si ((f = xml_filius(e, "unpitched")) != NIHIL)
    {
        n->genus = MUSICXML_NOTA_PERCUSSA;
        si (   _textus_filii(l, f, "display-step", &t) && t.mensura == I
            && _gradus((character)t.datum[ZEPHYRUM]) >= ZEPHYRUM)
        {
            n->littera = (character)t.datum[ZEPHYRUM];
        }
        si (_integer_filii(l, f, "display-octave", &v))
        {
            n->octava = (s32)v;
        }
        si ((f = xml_filius(e, "instrument")) != NIHIL)
        {
            n->instrumentum = _attributum(f, "id");
        }
    }
    alioquin
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
            "nota sine pitch, rest aut unpitched (omissa)", _vacua());
        redde FALSUM;
    }

    si (!n->gratia)
    {
        si (!_duratio(l, e, sp, &d))
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                "nota sine duration (omissa)", _vacua());
            redde FALSUM;
        }
    }
    n->longitudo = d;
    si (n->gratia)
    {
        f = xml_filius(e, "grace");
        n->gratia_obliqua = chorda_aequalis_literis(
            _attributum(f, "slash"), "yes");
        n->furtum_prioris    = _centesimae(_attributum(f,
                                   "steal-time-previous"));
        n->furtum_sequentis  = _centesimae(_attributum(f,
                                   "steal-time-following"));
        si (_integer(_attributum(f, "make-time"), &v) && v > ZEPHYRUM)
        {
            n->tempus_factum = v * sp->factor;
        }
    }
    f = xml_filius(e, "time-modification");
    si (f != NIHIL)
    {
        si (_integer_filii(l, f, "actual-notes", &v) && v > ZEPHYRUM)
        {
            n->ratio_notarum = (i32)v;
        }
        si (_integer_filii(l, f, "normal-notes", &v) && v > ZEPHYRUM)
        {
            n->ratio_spatii = (i32)v;
        }
    }
    _verba_legere(l, e, n);
    _signa_legere(l, e, n);
    _sonantem_computare(sp, l->concentus, n);

    si (   acordum && sm->prior_adest && chorda_aequalis(sm->prior_vox,
            n->vox))
    {
        n->acordum = VERUM;
        n->positio = sm->prior_positio;
    }
    alioquin
    {
        si (acordum)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_ACORDUM_PRIMUM, e,
                "chord in nota prima vocis (nota plana facta)",
                _vacua());
        }
        n->positio         = sm->cursor;
        sm->prior_adest    = VERUM;
        sm->prior_positio  = sm->cursor;
        sm->prior_vox      = n->vox;
        sm->cursor         += d;
    }
    si (n->positio + d > sm->maximum)
    {
        sm->maximum = n->positio + d;
    }

    per (f = xml_filius(e, "tie"); f != NIHIL; f = xml_frater(f, "tie"))
    {
        t = _attributum(f, "type");
        si (chorda_aequalis_literis(t, "start"))
        {
            n->ligatura_initium = VERUM;
        }
        alioquin si (chorda_aequalis_literis(t, "stop"))
        {
            n->ligatura_finis = VERUM;
        }
    }
    _ligaturam_tractare(l, sp, e, n);
    redde VERUM;
}


/* ==================================================
 * Directiones et lineae mensurae
 * ================================================== */

interior MusicxmlDirectio*
_directionem_addere (
                   Lector* l,
                      Xar* directiones,
    MusicxmlDirectioGenus  genus,
                      s64  positio,
       constans XmlCoctus* e,
                   chorda  vox,
                      i32  baculum)
{
    MusicxmlDirectio* d = (MusicxmlDirectio*)xar_addere(directiones);

    si (d == NIHIL)
    {
        redde NIHIL;
    }
    memset(d, ZEPHYRUM, magnitudo(MusicxmlDirectio));
    d->genus    = genus;
    d->positio  = positio;
    d->vox      = vox;
    d->baculum  = baculum;
    (vacuum)xml_locus(e, &d->linea, &d->columna);
    (vacuum)l;
    redde d;
}

/* directio generis dati inter [prima, finem); NIHIL si nulla */
interior MusicxmlDirectio*
_directionem_invenire (
                      Xar* directiones,
                      i32  prima,
    MusicxmlDirectioGenus  genus)
{
    i32 i;

    per (i = prima; i < xar_numerus(directiones); i++)
    {
        MusicxmlDirectio* d = (MusicxmlDirectio*)xar_obtinere(
            directiones, i);

        si (d->genus == genus)
        {
            redde d;
        }
    }
    redde NIHIL;
}

/* sonus (sound): tempus, dynamica, saltus - directionibus eiusdem
 * <direction> (ab indice prima) adiungitur aut novas facit; sonus
 * VERUM (saltus intellectus, non verba sola) */
interior vacuum
_sonum_legere (
                 Lector* l,
     constans XmlCoctus* so,
                    Xar* directiones,
                    i32  prima,
                    s64  positio,
                 chorda  vox,
                    i32  baculum)
{
                  chorda  v;
                     f64  x;
        MusicxmlDirectio* d;

    v = _attributum(so, "tempo");
    si (v.mensura > ZEPHYRUM)
    {
        si (_decimalis(v, &x) && x > 0.0)
        {
            d = _directionem_invenire(directiones, prima,
                MUSICXML_DIRECTIO_TEMPUS);
            si (d == NIHIL)
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_TEMPUS, positio, so, vox,
                    baculum);
            }
            si (d != NIHIL)
            {
                d->valor_adest  = VERUM;
                d->valor        = x;
                d->sonus        = VERUM;
            }
        }
        alioquin
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, so,
                "sound tempo illegibile: ", v);
        }
    }
    v = _attributum(so, "dynamics");
    si (v.mensura > ZEPHYRUM && _decimalis(v, &x))
    {
        d = _directionem_invenire(directiones, prima,
            MUSICXML_DIRECTIO_DYNAMICA);
        si (d == NIHIL)
        {
            d = _directionem_addere(l, directiones,
                MUSICXML_DIRECTIO_DYNAMICA, positio, so, vox, baculum);
        }
        si (d != NIHIL)
        {
            d->valor_adest  = VERUM;
            d->valor        = x;
            d->sonus        = VERUM;
        }
    }
    si (chorda_aequalis_literis(_attributum(so, "dacapo"), "yes"))
    {
        d = _directionem_addere(l, directiones,
            MUSICXML_DIRECTIO_A_CAPITE,
            positio, so, vox, baculum);
        si (d != NIHIL)
        {
            d->sonus = VERUM;
        }
    }
    {
           constans character* attributa[V];
        MusicxmlDirectioGenus  genera[V];
                          b32  iungere[V];
                          i32  i;

        attributa[ZEPHYRUM]  = "dalsegno";
        genera[ZEPHYRUM]     = MUSICXML_DIRECTIO_A_SIGNO;
        iungere[ZEPHYRUM]    = FALSUM;
        attributa[I]         = "tocoda";
        genera[I]            = MUSICXML_DIRECTIO_AD_CAUDAM;
        iungere[I]           = FALSUM;
        attributa[II]        = "fine";
        genera[II]           = MUSICXML_DIRECTIO_FINIS;
        iungere[II]          = FALSUM;
        attributa[III]       = "segno";
        genera[III]          = MUSICXML_DIRECTIO_SIGNUM;
        iungere[III]         = VERUM;
        attributa[IV]        = "coda";
        genera[IV]           = MUSICXML_DIRECTIO_CAUDA;
        iungere[IV]          = VERUM;
        per (i = ZEPHYRUM; i < V; i++)
        {
            si (!xml_attributum(so, attributa[i], &v))
            {
                perge;
            }
            d = iungere[i] ? _directionem_invenire(directiones, prima,
                                 genera[i])
                           : NIHIL;
            si (d == NIHIL)
            {
                d = _directionem_addere(l, directiones, genera[i],
                    positio, so, vox, baculum);
            }
            si (d != NIHIL)
            {
                d->textus  = _praecidere(v);
                d->sonus   = VERUM;
            }
        }
    }
}

/* beat-unit in quartis (puncta: x 1.5, 1.75 ...); 0 si ignota */
interior f64
_unitas_quartarum (
    chorda unitas)
{
    si (chorda_aequalis_literis(unitas, "long"))    redde 16.0;
    si (chorda_aequalis_literis(unitas, "breve"))   redde 8.0;
    si (chorda_aequalis_literis(unitas, "whole"))   redde 4.0;
    si (chorda_aequalis_literis(unitas, "half"))    redde 2.0;
    si (chorda_aequalis_literis(unitas, "quarter")) redde 1.0;
    si (chorda_aequalis_literis(unitas, "eighth"))  redde 0.5;
    si (chorda_aequalis_literis(unitas, "16th"))    redde 0.25;
    si (chorda_aequalis_literis(unitas, "32nd"))    redde 0.125;
    si (chorda_aequalis_literis(unitas, "64th"))    redde 0.0625;
    redde 0.0;
}

interior b32
_directionem_legere (
                 Lector* l,
     constans XmlCoctus* e,
           StatusPartis* sp,
                    s64  cursor,
                    Xar* directiones)
{
     constans XmlCoctus* dt;
     constans XmlCoctus* x;
     constans XmlCoctus* f;
                 chorda  vox;
                 chorda  t;
                    i32  baculum = ZEPHYRUM;
                    s64  positio = cursor;
                    s64  v;
                    f64  q;
                    i32  prima = xar_numerus(directiones);

    (vacuum)_textus_filii(l, e, "voice", &vox);
    si (_integer_filii(l, e, "staff", &v) && v > ZEPHYRUM)
    {
        baculum = (i32)v;
    }
    f = xml_filius(e, "offset");
    si (   f != NIHIL
        && chorda_aequalis_literis(_attributum(f, "sound"), "yes")
        && xml_textus_totus(f, l->piscina, &t) && _decimalis(t, &q))
    {
        positio = cursor + (s64)(q * (f64)sp->factor
                                 + (q < 0.0 ? -0.5 : 0.5));
        si (positio < ZEPHYRUM)
        {
            positio = ZEPHYRUM;
        }
    }
    per (dt = xml_filius(e, "direction-type"); dt != NIHIL;
         dt = xml_frater(dt, "direction-type"))
    {
        per (x = xml_filius(dt, NIHIL); x != NIHIL;
             x = xml_frater(x, NIHIL))
        {
                       chorda  titulus = xml_titulus(x);
             MusicxmlDirectio* d;

            si (chorda_aequalis_literis(titulus, "words"))
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_VERBA, positio, e, vox, baculum);
                si (d != NIHIL && xml_textus_totus(x, l->piscina, &t))
                {
                    d->textus = _praecidere(t);
                }
            }
            alioquin si (chorda_aequalis_literis(titulus, "rehearsal"))
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_EXERCITIUM, positio, e, vox,
                    baculum);
                si (d != NIHIL && xml_textus_totus(x, l->piscina, &t))
                {
                    d->textus = _praecidere(t);
                }
            }
            alioquin si (   chorda_aequalis_literis(titulus, "segno")
                         || chorda_aequalis_literis(titulus, "coda"))
            {
                d = _directionem_addere(l, directiones,
                    chorda_aequalis_literis(titulus, "segno")
                        ? MUSICXML_DIRECTIO_SIGNUM
                        : MUSICXML_DIRECTIO_CAUDA,
                    positio, e, vox, baculum);
                si (d != NIHIL)
                {
                    d->textus = _attributum(x, "id");
                }
            }
            alioquin si (chorda_aequalis_literis(titulus, "dynamics"))
            {
                constans XmlCoctus* y = xml_filius(x, NIHIL);

                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_DYNAMICA, positio, e, vox,
                    baculum);
                si (d != NIHIL && y != NIHIL)
                {
                    d->textus = xml_titulus(y);
                    si (   chorda_aequalis_literis(d->textus,
                               "other-dynamics")
                        && xml_textus_totus(y, l->piscina, &t))
                    {
                        d->textus = _praecidere(t);
                    }
                }
            }
            alioquin si (chorda_aequalis_literis(titulus, "wedge"))
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_CUNEUS, positio, e, vox, baculum);
                si (d == NIHIL)
                {
                    redde FALSUM;
                }
                t           = _attributum(x, "type");
                d->textus   = t;
                d->numerus  = I;
                si (   _integer(_attributum(x, "number"), &v)
                    && v > ZEPHYRUM)
                {
                    d->numerus = (i32)v;
                }
                si (chorda_aequalis_literis(t, "crescendo"))
                {
                    d->cuneus = MUSICXML_CUNEUS_CRESCENDO;
                }
                alioquin si (chorda_aequalis_literis(t, "diminuendo"))
                {
                    d->cuneus = MUSICXML_CUNEUS_DIMINUENDO;
                }
                alioquin si (chorda_aequalis_literis(t, "stop"))
                {
                    d->cuneus = MUSICXML_CUNEUS_FINIS;
                }
                alioquin si (!chorda_aequalis_literis(t, "continue"))
                {
                    d->cuneus = MUSICXML_CUNEUS_IGNOTUS;
                    _vitium(l, MUSICXML_MONITUM, MUSICXML_VALOR_IGNOTUS,
                        x,
                        "wedge type ignotum: ", t);
                }
            }
            alioquin si (chorda_aequalis_literis(titulus, "metronome"))
            {
                 constans XmlCoctus* y;
                                f64  unitas    = 0.0;
                                f64  addendum  = 0.0;
                                i32  unitates  = ZEPHYRUM;

                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_TEMPUS, positio, e, vox, baculum);
                si (d == NIHIL)
                {
                    redde FALSUM;
                }
                per (y = xml_filius(x, NIHIL); y != NIHIL;
                     y = xml_frater(y, NIHIL))
                {
                    si (chorda_aequalis_literis(xml_titulus(y),
                            "beat-unit"))
                    {
                        (vacuum)xml_textus_totus(y, l->piscina, &t);
                        unitas    = _unitas_quartarum(_praecidere(t));
                        addendum  = unitas / 2.0;
                        unitates++;
                    }
                    alioquin si (chorda_aequalis_literis(xml_titulus(y),
                                     "beat-unit-dot"))
                    {
                        unitas    += addendum;
                        addendum  /= 2.0;
                    }
                }
                si (   _textus_filii(l, x, "per-minute", &t)
                    && unitates == I && unitas > 0.0)
                {
                    d->textus = t;
                    si (_decimalis(t, &q) && q > 0.0)
                    {
                        d->valor_adest  = VERUM;
                        d->valor        = q * unitas;
                    }
                }
            }
            alioquin si (chorda_aequalis_literis(titulus, "pedal"))
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_PEDALE, positio, e, vox, baculum);
                si (d != NIHIL)
                {
                    d->textus = _attributum(x, "type");
                }
            }
            alioquin si (chorda_aequalis_literis(titulus,
                         "octave-shift"))
            {
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_OCTAVA, positio, e, vox, baculum);
                si (d != NIHIL)
                {
                    d->textus   = _attributum(x, "type");
                    d->numerus  = VIII;
                    si (   _integer(_attributum(x, "size"), &v)
                        && v > ZEPHYRUM)
                    {
                        d->numerus = (i32)v;
                    }
                }
            }
            alioquin
            {
                /* other-direction et cetera: textus aut titulus */
                d = _directionem_addere(l, directiones,
                    MUSICXML_DIRECTIO_ALIA, positio, e, vox, baculum);
                si (d != NIHIL)
                {
                    d->textus = titulus;
                    si (   xml_textus_totus(x, l->piscina, &t)
                        && _praecidere(t).mensura > ZEPHYRUM)
                    {
                        d->textus = _praecidere(t);
                    }
                }
            }
        }
    }
    f = xml_filius(e, "sound");
    si (f != NIHIL)
    {
        _sonum_legere(l, f, directiones, prima, positio, vox, baculum);
    }
    redde VERUM;
}

interior b32
_lineam_legere (
                     Lector* l,
         constans XmlCoctus* e,
      MusicxmlLineaMensurae* b)
{
     constans XmlCoctus* f;
                 chorda  t;
                    s64  v;
                    Xar* numeri;

    memset(b, ZEPHYRUM, magnitudo(MusicxmlLineaMensurae));
    (vacuum)xml_locus(e, &b->linea, &b->columna);
    t = _attributum(e, "location");
    b->locus = chorda_aequalis_literis(t, "left")
                   ? MUSICXML_LOCUS_SINISTER
                   : chorda_aequalis_literis(t, "middle")
                         ? MUSICXML_LOCUS_MEDIUS : MUSICXML_LOCUS_DEXTER;
    (vacuum)_textus_filii(l, e, "bar-style", &b->stilus);
    b->corona  = xml_filius(e, "fermata") != NIHIL;
    f          = xml_filius(e, "repeat");
    si (f != NIHIL)
    {
        t = _attributum(f, "direction");
        si (chorda_aequalis_literis(t, "forward"))
        {
            b->repetitio = MUSICXML_REPETITIO_PRORSUM;
        }
        alioquin si (chorda_aequalis_literis(t, "backward"))
        {
            b->repetitio = MUSICXML_REPETITIO_RETRORSUM;
        }
        alioquin
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_VALOR_IGNOTUS, f,
                "repeat direction ignota: ", t);
        }
        si (_integer(_attributum(f, "times"), &v) && v > ZEPHYRUM)
        {
            b->vices = (i32)v;
        }
    }
    f = xml_filius(e, "ending");
    si (f != NIHIL)
    {
        i32 i;
        s32 initium = -I;

        t = _attributum(f, "type");
        si (chorda_aequalis_literis(t, "start"))
        {
            b->conclusio = MUSICXML_CONCLUSIO_INITIUM;
        }
        alioquin si (chorda_aequalis_literis(t, "stop"))
        {
            b->conclusio = MUSICXML_CONCLUSIO_FINIS;
        }
        alioquin si (chorda_aequalis_literis(t, "discontinue"))
        {
            b->conclusio = MUSICXML_CONCLUSIO_INTERRUPTA;
        }
        alioquin
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_VALOR_IGNOTUS, f,
                "ending type ignotum: ", t);
        }
        b->conclusionum_textus = _attributum(f, "number");
        numeri = xar_creare(l->piscina, (i32)magnitudo(i32));
        si (numeri == NIHIL)
        {
            redde FALSUM;
        }
        /* "1, 2" aut "1 2": numeri commatibus aut spatiis separati */
        t = b->conclusionum_textus;
        per (i = ZEPHYRUM; i <= t.mensura; i++)
        {
            b32 digitus = i < t.mensura && t.datum[i] >= '0'
                        && t.datum[i] <= '9';

            si (digitus && initium < ZEPHYRUM)
            {
                initium = (s32)i;
            }
            alioquin si (!digitus && initium >= ZEPHYRUM)
            {
                i32* n = (i32*)xar_addere(numeri);

                si (n == NIHIL)
                {
                    redde FALSUM;
                }
                (vacuum)_integer(chorda_sectio(t, (i32)initium, i), &v);
                *n       = (i32)v;
                initium  = -I;
            }
        }
        b->conclusiones = (i32*)_tabula(l, numeri, (i32)magnitudo(i32),
            &b->numerus_conclusionum);
    }
    redde VERUM;
}


/* ==================================================
 * Mensurae et partes
 * ================================================== */

interior b32
_mensuram_legere (
                 Lector* l,
     constans XmlCoctus* me,
     constans XmlCoctus* contentum,
           StatusPartis* sp,
        MusicxmlMensura* m)
{
     constans XmlCoctus* f;
         StatusMensurae  sm;
                    Xar* notae;
                    Xar* attributa;
                    Xar* directiones;
                    Xar* lineae;

    memset(m, ZEPHYRUM, magnitudo(MusicxmlMensura));
    memset(&sm, ZEPHYRUM, magnitudo(StatusMensurae));
    (vacuum)xml_locus(me, &m->linea, &m->columna);
    m->numerus    = _attributum(me, "number");
    m->implicita  = chorda_aequalis_literis(_attributum(me, "implicit"),
                        "yes");
    notae        = xar_creare(l->piscina, (i32)magnitudo(MusicxmlNota));
    attributa    = xar_creare(l->piscina,
                       (i32)magnitudo(MusicxmlAttributa));
    directiones  = xar_creare(l->piscina,
                       (i32)magnitudo(MusicxmlDirectio));
    lineae       = xar_creare(l->piscina,
                       (i32)magnitudo(MusicxmlLineaMensurae));
    si (   notae  == NIHIL || attributa == NIHIL || directiones == NIHIL
        || lineae == NIHIL)
    {
        redde FALSUM;
    }
    /* timewise: pars in mensura absens = mensura vacua */
    per (f = contentum != NIHIL ? xml_filius(contentum, NIHIL) : NIHIL;
         f != NIHIL; f = xml_frater(f, NIHIL))
    {
        chorda titulus = xml_titulus(f);
           s64 d;

        si (chorda_aequalis_literis(titulus, "note"))
        {
            MusicxmlNota n;

            si (_notam_legere(l, f, sp, &sm, &n))
            {
                MusicxmlNota* x = (MusicxmlNota*)xar_addere(notae);

                si (x == NIHIL)
                {
                    redde FALSUM;
                }
                *x = n;
            }
        }
        alioquin si (chorda_aequalis_literis(titulus, "backup"))
        {
            si (!_duratio(l, f, sp, &d))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, f,
                    "backup sine duration (praeteritum)", _vacua());
                perge;
            }
            sm.cursor -= d;
            si (sm.cursor < ZEPHYRUM)
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_REGRESSUS_NIMIUS,
                    f,
                    "backup ante initium mensurae (coercitum)",
                    _vacua());
                sm.cursor = ZEPHYRUM;
            }
            sm.prior_adest = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(titulus, "forward"))
        {
            si (!_duratio(l, f, sp, &d))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, f,
                    "forward sine duration (praeteritum)", _vacua());
                perge;
            }
            sm.cursor += d;
            si (sm.cursor > sm.maximum)
            {
                sm.maximum = sm.cursor;
            }
            sm.prior_adest = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(titulus, "attributes"))
        {
            MusicxmlAttributa* t = (MusicxmlAttributa*)xar_addere(
                attributa);

            si (   t == NIHIL
                || !_attributa_legere(l, f, sm.cursor, sp, t))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(titulus, "direction"))
        {
            si (!_directionem_legere(l, f, sp, sm.cursor, directiones))
            {
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(titulus, "sound"))
        {
            _sonum_legere(l, f, directiones, xar_numerus(directiones),
                sm.cursor, _vacua(), ZEPHYRUM);
        }
        alioquin si (chorda_aequalis_literis(titulus, "barline"))
        {
            MusicxmlLineaMensurae* b = (MusicxmlLineaMensurae*)
                xar_addere(lineae);

            si (b == NIHIL || !_lineam_legere(l, f, b))
            {
                redde FALSUM;
            }
        }
        /* cetera (print, harmony, figured-bass ...) praetereuntur */
    }
    m->directiones = (MusicxmlDirectio*)_tabula(l, directiones,
        (i32)magnitudo(MusicxmlDirectio), &m->numerus_directionum);
    m->lineae = (MusicxmlLineaMensurae*)_tabula(l, lineae,
        (i32)magnitudo(MusicxmlLineaMensurae), &m->numerus_linearum);
    m->longitudo  = sm.maximum;
    m->notae      = (MusicxmlNota*)_tabula(l, notae,
                        (i32)magnitudo(MusicxmlNota),
                        &m->numerus_notarum);
    m->attributa  = (MusicxmlAttributa*)_tabula(l, attributa,
                        (i32)magnitudo(MusicxmlAttributa),
                        &m->numerus_attributorum);
    redde VERUM;
}

interior b32
_partem_legere (
                 Lector* l,
     constans XmlCoctus* pe,
     constans XmlCoctus* radix_temporum,
           MusicxmlPars* p)
{
     constans XmlCoctus* me;
           StatusPartis  sp;
                    Xar* mensurae;
                    i32  i;

    memset(&sp, ZEPHYRUM, magnitudo(StatusPartis));
    sp.factor                  = l->divisiones;
    sp.instrumenta             = p->instrumenta;
    sp.numerus_instrumentorum  = p->numerus_instrumentorum;
    sp.ligaturae  = xar_creare(l->piscina,
                        (i32)magnitudo(LigaturaAperta));
    mensurae      = xar_creare(l->piscina,
                        (i32)magnitudo(MusicxmlMensura));
    si (sp.ligaturae == NIHIL || mensurae == NIHIL)
    {
        redde FALSUM;
    }
    /* partwise: <measure> partis; timewise: <measure> radicis, cum
     * <part> eius id (contentum) */
    per (me = xml_filius(radix_temporum != NIHIL ? radix_temporum : pe,
             "measure");
         me != NIHIL; me = xml_frater(me, "measure"))
    {
           MusicxmlMensura* m = (MusicxmlMensura*)xar_addere(mensurae);
        constans XmlCoctus* contentum = me;

        si (radix_temporum != NIHIL)
        {
            per (contentum = xml_filius(me, "part"); contentum != NIHIL;
                 contentum = xml_frater(contentum, "part"))
            {
                si (chorda_aequalis(_attributum(contentum, "id"),
                    p->id))
                {
                    frange;
                }
            }
        }
        si (m == NIHIL || !_mensuram_legere(l, me, contentum, &sp, m))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(sp.ligaturae); i++)
    {
        LigaturaAperta* a = (LigaturaAperta*)xar_obtinere(sp.ligaturae,
            i);

        _vitium(l, MUSICXML_MONITUM, MUSICXML_LIGATURA_PENDENS,
            a->nodus,
            "tie start sine fine", _vacua());
    }
    p->mensurae = (MusicxmlMensura*)_tabula(l, mensurae,
        (i32)magnitudo(MusicxmlMensura), &p->numerus_mensurarum);
    redde VERUM;
}

/* index partium (part-list): MusicxmlPars sine mensuris, ordine indicis */
nomen structura {
          MusicxmlPars  pars;
    constans XmlCoctus* nodus;
} IndexPartis;

interior b32
_indicem_legere (
                 Lector* l,
     constans XmlCoctus* radix,
                    Xar* index)
{
    constans XmlCoctus* lista = xml_filius(radix, "part-list");
    constans XmlCoctus* sp;

    si (lista == NIHIL)
    {
        redde VERUM;
    }
    per (sp = xml_filius(lista, "score-part"); sp != NIHIL;
         sp = xml_frater(sp, "score-part"))
    {
                 IndexPartis* x = (IndexPartis*)xar_addere(index);
          constans XmlCoctus* f;
                         Xar* instrumenta;

        si (x == NIHIL)
        {
            redde FALSUM;
        }
        memset(x, ZEPHYRUM, magnitudo(IndexPartis));
        x->nodus    = sp;
        x->pars.id  = _attributum(sp, "id");
        (vacuum)_textus_filii(l, sp, "part-name", &x->pars.titulus);
        (vacuum)_textus_filii(l, sp, "part-abbreviation",
            &x->pars.abbreviatio);
        instrumenta = xar_creare(l->piscina,
            (i32)magnitudo(MusicxmlInstrumentum));
        si (instrumenta == NIHIL)
        {
            redde FALSUM;
        }
        per (f = xml_filius(sp, "score-instrument"); f != NIHIL;
             f = xml_frater(f, "score-instrument"))
        {
            MusicxmlInstrumentum* s = (MusicxmlInstrumentum*)xar_addere(
                instrumenta);

            si (s == NIHIL)
            {
                redde FALSUM;
            }
            memset(s, ZEPHYRUM, magnitudo(MusicxmlInstrumentum));
            s->id       = _attributum(f, "id");
            s->volumen  = -1.0;
            (vacuum)_textus_filii(l, f, "instrument-name", &s->titulus);
        }
        per (f = xml_filius(sp, "midi-instrument"); f != NIHIL;
             f = xml_frater(f, "midi-instrument"))
        {
            MusicxmlInstrumentum* s   = NIHIL;
                          chorda  id  = _attributum(f, "id");
                          chorda  t;
                             s64  v;
                             i32  i;

            per (i = ZEPHYRUM; i < xar_numerus(instrumenta); i++)
            {
                MusicxmlInstrumentum* q = (MusicxmlInstrumentum*)
                    xar_obtinere(instrumenta, i);

                si (chorda_aequalis(q->id, id))
                {
                    s = q;
                    frange;
                }
            }
            si (s == NIHIL)
            {
                s = (MusicxmlInstrumentum*)xar_addere(instrumenta);
                si (s == NIHIL)
                {
                    redde FALSUM;
                }
                memset(s, ZEPHYRUM, magnitudo(MusicxmlInstrumentum));
                s->id       = id;
                s->volumen  = -1.0;
            }
            si (_integer_filii(l, f, "midi-channel", &v))
            {
                s->canalis = (i32)v;
            }
            si (_integer_filii(l, f, "midi-program", &v))
            {
                s->programma = (i32)v;
            }
            si (_integer_filii(l, f, "midi-unpitched", &v))
            {
                s->percussio = (i32)v;
            }
            si (   _textus_filii(l, f, "volume", &t)
                && !_decimalis(t, &s->volumen))
            {
                s->volumen = -1.0;
            }
            si (   _textus_filii(l, f, "pan", &t)
                && !_decimalis(t, &s->positio))
            {
                s->positio = 0.0;
            }
        }
        x->pars.instrumenta = (MusicxmlInstrumentum*)_tabula(l,
            instrumenta, (i32)magnitudo(MusicxmlInstrumentum),
            &x->pars.numerus_instrumentorum);
    }
    redde VERUM;
}

/* elementum partis: partwise <part>; timewise id quaeque ordine primae
 * apparitionis (nodus = <part> primus eius) */
nomen structura {
                 chorda  id;
     constans XmlCoctus* nodus;
                    b32  usus;
} ElementumPartis;

interior b32
_elementa_partium (
     constans XmlCoctus* radix,
                    b32  temporum,
                    Xar* elementa)
{
     constans XmlCoctus* me;
     constans XmlCoctus* pe;
                    i32  i;

    per (me = temporum ? xml_filius(radix, "measure") : radix; me
        != NIHIL;
         me = temporum ? xml_frater(me, "measure") : NIHIL)
    {
        per (pe = xml_filius(me, "part"); pe != NIHIL;
             pe = xml_frater(pe, "part"))
        {
                     chorda  id = _attributum(pe, "id");
            ElementumPartis* x;
                        b32  iteratum = FALSUM;

            per (i = ZEPHYRUM; temporum
                && i < xar_numerus(elementa); i++)
            {
                si (chorda_aequalis(((ElementumPartis*)xar_obtinere(
                        elementa, i))->id, id))
                {
                    iteratum = VERUM;
                    frange;
                }
            }
            si (iteratum)
            {
                perge;
            }
            x = (ElementumPartis*)xar_addere(elementa);
            si (x == NIHIL)
            {
                redde FALSUM;
            }
            x->id     = id;
            x->nodus  = pe;
            x->usus   = FALSUM;
        }
    }
    redde VERUM;
}

/* partes ordine indicis; partes sine indice post eas, ordine
 * documenti (index partium discors - monitum utrimque) */
interior b32
_partes_legere (
                 Lector* l,
     constans XmlCoctus* radix,
                    b32  temporum,
           MusicxmlOpus* opus)
{
     Xar* index;
     Xar* partes;
     Xar* elementa;
     i32  i;
     i32  k;

    index     = xar_creare(l->piscina, (i32)magnitudo(IndexPartis));
    partes    = xar_creare(l->piscina, (i32)magnitudo(MusicxmlPars));
    elementa  = xar_creare(l->piscina, (i32)magnitudo(ElementumPartis));
    si (   index == NIHIL || partes == NIHIL || elementa == NIHIL
        || !_indicem_legere(l, radix, index)
        || !_elementa_partium(radix, temporum, elementa))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(index); i++)
    {
         IndexPartis* x        = (IndexPartis*)xar_obtinere(index, i);
                 b32  inventa  = FALSUM;

        per (k = ZEPHYRUM; k < xar_numerus(elementa); k++)
        {
            ElementumPartis* e = (ElementumPartis*)xar_obtinere(
                elementa, k);
            MusicxmlPars* p;

            si (e->usus || !chorda_aequalis(e->id, x->pars.id))
            {
                perge;
            }
            p = (MusicxmlPars*)xar_addere(partes);
            si (p == NIHIL)
            {
                redde FALSUM;
            }
            *p       = x->pars;
            e->usus  = VERUM;
            inventa  = VERUM;
            si (!_partem_legere(l, e->nodus, temporum ? radix : NIHIL,
                p))
            {
                redde FALSUM;
            }
            frange;
        }
        si (!inventa)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_PARS_DISCORS,
                x->nodus,
                "pars in indice sine <part> (omissa): ", x->pars.id);
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(elementa); k++)
    {
        ElementumPartis* e = (ElementumPartis*)xar_obtinere(elementa,
            k);
           MusicxmlPars* p;

        si (e->usus)
        {
            perge;
        }
        p = (MusicxmlPars*)xar_addere(partes);
        si (p == NIHIL)
        {
            redde FALSUM;
        }
        memset(p, ZEPHYRUM, magnitudo(MusicxmlPars));
        p->id       = e->id;
        p->titulus  = e->id;
        _vitium(l, MUSICXML_MONITUM, MUSICXML_PARS_DISCORS, e->nodus,
            "<part> sine indice (servata): ", e->id);
        si (!_partem_legere(l, e->nodus, temporum ? radix : NIHIL, p))
        {
            redde FALSUM;
        }
    }
    opus->partes = (MusicxmlPars*)_tabula(l, partes,
        (i32)magnitudo(MusicxmlPars), &opus->numerus_partium);
    redde VERUM;
}

/* M15: initium[k+1] = initium[k] + maxima longitudo mensurae k */
interior b32
_lineam_temporis_ponere (
           Lector* l,
     MusicxmlOpus* opus)
{
    i32 n = ZEPHYRUM;
    i32 p;
    i32 k;
    s64 initium = ZEPHYRUM;

    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        si (opus->partes[p].numerus_mensurarum > n)
        {
            n = opus->partes[p].numerus_mensurarum;
        }
    }
    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    opus->initia = (s64*)piscina_allocare(l->piscina,
        (memoriae_index)(n * (i32)magnitudo(s64)));
    si (opus->initia == NIHIL)
    {
        redde FALSUM;
    }
    opus->numerus_mensurarum = n;
    per (k = ZEPHYRUM; k < n; k++)
    {
        s64 maxima = ZEPHYRUM;

        per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
        {
            si (   k < opus->partes[p].numerus_mensurarum
                && opus->partes[p].mensurae[k].longitudo > maxima)
            {
                maxima = opus->partes[p].mensurae[k].longitudo;
            }
        }
        opus->initia[k] = initium;
        per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
        {
            MusicxmlMensura* m;

            si (k >= opus->partes[p].numerus_mensurarum)
            {
                perge;
            }
            m           = &opus->partes[p].mensurae[k];
            m->initium  = initium;
            si (m->longitudo < maxima)
            {
                ChordaAedificator* a = chorda_aedificator_creare(
                    l->piscina, LXIV);

                si (a == NIHIL)
                {
                    redde FALSUM;
                }
                chorda_aedificator_appendere_chorda(a,
                    opus->partes[p].id);
                chorda_aedificator_appendere_literis(a, ", mensura ");
                chorda_aedificator_appendere_chorda(a, m->numerus);
                _vitium_locatum(l, MUSICXML_MONITUM,
                    MUSICXML_MENSURA_BREVIOR, m->linea, m->columna,
                    "mensura brevior quam partium ceterarum (tempore "
                    "pausae impleta): pars ",
                    chorda_aedificator_finire(a));
            }
        }
        initium += maxima;
    }
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        si (opus->partes[p].numerus_mensurarum < n)
        {
            _vitium_locatum(l, MUSICXML_MONITUM,
                MUSICXML_MENSURA_BREVIOR, ZEPHYRUM, ZEPHYRUM,
                "pars mensuris paucioribus quam ceterae: ",
                opus->partes[p].id);
        }
    }
    opus->longitudo = initium;
    redde VERUM;
}


/* ==================================================
 * Documentum
 * ================================================== */

/* UTF-16 aut UTF-32: BOM (FE FF, FF FE) aut octetus nullus in duobus
 * primis (M13: solum UTF-8) */
interior b32
_non_utf8 (
    chorda datum)
{
    insignatus character a;
    insignatus character b;

    si (datum.mensura < II)
    {
        redde FALSUM;
    }
    a = (insignatus character)datum.datum[ZEPHYRUM];
    b = (insignatus character)datum.datum[I];
    redde (a == 0xFE && b == 0xFF) || (a == 0xFF && b == 0xFE)
        || a == ZEPHYRUM || b == ZEPHYRUM;
}

/* 'PK\3\4': arca ZIP (.mxl) */
interior b32
_arca_signum (
    chorda datum)
{
    redde datum.mensura >= IV && datum.datum[ZEPHYRUM] == 'P'
        && datum.datum[I] == 'K' && datum.datum[II] == III
        && datum.datum[III] == IV;
}

interior vacuum
_identificationem_legere (
                 Lector* l,
     constans XmlCoctus* radix,
           MusicxmlOpus* opus)
{
    constans XmlCoctus* f;
    constans XmlCoctus* c;

    f = xml_filius(radix, "work");
    si (f != NIHIL)
    {
        (vacuum)_textus_filii(l, f, "work-title",
            &opus->titulus_operis);
        (vacuum)_textus_filii(l, f, "work-number",
            &opus->numerus_operis);
    }
    (vacuum)_textus_filii(l, radix, "movement-title",
        &opus->titulus_motus);
    (vacuum)_textus_filii(l, radix, "movement-number",
        &opus->numerus_motus);
    f = xml_filius(radix, "identification");
    si (f == NIHIL)
    {
        redde;
    }
    per (c = xml_filius(f, "creator"); c != NIHIL;
         c = xml_frater(c, "creator"))
    {
        chorda genus = _attributum(c, "type");
        chorda t;

        si (!xml_textus_totus(c, l->piscina, &t))
        {
            perge;
        }
        t = _praecidere(t);
        si (   chorda_aequalis_literis(genus, "composer")
            && opus->compositor.mensura == ZEPHYRUM)
        {
            opus->compositor = t;
        }
        alioquin si (   (chorda_aequalis_literis(genus, "lyricist")
                      || chorda_aequalis_literis(genus, "poet"))
                     && opus->poeta.mensura == ZEPHYRUM)
        {
            opus->poeta = t;
        }
    }
    (vacuum)_textus_filii(l, f, "rights", &opus->ius);
}

interior b32
_documentum_legere (
          Lector* l,
          chorda  datum,
    MusicxmlOpus* opus)
{
           XmlDocumentum* d;
      constans XmlCoctus* radix;
                  chorda  titulus;

    si (_non_utf8(datum))
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_NON_UTF8, ZEPHYRUM,
            ZEPHYRUM, "documentum non UTF-8 (UTF-16?) - solum UTF-8 "
            "legitur", _vacua());
        redde VERUM;
    }
    d = xml_documentum_legere(datum, l->piscina);
    si (d == NIHIL)
    {
        redde FALSUM;
    }
    si (!xml_documentum_successus(d))
    {
        Xar* dg = xml_documentum_diagnostica(d);
        constans MateriaDiagnosticum* m = NIHIL;

        si (dg != NIHIL && xar_numerus(dg) > ZEPHYRUM)
        {
            m = (constans MateriaDiagnosticum*)xar_obtinere(dg,
                ZEPHYRUM);
        }
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_XML_MALUM,
            m != NIHIL && m->tractus.initium >= ZEPHYRUM
                ? m->tractus.linea : ZEPHYRUM,
            m != NIHIL && m->tractus.initium >= ZEPHYRUM
                ? m->tractus.columna : ZEPHYRUM,
            "XML non bene formatum: ",
            m != NIHIL && m->causa != NIHIL
                ? chorda_ex_literis(m->causa, l->piscina)
                : chorda_ex_literis("documentum vacuum", l->piscina));
        redde VERUM;
    }
    radix    = xml_documentum_radix(d);
    titulus  = xml_titulus(radix);
    si (chorda_aequalis_literis(titulus, "score-partwise"))
    {
        opus->forma = MUSICXML_FORMA_PARTIUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "opus"))
    {
        _vitium(l, MUSICXML_VITIUM, MUSICXML_OPUS_RADIX, radix,
            "radix <opus> (collectio partiturarum) non legitur",
            _vacua());
        redde VERUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "score-timewise"))
    {
        opus->forma = MUSICXML_FORMA_TEMPORUM;
    }
    alioquin
    {
        _vitium(l, MUSICXML_VITIUM, MUSICXML_NON_MUSICXML, radix,
            "radix nec score-partwise nec score-timewise: ", titulus);
        redde VERUM;
    }
    opus->versio     = _attributum(radix, "version");
    opus->concentus  = xml_filius(xml_filius(radix, "defaults"),
                           "concert-score") != NIHIL;
    l->concentus     = opus->concentus;
    _identificationem_legere(l, radix, opus);
    si (!_divisiones_colligere(l, radix))
    {
        redde VERUM;
    }
    opus->divisiones = l->divisiones;
    si (!_partes_legere(l, radix,
            opus->forma == MUSICXML_FORMA_TEMPORUM, opus))
    {
        redde FALSUM;
    }
    si (opus->numerus_partium == ZEPHYRUM)
    {
        _vitium(l, MUSICXML_VITIUM, MUSICXML_SINE_PARTIBUS, radix,
            "partitura sine partibus", _vacua());
        redde VERUM;
    }
    redde _lineam_temporis_ponere(l, opus);
}


#define CONTINENS "META-INF/container.xml"

/* .mxl: container.xml -> rootfile PRIMUS -> _documentum_legere. Vitia
 * arcae sine sede (linea 0), nomen introitus in causa. */
interior b32
_arcam_legere (
          Lector* l,
          chorda  datum,
    MusicxmlOpus* opus)
{
                       Sarcina* s;
     constans SarcinaIntroitus* e;
                        chorda  vitium;
                        chorda  textus;
                XmlDocumentum* d;
          constans XmlCoctus* r;
                       chorda  via;

    vitium  = _vacua();
    s       = sarcina_aperire(datum, l->piscina, &vitium);
    si (s == NIHIL)
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, "arca ZIP mala: ", vitium);
        redde VERUM;
    }
    e = sarcina_invenire(s, chorda_ex_literis(CONTINENS, l->piscina));
    si (   e == NIHIL
        || !sarcina_legere(s, e, l->piscina, &textus, &vitium))
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, CONTINENS " deest aut non legitur: ", vitium);
        redde VERUM;
    }
    d = xml_documentum_legere(textus, l->piscina);
    si (d == NIHIL)
    {
        redde FALSUM;
    }
    r = xml_documentum_radix(d);
    si (!xml_documentum_successus(d) || r == NIHIL)
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, CONTINENS " non bene formatum", _vacua());
        redde VERUM;
    }
    r = xml_filius(xml_filius(r, "rootfiles"), "rootfile");
    si (   r           == NIHIL || !xml_attributum(r, "full-path", &via)
        || via.mensura == ZEPHYRUM)
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, CONTINENS " sine rootfile full-path", _vacua());
        redde VERUM;
    }
    e = sarcina_invenire(s, via);
    si (   e == NIHIL
        || !sarcina_legere(s, e, l->piscina, &textus, &vitium))
    {
        ChordaAedificator* a = chorda_aedificator_creare(l->piscina,
            LXIV);

        si (a == NIHIL)
        {
            redde FALSUM;
        }
        chorda_aedificator_appendere_chorda(a, via);
        si (e != NIHIL && vitium.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " - ");
            chorda_aedificator_appendere_chorda(a, vitium);
        }
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, "radix arcae abest aut non legitur: ",
            chorda_aedificator_finire(a));
        redde VERUM;
    }
    si (_arca_signum(textus))
    {
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM,
            ZEPHYRUM, "arca in arca non legitur: ", via);
        redde VERUM;
    }
    redde _documentum_legere(l, textus, opus);
}


/* ==================================================
 * API
 * ================================================== */

MusicxmlOpus*
musicxml_legere (
      chorda  datum,
     Piscina* piscina)
{
          Lector  l;
    MusicxmlOpus* opus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    opus = (MusicxmlOpus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MusicxmlOpus));
    si (opus == NIHIL)
    {
        redde NIHIL;
    }
    memset(opus, ZEPHYRUM, magnitudo(MusicxmlOpus));
    l.piscina     = piscina;
    l.divisiones  = I;
    l.concentus   = FALSUM;
    l.vitia       = xar_creare(piscina, (i32)magnitudo(MusicxmlVitium));
    si (l.vitia == NIHIL)
    {
        redde NIHIL;
    }
    opus->divisiones = I;
    si (_arca_signum(datum) ? !_arcam_legere(&l, datum, opus)
                            : !_documentum_legere(&l, datum, opus))
    {
        redde NIHIL;
    }
    opus->vitia = (MusicxmlVitium*)_tabula(&l, l.vitia,
        (i32)magnitudo(MusicxmlVitium), &opus->numerus_vitiorum);
    redde opus;
}

b32
musicxml_successus (
    constans MusicxmlOpus* opus)
{
    i32 i;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < opus->numerus_vitiorum; i++)
    {
        si (opus->vitia[i].gravitas == MUSICXML_VITIUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_s64_scribere (
    ChordaAedificator* a,
                  s64  n)
{
    character cifrae[XXIV];
          i32 k      = ZEPHYRUM;
          b32 minus  = n < ZEPHYRUM;

    si (minus)
    {
        n = -n;
    }
    fac
    {
        cifrae[k++]  = (character)('0' + (integer)(n % X));
        n            /= X;
    }
    dum (n > ZEPHYRUM && k < XXIII);
    si (minus)
    {
        chorda_aedificator_appendere_character(a, '-');
    }
    dum (k > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, cifrae[--k]);
    }
}

interior vacuum
_vitia_lineis (
              ChordaAedificator* a,
        constans MusicxmlVitium* vitia,
                            i32  numerus,
             constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(a, via);
        chorda_aedificator_appendere_character(a, ':');
        si (vitia[i].linea > ZEPHYRUM)
        {
            chorda_aedificator_appendere_i32(a, vitia[i].linea);
            chorda_aedificator_appendere_character(a, ':');
            chorda_aedificator_appendere_i32(a, vitia[i].columna);
            chorda_aedificator_appendere_character(a, ':');
        }
        chorda_aedificator_appendere_literis(a,
            vitia[i].gravitas == MUSICXML_VITIUM ? " vitium: "
                                                 : " monitum: ");
        chorda_aedificator_appendere_chorda(a, vitia[i].causa);
        chorda_aedificator_appendere_character(a, '\n');
    }
}

b32
musicxml_vitia_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
       constans character* via,
        ChordaAedificator* exitus)
{
    si (opus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    _vitia_lineis(exitus, opus->vitia, opus->numerus_vitiorum,
        via != NIHIL ? via : "");
    si (ordo != NIHIL)
    {
        _vitia_lineis(exitus, ordo->vitia, ordo->numerus_vitiorum,
            via != NIHIL ? via : "");
    }
    redde VERUM;
}


/* ==================================================
 * Tabula canonica (oraculum)
 * ================================================== */

nomen structura {
                      i32  pars;
                      s64  initium;
                      s64  longitudo;
    constans MusicxmlNota* nota;
                      b32  aperta;      /* ligatura initium pendens */
} LineaTabulae;

interior integer
_lineas_conferre (
    constans vacuum* a,
    constans vacuum* b)
{
    constans LineaTabulae* x = (constans LineaTabulae*)a;
    constans LineaTabulae* y = (constans LineaTabulae*)b;

    si (x->pars != y->pars)
    {
        redde x->pars < y->pars ? -I : I;
    }
    si (x->initium != y->initium)
    {
        redde x->initium < y->initium ? -I : I;
    }
    si (x->nota->nota != y->nota->nota)
    {
        redde x->nota->nota < y->nota->nota ? -I : I;
    }
    redde ZEPHYRUM;
}

/* quartae ut fractio reducta: "3/2", "1", "0" */
interior vacuum
_quartas_scribere (
    ChordaAedificator* a,
                  s64  v,
                  s64  divisiones)
{
    s64 g = _divisor_communis(v, divisiones);

    si (v == ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '0');
        redde;
    }
    _s64_scribere(a, v / g);
    si (divisiones / g != I)
    {
        chorda_aedificator_appendere_character(a, '/');
        _s64_scribere(a, divisiones / g);
    }
}

/* alteratio: integra sine puncto, aliter decimalis sine zephyris */
interior vacuum
_alterationem_scribere (
    ChordaAedificator* a,
                  f64  x)
{
          s64 scala = (s64)X * X * X * X;
          s64 v;
          s64 fractio;
          s32 k;
    character cifrae[V];

    si (x < 0.0)
    {
        chorda_aedificator_appendere_character(a, '-');
        x = -x;
    }
    v = (s64)(x * (f64)scala + 0.5);
    _s64_scribere(a, v / scala);
    fractio = v % scala;
    si (fractio == ZEPHYRUM)
    {
        redde;
    }
    per (k = III; k >= ZEPHYRUM; k--)
    {
        cifrae[k]  = (character)('0' + (integer)(fractio % X));
        fractio    /= X;
    }
    k = IV;
    dum (k > ZEPHYRUM && cifrae[k - I] == '0')
    {
        k--;
    }
    chorda_aedificator_appendere_character(a, '.');
    per (fractio = ZEPHYRUM; fractio < (s64)k; fractio++)
    {
        chorda_aedificator_appendere_character(a, cifrae[fractio]);
    }
}

interior constans character*
_locus_syllabae (
    MusicxmlLocusSyllabae locus)
{
    commutatio (locus)
    {
        casus MUSICXML_SYLLABA_SINGULA: redde "single";
        casus MUSICXML_SYLLABA_INITIUM: redde "begin";
        casus MUSICXML_SYLLABA_MEDIA:   redde "middle";
        casus MUSICXML_SYLLABA_FINIS:   redde "end";
        ordinarius:                     redde "?";
    }
}

interior vacuum
_textum_tabulae (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        chorda_aedificator_appendere_character(a,
            (c == '\t' || c == '\n' || c == '\r'
                || c == '|') ? ' ' : c);
    }
}

b32
musicxml_tabulam_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
        ChordaAedificator* exitus)
{
             Piscina* piscina;
                 Xar* lineae;
        LineaTabulae* tabula;
                 i32  numerus;
                 i32  p;
                 i32  v;
                 i32  i;
                 i32  numerus_visitationum;

    si (opus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("musicxml_tabula",
        (memoriae_index)(IV * M));
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    lineae = xar_creare(piscina, (i32)magnitudo(LineaTabulae));
    si (lineae == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    numerus_visitationum = ordo != NIHIL ? ordo->numerus
                                         : opus->numerus_mensurarum;
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
         constans MusicxmlPars* pars   = &opus->partes[p];
                           i32  prima  = xar_numerus(lineae);

        per (v = ZEPHYRUM; v < numerus_visitationum; v++)
        {
            i32 k       = ordo
                != NIHIL ? ordo->visitationes[v].mensura : v;
            s64 initium = ordo != NIHIL ? ordo->visitationes[v].initium
                                        : opus->initia[v];
            constans MusicxmlMensura* m;

            si (k < ZEPHYRUM || k >= pars->numerus_mensurarum)
            {
                perge;
            }
            m = &pars->mensurae[k];
            per (i = ZEPHYRUM; i < m->numerus_notarum; i++)
            {
                 constans MusicxmlNota* n      = &m->notae[i];
                                   s64  ortus  = initium + n->positio;
                          LineaTabulae* x      = NIHIL;
                                   i32  j;

                si (n->genus == MUSICXML_NOTA_PAUSA)
                {
                    perge;
                }
                si (n->ligatura_finis)
                {
                    per (j = prima; j < xar_numerus(lineae); j++)
                    {
                        LineaTabulae* q = (LineaTabulae*)xar_obtinere(
                            lineae, j);

                        si (   q->aperta && q->nota->nota == n->nota
                            && chorda_aequalis(q->nota->vox, n->vox)
                            && q->initium + q->longitudo == ortus)
                        {
                            x = q;
                            frange;
                        }
                    }
                }
                si (x != NIHIL)
                {
                    x->longitudo  += n->longitudo;
                    x->aperta     = n->ligatura_initium;
                    perge;
                }
                x = (LineaTabulae*)xar_addere(lineae);
                si (x == NIHIL)
                {
                    piscina_destruere(piscina);
                    redde FALSUM;
                }
                x->pars       = p;
                x->initium    = ortus;
                x->longitudo  = n->longitudo;
                x->nota       = n;
                x->aperta     = n->ligatura_initium;
            }
        }
    }
    tabula = (LineaTabulae*)piscina_allocare(piscina,
        (memoriae_index)((xar_numerus(lineae) + I)
            * (i32)magnitudo(LineaTabulae)));
    si (tabula == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    numerus = xar_copiare_ad_tabulam(lineae, tabula, ZEPHYRUM,
        xar_numerus(lineae));
    qsort(tabula, (size_t)numerus, magnitudo(LineaTabulae),
        _lineas_conferre);

    chorda_aedificator_appendere_literis(exitus,
        "DATA musicxml tabula 1\n"
        "# pars\tinitium\tlongitudo\tmidi\tlittera\talteratio\tgratia"
        "\tversus\n");
    per (i = ZEPHYRUM; i < numerus; i++)
    {
         constans LineaTabulae* x = &tabula[i];
         constans MusicxmlNota* n = x->nota;
                           i32  s;

        chorda_aedificator_appendere_chorda(exitus,
            opus->partes[x->pars].id);
        chorda_aedificator_appendere_character(exitus, '\t');
        _quartas_scribere(exitus, x->initium, opus->divisiones);
        chorda_aedificator_appendere_character(exitus, '\t');
        _quartas_scribere(exitus, x->longitudo, opus->divisiones);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, n->nota);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_character(exitus,
            n->littera_sonans != '\0' ? n->littera_sonans : '-');
        chorda_aedificator_appendere_character(exitus, '\t');
        _alterationem_scribere(exitus, n->alteratio_sonans);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_character(exitus,
            n->gratia ? '1' : '0');
        chorda_aedificator_appendere_character(exitus, '\t');
        si (n->numerus_syllabarum == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(exitus, '-');
        }
        per (s = ZEPHYRUM; s < n->numerus_syllabarum; s++)
        {
            constans MusicxmlSyllaba* y = &n->syllabae[s];

            si (s > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(exitus, '|');
            }
            chorda_aedificator_appendere_i32(exitus, y->versus);
            chorda_aedificator_appendere_character(exitus, ':');
            chorda_aedificator_appendere_literis(exitus,
                _locus_syllabae(y->locus));
            chorda_aedificator_appendere_character(exitus, ':');
            _textum_tabulae(exitus, y->textus);
        }
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    piscina_destruere(piscina);
    redde VERUM;
}
