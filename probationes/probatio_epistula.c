/* probatio_epistula.c - epistula (communio-plan-3 T2): corpus Resend,
 * provisor fictus, Resend per hospitium locale (caput, corpus,
 * Idempotency-Key), provisor deficiens sine clave in causa */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "reactor.h"
#include "hospitium.h"
#include "epistula.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>

#define CLAVIS_PROBATIONIS "re_probatio_CLAVIS_NUMQUAM_VISA"

hic_manens Piscina* piscina;

/* quod servus Resend fictus vidit */
hic_manens structura {
       i32 petitiones;
       i32 status;          /* responsum dandum */
    chorda auctoritas;
    chorda idempotentia;
    chorda corpus;
} servus;

/* finis missionis */
hic_manens structura {
       i32 vocationes;
       b32 successus;
    chorda causa;
} exitus;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
    memset(&servus, ZEPHYRUM, magnitudo(servus));
    memset(&exitus, ZEPHYRUM, magnitudo(exitus));
    servus.status = CC;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior chorda
copia (
    chorda c)
{
    redde chorda_ex_literis(chorda_ut_cstr(c, piscina), piscina);
}

interior vacuum
_finis (
     vacuum* ctx,
        b32  successus,
     chorda  causa)
{
    (vacuum)ctx;
    exitus.vocationes++;
    exitus.successus  = successus;
    exitus.causa      = copia(causa);
}

interior EpistulaNuntius
nuntius_bonus (vacuum)
{
    EpistulaNuntius n;

    memset(&n, ZEPHYRUM, magnitudo(n));
    n.a                     = lit("communio@example.org");
    n.ad                    = lit("persona@example.org");
    n.titulus               = lit("Sign in to golden.earth");
    n.textus                = lit("Code: ABCD-EFGH\n\"quoted\" line");
    n.html                  = lit("<p>Code: <b>ABCD-EFGH</b></p>");
    n.clavis_idempotentiae  = lit("01ARZ3NDEKTSV4RRFFQ69G5FAV");
    redde n;
}

interior vacuum
_viam_epistularum (
    HospitiumColloquium* col)
{
    servus.petitiones++;
    servus.auctoritas = copia(colloquium_caput(col,
        "Authorization"));
    servus.idempotentia = copia(colloquium_caput(col,
        "Idempotency-Key"));
    servus.corpus = copia(colloquium_petitio(col)->corpus);
    colloquium_respondere(col, servus.status, "application/json",
        lit("{\"id\":\"x\"}"));
}

interior vacuum
agere_donec_finis (
    Reactor* r)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; exitus.vocationes == ZEPHYRUM && gyri < D;
         gyri++)
    {
        (vacuum)reactor_poll(r, X);
    }
}

interior vacuum
_sectio_corpus (
    CredoContextus* c)
{
     EpistulaNuntius n;
              chorda t;
        JsonResultus j;

    (vacuum)c;
    n = nuntius_bonus();
    t = epistula_resend_corpus(&n, piscina);
    j = json_legere(t, piscina);
    CREDO_NECESSE(j.successus);
    CREDO_CHORDA_AEQUALIS(json_capere_chorda(j.radix, "from", lit("")),
        n.a);
    CREDO_CHORDA_AEQUALIS(json_capere_chorda(j.radix, "subject",
        lit("")),
        n.titulus);
    CREDO_CHORDA_AEQUALIS(json_tabulatum_obtinere(json_objectum_capere(
        j.radix, "to"), ZEPHYRUM)->datum.chorda_valor, n.ad);
    CREDO_CHORDA_AEQUALIS(json_capere_chorda(j.radix, "text", lit("")),
        n.textus);
    CREDO_CHORDA_AEQUALIS(json_capere_chorda(j.radix, "html", lit("")),
        n.html);

    CREDO_GRADUS("sine html");
    n.html  = lit("");
    j       = json_legere(epistula_resend_corpus(&n, piscina), piscina);
    CREDO_NECESSE(j.successus);
    CREDO_NIHIL(json_objectum_capere(j.radix, "html"));
}

