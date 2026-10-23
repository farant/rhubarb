/* signatura_aws.c - AWS Signature Version 4 (vide signatura_aws.h)
 *
 * Gradus (AWS SigV4): petitio canonica -> chorda signanda -> clavis
 * signandi derivata (HMAC catena) -> signatura hex. Capita: titulus
 * minusculus, valor contractus; ordo per titulum STABILIS (duplicata
 * ordine dato coniunguntur). Quaestio: paria codificata, ordo per
 * clavem deinde valorem (octeti). */
#include "latina.h"
#include "signatura_aws.h"
#include "sigillum.h"
#include "fasti.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character cifrae_maiusculae[] = "0123456789ABCDEF";
hic_manens constans character algorithmus_aws[]   = "AWS4-HMAC-SHA256";
hic_manens constans character sigillum_vacui[]    =
    "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

interior chorda
_literae (
    constans character* l,
               Piscina* piscina)
{
    redde chorda_ex_literis(l, piscina);
}

interior b32
_non_reservatus (
    i8 c)
{
    redde (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.'
        || c == '~';
}

chorda
signatura_aws_codificare (
     chorda  s,
        b32  servare_virgulam,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
                               (memoriae_index)(s.mensura * III + I));
                   i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (_non_reservatus(c) || (servare_virgulam && c == '/'))
        {
            chorda_aedificator_appendere_character(a, (character)c);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, '%');
            chorda_aedificator_appendere_character(a,
                cifrae_maiusculae[((i32)c >> IV) & 0x0F]);
            chorda_aedificator_appendere_character(a,
                cifrae_maiusculae[(i32)c & 0x0F]);
        }
    }
    redde chorda_aedificator_finire(a);
}

chorda
signatura_aws_dies (
    MomentumSec  tempus,
        Piscina* piscina)
{
     DiesHora dh = fasti_ex_unix(tempus);
    character capsa[XXXII];

    sprintf(capsa, "%04d%02d%02dT%02d%02d%02dZ", (integer)dh.dies.annus,
        (integer)dh.dies.mensis, (integer)dh.dies.dies,
        (integer)dh.hora.hora, (integer)dh.hora.minutum,
        (integer)dh.hora.secundum);
    redde chorda_ex_literis(capsa, piscina);
}

/* comparatio octetorum (memcmp + longitudo) */
interior s32
_comparare (
    chorda a,
    chorda b)
{
    i32 n = a.mensura < b.mensura ? a.mensura : b.mensura;
    s32 r = n > ZEPHYRUM ? (s32)memcmp(a.datum, b.datum, (size_t)n)
                         : ZEPHYRUM;

    si (r != ZEPHYRUM)
    {
        redde r;
    }
    si (a.mensura == b.mensura)
    {
        redde ZEPHYRUM;
    }
    redde a.mensura < b.mensura ? -I : I;
}

interior b32
_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* valor praecisus, spatia interiora in unum contracta */
interior chorda
_valorem_contrahere (
     chorda  v,
    Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
                               (memoriae_index)(v.mensura + I));
                   i32 i;
                   b32 spatium_pendens  = FALSUM;
                   b32 initium          = VERUM;

    per (i = ZEPHYRUM; i < v.mensura; i++)
    {
        si (_spatium(v.datum[i]))
        {
            spatium_pendens = !initium;
            perge;
        }
        si (spatium_pendens)
        {
            chorda_aedificator_appendere_character(a, ' ');
            spatium_pendens = FALSUM;
        }
        chorda_aedificator_appendere_character(a,
            (character)v.datum[i]);
        initium = FALSUM;
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_minuscula (
     chorda  s,
    Piscina* piscina)
{
    i8* d = (i8*)piscina_allocare(piscina, (memoriae_index)(s.mensura
        + I));
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        d[i] = (c >= 'A' && c <= 'Z') ? (i8)(c + ('a' - 'A')) : c;
    }
    redde chorda_ex_buffer(d, s.mensura);
}

