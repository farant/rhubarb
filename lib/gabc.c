/* gabc.c - lector gabc (caput include/gabc.h)
 *
 * Scrutator manu scriptus super octetos (gabc non XML est). Tres
 * gradus: (1) caput usque ad lineam '%%'; (2) corpus: textus usque ad
 * '(', notae usque ad ')' -> syllabae cum elementis planis, notae in
 * tabula una; (3) gradus resolvuntur, clave in vigore per ordinem
 * fontis. Grammatica notarum = lexicon Gregorii
 * (gabc-notes-determination.l): omnia signa eius hic consumuntur.
 *
 * Notae cum formis, signis rhythmicis et signis supra notam (vv vvv VV
 * VVV ss sss notas iterant), neumae, claves (cb), divisiones,
 * alterationes, spatia, fractiones, custodes; signa quae non exemplantur
 * ('[x:...]', nabc post '|', '{' '}', '[[' ']]', '@[' ']') ut signa
 * opaca cum textu fontis (G5). Spatium Gregorii inseparabile ('!/' '!//'
 * '! ' '!/[n]') = duo elementa, nullum deinde spatium; '!/0' '!/!' =
 * spatium solum (Gregorius '!' ibi neglegit).
 *
 * TEXTUS SYLLABAE (lexicon Gregorii gabc-score-determination.l): '$'
 * characterem sequentem effugit; '%' commentarium; linea nova (cum
 * spatiis sequentibus) = spatium unum; '~' = U+203F. Signa i b ul c sc
 * tt per syllabas manent donec clauduntur (Gregorius idem), e in fine
 * syllabae clauditur. sp -> UTF-8 ex tabula gregoriotex-symbols.tex; v
 * et alt crudi usque ad signum claudens (etiam '(' intra). Cursus
 * textus = octeti eiusdem generis et stili; chorda in fontem monstrat
 * si octeti fontis contigui sunt, aliter in piscina copiatur.
 *
 * AMBITUS ALTERATIONIS (regula Solesmensis, Gregorius nullam habet):
 * signum in positione P notas sequentes in P SOLA (non octavam) alterat
 * usque ad verbum novum (syllaba cum verbum_incipit), divisionem,
 * clavem, aut signum aliud in P. Clavis cb/fb: si (gradus VI modulo
 * VII) bemolle ubi signum nullum valet. Ficta r6-r8 notam solam.
 */

#include "gabc.h"
#include "xar.h"
#include "utf8.h"
#include <string.h>

/* nulla clavis ante notas: c3 (Gregorii 'initial-key' vetus) - do in
 * positione VII (h) */
#define DO_ORDINARIUM ((s32)VII)
#define POSITIO_P     ((s32)XIV)
#define POSITIONES    XV

/* signum nullum in positione (ambitus alterationis) */
#define ALTERATIO_NULLA ((s32)C)


/* ==================================================
 * Lector
 * ================================================== */

nomen structura {
       Piscina* piscina;
            i8* datum;
           i32  mensura;
    GabcCantus* cantus;
           Xar* initia;          /* i32: initium cuiusque lineae */
           Xar* vitia;           /* GabcVitium */
           Xar* capita;          /* GabcCaput */
           Xar* syllabae;        /* GabcSyllaba */
           Xar* notae;           /* GabcNota */
           Xar* elementa;        /* GabcElementum: syllabae currentis */
           Xar* textus;          /* GabcTextus: syllabae currentis */
           Xar* octeti;          /* i8: cursus textus currens */
GabcTextusGenus cursus_genus;
           i32  cursus_stilus;
           i32  cursus_fons;     /* offset fontis octeti primi */
           b32  cursus_purus;    /* octeti = fons contiguus */
           i32  cantati;         /* octeti cantati syllabae currentis */
           i32  stilus;          /* GABC_STILUS_* aperti (per syllabas) */
           i32  stili_loci[VII]; /* offset signi aperientis per bit */
           s32  sine_fine_v;     /* nullum signum v claudens ab hoc */
           s32  sine_fine_alt;
           b32  deficit;         /* memoria defecit */
} Lector;

