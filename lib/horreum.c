/* horreum.c - nucleus horrei (vide horreum.h; horreum-plan T2)
 *
 * Secretum et clavis_id in capsis fixis intra structuram Horreum (non
 * in piscina sparsa), ut horreum_claudere eas zephyret. Petitio
 * signata: via DECODIFICATA signatori datur, filo via
 * signatura_aws_codificare(servare '/') - eadem functio, ergo via
 * signata == via missa per constructionem. Quaestio missa = quaestio
 * canonica (linea III petitionis canonicae). */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum.h"
#include "signatura_aws.h"
#include "sigillum.h"
#include "friatio.h"
#include "base64.h"
#include "xml.h"
#include "internamentum.h"
#include "chorda_aedificator.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define HORREUM_HOSPES_MAXIMUS   CCLVI
#define HORREUM_TABELLA_MAXIMA (IV * MXXIV)
#define HORREUM_CAPITA_OPERIS    XXXII
#define HORREUM_QUAESTIO_OPERIS  XVI

structura Horreum {
    constans HorreumProvisor* provisor;
                   character  schema[VIII];
                   character  hospes[HORREUM_HOSPES_MAXIMUS];
                   character  regio[LXIV];
                   character  situla[HORREUM_HOSPES_MAXIMUS];
                         i32  capacitates;
                         i32  probatae;
               HorreumStilus  stilus;
                   character  clavis_id[HORREUM_CAPSA_CLAVIS];
                   character  secretum[HORREUM_CAPSA_CLAVIS];
                      vacuum* sedes;          /* executoris */
};

/* provisores: capacitates "creditum" (documenta) donec horreum probare
 * eas probat (probatae) aut refutat (capacitas deletur); responsa vera
 * in probationes/fixa/horreum/<provisor>/ */
hic_manens constans HorreumProvisor provisores_horrei[] = {
    /* probatum 2026-10-22 (horreum probare): acl (public-read legitur
     * sine signatura), solum_si_abest, delere_plura. refutatum
     * 2026-10-22: solum_si_etag (If-Match in PUT semper 412, etiam
     * ETag verum - in GET valet), checksum crc32/sha256 (summa falsa
     * accepta) */
    { "digitalocean", "{regio}.digitaloceanspaces.com",
        HORREUM_STILUS_HOSPES,
      NIHIL, HORREUM_CAP_ACL | HORREUM_CAP_DELERE_PLURA
          | HORREUM_CAP_SOLUM_SI_ABEST,
      HORREUM_CAP_ACL | HORREUM_CAP_DELERE_PLURA
          | HORREUM_CAP_SOLUM_SI_ABEST, FALSUM },
    /* probatum 2026-10-22 (horreum probare): solum_si_abest,
     * solum_si_etag, delere_plura, checksum crc32/sha256 (falsa:
     * BadDigest). refutatum 2026-10-22: acl (x-amz-acl acceptum sed
     * obiectum non publicum - accessus publicus per situlam). Nota:
     * ListMultipartUploads UploadId ALIUM reddit quam Create; utrumque
     * valet */
    { "r2", "{rationarium}.r2.cloudflarestorage.com",
        HORREUM_STILUS_HOSPES,
      "auto", HORREUM_CAP_SOLUM_SI_ABEST | HORREUM_CAP_SOLUM_SI_ETAG
          | HORREUM_CAP_DELERE_PLURA | HORREUM_CAP_CHECKSUM_CRC32
          | HORREUM_CAP_CHECKSUM_SHA256,
      HORREUM_CAP_SOLUM_SI_ABEST | HORREUM_CAP_SOLUM_SI_ETAG
          | HORREUM_CAP_DELERE_PLURA | HORREUM_CAP_CHECKSUM_CRC32
          | HORREUM_CAP_CHECKSUM_SHA256, FALSUM },
    /* creditum: backblaze.com/docs/cloud-storage-s3-compatible-api */
    { "b2", "s3.{regio}.backblazeb2.com", HORREUM_STILUS_HOSPES, NIHIL,
      HORREUM_CAP_DELERE_PLURA, ZEPHYRUM, FALSUM },
    /* generic: omnia credita, NIHIL probatum - repositorium ignotum
     * If-None-Match neglegere potest, ergo solum_si_abest solum cum
     * periculo_accipere (spec II.4; recensio I4 2026-10-22) */
    { "generic", NIHIL, HORREUM_STILUS_VIA, NIHIL,
      HORREUM_CAP_ACL | HORREUM_CAP_SOLUM_SI_ABEST
          | HORREUM_CAP_SOLUM_SI_ETAG
          | HORREUM_CAP_DELERE_PLURA | HORREUM_CAP_CHECKSUM_CRC32
          | HORREUM_CAP_CHECKSUM_SHA256,
      ZEPHYRUM, VERUM },
    { NIHIL, NIHIL, HORREUM_STILUS_HOSPES, NIHIL, ZEPHYRUM, ZEPHYRUM,
        FALSUM }
};

hic_manens constans character sigillum_vacui_horrei[] =
    "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

/* ===== auxilia ===== */
interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior vacuum
_errorem_ponere (
         HorreumError* e,
    HorreumErrorGenus  genus,
                  i32  status,
   constans character* nuntius,
              Piscina* p)
{
    si (!e)
    {
        redde;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    e->genus    = genus;
    e->status   = status;
    e->nuntius  = _l(nuntius, p);
}

interior b32
_cifra_hex (
    i8 c)
{
    redde (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

interior chorda
_hex (
    constans i8* octeti,
            i32  n,
        Piscina* p)
{
    hic_manens constans character  cifrae[] = "0123456789abcdef";
                        character* t = (character*)piscina_allocare(p,
                            (memoriae_index)(n * II + I));
          i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        t[i * II]      = cifrae[((i32)octeti[i] >> IV) & 0x0F];
        t[i * II + I]  = cifrae[(i32)octeti[i] & 0x0F];
    }
    t[n * II] = '\0';
    redde chorda_ex_buffer((i8*)t, n * II);
}

interior chorda
_sha256_hex (
     chorda  corpus,
    Piscina* p)
{
     Sigillum s = sigillum_computare(corpus.datum,
                      (memoriae_index)corpus.mensura);
    character capsa[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, capsa);
    redde _l(capsa, p);
}

/* chorda in capsam terminatam; FALSUM si non capit */
interior b32
_in_capsam (
        chorda  s,
     character* capsa,
           i32  capacitas)
{
    si (s.mensura >= capacitas)
    {
        redde FALSUM;
    }
    si (s.mensura > ZEPHYRUM)
    {
        memcpy(capsa, s.datum, (size_t)s.mensura);
    }
    capsa[s.mensura] = '\0';
    redde VERUM;
}

interior chorda
_capsa (
    character* capsa)
{
    redde chorda_ex_buffer((i8*)capsa, (i32)strlen(capsa));
}

/* chorda virgulis duplicibus liberata ("abc" -> abc) */
interior chorda
_sine_virgulis (
    chorda s)
{
    si (   s.mensura              >= II && s.datum[ZEPHYRUM] == '"'
        && s.datum[s.mensura - I] == '"')
    {
        redde chorda_sectio(s, I, s.mensura - I);
    }
    redde s;
}

interior chorda
_cum_virgulis (
     chorda  s,
    Piscina* p)
{
    ChordaAedificator* a;

    si (s.mensura >= II && s.datum[ZEPHYRUM] == '"')
    {
        redde s;
    }
    a = chorda_aedificator_creare(p, (memoriae_index)(s.mensura + III));
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_chorda(a, s);
    chorda_aedificator_appendere_character(a, '"');
    redde chorda_aedificator_finire(a);
}

interior s64
_numerus (
    chorda s)
{
    s64 n = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] < '0' || s.datum[i] > '9')
        {
            redde -I;
        }
        n = n * X + (s64)(s.datum[i] - '0');
    }
    redde s.mensura > ZEPHYRUM ? n : -I;
}

interior chorda
_numerus_chorda (
        s64  n,
    Piscina* p)
{
    character capsa[XXIV];

    sprintf(capsa, "%lld", (long long)n);
    redde _l(capsa, p);
}

/* XML: & < > " ' effugiuntur */
interior vacuum
_xml_effugere (
    ChordaAedificator* a,
               chorda  s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        commutatio (s.datum[i])
        {
            casus '&': chorda_aedificator_appendere_literis(a, "&amp;");
                frange;
            casus '<': chorda_aedificator_appendere_literis(a, "&lt;");
                frange;
            casus '>': chorda_aedificator_appendere_literis(a, "&gt;");
                frange;
            casus '"': chorda_aedificator_appendere_literis(a,
                           "&quot;");
                frange;
            casus '\'': chorda_aedificator_appendere_literis(a,
                            "&apos;");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)s.datum[i]);
        }
    }
}

/* ===== tempora (P7) ===== */
MomentumSec
horreum_tempus_iso (
    chorda s)
{
    character capsa[XXXII];
          i32 k = ZEPHYRUM;
          i32 i;
          b32 in_fractione = FALSUM;
     DiesHora dh;

    si (s.mensura <= ZEPHYRUM || s.mensura >= (i32)magnitudo(capsa))
    {
        redde ZEPHYRUM;
    }
    /* fractio secundorum (.000) praeteritur: fasti_ex_iso eam
     * recusat */
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == '.')
        {
            in_fractione = VERUM;
            perge;
        }
        si (in_fractione && s.datum[i] >= '0' && s.datum[i] <= '9')
        {
            perge;
        }
        in_fractione  = FALSUM;
        capsa[k++]    = (character)s.datum[i];
    }
    capsa[k] = '\0';
    si (!fasti_ex_iso(chorda_ex_buffer((i8*)capsa, k), &dh))
    {
        redde ZEPHYRUM;
    }
    redde fasti_ad_unix(dh);
}

