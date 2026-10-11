/* probatio_portae.c - PORTAE DECLARATAE (plan portae-declaratae T1,
 * T2): lector declarationum <porta> in capitibus cursorum et derivatio
 * operturae per suturam fictam (plagulae in memoria, registrum PORTAE
 * fictum, summaria effectuum ut litterae STML, litterae probationum).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
#include "internamentum.h"
#include "stml.h"
#include "portae.h"
#include <string.h>

/* mundus fictus: plagulae (via, textus) + registrum */
nomen structura {
    constans character* via;
    constans character* textus;
} PlagulaFicta;

nomen structura {
    constans PlagulaFicta* plagulae;   /* terminus via NIHIL */
                      Xar* tabula_portarum;
    /* via cursoris -> summarium STML; absens = ignotum (NIHIL licet) */
    constans PlagulaFicta* effectus;
    /* fons probationis -> litterae lineis divisae; absens = ignotum */
    constans PlagulaFicta* litterae;
} MundusPortarum;

interior Xar*
_enumerare (
     vacuum* datum,
    Piscina* piscina)
{
    constans MundusPortarum* m;
                        Xar* viae;
                        i32  i;

    m     = (constans MundusPortarum*)datum;
    viae  = xar_creare(piscina, (i32)magnitudo(chorda));

    per (i = ZEPHYRUM; m->plagulae[i].via != NIHIL; i++)
    {
        *(chorda*)xar_addere(viae) =
            chorda_ex_literis(m->plagulae[i].via,
            piscina);
    }
    redde viae;
}