interior b32
_spatium_est (
    i8 ch)
{
    redde ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

interior chorda
_sectio (
    constans Lector* l,
                i32  a,
                i32  b)
{
    chorda ch;

    ch.mensura  = b - a;
    ch.datum    = l->datum + a;
    redde ch;
}

interior b32
_aequat (
                 chorda  ch,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde ch.mensura == n && (n == ZEPHYRUM
        || memcmp(ch.datum, literae, (size_t)n) == ZEPHYRUM);
}

/* offset -> linea, columna (I-basatae) per quaestionem binariam */
interior vacuum
_locus (
    constans Lector* l,
                i32  offset,
                i32* linea,
                i32* columna)
{
    i32 sinister  = ZEPHYRUM;
    i32 dexter    = xar_numerus(l->initia) - I;
    i32 medius;

    dum (sinister < dexter)
    {
        medius = (sinister + dexter + I) / II;
        si (*(i32*)xar_obtinere(l->initia, medius) <= offset)
        {
            sinister = medius;
        }
        alioquin
        {
            dexter = medius - I;
        }
    }
    *linea    = sinister + I;
    *columna  = offset - *(i32*)xar_obtinere(l->initia, sinister) + I;
}

interior vacuum
_vitium (
                  Lector* l,
            GabcGravitas  gravitas,
         GabcVitiumGenus  genus,
                     i32  offset,
      constans character* causa)
{
    GabcVitium* v = (GabcVitium*)xar_addere(l->vitia);

    si (v == NIHIL)
    {
        l->deficit = VERUM;
        redde;
    }
    v->gravitas  = gravitas;
    v->genus     = genus;
    _locus(l, offset, &v->linea, &v->columna);
    v->causa = chorda_ex_literis(causa, l->piscina);
}

/* Xar -> tabula in piscina; NIHIL si vacua */
interior vacuum*
_tabula (
              Lector* l,
        constans Xar* x,
                 i32  magnitudo_elementi,
                 i32* numerus)
{
    vacuum* t;
       i32  n = xar_numerus(x);

    *numerus = ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    t = piscina_allocare(l->piscina,
            (memoriae_index)(n * magnitudo_elementi));
    si (t == NIHIL)
    {
        l->deficit = VERUM;
        redde NIHIL;
    }
    (vacuum)xar_copiare_ad_tabulam(x, t, ZEPHYRUM, n);
    *numerus = n;
    redde t;
}


/* ==================================================
 * Lineae
 * ================================================== */

interior i32
_finis_lineae (
    constans Lector* l,
                i32  i)
{
    dum (i < l->mensura && l->datum[i] != '\n')
    {
        i++;
    }
    redde i;
}

interior i32
_sinistram_praecidere (
    constans Lector* l,
                i32  a,
                i32  b)
{
    dum (a < b && _spatium_est(l->datum[a]))
    {
        a++;
    }
    redde a;
}

interior i32
_dextram_praecidere (
    constans Lector* l,
                i32  a,
                i32  b)
{
    dum (b > a && _spatium_est(l->datum[b - I]))
    {
        b--;
    }
    redde b;
}

/* finis lineae ante '%' commentarii */
interior i32
_sine_commentario (
    constans Lector* l,
                i32  a,
                i32  b)
{
    i32 i;

    per (i = a; i < b; i++)
    {
        si (l->datum[i] == '%')
        {
            redde i;
        }
    }
    redde b;
}

/* linea quae (spatiis demptis) est '%%'; -1 si nulla. *post = initium
 * lineae sequentis */
interior s32
_separatorem_invenire (
    constans Lector* l,
                i32  initium,
                i32* post)
{
    i32 i = initium;
    i32 fl;
    i32 a;
    i32 b;

    dum (i < l->mensura)
    {
        fl  = _finis_lineae(l, i);
        a   = _sinistram_praecidere(l, i, fl);
        b   = _dextram_praecidere(l, a, fl);
        si (b - a == II && l->datum[a] == '%' && l->datum[a + I] == '%')
        {
            *post = fl < l->mensura ? fl + I : l->mensura;
            redde (s32)i;
        }
        i = fl + I;
    }
    redde -I;
}


/* ==================================================
 * Caput
 * ================================================== */

/* numerus decimalis brevis; -1 si non */
interior s32
_numerus (
    chorda ch)
{
    s32 n = ZEPHYRUM;
    i32 i;

    si (ch.mensura == ZEPHYRUM || ch.mensura > III)
    {
        redde -I;
    }
    per (i = ZEPHYRUM; i < ch.mensura; i++)
    {
        si (ch.datum[i] < '0' || ch.datum[i] > '9')
        {
            redde -I;
        }
        n = n * X + (s32)(ch.datum[i] - '0');
    }
    redde n;
}

interior vacuum
_caput_addere (
    Lector* l,
    chorda  clavis,
    chorda  valor,
       i32  offset)
{
    GabcCantus* c      = l->cantus;
     GabcCaput* caput  = (GabcCaput*)xar_addere(l->capita);
           i32  columna;
           s32  n;

    si (caput == NIHIL)
    {
        l->deficit = VERUM;
        redde;
    }
    caput->clavis  = clavis;
    caput->valor   = valor;
    _locus(l, offset, &caput->linea, &columna);

    si (_aequat(clavis, "name") && c->titulus.datum == NIHIL)
    {
        c->titulus = valor;
    }
    alioquin si (_aequat(clavis, "mode") && c->modus.datum == NIHIL)
    {
        c->modus          = valor;
        n                 = _numerus(valor);
        c->modus_numerus  = n >= I && n <= VIII ? n : ZEPHYRUM;
    }
    alioquin si (_aequat(clavis, "mode-differentia"))
    {
        c->differentia = valor;
    }
    alioquin si (   _aequat(clavis, "annotation")
                 && c->numerus_annotationum < II)
    {
        c->annotationes[c->numerus_annotationum++] = valor;
    }
    alioquin si (_aequat(clavis, "staff-lines"))
    {
        n = _numerus(valor);
        si (n >= II && n <= V)
        {
            c->lineae = (i32)n;
        }
        alioquin
        {
            _vitium(l, GABC_MONITUM, GABC_CAPUT_MALUM, offset,
                "staff-lines extra II..V: IV ponitur");
        }
    }
}

/* "clavis: valor;" per lineam; valor sine ';' per lineas usque ad
 * ';;'. '%' commentarium usque ad finem lineae. */
interior vacuum
_caput_legere (
    Lector* l,
       i32  initium,
       i32  finis)
{
    i32 i = initium;
    i32 fl;
    i32 a;
    i32 b;
    s32 colon;
    i32 va;
    i32 vb;
    i32 j;
    i32 f2;
    i32 b2;
    b32 clausus;

    dum (i < finis)
    {
        fl = _finis_lineae(l, i);
        si (fl > finis)
        {
            fl = finis;
        }
        a = _sinistram_praecidere(l, i, fl);
        b = _dextram_praecidere(l, a, _sine_commentario(l, a, fl));
        si (a >= b)
        {
            i = fl + I;
            perge;
        }
        colon = -I;
        per (j = a; j < b; j++)
        {
            si (l->datum[j] == ':')
            {
                colon = (s32)j;
                frange;
            }
        }
        si (colon < ZEPHYRUM)
        {
            _vitium(l, GABC_VITIUM, GABC_CAPUT_MALUM, a,
                "linea capitis sine ':'");
            i = fl + I;
            perge;
        }
        va = _sinistram_praecidere(l, (i32)colon + I, b);
        si (l->datum[b - I] == ';')
        {
            vb = b - I;
            si (vb > va && l->datum[vb - I] == ';')
            {
                vb--;
            }
            i = fl + I;
        }
        alioquin
        {
            /* valor per lineas plures: usque ad ';;' */
            clausus  = FALSUM;
            vb       = b;
            j        = fl + I;
            dum (j < finis)
            {
                f2 = _finis_lineae(l, j);
                si (f2 > finis)
                {
                    f2 = finis;
                }
                b2 = _dextram_praecidere(l, j,
                    _sine_commentario(l, j, f2));
                si (   b2 >= j + II && l->datum[b2 - I] == ';'
                    && l->datum[b2 - II] == ';')
                {
                    vb       = b2 - II;
                    clausus  = VERUM;
                    i        = f2 + I;
                    frange;
                }
                j = f2 + I;
            }
            si (!clausus)
            {
                _vitium(l, GABC_VITIUM, GABC_CAPUT_MALUM, va,
                    "valor capitis non clausus (';;' deest)");
                vb  = finis;
                i   = finis;
            }
        }
        _caput_addere(l,
            _sectio(l, a, _dextram_praecidere(l, a, (i32)colon)),
            _sectio(l, va, _dextram_praecidere(l, va, vb)), a);
    }
}


/* ==================================================
 * Notae
 * ================================================== */

/* littera positionis (a..n, p; maiuscula = inclinatum) -> 0..14; -1 */
interior s32
_positio (
    i8 ch)
{
    si (ch >= 'a' && ch <= 'n')
    {
        redde (s32)(ch - 'a');
    }
    si (ch >= 'A' && ch <= 'N')
    {
        redde (s32)(ch - 'A');
    }
    si (ch == 'p' || ch == 'P')
    {
        redde POSITIO_P;
    }
    redde -I;
}

interior b32
_alteratio_est (
    i8 ch)
{
    redde ch == 'x' || ch == 'y' || ch == '#' || ch == 'X' || ch == 'Y';
}

/* [cf]b?[1-5] ad j -> longitudo; 0 si non clavis */
interior i32
_clavis_longitudo (
    constans Lector* l,
                i32  j,
                i32  b)
{
    i32 k = j + I;

    si (j >= b || (l->datum[j] != 'c' && l->datum[j] != 'f'))
    {
        redde ZEPHYRUM;
    }
    si (k < b && l->datum[k] == 'b')
    {
        k++;
    }
    si (k < b && l->datum[k] >= '1' && l->datum[k] <= '5')
    {
        redde k + I - j;
    }
    redde ZEPHYRUM;
}

/* custos ad j? littera minuscula + '+' (Gregorii [a-np]\+) */
interior b32
_custos_est (
    constans Lector* l,
                i32  j,
                i32  b)
{
    redde j + I < b && l->datum[j + I] == '+' && l->datum[j] >= 'a'
        && l->datum[j] <= 'p';
}

/* incipitne nota ad j? (praefixum - aut @, littera quae non clavis,
 * custos, alteratio) */
interior b32
_notam_incipit (
    constans Lector* l,
                i32  j,
                i32  b)
{
    si (j < b && (l->datum[j] == '-' || l->datum[j] == '@'))
    {
        j++;
    }
    si (   j >= b || _positio(l->datum[j]) < ZEPHYRUM
        || _clavis_longitudo(l, j, b) > ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde !(j + I < b && (_custos_est(l, j, b)
        || _alteratio_est(l->datum[j + I])));
}

/* positio supra systema (Gregorii highest_pitch: m in IV lineis, p in
 * V) -> monitum */
interior vacuum
_positionem_probare (
    Lector* l,
       s32  positio,
       i32  offset)
{
    si (positio > (s32)(II * l->cantus->lineae + IV))
    {
        _vitium(l, GABC_MONITUM, GABC_POSITIO_EXTRA, offset,
            "positio supra systema");
    }
}

interior GabcElementum*
_elementum_novum (
                Lector* l,
     GabcElementiGenus  genus,
                   i32  offset)
{
    GabcElementum* el = (GabcElementum*)xar_addere(l->elementa);

    si (el == NIHIL)
    {
        l->deficit = VERUM;
        redde NIHIL;
    }
    memset(el, ZEPHYRUM, magnitudo(GabcElementum));
    el->genus    = genus;
    el->positio  = -I;
    el->nota     = -I;
    _locus(l, offset, &el->linea, &el->columna);
    redde el;
}

/* nota ad litteram; neuma aperta crescit aut nova aperitur. initium =
 * initium signi (praefixum incluso). Redde offset post notam. */
interior i32
_notam_legere (
    Lector* l,
       i32  initium,
       i32  littera,
       i32  b,
       s32* neuma)
{
         GabcNota* nota;
    GabcElementum* el;
               i8  ch  = l->datum[littera];
              i32  k   = littera + I;

    nota = (GabcNota*)xar_addere(l->notae);
    si (nota == NIHIL)
    {
        l->deficit = VERUM;
        redde b;
    }
    memset(nota, ZEPHYRUM, magnitudo(GabcNota));
    nota->positio      = (i8)_positio(ch);
    nota->directio     = -I;
    nota->mora_locus   = -I;
    nota->ictus_locus  = -I;
    _locus(l, littera, &nota->linea, &nota->columna);
    _positionem_probare(l, (s32)nota->positio, littera);
    si (ch >= 'A' && ch <= 'P')
    {
        nota->formae |= GABC_FORMA_INCLINATUM;
        si (k < b && l->datum[k] >= '0' && l->datum[k] <= '2')
        {
            nota->directio = (s8)(l->datum[k] - '0');
            k++;
        }
    }
    si (initium < littera)
    {
        nota->formae |= l->datum[initium] == '-'
            ? GABC_FORMA_INITIO_DEBILIS : GABC_FORMA_FUSIO;
    }

    si (*neuma >= ZEPHYRUM)
    {
        el = (GabcElementum*)xar_obtinere(l->elementa, (i32)*neuma);
        el->numerus++;
    }
    alioquin
    {
        el = _elementum_novum(l, GABC_EL_NEUMA, initium);
        si (el != NIHIL)
        {
            el->prima    = xar_numerus(l->notae) - I;
            el->numerus  = I;
            *neuma       = (s32)(xar_numerus(l->elementa) - I);
        }
    }
    redde k;
}

/* clavis ad j (post '@' si secunda); redde longitudinem */
interior i32
_clavem_legere (
    Lector* l,
       i32  j,
       i32  lon,
       b32  secunda)
{
    GabcElementum* el = _elementum_novum(l, GABC_EL_CLAVIS,
        secunda ? j - I : j);

    si (el == NIHIL)
    {
        redde lon;
    }
    el->clavis          = (character)l->datum[j];
    el->clavis_bemolle  = lon == III;
    el->clavis_secunda  = secunda;
    el->clavis_linea    = (i8)(l->datum[j + lon - I] - '0');
    si ((i32)el->clavis_linea > l->cantus->lineae)
    {
        _vitium(l, GABC_VITIUM, GABC_CLAVIS_MALA, j,
            "linea clavis extra systema");
    }
    redde lon;
}

/* littera + x y # (X Y ## molles, ? parenthesis) ad j; longitudo */
interior i32
_alterationem_legere (
    Lector* l,
       i32  j,
       i32  b)
{
    GabcElementum* el      = _elementum_novum(l, GABC_EL_ALTERATIO, j);
               i8  signum  = l->datum[j + I];
              i32  k       = j + II;

    si (el == NIHIL)
    {
        redde II;
    }
    el->positio = _positio(l->datum[j]);
    _positionem_probare(l, el->positio, j);
    el->alteratio = signum == '#' ? GABC_ALTERATIO_DIESIS
        : (signum == 'x' || signum == 'X') ? GABC_ALTERATIO_BEMOLLE
        : GABC_ALTERATIO_NATURALE;
    el->molle = signum == 'X' || signum == 'Y';
    si (signum == '#' && k < b && l->datum[k] == '#')
    {
        el->molle = VERUM;
        k++;
    }
    alioquin si (   k < b && l->datum[k] == '?'
                 && (signum == 'x' || signum == 'y' || signum == '#'))
    {
        el->parenthesis = VERUM;
        k++;
    }
    redde k - j;
}

/* divisio ad j; longitudo. post_notam: nota coniuncta praecedit */
interior i32
_divisionem_legere (
    Lector* l,
       i32  j,
       i32  b,
       b32  post_notam)
{
    GabcElementum* el  = _elementum_novum(l, GABC_EL_DIVISIO, j);
               i8  ch  = l->datum[j];
              i32  k   = j + I;
              i32  r;

    si (el == NIHIL)
    {
        redde I;
    }
    si (   (ch == ',' || ch == ';') && k < b
        && l->datum[k] >= '1' && l->datum[k] <= '8')
    {
        el->divisio          = GABC_DIVISIO_DOMINICANA;
        el->divisio_numerus  = (i8)(l->datum[k] - '0');
        el->divisio_signum   = (character)ch;
        k++;
    }
    alioquin si (ch == ';')
    {
        el->divisio = GABC_DIVISIO_MINOR;
    }
    alioquin si (ch == ':')
    {
        el->divisio = GABC_DIVISIO_MAIOR;
        si (k < b && l->datum[k] == ':')
        {
            el->divisio = GABC_DIVISIO_FINALIS;
            k++;
        }
        alioquin si (k < b && l->datum[k] == '?')
        {
            el->divisio = GABC_DIVISIO_MAIOR_PUNCTATA;
            k++;
        }
    }
    alioquin
    {
        /* ` ^ , cum 0 (alta) et ? (parenthesis, non ^) quolibet ordine */
        el->divisio = ch == '`' ? GABC_DIVISIO_VIRGULA
            : ch == '^' ? GABC_DIVISIO_MINIMIS : GABC_DIVISIO_MINIMA;
        per (r = ZEPHYRUM; r < II && k < b; r++)
        {
            si (l->datum[k] == '0' && !el->alta)
            {
                el->alta = VERUM;
                k++;
            }
            alioquin si (   l->datum[k] == '?' && ch != '^'
                         && !el->parenthesis)
            {
                el->parenthesis = VERUM;
                k++;
            }
        }
    }
    si (post_notam || _notam_incipit(l, k, b))
    {
        _vitium(l, GABC_MONITUM, GABC_DIVISIO_IN_NEUMA, j,
            "divisio intra notas coniunctas");
    }
    redde k - j;
}

interior vacuum
_spatium_addere (
          Lector* l,
     GabcSpatium  genus,
             i32  offset)
{
    GabcElementum* el = _elementum_novum(l, GABC_EL_SPATIUM, offset);

    si (el != NIHIL)
    {
        el->spatium = genus;
    }
}

/* spatium '/' ad j (etiam post '!'): / // /0 /! /[n]; longitudo */
interior i32
_spatium_legere (
    Lector* l,
       i32  j,
       i32  b)
{
     GabcElementum* el     = _elementum_novum(l, GABC_EL_SPATIUM, j);
       GabcSpatium  genus  = GABC_SPATIUM_NEUMATICUM;
               i32  k      = j + I;
               i32  lon    = I;

    si (k < b && l->datum[k] == '0')
    {
        genus  = GABC_SPATIUM_DIMIDIUM;
        lon    = II;
    }
    alioquin si (k < b && l->datum[k] == '!')
    {
        genus  = GABC_SPATIUM_INSEPARABILE;
        lon    = II;
    }
    alioquin si (k < b && l->datum[k] == '/')
    {
        /* "//[" = '/' deinde "/[n]" */
        si (!(k + I < b && l->datum[k + I] == '['))
        {
            genus  = GABC_SPATIUM_MAGNUM;
            lon    = II;
        }
    }
    alioquin si (k < b && l->datum[k] == '[')
    {
        genus = GABC_SPATIUM_FACTOR;
        k++;
        dum (k < b && l->datum[k] != ']')
        {
            k++;
        }
        si (el != NIHIL)
        {
            el->factor = _sectio(l, j + II, k);
        }
        si (k >= b)
        {
            _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, j,
                "'/[' sine ']'");
            lon = b - j;
        }
        alioquin
        {
            lon = k + I - j;
        }
    }
    si (el != NIHIL)
    {
        el->spatium = genus;
    }
    redde lon;
}

/* signum opacum [a, b): textus fontis; nota = ultima neumae apertae */
interior vacuum
_signum_opacum (
    Lector* l,
       i32  a,
       i32  b,
       s32  neuma)
{
    GabcElementum* el = _elementum_novum(l, GABC_EL_SIGNUM_OPACUM, a);

    si (el != NIHIL)
    {
        el->textus  = _sectio(l, a, b);
        el->nota    = neuma >= ZEPHYRUM
            ? (s32)(xar_numerus(l->notae) - I) : -I;
    }
}

/* claves signorum '[x:...]' quae Gregorius novit (litterae minusculae
 * post '['; numeri sequentes, ut nm1, non numerantur) */
hic_manens constans character* SIGNA_NOTARUM[] = {
    "cs", "cn", "ob", "ub", "ocb", "ocba", "nm", "gm", "em", "altm",
    "nv", "gv", "ev", "alt", "oll", "ull", "shape", "oslur", "uslur",
    "oh", "uh", "nocustos", "hl", "ll", NIHIL
};

interior b32
_signum_notae_notum (
    constans Lector* l,
                i32  a,
                i32  b)
{
    i32 k = a + I;
    i32 n;
    i32 i;

    dum (k < b && l->datum[k] >= 'a' && l->datum[k] <= 'z')
    {
        k++;
    }
    n = k - a - I;
    per (i = ZEPHYRUM; SIGNA_NOTARUM[i] != NIHIL; i++)
    {
        si (   (i32)strlen(SIGNA_NOTARUM[i]) == n
            && memcmp(l->datum + a + I, SIGNA_NOTARUM[i], (size_t)n)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* signum litteris datum ad j? */
interior b32
_literae_ad (
       constans Lector* l,
                   i32  j,
                   i32  b,
    constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde j + n <= b
        && memcmp(l->datum + j, literae, (size_t)n) == ZEPHYRUM;
}

/* signum modificans post notam; longitudo, 0 si non */
interior i32
_modificatoris_longitudo (
    constans Lector* l,
                i32  j,
                i32  b)
{
     i8 ch  = l->datum[j];
    i32 k   = j + I;

    commutatio (ch)
    {
        casus 'v': casus 'V': casus 's': casus 'w': casus 'W':
        casus 'q': casus '=': casus '~': casus '<': casus '>':
        casus 'R':
            redde I;
        casus 'o': casus 'O': casus '\'': casus '.':
            redde (k < b && (l->datum[k] == '0' || l->datum[k] == '1'))
                ? II : I;
        casus 'r':
            redde (k < b && l->datum[k] >= '0' && l->datum[k] <= '8')
                ? II : I;
        casus '_':
            dum (k < b && l->datum[k] >= '0' && l->datum[k] <= '5')
            {
                k++;
            }
            redde k - j;
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* vv vvv VV VVV ss sss: nota nova in eadem positione, forma data, in
 * neuma aperta (locus = signi iterantis) */
interior vacuum
_notam_iterare (
    Lector* l,
       i32  j,
       s32  neuma,
       i32  forma)
{
          GabcNota  prior;
          GabcNota* nota;
     GabcElementum* el;

    prior = *(GabcNota*)xar_obtinere(l->notae, xar_numerus(l->notae)
        - I);
    nota = (GabcNota*)xar_addere(l->notae);
    si (nota == NIHIL)
    {
        l->deficit = VERUM;
        redde;
    }
    memset(nota, ZEPHYRUM, magnitudo(GabcNota));
    nota->positio      = prior.positio;
    nota->formae       = forma | (prior.formae & GABC_FORMA_INCLINATUM);
    nota->directio     = prior.directio;
    nota->mora_locus   = -I;
    nota->ictus_locus  = -I;
    _locus(l, j, &nota->linea, &nota->columna);
    el = (GabcElementum*)xar_obtinere(l->elementa, (i32)neuma);
    el->numerus++;
}

/* signum modificans ad j in notam ultimam neumae apertae; longitudo
 * (0 si non signum). Sine nota -> vitium, signum praeteritur. */
interior i32
_modificatorem_legere (
    Lector* l,
       i32  j,
       i32  b,
       s32  neuma)
{
          i32  lon = _modificatoris_longitudo(l, j, b);
     GabcNota* nota;
           i8  ch  = l->datum[j];
          s32  cifra;
          i32  forma;
          i32  k;

    si (lon == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (neuma < ZEPHYRUM)
    {
        _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
            "signum notae sine nota");
        redde lon;
    }
    nota = (GabcNota*)xar_obtinere(l->notae, xar_numerus(l->notae) - I);
    cifra = lon == II ? (s32)(l->datum[j + I] - '0') : -I;
    commutatio (ch)
    {
        casus 'v': casus 'V': casus 's':
            forma = ch == 'v' ? GABC_FORMA_VIRGA
                : ch
                    == 'V' ? GABC_FORMA_VIRGA_REVERSA : GABC_FORMA_STROPHA;
            si ((nota->formae & forma) != ZEPHYRUM)
            {
                _notam_iterare(l, j, neuma, forma);
            }
            alioquin
            {
                nota->formae |= forma;
            }
            frange;
        casus 'w': nota->formae |= GABC_FORMA_QUILISMA; frange;
        casus 'W': nota->formae |= GABC_FORMA_QUILISMA_QUADRATUM; frange;
        casus 'q': nota->formae |= GABC_FORMA_QUADRATUM; frange;
        casus '=': nota->formae |= GABC_FORMA_LINEA; frange;
        casus '~': nota->formae |= GABC_FORMA_LIQUESCENS; frange;
        casus '<': nota->formae |= GABC_FORMA_AUCTA_ASCENDENS; frange;
        casus '>': nota->formae |= GABC_FORMA_AUCTA_DESCENDENS; frange;
        casus 'R': nota->formae |= GABC_FORMA_LINEA_PUNCTUM; frange;
        casus 'o': casus 'O':
            nota->formae |= ch == 'o' ? GABC_FORMA_ORISCUS
                : GABC_FORMA_ORISCUS_SCAPUS;
            si (cifra >= ZEPHYRUM)
            {
                nota->directio = (s8)cifra;
            }
            frange;
        casus 'r':
            si (cifra < ZEPHYRUM)
            {
                nota->formae |= GABC_FORMA_CAVUM;
            }
            alioquin si (cifra == ZEPHYRUM)
            {
                nota->formae |= GABC_FORMA_CAVUM
                    | GABC_FORMA_LINEA_PUNCTUM;
            }
            alioquin si (cifra <= V)
            {
                nota->signa |= (i8)(I << (cifra - I));
            }
            alioquin
            {
                /* r6 bemolle, r7 naturale, r8 diesis: nota sola */
                nota->signa      |= GABC_SIGNUM_FICTA;
                nota->alteratio  = cifra - VII;
            }
            frange;
        casus '\'':
            nota->ictus = I;
            si (cifra >= ZEPHYRUM)
            {
                nota->ictus_locus = (s8)cifra;
            }
            frange;
        casus '.':
            nota->mora++;
            si (cifra >= ZEPHYRUM)
            {
                nota->mora_locus = (s8)cifra;
            }
            frange;
        casus '_':
            nota->episema |= GABC_EPISEMA_ADEST;
            per (k = j + I; k < j + lon; k++)
            {
                nota->episema |= (i8)GABC_EPISEMA_NUMERUS(
                    l->datum[k] - '0');
            }
            frange;
        ordinarius:
            frange;
    }
    redde lon;
}

/* notae inter '(' et ')': [a, b) -> elementa syllabae currentis */
interior vacuum
_notas_legere (
    Lector* l,
       i32  a,
       i32  b)
{
              i32  j      = a;
              s32  neuma  = -I;
              b32  fusio  = FALSUM;   /* '@[' apertum */
              i32  lon;
              i32  k;
               i8  ch;
    GabcElementum* el;

    dum (j < b)
    {
        ch = l->datum[j];
        commutatio (ch)
        {
            casus ' ':
                _spatium_addere(l, GABC_SPATIUM_GLYPHAE, j);
                neuma = -I;
                j++;
                frange;
            casus '\t': casus '\r': casus '\n':
                j++;
                frange;
            casus '/':
                neuma = -I;
                j += _spatium_legere(l, j, b);
                frange;
            casus '!':
                neuma = -I;
                si (   j + II < b && l->datum[j + I] == '/'
                    && (l->datum[j + II] == '0'
                    || l->datum[j + II] == '!'))
                {
                    /* !/0 !/! = /0 /! (Gregorius) */
                    j += I + _spatium_legere(l, j + I, b);
                    frange;
                }
                _spatium_addere(l, GABC_SPATIUM_NULLUM, j);
                si (j + I < b && l->datum[j + I] == '/')
                {
                    j += I + _spatium_legere(l, j + I, b);
                }
                alioquin si (j + I < b && l->datum[j + I] == ' ')
                {
                    _spatium_addere(l, GABC_SPATIUM_GLYPHAE, j + I);
                    j += II;
                }
                alioquin
                {
                    j++;
                }
                frange;
            casus '`': casus '^': casus ',': casus ';': casus ':':
                j += _divisionem_legere(l, j, b, neuma >= ZEPHYRUM);
                neuma = -I;
                frange;
            casus '[':
                si (j + I < b && l->datum[j + I] == '[')
                {
                    _signum_opacum(l, j, j + II, neuma);
                    j += II;
                    frange;
                }
                k = j + I;
                dum (k < b && l->datum[k] != ']')
                {
                    k += (l->datum[k] == '$' && k + I < b) ? II : I;
                }
                si (k >= b)
                {
                    _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, j,
                        "'[' sine ']'");
                    j = b;
                    frange;
                }
                si (!_signum_notae_notum(l, j, k))
                {
                    _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, j,
                        "signum notae Gregorio ignotum: opacum servatur");
                }
                _signum_opacum(l, j, k + I, neuma);
                j = k + I;
                frange;
            casus ']':
                si (j + I < b && l->datum[j + I] == ']')
                {
                    _signum_opacum(l, j, j + II, neuma);
                    j += II;
                }
                alioquin si (fusio)
                {
                    /* finis '@[' (Gregorii autofusio) */
                    _signum_opacum(l, j, j + I, neuma);
                    fusio = FALSUM;
                    j++;
                }
                alioquin
                {
                    _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
                        "character in notis ignotus");
                    j++;
                }
                frange;
            casus '{': casus '}':
                _signum_opacum(l, j, j + I, neuma);
                neuma = -I;
                j++;
                frange;
            casus '|':
                /* nabc usque ad '|' proximum (nabc-lines I) */
                k = j + I;
                dum (k < b && l->datum[k] != '|')
                {
                    k++;
                }
                _signum_opacum(l, j, k, neuma);
                neuma = -I;
                j = k < b ? k + I : b;
                frange;
            casus '<':
                lon = _literae_ad(l, j, b, "<nlba>") ? VI
                    : _literae_ad(l, j, b, "</nlba>") ? VII : ZEPHYRUM;
                si (lon > ZEPHYRUM)
                {
                    _signum_opacum(l, j, j + lon, neuma);
                    j += lon;
                    frange;
                }
                /* '<' aliter: aucta ascendens (signum notae) */
                lon = _modificatorem_legere(l, j, b, neuma);
                j += lon > ZEPHYRUM ? lon : I;
                frange;
            casus 'z': casus 'Z':
                neuma = -I;
                si (ch == 'z' && j + I < b && l->datum[j + I] == '0')
                {
                    /* custos automaticus */
                    (vacuum)_elementum_novum(l, GABC_EL_CUSTOS, j);
                    j += II;
                    frange;
                }
                el = _elementum_novum(l, GABC_EL_FRACTIO, j);
                j++;
                si (el != NIHIL)
                {
                    el->iustificata = ch == 'z';
                }
                si (j < b && (l->datum[j] == '+' || l->datum[j] == '-'))
                {
                    si (el != NIHIL)
                    {
                        el->custos = l->datum[j] == '+' ? I : -I;
                    }
                    j++;
                }
                frange;
            casus '@':
                lon = _clavis_longitudo(l, j + I, b);
                si (lon > ZEPHYRUM)
                {
                    j      += I + _clavem_legere(l, j + I, lon, VERUM);
                    neuma  = -I;
                }
                alioquin si (j + I < b && l->datum[j + I] == '[')
                {
                    _signum_opacum(l, j, j + II, neuma);
                    fusio  = VERUM;
                    j      += II;
                }
                alioquin si (_notam_incipit(l, j + I, b))
                {
                    j = _notam_legere(l, j, j + I, b, &neuma);
                }
                alioquin
                {
                    _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
                        "character in notis ignotus");
                    j++;
                }
                frange;
            casus '-':
                si (_notam_incipit(l, j + I, b))
                {
                    j = _notam_legere(l, j, j + I, b, &neuma);
                }
                alioquin
                {
                    _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
                        "character in notis ignotus");
                    j++;
                }
                frange;
            ordinarius:
                si (_positio(ch) >= ZEPHYRUM)
                {
                    lon = _clavis_longitudo(l, j, b);
                    si (lon > ZEPHYRUM)
                    {
                        j      += _clavem_legere(l, j, lon, FALSUM);
                        neuma  = -I;
                    }
                    alioquin si (_custos_est(l, j, b))
                    {
                        el = _elementum_novum(l, GABC_EL_CUSTOS, j);
                        si (el != NIHIL)
                        {
                            el->positio = _positio(ch);
                        }
                        _positionem_probare(l, _positio(ch), j);
                        neuma  = -I;
                        j      += II;
                    }
                    alioquin si (   j + I < b
                                 && _alteratio_est(l->datum[j + I]))
                    {
                        j      += _alterationem_legere(l, j, b);
                        neuma  = -I;
                    }
                    alioquin
                    {
                        j = _notam_legere(l, j, j, b, &neuma);
                    }
                    frange;
                }
                lon = _modificatorem_legere(l, j, b, neuma);
                si (lon > ZEPHYRUM)
                {
                    j += lon;
                    frange;
                }
                si (_alteratio_est(ch))
                {
                    _vitium(l, GABC_VITIUM,
                        GABC_ALTERATIO_SINE_POSITIONE,
                        j, "alteratio sine littera positionis");
                    j++;
                    frange;
                }
                _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
                    "character in notis ignotus");
                j++;
                /* sequentia UTF-8 una, vitium unum */
                dum (j < b && (l->datum[j] & 0xC0) == 0x80)
                {
                    j++;
                }
                frange;
        }
    }
}


