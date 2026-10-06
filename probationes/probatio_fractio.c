/* probatio_fractio.c - Probationes numerorum rationalium
 *
 * Latera duo: vectores ex fractions.Fraction Pythonis (oraculum
 * INDEPENDENS: summa, differentia, productum, quotiens, pavimentum,
 * tectum, rotundatio ad parem, ordo) et leges corporis super fractiones
 * fortuitas (sors). Forma canonica (denominator > 0, divisor communis
 * 1, nihil 0/1) post omnem operationem. Vide lib/fractio.worklog.md.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "sors.h"
#include "magnus.h"
#include "fractio.h"
#include <stdio.h>

nomen structura {
     constans character* a;
     constans character* b;
     constans character* summa;
     constans character* differentia;
     constans character* productum;
     constans character* quotiens;
     constans character* pavimentum;
     constans character* tectum;
     constans character* rotunda;
                    s32  ordo;
} VectorFractionis;

/* Ex Pythone (random.seed 20261005): fines s64, magni, signa, nihil */
interior constans VectorFractionis vectores[] = {
    {
        "1/2",
        "1/3",
        "5/6",
        "1/6",
        "1/6",
        "3/2",
        "0",
        "1",
        "0",
        1
    },
    {
        "-7/2",
        "5/6",
        "-8/3",
        "-13/3",
        "-35/12",
        "-21/5",
        "-4",
        "-3",
        "-4",
        -1
    },
    {
        "1/6",
        "-1/6",
        "0",
        "1/3",
        "-1/36",
        "-1",
        "0",
        "1",
        "0",
        1
    },
    {
        "3/4",
        "4/3",
        "25/12",
        "-7/12",
        "1",
        "9/16",
        "0",
        "1",
        "1",
        -1
    },
    {
        "0",
        "5/7",
        "5/7",
        "-5/7",
        "0",
        "0",
        "0",
        "0",
        "0",
        -1
    },
    {
        "-5/2",
        "-3/2",
        "-4",
        "-1",
        "15/4",
        "5/3",
        "-3",
        "-2",
        "-2",
        -1
    },
    {
        "18446744073709551616/3",
        "-9223372036854775808/5",
        "64563604257983430656/15",
        "119903836479112085504/15",
        "-170141183460469231731687303715884105728/15",
        "-10/3",
        "6148914691236517205",
        "6148914691236517206",
        "6148914691236517205",
        1
    },
    {
        "1000000000000000000000000000001/1000000000000000",
        "7/100000000000000000000",
        "100000000000000000000000000000100007/1000000000000000000"
        "00",
        "100000000000000000000000000000099993/1000000000000000000"
        "00",
        "7000000000000000000000000000007/100000000000000000000000"
        "000000000000",
        "100000000000000000000000000000100000/7",
        "1000000000000000",
        "1000000000000001",
        "1000000000000000",
        1
    },
    {
        "14697715679690864505827555550150426126974976/22539340290"
        "692258087863249",
        "-2910383045673370361328125/1180591620717411303424",
        "17351999975129880591143995744660239773612925596075378815"
        "373539699/2660975628368962139367118740545315938015746457"
        "6",
        "17351999975130011787371681131546174418276864848571144501"
        "408695949/2660975628368962139367118740545315938015746457"
        "6",
        "-36232666549256231787800788879394531250000000000/2253934"
        "0290692258087863249",
        "-1735199997512994618925783843810320709594489522232326165"
        "8391117824/655981138426934429673223319696262478828430175"
        "78125",
        "652091653532573236",
        "652091653532573237",
        "652091653532573237",
        1
    },
    {
        "9223372036854775807",
        "-9223372036854775808",
        "-1",
        "18446744073709551615",
        "-85070591730234615856620279821087277056",
        "-9223372036854775807/9223372036854775808",
        "9223372036854775807",
        "9223372036854775807",
        "9223372036854775807",
        1
    },
    {
        "1/9223372036854775807",
        "1/9223372036854775806",
        "18446744073709551613/85070591730234615838173535747377725"
        "442",
        "-1/85070591730234615838173535747377725442",
        "1/85070591730234615838173535747377725442",
        "9223372036854775806/9223372036854775807",
        "0",
        "1",
        "0",
        -1
    },
    {
        "946250263707004292162/18569067",
        "-6368992755255581381388250488556138345226/14667793892103"
        "002809356970428717769",
        "13879403720136200074852669836930557642577952755346102436"
        "/272367247524651430068137810807878976651523",
        "13879403956668706464563655426832510312141285142887550720"
        "/272367247524651430068137810807878976651523",
        "-6026661074208593729070913749854310596999670566604654121"
        "918612/272367247524651430068137810807878976651523",
        "-6939701919201226634854081315940766988679809474558413289"
        "/59133126597427746397475488167390833096885362071",
        "50958417227263",
        "50958417227264",
        "50958417227263",
        1
    },
    {
        "59613225355613106166087595663/14532042194917711438326038"
        "70",
        "11926248714555516990277955317/575393523607",
        "17331274954700423273050985755325013908220433251882593231"
        "/836164296373930429191086747329124559090",
        "-1733127495470035467092340386656346843757995735535996034"
        "9/836164296373930429191086747329124559090",
        "71096215226788915962515934348025302725479634667227699017"
        "1/836164296373930429191086747329124559090",
        "34301063790944380772735320237948261316441/17331274954700"
        "388971987194810944241172900195303621276790",
        "41",
        "42",
        "41",
        -1
    },
    {
        "-863023852956961945684684/869809166163532207",
        "-1329136374776909576880529649465700523130933687916035064"
        "651/636829036770301138138096202",
        "-1156095001862323759895114081385391494912425049370646670"
        "559589363854915684925/5539197334619010243974797474637222"
        "74691377814",
        "11560950018623237598951129821880935181602619851017670765"
        "73212307900615744589/55391973346190102439747974746372227"
        "4691377814",
        "57353819763260853749096179064528542503530351880976596134"
        "7853584492862129200252642/276959866730950512198739873731"
        "861137345688907",
        "549598648988376081532134439796993188527977149970168/1156"
        "09500186232375989511353178674250653634351723620687356640"
        "0835877765714757",
        "-992200",
        "-992199",
        "-992199",
        1
    },
    {
        "12442317364081854492660546933421879346072758060/86888780"
        "7318017939228619018357553",
        "-17714298106985/98188361785389704961313865422921285907",
        "12216907587931055260861057646022681610211787917793288750"
        "61836793990170039045521852715/85314670375855525855436745"
        "535142378276290775733178961271917353865905571",
        "12216907587931055260861057646022681610519622670600407002"
        "83356387286223979189075468125/85314670375855525855436745"
        "535142378276290775733178961271917353865905571",
        "-2204069189290617900719249441189099441285177277963359010"
        "49100/85314670375855525855436745535142378276290775733178"
        "961271917353865905571",
        "-1285990272413795290616953436423440169512179504652299776"
        "4974700954086284306497880636/162018290951114869586945227"
        "873968106018703239",
        "14319820417882",
        "14319820417883",
        "14319820417883",
        1
    },
    {
        "-168769819176542965876972491231458781268609705/977413133"
        "83881611894",
        "0",
        "-168769819176542965876972491231458781268609705/977413133"
        "83881611894",
        "-168769819176542965876972491231458781268609705/977413133"
        "83881611894",
        "0",
        "-",
        "-1726698908921911100830190",
        "-1726698908921911100830189",
        "-1726698908921911100830190",
        -1
    },
    {
        "-448990774486937929209591097/508778612688604799",
        "-6258472800711919/88948130117573801806527417",
        "-39936889830654390564936170043914286380447682129505730/4"
        "5254906242464704951725865921854852871274183",
        "-39936889830654390558567815825723132049132430049707168/4"
        "5254906242464704951725865921854852871274183",
        "2809996549897080048428517743321624484185143/452549062424"
        "64704951725865921854852871274183",
        "39936889830654390561751992934818709214790056089606449/31"
        "84177109095577165657626039899281",
        "-882487517",
        "-882487516",
        "-882487517",
        -1
    },
    {
        "-19645050317019880221592531707919575456031635/4230621149"
        "9",
        "2560399188263292620683/45127726709",
        "-886536461891026965959833810880831077354990928958605398/"
        "1909183150620025226791",
        "-886536461891026965960050452459992146907208410918273032/"
        "1909183150620025226791",
        "-3869166991160710821587718651844260815699003606455978001"
        "423331285/146860242355386555907",
        "-886536461891026965959942131670411612131099669938439215/"
        "108320789580534776108740979833817",
        "-464353805764059824344153140071730",
        "-464353805764059824344153140071729",
        "-464353805764059824344153140071730",
        -1
    },
    {
        "197374211946249145148541117587313096866788063409/1575925"
        "495427333567788594942410156",
        "-7617596545/3572562311326971498",
        "11752194513783856592004484450534713311426876385218344494"
        "3673534277/938348671737162910580494218950876710810931406"
        "288948",
        "39173981712612855306682948697850060445810093386741560323"
        "259901539/3127828905790543035268314063169589036036438020"
        "96316",
        "-1503517114993845213792730329123554982525695077071639321"
        "905/5630092030422977463482965313705260264865588437733688",
        "-1175219451378385659200468452994486572258495220062040629"
        "56726619447/2000794101524111580758154020618013053085170",
        "125243364942660",
        "125243364942661",
        "125243364942661",
        1
    },
    {
        "4276218851179827510156514206/389470144823792861",
        "-3049331965106963284554161659533294875488285333513451703"
        "27/8273089696082000124094786470937211",
        "-1187623762065676094091080898153603406873116207618396112"
        "84906060260556116081/32221214420732850541842257293155665"
        "65205842401050671",
        "11876237620663836449333986469314791349555347311650147025"
        "2893486039867155013/322212144207328505418422572931556656"
        "5205842401050671",
        "-1303961083269562440370123908146965182362919042852935859"
        "929739375535060604135665165362/3222121442073285054184225"
        "729315566565205842401050671",
        "-1072046730784467845266254670331096282755438908900390595"
        "621802/3598859885048575362158302341038003851255531725429"
        "410326330296156067019259",
        "10979580612",
        "10979580613",
        "10979580612",
        1
    }
};