/* ordo insertionis STABILIS per titulum (deinde valorem si
 * per_valorem) */
interior vacuum
_ordinare (
    SignaturaPar* paria,
             i32  numerus,
             b32  per_valorem)
{
    i32 i;

    per (i = I; i < numerus; i++)
    {
        SignaturaPar x = paria[i];
                 s32 j = (s32)i - I;

        dum (j >= ZEPHYRUM)
        {
            s32 r = _comparare(paria[j].titulus, x.titulus);

            si (r == ZEPHYRUM && per_valorem)
            {
                r = _comparare(paria[j].valor, x.valor);
            }
            si (r <= ZEPHYRUM)
            {
                frange;
            }
            paria[j + I] = paria[j];
            j--;
        }
        paria[j + I] = x;
    }
}

nomen structura {
    chorda canonica;   /* "a:x\nb:y\n" */
    chorda signata;    /* "a;b" */
} CapitaCanonica;

/* capita (+ host, + addita) -> forma canonica; duplicata coniuncta */
interior CapitaCanonica
_capita_canonica (
    constans SignaturaPetitio* sp,
        constans SignaturaPar* addita,
                          i32  addita_numerus,
                      Piscina* piscina)
{
               i32  n = sp->capita_numerus + addita_numerus + I;
      SignaturaPar* t = (SignaturaPar*)piscina_allocare(piscina,
                           (memoriae_index)(n
                               * (i32)magnitudo(SignaturaPar)));
               i32  k = ZEPHYRUM;
               i32  i;
 ChordaAedificator* can = chorda_aedificator_creare(piscina, CCLVI);
 ChordaAedificator* sig = chorda_aedificator_creare(piscina, LXIV);
    CapitaCanonica  r;

    t[k].titulus  = _literae("host", piscina);
    t[k].valor    = _valorem_contrahere(sp->hospes, piscina);
    k++;
    per (i = ZEPHYRUM; i < sp->capita_numerus; i++, k++)
    {
        t[k].titulus = _minuscula(sp->capita[i].titulus, piscina);
        t[k].valor = _valorem_contrahere(sp->capita[i].valor,
            piscina);
    }
    per (i = ZEPHYRUM; i < addita_numerus; i++, k++)
    {
        t[k] = addita[i];
    }
    _ordinare(t, k, FALSUM);
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (i > ZEPHYRUM && _comparare(t[i].titulus, t[i - I].titulus)
                == ZEPHYRUM)
        {
            /* duplicatum: valor priori ',' adiungitur */
            chorda_aedificator_truncare(can,
                chorda_aedificator_longitudo(can) - I);
            chorda_aedificator_appendere_character(can, ',');
            chorda_aedificator_appendere_chorda(can, t[i].valor);
            chorda_aedificator_appendere_character(can, '\n');
            perge;
        }
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(sig, ';');
        }
        chorda_aedificator_appendere_chorda(sig, t[i].titulus);
        chorda_aedificator_appendere_chorda(can, t[i].titulus);
        chorda_aedificator_appendere_character(can, ':');
        chorda_aedificator_appendere_chorda(can, t[i].valor);
        chorda_aedificator_appendere_character(can, '\n');
    }
    r.canonica  = chorda_aedificator_finire(can);
    r.signata   = chorda_aedificator_finire(sig);
    redde r;
}

