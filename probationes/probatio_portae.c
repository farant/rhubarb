/* probatio_portae.c - PORTAE DECLARATAE (plan portae-declaratae T1):
 * lector declarationum <porta> in capitibus cursorum per suturam
 * fictam (plagulae in memoria, registrum PORTAE fictum).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "credo.h"
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

/* registrum: "clavis=cursor" ... NIHIL */
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

        p->clavis          = chorda_ex_literis(paria[i], piscina);
        p->clavis.mensura  = (i32)(aeq - paria[i]);
        p->cursor          = chorda_ex_literis(aeq + 1, piscina);
    }
    redde r;
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

    mundus.plagulae            = plagulae;
    mundus.tabula_portarum     = _registrum(c->piscina, paria);
    sutura.datum               = &mundus;
    sutura.plagulas_enumerare  = _enumerare;
    sutura.plagulam_legere     = _legere;
    sutura.tabula_portarum     = mundus.tabula_portarum;
    *causa                     = chorda_ex_literis("", c->piscina);
    redde portae_declaratas_legere(&sutura, c->piscina, causa);
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("portae", SECTIONES);
}
