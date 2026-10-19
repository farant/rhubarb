/* multipartita.c - parser incrementalis multipart/form-data
 *
 * Machina status: PRAEAMBULUM (limes primus quaeritur, octeti
 * abiciuntur) -> CAPUT (capita partis colliguntur usque ad CRLF CRLF)
 * -> DATA (octeti ad reditum, limes quaeritur) -> CAPUT ... -> FINIS
 * (epilogus abicitur).
 *
 * Delimitator D = CRLF "--" limes. Par = numerus octetorum D iam
 * congruentium; ii octeti RETENTI sunt sed non conservati - sunt
 * D[0..par) ipsi. Discrepantia: per tabulam defectus (KMP) octeti
 * primi retentorum ut data emittuntur (ex D ipso), par decrescit.
 * Post D totum PROSPECTUS: "--" = finis, spatia* CRLF = pars nova,
 * aliud = data (D et prospecti iterum per machinam aguntur).
 * PRAEAMBULUM incipit cum par = II: CRLF virtualis ante corpus.
 */
#include "multipartita.h"

#include <string.h>

#define MP_CAPSA          (IV * MXXIV)
#define MP_PROSPECTUS     LXIV
#define MP_DELIM_MAXIMUS  (MULTIPARTITA_LIMES_MAXIMUS + IV)

nomen enumeratio {
    MP_PRAEAMBULUM = 0,
    MP_CAPUT,
    MP_DATA,
    MP_FINIS,
    MP_ERROR
} MpStatus;

nomen enumeratio {
    PROSP_INITIUM = 0,   /* post D: spatia, '-', aut CR */
    PROSP_LINEA,         /* '-' unum visum */
    PROSP_CR             /* CR visum, LF exspectatur */
} MpProspectus;

structura MultipartitaParser {
                Piscina* piscina;
    MultipartitaReditus  reditus;

    /* delimitator et tabula defectus */
     i8 delim[MP_DELIM_MAXIMUS];
    i32 delim_n;
    i32 defectus[MP_DELIM_MAXIMUS];

    MpStatus status;
         i32 par;

    /* prospectus post D totum */
             b32 prospiciens;
    MpProspectus gradus;
              i8 prospecti[MP_PROSPECTUS];
             i32 prospecti_n;

    /* caput partis */
     i8* caput;
    i32  caput_n;
    i32  caput_maximum;

    /* data partis (capsa ante reditum) */
     i8 capsa[MP_CAPSA];
    i32 capsa_n;
    b32 est_filum;
    i32 campus_n;

    /* limites */
    i32 partes;
    i32 partes_maximae;
    i32 campus_maximus;

    constans character* error;
};


/* ==================================================
 * AUXILIA
 * ================================================== */

interior b32
_errare (
     MultipartitaParser* p,
     constans character* causa)
{
    si (p->error == NIHIL)
    {
        p->error = causa;
    }
    p->status = MP_ERROR;
    redde FALSUM;
}

interior character
_minuscula (
    character c)
{
    redde (c >= 'A' && c <= 'Z') ? (character)(c - 'A' + 'a') : c;
}

