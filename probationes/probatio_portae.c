/* probatio_portae.c - PORTAE DECLARATAE (plan portae-declaratae T1,
 * T2, T3): lector declarationum <porta> in capitibus cursorum,
 * derivatio operturae, debita viarum mutatarum et contractus machinae
 * per suturam fictam (plagulae in memoria, registrum PORTAE fictum,
 * summaria effectuum ut litterae STML, litterae probationum,
 * clausurae fictae cum numero vocationum).
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
    /* fons C -> clausura lineis divisa; absens = ignota */
    constans PlagulaFicta* clausurae;
                      i32  clausurae_vocatae;
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
_lineae (
    constans PlagulaFicta* f,
                  Piscina* piscina)
{
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

interior Xar*
_litterae (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    constans MundusPortarum* m = (constans MundusPortarum*)datum;

    redde _lineae(_fictam_invenire(m->litterae, fons), piscina);
}

interior Xar*
_clausura (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    MundusPortarum* m = (MundusPortarum*)datum;

    m->clausurae_vocatae++;
    redde _lineae(_fictam_invenire(m->clausurae, fons), piscina);
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
    mundus->plagulae              = plagulae;
    mundus->tabula_portarum       = _registrum(c->piscina, paria);
    mundus->effectus              = NIHIL;
    mundus->litterae              = NIHIL;
    mundus->clausurae             = NIHIL;
    mundus->clausurae_vocatae     = ZEPHYRUM;
    sutura->datum                 = mundus;
    sutura->plagulas_enumerare    = _enumerare;
    sutura->plagulam_legere       = _legere;
    sutura->effectus              = _effectus;
    sutura->litterae_probationis  = _litterae;
    sutura->clausura_c            = _clausura;
    sutura->tabula_portarum       = mundus->tabula_portarum;
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
    redde portae_declaratas_legere(&sutura, c->piscina, causa, NIHIL);
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
    d = portae_declaratas_legere(&sutura, c->piscina, &causa, NIHIL);
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
        { "tools/fabrica.c", "" },
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
          "<lectio via=\"tools/.fumus_generatum.c\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\" mandatum=\"clang\""
          " scripta_in_ambitu=\"verum\""
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
          "<exsecutio via=\"bin/fabrica\" forma=\"via\""
          " resolutio=\"plena\" classis=\"instrumentum_domus\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<probatio via=\"include/credo.h\" forma=\"via\""
          " resolutio=\"plena\" classis=\"arbor\" operator=\"-nt\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<enumeratio via=\"include/*.h\" forma=\"globus\""
          " resolutio=\"plena\" classis=\"arbor\" mandatum=\"ls\""
          " plagula=\"tools/credo_fumus.sh\"/>"
          "<probatio via=\"bin/aedilis\" forma=\"via\""
          " resolutio=\"plena\" classis=\"instrumentum_domus\""
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
    /* exclusa: probatio et enumeratio (tempus aut existentia, non
     * contentum - custodia capitum '[ obj -nt caput ]'), scriptura,
     * partialis, build, custoditi */
    CREDO_VERUM(_exemplar(p->exemplaria, "include/credo.h") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "include/*.h") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/scriptum.txt")
        == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "lib/") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "build/x.o") == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/aedificator.sh")
        == NIHIL);
    CREDO_VERUM(_exemplar(p->exemplaria, "tools/secretum.c") == NIHIL);
    /* fons compilatus: clausura eius T3 tegit */
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "lib/credo.c"));
    /* instrumentum domus exsecutum -> fons introitus (probatio '-x'
     * non) */
    e = _exemplar(p->fontes_c, "tools/fabrica.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "tools/credo_fumus.sh currit bin/fabrica")));
    /* fons non tractus (scriptum ipsum eum scribit): clausura nulla */
    CREDO_VERUM(_exemplar(p->fontes_c, "tools/.fumus_generatum.c")
        == NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(p->fontes_c), II);
    CREDO_AEQUALIS_I32(xar_numerus(p->ignota), ZEPHYRUM);
    /* binaria quae porta currit (circulus commissionis II T2):
     * exsecutio bin/fabrica sic; probatio '-x' bin/aedilis non
     * (existentia, non vocatio) */
    e = _exemplar(p->binaria, "bin/fabrica");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "tools/credo_fumus.sh currit bin/fabrica")));
    CREDO_VERUM(_exemplar(p->binaria, "bin/aedilis") == NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(p->binaria), I);
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
        { "tools/acies.c", "" },
        { "lib/acies.c", "" },
        { "briar/instrumenta/acies.c", "" },
        { "tools/probationes/folium.c", "" },
        { "apps/x/musivum.c", "" },
        { "apps/y/musivum.c", "" },
        { "tessera/fixa/tabula.txt", "" },
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
          "bin/probatio_folium\nbin/folium\n"
          "tessera/fixa/tabula.txt\n" },
        { "tessera/probationes/probatio_musivum.c",
          "bin/musivum\nbin/nusquam -x\nbin/acies\n" },
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
    /* fixum nominatum: dependentia, verbum 'nominat' */
    e = _exemplar(p->exemplaria, "tessera/fixa/tabula.txt");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c,
        "probatio_folium.c nominat tessera/fixa/tabula.txt")));
    /* binarium -> fons introitus (probationes/ omissae), semel */
    e = _exemplar(p->fontes_c, "tools/folium_principale.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c, "currit bin/folium")));
    CREDO_AEQUALIS_I32(_exemplaria_numerare(p->fontes_c,
        "tools/folium_principale.c"), I);
    CREDO_VERUM(_exemplar(p->fontes_c, "tools/probationes/folium.c")
        == NIHIL);
    /* ambiguum: candidati OMNES tecti (tutius), ratio id dicit */
    e = _exemplar(p->fontes_c, "apps/x/musivum.c");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(chorda_continet(e->ratio, _ch(c, "ambiguum")));
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "apps/y/musivum.c"));
    /* bin/x: candidatus unicus in tools/ praefertur */
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "tools/acies.c"));
    CREDO_VERUM(_exemplar(p->fontes_c, "lib/acies.c") == NIHIL);
    CREDO_VERUM(_exemplar(p->fontes_c, "briar/instrumenta/acies.c")
        == NIHIL);
    /* binarium sine fonte: nota, non caecum */
    CREDO_VERUM(_ignotum_habet(c, p->notae, "bin/nusquam"));
    CREDO_AEQUALIS_I32(xar_numerus(p->ignota), ZEPHYRUM);
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
    /* binaria (circulus commissionis II T2): 'currit bin/x' et
     * 'currit tools/x.c' -> bin/x candidatus; scriptum non */
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/folium"));
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/x"));
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/nullum"));
    CREDO_AEQUALIS_I32(xar_numerus(p->binaria), III);
    p = _opertura(o, "tools/rete_fumus.sh");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_NON_NIHIL(_exemplar(p->exemplaria, "tools/rete_fumus.sh"));
}

