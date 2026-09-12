/* probatio_plist.c - modellum valoris, scriptor, lector, iter reditus
 *
 * Portae G1-G5 et G7 specificationis (project-specs/plist-spec.md
 * par. VII). Oraculum alienum (plutil) in
 * probationes/probatio_plist_plutil.sh vivit - porta G6.
 */

#include "latina.h"
#include "plist.h"
#include "credo.h"
#include "piscina.h"
#include "internamentum.h"

#include <stdio.h>
#include <string.h>

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_plist", 1048576);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        imprimere("FRACTA: internamentum\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- G1: ordo dictionis servatus, ponere substituit ---- */
    {
        PlistValor* d;
          PlistPar* par;

        imprimere("\n--- G1: modellum valoris ---\n");

        d = plist_dictio_creare(piscina);
        CREDO_NON_NIHIL(d);
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("alpha", piscina),
            plist_integrum_creare(1, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("beta", piscina),
            plist_integrum_creare(2, piscina), piscina));
        CREDO_AEQUALIS_I32(plist_numerus(d), II);

        /* clavis exsistens SUBSTITUITUR in loco: numerus idem, ordo
         * idem, valor novus */
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("alpha", piscina),
            plist_integrum_creare(3, piscina), piscina));
        CREDO_AEQUALIS_I32(plist_numerus(d), II);
        par = (PlistPar*)xar_obtinere(d->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(par);
        CREDO_CHORDA_AEQUALIS_LITERIS(par->clavis, "alpha");
        CREDO_AEQUALIS_S64(par->valor->integrum, 3);

        CREDO_NON_NIHIL(plist_dictio_capere(d, "beta"));
        CREDO_NIHIL(plist_dictio_capere(d, "gamma"));
        CREDO_AEQUALIS_I32(
            plist_numerus(plist_chordam_creare(
                chorda_ex_literis("x", piscina), piscina)),
            ZEPHYRUM);
    }

    /* ---- G3: scriptor - octeti, determinismus, genera omnia ---- */
    {
         PlistValor* d;
         PlistValor* lista;
        PlistStatus  status;
             chorda  prima;
             chorda  secunda;
           DiesHora  dh;
                 i8  octeti[III];

        imprimere("\n--- G3: scriptor ---\n");

        dh.dies    = fasti_dies(2026, 9, 11);
        dh.hora    = fasti_hora(2, 41, 32);
        octeti[0]  = (i8)0x00;
        octeti[1]  = (i8)0x01;
        octeti[2]  = (i8)0xFF;

        lista = plist_lista_creare(piscina);
        CREDO_VERUM(plist_lista_addere(lista,
            plist_veritatem_creare(VERUM, piscina), piscina));
        CREDO_VERUM(plist_lista_addere(lista,
            plist_veritatem_creare(FALSUM, piscina), piscina));

        d = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Titulus", piscina),
            plist_chordam_creare(
                chorda_ex_literis("a < b & c", piscina), piscina),
            piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Numerus", piscina),
            plist_integrum_creare(-9223372036854775807LL - 1LL,
                piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Realis", piscina),
            plist_realem_creare(1e300, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Dies", piscina),
            plist_diem_creare(dh, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Data", piscina),
            plist_data_creare(chorda_ex_buffer(octeti, III), piscina),
            piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Lista", piscina), lista, piscina));

        prima = plist_scribere(d, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        CREDO_CHORDA_NON_VACUA(prima);

        /* prologus et declaratio generis ASSERUNTUR: plutil utrumque
         * ignoscit, ergo silentium eius hic nihil probat */
        CREDO_CHORDA_INCIPIT(prima, chorda_ex_literis(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n",
            piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<plist version=\"1.0\">", piscina));
        /* escapatio a strato XML venit, non hinc */
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("a &lt; b &amp; c", piscina));
        CREDO_CHORDA_CONTINET(prima, chorda_ex_literis(
            "<integer>-9223372036854775808</integer>", piscina));
        CREDO_CHORDA_CONTINET(prima, chorda_ex_literis(
            "<date>2026-09-11T02:41:32Z</date>", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<data>AAH/</data>", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<true/>", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<false/>", piscina));
        /* realis: forma EXACTA, non decimalis fixa */
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("e+300</real>", piscina));

        /* DETERMINISMUS: idem valor, idem octeti */
        secunda = plist_scribere(d, &status, piscina);
        CREDO_CHORDA_AEQUALIS(prima, secunda);
    }

    /* ---- G7 (pars scribendi): signum C0 RECUSATUR ---- */
    {
         PlistValor* malus;
        PlistStatus  status;
             chorda  nihil_scriptum;
                 i8  cum_signo[II];

        imprimere("\n--- G7: signum C0 in textu ---\n");

        cum_signo[0] = (i8)'a';
        cum_signo[1] = (i8)0x01;

        malus = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(malus,
            chorda_ex_literis("x", piscina),
            plist_chordam_creare(chorda_ex_buffer(cum_signo, II),
                piscina), piscina));
        nihil_scriptum = plist_scribere(malus, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_ERROR_SIGNUM);
        CREDO_CHORDA_VACUA(nihil_scriptum);

        /* idem in CLAVE, non solum in valore */
        malus = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(malus,
            chorda_ex_buffer(cum_signo, II),
            plist_chordam_creare(chorda_ex_literis("y", piscina),
                piscina), piscina));
        nihil_scriptum = plist_scribere(malus, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_ERROR_SIGNUM);
        CREDO_CHORDA_VACUA(nihil_scriptum);
    }

    /* ---- G2: lector, unum genus per casum ---- */
    {
             PlistResultus  r;
                PlistValor* v;
        constans character* textus =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict>\n"
            "  <key>S</key><string>a &lt; b</string>\n"
            "  <key>I</key><integer>-42</integer>\n"
            "  <key>R</key><real>0.5</real>\n"
            "  <key>V</key><true/>\n"
            "  <key>D</key><date>2026-09-11T02:41:32Z</date>\n"
            "  <key>B</key><data>AAH/</data>\n"
            "  <key>L</key><array><integer>1</integer>"
            "<integer>2</integer></array>\n"
            "</dict></plist>\n";

        imprimere("\n--- G2: lector ---\n");

        r = plist_legere(chorda_ex_literis(textus, piscina), piscina,
                         intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);
        CREDO_AEQUALIS_I32(plist_numerus(r.radix), VII);

        v = plist_dictio_capere(r.radix, "S");
        CREDO_NON_NIHIL(v);
        CREDO_AEQUALIS_I32((i32)v->genus, (i32)PLIST_CHORDA);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "a < b");

        v = plist_dictio_capere(r.radix, "I");
        CREDO_AEQUALIS_S64(v->integrum, -42);
        v = plist_dictio_capere(r.radix, "R");
        CREDO_VERUM(v->realis == 0.5);
        v = plist_dictio_capere(r.radix, "V");
        CREDO_VERUM(v->veritas);
        v = plist_dictio_capere(r.radix, "D");
        CREDO_AEQUALIS_S32(v->dies.dies.annus, 2026);
        CREDO_AEQUALIS_S32(v->dies.hora.minutum, 41);
        v = plist_dictio_capere(r.radix, "B");
        CREDO_AEQUALIS_I32(v->textus.mensura, III);
        CREDO_AEQUALIS_I32((i32)(i8)v->textus.datum[II], 255);
        v = plist_dictio_capere(r.radix, "L");
        CREDO_AEQUALIS_I32(plist_numerus(v), II);
        CREDO_AEQUALIS_S64(plist_ad_indicem(v, I)->integrum, 2);
    }

    /* ---- G2: data ut Apple eam scribit (per lineas fracta) et
     * data VACUA (nulli octeti licent) ---- */
    {
             PlistResultus  r;
                PlistValor* v;
        constans character* fracta =
            "<plist version=\"1.0\"><dict>\n"
            "\t<key>B</key>\n"
            "\t<data>\n"
            "\tAAECAwQF\n"
            "\t</data>\n"
            "\t<key>V</key>\n"
            "\t<data></data>\n"
            "</dict></plist>";

        imprimere("\n--- G2: data fracta et vacua ---\n");

        r = plist_legere(chorda_ex_literis(fracta, piscina), piscina,
                         intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);

        v = plist_dictio_capere(r.radix, "B");
        CREDO_NON_NIHIL(v);
        CREDO_AEQUALIS_I32((i32)v->genus, (i32)PLIST_DATA);
        CREDO_AEQUALIS_I32(v->textus.mensura, VI);
        CREDO_AEQUALIS_I32((i32)(i8)v->textus.datum[ZEPHYRUM], 0);
        CREDO_AEQUALIS_I32((i32)(i8)v->textus.datum[V], 5);

        v = plist_dictio_capere(r.radix, "V");
        CREDO_NON_NIHIL(v);
        CREDO_AEQUALIS_I32((i32)v->genus, (i32)PLIST_DATA);
        CREDO_AEQUALIS_I32(v->textus.mensura, ZEPHYRUM);
    }

    /* ---- G2: recusationes, quaeque per statum et semitam ---- */
    {
             PlistResultus  r;
                       i32  i;
        constans character* mala[VIII];
               PlistStatus  exspectati[VIII];

        imprimere("\n--- G2: recusationes nominatae ---\n");

        mala[0]        = "<dict/>";
        exspectati[0]  = PLIST_ERROR_NON_PLIST;
        mala[1]        = "<plist version=\"2.0\"><dict/></plist>";
        exspectati[1]  = PLIST_ERROR_VERSIO;
        mala[2]        = "bplist00\001\002";
        exspectati[2]  = PLIST_ERROR_BINARIUM;
        mala[3]       = "<plist version=\"1.0\"><dict>"
                        "<key>a</key></dict></plist>";
        exspectati[3] = PLIST_ERROR_STRUCTURA;
        mala[4]       = "<plist version=\"1.0\"><dict>"
                        "<string>sine clave</string></dict></plist>";
        exspectati[4]  = PLIST_ERROR_STRUCTURA;
        mala[5]        = "<plist version=\"1.0\"><ignotum/></plist>";
        exspectati[5]  = PLIST_ERROR_STRUCTURA;
        mala[6]       = "<plist version=\"1.0\">"
                        "<data>!! non 64 !!</data></plist>";
        exspectati[6] = PLIST_ERROR_BASE64;
        mala[7]       = "<plist version=\"1.0\"><integer>"
                        "99999999999999999999</integer></plist>";
        exspectati[7] = PLIST_ERROR_NUMERUS;

        per (i = ZEPHYRUM; i < VIII; i++)
        {
            r = plist_legere(chorda_ex_literis(mala[i], piscina),
                             piscina, intern);
            CREDO_AEQUALIS_I32((i32)r.status, (i32)exspectati[i]);
            CREDO_NIHIL(r.radix);
            CREDO_CHORDA_NON_VACUA(r.semita);
        }

        /* dies mala, radices plures, textus non albus in continente */
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><date>heri</date></plist>",
            piscina), piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_DIES);
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><dict/><dict/></plist>", piscina),
            piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_STRUCTURA);
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><dict>sordes<key>a</key>"
            "<string>b</string></dict></plist>", piscina), piscina,
            intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_STRUCTURA);
    }

    /* ---- G4: iter reditus nostrum (valor -> octeti -> valor) ---- */
    {
         PlistValor* d;
         PlistValor* lista;
        PlistStatus  status;
      PlistResultus  r;
             chorda  octeti;
           DiesHora  dh;
                 i8  data[II];

        imprimere("\n--- G4: iter reditus ---\n");

        dh.dies = fasti_dies(1978, 2, 3);
        dh.hora = fasti_hora(23, 59, 58);
        data[0] = (i8)0x10;
        data[1] = (i8)0x20;

        lista = plist_lista_creare(piscina);
        plist_lista_addere(lista,
            plist_realem_creare(1e-20, piscina), piscina);
        plist_lista_addere(lista,
            plist_data_creare(chorda_ex_buffer(data, II), piscina),
            piscina);

        d = plist_dictio_creare(piscina);
        plist_dictio_ponere(d, chorda_ex_literis("a", piscina),
            plist_chordam_creare(
                chorda_ex_literis("&<>\"'", piscina), piscina),
            piscina);
        plist_dictio_ponere(d, chorda_ex_literis("b", piscina),
            plist_integrum_creare(9223372036854775807LL, piscina),
            piscina);
        plist_dictio_ponere(d, chorda_ex_literis("c", piscina),
            plist_diem_creare(dh, piscina), piscina);
        plist_dictio_ponere(d, chorda_ex_literis("d", piscina),
            lista, piscina);

        octeti = plist_scribere(d, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        r = plist_legere(octeti, piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_VERUM(plist_aequalis(d, r.radix));

        /* aequalitas ORDINEM curat: eaedem claves, ordo inversus */
        {
            PlistValor* e = plist_dictio_creare(piscina);
            PlistValor* f = plist_dictio_creare(piscina);

            plist_dictio_ponere(e, chorda_ex_literis("y", piscina),
                plist_integrum_creare(2, piscina), piscina);
            plist_dictio_ponere(e, chorda_ex_literis("x", piscina),
                plist_integrum_creare(1, piscina), piscina);
            plist_dictio_ponere(f, chorda_ex_literis("x", piscina),
                plist_integrum_creare(1, piscina), piscina);
            plist_dictio_ponere(f, chorda_ex_literis("y", piscina),
                plist_integrum_creare(2, piscina), piscina);
            CREDO_FALSUM(plist_aequalis(e, f));
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