/* quaestio canonica: paria data + addita, codificata, ordinata */
interior chorda
_quaestio_canonica (
    constans SignaturaPetitio* sp,
        constans SignaturaPar* addita,
                          i32  addita_numerus,
                      Piscina* piscina)
{
               i32  n = sp->quaestio_numerus + addita_numerus;
      SignaturaPar* t = (SignaturaPar*)piscina_allocare(piscina,
                           (memoriae_index)((n + I)
                               * (i32)magnitudo(SignaturaPar)));
               i32  k = ZEPHYRUM;
               i32  i;
 ChordaAedificator* a = chorda_aedificator_creare(piscina, CCLVI);

    per (i = ZEPHYRUM; i < sp->quaestio_numerus; i++, k++)
    {
        t[k].titulus = signatura_aws_codificare(sp->quaestio[i].titulus,
                           FALSUM, piscina);
        t[k].valor   = signatura_aws_codificare(sp->quaestio[i].valor,
                           FALSUM, piscina);
    }
    per (i = ZEPHYRUM; i < addita_numerus; i++, k++)
    {
        t[k].titulus = signatura_aws_codificare(addita[i].titulus,
            FALSUM,
                           piscina);
        t[k].valor   = signatura_aws_codificare(addita[i].valor, FALSUM,
                           piscina);
    }
    _ordinare(t, k, VERUM);
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(a, '&');
        }
        chorda_aedificator_appendere_chorda(a, t[i].titulus);
        chorda_aedificator_appendere_character(a, '=');
        chorda_aedificator_appendere_chorda(a, t[i].valor);
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_hex (
    constans Sigillum* s,
              Piscina* piscina)
{
    character capsa[SIGILLUM_HEX_MENSURA];

    sigillum_hex(s, capsa);
    redde chorda_ex_literis(capsa, piscina);
}

/* clavis derivata, signatura hex; omnia intermedia zephyrantur */
interior chorda
_signare (
    constans SignaturaPetitio* sp,
                       chorda  dies_brevis,
                       chorda  signanda,
                      Piscina* piscina)
{
    i8 initium[SIGNATURA_AWS_SECRETUM_MAXIMUM + IV];
    Sigillum k_dies, k_regio, k_servitium, k_signandi, s;
    chorda r;

    memcpy(initium, "AWS4", IV);
    memcpy(initium + IV, sp->secretum.datum,
        (size_t)sp->secretum.mensura);
    k_dies = sigillum_hmac(initium,
        (memoriae_index)(sp->secretum.mensura + IV), dies_brevis.datum,
        (memoriae_index)dies_brevis.mensura);
    k_regio = sigillum_hmac(k_dies.octeti, SIGILLUM_OCTETI,
        sp->regio.datum, (memoriae_index)sp->regio.mensura);
    k_servitium = sigillum_hmac(k_regio.octeti, SIGILLUM_OCTETI,
        sp->servitium.datum, (memoriae_index)sp->servitium.mensura);
    k_signandi = sigillum_hmac(k_servitium.octeti, SIGILLUM_OCTETI,
        "aws4_request", XII);
    s = sigillum_hmac(k_signandi.octeti, SIGILLUM_OCTETI,
        signanda.datum,
        (memoriae_index)signanda.mensura);
    r = _hex(&s, piscina);
    memset(initium, ZEPHYRUM, magnitudo(initium));
    memset(&k_dies, ZEPHYRUM, magnitudo(k_dies));
    memset(&k_regio, ZEPHYRUM, magnitudo(k_regio));
    memset(&k_servitium, ZEPHYRUM, magnitudo(k_servitium));
    memset(&k_signandi, ZEPHYRUM, magnitudo(k_signandi));
    redde r;
}

interior SignaturaResultus
_error (
    constans character* nuntius,
               Piscina* piscina)
{
    SignaturaResultus r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.error = chorda_ex_literis(nuntius, piscina);
    redde r;
}