/* ==================================================
 * Textus syllabae
 * ================================================== */

/* signa stili ordine bitorum GABC_STILUS_* (pr seorsum) */
hic_manens constans character* STILI[VII] = {
    "i", "b", "ul", "c", "sc", "tt", "e"
};
#define STILUS_ELISIO_INDEX VI

/* sp -> UTF-8 (gregoriotex-symbols.tex, \gresetspecial). A/ = U+023A
 * (A cum linea, propinquum); * = U+2736 (StarSix); + = U+2020 (Dagger);
 * ut ot dt = U+203F U+2040 U+2050; - = hyphen (zerohyph) */
hic_manens constans character* SPECIALES[][II] = {
    { "A/", "\xC8\xBA" },          { "%", "%" },
    { "R/", "\xE2\x84\x9F" },      { "V/", "\xE2\x84\xA3" },
    { "'ae", "\xC7\xBD" },         { "'oe", "\xC5\x93\xCC\x81" },
    { "ae", "\xC3\xA6" },          { "oe", "\xC5\x93" },
    { "*", "\xE2\x9C\xB6" },       { "+", "\xE2\x80\xA0" },
    { "-", "-" },                  { "\\", "\\" },
    { "&", "&" },                  { "#", "#" },
    { "_", "_" },                  { "ut", "\xE2\x80\xBF" },
    { "dt", "\xE2\x81\x90" },      { "ot", "\xE2\x81\x80" },
    { "~", "~" },                  { "'\xC3\xA6", "\xC7\xBD" },
    { "'\xC5\x93", "\xC5\x93\xCC\x81" },
    { NIHIL, NIHIL }
};

