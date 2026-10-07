/* probatio_magnus.c - Probationes integrorum magnorum
 *
 * Latera duo: vectores ex integris Pythonis (oraculum INDEPENDENS:
 * summa, differentia, productum, divisio Euclidea, divisor communis),
 * et proprietates algebraicae super numeros fortuitos (sors) - quae
 * sine oraculo valent. Forma canonica post omnem operationem.
 * Vide lib/magnus.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "magnus.h"
#include <stdio.h>

nomen structura {
    constans character* a;
    constans character* b;
    constans character* summa;
    constans character* differentia;
    constans character* productum;
    constans character* quotiens;
    constans character* residuum;
    constans character* divisor_communis;
} VectorMagnus;

/* Ex Pythone (random.seed 20261005); primi duo: Knuth D6 (adde retro)
 * et D3 (aestimatio correcta), ex Hacker's Delight ad XXXII bits. */
interior constans VectorMagnus vectores[] = {
    {
        "170141183460469231750134047781003722752",
        "39614081257132168801066942463",
        "170141183500083313007266216582070665215",
        "170141183420855150493001878979936780289",
        "67399866667876599501282554085753870363107914056152883341"
        "65188018176",
        "4294967295",
        "39614081257132168796771975167",
        "1"
    },
    {
        "170141183420855150474555134919112130560",
        "39614081257132168796771975169",
        "170141183460469231731687303715884105729",
        "170141183381241069217422966122340155391",
        "67399866652183845148200835809661014960282246520841093940"
        "79006064640",
        "4294967294",
        "39614081257132168792477007874",
        "3"
    },
    {
        "9223372036854775808",
        "1",
        "9223372036854775809",
        "9223372036854775807",
        "9223372036854775808",
        "9223372036854775808",
        "0",
        "1"
    },
    {
        "-9223372036854775808",
        "-1",
        "-9223372036854775809",
        "-9223372036854775807",
        "9223372036854775808",
        "9223372036854775808",
        "0",
        "1"
    },
    {
        "9223372036854775807",
        "-9223372036854775808",
        "-1",
        "18446744073709551615",
        "-85070591730234615856620279821087277056",
        "0",
        "9223372036854775807",
        "1"
    },
    {
        "18446744073709551616",
        "4294967296",
        "18446744078004518912",
        "18446744069414584320",
        "79228162514264337593543950336",
        "4294967296",
        "0",
        "4294967296"
    },
    {
        "-18446744073709551615",
        "3",
        "-18446744073709551612",
        "-18446744073709551618",
        "-55340232221128654845",
        "-6148914691236517205",
        "0",
        "3"
    },
    {
        "1000000000000000000000000000000",
        "1000000000000007",
        "1000000000000001000000000000007",
        "999999999999998999999999999993",
        "1000000000000007000000000000000000000000000000",
        "999999999999993",
        "49",
        "1"
    },
    {
        "-10000000000000000000000000000000000000123",
        "100000000000000000000",
        "-9999999999999999999900000000000000000123",
        "-10000000000000000000100000000000000000123",
        "-1000000000000000000000000000000000000012300000000000000"
        "000000",
        "-100000000000000000001",
        "99999999999999999877",
        "1"
    },
    {
        "340282366920938463463374607431768211455",
        "18446744073709551615",
        "340282366920938463481821351505477763070",
        "340282366920938463444927863358058659840",
        "62771017353866807634955070562867279526205340929585567498"
        "25",
        "18446744073709551617",
        "0",
        "18446744073709551615"
    },
    {
        "82817974522014550258408423595736849801612281185389443546"
        "4201864103254919330121223037770283296858019385573376",
        "1798465042647412146620280340569649349251249",
        "82817974522014550258408423595736849801612281185389443546"
        "4201864105053384372768635184390563637427668734824625",
        "82817974522014550258408423595736849801612281185389443546"
        "4201864101456454287473810891150002956288370036322127",
        "14894523208070719071914074206308544467600212618187816635"
        "70332050008810785634558188891270547758574206665172591436"
        "240137451452431156701374142738149146624",
        "46049254535469421445299621817290545564333947095352604006"
        "0005219382",
        "1082842467932234673396375220263285703065258",
        "1"
    },
    {
        "-2656139888758747693387813220357796268292334526533944959"
        "74574961739092490901302182994384699044001",
        "-1237940039285380274899124225",
        "-2656139888758747693387813220357796268292334526533944959"
        "74574961739093728841341468374659598168226",
        "-2656139888758747693387813220357796268292334526533944959"
        "74574961739091252961262897614109799919776",
        "32881419182374697127681701957421232631982525304588800900"
        "09661548960892854380122333096760431402223833062578653160"
        "69640024225",
        "21456127150488200236167695914332890806477298050107457964"
        "4924454221988",
        "1205021844684356220239415299",
        "1"
    },
    {
        "5",
        "10000000000000000000000000",
        "10000000000000000000000005",
        "-9999999999999999999999995",
        "50000000000000000000000000",
        "0",
        "5",
        "5"
    },
    {
        "-5",
        "10000000000000000000000000",
        "9999999999999999999999995",
        "-10000000000000000000000005",
        "-50000000000000000000000000",
        "-1",
        "9999999999999999999999995",
        "5"
    },
    {
        "16069380442589902755419620923411626025222029937827928353"
        "01376",
        "-1267650600228229401496703205379",
        "16069380442589902755419620923398949519219747643812961320"
        "95997",
        "16069380442589902755419620923424302531224312231842895385"
        "06755",
        "-2037035976334486086268445688414198975184245364492562136"
        "913163937161947908744685084689301504",
        "-1267650600228229401496703205373",
        "9",
        "1"
    },
    {
        "26838083675158543920250669552352679373490829033200459881"
        "499714",
        "-51649980101510967",
        "26838083675158543920250669552352679373490828981550479779"
        "988747",
        "26838083675158543920250669552352679373490829084850439983"
        "010681",
        "-1386186487784625116602326349022241191243991457992231309"
        "662116385521461378363438",
        "-519614598542186532403623861106702982813850442",
        "13690960520702300",
        "11"
    },
    {
        "12729495845155517898772603939295048155104706174517247762"
        "6427637085664658171837402836728481803484089826",
        "-874484262432622191924",
        "12729495845155517898772603939295048155104706174517247762"
        "6427637085664658171837401962244219370861897902",
        "12729495845155517898772603939295048155104706174517247762"
        "6427637085664658171837403711212744236106281750",
        "-1113174378528995173955458231170314165163014127997008893"
        "09334369909444262566653736180393708766053064611295353972"
        "584227765224",
        "-1455657510604578549806851009151530876949325756997718892"
        "05024097084309402309157270",
        "684053776558844202346",
        "2"
    },
    {
        "87924601396313531310362757400122113979473835207196359272"
        "004874353275721101045",
        "-445009863293003943854990132680",
        "87924601396313531310362757400122113979473835206751349408"
        "711870409420730968365",
        "87924601396313531310362757400122113979473835207641369135"
        "297878297130711233725",
        "-3912731484746534824446857810279104220179063250358134131"
        "4286841273614750453916001588523424397240140236650600",
        "-197578994644489725965069887953233693395321934460",
        "366386589724558138006056948245",
        "5"
    },
    {
        "2899045639500233288538839397947655395759715367561091502",
        "-685791937129685763275844660813372637449461",
        "2899045639499547496601709712184379551098901994923642041",
        "2899045639500919080475969083710931240420528740198540963",
        "-1988142124940233645429027524263023780208201227259461291"
        "664441253668659789314226002399406521580422",
        "-4227296184953",
        "385032239143391512630947544449265914931169",
        "1"
    },
    {
        "1684823124842332362417389997",
        "4768933734741786703802288554036485933725286",
        "4768933734741788388625413396368848351115283",
        "-4768933734741785018979163711704123516335289",
        "80348098371336716266388703715122442876428882552104828172"
        "73671322364142",
        "0",
        "1684823124842332362417389997",
        "3"
    },
    {
        "97462157112781819300287147497903683644716563062",
        "-244087362215923693279228",
        "97462157112781819300286903410541467721023283834",
        "97462157112781819300287391585265899568409842290",
        "-2378928084553283967332000680813272935868506386792577245"
        "5361344436676136",
        "-399292106842324728363643",
        "90455269390899594255458",
        "2"
    }
};