interior Piscina* piscina;

interior Fractio
_ex (
    constans character* litterae)
{
    Fractio f = fractio_ex_s64(ZEPHYRUM);
        b32 bene;

    bene = fractio_ex_chorda(chorda_ex_literis(litterae, piscina),
        piscina, &f);
    CREDO_VERUM (bene);
    redde f;
}

interior b32
_textus_est (
                Fractio  f,
     constans character* litterae)
{
    redde chorda_aequalis_literis(fractio_ad_chordam(f, piscina),
        litterae);
}

interior b32
_magnus_textus_est (
                 Magnus  m,
     constans character* litterae)
{
    redde chorda_aequalis_literis(magnus_ad_chordam(m, piscina),
        litterae);
}

/* forma canonica: denominator > 0, mdc 1, nihil = 0/1 */
interior b32
_canonica (
    Fractio f)
{
    Magnus n = fractio_numerator(f);
    Magnus d = fractio_denominator(f);
    Magnus g;

    si (magnus_signum(d) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    g = magnus_divisor_communis(n, d, piscina);
    si (magnus_signum(n) == ZEPHYRUM)
    {
        redde magnus_aequalis(d, magnus_ex_s64(I));
    }
    redde magnus_aequalis(g, magnus_ex_s64(I));
}

/* fractio fortuita: interdum integra aut finitima, aliter numerator et
 * denominator digitorum fortuitorum (usque ad XL) */
interior constans character* finitimae[] = {
    "0", "1", "-1", "1/2", "-1/2", "3/2", "-3/2",
    "9223372036854775807", "-9223372036854775808",
    "1/9223372036854775807", "-9223372036854775808/3",
    "18446744073709551616/18446744073709551615"
};

interior vacuum
_digiti (
         Sors* s,
    character* alveus,
          i32* positus,
          b32  sine_nihilo)
{
    i32 longitudo = I + sors_intra(s, XL);
    i32 k;

    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        alveus[(*positus)++] = (character)('0' + sors_intra(s, X));
    }
    si (sine_nihilo)
    {
        alveus[*positus - I] = (character)('1' + sors_intra(s, IX));
    }
}