/* cursum currentem in textus syllabae claudit (vacuum: nihil) */
interior vacuum
_cursum_claudere (
    Lector* l)
{
    GabcTextus* t;
            i8* b;
           i32  n = xar_numerus(l->octeti);

    si (n == ZEPHYRUM)
    {
        redde;
    }
    t = (GabcTextus*)xar_addere(l->textus);
    si (t == NIHIL)
    {
        l->deficit = VERUM;
        redde;
    }
    t->genus   = l->cursus_genus;
    t->stilus  = l->cursus_stilus;
    si (l->cursus_purus)
    {
        t->textus = _sectio(l, l->cursus_fons, l->cursus_fons + n);
    }
    alioquin
    {
        b = (i8*)piscina_allocare(l->piscina, (memoriae_index)n);
        si (b == NIHIL)
        {
            l->deficit = VERUM;
            redde;
        }
        (vacuum)xar_copiare_ad_tabulam(l->octeti, b, ZEPHYRUM, n);
        t->textus.datum    = b;
        t->textus.mensura  = n;
    }
    xar_vacare(l->octeti);
}

/* octetus in cursum (genus, stilus); fons = offset fontis aut -1
 * (octetus factus, non in fonte) */
interior vacuum
_octetum_addere (
              Lector* l,
     GabcTextusGenus  genus,
                 i32  stilus,
                  i8  octetus,
                 s32  fons)
{
    i8* p;
   i32  n;

    si (genus != l->cursus_genus || stilus != l->cursus_stilus)
    {
        _cursum_claudere(l);
        l->cursus_genus   = genus;
        l->cursus_stilus  = stilus;
    }
    n = xar_numerus(l->octeti);
    si (n == ZEPHYRUM)
    {
        l->cursus_purus  = fons >= ZEPHYRUM;
        l->cursus_fons   = fons >= ZEPHYRUM ? (i32)fons : ZEPHYRUM;
    }
    alioquin si (fons < ZEPHYRUM || (i32)fons != l->cursus_fons + n)
    {
        l->cursus_purus = FALSUM;
    }
    p = (i8*)xar_addere(l->octeti);
    si (p == NIHIL)
    {
        l->deficit = VERUM;
        redde;
    }
    *p = octetus;
    si (genus == GABC_TEXTUS_CANTATUS)
    {
        l->cantati++;
    }
}

interior vacuum
_literas_addere (
                Lector* l,
       GabcTextusGenus  genus,
                   i32  stilus,
    constans character* literae)
{
    dum (*literae != '\0')
    {
        _octetum_addere(l, genus, stilus, (i8)*literae, -I);
        literae++;
    }
}

/* "<t>" (clausum: "</t>") ad i -> longitudo; 0 si non */
interior i32
_signum_aequat (
       constans Lector* l,
                   i32  i,
    constans character* titulus,
                   b32  clausum)
{
    i32 n = (i32)strlen(titulus);
    i32 k = i + I;

    si (k < l->mensura && clausum)
    {
        si (l->datum[k] != '/')
        {
            redde ZEPHYRUM;
        }
        k++;
    }
    si (   k + n < l->mensura
        && memcmp(l->datum + k, titulus, (size_t)n) == ZEPHYRUM
        && l->datum[k + n]                          == '>')
    {
        redde k + n + I - i;
    }
    redde ZEPHYRUM;
}

/* '<', spatia, t, spatia, '/' optionale, spatia, '>' (Gregorii clear,
 * pr) ad i -> longitudo; 0 si non. valor: si non NIHIL, etiam t ':'
 * numerus accipitur et *valor = VERUM */
interior i32
_signum_vacuum_aequat (
       constans Lector* l,
                   i32  i,
    constans character* titulus,
                   b32* valor)
{
    i32 n = (i32)strlen(titulus);
    i32 k = i + I;

    dum (k < l->mensura && _spatium_est(l->datum[k]))
    {
        k++;
    }
    si (!(k + n <= l->mensura
        && memcmp(l->datum + k, titulus, (size_t)n) == ZEPHYRUM))
    {
        redde ZEPHYRUM;
    }
    k += n;
    dum (k < l->mensura && _spatium_est(l->datum[k]))
    {
        k++;
    }
    si (valor != NIHIL && k < l->mensura && l->datum[k] == ':')
    {
        k++;
        dum (k < l->mensura && _spatium_est(l->datum[k]))
        {
            k++;
        }
        si (!(k < l->mensura && ((l->datum[k] >= '0'
            && l->datum[k] <= '9') || l->datum[k] == '.')))
        {
            redde ZEPHYRUM;
        }
        dum (   k < l->mensura && ((l->datum[k] >= '0'
            && l->datum[k] <= '9') || l->datum[k] == '.'))
        {
            k++;
        }
        dum (k < l->mensura && _spatium_est(l->datum[k]))
        {
            k++;
        }
        *valor = VERUM;
    }
    si (k < l->mensura && l->datum[k] == '/')
    {
        k++;
    }
    dum (k < l->mensura && _spatium_est(l->datum[k]))
    {
        k++;
    }
    redde (k < l->mensura && l->datum[k] == '>') ? k + I - i : ZEPHYRUM;
}

/* signum claudens (literae) ab a, '$' effugit; -1 si nullum. *sine:
 * cache (nullum ab hoc offset), ne quaestio iterata quadratica fiat */
interior s32
_finem_crudum_invenire (
       constans Lector* l,
                   i32  a,
    constans character* literae,
                   s32* sine)
{
    i32 k = a;

    si (*sine >= ZEPHYRUM && (s32)a >= *sine)
    {
        redde -I;
    }
    dum (k < l->mensura)
    {
        si (l->datum[k] == '$' && k + I < l->mensura)
        {
            k += II;
            perge;
        }
        si (_literae_ad(l, k, l->mensura, literae))
        {
            redde (s32)k;
        }
        k++;
    }
    *sine = (s32)a;
    redde -I;
}

/* causa "'<t>' ..." vel "'</t>' ..." in receptaculum */
interior constans character*
_causa_signi (
              character* receptaculum,
     constans character* titulus,
                    b32  clausum,
     constans character* reliquum)
{
    strcpy(receptaculum, clausum ? "'</" : "'<");
    strcat(receptaculum, titulus);
    strcat(receptaculum, ">'");
    strcat(receptaculum, reliquum);
    redde receptaculum;
}

/* cursus crudus (v, alt) ab a usque ad finis, '$' effugit */
interior vacuum
_crudum_addere (
              Lector* l,
     GabcTextusGenus  genus,
                 i32  a,
                 i32  finis)
{
    i32 k = a;

    dum (k < finis)
    {
        si (l->datum[k] == '$' && k + I < finis)
        {
            k++;
        }
        _octetum_addere(l, genus, ZEPHYRUM, l->datum[k], (s32)k);
        k++;
    }
    _cursum_claudere(l);
}