interior Piscina* piscina;

interior Magnus
_ex (
    constans character* litterae)
{
    Magnus m = magnus_ex_s64(ZEPHYRUM);
       b32 bene;

    bene = magnus_ex_chorda(chorda_ex_literis(litterae, piscina),
        piscina,
        &m);
    CREDO_VERUM (bene);
    redde m;
}

/* forma canonica: parvus si capit; aliter signum +-1, longitudo >= 2,
 * membrum summum non nullum, et valor extra s64 */
interior b32
_canonicus (
    Magnus m)
{
    si (m.membra == NIHIL)
    {
        redde m.signum == ZEPHYRUM && m.longitudo == ZEPHYRUM;
    }
    si (m.signum != I && m.signum != -I) redde FALSUM;
    si (m.longitudo < II) redde FALSUM;
    si (m.membra[m.longitudo - I] == ZEPHYRUM) redde FALSUM;
    si (m.longitudo == II)
    {
        i64 modulus = ((i64)m.membra[I] << XXXII) | m.membra[ZEPHYRUM];

        si (m.signum > ZEPHYRUM && modulus <= 0x7FFFFFFFFFFFFFFFULL)
        {
            redde FALSUM;
        }
        si (m.signum < ZEPHYRUM && modulus <= 0x8000000000000000ULL)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_textus_est (
                Magnus  m,
    constans character* litterae)
{
    redde chorda_aequalis_literis(magnus_ad_chordam(m, piscina),
        litterae);
}

/* numerus fortuitus: interdum valor finitimus (transitus s64), aliter
 * digiti fortuiti usque ad LXXX */
interior constans character* finitimi[] = {
    "0", "1", "-1", "2", "-2",
    "9223372036854775807", "-9223372036854775807",
    "9223372036854775808", "-9223372036854775808",
    "-9223372036854775809", "18446744073709551615",
    "18446744073709551616", "-18446744073709551616",
    "4294967295", "4294967296", "-4294967296"
};

interior Magnus
_fortuitus (
    Sors* s)
{
    character alveus[LXXXII];
          i32 longitudo;
          i32 k;
          i32 positus = ZEPHYRUM;
       Magnus m;

    si (sors_intra(s, V) == ZEPHYRUM)
    {
        redde _ex(finitimi[sors_intra(s,
            (i32)(magnitudo(finitimi)
                / magnitudo(finitimi[ZEPHYRUM])))]);
    }
    si (sors_intra(s, II) == ZEPHYRUM)
    {
        alveus[positus++] = '-';
    }
    longitudo = I + sors_intra(s, LXXX);
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        alveus[positus++] = (character)('0' + sors_intra(s, X));
    }
    (vacuum)magnus_ex_chorda(chorda_ex_buffer((i8*)alveus, positus),
        piscina, &m);
    redde m;
}

/* proprietates unius casus: VERUM si omnes tenent */
interior b32
_casum_probare (
    Magnus a,
    Magnus b,
    Magnus c)
{
    Magnus t;
    Magnus x;
    Magnus y;
    Magnus q;
    Magnus r;
    Magnus u;
    Magnus w;
    Magnus g;

    /* aequalitas falsificabilis: a != a + 1 (sine hoc proprietates
     * omnes vacuae essent si aequalis semper VERUM redderet) */
    si (magnus_aequalis(a, magnus_adde(a, magnus_ex_s64(I), piscina)))
    {
        redde FALSUM;
    }

    /* textus et reditus */
    t = magnus_ex_s64(ZEPHYRUM);
    si (!magnus_ex_chorda(magnus_ad_chordam(a, piscina), piscina, &t))
    {
        redde FALSUM;
    }
    si (!magnus_aequalis(t, a))
    {
        redde FALSUM;
    }

    /* (a + b) - b = a */
    t = magnus_subtrahe(magnus_adde(a, b, piscina), b, piscina);
    si (!magnus_aequalis(t, a) || !_canonicus(t))
    {
        redde FALSUM;
    }

    /* commutatio et distributio */
    x = magnus_multiplica(a, b, piscina);
    y = magnus_multiplica(b, a, piscina);
    si (!magnus_aequalis(x, y) || !_canonicus(x))
    {
        redde FALSUM;
    }
    t = magnus_multiplica(a, magnus_adde(b, c, piscina), piscina);
    y = magnus_adde(x, magnus_multiplica(a, c, piscina), piscina);
    si (!magnus_aequalis(t, y) || !_canonicus(t))
    {
        redde FALSUM;
    }

    /* divisio: a = q b + r, 0 <= r < |b|; divisor nullus FALSUM */
    si (magnus_signum(b) == ZEPHYRUM)
    {
        redde !magnus_divide(a, b, piscina, &q, &r);
    }
    (vacuum)magnus_divide(a, b, piscina, &q, &r);
    si (!_canonicus(q) || !_canonicus(r))
    {
        redde FALSUM;
    }
    t = magnus_adde(magnus_multiplica(q, b, piscina), r, piscina);
    si (!magnus_aequalis(t, a) || magnus_signum(r) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (magnus_compara(r, magnus_absolutum(b, piscina)) >= ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* divisor communis cum testibus: g = u a + w b, g | a, g | b */
    g = magnus_divisor_communis_testatus(a, b, piscina, &u, &w);
    t = magnus_adde(magnus_multiplica(u, a, piscina),
        magnus_multiplica(w, b, piscina), piscina);
    si (magnus_signum(g) < ZEPHYRUM || !magnus_aequalis(g, t))
    {
        redde FALSUM;
    }
    si (magnus_signum(g) != ZEPHYRUM)
    {
        (vacuum)magnus_divide(a, g, piscina, NIHIL, &r);
        si (magnus_signum(r) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        (vacuum)magnus_divide(b, g, piscina, NIHIL, &r);
        si (magnus_signum(r) != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_magnus", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PARVI et TEXTUS
     * ================================================== */

    {
           s64 x;
        Magnus m;

        imprimere("\n--- Probans parvos et textum ---\n");
        CREDO_VERUM (magnus_ad_s64(magnus_ex_s64(-XLII), &x));
        CREDO_AEQUALIS_S64 (x, -XLII);
        CREDO_VERUM (_textus_est(magnus_ex_s64(ZEPHYRUM), "0"));
        CREDO_VERUM (_textus_est(_ex("-0"), "0"));
        CREDO_VERUM (_textus_est(_ex("000123"), "123"));
        CREDO_VERUM (_textus_est(_ex("-9223372036854775808"),
            "-9223372036854775808"));
        CREDO_VERUM (_textus_est(_ex("1000000000000000000000000000001"),
            "1000000000000000000000000000001"));
        CREDO_VERUM (_textus_est(_ex("-1000000000"), "-1000000000"));

        /* malformati: exitus non tangitur */
        m = magnus_ex_s64(VII);
        CREDO_FALSUM (magnus_ex_chorda(chorda_ex_literis("", piscina),
            piscina, &m));
        CREDO_FALSUM (magnus_ex_chorda(chorda_ex_literis("-", piscina),
            piscina, &m));
        CREDO_FALSUM (magnus_ex_chorda(chorda_ex_literis("+5", piscina),
            piscina, &m));
        CREDO_FALSUM (magnus_ex_chorda(chorda_ex_literis("12a",
            piscina),
            piscina, &m));
        CREDO_FALSUM (magnus_ex_chorda(chorda_ex_literis(" 1", piscina),
            piscina, &m));
        CREDO_VERUM (magnus_ad_s64(m, &x));
        CREDO_AEQUALIS_S64 (x, VII);
    }


    /* ==================================================
     * TRANSITUS s64: promotio et demotio canonicae
     * ================================================== */

    {
        Magnus summus  = magnus_ex_s64((s64)0x7FFFFFFFFFFFFFFFLL);
        Magnus imus    = magnus_ex_s64(-(s64)0x7FFFFFFFFFFFFFFFLL - I);
        Magnus unum    = magnus_ex_s64(I);
        Magnus m;
           s64 x;

        imprimere("\n--- Probans transitum s64 ---\n");
        m = magnus_adde(summus, unum, piscina);
        CREDO_FALSUM (magnus_ad_s64(m, &x));
        CREDO_VERUM (_canonicus(m));
        CREDO_VERUM (_textus_est(m, "9223372036854775808"));
        m = magnus_subtrahe(m, unum, piscina);
        CREDO_VERUM (magnus_ad_s64(m, &x));
        CREDO_VERUM (_canonicus(m));
        CREDO_VERUM (magnus_aequalis(m, summus));

        m = magnus_subtrahe(imus, unum, piscina);
        CREDO_VERUM (_textus_est(m, "-9223372036854775809"));
        CREDO_VERUM (_canonicus(m));

        m = magnus_nega(imus, piscina);
        CREDO_VERUM (_textus_est(m, "9223372036854775808"));
        CREDO_VERUM (_canonicus(m));
        m = magnus_nega(m, piscina);
        CREDO_VERUM (_canonicus(m));
        CREDO_VERUM (magnus_aequalis(m, imus));
        CREDO_VERUM (_textus_est(magnus_absolutum(imus, piscina),
            "9223372036854775808"));

        m = magnus_multiplica(imus, magnus_ex_s64(-I), piscina);
        CREDO_VERUM (_textus_est(m, "9223372036854775808"));
        m = magnus_multiplica(summus, summus, piscina);
        CREDO_VERUM (_textus_est(m,
            "85070591730234615847396907784232501249"));

        CREDO_AEQUALIS_S32 (magnus_compara(imus, summus), -I);
        CREDO_AEQUALIS_S32 (magnus_compara(_ex("-99999999999999999999"),
            imus), -I);
        CREDO_AEQUALIS_S32 (magnus_compara(_ex("99999999999999999999"),
            _ex("99999999999999999998")), I);
        CREDO_AEQUALIS_S32 (magnus_signum(_ex("-99999999999999999999")),
            -I);
    }


    /* ==================================================
     * DIVISIO EUCLIDEA: 0 <= r < |b|, signa omnia
     * ================================================== */

    {
        Magnus q;
        Magnus r;
           s64 x;

        imprimere("\n--- Probans divisionem Euclideam ---\n");
        CREDO_VERUM (magnus_divide(magnus_ex_s64(VII),
            magnus_ex_s64(II),
            piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "3") && _textus_est(r, "1"));
        CREDO_VERUM (magnus_divide(magnus_ex_s64(-VII),
            magnus_ex_s64(II),
            piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "-4") && _textus_est(r, "1"));
        CREDO_VERUM (magnus_divide(magnus_ex_s64(VII),
            magnus_ex_s64(-II),
            piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "-3") && _textus_est(r, "1"));
        CREDO_VERUM (magnus_divide(magnus_ex_s64(-VII),
            magnus_ex_s64(-II),
            piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "4") && _textus_est(r, "1"));
        CREDO_VERUM (magnus_divide(magnus_ex_s64(-VI),
            magnus_ex_s64(III),
            piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "-2") && _textus_est(r, "0"));

        /* divisor nullus: FALSUM, exitus intacti */
        q = magnus_ex_s64(V);
        CREDO_FALSUM (magnus_divide(magnus_ex_s64(VII),
            magnus_ex_s64(ZEPHYRUM), piscina, &q, &r));
        CREDO_VERUM (magnus_ad_s64(q, &x));
        CREDO_AEQUALIS_S64 (x, V);

        /* S64_IMUS / -1 = 2^63: quotiens promovetur */
        CREDO_VERUM (magnus_divide(_ex("-9223372036854775808"),
            magnus_ex_s64(-I), piscina, &q, &r));
        CREDO_VERUM (_textus_est(q, "9223372036854775808"));
        CREDO_VERUM (_canonicus(q));
    }


    /* ==================================================
     * VECTORES PYTHONIS
     * ================================================== */

    {
        i32 k;
        i32 numerus = (i32)(magnitudo(vectores)
            / magnitudo(vectores[ZEPHYRUM]));

        imprimere("\n--- Probans vectores Pythonis (%u) ---\n",
            numerus);
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            constans VectorMagnus* v = &vectores[k];
                           Magnus  a = _ex(v->a);
                           Magnus  b = _ex(v->b);
                           Magnus  q;
                           Magnus  r;
                           Magnus  u;
                           Magnus  w;
                           Magnus  g;

            CREDO_VERUM (_textus_est(magnus_adde(a, b, piscina),
                v->summa));
            CREDO_VERUM (_textus_est(magnus_subtrahe(a, b, piscina),
                v->differentia));
            CREDO_VERUM (_textus_est(magnus_multiplica(a, b, piscina),
                v->productum));
            CREDO_VERUM (magnus_divide(a, b, piscina, &q, &r));
            CREDO_VERUM (_textus_est(q, v->quotiens));
            CREDO_VERUM (_textus_est(r, v->residuum));
            CREDO_VERUM (_canonicus(q) && _canonicus(r));
            g = magnus_divisor_communis_testatus(a, b, piscina, &u, &w);
            CREDO_VERUM (_textus_est(g, v->divisor_communis));
            CREDO_VERUM (_textus_est(magnus_divisor_communis(a, b,
                piscina),
                v->divisor_communis));
            CREDO_VERUM (magnus_aequalis(g, magnus_adde(
                magnus_multiplica(u, a, piscina),
                magnus_multiplica(w, b, piscina), piscina)));
        }
    }


    /* ==================================================
     * POTENTIA et FACTORIALE
     * ================================================== */

    {
        Magnus f = magnus_ex_s64(I);
           i32 k;

        imprimere("\n--- Probans potentiam ---\n");
        CREDO_VERUM (_textus_est(magnus_potentia(magnus_ex_s64(XII), C,
            piscina),
            "82817974522014550258408423595736849801612281185389443546"
            "4201864103254919330121223037770283296858019385573376"));
        CREDO_VERUM (_textus_est(magnus_potentia(magnus_ex_s64(-III), V,
            piscina), "-243"));
        CREDO_VERUM (_textus_est(magnus_potentia(
            magnus_ex_s64(ZEPHYRUM), ZEPHYRUM, piscina), "1"));
        CREDO_VERUM (_textus_est(magnus_potentia(magnus_ex_s64(II),
            LXIV,
            piscina), "18446744073709551616"));
        per (k = II; k <= C; k++)
        {
            f = magnus_multiplica(f, magnus_ex_s64((s64)k), piscina);
        }
        CREDO_VERUM (_textus_est(f,
            "93326215443944152681699238856266700490715968264381621468"
            "59296389521759999322991560894146397615651828625369792082"
            "7223758251185210916864000000000000000000000000"));
    }


    /* ==================================================
     * RECENSIO 2026-10-05: lacunae a recensore inventae
     * ================================================== */

    {
        Magnus m = magnus_ex_s64(VII);
        Magnus q;
        Magnus t;
        chorda nulla;
           s64 x;
        chorda textus;
           i32 k;
           b32 bene = VERUM;

        imprimere("\n--- Probans lacunas recensionis ---\n");

        /* aequalitas falsificabilis: parvi, magni, mixti */
        CREDO_FALSUM (magnus_aequalis(magnus_ex_s64(III),
            magnus_ex_s64(IV)));
        CREDO_FALSUM (magnus_aequalis(_ex("99999999999999999999"),
            _ex("99999999999999999998")));
        CREDO_FALSUM (magnus_aequalis(_ex("-99999999999999999999"),
            _ex("99999999999999999999")));
        CREDO_FALSUM (magnus_aequalis(magnus_ex_s64(V),
            _ex("18446744073709551621")));

        /* chorda sine datis: FALSUM, exitus non tactus */
        nulla.mensura  = III;
        nulla.datum    = NIHIL;
        CREDO_FALSUM (magnus_ex_chorda(nulla, piscina, &m));
        CREDO_VERUM (magnus_ad_s64(m, &x) && x == VII);

        /* exitus NIHIL: divisio (via celeris) et ad_s64 */
        CREDO_VERUM (magnus_divide(magnus_ex_s64(XVII),
            magnus_ex_s64(V),
            piscina, &q, NIHIL));
        CREDO_VERUM (_textus_est(q, "3"));
        CREDO_VERUM (magnus_divide(_ex("100000000000000000000"),
            magnus_ex_s64(-VII), piscina, NIHIL, &t));
        CREDO_VERUM (_textus_est(t, "2"));   /* 10^20 mod 7 */
        CREDO_VERUM (magnus_ad_s64(magnus_ex_s64(-I), NIHIL));
        CREDO_FALSUM (magnus_ad_s64(_ex("99999999999999999999"),
            NIHIL));

        /* 10^10000: "1" et X milia nullarum, et reditus */
        m       = magnus_potentia(magnus_ex_s64(X), X * M, piscina);
        textus  = magnus_ad_chordam(m, piscina);
        CREDO_AEQUALIS_I32 (textus.mensura, X * M + I);
        si (textus.mensura > ZEPHYRUM && textus.datum[ZEPHYRUM] != '1')
        {
            bene = FALSUM;
        }
        per (k = I; k < textus.mensura; k++)
        {
            si (textus.datum[k] != '0')
            {
                bene = FALSUM;
            }
        }
        CREDO_VERUM (bene);
        CREDO_VERUM (magnus_ex_chorda(textus, piscina, &t));
        CREDO_VERUM (magnus_aequalis(t, m));
    }


    /* ==================================================
     * TRANSCRIPTIO: copia superstat piscinae originis destructae
     * ================================================== */

    {
        Piscina* origo = piscina_generare_dynamicum("probatio_origo",
            (memoriae_index)4096);
          Magnus magnum;
          Magnus copia;
          Magnus parvum;

        imprimere("\n--- Probans transcriptionem ---\n");
        magnum  = magnus_potentia(magnus_ex_s64(-VII), C, origo);
        copia   = magnus_transcribe(magnum, piscina);
        parvum  = magnus_transcribe(magnus_ex_s64(-XLII), piscina);
        CREDO_VERUM (magnus_aequalis(copia, magnum));
        /* membra NOVA, non partita (lectio post destructionem sola
         * sine sanitatore nihil probaret) */
        CREDO_VERUM (copia.membra != magnum.membra);
        piscina_destruere(origo);
        CREDO_VERUM (magnus_aequalis(copia, magnus_potentia(
            magnus_ex_s64(-VII), C, piscina)));
        CREDO_VERUM (_textus_est(parvum, "-42"));
    }


    /* ==================================================
     * EUCLIDES: Fibonacci (casus pessimus) et memoria vocantis
     *
     * mdc(F_m, F_n) = F_mdc(m,n). Gradus Euclidis = index: olim omnes
     * in piscina vocantis (XIII MB pro F_10000); nunc piscinae alternae
     * internae, vocans solum effectum accipit.
     * ================================================== */

    {
        Piscina* arca = piscina_generare_dynamicum("probatio_fibonacci",
            (memoriae_index)1048576);
        Piscina* vocans;
         Magnus  f[X * M + I];
         Magnus  g;
         Magnus  u;
         Magnus  w;
            i32  k;

        imprimere("\n--- Probans Euclidem super Fibonacci ---\n");
        f[ZEPHYRUM]  = magnus_ex_s64(ZEPHYRUM);
        f[I]         = magnus_ex_s64(I);
        per (k = II; k <= X * M; k++)
        {
            f[k] = magnus_adde(f[k - I], f[k - II], arca);
        }
        CREDO_VERUM (magnus_aequalis(magnus_divisor_communis(f[M],
            f[DCCC], piscina), f[CC]));
        CREDO_VERUM (magnus_aequalis(magnus_divisor_communis(f[CMXCIX],
            f[DCCCLXXXVIII], piscina), f[CXI]));

        vocans = piscina_generare_dynamicum("probatio_vocans",
            (memoriae_index)4096);
        g = magnus_divisor_communis(f[X * M], f[X * M - I], vocans);
        CREDO_VERUM (_textus_est(g, "1"));
        CREDO_MINOR_I32 ((i32)piscina_summa_usus(vocans), CDXCVI);
        /* piscinae alternae vere reficiuntur: apex internus parvus
         * (mensum MMMCDLXXXVIII; si numquam vacarentur, MB XIII) */
        CREDO_MAIOR_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
        CREDO_MINOR_I32 ((i32)magnus_apex_alternarum(), XVI * M);
        piscina_destruere(vocans);

        vocans = piscina_generare_dynamicum("probatio_vocans",
            (memoriae_index)4096);
        g = magnus_divisor_communis_testatus(f[X * M], f[X * M - I],
            vocans, &u, &w);
        CREDO_VERUM (_textus_est(g, "1"));
        CREDO_VERUM (magnus_aequalis(g, magnus_adde(
            magnus_multiplica(u, f[X * M], arca),
            magnus_multiplica(w, f[X * M - I], arca), arca)));
        /* testes ~ F_9999: duo numeri MMDCCCC digitorum circiter */
        CREDO_MINOR_I32 ((i32)piscina_summa_usus(vocans), XVI * M);
        CREDO_MAIOR_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
        CREDO_MINOR_I32 ((i32)magnus_apex_alternarum(), XVI * M);

        /* effectus vivit post mdc alterum: transcriptus in piscinam
         * vocantis, non in piscinam alternam (iam destructam) */
        g = magnus_divisor_communis(f[M], f[DCCC], piscina);
        (vacuum)magnus_divisor_communis(f[MCC], f[CM], piscina);
        CREDO_VERUM (magnus_aequalis(g, f[CC]));

        /* operandus unus parvus (recensio III): sine testibus via
         * vocantis, apex nullus; cum testibus piscinae alternae (unus
         * magnus sufficit, recensio IV) */
        g = magnus_divisor_communis(f[X * M], _ex("1001"), piscina);
        CREDO_AEQUALIS_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
        CREDO_VERUM (magnus_aequalis(g, magnus_divisor_communis(
            magnus_ex_s64(MI), f[X * M], piscina)));
        g = magnus_divisor_communis_testatus(magnus_ex_s64(-VII), f[M],
            piscina, &u, &w);
        CREDO_MAIOR_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
        CREDO_VERUM (magnus_aequalis(g, magnus_adde(
            magnus_multiplica(u, magnus_ex_s64(-VII), piscina),
            magnus_multiplica(w, f[M], piscina), piscina)));

        /* testes cum operando uno parvo (recensio IV): testis operandi
         * parvi post gradum primum magnus, deinde CLXXX gradus parvi -
         * via vocantis MB crescebat, ergo piscinae alternae */
        {
            Magnus magnum = magnus_adde(magnus_multiplica(f[X * M],
                f[CLXXXIV], arca), f[CLXXXIII], arca);

            piscina_destruere(vocans);
            vocans = piscina_generare_dynamicum("probatio_vocans",
                (memoriae_index)4096);
            g = magnus_divisor_communis_testatus(magnum, f[CLXXXIV],
                vocans, &u, &w);
            CREDO_VERUM (_textus_est(g, "1"));
            CREDO_VERUM (magnus_aequalis(g, magnus_adde(
                magnus_multiplica(u, magnum, arca),
                magnus_multiplica(w, f[CLXXXIV], arca), arca)));
            CREDO_MINOR_I32 ((i32)piscina_summa_usus(vocans), XVI * M);
            CREDO_MAIOR_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
            piscina_destruere(vocans);

            /* sine testibus: gradus primus solus magnus, via
             * vocantis */
            vocans = piscina_generare_dynamicum("probatio_vocans",
                (memoriae_index)4096);
            g = magnus_divisor_communis(magnum, f[CLXXXIV], vocans);
            CREDO_VERUM (_textus_est(g, "1"));
            CREDO_AEQUALIS_I32 ((i32)magnus_apex_alternarum(),
                ZEPHYRUM);
            CREDO_MINOR_I32 ((i32)piscina_summa_usus(vocans), XVI * M);
        }

        /* operandi pauci membrorum: via vocantis, apex nullus */
        (vacuum)magnus_divisor_communis(f[LX], f[LIX], piscina);
        CREDO_AEQUALIS_I32 ((i32)magnus_apex_alternarum(), ZEPHYRUM);
        piscina_destruere(vocans);
        piscina_destruere(arca);
    }


    /* ==================================================
     * PROPRIETATES super numeros fortuitos
     * ================================================== */

    {
        Sors s;
         i32 k;
         b32 bene          = VERUM;
         i32 casus_primus  = 0xFFFFFFFFU;

        imprimere("\n--- Probans proprietates (MD casus) ---\n");
        sors_seminare(&s, 2026ULL, ZEPHYRUM);
        per (k = ZEPHYRUM; k < MD; k++)
        {
            Magnus a    = _fortuitus(&s);
            Magnus b    = _fortuitus(&s);
            Magnus c    = _fortuitus(&s);
               b32 hic  = _casum_probare(a, b, c);

            si (!hic && bene)
            {
                bene          = FALSUM;
                casus_primus  = k;
            }
        }
        si (!bene)
        {
            imprimere("  casus primus fractus: %u\n", casus_primus);
        }
        CREDO_VERUM (bene);
    }

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        credo_claudere();
        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