interior b32
_legere (
      vacuum* datum,
      chorda  via,
     Piscina* piscina,
      chorda* textus)
{
    constans MundusPortarum* m = (constans MundusPortarum*)datum;
                        i32  i;

    per (i = ZEPHYRUM; m->plagulae[i].via != NIHIL; i++)
    {
        si (chorda_aequalis_literis(via, m->plagulae[i].via))
        {
            *textus = chorda_ex_literis(m->plagulae[i].textus, piscina);
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior constans PlagulaFicta*
_fictam_invenire (
    constans PlagulaFicta* tabula,
                   chorda  via)
{
    i32 i;

    per (i = ZEPHYRUM; tabula != NIHIL && tabula[i].via != NIHIL; i++)
    {
        si (chorda_aequalis_literis(via, tabula[i].via))
        {
            redde &tabula[i];
        }
    }
    redde NIHIL;
}

interior StmlNodus*
_effectus (
      vacuum* datum,
      chorda  cursor,
     Piscina* piscina)
{
    constans MundusPortarum* m = (constans MundusPortarum*)datum;
      constans PlagulaFicta* f = _fictam_invenire(m->effectus, cursor);
               StmlResultus  r;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    r = stml_legere_ex_literis(f->textus, piscina,
        internamentum_creare(piscina));
    redde r.successus ? r.elementum_radix : NIHIL;
}

interior Xar*
_litterae (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    constans MundusPortarum* m = (constans MundusPortarum*)datum;
      constans PlagulaFicta* f = _fictam_invenire(m->litterae, fons);
                        Xar* fructus;
         constans character* initium;
         constans character* finis;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    per (initium = f->textus; *initium != '\0'; initium = finis)
    {
        chorda* l = (chorda*)xar_addere(fructus);

        finis = strchr(initium, '\n');
        si (finis == NIHIL)
        {
            finis = initium + strlen(initium);
        }
        *l          = chorda_ex_literis(initium, piscina);
        l->mensura  = (i32)(finis - initium);
        si (*finis == '\n')
        {
            finis++;
        }
    }
    redde fructus;
}

/* registrum: "clavis=cursor" aut "clavis=cursor|probationes" ...
 * NIHIL */
interior Xar*
_registrum (
               Piscina* piscina,
    constans character* constans* paria)
{
    Xar* r = xar_creare(piscina, (i32)magnitudo(PortaeIngressus));
    i32  i;

    per (i = ZEPHYRUM; paria[i] != NIHIL; i++)
    {
           PortaeIngressus* p    = (PortaeIngressus*)xar_addere(r);
        constans character* aeq  = strchr(paria[i], '=');

        constans character* sep  = strchr(paria[i], '|');

        p->clavis          = chorda_ex_literis(paria[i], piscina);
        p->clavis.mensura  = (i32)(aeq - paria[i]);
        p->cursor          = chorda_ex_literis(aeq + 1, piscina);
        p->probationes     = chorda_ex_literis("", piscina);
        si (sep != NIHIL)
        {
            p->cursor.mensura  = (i32)(sep - (aeq + 1));
            p->probationes     = chorda_ex_literis(sep + 1, piscina);
        }
    }
    redde r;
}

interior vacuum
_suturam_parare (
           CredoContextus* c,
           MundusPortarum* mundus,
              PortaeSutura* sutura,
    constans PlagulaFicta* plagulae,
    constans character* constans* paria)
{
    mundus->plagulae             = plagulae;
    mundus->tabula_portarum      = _registrum(c->piscina, paria);
    mundus->effectus             = NIHIL;
    mundus->litterae             = NIHIL;
    sutura->datum                = mundus;
    sutura->plagulas_enumerare   = _enumerare;
    sutura->plagulam_legere      = _legere;
    sutura->effectus             = _effectus;
    sutura->litterae_vocationum  = _litterae;
    sutura->tabula_portarum      = mundus->tabula_portarum;
}

/* lectio una: Xar declarationum aut NIHIL cum causa */
interior Xar*
_legere_mundum (
    CredoContextus* c,
    constans PlagulaFicta* plagulae,
    constans character* constans* paria,
                 chorda* causa)
{
    MundusPortarum mundus;
      PortaeSutura sutura;

    _suturam_parare(c, &mundus, &sutura, plagulae, paria);
    *causa = chorda_ex_literis("", c->piscina);
    redde portae_declaratas_legere(&sutura, c->piscina, causa);
}

/* lectio + derivatio: operturae (declarationes sanae necessariae) */
interior Xar*
_derivare_mundum (
           CredoContextus* c,
    constans PlagulaFicta* plagulae,
    constans character* constans* paria,
    constans PlagulaFicta* effectus,
    constans PlagulaFicta* litterae)
{
    MundusPortarum  mundus;
      PortaeSutura  sutura;
            chorda  causa;
               Xar* d;

    _suturam_parare(c, &mundus, &sutura, plagulae, paria);
    mundus.effectus = effectus;
    mundus.litterae = litterae;
    d = portae_declaratas_legere(&sutura, c->piscina, &causa);
    si (d == NIHIL)
    {
        redde NIHIL;
    }
    redde portae_operturas_derivare(&sutura, d, c->piscina);
}

/* opertura cuius declaratio titulum (aut, manu, viam) habet */
interior constans PortaeOpertura*
_opertura (
                   Xar* o,
    constans character* clavis)
{
    i32 i;

    per (i = ZEPHYRUM; o != NIHIL && i < xar_numerus(o); i++)
    {
        constans PortaeOpertura* p = (constans PortaeOpertura*)
            xar_obtinere(o, i);

        si (   chorda_aequalis_literis(p->declaratio->titulus, clavis)
            || (p->declaratio->manu
                && chorda_aequalis_literis(p->declaratio->via, clavis)))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior constans PortaeExemplar*
_exemplar (
                   Xar* exemplaria,
    constans character* exemplar)
{
    i32 i;

    per (i = ZEPHYRUM; exemplaria != NIHIL
        && i < xar_numerus(exemplaria);
         i++)
    {
        constans PortaeExemplar* e = (constans PortaeExemplar*)
            xar_obtinere(exemplaria, i);

        si (chorda_aequalis_literis(e->exemplar, exemplar))
        {
            redde e;
        }
    }
    redde NIHIL;
}

/* exemplaria cum hoc exemplari (duplicata = vitium) */
interior i32
_exemplaria_numerare (
                   Xar* exemplaria,
    constans character* exemplar)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; exemplaria != NIHIL
        && i < xar_numerus(exemplaria);
         i++)
    {
        si (chorda_aequalis_literis(((constans PortaeExemplar*)
                xar_obtinere(exemplaria, i))->exemplar, exemplar))
        {
            n++;
        }
    }
    redde n;
}

interior b32
_ignotum_habet (
        CredoContextus* c,
                   Xar* ignota,
    constans character* pars)
{
    i32 i;

    per (i = ZEPHYRUM; ignota != NIHIL && i < xar_numerus(ignota); i++)
    {
        si (chorda_continet(*(chorda*)xar_obtinere(ignota, i),
                chorda_ex_literis(pars, c->piscina)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior constans PortaDeclarata*
_declaratio (
                   Xar* d,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; d != NIHIL && i < xar_numerus(d); i++)
    {
        constans PortaDeclarata* p = (constans PortaDeclarata*)
            xar_obtinere(d, i);

        si (chorda_aequalis_literis(p->titulus, titulus))
        {
            redde p;
        }
    }
    redde NIHIL;
}

/* I. lectio sana: titulus cum tegit/currit, manu cum causa, scriptum
 * duas portas currens, plagula sine annotatione praeterita */
interior vacuum
_sectio_lectio (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
          "#!/bin/bash\n"
          "# tools/credo_fumus.sh - porta credo\n"
          "# <porta titulus=\"credo\""
          " tegit=\"tools/credo_*.py lib/credo.c\""
          " currit=\"bin/aedilis\"/>\n"
          "set -u\necho sanum\n" },
        { "tools/rete_fumus.sh",
          "#!/bin/bash\n"
          "# <porta manu causa=\"rete poscit\"/>\n"
          "echo rete\n" },
        { "silva/examen_vectis.sh",
          "#!/bin/bash\n"
          "# <porta titulus=\"examen-vectis\"/>\n"
          "# <porta titulus=\"examen-corpus\""
          " tegit=\"x/exclusiones.txt\"/>\n"
          "echo vectis\n" },
        { "tools/alius.sh",
            "#!/bin/bash\n# commentum prosae\necho a\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh",
        "examen-vectis=silva/examen_vectis.sh",
        "examen-corpus=silva/examen_vectis.sh",
        NIHIL
    };
                     chorda  causa;
                        Xar* d;
    constans PortaDeclarata* p;
                        i32  i;
                        b32  manu_inventa = FALSUM;

    d = _legere_mundum(c, plagulae, paria, &causa);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_AEQUALIS_I32(xar_numerus(d), IV);
    p = _declaratio(d, "credo");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(chorda_aequalis_literis(p->via,
        "tools/credo_fumus.sh"));
    CREDO_AEQUALIS_I32(p->linea, III);
    CREDO_FALSUM(p->manu);
    CREDO_NECESSE(xar_numerus(p->tegit) == II);
    CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(p->tegit,
        ZEPHYRUM), "tools/credo_*.py"));
    CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(p->tegit,
        I),
        "lib/credo.c"));
    CREDO_NECESSE(xar_numerus(p->currit) == I);
    CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
        p->currit,
        ZEPHYRUM), "bin/aedilis"));
    CREDO_NON_NIHIL(_declaratio(d, "examen-vectis"));
    CREDO_NON_NIHIL(_declaratio(d, "examen-corpus"));
    per (i = ZEPHYRUM; i < xar_numerus(d); i++)
    {
        p = (constans PortaDeclarata*)xar_obtinere(d, i);
        si (p->manu)
        {
            manu_inventa = VERUM;
            CREDO_VERUM(chorda_aequalis_literis(p->via,
                "tools/rete_fumus.sh"));
            CREDO_VERUM(chorda_aequalis_literis(p->causa,
                "rete poscit"));
            CREDO_AEQUALIS_I32(p->titulus.mensura, ZEPHYRUM);
        }
    }
    CREDO_VERUM(manu_inventa);
}

