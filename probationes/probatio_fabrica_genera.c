/* probatio_fabrica_genera.c - probationes fabricae (fabrica-6 H3),
 * pars genera: sigillum copiae, manifestum, directoria,
 * plagulae, manifesta, radices, provenientia, actio tacta, genera et
 * strategiae, instrumentum domus, effectus, axes duo, chassis
 * (fixa conformitatis)
 *
 * Mundus fictus (discus in memoria, sutura ficta):
 * probationes/fabrica_mundus_fictus.h. Spec:
 * project-specs/fabrica-spec-v2.md et sequentes. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "internamentum.h"
#include "fabrica.h"
#include "credo.h"
#include "fabrica_mundus_fictus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * CHASSIS (fabrica-6 T1): fixum conformitatis per genus ingressus.
 * Mundus fictus, deinde (a) mutatio classis ingressus sigillum mutat
 * et particulam nominat, (b) mutatio aliena sigillum servat. Genus
 * registratum sine fixo = FRACTUM (probatio per registrum enumerat,
 * non per indicem manu scriptum).
 * ================================================== */

nomen structura {
    constans character* titulus;
    vacuum (*mundus)(DiscusFictus* discus);
    constans character* via;
    constans character* suffixa;
    vacuum (*mutare)(DiscusFictus* discus);
    constans character* particula;      /* via particulae mutatae */
    vacuum (*alienum)(DiscusFictus* discus);
} FixumConformitatis;

/* mundi fixorum (DiscusFictus + ficta globalia identitatis/effectus) */
interior vacuum
_fx_plagula (
    DiscusFictus* d)
{
    mundi_ponere(d, "data/f.txt", "f I\n");
    mundi_ponere(d, "data/alia.txt", "a I\n");
}

interior vacuum
_fx_plagula_mutata (
    DiscusFictus* d)
{
    mundi_ponere(d, "data/f.txt", "f II\n");
}

interior vacuum
_fx_alia_mutata (
    DiscusFictus* d)
{
    mundi_ponere(d, "data/alia.txt", "a II\n");
}

interior vacuum
_fx_directorium (
    DiscusFictus* d)
{
    constans character* nomina[I];

    nomina[0] = "f.txt";
    mundi_directorium_ponere(d, "data", nomina, I);
    mundi_ponere(d, "data/f.txt", "f I\n");
}

interior vacuum
_fx_directorium_auctum (
    DiscusFictus* d)
{
    constans character* nomina[II];

    nomina[0] = "f.txt";
    nomina[1] = "g.txt";
    mundi_directorium_ponere(d, "data", nomina, II);
    mundi_ponere(d, "data/g.txt", "g I\n");
}

interior vacuum
_fx_plagulae (
    DiscusFictus* d)
{
    constans character* nomina[II];

    nomina[0] = "a.c";
    nomina[1] = "n.txt";
    mundi_directorium_ponere(d, "src", nomina, II);
    mundi_ponere(d, "src/a.c", "int a;\n");
    mundi_ponere(d, "src/n.txt", "nota I\n");
}

interior vacuum
_fx_plagulae_mutatae (
    DiscusFictus* d)
{
    mundi_ponere(d, "src/a.c", "int a2;\n");
}

interior vacuum
_fx_plagulae_alienae (
    DiscusFictus* d)
{
    mundi_ponere(d, "src/n.txt", "nota II\n");
}

interior vacuum
_fx_manifestum (
    DiscusFictus* d)
{
    mundi_ponere(d, "build/m.stml",
        "<aedilis-manifestum scopus=\"tools/a.c\">\n"
        "  <obiecta><obiectum via=\"lib/x.c\"/></obiecta>\n"
        "</aedilis-manifestum>\n");
    mundi_ponere(d, "tools/a.c", "int a;\n");
    mundi_ponere(d, "lib/x.c", "int x;\n");
    mundi_ponere(d, "lib/z.c", "int z;\n");
}

interior vacuum
_fx_x_mutatum (
    DiscusFictus* d)
{
    mundi_ponere(d, "lib/x.c", "int x2;\n");
}

interior vacuum
_fx_z_mutatum (
    DiscusFictus* d)
{
    mundi_ponere(d, "lib/z.c", "int z2;\n");
}

interior vacuum
_fx_manifesta (
    DiscusFictus* d)
{
    constans character* nomina[I];

    nomina[0] = "a.stml";
    mundi_directorium_ponere(d, "build/cl", nomina, I);
    mundi_ponere(d, "build/cl/a.stml",
        "<aedilis-manifestum scopus=\"tools/a.c\">\n"
        "  <obiecta><obiectum via=\"lib/x.c\"/></obiecta>\n"
        "</aedilis-manifestum>\n");
    mundi_ponere(d, "tools/a.c", "int a;\n");
    mundi_ponere(d, "lib/x.c", "int x;\n");
    mundi_ponere(d, "lib/z.c", "int z;\n");
}

interior vacuum
_fx_radices (
    DiscusFictus* d)
{
    constans character* nomina[I];

    mundi_ponere(d, "aedilis.stml",
        "<aedilis>\n  <inclusa>\n    <via (>include\n  </inclusa>\n"
        "</aedilis>\n");
    nomina[0] = "a.h";
    mundi_directorium_ponere(d, "include", nomina, I);
    mundi_ponere(d, "include/a.h", "int a;\n");
}

interior vacuum
_fx_radices_auctae (
    DiscusFictus* d)
{
    constans character* nomina[II];

    nomina[0] = "a.h";
    nomina[1] = "b.h";
    mundi_directorium_ponere(d, "include", nomina, II);
    mundi_ponere(d, "include/b.h", "int b;\n");
}

interior vacuum
_fx_caput_mutatum (
    DiscusFictus* d)
{
    mundi_ponere(d, "include/a.h", "int a2;\n");
}

interior vacuum
_fx_identitas (
    DiscusFictus* d)
{
    mundi_identitas_ficta = "clang I";
    _fx_plagula(d);
}

interior vacuum
_fx_identitas_mutata (
    DiscusFictus* d)
{
    (vacuum)d;
    mundi_identitas_ficta = "clang II";
}

interior vacuum
_fx_domus (
    DiscusFictus* d)
{
    d->relatio = "provenientia 1\nartificium bin/x\ningressus aaaa\n"
        "commissum c1\n";
}