hic_manens constans character* menses_http[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* IMF-fixdate: "Wed, 21 Oct 2026 14:09:00 GMT" */
MomentumSec
horreum_tempus_http (
    chorda s)
{
    character capsa[XXXII];
    character mensis[IV];
      integer d, an, horae, mi, se;
          i32 k;
    character iso[XXXII];
     DiesHora dh;

    si (s.mensura < XXIX || s.mensura >= (i32)magnitudo(capsa))
    {
        redde ZEPHYRUM;
    }
    memcpy(capsa, s.datum, (size_t)s.mensura);
    capsa[s.mensura] = '\0';
    si (sscanf(capsa + V, "%2d %3s %4d %2d:%2d:%2d", &d, mensis, &an,
        &horae,
            &mi, &se) != VI)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < XII; k++)
    {
        si (strcmp(mensis, menses_http[k]) == ZEPHYRUM)
        {
            frange;
        }
    }
    si (k == XII)
    {
        redde ZEPHYRUM;
    }
    sprintf(iso, "%04d-%02d-%02dT%02d:%02d:%02dZ", an, (integer)(k + I),
        d,
        horae, mi, se);
    si (!fasti_ex_iso(_capsa(iso), &dh))
    {
        redde ZEPHYRUM;
    }
    redde fasti_ad_unix(dh);
}

interior chorda
_tempus_http_scribere (
    MomentumSec  t,
        Piscina* p)
{
    hic_manens constans character* dies_hebdomadis[] = {
        "Thu", "Fri", "Sat", "Sun", "Mon", "Tue", "Wed"
    };
     DiesHora dh = fasti_ex_unix(t);
    character capsa[XL];
          s64 dies = (s64)t / (s64)86400L;

    sprintf(capsa, "%s, %02d %s %04d %02d:%02d:%02d GMT",
        dies_hebdomadis[(i32)(dies % VII)], (integer)dh.dies.dies,
        menses_http[dh.dies.mensis - I], (integer)dh.dies.annus,
        (integer)dh.hora.hora, (integer)dh.hora.minutum,
        (integer)dh.hora.secundum);
    redde _l(capsa, p);
}

/* ===== provisores et creatio ===== */
constans HorreumProvisor*
horreum_provisor (
    constans character* titulus)
{
    i32 k;

    si (!titulus)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; provisores_horrei[k].titulus; k++)
    {
        si (strcmp(provisores_horrei[k].titulus, titulus) == ZEPHYRUM)
        {
            redde &provisores_horrei[k];
        }
    }
    redde NIHIL;
}

/* forma hospitis: {regio} et {rationarium} substituuntur */
interior b32
_formam_implere (
     constans character* forma,
                 chorda  regio,
                 chorda  rationarium,
              character* exitus,
                    i32  capacitas)
{
    i32 k = ZEPHYRUM;

    dum (*forma)
    {
        constans character* fons = NIHIL;
                    chorda  valor;

        si (strncmp(forma, "{regio}", VII) == ZEPHYRUM)
        {
            valor  = regio;
            fons   = "{regio}";
        }
        alioquin si (strncmp(forma, "{rationarium}", XIII) == ZEPHYRUM)
        {
            valor  = rationarium;
            fons   = "{rationarium}";
        }
        si (fons)
        {
            si (k + valor.mensura >= capacitas)
            {
                redde FALSUM;
            }
            memcpy(exitus + k, valor.datum, (size_t)valor.mensura);
            k      += valor.mensura;
            forma  += strlen(fons);
            perge;
        }
        si (k + I >= capacitas)
        {
            redde FALSUM;
        }
        exitus[k++] = *forma++;
    }
    exitus[k] = '\0';
    redde VERUM;
}

Horreum*
horreum_creare (
    constans HorreumConfiguratio* c,
                    HorreumError* e,
                         Piscina* p)
{
                     Horreum  h;
                     Horreum* exitus;
    constans HorreumProvisor* pv;
                   character  nuntius[CXXVIII];
                      chorda  regio;

    si (!c || !p)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: configuratio NIHIL", p);
        redde NIHIL;
    }
    pv = horreum_provisor(c->provisor);
    si (!pv)
    {
        sprintf(nuntius, "horreum: provisor ignotus: %.40s",
            c->provisor ? c->provisor : "(nullus)");
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde NIHIL;
    }
    memset(&h, ZEPHYRUM, magnitudo(h));
    h.provisor     = pv;
    h.stilus       = pv->stilus;
    h.capacitates  = pv->capacitates;
    h.probatae     = pv->probatae;
    si (c->clavis_id.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis_id vacua", p);
        redde NIHIL;
    }
    si (c->secretum.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: secretum vacuum", p);
        redde NIHIL;
    }
    si (   !_in_capsam(c->clavis_id, h.clavis_id, HORREUM_CAPSA_CLAVIS)
        || !_in_capsam(c->secretum, h.secretum, HORREUM_CAPSA_CLAVIS))
    {
        memset(&h, ZEPHYRUM, magnitudo(h));
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis_id aut secretum longius quam CCLV octeti",
            p);
        redde NIHIL;
    }
    si (c->terminus.mensura == ZEPHYRUM && !pv->hospes_forma)
    {
        memset(&h, ZEPHYRUM, magnitudo(h));
        sprintf(nuntius, "horreum: terminus necessarius (provisor %s)",
            pv->titulus);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde NIHIL;
    }
    regio = c->regio.mensura > ZEPHYRUM ? c->regio
          : _l(pv->regio ? pv->regio : "", p);
    si (regio.mensura == ZEPHYRUM)
    {
        memset(&h, ZEPHYRUM, magnitudo(h));
        sprintf(nuntius, "horreum: regio necessaria (provisor %s)",
            pv->titulus);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde NIHIL;
    }
    (vacuum)_in_capsam(regio, h.regio, (i32)magnitudo(h.regio));

    si (c->terminus.mensura > ZEPHYRUM)
    {
        chorda t = c->terminus;
        chorda reliquum;

        si (chorda_incipit(t, _l("https://", p)))
        {
            strcpy(h.schema, "https");
            reliquum = chorda_sectio(t, VIII, t.mensura);
        }
        alioquin si (chorda_incipit(t, _l("http://", p)))
        {
            si (!pv->http_licet)
            {
                memset(&h, ZEPHYRUM, magnitudo(h));
                _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                    "horreum: terminus http non licet (solum https; "
                    "http solum provisori generic)", p);
                redde NIHIL;
            }
            strcpy(h.schema, "http");
            reliquum = chorda_sectio(t, VII, t.mensura);
        }
        alioquin
        {
            memset(&h, ZEPHYRUM, magnitudo(h));
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: terminus debet incipere https://", p);
            redde NIHIL;
        }
        si (   reliquum.mensura > ZEPHYRUM
            && reliquum.datum[reliquum.mensura - I] == '/')
        {
            reliquum.mensura--;
        }
        si (   reliquum.mensura == ZEPHYRUM
            || chorda_continet(reliquum, _l("/", p))
            || !_in_capsam(reliquum, h.hospes, HORREUM_HOSPES_MAXIMUS))
        {
            memset(&h, ZEPHYRUM, magnitudo(h));
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: terminus malus (https://hospes[:portus])", p);
            redde NIHIL;
        }
    }
    alioquin
    {
        si (!pv->hospes_forma)
        {
            memset(&h, ZEPHYRUM, magnitudo(h));
            sprintf(nuntius,
                "horreum: terminus necessarius (provisor %s)",
                pv->titulus);
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
            redde NIHIL;
        }
        si (   strstr(pv->hospes_forma, "{rationarium}")
            && c->rationarium.mensura == ZEPHYRUM)
        {
            memset(&h, ZEPHYRUM, magnitudo(h));
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: rationarium necessarium (aut terminus)", p);
            redde NIHIL;
        }
        strcpy(h.schema, "https");
        si (!_formam_implere(pv->hospes_forma, regio, c->rationarium,
                h.hospes, HORREUM_HOSPES_MAXIMUS))
        {
            memset(&h, ZEPHYRUM, magnitudo(h));
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: hospes nimis longus", p);
            redde NIHIL;
        }
    }
    (vacuum)_in_capsam(c->situla, h.situla, HORREUM_HOSPES_MAXIMUS);

    exitus = (Horreum*)piscina_allocare(p,
        (memoriae_index)magnitudo(Horreum));
    *exitus = h;
    memset(&h, ZEPHYRUM, magnitudo(h));
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde exitus;
}