/* mundus debitorum communis: credo (cursor + fons compilatus), tessera
 * (suita), rete (manu) */
hic_manens constans PlagulaFicta _plagulae_debitorum[] = {
    { "tools/credo_fumus.sh", "# <porta titulus=\"credo\"/>\n" },
    { "tools/credo_convertere.py", "" },
    { "lib/credo.c", "" },
    { "include/credo.h", "" },
    { "tessera/compile_probationes.sh",
      "# <porta titulus=\"tessera\" tegit=\"tessera/fixa/*\"/>\n" },
    { "tessera/probationes/probatio_a.c", "" },
    { "tessera/probationes/probatio_b.c", "" },
    { "tools/rete_fumus.sh",
      "# <porta manu causa=\"rete poscit\"/>\n" },
    { "docs/x.md", "" },
    { "silva/aeq.sh",
      "# <porta titulus=\"aeq\" manu"
      " causa=\"basis manu condita\"/>\n" },
    { NIHIL, NIHIL }
};

hic_manens constans character* constans _paria_debitorum[] = {
    "tessera=tessera/compile_probationes.sh|tessera/probationes",
    "credo=tools/credo_fumus.sh", "aeq=silva/aeq.sh", NIHIL
};

hic_manens constans PlagulaFicta _effectus_debitorum[] = {
    { "tools/credo_fumus.sh",
      "<effectus><processus radix=\"tools/credo_fumus.sh\">"
      "<exsecutio via=\"tools/credo_convertere.py\" forma=\"via\""
      " resolutio=\"plena\" classis=\"arbor\""
      " plagula=\"tools/credo_fumus.sh\"/>"
      "<lectio via=\"lib/credo.c\" forma=\"via\" resolutio=\"plena\""
      " classis=\"arbor\" mandatum=\"clang\""
      " plagula=\"tools/credo_fumus.sh\"/>"
      "</processus></effectus>" },
    { "tessera/compile_probationes.sh", "<effectus/>" },
    { "tools/rete_fumus.sh", "<effectus/>" },
    { "silva/aeq.sh", "<effectus/>" },
    { NIHIL, NIHIL }
};