/* campi necessarii; nuntius nominat campum, numquam valorem */
interior constans character*
_vitium (
    constans SignaturaPetitio* sp,
                         b32  quaestione)
{
    si (!sp->methodus || sp->methodus[ZEPHYRUM] == '\0')
    {
        redde "signatura_aws: methodus vacua";
    }
    si (sp->hospes.mensura == ZEPHYRUM)
    {
        redde "signatura_aws: hospes vacuus";
    }
    si (sp->regio.mensura == ZEPHYRUM)
    {
        redde "signatura_aws: regio vacua";
    }
    si (sp->servitium.mensura == ZEPHYRUM)
    {
        redde "signatura_aws: servitium vacuum";
    }
    si (sp->clavis_id.mensura == ZEPHYRUM)
    {
        redde "signatura_aws: clavis_id vacua";
    }
    si (sp->secretum.mensura == ZEPHYRUM)
    {
        redde "signatura_aws: secretum vacuum";
    }
    si (sp->secretum.mensura > SIGNATURA_AWS_SECRETUM_MAXIMUM)
    {
        redde "signatura_aws: secretum longius quam CCLVI octeti";
    }
    si (   quaestione && (sp->duratio < I
            || sp->duratio > SIGNATURA_AWS_DURATIO_MAXIMA))
    {
        redde "signatura_aws: duratio extra 1..604800";
    }
    redde NIHIL;
}

/* petitio canonica + chorda signanda + signatura in r */
interior vacuum
_perficere (
       constans SignaturaPetitio* sp,
                          chorda  quaestio,
         constans CapitaCanonica* capita,
                          chorda  dies_brevis,
                          chorda  ambitus,
               SignaturaResultus* r,
                         Piscina* piscina)
{
     ChordaAedificator* a = chorda_aedificator_creare(piscina, DXII);
              Sigillum  s;

    chorda_aedificator_appendere_literis(a, sp->methodus);
    chorda_aedificator_appendere_character(a, '\n');
    si (sp->via.mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '/');
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(a,
            signatura_aws_codificare(sp->via, VERUM, piscina));
    }
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, quaestio);
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, capita->canonica);
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, capita->signata);
    chorda_aedificator_appendere_character(a, '\n');
    si (sp->corpus_sigillum.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, sp->corpus_sigillum);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, sigillum_vacui);
    }
    r->petitio_canonica = chorda_aedificator_finire(a);

    s = sigillum_computare(r->petitio_canonica.datum,
        (memoriae_index)r->petitio_canonica.mensura);
    a = chorda_aedificator_creare(piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, algorithmus_aws);
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, r->dies_amz);
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, ambitus);
    chorda_aedificator_appendere_character(a, '\n');
    chorda_aedificator_appendere_chorda(a, _hex(&s, piscina));
    r->chorda_signanda = chorda_aedificator_finire(a);
    r->signatura = _signare(sp, dies_brevis, r->chorda_signanda,
        piscina);
}

/* "20150830/us-east-1/service/aws4_request" */
interior chorda
_ambitus (
    constans SignaturaPetitio* sp,
                       chorda  dies_brevis,
                      Piscina* piscina)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina, LXIV);

    chorda_aedificator_appendere_chorda(a, dies_brevis);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a, sp->regio);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a, sp->servitium);
    chorda_aedificator_appendere_literis(a, "/aws4_request");
    redde chorda_aedificator_finire(a);
}