chorda
horreum_situla_ordinaria (
    Horreum* h)
{
    si (!h)
    {
        redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    redde chorda_ex_buffer((i8*)h->situla, (i32)strlen(h->situla));
}

constans HorreumProvisor*
horreum_provisor_horrei (
    constans Horreum* h)
{
    redde h ? h->provisor : NIHIL;
}

vacuum
horreum_claudere (
    Horreum* h)
{
    si (h)
    {
        memset(h, ZEPHYRUM, magnitudo(*h));
    }
}

vacuum
horreum_sedem_ponere (
    Horreum* h,
     vacuum* sedes)
{
    si (h)
    {
        h->sedes = sedes;
    }
}

vacuum*
horreum_sedes (
    Horreum* h)
{
    redde h ? h->sedes : NIHIL;
}

vacuum
horreum_omnia_permittere (
    Horreum* h)
{
    i32 omnes = HORREUM_CAP_ACL | HORREUM_CAP_SOLUM_SI_ABEST
              | HORREUM_CAP_SOLUM_SI_ETAG | HORREUM_CAP_CHECKSUM_CRC32
              | HORREUM_CAP_CHECKSUM_SHA256 | HORREUM_CAP_DELERE_PLURA;

    si (!h)
    {
        redde;
    }
    h->capacitates  = omnes;
    h->probatae     = omnes;
}

/* ===== tabella (P5) ===== */
/* clavis "nominabilis" (nomen in errore dici potest): [a-z_], <= XVI -
 * linea positionalis (secretum nudum) numquam nominatur */
interior b32
_clavis_dicenda (
    chorda k)
{
    i32 i;

    si (k.mensura == ZEPHYRUM || k.mensura > XVI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < k.mensura; i++)
    {
        si (!((k.datum[i] >= 'a' && k.datum[i] <= 'z')
            || k.datum[i] == '_'))
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
    redde c == ' ' || c == '\t' || c == '\r';
}

interior chorda
_praecidere (
    chorda s)
{
    dum (s.mensura > ZEPHYRUM && _spatium(s.datum[ZEPHYRUM]))
    {
        s = chorda_sectio(s, I, s.mensura);
    }
    dum (s.mensura > ZEPHYRUM && _spatium(s.datum[s.mensura - I]))
    {
        s.mensura--;
    }
    redde s;
}

/* lectio CRUDA (numquam lectiones), ut oceanus/custodia; capsa in
 * acervo vocantis zephyratur */
interior s32
_tabellam_legere (
     constans character* via,
              character* capsa,
                    i32  capacitas,
           HorreumError* e,
                Piscina* p)
{
       integer fd;
    structura stat st;
       ssize_t n;
     character nuntius[CCLVI];

    /* <tolera codex="CANALIS_PRAETERITUS" (>clavis secreta horrei */
    fd = open(via, O_RDONLY | O_NOFOLLOW);
    si (fd < ZEPHYRUM)
    {
        sprintf(nuntius, "horreum: tabella legi nequit: %.200s", via);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde -I;
    }
    si (fstat(fd, &st) != ZEPHYRUM || !S_ISREG(st.st_mode))
    {
        close(fd);
        sprintf(nuntius, "horreum: tabella non filum regulare: %.200s",
            via);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde -I;
    }
    si ((st.st_mode & 077) != ZEPHYRUM)
    {
        close(fd);
        sprintf(nuntius,
            "horreum: tabella modus latior quam 0600 (chmod "
            "600 %.200s)", via);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde -I;
    }
    n = read(fd, capsa, (size_t)(capacitas - I));
    close(fd);
    si (n <= ZEPHYRUM)
    {
        sprintf(nuntius, "horreum: tabella vacuum: %.200s", via);
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
        redde -I;
    }
    capsa[n] = '\0';
    redde (s32)n;
}

Horreum*
horreum_ex_tabella (
    constans character* via,
          HorreumError* e,
               Piscina* p)
{
    hic_manens constans character* claves_tabellae[] = {
        "provisor", "terminus", "regio", "rationarium", "situla",
        "clavis_id", "secretum", NIHIL
    };
               character  capsa[HORREUM_TABELLA_MAXIMA];
               character  provisor[XXXII];
                  chorda  valores[VII];
                  chorda  reliquum;
                     s32  n;
                     i32  linea = ZEPHYRUM;
                     i32  k;
               character  nuntius[CXXVIII];
     HorreumConfiguratio  c;
                 Horreum* h = NIHIL;

    si (!via)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: via NIHIL", p);
        redde NIHIL;
    }
    n = _tabellam_legere(via, capsa, (i32)magnitudo(capsa), e, p);
    si (n < ZEPHYRUM)
    {
        redde NIHIL;
    }
    memset(valores, ZEPHYRUM, magnitudo(valores));
    reliquum = chorda_ex_buffer((i8*)capsa, (i32)n);
    dum (reliquum.mensura > ZEPHYRUM)
    {
           s32 nl = chorda_invenire_index(reliquum, _l("\n", p));
        chorda l = nl < ZEPHYRUM ? reliquum
                                 : chorda_sectio(reliquum, ZEPHYRUM,
                                 (i32)nl);
        chorda titulus;
        chorda valor;
           i32 i;

        linea++;
        reliquum = nl < ZEPHYRUM ? chorda_sectio(reliquum,
            reliquum.mensura,
                                       reliquum.mensura)
                                 : chorda_sectio(reliquum, (i32)nl + I,
                                       reliquum.mensura);
        /* commentum: linea '#', aut ' #' / '\t#' post valorem */
        per (i = ZEPHYRUM; i < l.mensura; i++)
        {
            si (   l.datum[i] == '#'
                && (i == ZEPHYRUM || _spatium(l.datum[i - I])))
            {
                l.mensura = i;
                frange;
            }
        }
        l = _praecidere(l);
        si (l.mensura == ZEPHYRUM)
        {
            perge;
        }
        per (i = ZEPHYRUM; i < l.mensura && !_spatium(l.datum[i]); i++)
        {
        }
        titulus  = chorda_sectio(l, ZEPHYRUM, i);
        valor    = _praecidere(chorda_sectio(l, i, l.mensura));
        per (k = ZEPHYRUM; claves_tabellae[k]; k++)
        {
            si (chorda_aequalis_literis(titulus, claves_tabellae[k]))
            {
                frange;
            }
        }
        si (!claves_tabellae[k])
        {
            si (_clavis_dicenda(titulus))
            {
                sprintf(nuntius,
                    "horreum: tabella: clavis ignota '%.*s' "
                    "(linea %d)", (integer)titulus.mensura,
                    (constans character*)titulus.datum, (integer)linea);
            }
            alioquin
            {
                sprintf(nuntius, "horreum: tabella: linea %d non est "
                    "'clavis valor'", (integer)linea);
            }
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
            memset(capsa, ZEPHYRUM, magnitudo(capsa));
            redde NIHIL;
        }
        si (valor.mensura == ZEPHYRUM || valores[k].mensura > ZEPHYRUM)
        {
            sprintf(nuntius, "horreum: tabella: '%s' %s (linea %d)",
                claves_tabellae[k], valor.mensura == ZEPHYRUM
                    ? "sine valore" : "duplicata", (integer)linea);
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
            memset(capsa, ZEPHYRUM, magnitudo(capsa));
            redde NIHIL;
        }
        valores[k] = valor;
    }
    per (k = ZEPHYRUM; k < VII; k++)
    {
        si (   valores[k].mensura == ZEPHYRUM
            && (k == ZEPHYRUM || k == V || k == VI))
        {
            sprintf(nuntius, "horreum: tabella: '%s' deest",
                claves_tabellae[k]);
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM, nuntius, p);
            memset(capsa, ZEPHYRUM, magnitudo(capsa));
            redde NIHIL;
        }
    }
    si (!_in_capsam(valores[ZEPHYRUM], provisor,
        (i32)magnitudo(provisor)))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: tabella: provisor nimis longus", p);
        memset(capsa, ZEPHYRUM, magnitudo(capsa));
        redde NIHIL;
    }
    memset(&c, ZEPHYRUM, magnitudo(c));
    c.provisor     = provisor;
    c.terminus     = valores[I];
    c.regio        = valores[II];
    c.rationarium  = valores[III];
    c.situla       = valores[IV];
    c.clavis_id    = valores[V];
    c.secretum     = valores[VI];
    h              = horreum_creare(&c, e, p);
    memset(&c, ZEPHYRUM, magnitudo(c));
    memset(capsa, ZEPHYRUM, magnitudo(capsa));
    redde h;
}

/* ===== opera ===== */
interior b32
_capax (
     constans Horreum* h,
                  i32  capacitas,
   constans character* quid,
         HorreumError* e,
              Piscina* p)
{
    character nuntius[CXXVIII];

    si ((h->capacitates & capacitas) != ZEPHYRUM)
    {
        redde VERUM;
    }
    sprintf(nuntius, "horreum: provisor %s caret '%s' - nihil missum",
        h->provisor->titulus, quid);
    _errorem_ponere(e, HORREUM_INCAPAX, ZEPHYRUM, nuntius, p);
    redde FALSUM;
}

/* titulus situlae S3 (III..LXIII, [a-z0-9.-], margines alphanumerici,
 * sine ".."); punctum sub stilo hospitis (TLS) non est titulus DNS */
interior b32
_situla_valida (
    constans Horreum* h,
              chorda  s,
        HorreumError* e,
             Piscina* p)
{
    i32 i;
    b32 punctum = FALSUM;

    si (s.mensura < III || s.mensura > LXIII)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: situla III..LXIII characteres", p);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (c == '.')
        {
            punctum = VERUM;
            si (i > ZEPHYRUM && s.datum[i - I] == '.')
            {
                _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                    "horreum: situla '..' continet", p);
                redde FALSUM;
            }
            perge;
        }
        si (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
            || c == '-'))
        {
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: situla solum [a-z0-9.-]", p);
            redde FALSUM;
        }
    }
    si (   s.datum[ZEPHYRUM]      == '-' || s.datum[ZEPHYRUM] == '.'
        || s.datum[s.mensura - I] == '-'
        || s.datum[s.mensura - I] == '.')
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: situla incipit aut desinit non alphanumerice", p);
        redde FALSUM;
    }
    si (punctum && h->stilus == HORREUM_STILUS_HOSPES)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: situla cum puncto non est titulus DNS sub TLS "
            "(stilus hospitis)", p);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_opus_initium (
        HorreumOpus* o,
   HorreumOpusGenus  genus,
       HttpMethodus  methodus,
             chorda  situla,
             chorda  clavis,
            Piscina* p)
{
    memset(o, ZEPHYRUM, magnitudo(*o));
    o->genus       = genus;
    o->methodus    = methodus;
    o->situla      = situla;
    o->clavis      = clavis;
    o->idempotens  = VERUM;
    o->quaestio   = (SignaturaPar*)piscina_allocare(p,
        (memoriae_index)(HORREUM_QUAESTIO_OPERIS
            * (i32)magnitudo(SignaturaPar)));
    o->capita     = (SignaturaPar*)piscina_allocare(p,
        (memoriae_index)(HORREUM_CAPITA_OPERIS
            * (i32)magnitudo(SignaturaPar)));
    o->corpus_sha256_hex = _l(sigillum_vacui_horrei, p);
}

interior vacuum
_caput (
        HorreumOpus* o,
 constans character* titulus,
             chorda  valor,
            Piscina* p)
{
    si (o->capita_numerus >= HORREUM_CAPITA_OPERIS)
    {
        redde;
    }
    o->capita[o->capita_numerus].titulus  = _l(titulus, p);
    o->capita[o->capita_numerus].valor    = valor;
    o->capita_numerus++;
}

interior vacuum
_quaestio (
        HorreumOpus* o,
 constans character* titulus,
             chorda  valor,
            Piscina* p)
{
    si (o->quaestio_numerus >= HORREUM_QUAESTIO_OPERIS)
    {
        redde;
    }
    o->quaestio[o->quaestio_numerus].titulus  = _l(titulus, p);
    o->quaestio[o->quaestio_numerus].valor    = valor;
    o->quaestio_numerus++;
}

interior vacuum
_corpus (
    HorreumOpus* o,
         chorda  corpus,
        Piscina* p)
{
    o->corpus             = corpus;
    o->corpus_sha256_hex  = _sha256_hex(corpus, p);
}

/* optiones ponendi validae pro hoc provisore? (ante omnem
 * scripturam) */