hic_manens constans PlagulaFicta _litterae_debitorum[] = {
    { "tessera/probationes/probatio_a.c", "" },
    { "tessera/probationes/probatio_b.c", "" },
    { NIHIL, NIHIL }
};

/* lectio + derivatio + debita viarum (lineis divisae) in mundo dato;
 * *vocatae = clausurae rogatae */
interior Xar*
_debita_mundi (
           CredoContextus*  c,
    constans PlagulaFicta*  effectus,
    constans PlagulaFicta*  clausurae,
       constans character*  viae_lineis,
                      Xar** intecta,
                      i32*  vocatae,
                      b32   tactus)
{
    MundusPortarum  mundus;
      PortaeSutura  sutura;
            chorda  causa;
               Xar* d;
               Xar* o;
               Xar* viae;
      PlagulaFicta  viae_fictae;
               Xar* debita;

    _suturam_parare(c, &mundus, &sutura, _plagulae_debitorum,
        _paria_debitorum);
    mundus.effectus = effectus;
    mundus.litterae = _litterae_debitorum;
    mundus.clausurae = clausurae;
    d = portae_declaratas_legere(&sutura, c->piscina, &causa, NIHIL);
    si (d == NIHIL)
    {
        redde NIHIL;
    }
    o = portae_operturas_derivare(&sutura, d, c->piscina);
    si (o == NIHIL)
    {
        redde NIHIL;
    }
    viae_fictae.via           = "viae";
    viae_fictae.textus        = viae_lineis;
    viae                      = _lineae(&viae_fictae, c->piscina);
    mundus.clausurae_vocatae  = ZEPHYRUM;
    debita = tactus ? portae_tactus_computare(&sutura, o, viae, intecta,
        c->piscina) : portae_debita_computare(&sutura, o, viae, intecta,
        c->piscina);
    *vocatae = mundus.clausurae_vocatae;
    redde debita;
}

