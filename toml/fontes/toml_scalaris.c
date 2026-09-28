/* toml_scalaris.c - Coctio scalarium toml (vide toml_scalaris.h)
 *
 * Lectiones purae (numerus, tempus) vitium PRIMUM reddunt - lexema
 * atomum est, vitium secundum eiusdem lexematis ruina primi plerumque.
 * Chordae et commentaria OMNE vitium reddunt: effugia et octeti
 * independentes sunt ('\q ... \z' duo errata auctoris).
 *
 * Sedes: offset vitii intra lexema -> tractus in fonte (linea et
 * columna per octetos lexematis promotae, ut lector STML facit).
 * "Post X deest" ad X ponitur; "deest in fine" punctum est in fine.
 */

#include "toml_scalaris.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 2^63: limes magnitudinis integri negativi; positivi limes - 1 */
#define TOML_LIMES_S64  ((i64)I << LXIII)

/* excerptum lexematis in causa: octeti ad XL */
#define TOML_EXCERPTUM  XL


/* ==================================================
 * Diagnostica
 * ================================================== */

/* tractus octetorum [ab, ad) lexematis 't' */
interior vacuum
_tractus (
    constans MateriaToken* t,
                      s32  ab,
                      s32  ad,
           MateriaTractus* tractus)
{
    constans i8* s        = (constans i8*)t->valor.datum;
            i32  linea    = t->linea;
            i32  columna  = t->columna;
            s32  k;

    memset(tractus, ZEPHYRUM, magnitudo(MateriaTractus));
    tractus->fons_index  = t->fons_index;
    tractus->est_fons    = VERUM;
    tractus->initium    = t->byte_offset < ZEPHYRUM ? (s32)-I
        : t->byte_offset + ab;
    tractus->finis      = t->byte_offset < ZEPHYRUM ? (s32)-I
        : t->byte_offset + ad;
    per (k = ZEPHYRUM; k <= ad; k++)
    {
        si (k == ab)
        {
            tractus->linea    = linea;
            tractus->columna  = columna;
        }
        si (k == ad)
        {
            tractus->linea_finis    = linea;
            tractus->columna_finis  = columna;
            frange;
        }
        si (k < (s32)t->valor.mensura && s[k] == '\n')
        {
            linea++;
            columna = I;
        }
        alioquin
        {
            columna++;
        }
    }
}

interior vacuum
_diagnosticum (
                      Xar* diagnostica,
    constans MateriaToken* t,
    constans MateriaNodus* nodus,
                      s32  ab,
                      s32  ad,
       constans character* codex,
       constans character* causa)
{
    MateriaDiagnosticum* d;

    si (diagnostica == NIHIL)
    {
        redde;
    }
    d = (MateriaDiagnosticum*)xar_addere(diagnostica);
    si (d == NIHIL)
    {
        redde;
    }
    memset(d, ZEPHYRUM, magnitudo(MateriaDiagnosticum));
    d->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex     = codex;
    d->causa     = causa;
    d->nodus     = nodus;
    _tractus(t, ab, ad, &d->tractus);
}

interior character*
_alveus (
    Piscina* piscina)
{
    redde (character*)piscina_allocare(piscina, (i64)CCLVI);
}

/* vitium lectionis purae -> diagnosticum cum textu lexematis */
interior vacuum
_vitium_nuntiare (
                       Piscina* piscina,
                           Xar* diagnostica,
         constans MateriaToken* t,
         constans MateriaNodus* nodus,
    constans TomlVitiumScalare* v)
{
             character* b;
    constans character* forma;
               integer  m = (integer)t->valor.mensura;

    si (diagnostica == NIHIL || v->codex == NIHIL)
    {
        redde;
    }
    b = _alveus(piscina);
    si (b == NIHIL)
    {
        redde;
    }
    si (m > (integer)TOML_EXCERPTUM)
    {
        m = (integer)TOML_EXCERPTUM;
    }
    si (strcmp(v->codex, TOML_CODEX_INTEGER) == ZEPHYRUM)
    {
        forma = "integer pravum '%.*s': %s";
    }
    alioquin si (strcmp(v->codex, TOML_CODEX_FLUITANS) == ZEPHYRUM)
    {
        forma = "fluitans pravum '%.*s': %s";
    }
    alioquin si (strcmp(v->codex, TOML_CODEX_EXTRA_FINES) == ZEPHYRUM)
    {
        forma = "integer '%.*s' %s";
    }
    alioquin si (strcmp(v->codex, TOML_CODEX_TEMPUS_FORMA) == ZEPHYRUM)
    {
        forma = "tempus pravum '%.*s': %s";
    }
    alioquin
    {
        forma = "tempus '%.*s': %s";
    }
    sprintf(b, forma, m, (constans character*)t->valor.datum, v->causa);
    _diagnosticum(diagnostica, t, nodus, v->initium, v->finis, v->codex,
        b);
}

interior b32
_vitium (
     TomlVitiumScalare* v,
    constans character* codex,
    constans character* causa,
                   s32  ab,
                   s32  ad)
{
    v->codex    = codex;
    v->causa    = causa;
    v->initium  = ab;
    v->finis    = ad;
    redde FALSUM;
}

