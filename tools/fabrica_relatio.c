/* fabrica_relatio.c - bin/fabrica portae relatio: PORTAE PER UNITATES
 * (spec unitates-codicis par. VII semis, U5 a): quomodo portae genera
 * unitatum tractant - relatio, numquam porta.
 *
 * Iunctio trium: tactus OMNIUM viarum tractatarum (lib/portae.c, cum
 * generibus rationum typatis), unitates (lib/unitates.c: genus
 * unitatis, munus, cura, vita, provenientia), tempora portarum
 * (mediana ex build/portae/tempora.tsv, portae_ordinare). Portae
 * manu (homo currit) nihil tegunt hic.
 *
 * Sectiones:
 *   I   GENERA: per genus unitatis codex domesticus manu scriptus
 *       (fons caput scriptum probatio, non archivatus), quot tecti,
 *       mediana portarum et secundarum (commissio / fusio) per
 *       plagulam mutatam
 *   II  BIBLIOTHECAE: debita capitis per bibliothecam, fasciis;
 *       FUNDAMENTA = capita quae portas omnes (praeter V) debent
 *   III INTECTA: codex domesticus manu scriptus quem NULLA porta tegit,
 *       per unitatem
 *   IV  RATIONES: debita quae SOLUM per 'tegit' aut 'nominat' stant
 *       (textus), et quae SOLUM per 'currit' annotationis programmatis
 *       portae alienae (infrastructura) - per portam, cum secundis
 *
 * -machina: lineae TAB separatae, prima columna genus lineae:
 *   GENUS genus codex tecti mediana_portarum commissio_ms fusio_ms
 *   BIBLIOTHECA titulus portae commissio_ms fusio_ms
 *   INTECTUM unitas genus munus via
 *   RATIO porta textus|alienum debita ms_portae
 *   SUMMA viae tactus portae
 */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "portae.h"
#include "unitates.h"
#include "unitates_arboris.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* porta una: gradus, tempus */
nomen structura {
    chorda titulus;
       b32 fusio;
       s32 millesimae;   /* -I = sine historia (0 numeratur) */
       i32 textus;       /* debita textu solo */
       i32 alienum;      /* debita infrastructura sola */
} RelatioPorta;

/* via una */
nomen structura {
    chorda via;
       i32 portae;       /* debitae (caecae quoque) */
       i32 tegentes;     /* quae viam vere tegunt (non caeca sola) */
       s32 commissio_ms;
       s32 fusio_ms;
       b32 manu;         /* porta manu tantum */
} RelatioVia;

interior vacuum
_scribere (
    chorda c)
{
    (vacuum)fwrite(c.datum, I, (size_t)c.mensura, stdout);
}

interior s32
_s32_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    s32 p = *(constans s32*)a;
    s32 q = *(constans s32*)b;

    redde p < q ? -I : (p > q ? I : ZEPHYRUM);
}