interior constans PortaeDebitum*
_debitum (
                   Xar* debita,
    constans character* porta)
{
    i32 i;

    per (i = ZEPHYRUM; debita != NIHIL && i < xar_numerus(debita); i++)
    {
        constans PortaeDebitum* p = (constans PortaeDebitum*)
            xar_obtinere(debita, i);

        si (chorda_aequalis_literis(p->porta, porta))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior b32
_chordam_habet (
                   Xar* x,
    constans character* valor)
{
    i32 i;

    per (i = ZEPHYRUM; x != NIHIL && i < xar_numerus(x); i++)
    {
        si (chorda_aequalis_literis(*(chorda*)xar_obtinere(x, i),
            valor))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* IX. debita: via exemplari tecta, clausura fontis, manu nominata,
 * intecta; ordo registri deinde manu; clausurae solum si via C mutata
 * et semel per fontem; causae plures '(+N aliae)'; tactus: rationes
 * OMNES per (viam, portam) */
interior vacuum
_sectio_debita (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta clausurae[] = {
        { "lib/credo.c", "lib/credo.c\ninclude/credo.h\n" },
        { "tessera/probationes/probatio_a.c",
          "tessera/probationes/probatio_a.c\ninclude/credo.h\n" },
        { "tessera/probationes/probatio_b.c",
          "tessera/probationes/probatio_b.c\n" },
        { NIHIL, NIHIL }
    };
                       Xar* debita;
                       Xar* intecta;
                       i32  vocatae;
    constans PortaeDebitum* p;

    /* nulla via C: clausurae non rogantur */
    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "tools/credo_convertere.py\ntools/rete_fumus.sh\ndocs/x.md\n",
        &intecta, &vocatae, FALSUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    CREDO_AEQUALIS_I32(vocatae, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(debita), II);
    p = (constans PortaeDebitum*)xar_obtinere(debita, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(p->porta, "credo"));
    CREDO_FALSUM(p->manu);
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "tools/credo_convertere.py (exsecutio in"
        " tools/credo_fumus.sh)"));
    p = (constans PortaeDebitum*)xar_obtinere(debita, I);
    CREDO_VERUM(chorda_aequalis_literis(p->porta,
        "tools/rete_fumus.sh"));
    CREDO_VERUM(p->manu);
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "tools/rete_fumus.sh (cursor ipse)"));
    CREDO_AEQUALIS_I32(xar_numerus(intecta), I);
    CREDO_VERUM(_chordam_habet(intecta, "docs/x.md"));
    /* caput C: clausurae fontium (semel quisque); ordo registri
     * (tessera ante credo); causae plures */
    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "include/credo.h\nlib/credo.c\n", &intecta, &vocatae, FALSUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    CREDO_AEQUALIS_I32(vocatae, III);
    CREDO_AEQUALIS_I32(xar_numerus(debita), II);
    p = (constans PortaeDebitum*)xar_obtinere(debita, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(p->porta, "tessera"));
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "include/credo.h in clausura tessera/probationes/probatio_a.c"
        " (probatio suitae tessera/probationes)"));
    p = _debitum(debita, "credo");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "include/credo.h in clausura lib/credo.c (lectio (clang) in"
        " tools/credo_fumus.sh) (+1 aliae)"));
    CREDO_AEQUALIS_I32(xar_numerus(intecta), ZEPHYRUM);
    /* tactus: par (via, porta) cum rationibus omnibus; lineae TANGIT */
    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "include/credo.h\nlib/credo.c\n", &intecta, &vocatae, VERUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    CREDO_AEQUALIS_I32(xar_numerus(debita), III);
    {
        constans PortaeTactus* t = NIHIL;
                          i32  k;

        per (k = ZEPHYRUM; k < xar_numerus(debita); k++)
        {
            constans PortaeTactus* u = (constans PortaeTactus*)
                xar_obtinere(debita, k);

            si (   chorda_aequalis_literis(u->via, "lib/credo.c")
                && chorda_aequalis_literis(u->porta, "credo"))
            {
                t = u;
            }
        }
        CREDO_NECESSE_NON_NIHIL(t);
        /* exemplar et fons_c eandem rationem dant: semel */
        CREDO_AEQUALIS_I32(xar_numerus(t->rationes), I);
    }
    CREDO_VERUM(chorda_continet(portae_tactus_scribere(debita, intecta,
        VERUM, c->piscina), _ch(c,
        "TANGIT\tinclude/credo.h\tcredo\t0\tinclude/credo.h in clausura"
        " lib/credo.c (lectio (clang) in tools/credo_fumus.sh)\n")));
    /* porta registrata manu (titulus + manu): debitum MANU nomine
     * registri */
    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "silva/aeq.sh\n", &intecta, &vocatae, FALSUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    CREDO_AEQUALIS_I32(xar_numerus(debita), I);
    p = (constans PortaeDebitum*)xar_obtinere(debita, ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis_literis(p->porta, "aeq"));
    CREDO_VERUM(p->manu);
    /* tegit annotationis */
    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "tessera/fixa/a.txt\n", &intecta, &vocatae, FALSUM);
    p = _debitum(debita, "tessera");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "tessera/fixa/a.txt congruit 'tessera/fixa/*' (tegit"
        " (tessera/compile_probationes.sh:1))"));
}