interior b32
_ponendi_validae (
           constans Horreum* h,
    constans HorreumPonendi* ponendi,
                        b32  condiciones_licent,
               HorreumError* e,
                    Piscina* p)
{
    i32 i;

    si (!ponendi)
    {
        redde VERUM;
    }
    si (ponendi->meta_numerus > HORREUM_META_MAXIMA)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: meta plura quam HORREUM_META_MAXIMA (XVI)", p);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < ponendi->meta_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(ponendi->meta[i].titulus,
                _l(HORREUM_META_SIGILLUM, p)))
        {
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: meta titulus " HORREUM_META_SIGILLUM
                " horrei est (P10)", p);
            redde FALSUM;
        }
    }
    si (   ponendi->acl != HORREUM_ACL_NULLA
        && !_capax(h, HORREUM_CAP_ACL, "acl", e, p))
    {
        redde FALSUM;
    }
    si (   !condiciones_licent && (ponendi->solum_si_abest
            || ponendi->solum_si_etag.mensura > ZEPHYRUM
            || ponendi->checksum != HORREUM_CHECKSUM_NULLUM))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: condiciones et checksum solum in ponere", p);
        redde FALSUM;
    }
    si (ponendi->solum_si_abest)
    {
        si (!_capax(h, HORREUM_CAP_SOLUM_SI_ABEST, "solum_si_abest", e,
            p))
        {
            redde FALSUM;
        }
        si (   (h->probatae & HORREUM_CAP_SOLUM_SI_ABEST) == ZEPHYRUM
            && !ponendi->periculum_accipere)
        {
            _errorem_ponere(e, HORREUM_INCAPAX, ZEPHYRUM,
                "horreum: solum_si_abest nondum probatum vivum pro hoc "
                "provisore (horreum probare; aut periculum_accipere)",
                p);
            redde FALSUM;
        }
    }
    si (   ponendi->solum_si_etag.mensura > ZEPHYRUM
        && !_capax(h, HORREUM_CAP_SOLUM_SI_ETAG, "solum_si_etag", e, p))
    {
        redde FALSUM;
    }
    si (   ponendi->checksum == HORREUM_CHECKSUM_CRC32
        && !_capax(h, HORREUM_CAP_CHECKSUM_CRC32, "checksum crc32", e,
        p))
    {
        redde FALSUM;
    }
    si (   ponendi->checksum == HORREUM_CHECKSUM_SHA256
        && !_capax(h, HORREUM_CAP_CHECKSUM_SHA256, "checksum sha256", e,
        p))
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* capita ponendi (genus, cache, meta, acl) */
interior vacuum
_ponendi_capita (
                HorreumOpus* o,
    constans HorreumPonendi* ponendi,
                    Piscina* p)
{
    i32 i;

    si (!ponendi)
    {
        redde;
    }
    si (ponendi->genus_contenti.mensura > ZEPHYRUM)
    {
        _caput(o, "Content-Type", ponendi->genus_contenti, p);
    }
    si (ponendi->cache_control.mensura > ZEPHYRUM)
    {
        _caput(o, "Cache-Control", ponendi->cache_control, p);
    }
    per (i = ZEPHYRUM; i < ponendi->meta_numerus; i++)
    {
        ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);

        chorda_aedificator_appendere_literis(a, "x-amz-meta-");
        chorda_aedificator_appendere_chorda(a,
            chorda_minuscula(ponendi->meta[i].titulus, p));
        /* meta <= HORREUM_META_MAXIMA (_ponendi_validae): capita
         * fixa (VII) + XVI < HORREUM_CAPITA_OPERIS - nihil perit */
        si (o->capita_numerus < HORREUM_CAPITA_OPERIS)
        {
            o->capita[o->capita_numerus].titulus =
                chorda_aedificator_finire(a);
            o->capita[o->capita_numerus].valor = ponendi->meta[i].valor;
            o->capita_numerus++;
        }
    }
    si (ponendi->acl == HORREUM_ACL_PRIVATA)
    {
        _caput(o, "x-amz-acl", _l("private", p), p);
    }
    alioquin si (ponendi->acl == HORREUM_ACL_PUBLICA_LECTIO)
    {
        _caput(o, "x-amz-acl", _l("public-read", p), p);
    }
}

b32
horreum_opus_ponere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
                     chorda  corpus,
    constans HorreumPonendi* ponendi,
                HorreumOpus* o,
               HorreumError* e,
                    Piscina* p)
{
    si (   !h || !o || !_situla_valida(h, situla, e, p)
        || !_ponendi_validae(h, ponendi, VERUM, e, p))
    {
        redde FALSUM;
    }
    si (clavis.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis vacua",
            p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_PONERE, HTTP_PUT, situla, clavis, p);
    _corpus(o, corpus, p);
    _ponendi_capita(o, ponendi, p);
    si (ponendi && ponendi->solum_si_abest)
    {
        _caput(o, "If-None-Match", _l("*", p), p);
        o->condicionale = VERUM;
    }
    si (ponendi && ponendi->solum_si_etag.mensura > ZEPHYRUM)
    {
        _caput(o, "If-Match", _cum_virgulis(ponendi->solum_si_etag, p),
            p);
        o->condicionale = VERUM;
    }
    si (o->condicionale)
    {
        /* P10: nostrum post iterationem cognoscitur */
        _caput(o, "x-amz-meta-" HORREUM_META_SIGILLUM,
            o->corpus_sha256_hex, p);
    }
    si (ponendi && ponendi->checksum == HORREUM_CHECKSUM_CRC32)
    {
        i32 crc = crc32_calculare(corpus.datum, corpus.mensura);
         i8 be[IV];

        be[ZEPHYRUM]  = (i8)((crc >> XXIV) & 0xFF);
        be[I]         = (i8)((crc >> XVI) & 0xFF);
        be[II]        = (i8)((crc >> VIII) & 0xFF);
        be[III]       = (i8)(crc & 0xFF);
        _caput(o, "x-amz-checksum-crc32", base64_codificare(be, IV, p),
            p);
    }
    alioquin si (   ponendi
                 && ponendi->checksum == HORREUM_CHECKSUM_SHA256)
    {
        Sigillum s = sigillum_computare(corpus.datum,
                         (memoriae_index)corpus.mensura);

        _caput(o, "x-amz-checksum-sha256",
            base64_codificare(s.octeti, SIGILLUM_OCTETI, p), p);
    }
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

b32
horreum_opus_capere (
                     Horreum* h,
                      chorda  situla,
                      chorda  clavis,
    constans HorreumCapiendi* ca,
                 HorreumOpus* o,
                HorreumError* e,
                     Piscina* p)
{
    si (!h || !o || !_situla_valida(h, situla, e, p))
    {
        redde FALSUM;
    }
    si (   clavis.mensura == ZEPHYRUM
        || (ca && ca->frustum && ca->ad < ca->ab))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis vacua aut frustum malum", p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_CAPERE, HTTP_GET, situla, clavis, p);
    si (ca && ca->frustum)
    {
        character capsa[LXIV];

        sprintf(capsa, "bytes=%lld-%lld", (long long)ca->ab,
            (long long)ca->ad);
        _caput(o, "Range", _l(capsa, p), p);
    }
    si (ca && ca->si_etag.mensura > ZEPHYRUM)
    {
        _caput(o, "If-Match", _cum_virgulis(ca->si_etag, p), p);
    }
    si (ca && ca->nisi_etag.mensura > ZEPHYRUM)
    {
        _caput(o, "If-None-Match", _cum_virgulis(ca->nisi_etag, p), p);
    }
    si (ca && ca->si_mutatum_post > ZEPHYRUM)
    {
        _caput(o, "If-Modified-Since",
            _tempus_http_scribere(ca->si_mutatum_post, p), p);
    }
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

interior b32
_opus_simplex (
            Horreum* h,
   HorreumOpusGenus  genus,
       HttpMethodus  methodus,
             chorda  situla,
             chorda  clavis,
        HorreumOpus* o,
       HorreumError* e,
            Piscina* p)
{
    si (!h || !o || !_situla_valida(h, situla, e, p))
    {
        redde FALSUM;
    }
    si (clavis.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis vacua",
            p);
        redde FALSUM;
    }
    _opus_initium(o, genus, methodus, situla, clavis, p);
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

b32
horreum_opus_inspicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    redde _opus_simplex(h, HORREUM_OPUS_INSPICERE, HTTP_HEAD, situla,
        clavis,
        o, e, p);
}

b32
horreum_opus_delere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    redde _opus_simplex(h, HORREUM_OPUS_DELERE, HTTP_DELETE, situla,
        clavis,
        o, e, p);
}

b32
horreum_opus_delere_plura (
            Horreum* h,
             chorda  situla,
    constans chorda* claves,
                i32  numerus,
        HorreumOpus* o,
       HorreumError* e,
            Piscina* p)
{
     ChordaAedificator* a;
                   i32  i;
                    i8  md5[MD5_DIGEST_MENSURA];

    si (   !h || !o || !_situla_valida(h, situla, e, p)
        || !_capax(h, HORREUM_CAP_DELERE_PLURA, "delere_plura", e, p))
    {
        redde FALSUM;
    }
    si (!claves || numerus < I || numerus > HORREUM_CLAVES_PLURIMAE)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: delere_plura 1..1000 claves", p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_DELERE_PLURA, HTTP_POST, situla,
        chorda_ex_buffer(NIHIL, ZEPHYRUM), p);
    a = chorda_aedificator_creare(p, (memoriae_index)(numerus * XLVIII
        + LXIV));
    chorda_aedificator_appendere_literis(a,
        "<Delete><Quiet>false</Quiet>");
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(a, "<Object><Key>");
        _xml_effugere(a, claves[i]);
        chorda_aedificator_appendere_literis(a, "</Key></Object>");
    }
    chorda_aedificator_appendere_literis(a, "</Delete>");
    _corpus(o, chorda_aedificator_finire(a), p);
    md5_friare(o->corpus.datum, o->corpus.mensura, md5);
    _caput(o, "Content-MD5", base64_codificare(md5, MD5_DIGEST_MENSURA,
        p), p);
    _caput(o, "Content-Type", _l("application/xml", p), p);
    _quaestio(o, "delete", _l("", p), p);
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

