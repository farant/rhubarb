/* probatio_fasciculum.c - planum purum, scriptor, lector, iter reditus
 *
 * Portae H1-H6 (project-specs/fasciculum-spec.md par. VIII). Quaestio
 * quam nulla suita respondere potest - an fasciculus duplici ictu
 * aperiatur - in tools/fasciculum_fumus.sh manu vivit.
 */

#include "latina.h"
#include "fasciculum.h"
#include "filum.h"
#include "via.h"
#include "credo.h"
#include "piscina.h"
#include "internamentum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fasciculum minimum: exsecutabile verum in area, nihil aliud. */
interior Fasciculum
_fasciculum_minimum (
    constans character* via_exsecutabilis,
               Piscina* piscina)
{
    Fasciculum f;

    memset(&f, ZEPHYRUM, magnitudo(Fasciculum));
    f.identitas     = chorda_ex_literis("org.rhubarb.probatio",
                                        piscina);
    f.titulus       = chorda_ex_literis("Probatio", piscina);
    f.exsecutabile  = via_exsecutabilis;
    redde f;
}

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;
     constans character* via_binarii =
         "/tmp/probatio_fasciculum_binarium";

    piscina = piscina_generare_dynamicum("probatio_fasciculum",
                                         1048576);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* exsecutabile fictum: reddere eum EXSISTERE poscit */
    CREDO_VERUM(filum_scribere_literis(via_binarii, "#!/bin/sh\n"));

    /* ---- H1: reddere PURUM ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
          FasciculumPars* pars;

        imprimere("\n--- H1: planum purum ---\n");

        f = _fasciculum_minimum(via_binarii, piscina);
        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_SUCCESSUS);
        CREDO_NON_NIHIL(partes);
        /* sine icona: Info.plist + exsecutabile = duae partes */
        CREDO_AEQUALIS_I32(xar_numerus(partes), II);

        pars = (FasciculumPars*)xar_obtinere(partes, ZEPHYRUM);
        CREDO_NON_NIHIL(pars);
        CREDO_AEQUALIS_I32((i32)pars->origo, (i32)FASCICULUM_GENITUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(pars->semita,
                                      "Contents/Info.plist");
        CREDO_CHORDA_NON_VACUA(pars->octeti);
        CREDO_FALSUM(pars->exsecutabile_fiat);

        pars = (FasciculumPars*)xar_obtinere(partes, I);
        CREDO_NON_NIHIL(pars);
        CREDO_AEQUALIS_I32((i32)pars->origo, (i32)FASCICULUM_COPIATUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(pars->semita,
            "Contents/MacOS/probatio_fasciculum_binarium");
        CREDO_VERUM(pars->exsecutabile_fiat);
        /* fons PER INDICEM tenetur, non copiatur: binarium XX MB per
         * piscinam non transit (par. III) */
        CREDO_VERUM(pars->fons == via_binarii);

        /* PURITAS: nihil in disco creatum est. Nota limitem - reddere
         * radicem NON accipit, ergo fasciculum scribere NON POTEST;
         * hoc solum probat nihil relativum in cwd apparuisse. */
        CREDO_FALSUM(filum_directorium_existit("Contents"));
        CREDO_FALSUM(filum_existit("Contents/Info.plist"));
    }

    /* ---- H2: claves Info.plist et directio fusionis ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
          FasciculumPars* pars;
           PlistResultus  r;
              PlistValor* v;

        imprimere("\n--- H2: claves et fusio ---\n");

        f                = _fasciculum_minimum(via_binarii, piscina);
        f.versio         = chorda_ex_literis("2.5", piscina);
        f.sine_scandali  = VERUM;
        f.plista_extra   = plist_dictio_creare(piscina);
        /* clavis generata SUBSTITUITUR a vocante (D8) */
        plist_dictio_ponere(f.plista_extra,
            chorda_ex_literis("CFBundlePackageType", piscina),
            plist_chordam_creare(chorda_ex_literis("BNDL", piscina),
                                 piscina), piscina);
        plist_dictio_ponere(f.plista_extra,
            chorda_ex_literis("NSAppleScriptEnabled", piscina),
            plist_veritatem_creare(VERUM, piscina), piscina);

        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));
        pars = (FasciculumPars*)xar_obtinere(partes, ZEPHYRUM);
        CREDO_NON_NIHIL(pars);
        r = plist_legere(pars->octeti, piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);

        v = plist_dictio_capere(r.radix, "CFBundleIdentifier");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus,
                                      "org.rhubarb.probatio");
        /* DERIVATA, non data */
        v = plist_dictio_capere(r.radix, "CFBundleExecutable");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus,
            "probatio_fasciculum_binarium");
        v = plist_dictio_capere(r.radix, "CFBundleName");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "Probatio");
        v = plist_dictio_capere(r.radix, "CFBundleShortVersionString");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "2.5");
        /* omissa ad ordinarium, non ad vacuum */
        v = plist_dictio_capere(r.radix, "CFBundleVersion");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "1");
        v = plist_dictio_capere(r.radix,
                                "CFBundleInfoDictionaryVersion");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "6.0");
        /* praesto SEMPER: sine hoc fenestra in scrinio retinae
         * scalata et obscura redditur - vitium quod consilium
         * simulat */
        v = plist_dictio_capere(r.radix, "NSHighResolutionCapable");
        CREDO_NON_NIHIL(v);
        CREDO_VERUM(v->veritas);
        v = plist_dictio_capere(r.radix, "LSUIElement");
        CREDO_NON_NIHIL(v);
        CREDO_VERUM(v->veritas);
        /* sine icona clavis ABEST, non vacua est */
        CREDO_NIHIL(plist_dictio_capere(r.radix, "CFBundleIconFile"));
        /* FUSIO: vocans vincit clavem generatam */
        v = plist_dictio_capere(r.radix, "CFBundlePackageType");
        CREDO_NON_NIHIL(v);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "BNDL");
        /* et clavem SUAM adfert quam bibliotheca numquam generat */
        v = plist_dictio_capere(r.radix, "NSAppleScriptEnabled");
        CREDO_NON_NIHIL(v);
        /* NIHIL in assertione IPSA, non circa eam: clavis absens
         * rubrum LEGIBILE dare debet, non SIGSEGV qui compendium
         * reliquum truncat (culpa plantata II hanc clavem tollit) */
        CREDO_VERUM(v != NIHIL && v->veritas);
    }

    filum_delere(via_binarii);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