/* II. '<porta' in CHORDA (non commentario) et annotationes aliae
 * praetereuntur - crusta commentaria novit, '#' in chorda non est */
interior vacuum
_sectio_non_commentaria (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
          "#!/bin/bash\n"
          "# <porta titulus=\"credo\"/>\n"
          "# <tolera codex=\"lint:x\" (>causa alia\n"
          "echo \"# <porta titulus=\\\"fictum\\\"/>\"\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh", NIHIL
    };
    chorda  causa;
       Xar* d = _legere_mundum(c, plagulae, paria, &causa);

    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_AEQUALIS_I32(xar_numerus(d), I);
    CREDO_VERUM(_declaratio(d, "fictum") == NIHIL);
}

/* III. recusationes singulae, causa nominata (via:linea) */
interior vacuum
_recusatio_probare (
           CredoContextus* c,
    constans character* textus,
    constans character* constans* paria,
    constans character* expecta)
{
    PlagulaFicta  plagulae[II];
          chorda  causa;
             Xar* d;

    plagulae[ZEPHYRUM].via = "tools/credo_fumus.sh";
    plagulae[ZEPHYRUM].textus = textus;
    plagulae[I].via = NIHIL;
    plagulae[I].textus = NIHIL;
    d = _legere_mundum(c, plagulae, paria, &causa);
    CREDO_VERUM(d == NIHIL);
    CREDO_VERUM(chorda_continet(causa, _ch(c, expecta)));
}

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    hic_manens constans character* const credo_solum[] = {
        "credo=tools/credo_fumus.sh", NIHIL
    };
    hic_manens constans character* const credo_alibi[] = {
        "credo=tools/alius.sh", NIHIL
    };
    hic_manens constans character* const credo_et_sera[] = {
        "credo=tools/credo_fumus.sh", "sera=tools/sera_fumus.sh", NIHIL
    };

    _recusatio_probare(c, "# <porta titulus=\"credo\"\n", credo_solum,
        "tools/credo_fumus.sh:1: annotatio porta illegibilis");
    _recusatio_probare(c, "# <porta titulus=\"credo\" tegti=\"x\"/>\n",
        credo_solum,
        "tools/credo_fumus.sh:1: attributum ignotum 'tegti'");
    _recusatio_probare(c,
        "# <porta titulus=\"credo\" manu causa=\"x\"/>\n",
        credo_solum, "titulus et manu simul");
    _recusatio_probare(c, "# <porta tegit=\"x\"/>\n", credo_solum,
        "nec titulus nec manu");
    _recusatio_probare(c,
        "# <porta manu/>\n# <porta titulus=\"credo\"/>\n",
        credo_solum, "tools/credo_fumus.sh:1: manu sine causa");
    _recusatio_probare(c, "# <porta titulus=\"ignota\"/>\n"
        "# <porta titulus=\"credo\"/>\n", credo_solum,
        "titulus 'ignota' extra registrum PORTAE");
    _recusatio_probare(c, "# <porta titulus=\"credo\"/>\n", credo_alibi,
        "cursor alienus: registrum 'credo' dat tools/alius.sh");
    _recusatio_probare(c, "# <porta titulus=\"credo\"/>\n"
        "# <porta titulus=\"credo\"/>\n", credo_solum,
        "titulus 'credo' bis declaratus");
    _recusatio_probare(c, "# <porta titulus=\"credo\"/>\n",
        credo_et_sera,
        "porta 'sera' (cursor tools/sera_fumus.sh) sine annotatione");
}