b32
horreum_opus_copiare (
                    Horreum* h,
                     chorda  situla_fons,
                     chorda  clavis_fons,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* ponendi,
                HorreumOpus* o,
               HorreumError* e,
                    Piscina* p)
{
    ChordaAedificator* a;

    si (   !h || !o || !_situla_valida(h, situla_fons, e, p)
        || !_situla_valida(h, situla, e, p)
        || !_ponendi_validae(h, ponendi, FALSUM, e, p))
    {
        redde FALSUM;
    }
    si (clavis.mensura == ZEPHYRUM || clavis_fons.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis vacua",
            p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_COPIARE, HTTP_PUT, situla, clavis, p);
    a = chorda_aedificator_creare(p, CXXVIII);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a, situla_fons);
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_chorda(a,
        signatura_aws_codificare(clavis_fons, VERUM, p));
    _caput(o, "x-amz-copy-source", chorda_aedificator_finire(a), p);
    _caput(o, "x-amz-metadata-directive",
        _l(ponendi ? "REPLACE" : "COPY",
        p), p);
    _ponendi_capita(o, ponendi, p);
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

b32
horreum_opus_enumerare (
                       Horreum* h,
                        chorda  situla,
    constans HorreumEnumerandi* en,
                   HorreumOpus* o,
                  HorreumError* e,
                       Piscina* p)
{
    i32 pagina = (en && en->magnitudo_paginae > ZEPHYRUM)
                     ? en->magnitudo_paginae : M;

    si (!h || !o || !_situla_valida(h, situla, e, p))
    {
        redde FALSUM;
    }
    si (pagina > M)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: magnitudo_paginae 1..1000", p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_ENUMERARE, HTTP_GET, situla,
        chorda_ex_buffer(NIHIL, ZEPHYRUM), p);
    _quaestio(o, "list-type", _l("2", p), p);
    _quaestio(o, "max-keys", _numerus_chorda((s64)pagina, p), p);
    si (en && en->praefixum.mensura > ZEPHYRUM)
    {
        _quaestio(o, "prefix", en->praefixum, p);
    }
    si (en && en->delimitator.mensura > ZEPHYRUM)
    {
        _quaestio(o, "delimiter", en->delimitator, p);
    }
    si (en && en->signum_continuationis.mensura > ZEPHYRUM)
    {
        _quaestio(o, "continuation-token", en->signum_continuationis,
            p);
    }
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

b32
horreum_opus_mp_incipere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* ponendi,
                HorreumOpus* o,
               HorreumError* e,
                    Piscina* p)
{
    si (   !h || !o || !_situla_valida(h, situla, e, p)
        || !_ponendi_validae(h, ponendi, FALSUM, e, p))
    {
        redde FALSUM;
    }
    si (clavis.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: clavis vacua",
            p);
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_MP_INCIPERE, HTTP_POST, situla,
        clavis, p);
    _quaestio(o, "uploads", _l("", p), p);
    _ponendi_capita(o, ponendi, p);
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

interior b32
_onus_validum (
           chorda  id,
     HorreumError* e,
          Piscina* p)
{
    si (id.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: id_oneris vacuum", p);
        redde FALSUM;
    }
    redde VERUM;
}

b32
horreum_opus_mp_pars (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id,
             i32  numerus,
          chorda  corpus,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    si (   !_opus_simplex(h, HORREUM_OPUS_MP_PARS, HTTP_PUT, situla,
        clavis, o,
            e, p) || !_onus_validum(id, e, p))
    {
        redde FALSUM;
    }
    si (numerus < I || numerus > HORREUM_PARTES_MAXIMAE)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: numerus partis 1..10000", p);
        redde FALSUM;
    }
    _quaestio(o, "partNumber", _numerus_chorda((s64)numerus, p), p);
    _quaestio(o, "uploadId", id, p);
    _corpus(o, corpus, p);
    redde VERUM;
}

/* ETag multipartis: md5(md5_1 || ... || md5_n) hex + "-n", si omnes
 * partes ETag md5 (XXXII hex) habent; aliter vacua */
interior chorda
_etag_partium (
    constans HorreumPars* partes,
                     i32  numerus,
                 Piscina* p)
{
         MD5Contextus  ctx;
                   i8  digest[MD5_DIGEST_MENSURA];
                  i32  k;
            character  suffixum[XVI];
    ChordaAedificator* a;

    md5_initiare(&ctx);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        chorda t = _sine_virgulis(partes[k].etag);
            i8 bin[MD5_DIGEST_MENSURA];
           i32 j;

        si (t.mensura != XXXII)
        {
            redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
        }
        per (j = ZEPHYRUM; j < XXXII; j++)
        {
            si (!_cifra_hex(t.datum[j]))
            {
                redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
            }
        }
        per (j = ZEPHYRUM; j < MD5_DIGEST_MENSURA; j++)
        {
            character par[III];
              integer v;

            par[ZEPHYRUM]  = (character)t.datum[j * II];
            par[I]         = (character)t.datum[j * II + I];
            par[II]        = '\0';
            sscanf(par, "%x", (insignatus integer*)&v);
            bin[j] = (i8)v;
        }
        md5_addere(&ctx, bin, MD5_DIGEST_MENSURA);
    }
    md5_finire(&ctx, digest);
    a = chorda_aedificator_creare(p, XLVIII);
    chorda_aedificator_appendere_chorda(a, _hex(digest,
        MD5_DIGEST_MENSURA,
        p));
    sprintf(suffixum, "-%d", (integer)numerus);
    chorda_aedificator_appendere_literis(a, suffixum);
    redde chorda_aedificator_finire(a);
}

b32
horreum_opus_mp_perficere (
                Horreum* h,
                 chorda  situla,
                 chorda  clavis,
                 chorda  id,
    constans HorreumPars* partes,
                    i32  numerus,
            HorreumOpus* o,
           HorreumError* e,
                Piscina* p)
{
     ChordaAedificator* a;
                   i32  k;

    si (!_opus_simplex(h, HORREUM_OPUS_MP_PERFICERE, HTTP_POST, situla,
            clavis, o, e, p) || !_onus_validum(id, e, p))
    {
        redde FALSUM;
    }
    si (!partes || numerus < I || numerus > HORREUM_PARTES_MAXIMAE)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: perficere 1..10000 partes", p);
        redde FALSUM;
    }
    _quaestio(o, "uploadId", id, p);
    a = chorda_aedificator_creare(p, (memoriae_index)(numerus * LXXX
        + LXIV));
    chorda_aedificator_appendere_literis(a,
        "<CompleteMultipartUpload>");
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        character capsa[XXIV];

        sprintf(capsa, "%d", (integer)partes[k].numerus);
        chorda_aedificator_appendere_literis(a, "<Part><PartNumber>");
        chorda_aedificator_appendere_literis(a, capsa);
        chorda_aedificator_appendere_literis(a, "</PartNumber><ETag>");
        _xml_effugere(a, _cum_virgulis(partes[k].etag, p));
        chorda_aedificator_appendere_literis(a, "</ETag></Part>");
    }
    chorda_aedificator_appendere_literis(a,
        "</CompleteMultipartUpload>");
    _corpus(o, chorda_aedificator_finire(a), p);
    _caput(o, "Content-Type", _l("application/xml", p), p);
    o->perficere_etag_expectatum = _etag_partium(partes, numerus,
        p);
    redde VERUM;
}

b32
horreum_opus_mp_abicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    si (!_opus_simplex(h, HORREUM_OPUS_MP_ABICERE, HTTP_DELETE, situla,
            clavis, o, e, p) || !_onus_validum(id, e, p))
    {
        redde FALSUM;
    }
    _quaestio(o, "uploadId", id, p);
    redde VERUM;
}

b32
horreum_opus_mp_partes (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    si (   !_opus_simplex(h, HORREUM_OPUS_MP_PARTES, HTTP_GET, situla,
        clavis,
            o, e, p) || !_onus_validum(id, e, p))
    {
        redde FALSUM;
    }
    _quaestio(o, "uploadId", id, p);
    redde VERUM;
}

b32
horreum_opus_mp_pendentes (
         Horreum* h,
          chorda  situla,
          chorda  praefixum,
     HorreumOpus* o,
    HorreumError* e,
         Piscina* p)
{
    si (!h || !o || !_situla_valida(h, situla, e, p))
    {
        redde FALSUM;
    }
    _opus_initium(o, HORREUM_OPUS_MP_PENDENTES, HTTP_GET, situla,
        chorda_ex_buffer(NIHIL, ZEPHYRUM), p);
    _quaestio(o, "uploads", _l("", p), p);
    si (praefixum.mensura > ZEPHYRUM)
    {
        _quaestio(o, "prefix", praefixum, p);
    }
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde VERUM;
}

/* ===== signatio ===== */
/* hospes et via DECODIFICATA operis */
interior vacuum
_destinatio (
             Horreum* h,
              chorda  situla,
              chorda  clavis,
              chorda* hospes,
              chorda* via,
             Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, CCLVI);
    ChordaAedificator* v = chorda_aedificator_creare(p,
                               (memoriae_index)(clavis.mensura + LXXX));

    si (h->stilus == HORREUM_STILUS_HOSPES)
    {
        chorda_aedificator_appendere_chorda(a, situla);
        chorda_aedificator_appendere_character(a, '.');
        chorda_aedificator_appendere_literis(a, h->hospes);
        chorda_aedificator_appendere_character(v, '/');
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, h->hospes);
        chorda_aedificator_appendere_character(v, '/');
        chorda_aedificator_appendere_chorda(v, situla);
        chorda_aedificator_appendere_character(v, '/');
    }
    chorda_aedificator_appendere_chorda(v, clavis);
    *hospes  = chorda_aedificator_finire(a);
    *via     = chorda_aedificator_finire(v);
}