/* "hic exspectatur": octetus j, aut punctum in fine */
interior b32
_vitium_ad (
     TomlVitiumScalare* v,
    constans character* codex,
    constans character* causa,
                   s32  j,
                   s32  n)
{
    redde _vitium(v, codex, causa, j, j < n ? j + I : n);
}


/* ==================================================
 * Octeti
 * ================================================== */

interior b32
_digitus (
    s32 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_digitus_basis (
    s32  c,
    s32  basis,
    s32* valor)
{
    s32 d = (s32)-I;

    si (c >= '0' && c <= '9')
    {
        d = c - '0';
    }
    alioquin si (c >= 'a' && c <= 'f')
    {
        d = c - 'a' + X;
    }
    alioquin si (c >= 'A' && c <= 'F')
    {
        d = c - 'A' + X;
    }
    si (d < ZEPHYRUM || d >= basis)
    {
        redde FALSUM;
    }
    si (valor != NIHIL)
    {
        *valor = d;
    }
    redde VERUM;
}

/* octetus moderans vetitus (TAB licet; LF/CRLF vocans iudicat) */
interior b32
_moderans (
    s32 c)
{
    redde (c < 0x20 && c != '\t') || c == 0x7F;
}

/* sequentia UTF-8 ab s[i] (i < ad): mensura valida, aut -mensura
 * pravae (octetus principalis + continuationes, ad III) */
interior s32
_utf8 (
    constans i8* s,
            s32  i,
            s32  ad)
{
    constans i8* p = s + i;
            s32  k = I;

    si (utf8_decodere(&p, s + ad) >= ZEPHYRUM)
    {
        redde (s32)(p - (s + i));
    }
    dum (k < IV && i + k < ad && utf8_est_continuatio(s[i + k]))
    {
        k++;
    }
    redde -k;
}


/* ==================================================
 * Chordae
 * ================================================== */

nomen structura {
                  Piscina* piscina;
                      Xar* diagnostica;
    constans MateriaToken* t;
    constans MateriaNodus* nodus;
              constans i8* s;
                      s32  ad;         /* finis contenti */
                      b32  multa;
                character* exitus;
                      s32  m;          /* octeti scripti */
                      b32  sanum;
} Decoctio;

interior vacuum
_culpa (
               Decoctio* dc,
                    s32  ab,
                    s32  ad,
     constans character* codex,
     constans character* causa)
{
    dc->sanum = FALSUM;
    _diagnosticum(dc->diagnostica, dc->t, dc->nodus, ab, ad, codex,
        causa);
}

/* effugium ab s[i] == '\\'; reddit indicem post id */
interior s32
_effugium (
    Decoctio* dc,
         s32  i)
{
    constans i8* s = dc->s;
            s32  e;
            s32  digiti;
            s32  k;
            s32  valor = ZEPHYRUM;

    si (i + I >= dc->ad)
    {
        _culpa(dc, i, i + I, TOML_CODEX_EFFUGIUM,
            "effugium imperfectum in fine chordae");
        redde dc->ad;
    }
    e = (s32)s[i + I];
    commutatio (e)
    {
        ordinarius:
            frange;
        casus 'b':  dc->exitus[dc->m++] = '\b';  redde i + II;
        casus 't':  dc->exitus[dc->m++] = '\t';  redde i + II;
        casus 'n':  dc->exitus[dc->m++] = '\n';  redde i + II;
        casus 'f':  dc->exitus[dc->m++] = '\f';  redde i + II;
        casus 'r':  dc->exitus[dc->m++] = '\r';  redde i + II;
        casus '"':  dc->exitus[dc->m++] = '"';   redde i + II;
        casus '\\': dc->exitus[dc->m++] = '\\';  redde i + II;
    }
    si (e == 'u' || e == 'U')
    {
        digiti  = e == 'u' ? IV : VIII;
        k       = ZEPHYRUM;
        dum (k < digiti && i + II + k < dc->ad)
        {
            s32 d;

            si (!_digitus_basis((s32)s[i + II + k], XVI, &d))
            {
                frange;
            }
            valor = valor * XVI + d;
            k++;
        }
        si (k < digiti)
        {
            _culpa(dc, i, i + II + k, TOML_CODEX_EFFUGIUM_MANCUM,
                e == 'u'
                ? "effugium '\\u' IV digitos hexadecimales poscit"
                : "effugium '\\U' VIII digitos hexadecimales poscit");
            redde i + II + k;
        }
        si (valor >= 0xD800 && valor <= 0xDFFF)
        {
            _culpa(dc, i, i + II + digiti, TOML_CODEX_PUNCTUM_CODICIS,
                "punctum codicis surrogatum (D800-DFFF) non est valor "
                "scalaris Unicode");
        }
        alioquin si (valor > 0x10FFFF || valor < ZEPHYRUM)
        {
            _culpa(dc, i, i + II + digiti, TOML_CODEX_PUNCTUM_CODICIS,
                "punctum codicis ultra U+10FFFF");
        }
        alioquin
        {
            dc->m += utf8_codere(valor, (i8*)dc->exitus + dc->m);
        }
        redde i + II + digiti;
    }
    /* '\' in fine lineae (chorda multa): spatia et lineae omittuntur */
    si (dc->multa && (e == ' ' || e == '\t' || e == '\n' || e == '\r'))
    {
        k = i + I;
        dum (k < dc->ad && (s[k] == ' ' || s[k] == '\t'))
        {
            k++;
        }
        si (   k < dc->ad
            && (s[k] == '\n'
                || (s[k] == '\r' && k + I < dc->ad
                    && s[k + I] == '\n')))
        {
            dum (k < dc->ad)
            {
                si (s[k] == ' ' || s[k] == '\t' || s[k] == '\n')
                {
                    k++;
                }
                alioquin si (   s[k]     == '\r' && k + I < dc->ad
                             && s[k + I] == '\n')
                {
                    k += II;
                }
                alioquin
                {
                    frange;
                }
            }
            redde k;
        }
        _culpa(dc, i, i + II, TOML_CODEX_EFFUGIUM,
            "effugium ignotum '\\ ': post '\\' spatia solum ante "
            "lineam novam licent");
        redde i + II;
    }
    _culpa(dc, i, i + II, TOML_CODEX_EFFUGIUM,
        "effugium ignotum (licita: \\b \\t \\n \\f \\r \\\" \\\\ "
        "\\uXXXX \\UXXXXXXXX)");
    redde e < 0x80 ? i + II : i + I;
}

interior b32
_chorda (
                  Piscina* piscina,
    constans MateriaToken* t,
    constans MateriaNodus* nodus,
                   chorda* exitus,
                      Xar* diagnostica)
{
    Decoctio dc;
         b32 gemina;
         s32 n = (s32)t->valor.mensura;
         s32 ab;
         s32 j;
         s32 q;
         s32 i;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    commutatio (t->genus)
    {
        casus TOML_LEX_CLAVIS_NUDA:
            *exitus = t->valor;
            redde VERUM;
        casus TOML_LEX_CHORDA_GEMINA:
        casus TOML_LEX_CLAVIS_GEMINA:
            gemina   = VERUM;
            dc.multa = FALSUM;
            frange;
        casus TOML_LEX_CHORDA_GEMINA_MULTA:
            gemina   = VERUM;
            dc.multa = VERUM;
            frange;
        casus TOML_LEX_CHORDA_SIMPLEX:
        casus TOML_LEX_CLAVIS_SIMPLEX:
            gemina   = FALSUM;
            dc.multa = FALSUM;
            frange;
        casus TOML_LEX_CHORDA_SIMPLEX_MULTA:
            gemina   = FALSUM;
            dc.multa = VERUM;
            frange;
        ordinarius:
            redde FALSUM;
    }
    dc.piscina      = piscina;
    dc.diagnostica  = diagnostica;
    dc.t            = t;
    dc.nodus        = nodus;
    dc.s            = (constans i8*)t->valor.datum;
    dc.sanum        = VERUM;
    dc.m            = ZEPHYRUM;
    q               = gemina ? '"' : '\'';
    ab              = dc.multa ? III : I;
    si (ab > n)
    {
        ab = n;
    }

    /* finis contenti: claudens ut lector eum invenit (multa: duo
     * signa ultra ad contentum pertinent); non clausa = ad finem */
    dc.ad  = n;
    j      = ab;
    dum (j < n)
    {
        si (gemina && dc.s[j] == '\\' && j + I < n)
        {
            j += II;
            perge;
        }
        si (   dc.s[j] == q
            && (!dc.multa
                || (j + II < n && dc.s[j + I] == q
                    && dc.s[j + II] == q)))
        {
            s32 plus = ZEPHYRUM;

            si (dc.multa)
            {
                s32 k = j + III;

                dum (plus < II && k < n && dc.s[k] == q)
                {
                    k++;
                    plus++;
                }
            }
            dc.ad = j + plus;
            frange;
        }
        j++;
    }

    /* multa: linea nova statim post aperientem omittitur */
    si (dc.multa && ab < dc.ad)
    {
        si (dc.s[ab] == '\n')
        {
            ab++;
        }
        alioquin si (   dc.s[ab]     == '\r' && ab + I < dc.ad
                     && dc.s[ab + I] == '\n')
        {
            ab += II;
        }
    }

    dc.exitus = (character*)piscina_allocare(piscina,
        (i64)(dc.ad - ab + I));
    si (dc.exitus == NIHIL)
    {
        redde FALSUM;
    }
    i = ab;
    dum (i < dc.ad)
    {
        s32 c = (s32)dc.s[i];

        si (gemina && c == '\\')
        {
            i = _effugium(&dc, i);
            perge;
        }
        si (dc.multa && c == '\n')
        {
            dc.exitus[dc.m++] = '\n';
            i++;
            perge;
        }
        si (   dc.multa && c == '\r' && i + I < dc.ad
            && dc.s[i + I] == '\n')
        {
            dc.exitus[dc.m++]  = '\n';
            i                  += II;
            perge;
        }
        si (_moderans(c))
        {
            _culpa(&dc, i, i + I, TOML_CODEX_OCTETUS_MODERANS,
                "octetus moderans in chorda non licet (TAB solus)");
            i++;
            perge;
        }
        si (c >= 0x80)
        {
            s32 l = _utf8(dc.s, i, dc.ad);

            si (l < ZEPHYRUM)
            {
                _culpa(&dc, i, i - l, TOML_CODEX_UTF8,
                    "UTF-8 pravum in chorda");
                i -= l;
                perge;
            }
            memcpy(dc.exitus + dc.m, dc.s + i, (size_t)l);
            dc.m  += l;
            i     += l;
            perge;
        }
        dc.exitus[dc.m++] = (character)c;
        i++;
    }
    dc.exitus[dc.m]  = '\0';
    exitus->datum    = (i8*)dc.exitus;
    exitus->mensura  = (i32)dc.m;
    redde dc.sanum;
}

b32
toml_chordam_coquere (
                  Piscina* piscina,
    constans MateriaToken* lexema,
                   chorda* exitus,
                      Xar* diagnostica)
{
    si (piscina == NIHIL || lexema == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    redde _chorda(piscina, lexema, NIHIL, exitus, diagnostica);
}


/* ==================================================
 * Commentaria
 * ================================================== */

interior vacuum
_commentum (
    constans MateriaToken* t,
    constans MateriaNodus* nodus,
                      Xar* diagnostica)
{
    constans i8* s = (constans i8*)t->valor.datum;
            s32  n = (s32)t->valor.mensura;
            s32  i = I;

    dum (i < n)
    {
        s32 c = (s32)s[i];

        si (_moderans(c))
        {
            _diagnosticum(diagnostica, t, nodus, i, i + I,
                TOML_CODEX_OCTETUS_MODERANS,
                "octetus moderans in commentario non licet "
                "(TAB solus)");
            i++;
        }
        alioquin si (c >= 0x80)
        {
            s32 l = _utf8(s, i, n);

            si (l < ZEPHYRUM)
            {
                _diagnosticum(diagnostica, t, nodus, i, i - l,
                    TOML_CODEX_UTF8, "UTF-8 pravum in commentario");
                l = -l;
            }
            i += l;
        }
        alioquin
        {
            i++;
        }
    }
}


/* ==================================================
 * Numeri
 * ================================================== */

/* DIGIT *( ["_"] DIGIT ) ab *j; s[*j] digitus aut '_' esse debet */
interior b32
_digiti (
           constans i8* s,
                   s32  n,
                   s32* j,
    constans character* codex,
     TomlVitiumScalare* v)
{
    s32 k = *j;

    si (k < n && s[k] == '_')
    {
        redde _vitium(v, codex, "'_' solum inter digitos", k, k + I);
    }
    dum (k < n)
    {
        si (_digitus((s32)s[k]))
        {
            k++;
        }
        alioquin si (s[k] == '_')
        {
            si (k + I < n && _digitus((s32)s[k + I]))
            {
                k++;
            }
            alioquin
            {
                redde _vitium(v, codex, "'_' solum inter digitos", k,
                    k + I);
            }
        }
        alioquin
        {
            frange;
        }
    }
    *j = k;
    redde VERUM;
}

interior b32
_numerum_legere (
                Piscina* piscina,
          constans i8* s,
                  s32  n,
             TomlValor* exitus,
    TomlVitiumScalare* v)
{
                   s32  i             = ZEPHYRUM;
                   b32  negativum     = FALSUM;
                   b32  est_fluitans  = FALSUM;
                   s32  j;
                   s32  k;
    constans character* codex;

    si (n > ZEPHYRUM && (s[ZEPHYRUM] == '+' || s[ZEPHYRUM] == '-'))
    {
        negativum  = s[ZEPHYRUM] == '-';
        i          = I;
    }
    si (i < n && (s[i] == '+' || s[i] == '-'))
    {
        redde _vitium(v, TOML_CODEX_INTEGER, "signum duplex", i, i + I);
    }

    /* inf, nan (signo licito) */
    si (i < n && (s[i] == 'i' || s[i] == 'n'))
    {
        si (   n - i == III
            && (memcmp(s + i, "inf", III) == ZEPHYRUM
                || memcmp(s + i, "nan", III) == ZEPHYRUM))
        {
            exitus->genus = TOML_VALOR_FLUITANS;
            exitus->datum.fluitans_valor = strtod(s[i] == 'i' ? "inf"
                : "nan", NIHIL);
            si (negativum)
            {
                exitus->datum.fluitans_valor =
                    -exitus->datum.fluitans_valor;
            }
            redde VERUM;
        }
        redde _vitium(v, TOML_CODEX_FLUITANS,
            "'inf' aut 'nan' exspectatur", ZEPHYRUM, n);
    }

    /* 0x 0o 0b: sine signo, praefixum minusculum */
    si (   i + I < n && s[i] == '0'
        && (s[i + I] == 'x' || s[i + I] == 'X' || s[i + I] == 'o'
            || s[i + I] == 'O' || s[i + I] == 'b' || s[i + I] == 'B'))
    {
        s32 basis;
        i64 summa = ZEPHYRUM;
        b32 ultra = FALSUM;

        si (s[i + I] == 'X' || s[i + I] == 'O' || s[i + I] == 'B')
        {
            redde _vitium(v, TOML_CODEX_INTEGER,
                "praefixum minusculum exspectatur (0x, 0o, 0b)", i + I,
                i + II);
        }
        si (i > ZEPHYRUM)
        {
            redde _vitium(v, TOML_CODEX_INTEGER,
                "signum ante praefixum non licet", ZEPHYRUM, I);
        }
        basis  = s[i + I] == 'x' ? XVI : s[i + I] == 'o' ? VIII : II;
        j      = i + II;
        si (j >= n)
        {
            redde _vitium(v, TOML_CODEX_INTEGER,
                "digiti post praefixum desunt", i, i + II);
        }
        si (s[j] == '_')
        {
            redde _vitium(v, TOML_CODEX_INTEGER,
                "'_' solum inter digitos", j, j + I);
        }
        dum (j < n)
        {
            s32 d;

            si (s[j] == '_')
            {
                si (   j + I < n && _digitus_basis((s32)s[j + I], basis,
                        NIHIL))
                {
                    j++;
                    perge;
                }
                redde _vitium(v, TOML_CODEX_INTEGER,
                    "'_' solum inter digitos", j, j + I);
            }
            si (!_digitus_basis((s32)s[j], basis, &d))
            {
                redde _vitium(v, TOML_CODEX_INTEGER,
                    _digitus_basis((s32)s[j], XVI, NIHIL)
                    ? "digitus extra basim praefixi"
                    : "character inexspectatus", j, j + I);
            }
            si (summa > ((TOML_LIMES_S64 - I) - (i64)d) / (i64)basis)
            {
                ultra = VERUM;
            }
            alioquin
            {
                summa = summa * (i64)basis + (i64)d;
            }
            j++;
        }
        si (ultra)
        {
            redde _vitium(v, TOML_CODEX_EXTRA_FINES,
                "extra fines s64", ZEPHYRUM, n);
        }
        exitus->genus                = TOML_VALOR_INTEGER;
        exitus->datum.integer_valor  = (s64)summa;
        redde VERUM;
    }

    /* decimalis: integer aut fluitans */
    per (k = i; k < n; k++)
    {
        si (s[k] == '.' || s[k] == 'e' || s[k] == 'E')
        {
            est_fluitans = VERUM;
        }
    }
    codex  = est_fluitans ? TOML_CODEX_FLUITANS : TOML_CODEX_INTEGER;
    j      = i;
    si (j >= n)
    {
        redde _vitium(v, codex, "digiti desunt", ZEPHYRUM, n);
    }
    si (s[j] == '.')
    {
        redde _vitium(v, codex, "digitus ante '.' deest", j, j + I);
    }
    si (s[j] != '_' && !_digitus((s32)s[j]))
    {
        redde _vitium(v, codex, "character inexspectatus", j, j + I);
    }
    si (   s[j] == '0' && j + I < n
        && (_digitus((s32)s[j + I]) || s[j + I] == '_'))
    {
        redde _vitium(v, codex, "zephyra praevia non licent", j, j + I);
    }
    si (!_digiti(s, n, &j, codex, v))
    {
        redde FALSUM;
    }
    si (j < n && s[j] == '.')
    {
        j++;
        si (j >= n || (s[j] != '_' && !_digitus((s32)s[j])))
        {
            redde _vitium(v, codex, "digitus post '.' deest", j - I, j);
        }
        si (!_digiti(s, n, &j, codex, v))
        {
            redde FALSUM;
        }
    }
    si (j < n && (s[j] == 'e' || s[j] == 'E'))
    {
        s32 e = j;

        j++;
        si (j < n && (s[j] == '+' || s[j] == '-'))
        {
            j++;
        }
        si (j >= n || (s[j] != '_' && !_digitus((s32)s[j])))
        {
            redde _vitium(v, codex, "exponens sine digitis", e, e + I);
        }
        si (!_digiti(s, n, &j, codex, v))
        {
            redde FALSUM;
        }
    }
    si (j < n)
    {
        redde _vitium(v, codex, "character inexspectatus", j, j + I);
    }

    si (est_fluitans)
    {
        character* copia = (character*)piscina_allocare(piscina,
                               (i64)(n + I));
               s32 m = ZEPHYRUM;

        si (copia == NIHIL)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (s[k] != '_')
            {
                copia[m++] = (character)s[k];
            }
        }
        copia[m]                      = '\0';
        exitus->genus                 = TOML_VALOR_FLUITANS;
        exitus->datum.fluitans_valor  = strtod(copia, NIHIL);
        redde VERUM;
    }
    {
        i64 limes = negativum ? TOML_LIMES_S64 : TOML_LIMES_S64 - I;
        i64 summa = ZEPHYRUM;

        per (k = i; k < n; k++)
        {
            i64 d;

            si (s[k] == '_')
            {
                perge;
            }
            d = (i64)(s[k] - '0');
            si (summa > (limes - d) / X)
            {
                redde _vitium(v, TOML_CODEX_EXTRA_FINES,
                    "extra fines s64", ZEPHYRUM, n);
            }
            summa = summa * X + d;
        }
        exitus->genus = TOML_VALOR_INTEGER;
        si (negativum)
        {
            exitus->datum.integer_valor = summa == ZEPHYRUM
                ? ZEPHYRUM : -(s64)(summa - I) - I;
        }
        alioquin
        {
            exitus->datum.integer_valor = (s64)summa;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Tempora
 * ================================================== */

/* duo digiti ab *j */
interior b32
_duo (
           constans i8* s,
                   s32  n,
                   s32* j,
                   s32* valor,
    constans character* causa,
     TomlVitiumScalare* v)
{
    si (   *j + I >= n || !_digitus((s32)s[*j])
        || !_digitus((s32)s[*j + I]))
    {
        redde _vitium_ad(v, TOML_CODEX_TEMPUS_FORMA, causa, *j, n);
    }
    *valor  = (s32)(s[*j] - '0') * X + (s32)(s[*j + I] - '0');
    *j      += II;
    redde VERUM;
}

interior b32
_signum (
           constans i8* s,
                   s32  n,
                   s32* j,
                   s32  signum,
    constans character* causa,
     TomlVitiumScalare* v)
{
    si (*j >= n || (s32)s[*j] != signum)
    {
        redde _vitium_ad(v, TOML_CODEX_TEMPUS_FORMA, causa, *j, n);
    }
    (*j)++;
    redde VERUM;
}

interior b32
_bissextilis (
    s32 annus)
{
    redde (annus % IV == ZEPHYRUM && annus % C != ZEPHYRUM)
        || annus % CD == ZEPHYRUM;
}

hic_manens constans s32 DIES_MENSIUM[XII] = {
    XXXI, XXVIII, XXXI, XXX, XXXI, XXX, XXXI, XXXI, XXX, XXXI, XXX, XXXI
};

/* hora HH:MM:SS[.f] ab *j; sedes campi horae in *sedes */
interior b32
_horam_legere (
          constans i8* s,
                  s32  n,
                  s32* j,
           TomlTempus* t,
                  s32* sedes,
    TomlVitiumScalare* v)
{
    sedes[ZEPHYRUM] = *j;
    si (!_duo(s, n, j, &t->hora, "II digiti horae exspectantur", v))
    {
        redde FALSUM;
    }
    si (!_signum(s, n, j, ':', "':' post horam exspectatur", v))
    {
        redde FALSUM;
    }
    sedes[I] = *j;
    si (!_duo(s, n, j, &t->minutum, "II digiti minutorum exspectantur",
            v))
    {
        redde FALSUM;
    }
    si (!_signum(s, n, j, ':',
            "secunda exspectantur (HH:MM:SS; TOML 1.0)", v))
    {
        redde FALSUM;
    }
    sedes[II] = *j;
    si (!_duo(s, n, j, &t->secundum,
        "II digiti secundorum exspectantur",
            v))
    {
        redde FALSUM;
    }
    si (*j < n && s[*j] == '.')
    {
        s32 p       = *j;
        s32 digiti  = ZEPHYRUM;

        (*j)++;
        si (*j >= n || !_digitus((s32)s[*j]))
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
                "digiti post '.' desunt", p, p + I);
        }
        dum (*j < n && _digitus((s32)s[*j]))
        {
            si (digiti < IX)
            {
                t->nanosecunda = t->nanosecunda * X
                    + (s32)(s[*j] - '0');
                digiti++;
            }
            (*j)++;
        }
        dum (digiti < IX)
        {
            t->nanosecunda *= X;
            digiti++;
        }
    }
    redde VERUM;
}

b32
toml_tempus_legere (
                chorda  textus,
            TomlTempus* exitus,
     TomlVitiumScalare* v)
{
    constans i8* s     = (constans i8*)textus.datum;
            s32  n     = (s32)textus.mensura;
            s32  j     = ZEPHYRUM;
            b32  dies  = FALSUM;
            b32  hora  = FALSUM;
            s32  sedes_horae[III];
            s32  sedes_zonae  = (s32)-I;
            s32  zona_hora    = ZEPHYRUM;
            s32  zona_minuta  = ZEPHYRUM;
            s32  k;

    memset(exitus, ZEPHYRUM, magnitudo(TomlTempus));
    memset(v, ZEPHYRUM, magnitudo(TomlVitiumScalare));

    si (   n >= V && _digitus((s32)s[ZEPHYRUM]) && _digitus((s32)s[I])
        && _digitus((s32)s[II]) && _digitus((s32)s[III])
        && s[IV] == '-')
    {
        dies           = VERUM;
        exitus->annus  = ZEPHYRUM;
        per (k = ZEPHYRUM; k < IV; k++)
        {
            exitus->annus = exitus->annus * X + (s32)(s[k] - '0');
        }
        j = V;
        si (   !_duo(s, n, &j, &exitus->mensis,
                "II digiti mensis exspectantur", v)
            || !_signum(s, n, &j, '-', "'-' post mensem exspectatur", v)
            || !_duo(s, n, &j, &exitus->dies,
                "II digiti diei exspectantur", v))
        {
            redde FALSUM;
        }
        si (j < n)
        {
            si (s[j] == 'T' || s[j] == 't' || s[j] == ' ')
            {
                s32 p = j;

                j++;
                si (j >= n)
                {
                    redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
                        "hora post separatorem exspectatur", p, p + I);
                }
                hora = VERUM;
            }
            alioquin si (_digitus((s32)s[j]))
            {
                redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
                    "'T' inter diem et horam exspectatur", j, j + I);
            }
            alioquin
            {
                redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
                    "character inexspectatus", j, j + I);
            }
        }
    }
    alioquin si (   n >= III && _digitus((s32)s[ZEPHYRUM])
                 && _digitus((s32)s[I]) && s[II] == ':')
    {
        hora = VERUM;
    }
    alioquin
    {
        redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
            "dies (AAAA-MM-DD) aut hora (HH:MM:SS) exspectatur",
            ZEPHYRUM, n);
    }

    si (hora && !_horam_legere(s, n, &j, exitus, sedes_horae, v))
    {
        redde FALSUM;
    }
    si (dies && hora && j < n)
    {
        si (s[j] == 'Z' || s[j] == 'z')
        {
            j++;
            exitus->genus = TOML_TEMPUS_CUM_ZONA;
        }
        alioquin si (s[j] == '+' || s[j] == '-')
        {
            s32 signum = s[j] == '-' ? (s32)-I : (s32)I;

            j++;
            sedes_zonae = j;
            si (   !_duo(s, n, &j, &zona_hora,
                    "II digiti horae zonae exspectantur", v)
                || !_signum(s, n, &j, ':',
                    "':' in zona exspectatur (+HH:MM)", v)
                || !_duo(s, n, &j, &zona_minuta,
                    "II digiti minutorum zonae exspectantur", v))
            {
                redde FALSUM;
            }
            exitus->genus = TOML_TEMPUS_CUM_ZONA;
            exitus->zona_minuta = signum * (zona_hora * LX
                + zona_minuta);
        }
        alioquin
        {
            exitus->genus = TOML_TEMPUS_LOCALE;
        }
    }
    alioquin si (dies && hora)
    {
        exitus->genus = TOML_TEMPUS_LOCALE;
    }
    alioquin si (dies)
    {
        exitus->genus = TOML_DIES_LOCALIS;
    }
    alioquin
    {
        exitus->genus = TOML_HORA_LOCALIS;
    }
    si (j < n)
    {
        redde _vitium(v, TOML_CODEX_TEMPUS_FORMA,
            "character inexspectatus", j, j + I);
    }

    /* limites campi */
    si (dies)
    {
        s32 ultimus;

        si (exitus->mensis < I || exitus->mensis > XII)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "mensis extra 01..12", V, VII);
        }
        ultimus = DIES_MENSIUM[exitus->mensis - I];
        si (exitus->mensis == II && _bissextilis(exitus->annus))
        {
            ultimus = XXIX;
        }
        si (exitus->dies < I || exitus->dies > ultimus)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                exitus->mensis == II && exitus->dies == XXIX
                ? "29 februarii: annus non bissextilis"
                : "dies extra dies mensis", VIII, X);
        }
    }
    si (hora)
    {
        si (exitus->hora > XXIII)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "hora extra 00..23", sedes_horae[ZEPHYRUM],
                sedes_horae[ZEPHYRUM] + II);
        }
        si (exitus->minutum > LIX)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "minutum extra 00..59", sedes_horae[I],
                sedes_horae[I] + II);
        }
        si (exitus->secundum > LX)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "secundum extra 00..60", sedes_horae[II],
                sedes_horae[II] + II);
        }
    }
    si (sedes_zonae >= ZEPHYRUM)
    {
        si (zona_hora > XXIII)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "hora zonae extra 00..23", sedes_zonae, sedes_zonae
                    + II);
        }
        si (zona_minuta > LIX)
        {
            redde _vitium(v, TOML_CODEX_TEMPUS_LIMITES,
                "minuta zonae extra 00..59", sedes_zonae + III,
                sedes_zonae + V);
        }
    }
    redde VERUM;
}