/* X. clausura ignota DEBETUR; opertura caeca omni viae debetur sed
 * viam non tegit (INTECTUM manet) */
interior vacuum
_sectio_debita_caeca (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta clausurae[] = {
        { "lib/credo.c", "lib/credo.c\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans PlagulaFicta effectus_sine_credo[] = {
        { "tessera/compile_probationes.sh", "<effectus/>" },
        { "tools/rete_fumus.sh", "<effectus/>" },
        { NIHIL, NIHIL }
    };
                       Xar* debita;
                       Xar* intecta;
                       i32  vocatae;
    constans PortaeDebitum* p;

    debita = _debita_mundi(c, _effectus_debitorum, clausurae,
        "include/alia.h\n", &intecta, &vocatae, FALSUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    p = _debitum(debita, "tessera");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(chorda_continet(p->causa, _ch(c,
        "include/alia.h: clausura tessera/probationes/probatio_a.c"
        " ignota - debetur")));
    CREDO_VERUM(_debitum(debita, "credo") == NIHIL);
    /* effectus credo ignotum: caeca */
    debita = _debita_mundi(c, effectus_sine_credo, clausurae,
        "docs/x.md\n", &intecta, &vocatae, FALSUM);
    CREDO_NECESSE_NON_NIHIL(debita);
    p = _debitum(debita, "credo");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(chorda_aequalis_literis(p->causa,
        "docs/x.md: effectus tools/credo_fumus.sh ignotum (quae cursor"
        " legit et exsequitur caeca) - debetur"));
    CREDO_VERUM(_chordam_habet(intecta, "docs/x.md"));
}

/* XI. contractus machinae: effugium, lineae TSV exactae, forma
 * hominis, recusationes structae */
interior vacuum
_sectio_contractus (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/credo_fumus.sh",
          "#!/bin/bash\n# <porta titulus=\"credo\" tegti=\"x\"/>\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "credo=tools/credo_fumus.sh", "sera=tools/sera_fumus.sh", NIHIL
    };
               Xar* debita;
               Xar* intecta;
               Xar* recusationes;
               Xar* binaria;
     PortaeDebitum* d;
    PortaeExemplar* e_b;
    MundusPortarum  mundus;
      PortaeSutura  sutura;
            chorda  causa;
            chorda  t;

    CREDO_VERUM(chorda_aequalis_literis(portae_campum_effugere(_ch(c,
        "a\tb\nc\\d\re"), c->piscina), "a\\tb\\nc\\\\d\\re"));
    debita   = xar_creare(c->piscina, (i32)magnitudo(PortaeDebitum));
    intecta  = xar_creare(c->piscina, (i32)magnitudo(chorda));
    /* BINARIUM (circulus commissionis II T2): binaria quae porta
     * currit post DEBITUM suum; porta manu nulla (homo currit) */
    binaria = xar_creare(c->piscina, (i32)magnitudo(PortaeExemplar));
    e_b = (PortaeExemplar*)xar_addere(binaria);
    e_b->exemplar = _ch(c, "bin/fabrica");
    e_b->genus = PORTAE_EXEMPLAR_VIA;
    e_b->ratio = _ch(c, "currit bin/fabrica");
    d = (PortaeDebitum*)xar_addere(debita);
    d->porta = _ch(c, "credo");
    d->causa = _ch(c, "x.c (a\tb)");
    d->manu = FALSUM;
    d->binaria = binaria;
    d = (PortaeDebitum*)xar_addere(debita);
    d->porta = _ch(c, "tools/rete_fumus.sh");
    d->causa = _ch(c, "y");
    d->manu = VERUM;
    d->binaria = binaria;
    *(chorda*)xar_addere(intecta) = _ch(c, "docs/x.md");
    CREDO_VERUM(chorda_aequalis_literis(portae_debita_scribere(debita,
        intecta, NIHIL, III, VERUM, c->piscina),
        "DEBITUM\tcredo\t0\tx.c (a\\tb)\n"
        "BINARIUM\tcredo\tbin/fabrica\tcurrit bin/fabrica\n"
        "DEBITUM\ttools/rete_fumus.sh\t1\ty\n"
        "INTECTUM\tdocs/x.md\n"));
    CREDO_VERUM(chorda_aequalis_literis(portae_debita_scribere(debita,
        intecta, NIHIL, III, FALSUM, c->piscina),
        "PORTAE DEBITAE (3 viae):\n"
        "  credo\n"
        "      x.c (a\tb)\n"
        "MANU (homo currit):\n"
        "  tools/rete_fumus.sh\n"
        "      y\n"
        "INTECTA (nulla porta):\n"
        "  docs/x.md\n"));
    xar_vacare(debita);
    CREDO_VERUM(chorda_aequalis_literis(portae_debita_scribere(debita,
        NIHIL, NIHIL, I, FALSUM, c->piscina),
        "PORTAE DEBITAE (1 via):\n  (nulla)\n"));
    /* declaratae: PORTA, TEGIT, CLAUSURA */
    t = portae_declaratas_scribere(_derivare_mundum(c,
        _plagulae_debitorum, _paria_debitorum, _effectus_debitorum,
        _litterae_debitorum), NIHIL, VERUM, c->piscina);
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "PORTA\ttessera\t0\ttessera/compile_probationes.sh\t1\t\n")));
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "TEGIT\tcredo\tvia\ttools/credo_convertere.py\texsecutio in"
        " tools/credo_fumus.sh\n")));
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "TEGIT\ttessera\tglobus\ttessera/fixa/*\ttegit"
        " (tessera/compile_probationes.sh:1)\n")));
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "CLAUSURA\tcredo\tlib/credo.c\tlectio (clang) in"
        " tools/credo_fumus.sh\n")));
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "PORTA\ttools/rete_fumus.sh\t1\ttools/rete_fumus.sh\t1\trete"
        " poscit\n")));
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "PORTA\taeq\t1\tsilva/aeq.sh\t1\tbasis manu condita\n")));
    /* recusationes structae: locus et causa separati */
    _suturam_parare(c, &mundus, &sutura, plagulae, paria);
    recusationes = NIHIL;
    CREDO_VERUM(portae_declaratas_legere(&sutura, c->piscina, &causa,
        &recusationes) == NIHIL);
    CREDO_NECESSE_NON_NIHIL(recusationes);
    CREDO_AEQUALIS_I32(xar_numerus(recusationes), III);
    CREDO_VERUM(chorda_aequalis_literis(portae_debita_scribere(NIHIL,
        NIHIL, recusationes, ZEPHYRUM, VERUM, c->piscina),
        "RECUSATIO\ttools/credo_fumus.sh\t2\tattributum ignotum"
        " 'tegti' in <porta> (titulus manu causa tegit currit"
        " instrumentum genus)\n"
        "RECUSATIO\t\t0\tporta 'credo' (cursor tools/credo_fumus.sh)"
        " sine annotatione <porta titulus=\"...\"/>\n"
        "RECUSATIO\t\t0\tporta 'sera' (cursor tools/sera_fumus.sh)"
        " sine annotatione <porta titulus=\"...\"/>\n"));
}