interior SignaturaPetitio
_signatura_initium (
             Horreum* h,
        HttpMethodus  methodus,
              chorda  hospes,
              chorda  via,
         MomentumSec  tempus,
             Piscina* p)
{
    SignaturaPetitio sp;

    memset(&sp, ZEPHYRUM, magnitudo(sp));
    sp.methodus  = http_methodus_nomen(methodus);
    sp.hospes    = hospes;
    sp.via       = via;
    sp.tempus    = tempus;
    sp.regio     = chorda_ex_buffer((i8*)h->regio,
                       (i32)strlen(h->regio));
    sp.servitium = _l("s3", p);
    sp.clavis_id = chorda_ex_buffer((i8*)h->clavis_id,
                       (i32)strlen(h->clavis_id));
    sp.secretum  = chorda_ex_buffer((i8*)h->secretum,
                       (i32)strlen(h->secretum));
    redde sp;
}

/* linea III petitionis canonicae = quaestio canonica */
interior chorda
_quaestio_canonica (
    chorda canonica)
{
    i32 i;
    i32 linea    = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < canonica.mensura; i++)
    {
        si (canonica.datum[i] != '\n')
        {
            perge;
        }
        linea++;
        si (linea == II)
        {
            initium = i + I;
        }
        si (linea == III)
        {
            redde chorda_sectio(canonica, initium, i);
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

HttpPetitio*
horreum_petitionem_signare (
                 Horreum* h,
    constans HorreumOpus* o,
             MomentumSec  tempus,
            HorreumError* e,
                 Piscina* p)
{
      SignaturaPetitio  sp;
     SignaturaResultus  r;
                chorda  hospes;
                chorda  via;
                chorda  quaestio;
     ChordaAedificator* u;
           HttpPetitio* pet;
                   i32  k;

    si (!h || !o)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: signare sine horreo aut opere", p);
        redde NIHIL;
    }
    _destinatio(h, o->situla, o->clavis, &hospes, &via, p);
    sp = _signatura_initium(h, o->methodus, hospes, via, tempus, p);
    sp.quaestio = o->quaestio;
    sp.quaestio_numerus = o->quaestio_numerus;
    sp.capita = o->capita;
    sp.capita_numerus = o->capita_numerus;
    sp.corpus_sigillum = o->corpus_sha256_hex;
    sp.corpus_capite = VERUM;
    r = signatura_aws_capitibus(&sp, p);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    si (!r.successus)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            chorda_ut_cstr(r.error, p), p);
        redde NIHIL;
    }
    quaestio = _quaestio_canonica(r.petitio_canonica);

    u = chorda_aedificator_creare(p, DXII);
    chorda_aedificator_appendere_literis(u, h->schema);
    chorda_aedificator_appendere_literis(u, "://");
    chorda_aedificator_appendere_chorda(u, hospes);
    chorda_aedificator_appendere_chorda(u,
        signatura_aws_codificare(via, VERUM, p));
    si (quaestio.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(u, '?');
        chorda_aedificator_appendere_chorda(u, quaestio);
    }
    pet = http_petitio_creare(p, o->methodus,
              chorda_ut_cstr(chorda_aedificator_finire(u), p));
    si (!pet)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: URL petitionis mala", p);
        redde NIHIL;
    }
    http_petitio_caput_addere(pet, "Host", chorda_ut_cstr(hospes, p));
    per (k = ZEPHYRUM; k < o->capita_numerus; k++)
    {
        http_petitio_caput_addere(pet,
            chorda_ut_cstr(o->capita[k].titulus, p),
            chorda_ut_cstr(o->capita[k].valor, p));
    }
    per (k = ZEPHYRUM; k < r.capita_addita_numerus; k++)
    {
        http_petitio_caput_addere(pet,
            chorda_ut_cstr(r.capita_addita[k].titulus, p),
            chorda_ut_cstr(r.capita_addita[k].valor, p));
    }
    http_petitio_caput_addere(pet, "Authorization",
        chorda_ut_cstr(r.auctoritas, p));
    si (o->corpus.mensura > ZEPHYRUM)
    {
        http_petitio_corpus_ponere_chorda(pet, o->corpus);
    }
    alioquin si (o->methodus == HTTP_PUT || o->methodus == HTTP_POST)
    {
        /* P3: S3 sine Content-Length 411 reddit */
        http_petitio_caput_addere(pet, "Content-Length", "0");
    }
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde pet;
}

chorda
horreum_praesignare (
         Horreum* h,
    HttpMethodus  methodus,
          chorda  situla,
          chorda  clavis,
             i32  secundae,
          chorda  genus_contenti,
     MomentumSec  tempus,
    HorreumError* e,
         Piscina* p)
{
     SignaturaPetitio  sp;
    SignaturaResultus  r;
         SignaturaPar  caput;
               chorda  hospes;
               chorda  via;
    ChordaAedificator* u;
               chorda  vacua = chorda_ex_buffer(NIHIL, ZEPHYRUM);

    si (!h || (methodus != HTTP_GET && methodus != HTTP_PUT))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: praesignare solum GET aut PUT", p);
        redde vacua;
    }
    si (!_situla_valida(h, situla, e, p))
    {
        redde vacua;
    }
    si (   clavis.mensura == ZEPHYRUM || secundae < I
        || secundae > SIGNATURA_AWS_DURATIO_MAXIMA)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: praesignare: clavis vacua aut secundae extra "
            "1..604800", p);
        redde vacua;
    }
    _destinatio(h, situla, clavis, &hospes, &via, p);
    sp = _signatura_initium(h, methodus, hospes, via, tempus, p);
    si (genus_contenti.mensura > ZEPHYRUM)
    {
        caput.titulus      = _l("content-type", p);
        caput.valor        = genus_contenti;
        sp.capita          = &caput;
        sp.capita_numerus  = I;
    }
    sp.corpus_sigillum  = _l("UNSIGNED-PAYLOAD", p);
    sp.duratio          = secundae;
    r                   = signatura_aws_quaestione(&sp, p);
    memset(&sp, ZEPHYRUM, magnitudo(sp));
    si (!r.successus)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            chorda_ut_cstr(r.error, p), p);
        redde vacua;
    }
    u = chorda_aedificator_creare(p, DXII);
    chorda_aedificator_appendere_literis(u, h->schema);
    chorda_aedificator_appendere_literis(u, "://");
    chorda_aedificator_appendere_chorda(u, hospes);
    chorda_aedificator_appendere_chorda(u,
        signatura_aws_codificare(via, VERUM, p));
    chorda_aedificator_appendere_character(u, '?');
    chorda_aedificator_appendere_chorda(u, r.quaestio_signata);
    si (e)
    {
        memset(e, ZEPHYRUM, magnitudo(*e));
    }
    redde chorda_aedificator_finire(u);
}

/* ===== lectio responsorum ===== */
interior chorda
_textus (
              XmlNodus* parens,
    constans character* titulus,
               Piscina* p)
{
    XmlNodus* n = parens ? xml_invenire_liberum(parens,
        titulus) : NIHIL;

    si (!n)
    {
        redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    /* xml.h entitates et referentias numericas SEMEL decodificat
     * (P15; iterum decodificare "&amp;#65;" in "A" verteret - recensio
     * I3) */
    redde xml_textus_internus(n, p);
}

interior XmlNodus*
_radix (
                chorda  corpus,
    constans character* titulus,
          HorreumError* e,
               Piscina* p)
{
    XmlResultus r;

    si (corpus.mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: XML vacuum", p);
        redde NIHIL;
    }
    r = xml_legere(corpus, p, internamentum_creare(p));
    si (!r.successus || !r.radix)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: XML malum", p);
        redde NIHIL;
    }
    si (   titulus && (!r.radix->titulus
            || !chorda_aequalis_literis(*r.radix->titulus, titulus)))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: XML radix inexpectata", p);
        redde NIHIL;
    }
    redde r.radix;
}

/* tabula erroris (II.5) */
interior HorreumErrorGenus
_genus_erroris (
       i32 status,
    chorda codex)
{
    si (   chorda_aequalis_literis(codex, "RequestTimeTooSkewed")
        || chorda_aequalis_literis(codex, "SlowDown")
        || chorda_aequalis_literis(codex, "InternalError")
        || chorda_aequalis_literis(codex, "ServiceUnavailable"))
    {
        redde HORREUM_TRANSITORIUM;
    }
    si (   chorda_aequalis_literis(codex, "NoSuchKey")
        || chorda_aequalis_literis(codex, "NoSuchBucket")
        || chorda_aequalis_literis(codex, "NoSuchUpload"))
    {
        redde HORREUM_ABEST;
    }
    si (   chorda_aequalis_literis(codex, "AccessDenied")
        || chorda_aequalis_literis(codex, "SignatureDoesNotMatch")
        || chorda_aequalis_literis(codex, "InvalidAccessKeyId"))
    {
        redde HORREUM_VETITUM;
    }
    si (chorda_aequalis_literis(codex, "PreconditionFailed"))
    {
        redde HORREUM_CONDITIO;
    }
    si (status == CDIV)
    {
        redde HORREUM_ABEST;
    }
    si (status == CDXII || status == CCCIV)
    {
        redde HORREUM_CONDITIO;
    }
    si (status == CDIII)
    {
        redde HORREUM_VETITUM;
    }
    si (status >= D && status <= DXCIX)
    {
        redde HORREUM_TRANSITORIUM;
    }
    redde HORREUM_MALUM;
}

interior chorda
_caput_responsi (
    constans HttpResponsum* r,
        constans character* titulus,
                   Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; i < r->capita_numerus; i++)
    {
        si (chorda_aequalis_case_insensitivus(r->capita[i].titulus,
                _l(titulus, p)))
        {
            redde r->capita[i].valor;
        }
    }
    redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
}

