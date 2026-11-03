/* gabc.c - lector gabc (caput include/gabc.h)
 *
 * Scrutator manu scriptus super octetos (gabc non XML est). Tres
 * gradus: (1) caput usque ad lineam '%%'; (2) corpus: textus usque ad
 * '(', notae usque ad ')' -> syllabae cum elementis planis, notae in
 * tabula una; (3) gradus resolvuntur, clave in vigore per ordinem
 * fontis. Grammatica notarum = lexicon Gregorii
 * (gabc-notes-determination.l): omnia signa eius hic consumuntur.
 *
 * T2 (nucleus): notae, neumae, claves, divisiones, alterationes ut
 * elementa. Signa formae et rhythmi (T3), spatia, fractiones,
 * custodes, uncini, nabc (T4) consumuntur sed NONDUM servantur - G5
 * per T4 restituitur; donec, tabula ea non monstrat.
 */

#include "gabc.h"
#include "xar.h"
#include <string.h>

/* nulla clavis ante notas: c3 (Gregorii 'initial-key' vetus) - do in
 * positione VII (h) */
#define DO_ORDINARIUM ((s32)VII)
#define POSITIO_P     ((s32)XIV)


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
    redde !(j + I < b && (l->datum[j + I] == '+'
        || _alteratio_est(l->datum[j + I])));
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
    si (   ch          >= 'A' && ch <= 'P' && k < b
        && l->datum[k] >= '0' && l->datum[k] <= '2')
    {
        nota->directio = (s8)(l->datum[k] - '0');
        k++;
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

/* spatium '/' ad j (etiam post '!'): / // /0 /! /[n]; longitudo
 * (T4 servabit) */
interior i32
_spatii_longitudo (
    constans Lector* l,
                i32  j,
                i32  b)
{
    i32 k = j + I;

    si (k >= b)
    {
        redde I;
    }
    si (l->datum[k] == '0' || l->datum[k] == '!')
    {
        redde II;
    }
    si (l->datum[k] == '/')
    {
        /* "//[" = '/' deinde "/[n]" */
        redde (k + I < b && l->datum[k + I] == '[') ? I : II;
    }
    si (l->datum[k] == '[')
    {
        dum (k < b && l->datum[k] != ']')
        {
            k++;
        }
        redde (k < b ? k + I : b) - j;
    }
    redde I;
}

/* signum modificans post notam (T3 servabit); longitudo, 0 si non */
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

/* notae inter '(' et ')': [a, b) -> elementa syllabae currentis */
interior vacuum
_notas_legere (
    Lector* l,
       i32  a,
       i32  b)
{
    i32 j      = a;
    s32 neuma  = -I;
    b32 nabc   = FALSUM;
    i32 lon;
    i32 k;
     i8 ch;

    dum (j < b)
    {
        ch = l->datum[j];
        si (nabc)
        {
            /* nabc (T4): usque ad '|' proximum */
            nabc = ch != '|';
            j++;
            perge;
        }
        commutatio (ch)
        {
            casus ' ':
                neuma = -I;
                j++;
                frange;
            casus '\t': casus '\r': casus '\n':
                j++;
                frange;
            casus '/':
                neuma = -I;
                j += _spatii_longitudo(l, j, b);
                frange;
            casus '!':
                neuma = -I;
                si (j + I < b && l->datum[j + I] == '/')
                {
                    j += I + _spatii_longitudo(l, j + I, b);
                }
                alioquin
                {
                    j += (j + I < b && l->datum[j + I] == ' ') ? II : I;
                }
                frange;
            casus '`': casus '^': casus ',': casus ';': casus ':':
                j += _divisionem_legere(l, j, b, neuma >= ZEPHYRUM);
                neuma = -I;
                frange;
            casus '[':
                si (j + I < b && l->datum[j + I] == '[')
                {
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
                }
                alioquin
                {
                    j = k + I;
                }
                frange;
            casus ']':
                si (j + I < b && l->datum[j + I] == ']')
                {
                    j += II;
                }
                alioquin
                {
                    _vitium(l, GABC_VITIUM, GABC_NOTA_IGNOTA, j,
                        "character in notis ignotus");
                    j++;
                }
                frange;
            casus '{': casus '}':
                neuma = -I;
                j++;
                frange;
            casus '|':
                neuma = -I;
                nabc = VERUM;
                j++;
                frange;
            casus 'z': casus 'Z':
                neuma = -I;
                j++;
                si (j < b && (l->datum[j] == '+' || l->datum[j] == '-'
                    || (ch == 'z' && l->datum[j] == '0')))
                {
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
                    j += II;
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
                    alioquin si (j + I < b && l->datum[j + I] == '+')
                    {
                        /* custos manualis (T4) */
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
                lon = _modificatoris_longitudo(l, j, b);
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
 * Corpus
 * ================================================== */

/* textus usque ad '(', notae usque ad ')'. verbum_incipit: textus
 * adest et (primus textus aut spatium post textum priorem). */
interior vacuum
_corpus_legere (
    Lector* l,
       i32  initium)
{
            i32  i             = initium;
            b32  spatium       = VERUM;
            b32  textus_visus  = FALSUM;
            i32  ta;
            i32  tb;
            i32  clausa;
    GabcSyllaba* s;
     GabcTextus* t;

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

        ta = i;
        dum (i < l->mensura && l->datum[i] != '(')
        {
            i++;
        }
        tb = i;

        s = (GabcSyllaba*)xar_addere(l->syllabae);
        si (s == NIHIL)
        {
            l->deficit = VERUM;
            redde;
        }
        memset(s, ZEPHYRUM, magnitudo(GabcSyllaba));
        s->vocalis_initium  = -I;
        s->vocalis_finis    = -I;
        _locus(l, ta, &s->linea, &s->columna);
        si (tb > ta)
        {
            t = (GabcTextus*)piscina_allocare(l->piscina,
                (memoriae_index)magnitudo(GabcTextus));
            t->genus            = GABC_TEXTUS_CANTATUS;
            t->textus           = _sectio(l, ta, tb);
            t->stilus           = ZEPHYRUM;
            s->textus           = t;
            s->numerus_textuum  = I;
            s->verbum_incipit   = spatium || !textus_visus;
            textus_visus        = VERUM;
            spatium             = FALSUM;
        }

        xar_vacare(l->elementa);
        si (i < l->mensura)
        {
            clausa = i + I;
            dum (clausa < l->mensura && l->datum[clausa] != ')')
            {
                clausa++;
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

/* gradus a do: clavis in vigore ordine fontis (secunda non mutat) */
interior vacuum
_gradus_resolvere (
    GabcCantus* c)
{
                       s32  locus_do = DO_ORDINARIUM;
                       i32  s;
                       i32  e;
                       i32  k;
    constans GabcElementum* el;

    per (s = ZEPHYRUM; s < c->numerus_syllabarum; s++)
    {
        per (e = ZEPHYRUM; e < c->syllabae[s].numerus_elementorum; e++)
        {
            el = &c->syllabae[s].elementa[e];
            si (el->genus == GABC_EL_CLAVIS && !el->clavis_secunda)
            {
                locus_do = (s32)(II * (i32)el->clavis_linea + I)
                    - (el->clavis == 'f' ? (s32)III : ZEPHYRUM);
            }
            alioquin si (el->genus == GABC_EL_NEUMA)
            {
                per (k = el->prima; k < el->prima + el->numerus; k++)
                {
                    c->notae[k].gradus = (s32)c->notae[k].positio
                        - locus_do;
                }
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

    l.piscina   = piscina;
    l.datum     = datum.datum;
    l.mensura   = datum.datum != NIHIL ? datum.mensura : ZEPHYRUM;
    l.cantus    = c;
    l.initia    = xar_creare(piscina, (i32)magnitudo(i32));
    l.vitia     = xar_creare(piscina, (i32)magnitudo(GabcVitium));
    l.capita    = xar_creare(piscina, (i32)magnitudo(GabcCaput));
    l.syllabae  = xar_creare(piscina, (i32)magnitudo(GabcSyllaba));
    l.notae     = xar_creare(piscina, (i32)magnitudo(GabcNota));
    l.elementa  = xar_creare(piscina, (i32)magnitudo(GabcElementum));
    si (   l.initia   == NIHIL || l.vitia == NIHIL || l.capita == NIHIL
        || l.syllabae == NIHIL || l.notae == NIHIL
        || l.elementa == NIHIL)
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
    _gradus_resolvere(c);
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
 * ================================================== */

GabcFigura
gabc_figura (
       constans GabcCantus* cantus,
    constans GabcElementum* neuma)
{
    (vacuum)cantus;
    (vacuum)neuma;
    redde GABC_FIGURA_IGNOTA;
}

constans character*
gabc_figurae_titulus (
    GabcFigura figura)
{
    (vacuum)figura;
    redde "ignota";
}