interior s32
_mediana (
         Xar* numeri)
{
    si (xar_numerus(numeri) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    xar_ordinare(numeri, _s32_comparare);
    redde *(s32*)xar_obtinere(numeri, xar_numerus(numeri) / II);
}

/* genus unitatis: segmentum ultimum viae naturae */
interior chorda
_genus_breve (
    constans Unitas* x)
{
    i32 i = x->genus.mensura;

    dum (i > ZEPHYRUM && x->genus.datum[i - I] != '/')
    {
        i--;
    }
    redde chorda_sectio(x->genus, i, x->genus.mensura);
}

interior b32
_codex_domesticus (
    constans UnitatisResponsum* r)
{
    redde chorda_aequalis_literis(r->cura.valor, "domestica")
        && !chorda_aequalis_literis(r->vita.valor, "archivata")
        && chorda_aequalis_literis(r->provenientia.valor, "manu")
        && (   chorda_aequalis_literis(r->munus.valor, "fons")
            || chorda_aequalis_literis(r->munus.valor, "caput")
            || chorda_aequalis_literis(r->munus.valor, "scriptum")
            || chorda_aequalis_literis(r->munus.valor, "probatio"));
}

#define RATIO_BIT(g) ((i32)I << (i32)(g))

s32
suturae_portae_relatio (
    constans PortaeSutura* sutura,
             constans Xar* declaratae,
             constans Xar* operturae,
             constans Xar* viae,
                   chorda  tempora,
                      b32  machina,
                  Piscina* piscina)
{
    constans character* genera[] = {
        "bibliotheca", "subsystema", "applicatio", "opus_explorationis",
        "adscitio", NIHIL
    };
           UnitatesSutura* sutura_unitatum;
                 Unitates* u;
                   chorda  causa;
                      Xar* tactus;
                      Xar* nomina;
                      Xar* ordo;
                      Xar* portae;
                      Xar* res;
           TabulaDispersa* index_portarum;
           TabulaDispersa* index_viarum;
                      Xar* responsa;
                      i32  automaticae = ZEPHYRUM;
                      i32  i;
                      i32  j;

    /* portae: gradus ex declarationibus, tempora ex ordine */
    nomina = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(declaratae); i++)
    {
        constans PortaDeclarata* d = (constans PortaDeclarata*)
            xar_obtinere(declaratae, i);

        si (!d->manu && d->titulus.mensura > ZEPHYRUM)
        {
            *(chorda*)xar_addere(nomina) = d->titulus;
        }
    }
    ordo = portae_ordinare(declaratae, nomina, tempora, piscina);
    portae = xar_creare(piscina, (i32)magnitudo(RelatioPorta));
    index_portarum = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    per (i = ZEPHYRUM; ordo != NIHIL && i < xar_numerus(ordo); i++)
    {
        constans PortaeOrdo* o =
            (constans PortaeOrdo*)xar_obtinere(ordo,
            i);
              RelatioPorta* p = (RelatioPorta*)xar_addere(portae);

        p->titulus     = o->porta;
        p->fusio       = o->gradus == PORTA_FUSIO;
        p->millesimae  = o->millesimae;
        p->textus      = ZEPHYRUM;
        p->alienum     = ZEPHYRUM;
        automaticae++;
    }
    /* indices post additiones omnes (xar non movet: segmenta
     * stabilia) */
    per (i = ZEPHYRUM; i < xar_numerus(portae); i++)
    {
        RelatioPorta* p = (RelatioPorta*)xar_obtinere(portae, i);

        tabula_dispersa_inserere(index_portarum, p->titulus, p);
    }

    /* viae */
    res = xar_creare(piscina, (i32)magnitudo(RelatioVia));
    index_viarum = tabula_dispersa_creare_chorda(piscina, (i32)XVI * M);
    per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
    {
        RelatioVia* v = (RelatioVia*)xar_addere(res);

        v->via           = *(constans chorda*)xar_obtinere(viae, i);
        v->portae        = ZEPHYRUM;
        v->tegentes      = ZEPHYRUM;
        v->commissio_ms  = ZEPHYRUM;
        v->fusio_ms      = ZEPHYRUM;
        v->manu          = FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(res); i++)
    {
        RelatioVia* v = (RelatioVia*)xar_obtinere(res, i);

        tabula_dispersa_inserere(index_viarum, v->via, v);
    }

    tactus = portae_tactus_computare(sutura, operturae, viae, NIHIL,
        piscina);
    si (tactus == NIHIL)
    {
        fprintf(stderr, "fabrica portae relatio: tactus computari "
            "nequeunt\n");
        redde II;
    }
    per (i = ZEPHYRUM; i < xar_numerus(tactus); i++)
    {
        constans PortaeTactus* t = (constans PortaeTactus*)xar_obtinere(
            tactus, i);
                  RelatioVia* v = NIHIL;
                RelatioPorta* p = NIHIL;
                         i32  textus_soli =
                             RATIO_BIT(PORTAE_RATIO_TEGIT)
                             | RATIO_BIT(PORTAE_RATIO_NOMINAT);

        si (!tabula_dispersa_invenire(index_viarum, t->via,
                (vacuum**)&v))
        {
            perge;
        }
        si (t->manu)
        {
            v->manu = VERUM;
            perge;
        }
        si (!tabula_dispersa_invenire(index_portarum, t->porta,
                (vacuum**)&p))
        {
            perge;
        }
        v->portae++;
        /* caeca (ignota) debetur, non tegit (lib/portae.c) */
        si (   (t->directa & ~RATIO_BIT(PORTAE_RATIO_IGNOTA))
            != ZEPHYRUM
            || t->clausurae != ZEPHYRUM)
        {
            v->tegentes++;
        }
        si (p->fusio)
        {
            v->fusio_ms += p->millesimae > ZEPHYRUM ? p->millesimae
                : ZEPHYRUM;
        }
        alioquin
        {
            v->commissio_ms += p->millesimae > ZEPHYRUM ? p->millesimae
                : ZEPHYRUM;
        }
        si (   t->clausurae == ZEPHYRUM && t->directa != ZEPHYRUM
            && (t->directa & ~textus_soli) == ZEPHYRUM)
        {
            p->textus++;
        }
        si (   t->directa   == ZEPHYRUM
            && t->clausurae == RATIO_BIT(PORTAE_RATIO_CURRIT_ALIENUM))
        {
            p->alienum++;
        }
    }

    /* unitates */
    sutura_unitatum = unitates_sutura_arboris(NIHIL, piscina);
    u = unitates_legere(sutura_unitatum, piscina, &causa, NIHIL);
    si (u == NIHIL)
    {
        fputs("fabrica portae relatio: RECUSATIO unitatum:\n", stderr);
        _scribere(causa);
        fputc('\n', stderr);
        redde II;
    }
    responsa = xar_creare(piscina, (i32)magnitudo(UnitatisResponsum));
    per (i = ZEPHYRUM; i < xar_numerus(res); i++)
    {
        constans RelatioVia* v = (constans RelatioVia*)xar_obtinere(res,
            i);
        UnitatisResponsum* r = (UnitatisResponsum*)xar_addere(responsa);

        memset(r, ZEPHYRUM, magnitudo(*r));
        (vacuum)unitates_quid(u, v->via, piscina, r);
    }

    /* I GENERA */
    si (!machina)
    {
        puts("I   GENERA - codex domesticus manu scriptus (fons caput "
            "scriptum probatio, non archivatus); per plagulam mutatam "
            "mediana:");
        printf("    %-20s %6s %6s %5s  %7s %10s %10s\n", "genus",
            "codex",
            "tecti", "%", "portae", "commissio", "fusio");
    }
    per (j = ZEPHYRUM; genera[j] != NIHIL; j++)
    {
        Xar* n             = xar_creare(piscina, (i32)magnitudo(s32));
        Xar* commissio_tempora  = xar_creare(piscina,
            (i32)magnitudo(s32));
        Xar* fusio_tempora  = xar_creare(piscina, (i32)magnitudo(s32));
        i32  codex          = ZEPHYRUM;
        i32  tecti          = ZEPHYRUM;

        per (i = ZEPHYRUM; i < xar_numerus(res); i++)
        {
            constans RelatioVia* v = (constans RelatioVia*)xar_obtinere(
                res, i);
            constans UnitatisResponsum* r =
                (constans UnitatisResponsum*)
                xar_obtinere(responsa, i);

            si (   r->unitas == NIHIL
                || !chorda_aequalis_literis(_genus_breve(r->unitas),
                    genera[j])
                || !_codex_domesticus(r))
            {
                perge;
            }
            codex++;
            si (v->tegentes == ZEPHYRUM)
            {
                perge;
            }
            tecti++;
            *(s32*)xar_addere(n)                  = (s32)v->portae;
            *(s32*)xar_addere(commissio_tempora)  = v->commissio_ms;
            *(s32*)xar_addere(fusio_tempora)      = v->fusio_ms;
        }
        si (machina)
        {
            printf("GENUS\t%s\t%ld\t%ld\t%ld\t%ld\t%ld\n", genera[j],
                (longus)codex, (longus)tecti, (longus)_mediana(n),
                (longus)_mediana(commissio_tempora),
                (longus)_mediana(fusio_tempora));
            perge;
        }
        printf("    %-20s %6ld %6ld %4ld%%  %7ld %9lds %9lds\n",
            genera[j], (longus)codex, (longus)tecti,
            codex > ZEPHYRUM ? (longus)(C * tecti / codex) : 0L,
            (longus)_mediana(n), (longus)(_mediana(commissio_tempora)
                / M),
            (longus)(_mediana(fusio_tempora) / M));
    }

    /* II BIBLIOTHECAE: debita capitis (include/S.h) per bibliothecam */
    {
        i32 limen = automaticae > V ? automaticae - V : automaticae;
        i32 fundamenta = ZEPHYRUM;
        i32 f[IV];

        f[0] = f[1] = f[2] = f[3] = ZEPHYRUM;
        si (!machina)
        {
            printf("\nII  BIBLIOTHECAE - debita capitis mutati "
                "(fasciae); FUNDAMENTA = %ld+ ex %ld portis"
                " automaticis:\n",
                (longus)limen, (longus)automaticae);
        }
        per (i = ZEPHYRUM; i < xar_numerus(res); i++)
        {
            constans RelatioVia* v = (constans RelatioVia*)xar_obtinere(
                res, i);
            constans UnitatisResponsum* r =
                (constans UnitatisResponsum*)
                xar_obtinere(responsa, i);

            si (   r->unitas == NIHIL
                || r->unitas->fons != UNITAS_BIBLIOTHECAE
                || !chorda_aequalis_literis(r->munus.valor, "caput")
                || v->via.mensura < VIII
                || memcmp(v->via.datum, "include/", VIII) != ZEPHYRUM)
            {
                perge;
            }
            f[v->portae <= III ? 0 : v->portae <= X ? 1
                : v->portae < limen ? 2 : 3]++;
            si (machina)
            {
                fputs("BIBLIOTHECA\t", stdout);
                _scribere(r->unitas->titulus);
                printf("\t%ld\t%ld\t%ld\n", (longus)v->portae,
                    (longus)v->commissio_ms, (longus)v->fusio_ms);
            }
            alioquin si (v->portae >= limen)
            {
                si (fundamenta == ZEPHYRUM)
                {
                    printf("    fundamenta (%ld portae, commissio %lds,"
                        " fusio %lds):", (longus)v->portae,
                        (longus)(v->commissio_ms / M),
                        (longus)(v->fusio_ms / M));
                }
                putchar(' ');
                _scribere(r->unitas->titulus);
                fundamenta++;
            }
        }
        si (!machina)
        {
            printf("%s    capita: 0-3 portae %ld, 4-10 %ld, 11-%ld %ld,"
                " fundamenta %ld\n", fundamenta > ZEPHYRUM ? "\n" : "",
                (longus)f[0], (longus)f[1], (longus)(limen - I),
                (longus)f[2], (longus)f[3]);
        }
    }

    /* III INTECTA */
    {
        i32 n = ZEPHYRUM;

        si (!machina)
        {
            puts("\nIII INTECTA - codex domesticus manu scriptus in"
                " nulla porta automatica:");
        }
        per (i = ZEPHYRUM; i < xar_numerus(res); i++)
        {
            constans RelatioVia* v = (constans RelatioVia*)xar_obtinere(
                res, i);
            constans UnitatisResponsum* r =
                (constans UnitatisResponsum*)
                xar_obtinere(responsa, i);

            si (   r->unitas == NIHIL || v->tegentes > ZEPHYRUM
                || !_codex_domesticus(r))
            {
                perge;
            }
            n++;
            si (machina)
            {
                fputs("INTECTUM\t", stdout);
                _scribere(r->unitas->titulus);
                putchar('\t');
                _scribere(_genus_breve(r->unitas));
                putchar('\t');
                _scribere(r->munus.valor);
                putchar('\t');
                _scribere(v->via);
                putchar('\n');
                perge;
            }
            printf("    %-14.*s %-11.*s %-9.*s ",
                (integer)r->unitas->titulus.mensura,
                (constans character*)r->unitas->titulus.datum,
                (integer)_genus_breve(r->unitas).mensura,
                (constans character*)_genus_breve(r->unitas).datum,
                (integer)r->munus.valor.mensura,
                (constans character*)r->munus.valor.datum);
            _scribere(v->via);
            puts(v->manu ? "  (porta manu solum)" : "");
        }
        si (!machina)
        {
            printf("    summa: %ld\n", (longus)n);
        }
    }

    /* IV RATIONES */
    si (!machina)
    {
        puts("\nIV  RATIONES - debita quae solum textu (tegit/nominat)"
            " aut solum infrastructura (currit portae alienae) stant:");
        printf("    %-22s %8s %8s %8s\n", "porta", "textus", "alienum",
            "s/cursus");
    }
    per (i = ZEPHYRUM; i < xar_numerus(portae); i++)
    {
        constans RelatioPorta* p = (constans RelatioPorta*)xar_obtinere(
            portae, i);

        si (p->textus == ZEPHYRUM && p->alienum == ZEPHYRUM)
        {
            perge;
        }
        si (machina)
        {
            fputs("RATIO\t", stdout);
            _scribere(p->titulus);
            printf("\ttextus\t%ld\t%ld\n", (longus)p->textus,
                (longus)p->millesimae);
            fputs("RATIO\t", stdout);
            _scribere(p->titulus);
            printf("\talienum\t%ld\t%ld\n", (longus)p->alienum,
                (longus)p->millesimae);
            perge;
        }
        printf("    %-22.*s %8ld %8ld %7lds%s\n",
            (integer)p->titulus.mensura,
            (constans character*)p->titulus.datum, (longus)p->textus,
            (longus)p->alienum,
            (longus)(p->millesimae > ZEPHYRUM ? p->millesimae / M : 0),
            p->fusio ? "  [fusio]" : "");
    }
    printf(machina ? "SUMMA\t%ld\t%ld\t%ld\n"
        : "\nsumma: %ld viae tractatae, %ld tactus, %ld portae "
          "automaticae\n", (longus)xar_numerus(viae),
        (longus)xar_numerus(tactus), (longus)automaticae);
    redde ZEPHYRUM;
}