/* sp ad i (signum "<sp>" longitudinis lon): UTF-8 ex tabula; redde
 * longitudinem totam, aut lon si non clausum (signum praeteritur) */
interior i32
_specialem_legere (
              Lector* l,
                 i32  i,
                 i32  lon,
     GabcTextusGenus  genus,
                 i32  stilus)
{
    character codex[XVI];
          i32 n      = ZEPHYRUM;
          i32 k      = i + lon;
          s32 finis  = -I;
          i32 r;

    dum (k < l->mensura)
    {
        si (l->datum[k] == '$' && k + I < l->mensura)
        {
            k += II;
            perge;
        }
        si (l->datum[k] == '<')
        {
            si (_literae_ad(l, k, l->mensura, "</sp>"))
            {
                finis = (s32)k;
            }
            frange;
        }
        si (l->datum[k] == '{' || l->datum[k] == '}')
        {
            frange;
        }
        k++;
    }
    si (finis < ZEPHYRUM)
    {
        _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, i,
            "'<sp>' sine '</sp>'");
        redde lon;
    }
    per (k = i + lon; k < (i32)finis; k++)
    {
        si (l->datum[k] == '$' && k + I < (i32)finis)
        {
            k++;
        }
        si (n < XV)
        {
            codex[n] = (character)l->datum[k];
        }
        n++;
    }
    codex[n < XV ? n : XV] = '\0';
    per (r = ZEPHYRUM; n < XV && SPECIALES[r][ZEPHYRUM] != NIHIL; r++)
    {
        si (strcmp(codex, SPECIALES[r][ZEPHYRUM]) == ZEPHYRUM)
        {
            _literas_addere(l, genus, stilus, SPECIALES[r][I]);
            redde (i32)finis + V - i;
        }
    }
    _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
        "'<sp>' ignotum: codex ut textus servatur");
    per (k = i + lon; k < (i32)finis; k++)
    {
        si (l->datum[k] == '$' && k + I < (i32)finis)
        {
            k++;
        }
        _octetum_addere(l, genus, stilus, l->datum[k], (s32)k);
    }
    redde (i32)finis + V - i;
}

/* signum '<' ad i in textu; redde longitudinem consumptam (>= 1) */
interior i32
_signum_textus_legere (
              Lector* l,
                 i32  i,
         GabcSyllaba* s,
     GabcTextusGenus  genus,
                 b32* protrusio)
{
    character causa[LXIV];
          i32 stilus  = l->stilus | (*protrusio ? GABC_STILUS_PROTRUSIO
                            : ZEPHYRUM);
          i32 lon;
          i32 bit;
          i32 k;
          s32 finis;
          b32 valor   = FALSUM;

    per (k = ZEPHYRUM; k < VII; k++)
    {
        bit = (i32)I << k;
        lon = _signum_aequat(l, i, STILI[k], FALSUM);
        si (lon > ZEPHYRUM)
        {
            si ((l->stilus & bit) != ZEPHYRUM)
            {
                _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
                    _causa_signi(causa, STILI[k], FALSUM,
                    " iam apertum"));
            }
            alioquin
            {
                l->stilus         |= bit;
                l->stili_loci[k]  = i;
            }
            redde lon;
        }
        lon = _signum_aequat(l, i, STILI[k], VERUM);
        si (lon > ZEPHYRUM)
        {
            si ((l->stilus & bit) != ZEPHYRUM)
            {
                l->stilus &= ~bit;
            }
            alioquin
            {
                _causa_signi(causa, STILI[k], VERUM, " sine '<");
                strcat(causa, STILI[k]);
                strcat(causa, ">'");
                _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i, causa);
            }
            redde lon;
        }
    }

    lon = _signum_aequat(l, i, "sp", FALSUM);
    si (lon > ZEPHYRUM)
    {
        redde _specialem_legere(l, i, lon, genus, stilus);
    }
    lon = _signum_aequat(l, i, "v", FALSUM);
    si (lon > ZEPHYRUM)
    {
        finis = _finem_crudum_invenire(l, i + lon, "</v>",
            &l->sine_fine_v);
        si (finis < ZEPHYRUM)
        {
            _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, i,
                "'<v>' sine '</v>'");
            redde lon;
        }
        _crudum_addere(l, GABC_TEXTUS_VERBATIM, i + lon, (i32)finis);
        redde (i32)finis + IV - i;
    }
    lon = _signum_aequat(l, i, "alt", FALSUM);
    si (lon > ZEPHYRUM)
    {
        finis = _finem_crudum_invenire(l, i + lon, "</alt>",
            &l->sine_fine_alt);
        si (finis < ZEPHYRUM)
        {
            _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, i,
                "'<alt>' sine '</alt>'");
            redde lon;
        }
        _crudum_addere(l, GABC_TEXTUS_SUPRA, i + lon, (i32)finis);
        redde (i32)finis + VI - i;
    }
    si ((lon = _signum_aequat(l, i, "eu", FALSUM)) > ZEPHYRUM)
    {
        s->signa |= GABC_SYLLABA_EUOUAE_INITIUM;
        redde lon;
    }
    si ((lon = _signum_aequat(l, i, "eu", VERUM)) > ZEPHYRUM)
    {
        s->signa |= GABC_SYLLABA_EUOUAE_FINIS;
        redde lon;
    }
    si ((lon = _signum_aequat(l, i, "nlba", FALSUM)) > ZEPHYRUM)
    {
        s->signa |= GABC_SYLLABA_NLBA_INITIUM;
        redde lon;
    }
    si ((lon = _signum_aequat(l, i, "nlba", VERUM)) > ZEPHYRUM)
    {
        s->signa |= GABC_SYLLABA_NLBA_FINIS;
        redde lon;
    }
    si ((lon = _signum_vacuum_aequat(l, i, "clear", NIHIL)) > ZEPHYRUM)
    {
        s->signa |= GABC_SYLLABA_PURGA;
        redde lon;
    }
    si ((lon = _signum_vacuum_aequat(l, i, "pr", &valor)) > ZEPHYRUM)
    {
        *protrusio = VERUM;
        si (valor)
        {
            _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
                "valor protrusionis non servatur");
        }
        redde lon;
    }

    /* signum ignotum: '<' ut textus (Gregorius idem), monitum si forma
     * signi est */
    k = i + I;
    si (k < l->mensura && l->datum[k] == '/')
    {
        k++;
    }
    lon = k;
    dum (k < l->mensura && ((l->datum[k] >= 'a' && l->datum[k] <= 'z')
        || (l->datum[k] >= 'A' && l->datum[k] <= 'Z')))
    {
        k++;
    }
    si (k > lon && k < l->mensura && l->datum[k] == '>')
    {
        _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
            "signum textus ignotum: ut textus servatur");
    }
    _octetum_addere(l, genus, stilus, l->datum[i], (s32)i);
    redde I;
}

/* textus syllabae ab i usque ad '(' (non effugitum, non in v alt sp
 * commentariove); cursus in s->textus. Redde offset '(' aut mensuram. */
interior i32
_textum_legere (
         Lector* l,
            i32  i,
    GabcSyllaba* s)
{
    GabcTextusGenus  genus       = GABC_TEXTUS_CANTATUS;
                b32  protrusio   = FALSUM;
                s32  translatio  = -I;     /* offset '[' aperti */
                s32  centrum     = -I;     /* offset '{' aperti */
                i32  initium     = i;
                i32  stilus;
       constans  i8* p;
                 i8  ch;

    xar_vacare(l->textus);
    xar_vacare(l->octeti);
    l->cantati        = ZEPHYRUM;
    l->cursus_genus   = GABC_TEXTUS_CANTATUS;
    l->cursus_stilus  = ZEPHYRUM;

    dum (i < l->mensura && l->datum[i] != '(')
    {
        ch      = l->datum[i];
        stilus  = l->stilus | (protrusio ? GABC_STILUS_PROTRUSIO
            : ZEPHYRUM);
        commutatio (ch)
        {
            casus '$':
                si (i + I < l->mensura)
                {
                    _octetum_addere(l, genus, stilus, l->datum[i + I],
                        (s32)(i + I));
                    i += II;
                }
                alioquin
                {
                    _octetum_addere(l, genus, stilus, ch, (s32)i);
                    i++;
                }
                frange;
            casus '%':
                i = _finis_lineae(l, i);
                dum (   i < l->mensura
                     && (l->datum[i] == '\n' || l->datum[i] == '\r'))
                {
                    i++;
                }
                frange;
            casus '\n': casus '\r':
                i++;
                dum (i < l->mensura && _spatium_est(l->datum[i]))
                {
                    i++;
                }
                _octetum_addere(l, genus, stilus, ' ', -I);
                frange;
            casus '~':
                _literas_addere(l, genus, stilus, "\xE2\x80\xBF");
                i++;
                frange;
            casus '{':
                si (translatio >= ZEPHYRUM)
                {
                    _octetum_addere(l, genus, stilus, ch, (s32)i);
                }
                alioquin si (centrum >= ZEPHYRUM || s->vocalis_initium
                             >= ZEPHYRUM)
                {
                    _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
                        "'{' iteratum");
                }
                alioquin
                {
                    centrum             = (s32)i;
                    s->vocalis_initium  = (s32)l->cantati;
                }
                i++;
                frange;
            casus '}':
                si (translatio >= ZEPHYRUM)
                {
                    _octetum_addere(l, genus, stilus, ch, (s32)i);
                }
                alioquin si (centrum >= ZEPHYRUM)
                {
                    s->vocalis_finis  = (s32)l->cantati;
                    centrum           = -I;
                }
                alioquin
                {
                    _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
                        "'}' sine '{'");
                }
                i++;
                frange;
            casus '[':
                si (translatio >= ZEPHYRUM)
                {
                    _octetum_addere(l, genus, stilus, ch, (s32)i);
                    i++;
                }
                alioquin si (_literae_ad(l, i, l->mensura, "[/]"))
                {
                    /* finis ambitus translationis */
                    _cursum_claudere(l);
                    _octetum_addere(l, GABC_TEXTUS_TRANSLATIO, ZEPHYRUM,
                        '/', (s32)(i + I));
                    _cursum_claudere(l);
                    i += III;
                }
                alioquin
                {
                    translatio  = (s32)i;
                    genus       = GABC_TEXTUS_TRANSLATIO;
                    i++;
                }
                frange;
            casus ']':
                si (translatio >= ZEPHYRUM)
                {
                    _cursum_claudere(l);
                    translatio  = -I;
                    genus       = GABC_TEXTUS_CANTATUS;
                }
                alioquin
                {
                    _vitium(l, GABC_MONITUM, GABC_SIGNUM_IGNOTUM, i,
                        "']' sine '['");
                    _octetum_addere(l, genus, stilus, ch, (s32)i);
                }
                i++;
                frange;
            casus '<':
                i += _signum_textus_legere(l, i, s, genus, &protrusio);
                frange;
            ordinarius:
                _octetum_addere(l, genus, stilus, ch, (s32)i);
                i++;
                frange;
        }
    }

    si (translatio >= ZEPHYRUM)
    {
        _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, (i32)translatio,
            "'[' sine ']'");
    }
    si (centrum >= ZEPHYRUM)
    {
        _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM, (i32)centrum,
            "'{' sine '}'");
        s->vocalis_finis = (s32)l->cantati;
    }
    si ((l->stilus & GABC_STILUS_ELISIO) != ZEPHYRUM)
    {
        _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM,
            l->stili_loci[STILUS_ELISIO_INDEX], "'<e>' sine '</e>'");
        l->stilus &= ~(i32)GABC_STILUS_ELISIO;
    }
    _cursum_claudere(l);
    s->textus = (GabcTextus*)_tabula(l, l->textus,
        (i32)magnitudo(GabcTextus), &s->numerus_textuum);

    /* UTF-8: monitum unum ad octetum malum primum */
    p = l->datum + initium;
    dum (p < l->datum + i)
    {
        constans i8* ante = p;

        si (utf8_decodere(&p, l->datum + i) < ZEPHYRUM)
        {
            _vitium(l, GABC_MONITUM, GABC_NON_UTF8,
                (i32)(ante - l->datum), "textus non UTF-8");
            frange;
        }
        si (p == ante)
        {
            frange;
        }
    }
    redde i;
}

