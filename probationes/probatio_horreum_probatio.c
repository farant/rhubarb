/* probatio_horreum_probatio.c - horreum probare (horreum-plan T6) contra
 * S3 fictum per vecturam: profilum digitalocean (stilus hospitis,
 * capacitates creditae paucae) - omnia permissa et probata; simulacrum
 * neglegens (condiciones et summae neglectae) - refutata nominata;
 * purgatio nihil relinquit; responsa servata purgata. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum.h"
#include "horreum_cliens.h"
#include "horreum_probatio.h"
#include "horreum_simulacrum.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SECRETUM   "SECRETUM-SIMULACRI-0123456789"
#define CLAVIS_ID  "AKIDSIMULACRI"

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior Horreum*
_horreum_do (
    Piscina* p)
{
    HorreumConfiguratio c;
           HorreumError e;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.provisor   = "digitalocean";
    c.terminus   = _l("https://simulacrum.invalid", p);
    c.regio      = _l("us-east-1", p);
    c.situla     = _l(SIMULACRUM_SITULA, p);
    c.clavis_id  = _l(CLAVIS_ID, p);
    c.secretum   = _l(SECRETUM, p);
    redde horreum_creare(&c, &e, p);
}

interior constans HorreumProbatioLinea*
_linea (
    constans HorreumProbatio* r,
          constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < r->numerus; i++)
    {
        si (strcmp(r->lineae[i].titulus, titulus) == ZEPHYRUM)
        {
            redde &r->lineae[i];
        }
    }
    redde NIHIL;
}

/* verdictum lineae; causa impressa si alienum */
interior b32
_verdictum (
    constans HorreumProbatio* r,
          constans character* titulus,
            HorreumVerdictum  expectatum)
{
    constans HorreumProbatioLinea* l = _linea(r, titulus);

    si (!l)
    {
        imprimere("  linea deest: %s\n", titulus);
        redde FALSUM;
    }
    si (l->verdictum != expectatum)
    {
        imprimere("  %s: %s (%.*s)\n", titulus,
            horreum_verdictum_nomen(l->verdictum),
            (integer)l->causa.mensura,
            (constans character*)l->causa.datum);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_directorium (
    constans character* titulus,
               Piscina* p)
{
    character via[CCLVI];

    sprintf(via, "/tmp/horreum-%ld-%s", (longus)getpid(), titulus);
    (vacuum)filum_arborem_delere(via);
    redde _l(via, p);
}

interior b32
_onera_viva (
    Simulacrum* s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s->onera_numerus; i++)
    {
        si (s->onera[i].vivum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ===== simulacrum_omnia: provisor honestus, omnia probata ===== */
interior vacuum
_sectio_simulacrum_omnia (
    CredoContextus* c)
{
             Piscina* p = c->piscina;
          Simulacrum* s = simulacrum_creare(_l(CLAVIS_ID, p),
                              _l(SECRETUM, p), _l("us-east-1", p), p);
               Horreum* h = _horreum_do(p);
       HorreumProbandi  o;
       HorreumProbatio  r;
          HorreumError  e;
                chorda  d = _directorium("omnia", p);
                chorda  via;
                chorda  corpus;
                   i32  i;
    constans character* tituli[] = { "iter", "solum_si_abest",
        "solum_si_etag", "acl", "checksum_crc32", "checksum_sha256",
        "delere_plura", "enumerare", "multipars", "praesignare",
        "purgatio" };
    constans character* fila[] = { "enumerare.xml", "delere_plura.xml",
        "error_abest.xml", "error_conditio.xml", "mp_incipere.xml",
        "mp_partes.xml", "mp_pendentes.xml", "mp_perficere.xml" };

    CREDO_NECESSE_NON_NIHIL(h);
    /* obiectum alienum extra praefixum manere debet; 503 semel
     * iteratur; horologium servi verum */
    simulacrum_obiectum_ponere(s, _l("alienum.txt", p), _l("manet", p));
    simulacrum_horam_ponere(s, (MomentumSec)time(NIHIL));
    {
        SimulacrumVitium v;

        memset(&v, ZEPHYRUM, magnitudo(v));
        v.genus     = SIMULACRUM_VITIUM_STATUS;
        v.methodus  = HTTP_PUT;
        v.status    = DIII;
        v.codex     = "SlowDown";
        v.numerus   = I;
        simulacrum_vitium_addere(s, v);
    }
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.vectura   = simulacrum_vectura(s);
    o.id        = _l("probatio", p);
    o.responsa  = chorda_ut_cstr(d, p);
    CREDO_VERUM(horreum_probare(h, &o, &r, &e, p));
    CREDO_AEQUALIS_I32(r.numerus, XI);
    per (i = ZEPHYRUM; i < XI; i++)
    {
        CREDO_VERUM(_verdictum(&r, tituli[i], HORREUM_PROBATUM));
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(r.praefixum,
        "horreum-probatio/probatio/");
    CREDO_CHORDA_CONTINET(_linea(&r, "acl")->causa, _l("sine signatura",
        p));
    CREDO_CHORDA_AEQUALIS_LITERIS(r.caput_petitionis,
        "x-amz-request-id");
    CREDO_CHORDA_TERMINATUR(r.clavis_difficilis, _l("//x/", p));
    CREDO_AEQUALIS_S32(r.relicta, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->numerus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(simulacrum_obiectum_corpus(s,
        _l("alienum.txt", p)), "manet");
    CREDO_FALSUM(_onera_viva(s));
    /* pars una per petitionem, V MiB */
    CREDO_AEQUALIS_I32(simulacrum_corpus_maximum(s), V * MXXIV * MXXIV);

    per (i = ZEPHYRUM; i < VIII; i++)
    {
        via = chorda_concatenare(d, _l("/", p), p);
        via = chorda_concatenare(via, _l(fila[i], p), p);
        CREDO_VERUM(filum_existit(chorda_ut_cstr(via, p)));
        corpus = filum_legere_totum(chorda_ut_cstr(via, p), p);
        CREDO_FALSUM(chorda_continet(corpus, _l(SECRETUM, p)));
    }
    simulacrum_claudere(s);
    horreum_claudere(h);
    CREDO_VERUM(filum_arborem_delere(chorda_ut_cstr(d, p)));
}

/* ===== simulacrum_neglegens: condiciones et summae neglectae ===== */
interior vacuum
_sectio_simulacrum_neglegens (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
      Simulacrum* s = simulacrum_creare(_l(CLAVIS_ID, p), _l(SECRETUM,
          p),
                          _l("us-east-1", p), p);
         Horreum* h = _horreum_do(p);
 HorreumProbandi  o;
 HorreumProbatio  r;
    HorreumError  e;

    CREDO_NECESSE_NON_NIHIL(h);
    s->neglegere_condiciones  = VERUM;
    s->neglegere_summas       = VERUM;
    s->neglegere_acl          = VERUM;
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.vectura = simulacrum_vectura(s);
    CREDO_VERUM(horreum_probare(h, &o, &r, &e, p));
    CREDO_VERUM(_verdictum(&r, "solum_si_abest", HORREUM_REFUTATUM));
    CREDO_VERUM(_verdictum(&r, "solum_si_etag", HORREUM_REFUTATUM));
    CREDO_VERUM(_verdictum(&r, "checksum_crc32", HORREUM_REFUTATUM));
    CREDO_VERUM(_verdictum(&r, "checksum_sha256", HORREUM_REFUTATUM));
    CREDO_VERUM(_verdictum(&r, "acl", HORREUM_REFUTATUM));
    CREDO_VERUM(_verdictum(&r, "iter", HORREUM_PROBATUM));
    CREDO_VERUM(_verdictum(&r, "multipars", HORREUM_PROBATUM));
    CREDO_VERUM(_verdictum(&r, "praesignare", HORREUM_PROBATUM));
    CREDO_VERUM(_verdictum(&r, "purgatio", HORREUM_PROBATUM));
    CREDO_CHORDA_CONTINET(_linea(&r, "checksum_crc32")->causa,
        _l("falsa",
        p));
    CREDO_CHORDA_INCIPIT(r.praefixum, _l("horreum-probatio/", p));
    CREDO_AEQUALIS_S32(r.relicta, ZEPHYRUM);
    CREDO_AEQUALIS_I32(s->numerus, ZEPHYRUM);
    simulacrum_claudere(s);
    horreum_claudere(h);
}

/* ===== id_oneris_alienum: ListMultipartUploads id aliud reddit (R2) ===== */
interior vacuum
_sectio_id_oneris_alienum (
    CredoContextus* c)
{
         Piscina* p = c->piscina;
      Simulacrum* s = simulacrum_creare(_l(CLAVIS_ID, p), _l(SECRETUM,
          p),
                          _l("us-east-1", p), p);
         Horreum* h = _horreum_do(p);
 HorreumProbandi  o;
 HorreumProbatio  r;
    HorreumError  e;

    CREDO_NECESSE_NON_NIHIL(h);
    s->id_oneris_alienum = VERUM;
    memset(&o, ZEPHYRUM, magnitudo(o));
    o.vectura = simulacrum_vectura(s);
    CREDO_VERUM(horreum_probare(h, &o, &r, &e, p));
    CREDO_VERUM(_verdictum(&r, "multipars", HORREUM_PROBATUM));
    CREDO_VERUM(_verdictum(&r, "purgatio", HORREUM_PROBATUM));
    CREDO_FALSUM(_onera_viva(s));
    simulacrum_claudere(s);
    horreum_claudere(h);
}

/* ===== purgare: ID et DisplayName ===== */
interior vacuum
_sectio_purgare (
    CredoContextus* c)
{
    Piscina* p = c->piscina;
     chorda  ante = _l("<Upload><Initiator><ID>arn:abc:123</ID>"
         "<DisplayName>fran</DisplayName></Initiator><Owner><ID>9988</ID>"
         "</Owner><Key>k</Key></Upload>", p);
     chorda post = horreum_responsum_purgare(ante, p);

    CREDO_CHORDA_AEQUALIS_LITERIS(post,
        "<Upload><Initiator><ID>SCRUBBED"
        "</ID><DisplayName>SCRUBBED</DisplayName></Initiator><Owner><ID>"
        "SCRUBBED</ID></Owner><Key>k</Key></Upload>");
    CREDO_CHORDA_AEQUALIS_LITERIS(horreum_responsum_purgare(_l(
        "<a>nihil</a>", p), p), "<a>nihil</a>");
    /* R2: Location fert rationarium in hospite */
    CREDO_CHORDA_AEQUALIS_LITERIS(horreum_responsum_purgare(_l(
        "<Location>https://0123abcd.r2.cloudflarestorage.com/s/k</Location>",
        p), p), "<Location>SCRUBBED</Location>");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "simulacrum_omnia", _sectio_simulacrum_omnia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "simulacrum_neglegens", _sectio_simulacrum_neglegens, NIHIL,
        NIHIL,
        NIHIL, NIHIL },
    { "id_oneris_alienum", _sectio_id_oneris_alienum, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "purgare", _sectio_purgare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("horreum_probatio", SECTIONES);
}