interior b32
_aequalis_ignora_casus (
                chorda  a,
    constans character* b)
{
    i32 n = (i32)strlen(b);
    i32 i;

    si (a.mensura != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (_minuscula((character)a.datum[i]) != _minuscula(b[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t';
}

interior chorda
_amputare (
    chorda s)
{
    dum (s.mensura > ZEPHYRUM && _spatium(s.datum[ZEPHYRUM]))
    {
        s.datum++;
        s.mensura--;
    }
    dum (s.mensura > ZEPHYRUM && _spatium(s.datum[s.mensura - I]))
    {
        s.mensura--;
    }
    redde s;
}

interior chorda
_copia (
      chorda  s,
     Piscina* piscina)
{
    chorda c;

    c.mensura  = s.mensura;
    c.datum    = (i8*)piscina_allocare(piscina, (i64)(s.mensura + I));
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    redde c;
}

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

/* Parametrum proximum "; clavis=valor" ex s a *i: valor citatus
 * (cum \" et \\) aut nudus (usque ad ';'). FALSUM si nullum. */
interior b32
_parametrum (
              chorda  s,
                 i32* i,
              chorda* clavis,
              chorda* valor,
             Piscina* piscina)
{
    i32 k;

    dum (   *i < s.mensura
         && (s.datum[*i] == ';' || _spatium(s.datum[*i])))
    {
        (*i)++;
    }
    si (*i >= s.mensura)
    {
        redde FALSUM;
    }
    k = *i;
    dum (*i < s.mensura && s.datum[*i] != '=' && s.datum[*i] != ';')
    {
        (*i)++;
    }
    clavis->datum    = s.datum + k;
    clavis->mensura  = *i - k;
    *clavis          = _amputare(*clavis);
    *valor           = _vacua();
    si (*i >= s.mensura || s.datum[*i] == ';')
    {
        redde VERUM;   /* clavis sine valore */
    }
    (*i)++;   /* '=' */
    dum (*i < s.mensura && _spatium(s.datum[*i]))
    {
        (*i)++;
    }
    si (*i < s.mensura && s.datum[*i] == '"')
    {
         i8* d = (i8*)piscina_allocare(piscina, (i64)(s.mensura - *i));
        i32  m = ZEPHYRUM;

        (*i)++;
        dum (*i < s.mensura && s.datum[*i] != '"')
        {
            si (s.datum[*i] == '\\' && *i + I < s.mensura)
            {
                (*i)++;
            }
            d[m++] = s.datum[*i];
            (*i)++;
        }
        si (*i < s.mensura)
        {
            (*i)++;   /* '"' claudens */
        }
        dum (*i < s.mensura && s.datum[*i] != ';')
        {
            (*i)++;
        }
        valor->datum    = d;
        valor->mensura  = m;
        redde VERUM;
    }
    k = *i;
    dum (*i < s.mensura && s.datum[*i] != ';')
    {
        (*i)++;
    }
    valor->datum    = s.datum + k;
    valor->mensura  = *i - k;
    *valor          = _amputare(*valor);
    redde VERUM;
}

interior s32
_hex (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    redde -I;
}

/* RFC 5987: charset'lingua'%XX..; solum UTF-8; vacua si malum */
interior chorda
_extensum_decodere (
      chorda  v,
     Piscina* piscina)
{
    chorda  codex_signorum;
       i32  i = ZEPHYRUM;
       i32  secundum;
        i8* d;
       i32  m = ZEPHYRUM;

    dum (i < v.mensura && v.datum[i] != '\'')
    {
        i++;
    }
    si (i >= v.mensura)
    {
        redde _vacua();
    }
    codex_signorum.datum    = v.datum;
    codex_signorum.mensura  = i;
    si (!_aequalis_ignora_casus(codex_signorum, "UTF-8"))
    {
        redde _vacua();
    }
    secundum = i + I;
    dum (secundum < v.mensura && v.datum[secundum] != '\'')
    {
        secundum++;
    }
    si (secundum >= v.mensura)
    {
        redde _vacua();
    }
    d = (i8*)piscina_allocare(piscina, (i64)(v.mensura + I));
    per (i = secundum + I; i < v.mensura; i++)
    {
        si (v.datum[i] == '%')
        {
            s32 a;
            s32 b;

            si (i + II >= v.mensura)
            {
                redde _vacua();
            }
            a = _hex(v.datum[i + I]);
            b = _hex(v.datum[i + II]);
            si (a < ZEPHYRUM || b < ZEPHYRUM)
            {
                redde _vacua();
            }
            d[m++]  = (i8)(a * XVI + b);
            i       += II;
        }
        alioquin
        {
            d[m++] = v.datum[i];
        }
    }
    {
        chorda r;

        r.datum    = d;
        r.mensura  = m;
        redde r;
    }
}

/* Nomen ultimum (post '/' aut '\'), signa moderantia abiecta; "." et
 * ".." vacua - numquam via */
interior chorda
_nomen_ultimum (
      chorda  t,
     Piscina* piscina)
{
       i32  initium = ZEPHYRUM;
       i32  i;
        i8* d;
       i32  m = ZEPHYRUM;
    chorda  r;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '/' || t.datum[i] == '\\')
        {
            initium = i + I;
        }
    }
    d = (i8*)piscina_allocare(piscina, (i64)(t.mensura + I));
    per (i = initium; i < t.mensura; i++)
    {
        si (t.datum[i] >= (i8)' ' && t.datum[i] != (i8)0x7F)
        {
            d[m++] = t.datum[i];
        }
    }
    r.datum    = d;
    r.mensura  = m;
    si (   (m == I && d[ZEPHYRUM] == '.')
        || (m == II && d[ZEPHYRUM] == '.' && d[I] == '.'))
    {
        redde _vacua();
    }
    redde r;
}


/* ==================================================
 * CAPITA PARTIS
 * ================================================== */

interior b32
_caput_finire (
    MultipartitaParser* p)
{
    chorda campus         = _vacua();
    chorda titulus        = _vacua();
    chorda extensum       = _vacua();
    chorda mimen          = _vacua();
       b32 habet_titulum  = FALSUM;
       b32 dispositio     = FALSUM;
       b32 habet_campum   = FALSUM;
       i32 i              = ZEPHYRUM;
       i32 limen          = p->caput_n - II;  /* sine CRLF ultimo */

    dum (i < limen)
    {
        chorda linea;
        chorda titulus_capitis;
        chorda valor;
           i32 k      = i;
           s32 colon  = -I;

        dum (   k + I < p->caput_n
             && !(p->caput[k] == '\r' && p->caput[k + I] == '\n'))
        {
            si (colon < ZEPHYRUM && p->caput[k] == ':')
            {
                colon = (s32)k;
            }
            k++;
        }
        linea.datum    = p->caput + i;
        linea.mensura  = k - i;
        i              = k + II;
        si (linea.mensura == ZEPHYRUM || colon < ZEPHYRUM)
        {
            perge;
        }
        titulus_capitis.datum    = linea.datum;
        titulus_capitis.mensura  = (i32)colon - (i32)(linea.datum
            - p->caput);
        titulus_capitis  = _amputare(titulus_capitis);
        valor.datum      = p->caput + (i32)colon + I;
        valor.mensura    = k - (i32)colon - I;
        valor            = _amputare(valor);

        si (_aequalis_ignora_casus(titulus_capitis,
            "Content-Disposition"))
        {
            chorda primum;
               i32 j = ZEPHYRUM;
            chorda clavis;
            chorda v;

            dum (j < valor.mensura && valor.datum[j] != ';')
            {
                j++;
            }
            primum.datum    = valor.datum;
            primum.mensura  = j;
            si (!_aequalis_ignora_casus(_amputare(primum), "form-data"))
            {
                redde _errare(p, "Content-Disposition non form-data");
            }
            dispositio = VERUM;
            dum (_parametrum(valor, &j, &clavis, &v, p->piscina))
            {
                si (_aequalis_ignora_casus(clavis, "name"))
                {
                    campus        = _copia(v, p->piscina);
                    habet_campum  = VERUM;
                }
                alioquin si (_aequalis_ignora_casus(clavis, "filename"))
                {
                    titulus        = v;
                    habet_titulum  = VERUM;
                }
                alioquin si (_aequalis_ignora_casus(clavis,
                             "filename*"))
                {
                    extensum       = _extensum_decodere(v, p->piscina);
                    habet_titulum  = VERUM;
                }
            }
        }
        alioquin si (_aequalis_ignora_casus(titulus_capitis,
                     "Content-Type"))
        {
            mimen = _copia(valor, p->piscina);
        }
    }

    si (!dispositio)
    {
        redde _errare(p, "Content-Disposition absens");
    }
    si (!habet_campum)
    {
        redde _errare(p, "name absens in Content-Disposition");
    }
    p->partes++;
    si (p->partes > p->partes_maximae)
    {
        redde _errare(p, "partes nimis multae");
    }
    si (extensum.mensura > ZEPHYRUM)
    {
        titulus = extensum;   /* filename* praevalet */
    }
    titulus = _nomen_ultimum(titulus, p->piscina);
    si (mimen.mensura == ZEPHYRUM)
    {
        mimen = chorda_ex_literis("text/plain", p->piscina);
    }
    p->est_filum  = habet_titulum;
    p->campus_n   = ZEPHYRUM;
    p->capsa_n    = ZEPHYRUM;
    si (   p->reditus.initium
        && !p->reditus.initium(p->reditus.datum, campus, titulus,
            habet_titulum, mimen))
    {
        redde _errare(p, "reditus recusavit (initium)");
    }
    p->caput_n  = ZEPHYRUM;
    p->status   = MP_DATA;
    p->par      = ZEPHYRUM;
    redde VERUM;
}


/* ==================================================
 * DATA ET DELIMITATOR
 * ================================================== */

interior b32
_capsam_effundere (
    MultipartitaParser* p)
{
    si (p->capsa_n > ZEPHYRUM)
    {
        i32 n = p->capsa_n;

        p->capsa_n = ZEPHYRUM;
        si (   p->reditus.data
            && !p->reditus.data(p->reditus.datum, p->capsa, n))
        {
            redde _errare(p, "reditus recusavit (data)");
        }
    }
    redde VERUM;
}

/* octeti certe data (non delimitator): in PRAEAMBULUM abiciuntur */
interior b32
_emittere (
     MultipartitaParser* p,
    constans i8* octeti,
            i32  n)
{
    i32 i;

    si (p->status != MP_DATA)
    {
        redde VERUM;
    }
    si (!p->est_filum)
    {
        si (n > p->campus_maximus - p->campus_n)
        {
            redde _errare(p, "campus nimis magnus");
        }
        p->campus_n += n;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (p->capsa_n == MP_CAPSA && !_capsam_effundere(p))
        {
            redde FALSUM;
        }
        p->capsa[p->capsa_n++] = octeti[i];
    }
    redde VERUM;
}

interior b32
_octetum (
    MultipartitaParser* p,
                    i8  c);

/* D totum + prospecti non delimitator sunt: D (pars prima) ut data,
 * reliqua per machinam iterum */
interior b32
_prospectus_deficit (
    MultipartitaParser* p,
                    i8  c)
{
     i8 iterandi[MP_PROSPECTUS + I];
    i32 n = p->prospecti_n;
    i32 k = p->defectus[p->delim_n - I];
    i32 i;

    memcpy(iterandi, p->prospecti, (size_t)n);
    iterandi[n++]   = c;
    p->prospiciens  = FALSUM;
    p->prospecti_n  = ZEPHYRUM;
    si (!_emittere(p, p->delim, p->delim_n - k))
    {
        redde FALSUM;
    }
    p->par = k;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!_octetum(p, iterandi[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* D cum prospectu agnitus: pars finitur; nova aut finis */
interior b32
_delimitator (
    MultipartitaParser* p,
                   b32  finalis)
{
    p->prospiciens  = FALSUM;
    p->prospecti_n  = ZEPHYRUM;
    p->par          = ZEPHYRUM;
    si (p->status == MP_DATA)
    {
        si (!_capsam_effundere(p))
        {
            redde FALSUM;
        }
        si (p->reditus.finis && !p->reditus.finis(p->reditus.datum))
        {
            redde _errare(p, "reditus recusavit (finis)");
        }
    }
    p->status   = finalis ? MP_FINIS : MP_CAPUT;
    p->caput_n  = ZEPHYRUM;
    redde VERUM;
}

interior b32
_prospicere (
    MultipartitaParser* p,
                    i8  c)
{
    si (p->prospecti_n >= MP_PROSPECTUS)
    {
        redde _prospectus_deficit(p, c);
    }
    commutatio (p->gradus)
    {
        casus PROSP_INITIUM:
            si (c == '-' && p->prospecti_n == ZEPHYRUM)
            {
                p->gradus = PROSP_LINEA;
            }
            alioquin si (c == '\r')
            {
                p->gradus = PROSP_CR;
            }
            alioquin si (c == '\n')
            {
                redde _errare(p, "LF sine CR post limitem (RFC 7578 "
                    "CRLF postulat)");
            }
            alioquin si (!_spatium(c))
            {
                redde _prospectus_deficit(p, c);
            }
            frange;
        casus PROSP_LINEA:
            si (c == '-')
            {
                redde _delimitator(p, VERUM);
            }
            redde _prospectus_deficit(p, c);
        casus PROSP_CR:
            si (c == '\n')
            {
                redde _delimitator(p, FALSUM);
            }
            redde _prospectus_deficit(p, c);
    }
    p->prospecti[p->prospecti_n++] = c;
    redde VERUM;
}

interior b32
_caput_octetum (
    MultipartitaParser* p,
                    i8  c)
{
    si (   c == '\n'
        && (p->caput_n == ZEPHYRUM || p->caput[p->caput_n - I] != '\r'))
    {
        redde _errare(p, "LF sine CR in capite partis (RFC 7578 CRLF "
            "postulat)");
    }
    si (p->caput_n >= p->caput_maximum)
    {
        redde _errare(p, "caput partis nimis magnum");
    }
    p->caput[p->caput_n++] = c;
    si (c != '\n')
    {
        redde VERUM;
    }
    si (   p->caput_n == II
        || (   p->caput_n >= IV
            && p->caput[p->caput_n - III] == '\n'
            && p->caput[p->caput_n - IV] == '\r'))
    {
        redde _caput_finire(p);
    }
    redde VERUM;
}

interior b32
_octetum (
    MultipartitaParser* p,
                    i8  c)
{
    commutatio (p->status)
    {
        casus MP_FINIS:
            redde VERUM;   /* epilogus */
        casus MP_ERROR:
            redde FALSUM;
        casus MP_CAPUT:
            redde _caput_octetum(p, c);
        casus MP_PRAEAMBULUM:
        casus MP_DATA:
            frange;
    }

    si (p->prospiciens)
    {
        redde _prospicere(p, c);
    }

    dum (p->par > ZEPHYRUM && c != p->delim[p->par])
    {
        i32 k = p->defectus[p->par - I];

        si (!_emittere(p, p->delim, p->par - k))
        {
            redde FALSUM;
        }
        p->par = k;
    }
    si (c == p->delim[p->par])
    {
        p->par++;
        si (p->par == p->delim_n)
        {
            p->prospiciens  = VERUM;
            p->gradus       = PROSP_INITIUM;
            p->prospecti_n  = ZEPHYRUM;
        }
        redde VERUM;
    }
    redde _emittere(p, &c, I);
}


/* ==================================================
 * FUNCTIONES PUBLICAE
 * ================================================== */

interior b32
_signum_limitis (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z') || c == '\'' || c == '(' || c == ')'
        || c == '+' || c == '_' || c == ',' || c == '-' || c == '.'
        || c == '/' || c == ':' || c == '=' || c == '?' || c == ' ';
}

chorda
multipartita_limes_ex_typo (
      chorda  content_type,
     Piscina* piscina)
{
    chorda typus;
    chorda clavis;
    chorda valor;
       i32 i = ZEPHYRUM;

    si (!piscina || !content_type.datum)
    {
        redde _vacua();
    }
    dum (i < content_type.mensura && content_type.datum[i] != ';')
    {
        i++;
    }
    typus.datum    = content_type.datum;
    typus.mensura  = i;
    si (!_aequalis_ignora_casus(_amputare(typus),
        "multipart/form-data"))
    {
        redde _vacua();
    }
    dum (_parametrum(content_type, &i, &clavis, &valor, piscina))
    {
        si (_aequalis_ignora_casus(clavis, "boundary"))
        {
            i32 k;

            si (   valor.mensura < I
                || valor.mensura > MULTIPARTITA_LIMES_MAXIMUS
                || valor.datum[valor.mensura - I] == ' ')
            {
                redde _vacua();
            }
            per (k = ZEPHYRUM; k < valor.mensura; k++)
            {
                si (!_signum_limitis(valor.datum[k]))
                {
                    redde _vacua();
                }
            }
            redde _copia(valor, piscina);
        }
    }
    redde _vacua();
}

MultipartitaParser*
multipartita_creare (
                          chorda  limes,
    constans MultipartitaLimites* limites,
             MultipartitaReditus  reditus,
                         Piscina* piscina)
{
    MultipartitaParser* p;
                   i32  k;
                   i32  i;

    si (   !piscina || !limes.datum || limes.mensura < I
        || limes.mensura > MULTIPARTITA_LIMES_MAXIMUS)
    {
        redde NIHIL;
    }
    p = (MultipartitaParser*)piscina_allocare(piscina,
        (i64)magnitudo(MultipartitaParser));
    si (!p)
    {
        redde NIHIL;
    }
    memset(p, ZEPHYRUM, magnitudo(*p));
    p->piscina = piscina;
    p->reditus = reditus;

    p->partes_maximae = (limites && limites->partes_maximae)
        ? limites->partes_maximae : LXIV;
    p->caput_maximum = (limites && limites->caput_partis_maximum)
        ? limites->caput_partis_maximum : VIII * MXXIV;
    p->campus_maximus = (limites && limites->campus_maximus)
        ? limites->campus_maximus : LXIV * MXXIV;
    p->caput = (i8*)piscina_allocare(piscina, (i64)p->caput_maximum);
    si (!p->caput)
    {
        redde NIHIL;
    }

    p->delim[ZEPHYRUM]  = '\r';
    p->delim[I]         = '\n';
    p->delim[II]        = '-';
    p->delim[III]       = '-';
    memcpy(p->delim + IV, limes.datum, (size_t)limes.mensura);
    p->delim_n = limes.mensura + IV;

    /* tabula defectus: defectus[i] = longitudo praefixi proprii
     * maximi D[0..i] quod etiam suffixum est */
    p->defectus[ZEPHYRUM]  = ZEPHYRUM;
    k                      = ZEPHYRUM;
    per (i = I; i < p->delim_n; i++)
    {
        dum (k > ZEPHYRUM && p->delim[i] != p->delim[k])
        {
            k = p->defectus[k - I];
        }
        si (p->delim[i] == p->delim[k])
        {
            k++;
        }
        p->defectus[i] = k;
    }

    p->status  = MP_PRAEAMBULUM;
    p->par     = II;   /* CRLF virtualis ante corpus */
    redde p;
}

b32
multipartita_adicere (
     MultipartitaParser* parser,
            constans i8* octeti,
                    i32  n)
{
    i32 i;

    si (!parser || parser->status == MP_ERROR)
    {
        redde FALSUM;
    }
    si (n > ZEPHYRUM && !octeti)
    {
        redde _errare(parser, "octeti NIHIL");
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!_octetum(parser, octeti[i]))
        {
            redde FALSUM;
        }
    }
    si (parser->status == MP_DATA)
    {
        redde _capsam_effundere(parser);
    }
    redde VERUM;
}

b32
multipartita_finire (
    MultipartitaParser* parser)
{
    si (!parser)
    {
        redde FALSUM;
    }
    si (parser->status == MP_FINIS)
    {
        redde VERUM;
    }
    si (parser->status != MP_ERROR)
    {
        (vacuum)_errare(parser,
            "finis absens (--limes-- non inventum)");
    }
    redde FALSUM;
}

chorda
multipartita_error (
    MultipartitaParser* parser)
{
    si (!parser || parser->error == NIHIL)
    {
        redde _vacua();
    }
    redde chorda_ex_literis(parser->error, parser->piscina);
}