/* signa textus (i b ul c sc tt) in fine fontis aperta -> vitium */
interior vacuum
_stilos_apertos_nuntiare (
    Lector* l)
{
    character causa[LXIV];
          i32 k;

    per (k = ZEPHYRUM; k < STILUS_ELISIO_INDEX; k++)
    {
        si ((l->stilus & ((i32)I << k)) != ZEPHYRUM)
        {
            _causa_signi(causa, STILI[k], FALSUM, " sine '</");
            strcat(causa, STILI[k]);
            strcat(causa, ">'");
            _vitium(l, GABC_VITIUM, GABC_SIGNUM_APERTUM,
                l->stili_loci[k], causa);
        }
    }
}


/* ==================================================
 * Corpus
 * ================================================== */

/* linea quae ad i incipit (spatiis demptis) est '%%'? */
interior b32
_separator_ad (
    constans Lector* l,
                i32  i)
{
    i32 fl  = _finis_lineae(l, i);
    i32 b   = _dextram_praecidere(l, i, fl);
    i32 a   = i;

    dum (a > ZEPHYRUM && l->datum[a - I] != '\n')
    {
        a--;
        si (!_spatium_est(l->datum[a]))
        {
            redde FALSUM;
        }
    }
    redde b - i == II && l->datum[i] == '%' && l->datum[i + I] == '%';
}

/* textus usque ad '(', notae usque ad ')'. verbum_incipit: textus
 * cantatus adest et (primus textus aut spatium post textum priorem). */
interior vacuum
_corpus_legere (
    Lector* l,
       i32  initium)
{
            i32  i             = initium;
            b32  spatium       = VERUM;
            b32  textus_visus  = FALSUM;
            i32  ta;
            i32  clausa;
    GabcSyllaba* s;

    dum (i < l->mensura)
    {
        /* spatia et commentaria inter syllabas */
        dum (i < l->mensura)
        {
            si (_spatium_est(l->datum[i]))
            {
                spatium = VERUM;
                i++;
            }
            alioquin si (l->datum[i] == '%')
            {
                si (_separator_ad(l, i))
                {
                    _vitium(l, GABC_MONITUM, GABC_SEPARATOR_ITERATUS, i,
                        "separator '%%' iteratus: commentarium putatur");
                }
                i = _finis_lineae(l, i);
            }
            alioquin
            {
                frange;
            }
        }
        si (i >= l->mensura)
        {
            frange;
        }

        ta  = i;
        s   = (GabcSyllaba*)xar_addere(l->syllabae);
        si (s == NIHIL)
        {
            l->deficit = VERUM;
            redde;
        }
        memset(s, ZEPHYRUM, magnitudo(GabcSyllaba));
        s->vocalis_initium  = -I;
        s->vocalis_finis    = -I;
        _locus(l, ta, &s->linea, &s->columna);
        i = _textum_legere(l, i, s);
        si (l->cantati > ZEPHYRUM)
        {
            s->verbum_incipit  = spatium || !textus_visus;
            textus_visus       = VERUM;
            spatium            = FALSUM;
        }

        xar_vacare(l->elementa);
        si (i < l->mensura)
        {
            /* ')' non effugitum (Gregorii notes: (\$.|[^|\)])+) */
            clausa = i + I;
            dum (clausa < l->mensura && l->datum[clausa] != ')')
            {
                clausa += (l->datum[clausa] == '$'
                    && clausa + I < l->mensura) ? II : I;
            }
            si (clausa >= l->mensura)
            {
                _vitium(l, GABC_VITIUM, GABC_PARENTHESIS_APERTA, i,
                    "'(' sine ')'");
            }
            _notas_legere(l, i + I, clausa);
            i = clausa < l->mensura ? clausa + I : l->mensura;
        }
        s->elementa = (GabcElementum*)_tabula(l, l->elementa,
            (i32)magnitudo(GabcElementum), &s->numerus_elementorum);
    }
}

interior vacuum
_signa_purgare (
    s32* signa)
{
    i32 p;

    per (p = ZEPHYRUM; p < POSITIONES; p++)
    {
        signa[p] = ALTERATIO_NULLA;
    }
}

/* gradus a do et alteratio notarum: clavis in vigore ordine fontis
 * (secunda non mutat); ambitus alterationis ut in capite plagulae */
interior vacuum
_altitudinem_resolvere (
    GabcCantus* c)
{
                       s32  locus_do  = DO_ORDINARIUM;
                       b32  bemolle   = FALSUM;
                       s32  signa[POSITIONES];
                       i32  s;
                       i32  e;
                       i32  k;
                       s32  gradus;
                  GabcNota* nota;
    constans GabcElementum* el;

    _signa_purgare(signa);
    per (s = ZEPHYRUM; s < c->numerus_syllabarum; s++)
    {
        si (c->syllabae[s].verbum_incipit)
        {
            _signa_purgare(signa);
        }
        per (e = ZEPHYRUM; e < c->syllabae[s].numerus_elementorum; e++)
        {
            el = &c->syllabae[s].elementa[e];
            commutatio (el->genus)
            {
                casus GABC_EL_CLAVIS:
                    si (!el->clavis_secunda)
                    {
                        locus_do = (s32)(II * (i32)el->clavis_linea + I)
                            - (el->clavis == 'f' ? (s32)III : ZEPHYRUM);
                        bemolle = el->clavis_bemolle;
                        _signa_purgare(signa);
                    }
                    frange;
                casus GABC_EL_DIVISIO:
                    _signa_purgare(signa);
                    frange;
                casus GABC_EL_ALTERATIO:
                    si (   el->positio >= ZEPHYRUM
                        && el->positio < (s32)POSITIONES)
                    {
                        signa[el->positio] =
                            el->alteratio == GABC_ALTERATIO_BEMOLLE ? -I
                            : el->alteratio == GABC_ALTERATIO_DIESIS ? I
                            : ZEPHYRUM;
                    }
                    frange;
                casus GABC_EL_NEUMA:
                    per (k = el->prima; k < el->prima
                        + el->numerus; k++)
                    {
                        nota          = &c->notae[k];
                        gradus        = (s32)nota->positio - locus_do;
                        nota->gradus  = gradus;
                        si ((nota->signa & GABC_SIGNUM_FICTA)
                            != ZEPHYRUM)
                        {
                            perge;
                        }
                        si (signa[nota->positio] != ALTERATIO_NULLA)
                        {
                            nota->alteratio = signa[nota->positio];
                        }
                        alioquin
                        {
                            nota->alteratio = (bemolle
                                && ((gradus % VII) + VII) % VII == VI)
                                ? -I : ZEPHYRUM;
                        }
                    }
                    frange;
                ordinarius:
                    frange;
            }
        }
    }
}


/* ==================================================
 * Functiones publicae
 * ================================================== */

GabcCantus*
gabc_legere (
      chorda  datum,
     Piscina* piscina)
{
         Lector  l;
     GabcCantus* c;
            i32  i;
            i32  initium  = ZEPHYRUM;
            i32  corpus   = ZEPHYRUM;
            i32* linea;
            s32  separator;

    memset(&l, ZEPHYRUM, magnitudo(Lector));
    c = (GabcCantus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(GabcCantus));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    memset(c, ZEPHYRUM, magnitudo(GabcCantus));
    c->lineae = IV;

    l.piscina = piscina;
    l.datum = datum.datum;
    l.mensura = datum.datum != NIHIL ? datum.mensura : ZEPHYRUM;
    l.cantus = c;
    l.initia = xar_creare(piscina, (i32)magnitudo(i32));
    l.vitia = xar_creare(piscina, (i32)magnitudo(GabcVitium));
    l.capita = xar_creare(piscina, (i32)magnitudo(GabcCaput));
    l.syllabae = xar_creare(piscina, (i32)magnitudo(GabcSyllaba));
    l.notae = xar_creare(piscina, (i32)magnitudo(GabcNota));
    l.elementa = xar_creare(piscina, (i32)magnitudo(GabcElementum));
    l.textus = xar_creare(piscina, (i32)magnitudo(GabcTextus));
    l.octeti = xar_creare(piscina, (i32)magnitudo(i8));
    l.sine_fine_v = -I;
    l.sine_fine_alt = -I;
    si (   l.initia   == NIHIL || l.vitia == NIHIL || l.capita == NIHIL
        || l.syllabae == NIHIL || l.notae == NIHIL
        || l.elementa == NIHIL || l.textus == NIHIL
        || l.octeti   == NIHIL)
    {
        redde NIHIL;
    }

    linea = (i32*)xar_addere(l.initia);
    si (linea == NIHIL)
    {
        redde NIHIL;
    }
    *linea = ZEPHYRUM;
    per (i = ZEPHYRUM; i < l.mensura; i++)
    {
        si (l.datum[i] == '\n')
        {
            linea = (i32*)xar_addere(l.initia);
            si (linea == NIHIL)
            {
                redde NIHIL;
            }
            *linea = i + I;
        }
    }

    /* BOM UTF-8: nihil monet */
    si (   l.mensura  >= III && l.datum[ZEPHYRUM] == 0xEF
        && l.datum[I] == 0xBB && l.datum[II] == 0xBF)
    {
        initium = III;
    }

    separator = _separatorem_invenire(&l, initium, &corpus);
    si (separator >= ZEPHYRUM)
    {
        _caput_legere(&l, initium, (i32)separator);
    }
    alioquin
    {
        _vitium(&l, GABC_MONITUM, GABC_SINE_CAPITE, initium,
            "separator '%%' deest: totum corpus putatur");
        corpus = initium;
    }
    _corpus_legere(&l, corpus);
    _stilos_apertos_nuntiare(&l);

    c->capita = (GabcCaput*)_tabula(&l, l.capita,
        (i32)magnitudo(GabcCaput), &c->numerus_capitum);
    c->syllabae = (GabcSyllaba*)_tabula(&l, l.syllabae,
        (i32)magnitudo(GabcSyllaba), &c->numerus_syllabarum);
    c->notae = (GabcNota*)_tabula(&l, l.notae,
        (i32)magnitudo(GabcNota), &c->numerus_notarum);
    c->vitia = (GabcVitium*)_tabula(&l, l.vitia,
        (i32)magnitudo(GabcVitium), &c->numerus_vitiorum);
    si (l.deficit)
    {
        redde NIHIL;
    }
    _altitudinem_resolvere(c);
    redde c;
}

