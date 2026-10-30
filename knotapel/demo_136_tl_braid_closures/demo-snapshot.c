/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_136_tl_braid_closures/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): 28cf334612d8f1c5356024477473b17df49090d2
 * Regenerate: ./knotapel/archive.sh knotapel/demo_136_tl_braid_closures/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   64068da453692e55d89f3395ecd9a0e9350cfa29  include/anulus.h
 *   6f9b7a043cebe2912c612139453b66fcb04770cb  include/chorda_aedificator.h
 *   7db315706b013efbb850928c78e1c13b0fb72362  include/chorda.h
 *   37b6fb1c16e76827120976705e418e11c89406a6  include/congruentia.h
 *   3c0f81a81b4a6d9f8a0a1f9944e508027d96dcae  include/contractio.h
 *   53645f652dd16a7a8c8ad79df9e2e70289ee5e52  include/fractio.h
 *   3e213f5458ec1104e3b0ce27da019c97d63dc41d  include/interpolatio.h
 *   38984a165eab7e5cc0106a7ad31fe12f66c3df41  include/laqueus.h
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   bfde2073ff18d6f5be1c3844556cc82d39ee0937  include/magnus.h
 *   423a176668f285b2661452e215cdf84259389d93  include/matrix.h
 *   7e572ff53871833293550d16eee4eeeb43730a3e  include/piscina.h
 *   95f5c575951519ca8cee6170ec3a4519b284d0c4  include/polynomium.h
 *   a34b9f2536efa81a09cea36f9a9acba28fb4b2fc  include/postulata_posix.h
 *   161daa5e9105613138e1126678662a71120abb84  include/situs.h
 *   91e1de36acad556015e29cef636227ac270a6d56  include/sors.h
 *   14c612270e785565b2aeb7b53850a9b905462060  include/xar.h
 *   9f20940b276fc89410cce3ddffdd62a77ac12a29  lib/anulus.c
 *   042f5c148e14f2769ccc1da2f5ffd01dd72ee1f2  lib/chorda_aedificator.c
 *   b4c8c649c03e84eca53408282c39d5b0e24c6016  lib/chorda.c
 *   c446f3092b17cdca38b584886d86a5ca80856eac  lib/congruentia.c
 *   03eac1f2564c0cf7bc166e4a2c1b12c4aee686fe  lib/contractio.c
 *   61ec3b3106345cce9ea64477aafe7d1072030be7  lib/fractio.c
 *   8264967888e45a5a09d763c03cd604603035d43f  lib/interpolatio.c
 *   efed04ba1d88fe554f7dd45ed7b6b54c85fd8173  lib/laqueus.c
 *   fd94a8d26a4bd4b340875ac0a1551bc3f3c55e8e  lib/magnus.c
 *   5fc06983f571b6cf493cc884ab9647db9f499d44  lib/matrix.c
 *   364f2d5b99fb6ca39ae93add2091ef3adcd035f7  lib/piscina.c
 *   9967d4ccc5d5f5e18bf84f716b4f8fb7b7964a85  lib/polynomium.c
 *   216c408f5f2064e64312685d551841f3cabdc92f  lib/situs.c
 *   a1a3bbc96bc609e89e4098ef95f770c5e10a3ea5  lib/sors.c
 *   6d5b9cc80ff8ae053cad83bc764a0c768e1ff523  lib/xar.c
 *   0bb0d74dcf0257460fbe0806e584a1a173da3ed2  knotapel/demo_136_tl_braid_closures/main.c (uncommitted, embedded verbatim)
 */

#line 1 "include/postulata_posix.h"
/* postulata_posix.h - postulata platformae pro superficie POSIX
 *
 * SUTURA praeprocessoris pura: interfacies portabilis, mores
 * per-platformam. glibc sub -std=c89 declarationes POSIX CELAT nisi
 * macro probationis proprietatum ante caput systematis primum
 * definitur; Darwin et musl ordinarie permissivi sunt. Sine hoc
 * capite plagula quaeque POSIX-utens in Linux glibc cadit
 * (tcp_posix.c: XX errores ex radicibus IV celatis - mensuratum).
 *
 * CUR _DEFAULT_SOURCE: sonda Docker 2026-08-03 (glibc 2.35 gcc 11.4;
 * musl 1.2.5 gcc 13.2; VI plagulae x V variantes - acta in actis
 * tabularii 01KYTGNA36) mensuravit: _DEFAULT_SOURCE omnia
 * macro-sanabilia in AMBABUS libc sanat et in Darwin nihil agit.
 * Variantes strictae PEIORES sunt, non aequales: _XOPEN_SOURCE 700
 * et _POSIX_C_SOURCE usleep RE-CELANT (XPG7 sustulit). Decretum
 * 01KZ3RYZWK: caput unum, non definitiones per plagulam.
 *
 * LEX (codex examinis 85 custodit): hoc caput inclusio PRIMA
 * plagulae POSIX-utentis sit - ante caput proprium, ante latina.h.
 * features.h glibc copiam SEMEL figit, primo tactu capitis systematis
 * cuiuslibet; latina.h stddef.h trahit, ergo "prima" ad litteram.
 *
 * Nomen _DEFAULT_SOURCE classis reservatae est (C89 7.1.3) -
 * REFERIMUS interruptorem glibc documentatum, non coinamus (eadem
 * licentia qua externa systematis referuntur).
 */

#ifndef POSTULATA_POSIX_H
#define POSTULATA_POSIX_H

#define _DEFAULT_SOURCE 1

#endif /* POSTULATA_POSIX_H */
#line 1 "include/latina.h"
/* latina.h - Verba clavis C89 Latine reddita (lexicon domus) */
#ifndef LATINA_H
#define LATINA_H

#include <stddef.h>

#define character     char
#define brevis             short
#define integer         int
#define longus            long
#define fluitans        float
#define duplex            double

#define vacuum            void
#define signatus         signed
#define insignatus  unsigned
#define constans        const
#define volatilis        volatile
#define sponte            auto
#define registrum     register
#define staticus         static
#define    externus         extern

#define si                    if
#define alioquin        else
#define commutatio    switch
#define casus                case
#define ordinarius    default
#define per                    for
#define dum                 while
#define fac                 do
#define frange             break
#define perge             continue
#define salta                goto
#define redde                return

#define structura        struct
#define unio                 union
#define enumeratio     enum
#define nomen             typedef

#define magnitudo     sizeof

#define principale     main

#define NIHIL                NULL
#define VERUM             1
#define FALSUM             0

/* NUMERI ROMANI - GENERATUM ex numerus_romanus_scribere
 * (tools/latina_numeri.sh -scribere) - NE MANU MUTES; porta
 * 'generata' iudicat. Omnes ZEPHYRUM-MMMCMXCIX ordine: numerus
 * Romanus classicus ad MMMCMXCIX finit - maiores ut expressio
 * (vinculum ut '* M'): IV * M (4000), IV * MXXIV (4096); vide
 * numerus_romanus_exprimere. Omne numerale identificator
 * reservatus est: capita systematis ANTE latina.h includenda
 * (dns_util.h membra 'MD' et 'MX' habet). */
#define ZEPHYRUM         0
#define I                1
#define II               2
#define III              3
#define IV               4
#define V                5
#define VI               6
#define VII              7
#define VIII             8
#define IX               9
#define X                10
#define XI               11
#define XII              12
#define XIII             13
#define XIV              14
#define XV               15
#define XVI              16
#define XVII             17
#define XVIII            18
#define XIX              19
#define XX               20
#define XXI              21
#define XXII             22
#define XXIII            23
#define XXIV             24
#define XXV              25
#define XXVI             26
#define XXVII            27
#define XXVIII           28
#define XXIX             29
#define XXX              30
#define XXXI             31
#define XXXII            32
#define XXXIII           33
#define XXXIV            34
#define XXXV             35
#define XXXVI            36
#define XXXVII           37
#define XXXVIII          38
#define XXXIX            39
#define XL               40
#define XLI              41
#define XLII             42
#define XLIII            43
#define XLIV             44
#define XLV              45
#define XLVI             46
#define XLVII            47
#define XLVIII           48
#define XLIX             49
#define L                50
#define LI               51
#define LII              52
#define LIII             53
#define LIV              54
#define LV               55
#define LVI              56
#define LVII             57
#define LVIII            58
#define LIX              59
#define LX               60
#define LXI              61
#define LXII             62
#define LXIII            63
#define LXIV             64
#define LXV              65
#define LXVI             66
#define LXVII            67
#define LXVIII           68
#define LXIX             69
#define LXX              70
#define LXXI             71
#define LXXII            72
#define LXXIII           73
#define LXXIV            74
#define LXXV             75
#define LXXVI            76
#define LXXVII           77
#define LXXVIII          78
#define LXXIX            79
#define LXXX             80
#define LXXXI            81
#define LXXXII           82
#define LXXXIII          83
#define LXXXIV           84
#define LXXXV            85
#define LXXXVI           86
#define LXXXVII          87
#define LXXXVIII         88
#define LXXXIX           89
#define XC               90
#define XCI              91
#define XCII             92
#define XCIII            93
#define XCIV             94
#define XCV              95
#define XCVI             96
#define XCVII            97
#define XCVIII           98
#define XCIX             99
#define C                100
#define CI               101
#define CII              102
#define CIII             103
#define CIV              104
#define CV               105
#define CVI              106
#define CVII             107
#define CVIII            108
#define CIX              109
#define CX               110
#define CXI              111
#define CXII             112
#define CXIII            113
#define CXIV             114
#define CXV              115
#define CXVI             116
#define CXVII            117
#define CXVIII           118
#define CXIX             119
#define CXX              120
#define CXXI             121
#define CXXII            122
#define CXXIII           123
#define CXXIV            124
#define CXXV             125
#define CXXVI            126
#define CXXVII           127
#define CXXVIII          128
#define CXXIX            129
#define CXXX             130
#define CXXXI            131
#define CXXXII           132
#define CXXXIII          133
#define CXXXIV           134
#define CXXXV            135
#define CXXXVI           136
#define CXXXVII          137
#define CXXXVIII         138
#define CXXXIX           139
#define CXL              140
#define CXLI             141
#define CXLII            142
#define CXLIII           143
#define CXLIV            144
#define CXLV             145
#define CXLVI            146
#define CXLVII           147
#define CXLVIII          148
#define CXLIX            149
#define CL               150
#define CLI              151
#define CLII             152
#define CLIII            153
#define CLIV             154
#define CLV              155
#define CLVI             156
#define CLVII            157
#define CLVIII           158
#define CLIX             159
#define CLX              160
#define CLXI             161
#define CLXII            162
#define CLXIII           163
#define CLXIV            164
#define CLXV             165
#define CLXVI            166
#define CLXVII           167
#define CLXVIII          168
#define CLXIX            169
#define CLXX             170
#define CLXXI            171
#define CLXXII           172
#define CLXXIII          173
#define CLXXIV           174
#define CLXXV            175
#define CLXXVI           176
#define CLXXVII          177
#define CLXXVIII         178
#define CLXXIX           179
#define CLXXX            180
#define CLXXXI           181
#define CLXXXII          182
#define CLXXXIII         183
#define CLXXXIV          184
#define CLXXXV           185
#define CLXXXVI          186
#define CLXXXVII         187
#define CLXXXVIII        188
#define CLXXXIX          189
#define CXC              190
#define CXCI             191
#define CXCII            192
#define CXCIII           193
#define CXCIV            194
#define CXCV             195
#define CXCVI            196
#define CXCVII           197
#define CXCVIII          198
#define CXCIX            199
#define CC               200
#define CCI              201
#define CCII             202
#define CCIII            203
#define CCIV             204
#define CCV              205
#define CCVI             206
#define CCVII            207
#define CCVIII           208
#define CCIX             209
#define CCX              210
#define CCXI             211
#define CCXII            212
#define CCXIII           213
#define CCXIV            214
#define CCXV             215
#define CCXVI            216
#define CCXVII           217
#define CCXVIII          218
#define CCXIX            219
#define CCXX             220
#define CCXXI            221
#define CCXXII           222
#define CCXXIII          223
#define CCXXIV           224
#define CCXXV            225
#define CCXXVI           226
#define CCXXVII          227
#define CCXXVIII         228
#define CCXXIX           229
#define CCXXX            230
#define CCXXXI           231
#define CCXXXII          232
#define CCXXXIII         233
#define CCXXXIV          234
#define CCXXXV           235
#define CCXXXVI          236
#define CCXXXVII         237
#define CCXXXVIII        238
#define CCXXXIX          239
#define CCXL             240
#define CCXLI            241
#define CCXLII           242
#define CCXLIII          243
#define CCXLIV           244
#define CCXLV            245
#define CCXLVI           246
#define CCXLVII          247
#define CCXLVIII         248
#define CCXLIX           249
#define CCL              250
#define CCLI             251
#define CCLII            252
#define CCLIII           253
#define CCLIV            254
#define CCLV             255
#define CCLVI            256
#define CCLVII           257
#define CCLVIII          258
#define CCLIX            259
#define CCLX             260
#define CCLXI            261
#define CCLXII           262
#define CCLXIII          263
#define CCLXIV           264
#define CCLXV            265
#define CCLXVI           266
#define CCLXVII          267
#define CCLXVIII         268
#define CCLXIX           269
#define CCLXX            270
#define CCLXXI           271
#define CCLXXII          272
#define CCLXXIII         273
#define CCLXXIV          274
#define CCLXXV           275
#define CCLXXVI          276
#define CCLXXVII         277
#define CCLXXVIII        278
#define CCLXXIX          279
#define CCLXXX           280
#define CCLXXXI          281
#define CCLXXXII         282
#define CCLXXXIII        283
#define CCLXXXIV         284
#define CCLXXXV          285
#define CCLXXXVI         286
#define CCLXXXVII        287
#define CCLXXXVIII       288
#define CCLXXXIX         289
#define CCXC             290
#define CCXCI            291
#define CCXCII           292
#define CCXCIII          293
#define CCXCIV           294
#define CCXCV            295
#define CCXCVI           296
#define CCXCVII          297
#define CCXCVIII         298
#define CCXCIX           299
#define CCC              300
#define CCCI             301
#define CCCII            302
#define CCCIII           303
#define CCCIV            304
#define CCCV             305
#define CCCVI            306
#define CCCVII           307
#define CCCVIII          308
#define CCCIX            309
#define CCCX             310
#define CCCXI            311
#define CCCXII           312
#define CCCXIII          313
#define CCCXIV           314
#define CCCXV            315
#define CCCXVI           316
#define CCCXVII          317
#define CCCXVIII         318
#define CCCXIX           319
#define CCCXX            320
#define CCCXXI           321
#define CCCXXII          322
#define CCCXXIII         323
#define CCCXXIV          324
#define CCCXXV           325
#define CCCXXVI          326
#define CCCXXVII         327
#define CCCXXVIII        328
#define CCCXXIX          329
#define CCCXXX           330
#define CCCXXXI          331
#define CCCXXXII         332
#define CCCXXXIII        333
#define CCCXXXIV         334
#define CCCXXXV          335
#define CCCXXXVI         336
#define CCCXXXVII        337
#define CCCXXXVIII       338
#define CCCXXXIX         339
#define CCCXL            340
#define CCCXLI           341
#define CCCXLII          342
#define CCCXLIII         343
#define CCCXLIV          344
#define CCCXLV           345
#define CCCXLVI          346
#define CCCXLVII         347
#define CCCXLVIII        348
#define CCCXLIX          349
#define CCCL             350
#define CCCLI            351
#define CCCLII           352
#define CCCLIII          353
#define CCCLIV           354
#define CCCLV            355
#define CCCLVI           356
#define CCCLVII          357
#define CCCLVIII         358
#define CCCLIX           359
#define CCCLX            360
#define CCCLXI           361
#define CCCLXII          362
#define CCCLXIII         363
#define CCCLXIV          364
#define CCCLXV           365
#define CCCLXVI          366
#define CCCLXVII         367
#define CCCLXVIII        368
#define CCCLXIX          369
#define CCCLXX           370
#define CCCLXXI          371
#define CCCLXXII         372
#define CCCLXXIII        373
#define CCCLXXIV         374
#define CCCLXXV          375
#define CCCLXXVI         376
#define CCCLXXVII        377
#define CCCLXXVIII       378
#define CCCLXXIX         379
#define CCCLXXX          380
#define CCCLXXXI         381
#define CCCLXXXII        382
#define CCCLXXXIII       383
#define CCCLXXXIV        384
#define CCCLXXXV         385
#define CCCLXXXVI        386
#define CCCLXXXVII       387
#define CCCLXXXVIII      388
#define CCCLXXXIX        389
#define CCCXC            390
#define CCCXCI           391
#define CCCXCII          392
#define CCCXCIII         393
#define CCCXCIV          394
#define CCCXCV           395
#define CCCXCVI          396
#define CCCXCVII         397
#define CCCXCVIII        398
#define CCCXCIX          399
#define CD               400
#define CDI              401
#define CDII             402
#define CDIII            403
#define CDIV             404
#define CDV              405
#define CDVI             406
#define CDVII            407
#define CDVIII           408
#define CDIX             409
#define CDX              410
#define CDXI             411
#define CDXII            412
#define CDXIII           413
#define CDXIV            414
#define CDXV             415
#define CDXVI            416
#define CDXVII           417
#define CDXVIII          418
#define CDXIX            419
#define CDXX             420
#define CDXXI            421
#define CDXXII           422
#define CDXXIII          423
#define CDXXIV           424
#define CDXXV            425
#define CDXXVI           426
#define CDXXVII          427
#define CDXXVIII         428
#define CDXXIX           429
#define CDXXX            430
#define CDXXXI           431
#define CDXXXII          432
#define CDXXXIII         433
#define CDXXXIV          434
#define CDXXXV           435
#define CDXXXVI          436
#define CDXXXVII         437
#define CDXXXVIII        438
#define CDXXXIX          439
#define CDXL             440
#define CDXLI            441
#define CDXLII           442
#define CDXLIII          443
#define CDXLIV           444
#define CDXLV            445
#define CDXLVI           446
#define CDXLVII          447
#define CDXLVIII         448
#define CDXLIX           449
#define CDL              450
#define CDLI             451
#define CDLII            452
#define CDLIII           453
#define CDLIV            454
#define CDLV             455
#define CDLVI            456
#define CDLVII           457
#define CDLVIII          458
#define CDLIX            459
#define CDLX             460
#define CDLXI            461
#define CDLXII           462
#define CDLXIII          463
#define CDLXIV           464
#define CDLXV            465
#define CDLXVI           466
#define CDLXVII          467
#define CDLXVIII         468
#define CDLXIX           469
#define CDLXX            470
#define CDLXXI           471
#define CDLXXII          472
#define CDLXXIII         473
#define CDLXXIV          474
#define CDLXXV           475
#define CDLXXVI          476
#define CDLXXVII         477
#define CDLXXVIII        478
#define CDLXXIX          479
#define CDLXXX           480
#define CDLXXXI          481
#define CDLXXXII         482
#define CDLXXXIII        483
#define CDLXXXIV         484
#define CDLXXXV          485
#define CDLXXXVI         486
#define CDLXXXVII        487
#define CDLXXXVIII       488
#define CDLXXXIX         489
#define CDXC             490
#define CDXCI            491
#define CDXCII           492
#define CDXCIII          493
#define CDXCIV           494
#define CDXCV            495
#define CDXCVI           496
#define CDXCVII          497
#define CDXCVIII         498
#define CDXCIX           499
#define D                500
#define DI               501
#define DII              502
#define DIII             503
#define DIV              504
#define DV               505
#define DVI              506
#define DVII             507
#define DVIII            508
#define DIX              509
#define DX               510
#define DXI              511
#define DXII             512
#define DXIII            513
#define DXIV             514
#define DXV              515
#define DXVI             516
#define DXVII            517
#define DXVIII           518
#define DXIX             519
#define DXX              520
#define DXXI             521
#define DXXII            522
#define DXXIII           523
#define DXXIV            524
#define DXXV             525
#define DXXVI            526
#define DXXVII           527
#define DXXVIII          528
#define DXXIX            529
#define DXXX             530
#define DXXXI            531
#define DXXXII           532
#define DXXXIII          533
#define DXXXIV           534
#define DXXXV            535
#define DXXXVI           536
#define DXXXVII          537
#define DXXXVIII         538
#define DXXXIX           539
#define DXL              540
#define DXLI             541
#define DXLII            542
#define DXLIII           543
#define DXLIV            544
#define DXLV             545
#define DXLVI            546
#define DXLVII           547
#define DXLVIII          548
#define DXLIX            549
#define DL               550
#define DLI              551
#define DLII             552
#define DLIII            553
#define DLIV             554
#define DLV              555
#define DLVI             556
#define DLVII            557
#define DLVIII           558
#define DLIX             559
#define DLX              560
#define DLXI             561
#define DLXII            562
#define DLXIII           563
#define DLXIV            564
#define DLXV             565
#define DLXVI            566
#define DLXVII           567
#define DLXVIII          568
#define DLXIX            569
#define DLXX             570
#define DLXXI            571
#define DLXXII           572
#define DLXXIII          573
#define DLXXIV           574
#define DLXXV            575
#define DLXXVI           576
#define DLXXVII          577
#define DLXXVIII         578
#define DLXXIX           579
#define DLXXX            580
#define DLXXXI           581
#define DLXXXII          582
#define DLXXXIII         583
#define DLXXXIV          584
#define DLXXXV           585
#define DLXXXVI          586
#define DLXXXVII         587
#define DLXXXVIII        588
#define DLXXXIX          589
#define DXC              590
#define DXCI             591
#define DXCII            592
#define DXCIII           593
#define DXCIV            594
#define DXCV             595
#define DXCVI            596
#define DXCVII           597
#define DXCVIII          598
#define DXCIX            599
#define DC               600
#define DCI              601
#define DCII             602
#define DCIII            603
#define DCIV             604
#define DCV              605
#define DCVI             606
#define DCVII            607
#define DCVIII           608
#define DCIX             609
#define DCX              610
#define DCXI             611
#define DCXII            612
#define DCXIII           613
#define DCXIV            614
#define DCXV             615
#define DCXVI            616
#define DCXVII           617
#define DCXVIII          618
#define DCXIX            619
#define DCXX             620
#define DCXXI            621
#define DCXXII           622
#define DCXXIII          623
#define DCXXIV           624
#define DCXXV            625
#define DCXXVI           626
#define DCXXVII          627
#define DCXXVIII         628
#define DCXXIX           629
#define DCXXX            630
#define DCXXXI           631
#define DCXXXII          632
#define DCXXXIII         633
#define DCXXXIV          634
#define DCXXXV           635
#define DCXXXVI          636
#define DCXXXVII         637
#define DCXXXVIII        638
#define DCXXXIX          639
#define DCXL             640
#define DCXLI            641
#define DCXLII           642
#define DCXLIII          643
#define DCXLIV           644
#define DCXLV            645
#define DCXLVI           646
#define DCXLVII          647
#define DCXLVIII         648
#define DCXLIX           649
#define DCL              650
#define DCLI             651
#define DCLII            652
#define DCLIII           653
#define DCLIV            654
#define DCLV             655
#define DCLVI            656
#define DCLVII           657
#define DCLVIII          658
#define DCLIX            659
#define DCLX             660
#define DCLXI            661
#define DCLXII           662
#define DCLXIII          663
#define DCLXIV           664
#define DCLXV            665
#define DCLXVI           666
#define DCLXVII          667
#define DCLXVIII         668
#define DCLXIX           669
#define DCLXX            670
#define DCLXXI           671
#define DCLXXII          672
#define DCLXXIII         673
#define DCLXXIV          674
#define DCLXXV           675
#define DCLXXVI          676
#define DCLXXVII         677
#define DCLXXVIII        678
#define DCLXXIX          679
#define DCLXXX           680
#define DCLXXXI          681
#define DCLXXXII         682
#define DCLXXXIII        683
#define DCLXXXIV         684
#define DCLXXXV          685
#define DCLXXXVI         686
#define DCLXXXVII        687
#define DCLXXXVIII       688
#define DCLXXXIX         689
#define DCXC             690
#define DCXCI            691
#define DCXCII           692
#define DCXCIII          693
#define DCXCIV           694
#define DCXCV            695
#define DCXCVI           696
#define DCXCVII          697
#define DCXCVIII         698
#define DCXCIX           699
#define DCC              700
#define DCCI             701
#define DCCII            702
#define DCCIII           703
#define DCCIV            704
#define DCCV             705
#define DCCVI            706
#define DCCVII           707
#define DCCVIII          708
#define DCCIX            709
#define DCCX             710
#define DCCXI            711
#define DCCXII           712
#define DCCXIII          713
#define DCCXIV           714
#define DCCXV            715
#define DCCXVI           716
#define DCCXVII          717
#define DCCXVIII         718
#define DCCXIX           719
#define DCCXX            720
#define DCCXXI           721
#define DCCXXII          722
#define DCCXXIII         723
#define DCCXXIV          724
#define DCCXXV           725
#define DCCXXVI          726
#define DCCXXVII         727
#define DCCXXVIII        728
#define DCCXXIX          729
#define DCCXXX           730
#define DCCXXXI          731
#define DCCXXXII         732
#define DCCXXXIII        733
#define DCCXXXIV         734
#define DCCXXXV          735
#define DCCXXXVI         736
#define DCCXXXVII        737
#define DCCXXXVIII       738
#define DCCXXXIX         739
#define DCCXL            740
#define DCCXLI           741
#define DCCXLII          742
#define DCCXLIII         743
#define DCCXLIV          744
#define DCCXLV           745
#define DCCXLVI          746
#define DCCXLVII         747
#define DCCXLVIII        748
#define DCCXLIX          749
#define DCCL             750
#define DCCLI            751
#define DCCLII           752
#define DCCLIII          753
#define DCCLIV           754
#define DCCLV            755
#define DCCLVI           756
#define DCCLVII          757
#define DCCLVIII         758
#define DCCLIX           759
#define DCCLX            760
#define DCCLXI           761
#define DCCLXII          762
#define DCCLXIII         763
#define DCCLXIV          764
#define DCCLXV           765
#define DCCLXVI          766
#define DCCLXVII         767
#define DCCLXVIII        768
#define DCCLXIX          769
#define DCCLXX           770
#define DCCLXXI          771
#define DCCLXXII         772
#define DCCLXXIII        773
#define DCCLXXIV         774
#define DCCLXXV          775
#define DCCLXXVI         776
#define DCCLXXVII        777
#define DCCLXXVIII       778
#define DCCLXXIX         779
#define DCCLXXX          780
#define DCCLXXXI         781
#define DCCLXXXII        782
#define DCCLXXXIII       783
#define DCCLXXXIV        784
#define DCCLXXXV         785
#define DCCLXXXVI        786
#define DCCLXXXVII       787
#define DCCLXXXVIII      788
#define DCCLXXXIX        789
#define DCCXC            790
#define DCCXCI           791
#define DCCXCII          792
#define DCCXCIII         793
#define DCCXCIV          794
#define DCCXCV           795
#define DCCXCVI          796
#define DCCXCVII         797
#define DCCXCVIII        798
#define DCCXCIX          799
#define DCCC             800
#define DCCCI            801
#define DCCCII           802
#define DCCCIII          803
#define DCCCIV           804
#define DCCCV            805
#define DCCCVI           806
#define DCCCVII          807
#define DCCCVIII         808
#define DCCCIX           809
#define DCCCX            810
#define DCCCXI           811
#define DCCCXII          812
#define DCCCXIII         813
#define DCCCXIV          814
#define DCCCXV           815
#define DCCCXVI          816
#define DCCCXVII         817
#define DCCCXVIII        818
#define DCCCXIX          819
#define DCCCXX           820
#define DCCCXXI          821
#define DCCCXXII         822
#define DCCCXXIII        823
#define DCCCXXIV         824
#define DCCCXXV          825
#define DCCCXXVI         826
#define DCCCXXVII        827
#define DCCCXXVIII       828
#define DCCCXXIX         829
#define DCCCXXX          830
#define DCCCXXXI         831
#define DCCCXXXII        832
#define DCCCXXXIII       833
#define DCCCXXXIV        834
#define DCCCXXXV         835
#define DCCCXXXVI        836
#define DCCCXXXVII       837
#define DCCCXXXVIII      838
#define DCCCXXXIX        839
#define DCCCXL           840
#define DCCCXLI          841
#define DCCCXLII         842
#define DCCCXLIII        843
#define DCCCXLIV         844
#define DCCCXLV          845
#define DCCCXLVI         846
#define DCCCXLVII        847
#define DCCCXLVIII       848
#define DCCCXLIX         849
#define DCCCL            850
#define DCCCLI           851
#define DCCCLII          852
#define DCCCLIII         853
#define DCCCLIV          854
#define DCCCLV           855
#define DCCCLVI          856
#define DCCCLVII         857
#define DCCCLVIII        858
#define DCCCLIX          859
#define DCCCLX           860
#define DCCCLXI          861
#define DCCCLXII         862
#define DCCCLXIII        863
#define DCCCLXIV         864
#define DCCCLXV          865
#define DCCCLXVI         866
#define DCCCLXVII        867
#define DCCCLXVIII       868
#define DCCCLXIX         869
#define DCCCLXX          870
#define DCCCLXXI         871
#define DCCCLXXII        872
#define DCCCLXXIII       873
#define DCCCLXXIV        874
#define DCCCLXXV         875
#define DCCCLXXVI        876
#define DCCCLXXVII       877
#define DCCCLXXVIII      878
#define DCCCLXXIX        879
#define DCCCLXXX         880
#define DCCCLXXXI        881
#define DCCCLXXXII       882
#define DCCCLXXXIII      883
#define DCCCLXXXIV       884
#define DCCCLXXXV        885
#define DCCCLXXXVI       886
#define DCCCLXXXVII      887
#define DCCCLXXXVIII     888
#define DCCCLXXXIX       889
#define DCCCXC           890
#define DCCCXCI          891
#define DCCCXCII         892
#define DCCCXCIII        893
#define DCCCXCIV         894
#define DCCCXCV          895
#define DCCCXCVI         896
#define DCCCXCVII        897
#define DCCCXCVIII       898
#define DCCCXCIX         899
#define CM               900
#define CMI              901
#define CMII             902
#define CMIII            903
#define CMIV             904
#define CMV              905
#define CMVI             906
#define CMVII            907
#define CMVIII           908
#define CMIX             909
#define CMX              910
#define CMXI             911
#define CMXII            912
#define CMXIII           913
#define CMXIV            914
#define CMXV             915
#define CMXVI            916
#define CMXVII           917
#define CMXVIII          918
#define CMXIX            919
#define CMXX             920
#define CMXXI            921
#define CMXXII           922
#define CMXXIII          923
#define CMXXIV           924
#define CMXXV            925
#define CMXXVI           926
#define CMXXVII          927
#define CMXXVIII         928
#define CMXXIX           929
#define CMXXX            930
#define CMXXXI           931
#define CMXXXII          932
#define CMXXXIII         933
#define CMXXXIV          934
#define CMXXXV           935
#define CMXXXVI          936
#define CMXXXVII         937
#define CMXXXVIII        938
#define CMXXXIX          939
#define CMXL             940
#define CMXLI            941
#define CMXLII           942
#define CMXLIII          943
#define CMXLIV           944
#define CMXLV            945
#define CMXLVI           946
#define CMXLVII          947
#define CMXLVIII         948
#define CMXLIX           949
#define CML              950
#define CMLI             951
#define CMLII            952
#define CMLIII           953
#define CMLIV            954
#define CMLV             955
#define CMLVI            956
#define CMLVII           957
#define CMLVIII          958
#define CMLIX            959
#define CMLX             960
#define CMLXI            961
#define CMLXII           962
#define CMLXIII          963
#define CMLXIV           964
#define CMLXV            965
#define CMLXVI           966
#define CMLXVII          967
#define CMLXVIII         968
#define CMLXIX           969
#define CMLXX            970
#define CMLXXI           971
#define CMLXXII          972
#define CMLXXIII         973
#define CMLXXIV          974
#define CMLXXV           975
#define CMLXXVI          976
#define CMLXXVII         977
#define CMLXXVIII        978
#define CMLXXIX          979
#define CMLXXX           980
#define CMLXXXI          981
#define CMLXXXII         982
#define CMLXXXIII        983
#define CMLXXXIV         984
#define CMLXXXV          985
#define CMLXXXVI         986
#define CMLXXXVII        987
#define CMLXXXVIII       988
#define CMLXXXIX         989
#define CMXC             990
#define CMXCI            991
#define CMXCII           992
#define CMXCIII          993
#define CMXCIV           994
#define CMXCV            995
#define CMXCVI           996
#define CMXCVII          997
#define CMXCVIII         998
#define CMXCIX           999
#define M                1000
#define MI               1001
#define MII              1002
#define MIII             1003
#define MIV              1004
#define MV               1005
#define MVI              1006
#define MVII             1007
#define MVIII            1008
#define MIX              1009
#define MX               1010
#define MXI              1011
#define MXII             1012
#define MXIII            1013
#define MXIV             1014
#define MXV              1015
#define MXVI             1016
#define MXVII            1017
#define MXVIII           1018
#define MXIX             1019
#define MXX              1020
#define MXXI             1021
#define MXXII            1022
#define MXXIII           1023
#define MXXIV            1024
#define MXXV             1025
#define MXXVI            1026
#define MXXVII           1027
#define MXXVIII          1028
#define MXXIX            1029
#define MXXX             1030
#define MXXXI            1031
#define MXXXII           1032
#define MXXXIII          1033
#define MXXXIV           1034
#define MXXXV            1035
#define MXXXVI           1036
#define MXXXVII          1037
#define MXXXVIII         1038
#define MXXXIX           1039
#define MXL              1040
#define MXLI             1041
#define MXLII            1042
#define MXLIII           1043
#define MXLIV            1044
#define MXLV             1045
#define MXLVI            1046
#define MXLVII           1047
#define MXLVIII          1048
#define MXLIX            1049
#define ML               1050
#define MLI              1051
#define MLII             1052
#define MLIII            1053
#define MLIV             1054
#define MLV              1055
#define MLVI             1056
#define MLVII            1057
#define MLVIII           1058
#define MLIX             1059
#define MLX              1060
#define MLXI             1061
#define MLXII            1062
#define MLXIII           1063
#define MLXIV            1064
#define MLXV             1065
#define MLXVI            1066
#define MLXVII           1067
#define MLXVIII          1068
#define MLXIX            1069
#define MLXX             1070
#define MLXXI            1071
#define MLXXII           1072
#define MLXXIII          1073
#define MLXXIV           1074
#define MLXXV            1075
#define MLXXVI           1076
#define MLXXVII          1077
#define MLXXVIII         1078
#define MLXXIX           1079
#define MLXXX            1080
#define MLXXXI           1081
#define MLXXXII          1082
#define MLXXXIII         1083
#define MLXXXIV          1084
#define MLXXXV           1085
#define MLXXXVI          1086
#define MLXXXVII         1087
#define MLXXXVIII        1088
#define MLXXXIX          1089
#define MXC              1090
#define MXCI             1091
#define MXCII            1092
#define MXCIII           1093
#define MXCIV            1094
#define MXCV             1095
#define MXCVI            1096
#define MXCVII           1097
#define MXCVIII          1098
#define MXCIX            1099
#define MC               1100
#define MCI              1101
#define MCII             1102
#define MCIII            1103
#define MCIV             1104
#define MCV              1105
#define MCVI             1106
#define MCVII            1107
#define MCVIII           1108
#define MCIX             1109
#define MCX              1110
#define MCXI             1111
#define MCXII            1112
#define MCXIII           1113
#define MCXIV            1114
#define MCXV             1115
#define MCXVI            1116
#define MCXVII           1117
#define MCXVIII          1118
#define MCXIX            1119
#define MCXX             1120
#define MCXXI            1121
#define MCXXII           1122
#define MCXXIII          1123
#define MCXXIV           1124
#define MCXXV            1125
#define MCXXVI           1126
#define MCXXVII          1127
#define MCXXVIII         1128
#define MCXXIX           1129
#define MCXXX            1130
#define MCXXXI           1131
#define MCXXXII          1132
#define MCXXXIII         1133
#define MCXXXIV          1134
#define MCXXXV           1135
#define MCXXXVI          1136
#define MCXXXVII         1137
#define MCXXXVIII        1138
#define MCXXXIX          1139
#define MCXL             1140
#define MCXLI            1141
#define MCXLII           1142
#define MCXLIII          1143
#define MCXLIV           1144
#define MCXLV            1145
#define MCXLVI           1146
#define MCXLVII          1147
#define MCXLVIII         1148
#define MCXLIX           1149
#define MCL              1150
#define MCLI             1151
#define MCLII            1152
#define MCLIII           1153
#define MCLIV            1154
#define MCLV             1155
#define MCLVI            1156
#define MCLVII           1157
#define MCLVIII          1158
#define MCLIX            1159
#define MCLX             1160
#define MCLXI            1161
#define MCLXII           1162
#define MCLXIII          1163
#define MCLXIV           1164
#define MCLXV            1165
#define MCLXVI           1166
#define MCLXVII          1167
#define MCLXVIII         1168
#define MCLXIX           1169
#define MCLXX            1170
#define MCLXXI           1171
#define MCLXXII          1172
#define MCLXXIII         1173
#define MCLXXIV          1174
#define MCLXXV           1175
#define MCLXXVI          1176
#define MCLXXVII         1177
#define MCLXXVIII        1178
#define MCLXXIX          1179
#define MCLXXX           1180
#define MCLXXXI          1181
#define MCLXXXII         1182
#define MCLXXXIII        1183
#define MCLXXXIV         1184
#define MCLXXXV          1185
#define MCLXXXVI         1186
#define MCLXXXVII        1187
#define MCLXXXVIII       1188
#define MCLXXXIX         1189
#define MCXC             1190
#define MCXCI            1191
#define MCXCII           1192
#define MCXCIII          1193
#define MCXCIV           1194
#define MCXCV            1195
#define MCXCVI           1196
#define MCXCVII          1197
#define MCXCVIII         1198
#define MCXCIX           1199
#define MCC              1200
#define MCCI             1201
#define MCCII            1202
#define MCCIII           1203
#define MCCIV            1204
#define MCCV             1205
#define MCCVI            1206
#define MCCVII           1207
#define MCCVIII          1208
#define MCCIX            1209
#define MCCX             1210
#define MCCXI            1211
#define MCCXII           1212
#define MCCXIII          1213
#define MCCXIV           1214
#define MCCXV            1215
#define MCCXVI           1216
#define MCCXVII          1217
#define MCCXVIII         1218
#define MCCXIX           1219
#define MCCXX            1220
#define MCCXXI           1221
#define MCCXXII          1222
#define MCCXXIII         1223
#define MCCXXIV          1224
#define MCCXXV           1225
#define MCCXXVI          1226
#define MCCXXVII         1227
#define MCCXXVIII        1228
#define MCCXXIX          1229
#define MCCXXX           1230
#define MCCXXXI          1231
#define MCCXXXII         1232
#define MCCXXXIII        1233
#define MCCXXXIV         1234
#define MCCXXXV          1235
#define MCCXXXVI         1236
#define MCCXXXVII        1237
#define MCCXXXVIII       1238
#define MCCXXXIX         1239
#define MCCXL            1240
#define MCCXLI           1241
#define MCCXLII          1242
#define MCCXLIII         1243
#define MCCXLIV          1244
#define MCCXLV           1245
#define MCCXLVI          1246
#define MCCXLVII         1247
#define MCCXLVIII        1248
#define MCCXLIX          1249
#define MCCL             1250
#define MCCLI            1251
#define MCCLII           1252
#define MCCLIII          1253
#define MCCLIV           1254
#define MCCLV            1255
#define MCCLVI           1256
#define MCCLVII          1257
#define MCCLVIII         1258
#define MCCLIX           1259
#define MCCLX            1260
#define MCCLXI           1261
#define MCCLXII          1262
#define MCCLXIII         1263
#define MCCLXIV          1264
#define MCCLXV           1265
#define MCCLXVI          1266
#define MCCLXVII         1267
#define MCCLXVIII        1268
#define MCCLXIX          1269
#define MCCLXX           1270
#define MCCLXXI          1271
#define MCCLXXII         1272
#define MCCLXXIII        1273
#define MCCLXXIV         1274
#define MCCLXXV          1275
#define MCCLXXVI         1276
#define MCCLXXVII        1277
#define MCCLXXVIII       1278
#define MCCLXXIX         1279
#define MCCLXXX          1280
#define MCCLXXXI         1281
#define MCCLXXXII        1282
#define MCCLXXXIII       1283
#define MCCLXXXIV        1284
#define MCCLXXXV         1285
#define MCCLXXXVI        1286
#define MCCLXXXVII       1287
#define MCCLXXXVIII      1288
#define MCCLXXXIX        1289
#define MCCXC            1290
#define MCCXCI           1291
#define MCCXCII          1292
#define MCCXCIII         1293
#define MCCXCIV          1294
#define MCCXCV           1295
#define MCCXCVI          1296
#define MCCXCVII         1297
#define MCCXCVIII        1298
#define MCCXCIX          1299
#define MCCC             1300
#define MCCCI            1301
#define MCCCII           1302
#define MCCCIII          1303
#define MCCCIV           1304
#define MCCCV            1305
#define MCCCVI           1306
#define MCCCVII          1307
#define MCCCVIII         1308
#define MCCCIX           1309
#define MCCCX            1310
#define MCCCXI           1311
#define MCCCXII          1312
#define MCCCXIII         1313
#define MCCCXIV          1314
#define MCCCXV           1315
#define MCCCXVI          1316
#define MCCCXVII         1317
#define MCCCXVIII        1318
#define MCCCXIX          1319
#define MCCCXX           1320
#define MCCCXXI          1321
#define MCCCXXII         1322
#define MCCCXXIII        1323
#define MCCCXXIV         1324
#define MCCCXXV          1325
#define MCCCXXVI         1326
#define MCCCXXVII        1327
#define MCCCXXVIII       1328
#define MCCCXXIX         1329
#define MCCCXXX          1330
#define MCCCXXXI         1331
#define MCCCXXXII        1332
#define MCCCXXXIII       1333
#define MCCCXXXIV        1334
#define MCCCXXXV         1335
#define MCCCXXXVI        1336
#define MCCCXXXVII       1337
#define MCCCXXXVIII      1338
#define MCCCXXXIX        1339
#define MCCCXL           1340
#define MCCCXLI          1341
#define MCCCXLII         1342
#define MCCCXLIII        1343
#define MCCCXLIV         1344
#define MCCCXLV          1345
#define MCCCXLVI         1346
#define MCCCXLVII        1347
#define MCCCXLVIII       1348
#define MCCCXLIX         1349
#define MCCCL            1350
#define MCCCLI           1351
#define MCCCLII          1352
#define MCCCLIII         1353
#define MCCCLIV          1354
#define MCCCLV           1355
#define MCCCLVI          1356
#define MCCCLVII         1357
#define MCCCLVIII        1358
#define MCCCLIX          1359
#define MCCCLX           1360
#define MCCCLXI          1361
#define MCCCLXII         1362
#define MCCCLXIII        1363
#define MCCCLXIV         1364
#define MCCCLXV          1365
#define MCCCLXVI         1366
#define MCCCLXVII        1367
#define MCCCLXVIII       1368
#define MCCCLXIX         1369
#define MCCCLXX          1370
#define MCCCLXXI         1371
#define MCCCLXXII        1372
#define MCCCLXXIII       1373
#define MCCCLXXIV        1374
#define MCCCLXXV         1375
#define MCCCLXXVI        1376
#define MCCCLXXVII       1377
#define MCCCLXXVIII      1378
#define MCCCLXXIX        1379
#define MCCCLXXX         1380
#define MCCCLXXXI        1381
#define MCCCLXXXII       1382
#define MCCCLXXXIII      1383
#define MCCCLXXXIV       1384
#define MCCCLXXXV        1385
#define MCCCLXXXVI       1386
#define MCCCLXXXVII      1387
#define MCCCLXXXVIII     1388
#define MCCCLXXXIX       1389
#define MCCCXC           1390
#define MCCCXCI          1391
#define MCCCXCII         1392
#define MCCCXCIII        1393
#define MCCCXCIV         1394
#define MCCCXCV          1395
#define MCCCXCVI         1396
#define MCCCXCVII        1397
#define MCCCXCVIII       1398
#define MCCCXCIX         1399
#define MCD              1400
#define MCDI             1401
#define MCDII            1402
#define MCDIII           1403
#define MCDIV            1404
#define MCDV             1405
#define MCDVI            1406
#define MCDVII           1407
#define MCDVIII          1408
#define MCDIX            1409
#define MCDX             1410
#define MCDXI            1411
#define MCDXII           1412
#define MCDXIII          1413
#define MCDXIV           1414
#define MCDXV            1415
#define MCDXVI           1416
#define MCDXVII          1417
#define MCDXVIII         1418
#define MCDXIX           1419
#define MCDXX            1420
#define MCDXXI           1421
#define MCDXXII          1422
#define MCDXXIII         1423
#define MCDXXIV          1424
#define MCDXXV           1425
#define MCDXXVI          1426
#define MCDXXVII         1427
#define MCDXXVIII        1428
#define MCDXXIX          1429
#define MCDXXX           1430
#define MCDXXXI          1431
#define MCDXXXII         1432
#define MCDXXXIII        1433
#define MCDXXXIV         1434
#define MCDXXXV          1435
#define MCDXXXVI         1436
#define MCDXXXVII        1437
#define MCDXXXVIII       1438
#define MCDXXXIX         1439
#define MCDXL            1440
#define MCDXLI           1441
#define MCDXLII          1442
#define MCDXLIII         1443
#define MCDXLIV          1444
#define MCDXLV           1445
#define MCDXLVI          1446
#define MCDXLVII         1447
#define MCDXLVIII        1448
#define MCDXLIX          1449
#define MCDL             1450
#define MCDLI            1451
#define MCDLII           1452
#define MCDLIII          1453
#define MCDLIV           1454
#define MCDLV            1455
#define MCDLVI           1456
#define MCDLVII          1457
#define MCDLVIII         1458
#define MCDLIX           1459
#define MCDLX            1460
#define MCDLXI           1461
#define MCDLXII          1462
#define MCDLXIII         1463
#define MCDLXIV          1464
#define MCDLXV           1465
#define MCDLXVI          1466
#define MCDLXVII         1467
#define MCDLXVIII        1468
#define MCDLXIX          1469
#define MCDLXX           1470
#define MCDLXXI          1471
#define MCDLXXII         1472
#define MCDLXXIII        1473
#define MCDLXXIV         1474
#define MCDLXXV          1475
#define MCDLXXVI         1476
#define MCDLXXVII        1477
#define MCDLXXVIII       1478
#define MCDLXXIX         1479
#define MCDLXXX          1480
#define MCDLXXXI         1481
#define MCDLXXXII        1482
#define MCDLXXXIII       1483
#define MCDLXXXIV        1484
#define MCDLXXXV         1485
#define MCDLXXXVI        1486
#define MCDLXXXVII       1487
#define MCDLXXXVIII      1488
#define MCDLXXXIX        1489
#define MCDXC            1490
#define MCDXCI           1491
#define MCDXCII          1492
#define MCDXCIII         1493
#define MCDXCIV          1494
#define MCDXCV           1495
#define MCDXCVI          1496
#define MCDXCVII         1497
#define MCDXCVIII        1498
#define MCDXCIX          1499
#define MD               1500
#define MDI              1501
#define MDII             1502
#define MDIII            1503
#define MDIV             1504
#define MDV              1505
#define MDVI             1506
#define MDVII            1507
#define MDVIII           1508
#define MDIX             1509
#define MDX              1510
#define MDXI             1511
#define MDXII            1512
#define MDXIII           1513
#define MDXIV            1514
#define MDXV             1515
#define MDXVI            1516
#define MDXVII           1517
#define MDXVIII          1518
#define MDXIX            1519
#define MDXX             1520
#define MDXXI            1521
#define MDXXII           1522
#define MDXXIII          1523
#define MDXXIV           1524
#define MDXXV            1525
#define MDXXVI           1526
#define MDXXVII          1527
#define MDXXVIII         1528
#define MDXXIX           1529
#define MDXXX            1530
#define MDXXXI           1531
#define MDXXXII          1532
#define MDXXXIII         1533
#define MDXXXIV          1534
#define MDXXXV           1535
#define MDXXXVI          1536
#define MDXXXVII         1537
#define MDXXXVIII        1538
#define MDXXXIX          1539
#define MDXL             1540
#define MDXLI            1541
#define MDXLII           1542
#define MDXLIII          1543
#define MDXLIV           1544
#define MDXLV            1545
#define MDXLVI           1546
#define MDXLVII          1547
#define MDXLVIII         1548
#define MDXLIX           1549
#define MDL              1550
#define MDLI             1551
#define MDLII            1552
#define MDLIII           1553
#define MDLIV            1554
#define MDLV             1555
#define MDLVI            1556
#define MDLVII           1557
#define MDLVIII          1558
#define MDLIX            1559
#define MDLX             1560
#define MDLXI            1561
#define MDLXII           1562
#define MDLXIII          1563
#define MDLXIV           1564
#define MDLXV            1565
#define MDLXVI           1566
#define MDLXVII          1567
#define MDLXVIII         1568
#define MDLXIX           1569
#define MDLXX            1570
#define MDLXXI           1571
#define MDLXXII          1572
#define MDLXXIII         1573
#define MDLXXIV          1574
#define MDLXXV           1575
#define MDLXXVI          1576
#define MDLXXVII         1577
#define MDLXXVIII        1578
#define MDLXXIX          1579
#define MDLXXX           1580
#define MDLXXXI          1581
#define MDLXXXII         1582
#define MDLXXXIII        1583
#define MDLXXXIV         1584
#define MDLXXXV          1585
#define MDLXXXVI         1586
#define MDLXXXVII        1587
#define MDLXXXVIII       1588
#define MDLXXXIX         1589
#define MDXC             1590
#define MDXCI            1591
#define MDXCII           1592
#define MDXCIII          1593
#define MDXCIV           1594
#define MDXCV            1595
#define MDXCVI           1596
#define MDXCVII          1597
#define MDXCVIII         1598
#define MDXCIX           1599
#define MDC              1600
#define MDCI             1601
#define MDCII            1602
#define MDCIII           1603
#define MDCIV            1604
#define MDCV             1605
#define MDCVI            1606
#define MDCVII           1607
#define MDCVIII          1608
#define MDCIX            1609
#define MDCX             1610
#define MDCXI            1611
#define MDCXII           1612
#define MDCXIII          1613
#define MDCXIV           1614
#define MDCXV            1615
#define MDCXVI           1616
#define MDCXVII          1617
#define MDCXVIII         1618
#define MDCXIX           1619
#define MDCXX            1620
#define MDCXXI           1621
#define MDCXXII          1622
#define MDCXXIII         1623
#define MDCXXIV          1624
#define MDCXXV           1625
#define MDCXXVI          1626
#define MDCXXVII         1627
#define MDCXXVIII        1628
#define MDCXXIX          1629
#define MDCXXX           1630
#define MDCXXXI          1631
#define MDCXXXII         1632
#define MDCXXXIII        1633
#define MDCXXXIV         1634
#define MDCXXXV          1635
#define MDCXXXVI         1636
#define MDCXXXVII        1637
#define MDCXXXVIII       1638
#define MDCXXXIX         1639
#define MDCXL            1640
#define MDCXLI           1641
#define MDCXLII          1642
#define MDCXLIII         1643
#define MDCXLIV          1644
#define MDCXLV           1645
#define MDCXLVI          1646
#define MDCXLVII         1647
#define MDCXLVIII        1648
#define MDCXLIX          1649
#define MDCL             1650
#define MDCLI            1651
#define MDCLII           1652
#define MDCLIII          1653
#define MDCLIV           1654
#define MDCLV            1655
#define MDCLVI           1656
#define MDCLVII          1657
#define MDCLVIII         1658
#define MDCLIX           1659
#define MDCLX            1660
#define MDCLXI           1661
#define MDCLXII          1662
#define MDCLXIII         1663
#define MDCLXIV          1664
#define MDCLXV           1665
#define MDCLXVI          1666
#define MDCLXVII         1667
#define MDCLXVIII        1668
#define MDCLXIX          1669
#define MDCLXX           1670
#define MDCLXXI          1671
#define MDCLXXII         1672
#define MDCLXXIII        1673
#define MDCLXXIV         1674
#define MDCLXXV          1675
#define MDCLXXVI         1676
#define MDCLXXVII        1677
#define MDCLXXVIII       1678
#define MDCLXXIX         1679
#define MDCLXXX          1680
#define MDCLXXXI         1681
#define MDCLXXXII        1682
#define MDCLXXXIII       1683
#define MDCLXXXIV        1684
#define MDCLXXXV         1685
#define MDCLXXXVI        1686
#define MDCLXXXVII       1687
#define MDCLXXXVIII      1688
#define MDCLXXXIX        1689
#define MDCXC            1690
#define MDCXCI           1691
#define MDCXCII          1692
#define MDCXCIII         1693
#define MDCXCIV          1694
#define MDCXCV           1695
#define MDCXCVI          1696
#define MDCXCVII         1697
#define MDCXCVIII        1698
#define MDCXCIX          1699
#define MDCC             1700
#define MDCCI            1701
#define MDCCII           1702
#define MDCCIII          1703
#define MDCCIV           1704
#define MDCCV            1705
#define MDCCVI           1706
#define MDCCVII          1707
#define MDCCVIII         1708
#define MDCCIX           1709
#define MDCCX            1710
#define MDCCXI           1711
#define MDCCXII          1712
#define MDCCXIII         1713
#define MDCCXIV          1714
#define MDCCXV           1715
#define MDCCXVI          1716
#define MDCCXVII         1717
#define MDCCXVIII        1718
#define MDCCXIX          1719
#define MDCCXX           1720
#define MDCCXXI          1721
#define MDCCXXII         1722
#define MDCCXXIII        1723
#define MDCCXXIV         1724
#define MDCCXXV          1725
#define MDCCXXVI         1726
#define MDCCXXVII        1727
#define MDCCXXVIII       1728
#define MDCCXXIX         1729
#define MDCCXXX          1730
#define MDCCXXXI         1731
#define MDCCXXXII        1732
#define MDCCXXXIII       1733
#define MDCCXXXIV        1734
#define MDCCXXXV         1735
#define MDCCXXXVI        1736
#define MDCCXXXVII       1737
#define MDCCXXXVIII      1738
#define MDCCXXXIX        1739
#define MDCCXL           1740
#define MDCCXLI          1741
#define MDCCXLII         1742
#define MDCCXLIII        1743
#define MDCCXLIV         1744
#define MDCCXLV          1745
#define MDCCXLVI         1746
#define MDCCXLVII        1747
#define MDCCXLVIII       1748
#define MDCCXLIX         1749
#define MDCCL            1750
#define MDCCLI           1751
#define MDCCLII          1752
#define MDCCLIII         1753
#define MDCCLIV          1754
#define MDCCLV           1755
#define MDCCLVI          1756
#define MDCCLVII         1757
#define MDCCLVIII        1758
#define MDCCLIX          1759
#define MDCCLX           1760
#define MDCCLXI          1761
#define MDCCLXII         1762
#define MDCCLXIII        1763
#define MDCCLXIV         1764
#define MDCCLXV          1765
#define MDCCLXVI         1766
#define MDCCLXVII        1767
#define MDCCLXVIII       1768
#define MDCCLXIX         1769
#define MDCCLXX          1770
#define MDCCLXXI         1771
#define MDCCLXXII        1772
#define MDCCLXXIII       1773
#define MDCCLXXIV        1774
#define MDCCLXXV         1775
#define MDCCLXXVI        1776
#define MDCCLXXVII       1777
#define MDCCLXXVIII      1778
#define MDCCLXXIX        1779
#define MDCCLXXX         1780
#define MDCCLXXXI        1781
#define MDCCLXXXII       1782
#define MDCCLXXXIII      1783
#define MDCCLXXXIV       1784
#define MDCCLXXXV        1785
#define MDCCLXXXVI       1786
#define MDCCLXXXVII      1787
#define MDCCLXXXVIII     1788
#define MDCCLXXXIX       1789
#define MDCCXC           1790
#define MDCCXCI          1791
#define MDCCXCII         1792
#define MDCCXCIII        1793
#define MDCCXCIV         1794
#define MDCCXCV          1795
#define MDCCXCVI         1796
#define MDCCXCVII        1797
#define MDCCXCVIII       1798
#define MDCCXCIX         1799
#define MDCCC            1800
#define MDCCCI           1801
#define MDCCCII          1802
#define MDCCCIII         1803
#define MDCCCIV          1804
#define MDCCCV           1805
#define MDCCCVI          1806
#define MDCCCVII         1807
#define MDCCCVIII        1808
#define MDCCCIX          1809
#define MDCCCX           1810
#define MDCCCXI          1811
#define MDCCCXII         1812
#define MDCCCXIII        1813
#define MDCCCXIV         1814
#define MDCCCXV          1815
#define MDCCCXVI         1816
#define MDCCCXVII        1817
#define MDCCCXVIII       1818
#define MDCCCXIX         1819
#define MDCCCXX          1820
#define MDCCCXXI         1821
#define MDCCCXXII        1822
#define MDCCCXXIII       1823
#define MDCCCXXIV        1824
#define MDCCCXXV         1825
#define MDCCCXXVI        1826
#define MDCCCXXVII       1827
#define MDCCCXXVIII      1828
#define MDCCCXXIX        1829
#define MDCCCXXX         1830
#define MDCCCXXXI        1831
#define MDCCCXXXII       1832
#define MDCCCXXXIII      1833
#define MDCCCXXXIV       1834
#define MDCCCXXXV        1835
#define MDCCCXXXVI       1836
#define MDCCCXXXVII      1837
#define MDCCCXXXVIII     1838
#define MDCCCXXXIX       1839
#define MDCCCXL          1840
#define MDCCCXLI         1841
#define MDCCCXLII        1842
#define MDCCCXLIII       1843
#define MDCCCXLIV        1844
#define MDCCCXLV         1845
#define MDCCCXLVI        1846
#define MDCCCXLVII       1847
#define MDCCCXLVIII      1848
#define MDCCCXLIX        1849
#define MDCCCL           1850
#define MDCCCLI          1851
#define MDCCCLII         1852
#define MDCCCLIII        1853
#define MDCCCLIV         1854
#define MDCCCLV          1855
#define MDCCCLVI         1856
#define MDCCCLVII        1857
#define MDCCCLVIII       1858
#define MDCCCLIX         1859
#define MDCCCLX          1860
#define MDCCCLXI         1861
#define MDCCCLXII        1862
#define MDCCCLXIII       1863
#define MDCCCLXIV        1864
#define MDCCCLXV         1865
#define MDCCCLXVI        1866
#define MDCCCLXVII       1867
#define MDCCCLXVIII      1868
#define MDCCCLXIX        1869
#define MDCCCLXX         1870
#define MDCCCLXXI        1871
#define MDCCCLXXII       1872
#define MDCCCLXXIII      1873
#define MDCCCLXXIV       1874
#define MDCCCLXXV        1875
#define MDCCCLXXVI       1876
#define MDCCCLXXVII      1877
#define MDCCCLXXVIII     1878
#define MDCCCLXXIX       1879
#define MDCCCLXXX        1880
#define MDCCCLXXXI       1881
#define MDCCCLXXXII      1882
#define MDCCCLXXXIII     1883
#define MDCCCLXXXIV      1884
#define MDCCCLXXXV       1885
#define MDCCCLXXXVI      1886
#define MDCCCLXXXVII     1887
#define MDCCCLXXXVIII    1888
#define MDCCCLXXXIX      1889
#define MDCCCXC          1890
#define MDCCCXCI         1891
#define MDCCCXCII        1892
#define MDCCCXCIII       1893
#define MDCCCXCIV        1894
#define MDCCCXCV         1895
#define MDCCCXCVI        1896
#define MDCCCXCVII       1897
#define MDCCCXCVIII      1898
#define MDCCCXCIX        1899
#define MCM              1900
#define MCMI             1901
#define MCMII            1902
#define MCMIII           1903
#define MCMIV            1904
#define MCMV             1905
#define MCMVI            1906
#define MCMVII           1907
#define MCMVIII          1908
#define MCMIX            1909
#define MCMX             1910
#define MCMXI            1911
#define MCMXII           1912
#define MCMXIII          1913
#define MCMXIV           1914
#define MCMXV            1915
#define MCMXVI           1916
#define MCMXVII          1917
#define MCMXVIII         1918
#define MCMXIX           1919
#define MCMXX            1920
#define MCMXXI           1921
#define MCMXXII          1922
#define MCMXXIII         1923
#define MCMXXIV          1924
#define MCMXXV           1925
#define MCMXXVI          1926
#define MCMXXVII         1927
#define MCMXXVIII        1928
#define MCMXXIX          1929
#define MCMXXX           1930
#define MCMXXXI          1931
#define MCMXXXII         1932
#define MCMXXXIII        1933
#define MCMXXXIV         1934
#define MCMXXXV          1935
#define MCMXXXVI         1936
#define MCMXXXVII        1937
#define MCMXXXVIII       1938
#define MCMXXXIX         1939
#define MCMXL            1940
#define MCMXLI           1941
#define MCMXLII          1942
#define MCMXLIII         1943
#define MCMXLIV          1944
#define MCMXLV           1945
#define MCMXLVI          1946
#define MCMXLVII         1947
#define MCMXLVIII        1948
#define MCMXLIX          1949
#define MCML             1950
#define MCMLI            1951
#define MCMLII           1952
#define MCMLIII          1953
#define MCMLIV           1954
#define MCMLV            1955
#define MCMLVI           1956
#define MCMLVII          1957
#define MCMLVIII         1958
#define MCMLIX           1959
#define MCMLX            1960
#define MCMLXI           1961
#define MCMLXII          1962
#define MCMLXIII         1963
#define MCMLXIV          1964
#define MCMLXV           1965
#define MCMLXVI          1966
#define MCMLXVII         1967
#define MCMLXVIII        1968
#define MCMLXIX          1969
#define MCMLXX           1970
#define MCMLXXI          1971
#define MCMLXXII         1972
#define MCMLXXIII        1973
#define MCMLXXIV         1974
#define MCMLXXV          1975
#define MCMLXXVI         1976
#define MCMLXXVII        1977
#define MCMLXXVIII       1978
#define MCMLXXIX         1979
#define MCMLXXX          1980
#define MCMLXXXI         1981
#define MCMLXXXII        1982
#define MCMLXXXIII       1983
#define MCMLXXXIV        1984
#define MCMLXXXV         1985
#define MCMLXXXVI        1986
#define MCMLXXXVII       1987
#define MCMLXXXVIII      1988
#define MCMLXXXIX        1989
#define MCMXC            1990
#define MCMXCI           1991
#define MCMXCII          1992
#define MCMXCIII         1993
#define MCMXCIV          1994
#define MCMXCV           1995
#define MCMXCVI          1996
#define MCMXCVII         1997
#define MCMXCVIII        1998
#define MCMXCIX          1999
#define MM               2000
#define MMI              2001
#define MMII             2002
#define MMIII            2003
#define MMIV             2004
#define MMV              2005
#define MMVI             2006
#define MMVII            2007
#define MMVIII           2008
#define MMIX             2009
#define MMX              2010
#define MMXI             2011
#define MMXII            2012
#define MMXIII           2013
#define MMXIV            2014
#define MMXV             2015
#define MMXVI            2016
#define MMXVII           2017
#define MMXVIII          2018
#define MMXIX            2019
#define MMXX             2020
#define MMXXI            2021
#define MMXXII           2022
#define MMXXIII          2023
#define MMXXIV           2024
#define MMXXV            2025
#define MMXXVI           2026
#define MMXXVII          2027
#define MMXXVIII         2028
#define MMXXIX           2029
#define MMXXX            2030
#define MMXXXI           2031
#define MMXXXII          2032
#define MMXXXIII         2033
#define MMXXXIV          2034
#define MMXXXV           2035
#define MMXXXVI          2036
#define MMXXXVII         2037
#define MMXXXVIII        2038
#define MMXXXIX          2039
#define MMXL             2040
#define MMXLI            2041
#define MMXLII           2042
#define MMXLIII          2043
#define MMXLIV           2044
#define MMXLV            2045
#define MMXLVI           2046
#define MMXLVII          2047
#define MMXLVIII         2048
#define MMXLIX           2049
#define MML              2050
#define MMLI             2051
#define MMLII            2052
#define MMLIII           2053
#define MMLIV            2054
#define MMLV             2055
#define MMLVI            2056
#define MMLVII           2057
#define MMLVIII          2058
#define MMLIX            2059
#define MMLX             2060
#define MMLXI            2061
#define MMLXII           2062
#define MMLXIII          2063
#define MMLXIV           2064
#define MMLXV            2065
#define MMLXVI           2066
#define MMLXVII          2067
#define MMLXVIII         2068
#define MMLXIX           2069
#define MMLXX            2070
#define MMLXXI           2071
#define MMLXXII          2072
#define MMLXXIII         2073
#define MMLXXIV          2074
#define MMLXXV           2075
#define MMLXXVI          2076
#define MMLXXVII         2077
#define MMLXXVIII        2078
#define MMLXXIX          2079
#define MMLXXX           2080
#define MMLXXXI          2081
#define MMLXXXII         2082
#define MMLXXXIII        2083
#define MMLXXXIV         2084
#define MMLXXXV          2085
#define MMLXXXVI         2086
#define MMLXXXVII        2087
#define MMLXXXVIII       2088
#define MMLXXXIX         2089
#define MMXC             2090
#define MMXCI            2091
#define MMXCII           2092
#define MMXCIII          2093
#define MMXCIV           2094
#define MMXCV            2095
#define MMXCVI           2096
#define MMXCVII          2097
#define MMXCVIII         2098
#define MMXCIX           2099
#define MMC              2100
#define MMCI             2101
#define MMCII            2102
#define MMCIII           2103
#define MMCIV            2104
#define MMCV             2105
#define MMCVI            2106
#define MMCVII           2107
#define MMCVIII          2108
#define MMCIX            2109
#define MMCX             2110
#define MMCXI            2111
#define MMCXII           2112
#define MMCXIII          2113
#define MMCXIV           2114
#define MMCXV            2115
#define MMCXVI           2116
#define MMCXVII          2117
#define MMCXVIII         2118
#define MMCXIX           2119
#define MMCXX            2120
#define MMCXXI           2121
#define MMCXXII          2122
#define MMCXXIII         2123
#define MMCXXIV          2124
#define MMCXXV           2125
#define MMCXXVI          2126
#define MMCXXVII         2127
#define MMCXXVIII        2128
#define MMCXXIX          2129
#define MMCXXX           2130
#define MMCXXXI          2131
#define MMCXXXII         2132
#define MMCXXXIII        2133
#define MMCXXXIV         2134
#define MMCXXXV          2135
#define MMCXXXVI         2136
#define MMCXXXVII        2137
#define MMCXXXVIII       2138
#define MMCXXXIX         2139
#define MMCXL            2140
#define MMCXLI           2141
#define MMCXLII          2142
#define MMCXLIII         2143
#define MMCXLIV          2144
#define MMCXLV           2145
#define MMCXLVI          2146
#define MMCXLVII         2147
#define MMCXLVIII        2148
#define MMCXLIX          2149
#define MMCL             2150
#define MMCLI            2151
#define MMCLII           2152
#define MMCLIII          2153
#define MMCLIV           2154
#define MMCLV            2155
#define MMCLVI           2156
#define MMCLVII          2157
#define MMCLVIII         2158
#define MMCLIX           2159
#define MMCLX            2160
#define MMCLXI           2161
#define MMCLXII          2162
#define MMCLXIII         2163
#define MMCLXIV          2164
#define MMCLXV           2165
#define MMCLXVI          2166
#define MMCLXVII         2167
#define MMCLXVIII        2168
#define MMCLXIX          2169
#define MMCLXX           2170
#define MMCLXXI          2171
#define MMCLXXII         2172
#define MMCLXXIII        2173
#define MMCLXXIV         2174
#define MMCLXXV          2175
#define MMCLXXVI         2176
#define MMCLXXVII        2177
#define MMCLXXVIII       2178
#define MMCLXXIX         2179
#define MMCLXXX          2180
#define MMCLXXXI         2181
#define MMCLXXXII        2182
#define MMCLXXXIII       2183
#define MMCLXXXIV        2184
#define MMCLXXXV         2185
#define MMCLXXXVI        2186
#define MMCLXXXVII       2187
#define MMCLXXXVIII      2188
#define MMCLXXXIX        2189
#define MMCXC            2190
#define MMCXCI           2191
#define MMCXCII          2192
#define MMCXCIII         2193
#define MMCXCIV          2194
#define MMCXCV           2195
#define MMCXCVI          2196
#define MMCXCVII         2197
#define MMCXCVIII        2198
#define MMCXCIX          2199
#define MMCC             2200
#define MMCCI            2201
#define MMCCII           2202
#define MMCCIII          2203
#define MMCCIV           2204
#define MMCCV            2205
#define MMCCVI           2206
#define MMCCVII          2207
#define MMCCVIII         2208
#define MMCCIX           2209
#define MMCCX            2210
#define MMCCXI           2211
#define MMCCXII          2212
#define MMCCXIII         2213
#define MMCCXIV          2214
#define MMCCXV           2215
#define MMCCXVI          2216
#define MMCCXVII         2217
#define MMCCXVIII        2218
#define MMCCXIX          2219
#define MMCCXX           2220
#define MMCCXXI          2221
#define MMCCXXII         2222
#define MMCCXXIII        2223
#define MMCCXXIV         2224
#define MMCCXXV          2225
#define MMCCXXVI         2226
#define MMCCXXVII        2227
#define MMCCXXVIII       2228
#define MMCCXXIX         2229
#define MMCCXXX          2230
#define MMCCXXXI         2231
#define MMCCXXXII        2232
#define MMCCXXXIII       2233
#define MMCCXXXIV        2234
#define MMCCXXXV         2235
#define MMCCXXXVI        2236
#define MMCCXXXVII       2237
#define MMCCXXXVIII      2238
#define MMCCXXXIX        2239
#define MMCCXL           2240
#define MMCCXLI          2241
#define MMCCXLII         2242
#define MMCCXLIII        2243
#define MMCCXLIV         2244
#define MMCCXLV          2245
#define MMCCXLVI         2246
#define MMCCXLVII        2247
#define MMCCXLVIII       2248
#define MMCCXLIX         2249
#define MMCCL            2250
#define MMCCLI           2251
#define MMCCLII          2252
#define MMCCLIII         2253
#define MMCCLIV          2254
#define MMCCLV           2255
#define MMCCLVI          2256
#define MMCCLVII         2257
#define MMCCLVIII        2258
#define MMCCLIX          2259
#define MMCCLX           2260
#define MMCCLXI          2261
#define MMCCLXII         2262
#define MMCCLXIII        2263
#define MMCCLXIV         2264
#define MMCCLXV          2265
#define MMCCLXVI         2266
#define MMCCLXVII        2267
#define MMCCLXVIII       2268
#define MMCCLXIX         2269
#define MMCCLXX          2270
#define MMCCLXXI         2271
#define MMCCLXXII        2272
#define MMCCLXXIII       2273
#define MMCCLXXIV        2274
#define MMCCLXXV         2275
#define MMCCLXXVI        2276
#define MMCCLXXVII       2277
#define MMCCLXXVIII      2278
#define MMCCLXXIX        2279
#define MMCCLXXX         2280
#define MMCCLXXXI        2281
#define MMCCLXXXII       2282
#define MMCCLXXXIII      2283
#define MMCCLXXXIV       2284
#define MMCCLXXXV        2285
#define MMCCLXXXVI       2286
#define MMCCLXXXVII      2287
#define MMCCLXXXVIII     2288
#define MMCCLXXXIX       2289
#define MMCCXC           2290
#define MMCCXCI          2291
#define MMCCXCII         2292
#define MMCCXCIII        2293
#define MMCCXCIV         2294
#define MMCCXCV          2295
#define MMCCXCVI         2296
#define MMCCXCVII        2297
#define MMCCXCVIII       2298
#define MMCCXCIX         2299
#define MMCCC            2300
#define MMCCCI           2301
#define MMCCCII          2302
#define MMCCCIII         2303
#define MMCCCIV          2304
#define MMCCCV           2305
#define MMCCCVI          2306
#define MMCCCVII         2307
#define MMCCCVIII        2308
#define MMCCCIX          2309
#define MMCCCX           2310
#define MMCCCXI          2311
#define MMCCCXII         2312
#define MMCCCXIII        2313
#define MMCCCXIV         2314
#define MMCCCXV          2315
#define MMCCCXVI         2316
#define MMCCCXVII        2317
#define MMCCCXVIII       2318
#define MMCCCXIX         2319
#define MMCCCXX          2320
#define MMCCCXXI         2321
#define MMCCCXXII        2322
#define MMCCCXXIII       2323
#define MMCCCXXIV        2324
#define MMCCCXXV         2325
#define MMCCCXXVI        2326
#define MMCCCXXVII       2327
#define MMCCCXXVIII      2328
#define MMCCCXXIX        2329
#define MMCCCXXX         2330
#define MMCCCXXXI        2331
#define MMCCCXXXII       2332
#define MMCCCXXXIII      2333
#define MMCCCXXXIV       2334
#define MMCCCXXXV        2335
#define MMCCCXXXVI       2336
#define MMCCCXXXVII      2337
#define MMCCCXXXVIII     2338
#define MMCCCXXXIX       2339
#define MMCCCXL          2340
#define MMCCCXLI         2341
#define MMCCCXLII        2342
#define MMCCCXLIII       2343
#define MMCCCXLIV        2344
#define MMCCCXLV         2345
#define MMCCCXLVI        2346
#define MMCCCXLVII       2347
#define MMCCCXLVIII      2348
#define MMCCCXLIX        2349
#define MMCCCL           2350
#define MMCCCLI          2351
#define MMCCCLII         2352
#define MMCCCLIII        2353
#define MMCCCLIV         2354
#define MMCCCLV          2355
#define MMCCCLVI         2356
#define MMCCCLVII        2357
#define MMCCCLVIII       2358
#define MMCCCLIX         2359
#define MMCCCLX          2360
#define MMCCCLXI         2361
#define MMCCCLXII        2362
#define MMCCCLXIII       2363
#define MMCCCLXIV        2364
#define MMCCCLXV         2365
#define MMCCCLXVI        2366
#define MMCCCLXVII       2367
#define MMCCCLXVIII      2368
#define MMCCCLXIX        2369
#define MMCCCLXX         2370
#define MMCCCLXXI        2371
#define MMCCCLXXII       2372
#define MMCCCLXXIII      2373
#define MMCCCLXXIV       2374
#define MMCCCLXXV        2375
#define MMCCCLXXVI       2376
#define MMCCCLXXVII      2377
#define MMCCCLXXVIII     2378
#define MMCCCLXXIX       2379
#define MMCCCLXXX        2380
#define MMCCCLXXXI       2381
#define MMCCCLXXXII      2382
#define MMCCCLXXXIII     2383
#define MMCCCLXXXIV      2384
#define MMCCCLXXXV       2385
#define MMCCCLXXXVI      2386
#define MMCCCLXXXVII     2387
#define MMCCCLXXXVIII    2388
#define MMCCCLXXXIX      2389
#define MMCCCXC          2390
#define MMCCCXCI         2391
#define MMCCCXCII        2392
#define MMCCCXCIII       2393
#define MMCCCXCIV        2394
#define MMCCCXCV         2395
#define MMCCCXCVI        2396
#define MMCCCXCVII       2397
#define MMCCCXCVIII      2398
#define MMCCCXCIX        2399
#define MMCD             2400
#define MMCDI            2401
#define MMCDII           2402
#define MMCDIII          2403
#define MMCDIV           2404
#define MMCDV            2405
#define MMCDVI           2406
#define MMCDVII          2407
#define MMCDVIII         2408
#define MMCDIX           2409
#define MMCDX            2410
#define MMCDXI           2411
#define MMCDXII          2412
#define MMCDXIII         2413
#define MMCDXIV          2414
#define MMCDXV           2415
#define MMCDXVI          2416
#define MMCDXVII         2417
#define MMCDXVIII        2418
#define MMCDXIX          2419
#define MMCDXX           2420
#define MMCDXXI          2421
#define MMCDXXII         2422
#define MMCDXXIII        2423
#define MMCDXXIV         2424
#define MMCDXXV          2425
#define MMCDXXVI         2426
#define MMCDXXVII        2427
#define MMCDXXVIII       2428
#define MMCDXXIX         2429
#define MMCDXXX          2430
#define MMCDXXXI         2431
#define MMCDXXXII        2432
#define MMCDXXXIII       2433
#define MMCDXXXIV        2434
#define MMCDXXXV         2435
#define MMCDXXXVI        2436
#define MMCDXXXVII       2437
#define MMCDXXXVIII      2438
#define MMCDXXXIX        2439
#define MMCDXL           2440
#define MMCDXLI          2441
#define MMCDXLII         2442
#define MMCDXLIII        2443
#define MMCDXLIV         2444
#define MMCDXLV          2445
#define MMCDXLVI         2446
#define MMCDXLVII        2447
#define MMCDXLVIII       2448
#define MMCDXLIX         2449
#define MMCDL            2450
#define MMCDLI           2451
#define MMCDLII          2452
#define MMCDLIII         2453
#define MMCDLIV          2454
#define MMCDLV           2455
#define MMCDLVI          2456
#define MMCDLVII         2457
#define MMCDLVIII        2458
#define MMCDLIX          2459
#define MMCDLX           2460
#define MMCDLXI          2461
#define MMCDLXII         2462
#define MMCDLXIII        2463
#define MMCDLXIV         2464
#define MMCDLXV          2465
#define MMCDLXVI         2466
#define MMCDLXVII        2467
#define MMCDLXVIII       2468
#define MMCDLXIX         2469
#define MMCDLXX          2470
#define MMCDLXXI         2471
#define MMCDLXXII        2472
#define MMCDLXXIII       2473
#define MMCDLXXIV        2474
#define MMCDLXXV         2475
#define MMCDLXXVI        2476
#define MMCDLXXVII       2477
#define MMCDLXXVIII      2478
#define MMCDLXXIX        2479
#define MMCDLXXX         2480
#define MMCDLXXXI        2481
#define MMCDLXXXII       2482
#define MMCDLXXXIII      2483
#define MMCDLXXXIV       2484
#define MMCDLXXXV        2485
#define MMCDLXXXVI       2486
#define MMCDLXXXVII      2487
#define MMCDLXXXVIII     2488
#define MMCDLXXXIX       2489
#define MMCDXC           2490
#define MMCDXCI          2491
#define MMCDXCII         2492
#define MMCDXCIII        2493
#define MMCDXCIV         2494
#define MMCDXCV          2495
#define MMCDXCVI         2496
#define MMCDXCVII        2497
#define MMCDXCVIII       2498
#define MMCDXCIX         2499
#define MMD              2500
#define MMDI             2501
#define MMDII            2502
#define MMDIII           2503
#define MMDIV            2504
#define MMDV             2505
#define MMDVI            2506
#define MMDVII           2507
#define MMDVIII          2508
#define MMDIX            2509
#define MMDX             2510
#define MMDXI            2511
#define MMDXII           2512
#define MMDXIII          2513
#define MMDXIV           2514
#define MMDXV            2515
#define MMDXVI           2516
#define MMDXVII          2517
#define MMDXVIII         2518
#define MMDXIX           2519
#define MMDXX            2520
#define MMDXXI           2521
#define MMDXXII          2522
#define MMDXXIII         2523
#define MMDXXIV          2524
#define MMDXXV           2525
#define MMDXXVI          2526
#define MMDXXVII         2527
#define MMDXXVIII        2528
#define MMDXXIX          2529
#define MMDXXX           2530
#define MMDXXXI          2531
#define MMDXXXII         2532
#define MMDXXXIII        2533
#define MMDXXXIV         2534
#define MMDXXXV          2535
#define MMDXXXVI         2536
#define MMDXXXVII        2537
#define MMDXXXVIII       2538
#define MMDXXXIX         2539
#define MMDXL            2540
#define MMDXLI           2541
#define MMDXLII          2542
#define MMDXLIII         2543
#define MMDXLIV          2544
#define MMDXLV           2545
#define MMDXLVI          2546
#define MMDXLVII         2547
#define MMDXLVIII        2548
#define MMDXLIX          2549
#define MMDL             2550
#define MMDLI            2551
#define MMDLII           2552
#define MMDLIII          2553
#define MMDLIV           2554
#define MMDLV            2555
#define MMDLVI           2556
#define MMDLVII          2557
#define MMDLVIII         2558
#define MMDLIX           2559
#define MMDLX            2560
#define MMDLXI           2561
#define MMDLXII          2562
#define MMDLXIII         2563
#define MMDLXIV          2564
#define MMDLXV           2565
#define MMDLXVI          2566
#define MMDLXVII         2567
#define MMDLXVIII        2568
#define MMDLXIX          2569
#define MMDLXX           2570
#define MMDLXXI          2571
#define MMDLXXII         2572
#define MMDLXXIII        2573
#define MMDLXXIV         2574
#define MMDLXXV          2575
#define MMDLXXVI         2576
#define MMDLXXVII        2577
#define MMDLXXVIII       2578
#define MMDLXXIX         2579
#define MMDLXXX          2580
#define MMDLXXXI         2581
#define MMDLXXXII        2582
#define MMDLXXXIII       2583
#define MMDLXXXIV        2584
#define MMDLXXXV         2585
#define MMDLXXXVI        2586
#define MMDLXXXVII       2587
#define MMDLXXXVIII      2588
#define MMDLXXXIX        2589
#define MMDXC            2590
#define MMDXCI           2591
#define MMDXCII          2592
#define MMDXCIII         2593
#define MMDXCIV          2594
#define MMDXCV           2595
#define MMDXCVI          2596
#define MMDXCVII         2597
#define MMDXCVIII        2598
#define MMDXCIX          2599
#define MMDC             2600
#define MMDCI            2601
#define MMDCII           2602
#define MMDCIII          2603
#define MMDCIV           2604
#define MMDCV            2605
#define MMDCVI           2606
#define MMDCVII          2607
#define MMDCVIII         2608
#define MMDCIX           2609
#define MMDCX            2610
#define MMDCXI           2611
#define MMDCXII          2612
#define MMDCXIII         2613
#define MMDCXIV          2614
#define MMDCXV           2615
#define MMDCXVI          2616
#define MMDCXVII         2617
#define MMDCXVIII        2618
#define MMDCXIX          2619
#define MMDCXX           2620
#define MMDCXXI          2621
#define MMDCXXII         2622
#define MMDCXXIII        2623
#define MMDCXXIV         2624
#define MMDCXXV          2625
#define MMDCXXVI         2626
#define MMDCXXVII        2627
#define MMDCXXVIII       2628
#define MMDCXXIX         2629
#define MMDCXXX          2630
#define MMDCXXXI         2631
#define MMDCXXXII        2632
#define MMDCXXXIII       2633
#define MMDCXXXIV        2634
#define MMDCXXXV         2635
#define MMDCXXXVI        2636
#define MMDCXXXVII       2637
#define MMDCXXXVIII      2638
#define MMDCXXXIX        2639
#define MMDCXL           2640
#define MMDCXLI          2641
#define MMDCXLII         2642
#define MMDCXLIII        2643
#define MMDCXLIV         2644
#define MMDCXLV          2645
#define MMDCXLVI         2646
#define MMDCXLVII        2647
#define MMDCXLVIII       2648
#define MMDCXLIX         2649
#define MMDCL            2650
#define MMDCLI           2651
#define MMDCLII          2652
#define MMDCLIII         2653
#define MMDCLIV          2654
#define MMDCLV           2655
#define MMDCLVI          2656
#define MMDCLVII         2657
#define MMDCLVIII        2658
#define MMDCLIX          2659
#define MMDCLX           2660
#define MMDCLXI          2661
#define MMDCLXII         2662
#define MMDCLXIII        2663
#define MMDCLXIV         2664
#define MMDCLXV          2665
#define MMDCLXVI         2666
#define MMDCLXVII        2667
#define MMDCLXVIII       2668
#define MMDCLXIX         2669
#define MMDCLXX          2670
#define MMDCLXXI         2671
#define MMDCLXXII        2672
#define MMDCLXXIII       2673
#define MMDCLXXIV        2674
#define MMDCLXXV         2675
#define MMDCLXXVI        2676
#define MMDCLXXVII       2677
#define MMDCLXXVIII      2678
#define MMDCLXXIX        2679
#define MMDCLXXX         2680
#define MMDCLXXXI        2681
#define MMDCLXXXII       2682
#define MMDCLXXXIII      2683
#define MMDCLXXXIV       2684
#define MMDCLXXXV        2685
#define MMDCLXXXVI       2686
#define MMDCLXXXVII      2687
#define MMDCLXXXVIII     2688
#define MMDCLXXXIX       2689
#define MMDCXC           2690
#define MMDCXCI          2691
#define MMDCXCII         2692
#define MMDCXCIII        2693
#define MMDCXCIV         2694
#define MMDCXCV          2695
#define MMDCXCVI         2696
#define MMDCXCVII        2697
#define MMDCXCVIII       2698
#define MMDCXCIX         2699
#define MMDCC            2700
#define MMDCCI           2701
#define MMDCCII          2702
#define MMDCCIII         2703
#define MMDCCIV          2704
#define MMDCCV           2705
#define MMDCCVI          2706
#define MMDCCVII         2707
#define MMDCCVIII        2708
#define MMDCCIX          2709
#define MMDCCX           2710
#define MMDCCXI          2711
#define MMDCCXII         2712
#define MMDCCXIII        2713
#define MMDCCXIV         2714
#define MMDCCXV          2715
#define MMDCCXVI         2716
#define MMDCCXVII        2717
#define MMDCCXVIII       2718
#define MMDCCXIX         2719
#define MMDCCXX          2720
#define MMDCCXXI         2721
#define MMDCCXXII        2722
#define MMDCCXXIII       2723
#define MMDCCXXIV        2724
#define MMDCCXXV         2725
#define MMDCCXXVI        2726
#define MMDCCXXVII       2727
#define MMDCCXXVIII      2728
#define MMDCCXXIX        2729
#define MMDCCXXX         2730
#define MMDCCXXXI        2731
#define MMDCCXXXII       2732
#define MMDCCXXXIII      2733
#define MMDCCXXXIV       2734
#define MMDCCXXXV        2735
#define MMDCCXXXVI       2736
#define MMDCCXXXVII      2737
#define MMDCCXXXVIII     2738
#define MMDCCXXXIX       2739
#define MMDCCXL          2740
#define MMDCCXLI         2741
#define MMDCCXLII        2742
#define MMDCCXLIII       2743
#define MMDCCXLIV        2744
#define MMDCCXLV         2745
#define MMDCCXLVI        2746
#define MMDCCXLVII       2747
#define MMDCCXLVIII      2748
#define MMDCCXLIX        2749
#define MMDCCL           2750
#define MMDCCLI          2751
#define MMDCCLII         2752
#define MMDCCLIII        2753
#define MMDCCLIV         2754
#define MMDCCLV          2755
#define MMDCCLVI         2756
#define MMDCCLVII        2757
#define MMDCCLVIII       2758
#define MMDCCLIX         2759
#define MMDCCLX          2760
#define MMDCCLXI         2761
#define MMDCCLXII        2762
#define MMDCCLXIII       2763
#define MMDCCLXIV        2764
#define MMDCCLXV         2765
#define MMDCCLXVI        2766
#define MMDCCLXVII       2767
#define MMDCCLXVIII      2768
#define MMDCCLXIX        2769
#define MMDCCLXX         2770
#define MMDCCLXXI        2771
#define MMDCCLXXII       2772
#define MMDCCLXXIII      2773
#define MMDCCLXXIV       2774
#define MMDCCLXXV        2775
#define MMDCCLXXVI       2776
#define MMDCCLXXVII      2777
#define MMDCCLXXVIII     2778
#define MMDCCLXXIX       2779
#define MMDCCLXXX        2780
#define MMDCCLXXXI       2781
#define MMDCCLXXXII      2782
#define MMDCCLXXXIII     2783
#define MMDCCLXXXIV      2784
#define MMDCCLXXXV       2785
#define MMDCCLXXXVI      2786
#define MMDCCLXXXVII     2787
#define MMDCCLXXXVIII    2788
#define MMDCCLXXXIX      2789
#define MMDCCXC          2790
#define MMDCCXCI         2791
#define MMDCCXCII        2792
#define MMDCCXCIII       2793
#define MMDCCXCIV        2794
#define MMDCCXCV         2795
#define MMDCCXCVI        2796
#define MMDCCXCVII       2797
#define MMDCCXCVIII      2798
#define MMDCCXCIX        2799
#define MMDCCC           2800
#define MMDCCCI          2801
#define MMDCCCII         2802
#define MMDCCCIII        2803
#define MMDCCCIV         2804
#define MMDCCCV          2805
#define MMDCCCVI         2806
#define MMDCCCVII        2807
#define MMDCCCVIII       2808
#define MMDCCCIX         2809
#define MMDCCCX          2810
#define MMDCCCXI         2811
#define MMDCCCXII        2812
#define MMDCCCXIII       2813
#define MMDCCCXIV        2814
#define MMDCCCXV         2815
#define MMDCCCXVI        2816
#define MMDCCCXVII       2817
#define MMDCCCXVIII      2818
#define MMDCCCXIX        2819
#define MMDCCCXX         2820
#define MMDCCCXXI        2821
#define MMDCCCXXII       2822
#define MMDCCCXXIII      2823
#define MMDCCCXXIV       2824
#define MMDCCCXXV        2825
#define MMDCCCXXVI       2826
#define MMDCCCXXVII      2827
#define MMDCCCXXVIII     2828
#define MMDCCCXXIX       2829
#define MMDCCCXXX        2830
#define MMDCCCXXXI       2831
#define MMDCCCXXXII      2832
#define MMDCCCXXXIII     2833
#define MMDCCCXXXIV      2834
#define MMDCCCXXXV       2835
#define MMDCCCXXXVI      2836
#define MMDCCCXXXVII     2837
#define MMDCCCXXXVIII    2838
#define MMDCCCXXXIX      2839
#define MMDCCCXL         2840
#define MMDCCCXLI        2841
#define MMDCCCXLII       2842
#define MMDCCCXLIII      2843
#define MMDCCCXLIV       2844
#define MMDCCCXLV        2845
#define MMDCCCXLVI       2846
#define MMDCCCXLVII      2847
#define MMDCCCXLVIII     2848
#define MMDCCCXLIX       2849
#define MMDCCCL          2850
#define MMDCCCLI         2851
#define MMDCCCLII        2852
#define MMDCCCLIII       2853
#define MMDCCCLIV        2854
#define MMDCCCLV         2855
#define MMDCCCLVI        2856
#define MMDCCCLVII       2857
#define MMDCCCLVIII      2858
#define MMDCCCLIX        2859
#define MMDCCCLX         2860
#define MMDCCCLXI        2861
#define MMDCCCLXII       2862
#define MMDCCCLXIII      2863
#define MMDCCCLXIV       2864
#define MMDCCCLXV        2865
#define MMDCCCLXVI       2866
#define MMDCCCLXVII      2867
#define MMDCCCLXVIII     2868
#define MMDCCCLXIX       2869
#define MMDCCCLXX        2870
#define MMDCCCLXXI       2871
#define MMDCCCLXXII      2872
#define MMDCCCLXXIII     2873
#define MMDCCCLXXIV      2874
#define MMDCCCLXXV       2875
#define MMDCCCLXXVI      2876
#define MMDCCCLXXVII     2877
#define MMDCCCLXXVIII    2878
#define MMDCCCLXXIX      2879
#define MMDCCCLXXX       2880
#define MMDCCCLXXXI      2881
#define MMDCCCLXXXII     2882
#define MMDCCCLXXXIII    2883
#define MMDCCCLXXXIV     2884
#define MMDCCCLXXXV      2885
#define MMDCCCLXXXVI     2886
#define MMDCCCLXXXVII    2887
#define MMDCCCLXXXVIII   2888
#define MMDCCCLXXXIX     2889
#define MMDCCCXC         2890
#define MMDCCCXCI        2891
#define MMDCCCXCII       2892
#define MMDCCCXCIII      2893
#define MMDCCCXCIV       2894
#define MMDCCCXCV        2895
#define MMDCCCXCVI       2896
#define MMDCCCXCVII      2897
#define MMDCCCXCVIII     2898
#define MMDCCCXCIX       2899
#define MMCM             2900
#define MMCMI            2901
#define MMCMII           2902
#define MMCMIII          2903
#define MMCMIV           2904
#define MMCMV            2905
#define MMCMVI           2906
#define MMCMVII          2907
#define MMCMVIII         2908
#define MMCMIX           2909
#define MMCMX            2910
#define MMCMXI           2911
#define MMCMXII          2912
#define MMCMXIII         2913
#define MMCMXIV          2914
#define MMCMXV           2915
#define MMCMXVI          2916
#define MMCMXVII         2917
#define MMCMXVIII        2918
#define MMCMXIX          2919
#define MMCMXX           2920
#define MMCMXXI          2921
#define MMCMXXII         2922
#define MMCMXXIII        2923
#define MMCMXXIV         2924
#define MMCMXXV          2925
#define MMCMXXVI         2926
#define MMCMXXVII        2927
#define MMCMXXVIII       2928
#define MMCMXXIX         2929
#define MMCMXXX          2930
#define MMCMXXXI         2931
#define MMCMXXXII        2932
#define MMCMXXXIII       2933
#define MMCMXXXIV        2934
#define MMCMXXXV         2935
#define MMCMXXXVI        2936
#define MMCMXXXVII       2937
#define MMCMXXXVIII      2938
#define MMCMXXXIX        2939
#define MMCMXL           2940
#define MMCMXLI          2941
#define MMCMXLII         2942
#define MMCMXLIII        2943
#define MMCMXLIV         2944
#define MMCMXLV          2945
#define MMCMXLVI         2946
#define MMCMXLVII        2947
#define MMCMXLVIII       2948
#define MMCMXLIX         2949
#define MMCML            2950
#define MMCMLI           2951
#define MMCMLII          2952
#define MMCMLIII         2953
#define MMCMLIV          2954
#define MMCMLV           2955
#define MMCMLVI          2956
#define MMCMLVII         2957
#define MMCMLVIII        2958
#define MMCMLIX          2959
#define MMCMLX           2960
#define MMCMLXI          2961
#define MMCMLXII         2962
#define MMCMLXIII        2963
#define MMCMLXIV         2964
#define MMCMLXV          2965
#define MMCMLXVI         2966
#define MMCMLXVII        2967
#define MMCMLXVIII       2968
#define MMCMLXIX         2969
#define MMCMLXX          2970
#define MMCMLXXI         2971
#define MMCMLXXII        2972
#define MMCMLXXIII       2973
#define MMCMLXXIV        2974
#define MMCMLXXV         2975
#define MMCMLXXVI        2976
#define MMCMLXXVII       2977
#define MMCMLXXVIII      2978
#define MMCMLXXIX        2979
#define MMCMLXXX         2980
#define MMCMLXXXI        2981
#define MMCMLXXXII       2982
#define MMCMLXXXIII      2983
#define MMCMLXXXIV       2984
#define MMCMLXXXV        2985
#define MMCMLXXXVI       2986
#define MMCMLXXXVII      2987
#define MMCMLXXXVIII     2988
#define MMCMLXXXIX       2989
#define MMCMXC           2990
#define MMCMXCI          2991
#define MMCMXCII         2992
#define MMCMXCIII        2993
#define MMCMXCIV         2994
#define MMCMXCV          2995
#define MMCMXCVI         2996
#define MMCMXCVII        2997
#define MMCMXCVIII       2998
#define MMCMXCIX         2999
#define MMM              3000
#define MMMI             3001
#define MMMII            3002
#define MMMIII           3003
#define MMMIV            3004
#define MMMV             3005
#define MMMVI            3006
#define MMMVII           3007
#define MMMVIII          3008
#define MMMIX            3009
#define MMMX             3010
#define MMMXI            3011
#define MMMXII           3012
#define MMMXIII          3013
#define MMMXIV           3014
#define MMMXV            3015
#define MMMXVI           3016
#define MMMXVII          3017
#define MMMXVIII         3018
#define MMMXIX           3019
#define MMMXX            3020
#define MMMXXI           3021
#define MMMXXII          3022
#define MMMXXIII         3023
#define MMMXXIV          3024
#define MMMXXV           3025
#define MMMXXVI          3026
#define MMMXXVII         3027
#define MMMXXVIII        3028
#define MMMXXIX          3029
#define MMMXXX           3030
#define MMMXXXI          3031
#define MMMXXXII         3032
#define MMMXXXIII        3033
#define MMMXXXIV         3034
#define MMMXXXV          3035
#define MMMXXXVI         3036
#define MMMXXXVII        3037
#define MMMXXXVIII       3038
#define MMMXXXIX         3039
#define MMMXL            3040
#define MMMXLI           3041
#define MMMXLII          3042
#define MMMXLIII         3043
#define MMMXLIV          3044
#define MMMXLV           3045
#define MMMXLVI          3046
#define MMMXLVII         3047
#define MMMXLVIII        3048
#define MMMXLIX          3049
#define MMML             3050
#define MMMLI            3051
#define MMMLII           3052
#define MMMLIII          3053
#define MMMLIV           3054
#define MMMLV            3055
#define MMMLVI           3056
#define MMMLVII          3057
#define MMMLVIII         3058
#define MMMLIX           3059
#define MMMLX            3060
#define MMMLXI           3061
#define MMMLXII          3062
#define MMMLXIII         3063
#define MMMLXIV          3064
#define MMMLXV           3065
#define MMMLXVI          3066
#define MMMLXVII         3067
#define MMMLXVIII        3068
#define MMMLXIX          3069
#define MMMLXX           3070
#define MMMLXXI          3071
#define MMMLXXII         3072
#define MMMLXXIII        3073
#define MMMLXXIV         3074
#define MMMLXXV          3075
#define MMMLXXVI         3076
#define MMMLXXVII        3077
#define MMMLXXVIII       3078
#define MMMLXXIX         3079
#define MMMLXXX          3080
#define MMMLXXXI         3081
#define MMMLXXXII        3082
#define MMMLXXXIII       3083
#define MMMLXXXIV        3084
#define MMMLXXXV         3085
#define MMMLXXXVI        3086
#define MMMLXXXVII       3087
#define MMMLXXXVIII      3088
#define MMMLXXXIX        3089
#define MMMXC            3090
#define MMMXCI           3091
#define MMMXCII          3092
#define MMMXCIII         3093
#define MMMXCIV          3094
#define MMMXCV           3095
#define MMMXCVI          3096
#define MMMXCVII         3097
#define MMMXCVIII        3098
#define MMMXCIX          3099
#define MMMC             3100
#define MMMCI            3101
#define MMMCII           3102
#define MMMCIII          3103
#define MMMCIV           3104
#define MMMCV            3105
#define MMMCVI           3106
#define MMMCVII          3107
#define MMMCVIII         3108
#define MMMCIX           3109
#define MMMCX            3110
#define MMMCXI           3111
#define MMMCXII          3112
#define MMMCXIII         3113
#define MMMCXIV          3114
#define MMMCXV           3115
#define MMMCXVI          3116
#define MMMCXVII         3117
#define MMMCXVIII        3118
#define MMMCXIX          3119
#define MMMCXX           3120
#define MMMCXXI          3121
#define MMMCXXII         3122
#define MMMCXXIII        3123
#define MMMCXXIV         3124
#define MMMCXXV          3125
#define MMMCXXVI         3126
#define MMMCXXVII        3127
#define MMMCXXVIII       3128
#define MMMCXXIX         3129
#define MMMCXXX          3130
#define MMMCXXXI         3131
#define MMMCXXXII        3132
#define MMMCXXXIII       3133
#define MMMCXXXIV        3134
#define MMMCXXXV         3135
#define MMMCXXXVI        3136
#define MMMCXXXVII       3137
#define MMMCXXXVIII      3138
#define MMMCXXXIX        3139
#define MMMCXL           3140
#define MMMCXLI          3141
#define MMMCXLII         3142
#define MMMCXLIII        3143
#define MMMCXLIV         3144
#define MMMCXLV          3145
#define MMMCXLVI         3146
#define MMMCXLVII        3147
#define MMMCXLVIII       3148
#define MMMCXLIX         3149
#define MMMCL            3150
#define MMMCLI           3151
#define MMMCLII          3152
#define MMMCLIII         3153
#define MMMCLIV          3154
#define MMMCLV           3155
#define MMMCLVI          3156
#define MMMCLVII         3157
#define MMMCLVIII        3158
#define MMMCLIX          3159
#define MMMCLX           3160
#define MMMCLXI          3161
#define MMMCLXII         3162
#define MMMCLXIII        3163
#define MMMCLXIV         3164
#define MMMCLXV          3165
#define MMMCLXVI         3166
#define MMMCLXVII        3167
#define MMMCLXVIII       3168
#define MMMCLXIX         3169
#define MMMCLXX          3170
#define MMMCLXXI         3171
#define MMMCLXXII        3172
#define MMMCLXXIII       3173
#define MMMCLXXIV        3174
#define MMMCLXXV         3175
#define MMMCLXXVI        3176
#define MMMCLXXVII       3177
#define MMMCLXXVIII      3178
#define MMMCLXXIX        3179
#define MMMCLXXX         3180
#define MMMCLXXXI        3181
#define MMMCLXXXII       3182
#define MMMCLXXXIII      3183
#define MMMCLXXXIV       3184
#define MMMCLXXXV        3185
#define MMMCLXXXVI       3186
#define MMMCLXXXVII      3187
#define MMMCLXXXVIII     3188
#define MMMCLXXXIX       3189
#define MMMCXC           3190
#define MMMCXCI          3191
#define MMMCXCII         3192
#define MMMCXCIII        3193
#define MMMCXCIV         3194
#define MMMCXCV          3195
#define MMMCXCVI         3196
#define MMMCXCVII        3197
#define MMMCXCVIII       3198
#define MMMCXCIX         3199
#define MMMCC            3200
#define MMMCCI           3201
#define MMMCCII          3202
#define MMMCCIII         3203
#define MMMCCIV          3204
#define MMMCCV           3205
#define MMMCCVI          3206
#define MMMCCVII         3207
#define MMMCCVIII        3208
#define MMMCCIX          3209
#define MMMCCX           3210
#define MMMCCXI          3211
#define MMMCCXII         3212
#define MMMCCXIII        3213
#define MMMCCXIV         3214
#define MMMCCXV          3215
#define MMMCCXVI         3216
#define MMMCCXVII        3217
#define MMMCCXVIII       3218
#define MMMCCXIX         3219
#define MMMCCXX          3220
#define MMMCCXXI         3221
#define MMMCCXXII        3222
#define MMMCCXXIII       3223
#define MMMCCXXIV        3224
#define MMMCCXXV         3225
#define MMMCCXXVI        3226
#define MMMCCXXVII       3227
#define MMMCCXXVIII      3228
#define MMMCCXXIX        3229
#define MMMCCXXX         3230
#define MMMCCXXXI        3231
#define MMMCCXXXII       3232
#define MMMCCXXXIII      3233
#define MMMCCXXXIV       3234
#define MMMCCXXXV        3235
#define MMMCCXXXVI       3236
#define MMMCCXXXVII      3237
#define MMMCCXXXVIII     3238
#define MMMCCXXXIX       3239
#define MMMCCXL          3240
#define MMMCCXLI         3241
#define MMMCCXLII        3242
#define MMMCCXLIII       3243
#define MMMCCXLIV        3244
#define MMMCCXLV         3245
#define MMMCCXLVI        3246
#define MMMCCXLVII       3247
#define MMMCCXLVIII      3248
#define MMMCCXLIX        3249
#define MMMCCL           3250
#define MMMCCLI          3251
#define MMMCCLII         3252
#define MMMCCLIII        3253
#define MMMCCLIV         3254
#define MMMCCLV          3255
#define MMMCCLVI         3256
#define MMMCCLVII        3257
#define MMMCCLVIII       3258
#define MMMCCLIX         3259
#define MMMCCLX          3260
#define MMMCCLXI         3261
#define MMMCCLXII        3262
#define MMMCCLXIII       3263
#define MMMCCLXIV        3264
#define MMMCCLXV         3265
#define MMMCCLXVI        3266
#define MMMCCLXVII       3267
#define MMMCCLXVIII      3268
#define MMMCCLXIX        3269
#define MMMCCLXX         3270
#define MMMCCLXXI        3271
#define MMMCCLXXII       3272
#define MMMCCLXXIII      3273
#define MMMCCLXXIV       3274
#define MMMCCLXXV        3275
#define MMMCCLXXVI       3276
#define MMMCCLXXVII      3277
#define MMMCCLXXVIII     3278
#define MMMCCLXXIX       3279
#define MMMCCLXXX        3280
#define MMMCCLXXXI       3281
#define MMMCCLXXXII      3282
#define MMMCCLXXXIII     3283
#define MMMCCLXXXIV      3284
#define MMMCCLXXXV       3285
#define MMMCCLXXXVI      3286
#define MMMCCLXXXVII     3287
#define MMMCCLXXXVIII    3288
#define MMMCCLXXXIX      3289
#define MMMCCXC          3290
#define MMMCCXCI         3291
#define MMMCCXCII        3292
#define MMMCCXCIII       3293
#define MMMCCXCIV        3294
#define MMMCCXCV         3295
#define MMMCCXCVI        3296
#define MMMCCXCVII       3297
#define MMMCCXCVIII      3298
#define MMMCCXCIX        3299
#define MMMCCC           3300
#define MMMCCCI          3301
#define MMMCCCII         3302
#define MMMCCCIII        3303
#define MMMCCCIV         3304
#define MMMCCCV          3305
#define MMMCCCVI         3306
#define MMMCCCVII        3307
#define MMMCCCVIII       3308
#define MMMCCCIX         3309
#define MMMCCCX          3310
#define MMMCCCXI         3311
#define MMMCCCXII        3312
#define MMMCCCXIII       3313
#define MMMCCCXIV        3314
#define MMMCCCXV         3315
#define MMMCCCXVI        3316
#define MMMCCCXVII       3317
#define MMMCCCXVIII      3318
#define MMMCCCXIX        3319
#define MMMCCCXX         3320
#define MMMCCCXXI        3321
#define MMMCCCXXII       3322
#define MMMCCCXXIII      3323
#define MMMCCCXXIV       3324
#define MMMCCCXXV        3325
#define MMMCCCXXVI       3326
#define MMMCCCXXVII      3327
#define MMMCCCXXVIII     3328
#define MMMCCCXXIX       3329
#define MMMCCCXXX        3330
#define MMMCCCXXXI       3331
#define MMMCCCXXXII      3332
#define MMMCCCXXXIII     3333
#define MMMCCCXXXIV      3334
#define MMMCCCXXXV       3335
#define MMMCCCXXXVI      3336
#define MMMCCCXXXVII     3337
#define MMMCCCXXXVIII    3338
#define MMMCCCXXXIX      3339
#define MMMCCCXL         3340
#define MMMCCCXLI        3341
#define MMMCCCXLII       3342
#define MMMCCCXLIII      3343
#define MMMCCCXLIV       3344
#define MMMCCCXLV        3345
#define MMMCCCXLVI       3346
#define MMMCCCXLVII      3347
#define MMMCCCXLVIII     3348
#define MMMCCCXLIX       3349
#define MMMCCCL          3350
#define MMMCCCLI         3351
#define MMMCCCLII        3352
#define MMMCCCLIII       3353
#define MMMCCCLIV        3354
#define MMMCCCLV         3355
#define MMMCCCLVI        3356
#define MMMCCCLVII       3357
#define MMMCCCLVIII      3358
#define MMMCCCLIX        3359
#define MMMCCCLX         3360
#define MMMCCCLXI        3361
#define MMMCCCLXII       3362
#define MMMCCCLXIII      3363
#define MMMCCCLXIV       3364
#define MMMCCCLXV        3365
#define MMMCCCLXVI       3366
#define MMMCCCLXVII      3367
#define MMMCCCLXVIII     3368
#define MMMCCCLXIX       3369
#define MMMCCCLXX        3370
#define MMMCCCLXXI       3371
#define MMMCCCLXXII      3372
#define MMMCCCLXXIII     3373
#define MMMCCCLXXIV      3374
#define MMMCCCLXXV       3375
#define MMMCCCLXXVI      3376
#define MMMCCCLXXVII     3377
#define MMMCCCLXXVIII    3378
#define MMMCCCLXXIX      3379
#define MMMCCCLXXX       3380
#define MMMCCCLXXXI      3381
#define MMMCCCLXXXII     3382
#define MMMCCCLXXXIII    3383
#define MMMCCCLXXXIV     3384
#define MMMCCCLXXXV      3385
#define MMMCCCLXXXVI     3386
#define MMMCCCLXXXVII    3387
#define MMMCCCLXXXVIII   3388
#define MMMCCCLXXXIX     3389
#define MMMCCCXC         3390
#define MMMCCCXCI        3391
#define MMMCCCXCII       3392
#define MMMCCCXCIII      3393
#define MMMCCCXCIV       3394
#define MMMCCCXCV        3395
#define MMMCCCXCVI       3396
#define MMMCCCXCVII      3397
#define MMMCCCXCVIII     3398
#define MMMCCCXCIX       3399
#define MMMCD            3400
#define MMMCDI           3401
#define MMMCDII          3402
#define MMMCDIII         3403
#define MMMCDIV          3404
#define MMMCDV           3405
#define MMMCDVI          3406
#define MMMCDVII         3407
#define MMMCDVIII        3408
#define MMMCDIX          3409
#define MMMCDX           3410
#define MMMCDXI          3411
#define MMMCDXII         3412
#define MMMCDXIII        3413
#define MMMCDXIV         3414
#define MMMCDXV          3415
#define MMMCDXVI         3416
#define MMMCDXVII        3417
#define MMMCDXVIII       3418
#define MMMCDXIX         3419
#define MMMCDXX          3420
#define MMMCDXXI         3421
#define MMMCDXXII        3422
#define MMMCDXXIII       3423
#define MMMCDXXIV        3424
#define MMMCDXXV         3425
#define MMMCDXXVI        3426
#define MMMCDXXVII       3427
#define MMMCDXXVIII      3428
#define MMMCDXXIX        3429
#define MMMCDXXX         3430
#define MMMCDXXXI        3431
#define MMMCDXXXII       3432
#define MMMCDXXXIII      3433
#define MMMCDXXXIV       3434
#define MMMCDXXXV        3435
#define MMMCDXXXVI       3436
#define MMMCDXXXVII      3437
#define MMMCDXXXVIII     3438
#define MMMCDXXXIX       3439
#define MMMCDXL          3440
#define MMMCDXLI         3441
#define MMMCDXLII        3442
#define MMMCDXLIII       3443
#define MMMCDXLIV        3444
#define MMMCDXLV         3445
#define MMMCDXLVI        3446
#define MMMCDXLVII       3447
#define MMMCDXLVIII      3448
#define MMMCDXLIX        3449
#define MMMCDL           3450
#define MMMCDLI          3451
#define MMMCDLII         3452
#define MMMCDLIII        3453
#define MMMCDLIV         3454
#define MMMCDLV          3455
#define MMMCDLVI         3456
#define MMMCDLVII        3457
#define MMMCDLVIII       3458
#define MMMCDLIX         3459
#define MMMCDLX          3460
#define MMMCDLXI         3461
#define MMMCDLXII        3462
#define MMMCDLXIII       3463
#define MMMCDLXIV        3464
#define MMMCDLXV         3465
#define MMMCDLXVI        3466
#define MMMCDLXVII       3467
#define MMMCDLXVIII      3468
#define MMMCDLXIX        3469
#define MMMCDLXX         3470
#define MMMCDLXXI        3471
#define MMMCDLXXII       3472
#define MMMCDLXXIII      3473
#define MMMCDLXXIV       3474
#define MMMCDLXXV        3475
#define MMMCDLXXVI       3476
#define MMMCDLXXVII      3477
#define MMMCDLXXVIII     3478
#define MMMCDLXXIX       3479
#define MMMCDLXXX        3480
#define MMMCDLXXXI       3481
#define MMMCDLXXXII      3482
#define MMMCDLXXXIII     3483
#define MMMCDLXXXIV      3484
#define MMMCDLXXXV       3485
#define MMMCDLXXXVI      3486
#define MMMCDLXXXVII     3487
#define MMMCDLXXXVIII    3488
#define MMMCDLXXXIX      3489
#define MMMCDXC          3490
#define MMMCDXCI         3491
#define MMMCDXCII        3492
#define MMMCDXCIII       3493
#define MMMCDXCIV        3494
#define MMMCDXCV         3495
#define MMMCDXCVI        3496
#define MMMCDXCVII       3497
#define MMMCDXCVIII      3498
#define MMMCDXCIX        3499
#define MMMD             3500
#define MMMDI            3501
#define MMMDII           3502
#define MMMDIII          3503
#define MMMDIV           3504
#define MMMDV            3505
#define MMMDVI           3506
#define MMMDVII          3507
#define MMMDVIII         3508
#define MMMDIX           3509
#define MMMDX            3510
#define MMMDXI           3511
#define MMMDXII          3512
#define MMMDXIII         3513
#define MMMDXIV          3514
#define MMMDXV           3515
#define MMMDXVI          3516
#define MMMDXVII         3517
#define MMMDXVIII        3518
#define MMMDXIX          3519
#define MMMDXX           3520
#define MMMDXXI          3521
#define MMMDXXII         3522
#define MMMDXXIII        3523
#define MMMDXXIV         3524
#define MMMDXXV          3525
#define MMMDXXVI         3526
#define MMMDXXVII        3527
#define MMMDXXVIII       3528
#define MMMDXXIX         3529
#define MMMDXXX          3530
#define MMMDXXXI         3531
#define MMMDXXXII        3532
#define MMMDXXXIII       3533
#define MMMDXXXIV        3534
#define MMMDXXXV         3535
#define MMMDXXXVI        3536
#define MMMDXXXVII       3537
#define MMMDXXXVIII      3538
#define MMMDXXXIX        3539
#define MMMDXL           3540
#define MMMDXLI          3541
#define MMMDXLII         3542
#define MMMDXLIII        3543
#define MMMDXLIV         3544
#define MMMDXLV          3545
#define MMMDXLVI         3546
#define MMMDXLVII        3547
#define MMMDXLVIII       3548
#define MMMDXLIX         3549
#define MMMDL            3550
#define MMMDLI           3551
#define MMMDLII          3552
#define MMMDLIII         3553
#define MMMDLIV          3554
#define MMMDLV           3555
#define MMMDLVI          3556
#define MMMDLVII         3557
#define MMMDLVIII        3558
#define MMMDLIX          3559
#define MMMDLX           3560
#define MMMDLXI          3561
#define MMMDLXII         3562
#define MMMDLXIII        3563
#define MMMDLXIV         3564
#define MMMDLXV          3565
#define MMMDLXVI         3566
#define MMMDLXVII        3567
#define MMMDLXVIII       3568
#define MMMDLXIX         3569
#define MMMDLXX          3570
#define MMMDLXXI         3571
#define MMMDLXXII        3572
#define MMMDLXXIII       3573
#define MMMDLXXIV        3574
#define MMMDLXXV         3575
#define MMMDLXXVI        3576
#define MMMDLXXVII       3577
#define MMMDLXXVIII      3578
#define MMMDLXXIX        3579
#define MMMDLXXX         3580
#define MMMDLXXXI        3581
#define MMMDLXXXII       3582
#define MMMDLXXXIII      3583
#define MMMDLXXXIV       3584
#define MMMDLXXXV        3585
#define MMMDLXXXVI       3586
#define MMMDLXXXVII      3587
#define MMMDLXXXVIII     3588
#define MMMDLXXXIX       3589
#define MMMDXC           3590
#define MMMDXCI          3591
#define MMMDXCII         3592
#define MMMDXCIII        3593
#define MMMDXCIV         3594
#define MMMDXCV          3595
#define MMMDXCVI         3596
#define MMMDXCVII        3597
#define MMMDXCVIII       3598
#define MMMDXCIX         3599
#define MMMDC            3600
#define MMMDCI           3601
#define MMMDCII          3602
#define MMMDCIII         3603
#define MMMDCIV          3604
#define MMMDCV           3605
#define MMMDCVI          3606
#define MMMDCVII         3607
#define MMMDCVIII        3608
#define MMMDCIX          3609
#define MMMDCX           3610
#define MMMDCXI          3611
#define MMMDCXII         3612
#define MMMDCXIII        3613
#define MMMDCXIV         3614
#define MMMDCXV          3615
#define MMMDCXVI         3616
#define MMMDCXVII        3617
#define MMMDCXVIII       3618
#define MMMDCXIX         3619
#define MMMDCXX          3620
#define MMMDCXXI         3621
#define MMMDCXXII        3622
#define MMMDCXXIII       3623
#define MMMDCXXIV        3624
#define MMMDCXXV         3625
#define MMMDCXXVI        3626
#define MMMDCXXVII       3627
#define MMMDCXXVIII      3628
#define MMMDCXXIX        3629
#define MMMDCXXX         3630
#define MMMDCXXXI        3631
#define MMMDCXXXII       3632
#define MMMDCXXXIII      3633
#define MMMDCXXXIV       3634
#define MMMDCXXXV        3635
#define MMMDCXXXVI       3636
#define MMMDCXXXVII      3637
#define MMMDCXXXVIII     3638
#define MMMDCXXXIX       3639
#define MMMDCXL          3640
#define MMMDCXLI         3641
#define MMMDCXLII        3642
#define MMMDCXLIII       3643
#define MMMDCXLIV        3644
#define MMMDCXLV         3645
#define MMMDCXLVI        3646
#define MMMDCXLVII       3647
#define MMMDCXLVIII      3648
#define MMMDCXLIX        3649
#define MMMDCL           3650
#define MMMDCLI          3651
#define MMMDCLII         3652
#define MMMDCLIII        3653
#define MMMDCLIV         3654
#define MMMDCLV          3655
#define MMMDCLVI         3656
#define MMMDCLVII        3657
#define MMMDCLVIII       3658
#define MMMDCLIX         3659
#define MMMDCLX          3660
#define MMMDCLXI         3661
#define MMMDCLXII        3662
#define MMMDCLXIII       3663
#define MMMDCLXIV        3664
#define MMMDCLXV         3665
#define MMMDCLXVI        3666
#define MMMDCLXVII       3667
#define MMMDCLXVIII      3668
#define MMMDCLXIX        3669
#define MMMDCLXX         3670
#define MMMDCLXXI        3671
#define MMMDCLXXII       3672
#define MMMDCLXXIII      3673
#define MMMDCLXXIV       3674
#define MMMDCLXXV        3675
#define MMMDCLXXVI       3676
#define MMMDCLXXVII      3677
#define MMMDCLXXVIII     3678
#define MMMDCLXXIX       3679
#define MMMDCLXXX        3680
#define MMMDCLXXXI       3681
#define MMMDCLXXXII      3682
#define MMMDCLXXXIII     3683
#define MMMDCLXXXIV      3684
#define MMMDCLXXXV       3685
#define MMMDCLXXXVI      3686
#define MMMDCLXXXVII     3687
#define MMMDCLXXXVIII    3688
#define MMMDCLXXXIX      3689
#define MMMDCXC          3690
#define MMMDCXCI         3691
#define MMMDCXCII        3692
#define MMMDCXCIII       3693
#define MMMDCXCIV        3694
#define MMMDCXCV         3695
#define MMMDCXCVI        3696
#define MMMDCXCVII       3697
#define MMMDCXCVIII      3698
#define MMMDCXCIX        3699
#define MMMDCC           3700
#define MMMDCCI          3701
#define MMMDCCII         3702
#define MMMDCCIII        3703
#define MMMDCCIV         3704
#define MMMDCCV          3705
#define MMMDCCVI         3706
#define MMMDCCVII        3707
#define MMMDCCVIII       3708
#define MMMDCCIX         3709
#define MMMDCCX          3710
#define MMMDCCXI         3711
#define MMMDCCXII        3712
#define MMMDCCXIII       3713
#define MMMDCCXIV        3714
#define MMMDCCXV         3715
#define MMMDCCXVI        3716
#define MMMDCCXVII       3717
#define MMMDCCXVIII      3718
#define MMMDCCXIX        3719
#define MMMDCCXX         3720
#define MMMDCCXXI        3721
#define MMMDCCXXII       3722
#define MMMDCCXXIII      3723
#define MMMDCCXXIV       3724
#define MMMDCCXXV        3725
#define MMMDCCXXVI       3726
#define MMMDCCXXVII      3727
#define MMMDCCXXVIII     3728
#define MMMDCCXXIX       3729
#define MMMDCCXXX        3730
#define MMMDCCXXXI       3731
#define MMMDCCXXXII      3732
#define MMMDCCXXXIII     3733
#define MMMDCCXXXIV      3734
#define MMMDCCXXXV       3735
#define MMMDCCXXXVI      3736
#define MMMDCCXXXVII     3737
#define MMMDCCXXXVIII    3738
#define MMMDCCXXXIX      3739
#define MMMDCCXL         3740
#define MMMDCCXLI        3741
#define MMMDCCXLII       3742
#define MMMDCCXLIII      3743
#define MMMDCCXLIV       3744
#define MMMDCCXLV        3745
#define MMMDCCXLVI       3746
#define MMMDCCXLVII      3747
#define MMMDCCXLVIII     3748
#define MMMDCCXLIX       3749
#define MMMDCCL          3750
#define MMMDCCLI         3751
#define MMMDCCLII        3752
#define MMMDCCLIII       3753
#define MMMDCCLIV        3754
#define MMMDCCLV         3755
#define MMMDCCLVI        3756
#define MMMDCCLVII       3757
#define MMMDCCLVIII      3758
#define MMMDCCLIX        3759
#define MMMDCCLX         3760
#define MMMDCCLXI        3761
#define MMMDCCLXII       3762
#define MMMDCCLXIII      3763
#define MMMDCCLXIV       3764
#define MMMDCCLXV        3765
#define MMMDCCLXVI       3766
#define MMMDCCLXVII      3767
#define MMMDCCLXVIII     3768
#define MMMDCCLXIX       3769
#define MMMDCCLXX        3770
#define MMMDCCLXXI       3771
#define MMMDCCLXXII      3772
#define MMMDCCLXXIII     3773
#define MMMDCCLXXIV      3774
#define MMMDCCLXXV       3775
#define MMMDCCLXXVI      3776
#define MMMDCCLXXVII     3777
#define MMMDCCLXXVIII    3778
#define MMMDCCLXXIX      3779
#define MMMDCCLXXX       3780
#define MMMDCCLXXXI      3781
#define MMMDCCLXXXII     3782
#define MMMDCCLXXXIII    3783
#define MMMDCCLXXXIV     3784
#define MMMDCCLXXXV      3785
#define MMMDCCLXXXVI     3786
#define MMMDCCLXXXVII    3787
#define MMMDCCLXXXVIII   3788
#define MMMDCCLXXXIX     3789
#define MMMDCCXC         3790
#define MMMDCCXCI        3791
#define MMMDCCXCII       3792
#define MMMDCCXCIII      3793
#define MMMDCCXCIV       3794
#define MMMDCCXCV        3795
#define MMMDCCXCVI       3796
#define MMMDCCXCVII      3797
#define MMMDCCXCVIII     3798
#define MMMDCCXCIX       3799
#define MMMDCCC          3800
#define MMMDCCCI         3801
#define MMMDCCCII        3802
#define MMMDCCCIII       3803
#define MMMDCCCIV        3804
#define MMMDCCCV         3805
#define MMMDCCCVI        3806
#define MMMDCCCVII       3807
#define MMMDCCCVIII      3808
#define MMMDCCCIX        3809
#define MMMDCCCX         3810
#define MMMDCCCXI        3811
#define MMMDCCCXII       3812
#define MMMDCCCXIII      3813
#define MMMDCCCXIV       3814
#define MMMDCCCXV        3815
#define MMMDCCCXVI       3816
#define MMMDCCCXVII      3817
#define MMMDCCCXVIII     3818
#define MMMDCCCXIX       3819
#define MMMDCCCXX        3820
#define MMMDCCCXXI       3821
#define MMMDCCCXXII      3822
#define MMMDCCCXXIII     3823
#define MMMDCCCXXIV      3824
#define MMMDCCCXXV       3825
#define MMMDCCCXXVI      3826
#define MMMDCCCXXVII     3827
#define MMMDCCCXXVIII    3828
#define MMMDCCCXXIX      3829
#define MMMDCCCXXX       3830
#define MMMDCCCXXXI      3831
#define MMMDCCCXXXII     3832
#define MMMDCCCXXXIII    3833
#define MMMDCCCXXXIV     3834
#define MMMDCCCXXXV      3835
#define MMMDCCCXXXVI     3836
#define MMMDCCCXXXVII    3837
#define MMMDCCCXXXVIII   3838
#define MMMDCCCXXXIX     3839
#define MMMDCCCXL        3840
#define MMMDCCCXLI       3841
#define MMMDCCCXLII      3842
#define MMMDCCCXLIII     3843
#define MMMDCCCXLIV      3844
#define MMMDCCCXLV       3845
#define MMMDCCCXLVI      3846
#define MMMDCCCXLVII     3847
#define MMMDCCCXLVIII    3848
#define MMMDCCCXLIX      3849
#define MMMDCCCL         3850
#define MMMDCCCLI        3851
#define MMMDCCCLII       3852
#define MMMDCCCLIII      3853
#define MMMDCCCLIV       3854
#define MMMDCCCLV        3855
#define MMMDCCCLVI       3856
#define MMMDCCCLVII      3857
#define MMMDCCCLVIII     3858
#define MMMDCCCLIX       3859
#define MMMDCCCLX        3860
#define MMMDCCCLXI       3861
#define MMMDCCCLXII      3862
#define MMMDCCCLXIII     3863
#define MMMDCCCLXIV      3864
#define MMMDCCCLXV       3865
#define MMMDCCCLXVI      3866
#define MMMDCCCLXVII     3867
#define MMMDCCCLXVIII    3868
#define MMMDCCCLXIX      3869
#define MMMDCCCLXX       3870
#define MMMDCCCLXXI      3871
#define MMMDCCCLXXII     3872
#define MMMDCCCLXXIII    3873
#define MMMDCCCLXXIV     3874
#define MMMDCCCLXXV      3875
#define MMMDCCCLXXVI     3876
#define MMMDCCCLXXVII    3877
#define MMMDCCCLXXVIII   3878
#define MMMDCCCLXXIX     3879
#define MMMDCCCLXXX      3880
#define MMMDCCCLXXXI     3881
#define MMMDCCCLXXXII    3882
#define MMMDCCCLXXXIII   3883
#define MMMDCCCLXXXIV    3884
#define MMMDCCCLXXXV     3885
#define MMMDCCCLXXXVI    3886
#define MMMDCCCLXXXVII   3887
#define MMMDCCCLXXXVIII  3888
#define MMMDCCCLXXXIX    3889
#define MMMDCCCXC        3890
#define MMMDCCCXCI       3891
#define MMMDCCCXCII      3892
#define MMMDCCCXCIII     3893
#define MMMDCCCXCIV      3894
#define MMMDCCCXCV       3895
#define MMMDCCCXCVI      3896
#define MMMDCCCXCVII     3897
#define MMMDCCCXCVIII    3898
#define MMMDCCCXCIX      3899
#define MMMCM            3900
#define MMMCMI           3901
#define MMMCMII          3902
#define MMMCMIII         3903
#define MMMCMIV          3904
#define MMMCMV           3905
#define MMMCMVI          3906
#define MMMCMVII         3907
#define MMMCMVIII        3908
#define MMMCMIX          3909
#define MMMCMX           3910
#define MMMCMXI          3911
#define MMMCMXII         3912
#define MMMCMXIII        3913
#define MMMCMXIV         3914
#define MMMCMXV          3915
#define MMMCMXVI         3916
#define MMMCMXVII        3917
#define MMMCMXVIII       3918
#define MMMCMXIX         3919
#define MMMCMXX          3920
#define MMMCMXXI         3921
#define MMMCMXXII        3922
#define MMMCMXXIII       3923
#define MMMCMXXIV        3924
#define MMMCMXXV         3925
#define MMMCMXXVI        3926
#define MMMCMXXVII       3927
#define MMMCMXXVIII      3928
#define MMMCMXXIX        3929
#define MMMCMXXX         3930
#define MMMCMXXXI        3931
#define MMMCMXXXII       3932
#define MMMCMXXXIII      3933
#define MMMCMXXXIV       3934
#define MMMCMXXXV        3935
#define MMMCMXXXVI       3936
#define MMMCMXXXVII      3937
#define MMMCMXXXVIII     3938
#define MMMCMXXXIX       3939
#define MMMCMXL          3940
#define MMMCMXLI         3941
#define MMMCMXLII        3942
#define MMMCMXLIII       3943
#define MMMCMXLIV        3944
#define MMMCMXLV         3945
#define MMMCMXLVI        3946
#define MMMCMXLVII       3947
#define MMMCMXLVIII      3948
#define MMMCMXLIX        3949
#define MMMCML           3950
#define MMMCMLI          3951
#define MMMCMLII         3952
#define MMMCMLIII        3953
#define MMMCMLIV         3954
#define MMMCMLV          3955
#define MMMCMLVI         3956
#define MMMCMLVII        3957
#define MMMCMLVIII       3958
#define MMMCMLIX         3959
#define MMMCMLX          3960
#define MMMCMLXI         3961
#define MMMCMLXII        3962
#define MMMCMLXIII       3963
#define MMMCMLXIV        3964
#define MMMCMLXV         3965
#define MMMCMLXVI        3966
#define MMMCMLXVII       3967
#define MMMCMLXVIII      3968
#define MMMCMLXIX        3969
#define MMMCMLXX         3970
#define MMMCMLXXI        3971
#define MMMCMLXXII       3972
#define MMMCMLXXIII      3973
#define MMMCMLXXIV       3974
#define MMMCMLXXV        3975
#define MMMCMLXXVI       3976
#define MMMCMLXXVII      3977
#define MMMCMLXXVIII     3978
#define MMMCMLXXIX       3979
#define MMMCMLXXX        3980
#define MMMCMLXXXI       3981
#define MMMCMLXXXII      3982
#define MMMCMLXXXIII     3983
#define MMMCMLXXXIV      3984
#define MMMCMLXXXV       3985
#define MMMCMLXXXVI      3986
#define MMMCMLXXXVII     3987
#define MMMCMLXXXVIII    3988
#define MMMCMLXXXIX      3989
#define MMMCMXC          3990
#define MMMCMXCI         3991
#define MMMCMXCII        3992
#define MMMCMXCIII       3993
#define MMMCMXCIV        3994
#define MMMCMXCV         3995
#define MMMCMXCVI        3996
#define MMMCMXCVII       3997
#define MMMCMXCVIII      3998
#define MMMCMXCIX        3999
/* finis numerorum generatorum */

#define imprimere     printf
#define liberare         free
#define memoriae_allocare    malloc
#define exire                exit

#define interior         static
#define hic_manens     static
#define universalis static

#define FILUM FILE

nomen insignatus character    i8;
nomen insignatus brevis         i16;
nomen insignatus integer       i32;
nomen insignatus longus longus    i64;

nomen signatus character    s8;
nomen signatus brevis            s16;
nomen signatus integer         s32;
nomen signatus longus longus    s64;

nomen fluitans                  f32;
nomen duplex                         f64;

nomen integer                    b32;

nomen size_t                                 memoriae_index;

#endif /* LATINA_H */
#line 1 "include/piscina.h"
/* piscina.h - arena memoriae: liberatio tota semel (arena, pool) */
#ifndef PISCINA_H
#define PISCINA_H




/* ===============================================
 * Creatio
 * =============================================== */

nomen structura Piscina Piscina;

/* PiscinaNotatio - nota pro mark/reset pattern
 * Captat statum piscinam ut postea reficere possit
 */
nomen structura PiscinaNotatio {
            vacuum* alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index  positus;       /* Offset in alveo */
} PiscinaNotatio;

Piscina*
piscina_generare_dynamicum (
          constans character* piscinae_titulum,
              memoriae_index  mensura_alvei_initia);

Piscina*
piscina_generare_certae_magnitudinis (
        constans character* piscinae_titulum,
            memoriae_index  mensura_buffer);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
piscina_destruere (
        Piscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 *
 * piscina_allocare ordinat ad PISCINA_ORDINATIO_ORDINARIA (VIII):
 * satis pro omni typo domus (indices, i64/s64, f64), sicut malloc.
 * Octeti soli (textus) arte stipari possunt per
 * piscina_allocare_ordinatum(piscina, mensura, I). Ante 2026-10-05
 * ordinatio ordinaria erat I: membra latiora non ordinata - mores
 * indefiniti in C, quos sanitas 'alignment' capit.
 * =============================================== */

#define PISCINA_ORDINATIO_ORDINARIA VIII

vacuum*
piscina_allocare (
                         Piscina* piscina,
                  memoriae_index  mensura);

vacuum*
piscina_allocare_ordinatum (
                         Piscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);


/* ===============================================
 * Allocatio - redde NIHIL si defectu
 * =============================================== */

vacuum*
piscina_conari_allocare (
                         Piscina* piscina,
                  memoriae_index  mensura);

vacuum*
piscina_conari_allocare_ordinatum (
                         Piscina* piscina,
                  memoriae_index  mensura,
                  memoriae_index  ordinatio);


/* ===============================================
 * Ampliatio - allocationem augere sine copia (2026-10-21)
 *
 * piscina_conari_ampliare: allocationem 'datum' (mensura_vetus
 * octetorum EXACTE, ut allocata est) ad mensura_nova augere. Viae
 * duae:
 *   I.  allocatio ULTIMA alvei currentis et spatium suppetit: in
 *       loco augetur - redde idem 'datum'.
 *   II. allocatio SOLA alvei currentis (ab initio alvei usque ad
 *       finem usus), piscina dynamica: alveus ipse per realloc
 *       augetur - redde monstratorem fortasse MOTUM, contentis
 *       servatis.
 * Aliter NIHIL, allocatione intacta: vocans novam allocat et copiat,
 * ut antea. Numerus allocationum non crescit (allocatio eadem est).
 *
 * CUR: aedificator chordae duplicando crescit, et arena buffera
 * vetera numquam reddit - scriptura STML magna III.VI x textum
 * consumebat (materia/fontes/materia_arbor.worklog.md 2026-10-21).
 *
 * CAUTIO viae II: monstratores in 'datum' veterem post motum
 * PENDENT - memoria liberata est, non in arena servata. Solus
 * possessor allocationis eos tenere debet (aedificator: spectatio
 * valet solum usque ad mutationem proximam). Sub -DPISCINA_VENENUM=1
 * via II SEMPER movet et buffer vetus veneno implet ante
 * liberationem.
 * =============================================== */

vacuum*
piscina_conari_ampliare (
                         Piscina* piscina,
                          vacuum* datum,
                  memoriae_index  mensura_vetus,
                  memoriae_index  mensura_nova);


/* ===============================================
 * Cyclus Vitae
 * =============================================== */

vacuum
piscina_vacare (
        Piscina* piscina);


/* ===============================================
 * Quaestio
 * =============================================== */

memoriae_index
piscina_summa_usus (
        constans Piscina* piscina);

memoriae_index
piscina_summa_inutilis_allocatus (
        constans Piscina* piscina);

memoriae_index
piscina_reliqua_antequam_cresca_alvei (
        constans Piscina* piscina);

memoriae_index
piscina_summa_apex_usus (
        constans Piscina* piscina);

/* Numerus alveorum (alvei omnes, ab primo). Cum summa_usus +
 * summa_inutilis_allocatus imaginem memoriae dat: quot alvei,
 * quantum commissum (usus + inutilis), quantum otiosum. */
memoriae_index
piscina_numerus_alveorum (
        constans Piscina* piscina);

/* Numerus allocationum successarum ab ortu piscinae (reficere eum
 * non minuit - historia est, non status). Instrumentis: piscina
 * allocationibus, non octetis, ligata est (RP 2.4 - Xar per
 * lexema quattuor), ergo numerus ipse mensura celeritatis est. */
memoriae_index
piscina_numerus_allocationum (
        constans Piscina* piscina);


/* ===============================================
 * Notatio - mark/reset pattern
 * =============================================== */

/* piscina_notare - Captat statum currentem
 * "Notare positionem currentem pro refectione postea"
 *
 * Usus: PiscinaNotatio nota = piscina_notare(piscina);
 *       ... allocare temporaria ...
 *       piscina_reficere(piscina, nota);
 */
PiscinaNotatio
piscina_notare (
        Piscina* piscina);

/* piscina_reficere - Reficit statum ad notationem
 * "Reficere piscinam ad statum notatum"
 *
 * Omnia allocata post notationem erunt invalida! Memoria NON
 * liberatur nec deletur: lectio post refectionem valores veteres
 * saepe adhuc videt, ergo vitium tacitum est. Modus probandi:
 * -DPISCINA_VENENUM=1 octetos liberatos 0xA5 implet (porta
 * tools/venenum_probare.sh).
 */
vacuum
piscina_reficere (
               Piscina* piscina,
        PiscinaNotatio  notatio);

/* piscina_potesne_allocare - Verificat si allocatio possibilis
 * "Potesne allocare hanc mensuram sine crescentia?"
 *
 * Utile pro piscinis certae magnitudinis
 * Redde: VERUM si allocatio in alveo nunc capit
 */
b32
piscina_potesne_allocare (
        constans Piscina* piscina,
          memoriae_index  mensura);

#endif
#line 1 "include/sors.h"
/* sors.h - Numeri pseudo-fortuiti SEMINABILES et deterministici
 *
 * Idem semen = eadem series in omni machina, compilatore et versione:
 * integri soli (PCG32 pro rivo, SplitMix64 pro semine expandendo), ergo
 * replay (ictus aspergilli), artificia proceduralia et probationes
 * fuzzing reproducibiles sunt. Distincta a moneta (/dev/urandom pro
 * identitatibus - tuta sed NON seminabilis).
 *
 * Sors valor purus est (XVI octeti): sine piscina, sine statu globali;
 * copiatur et in ictu servatur libere.
 *
 * USUS:
 *   Sors s;
 *   sors_seminare(&s, semen, ZEPHYRUM);
 *   x = sors_intra(&s, C);            (* [0, C) sine inclinatione *)
 *   Sors ictus = sors_derivare(&s, numerus_ictus);   (* rivus filius *)
 */
/* <aedilis corpus="lib/sors.c"/> */
#ifndef SORS_H
#define SORS_H



nomen structura {
    i64 status;
    i64 incrementum;   /* impar semper - rivum (seriem) eligit */
} Sors;

/* semen: quilibet valor (SplitMix64 eum miscet); series: numerus rivi
 * independentis (0, 1, 2...) - eadem semen, series diversae = rivi
 * disiuncti */
vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series);

/* rivus filius ex parente et clave - PURUS in parente (parens non
 * mutatur): filius clavis k idem est quotcumque alii ante derivati
 * sunt. Pro seminibus per ictum, per artificium, per casum
 * probationis */
Sors
sors_derivare (
    constans Sors* s,
              i64  clavis);

/* XXXII bits */
i32
sors_proximum (
    Sors* s);

/* [0, limes) sine inclinatione (Lemire); limes 0 -> 0 */
i32
sors_intra (
    Sors* s,
     i32  limes);

/* [imum, summum] inclusive; imum > summum -> imum */
s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum);

/* [0, 1) ex LIII bits */
f64
sors_f64 (
    Sors* s);

/* [0, 1) ex XXIV bits */
f32
sors_f32 (
    Sors* s);

/* VERUM cum probabilitate numerator/denominator; denominator 0 ->
 * FALSUM */
b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator);

/* Fisher-Yates: numerus elementa magnitudinis 'magnitudo_elementi'
 * permutat (octeti permutantur - nulla allocatio) */
vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi);

/* LEGATUM: LCG classicus ANSI (status * 1103515245 + 12345) - statum
 * provehit et reddit. Ut probationes totalitatis bibliotheca utantur
 * sine casibus generatis mutatis */
i32
sors_lcg_classicus (
    i32* status);

#endif /* SORS_H */
#line 1 "include/chorda.h"
/* chorda.h - chorda: mensura + datum, SINE NUL (string slice) */
#ifndef CHORDA_H
#define CHORDA_H





/* ==================================================
 * Creatio
 * ================================================== */

nomen structura chorda {
    i32  mensura;
     i8* datum;
} chorda;


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
                  constans character* litterae,
                             Piscina* piscina);

chorda
chorda_ex_buffer (
     i8* buffer,
    i32  mensura);

chorda
chorda_sectio (
          chorda s,
             i32 initium,
             i32 finis);

chorda
chorda_transcribere (
         chorda  s,
        Piscina* piscina);

chorda
chorda_concatenare (
         chorda  a,
         chorda  b,
        Piscina* piscina);

chorda
chorda_praecidi_laterale (
         chorda  s,
        Piscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
    chorda* elementa;
       i32  numerus;
} chorda_fissio_fructus;

chorda_fissio_fructus
chorda_fissio (
           chorda  s,
        character  delim,
          Piscina* piscina);

/* chorda_fissio_chorda - Dividere chordam per delimitatorem chorda
 * Similis chorda_fissio sed delimitator est chorda, non character
 */
chorda_fissio_fructus
chorda_fissio_chorda (
         chorda  s,
         chorda  delim,
        Piscina* piscina);

/* chorda_iungere - Iungere array chordarum cum separatore
 * elementa: array chordarum
 * numerus: numerus elementorum in array
 * separator: chorda inter elementa (potest esse vacua)
 * Redde: Nova chorda iuncta (allocata ex piscina)
 */
chorda
chorda_iungere (
         chorda* elementa,
            i32  numerus,
         chorda  separator,
        Piscina* piscina);


/* ==================================================
 * Comparatio
 * ================================================= */

b32
chorda_aequalis (
        chorda a,
        chorda b);

b32
chorda_aequalis_literis (
                    chorda  s,
        constans character* cstr);

b32
chorda_aequalis_case_insensitivus (
        chorda a,
        chorda b);

s32
chorda_comparare (
        chorda a,
        chorda b);


/* ==================================================
 * Quaestio
 * ================================================= */

b32
chorda_continet (
        chorda fenum,
        chorda acus);

b32
chorda_incipit (
        chorda s,
        chorda prefixum);

b32
chorda_terminatur (
        chorda s,
        chorda suffixum);

/* chorda_invenire - Invenire primam occurrentiam acus in fenum
 * Redde: sectionem CONGRUENTEM solam (mensura = acus.mensura, datum
 *        intra fenum), NON reliquum fenum ut strstr; chorda vacua
 *        {0, NIHIL} si non inventus. Acus vacua -> fenum totum.
 *        Pro reliquo: chorda_invenire_index + chorda_sectio; pro
 *        inventum/non: chorda_continet.
 */
chorda
chorda_invenire (
        chorda fenum,
        chorda acus);

/* chorda_invenire_index - Invenire positionem acus in fenum
 * Redde: Index (0-based) primae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_index (
        chorda fenum,
        chorda acus);

/* chorda_invenire_ultimum - Invenire ultimam occurrentiam acus in fenum
 * Redde: sectionem congruentem ultimam (ut chorda_invenire, non
 *        reliquum), vel chorda vacua si non inventus
 */
chorda
chorda_invenire_ultimum (
        chorda fenum,
        chorda acus);

/* chorda_invenire_ultimum_index - Invenire positionem ultimae occurrentiae
 * Redde: Index (0-based) ultimae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_ultimum_index (
        chorda fenum,
        chorda acus);

i32
chorda_numerare_occurrentia (
        chorda fenum,
        chorda acus);


/* ==================================================
 * Manipulatio
 * ================================================= */

chorda
chorda_praecidere (
        chorda s);

chorda
chorda_minuscula (
         chorda  s,
        Piscina* piscina);

chorda
chorda_maiuscula (
         chorda  s,
        Piscina* piscina);

/* chorda_praecidere_sinistram - Praecidere spatium album a sinistra
 * Redde: Vista (non allocatio) ad chordam sine spatio album initiali
 */
chorda
chorda_praecidere_sinistram (
        chorda s);

/* chorda_praecidere_dextram - Praecidere spatium album a dextra
 * Redde: Vista (non allocatio) ad chordam sine spatio album finali
 */
chorda
chorda_praecidere_dextram (
        chorda s);

/* chorda_substituere - Substituere omnes occurrentias antiqui cum novo
 * Redde: Nova chorda cum substitutionibus (allocata ex piscina)
 */
chorda
chorda_substituere (
          chorda  s,
          chorda  antiquum,
          chorda  novum,
         Piscina* piscina);

/* chorda_invertere - Invertere ordinem characterum
 * Redde: Nova chorda inversa (allocata ex piscina)
 */
chorda
chorda_invertere (
         chorda  s,
        Piscina* piscina);

/* chorda_duplicare - Repetere chordam n vicibus
 * Redde: Nova chorda cum repetitionibus (allocata ex piscina)
 */
chorda
chorda_duplicare (
         chorda  s,
            i32  numerus,
        Piscina* piscina);


/* ==================================================
 * Convenientia
 * ================================================= */

character*
chorda_ut_cstr (
         chorda  s,
        Piscina* piscina);

/* chorda_ut_s32 - Convertere chordam ad integrum signatum (basis X)
 *
 * TEXTUS -> NUMERUS pro chorda (atoi/strtol in c.datum ULTRA
 * mensuram legunt: chorda NUL finale non fert). Tota chorda consumi
 * debet: signum optionale, cifrae, nihil post (spatia ducentia
 * tolerantur, ut strtol). RECUSAT: vacuam, NULLUM insertum, sordes
 * post numerum ("12x"), extra fines s32. Pro LXIV bitis:
 * chorda_ut_s64; praefixum numericum ("p003" -> III): chorda_sectio
 * prius.
 *
 * Redde: VERUM si conversa (fructus solum tunc positus)
 */
b32
chorda_ut_s32 (
        chorda  s,
           s32* fructus);

/* chorda_ut_i32 - Ut chorda_ut_s32, sed insignatum: signum '-'
 * recusat, fines 0..4294967295.
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_i32 (
          chorda  s,
             i32* fructus);

/* chorda_ut_f64 - Convertere chordam ad numerum fluitantem (strtod
 * super copiam terminatam): tota chorda consumi debet; recusat
 * vacuam, NULLUM insertum, sordes post numerum.
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_f64 (
        chorda  s,
           f64* fructus);

/* chorda_ex_s32 - Convertere integrum signatum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s32 (
            s32  numerus,
        Piscina* piscina);

/* chorda_ex_f64 - Convertere numerum puncta fluitantia ad chordam
 * praecisio: numerus locorum decimalium
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_f64 (
            f64  numerus,
            i32  praecisio,
        Piscina* piscina);

/* chorda_ut_s64 - Convertere chordam ad integrum LXIV bitorum
 *
 * Omnes cifrae, signum optionale, nihil aliud; zephyra ducentia sine
 * pondere. RECUSAT: NULLUM insertum (chorda terminatorem non fert,
 * ergo cstr eam truncaret), sordes post numerum, superfluitas (limes
 * LEXICE confertur - strtoll sub -std=c89 latet).
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_s64 (
         chorda  s,
            s64* fructus);

/* chorda_ex_s64 - Convertere integrum LXIV bitorum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s64 (
            s64  numerus,
        Piscina* piscina);

/* chorda_ex_f64_exacta - Forma quae ITER REDITUS fert ('%.17g')
 *
 * chorda_ex_f64 praecisionem decimalem FIXAM scribit, ergo 1e300 et
 * 1e-20 per eam pereunt. Haec forma legi potest et eundem duplicem
 * reddere.
 *
 * Redde: Nova chorda (allocata ex piscina), vacua si defectu
 */
chorda
chorda_ex_f64_exacta (
            f64  numerus,
        Piscina* piscina);

/* chorda_character_ad - Extrahere singularem characterem ut chordam
 * Redde: Chorda longitudinis 1, vel vacua si index extra limites
 */
chorda
chorda_character_ad (
         chorda  s,
            i32  index,
        Piscina* piscina);

/* chorda_vacua - Verificare si chorda vacua est
 * Redde: VERUM si datum == NIHIL vel mensura == 0
 */
b32
chorda_vacua (
        chorda s);


/* ==================================================
 * Conversio Casus
 * ================================================= */

/* chorda_pascalis - Convertere ad PascalCase
 * "hello world" → "HelloWorld"
 * "getElementById" → "GetElementById"
 */
chorda
chorda_pascalis (
         chorda  s,
        Piscina* piscina);

/* chorda_camelus - Convertere ad camelCase
 * "hello world" → "helloWorld"
 * "getElementById" → "getElementById"
 */
chorda
chorda_camelus (
         chorda  s,
        Piscina* piscina);

/* chorda_serpens - Convertere ad snake_case
 * "HelloWorld" → "hello_world"
 * "getElementById" → "get_element_by_id"
 */
chorda
chorda_serpens (
         chorda  s,
        Piscina* piscina);

/* chorda_kebab - Convertere ad kebab-case
 * "HelloWorld" → "hello-world"
 * "getElementById" → "get-element-by-id"
 */
chorda
chorda_kebab (
         chorda  s,
        Piscina* piscina);

/* chorda_pascalis_serpens - Convertere ad Pascal_Snake_Case
 * "hello world" → "Hello_World"
 * "getElementById" → "Get_Element_By_Id"
 */
chorda
chorda_pascalis_serpens (
         chorda  s,
        Piscina* piscina);


/* ==================================================
 * Formatatio
 * ================================================= */

/* chorda_ex_bytes_legibilis - Convertere bytes ad formam legibilem
 * Exempla:
 *   1024      → "1.0 KB"
 *   1536      → "1.5 KB"
 *   1048576   → "1.0 MB"
 *   1073741824 → "1.0 GB"
 */
chorda
chorda_ex_bytes_legibilis (
            i64  bytes,
        Piscina* piscina);


/* ==================================================
 * Friatio
 * ================================================= */

i32
chorda_friare (
        chorda s);

#endif /* CHORDA_H */
#line 1 "include/magnus.h"
/* magnus.h - Integri magni EXACTI (sine limite)
 *
 * Numerus integer quilibet, exacte: nulla exundatio tacita. Valor
 * immutabilis (sicut chorda): omnis operatio novum valorem reddit;
 * memoria ex piscina. Valores qui in s64 capiunt INTRA structuram
 * manent (via celeris, sine allocatione); maiores in membris
 * XXXII bitorum in piscina. Nulli fluitantes: idem effectus in omni
 * machina et omni gradu optimizationis.
 *
 * Divisio EUCLIDEA: 0 <= residuum < |divisor|, semper (C89 signum
 * residui negativi non definit; nos definimus).
 *
 * VITA: membra immutabilia sunt, ergo effectus membra argumentorum
 * PARTIRI potest (e.g. magnus_nega signum tantum vertit). Effectus
 * valet dum piscinae argumentorum et effectus vivunt.
 *
 * USUS:
 *   Magnus a = magnus_ex_s64(XII);
 *   Magnus b = magnus_potentia(a, C, piscina);      (* 12^100 *)
 *   chorda t = magnus_ad_chordam(b, piscina);
 *
 * Vide lib/magnus.worklog.md.
 */
/* <aedilis corpus="lib/magnus.c"/> */
#ifndef MAGNUS_H
#define MAGNUS_H





/* Membra PRIVATA - per functiones tantum legenda.
 * Invarians: si valor in s64 capit, membra == NIHIL et valor in
 * 'parvus'; aliter signum (+1/-1) et longitudo membrorum (>= II,
 * membrum summum non nullum). Ergo forma canonica et unica. */
nomen structura {
     s64  parvus;
     s32  signum;
     i32  longitudo;
     i32* membra;      /* ordine parvo primum, basis 2^32 */
} Magnus;


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Magnus
magnus_ex_s64 (
    s64 valor);

/* VERUM et *exitus si valor in s64 capit; aliter FALSUM */
b32
magnus_ad_s64 (
    Magnus  a,
       s64* exitus);

/* decimalis, signum '-' optionale, nulla spatia; FALSUM si textus
 * malformatus (exitus non tangitur) */
b32
magnus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
      Magnus* exitus);

/* decimalis */
chorda
magnus_ad_chordam (
      Magnus  a,
     Piscina* piscina);

/* copia in piscinam datam: membra NOVA (valores in s64 sine
 * allocatione). Ad effectum ex piscina temporaria servandum, cum
 * piscina illa reficienda aut destruenda est. */
Magnus
magnus_transcribe (
      Magnus  a,
     Piscina* piscina);


/* ==================================================
 * Inspectio
 * ================================================== */

/* -1, 0, +1 */
s32
magnus_signum (
    Magnus a);

/* -1, 0, +1 sicut a <, =, > b */
s32
magnus_compara (
    Magnus a,
    Magnus b);

b32
magnus_aequalis (
    Magnus a,
    Magnus b);


/* ==================================================
 * Arithmetica - piscina tantum si via celeris non sufficit
 * ================================================== */

Magnus
magnus_nega (
      Magnus  a,
     Piscina* piscina);

Magnus
magnus_absolutum (
      Magnus  a,
     Piscina* piscina);

Magnus
magnus_adde (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

Magnus
magnus_subtrahe (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

Magnus
magnus_multiplica (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

/* a = quotiens * divisor + residuum, 0 <= residuum < |divisor|.
 * FALSUM si divisor nullus (exitus non tanguntur). Uterque exitus
 * NIHIL esse potest. */
b32
magnus_divide (
      Magnus  a,
      Magnus  divisor,
     Piscina* piscina,
      Magnus* quotiens,
      Magnus* residuum);

/* basis^exponens; 0^0 = 1 */
Magnus
magnus_potentia (
      Magnus  basis,
         i32  exponens,
     Piscina* piscina);

/* numerus bitorum |a|: 0 pro 0, 1 pro +-1, k + 1 pro 2^k <= |a| <
 * 2^(k+1). Pro limitibus (log2) sine allocatione. */
i32
magnus_bitorum (
    Magnus a);

/* a mod n (Euclideum: 0 <= r < n) pro moduli verbi (1 <= n < 2^32):
 * per membra, sine piscina; n == 0 -> 0 */
i32
magnus_residuum_parvum (
    Magnus a,
       i32 n);

/* maximus divisor communis, semper >= 0; (0, 0) -> 0 */
Magnus
magnus_divisor_communis (
      Magnus  a,
      Magnus  b,
     Piscina* piscina);

/* idem cum TESTIBUS (Bezout): g = u*a + v*b - certificatum quod
 * quisque sine hac bibliotheca verificare potest */
Magnus
magnus_divisor_communis_testatus (
      Magnus  a,
      Magnus  b,
     Piscina* piscina,
      Magnus* u,
      Magnus* v);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) piscinae internae alternae in ultimo divisore
 * communi magnorum operandorum (Euclides in piscinis alternis); 0 si
 * via vocantis electa est (sine testibus: operandus aliquis pauci
 * membrorum; cum testibus: ambo). Computator sumptus
 * deterministicus: idem in omni machina, ergo probationibus
 * asseribilis. */
memoriae_index
magnus_apex_alternarum (
    vacuum);

#endif /* MAGNUS_H */
#line 1 "include/fractio.h"
/* fractio.h - Numeri rationales EXACTI
 *
 * Fractio = numerator / denominator, ambo Magnus, in forma canonica:
 * denominator > 0, divisor communis 1, nihil = 0/1. Valor immutabilis
 * in piscina, sicut magnus (membra partiri potest; effectus valet dum
 * piscinae argumentorum et effectus vivunt). Via celeris magni
 * hereditate: fractiones parvae sine allocatione manent ubi possibile.
 * Nulli fluitantes.
 *
 * Reductio Henrici (Knuth II, 4.5.1): divisores communes ANTE
 * multiplicationem tolluntur, ergo intermedia parva manent et effectus
 * iam reductus est.
 *
 * USUS:
 *   Fractio a = fractio_ex_s64(III);
 *   Fractio b;
 *   (vacuum)fractio_ex_s64_s64(I, IV, piscina, &b);     (* 1/4 *)
 *   Fractio c = fractio_adde(a, b, piscina);
 *   chorda t = fractio_ad_chordam(c, piscina);           (* "13/4" *)
 *
 * Vide lib/fractio.worklog.md.
 */
/* <aedilis corpus="lib/fractio.c"/> */
#ifndef FRACTIO_H
#define FRACTIO_H






/* Membra PRIVATA - per functiones legenda.
 * Invarians: denominator > 0; divisor communis numeratoris et
 * denominatoris 1; nihil = 0/1. Forma canonica et unica.
 *
 * SEMPER per fractio_ex_* (aut operationem) creanda. Fractio octetis
 * nullis impleta (memset, calloc, structura statica) NON est nihil sed
 * 0/0, quod tacite propagatur (0/0 + 1/2 = 0/0) - dissimile Magno,
 * cuius octeti nulli nihil validum sunt. Nihil: fractio_ex_s64(0). */
nomen structura {
    Magnus numerator;
    Magnus denominator;
} Fractio;


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Fractio
fractio_ex_s64 (
    s64 valor);

Fractio
fractio_ex_magno (
    Magnus valor);

/* numerator/denominator reducta; FALSUM si denominator nullus
 * (exitus non tangitur) */
b32
fractio_ex_s64_s64 (
         s64  numerator,
         s64  denominator,
     Piscina* piscina,
     Fractio* exitus);

b32
fractio_ex_magnis (
      Magnus  numerator,
      Magnus  denominator,
     Piscina* piscina,
     Fractio* exitus);

/* "a" aut "a/b" decimalis, signum '-' tantum ante numeratorem, b > 0,
 * nulla spatia; non canonica accipitur ("6/4" -> 3/2). FALSUM si
 * malformata aut b nullus (exitus non tangitur). */
b32
fractio_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Fractio* exitus);

/* copia in piscinam datam: membra NOVA numeratoris et denominatoris
 * (vide magnus_transcribe) */
Fractio
fractio_transcribe (
     Fractio  a,
     Piscina* piscina);

/* forma canonica: "a" si integra, aliter "a/b" */
chorda
fractio_ad_chordam (
     Fractio  a,
     Piscina* piscina);

Magnus
fractio_numerator (
    Fractio a);

/* semper > 0 */
Magnus
fractio_denominator (
    Fractio a);


/* ==================================================
 * Inspectio
 * ================================================== */

/* -1, 0, +1 */
s32
fractio_signum (
    Fractio a);

/* -1, 0, +1 sicut a <, =, > b; per a*d contra c*b (denominatores
 * positivi), sine divisione */
s32
fractio_compara (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

b32
fractio_aequalis (
    Fractio a,
    Fractio b);

/* denominator == 1 */
b32
fractio_est_integra (
    Fractio a);


/* ==================================================
 * Arithmetica
 * ================================================== */

Fractio
fractio_nega (
     Fractio  a,
     Piscina* piscina);

Fractio
fractio_absolutum (
     Fractio  a,
     Piscina* piscina);

Fractio
fractio_adde (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

Fractio
fractio_subtrahe (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

Fractio
fractio_multiplica (
     Fractio  a,
     Fractio  b,
     Piscina* piscina);

/* FALSUM si divisor nullus (exitus non tangitur) */
b32
fractio_divide (
     Fractio  a,
     Fractio  divisor,
     Piscina* piscina,
     Fractio* exitus);

/* 1/a; FALSUM si a nullus */
b32
fractio_inversa (
     Fractio  a,
     Piscina* piscina,
     Fractio* exitus);

/* basis^exponens, exponens signatus (corpus est: a^-n = 1/a^n);
 * FALSUM si basis nulla et exponens < 0; 0^0 = 1 */
b32
fractio_potentia (
     Fractio  basis,
         s32  exponens,
     Piscina* piscina,
     Fractio* exitus);


/* ==================================================
 * Ad integros
 * ================================================== */

/* maximus integer <= a */
Magnus
fractio_pavimentum (
     Fractio  a,
     Piscina* piscina);

/* minimus integer >= a */
Magnus
fractio_tectum (
     Fractio  a,
     Piscina* piscina);

/* integer proximus; dimidia AD PAREM (1/2 -> 0, 3/2 -> 2, 5/2 -> 2,
 * -1/2 -> 0): sine inclinatione cum multa dimidia rotundantur (regula
 * IEEE). Regula vulgaris ("a nihilo recedens") ex pavimento et tecto
 * facile componitur. */
Magnus
fractio_rotunda (
     Fractio  a,
     Piscina* piscina);

#endif /* FRACTIO_H */
#line 1 "include/polynomium.h"
/* polynomium.h - Polynomia Laurentiana EXACTA super magnum
 *
 * Z[t, t^-1]: polynomia unius variabilis, coefficientibus integris
 * magnis, exponentibus signatis (polynomia ordinaria = casus imus >=
 * 0). Valores immutabiles in piscina, sicut magnus: omnis operatio
 * novum valorem reddit; effectus coefficientes argumentorum PARTIRI
 * potest.
 *
 * Forma canonica: nullum = nulla membra (imus 0); aliter
 * coefficientes imus et summus non nulli. Ergo aequalitas =
 * aequalitas partium.
 *
 * EXPONENTES: |e| <= POLYNOMIUM_EXPONENS_MAXIMUS (2^30 - 1), ita ut
 * amplitudo omnis summae in i32 capiat et omne productum exponentis in
 * s64. Operationes quae exponentem accipiunt aut movent b32 reddunt:
 * FALSUM si exponens aliquis extra fines caderet (exitus non tangitur)
 * - numquam exundatio tacita. nega, adde, subtrahe, multiplica_scalari
 * exponentes non movent et valorem reddunt.
 *
 * MEMORIA densa: summus - imus + 1 coefficientes. t^1000000 unum
 * coefficientem habet, sed t^1000000 + 1 decies centena milia.
 * Amplitudo est sumptus: lectio ideo amplitudinem maiorem quam
 * POLYNOMIUM_AMPLITUDO_LECTIONIS refutat (textus XXX octetorum aliter
 * gigaoctetos posceret). Operationes ipsae nullum talem finem habent.
 *
 * USUS:
 *   Polynomium p;
 *   Polynomium q;
 *   (vacuum)polynomium_ex_chorda(chorda_ex_literis("t^2 - 1", piscina),
 *       't', piscina, &p);
 *   si (!polynomium_multiplica(p, p, piscina, &q)) refutare;
 *
 * Vide lib/polynomium.worklog.md.
 */
/* <aedilis corpus="lib/polynomium.c"/> */
#ifndef POLYNOMIUM_H
#define POLYNOMIUM_H







#define POLYNOMIUM_EXPONENS_MAXIMUS    ((s32)0x3FFFFFFFL)
#define POLYNOMIUM_AMPLITUDO_LECTIONIS  ((i32)0x100000L)   /* 2^20 */

/* Membra PRIVATA - per functiones legenda. coefficientes[i] pertinet
 * ad t^(imus + i). */
nomen structura {
     constans Magnus* coefficientes;
                 i32  numerus;
                 s32  imus;
} Polynomium;


/* ==================================================
 * Constructio et textus
 * ================================================== */

Polynomium
polynomium_nullum (
    vacuum);

Polynomium
polynomium_constans (
      Magnus  c,
     Piscina* piscina);

/* c t^exponens; FALSUM si exponens extra fines */
b32
polynomium_monomium (
        Magnus  c,
           s32  exponens,
       Piscina* piscina,
    Polynomium* exitus);

/* c[i] pro t^(imus + i), copiati; zephyra extrema absciduntur.
 * FALSUM si exponens coefficientis non nulli extra fines. */
b32
polynomium_ex_coefficientibus (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus,
             Piscina* piscina,
          Polynomium* exitus);

/* "3t^2 - t + 1 - 2t^-1": termini ordine quolibet, exponentes iterati
 * coniuncti, spatia et tabulae liberae; "0" = nullum. Littera
 * variabilis ASCII (a-z, A-Z). FALSUM si littera alia, textus
 * malformatus, exponens extra fines, aut summus - imus + 1 >
 * POLYNOMIUM_AMPLITUDO_LECTIONIS (exitus non tangitur). */
b32
polynomium_ex_chorda (
        chorda  textus,
     character  littera,
       Piscina* piscina,
    Polynomium* exitus);

/* copia profunda in piscinam datam: alveus et coefficientes novi (vide
 * magnus_transcribe) */
Polynomium
polynomium_transcribe (
    Polynomium  p,
       Piscina* piscina);

/* forma canonica: gradu summo primo, " + " / " - " inter terminos,
 * nec "1t" nec "t^1"; nullum = "0". Littera ASCII ut supra (aliter
 * textus non relegibilis). */
chorda
polynomium_ad_chordam (
    Polynomium  p,
     character  littera,
       Piscina* piscina);


/* ==================================================
 * Lectio
 * ================================================== */

b32
polynomium_est_nullum (
    Polynomium p);

/* 0 si nullum (vide polynomium_est_nullum) */
s32
polynomium_gradus_imus (
    Polynomium p);

/* 0 si nullum */
s32
polynomium_gradus_summus (
    Polynomium p);

/* coefficiens t^exponens; 0 extra */
Magnus
polynomium_coefficiens (
    Polynomium p,
           s32 exponens);

b32
polynomium_aequalis (
    Polynomium a,
    Polynomium b);

/* maximus divisor communis coefficientium, >= 0; nullum -> 0 */
Magnus
polynomium_contentum (
    Polynomium  p,
       Piscina* piscina);


/* ==================================================
 * Arithmetica
 * ================================================== */

Polynomium
polynomium_nega (
    Polynomium  a,
       Piscina* piscina);

Polynomium
polynomium_adde (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina);

Polynomium
polynomium_subtrahe (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina);

Polynomium
polynomium_multiplica_scalari (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina);

/* FALSUM si exponens extra fines */
b32
polynomium_multiplica (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus);

/* p^n; p^0 = 1 (etiam 0^0). FALSUM si exponens extra fines */
b32
polynomium_potentia (
    Polynomium  p,
           i32  n,
       Piscina* piscina,
    Polynomium* exitus);

/* q = a / b in Z[t, t^-1] (t unitas est). FALSUM si b nullum, si b
 * non dividit a, aut si exponens quotientis extra fines. */
b32
polynomium_divide_exacte (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* quotiens);


/* ==================================================
 * Substitutiones
 * ================================================== */

/* p t^k (coefficientes partiti, nulla allocatio); FALSUM si extra
 * fines */
b32
polynomium_translata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* t -> t^k, k != 0 (k = 0 FALSUM); k = -1 speculum (chiralitas) */
b32
polynomium_dilata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* inversa dilatationis: t^k -> t. FALSUM si k = 0 aut exponens
 * aliquis non divisibilis per k (uncinus Kauffman in A -> Jones in
 * t: A = t^(-1/4), ergo contrahe per -4 ubi exponentes A per 4
 * divisibiles sunt) */
b32
polynomium_contrahe (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus);

/* forma normalis ad unitatem +-t^k: imus = 0, coefficiens imus > 0
 * (Alexander definitum est ad +-t^k); nullum -> nullum. FALSUM si
 * summus - imus extra fines. */
b32
polynomium_normale (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* p(1/t): exponentes negati (fines symmetrici: semper intra) */
Polynomium
polynomium_inversum (
    Polynomium  p,
       Piscina* piscina);

/* coefficientes ordine inverso idem (p = t^k p(1/t), signo eodem):
 * symmetria omnis polynomii Alexander et Jones nodorum amphichiralium;
 * nullum symmetricum est */
b32
polynomium_est_symmetricum (
    Polynomium p);


/* ==================================================
 * Derivata, divisor communis, pars libera, translatio Taylor
 * (radices.h his innititur)
 * ================================================== */

/* p' : t^e -> e t^(e-1) (Laurent licet); FALSUM si exponens extra
 * fines */
b32
polynomium_derivata (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* maximus divisor communis in Q[t], forma primitiva, coefficiente
 * summo > 0 (unicus); gcd(0, 0) = 0, gcd(a, 0) = forma primitiva a.
 * Polynomia ORDINARIA solum (exponentes >= 0): in Z[t, t^-1] divisor
 * usque ad unitatem t^k ambiguus - FALSUM si exponens negativus adest. */
b32
polynomium_divisor_communis (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus);

/* pars libera quadratis: p / gcd(p, p'), primitiva, coefficiente summo
 * > 0 - easdem radices DISTINCTAS habet, simplices. nullum -> nullum,
 * constans -> 1. FALSUM sicut polynomium_divisor_communis. */
b32
polynomium_pars_libera (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus);

/* p(t + c), translatio Taylor; FALSUM si exponens negativus adest */
b32
polynomium_translatum (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina,
    Polynomium* exitus);


/* ==================================================
 * Valor
 * ================================================== */

/* p(x) per Hornerum super fractionem; FALSUM si x = 0 et exponens
 * negativus adest */
b32
polynomium_valor (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina,
       Fractio* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) officinae internae in ultima multiplicatione
 * aut divisione exacta per officinas computata; 0 si operatio parva in
 * piscina vocantis facta est. Computator sumptus deterministicus,
 * probationibus asseribilis (sicut magnus_apex_alternarum). */
memoriae_index
polynomium_apex_officinarum (
    vacuum);

#endif /* POLYNOMIUM_H */
#line 1 "include/interpolatio.h"
/* interpolatio.h - Aestimatio, interpolatio, restitutio EXACTA
 *
 * Polynomium (Laurentianum) in Z[t, t^-1] ex valoribus eius modulo
 * primos: per primum valores in punctis colliguntur, interpolatio
 * Lagrange modulo p coefficientes dat, deinde restitutio Sinica
 * (congruentia_restitue) trans primos coefficientes integros reddit.
 * Ita computatio gravis (e.g. contractio retis) in arithmetica i32
 * fit et effectus tamen exactus est.
 *
 * EXACTITUDO. Numerus primorum ex LIMITE PROBATO coefficientium
 * (limes_bitorum, a vocante demonstratus) sequitur, numquam ex eo
 * quod responsum mutari desiit. Probationes additae (puncta addita
 * per primum, primus unus additus post restitutionem) culpas
 * arithmeticae, limitis aut fenestrae capiunt - non exemplar falsum
 * sed sibi constans, quod solum oraculum alienum capit.
 *
 * PUNCTA. Aestimator punctum ipse ELIGIT (index -> punctum, valor):
 * ita puncta sine radice quadrata modulari enumerari possunt (uncinus
 * per modulum vertex: omega = k + 2, A = i omega^2). Aestimator tres
 * status reddit - UTILE, INUTILE (hic index nihil dat), ERRATUM
 * (defectus verus, statim FALSUM) - et conatus per primum
 * finiuntur.
 *
 * PASSUS. f(x) = summa c_k x^(imus + passus k): interpolatur g(y) =
 * x^(-imus) f(x) in y = x^passus, ita exponentes in una classe modulo
 * passus (uncinus Kauffman: passus 4) puncta (summus - imus)/passus +
 * 1 tantum poscunt. Gradus 0 (imus == summus == 0) = restitutio
 * scalaris: numerus integer trans primos.
 *
 * USUS:
 *   InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();
 *   q.gradus_imus    = -XII;
 *   q.gradus_summus  = XII;
 *   q.passus         = IV;
 *   q.limes_bitorum  = XVI;
 *   si (!interpolatio_polynomium(aestimare, &contextus, &q, piscina,
 *       &f)) refutare;
 *
 * Vide lib/interpolatio.worklog.md.
 */
/* <aedilis corpus="lib/interpolatio.c"/> */
#ifndef INTERPOLATIO_H
#define INTERPOLATIO_H






/* ==================================================
 * Interpolatio Lagrange modulo p
 * ================================================== */

/* numerus puncta DISTINCTA x (0 <= x < p, p primus) et valores y (0 <=
 * y < p) -> coefficientes c[0 .. numerus - 1] (c[j] pro t^j), unicum
 * polynomium gradus < numerus. FALSUM si numerus nullus, p non
 * primus, punctum aut valor >= p, aut puncta non distincta
 * (coefficientes tunc non tanguntur). Sumptus O(numerus^2). */
b32
interpolatio_lagrange_modulo (
    constans i32* x,
    constans i32* y,
             i32  numerus,
             i32  p,
         Piscina* piscina,
             i32* coefficientes);


/* ==================================================
 * Aestimatio et restitutio
 * ================================================== */

#define INTERPOLATIO_UTILE    I         /* punctum et valor scripti */
#define INTERPOLATIO_INUTILE  ZEPHYRUM  /* hic index nihil: proximum */
#define INTERPOLATIO_ERRATUM  (-I)      /* defectus verus: FALSUM statim */

/* limes_bitorum maximus admissus (2^24): ultra nullus usus, et summa
 * bitorum primorum in s64 tuta manet */
#define INTERPOLATIO_LIMES_MAXIMUS  ((i32)0x1000000UL)

/* aestimator: pro indice (0, 1, 2, ... intra primum) punctum x (0 <= x
 * < p) eligit et valorem f(x) mod p (0 <= valor < p) scribit, UTILE
 * reddens. Punctum 0 ut INUTILE omittitur (x^passus nihil
 * distinguit); punctum >= p aut valor >= p ut ERRATUM tractatur.
 * Status alius quam UTILE aut INUTILE = ERRATUM. */
nomen s32 (*InterpolatioAestimator) (
    vacuum* contextus,
       i32  index,
       i32  p,
       i32* punctum,
       i32* valor);

/* Membra a vocante post interpolatio_quaestio_ordinaria() posita:
 * membra futura ita valores ordinarios semper habebunt. */
nomen structura {
    s32 gradus_imus;      /* exponens minimus possibilis (0) */
    s32 gradus_summus;    /* maximus possibilis (0); (summus - imus) %
                           * passus == 0 */
    i32 passus;           /* exponentes imus + passus k (1) */
    i32 limes_bitorum;    /* |c_k| < 2^limes_bitorum PROBATUM (0 =
                           * omnes nulli) */
    i32 residuum_primi;   /* primi p = residuum (mod modulus_primi) (0);
                           * mdc(residuum mod modulus, modulus) == 1
                           * debet, aliter quaestio invalida */
    i32 modulus_primi;    /* < 2 = omnes primi (0) */
    i32 primus_limes;     /* primi < primus_limes descendentes
                           * (0xFFFFFFFF) */
    i32 puncta_addita;    /* puncta probationis per primum (I) */
    i32 conatus_maximi;   /* indices per primum ante FALSUM (2^20) */
} InterpolatioQuaestio;

InterpolatioQuaestio
interpolatio_quaestio_ordinaria (
    vacuum);

/* f in Z[t, t^-1] ex aestimationibus. Per primum: indices 0, 1, ...
 * dum K + puncta_addita puncta utilia, K = (summus - imus)/passus + 1,
 * cum y = punctum^passus DISTINCTIS (y iteratum omittitur, sed valorem
 * eundem dare debet); Lagrange in primis K, puncta addita congruere
 * debent. Primi descendentes infra primus_limes, filtro residui (per
 * gradus moduli), et soli qui satis y distincta dant ((p - 1) /
 * mdc(passus, p - 1) >= K + puncta_addita), donec summa floor(log2
 * p) >= limes_bitorum + 1 (productum > 2 max |c|); deinde UNUS
 * ADDITUS cuius residua restitutionem confirmare debent, et |c| <
 * 2^limes_bitorum. Coefficientes per congruentia_restitue
 * symmetricam; memoria effectus ex coefficientibus non nullis, non ex
 * fenestra.
 *
 * FALSUM (exitus non tangitur), SINE ULLA aestimatione, si quaestio
 * invalida: aestimare NIHIL, passus nullus, conatus_maximi nullus,
 * summus < imus, |imus| aut |summus| > POLYNOMIUM_EXPONENS_MAXIMUS,
 * fenestra non divisibilis per passum, K + puncta_addita > 2^31 - 1,
 * limes_bitorum > INTERPOLATIO_LIMES_MAXIMUS, filtrum non admissibile.
 * FALSUM etiam si aestimator ERRATUM reddit, conatus exhausti, primi
 * desunt, allocatio deficit, aut probatio discordat (y iteratum,
 * punctum additum, primus additus, limes). */
b32
interpolatio_polynomium (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* quaestio,
                          Piscina* piscina,
                       Polynomium* exitus);

#endif /* INTERPOLATIO_H */
#line 1 "include/congruentia.h"
/* congruentia.h - Arithmetica modularis EXACTA (Z/n, n < 2^32)
 *
 * "Si numerus a numerorum b, c differentiam metitur, b et c secundum a
 * congrui dicuntur" (Gauss, Disquisitiones Arithmeticae, art. 1).
 * Residua 0 <= x < n in i32 (insignatus), modulus explicitus: nulla
 * structura, ergo ansae calidae (gradus et determinantes modulo p,
 * restitutio Sinica) sine sumptu. Producta in i64: n^2 < 2^64, C89
 * sine 128 bitis.
 *
 * Modulus n quilibet 2 <= n < 2^32 (non solum primi): inversa FALSUM
 * reddit si mdc(a, n) != 1. Argumenta arithmeticae iam reducta esse
 * debent (0 <= a < n) - ex_s64 / ex_magno reducunt. Contractus non
 * custoditur (ansa calida): n == 0 in arithmetica divisio per nullum
 * est (indefinitum); argumenta non reducta effectum falsum dant.
 * ex_s64, ex_magno n == 0 -> 0 definiunt.
 *
 * USUS:
 *   i32 p = congruentia_primus_infra(0xFFFFFFFFU);
 *   i32 x = congruentia_ex_s64(-17, p);
 *   i32 inversa;
 *   si (congruentia_inversa(x, p, &inversa)) ...
 *
 * Vide lib/congruentia.worklog.md.
 */
/* <aedilis corpus="lib/congruentia.c"/> */
#ifndef CONGRUENTIA_H
#define CONGRUENTIA_H






/* ==================================================
 * Reductio
 * ================================================== */

/* x mod n, 0 <= r < n, etiam x negativus (S64 imus incluso) */
i32
congruentia_ex_s64 (
    s64 x,
    i32 n);

/* x mod n per membra magni (magnus_residuum_parvum), sine piscina */
i32
congruentia_ex_magno (
    Magnus x,
       i32 n);


/* ==================================================
 * Arithmetica (argumenta reducta: 0 <= a, b < n)
 * ================================================== */

i32
congruentia_adde (
    i32 a,
    i32 b,
    i32 n);

i32
congruentia_subtrahe (
    i32 a,
    i32 b,
    i32 n);

i32
congruentia_multiplica (
    i32 a,
    i32 b,
    i32 n);

/* a^e mod n; 0^0 = 1 */
i32
congruentia_potentia (
    i32 a,
    i64 e,
    i32 n);

/* a^-1 mod n; FALSUM si mdc(a, n) != 1 aut n < 2 (exitus non
 * tangitur) */
b32
congruentia_inversa (
     i32  a,
     i32  n,
     i32* exitus);

/* representans symmetricus: -n/2 < x <= n/2 */
s64
congruentia_symmetrica (
    i32 a,
    i32 n);


/* ==================================================
 * Primi
 * ================================================== */

/* Miller-Rabin deterministicus (bases 2, 7, 61: exactus pro n <
 * 4759123141 > 2^32) */
b32
congruentia_est_primus (
    i32 n);

/* primus maximus < limes; 0 si nullus (limes <= 2). Series pro
 * reconstructione: p0 = primus_infra(0xFFFFFFFF), p1 =
 * primus_infra(p0), ... */
i32
congruentia_primus_infra (
    i32 limes);


/* ==================================================
 * Reconstructio Sinica (CRT)
 * ================================================== */

/* x mod M = prod(moduli) ex residuis per Garner: 0 <= x < M, aut si
 * symmetricus (verum) representans symmetricus -M/2 < x <= M/2.
 * FALSUM si moduli
 * non bini coprimi, modulus < 2, aut residuum >= modulus suus (exitus
 * non tangitur). numerus 0 -> 0. */
b32
congruentia_restitue (
    constans i32* residua,
    constans i32* moduli,
             i32  numerus,
             b32  symmetricus,
         Piscina* piscina,
          Magnus* exitus);

#endif /* CONGRUENTIA_H */
#line 1 "include/anulus.h"
/* anulus.h - Descriptio anuli: operationes elementorum per tabulam
 * functionum
 *
 * Bibliothecae algebraicae (matrix, et futurae) super quemlibet anulum
 * domus operantur per hanc tabulam: elementa opaca magnitudinis datae,
 * operationes per indices functionum. Tres anuli parati: Z (magnus), Q
 * (fractio, corpus), Z[t, t^-1] (polynomium, littera 't').
 *
 * Operationes b32 reddunt: FALSUM si operatio elementi refutat
 * (polynomium: exponens extra fines; divisio exacta non exacta) -
 * exitus tunc non tangitur. Effectus in piscina data vivunt;
 * transcribe copiam PROFUNDAM facit (pro piscinis temporariis
 * reficiendis).
 *
 * USUS:
 *   constans Anulus* z = &ANULUS_INTEGRORUM;
 *   Magnus a = magnus_ex_s64(VI);
 *   Magnus b = magnus_ex_s64(VII);
 *   Magnus c;
 *   (vacuum)z->multiplica(z, &a, &b, piscina, &c);
 *
 * Vide lib/anulus.worklog.md.
 */
/* <aedilis corpus="lib/anulus.c"/> */
#ifndef ANULUS_H
#define ANULUS_H





nomen structura Anulus Anulus;

/* Omnis functio descriptionem ipsam primam accipit (anulus): anuli cum
 * parametris (Z/n: modulus in contextu) eadem tabula utuntur. */
structura Anulus {
    constans character* titulus;      /* "Z", "Q", "Z[t,t^-1]" */
        memoriae_index  mensura;      /* octeti elementi */
                   b32  corpus;       /* non nulli invertibiles */

    vacuum (*nullum) (
        constans Anulus* anulus,
        vacuum* exitus);
    vacuum (*unum) (
        constans Anulus* anulus,
        Piscina* piscina,
         vacuum* exitus);
    b32 (*est_nullum) (
        constans Anulus* anulus,
        constans vacuum* a);
    /* elementum sine memoria externa (totum in structura): Z in s64, Q
     * numerator et denominator in s64, Z[t] nullum solum. Algorithmi
     * hoc ad viam parvam (sine piscinis temporariis) eligendam
     * utuntur. */
    b32 (*parvum) (
        constans Anulus* anulus,
        constans vacuum* a);
    b32 (*aequalis) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b);
    b32 (*adde) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*subtrahe) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*multiplica) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    /* a / b in anulo: FALSUM si b nullus aut b non dividit a */
    b32 (*divide_exacte) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    vacuum (*transcribe) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina,
                 vacuum* exitus);
    chorda (*ad_chordam) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina);
    b32 (*ex_chorda) (
        constans Anulus* anulus,
          chorda  textus,
        Piscina*  piscina,
         vacuum*  exitus);

    /* Anuli Euclidei solum (NIHIL aliter: Q, Z[t, t^-1]) */
    /* g = u a + v b, g normalis (Z: g >= 0; (0, 0) -> 0) */
    b32 (*divisor_communis) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* g,
                 vacuum* u,
                 vacuum* v);
    /* a = q b + r, r reductus (Z: 0 <= r < |b|); FALSUM si b nullus */
    b32 (*divide_cum_residuo) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* q,
                 vacuum* r);
    /* norma Euclidea: -1, 0, +1 sicut N(a) <, =, > N(b) (Z: |a|
     * ad |b|); residuum divisionis normam stricte minorem habet quam
     * divisor */
    s32 (*compara_normam) (
        constans Anulus* anulus,
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina);

    /* parametra anuli (Z/n: modulus); NIHIL pro Z, Q, Z[t] */
    constans vacuum* contextus;
    /* anulus integer (sine divisoribus nullius): Z, Q, Z[t] VERUM; Z/n
     * sse n primus. Eliminatio (Bareiss: determinans, gradus, nucleus)
     * solum super anulum integrum valet - Z/6 [0,3,3; 4,4,5; 5,5,5]
     * cardine 4 (divisore nullius) columnam totam necabat, det 0 pro 3
     * (recensio anulus-I). */
    b32 integrum;

    /* anuli ORDINATI solum (NIHIL aliter: Z[t, t^-1], Z/n,
     * cyclotomia, extensio sine radice electa): signum a, -1 / 0 / +1;
     * FALSUM si refutatum. Geometria exacta (quaternio: angulus,
     * directio proxima) per quadrata et hoc signum comparat. */
    b32 (*signum) (
        constans Anulus* anulus,
        constans vacuum* a,
                Piscina* piscina,
                    s32* exitus);
};

/* Z: elementa Magnus */
extern constans Anulus ANULUS_INTEGRORUM;

/* Q: elementa Fractio (corpus) */
extern constans Anulus ANULUS_RATIONALIUM;

/* Z[t, t^-1]: elementa Polynomium, textus littera 't' */
extern constans Anulus ANULUS_POLYNOMIORUM;

/* Z/n (2 <= n < 2^32): elementa i32 (residua 0 <= x < n, congruentia),
 * corpus et integrum sse n primus; divide_exacte per inversam (FALSUM
 * si divisor non invertibilis). Descriptio in piscina vivit; NIHIL si
 * n < 2. Matrices comparantur per INDICEM descriptionis: una descriptio
 * pro quoque modulo (duae Z/7 anuli diversi sunt). */
constans Anulus*
anulus_residuorum (
         i32  n,
     Piscina* piscina);

#endif /* ANULUS_H */
#line 1 "include/contractio.h"
/* contractio.h - Retia tensorum et contractio eorum EXACTA
 *
 * Rete = indices (dimensio d >= 1) et tensores super indices
 * distinctos; quisque index in duobus tensoribus exacte (rete
 * clausum) summatur. Contractio valorem retis reddit: summa super
 * omnes assignationes indicum producti elementorum.
 *
 * STRUCTURA SEPARATA A VALORIBUS. Rete et ordo eius semel
 * aedificantur; valores per exsecutionem dantur. Unum rete, unus
 * ordo, una officina omnibus punctis et primis serviunt.
 *
 * ORDO. contractio_ordinare ordinem binarium deterministicum eligit
 * (v1: avidus, clavis exacta infra) et omnem tensorem intermedium in
 * una tabula (slab) disponit - offsets fixi per vitam, primum aptum.
 * Magnitudo tabulae in octetis ANTE allocationem nota est (sumptus);
 * officina semel allocatur et supra limitem vocantis refutatur, non
 * abortitur. Exsecutio calida deinde nihil allocat.
 *
 * CLAVIS AVIDA. Inter paria tensorum vivorum quae indicem saltem unum
 * communem habent, minimum (elementa effectus, saturata; deinde id
 * minor; deinde id maior). Effectus id proximum accipit (tensores
 * initiales 0 .. n - 1, intermedii n, n + 1, ...). Si nullum par
 * indicem communem habet (componentes disiunctae, scalares), eadem
 * clavis super OMNIA paria (productum externum). Clavis ab id
 * indicum non pendet: renumeratio indicum ordinem et sumptus non
 * mutat; ordo insertionis tensorum sumptus mutare potest.
 *
 * TENSOR INTERMEDIUS super indices non contractos, ordine id
 * ascendente; index ULTIMUS celerrimus (ut valores initiales).
 *
 * DUO GRADUS, UNUM RESPONSUM. Gradus calidus: residua i32 modulo n
 * (2 <= n < 2^32; accumulator duorum verborum, nulla exundatio).
 * Gradus referentiae: quilibet anulus domus (anulus.h). Probationes
 * gradum calidum accipiunt solum ubi responso referentiae modulo n
 * aequalis est.
 *
 * USUS:
 *   ContractioRete*     r = contractio_rete_creare(piscina);
 *   ContractioOptiones  o = contractio_optiones_ordinariae();
 *   ContractioOrdo*     ordo;
 *   ContractioOfficina* officina;
 *   i32 a, b, t0, t1;
 *   contractio_index_addere(r, II, &a);
 *   ... tensores ...
 *   ordo     = contractio_ordinare(r, &o, piscina);
 *   officina = contractio_officina_creare(ordo, limes, piscina);
 *   si (!contractio_exsequi_modulo(r, ordo, officina, valores, p,
 *       &valor)) refutare;
 *
 * Vide lib/contractio.worklog.md et project-specs/contractio-plan.md.
 */
/* <aedilis corpus="lib/contractio.c"/> */
#ifndef CONTRACTIO_H
#define CONTRACTIO_H





nomen structura ContractioRete     ContractioRete;     /* opacum */
nomen structura ContractioOrdo     ContractioOrdo;     /* opacum */
nomen structura ContractioOfficina ContractioOfficina; /* opacum */


/* ==================================================
 * Rete
 * ================================================== */

ContractioRete*
contractio_rete_creare (
    Piscina* piscina);

/* index dimensionis d >= 1; id = 0, 1, 2, ... ordine additionis.
 * FALSUM si d == 0 aut rete iam ordinatum (id non tangitur). */
b32
contractio_index_addere (
    ContractioRete* rete,
               i32  dimensio,
               i32* id);

/* tensor super indices distinctos (ordo valorum: index ULTIMUS
 * celerrimus; numerus 0 = scalaris, indices tunc NIHIL licet).
 * FALSUM si index ignotus, index bis in eodem tensore (v1: vocans
 * vestigium ipse plicat), index iam duobus tensoribus usus, aut rete
 * iam ordinatum - rete tunc non mutatur, id non tangitur. */
b32
contractio_tensor_addere (
        ContractioRete* rete,
          constans i32* indices,
                   i32  numerus,
                   i32* id);

/* VERUM sse quisque index in duobus tensoribus exacte (rete vacuum
 * clausum est) */
b32
contractio_rete_clausum (
    constans ContractioRete* rete);


/* ==================================================
 * Ordo
 * ================================================== */

/* methodi: AVIDUS = clavis T2 (ordinaria; ordo manu probabilis,
 * invarians sub renumeratione indicum). OPTIMUS (contractio-plan T5b) =
 * plures candidati, vilissimus per multiplicationes EXACTAS (deinde
 * octeti tabulae, deinde numerus candidati); effectus contractionis
 * idem, solum tempus et memoria mutantur. OPTIMUS deterministicus pro
 * semine et conatibus datis; sub renumeratione indicum non invarians
 * (min-fill nexus per id indicum frangit). */
#define CONTRACTIO_AVIDUS  I
#define CONTRACTIO_OPTIMUS II

/* candidati OPTIMUS, ordine fixo (numeri 1, 2, 3, 4 .. 3 + conatus) */
#define CONTRACTIO_CANDIDATUS_OMNES     ZEPHYRUM
#define CONTRACTIO_CANDIDATUS_AVIDUS    I    /* clavis T2 */
#define CONTRACTIO_CANDIDATUS_REDUCTIO  II   /* clavis E_r - E_a - E_b */
#define CONTRACTIO_CANDIDATUS_IMPLETIO  III  /* min-fill, nexus (gradus, id) */
#define CONTRACTIO_CANDIDATUS_FORTUITUS IV   /* + k: min-fill, nexus fortuiti */

/* conatus maximi (cursus min-fill fortuiti) et semen ordinarium */
#define CONTRACTIO_CONATUS_MAXIMI    M
#define CONTRACTIO_SEMEN_ORDINARIUM  ((i64)MMXXVI)
/* 2^20 multiplicationes ~ 1 ms per contractionem: infra, candidati
 * ceteri plus constant quam lucrari possunt (nodi tabulae: IV x
 * tempus probationum sine limine) */
#define CONTRACTIO_LIMEN_ORDINARIUM  ((i64)I << XX)

/* sumptus: omnia i64 SATURANTUR ad CONTRACTIO_SATURATUM (2^64 - 1);
 * latitudo ex magnitudinibus EXACTIS, non ex apice saturato */
#define CONTRACTIO_SATURATUM ((i64)~(i64)ZEPHYRUM)

nomen structura {
    i32 methodus;      /* CONTRACTIO_AVIDUS (ordinarius) | _OPTIMUS */
    i32 conatus;       /* OPTIMUS: cursus min-fill fortuiti (XVI) */
    i64 semen;         /* OPTIMUS: semen sortis (SEMEN_ORDINARIUM) */
    i64 octeti_maximi; /* OPTIMUS: candidati cum tabula maiore omittuntur
                        * (ordinarie CONTRACTIO_SATURATUM: nullus limes);
                        * si omnes maiores, tabula minima */
    i32 candidatus;    /* OPTIMUS: CANDIDATUS_OMNES aut unus coactus */
    i64 limen;         /* OPTIMUS (OMNES): si AVIDUS < limen multiplicationes
                        * poscit, ceteri non aestimantur - ordo vilis
                        * iam est (ordinarie CONTRACTIO_LIMEN_ORDINARIUM;
                        * 0 = omnes semper) */
} ContractioOptiones;

/* vocans SEMPER ab hac incipit: membra postea addita valores
 * ordinarios habent */
ContractioOptiones
contractio_optiones_ordinariae (vacuum);

nomen structura {
    i64 apex;                 /* elementa tensoris intermedii maximi
                               * (0 si nullus intermedius) */
    i64 octeti_officinae;     /* tabula residuorum i32, octeti */
    i64 multiplicationes;     /* summa per gradus producti dimensionum
                               * indicum unionis */
    i32 latitudo;             /* max per intermedios log2 elementorum,
                               * sursum (exacte, etiam ultra 2^64) */
} ContractioSumptus;

/* ordo binarius deterministicus (clavis avida supra) et dispositio
 * tabulae: offsets per vitam, primum aptum (effectus gradus non
 * superponitur operandis eius). Rete post hoc immutabile. NIHIL si
 * rete non clausum (v1; rete tunc mutabile manet) aut optiones
 * invalidae (methodus ignota; OPTIMUS: conatus > CONATUS_MAXIMI aut
 * candidatus > 3 + conatus). Rete vacuum aut tensor solus: ordo sine gradibus.
 * Memoria ordinis (magnitudine retis, non tensorum) per
 * piscina_allocare: in piscina certae magnitudinis defectus fatalis
 * est, non NIHIL. */
ContractioOrdo*
contractio_ordinare (
                 ContractioRete* rete,
    constans ContractioOptiones* optiones,
                        Piscina* piscina);

ContractioSumptus
contractio_sumptus (
    constans ContractioOrdo* ordo);


/* ==================================================
 * Inspectio ordinis (probationes, mensura)
 * ================================================== */

/* candidatus victor (1 pro AVIDUS; 0 si ordo NIHIL) */
i32
contractio_ordo_candidatus (
    constans ContractioOrdo* ordo);

/* sumptus SIMULATUS candidati k (1 ..): FALSUM si k ignotus aut non
 * aestimatus (OPTIMUS: candidatus abiectus quia multiplicationes eius
 * victorem iam superabant; aut candidatus alius coactus). */
b32
contractio_ordo_candidati (
    constans ContractioOrdo* ordo,
                        i32  k,
          ContractioSumptus* simulatus);

/* numerus graduum: n - 1 pro n >= 1 tensoribus, 0 pro rete vacuo */
i32
contractio_ordo_gradus_numerus (
    constans ContractioOrdo* ordo);

/* gradus k: tensores sinister < dexter in effectum contrahuntur.
 * FALSUM si k extra fines (exitus non tanguntur). */
b32
contractio_ordo_gradus (
    constans ContractioOrdo* ordo,
                        i32  k,
                        i32* sinister,
                        i32* dexter,
                        i32* effectus);

/* sedes tensoris intermedii in tabula (offset et elementa, in
 * elementis i32). FALSUM pro tensore initiali aut ignoto, et si
 * ELEMENTA tabulae saturantur (intermedius aut finis >= 2^64 - 1;
 * si solum octeti saturantur, sedes tamen redduntur) - exitus tunc
 * non tanguntur. */
b32
contractio_ordo_sedes (
    constans ContractioOrdo* ordo,
                        i32  tensor,
                        i64* offset,
                        i64* elementa);

/* indices tensoris (initialis: ordine vocantis; intermedius: ordine
 * id ascendente), memoria ordinis - non mutanda. Scalaris: numerus
 * 0, indices NIHIL. FALSUM si tensor ignotus (exitus non tanguntur). */
b32
contractio_ordo_indices (
    constans ContractioOrdo*  ordo,
                        i32   tensor,
               constans i32** indices,
                        i32*  numerus);


/* ==================================================
 * Exsecutio
 * ================================================== */

/* officina gradus calidi: tabula UNA allocatione
 * (piscina_conari_allocare) octeti_officinae octetorum, plus
 * structura parva; reutilis per omnia puncta et primos. NIHIL si
 * octeti_officinae > octeti_maximi aut > 2^62, si sumptus saturati
 * (octeti aut multiplicationes: 2^64 operationes numquam finiuntur),
 * aut si allocatio deficit. */
ContractioOfficina*
contractio_officina_creare (
    constans ContractioOrdo* ordo,
                        i64  octeti_maximi,
                    Piscina* piscina);

/* valores[k] = elementa tensoris k (ordine indicum vocantis, index
 * ultimus celerrimus), 0 <= v < modulus (praesumptum, non probatum in
 * ansa calida), 2 <= modulus < 2^32 (primus non necessarius). exitus
 * = tensor densus super indices APERTOS ordine id (v1: unum
 * elementum; rete vacuum: 1; tensor solus: valores[0][0] mod n). Nulla
 * allocatio; accumulator duorum verborum, exactus pro omni
 * longitudine summae. FALSUM si ordo non huius retis, officina non
 * huius ordinis, modulus < 2, aut valores (vel valores[k]) NIHIL. */
b32
contractio_exsequi_modulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
         ContractioOfficina*    officina,
    constans i32* constans*     valores,
                        i32     modulus,
                        i32*    exitus);

/* gradus referentiae: valores[k] = tabula elementorum anuli
 * (anulus->mensura octeti quisque), eadem dispositio. Intermedii in
 * piscina privata, summae in altera per elementum refecta; effectus
 * per anulus->transcribe in piscinam exitus. Ad retia parva: nullus
 * limes octetorum nisi 2^62 per intermedium. FALSUM (exitus non
 * tactus) si ordo non huius retis, sumptus saturati (multiplicationes
 * aut apex - hic nulla officina custodit), tabula intermedii > 2^62
 * octeti aut eius allocatio (conata) deficit, operatio anuli refutat,
 * aut valores NIHIL. Memoria elementorum (operationes anuli,
 * transcribe) per allocationem FATALEM: defectus ibi processum finit,
 * non FALSUM. */
b32
contractio_exsequi_anulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
    constans vacuum* constans*  valores,
           constans Anulus*     anulus,
                    Piscina*    piscina,
                     vacuum*    exitus);

#endif /* CONTRACTIO_H */
#line 1 "include/situs.h"
/* situs.h - Praedicata geometrica EXACTA: orientatio, sectio, contactus
 *
 * Decisiones geometricae (supra/infra, sectio, coplanaritas, motus
 * trianguli) exacte super coordinatas rationales (fractio): signum
 * determinantis, numquam fluitantes. Casus DEGENERES (collineares,
 * coplanares, contactus in extremo, proiectio non generica) NOMINANTUR
 * et numquam tacite in partem unam cadunt: vocans decernit (e.g.
 * directionem proiectionis mutat, motum refutat).
 *
 * MEMORIA: piscina est alveus temporarius. Praedicata quae signum aut
 * genus reddunt piscinam relinquunt ut acceperunt (notare ...
 * reficere), ergo in ansis calidis (O(n^2) paria segmentorum) memoria
 * non crescit.
 * Puncta ipsa in memoria vocantis manent.
 *
 * VICINI: segmenta polygoni contigua (vertice communi) per praedicata
 * generalia semper TANGUNT (vertex ipse contactus est). Pro eis
 * praedicata *_vicin* quaerunt num ALIUD punctum commune sit:
 * DISIUNCTA = solum vertex communis, TANGUNT = superpositio aut
 * degeneratio. Numquam SECANT.
 *
 * USUS (motus trianguli AB -> AC + CB in polygono ... Z A B W ...):
 *   segmentum alienum [p,q]:
 *     si (situs_triangulum_segmentum(a, b, c, p, q, piscina)
 *             != SITUS_DISIUNCTA) refutare motum;
 *   segmenta contigua [Z,A] et [B,W]:
 *     si (   situs_triangulum_vicinum(a, b, c, z, piscina)
 *             != SITUS_DISIUNCTA
 *         || situs_triangulum_vicinum(b, c, a, w, piscina)
 *             != SITUS_DISIUNCTA) refutare motum;
 *   motus inversus (AC + CB -> AB): idem triangulum, eaedem
 *   vocationes.
 *
 * Vide lib/situs.worklog.md.
 */
/* <aedilis corpus="lib/situs.c"/> */
#ifndef SITUS_H
#define SITUS_H





/* puncta: coordinatae rationales; integra (cancelli) per
 * denominatorem 1 */
nomen structura {
    Fractio x;
    Fractio y;
} PunctumPlani;

nomen structura {
    Fractio x;
    Fractio y;
    Fractio z;
} Punctum;

/* constructores e coordinatis integris (casus frequentissimus) */
PunctumPlani
situs_punctum_plani (
    s64 x,
    s64 y);

Punctum
situs_punctum (
    s64 x,
    s64 y,
    s64 z);

b32
situs_puncta_aequalia (
    Punctum a,
    Punctum b);


/* ==================================================
 * Orientatio
 * ================================================== */

/* signum (b - a) x (c - a): +1 sinistrorsum, 0 collineares, -1
 * dextrorsum */
s32
situs_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina);

/* signum det[b - a, c - a, d - a] (productum triplex): +1 si d ex parte
 * normalis (b - a) x (c - a), 0 coplanares, -1 aliter */
s32
situs_orientatio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);

/* valor ipse determinantis (sexies volumen tetraedri signatum): TESTIS
 * signi, verificabilis sine hac bibliotheca. Hic piscina retinetur
 * (effectus in ea vivit). */
Fractio
situs_volumen_sexies (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);


/* ==================================================
 * Contactus
 * ================================================== */

nomen enumeratio {
    SITUS_DISIUNCTA  = ZEPHYRUM,   /* nullum punctum commune */
    SITUS_SECANT     = I,          /* interiora in puncto uno */
    SITUS_TANGUNT    = II          /* contactus degener: extremum in
                                    * altero, collineares superpositae,
                                    * coplanaritas cum contactu */
} SitusContactus;

/* segmenta [a,b] et [c,d] in plano (segmentum nullum, a == b, ut
 * punctum tractatur) */
SitusContactus
situs_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina);

/* segmenta in spatio: polygonum se ipsum secat? Non coplanaria numquam
 * se tangunt. */
SitusContactus
situs_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina);

/* triangulum clausum [t0,t1,t2] et segmentum [p,q]. SECANT: segmentum
 * interius trianguli proprie transfigit; TANGUNT: contactus in margine,
 * vertice, extremo segmenti, aut coplanaritas cum contactu (triangulum
 * degener - collineare - semper TANGUNT aut DISIUNCTA). Motus trianguli
 * (AB -> AC + CB) licitus est sse DISIUNCTA pro omni segmento
 * alieno; contigua per situs_triangulum_vicinum. */
SitusContactus
situs_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina);


/* ==================================================
 * Transitus: supra et infra
 * ================================================== */

/* Proiectio parallela secundum v (vector non nullus; spectator in
 * directione +v, ergo punctum maioris v.P propius). SECANT:
 * proiectiones [a,b] et [c,d] proprie se secant; *superius = 0 si
 * [a,b] propius, 1 si [c,d]; *signum = signum transitus, regula
 * dextrae: signum det[o, u, v] (o supra, u infra, segmentis orientatis
 * a->b, c->d). TANGUNT: proiectio non generica (contactus in
 * proiectione non proprius) aut segmenta in spatio se secant - vocans
 * v mutat aut polygonum refutat. Exitus solum si SECANT scribuntur.
 * Sine coordinatis proiectis: omnia per producta triplicia cum v. */
SitusContactus
situs_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum);


/* idem, et si SECANT positiones transitus in segmentis: P = a + s (b -
 * a) = c + t (d - c) in proiectione, 0 < s, t < 1 (ordo transituum in
 * eodem segmento; codex Gauss). Exitus (etiam parametri) solum si
 * SECANT scribuntur; tunc piscina RETINETUR (parametri in ea vivunt),
 * aliter refecta. */
SitusContactus
situs_transitus_parametri (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum,
     Fractio* parametrum_ab,
     Fractio* parametrum_cd);


/* ==================================================
 * Vicini: vertice communi
 * ================================================== */

/* [a,b] et [b,c]: TANGUNT sse collinearia eadem directione ab b
 * (superpositio) aut segmentum nullum */
SitusContactus
situs_segmenta_vicina (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Piscina* piscina);

/* segmentum [t0,x] et triangulum clausum [t0,t1,t2]: TANGUNT sse
 * x - t0 in plano trianguli et in angulo ad t0 (margines inclusi),
 * aut x == t0, aut triangulum degener. Pro aliis verticibus ordinem
 * rota. */
SitusContactus
situs_triangulum_vicinum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  x,
     Piscina* piscina);

/* proiectiones [a,b] et [b,c] secundum v: TANGUNT sse proiectiones
 * eadem directione ab b superponuntur, aliqua in punctum proiicitur,
 * aut v nullus. Transitum non habent: DISIUNCTA aut TANGUNT. */
SitusContactus
situs_transitus_vicinus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  v,
     Piscina* piscina);


/* ==================================================
 * Reflexio
 * ================================================== */

/* p per planum (a, b, c) reflexum: p - 2 (n.(p - a)) / (n.n) n, n =
 * (b - a) x (c - a). FALSUM si a, b, c collinearia (planum nullum;
 * exitus non tangitur). Effectus in piscina vivit. */
b32
situs_reflexio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  p,
     Piscina* piscina,
     Punctum* exitus);

#endif /* SITUS_H */
#line 1 "include/laqueus.h"
/* laqueus.h - Laquei polygonales EXACTI in Q^3: nodi et catenae
 *
 * Laqueus = componentes clausae polygonales, verticibus rationalibus
 * (situs.h). Omnis decisio geometrica exacta (situs): simplicitas,
 * transitus in proiectione (supra/infra, signum), motus trianguli.
 * Invariantes ex diagrammate: scriptura (writhe), numerus ligationis,
 * uncinus Kauffman, Jones, Alexander (polynomium, matrix).
 *
 * Segmentum i = a vertice i ad sequentem eiusdem componentis (ultimus
 * ad primum). Valores immutabiles in piscina; motus laqueum NOVUM
 * reddunt. Functiones laqueum simplicem praesumunt (laqueus_simplex
 * probat).
 *
 * USUS:
 *   Laqueus trifolium;
 *   Diagramma d;
 *   Polynomium v;
 *   (vacuum)laqueus_ex_chorda(chorda_ex_literis("[(0, 0, 0), ...]",
 *       piscina), piscina, &trifolium);
 *   si (laqueus_diagramma_genericum(trifolium, piscina, &d)
 *       && diagramma_jones(d, piscina, &v)) ... (-t^4 + t^3 + t)
 *
 * Vide lib/laqueus.worklog.md.
 */
/* <aedilis corpus="lib/laqueus.c"/> */
#ifndef LAQUEUS_H
#define LAQUEUS_H









/* transitus plures uncinus per summam statuum (2^c) non computat */
#define LAQUEUS_TRANSITUS_MAXIMI XXIV
/* laqueus_diagramma_minimum: radius maximus (XXXII: ~ 140000
 * directiones; i32 sine signo - radius "negativus" aliter ~ 4.3e9
 * pendet) */
#define LAQUEUS_RADIUS_MAXIMUS XXXII

/* Membra PRIVATA - per functiones legenda */
nomen structura {
    constans Punctum* vertices;
                 i32  numerus;
        constans i32* initia;        /* componentes + 1 */
                 i32  componentes;
} Laqueus;

/* transitus in diagrammate: segmenta supra et infra, positiones in eis
 * (0 < s < 1), signum regulae dextrae (situs_transitus) */
nomen structura {
        i32 supra;
        i32 infra;
    Fractio parametrum_supra;
    Fractio parametrum_infra;
        s32 signum;
} Transitus;

/* Membra PRIVATA. percursus: transitus ordine laquei, elementum 2k
 * (transitus k supra) aut 2k + 1 (infra); initia_percursus per
 * componentes. */
nomen structura {
               Laqueus  laqueus;
               Punctum  directio;
    constans Transitus* transitus;
                   i32  numerus;
          constans i32* percursus;
          constans i32* initia_percursus;
} Diagramma;


/* ==================================================
 * Constructio et textus
 * ================================================== */

/* "[(x, y, z), (x, y, z), ...; (...)]": coordinatae fractiones,
 * componentes per ';', quaeque >= III vertices. FALSUM si malformatum
 * (exitus non tangitur). */
b32
laqueus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Laqueus* exitus);

/* ex punctis: componens k = puncta[initia[k] .. initia[k+1] - 1]
 * (initia componentes + 1 elementa, ultimum = numerus); copiantur.
 * FALSUM si componens < III vertices. */
b32
laqueus_ex_punctis (
    constans Punctum* puncta,
        constans i32* initia,
                 i32  componentes,
             Piscina* piscina,
             Laqueus* exitus);

chorda
laqueus_ad_chordam (
     Laqueus  l,
     Piscina* piscina);

i32
laqueus_numerus (
    Laqueus l);

i32
laqueus_componentes (
    Laqueus l);

Punctum
laqueus_vertex (
    Laqueus l,
        i32 i);

/* nulla segmenta se tangunt nisi contigua in vertice communi (et ea
 * solum ibi) */
b32
laqueus_simplex (
     Laqueus  l,
     Piscina* piscina);


/* ==================================================
 * Diagramma: proiectio secundum v
 * ================================================== */

/* FALSUM si proiectio non generica (contactus non proprius, segmenta
 * contigua superposita, transitus duo in eodem puncto segmenti): vocans
 * directionem mutat */
b32
laqueus_diagramma (
        Laqueus  l,
        Punctum  v,
        Piscina* piscina,
      Diagramma* exitus);

/* directiones ex serie fixa deterministica ((0,0,1), (1,2,3), ...),
 * deinde (1, k, k^2) pro k = 1, 2, ... donec generica; FALSUM si
 * laqueus non simplex (nulla directio generica). Conatus irriti
 * reficiuntur. */
b32
laqueus_diagramma_genericum (
        Laqueus  l,
        Piscina* piscina,
      Diagramma* exitus);

/* inter directiones v componentibus integris |v_i| <= radius (una ex
 * quoque +-v), diagramma genericum transituum paucissimorum; aequalia
 * -> primum ordine enumerationis (a, b, c crescentes). FALSUM si nulla
 * directio in ambitu generica (laqueus_diagramma_genericum tunc conari
 * potest; laqueo simplici semper succedit) aut radius nullus aut
 * radius > LAQUEUS_RADIUS_MAXIMUS. Sumptus ~ (2 radius + 1)^3 / 2
 * diagrammata; conatus reficiuntur. */
b32
laqueus_diagramma_minimum (
     Laqueus  l,
         i32  radius,
     Piscina* piscina,
   Diagramma* exitus);

i32
diagramma_numerus (
    Diagramma d);

Transitus
diagramma_transitus (
    Diagramma d,
          i32 k);


/* ==================================================
 * Invariantes
 * ================================================== */

/* scriptura (writhe): summa signorum */
s32
diagramma_scriptura (
    Diagramma d);

/* numerus ligationis componentium a != b: (summa signorum inter eas) /
 * 2 */
s32
diagramma_numerus_ligationis (
    Diagramma d,
          i32 a,
          i32 b);

/* uncinus Kauffman D in A (circulus nodatus O -> 1), per summam
 * statuum; FALSUM si transitus > LAQUEUS_TRANSITUS_MAXIMI */
b32
diagramma_uncinus (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);

/* Jones V(t) = f(A = t^(-1/4)), f = (-A^3)^(-w) uncinus(D); FALSUM si
 * exponentes dimidii (catenae componentium numeri paris - ante uncinum
 * refutatur) aut transitus nimii */
b32
diagramma_jones (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);

/* Alexander per praesentationem Wirtinger (calculus Fox, minor
 * matricis): nodi soli (FALSUM pro catenis); forma normalis ad +-t^k */
b32
diagramma_alexander (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);


/* ==================================================
 * Codex PD (KnotTheory, KnotInfo)
 * ================================================== */

/* codex PD nodi: 4 ora per transitum, ora 1..2c secundum nodum; quisque
 * transitus [infra intrans, deinde contra horologium] - positivus [i, o
 * exiens, i exiens, o intrans]. Ita KnotTheory et KnotInfo. Nodi soli;
 * memoria in piscina. */
b32
diagramma_pd (
     Diagramma   d,
       Piscina*  piscina,
           i32** pd);

/* invariantes ex codice PD nodi (transitus 0 = nodus trivialis; ora
 * 1..2c bis quaeque, infra X0 -> X2 consecutiva; transitus unus
 * refutatur: signum ex ordine orarum non determinatur). Codex validus:
 * os quodque semel intrans et semel exiens (componens una) et PLANUS (V
 * - E + F = 2 in systemate rotationis; codices nodorum virtualium
 * refutantur). FALSUM si codex invalidus aut transitus >
 * LAQUEUS_TRANSITUS_MAXIMI (uncinus, Jones). */
b32
laqueus_uncinus_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus);

b32
laqueus_jones_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus);

b32
laqueus_alexander_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus);


/* ==================================================
 * Uncinus per modulum vertex et contractionem (sine limite 2^c)
 *
 * Spin +-1 per os (index dimensionis 2), gyrationes ossium ex arbore
 * dualis (facies externa +4, ceterae -4), tensor per transitum; rete
 * per contractio (ordo avidus, officina una) modulo primos 1 mod 4 ad
 * puncta A = i omega^2 aestimatur, uncinus per interpolatio
 * restituitur EXACTE (limes probatus per arbores extensas graphorum
 * Tait, numquam > 2c bitorum; fenestra
 * Kauffman in classe 3w modulo 4, passus 4). Nullus limes transituum
 * praeter octeti_maximi (officina). Vide
 * project-specs/contractio-plan.md.
 * ================================================== */

nomen structura LaqueusModulusVertex LaqueusModulusVertex; /* opacum */

/* structura semel: gyrationes, rete, ordo, officina (<= octeti_maximi
 * octeti). Validatio eadem ac laqueus_uncinus_ex_pd; NIHIL etiam si
 * transitus < 2 aut officina refutata. Memoria in piscina. */
LaqueusModulusVertex*
laqueus_modulus_vertex_creare (
    constans i32* pd,
             i32  transitus,
             i64  octeti_maximi,
         Piscina* piscina);

/* uncinus <D> in puncto: A = i omega^2 (i = g^((p-1)/4), g non
 * residuum quadraticum minimum), valor = <D>(A) modulo p.
 * INTERPOLATIO_UTILE (punctum = A); INTERPOLATIO_INUTILE si omega = 0
 * aut d = omega^4 + omega^-4 = 0 (A^4 = -1); INTERPOLATIO_ERRATUM si
 * p < 5, p non 1 modulo 4, aut p non primus (Miller-Rabin
 * deterministicus, congruentia_est_primus). Nihil
 * allocat (officina reutitur); primus mutatus i recomputat. */
s32
laqueus_modulus_vertex_aestimare (
    LaqueusModulusVertex* mv,
                     i32  omega,
                     i32  p,
                     i32* punctum,
                     i32* valor);

/* gyrationes (quartae) per os 0..2c-1, directione a primo occursu in
 * codice ad secundum; memoria structurae */
constans s32*
laqueus_modulus_vertex_gyrationes (
    constans LaqueusModulusVertex* mv);

ContractioSumptus
laqueus_modulus_vertex_sumptus (
    constans LaqueusModulusVertex* mv);

/* ordo contractionis retis (CONTRACTIO_OPTIMUS: contractio_ordo_
 * candidatus et _candidati legi possunt); NIHIL si mv NIHIL */
constans ContractioOrdo*
laqueus_modulus_vertex_ordo (
    constans LaqueusModulusVertex* mv);

/* limes coefficientium uncinus (contractio-plan T5a): B = minimum
 * super duos graphos Tait (regiones unius coloris, arcus per
 * transitum) producti Pi_{v != r} gradus(v) (ansis exclusis, r vertex
 * gradus maximi). Sigma_k |c_k| <= arbores extensae <= B (expansio
 * Thistlethwaite signata), ergo |c_k| < 2^limes. _limes = bita(B)
 * (0 si mv NIHIL; numquam > 2c); _limes_exactus = B ipse. */
i32
laqueus_modulus_vertex_limes (
    constans LaqueusModulusVertex* mv);

b32
laqueus_modulus_vertex_limes_exactus (
    constans LaqueusModulusVertex* mv,
                           Magnus* exitus);

/* uncinus exactus in structura iam aedificata (ut
 * laqueus_uncinus_ex_pd_rete, sine aedificatione iterata). FALSUM si
 * restitutio refutat (exitus non tangitur). */
b32
laqueus_modulus_vertex_uncinus (
    LaqueusModulusVertex* mv,
                 Piscina* piscina,
              Polynomium* exitus);

/* contractiones factae (puncta in aestimare usque ad contractionem
 * perducta) et mutationes primi ab creatione - ut probationes et
 * mensurae limitem in usu videant. Exitus NIHIL licet. */
vacuum
laqueus_modulus_vertex_vocationes (
    constans LaqueusModulusVertex* mv,
                              i32* aestimationes,
                              i32* primi);

/* uncinus Kauffman et Jones ex codice PD per modulum vertex; codex ut
 * laqueus_uncinus_ex_pd (transitus 0 -> 1; transitus unus refutatur).
 * sumptus NIHIL licet (aliter sumptus contractionis; 0 pro transitu
 * 0). FALSUM si codex invalidus aut officina > octeti_maximi (exitus
 * non tangitur). */
b32
laqueus_uncinus_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus);

b32
laqueus_jones_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus);

/* codex PD per motus Reidemeister I et II simplificatus: dum motus
 * aliquis applicari potest, transitus removentur (R1: ansa - transitus
 * cuius duo ora vicina idem sunt; R2: duo transitus duo ora
 * communicantes, eodem filo supra utrumque - in nodo aut bilaterum
 * facies aut summa connexa T1 # T2 per eos, utrimque isotopia). Ora
 * deinde renumerantur 1..2c' secundum nodum, transitus [infra intrans,
 * contra horologium] ut in diagramma_pd. Isotopia: invariantes omnes
 * idem. Nullo motu possibili codex idem redditur.
 *
 * AVIDUS, non completus: motus R3 non tentatur, ergo minimum localem
 * reddere potest. Sed c' == 0 = nodus trivialis PROBATUS (diagramma
 * sine transitu; pd_exitus NIHIL). Transitus unus accipitur (semper
 * ansa). Nodi soli; FALSUM si codex invalidus (exitus non tangitur).
 * Memoria in piscina. */
b32
laqueus_pd_simplificare (
    constans i32*  pd,
             i32   transitus,
         Piscina*  piscina,
             i32** pd_exitus,
             i32*  transitus_exitus);

/* codex PD clausurae tricae (plexus): verbum[k] = +-i (1 <= i < fila)
 * = sigma_i^(+-1). Conventio: sigma_i^(+1) transitum POSITIVUM dat
 * (scriptura +1), ergo clausura sigma_1^3 = trifolium dextrum (3_1
 * tabulae), (sigma_1 .. sigma_(p-1))^q = T(p, q) dextrum. Ora 1..2c per
 * percursum renumerantur, transitus [infra intrans, contra horologium]
 * ut in diagramma_pd; transitus k = littera k verbi. Codex reddendus
 * per _pd_legere probatur (c >= 2). fila 1 et verbum vacuum: nodus
 * trivialis (transitus 0, pd_exitus NIHIL). FALSUM (exitus non tacti)
 * si fila 0, littera 0 aut |littera| >= fila, verbum vacuum cum fila >= 2,
 * filum quod nulla littera tangit, aut clausura plures componentes
 * habet (permutatio verbi non cyclus fila: e.g. longitudo et fila
 * eiusdem paritatis non possunt nodum dare). Memoria in piscina.
 * Litterae eaedem ac Plexus (plexus.h): vocans plexum tenens
 * laqueus_pd_ex_plexu(p.litterae, p.longitudo, p.fila, ...) vocat -
 * caput plexus.h hic non includitur (eius dependentiae graves). */
b32
laqueus_pd_ex_plexu (
    constans s32*  verbum,
             i32   longitudo,
             i32   fila,
         Piscina*  piscina,
             i32** pd_exitus,
             i32*  transitus_exitus);

/* determinans nodi |Delta(-1)|, magnus exactus; nodi soli (FALSUM pro
 * catenis, sicut diagramma_alexander). Residua in piscina manent. */
b32
diagramma_determinans (
     Diagramma  d,
       Piscina* piscina,
        Magnus* exitus);


/* ==================================================
 * Motus trianguli
 * ================================================== */

/* segmentum i -> (v_i, c, v_sequens) sse triangulum nihil aliud tangit
 * (situs_triangulum_segmentum, _vicinum) et non degener */
b32
laqueus_motus_addere (
     Laqueus  l,
         i32  i,
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus);

/* vertex i removetur (v_prior -> v_sequens) sse triangulum nihil aliud
 * tangit; vertex collinearis inter vicinos semper; componens >= IV
 * vertices requirit */
b32
laqueus_motus_removere (
     Laqueus  l,
         i32  i,
     Piscina* piscina,
     Laqueus* exitus);

/* vertices avide per motus LEGITIMOS removentur
 * (laqueus_motus_removere): iterum vertex primus (ordine indicum) cuius
 * remotio legitima est, donec nullus. Isotopia ambiens: typus
 * nodi/catenae idem. Componens quisque >= III vertices servat. FALSUM
 * si l non simplex (probatio motus immersionem praesumit). Memoria
 * O(n): conatus irriti reficiuntur. */
b32
laqueus_simplificare (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus);

/* speculum (z -> -z): Jones(t) -> Jones(1/t), Alexander idem, numeri
 * ligationis negati */
b32
laqueus_speculum (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus);

#endif /* LAQUEUS_H */
#line 1 "include/chorda_aedificator.h"
/* chorda_aedificator.h - chordas accumulare (string builder) */
#ifndef CHORDA_AEDIFICATOR_H
#define CHORDA_AEDIFICATOR_H






/* ==================================================
 * ChordaAedificator - Accumulatio efficax chordarum
 *
 * Ad chordas aedificandas quando magnitudo finalis ignota.
 * Destinatus ad generationem texti structurati:
 * - INI/JSON/XML aedificatores (casus primarii)
 * - Formatatio diariorum
 * - Generatio texti ex structuris datorum
 *
 * EXEMPLUM:
 *   				   Piscina* p = piscina_generare_dynamicum("probatio", 4096);
 *   ChordaAedificator* a = chorda_aedificator_creare(p, 256);
 *
 *   chorda_aedificator_appendere_literis(a, "nomen");
 *   chorda_aedificator_appendere_character(a, ':');
 *   chorda_aedificator_appendere_integer(a, 42);
 *   chorda_aedificator_appendere_lineam_novam(a);
 *
 *   chorda fructus = chorda_aedificator_finire(a);
 *   piscina_destruere(p);
 *
 * PROPRIETATES:
 * - Crescentia automatica capacitatis (duplicatur quando plenus)
 * - Indentationis observatio (pro impressione pulchra)
 * - Effugium specificum formati (JSON effugium inclusum)
 * - Reutilizabilis per reset()
 * - Omnes allocationes ex Piscina data
 *
 * ================================================== */

nomen structura ChordaAedificator ChordaAedificator;


/* ==================================================
 * Creatio / Destructio
 * ================================================== */

ChordaAedificator*
chorda_aedificator_creare (
           Piscina* piscina,
    memoriae_index  capacitas_initialis);

vacuum
chorda_aedificator_destruere (
    ChordaAedificator* aedificator);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

b32
chorda_aedificator_appendere_character (
    ChordaAedificator* aedificator,
            character  c);


/* ==================================================
 * Appendere - Chordae (Chordae et C-chordae)
 * ================================================== */

b32
chorda_aedificator_appendere_literis (
     ChordaAedificator* aedificator,
    constans character* cstr);

b32
chorda_aedificator_appendere_chorda (
    ChordaAedificator* aedificator,
               chorda  s);


/* ==================================================
 * Appendere - Numeri (Integri)
 * ================================================== */

b32
chorda_aedificator_appendere_s32 (
    ChordaAedificator* aedificator,
                  s32  n);

b32
chorda_aedificator_appendere_i32 (
    ChordaAedificator* aedificator,
                  i32  n);


/* ==================================================
 * Appendere - Numeri (Puncta Fluitantia)
 *
 * decimales: numerus locorum decimalium ostendorum
 * ================================================== */

b32
chorda_aedificator_appendere_f64 (
    ChordaAedificator* aedificator,
                  f64  n,
                  i32  decimales);

/* appendere_repetita: appende characterem n vicibus
 * Utile ad padding vel indentationem */
b32
chorda_aedificator_appendere_repetita (
    ChordaAedificator* aedificator,
            character  c,
                  i32  numerus);

/* appendere_hex_i32: appende integrum ut hexadecimale (minusculae, sine 0x)
 * Utile ad debugging vel IDs */
b32
chorda_aedificator_appendere_hex_i32 (
    ChordaAedificator* aedificator,
                  i32  n);


/* ==================================================
 * Appendere - Evasus (Specificus Formati)
 *
 * appendere_evasus_json: Effugium chordae JSON
 *   - Tractat: citationem, virgulam inversam, characteres imperantes
 *   - Proprie effugit ad usum in chordis JSON
 * ================================================== */

b32
chorda_aedificator_appendere_evasus_json (
    ChordaAedificator* aedificator,
               chorda  s);

b32
chorda_aedificator_appendere_literis_evasus_json (
     ChordaAedificator* aedificator,
    constans character* cstr);


/* ==================================================
 * Appendere - Spatium Album et Structura
 * ================================================== */

b32
chorda_aedificator_appendere_lineam_novam (
    ChordaAedificator* aedificator);

/* appendere_indentationem: appende N spatia (pro gradu currenti)
 * ubi N = gradus * spatia_per_gradum (typice 2 vel 4)
 *
 * Trade indentatio_gradus ab aedificatore, vel specifica manualiter */
b32
chorda_aedificator_appendere_indentationem (
    ChordaAedificator* aedificator,
                  i32  gradus);


/* ==================================================
 * Indentationis Observatio (pro impressione pulchra)
 *
 * Aedificatores observant internum gradum indentationis.
 * push_indentationem(): incrementa gradum
 * pop_indentationem(): decrementa gradum
 * indentatio_gradus(): interroga gradum currentem
 *
 * Modus communis:
 *   chorda_aedificator_appendere_literis(a, "{");
 *   chorda_aedificator_appendere_lineam_novam(a);
 *   chorda_aedificator_push_indentationem(a);
 *   chorda_aedificator_appendere_indentationem(a,
 *       chorda_aedificator_indentatio_gradus(a));
 *   ... contenta appendere ...
 *   chorda_aedificator_pop_indentationem(a);
 * ================================================== */

vacuum
chorda_aedificator_push_indentationem (
    ChordaAedificator* aedificator);

vacuum
chorda_aedificator_pop_indentationem (
    ChordaAedificator* aedificator);

i32
chorda_aedificator_indentatio_gradus (
    ChordaAedificator* aedificator);


/* ==================================================
 * Quaestio - Interrogatio Status
 * ================================================== */

/* longitudo: longitudo currens chordae accumulatae */
memoriae_index
chorda_aedificator_longitudo (
    ChordaAedificator* aedificator);

/* spectare: vide contentum currentem sine finiendo
 * Reddit chordam spectationem buffer currenti.
 * Validus solum usque ad proximam mutationem. */
chorda
chorda_aedificator_spectare (
    ChordaAedificator* aedificator);

/* truncare: longitudinem ad valorem minorem reducere (numquam
 * auget; capacitas servatur). Pro redditione optimistica cum
 * reversione: signum = longitudo(), scribe, si displicet
 * truncare(signum). */
vacuum
chorda_aedificator_truncare (
    ChordaAedificator* aedificator,
       memoriae_index  longitudo_nova);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* reset: purga contentum, serva capacitatem allocatam
 * Utile ad reutilizandum aedificatorem pro chordis multiplicibus */
vacuum
chorda_aedificator_reset (
    ChordaAedificator* aedificator);

/* finire: converte aedificatorem ad chordam
 * Transfert dominium chordae accumulatae ad vocantem.
 * Aedificator destruitur post finire.
 * Vocans debet finaliter deallocare per piscinam. */
chorda
chorda_aedificator_finire (
    ChordaAedificator* aedificator);


/* ==================================================
 * Constantae Configurationis
 * ================================================== */

/* CHORDA_AEDIFICATOR_INDENTATIO_SPATIA
 * Numerus spatiorum per gradum indentationis (typice 2 vel 4) */
#define CHORDA_AEDIFICATOR_INDENTATIO_SPATIA II


#endif /* CHORDA_AEDIFICATOR_H */
#line 1 "include/xar.h"
/* xar.h - tabula crescens segmentata (growable array, vector)
 * "Codex Tabulae Exponentialis" - segmenta NON contigua: numquam
 * xar_obtinere(x, 0) ut tabulam planam lege
 *
 * Tabula crescens sine reallocatio
 *
 * PHILOSOPHIA:
 * - Segmenta duplicant magnitudinem: primi, primi, 2×primi, 4×primi, 8×primi...
 * - Duo prima segmenta eiusdem magnitudinis (optimizatio pro parvis tabulis)
 * - Elementa numquam movent (indices stabiles permanent)
 * - Omnia ex piscina allocata (destructio simplex)
 * - Algorithmus O(1) pro locatio indicis
 *
 * USUS:
 *   Xar* xar = xar_creare(piscina, sizeof(MeusTypus));
 *   MeusTypus* elem = xar_addere(xar);
 *   MeusTypus* get = xar_obtinere(xar, index);
 */

#ifndef XAR_H
#define XAR_H




/* Constantae (RP pars II, 2026-09-02: tabula segmentorum per xar
 * ALLOCATA, non in capite inscripta - caput DLXXVI -> XLVIII octeti;
 * primum IV, non XVI: ex XI M Xar triviorum in lib/stml.c X M unum
 * elementum tenebant, CXXVIII octeti pro VIII). Segmentum k primum x
 * 2^(k-1) tenet: tabula ordinaria XXIV segmenta = cum primo IV XXXIII M
 * elementa (primum x 2^XXIII); ultra fines REFUSIO
 * nominata (stderr, xar_addere NIHIL), numquam truncatio tacita -
 * xar_creare_magnum tabulam LXIV dat. */
#define XAR_SEGMENTA_ORDINARIA      XXIV    /* tabula ordinaria */
#define XAR_MAXIMUS_SEGMENTORUM     LXIV    /* tabula magna (xar_creare_magnum) */
#define XAR_PRIMUS_SEGMENTUM        IV      /* elementa in primo segmento */

/* TABULA CRESCENS (2026-10-21): segmenta duo prima in capite INSCRIPTA;
 * tabula plena (segmenta_maxima) ex piscina solum cum segmentum
 * tertium petitur. Proiectio STML plagulae toml LXX K: XCVI M Xar,
 * LIII M vacua, capita (CCXL octeti) LXV% arenae - nunc LXIV octeti. */
#define XAR_SEGMENTA_INLINEA        II

/* Vexilla */
#define XAR_VEXILLUM_ORDINARIUS     ZEPHYRUM    /* Ordinarius: memoria ad zephyrum */
#define XAR_VEXILLUM_SINE_ZEPHYRUM  I           /* Non zephyrum facere memoriam in allocatio */

/* XarIterator - Iterator pro ambulatione omnium elementorum
 */
nomen structura XarIterator {
    constans structura Xar* xar;     /* Tabula iterata */
                              i32  index_currens;         /* Index currens */
                              i32  index_segmenti;        /* Segmentum currens */
                              i32  offset_in_segmento;    /* Offset in segmento */
                           vacuum* basis_segmenti;        /* Basis segmenti currentis */
                              i32  finis_segmenti;        /* Finis segmenti currentis */
} XarIterator;

/* XarComparator - Index functionis pro comparationibus
 * Redde: -1 si a < b, 0 si a == b, +1 si a > b
 */
nomen s32 (*XarComparator)(constans vacuum* a, constans vacuum* b);

/* Xar - Structura principalis
 * "Tabula Exponentialis"
 */
nomen structura Xar {
                   i32 numerus_elementorum; /* Quot elementa? */
                   i32 magnitudo_elementi;  /* Quam magnus elementum? (in bytes) */
                   i32 magnitudo_primi;     /* Magnitudo primi segmenti */
                   i32 numerus_segmentorum; /* Quot segmenta allocata? */
                   i32 capacitas_totalis;   /* Capacitas totalis */
                   i32 vexilla;             /* Signa comportamenti */
                   i32 segmenta_maxima;     /* fines tabulae: ORDINARIA | MAXIMUS */
                   i32 mensura_tabulae;     /* longitudo tabulae currentis:
                                              * INLINEA, deinde segmenta_maxima */
               Piscina* piscina;             /* Piscina memoriae */
               vacuum** segmenta;            /* -> segmenta_inlinea, aut tabula
                                              * plena ex piscina (segmento
                                              * tertio petito) */
                vacuum* segmenta_inlinea[XAR_SEGMENTA_INLINEA];
} Xar;


/* ========================================================================
 * FUNCTIONES PRINCIPALES
 * ======================================================================== */

/* Xar Creare - Creare novum xar
 * "Genesis tabulae exponentialis"
 */
Xar*
xar_creare (
             Piscina* piscina,
                 i32  magnitudo_elementi);

/* Xar Creare Cum Magnitudine
 * "Creare cum magnitudine prima specifica"
 */
Xar*
xar_creare_cum_magnitudine (
        Piscina* piscina,
            i32  magnitudo_elementi,
            i32  magnitudo_primi);

/* Xar Creare Cum Vexillis
 * "Creare cum omnibus optionibus"
 */
Xar*
xar_creare_cum_vexillis (
        Piscina* piscina,
            i32  magnitudo_elementi,
            i32  magnitudo_primi,
            i32  vexilla);

/* Xar Creare Magnum - tabula segmentorum LXIV (XAR_MAXIMUS_SEGMENTORUM)
 * pro tabulis vere ingentibus (> XXXIII M elementa cum primo IV); forma
 * ordinaria XXIV segmenta habet et ultra REFUSAT nominatim. */
Xar*
xar_creare_magnum (
        Piscina* piscina,
            i32  magnitudo_elementi,
            i32  magnitudo_primi);

/* Xar Destruere
 * "Cum piscina, nihil agendum!"
 */
vacuum
xar_destruere (
        Xar* xar);

/* Xar Ponere Vexilla
 * "Ponere vexilla comportamenti"
 */
vacuum
xar_ponere_vexilla (
        Xar* xar,
        i32  vexilla);


/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Obtinere - Obtinere elementum ad indicem
 * "Obtinere elementum ad indicem"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus
 */
vacuum*
xar_obtinere (
        constans Xar* xar,
                 i32  index);

/* Xar Obtinere Signatum - Obtinere cum indice signato
 * "Pro iterationibus cum decrementis"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus vel negativus
 */
vacuum*
xar_obtinere_s (
        constans Xar* xar,
                 s32  index);

/* Xar Obtinere Vel Creare - Obtinere elementum vel creare si necessarium
 * "Obtinere elementum vel creare si necessarium"
 *
 * Utile pro tabulis dispersis - allocat omnia segmenta usque ad indicem
 */
vacuum*
xar_obtinere_vel_creare (
        Xar* xar,
        i32  index);

/* Xar Ponere - Ponere elementum ad indicem
 * "Ponere elementum ad indicem"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
xar_ponere (
                                Xar* xar,
                                i32  index,
                    constans vacuum* elementum);


/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere - Addere elementum ad finem
 * "Addere elementum ad finem"
 *
 * Redde: Index ad elementum novum, vel NIHIL si allocatio fracta
 */
vacuum*
xar_addere (
        Xar* xar);

/* Xar Addere Multos - Addere elementa multa
 * "Addere elementa multa - operatio moles"
 *
 * Optimizatum pro insertionibus magnis - usat memcpy per blocos
 * Redde: Numerus elementorum cum successu additorum
 */
i32
xar_addere_multos (
                                Xar* xar,
                    constans vacuum* elementa,
                                i32  numerus);


/* ========================================================================
 * ITERATIO
 * ======================================================================== */

/* Xar Iterator Initium - Initializare iteratorem
 * "Initium iterationis"
 */
XarIterator
xar_iterator_initium (
        constans Xar* xar);

/* Xar Iterator Proximum - Obtinere elementum proximum
 * "Obtinere elementum proximum"
 *
 * Redde: Index ad elementum proximum, vel NIHIL si completus
 */
vacuum*
xar_iterator_proximum (
        XarIterator* iter);

/* Xar Iterator Finis - Verificare si iteratio completa
 * "Verificare si iteratio completa"
 *
 * Redde: VERUM si finis, FALSUM si adhuc elementa
 */
b32
xar_iterator_finis (
        XarIterator* iter);


/* ========================================================================
 * QUAESTIO
 * ======================================================================== */

/* Xar Invenire - Quaerere elementum linearie
 * "Quaerere elementum linearie"
 *
 * Redde: Index ad elementum, vel NIHIL si non inventus
 */
vacuum*
xar_invenire (
           constans         Xar* xar,
           constans      vacuum* clavis,
                  XarComparator  comparator);

/* Xar Invenire Index - Quaerere elementum linearie, reddere indicem
 * "Quaerere elementum linearie, reddere indicem"
 *
 * Redde: Index elementi, vel -1 si non inventus
 */
s32
xar_invenire_index (
           constans         Xar* xar,
           constans      vacuum* clavis,
                  XarComparator  comparator);

/* Xar Quaerere Binarie - Quaerere binarie (assumit ordinatum)
 * "Quaerere binarie - assumit ordinatum"
 *
 * Redde: Index ad elementum, vel NIHIL si non inventus
 */
vacuum*
xar_quaerere_binarie (
           constans         Xar* xar,
           constans      vacuum* clavis,
                  XarComparator  comparator);

/* Xar Quaerere Binarie Index - Quaerere binarie, reddere indicem
 * "Quaerere binarie, reddere indicem"
 *
 * Redde: Index elementi, vel -1 si non inventus
 */
s32
xar_quaerere_binarie_index (
           constans         Xar* xar,
           constans      vacuum* clavis,
                  XarComparator  comparator);


/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus - Obtinere numerum elementorum
 * "Obtinere numerum elementorum"
 */
i32
xar_numerus (
        constans Xar* xar);

/* Xar Capacitas - Obtinere capacitatem totalem
 * "Obtinere capacitatem totalem"
 */
i32
xar_capacitas (
        constans Xar* xar);

/* Xar Vacuum Est - Verificare si vacua
 * "Verificare si vacua"
 */
b32
xar_vacuum_est (
        constans Xar* xar);

/* Xar Vacare - Vacuare omnia elementa
 * "Vacuare omnia elementa (segmenta manent allocata)"
 */
vacuum
xar_vacare (
        Xar* xar);

/* Xar Truncare - Truncare ad magnitudinem minorem
 * "Truncare ad magnitudinem minorem"
 */
vacuum
xar_truncare (
        Xar* xar,
        i32  numerus_novus);

/* Xar Copiare Ad Tabulam - Copiare elementa ad tabulam
 * "Copiare elementa ad tabulam"
 *
 * Redde: Numerus elementorum copiatorum
 */
i32
xar_copiare_ad_tabulam (
        constans Xar* xar,
              vacuum* destinatio,
                 i32  initium,
                 i32  numerus);

/* Xar Status Imprimere - Imprimere statisticas pro depuratione
 * "Imprimere statisticas pro depuratione"
 */
vacuum
xar_status_imprimere (
        constans Xar* xar);


/* ========================================================================
 * REMOTIO
 * ======================================================================== */

/* Xar Removere Ultimum - Removere elementum ultimum
 * "Removere ultimum - O(1)"
 *
 * Redde: VERUM si successus, FALSUM si xar vacuum
 */
b32
xar_removere_ultimum (
        Xar* xar);

/* Xar Removere Cum Ultimo - Swap-remove O(1)
 * "Removere elementum ad indicem mutando cum ultimo"
 *
 * Nota: Non servat ordinem! Utile si ordo non refert.
 * Redde: VERUM si successus, FALSUM si index invalidus
 */
b32
xar_removere_cum_ultimo (
        Xar* xar,
        i32  index);

/* Xar Tollere - Pop ultimum elementum (alias removere_ultimum + obtinere)
 * "Tollere ultimum elementum"
 *
 * Copiare ultimum elementum ad destinationem et removere
 * Redde: VERUM si successus, FALSUM si xar vacuum
 */
b32
xar_tollere (
            Xar* xar,
         vacuum* destinatio);


/* ========================================================================
 * ORDINATIO ET MANIPULATIO
 * ======================================================================== */

/* Xar Ordinare - Ordinare in loco
 * "Ordinare tabulam in loco usans comparatorem"
 *
 * Fusio STABILIS (aequalia ordinem insertionis servant), O(n log n).
 * Scriptura temporaria 2n elementorum ex piscina tabulae, post
 * ordinationem reddita (notatio) - ergo comparator ex piscina tabulae
 * NE allocet. Piscina certae magnitudinis sine spatio: insertio
 * stabilis in loco (O(n^2), sine allocatione).
 */
vacuum
xar_ordinare (
                  Xar* xar,
        XarComparator  comparator);

/* Xar Invertere - Invertere ordinem in loco
 * "Invertere ordinem elementorum"
 */
vacuum
xar_invertere (
        Xar* xar);

/* Xar Mutare - Mutare duo elementa
 * "Mutare elementa ad indices"
 *
 * Redde: VERUM si successus, FALSUM si indices invalidi
 */
b32
xar_mutare (
        Xar* xar,
        i32  index_a,
        i32  index_b);

/* Xar Continet - Verificare si elementum exsistit
 * "Verificare si elementum exsistit"
 *
 * Redde: VERUM si inventus, FALSUM si non
 */
b32
xar_continet (
           constans         Xar* xar,
           constans      vacuum* clavis,
                  XarComparator  comparator);

#endif /* XAR_H */
#line 1 "include/matrix.h"
/* matrix.h - Matrices EXACTAE super anulum (anulus.h)
 *
 * Matrix super quemlibet anulum domus: Z, Q, Z[t, t^-1]. Elementa opaca
 * (magnitudo anuli), ordine linearum. Algorithmi sine fractionibus
 * (Bareiss): divisiones exactae tantum, ergo super omni anulo integro
 * cum divisione exacta operantur.
 *
 * CONSTRUCTIO: matrix_pone solum dum vocans matricem aedificat. Omnis
 * operatio matricem NOVAM reddit et argumenta numquam mutat.
 *
 * DEFECTUS: functiones b32 reddunt - FALSUM si dimensiones aut anuli
 * discrepant, aut operatio elementi refutat (polynomium: exponens
 * extra fines). Exitus tunc non tangitur.
 *
 * MEMORIA: eliminatio in officinis internis (piscinae temporariae,
 * alternae); solus effectus in piscina vocantis. Matrices parvae (<=
 * XXV elementa) elementis parvis (anulus->parvum) in piscina vocantis
 * directe.
 *
 * VITA: effectus determinantis, nuclei, multiplicationis semper copia
 * profunda in piscina vocantis - memoriam argumentorum non partiuntur.
 * transposita, adde, subtrahe, pone autem structuras elementorum
 * copiant: effectus elementa argumentorum PARTIRI potest (sicut
 * magnus) et valet dum piscinae argumentorum vivunt.
 *
 * Z[t, t^-1]: eliminatio exponentes intermedios crescere facit, ergo
 * determinans, gradus, nucleus FALSUM reddere possunt etiam ubi
 * effectus bene definitus est (e.g. gradus [t^(2^29), 1; 1, t^(2^29)]
 * - t^(2^30) extra fines), et exponentes magni rari memoriam densam
 * polynomii poscunt (vide polynomium.h).
 *
 * USUS:
 *   Matrix v;
 *   Polynomium delta;
 *   (vacuum)matrix_ex_chorda(&ANULUS_POLYNOMIORUM,
 *       chorda_ex_literis("[t - 1, 1; -t, t - 1]", piscina), piscina,
 *       &v);
 *   si (matrix_determinans(v, piscina, &delta)) ... (t^2 - t + 1)
 *
 * Vide lib/matrix.worklog.md.
 */
/* <aedilis corpus="lib/matrix.c"/> */
#ifndef MATRIX_H
#define MATRIX_H






/* Membra PRIVATA - per functiones legenda */
nomen structura {
     constans Anulus* anulus;
                 i32  lineae;
                 i32  columnae;
                  i8* elementa;
} Matrix;


/* ==================================================
 * Constructio et textus
 * ================================================== */

b32
matrix_nulla (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus);

b32
matrix_identitas (
     constans Anulus* anulus,
                 i32  n,
             Piscina* piscina,
              Matrix* exitus);

/* "[a, b; c, d]": elementa per ',' (textus anuli, spatia circum
 * libera), lineae per ';'; "[]" = 0 x 0. FALSUM si malformatum aut
 * lineae longitudinis inaequalis. */
b32
matrix_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              Matrix* exitus);

/* "[a, b; c, d]"; matrix cum dimensione nulla (r x 0, 0 x c) "[]"
 * scribitur - dimensiones perduntur, lectio 0 x 0 reddit */
chorda
matrix_ad_chordam (
     Matrix  m,
    Piscina* piscina);

constans Anulus*
matrix_anulus (
    Matrix m);

i32
matrix_lineae (
    Matrix m);

i32
matrix_columnae (
    Matrix m);

/* index ad elementum (typus anuli, e.g. constans Magnus*); NIHIL
 * extra fines */
constans vacuum*
matrix_elementum (
    Matrix m,
       i32 linea,
       i32 columna);

/* solum dum vocans matricem aedificat; extra fines nihil agit */
vacuum
matrix_pone (
               Matrix* m,
                  i32  linea,
                  i32  columna,
      constans vacuum* valor);


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
matrix_aequalis (
    Matrix a,
    Matrix b);

b32
matrix_adde (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_subtrahe (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_multiplica (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus);

b32
matrix_transposita (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Eliminatio (Bareiss)
 * ================================================== */

/* determinans matricis quadratae (0 x 0 -> 1); exitus elementum
 * anuli */
b32
matrix_determinans (
     Matrix  m,
    Piscina* piscina,
     vacuum* exitus);

/* gradus (rank) super corpus fractionum anuli */
b32
matrix_gradus (
     Matrix  m,
    Piscina* piscina,
        i32* exitus);

/* nucleus: columnae = basis nuclei super corpus fractionum anuli,
 * elementis IN anulo, per formam Gauss-Jordan sine fractionibus:
 * elementa minores matricis (magnitudo Hadamard finita), sed non
 * reducti (super Z: non primitivi, non basis reticuli). columnae -
 * gradus columnae; 0 si nucleus nullus. */
b32
matrix_nucleus (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Formae normales (anulus Euclideus: divisor_communis,
 * divide_cum_residuo, compara_normam; aliter FALSUM - Z solus hodie).
 * Euclides "cardo normae minimae sursum, ceteri modulo eum":
 * incrementum intermedium modicum (Hermite 40 x 40 ~18 ms; Smith cum
 * U, V 40 x 40 ~46 ms).
 * ================================================== */

/* Hermite (lineae): H = U A, U unimodularis; cardines normales (Z: >
 * 0), elementa supra cardinem reducta (Z: 0 <= x < cardo), lineae
 * nullae in fundo. u NIHIL si certificatum non quaeritur. */
b32
matrix_forma_hermite (
     Matrix  a,
    Piscina* piscina,
     Matrix* h,
     Matrix* u);

/* Smith: D = U A V, D diagonalis, d_i normales (Z: >= 0), d_i |
 * d_(i+1), nulli ultimi; U, V unimodulares (NIHIL si non quaeruntur) */
b32
matrix_forma_smith (
     Matrix  a,
    Piscina* piscina,
     Matrix* d,
     Matrix* u,
     Matrix* v);

/* basis RETICULI nuclei (columnae): omnis vector integer x cum A x = 0
 * combinatio integra columnarum est (contra matrix_nucleus) */
b32
matrix_reticulum_nuclei (
     Matrix  a,
    Piscina* piscina,
     Matrix* exitus);


/* ==================================================
 * Diagnosis
 * ================================================== */

/* maximus usus (octeti) officinarum alternarum in ultima eliminatione;
 * computator sumptus deterministicus (sicut magnus_apex_alternarum) */
memoriae_index
matrix_apex_officinarum (
    vacuum);

#endif /* MATRIX_H */
/* lib/piscina.c: statica per plagulam renominata */
#define _allocare_interna _allocare_interna_piscina
#define _alveus_destruere _alveus_destruere_piscina
#define _alveus_nova _alveus_nova_piscina
#define _catena_alveus_destruere _catena_alveus_destruere_piscina
#define _catena_alveus_vacare _catena_alveus_vacare_piscina
#define _debug_imprimere _debug_imprimere_piscina
#define _exundatio _exundatio_piscina
#define _sedem_computare _sedem_computare_piscina
#define _usum_augere _usum_augere_piscina
#define Alveus Alveus_piscina
#line 1 "lib/piscina.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef PISCINA_DEBUG
#define PISCINA_DEBUG FALSUM /* Muta ad VERUM pro imprimere debugging,
                              * vel -DPISCINA_DEBUG=1 in linea compilandi */
#endif

/* VENENUM (modus probandi, -DPISCINA_VENENUM=1): piscina_reficere
 * octetos liberatos PISCINA_OCTETUS_VENENI implet. Sine eo memoria
 * post refectionem valores veteres servat, et valor qui refectionem
 * superstat (vitium vitae) recte legi videtur - recensio polynomium-II
 * duo talia vitia plantata invenit quae suita ordinaria non videbat.
 * Porta: tools/venenum_probare.sh. */
#ifndef PISCINA_VENENUM
#define PISCINA_VENENUM FALSUM
#endif
#define PISCINA_OCTETUS_VENENI 0xA5


/* ===========================================================
 * Structura Alvei - allocatio singularis
 * =========================================================== */

nomen structura Alveus {
              vacuum* buffer;
      memoriae_index  capacitas;
      memoriae_index  offset;
    structura Alveus* sequens;
} Alveus;


/* ===========================================================
 * Structura Piscinae - regit alveos multiples
 * =========================================================== */

structura Piscina {
            Alveus* primus;
            Alveus* nunc;
    memoriae_index  mensura_alvei_initia;
         character* titulus;
               b32  est_dynamicum;
        memoriae_index  maximus_usus;
    memoriae_index  usus_currens;           /* summa offsetuum, incrementalis */
    memoriae_index  numerus_allocationum;   /* historia, numquam minuitur */
};


/* ===========================================================
 * ADIUTORES INTERNI
 * =========================================================== */

#define MEMORIAE_INDEX_MAXIMUS \
    ((memoriae_index)~(memoriae_index)ZEPHYRUM)

/* Sedes allocationis in alveo: offset ad ordinationem promotus
 * (*ordinatus) et finis allocationis (*necessaria). FALSUM si
 * ordinatio nulla aut summa circumiret (parcum …1Q6158) - vocans ut
 * defectum tractat, nihil mutatum. Probatio per spatium RELIQUUM
 * (a > MAXIMUS - b), numquam per summam: summa insignata tacite
 * circumit et 'capit' falso dicit (offset retro, allocatio proxima
 * vivam superponebat). */
interior b32
_sedem_computare (
    memoriae_index  offset,
    memoriae_index  ordinatio,
    memoriae_index  mensura,
    memoriae_index* ordinatus,
    memoriae_index* necessaria)
{
    memoriae_index promotus;

    si (   ordinatio == ZEPHYRUM
        || ordinatio - I > MEMORIAE_INDEX_MAXIMUS - offset)
    {
        redde FALSUM;
    }
    promotus = offset + (ordinatio - I);
    promotus -= promotus % ordinatio;
    si (mensura > MEMORIAE_INDEX_MAXIMUS - promotus)
    {
        redde FALSUM;
    }
    *ordinatus   = promotus;
    *necessaria  = promotus + mensura;
    redde VERUM;
}

interior vacuum
_debug_imprimere (
    constans character* piscinae_titulum,
    constans character* operatio,
        memoriae_index  mensura)
{
    si (PISCINA_DEBUG)
    {
        imprimere("[PISCINA %s] %s: %lu bytes\n", piscinae_titulum,
                  operatio, (insignatus longus)mensura);
    }
}


/* ===========================================================
 * REGIO ALVEI
 * =========================================================== */

interior Alveus*
_alveus_nova (
    memoriae_index capacitas)
{
    Alveus* alveus = (Alveus*)memoriae_allocare(magnitudo(Alveus));
    si (!alveus) redde NIHIL;

    alveus->buffer = memoriae_allocare(capacitas);
    si (!alveus->buffer)
    {
        liberare(alveus);
        redde NIHIL;
    }

    alveus->capacitas  = capacitas;
    alveus->offset     = ZEPHYRUM;
    alveus->sequens    = NIHIL;

    redde alveus;
}

interior vacuum
_alveus_destruere (
        Alveus* alveus)
{
    si (!alveus) redde;

    si (alveus->buffer) liberare(alveus->buffer);
    liberare(alveus);
}

interior vacuum
_catena_alveus_destruere (
        Alveus* alveus)
{
    dum (alveus)
    {
        Alveus* sequens_temporalis = alveus->sequens;
        _alveus_destruere(alveus);
        alveus = sequens_temporalis;
    }
}

interior vacuum
_catena_alveus_vacare (
        Alveus* alveus)
{
    dum (alveus)
    {
        memset(alveus->buffer, ZEPHYRUM, alveus->capacitas);
        alveus->offset  = ZEPHYRUM;
        alveus          = alveus->sequens;
    }
}


/* ===========================================================
 * ALLOCATIO FUNDAMENTALIS LOGICA
 * =========================================================== */

/* petitio quae circumiret (aut ordinatio nulla): ut defectus -
 * fatalis exit, conari NIHIL; piscina intacta */
interior vacuum*
_exundatio (
    constans Piscina* piscina,
      memoriae_index  mensura,
                 b32  fatalis)
{
    si (fatalis)
    {
        imprimere("ALLOCATIO PISCINAE FRACTA: %s (exundatio: mensura"
                  " %lu aut ordinatio nulla)\n",
                  piscina->titulus ? piscina->titulus : "nemo",
                  (insignatus longus)mensura);
        exire(I);
    }
    redde NIHIL;
}

interior vacuum*
_allocare_interna (
               Piscina* piscina,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
{
        memoriae_index  ordinatus_offset;
        memoriae_index  necessaria;
                vacuum* ptr;

    si (!piscina || mensura == ZEPHYRUM) redde NIHIL;

    si (!_sedem_computare(piscina->nunc->offset, ordinatio, mensura,
        &ordinatus_offset, &necessaria))
    {
        redde _exundatio(piscina, mensura, fatalis);
    }

    /* Si allocatio in alveum nunc non capit, invenire vel generare alveum novum */
    dum (necessaria > piscina->nunc->capacitas)
    {
        si (piscina->nunc->sequens)
        {
            /* Transire ad alveum sequentem */
            piscina->nunc = piscina->nunc->sequens;
            si (!_sedem_computare(piscina->nunc->offset, ordinatio,
                mensura, &ordinatus_offset, &necessaria))
            {
                redde _exundatio(piscina, mensura, fatalis);
            }
        }
        alioquin si (piscina->est_dynamicum)
        {
            Alveus* alveus_novum;

            /* Generare alveum novum */
            memoriae_index capacitas_nova =
                piscina->mensura_alvei_initia
                    > MEMORIAE_INDEX_MAXIMUS / II
                ? piscina->mensura_alvei_initia
                : piscina->mensura_alvei_initia * II;

            /* Petitio maior quam duplum: alveus ad mensuram petitionis
             * (+ basis), BASIS INTACTA. Olim basis ad hanc mensuram
             * ratchetabatur et numquam decrescebat: lib/stml.c alvei
             * 1, 2, 3, 6, 9, 18, 27, 54 MB, ultimo 54 MB VII tenente -
             * XXXIX% otiosum (RP §6, 2026-09-02). */
            si (necessaria > capacitas_nova)
            {
                /* necessaria + basis circumiret: petitio numquam
                 * satisfieri potest - refutatur ANTE malloc (olim
                 * alveus minimus et gyrus sine fine; sub ASan malloc
                 * immanis abortat) */
                si (   necessaria > MEMORIAE_INDEX_MAXIMUS
                                    - piscina->mensura_alvei_initia)
                {
                    redde _exundatio(piscina, mensura, fatalis);
                }
                capacitas_nova = necessaria
                    + piscina->mensura_alvei_initia;
            }

            alveus_novum = _alveus_nova(capacitas_nova);
            si (!alveus_novum)
            {
                si (fatalis)
                {
                    imprimere("CREATIO ALVEI FRACTA: %s\n",
                              piscina->titulus ? piscina->titulus : "nemo");
                    exire(I);
                }
                redde NIHIL;
            }

            piscina->nunc->sequens  = alveus_novum;
            piscina->nunc           = alveus_novum;

            /* alveus novus vacuus: capacitas >= necessaria prior >=
             * ordinatio - I + mensura, ergo sedes semper computatur */
            (vacuum)_sedem_computare(piscina->nunc->offset, ordinatio,
                mensura, &ordinatus_offset, &necessaria);

            _debug_imprimere(
                    piscina->titulus ? piscina->titulus : "nemo",
                    "alveus_novum",
                    capacitas_nova);
        }
        alioquin
        {
            /* Non dynamicum et nulli alvei reliqui */
            si (fatalis)
            {
                imprimere("ALLOCATIO PISCINAE FRACTA: %s (indigentia %lu)\n",
                          piscina->titulus ? piscina->titulus : "nemo",
                          (insignatus longus)necessaria);
                exire(I);
            }
            redde NIHIL;
        }
    }


        /* Allocare ex alveo nunc. Apex INCREMENTALITER (2026-09-02): olim
     * omnes alvei per allocationem percurrebantur (I.II M allocationes
     * x XVII alvei in lib/stml.c = XIII% foliorum profili); summa
     * offsetuum mutatur solum hic (delta), in vacare (nihil) et in
     * reficere (recomputata semel). */
    ptr = (character*)(piscina->nunc->buffer) + ordinatus_offset;
    piscina->usus_currens += necessaria - piscina->nunc->offset;
    piscina->nunc->offset = necessaria;
    si (piscina->usus_currens > piscina->maximus_usus)
    {
        piscina->maximus_usus = piscina->usus_currens;
    }
    piscina->numerus_allocationum += I;

    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "allocare", mensura);

    redde ptr;
}


/* ===========================================================
 * GENERATIO
 * =========================================================== */

Piscina*
piscina_generare_dynamicum (
    constans character* piscinae_titulum,
        memoriae_index  mensura_alvei_initia)
{
    Alveus* alveus_primus;

    Piscina* piscina = (Piscina*)memoriae_allocare(magnitudo(Piscina));
    si (!piscina) redde NIHIL;

    alveus_primus = _alveus_nova(mensura_alvei_initia);
    si (!alveus_primus)
    {
        liberare(piscina);
        redde NIHIL;
    }

    piscina->primus                = alveus_primus;
    piscina->nunc                  = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_alvei_initia;
        piscina->est_dynamicum     = VERUM;
    piscina->maximus_usus          = ZEPHYRUM;
    piscina->usus_currens          = ZEPHYRUM;
    piscina->numerus_allocationum  = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli
            + I);

        si (piscina->titulus)
        {
            strcpy(piscina->titulus, piscinae_titulum);
        }
        alioquin
        {
            piscina->titulus = NIHIL;
        }
    }
    alioquin
    {
        piscina->titulus = NIHIL;
    }

    redde piscina;
}

Piscina*
piscina_generare_certae_magnitudinis (
    constans character* piscinae_titulum,
        memoriae_index  mensura_buffer)
{
    Alveus* alveus_primus;

    Piscina* piscina = (Piscina*)memoriae_allocare(magnitudo(Piscina));
    si (!piscina) redde NIHIL;

    alveus_primus = _alveus_nova(mensura_buffer);
    si (!alveus_primus)
    {
        liberare(piscina);
        redde NIHIL;
    }

    piscina->primus                = alveus_primus;
    piscina->nunc                  = alveus_primus;
    piscina->mensura_alvei_initia  = mensura_buffer;
        piscina->est_dynamicum     = FALSUM;
    piscina->numerus_allocationum  = ZEPHYRUM;
    piscina->maximus_usus          = ZEPHYRUM;
    piscina->usus_currens          = ZEPHYRUM;

    si (piscinae_titulum)
    {
        memoriae_index mensura_tituli = strlen(piscinae_titulum);
        piscina->titulus = (character*)memoriae_allocare(mensura_tituli
            + I);
        si (piscina->titulus)
        {
            strcpy(piscina->titulus, piscinae_titulum);
        }
        alioquin
        {
            piscina->titulus = NIHIL;
        }
    }
    alioquin
    {
        piscina->titulus = NIHIL;
    }

    redde piscina;
}


/* ===========================================================
 * DESTRUCTIO
 * =========================================================== */

vacuum
piscina_destruere (
        Piscina* piscina)
{
    si (!piscina) redde;

    si (piscina->primus) _catena_alveus_destruere(piscina->primus);
    si (piscina->titulus) liberare(piscina->titulus);

    liberare(piscina);
}


/* ===========================================================
 * ALLOCATIO - EXITIUM SI DEFECIT
 * =========================================================== */

vacuum*
piscina_allocare (
           Piscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, VERUM);
}

vacuum*
piscina_allocare_ordinatum (
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, VERUM);
}


/* ===========================================================
 * ALLOCATIO - REDDE NIHIL SI DEFECIT
 * =========================================================== */

vacuum*
piscina_conari_allocare (
           Piscina* piscina,
    memoriae_index  mensura)
{
    redde _allocare_interna(piscina, mensura,
        PISCINA_ORDINATIO_ORDINARIA, FALSUM);
}

vacuum*
piscina_conari_allocare_ordinatum (
           Piscina* piscina,
    memoriae_index  mensura,
    memoriae_index  ordinatio)
{
    redde _allocare_interna(piscina, mensura, ordinatio, FALSUM);
}


/* ==================================================
 * AMPLIATIO (2026-10-21) - vide caput
 * ================================================== */

interior vacuum
_usum_augere (
           Piscina* piscina,
    memoriae_index  incrementum)
{
    piscina->usus_currens += incrementum;
    si (piscina->usus_currens > piscina->maximus_usus)
    {
        piscina->maximus_usus = piscina->usus_currens;
    }
}

vacuum*
piscina_conari_ampliare (
           Piscina* piscina,
            vacuum* datum,
    memoriae_index  mensura_vetus,
    memoriae_index  mensura_nova)
{
            Alveus* alveus;
    memoriae_index  incrementum;
            vacuum* buffer_novum;

    si (   !piscina || !piscina->nunc || !datum
        || mensura_nova < mensura_vetus)
    {
        redde NIHIL;
    }
    alveus       = piscina->nunc;
    incrementum  = mensura_nova - mensura_vetus;

    /* ULTIMA alvei currentis? (aequalitas monstratorum tantum) */
    si ((i8*)datum + mensura_vetus != (i8*)alveus->buffer
        + alveus->offset)
    {
        redde NIHIL;
    }

    /* I. in loco */
    si (incrementum <= alveus->capacitas - alveus->offset)
    {
        alveus->offset += incrementum;
        _usum_augere(piscina, incrementum);
        redde datum;
    }

    /* II. SOLA alvei: nemo alius in eum monstrat, ergo alveus ipse
     * moveri potest. Piscina certae magnitudinis numquam crescit. */
    si (!piscina->est_dynamicum || datum != alveus->buffer)
    {
        redde NIHIL;
    }
    si (PISCINA_VENENUM)
    {
        /* semper movere, vetus veneno implere: spectatio pendens
         * venenum legit, non contenta vetera quae recte viderentur */
        buffer_novum = memoriae_allocare(mensura_nova);
        si (!buffer_novum)
        {
            redde NIHIL;
        }
        memcpy(buffer_novum, alveus->buffer, mensura_vetus);
        memset(alveus->buffer, PISCINA_OCTETUS_VENENI,
            alveus->capacitas);
        liberare(alveus->buffer);
    }
    alioquin
    {
        buffer_novum = realloc(alveus->buffer, mensura_nova);
        si (!buffer_novum)
        {
            redde NIHIL;
        }
    }
    alveus->buffer     = buffer_novum;
    alveus->capacitas  = mensura_nova;
    alveus->offset     = mensura_nova;
    _usum_augere(piscina, incrementum);
    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "ampliare (alveus motus)", mensura_nova);
    redde buffer_novum;
}


/* ===========================================================
 * CYCLUS VITAE
 * =========================================================== */

vacuum
piscina_vacare (
        Piscina* piscina)
{
        si (!piscina) redde;
    _catena_alveus_vacare(piscina->primus);
    piscina->nunc          = piscina->primus;
    piscina->usus_currens  = ZEPHYRUM;
    _debug_imprimere(piscina->titulus ? piscina->titulus : "nemo",
        "vacare", ZEPHYRUM);
}


/* ===========================================================
 * QUAESTIO
 * =========================================================== */

memoriae_index
piscina_summa_usus (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  summa;

    si (!piscina) redde ZEPHYRUM;

    summa = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        summa += b->offset;
    }
    redde summa;
}

memoriae_index
piscina_summa_inutilis_allocatus (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  reliqua;

    si (!piscina) redde ZEPHYRUM;

    reliqua = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        reliqua += (b->capacitas - b->offset);
    }
    redde reliqua;
}

memoriae_index
piscina_reliqua_antequam_cresca_alvei (
        constans Piscina* piscina)
{
    si (!piscina || !piscina->nunc) redde ZEPHYRUM;
    redde piscina->nunc->capacitas - piscina->nunc->offset;
}

memoriae_index
piscina_summa_apex_usus (
        constans Piscina* piscina)
{
    redde piscina ? piscina->maximus_usus : ZEPHYRUM;
}

memoriae_index
piscina_numerus_alveorum (
        constans Piscina* piscina)
{
    constans Alveus* b;
     memoriae_index  numerus;

    si (!piscina) redde ZEPHYRUM;

    numerus = ZEPHYRUM;
    per (b = piscina->primus; b; b = b->sequens)
    {
        numerus += I;
    }
    redde numerus;
}

memoriae_index
piscina_numerus_allocationum (
        constans Piscina* piscina)
{
    redde piscina ? piscina->numerus_allocationum : ZEPHYRUM;
}


/* ===========================================================
 * NOTATIO - MARK/RESET PATTERN
 * =========================================================== */

PiscinaNotatio
piscina_notare (
        Piscina* piscina)
{
    PiscinaNotatio notatio;

    si (!piscina)
    {
        notatio.alveus_nunc  = NIHIL;
        notatio.positus      = ZEPHYRUM;
        redde notatio;
    }

    notatio.alveus_nunc  = piscina->nunc;
    notatio.positus      = piscina->nunc->offset;

    _debug_imprimere(
            piscina->titulus ? piscina->titulus : "nemo",
            "notare",
            notatio.positus);

    redde notatio;
}

vacuum
piscina_reficere (
               Piscina* piscina,
        PiscinaNotatio  notatio)
{
    Alveus* alveus_notatus;
    Alveus* alveus_iter;

    si (!piscina || !notatio.alveus_nunc) redde;

    alveus_notatus = (Alveus*)notatio.alveus_nunc;

    si (PISCINA_VENENUM)
    {
        /* octeti liberati: pars alvei notati post positum, et alvei
         * sequentes toti (usque ad offset suum) */
        si (alveus_notatus->offset > notatio.positus)
        {
            memset((i8*)alveus_notatus->buffer + notatio.positus,
                PISCINA_OCTETUS_VENENI,
                alveus_notatus->offset - notatio.positus);
        }
        per (alveus_iter = alveus_notatus->sequens; alveus_iter;
             alveus_iter = alveus_iter->sequens)
        {
            memset(alveus_iter->buffer, PISCINA_OCTETUS_VENENI,
                alveus_iter->offset);
        }
    }

    /* Reficere alveum notatum ad positionem notatam */
    alveus_notatus->offset = notatio.positus;

    /* Vacare omnes alveos post alveum notatum */
    per (alveus_iter =
        alveus_notatus->sequens; alveus_iter; alveus_iter =
        alveus_iter->sequens)
    {
        alveus_iter->offset = ZEPHYRUM;
    }

        /* Reficere piscina->nunc ad alveum notatum; summa offsetuum
     * recomputata SEMEL (alvei ante notatum offsetus servant) */
    piscina->nunc          = alveus_notatus;
    piscina->usus_currens  = ZEPHYRUM;
    per (alveus_iter = piscina->primus; alveus_iter;
         alveus_iter = alveus_iter->sequens)
    {
        piscina->usus_currens += alveus_iter->offset;
    }

    _debug_imprimere(
            piscina->titulus ? piscina->titulus : "nemo",
            "reficere",
            notatio.positus);
}

b32
piscina_potesne_allocare (
        constans Piscina* piscina,
          memoriae_index  mensura)
{
    memoriae_index reliqua;

    si (!piscina || !piscina->nunc) redde FALSUM;

    reliqua = piscina->nunc->capacitas - piscina->nunc->offset;

    /* Pro piscinis dynamicis, semper possibile (crescet) */
    si (piscina->est_dynamicum) redde VERUM;

    /* Pro piscinis certae magnitudinis, verificare spatium */
    redde mensura <= reliqua ? VERUM : FALSUM;
}
#undef _allocare_interna
#undef _alveus_destruere
#undef _alveus_nova
#undef _catena_alveus_destruere
#undef _catena_alveus_vacare
#undef _debug_imprimere
#undef _exundatio
#undef _sedem_computare
#undef _usum_augere
#undef Alveus
#undef MEMORIAE_INDEX_MAXIMUS
#undef PISCINA_DEBUG
#undef PISCINA_OCTETUS_VENENI
#undef PISCINA_VENENUM
/* lib/sors.c: statica per plagulam renominata */
#define _pcg_initiare _pcg_initiare_sors
#define _splitmix_proximum _splitmix_proximum_sors
#line 1 "lib/sors.c"
/* sors.c - Numeri pseudo-fortuiti seminabiles (PCG32 + SplitMix64)
 *
 * PCG32 (O'Neill): status LXIV bits, effusio XXXII bits per
 * 'xorshift + rotatio'. SplitMix64 (Vigna) semen usoris in statum
 * initialem bene mixtum vertit. Vide lib/sors.worklog.md.
 */


#define SORS_MULTIPLICATOR  6364136223846793005ULL
#define SPLITMIX_GAMMA      0x9E3779B97F4A7C15ULL
#define SPLITMIX_MISCENS_I   0xBF58476D1CE4E5B9ULL
#define SPLITMIX_MISCENS_II  0x94D049BB133111EBULL

/* SplitMix64: statum provehit, valorem mixtum reddit */
interior i64
_splitmix_proximum (
    i64* x)
{
    i64 z;

    *x  = *x + SPLITMIX_GAMMA;
    z   = *x;
    z   = (z ^ (z >> XXX)) * SPLITMIX_MISCENS_I;
    z   = (z ^ (z >> XXVII)) * SPLITMIX_MISCENS_II;
    redde z ^ (z >> XXXI);
}

/* PCG32 initium canonicum (pcg32_srandom_r): status nullus,
 * incrementum impar ex serie, gradus, status += initium, gradus */
interior vacuum
_pcg_initiare (
    Sors* s,
     i64  initium,
     i64  series)
{
    s->status       = 0ULL;
    s->incrementum  = (series << I) | 1ULL;
    (vacuum)sors_proximum(s);
    s->status = s->status + initium;
    (vacuum)sors_proximum(s);
}

vacuum
sors_seminare (
    Sors* s,
     i64  semen,
     i64  series)
{
    i64 x = semen;

    _pcg_initiare(s, _splitmix_proximum(&x), series);
}

Sors
sors_derivare (
    constans Sors* s,
              i64  clavis)
{
    Sors filius;
     i64 y = s->status + clavis * SPLITMIX_GAMMA;

    sors_seminare(&filius, _splitmix_proximum(&y) ^ s->incrementum,
        clavis);
    redde filius;
}

i32
sors_proximum (
    Sors* s)
{
    i64 vetus = s->status;
    i32 mixtum;
    i32 rotatio;

    s->status  = vetus * SORS_MULTIPLICATOR + s->incrementum;
    mixtum     = (i32)(((vetus >> XVIII) ^ vetus) >> XXVII);
    rotatio    = (i32)(vetus >> LIX);
    redde (mixtum >> rotatio) | (mixtum << ((0U - rotatio) & XXXI));
}

i32
sors_intra (
    Sors* s,
     i32  limes)
{
    i64 productum;
    i32 inferior;

    si (limes == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    productum  = (i64)sors_proximum(s) * (i64)limes;
    inferior   = (i32)productum;
    si (inferior < limes)
    {
        i32 limen = (0U - limes) % limes;

        dum (inferior < limen)
        {
            productum  = (i64)sors_proximum(s) * (i64)limes;
            inferior   = (i32)productum;
        }
    }
    redde (i32)(productum >> XXXII);
}

s32
sors_inter (
    Sors* s,
     s32  imum,
     s32  summum)
{
    i64 spatium;

    si (imum >= summum)
    {
        redde imum;
    }
    spatium = (i64)((s64)summum - (s64)imum) + 1ULL;
    si (spatium > 0xFFFFFFFFULL)
    {
        redde (s32)sors_proximum(s);
    }
    redde (s32)((s64)imum + (s64)sors_intra(s, (i32)spatium));
}

f64
sors_f64 (
    Sors* s)
{
    i32 superior = sors_proximum(s) >> V;
    i32 inferior = sors_proximum(s) >> VI;

    redde ((f64)superior * 67108864.0 + (f64)inferior)
        / 9007199254740992.0;
}

f32
sors_f32 (
    Sors* s)
{
    redde (f32)(sors_proximum(s) >> VIII) * (1.0f / 16777216.0f);
}

b32
sors_casu (
    Sors* s,
     i32  numerator,
     i32  denominator)
{
    si (denominator == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde sors_intra(s, denominator) < numerator;
}

vacuum
sors_miscere (
      Sors* s,
    vacuum* elementa,
       i32  numerus,
       i32  magnitudo_elementi)
{
     i8* octeti = (i8*)elementa;
    i32  i;

    si (elementa == NIHIL || numerus < II)
    {
        redde;
    }
    per (i = numerus - I; i > ZEPHYRUM; i--)
    {
        i32 j = sors_intra(s, i + I);
        i32 k;

        si (j == i)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < magnitudo_elementi; k++)
        {
            i8 transitus = octeti[i * magnitudo_elementi + k];

            octeti[i * magnitudo_elementi + k]  =
                octeti[j * magnitudo_elementi + k];
            octeti[j * magnitudo_elementi + k]  = transitus;
        }
    }
}

i32
sors_lcg_classicus (
    i32* status)
{
    *status = *status * 1103515245U + 12345U;
    redde *status;
}
#undef _pcg_initiare
#undef _splitmix_proximum
#undef SORS_MULTIPLICATOR
#undef SPLITMIX_GAMMA
#undef SPLITMIX_MISCENS_I
#undef SPLITMIX_MISCENS_II
/* lib/chorda.c: statica per plagulam renominata */
#define _chorda_nullum_habet _chorda_nullum_habet_chorda
#define _extrahere_verba _extrahere_verba_chorda
#line 1 "lib/chorda.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */




#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define CHORDA_FRIATUM_OFFSET  2166136261U
#define CHORDA_FRIATUM_PRIMUS    16777619U


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
    constans character* litterae,
               Piscina* piscina)
{
    chorda  fructus;
       i32  mensura;
        i8* allocatus;

    si (!litterae || !piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

      mensura = (i32)strlen(litterae);
    allocatus = (i8*)piscina_allocare(piscina, mensura);

    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, litterae, mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_ex_buffer (
     i8* buffer,
    i32  mensura)
{
    chorda fructus;

    si (!buffer)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = mensura;
    fructus.datum    = buffer;

    redde fructus;
}

chorda
chorda_sectio (
    chorda s,
       i32 initium,
       i32 finis)
{
    chorda fructus;

    si (!s.datum || initium > finis || finis > s.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = finis - initium;
    fructus.datum    = s.datum + initium;

    redde fructus;
}

chorda
chorda_transcribere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, s.datum, s.mensura);

    fructus.mensura  = s.mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_concatenare (
     chorda  a,
     chorda  b,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  mensura_totalis;

    si (!piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Si a vacuus est, redde transcriptionem b */
    si (a.mensura == ZEPHYRUM || !a.datum)
    {
        redde chorda_transcribere(b, piscina);
    }

    /* Si b vacuus est, redde transcriptionem a */
    si (b.mensura == ZEPHYRUM || !b.datum)
    {
        redde chorda_transcribere(a, piscina);
    }

    mensura_totalis  = a.mensura + b.mensura;
    allocatus        = (i8*)piscina_allocare(piscina, mensura_totalis);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    memcpy(allocatus,             a.datum, (memoriae_index)a.mensura);
    memcpy(allocatus + a.mensura, b.datum, (memoriae_index)b.mensura);

    fructus.mensura  = mensura_totalis;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_praecidi_laterale (
     chorda  s,
    Piscina* piscina)
{
    chorda fructus;
       i32 initium;
       i32 finis;
       i32 i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* initium = sententia "non inventum" (s.mensura): chorda tota
	 * spatia -> initium >= finis -> vacua redditur (ante 2026-07-17
	 * initium ZEPHYRUM manebat ET circulus retro insignatus
	 * volvebatur - circulus infinitus cum lectionibus feris) */
    initium  = s.mensura;
    finis    = s.mensura;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (!isspace((i8)s.datum[i]))
        {
            initium = i;
            frange;
        }
    }

    /* idioma numerus-deorsum: i insignatus numquam sub zephyrum */
    per (i = s.mensura; i > ZEPHYRUM; i--)
    {
        si (!isspace((i8)s.datum[i - I]))
        {
            finis = i;
            frange;
        }
    }

    si (initium >= finis)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    redde chorda_sectio(s, initium, finis);
}


/* ==================================================
 * Divisio
 * ================================================== */

chorda_fissio_fructus
chorda_fissio (
       chorda  s,
    character  delim,
      Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* elementa;
                      i32  capacitas;
                      i32  numerus;
                      i32  initium;
                      i32  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.elementa  = NIHIL;
        fructus.numerus   = ZEPHYRUM;
        redde fructus;
    }

    capacitas = XVI;
    elementa = (chorda*)piscina_allocare(piscina,
        capacitas * magnitudo(chorda));
    si (!elementa)
    {
        fructus.elementa  = NIHIL;
        fructus.numerus   = ZEPHYRUM;
        redde fructus;
    }

    numerus = ZEPHYRUM;
    initium = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        b32 est_delim = (i < s.mensura && s.datum[i] == delim);
        b32 est_finis = (i == s.mensura);

        si (est_delim || est_finis)
        {
            si (numerus >= capacitas)
            {
                 chorda* elementa_nova;
                    i32  j;

                capacitas *= II;
                elementa_nova = (chorda*)piscina_allocare(piscina,
                    capacitas * magnitudo(chorda));
                si (!elementa_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                /* Transcribere elementa veteres ad array novum */
                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    elementa_nova[j] = elementa[j];
                }

                elementa = elementa_nova;
            }

            elementa[numerus] = chorda_sectio(s, initium, i);
            numerus++;
            initium = i + I;
        }
    }

    fructus.elementa  = elementa;
    fructus.numerus   = numerus;
    redde fructus;
}


/* ==================================================
 * Comparatio
 * ================================================== */

b32
chorda_aequalis (
    chorda a,
    chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    si (a.datum == b.datum)
    {
        redde VERUM;
    }

    redde memcmp(a.datum, b.datum, a.mensura) == ZEPHYRUM;
}

b32
chorda_aequalis_literis (
                chorda  s,
    constans character* cstr)
{
    i32 len;
    i32 i;

    si (!cstr)
    {
        redde FALSUM;
    }

    /* Mensura literarum */
    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    si (s.mensura != len)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)s.datum[i] != cstr[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

b32
chorda_aequalis_case_insensitivus (
    chorda a,
    chorda b)
{
    memoriae_index i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        si (tolower((character)a.datum[i])
            != tolower((character)b.datum[i]))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

s32
chorda_comparare (
    chorda a,
    chorda b)
{
    memoriae_index minima_mensura;
               s32 cmp_result;

    minima_mensura  = a.mensura < b.mensura ? a.mensura : b.mensura;
    cmp_result      = memcmp(a.datum, b.datum, minima_mensura);

    si (cmp_result != ZEPHYRUM)
    {
        redde cmp_result;
    }

    si (a.mensura < b.mensura) redde -I;
    si (a.mensura > b.mensura) redde I;

    redde ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

b32
chorda_continet (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde FALSUM;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
chorda_incipit (
    chorda s,
    chorda prefixum)
{
    si (!s.datum || !prefixum.datum || prefixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (prefixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    redde memcmp(s.datum, prefixum.datum, prefixum.mensura) == ZEPHYRUM;
}

b32
chorda_terminatur (
    chorda s,
    chorda suffixum)
{
    memoriae_index offset;

    si (!s.datum || !suffixum.datum || suffixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (suffixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    offset = s.mensura - suffixum.mensura;
    redde memcmp(s.datum + offset, suffixum.datum, suffixum.mensura)
        == ZEPHYRUM;
}

chorda
chorda_invenire (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;
            chorda fructus;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde fenum;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura)
            == ZEPHYRUM)
        {
            fructus.mensura  = acus.mensura;
            fructus.datum    = fenum.datum + i;
            redde fructus;
        }
    }

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    redde fructus;
}

s32
chorda_invenire_index (
    chorda fenum,
    chorda acus)
{
    i32 i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }

    redde -I;
}

chorda
chorda_invenire_ultimum (
    chorda fenum,
    chorda acus)
{
    chorda fructus;
       s32 i;
       s32 max_i;
       s32 ultima_positio;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde fenum;
    }

    ultima_positio  = -I;
    max_i           = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            ultima_positio = i;
            frange;
        }
    }

    si (ultima_positio < ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    fructus.mensura  = acus.mensura;
    fructus.datum    = fenum.datum + ultima_positio;
    redde fructus;
}

s32
chorda_invenire_ultimum_index (
    chorda fenum,
    chorda acus)
{
    s32 i;
    s32 max_i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde (s32)fenum.mensura;
    }

    max_i = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }

    redde -I;
}

i32
chorda_numerare_occurrentia (
    chorda fenum,
    chorda acus)
{
    i32 count;
    i32 positus;

    si (   !fenum.datum || !acus.datum || acus.mensura > fenum.mensura
        || acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    count    = ZEPHYRUM;
    positus  = ZEPHYRUM;

    dum (positus <= fenum.mensura - acus.mensura)
    {
        si (memcmp(fenum.datum + positus, acus.datum,
            (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            count++;
            positus += acus.mensura;
        }
        alioquin
        {
            positus++;
        }
    }

    redde count;
}


/* ==================================================
 * Manipulatio
 * ================================================== */

chorda
chorda_praecidere (
    chorda s)
{
    i32 initium;
    i32 finis;
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium  = ZEPHYRUM;
    finis    = s.mensura;

    /* Praecidere initium */
    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    /* Praecidere finis */
    dum (finis > initium && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, initium, finis);
}

chorda
chorda_minuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = (character)tolower((character)s.datum[i]);
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = (i8*)(void*)allocatus;

    redde fructus;
}

chorda
chorda_maiuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < (memoriae_index)s.mensura; i++)
    {
        allocatus[i] = (character)toupper((character)s.datum[i]);
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = (i8*)(void*)allocatus;

    redde fructus;
}

chorda
chorda_praecidere_sinistram (
    chorda s)
{
    i32 initium;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium = ZEPHYRUM;

    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    redde chorda_sectio(s, initium, s.mensura);
}

chorda
chorda_praecidere_dextram (
    chorda s)
{
    i32 finis;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    finis = s.mensura;

    dum (finis > ZEPHYRUM && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, ZEPHYRUM, finis);
}

chorda
chorda_substituere (
      chorda  s,
      chorda  antiquum,
      chorda  novum,
     Piscina* piscina)
{
    chorda  fructus;
       i32  numerus_occurrentia;
       i32  mensura_nova;
        i8* allocatus;
       i32  positus_lecti;
       i32  positus_scripti;
       i32  i;

    si (!piscina)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Si antiquum vacuus, redde copiam originalem */
    si (!antiquum.datum || antiquum.mensura == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Si s vacuus, redde vacuus */
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Numerare occurrentias */
    numerus_occurrentia = chorda_numerare_occurrentia(s, antiquum);

    si (numerus_occurrentia == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Calculare mensuram novam */
    mensura_nova = s.mensura + (numerus_occurrentia * (novum.mensura
        - antiquum.mensura));

    si (mensura_nova <= ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus_lecti    = ZEPHYRUM;
    positus_scripti  = ZEPHYRUM;

    dum (positus_lecti < s.mensura)
    {
        /* Verificare si hic est antiquum */
        si (   positus_lecti <= s.mensura - antiquum.mensura
            && memcmp(s.datum + positus_lecti, antiquum.datum,
            (memoriae_index)antiquum.mensura) == ZEPHYRUM)
        {
            /* Scribere novum */
            per (i = ZEPHYRUM; i < novum.mensura; i++)
            {
                allocatus[positus_scripti++] = novum.datum[i];
            }
            positus_lecti += antiquum.mensura;
        }
        alioquin
        {
            allocatus[positus_scripti++] = s.datum[positus_lecti++];
        }
    }

    fructus.mensura  = mensura_nova;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_invertere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = s.datum[s.mensura - I - i];
    }

    fructus.mensura  = s.mensura;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_duplicare (
     chorda  s,
        i32  numerus,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  mensura_nova;
       i32  i;

    si (!piscina || numerus <= ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    mensura_nova = s.mensura * numerus;

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(allocatus + (i * s.mensura), s.datum,
            (memoriae_index)s.mensura);
    }

    fructus.mensura  = mensura_nova;
    fructus.datum    = allocatus;
    redde fructus;
}


/* ==================================================
 * Convenientia
 * ================================================== */

character*
chorda_ut_cstr (
     chorda  s,
    Piscina* piscina)
{
    character* allocatus;

    si (!s.datum)
    {
        redde NIHIL;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura + I);
    si (!allocatus)
    {
        redde NIHIL;
    }

    memcpy(allocatus, s.datum, s.mensura);
    allocatus[s.mensura] = '\0';

    redde allocatus;
}

/* NULLUM insertum? Conversiones numericae per cstr transeunt, ubi
 * NULLUM tacite truncaret ('1\0garbage' -> 1, quia *terminus == '\0'
 * VERUM est AD nullum insertum). Custodia UNA in limine. */
interior b32
_chorda_nullum_habet (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
chorda_ut_s32 (
    chorda  s,
       s32* fructus)
{
          character* cstr_temporalis;
          character* terminus;
    signatus longus  longus_valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    longus_valor = strtol(cstr_temporalis, &terminus, X);

    si (   terminus     != cstr_temporalis && *terminus == '\0'
        && longus_valor <= 2147483647L
        && longus_valor >= -2147483647L - 1L)
    {
        *fructus = (s32)longus_valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}

b32
chorda_ut_i32 (
    chorda  s,
       i32* fructus)
{
            character* cstr_temporalis;
            character* terminus;
    insignatus longus  valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s) || s.datum[ZEPHYRUM] == '-')
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtoul(cstr_temporalis, &terminus, X);


    /* Confer si tota chorda parata est */
    si (   terminus != cstr_temporalis && *terminus == '\0'
        && valor    <= 4294967295UL)
    {
        *fructus = (i32)valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}


/* ==================================================
 * FRIATIO
 * ================================================== */

i32
chorda_friare (
    chorda s)
{
    /* Algoritmus FNV-1a friationis */
    i32 friatum;
    i32 i;

    friatum = CHORDA_FRIATUM_OFFSET;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        friatum ^= (i8)s.datum[i];
        friatum *= CHORDA_FRIATUM_PRIMUS;
    }

    redde friatum;
}


/* ==================================================
 * Conversio Casus
 * ================================================== */

/* Interior: Extrahere verba ex chorda
 * Regulae:
 * - Characteres non-alphanumerici sunt delimitatores (omittuntur)
 * - Transitio minuscula→maiuscula incipit verbum novum
 * - Littera post digitum incipit verbum novum
 * - Maiusculae consecutivae ante minusculam: scinde ante ultimam maiusculam
 */
interior chorda_fissio_fructus
_extrahere_verba (
       chorda  s,
      Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* verba;
                      i32  capacitas;
                      i32  numerus;
                      i32  i;
                      s32  initium_verbi;
                      b32  in_verbo;
                character  c_currens;
                character  c_praecedans;

    fructus.elementa  = NIHIL;
    fructus.numerus   = ZEPHYRUM;

    si (!s.datum || s.mensura == ZEPHYRUM || !piscina)
    {
        redde fructus;
    }

    capacitas = XVI;
    verba = (chorda*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(chorda));
    si (!verba)
    {
        redde fructus;
    }

    numerus        = ZEPHYRUM;
    initium_verbi  = -I;
    in_verbo       = FALSUM;
    c_praecedans   = '\0';

    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        b32 est_finis        = (i == s.mensura);
        b32 est_alpha        = FALSUM;
        b32 est_digitus      = FALSUM;
        b32 est_maiuscula    = FALSUM;
        b32 praec_minuscula  = FALSUM;
        b32 praec_digitus    = FALSUM;
        b32 praec_maiuscula  = FALSUM;
        b32 debet_scindere   = FALSUM;

        si (!est_finis)
        {
            c_currens      = (character)s.datum[i];
            est_alpha      = isalpha((integer)c_currens) != ZEPHYRUM;
            est_digitus    = isdigit((integer)c_currens) != ZEPHYRUM;
            est_maiuscula  = isupper((integer)c_currens) != ZEPHYRUM;
        }

        si (c_praecedans != '\0')
        {
            praec_minuscula = islower((integer)c_praecedans)
                != ZEPHYRUM;
            praec_digitus = isdigit((integer)c_praecedans) != ZEPHYRUM;
            praec_maiuscula = isupper((integer)c_praecedans)
                != ZEPHYRUM;
        }

        /* Determinare si debemus scindere */
        si (in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            /* Minuscula ad maiusculam: scinde */
            si (praec_minuscula && est_maiuscula)
            {
                debet_scindere = VERUM;
            }
            /* Digitus ad litteram: scinde */
            alioquin si (praec_digitus && est_alpha)
            {
                debet_scindere = VERUM;
            }
            /* Maiuscula ante maiusculam, sed proximus est minuscula: scinde ante currens */
            /* Exemplum: "XMLParser" - quando ad 'P' venimus post 'L', scinde */
            alioquin si (   praec_maiuscula && est_maiuscula
                         && i + I < s.mensura)
            {
                character c_proximus = (character)s.datum[i + I];
                si (islower((integer)c_proximus))
                {
                    debet_scindere = VERUM;
                }
            }
        }

        /* Finis verbi */
        si (   in_verbo
            && (est_finis || (!est_alpha && !est_digitus)
            || debet_scindere))
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                 chorda* verba_nova;
                    i32  j;

                capacitas *= II;
                verba_nova = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)capacitas * magnitudo(chorda));
                si (!verba_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    verba_nova[j] = verba[j];
                }
                verba = verba_nova;
            }

            verba[numerus] = chorda_sectio(s, (i32)initium_verbi, i);
            numerus++;

            si (debet_scindere)
            {
                /* Initium novi verbi est currens character */
                initium_verbi  = (s32)i;
                in_verbo       = VERUM;
            }
            alioquin
            {
                in_verbo       = FALSUM;
                initium_verbi  = -I;
            }
        }

        /* Initium verbi novi */
        si (!in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            initium_verbi  = (s32)i;
            in_verbo       = VERUM;
        }

        si (!est_finis)
        {
            c_praecedans = c_currens;
        }
    }

    fructus.elementa  = verba;
    fructus.numerus   = numerus;
    redde fructus;
}

chorda
chorda_pascalis (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Allocare maximum possibile (originalis mensura) */
    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_camelus (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (i == ZEPHYRUM)
            {
                /* Primum verbum totum minusculum */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
            alioquin si (j == ZEPHYRUM)
            {
                /* Prima littera verborum subsequentium maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] =
                (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_kebab (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '-' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '-';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] =
                (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}

chorda
chorda_pascalis_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                       i8* allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura  = positus;
    fructus.datum    = allocatus;
    redde fructus;
}


/* ==================================================
 * Novae Functiones Utilitatis
 * ================================================== */

b32
chorda_ut_f64 (
    chorda  s,
       f64* fructus)
{
    character* cstr_temporalis;
    character* terminus;
          f64  valor;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }

    cstr_temporalis =
        (character*)memoriae_allocare((memoriae_index)s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, (memoriae_index)s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtod(cstr_temporalis, &terminus);

    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *fructus = valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}

chorda
chorda_ex_s32 (
        s32  numerus,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina)
    {
        redde fructus;
    }

    mensura_signed = snprintf(buffer, magnitudo(buffer), "%d", numerus);
    si (mensura_signed < ZEPHYRUM)
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

b32
chorda_ut_s64 (
     chorda  s,
        s64* fructus)
{
    constans character* limes;
                   s64  valor    = ZEPHYRUM;
                   i32  initium  = ZEPHYRUM;
                   i32  cifrae;
                   i32  k;
                   b32  negans  = FALSUM;

    si (   !s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }
    si (s.datum[ZEPHYRUM] == '+' || s.datum[ZEPHYRUM] == '-')
    {
        negans   = (s.datum[ZEPHYRUM] == '-') ? VERUM : FALSUM;
        initium  = I;
    }
    si (initium >= s.mensura)
    {
        redde FALSUM;
    }
    per (k = initium; k < s.mensura; k++)
    {
        si (s.datum[k] < '0' || s.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    /* zephyra ducentia sine pondere (unum servatur) */
    dum (initium < s.mensura - I && s.datum[initium] == '0')
    {
        initium++;
    }
    cifrae = s.mensura - initium;
    si (cifrae > XIX)
    {
        redde FALSUM;
    }
    /* limes LEXICE confertur: arithmetica ipsa circumvolveret */
    limes = negans ? "9223372036854775808" : "9223372036854775807";
    si (cifrae == XIX)
    {
        per (k = ZEPHYRUM; k < XIX; k++)
        {
            si (s.datum[initium + k] > (i8)limes[k])
            {
                redde FALSUM;
            }
            si (s.datum[initium + k] < (i8)limes[k])
            {
                frange;
            }
        }
    }
    /* negativa NEGATIVE accumulantur: -9223372036854775808 positivum
     * esse non potest */
    per (k = initium; k < s.mensura; k++)
    {
        s64 cifra = (s64)(s.datum[k] - '0');

        valor = negans ? valor * X - cifra : valor * X + cifra;
    }
    *fructus = valor;
    redde VERUM;
}

chorda
chorda_ex_s64 (
            s64  numerus,
        Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XXIV];
    character  inversa[XXIV];
          i32  mensura = ZEPHYRUM;
          i32  k;
          i64  magnitudo_numeri;
          b32  negans;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    si (numerus < ZEPHYRUM)
    {
        negans            = VERUM;
        /* -(numerus + I) + I: extremum negativum negari non potest */
        magnitudo_numeri  = (i64)(-(numerus + I)) + I;
    }
    alioquin
    {
        negans            = FALSUM;
        magnitudo_numeri  = (i64)numerus;
    }
    fac
    {
        buffer[mensura] = (character)('0'
            + (integer)(magnitudo_numeri % X));
        mensura++;
        magnitudo_numeri = magnitudo_numeri / X;
    }
    dum (magnitudo_numeri != ZEPHYRUM);
    si (negans)
    {
        buffer[mensura] = '-';
        mensura++;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        inversa[k] = buffer[mensura - I - k];
    }
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, inversa, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}

chorda
chorda_ex_f64_exacta (
            f64  numerus,
        Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XLVIII];
          s32  mensura_signed;
          i32  mensura;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    mensura_signed = snprintf(buffer, magnitudo(buffer), "%.17g",
                              numerus);
    si (   mensura_signed < ZEPHYRUM
        || mensura_signed >= (s32)magnitudo(buffer))
    {
        redde fructus;
    }
    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}

chorda
chorda_ex_f64 (
        f64  numerus,
        i32  praecisio,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
      character  formatalis[XVI];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina || praecisio < ZEPHYRUM || praecisio > L)
    {
        redde fructus;
    }

    snprintf(formatalis, magnitudo(formatalis), "%%.%df", praecisio);
    mensura_signed = snprintf(buffer, magnitudo(buffer), formatalis,
        numerus);
    /* snprintf longitudinem VERAM reddit, non scriptam: sine hac
     * custodia memcpy infra trans buffer legebat (1e300 sub '%.6f'
     * CCCVII octetos poscit, buffer CXXXII fert) */
    si (   mensura_signed < ZEPHYRUM
        || mensura_signed >= (s32)magnitudo(buffer))
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura  = mensura;
    fructus.datum    = allocatus;

    redde fructus;
}

chorda
chorda_character_ad (
     chorda  s,
        i32  index,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;

    si (!piscina || !s.datum || index < ZEPHYRUM || index >= s.mensura)
    {
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, I);
    si (!allocatus)
    {
        redde fructus;
    }

    allocatus[ZEPHYRUM]  = s.datum[index];
    fructus.mensura      = I;
    fructus.datum        = allocatus;

    redde fructus;
}

b32
chorda_vacua (
    chorda s)
{
    redde s.datum == NIHIL || s.mensura == ZEPHYRUM;
}

chorda_fissio_fructus
chorda_fissio_chorda (
     chorda  s,
     chorda  delim,
    Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* elementa;
                      i32  capacitas;
                      i32  numerus;
                      i32  initium;
                      i32  i;

    fructus.elementa  = NIHIL;
    fructus.numerus   = ZEPHYRUM;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        redde fructus;
    }

    /* Si delimitator vacuus, redde totam chordam ut unum elementum */
    si (!delim.datum || delim.mensura == ZEPHYRUM)
    {
        elementa = (chorda*)piscina_allocare(piscina,
            magnitudo(chorda));
        si (!elementa)
        {
            redde fructus;
        }
        elementa[ZEPHYRUM]  = s;
        fructus.elementa    = elementa;
        fructus.numerus     = I;
        redde fructus;
    }

    capacitas = XVI;
    elementa = (chorda*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(chorda));
    si (!elementa)
    {
        redde fructus;
    }

    numerus  = ZEPHYRUM;
    initium  = ZEPHYRUM;
    i        = ZEPHYRUM;

    dum (i <= s.mensura - delim.mensura)
    {
        /* Verificare si hic est delimitator */
        si (memcmp(s.datum + i, delim.datum,
            (memoriae_index)delim.mensura) == ZEPHYRUM)
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                 chorda* elementa_nova;
                    i32  j;

                capacitas *= II;
                elementa_nova = (chorda*)piscina_allocare(piscina,
                    (memoriae_index)capacitas * magnitudo(chorda));
                si (!elementa_nova)
                {
                    fructus.elementa  = NIHIL;
                    fructus.numerus   = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    elementa_nova[j] = elementa[j];
                }
                elementa = elementa_nova;
            }

            elementa[numerus] = chorda_sectio(s, initium, i);
            numerus++;
            initium  = i + delim.mensura;
            i        = initium;
        }
        alioquin
        {
            i++;
        }
    }

    /* Addere ultimum segmentum */
    si (numerus >= capacitas)
    {
         chorda* elementa_nova;
            i32  j;

        capacitas *= II;
        elementa_nova = (chorda*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(chorda));
        si (!elementa_nova)
        {
            fructus.elementa  = NIHIL;
            fructus.numerus   = ZEPHYRUM;
            redde fructus;
        }

        per (j = ZEPHYRUM; j < numerus; j++)
        {
            elementa_nova[j] = elementa[j];
        }
        elementa = elementa_nova;
    }

    elementa[numerus] = chorda_sectio(s, initium, s.mensura);
    numerus++;

    fructus.elementa  = elementa;
    fructus.numerus   = numerus;
    redde fructus;
}

chorda
chorda_iungere (
     chorda* elementa,
        i32  numerus,
     chorda  separator,
    Piscina* piscina)
{
     ChordaAedificator* aed;
                   i32  i;
                chorda  fructus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }

    si (numerus == ZEPHYRUM || elementa == NIHIL)
    {
        redde fructus;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   i > ZEPHYRUM && separator.datum != NIHIL
            && separator.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, separator);
        }
        si (elementa[i].datum != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, elementa[i]);
        }
    }

    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Formatatio
 * ================================================== */

chorda
chorda_ex_bytes_legibilis (
            i64  bytes,
        Piscina* piscina)
{
                chorda  fructus;
             character  buffer[XXXII];
    constans character* suffixum;
                   f64  valor;
               integer  longitudo;

    fructus.datum    = NIHIL;
    fructus.mensura  = 0;

    si (piscina == NIHIL)
    {
        redde fructus;
    }

    /* Determinare suffixum et valorem (bytes i64 insignatus -
     * cohibitio negativi mortua erat, remota 2026-07-17) */
    si (bytes < M * M)  /* < 1 MB */
    {
        si (bytes < M)  /* < 1 KB */
        {
            valor     = (f64)bytes;
            suffixum  = " B";
        }
        alioquin
        {
            valor     = (f64)bytes / (f64)M;
            suffixum  = " KB";
        }
    }
    alioquin si (bytes < (i64)M * (i64)M * (i64)M)  /* < 1 GB */
    {
        valor     = (f64)bytes / (f64)(M * M);
        suffixum  = " MB";
    }
    alioquin
    {
        valor     = (f64)bytes / (f64)((i64)M * (i64)M * (i64)M);
        suffixum  = " GB";
    }

    /* Formatare */
    si (valor < 10.0)
    {
        longitudo = snprintf(buffer, XXXII, "%.1f%s", valor, suffixum);
    }
    alioquin si (valor < 100.0)
    {
        longitudo = snprintf(buffer, XXXII, "%.1f%s", valor, suffixum);
    }
    alioquin
    {
        longitudo = snprintf(buffer, XXXII, "%.0f%s", valor, suffixum);
    }

    si (longitudo <= 0 || longitudo >= (integer)XXXII)
    {
        redde fructus;
    }

    fructus = chorda_ex_literis(buffer, piscina);

    redde fructus;
}
#undef _chorda_nullum_habet
#undef _extrahere_verba
#undef CHORDA_FRIATUM_OFFSET
#undef CHORDA_FRIATUM_PRIMUS
/* lib/magnus.c: statica per plagulam renominata */
#define _absolutum_s64 _absolutum_s64_magnus
#define _alternae_aperire _alternae_aperire_magnus
#define _alternae_claudere _alternae_claudere_magnus
#define _alternae_illic _alternae_illic_magnus
#define _alternae_vertere _alternae_vertere_magnus
#define _apex_alternarum _apex_alternarum_magnus
#define _apex_notare _apex_notare_magnus
#define _aspectus _aspectus_magnus
#define _ex_moduli_propriis _ex_moduli_propriis_magnus
#define _ex_signo_et_modulo _ex_signo_et_modulo_magnus
#define _longitudo_vera _longitudo_vera_magnus
#define _membra_nova _membra_nova_magnus
#define _moduli_adde _moduli_adde_magnus
#define _moduli_compara _moduli_compara_magnus
#define _moduli_divide _moduli_divide_magnus
#define _moduli_divide_parvo _moduli_divide_parvo_magnus
#define _moduli_multiplica _moduli_multiplica_magnus
#define _moduli_subtrahe _moduli_subtrahe_magnus
#define _parvus _parvus_magnus
#define _per_alternas _per_alternas_magnus
#define _summa_signata _summa_signata_magnus
#define _transcribere _transcribere_magnus
#define MagnusAlternae MagnusAlternae_magnus
#line 1 "lib/magnus.c"
/* magnus.c - Integri magni exacti
 *
 * Moduli (valores absoluti) in membris XXXII bitorum insignatis,
 * ordine parvo primum; producta et portationes in i64 insignato, ubi
 * omnis gradus exactus est: (2^32 - 1)^2 + 2(2^32 - 1) = 2^64 - 1.
 * Arithmetica insignata in membris ubique (C definit circumvolutionem
 * insignatam; exundatio signata indefinita est). Via celeris in s64
 * limites ANTE operationem probat, numquam post. Divisio: Knuth,
 * algorithmus D (TAOCP II, 4.3.1). Nulli fluitantes.
 * Vide lib/magnus.worklog.md.
 */


#define MAGNUS_S64_SUMMUS       ((s64)0x7FFFFFFFFFFFFFFFLL)
#define MAGNUS_S64_IMUS         (-MAGNUS_S64_SUMMUS - I)
#define MAGNUS_LIMES_NEGATIVUS  0x8000000000000000ULL   /* |S64_IMUS| */
#define MAGNUS_BASIS            0x100000000ULL           /* 2^32 */
#define MAGNUS_FRUSTUM          1000000000U              /* 10^9 */
#define MAGNUS_FACTOR_PARVUS    0x7FFFFFFFLL             /* 2^31 - 1 */


/* ==================================================
 * Auxilia: creatio canonica
 * ================================================== */

interior Magnus
_parvus (
    s64 valor)
{
    Magnus a;

    a.parvus     = valor;
    a.signum     = ZEPHYRUM;
    a.longitudo  = ZEPHYRUM;
    a.membra     = NIHIL;
    redde a;
}

/* |valor| sine exundatione: -(valor + 1) semper capit */
interior i64
_absolutum_s64 (
    s64 valor)
{
    si (valor < ZEPHYRUM)
    {
        redde (i64)(-(valor + I)) + 1ULL;
    }
    redde (i64)valor;
}

interior i32*
_membra_nova (
    Piscina* piscina,
        i32  numerus)
{
    i32* membra;
    i32  k;

    membra = (i32*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)numerus * magnitudo(i32), magnitudo(i32));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        membra[k] = ZEPHYRUM;
    }
    redde membra;
}

/* longitudo sine membris summis nullis */
interior i32
_longitudo_vera (
    constans i32* moduli,
             i32  longitudo)
{
    dum (longitudo > ZEPHYRUM && moduli[longitudo - I] == ZEPHYRUM)
    {
        longitudo--;
    }
    redde longitudo;
}

/* valor ex signo et modulo i64; piscina tantum si non capit */
interior Magnus
_ex_signo_et_modulo (
         s32  signum,
         i64  modulus,
    Piscina*  piscina)
{
    Magnus a;

    si (modulus == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    si (signum > ZEPHYRUM && modulus <= (i64)MAGNUS_S64_SUMMUS)
    {
        redde _parvus((s64)modulus);
    }
    si (signum < ZEPHYRUM && modulus <= MAGNUS_LIMES_NEGATIVUS)
    {
        redde _parvus(-(s64)(modulus - 1ULL) - I);
    }
    a.parvus            = ZEPHYRUM;
    a.signum            = signum;
    a.longitudo         = II;
    a.membra            = _membra_nova(piscina, II);
    a.membra[ZEPHYRUM]  = (i32)modulus;
    a.membra[I]         = (i32)(modulus >> XXXII);
    redde a;
}

/* forma canonica ex membris PROPRIIS (ex piscina aut membris
 * immutabilibus alius valoris): numquam alveum temporarium recipit,
 * nisi valor certe in s64 capit */
interior Magnus
_ex_moduli_propriis (
    s32  signum,
    i32* moduli,
    i32  longitudo)
{
    Magnus a;

    longitudo = _longitudo_vera(moduli, longitudo);
    si (longitudo == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    si (longitudo <= II)
    {
        i64 modulus = (i64)moduli[ZEPHYRUM];

        si (longitudo == II)
        {
            modulus |= (i64)moduli[I] << XXXII;
        }
        si (signum > ZEPHYRUM && modulus <= (i64)MAGNUS_S64_SUMMUS)
        {
            redde _parvus((s64)modulus);
        }
        si (signum < ZEPHYRUM && modulus <= MAGNUS_LIMES_NEGATIVUS)
        {
            redde _parvus(-(s64)(modulus - 1ULL) - I);
        }
    }
    a.parvus     = ZEPHYRUM;
    a.signum     = signum;
    a.longitudo  = longitudo;
    a.membra     = moduli;
    redde a;
}

/* aspectus moduli: parvi in alveum II membrorum explicantur */
interior vacuum
_aspectus (
             Magnus   a,
                i32*  alveus,
    constans    i32** moduli,
                i32*  longitudo,
                s32*  signum)
{
    si (a.membra == NIHIL)
    {
        i64 modulus = _absolutum_s64(a.parvus);

        alveus[ZEPHYRUM] = (i32)modulus;
        alveus[I] = (i32)(modulus >> XXXII);
        *moduli = alveus;
        *longitudo = _longitudo_vera(alveus, II);
        *signum = (a.parvus > ZEPHYRUM) - (a.parvus < ZEPHYRUM);
    }
    alioquin
    {
        *moduli     = a.membra;
        *longitudo  = a.longitudo;
        *signum     = a.signum;
    }
}


/* ==================================================
 * Auxilia: arithmetica modulorum
 * ================================================== */

/* -1, 0, +1 */
interior s32
_moduli_compara (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb)
{
    s32 k;

    si (la != lb)
    {
        redde (la < lb) ? -I : I;
    }
    per (k = (s32)la - I; k >= ZEPHYRUM; k--)
    {
        si (a[k] != b[k])
        {
            redde (a[k] < b[k]) ? -I : I;
        }
    }
    redde ZEPHYRUM;
}

/* exitus longitudinis max(la, lb) + 1 */
interior vacuum
_moduli_adde (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i64 portans          = ZEPHYRUM;
    i32 summa_longitudo  = (la > lb) ? la : lb;
    i32 k;

    per (k = ZEPHYRUM; k < summa_longitudo; k++)
    {
        i64 t = portans;

        si (k < la) t += a[k];
        si (k < lb) t += b[k];
        exitus[k]  = (i32)t;
        portans    = t >> XXXII;
    }
    exitus[summa_longitudo] = (i32)portans;
}

/* a >= b; exitus longitudinis la */
interior vacuum
_moduli_subtrahe (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i64 mutuum = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < la; k++)
    {
        i64 d = (i64)a[k] - mutuum;

        si (k < lb) d -= b[k];
        exitus[k]  = (i32)d;
        mutuum     = d >> LXIII;   /* circumvolutio: bit summum */
    }
}

/* exitus nullus, longitudinis la + lb */
interior vacuum
_moduli_multiplica (
    constans i32* a,
             i32  la,
    constans i32* b,
             i32  lb,
             i32* exitus)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < la; k++)
    {
        i64 portans = ZEPHYRUM;

        per (j = ZEPHYRUM; j < lb; j++)
        {
            i64 t = (i64)a[k] * b[j] + exitus[k + j] + portans;

            exitus[k + j]  = (i32)t;
            portans        = t >> XXXII;
        }
        exitus[k + lb] = (i32)portans;
    }
}

/* quotiens in exitum (idem ac a licet); residuum redditur */
interior i32
_moduli_divide_parvo (
    constans i32* a,
             i32  la,
             i32  divisor,
             i32* exitus)
{
    i64 residuum = ZEPHYRUM;
    s32 k;

    per (k = (s32)la - I; k >= ZEPHYRUM; k--)
    {
        i64 numerus = (residuum << XXXII) | a[k];

        exitus[k]  = (i32)(numerus / divisor);
        residuum   = numerus % divisor;
    }
    redde (i32)residuum;
}

/* Knuth D: u (lu membra) / v (lv >= 2 membra), lu >= lv, v summum
 * non nullum. quotiens: lu - lv + 1 membra; residuum: lv membra. */
interior vacuum
_moduli_divide (
    constans i32* u,
             i32  lu,
    constans i32* v,
             i32  lv,
             i32* quotiens,
             i32* residuum,
         Piscina* piscina)
{
    i32* un  = _membra_nova(piscina, lu + I);
    i32* vn  = _membra_nova(piscina, lv);
    i32  s   = ZEPHYRUM;
    i32  summum;
    s32  j;
    i32  k;

    /* D1: normalizatio - divisor summum bit habeat */
    summum = v[lv - I];
    dum (!(summum & 0x80000000U))
    {
        summum <<= I;
        s++;
    }
    per (k = lv - I; k > ZEPHYRUM; k--)
    {
        vn[k] = (i32)((((i64)v[k] << XXXII) | v[k - I]) >> (XXXII - s));
    }
    vn[ZEPHYRUM]  = v[ZEPHYRUM] << s;
    un[lu]        = (i32)((i64)u[lu - I] >> (XXXII - s));
    per (k = lu - I; k > ZEPHYRUM; k--)
    {
        un[k] = (i32)((((i64)u[k] << XXXII) | u[k - I]) >> (XXXII - s));
    }
    un[ZEPHYRUM] = u[ZEPHYRUM] << s;

    per (j = (s32)(lu - lv); j >= ZEPHYRUM; j--)
    {
        i64 numerus;
        i64 aestimatio;
        i64 reliquum;
        i64 portans  = ZEPHYRUM;
        i64 mutuum   = ZEPHYRUM;
        i64 d;
        i32 jj = (i32)j;

        /* D3: aestimatio quotientis ex duobus membris summis */
        numerus     = ((i64)un[jj + lv] << XXXII) | un[jj + lv - I];
        aestimatio  = numerus / vn[lv - I];
        reliquum    = numerus - aestimatio * vn[lv - I];
        dum (   aestimatio >= MAGNUS_BASIS
             || aestimatio * vn[lv - II]
            > ((reliquum << XXXII) | un[jj + lv - II]))
        {
            aestimatio--;
            reliquum += vn[lv - I];
            si (reliquum >= MAGNUS_BASIS)
            {
                frange;
            }
        }

        /* D4: multiplica et subtrahe */
        per (k = ZEPHYRUM; k < lv; k++)
        {
            i64 p = aestimatio * vn[k] + portans;

            portans     = p >> XXXII;
            d           = (i64)un[k + jj] - (i32)p - mutuum;
            un[k + jj]  = (i32)d;
            mutuum      = d >> LXIII;
        }
        d            = (i64)un[jj + lv] - portans - mutuum;
        un[jj + lv]  = (i32)d;

        /* D6: aestimatio uno nimia (rarissime) - adde retro */
        si (d >> LXIII)
        {
            aestimatio--;
            portans = ZEPHYRUM;
            per (k = ZEPHYRUM; k < lv; k++)
            {
                i64 t = (i64)un[k + jj] + vn[k] + portans;

                un[k + jj]  = (i32)t;
                portans     = t >> XXXII;
            }
            /* portatio ultima neglegitur (Knuth D6): un[jj + lv]
             * post hunc gradum non legitur */
        }
        quotiens[jj] = (i32)aestimatio;
    }

    /* D8: denormalizatio residui */
    per (k = ZEPHYRUM; k + I < lv; k++)
    {
        residuum[k] = (i32)((((i64)un[k + I] << XXXII) | un[k]) >> s);
    }
    residuum[lv - I] = un[lv - I] >> s;
}


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Magnus
magnus_ex_s64 (
    s64 valor)
{
    redde _parvus(valor);
}

b32
magnus_ad_s64 (
    Magnus  a,
       s64* exitus)
{
    si (a.membra != NIHIL)
    {
        redde FALSUM;
    }
    si (exitus)
    {
        *exitus = a.parvus;
    }
    redde VERUM;
}

b32
magnus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
      Magnus* exitus)
{
    i32  positus  = ZEPHYRUM;
    s32  signum   = I;
    i32  numerus_digitorum;
    i32  frustum;
    i32  longitudo = ZEPHYRUM;
    i32* moduli;
    i32  k;

    si (textus.mensura == ZEPHYRUM || textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    si (textus.datum[ZEPHYRUM] == '-')
    {
        signum   = -I;
        positus  = I;
    }
    si (positus == textus.mensura)
    {
        redde FALSUM;
    }
    per (k = positus; k < textus.mensura; k++)
    {
        si (textus.datum[k] < '0' || textus.datum[k] > '9')
        {
            redde FALSUM;
        }
    }

    numerus_digitorum = textus.mensura - positus;
    moduli = _membra_nova(piscina, numerus_digitorum / IX + II);

    /* frusta IX digitorum a sinistra; primum frustum residuum */
    frustum = numerus_digitorum % IX;
    si (frustum == ZEPHYRUM)
    {
        frustum = IX;
    }
    dum (positus < textus.mensura)
    {
        i32 valor          = ZEPHYRUM;
        i32 multiplicator  = I;
        i64 portans;
        i32 m;

        per (m = ZEPHYRUM; m < frustum; m++)
        {
            valor = valor * X + (i32)(textus.datum[positus + m]
                - '0');
            multiplicator *= X;
        }
        positus += frustum;
        frustum = IX;

        /* moduli = moduli * multiplicator + valor */
        portans = valor;
        per (m = ZEPHYRUM; m < longitudo; m++)
        {
            i64 t = (i64)moduli[m] * multiplicator + portans;

            moduli[m]  = (i32)t;
            portans    = t >> XXXII;
        }
        si (portans)
        {
            moduli[longitudo++] = (i32)portans;
        }
    }

    *exitus = _ex_moduli_propriis(signum, moduli, longitudo);
    redde VERUM;
}

chorda
magnus_ad_chordam (
      Magnus  a,
     Piscina* piscina)
{
               i32  alveus[II];
    constans   i32* moduli;
               i32  longitudo;
               s32  signum;
               i32* opus;
               i32* frusta;
               i32  numerus_frustorum = ZEPHYRUM;
                i8* litterae;
               i32  positus = ZEPHYRUM;
               i32  k;

    _aspectus(a, alveus, &moduli, &longitudo, &signum);
    si (longitudo == ZEPHYRUM)
    {
        litterae            = (i8*)piscina_allocare(piscina, I);
        litterae[ZEPHYRUM]  = '0';
        redde chorda_ex_buffer(litterae, I);
    }

    /* frusta 10^9 a dextra, divisione parva repetita */
    opus    = _membra_nova(piscina, longitudo);
    frusta  = _membra_nova(piscina, longitudo * II + II);
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        opus[k] = moduli[k];
    }
    dum (longitudo > ZEPHYRUM)
    {
        frusta[numerus_frustorum++] =
            _moduli_divide_parvo(opus, longitudo, MAGNUS_FRUSTUM, opus);
        longitudo = _longitudo_vera(opus, longitudo);
    }

    /* IX digiti per frustum 10^9; signum unum */
    litterae = (i8*)piscina_allocare(piscina,
        (memoriae_index)numerus_frustorum * IX + II);
    si (signum < ZEPHYRUM)
    {
        litterae[positus++] = '-';
    }
    {
         i8 digiti[X];
        i32 numerus  = ZEPHYRUM;
        i32 summum   = frusta[numerus_frustorum - I];

        dum (summum > ZEPHYRUM)
        {
            digiti[numerus++]  = (i8)('0' + summum % X);
            summum             /= X;
        }
        dum (numerus > ZEPHYRUM)
        {
            litterae[positus++] = digiti[--numerus];
        }
    }
    per (k = numerus_frustorum - I; k > ZEPHYRUM; k--)
    {
        i32 frustum = frusta[k - I];
        s32 m;

        per (m = VIII; m >= ZEPHYRUM; m--)
        {
            litterae[positus + (i32)m]  = (i8)('0' + frustum % X);
            frustum                     /= X;
        }
        positus += IX;
    }
    redde chorda_ex_buffer(litterae, positus);
}


/* ==================================================
 * Inspectio
 * ================================================== */

s32
magnus_signum (
    Magnus a)
{
    si (a.membra != NIHIL)
    {
        redde a.signum;
    }
    redde (a.parvus > ZEPHYRUM) - (a.parvus < ZEPHYRUM);
}

s32
magnus_compara (
    Magnus a,
    Magnus b)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               s32  ordo;

    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        redde (a.parvus > b.parvus) - (a.parvus < b.parvus);
    }
    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    si (sa != sb)
    {
        redde (sa < sb) ? -I : I;
    }
    ordo = _moduli_compara(ma, la, mb, lb);
    redde (sa >= ZEPHYRUM) ? ordo : -ordo;
}

b32
magnus_aequalis (
    Magnus a,
    Magnus b)
{
    redde magnus_compara(a, b) == ZEPHYRUM;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Magnus
magnus_nega (
      Magnus  a,
    Piscina*  piscina)
{
    si (a.membra == NIHIL)
    {
        si (a.parvus != MAGNUS_S64_IMUS)
        {
            redde _parvus(-a.parvus);
        }
        redde _ex_signo_et_modulo(I, MAGNUS_LIMES_NEGATIVUS, piscina);
    }
    /* membra partita: +2^63 negatum in s64 capit */
    redde _ex_moduli_propriis(-a.signum, a.membra, a.longitudo);
}

Magnus
magnus_absolutum (
      Magnus  a,
     Piscina* piscina)
{
    si (magnus_signum(a) >= ZEPHYRUM)
    {
        redde a;
    }
    redde magnus_nega(a, piscina);
}

/* summa signata modulorum; semper in membra nova */
interior Magnus
_summa_signata (
             Magnus  a,
             Magnus  b,
                s32  flexio_b,
            Piscina* piscina)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* exitus;

    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    sb *= flexio_b;

    si (sa == ZEPHYRUM || sb == ZEPHYRUM || sa == sb)
    {
        i32 longitudo = ((la > lb) ? la : lb) + I;

        exitus = _membra_nova(piscina, longitudo);
        _moduli_adde(ma, la, mb, lb, exitus);
        redde _ex_moduli_propriis((sa != ZEPHYRUM) ? sa : sb,
            exitus, longitudo);
    }
    si (_moduli_compara(ma, la, mb, lb) >= ZEPHYRUM)
    {
        exitus = _membra_nova(piscina, la);
        _moduli_subtrahe(ma, la, mb, lb, exitus);
        redde _ex_moduli_propriis(sa, exitus, la);
    }
    exitus = _membra_nova(piscina, lb);
    _moduli_subtrahe(mb, lb, ma, la, exitus);
    redde _ex_moduli_propriis(sb, exitus, lb);
}

Magnus
magnus_adde (
      Magnus  a,
      Magnus  b,
    Piscina*  piscina)
{
    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        si (!((y > ZEPHYRUM && x > MAGNUS_S64_SUMMUS - y)
            || (y < ZEPHYRUM && x < MAGNUS_S64_IMUS - y)))
        {
            redde _parvus(x + y);
        }
    }
    redde _summa_signata(a, b, I, piscina);
}

Magnus
magnus_subtrahe (
      Magnus  a,
      Magnus  b,
    Piscina*  piscina)
{
    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        si (!((y < ZEPHYRUM && x > MAGNUS_S64_SUMMUS + y)
            || (y > ZEPHYRUM && x < MAGNUS_S64_IMUS + y)))
        {
            redde _parvus(x - y);
        }
    }
    redde _summa_signata(a, b, -I, piscina);
}

Magnus
magnus_multiplica (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* exitus;

    si (a.membra == NIHIL && b.membra == NIHIL)
    {
        s64 x = a.parvus;
        s64 y = b.parvus;

        /* |x|, |y| < 2^31 => |x*y| < 2^62 */
        si (   x >= -MAGNUS_FACTOR_PARVUS && x <= MAGNUS_FACTOR_PARVUS
            && y >= -MAGNUS_FACTOR_PARVUS && y <= MAGNUS_FACTOR_PARVUS)
        {
            redde _parvus(x * y);
        }
    }
    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(b, alveus_b, &mb, &lb, &sb);
    si (la == ZEPHYRUM || lb == ZEPHYRUM)
    {
        redde _parvus(ZEPHYRUM);
    }
    exitus = _membra_nova(piscina, la + lb);
    _moduli_multiplica(ma, la, mb, lb, exitus);
    redde _ex_moduli_propriis(sa * sb, exitus, la + lb);
}

b32
magnus_divide (
      Magnus  a,
      Magnus  divisor,
     Piscina* piscina,
      Magnus* quotiens,
      Magnus* residuum)
{
               i32  alveus_a[II];
               i32  alveus_b[II];
    constans   i32* ma;
    constans   i32* mb;
               i32  la;
               i32  lb;
               s32  sa;
               s32  sb;
               i32* q;
               i32* r;
               i32  lq;
               i32  k;

    si (magnus_signum(divisor) == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* via celeris: moduli in i64, divisio insignata (definita) */
    si (a.membra == NIHIL && divisor.membra == NIHIL)
    {
        i64 modulus_a  = _absolutum_s64(a.parvus);
        i64 modulus_b  = _absolutum_s64(divisor.parvus);
        i64 q0         = modulus_a / modulus_b;
        i64 r0         = modulus_a % modulus_b;
        s32 signum_b   = (divisor.parvus > ZEPHYRUM) ? I : -I;

        si (a.parvus < ZEPHYRUM && r0 != ZEPHYRUM)
        {
            q0 += 1ULL;
            r0 = modulus_b - r0;
        }
        si (quotiens)
        {
            *quotiens = _ex_signo_et_modulo(
                (a.parvus < ZEPHYRUM) ? -signum_b : signum_b, q0,
                piscina);
        }
        si (residuum)
        {
            *residuum = _parvus((s64)r0);
        }
        redde VERUM;
    }

    _aspectus(a, alveus_a, &ma, &la, &sa);
    _aspectus(divisor, alveus_b, &mb, &lb, &sb);

    lq  = (la >= lb) ? la - lb + I : I;
    q   = _membra_nova(piscina, lq + I);
    r   = _membra_nova(piscina, lb + I);
    si (_moduli_compara(ma, la, mb, lb) < ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < la; k++)
        {
            r[k] = ma[k];
        }
    }
    alioquin si (lb == I)
    {
        r[ZEPHYRUM] = _moduli_divide_parvo(ma, la, mb[ZEPHYRUM], q);
    }
    alioquin
    {
        _moduli_divide(ma, la, mb, lb, q, r, piscina);
    }

    /* Euclidea: a < 0 et residuum non nullum => q + 1, |b| - r */
    si (sa < ZEPHYRUM && _longitudo_vera(r, lb) > ZEPHYRUM)
    {
        i32  unum[I];
        i32* q2 = _membra_nova(piscina, lq + I);
        i32* r2 = _membra_nova(piscina, lb + I);

        unum[ZEPHYRUM] = I;
        _moduli_adde(q, lq, unum, I, q2);
        _moduli_subtrahe(mb, lb, r, _longitudo_vera(r, lb), r2);
        q = q2;
        r = r2;
    }
    si (quotiens)
    {
        *quotiens = _ex_moduli_propriis((sa < ZEPHYRUM) ? -sb : sb, q,
            lq + I);
    }
    si (residuum)
    {
        *residuum = _ex_moduli_propriis(I, r, lb);
    }
    redde VERUM;
}

Magnus
magnus_potentia (
      Magnus  basis,
         i32  exponens,
     Piscina* piscina)
{
    Magnus fructus = _parvus(I);

    dum (exponens > ZEPHYRUM)
    {
        si (exponens & I)
        {
            fructus = magnus_multiplica(fructus, basis, piscina);
        }
        exponens >>= I;
        si (exponens > ZEPHYRUM)
        {
            basis = magnus_multiplica(basis, basis, piscina);
        }
    }
    redde fructus;
}

/* Euclides in piscinis ALTERNIS. Gradus quisque in piscinam
 * alteram computat, valores qui supersunt eo transcribit, priorem ad
 * notam initialem reficit: memoria ergo proportionalis magnitudini
 * operandorum, non gradibus (recensio I, 2026-10-05: olim CXX MB pro
 * mdc X milium digitorum, omnia in piscina vocantis). Effectus solus
 * in piscinam vocantis transcribitur. */
nomen structura {
           Piscina* piscinae[II];
    PiscinaNotatio  notae[II];
               i32  hic;
} MagnusAlternae;

/* Operandi usque ad IV membra (~XXXVIII digiti): Euclides in piscina
 * vocantis, ut olim - piscinae alternae pro numeris parvis plus
 * constant quam servant (recensio II: mdc XX digitorum 0.46 -> 2.35
 * us); iactura vocantis paucis milibus octetorum finitur. */
#define MAGNUS_LIMES_ALTERNARUM IV

/* DIAGNOSIS (agenda A3): maximus usus piscinae alternae in ultimo
 * divisore communi per alternas computato */
interior memoriae_index _apex_alternarum = ZEPHYRUM;

/* Divisor communis SINE testibus: AMBO magni requiruntur - si unus
 * parvus est, gradus primus omnia parva facit, ergo Euclides in
 * piscina vocantis finitus manet (recensio III: mdc(magnus, 1) in
 * fractione piscinas alternas sine causa aperiebat - saltus temporis
 * ad XL digitos). CUM testibus: UNUS magnus sufficit - post gradum
 * primum testis operandi parvi ~ |a|/g magnus est et in omni gradu
 * sequente novus fit (recensio IV: K F184 + F183 et F184, X M
 * digitorum: MB 1.5 in piscina vocantis pro KB IV effectus). */
interior b32
_per_alternas (
    Magnus a,
    Magnus b,
       b32 ambo)
{
    b32 a_magnus = a.membra != NIHIL
        && a.longitudo > MAGNUS_LIMES_ALTERNARUM;
    b32 b_magnus = b.membra != NIHIL
        && b.longitudo > MAGNUS_LIMES_ALTERNARUM;

    redde ambo ? (a_magnus && b_magnus) : (a_magnus || b_magnus);
}

interior vacuum
_apex_notare (
    Piscina* piscina)
{
    memoriae_index usus = piscina_summa_usus(piscina);

    si (usus > _apex_alternarum)
    {
        _apex_alternarum = usus;
    }
}

interior b32
_alternae_aperire (
    MagnusAlternae* al)
{
    al->piscinae[ZEPHYRUM]  =
        piscina_generare_dynamicum("magnus_alterna",
        (memoriae_index)4096);
    al->piscinae[I]         =
        piscina_generare_dynamicum("magnus_alterna",
        (memoriae_index)4096);
    al->hic           = ZEPHYRUM;
    _apex_alternarum  = ZEPHYRUM;
    si (al->piscinae[ZEPHYRUM] == NIHIL || al->piscinae[I] == NIHIL)
    {
        si (al->piscinae[ZEPHYRUM])
        {
            piscina_destruere(al->piscinae[ZEPHYRUM]);
        }
        si (al->piscinae[I])
        {
            piscina_destruere(al->piscinae[I]);
        }
        redde FALSUM;
    }
    al->notae[ZEPHYRUM]  = piscina_notare(al->piscinae[ZEPHYRUM]);
    al->notae[I]         = piscina_notare(al->piscinae[I]);
    redde VERUM;
}

interior Piscina*
_alternae_illic (
    constans MagnusAlternae* al)
{
    redde al->piscinae[I - al->hic];
}

/* Piscina currens ad notam initialem reficitur, NON vacatur:
 * piscina_vacare totam capacitatem memset implet, et id gradu quoque
 * erat sumptus a recensione II inventus (membra nova a _membra_nova
 * iam nullantur). Altera fit currens. */
interior vacuum
_alternae_vertere (
    MagnusAlternae* al)
{
    _apex_notare(al->piscinae[al->hic]);
    piscina_reficere(al->piscinae[al->hic], al->notae[al->hic]);
    al->hic = I - al->hic;
}

interior vacuum
_alternae_claudere (
    MagnusAlternae* al)
{
    _apex_notare(al->piscinae[ZEPHYRUM]);
    _apex_notare(al->piscinae[I]);
    piscina_destruere(al->piscinae[ZEPHYRUM]);
    piscina_destruere(al->piscinae[I]);
}

/* copia valoris in piscinam datam (parvi sine allocatione) */
interior Magnus
_transcribere (
      Magnus  a,
     Piscina* piscina)
{
    Magnus copia = a;
       i32 k;

    si (a.membra == NIHIL)
    {
        redde a;
    }
    copia.membra = _membra_nova(piscina, a.longitudo);
    per (k = ZEPHYRUM; k < a.longitudo; k++)
    {
        copia.membra[k] = a.membra[k];
    }
    redde copia;
}

i32
magnus_bitorum (
    Magnus a)
{
               i32  alveus[II];
    constans   i32* moduli;
               i32  longitudo;
               s32  signum;
               i32  summum;
               i32  bita;

    si (magnus_signum(a) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    _aspectus(a, alveus, &moduli, &longitudo, &signum);
    summum  = moduli[longitudo - I];
    bita    = (longitudo - I) * XXXII;
    dum (summum != ZEPHYRUM)
    {
        bita++;
        summum = summum >> I;
    }
    redde bita;
}

i32
magnus_residuum_parvum (
    Magnus a,
       i32 n)
{
    i64 residuum = ZEPHYRUM;
    i32 k;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (a.membra == NIHIL)
    {
        /* |parvus| sine exundatione etiam pro S64 imo */
        i64 modulus = a.parvus < ZEPHYRUM
            ? (i64)(-(a.parvus + I)) + (i64)I : (i64)a.parvus;

        residuum = modulus % (i64)n;
        si (a.parvus < ZEPHYRUM && residuum != ZEPHYRUM)
        {
            residuum = (i64)n - residuum;
        }
        redde (i32)residuum;
    }
    /* Horner super membra ab summo: residuum < n < 2^32, ergo
     * (residuum << 32) | membrum < 2^64 */
    per (k = a.longitudo; k-- > ZEPHYRUM;)
    {
        residuum = ((residuum << XXXII) | (i64)a.membra[k]) % (i64)n;
    }
    si (a.signum < ZEPHYRUM && residuum != ZEPHYRUM)
    {
        residuum = (i64)n - residuum;
    }
    redde (i32)residuum;
}

Magnus
magnus_transcribe (
      Magnus  a,
     Piscina* piscina)
{
    redde _transcribere(a, piscina);
}

Magnus
magnus_divisor_communis (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
    MagnusAlternae al;
            Magnus x = magnus_absolutum(a, piscina);
            Magnus y = magnus_absolutum(b, piscina);

    _apex_alternarum = ZEPHYRUM;
    /* operandus pauci membrorum: in piscina vocantis (vide
     * _per_alternas) */
    si (!_per_alternas(a, b, VERUM) || !_alternae_aperire(&al))
    {
        dum (magnus_signum(y) != ZEPHYRUM)
        {
            Magnus r;

            (vacuum)magnus_divide(x, y, piscina, NIHIL, &r);
            x = y;
            y = r;
        }
        redde x;
    }
    dum (magnus_signum(y) != ZEPHYRUM)
    {
        Piscina* illic = _alternae_illic(&al);
         Magnus  r;

        (vacuum)magnus_divide(x, y, illic, NIHIL, &r);
        x = _transcribere(y, illic);
        y = r;
        _alternae_vertere(&al);
    }
    x = _transcribere(x, piscina);
    _alternae_claudere(&al);
    redde x;
}

Magnus
magnus_divisor_communis_testatus (
      Magnus  a,
      Magnus  b,
     Piscina* piscina,
      Magnus* u,
      Magnus* v)
{
    MagnusAlternae al;
               b32 alternae;
            Magnus r0 = magnus_absolutum(a, piscina);
            Magnus r1 = magnus_absolutum(b, piscina);
            Magnus s0 = _parvus(I);
            Magnus s1 = _parvus(ZEPHYRUM);
            Magnus t0 = _parvus(ZEPHYRUM);
            Magnus t1 = _parvus(I);

    /* ambo operandi pauci membrorum: testes |s| <= |b|, |t| <= |a|
     * parvi manent, ergo in piscina vocantis (vide _per_alternas) */
    _apex_alternarum  = ZEPHYRUM;
    alternae          = _per_alternas(a, b, FALSUM)
        && _alternae_aperire(&al);

    dum (magnus_signum(r1) != ZEPHYRUM)
    {
        Piscina* illic = alternae ? _alternae_illic(&al) : piscina;
         Magnus  q;
         Magnus  r2;
         Magnus  s2;
         Magnus  t2;

        (vacuum)magnus_divide(r0, r1, illic, &q, &r2);
        s2 = magnus_subtrahe(s0, magnus_multiplica(q, s1, illic),
            illic);
        t2 = magnus_subtrahe(t0, magnus_multiplica(q, t1, illic),
            illic);
        si (alternae)
        {
            r0 = _transcribere(r1, illic);
            s0 = _transcribere(s1, illic);
            t0 = _transcribere(t1, illic);
        }
        alioquin
        {
            r0 = r1;
            s0 = s1;
            t0 = t1;
        }
        r1 = r2;
        s1 = s2;
        t1 = t2;
        si (alternae)
        {
            _alternae_vertere(&al);
        }
    }
    si (alternae)
    {
        r0 = _transcribere(r0, piscina);
        s0 = _transcribere(s0, piscina);
        t0 = _transcribere(t0, piscina);
        _alternae_claudere(&al);
    }
    /* g = s0|a| + t0|b|: signa argumentorum in testes transfer */
    si (u)
    {
        *u = (magnus_signum(a) < ZEPHYRUM) ? magnus_nega(s0,
            piscina) : s0;
    }
    si (v)
    {
        *v = (magnus_signum(b) < ZEPHYRUM) ? magnus_nega(t0,
            piscina) : t0;
    }
    redde r0;
}

memoriae_index
magnus_apex_alternarum (
    vacuum)
{
    redde _apex_alternarum;
}
#undef _absolutum_s64
#undef _alternae_aperire
#undef _alternae_claudere
#undef _alternae_illic
#undef _alternae_vertere
#undef _apex_alternarum
#undef _apex_notare
#undef _aspectus
#undef _ex_moduli_propriis
#undef _ex_signo_et_modulo
#undef _longitudo_vera
#undef _membra_nova
#undef _moduli_adde
#undef _moduli_compara
#undef _moduli_divide
#undef _moduli_divide_parvo
#undef _moduli_multiplica
#undef _moduli_subtrahe
#undef _parvus
#undef _per_alternas
#undef _summa_signata
#undef _transcribere
#undef MAGNUS_BASIS
#undef MAGNUS_FACTOR_PARVUS
#undef MAGNUS_FRUSTUM
#undef MAGNUS_LIMES_ALTERNARUM
#undef MAGNUS_LIMES_NEGATIVUS
#undef MAGNUS_S64_IMUS
#undef MAGNUS_S64_SUMMUS
#undef MagnusAlternae
/* lib/fractio.c: statica per plagulam renominata */
#define _est_unum _est_unum_fractio
#define _ex_partibus _ex_partibus_fractio
#define _quotiens _quotiens_fractio
#line 1 "lib/fractio.c"
/* fractio.c - Numeri rationales exacti super magnum
 *
 * Omnis operatio formam canonicam reddit (denominator > 0, divisor
 * communis 1). Summa et productum per reductionem Henrici (Knuth II,
 * 4.5.1): divisores communes ante multiplicationem tolluntur, et
 * effectus sine reductione finali iam canonicus est. Divisio exacta
 * per divisionem Euclideam magni (residuum nullum). Nulli fluitantes.
 * Vide lib/fractio.worklog.md.
 */



/* ==================================================
 * Auxilia
 * ================================================== */

/* fractio ex partibus IAM canonicis */
interior Fractio
_ex_partibus (
    Magnus numerator,
    Magnus denominator)
{
    Fractio a;

    a.numerator    = numerator;
    a.denominator  = denominator;
    redde a;
}

/* quotiens Euclideus a / b (exactus ubi b a dividit) */
interior Magnus
_quotiens (
      Magnus  a,
      Magnus  b,
     Piscina* piscina)
{
    Magnus quotiens = magnus_ex_s64(ZEPHYRUM);

    (vacuum)magnus_divide(a, b, piscina, &quotiens, NIHIL);
    redde quotiens;
}

interior b32
_est_unum (
    Magnus a)
{
    redde magnus_aequalis(a, magnus_ex_s64(I));
}


/* ==================================================
 * Creatio et conversio
 * ================================================== */

Fractio
fractio_ex_s64 (
    s64 valor)
{
    redde _ex_partibus(magnus_ex_s64(valor), magnus_ex_s64(I));
}

Fractio
fractio_ex_magno (
    Magnus valor)
{
    redde _ex_partibus(valor, magnus_ex_s64(I));
}

b32
fractio_ex_magnis (
      Magnus  numerator,
      Magnus  denominator,
     Piscina* piscina,
     Fractio* exitus)
{
    Magnus divisor;

    si (magnus_signum(denominator) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* divisor communis(0, d) = |d|: nihil fit 0/1 */
    divisor      = magnus_divisor_communis(numerator, denominator,
        piscina);
    numerator    = _quotiens(numerator, divisor, piscina);
    denominator  = _quotiens(denominator, divisor, piscina);
    si (magnus_signum(denominator) < ZEPHYRUM)
    {
        numerator    = magnus_nega(numerator, piscina);
        denominator  = magnus_nega(denominator, piscina);
    }
    *exitus = _ex_partibus(numerator, denominator);
    redde VERUM;
}

b32
fractio_ex_s64_s64 (
         s64  numerator,
         s64  denominator,
     Piscina* piscina,
     Fractio* exitus)
{
    redde fractio_ex_magnis(magnus_ex_s64(numerator),
        magnus_ex_s64(denominator), piscina, exitus);
}

b32
fractio_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Fractio* exitus)
{
       i32 k;
       i32 vinculum = textus.mensura;
    Magnus numerator;
    Magnus denominator;

    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '/')
        {
            vinculum = k;
            frange;
        }
    }

    si (vinculum == textus.mensura)
    {
        si (!magnus_ex_chorda(textus, piscina, &numerator))
        {
            redde FALSUM;
        }
        *exitus = fractio_ex_magno(numerator);
        redde VERUM;
    }

    /* denominator: digiti soli (signum in numeratore tantum) */
    si (   vinculum + I >= textus.mensura
        || textus.datum[vinculum + I] < '0'
        || textus.datum[vinculum + I] > '9')
    {
        redde FALSUM;
    }
    si (   !magnus_ex_chorda(chorda_sectio(textus, ZEPHYRUM, vinculum),
            piscina, &numerator)
        || !magnus_ex_chorda(chorda_sectio(textus, vinculum + I,
            textus.mensura), piscina, &denominator))
    {
        redde FALSUM;
    }
    redde fractio_ex_magnis(numerator, denominator, piscina, exitus);
}

Fractio
fractio_transcribe (
     Fractio  a,
     Piscina* piscina)
{
    Fractio copia;

    copia.numerator    = magnus_transcribe(a.numerator, piscina);
    copia.denominator  = magnus_transcribe(a.denominator, piscina);
    redde copia;
}

chorda
fractio_ad_chordam (
     Fractio  a,
     Piscina* piscina)
{
    chorda textus = magnus_ad_chordam(a.numerator, piscina);

    si (_est_unum(a.denominator))
    {
        redde textus;
    }
    textus = chorda_concatenare(textus, chorda_ex_literis("/", piscina),
        piscina);
    redde chorda_concatenare(textus,
        magnus_ad_chordam(a.denominator, piscina), piscina);
}

Magnus
fractio_numerator (
    Fractio a)
{
    redde a.numerator;
}

Magnus
fractio_denominator (
    Fractio a)
{
    redde a.denominator;
}


/* ==================================================
 * Inspectio
 * ================================================== */

s32
fractio_signum (
    Fractio a)
{
    redde magnus_signum(a.numerator);
}

s32
fractio_compara (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    si (_est_unum(a.denominator) && _est_unum(b.denominator))
    {
        redde magnus_compara(a.numerator, b.numerator);
    }
    redde magnus_compara(
        magnus_multiplica(a.numerator, b.denominator, piscina),
        magnus_multiplica(b.numerator, a.denominator, piscina));
}

/* forma canonica unica: aequalitas partium */
b32
fractio_aequalis (
    Fractio a,
    Fractio b)
{
    redde magnus_aequalis(a.numerator, b.numerator)
        && magnus_aequalis(a.denominator, b.denominator);
}

b32
fractio_est_integra (
    Fractio a)
{
    redde _est_unum(a.denominator);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Fractio
fractio_nega (
     Fractio  a,
     Piscina* piscina)
{
    redde _ex_partibus(magnus_nega(a.numerator, piscina),
        a.denominator);
}

Fractio
fractio_absolutum (
     Fractio  a,
     Piscina* piscina)
{
    redde _ex_partibus(magnus_absolutum(a.numerator, piscina),
        a.denominator);
}

/* u/u' + v/v' (Knuth II, 4.5.1): d1 = mdc(u', v'); si d1 = 1, summa
 * (u v' + u' v) / (u' v') iam reducta. Aliter
 * t = u (v'/d1) + v (u'/d1), d2 = mdc(t, d1), et summa
 * (t/d2) / ((u'/d1)(v'/d2)) reducta. */
Fractio
fractio_adde (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    Magnus d1;
    Magnus d2;
    Magnus t;
    Magnus ua;
    Magnus vb;

    d1 = magnus_divisor_communis(a.denominator, b.denominator, piscina);
    si (_est_unum(d1))
    {
        t = magnus_adde(
            magnus_multiplica(a.numerator, b.denominator, piscina),
            magnus_multiplica(b.numerator, a.denominator, piscina),
            piscina);
        redde _ex_partibus(t,
            magnus_multiplica(a.denominator, b.denominator, piscina));
    }
    ua = _quotiens(a.denominator, d1, piscina);
    vb = _quotiens(b.denominator, d1, piscina);
    t   = magnus_adde(magnus_multiplica(a.numerator, vb, piscina),
        magnus_multiplica(b.numerator, ua, piscina), piscina);
    d2  = magnus_divisor_communis(t, d1, piscina);
    redde _ex_partibus(_quotiens(t, d2, piscina),
        magnus_multiplica(ua,
            _quotiens(b.denominator, d2, piscina), piscina));
}

Fractio
fractio_subtrahe (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    redde fractio_adde(a, fractio_nega(b, piscina), piscina);
}

/* (u/u')(v/v'): d1 = mdc(u, v'), d2 = mdc(u', v); productum
 * ((u/d1)(v/d2)) / ((u'/d2)(v'/d1)) reductum. mdc(x, 0) = |x| facit
 * ut nihil 0/1 fiat. */
Fractio
fractio_multiplica (
     Fractio  a,
     Fractio  b,
     Piscina* piscina)
{
    Magnus d1 = magnus_divisor_communis(a.numerator, b.denominator,
        piscina);
    Magnus d2 = magnus_divisor_communis(a.denominator, b.numerator,
        piscina);

    redde _ex_partibus(
        magnus_multiplica(_quotiens(a.numerator, d1, piscina),
            _quotiens(b.numerator, d2, piscina), piscina),
        magnus_multiplica(_quotiens(a.denominator, d2, piscina),
            _quotiens(b.denominator, d1, piscina), piscina));
}

b32
fractio_inversa (
     Fractio  a,
     Piscina* piscina,
     Fractio* exitus)
{
    s32 signum = magnus_signum(a.numerator);

    si (signum == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (signum > ZEPHYRUM)
    {
        *exitus = _ex_partibus(a.denominator, a.numerator);
    }
    alioquin
    {
        *exitus = _ex_partibus(magnus_nega(a.denominator, piscina),
            magnus_nega(a.numerator, piscina));
    }
    redde VERUM;
}

b32
fractio_divide (
     Fractio  a,
     Fractio  divisor,
     Piscina* piscina,
     Fractio* exitus)
{
    Fractio inversa;

    si (!fractio_inversa(divisor, piscina, &inversa))
    {
        redde FALSUM;
    }
    *exitus = fractio_multiplica(a, inversa, piscina);
    redde VERUM;
}

b32
fractio_potentia (
     Fractio  basis,
         s32  exponens,
     Piscina* piscina,
     Fractio* exitus)
{
    i32 modulus;

    si (exponens < ZEPHYRUM)
    {
        si (!fractio_inversa(basis, piscina, &basis))
        {
            redde FALSUM;
        }
        /* -(exponens + 1) + 1: sine exundatione etiam pro S32 imo */
        modulus = (i32)(-(exponens + I)) + I;
    }
    alioquin
    {
        modulus = (i32)exponens;
    }
    /* mdc(n, d) = 1 => mdc(n^k, d^k) = 1: nulla reductio */
    *exitus = _ex_partibus(magnus_potentia(basis.numerator, modulus,
        piscina), magnus_potentia(basis.denominator, modulus, piscina));
    redde VERUM;
}


/* ==================================================
 * Ad integros
 * ================================================== */

/* divisio Euclidea per denominatorem positivum: quotiens est
 * pavimentum */
Magnus
fractio_pavimentum (
     Fractio  a,
     Piscina* piscina)
{
    redde _quotiens(a.numerator, a.denominator, piscina);
}

Magnus
fractio_tectum (
     Fractio  a,
     Piscina* piscina)
{
    Magnus quotiens = magnus_ex_s64(ZEPHYRUM);
    Magnus residuum = magnus_ex_s64(ZEPHYRUM);

    (vacuum)magnus_divide(a.numerator, a.denominator, piscina,
        &quotiens, &residuum);
    si (magnus_signum(residuum) == ZEPHYRUM)
    {
        redde quotiens;
    }
    redde magnus_adde(quotiens, magnus_ex_s64(I), piscina);
}

/* q = pavimentum, r = residuum (0 <= r < d): 2r < d -> q; 2r > d ->
 * q + 1; 2r = d (dimidium) -> par ex q et q + 1 */
Magnus
fractio_rotunda (
     Fractio  a,
     Piscina* piscina)
{
    Magnus quotiens  = magnus_ex_s64(ZEPHYRUM);
    Magnus residuum  = magnus_ex_s64(ZEPHYRUM);
    Magnus paritas   = magnus_ex_s64(ZEPHYRUM);
       s32 ordo;

    (vacuum)magnus_divide(a.numerator, a.denominator, piscina,
        &quotiens, &residuum);
    ordo = magnus_compara(magnus_adde(residuum, residuum, piscina),
        a.denominator);
    si (ordo < ZEPHYRUM)
    {
        redde quotiens;
    }
    si (ordo == ZEPHYRUM)
    {
        (vacuum)magnus_divide(quotiens, magnus_ex_s64(II), piscina,
            NIHIL, &paritas);
        si (magnus_signum(paritas) == ZEPHYRUM)
        {
            redde quotiens;
        }
    }
    redde magnus_adde(quotiens, magnus_ex_s64(I), piscina);
}
#undef _est_unum
#undef _ex_partibus
#undef _quotiens
/* lib/polynomium.c: statica per plagulam renominata */
#define _alveus _alveus_polynomium
#define _apex_notare _apex_notare_polynomium
#define _apex_officinarum _apex_officinarum_polynomium
#define _est_digitus _est_digitus_polynomium
#define _ex_alveo _ex_alveo_polynomium
#define _intra _intra_polynomium
#define _modulus_maximus _modulus_maximus_polynomium
#define _officina_reficere _officina_reficere_polynomium
#define _officinae_aperire _officinae_aperire_polynomium
#define _officinae_claudere _officinae_claudere_polynomium
#define _officinis_utendum _officinis_utendum_polynomium
#define _primitiva_positiva _primitiva_positiva_polynomium
#define _pseudo_residuum _pseudo_residuum_polynomium
#define _servare _servare_polynomium
#define _summa _summa_polynomium
#define _summus _summus_polynomium
#define _transili _transili_polynomium
#define Officinae Officinae_polynomium
#line 1 "lib/polynomium.c"
/* polynomium.c - Polynomia Laurentiana exacta super magnum
 *
 * Densa: coefficientes[i] pro t^(imus + i). Fines exponentium
 * (|e| <= 2^30 - 1) efficiunt ut omnis amplitudo (summus - imus + 1)
 * in i32 capiat et omne productum exponentis in s64 - ergo probationes
 * finium ipsae sine exundatione computantur. Divisio exacta: longa ex
 * summo, quotiens partialis per coefficientem ducem divisibilis esse
 * debet, residuum nullum. Vide lib/polynomium.worklog.md.
 */



/* Officinae: piscinae temporariae pro summis partialibus (recensio
 * polynomium-I, A3: multiplicatio CC x CC terminorum C digitorum VI.VI
 * MB in piscina vocantis relinquebat pro XLII KB effectus). Solum si
 * na * nb >= LXIV ET summa partialis s64 relinquere potest (vide
 * _officinis_utendum): aliter via celeris magni nihil allocat et
 * officinae tempus solum constant (recensio polynomium-II: 8 x 8
 * coefficientium parvorum 0.42 -> 0.80 us - regimen ipsum polynomiorum
 * nodorum). Divisio: residua per quotientem crescere possunt; criterium
 * idem in dividendo et divisore, casus rarus residuorum magnorum ex
 * operandis parvis in piscina vocantis manet (ut olim). */
#define POLYNOMIUM_LIMES_OFFICINARUM LXIV
#define POLYNOMIUM_S64_SUMMUS ((s64)0x7FFFFFFFFFFFFFFFLL)


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_intra (
    s64 exponens)
{
    redde exponens >= -(s64)POLYNOMIUM_EXPONENS_MAXIMUS
        && exponens <= (s64)POLYNOMIUM_EXPONENS_MAXIMUS;
}

interior s64
_summus (
    Polynomium p)
{
    redde (s64)p.imus + (s64)p.numerus - I;
}

/* numerus coefficientium nullorum */
interior Magnus*
_alveus (
    Piscina* piscina,
        i32  numerus)
{
    Magnus* c = (Magnus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(Magnus));
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        c[k] = magnus_ex_s64(ZEPHYRUM);
    }
    redde c;
}

/* forma canonica: zephyra extrema absciduntur (imus crescit) */
interior Polynomium
_ex_alveo (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus)
{
    Polynomium p;
           i32 primus = ZEPHYRUM;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        redde polynomium_nullum();
    }
    dum (magnus_signum(c[numerus - I]) == ZEPHYRUM)
    {
        numerus--;
    }
    p.coefficientes  = c + primus;
    p.numerus        = numerus - primus;
    p.imus           = (s32)((s64)imus + (s64)primus);
    redde p;
}

interior b32
_est_digitus (
    i8 c)
{
    redde c >= '0' && c <= '9';
}

interior i32
_transili (
    chorda textus,
       i32 k)
{
    dum (   k < textus.mensura && (textus.datum[k] == ' '
        || textus.datum[k] == '\t'))
    {
        k++;
    }
    redde k;
}


/* Duae piscinae temporariae cum notis initialibus. Si creari non
 * possunt (aut operatio parva est), ambae = piscina vocantis et
 * refectio nihil agit: effectus idem, memoria ut olim. */
nomen structura {
           Piscina* piscinae[II];
    PiscinaNotatio  notae[II];
               b32  propriae;
} Officinae;

/* DIAGNOSIS: maximus usus officinae in operatione ultima */
interior memoriae_index _apex_officinarum = ZEPHYRUM;

interior vacuum
_apex_notare (
    Piscina* officina)
{
    memoriae_index usus = piscina_summa_usus(officina);

    si (usus > _apex_officinarum)
    {
        _apex_officinarum = usus;
    }
}

interior vacuum
_officinae_aperire (
     Officinae* o,
       Piscina* vocantis,
           b32  utendae)
{
    o->piscinae[ZEPHYRUM]  = vocantis;
    o->piscinae[I]         = vocantis;
    o->propriae            = FALSUM;
    _apex_officinarum      = ZEPHYRUM;
    si (!utendae)
    {
        redde;
    }
    o->piscinae[ZEPHYRUM] =
        piscina_generare_dynamicum("polynomium_officina",
        (memoriae_index)4096);
    o->piscinae[I] = piscina_generare_dynamicum("polynomium_officina",
        (memoriae_index)4096);
    si (o->piscinae[ZEPHYRUM] == NIHIL || o->piscinae[I] == NIHIL)
    {
        si (o->piscinae[ZEPHYRUM])
        {
            piscina_destruere(o->piscinae[ZEPHYRUM]);
        }
        si (o->piscinae[I])
        {
            piscina_destruere(o->piscinae[I]);
        }
        o->piscinae[ZEPHYRUM]  = vocantis;
        o->piscinae[I]         = vocantis;
        redde;
    }
    o->notae[ZEPHYRUM]  = piscina_notare(o->piscinae[ZEPHYRUM]);
    o->notae[I]         = piscina_notare(o->piscinae[I]);
    o->propriae         = VERUM;
}

interior vacuum
_officina_reficere (
     Officinae* o,
           i32  index)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[index]);
        piscina_reficere(o->piscinae[index], o->notae[index]);
    }
}

interior vacuum
_officinae_claudere (
    Officinae* o)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[ZEPHYRUM]);
        _apex_notare(o->piscinae[I]);
        piscina_destruere(o->piscinae[ZEPHYRUM]);
        piscina_destruere(o->piscinae[I]);
    }
}

/* maximus |c| si omnes |c| < 2^31 (producta in via celeri magni);
 * -1 si coefficiens aliquis maior */
interior s64
_modulus_maximus (
    Polynomium p)
{
    s64 maximus = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        s64 valor;

        si (!magnus_ad_s64(p.coefficientes[k], &valor))
        {
            redde -I;
        }
        si (valor < ZEPHYRUM)
        {
            valor = -valor;
        }
        si (valor > (s64)0x7FFFFFFFL)
        {
            redde -I;
        }
        si (valor > maximus)
        {
            maximus = valor;
        }
    }
    redde maximus;
}

/* officinae nisi nulla summa partialis s64 relinquere potest:
 * max|a| max|b| min(na, nb) < 2^63 (producta < 2^62, summae minus quam
 * min(na, nb) productorum). Coefficientes < 2^31 soli non sufficiunt:
 * summa duorum productorum ~2^62 iam exundat et allocat (recensio
 * polynomium-II, planta "scrutatio omissa"). */
interior b32
_officinis_utendum (
    Polynomium a,
    Polynomium b,
           s64 opera)
{
    s64 maximus_a;
    s64 maximus_b;
    s64 minimus_terminorum;

    si (opera < (s64)POLYNOMIUM_LIMES_OFFICINARUM)
    {
        redde FALSUM;
    }
    maximus_a = _modulus_maximus(a);
    maximus_b = _modulus_maximus(b);
    si (maximus_a < ZEPHYRUM || maximus_b < ZEPHYRUM)
    {
        redde VERUM;
    }
    minimus_terminorum = a.numerus < b.numerus ? (s64)a.numerus
        : (s64)b.numerus;
    redde maximus_a * maximus_b > POLYNOMIUM_S64_SUMMUS
        / minimus_terminorum;
}

/* valor ex officina in piscinam vocantis servandus */
interior Magnus
_servare (
    constans Officinae* o,
                Magnus  valor,
               Piscina* piscina)
{
    redde o->propriae ? magnus_transcribe(valor, piscina) : valor;
}


/* ==================================================
 * Constructio et textus
 * ================================================== */

Polynomium
polynomium_nullum (
    vacuum)
{
    Polynomium p;

    p.coefficientes  = NIHIL;
    p.numerus        = ZEPHYRUM;
    p.imus           = ZEPHYRUM;
    redde p;
}

Polynomium
polynomium_constans (
      Magnus  c,
     Piscina* piscina)
{
    Polynomium p = polynomium_nullum();

    (vacuum)polynomium_monomium(c, ZEPHYRUM, piscina, &p);
    redde p;
}

b32
polynomium_monomium (
        Magnus  c,
           s32  exponens,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* alveus;

    si (!_intra((s64)exponens))
    {
        redde FALSUM;
    }
    si (magnus_signum(c) == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    alveus                 = _alveus(piscina, I);
    alveus[ZEPHYRUM]       = c;
    exitus->coefficientes  = alveus;
    exitus->numerus        = I;
    exitus->imus           = exponens;
    redde VERUM;
}

b32
polynomium_ex_coefficientibus (
     constans Magnus* c,
                 i32  numerus,
                 s32  imus,
             Piscina* piscina,
          Polynomium* exitus)
{
       i32  primus = ZEPHYRUM;
       i32  ultimus;
       i32  k;
    Magnus* alveus;

    dum (primus < numerus && magnus_signum(c[primus]) == ZEPHYRUM)
    {
        primus++;
    }
    si (primus == numerus)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    ultimus = numerus - I;
    dum (magnus_signum(c[ultimus]) == ZEPHYRUM)
    {
        ultimus--;
    }
    si (   !_intra((s64)imus + (s64)primus)
        || !_intra((s64)imus + (s64)ultimus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, ultimus - primus + I);
    per (k = primus; k <= ultimus; k++)
    {
        alveus[k - primus] = c[k];
    }
    exitus->coefficientes  = alveus;
    exitus->numerus        = ultimus - primus + I;
    exitus->imus           = (s32)((s64)imus + (s64)primus);
    redde VERUM;
}

b32
polynomium_ex_chorda (
        chorda  textus,
     character  littera,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* valores;
       s64* exponentes;
       i32  numerus_terminorum = ZEPHYRUM;
       i32  k;
       s64  imus;
       s64  summus;
    Magnus* alveus;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* littera ASCII tantum: byte >= 0x80 solus UTF-8 non est, et
     * character signatus cum i8 non congruit (recensio polynomium-I,
     * A1) */
    si (!(   (littera >= 'a' && littera <= 'z')
          || (littera >= 'A' && littera <= 'Z')))
    {
        redde FALSUM;
    }
    /* quisque terminus saltem characterem unum consumit */
    valores     = _alveus(piscina, textus.mensura);
    exponentes  = (s64*)piscina_allocare(piscina,
        (memoriae_index)textus.mensura * magnitudo(s64));

    k = _transili(textus, ZEPHYRUM);
    dum (VERUM)
    {
           s32 signum               = I;
           b32 habet_coefficientem  = FALSUM;
           b32 habet_litteram       = FALSUM;
        Magnus valor                = magnus_ex_s64(I);
           s64 exponens             = ZEPHYRUM;
           i32 initium;

        si (numerus_terminorum > ZEPHYRUM)
        {
            si (k == textus.mensura)
            {
                frange;
            }
            si (textus.datum[k] == '-')
            {
                signum = -I;
            }
            alioquin si (textus.datum[k] != '+')
            {
                redde FALSUM;
            }
            k = _transili(textus, k + I);
        }
        alioquin si (k < textus.mensura && textus.datum[k] == '-')
        {
            signum  = -I;
            k       = _transili(textus, k + I);
        }

        initium = k;
        dum (k < textus.mensura && _est_digitus(textus.datum[k]))
        {
            k++;
        }
        si (k > initium)
        {
            si (!magnus_ex_chorda(chorda_sectio(textus, initium, k),
                piscina, &valor))
            {
                redde FALSUM;
            }
            habet_coefficientem  = VERUM;
            k                    = _transili(textus, k);
        }

        si (k < textus.mensura && textus.datum[k] == (i8)littera)
        {
            habet_litteram  = VERUM;
            exponens        = I;
            k++;
            si (k < textus.mensura && textus.datum[k] == '^')
            {
                b32 negativus = FALSUM;

                k++;
                si (k < textus.mensura && textus.datum[k] == '-')
                {
                    negativus = VERUM;
                    k++;
                }
                initium   = k;
                exponens  = ZEPHYRUM;
                dum (   k < textus.mensura
                     && _est_digitus(textus.datum[k]))
                {
                    exponens = exponens * X
                        + (s64)(textus.datum[k] - '0');
                    si (exponens > (s64)POLYNOMIUM_EXPONENS_MAXIMUS)
                    {
                        redde FALSUM;
                    }
                    k++;
                }
                si (k == initium)
                {
                    redde FALSUM;
                }
                si (negativus)
                {
                    exponens = -exponens;
                }
            }
            k = _transili(textus, k);
        }

        si (!habet_coefficientem && !habet_litteram)
        {
            redde FALSUM;
        }
        si (signum < ZEPHYRUM)
        {
            valor = magnus_nega(valor, piscina);
        }
        valores[numerus_terminorum]     = valor;
        exponentes[numerus_terminorum]  = exponens;
        numerus_terminorum++;
    }

    /* termini in alveum densum; exponentes iterati coniunguntur */
    imus    = exponentes[ZEPHYRUM];
    summus  = exponentes[ZEPHYRUM];
    per (k = I; k < numerus_terminorum; k++)
    {
        si (exponentes[k] < imus)
        {
            imus = exponentes[k];
        }
        si (exponentes[k] > summus)
        {
            summus = exponentes[k];
        }
    }
    si (summus - imus + I > (s64)POLYNOMIUM_AMPLITUDO_LECTIONIS)
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (k = ZEPHYRUM; k < numerus_terminorum; k++)
    {
        i32 locus = (i32)(exponentes[k] - imus);

        alveus[locus] = magnus_adde(alveus[locus], valores[k], piscina);
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

Polynomium
polynomium_transcribe (
    Polynomium  p,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM)
    {
        redde p;
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = magnus_transcribe(p.coefficientes[k], piscina);
    }
    p.coefficientes = alveus;
    redde p;
}

chorda
polynomium_ad_chordam (
    Polynomium  p,
     character  littera,
       Piscina* piscina)
{
     ChordaAedificator* scriba;
                   i32  k;
                   b32  primus  = VERUM;
                Magnus  unum    = magnus_ex_s64(I);

    si (p.numerus == ZEPHYRUM)
    {
        redde chorda_ex_literis("0", piscina);
    }
    scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        Magnus c         = p.coefficientes[k];
           s32 exponens  = (s32)((s64)p.imus + (s64)k);
        Magnus modulus;

        si (magnus_signum(c) == ZEPHYRUM)
        {
            perge;
        }
        si (primus)
        {
            si (magnus_signum(c) < ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '-');
            }
            primus = FALSUM;
        }
        alioquin
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba,
                magnus_signum(c) < ZEPHYRUM ? " - " : " + ");
        }
        modulus = magnus_absolutum(c, piscina);
        si (exponens == ZEPHYRUM || !magnus_aequalis(modulus, unum))
        {
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                magnus_ad_chordam(modulus, piscina));
        }
        si (exponens != ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_character(scriba,
                littera);
            si (exponens != I)
            {
                (vacuum)chorda_aedificator_appendere_character(scriba,
                    '^');
                (vacuum)chorda_aedificator_appendere_s32(scriba,
                    exponens);
            }
        }
    }
    redde chorda_aedificator_finire(scriba);
}


/* ==================================================
 * Lectio
 * ================================================== */

b32
polynomium_est_nullum (
    Polynomium p)
{
    redde p.numerus == ZEPHYRUM;
}

s32
polynomium_gradus_imus (
    Polynomium p)
{
    redde p.imus;
}

s32
polynomium_gradus_summus (
    Polynomium p)
{
    si (p.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s32)_summus(p);
}

Magnus
polynomium_coefficiens (
    Polynomium p,
           s32 exponens)
{
    s64 locus = (s64)exponens - (s64)p.imus;

    si (locus < ZEPHYRUM || locus >= (s64)p.numerus)
    {
        redde magnus_ex_s64(ZEPHYRUM);
    }
    redde p.coefficientes[(i32)locus];
}

b32
polynomium_aequalis (
    Polynomium a,
    Polynomium b)
{
    i32 k;

    si (a.numerus != b.numerus || a.imus != b.imus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        si (!magnus_aequalis(a.coefficientes[k], b.coefficientes[k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Magnus
polynomium_contentum (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g = magnus_ex_s64(ZEPHYRUM);
       i32 k;

    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        g = magnus_divisor_communis(g, p.coefficientes[k], piscina);
    }
    redde g;
}


/* ==================================================
 * Arithmetica
 * ================================================== */

Polynomium
polynomium_nega (
    Polynomium  a,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (a.numerus == ZEPHYRUM)
    {
        redde a;
    }
    alveus = _alveus(piscina, a.numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[k] = magnus_nega(a.coefficientes[k], piscina);
    }
    redde _ex_alveo(alveus, a.numerus, a.imus);
}

/* a + signum * b */
interior Polynomium
_summa (
    Polynomium  a,
    Polynomium  b,
           s32  signum,
       Piscina* piscina)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  numerus;
        i32  k;

    si (b.numerus == ZEPHYRUM)
    {
        redde a;
    }
    si (a.numerus == ZEPHYRUM)
    {
        redde signum > ZEPHYRUM ? b : polynomium_nega(b, piscina);
    }
    imus     = a.imus < b.imus ? (s64)a.imus : (s64)b.imus;
    summus   = _summus(a) > _summus(b) ? _summus(a) : _summus(b);
    numerus  = (i32)(summus - imus + I);
    alveus   = _alveus(piscina, numerus);
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        alveus[(i32)((s64)a.imus - imus) + k] = a.coefficientes[k];
    }
    per (k = ZEPHYRUM; k < b.numerus; k++)
    {
        i32 locus = (i32)((s64)b.imus - imus) + k;

        alveus[locus] = signum > ZEPHYRUM
            ? magnus_adde(alveus[locus], b.coefficientes[k], piscina)
            : magnus_subtrahe(alveus[locus], b.coefficientes[k],
            piscina);
    }
    redde _ex_alveo(alveus, numerus, (s32)imus);
}

Polynomium
polynomium_adde (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, I, piscina);
}

Polynomium
polynomium_subtrahe (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
    redde _summa(a, b, -I, piscina);
}

Polynomium
polynomium_multiplica_scalari (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM || magnus_signum(c) == ZEPHYRUM)
    {
        redde polynomium_nullum();
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = magnus_multiplica(p.coefficientes[k], c, piscina);
    }
    redde _ex_alveo(alveus, p.numerus, p.imus);
}

b32
polynomium_multiplica (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus)
{
      Magnus* alveus;
   Officinae  officinae;
         s64  imus;
         i32  numerus;
         i32  m;

    si (a.numerus == ZEPHYRUM || b.numerus == ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    imus = (s64)a.imus + (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) + _summus(b)))
    {
        redde FALSUM;
    }
    numerus  = a.numerus + b.numerus - I;
    alveus   = _alveus(piscina, numerus);

    _officinae_aperire(&officinae, piscina, _officinis_utendum(a, b,
        (s64)a.numerus * (s64)b.numerus));
    si (!officinae.propriae)
    {
        /* via vocantis: ansa ordinaria (i, j) sine sumptu officinarum -
         * regimen polynomiorum nodorum */
        i32 i;
        i32 j;

        per (i = ZEPHYRUM; i < a.numerus; i++)
        {
            per (j = ZEPHYRUM; j < b.numerus; j++)
            {
                alveus[i + j] = magnus_adde(alveus[i + j],
                    magnus_multiplica(a.coefficientes[i],
                        b.coefficientes[j], piscina), piscina);
            }
        }
    }
    alioquin
    {
        /* coefficiens quisque totus in officina computatur, solus valor
         * finalis in piscinam vocantis transcribitur */
        per (m = ZEPHYRUM; m < numerus; m++)
        {
             Piscina* officina  = officinae.piscinae[ZEPHYRUM];
              Magnus  summa     = magnus_ex_s64(ZEPHYRUM);
                 i32  i         = m + I > b.numerus ? m + I - b.numerus
                     : ZEPHYRUM;

            per (; i < a.numerus && i <= m; i++)
            {
                summa = magnus_adde(summa, magnus_multiplica(
                    a.coefficientes[i], b.coefficientes[m - i],
                    officina),
                    officina);
            }
            alveus[m] = magnus_transcribe(summa, piscina);
            _officina_reficere(&officinae, ZEPHYRUM);
        }
    }
    _officinae_claudere(&officinae);
    *exitus = _ex_alveo(alveus, numerus, (s32)imus);
    redde VERUM;
}

b32
polynomium_potentia (
    Polynomium  p,
           i32  n,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium effectus;
    Polynomium basis     = p;
           i32 reliquum  = n;

    si (n == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| <= 2^30, n < 2^32: productum in s64 capit */
    si (   !_intra((s64)p.imus * (s64)n)
        || !_intra(_summus(p) * (s64)n))
    {
        redde FALSUM;
    }
    /* quadrata intermedia p^(2^k), 2^k <= n: intra fines */
    effectus = polynomium_constans(magnus_ex_s64(I), piscina);
    dum (VERUM)
    {
        si (reliquum & I)
        {
            si (!polynomium_multiplica(effectus, basis, piscina,
                &effectus))
            {
                redde FALSUM;
            }
        }
        reliquum >>= I;
        si (reliquum == ZEPHYRUM)
        {
            frange;
        }
        si (!polynomium_multiplica(basis, basis, piscina, &basis))
        {
            redde FALSUM;
        }
    }
    *exitus = effectus;
    redde VERUM;
}

b32
polynomium_divide_exacte (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* quotiens)
{
      Magnus* residua;
      Magnus* partes;
      Magnus  dux;
   Officinae  officinae;
         i32  hic = ZEPHYRUM;
         s64  imus;
         i32  numerus;
         i32  k;
         i32  j;
         b32  exacta = VERUM;

    si (b.numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (a.numerus == ZEPHYRUM)
    {
        *quotiens = a;
        redde VERUM;
    }
    si (a.numerus < b.numerus)
    {
        redde FALSUM;
    }
    imus = (s64)a.imus - (s64)b.imus;
    si (!_intra(imus) || !_intra(_summus(a) - _summus(b)))
    {
        redde FALSUM;
    }

    /* A = a t^-imus(a), B = b t^-imus(b): termini constantes non
     * nulli (t unitas est); divisio longa in Z[t] ex summo.
     * Fenestra residuorum mutatorum [k, k + nb - 1] gradu quoque tota
     * rescribitur, ergo in officinis ALTERNIS vivit (sicut Euclides
     * magni): gradus in officinam alteram computat, priorem reficit.
     * Partes quotientis in piscinam vocantis transcribuntur. */
    numerus  = a.numerus - b.numerus + I;
    residua  = _alveus(piscina, a.numerus);
    partes   = _alveus(piscina, numerus);
    dux      = b.coefficientes[b.numerus - I];
    per (k = ZEPHYRUM; k < a.numerus; k++)
    {
        residua[k] = a.coefficientes[k];
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(a, b,
        (s64)numerus * (s64)b.numerus));
    per (k = numerus; k-- > ZEPHYRUM;)
    {
         Piscina* illic = officinae.piscinae[I - hic];
          Magnus  pars;
          Magnus  residuum;

        (vacuum)magnus_divide(residua[k + b.numerus - I], dux, illic,
            &pars, &residuum);
        si (magnus_signum(residuum) != ZEPHYRUM)
        {
            exacta = FALSUM;
            frange;
        }
        partes[k] = _servare(&officinae, pars, piscina);
        si (magnus_signum(pars) == ZEPHYRUM)
        {
            _officina_reficere(&officinae, I - hic);
            perge;
        }
        per (j = ZEPHYRUM; j < b.numerus; j++)
        {
            /* coefficiens B internus nullus: x - 0 = x ipse, fortasse
             * in officina reficienda - transcribe in illic */
            si (   officinae.propriae
                && magnus_signum(b.coefficientes[j]) == ZEPHYRUM)
            {
                residua[k + j] = _servare(&officinae, residua[k + j],
                    illic);
                perge;
            }
            residua[k + j] = magnus_subtrahe(residua[k + j],
                magnus_multiplica(pars, b.coefficientes[j], illic),
                illic);
        }
        _officina_reficere(&officinae, hic);
        hic = I - hic;
    }
    /* residuum infra gradum B nullum esse debet (lectum ANTE
     * officinas clausas) */
    per (k = ZEPHYRUM; exacta && k + I < b.numerus; k++)
    {
        si (magnus_signum(residua[k]) != ZEPHYRUM)
        {
            exacta = FALSUM;
        }
    }
    _officinae_claudere(&officinae);
    si (!exacta)
    {
        redde FALSUM;
    }
    *quotiens = _ex_alveo(partes, numerus, (s32)imus);
    redde VERUM;
}


/* ==================================================
 * Substitutiones
 * ================================================== */

b32
polynomium_translata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
    (vacuum)piscina;
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (   !_intra((s64)p.imus + (s64)k)
        || !_intra(_summus(p) + (s64)k))
    {
        redde FALSUM;
    }
    *exitus       = p;
    exitus->imus  = (s32)((s64)p.imus + (s64)k);
    redde VERUM;
}

b32
polynomium_dilata (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* |exponens| < 2^30, |k| <= 2^31: productum in s64 capit */
    imus    = (s64)p.imus * (s64)k;
    summus  = _summus(p) * (s64)k;
    si (k < ZEPHYRUM)
    {
        s64 t = imus;

        imus    = summus;
        summus  = t;
    }
    si (!_intra(imus) || !_intra(summus))
    {
        redde FALSUM;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens = ((s64)p.imus + (s64)i) * (s64)k;

        alveus[(i32)(exponens - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_contrahe (
    Polynomium  p,
           s32  k,
       Piscina* piscina,
    Polynomium* exitus)
{
     Magnus* alveus;
        s64  modulus_k;
        s64  imus;
        s64  summus;
        i32  i;

    si (k == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    /* divisibilitas per modulos non negativos (C89 '%' cum negativis
     * definitionem non habet) */
    modulus_k = k < ZEPHYRUM ? -(s64)k : (s64)k;
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;

        si (   magnus_signum(p.coefficientes[i]) != ZEPHYRUM
            && modulus % modulus_k               != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    /* quotientes exacti: e / k = signum * (|e| / |k|); |e/k| <= |e|,
     * ergo intra fines */
    imus    = (s64)p.imus;
    summus  = _summus(p);
    imus    = (imus < ZEPHYRUM ? -((-imus) / modulus_k) : imus
        / modulus_k);
    summus  = (summus < ZEPHYRUM ? -((-summus) / modulus_k) : summus
        / modulus_k);
    si (k < ZEPHYRUM)
    {
        s64 t = -imus;

        imus    = -summus;
        summus  = t;
    }
    alveus = _alveus(piscina, (i32)(summus - imus + I));
    per (i = ZEPHYRUM; i < p.numerus; i++)
    {
        s64 exponens  = (s64)p.imus + (s64)i;
        s64 modulus   = exponens < ZEPHYRUM ? -exponens : exponens;
        s64 novus;

        si (magnus_signum(p.coefficientes[i]) == ZEPHYRUM)
        {
            perge;
        }
        novus = modulus / modulus_k;
        si ((exponens < ZEPHYRUM) != (k < ZEPHYRUM))
        {
            novus = -novus;
        }
        alveus[(i32)(novus - imus)] = p.coefficientes[i];
    }
    *exitus = _ex_alveo(alveus, (i32)(summus - imus + I), (s32)imus);
    redde VERUM;
}

b32
polynomium_normale (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium q;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = p;
        redde VERUM;
    }
    si (!polynomium_translata(p, -p.imus, piscina, &q))
    {
        redde FALSUM;
    }
    si (magnus_signum(q.coefficientes[ZEPHYRUM]) < ZEPHYRUM)
    {
        q = polynomium_nega(q, piscina);
    }
    *exitus = q;
    redde VERUM;
}

Polynomium
polynomium_inversum (
    Polynomium  p,
       Piscina* piscina)
{
     Magnus* alveus;
        i32  k;

    si (p.numerus == ZEPHYRUM)
    {
        redde p;
    }
    alveus = _alveus(piscina, p.numerus);
    per (k = ZEPHYRUM; k < p.numerus; k++)
    {
        alveus[k] = p.coefficientes[p.numerus - I - k];
    }
    /* t^e -> t^-e: summus fit -imus; |e| <= MAXIMUS utrimque */
    redde _ex_alveo(alveus, p.numerus, (s32)(-_summus(p)));
}

b32
polynomium_est_symmetricum (
    Polynomium p)
{
    i32 k;

    per (k = ZEPHYRUM; k < p.numerus / II; k++)
    {
        si (!magnus_aequalis(p.coefficientes[k],
            p.coefficientes[p.numerus - I - k]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Valor
 * ================================================== */

b32
polynomium_valor (
    Polynomium  p,
       Fractio  x,
       Piscina* piscina,
       Fractio* exitus)
{
    Fractio valor = fractio_ex_s64(ZEPHYRUM);
    Fractio factor_imus;
        i32 k;

    si (p.numerus == ZEPHYRUM)
    {
        *exitus = valor;
        redde VERUM;
    }
    si (!fractio_potentia(x, p.imus, piscina, &factor_imus))
    {
        redde FALSUM;   /* x = 0, exponens negativus */
    }
    per (k = p.numerus; k-- > ZEPHYRUM;)
    {
        valor = fractio_adde(fractio_multiplica(valor, x, piscina),
            fractio_ex_magno(p.coefficientes[k]), piscina);
    }
    *exitus = fractio_multiplica(valor, factor_imus, piscina);
    redde VERUM;
}


/* ==================================================
 * Diagnosis
 * ================================================== */

memoriae_index
polynomium_apex_officinarum (
    vacuum)
{
    redde _apex_officinarum;
}


/* ==================================================
 * Derivata, divisor communis, pars libera, translatio Taylor
 * ================================================== */

b32
polynomium_derivata (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Magnus* c;
       s32  imus;
       s32  summus;
       s32  e;

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    imus    = polynomium_gradus_imus(p);
    summus  = polynomium_gradus_summus(p);
    si (imus - I < -POLYNOMIUM_EXPONENS_MAXIMUS)
    {
        redde FALSUM;
    }
    c = (Magnus*)piscina_allocare(piscina, (memoriae_index)(summus
        - imus
        + I) * magnitudo(Magnus));
    per (e = imus; e <= summus; e++)
    {
        c[e - imus] = magnus_multiplica(polynomium_coefficiens(p, e),
            magnus_ex_s64((s64)e), piscina);
    }
    redde polynomium_ex_coefficientibus(c, (i32)(summus - imus + I),
        imus - I, piscina, exitus);
}

/* p / contentum, coefficiens summus > 0 */
interior Polynomium
_primitiva_positiva (
    Polynomium  p,
       Piscina* piscina)
{
    Magnus g;

    si (polynomium_est_nullum(p))
    {
        redde p;
    }
    g = polynomium_contentum(p, piscina);
    si (magnus_signum(polynomium_coefficiens(p,
            polynomium_gradus_summus(p))) < ZEPHYRUM)
    {
        g = magnus_nega(g, piscina);
    }
    si (magnus_compara(g, magnus_ex_s64(I)) != ZEPHYRUM)
    {
        (vacuum)polynomium_divide_exacte(p, polynomium_constans(g,
            piscina), piscina, &p);
    }
    redde p;
}

/* pseudo-residuum scala POSITIVA |lc(b)|^k a mod b (extensio.c _residuum;
 * limes structuralis passuum) */
interior Polynomium
_pseudo_residuum (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina)
{
       s32 gradus_b   = polynomium_gradus_summus(b);
    Magnus lc_b       = polynomium_coefficiens(b, gradus_b);
    Magnus absolutum  = magnus_absolutum(lc_b, piscina);
       s32 signum_b   = magnus_signum(lc_b);
       s32 gradus_initium;
       s32 iteratio;

    si (polynomium_est_nullum(a))
    {
        redde a;
    }
    gradus_initium = polynomium_gradus_summus(a);
    per (iteratio = ZEPHYRUM; iteratio <= gradus_initium - gradus_b
        && !polynomium_est_nullum(a)
        && polynomium_gradus_summus(a) >= gradus_b; iteratio++)
    {
               s32 gradus_a   = polynomium_gradus_summus(a);
            Magnus lc_a       = polynomium_coefficiens(a, gradus_a);
        Polynomium translata  = polynomium_nullum();

        (vacuum)polynomium_translata(b, gradus_a - gradus_b, piscina,
            &translata);
        si (signum_b < ZEPHYRUM)
        {
            lc_a = magnus_nega(lc_a, piscina);
        }
        a = polynomium_subtrahe(polynomium_multiplica_scalari(a,
            absolutum, piscina),
            polynomium_multiplica_scalari(translata,
            lc_a, piscina), piscina);
    }
    redde a;
}

b32
polynomium_divisor_communis (
    Polynomium  a,
    Polynomium  b,
       Piscina* piscina,
    Polynomium* exitus)
{
    s32 iteratio;
    s32 limes;

    si (   (!polynomium_est_nullum(a)
        && polynomium_gradus_imus(a) < ZEPHYRUM)
        || (!polynomium_est_nullum(b)
        && polynomium_gradus_imus(b) < ZEPHYRUM))
    {
        redde FALSUM;
    }
    a = _primitiva_positiva(a, piscina);
    b = _primitiva_positiva(b, piscina);
    si (   !polynomium_est_nullum(a) && !polynomium_est_nullum(b)
        && polynomium_gradus_summus(a) < polynomium_gradus_summus(b))
    {
        Polynomium t = a;

        a = b;
        b = t;
    }
    /* gradus b strictim decrescit: limes structuralis */
    limes = polynomium_est_nullum(b) ? ZEPHYRUM
        : polynomium_gradus_summus(b) + II;
    per (iteratio = ZEPHYRUM; iteratio < limes
        && !polynomium_est_nullum(b); iteratio++)
    {
        Polynomium r = _primitiva_positiva(_pseudo_residuum(a, b,
            piscina),
            piscina);

        a = b;
        b = r;
    }
    si (!polynomium_est_nullum(b))
    {
        redde FALSUM;               /* non accidit: limes structuralis */
    }
    *exitus = _primitiva_positiva(a, piscina);
    redde VERUM;
}

b32
polynomium_pars_libera (
    Polynomium  p,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium derivata  = polynomium_nullum();
    Polynomium g         = polynomium_nullum();
    Polynomium q         = polynomium_nullum();

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    si (polynomium_gradus_imus(p) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (polynomium_gradus_summus(p) == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (   !polynomium_derivata(p, piscina, &derivata)
        || !polynomium_divisor_communis(p, derivata, piscina, &g)
        || !polynomium_divide_exacte(_primitiva_positiva(p, piscina), g,
        piscina, &q))
    {
        redde FALSUM;
    }
    *exitus = _primitiva_positiva(q, piscina);
    redde VERUM;
}

b32
polynomium_translatum (
    Polynomium  p,
        Magnus  c,
       Piscina* piscina,
    Polynomium* exitus)
{
       s32  n;
       s32  i;
       s32  j;
       b32  unus;
    Magnus* a;

    si (polynomium_est_nullum(p))
    {
        *exitus = p;
        redde VERUM;
    }
    si (polynomium_gradus_imus(p) < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* translatio Taylor in loco (Horner iteratus): pro i = 0..n-1, pro j
     * = n-1 .. i: a_j += c a_(j+1). Additiones solae si c = 1 (VCA);
     * olim Horner per polynomium_multiplica, polynomium novum in omni
     * gradu (worklog 2026-10-10) */
    n = polynomium_gradus_summus(p);
    a = (Magnus*)piscina_allocare(piscina, (memoriae_index)(n + I)
        * magnitudo(Magnus));
    per (i = ZEPHYRUM; i <= n; i++)
    {
        a[i] = polynomium_coefficiens(p, i);
    }
    unus = magnus_compara(c, magnus_ex_s64(I)) == ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = n - I; j >= i; j--)
        {
            a[j] = magnus_adde(a[j], unus ? a[j + I]
                : magnus_multiplica(c, a[j + I], piscina), piscina);
        }
    }
    redde polynomium_ex_coefficientibus(a, (i32)(n + I), ZEPHYRUM,
        piscina,
        exitus);
}
#undef _alveus
#undef _apex_notare
#undef _apex_officinarum
#undef _est_digitus
#undef _ex_alveo
#undef _intra
#undef _modulus_maximus
#undef _officina_reficere
#undef _officinae_aperire
#undef _officinae_claudere
#undef _officinis_utendum
#undef _primitiva_positiva
#undef _pseudo_residuum
#undef _servare
#undef _summa
#undef _summus
#undef _transili
#undef Officinae
#undef POLYNOMIUM_LIMES_OFFICINARUM
#undef POLYNOMIUM_S64_SUMMUS
/* lib/interpolatio.c: statica per plagulam renominata */
#define _bita_infra _bita_infra_interpolatio
#define _divisor_communis _divisor_communis_interpolatio
#define _horner _horner_interpolatio
#define _primum_aestimare _primum_aestimare_interpolatio
#define _primus_aptus _primus_aptus_interpolatio
#define _primus_proximus _primus_proximus_interpolatio
#define _quaestio_valida _quaestio_valida_interpolatio
#line 1 "lib/interpolatio.c"
/* interpolatio.c - Aestimatio, interpolatio Lagrange modulo p,
 * restitutio Sinica trans primos
 *
 * Lagrange per polynomium magistrum M(t) = prod (t - x_i) et divisionem
 * syntheticam M / (t - x_i): O(n^2) operationes, nulla matrix.
 * Restitutio per congruentia_restitue (Garner, symmetricus). Primi
 * PRIMUM enumerantur (filtro residui) donec summa floor(log2 p) >=
 * limes + 2, ita productum > 2^(limes + 1) certe; deinde unus
 * additus. Vide lib/interpolatio.worklog.md.
 */





/* ==================================================
 * Interpolatio Lagrange modulo p
 * ================================================== */

b32
interpolatio_lagrange_modulo (
    constans i32* x,
    constans i32* y,
             i32  numerus,
             i32  p,
         Piscina* piscina,
             i32* coefficientes)
{
    i32* magister;      /* M(t), numerus + 1 coefficientes */
    i32* quotiens;      /* M(t) / (t - x_i), numerus coefficientes */
    i32* summa;
    i32  i;
    i32  j;

    si (numerus == ZEPHYRUM || !congruentia_est_primus(p))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (x[i] >= p || y[i] >= p)
        {
            redde FALSUM;
        }
    }
    magister  = (i32*)piscina_allocare(piscina, (memoriae_index)(numerus
        + I) * magnitudo(i32));
    quotiens  = (i32*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(i32));
    summa     = (i32*)piscina_allocare(piscina, (memoriae_index)numerus
        * magnitudo(i32));
    per (j = ZEPHYRUM; j <= numerus; j++)
    {
        magister[j] = ZEPHYRUM;
    }
    magister[ZEPHYRUM] = I;
    /* M <- M (t - x_i): gradus crescit unum per punctum */
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        per (j = i + I; j > ZEPHYRUM; j--)
        {
            magister[j] = congruentia_subtrahe(magister[j - I],
                congruentia_multiplica(x[i], magister[j], p), p);
        }
        magister[ZEPHYRUM] = congruentia_subtrahe(ZEPHYRUM,
            congruentia_multiplica(x[i], magister[ZEPHYRUM], p), p);
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        summa[j] = ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i32 differentiae = ZEPHYRUM; /* q(x_i) = prod_(j != i) (x_i - x_j) */
        i32 inversa;
        i32 pondus;

        /* divisio synthetica: q[n-1] = M[n], q[j-1] = M[j] + x_i q[j] */
        quotiens[numerus - I] = magister[numerus];
        per (j = numerus - I; j > ZEPHYRUM; j--)
        {
            quotiens[j - I] = congruentia_adde(magister[j],
                congruentia_multiplica(x[i], quotiens[j], p), p);
        }
        per (j = numerus; j > ZEPHYRUM; j--)
        {
            differentiae =
                congruentia_adde(congruentia_multiplica(differentiae,
                x[i], p), quotiens[j - I], p);
        }
        si (!congruentia_inversa(differentiae, p, &inversa))
        {
            redde FALSUM;           /* productum nullum: puncta iterata */
        }
        pondus = congruentia_multiplica(y[i], inversa, p);
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            summa[j] = congruentia_adde(summa[j],
                congruentia_multiplica(pondus, quotiens[j], p), p);
        }
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        coefficientes[j] = summa[j];
    }
    redde VERUM;
}


/* ==================================================
 * Aestimatio et restitutio
 * ================================================== */

InterpolatioQuaestio
interpolatio_quaestio_ordinaria (
    vacuum)
{
    InterpolatioQuaestio q;

    q.gradus_imus     = ZEPHYRUM;
    q.gradus_summus   = ZEPHYRUM;
    q.passus          = I;
    q.limes_bitorum   = ZEPHYRUM;
    q.residuum_primi  = ZEPHYRUM;
    q.modulus_primi   = ZEPHYRUM;
    q.primus_limes    = (i32)0xFFFFFFFFUL;
    q.puncta_addita   = I;
    q.conatus_maximi  = (i32)0x100000UL;
    redde q;
}

/* floor(log2 p), p >= 2 */
interior i32
_bita_infra (
    i32 p)
{
    i32 bita = ZEPHYRUM;

    dum (p > I)
    {
        p >>= I;
        bita++;
    }
    redde bita;
}

/* maximus divisor communis; (0, 0) -> 0 */
interior i32
_divisor_communis (
    i32 a,
    i32 b)
{
    dum (b != ZEPHYRUM)
    {
        i32 t = a % b;

        a = b;
        b = t;
    }
    redde a;
}

/* primus p satis y distincta dat: y = x^passus, x != 0, sumit (p - 1) /
 * mdc(passus, p - 1) valores; minus quam necessaria -> primus omittitur
 * (aliter conatus toti consumerentur) */
interior b32
_primus_aptus (
    constans InterpolatioQuaestio* q,
                              i32  p,
                              s64  necessaria)
{
    redde (s64)((p - I) / _divisor_communis(q->passus, p - I))
        >= necessaria;
}

/* primus proximus < limes: filtro residui (gradibus moduli, non omnes
 * impares - filtrum admissibile a vocante probatum) et aptus; 0 si
 * nullus. Idem semper pro eisdem argumentis (duo transitus). */
interior i32
_primus_proximus (
    constans InterpolatioQuaestio* q,
                              i32  limes,
                              s64  necessaria)
{
    si (q->modulus_primi < II)
    {
        i32 p = limes;

        per (;;)
        {
            p = congruentia_primus_infra(p);
            si (p == ZEPHYRUM)
            {
                redde ZEPHYRUM;
            }
            si (_primus_aptus(q, p, necessaria))
            {
                redde p;
            }
        }
    }
    alioquin
    {
        s64 m = (s64)q->modulus_primi;
        s64 r = (s64)(q->residuum_primi % q->modulus_primi);
        s64 n;

        si (limes == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        /* maximus n < limes cum n = r (mod m) */
        n = (s64)limes - (s64)I;
        n = n - ((n % m - r + m) % m);
        dum (n >= (s64)II)
        {
            si (   congruentia_est_primus((i32)n)
                && _primus_aptus(q, (i32)n, necessaria))
            {
                redde (i32)n;
            }
            n -= m;
        }
        redde ZEPHYRUM;
    }
}

/* polynomium (coefficientes c[0..k-1] in y) in y mod p, Horner */
interior i32
_horner (
    constans i32* c,
             i32  k,
             i32  y,
             i32  p)
{
    i32 valor = ZEPHYRUM;
    i32 j;

    per (j = k; j > ZEPHYRUM; j--)
    {
        valor = congruentia_adde(congruentia_multiplica(valor, y, p),
            c[j - I], p);
    }
    redde valor;
}

/* coefficientes g in y = x^passus pro uno primo: puncta colliguntur
 * (x == 0 et y iteratum omissa; y iteratum valorem eundem dare debet),
 * Lagrange in primis K, addita probantur. FALSUM si ERRATUM, punctum
 * aut valor >= p, conatus exhausti, allocatio deficit aut probatio
 * discordat. */
interior b32
_primum_aestimare (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                              i32  numerus_coefficientium,
                              i32  p,
                          Piscina* piscina,
                              i32* coefficientes)
{
    i32  necessaria = numerus_coefficientium + q->puncta_addita;
    i32* puncta_y   = (i32*)piscina_conari_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32* valores_g  = (i32*)piscina_conari_allocare(piscina,
        (memoriae_index)necessaria * magnitudo(i32));
    i32 collecta   = ZEPHYRUM;
    i32 index;
    i32 j;

    si (puncta_y == NIHIL || valores_g == NIHIL)
    {
        redde FALSUM;
    }
    per (index = ZEPHYRUM; index < q->conatus_maximi && collecta
        < necessaria; index++)
    {
        i32 punctum  = ZEPHYRUM;
        i32 valor    = ZEPHYRUM;
        s32 status   = aestimare(contextus, index, p, &punctum, &valor);
        i32 y;
        i32 g;
        b32 iteratum = FALSUM;

        si (status == INTERPOLATIO_INUTILE)
        {
            perge;
        }
        si (status != INTERPOLATIO_UTILE || punctum >= p || valor >= p)
        {
            redde FALSUM;
        }
        si (punctum == ZEPHYRUM)
        {
            perge;                  /* x^passus nihil distinguit */
        }
        y = congruentia_potentia(punctum, (i64)q->passus, p);
        /* g(y) = x^(-imus) f(x) */
        si (q->gradus_imus <= ZEPHYRUM)
        {
            g = congruentia_multiplica(valor, congruentia_potentia(
                punctum, (i64)(-(s64)q->gradus_imus), p), p);
        }
        alioquin
        {
            i32 inversa;

            si (!congruentia_inversa(punctum, p, &inversa))
            {
                redde FALSUM;
            }
            g = congruentia_multiplica(valor, congruentia_potentia(
                inversa, (i64)q->gradus_imus, p), p);
        }
        per (j = ZEPHYRUM; j < collecta; j++)
        {
            si (puncta_y[j] == y)
            {
                si (valores_g[j] != g)
                {
                    redde FALSUM;   /* passus aut fenestra falsa */
                }
                iteratum = VERUM;
            }
        }
        si (iteratum)
        {
            perge;
        }
        puncta_y[collecta]   = y;
        valores_g[collecta]  = g;
        collecta++;
    }
    si (collecta < necessaria)
    {
        redde FALSUM;
    }
    si (!interpolatio_lagrange_modulo(puncta_y, valores_g,
        numerus_coefficientium, p, piscina, coefficientes))
    {
        redde FALSUM;
    }
    per (j = numerus_coefficientium; j < necessaria; j++)
    {
        si (_horner(coefficientes, numerus_coefficientium, puncta_y[j],
            p)
            != valores_g[j])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* quaestio valida? Omnia ante primam aestimationem iudicantur. */
interior b32
_quaestio_valida (
    constans InterpolatioQuaestio* q)
{
    s64 imus     = (s64)q->gradus_imus;
    s64 summus   = (s64)q->gradus_summus;
    s64 maximus  = (s64)POLYNOMIUM_EXPONENS_MAXIMUS;

    si (   q->passus == ZEPHYRUM || q->conatus_maximi == ZEPHYRUM
        || summus < imus || imus < -maximus || summus > maximus
        || q->limes_bitorum > INTERPOLATIO_LIMES_MAXIMUS)
    {
        redde FALSUM;
    }
    si ((summus - imus) % (s64)q->passus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   (summus - imus) / (s64)q->passus + (s64)I
        + (s64)q->puncta_addita
        > (s64)0x7FFFFFFFL)
    {
        redde FALSUM;
    }
    si (   q->modulus_primi >= II
        && _divisor_communis(q->residuum_primi % q->modulus_primi,
        q->modulus_primi)
        != I)
    {
        redde FALSUM;               /* filtrum nullos primos (praeter
                                     * fortasse unum) admittit */
    }
    redde VERUM;
}

b32
interpolatio_polynomium (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* q,
                          Piscina* piscina,
                       Polynomium* exitus)
{
       i32  numerus_coefficientium;
       s64  necessaria;
       i32  numerus_primorum;      /* primi restitutionis, sine addito */
       i32* primi;
       i32* residua;               /* [primus][coefficiens] */
       i32* columna;
    Magnus* coefficientes;
    Magnus* densa;
       s64  bita;
       s64  primus_nonnullus;
       s64  ultimus_nonnullus;
       s64  amplitudo;
       i32  p;
       i32  j;
       i32  k;

    si (aestimare == NIHIL || !_quaestio_valida(q))
    {
        redde FALSUM;
    }
    numerus_coefficientium = (i32)(((s64)q->gradus_summus
        - (s64)q->gradus_imus) / (s64)q->passus + (s64)I);
    necessaria = (s64)numerus_coefficientium + (s64)q->puncta_addita;

    /* primi: summa floor(log2 p) >= limes + 1 => productum >= 2^(limes +
     * 1) > 2 max |c| (|c| <= 2^limes - 1); deinde unus additus */
    numerus_primorum  = ZEPHYRUM;
    bita              = ZEPHYRUM;
    p                 = q->primus_limes;
    dum (bita < (s64)q->limes_bitorum + (s64)I)
    {
        p = _primus_proximus(q, p, necessaria);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        bita += (s64)_bita_infra(p);
        numerus_primorum++;
    }
    primi = (i32*)piscina_allocare(piscina, (memoriae_index)(
        numerus_primorum + I) * magnitudo(i32));
    p = q->primus_limes;
    per (j = ZEPHYRUM; j <= numerus_primorum; j++)
    {
        p = _primus_proximus(q, p, necessaria);
        si (p == ZEPHYRUM)
        {
            redde FALSUM;
        }
        primi[j] = p;
    }

    residua = (i32*)piscina_conari_allocare(piscina, (memoriae_index)(
        numerus_primorum + I) * (memoriae_index)numerus_coefficientium
        * magnitudo(i32));
    si (residua == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j <= numerus_primorum; j++)
    {
        si (!_primum_aestimare(aestimare, contextus, q,
            numerus_coefficientium, primi[j], piscina, residua
            + (memoriae_index)j * (memoriae_index)numerus_coefficientium))
        {
            redde FALSUM;
        }
    }

    /* restitutio et probatio per primum additum et limitem */
    columna       = (i32*)piscina_allocare(piscina,
        (memoriae_index)numerus_primorum * magnitudo(i32));
    coefficientes = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)numerus_coefficientium * magnitudo(Magnus));
    si (coefficientes == NIHIL)
    {
        redde FALSUM;
    }
    primus_nonnullus   = (s64)-I;
    ultimus_nonnullus  = (s64)-I;
    per (k = ZEPHYRUM; k < numerus_coefficientium; k++)
    {
        Magnus c;

        per (j = ZEPHYRUM; j < numerus_primorum; j++)
        {
            columna[j] = residua[(memoriae_index)j
                * (memoriae_index)numerus_coefficientium
                    + (memoriae_index)k];
        }
        si (!congruentia_restitue(columna, primi, numerus_primorum,
            VERUM,
            piscina, &c))
        {
            redde FALSUM;
        }
        si (   magnus_residuum_parvum(c, primi[numerus_primorum])
            != residua[(memoriae_index)numerus_primorum
            * (memoriae_index)numerus_coefficientium
                + (memoriae_index)k]
            || magnus_bitorum(c) > q->limes_bitorum)
        {
            redde FALSUM;
        }
        coefficientes[k] = c;
        si (magnus_signum(c) != ZEPHYRUM)
        {
            si (primus_nonnullus < ZEPHYRUM)
            {
                primus_nonnullus = (s64)k;
            }
            ultimus_nonnullus = (s64)k;
        }
    }

    /* densum solum a coefficiente non nullo primo ad ultimum: memoria ex
     * effectu, non ex fenestra */
    si (primus_nonnullus < ZEPHYRUM)
    {
        *exitus = polynomium_nullum();
        redde VERUM;
    }
    amplitudo = (ultimus_nonnullus - primus_nonnullus) * (s64)q->passus
        + (s64)I;
    densa = (Magnus*)piscina_conari_allocare(piscina,
        (memoriae_index)amplitudo * magnitudo(Magnus));
    si (densa == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; (s64)k < amplitudo; k++)
    {
        densa[k] = magnus_ex_s64(ZEPHYRUM);
    }
    per (k = (i32)primus_nonnullus; (s64)k <= ultimus_nonnullus; k++)
    {
        densa[(memoriae_index)((s64)k - primus_nonnullus)
            * (memoriae_index)q->passus] = coefficientes[k];
    }
    redde polynomium_ex_coefficientibus(densa, (i32)amplitudo, (s32)(
        (s64)q->gradus_imus + primus_nonnullus * (s64)q->passus),
        piscina,
        exitus);
}
#undef _bita_infra
#undef _divisor_communis
#undef _horner
#undef _primum_aestimare
#undef _primus_aptus
#undef _primus_proximus
#undef _quaestio_valida
/* lib/congruentia.c: statica per plagulam renominata */
#define _testis _testis_congruentia
#line 1 "lib/congruentia.c"
/* congruentia.c - Arithmetica modularis exacta, moduli verbi (n < 2^32)
 *
 * Producta in i64 insignato: (n - 1)^2 < 2^64. Inversa per Euclidem
 * extensum in s64 (|coefficientes| <= n < 2^32). Primalitas per
 * Miller-Rabin cum basibus 2, 7, 61 - exacta pro omni n < 4759123141
 * (Jaeschke 1993), ergo pro omni i32. Reconstructio Sinica per Garner
 * super magnum. Vide lib/congruentia.worklog.md.
 */



/* ==================================================
 * Reductio
 * ================================================== */

i32
congruentia_ex_s64 (
    s64 x,
    i32 n)
{
    i64 modulus;
    i64 residuum;

    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (x >= ZEPHYRUM)
    {
        redde (i32)((i64)x % (i64)n);
    }
    /* |x| sine exundatione etiam pro S64 imo */
    modulus   = (i64)(-(x + I)) + (i64)I;
    residuum  = modulus % (i64)n;
    redde residuum == ZEPHYRUM ? ZEPHYRUM : (i32)((i64)n - residuum);
}

i32
congruentia_ex_magno (
    Magnus x,
       i32 n)
{
    redde magnus_residuum_parvum(x, n);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

i32
congruentia_adde (
    i32 a,
    i32 b,
    i32 n)
{
    i64 summa = (i64)a + (i64)b;

    si (summa >= (i64)n)
    {
        summa -= (i64)n;
    }
    redde (i32)summa;
}

i32
congruentia_subtrahe (
    i32 a,
    i32 b,
    i32 n)
{
    redde a >= b ? a - b : (i32)((i64)a + (i64)n - (i64)b);
}

i32
congruentia_multiplica (
    i32 a,
    i32 b,
    i32 n)
{
    redde (i32)(((i64)a * (i64)b) % (i64)n);
}

i32
congruentia_potentia (
    i32 a,
    i64 e,
    i32 n)
{
    i32 effectus  = (n == I) ? ZEPHYRUM : I;
    i32 basis     = a;

    dum (e > ZEPHYRUM)
    {
        si (e & (i64)I)
        {
            effectus = congruentia_multiplica(effectus, basis, n);
        }
        e >>= I;
        si (e > ZEPHYRUM)
        {
            basis = congruentia_multiplica(basis, basis, n);
        }
    }
    redde effectus;
}

b32
congruentia_inversa (
     i32  a,
     i32  n,
     i32* exitus)
{
    s64 r0 = (s64)n;
    s64 r1 = (s64)a;
    s64 t0 = ZEPHYRUM;
    s64 t1 = I;

    si (n < II)
    {
        redde FALSUM;
    }
    /* invarians: r_k = t_k a (mod n); |t_k| <= n */
    dum (r1 != ZEPHYRUM)
    {
        s64 quotiens  = r0 / r1;   /* r0, r1 >= 0: C89 definitum */
        s64 r2        = r0 - quotiens * r1;
        s64 t2        = t0 - quotiens * t1;

        r0 = r1;
        r1 = r2;
        t0 = t1;
        t1 = t2;
    }
    si (r0 != I)
    {
        redde FALSUM;
    }
    *exitus = congruentia_ex_s64(t0, n);
    redde VERUM;
}

s64
congruentia_symmetrica (
    i32 a,
    i32 n)
{
    redde a > n / II ? (s64)a - (s64)n : (s64)a;
}


/* ==================================================
 * Primi
 * ================================================== */

/* testis Miller-Rabin: n - 1 = d 2^s, d impar; VERUM si n probabiliter
 * primus ad basim b */
interior b32
_testis (
    i32 n,
    i32 basis,
    i32 d,
    i32 s)
{
    i32 x = congruentia_potentia(basis % n, (i64)d, n);
    i32 k;

    si (x == I || x == n - I || basis % n == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (k = I; k < s; k++)
    {
        x = congruentia_multiplica(x, x, n);
        si (x == n - I)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
congruentia_est_primus (
    i32 n)
{
    i32 d;
    i32 s = ZEPHYRUM;

    si (n < II)
    {
        redde FALSUM;
    }
    si (n < IV)
    {
        redde VERUM;
    }
    si (n % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    d = n - I;
    dum (d % II == ZEPHYRUM)
    {
        d /= II;
        s++;
    }
    redde _testis(n, II, d, s) && _testis(n, VII, d, s)
        && _testis(n, LXI, d, s);
}

i32
congruentia_primus_infra (
    i32 limes)
{
    i32 n;

    si (limes <= II)
    {
        redde ZEPHYRUM;
    }
    si (limes == III)
    {
        redde II;
    }
    /* impar maximus < limes */
    n = (limes - I) % II == ZEPHYRUM ? limes - II : limes - I;
    dum (n >= III)
    {
        si (congruentia_est_primus(n))
        {
            redde n;
        }
        n -= II;
    }
    redde II;
}


/* ==================================================
 * Reconstructio Sinica
 * ================================================== */

b32
congruentia_restitue (
    constans i32* residua,
    constans i32* moduli,
             i32  numerus,
             b32  symmetricus,
         Piscina* piscina,
          Magnus* exitus)
{
    Magnus x;
    Magnus productum;
       i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (moduli[k] < II || residua[k] >= moduli[k])
        {
            redde FALSUM;
        }
    }
    si (numerus == ZEPHYRUM)
    {
        *exitus = magnus_ex_s64(ZEPHYRUM);
        redde VERUM;
    }
    /* Garner: x_k = x_(k-1) + M_(k-1) t, t = (r_k - x_(k-1)) M^-1 mod
     * m_k; M^-1 existit sse moduli coprimi */
    x          = magnus_ex_s64((s64)residua[ZEPHYRUM]);
    productum  = magnus_ex_s64((s64)moduli[ZEPHYRUM]);
    per (k = I; k < numerus; k++)
    {
        i32 m = moduli[k];
        i32 inversa;
        i32 t;

        si (!congruentia_inversa(magnus_residuum_parvum(productum, m),
            m,
            &inversa))
        {
            redde FALSUM;
        }
        t = congruentia_multiplica(congruentia_subtrahe(residua[k],
            magnus_residuum_parvum(x, m), m), inversa, m);
        x = magnus_adde(x, magnus_multiplica(productum, magnus_ex_s64(
            (s64)t), piscina), piscina);
        productum = magnus_multiplica(productum, magnus_ex_s64((s64)m),
            piscina);
    }
    /* symmetricus: x > M/2 sse 2x > M */
    si (   symmetricus
        && magnus_compara(magnus_multiplica(x, magnus_ex_s64(II),
        piscina),
            productum) > ZEPHYRUM)
    {
        x = magnus_subtrahe(x, productum, piscina);
    }
    *exitus = x;
    redde VERUM;
}
#undef _testis
/* lib/anulus.c: statica per plagulam renominata */
#define _modulus _modulus_anulus
#define _p_ad_chordam _p_ad_chordam_anulus
#define _p_adde _p_adde_anulus
#define _p_aequalis _p_aequalis_anulus
#define _p_divide_exacte _p_divide_exacte_anulus
#define _p_est_nullum _p_est_nullum_anulus
#define _p_ex_chorda _p_ex_chorda_anulus
#define _p_multiplica _p_multiplica_anulus
#define _p_nullum _p_nullum_anulus
#define _p_parvum _p_parvum_anulus
#define _p_subtrahe _p_subtrahe_anulus
#define _p_transcribe _p_transcribe_anulus
#define _p_unum _p_unum_anulus
#define _q_ad_chordam _q_ad_chordam_anulus
#define _q_adde _q_adde_anulus
#define _q_aequalis _q_aequalis_anulus
#define _q_divide_exacte _q_divide_exacte_anulus
#define _q_est_nullum _q_est_nullum_anulus
#define _q_ex_chorda _q_ex_chorda_anulus
#define _q_multiplica _q_multiplica_anulus
#define _q_nullum _q_nullum_anulus
#define _q_parvum _q_parvum_anulus
#define _q_signum _q_signum_anulus
#define _q_subtrahe _q_subtrahe_anulus
#define _q_transcribe _q_transcribe_anulus
#define _q_unum _q_unum_anulus
#define _r_ad_chordam _r_ad_chordam_anulus
#define _r_adde _r_adde_anulus
#define _r_aequalis _r_aequalis_anulus
#define _r_divide_exacte _r_divide_exacte_anulus
#define _r_est_nullum _r_est_nullum_anulus
#define _r_ex_chorda _r_ex_chorda_anulus
#define _r_multiplica _r_multiplica_anulus
#define _r_nullum _r_nullum_anulus
#define _r_parvum _r_parvum_anulus
#define _r_subtrahe _r_subtrahe_anulus
#define _r_transcribe _r_transcribe_anulus
#define _r_unum _r_unum_anulus
#define _z_ad_chordam _z_ad_chordam_anulus
#define _z_adde _z_adde_anulus
#define _z_aequalis _z_aequalis_anulus
#define _z_compara_normam _z_compara_normam_anulus
#define _z_divide_cum_residuo _z_divide_cum_residuo_anulus
#define _z_divide_exacte _z_divide_exacte_anulus
#define _z_divisor_communis _z_divisor_communis_anulus
#define _z_est_nullum _z_est_nullum_anulus
#define _z_ex_chorda _z_ex_chorda_anulus
#define _z_multiplica _z_multiplica_anulus
#define _z_nullum _z_nullum_anulus
#define _z_parvum _z_parvum_anulus
#define _z_signum _z_signum_anulus
#define _z_subtrahe _z_subtrahe_anulus
#define _z_transcribe _z_transcribe_anulus
#define _z_unum _z_unum_anulus
#define FORMA_RESIDUORUM FORMA_RESIDUORUM_anulus
#line 1 "lib/anulus.c"
/* anulus.c - Anuli domus per tabulas functionum: Z (magnus), Q
 * (fractio), Z[t, t^-1] (polynomium), Z/n (congruentia, modulus in
 * contextu). Involucra tenuia: elementa opaca
 * ad typum suum convertuntur et functio bibliothecae vocatur. Vide
 * lib/anulus.worklog.md.
 */







/* ==================================================
 * Z: magnus
 * ================================================== */

#define VALOR_Z(x) (*(constans Magnus*)(x))

interior vacuum
_z_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_ex_s64(ZEPHYRUM);
}

interior vacuum
_z_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(Magnus*)exitus = magnus_ex_s64(I);
}

interior b32
_z_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde magnus_signum(VALOR_Z(a)) == ZEPHYRUM;
}

interior b32
_z_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    s64 valor;

    (vacuum)anulus;
    redde magnus_ad_s64(VALOR_Z(a), &valor);
}

interior b32
_z_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde magnus_aequalis(VALOR_Z(a), VALOR_Z(b));
}

interior b32
_z_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_adde(VALOR_Z(a), VALOR_Z(b), piscina);
    redde VERUM;
}

interior b32
_z_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_subtrahe(VALOR_Z(a), VALOR_Z(b), piscina);
    redde VERUM;
}

interior b32
_z_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_multiplica(VALOR_Z(a), VALOR_Z(b),
        piscina);
    redde VERUM;
}

interior b32
_z_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    Magnus quotiens;
    Magnus residuum;

    (vacuum)anulus;
    si (   !magnus_divide(VALOR_Z(a), VALOR_Z(b), piscina, &quotiens,
        &residuum)
        || magnus_signum(residuum) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *(Magnus*)exitus = quotiens;
    redde VERUM;
}

interior vacuum
_z_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Magnus*)exitus = magnus_transcribe(VALOR_Z(a), piscina);
}

interior chorda
_z_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_ad_chordam(VALOR_Z(a), piscina);
}

interior b32
_z_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde magnus_ex_chorda(textus, piscina, (Magnus*)exitus);
}

interior b32
_z_divisor_communis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* g,
             vacuum* u,
             vacuum* v)
{
    (vacuum)anulus;
    *(Magnus*)g = magnus_divisor_communis_testatus(VALOR_Z(a),
        VALOR_Z(b),
        piscina, (Magnus*)u, (Magnus*)v);
    redde VERUM;
}

interior b32
_z_divide_cum_residuo (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* q,
             vacuum* r)
{
    (vacuum)anulus;
    redde magnus_divide(VALOR_Z(a), VALOR_Z(b), piscina, (Magnus*)q,
        (Magnus*)r);
}

interior s32
_z_compara_normam (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_compara(magnus_absolutum(VALOR_Z(a), piscina),
        magnus_absolutum(VALOR_Z(b), piscina));
}

interior b32
_z_signum (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
                s32* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *exitus = magnus_signum(VALOR_Z(a));
    redde VERUM;
}

constans Anulus ANULUS_INTEGRORUM = {
    "Z", (memoriae_index)magnitudo(Magnus), FALSUM,
    _z_nullum, _z_unum, _z_est_nullum, _z_parvum, _z_aequalis, _z_adde,
        _z_subtrahe,
    _z_multiplica, _z_divide_exacte, _z_transcribe, _z_ad_chordam,
    _z_ex_chorda, _z_divisor_communis, _z_divide_cum_residuo,
    _z_compara_normam, NIHIL, VERUM, _z_signum
};


/* ==================================================
 * Q: fractio
 * ================================================== */

#define VALOR_Q(x) (*(constans Fractio*)(x))

interior vacuum
_q_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_ex_s64(ZEPHYRUM);
}

interior vacuum
_q_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(Fractio*)exitus = fractio_ex_s64(I);
}

interior b32
_q_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde fractio_signum(VALOR_Q(a)) == ZEPHYRUM;
}

interior b32
_q_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    s64 valor;

    (vacuum)anulus;
    redde magnus_ad_s64(fractio_numerator(VALOR_Q(a)), &valor)
        && magnus_ad_s64(fractio_denominator(VALOR_Q(a)), &valor);
}

interior b32
_q_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde fractio_aequalis(VALOR_Q(a), VALOR_Q(b));
}

interior b32
_q_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_adde(VALOR_Q(a), VALOR_Q(b), piscina);
    redde VERUM;
}

interior b32
_q_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_subtrahe(VALOR_Q(a), VALOR_Q(b),
        piscina);
    redde VERUM;
}

interior b32
_q_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_multiplica(VALOR_Q(a), VALOR_Q(b),
        piscina);
    redde VERUM;
}

interior b32
_q_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde fractio_divide(VALOR_Q(a), VALOR_Q(b), piscina,
        (Fractio*)exitus);
}

interior vacuum
_q_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Fractio*)exitus = fractio_transcribe(VALOR_Q(a), piscina);
}

interior chorda
_q_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde fractio_ad_chordam(VALOR_Q(a), piscina);
}

interior b32
_q_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde fractio_ex_chorda(textus, piscina, (Fractio*)exitus);
}

interior b32
_q_signum (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
                s32* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *exitus = fractio_signum(VALOR_Q(a));
    redde VERUM;
}

constans Anulus ANULUS_RATIONALIUM = {
    "Q", (memoriae_index)magnitudo(Fractio), VERUM,
    _q_nullum, _q_unum, _q_est_nullum, _q_parvum, _q_aequalis, _q_adde,
        _q_subtrahe,
    _q_multiplica, _q_divide_exacte, _q_transcribe, _q_ad_chordam,
    _q_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM, _q_signum
};


/* ==================================================
 * Z[t, t^-1]: polynomium
 * ================================================== */

#define VALOR_P(x) (*(constans Polynomium*)(x))

interior vacuum
_p_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_nullum();
}

interior vacuum
_p_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_constans(magnus_ex_s64(I),
        piscina);
}

interior b32
_p_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde polynomium_est_nullum(VALOR_P(a));
}

interior b32
_p_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    redde polynomium_est_nullum(VALOR_P(a));
}

interior b32
_p_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    (vacuum)anulus;
    redde polynomium_aequalis(VALOR_P(a), VALOR_P(b));
}

interior b32
_p_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_adde(VALOR_P(a), VALOR_P(b),
        piscina);
    redde VERUM;
}

interior b32
_p_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_subtrahe(VALOR_P(a), VALOR_P(b),
        piscina);
    redde VERUM;
}

interior b32
_p_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_multiplica(VALOR_P(a), VALOR_P(b), piscina,
        (Polynomium*)exitus);
}

interior b32
_p_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_divide_exacte(VALOR_P(a), VALOR_P(b), piscina,
        (Polynomium*)exitus);
}

interior vacuum
_p_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(Polynomium*)exitus = polynomium_transcribe(VALOR_P(a), piscina);
}

interior chorda
_p_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde polynomium_ad_chordam(VALOR_P(a), 't', piscina);
}

interior b32
_p_ex_chorda (
    constans Anulus* anulus,
             chorda  textus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    redde polynomium_ex_chorda(textus, 't', piscina,
        (Polynomium*)exitus);
}

constans Anulus ANULUS_POLYNOMIORUM = {
    "Z[t,t^-1]", (memoriae_index)magnitudo(Polynomium), FALSUM,
    _p_nullum, _p_unum, _p_est_nullum, _p_parvum, _p_aequalis, _p_adde,
        _p_subtrahe,
    _p_multiplica, _p_divide_exacte, _p_transcribe, _p_ad_chordam,
    _p_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM, NIHIL
};


/* ==================================================
 * Z/n: congruentia (modulus in anulus->contextus)
 * ================================================== */

#define VALOR_R(x) (*(constans i32*)(x))

interior i32
_modulus (
    constans Anulus* anulus)
{
    redde *(constans i32*)anulus->contextus;
}

interior vacuum
_r_nullum (
    constans Anulus* anulus,
             vacuum* exitus)
{
    (vacuum)anulus;
    *(i32*)exitus = ZEPHYRUM;
}

interior vacuum
_r_unum (
    constans Anulus* anulus,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = I % _modulus(anulus);
}

interior b32
_r_est_nullum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    i32 valor = VALOR_R(a);

    (vacuum)anulus;
    redde valor == ZEPHYRUM;
}

interior b32
_r_parvum (
    constans Anulus* anulus,
    constans vacuum* a)
{
    (vacuum)anulus;
    (vacuum)a;
    redde VERUM;
}

interior b32
_r_aequalis (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b)
{
    i32 x = VALOR_R(a);
    i32 y = VALOR_R(b);

    (vacuum)anulus;
    redde x == y;
}

interior b32
_r_adde (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_adde(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

interior b32
_r_subtrahe (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_subtrahe(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

interior b32
_r_multiplica (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)piscina;
    *(i32*)exitus = congruentia_multiplica(VALOR_R(a), VALOR_R(b),
        _modulus(anulus));
    redde VERUM;
}

/* a b^-1; FALSUM si b non invertibilis (etiam si a forte divisibilis:
 * Z/n compositus solum per unitates dividit) */
interior b32
_r_divide_exacte (
    constans Anulus* anulus,
    constans vacuum* a,
    constans vacuum* b,
            Piscina* piscina,
             vacuum* exitus)
{
    i32 inversa;

    (vacuum)piscina;
    si (!congruentia_inversa(VALOR_R(b), _modulus(anulus), &inversa))
    {
        redde FALSUM;
    }
    *(i32*)exitus = congruentia_multiplica(VALOR_R(a), inversa,
        _modulus(anulus));
    redde VERUM;
}

interior vacuum
_r_transcribe (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina,
             vacuum* exitus)
{
    (vacuum)anulus;
    (vacuum)piscina;
    *(i32*)exitus = VALOR_R(a);
}

interior chorda
_r_ad_chordam (
    constans Anulus* anulus,
    constans vacuum* a,
            Piscina* piscina)
{
    (vacuum)anulus;
    redde magnus_ad_chordam(magnus_ex_s64((s64)VALOR_R(a)), piscina);
}

/* integer decimalis quilibet (etiam negativus), modulo n reductus */
interior b32
_r_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              vacuum* exitus)
{
    Magnus valor;

    si (!magnus_ex_chorda(textus, piscina, &valor))
    {
        redde FALSUM;
    }
    *(i32*)exitus = congruentia_ex_magno(valor, _modulus(anulus));
    redde VERUM;
}

interior constans Anulus FORMA_RESIDUORUM = {
    "Z/n", (memoriae_index)magnitudo(i32), FALSUM,
    _r_nullum, _r_unum, _r_est_nullum, _r_parvum, _r_aequalis, _r_adde,
    _r_subtrahe, _r_multiplica, _r_divide_exacte, _r_transcribe,
    _r_ad_chordam, _r_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, FALSUM,
    NIHIL
};

constans Anulus*
anulus_residuorum (
         i32  n,
     Piscina* piscina)
{
       Anulus* anulus;
          i32* modulus;
    character* titulus;
    character  digiti[XII];
          i32  numerus = ZEPHYRUM;
          i32  k;
          i32  x = n;

    si (n < II)
    {
        redde NIHIL;
    }
    anulus    = (Anulus*)piscina_allocare(piscina, magnitudo(Anulus));
    modulus   = (i32*)piscina_allocare(piscina, magnitudo(i32));
    *anulus   = FORMA_RESIDUORUM;
    *modulus  = n;
    /* titulus "Z/n" */
    dum (x > ZEPHYRUM)
    {
        digiti[numerus++]  = (character)('0' + (s32)(x % X));
        x                  /= X;
    }
    titulus = (character*)piscina_allocare(piscina, (memoriae_index)(
        numerus + IV));
    titulus[ZEPHYRUM]  = 'Z';
    titulus[I]         = '/';
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        titulus[II + k] = digiti[numerus - I - k];
    }
    titulus[II + numerus]  = '\0';
    anulus->titulus        = titulus;
    anulus->corpus         = congruentia_est_primus(n);
    anulus->integrum       = anulus->corpus;
    anulus->contextus      = modulus;
    redde anulus;
}
#undef _modulus
#undef _p_ad_chordam
#undef _p_adde
#undef _p_aequalis
#undef _p_divide_exacte
#undef _p_est_nullum
#undef _p_ex_chorda
#undef _p_multiplica
#undef _p_nullum
#undef _p_parvum
#undef _p_subtrahe
#undef _p_transcribe
#undef _p_unum
#undef _q_ad_chordam
#undef _q_adde
#undef _q_aequalis
#undef _q_divide_exacte
#undef _q_est_nullum
#undef _q_ex_chorda
#undef _q_multiplica
#undef _q_nullum
#undef _q_parvum
#undef _q_signum
#undef _q_subtrahe
#undef _q_transcribe
#undef _q_unum
#undef _r_ad_chordam
#undef _r_adde
#undef _r_aequalis
#undef _r_divide_exacte
#undef _r_est_nullum
#undef _r_ex_chorda
#undef _r_multiplica
#undef _r_nullum
#undef _r_parvum
#undef _r_subtrahe
#undef _r_transcribe
#undef _r_unum
#undef _z_ad_chordam
#undef _z_adde
#undef _z_aequalis
#undef _z_compara_normam
#undef _z_divide_cum_residuo
#undef _z_divide_exacte
#undef _z_divisor_communis
#undef _z_est_nullum
#undef _z_ex_chorda
#undef _z_multiplica
#undef _z_nullum
#undef _z_parvum
#undef _z_signum
#undef _z_subtrahe
#undef _z_transcribe
#undef _z_unum
#undef FORMA_RESIDUORUM
#undef VALOR_P
#undef VALOR_Q
#undef VALOR_R
#undef VALOR_Z
/* lib/contractio.c: statica per plagulam renominata */
#define _addere_saturate _addere_saturate_contractio
#define _alter _alter_contractio
#define _avide_producere _avide_producere_contractio
#define _bita _bita_contractio
#define _candidatum_currere _candidatum_currere_contractio
#define _clavis_minor _clavis_minor_contractio
#define _clavis_paris _clavis_paris_contractio
#define _contrahere _contrahere_contractio
#define _elementa_effectus _elementa_effectus_contractio
#define _elementum _elementum_contractio
#define _exsequi_anulo_intra _exsequi_anulo_intra_contractio
#define _gradum_anulo _gradum_anulo_contractio
#define _gradum_facere _gradum_facere_contractio
#define _gradum_modulo _gradum_modulo_contractio
#define _gradum_parare _gradum_parare_contractio
#define _implendo_producere _implendo_producere_contractio
#define _impletio _impletio_contractio
#define _latitudo _latitudo_contractio
#define _melior _melior_contractio
#define _movet _movet_contractio
#define _multiplicare_saturate _multiplicare_saturate_contractio
#define _numeratores_promovere _numeratores_promovere_contractio
#define _operandum_modulo _operandum_modulo_contractio
#define _par_eligere _par_eligere_contractio
#define _passus_ponere _passus_ponere_contractio
#define _signa_delere _signa_delere_contractio
#define _statum_finire _statum_finire_contractio
#define _statum_incipere _statum_incipere_contractio
#define _tabulam_disponere _tabulam_disponere_contractio
#define _verticem_tollere _verticem_tollere_contractio
#define _vicinum_addere _vicinum_addere_contractio
#define _vicinum_tollere _vicinum_tollere_contractio
#define GradusOrdinis GradusOrdinis_contractio
#define IndexRetis IndexRetis_contractio
#define IntervallumTabulae IntervallumTabulae_contractio
#define StatusOrdinis StatusOrdinis_contractio
#define TensorOrdinis TensorOrdinis_contractio
#define TensorRetis TensorRetis_contractio
#define VicinitasIndicis VicinitasIndicis_contractio
#line 1 "lib/contractio.c"
/* contractio.c - Retia tensorum: structura, ordo avidus, tabula
 *
 * T2 (project-specs/contractio-plan.md): rete, ordo avidus cum clave
 * exacta, dispositio tabulae per vitam (primum aptum), sumptus
 * saturati, officina. Exsecutio (gradus calidus et referentiae) in
 * T3. Vide include/contractio.h et lib/contractio.worklog.md.
 */





/* id tensoris nullus (possessor absens) */
#define CONTRACTIO_NULLUS      ((i32)0xFFFFFFFFUL)

/* signum temporarium in IndexRetis.usus: index iam in hoc tensore */
#define CONTRACTIO_USUS_SIGNUM ((i32)0x80000000UL)


/* ==================================================
 * Structurae privatae
 * ================================================== */

nomen structura {
    i32 dimensio;
    i32 usus;            /* tensores qui indicem tenent: 0, I, II */
} IndexRetis;

/* indices tensoris in rete->membra [initium, initium + numerus) */
nomen structura {
    i32 initium;
    i32 numerus;
} TensorRetis;

structura ContractioRete {
    Piscina* piscina;
        Xar* indices;    /* IndexRetis */
        Xar* tensores;   /* TensorRetis */
        Xar* membra;     /* i32: indices omnium tensorum, continui */
        b32  ordinatum;
};

/* tensor in ordine: initiales indices ordine vocantis, intermedii
 * ordine id ascendente */
nomen structura {
    i32* indices;
    i32  numerus;
    i64  elementa;       /* saturata */
    i64  offset;         /* in tabula (elementa i32); solum intermedii */
} TensorOrdinis;

/* gradus cum consilio ansae: solum indices dimensionis >= 2 (indices
 * dimensionis 1 offset numquam movent et numero non finiuntur) */
nomen structura {
    i32  sinister;
    i32  dexter;
    i32  effectus;
    i32  externi;            /* indices effectus, ordine effectus */
    i32  interni;            /* indices communes, ordine sinistri */
    i32* dimensiones;        /* externi + interni */
    i64* passus_sinistri;    /* passus in sinistro; 0 si ibi absens */
    i64* passus_dextri;      /* passus in dextro; 0 si ibi absens */
} GradusOrdinis;

structura ContractioOrdo {
    constans ContractioRete* rete;
                        i32  initiales;          /* n */
                        i32  numerus_graduum;
              GradusOrdinis* gradus;
              TensorOrdinis* tensores;           /* n + gradus */
                        b32  tabula_saturata;
          ContractioSumptus  sumptus;
                        i32  candidatus;           /* victor (T5b) */
                        i32  numerus_candidatorum;
          ContractioSumptus* candidati;            /* simulati, 1.. */
                        b32* candidati_aestimati;
};

structura ContractioOfficina {
    constans ContractioOrdo* ordo;
                        i32* tabula;             /* NIHIL si vacua */
};

/* intervallum tabulae occupatum [offset, finis) */
nomen structura {
    i64 offset;
    i64 finis;
    i32 tensor;
} IntervallumTabulae;


/* ==================================================
 * Arithmetica saturata
 * ================================================== */

interior i64
_multiplicare_saturate (
    i64 a,
    i64 b)
{
    si (a == ZEPHYRUM || b == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (a > CONTRACTIO_SATURATUM / b)
    {
        redde CONTRACTIO_SATURATUM;
    }
    redde a * b;
}

interior i64
_addere_saturate (
    i64 a,
    i64 b)
{
    si (a > CONTRACTIO_SATURATUM - b)
    {
        redde CONTRACTIO_SATURATUM;
    }
    redde a + b;
}

/* numerus bitorum x: 0 pro 0, k + 1 pro 2^k <= x < 2^(k+1) */
interior i32
_bita (
    i64 x)
{
    i32 n = ZEPHYRUM;

    dum (x != ZEPHYRUM)
    {
        n++;
        x >>= I;
    }
    redde n;
}


/* ==================================================
 * Rete
 * ================================================== */

ContractioRete*
contractio_rete_creare (
    Piscina* piscina)
{
    ContractioRete* rete;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    rete = (ContractioRete*)piscina_allocare(piscina,
        magnitudo(ContractioRete));
    rete->piscina    = piscina;
    rete->indices    = xar_creare(piscina, (i32)magnitudo(IndexRetis));
    rete->tensores   = xar_creare(piscina, (i32)magnitudo(TensorRetis));
    rete->membra     = xar_creare(piscina, (i32)magnitudo(i32));
    rete->ordinatum  = FALSUM;
    redde rete;
}

b32
contractio_index_addere (
    ContractioRete* rete,
               i32  dimensio,
               i32* id)
{
    IndexRetis* index;

    si (   rete     == NIHIL || id == NIHIL || rete->ordinatum
        || dimensio == ZEPHYRUM)
    {
        redde FALSUM;
    }
    index = (IndexRetis*)xar_addere(rete->indices);
    si (index == NIHIL)
    {
        redde FALSUM;
    }
    index->dimensio  = dimensio;
    index->usus      = ZEPHYRUM;
    *id              = xar_numerus(rete->indices) - I;
    redde VERUM;
}

/* signa temporaria indicum [0, numerus) delere */
interior vacuum
_signa_delere (
    ContractioRete* rete,
     constans i32*  indices,
              i32   numerus)
{
    i32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index = (IndexRetis*)xar_obtinere(rete->indices,
            indices[j]);

        index->usus &= ~CONTRACTIO_USUS_SIGNUM;
    }
}

b32
contractio_tensor_addere (
        ContractioRete* rete,
         constans i32*  indices,
                  i32   numerus,
                  i32*  id)
{
     TensorRetis* tensor;
             i32  numerus_indicum;
             i32  initium;
             i32  j;

    si (   rete == NIHIL || id == NIHIL || rete->ordinatum
        || (numerus > ZEPHYRUM && indices == NIHIL))
    {
        redde FALSUM;
    }
    numerus_indicum = xar_numerus(rete->indices);

    /* validatio in O(numerus): signum in usus pro indice iam viso */
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index;

        si (indices[j] >= numerus_indicum)
        {
            _signa_delere(rete, indices, j);
            redde FALSUM;
        }
        index = (IndexRetis*)xar_obtinere(rete->indices, indices[j]);
        si (   (index->usus & CONTRACTIO_USUS_SIGNUM) != ZEPHYRUM
            || index->usus                            >= II)
        {
            _signa_delere(rete, indices, j);
            redde FALSUM;
        }
        index->usus |= CONTRACTIO_USUS_SIGNUM;
    }
    _signa_delere(rete, indices, numerus);

    initium = xar_numerus(rete->membra);
    si (   numerus > ZEPHYRUM
        && xar_addere_multos(rete->membra, indices, numerus) != numerus)
    {
        xar_truncare(rete->membra, initium);
        redde FALSUM;
    }
    tensor = (TensorRetis*)xar_addere(rete->tensores);
    si (tensor == NIHIL)
    {
        xar_truncare(rete->membra, initium);
        redde FALSUM;
    }
    tensor->initium = initium;
    tensor->numerus = numerus;
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index = (IndexRetis*)xar_obtinere(rete->indices,
            indices[j]);

        index->usus++;
    }
    *id = xar_numerus(rete->tensores) - I;
    redde VERUM;
}

b32
contractio_rete_clausum (
    constans ContractioRete* rete)
{
    i32 numerus;
    i32 j;

    si (rete == NIHIL)
    {
        redde FALSUM;
    }
    numerus = xar_numerus(rete->indices);
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        constans IndexRetis* index = (constans IndexRetis*)xar_obtinere(
            rete->indices, j);

        si (index->usus != II)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ordo avidus
 * ================================================== */

ContractioOptiones
contractio_optiones_ordinariae (vacuum)
{
    ContractioOptiones o;

    o.methodus       = CONTRACTIO_AVIDUS;
    o.conatus        = XVI;
    o.semen          = CONTRACTIO_SEMEN_ORDINARIUM;
    o.octeti_maximi  = CONTRACTIO_SATURATUM;
    o.candidatus     = CONTRACTIO_CANDIDATUS_OMNES;
    o.limen          = CONTRACTIO_LIMEN_ORDINARIUM;
    redde o;
}

/* possessor alter indicis respectu tensoris */
interior i32
_alter (
    constans i32* possessores,
             i32  index,
             i32  tensor)
{
    si (possessores[II * index] == tensor)
    {
        redde possessores[II * index + I];
    }
    redde possessores[II * index];
}

/* elementa effectus si a et b contrahuntur: productum dimensionum
 * indicum non communium (saturatum) */
interior i64
_elementa_effectus (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
                       i32  a,
                       i32  b)
{
    i64 elementa = I;
    i32 j;

    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        i32 index = tensores[a].indices[j];

        si (_alter(possessores, index, a) != b)
        {
            elementa = _multiplicare_saturate(elementa,
                dimensiones[index]);
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        i32 index = tensores[b].indices[j];

        si (_alter(possessores, index, b) != a)
        {
            elementa = _multiplicare_saturate(elementa,
                dimensiones[index]);
        }
    }
    redde elementa;
}

/* clavis (elementa, a, b) lexicographice minor quam optima? */
interior b32
_clavis_minor (
    i64 elementa,
    i32 a,
    i32 b,
    i64 optima_elementa,
    i32 optimus_a,
    i32 optimus_b)
{
    si (elementa != optima_elementa)
    {
        redde elementa < optima_elementa;
    }
    si (a != optimus_a)
    {
        redde a < optimus_a;
    }
    redde b < optimus_b;
}

/* clavis paris: AVIDUS (T2) = elementa effectus (i64 saturata);
 * REDUCTIO = E_r - E_a - E_b (quisque E ad 2^62 limitatus, s64),
 * translata (+ 2^63) ut ordo i64 sine signo ordinem s64 servet */
interior i64
_clavis_paris (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
                       i32  genus,
                       i32  a,
                       i32  b)
{
    i64 effectus = _elementa_effectus(tensores, possessores,
        dimensiones,
        a, b);
    i64 limes = (i64)I << LXII;
    s64 clavis;

    si (genus != CONTRACTIO_CANDIDATUS_REDUCTIO)
    {
        redde effectus;
    }
    clavis = (s64)(effectus < limes ? effectus : limes)
        - (s64)(tensores[a].elementa
        < limes ? tensores[a].elementa : limes)
        - (s64)(tensores[b].elementa
        < limes ? tensores[b].elementa : limes);
    redde (i64)clavis ^ ((i64)I << LXIII);
}

/* par eligere: primum inter paria indicem communem habentia; si
 * nullum, inter omnia paria (vivi ascendentes, ergo a < b) */
interior vacuum
_par_eligere (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
              constans i32* vivi,
                       i32  numerus_vivorum,
                       i32  genus,
                       i32* sinister,
                       i32* dexter)
{
    b32 inventum  = FALSUM;
    i64 optima    = ZEPHYRUM;
    i32 oa        = ZEPHYRUM;
    i32 ob        = ZEPHYRUM;
    i32 x;
    i32 y;
    i32 j;

    per (x = ZEPHYRUM; x < numerus_vivorum; x++)
    {
        i32 a = vivi[x];

        per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
        {
            i32 b = _alter(possessores, tensores[a].indices[j], a);
            i64 elementa;

            si (b == CONTRACTIO_NULLUS || b <= a)
            {
                perge;
            }
            elementa = _clavis_paris(tensores, possessores, dimensiones,
                genus, a, b);
            si (   !inventum
                || _clavis_minor(elementa, a, b, optima, oa, ob))
            {
                inventum  = VERUM;
                optima    = elementa;
                oa        = a;
                ob        = b;
            }
        }
    }
    si (!inventum)
    {
        /* componentes disiunctae: productum externum. Si omnes vivi
         * scalares sunt (in rete clauso semper), omnis clavis 1 est:
         * par = duo id minimi, sine scansione O(L^2) per gradum. */
        b32 sine_indicibus = VERUM;

        per (x = ZEPHYRUM; x < numerus_vivorum; x++)
        {
            si (tensores[vivi[x]].numerus != ZEPHYRUM)
            {
                sine_indicibus = FALSUM;
                frange;
            }
        }
        si (sine_indicibus)
        {
            *sinister  = vivi[ZEPHYRUM];
            *dexter    = vivi[I];
            redde;
        }
        per (x = ZEPHYRUM; x < numerus_vivorum; x++)
        {
            per (y = x + I; y < numerus_vivorum; y++)
            {
                i64 elementa = _clavis_paris(tensores, possessores,
                    dimensiones, genus, vivi[x], vivi[y]);

                si (   !inventum
                    || _clavis_minor(elementa, vivi[x], vivi[y],
                    optima, oa, ob))
                {
                    inventum  = VERUM;
                    optima    = elementa;
                    oa        = vivi[x];
                    ob        = vivi[y];
                }
            }
        }
    }
    *sinister  = oa;
    *dexter    = ob;
}

/* a et b in c contrahere: indices c (ordine id), possessores
 * renovati, elementa; redde multiplicationes gradus (productum
 * dimensionum unionis, saturatum) */
interior i64
_contrahere (
    TensorOrdinis* tensores,
              i32* possessores,
     constans i32* dimensiones,
              i32  a,
              i32  b,
              i32  c,
          Piscina* piscina)
{
     TensorOrdinis* t        = &tensores[c];
               i32  numerus  = ZEPHYRUM;
               i64  multiplicationes;
               i32  j;
               i32  k;

    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        si (_alter(possessores, tensores[a].indices[j], a) != b)
        {
            numerus++;
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        si (_alter(possessores, tensores[b].indices[j], b) != a)
        {
            numerus++;
        }
    }
    t->numerus   = numerus;
    t->indices   = NIHIL;
    t->elementa  = I;
    t->offset    = CONTRACTIO_SATURATUM;
    si (numerus > ZEPHYRUM)
    {
        t->indices = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i32));
    }

    /* indices servati; communes semel in multiplicationes */
    numerus           = ZEPHYRUM;
    multiplicationes  = I;
    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        i32 index = tensores[a].indices[j];

        si (_alter(possessores, index, a) != b)
        {
            t->indices[numerus++] = index;
        }
        alioquin
        {
            multiplicationes = _multiplicare_saturate(multiplicationes,
                dimensiones[index]);
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        i32 index = tensores[b].indices[j];

        si (_alter(possessores, index, b) != a)
        {
            t->indices[numerus++] = index;
        }
    }

    /* ordo id ascendens (insertio: tensores parvi) */
    per (j = I; j < numerus; j++)
    {
        i32 index = t->indices[j];

        k = j;
        dum (k > ZEPHYRUM && t->indices[k - I] > index)
        {
            t->indices[k] = t->indices[k - I];
            k--;
        }
        t->indices[k] = index;
    }

    /* possessores: a aut b -> c */
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        i32 index = t->indices[j];

        si (   possessores[II * index] == a
            || possessores[II * index] == b)
        {
            possessores[II * index] = c;
        }
        alioquin
        {
            possessores[II * index + I] = c;
        }
        t->elementa = _multiplicare_saturate(t->elementa,
            dimensiones[index]);
    }
    redde _multiplicare_saturate(multiplicationes, t->elementa);
}

/* index dimensionis 1 offset numquam movet: e consilio ansae omittitur */
interior b32
_movet (
    i32 dimensio)
{
    redde dimensio > I;
}

/* passus indicum tensoris (index ultimus celerrimus) in tabulam per
 * indicem scribere (valor > 0), aut delere (ZEPHYRUM) */
interior vacuum
_passus_ponere (
    constans TensorOrdinis* t,
              constans i32* dimensiones,
                       i64* passus,
                       b32  ponere)
{
    i64 p = I;
    i32 j = t->numerus;

    dum (j > ZEPHYRUM)
    {
        j--;
        passus[t->indices[j]] = ponere ? p : ZEPHYRUM;
        p = _multiplicare_saturate(p, dimensiones[t->indices[j]]);
    }
}

/* consilium ansae gradus: externi = indices effectus dimensionis >= 2
 * (ordine effectus: offset effectus tunc simpliciter crescit),
 * interni = indices communes dimensionis >= 2 (ordine sinistri),
 * passus in utroque operando. Tabulae passus_a/passus_b nullae ante
 * et post. */
interior vacuum
_gradum_parare (
     constans TensorOrdinis* tensores,
               constans i32* dimensiones,
                        i64* passus_a,
                        i64* passus_b,
              GradusOrdinis* g,
                    Piscina* piscina)
{
    constans TensorOrdinis* a = &tensores[g->sinister];
    constans TensorOrdinis* b = &tensores[g->dexter];
    constans TensorOrdinis* c = &tensores[g->effectus];
                       i32  numerus;
                       i32  j;
                       i32  k = ZEPHYRUM;

    _passus_ponere(a, dimensiones, passus_a, VERUM);
    _passus_ponere(b, dimensiones, passus_b, VERUM);
    g->externi = ZEPHYRUM;
    g->interni = ZEPHYRUM;
    per (j = ZEPHYRUM; j < c->numerus; j++)
    {
        si (_movet(dimensiones[c->indices[j]]))
        {
            g->externi++;
        }
    }
    per (j = ZEPHYRUM; j < a->numerus; j++)
    {
        i32 index = a->indices[j];

        si (passus_b[index] != ZEPHYRUM && _movet(dimensiones[index]))
        {
            g->interni++;
        }
    }
    numerus             = g->externi + g->interni;
    g->dimensiones      = NIHIL;
    g->passus_sinistri  = NIHIL;
    g->passus_dextri    = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        g->dimensiones = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i32));
        g->passus_sinistri = (i64*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i64));
        g->passus_dextri = (i64*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i64));
    }
    per (j = ZEPHYRUM; j < c->numerus; j++)
    {
        i32 index = c->indices[j];

        si (_movet(dimensiones[index]))
        {
            g->dimensiones[k]      = dimensiones[index];
            g->passus_sinistri[k]  = passus_a[index];
            g->passus_dextri[k]    = passus_b[index];
            k++;
        }
    }
    per (j = ZEPHYRUM; j < a->numerus; j++)
    {
        i32 index = a->indices[j];

        si (passus_b[index] != ZEPHYRUM && _movet(dimensiones[index]))
        {
            g->dimensiones[k]      = dimensiones[index];
            g->passus_sinistri[k]  = passus_a[index];
            g->passus_dextri[k]    = passus_b[index];
            k++;
        }
    }
    _passus_ponere(a, dimensiones, passus_a, FALSUM);
    _passus_ponere(b, dimensiones, passus_b, FALSUM);
}

/* tabula: intermedii ordine graduum; effectus gradus primo apto
 * ponitur dum operandi eius adhuc vivunt, deinde operandi liberantur */
interior vacuum
_tabulam_disponere (
    ContractioOrdo* ordo,
          Piscina* piscina)
{
    IntervallumTabulae* occupata;
                   i32  numerus  = ZEPHYRUM;
                   i64  summum   = ZEPHYRUM;
                   i32  s;
                   i32  j;

    ordo->tabula_saturata = FALSUM;
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        ordo->sumptus.octeti_officinae = ZEPHYRUM;
        redde;
    }
    occupata = (IntervallumTabulae*)piscina_allocare(piscina,
        (memoriae_index)ordo->numerus_graduum * magnitudo(IntervallumTabulae));

    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
         constans GradusOrdinis* g = &ordo->gradus[s];
                  TensorOrdinis* t = &ordo->tensores[g->effectus];
                            i64  candidatum = ZEPHYRUM;
                            i64  finis;
                            i32  locus = numerus;

        si (t->elementa == CONTRACTIO_SATURATUM)
        {
            ordo->tabula_saturata = VERUM;
            frange;
        }
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   occupata[j].offset              >= candidatum
                && occupata[j].offset - candidatum >= t->elementa)
            {
                locus = j;
                frange;
            }
            candidatum = occupata[j].finis;
        }
        /* finis tabulae <= summa intermediorum <= multiplicationes:
         * hic saturatio solum si multiplicationes iam saturatae
         * (officina tunc refutatur) - ramus custos, non probatus */
        finis = _addere_saturate(candidatum, t->elementa);
        si (finis == CONTRACTIO_SATURATUM)
        {
            ordo->tabula_saturata = VERUM;
            frange;
        }
        per (j = numerus; j > locus; j--)
        {
            occupata[j] = occupata[j - I];
        }
        occupata[locus].offset  = candidatum;
        occupata[locus].finis   = finis;
        occupata[locus].tensor  = g->effectus;
        numerus++;
        t->offset = candidatum;
        si (finis > summum)
        {
            summum = finis;
        }

        /* operandi liberantur (initiales numquam in tabula) */
        j = ZEPHYRUM;
        dum (j < numerus)
        {
            si (   occupata[j].tensor == g->sinister
                || occupata[j].tensor == g->dexter)
            {
                i32 k;

                per (k = j; k + I < numerus; k++)
                {
                    occupata[k] = occupata[k + I];
                }
                numerus--;
            }
            alioquin
            {
                j++;
            }
        }
    }
    si (ordo->tabula_saturata)
    {
        per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
        {
            ordo->tensores[ordo->gradus[s].effectus].offset =
                CONTRACTIO_SATURATUM;
        }
        ordo->sumptus.octeti_officinae = CONTRACTIO_SATURATUM;
        redde;
    }
    ordo->sumptus.octeti_officinae = _multiplicare_saturate(summum,
        (i64)magnitudo(i32));
}

/* latitudo = max ceil(log2 elementa) super intermedios, EXACTE:
 * elementa saturata per magnum recomputantur */
interior i32
_latitudo (
    constans ContractioOrdo* ordo,
              constans i32* dimensiones,
                   Piscina* piscina)
{
    PiscinaNotatio nota      = piscina_notare(piscina);
               i32 latitudo  = ZEPHYRUM;
               i32 s;
               i32 j;

    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans TensorOrdinis* t =
            &ordo->tensores[ordo->gradus[s].effectus];
        i32 bita;

        si (t->elementa != CONTRACTIO_SATURATUM)
        {
            bita = _bita(t->elementa - I);
        }
        alioquin
        {
            Magnus productum = magnus_ex_s64((s64)I);

            per (j = ZEPHYRUM; j < t->numerus; j++)
            {
                productum = magnus_multiplica(productum,
                    magnus_ex_s64((s64)dimensiones[t->indices[j]]),
                    piscina);
            }
            bita = magnus_bitorum(magnus_subtrahe(productum,
                magnus_ex_s64((s64)I), piscina));
        }
        si (bita > latitudo)
        {
            latitudo = bita;
        }
    }
    piscina_reficere(piscina, nota);
    redde latitudo;
}


/* ==================================================
 * Constructio ordinis (communis omnibus methodis)
 * ================================================== */

/* status constructionis: ordo crescens, possessores indicum, vivi */
nomen structura {
    ContractioOrdo* ordo;
              i32* possessores;
              i32* dimensiones;
              i32* vivi;
              i32  numerus_vivorum;
              i32  gradus_facti;
              i64* passus_a;            /* NIHIL: sine consiliis ansae */
              i64* passus_b;
              i64  limes;               /* abicere si multiplicationes > */
} StatusOrdinis;

/* ordo novus cum tensoribus initialibus; consilia: passus pro consiliis
 * ansae (materializatio), aliter simulatio sola */
interior vacuum
_statum_incipere (
    constans ContractioRete* rete,
                        b32  consilia,
                        i64  limes,
                   Piscina* piscina,
             StatusOrdinis* st)
{
    ContractioOrdo* ordo;
               i32  n = xar_numerus(rete->tensores);
               i32  m = xar_numerus(rete->indices);
               i32  j;
               i32  k;

    ordo = (ContractioOrdo*)piscina_allocare(piscina,
        magnitudo(ContractioOrdo));
    ordo->rete                      = rete;
    ordo->initiales                 = n;
    ordo->numerus_graduum           = n > ZEPHYRUM ? n - I : ZEPHYRUM;
    ordo->gradus                    = NIHIL;
    ordo->tensores                  = NIHIL;
    ordo->tabula_saturata           = FALSUM;
    ordo->sumptus.apex              = ZEPHYRUM;
    ordo->sumptus.octeti_officinae  = ZEPHYRUM;
    ordo->sumptus.multiplicationes  = ZEPHYRUM;
    ordo->sumptus.latitudo          = ZEPHYRUM;
    ordo->candidatus                = CONTRACTIO_CANDIDATUS_AVIDUS;
    ordo->numerus_candidatorum      = ZEPHYRUM;
    ordo->candidati                 = NIHIL;
    ordo->candidati_aestimati       = NIHIL;
    st->ordo                        = ordo;
    st->possessores                 = NIHIL;
    st->dimensiones                 = NIHIL;
    st->vivi                        = NIHIL;
    st->numerus_vivorum             = n;
    st->gradus_facti                = ZEPHYRUM;
    st->passus_a                    = NIHIL;
    st->passus_b                    = NIHIL;
    st->limes                       = limes;
    si (n == ZEPHYRUM)
    {
        redde;
    }
    ordo->tensores = (TensorOrdinis*)piscina_allocare(piscina,
        (memoriae_index)(n + ordo->numerus_graduum)
        * magnitudo(TensorOrdinis));
    si (ordo->numerus_graduum > ZEPHYRUM)
    {
        ordo->gradus = (GradusOrdinis*)piscina_allocare(piscina,
            (memoriae_index)ordo->numerus_graduum
            * magnitudo(GradusOrdinis));
    }
    si (m > ZEPHYRUM)
    {
        st->dimensiones = (i32*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i32));
        st->possessores = (i32*)piscina_allocare(piscina,
            (memoriae_index)m * II * magnitudo(i32));
    }
    per (j = ZEPHYRUM; j < m; j++)
    {
        constans IndexRetis* index = (constans IndexRetis*)xar_obtinere(
            rete->indices, j);

        st->dimensiones[j]           = index->dimensio;
        st->possessores[II * j]      = CONTRACTIO_NULLUS;
        st->possessores[II * j + I]  = CONTRACTIO_NULLUS;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans TensorRetis* tr = (constans TensorRetis*)xar_obtinere(
            rete->tensores, k);
        TensorOrdinis* t = &ordo->tensores[k];

        t->numerus   = tr->numerus;
        t->indices   = NIHIL;
        t->elementa  = I;
        t->offset    = CONTRACTIO_SATURATUM;
        si (tr->numerus > ZEPHYRUM)
        {
            t->indices = (i32*)piscina_allocare(piscina,
                (memoriae_index)tr->numerus * magnitudo(i32));
            (vacuum)xar_copiare_ad_tabulam(rete->membra, t->indices,
                tr->initium, tr->numerus);
        }
        per (j = ZEPHYRUM; j < t->numerus; j++)
        {
            i32 index = t->indices[j];

            si (st->possessores[II * index] == CONTRACTIO_NULLUS)
            {
                st->possessores[II * index] = k;
            }
            alioquin
            {
                st->possessores[II * index + I] = k;
            }
            t->elementa = _multiplicare_saturate(t->elementa,
                st->dimensiones[index]);
        }
    }
    si (consilia && m > ZEPHYRUM)
    {
        st->passus_a = (i64*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i64));
        st->passus_b = (i64*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i64));
        per (j = ZEPHYRUM; j < m; j++)
        {
            st->passus_a[j] = ZEPHYRUM;
            st->passus_b[j] = ZEPHYRUM;
        }
    }
    st->vivi = (i32*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(i32));
    per (k = ZEPHYRUM; k < n; k++)
    {
        st->vivi[k] = k;
    }
}

/* gradus unus: a < b vivi in c = n + s contrahuntur. FALSUM si
 * multiplicationes summae limitem superant (candidatus abicitur). */
interior b32
_gradum_facere (
    StatusOrdinis* st,
              i32  a,
              i32  b,
          Piscina* piscina)
{
    ContractioOrdo* ordo  = st->ordo;
               i32  s     = st->gradus_facti;
               i32  c     = ordo->initiales + s;
               i64  multiplicationes;
               i32  j;
               i32  k;

    multiplicationes = _contrahere(ordo->tensores, st->possessores,
        st->dimensiones, a, b, c, piscina);
    ordo->gradus[s].sinister  = a;
    ordo->gradus[s].dexter    = b;
    ordo->gradus[s].effectus  = c;
    si (st->passus_a != NIHIL)
    {
        _gradum_parare(ordo->tensores, st->dimensiones, st->passus_a,
            st->passus_b, &ordo->gradus[s], piscina);
    }
    alioquin
    {
        ordo->gradus[s].externi          = ZEPHYRUM;
        ordo->gradus[s].interni          = ZEPHYRUM;
        ordo->gradus[s].dimensiones      = NIHIL;
        ordo->gradus[s].passus_sinistri  = NIHIL;
        ordo->gradus[s].passus_dextri    = NIHIL;
    }
    ordo->sumptus.multiplicationes = _addere_saturate(
        ordo->sumptus.multiplicationes, multiplicationes);
    si (ordo->tensores[c].elementa > ordo->sumptus.apex)
    {
        ordo->sumptus.apex = ordo->tensores[c].elementa;
    }
    k = ZEPHYRUM;
    per (j = ZEPHYRUM; j < st->numerus_vivorum; j++)
    {
        si (st->vivi[j] != a && st->vivi[j] != b)
        {
            st->vivi[k++] = st->vivi[j];
        }
    }
    st->vivi[k++]        = c;
    st->numerus_vivorum  = k;
    st->gradus_facti++;
    redde ordo->sumptus.multiplicationes <= st->limes;
}

interior vacuum
_statum_finire (
    StatusOrdinis* st,
          Piscina* piscina)
{
    _tabulam_disponere(st->ordo, piscina);
    st->ordo->sumptus.latitudo = _latitudo(st->ordo, st->dimensiones,
        piscina);
}

/* producens avidus: clavis genere (AVIDUS: T2; REDUCTIO) */
interior b32
_avide_producere (
    StatusOrdinis* st,
              i32  genus,
          Piscina* piscina)
{
    dum (st->numerus_vivorum > I)
    {
        i32 a;
        i32 b;

        _par_eligere(st->ordo->tensores, st->possessores,
            st->dimensiones,
            st->vivi, st->numerus_vivorum, genus, &a, &b);
        si (!_gradum_facere(st, a, b, piscina))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Min-fill in graphe linearum (indices vicini si tensorem communem
 * habent); eliminatio indicis = contractio duorum tensorum eius
 * (online), indices communes clausi e graphe statim tolluntur (vicini
 * eorum iam clique sunt: nulla impletio). Candidati per gradum: LXIV
 * vertices (gradus, id) minimi; electio (impletio, gradus, id) aut,
 * cum sorte, fortuita inter aequales (impletio, gradus).
 * ================================================== */

#define IMPLETIO_CANDIDATI LXIV

nomen structura {
    i32* vicini;
    i32  numerus;
    i32  capacitas;
} VicinitasIndicis;

interior vacuum
_vicinum_addere (
    VicinitasIndicis* v,
                 i32  index,
             Piscina* piscina)
{
    si (v->numerus == v->capacitas)
    {
        i32 capacitas = v->capacitas
            == ZEPHYRUM ? VIII : II * v->capacitas;
        i32* nova      = (i32*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(i32));
        i32 j;

        per (j = ZEPHYRUM; j < v->numerus; j++)
        {
            nova[j] = v->vicini[j];
        }
        v->vicini     = nova;
        v->capacitas  = capacitas;
    }
    v->vicini[v->numerus++] = index;
}

interior vacuum
_vicinum_tollere (
    VicinitasIndicis* v,
                 i32  index)
{
    i32 j;

    per (j = ZEPHYRUM; j < v->numerus; j++)
    {
        si (v->vicini[j] == index)
        {
            v->vicini[j] = v->vicini[--v->numerus];
            redde;
        }
    }
}

/* clique super vicinos ipsius v (praeter v): margines absentes
 * adduntur; si numerare, solum numerantur (nihil additur) */
interior i32
_impletio (
    VicinitasIndicis* vicinitates,
                 i32* signa,
                 i32* signum,
                 i32  v,
                 b32  addere,
             Piscina* piscina)
{
    VicinitasIndicis* nv        = &vicinitates[v];
                 i32  impletio  = ZEPHYRUM;
                 i32  p;
                 i32  q;

    per (p = ZEPHYRUM; p < nv->numerus; p++)
    {
        i32 x = nv->vicini[p];
        i32 j;

        (*signum)++;
        per (j = ZEPHYRUM; j < vicinitates[x].numerus; j++)
        {
            signa[vicinitates[x].vicini[j]] = *signum;
        }
        per (q = addere ? ZEPHYRUM : p + I; q < nv->numerus; q++)
        {
            i32 y = nv->vicini[q];

            si (y != x && signa[y] != *signum)
            {
                si (addere)
                {
                    _vicinum_addere(&vicinitates[x], y, piscina);
                }
                alioquin
                {
                    impletio++;
                }
            }
        }
    }
    redde impletio;
}

interior vacuum
_verticem_tollere (
    VicinitasIndicis* vicinitates,
                 b32* vivus,
                 i32  v)
{
    i32 j;

    per (j = ZEPHYRUM; j < vicinitates[v].numerus; j++)
    {
        _vicinum_tollere(&vicinitates[vicinitates[v].vicini[j]], v);
    }
    vicinitates[v].numerus  = ZEPHYRUM;
    vivus[v]                = FALSUM;
}

interior b32
_implendo_producere (
    StatusOrdinis* st,
             Sors* sors,
          Piscina* piscina)
{
    constans ContractioRete* rete         = st->ordo->rete;
                        i32  m            = xar_numerus(rete->indices);
                        i32  n            = st->ordo->initiales;
           VicinitasIndicis* vicinitates  = NIHIL;
                        b32* vivus        = NIHIL;
                        i32* signa        = NIHIL;
                        i32  signum       = ZEPHYRUM;
                        i32  reliqui      = m;
                        i32  k;
                        i32  j;

    si (m > ZEPHYRUM)
    {
        vicinitates = (VicinitasIndicis*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(VicinitasIndicis));
        vivus   = (b32*)piscina_allocare(piscina, (memoriae_index)m
            * magnitudo(b32));
        signa   = (i32*)piscina_allocare(piscina, (memoriae_index)m
            * magnitudo(i32));
        per (j = ZEPHYRUM; j < m; j++)
        {
            vicinitates[j].vicini     = NIHIL;
            vicinitates[j].numerus    = ZEPHYRUM;
            vicinitates[j].capacitas  = ZEPHYRUM;
            vivus[j]                  = VERUM;
            signa[j]                  = ZEPHYRUM;
        }
        /* margines: paria indicum cuiusque tensoris initialis */
        per (k = ZEPHYRUM; k < n; k++)
        {
            constans TensorOrdinis* t = &st->ordo->tensores[k];
                               i32  p;
                               i32  q;

            per (p = ZEPHYRUM; p < t->numerus; p++)
            {
                i32 x = t->indices[p];

                signum++;
                per (j = ZEPHYRUM; j < vicinitates[x].numerus; j++)
                {
                    signa[vicinitates[x].vicini[j]] = signum;
                }
                per (q = ZEPHYRUM; q < t->numerus; q++)
                {
                    i32 y = t->indices[q];

                    si (y != x && signa[y] != signum)
                    {
                        _vicinum_addere(&vicinitates[x], y, piscina);
                        signa[y] = signum;
                    }
                }
            }
        }
    }

    dum (reliqui > ZEPHYRUM)
    {
        i32 candidati[IMPLETIO_CANDIDATI];
        i32 numerus_candidatorum = ZEPHYRUM;
        i32 v;
        i32 optima_impletio  = ZEPHYRUM;
        i32 optimus_gradus   = ZEPHYRUM;
        i32 aequales         = ZEPHYRUM;
        i32 electio;
        i32 a;
        i32 b;

        /* LXIV vertices vivi (gradus, id) minimi, ordine ascendente */
        per (v = ZEPHYRUM; v < m; v++)
        {
            i32 p;

            si (!vivus[v])
            {
                perge;
            }
            si (   numerus_candidatorum == IMPLETIO_CANDIDATI
                && vicinitates[candidati[numerus_candidatorum
                    - I]].numerus
                   <= vicinitates[v].numerus)
            {
                perge;
            }
            p = numerus_candidatorum < IMPLETIO_CANDIDATI
                ? numerus_candidatorum++ : IMPLETIO_CANDIDATI - I;
            dum (   p > ZEPHYRUM
                 && vicinitates[candidati[p - I]].numerus
                    > vicinitates[v].numerus)
            {
                candidati[p] = candidati[p - I];
                p--;
            }
            candidati[p] = v;
        }

        /* (impletio, gradus) minimi; aequales ordine candidatorum */
        per (j = ZEPHYRUM; j < numerus_candidatorum; j++)
        {
            i32 w = candidati[j];
            i32 f = _impletio(vicinitates, signa, &signum, w, FALSUM,
                piscina);
            i32 g = vicinitates[w].numerus;

            si (   j == ZEPHYRUM || f < optima_impletio
                || (f == optima_impletio && g < optimus_gradus))
            {
                optima_impletio  = f;
                optimus_gradus   = g;
                aequales         = ZEPHYRUM;
            }
            si (f == optima_impletio && g == optimus_gradus)
            {
                candidati[aequales++] = w;
            }
        }
        electio = sors != NIHIL && aequales > I
            ? sors_intra(sors, aequales) : ZEPHYRUM;
        v = candidati[electio];

        /* eliminatio: tensores duo indicis v contrahuntur */
        a = st->possessores[II * v];
        b = st->possessores[II * v + I];
        (vacuum)_impletio(vicinitates, signa, &signum, v, VERUM,
            piscina);
        _verticem_tollere(vicinitates, vivus, v);
        reliqui--;
        si (!_gradum_facere(st, a < b ? a : b, a < b ? b : a, piscina))
        {
            redde FALSUM;
        }
        /* indices communes, nunc clausi: e graphe sine impletione */
        per (j = ZEPHYRUM; j < st->ordo->tensores[a].numerus; j++)
        {
            i32 u   = st->ordo->tensores[a].indices[j];
            i32 o0  = st->possessores[II * u];
            i32 o1  = st->possessores[II * u + I];

            si (   vivus[u] && (o0 == a || o0 == b)
                && (o1 == a || o1 == b))
            {
                _verticem_tollere(vicinitates, vivus, u);
                reliqui--;
            }
        }
    }

    /* reliqui tensores (partes disiunctae, scalares): clavis T2 */
    redde _avide_producere(st, CONTRACTIO_CANDIDATUS_AVIDUS, piscina);
}

/* candidatus k: producere et finire in statu novo */
interior b32
_candidatum_currere (
        constans ContractioRete* rete,
                            i32  k,
    constans ContractioOptiones* optiones,
                            b32  consilia,
                            i64  limes,
                        Piscina* piscina,
                  StatusOrdinis* st)
{
     b32 bene;
    Sors sors;

    _statum_incipere(rete, consilia, limes, piscina, st);
    si (   k == CONTRACTIO_CANDIDATUS_AVIDUS
        || k == CONTRACTIO_CANDIDATUS_REDUCTIO)
    {
        bene = _avide_producere(st, k, piscina);
    }
    alioquin si (k == CONTRACTIO_CANDIDATUS_IMPLETIO)
    {
        bene = _implendo_producere(st, NIHIL, piscina);
    }
    alioquin
    {
        sors_seminare(&sors, optiones->semen,
            (i64)(k - CONTRACTIO_CANDIDATUS_FORTUITUS));
        bene = _implendo_producere(st, &sors, piscina);
    }
    si (bene)
    {
        _statum_finire(st, piscina);
    }
    redde bene;
}

/* clavis electionis: aptus (octeti <= limes) ante non aptum; inter
 * aptos (multiplicationes, octeti, k); inter non aptos (octeti, k) */
interior b32
_melior (
    constans ContractioSumptus* s,
                           i32  k,
    constans ContractioSumptus* optimus,
                           i32  optimus_k,
                           i64  octeti_maximi)
{
    b32 aptus          = s->octeti_officinae <= octeti_maximi;
    b32 optimus_aptus  = optimus->octeti_officinae <= octeti_maximi;

    si (aptus != optimus_aptus)
    {
        redde aptus;
    }
    si (aptus && s->multiplicationes != optimus->multiplicationes)
    {
        redde s->multiplicationes < optimus->multiplicationes;
    }
    si (s->octeti_officinae != optimus->octeti_officinae)
    {
        redde s->octeti_officinae < optimus->octeti_officinae;
    }
    redde k < optimus_k;
}

ContractioOrdo*
contractio_ordinare (
                ContractioRete* rete,
    constans ContractioOptiones* optiones,
                       Piscina* piscina)
{
        StatusOrdinis  st;
    ContractioSumptus* candidati;
                  b32* aestimati;
                  i32* optimi_gradus;
                  i32  numerus;
                  i32  optimus_k = ZEPHYRUM;
                  i32  k;
                  i32  s;

    si (   rete == NIHIL || optiones == NIHIL || piscina == NIHIL
        || (   optiones->methodus != CONTRACTIO_AVIDUS
            && optiones->methodus != CONTRACTIO_OPTIMUS)
        || (   optiones->methodus == CONTRACTIO_OPTIMUS
            && (   optiones->conatus > CONTRACTIO_CONATUS_MAXIMI
                || optiones->candidatus > CONTRACTIO_CANDIDATUS_IMPLETIO
                   + optiones->conatus))
        || !contractio_rete_clausum(rete))
    {
        redde NIHIL;
    }
    rete->ordinatum = VERUM;

    si (optiones->methodus == CONTRACTIO_AVIDUS)
    {
        (vacuum)_candidatum_currere(rete, CONTRACTIO_CANDIDATUS_AVIDUS,
            optiones, VERUM, CONTRACTIO_SATURATUM, piscina, &st);
        st.ordo->numerus_candidatorum  = I;
        st.ordo->candidati             = (ContractioSumptus*)
            piscina_allocare(piscina, magnitudo(ContractioSumptus));
        st.ordo->candidati_aestimati   = (b32*)piscina_allocare(piscina,
            magnitudo(b32));
        st.ordo->candidati[ZEPHYRUM]            = st.ordo->sumptus;
        st.ordo->candidati_aestimati[ZEPHYRUM]  = VERUM;
        redde st.ordo;
    }

    /* OPTIMUS: candidati in regione refecta, gradus victoris servati */
    numerus        = CONTRACTIO_CANDIDATUS_IMPLETIO + optiones->conatus;
    candidati      = (ContractioSumptus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(ContractioSumptus));
    aestimati      = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(b32));
    optimi_gradus  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * xar_numerus(rete->tensores) + I)
        * magnitudo(i32));
    per (k = I; k <= numerus; k++)
    {
        PiscinaNotatio nota;
                   i64 limes = CONTRACTIO_SATURATUM;

        aestimati[k - I] = FALSUM;
        si (   optiones->candidatus != CONTRACTIO_CANDIDATUS_OMNES
            && optiones->candidatus != k)
        {
            perge;
        }
        si (   optimus_k != ZEPHYRUM
            && candidati[optimus_k - I].octeti_officinae
               <= optiones->octeti_maximi)
        {
            limes = candidati[optimus_k - I].multiplicationes;
            si (limes == CONTRACTIO_SATURATUM)
            {
                /* incumbens saturatus: candidatus saturans abicitur
                 * (officina ordinem saturatum utcumque refutat) */
                limes = CONTRACTIO_SATURATUM - I;
            }
        }
        nota = piscina_notare(piscina);
        si (_candidatum_currere(rete, k, optiones, FALSUM, limes,
            piscina,
            &st))
        {
            candidati[k - I] = st.ordo->sumptus;
            aestimati[k - I] = VERUM;
            si (   optimus_k == ZEPHYRUM
                || _melior(&candidati[k - I], k,
                       &candidati[optimus_k - I], optimus_k,
                       optiones->octeti_maximi))
            {
                optimus_k = k;
                per (s = ZEPHYRUM; s < st.ordo->numerus_graduum; s++)
                {
                    optimi_gradus[II * s] =
                        st.ordo->gradus[s].sinister;
                    optimi_gradus[II * s + I] =
                        st.ordo->gradus[s].dexter;
                }
            }
        }
        piscina_reficere(piscina, nota);
        si (   k == CONTRACTIO_CANDIDATUS_AVIDUS && aestimati[ZEPHYRUM]
            && optiones->candidatus == CONTRACTIO_CANDIDATUS_OMNES
            && candidati[ZEPHYRUM].multiplicationes < optiones->limen
            && candidati[ZEPHYRUM].octeti_officinae
               <= optiones->octeti_maximi)
        {
            /* ordo avidus iam vilis: ceteri non aestimantur */
            per (k = k + I; k <= numerus; k++)
            {
                aestimati[k - I] = FALSUM;
            }
            frange;
        }
    }

    /* victor materializatur: gradus iterantur cum consiliis ansae */
    _statum_incipere(rete, VERUM, CONTRACTIO_SATURATUM, piscina, &st);
    per (s = ZEPHYRUM; s < st.ordo->numerus_graduum; s++)
    {
        (vacuum)_gradum_facere(&st, optimi_gradus[II * s],
            optimi_gradus[II * s + I], piscina);
    }
    _statum_finire(&st, piscina);
    st.ordo->candidatus            = optimus_k;
    st.ordo->numerus_candidatorum  = numerus;
    st.ordo->candidati             = candidati;
    st.ordo->candidati_aestimati   = aestimati;
    redde st.ordo;
}

ContractioSumptus
contractio_sumptus (
    constans ContractioOrdo* ordo)
{
    ContractioSumptus nullus;

    si (ordo == NIHIL)
    {
        nullus.apex              = ZEPHYRUM;
        nullus.octeti_officinae  = ZEPHYRUM;
        nullus.multiplicationes  = ZEPHYRUM;
        nullus.latitudo          = ZEPHYRUM;
        redde nullus;
    }
    redde ordo->sumptus;
}


/* ==================================================
 * Inspectio ordinis
 * ================================================== */

i32
contractio_ordo_gradus_numerus (
    constans ContractioOrdo* ordo)
{
    redde ordo == NIHIL ? ZEPHYRUM : ordo->numerus_graduum;
}

b32
contractio_ordo_gradus (
    constans ContractioOrdo* ordo,
                        i32  k,
                        i32* sinister,
                        i32* dexter,
                        i32* effectus)
{
    si (ordo == NIHIL || k >= ordo->numerus_graduum || sinister == NIHIL
        || dexter == NIHIL || effectus == NIHIL)
    {
        redde FALSUM;
    }
    *sinister  = ordo->gradus[k].sinister;
    *dexter    = ordo->gradus[k].dexter;
    *effectus  = ordo->gradus[k].effectus;
    redde VERUM;
}

b32
contractio_ordo_sedes (
    constans ContractioOrdo* ordo,
                        i32  tensor,
                        i64* offset,
                        i64* elementa)
{
    si (   ordo == NIHIL || offset == NIHIL || elementa == NIHIL
        || ordo->tabula_saturata || tensor < ordo->initiales
        || tensor - ordo->initiales >= ordo->numerus_graduum)
    {
        redde FALSUM;
    }
    *offset    = ordo->tensores[tensor].offset;
    *elementa  = ordo->tensores[tensor].elementa;
    redde VERUM;
}

b32
contractio_ordo_indices (
    constans ContractioOrdo*  ordo,
                        i32   tensor,
               constans i32** indices,
                        i32*  numerus)
{
    si (   ordo   == NIHIL || indices == NIHIL || numerus == NIHIL
        || tensor >= ordo->initiales + ordo->numerus_graduum)
    {
        redde FALSUM;
    }
    *indices = ordo->tensores[tensor].indices;
    *numerus = ordo->tensores[tensor].numerus;
    redde VERUM;
}

i32
contractio_ordo_candidatus (
    constans ContractioOrdo* ordo)
{
    redde ordo == NIHIL ? ZEPHYRUM : ordo->candidatus;
}

b32
contractio_ordo_candidati (
    constans ContractioOrdo* ordo,
                        i32  k,
          ContractioSumptus* simulatus)
{
    si (   ordo == NIHIL || simulatus == NIHIL || k == ZEPHYRUM
        || k > ordo->numerus_candidatorum
        || !ordo->candidati_aestimati[k - I])
    {
        redde FALSUM;
    }
    *simulatus = ordo->candidati[k - I];
    redde VERUM;
}


/* ==================================================
 * Exsecutio
 * ================================================== */

/* Multiplicationes non saturatae (< 2^64) implicant quemque gradum
 * habere < 64 indices dimensionis >= 2 in unione: numeratores
 * exsecutionis (T3) in acervo fixo capiunt - SI indices dimensionis
 * 1 omittuntur (eorum numerus non finitur: 100 indices dimensionis 1
 * communes dant multiplicationem unam). */
ContractioOfficina*
contractio_officina_creare (
    constans ContractioOrdo* ordo,
                       i64  octeti_maximi,
                   Piscina* piscina)
{
     ContractioOfficina* officina;
                    i64  octeti;

    si (ordo == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    /* limes 2^62: piscina mensuram prope 2^64 non tuto tractat
     * (offset + mensura exundat); nullum rete notum huc pervenit */
    octeti = ordo->sumptus.octeti_officinae;
    si (   octeti                         == CONTRACTIO_SATURATUM
        || ordo->sumptus.multiplicationes == CONTRACTIO_SATURATUM
        || octeti > ((i64)I << LXII)
        || octeti > octeti_maximi)
    {
        redde NIHIL;
    }
    officina = (ContractioOfficina*)piscina_conari_allocare(piscina,
        magnitudo(ContractioOfficina));
    si (officina == NIHIL)
    {
        redde NIHIL;
    }
    officina->ordo    = ordo;
    officina->tabula  = NIHIL;
    si (octeti > ZEPHYRUM)
    {
        officina->tabula = (i32*)piscina_conari_allocare(piscina,
            (memoriae_index)octeti);
        si (officina->tabula == NIHIL)
        {
            redde NIHIL;
        }
    }
    redde officina;
}

/* numeratores odometri: < 64 indices dimensionis >= 2 per gradum
 * (multiplicationes non saturatae, vide supra) */
#define CONTRACTIO_NUMERATORES LXIV

/* odometrum super indices consilii [primus, finis): assignatio
 * proxima, offsets in operandis renovati. FALSUM si exhaustum -
 * numeratores tunc nulli et offsets ad initium redierunt. */
interior b32
_numeratores_promovere (
    constans GradusOrdinis* g,
                       i32* numeratores,
                       i32  primus,
                       i32  finis,
                       i64* offset_a,
                       i64* offset_b)
{
    i32 k = finis;

    dum (k > primus)
    {
        k--;
        numeratores[k]++;
        *offset_a += g->passus_sinistri[k];
        *offset_b += g->passus_dextri[k];
        si (numeratores[k] < g->dimensiones[k])
        {
            redde VERUM;
        }
        *offset_a       -= g->passus_sinistri[k] * g->dimensiones[k];
        *offset_b       -= g->passus_dextri[k] * g->dimensiones[k];
        numeratores[k]  = ZEPHYRUM;
    }
    redde FALSUM;
}

/* gradus calidus: pro quoque elemento effectus summa productorum in
 * accumulatore duorum verborum (humile + numerus portationum: exacta
 * pro omni longitudine), reductio semel: (portatio * 2^64 + humile)
 * mod n, residuum_verbi = 2^64 mod n */
interior vacuum
_gradum_modulo (
    constans GradusOrdinis* g,
              constans i32* sinister,
              constans i32* dexter,
                       i32* effectus,
                       i64  elementa,
                       i64  modulus,
                       i64  residuum_verbi)
{
    i32 numeratores[CONTRACTIO_NUMERATORES];
    i32 summus  = g->externi + g->interni;
    i64 oa      = ZEPHYRUM;
    i64 ob      = ZEPHYRUM;
    i64 o;
    i32 k;

    per (k = ZEPHYRUM; k < summus; k++)
    {
        numeratores[k] = ZEPHYRUM;
    }
    per (o = ZEPHYRUM; o < elementa; o++)
    {
        i64 humile    = ZEPHYRUM;
        i64 portatio  = ZEPHYRUM;

        si (g->interni == ZEPHYRUM)
        {
            humile = (i64)sinister[oa] * (i64)dexter[ob];
        }
        alioquin
        {
            /* index communis ultimus in ansa stricta, ceteri per
             * odometrum */
            i32 ultimus  = summus - I;
            i32 d        = g->dimensiones[ultimus];
            i64 pa       = g->passus_sinistri[ultimus];
            i64 pb       = g->passus_dextri[ultimus];
            i64 ia       = oa;
            i64 ib       = ob;

            fac
            {
                i64 xa = ia;
                i64 xb = ib;
                i32 j;

                per (j = ZEPHYRUM; j < d; j++)
                {
                    i64 productum = (i64)sinister[xa] * (i64)dexter[xb];

                    humile += productum;
                    si (humile < productum)
                    {
                        portatio++;
                    }
                    xa += pa;
                    xb += pb;
                }
            }
            dum (_numeratores_promovere(g, numeratores, g->externi,
                ultimus, &ia, &ib));
        }
        effectus[o] = (i32)(((portatio % modulus) * residuum_verbi
            % modulus
            + humile % modulus) % modulus);
        (vacuum)_numeratores_promovere(g, numeratores, ZEPHYRUM,
            g->externi, &oa, &ob);
    }
}

/* operandum calidum: initialis ex valoribus, intermedius ex tabula */
interior constans i32*
_operandum_modulo (
       constans ContractioOrdo* ordo,
    constans ContractioOfficina* officina,
         constans i32* constans* valores,
                            i32  tensor)
{
    si (tensor < ordo->initiales)
    {
        redde valores[tensor];
    }
    redde officina->tabula + ordo->tensores[tensor].offset;
}

b32
contractio_exsequi_modulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
         ContractioOfficina*    officina,
    constans i32* constans*     valores,
                        i32     modulus,
                        i32*    exitus)
{
    i64 residuum_verbi;
    i32 n;
    i32 s;
    i32 k;

    si (   rete           == NIHIL || ordo == NIHIL || officina == NIHIL
        || exitus         == NIHIL || ordo->rete != rete
        || officina->ordo != ordo || modulus < II)
    {
        redde FALSUM;
    }
    n = ordo->initiales;
    si (n == ZEPHYRUM)
    {
        *exitus = I;
        redde VERUM;
    }
    si (valores == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (valores[k] == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        *exitus = valores[ZEPHYRUM][ZEPHYRUM] % modulus;
        redde VERUM;
    }
    residuum_verbi = (CONTRACTIO_SATURATUM % (i64)modulus + I)
        % (i64)modulus;
    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans GradusOrdinis* g = &ordo->gradus[s];
        constans TensorOrdinis* c = &ordo->tensores[g->effectus];

        _gradum_modulo(g,
            _operandum_modulo(ordo, officina, valores, g->sinister),
            _operandum_modulo(ordo, officina, valores, g->dexter),
            officina->tabula + c->offset, c->elementa, (i64)modulus,
            residuum_verbi);
    }
    *exitus = officina->tabula[ordo->tensores[ordo->gradus[
        ordo->numerus_graduum - I].effectus].offset];
    redde VERUM;
}

/* elementum anuli ad indicem in tabula elementorum */
interior constans vacuum*
_elementum (
    constans vacuum* tabula,
                i64  index,
     memoriae_index  mensura)
{
    redde (constans character*)tabula + (memoriae_index)index * mensura;
}

/* gradus referentiae: eadem ansa, operationes anuli; summa cuiusque
 * elementi in scrinio, transcripta in servatam, scrinium refectum */
interior b32
_gradum_anulo (
    constans GradusOrdinis* g,
           constans vacuum* sinister,
           constans vacuum* dexter,
                 character* effectus,
                       i64  elementa,
           constans Anulus* anulus,
                   Piscina* servata,
                   Piscina* scrinium,
                 character* officium)
{
    memoriae_index mensura = anulus->mensura;
               i32 numeratores[CONTRACTIO_NUMERATORES];
               i32 summus  = g->externi + g->interni;
               i64 oa      = ZEPHYRUM;
               i64 ob      = ZEPHYRUM;
               i64 o;
               i32 k;

    per (k = ZEPHYRUM; k < summus; k++)
    {
        numeratores[k] = ZEPHYRUM;
    }
    per (o = ZEPHYRUM; o < elementa; o++)
    {
         PiscinaNotatio  nota       = piscina_notare(scrinium);
              character* summa      = officium;
              character* altera     = officium + mensura;
              character* productum  = officium + II * mensura;
                    i64  ia         = oa;
                    i64  ib         = ob;

        anulus->nullum(anulus, summa);
        fac
        {
            character* t;

            si (   !anulus->multiplica(anulus,
                       _elementum(sinister, ia, mensura),
                       _elementum(dexter, ib, mensura), scrinium,
                       productum)
                || !anulus->adde(anulus, summa, productum, scrinium,
                       altera))
            {
                piscina_reficere(scrinium, nota);
                redde FALSUM;
            }
            t       = summa;
            summa   = altera;
            altera  = t;
        }
        dum (_numeratores_promovere(g, numeratores, g->externi, summus,
            &ia, &ib));
        anulus->transcribe(anulus, summa, servata,
            effectus + (memoriae_index)o * mensura);
        piscina_reficere(scrinium, nota);
        (vacuum)_numeratores_promovere(g, numeratores, ZEPHYRUM,
            g->externi, &oa, &ob);
    }
    redde VERUM;
}

/* intermedii in 'servata', operationes in 'scrinio'; FALSUM si
 * allocatio aut operatio anuli deficit */
interior b32
_exsequi_anulo_intra (
           constans ContractioOrdo* ordo,
         constans vacuum* constans* valores,
                   constans Anulus* anulus,
                           Piscina* servata,
                           Piscina* scrinium,
                           Piscina* piscina,
                            vacuum* exitus)
{
     memoriae_index   mensura = anulus->mensura;
    constans vacuum** data;
          character*  officium;
                i32   totales = ordo->initiales + ordo->numerus_graduum;
                i32   s;
                i32   k;

    data = (constans vacuum**)piscina_conari_allocare(servata,
        (memoriae_index)totales * magnitudo(constans vacuum*));
    officium = (character*)piscina_conari_allocare(servata,
        III * mensura);
    si (data == NIHIL || officium == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ordo->initiales; k++)
    {
        data[k] = valores[k];
    }
    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans GradusOrdinis* g = &ordo->gradus[s];
                           i64  elementa =
                               ordo->tensores[g->effectus].elementa;
                     character* effectus;

        /* limes 2^62 octetorum, ut officina: piscina prope 2^64
         * exundat (tabularium 01M5YW4S...) */
        si (elementa > ((i64)I << LXII) / (i64)mensura)
        {
            redde FALSUM;
        }
        effectus = (character*)piscina_conari_allocare(servata,
            (memoriae_index)elementa * mensura);
        si (   effectus == NIHIL
            || !_gradum_anulo(g, data[g->sinister], data[g->dexter],
                   effectus, elementa, anulus, servata, scrinium,
                   officium))
        {
            redde FALSUM;
        }
        data[g->effectus] = effectus;
    }
    anulus->transcribe(anulus,
        data[ordo->gradus[ordo->numerus_graduum - I].effectus], piscina,
        exitus);
    redde VERUM;
}

b32
contractio_exsequi_anulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
    constans vacuum* constans*  valores,
           constans Anulus*     anulus,
                    Piscina*    piscina,
                     vacuum*    exitus)
{
     Piscina* servata;
     Piscina* scrinium;
         b32  bene;
         i32  k;

    si (   rete == NIHIL || ordo == NIHIL || anulus == NIHIL
        || piscina == NIHIL || exitus == NIHIL || ordo->rete != rete
        || ordo->sumptus.multiplicationes == CONTRACTIO_SATURATUM
        || ordo->sumptus.apex == CONTRACTIO_SATURATUM)
    {
        redde FALSUM;
    }
    si (ordo->initiales == ZEPHYRUM)
    {
        anulus->unum(anulus, piscina, exitus);
        redde VERUM;
    }
    si (valores == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ordo->initiales; k++)
    {
        si (valores[k] == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        anulus->transcribe(anulus, valores[ZEPHYRUM], piscina, exitus);
        redde VERUM;
    }
    servata   = piscina_generare_dynamicum("contractio_servata", 4096);
    scrinium  = piscina_generare_dynamicum("contractio_scrinium", 4096);
    bene     = servata != NIHIL && scrinium != NIHIL
        && _exsequi_anulo_intra(ordo, valores, anulus, servata,
        scrinium,
               piscina, exitus);
    si (servata != NIHIL)
    {
        piscina_destruere(servata);
    }
    si (scrinium != NIHIL)
    {
        piscina_destruere(scrinium);
    }
    redde bene;
}
#undef _addere_saturate
#undef _alter
#undef _avide_producere
#undef _bita
#undef _candidatum_currere
#undef _clavis_minor
#undef _clavis_paris
#undef _contrahere
#undef _elementa_effectus
#undef _elementum
#undef _exsequi_anulo_intra
#undef _gradum_anulo
#undef _gradum_facere
#undef _gradum_modulo
#undef _gradum_parare
#undef _implendo_producere
#undef _impletio
#undef _latitudo
#undef _melior
#undef _movet
#undef _multiplicare_saturate
#undef _numeratores_promovere
#undef _operandum_modulo
#undef _par_eligere
#undef _passus_ponere
#undef _signa_delere
#undef _statum_finire
#undef _statum_incipere
#undef _tabulam_disponere
#undef _verticem_tollere
#undef _vicinum_addere
#undef _vicinum_tollere
#undef CONTRACTIO_NULLUS
#undef CONTRACTIO_NUMERATORES
#undef CONTRACTIO_USUS_SIGNUM
#undef GradusOrdinis
#undef IMPLETIO_CANDIDATI
#undef IndexRetis
#undef IntervallumTabulae
#undef StatusOrdinis
#undef TensorOrdinis
#undef TensorRetis
#undef VicinitasIndicis
/* lib/situs.c: statica per plagulam renominata */
#define _axis_non_nulla _axis_non_nulla_situs
#define _coordinata _coordinata_situs
#define _crux _crux_situs
#define _in_triangulo _in_triangulo_situs
#define _inter_plana _inter_plana_situs
#define _inter_proiectum _inter_proiectum_situs
#define _minus _minus_situs
#define _nullus _nullus_situs
#define _omissa _omissa_situs
#define _orientatio_plana _orientatio_plana_situs
#define _scalare _scalare_situs
#define _scalare_proiectum _scalare_proiectum_situs
#define _segmenta _segmenta_situs
#define _segmenta_plana _segmenta_plana_situs
#define _segmenta_vicina _segmenta_vicina_situs
#define _signum_triplex _signum_triplex_situs
#define _transitus _transitus_situs
#define _transitus_vicinus _transitus_vicinus_situs
#define _triangulum_segmentum _triangulum_segmentum_situs
#define _triangulum_vicinum _triangulum_vicinum_situs
#define _triplex _triplex_situs
#line 1 "lib/situs.c"
/* situs.c - Praedicata geometrica exacta super fractionem
 *
 * Omnia signa determinantium: orientatio plana = productum crucis,
 * orientatio spatii = productum triplex det[u, v, w] = u . (v x w).
 * Casus degeneres per probationes "inter" (coordinatae aut producta
 * scalaria) nominantur. Transitus sine coordinatis proiectis: in
 * proiectione secundum v, crux plana fit det[x, y, v] et scalare planum
 * (x.y)(v.v) - (x.v)(y.v) - utrumque per factorem positivum
 * multiplicatum (|v|, |v|^2), ergo signa exacta. Functiones publicae
 * quae signum aut genus reddunt piscinam notant et reficiunt.
 * Vide lib/situs.worklog.md.
 */



/* ==================================================
 * Auxilia vectorum (piscina sine refectione)
 * ================================================== */

/* a - b */
interior Punctum
_minus (
     Punctum  a,
     Punctum  b,
     Piscina* piscina)
{
    Punctum r;

    r.x = fractio_subtrahe(a.x, b.x, piscina);
    r.y = fractio_subtrahe(a.y, b.y, piscina);
    r.z = fractio_subtrahe(a.z, b.z, piscina);
    redde r;
}

interior Fractio
_scalare (
     Punctum  u,
     Punctum  v,
     Piscina* piscina)
{
    redde fractio_adde(fractio_adde(
        fractio_multiplica(u.x, v.x, piscina),
        fractio_multiplica(u.y, v.y, piscina), piscina),
        fractio_multiplica(u.z, v.z, piscina), piscina);
}

interior Punctum
_crux (
     Punctum  u,
     Punctum  v,
     Piscina* piscina)
{
    Punctum r;

    r.x = fractio_subtrahe(fractio_multiplica(u.y, v.z, piscina),
        fractio_multiplica(u.z, v.y, piscina), piscina);
    r.y = fractio_subtrahe(fractio_multiplica(u.z, v.x, piscina),
        fractio_multiplica(u.x, v.z, piscina), piscina);
    r.z = fractio_subtrahe(fractio_multiplica(u.x, v.y, piscina),
        fractio_multiplica(u.y, v.x, piscina), piscina);
    redde r;
}

/* det[u, v, w] = u . (v x w) */
interior Fractio
_triplex (
     Punctum  u,
     Punctum  v,
     Punctum  w,
     Piscina* piscina)
{
    redde _scalare(u, _crux(v, w, piscina), piscina);
}

interior s32
_signum_triplex (
     Punctum  u,
     Punctum  v,
     Punctum  w,
     Piscina* piscina)
{
    redde fractio_signum(_triplex(u, v, w, piscina));
}

interior b32
_nullus (
    Punctum u)
{
    redde fractio_signum(u.x) == ZEPHYRUM
        && fractio_signum(u.y) == ZEPHYRUM
        && fractio_signum(u.z) == ZEPHYRUM;
}

/* coordinata k (0 x, 1 y, 2 z) */
interior Fractio
_coordinata (
    Punctum a,
        s32 k)
{
    si (k == ZEPHYRUM)
    {
        redde a.x;
    }
    si (k == I)
    {
        redde a.y;
    }
    redde a.z;
}

/* proiectio in planum coordinatarum omissa coordinata k */
interior PunctumPlani
_omissa (
    Punctum a,
        s32 k)
{
    PunctumPlani r;

    r.x = (k == ZEPHYRUM) ? a.y : a.x;
    r.y = (k == II) ? a.y : a.z;
    redde r;
}

/* axis ubi componens vectoris non nulla est (-1 si nullus) */
interior s32
_axis_non_nulla (
    Punctum u)
{
    s32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (fractio_signum(_coordinata(u, k)) != ZEPHYRUM)
        {
            redde k;
        }
    }
    redde -I;
}


/* ==================================================
 * Planum
 * ================================================== */

interior s32
_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina)
{
    Fractio crux = fractio_subtrahe(
        fractio_multiplica(fractio_subtrahe(b.x, a.x, piscina),
            fractio_subtrahe(c.y, a.y, piscina), piscina),
        fractio_multiplica(fractio_subtrahe(b.y, a.y, piscina),
            fractio_subtrahe(c.x, a.x, piscina), piscina), piscina);

    redde fractio_signum(crux);
}

/* r inter extrema p et q (singulis coordinatis; r collineare datum) */
interior b32
_inter_plana (
    PunctumPlani  p,
    PunctumPlani  q,
    PunctumPlani  r,
         Piscina* piscina)
{
    redde fractio_compara(r.x, p.x, piscina) *
              fractio_compara(r.x, q.x, piscina) <= ZEPHYRUM
        && fractio_compara(r.y, p.y, piscina) *
              fractio_compara(r.y, q.y, piscina) <= ZEPHYRUM;
}

interior SitusContactus
_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina)
{
    s32 o1 = _orientatio_plana(a, b, c, piscina);
    s32 o2 = _orientatio_plana(a, b, d, piscina);
    s32 o3 = _orientatio_plana(c, d, a, piscina);
    s32 o4 = _orientatio_plana(c, d, b, piscina);

    si (o1 * o2 < ZEPHYRUM && o3 * o4 < ZEPHYRUM)
    {
        redde SITUS_SECANT;
    }
    si (   (o1 == ZEPHYRUM && _inter_plana(a, b, c, piscina))
        || (o2 == ZEPHYRUM && _inter_plana(a, b, d, piscina))
        || (o3 == ZEPHYRUM && _inter_plana(c, d, a, piscina))
        || (o4 == ZEPHYRUM && _inter_plana(c, d, b, piscina)))
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Spatium
 * ================================================== */

interior SitusContactus
_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    Punctum ab = _minus(b, a, piscina);
    Punctum normalis;
        s32 k;

    si (_signum_triplex(ab, _minus(c, a, piscina), _minus(d, a,
        piscina),
            piscina) != ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;
    }

    /* coplanaria: normalis non nulla ex tribus punctis non
     * collinearibus */
    normalis = _crux(ab, _minus(c, a, piscina), piscina);
    si (_nullus(normalis))
    {
        normalis = _crux(ab, _minus(d, a, piscina), piscina);
    }
    si (_nullus(normalis))
    {
        Punctum cd = _minus(d, c, piscina);

        normalis = _crux(cd, _minus(a, c, piscina), piscina);
        si (_nullus(normalis))
        {
            normalis = _crux(cd, _minus(b, c, piscina), piscina);
        }
    }
    k = _axis_non_nulla(normalis);
    si (k >= ZEPHYRUM)
    {
        /* proiectio omissa axe ubi normalis non nulla: iniectiva in
         * plano */
        redde _segmenta_plana(_omissa(a, k), _omissa(b, k), _omissa(c,
            k),
            _omissa(d, k), piscina);
    }

    /* omnia collinearia: intervalla in coordinata ubi recta non est
     * perpendicularis */
    {
        Punctum directio = ab;
        Fractio imum_primum;
        Fractio summum_primum;
        Fractio imum_alterum;
        Fractio summum_alterum;

        si (_nullus(directio))
        {
            directio = _minus(d, c, piscina);
        }
        si (_nullus(directio))
        {
            directio = _minus(c, a, piscina);
        }
        k = _axis_non_nulla(directio);
        si (k < ZEPHYRUM)
        {
            redde SITUS_TANGUNT;   /* puncta omnia eadem */
        }
        imum_primum    = _coordinata(a, k);
        summum_primum  = _coordinata(b, k);
        si (fractio_compara(imum_primum, summum_primum, piscina)
            > ZEPHYRUM)
        {
            Fractio t = imum_primum;

            imum_primum    = summum_primum;
            summum_primum  = t;
        }
        imum_alterum    = _coordinata(c, k);
        summum_alterum  = _coordinata(d, k);
        si (fractio_compara(imum_alterum, summum_alterum, piscina)
            > ZEPHYRUM)
        {
            Fractio t = imum_alterum;

            imum_alterum    = summum_alterum;
            summum_alterum  = t;
        }
        si (   fractio_compara(imum_primum, summum_alterum, piscina)
            <= ZEPHYRUM
            && fractio_compara(imum_alterum, summum_primum, piscina)
                <= ZEPHYRUM)
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }
}

/* [a,b] et [b,c] vertice b communi: aliud punctum commune sse
 * collinearia eadem directione ab b (u x w nullum, u . w > 0) */
interior SitusContactus
_segmenta_vicina (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Piscina* piscina)
{
    Punctum u = _minus(a, b, piscina);
    Punctum w = _minus(c, b, piscina);

    si (_nullus(u) || _nullus(w))
    {
        redde SITUS_TANGUNT;   /* segmentum nullum: punctum ipsum b */
    }
    si (   _nullus(_crux(u, w, piscina))
        && fractio_signum(_scalare(u, w, piscina)) > ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}

/* p coplanare cum triangulo non degeneri: intra triangulum clausum? */
interior b32
_in_triangulo (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  normalis,
     Punctum  p,
     Piscina* piscina)
{
             s32 k   = _axis_non_nulla(normalis);
    PunctumPlani a   = _omissa(t0, k);
    PunctumPlani b   = _omissa(t1, k);
    PunctumPlani c   = _omissa(t2, k);
    PunctumPlani r   = _omissa(p, k);
             s32 o1  = _orientatio_plana(a, b, r, piscina);
             s32 o2  = _orientatio_plana(b, c, r, piscina);
             s32 o3  = _orientatio_plana(c, a, r, piscina);

    redde (o1 >= ZEPHYRUM && o2 >= ZEPHYRUM && o3 >= ZEPHYRUM)
        || (o1 <= ZEPHYRUM && o2 <= ZEPHYRUM && o3 <= ZEPHYRUM);
}

interior SitusContactus
_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina)
{
    Punctum e1        = _minus(t1, t0, piscina);
    Punctum e2        = _minus(t2, t0, piscina);
    Punctum normalis  = _crux(e1, e2, piscina);
        s32 latus_p;
        s32 latus_q;
        s32 s0;
        s32 s1;
        s32 s2;
    Punctum directio_segmenti;

    /* triangulum degener (collineare): contactus cum marginibus
     * solus */
    si (_nullus(normalis))
    {
        si (   _segmenta(t0, t1, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t1, t2, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t2, t0, p, q, piscina) != SITUS_DISIUNCTA)
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }

    latus_p = fractio_signum(_scalare(normalis, _minus(p, t0, piscina),
        piscina));
    latus_q = fractio_signum(_scalare(normalis, _minus(q, t0, piscina),
        piscina));
    si (latus_p * latus_q > ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;   /* ex eadem parte plani */
    }

    si (latus_p == ZEPHYRUM && latus_q == ZEPHYRUM)
    {
        /* segmentum coplanare: contactus = margo tacta aut extremum
         * intra (tunc segmentum totum intra esse potest) */
        si (   _segmenta(t0, t1, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t1, t2, p, q, piscina) != SITUS_DISIUNCTA
            || _segmenta(t2, t0, p, q, piscina) != SITUS_DISIUNCTA
            || _in_triangulo(t0, t1, t2, normalis, p, piscina))
        {
            redde SITUS_TANGUNT;
        }
        redde SITUS_DISIUNCTA;
    }

    /* recta pq planum in puncto uno secat (intra segmentum, quia
     * latus_p * latus_q <= 0): intra triangulum clausum sse s0, s1,
     * s2 signis strictis non discrepant */
    directio_segmenti = _minus(q, p, piscina);
    s0 = _signum_triplex(directio_segmenti, _minus(t0, p, piscina),
        _minus(t1, p,
        piscina), piscina);
    s1 = _signum_triplex(directio_segmenti, _minus(t1, p, piscina),
        _minus(t2, p,
        piscina), piscina);
    s2 = _signum_triplex(directio_segmenti, _minus(t2, p, piscina),
        _minus(t0, p,
        piscina), piscina);
    si (   (s0 > ZEPHYRUM || s1 > ZEPHYRUM || s2 > ZEPHYRUM)
        && (s0 < ZEPHYRUM || s1 < ZEPHYRUM || s2 < ZEPHYRUM))
    {
        redde SITUS_DISIUNCTA;
    }
    si (   latus_p == ZEPHYRUM || latus_q == ZEPHYRUM
        || s0      == ZEPHYRUM || s1 == ZEPHYRUM || s2 == ZEPHYRUM)
    {
        /* extremum in plano, aut margo/vertex */
        redde SITUS_TANGUNT;
    }
    redde SITUS_SECANT;
}

/* segmentum [t0,x] et triangulum [t0,t1,t2] vertice t0 communi.
 * Triangulum convexum t0 continet, ergo aliud punctum commune sse
 * d = x - t0 coplanaris et in cono clauso {alpha e1 + beta e2: alpha,
 * beta >= 0}. Pro d = alpha e1 + beta e2: (e1 x d) . n = beta |n|^2 et
 * (d x e2) . n = alpha |n|^2, ergo signa coefficientes nominant. */
interior SitusContactus
_triangulum_vicinum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  x,
     Piscina* piscina)
{
    Punctum e1        = _minus(t1, t0, piscina);
    Punctum e2        = _minus(t2, t0, piscina);
    Punctum d         = _minus(x, t0, piscina);
    Punctum normalis  = _crux(e1, e2, piscina);

    si (_nullus(d) || _nullus(normalis))
    {
        /* segmentum nullum aut triangulum degener */
        redde SITUS_TANGUNT;
    }
    si (fractio_signum(_scalare(normalis, d, piscina)) != ZEPHYRUM)
    {
        redde SITUS_DISIUNCTA;   /* planum in t0 solo transit */
    }
    si (   fractio_signum(_scalare(_crux(e1, d, piscina), normalis,
               piscina)) >= ZEPHYRUM
        && fractio_signum(_scalare(_crux(d, e2, piscina), normalis,
               piscina)) >= ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Proiectio secundum v
 * ================================================== */

/* scalare in proiectione (multiplicatum per v.v > 0) */
interior Fractio
_scalare_proiectum (
     Punctum  x,
     Punctum  y,
     Punctum  v,
     Piscina* piscina)
{
    redde fractio_subtrahe(
        fractio_multiplica(_scalare(x, y, piscina),
            _scalare(v, v, piscina), piscina),
        fractio_multiplica(_scalare(x, v, piscina),
            _scalare(y, v, piscina), piscina), piscina);
}

/* r (in proiectione collineare cum pq) inter extrema proiecta? Si pq in
 * punctum proiicitur, r illud punctum esse debet. */
interior b32
_inter_proiectum (
     Punctum  p,
     Punctum  q,
     Punctum  r,
     Punctum  v,
     Piscina* piscina)
{
    Punctum directio_segmenti  = _minus(q, p, piscina);
    Punctum pr                 = _minus(r, p, piscina);

    si (fractio_signum(_scalare_proiectum(directio_segmenti,
        directio_segmenti, v, piscina))
        == ZEPHYRUM)
    {
        redde fractio_signum(_scalare_proiectum(pr, pr, v, piscina))
            == ZEPHYRUM;
    }
    redde fractio_signum(_scalare_proiectum(pr, directio_segmenti, v,
        piscina))
        >= ZEPHYRUM
        && fractio_signum(_scalare_proiectum(_minus(r, q, piscina),
            _minus(p, q, piscina), v, piscina)) >= ZEPHYRUM;
}

interior SitusContactus
_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum,
     Fractio* parametrum_ab,
     Fractio* parametrum_cd)
{
    Punctum u = _minus(b, a, piscina);
    Punctum w = _minus(d, c, piscina);
    Punctum ca;
        s32 o1;
        s32 o2;
        s32 o3;
        s32 o4;

    si (_nullus(v))
    {
        redde SITUS_TANGUNT;
    }
    ca = _minus(c, a, piscina);
    o1 = _signum_triplex(u, ca, v, piscina);
    o2 = _signum_triplex(u, _minus(d, a, piscina), v, piscina);
    o3 = _signum_triplex(w, _minus(a, c, piscina), v, piscina);
    o4 = _signum_triplex(w, _minus(b, c, piscina), v, piscina);

    si (o1 * o2 < ZEPHYRUM && o3 * o4 < ZEPHYRUM)
    {
        /* s u - t w = (c - a) + lambda v; Cramer cum v: det[u, w, v]
         * non nullum quia proiectiones non parallelae */
        Fractio determinans  = _triplex(u, w, v, piscina);
        Fractio s            = fractio_ex_s64(ZEPHYRUM);
        Fractio t            = fractio_ex_s64(ZEPHYRUM);
        Punctum pa;
        Punctum pc;
            s32 ordo;

        (vacuum)fractio_divide(_triplex(ca, w, v, piscina), determinans,
            piscina, &s);
        (vacuum)fractio_divide(_triplex(ca, u, v, piscina), determinans,
            piscina, &t);
        pa.x = fractio_adde(a.x, fractio_multiplica(s, u.x, piscina),
            piscina);
        pa.y = fractio_adde(a.y, fractio_multiplica(s, u.y, piscina),
            piscina);
        pa.z = fractio_adde(a.z, fractio_multiplica(s, u.z, piscina),
            piscina);
        pc.x = fractio_adde(c.x, fractio_multiplica(t, w.x, piscina),
            piscina);
        pc.y = fractio_adde(c.y, fractio_multiplica(t, w.y, piscina),
            piscina);
        pc.z = fractio_adde(c.z, fractio_multiplica(t, w.z, piscina),
            piscina);
        ordo = fractio_compara(_scalare(v, pa, piscina),
            _scalare(v, pc, piscina), piscina);
        si (ordo == ZEPHYRUM)
        {
            redde SITUS_TANGUNT;   /* segmenta in spatio se secant */
        }
        si (superius)
        {
            *superius = (ordo > ZEPHYRUM) ? ZEPHYRUM : I;
        }
        si (signum)
        {
            /* det[o, u, v]: o = u si [a,b] supra, aliter o = w */
            s32 signum_determinantis = fractio_signum(determinans);

            *signum = (ordo > ZEPHYRUM) ? signum_determinantis
                : -signum_determinantis;
        }
        si (parametrum_ab)
        {
            *parametrum_ab = s;
        }
        si (parametrum_cd)
        {
            *parametrum_cd = t;
        }
        redde SITUS_SECANT;
    }
    si (   (o1 == ZEPHYRUM && _inter_proiectum(a, b, c, v, piscina))
        || (o2 == ZEPHYRUM && _inter_proiectum(a, b, d, v, piscina))
        || (o3 == ZEPHYRUM && _inter_proiectum(c, d, a, v, piscina))
        || (o4 == ZEPHYRUM && _inter_proiectum(c, d, b, v, piscina)))
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}

/* proiectiones [a,b] et [b,c] secundum v, vertice b communi: aliud
 * punctum commune sse aliqua in punctum proiicitur aut proiectiones
 * parallelae (det[u, w, v] nullum) eadem directione ab b */
interior SitusContactus
_transitus_vicinus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  v,
     Piscina* piscina)
{
    Punctum u = _minus(a, b, piscina);
    Punctum w = _minus(c, b, piscina);

    si (   _nullus(v)
        || fractio_signum(_scalare_proiectum(u, u, v, piscina))
            == ZEPHYRUM
        || fractio_signum(_scalare_proiectum(w, w, v, piscina))
            == ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    si (   _signum_triplex(u, w, v, piscina) == ZEPHYRUM
        && fractio_signum(_scalare_proiectum(u, w, v, piscina))
            > ZEPHYRUM)
    {
        redde SITUS_TANGUNT;
    }
    redde SITUS_DISIUNCTA;
}


/* ==================================================
 * Publica
 * ================================================== */

PunctumPlani
situs_punctum_plani (
    s64 x,
    s64 y)
{
    PunctumPlani p;

    p.x = fractio_ex_s64(x);
    p.y = fractio_ex_s64(y);
    redde p;
}

Punctum
situs_punctum (
    s64 x,
    s64 y,
    s64 z)
{
    Punctum p;

    p.x = fractio_ex_s64(x);
    p.y = fractio_ex_s64(y);
    p.z = fractio_ex_s64(z);
    redde p;
}

b32
situs_puncta_aequalia (
    Punctum a,
    Punctum b)
{
    redde fractio_aequalis(a.x, b.x) && fractio_aequalis(a.y, b.y)
        && fractio_aequalis(a.z, b.z);
}

s32
situs_orientatio_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
         Piscina* piscina)
{
    PiscinaNotatio nota          = piscina_notare(piscina);
               s32 r  = _orientatio_plana(a, b, c,
                   piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

s32
situs_orientatio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
               s32 r    = _signum_triplex(_minus(b, a,
                   piscina),
                   _minus(c, a, piscina), _minus(d, a, piscina),
                   piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

Fractio
situs_volumen_sexies (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    redde _triplex(_minus(b, a, piscina), _minus(c, a, piscina),
        _minus(d, a, piscina), piscina);
}

SitusContactus
situs_segmenta_plana (
    PunctumPlani  a,
    PunctumPlani  b,
    PunctumPlani  c,
    PunctumPlani  d,
         Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta_plana(a, b, c, d, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_segmenta (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta(a, b, c, d, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_triangulum_segmentum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  p,
     Punctum  q,
     Piscina* piscina)
{
    PiscinaNotatio nota = piscina_notare(piscina);
    SitusContactus r    = _triangulum_segmentum(t0, t1, t2, p, q,
        piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_transitus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum)
{
    PiscinaNotatio nota = piscina_notare(piscina);
    SitusContactus r    = _transitus(a, b, c, d, v, piscina, superius,
        signum, NIHIL, NIHIL);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_transitus_parametri (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  d,
     Punctum  v,
     Piscina* piscina,
         s32* superius,
         s32* signum,
     Fractio* parametrum_ab,
     Fractio* parametrum_cd)
{
    /* refectio nisi SECANT: parametri (soli exitus in piscina) tunc
     * soli scribuntur. Sine ea laqueus_diagramma piscinam O(n^2)
     * crescebat - coordinatis rationalibus CXX vertices CXXI MB
     * (recensio laqueus-I, A) */
    PiscinaNotatio nota = piscina_notare(piscina);
    SitusContactus r    = _transitus(a, b, c, d, v, piscina, superius,
        signum, parametrum_ab, parametrum_cd);

    si (r != SITUS_SECANT)
    {
        piscina_reficere(piscina, nota);
    }
    redde r;
}

SitusContactus
situs_segmenta_vicina (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _segmenta_vicina(a, b, c, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_triangulum_vicinum (
     Punctum  t0,
     Punctum  t1,
     Punctum  t2,
     Punctum  x,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _triangulum_vicinum(t0, t1, t2, x, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

SitusContactus
situs_transitus_vicinus (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  v,
     Piscina* piscina)
{
    PiscinaNotatio nota  = piscina_notare(piscina);
    SitusContactus r     = _transitus_vicinus(a, b, c, v, piscina);

    piscina_reficere(piscina, nota);
    redde r;
}

b32
situs_reflexio (
     Punctum  a,
     Punctum  b,
     Punctum  c,
     Punctum  p,
     Piscina* piscina,
     Punctum* exitus)
{
    Punctum u;
    Punctum w;
    Punctum n;
    Fractio nn;
    Fractio dd;
    Fractio t;

    u.x = fractio_subtrahe(b.x, a.x, piscina);
    u.y = fractio_subtrahe(b.y, a.y, piscina);
    u.z = fractio_subtrahe(b.z, a.z, piscina);
    w.x = fractio_subtrahe(c.x, a.x, piscina);
    w.y = fractio_subtrahe(c.y, a.y, piscina);
    w.z = fractio_subtrahe(c.z, a.z, piscina);
    /* n = u x w */
    n.x = fractio_subtrahe(fractio_multiplica(u.y, w.z, piscina),
        fractio_multiplica(u.z, w.y, piscina), piscina);
    n.y = fractio_subtrahe(fractio_multiplica(u.z, w.x, piscina),
        fractio_multiplica(u.x, w.z, piscina), piscina);
    n.z = fractio_subtrahe(fractio_multiplica(u.x, w.y, piscina),
        fractio_multiplica(u.y, w.x, piscina), piscina);
    nn = fractio_adde(fractio_adde(fractio_multiplica(n.x, n.x,
        piscina),
        fractio_multiplica(n.y, n.y, piscina), piscina),
        fractio_multiplica(n.z, n.z, piscina), piscina);
    si (fractio_signum(nn) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    dd = fractio_adde(fractio_adde(
        fractio_multiplica(n.x, fractio_subtrahe(p.x, a.x, piscina),
        piscina),
        fractio_multiplica(n.y, fractio_subtrahe(p.y, a.y, piscina),
        piscina), piscina),
        fractio_multiplica(n.z, fractio_subtrahe(p.z, a.z, piscina),
        piscina), piscina);
    /* t = 2 (n.(p - a)) / (n.n) */
    (vacuum)fractio_divide(fractio_adde(dd, dd, piscina), nn, piscina,
        &t);
    exitus->x = fractio_subtrahe(p.x, fractio_multiplica(t, n.x,
        piscina),
        piscina);
    exitus->y = fractio_subtrahe(p.y, fractio_multiplica(t, n.y,
        piscina),
        piscina);
    exitus->z = fractio_subtrahe(p.z, fractio_multiplica(t, n.z,
        piscina),
        piscina);
    redde VERUM;
}
#undef _axis_non_nulla
#undef _coordinata
#undef _crux
#undef _in_triangulo
#undef _inter_plana
#undef _inter_proiectum
#undef _minus
#undef _nullus
#undef _omissa
#undef _orientatio_plana
#undef _scalare
#undef _scalare_proiectum
#undef _segmenta
#undef _segmenta_plana
#undef _segmenta_vicina
#undef _signum_triplex
#undef _transitus
#undef _transitus_vicinus
#undef _triangulum_segmentum
#undef _triangulum_vicinum
#undef _triplex
/* lib/laqueus.c: statica per plagulam renominata */
#define _aestimator_verticis _aestimator_verticis_laqueus
#define _alexander_ex_arcubus _alexander_ex_arcubus_laqueus
#define _collinearia _collinearia_laqueus
#define _componens _componens_laqueus
#define _est_spatium _est_spatium_laqueus
#define _gyrationes _gyrationes_laqueus
#define _jones_ex_uncino _jones_ex_uncino_laqueus
#define _laqueus_cum_vertice _laqueus_cum_vertice_laqueus
#define _limes_arborum _limes_arborum_laqueus
#define _modulo_quattuor _modulo_quattuor_laqueus
#define _ora_diagrammatis _ora_diagrammatis_laqueus
#define _pd_legere _pd_legere_laqueus
#define _pd_motus _pd_motus_laqueus
#define _pd_planus _pd_planus_laqueus
#define _pd_renumerare _pd_renumerare_laqueus
#define _positio_sequens _positio_sequens_laqueus
#define _prior _prior_laqueus
#define _productum_addere _productum_addere_laqueus
#define _punctum_legere _punctum_legere_laqueus
#define _radix _radix_laqueus
#define _sequens _sequens_laqueus
#define _sine_spatiis _sine_spatiis_laqueus
#define _sumptus_nullus _sumptus_nullus_laqueus
#define _terminos_transitus _terminos_transitus_laqueus
#define _uncinus_ex_oris _uncinus_ex_oris_laqueus
#define _uncinus_per_rete _uncinus_per_rete_laqueus
#define TerminusVerticis TerminusVerticis_laqueus
#line 1 "lib/laqueus.c"
/* laqueus.c - Laquei polygonales exacti: diagrammata et invariantes
 * Geometria tota per situs (exacta). Diagramma: omnia paria segmentorum
 * (contigua per situs_transitus_vicinus, cetera per
 * situs_transitus_parametri); transitus per segmentum ordine parametri,
 * percursus laquei = codex Gauss. Uncinus per summam statuum super
 * codicem PD: positivus ora[u_in, o_out, u_out, o_in], negativus
 * ora[u_in, o_in, u_out, o_out] (contra horologium, spectator ad +v); A
 * iungit (X0 X1)(X2 X3), B (X0 X3)(X1 X2). Alexander per calculum Fox
 * praesentationis Wirtinger: positivus x_b = x_o x_a x_o^-1 -> (1 - t,
 * t, -1); negativus x_b = x_o^-1 x_a x_o -> (1 - t^-1, t^-1, -1). Vide
 * lib/laqueus.worklog.md. */






#include <string.h>


/* ==================================================
 * Auxilia topologiae
 * ================================================== */

interior i32
_componens (
    Laqueus l,
        i32 i)
{
    i32 k = ZEPHYRUM;

    dum (k + I < l.componentes && l.initia[k + I] <= i)
    {
        k++;
    }
    redde k;
}

interior i32
_sequens (
    Laqueus l,
        i32 i)
{
    i32 k = _componens(l, i);

    redde (i + I == l.initia[k + I]) ? l.initia[k] : i + I;
}

interior i32
_prior (
    Laqueus l,
        i32 i)
{
    i32 k = _componens(l, i);

    redde (i == l.initia[k]) ? l.initia[k + I] - I : i - I;
}

/* tria puncta collinearia (proiectiones in tria plana coordinatarum
 * omnes collineares) */
interior b32
_collinearia (
    Punctum  a,
    Punctum  b,
    Punctum  c,
    Piscina* piscina)
{
    PunctumPlani pa;
    PunctumPlani pb;
    PunctumPlani pc;

    pa.x = a.x; pa.y = a.y; pb.x = b.x; pb.y = b.y; pc.x = c.x; pc.y =
                                                                    c.y;
    si (situs_orientatio_plana(pa, pb, pc, piscina) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    pa.x = a.y; pa.y = a.z; pb.x = b.y; pb.y = b.z; pc.x = c.y; pc.y =
                                                                    c.z;
    si (situs_orientatio_plana(pa, pb, pc, piscina) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    pa.x = a.x; pa.y = a.z; pb.x = b.x; pb.y = b.z; pc.x = c.x; pc.y =
                                                                    c.z;
    redde situs_orientatio_plana(pa, pb, pc, piscina) == ZEPHYRUM;
}


/* ==================================================
 * Textus
 * ================================================== */

interior b32
_est_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior chorda
_sine_spatiis (
    chorda textus)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = textus.mensura;

    dum (initium < finis && _est_spatium(textus.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _est_spatium(textus.datum[finis - I]))
    {
        finis--;
    }
    redde chorda_sectio(textus, initium, finis);
}

/* "(x, y, z)" sine spatiis extremis */
interior b32
_punctum_legere (
      chorda  textus,
     Piscina* piscina,
     Punctum* exitus)
{
        i32 commata[II];
        i32 numerus = ZEPHYRUM;
        i32 k;
    Punctum p;

    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '('
        || textus.datum[textus.mensura - I] != ')')
    {
        redde FALSUM;
    }
    per (k = I; k + I < textus.mensura; k++)
    {
        si (textus.datum[k] == ',')
        {
            si (numerus == II)
            {
                redde FALSUM;
            }
            commata[numerus++] = k;
        }
    }
    si (   numerus != II
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus, I,
            commata[ZEPHYRUM])), piscina, &p.x)
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus,
            commata[ZEPHYRUM] + I, commata[I])), piscina, &p.y)
        || !fractio_ex_chorda(_sine_spatiis(chorda_sectio(textus,
            commata[I] + I, textus.mensura - I)), piscina, &p.z))
    {
        redde FALSUM;
    }
    *exitus = p;
    redde VERUM;
}

b32
laqueus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* vertices;
        i32* initia;
        i32  capacitas    = ZEPHYRUM;
        i32  numerus      = ZEPHYRUM;
        i32  componentes  = ZEPHYRUM;
        i32  k;
        i32  initium_componentis = ZEPHYRUM;

    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    textus = _sine_spatiis(textus);
    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '['
        || textus.datum[textus.mensura - I] != ']')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        si (textus.datum[k] == '(')
        {
            capacitas++;
        }
    }
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)(
        capacitas + I) * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(capacitas
        + II) * magnitudo(i32));
    initia[ZEPHYRUM] = ZEPHYRUM;

    k = I;
    dum (VERUM)
    {
        i32 initium;

        dum (k < textus.mensura - I && _est_spatium(textus.datum[k]))
        {
            k++;
        }
        si (textus.datum[k] != '(')
        {
            redde FALSUM;
        }
        initium = k;
        dum (k < textus.mensura - I && textus.datum[k] != ')')
        {
            k++;
        }
        si (   textus.datum[k] != ')'
            || !_punctum_legere(chorda_sectio(textus, initium, k + I),
                piscina, &vertices[numerus]))
        {
            redde FALSUM;
        }
        numerus++;
        k++;
        dum (k < textus.mensura - I && _est_spatium(textus.datum[k]))
        {
            k++;
        }
        si (textus.datum[k] == ',')
        {
            k++;
            perge;
        }
        si (textus.datum[k] == ';' || k == textus.mensura - I)
        {
            si (numerus - initium_componentis < III)
            {
                redde FALSUM;
            }
            componentes++;
            initia[componentes] = numerus;
            initium_componentis = numerus;
            si (k == textus.mensura - I)
            {
                frange;
            }
            k++;
            perge;
        }
        redde FALSUM;
    }
    exitus->vertices     = vertices;
    exitus->numerus      = numerus;
    exitus->initia       = initia;
    exitus->componentes  = componentes;
    redde VERUM;
}

b32
laqueus_ex_punctis (
    constans Punctum* puncta,
        constans i32* initia,
                 i32  componentes,
             Piscina* piscina,
             Laqueus* exitus)
{
    Punctum* vertices;
        i32* initia_nova;
        i32  numerus;
        i32  k;

    si (componentes == ZEPHYRUM || initia[ZEPHYRUM] != ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < componentes; k++)
    {
        si (initia[k + I] < initia[k] + III)
        {
            redde FALSUM;
        }
    }
    numerus      = initia[componentes];
    vertices     = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        numerus * magnitudo(Punctum));
    initia_nova  = (i32*)piscina_allocare(piscina, (memoriae_index)(
        componentes + I) * magnitudo(i32));
    memcpy(vertices, puncta,
        (memoriae_index)numerus * magnitudo(Punctum));
    memcpy(initia_nova, initia, (memoriae_index)(componentes + I)
        * magnitudo(i32));
    exitus->vertices     = vertices;
    exitus->numerus      = numerus;
    exitus->initia       = initia_nova;
    exitus->componentes  = componentes;
    redde VERUM;
}

chorda
laqueus_ad_chordam (
     Laqueus  l,
     Piscina* piscina)
{
    ChordaAedificator* scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);
                   i32 k;
                   i32 i;

    (vacuum)chorda_aedificator_appendere_character(scriba, '[');
    per (k = ZEPHYRUM; k < l.componentes; k++)
    {
        si (k > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba, "; ");
        }
        per (i = l.initia[k]; i < l.initia[k + I]; i++)
        {
            si (i > l.initia[k])
            {
                (vacuum)chorda_aedificator_appendere_literis(scriba,
                    ", ");
            }
            (vacuum)chorda_aedificator_appendere_character(scriba, '(');
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].x, piscina));
            (vacuum)chorda_aedificator_appendere_literis(scriba, ", ");
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].y, piscina));
            (vacuum)chorda_aedificator_appendere_literis(scriba, ", ");
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                fractio_ad_chordam(l.vertices[i].z, piscina));
            (vacuum)chorda_aedificator_appendere_character(scriba, ')');
        }
    }
    (vacuum)chorda_aedificator_appendere_character(scriba, ']');
    redde chorda_aedificator_finire(scriba);
}

i32
laqueus_numerus (
    Laqueus l)
{
    redde l.numerus;
}

i32
laqueus_componentes (
    Laqueus l)
{
    redde l.componentes;
}

Punctum
laqueus_vertex (
    Laqueus l,
        i32 i)
{
    redde l.vertices[i];
}

b32
laqueus_simplex (
     Laqueus  l,
     Piscina* piscina)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per (j = i + I; j < l.numerus; j++)
        {
            Punctum a = l.vertices[i];
            Punctum b = l.vertices[_sequens(l, i)];
            Punctum c = l.vertices[j];
            Punctum d = l.vertices[_sequens(l, j)];

            si (_sequens(l, i) == j)
            {
                si (situs_segmenta_vicina(a, c, d, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (_sequens(l, j) == i)
            {
                si (situs_segmenta_vicina(c, a, b, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (situs_segmenta(a, b, c, d, piscina)
                         != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}


/* ==================================================
 * Diagramma
 * ================================================== */

b32
laqueus_diagramma (
        Laqueus  l,
        Punctum  v,
        Piscina* piscina,
      Diagramma* exitus)
{
    Transitus* transitus;
          i32  numerus    = ZEPHYRUM;
          i32  capacitas  = l.numerus + I;
          i32  i;
          i32  j;
          i32* per_segmentum;    /* numerus eventuum per segmentum */
          i32* initia_eventuum;
          i32* eventus;          /* codices 2k / 2k+1 per segmentum */
          i32* percursus;
          i32* initia_percursus;
          i32  positus;

    transitus = (Transitus*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(Transitus));
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per (j = i + I; j < l.numerus; j++)
        {
                   Punctum a = l.vertices[i];
                   Punctum b = l.vertices[_sequens(l, i)];
                   Punctum c = l.vertices[j];
                   Punctum d = l.vertices[_sequens(l, j)];
                       s32 superius;
                       s32 signum;
                   Fractio s;
                   Fractio t;
            SitusContactus r;

            si (_sequens(l, i) == j)
            {
                si (situs_transitus_vicinus(a, c, d, v, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
                perge;
            }
            si (_sequens(l, j) == i)
            {
                si (situs_transitus_vicinus(c, a, b, v, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
                perge;
            }
            r = situs_transitus_parametri(a, b, c, d, v, piscina,
                &superius, &signum, &s, &t);
            si (r == SITUS_DISIUNCTA)
            {
                perge;
            }
            si (r != SITUS_SECANT)
            {
                redde FALSUM;
            }
            si (numerus == capacitas)
            {
                memoriae_index mensura =
                    (memoriae_index)(capacitas * II)
                    * magnitudo(Transitus);
                Transitus* novi = (Transitus*)piscina_allocare(piscina,
                    mensura);

                memcpy(novi, transitus, (memoriae_index)numerus
                    * magnitudo(Transitus));
                transitus = novi;
                capacitas = capacitas * II;
            }
            si (superius == ZEPHYRUM)
            {
                transitus[numerus].supra             = i;
                transitus[numerus].parametrum_supra  = s;
                transitus[numerus].infra             = j;
                transitus[numerus].parametrum_infra  = t;
            }
            alioquin
            {
                transitus[numerus].supra             = j;
                transitus[numerus].parametrum_supra  = t;
                transitus[numerus].infra             = i;
                transitus[numerus].parametrum_infra  = s;
            }
            transitus[numerus].signum = signum;
            numerus++;
        }
    }

    /* eventus per segmentum, ordine parametri; aequales = punctum
     * triplex (non genericum) */
    per_segmentum = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.numerus + I) * magnitudo(i32));
    initia_eventuum = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.numerus + I) * magnitudo(i32));
    eventus = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * numerus + I) * magnitudo(i32));
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        per_segmentum[i] = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        per_segmentum[transitus[j].supra]++;
        per_segmentum[transitus[j].infra]++;
    }
    positus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        initia_eventuum[i]  = positus;
        positus             += per_segmentum[i];
        per_segmentum[i]    = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        i32 si_ = transitus[j].supra;
        i32 in_ = transitus[j].infra;

        eventus[initia_eventuum[si_] + per_segmentum[si_]++] = II * j;
        eventus[initia_eventuum[in_] + per_segmentum[in_]++] = II * j
            + I;
    }
    per (i = ZEPHYRUM; i < l.numerus; i++)
    {
        i32* e = eventus + initia_eventuum[i];
        i32  m = per_segmentum[i];
        i32  a;
        i32  b;

        /* insertio ordine parametri */
        per (a = I; a < m; a++)
        {
            i32 clavis = e[a];

            b = a;
            dum (b > ZEPHYRUM)
            {
                    i32 x = e[b - I];
                Fractio px = (x % II == ZEPHYRUM)
                    ? transitus[x / II].parametrum_supra
                    : transitus[x / II].parametrum_infra;
                Fractio pc = (clavis % II == ZEPHYRUM)
                    ? transitus[clavis / II].parametrum_supra
                    : transitus[clavis / II].parametrum_infra;
                s32 ordo = fractio_compara(px, pc, piscina);

                si (ordo == ZEPHYRUM)
                {
                    redde FALSUM;   /* punctum triplex */
                }
                si (ordo < ZEPHYRUM)
                {
                    frange;
                }
                e[b] = x;
                b--;
            }
            e[b] = clavis;
        }
    }

    /* percursus per componentes, segmenta ordine */
    percursus = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * numerus + I) * magnitudo(i32));
    initia_percursus = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    positus = ZEPHYRUM;
    per (j = ZEPHYRUM; j < l.componentes; j++)
    {
        initia_percursus[j] = positus;
        per (i = l.initia[j]; i < l.initia[j + I]; i++)
        {
            i32 a;

            per (a = ZEPHYRUM; a < per_segmentum[i]; a++)
            {
                percursus[positus++] = eventus[initia_eventuum[i] + a];
            }
        }
    }
    initia_percursus[l.componentes] = positus;

    exitus->laqueus           = l;
    exitus->directio          = v;
    exitus->transitus         = transitus;
    exitus->numerus           = numerus;
    exitus->percursus         = percursus;
    exitus->initia_percursus  = initia_percursus;
    redde VERUM;
}

b32
laqueus_diagramma_genericum (
        Laqueus  l,
        Piscina* piscina,
      Diagramma* exitus)
{
    s32 directiones[XII][III] = {
        { 0, 0, 1 }, { 1, 2, 3 }, { 2, 3, 5 }, { 3, 5, 7 },
        { 1, -2, 4 }, { 5, -3, 2 }, { 7, 11, 13 }, { -2, 3, 11 },
        { 13, -7, 5 }, { 1, 1, 17 }, { 19, -23, 29 }, { 31, 37, -41 }
    };
    i32 k;

    per (k = ZEPHYRUM; k < XII; k++)
    {
        PiscinaNotatio nota = piscina_notare(piscina);

        si (laqueus_diagramma(l, situs_punctum(directiones[k][ZEPHYRUM],
            directiones[k][I], directiones[k][II]), piscina, exitus))
        {
            redde VERUM;
        }
        piscina_reficere(piscina, nota);
    }
    /* Series fixa exhauriri potest (polygonum simplex XIII verticum cum
     * segmento parallelo cuique directioni: recensio laqueus-I, B).
     * Curva momentorum (1, k, k^2): quaeque condicio non generica v in
     * plano (vertex in segmento proiectus, vicini superpositi), in
     * recta (segmentum parallelum) aut in cono quadrico (punctum
     * triplex: rectae tres rectas obliquas secantes regulum faciunt)
     * ponit; curva planum bis, conum quater ad summum secat - ergo pro
     * laqueo simplici k finitus sufficit. Laqueus non simplex numquam
     * genericus est: ante iter refutatur. Limes = numerus k irritorum
     * possibilium + 1: n (parallela) + 2n (vicini) + 2n^2 (vertex in
     * segmento) + 4 C(n, 3) (puncta triplicia); ultra eum FALSUM
     * (error, non pendere - planta M32 sine limite X minuta currebat
     * donec interfecta). k^2 < 2^63 etiam. */
    si (!laqueus_simplex(l, piscina))
    {
        redde FALSUM;
    }
    {
        s64 n = (s64)l.numerus;
        s64 limes;
        s64 k;

        limes = (n > (s64)M * M)
            ? (s64)MMMXXXVII * M * M
            : I + III * n + II * n * n + II * n * (n - I) * (n - II)
                / III;
        si (limes > (s64)MMMXXXVII * M * M)
        {
            limes = (s64)MMMXXXVII * M * M;
        }
        per (k = I; k <= limes; k++)
        {
            PiscinaNotatio nota = piscina_notare(piscina);

            si (laqueus_diagramma(l, situs_punctum(I, k, k * k),
                piscina,
                exitus))
            {
                redde VERUM;
            }
            piscina_reficere(piscina, nota);
        }
    }
    redde FALSUM;
}

b32
laqueus_diagramma_minimum (
     Laqueus  l,
         i32  radius,
     Piscina* piscina,
   Diagramma* exitus)
{
    s64 r = (s64)radius;
    s64 a;
    s64 b;
    s64 c;
    s64 optima[III];
    i32 paucissimi  = ZEPHYRUM;
    b32 inventum    = FALSUM;

    si (radius == ZEPHYRUM || radius > LAQUEUS_RADIUS_MAXIMUS)
    {
        redde FALSUM;
    }
    per (a = -r; a <= r; a++)
    {
        per (b = -r; b <= r; b++)
        {
            per (c = -r; c <= r; c++)
            {
                PiscinaNotatio nota;
                     Diagramma d;

                /* una ex +-v: componens prima non nulla positiva */
                si (   a < ZEPHYRUM || (a == ZEPHYRUM && (b < ZEPHYRUM
                    || (b == ZEPHYRUM && c <= ZEPHYRUM))))
                {
                    perge;
                }
                nota = piscina_notare(piscina);
                si (   laqueus_diagramma(l, situs_punctum(a, b, c),
                    piscina,
                        &d)
                    && (!inventum || d.numerus < paucissimi))
                {
                    inventum          = VERUM;
                    paucissimi        = d.numerus;
                    optima[ZEPHYRUM]  = a;
                    optima[I]         = b;
                    optima[II]        = c;
                }
                piscina_reficere(piscina, nota);
            }
        }
    }
    redde inventum
        && laqueus_diagramma(l, situs_punctum(optima[ZEPHYRUM],
        optima[I], optima[II]), piscina, exitus);
}

i32
diagramma_numerus (
    Diagramma d)
{
    redde d.numerus;
}

Transitus
diagramma_transitus (
    Diagramma d,
          i32 k)
{
    redde d.transitus[k];
}


/* ==================================================
 * Invariantes
 * ================================================== */

s32
diagramma_scriptura (
    Diagramma d)
{
    s32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < d.numerus; k++)
    {
        summa += d.transitus[k].signum;
    }
    redde summa;
}

s32
diagramma_numerus_ligationis (
    Diagramma d,
          i32 a,
          i32 b)
{
    s32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < d.numerus; k++)
    {
        i32 cs = _componens(d.laqueus, d.transitus[k].supra);
        i32 ci = _componens(d.laqueus, d.transitus[k].infra);

        si ((cs == a && ci == b) || (cs == b && ci == a))
        {
            summa += d.transitus[k].signum;
        }
    }
    redde summa / II;
}

/* positio sequens in percursu (eiusdem componentis, circulariter) */
interior i32
_positio_sequens (
    Diagramma d,
          i32 p)
{
    i32 k = ZEPHYRUM;

    dum (d.initia_percursus[k + I] <= p)
    {
        k++;
    }
    redde (p + I == d.initia_percursus[k + I]) ? d.initia_percursus[k]
        : p + I;
}

interior i32
_radix (
    i32* pater,
    i32  x)
{
    dum (pater[x] != x)
    {
        pater[x]  = pater[pater[x]];
        x         = pater[x];
    }
    redde x;
}

/* ora (codex PD in positionibus 0..m-1, 4 per transitum, sicut
 * diagramma_pd sed 0-basatum) diagrammatis */
interior i32*
_ora_diagrammatis (
    Diagramma  d,
      Piscina* piscina)
{
    i32  c = d.numerus;
    i32  m = II * c;
    i32* ora      = (i32*)piscina_allocare(piscina, (memoriae_index)(IV
        * c + I) * magnitudo(i32));
    i32* positio  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    i32 k;

    per (k = ZEPHYRUM; k < m; k++)
    {
        positio[d.percursus[k]] = k;
    }
    /* ora: ora p = arcus intrans passum ad positionem p */
    per (k = ZEPHYRUM; k < c; k++)
    {
        i32 u_in   = positio[II * k + I];
        i32 u_out  = _positio_sequens(d, u_in);
        i32 o_in   = positio[II * k];
        i32 o_out  = _positio_sequens(d, o_in);

        ora[IV * k]       = u_in;
        ora[IV * k + II]  = u_out;
        si (d.transitus[k].signum > ZEPHYRUM)
        {
            ora[IV * k + I]    = o_out;
            ora[IV * k + III]  = o_in;
        }
        alioquin
        {
            ora[IV * k + I]    = o_in;
            ora[IV * k + III]  = o_out;
        }
    }
    redde ora;
}

/* uncinus Kauffman ex ora (0..m-1) per summam statuum; ansae_liberae =
 * componentes sine transitu */
interior b32
_uncinus_ex_oris (
    constans i32* ora,
             i32  c,
             i32  m,
             i32  ansae_liberae,
             i32  componentes,
         Piscina* piscina,
      Polynomium* exitus)
{
           i32* pater;
           s64* numeri;     /* [exponens + c][ansae] */
           i32  latitudo;
           i32  k;
           i64  status;
    Polynomium  summa = polynomium_nullum();
    Polynomium  dd;
    Polynomium  potentia_d;

    si (c > LAQUEUS_TRANSITUS_MAXIMI)
    {
        redde FALSUM;
    }
    latitudo = c + componentes + I;
    pater    = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    numeri   = (s64*)piscina_allocare(piscina, (memoriae_index)(II * c
        + I) * (memoriae_index)latitudo * magnitudo(s64));
    per (k = ZEPHYRUM; k < (II * c + I) * latitudo; k++)
    {
        numeri[k] = ZEPHYRUM;
    }
    per (status = ZEPHYRUM; status < ((i64)I << c); status++)
    {
        i32 ansae     = ansae_liberae;
        s32 exponens  = ZEPHYRUM;
        i32 e;

        per (e = ZEPHYRUM; e < m; e++)
        {
            pater[e] = e;
        }
        per (k = ZEPHYRUM; k < c; k++)
        {
            constans i32* x = ora + IV * k;

            si ((status >> k) & (i64)I)
            {
                /* B: (X0 X3)(X1 X2) */
                pater[_radix(pater, x[ZEPHYRUM])] = _radix(pater,
                    x[III]);
                pater[_radix(pater, x[I])] = _radix(pater, x[II]);
                exponens--;
            }
            alioquin
            {
                /* A: (X0 X1)(X2 X3) */
                pater[_radix(pater, x[ZEPHYRUM])] = _radix(pater, x[I]);
                pater[_radix(pater, x[II])] = _radix(pater, x[III]);
                exponens++;
            }
        }
        per (e = ZEPHYRUM; e < m; e++)
        {
            si (_radix(pater, e) == e)
            {
                ansae++;
            }
        }
        numeri[(i32)(exponens + (s32)c) * latitudo + ansae]++;
    }

    /* D uncinatum = summa A^e d^(ansae - 1), d = -A^2 - A^-2 */
    dd = polynomium_nullum();
    {
        Polynomium a2   = polynomium_nullum();
        Polynomium a_2  = polynomium_nullum();

        (vacuum)polynomium_monomium(magnus_ex_s64(-I), II, piscina,
            &a2);
        (vacuum)polynomium_monomium(magnus_ex_s64(-I), -II, piscina,
            &a_2);
        dd = polynomium_adde(a2, a_2, piscina);
    }
    per (k = ZEPHYRUM; k < (II * c + I) * latitudo; k++)
    {
               s32 exponens  = (s32)(k / latitudo) - (s32)c;
               i32 ansae     = k % latitudo;
        Polynomium terminus  = polynomium_nullum();

        si (numeri[k] == ZEPHYRUM)
        {
            perge;
        }
        si (   ansae == ZEPHYRUM
            || !polynomium_potentia(dd, ansae - I, piscina, &potentia_d)
            || !polynomium_monomium(magnus_ex_s64(numeri[k]), exponens,
                piscina, &terminus)
            || !polynomium_multiplica(terminus, potentia_d, piscina,
                &terminus))
        {
            redde FALSUM;
        }
        summa = polynomium_adde(summa, terminus, piscina);
    }
    *exitus = summa;
    redde VERUM;
}

b32
diagramma_uncinus (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    i32 ansae_liberae = ZEPHYRUM;
    i32 k;

    si (d.numerus > LAQUEUS_TRANSITUS_MAXIMI)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < d.laqueus.componentes; k++)
    {
        si (d.initia_percursus[k + I] == d.initia_percursus[k])
        {
            ansae_liberae++;
        }
    }
    redde _uncinus_ex_oris(_ora_diagrammatis(d, piscina), d.numerus,
        II * d.numerus, ansae_liberae, d.laqueus.componentes, piscina,
        exitus);
}

/* Jones V(t) = f(A = t^(-1/4)), f = (-A^3)^-w uncinus = (-1)^w A^-3w
 * uncinus */
interior b32
_jones_ex_uncino (
    Polynomium  uncinus,
           s32  w,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium factor;
    Polynomium f;

    si (   !polynomium_monomium(magnus_ex_s64((w % II == ZEPHYRUM) ? I
            : -I), -III * w, piscina, &factor)
        || !polynomium_multiplica(factor, uncinus, piscina, &f))
    {
        redde FALSUM;
    }
    redde polynomium_contrahe(f, -IV, piscina, exitus);
}

b32
diagramma_jones (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    Polynomium uncinus;

    /* catena componentium numeri paris: V in t^(1/2) Z[t, t^-1] -
     * exponentes dimidii certi, uncinus (2^c status) frustra */
    si (d.laqueus.componentes % II == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde diagramma_uncinus(d, piscina, &uncinus)
        && _jones_ex_uncino(uncinus, diagramma_scriptura(d), piscina,
            exitus);
}

/* Alexander ex arcubus (calculus Fox): linea k transitus k, columnae
 * arcus supra, arcus intrans infra, arcus exiens infra; positivus (1 -
 * t, t, -1), negativus (1 - t^-1, t^-1, -1); minor linea et columna
 * ultimis deletis, forma normalis */
interior b32
_alexander_ex_arcubus (
               i32  c,
      constans i32* supra_arcus,
      constans i32* in_arcus,
      constans i32* ex_arcus,
      constans s32* signa,
           Piscina* piscina,
        Polynomium* exitus)
{
    constans Anulus* p = &ANULUS_POLYNOMIORUM;
                i32  q;
                i32  k;
             Matrix  matrix_plena;
             Matrix  minor;
         Polynomium  delta;
         Polynomium  unum = polynomium_constans(magnus_ex_s64(I),
             piscina);
         Polynomium t    = polynomium_nullum();
         Polynomium t_1  = polynomium_nullum();
         Polynomium minus_unum = polynomium_constans(magnus_ex_s64(-I),
             piscina);

    si (c == ZEPHYRUM)
    {
        *exitus = unum;
        redde VERUM;
    }
    (vacuum)polynomium_monomium(magnus_ex_s64(I), I, piscina, &t);
    (vacuum)polynomium_monomium(magnus_ex_s64(I), -I, piscina, &t_1);
    (vacuum)matrix_nulla(p, c, c, piscina, &matrix_plena);
    per (k = ZEPHYRUM; k < c; k++)
    {
        Polynomium valores[III];
               i32 columnae[III];
               i32 r;

        si (signa[k] > ZEPHYRUM)
        {
            valores[ZEPHYRUM]  = polynomium_subtrahe(unum, t, piscina);
            valores[I]         = t;
        }
        alioquin
        {
            valores[ZEPHYRUM] = polynomium_subtrahe(unum, t_1, piscina);
            valores[I] = t_1;
        }
        valores[II]         = minus_unum;
        columnae[ZEPHYRUM]  = supra_arcus[k];
        columnae[I]         = in_arcus[k];
        columnae[II]        = ex_arcus[k];
        per (r = ZEPHYRUM; r < III; r++)
        {
            Polynomium summa = polynomium_adde(*(constans Polynomium*)
                matrix_elementum(matrix_plena, k, columnae[r]),
                valores[r], piscina);

            matrix_pone(&matrix_plena, k, columnae[r], &summa);
        }
    }
    /* minor: linea et columna ultimae deletae */
    (vacuum)matrix_nulla(p, c - I, c - I, piscina, &minor);
    per (k = ZEPHYRUM; k + I < c; k++)
    {
        per (q = ZEPHYRUM; q + I < c; q++)
        {
            matrix_pone(&minor, k, q, matrix_elementum(matrix_plena, k,
                q));
        }
    }
    si (   !matrix_determinans(minor, piscina, &delta)
        || polynomium_est_nullum(delta))
    {
        redde FALSUM;
    }
    redde polynomium_normale(delta, piscina, exitus);
}

b32
diagramma_alexander (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus)
{
    i32  c = d.numerus;
    i32  m = II * c;
    i32* supra_arcus;
    i32* in_arcus;
    i32* ex_arcus;
    s32* signa;
    i32  initium  = ZEPHYRUM;
    i32  arcus    = ZEPHYRUM;
    i32  q;
    i32  k;

    si (d.laqueus.componentes != I)
    {
        redde FALSUM;
    }
    supra_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I)
        * magnitudo(i32));
    in_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(i32));
    ex_arcus = (i32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(i32));
    signa = (s32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(s32));
    /* incipe post passum infra primum: arcus 0 ibi incipit */
    per (k = ZEPHYRUM; c > ZEPHYRUM && d.percursus[k] % II == ZEPHYRUM;
         k++)
    {
        initium++;
    }
    per (q = I; c > ZEPHYRUM && q <= m; q++)
    {
        i32 codex  = d.percursus[(initium + q) % m];
        i32 k_     = codex / II;

        si (codex % II == ZEPHYRUM)
        {
            supra_arcus[k_] = arcus;
        }
        alioquin
        {
            in_arcus[k_]  = arcus;
            arcus         = (arcus + I) % c;
            ex_arcus[k_]  = arcus;
        }
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        signa[k] = d.transitus[k].signum;
    }
    redde _alexander_ex_arcubus(c, supra_arcus, in_arcus, ex_arcus,
        signa,
        piscina, exitus);
}


/* ==================================================
 * Codex PD
 * ================================================== */

b32
diagramma_pd (
     Diagramma   d,
       Piscina*  piscina,
           i32** pd)
{
    i32* ora = _ora_diagrammatis(d, piscina);
    i32  k;

    si (d.laqueus.componentes != I)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV * d.numerus; k++)
    {
        ora[k] = ora[k] + I;
    }
    *pd = ora;
    redde VERUM;
}

/* codex planus? Systema rotationis (sedes 0..3 contra horologium):
 * facies = orbitae sequentis(d) = sedes post alterum finem ossis d;
 * planum iff V - E + F = 2, i.e. F = c + 2. Codex validus sed non
 * planus (nodus virtualis, e.g. [[1,3,2,4],[2,5,3,6],[4,1,5,6]]: facies
 * una) refutatur - invariantes et motus R2 sine facie planitiem
 * praesumunt. ora 0-basata, bis quodque. */
interior b32
_pd_planus (
    constans i32* ora,
             i32  c,
         Piscina* piscina)
{
    i32  n = IV * c;
    i32* alter  = (i32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(i32));
    i32* prima  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * c
        + I) * magnitudo(i32));
    b32* visa   = (b32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(b32));
    i32 facies = ZEPHYRUM;
    i32 d;

    per (d = ZEPHYRUM; d < II * c; d++)
    {
        prima[d] = n;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        visa[d] = FALSUM;
        si (prima[ora[d]] == n)
        {
            prima[ora[d]] = d;
        }
        alioquin
        {
            alter[d]              = prima[ora[d]];
            alter[prima[ora[d]]]  = d;
        }
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 e = d;

        si (visa[d])
        {
            perge;
        }
        facies++;
        dum (!visa[e])
        {
            visa[e]  = VERUM;
            e        = IV * (alter[e] / IV) + (alter[e] % IV + I) % IV;
        }
    }
    redde facies == c + II;
}

/* codex PD nodi (KnotTheory): ora 1..2c bis quaeque; infra X0 -> X2
 * (X2 = sequens X0), supra aut X3 -> X1 (positivus) aut X1 -> X3
 * (negativus). ora 0-basata et signa redduntur; FALSUM si invalidus aut
 * c == 1 (signum ex ordine orarum non determinatur) */
interior b32
_pd_legere (
    constans i32*  pd,
             i32   c,
         Piscina*  piscina,
             i32** ora,
             s32** signa)
{
    i32  m = II * c;
    i32* visa;
    i32  k;

    si (c == I)
    {
        redde FALSUM;
    }
    *ora    = (i32*)piscina_allocare(piscina, (memoriae_index)(IV * c
        + I)
        * magnitudo(i32));
    *signa  = (s32*)piscina_allocare(piscina, (memoriae_index)(c + I)
        * magnitudo(s32));
    visa    = (i32*)piscina_allocare(piscina, (memoriae_index)(m + I)
        * magnitudo(i32));
    per (k = ZEPHYRUM; k < m; k++)
    {
        visa[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        si (pd[k] < I || pd[k] > m)
        {
            redde FALSUM;
        }
        (*ora)[k] = pd[k] - I;
        visa[(*ora)[k]]++;
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (visa[k] != II)
        {
            redde FALSUM;
        }
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = *ora + IV * k;

        si (x[II] != (x[ZEPHYRUM] + I) % m)
        {
            redde FALSUM;
        }
        si (x[I] == (x[III] + I) % m)
        {
            (*signa)[k] = I;
        }
        alioquin si (x[III] == (x[I] + I) % m)
        {
            (*signa)[k] = -I;
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    /* quodque os semel intrans, semel exiens (infra X0; supra X3
     * positivus, X1 negativus): aliter ora "bis" sed orientatio falsa -
     * [1, 1, 2, 2, 3, 3, 4, 4] = duae ansae disiunctae. Ita numeratio
     * l -> l + 1 per omnia ora currit: componens una. */
    per (k = ZEPHYRUM; k < m; k++)
    {
        visa[k] = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        visa[(*ora)[IV * k]]++;
        visa[(*ora)[IV * k + ((*signa)[k] > ZEPHYRUM ? III : I)]]++;
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (visa[k] != I)
        {
            redde FALSUM;
        }
    }
    redde _pd_planus(*ora, c, piscina);
}

b32
laqueus_uncinus_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
    i32* ora;
    s32* signa;

    si (transitus == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    redde _pd_legere(pd, transitus, piscina, &ora, &signa)
        && _uncinus_ex_oris(ora, transitus, II * transitus, ZEPHYRUM, I,
            piscina, exitus);
}

b32
laqueus_jones_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
           i32* ora;
           s32* signa;
           s32  w = ZEPHYRUM;
           i32  k;
    Polynomium  uncinus;

    si (transitus == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (!_pd_legere(pd, transitus, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < transitus; k++)
    {
        w = w + signa[k];
    }
    redde _uncinus_ex_oris(ora, transitus, II * transitus, ZEPHYRUM, I,
            piscina, &uncinus)
        && _jones_ex_uncino(uncinus, w, piscina, exitus);
}

b32
laqueus_alexander_ex_pd (
    constans i32* pd,
             i32  transitus,
         Piscina* piscina,
      Polynomium* exitus)
{
    i32  c = transitus;
    i32  m = II * c;
    i32* ora;
    s32* signa;
    i32* pater;
    i32* numerus_arcus;
    i32* supra_arcus;
    i32* in_arcus;
    i32* ex_arcus;
    i32  arcus = ZEPHYRUM;
    i32  k;

    si (c == ZEPHYRUM)
    {
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    si (!_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    /* arcus: ora supra (X1, X3) eiusdem arcus; infra arcus finitur */
    pater          = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    numerus_arcus  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I) * magnitudo(i32));
    supra_arcus    = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    in_arcus       = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    ex_arcus       = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I) * magnitudo(i32));
    per (k = ZEPHYRUM; k < m; k++)
    {
        pater[k]          = k;
        numerus_arcus[k]  = m;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        pater[_radix(pater, ora[IV * k + I])] = _radix(pater,
            ora[IV * k + III]);
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        i32 r = _radix(pater, k);

        si (numerus_arcus[r] == m)
        {
            numerus_arcus[r] = arcus;
            arcus++;
        }
    }
    si (arcus != c)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        supra_arcus[k]  = numerus_arcus[_radix(pater, ora[IV * k + I])];
        in_arcus[k]     = numerus_arcus[_radix(pater, ora[IV * k])];
        ex_arcus[k] = numerus_arcus[_radix(pater, ora[IV * k
            + II])];
    }
    redde _alexander_ex_arcubus(c, supra_arcus, in_arcus, ex_arcus,
        signa,
        piscina, exitus);
}

/* ora transituum vivorum (vivus) cum oris per pater coniunctis secundum
 * nodum renumerantur 0..2c'-1 (ora intrans X0 -> exiens X2 = X0 + 1);
 * signa servantur. FALSUM si os non semel intrans et semel exiens aut
 * percursus ora omnia non tangit. c' == 0: ora nova NIHIL. */
interior b32
_pd_renumerare (
    constans i32*  ora,
    constans s32*  signa,
    constans b32*  vivus,
             i32   c,
             i32*  pater,
         Piscina*  piscina,
             i32** ora_nova,
             s32** signa_nova,
             i32*  c_nova)
{
    i32  m             = II * c;
    i32  locus_nullus  = IV * c;
    i32* intrans  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* numerus  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* exiens   = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* novus    = (i32*)piscina_allocare(piscina, (memoriae_index)(c
        + I)
        * magnitudo(i32));
    i32 vivi    = ZEPHYRUM;
    i32 primus  = c;
    i32 q       = ZEPHYRUM;
    i32 os;
    i32 os_primum;
    i32 k;
    i32 s;

    per (k = ZEPHYRUM; k < m; k++)
    {
        intrans[k]  = locus_nullus;
        exiens[k]   = locus_nullus;
        numerus[k]  = m;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        si (vivus[k])
        {
            si (primus == c)
            {
                primus = k;
            }
            novus[k] = vivi;
            vivi++;
        }
    }
    *c_nova = vivi;
    si (vivi == ZEPHYRUM)
    {
        *ora_nova    = NIHIL;
        *signa_nova  = NIHIL;
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (s = ZEPHYRUM; vivus[k] && s < IV; s++)
        {
            i32 radix = _radix(pater, ora[IV * k + s]);

            si (s == ZEPHYRUM || s == (signa[k] > ZEPHYRUM ? III : I))
            {
                si (intrans[radix] != locus_nullus)
                {
                    redde FALSUM;
                }
                intrans[radix] = IV * k + s;
            }
            alioquin
            {
                si (exiens[radix] != locus_nullus)
                {
                    redde FALSUM;
                }
                exiens[radix] = IV * k + s;
            }
        }
    }
    /* percursus: os intrans ad sedem s, exiens eiusdem fili */
    os_primum  = _radix(pater, ora[IV * primus]);
    os         = os_primum;
    fac
    {
        i32 locus = intrans[os];

        si (locus == locus_nullus || numerus[os] != m || q >= II * vivi)
        {
            redde FALSUM;
        }
        numerus[os] = q;
        q++;
        k = locus / IV;
        s = locus % IV;
        os  = _radix(pater, ora[IV * k + (s == ZEPHYRUM ? II : (signa[k]
            > ZEPHYRUM ? I : III))]);
    }
    dum (os != os_primum);
    si (q != II * vivi)
    {
        redde FALSUM;
    }
    *ora_nova = (i32*)piscina_allocare(piscina, (memoriae_index)(IV
        * vivi + I) * magnitudo(i32));
    *signa_nova = (s32*)piscina_allocare(piscina, (memoriae_index)(vivi
        + I) * magnitudo(s32));
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (s = ZEPHYRUM; vivus[k] && s < IV; s++)
        {
            (*ora_nova)[IV * novus[k] + s] = numerus[_radix(pater,
                ora[IV * k
                + s])];
        }
        si (vivus[k])
        {
            (*signa_nova)[novus[k]] = signa[k];
        }
    }
    redde VERUM;
}

/* motus unus Reidemeister in oris 0-basatis normalibus: R1 primum,
 * deinde R2. R1: os ad sedes vicinas eiusdem transitus (ansa; in codice
 * valido semper vicinas - monogonum). R2: transitus diversi j, k duo
 * ora e, f communicantes, in j sedibus vicinis (fila diversa), eodem
 * filo supra utrumque (paritas sedis ossis e in j et in k eadem). In
 * nodo (componente una) e, f aut faciem bilateram claudunt (R2
 * classicum) aut tangle 1-1 ad j et alterum ad k pendet ex utraque
 * parte - tunc nodus T1 # T2 et ante et post remotionem, codex planus
 * manet. Ergo facies non requiritur (vide lib/laqueus.worklog.md,
 * 2026-10-07). Reddit 1 applicatus, 0 nullus, -1 error. */
interior s32
_pd_motus (
         i32** ora,
         s32** signa,
         i32*  c,
     Piscina*  piscina)
{
    i32  n = *c;
    i32  m = II * n;
    i32* o = *ora;
    i32* alter  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(IV * n
        + I) * magnitudo(i32));
    i32* prima  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    i32* pater  = (i32*)piscina_allocare(piscina, (memoriae_index)(m
        + I)
        * magnitudo(i32));
    b32* vivus  = (b32*)piscina_allocare(piscina, (memoriae_index)(n
        + I)
        * magnitudo(b32));
    i32 d;
    i32 k;

    per (d = ZEPHYRUM; d < m; d++)
    {
        prima[d] = IV * n;
        pater[d] = d;
    }
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        si (prima[o[d]] == IV * n)
        {
            prima[o[d]] = d;
        }
        alioquin
        {
            alter[d]            = prima[o[d]];
            alter[prima[o[d]]]  = d;
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        vivus[k] = VERUM;
    }
    /* R1: ansa ad sedes s, s + 1 */
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        k = d / IV;
        si (alter[d] / IV == k && (d % IV + I) % IV == alter[d] % IV)
        {
            vivus[k] = FALSUM;
            pater[_radix(pater, o[IV * k + (d % IV + II) % IV])] =
                _radix(
                pater, o[IV * k + (d % IV + III) % IV]);
            redde _pd_renumerare(o, *signa, vivus, n, pater, piscina,
                ora,
                signa, c) ? I : -I;
        }
    }
    /* R2: os e = sedes a in j, a1 in k; os f = sedes b = a + 1 in j,
     * b1 in k */
    per (d = ZEPHYRUM; d < IV * n; d++)
    {
        i32 j   = d / IV;
        i32 a   = d % IV;
        i32 a1  = alter[d] % IV;
        i32 b   = (a + I) % IV;
        i32 b1  = alter[IV * j + b] % IV;

        k = alter[d] / IV;
        si (k == j || alter[IV * j + b] / IV != k)
        {
            perge;
        }
        si (a % II != a1 % II)
        {
            /* fibula: filum supra in uno, infra in altero */
            perge;
        }
        vivus[j] = FALSUM;
        vivus[k] = FALSUM;
        /* filum ossis e (sedes oppositae) et filum ossis f iunguntur */
        pater[_radix(pater, o[IV * j + (a + II) % IV])] = _radix(pater,
            o[IV * k + (a1 + II) % IV]);
        pater[_radix(pater, o[IV * j + (b + II) % IV])] = _radix(pater,
            o[IV * k + (b1 + II) % IV]);
        redde _pd_renumerare(o, *signa, vivus, n, pater, piscina, ora,
            signa, c) ? I : -I;
    }
    redde ZEPHYRUM;
}

b32
laqueus_pd_ex_plexu (
    constans s32*  verbum,
             i32   longitudo,
             i32   fila,
         Piscina*  piscina,
             i32** pd_exitus,
             i32*  transitus_exitus)
{
    i32  c = longitudo;
    i32  ossa;
    i32* x;
    i32* currens;
    i32* idem;
    i32* caput;
    i32* titulus;
    i32* pd;
    i32* ora;
    s32* signa;
    i32  proximum;
    i32  numerus = ZEPHYRUM;
    i32  e;
    i32  k;
    i32  j;

    si (   piscina == NIHIL || pd_exitus == NIHIL
        || transitus_exitus == NIHIL
        || fila == ZEPHYRUM || (c > ZEPHYRUM && verbum == NIHIL))
    {
        redde FALSUM;
    }
    si (c == ZEPHYRUM)
    {
        si (fila != I)
        {
            redde FALSUM;
        }
        *pd_exitus         = NIHIL;
        *transitus_exitus  = ZEPHYRUM;
        redde VERUM;
    }
    /* nodus: omne filum tangitur et permutatio fila coniungit, ergo c >=
     * fila - 1 (exacte); simul ossa = fila + 2c <= 3c + 1 et IV c sine
     * involutione i32 manent (recensio: fila 2^32 - 1 ossa involvebat) */
    si (fila > c + I || c > (i32)0x3FFFFFFFUL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        /* |littera| in i32 sine signo: -INT_MIN nullum UB */
        si (   verbum[k] == ZEPHYRUM || (verbum[k] > ZEPHYRUM
            ? (i32)verbum[k] : (i32)ZEPHYRUM - (i32)verbum[k]) >= fila)
        {
            redde FALSUM;
        }
    }
    ossa     = fila + II * c;
    x        = (i32*)piscina_allocare(piscina, (memoriae_index)IV * c
        * magnitudo(i32));
    currens  = (i32*)piscina_allocare(piscina, (memoriae_index)fila
        * magnitudo(i32));
    idem     = (i32*)piscina_allocare(piscina, (memoriae_index)ossa
        * magnitudo(i32));
    caput    = (i32*)piscina_allocare(piscina, (memoriae_index)ossa
        * magnitudo(i32));
    titulus  = (i32*)piscina_allocare(piscina, (memoriae_index)ossa
        * magnitudo(i32));
    pd       = (i32*)piscina_allocare(piscina, (memoriae_index)IV * c
        * magnitudo(i32));
    per (j = ZEPHYRUM; j < fila; j++)
    {
        currens[j] = j;
    }

    /* fila a summo deorsum; transitus k inter positiones i, i + 1:
     * a = filum sinistrum superius (it ad dextrum inferius), b =
     * dextrum superius (ad sinistrum inferius); ossa nova cn (positio
     * i) et dn (i + 1). sigma positivum: a infra, b supra ab X3 ad X1
     * -> [a, cn, dn, b]; negativum: b infra, a supra ab X1 ad X3 ->
     * [b, a, cn, dn]. Arma contra horologium. */
    proximum = fila;
    per (k = ZEPHYRUM; k < c; k++)
    {
        i32 i = (verbum[k] > ZEPHYRUM ? (i32)verbum[k]
            : (i32)ZEPHYRUM - (i32)verbum[k]) - I;
        i32 a   = currens[i];
        i32 b   = currens[i + I];
        i32 cn  = proximum++;
        i32 dn  = proximum++;

        si (verbum[k] > ZEPHYRUM)
        {
            x[IV * k]        = a;
            x[IV * k + I]    = cn;
            x[IV * k + II]   = dn;
            x[IV * k + III]  = b;
        }
        alioquin
        {
            x[IV * k]        = b;
            x[IV * k + I]    = a;
            x[IV * k + II]   = cn;
            x[IV * k + III]  = dn;
        }
        currens[i]      = cn;
        currens[i + I]  = dn;
    }

    /* clausura: os ultimum positionis j = os initiale j */
    per (e = ZEPHYRUM; e < ossa; e++)
    {
        idem[e]     = e;
        titulus[e]  = ZEPHYRUM;
    }
    per (j = ZEPHYRUM; j < fila; j++)
    {
        si (currens[j] == j)
        {
            redde FALSUM;
        }
        idem[currens[j]] = j;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        x[k] = idem[x[k]];
    }

    /* caput ossis: ubi intrat (X0; X3 si positivum, X1 si negativum) */
    per (k = ZEPHYRUM; k < c; k++)
    {
        caput[x[IV * k]] = IV * k;
        si (verbum[k] > ZEPHYRUM)
        {
            caput[x[IV * k + III]] = IV * k + III;
        }
        alioquin
        {
            caput[x[IV * k + I]] = IV * k + I;
        }
    }

    /* percursus ab osse 0: tituli 1..2c; aliter componentes plures */
    e = ZEPHYRUM;
    fac
    {
        i32 positio  = caput[e];
        i32 arma     = positio % IV;
        i32 exiens   = arma == ZEPHYRUM ? II : (arma == I ? III : I);

        titulus[e]  = ++numerus;
        e           = x[IV * (positio / IV) + exiens];
    }
    dum (e != ZEPHYRUM && numerus <= II * c);
    si (numerus != II * c)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        pd[k] = titulus[x[k]];
    }
    si (c >= II && !_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    *pd_exitus         = pd;
    *transitus_exitus  = c;
    redde VERUM;
}

b32
laqueus_pd_simplificare (
    constans i32*  pd,
             i32   transitus,
         Piscina*  piscina,
             i32** pd_exitus,
             i32*  transitus_exitus)
{
    i32* ora;
    s32* signa;
    i32  c = transitus;
    i32  k;

    si (c == I)
    {
        /* transitus unus: ora 1, 2 bis, X2 = X0 + 1 - ansa semper */
        si (   pd[ZEPHYRUM] < I || pd[ZEPHYRUM] > II
            || pd[II]  != III - pd[ZEPHYRUM] || pd[I] < I || pd[I] > II
            || pd[III] != III - pd[I])
        {
            redde FALSUM;
        }
        c = ZEPHYRUM;
    }
    si (c > ZEPHYRUM && !_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde FALSUM;
    }
    dum (c > I)
    {
        s32 effectus = _pd_motus(&ora, &signa, &c, piscina);

        si (effectus < ZEPHYRUM)
        {
            redde FALSUM;
        }
        si (effectus == ZEPHYRUM)
        {
            frange;
        }
    }
    si (c <= I)
    {
        /* diagramma unius transitus semper ansam habet */
        *pd_exitus         = NIHIL;
        *transitus_exitus  = ZEPHYRUM;
        redde VERUM;
    }
    *pd_exitus = (i32*)piscina_allocare(piscina, (memoriae_index)(IV * c
        + I) * magnitudo(i32));
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        (*pd_exitus)[k] = ora[k] + I;
    }
    *transitus_exitus = c;
    redde VERUM;
}

b32
diagramma_determinans (
     Diagramma  d,
       Piscina* piscina,
        Magnus* exitus)
{
    Polynomium delta;
       Fractio valor;

    si (   !diagramma_alexander(d, piscina, &delta)
        || !polynomium_valor(delta, fractio_ex_s64(-I), piscina,
        &valor))
    {
        redde FALSUM;
    }
    /* Delta in Z[t, t^-1]: valor ad -1 integer */
    *exitus = magnus_absolutum(fractio_numerator(valor), piscina);
    redde VERUM;
}


/* ==================================================
 * Motus trianguli
 * ================================================== */

interior vacuum
_laqueus_cum_vertice (
     Laqueus  l,
         i32  post,        /* insere post verticem 'post' */
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* vertices = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)(l.numerus + I) * magnitudo(Punctum));
        i32* initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
            l.componentes + I) * magnitudo(i32));
        i32 i;
        i32 k = _componens(l, post);

    per (i = ZEPHYRUM; i <= post; i++)
    {
        vertices[i] = l.vertices[i];
    }
    vertices[post + I] = c;
    per (i = post + I; i < l.numerus; i++)
    {
        vertices[i + I] = l.vertices[i];
    }
    per (i = ZEPHYRUM; i <= l.componentes; i++)
    {
        initia[i] = l.initia[i] + ((i > k) ? I : ZEPHYRUM);
    }
    exitus->vertices     = vertices;
    exitus->numerus      = l.numerus + I;
    exitus->initia       = initia;
    exitus->componentes  = l.componentes;
}

b32
laqueus_motus_addere (
     Laqueus  l,
         i32  i,
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus)
{
        i32 n;
        i32 j;
    Punctum a;
    Punctum b;

    si (i >= l.numerus)
    {
        redde FALSUM;
    }
    n = _sequens(l, i);
    a = l.vertices[i];
    b = l.vertices[n];
    si (_collinearia(a, b, c, piscina))
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < l.numerus; j++)
    {
        Punctum p = l.vertices[j];
        Punctum q = l.vertices[_sequens(l, j)];

        si (j == i)
        {
            perge;
        }
        si (j == _prior(l, i))
        {
            si (situs_triangulum_vicinum(a, b, c, p, piscina)
                != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
        alioquin si (j == n)
        {
            si (situs_triangulum_vicinum(b, c, a, q, piscina)
                != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
        alioquin si (situs_triangulum_segmentum(a, b, c, p, q, piscina)
                     != SITUS_DISIUNCTA)
        {
            redde FALSUM;
        }
    }
    _laqueus_cum_vertice(l, i, c, piscina, exitus);
    redde VERUM;
}

b32
laqueus_motus_removere (
     Laqueus  l,
         i32  i,
     Piscina* piscina,
     Laqueus* exitus)
{
        i32  k;
        i32  p;
        i32  n;
        i32  pp;
        i32  j;
    Punctum  a;
    Punctum  b;
    Punctum  c;
    Punctum* vertices;
        i32* initia;

    si (i >= l.numerus)
    {
        redde FALSUM;
    }
    k = _componens(l, i);
    si (l.initia[k + I] - l.initia[k] < IV)
    {
        redde FALSUM;
    }
    p   = _prior(l, i);
    n   = _sequens(l, i);
    pp  = _prior(l, p);
    a   = l.vertices[p];
    b   = l.vertices[i];
    c   = l.vertices[n];
    si (_collinearia(a, b, c, piscina))
    {
        /* vertex in segmento [a, c]: nihil verritur; aliter reflexio */
        si (situs_segmenta(a, c, b, b, piscina) == SITUS_DISIUNCTA)
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        per (j = ZEPHYRUM; j < l.numerus; j++)
        {
            Punctum x = l.vertices[j];
            Punctum y = l.vertices[_sequens(l, j)];

            si (j == p || j == i)
            {
                perge;
            }
            si (j == pp)
            {
                si (situs_triangulum_vicinum(a, b, c, x, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (j == n)
            {
                si (situs_triangulum_vicinum(c, a, b, y, piscina)
                    != SITUS_DISIUNCTA)
                {
                    redde FALSUM;
                }
            }
            alioquin si (situs_triangulum_segmentum(a, b, c, x, y,
                         piscina) != SITUS_DISIUNCTA)
            {
                redde FALSUM;
            }
        }
    }
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        l.numerus * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    per (j = ZEPHYRUM; j < l.numerus; j++)
    {
        si (j < i)
        {
            vertices[j] = l.vertices[j];
        }
        alioquin si (j > i)
        {
            vertices[j - I] = l.vertices[j];
        }
    }
    per (j = ZEPHYRUM; j <= l.componentes; j++)
    {
        initia[j] = l.initia[j] - ((j > k) ? I : ZEPHYRUM);
    }
    exitus->vertices     = vertices;
    exitus->numerus      = l.numerus - I;
    exitus->initia       = initia;
    exitus->componentes  = l.componentes;
    redde VERUM;
}

b32
laqueus_simplificare (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus)
{
    PiscinaNotatio  nota;
           Punctum* vertices;
               i32* initia;
           Laqueus  cur;
               b32  simplex;
               b32  mutatum = VERUM;
               i32  k;

    nota     = piscina_notare(piscina);
    simplex  = laqueus_simplex(l, piscina);
    piscina_reficere(piscina, nota);
    si (!simplex)
    {
        redde FALSUM;
    }
    /* tabula laboris propria: motus removere solum ut oraculum legitimi
     * vocatur et statim reficitur */
    vertices = (Punctum*)piscina_allocare(piscina, (memoriae_index)
        l.numerus * magnitudo(Punctum));
    initia = (i32*)piscina_allocare(piscina, (memoriae_index)(
        l.componentes + I) * magnitudo(i32));
    memcpy(vertices, l.vertices, (memoriae_index)l.numerus
        * magnitudo(Punctum));
    memcpy(initia, l.initia, (memoriae_index)(l.componentes + I)
        * magnitudo(i32));
    cur.vertices     = vertices;
    cur.numerus      = l.numerus;
    cur.initia       = initia;
    cur.componentes  = l.componentes;
    dum (mutatum)
    {
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < cur.numerus; k++)
        {
            Laqueus ignotus;
                b32 legitimus;
                i32 j;
                i32 m;

            nota       = piscina_notare(piscina);
            legitimus  = laqueus_motus_removere(cur, k, piscina,
                &ignotus);
            piscina_reficere(piscina, nota);
            si (!legitimus)
            {
                perge;
            }
            /* vertex k e tabula laboris removetur */
            per (j = k; j + I < cur.numerus; j++)
            {
                vertices[j] = vertices[j + I];
            }
            per (m = ZEPHYRUM; m <= cur.componentes; m++)
            {
                si (initia[m] > k)
                {
                    initia[m]--;
                }
            }
            cur.numerus--;
            mutatum = VERUM;
            frange;
        }
    }
    *exitus = cur;
    redde VERUM;
}

b32
laqueus_speculum (
     Laqueus  l,
     Piscina* piscina,
     Laqueus* exitus)
{
    Punctum* puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)
        l.numerus * magnitudo(Punctum));
        i32 k;

    per (k = ZEPHYRUM; k < l.numerus; k++)
    {
        puncta[k]    = l.vertices[k];
        puncta[k].z  = fractio_nega(l.vertices[k].z, piscina);
    }
    redde laqueus_ex_punctis(puncta, l.initia, l.componentes, piscina,
        exitus);
}


/* ==================================================
 * Modulus vertex: uncinus per contractionem retis
 *
 * Index per os (dimensio 2): spin +1 = ansa per os a dart a (primus
 * occursus in codice) ad dart b. Gyrationes t (quartae, a -> b) ex
 * arbore dualis: facies 0 (orbita dart 0) +4, ceterae -4, quisque
 * angulus -1. Tensor transitus = A [(X0 X1)(X2 X3)] + A^-1 [(X0 X3)(X1
 * X2)], par quodque 0 nisi alter finis intrat et alter exit, deinde
 * omega^-1 (arma p -> p+1) aut omega^+1; factor ossis omega^(spin t) ad
 * dart b. Os cuius ambo darts in eodem transitu (ansa R1) index non
 * est: spin eius in tensore summatur. Contractio = d <D>, d = omega^4
 * + omega^-4 = -A^2 - A^-2 cum A = i omega^2. Vide
 * project-specs/contractio-plan.md (The vertex model, Why it is
 * correct).
 * ================================================== */

/* terminus tensoris transitus: elementum += A^potentia_a omega^exponens */
nomen structura {
    i32 elementum;
    s32 potentia_a;
    s32 exponens;
} TerminusVerticis;

structura LaqueusModulusVertex {
                 i32  transitus;
                 s32  scriptura;
                 s32  imus;               /* fenestra uncinus, classis 3w */
                 s32  summus;
                 s32* gyrationes;         /* per os, a -> b */
      ContractioRete* rete;
      ContractioOrdo* ordo;
  ContractioOfficina* officina;
                 i32** valores;           /* per transitum */
                 i32* elementa;           /* per transitum */
                 i32* initia_terminorum;  /* transitus + 1 */
    TerminusVerticis* termini;
                 s32  exponens_maximus;   /* E >= IV */
                 i32* potentiae;          /* omega^j, j in [-E, E] */
                 i32  primus;             /* primus currens, 0 nullus */
                 i32  radix_i;            /* i^2 = -1 modulo primum */
                 i32  limes;              /* bita B (T5a) */
               Magnus limes_exactus;      /* B = min Pi gradus */
                 i32  aestimationes;      /* contractiones factae */
                 i32  mutationes_primi;   /* primi novi in aestimare */
};

/* residuum non negativum s64 modulo IV */
interior s64
_modulo_quattuor (
    s64 x)
{
    s64 r = x % (s64)IV;

    redde r < ZEPHYRUM ? r + (s64)IV : r;
}

/* gyrationes t per os (a -> b) ex aequationibus facierum per arborem
 * dualem (BFS a facie 0, folia primum); *faciei_exitus = facies per
 * positionem (0..c+1, orbita sequentis). ora 0-basata, codex planus.
 * FALSUM si aequatio non integra aut residuum manet (codex non planus:
 * non accidit post _pd_legere). */
interior b32
_gyrationes (
    constans i32*  ora,
             i32   c,
    constans i32*  finis_a,
    constans i32*  finis_b,
         Piscina*  piscina,
             s32*  gyrationes,
             i32** faciei_exitus)
{
    i32  n = IV * c;
    i32  m = II * c;
    i32* faciei = (i32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(i32));
    i32* alter  = (i32*)piscina_allocare(piscina, (memoriae_index)n
        * magnitudo(i32));
    s64* residua;
    s64* basis  = (s64*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(s64));
    i32* initia;
    i32* ossa_faciei;
    i32* parens;
    i32* ordo;
    i32  facies = ZEPHYRUM;
    i32  caput;
    i32  cauda;
    i32  d;
    i32  l;
    i32  f;

    *faciei_exitus = faciei;
    per (l = ZEPHYRUM; l < m; l++)
    {
        alter[finis_a[l]] = finis_b[l];
        alter[finis_b[l]] = finis_a[l];
        basis[l]          = (s64)((finis_b[l] % IV + IV - finis_a[l]
            % IV
            + II) % IV);
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        faciei[d] = n;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 e = d;

        si (faciei[d] != n)
        {
            perge;
        }
        dum (faciei[e] == n)
        {
            faciei[e] = facies;
            e = IV * (alter[e] / IV) + (alter[e] % IV + I)
                % IV;
        }
        facies++;
    }
    si (facies != c + II)
    {
        redde FALSUM;
    }

    /* R_f = scopus + longitudo - summa s basis; scopus +4 externa, -4
     * ceterae; quartae partes integrae */
    residua = (s64*)piscina_allocare(piscina, (memoriae_index)facies
        * magnitudo(s64));
    per (f = ZEPHYRUM; f < facies; f++)
    {
        residua[f] = f == ZEPHYRUM ? (s64)IV : -(s64)IV;
    }
    per (d = ZEPHYRUM; d < n; d++)
    {
        i32 os = ora[d];

        residua[faciei[d]] += (s64)I - (finis_a[os] == d ? basis[os]
            : -basis[os]);
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        si (_modulo_quattuor(residua[f]) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        residua[f] /= (s64)IV;
    }

    /* graphum duale: ossa per faciem (quodque os in duabus faciebus) */
    initia       = (i32*)piscina_allocare(piscina,
        (memoriae_index)(facies + I) * magnitudo(i32));
    ossa_faciei  = (i32*)piscina_allocare(piscina, (memoriae_index)(II
        * m) * magnitudo(i32));
    parens       = (i32*)piscina_allocare(piscina,
        (memoriae_index)facies * magnitudo(i32));
    ordo         = (i32*)piscina_allocare(piscina,
        (memoriae_index)facies * magnitudo(i32));
    per (f = ZEPHYRUM; f <= facies; f++)
    {
        initia[f] = ZEPHYRUM;
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        initia[faciei[finis_a[l]] + I]++;
        initia[faciei[finis_b[l]] + I]++;
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        initia[f + I]  += initia[f];
        parens[f]      = m;
    }
    {
        i32* positio = (i32*)piscina_allocare(piscina,
            (memoriae_index)facies * magnitudo(i32));

        per (f = ZEPHYRUM; f < facies; f++)
        {
            positio[f] = initia[f];
        }
        per (l = ZEPHYRUM; l < m; l++)
        {
            ossa_faciei[positio[faciei[finis_a[l]]]++] = l;
            ossa_faciei[positio[faciei[finis_b[l]]]++] = l;
        }
    }

    /* BFS a facie externa; parens[f] = os arboris (m: nullum) */
    ordo[ZEPHYRUM]    = ZEPHYRUM;
    parens[ZEPHYRUM]  = II * m;
    caput             = ZEPHYRUM;
    cauda             = I;
    dum (caput < cauda)
    {
        i32 g = ordo[caput++];
        i32 j;

        per (j = initia[g]; j < initia[g + I]; j++)
        {
            i32 os  = ossa_faciei[j];
            i32 fa  = faciei[finis_a[os]];
            i32 h   = fa == g ? faciei[finis_b[os]] : fa;

            si (parens[h] == m)
            {
                parens[h]      = os;
                ordo[cauda++]  = h;
            }
        }
    }
    si (cauda != facies)
    {
        redde FALSUM;
    }

    /* m_l: folia primum; ossa extra arborem m = 0 */
    per (l = ZEPHYRUM; l < m; l++)
    {
        gyrationes[l] = (s32)basis[l];
    }
    dum (cauda > I)
    {
        i32 g     = ordo[--cauda];
        i32 os    = parens[g];
        i32 fa    = faciei[finis_a[os]];
        i32 fb    = faciei[finis_b[os]];
        s64 gyri  = fa == g ? residua[g] : -residua[g];

        residua[fa]     -= gyri;
        residua[fb]     += gyri;
        gyrationes[os]  = (s32)(basis[os] + (s64)IV * gyri);
    }
    per (f = ZEPHYRUM; f < facies; f++)
    {
        si (residua[f] != ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* factorem in productum: acervus u64 (< 2^32 ante multiplicationem,
 * factor < 2^31, ergo < 2^63) in Magnus solum cum crescit effunditur */
interior vacuum
_productum_addere (
        i64* acervus,
     Magnus* productum,
        i32  factor,
    Piscina* piscina)
{
    *acervus *= (i64)factor;
    si (*acervus >= ((i64)I << XXXII))
    {
        *productum = magnus_multiplica(*productum,
            magnus_ex_s64((s64)*acervus), piscina);
        *acervus = I;
    }
}

/* limes coefficientium uncinus (contractio-plan T5a): Sigma |c_k| <=
 * tau(G) <= Pi_{v != r} gradus(v) super graphum Tait utriusque
 * coloris; *limes_exitus = bita minimi, *exactus = minimum. Anguli
 * (k, j) = regio inter arma j et j+1 = faciei[4k + (j+1) % 4]; colores
 * per unionem-inventionem angulorum OPPOSITORUM (0~2, 1~3), non per
 * regulam paritatis (quae in diagrammatibus non alternantibus fallit).
 * Gradus sine colore: per transitum, si anguli oppositi regiones
 * diversae sunt, utraque +1 (ansae Tait excluduntur). FALSUM nisi
 * exacte duae classes et anguli vicini in classibus diversis. */
interior b32
_limes_arborum (
     constans i32* faciei,
              i32  c,
          Piscina* piscina,
           Magnus* exactus,
              i32* limes_exitus)
{
    i32  F       = c + II;
    i32* pater   = (i32*)piscina_allocare(piscina, (memoriae_index)F
        * magnitudo(i32));
    i32* gradus  = (i32*)piscina_allocare(piscina, (memoriae_index)F
        * magnitudo(i32));
       i32 radices[II];
       i32 numerus_radicum = ZEPHYRUM;
    Magnus producta[II];
       i32 q;
       i32 k;
       i32 j;
       i32 f;

    per (f = ZEPHYRUM; f < F; f++)
    {
        pater[f]   = f;
        gradus[f]  = ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (j = ZEPHYRUM; j < II; j++)
        {
            i32 a = faciei[IV * k + (j + I) % IV];
            i32 b = faciei[IV * k + (j + III) % IV];

            pater[_radix(pater, a)] = _radix(pater, b);
            si (a != b)
            {
                gradus[a]++;
                gradus[b]++;
            }
        }
    }
    per (f = ZEPHYRUM; f < F; f++)
    {
        si (_radix(pater, f) == f)
        {
            si (numerus_radicum == II)
            {
                redde FALSUM;
            }
            radices[numerus_radicum++] = f;
        }
    }
    si (numerus_radicum != II)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        per (j = ZEPHYRUM; j < IV; j++)
        {
            si (_radix(pater, faciei[IV * k + (j + I) % IV])
                == _radix(pater, faciei[IV * k + (j + II) % IV]))
            {
                redde FALSUM;
            }
        }
    }

    /* per colorem: radix arboris = vertex gradus maximi exclusus */
    per (q = ZEPHYRUM; q < II; q++)
    {
        i32 maximus = F;
        i64 acervus = I;

        producta[q] = magnus_ex_s64((s64)I);
        per (f = ZEPHYRUM; f < F; f++)
        {
            si (   _radix(pater, f) == radices[q]
                && (maximus == F || gradus[f] > gradus[maximus]))
            {
                maximus = f;
            }
        }
        per (f = ZEPHYRUM; f < F; f++)
        {
            si (_radix(pater, f) == radices[q] && f != maximus)
            {
                _productum_addere(&acervus, &producta[q],
                    gradus[f] > ZEPHYRUM ? gradus[f] : I, piscina);
            }
        }
        producta[q] = magnus_multiplica(producta[q],
            magnus_ex_s64((s64)acervus), piscina);
    }
    *exactus = magnus_compara(producta[ZEPHYRUM], producta[I])
        <= ZEPHYRUM
        ? producta[ZEPHYRUM] : producta[I];
    *limes_exitus = magnus_bitorum(*exactus);
    redde VERUM;
}

/* termini tensoris transitus k: omnes spins ossium distinctorum in
 * transitu (ansa R1: ambo arma idem os, summatur), elementum ex
 * spinibus ossium externorum (ordine armorum; spin -1 = bitum 1,
 * ultimus celerrimus). Redde numerum terminorum scriptorum. */
interior i32
_terminos_transitus (
        constans i32* ora,
                 i32  k,
        constans i32* finis_a,
        constans i32* finis_b,
        constans s32* gyrationes,
    TerminusVerticis* termini,
                 i32* externa,
                 i32* numerus_externorum)
{
    constans i32 paria[II][IV] = {
        { ZEPHYRUM, I, II, III },    /* A: (X0 X1)(X2 X3) */
        { ZEPHYRUM, III, I, II }     /* B: (X0 X3)(X1 X2) */
    };
    i32 distincta[IV];
    i32 numerus_distinctorum = ZEPHYRUM;
    i32 situs_ossis[IV];             /* arma j -> index in distinctis */
    i32 positio_externa[IV];         /* distinctum -> positio, aut IV */
    i32 scripti = ZEPHYRUM;
    i32 masca;
    i32 j;
    i32 u;

    *numerus_externorum = ZEPHYRUM;
    per (j = ZEPHYRUM; j < IV; j++)
    {
        i32 os = ora[IV * k + j];

        per (u = ZEPHYRUM; u < numerus_distinctorum; u++)
        {
            si (distincta[u] == os)
            {
                frange;
            }
        }
        si (u == numerus_distinctorum)
        {
            distincta[numerus_distinctorum++] = os;
            si (finis_a[os] / IV == finis_b[os] / IV)
            {
                positio_externa[u] = IV;
            }
            alioquin
            {
                positio_externa[u]                = *numerus_externorum;
                externa[(*numerus_externorum)++]  = os;
            }
        }
        situs_ossis[j] = u;
    }

    per (masca = ZEPHYRUM; masca < ((i32)I << numerus_distinctorum);
         masca++)
    {
        b32 intrat[IV];
        s32 exponens_ossium  = ZEPHYRUM;
        i32 elementum        = ZEPHYRUM;
        i32 s;

        per (u = ZEPHYRUM; u < numerus_distinctorum; u++)
        {
            i32 os     = distincta[u];
            b32 minus  = ((masca >> u) & I) != ZEPHYRUM;

            si (finis_b[os] / IV == k)
            {
                exponens_ossium += minus ? -gyrationes[os]
                    : gyrationes[os];
            }
            si (positio_externa[u] != IV && minus)
            {
                elementum |= (i32)I << (*numerus_externorum - I
                    - positio_externa[u]);
            }
        }
        per (j = ZEPHYRUM; j < IV; j++)
        {
            i32 os     = ora[IV * k + j];
            b32 minus  = ((masca >> situs_ossis[j]) & I) != ZEPHYRUM;

            /* ad dart b os intrat sse spin +1; ad dart a sse spin -1 */
            intrat[j] = finis_b[os] == IV * k + j ? !minus : minus;
        }
        per (s = ZEPHYRUM; s < II; s++)
        {
            s32 exponens  = exponens_ossium;
            b32 validum   = VERUM;
            i32 q;

            per (q = ZEPHYRUM; q < IV; q += II)
            {
                i32 p0 = paria[s][q];
                i32 p1 = paria[s][q + I];
                i32 in;
                i32 ex;

                si (intrat[p0] == intrat[p1])
                {
                    validum = FALSUM;
                    frange;
                }
                in = intrat[p0] ? p0 : p1;
                ex = intrat[p0] ? p1 : p0;
                exponens += (ex + IV - in) % IV == I ? -(s32)I : (s32)I;
            }
            si (validum)
            {
                termini[scripti].elementum   = elementum;
                termini[scripti].potentia_a  = s == ZEPHYRUM ? (s32)I
                    : -(s32)I;
                termini[scripti].exponens    = exponens;
                scripti++;
            }
        }
    }
    redde scripti;
}

LaqueusModulusVertex*
laqueus_modulus_vertex_creare (
    constans i32* pd,
             i32  transitus,
             i64  octeti_maximi,
         Piscina* piscina)
{
    LaqueusModulusVertex* mv;
      ContractioOptiones  optiones = contractio_optiones_ordinariae();
                     i32* ora;
                     s32* signa;
                     i32* finis_a;
                     i32* finis_b;
                     i32* faciei;
                     i32* index_ossis;
                     i32* pater;
                     i32  c = transitus;
                     i32  m;
                     i32  ansae_a = ZEPHYRUM;
                     i32  ansae_b = ZEPHYRUM;
                     s32  classis;
                     s32  e_max = IV;
                     i32  k;
                     i32  l;

    si (   pd == NIHIL || piscina == NIHIL || c < II
        || !_pd_legere(pd, c, piscina, &ora, &signa))
    {
        redde NIHIL;
    }
    m      = II * c;
    mv     = (LaqueusModulusVertex*)piscina_allocare(piscina,
        magnitudo(LaqueusModulusVertex));
    finis_a = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    finis_b = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        finis_a[l] = IV * c;
    }
    per (k = ZEPHYRUM; k < IV * c; k++)
    {
        si (finis_a[ora[k]] == IV * c)
        {
            finis_a[ora[k]] = k;
        }
        alioquin
        {
            finis_b[ora[k]] = k;
        }
    }
    mv->transitus         = c;
    mv->primus            = ZEPHYRUM;
    mv->radix_i           = ZEPHYRUM;
    mv->aestimationes     = ZEPHYRUM;
    mv->mutationes_primi  = ZEPHYRUM;
    mv->gyrationes  = (s32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(s32));
    si (   !_gyrationes(ora, c, finis_a, finis_b, piscina,
        mv->gyrationes,
               &faciei)
        || !_limes_arborum(faciei, c, piscina, &mv->limes_exactus,
               &mv->limes))
    {
        redde NIHIL;
    }

    /* rete: index per os non R1, tensor per transitum */
    mv->rete     = contractio_rete_creare(piscina);
    index_ossis  = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (   finis_a[l] / IV != finis_b[l] / IV
            && !contractio_index_addere(mv->rete, II, &index_ossis[l]))
        {
            redde NIHIL;
        }
    }
    mv->elementa           = (i32*)piscina_allocare(piscina,
        (memoriae_index)c * magnitudo(i32));
    mv->valores            = (i32**)piscina_allocare(piscina,
        (memoriae_index)c * magnitudo(i32*));
    mv->initia_terminorum  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(c + I) * magnitudo(i32));
    mv->termini            =
        (TerminusVerticis*)piscina_allocare(piscina,
        (memoriae_index)c * XXXII * magnitudo(TerminusVerticis));
    mv->initia_terminorum[ZEPHYRUM] = ZEPHYRUM;
    per (k = ZEPHYRUM; k < c; k++)
    {
        i32 externa[IV];
        i32 indices[IV];
        i32 numerus_externorum;
        i32 numerus;
        i32 id;
        i32 j;

        numerus = _terminos_transitus(ora, k, finis_a, finis_b,
            mv->gyrationes, mv->termini + mv->initia_terminorum[k],
            externa, &numerus_externorum);
        mv->initia_terminorum[k + I] = mv->initia_terminorum[k]
            + numerus;
        per (j = ZEPHYRUM; j < numerus_externorum; j++)
        {
            indices[j] = index_ossis[externa[j]];
        }
        si (!contractio_tensor_addere(mv->rete, indices,
            numerus_externorum, &id))
        {
            redde NIHIL;
        }
        mv->elementa[k] = (i32)I << numerus_externorum;
        mv->valores[k]  = (i32*)piscina_allocare(piscina,
            (memoriae_index)mv->elementa[k] * magnitudo(i32));
        per (j = mv->initia_terminorum[k]; j < mv->initia_terminorum[k
            + I]; j++)
        {
            s32 e = mv->termini[j].exponens;

            si (e < ZEPHYRUM)
            {
                e = -e;
            }
            si (e > e_max)
            {
                e_max = e;
            }
        }
    }
    mv->exponens_maximus = e_max;
    mv->potentiae        = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * e_max + I) * magnitudo(i32));
    /* ordo optimus inter candidatos (contractio-plan T5b): idem
     * uncinus, tempus per punctum saepe multo minus in tricis */
    optiones.methodus = CONTRACTIO_OPTIMUS;
    optiones.octeti_maximi = octeti_maximi;
    mv->ordo = contractio_ordinare(mv->rete, &optiones, piscina);
    si (mv->ordo == NIHIL)
    {
        redde NIHIL;
    }
    mv->officina = contractio_officina_creare(mv->ordo, octeti_maximi,
        piscina);
    si (mv->officina == NIHIL)
    {
        redde NIHIL;
    }

    /* fenestra Kauffman [-c - 2|s_B| + 2, c + 2|s_A| - 2], ad classem
     * 3w modulo 4 rotundata */
    pater = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * magnitudo(i32));
    per (l = ZEPHYRUM; l < m; l++)
    {
        pater[l] = l;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = ora + IV * k;

        pater[_radix(pater, x[ZEPHYRUM])]  = _radix(pater, x[I]);
        pater[_radix(pater, x[II])]        = _radix(pater, x[III]);
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (_radix(pater, l) == l)
        {
            ansae_a++;
        }
        pater[l] = l;
    }
    per (k = ZEPHYRUM; k < c; k++)
    {
        constans i32* x = ora + IV * k;

        pater[_radix(pater, x[ZEPHYRUM])]  = _radix(pater, x[III]);
        pater[_radix(pater, x[I])]         = _radix(pater, x[II]);
    }
    per (l = ZEPHYRUM; l < m; l++)
    {
        si (_radix(pater, l) == l)
        {
            ansae_b++;
        }
    }
    mv->scriptura = ZEPHYRUM;
    per (k = ZEPHYRUM; k < c; k++)
    {
        mv->scriptura += signa[k];
    }
    classis = (s32)_modulo_quattuor((s64)III * (s64)mv->scriptura);
    mv->imus = -(s32)c - II * (s32)ansae_b + II;
    mv->imus += (s32)_modulo_quattuor((s64)classis - (s64)mv->imus);
    mv->summus = (s32)c + II * (s32)ansae_a - II;
    mv->summus -= (s32)_modulo_quattuor((s64)mv->summus - (s64)classis);
    redde mv;
}

s32
laqueus_modulus_vertex_aestimare (
    LaqueusModulusVertex* mv,
                    i32  omega,
                    i32  p,
                    i32* punctum,
                    i32* valor)
{
    i32 a;
    i32 a_inversa;
    i32 w;
    i32 w_inversa;
    i32 d;
    i32 d_inversa;
    i32 z;
    s32 e_max;
    s32 j;
    i32 k;

    si (   mv == NIHIL || punctum == NIHIL || valor == NIHIL || p < V
        || p % IV != I)
    {
        redde INTERPOLATIO_ERRATUM;
    }
    si (p != mv->primus)
    {
        /* primus PROBATUS ante quaestionem g: modulus compositus 3 | p
         * nullum g habet (scansio ~p), alii falso accipiuntur */
        i32 g = II;

        si (!congruentia_est_primus(p))
        {
            redde INTERPOLATIO_ERRATUM;
        }
        /* i = g^((p-1)/4), g non-residuum quadraticum minimum (p
         * primus: dimidium residuorum, g parvum invenitur; limes g < p
         * custos tantum - sine eo modulus compositus, si probatio
         * umquam praeteritur, in aeternum quaereret) */
        dum (   g < p
             && congruentia_potentia(g, (i64)((p - I) / II), p) != p
                 - I)
        {
            g++;
        }
        si (g == p)
        {
            redde INTERPOLATIO_ERRATUM;
        }
        mv->radix_i  = congruentia_potentia(g, (i64)((p - I) / IV), p);
        mv->primus   = p;
        mv->mutationes_primi++;
    }
    w = omega % p;
    si (w == ZEPHYRUM || !congruentia_inversa(w, p, &w_inversa))
    {
        redde INTERPOLATIO_INUTILE;
    }

    /* potentiae omega^j, j in [-E, E] */
    e_max                 = mv->exponens_maximus;
    mv->potentiae[e_max]  = I;
    per (j = I; j <= e_max; j++)
    {
        mv->potentiae[e_max + j] = congruentia_multiplica(
            mv->potentiae[e_max + j - I], w, p);
        mv->potentiae[e_max - j] = congruentia_multiplica(
            mv->potentiae[e_max - j + I], w_inversa, p);
    }
    d = congruentia_adde(mv->potentiae[e_max + IV],
        mv->potentiae[e_max - IV], p);
    si (d == ZEPHYRUM)
    {
        redde INTERPOLATIO_INUTILE;
    }
    a = congruentia_multiplica(mv->radix_i, mv->potentiae[e_max + II],
        p);
    a_inversa = congruentia_multiplica(p - mv->radix_i,
        mv->potentiae[e_max - II], p);

    per (k = ZEPHYRUM; k < mv->transitus; k++)
    {
        per (j = ZEPHYRUM; j < (s32)mv->elementa[k]; j++)
        {
            mv->valores[k][j] = ZEPHYRUM;
        }
        per (j = (s32)mv->initia_terminorum[k];
             j < (s32)mv->initia_terminorum[k + I]; j++)
        {
            constans TerminusVerticis* t = &mv->termini[j];

            mv->valores[k][t->elementum] = congruentia_adde(
                mv->valores[k][t->elementum], congruentia_multiplica(
                t->potentia_a > ZEPHYRUM ? a : a_inversa,
                mv->potentiae[e_max + t->exponens], p), p);
        }
    }
    mv->aestimationes++;
    si (!contractio_exsequi_modulo(mv->rete, mv->ordo, mv->officina,
        (constans i32* constans*)mv->valores, p, &z))
    {
        redde INTERPOLATIO_ERRATUM;
    }
    (vacuum)congruentia_inversa(d, p, &d_inversa);
    *punctum  = a;
    *valor    = congruentia_multiplica(z, d_inversa, p);
    redde INTERPOLATIO_UTILE;
}

constans s32*
laqueus_modulus_vertex_gyrationes (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? NIHIL : mv->gyrationes;
}

ContractioSumptus
laqueus_modulus_vertex_sumptus (
    constans LaqueusModulusVertex* mv)
{
    redde contractio_sumptus(mv == NIHIL ? NIHIL : mv->ordo);
}

constans ContractioOrdo*
laqueus_modulus_vertex_ordo (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? NIHIL : mv->ordo;
}

i32
laqueus_modulus_vertex_limes (
    constans LaqueusModulusVertex* mv)
{
    redde mv == NIHIL ? ZEPHYRUM : mv->limes;
}

b32
laqueus_modulus_vertex_limes_exactus (
    constans LaqueusModulusVertex* mv,
                           Magnus* exitus)
{
    si (mv == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    *exitus = mv->limes_exactus;
    redde VERUM;
}

vacuum
laqueus_modulus_vertex_vocationes (
    constans LaqueusModulusVertex* mv,
                              i32* aestimationes,
                              i32* primi)
{
    si (aestimationes != NIHIL)
    {
        *aestimationes = mv == NIHIL ? ZEPHYRUM : mv->aestimationes;
    }
    si (primi != NIHIL)
    {
        *primi = mv == NIHIL ? ZEPHYRUM : mv->mutationes_primi;
    }
}

/* aestimator interpolationis: omega = index + 2 */
interior s32
_aestimator_verticis (
     vacuum* contextus,
        i32  index,
        i32  p,
        i32* punctum,
        i32* valor)
{
    redde laqueus_modulus_vertex_aestimare(
        (LaqueusModulusVertex*)contextus, index + II, p, punctum,
        valor);
}

/* uncinus exactus: puncta in classe 3w (passus 4), limes = bita
 * minimi Pi gradus super graphos Tait (T5a: Sigma |c_k| <= arbores
 * extensae <= Pi gradus; numquam > 2c), primi 1 modulo 4 */
interior b32
_uncinus_per_rete (
    LaqueusModulusVertex* mv,
                 Piscina* piscina,
              Polynomium* exitus)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    q.gradus_imus     = mv->imus;
    q.gradus_summus   = mv->summus;
    q.passus          = IV;
    q.limes_bitorum   = mv->limes;
    q.residuum_primi  = I;
    q.modulus_primi   = IV;
    q.puncta_addita   = II;
    q.conatus_maximi  = (i32)((mv->summus - mv->imus) / IV) + I + II
        + LXIV;
    redde interpolatio_polynomium(_aestimator_verticis, mv, &q, piscina,
        exitus);
}

b32
laqueus_modulus_vertex_uncinus (
    LaqueusModulusVertex* mv,
                 Piscina* piscina,
              Polynomium* exitus)
{
    si (mv == NIHIL || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    redde _uncinus_per_rete(mv, piscina, exitus);
}

interior vacuum
_sumptus_nullus (
    ContractioSumptus* sumptus)
{
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(NIHIL);
    }
}

b32
laqueus_uncinus_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus)
{
    LaqueusModulusVertex* mv;

    si (exitus == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (transitus == ZEPHYRUM)
    {
        _sumptus_nullus(sumptus);
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    mv = laqueus_modulus_vertex_creare(pd, transitus, octeti_maximi,
        piscina);
    si (mv == NIHIL || !_uncinus_per_rete(mv, piscina, exitus))
    {
        redde FALSUM;
    }
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(mv->ordo);
    }
    redde VERUM;
}

b32
laqueus_jones_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
           Polynomium* exitus,
    ContractioSumptus* sumptus)
{
    LaqueusModulusVertex* mv;
              Polynomium  uncinus;

    si (exitus == NIHIL || piscina == NIHIL)
    {
        redde FALSUM;
    }
    si (transitus == ZEPHYRUM)
    {
        _sumptus_nullus(sumptus);
        *exitus = polynomium_constans(magnus_ex_s64(I), piscina);
        redde VERUM;
    }
    mv = laqueus_modulus_vertex_creare(pd, transitus, octeti_maximi,
        piscina);
    si (   mv == NIHIL || !_uncinus_per_rete(mv, piscina, &uncinus)
        || !_jones_ex_uncino(uncinus, mv->scriptura, piscina, exitus))
    {
        redde FALSUM;
    }
    si (sumptus != NIHIL)
    {
        *sumptus = contractio_sumptus(mv->ordo);
    }
    redde VERUM;
}
#undef _aestimator_verticis
#undef _alexander_ex_arcubus
#undef _collinearia
#undef _componens
#undef _est_spatium
#undef _gyrationes
#undef _jones_ex_uncino
#undef _laqueus_cum_vertice
#undef _limes_arborum
#undef _modulo_quattuor
#undef _ora_diagrammatis
#undef _pd_legere
#undef _pd_motus
#undef _pd_planus
#undef _pd_renumerare
#undef _positio_sequens
#undef _prior
#undef _productum_addere
#undef _punctum_legere
#undef _radix
#undef _sequens
#undef _sine_spatiis
#undef _sumptus_nullus
#undef _terminos_transitus
#undef _uncinus_ex_oris
#undef _uncinus_per_rete
#undef TerminusVerticis
/* lib/chorda_aedificator.c: statica per plagulam renominata */
#define _appendere_interna _appendere_interna_chorda_aedificator
#define _crescere _crescere_chorda_aedificator
#define _evadere_json _evadere_json_chorda_aedificator
#define _format_duplex _format_duplex_chorda_aedificator
#define _format_integer_i32 _format_integer_i32_chorda_aedificator
#define _format_integer_s32 _format_integer_s32_chorda_aedificator
#define _proxima_capacitas _proxima_capacitas_chorda_aedificator
#line 1 "lib/chorda_aedificator.c"


#include <stdio.h>
#include <string.h>


/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura ChordaAedificator {
                i8* buffer;
    memoriae_index  capacitas;
    memoriae_index  offset;
           Piscina* piscina;
               i32  indentatio_gradus;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas (
    memoriae_index nunc)
{
    /* Duplica capacitatem donec satis habeamus */
    redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere (
    ChordaAedificator* aedificator,
       memoriae_index  necessaria)
{
    memoriae_index  capacitas_nova;
                i8* buffer_novum;

    capacitas_nova = aedificator->capacitas;
    dum (capacitas_nova < necessaria)
    {
        capacitas_nova = _proxima_capacitas(capacitas_nova);
    }

    /* AMPLIATIO primum (2026-10-21): buffer allocatio ultima aut sola
     * alvei est - augetur sine copia, buffer vetus non relinquitur
     * (duplicatio in arena olim ~II x finalem otiosum relinquebat).
     * NIHIL = non potest: allocare et copiare ut antea. */
    buffer_novum = (i8*)piscina_conari_ampliare(aedificator->piscina,
        aedificator->buffer, aedificator->capacitas, capacitas_nova);
    si (buffer_novum != NIHIL)
    {
        aedificator->buffer     = buffer_novum;
        aedificator->capacitas  = capacitas_nova;
        redde VERUM;
    }

    buffer_novum = (i8*)piscina_allocare(aedificator->piscina,
        capacitas_nova);
    si (!buffer_novum) redde FALSUM;

    si (aedificator->buffer && aedificator->offset > ZEPHYRUM)
    {
        memcpy(buffer_novum, aedificator->buffer, aedificator->offset);
    }

    aedificator->buffer     = buffer_novum;
    aedificator->capacitas  = capacitas_nova;

    redde VERUM;
}

interior b32
_appendere_interna (
    ChordaAedificator* aedificator,
          constans i8* datum,
       memoriae_index  mensura)
{
    memoriae_index necessaria;
    memoriae_index locus;
               b32 intra;

    /* Appendix vacua bona est */
    si (!aedificator || !datum || mensura == ZEPHYRUM)
    {
        redde mensura == ZEPHYRUM;
    }

    necessaria = aedificator->offset + mensura;

    si (necessaria > aedificator->capacitas)
    {
        /* APPENDIX SUI (spectatio aedificatoris ipsius): crescentia
         * buffer movere potest (piscina_conari_ampliare via II
         * liberat vetus), ergo fons ut offset servatur et post
         * crescentiam re-derivatur */
        intra = (   aedificator->buffer != NIHIL
                 && datum >= aedificator->buffer
                 && datum < aedificator->buffer
                     + aedificator->capacitas);
        locus = intra ? (memoriae_index)(datum - aedificator->buffer)
                      : ZEPHYRUM;
        si (!_crescere(aedificator, necessaria)) redde FALSUM;
        si (intra)
        {
            datum = aedificator->buffer + locus;
        }
    }

    memcpy(aedificator->buffer + aedificator->offset, datum, mensura);
    aedificator->offset += mensura;

    redde VERUM;
}

/* Formata integrum signatum ad buffer
 * Vocans praebet buffer, debet esse >= 32 bytes (tutus pro s32) */
interior memoriae_index
_format_integer_s32 (
               s32  n,
                i8* buffer,
    memoriae_index  capacitas)
{
            character cstr[CXXXII];
                  s32 mensura_signed;
       memoriae_index mensura;

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        "%d", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

interior memoriae_index
_format_integer_i32 (
               i32  n,
                i8* buffer,
    memoriae_index  capacitas)
{
         character cstr[CXXXII];
               s32 mensura_signed;
    memoriae_index mensura;

    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        "%u", n);
    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

interior memoriae_index
_format_duplex (
               f64  n,
               i32  decimales,
                i8* buffer,
    memoriae_index  capacitas)
{
         character cstr[CXXXII];
         character formatalis[XVI];
               s32 mensura_signed;
    memoriae_index mensura;

    snprintf(formatalis, (memoriae_index)magnitudo(formatalis),
        "%%.%df", decimales);
    mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr),
        formatalis, n);

    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    mensura = (memoriae_index)mensura_signed;
    si (mensura >= capacitas) redde ZEPHYRUM;

    memcpy(buffer, cstr, mensura);
    redde mensura;
}

/* Effugium chordae JSON
 * Tractat: citationem, virgulam inversam, characteres imperantes
 * Vocans praebet buffer */
interior memoriae_index
_evadere_json (
       constans i8* input,
    memoriae_index  mensura,
                i8* output,
    memoriae_index  capacitas_output)
{
          memoriae_index  index_input;
          memoriae_index  index_output;
               character  c;
    insignatus character  u;
      constans character* hex = "0123456789abcdef";

    index_input   = ZEPHYRUM;
    index_output  = ZEPHYRUM;

    dum (index_input < mensura && index_output < capacitas_output - I)
    {
        c = (character)input[index_input];

        si (c == '"')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = '"';
        }
        alioquin si (c == '\\')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = '\\';
        }
        alioquin si (c == '\n')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 'n';
        }
        alioquin si (c == '\r')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 'r';
        }
        alioquin si (c == '\t')
        {
            si (index_output + II > capacitas_output) redde ZEPHYRUM;
            output[index_output++] = '\\';
            output[index_output++] = 't';
        }
        alioquin si (   (insignatus character)c < 0x20
                     || (insignatus character)c == 0x7F)
        {
            /* Alii characteres imperantes ut \u00xx. REGULA JSON per
             * se, non ctype (2026-09-22): iscntrl((signed char)c)
             * erat UB pro octetis >= 0x80 (valor negativus non EOF)
             * et a locale pendebat. DEL (0x7F) effugitur ut olim -
             * scriptura immutata. */
            u = (insignatus character)c;
            si (index_output + VI > capacitas_output)
            {
                redde ZEPHYRUM;
            }
            output[index_output++] = '\\';
            output[index_output++] = 'u';
            output[index_output++] = '0';
            output[index_output++] = '0';
            output[index_output++] = (i8)hex[(u >> IV) & 0xF];
            output[index_output++] = (i8)hex[u & 0xF];
        }
        alioquin
        {
            output[index_output++] = (i8)c;
        }

        index_input++;
    }

    redde index_output;
}


/* ==================================================
 * Creatio
 * ================================================== */

ChordaAedificator*
chorda_aedificator_creare (
           Piscina* piscina,
    memoriae_index  capacitas_initialis)
{
    ChordaAedificator* aedificator;
                   i8* buffer;

    si (!piscina || capacitas_initialis == ZEPHYRUM) redde NIHIL;

    aedificator = (ChordaAedificator*)piscina_allocare(
                                        piscina,
                                        magnitudo(ChordaAedificator));
    si (!aedificator) redde NIHIL;

    buffer = (i8*)piscina_allocare(piscina, capacitas_initialis);
    si (!buffer) redde NIHIL;

    aedificator->buffer             = buffer;
    aedificator->capacitas          = capacitas_initialis;
    aedificator->offset             = ZEPHYRUM;
    aedificator->piscina            = piscina;
    aedificator->indentatio_gradus  = ZEPHYRUM;

    redde aedificator;
}


/* ==================================================
 * Destructio
 * ================================================== */

vacuum
chorda_aedificator_destruere (
    ChordaAedificator* aedificator)
{
    /* Piscina possidet memoriam; solum liberamus structuram */
    si (aedificator)
    {
        /* Nota: buffer etiam allocatus ex piscina,
		 * ergo liberabitur quando piscina destruitur */
    }
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

b32
chorda_aedificator_appendere_character (
    ChordaAedificator* aedificator,
            character  c)
{
    i8 ch = (i8)c;
    redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

b32
chorda_aedificator_appendere_literis (
     ChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index mensura;

    si (!aedificator || !cstr) redde FALSUM;

    mensura = strlen(cstr);
    redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

b32
chorda_aedificator_appendere_chorda (
    ChordaAedificator* aedificator,
               chorda  s)
{
    si (!aedificator || !s.datum) redde FALSUM;

    redde _appendere_interna(aedificator, s.datum, s.mensura);
}


/* ==================================================
 * Appendere - Numeri
 * ================================================== */

b32
chorda_aedificator_appendere_s32 (
    ChordaAedificator* aedificator,
                  s32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_s32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_i32 (
    ChordaAedificator* aedificator,
                  i32  n)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_f64 (
    ChordaAedificator* aedificator,
                  f64  n,
                  i32  decimales)
{
                i8 buffer[CXXXII];
    memoriae_index mensura;

    si (!aedificator || decimales < ZEPHYRUM || decimales > XXX)
    {
        redde FALSUM;
    }

    mensura = _format_duplex(n, decimales, buffer, magnitudo(buffer));
    si (mensura == ZEPHYRUM) redde FALSUM;

    redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_repetita (
     ChordaAedificator* aedificator,
             character  c,
                   i32  numerus)
{
    i32 i;

    /* numerus i32 insignatus - custodia negativi mortua remota
	 * (2026-07-17); familia indentationis non-negativa per push/pop */
    si (!aedificator) redde FALSUM;

    si (numerus == ZEPHYRUM) redde VERUM;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (!chorda_aedificator_appendere_character(aedificator, c))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

b32
chorda_aedificator_appendere_hex_i32 (
    ChordaAedificator* aedificator,
                  i32  n)
{
             character buffer[XVI];
                   s32 mensura_signed;
        memoriae_index mensura;

    si (!aedificator) redde FALSUM;

    mensura_signed = snprintf(buffer, magnitudo(buffer), "%x", n);
    si (mensura_signed < ZEPHYRUM) redde FALSUM;

    mensura = (memoriae_index)mensura_signed;

    redde _appendere_interna(aedificator, (i8*)buffer, mensura);
}


/* ==================================================
 * Appendere - Evasus
 * ================================================== */

b32
chorda_aedificator_appendere_evasus_json (
    ChordaAedificator* aedificator,
               chorda  s)
{
                i8  buffer[D];  /* Buffer in acervo pro effugiis parvis */
                i8* output_buffer;
    memoriae_index  mensura_evasus;
    memoriae_index  necessaria;

    si (!aedificator)
    {
        redde FALSUM;
    }
    /* chorda vacua (datum quodvis, etiam NIHIL - forma domus): nihil
     * addendum, SUCCESSUS. Olim FALSUM: _evadere_json nullos octetos
     * scriptos ut defectum reddebat (2026-09-22). */
    si (s.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (!s.datum)
    {
        redde FALSUM;
    }

    /* Pessimus casus: omnis character fit \uXXXX (6 bytes) */
    necessaria = s.mensura * VI;

    si (necessaria <= magnitudo(buffer))
    {
        output_buffer = buffer;
    }
    alioquin
    {
        output_buffer = (i8*)piscina_allocare(aedificator->piscina,
            necessaria);
        si (!output_buffer) redde FALSUM;
    }

    mensura_evasus = _evadere_json(s.datum, s.mensura, output_buffer,
        necessaria);
    si (mensura_evasus == ZEPHYRUM)
    {
        redde FALSUM;
    }

    redde _appendere_interna(aedificator, output_buffer,
        mensura_evasus);
}

b32
chorda_aedificator_appendere_literis_evasus_json (
     ChordaAedificator* aedificator,
    constans character* cstr)
{
    memoriae_index  mensura;
                i8* buffer_temporalis;
            chorda  s;
               b32  result;

    si (!cstr || !aedificator) redde FALSUM;

    mensura = strlen(cstr);
    /* "" = successus, nihil additum (piscina_allocare(0) NIHIL
     * reddit - olim FALSUM, 2026-09-22) */
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    buffer_temporalis = (i8*)piscina_allocare(aedificator->piscina,
        mensura);
    si (!buffer_temporalis) redde FALSUM;

    memcpy(buffer_temporalis, cstr, mensura);
    s.datum    = buffer_temporalis;
    s.mensura  = (i32)mensura;

    result = chorda_aedificator_appendere_evasus_json(aedificator, s);
    redde result;
}


/* ==================================================
 * Appendere - Spatium Album/Structura
 * ================================================== */

b32
chorda_aedificator_appendere_lineam_novam (
    ChordaAedificator* aedificator)
{
    si (!aedificator) redde FALSUM;

    redde chorda_aedificator_appendere_character(aedificator, '\n');
}

b32
chorda_aedificator_appendere_indentationem (
    ChordaAedificator* aedificator,
                  i32  gradus)
{
    i32 i;
    i32 spatia;

    /* gradus i32 insignatus - custodia negativi mortua remota
	 * (2026-07-17); gradus tractatus <= M per push, pop ad zephyrum
	 * sistit */
    si (!aedificator) redde FALSUM;

    spatia = gradus * CHORDA_AEDIFICATOR_INDENTATIO_SPATIA;

    per (i = ZEPHYRUM; i < spatia; i++)
    {
        si (!chorda_aedificator_appendere_character(aedificator, ' '))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ==================================================
 * Indentationis Observatio
 * ================================================== */

vacuum
chorda_aedificator_push_indentationem (
    ChordaAedificator* aedificator)
{
    si (aedificator && aedificator->indentatio_gradus < M)
    {
        aedificator->indentatio_gradus++;
    }
}

vacuum
chorda_aedificator_pop_indentationem (
    ChordaAedificator* aedificator)
{
    si (aedificator && aedificator->indentatio_gradus > ZEPHYRUM)
    {
        aedificator->indentatio_gradus--;
    }
}

i32
chorda_aedificator_indentatio_gradus (
    ChordaAedificator* aedificator)
{
    redde aedificator ? aedificator->indentatio_gradus : ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

memoriae_index
chorda_aedificator_longitudo (
    ChordaAedificator* aedificator)
{
    redde aedificator ? aedificator->offset : ZEPHYRUM;
}

vacuum
chorda_aedificator_truncare (
    ChordaAedificator* aedificator,
       memoriae_index  longitudo_nova)
{
    si (aedificator == NIHIL)
    {
        redde;
    }
    si (longitudo_nova < aedificator->offset)
    {
        aedificator->offset = longitudo_nova;
    }
}

chorda
chorda_aedificator_spectare (
    ChordaAedificator* aedificator)
{
    chorda result;

    si (!aedificator || !aedificator->buffer)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
    }
    alioquin
    {
        result.mensura  = (i32)aedificator->offset;
        result.datum    = aedificator->buffer;
    }

    redde result;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
chorda_aedificator_reset (
    ChordaAedificator* aedificator)
{
    si (!aedificator) redde;

    aedificator->offset             = ZEPHYRUM;
    aedificator->indentatio_gradus  = ZEPHYRUM;
}

chorda
chorda_aedificator_finire (
    ChordaAedificator* aedificator)
{
    chorda result;

    si (!aedificator)
    {
        result.mensura  = ZEPHYRUM;
        result.datum    = NIHIL;
        redde result;
    }

    result.mensura  = (i32)aedificator->offset;
    result.datum    = aedificator->buffer;

    redde result;
}
#undef _appendere_interna
#undef _crescere
#undef _evadere_json
#undef _format_duplex
#undef _format_integer_i32
#undef _format_integer_s32
#undef _proxima_capacitas
/* lib/xar.c: statica per plagulam renominata */
#define _bitum_summum _bitum_summum_xar
#define _creare _creare_xar
#define _cursus_fundere _cursus_fundere_xar
#define _elementa_permutare _elementa_permutare_xar
#define _ordinare_inserendo _ordinare_inserendo_xar
#define _refusio_segmentorum _refusio_segmentorum_xar
#define _segmentum _segmentum_xar
#define _tabulam_crescere _tabulam_crescere_xar
#define allocare_segmentum allocare_segmentum_xar
#define computare_magnitudinem_segmenti computare_magnitudinem_segmenti_xar
#define xar_locare xar_locare_xar
#define XarLocatio XarLocatio_xar
#line 1 "lib/xar.c"
/* <portabile/> probationibus sub glibc probata 2026-08-03 */
/* xar.c - "Codex Tabulae Exponentialis" - Implementatio
 *
 * Tabula crescens sine reallocatio
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* ========================================================================
 * STRUCTURAE INTERNAE
 * ======================================================================== */

/* XarLocatio - Locatio elementi in structura segmentata (interior)
 */
nomen structura XarLocatio {
       i32  index_segmenti;         /* Quis segmentum? */
       i32  offset_in_segmento;     /* Ubi in segmento? */
       i32  magnitudo_segmenti;     /* Quam magnus? */
    vacuum* basis_segmenti;         /* Initium segmenti */
} XarLocatio;


/* ========================================================================
 * CONSTANTAE INTERNAE
 * ======================================================================== */

/* Factor duplicandi pro crescentia segmentorum */
#define XAR_FACTOR_DUPLICANDI  II


/* ========================================================================
 * FUNCTIONES AUXILIARES
 * ======================================================================== */

/* Computare Magnitudinem Segmenti
 * "Computare magnitudinem segmenti usans formam conservatam"
 *
 * Notitia clavis: duo prima segmenta eadem magnitudo, deinde duplicatio
 * "Regula duplicandi"
 */
interior i32
computare_magnitudinem_segmenti (
    constans Xar* xar,
             i32  index_segmenti)
{
    i32 shift_amount;

    si (index_segmenti <= I)
    {
        /* Duo prima segmenta: eadem magnitudo */
        redde xar->magnitudo_primi;
    }

    shift_amount = index_segmenti - I;

    /* Impossibilis cum indices i32 validi
	 * Cum magnitudo_primi = XVI et indices i32 (max ~2^31),
	 * numquam opus est plus quam ~27-30 segmenta.
	 * Si hic pervenis, corruptio structurae vel error gravis.
	 */
    si (shift_amount >= XXX)
    {
        imprimere("FRACTA: xar segmentum nimis altum: %d\n",
            index_segmenti);
        imprimere("        (impossibilis cum indices i32 - corruptio?)\n");
        exire(I);
    }

    /* Segmenta sequentia: duplicant */
    redde xar->magnitudo_primi << shift_amount;
}

/* Refusio nominata: tabula segmentorum exhausta (RP 7.obj.1 - finis
 * nominatus, non truncatio tacita). xar_addere NIHIL reddit. */
interior vacuum
_refusio_segmentorum (
    constans Xar* xar)
{
    fprintf(stderr, "xar: tabula segmentorum exhausta (%u segmenta,"
        " %u elementa) - xar_creare_magnum pro tabula LXIV\n",
        xar->segmenta_maxima, xar->numerus_elementorum);
}

/* Segmentum k legere: NIHIL ultra tabulam currentem (segmenta duo
 * prima inscripta; tabula plena pigre - TABULA CRESCENS, xar.h) */
interior vacuum*
_segmentum (
    constans Xar* xar,
             i32  k)
{
    redde k < xar->mensura_tabulae ? xar->segmenta[k] : NIHIL;
}

/* Tabulam plenam (segmenta_maxima) ex piscina facere, segmentis
 * inscriptis translatis - semel, cum segmentum ultra INLINEA petitur.
 * FALSUM = allocatio fracta (fines ipsos vocans iam probavit). */
interior b32
_tabulam_crescere (
    Xar* xar)
{
      vacuum** tabula;
         i32   i;

    tabula = (vacuum**)piscina_allocare_ordinatum(xar->piscina,
        (memoriae_index)xar->segmenta_maxima * magnitudo(vacuum*),
        VIII);
    si (!tabula)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar->segmenta_maxima; i++)
    {
        tabula[i] = i < xar->mensura_tabulae ? xar->segmenta[i] : NIHIL;
    }
    xar->segmenta         = tabula;
    xar->mensura_tabulae  = xar->segmenta_maxima;
    redde VERUM;
}

/* Allocare Segmentum
 * "Partem allocare"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
interior b32
allocare_segmentum (
    Xar* xar,
    i32  index_segmenti)
{
               i32  magnitudo_segmenti;
    memoriae_index  magnitudo_memoriae;
            vacuum* memoria;

    si (index_segmenti >= xar->segmenta_maxima)
    {
        _refusio_segmentorum(xar);
        redde FALSUM;  /* Nimis multa segmenta! */
    }

    si (   index_segmenti >= xar->mensura_tabulae
        && !_tabulam_crescere(xar))
    {
        redde FALSUM;  /* Allocatio tabulae fracta */
    }

    si (xar->segmenta[index_segmenti])
    {
        redde VERUM;  /* Iam allocatus */
    }

    /* Computare magnitudinem segmenti */
    magnitudo_segmenti = computare_magnitudinem_segmenti(xar,
        index_segmenti);
    magnitudo_memoriae =
        (memoriae_index)magnitudo_segmenti * xar->magnitudo_elementi;

    /* Allocare ex piscina */
    memoria = piscina_allocare(xar->piscina, magnitudo_memoriae);
    si (!memoria)
    {
        redde FALSUM;  /* Allocatio fracta */
    }

    /* Zephyrum memoriam si vexillum non ponit */
    si (!(xar->vexilla & XAR_VEXILLUM_SINE_ZEPHYRUM))
    {
        memset(memoria, ZEPHYRUM, magnitudo_memoriae);
    }

    /* Renovare statum xar */
    xar->segmenta[index_segmenti] = memoria;
    si (index_segmenti >= xar->numerus_segmentorum)
    {
        xar->numerus_segmentorum = index_segmenti + I;
    }
    /* Incrementaliter renovare capacitatem - O(1) non O(n) */
    xar->capacitas_totalis += magnitudo_segmenti;

    redde VERUM;
}

/* Genesis: caput solum, segmenta duo prima in eo inscripta; tabula
 * plena (segmenta_maxima, fines) pigre ex piscina - TABULA CRESCENS. */
interior Xar*
_creare (
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla,
        i32  segmenta_maxima)
{
               Xar* xar;
    memoriae_index  mensura;
               i32  i;

    si (!piscina || magnitudo_elementi == ZEPHYRUM)
    {
        redde NIHIL;
    }
    mensura  = magnitudo(Xar);
    xar      = (Xar*)piscina_allocare_ordinatum(piscina, mensura, VIII);
    si (!xar)
    {
        redde NIHIL;
    }
    xar->numerus_elementorum  = ZEPHYRUM;
    xar->magnitudo_elementi   = magnitudo_elementi;
    xar->magnitudo_primi     = magnitudo_primi ? magnitudo_primi
                                               : XAR_PRIMUS_SEGMENTUM;
    xar->numerus_segmentorum  = ZEPHYRUM;
    xar->capacitas_totalis    = ZEPHYRUM;
    xar->vexilla              = vexilla;
    xar->segmenta_maxima      = segmenta_maxima;
    xar->mensura_tabulae      = XAR_SEGMENTA_INLINEA;
    xar->piscina              = piscina;
    xar->segmenta             = xar->segmenta_inlinea;
    per (i = ZEPHYRUM; i < XAR_SEGMENTA_INLINEA; i++)
    {
        xar->segmenta_inlinea[i] = NIHIL;
    }
    redde xar;
}


/* ========================================================================
 * FUNCTIONES CREATIONIS
 * ======================================================================== */

/* Xar Creare */
Xar*
xar_creare (
    Piscina* piscina,
        i32  magnitudo_elementi)
{
    redde xar_creare_cum_vexillis(piscina,
                                   magnitudo_elementi,
                                   XAR_PRIMUS_SEGMENTUM,
                                   XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Magnitudine */
Xar*
xar_creare_cum_magnitudine (
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi)
{
    redde xar_creare_cum_vexillis(piscina,
                                   magnitudo_elementi,
                                   magnitudo_primi,
                                   XAR_VEXILLUM_ORDINARIUS);
}

/* Xar Creare Cum Vexillis
 * "Genesis tabulae exponentialis - cum omnibus optionibus"
 */
Xar*
xar_creare_cum_vexillis (
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla)
{
    redde _creare(piscina, magnitudo_elementi, magnitudo_primi, vexilla,
                  XAR_SEGMENTA_ORDINARIA);
}

/* Xar Creare Magnum - tabula LXIV */
Xar*
xar_creare_magnum (
    Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi)
{
    redde _creare(piscina, magnitudo_elementi, magnitudo_primi,
                  XAR_VEXILLUM_ORDINARIUS, XAR_MAXIMUS_SEGMENTORUM);
}

/* Xar Destruere */
vacuum
xar_destruere (
    Xar* xar)
{
    /* Cum piscina, nihil agendum!
	 */
    (vacuum)xar;
}

/* Xar Ponere Vexilla */
vacuum
xar_ponere_vexilla (
    Xar* xar,
    i32  vexilla)
{
    si (xar)
    {
        xar->vexilla = vexilla;
    }
}

/* Bitum summum per instructionem UNAM (clz) ubi compilator eam praebet,
 * aliter scansio C89 infra (eadem responsa; q > 0 semper). xar_mensura
 * 2026-10-15: scansio ~XV instructiones in omni xar_obtinere, catena
 * dependens - clz seriatim 3.0 -> 2.2 ns, temere 2.6 -> 1.5 ns.
 * -DXAR_SINE_INTRINSECIS viam C89 cogit (probatio eius in clang). */
#if (defined(__clang__) || defined(__GNUC__)) \
    && !defined(XAR_SINE_INTRINSECIS)
#define XAR_BITUM_SUMMUM(q) \
    ((i32)(XXXI - (i32)__builtin_clz((insignatus integer)(q))))
#else
/* Bitum summum positum (0-basatum) numeri positivi: scansio binaria
 * quinque graduum (C89 sine intrinsecis). Pro xar_locare: segmentum k
 * >= II indices [primum x 2^(k-1), primum x 2^k) tenet, ergo cum
 * q = index / primum, k = bitum_summum(q) + I - tempore constanti,
 * non gyro duplicandi (XVI% foliorum profili in lib/stml.c, 2026-09-02). */
interior i32
_bitum_summum (
    i32 q)
{
    i32 k = ZEPHYRUM;

    si (q >> XVI)
    { k += XVI; q >>= XVI;
    }
    si (q >> VIII)
    { k += VIII; q >>= VIII;
    }
    si (q >> IV)
    { k += IV;   q >>= IV;
    }
    si (q >> II)
    { k += II;   q >>= II;
    }
    si (q >> I)
    { k += I;
    }
    redde k;
}

#define XAR_BITUM_SUMMUM(q) _bitum_summum(q)
#endif


/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Locare - Algorithmus O(1)! (interior)
 * "Invenire locum indicis"
 *
 * ALGORITHMUS:
 * 1. Via rapida pro primis duobus segmentis (computatio directa)
 * 2. Quaestio exponentia pro ceteris (sine circulatio per omnia segmenta)
 */
interior b32
xar_locare (
    constans     Xar* xar,
                 i32  index,
          XarLocatio* locatio)
{
    i32 index_segmenti;
    i32 magnitudo_segmenti;
    i32 index_adiustus;

    si (!xar || !locatio)
    {
        redde FALSUM;
    }

    /* Segmentum primum: via rapida */
    si (index < xar->magnitudo_primi)
    {
        locatio->index_segmenti      = ZEPHYRUM;
        locatio->offset_in_segmento  = index;
        locatio->magnitudo_segmenti  = xar->magnitudo_primi;
        locatio->basis_segmenti      = xar->segmenta[ZEPHYRUM];
        redde VERUM;
    }

    /* PRIMUM POTENTIA BINARIA (ordinarium: IV, et omnes vocantes domus
     * I/IV/VIII/XVI): primum = 2^p, h = bitum_summum(index) ->
     * segmentum h - p + I, magnitudo 2^h, offset index - 2^h - sine
     * divisione (segmentum I quoque: h = p). Primum aliud (probatio
     * finium III, VII) per formam divisionis infra - mores exacti. */
    si ((xar->magnitudo_primi & (xar->magnitudo_primi - I)) == ZEPHYRUM)
    {
        i32 h = XAR_BITUM_SUMMUM(index);

        index_segmenti      = h - XAR_BITUM_SUMMUM(xar->magnitudo_primi)
                            + I;
        magnitudo_segmenti  = (i32)I << h;
        index_adiustus      = index - magnitudo_segmenti;
        si (index_segmenti >= xar->segmenta_maxima)
        {
            redde FALSUM;
        }
        locatio->index_segmenti      = index_segmenti;
        locatio->offset_in_segmento  = index_adiustus;
        locatio->magnitudo_segmenti  = magnitudo_segmenti;
        locatio->basis_segmenti      = _segmentum(xar, index_segmenti);
        redde VERUM;
    }

    /* Computatio directa pro segmentis exponentialibus
	 *
	 * Usans manipulationem bitorum pro quaestio exponentia
	 */
    /* Forma clausa (2026-09-02, olim gyrus duplicandi): q = index /
     * primum; segmentum = bitum_summum(q) + I; magnitudo = primum <<
     * bitum; offset = index - magnitudo. Eaedem fines ac gyrus (probatio
     * xar: numerus segmentorum et capacitas per fines contra
     * referentiam). */
    {
        i32 q = index / xar->magnitudo_primi;
        i32 b = XAR_BITUM_SUMMUM(q);

        index_segmenti      = b + I;
        magnitudo_segmenti  = xar->magnitudo_primi << b;
        index_adiustus      = index - magnitudo_segmenti;
    }

    si (index_segmenti >= xar->segmenta_maxima)
    {
        redde FALSUM;  /* Index nimis magnus (vocator additionis clamat) */
    }

    locatio->index_segmenti      = index_segmenti;
    locatio->offset_in_segmento  = index_adiustus;
    locatio->magnitudo_segmenti  = magnitudo_segmenti;
    locatio->basis_segmenti      = _segmentum(xar, index_segmenti);

    redde VERUM;
}

/* Xar Obtinere
 * "Obtinere elementum"
 */
vacuum*
xar_obtinere (
    constans Xar* xar,
             i32  index)
{
    XarLocatio  locatio;
            i8* basis;

    si (!xar || index >= xar->numerus_elementorum)
    {
        redde NIHIL;
    }

    si (!xar_locare(xar, index, &locatio))
    {
        redde NIHIL;
    }

    si (!locatio.basis_segmenti)
    {
        redde NIHIL;  /* Segmentum non allocatus */
    }

    /* Computare locum elementi
	 */
    basis = (i8*)locatio.basis_segmenti;
    redde basis
        + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Obtinere Signatum
 * "Pro iterationibus cum decrementis"
 */
vacuum*
xar_obtinere_s (
    constans Xar* xar,
             s32  index)
{
    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde xar_obtinere(xar, (i32)index);
}

/* Xar Obtinere Vel Creare
 * "Utile pro tabulis dispersis"
 */
vacuum*
xar_obtinere_vel_creare (
    Xar* xar,
    i32  index)
{
    XarLocatio  locatio;
            i8* basis;
           i32  i;

    si (!xar)
    {
        redde NIHIL;
    }

    /* Invenire locatio */
    si (!xar_locare(xar, index, &locatio))
    {
        redde NIHIL;
    }

    /* Assecurare omnia segmenta usque ad hoc existunt
	 */
    per (i = ZEPHYRUM; i <= locatio.index_segmenti; i++)
    {
        si (!_segmentum(xar, i))
        {
            si (!allocare_segmentum(xar, i))
            {
                redde NIHIL;
            }
        }
    }

    /* Renovare numerum si necessarium */
    si (index >= xar->numerus_elementorum)
    {
        xar->numerus_elementorum = index + I;
    }

    /* Reddere indicem elementi */
    basis = (i8*)xar->segmenta[locatio.index_segmenti];
    redde basis
        + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Ponere */
b32
xar_ponere (
                Xar* xar,
                i32  index,
    constans vacuum* elementum)
{
    vacuum* destinatio;

    destinatio = xar_obtinere_vel_creare(xar, index);
    si (!destinatio)
    {
        redde FALSUM;
    }

    memcpy(destinatio, elementum, xar->magnitudo_elementi);
    redde VERUM;
}


/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere
 * "Addere ad finem"
 */
vacuum*
xar_addere (
    Xar* xar)
{
    XarLocatio  locatio;
           i32  index_novus;
            i8* basis;

    si (!xar)
    {
        redde NIHIL;
    }

    index_novus = xar->numerus_elementorum;

    /* Invenire ubi elementum novum ibit */
    si (!xar_locare(xar, index_novus, &locatio))
    {
        _refusio_segmentorum(xar);
        redde NIHIL;
    }

    /* Assecurare segmentum existit */
    si (!_segmentum(xar, locatio.index_segmenti))
    {
        si (!allocare_segmentum(xar, locatio.index_segmenti))
        {
            redde NIHIL;  /* Allocatio fracta */
        }
        /* Renovare locationem cum segmento novo */
        locatio.basis_segmenti = xar->segmenta[locatio.index_segmenti];
    }

    /* Incrementare numerum */
    xar->numerus_elementorum++;

    /* Reddere indicem ad elementum novum */
    basis = (i8*)locatio.basis_segmenti;
    redde basis
        + (locatio.offset_in_segmento * xar->magnitudo_elementi);
}

/* Xar Addere Multos
 * "Addere elementa multa - optimizatum pro operationibus magnis"
 */
i32
xar_addere_multos (
                Xar* xar,
    constans vacuum* elementa,
                i32  numerus)
{
    constans i8* src;
            i32  index_initialis;
            i32  index_finalis;
     XarLocatio  locatio;
            i32  i, j;
            i32  elementa_copiata;
            i32  residuum_segmenti;
             i8* destinatio_ptr;
            i32  ad_copiam;

    si (!xar || !elementa || numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    src              = (constans i8*)elementa;
    index_initialis  = xar->numerus_elementorum;
    index_finalis    = index_initialis + numerus - I;

    /* Praeallocare omnia segmenta necessaria
	 */
    si (!xar_locare(xar, index_finalis, &locatio))
    {
        _refusio_segmentorum(xar);
        redde ZEPHYRUM;  /* Index nimis magnus */
    }

    /* Assecurare omnia segmenta ab initio usque ad finem allocata sunt */
    per (i = ZEPHYRUM; i <= locatio.index_segmenti; i++)
    {
        si (!_segmentum(xar, i))
        {
            si (!allocare_segmentum(xar, i))
            {
                i32 index_ultimus;

                /* Allocatio partialis - renovare numerum et reddere quantum habuimus */
                si (i == ZEPHYRUM) redde ZEPHYRUM;

                /* Computare quot elementa re vera addere possumus */
                index_ultimus = ZEPHYRUM;
                per (j = ZEPHYRUM; j < i; j++)
                {
                    index_ultimus += computare_magnitudinem_segmenti(xar,
                        j);
                }
                si (index_ultimus > index_initialis)
                {
                    numerus = index_ultimus - index_initialis;
                }
                alioquin
                {
                    redde ZEPHYRUM;
                }
                frange;
            }
        }
    }

    /* Renovare numerum primum */
    xar->numerus_elementorum = index_initialis + numerus;

    /* Nunc facere copias efficientes per segmentum
	 */
    xar_locare(xar, index_initialis, &locatio);
    i                 = ZEPHYRUM;
    elementa_copiata  = ZEPHYRUM;

    dum (i < numerus)
    {
        /* Computare quot elementa in segmento currenti cabent */
        residuum_segmenti = locatio.magnitudo_segmenti
            - locatio.offset_in_segmento;
        ad_copiam = (numerus - i < residuum_segmenti) ? (numerus
            - i) : residuum_segmenti;

        /* Obtinere indicem destinationis */
        destinatio_ptr = (i8*)xar->segmenta[locatio.index_segmenti];
        destinatio_ptr += locatio.offset_in_segmento * xar->magnitudo_elementi;

        /* Copiare blocum elementorum */
        memcpy(destinatio_ptr, src + (i * xar->magnitudo_elementi),
               ad_copiam * xar->magnitudo_elementi);

        i                 += ad_copiam;
        elementa_copiata  += ad_copiam;

        /* Movere ad segmentum proximum si necessarium */
        si (i < numerus)
        {
            locatio.index_segmenti++;
            locatio.offset_in_segmento = ZEPHYRUM;
            locatio.magnitudo_segmenti =
                computare_magnitudinem_segmenti(
                                            xar,
                                            locatio.index_segmenti);
        }
    }

    redde elementa_copiata;
}


/* ========================================================================
 * ITERATIO
 * ======================================================================== */

/* Xar Iterator Initium
 * "Implementatio iteratoris"
 */
XarIterator
xar_iterator_initium (
    constans Xar* xar)
{
    XarIterator iter = {ZEPHYRUM};

    iter.xar                 = xar;
    iter.index_currens       = ZEPHYRUM;
    iter.index_segmenti      = ZEPHYRUM;
    iter.offset_in_segmento  = ZEPHYRUM;

    si (xar && xar->numerus_segmentorum > ZEPHYRUM)
    {
        iter.basis_segmenti = xar->segmenta[ZEPHYRUM];
        iter.finis_segmenti = computare_magnitudinem_segmenti(xar,
            ZEPHYRUM);
    }

    redde iter;
}

/* Xar Iterator Proximum
 * "Obtinere elementum proximum"
 */
vacuum*
xar_iterator_proximum (
    XarIterator* iter)
{
    vacuum* elementum;
        i8* basis;

    si (   !iter || !iter->xar
        || iter->index_currens >= iter->xar->numerus_elementorum)
    {
        redde NIHIL;
    }

    /* Verificare si oportet movere ad segmentum proximum */
    si (iter->offset_in_segmento >= iter->finis_segmenti)
    {
        iter->index_segmenti++;
        iter->offset_in_segmento = ZEPHYRUM;

        si (iter->index_segmenti < iter->xar->numerus_segmentorum)
        {
            iter->basis_segmenti =
                iter->xar->segmenta[iter->index_segmenti];
            iter->finis_segmenti = computare_magnitudinem_segmenti(
                                        iter->xar,
                                        iter->index_segmenti);
        }
    }

    /* Obtinere elementum currens */
    basis = (i8*)iter->basis_segmenti;
    elementum = basis
        + (iter->offset_in_segmento * iter->xar->magnitudo_elementi);

    /* Promovere iteratorem */
    iter->index_currens++;
    iter->offset_in_segmento++;

    redde elementum;
}

/* Xar Iterator Finis */
b32
xar_iterator_finis (
    XarIterator* iter)
{
    redde !iter || !iter->xar
        || iter->index_currens >= iter->xar->numerus_elementorum;
}


/* ========================================================================
 * QUAESTIO
 * ======================================================================== */

/* Xar Invenire Index
 * "Quaestio linearis - reddere indicem"
 */
s32
xar_invenire_index (
       constans         Xar* xar,
       constans      vacuum* clavis,
              XarComparator  comparator)
{
       i32  i;
    vacuum* elementum;

    per (i = ZEPHYRUM; i < xar->numerus_elementorum; i++)
    {
        elementum = xar_obtinere(xar, i);
        si (elementum && comparator(clavis, elementum) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }

    redde -I;  /* Non inventus */
}

/* Xar Invenire
 * "Quaestio linearis - reddere indicem ad elementum"
 */
vacuum*
xar_invenire (
       constans         Xar* xar,
       constans      vacuum* clavis,
              XarComparator  comparator)
{
    s32 index;

    index = xar_invenire_index(xar, clavis, comparator);
    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }

    redde xar_obtinere(xar, (i32)index);
}

/* Xar Quaerere Binarie
 * "Quaestio binaria - assumit tabulam ordinatam"
 */
vacuum*
xar_quaerere_binarie (
       constans         Xar* xar,
       constans      vacuum* clavis,
              XarComparator  comparator)
{
        i32  sinister;
        i32  dexter;
        i32  medius;
     vacuum* elementum;
        s32  cmp;

    sinister  = ZEPHYRUM;
    dexter    = xar->numerus_elementorum - I;

    dum (sinister <= dexter)
    {
        medius = sinister + (dexter - sinister) / II;

        elementum = xar_obtinere(xar, medius);
        si (!elementum)
        {
            redde NIHIL;
        }

        cmp = comparator(clavis, elementum);

        si (cmp == ZEPHYRUM)
        {
            redde elementum;  /* Inventus! */
        }
        alioquin si (cmp < ZEPHYRUM)
        {
            si (medius == ZEPHYRUM) frange;
            dexter = medius - I;
        }
        alioquin
        {
            sinister = medius + I;
        }
    }

    redde NIHIL;  /* Non inventus */
}

/* Xar Quaerere Binarie Index
 * "Quaestio binaria - reddere indicem"
 */
s32
xar_quaerere_binarie_index (
       constans         Xar* xar,
       constans      vacuum* clavis,
              XarComparator  comparator)
{
        i32  sinister;
        i32  dexter;
        i32  medius;
     vacuum* elementum;
        s32  cmp;

    sinister  = ZEPHYRUM;
    dexter    = xar->numerus_elementorum - I;

    dum (sinister <= dexter)
    {
        medius = sinister + (dexter - sinister) / II;

        elementum = xar_obtinere(xar, medius);
        si (!elementum)
        {
            redde -I;
        }

        cmp = comparator(clavis, elementum);

        si (cmp == ZEPHYRUM)
        {
            redde (s32)medius;  /* Inventus! */
        }
        alioquin si (cmp < ZEPHYRUM)
        {
            si (medius == ZEPHYRUM) frange;
            dexter = medius - I;
        }
        alioquin
        {
            sinister = medius + I;
        }
    }

    redde -I;  /* Non inventus */
}


/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus */
i32
xar_numerus (
    constans Xar* xar)
{
    redde xar ? xar->numerus_elementorum : ZEPHYRUM;
}

/* Xar Capacitas */
i32
xar_capacitas (
    constans Xar* xar)
{
    redde xar ? xar->capacitas_totalis : ZEPHYRUM;
}

/* Xar Vacuum Est */
b32
xar_vacuum_est (
    constans Xar* xar)
{
    redde !xar || xar->numerus_elementorum == ZEPHYRUM;
}

/* Xar Vacare */
vacuum
xar_vacare (
    Xar* xar)
{
    si (xar)
    {
        xar->numerus_elementorum = ZEPHYRUM;
        /* Nota: segmenta manent allocata cum piscina */
    }
}

/* Xar Truncare */
vacuum
xar_truncare (
    Xar* xar,
    i32  numerus_novus)
{
    si (xar && numerus_novus < xar->numerus_elementorum)
    {
        xar->numerus_elementorum = numerus_novus;
    }
}

/* Xar Copiare Ad Tabulam */
i32
xar_copiare_ad_tabulam (
    constans Xar* xar,
          vacuum* destinatio,
             i32  initium,
             i32  numerus)
{
        i8* destinatio_bytes;
       i32  i;
    vacuum* src;

    destinatio_bytes = (i8*)destinatio;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        src = xar_obtinere(xar, initium + i);
        si (!src)
        {
            redde i;  /* Reddere numerum copiatum cum successu */
        }
        memcpy(
            destinatio_bytes + (i * xar->magnitudo_elementi),
            src,
            xar->magnitudo_elementi);
    }

    redde numerus;
}

/* Xar Status Imprimere
 * "Imprimere statisticas pro depuratione"
 */
vacuum
xar_status_imprimere (
    constans Xar* xar)
{
    i32 i;
    i32 mensura;
    i32 usus;
    i32 allocatus;

    si (!xar)
    {
        imprimere("Xar: NIHIL\n");
        redde;
    }

    imprimere("Xar Status (tabula %u segmentorum):\n",
        xar->segmenta_maxima);
    imprimere("  Numerus elementorum: %u\n", xar->numerus_elementorum);
    imprimere("  Magnitudo elementi: %u bytes\n",
        xar->magnitudo_elementi);
    imprimere("  Capacitas totalis: %u\n", xar->capacitas_totalis);
    imprimere("  Numerus segmentorum: %u\n", xar->numerus_segmentorum);

    imprimere("\n  Segmenta:\n");
    allocatus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar->numerus_segmentorum; i++)
    {
        mensura = computare_magnitudinem_segmenti(xar, i);
        imprimere("    [%u]: %u elementa (%u bytes) - %s\n",
                  i, mensura, mensura * xar->magnitudo_elementi,
                  xar->segmenta[i] ? "ALLOCATUS" : "NULLUS");

        si (xar->segmenta[i])
        {
            allocatus += mensura;
        }
    }

    /* Computare usum */
    usus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar->numerus_segmentorum
        && usus < xar->numerus_elementorum; i++)
    {
        mensura = computare_magnitudinem_segmenti(xar, i);
        usus += (xar->numerus_elementorum - usus > mensura) ?
                mensura : (xar->numerus_elementorum - usus);
    }

    imprimere("\n  Usus memoriae:\n");
    imprimere("    Allocatus: %u elementa (%u bytes)\n",
              allocatus,
              allocatus * xar->magnitudo_elementi);
    imprimere("    Usus: %u elementa (%u bytes)\n",
              usus,
              usus * xar->magnitudo_elementi);
    imprimere("    Utilitas: %.1f%%\n",
              allocatus > ZEPHYRUM ? (100.0 * usus / allocatus) : 0.0);
}


/* ========================================================================
 * REMOTIO
 * ======================================================================== */

/* Xar Removere Ultimum
 * "Removere ultimum - O(1)"
 */
b32
xar_removere_ultimum (
    Xar* xar)
{
    si (!xar || xar->numerus_elementorum == ZEPHYRUM)
    {
        redde FALSUM;
    }

    xar->numerus_elementorum--;
    redde VERUM;
}

/* Xar Removere Cum Ultimo
 * "Swap-remove - O(1)"
 */
b32
xar_removere_cum_ultimo (
    Xar* xar,
    i32  index)
{
    vacuum* elementum_ad_remotionem;
    vacuum* elementum_ultimum;
       i32  index_ultimus;

    si (!xar || xar->numerus_elementorum == ZEPHYRUM)
    {
        redde FALSUM;
    }

    si (index >= xar->numerus_elementorum)
    {
        redde FALSUM;
    }

    index_ultimus = xar->numerus_elementorum - I;

    /* Si index est ultimus, simpliciter removere */
    si (index == index_ultimus)
    {
        xar->numerus_elementorum--;
        redde VERUM;
    }

    /* Mutare cum ultimo et removere ultimum */
    elementum_ad_remotionem  = xar_obtinere(xar, index);
    elementum_ultimum        = xar_obtinere(xar, index_ultimus);

    si (!elementum_ad_remotionem || !elementum_ultimum)
    {
        redde FALSUM;
    }

    /* Copiare ultimum ad positionem remotionis */
    memcpy(elementum_ad_remotionem, elementum_ultimum,
        xar->magnitudo_elementi);

    /* Decrementare numerum */
    xar->numerus_elementorum--;

    redde VERUM;
}

/* Xar Tollere
 * "Pop ultimum elementum"
 */
b32
xar_tollere (
        Xar* xar,
     vacuum* destinatio)
{
    vacuum* ultimum;
       i32  index_ultimus;

    si (!xar || !destinatio || xar->numerus_elementorum == ZEPHYRUM)
    {
        redde FALSUM;
    }

    index_ultimus  = xar->numerus_elementorum - I;
    ultimum        = xar_obtinere(xar, index_ultimus);

    si (!ultimum)
    {
        redde FALSUM;
    }

    /* Copiare ad destinationem */
    memcpy(destinatio, ultimum, xar->magnitudo_elementi);

    /* Removere ultimum */
    xar->numerus_elementorum--;

    redde VERUM;
}


/* ========================================================================
 * ORDINATIO ET MANIPULATIO
 * ======================================================================== */

/* Xar Mutare
 * "Mutare duo elementa"
 */
b32
xar_mutare (
    Xar* xar,
    i32  index_a,
    i32  index_b)
{
    vacuum* elem_a;
    vacuum* elem_b;
        i8  temporalis[CCLVI];  /* Buffer temporalis pro swap */
        i8* temp_heap;

    si (!xar)
    {
        redde FALSUM;
    }

    si (   index_a >= xar->numerus_elementorum

        || index_b >= xar->numerus_elementorum)
    {
        redde FALSUM;
    }

    /* Si idem index, nihil agendum */
    si (index_a == index_b)
    {
        redde VERUM;
    }

    elem_a = xar_obtinere(xar, index_a);
    elem_b = xar_obtinere(xar, index_b);

    si (!elem_a || !elem_b)
    {
        redde FALSUM;
    }

    /* Mutare usans buffer temporalem */
    si (xar->magnitudo_elementi <= CCLVI)
    {
        /* Usare buffer in stack */
        memcpy(temporalis, elem_a, xar->magnitudo_elementi);
        memcpy(elem_a, elem_b, xar->magnitudo_elementi);
        memcpy(elem_b, temporalis, xar->magnitudo_elementi);
    }
    alioquin
    {
        /* Allocare in heap pro elementis magnis */
        temp_heap = (i8*)piscina_allocare(xar->piscina,
            xar->magnitudo_elementi);
        si (!temp_heap)
        {
            redde FALSUM;
        }
        memcpy(temp_heap, elem_a, xar->magnitudo_elementi);
        memcpy(elem_a, elem_b, xar->magnitudo_elementi);
        memcpy(elem_b, temp_heap, xar->magnitudo_elementi);
        /* Nota: temp_heap liberabitur cum piscina */
    }

    redde VERUM;
}

/* Xar Invertere
 * "Invertere ordinem in loco"
 */
vacuum
xar_invertere (
    Xar* xar)
{
    i32 sinister;
    i32 dexter;

    si (!xar || xar->numerus_elementorum <= I)
    {
        redde;
    }

    sinister  = ZEPHYRUM;
    dexter    = xar->numerus_elementorum - I;

    dum (sinister < dexter)
    {
        xar_mutare(xar, sinister, dexter);
        sinister++;
        dexter--;
    }
}

/* Elementa duo permutare per buffer acervi, frustis CCLVI
 * octetorum: nulla allocatio, quaevis magnitudo elementi */
interior vacuum
_elementa_permutare (
                i8* a,
                i8* b,
    memoriae_index  mensura)
{
                i8 temporalis[CCLVI];
    memoriae_index frustum;

    dum (mensura > ZEPHYRUM)
    {
        frustum = (mensura < CCLVI) ? mensura : CCLVI;
        memcpy(temporalis, a, frustum);
        memcpy(a, b, frustum);
        memcpy(b, temporalis, frustum);
        a        += frustum;
        b        += frustum;
        mensura  -= frustum;
    }
}

/* Fusio cursuum [initium, medium) et [medium, finis) fontis in
 * destinationem. STABILIS: dexter praecedit solum si STRICTE minor */
interior vacuum
_cursus_fundere (
     constans i8* fons,
              i8* destinatio,
             i32  initium,
             i32  medium,
             i32  finis,
  memoriae_index  mensura,
   XarComparator  comparator)
{
    i32 sinister;
    i32 dexter;
    i32 k;

    sinister  = initium;
    dexter    = medium;
    per (k = initium; k < finis; k++)
    {
        si (   dexter < finis
            && (   sinister >= medium
                || comparator(fons + (memoriae_index)dexter * mensura,
                       fons + (memoriae_index)sinister * mensura)
                   < ZEPHYRUM))
        {
            memcpy(destinatio + (memoriae_index)k * mensura,
                fons + (memoriae_index)dexter * mensura, mensura);
            dexter++;
        }
        alioquin
        {
            memcpy(destinatio + (memoriae_index)k * mensura,
                fons + (memoriae_index)sinister * mensura, mensura);
            sinister++;
        }
    }
}

/* Insertio stabilis in loco - via sine scriptura (piscina certae
 * magnitudinis plena): O(n^2), nulla allocatio */
interior vacuum
_ordinare_inserendo (
              Xar* xar,
    XarComparator  comparator)
{
    i32 i;
    i32 j;

    per (i = I; i < xar->numerus_elementorum; i++)
    {
        per (j = i;
             j > ZEPHYRUM
                 && comparator(xar_obtinere(xar, j),
                        xar_obtinere(xar, j - I)) < ZEPHYRUM;
             j--)
        {
            _elementa_permutare((i8*)xar_obtinere(xar, j),
                (i8*)xar_obtinere(xar, j - I),
                (memoriae_index)xar->magnitudo_elementi);
        }
    }
}

/* Xar Ordinare
 * "Ordinare in loco: fusio stabilis, O(n log n)"
 *
 * Olim selectio O(n^2) (et NON stabilis, quamvis ita dicta): fabrica
 * (parcum …AR15) photographiam arboris (~XX M elementa) ~VIII s per
 * ordinationem ordinabat. Elementa in scripturam contiguam exscribuntur
 * (2n elementa ex piscina tabulae, notatione reddita), cursibus I, II,
 * IV... fusa, deinde in segmenta reddita.
 */
vacuum
xar_ordinare (
              Xar* xar,
    XarComparator  comparator)
{
    PiscinaNotatio  notatio;
    memoriae_index  mensura;
    memoriae_index  totum;
                i8* fons;
                i8* destinatio;
                i8* commutandum;
               i32  numerus;
               i32  latitudo;
               i32  initium;
               i32  medium;
               i32  finis;
               i32  i;

    si (!xar || !comparator || xar->numerus_elementorum <= I)
    {
        redde;
    }
    numerus  = xar->numerus_elementorum;
    mensura  = (memoriae_index)xar->magnitudo_elementi;
    totum    = (memoriae_index)numerus * mensura;
    notatio  = piscina_notare(xar->piscina);
    fons     = (i8*)piscina_conari_allocare_ordinatum(xar->piscina,
        totum, XVI);
    destinatio = (fons != NIHIL)
        ? (i8*)piscina_conari_allocare_ordinatum(xar->piscina, totum,
              XVI)
        : NIHIL;
    si (destinatio == NIHIL)
    {
        piscina_reficere(xar->piscina, notatio);
        _ordinare_inserendo(xar, comparator);
        redde;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(fons + (memoriae_index)i * mensura, xar_obtinere(xar, i),
            mensura);
    }
    per (latitudo = I; latitudo < numerus; latitudo *= II)
    {
        per (initium = ZEPHYRUM; initium < numerus;
             initium += II * latitudo)
        {
            medium = (latitudo < numerus - initium)
                ? initium + latitudo : numerus;
            finis  = (II * latitudo < numerus - initium)
                ? initium + II * latitudo : numerus;
            _cursus_fundere(fons, destinatio, initium, medium, finis,
                mensura, comparator);
        }
        commutandum  = fons;
        fons         = destinatio;
        destinatio   = commutandum;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(xar_obtinere(xar, i), fons + (memoriae_index)i * mensura,
            mensura);
    }
    piscina_reficere(xar->piscina, notatio);
}

/* Xar Continet
 * "Verificare si elementum exsistit"
 */
b32
xar_continet (
       constans         Xar* xar,
       constans      vacuum* clavis,
              XarComparator  comparator)
{
    redde xar_invenire_index(xar, clavis, comparator) >= ZEPHYRUM;
}
#undef _bitum_summum
#undef _creare
#undef _cursus_fundere
#undef _elementa_permutare
#undef _ordinare_inserendo
#undef _refusio_segmentorum
#undef _segmentum
#undef _tabulam_crescere
#undef allocare_segmentum
#undef computare_magnitudinem_segmenti
#undef XAR_BITUM_SUMMUM
#undef XAR_FACTOR_DUPLICANDI
#undef xar_locare
#undef XarLocatio
/* lib/matrix.c: statica per plagulam renominata */
#define _apex_notare _apex_notare_matrix
#define _apex_officinarum _apex_officinarum_matrix
#define _columnae _columnae_matrix
#define _columnae_transforma _columnae_transforma_matrix
#define _columnas_permuta _columnas_permuta_matrix
#define _compacta _compacta_matrix
#define _dividit_reliqua _dividit_reliqua_matrix
#define _effectus _effectus_matrix
#define _est_spatium _est_spatium_matrix
#define _hermite _hermite_matrix
#define _identitas_operis _identitas_operis_matrix
#define _linea_normalis _linea_normalis_matrix
#define _lineae _lineae_matrix
#define _lineae_transforma _lineae_transforma_matrix
#define _lineas_permuta _lineas_permuta_matrix
#define _locus _locus_matrix
#define _minima_in_columna _minima_in_columna_matrix
#define _minima_in_linea _minima_in_linea_matrix
#define _nova _nova_matrix
#define _officina_reficere _officina_reficere_matrix
#define _officinae_aperire _officinae_aperire_matrix
#define _officinae_claudere _officinae_claudere_matrix
#define _officinis_utendum _officinis_utendum_matrix
#define _operarius_aperire _operarius_aperire_matrix
#define _operis _operis_matrix
#define _operis_piscina _operis_piscina_matrix
#define _parvae _parvae_matrix
#define _passus _passus_matrix
#define _permuta _permuta_matrix
#define _reductio _reductio_matrix
#define _scala _scala_matrix
#define _servare _servare_matrix
#define _sine_spatiis _sine_spatiis_matrix
#define _summa _summa_matrix
#define _tabula_reddere _tabula_reddere_matrix
#define Officinae Officinae_matrix
#define Operarius Operarius_matrix
#line 1 "lib/matrix.c"
/* matrix.c - Matrices exactae super anulum: Bareiss sine fractionibus
 *
 * Forma scalaris sine fractionibus (_scala): post cardinem (r, c) omne
 * elementum E[i][j] (i > r, j > c) minor est (r + 2) ordinis, ergo
 * divisio per cardinem priorem exacta (identitas Sylvestri) - etiam cum
 * columnis sine cardine. Determinans = cardo ultimus (signo
 * permutationum); gradus = numerus cardinum; nucleus per
 * substitutionem retrogradam SINE divisione (vide _nucleus_vector).
 *
 * Officinae (piscinae internae): 0 et 1 alternae (submatrix adhuc
 * eliminanda), II temporaria (producta unius elementi, refecta post
 * quodque), III stabilis (tabula laboris, lineae cardinum perfectae).
 * Omnis valor qui in piscinam aliam transit per anulus->transcribe
 * transit: nullus valor piscinam refectam partitur. Vide
 * lib/matrix.worklog.md.
 */


#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

/* passus elementi: mensura anuli ad VIII rotundata (ordinatio) */
interior memoriae_index
_passus (
    constans Anulus* anulus)
{
    redde (anulus->mensura + (memoriae_index)VII)
        & ~(memoriae_index)VII;
}

interior i8*
_locus (
                 i8* elementa,
    constans Anulus* anulus,
                i32  columnae,
                i32  linea,
                i32  columna)
{
    redde elementa + ((memoriae_index)linea * (memoriae_index)columnae
        + (memoriae_index)columna) * _passus(anulus);
}

/* matrix nova, elementis nullis */
interior b32
_nova (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus)
{
    memoriae_index numerus = (memoriae_index)lineae
        * (memoriae_index)columnae;
    memoriae_index k;

    si (anulus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->anulus    = anulus;
    exitus->lineae    = lineae;
    exitus->columnae  = columnae;
    exitus->elementa  = NIHIL;
    si (numerus == ZEPHYRUM)
    {
        redde VERUM;
    }
    exitus->elementa = (i8*)piscina_allocare(piscina, numerus
        * _passus(anulus));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        anulus->nullum(anulus, exitus->elementa + k * _passus(anulus));
    }
    redde VERUM;
}


/* ==================================================
 * Officinae
 * ================================================== */

#define OFFICINA_TEMPORARIA  II
#define OFFICINA_STABILIS    III

nomen structura {
           Piscina* piscinae[IV];
    PiscinaNotatio  notae[IV];
               b32  propriae;
} Officinae;

/* DIAGNOSIS: maximus usus officinae in operatione ultima */
interior memoriae_index _apex_officinarum = ZEPHYRUM;

interior vacuum
_apex_notare (
    Piscina* officina)
{
    memoriae_index usus = piscina_summa_usus(officina);

    si (usus > _apex_officinarum)
    {
        _apex_officinarum = usus;
    }
}

/* Via parva (sine officinis, in piscina vocantis) solum si matrix
 * parva ET omnia elementa parva (anulus->parvum: sine memoria externa):
 * creatio quattuor piscinarum plus constat quam servat (recensio
 * matrix-I: 2 x 2 0.5 us contra 0.007 us ad - bc), et iactura vocantis
 * minoribus elementorum parvorum finita. Numerus solus non sufficit
 * (recensio matrix-II: 5 x 5 elementis M digitorum 204 KB pro
 * determinante 2 KB). */
#define MATRIX_LIMES_ELEMENTORUM  XXV
#define MATRIX_LIMES_OPERUM       CXXV

interior b32
_parvae (
    Matrix m)
{
    memoriae_index numerus = (memoriae_index)m.lineae
        * (memoriae_index)m.columnae;
    memoriae_index k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!m.anulus->parvum(m.anulus, m.elementa
            + k * _passus(m.anulus)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* via officinarum nisi matrix parva et elementis parvis */
interior b32
_officinis_utendum (
            Matrix m,
    memoriae_index limes)
{
    redde (memoriae_index)m.lineae * (memoriae_index)m.columnae > limes
        || !_parvae(m);
}

/* effectus vocanti redditus: semper copia profunda in piscinam eius,
 * etiam via parva - effectus eliminationis et multiplicationis numquam
 * memoriam argumentorum partiuntur (recensio matrix-II) */
interior vacuum
_effectus (
    constans Anulus* anulus,
                 i8* fons,
            Piscina* piscina,
                 i8* destinatio)
{
    anulus->transcribe(anulus, fons, piscina, destinatio);
}

/* Si non utendae aut creatio deficit, omnes = piscina vocantis et
 * refectio nihil agit: effectus idem, memoria sine refectione. */
interior vacuum
_officinae_aperire (
     Officinae* o,
       Piscina* vocantis,
           b32  utendae)
{
    i32 k;
    b32 bene = VERUM;

    _apex_officinarum = ZEPHYRUM;
    si (!utendae)
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            o->piscinae[k] = vocantis;
        }
        o->propriae = FALSUM;
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        o->piscinae[k] = piscina_generare_dynamicum("matrix_officina",
            (memoriae_index)4096);
        si (o->piscinae[k] == NIHIL)
        {
            bene = FALSUM;
        }
    }
    si (!bene)
    {
        per (k = ZEPHYRUM; k < IV; k++)
        {
            si (o->piscinae[k])
            {
                piscina_destruere(o->piscinae[k]);
            }
            o->piscinae[k] = vocantis;
        }
        o->propriae = FALSUM;
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        o->notae[k] = piscina_notare(o->piscinae[k]);
    }
    o->propriae = VERUM;
}

interior vacuum
_officina_reficere (
     Officinae* o,
           i32  index)
{
    si (o->propriae)
    {
        _apex_notare(o->piscinae[index]);
        piscina_reficere(o->piscinae[index], o->notae[index]);
    }
}

/* valor in piscinam destinationis: copia profunda si officinae
 * propriae (fons refici potest), aliter copia structurae (omnia in
 * piscina vocantis manent) */
interior vacuum
_servare (
    constans Officinae* o,
       constans Anulus* anulus,
                    i8* fons,
               Piscina* piscina,
                    i8* destinatio)
{
    si (o->propriae)
    {
        anulus->transcribe(anulus, fons, piscina, destinatio);
    }
    alioquin si (fons != destinatio)
    {
        memcpy(destinatio, fons, anulus->mensura);
    }
}

interior vacuum
_officinae_claudere (
    Officinae* o)
{
    i32 k;

    si (!o->propriae)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        _apex_notare(o->piscinae[k]);
        piscina_destruere(o->piscinae[k]);
    }
}


/* ==================================================
 * Constructio et textus
 * ================================================== */

b32
matrix_nulla (
     constans Anulus* anulus,
                 i32  lineae,
                 i32  columnae,
             Piscina* piscina,
              Matrix* exitus)
{
    Matrix m;

    si (!_nova(anulus, lineae, columnae, piscina, &m))
    {
        redde FALSUM;
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_identitas (
     constans Anulus* anulus,
                 i32  n,
             Piscina* piscina,
              Matrix* exitus)
{
    Matrix m;
       i32 k;

    si (!_nova(anulus, n, n, piscina, &m))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        anulus->unum(anulus, piscina, _locus(m.elementa, anulus, n, k,
            k));
    }
    *exitus = m;
    redde VERUM;
}

interior b32
_est_spatium (
    i8 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* sectio sine spatiis extremis */
interior chorda
_sine_spatiis (
    chorda textus)
{
    i32 initium  = ZEPHYRUM;
    i32 finis    = textus.mensura;

    dum (initium < finis && _est_spatium(textus.datum[initium]))
    {
        initium++;
    }
    dum (finis > initium && _est_spatium(textus.datum[finis - I]))
    {
        finis--;
    }
    redde chorda_sectio(textus, initium, finis);
}

b32
matrix_ex_chorda (
     constans Anulus* anulus,
              chorda  textus,
             Piscina* piscina,
              Matrix* exitus)
{
    chorda  interior_textus;
       i32  lineae    = I;
       i32  columnae  = I;
       i32  commata   = ZEPHYRUM;
       i32  k;
       i32  linea;
       i32  columna;
       i32  initium;
    Matrix  m;
        i8* valor;

    si (anulus == NIHIL || textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    textus = _sine_spatiis(textus);
    si (   textus.mensura < II || textus.datum[ZEPHYRUM] != '['
        || textus.datum[textus.mensura - I] != ']')
    {
        redde FALSUM;
    }
    interior_textus = _sine_spatiis(chorda_sectio(textus, I,
        textus.mensura - I));
    si (interior_textus.mensura == ZEPHYRUM)
    {
        redde matrix_nulla(anulus, ZEPHYRUM, ZEPHYRUM, piscina, exitus);
    }

    /* dimensiones: lineae per ';', columnae lineae primae per ',';
     * omnes lineae tot commata habere debent */
    per (k = ZEPHYRUM; k < interior_textus.mensura; k++)
    {
        si (interior_textus.datum[k] == ';')
        {
            si (lineae == I)
            {
                columnae = commata + I;
            }
            alioquin si (commata + I != columnae)
            {
                redde FALSUM;
            }
            lineae++;
            commata = ZEPHYRUM;
        }
        alioquin si (interior_textus.datum[k] == ',')
        {
            commata++;
        }
    }
    si (lineae == I)
    {
        columnae = commata + I;
    }
    alioquin si (commata + I != columnae)
    {
        redde FALSUM;
    }

    si (!_nova(anulus, lineae, columnae, piscina, &m))
    {
        redde FALSUM;
    }
    valor    = (i8*)piscina_allocare(piscina, _passus(anulus));
    linea    = ZEPHYRUM;
    columna  = ZEPHYRUM;
    initium  = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= interior_textus.mensura; k++)
    {
        si (   k                        == interior_textus.mensura
            || interior_textus.datum[k] == ','
            || interior_textus.datum[k] == ';')
        {
            si (!anulus->ex_chorda(anulus, _sine_spatiis(chorda_sectio(
                interior_textus, initium, k)), piscina, valor))
            {
                redde FALSUM;
            }
            memcpy(_locus(m.elementa, anulus, columnae, linea, columna),
                valor, anulus->mensura);
            si (   k < interior_textus.mensura
                && interior_textus.datum[k] == ';')
            {
                linea++;
                columna = ZEPHYRUM;
            }
            alioquin
            {
                columna++;
            }
            initium = k + I;
        }
    }
    *exitus = m;
    redde VERUM;
}

chorda
matrix_ad_chordam (
     Matrix  m,
    Piscina* piscina)
{
    ChordaAedificator* scriba = chorda_aedificator_creare(piscina,
        (memoriae_index)LXIV);
                   i32 linea;
                   i32 columna;

    (vacuum)chorda_aedificator_appendere_character(scriba, '[');
    per (linea = ZEPHYRUM; m.columnae > ZEPHYRUM && linea < m.lineae;
         linea++)
    {
        si (linea > ZEPHYRUM)
        {
            (vacuum)chorda_aedificator_appendere_literis(scriba, "; ");
        }
        per (columna = ZEPHYRUM; columna < m.columnae; columna++)
        {
            si (columna > ZEPHYRUM)
            {
                (vacuum)chorda_aedificator_appendere_literis(scriba,
                    ", ");
            }
            (vacuum)chorda_aedificator_appendere_chorda(scriba,
                m.anulus->ad_chordam(m.anulus, _locus(m.elementa,
                m.anulus,
                    m.columnae, linea, columna), piscina));
        }
    }
    (vacuum)chorda_aedificator_appendere_character(scriba, ']');
    redde chorda_aedificator_finire(scriba);
}

constans Anulus*
matrix_anulus (
    Matrix m)
{
    redde m.anulus;
}

i32
matrix_lineae (
    Matrix m)
{
    redde m.lineae;
}

i32
matrix_columnae (
    Matrix m)
{
    redde m.columnae;
}

constans vacuum*
matrix_elementum (
    Matrix m,
       i32 linea,
       i32 columna)
{
    si (linea >= m.lineae || columna >= m.columnae)
    {
        redde NIHIL;
    }
    redde _locus(m.elementa, m.anulus, m.columnae, linea, columna);
}

vacuum
matrix_pone (
               Matrix* m,
                  i32  linea,
                  i32  columna,
      constans vacuum* valor)
{
    si (linea >= m->lineae || columna >= m->columnae)
    {
        redde;
    }
    memcpy(_locus(m->elementa, m->anulus, m->columnae, linea, columna),
        valor, m->anulus->mensura);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
matrix_aequalis (
    Matrix a,
    Matrix b)
{
    i32 linea;
    i32 columna;

    si (   a.anulus   != b.anulus || a.lineae != b.lineae
        || a.columnae != b.columnae)
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < a.columnae; columna++)
        {
            si (!a.anulus->aequalis(a.anulus, _locus(a.elementa,
                a.anulus,
                a.columnae,
                linea, columna), _locus(b.elementa, b.anulus,
                b.columnae,
                linea, columna)))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* a + signum b, per elementa */
interior b32
_summa (
     Matrix  a,
     Matrix  b,
        s32  signum,
    Piscina* piscina,
     Matrix* exitus)
{
    Matrix m;
       i32 linea;
       i32 columna;

    si (   a.anulus   != b.anulus || a.lineae != b.lineae
        || a.columnae != b.columnae
        || !_nova(a.anulus, a.lineae, a.columnae, piscina, &m))
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < a.columnae; columna++)
        {
            constans vacuum* x = _locus(a.elementa, a.anulus,
                a.columnae,
                linea, columna);
            constans vacuum* y = _locus(b.elementa, b.anulus,
                b.columnae,
                linea, columna);
                     vacuum* z = _locus(m.elementa, m.anulus,
                         m.columnae,
                         linea, columna);

            si (!(signum > ZEPHYRUM ? a.anulus->adde(a.anulus, x, y,
                piscina, z)
                : a.anulus->subtrahe(a.anulus, x, y, piscina, z)))
            {
                redde FALSUM;
            }
        }
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_adde (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
    redde _summa(a, b, I, piscina, exitus);
}

b32
matrix_subtrahe (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
    redde _summa(a, b, -I, piscina, exitus);
}

b32
matrix_multiplica (
     Matrix  a,
     Matrix  b,
    Piscina* piscina,
     Matrix* exitus)
{
             Matrix  m;
          Officinae  officinae;
    constans Anulus* anulus = a.anulus;
                 i8* productum;
                 i8* summa;
                i32  linea;
                i32  columna;
                i32  k;
                b32  bene = VERUM;

    si (   a.anulus != b.anulus || a.columnae != b.lineae
        || !_nova(anulus, a.lineae, b.columnae, piscina, &m))
    {
        redde FALSUM;
    }
    /* elementum quodque totum in officina temporaria, solus valor
     * finalis transcriptus */
    _officinae_aperire(&officinae, piscina, (memoriae_index)a.lineae
        * (memoriae_index)b.columnae * (memoriae_index)a.columnae
        > (memoriae_index)MATRIX_LIMES_OPERUM || !_parvae(a)
        || !_parvae(b));
    productum  = (i8*)piscina_allocare(officinae.piscinae[
        OFFICINA_STABILIS], _passus(anulus));
    summa      = (i8*)piscina_allocare(officinae.piscinae[
        OFFICINA_STABILIS], _passus(anulus));
    per (linea = ZEPHYRUM; bene && linea < a.lineae; linea++)
    {
        per (columna = ZEPHYRUM; bene
            && columna < b.columnae; columna++)
        {
            Piscina* temporaria =
                officinae.piscinae[OFFICINA_TEMPORARIA];

            anulus->nullum(anulus, summa);
            per (k = ZEPHYRUM; bene && k < a.columnae; k++)
            {
                bene = anulus->multiplica(anulus, _locus(a.elementa,
                    anulus,
                    a.columnae, linea, k), _locus(b.elementa, anulus,
                    b.columnae, k, columna), temporaria, productum)
                    && anulus->adde(anulus, summa, productum,
                    temporaria,
                    summa);
            }
            si (bene)
            {
                _effectus(anulus, summa, piscina,
                    _locus(m.elementa, anulus, m.columnae, linea,
                    columna));
            }
            _officina_reficere(&officinae, OFFICINA_TEMPORARIA);
        }
    }
    _officinae_claudere(&officinae);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = m;
    redde VERUM;
}

b32
matrix_transposita (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus)
{
    Matrix t;
       i32 linea;
       i32 columna;

    si (!_nova(m.anulus, m.columnae, m.lineae, piscina, &t))
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < m.lineae; linea++)
    {
        per (columna = ZEPHYRUM; columna < m.columnae; columna++)
        {
            memcpy(_locus(t.elementa, t.anulus, t.columnae, columna,
                linea), _locus(m.elementa, m.anulus, m.columnae, linea,
                columna), m.anulus->mensura);
        }
    }
    *exitus = t;
    redde VERUM;
}


/* ==================================================
 * Eliminatio
 * ================================================== */

/* Forma scalaris sine fractionibus in tabula laboris (officina
 * stabilis). cardines[r] = columna cardinis lineae r (vocans praebet
 * lineae elementa); *gradus, *signum (permutationes linearum). FALSUM
 * si operatio elementi refutat.
 *
 * plena FALSUM (determinans, gradus): lineae infra cardinem solae;
 * lineae cardinum perfectae in officina stabili.
 * plena VERUM (Gauss-Jordan sine fractionibus, nucleus): lineae SUPRA
 * cardinem quoque - post gradum k omnes cardines priores = cardo k
 * (minor), omnia elementa minores, ergo divisio exacta et magnitudo
 * minoribus finita (recensio matrix-I: substitutio retrograda sine
 * divisione 1207 bitorum pro 104 in 25 x 30). Lineae omnes mutantur,
 * ergo omnes in officina alterna vivunt. */
interior b32
_scala (
         Matrix   m,
            b32   plena,
      Officinae*  officinae,
             i8** tabula_exitus,
            i32*  cardines,
            i32*  gradus,
            s32*  signum)
{
     constans Anulus* anulus    = m.anulus;
             Piscina* stabilis;
      memoriae_index  passus = _passus(anulus);
                  i8* tabula = NIHIL;
                  i8* prior;
                  i8* primum;
                  i8* secundum;
                  i8* permutatio;
                 i32  hic  = ZEPHYRUM;
                 i32  r    = ZEPHYRUM;
                 i32  c;
                 i32  i;
                 i32  j;

    stabilis  = officinae->piscinae[OFFICINA_STABILIS];
    *signum   = I;
    si (m.lineae > ZEPHYRUM && m.columnae > ZEPHYRUM)
    {
        tabula = (i8*)piscina_allocare(stabilis,
            (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * passus);
        memcpy(tabula, m.elementa, (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * passus);
    }
    prior       = (i8*)piscina_allocare(stabilis, passus);
    primum      = (i8*)piscina_allocare(stabilis, passus);
    secundum    = (i8*)piscina_allocare(stabilis, passus);
    permutatio  = (i8*)piscina_allocare(stabilis, passus);
    anulus->unum(anulus, stabilis, prior);

    per (c = ZEPHYRUM; c < m.columnae && r < m.lineae; c++)
    {
         Piscina* illic           = officinae->piscinae[I - hic];
             i32  linea_cardinis  = r;

        dum (   linea_cardinis < m.lineae
             && anulus->est_nullum(anulus, _locus(tabula,
            anulus, m.columnae, linea_cardinis, c)))
        {
            linea_cardinis++;
        }
        si (linea_cardinis == m.lineae)
        {
            perge;   /* columna sine cardine */
        }
        si (linea_cardinis != r)
        {
            per (j = ZEPHYRUM; j < m.columnae; j++)
            {
                i8* x = _locus(tabula, anulus, m.columnae, r, j);
                i8* y = _locus(tabula, anulus, m.columnae,
                    linea_cardinis,
                    j);

                memcpy(permutatio, x, anulus->mensura);
                memcpy(x, y, anulus->mensura);
                memcpy(y, permutatio, anulus->mensura);
            }
            *signum = -*signum;
        }

        /* E[i][j] = (E[r][c] E[i][j] - E[i][c] E[r][j]) / prior; supra
         * cardinem (plena) etiam j < c: E[r][j] ibi nullum, ergo
         * scalatio per E[r][c] / prior */
        per (i = plena ? ZEPHYRUM : r + I; i < m.lineae; i++)
        {
            si (i == r)
            {
                perge;
            }
            per (j = (i < r) ? ZEPHYRUM : c + I; j < m.columnae; j++)
            {
                Piscina* temporaria = officinae->piscinae[
                    OFFICINA_TEMPORARIA];
                     b32 bene;

                si (j == c)
                {
                    perge;
                }
                bene = anulus->multiplica(anulus, _locus(tabula, anulus,
                    m.columnae, r, c), _locus(tabula, anulus,
                    m.columnae, i,
                    j), temporaria, primum)
                    && anulus->multiplica(anulus, _locus(tabula, anulus,
                        m.columnae, i, c), _locus(tabula, anulus,
                        m.columnae, r, j), temporaria, secundum)
                    && anulus->subtrahe(anulus, primum, secundum,
                    temporaria,
                        primum)
                    && anulus->divide_exacte(anulus, primum, prior,
                    temporaria,
                        secundum);
                si (!bene)
                {
                    redde FALSUM;
                }
                _servare(officinae, anulus, secundum, illic,
                    _locus(tabula, anulus, m.columnae, i, j));
                _officina_reficere(officinae, OFFICINA_TEMPORARIA);
            }
            anulus->nullum(anulus, _locus(tabula, anulus, m.columnae, i,
                c));
        }

        /* linea r: perfecta in officinam stabilem (plena: in alternam,
         * quia gradibus sequentibus mutatur); cardo fit prior */
        per (j = plena ? ZEPHYRUM : c; j < m.columnae; j++)
        {
            i8* x = _locus(tabula, anulus, m.columnae, r, j);

            memcpy(permutatio, x, anulus->mensura);
            _servare(officinae, anulus, permutatio, plena ? illic
                : stabilis, x);
        }
        memcpy(prior, _locus(tabula, anulus, m.columnae, r, c),
            anulus->mensura);
        _officina_reficere(officinae, hic);
        hic          = I - hic;
        cardines[r]  = c;
        r++;
    }
    *tabula_exitus  = tabula;
    *gradus         = r;
    redde VERUM;
}

b32
matrix_determinans (
     Matrix  m,
    Piscina* piscina,
     vacuum* exitus)
{
     constans Anulus* anulus = m.anulus;
           Officinae  officinae;
                  i8* tabula;
                 i32* cardines;
                 i32  gradus;
                 s32  signum;
                  i8* valor;

    si (m.lineae != m.columnae || !anulus->integrum)
    {
        redde FALSUM;
    }
    si (m.lineae == ZEPHYRUM)
    {
        anulus->unum(anulus, piscina, exitus);
        redde VERUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    cardines =
        (i32*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        (memoriae_index)m.lineae * magnitudo(i32));
    valor = (i8*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        _passus(anulus));
    si (!_scala(m, FALSUM, &officinae, &tabula, cardines, &gradus,
        &signum))
    {
        _officinae_claudere(&officinae);
        redde FALSUM;
    }
    si (gradus < m.lineae)
    {
        anulus->nullum(anulus, exitus);
        _officinae_claudere(&officinae);
        redde VERUM;
    }
    /* cardo ultimus = determinans matricis permutatae */
    memcpy(valor, _locus(tabula, anulus, m.columnae, m.lineae - I,
        m.columnae - I), anulus->mensura);
    si (signum < ZEPHYRUM)
    {
        i8* nullum = (i8*)piscina_allocare(officinae.piscinae[
            OFFICINA_STABILIS], _passus(anulus));

        anulus->nullum(anulus, nullum);
        si (!anulus->subtrahe(anulus, nullum, valor, officinae.piscinae[
            OFFICINA_STABILIS], valor))
        {
            _officinae_claudere(&officinae);
            redde FALSUM;
        }
    }
    _effectus(anulus, valor, piscina, (i8*)exitus);
    _officinae_claudere(&officinae);
    redde VERUM;
}

b32
matrix_gradus (
     Matrix  m,
    Piscina* piscina,
        i32* exitus)
{
    Officinae  officinae;
           i8* tabula;
          i32* cardines;
          i32  gradus;
          s32  signum;
          b32  bene;

    si (!m.anulus->integrum)
    {
        /* gradus super anulum non integrum non definitus */
        redde FALSUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    cardines =
        (i32*)piscina_allocare(officinae.piscinae[OFFICINA_STABILIS],
        (memoriae_index)(m.lineae + I) * magnitudo(i32));
    bene = _scala(m, FALSUM, &officinae, &tabula, cardines, &gradus,
        &signum);
    _officinae_claudere(&officinae);
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = gradus;
    redde VERUM;
}

/* Nucleus ex forma Gauss-Jordan sine fractionibus: cardines omnes = D
 * (cardo ultimus), ergo linea i: D z_p(i) + summa_{f libera} E[i][f]
 * z_f = 0. Pro columna libera f: z_f = D, z_p(i) = -E[i][f], ceteri 0
 * - elementa minores (magnitudo Hadamard finita). Sine cardine: D = 1,
 * nucleus = identitas. */
b32
matrix_nucleus (
     Matrix  m,
    Piscina* piscina,
     Matrix* exitus)
{
     constans Anulus* anulus = m.anulus;
           Officinae  officinae;
              Matrix  nucleus;
                  i8* tabula;
                 i32* cardines;
                 b32* est_cardo;
                  i8* d;
                  i8* nullum;
                  i8* valor;
                 i32  gradus;
                 s32  signum;
                 i32  j;
                 i32  q = ZEPHYRUM;
             Piscina* stabilis;

    si (!anulus->integrum)
    {
        /* nucleus super corpus fractionum: integrum requiritur */
        redde FALSUM;
    }
    _officinae_aperire(&officinae, piscina, _officinis_utendum(m,
        (memoriae_index)MATRIX_LIMES_ELEMENTORUM));
    stabilis   = officinae.piscinae[OFFICINA_STABILIS];
    cardines   = (i32*)piscina_allocare(stabilis, (memoriae_index)(
        m.lineae + I) * magnitudo(i32));
    est_cardo  = (b32*)piscina_allocare(stabilis, (memoriae_index)(
        m.columnae + I) * magnitudo(b32));
    d       = (i8*)piscina_allocare(stabilis, _passus(anulus));
    nullum  = (i8*)piscina_allocare(stabilis, _passus(anulus));
    valor   = (i8*)piscina_allocare(stabilis, _passus(anulus));
    si (   !_scala(m, VERUM, &officinae, &tabula, cardines, &gradus,
            &signum)
        || !_nova(anulus, m.columnae, m.columnae - gradus, piscina,
            &nucleus))
    {
        _officinae_claudere(&officinae);
        redde FALSUM;
    }
    anulus->nullum(anulus, nullum);
    si (gradus == ZEPHYRUM)
    {
        anulus->unum(anulus, stabilis, d);
    }
    alioquin
    {
        memcpy(d, _locus(tabula, anulus, m.columnae, gradus - I,
            cardines[gradus - I]), anulus->mensura);
    }
    per (j = ZEPHYRUM; j < m.columnae; j++)
    {
        est_cardo[j] = FALSUM;
    }
    per (j = ZEPHYRUM; j < gradus; j++)
    {
        est_cardo[cardines[j]] = VERUM;
    }
    per (j = ZEPHYRUM; j < m.columnae; j++)
    {
        i32 i;

        si (est_cardo[j])
        {
            perge;
        }
        _effectus(anulus, d, piscina,
            _locus(nucleus.elementa,
            anulus, nucleus.columnae, j, q));
        per (i = ZEPHYRUM; i < gradus; i++)
        {
            si (!anulus->subtrahe(anulus, nullum, _locus(tabula, anulus,
                m.columnae, i, j),
                officinae.piscinae[OFFICINA_TEMPORARIA],
                valor))
            {
                _officinae_claudere(&officinae);
                redde FALSUM;
            }
            _effectus(anulus, valor, piscina, _locus(
                nucleus.elementa, anulus, nucleus.columnae, cardines[i],
                q));
            _officina_reficere(&officinae, OFFICINA_TEMPORARIA);
        }
        q++;
    }
    _officinae_claudere(&officinae);
    *exitus = nucleus;
    redde VERUM;
}


/* ==================================================
 * Formae normales super anulum Euclideum (Hermite, Smith)
 * ================================================== */

#define TABULA_A  ZEPHYRUM
#define TABULA_U  I
#define TABULA_V  II

/* Operarius: tabulae laboris (A, et U, V si certificata quaeruntur) in
 * officina stabili; valores vivi in officina alterna currente (hic).
 * Post
 * quemque gradum cardinis _compacta omnes valores vivos in alteram
 * transcribit et priorem reficit: memoria proportionalis tabulis, non
 * operationibus. Alvei elementorum (coefficientes, temporaria) in
 * stabili. */
nomen structura {
    constans Anulus* anulus;
          Officinae  officinae;
                i32  hic;
                 i8* tabulae[III];
                i32  lineae[III];
                i32  columnae[III];
                 i8* alvei[XII];
} Operarius;

#define ALVEUS_A    ZEPHYRUM
#define ALVEUS_B    I
#define ALVEUS_C    II
#define ALVEUS_D    III
#define ALVEUS_G    IV
#define ALVEUS_P    V
#define ALVEUS_Q    VI
#define ALVEUS_X    VII
#define ALVEUS_Y    VIII
#define ALVEUS_T    IX
#define ALVEUS_NX   X
#define ALVEUS_NY   XI

interior i8*
_operis (
     Operarius* o,
           i32  tabula,
           i32  linea,
           i32  columna)
{
    redde _locus(o->tabulae[tabula], o->anulus, o->columnae[tabula],
        linea,
        columna);
}

/* tabula identitatis n x n in officina stabili */
interior i8*
_identitas_operis (
            Operarius* o,
                  i32  n)
{
     Piscina* stabilis  = o->officinae.piscinae[OFFICINA_STABILIS];
          i8* tabula    = NIHIL;
         i32  i;
         i32  j;

    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    tabula = (i8*)piscina_allocare(stabilis, (memoriae_index)n
        * (memoriae_index)n * _passus(o->anulus));
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = ZEPHYRUM; j < n; j++)
        {
            i8* x = _locus(tabula, o->anulus, n, i, j);

            si (i == j)
            {
                o->anulus->unum(o->anulus, stabilis, x);
            }
            alioquin
            {
                o->anulus->nullum(o->anulus, x);
            }
        }
    }
    redde tabula;
}

interior vacuum
_operarius_aperire (
      Operarius* o,
         Matrix  m,
        Piscina* piscina,
            b32  cum_u,
            b32  cum_v)
{
     Piscina* stabilis;
         i32  k;

    o->anulus = m.anulus;
    _officinae_aperire(&o->officinae, piscina, VERUM);
    o->hic    = ZEPHYRUM;
    stabilis  = o->officinae.piscinae[OFFICINA_STABILIS];
    per (k = ZEPHYRUM; k < XII; k++)
    {
        o->alvei[k] = (i8*)piscina_allocare(stabilis,
            _passus(m.anulus));
    }
    o->lineae[TABULA_A]    = m.lineae;
    o->columnae[TABULA_A]  = m.columnae;
    o->tabulae[TABULA_A]   = NIHIL;
    si (m.lineae > ZEPHYRUM && m.columnae > ZEPHYRUM)
    {
        o->tabulae[TABULA_A] = (i8*)piscina_allocare(stabilis,
            (memoriae_index)m.lineae * (memoriae_index)m.columnae
            * _passus(m.anulus));
        memcpy(o->tabulae[TABULA_A], m.elementa,
            (memoriae_index)m.lineae
            * (memoriae_index)m.columnae * _passus(m.anulus));
    }
    o->lineae[TABULA_U]    = cum_u ? m.lineae : ZEPHYRUM;
    o->columnae[TABULA_U]  = o->lineae[TABULA_U];
    o->tabulae[TABULA_U]   = _identitas_operis(o, o->lineae[TABULA_U]);
    o->lineae[TABULA_V]    = cum_v ? m.columnae : ZEPHYRUM;
    o->columnae[TABULA_V]  = o->lineae[TABULA_V];
    o->tabulae[TABULA_V]   = _identitas_operis(o, o->lineae[TABULA_V]);
}

interior Piscina*
_operis_piscina (
    Operarius* o)
{
    redde o->officinae.piscinae[o->hic];
}

/* omnes valores vivi in officinam alteram; prior reficitur */
interior vacuum
_compacta (
    Operarius* o)
{
     Piscina* illic = o->officinae.piscinae[I - o->hic];
         i32  t;
         i32  i;
         i32  j;

    per (t = ZEPHYRUM; t < III; t++)
    {
        per (i = ZEPHYRUM; i < o->lineae[t]; i++)
        {
            per (j = ZEPHYRUM; j < o->columnae[t]; j++)
            {
                i8* x = _operis(o, t, i, j);

                _servare(&o->officinae, o->anulus, x, illic, x);
            }
        }
    }
    _officina_reficere(&o->officinae, o->hic);
    o->hic = I - o->hic;
}

/* lineae i, k tabulae t: L_i <- a L_i + b L_k, L_k <- c L_i + d L_k */
interior b32
_lineae_transforma (
     Operarius* o,
           i32  t,
           i32  i,
           i32  k)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  j;

    per (j = ZEPHYRUM; j < o->columnae[t]; j++)
    {
        i8* x = _operis(o, t, i, j);
        i8* y = _operis(o, t, k, j);

        si (   !anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_B], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NX],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_C], x, hic,
                o->alvei[ALVEUS_NY])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_D], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NY],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NY]))
        {
            redde FALSUM;
        }
        memcpy(x, o->alvei[ALVEUS_NX], anulus->mensura);
        memcpy(y, o->alvei[ALVEUS_NY], anulus->mensura);
    }
    redde VERUM;
}

/* columnae j, k tabulae t: C_j <- a C_j + b C_k, C_k <- c C_j +
 * d C_k */
interior b32
_columnae_transforma (
     Operarius* o,
           i32  t,
           i32  j,
           i32  k)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  i;

    per (i = ZEPHYRUM; i < o->lineae[t]; i++)
    {
        i8* x = _operis(o, t, i, j);
        i8* y = _operis(o, t, i, k);

        si (   !anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_B], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NX],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NX])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_C], x, hic,
                o->alvei[ALVEUS_NY])
            || !anulus->multiplica(anulus, o->alvei[ALVEUS_D], y, hic,
                o->alvei[ALVEUS_T])
            || !anulus->adde(anulus, o->alvei[ALVEUS_NY],
            o->alvei[ALVEUS_T],
            hic,
                o->alvei[ALVEUS_NY]))
        {
            redde FALSUM;
        }
        memcpy(x, o->alvei[ALVEUS_NX], anulus->mensura);
        memcpy(y, o->alvei[ALVEUS_NY], anulus->mensura);
    }
    redde VERUM;
}

/* Reductio Euclidea: x = q p + r; alvei (A, B, C, D) = (1, 0, -q, 1),
 * ergo operatio (cardo, k) L_k <- L_k - q L_cardinis (aut columnae).
 * *exacta = (r nullum). Sine Bezout: multiplicatores toti lineae soli
 * quotientes sunt (recensio matrix-III: Bezout lineam cardinis per
 * columnam totam multiplicabat - Hermite 36 x 36 22.6 s, 40 MB). */
interior b32
_reductio (
             Operarius* o,
       constans vacuum* x,
       constans vacuum* p,
                   b32* exacta)
{
    constans Anulus* anulus  = o->anulus;
            Piscina* hic     = _operis_piscina(o);

    si (!anulus->divide_cum_residuo(anulus, x, p, hic,
        o->alvei[ALVEUS_X],
        o->alvei[ALVEUS_Y]))
    {
        redde FALSUM;
    }
    *exacta = anulus->est_nullum(anulus, o->alvei[ALVEUS_Y]);
    anulus->unum(anulus, hic, o->alvei[ALVEUS_A]);
    anulus->nullum(anulus, o->alvei[ALVEUS_B]);
    anulus->unum(anulus, hic, o->alvei[ALVEUS_D]);
    anulus->nullum(anulus, o->alvei[ALVEUS_T]);
    redde anulus->subtrahe(anulus, o->alvei[ALVEUS_T],
        o->alvei[ALVEUS_X], hic,
        o->alvei[ALVEUS_C]);
}

/* linea (>= ab) cum elemento non nullo normae minimae in columna c; -1
 * si nullum */
interior s32
_minima_in_columna (
     Operarius* o,
           i32  c,
           i32  ab)
{
     constans Anulus* anulus = o->anulus;
                 s32  optima = -I;
                 i32  i;

    per (i = ab; i < o->lineae[TABULA_A]; i++)
    {
        i8* x = _operis(o, TABULA_A, i, c);

        si (anulus->est_nullum(anulus, x))
        {
            perge;
        }
        si (   optima < ZEPHYRUM
            || anulus->compara_normam(anulus, x, _operis(o, TABULA_A,
            (i32)optima,
                c), _operis_piscina(o)) < ZEPHYRUM)
        {
            optima = (s32)i;
        }
    }
    redde optima;
}

/* columna (>= ab) cum elemento non nullo normae minimae in linea l */
interior s32
_minima_in_linea (
     Operarius* o,
           i32  l,
           i32  ab)
{
     constans Anulus* anulus = o->anulus;
                 s32  optima = -I;
                 i32  j;

    per (j = ab; j < o->columnae[TABULA_A]; j++)
    {
        i8* x = _operis(o, TABULA_A, l, j);

        si (anulus->est_nullum(anulus, x))
        {
            perge;
        }
        si (   optima < ZEPHYRUM
            || anulus->compara_normam(anulus, x, _operis(o, TABULA_A, l,
                (i32)optima), _operis_piscina(o)) < ZEPHYRUM)
        {
            optima = (s32)j;
        }
    }
    redde optima;
}

/* operatio linearum in A et U */
interior b32
_lineae (
     Operarius* o,
           i32  i,
           i32  k)
{
    redde _lineae_transforma(o, TABULA_A, i, k)
        && (o->lineae[TABULA_U] == ZEPHYRUM
            || _lineae_transforma(o, TABULA_U, i, k));
}

/* operatio columnarum in A et V */
interior b32
_columnae (
     Operarius* o,
           i32  j,
           i32  k)
{
    redde _columnae_transforma(o, TABULA_A, j, k)
        && (o->lineae[TABULA_V] == ZEPHYRUM
            || _columnae_transforma(o, TABULA_V, j, k));
}

interior vacuum
_permuta (
    Operarius* o,
           i8* x,
           i8* y)
{
    memcpy(o->alvei[ALVEUS_T], x, o->anulus->mensura);
    memcpy(x, y, o->anulus->mensura);
    memcpy(y, o->alvei[ALVEUS_T], o->anulus->mensura);
}

interior vacuum
_lineas_permuta (
     Operarius* o,
           i32  i,
           i32  k)
{
    i32 t;
    i32 j;

    per (t = TABULA_A; t <= TABULA_U; t++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t] && i != k; j++)
        {
            _permuta(o, _operis(o, t, i, j), _operis(o, t, k, j));
        }
    }
}

interior vacuum
_columnas_permuta (
     Operarius* o,
           i32  j,
           i32  k)
{
    i32 i;

    per (i = ZEPHYRUM; i < o->lineae[TABULA_A] && j != k; i++)
    {
        _permuta(o, _operis(o, TABULA_A, i, j), _operis(o, TABULA_A, i,
            k));
    }
    per (i = ZEPHYRUM; i < o->lineae[TABULA_V] && j != k; i++)
    {
        _permuta(o, _operis(o, TABULA_V, i, j), _operis(o, TABULA_V, i,
            k));
    }
}

/* cardo lineae i normalis: linea per unitatem u multiplicatur ubi u p =
 * g = mdc(p, 0) (Z: signum) */
interior b32
_linea_normalis (
     Operarius* o,
           i32  i,
           i32  c)
{
     constans Anulus* anulus  = o->anulus;
             Piscina* hic     = _operis_piscina(o);
                 i32  t;
                 i32  j;

    anulus->nullum(anulus, o->alvei[ALVEUS_T]);
    si (!anulus->divisor_communis(anulus, _operis(o, TABULA_A, i, c),
        o->alvei[ALVEUS_T], hic, o->alvei[ALVEUS_G], o->alvei[ALVEUS_A],
        o->alvei[ALVEUS_B]))
    {
        redde FALSUM;
    }
    per (t = TABULA_A; t <= TABULA_U; t++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t]; j++)
        {
            i8* x = _operis(o, t, i, j);

            si (!anulus->multiplica(anulus, o->alvei[ALVEUS_A], x, hic,
                x))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* tabula operis in matricem vocantis (copia profunda) */
interior b32
_tabula_reddere (
      Operarius* o,
            i32  t,
        Piscina* piscina,
         Matrix* exitus)
{
    Matrix m;
       i32 i;
       i32 j;

    si (!_nova(o->anulus, o->lineae[t], o->columnae[t], piscina, &m))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < o->lineae[t]; i++)
    {
        per (j = ZEPHYRUM; j < o->columnae[t]; j++)
        {
            _effectus(o->anulus, _operis(o, t, i, j), piscina, _locus(
                m.elementa, o->anulus, m.columnae, i, j));
        }
    }
    *exitus = m;
    redde VERUM;
}

/* Hermite in operario per Euclidem in quaque columna: cardo normae
 * minimae sursum, ceterae lineae modulo eum (quotiens solus
 * multiplicator), donec columna infra nulla; *gradus = lineae non
 * nullae */
interior b32
_hermite (
    Operarius* o,
          i32* gradus)
{
     constans Anulus* anulus    = o->anulus;
                 i32  lineae    = o->lineae[TABULA_A];
                 i32  columnae  = o->columnae[TABULA_A];
                 i32  r         = ZEPHYRUM;
                 i32  c;
                 i32  i;
                 i32  k;

    per (c = ZEPHYRUM; c < columnae && r < lineae; c++)
    {
        dum (VERUM)
        {
            s32 cardo   = _minima_in_columna(o, c, r);
            b32 exacta  = VERUM;

            si (cardo < ZEPHYRUM)
            {
                frange;
            }
            _lineas_permuta(o, r, (i32)cardo);
            per (i = r + I; i < lineae; i++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(o, TABULA_A, i,
                    c)))
                {
                    perge;
                }
                si (   !_reductio(o, _operis(o, TABULA_A, i, c),
                        _operis(o, TABULA_A, r, c), &haec_exacta)
                    || !_lineae(o, r, i))
                {
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            _compacta(o);
            si (exacta)
            {
                frange;
            }
        }
        si (anulus->est_nullum(anulus, _operis(o, TABULA_A, r, c)))
        {
            perge;
        }
        si (!_linea_normalis(o, r, c))
        {
            redde FALSUM;
        }
        /* supra cardinem: residua 0 <= x < cardo */
        per (k = ZEPHYRUM; k < r; k++)
        {
            Piscina* hic = _operis_piscina(o);

            si (!anulus->divide_cum_residuo(anulus, _operis(o, TABULA_A,
                k, c),
                _operis(o, TABULA_A, r, c), hic, o->alvei[ALVEUS_X],
                o->alvei[ALVEUS_Y]))
            {
                redde FALSUM;
            }
            anulus->unum(anulus, hic, o->alvei[ALVEUS_A]);
            anulus->nullum(anulus, o->alvei[ALVEUS_T]);
            si (!anulus->subtrahe(anulus, o->alvei[ALVEUS_T],
                o->alvei[ALVEUS_X],
                hic, o->alvei[ALVEUS_B]))
            {
                redde FALSUM;
            }
            anulus->nullum(anulus, o->alvei[ALVEUS_C]);
            anulus->unum(anulus, hic, o->alvei[ALVEUS_D]);
            si (!_lineae(o, k, r))
            {
                redde FALSUM;
            }
        }
        _compacta(o);
        r++;
    }
    *gradus = r;
    redde VERUM;
}

b32
matrix_forma_hermite (
     Matrix  a,
    Piscina* piscina,
     Matrix* h,
     Matrix* u)
{
    Operarius o;
          i32 gradus;
       Matrix forma;
       Matrix transformatio;

    si (   a.anulus->divisor_communis   == NIHIL
        || a.anulus->divide_cum_residuo == NIHIL
        || a.anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    _operarius_aperire(&o, a, piscina, u != NIHIL, FALSUM);
    si (   !_hermite(&o, &gradus)
        || !_tabula_reddere(&o, TABULA_A, piscina, &forma)
        || (u != NIHIL && !_tabula_reddere(&o, TABULA_U, piscina,
            &transformatio)))
    {
        _officinae_claudere(&o.officinae);
        redde FALSUM;
    }
    _officinae_claudere(&o.officinae);
    *h = forma;
    si (u != NIHIL)
    {
        *u = transformatio;
    }
    redde VERUM;
}

/* E[t][t] dividit omnia E[i][j] (i, j > t)? Si non, *linea = i. */
interior b32
_dividit_reliqua (
     Operarius* o,
           i32  t,
           i32* linea)
{
     constans Anulus* anulus = o->anulus;
                 i32  i;
                 i32  j;

    per (i = t + I; i < o->lineae[TABULA_A]; i++)
    {
        per (j = t + I; j < o->columnae[TABULA_A]; j++)
        {
            si (!anulus->divide_exacte(anulus, _operis(o, TABULA_A, i,
                j),
                _operis(o, TABULA_A, t, t), _operis_piscina(o),
                o->alvei[ALVEUS_X]))
            {
                *linea = i;
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

b32
matrix_forma_smith (
     Matrix  a,
    Piscina* piscina,
     Matrix* d,
     Matrix* u,
     Matrix* v)
{
     constans Anulus* anulus = a.anulus;
           Operarius  o;
                 i32  t;
                 i32  n = a.lineae < a.columnae ? a.lineae : a.columnae;
              Matrix  forma;
              Matrix  sinistra;
              Matrix  dextra;

    si (   anulus->divisor_communis   == NIHIL
        || anulus->divide_cum_residuo == NIHIL
        || anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    _operarius_aperire(&o, a, piscina, u != NIHIL, v != NIHIL);
    per (t = ZEPHYRUM; t < n; t++)
    {
        i32 i;
        i32 j;
        b32 inventum = FALSUM;

        /* elementum non nullum in submatrice ad (t, t) */
        per (i = t; i < a.lineae && !inventum; i++)
        {
            per (j = t; j < a.columnae && !inventum; j++)
            {
                si (!anulus->est_nullum(anulus, _operis(&o, TABULA_A, i,
                    j)))
                {
                    _lineas_permuta(&o, t, i);
                    _columnas_permuta(&o, t, j);
                    inventum = VERUM;
                }
            }
        }
        si (!inventum)
        {
            frange;
        }
        /* Euclides alternus: columna t (cardo minimus sursum, lineae
         * modulo), deinde linea t (cardo minimus sinistrorsum, columnae
         * modulo); quodque residuum non nullum normam cardinis stricte
         * minuit, ergo terminatio. Operationes columnarum columnam t
         * non replent (infra cardinem nulla). */
        dum (VERUM)
        {
            s32 k;
            b32 exacta = VERUM;
            i32 linea;

            k = _minima_in_columna(&o, t, t);
            _lineas_permuta(&o, t, (i32)k);
            per (i = t + I; i < a.lineae; i++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(&o, TABULA_A, i,
                    t)))
                {
                    perge;
                }
                si (   !_reductio(&o, _operis(&o, TABULA_A, i, t),
                        _operis(&o, TABULA_A, t, t), &haec_exacta)
                    || !_lineae(&o, t, i))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            si (!exacta)
            {
                _compacta(&o);
                perge;
            }
            k = _minima_in_linea(&o, t, t);
            si ((i32)k != t)
            {
                _columnas_permuta(&o, t, (i32)k);
                _compacta(&o);
                perge;
            }
            per (j = t + I; j < a.columnae; j++)
            {
                b32 haec_exacta;

                si (anulus->est_nullum(anulus, _operis(&o, TABULA_A, t,
                    j)))
                {
                    perge;
                }
                si (   !_reductio(&o, _operis(&o, TABULA_A, t, j),
                        _operis(&o, TABULA_A, t, t), &haec_exacta)
                    || !_columnae(&o, t, j))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                si (!haec_exacta)
                {
                    exacta = FALSUM;
                }
            }
            _compacta(&o);
            si (!exacta)
            {
                perge;
            }
            /* divisibilitas: si cardo elementum reliquum non dividit,
             * L_t += L_i et iterum (cardo ad mdc decrescit) */
            si (!_dividit_reliqua(&o, t, &linea))
            {
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_A]);
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_B]);
                anulus->nullum(anulus, o.alvei[ALVEUS_C]);
                anulus->unum(anulus, _operis_piscina(&o),
                    o.alvei[ALVEUS_D]);
                si (!_lineae(&o, t, linea))
                {
                    _officinae_claudere(&o.officinae);
                    redde FALSUM;
                }
                perge;
            }
            frange;
        }
        si (!_linea_normalis(&o, t, t))
        {
            _officinae_claudere(&o.officinae);
            redde FALSUM;
        }
        _compacta(&o);
    }
    si (   !_tabula_reddere(&o, TABULA_A, piscina, &forma)
        || (u != NIHIL && !_tabula_reddere(&o, TABULA_U, piscina,
            &sinistra))
        || (v != NIHIL
            && !_tabula_reddere(&o, TABULA_V, piscina, &dextra)))
    {
        _officinae_claudere(&o.officinae);
        redde FALSUM;
    }
    _officinae_claudere(&o.officinae);
    *d = forma;
    si (u != NIHIL)
    {
        *u = sinistra;
    }
    si (v != NIHIL)
    {
        *v = dextra;
    }
    redde VERUM;
}

/* Basis reticuli nuclei: U A^T = H (Hermite); lineae U quibus lineae H
 * nullae respondent nucleum A generant super Z (U unimodularis: basis
 * totius reticuli, non sub-reticuli). */
b32
matrix_reticulum_nuclei (
     Matrix  a,
    Piscina* piscina,
     Matrix* exitus)
{
     constans Anulus* anulus = a.anulus;
             Piscina* privata;
              Matrix  transposita;
              Matrix  nucleus;
           Operarius  o;
                 i32  gradus;
                 i32  i;
                 i32  q;
                 b32  bene;

    si (   anulus->divisor_communis   == NIHIL
        || anulus->divide_cum_residuo == NIHIL
        || anulus->compara_normam     == NIHIL)
    {
        redde FALSUM;
    }
    privata = piscina_generare_dynamicum("matrix_reticulum",
        (memoriae_index)4096);
    si (privata == NIHIL)
    {
        privata = piscina;
    }
    bene = matrix_transposita(a, privata, &transposita);
    si (bene)
    {
        _operarius_aperire(&o, transposita, privata, VERUM, FALSUM);
        bene = _hermite(&o, &gradus)
            && _nova(anulus, a.columnae, a.columnae - gradus, piscina,
                &nucleus);
        per (q = ZEPHYRUM; bene && q < a.columnae - gradus; q++)
        {
            per (i = ZEPHYRUM; i < a.columnae; i++)
            {
                _effectus(anulus, _operis(&o, TABULA_U, gradus + q, i),
                    piscina, _locus(nucleus.elementa, anulus,
                    nucleus.columnae, i, q));
            }
        }
        _officinae_claudere(&o.officinae);
    }
    si (privata != piscina)
    {
        piscina_destruere(privata);
    }
    si (!bene)
    {
        redde FALSUM;
    }
    *exitus = nucleus;
    redde VERUM;
}


/* ==================================================
 * Diagnosis
 * ================================================== */

memoriae_index
matrix_apex_officinarum (
    vacuum)
{
    redde _apex_officinarum;
}
#undef _apex_notare
#undef _apex_officinarum
#undef _columnae
#undef _columnae_transforma
#undef _columnas_permuta
#undef _compacta
#undef _dividit_reliqua
#undef _effectus
#undef _est_spatium
#undef _hermite
#undef _identitas_operis
#undef _linea_normalis
#undef _lineae
#undef _lineae_transforma
#undef _lineas_permuta
#undef _locus
#undef _minima_in_columna
#undef _minima_in_linea
#undef _nova
#undef _officina_reficere
#undef _officinae_aperire
#undef _officinae_claudere
#undef _officinis_utendum
#undef _operarius_aperire
#undef _operis
#undef _operis_piscina
#undef _parvae
#undef _passus
#undef _permuta
#undef _reductio
#undef _scala
#undef _servare
#undef _sine_spatiis
#undef _summa
#undef _tabula_reddere
#undef ALVEUS_A
#undef ALVEUS_B
#undef ALVEUS_C
#undef ALVEUS_D
#undef ALVEUS_G
#undef ALVEUS_NX
#undef ALVEUS_NY
#undef ALVEUS_P
#undef ALVEUS_Q
#undef ALVEUS_T
#undef ALVEUS_X
#undef ALVEUS_Y
#undef MATRIX_LIMES_ELEMENTORUM
#undef MATRIX_LIMES_OPERUM
#undef OFFICINA_STABILIS
#undef OFFICINA_TEMPORARIA
#undef Officinae
#undef Operarius
#undef TABULA_A
#undef TABULA_U
#undef TABULA_V
#line 1 "knotapel/demo_136_tl_braid_closures/main.c"
/*
 * KNOTAPEL DEMO 136: brackets of large braid closures through
 * Temperley-Lieb standard modules
 * ============================================================
 *
 * Plan: plan.md (v2 after a design review before any data; T0 amendment).
 * The Kauffman bracket of the closure of a braid b in B_n is a weighted
 * sum of traces of b in the Temperley-Lieb standard modules W_{n,j}:
 *
 *   <closure(b)>_delta = sum_j Delta_j(delta) tr rho_j(b),
 *   Delta_0 = 1, Delta_1 = delta, Delta_{j+1} = delta Delta_j - Delta_{j-1},
 *   <closure(b)> = <closure(b)>_delta / delta          (<O> = 1),
 *   sigma_i^{+-1} -> A^{+-1} 1 + A^{-+1} e_i,  delta = -A^2 - A^-2 (conv0).
 *
 * The modules' dimensions square-sum to Catalan(n) (208,012 at n = 12)
 * instead of the vertex model's ~2^(2n) states. Checked here (plan.md):
 *
 *   R0   the test braids are the projection run's braids (bound, width)
 *   R1   TL bracket == state sum (c <= 24) and == vertex model (full)
 *   R1p  TL value == vertex value == reduced exact TL polynomial at
 *        points of a prime p* the interpolation never uses
 *   R2   link-state counts, validity, sum d^2 = Catalan(n), n <= 16
 *   R3   TL and braid relations on every basis state, n <= 16
 *   R4   V(1) = 1, V(e^(2 pi i/3)) = 1, V(i) = +-1, |V(-1)| = det
 *   R5   stabilization (back, front), index flip, conjugation, at points
 *   R6   speed per point against the vertex model (12 x 401, 12 x 801)
 *   plus conv0 (the trefoil against the state sum at points; the
 *   one-strand trivial braid against 1).
 *
 * House libraries: piscina, sors, magnus, polynomium, interpolatio,
 * congruentia, laqueus (and closure), hence latina.h: Roman numerals and
 * Latin keywords are macros here (never name a variable I, V, X, L, C, D
 * or M, or per, si, dum, ...). House i32/i64 are UNSIGNED; s32 is int.
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_136_tl_braid_closures/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * D136_MODUS=t1 runs the small set (T1; prints PARTIAL RUN); =p8 adds
 * 10 x 401 (R1p, R4: plant 8); default = the registered test set (T2);
 * =desc runs the descriptive measurements (no predictions; D136_PARS =
 * times | spans | integers | polynomials picks one). Times print only
 * with D136_TEMPORA=1 (except in desc), so archived output is stable.
 * Run under tools/custos.sh.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>











#define NMAX 16
#define WORKSPACE_MAX ((i64)0x80000000UL)   /* 2 GiB, as uncinus_mensura */

typedef i32 res;                            /* residue mod p < 2^32 */
typedef unsigned long long ull;

/* ---------- checks and verdicts ---------- */

enum {
    P_R0, P_R1, P_R1P, P_R2, P_R3, P_R4, P_R5, P_R6, P_CONV, P_NUM
};

static const char *pred_name[P_NUM] = {
    "R0", "R1", "R1p", "R2", "R3", "R4", "R5", "R6", "conv0"
};

static long pred_checks[P_NUM];
static long pred_fails[P_NUM];
static int n_pass = 0;
static int n_fail = 0;

static void
check (
    int         pred,
    const char *msg,
    int         ok)
{
    pred_checks[pred]++;
    if (ok) {
        n_pass++;
        printf("  PASS: %s: %s\n", pred_name[pred], msg);
    } else {
        n_fail++;
        pred_fails[pred]++;
        printf("  FAIL: %s: %s\n", pred_name[pred], msg);
    }
    fflush(stdout);
}

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static double
seconds_since (
    clock_t start)
{
    return (double)(clock() - start) / (double)CLOCKS_PER_SEC;
}

/* ---------- arithmetic mod p (p < 2^32) ---------- */

static res
mulm (res a, res b, res p)
{
    return (res)((ull)a * (ull)b % (ull)p);
}

static res
addm (res a, res b, res p)
{
    ull s = (ull)a + (ull)b;

    return (res)(s >= (ull)p ? s - (ull)p : s);
}

static res
subm (res a, res b, res p)
{
    return a >= b ? a - b : (res)((ull)a + (ull)p - (ull)b);
}

static res
powm (res a, ull e, res p)
{
    ull r = 1, b = (ull)a % (ull)p;

    while (e > 0) {
        if (e & 1ULL) r = r * b % (ull)p;
        b = b * b % (ull)p;
        e >>= 1;
    }
    return (res)r;
}

static res
invm (res a, res p)
{
    return powm(a, (ull)p - 2ULL, p);
}

/* ---------- link states and e_i tables ---------- */

typedef struct {
    int            n, j, d;
    unsigned int  *key;        /* sorted; 2 bits per point: 0 defect, 1 open, 2 close */
    signed char   *partner;    /* d * n; -1 = defect */
    int           *image;      /* (n - 1) * d: image index, -1 = zero */
    unsigned char *tag;        /* (n - 1) * d: 0 zero, 1 one, 2 delta */
} Module;

static Module modules[NMAX + 2][NMAX + 2];
static int modules_built[NMAX + 2];

static signed char cur[NMAX];
static int open_stack[NMAX];
static int fill_count;
static Module *fill_target;

static void
enum_rec (
    int n,
    int j,
    int k,
    int depth,
    int defects)
{
    if (k == n) {
        if (depth == 0 && defects == j) {
            if (fill_target != NULL)
                memcpy(fill_target->partner + (long)fill_count * n, cur, (size_t)n);
            fill_count++;
        }
        return;
    }
    if (depth + 1 <= n - k - 1) {                     /* open an arc */
        open_stack[depth] = k;
        enum_rec(n, j, k + 1, depth + 1, defects);
    }
    if (depth > 0) {                                  /* close the innermost arc */
        int a = open_stack[depth - 1];

        cur[a] = (signed char)k;
        cur[k] = (signed char)a;
        enum_rec(n, j, k + 1, depth - 1, defects);
        open_stack[depth - 1] = a;
    }
    if (depth == 0 && defects < j) {                  /* defect: outside every arc */
        cur[k] = -1;
        enum_rec(n, j, k + 1, depth, defects + 1);
    }
}

static unsigned int
state_key (
    const signed char *s,
    int                n)
{
    unsigned int key = 0;
    int x;

    for (x = 0; x < n; x++) {
        unsigned int code = s[x] < 0 ? 0U : (s[x] > x ? 1U : 2U);

        key |= code << (2 * x);
    }
    return key;
}

static Module *sort_module;

static int
cmp_by_key (
    const void *a,
    const void *b)
{
    unsigned int ka = state_key(sort_module->partner + (long)*(const int *)a * sort_module->n, sort_module->n);
    unsigned int kb = state_key(sort_module->partner + (long)*(const int *)b * sort_module->n, sort_module->n);

    return ka < kb ? -1 : (ka > kb ? 1 : 0);
}

static int
find_state (
    const Module *m,
    unsigned int  key)
{
    int lo = 0, hi = m->d - 1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        if (m->key[mid] == key) return mid;
        if (m->key[mid] < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

static void
build_module (
    Module *m,
    int     n,
    int     j)
{
    int *order;
    signed char *sorted;
    signed char tmp[NMAX];
    int s, i;

    m->n = n;
    m->j = j;
    fill_target = NULL;
    fill_count = 0;
    enum_rec(n, j, 0, 0, 0);
    m->d = fill_count;
    m->partner = (signed char *)malloc((size_t)(m->d > 0 ? m->d : 1) * (size_t)n);
    fill_target = m;
    fill_count = 0;
    enum_rec(n, j, 0, 0, 0);
    fill_target = NULL;

    order = (int *)malloc((size_t)(m->d > 0 ? m->d : 1) * sizeof(int));
    for (s = 0; s < m->d; s++) order[s] = s;
    sort_module = m;
    qsort(order, (size_t)m->d, sizeof(int), cmp_by_key);
    sorted = (signed char *)malloc((size_t)(m->d > 0 ? m->d : 1) * (size_t)n);
    for (s = 0; s < m->d; s++)
        memcpy(sorted + (long)s * n, m->partner + (long)order[s] * n, (size_t)n);
    free(m->partner);
    m->partner = sorted;
    free(order);
    m->key = (unsigned int *)malloc((size_t)(m->d > 0 ? m->d : 1) * sizeof(unsigned int));
    for (s = 0; s < m->d; s++) m->key[s] = state_key(m->partner + (long)s * n, n);

    m->image = (int *)malloc((size_t)((n > 1 ? n - 1 : 1) * (m->d > 0 ? m->d : 1)) * sizeof(int));
    m->tag = (unsigned char *)malloc((size_t)((n > 1 ? n - 1 : 1) * (m->d > 0 ? m->d : 1)));
    for (i = 0; i + 1 < n; i++) {
        for (s = 0; s < m->d; s++) {
            int a = i, b = i + 1, pa, pb, img = -1;
            unsigned char tag;

            memcpy(tmp, m->partner + (long)s * n, (size_t)n);
            pa = tmp[a];
            pb = tmp[b];
            if (pa == b) {                            /* (a) */
                tag = 2;
            } else if (pa >= 0 && pb >= 0) {          /* (b) */
                tmp[a] = (signed char)b;
                tmp[b] = (signed char)a;
                tmp[pa] = (signed char)pb;
                tmp[pb] = (signed char)pa;
                tag = 1;
            } else if (pa < 0 && pb < 0) {            /* (d) */
                tag = 0;
            } else {                                  /* (c) */
                int other = pa < 0 ? pb : pa;

                tmp[a] = (signed char)b;
                tmp[b] = (signed char)a;
                tmp[other] = -1;
                tag = 1;   /* case (c) */
            }
            if (tag == 2) img = s;
            else if (tag == 1) {
                img = find_state(m, state_key(tmp, n));
                if (img < 0) fatal("e_i image is not a link state of the module");
            }
            m->image[(long)i * m->d + s] = img;
            m->tag[(long)i * m->d + s] = tag;
        }
    }
}

static void
ensure_modules (
    int n)
{
    int j;

    if (n < 1 || n > NMAX) fatal("strand count out of range");
    if (modules_built[n]) return;
    for (j = n % 2; j <= n; j += 2) build_module(&modules[n][j], n, j);
    modules_built[n] = 1;
}

static long
binom (int n, int k)
{
    long r = 1;
    int t;

    if (k < 0 || k > n) return 0;
    for (t = 1; t <= k; t++) r = r * (long)(n - k + t) / (long)t;
    return r;
}

static long
dim_formula (int n, int j)
{
    if ((n - j) % 2 != 0 || j < 0 || j > n) return 0;
    return binom(n, (n - j) / 2) - binom(n, (n - j) / 2 - 1);
}

/* ---------- the TL value at a point ---------- */

static res *buf_a = NULL, *buf_b = NULL;
static long buf_size = 0;

/* bracket <closure(word)> (<O> = 1) at A mod p; *ok = 0 if delta = 0 */
static res
tl_value (
    int        n,
    const int *word,
    int        len,
    res        A,
    res        p,
    int       *ok)
{
    res Ainv = invm(A, p);
    res delta = subm(0, addm(mulm(A, A, p), mulm(Ainv, Ainv, p), p), p);
    res Dl[NMAX + 3];
    res total = 0, result;
    int j, k;

    *ok = 1;
    if (delta == 0) {
        *ok = 0;
        return 0;
    }
    ensure_modules(n);
    Dl[0] = 1;
    Dl[1] = delta;
    for (j = 1; j + 1 <= n; j++)
        Dl[j + 1] = subm(mulm(delta, Dl[j], p), Dl[j - 1], p);
    for (j = n % 2; j <= n; j += 2) {
        const Module *m = &modules[n][j];
        long d = m->d, e, s;
        res tr = 0;
        res *mat, *nb;

        if (d * d > buf_size) {
            free(buf_a);
            free(buf_b);
            buf_size = d * d;
            buf_a = (res *)malloc((size_t)buf_size * sizeof(res));
            buf_b = (res *)malloc((size_t)buf_size * sizeof(res));
            if (buf_a == NULL || buf_b == NULL) fatal("out of memory (trace buffers)");
        }
        mat = buf_a;
        nb = buf_b;
        for (e = 0; e < d * d; e++) mat[e] = 0;
        for (s = 0; s < d; s++) mat[s * d + s] = 1;
        /* M = rho(w_1) rho(w_2) ... rho(w_len): apply letters last to first */
        for (k = len - 1; k >= 0; k--) {
            int letter = word[k];
            int i = (letter > 0 ? letter : -letter) - 1;
            res c1, c2, *swap;
            const int *img = m->image + (long)i * d;
            const unsigned char *tg = m->tag + (long)i * d;

            if (letter > 0) { c1 = A; c2 = Ainv; } else { c1 = Ainv; c2 = A; }
            for (e = 0; e < d * d; e++) nb[e] = mulm(c1, mat[e], p);
            for (s = 0; s < d; s++) {
                res f;
                res *dst;
                const res *src;
                long c;

                if (tg[s] == 0) continue;
                f = tg[s] == 2 ? mulm(c2, delta, p) : c2;
                dst = nb + (long)img[s] * d;
                src = mat + s * d;
                for (c = 0; c < d; c++)
                    dst[c] = (res)(((ull)dst[c] + (ull)f * (ull)src[c]) % (ull)p);
            }
            swap = mat;
            mat = nb;
            nb = swap;
        }
        for (s = 0; s < d; s++) tr = addm(tr, mat[s * d + s], p);
        total = addm(total, mulm(Dl[j], tr, p), p);
    }
    result = mulm(total, invm(delta, p), p);
    return result;
}

/* ---------- points: the vertex model's choice (A = i omega^2) ---------- */

typedef struct {
    res p;
    res radix_i;
} PrimeCache;

static s32
choose_point (
    PrimeCache *pc,
    res         p,
    res         omega,
    res        *A)
{
    res w, winv, d;

    if (p < 5 || p % 4 != 1) return INTERPOLATIO_ERRATUM;
    if (pc->p != p) {
        res g = 2;

        if (!congruentia_est_primus(p)) return INTERPOLATIO_ERRATUM;
        while (g < p && powm(g, ((ull)p - 1ULL) / 2ULL, p) != p - 1) g++;
        if (g == p) return INTERPOLATIO_ERRATUM;
        pc->radix_i = powm(g, ((ull)p - 1ULL) / 4ULL, p);
        pc->p = p;
    }
    w = omega % p;
    if (w == 0) return INTERPOLATIO_INUTILE;
    winv = invm(w, p);
    d = addm(powm(w, 4, p), powm(winv, 4, p), p);
    if (d == 0) return INTERPOLATIO_INUTILE;
    *A = mulm(pc->radix_i, mulm(w, w, p), p);
    return INTERPOLATIO_UTILE;
}

typedef struct {
    int        n;
    const int *word;
    int        len;
    PrimeCache pc;
    res        min_p;
    long       evaluations;
} TLContext;

static s32
tl_estimator (
    void *contextus,
    i32   index,
    i32   p,
    i32  *punctum,
    i32  *valor)
{
    TLContext *ctx = (TLContext *)contextus;
    res A = 0;
    s32 st = choose_point(&ctx->pc, p, index + 2U, &A);
    int ok;

    if (st != INTERPOLATIO_UTILE) return st;
    if (ctx->min_p == 0 || p < ctx->min_p) ctx->min_p = p;
    *punctum = A;
    *valor = tl_value(ctx->n, ctx->word, ctx->len, A, p, &ok);
    ctx->evaluations++;
    return ok ? INTERPOLATIO_UTILE : INTERPOLATIO_INUTILE;
}

/* ---------- copied from tools/uncinus_mensura.c (interior there) ---------- */

static i32
_radix (
     i32* pater,
     i32  x)
{
    while (pater[x] != x)
    {
        pater[x]  = pater[pater[x]];
        x         = pater[x];
    }
    return x;
}

static s64
_modulo_quattuor (
    s64 x)
{
    s64 r = x % (s64)IV;

    return r < ZEPHYRUM ? r + (s64)IV : r;
}

static s32
_signum (
    const i32* x,
          i32  m)
{
    return x[I] == x[III] % m + I ? (s32)I : -(s32)I;
}

/* fenestra Kauffman in classe 3w, ut laqueus_modulus_vertex_creare */
static void
_fenestra (
    const i32* pd,
          i32  c,
      Piscina* piscina,
          s32* imus,
          s32* summus)
{
    i32  m     = II * c;
    i32* pater = (i32*)piscina_allocare(piscina, (memoriae_index)m
        * sizeof(i32));
    i32 ansae_a  = ZEPHYRUM;
    i32 ansae_b  = ZEPHYRUM;
    s32 w        = ZEPHYRUM;
    s32 classis;
    i32 l;
    i32 k;
    i32 status;

    for (status = ZEPHYRUM; status < II; status++)
    {
        for (l = ZEPHYRUM; l < m; l++)
        {
            pater[l] = l;
        }
        for (k = ZEPHYRUM; k < c; k++)
        {
            const i32* x = pd + IV * k;

            if (status == ZEPHYRUM)
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[I]
                    - I);
                pater[_radix(pater, x[II] - I)] = _radix(pater, x[III]
                    - I);
            }
            else
            {
                pater[_radix(pater, x[0] - I)] = _radix(pater, x[III]
                    - I);
                pater[_radix(pater, x[I] - I)] = _radix(pater, x[II]
                    - I);
            }
        }
        for (l = ZEPHYRUM; l < m; l++)
        {
            if (_radix(pater, l) == l)
            {
                if (status == ZEPHYRUM)
                {
                    ansae_a++;
                }
                else
                {
                    ansae_b++;
                }
            }
        }
    }
    for (k = ZEPHYRUM; k < c; k++)
    {
        w += _signum(pd + IV * k, m);
    }
    classis  = (s32)_modulo_quattuor((s64)III * (s64)w);
    *imus    = -(s32)c - II * (s32)ansae_b + II;
    *imus    += (s32)_modulo_quattuor((s64)classis - (s64)*imus);
    *summus  = (s32)c + II * (s32)ansae_a - II;
    *summus  -= (s32)_modulo_quattuor((s64)*summus - (s64)classis);
}

/* R0 generator: copy of _plexus_fortuitus with its one-line wrapper
 * _plexus_pd inlined, plus one output
 * parameter (the word) so the TL route can read the braid itself. */
static i32*
_plexus_fortuitus (
       Sors* s,
        i32  n,
        i32* longitudo,
    Piscina* piscina,
       s32** verbum_exitus)
{
    s32* verbum;
    i32  conatus;

    if ((*longitudo + n - I) % II != ZEPHYRUM)
    {
        (*longitudo)++;
    }
    verbum = (s32*)piscina_allocare(piscina, (memoriae_index)*longitudo
        * sizeof(s32));
    for (conatus = ZEPHYRUM; conatus < M; conatus++)
    {
        PiscinaNotatio  nota = piscina_notare(piscina);
                   i32* pd;
                   i32  transitus;
                   i32  k;

        for (k = ZEPHYRUM; k < *longitudo; k++)
        {
            s32 g = (s32)(I + sors_intra(s, n - I));

            verbum[k] = sors_intra(s, II) == ZEPHYRUM ? g : -g;
        }
        if (laqueus_pd_ex_plexu(verbum, *longitudo, n, piscina, &pd,
            &transitus))
        {
            *verbum_exitus = verbum;
            return pd;
        }
        piscina_reficere(piscina, nota);
    }
    return NULL;
}

/* ---------- polynomials ---------- */

/* sum c_k A^k mod p */
static res
poly_at (
    Polynomium f,
    res        A,
    res        p)
{
    s32 e, lo, hi;
    res Ainv = invm(A, p), acc = 0;

    if (polynomium_est_nullum(f)) return 0;
    lo = polynomium_gradus_imus(f);
    hi = polynomium_gradus_summus(f);
    for (e = lo; e <= hi; e++) {
        res c = congruentia_ex_magno(polynomium_coefficiens(f, e), p);
        res pw = e >= 0 ? powm(A, (ull)e, p) : powm(Ainv, (ull)(-e), p);

        acc = addm(acc, mulm(c, pw, p), p);
    }
    return acc;
}

static i32
max_coeff_bits (
    Polynomium f)
{
    s32 e;
    i32 b = 0;

    if (polynomium_est_nullum(f)) return 0;
    for (e = polynomium_gradus_imus(f); e <= polynomium_gradus_summus(f); e++) {
        i32 x = magnus_bitorum(polynomium_coefficiens(f, e));

        if (x > b) b = x;
    }
    return b;
}

/* ---------- colouring-matrix determinant (R4, every c) ---------- */

static res
colouring_det_mod (
    const i32 *pd,
    int        c,
    res        p)
{
    int m = 2 * c, k, r, q, n = c - 1, arcs = 0;
    i32 *parent = (i32 *)malloc((size_t)(m + 1) * sizeof(i32));
    int *index = (int *)malloc((size_t)(m + 1) * sizeof(int));
    res *A;
    res det = 1;

    for (k = 0; k <= m; k++) {
        parent[k] = (i32)k;
        index[k] = -1;
    }
    for (k = 0; k < c; k++) {
        i32 x = _radix(parent, pd[4 * k + 1]);
        i32 y = _radix(parent, pd[4 * k + 3]);

        if (x != y) parent[x] = y;
    }
    for (k = 1; k <= m; k++) {
        i32 x = _radix(parent, (i32)k);

        if (index[x] < 0) index[x] = arcs++;
    }
    if (arcs != c) fatal("colouring matrix: arc count != crossing count");
    A = (res *)calloc((size_t)c * (size_t)c, sizeof(res));
    for (k = 0; k < c; k++) {
        int over = index[_radix(parent, pd[4 * k + 1])];
        int ua = index[_radix(parent, pd[4 * k])];
        int ub = index[_radix(parent, pd[4 * k + 2])];
        res *row = A + (long)k * c;

        row[over] = addm(row[over], 2, p);
        row[ua] = subm(row[ua], 1, p);
        row[ub] = subm(row[ub], 1, p);
    }
    for (q = 0; q < n; q++) {
        int pivot = -1;
        res inv;

        for (r = q; r < n; r++)
            if (A[(long)r * c + q] != 0) { pivot = r; break; }
        if (pivot < 0) { det = 0; break; }
        if (pivot != q) {
            for (k = 0; k < n; k++) {
                res x = A[(long)q * c + k];

                A[(long)q * c + k] = A[(long)pivot * c + k];
                A[(long)pivot * c + k] = x;
            }
            det = subm(0, det, p);
        }
        det = mulm(det, A[(long)q * c + q], p);
        inv = invm(A[(long)q * c + q], p);
        for (r = q + 1; r < n; r++) {
            res f = mulm(A[(long)r * c + q], inv, p);

            if (f == 0) continue;
            for (k = q; k < n; k++)
                A[(long)r * c + k] = subm(A[(long)r * c + k], mulm(f, A[(long)q * c + k], p), p);
        }
    }
    free(parent);
    free(index);
    free(A);
    return det;
}

/* ---------- a test braid ---------- */

typedef struct {
    char       name[48];
    int        n;
    int        len;         /* crossings */
    const int *word;
    i32       *pd;
    s32        imus, summus;
    i32        limes;
    LaqueusModulusVertex *mv;
} Braid;

static int word_writhe (const int *word, int len)
{
    int k, w = 0;

    for (k = 0; k < len; k++) w += word[k] > 0 ? 1 : -1;
    return w;
}

static int
prepare_braid (
    Braid     *b,
    const char *name,
    int        n,
    const int *word,
    int        len,
    i32       *pd,
    Piscina   *piscina)
{
    strncpy(b->name, name, sizeof(b->name) - 1);
    b->name[sizeof(b->name) - 1] = 0;
    b->n = n;
    b->len = len;
    b->word = word;
    if (pd == NULL) {
        s32 *w32 = (s32 *)piscina_allocare(piscina, (memoriae_index)len * sizeof(s32));
        i32 c;
        int k;

        for (k = 0; k < len; k++) w32[k] = word[k];
        if (!laqueus_pd_ex_plexu(w32, (i32)len, (i32)n, piscina, &pd, &c)) return 0;
    }
    b->pd = pd;
    _fenestra(pd, (i32)len, piscina, &b->imus, &b->summus);
    b->mv = laqueus_modulus_vertex_creare(pd, (i32)len, WORKSPACE_MAX, piscina);
    b->limes = laqueus_modulus_vertex_limes(b->mv);
    return b->mv != NULL;
}

static int
random_braid (
    Braid   *b,
    int      n,
    int      len_req,
    Piscina *piscina)
{
    Sors s;
    i32 len = (i32)len_req;
    s32 *w32 = NULL;
    i32 *pd;
    int *word;
    int k;
    char name[48];

    sors_seminare(&s, (i64)2028, 0);
    pd = _plexus_fortuitus(&s, (i32)n, &len, piscina, &w32);
    if (pd == NULL) return 0;
    word = (int *)piscina_allocare(piscina, (memoriae_index)len * sizeof(int));
    for (k = 0; k < (int)len; k++) word[k] = w32[k];
    sprintf(name, "plexus%d x %u", n, len);
    return prepare_braid(b, name, n, word, (int)len, pd, piscina);
}

/* full TL bracket by interpolation, settings as _uncinus_per_rete */
static int
tl_bracket (
    const Braid *b,
    Piscina     *piscina,
    Polynomium  *f,
    TLContext   *ctx)
{
    InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();

    memset(ctx, 0, sizeof(*ctx));
    ctx->n = b->n;
    ctx->word = b->word;
    ctx->len = b->len;
    q.gradus_imus = b->imus;
    q.gradus_summus = b->summus;
    q.passus = IV;
    q.limes_bitorum = b->limes;
    q.residuum_primi = I;
    q.modulus_primi = IV;
    q.puncta_addita = II;
    q.conatus_maximi = (i32)((b->summus - b->imus) / IV) + I + II + LXIV;
    return interpolatio_polynomium(tl_estimator, ctx, &q, piscina, f);
}

/* ---------- R4 on an exact bracket ---------- */

static void
check_r4 (
    const Braid *b,
    Polynomium   bracket,
    Piscina     *piscina,
    int          run_alexander)
{
    int w = word_writhe(b->word, b->len);
    long bump = 0;
    Magnus S[4], T[3], v1, vm1, zero = magnus_ex_s64(0);
    s32 e, lo, hi;
    int r, bad_class = 0, first = 1;
    char msg[256];

    for (r = 0; r < 4; r++) S[r] = zero;
    for (r = 0; r < 3; r++) T[r] = zero;
    lo = polynomium_gradus_imus(bracket);
    hi = polynomium_gradus_summus(bracket);
    for (e = lo; e <= hi; e++) {
        Magnus cf = polynomium_coefficiens(bracket, e);
        long ep = (long)e - 3L * (long)w, kk;
        int k4, k3;

        if (magnus_signum(cf) == 0) continue;
        if (ep % 4 != 0) { bad_class = 1; continue; }
        if (w % 2 != 0) cf = magnus_nega(cf, piscina);     /* (-1)^w */
        if (first) {
            cf = magnus_adde(cf, magnus_ex_s64((s64)bump), piscina);
            first = 0;
        }
        kk = -ep / 4;                                       /* t = A^-4 */
        k4 = (int)(((kk % 4) + 4) % 4);
        k3 = (int)(((kk % 3) + 3) % 3);
        S[k4] = magnus_adde(S[k4], cf, piscina);
        T[k3] = magnus_adde(T[k3], cf, piscina);
    }
    sprintf(msg, "%s: Jones exponents integral (A-exponents - 3w in 4Z)", b->name);
    check(P_R4, msg, !bad_class);
    v1 = magnus_adde(magnus_adde(S[0], S[1], piscina), magnus_adde(S[2], S[3], piscina), piscina);
    vm1 = magnus_subtrahe(magnus_adde(S[0], S[2], piscina), magnus_adde(S[1], S[3], piscina), piscina);
    sprintf(msg, "%s: V(1) = 1", b->name);
    check(P_R4, msg, magnus_aequalis(v1, magnus_ex_s64(1)));
    {
        Magnus d = magnus_subtrahe(T[0], T[2], piscina);

        sprintf(msg, "%s: V(e^(2 pi i/3)) = 1 (T1 = T2, T0 - T2 = 1)", b->name);
        check(P_R4, msg, magnus_aequalis(T[1], T[2]) && magnus_aequalis(d, magnus_ex_s64(1)));
    }
    {
        Magnus d = magnus_subtrahe(S[0], S[2], piscina);

        sprintf(msg, "%s: V(i) = +-1 (S1 = S3, S0 - S2 = +-1)", b->name);
        check(P_R4, msg, magnus_aequalis(S[1], S[3])
            && (magnus_aequalis(d, magnus_ex_s64(1)) || magnus_aequalis(d, magnus_ex_s64(-1))));
    }
    /* det, route 1: laqueus Alexander (c <= 401) */
    if (run_alexander && b->len <= 401) {
        Polynomium alex;

        if (laqueus_alexander_ex_pd(b->pd, (i32)b->len, piscina, &alex)) {
            Magnus a = zero;

            for (e = polynomium_gradus_imus(alex); e <= polynomium_gradus_summus(alex); e++) {
                Magnus cf = polynomium_coefficiens(alex, e);

                if (e % 2 != 0) cf = magnus_nega(cf, piscina);
                a = magnus_adde(a, cf, piscina);
            }
            sprintf(msg, "%s: |V(-1)| = |Delta(-1)| (laqueus Alexander, %u bits)", b->name,
                magnus_bitorum(a));
            check(P_R4, msg, magnus_aequalis(magnus_absolutum(a, piscina), magnus_absolutum(vm1, piscina)));
        } else {
            sprintf(msg, "%s: laqueus Alexander refused", b->name);
            check(P_R4, msg, 0);
        }
    }
    /* det, route 2: colouring minor mod primes; V(-1)^2 == det^2 */
    {
        double need = (double)(b->len - 1) * 2.585 + 2.0;     /* log2 6^(c-1) */
        double have = 0.0;
        double vbits = 2.0 * (double)magnus_bitorum(vm1) + 2.0;
        res p = 0x7FFFFFFFU;
        int primes = 0, agree = 1;

        if (vbits > need) need = vbits;
        while (have < need) {
            res v, det;

            p = congruentia_primus_infra(p);
            v = congruentia_ex_magno(vm1, p);
            det = colouring_det_mod(b->pd, b->len, p);
            if (mulm(v, v, p) != mulm(det, det, p)) agree = 0;
            have += 30.9;
            primes++;
        }
        sprintf(msg, "%s: |V(-1)| = |colouring minor| (%d primes, %u bits)", b->name, primes,
            magnus_bitorum(vm1));
        check(P_R4, msg, agree);
    }
}

/* ---------- R5 at points ---------- */

typedef struct {
    int    n;
    int    len;
    int   *word;
} Word;

static Word
word_make (int n, int len)
{
    Word w;

    w.n = n;
    w.len = len;
    w.word = (int *)malloc((size_t)(len > 0 ? len : 1) * sizeof(int));
    return w;
}

static void
check_r5 (
    const Braid *b,
    res          p,
    int          points)
{
    PrimeCache pc;
    res *A = (res *)malloc((size_t)points * sizeof(res));
    res *v0 = (res *)malloc((size_t)points * sizeof(res));
    int got = 0, t, k, n = b->n, len = b->len, ok;
    res omega = 2;
    Word back[2], front[2], flip, conj;
    int sgn, kc = n / 2 > 0 ? n / 2 : 1;
    char msg[256];
    long bad;

    memset(&pc, 0, sizeof(pc));
    while (got < points) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) == INTERPOLATIO_UTILE) {
            v0[got] = tl_value(n, b->word, len, a, p, &ok);
            if (!ok) continue;
            A[got++] = a;
        }
    }
    for (sgn = 0; sgn < 2; sgn++) {
        int s = sgn == 0 ? 1 : -1;

        back[sgn] = word_make(n + 1, len + 1);
        for (k = 0; k < len; k++) back[sgn].word[k] = b->word[k];
        back[sgn].word[len] = s * n;
        front[sgn] = word_make(n + 1, len + 1);
        front[sgn].word[0] = s;
        for (k = 0; k < len; k++)
            front[sgn].word[k + 1] = b->word[k] > 0 ? b->word[k] + 1 : b->word[k] - 1;
    }
    flip = word_make(n, len);
    for (k = 0; k < len; k++)
        flip.word[k] = b->word[k] > 0 ? n - b->word[k] : -(n + b->word[k]);
    conj = word_make(n, len + 2);
    conj.word[0] = kc;
    for (k = 0; k < len; k++) conj.word[k + 1] = b->word[k];
    conj.word[len + 1] = -kc;

    for (sgn = 0; sgn < 2; sgn++) {
        int end;

        for (end = 0; end < 2; end++) {
            Word *wv = end == 0 ? &back[sgn] : &front[sgn];

            bad = 0;
            for (t = 0; t < points; t++) {
                res a3 = powm(A[t], 3, p);
                res factor = subm(0, sgn == 0 ? a3 : invm(a3, p), p);
                res v = tl_value(wv->n, wv->word, wv->len, A[t], p, &ok);

                if (!ok || v != mulm(factor, v0[t], p)) bad++;
            }
            sprintf(msg, "%s: stabilization at the %s, sigma^%s on %d strands: value = -A^%s3 value (%d points, %ld off)",
                b->name, end == 0 ? "back" : "front", sgn == 0 ? "+1" : "-1", n + 1,
                sgn == 0 ? "+" : "-", points, bad);
            check(P_R5, msg, bad == 0);
        }
    }
    bad = 0;
    for (t = 0; t < points; t++)
        if (tl_value(n, flip.word, len, A[t], p, &ok) != v0[t]) bad++;
    sprintf(msg, "%s: index flip sigma_i -> sigma_(n-i): same value (%d points, %ld off)", b->name, points, bad);
    check(P_R5, msg, bad == 0);
    bad = 0;
    for (t = 0; t < points; t++)
        if (tl_value(n, conj.word, len + 2, A[t], p, &ok) != v0[t]) bad++;
    sprintf(msg, "%s: conjugation by sigma_%d (smoke): same value (%d points, %ld off)", b->name, kc, points, bad);
    check(P_R5, msg, bad == 0);
    for (sgn = 0; sgn < 2; sgn++) {
        free(back[sgn].word);
        free(front[sgn].word);
    }
    free(flip.word);
    free(conj.word);
    free(A);
    free(v0);
}

/* ---------- R1p at off-interpolation points ---------- */

static void
check_r1p (
    const Braid *b,
    Polynomium   bracket,
    res          pstar,
    res          min_interp_p,
    int          points,
    double      *t_tl,
    double      *t_vx)
{
    PrimeCache pc;
    res omega = 2;
    int got = 0;
    long bad = 0, bad_point = 0;
    char msg[256];

    sprintf(msg, "%s: p* = %u below every interpolation prime (lowest used %u)", b->name, pstar, min_interp_p);
    check(P_R1P, msg, min_interp_p > pstar);
    memset(&pc, 0, sizeof(pc));
    *t_tl = 0.0;
    *t_vx = 0.0;
    while (got < points) {
        res a = 0, vt, vp;
        i32 av = 0, vv = 0;
        s32 st = choose_point(&pc, pstar, omega, &a);
        s32 sv;
        clock_t t0;
        int ok;

        if (st != INTERPOLATIO_UTILE) { omega++; continue; }
        t0 = clock();
        vt = tl_value(b->n, b->word, b->len, a, pstar, &ok);
        *t_tl += seconds_since(t0);
        t0 = clock();
        sv = laqueus_modulus_vertex_aestimare(b->mv, omega, pstar, &av, &vv);
        *t_vx += seconds_since(t0);
        vp = poly_at(bracket, a, pstar);
        if (sv != INTERPOLATIO_UTILE || av != a) bad_point++;
        else if (vt != vv || vp != vv) bad++;
        got++;
        omega++;
    }
    sprintf(msg, "%s: TL value == vertex value == reduced exact polynomial at %d points of p* (A equal: %ld off; values: %ld off)",
        b->name, points, bad_point, bad);
    check(P_R1P, msg, bad == 0 && bad_point == 0);
}

/* ---------- R2, R3 ---------- */

static void
part_r2 (void)
{
    int n, j, s, x, y;

    printf("\nR2: link states of W_{n,j}, n <= 16\n");
    for (n = 1; n <= NMAX; n++) {
        long sum_d = 0, sum_d2 = 0, bad_count = 0, bad_valid = 0, bad_order = 0;
        char msg[256];

        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2) {
            const Module *m = &modules[n][j];

            if ((long)m->d != dim_formula(n, j)) bad_count++;
            sum_d += m->d;
            sum_d2 += (long)m->d * (long)m->d;
            for (s = 0; s < m->d; s++) {
                const signed char *st = m->partner + (long)s * n;
                int defects = 0;

                if (s > 0 && !(m->key[s - 1] < m->key[s])) bad_order++;
                for (x = 0; x < n; x++) {
                    if (st[x] < 0) { defects++; continue; }
                    if (st[x] >= n || st[(int)st[x]] != x || st[x] == x) bad_valid++;
                }
                if (defects != j) bad_valid++;
                for (x = 0; x < n; x++) {
                    int a = x, bb = st[x];

                    if (bb <= a) continue;
                    for (y = a + 1; y < bb; y++) {
                        if (st[y] < 0) bad_valid++;                       /* enclosed defect */
                        else if (st[y] < a || st[y] > bb) bad_valid++;    /* crossing */
                    }
                }
            }
        }
        sprintf(msg, "n=%2d: d_{n,j} = formula for every j, distinct (%ld), valid (%ld), sum d = C(n,n/2) = %ld, sum d^2 = Catalan(n) = %ld",
            n, bad_order, bad_valid, sum_d, sum_d2);
        check(P_R2, msg, bad_count == 0 && bad_order == 0 && bad_valid == 0
            && sum_d == binom(n, n / 2) && sum_d2 == binom(2 * n, n) / (n + 1));
    }
}

/* sparse vector: up to 64 (index, coefficient) terms */
typedef struct {
    int  k;
    int  idx[64];
    res  cf[64];
} Sparse;

static void
sp_norm (Sparse *v, res p)
{
    int a, b, out = 0;

    for (a = 1; a < v->k; a++) {                      /* insertion sort by index */
        int ti = v->idx[a];
        res tc = v->cf[a];

        b = a - 1;
        while (b >= 0 && v->idx[b] > ti) {
            v->idx[b + 1] = v->idx[b];
            v->cf[b + 1] = v->cf[b];
            b--;
        }
        v->idx[b + 1] = ti;
        v->cf[b + 1] = tc;
    }
    for (a = 0; a < v->k; a++) {
        if (out > 0 && v->idx[out - 1] == v->idx[a]) v->cf[out - 1] = addm(v->cf[out - 1], v->cf[a], p);
        else { v->idx[out] = v->idx[a]; v->cf[out] = v->cf[a]; out++; }
    }
    v->k = 0;
    for (a = 0; a < out; a++)
        if (v->cf[a] != 0) { v->idx[v->k] = v->idx[a]; v->cf[v->k] = v->cf[a]; v->k++; }
}

/* apply e_i (gen = 0) or sigma_i^{+-1} (gen = +-1) */
static Sparse
sp_apply (const Module *m, int i, int gen, const Sparse *v, res A, res delta, res p)
{
    Sparse out;
    int a;
    res Ainv = invm(A, p);

    out.k = 0;
    for (a = 0; a < v->k; a++) {
        int s = v->idx[a];
        int img = m->image[(long)i * m->d + s];
        unsigned char tg = m->tag[(long)i * m->d + s];
        res ce = gen == 0 ? 1 : (gen > 0 ? Ainv : A);
        res c1 = gen > 0 ? A : Ainv;

        if (gen != 0) {
            out.idx[out.k] = s;
            out.cf[out.k] = mulm(c1, v->cf[a], p);
            out.k++;
        }
        if (tg != 0) {
            res f = tg == 2 ? mulm(ce, delta, p) : ce;

            out.idx[out.k] = img;
            out.cf[out.k] = mulm(f, v->cf[a], p);
            out.k++;
        }
        if (out.k > 60) fatal("sparse vector overflow");
    }
    sp_norm(&out, p);
    return out;
}

static int
sp_equal (const Sparse *a, const Sparse *b)
{
    int t;

    if (a->k != b->k) return 0;
    for (t = 0; t < a->k; t++)
        if (a->idx[t] != b->idx[t] || a->cf[t] != b->cf[t]) return 0;
    return 1;
}

static void
part_r3 (void)
{
    res p = 2147483629U, A = 123456789U;
    res Ainv = invm(A, p);
    res delta = subm(0, addm(mulm(A, A, p), mulm(Ainv, Ainv, p), p), p);
    int n, j, s, i, k;

    printf("\nR3: relations on every basis state of W_{n,j}, n <= 16 (mod p, A fixed)\n");
    for (n = 2; n <= NMAX; n++) {
        long bad_sq = 0, bad_tri = 0, bad_far = 0, bad_braid = 0, bad_inv = 0;
        char msg[256];

        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2) {
            const Module *m = &modules[n][j];

            for (s = 0; s < m->d; s++) {
                Sparse e1;

                e1.k = 1;
                e1.idx[0] = s;
                e1.cf[0] = 1;
                for (i = 0; i + 1 < n; i++) {
                    Sparse x = sp_apply(m, i, 0, &e1, A, delta, p), xx, ref;

                    /* e_i^2 = delta e_i */
                    xx = sp_apply(m, i, 0, &x, A, delta, p);
                    ref = x;
                    for (k = 0; k < ref.k; k++) ref.cf[k] = mulm(ref.cf[k], delta, p);
                    if (!sp_equal(&xx, &ref)) bad_sq++;
                    /* sigma_i sigma_i^-1 = 1 */
                    {
                        Sparse y = sp_apply(m, i, 1, &e1, A, delta, p);

                        y = sp_apply(m, i, -1, &y, A, delta, p);
                        if (!sp_equal(&y, &e1)) bad_inv++;
                    }
                    if (i + 2 < n) {
                        Sparse y, z;

                        /* e_i e_{i+1} e_i = e_i and e_{i+1} e_i e_{i+1} = e_{i+1} */
                        y = sp_apply(m, i, 0, &e1, A, delta, p);
                        y = sp_apply(m, i + 1, 0, &y, A, delta, p);
                        y = sp_apply(m, i, 0, &y, A, delta, p);
                        if (!sp_equal(&y, &x)) bad_tri++;
                        z = sp_apply(m, i + 1, 0, &e1, A, delta, p);
                        y = sp_apply(m, i, 0, &z, A, delta, p);
                        y = sp_apply(m, i + 1, 0, &y, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_tri++;
                        /* sigma_i sigma_{i+1} sigma_i = sigma_{i+1} sigma_i sigma_{i+1} */
                        y = sp_apply(m, i, 1, &e1, A, delta, p);
                        y = sp_apply(m, i + 1, 1, &y, A, delta, p);
                        y = sp_apply(m, i, 1, &y, A, delta, p);
                        z = sp_apply(m, i + 1, 1, &e1, A, delta, p);
                        z = sp_apply(m, i, 1, &z, A, delta, p);
                        z = sp_apply(m, i + 1, 1, &z, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_braid++;
                    }
                    for (k = i + 2; k + 1 < n; k++) {
                        Sparse y, z;

                        y = sp_apply(m, i, 0, &e1, A, delta, p);
                        y = sp_apply(m, k, 0, &y, A, delta, p);
                        z = sp_apply(m, k, 0, &e1, A, delta, p);
                        z = sp_apply(m, i, 0, &z, A, delta, p);
                        if (!sp_equal(&y, &z)) bad_far++;
                    }
                }
            }
        }
        sprintf(msg, "n=%2d: e_i^2 = delta e_i (%ld off), e_i e_(i+-1) e_i = e_i (%ld off), far commute (%ld off), braid relation (%ld off), sigma sigma^-1 = 1 (%ld off)",
            n, bad_sq, bad_tri, bad_far, bad_braid, bad_inv);
        check(P_R3, msg, bad_sq + bad_tri + bad_far + bad_braid + bad_inv == 0);
    }
}

/* ---------- conv0: trefoil against the state sum; trivial braid ---------- */

static void
part_conv (Piscina *piscina)
{
    static const int trefoil[3] = { 1, 1, 1 };
    s32 w32[3] = { 1, 1, 1 };
    i32 *pd, c, k;
    Polynomium ss;
    PrimeCache pc;
    res p = congruentia_primus_infra(0xFFFFFFFFU), omega = 2;
    int got = 0, bad = 0, ok;
    char msg[256];

    printf("\nconv0: the trefoil sigma_1^3 against the state sum at points; the trivial braid\n");
    while (p % 4 != 1) p = congruentia_primus_infra(p);
    if (!laqueus_pd_ex_plexu(w32, 3, 2, piscina, &pd, &c) || !laqueus_uncinus_ex_pd(pd, c, piscina, &ss))
        fatal("trefoil: PD or state sum refused");
    {
        s32 wpd = 0;

        for (k = 0; k < c; k++) wpd += _signum(pd + 4 * k, 2 * c);
        sprintf(msg, "trefoil: PD writhe %d = word writhe %d (sigma^+1 = positive crossing)", wpd,
            word_writhe(trefoil, 3));
        check(P_CONV, msg, wpd == word_writhe(trefoil, 3));
    }
    memset(&pc, 0, sizeof(pc));
    while (got < 8) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) != INTERPOLATIO_UTILE) continue;
        if (tl_value(2, trefoil, 3, a, p, &ok) != poly_at(ss, a, p) || !ok) bad++;
        got++;
    }
    sprintf(msg, "trefoil: TL value (conv0) == state sum at 8 points (%d off)", bad);
    check(P_CONV, msg, bad == 0);
    bad = 0;
    got = 0;
    omega = 2;
    while (got < 8) {
        res a = 0;

        if (choose_point(&pc, p, omega++, &a) != INTERPOLATIO_UTILE) continue;
        if (tl_value(1, trefoil, 0, a, p, &ok) != 1 || !ok) bad++;
        got++;
    }
    sprintf(msg, "trivial braid, one strand: value 1 at 8 points (%d off)", bad);
    check(P_CONV, msg, bad == 0);
}

/* ---------- one braid through R1, R4, R5 (and R1p, R6) ---------- */

static res pstar = 0;
static int show_times = 0;                   /* D136_TEMPORA=1 */

/* span of V = span of the bracket in A over 4 (t = A^-4) */
static void
print_span (const Braid *b, Polynomium tl)
{
    int span = (int)((polynomium_gradus_summus(tl) - polynomium_gradus_imus(tl)) / 4);

    printf("  span of V %d (c - span = %d)\n", span, b->len - span);
}

static int
vertex_full_allowed (const Braid *b)
{
    return (b->n <= 12 && b->len <= 202) || (b->n == 8 && (b->len == 401 || b->len == 801));
}

static void
run_braid (
    Braid   *b,
    Piscina *piscina,
    int      r1p_points,
    int      r6)
{
    Polynomium tl, ref;
    TLContext ctx;
    clock_t t0 = clock();
    double t_tl, t_vx;
    char msg[512];
    int have_tl, r5_points;

    printf("\n%s (n = %d, c = %d, window [%d, %d], bound %u bits)\n", b->name, b->n, b->len,
        b->imus, b->summus, b->limes);
    have_tl = tl_bracket(b, piscina, &tl, &ctx);
    if (show_times)
        printf("  TL bracket: %s, %.2f s, %ld evaluations\n", have_tl ? "ok" : "REFUSED",
            seconds_since(t0), ctx.evaluations);
    else
        printf("  TL bracket: %s, %ld evaluations\n", have_tl ? "ok" : "REFUSED", ctx.evaluations);
    if (!have_tl) {
        sprintf(msg, "%s: TL bracket by interpolation", b->name);
        check(P_R1, msg, 0);
        return;
    }
    printf("  largest coefficient %u bits (bound %u)\n", max_coeff_bits(tl), b->limes);
    print_span(b, tl);
    if (b->len <= 24) {
        if (laqueus_uncinus_ex_pd(b->pd, (i32)b->len, piscina, &ref)) {
            sprintf(msg, "%s: TL bracket == state sum", b->name);
            check(P_R1, msg, polynomium_aequalis(tl, ref));
        } else {
            sprintf(msg, "%s: state sum refused", b->name);
            check(P_R1, msg, 0);
        }
    }
    if (vertex_full_allowed(b)) {
        t0 = clock();
        if (laqueus_modulus_vertex_uncinus(b->mv, piscina, &ref)) {
            if (show_times)
                sprintf(msg, "%s: TL bracket == vertex-model bracket (vertex %.2f s)", b->name,
                    seconds_since(t0));
            else
                sprintf(msg, "%s: TL bracket == vertex-model bracket", b->name);
            check(P_R1, msg, polynomium_aequalis(tl, ref));
        } else {
            sprintf(msg, "%s: vertex-model bracket refused", b->name);
            check(P_R1, msg, 0);
        }
    }
    check_r4(b, tl, piscina, 1);
    r5_points = (int)((b->summus - b->imus) / 4) + 3;
    check_r5(b, pstar, r5_points);
    if (r1p_points > 0) {
        check_r1p(b, tl, pstar, ctx.min_p, r1p_points, &t_tl, &t_vx);
        if (show_times) printf("  R1p timing: TL %.4f s, vertex %.4f s over %d points (ratio %.1f)\n", t_tl, t_vx,
            r1p_points, t_tl > 0 ? t_vx / t_tl : 0.0);
        if (r6 > 0) {
            sprintf(msg, "%s: TL per point >= %dx faster than the vertex model (measured %.1fx)",
                b->name, r6, t_tl > 0 ? t_vx / t_tl : 0.0);
            check(P_R6, msg, t_tl > 0 && t_vx / t_tl >= (double)r6);
        }
    }
}

/* ---------- R0 ---------- */

typedef struct { int n, len; i32 limes, latitudo; } Fingerprint;

static void
part_r0 (void)
{
    static const Fingerprint fp[4] = {
        { 8, 801, 642, 16 }, { 10, 401, 334, 22 }, { 12, 401, 333, 24 }, { 12, 801, 654, 26 }
    };
    int t;

    printf("\nR0: the test braids are the projection run's braids\n");
    for (t = 0; t < 4; t++) {
        Piscina *pisc = piscina_generare_dynamicum("d136_r0", 1048576);
        Braid b;
        char msg[256];

        if (!random_braid(&b, fp[t].n, fp[t].len, pisc)) {
            sprintf(msg, "%d x %d: braid or vertex structure refused", fp[t].n, fp[t].len);
            check(P_R0, msg, 0);
        } else {
            i32 lat = laqueus_modulus_vertex_sumptus(b.mv).latitudo;

            sprintf(msg, "%s: limes %u (registered %u), latitudo %u (registered %u)", b.name,
                b.limes, fp[t].limes, lat, fp[t].latitudo);
            check(P_R0, msg, b.limes == fp[t].limes && lat == fp[t].latitudo);
        }
        piscina_destruere(pisc);
    }
}

/* ---------- descriptive (no prediction): plan.md "Descriptive" ---------- */

/* one line per bracket: name, writhe, lowest exponent, coefficients */
static void
write_bracket (FILE *out, const Braid *b, Polynomium tl, Piscina *pisc)
{
    s32 e, lo = polynomium_gradus_imus(tl), hi = polynomium_gradus_summus(tl);

    fprintf(out, "%s | n %d | c %d | writhe %d | A^%d ..", b->name, b->n, b->len,
        word_writhe(b->word, b->len), (int)lo);
    for (e = lo; e <= hi; e++) {
        chorda t = magnus_ad_chordam(polynomium_coefficiens(tl, e), pisc);

        fprintf(out, " %.*s", (int)t.mensura, (const char *)t.datum);
    }
    fprintf(out, "\n");
    fflush(out);
}

/* Spans: the TL bracket of every test braid, span of V against c.
 * D136_UNCINI=path also writes every exact bracket there;
 * D136_SOLUM="n,c" runs only that braid (c as requested). */
static void
desc_spans (void)
{
    static const int lens[4] = { 20, 50, 101, 201 };
    static const int big[5][2] = { { 8, 401 }, { 8, 801 }, { 10, 401 }, { 12, 401 }, { 12, 801 } };
    const char *path = getenv("D136_UNCINI");
    const char *solum = getenv("D136_SOLUM");
    FILE *out = path != NULL ? fopen(path, "w") : NULL;
    int n, k, solum_n = 0, solum_c = 0;

    if (path != NULL && out == NULL) fatal("cannot open D136_UNCINI");
    if (solum != NULL && sscanf(solum, "%d,%d", &solum_n, &solum_c) != 2) fatal("D136_SOLUM is n,c");
    printf("\nDescriptive: span of V against c (reduced alternating iff c - span = 0)\n");
    for (k = 0; k < 4 * 10 + 5; k++) {
        Piscina *pisc;
        Braid b;
        Polynomium tl;
        TLContext ctx;
        int len;

        n = k < 40 ? 3 + k / 4 : big[k - 40][0];
        len = k < 40 ? lens[k % 4] : big[k - 40][1];
        if (solum != NULL && (n != solum_n || len != solum_c)) continue;
        pisc = piscina_generare_dynamicum("d136_span", 1048576);
        if (!random_braid(&b, n, len, pisc) || !tl_bracket(&b, pisc, &tl, &ctx))
            printf("%d x %d: REFUSED\n", n, len);
        else {
            printf("%s: largest coefficient %u bits (bound %u)\n", b.name, max_coeff_bits(tl), b.limes);
            print_span(&b, tl);
            if (out != NULL) write_bracket(out, &b, tl, pisc);
        }
        fflush(stdout);
        piscina_destruere(pisc);
    }
    if (out != NULL) fclose(out);
}

/* Per-point time at c = 801, n = 4 ... 16 (seed 2028 per n, as R0). */
static void
desc_times (void)
{
    int n;

    printf("\nDescriptive: TL time per point at c = 801 (3 points of p*)\n");
    for (n = 4; n <= NMAX; n++) {
        Piscina *pisc = piscina_generare_dynamicum("d136_times", 1048576);
        Sors s;
        i32 len = 801;
        s32 *w32 = NULL;
        int *word;
        PrimeCache pc;
        double secs = 0;
        long catalan = binom(2 * n, n) / (n + 1), dmax = 0;
        int k, j, ok, pts = 0;

        sors_seminare(&s, (i64)2028, 0);
        if (_plexus_fortuitus(&s, (i32)n, &len, pisc, &w32) == NULL) {
            printf("  n = %2d: no knot\n", n);
            piscina_destruere(pisc);
            continue;
        }
        word = (int *)piscina_allocare(pisc, (memoriae_index)len * sizeof(int));
        for (k = 0; k < (int)len; k++) word[k] = w32[k];
        ensure_modules(n);
        for (j = n % 2; j <= n; j += 2)
            if (modules[n][j].d > dmax) dmax = modules[n][j].d;
        memset(&pc, 0, sizeof(pc));
        for (k = 0; k < 3; k++) {
            res A = 0;
            clock_t t0;

            if (choose_point(&pc, pstar, (res)(k + 2), &A) != INTERPOLATIO_UTILE) continue;
            t0 = clock();
            (void)tl_value(n, word, (int)len, A, pstar, &ok);
            secs += seconds_since(t0);
            pts++;
        }
        if (pts > 0) secs /= pts;
        printf("  n = %2d  c = %u  Catalan %9ld  largest module %5ld  %9.3f s per point  (%.2f ns per c x Catalan)\n",
            n, len, catalan, dmax, secs, secs * 1e9 / ((double)len * (double)catalan));
        fflush(stdout);
        piscina_destruere(pisc);
    }
}

/* Exact traces over Z[x], x = A^2, with no primes. Each letter is scaled
 * by A^3 so every entry stays an integer polynomial:
 *   A^3 sigma = x^2 1 + E~,   A^3 sigma^-1 = x 1 + x E~,
 *   E~ = A^2 e: coefficient x (re-pair, moved defect), -(x^2 + 1) (loop).
 * The degree rises by at most 3 per letter, so 3c + 1 coefficients.
 * terms == 1: the same at the single point A = 2 (x = 4), entries in Z;
 * there the scaling adds up to 3c bits (8^c), in Z[x] it is a shift. */

static const s64 ex_id_pos[4] = { 0, 0, 1, 0 };
static const s64 ex_id_neg[4] = { 0, 1, 0, 0 };
static const s64 ex_e_pos[3][4] = { { 0, 0, 0, 0 }, { 0, 1, 0, 0 }, { -1, 0, -1, 0 } };
static const s64 ex_e_neg[3][4] = { { 0, 0, 0, 0 }, { 0, 0, 1, 0 }, { 0, -1, 0, -1 } };

typedef struct {
    int            terms;
    i32            max_bits;     /* every intermediate entry, every module */
    i32            mark_bits[4]; /* after c/4, c/2, 3c/4, c letters */
    memoriae_index arena_peak;
    long           ops;
} ExactStats;

/* dst += f src, f a polynomial in x of degree <= 3 (point mode: f(4)).
 * magnus_adde and _multiplica always allocate the result in pisc, so
 * nothing in dst shares storage with src. */
static void
ex_axpy (
    Magnus       *dst,
    const Magnus *src,
    const s64    *f,
    int           terms,
    int           hi,
    Piscina      *pisc,
    long         *ops)
{
    int k, e;

    if (terms == 1) {
        s64 v = f[0] + 4 * f[1] + 16 * f[2] + 64 * f[3];

        if (v == 0 || magnus_signum(src[0]) == 0) return;
        dst[0] = magnus_adde(dst[0], magnus_multiplica(src[0], magnus_ex_s64(v), pisc), pisc);
        (*ops)++;
        return;
    }
    for (k = 0; k < 4; k++) {
        if (f[k] == 0) continue;
        for (e = 0; e <= hi; e++) {
            if (magnus_signum(src[e]) == 0) continue;
            if (e + k >= terms) fatal("exact trace: degree bound exceeded");
            if (f[k] == 1) dst[e + k] = magnus_adde(dst[e + k], src[e], pisc);
            else dst[e + k] = magnus_subtrahe(dst[e + k], src[e], pisc);
            (*ops)++;
        }
    }
}

/* tr[j] = the trace of A^(3c) rho_j(word), terms coefficients, in keep */
static void
exact_traces (
    int         n,
    const int  *word,
    int         len,
    Magnus    **tr,
    Piscina    *keep,
    ExactStats *st)
{
    Piscina *ar[2];
    int terms = st->terms, j, q;

    ar[0] = piscina_generare_dynamicum("d136_exact_a", 1048576);
    ar[1] = piscina_generare_dynamicum("d136_exact_b", 1048576);
    ensure_modules(n);
    for (j = n % 2; j <= n; j += 2) {
        const Module *m = &modules[n][j];
        long d = m->d, cells = d * d * terms, e, s, r, c;
        Magnus *mat = (Magnus *)malloc((size_t)cells * sizeof(Magnus));
        Magnus *nb = (Magnus *)malloc((size_t)cells * sizeof(Magnus));
        Magnus *swap, zero = magnus_ex_s64(0);
        int k, hi = 0, step = 0;

        if (mat == NULL || nb == NULL) fatal("out of memory (exact trace)");
        for (e = 0; e < cells; e++) mat[e] = zero;
        for (s = 0; s < d; s++) mat[(s * d + s) * terms] = magnus_ex_s64(1);
        for (k = len - 1; k >= 0; k--, step++) {
            int letter = word[k];
            int i = (letter > 0 ? letter : -letter) - 1;
            const int *img = m->image + (long)i * d;
            const unsigned char *tg = m->tag + (long)i * d;
            const s64 *fid = letter > 0 ? ex_id_pos : ex_id_neg;
            int nhi = terms == 1 ? 0 : hi + (letter > 0 ? 2 : 3);
            Piscina *t = ar[step % 2];
            i32 b = 0;

            if (nhi >= terms) nhi = terms - 1;
            piscina_vacare(t);
            for (e = 0; e < cells; e++) nb[e] = zero;
            for (r = 0; r < d * d; r++) ex_axpy(nb + r * terms, mat + r * terms, fid, terms, hi, t, &st->ops);
            for (s = 0; s < d; s++) {
                const s64 *fe;

                if (tg[s] == 0) continue;
                fe = letter > 0 ? ex_e_pos[tg[s]] : ex_e_neg[tg[s]];
                for (c = 0; c < d; c++)
                    ex_axpy(nb + ((long)img[s] * d + c) * terms, mat + (s * d + c) * terms, fe, terms,
                        hi, t, &st->ops);
            }
            hi = nhi;
            swap = mat;
            mat = nb;
            nb = swap;
            for (e = 0; e < cells; e++) {
                i32 x = magnus_bitorum(mat[e]);

                if (x > b) b = x;
            }
            if (b > st->max_bits) st->max_bits = b;
            for (q = 0; q < 4; q++)
                if (step + 1 == (len * (q + 1)) / 4 && b > st->mark_bits[q]) st->mark_bits[q] = b;
        }
        tr[j] = (Magnus *)piscina_allocare(keep, (memoriae_index)terms * sizeof(Magnus));
        for (e = 0; e < terms; e++) {
            Magnus acc = zero;

            for (s = 0; s < d; s++) acc = magnus_adde(acc, mat[(s * d + s) * terms + e], keep);
            tr[j][e] = acc;
        }
        free(mat);
        free(nb);
    }
    st->arena_peak = piscina_summa_apex_usus(ar[0]) + piscina_summa_apex_usus(ar[1]);
    piscina_destruere(ar[0]);
    piscina_destruere(ar[1]);
}

/* the bracket at A = 2 mod p from the exact traces */
static res
exact_at_two (
    int      n,
    int      len,
    Magnus **tr,
    int      terms,
    res      p)
{
    res Ainv = invm(2, p);
    res delta = subm(0, addm(4, mulm(Ainv, Ainv, p), p), p);
    res Dl[NMAX + 3], total = 0;
    int j, e;

    Dl[0] = 1;
    Dl[1] = delta;
    for (j = 1; j + 1 <= n; j++) Dl[j + 1] = subm(mulm(delta, Dl[j], p), Dl[j - 1], p);
    for (j = n % 2; j <= n; j += 2) {
        res v = 0, xe = 1;

        for (e = 0; e < terms; e++) {
            v = addm(v, mulm(magnus_residuum_parvum(tr[j][e], p), xe, p), p);
            xe = mulm(xe, 4, p);
        }
        total = addm(total, mulm(Dl[j], v, p), p);
    }
    total = mulm(total, powm(Ainv, 3ULL * (ull)len, p), p);
    return mulm(total, invm(delta, p), p);
}

static void
desc_exact (int n, int len_req, int poly)
{
    Piscina *pisc = piscina_generare_dynamicum("d136_exact", 1048576);
    Braid b;
    Magnus *tr[NMAX + 1];
    ExactStats st;
    clock_t t0;
    i32 trbits = 0;
    res mine, ref;
    int j, e, ok;

    if (!random_braid(&b, n, len_req, pisc)) {
        printf("  %d x %d: REFUSED\n", n, len_req);
        piscina_destruere(pisc);
        return;
    }
    memset(&st, 0, sizeof(st));
    st.terms = poly ? 3 * b.len + 1 : 1;
    t0 = clock();
    exact_traces(n, b.word, b.len, tr, pisc, &st);
    for (j = n % 2; j <= n; j += 2)
        for (e = 0; e < st.terms; e++) {
            i32 x = magnus_bitorum(tr[j][e]);

            if (x > trbits) trbits = x;
        }
    mine = exact_at_two(n, b.len, tr, st.terms, pstar);
    ref = tl_value(n, b.word, b.len, 2, pstar, &ok);
    printf("  %s: entries <= %u bits (after c/4, c/2, 3c/4, c: %u, %u, %u, %u); traces %u bits\n",
        b.name, st.max_bits, st.mark_bits[0], st.mark_bits[1], st.mark_bits[2], st.mark_bits[3], trbits);
    printf("    %ld big-number operations, arenas peak %.1f MB, %.1f s; at A = 2 mod p*: %s\n",
        st.ops, (double)st.arena_peak / 1048576.0, seconds_since(t0),
        ok && mine == ref ? "agrees with the modular route" : "DISAGREES");
    if (poly) {
        Polynomium tl;
        TLContext ctx;

        if (tl_bracket(&b, pisc, &tl, &ctx))
            printf("    bracket coefficients %u bits (bound %u)\n", max_coeff_bits(tl), b.limes);
    } else printf("    (scaling 8^c adds up to %d bits)\n", 3 * b.len);
    fflush(stdout);
    piscina_destruere(pisc);
}

static void
desc_integers (void)
{
    static const int set[7][2] = { { 4, 201 }, { 8, 201 }, { 12, 201 }, { 8, 401 }, { 8, 801 }, { 12, 401 }, { 12, 801 } };
    int k;

    printf("\nDescriptive: largest entry over Z at A = 2 (no primes)\n");
    for (k = 0; k < 7; k++) desc_exact(set[k][0], set[k][1], 0);
}

static void
desc_polynomials (void)
{
    /* the last two go past the plan's c <= 201 (cheap at 8 strands) */
    static const int set[15][2] = {
        { 4, 50 }, { 4, 101 }, { 4, 201 }, { 6, 50 }, { 6, 101 }, { 6, 201 },
        { 8, 50 }, { 8, 101 }, { 8, 201 }, { 10, 50 }, { 10, 101 }, { 12, 101 }, { 12, 201 },
        { 8, 401 }, { 8, 801 }
    };
    int k;

    printf("\nDescriptive: largest coefficient over Z[A] (no primes), c <= 201 (+ 8 x 401, 8 x 801)\n");
    for (k = 0; k < 15; k++) desc_exact(set[k][0], set[k][1], 1);
}

static void
run_descriptive (const char *pars)
{
    int all = pars == NULL;

    if (all || strcmp(pars, "times") == 0) desc_times();
    if (all || strcmp(pars, "spans") == 0) desc_spans();
    if (all || strcmp(pars, "integers") == 0) desc_integers();
    if (all || strcmp(pars, "polynomials") == 0) desc_polynomials();
}

/* ---------- main ---------- */

static void
run_named (const char *name, int n, const int *word, int len)
{
    Piscina *pisc = piscina_generare_dynamicum("d136", 1048576);
    Braid b;

    if (!prepare_braid(&b, name, n, word, len, NULL, pisc)) {
        char msg[128];

        sprintf(msg, "%s: PD or vertex structure refused", name);
        check(P_R1, msg, 0);
    } else run_braid(&b, pisc, 0, 0);
    piscina_destruere(pisc);
}

static void
run_random (int n, int len_req, int r1p_points, int r6)
{
    Piscina *pisc = piscina_generare_dynamicum("d136", 1048576);
    Braid b;

    if (!random_braid(&b, n, len_req, pisc)) {
        char msg[128];

        sprintf(msg, "%d x %d: no knot or vertex structure refused", n, len_req);
        check(P_R1, msg, 0);
    } else run_braid(&b, pisc, r1p_points, r6);
    piscina_destruere(pisc);
}

int
main (void)
{
    static const int w31[3] = { 1, 1, 1 };
    static const int w41[4] = { 1, -2, 1, -2 };
    static int t34[8], t45[15];
    const char *modus = getenv("D136_MODUS");
    int full = modus == NULL || strcmp(modus, "plene") == 0;
    int desc = modus != NULL && strcmp(modus, "desc") == 0;
    int p8 = modus != NULL && strcmp(modus, "p8") == 0;
    Piscina *pisc = piscina_generare_dynamicum("d136_main", 1048576);
    int k, n, pred;

    for (k = 0; k < 8; k++) t34[k] = k % 2 + 1;
    for (k = 0; k < 15; k++) t45[k] = k % 3 + 1;
    printf("KNOTAPEL DEMO 136: brackets of braid closures through TL standard modules\n");
    printf("mode: %s\n", full ? "registered test set (plan.md v2)" : desc ? "descriptive (no predictions)" :
        (p8 ? "p8 (t1 + 10 x 401)" : "t1 (PARTIAL RUN)"));
    show_times = getenv("D136_TEMPORA") != NULL;

    pstar = 1U << 30;
    do pstar = congruentia_primus_infra(pstar); while (pstar % 4 != 1);
    printf("p* = %u (largest prime = 1 mod 4 below 2^30)\n", pstar);
    if (desc) {
        run_descriptive(getenv("D136_PARS"));
        piscina_destruere(pisc);
        return 0;
    }

    part_r2();
    part_r3();
    part_conv(pisc);
    part_r0();

    run_named("3_1 = sigma_1^3", 2, w31, 3);
    run_named("4_1 = s1 s2^-1 s1 s2^-1", 3, w41, 4);
    run_named("T(3,4) = (s1 s2)^4", 3, t34, 8);
    run_named("T(4,5) = (s1 s2 s3)^5", 4, t45, 15);

    if (full) {
        static const int lens[4] = { 20, 50, 101, 201 };

        for (n = 3; n <= 12; n++)
            for (k = 0; k < 4; k++) run_random(n, lens[k], 0, 0);
        run_random(8, 401, 0, 0);
        run_random(8, 801, 0, 0);
        run_random(10, 401, 16, 0);
        run_random(12, 401, 8, 25);
        run_random(12, 801, 4, 100);
    } else {
        for (n = 3; n <= 6; n++) {
            run_random(n, 20, 0, 0);
            run_random(n, 50, 0, 0);
        }
        run_random(8, 101, 8, 0);                     /* R1p smoke */
        if (p8) run_random(10, 401, 16, 0);
    }

    printf("\nVerdicts (plan.md v2):\n");
    for (pred = 0; pred < P_NUM; pred++) {
        const char *state;

        if (pred_checks[pred] == 0) state = "NOT TESTED";
        else if (pred_fails[pred] != 0) state = "FALLS";
        else if (!full) state = "holds so far (PARTIAL RUN)";
        else state = "HOLDS";
        printf("  %-6s %s (%ld checks, %ld failed)\n", pred_name[pred], state,
            pred_checks[pred], pred_fails[pred]);
    }
    printf("\n%d passed, %d failed\n", n_pass, n_fail);
    piscina_destruere(pisc);
    return n_fail == 0 ? 0 : 1;
}