SignaturaResultus
signatura_aws_capitibus (
    constans SignaturaPetitio* sp,
                      Piscina* piscina)
{
     SignaturaResultus  r;
    constans character* vitium;
          SignaturaPar* addita;
                   i32  numerus = I;
                chorda  dies_brevis;
                chorda  ambitus;
        CapitaCanonica  capita;
     ChordaAedificator* a;

    si (!sp)
    {
        redde _error("signatura_aws: petitio NIHIL", piscina);
    }
    vitium = _vitium(sp, FALSUM);
    si (vitium)
    {
        redde _error(vitium, piscina);
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.dies_amz   = signatura_aws_dies(sp->tempus, piscina);
    dies_brevis  = chorda_sectio(r.dies_amz, ZEPHYRUM, VIII);
    ambitus      = _ambitus(sp, dies_brevis, piscina);

    addita = (SignaturaPar*)piscina_allocare(piscina,
                 (memoriae_index)(II * (i32)magnitudo(SignaturaPar)));
    addita[ZEPHYRUM].titulus  = _literae("x-amz-date", piscina);
    addita[ZEPHYRUM].valor    = r.dies_amz;
    si (sp->corpus_capite)
    {
        addita[I].titulus = _literae("x-amz-content-sha256", piscina);
        addita[I].valor   = sp->corpus_sigillum.mensura > ZEPHYRUM
                                ? sp->corpus_sigillum
                                : _literae(sigillum_vacui, piscina);
        numerus = II;
    }
    capita = _capita_canonica(sp, addita, numerus, piscina);
    _perficere(sp, _quaestio_canonica(sp, NIHIL, ZEPHYRUM, piscina),
        &capita, dies_brevis, ambitus, &r, piscina);

    a = chorda_aedificator_creare(piscina, CCLVI);
    chorda_aedificator_appendere_literis(a, algorithmus_aws);
    chorda_aedificator_appendere_literis(a, " Credential=");
    chorda_aedificator_appendere_chorda(a, sp->clavis_id);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a, ambitus);
    chorda_aedificator_appendere_literis(a, ", SignedHeaders=");
    chorda_aedificator_appendere_chorda(a, capita.signata);
    chorda_aedificator_appendere_literis(a, ", Signature=");
    chorda_aedificator_appendere_chorda(a, r.signatura);
    r.auctoritas             = chorda_aedificator_finire(a);
    r.capita_addita          = addita;
    r.capita_addita_numerus  = numerus;
    r.successus              = VERUM;
    redde r;
}

SignaturaResultus
signatura_aws_quaestione (
    constans SignaturaPetitio* sp,
                      Piscina* piscina)
{
     SignaturaResultus  r;
    constans character* vitium;
          SignaturaPar  addita[V];
                chorda  dies_brevis;
                chorda  ambitus;
        CapitaCanonica  capita;
     ChordaAedificator* a;
             character  duratio[XVI];
                chorda  quaestio;

    si (!sp)
    {
        redde _error("signatura_aws: petitio NIHIL", piscina);
    }
    vitium = _vitium(sp, VERUM);
    si (vitium)
    {
        redde _error(vitium, piscina);
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.dies_amz   = signatura_aws_dies(sp->tempus, piscina);
    dies_brevis  = chorda_sectio(r.dies_amz, ZEPHYRUM, VIII);
    ambitus      = _ambitus(sp, dies_brevis, piscina);
    capita       = _capita_canonica(sp, NIHIL, ZEPHYRUM, piscina);

    a = chorda_aedificator_creare(piscina, CXXVIII);
    chorda_aedificator_appendere_chorda(a, sp->clavis_id);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a, ambitus);
    sprintf(duratio, "%d", (integer)sp->duratio);

    addita[ZEPHYRUM].titulus = _literae("X-Amz-Algorithm", piscina);
    addita[ZEPHYRUM].valor = _literae(algorithmus_aws, piscina);
    addita[I].titulus = _literae("X-Amz-Credential", piscina);
    addita[I].valor = chorda_aedificator_finire(a);
    addita[II].titulus = _literae("X-Amz-Date", piscina);
    addita[II].valor = r.dies_amz;
    addita[III].titulus = _literae("X-Amz-Expires", piscina);
    addita[III].valor = chorda_ex_literis(duratio, piscina);
    addita[IV].titulus = _literae("X-Amz-SignedHeaders", piscina);
    addita[IV].valor = capita.signata;
    quaestio = _quaestio_canonica(sp, addita, V, piscina);
    _perficere(sp, quaestio, &capita, dies_brevis, ambitus, &r,
        piscina);

    a = chorda_aedificator_creare(piscina,
            (memoriae_index)(quaestio.mensura + C));
    chorda_aedificator_appendere_chorda(a, quaestio);
    chorda_aedificator_appendere_literis(a, "&X-Amz-Signature=");
    chorda_aedificator_appendere_chorda(a, r.signatura);
    r.quaestio_signata  = chorda_aedificator_finire(a);
    r.successus         = VERUM;
    redde r;
}
