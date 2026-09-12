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

    /* ---- H4: scriptor in area, cum icone ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
             FilumStatus  st;
      constans character* via_iconis =
          "/tmp/probatio_fasciculum_icon.icns";
     constans character* radix_areae =
         "/tmp/probatio_fasciculum_area";
     constans character* area =
         "/tmp/probatio_fasciculum_area/Probatio.app";
               character via_contents[CCLVI];
               character via_macos[CCLVI];
               character via_res[CCLVI];
               character via_plistae[CCLVI];
               character via_exs[CCLVI];
               character via_icon_posita[CCLVI];

        imprimere("\n--- H4: scriptor ---\n");

        sprintf(via_contents, "%.200s/Contents", area);
        sprintf(via_macos, "%.200s/Contents/MacOS", area);
        sprintf(via_res, "%.200s/Contents/Resources", area);
        sprintf(via_plistae, "%.200s/Contents/Info.plist", area);
        sprintf(via_exs,
            "%.200s/Contents/MacOS/probatio_fasciculum_binarium", area);
        sprintf(via_icon_posita,
            "%.200s/Contents/Resources/probatio_fasciculum_icon.icns",
            area);

        /* STATUS DETERMINATUS: arborem cursus prioris tollere,
         * altissima primum - aliter 'scriptor directoria creavit'
         * nihil probat, quia iam adessent (viriditas mendax) */
        filum_delere(via_plistae);
        filum_delere(via_exs);
        filum_delere(via_icon_posita);
        filum_delere(via_macos);
        filum_delere(via_res);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
        CREDO_FALSUM(filum_directorium_existit(area));

        /* icon fictus: fasciculum octetos PONIT, non iudicat
         * (par. XI) */
        CREDO_VERUM(filum_scribere_literis(via_iconis, "icns"));

        f       = _fasciculum_minimum(via_binarii, piscina);
        f.icon  = via_iconis;
        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));
        CREDO_AEQUALIS_I32(xar_numerus(partes), III);

        CREDO_VERUM(fasciculum_scribere(partes, area, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_SUCCESSUS);

        /* DIRECTORIA NIDIFICATA ab area virgine creata */
        CREDO_VERUM(filum_directorium_existit(via_contents));
        CREDO_VERUM(filum_directorium_existit(via_macos));
        CREDO_VERUM(filum_directorium_existit(via_res));

        CREDO_VERUM(filum_existit(via_plistae));
        CREDO_VERUM(filum_existit(via_icon_posita));

        CREDO_VERUM(filum_status(via_exs, &st));
        CREDO_VERUM(st.est_filum);
        /* BIT EXSEQUENDI: causa paris in filo (opus I) */
        CREDO_VERUM(st.potest_exsequi);

        /* modus SELECTIVUS: plista et icon exsecutabiles NON fiunt */
        CREDO_VERUM(filum_status(via_plistae, &st));
        CREDO_FALSUM(st.potest_exsequi);
        CREDO_VERUM(filum_status(via_icon_posita, &st));
        CREDO_FALSUM(st.potest_exsequi);

        filum_delere(via_plistae);
        filum_delere(via_exs);
        filum_delere(via_icon_posita);
        filum_delere(via_macos);
        filum_delere(via_res);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
        filum_delere(via_iconis);
    }

    /* ---- H6: scriptio semiperfecta status semitam nominat ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
      constans character* radix_areae =
          "/tmp/probatio_fasciculum_clausa";
     constans character* area =
         "/tmp/probatio_fasciculum_clausa/Probatio.app";
               character via_contents[CCLVI];
               character via_macos[CCLVI];
               character via_plistae[CCLVI];
               character via_exs[CCLVI];

        imprimere("\n--- H6: scriptio semiperfecta ---\n");

        sprintf(via_contents, "%.200s/Contents", area);
        sprintf(via_macos, "%.200s/Contents/MacOS", area);
        sprintf(via_plistae, "%.200s/Contents/Info.plist", area);
        sprintf(via_exs,
            "%.200s/Contents/MacOS/probatio_fasciculum_binarium", area);

        /* modum PRIMUM restituere: si cursus prior in 0500 mortuus est,
         * deletiones ipsae deficerent et cursum proximum venenarent */
        filum_modum_ponere(via_macos, 0755);
        filum_delere(via_exs);
        filum_delere(via_plistae);
        filum_delere(via_macos);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);

        f = _fasciculum_minimum(via_binarii, piscina);
        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));

        /* Contents/MacOS praestruitur et CLAUDITUR. Semita tota, quia
         * creare_si_necesse hic de ENOENT deficeret (par modi II) */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(via_macos));
        CREDO_VERUM(filum_modum_ponere(via_macos, 0500));

        CREDO_FALSUM(fasciculum_scribere(partes, area, &status, &sedes,
                                         piscina));
        CREDO_VERUM(status == FASCICULUM_ERROR_SCRIPTIO
                 || status == FASCICULUM_ERROR_MODUS);
        CREDO_CHORDA_CONTINET(sedes,
            chorda_ex_literis("Contents/MacOS", piscina));
        /* SINE REVERSIONE: plista iam scripta MANET (par. V) */
        CREDO_VERUM(filum_existit(via_plistae));

        /* modum RESTITUE semper, in omni exitu */
        CREDO_VERUM(filum_modum_ponere(via_macos, 0755));

        filum_delere(via_exs);
        filum_delere(via_plistae);
        filum_delere(via_macos);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
    }

    /* ---- H6 b: scriptio GENITI fracta (ramus alter) ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
      constans character* radix_areae =
          "/tmp/probatio_fasciculum_clausa_b";
     constans character* area =
         "/tmp/probatio_fasciculum_clausa_b/Probatio.app";
               character via_contents[CCLVI];
               character via_plistae[CCLVI];

        imprimere("\n--- H6 b: scriptio geniti fracta ---\n");

        sprintf(via_contents, "%.200s/Contents", area);
        sprintf(via_plistae, "%.200s/Contents/Info.plist", area);

        filum_modum_ponere(via_contents, 0755);
        filum_delere(via_plistae);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);

        f = _fasciculum_minimum(via_binarii, piscina);
        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));

        /* Contents EXSISTIT sed CLAUDITUR: ergo directorium creari
         * non debet, sed plista GENITA scribi non potest. Hoc ramum
         * GENITUM probat, quem H6 numquam tangit (ibi Contents apertum
         * est et copia sola deficit) - culpa plantata in eo ramo
         * VIRIDIS mansit donec haec assertio addita est. */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            via_contents));
        CREDO_VERUM(filum_modum_ponere(via_contents, 0500));

        CREDO_FALSUM(fasciculum_scribere(partes, area, &status, &sedes,
                                         piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_ERROR_SCRIPTIO);
        CREDO_CHORDA_CONTINET(sedes,
            chorda_ex_literis("Contents/Info.plist", piscina));
        /* NIHIL scriptum: pars prima ipsa defecit, ergo hic nullus
         * superstes est quem par. V servaret */
        CREDO_FALSUM(filum_existit(via_plistae));

        CREDO_VERUM(filum_modum_ponere(via_contents, 0755));
        filum_delere(via_plistae);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
    }

    /* ---- H3: recusationes, unaquaeque statu SUO et sede SUA ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
      constans character* via_nusquam =
          "/tmp/probatio_fasciculum_nusquam_xyz";
     constans character* radix_areae =
         "/tmp/probatio_fasciculum_recusata";
     constans character* area =
         "/tmp/probatio_fasciculum_recusata/Probatio.app";
               character via_contents[CCLVI];
               character via_macos[CCLVI];
               character via_plistae[CCLVI];
               character via_exs[CCLVI];

        imprimere("\n--- H3: recusationes ---\n");

        sprintf(via_contents, "%.200s/Contents", area);
        sprintf(via_macos, "%.200s/Contents/MacOS", area);
        sprintf(via_plistae, "%.200s/Contents/Info.plist", area);
        sprintf(via_exs,
            "%.200s/Contents/MacOS/probatio_fasciculum_binarium", area);

        filum_delere(via_plistae);
        filum_delere(via_exs);
        filum_delere(via_macos);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);

        /* (i) identitas DEEST: clavis nominatur, non chorda vacua.
         * Tres primae recusationes in reddere ab opere II iam vivunt;
         * hic primum ASSERUNTUR. */
        f = _fasciculum_minimum(via_binarii, piscina);
        f.identitas.datum = NIHIL;
        f.identitas.mensura = ZEPHYRUM;
        CREDO_FALSUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_ERROR_DESUNT);
        CREDO_CHORDA_AEQUALIS_LITERIS(sedes, "CFBundleIdentifier");

        /* (ii) exsecutabile NON exsistit: via ipsa nominatur */
        f = _fasciculum_minimum(via_nusquam, piscina);
        CREDO_FALSUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_I32((i32)status,
                           (i32)FASCICULUM_ERROR_EXSECUTABILE);
        CREDO_CHORDA_AEQUALIS_LITERIS(sedes, via_nusquam);

        /* (iii) icon NON exsistit: status SUUS, non EXSECUTABILE */
        f       = _fasciculum_minimum(via_binarii, piscina);
        f.icon  = via_nusquam;
        CREDO_FALSUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_ERROR_ICON);
        CREDO_CHORDA_AEQUALIS_LITERIS(sedes, via_nusquam);

        /* (iv) directorium SINE Contents/: non est fasciculus */
        {
            Fasciculum lectus;

            CREDO_VERUM(filum_directorium_creare_cum_parentibus(area));
            CREDO_FALSUM(fasciculum_legere(area, &lectus, &status,
                                           &sedes, piscina, intern));
            CREDO_AEQUALIS_I32((i32)status,
                               (i32)FASCICULUM_ERROR_NON_FASCICULUM);
            CREDO_CHORDA_CONTINET(sedes,
                chorda_ex_literis("Contents", piscina));
        }

        /* (v) Info.plist DEPRAVATA: status PLISTA, et sedes plagulam
         * NOMINAT. Nota: par. VI dicit statum ipsius plist quoque
         * iter facere, sed signatura probata nullum locum PlistStatus
         * habet - semita elementi in sede vehitur, numerus non. */
        {
            Fasciculum lectus;

            CREDO_VERUM(filum_directorium_creare_cum_parentibus(
                via_contents));
            CREDO_VERUM(filum_scribere_literis(via_plistae,
                "<?xml version=\"1.0\"?>\n<plist><dict><key>"));
            CREDO_FALSUM(fasciculum_legere(area, &lectus, &status,
                                           &sedes, piscina, intern));
            CREDO_AEQUALIS_I32((i32)status,
                               (i32)FASCICULUM_ERROR_PLISTA);
            CREDO_CHORDA_CONTINET(sedes,
                chorda_ex_literis("Contents/Info.plist", piscina));
        }

        /* (vi) exsecutabile quod CFBundleExecutable nominat ABEST:
         * fasciculus qui duplici ictu taceret */
        {
            Fasciculum lectus;

            f = _fasciculum_minimum(via_binarii, piscina);
            CREDO_VERUM(fasciculum_reddere(&f, &partes, &status,
                                           &sedes, piscina));
            CREDO_VERUM(fasciculum_scribere(partes, area, &status,
                                            &sedes, piscina));
            CREDO_VERUM(filum_delere(via_exs));
            CREDO_FALSUM(fasciculum_legere(area, &lectus, &status,
                                           &sedes, piscina, intern));
            CREDO_AEQUALIS_I32((i32)status,
                               (i32)FASCICULUM_ERROR_EXSECUTABILE);
            CREDO_CHORDA_CONTINET(sedes,
                chorda_ex_literis("probatio_fasciculum_binarium",
                                  piscina));
        }

        filum_delere(via_plistae);
        filum_delere(via_exs);
        filum_delere(via_macos);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
    }

    /* ---- H5: iter reditus, et aequalitas EFFECTIVA ---- */
    {
              Fasciculum  f;
              Fasciculum  lectus;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;
      constans character* via_iconis =
          "/tmp/probatio_fasciculum_h5_icon.icns";
     constans character* radix_areae =
         "/tmp/probatio_fasciculum_iter";
     constans character* area =
         "/tmp/probatio_fasciculum_iter/Probatio.app";
               character via_contents[CCLVI];
               character via_macos[CCLVI];
               character via_res[CCLVI];
               character via_plistae[CCLVI];
               character via_exs[CCLVI];
               character via_icon_posita[CCLVI];

        imprimere("\n--- H5: iter reditus ---\n");

        sprintf(via_contents, "%.200s/Contents", area);
        sprintf(via_macos, "%.200s/Contents/MacOS", area);
        sprintf(via_res, "%.200s/Contents/Resources", area);
        sprintf(via_plistae, "%.200s/Contents/Info.plist", area);
        sprintf(via_exs,
            "%.200s/Contents/MacOS/probatio_fasciculum_binarium", area);
        sprintf(via_icon_posita,
            "%.200s/Contents/Resources/"
            "probatio_fasciculum_h5_icon.icns",
            area);

        filum_delere(via_plistae);
        filum_delere(via_exs);
        filum_delere(via_icon_posita);
        filum_delere(via_macos);
        filum_delere(via_res);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(radix_areae);
        CREDO_VERUM(filum_scribere_literis(via_iconis, "icns"));

        /* VALOR PLENE DICTUS: iter exactum est solum ubi nihil
         * ordinarium supponitur (vide aequalitatem effectivam infra) */
        f = _fasciculum_minimum(via_binarii, piscina);
        f.versio = chorda_ex_literis("3.1", piscina);
        f.versio_aedificationis = chorda_ex_literis("42", piscina);
        f.sine_scandali = VERUM;
        f.icon = via_iconis;
        f.plista_extra = plist_dictio_creare(piscina);
        plist_dictio_ponere(f.plista_extra,
            chorda_ex_literis("NSAppleScriptEnabled", piscina),
            plist_veritatem_creare(VERUM, piscina), piscina);

        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));
        CREDO_VERUM(fasciculum_scribere(partes, area, &status, &sedes,
                                        piscina));
        CREDO_VERUM(fasciculum_legere(area, &lectus, &status, &sedes,
                                      piscina, intern));
        CREDO_AEQUALIS_I32((i32)status, (i32)FASCICULUM_SUCCESSUS);

        /* campi singuli ANTE aequalitatem, ut defectus se nominet */
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
                                      "org.rhubarb.probatio");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.titulus, "Probatio");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.versio, "3.1");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.versio_aedificationis,
                                      "42");
        CREDO_VERUM(lectus.sine_scandali);
        /* vias SUAS novit, non fontium: nomina sola conferuntur */
        CREDO_NON_NIHIL(lectus.exsecutabile);
        CREDO_NON_NIHIL(lectus.icon);

        CREDO_VERUM(fasciculum_aequalis(&f, &lectus));

        /* AEQUALITAS EFFECTIVA: valor minimus (versio vacua,
         * plista_extra NIHIL) aequalis est fasciculo qui '1.0', '1' et
         * dictionem vacuam fert - aliter scriptor fasciculum iam
         * rectum semper rescriberet, quod huius functionis causa est */
        {
                  Fasciculum  minimus;
                  Fasciculum  lectus_minimi;
                         Xar* partes_minimi;
          constans character* area_minimi =
              "/tmp/probatio_fasciculum_iter/Minimus.app";

            minimus = _fasciculum_minimum(via_binarii, piscina);
            CREDO_VERUM(fasciculum_reddere(&minimus, &partes_minimi,
                                           &status, &sedes, piscina));
            CREDO_VERUM(fasciculum_scribere(partes_minimi, area_minimi,
                                            &status, &sedes, piscina));
            CREDO_VERUM(fasciculum_legere(area_minimi, &lectus_minimi,
                                          &status, &sedes, piscina,
                                          intern));
            CREDO_CHORDA_AEQUALIS_LITERIS(lectus_minimi.versio, "1.0");
            CREDO_CHORDA_AEQUALIS_LITERIS(
                lectus_minimi.versio_aedificationis, "1");
            CREDO_VERUM(chorda_vacua(minimus.versio));
            CREDO_NIHIL(minimus.plista_extra);
            CREDO_VERUM(fasciculum_aequalis(&minimus, &lectus_minimi));
        }

        /* DISSIMILITUDO vera deprehenditur, ne aequalitas semper
         * VERUM reddat */
        {
            Fasciculum alius;

            alius            = lectus;
            alius.identitas  = chorda_ex_literis("org.aliud", piscina);
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius          = lectus;
            alius.titulus  = chorda_ex_literis("Aliud", piscina);
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius         = lectus;
            alius.versio  = chorda_ex_literis("9.9", piscina);
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius = lectus;
            alius.versio_aedificationis =
                chorda_ex_literis("99", piscina);
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius                = lectus;
            alius.sine_scandali  = FALSUM;
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius               = lectus;
            alius.exsecutabile  = "/alibi/nomen_aliud";
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius       = lectus;
            alius.icon  = "/alibi/icon_aliud.icns";
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));

            alius               = lectus;
            alius.plista_extra  = plist_dictio_creare(piscina);
            CREDO_FALSUM(fasciculum_aequalis(&f, &alius));
        }

        /* exsecutabile SUBLATUM: lector RECUSAT */
        CREDO_VERUM(filum_delere(via_exs));
        CREDO_FALSUM(fasciculum_legere(area, &lectus, &status, &sedes,
                                       piscina, intern));
        CREDO_AEQUALIS_I32((i32)status,
                           (i32)FASCICULUM_ERROR_EXSECUTABILE);

        filum_delere(via_plistae);
        filum_delere(via_icon_posita);
        filum_delere(via_macos);
        filum_delere(via_res);
        filum_delere(via_contents);
        filum_delere(area);
        filum_delere(via_iconis);
    }

    filum_delere(via_binarii);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