/* IV. causae OMNES una lectione (non prima sola) */
interior vacuum
_sectio_causae_omnes (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
          "# <porta titulus=\"credo\" tegti=\"x\"/>\n"
          "# <porta manu/>\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh", "sera=tools/sera_fumus.sh", NIHIL
    };
    chorda  causa;
       Xar* d = _legere_mundum(c, plagulae, paria, &causa);

    CREDO_VERUM(d == NIHIL);
    CREDO_VERUM(chorda_continet(causa, _ch(c,
        "attributum ignotum 'tegti'")));
    CREDO_VERUM(chorda_continet(causa, _ch(c, "manu sine causa")));
    CREDO_VERUM(chorda_continet(causa, _ch(c, "porta 'sera'")));
}

/* V. causae trans PLAGULAS: plagula prima vitiosa lectionem secundae
 * non sistit (olim ansa post causam primam desinebat) */
interior vacuum
_sectio_trans_plagulas (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
            "# <porta titulus=\"credo\" tegti=\"x\"/>\n" },
        { "tools/sera_fumus.sh", "# <porta manu/>\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh", NIHIL
    };
    chorda  causa;
       Xar* d = _legere_mundum(c, plagulae, paria, &causa);

    CREDO_VERUM(d == NIHIL);
    CREDO_VERUM(chorda_continet(causa, _ch(c,
        "tools/credo_fumus.sh:1: attributum ignotum 'tegti'")));
    CREDO_VERUM(chorda_continet(causa, _ch(c,
        "tools/sera_fumus.sh:1: manu sine causa")));
}