interior Fractio
_fortuita (
    Sors* s)
{
    character alveus[XC];
          i32 positus  = ZEPHYRUM;
      Fractio f        = fractio_ex_s64(ZEPHYRUM);

    si (sors_intra(s, V) == ZEPHYRUM)
    {
        redde _ex(finitimae[sors_intra(s,
            (i32)(magnitudo(finitimae)
                / magnitudo(finitimae[ZEPHYRUM])))]);
    }
    si (sors_intra(s, II) == ZEPHYRUM)
    {
        alveus[positus++] = '-';
    }
    _digiti(s, alveus, &positus, FALSUM);
    si (sors_intra(s, IV) != ZEPHYRUM)
    {
        alveus[positus++] = '/';
        _digiti(s, alveus, &positus, VERUM);
    }
    (vacuum)fractio_ex_chorda(chorda_ex_buffer((i8*)alveus, positus),
        piscina, &f);
    redde f;
}

/* leges unius casus: VERUM si omnes tenent */
interior b32
_casum_probare (
    Fractio a,
    Fractio b,
    Fractio c)
{
    Fractio t = fractio_ex_s64(ZEPHYRUM);
    Fractio x;
    Fractio y;
    Fractio unum = fractio_ex_s64(I);
     Magnus m;

    /* aequalitas falsificabilis: a != a + 1; et a != a/2 (a non
     * nullo) - numerator idem si impar, denominator solus differt */
    si (fractio_aequalis(a, fractio_adde(a, unum, piscina)))
    {
        redde FALSUM;
    }
    (vacuum)fractio_ex_s64_s64(I, II, piscina, &x);
    si (   fractio_signum(a) != ZEPHYRUM
        && fractio_aequalis(a, fractio_multiplica(a, x, piscina)))
    {
        redde FALSUM;
    }

    /* textus et reditus */
    si (!fractio_ex_chorda(fractio_ad_chordam(a, piscina), piscina, &t))
    {
        redde FALSUM;
    }
    si (!fractio_aequalis(t, a))
    {
        redde FALSUM;
    }

    /* ex partibus: n / (-d) = -(n/d), canonica (signum transfertur) */
    si (!fractio_ex_magnis(fractio_numerator(a),
            magnus_nega(fractio_denominator(a), piscina), piscina, &t))
    {
        redde FALSUM;
    }
    si (!fractio_aequalis(t, fractio_nega(a, piscina)) || !_canonica(t))
    {
        redde FALSUM;
    }

    /* (a + b) - b = a */
    t = fractio_subtrahe(fractio_adde(a, b, piscina), b, piscina);
    si (!fractio_aequalis(t, a) || !_canonica(t))
    {
        redde FALSUM;
    }

    /* commutatio, distributio */
    x = fractio_multiplica(a, b, piscina);
    y = fractio_multiplica(b, a, piscina);
    si (!fractio_aequalis(x, y) || !_canonica(x))
    {
        redde FALSUM;
    }
    t = fractio_multiplica(a, fractio_adde(b, c, piscina), piscina);
    y = fractio_adde(x, fractio_multiplica(a, c, piscina), piscina);
    si (!fractio_aequalis(t, y) || !_canonica(t) || !_canonica(y))
    {
        redde FALSUM;
    }

    /* ordo congruit cum signo differentiae */
    si (fractio_compara(a, b, piscina)
        != fractio_signum(fractio_subtrahe(a, b, piscina)))
    {
        redde FALSUM;
    }

    /* divisio et inversa: (a / b) b = a, b (1/b) = 1 */
    si (fractio_signum(b) == ZEPHYRUM)
    {
        redde !fractio_divide(a, b, piscina, &t)
            && !fractio_inversa(b, piscina, &t);
    }
    (vacuum)fractio_divide(a, b, piscina, &t);
    si (   !_canonica(t)
        || !fractio_aequalis(fractio_multiplica(t, b, piscina), a))
    {
        redde FALSUM;
    }
    (vacuum)fractio_inversa(b, piscina, &t);
    si (!fractio_aequalis(fractio_multiplica(t, b, piscina), unum))
    {
        redde FALSUM;
    }

    /* pavimentum <= a < pavimentum + 1; |a - rotunda(a)| <= 1/2 */
    m = fractio_pavimentum(a, piscina);
    x = fractio_ex_magno(m);
    si (   fractio_compara(x, a, piscina) > ZEPHYRUM
        || fractio_compara(fractio_adde(x, unum, piscina), a, piscina)
        <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    x = fractio_subtrahe(a, fractio_ex_magno(fractio_rotunda(a,
        piscina)),
        piscina);
    (vacuum)fractio_ex_s64_s64(I, II, piscina, &y);
    si (fractio_compara(fractio_absolutum(x, piscina), y, piscina)
        > ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde VERUM;
}

s32 principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_fractio", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * TEXTUS et FORMA CANONICA
     * ================================================== */

    {
        Fractio f;
        Fractio g = fractio_ex_s64(VII);

        imprimere("\n--- Probans textum et formam canonicam ---\n");
        CREDO_VERUM (_textus_est(_ex("6/4"), "3/2"));
        CREDO_VERUM (_textus_est(_ex("-6/4"), "-3/2"));
        CREDO_VERUM (_textus_est(_ex("0/5"), "0"));
        CREDO_VERUM (_textus_est(_ex("-0/3"), "0"));
        CREDO_VERUM (_textus_est(_ex("10/5"), "2"));
        CREDO_VERUM (_textus_est(_ex("5"), "5"));
        CREDO_VERUM (_textus_est(_ex("-5"), "-5"));
        CREDO_VERUM (_canonica(_ex("0/5")));
        CREDO_VERUM (fractio_est_integra(_ex("10/5")));
        CREDO_FALSUM (fractio_est_integra(_ex("10/4")));
        CREDO_VERUM (fractio_ex_s64_s64(III, -VI, piscina, &f));
        CREDO_VERUM (_textus_est(f, "-1/2") && _canonica(f));
        CREDO_FALSUM (fractio_ex_s64_s64(III, ZEPHYRUM, piscina, &g));

        /* malformatae: exitus non tangitur */
        f = fractio_ex_s64(VII);
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("3/-4",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("3/0",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("/4",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("3/",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("1/2/3",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("+1/2",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis(" 1/2",
            piscina), piscina, &f));
        CREDO_FALSUM (fractio_ex_chorda(chorda_ex_literis("",
            piscina), piscina, &f));
        CREDO_VERUM (fractio_aequalis(f, g));

        /* chorda sine datis: FALSUM, exitus non tactus */
        {
            chorda nulla;

            nulla.mensura  = III;
            nulla.datum    = NIHIL;
            CREDO_FALSUM (fractio_ex_chorda(nulla, piscina, &f));
            CREDO_VERUM (fractio_aequalis(f, g));
        }

        /* aequalitas falsificabilis (recensio 2026-10-05: mutans qui
         * numeratores solos comparabat suitam transibat) */
        CREDO_FALSUM (fractio_aequalis(_ex("1/2"), _ex("1/3")));
        CREDO_FALSUM (fractio_aequalis(_ex("1/2"), _ex("2/3")));
        CREDO_FALSUM (fractio_aequalis(_ex("-1/2"), _ex("1/2")));
        CREDO_VERUM (fractio_aequalis(_ex("2/4"), _ex("1/2")));
    }


    /* ==================================================
     * ROTUNDATIO: pavimentum, tectum, ad parem
     * ================================================== */

    {
        interior constans character* casus_rotundi[][IV] = {
            /* a, pavimentum, tectum, rotunda */
            { "1/2",   "0",  "1",  "0" },
            { "-1/2",  "-1", "0",  "0" },
            { "3/2",   "1",  "2",  "2" },
            { "-3/2",  "-2", "-1", "-2" },
            { "5/2",   "2",  "3",  "2" },
            { "-5/2",  "-3", "-2", "-2" },
            { "7/2",   "3",  "4",  "4" },
            { "1/3",   "0",  "1",  "0" },
            { "2/3",   "0",  "1",  "1" },
            { "-2/3",  "-1", "0",  "-1" },
            { "-7/2",  "-4", "-3", "-4" },
            { "4",     "4",  "4",  "4" },
            { "-4",    "-4", "-4", "-4" }
        };
        i32 k;

        imprimere("\n--- Probans rotundationem ---\n");
        per (k = ZEPHYRUM; k < (i32)(magnitudo(casus_rotundi)
            / magnitudo(casus_rotundi[ZEPHYRUM])); k++)
        {
            Fractio f = _ex(casus_rotundi[k][ZEPHYRUM]);

            CREDO_VERUM (_magnus_textus_est(fractio_pavimentum(f,
                piscina),
                casus_rotundi[k][I]));
            CREDO_VERUM (_magnus_textus_est(fractio_tectum(f, piscina),
                casus_rotundi[k][II]));
            CREDO_VERUM (_magnus_textus_est(fractio_rotunda(f, piscina),
                casus_rotundi[k][III]));
        }
    }


    /* ==================================================
     * POTENTIA, INVERSA, DIVISIO PER NIHIL
     * ================================================== */

    {
        Fractio f = fractio_ex_s64(IX);

        imprimere("\n--- Probans potentiam et inversam ---\n");
        CREDO_VERUM (fractio_potentia(_ex("2/3"), -III, piscina, &f));
        CREDO_VERUM (_textus_est(f, "27/8") && _canonica(f));
        CREDO_VERUM (fractio_potentia(_ex("-2/3"), III, piscina, &f));
        CREDO_VERUM (_textus_est(f, "-8/27"));
        CREDO_VERUM (fractio_potentia(_ex("-1/2"), -I, piscina, &f));
        CREDO_VERUM (_textus_est(f, "-2") && _canonica(f));
        CREDO_VERUM (fractio_potentia(_ex("0"), ZEPHYRUM, piscina, &f));
        CREDO_VERUM (_textus_est(f, "1"));
        /* exponens imus s32: negatio sine exundatione */
        CREDO_VERUM (fractio_potentia(_ex("-1"), (s32)0x80000000U,
            piscina, &f));
        CREDO_VERUM (_textus_est(f, "1"));

        f = fractio_ex_s64(IX);
        CREDO_FALSUM (fractio_potentia(_ex("0"), -I, piscina, &f));
        CREDO_FALSUM (fractio_inversa(_ex("0"), piscina, &f));
        CREDO_FALSUM (fractio_divide(_ex("1/2"), _ex("0"), piscina,
            &f));
        CREDO_VERUM (_textus_est(f, "9"));
        CREDO_VERUM (fractio_inversa(_ex("-3/7"), piscina, &f));
        CREDO_VERUM (_textus_est(f, "-7/3") && _canonica(f));
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
            constans VectorFractionis* v = &vectores[k];
                              Fractio  a = _ex(v->a);
                              Fractio  b = _ex(v->b);
                              Fractio  t   =
                                  fractio_ex_s64(ZEPHYRUM);

            t = fractio_adde(a, b, piscina);
            CREDO_VERUM (_textus_est(t, v->summa) && _canonica(t));
            t = fractio_subtrahe(a, b, piscina);
            CREDO_VERUM (_textus_est(t, v->differentia)
                && _canonica(t));
            t = fractio_multiplica(a, b, piscina);
            CREDO_VERUM (_textus_est(t, v->productum) && _canonica(t));
            /* "-" = divisor nullus in oraculo: refutatio exspectatur */
            si (v->quotiens[ZEPHYRUM] == '-' && v->quotiens[I] == '\0')
            {
                CREDO_FALSUM (fractio_divide(a, b, piscina, &t));
            }
            alioquin
            {
                CREDO_VERUM (fractio_divide(a, b, piscina, &t));
                CREDO_VERUM (_textus_est(t, v->quotiens)
                    && _canonica(t));
            }
            CREDO_VERUM (_magnus_textus_est(fractio_pavimentum(a,
                piscina),
                v->pavimentum));
            CREDO_VERUM (_magnus_textus_est(fractio_tectum(a, piscina),
                v->tectum));
            CREDO_VERUM (_magnus_textus_est(fractio_rotunda(a, piscina),
                v->rotunda));
            CREDO_AEQUALIS_S32 (fractio_compara(a, b, piscina),
                v->ordo);
        }
    }


    /* ==================================================
     * LEGES CORPORIS super fractiones fortuitas
     * ================================================== */

    {
        Sors s;
         i32 k;
         b32 bene          = VERUM;
         i32 casus_primus  = 0xFFFFFFFFU;

        imprimere("\n--- Probans leges corporis (M casus) ---\n");
        sors_seminare(&s, 2026ULL, I);
        per (k = ZEPHYRUM; k < M; k++)
        {
            Fractio a = _fortuita(&s);
            Fractio b = _fortuita(&s);
            Fractio c = _fortuita(&s);

            si (!_casum_probare(a, b, c) && bene)
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