interior vacuum
_fx_domus_mutata (
    DiscusFictus* d)
{
    d->relatio = "provenientia 1\nartificium bin/x\ningressus bbbb\n"
        "commissum c1\n";
}

interior vacuum
_fx_domus_commissum (
    DiscusFictus* d)
{
    d->relatio = "provenientia 1\nartificium bin/x\ningressus aaaa\n"
        "commissum c2\n";
}

interior vacuum
_fx_effectus (
    DiscusFictus* d)
{
    mundi_effectus_effusio = "octeti\tdata/f.txt\n";
    mundi_ponere(d, "porta.sh", "#!/bin/sh\ncat data/f.txt\n");
    _fx_plagula(d);
}

/* repositorium fictum (fabrica-6 T2): HEAD quem sutura.repositorium
 * reddit */
hic_manens constans character* _caput_fictum = "commissum I";

interior b32
_repositorium_fictum (
                vacuum* datum,
    constans character* clavis,
               Piscina* piscina,
                chorda* valor_out)
{
    (vacuum)datum;
    si (strcmp(clavis, "commissum") != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *valor_out = chorda_ex_literis(_caput_fictum, piscina);
    redde VERUM;
}

interior vacuum
_fx_repositorium (
    DiscusFictus* d)
{
    _caput_fictum = "commissum I";
    _fx_plagula(d);
}

interior vacuum
_fx_repositorium_mutatum (
    DiscusFictus* d)
{
    (vacuum)d;
    _caput_fictum = "commissum II";
}

/* titulus, mundus, via, suffixa, mutatio classis, particula, aliena */
hic_manens constans FixumConformitatis _fixa_conformitatis[] = {
    { "fasciculus", _fx_plagula, "data/f.txt", NIHIL,
      _fx_plagula_mutata, "data/f.txt", _fx_alia_mutata },
    { "configuratio", _fx_plagula, "data/f.txt", NIHIL,
      _fx_plagula_mutata, "data/f.txt", _fx_alia_mutata },
    { "instrumentum", _fx_plagula, "data/f.txt", NIHIL,
      _fx_plagula_mutata, "data/f.txt", _fx_alia_mutata },
    { "binarium", _fx_plagula, "data/f.txt", NIHIL,
      _fx_plagula_mutata, "data/f.txt", _fx_alia_mutata },
    { "directorium", _fx_directorium, "data", NIHIL,
      _fx_directorium_auctum, "data/", _fx_plagula_mutata },
    { "plagulae", _fx_plagulae, "src", ".c",
      _fx_plagulae_mutatae, "src/a.c", _fx_plagulae_alienae },
    { "manifestum", _fx_manifestum, "build/m.stml", NIHIL,
      _fx_x_mutatum, "lib/x.c", _fx_z_mutatum },
    { "manifesta", _fx_manifesta, "build/cl", NIHIL,
      _fx_x_mutatum, "lib/x.c", _fx_z_mutatum },
    { "radices", _fx_radices, "aedilis.stml", NIHIL,
      _fx_radices_auctae, "include/", _fx_caput_mutatum },
    { "identitas_clang", _fx_identitas, "clang", NIHIL,
      _fx_identitas_mutata, "identitas:clang", _fx_alia_mutata },
    { "instrumentum_domus", _fx_domus, "bin/x", NIHIL,
      _fx_domus_mutata, "bin/x", _fx_domus_commissum },
    { "effectus", _fx_effectus, "porta.sh", NIHIL,
      _fx_plagula_mutata, "data/f.txt", _fx_alia_mutata },
    { "repositorium", _fx_repositorium, ".", NIHIL,
      _fx_repositorium_mutatum, "repositorium:commissum",
          _fx_alia_mutata },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

interior constans FixumConformitatis*
_fixum_invenire (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; _fixa_conformitatis[i].titulus != NIHIL; i++)
    {
        si (strcmp(_fixa_conformitatis[i].titulus, titulus) == ZEPHYRUM)
        {
            redde &_fixa_conformitatis[i];
        }
    }
    redde NIHIL;
}

/* particulae mundi post 'mutatio' (NIHIL = nulla) */
interior Xar*
_particulas_fixi (
    constans FixumConformitatis* fixum,
          constans FabricaGenus* genus,
                           vacuum (*mutatio)(DiscusFictus* discus),
                        Piscina* piscina)
{
         DiscusFictus  discus;
        FabricaSutura  sutura;
     FabricaIngressus  ingressus;
                  Xar* particulae;
               chorda  causa;

    mundi_discum_parare(&discus, &sutura, piscina);
    sutura.identitas     = mundi_identitatem_fictam_dare;
    sutura.effectus      = mundi_effectus_ficti;
    sutura.repositorium  = _repositorium_fictum;
    fixum->mundus(&discus);
    si (mutatio != NIHIL)
    {
        mutatio(&discus);
    }
    ingressus.genus  = genus;
    ingressus.via    = chorda_ex_literis(fixum->via, piscina);
    ingressus.suffixa  = chorda_ex_literis(fixum->suffixa != NIHIL
        ? fixum->suffixa : "", piscina);
    particulae  = xar_creare(piscina, (i32)magnitudo(FabricaParticula));
    causa       = chorda_ex_literis("", piscina);
    si (!genus->sigillare(&sutura, &ingressus, NIHIL, piscina,
        particulae,
            &causa))
    {
        imprimere("  sigillare %s: %.*s\n", genus->titulus,
            (integer)causa.mensura, (constans character*)causa.datum);
        redde NIHIL;
    }
    redde particulae;
}

/* viae particularum quae inter a et b differunt (octetis aut praesentia) */
interior b32
_particula_differt (
          constans Xar* a,
          constans Xar* b,
    constans character* via,
               Piscina* piscina)
{
         i32 i;
         i32 j;
         b32 in_a = FALSUM;
         b32 in_b = FALSUM;
    Sigillum sa;
    Sigillum sb;

    (vacuum)piscina;
    per (i = ZEPHYRUM; i < xar_numerus(a); i++)
    {
        constans FabricaParticula* p = (constans FabricaParticula*)
            xar_obtinere(a, i);

        si (chorda_aequalis_literis(p->via, via))
        {
            in_a  = VERUM;
            sa    = p->octeti;
        }
    }
    per (j = ZEPHYRUM; j < xar_numerus(b); j++)
    {
        constans FabricaParticula* p = (constans FabricaParticula*)
            xar_obtinere(b, j);

        si (chorda_aequalis_literis(p->via, via))
        {
            in_b  = VERUM;
            sb    = p->octeti;
        }
    }
    si (in_a != in_b)
    {
        redde VERUM;
    }
    redde in_a && memcmp(&sa, &sb, magnitudo(Sigillum)) != ZEPHYRUM;
}

interior b32
_particulae_aequales (
    constans Xar* a,
    constans Xar* b)
{
    i32 i;

    si (xar_numerus(a) != xar_numerus(b))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(a); i++)
    {
        constans FabricaParticula* p = (constans FabricaParticula*)
            xar_obtinere(a, i);
        constans FabricaParticula* q = (constans FabricaParticula*)
            xar_obtinere(b, i);

        si (   !chorda_aequalis(p->via, q->via)
            || memcmp(&p->octeti, &q->octeti, magnitudo(Sigillum))
               != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum(
        "probatio_fabrica_genera", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== */

    /* PROBARE: sigillum copiae - ordo et contentum        */


    /* ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
              FabricaActio* a;
              FabricaActio* b;
                  Sigillum  sa;
                  Sigillum  sb;

        imprimere("\n--- Probans sigillum copiae ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "lib/x.c", "int x;\n");
        mundi_ponere(&discus, "lib/y.c", "int y;\n");

        a = mundi_actio(piscina, "a", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        mundi_ingressum_addere(a, "fasciculus", "lib/y.c",
            piscina);
        b = mundi_actio(piscina, "b", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(b, "fasciculus", "lib/y.c",
            piscina);
        mundi_ingressum_addere(b, "fasciculus", "lib/x.c",
            piscina);

        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &sa));
        CREDO_VERUM(mundi_sigillum(&sutura, b, piscina, &sb));
        /* ordo declarationis nihil refert */
        CREDO_VERUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* octetus unus mutatus -> sigillum aliud */
        mundi_ponere(&discus, "lib/x.c", "int z;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &sb));
        CREDO_FALSUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* contenta permutata inter vias -> sigillum aliud (via in
         * sigillo est, non solum contenta) */
        mundi_ponere(&discus, "lib/x.c", "int y;\n");
        mundi_ponere(&discus, "lib/y.c", "int x;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &sb));
        CREDO_FALSUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* via renominata, contentum idem -> sigillum aliud (via IPSA
         * in sigillo est; permutatio supra id non probat - ordo
         * sigillorum contentorum solus eam mutat) */
        {
             FabricaActio* r1;
             FabricaActio* r2;
                 Sigillum  s1;
                 Sigillum  s2;

            mundi_ponere(&discus, "lib/prima.c", "idem\n");
            mundi_ponere(&discus, "lib/secunda.c", "idem\n");
            r1 = mundi_actio(piscina, "r1", FABRICA_ACTIO_GENERATOR);
            mundi_ingressum_addere(r1, "fasciculus",
                "lib/prima.c", piscina);
            r2 = mundi_actio(piscina, "r2", FABRICA_ACTIO_GENERATOR);
            mundi_ingressum_addere(r2, "fasciculus",
                "lib/secunda.c", piscina);
            CREDO_VERUM(mundi_sigillum(&sutura, r1, piscina, &s1));
            CREDO_VERUM(mundi_sigillum(&sutura, r2, piscina, &s2));
            CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
                == 0);
        }

        /* ingressus absens -> FALSUM, via nominata */
        {
            FabricaActio* c;
                  chorda  causa;

            c = mundi_actio(piscina, "c", FABRICA_ACTIO_GENERATOR);
            mundi_ingressum_addere(c, "fasciculus",
                "lib/nusquam.c", piscina);
            causa.datum    = NIHIL;
            causa.mensura  = ZEPHYRUM;
            CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, c, NIHIL,
                piscina, &sb, &causa));
            CREDO_VERUM(mundi_continet(causa, "lib/nusquam.c",
                piscina));
        }
    }


    /* ================================================== */

    /* PROBARE: manifestum aedilis                         */


    /* ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
                    chorda  parvum;
                    chorda  inresolutum;
                    chorda  causa;
                       Xar* viae;
                       Xar* inresolutae;
              FabricaActio* a;
                  Sigillum  s;

        imprimere("\n--- Probans manifestum ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        parvum = filum_legere_totum(
            "probationes/fixa/fabrica/manifestum_parvum.stml",
            piscina);
        inresolutum = filum_legere_totum(
            "probationes/fixa/fabrica/manifestum_inresolutum.stml",
            piscina);
        CREDO_VERUM(parvum.mensura > 0);
        CREDO_VERUM(inresolutum.mensura > 0);

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_VERUM(fabrica_manifestum_legere(parvum, piscina, &viae,
            &inresolutae, &causa));
        /* scopus I + obiecta II + capita II + vendor I; systemata NON.
         * SCOPUS (fons principalis) inter viae: manifestum eum in
         * attributo solo nominat - sine eo mutatio fontis principalis
         * binarium stalum non faceret (T7: canon_examen et
         * canon_coquere digestum IDEM ferebant) */
        CREDO_AEQUALIS_I32(xar_numerus(viae), VI);
        {
            i32 k;
            b32 scopus_inventus;

            scopus_inventus = FALSUM;
            per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
            {
                si (chorda_aequalis_literis(
                        *(chorda*)xar_obtinere(viae, k),
                        "tools/parvum.c"))
                {
                    scopus_inventus = VERUM;
                }
            }
            CREDO_VERUM(scopus_inventus);
        }
        CREDO_AEQUALIS_I32(xar_numerus(inresolutae), ZEPHYRUM);
        {
            i32 i;
            b32 systema_inventum;

            systema_inventum = FALSUM;
            per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
            {
                si (chorda_aequalis_literis(
                        *(chorda*)xar_obtinere(viae, i), "stdio.h"))
                {
                    systema_inventum = VERUM;
                }
            }
            CREDO_FALSUM(systema_inventum);
        }

        CREDO_VERUM(fabrica_manifestum_legere(inresolutum, piscina,
            &viae, &inresolutae, &causa));
        CREDO_AEQUALIS_I32(xar_numerus(inresolutae), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(inresolutae, ZEPHYRUM), "nusquam.h");

        /* ingressus MANIFESTUM: viae eius sigillantur */
        mundi_ponere(&discus, "build/m.stml", chorda_ut_cstr(parvum,
            piscina));
        mundi_ponere(&discus, "lib/alpha.c", "a\n");
        mundi_ponere(&discus, "lib/beta.c", "b\n");
        mundi_ponere(&discus, "include/alpha.h", "ah\n");
        mundi_ponere(&discus, "include/beta.h", "bh\n");
        mundi_ponere(&discus, "vendor/gamma.c", "g\n");
        mundi_ponere(&discus, "tools/parvum.c", "int principale;\n");
        a = mundi_actio(piscina, "m", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(a, "manifestum",
            "build/m.stml", piscina);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s));

        /* manifestum incompletum -> FALSUM, inresoluta nominata */
        mundi_ponere(&discus, "build/m.stml",
            chorda_ut_cstr(inresolutum,
            piscina));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s, &causa));
        CREDO_VERUM(mundi_continet(causa, "nusquam.h", piscina));
    }


    /* ================================================== */

    /* PROBARE: directorium - nomen novum (Review Focus 2) */


    /* ================================================== */

    {
                    DiscusFictus  discus;
                   FabricaSutura  sutura;
                    FabricaActio* a;
                        Sigillum  s1;
                        Sigillum  s2;
              constans character* bina[II];
              constans character* terna[III];

        imprimere("\n--- Probans directorium ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        bina[0]   = "a.h";
        bina[1]   = "b.h";
        terna[0]  = "a.h";
        terna[1]  = "b.h";
        terna[2]  = "c.h";
        mundi_directorium_ponere(&discus, "include", bina, II);
        a = mundi_actio(piscina, "d", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "directorium", "include",
            piscina);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));

        /* caput novum in radice inclusa: resolutionem priorem
         * obumbrare potest -> sigillum aliud */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "include", terna, III);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
    }


    /* ==================================================
     * PROBARE: plagula provenientiae exclusa (T7, Review Focus 3)
     * ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
              FabricaActio* cum;
              FabricaActio* sine;
                  Sigillum  s1;
                  Sigillum  s2;
                    chorda  causa;

        imprimere("\n--- Probans provenientiam exclusam ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        causa = chorda_ex_literis("", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            fabrica_provenientia_via(chorda_ex_literis("manus",
            piscina),
                piscina),
            "build/fabrica/provenientia/manus.c");

        mundi_ponere(&discus, "lib/manus.c", "int manus;\n");
        mundi_ponere(&discus, "build/fabrica/provenientia/manus.c",
            "H1\n");
        cum = mundi_actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(cum, "fasciculus",
            "lib/manus.c", piscina);
        mundi_ingressum_addere(cum, "fasciculus",
            "build/fabrica/provenientia/manus.c", piscina);
        sine = mundi_actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(sine, "fasciculus",
            "lib/manus.c", piscina);

        /* plagula provenientiae nihil ad sigillum confert */
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s1, &causa));
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, sine, piscina,
            &s2, &causa));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* digestum novum in ea scriptum -> sigillum idem (aliter
         * binarium statim post institutionem stalum esset) */
        mundi_ponere(&discus, "build/fabrica/provenientia/manus.c",
            "H2\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s2, &causa));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* fons verus mutatus -> sigillum aliud */
        mundi_ponere(&discus, "lib/manus.c", "int manus_nova;\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s2, &causa));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);

        /* FAMILIA (1b T5, D2): actio 'canon' binaria duo, plagula
         * provenientiae per BINARIUM (canon_examen.c) - directorium
         * provenientiae totum excluditur, non solum TITULUS.c */
        mundi_ponere(&discus,
            "build/fabrica/provenientia/canon_examen.c",
            "H1\n");
        cum = mundi_actio(piscina, "canon", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(cum, "fasciculus", "lib/manus.c",
            piscina);
        mundi_ingressum_addere(cum, "fasciculus",
            "build/fabrica/provenientia/canon_examen.c", piscina);
        sine = mundi_actio(piscina, "canon", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(sine, "fasciculus", "lib/manus.c",
            piscina);
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s1, &causa));
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, sine, piscina,
            &s2, &causa));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
    }


    /* ==================================================
     * PROBARE: memoria sigillorum per cursum (celer < II s)
     * ================================================== */

    {
         DiscusFictus  discus;
        FabricaSutura  sutura;
         FabricaActio* a;
             Sigillum  s1;
             Sigillum  s2;
                  i32  lecturae;

        imprimere("\n--- Probans memoriam sigillorum ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "lib/x.c", "int x;\n");
        mundi_ponere(&discus, "lib/y.c", "int y;\n");
        a = mundi_actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        mundi_ingressum_addere(a, "fasciculus", "lib/y.c",
            piscina);
        sutura.sigilla = tabula_dispersa_creare_chorda(piscina, 16);

        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));
        lecturae = discus.lecturae;
        CREDO_AEQUALIS_I32(lecturae, II);
        /* iterum: nulla lectio, sigillum idem */
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_AEQUALIS_I32(discus.lecturae, lecturae);
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
        /* memoria PER CURSUM: mutatio intra cursum non videtur (id
         * consulto - iudicium unum, arbor una) */
        mundi_ponere(&discus, "lib/x.c", "int z;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
    }


    /* ==================================================
     * PROBARE: ingressus PLAGULAE (corpus infixum)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
        FabricaIngressus* ingressus;
                Sigillum  s1;
                Sigillum  s2;
      constans character* nomina[IV];
      constans character* nomina_plus[V];

        imprimere("\n--- Probans plagulas directorii ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        nomina[0] = "a.c";
        nomina[1] = "b.h";
        nomina[2] = "notae.md";
        nomina[3] = "sub.c";
        mundi_directorium_ponere(&discus, "src", nomina, IV);
        /* sub.c DIRECTORIUM est, suffixo congruens: praetermittitur */
        mundi_directorium_ponere(&discus, "src/sub.c", nomina,
            ZEPHYRUM);
        mundi_ponere(&discus, "src/a.c", "int a;\n");
        mundi_ponere(&discus, "src/b.h", "int b;\n");
        mundi_ponere(&discus, "src/notae.md", "notae\n");
        a = mundi_actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(a, "plagulae", "src",
            piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(a->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c .h", piscina);

        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));
        /* plagula suffixo non congruens: nihil confert */
        mundi_ponere(&discus, "src/notae.md", "notae mutatae\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
        /* contentum congruentis mutatum -> aliud */
        mundi_ponere(&discus, "src/b.h", "int b2;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
        /* plagula nova congruens -> aliud (rebake post lib/) */
        mundi_ponere(&discus, "src/b.h", "int b;\n");
        nomina_plus[0] = "a.c";
        nomina_plus[1] = "b.h";
        nomina_plus[2] = "d.c";
        nomina_plus[3] = "notae.md";
        nomina_plus[4] = "sub.c";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "src", nomina_plus, V);
        mundi_directorium_ponere(&discus, "src/sub.c", nomina,
            ZEPHYRUM);
        mundi_ponere(&discus, "src/d.c", "int d;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
    }


    /* ==================================================
     * PROBARE: ingressus MANIFESTA (directorium manifestorum)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
                Sigillum  s1;
                Sigillum  s2;
                  chorda  causa;
      constans character* nomina[III];
      constans character* nomina_minus[II];

        imprimere("\n--- Probans manifesta ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        nomina[0] = "a.stml";
        nomina[1] = "b.stml";
        nomina[2] = "notae.txt";
        mundi_directorium_ponere(&discus, "build/cl", nomina, III);
        mundi_ponere(&discus, "build/cl/a.stml",
            "<aedilis-manifestum scopus=\"tools/a.c\">\n"
            "  <obiecta><obiectum via=\"lib/x.c\"/></obiecta>\n"
            "</aedilis-manifestum>\n");
        mundi_ponere(&discus, "build/cl/b.stml",
            "<aedilis-manifestum scopus=\"tools/b.c\">\n"
            "  <capita><caput via=\"include/y.h\"/></capita>\n"
            "</aedilis-manifestum>\n");
        mundi_ponere(&discus, "build/cl/notae.txt", "non manifestum\n");
        mundi_ponere(&discus, "tools/a.c", "int a;\n");
        mundi_ponere(&discus, "tools/b.c", "int b;\n");
        mundi_ponere(&discus, "lib/x.c", "int x;\n");
        mundi_ponere(&discus, "include/y.h", "int y;\n");
        a = mundi_actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "manifesta", "build/cl",
            piscina);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));

        /* plagula in clausura manifesti SECUNDI mutata -> aliud */
        mundi_ponere(&discus, "include/y.h", "int y2;\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
        mundi_ponere(&discus, "include/y.h", "int y;\n");

        /* plagula non .stml nihil confert */
        mundi_ponere(&discus, "build/cl/notae.txt", "mutatae\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* manifestum ablatum (radix deleta) -> aliud */
        nomina_minus[0] = "a.stml";
        nomina_minus[1] = "notae.txt";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "build/cl", nomina_minus, II);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);

        /* directorium absens (clonus recens) -> FALSUM nominatum */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(mundi_continet(causa, "build/cl", piscina));
    }


    /* ==================================================
     * PROBARE: ingressus RADICES (radices inclusionum configurationis)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
                Sigillum  s1;
                Sigillum  s2;
                  chorda  causa;
      constans character* include_nomina[I];
      constans character* src_nomina[I];
      constans character* src_plus[II];

        imprimere("\n--- Probans radices ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "aedilis.stml",
            "<aedilis>\n"
            "  <inclusa>\n"
            "    <via (>include\n"
            "    <via (>src\n"
            "  </inclusa>\n"
            "</aedilis>\n");
        include_nomina[0]  = "a.h";
        src_nomina[0]      = "b.h";
        mundi_directorium_ponere(&discus, "include", include_nomina, I);
        mundi_directorium_ponere(&discus, "src", src_nomina, I);
        a = mundi_actio(piscina, "r", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "radices", "aedilis.stml",
            piscina);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));

        /* caput novum in radice SECUNDA -> aliud (Review Focus 2) */
        src_plus[0] = "b.h";
        src_plus[1] = "c.h";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "include", include_nomina, I);
        mundi_directorium_ponere(&discus, "src", src_plus, II);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);

        /* radix nominata sed absens -> FALSUM nominatum */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "include", include_nomina, I);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(mundi_continet(causa, "src", piscina));

        /* configuratio sine sectione inclusarum -> FALSUM (radices
         * nullae = tegmen nullum, numquam tacitum) */
        mundi_directorium_ponere(&discus, "src", src_nomina, I);
        mundi_ponere(&discus, "aedilis.stml",
            "<aedilis>\n</aedilis>\n");
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(mundi_continet(causa, "inclusa", piscina));
    }


    /* ==================================================
     * PROBARE: actio tacta a viis commissis (T8 gradus III)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
            FabricaActio* m;
            FabricaActio* p;
        FabricaIngressus* ingressus;
                     Xar* viae;
      constans character* nomina[II];
      constans character* src_nomina[I];

        imprimere("\n--- Probans actionem tactam ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "lib/x.c", "int x;\n");
        mundi_ponere(&discus, "gen/exitus.c", "g\n");
        nomina[0] = "a.h";
        nomina[1] = "b.h";
        mundi_directorium_ponere(&discus, "include", nomina, II);
        a = mundi_actio(piscina, "a", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        mundi_ingressum_addere(a, "directorium", "include",
            piscina);
        (vacuum)mundi_exitum_addere(a, "gen/exitus.c",
            "regeneratio", piscina);

        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("lib/y.c",
            piscina);
        /* via aliena -> non tacta */
        CREDO_FALSUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* ingressus ipse -> tacta */
        *(chorda*)xar_addere(viae) = chorda_ex_literis("lib/x.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* plagula nova in directorio enumerato (include/nova.h) */
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("include/nova.h",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* exitus ipse commissus -> tacta */
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("gen/exitus.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));

        /* manifestum: plagula in clausura eius */
        mundi_ponere(&discus, "build/m.stml",
            "<aedilis-manifestum scopus=\"tools/m.c\">\n"
            "  <capita><caput via=\"include/a.h\"/></capita>\n"
            "</aedilis-manifestum>\n");
        mundi_ponere(&discus, "tools/m.c", "int m;\n");
        mundi_ponere(&discus, "include/a.h", "int a;\n");
        m = mundi_actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(m, "manifestum",
            "build/m.stml",
            piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("include/a.h",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, m, viae, piscina));

        /* plagulae: plagula nova suffixo congruens in directorio */
        src_nomina[0] = "a.c";
        mundi_directorium_ponere(&discus, "src", src_nomina, I);
        mundi_ponere(&discus, "src/a.c", "int a;\n");
        p = mundi_actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(p, "plagulae", "src",
            piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(p->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c", piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("src/nova.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, p, viae, piscina));
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("src/notae.md",
            piscina);
        CREDO_FALSUM(fabrica_actio_tacta(&sutura, p, viae, piscina));

        /* ingressus absens -> tacta (conservativum: iudex IGNOTUM
         * nominabit, numquam tacite praetermittitur) */
        mundi_ingressum_addere(a, "fasciculus",
            "lib/abest.c",
            piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("doc/nihil.md",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
    }


    /* ==================================================
     * PROBARE: genera et strategiae (plan 1b T1)
     * ================================================== */

    {
               DiscusFictus  discus;
              FabricaSutura  sutura;
               FabricaActio* p;
           FabricaIngressus* ingressus;
              FabricaExitus  exitus;
               FabricaLocus* locus;
        InternamentumChorda* intern;
                     chorda  contentum;
                     chorda  causa;
                        Xar* loci;
                        Xar* actiones;
                        i32  i;
                        b32  plagulae_inventae;
                        b32  plagula_inventa;
         constans character* genera[IX];
         constans character* src_nomina[II];

        imprimere("\n--- Probans genera et strategias ---\n");
        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* registrum generum: omnia nomina declarationum, sigillare
         * omnibus */
        genera[0] = "fasciculus";
        genera[1] = "configuratio";
        genera[2] = "instrumentum";
        genera[3] = "directorium";
        genera[4] = "manifestum";
        genera[5] = "plagulae";
        genera[6] = "manifesta";
        genera[7] = "radices";
        genera[8] = "binarium";
        per (i = ZEPHYRUM; i < IX; i++)
        {
            CREDO_NON_NIHIL(mundi_genus(genera[i], piscina));
            CREDO_VERUM(mundi_genus(genera[i], piscina)->sigillare
                != NIHIL);
        }
        CREDO_NIHIL(mundi_genus("compilatio", piscina));

        /* exitus esse possunt: fasciculus, binarium; ceteri non */
        CREDO_VERUM(mundi_genus("fasciculus", piscina)->locare
            != NIHIL);
        CREDO_VERUM(mundi_genus("binarium", piscina)->locare != NIHIL);
        CREDO_VERUM(mundi_genus("manifestum", piscina)->locare
            == NIHIL);
        CREDO_VERUM(mundi_genus("fasciculus", piscina)->reproducibile);
        CREDO_FALSUM(mundi_genus("binarium", piscina)->reproducibile);

        /* locare: locus unus, PLAGULA, via exitus */
        exitus.via = chorda_ex_literis("bin/manus", piscina);
        exitus.scriptura = exitus.via;
        exitus.genus = mundi_genus("binarium", piscina);
        exitus.strategia = mundi_strategia("relatio", piscina);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        CREDO_VERUM(exitus.genus->locare(&exitus, piscina, loci));
        CREDO_AEQUALIS_I32(xar_numerus(loci), I);
        locus = (FabricaLocus*)xar_obtinere(loci, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)locus->forma,
            (i32)FABRICA_LOCUS_PLAGULA);
        CREDO_CHORDA_AEQUALIS_LITERIS(locus->via, "bin/manus");

        /* registrum strategiarum */
        CREDO_NON_NIHIL(mundi_strategia("regeneratio", piscina));
        CREDO_NON_NIHIL(mundi_strategia("relatio", piscina));
        CREDO_NIHIL(mundi_strategia("compilatio", piscina));
        CREDO_VERUM(mundi_strategia("regeneratio",
            piscina)->octetis_comparat);
        CREDO_FALSUM(mundi_strategia("relatio",
            piscina)->octetis_comparat);
        CREDO_VERUM(strcmp(mundi_strategia("relatio",
            piscina)->genus_ordinarium, "binarium") == 0);
        CREDO_VERUM(strcmp(mundi_strategia("regeneratio",
            piscina)->genus_ordinarium, "fasciculus") == 0);

        /* lector: genus exitus absens -> genus ordinarium */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"i\" genus=\"institutio\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"bin/i\" provenientia=\"relatio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        CREDO_VERUM(((FabricaExitus*)xar_obtinere(((FabricaActio*)
            xar_obtinere(actiones, ZEPHYRUM))->exitus, ZEPHYRUM))->genus
            == mundi_genus("binarium", piscina));

        /* lector: regeneratio generi non reproducibili recusatur */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"b\" genus=\"binarium\""
            " provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:4", piscina));
        CREDO_VERUM(mundi_continet(causa,
            "strategia regeneratio generi binarium non licet",
            piscina));

        /* lector: genus ingressus solum exitus esse nequit */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"b\" genus=\"manifestum\""
            " provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "exitus esse nequit",
            piscina));

        /* enumerare: plagulae -> locus PLAGULAE cum suffixis, et
         * PLAGULA pro quaque plagula congruente */
        mundi_discum_parare(&discus, &sutura, piscina);
        src_nomina[0] = "a.c";
        src_nomina[1] = "notae.md";
        mundi_directorium_ponere(&discus, "src", src_nomina, II);
        mundi_ponere(&discus, "src/a.c", "int a;\n");
        mundi_ponere(&discus, "src/notae.md", "notae\n");
        p = mundi_actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(p, "plagulae", "src", piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(p->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c", piscina);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        CREDO_VERUM(fabrica_actionem_enumerare(&sutura, p, piscina,
            loci, &causa));
        plagulae_inventae  = FALSUM;
        plagula_inventa    = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(loci); i++)
        {
            locus = (FabricaLocus*)xar_obtinere(loci, i);
            si (   locus->forma == FABRICA_LOCUS_PLAGULAE
                && chorda_aequalis_literis(locus->via, "src")
                && chorda_aequalis_literis(locus->suffixa, ".c"))
            {
                plagulae_inventae = VERUM;
            }
            si (   locus->forma == FABRICA_LOCUS_PLAGULA
                && chorda_aequalis_literis(locus->via, "src/a.c"))
            {
                plagula_inventa = VERUM;
            }
            /* plagula suffixo non congruens nusquam */
            CREDO_FALSUM(chorda_aequalis_literis(locus->via,
                "src/notae.md"));
        }
        CREDO_VERUM(plagulae_inventae);
        CREDO_VERUM(plagula_inventa);
    }


    /* ==================================================
     * PROBARE: instrumentum_domus (fabrica spec 3 v4) - binarium domus
     * per lineam 'ingressus' relationis sigillatum: relinkatio (octeti
     * alii) et linea 'commissum' clavem non mutant; fontes alii (linea
     * ingressus alia) mutant; sine relatione: octeti
     * ================================================== */

    {
         DiscusFictus  discus;
        FabricaSutura  sutura;
         FabricaActio* a;
             Sigillum  s1;
             Sigillum  s2;

        imprimere("\n--- Probans instrumentum domus (provenientia) ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "bin/inst", "octeti I\n");
        a = mundi_actio(piscina, "usor", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "instrumentum_domus", "bin/inst",
            piscina);
        discus.relatio = "provenientia 1\nartificium bin/inst\n"
            "ingressus aaaa\ncommissum 1111 SORDIDUM\n";
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));
        mundi_ponere(&discus, "bin/inst", "octeti II (relinkatum)\n");
        discus.relatio = "provenientia 1\nartificium bin/inst\n"
            "ingressus aaaa\ncommissum 2222\n";
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
        discus.relatio = "provenientia 1\nartificium bin/inst\n"
            "ingressus bbbb\ncommissum 2222\n";
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
        /* sine relatione: octeti (cautum) */
        discus.relatio = NIHIL;
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s1));
        mundi_ponere(&discus, "bin/inst", "octeti III\n");
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
    }


    /* ==================================================
     * PROBARE: genus ingressus 'effectus' (effectus-plan T7, spec par.
     * VII) - regula digestionis quaeque: ingressus mutatus clavem mutat
     * (IGNOTUM, cursus novus); idem -> RECENS
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* actiones[I];
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
                         Xar* ordo;
                         Xar* electa;
                      chorda  causa;
          constans character* VERDICTUM;
          constans character* nomina_fontium[] = { "a.c", "b.h" };
          constans character* nomina_aucta[] = { "a.c", "b.h", "c.c" };
          constans character* nomina_aliena[] = { "a.c", "b.h", "d.h" };

        imprimere("\n--- Probans genus effectus ---\n");
        VERDICTUM      = "build/fabrica/verdicta/e.txt";
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.lectiones_legere = mundi_lectiones_legere;
        sutura.lectiones_scribere = mundi_lectiones_scribere;
        sutura.ambitus = mundi_ambitus_fictum;
        sutura.species = mundi_species_ficta;
        sutura.effectus = mundi_effectus_ficti;
        sutura.exitus_noti = tabula_dispersa_creare_chorda(piscina, 16);
        (vacuum)tabula_dispersa_inserere(sutura.exitus_noti,
            chorda_ex_literis("build/corpus.lst", piscina), NIHIL);
        mundi_ambitus_ficti[0] = "HOME";
        mundi_ambitus_ficti[1] = "/home/u";
        mundi_ambitus_ficti[2] = NIHIL;
        mundi_via_absens_ficta = "build/gen.h";
        mundi_ponere(&discus, "porta.sh", "echo porta\n");
        mundi_ponere(&discus, "flag.txt", "ok\n");
        mundi_ponere(&discus, "build/corpus.lst", "a.toml\n");
        mundi_directorium_ponere(&discus, "src/", nomina_fontium, II);
        mundi_effectus_effusio =
            "octeti\tporta.sh\n"
            "octeti\tflag.txt\n"
            "probatio\tbuild/gen.h\n"
            "ambitus\tHOME\n"
            "ambitus\tFABRICA_LECTIONES\n"
            "nomina\tsrc/\t*.c\n"
            "dominus\tbuild/corpus.lst\n";
        mundi_scriptum_addere(&discus, "porta_sh", VERDICTUM, NIHIL,
            "e: transiit\n", 0, FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "porta_e",
            "porta_sh",
            "porta.sh", VERDICTUM, "verdictum");
        actiones[0]->genus      = FABRICA_ACTIO_IUDICIUM;
        actiones[0]->lectiones  = VERUM;
        mundi_ingressum_addere(actiones[0], "effectus", "porta.sh",
            piscina);
        exitus = (FabricaExitus*)xar_obtinere(actiones[0]->exitus,
            ZEPHYRUM);
        ordo    = mundi_ordinare_fictas(piscina, actiones, I);
        electa  = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VERDICTUM,
            piscina);
        mundi_ponere(&discus, "build/fabrica/lectiones/porta_e.tsv",
            "L\tflag.txt\n");
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* octeti: lectio bash (flag.txt) mutata */
        mundi_ponere(&discus, "flag.txt", "non\n");
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_FALSUM(iudicium.status == FABRICA_RECENS);
        mundi_ponere(&discus, "flag.txt", "ok\n");
        /* probatio: absens -> praesens */
        mundi_via_absens_ficta = NIHIL;
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_FALSUM(iudicium.status == FABRICA_RECENS);
        mundi_via_absens_ficta = "build/gen.h";
        /* ambitus: HOME mutata; FABRICA_* protocollum, non ingressus */
        mundi_ambitus_ficti[1] = "/home/v";
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_FALSUM(iudicium.status == FABRICA_RECENS);
        mundi_ambitus_ficti[1] = "/home/u";
        mundi_ambitus_ficti[2] = "FABRICA_LECTIONES";
        mundi_ambitus_ficti[3] = "build/x.tsv";
        mundi_ambitus_ficti[4] = NIHIL;
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        mundi_ambitus_ficti[2] = NIHIL;
        /* nomina: plagula congruens addita; non congruens nihil */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "src/", nomina_aliena, III);
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "src/", nomina_aucta, III);
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_FALSUM(iudicium.status == FABRICA_RECENS);
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "src/", nomina_fontium, II);
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        /* arbor (effectus-plan-2 T6): contenta subarboris - mutatio
         * plagulae intra (nomina eadem) clavem mutat; 'directorium'
         * (nomina sola) eam non videbat */
        mundi_ponere(&discus, "src/a.c", "int a;\n");
        mundi_ponere(&discus, "src/b.h", "/* b */\n");
        mundi_effectus_effusio =
            "octeti\tporta.sh\n"
            "arbor\tsrc/\n";
        (vacuum)fabrica_sanare(&sutura, ordo, electa, FALSUM, piscina,
            &causa);
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        mundi_ponere(&discus, "src/a.c", "int b;\n");
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_FALSUM(iudicium.status == FABRICA_RECENS);
        mundi_ponere(&discus, "src/a.c", "int a;\n");
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        /* ignotum: IGNOTUM sedes nominata */
        mundi_effectus_effusio =
            "octeti\tporta.sh\n"
            "ignotum\tporta.sh:9:1-9:5\tvalor ignotus\n";
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "porta.sh:9",
            piscina));
        /* dominus absens: IGNOTUM nominatum */
        mundi_effectus_effusio = "dominus\tbuild/sine_domino.txt\n";
        iudicium = fabrica_iudicare(&sutura, actiones[0], exitus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "sine domino",
            piscina));
        mundi_effectus_effusio = "";
        mundi_via_absens_ficta = NIHIL;
    }


    /* ========================================================
     * AXES DUO (fabrica-6 T2): res x clavis -> genus; alias 'genus'
     * idem; ambo aut par ignotum recusantur nominatim
     * ======================================================== */

    {
        hic_manens constans character* constans paria[][III] = {
            { "plagula", "contentum", "fasciculus" },
            { "plagulae", "contentum", "plagulae" },
            { "directorium", "nomina", "directorium" },
            { "plagula", "clausura", "manifestum" },
            { "directorium", "clausura", "manifesta" },
            { "instrumentum", "contentum", "instrumentum" },
            { "instrumentum", "provenientia", "instrumentum_domus" },
            { "instrumentum", "identitas", "identitas_clang" },
            { "plagula", "effectus", "effectus" },
            { "repositorium", "commissum", "repositorium" }
        };
         InternamentumChorda* intern = internamentum_creare(piscina);
                      chorda  causa;
                         i32  k;

        imprimere("\n--- Probans axes duo (res x clavis) ---\n");
        per (k = ZEPHYRUM;
             k < (i32)(magnitudo(paria) / magnitudo(paria[0])); k++)
        {
            character  textus[DXII];
                  Xar* actiones;

            sprintf(textus, "<aedificatio>\n  <actio titulus=\"a\" "
                "genus=\"generator\">\n    <mandatum>\n"
                "      <verbum! (>x\n    </mandatum>\n"
                "    <ingressus res=\"%s\" clavis=\"%s\" via=\"v\"/>\n"
                "    <exitus via=\"o\" provenientia=\"regeneratio\"/>\n"
                "  </actio>\n</aedificatio>\n", paria[k][0],
                paria[k][1]);
            causa = chorda_ex_literis("", piscina);
            actiones = fabrica_declarationes_legere(
                chorda_ex_literis(textus, piscina), "d.stml", piscina,
                intern, &causa);
            CREDO_NON_NIHIL(actiones);
            si (actiones != NIHIL)
            {
                constans FabricaIngressus* ingressus_lectus =
                    (constans FabricaIngressus*)xar_obtinere(
                        ((FabricaActio*)xar_obtinere(actiones,
                            ZEPHYRUM))->ingressus, ZEPHYRUM);

                CREDO_VERUM(ingressus_lectus->genus
                    == fabrica_genus_invenire(
                    chorda_ex_literis(paria[k][2], piscina)));
            }
            alioquin
            {
                imprimere("  par %s/%s: %.*s\n", paria[k][0],
                    paria[k][1],
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
            }
        }
        /* ambo -> recusatio */
        causa = chorda_ex_literis("", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(chorda_ex_literis(
            "<aedificatio>\n  <actio titulus=\"a\" genus=\"generator\">\n"
            "    <mandatum>\n      <verbum! (>x\n    </mandatum>\n"
            "    <ingressus genus=\"fasciculus\" res=\"plagula\" "
            "clavis=\"contentum\" via=\"v\"/>\n"
            "    <exitus via=\"o\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n</aedificatio>\n", piscina), "d.stml", piscina,
            intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "genus et res", piscina));
        /* par sine genere -> recusatio par nominans */
        causa = chorda_ex_literis("", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(chorda_ex_literis(
            "<aedificatio>\n  <actio titulus=\"a\" genus=\"generator\">\n"
            "    <mandatum>\n      <verbum! (>x\n    </mandatum>\n"
            "    <ingressus res=\"directorium\" clavis=\"provenientia\" "
            "via=\"v\"/>\n"
            "    <exitus via=\"o\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n</aedificatio>\n", piscina), "d.stml", piscina,
            intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "directorium/provenientia",
            piscina));
    }


    /* ========================================================
     * CHASSIS (fabrica-6 T1): omne genus registratum fixum
     * conformitatis habet et id implet
     * ======================================================== */

    {
        i32 g;

        imprimere("\n--- Probans chassis: fixa conformitatis generum ---\n");
        CREDO_VERUM(fabrica_genera_numerus() > ZEPHYRUM);
        CREDO_NIHIL(fabrica_genus_obtinere(fabrica_genera_numerus()));
        per (g = ZEPHYRUM; g < fabrica_genera_numerus(); g++)
        {
                  constans FabricaGenus* genus =
                      fabrica_genus_obtinere(g);
            constans FixumConformitatis* fixum = _fixum_invenire(
                genus->titulus);
                                    Xar* p0;
                                    Xar* p1;
                                    Xar* p2;

            si (fixum == NIHIL)
            {
                imprimere("  FRACTUM: genus '%s' sine fixo conformitatis\n",
                    genus->titulus);
                CREDO_NON_NIHIL(fixum);
                perge;
            }
            p0 = _particulas_fixi(fixum, genus, NIHIL, piscina);
            p1 = _particulas_fixi(fixum, genus, fixum->mutare, piscina);
            p2 = _particulas_fixi(fixum, genus, fixum->alienum,
                piscina);
            CREDO_NON_NIHIL(p0);
            CREDO_NON_NIHIL(p1);
            CREDO_NON_NIHIL(p2);
            si (p0 == NIHIL || p1 == NIHIL || p2 == NIHIL)
            {
                perge;
            }
            si (!_particula_differt(p0, p1, fixum->particula, piscina))
            {
                imprimere("  FRACTUM: genus '%s': mutatio classis particulam"
                    " '%s' non mutat\n", genus->titulus,
                    fixum->particula);
            }
            CREDO_VERUM(_particula_differt(p0, p1, fixum->particula,
                piscina));
            si (!_particulae_aequales(p0, p2))
            {
                imprimere("  FRACTUM: genus '%s': mutatio aliena sigillum"
                    " mutat\n", genus->titulus);
            }
            CREDO_VERUM(_particulae_aequales(p0, p2));
        }
        mundi_identitas_ficta   = "clang I";
        mundi_effectus_effusio  = "";
        _caput_fictum           = "commissum I";
    }

    imprimere("\n");

    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
