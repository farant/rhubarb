/* rfc7541_exempla.h - RFC 7541 Appendix C (C.2-C.6) ut data: octeti
 * (hex), capita, tabula dynamica post decodationem (recentissima
 * primum) et magnitudo. GENERATA 2026-10-30 ex textu rfc7541.txt
 * (rfc-editor.org) per scriptum in http2-plan T2; summae entrium cum
 * "Table size" RFC concordant. Ab probatio_hpack.c sola inclusa. */
#ifndef RFC7541_EXEMPLA_H
#define RFC7541_EXEMPLA_H

#include "latina.h"

nomen structura {
    constans character* titulus;
    constans character* valor;
} ExemplumCaput;

nomen structura {
    constans character* id;
    constans character* hex;
                   i32  numerus_capitum;
         ExemplumCaput  capita[VI];
                   i32  numerus_entrium;
         ExemplumCaput  entria[V];
                   s32  mensura_tabulae; /* -1: RFC tabulam non dat */
} ExemplumHpack;

hic_manens constans ExemplumHpack EXEMPLA_RFC7541[] = {
    { "C.2.1",
        "400a637573746f6d2d6b65790d637573746f6d2d686561646572",
        I, {
            { "custom-key", "custom-header" }
        },
        I, {
            { "custom-key", "custom-header" }
        },
        LV },
    { "C.2.2",
        "040c2f73616d706c652f70617468",
        I, {
            { ":path", "/sample/path" }
        },
        ZEPHYRUM, {
            { NIHIL, NIHIL }
        },
        -I },
    { "C.2.3",
        "100870617373776f726406736563726574",
        I, {
            { "password", "secret" }
        },
        ZEPHYRUM, {
            { NIHIL, NIHIL }
        },
        -I },
    { "C.2.4",
        "82",
        I, {
            { ":method", "GET" }
        },
        ZEPHYRUM, {
            { NIHIL, NIHIL }
        },
        -I },
    { "C.3.1",
        "828684410f7777772e6578616d706c652e636f6d",
        IV, {
            { ":method", "GET" },
            { ":scheme", "http" },
            { ":path", "/" },
            { ":authority", "www.example.com" }
        },
        I, {
            { ":authority", "www.example.com" }
        },
        LVII },
    { "C.3.2",
        "828684be58086e6f2d6361636865",
        V, {
            { ":method", "GET" },
            { ":scheme", "http" },
            { ":path", "/" },
            { ":authority", "www.example.com" },
            { "cache-control", "no-cache" }
        },
        II, {
            { "cache-control", "no-cache" },
            { ":authority", "www.example.com" }
        },
        CX },
    { "C.3.3",
        "828785bf400a637573746f6d2d6b65790c637573746f6d2d76616c75"
        "65",
        V, {
            { ":method", "GET" },
            { ":scheme", "https" },
            { ":path", "/index.html" },
            { ":authority", "www.example.com" },
            { "custom-key", "custom-value" }
        },
        III, {
            { "custom-key", "custom-value" },
            { "cache-control", "no-cache" },
            { ":authority", "www.example.com" }
        },
        CLXIV },
    { "C.4.1",
        "828684418cf1e3c2e5f23a6ba0ab90f4ff",
        IV, {
            { ":method", "GET" },
            { ":scheme", "http" },
            { ":path", "/" },
            { ":authority", "www.example.com" }
        },
        I, {
            { ":authority", "www.example.com" }
        },
        LVII },
    { "C.4.2",
        "828684be5886a8eb10649cbf",
        V, {
            { ":method", "GET" },
            { ":scheme", "http" },
            { ":path", "/" },
            { ":authority", "www.example.com" },
            { "cache-control", "no-cache" }
        },
        II, {
            { "cache-control", "no-cache" },
            { ":authority", "www.example.com" }
        },
        CX },
    { "C.4.3",
        "828785bf408825a849e95ba97d7f8925a849e95bb8e8b4bf",
        V, {
            { ":method", "GET" },
            { ":scheme", "https" },
            { ":path", "/index.html" },
            { ":authority", "www.example.com" },
            { "custom-key", "custom-value" }
        },
        III, {
            { "custom-key", "custom-value" },
            { "cache-control", "no-cache" },
            { ":authority", "www.example.com" }
        },
        CLXIV },
    { "C.5.1",
        "4803333032580770726976617465611d4d6f6e2c203231204f637420"
        "323031332032303a31333a323120474d546e1768747470733a2f2f77"
        "77772e6578616d706c652e636f6d",
        IV, {
            { ":status", "302" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "location", "https://www.example.com" }
        },
        IV, {
            { "location", "https://www.example.com" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "cache-control", "private" },
            { ":status", "302" }
        },
        CCXXII },
    { "C.5.2",
        "4803333037c1c0bf",
        IV, {
            { ":status", "307" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "location", "https://www.example.com" }
        },
        IV, {
            { ":status", "307" },
            { "location", "https://www.example.com" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "cache-control", "private" }
        },
        CCXXII },
    { "C.5.3",
        "88c1611d4d6f6e2c203231204f637420323031332032303a31333a32"
        "3220474d54c05a04677a69707738666f6f3d4153444a4b48514b425a"
        "584f5157454f50495541585157454f49553b206d61782d6167653d33"
        "3630303b2076657273696f6e3d31",
        VI, {
            { ":status", "200" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:22 GMT" },
            { "location", "https://www.example.com" },
            { "content-encoding", "gzip" },
            { "set-cookie", "foo=ASDJKHQKBZXOQWEOPIUAXQWEOIU; max-age=3600; version=1" }
        },
        III, {
            { "set-cookie", "foo=ASDJKHQKBZXOQWEOPIUAXQWEOIU; max-age=3600; version=1" },
            { "content-encoding", "gzip" },
            { "date", "Mon, 21 Oct 2013 20:13:22 GMT" }
        },
        CCXV },
    { "C.6.1",
        "488264025885aec3771a4b6196d07abe941054d444a8200595040b81"
        "66e082a62d1bff6e919d29ad171863c78f0b97c8e9ae82ae43d3",
        IV, {
            { ":status", "302" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "location", "https://www.example.com" }
        },
        IV, {
            { "location", "https://www.example.com" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "cache-control", "private" },
            { ":status", "302" }
        },
        CCXXII },
    { "C.6.2",
        "4883640effc1c0bf",
        IV, {
            { ":status", "307" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "location", "https://www.example.com" }
        },
        IV, {
            { ":status", "307" },
            { "location", "https://www.example.com" },
            { "date", "Mon, 21 Oct 2013 20:13:21 GMT" },
            { "cache-control", "private" }
        },
        CCXXII },
    { "C.6.3",
        "88c16196d07abe941054d444a8200595040b8166e084a62d1bffc05a"
        "839bd9ab77ad94e7821dd7f2e6c7b335dfdfcd5b3960d5af27087f36"
        "72c1ab270fb5291f9587316065c003ed4ee5b1063d5007",
        VI, {
            { ":status", "200" },
            { "cache-control", "private" },
            { "date", "Mon, 21 Oct 2013 20:13:22 GMT" },
            { "location", "https://www.example.com" },
            { "content-encoding", "gzip" },
            { "set-cookie", "foo=ASDJKHQKBZXOQWEOPIUAXQWEOIU; max-age=3600; version=1" }
        },
        III, {
            { "set-cookie", "foo=ASDJKHQKBZXOQWEOPIUAXQWEOIU; max-age=3600; version=1" },
            { "content-encoding", "gzip" },
            { "date", "Mon, 21 Oct 2013 20:13:22 GMT" }
        },
        CCXV }
};

#define EXEMPLA_RFC7541_NUMERUS \
    (magnitudo(EXEMPLA_RFC7541) / magnitudo(EXEMPLA_RFC7541[ZEPHYRUM]))

#endif /* RFC7541_EXEMPLA_H */