interior vacuum
_obiectum_ex_capitibus (
      constans HorreumOpus* o,
    constans HttpResponsum* r,
           HorreumObjectum* ob,
                   Piscina* p)
{
    i32 i;
    i32 n = ZEPHYRUM;
    s64 m;

    memset(ob, ZEPHYRUM, magnitudo(*ob));
    ob->clavis  = o->clavis;
    ob->etag    = _sine_virgulis(_caput_responsi(r, "ETag", p));
    m           = _numerus(_caput_responsi(r, "Content-Length", p));
    si (o->genus == HORREUM_OPUS_PONERE)
    {
        ob->mensura = (i64)o->corpus.mensura;
    }
    alioquin si (m >= ZEPHYRUM)
    {
        ob->mensura = (i64)m;
    }
    ob->mutatum         = horreum_tempus_http(_caput_responsi(r,
                              "Last-Modified", p));
    ob->genus_contenti  = _caput_responsi(r, "Content-Type", p);
    ob->versio          = _caput_responsi(r, "x-amz-version-id", p);
    ob->checksum_crc32  = _caput_responsi(r, "x-amz-checksum-crc32", p);
    ob->checksum_sha256 = _caput_responsi(r, "x-amz-checksum-sha256",
        p);
    per (i = ZEPHYRUM; i < r->capita_numerus; i++)
    {
        si (chorda_incipit(chorda_minuscula(r->capita[i].titulus, p),
                _l("x-amz-meta-", p)))
        {
            n++;
        }
    }
    si (n > ZEPHYRUM)
    {
        ob->meta = (HorreumMeta*)piscina_allocare(p,
            (memoriae_index)(n * (i32)magnitudo(HorreumMeta)));
        per (i = ZEPHYRUM; i < r->capita_numerus; i++)
        {
            chorda t = chorda_minuscula(r->capita[i].titulus, p);

            si (chorda_incipit(t, _l("x-amz-meta-", p)))
            {
                ob->meta[ob->meta_numerus].titulus = chorda_sectio(t,
                    XI,
                    t.mensura);
                ob->meta[ob->meta_numerus].valor = r->capita[i].valor;
                ob->meta_numerus++;
            }
        }
    }
}

/* CopyObject et CompleteMultipartUpload: 200 cum <Error> (S3 errorem
 * post capita missa nuntiat) aut <CopyObjectResult> (ETag,
 * LastModified) / <CompleteMultipartUploadResult> (ETag) */
interior b32
_corpus_successus_legere (
    constans HttpResponsum* r,
           HorreumObjectum* ob,
              HorreumError* e,
                   Piscina* p)
{
    HorreumError  ignotum;
        XmlNodus* radix;

    si (r->corpus.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    radix = _radix(r->corpus, NIHIL, &ignotum, p);
    si (!radix || !radix->titulus)
    {
        redde VERUM;
    }
    si (chorda_aequalis_literis(*radix->titulus, "Error"))
    {
        si (ob)
        {
            memset(ob, ZEPHYRUM, magnitudo(*ob));
        }
        e->status      = r->status;
        e->codex       = _textus(radix, "Code", p);
        e->nuntius     = _textus(radix, "Message", p);
        e->petitio_id  = _caput_responsi(r, "x-amz-request-id", p);
        e->genus       = _genus_erroris(r->status, e->codex);
        si (e->nuntius.mensura == ZEPHYRUM)
        {
            e->nuntius = _l("horreum: 200 cum <Error>", p);
        }
        redde FALSUM;
    }
    si (   ob && chorda_aequalis_literis(*radix->titulus,
            "CompleteMultipartUploadResult"))
    {
        ob->etag = _sine_virgulis(_textus(radix, "ETag", p));
    }
    si (   ob && chorda_aequalis_literis(*radix->titulus,
            "CopyObjectResult"))
    {
        ob->etag     = _sine_virgulis(_textus(radix, "ETag", p));
        ob->mutatum  = horreum_tempus_iso(_textus(radix, "LastModified",
            p));
    }
    redde VERUM;
}

b32
horreum_responsum_legere (
    constans HorreumOpus* o,
            HttpResultus  res,
             MomentumSec  tempus_signandi,
         HorreumObjectum* ob,
            HorreumError* e,
                 Piscina* p)
{
     HttpResponsum* r = res.responsum;
          XmlNodus* radix;
      HorreumError  loc;

    si (!e)
    {
        e = &loc;
    }
    memset(e, ZEPHYRUM, magnitudo(*e));
    si (ob)
    {
        memset(ob, ZEPHYRUM, magnitudo(*ob));
    }
    si (!o)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: opus NIHIL", p);
        redde FALSUM;
    }
    si (!res.successus || !r)
    {
        si (chorda_continet(res.error_descriptio, _l("corpus nimium",
            p)))
        {
            _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
                "horreum: responsum maius quam XVI MB - "
                "horreum_capere_in_filum adhibe (P1)", p);
        }
        alioquin
        {
            _errorem_ponere(e, HORREUM_TRANSITORIUM, ZEPHYRUM,
                res.error_descriptio.mensura > ZEPHYRUM
                    ? chorda_ut_cstr(res.error_descriptio, p)
                    : "horreum: rete defecit", p);
        }
        redde FALSUM;
    }
    si (r->status >= CC && r->status < CCC)
    {
        si (ob)
        {
            _obiectum_ex_capitibus(o, r, ob, p);
        }
        redde (   o->genus != HORREUM_OPUS_COPIARE
               && o->genus != HORREUM_OPUS_MP_PERFICERE)
            || _corpus_successus_legere(r, ob, e, p);
    }
    e->status      = r->status;
    e->petitio_id  = _caput_responsi(r, "x-amz-request-id", p);
    si (r->corpus.mensura > ZEPHYRUM)
    {
        HorreumError ignotum;

        radix = _radix(r->corpus, "Error", &ignotum, p);
        si (radix)
        {
            e->codex    = _textus(radix, "Code", p);
            e->nuntius  = _textus(radix, "Message", p);
            si (e->petitio_id.mensura == ZEPHYRUM)
            {
                e->petitio_id = _textus(radix, "RequestId", p);
            }
        }
    }
    si (e->petitio_id.mensura == ZEPHYRUM)
    {
        e->petitio_id = _caput_responsi(r, "cf-ray", p);
    }
    e->genus = _genus_erroris(r->status, e->codex);
    si (chorda_aequalis_literis(e->codex, "RequestTimeTooSkewed"))
    {
        MomentumSec servus = horreum_tempus_http(_caput_responsi(r,
            "Date",
                                 p));

        si (servus > ZEPHYRUM && tempus_signandi > ZEPHYRUM)
        {
            e->deviatio_s = (s64)servus - (s64)tempus_signandi;
        }
    }
    si (e->nuntius.mensura == ZEPHYRUM)
    {
        character capsa[LXIV];

        sprintf(capsa, "horreum: HTTP %d", (integer)r->status);
        e->nuntius = _l(capsa, p);
    }
    redde FALSUM;
}

b32
horreum_paginam_legere (
           chorda  corpus,
    HorreumPagina* pg,
     HorreumError* e,
          Piscina* p)
{
     XmlNodus* radix;
          Xar* contenta;
          Xar* praefixa;
          i32  i;

    memset(pg, ZEPHYRUM, magnitudo(*pg));
    radix = _radix(corpus, "ListBucketResult", e, p);
    si (!radix)
    {
        redde FALSUM;
    }
    contenta = xml_invenire_omnes_liberos(radix, "Contents", p);
    praefixa = xml_invenire_omnes_liberos(radix, "CommonPrefixes", p);
    pg->obiecta_numerus =
        contenta ? (i32)xar_numerus(contenta) : ZEPHYRUM;
    pg->praefixa_numerus =
        praefixa ? (i32)xar_numerus(praefixa) : ZEPHYRUM;
    pg->obiecta = (HorreumObjectum*)piscina_allocare(p,
        (memoriae_index)((pg->obiecta_numerus + I)
            * (i32)magnitudo(HorreumObjectum)));
    pg->praefixa_communia = (chorda*)piscina_allocare(p,
        (memoriae_index)((pg->praefixa_numerus
            + I) * (i32)magnitudo(chorda)));
    per (i = ZEPHYRUM; i < pg->obiecta_numerus; i++)
    {
               XmlNodus* n   =
                   *(XmlNodus**)xar_obtinere(contenta, i);
        HorreumObjectum* ob  = &pg->obiecta[i];
                    s64  m;

        memset(ob, ZEPHYRUM, magnitudo(*ob));
        ob->clavis = _textus(n, "Key", p);
        ob->etag = _sine_virgulis(_textus(n, "ETag", p));
        ob->mutatum = horreum_tempus_iso(_textus(n, "LastModified", p));
        m = _numerus(_textus(n, "Size", p));
        ob->mensura = m >= ZEPHYRUM ? (i64)m : ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < pg->praefixa_numerus; i++)
    {
        XmlNodus* n = *(XmlNodus**)xar_obtinere(praefixa, i);

        pg->praefixa_communia[i] = _textus(n, "Prefix", p);
    }
    pg->truncata = chorda_aequalis_literis(_textus(radix, "IsTruncated",
        p),
                       "true");
    pg->signum_proximum = _textus(radix, "NextContinuationToken", p);
    redde VERUM;
}

b32
horreum_deletiones_legere (
             chorda   corpus,
     HorreumDeletio** exitus,
                i32*  numerus,
       HorreumError*  e,
            Piscina*  p)
{
          XmlNodus* radix = _radix(corpus, "DeleteResult", e, p);
               i32  n;
               i32  i;
               i32  k = ZEPHYRUM;
    HorreumDeletio* t;

    *numerus = ZEPHYRUM;
    si (!radix)
    {
        redde FALSUM;
    }
    n = xml_numerus_liberorum(radix);
    t = (HorreumDeletio*)piscina_allocare(p,
            (memoriae_index)((n + I) * (i32)magnitudo(HorreumDeletio)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        XmlNodus* c = xml_liberum_ad_indicem(radix, i);

        si (!c || c->genus != XML_NODUS_ELEMENTUM || !c->titulus)
        {
            perge;
        }
        memset(&t[k], ZEPHYRUM, magnitudo(t[k]));
        si (chorda_aequalis_literis(*c->titulus, "Deleted"))
        {
            t[k].clavis   = _textus(c, "Key", p);
            t[k].deletum  = VERUM;
            k++;
        }
        alioquin si (chorda_aequalis_literis(*c->titulus, "Error"))
        {
            t[k].clavis         = _textus(c, "Key", p);
            t[k].error.codex    = _textus(c, "Code", p);
            t[k].error.nuntius  = _textus(c, "Message", p);
            t[k].error.genus = _genus_erroris(ZEPHYRUM,
                t[k].error.codex);
            k++;
        }
    }
    *exitus   = t;
    *numerus  = k;
    redde VERUM;
}

b32
horreum_id_oneris_legere (
          chorda  corpus,
          chorda* id,
    HorreumError* e,
         Piscina* p)
{
    XmlNodus* radix = _radix(corpus, "InitiateMultipartUploadResult", e,
        p);

    si (!radix)
    {
        redde FALSUM;
    }
    *id = _textus(radix, "UploadId", p);
    si (id->mensura == ZEPHYRUM)
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: UploadId deest", p);
        redde FALSUM;
    }
    redde VERUM;
}