/* VI. derivatio cursoris (credo_fumus -> credo_convertere.py): cursor
 * ipse, situs effectuum (soli arboris, plene resoluti, non custoditi,
 * non scripturae), fontes .c compilati in fontes_c, tegit annotationis;
 * duplicata semel */
interior vacuum
_sectio_derivatio_cursoris (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
          "# <porta titulus=\"credo\" tegit=\"tools/credo_*.py\"/>\n" },
        { "tools/credo_convertere.py", "" },
        { "lib/credo.c", "" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh", NIHIL
    };
    hic_manens constans PlagulaFicta effectus[] = {
        { "tools/credo_fumus.sh",
          "<effectus lingua=\"bash\" radix=\"tools/credo_fumus.sh\">"
          "<processus radix=\"tools/credo_fumus.sh\">"
          "<exsecutio via=\"tools/credo_convertere.py\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<exsecutio via=\"tools/credo_convertere.py\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<lectio via=\"lib/credo.c\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\" mandatum=\"clang\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<fontatio via=\"tools/vexilla.sh\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<scriptura via=\"tools/scriptum.txt\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<lectio via=\"lib/\" forma=\"praefixum\""
          " resolutio=\"partialis\" classis=\"arbor\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<lectio via=\"build/x.o\" forma=\"via\""
          " resolutio=\"plena\" classis=\"build\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<exsecutio via=\"tools/aedificator.sh\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\" custodia=\"bin/x\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<ambitus_lectio titulus=\"TMPDIR\" assignatum=\"falsum\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "</processus>"
          "<processus radix=\"tools/aedificator.sh\""
          " custodia=\"bin/x\">"
          "<lectio via=\"tools/secretum.c\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\""
          " plagula=\"tools/aedificator.sh\"/>"
          "</processus>"
          "</effectus>" },
        { NIHIL, NIHIL }
    };
                        Xar* o;
    constans PortaeOpertura* p;
    constans PortaeExemplar* e;

    o = _derivare_mundum(c, plagulae, paria, effectus, NIHIL);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32(xar_numerus(o), I);
    p = _opertura(o, "credo");
    CREDO_NECESSE_NON_NIHIL(p);
    e = _exemplar(p->exemplaria, "tools/credo_fumus.sh");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, (i32)PORTAE_EXEMPLAR_VIA);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c, "cursor ipse")));
    e = _exemplar(p->exemplaria, "tools/credo_convertere.py");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "exsecutio in tools/credo_fumus.sh")));
    CREDO_AEQUALIS_I32(_exemplaria_numerare(p->exemplaria,
        "tools/credo_convertere.py"), I);
    e = _exemplar(p->exemplaria, "lib/credo.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c, "lectio (clang)")));
    CREDO_NON_NIHIL(_exemplar(p->exemplaria, "tools/vexilla.sh"));
    e = _exemplar(p->exemplaria, "tools/credo_*.py");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, (i32)PORTAE_EXEMPLAR_GLOBUS);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "tegit (tools/credo_fumus.sh:1)")));
    /* exclusa: scriptura, partialis, build, custoditi */
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/scriptum.txt")
        == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "lib/") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "build/x.o") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/aedificator.sh")
        == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/secretum.c") == NIHIL);
    /* fons compilatus: clausura eius T3 tegit */
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "lib/credo.c"));
    CREDO_AEQUALIS_I32(xar_numerus(p->fontes_c), I);
    CREDO_AEQUALIS_I32(xar_numerus(p->ignota), ZEPHYRUM);
}

