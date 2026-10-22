/* probatio_base64url.c - base64url (RFC 4648 sectio 5, sine '=')
 *
 * Vectores RFC 4648 sectionis X sine '=', signa '-' '_', refutationes
 * (padding, '+' '/', longitudo % IV == I, bits reliqui non nulli),
 * circuitus M chordarum fortuitarum, RFC 7636 appendix B octetim.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "base64.h"
#include "sigillum.h"
#include "sors.h"
#include <string.h>

interior chorda
_c (
    constans character* literae,
               Piscina* piscina)
{
    redde chorda_ex_literis(literae, piscina);
}

/* codificatio = exspectatum ET decodificatio reddit datum */
interior vacuum
_vector (
          CredoContextus* c,
    constans character* datum,
    constans character* exspectatum)
{
           chorda codificata;
    Base64Fructus f;
              i32 n = (i32)strlen(datum);

    codificata = base64url_codificare((constans i8*)datum, n,
        c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(codificata, exspectatum);
    f = base64url_decodificare(_c(exspectatum, c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(f.datum);
    CREDO_AEQUALIS_I32(f.mensura, n);
    CREDO_VERUM(memcmp(f.datum, datum, (size_t)n) == ZEPHYRUM);
}

interior vacuum
_sectio_vectores_normae (
    CredoContextus* c)
{
    _vector(c, "f", "Zg");
    _vector(c, "fo", "Zm8");
    _vector(c, "foo", "Zm9v");
    _vector(c, "foob", "Zm9vYg");
    _vector(c, "fooba", "Zm9vYmE");
    _vector(c, "foobar", "Zm9vYmFy");
}

/* "" -> "" et "" -> datum non NIHIL, mensura 0 (non error) */
interior vacuum
_sectio_vacuum (CredoContextus* c)
{
           chorda codificata;
    Base64Fructus f;

    codificata = base64url_codificare((constans i8*)"", ZEPHYRUM,
        c->piscina);
    CREDO_AEQUALIS_I32(codificata.mensura, ZEPHYRUM);
    f = base64url_decodificare(_c("", c->piscina), c->piscina);
    CREDO_NON_NIHIL(f.datum);
    CREDO_AEQUALIS_I32(f.mensura, ZEPHYRUM);
}

/* FB FF: base64 "+/8=", base64url "-_8" */
interior vacuum
_sectio_signa_url (CredoContextus* c)
{
               i8 o[II];
           chorda codificata;
    Base64Fructus f;

    o[ZEPHYRUM]  = 0xFB;
    o[I]         = 0xFF;
    codificata   = base64url_codificare(o, II, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(codificata, "-_8");
    f = base64url_decodificare(_c("-_8", c->piscina), c->piscina);
    CREDO_NECESSE_NON_NIHIL(f.datum);
    CREDO_AEQUALIS_I32(f.mensura, II);
    CREDO_AEQUALIS_I32(f.datum[ZEPHYRUM], 0xFB);
    CREDO_AEQUALIS_I32(f.datum[I], 0xFF);
}

interior vacuum
_refutatum (
          CredoContextus* c,
    constans character* codificata)
{
    Base64Fructus f;

    f = base64url_decodificare(_c(codificata, c->piscina), c->piscina);
    CREDO_NIHIL(f.datum);
    CREDO_AEQUALIS_I32(f.mensura, ZEPHYRUM);
}

/* '=' numquam; '+' '/' sunt base64, non base64url; longitudo % IV ==
 * I nullum octetum facit; bits reliqui non nulli ("Zh" = 'f' cum bit
 * superfluo) refutantur ut quisque valor UNAM scripturam habeat */
interior vacuum
_sectio_refutationes (
    CredoContextus* c)
{
    _refutatum(c, "Zm9v=");
    _refutatum(c, "Zg==");
    _refutatum(c, "Zm+v");
    _refutatum(c, "Zm/v");
    _refutatum(c, "Zm9vY");
    _refutatum(c, "Z");
    _refutatum(c, "Zh");
    _refutatum(c, "Zm9");
    _refutatum(c, "Zm v");
    _refutatum(c, "Zm9v\xC3");
}

/* base64url_scribere: capacitas parva -> 0, nihil scriptum */
interior vacuum
_sectio_scribere (CredoContextus* c)
{
    character t[VIII];
          i32 n;

    (vacuum)c;
    CREDO_AEQUALIS_I32(base64url_longitudo_codificata(ZEPHYRUM),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(base64url_longitudo_codificata(I), II);
    CREDO_AEQUALIS_I32(base64url_longitudo_codificata(II), III);
    CREDO_AEQUALIS_I32(base64url_longitudo_codificata(III), IV);
    CREDO_AEQUALIS_I32(base64url_longitudo_codificata(XXXII), XLIII);
    memset(t, 'x', magnitudo(t));
    n = base64url_scribere((constans i8*)"foob", IV, t, V);
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)t[ZEPHYRUM], (i32)'x');
    n = base64url_scribere((constans i8*)"foob", IV, t, VI);
    CREDO_AEQUALIS_I32(n, VI);
    CREDO_VERUM(memcmp(t, "Zm9vYg", VI) == ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)t[VI], (i32)'x');
}

/* M chordae fortuitae, longitudo 0..LXIV, semen fixum */
interior vacuum
_sectio_circuitus (CredoContextus* c)
{
     Sors s;
      i32 k;
      i32 i;
      i32 fracti = ZEPHYRUM;
       i8 o[LXIV];

    sors_seminare(&s, 0x6234757246ULL, ZEPHYRUM);
    per (k = ZEPHYRUM; k < M; k++)
    {
               i32 n = sors_intra(&s, LXV);
            chorda codificata;
     Base64Fructus f;

        per (i = ZEPHYRUM; i < n; i++)
        {
            o[i] = (i8)(sors_proximum(&s) & 0xFF);
        }
        codificata = base64url_codificare(o, n, c->piscina);
        si (codificata.mensura != base64url_longitudo_codificata(n))
        {
            fracti++;
            perge;
        }
        f = base64url_decodificare(codificata, c->piscina);
        si (   !f.datum || f.mensura != n
            || (n && memcmp(f.datum, o, (size_t)n) != ZEPHYRUM))
        {
            fracti++;
        }
    }
    CREDO_AEQUALIS_I32(fracti, ZEPHYRUM);
}

/* RFC 7636 appendix B: code_challenge = base64url(SHA-256(verifier)) */
interior vacuum
_sectio_provocatio (CredoContextus* c)
{
    constans character* verificator =
        "dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk";
               Sigillum s;
                 chorda provocatio;

    s = sigillum_computare(verificator, strlen(verificator));
    provocatio = base64url_codificare(s.octeti, SIGILLUM_OCTETI,
        c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(provocatio,
        "E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "rfc4648", _sectio_vectores_normae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuum", _sectio_vacuum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa_url", _sectio_signa_url, NIHIL, NIHIL, NIHIL, NIHIL },
    { "refutationes", _sectio_refutationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scribere", _sectio_scribere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circuitus", _sectio_circuitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "provocatio", _sectio_provocatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("base64url", SECTIONES);
}