b32
horreum_partes_legere (
          chorda   corpus,
     HorreumPars** exitus,
             i32*  numerus,
    HorreumError*  e,
         Piscina*  p)
{
       XmlNodus* radix = _radix(corpus, "ListPartsResult", e, p);
            Xar* partes;
            i32  i;
    HorreumPars* t;

    *numerus = ZEPHYRUM;
    si (!radix)
    {
        redde FALSUM;
    }
    partes    = xml_invenire_omnes_liberos(radix, "Part", p);
    *numerus  = partes ? (i32)xar_numerus(partes) : ZEPHYRUM;
    t = (HorreumPars*)piscina_allocare(p,
            (memoriae_index)((*numerus
                + I) * (i32)magnitudo(HorreumPars)));
    per (i = ZEPHYRUM; i < *numerus; i++)
    {
        XmlNodus* n = *(XmlNodus**)xar_obtinere(partes, i);
             s64  v;

        v             = _numerus(_textus(n, "PartNumber", p));
        t[i].numerus  = v > ZEPHYRUM ? (i32)v : ZEPHYRUM;
        t[i].etag     = _sine_virgulis(_textus(n, "ETag", p));
        v             = _numerus(_textus(n, "Size", p));
        t[i].mensura  = v >= ZEPHYRUM ? (i64)v : ZEPHYRUM;
    }
    *exitus = t;
    redde VERUM;
}

b32
horreum_pendentes_legere (
                 chorda   corpus,
     HorreumOnusPendens** exitus,
                    i32*  numerus,
           HorreumError*  e,
                Piscina*  p)
{
    XmlNodus* radix = _radix(corpus, "ListMultipartUploadsResult", e,
        p);
                   Xar* onera;
                   i32  i;
    HorreumOnusPendens* t;

    *numerus = ZEPHYRUM;
    si (!radix)
    {
        redde FALSUM;
    }
    onera     = xml_invenire_omnes_liberos(radix, "Upload", p);
    *numerus  = onera ? (i32)xar_numerus(onera) : ZEPHYRUM;
    t = (HorreumOnusPendens*)piscina_allocare(p,
            (memoriae_index)((*numerus + I)
                * (i32)magnitudo(HorreumOnusPendens)));
    per (i = ZEPHYRUM; i < *numerus; i++)
    {
        XmlNodus* n = *(XmlNodus**)xar_obtinere(onera, i);

        t[i].clavis = _textus(n, "Key", p);
        t[i].id_oneris = _textus(n, "UploadId", p);
        t[i].initium = horreum_tempus_iso(_textus(n, "Initiated", p));
    }
    *exitus = t;
    redde VERUM;
}

b32
horreum_etag_perfecti_legere (
          chorda  corpus,
          chorda* etag,
    HorreumError* e,
         Piscina* p)
{
    XmlNodus* radix = _radix(corpus, NIHIL, e, p);

    si (!radix || !radix->titulus)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(*radix->titulus, "Error"))
    {
        _errorem_ponere(e, HORREUM_MALUM, CC, "", p);
        e->codex       = _textus(radix, "Code", p);
        e->nuntius     = _textus(radix, "Message", p);
        e->petitio_id  = _textus(radix, "RequestId", p);
        e->genus       = _genus_erroris(CC, e->codex);
        redde FALSUM;
    }
    si (!chorda_aequalis_literis(*radix->titulus,
            "CompleteMultipartUploadResult"))
    {
        _errorem_ponere(e, HORREUM_MALUM, ZEPHYRUM,
            "horreum: XML radix inexpectata", p);
        redde FALSUM;
    }
    *etag = _sine_virgulis(_textus(radix, "ETag", p));
    redde VERUM;
}

/* ===== politica ===== */
b32
horreum_iterandum (
     constans HorreumOpus* o,
    constans HorreumError* e,
                   chorda  mora_servi,
                      i32  conatus,
                      i32  conatus_maximi,
                      i32  basis,
                      i32  fortuna,
                      i32* mora)
{
    i32 b = basis > ZEPHYRUM ? basis : CC;
    i32 k;
    s64 ra;

    si (   !o || !e || !mora || e->genus != HORREUM_TRANSITORIUM
        || !o->idempotens || conatus < I || conatus >= conatus_maximi)
    {
        redde FALSUM;
    }
    per (k = I; k < conatus && b < (LX * M); k++)
    {
        b = b * II;
    }
    *mora  = b + (fortuna % b);
    ra     = _numerus(mora_servi);
    si (ra > ZEPHYRUM && ra * M > (s64)*mora)
    {
        *mora = ra > (s64)DC ? (i32)(DC * M) : (i32)(ra * M);
    }
    redde VERUM;
}

i64
horreum_partis_mensura (
    i64 tota,
    i64 petita)
{
    i64 mib = (i64)HORREUM_MIB;
    i64 pars = petita > ZEPHYRUM ? petita : (i64)HORREUM_PARS_ORDINARIA;

    si (pars < (i64)HORREUM_PARS_MINIMA)
    {
        pars = (i64)HORREUM_PARS_MINIMA;
    }
    pars = ((pars + mib - I) / mib) * mib;
    si ((tota + pars - I) / pars > (i64)HORREUM_PARTES_MAXIMAE)
    {
        pars = (tota + (i64)HORREUM_PARTES_MAXIMAE - I)
             / (i64)HORREUM_PARTES_MAXIMAE;
        pars = ((pars + mib - I) / mib) * mib;
    }
    redde pars;
}

/* ===== observatio ===== */
#define HORREUM_CORPUS_DESCRIPTUM (IV * MXXIV)

/* capita quae credentias ferunt: valor numquam describitur */
interior b32
_caput_tegendum (
      chorda  titulus,
     Piscina* p)
{
    redde chorda_aequalis_case_insensitivus(titulus, _l("Authorization",
              p))
        || chorda_aequalis_case_insensitivus(titulus,
              _l("X-Amz-Security-Token", p));
}

interior vacuum
_capita_describere (
          ChordaAedificator* a,
                  character  signum,
         constans HttpCaput* capita,
                        i32  numerus,
                    Piscina* p)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_character(a, signum);
        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, capita[i].titulus);
        chorda_aedificator_appendere_literis(a, ": ");
        si (_caput_tegendum(capita[i].titulus, p))
        {
            chorda_aedificator_appendere_literis(a, "[TECTUM]");
        }
        alioquin
        {
            chorda_aedificator_appendere_chorda(a, capita[i].valor);
        }
        chorda_aedificator_appendere_character(a, '\n');
    }
    chorda_aedificator_appendere_character(a, signum);
    chorda_aedificator_appendere_character(a, '\n');
}

/* textus (cum \t \n \r) ut est, ultra limitem abbreviatus; binarium
 * solum mensura */
interior vacuum
_corpus_describere (
    ChordaAedificator* a,
               chorda  corpus)
{
    i32 i;
    i32 n = corpus.mensura < (i32)HORREUM_CORPUS_DESCRIPTUM
            ? corpus.mensura : (i32)HORREUM_CORPUS_DESCRIPTUM;

    si (corpus.mensura == ZEPHYRUM)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < corpus.mensura; i++)
    {
        i8 c = corpus.datum[i];

        si (   (c < (i8)' ' && c != (i8)'\t' && c != (i8)'\n'
                && c != (i8)'\r')
            || c == (i8)0x7F)
        {
            chorda_aedificator_appendere_literis(a,
                "[corpus binarium: ");
            chorda_aedificator_appendere_i32(a, corpus.mensura);
            chorda_aedificator_appendere_literis(a, " octeti]\n");
            redde;
        }
    }
    chorda_aedificator_appendere_chorda(a, chorda_sectio(corpus,
        ZEPHYRUM, n));
    si (corpus.datum[n - I] != (i8)'\n')
    {
        chorda_aedificator_appendere_character(a, '\n');
    }
    si (n < corpus.mensura)
    {
        chorda_aedificator_appendere_literis(a, "[");
        chorda_aedificator_appendere_i32(a, corpus.mensura - n);
        chorda_aedificator_appendere_literis(a, " octeti omissi]\n");
    }
}

chorda
horreum_observationem_describere (
    constans HorreumObservatio* o,
                       Piscina* p)
{
    ChordaAedificator* a;
     HttpPetitioVisus  v;

    si (!o || !o->petitio || !p)
    {
        redde chorda_ex_buffer(NIHIL, ZEPHYRUM);
    }
    v = http_petitio_visus(o->petitio);
    a = chorda_aedificator_creare(p, MXXIV);
    chorda_aedificator_appendere_literis(a, "> ");
    chorda_aedificator_appendere_literis(a,
        http_methodus_nomen(v.methodus));
    chorda_aedificator_appendere_character(a, ' ');
    chorda_aedificator_appendere_chorda(a, v.via);
    chorda_aedificator_appendere_literis(a, "  (conatus ");
    chorda_aedificator_appendere_i32(a, o->conatus);
    chorda_aedificator_appendere_literis(a, ")\n");
    _capita_describere(a, '>', v.capita, v.capita_numerus, p);
    _corpus_describere(a, v.corpus);
    si (!o->responsum)
    {
        chorda_aedificator_appendere_literis(a,
            "< defectus transportus: ");
        chorda_aedificator_appendere_chorda(a, o->error_transportus);
        chorda_aedificator_appendere_literis(a, "\n\n");
        redde chorda_aedificator_finire(a);
    }
    chorda_aedificator_appendere_literis(a, "< ");
    chorda_aedificator_appendere_i32(a, o->responsum->status);
    chorda_aedificator_appendere_character(a, ' ');
    chorda_aedificator_appendere_chorda(a,
        o->responsum->status_descriptio);
    chorda_aedificator_appendere_character(a, '\n');
    _capita_describere(a, '<', o->responsum->capita,
        o->responsum->capita_numerus, p);
    _corpus_describere(a, o->responsum->corpus);
    chorda_aedificator_appendere_character(a, '\n');
    redde chorda_aedificator_finire(a);
}
