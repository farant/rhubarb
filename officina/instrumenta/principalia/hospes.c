/* hospes.c - The first true host of the officina deliverable.
 *
 * VANILLA C89 ON PURPOSE: includes ONLY officina.h + the standard
 * library. Proves (M1a C):
 *   1. HOST POLLUTION: variables named after latina keyword macros
 *      compile and WORK - one leaked define and this does not build.
 *   2. EQUIVALENCE SMOKE: the amalgam's fabrica + scriptor produce
 *      the exact golden text, the lector round-trips it, and every
 *      public row is CALLED (standing rule).
 */
#include <stdio.h>
#include <string.h>
#include "officina.h"
#include "silva.h"   /* hospes duplex: demissio silvam consumit
                      * (saltuarius exemplar) */

/* --- latina keyword names as ordinary identifiers --- */
static int si = 1;
static double per = 2.0;
static char character = 'c';
static int nomen = 3;
static int magnitudo = 4;
static int redde = 5;
static int dum = 6;
static int vacuum = 7;

static int fideles = 0;
static int summa = 0;

static void proba(int conditio, const char* titulus)
{
    summa++;
    if (conditio) {
        fideles++;
    } else {
        fprintf(stderr, "hospes: INFIDELIS: %s\n", titulus);
    }
}

static OfficinaChorda ch(const char* literis)
{
    OfficinaChorda c;
    union { const char* c; unsigned char* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (unsigned int)strlen(literis);
    return c;
}

static MedullaInstructio instr(int op, int typus, int destinatio,
    MedullaOperandum a, MedullaOperandum b)
{
    MedullaInstructio i;

    memset(&i, 0, sizeof(MedullaInstructio));
    i.op = op;
    i.typus = typus;
    i.typus_secundus = MEDULLA_TYPUS_NIHIL;
    i.destinatio = destinatio;
    i.extra_index = -1;
    i.a = a;
    i.b = b;
    i.c = medulla_op_nihil();
    return i;
}

static const char* AUREUM =
    "modulus \"hospes.c\"\n"
    "\n"
    "functio $probare (s32 %n) -> s32\n"
    "@initium:\n"
    "    %d = addere.s32 %n, 1\n"
    "    %r = vocare.s32 $externa, %d, 2.5\n"
    "    salire @exitus\n"
    "@exitus:\n"
    "    redde %r\n"
    "@numquam:\n"
    "    sistere \"hospes causa\"\n"
    "\n"
    "datum $tabula magnitudo 8 ordinatio 8\n"
    "    octeti 01020304 05060708\n"
    "    relocatio 0 $externa\n";

int main(void)
{
    OfficinaPiscina* piscina;
    MedullaModulus* modulus;
    MedullaFunctio* functio;
    int r_n;
    int r_d;
    int r_r;
    int b_initium;
    int b_exitus;
    int b_numquam;
    int s_externa;
    int causa;
    MedullaDatum* tabula;
    MedullaInstructio instructio;

    /* the polluted names must actually work */
    si += nomen;
    per += (double)magnitudo;
    character = (char)(character + 1);
    redde += dum + vacuum;
    if (si != 4 || character != 'd' || redde != 18) {
        fprintf(stderr, "hospes: nomina latina corrupta?!\n");
        return 1;
    }

    piscina = officina_piscina_generare_dynamicum("hospes", 8388608);
    if (piscina == NULL) {
        fprintf(stderr, "hospes: piscina deest\n");
        return 1;
    }

    /* fabrica: $probare + $tabula (omnis functio publica vocatur) */
    modulus = medulla_modulum_creare(piscina, ch("hospes.c"));
    proba(modulus != NULL, "modulus");
    functio = medulla_functionem_creare(modulus, ch("probare"),
        MEDULLA_TYPUS_S32, 0);
    proba(functio != NULL, "functio");
    r_n = medulla_parametrum_addere(functio, ch("n"),
        MEDULLA_TYPUS_S32);
    r_d = medulla_registrum_novum(functio, ch("d"));
    r_r = medulla_registrum_novum(functio, ch("r"));
    b_initium = medulla_bloccum_creare(functio, ch("initium"));
    b_exitus = medulla_bloccum_creare(functio, ch("exitus"));
    b_numquam = medulla_bloccum_creare(functio, ch("numquam"));
    s_externa = medulla_symbolum_internare(modulus, ch("externa"));
    causa = medulla_causam_internare(modulus, ch("hospes causa"));
    proba(r_n == 0 && r_d == 1 && r_r == 2 && b_initium == 0
        && s_externa >= 0 && causa >= 0, "indices");

    instructio = instr(MEDULLA_OP_ADDERE, MEDULLA_TYPUS_S32, r_d,
        medulla_op_registrum(r_n), medulla_op_immediatum(1));
    proba(medulla_emittere(functio, b_initium, &instructio),
        "addere");
    {
        MedullaOperandum argumenta[2];

        argumenta[0] = medulla_op_registrum(r_d);
        argumenta[1] = medulla_op_immediatum_f(2.5);
        instructio = instr(MEDULLA_OP_VOCARE, MEDULLA_TYPUS_S32,
            r_r, medulla_op_symbolum(s_externa), medulla_op_nihil());
        instructio.extra_index = medulla_operanda_addere(functio,
            argumenta, 2);
        instructio.extra_numerus = 2;
        proba(instructio.extra_index == 0, "stiva");
        proba(medulla_emittere(functio, b_initium, &instructio),
            "vocare");
    }
    instructio = instr(MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL, -1,
        medulla_op_bloccum(b_exitus), medulla_op_nihil());
    proba(medulla_emittere(functio, b_initium, &instructio),
        "salire");
    instructio = instr(MEDULLA_OP_REDDE, MEDULLA_TYPUS_NIHIL, -1,
        medulla_op_registrum(r_r), medulla_op_nihil());
    proba(medulla_emittere(functio, b_exitus, &instructio), "redde");
    instructio = instr(MEDULLA_OP_SISTERE, MEDULLA_TYPUS_NIHIL, -1,
        medulla_op_causa(causa), medulla_op_nihil());
    proba(medulla_emittere(functio, b_numquam, &instructio),
        "sistere");
    proba(medulla_functio_terminata(functio), "terminata");
    proba(medulla_op_terminator(MEDULLA_OP_REDDE)
        && !medulla_op_terminator(MEDULLA_OP_ADDERE), "terminator");

    tabula = medulla_datum_creare(modulus, ch("tabula"), 8, 8);
    proba(tabula != NULL, "datum");
    {
        static const unsigned char octeti[8] =
            { 1, 2, 3, 4, 5, 6, 7, 8 };

        proba(medulla_datum_scribere(tabula, 0, octeti, 8),
            "scribere octeti");
        proba(medulla_relocationem_addere(tabula, 0, s_externa),
            "relocatio");
    }

    /* queries through the public surface */
    proba(officina_xar_numerus(functio->blocci) == 3, "xar numerus");
    {
        const MedullaBloccus* bloccus =
            medulla_bloccum_obtinere(functio, b_exitus);
        const MedullaInstructio* prima;

        proba(bloccus != NULL && bloccus->terminatus, "bloccus");
        prima = (const MedullaInstructio*)officina_xar_obtinere(
            bloccus->instructiones, 0);
        proba(prima != NULL && prima->op == MEDULLA_OP_REDDE,
            "xar obtinere");
        proba(officina_xar_obtinere_s(bloccus->instructiones, -1)
            == NULL, "xar obtinere_s");
    }
    {
        const OfficinaChorda* titulus =
            medulla_registrum_titulus(functio, r_d);

        proba(titulus != NULL && titulus->mensura == 1
            && titulus->datum[0] == 'd', "registrum titulus");
    }
    {
        const MedullaSymbolum* symbolum =
            medulla_symbolum_obtinere(modulus,
                medulla_symbolum_internare(modulus, ch("probare")));

        proba(symbolum != NULL
            && symbolum->genus == MEDULLA_SYMBOLUM_FUNCTIO,
            "symbolum genus");
    }
    {
        const OfficinaChorda* textus_causae =
            medulla_causam_obtinere(modulus, causa);

        proba(textus_causae != NULL && textus_causae->mensura == 12,
            "causa");
    }
    proba(strcmp(medulla_op_titulus(MEDULLA_OP_AUT), "aut") == 0
        && strcmp(medulla_typum_titulus(MEDULLA_TYPUS_F64), "f64")
            == 0, "nomina");

    /* golden text + round-trip through the public surface */
    {
        OfficinaChorda scriptura = medulla_textum_scribere(piscina,
            modulus);
        unsigned int linea = 0;
        MedullaModulus* iterum;
        OfficinaChorda secunda;

        proba(scriptura.mensura == (unsigned int)strlen(AUREUM)
            && memcmp(scriptura.datum, AUREUM, scriptura.mensura)
                == 0, "scriptura aurea");
        iterum = medulla_textum_legere(piscina, scriptura, &linea);
        proba(iterum != NULL && linea == 0, "lectio");
        secunda = medulla_textum_scribere(piscina, iterum);
        proba(secunda.mensura == scriptura.mensura
            && memcmp(secunda.datum, scriptura.datum,
                   secunda.mensura) == 0, "vectis byte-idem");
    }

    /* demissio per superficiem publicam DUPLICEM (silva.h +
     * officina.h in hospite uno) */
    {
        SilvaPiscina* piscina_silvae =
            silva_piscina_generare_dynamicum("hospes_silva",
                8388608);
        static const char fons_dem[] =
            "int addens(int x) { return x + 1; }\n";
        static const char* SPERATUM_DEM =
            "modulus \"hospes_dem.c\"\n"
            "\n"
            "functio $addens (s32 %x) -> s32\n"
            "@initium_0:\n"
            "    %t1 = addere.s32 %x, 1\n"
            "    redde %t1\n";
        SilvaParsura* parsura = NULL;
        SilvaSemantica* sem = NULL;
        MedullaModulus* modulus_dem = NULL;

        if (piscina_silvae != NULL) {
            parsura = silva_c89_parsare(piscina_silvae,
                "hospes_dem.c", fons_dem,
                (unsigned int)(sizeof(fons_dem) - 1), NULL);
        }
        if (parsura != NULL && parsura->numerus_errorum == 0) {
            sem = silva_c89_semantica_analysare(piscina_silvae,
                parsura);
        }
        if (sem != NULL) {
            modulus_dem = demissio_currere(piscina, parsura, sem,
                ch("hospes_dem.c"));
        }
        proba(modulus_dem != NULL, "demissio");
        if (modulus_dem != NULL) {
            OfficinaChorda scriptura = medulla_textum_scribere(
                piscina, modulus_dem);

            proba(scriptura.mensura
                    == (unsigned int)strlen(SPERATUM_DEM)
                && memcmp(scriptura.datum, SPERATUM_DEM,
                       scriptura.mensura) == 0, "demissio aurea");
            proba(officina_xar_numerus(modulus_dem->causae) == 0,
                "demissio sine sistere");
        }
        /* distillatio linearum (M2a): dum frons vivit */
        if (modulus_dem != NULL && parsura != NULL) {
            MedullaLineae* lineae = demissio_lineas_colligere(
                piscina, modulus_dem, parsura);
            MedullaFunctio** fd = (MedullaFunctio**)
                officina_xar_obtinere(modulus_dem->functiones, 0);
            const MedullaBloccus* bd;
            const MedullaInstructio* id;
            OfficinaChorda via;
            unsigned int linea = 0;

            proba(lineae != NULL, "lineas colligere");
            bd = medulla_bloccum_obtinere(*fd, 0);
            id = (const MedullaInstructio*)officina_xar_obtinere(
                bd->instructiones, 0);
            proba(lineae != NULL && id->origo != NULL
                && medulla_lineam_quaerere(lineae, id->origo, &via,
                       &linea)
                && linea > 0, "lineam quaerere colligata");
        }
        /* M3: indicium per superficiem publicam (coque -> scribe
         * -> reclude -> quaere; pactum ambulationis) */
        if (modulus_dem != NULL && parsura != NULL) {
            IndiciumScriptor* scriptor =
                indicium_scriptor_creare(piscina);
            Regio* regio_i = regio_generare(piscina);
            Conexio* conexio_i = conexio_creare(piscina, regio_i);
            Machinula* machinula_i;
            const char* via_i = "officina/build/hospes.indicium";

            proba(scriptor != NULL, "indicium scriptor");
            proba(indicium_modulum_colligere(scriptor, modulus_dem,
                parsura, sem), "indicium colligere");
            proba(conexio_i != NULL
                && conexio_modulum_addere(conexio_i, modulus_dem)
                && conexio_nectere(conexio_i), "indicium conexio");
            machinula_i = machinula_creare(piscina, conexio_i,
                regio_i);
            proba(machinula_i != NULL, "indicium machinula");
            proba(indicium_scribere(scriptor, conexio_i, via_i),
                "indicium scribere");
            {
                IndiciumLector* lector = indicium_aperire(piscina,
                    via_i);
                int f = -1;

                proba(lector != NULL, "indicium aperire");
                if (lector != NULL) {
                    f = indicium_functionem_quaerere(lector,
                        ch("addens"));
                }
                proba(f >= 0, "indicium quaerere");
                proba(lector != NULL
                    && indicium_functiones_numerus(lector) > 0,
                    "indicium numerus");
                if (lector != NULL && f >= 0) {
                    const IndiciumFunctio* ifu = indicium_functio(
                        lector, (unsigned int)f);
                    const IndiciumLinea* acies = NULL;
                    const IndiciumSitus* situs = NULL;
                    const IndiciumVariabile* varia = NULL;

                    proba(ifu != NULL
                        && ifu->instructiones_numerus
                            == machinula_numerus_instructionum_planarum(
                                   machinula_i, f),
                        "indicium pactum ambulationis");
                    proba(indicium_lineas_de_instructione(lector,
                            (unsigned int)f, 0, &acies) > 0
                        && acies != NULL && acies[0].linea == 1
                        && acies[0].profunditas == 0,
                        "indicium prorsum");
                    proba(indicium_situs_de_linea(lector,
                            ch("hospes_dem.c"), 1, &situs) > 0
                        && situs != NULL,
                        "indicium retro");
                    proba(indicium_variabilia_functionis(lector,
                            (unsigned int)f, &varia) > 0
                        && varia != NULL
                        && indicium_chorda(lector,
                               varia[0].titulus).mensura == 1,
                        "indicium variabilia");
                }
            }
            if (regio_i != NULL) {
                regio_destruere(regio_i);
            }
        }
        if (piscina_silvae != NULL) {
            silva_piscina_destruere(piscina_silvae);
        }
    }

    /* M2a: regio + conexio per superficiem publicam */
    {
        Regio* regio = regio_generare(piscina);

        proba(regio != NULL, "regio generare");
        if (regio != NULL) {
            void* globale;
            void* locus;

            proba(regio_basis(regio) != NULL, "regio basis");
            proba(regio_magnitudo_tota(regio) > 0, "regio tota");
            proba(regio_continet(regio, regio_basis(regio)),
                "regio continet");
            regio_custodiam_ponere(regio, 1);
            proba(regio_custodia(regio) == 1, "regio custodia");
            globale = regio_globalia_allocare(regio, 16, 16);
            proba(globale != NULL
                && regio_globalia_usus(regio) >= 16,
                "regio globalia");
            proba(regio_stiva_initium(regio) != NULL
                && regio_stiva_magnitudo_octetorum(regio) > 0,
                "regio stiva");
            locus = regio_allocare(regio, 32);
            proba(locus != NULL, "regio allocare");
            locus = regio_reallocare(regio, locus, 64);
            proba(locus != NULL, "regio reallocare");
            proba(regio_liberare(regio, locus), "regio liberare");
            proba(regio_acervus_usus(regio) == 0
                && regio_acervus_apex(regio) >= 64
                && regio_numerus_allocationum(regio) == 2
                && regio_numerus_liberationum(regio) == 2,
                "regio census");

            {
                Conexio* conexio = conexio_creare(piscina, regio);

                proba(conexio != NULL, "conexio creare");
                proba(conexio_modulum_addere(conexio, modulus),
                    "conexio addere");
                proba(conexio_nectere(conexio), "conexio nectere");
                proba(conexio_querela(conexio) != NULL
                    && conexio_querela_symbolum(conexio) != NULL,
                    "conexio querelae");
                proba(conexio_numerus_modulorum(conexio) == 1
                    && conexio_modulum_obtinere(conexio, 0)
                        == modulus, "conexio moduli");
                proba(conexio_numerus_symbolorum(conexio) == 3,
                    "conexio symbola");
                {
                    int idx = conexio_symbolum_quaerere(conexio,
                        ch("probare"));
                    const ConexioSymbolum* symbolum =
                        conexio_symbolum_obtinere(conexio, idx);
                    const ConexioDescriptor* descriptor;
                    const ConexioFunctioNexa* nexa;

                    proba(idx >= 0 && symbolum != NULL
                        && symbolum->genus
                            == CONEXIO_SYMBOLUM_FUNCTIO,
                        "conexio symbolum");
                    descriptor = (const ConexioDescriptor*)
                        conexio_sedes_quaerere(conexio,
                            ch("probare"));
                    proba(descriptor != NULL
                        && descriptor->signum
                            == CONEXIO_SIGNUM_INTERPRETATUM,
                        "conexio descriptor");
                    nexa = conexio_functionem_obtinere(conexio,
                        descriptor->index);
                    proba(nexa != NULL && nexa->functio == functio
                        && nexa->modulus_index == 0,
                        "conexio nexa");
                    proba(conexio_symbolum_globale(conexio, 0,
                            s_externa) >= 0, "conexio globale");
                }
                proba(conexio_numerus_functionum(conexio) == 1
                    && conexio_numerus_datorum(conexio) == 1
                    && conexio_numerus_cellarum(conexio) == 0
                    && conexio_numerus_decipularum(conexio) == 1,
                    "conexio census");
                proba(conexio_decipulam_obtinere(conexio, 0)
                    != NULL, "conexio decipula");
                /* relocatio ADDITIVA: locellus = sedes decipulae
                 * externae + addendum (octeti 01..08 LE) */
                {
                    const unsigned char* tab =
                        (const unsigned char*)conexio_sedes_quaerere(
                            conexio, ch("tabula"));
                    const void* externa_sedes =
                        conexio_sedes_quaerere(conexio,
                            ch("externa"));
                    long long locellus;

                    memcpy(&locellus, tab, 8);
                    proba(tab != NULL && externa_sedes != NULL
                        && locellus == (long long)(size_t)
                                externa_sedes
                            + 0x0807060504030201LL,
                        "relocatio additiva");
                }
            }
            regio_destruere(regio);
        }
    }

    /* M2b: machinula per superficiem publicam */
    {
        static const char* TEXTUS_M =
            "modulus \"m.medulla\"\n"
            "\n"
            "functio $quadra (s32 %x) -> s32\n"
            "@initium:\n"
            "    %t = multiplicare.s32 %x, %x\n"
            "    redde %t\n"
            "\n"
            "functio $main () -> s32\n"
            "@initium:\n"
            "    %a = vocare.s32 $quadra, 6\n"
            "    %t = addere.s32 %a, 6\n"
            "    redde %t\n";
        OfficinaChorda textus = ch(TEXTUS_M);
        unsigned int linea = 0;
        MedullaModulus* modulus_m = medulla_textum_legere(piscina,
            textus, &linea);
        Regio* regio = regio_generare(piscina);
        Conexio* conexio = conexio_creare(piscina, regio);
        Machinula* machinula;
        MachinulaExitus fructus_m;

        proba(modulus_m != NULL && regio != NULL && conexio != NULL
            && conexio_modulum_addere(conexio, modulus_m)
            && conexio_nectere(conexio), "machinula praeparatio");
        machinula = machinula_creare(piscina, conexio, regio);
        proba(machinula != NULL, "machinula creare");
        machinula_lineas_praebere(machinula, 0, NULL);
        fructus_m = machinula_currere(machinula, ch("main"));
        proba(fructus_m.genus == MACHINULA_BENE
            && fructus_m.codex == 42, "machinula currere");
        proba(machinula_numerus_instructionum(machinula) > 0
            && machinula_numerus_op(machinula,
                   MEDULLA_OP_MULTIPLICARE) == 1
            && machinula_numerus_vocationum(machinula) == 2
            && machinula_numerus_aedificatorum(machinula) == 0
            && machinula_stiva_apex(machinula) > 0,
            "machinula census");
        regio_destruere(regio);
    }

    /* M2a: lineae per claves opacas */
    {
        MedullaLineae* lineae = medulla_lineas_creare(piscina);
        const struct SilvaNodus* origo =
            (const struct SilvaNodus*)(const void*)AUREUM;
        OfficinaChorda via;
        unsigned int linea = 0;

        proba(lineae != NULL, "lineae creare");
        proba(medulla_lineam_ponere(lineae, origo, ch("x.c"), 7),
            "lineam ponere");
        proba(medulla_lineam_quaerere(lineae, origo, &via, &linea)
            && linea == 7 && via.mensura == 3, "lineam quaerere");
    }

    printf("hospes: %d/%d fideles (pollutio nulla)\n", fideles,
        summa);
    officina_piscina_destruere(piscina);
    return (fideles == summa) ? 0 : 1;
}