/* ==================================================
 * Scalaria
 * ================================================== */

/* lexema substantivum scalare: valor coctus, vitium nuntiatum */
interior b32
_lexema_coquere (
                  Piscina* piscina,
    constans MateriaToken* t,
    constans MateriaNodus* nodus,
                TomlValor* exitus,
                      Xar* diagnostica)
{
    TomlVitiumScalare v;

    memset(&v, ZEPHYRUM, magnitudo(v));
    commutatio (t->genus)
    {
        casus TOML_LEX_CHORDA_GEMINA:
        casus TOML_LEX_CHORDA_GEMINA_MULTA:
        casus TOML_LEX_CHORDA_SIMPLEX:
        casus TOML_LEX_CHORDA_SIMPLEX_MULTA:
            exitus->genus = TOML_VALOR_CHORDA;
            redde _chorda(piscina, t, nodus,
                &exitus->datum.chorda_valor,
                diagnostica);
        casus TOML_LEX_NUMERUS:
            si (_numerum_legere(piscina, (constans i8*)t->valor.datum,
                    (s32)t->valor.mensura, exitus, &v))
            {
                redde VERUM;
            }
            _vitium_nuntiare(piscina, diagnostica, t, nodus, &v);
            redde FALSUM;
        casus TOML_LEX_TEMPUS:
            exitus->genus = TOML_VALOR_TEMPUS;
            si (toml_tempus_legere(t->valor,
                &exitus->datum.tempus_valor,
                    &v))
            {
                redde VERUM;
            }
            _vitium_nuntiare(piscina, diagnostica, t, nodus, &v);
            redde FALSUM;
        casus TOML_LEX_VERUM:
        casus TOML_LEX_FALSUM:
            exitus->genus = TOML_VALOR_BOOLEAN;
            exitus->datum.boolean_valor = t->genus == TOML_LEX_VERUM;
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
toml_scalarem_coquere (
                  Piscina* piscina,
    constans MateriaNodus* nodus,
                TomlValor* exitus,
                      Xar* diagnostica)
{
    constans MateriaValor* l;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(TomlValor));
    exitus->nodus = nodus;
    si (piscina == NIHIL || nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (   nodus->genus != (s32)TOML_GENUS_CHORDA
        && nodus->genus != (s32)TOML_GENUS_NUMERUS
        && nodus->genus != (s32)TOML_GENUS_BOOLEAN
        && nodus->genus != (s32)TOML_GENUS_TEMPUS)
    {
        redde FALSUM;
    }
    l = &nodus->loci[TOML_LEXEMA_TOK];
    si (l->genus != MATERIA_VALOR_TOKEN || l->datum.token == NIHIL)
    {
        redde FALSUM;
    }
    redde _lexema_coquere(piscina, l->datum.token, nodus, exitus,
        diagnostica);
}


/* ==================================================
 * Ambulatio: omnia lexemata, ordine octetorum
 * ================================================== */

nomen structura {
             MateriaValor  valor;
    constans MateriaNodus* pater;
                      b32  in_malo;
} Gradus;

interior b32
_pellere (
                      Xar* acervus,
                      s32* cacumen,
             MateriaValor  valor,
    constans MateriaNodus* pater,
                      b32  in_malo)
{
    Gradus* g;

    si (*cacumen < (s32)xar_numerus(acervus))
    {
        g = (Gradus*)xar_obtinere(acervus, (i32)*cacumen);
    }
    alioquin
    {
        g = (Gradus*)xar_addere(acervus);
    }
    si (g == NIHIL)
    {
        redde FALSUM;
    }
    g->valor    = valor;
    g->pater    = pater;
    g->in_malo  = in_malo;
    (*cacumen)++;
    redde VERUM;
}

interior vacuum
_trivia (
             MateriaToken** trivia,
                      i32   numerus,
    constans MateriaNodus*  nodus,
                      Xar*  diagnostica)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (trivia[k]->genus == (s32)TOML_LEX_COMMENTUM)
        {
            _commentum(trivia[k], nodus, diagnostica);
        }
    }
}