interior vacuum
_sectio_fictus (
    CredoContextus* c)
{
             Reactor* r;
        Epistularius* e;
     EpistulaNuntius  n;
     EpistulaNuntius  lectus;

    (vacuum)c;
    r = reactor_creare(piscina);
    e = epistularium_aperire(piscina, r, "fictus", lit(""), NIHIL);
    CREDO_NECESSE(r && e);
    n = nuntius_bonus();
    CREDO_VERUM(epistulam_mittere(e, &n, _finis, NIHIL));
    agere_donec_finis(r);
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_VERUM(exitus.successus);
    n.ad = lit("altera@example.org");
    CREDO_VERUM(epistulam_mittere(e, &n, _finis, NIHIL));
    agere_donec_finis(r);
    exitus.vocationes = ZEPHYRUM;
    agere_donec_finis(r);
    CREDO_AEQUALIS_I32(epistularius_fictus_numerus(e), II);
    CREDO_VERUM(epistularius_fictus_nuntius(e, ZEPHYRUM, &lectus));
    CREDO_CHORDA_AEQUALIS_LITERIS(lectus.ad, "persona@example.org");
    CREDO_CHORDA_AEQUALIS(lectus.textus, n.textus);
    CREDO_CHORDA_AEQUALIS(lectus.clavis_idempotentiae,
        n.clavis_idempotentiae);
    CREDO_VERUM(epistularius_fictus_nuntius(e, I, &lectus));
    CREDO_CHORDA_AEQUALIS_LITERIS(lectus.ad, "altera@example.org");
    CREDO_FALSUM(epistularius_fictus_nuntius(e, II, &lectus));

    CREDO_GRADUS("nuntius malus; provisor ignotus");
    n.titulus = lit("");
    CREDO_FALSUM(epistulam_mittere(e, &n, _finis, NIHIL));
    CREDO_NIHIL(epistularium_aperire(piscina, r, "columba", lit(""),
        NIHIL));
    epistularium_claudere(e);
    reactor_destruere(r);
}

/* Resend per hospitium locale in reactore communi */
interior vacuum
_sectio_resend (
    CredoContextus* c)
{
                  Reactor* r;
                Hospitium* h;
    HospitiumConfiguratio  cfg;
             Epistularius* e;
          EpistulaNuntius  n;
                character  basis[LXIV];

    (vacuum)c;
    r = reactor_creare(piscina);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor  = r;
    h            = r ? hospitium_creare(piscina, &cfg) : NIHIL;
    CREDO_NECESSE(h
        && hospitium_praebere(h, HTTP_POST, "/emails",
        _viam_epistularum,
        NIHIL));
    sprintf(basis, "http://127.0.0.1:%d", (integer)hospitium_portus(h));
    e = epistularium_aperire(piscina, r, "resend",
        lit(CLAVIS_PROBATIONIS),
        basis);
    CREDO_NECESSE_NON_NIHIL(e);
    n = nuntius_bonus();
    CREDO_VERUM(epistulam_mittere(e, &n, _finis, NIHIL));
    agere_donec_finis(r);
    CREDO_AEQUALIS_I32(servus.petitiones, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(servus.auctoritas,
        "Bearer " CLAVIS_PROBATIONIS);
    CREDO_CHORDA_AEQUALIS(servus.idempotentia, n.clavis_idempotentiae);
    CREDO_CHORDA_AEQUALIS(servus.corpus, epistula_resend_corpus(&n,
        piscina));
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_VERUM(exitus.successus);

    CREDO_GRADUS("provisor deficit: causa sine clave");
    servus.status      = D;
    exitus.vocationes  = ZEPHYRUM;
    CREDO_VERUM(epistulam_mittere(e, &n, _finis, NIHIL));
    agere_donec_finis(r);
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_FALSUM(exitus.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus.causa, "resend: HTTP 500");

    CREDO_GRADUS("sine Idempotency-Key si vacua");
    servus.status           = CC;
    exitus.vocationes       = ZEPHYRUM;
    n.clavis_idempotentiae  = lit("");
    CREDO_VERUM(epistulam_mittere(e, &n, _finis, NIHIL));
    agere_donec_finis(r);
    CREDO_AEQUALIS_I32(servus.idempotentia.mensura, ZEPHYRUM);
    epistularium_claudere(e);
    hospitium_destruere(h);
    reactor_destruere(r);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "corpus Resend", _sectio_corpus, _parare, _purgare, NIHIL,
        NIHIL },
    { "fictus", _sectio_fictus, _parare, _purgare, NIHIL, NIHIL },
    { "resend per hospitium", _sectio_resend, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("epistula", SECTIONES);
}