/* XII. possessio instrumentorum (instrumentum="bin/x"): porta quae
 * instrumentum alienum currit fontes eius NON hereditat (nota
 * nominat possessorem); possessor ipse hereditat; 'currit' expressum
 * semper valet; possessio duplex recusatur */
interior vacuum
_sectio_possessio (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/aedilis_porta.sh",
          "# <porta titulus=\"aedilis\""
          " instrumentum=\"bin/acies\"/>\n" },
        { "tools/vates_fumus.sh", "# <porta titulus=\"vates\"/>\n" },
        { "tools/credo_fumus.sh",
          "# <porta titulus=\"credo\" currit=\"bin/acies\"/>\n" },
        { "tools/acies.c", "" },
        { "tools/folium.c", "" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "aedilis=tools/aedilis_porta.sh", "vates=tools/vates_fumus.sh",
        "credo=tools/credo_fumus.sh", NIHIL
    };
    hic_manens constans PlagulaFicta effectus[] = {
        { "tools/aedilis_porta.sh",
          "<effectus><processus radix=\"tools/aedilis_porta.sh\">"
          "<exsecutio via=\"bin/acies\" forma=\"via\""
          " resolutio=\"plena\""
          " classis=\"instrumentum_domus\""
          " plagula=\"tools/aedilis_porta.sh\"/>"
          "</processus></effectus>" },
        { "tools/vates_fumus.sh",
          "<effectus><processus radix=\"tools/vates_fumus.sh\">"
          "<exsecutio via=\"bin/acies\" forma=\"via\""
          " resolutio=\"plena\""
          " classis=\"instrumentum_domus\""
          " plagula=\"tools/vates_fumus.sh\"/>"
          "<exsecutio via=\"bin/folium\" forma=\"via\""
          " resolutio=\"plena\" classis=\"instrumentum_domus\""
          " plagula=\"tools/vates_fumus.sh\"/>"
          "</processus></effectus>" },
        { "tools/credo_fumus.sh", "<effectus/>" },
        { NIHIL, NIHIL }
    };
    hic_manens constans PlagulaFicta bina[] = {
        { "tools/aedilis_porta.sh",
          "# <porta titulus=\"aedilis\""
          " instrumentum=\"bin/acies\"/>\n" },
        { "tools/vates_fumus.sh",
          "# <porta titulus=\"vates\" instrumentum=\"bin/acies\"/>\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria_bina[] = {
        "aedilis=tools/aedilis_porta.sh", "vates=tools/vates_fumus.sh",
        NIHIL
    };
                         Xar* o;
                         Xar* d;
     constans PortaeOpertura* p;
                      chorda  causa;

    d = _legere_mundum(c, plagulae, paria, &causa);
    CREDO_NECESSE_NON_NIHIL(d);
    CREDO_AEQUALIS_I32(xar_numerus(_declaratio(d,
        "aedilis")->instrumenta),
        I);
    o = _derivare_mundum(c, plagulae, paria, effectus, NIHIL);
    CREDO_NECESSE_NON_NIHIL(o);
    /* possessor hereditat */
    p = _opertura(o, "aedilis");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "tools/acies.c"));
    /* alienus non hereditat; nota possessorem nominat */
    p = _opertura(o, "vates");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_VERUM(_exemplar(p->fontes_c, "tools/acies.c") == NIHIL);
    CREDO_VERUM(_ignotum_habet(c, p->notae,
        "currit bin/acies: instrumentum portae 'aedilis'"));
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "tools/folium.c"));
    CREDO_AEQUALIS_I32(xar_numerus(p->ignota), ZEPHYRUM);
    /* binaria: alienus instrumentum possessum TAMEN currit -> recens
     * esse debet (circulus commissionis II T2) */
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/acies"));
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/folium"));
    CREDO_AEQUALIS_I32(xar_numerus(p->binaria), II);
    p = _opertura(o, "aedilis");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_AEQUALIS_I32(xar_numerus(p->binaria), I);   /* semel */
    /* 'currit' expressum valet */
    p = _opertura(o, "credo");
    CREDO_NECESSE_NON_NIHIL(p);
    CREDO_NON_NIHIL(_exemplar(p->fontes_c, "tools/acies.c"));
    CREDO_NON_NIHIL(_exemplar(p->binaria, "bin/acies"));
    CREDO_VERUM(chorda_continet(portae_declaratas_scribere(o, NIHIL,
        VERUM,
        c->piscina), _ch(c, "INSTRUMENTUM\taedilis\tbin/acies\n")));
    /* possessio bina */
    CREDO_VERUM(_legere_mundum(c, bina, paria_bina, &causa) == NIHIL);
    CREDO_VERUM(chorda_continet(causa, _ch(c,
        "tools/vates_fumus.sh:1: instrumentum 'bin/acies' bis possessum"
        " (prior: aedilis)")));
}