/* VII. suita et vocationes (ludus_tessera -> tessera/...,
 * compile_tests -> tabulariumd): probationes suitae (directorium ipsum,
 * non subdirectoria) in fontes_c; globus .c effectus expansus; scripta
 * et binaria quae probationes vocant (binarium -> fons introitus per
 * nomen; ambiguum = ignotum nominatum; binaria probationum et litterae
 * aliae praetereuntur) */
interior vacuum
_sectio_derivatio_suitae (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tessera/compile_probationes.sh",
          "# <porta titulus=\"tessera\"/>\n" },
        { "tessera/probationes/probatio_folium.c", "" },
        { "tessera/probationes/probatio_musivum.c", "" },
        { "tessera/probationes/auxilia.h", "" },
        { "tessera/probationes/probatio_veteres/probatio_x.c", "" },
        { "gesta/tabulariumd.sh", "echo tabularium\n" },
        { "gesta/fontes/a.c", "" },
        { "gesta/fontes/b.c", "" },
        { "gesta/fontes/c.h", "" },
        { "tools/folium_principale.c", "" },
        { "tools/probationes/folium.c", "" },
        { "apps/x/musivum.c", "" },
        { "apps/y/musivum.c", "" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "tessera=tessera/compile_probationes.sh|tessera/probationes",
        NIHIL
    };
    hic_manens constans PlagulaFicta effectus[] = {
        { "tessera/compile_probationes.sh",
          "<effectus radix=\"tessera/compile_probationes.sh\">"
          "<processus radix=\"tessera/compile_probationes.sh\">"
          "<lectio via=\"gesta/fontes/*.c\" forma=\"globus\""
          " resolutio=\"plena\" classis=\"arbor\" mandatum=\"clang\""
          " plagula=\"tessera/compile_probationes.sh\"/>"
          "</processus></effectus>" },
        { NIHIL, NIHIL }
    };
    hic_manens constans PlagulaFicta litterae[] = {
        { "tessera/probationes/probatio_folium.c",
          "./gesta/tabulariumd.sh -sanum\nbin/folium -v\nsalve mundus\n"
          "bin/probatio_folium\nbin/folium\n" },
        { "tessera/probationes/probatio_musivum.c", "bin/musivum\n" },
        { NIHIL, NIHIL }
    };
                        Xar* o;
    constans PortaeOpertura* p;
    constans PortaeExemplar* e;

    o = _derivare_mundum(c, plagulae, paria, effectus, litterae);
    CREDO_NECESSE_NON_NIHIL(o);
    p = _opertura(o, "tessera");
    CREDO_NECESSE_NON_NIHIL(p);
    e = _exemplar(p->fontes_c, "tessera/probationes/probatio_folium.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "probatio suitae tessera/probationes")));
    CREDO_NON_NIHIL(_exemplar(p->fontes_c,
        "tessera/probationes/probatio_musivum.c"));
    CREDO_VERUM(_exemplar(p->fontes_c,
        "tessera/probationes/probatio_veteres/probatio_x.c") == NIHIL);
    CREDO_VERUM(_exemplar(p->fontes_c,
        "tessera/probationes/auxilia.h") == NIHIL);
    /* globus .c: exemplar ipse + fontes expansi (non .h) */
    e = _exemplar(p->exemplaria, "gesta/fontes/*.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, (i32)PORTAE_EXEMPLAR_GLOBUS);
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "gesta/fontes/a.c"));
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "gesta/fontes/b.c"));
    CREDO_VERUM(_exemplar(p->fontes_c, "gesta/fontes/c.h") == NIHIL);
    /* scriptum vocatum: './' demptum, verbum primum solum */
    e = _exemplar(p->exemplaria, "gesta/tabulariumd.sh");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "tessera/probationes/probatio_folium.c currit")));
    /* binarium -> fons introitus (probationes/ omissae), semel */
    e = _exemplar(p->fontes_c, "tools/folium_principale.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c, "currit bin/folium")));
    CREDO_AEQUALIS_I32(_exemplaria_numerare(p->fontes_c,
        "tools/folium_principale.c"), I);
    CREDO_VERUM(_exemplar(p->fontes_c, "tools/probationes/folium.c")
        == NIHIL);
    /* ambiguum nominatum, numquam electum */
    CREDO_VERUM(_ignotum_habet(c, p->ignota, "bin/musivum"));
    CREDO_VERUM(_exemplar(p->fontes_c, "apps/x/musivum.c") == NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(p->ignota), I);
}