/* loci nodi in acervum, ordine inverso (ordine fontis exeunt) */
interior b32
_liberos_pellere (
                      Xar* acervus,
                      s32* cacumen,
    constans MateriaNodus* nodus,
                      b32  in_malo)
{
    b32 malum = in_malo || nodus->genus == (s32)TOML_GENUS_MALUM;
    s32 locus;

    per (locus = (s32)nodus->numerus_locorum - I; locus >= ZEPHYRUM;
         locus--)
    {
        MateriaValor l = nodus->loci[locus];

        si (l.genus == MATERIA_VALOR_LISTA)
        {
            s32 k;

            per (k = (s32)materia_valor_lista_numerus(l) - I;
                 k >= ZEPHYRUM; k--)
            {
                MateriaValor* e = materia_valor_lista_obtinere(l,
                    (i32)k);

                si (   (e->genus == MATERIA_VALOR_NODUS
                        || e->genus == MATERIA_VALOR_TOKEN)
                    && !_pellere(acervus, cacumen, *e, nodus, malum))
                {
                    redde FALSUM;
                }
            }
        }
        alioquin si (   (l.genus == MATERIA_VALOR_NODUS
                         || l.genus == MATERIA_VALOR_TOKEN)
                     && !_pellere(acervus, cacumen, l, nodus, malum))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Xar*
toml_scalaria_iudicare (
                  Piscina* piscina,
    constans MateriaNodus* radix)
{
    Xar* diagnostica;
    Xar* acervus;
    s32  cacumen = ZEPHYRUM;

    si (piscina == NIHIL || radix == NIHIL)
    {
        redde NIHIL;
    }
    diagnostica = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    acervus = xar_creare(piscina, (i32)magnitudo(Gradus));
    si (   diagnostica == NIHIL || acervus == NIHIL
        || !_liberos_pellere(acervus, &cacumen, radix, FALSUM))
    {
        redde NIHIL;
    }
    dum (cacumen > ZEPHYRUM)
    {
        Gradus g;

        cacumen--;
        g = *(Gradus*)xar_obtinere(acervus, (i32)cacumen);
        si (g.valor.genus == MATERIA_VALOR_TOKEN)
        {
            MateriaToken* t = g.valor.datum.token;

            _trivia(t->spatia_ante, t->numerus_ante, g.pater,
                diagnostica);
            si (!g.in_malo)
            {
                si (   t->genus == (s32)TOML_LEX_CLAVIS_GEMINA
                    || t->genus == (s32)TOML_LEX_CLAVIS_SIMPLEX)
                {
                    chorda c;

                    _chorda(piscina, t, g.pater, &c, diagnostica);
                }
                alioquin
                {
                    TomlValor v;

                    memset(&v, ZEPHYRUM, magnitudo(v));
                    _lexema_coquere(piscina, t, g.pater, &v,
                        diagnostica);
                }
            }
            _trivia(t->spatia_post, t->numerus_post, g.pater,
                diagnostica);
        }
        alioquin si (   g.valor.genus == MATERIA_VALOR_NODUS
                     && !_liberos_pellere(acervus, &cacumen,
                         g.valor.datum.nodus, g.in_malo))
        {
            redde NIHIL;
        }
    }
    redde diagnostica;
}