/* XIII. genus (circulus commissionis II T4): 'praeiudicium' acceptum et
 * per GENUS nominatum (machina et homo); 'probatio' expressum =
 * ordinarium (nihil scribitur); valor alius recusatur nomine */
interior vacuum
_sectio_genus (
    CredoContextus* c)
{
    hic_manens constans PlagulaFicta plagulae[] = {
        { "tools/vexilla_fumus.sh",
          "# <porta titulus=\"vexilla\" genus=\"praeiudicium\"/>\n" },
        { "tools/credo_fumus.sh",
          "# <porta titulus=\"credo\" genus=\"probatio\"/>\n" },
        { NIHIL, NIHIL }
    };
    hic_manens constans character* constans paria[] = {
        "vexilla=tools/vexilla_fumus.sh", "credo=tools/credo_fumus.sh",
        NIHIL
    };
    hic_manens constans character* const credo_solum[] = {
        "credo=tools/credo_fumus.sh", NIHIL
    };
                         Xar* o;
                      chorda  t;
                      chorda  h;
                      chorda  causa;
                         Xar* d;
     constans PortaDeclarata* v;

    _recusatio_probare(c,
        "# <porta titulus=\"credo\" genus=\"praeiudicum\"/>\n",
        credo_solum,
        "tools/credo_fumus.sh:1: genus ignotum 'praeiudicum'");
    d = _legere_mundum(c, plagulae, paria, &causa);
    CREDO_NECESSE_NON_NIHIL(d);
    v = _declaratio(d, "vexilla");
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32((i32)v->genus, (i32)PORTA_PRAEIUDICIUM);
    v = _declaratio(d, "credo");
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32((i32)v->genus, (i32)PORTA_PROBATIO);
    o = _derivare_mundum(c, plagulae, paria, NIHIL, NIHIL);
    CREDO_NECESSE_NON_NIHIL(o);
    t = portae_declaratas_scribere(o, NIHIL, VERUM, c->piscina);
    CREDO_VERUM(chorda_continet(t, _ch(c,
        "GENUS\tvexilla\tpraeiudicium\n")));
    CREDO_VERUM(!chorda_continet(t, _ch(c, "GENUS\tcredo")));
    h = portae_declaratas_scribere(o, NIHIL, FALSUM, c->piscina);
    CREDO_VERUM(chorda_continet(h, _ch(c, "    genus praeiudicium\n")));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lectio sana", _sectio_lectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "genus portae", _sectio_genus, NIHIL, NIHIL, NIHIL, NIHIL },
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
    { "debita", _sectio_debita, NIHIL, NIHIL, NIHIL, NIHIL },
    { "debita caeca", _sectio_debita_caeca, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "contractus machinae", _sectio_contractus, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "possessio instrumentorum", _sectio_possessio, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("portae", SECTIONES);
}