/* VIII. ignota et annotatio 'currit': effectus ignotum, litterae
 * ignotae nominantur (cursor ipse manet); currit -> fons .c, scriptum,
 * binarium per nomen, ignotum; porta manu etiam derivatur */
interior vacuum
_sectio_derivatio_ignota (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "silva/compile_probationes.sh",
          "# <porta titulus=\"silva\""
          " currit=\"bin/folium tools/x.c tools/y.sh"
          " bin/nullum\"/>\n" },
        { "silva/probationes/probatio_a.c", "" },
        { "tools/rete_fumus.sh",
          "# <porta manu causa=\"rete\"/>\n" },
        { "tools/folium.c", "" },
        { "tools/x.c", "" },
        { "tools/y.sh", "" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "silva=silva/compile_probationes.sh|silva/probationes", NIHIL
    };
                        Xar* o;
    constans PortaeOpertura* p;
    constans PortaeExemplar* e;

    o = _derivare_mundum(c, plagulae, paria, NIHIL, NIHIL);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_AEQUALIS_I32(xar_numerus(o), II);
    p = _opertura(o, "silva");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_NON_NIHIL(_exemplar(p->exemplaria,
        "silva/compile_probationes.sh"));
    CREDO_VERUM(_ignotum_habet(c, p->ignota,
        "effectus silva/compile_probationes.sh ignotum"));
    CREDO_VERUM(_ignotum_habet(c, p->ignota,
        "litterae silva/probationes/probatio_a.c ignotae"));
    e = _exemplar(p->fontes_c, "tools/folium.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "currit bin/folium (silva/compile_probationes.sh:1)")));
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "tools/x.c"));
    e = _exemplar(p->exemplaria, "tools/y.sh");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32((i32)e->genus, (i32)PORTAE_EXEMPLAR_VIA);
    CREDO_VERUM(_ignotum_habet(c, p->ignota, "currit bin/nullum"));
    p = _opertura(o, "tools/rete_fumus.sh");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_NON_NIHIL(_exemplar(p->exemplaria, "tools/rete_fumus.sh"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lectio sana", _sectio_lectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chordae et annotationes aliae", _sectio_non_commentaria, NIHIL,
      NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "causae omnes", _sectio_causae_omnes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "causae trans plagulas", _sectio_trans_plagulas, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "derivatio cursoris", _sectio_derivatio_cursoris, NIHIL, NIHIL,
      NIHIL, NIHIL },
    { "derivatio suitae", _sectio_derivatio_suitae, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "derivatio ignota", _sectio_derivatio_ignota, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("portae", SECTIONES);
}