b32
gabc_successus (
    constans GabcCantus* cantus)
{
    i32 i;

    si (cantus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < cantus->numerus_vitiorum; i++)
    {
        si (cantus->vitia[i].gravitas == GABC_VITIUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Scriptura
 * ================================================== */

nomen structura {
    ChordaAedificator* a;
                  b32  bene;
} Scriptor;

interior vacuum
_lit (
               Scriptor* s,
     constans character* t)
{
    si (!chorda_aedificator_appendere_literis(s->a, t))
    {
        s->bene = FALSUM;
    }
}

interior vacuum
_num (
    Scriptor* s,
         s32  n)
{
    si (!chorda_aedificator_appendere_s32(s->a, n))
    {
        s->bene = FALSUM;
    }
}

interior vacuum
_characterem (
     Scriptor* s,
    character  ch)
{
    si (!chorda_aedificator_appendere_character(s->a, ch))
    {
        s->bene = FALSUM;
    }
}

/* textus cum \t \n \r \\ effugitis */
interior vacuum
_effugere (
     Scriptor* s,
       chorda  ch)
{
    i32 i;

    per (i = ZEPHYRUM; i < ch.mensura; i++)
    {
        commutatio (ch.datum[i])
        {
            casus '\t': _lit(s, "\\t"); frange;
            casus '\n': _lit(s, "\\n"); frange;
            casus '\r': _lit(s, "\\r"); frange;
            casus '\\': _lit(s, "\\\\"); frange;
            ordinarius: _characterem(s, (character)ch.datum[i]); frange;
        }
    }
}

interior character
_littera_positionis (
    s32 positio)
{
    si (positio < ZEPHYRUM)
    {
        redde '-';
    }
    redde positio >= POSITIO_P ? 'p' : (character)('a' + positio);
}

interior constans character*
_divisionis_titulus (
    GabcDivisio divisio)
{
    commutatio (divisio)
    {
        casus GABC_DIVISIO_VIRGULA:        redde "virgula";
        casus GABC_DIVISIO_MINIMIS:        redde "minimis";
        casus GABC_DIVISIO_MINIMA:         redde "minima";
        casus GABC_DIVISIO_MINOR:          redde "minor";
        casus GABC_DIVISIO_MAIOR:          redde "maior";
        casus GABC_DIVISIO_MAIOR_PUNCTATA: redde "maior_punctata";
        casus GABC_DIVISIO_FINALIS:        redde "finalis";
        casus GABC_DIVISIO_DOMINICANA:     redde "dominicana";
    }
    redde "?";
}

interior constans character*
_spatii_titulus (
    GabcSpatium spatium)
{
    commutatio (spatium)
    {
        casus GABC_SPATIUM_NULLUM:        redde "nullum";
        casus GABC_SPATIUM_DIMIDIUM:      redde "dimidium";
        casus GABC_SPATIUM_NEUMATICUM:    redde "neumaticum";
        casus GABC_SPATIUM_MAGNUM:        redde "magnum";
        casus GABC_SPATIUM_GLYPHAE:       redde "glyphae";
        casus GABC_SPATIUM_FACTOR:        redde "factor";
        casus GABC_SPATIUM_INSEPARABILE:  redde "inseparabile";
    }
    redde "?";
}

interior constans character*
_alterationis_titulus (
    GabcAlteratioGenus alteratio)
{
    commutatio (alteratio)
    {
        casus GABC_ALTERATIO_BEMOLLE:  redde "bemolle";
        casus GABC_ALTERATIO_NATURALE: redde "naturale";
        casus GABC_ALTERATIO_DIESIS:   redde "diesis";
    }
    redde "?";
}

/* formae: littera una per bit, ordine bitorum (cf. probatio_gabc.c) */
interior vacuum
_formas_scribere (
    Scriptor* s,
         i32  formae)
{
    constans character* litterae = "IvVsoOwWrR=q~<>-@";
                   i32  i;

    si (formae == ZEPHYRUM)
    {
        _characterem(s, '-');
        redde;
    }
    per (i = ZEPHYRUM; litterae[i] != '\0'; i++)
    {
        si ((formae & ((i32)I << i)) != ZEPHYRUM)
        {
            _characterem(s, litterae[i]);
        }
    }
}

interior vacuum
_notam_scribere (
             Scriptor* s,
    constans GabcNota* n)
{
    _lit(s, "nota\t");
    _characterem(s, _littera_positionis((s32)n->positio));
    _characterem(s, '\t');
    _num(s, n->gradus);
    _characterem(s, '\t');
    _num(s, n->alteratio);
    _characterem(s, '\t');
    _formas_scribere(s, n->formae);
    _characterem(s, '\t');
    _num(s, (s32)n->mora);
    _characterem(s, '\t');
    _num(s, (s32)n->episema);
    _characterem(s, '\t');
    _num(s, (s32)n->ictus);
    _characterem(s, '\t');
    _num(s, (s32)n->linea);
    _characterem(s, ':');
    _num(s, (s32)n->columna);
    _characterem(s, '\n');
}

interior vacuum
_elementum_scribere (
                  Scriptor* s,
       constans GabcCantus* c,
    constans GabcElementum* el)
{
           i32 k;
    GabcFigura figura;

    commutatio (el->genus)
    {
        casus GABC_EL_NEUMA:
            figura = gabc_figura(c, el);
            _lit(s, "neuma\t");
            _num(s, (s32)el->prima);
            _characterem(s, '\t');
            _num(s, (s32)el->numerus);
            _characterem(s, '\t');
            _lit(s, figura == GABC_FIGURA_IGNOTA ? "-"
                : gabc_figurae_titulus(figura));
            _characterem(s, '\n');
            per (k = el->prima; k < el->prima + el->numerus; k++)
            {
                _notam_scribere(s, &c->notae[k]);
            }
            frange;
        casus GABC_EL_ALTERATIO:
            _lit(s, "alteratio\t");
            _lit(s, _alterationis_titulus(el->alteratio));
            _characterem(s, '\t');
            _characterem(s, _littera_positionis(el->positio));
            _lit(s, el->molle ? "\t1" : "\t0");
            _lit(s, el->parenthesis ? "\t1\n" : "\t0\n");
            frange;
        casus GABC_EL_CLAVIS:
            _lit(s, "clavis\t");
            _characterem(s, el->clavis);
            _characterem(s, '\t');
            _num(s, (s32)el->clavis_linea);
            _lit(s, el->clavis_bemolle ? "\t1\n" : "\t0\n");
            frange;
        casus GABC_EL_DIVISIO:
            _lit(s, "divisio\t");
            _lit(s, _divisionis_titulus(el->divisio));
            _characterem(s, '\t');
            _num(s, (s32)el->divisio_numerus);
            _lit(s, el->alta ? "\t1" : "\t0");
            _lit(s, el->parenthesis ? "\t1\n" : "\t0\n");
            frange;
        casus GABC_EL_SPATIUM:
            _lit(s, "spatium\t");
            _lit(s, _spatii_titulus(el->spatium));
            _characterem(s, '\t');
            si (el->factor.mensura > ZEPHYRUM)
            {
                _effugere(s, el->factor);
            }
            alioquin
            {
                _characterem(s, '-');
            }
            _characterem(s, '\n');
            frange;
        casus GABC_EL_FRACTIO:
            _lit(s, el->iustificata ? "fractio\t1\t" : "fractio\t0\t");
            _num(s, el->custos);
            _characterem(s, '\n');
            frange;
        casus GABC_EL_CUSTOS:
            _lit(s, "custos\t");
            _characterem(s, _littera_positionis(el->positio));
            _characterem(s, '\n');
            frange;
        casus GABC_EL_SIGNUM_OPACUM:
            _lit(s, "signum\t");
            _effugere(s, el->textus);
            _characterem(s, '\t');
            _num(s, el->nota);
            _characterem(s, '\n');
            frange;
    }
}

/* bits ut litterae (ordine bitorum); '-' si nulla */
interior vacuum
_bits_scribere (
               Scriptor* s,
                    i32  bits,
     constans character* litterae)
{
    i32 i;

    si (bits == ZEPHYRUM)
    {
        _characterem(s, '-');
        redde;
    }
    per (i = ZEPHYRUM; litterae[i] != '\0'; i++)
    {
        si ((bits & ((i32)I << i)) != ZEPHYRUM)
        {
            _characterem(s, litterae[i]);
        }
    }
}

interior constans character*
_textus_genus_titulus (
    GabcTextusGenus genus)
{
    commutatio (genus)
    {
        casus GABC_TEXTUS_CANTATUS:   redde "cantatus";
        casus GABC_TEXTUS_TRANSLATIO: redde "translatio";
        casus GABC_TEXTUS_SUPRA:      redde "supra";
        casus GABC_TEXTUS_VERBATIM:   redde "verbatim";
    }
    redde "?";
}

/* cursus (si syllaba ornata), vocalis, signa - cf. probatio_gabc.c */
interior vacuum
_ornatum_scribere (
                Scriptor* s,
    constans GabcSyllaba* syllaba)
{
    b32 ornata = syllaba->numerus_textuum > I;
    i32 r;

    per (r = ZEPHYRUM; r < syllaba->numerus_textuum; r++)
    {
        si (   syllaba->textus[r].genus  != GABC_TEXTUS_CANTATUS
            || syllaba->textus[r].stilus != ZEPHYRUM)
        {
            ornata = VERUM;
        }
    }
    per (r = ZEPHYRUM; ornata && r < syllaba->numerus_textuum; r++)
    {
        _lit(s, "textus\t");
        _lit(s, _textus_genus_titulus(syllaba->textus[r].genus));
        _characterem(s, '\t');
        _bits_scribere(s, syllaba->textus[r].stilus, "ibucstep");
        _characterem(s, '\t');
        _effugere(s, syllaba->textus[r].textus);
        _characterem(s, '\n');
    }
    si (syllaba->vocalis_initium >= ZEPHYRUM)
    {
        _lit(s, "vocalis\t");
        _num(s, syllaba->vocalis_initium);
        _characterem(s, '\t');
        _num(s, syllaba->vocalis_finis);
        _characterem(s, '\n');
    }
    si (syllaba->signa != ZEPHYRUM)
    {
        _lit(s, "signa\t");
        _bits_scribere(s, syllaba->signa, "nNeEp");
        _characterem(s, '\n');
    }
}

b32
gabc_tabulam_scribere (
           constans GabcCantus* cantus,
             ChordaAedificator* exitus)
{
                Scriptor  s;
                     i32  i;
                     i32  e;
                     i32  r;
    constans GabcSyllaba* syllaba;

    si (cantus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    s.a     = exitus;
    s.bene  = VERUM;
    per (i = ZEPHYRUM; i < cantus->numerus_capitum; i++)
    {
        _lit(&s, "caput\t");
        _effugere(&s, cantus->capita[i].clavis);
        _characterem(&s, '\t');
        _effugere(&s, cantus->capita[i].valor);
        _characterem(&s, '\n');
    }
    per (i = ZEPHYRUM; i < cantus->numerus_syllabarum; i++)
    {
        syllaba = &cantus->syllabae[i];
        _lit(&s, "syllaba\t");
        _num(&s, (s32)i);
        _lit(&s, syllaba->verbum_incipit ? "\t1\t" : "\t0\t");
        per (r = ZEPHYRUM; r < syllaba->numerus_textuum; r++)
        {
            si (syllaba->textus[r].genus == GABC_TEXTUS_CANTATUS)
            {
                _effugere(&s, syllaba->textus[r].textus);
            }
        }
        _characterem(&s, '\n');
        _ornatum_scribere(&s, syllaba);
        per (e = ZEPHYRUM; e < syllaba->numerus_elementorum; e++)
        {
            _elementum_scribere(&s, cantus, &syllaba->elementa[e]);
        }
    }
    redde s.bene;
}

b32
gabc_vitia_scribere (
           constans GabcCantus* cantus,
            constans character* via,
             ChordaAedificator* exitus)
{
               Scriptor  s;
                    i32  i;
    constans GabcVitium* v;

    si (cantus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    s.a     = exitus;
    s.bene  = VERUM;
    per (i = ZEPHYRUM; i < cantus->numerus_vitiorum; i++)
    {
        v = &cantus->vitia[i];
        _lit(&s, via != NIHIL ? via : "-");
        _characterem(&s, ':');
        _num(&s, (s32)v->linea);
        _characterem(&s, ':');
        _num(&s, (s32)v->columna);
        _lit(&s, v->gravitas == GABC_MONITUM ? ": monitum: "
            : ": vitium: ");
        _effugere(&s, v->causa);
        _characterem(&s, '\n');
    }
    redde s.bene;
}


/* ==================================================
 * Figurae (T5)
 *
 * Neuma = notae inter spatia (elementum unum). Figura ex SIGNATURA:
 * classis cuiusque notae, et inter vicinas intervallum ('+' ascendit,
 * '-' descendit, '=' unisonus): "p+p-p" torculus est. Regulae ex
 * Gregorio (src/gabc/gabc-glyphs-determination.c, lectum 2026-11-03):
 * nota initio debilis glypham novam incipit, liquescens (~ < >)
 * claudit - ergo debilis nisi in prima et liquescens nisi in ultima
 * figuram nominatam nullam dant; unisonus puncta separat. Compositae
 * quas Gregorius in glyphas plures dividit (climacus, pes subbipunctis,
 * salicus, scandicus flexus, pes subtripunctis, pressus) ex
 * nomenclatura Solesmensi. Ancus = clivis + nota inferior deminuta
 * (Gregorius G_ANCUS). Quod tabula non novit
 * IGNOTA est - numquam nomen falsum.
 *
 * CLASSES: p punctum (cavum quoque), i inclinatum, v virga, r virga
 * reversa, s stropha, o oriscus (o O), w quilisma (w W), q quadratum,
 * x aliud (linea, linea punctum, formae plures simul).
 * ================================================== */

#define FORMAE_LIQUESCENTES (GABC_FORMA_LIQUESCENS \
    | GABC_FORMA_AUCTA_ASCENDENS | GABC_FORMA_AUCTA_DESCENDENS)

nomen structura {
     constans character* signatura; /* '*': par ultimum iteratur */
                    b32  debilis;   /* prima initio debilis licet */
                    b32  deminuta;  /* ultima '~' requiritur */
             GabcFigura  figura;
} RegulaFigurae;

/* ordo valet: regula prima quae congruit vincit (deminuta ante
 * simplicem) */
hic_manens constans RegulaFigurae REGULAE_FIGURARUM[] = {
    { "p",         FALSUM, FALSUM, GABC_FIGURA_PUNCTUM },
    { "i",         FALSUM, FALSUM, GABC_FIGURA_PUNCTUM_INCLINATUM },
    { "v",         FALSUM, FALSUM, GABC_FIGURA_VIRGA },
    { "r",         FALSUM, FALSUM, GABC_FIGURA_VIRGA_REVERSA },
    { "s",         FALSUM, FALSUM, GABC_FIGURA_STROPHA },
    { "o",         FALSUM, FALSUM, GABC_FIGURA_ORISCUS },
    { "v=v",       FALSUM, FALSUM, GABC_FIGURA_BIVIRGA },
    { "v=v=v",     FALSUM, FALSUM, GABC_FIGURA_TRIVIRGA },
    { "s=s",       FALSUM, FALSUM, GABC_FIGURA_DISTROPHA },
    { "s=s=s",     FALSUM, FALSUM, GABC_FIGURA_TRISTROPHA },
    { "p+p",       VERUM,  VERUM,  GABC_FIGURA_EPIPHONUS },
    { "p+p",       VERUM,  FALSUM, GABC_FIGURA_PES },
    { "q+p",       FALSUM, FALSUM, GABC_FIGURA_PES_QUADRATUS },
    { "w+p",       FALSUM, FALSUM, GABC_FIGURA_PES_QUILISMATICUS },
    { "o+p",       FALSUM, FALSUM, GABC_FIGURA_PES_QUASSUS },
    { "p-p",       VERUM,  VERUM,  GABC_FIGURA_CEPHALICUS },
    { "p-p",       VERUM,  FALSUM, GABC_FIGURA_CLIVIS },
    { "p+p-p",     VERUM,  FALSUM, GABC_FIGURA_TORCULUS },
    { "w+p-p",     FALSUM, FALSUM, GABC_FIGURA_TORCULUS_QUILISMATICUS },
    { "p+p-p+p",   VERUM,  FALSUM, GABC_FIGURA_TORCULUS_RESUPINUS },
    { "p+p-p+p-p", VERUM,  FALSUM,
      GABC_FIGURA_TORCULUS_RESUPINUS_FLEXUS },
    { "p-p+p",     FALSUM, FALSUM, GABC_FIGURA_PORRECTUS },
    { "p-p+p-p",   FALSUM, FALSUM, GABC_FIGURA_PORRECTUS_FLEXUS },
    { "p+p+p",     FALSUM, FALSUM, GABC_FIGURA_SCANDICUS },
    { "p+w+p",     FALSUM, FALSUM,
      GABC_FIGURA_SCANDICUS_QUILISMATICUS },
    { "p+p+p-p",   FALSUM, FALSUM, GABC_FIGURA_SCANDICUS_FLEXUS },
    { "p+o+p",     FALSUM, FALSUM, GABC_FIGURA_SALICUS },
    { "p+o+p-p",   FALSUM, FALSUM, GABC_FIGURA_SALICUS_FLEXUS },
    { "p-i-i*",    FALSUM, FALSUM, GABC_FIGURA_CLIMACUS },
    { "v-i-i*",    FALSUM, FALSUM, GABC_FIGURA_CLIMACUS },
    { "p-p-p",     VERUM,  VERUM,  GABC_FIGURA_ANCUS },
    { "p+p-i-i",   VERUM,  FALSUM, GABC_FIGURA_PES_SUBBIPUNCTIS },
    { "p+p-i-i-i", VERUM,  FALSUM, GABC_FIGURA_PES_SUBTRIPUNCTIS },
    { "p=p-p",     FALSUM, FALSUM, GABC_FIGURA_PRESSUS },
    { "p=o-p",     FALSUM, FALSUM, GABC_FIGURA_PRESSUS }
};

#define NUMERUS_REGULARUM_FIGURARUM \
    ((i32)(magnitudo(REGULAE_FIGURARUM) \
        / magnitudo(REGULAE_FIGURARUM[ZEPHYRUM])))

/* titulus per GabcFigura, ordine enumerationis */
hic_manens constans character* TITULI_FIGURARUM[] = {
    "ignota", "punctum", "punctum inclinatum", "virga", "virga reversa",
    "stropha", "oriscus", "bivirga", "trivirga", "distropha",
    "tristropha", "pes", "pes quadratus", "pes quilismaticus",
    "pes quassus", "epiphonus", "clivis", "cephalicus", "torculus",
    "torculus quilismaticus", "torculus resupinus",
    "torculus resupinus flexus", "porrectus", "porrectus flexus",
    "scandicus", "scandicus quilismaticus", "scandicus flexus",
    "salicus", "salicus flexus", "climacus", "ancus",
    "pes subbipunctis", "pes subtripunctis", "pressus"
};

interior character
_classis_notae (
    constans GabcNota* nota)
{
          i32 formae   = nota->formae;
          i32 genera   = ZEPHYRUM;
    character classis  = 'p';

    si (formae & GABC_FORMA_INCLINATUM)
    {
        genera++;
        classis = 'i';
    }
    si (formae & GABC_FORMA_VIRGA)
    {
        genera++;
        classis = 'v';
    }
    si (formae & GABC_FORMA_VIRGA_REVERSA)
    {
        genera++;
        classis = 'r';
    }
    si (formae & GABC_FORMA_STROPHA)
    {
        genera++;
        classis = 's';
    }
    si (formae & (GABC_FORMA_ORISCUS | GABC_FORMA_ORISCUS_SCAPUS))
    {
        genera++;
        classis = 'o';
    }
    si (formae & (GABC_FORMA_QUILISMA | GABC_FORMA_QUILISMA_QUADRATUM))
    {
        genera++;
        classis = 'w';
    }
    si (formae & GABC_FORMA_QUADRATUM)
    {
        genera++;
        classis = 'q';
    }
    si (formae & (GABC_FORMA_LINEA | GABC_FORMA_LINEA_PUNCTUM))
    {
        genera++;
        classis = 'x';
    }
    redde genera > I ? 'x' : classis;
}

interior character
_intervallum (
    constans GabcNota* prior,
    constans GabcNota* nota)
{
    si (nota->positio > prior->positio)
    {
        redde '+';
    }
    si (nota->positio < prior->positio)
    {
        redde '-';
    }
    redde '=';
}

/* signatura notas omnes tegit? classis prima, deinde paria
 * (intervallum, classis); '*' in fine par ultimum iterat */
interior b32
_signaturam_aequat (
     constans character* signatura,
      constans GabcNota* notae,
                    i32  numerus)
{
    i32 i;
    i32 j;

    si (signatura[ZEPHYRUM] != _classis_notae(&notae[ZEPHYRUM]))
    {
        redde FALSUM;
    }
    i = I;
    per (j = I; j < numerus; j++)
    {
        si (signatura[i] == '*')
        {
            i -= II;
        }
        si (   signatura[i] == '\0'
            || signatura[i] != _intervallum(&notae[j - I], &notae[j])
            || signatura[i + I] != _classis_notae(&notae[j]))
        {
            redde FALSUM;
        }
        i += II;
    }
    redde signatura[i] == '\0' || signatura[i] == '*';
}

GabcFigura
gabc_figura (
       constans GabcCantus* cantus,
    constans GabcElementum* neuma)
{
     constans GabcNota* notae;
                   i32  numerus;
                   i32  j;
                   i32  k;

    si (   cantus         == NIHIL || neuma == NIHIL
        || neuma->genus   != GABC_EL_NEUMA
        || neuma->numerus == ZEPHYRUM
        || neuma->prima > cantus->numerus_notarum
        || neuma->numerus > cantus->numerus_notarum - neuma->prima)
    {
        redde GABC_FIGURA_IGNOTA;
    }
    notae    = &cantus->notae[neuma->prima];
    numerus  = neuma->numerus;

    /* Gregorius: debilis incipit, liquescens claudit glypham */
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        si (   (j > ZEPHYRUM
            && (notae[j].formae & GABC_FORMA_INITIO_DEBILIS))
            || (j + I < numerus
                && (notae[j].formae & FORMAE_LIQUESCENTES)))
        {
            redde GABC_FIGURA_IGNOTA;
        }
    }

    per (k = ZEPHYRUM; k < NUMERUS_REGULARUM_FIGURARUM; k++)
    {
        constans RegulaFigurae* r = &REGULAE_FIGURARUM[k];

        si (   (notae[ZEPHYRUM].formae & GABC_FORMA_INITIO_DEBILIS)
            && !r->debilis)
        {
            perge;
        }
        si (   r->deminuta
            && !(notae[numerus - I].formae & GABC_FORMA_LIQUESCENS))
        {
            perge;
        }
        si (_signaturam_aequat(r->signatura, notae, numerus))
        {
            redde r->figura;
        }
    }
    redde GABC_FIGURA_IGNOTA;
}

constans character*
gabc_figurae_titulus (
    GabcFigura figura)
{
    i32 index = (i32)figura;

    si (index < (i32)(magnitudo(TITULI_FIGURARUM)
                      / magnitudo(TITULI_FIGURARUM[0])))
    {
        redde TITULI_FIGURARUM[index];
    }
    redde "ignota";
}
