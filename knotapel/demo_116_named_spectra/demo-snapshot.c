/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_116_named_spectra/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): 94c5d5d74635e5aa4a456c69f75ee0bb1ace6df1
 * Regenerate: ./knotapel/archive.sh knotapel/demo_116_named_spectra/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   d571eb3cef9e2aab290dfe4182c94036648fc98f  include/anulus.h
 *   7db315706b013efbb850928c78e1c13b0fb72362  include/chorda.h
 *   6f9b7a043cebe2912c612139453b66fcb04770cb  include/chorda_aedificator.h
 *   37b6fb1c16e76827120976705e418e11c89406a6  include/congruentia.h
 *   53645f652dd16a7a8c8ad79df9e2e70289ee5e52  include/fractio.h
 *   e1804770abd50053442c4d8f3ec6c02012e43bc9  include/laqueus.h
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   f0c8e680438f15b7e1fea4de9790bae94f9c1bba  include/magnus.h
 *   423a176668f285b2661452e215cdf84259389d93  include/matrix.h
 *   cd2db07dbd9f7bb6f71ffaa27b03f7cdb8ea65ee  include/piscina.h
 *   afa58ee57023d5aba2685ba7d6315f7245fdd73c  include/polynomium.h
 *   a34b9f2536efa81a09cea36f9a9acba28fb4b2fc  include/postulata_posix.h
 *   161daa5e9105613138e1126678662a71120abb84  include/situs.h
 *   5f3241baa796dc857d0d71602977618fdfd08f72  include/tabula_nodorum.h
 *   82a71ee84971f5909227b255a9303b4129eb0487  lib/anulus.c
 *   b4c8c649c03e84eca53408282c39d5b0e24c6016  lib/chorda.c
 *   ee055a36d7e4726ad5b3731e2ca64e62e93d084c  lib/chorda_aedificator.c
 *   c446f3092b17cdca38b584886d86a5ca80856eac  lib/congruentia.c
 *   61ec3b3106345cce9ea64477aafe7d1072030be7  lib/fractio.c
 *   f9469ae1e33a342ae8d9c4b77cf170fec719a674  lib/laqueus.c
 *   41efe7184a1027d6fb4b9c2a576dcb92ddf5c9b8  lib/magnus.c
 *   5fc06983f571b6cf493cc884ab9647db9f499d44  lib/matrix.c
 *   c6ab1e19274a3b36ff5cfdde651e45d079905b56  lib/piscina.c
 *   673a0b2c3f9258626883b6ecaed2ff76e4d060a8  lib/polynomium.c
 *   216c408f5f2064e64312685d551841f3cabdc92f  lib/situs.c
 *   3efb457d4592611775994befdfc3e8938c406de7  lib/tabula_nodorum.c
 *   8af4e457aef16eb25e9a370e5c3bf883c153c18d  lib/tabula_nodorum_data.c
 *   119dc962d76ad057f03b6efadd18e43495e4cb01  knotapel/demo_116_named_spectra/main.c (uncommitted, embedded verbatim)
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
#line 1 "include/tabula_nodorum.h"
/* tabula_nodorum.h - Tabula nodorum primorum (usque ad X transitus) et
 * agnitio per polynomia Alexander et Jones
 *
 * FONS: KnotInfo (C. Livingston, A. H. Moore, "KnotInfo: Table of Knot
 * Invariants", knotinfo.math.indiana.edu), per instantaneum
 * database_knotinfo 2026.10.5 (github.com/soehms/database_knotinfo).
 * Ex eo SOLUM nomina, numeri transituum, symmetria et codices PD
 * sumuntur (probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv); polynomia
 * Alexander et Jones per laqueus ex codicibus PD COMPUTANTUR (cum
 * columnis KnotInfo in extractione collata).
 *
 * CHIRALITAS: nomen "K" = diagramma tabulae (codex PD KnotInfo); "K*" =
 * eius speculum. Conventio KnotInfo = conventio physica laqueus: 3_1
 * tabulae est trifolium DEXTRUM (Jones -t^4 + t^3 + t). Knot Atlas
 * diagramma alterum pro quibusdam nodis pingit (3_1 sinistrum) -
 * tabulae chiralitate per nodum differunt; nomina hic semper ad codicem
 * PD referuntur.
 *
 * AGNITIO: congruentia Alexander ET Jones - non probatio typi (5_1
 * et 10_132* utrumque communicant: ambo redduntur). Nodus chiralis
 * cuius Jones symmetricus est (9_42, 10_48, 10_71, 10_91, 10_104,
 * 10_125) BIS redditur, K et K*: Jones chiralitatem non videt.
 * Compositi DUORUM nodorum non trivialium tabulae (Alexander et Jones
 * multiplicativi) quoque quaeruntur, speculo cuiusque factoris, sed
 * orientatione summandorum neglecta (K1 # K2 et K1 # rev K2 hic idem).
 *
 * USUS:
 *   TabulaNodorum* t = tabula_nodorum_aperire(piscina);
 *   Agnitio a[VIII];
 *   i32 n = tabula_nodorum_agnoscere(t, alexander, jones, piscina, a,
 *       VIII);
 *   ... a[0].primus->titulus, a[0].primus_speculum ...
 */
/* <aedilis corpus="lib/tabula_nodorum.c"/> */
/* <aedilis corpus="lib/tabula_nodorum_data.c"/> */
#ifndef TABULA_NODORUM_H
#define TABULA_NODORUM_H





/* symmetria (KnotInfo "symmetry type"): NULLA = nodus trivialis;
 * REVERSIBILIS = invertibilis, chiralis; CHIRALIS = nec invertibilis
 * nec amphichiralis */
#define TABULA_NODORUM_NULLA                  ZEPHYRUM
#define TABULA_NODORUM_REVERSIBILIS           I
#define TABULA_NODORUM_CHIRALIS               II
#define TABULA_NODORUM_AMPHICHIRALIS_PLENA    III
#define TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA IV
#define TABULA_NODORUM_AMPHICHIRALIS_POSITIVA V

nomen structura {
    constans character* titulus;      /* "3_1", "10_132" */
                   i32  transitus;    /* numerus transituum */
                   i32  symmetria;    /* TABULA_NODORUM_* */
                   i32  initium_pd;   /* in TABULA_NODORUM_PD */
    constans character* alexander;    /* forma normalis laqueus */
    constans character* jones;        /* diagrammatis tabulae */
} NodusTabulae;

/* data generata (lib/tabula_nodorum_data.c) */
extern constans NodusTabulae TABULA_NODORUM[];
extern constans i32          TABULA_NODORUM_NUMERUS;
extern constans i32          TABULA_NODORUM_PD[];
extern constans character*   TABULA_NODORUM_FONS;   /* provenientia */

i32
tabula_nodorum_numerus (vacuum);

/* NIHIL si i extra fines */
constans NodusTabulae*
tabula_nodorum_nodus (
    i32 i);

/* per titulum ("8_20"); NIHIL si ignotum */
constans NodusTabulae*
tabula_nodorum_quaere (
    constans character* titulus);

/* codex PD nodi (4 per transitum; NIHIL pro nodo triviali) */
constans i32*
tabula_nodorum_pd (
    constans NodusTabulae* n);

/* speculum nodus ipse est (amphichiralis) */
b32
tabula_nodorum_amphichiralis (
    constans NodusTabulae* n);

/* tabula parata: polynomia semel lecta (in piscina) */
nomen structura TabulaNodorum TabulaNodorum;

TabulaNodorum*
tabula_nodorum_aperire (
    Piscina* piscina);

/* candidatus: nodus primus (secundus NIHIL) aut compositum primus #
 * secundus; speculum = diagramma tabulae speculatum */
nomen structura {
    constans NodusTabulae* primus;
                      b32  primus_speculum;
    constans NodusTabulae* secundus;
                      b32  secundus_speculum;
} Agnitio;

/* omnes candidati quorum Alexander (forma normalis) et Jones datis
 * aequales sunt: primi, deinde compositi duorum (ordine tabulae, primus
 * <= secundus). Amphichiralis semel (speculum FALSUM). Scribit usque ad
 * maximus in exitus; reddit numerum TOTUM (potest > maximus). */
i32
tabula_nodorum_agnoscere (
    constans TabulaNodorum* tabula,
                Polynomium  alexander,
                Polynomium  jones,
                   Piscina* piscina,
                   Agnitio* exitus,
                       i32  maximus);

#endif /* TABULA_NODORUM_H */
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
/* lib/piscina.c: statica per plagulam renominata */
#define Alveus Alveus_piscina
#define _allocare_interna _allocare_interna_piscina
#define _alveus_destruere _alveus_destruere_piscina
#define _alveus_nova _alveus_nova_piscina
#define _catena_alveus_destruere _catena_alveus_destruere_piscina
#define _catena_alveus_vacare _catena_alveus_vacare_piscina
#define _debug_imprimere _debug_imprimere_piscina
#define _proxima_ordinatio _proxima_ordinatio_piscina
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

interior memoriae_index
_proxima_ordinatio (
        memoriae_index ptr,
        memoriae_index ordinatio)
{
    memoriae_index ordinatus = ptr + (ordinatio - I);
    redde ordinatus - (ordinatus % ordinatio);
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

    ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
        ordinatio);
    necessaria = ordinatus_offset + mensura;

    /* Si allocatio in alveum nunc non capit, invenire vel generare alveum novum */
    dum (necessaria > piscina->nunc->capacitas)
    {
        si (piscina->nunc->sequens)
        {
            /* Transire ad alveum sequentem */
            piscina->nunc = piscina->nunc->sequens;
            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
            necessaria = ordinatus_offset + mensura;
        }
        alioquin si (piscina->est_dynamicum)
        {
            Alveus* alveus_novum;

            /* Generare alveum novum */
            memoriae_index capacitas_nova =
                piscina->mensura_alvei_initia * II;

            /* Petitio maior quam duplum: alveus ad mensuram petitionis
             * (+ basis), BASIS INTACTA. Olim basis ad hanc mensuram
             * ratchetabatur et numquam decrescebat: lib/stml.c alvei
             * 1, 2, 3, 6, 9, 18, 27, 54 MB, ultimo 54 MB VII tenente -
             * XXXIX% otiosum (RP §6, 2026-09-02). */
            si (necessaria > capacitas_nova)
            {
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

            ordinatus_offset = _proxima_ordinatio(piscina->nunc->offset,
                ordinatio);
            necessaria = ordinatus_offset + mensura;

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
#undef Alveus
#undef PISCINA_DEBUG
#undef PISCINA_OCTETUS_VENENI
#undef PISCINA_VENENUM
#undef _allocare_interna
#undef _alveus_destruere
#undef _alveus_nova
#undef _catena_alveus_destruere
#undef _catena_alveus_vacare
#undef _debug_imprimere
#undef _proxima_ordinatio
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
#undef CHORDA_FRIATUM_OFFSET
#undef CHORDA_FRIATUM_PRIMUS
#undef _chorda_nullum_habet
#undef _extrahere_verba
/* lib/magnus.c: statica per plagulam renominata */
#define MagnusAlternae MagnusAlternae_magnus
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
#undef MAGNUS_BASIS
#undef MAGNUS_FACTOR_PARVUS
#undef MAGNUS_FRUSTUM
#undef MAGNUS_LIMES_ALTERNARUM
#undef MAGNUS_LIMES_NEGATIVUS
#undef MAGNUS_S64_IMUS
#undef MAGNUS_S64_SUMMUS
#undef MagnusAlternae
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
/* lib/polynomium.c: statica per plagulam renominata */
#define Officinae Officinae_polynomium
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
#define _servare _servare_polynomium
#define _summa _summa_polynomium
#define _summus _summus_polynomium
#define _transili _transili_polynomium
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
#undef Officinae
#undef POLYNOMIUM_LIMES_OFFICINARUM
#undef POLYNOMIUM_S64_SUMMUS
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
#undef _servare
#undef _summa
#undef _summus
#undef _transili
/* lib/laqueus.c: statica per plagulam renominata */
#define _alexander_ex_arcubus _alexander_ex_arcubus_laqueus
#define _collinearia _collinearia_laqueus
#define _componens _componens_laqueus
#define _est_spatium _est_spatium_laqueus
#define _jones_ex_uncino _jones_ex_uncino_laqueus
#define _laqueus_cum_vertice _laqueus_cum_vertice_laqueus
#define _ora_diagrammatis _ora_diagrammatis_laqueus
#define _pd_legere _pd_legere_laqueus
#define _pd_motus _pd_motus_laqueus
#define _pd_planus _pd_planus_laqueus
#define _pd_renumerare _pd_renumerare_laqueus
#define _positio_sequens _positio_sequens_laqueus
#define _prior _prior_laqueus
#define _punctum_legere _punctum_legere_laqueus
#define _radix _radix_laqueus
#define _sequens _sequens_laqueus
#define _sine_spatiis _sine_spatiis_laqueus
#define _uncinus_ex_oris _uncinus_ex_oris_laqueus
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
#undef _alexander_ex_arcubus
#undef _collinearia
#undef _componens
#undef _est_spatium
#undef _jones_ex_uncino
#undef _laqueus_cum_vertice
#undef _ora_diagrammatis
#undef _pd_legere
#undef _pd_motus
#undef _pd_planus
#undef _pd_renumerare
#undef _positio_sequens
#undef _prior
#undef _punctum_legere
#undef _radix
#undef _sequens
#undef _sine_spatiis
#undef _uncinus_ex_oris
/* lib/tabula_nodorum.c: statica per plagulam renominata */
#define _jones _jones_tabula_nodorum
#define _notare _notare_tabula_nodorum
#line 1 "lib/tabula_nodorum.c"
/* tabula_nodorum.c - tabula nodorum et agnitio (vide
 * include/tabula_nodorum.h; data in lib/tabula_nodorum_data.c,
 * GENERATA)
 */


#include <string.h>

structura TabulaNodorum {
           i32  numerus;
    Polynomium* alexander;       /* forma normalis */
    Polynomium* jones;
    Polynomium* jones_speculum;  /* J(1/t) */
           s32* amplitudo;       /* summus - imus Alexander */
};

i32
tabula_nodorum_numerus (vacuum)
{
    redde TABULA_NODORUM_NUMERUS;
}

constans NodusTabulae*
tabula_nodorum_nodus (
    i32 i)
{
    si (i >= TABULA_NODORUM_NUMERUS)
    {
        redde NIHIL;
    }
    redde &TABULA_NODORUM[i];
}

constans NodusTabulae*
tabula_nodorum_quaere (
    constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < TABULA_NODORUM_NUMERUS; k++)
    {
        si (strcmp(TABULA_NODORUM[k].titulus, titulus) == ZEPHYRUM)
        {
            redde &TABULA_NODORUM[k];
        }
    }
    redde NIHIL;
}

constans i32*
tabula_nodorum_pd (
    constans NodusTabulae* n)
{
    si (n->transitus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde TABULA_NODORUM_PD + n->initium_pd;
}

b32
tabula_nodorum_amphichiralis (
    constans NodusTabulae* n)
{
    redde n->symmetria != TABULA_NODORUM_REVERSIBILIS
        && n->symmetria != TABULA_NODORUM_CHIRALIS;
}

TabulaNodorum*
tabula_nodorum_aperire (
    Piscina* piscina)
{
    TabulaNodorum* t = (TabulaNodorum*)piscina_allocare(piscina,
        magnitudo(TabulaNodorum));
              i32 n = TABULA_NODORUM_NUMERUS;
              i32 k;

    t->numerus         = n;
    t->alexander       = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->jones           = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->jones_speculum  = (Polynomium*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(Polynomium));
    t->amplitudo       = (s32*)piscina_allocare(piscina,
        (memoriae_index)n
        * magnitudo(s32));
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (   !polynomium_ex_chorda(chorda_ex_literis(
                TABULA_NODORUM[k].alexander, piscina), 't', piscina,
                &t->alexander[k])
            || !polynomium_ex_chorda(chorda_ex_literis(
                TABULA_NODORUM[k].jones, piscina), 't', piscina,
                &t->jones[k]))
        {
            redde NIHIL;
        }
        t->jones_speculum[k]  = polynomium_inversum(t->jones[k],
            piscina);
        t->amplitudo[k]       =
            polynomium_gradus_summus(t->alexander[k])
            - polynomium_gradus_imus(t->alexander[k]);
    }
    redde t;
}

/* candidatus numeratur; scribitur si locus restat */
interior vacuum
_notare (
    Agnitio* exitus,
        i32  maximus,
        i32* numerus,
        i32  primus,
        b32  primus_speculum,
        i32  secundus,
        b32  secundus_speculum)
{
    si (*numerus < maximus)
    {
        Agnitio* a = &exitus[*numerus];

        a->primus           = &TABULA_NODORUM[primus];
        a->primus_speculum  = primus_speculum;
        a->secundus           = secundus
            == TABULA_NODORUM_NUMERUS ? NIHIL
            : &TABULA_NODORUM[secundus];
        a->secundus_speculum  = secundus_speculum;
    }
    (*numerus)++;
}

/* Jones nodi k: diagramma tabulae aut speculum eius */
interior Polynomium
_jones (
    constans TabulaNodorum* t,
                       i32  k,
                       b32  speculum)
{
    redde speculum ? t->jones_speculum[k] : t->jones[k];
}

i32
tabula_nodorum_agnoscere (
    constans TabulaNodorum* tabula,
                Polynomium  alexander,
                Polynomium  jones,
                   Piscina* piscina,
                   Agnitio* exitus,
                       i32  maximus)
{
    PiscinaNotatio nota      = piscina_notare(piscina);
        Polynomium normalis  = polynomium_nullum();
               i32 numerus   = ZEPHYRUM;
               i32 i;
               i32 j;
               s32 amplitudo;

    si (!polynomium_normale(alexander, piscina, &normalis))
    {
        piscina_reficere(piscina, nota);
        redde ZEPHYRUM;
    }
    amplitudo = polynomium_gradus_summus(normalis)
        - polynomium_gradus_imus(normalis);
    /* primi: ut picti, deinde speculum (amphichiralis semel) */
    per (i = ZEPHYRUM; i < tabula->numerus; i++)
    {
        si (!polynomium_aequalis(tabula->alexander[i], normalis))
        {
            perge;
        }
        si (polynomium_aequalis(tabula->jones[i], jones))
        {
            _notare(exitus, maximus, &numerus, i, FALSUM,
                TABULA_NODORUM_NUMERUS, FALSUM);
        }
        si (   !tabula_nodorum_amphichiralis(&TABULA_NODORUM[i])
            && polynomium_aequalis(tabula->jones_speculum[i], jones))
        {
            _notare(exitus, maximus, &numerus, i, VERUM,
                TABULA_NODORUM_NUMERUS, FALSUM);
        }
    }
    /* compositi duorum non trivialium: amplitudines Alexander
     * summantur, deinde producta exacta */
    per (i = ZEPHYRUM; i < tabula->numerus; i++)
    {
        si (   TABULA_NODORUM[i].transitus == ZEPHYRUM
            || tabula->amplitudo[i]        >= amplitudo)
        {
            perge;
        }
        per (j = i; j < tabula->numerus; j++)
        {
            PiscinaNotatio nota_par;
                Polynomium productum = polynomium_nullum();
                       b32 si_speculum;
                       b32 sj_speculum;

            si (   TABULA_NODORUM[j].transitus == ZEPHYRUM
                || tabula->amplitudo[i] + tabula->amplitudo[j]
                    != amplitudo)
            {
                perge;
            }
            nota_par = piscina_notare(piscina);
            si (   !polynomium_multiplica(tabula->alexander[i],
                    tabula->alexander[j], piscina, &productum)
                || !polynomium_aequalis(productum, normalis))
            {
                piscina_reficere(piscina, nota_par);
                perge;
            }
            per (si_speculum = FALSUM; si_speculum
                <= VERUM; si_speculum++)
            {
                si (   si_speculum
                    && tabula_nodorum_amphichiralis(&TABULA_NODORUM[i]))
                {
                    frange;
                }
                per (sj_speculum = FALSUM; sj_speculum <= VERUM;
                    sj_speculum++)
                {
                    si (   sj_speculum
                        && tabula_nodorum_amphichiralis(
                            &TABULA_NODORUM[j]))
                    {
                        frange;
                    }
                    /* i == j: (speculum, non) = (non, speculum) */
                    si (i == j && sj_speculum < si_speculum)
                    {
                        perge;
                    }
                    si (   polynomium_multiplica(_jones(tabula, i,
                        si_speculum),
                            _jones(tabula, j, sj_speculum), piscina,
                            &productum)
                        && polynomium_aequalis(productum, jones))
                    {
                        _notare(exitus, maximus, &numerus, i,
                            si_speculum,
                            j, sj_speculum);
                    }
                }
            }
            piscina_reficere(piscina, nota_par);
        }
    }
    piscina_reficere(piscina, nota);
    redde numerus;
}
#undef _jones
#undef _notare
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

    /* Appendix vacua bona est */
    si (!aedificator || !datum || mensura == ZEPHYRUM)
    {
        redde mensura == ZEPHYRUM;
    }

    necessaria = aedificator->offset + mensura;

    si (necessaria > aedificator->capacitas)
    {
        si (!_crescere(aedificator, necessaria)) redde FALSUM;
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
/* lib/anulus.c: statica per plagulam renominata */
#define FORMA_RESIDUORUM FORMA_RESIDUORUM_anulus
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
#define _z_subtrahe _z_subtrahe_anulus
#define _z_transcribe _z_transcribe_anulus
#define _z_unum _z_unum_anulus
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

constans Anulus ANULUS_INTEGRORUM = {
    "Z", (memoriae_index)magnitudo(Magnus), FALSUM,
    _z_nullum, _z_unum, _z_est_nullum, _z_parvum, _z_aequalis, _z_adde,
        _z_subtrahe,
    _z_multiplica, _z_divide_exacte, _z_transcribe, _z_ad_chordam,
    _z_ex_chorda, _z_divisor_communis, _z_divide_cum_residuo,
    _z_compara_normam, NIHIL, VERUM
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

constans Anulus ANULUS_RATIONALIUM = {
    "Q", (memoriae_index)magnitudo(Fractio), VERUM,
    _q_nullum, _q_unum, _q_est_nullum, _q_parvum, _q_aequalis, _q_adde,
        _q_subtrahe,
    _q_multiplica, _q_divide_exacte, _q_transcribe, _q_ad_chordam,
    _q_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM
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
    _p_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, VERUM
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
    _r_ad_chordam, _r_ex_chorda, NIHIL, NIHIL, NIHIL, NIHIL, FALSUM
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
#undef FORMA_RESIDUORUM
#undef VALOR_P
#undef VALOR_Q
#undef VALOR_R
#undef VALOR_Z
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
#undef _z_subtrahe
#undef _z_transcribe
#undef _z_unum
/* lib/matrix.c: statica per plagulam renominata */
#define Officinae Officinae_matrix
#define Operarius Operarius_matrix
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
#line 1 "lib/tabula_nodorum_data.c"
/* GENERATUM: tools/tabula_nodorum_generare.sh - NE EDITA MANU
 *
 * Ex probationes/fixa/knotinfo/2026.10.5/nodi_x.tsv:
 * nomina, transitus, symmetria et codices PD ex KnotInfo
 * (C. Livingston, A. H. Moore, knotinfo.math.indiana.edu) per
 * database_knotinfo 2026.10.5, knotinfo_data_complete.csv,
 * sha256 eb511ebc61204bc1c6d700d9da42257dcbb0369cb6756e89ed978a7821867c7d.
 * Polynomia Alexander (forma normalis) et Jones per laqueus ex
 * codicibus PD COMPUTATA sunt.
 */


constans character* TABULA_NODORUM_FONS =
    "KnotInfo (C. Livingston, A. H. Moore) per database_knotinfo 2026.10.5, sha256 eb511ebc61204bc1c6d700d9da42257dcbb0369cb6756e89ed978a7821867c7d";

constans i32 TABULA_NODORUM_PD[] = {
    /* 3_1 */
    1, 5, 2, 4, 3, 1, 4, 6, 5, 3, 6, 2,
    /* 4_1 */
    4, 2, 5, 1, 8, 6, 1, 5, 6, 3, 7, 4, 2, 7, 3, 8,
    /* 5_1 */
    2, 8, 3, 7, 4, 10, 5, 9, 6, 2, 7, 1, 8, 4, 9, 3, 10, 6, 1, 5,
    /* 5_2 */
    1, 5, 2, 4, 3, 9, 4, 8, 5, 1, 6, 10, 7, 3, 8, 2, 9, 7, 10, 6,
    /* 6_1 */
    1, 7, 2, 6, 3, 10, 4, 11, 5, 3, 6, 2, 7, 1, 8, 12, 9, 4, 10, 5, 11, 9, 12, 8,
    /* 6_2 */
    1, 8, 2, 9, 3, 11, 4, 10, 5, 1, 6, 12, 7, 2, 8, 3, 9, 7, 10, 6, 11, 5, 12, 4,
    /* 6_3 */
    4, 2, 5, 1, 8, 4, 9, 3, 12, 9, 1, 10, 10, 5, 11, 6, 6, 11, 7, 12, 2, 8, 3, 7,
    /* 7_1 */
    1, 9, 2, 8, 3, 11, 4, 10, 5, 13, 6, 12, 7, 1, 8, 14, 9, 3, 10, 2, 11, 5, 12, 4, 13, 7, 14, 6,
    /* 7_2 */
    2, 10, 3, 9, 4, 14, 5, 13, 6, 12, 7, 11, 8, 2, 9, 1, 10, 8, 11, 7, 12, 6, 13, 5, 14, 4, 1, 3,
    /* 7_3 */
    1, 9, 2, 8, 3, 11, 4, 10, 5, 1, 6, 14, 7, 13, 8, 12, 9, 3, 10, 2, 11, 5, 12, 4, 13, 7, 14, 6,
    /* 7_4 */
    2, 10, 3, 9, 4, 12, 5, 11, 6, 14, 7, 13, 8, 4, 9, 3, 10, 2, 11, 1, 12, 8, 13, 7, 14, 6, 1, 5,
    /* 7_5 */
    2, 10, 3, 9, 4, 2, 5, 1, 6, 14, 7, 13, 8, 12, 9, 11, 10, 4, 11, 3, 12, 6, 13, 5, 14, 8, 1, 7,
    /* 7_6 */
    1, 13, 2, 12, 3, 9, 4, 8, 5, 1, 6, 14, 7, 10, 8, 11, 9, 3, 10, 2, 11, 6, 12, 7, 13, 5, 14, 4,
    /* 7_7 */
    1, 10, 2, 11, 3, 13, 4, 12, 5, 14, 6, 1, 7, 5, 8, 4, 9, 2, 10, 3, 11, 9, 12, 8, 13, 6, 14, 7,
    /* 8_1 */
    1, 9, 2, 8, 3, 7, 4, 6, 5, 12, 6, 13, 7, 3, 8, 2, 9, 1, 10, 16, 11, 15, 12, 14, 13, 4, 14, 5, 15, 11, 16, 10,
    /* 8_2 */
    1, 10, 2, 11, 3, 13, 4, 12, 5, 15, 6, 14, 7, 1, 8, 16, 9, 2, 10, 3, 11, 9, 12, 8, 13, 5, 14, 4, 15, 7, 16, 6,
    /* 8_3 */
    6, 2, 7, 1, 14, 10, 15, 9, 10, 5, 11, 6, 12, 3, 13, 4, 4, 11, 5, 12, 2, 13, 3, 14, 16, 8, 1, 7, 8, 16, 9, 15,
    /* 8_4 */
    2, 11, 3, 12, 4, 9, 5, 10, 6, 16, 7, 15, 8, 14, 9, 13, 10, 1, 11, 2, 12, 3, 13, 4, 14, 8, 15, 7, 16, 6, 1, 5,
    /* 8_5 */
    1, 7, 2, 6, 3, 9, 4, 8, 5, 12, 6, 13, 7, 3, 8, 2, 9, 15, 10, 14, 11, 1, 12, 16, 13, 4, 14, 5, 15, 11, 16, 10,
    /* 8_6 */
    2, 9, 3, 10, 4, 14, 5, 13, 6, 16, 7, 15, 8, 12, 9, 11, 10, 1, 11, 2, 12, 8, 13, 7, 14, 6, 15, 5, 16, 4, 1, 3,
    /* 8_7 */
    2, 9, 3, 10, 4, 14, 5, 13, 6, 15, 7, 16, 8, 1, 9, 2, 10, 5, 11, 6, 12, 4, 13, 3, 14, 12, 15, 11, 16, 7, 1, 8,
    /* 8_8 */
    1, 7, 2, 6, 3, 12, 4, 13, 5, 9, 6, 8, 7, 3, 8, 2, 9, 16, 10, 1, 11, 14, 12, 15, 13, 4, 14, 5, 15, 10, 16, 11,
    /* 8_9 */
    6, 2, 7, 1, 14, 8, 15, 7, 10, 3, 11, 4, 2, 13, 3, 14, 12, 5, 13, 6, 4, 11, 5, 12, 16, 10, 1, 9, 8, 16, 9, 15,
    /* 8_10 */
    2, 14, 3, 13, 4, 9, 5, 10, 6, 11, 7, 12, 8, 15, 9, 16, 10, 5, 11, 6, 12, 2, 13, 1, 14, 7, 15, 8, 16, 4, 1, 3,
    /* 8_11 */
    1, 10, 2, 11, 3, 13, 4, 12, 5, 15, 6, 14, 7, 1, 8, 16, 9, 2, 10, 3, 11, 9, 12, 8, 13, 7, 14, 6, 15, 5, 16, 4,
    /* 8_12 */
    4, 2, 5, 1, 10, 8, 11, 7, 8, 3, 9, 4, 2, 9, 3, 10, 14, 6, 15, 5, 16, 11, 1, 12, 12, 15, 13, 16, 6, 14, 7, 13,
    /* 8_13 */
    1, 9, 2, 8, 3, 14, 4, 15, 5, 12, 6, 13, 7, 11, 8, 10, 9, 3, 10, 2, 11, 16, 12, 1, 13, 4, 14, 5, 15, 6, 16, 7,
    /* 8_14 */
    2, 12, 3, 11, 4, 8, 5, 7, 6, 15, 7, 16, 8, 14, 9, 13, 10, 2, 11, 1, 12, 10, 13, 9, 14, 4, 15, 3, 16, 5, 1, 6,
    /* 8_15 */
    1, 7, 2, 6, 3, 15, 4, 14, 5, 9, 6, 8, 7, 3, 8, 2, 9, 13, 10, 12, 11, 1, 12, 16, 13, 5, 14, 4, 15, 11, 16, 10,
    /* 8_16 */
    2, 7, 3, 8, 4, 10, 5, 9, 6, 1, 7, 2, 8, 14, 9, 13, 10, 15, 11, 16, 12, 6, 13, 5, 14, 3, 15, 4, 16, 11, 1, 12,
    /* 8_17 */
    6, 2, 7, 1, 14, 8, 15, 7, 8, 3, 9, 4, 2, 13, 3, 14, 12, 5, 13, 6, 4, 9, 5, 10, 16, 12, 1, 11, 10, 16, 11, 15,
    /* 8_18 */
    6, 2, 7, 1, 8, 3, 9, 4, 16, 11, 1, 12, 2, 14, 3, 13, 4, 15, 5, 16, 10, 6, 11, 5, 12, 7, 13, 8, 14, 10, 15, 9,
    /* 8_19 */
    2, 14, 3, 13, 5, 11, 6, 10, 7, 15, 8, 14, 9, 5, 10, 4, 11, 7, 12, 6, 12, 2, 13, 1, 15, 9, 16, 8, 16, 4, 1, 3,
    /* 8_20 */
    1, 7, 2, 6, 4, 13, 5, 14, 5, 9, 6, 8, 7, 3, 8, 2, 10, 15, 11, 16, 12, 9, 13, 10, 14, 3, 15, 4, 16, 11, 1, 12,
    /* 8_21 */
    1, 7, 2, 6, 4, 13, 5, 14, 5, 9, 6, 8, 7, 3, 8, 2, 9, 13, 10, 12, 11, 1, 12, 16, 14, 3, 15, 4, 15, 11, 16, 10,
    /* 9_1 */
    1, 11, 2, 10, 3, 13, 4, 12, 5, 15, 6, 14, 7, 17, 8, 16, 9, 1, 10, 18, 11, 3, 12, 2, 13, 5, 14, 4, 15, 7, 16, 6, 17, 9, 18, 8,
    /* 9_2 */
    2, 8, 3, 7, 4, 14, 5, 13, 6, 4, 7, 3, 8, 2, 9, 1, 10, 18, 11, 17, 12, 16, 13, 15, 14, 6, 15, 5, 16, 12, 17, 11, 18, 10, 1, 9,
    /* 9_3 */
    1, 11, 2, 10, 3, 15, 4, 14, 5, 13, 6, 12, 7, 17, 8, 16, 9, 1, 10, 18, 11, 3, 12, 2, 13, 5, 14, 4, 15, 7, 16, 6, 17, 9, 18, 8,
    /* 9_4 */
    2, 14, 3, 13, 4, 12, 5, 11, 6, 16, 7, 15, 8, 18, 9, 17, 10, 6, 11, 5, 12, 4, 13, 3, 14, 2, 15, 1, 16, 8, 17, 7, 18, 10, 1, 9,
    /* 9_5 */
    2, 14, 3, 13, 4, 12, 5, 11, 6, 16, 7, 15, 8, 18, 9, 17, 10, 6, 11, 5, 12, 4, 13, 3, 14, 2, 15, 1, 16, 10, 17, 9, 18, 8, 1, 7,
    /* 9_6 */
    1, 13, 2, 12, 3, 7, 4, 6, 5, 15, 6, 14, 7, 3, 8, 2, 9, 17, 10, 16, 11, 1, 12, 18, 13, 5, 14, 4, 15, 9, 16, 8, 17, 11, 18, 10,
    /* 9_7 */
    2, 10, 3, 9, 4, 12, 5, 11, 6, 16, 7, 15, 8, 6, 9, 5, 10, 4, 11, 3, 12, 2, 13, 1, 14, 18, 15, 17, 16, 8, 17, 7, 18, 14, 1, 13,
    /* 9_8 */
    1, 16, 2, 17, 3, 14, 4, 15, 5, 11, 6, 10, 7, 1, 8, 18, 9, 13, 10, 12, 11, 7, 12, 6, 13, 4, 14, 5, 15, 2, 16, 3, 17, 9, 18, 8,
    /* 9_9 */
    1, 13, 2, 12, 3, 9, 4, 8, 5, 15, 6, 14, 7, 17, 8, 16, 9, 3, 10, 2, 11, 1, 12, 18, 13, 5, 14, 4, 15, 7, 16, 6, 17, 11, 18, 10,
    /* 9_10 */
    1, 13, 2, 12, 3, 11, 4, 10, 5, 17, 6, 16, 7, 15, 8, 14, 9, 1, 10, 18, 11, 3, 12, 2, 13, 5, 14, 4, 15, 7, 16, 6, 17, 9, 18, 8,
    /* 9_11 */
    2, 7, 3, 8, 4, 13, 5, 14, 6, 15, 7, 16, 8, 12, 9, 11, 10, 17, 11, 18, 12, 3, 13, 4, 14, 5, 15, 6, 16, 2, 17, 1, 18, 9, 1, 10,
    /* 9_12 */
    2, 9, 3, 10, 4, 16, 5, 15, 6, 14, 7, 13, 8, 3, 9, 4, 10, 18, 11, 17, 12, 8, 13, 7, 14, 6, 15, 5, 16, 2, 17, 1, 18, 12, 1, 11,
    /* 9_13 */
    2, 12, 3, 11, 4, 14, 5, 13, 6, 16, 7, 15, 8, 18, 9, 17, 10, 6, 11, 5, 12, 4, 13, 3, 14, 2, 15, 1, 16, 10, 17, 9, 18, 8, 1, 7,
    /* 9_14 */
    1, 12, 2, 13, 3, 10, 4, 11, 5, 16, 6, 17, 7, 1, 8, 18, 9, 4, 10, 5, 11, 2, 12, 3, 13, 9, 14, 8, 15, 6, 16, 7, 17, 15, 18, 14,
    /* 9_15 */
    2, 9, 3, 10, 4, 7, 5, 8, 6, 15, 7, 16, 8, 3, 9, 4, 10, 14, 11, 13, 12, 17, 13, 18, 14, 5, 15, 6, 16, 2, 17, 1, 18, 11, 1, 12,
    /* 9_16 */
    1, 9, 2, 8, 3, 11, 4, 10, 5, 15, 6, 14, 7, 5, 8, 4, 9, 3, 10, 2, 11, 17, 12, 16, 13, 1, 14, 18, 15, 7, 16, 6, 17, 13, 18, 12,
    /* 9_17 */
    1, 9, 2, 8, 3, 12, 4, 13, 5, 16, 6, 17, 7, 1, 8, 18, 9, 3, 10, 2, 11, 4, 12, 5, 13, 11, 14, 10, 15, 6, 16, 7, 17, 15, 18, 14,
    /* 9_18 */
    2, 12, 3, 11, 4, 2, 5, 1, 6, 16, 7, 15, 8, 14, 9, 13, 10, 18, 11, 17, 12, 4, 13, 3, 14, 8, 15, 7, 16, 10, 17, 9, 18, 6, 1, 5,
    /* 9_19 */
    2, 14, 3, 13, 4, 11, 5, 12, 6, 4, 7, 3, 8, 17, 9, 18, 10, 5, 11, 6, 12, 8, 13, 7, 14, 2, 15, 1, 16, 9, 17, 10, 18, 16, 1, 15,
    /* 9_20 */
    1, 11, 2, 10, 3, 16, 4, 17, 5, 13, 6, 12, 7, 3, 8, 2, 9, 1, 10, 18, 11, 15, 12, 14, 13, 7, 14, 6, 15, 4, 16, 5, 17, 9, 18, 8,
    /* 9_21 */
    2, 7, 3, 8, 4, 13, 5, 14, 6, 15, 7, 16, 8, 12, 9, 11, 10, 17, 11, 18, 12, 5, 13, 6, 14, 3, 15, 4, 16, 2, 17, 1, 18, 9, 1, 10,
    /* 9_22 */
    1, 7, 2, 6, 3, 8, 4, 9, 5, 14, 6, 15, 7, 11, 8, 10, 9, 2, 10, 3, 11, 17, 12, 16, 13, 1, 14, 18, 15, 4, 16, 5, 17, 13, 18, 12,
    /* 9_23 */
    2, 16, 3, 15, 4, 12, 5, 11, 6, 4, 7, 3, 8, 18, 9, 17, 10, 14, 11, 13, 12, 6, 13, 5, 14, 8, 15, 7, 16, 2, 17, 1, 18, 10, 1, 9,
    /* 9_24 */
    1, 13, 2, 12, 3, 7, 4, 6, 5, 1, 6, 18, 7, 14, 8, 15, 9, 16, 10, 17, 11, 3, 12, 2, 13, 10, 14, 11, 15, 8, 16, 9, 17, 5, 18, 4,
    /* 9_25 */
    2, 10, 3, 9, 4, 17, 5, 18, 6, 12, 7, 11, 8, 4, 9, 3, 10, 14, 11, 13, 12, 8, 13, 7, 14, 2, 15, 1, 16, 5, 17, 6, 18, 16, 1, 15,
    /* 9_26 */
    2, 10, 3, 9, 4, 11, 5, 12, 6, 17, 7, 18, 8, 15, 9, 16, 10, 14, 11, 13, 12, 3, 13, 4, 14, 2, 15, 1, 16, 5, 17, 6, 18, 7, 1, 8,
    /* 9_27 */
    2, 9, 3, 10, 4, 7, 5, 8, 6, 14, 7, 13, 8, 15, 9, 16, 10, 18, 11, 17, 12, 3, 13, 4, 14, 6, 15, 5, 16, 2, 17, 1, 18, 12, 1, 11,
    /* 9_28 */
    1, 16, 2, 17, 3, 15, 4, 14, 5, 3, 6, 2, 7, 13, 8, 12, 9, 7, 10, 6, 11, 18, 12, 1, 13, 9, 14, 8, 15, 5, 16, 4, 17, 10, 18, 11,
    /* 9_29 */
    2, 10, 3, 9, 4, 17, 5, 18, 6, 12, 7, 11, 8, 4, 9, 3, 10, 15, 11, 16, 12, 6, 13, 5, 14, 1, 15, 2, 16, 7, 17, 8, 18, 13, 1, 14,
    /* 9_30 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 1, 6, 18, 7, 12, 8, 13, 9, 3, 10, 2, 11, 6, 12, 7, 13, 10, 14, 11, 15, 4, 16, 5, 17, 15, 18, 14,
    /* 9_31 */
    1, 9, 2, 8, 3, 10, 4, 11, 5, 13, 6, 12, 7, 1, 8, 18, 9, 16, 10, 17, 11, 15, 12, 14, 13, 7, 14, 6, 15, 4, 16, 5, 17, 3, 18, 2,
    /* 9_32 */
    2, 8, 3, 7, 4, 9, 5, 10, 6, 13, 7, 14, 8, 16, 9, 15, 10, 5, 11, 6, 12, 17, 13, 18, 14, 3, 15, 4, 16, 2, 17, 1, 18, 11, 1, 12,
    /* 9_33 */
    1, 15, 2, 14, 3, 16, 4, 17, 5, 12, 6, 13, 7, 5, 8, 4, 9, 2, 10, 3, 11, 6, 12, 7, 13, 1, 14, 18, 15, 11, 16, 10, 17, 8, 18, 9,
    /* 9_34 */
    2, 13, 3, 14, 4, 17, 5, 18, 6, 16, 7, 15, 8, 4, 9, 3, 10, 5, 11, 6, 12, 8, 13, 7, 14, 1, 15, 2, 16, 11, 17, 12, 18, 10, 1, 9,
    /* 9_35 */
    2, 12, 3, 11, 4, 16, 5, 15, 6, 14, 7, 13, 8, 18, 9, 17, 10, 4, 11, 3, 12, 2, 13, 1, 14, 6, 15, 5, 16, 10, 17, 9, 18, 8, 1, 7,
    /* 9_36 */
    2, 15, 3, 16, 4, 11, 5, 12, 6, 4, 7, 3, 8, 17, 9, 18, 10, 5, 11, 6, 12, 10, 13, 9, 14, 1, 15, 2, 16, 7, 17, 8, 18, 13, 1, 14,
    /* 9_37 */
    1, 10, 2, 11, 3, 14, 4, 15, 5, 1, 6, 18, 7, 17, 8, 16, 9, 2, 10, 3, 11, 9, 12, 8, 13, 4, 14, 5, 15, 13, 16, 12, 17, 7, 18, 6,
    /* 9_38 */
    2, 14, 3, 13, 4, 10, 5, 9, 6, 16, 7, 15, 8, 4, 9, 3, 10, 18, 11, 17, 12, 6, 13, 5, 14, 2, 15, 1, 16, 12, 17, 11, 18, 8, 1, 7,
    /* 9_39 */
    1, 12, 2, 13, 3, 17, 4, 16, 5, 10, 6, 11, 7, 18, 8, 1, 9, 14, 10, 15, 11, 2, 12, 3, 13, 6, 14, 7, 15, 5, 16, 4, 17, 8, 18, 9,
    /* 9_40 */
    1, 15, 2, 14, 3, 12, 4, 13, 5, 11, 6, 10, 7, 3, 8, 2, 9, 18, 10, 1, 11, 17, 12, 16, 13, 9, 14, 8, 15, 6, 16, 7, 17, 5, 18, 4,
    /* 9_41 */
    1, 15, 2, 14, 3, 12, 4, 13, 5, 16, 6, 17, 7, 3, 8, 2, 9, 18, 10, 1, 11, 4, 12, 5, 13, 9, 14, 8, 15, 6, 16, 7, 17, 10, 18, 11,
    /* 9_42 */
    1, 5, 2, 4, 5, 11, 6, 10, 3, 8, 4, 9, 9, 2, 10, 3, 16, 11, 17, 12, 14, 8, 15, 7, 6, 16, 7, 15, 18, 13, 1, 14, 12, 17, 13, 18,
    /* 9_43 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 1, 6, 18, 6, 12, 7, 11, 9, 3, 10, 2, 10, 14, 11, 13, 12, 8, 13, 7, 15, 4, 16, 5, 17, 15, 18, 14,
    /* 9_44 */
    2, 9, 3, 10, 3, 16, 4, 17, 5, 1, 6, 18, 6, 12, 7, 11, 8, 1, 9, 2, 10, 14, 11, 13, 12, 8, 13, 7, 15, 4, 16, 5, 17, 15, 18, 14,
    /* 9_45 */
    2, 9, 3, 10, 3, 16, 4, 17, 5, 1, 6, 18, 7, 12, 8, 13, 8, 1, 9, 2, 11, 6, 12, 7, 13, 10, 14, 11, 15, 4, 16, 5, 17, 15, 18, 14,
    /* 9_46 */
    2, 10, 3, 9, 3, 14, 4, 15, 6, 17, 7, 18, 8, 11, 9, 12, 10, 2, 11, 1, 13, 4, 14, 5, 15, 13, 16, 12, 16, 7, 17, 8, 18, 5, 1, 6,
    /* 9_47 */
    1, 15, 2, 14, 4, 17, 5, 18, 6, 16, 7, 15, 8, 4, 9, 3, 10, 5, 11, 6, 12, 8, 13, 7, 13, 3, 14, 2, 16, 11, 17, 12, 18, 10, 1, 9,
    /* 9_48 */
    1, 10, 2, 11, 3, 14, 4, 15, 6, 17, 7, 18, 9, 2, 10, 3, 11, 9, 12, 8, 13, 4, 14, 5, 15, 13, 16, 12, 16, 7, 17, 8, 18, 5, 1, 6,
    /* 9_49 */
    1, 15, 2, 14, 4, 12, 5, 11, 6, 16, 7, 15, 7, 3, 8, 2, 10, 18, 11, 17, 12, 4, 13, 3, 13, 9, 14, 8, 16, 6, 17, 5, 18, 10, 1, 9,
    /* 10_1 */
    2, 11, 3, 12, 4, 20, 5, 19, 6, 18, 7, 17, 8, 16, 9, 15, 10, 14, 11, 13, 12, 1, 13, 2, 14, 10, 15, 9, 16, 8, 17, 7, 18, 6, 19, 5, 20, 4, 1, 3,
    /* 10_2 */
    1, 13, 2, 12, 3, 14, 4, 15, 5, 3, 6, 2, 7, 17, 8, 16, 9, 19, 10, 18, 11, 1, 12, 20, 13, 4, 14, 5, 15, 7, 16, 6, 17, 9, 18, 8, 19, 11, 20, 10,
    /* 10_3 */
    2, 10, 3, 9, 4, 17, 5, 18, 6, 15, 7, 16, 8, 4, 9, 3, 10, 2, 11, 1, 12, 20, 13, 19, 14, 7, 15, 8, 16, 5, 17, 6, 18, 14, 19, 13, 20, 12, 1, 11,
    /* 10_4 */
    2, 13, 3, 14, 4, 11, 5, 12, 6, 20, 7, 19, 8, 18, 9, 17, 10, 16, 11, 15, 12, 1, 13, 2, 14, 3, 15, 4, 16, 10, 17, 9, 18, 8, 19, 7, 20, 6, 1, 5,
    /* 10_5 */
    2, 13, 3, 14, 4, 17, 5, 18, 6, 16, 7, 15, 8, 6, 9, 5, 10, 19, 11, 20, 12, 1, 13, 2, 14, 3, 15, 4, 16, 8, 17, 7, 18, 9, 19, 10, 20, 11, 1, 12,
    /* 10_6 */
    2, 11, 3, 12, 4, 16, 5, 15, 6, 18, 7, 17, 8, 20, 9, 19, 10, 14, 11, 13, 12, 1, 13, 2, 14, 10, 15, 9, 16, 6, 17, 5, 18, 8, 19, 7, 20, 4, 1, 3,
    /* 10_7 */
    1, 12, 2, 13, 3, 15, 4, 14, 5, 19, 6, 18, 7, 17, 8, 16, 9, 1, 10, 20, 11, 2, 12, 3, 13, 11, 14, 10, 15, 9, 16, 8, 17, 7, 18, 6, 19, 5, 20, 4,
    /* 10_8 */
    1, 12, 2, 13, 3, 10, 4, 11, 5, 15, 6, 14, 7, 17, 8, 16, 9, 19, 10, 18, 11, 2, 12, 3, 13, 20, 14, 1, 15, 7, 16, 6, 17, 9, 18, 8, 19, 5, 20, 4,
    /* 10_9 */
    1, 13, 2, 12, 3, 16, 4, 17, 5, 14, 6, 15, 7, 3, 8, 2, 9, 19, 10, 18, 11, 1, 12, 20, 13, 4, 14, 5, 15, 6, 16, 7, 17, 9, 18, 8, 19, 11, 20, 10,
    /* 10_10 */
    1, 11, 2, 10, 3, 18, 4, 19, 5, 16, 6, 17, 7, 14, 8, 15, 9, 13, 10, 12, 11, 3, 12, 2, 13, 20, 14, 1, 15, 6, 16, 7, 17, 4, 18, 5, 19, 8, 20, 9,
    /* 10_11 */
    2, 15, 3, 16, 4, 13, 5, 14, 6, 2, 7, 1, 8, 18, 9, 17, 10, 20, 11, 19, 12, 5, 13, 6, 14, 3, 15, 4, 16, 12, 17, 11, 18, 10, 19, 9, 20, 8, 1, 7,
    /* 10_12 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 13, 7, 14, 8, 19, 9, 20, 10, 1, 11, 2, 12, 7, 13, 8, 14, 5, 15, 6, 16, 4, 17, 3, 18, 16, 19, 15, 20, 9, 1, 10,
    /* 10_13 */
    2, 17, 3, 18, 4, 8, 5, 7, 6, 13, 7, 14, 8, 2, 9, 1, 10, 20, 11, 19, 12, 16, 13, 15, 14, 5, 15, 6, 16, 3, 17, 4, 18, 12, 19, 11, 20, 10, 1, 9,
    /* 10_14 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 11, 6, 10, 7, 19, 8, 18, 9, 1, 10, 20, 11, 5, 12, 4, 13, 2, 14, 3, 15, 13, 16, 12, 17, 7, 18, 6, 19, 9, 20, 8,
    /* 10_15 */
    2, 6, 3, 5, 4, 16, 5, 15, 6, 2, 7, 1, 8, 17, 9, 18, 10, 19, 11, 20, 12, 7, 13, 8, 14, 4, 15, 3, 16, 14, 17, 13, 18, 9, 19, 10, 20, 11, 1, 12,
    /* 10_16 */
    1, 10, 2, 11, 3, 15, 4, 14, 5, 17, 6, 16, 7, 19, 8, 18, 9, 2, 10, 3, 11, 20, 12, 1, 13, 9, 14, 8, 15, 7, 16, 6, 17, 5, 18, 4, 19, 12, 20, 13,
    /* 10_17 */
    6, 2, 7, 1, 12, 4, 13, 3, 20, 15, 1, 16, 16, 7, 17, 8, 18, 9, 19, 10, 8, 17, 9, 18, 10, 19, 11, 20, 14, 6, 15, 5, 2, 12, 3, 11, 4, 14, 5, 13,
    /* 10_18 */
    2, 14, 3, 13, 4, 10, 5, 9, 6, 17, 7, 18, 8, 15, 9, 16, 10, 20, 11, 19, 12, 2, 13, 1, 14, 12, 15, 11, 16, 7, 17, 8, 18, 5, 19, 6, 20, 4, 1, 3,
    /* 10_19 */
    1, 13, 2, 12, 3, 15, 4, 14, 5, 10, 6, 11, 7, 16, 8, 17, 9, 18, 10, 19, 11, 1, 12, 20, 13, 3, 14, 2, 15, 8, 16, 9, 17, 6, 18, 7, 19, 5, 20, 4,
    /* 10_20 */
    2, 8, 3, 7, 4, 15, 5, 16, 6, 4, 7, 3, 8, 2, 9, 1, 10, 18, 11, 17, 12, 20, 13, 19, 14, 5, 15, 6, 16, 14, 17, 13, 18, 12, 19, 11, 20, 10, 1, 9,
    /* 10_21 */
    1, 15, 2, 14, 3, 13, 4, 12, 5, 17, 6, 16, 7, 11, 8, 10, 9, 18, 10, 19, 11, 1, 12, 20, 13, 3, 14, 2, 15, 5, 16, 4, 17, 7, 18, 6, 19, 8, 20, 9,
    /* 10_22 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 16, 7, 15, 8, 14, 9, 13, 10, 1, 11, 2, 12, 19, 13, 20, 14, 6, 15, 5, 16, 8, 17, 7, 18, 4, 19, 3, 20, 9, 1, 10,
    /* 10_23 */
    2, 13, 3, 14, 4, 11, 5, 12, 6, 18, 7, 17, 8, 19, 9, 20, 10, 1, 11, 2, 12, 3, 13, 4, 14, 7, 15, 8, 16, 6, 17, 5, 18, 16, 19, 15, 20, 9, 1, 10,
    /* 10_24 */
    2, 14, 3, 13, 4, 10, 5, 9, 6, 17, 7, 18, 8, 6, 9, 5, 10, 4, 11, 3, 12, 20, 13, 19, 14, 2, 15, 1, 16, 7, 17, 8, 18, 12, 19, 11, 20, 16, 1, 15,
    /* 10_25 */
    1, 11, 2, 10, 3, 15, 4, 14, 5, 13, 6, 12, 7, 18, 8, 19, 9, 1, 10, 20, 11, 5, 12, 4, 13, 7, 14, 6, 15, 3, 16, 2, 17, 8, 18, 9, 19, 17, 20, 16,
    /* 10_26 */
    1, 12, 2, 13, 3, 14, 4, 15, 5, 17, 6, 16, 7, 19, 8, 18, 9, 1, 10, 20, 11, 4, 12, 5, 13, 2, 14, 3, 15, 11, 16, 10, 17, 9, 18, 8, 19, 7, 20, 6,
    /* 10_27 */
    1, 11, 2, 10, 3, 18, 4, 19, 5, 16, 6, 17, 7, 14, 8, 15, 9, 13, 10, 12, 11, 3, 12, 2, 13, 20, 14, 1, 15, 4, 16, 5, 17, 6, 18, 7, 19, 8, 20, 9,
    /* 10_28 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 14, 6, 15, 7, 11, 8, 10, 9, 3, 10, 2, 11, 20, 12, 1, 13, 18, 14, 19, 15, 4, 16, 5, 17, 6, 18, 7, 19, 12, 20, 13,
    /* 10_29 */
    2, 15, 3, 16, 4, 2, 5, 1, 6, 11, 7, 12, 8, 18, 9, 17, 10, 20, 11, 19, 12, 5, 13, 6, 14, 3, 15, 4, 16, 14, 17, 13, 18, 10, 19, 9, 20, 8, 1, 7,
    /* 10_30 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 11, 6, 10, 7, 19, 8, 18, 9, 1, 10, 20, 11, 5, 12, 4, 13, 2, 14, 3, 15, 13, 16, 12, 17, 9, 18, 8, 19, 7, 20, 6,
    /* 10_31 */
    2, 6, 3, 5, 4, 16, 5, 15, 6, 2, 7, 1, 8, 17, 9, 18, 10, 19, 11, 20, 12, 7, 13, 8, 14, 4, 15, 3, 16, 14, 17, 13, 18, 11, 19, 12, 20, 9, 1, 10,
    /* 10_32 */
    2, 14, 3, 13, 4, 10, 5, 9, 6, 15, 7, 16, 8, 17, 9, 18, 10, 20, 11, 19, 12, 2, 13, 1, 14, 12, 15, 11, 16, 7, 17, 8, 18, 5, 19, 6, 20, 4, 1, 3,
    /* 10_33 */
    6, 2, 7, 1, 14, 6, 15, 5, 20, 15, 1, 16, 16, 7, 17, 8, 8, 19, 9, 20, 18, 9, 19, 10, 10, 17, 11, 18, 2, 14, 3, 13, 12, 4, 13, 3, 4, 12, 5, 11,
    /* 10_34 */
    1, 7, 2, 6, 3, 14, 4, 15, 5, 9, 6, 8, 7, 3, 8, 2, 9, 20, 10, 1, 11, 18, 12, 19, 13, 16, 14, 17, 15, 4, 16, 5, 17, 12, 18, 13, 19, 10, 20, 11,
    /* 10_35 */
    1, 11, 2, 10, 3, 18, 4, 19, 5, 3, 6, 2, 7, 14, 8, 15, 9, 12, 10, 13, 11, 1, 12, 20, 13, 8, 14, 9, 15, 6, 16, 7, 17, 4, 18, 5, 19, 17, 20, 16,
    /* 10_36 */
    2, 15, 3, 16, 4, 14, 5, 13, 6, 12, 7, 11, 8, 18, 9, 17, 10, 8, 11, 7, 12, 6, 13, 5, 14, 20, 15, 19, 16, 1, 17, 2, 18, 10, 19, 9, 20, 4, 1, 3,
    /* 10_37 */
    4, 2, 5, 1, 10, 4, 11, 3, 12, 8, 13, 7, 8, 12, 9, 11, 18, 15, 19, 16, 16, 5, 17, 6, 6, 17, 7, 18, 20, 13, 1, 14, 14, 19, 15, 20, 2, 10, 3, 9,
    /* 10_38 */
    2, 16, 3, 15, 4, 12, 5, 11, 6, 10, 7, 9, 8, 17, 9, 18, 10, 6, 11, 5, 12, 20, 13, 19, 14, 2, 15, 1, 16, 14, 17, 13, 18, 7, 19, 8, 20, 4, 1, 3,
    /* 10_39 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 19, 6, 18, 7, 11, 8, 10, 9, 1, 10, 20, 11, 7, 12, 6, 13, 2, 14, 3, 15, 13, 16, 12, 17, 5, 18, 4, 19, 9, 20, 8,
    /* 10_40 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 19, 7, 20, 8, 13, 9, 14, 10, 1, 11, 2, 12, 9, 13, 10, 14, 5, 15, 6, 16, 4, 17, 3, 18, 16, 19, 15, 20, 7, 1, 8,
    /* 10_41 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 10, 6, 11, 7, 19, 8, 18, 9, 6, 10, 7, 11, 1, 12, 20, 13, 2, 14, 3, 15, 13, 16, 12, 17, 9, 18, 8, 19, 5, 20, 4,
    /* 10_42 */
    1, 9, 2, 8, 3, 18, 4, 19, 5, 14, 6, 15, 7, 11, 8, 10, 9, 3, 10, 2, 11, 20, 12, 1, 13, 17, 14, 16, 15, 4, 16, 5, 17, 13, 18, 12, 19, 6, 20, 7,
    /* 10_43 */
    4, 2, 5, 1, 10, 4, 11, 3, 14, 8, 15, 7, 20, 11, 1, 12, 12, 19, 13, 20, 8, 14, 9, 13, 18, 15, 19, 16, 16, 5, 17, 6, 6, 17, 7, 18, 2, 10, 3, 9,
    /* 10_44 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 20, 6, 1, 7, 19, 8, 18, 9, 7, 10, 6, 11, 5, 12, 4, 13, 2, 14, 3, 15, 13, 16, 12, 17, 10, 18, 11, 19, 9, 20, 8,
    /* 10_45 */
    4, 2, 5, 1, 12, 6, 13, 5, 10, 3, 11, 4, 2, 11, 3, 12, 20, 14, 1, 13, 14, 7, 15, 8, 6, 19, 7, 20, 18, 15, 19, 16, 16, 10, 17, 9, 8, 18, 9, 17,
    /* 10_46 */
    1, 15, 2, 14, 3, 9, 4, 8, 5, 11, 6, 10, 7, 16, 8, 17, 9, 5, 10, 4, 11, 19, 12, 18, 13, 1, 14, 20, 15, 3, 16, 2, 17, 6, 18, 7, 19, 13, 20, 12,
    /* 10_47 */
    2, 9, 3, 10, 4, 11, 5, 12, 6, 17, 7, 18, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 20, 15, 19, 16, 14, 17, 13, 18, 7, 19, 8, 20, 16, 1, 15,
    /* 10_48 */
    6, 2, 7, 1, 8, 4, 9, 3, 14, 6, 15, 5, 20, 15, 1, 16, 16, 9, 17, 10, 18, 11, 19, 12, 10, 17, 11, 18, 12, 19, 13, 20, 2, 8, 3, 7, 4, 14, 5, 13,
    /* 10_49 */
    1, 7, 2, 6, 3, 17, 4, 16, 5, 9, 6, 8, 7, 3, 8, 2, 9, 15, 10, 14, 11, 19, 12, 18, 13, 1, 14, 20, 15, 5, 16, 4, 17, 11, 18, 10, 19, 13, 20, 12,
    /* 10_50 */
    1, 10, 2, 11, 3, 19, 4, 18, 5, 13, 6, 12, 7, 15, 8, 14, 9, 17, 10, 16, 11, 20, 12, 1, 13, 9, 14, 8, 15, 7, 16, 6, 17, 3, 18, 2, 19, 5, 20, 4,
    /* 10_51 */
    2, 18, 3, 17, 4, 11, 5, 12, 6, 13, 7, 14, 8, 15, 9, 16, 10, 19, 11, 20, 12, 7, 13, 8, 14, 5, 15, 6, 16, 2, 17, 1, 18, 9, 19, 10, 20, 4, 1, 3,
    /* 10_52 */
    1, 13, 2, 12, 3, 19, 4, 18, 5, 10, 6, 11, 7, 14, 8, 15, 9, 16, 10, 17, 11, 1, 12, 20, 13, 8, 14, 9, 15, 6, 16, 7, 17, 3, 18, 2, 19, 5, 20, 4,
    /* 10_53 */
    1, 7, 2, 6, 3, 17, 4, 16, 5, 9, 6, 8, 7, 3, 8, 2, 9, 15, 10, 14, 11, 19, 12, 18, 13, 1, 14, 20, 15, 5, 16, 4, 17, 13, 18, 12, 19, 11, 20, 10,
    /* 10_54 */
    2, 16, 3, 15, 4, 9, 5, 10, 6, 11, 7, 12, 8, 19, 9, 20, 10, 5, 11, 6, 12, 18, 13, 17, 14, 2, 15, 1, 16, 14, 17, 13, 18, 7, 19, 8, 20, 4, 1, 3,
    /* 10_55 */
    2, 10, 3, 9, 4, 18, 5, 17, 6, 12, 7, 11, 8, 2, 9, 1, 10, 8, 11, 7, 12, 6, 13, 5, 14, 20, 15, 19, 16, 14, 17, 13, 18, 4, 19, 3, 20, 16, 1, 15,
    /* 10_56 */
    2, 16, 3, 15, 4, 10, 5, 9, 6, 12, 7, 11, 8, 17, 9, 18, 10, 6, 11, 5, 12, 20, 13, 19, 14, 2, 15, 1, 16, 14, 17, 13, 18, 7, 19, 8, 20, 4, 1, 3,
    /* 10_57 */
    1, 4, 2, 5, 3, 16, 4, 17, 5, 20, 6, 1, 7, 13, 8, 12, 9, 7, 10, 6, 11, 18, 12, 19, 13, 9, 14, 8, 15, 2, 16, 3, 17, 10, 18, 11, 19, 14, 20, 15,
    /* 10_58 */
    2, 15, 3, 16, 4, 10, 5, 9, 6, 13, 7, 14, 8, 6, 9, 5, 10, 20, 11, 19, 12, 7, 13, 8, 14, 18, 15, 17, 16, 1, 17, 2, 18, 12, 19, 11, 20, 4, 1, 3,
    /* 10_59 */
    1, 4, 2, 5, 3, 17, 4, 16, 5, 20, 6, 1, 7, 14, 8, 15, 9, 7, 10, 6, 11, 19, 12, 18, 13, 8, 14, 9, 15, 11, 16, 10, 17, 3, 18, 2, 19, 13, 20, 12,
    /* 10_60 */
    2, 15, 3, 16, 4, 8, 5, 7, 6, 11, 7, 12, 8, 14, 9, 13, 10, 17, 11, 18, 12, 5, 13, 6, 14, 20, 15, 19, 16, 1, 17, 2, 18, 9, 19, 10, 20, 4, 1, 3,
    /* 10_61 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 14, 6, 15, 7, 1, 8, 20, 9, 3, 10, 2, 11, 19, 12, 18, 13, 6, 14, 7, 15, 4, 16, 5, 17, 11, 18, 10, 19, 13, 20, 12,
    /* 10_62 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 13, 7, 14, 8, 19, 9, 20, 10, 1, 11, 2, 12, 5, 13, 6, 14, 7, 15, 8, 16, 4, 17, 3, 18, 16, 19, 15, 20, 9, 1, 10,
    /* 10_63 */
    1, 9, 2, 8, 3, 19, 4, 18, 5, 17, 6, 16, 7, 11, 8, 10, 9, 3, 10, 2, 11, 15, 12, 14, 13, 1, 14, 20, 15, 7, 16, 6, 17, 5, 18, 4, 19, 13, 20, 12,
    /* 10_64 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 14, 7, 13, 8, 16, 9, 15, 10, 1, 11, 2, 12, 19, 13, 20, 14, 8, 15, 7, 16, 4, 17, 3, 18, 6, 19, 5, 20, 9, 1, 10,
    /* 10_65 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 14, 6, 15, 7, 11, 8, 10, 9, 3, 10, 2, 11, 18, 12, 19, 13, 20, 14, 1, 15, 4, 16, 5, 17, 6, 18, 7, 19, 12, 20, 13,
    /* 10_66 */
    1, 11, 2, 10, 3, 13, 4, 12, 5, 19, 6, 18, 7, 15, 8, 14, 9, 5, 10, 4, 11, 3, 12, 2, 13, 17, 14, 16, 15, 9, 16, 8, 17, 1, 18, 20, 19, 7, 20, 6,
    /* 10_67 */
    1, 14, 2, 15, 3, 19, 4, 18, 5, 17, 6, 16, 7, 11, 8, 10, 9, 1, 10, 20, 11, 7, 12, 6, 13, 2, 14, 3, 15, 13, 16, 12, 17, 5, 18, 4, 19, 9, 20, 8,
    /* 10_68 */
    1, 11, 2, 10, 3, 16, 4, 17, 5, 14, 6, 15, 7, 18, 8, 19, 9, 13, 10, 12, 11, 3, 12, 2, 13, 20, 14, 1, 15, 4, 16, 5, 17, 8, 18, 9, 19, 6, 20, 7,
    /* 10_69 */
    2, 13, 3, 14, 4, 17, 5, 18, 6, 2, 7, 1, 8, 19, 9, 20, 10, 7, 11, 8, 12, 3, 13, 4, 14, 12, 15, 11, 16, 5, 17, 6, 18, 16, 19, 15, 20, 9, 1, 10,
    /* 10_70 */
    2, 15, 3, 16, 4, 11, 5, 12, 6, 13, 7, 14, 8, 20, 9, 19, 10, 7, 11, 8, 12, 5, 13, 6, 14, 18, 15, 17, 16, 1, 17, 2, 18, 10, 19, 9, 20, 4, 1, 3,
    /* 10_71 */
    1, 4, 2, 5, 3, 8, 4, 9, 11, 15, 12, 14, 5, 13, 6, 12, 13, 7, 14, 6, 9, 19, 10, 18, 15, 20, 16, 1, 19, 16, 20, 17, 17, 11, 18, 10, 7, 2, 8, 3,
    /* 10_72 */
    2, 15, 3, 16, 4, 12, 5, 11, 6, 14, 7, 13, 8, 18, 9, 17, 10, 8, 11, 7, 12, 6, 13, 5, 14, 20, 15, 19, 16, 1, 17, 2, 18, 10, 19, 9, 20, 4, 1, 3,
    /* 10_73 */
    2, 15, 3, 16, 4, 7, 5, 8, 6, 13, 7, 14, 8, 12, 9, 11, 10, 17, 11, 18, 12, 5, 13, 6, 14, 20, 15, 19, 16, 1, 17, 2, 18, 9, 19, 10, 20, 4, 1, 3,
    /* 10_74 */
    1, 12, 2, 13, 3, 15, 4, 14, 5, 17, 6, 16, 7, 1, 8, 20, 9, 19, 10, 18, 11, 2, 12, 3, 13, 11, 14, 10, 15, 7, 16, 6, 17, 5, 18, 4, 19, 9, 20, 8,
    /* 10_75 */
    1, 14, 2, 15, 3, 17, 4, 16, 5, 18, 6, 19, 7, 5, 8, 4, 9, 20, 10, 1, 11, 9, 12, 8, 13, 2, 14, 3, 15, 13, 16, 12, 17, 6, 18, 7, 19, 10, 20, 11,
    /* 10_76 */
    2, 11, 3, 12, 4, 18, 5, 17, 6, 20, 7, 19, 8, 14, 9, 13, 10, 16, 11, 15, 12, 1, 13, 2, 14, 10, 15, 9, 16, 8, 17, 7, 18, 6, 19, 5, 20, 4, 1, 3,
    /* 10_77 */
    1, 7, 2, 6, 3, 14, 4, 15, 5, 9, 6, 8, 7, 3, 8, 2, 9, 18, 10, 19, 11, 20, 12, 1, 13, 16, 14, 17, 15, 4, 16, 5, 17, 12, 18, 13, 19, 10, 20, 11,
    /* 10_78 */
    1, 7, 2, 6, 3, 17, 4, 16, 5, 9, 6, 8, 7, 3, 8, 2, 9, 13, 10, 12, 11, 1, 12, 20, 13, 18, 14, 19, 15, 5, 16, 4, 17, 14, 18, 15, 19, 11, 20, 10,
    /* 10_79 */
    6, 2, 7, 1, 8, 4, 9, 3, 12, 6, 13, 5, 18, 13, 19, 14, 16, 9, 17, 10, 10, 17, 11, 18, 20, 15, 1, 16, 14, 19, 15, 20, 2, 8, 3, 7, 4, 12, 5, 11,
    /* 10_80 */
    1, 17, 2, 16, 3, 11, 4, 10, 5, 19, 6, 18, 7, 13, 8, 12, 9, 5, 10, 4, 11, 15, 12, 14, 13, 9, 14, 8, 15, 1, 16, 20, 17, 3, 18, 2, 19, 7, 20, 6,
    /* 10_81 */
    4, 2, 5, 1, 8, 4, 9, 3, 12, 6, 13, 5, 16, 9, 17, 10, 20, 17, 1, 18, 18, 13, 19, 14, 14, 19, 15, 20, 10, 15, 11, 16, 6, 12, 7, 11, 2, 8, 3, 7,
    /* 10_82 */
    1, 12, 2, 13, 3, 15, 4, 14, 5, 19, 6, 18, 7, 1, 8, 20, 9, 2, 10, 3, 11, 16, 12, 17, 13, 9, 14, 8, 15, 10, 16, 11, 17, 5, 18, 4, 19, 7, 20, 6,
    /* 10_83 */
    2, 7, 3, 8, 4, 10, 5, 9, 6, 1, 7, 2, 8, 16, 9, 15, 10, 17, 11, 18, 12, 19, 13, 20, 14, 6, 15, 5, 16, 3, 17, 4, 18, 13, 19, 14, 20, 11, 1, 12,
    /* 10_84 */
    2, 10, 3, 9, 4, 2, 5, 1, 6, 13, 7, 14, 8, 12, 9, 11, 10, 4, 11, 3, 12, 17, 13, 18, 14, 20, 15, 19, 16, 7, 17, 8, 18, 6, 19, 5, 20, 16, 1, 15,
    /* 10_85 */
    2, 7, 3, 8, 4, 10, 5, 9, 6, 1, 7, 2, 8, 16, 9, 15, 10, 17, 11, 18, 12, 19, 13, 20, 14, 6, 15, 5, 16, 3, 17, 4, 18, 11, 19, 12, 20, 13, 1, 14,
    /* 10_86 */
    2, 7, 3, 8, 4, 10, 5, 9, 6, 1, 7, 2, 8, 15, 9, 16, 10, 18, 11, 17, 12, 20, 13, 19, 14, 4, 15, 3, 16, 5, 17, 6, 18, 14, 19, 13, 20, 12, 1, 11,
    /* 10_87 */
    2, 11, 3, 12, 4, 14, 5, 13, 6, 1, 7, 2, 8, 16, 9, 15, 10, 8, 11, 7, 12, 19, 13, 20, 14, 18, 15, 17, 16, 10, 17, 9, 18, 4, 19, 3, 20, 5, 1, 6,
    /* 10_88 */
    4, 2, 5, 1, 20, 14, 1, 13, 8, 3, 9, 4, 2, 9, 3, 10, 14, 7, 15, 8, 18, 15, 19, 16, 12, 6, 13, 5, 10, 18, 11, 17, 16, 12, 17, 11, 6, 19, 7, 20,
    /* 10_89 */
    2, 7, 3, 8, 4, 13, 5, 14, 6, 12, 7, 11, 8, 1, 9, 2, 10, 15, 11, 16, 12, 19, 13, 20, 14, 18, 15, 17, 16, 9, 17, 10, 18, 5, 19, 6, 20, 4, 1, 3,
    /* 10_90 */
    2, 9, 3, 10, 4, 13, 5, 14, 6, 20, 7, 19, 8, 16, 9, 15, 10, 3, 11, 4, 12, 18, 13, 17, 14, 1, 15, 2, 16, 12, 17, 11, 18, 8, 19, 7, 20, 6, 1, 5,
    /* 10_91 */
    6, 2, 7, 1, 20, 6, 1, 5, 16, 9, 17, 10, 10, 3, 11, 4, 2, 18, 3, 17, 14, 7, 15, 8, 8, 15, 9, 16, 12, 20, 13, 19, 18, 12, 19, 11, 4, 13, 5, 14,
    /* 10_92 */
    1, 9, 2, 8, 3, 19, 4, 18, 5, 12, 6, 13, 7, 11, 8, 10, 9, 3, 10, 2, 11, 17, 12, 16, 13, 4, 14, 5, 15, 1, 16, 20, 17, 7, 18, 6, 19, 15, 20, 14,
    /* 10_93 */
    2, 17, 3, 18, 4, 12, 5, 11, 6, 20, 7, 19, 8, 15, 9, 16, 10, 6, 11, 5, 12, 4, 13, 3, 14, 7, 15, 8, 16, 1, 17, 2, 18, 13, 19, 14, 20, 10, 1, 9,
    /* 10_94 */
    1, 9, 2, 8, 3, 10, 4, 11, 5, 14, 6, 15, 7, 1, 8, 20, 9, 17, 10, 16, 11, 4, 12, 5, 13, 19, 14, 18, 15, 2, 16, 3, 17, 13, 18, 12, 19, 7, 20, 6,
    /* 10_95 */
    2, 16, 3, 15, 4, 11, 5, 12, 6, 17, 7, 18, 8, 13, 9, 14, 10, 19, 11, 20, 12, 7, 13, 8, 14, 2, 15, 1, 16, 9, 17, 10, 18, 5, 19, 6, 20, 4, 1, 3,
    /* 10_96 */
    1, 16, 2, 17, 3, 10, 4, 11, 5, 1, 6, 20, 7, 12, 8, 13, 9, 4, 10, 5, 11, 19, 12, 18, 13, 6, 14, 7, 15, 2, 16, 3, 17, 15, 18, 14, 19, 9, 20, 8,
    /* 10_97 */
    2, 12, 3, 11, 4, 18, 5, 17, 6, 13, 7, 14, 8, 6, 9, 5, 10, 20, 11, 19, 12, 7, 13, 8, 14, 2, 15, 1, 16, 10, 17, 9, 18, 4, 19, 3, 20, 16, 1, 15,
    /* 10_98 */
    1, 10, 2, 11, 3, 17, 4, 16, 5, 13, 6, 12, 7, 19, 8, 18, 9, 15, 10, 14, 11, 20, 12, 1, 13, 7, 14, 6, 15, 3, 16, 2, 17, 9, 18, 8, 19, 5, 20, 4,
    /* 10_99 */
    6, 2, 7, 1, 10, 4, 11, 3, 16, 11, 17, 12, 14, 7, 15, 8, 8, 15, 9, 16, 20, 13, 1, 14, 12, 19, 13, 20, 18, 6, 19, 5, 2, 10, 3, 9, 4, 18, 5, 17,
    /* 10_100 */
    2, 9, 3, 10, 4, 17, 5, 18, 6, 12, 7, 11, 8, 1, 9, 2, 10, 16, 11, 15, 12, 19, 13, 20, 14, 8, 15, 7, 16, 3, 17, 4, 18, 5, 19, 6, 20, 13, 1, 14,
    /* 10_101 */
    2, 12, 3, 11, 4, 18, 5, 17, 6, 14, 7, 13, 8, 20, 9, 19, 10, 6, 11, 5, 12, 2, 13, 1, 14, 10, 15, 9, 16, 4, 17, 3, 18, 16, 19, 15, 20, 8, 1, 7,
    /* 10_102 */
    2, 9, 3, 10, 4, 12, 5, 11, 6, 18, 7, 17, 8, 16, 9, 15, 10, 19, 11, 20, 12, 6, 13, 5, 14, 1, 15, 2, 16, 8, 17, 7, 18, 4, 19, 3, 20, 13, 1, 14,
    /* 10_103 */
    2, 9, 3, 10, 4, 17, 5, 18, 6, 12, 7, 11, 8, 1, 9, 2, 10, 16, 11, 15, 12, 19, 13, 20, 14, 8, 15, 7, 16, 5, 17, 6, 18, 3, 19, 4, 20, 13, 1, 14,
    /* 10_104 */
    6, 2, 7, 1, 16, 4, 17, 3, 18, 9, 19, 10, 14, 7, 15, 8, 20, 13, 1, 14, 8, 17, 9, 18, 10, 19, 11, 20, 12, 6, 13, 5, 4, 12, 5, 11, 2, 16, 3, 15,
    /* 10_105 */
    1, 11, 2, 10, 3, 16, 4, 17, 5, 1, 6, 20, 7, 18, 8, 19, 9, 13, 10, 12, 11, 3, 12, 2, 13, 7, 14, 6, 15, 4, 16, 5, 17, 8, 18, 9, 19, 15, 20, 14,
    /* 10_106 */
    1, 15, 2, 14, 3, 10, 4, 11, 5, 17, 6, 16, 7, 13, 8, 12, 9, 2, 10, 3, 11, 18, 12, 19, 13, 1, 14, 20, 15, 5, 16, 4, 17, 7, 18, 6, 19, 8, 20, 9,
    /* 10_107 */
    1, 11, 2, 10, 3, 16, 4, 17, 5, 1, 6, 20, 7, 14, 8, 15, 9, 13, 10, 12, 11, 3, 12, 2, 13, 18, 14, 19, 15, 4, 16, 5, 17, 8, 18, 9, 19, 7, 20, 6,
    /* 10_108 */
    1, 15, 2, 14, 3, 11, 4, 10, 5, 12, 6, 13, 7, 16, 8, 17, 9, 3, 10, 2, 11, 18, 12, 19, 13, 1, 14, 20, 15, 8, 16, 9, 17, 6, 18, 7, 19, 5, 20, 4,
    /* 10_109 */
    6, 2, 7, 1, 10, 4, 11, 3, 18, 11, 19, 12, 16, 7, 17, 8, 8, 17, 9, 18, 20, 15, 1, 16, 12, 19, 13, 20, 14, 6, 15, 5, 2, 10, 3, 9, 4, 14, 5, 13,
    /* 10_110 */
    1, 14, 2, 15, 3, 10, 4, 11, 5, 1, 6, 20, 7, 17, 8, 16, 9, 4, 10, 5, 11, 19, 12, 18, 13, 2, 14, 3, 15, 7, 16, 6, 17, 13, 18, 12, 19, 9, 20, 8,
    /* 10_111 */
    1, 9, 2, 8, 3, 19, 4, 18, 5, 11, 6, 10, 7, 14, 8, 15, 9, 3, 10, 2, 11, 17, 12, 16, 13, 1, 14, 20, 15, 6, 16, 7, 17, 5, 18, 4, 19, 13, 20, 12,
    /* 10_112 */
    2, 10, 3, 9, 4, 11, 5, 12, 6, 19, 7, 20, 8, 14, 9, 13, 10, 15, 11, 16, 12, 18, 13, 17, 14, 1, 15, 2, 16, 4, 17, 3, 18, 5, 19, 6, 20, 7, 1, 8,
    /* 10_113 */
    2, 8, 3, 7, 4, 14, 5, 13, 6, 11, 7, 12, 8, 16, 9, 15, 10, 2, 11, 1, 12, 19, 13, 20, 14, 18, 15, 17, 16, 10, 17, 9, 18, 4, 19, 3, 20, 5, 1, 6,
    /* 10_114 */
    1, 8, 2, 9, 3, 11, 4, 10, 5, 14, 6, 15, 7, 12, 8, 13, 9, 17, 10, 16, 11, 18, 12, 19, 13, 6, 14, 7, 15, 1, 16, 20, 17, 2, 18, 3, 19, 5, 20, 4,
    /* 10_115 */
    6, 2, 7, 1, 14, 6, 15, 5, 20, 15, 1, 16, 16, 7, 17, 8, 8, 19, 9, 20, 18, 11, 19, 12, 10, 4, 11, 3, 4, 10, 5, 9, 12, 17, 13, 18, 2, 14, 3, 13,
    /* 10_116 */
    2, 16, 3, 15, 4, 17, 5, 18, 6, 12, 7, 11, 8, 1, 9, 2, 10, 4, 11, 3, 12, 19, 13, 20, 14, 8, 15, 7, 16, 9, 17, 10, 18, 5, 19, 6, 20, 13, 1, 14,
    /* 10_117 */
    1, 14, 2, 15, 3, 8, 4, 9, 5, 18, 6, 19, 7, 17, 8, 16, 9, 20, 10, 1, 11, 4, 12, 5, 13, 7, 14, 6, 15, 2, 16, 3, 17, 13, 18, 12, 19, 10, 20, 11,
    /* 10_118 */
    6, 2, 7, 1, 18, 6, 19, 5, 20, 13, 1, 14, 12, 19, 13, 20, 14, 7, 15, 8, 8, 3, 9, 4, 2, 16, 3, 15, 10, 18, 11, 17, 16, 10, 17, 9, 4, 11, 5, 12,
    /* 10_119 */
    1, 16, 2, 17, 3, 10, 4, 11, 5, 1, 6, 20, 7, 2, 8, 3, 9, 14, 10, 15, 11, 19, 12, 18, 13, 4, 14, 5, 15, 8, 16, 9, 17, 7, 18, 6, 19, 13, 20, 12,
    /* 10_120 */
    2, 10, 3, 9, 4, 18, 5, 17, 6, 12, 7, 11, 8, 4, 9, 3, 10, 16, 11, 15, 12, 20, 13, 19, 14, 8, 15, 7, 16, 2, 17, 1, 18, 14, 19, 13, 20, 6, 1, 5,
    /* 10_121 */
    2, 11, 3, 12, 4, 10, 5, 9, 6, 2, 7, 1, 8, 16, 9, 15, 10, 17, 11, 18, 12, 8, 13, 7, 14, 20, 15, 19, 16, 3, 17, 4, 18, 6, 19, 5, 20, 14, 1, 13,
    /* 10_122 */
    1, 8, 2, 9, 3, 11, 4, 10, 5, 12, 6, 13, 7, 16, 8, 17, 9, 15, 10, 14, 11, 18, 12, 19, 13, 1, 14, 20, 15, 2, 16, 3, 17, 6, 18, 7, 19, 5, 20, 4,
    /* 10_123 */
    8, 2, 9, 1, 10, 3, 11, 4, 12, 6, 13, 5, 4, 18, 5, 17, 18, 11, 19, 12, 2, 15, 3, 16, 16, 10, 17, 9, 20, 14, 1, 13, 14, 7, 15, 8, 6, 19, 7, 20,
    /* 10_124 */
    1, 9, 2, 8, 3, 11, 4, 10, 5, 13, 6, 12, 7, 19, 8, 18, 9, 3, 10, 2, 11, 5, 12, 4, 14, 20, 15, 19, 16, 14, 17, 13, 17, 7, 18, 6, 20, 16, 1, 15,
    /* 10_125 */
    1, 5, 2, 4, 3, 9, 4, 8, 5, 15, 6, 14, 20, 15, 1, 16, 16, 9, 17, 10, 18, 11, 19, 12, 10, 17, 11, 18, 12, 19, 13, 20, 13, 7, 14, 6, 7, 3, 8, 2,
    /* 10_126 */
    1, 7, 2, 6, 4, 15, 5, 16, 5, 9, 6, 8, 7, 3, 8, 2, 10, 17, 11, 18, 12, 19, 13, 20, 14, 9, 15, 10, 16, 3, 17, 4, 18, 11, 19, 12, 20, 13, 1, 14,
    /* 10_127 */
    1, 7, 2, 6, 4, 15, 5, 16, 5, 9, 6, 8, 7, 3, 8, 2, 9, 15, 10, 14, 11, 19, 12, 18, 13, 1, 14, 20, 16, 3, 17, 4, 17, 11, 18, 10, 19, 13, 20, 12,
    /* 10_128 */
    1, 9, 2, 8, 3, 11, 4, 10, 5, 13, 6, 12, 7, 19, 8, 18, 9, 5, 10, 4, 11, 3, 12, 2, 14, 20, 15, 19, 16, 14, 17, 13, 17, 7, 18, 6, 20, 16, 1, 15,
    /* 10_129 */
    1, 7, 2, 6, 3, 17, 4, 16, 5, 9, 6, 8, 7, 3, 8, 2, 10, 19, 11, 20, 12, 17, 13, 18, 14, 9, 15, 10, 15, 5, 16, 4, 18, 11, 19, 12, 20, 13, 1, 14,
    /* 10_130 */
    1, 7, 2, 6, 4, 15, 5, 16, 5, 9, 6, 8, 7, 3, 8, 2, 10, 19, 11, 20, 12, 17, 13, 18, 14, 9, 15, 10, 16, 3, 17, 4, 18, 11, 19, 12, 20, 13, 1, 14,
    /* 10_131 */
    1, 7, 2, 6, 4, 15, 5, 16, 5, 9, 6, 8, 7, 3, 8, 2, 9, 15, 10, 14, 11, 19, 12, 18, 13, 1, 14, 20, 16, 3, 17, 4, 17, 13, 18, 12, 19, 11, 20, 10,
    /* 10_132 */
    1, 8, 2, 9, 3, 18, 4, 19, 5, 12, 6, 13, 7, 10, 8, 11, 9, 2, 10, 3, 11, 6, 12, 7, 14, 20, 15, 19, 16, 14, 17, 13, 17, 4, 18, 5, 20, 16, 1, 15,
    /* 10_133 */
    2, 16, 3, 15, 4, 2, 5, 1, 7, 13, 8, 12, 9, 7, 10, 6, 11, 18, 12, 19, 13, 9, 14, 8, 14, 20, 15, 19, 16, 4, 17, 3, 17, 10, 18, 11, 20, 6, 1, 5,
    /* 10_134 */
    2, 16, 3, 15, 4, 2, 5, 1, 7, 13, 8, 12, 9, 7, 10, 6, 10, 18, 11, 17, 13, 9, 14, 8, 14, 20, 15, 19, 16, 4, 17, 3, 18, 12, 19, 11, 20, 6, 1, 5,
    /* 10_135 */
    1, 4, 2, 5, 3, 16, 4, 17, 5, 20, 6, 1, 7, 13, 8, 12, 9, 7, 10, 6, 10, 18, 11, 17, 13, 9, 14, 8, 15, 2, 16, 3, 18, 12, 19, 11, 19, 14, 20, 15,
    /* 10_136 */
    2, 17, 3, 18, 4, 2, 5, 1, 7, 14, 8, 15, 9, 7, 10, 6, 12, 19, 13, 20, 13, 8, 14, 9, 15, 11, 16, 10, 16, 3, 17, 4, 18, 11, 19, 12, 20, 6, 1, 5,
    /* 10_137 */
    1, 15, 2, 14, 4, 8, 5, 7, 6, 19, 7, 20, 9, 17, 10, 16, 11, 8, 12, 9, 13, 3, 14, 2, 15, 11, 16, 10, 17, 12, 18, 13, 18, 4, 19, 3, 20, 5, 1, 6,
    /* 10_138 */
    1, 15, 2, 14, 4, 8, 5, 7, 6, 19, 7, 20, 8, 12, 9, 11, 10, 15, 11, 16, 12, 18, 13, 17, 13, 3, 14, 2, 16, 9, 17, 10, 18, 4, 19, 3, 20, 5, 1, 6,
    /* 10_139 */
    1, 11, 2, 10, 4, 18, 5, 17, 5, 13, 6, 12, 7, 15, 8, 14, 9, 1, 10, 20, 11, 3, 12, 2, 13, 7, 14, 6, 16, 4, 17, 3, 18, 16, 19, 15, 19, 9, 20, 8,
    /* 10_140 */
    1, 15, 2, 14, 3, 10, 4, 11, 5, 12, 6, 13, 6, 18, 7, 17, 8, 16, 9, 15, 11, 4, 12, 5, 13, 1, 14, 20, 16, 8, 17, 7, 18, 10, 19, 9, 19, 3, 20, 2,
    /* 10_141 */
    2, 11, 3, 12, 4, 9, 5, 10, 5, 19, 6, 18, 7, 15, 8, 14, 10, 1, 11, 2, 12, 3, 13, 4, 13, 17, 14, 16, 15, 9, 16, 8, 17, 1, 18, 20, 19, 7, 20, 6,
    /* 10_142 */
    1, 15, 2, 14, 4, 12, 5, 11, 6, 18, 7, 17, 8, 16, 9, 15, 10, 4, 11, 3, 12, 6, 13, 5, 13, 1, 14, 20, 16, 8, 17, 7, 18, 10, 19, 9, 19, 3, 20, 2,
    /* 10_143 */
    2, 11, 3, 12, 4, 9, 5, 10, 6, 19, 7, 20, 7, 15, 8, 14, 10, 1, 11, 2, 12, 3, 13, 4, 13, 17, 14, 16, 15, 9, 16, 8, 18, 5, 19, 6, 20, 17, 1, 18,
    /* 10_144 */
    1, 9, 2, 8, 3, 16, 4, 17, 5, 14, 6, 15, 7, 11, 8, 10, 9, 3, 10, 2, 12, 20, 13, 19, 15, 4, 16, 5, 17, 6, 18, 7, 18, 12, 19, 11, 20, 14, 1, 13,
    /* 10_145 */
    1, 14, 2, 15, 3, 18, 4, 19, 6, 13, 7, 14, 8, 6, 9, 5, 9, 16, 10, 17, 11, 2, 12, 3, 12, 7, 13, 8, 15, 20, 16, 1, 17, 4, 18, 5, 19, 10, 20, 11,
    /* 10_146 */
    1, 14, 2, 15, 4, 10, 5, 9, 6, 14, 7, 13, 8, 17, 9, 18, 10, 4, 11, 3, 12, 8, 13, 7, 15, 20, 16, 1, 16, 11, 17, 12, 18, 5, 19, 6, 19, 3, 20, 2,
    /* 10_147 */
    2, 8, 3, 7, 3, 18, 4, 19, 6, 11, 7, 12, 8, 16, 9, 15, 10, 2, 11, 1, 13, 4, 14, 5, 14, 18, 15, 17, 16, 10, 17, 9, 19, 13, 20, 12, 20, 5, 1, 6,
    /* 10_148 */
    2, 17, 3, 18, 4, 9, 5, 10, 6, 19, 7, 20, 7, 13, 8, 12, 10, 3, 11, 4, 11, 15, 12, 14, 13, 9, 14, 8, 16, 1, 17, 2, 18, 5, 19, 6, 20, 15, 1, 16,
    /* 10_149 */
    1, 17, 2, 16, 4, 9, 5, 10, 5, 19, 6, 18, 7, 13, 8, 12, 10, 3, 11, 4, 11, 15, 12, 14, 13, 9, 14, 8, 15, 1, 16, 20, 17, 3, 18, 2, 19, 7, 20, 6,
    /* 10_150 */
    1, 17, 2, 16, 3, 7, 4, 6, 5, 1, 6, 20, 8, 14, 9, 13, 10, 8, 11, 7, 11, 18, 12, 19, 14, 10, 15, 9, 15, 3, 16, 2, 17, 12, 18, 13, 19, 5, 20, 4,
    /* 10_151 */
    2, 15, 3, 16, 4, 19, 5, 20, 6, 3, 7, 4, 8, 14, 9, 13, 10, 8, 11, 7, 11, 18, 12, 19, 14, 10, 15, 9, 16, 1, 17, 2, 17, 12, 18, 13, 20, 5, 1, 6,
    /* 10_152 */
    1, 7, 2, 6, 3, 9, 4, 8, 5, 19, 6, 18, 7, 3, 8, 2, 10, 16, 11, 15, 12, 20, 13, 19, 14, 10, 15, 9, 16, 12, 17, 11, 17, 5, 18, 4, 20, 14, 1, 13,
    /* 10_153 */
    2, 17, 3, 18, 3, 11, 4, 10, 6, 19, 7, 20, 7, 13, 8, 12, 9, 5, 10, 4, 11, 15, 12, 14, 13, 9, 14, 8, 16, 1, 17, 2, 18, 5, 19, 6, 20, 15, 1, 16,
    /* 10_154 */
    1, 17, 2, 16, 3, 7, 4, 6, 5, 1, 6, 20, 8, 14, 9, 13, 10, 8, 11, 7, 12, 18, 13, 17, 14, 10, 15, 9, 15, 3, 16, 2, 18, 12, 19, 11, 19, 5, 20, 4,
    /* 10_155 */
    1, 11, 2, 10, 3, 13, 4, 12, 5, 14, 6, 15, 6, 19, 7, 20, 9, 16, 10, 17, 11, 3, 12, 2, 13, 19, 14, 18, 15, 8, 16, 9, 17, 4, 18, 5, 20, 7, 1, 8,
    /* 10_156 */
    1, 13, 2, 12, 3, 8, 4, 9, 5, 14, 6, 15, 7, 18, 8, 19, 10, 15, 11, 16, 11, 1, 12, 20, 13, 6, 14, 7, 16, 9, 17, 10, 17, 4, 18, 5, 19, 3, 20, 2,
    /* 10_157 */
    2, 8, 3, 7, 3, 10, 4, 11, 5, 18, 6, 19, 8, 15, 9, 16, 11, 4, 12, 5, 14, 1, 15, 2, 16, 9, 17, 10, 17, 13, 18, 12, 19, 6, 20, 7, 20, 13, 1, 14,
    /* 10_158 */
    3, 9, 4, 8, 4, 11, 5, 12, 6, 20, 7, 19, 9, 17, 10, 16, 12, 5, 13, 6, 14, 1, 15, 2, 15, 11, 16, 10, 17, 3, 18, 2, 18, 8, 19, 7, 20, 13, 1, 14,
    /* 10_159 */
    2, 10, 3, 9, 4, 11, 5, 12, 5, 19, 6, 18, 7, 1, 8, 20, 8, 14, 9, 13, 10, 15, 11, 16, 12, 18, 13, 17, 14, 1, 15, 2, 16, 4, 17, 3, 19, 7, 20, 6,
    /* 10_160 */
    1, 13, 2, 12, 4, 18, 5, 17, 6, 14, 7, 13, 8, 4, 9, 3, 10, 15, 11, 16, 11, 1, 12, 20, 14, 6, 15, 5, 16, 9, 17, 10, 18, 8, 19, 7, 19, 3, 20, 2,
    /* 10_161 */
    1, 13, 2, 12, 4, 18, 5, 17, 6, 14, 7, 13, 8, 4, 9, 3, 9, 17, 10, 16, 11, 1, 12, 20, 14, 6, 15, 5, 15, 11, 16, 10, 18, 8, 19, 7, 19, 3, 20, 2,
    /* 10_162 */
    2, 9, 3, 10, 5, 12, 6, 13, 6, 18, 7, 17, 8, 16, 9, 15, 10, 19, 11, 20, 11, 4, 12, 5, 14, 1, 15, 2, 16, 8, 17, 7, 18, 4, 19, 3, 20, 13, 1, 14,
    /* 10_163 */
    1, 8, 2, 9, 3, 11, 4, 10, 6, 14, 7, 13, 9, 17, 10, 16, 11, 18, 12, 19, 12, 8, 13, 7, 14, 6, 15, 5, 15, 1, 16, 20, 17, 2, 18, 3, 19, 5, 20, 4,
    /* 10_164 */
    2, 12, 3, 11, 3, 16, 4, 17, 6, 14, 7, 13, 8, 15, 9, 16, 9, 5, 10, 4, 12, 2, 13, 1, 14, 19, 15, 20, 17, 10, 18, 11, 18, 5, 19, 6, 20, 8, 1, 7,
    /* 10_165 */
    1, 8, 2, 9, 3, 12, 4, 13, 4, 17, 5, 18, 7, 2, 8, 3, 9, 14, 10, 15, 11, 17, 12, 16, 13, 6, 14, 7, 15, 20, 16, 1, 18, 5, 19, 6, 19, 11, 20, 10,
};

constans NodusTabulae TABULA_NODORUM[] = {
    { "0_1", 0, TABULA_NODORUM_NULLA, 0,
        "1",
        "1" },
    { "3_1", 3, TABULA_NODORUM_REVERSIBILIS, 0,
        "t^2 - t + 1",
        "-t^4 + t^3 + t" },
    { "4_1", 4, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 12,
        "t^2 - 3t + 1",
        "t^2 - t + 1 - t^-1 + t^-2" },
    { "5_1", 5, TABULA_NODORUM_REVERSIBILIS, 28,
        "t^4 - t^3 + t^2 - t + 1",
        "-t^7 + t^6 - t^5 + t^4 + t^2" },
    { "5_2", 5, TABULA_NODORUM_REVERSIBILIS, 48,
        "2t^2 - 3t + 2",
        "-t^6 + t^5 - t^4 + 2t^3 - t^2 + t" },
    { "6_1", 6, TABULA_NODORUM_REVERSIBILIS, 68,
        "2t^2 - 5t + 2",
        "t^4 - t^3 + t^2 - 2t + 2 - t^-1 + t^-2" },
    { "6_2", 6, TABULA_NODORUM_REVERSIBILIS, 92,
        "t^4 - 3t^3 + 3t^2 - 3t + 1",
        "t^5 - 2t^4 + 2t^3 - 2t^2 + 2t - 1 + t^-1" },
    { "6_3", 6, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 116,
        "t^4 - 3t^3 + 5t^2 - 3t + 1",
        "-t^3 + 2t^2 - 2t + 3 - 2t^-1 + 2t^-2 - t^-3" },
    { "7_1", 7, TABULA_NODORUM_REVERSIBILIS, 140,
        "t^6 - t^5 + t^4 - t^3 + t^2 - t + 1",
        "-t^10 + t^9 - t^8 + t^7 - t^6 + t^5 + t^3" },
    { "7_2", 7, TABULA_NODORUM_REVERSIBILIS, 168,
        "3t^2 - 5t + 3",
        "-t^8 + t^7 - t^6 + 2t^5 - 2t^4 + 2t^3 - t^2 + t" },
    { "7_3", 7, TABULA_NODORUM_REVERSIBILIS, 196,
        "2t^4 - 3t^3 + 3t^2 - 3t + 2",
        "-t^9 + t^8 - 2t^7 + 3t^6 - 2t^5 + 2t^4 - t^3 + t^2" },
    { "7_4", 7, TABULA_NODORUM_REVERSIBILIS, 224,
        "4t^2 - 7t + 4",
        "-t^8 + t^7 - 2t^6 + 3t^5 - 2t^4 + 3t^3 - 2t^2 + t" },
    { "7_5", 7, TABULA_NODORUM_REVERSIBILIS, 252,
        "2t^4 - 4t^3 + 5t^2 - 4t + 2",
        "-t^9 + 2t^8 - 3t^7 + 3t^6 - 3t^5 + 3t^4 - t^3 + t^2" },
    { "7_6", 7, TABULA_NODORUM_REVERSIBILIS, 280,
        "t^4 - 5t^3 + 7t^2 - 5t + 1",
        "-t^6 + 2t^5 - 3t^4 + 4t^3 - 3t^2 + 3t - 2 + t^-1" },
    { "7_7", 7, TABULA_NODORUM_REVERSIBILIS, 308,
        "t^4 - 5t^3 + 9t^2 - 5t + 1",
        "-t^3 + 3t^2 - 3t + 4 - 4t^-1 + 3t^-2 - 2t^-3 + t^-4" },
    { "8_1", 8, TABULA_NODORUM_REVERSIBILIS, 336,
        "3t^2 - 7t + 3",
        "t^6 - t^5 + t^4 - 2t^3 + 2t^2 - 2t + 2 - t^-1 + t^-2" },
    { "8_2", 8, TABULA_NODORUM_REVERSIBILIS, 368,
        "t^6 - 3t^5 + 3t^4 - 3t^3 + 3t^2 - 3t + 1",
        "t^8 - 2t^7 + 2t^6 - 3t^5 + 3t^4 - 2t^3 + 2t^2 - t + 1" },
    { "8_3", 8, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 400,
        "4t^2 - 9t + 4",
        "t^4 - t^3 + 2t^2 - 3t + 3 - 3t^-1 + 2t^-2 - t^-3 + t^-4" },
    { "8_4", 8, TABULA_NODORUM_REVERSIBILIS, 432,
        "2t^4 - 5t^3 + 5t^2 - 5t + 2",
        "t^3 - t^2 + 2t - 3 + 3t^-1 - 3t^-2 + 3t^-3 - 2t^-4 + t^-5" },
    { "8_5", 8, TABULA_NODORUM_REVERSIBILIS, 464,
        "t^6 - 3t^5 + 4t^4 - 5t^3 + 4t^2 - 3t + 1",
        "t^8 - 2t^7 + 3t^6 - 4t^5 + 3t^4 - 3t^3 + 3t^2 - t + 1" },
    { "8_6", 8, TABULA_NODORUM_REVERSIBILIS, 496,
        "2t^4 - 6t^3 + 7t^2 - 6t + 2",
        "t^7 - 2t^6 + 3t^5 - 4t^4 + 4t^3 - 4t^2 + 3t - 1 + t^-1" },
    { "8_7", 8, TABULA_NODORUM_REVERSIBILIS, 528,
        "t^6 - 3t^5 + 5t^4 - 5t^3 + 5t^2 - 3t + 1",
        "-t^2 + 2t - 2 + 4t^-1 - 4t^-2 + 4t^-3 - 3t^-4 + 2t^-5 - t^-6" },
    { "8_8", 8, TABULA_NODORUM_REVERSIBILIS, 560,
        "2t^4 - 6t^3 + 9t^2 - 6t + 2",
        "-t^3 + 2t^2 - 3t + 5 - 4t^-1 + 4t^-2 - 3t^-3 + 2t^-4 - t^-5" },
    { "8_9", 8, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 592,
        "t^6 - 3t^5 + 5t^4 - 7t^3 + 5t^2 - 3t + 1",
        "t^4 - 2t^3 + 3t^2 - 4t + 5 - 4t^-1 + 3t^-2 - 2t^-3 + t^-4" },
    { "8_10", 8, TABULA_NODORUM_REVERSIBILIS, 624,
        "t^6 - 3t^5 + 6t^4 - 7t^3 + 6t^2 - 3t + 1",
        "-t^2 + 2t - 3 + 5t^-1 - 4t^-2 + 5t^-3 - 4t^-4 + 2t^-5 - t^-6" },
    { "8_11", 8, TABULA_NODORUM_REVERSIBILIS, 656,
        "2t^4 - 7t^3 + 9t^2 - 7t + 2",
        "t^7 - 2t^6 + 3t^5 - 5t^4 + 5t^3 - 4t^2 + 4t - 2 + t^-1" },
    { "8_12", 8, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 688,
        "t^4 - 7t^3 + 13t^2 - 7t + 1",
        "t^4 - 2t^3 + 4t^2 - 5t + 5 - 5t^-1 + 4t^-2 - 2t^-3 + t^-4" },
    { "8_13", 8, TABULA_NODORUM_REVERSIBILIS, 720,
        "2t^4 - 7t^3 + 11t^2 - 7t + 2",
        "-t^3 + 3t^2 - 4t + 5 - 5t^-1 + 5t^-2 - 3t^-3 + 2t^-4 - t^-5" },
    { "8_14", 8, TABULA_NODORUM_REVERSIBILIS, 752,
        "2t^4 - 8t^3 + 11t^2 - 8t + 2",
        "t^7 - 3t^6 + 4t^5 - 5t^4 + 6t^3 - 5t^2 + 4t - 2 + t^-1" },
    { "8_15", 8, TABULA_NODORUM_REVERSIBILIS, 784,
        "3t^4 - 8t^3 + 11t^2 - 8t + 3",
        "t^10 - 3t^9 + 4t^8 - 6t^7 + 6t^6 - 5t^5 + 5t^4 - 2t^3 + t^2" },
    { "8_16", 8, TABULA_NODORUM_REVERSIBILIS, 816,
        "t^6 - 4t^5 + 8t^4 - 9t^3 + 8t^2 - 4t + 1",
        "-t^2 + 3t - 4 + 6t^-1 - 6t^-2 + 6t^-3 - 5t^-4 + 3t^-5 - t^-6" },
    { "8_17", 8, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 848,
        "t^6 - 4t^5 + 8t^4 - 11t^3 + 8t^2 - 4t + 1",
        "t^4 - 3t^3 + 5t^2 - 6t + 7 - 6t^-1 + 5t^-2 - 3t^-3 + t^-4" },
    { "8_18", 8, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 880,
        "t^6 - 5t^5 + 10t^4 - 13t^3 + 10t^2 - 5t + 1",
        "t^4 - 4t^3 + 6t^2 - 7t + 9 - 7t^-1 + 6t^-2 - 4t^-3 + t^-4" },
    { "8_19", 8, TABULA_NODORUM_REVERSIBILIS, 912,
        "t^6 - t^5 + t^3 - t + 1",
        "-t^8 + t^5 + t^3" },
    { "8_20", 8, TABULA_NODORUM_REVERSIBILIS, 944,
        "t^4 - 2t^3 + 3t^2 - 2t + 1",
        "-t + 2 - t^-1 + 2t^-2 - t^-3 + t^-4 - t^-5" },
    { "8_21", 8, TABULA_NODORUM_REVERSIBILIS, 976,
        "t^4 - 4t^3 + 5t^2 - 4t + 1",
        "t^7 - 2t^6 + 2t^5 - 3t^4 + 3t^3 - 2t^2 + 2t" },
    { "9_1", 9, TABULA_NODORUM_REVERSIBILIS, 1008,
        "t^8 - t^7 + t^6 - t^5 + t^4 - t^3 + t^2 - t + 1",
        "-t^13 + t^12 - t^11 + t^10 - t^9 + t^8 - t^7 + t^6 + t^4" },
    { "9_2", 9, TABULA_NODORUM_REVERSIBILIS, 1044,
        "4t^2 - 7t + 4",
        "-t^10 + t^9 - t^8 + 2t^7 - 2t^6 + 2t^5 - 2t^4 + 2t^3 - t^2 + t" },
    { "9_3", 9, TABULA_NODORUM_REVERSIBILIS, 1080,
        "2t^6 - 3t^5 + 3t^4 - 3t^3 + 3t^2 - 3t + 2",
        "-t^12 + t^11 - 2t^10 + 3t^9 - 3t^8 + 3t^7 - 2t^6 + 2t^5 - t^4 + t^3" },
    { "9_4", 9, TABULA_NODORUM_REVERSIBILIS, 1116,
        "3t^4 - 5t^3 + 5t^2 - 5t + 3",
        "-t^11 + t^10 - 2t^9 + 3t^8 - 3t^7 + 4t^6 - 3t^5 + 2t^4 - t^3 + t^2" },
    { "9_5", 9, TABULA_NODORUM_REVERSIBILIS, 1152,
        "6t^2 - 11t + 6",
        "-t^10 + t^9 - 2t^8 + 3t^7 - 3t^6 + 4t^5 - 3t^4 + 3t^3 - 2t^2 + t" },
    { "9_6", 9, TABULA_NODORUM_REVERSIBILIS, 1188,
        "2t^6 - 4t^5 + 5t^4 - 5t^3 + 5t^2 - 4t + 2",
        "-t^12 + 2t^11 - 3t^10 + 4t^9 - 5t^8 + 4t^7 - 3t^6 + 3t^5 - t^4 + t^3" },
    { "9_7", 9, TABULA_NODORUM_REVERSIBILIS, 1224,
        "3t^4 - 7t^3 + 9t^2 - 7t + 3",
        "-t^11 + 2t^10 - 3t^9 + 4t^8 - 5t^7 + 5t^6 - 4t^5 + 3t^4 - t^3 + t^2" },
    { "9_8", 9, TABULA_NODORUM_REVERSIBILIS, 1260,
        "2t^4 - 8t^3 + 11t^2 - 8t + 2",
        "-t^6 + 2t^5 - 3t^4 + 5t^3 - 5t^2 + 5t - 4 + 3t^-1 - 2t^-2 + t^-3" },
    { "9_9", 9, TABULA_NODORUM_REVERSIBILIS, 1296,
        "2t^6 - 4t^5 + 6t^4 - 7t^3 + 6t^2 - 4t + 2",
        "-t^12 + 2t^11 - 4t^10 + 5t^9 - 5t^8 + 5t^7 - 4t^6 + 3t^5 - t^4 + t^3" },
    { "9_10", 9, TABULA_NODORUM_REVERSIBILIS, 1332,
        "4t^4 - 8t^3 + 9t^2 - 8t + 4",
        "-t^11 + t^10 - 3t^9 + 5t^8 - 5t^7 + 6t^6 - 5t^5 + 4t^4 - 2t^3 + t^2" },
    { "9_11", 9, TABULA_NODORUM_REVERSIBILIS, 1368,
        "t^6 - 5t^5 + 7t^4 - 7t^3 + 7t^2 - 5t + 1",
        "1 - 2t^-1 + 3t^-2 - 4t^-3 + 6t^-4 - 5t^-5 + 5t^-6 - 4t^-7 + 2t^-8 - t^-9" },
    { "9_12", 9, TABULA_NODORUM_REVERSIBILIS, 1404,
        "2t^4 - 9t^3 + 13t^2 - 9t + 2",
        "-t^8 + 2t^7 - 3t^6 + 5t^5 - 6t^4 + 6t^3 - 5t^2 + 4t - 2 + t^-1" },
    { "9_13", 9, TABULA_NODORUM_REVERSIBILIS, 1440,
        "4t^4 - 9t^3 + 11t^2 - 9t + 4",
        "-t^11 + 2t^10 - 4t^9 + 5t^8 - 6t^7 + 7t^6 - 5t^5 + 4t^4 - 2t^3 + t^2" },
    { "9_14", 9, TABULA_NODORUM_REVERSIBILIS, 1476,
        "2t^4 - 9t^3 + 15t^2 - 9t + 2",
        "-t^3 + 3t^2 - 4t + 6 - 6t^-1 + 6t^-2 - 5t^-3 + 3t^-4 - 2t^-5 + t^-6" },
    { "9_15", 9, TABULA_NODORUM_REVERSIBILIS, 1512,
        "2t^4 - 10t^3 + 15t^2 - 10t + 2",
        "t - 2 + 4t^-1 - 6t^-2 + 7t^-3 - 6t^-4 + 6t^-5 - 4t^-6 + 2t^-7 - t^-8" },
    { "9_16", 9, TABULA_NODORUM_REVERSIBILIS, 1548,
        "2t^6 - 5t^5 + 8t^4 - 9t^3 + 8t^2 - 5t + 2",
        "-t^12 + 3t^11 - 5t^10 + 6t^9 - 7t^8 + 6t^7 - 5t^6 + 4t^5 - t^4 + t^3" },
    { "9_17", 9, TABULA_NODORUM_REVERSIBILIS, 1584,
        "t^6 - 5t^5 + 9t^4 - 9t^3 + 9t^2 - 5t + 1",
        "-t^6 + 3t^5 - 4t^4 + 6t^3 - 7t^2 + 6t - 5 + 4t^-1 - 2t^-2 + t^-3" },
    { "9_18", 9, TABULA_NODORUM_REVERSIBILIS, 1620,
        "4t^4 - 10t^3 + 13t^2 - 10t + 4",
        "-t^11 + 2t^10 - 4t^9 + 6t^8 - 7t^7 + 7t^6 - 6t^5 + 5t^4 - 2t^3 + t^2" },
    { "9_19", 9, TABULA_NODORUM_REVERSIBILIS, 1656,
        "2t^4 - 10t^3 + 17t^2 - 10t + 2",
        "-t^5 + 3t^4 - 4t^3 + 6t^2 - 7t + 7 - 6t^-1 + 4t^-2 - 2t^-3 + t^-4" },
    { "9_20", 9, TABULA_NODORUM_REVERSIBILIS, 1692,
        "t^6 - 5t^5 + 9t^4 - 11t^3 + 9t^2 - 5t + 1",
        "-t^9 + 3t^8 - 5t^7 + 6t^6 - 7t^5 + 7t^4 - 5t^3 + 4t^2 - 2t + 1" },
    { "9_21", 9, TABULA_NODORUM_REVERSIBILIS, 1728,
        "2t^4 - 11t^3 + 17t^2 - 11t + 2",
        "t - 3 + 5t^-1 - 6t^-2 + 8t^-3 - 7t^-4 + 6t^-5 - 4t^-6 + 2t^-7 - t^-8" },
    { "9_22", 9, TABULA_NODORUM_REVERSIBILIS, 1764,
        "t^6 - 5t^5 + 10t^4 - 11t^3 + 10t^2 - 5t + 1",
        "-t^6 + 3t^5 - 5t^4 + 7t^3 - 7t^2 + 7t - 6 + 4t^-1 - 2t^-2 + t^-3" },
    { "9_23", 9, TABULA_NODORUM_REVERSIBILIS, 1800,
        "4t^4 - 11t^3 + 15t^2 - 11t + 4",
        "-t^11 + 3t^10 - 5t^9 + 6t^8 - 8t^7 + 8t^6 - 6t^5 + 5t^4 - 2t^3 + t^2" },
    { "9_24", 9, TABULA_NODORUM_REVERSIBILIS, 1836,
        "t^6 - 5t^5 + 10t^4 - 13t^3 + 10t^2 - 5t + 1",
        "-t^5 + 2t^4 - 4t^3 + 7t^2 - 7t + 8 - 7t^-1 + 5t^-2 - 3t^-3 + t^-4" },
    { "9_25", 9, TABULA_NODORUM_REVERSIBILIS, 1872,
        "3t^4 - 12t^3 + 17t^2 - 12t + 3",
        "-t^8 + 3t^7 - 5t^6 + 7t^5 - 8t^4 + 8t^3 - 7t^2 + 5t - 2 + t^-1" },
    { "9_26", 9, TABULA_NODORUM_REVERSIBILIS, 1908,
        "t^6 - 5t^5 + 11t^4 - 13t^3 + 11t^2 - 5t + 1",
        "-t^2 + 3t - 4 + 7t^-1 - 8t^-2 + 8t^-3 - 7t^-4 + 5t^-5 - 3t^-6 + t^-7" },
    { "9_27", 9, TABULA_NODORUM_REVERSIBILIS, 1944,
        "t^6 - 5t^5 + 11t^4 - 15t^3 + 11t^2 - 5t + 1",
        "-t^5 + 3t^4 - 5t^3 + 7t^2 - 8t + 9 - 7t^-1 + 5t^-2 - 3t^-3 + t^-4" },
    { "9_28", 9, TABULA_NODORUM_REVERSIBILIS, 1980,
        "t^6 - 5t^5 + 12t^4 - 15t^3 + 12t^2 - 5t + 1",
        "t^7 - 3t^6 + 5t^5 - 8t^4 + 9t^3 - 8t^2 + 8t - 5 + 3t^-1 - t^-2" },
    { "9_29", 9, TABULA_NODORUM_REVERSIBILIS, 2016,
        "t^6 - 5t^5 + 12t^4 - 15t^3 + 12t^2 - 5t + 1",
        "t^3 - 3t^2 + 5t - 7 + 9t^-1 - 8t^-2 + 8t^-3 - 6t^-4 + 3t^-5 - t^-6" },
    { "9_30", 9, TABULA_NODORUM_REVERSIBILIS, 2052,
        "t^6 - 5t^5 + 12t^4 - 17t^3 + 12t^2 - 5t + 1",
        "t^4 - 3t^3 + 6t^2 - 8t + 9 - 9t^-1 + 8t^-2 - 5t^-3 + 3t^-4 - t^-5" },
    { "9_31", 9, TABULA_NODORUM_REVERSIBILIS, 2088,
        "t^6 - 5t^5 + 13t^4 - 17t^3 + 13t^2 - 5t + 1",
        "t^7 - 4t^6 + 6t^5 - 8t^4 + 10t^3 - 9t^2 + 8t - 5 + 3t^-1 - t^-2" },
    { "9_32", 9, TABULA_NODORUM_CHIRALIS, 2124,
        "t^6 - 6t^5 + 14t^4 - 17t^3 + 14t^2 - 6t + 1",
        "-t^2 + 4t - 6 + 9t^-1 - 10t^-2 + 10t^-3 - 9t^-4 + 6t^-5 - 3t^-6 + t^-7" },
    { "9_33", 9, TABULA_NODORUM_CHIRALIS, 2160,
        "t^6 - 6t^5 + 14t^4 - 19t^3 + 14t^2 - 6t + 1",
        "t^4 - 4t^3 + 7t^2 - 9t + 11 - 10t^-1 + 9t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "9_34", 9, TABULA_NODORUM_REVERSIBILIS, 2196,
        "t^6 - 6t^5 + 16t^4 - 23t^3 + 16t^2 - 6t + 1",
        "t^4 - 4t^3 + 8t^2 - 10t + 12 - 12t^-1 + 10t^-2 - 7t^-3 + 4t^-4 - t^-5" },
    { "9_35", 9, TABULA_NODORUM_REVERSIBILIS, 2232,
        "7t^2 - 13t + 7",
        "-t^10 + t^9 - 3t^8 + 4t^7 - 3t^6 + 5t^5 - 4t^4 + 3t^3 - 2t^2 + t" },
    { "9_36", 9, TABULA_NODORUM_REVERSIBILIS, 2268,
        "t^6 - 5t^5 + 8t^4 - 9t^3 + 8t^2 - 5t + 1",
        "1 - 2t^-1 + 4t^-2 - 5t^-3 + 6t^-4 - 6t^-5 + 6t^-6 - 4t^-7 + 2t^-8 - t^-9" },
    { "9_37", 9, TABULA_NODORUM_REVERSIBILIS, 2304,
        "2t^4 - 11t^3 + 19t^2 - 11t + 2",
        "-t^5 + 3t^4 - 4t^3 + 7t^2 - 8t + 7 - 7t^-1 + 5t^-2 - 2t^-3 + t^-4" },
    { "9_38", 9, TABULA_NODORUM_REVERSIBILIS, 2340,
        "5t^4 - 14t^3 + 19t^2 - 14t + 5",
        "-t^11 + 3t^10 - 6t^9 + 8t^8 - 10t^7 + 10t^6 - 8t^5 + 7t^4 - 3t^3 + t^2" },
    { "9_39", 9, TABULA_NODORUM_REVERSIBILIS, 2376,
        "3t^4 - 14t^3 + 21t^2 - 14t + 3",
        "t - 3 + 6t^-1 - 8t^-2 + 10t^-3 - 9t^-4 + 8t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "9_40", 9, TABULA_NODORUM_REVERSIBILIS, 2412,
        "t^6 - 7t^5 + 18t^4 - 23t^3 + 18t^2 - 7t + 1",
        "t^7 - 4t^6 + 8t^5 - 11t^4 + 13t^3 - 13t^2 + 11t - 8 + 5t^-1 - t^-2" },
    { "9_41", 9, TABULA_NODORUM_REVERSIBILIS, 2448,
        "3t^4 - 12t^3 + 19t^2 - 12t + 3",
        "-t^3 + 3t^2 - 5t + 8 - 8t^-1 + 8t^-2 - 7t^-3 + 5t^-4 - 3t^-5 + t^-6" },
    { "9_42", 9, TABULA_NODORUM_REVERSIBILIS, 2484,
        "t^4 - 2t^3 + t^2 - 2t + 1",
        "t^3 - t^2 + t - 1 + t^-1 - t^-2 + t^-3" },
    { "9_43", 9, TABULA_NODORUM_REVERSIBILIS, 2520,
        "t^6 - 3t^5 + 2t^4 - t^3 + 2t^2 - 3t + 1",
        "-t^7 + 2t^6 - 2t^5 + 2t^4 - 2t^3 + 2t^2 - t + 1" },
    { "9_44", 9, TABULA_NODORUM_REVERSIBILIS, 2556,
        "t^4 - 4t^3 + 7t^2 - 4t + 1",
        "-t^5 + 2t^4 - 2t^3 + 3t^2 - 3t + 3 - 2t^-1 + t^-2" },
    { "9_45", 9, TABULA_NODORUM_REVERSIBILIS, 2592,
        "t^4 - 6t^3 + 9t^2 - 6t + 1",
        "2t^-1 - 3t^-2 + 4t^-3 - 4t^-4 + 4t^-5 - 3t^-6 + 2t^-7 - t^-8" },
    { "9_46", 9, TABULA_NODORUM_REVERSIBILIS, 2628,
        "2t^2 - 5t + 2",
        "2 - t^-1 + t^-2 - 2t^-3 + t^-4 - t^-5 + t^-6" },
    { "9_47", 9, TABULA_NODORUM_REVERSIBILIS, 2664,
        "t^6 - 4t^5 + 6t^4 - 5t^3 + 6t^2 - 4t + 1",
        "2t^5 - 4t^4 + 4t^3 - 5t^2 + 5t - 3 + 3t^-1 - t^-2" },
    { "9_48", 9, TABULA_NODORUM_REVERSIBILIS, 2700,
        "t^4 - 7t^3 + 11t^2 - 7t + 1",
        "t - 3 + 4t^-1 - 4t^-2 + 6t^-3 - 4t^-4 + 3t^-5 - 2t^-6" },
    { "9_49", 9, TABULA_NODORUM_REVERSIBILIS, 2736,
        "3t^4 - 6t^3 + 7t^2 - 6t + 3",
        "-2t^9 + 3t^8 - 4t^7 + 5t^6 - 4t^5 + 4t^4 - 2t^3 + t^2" },
    { "10_1", 10, TABULA_NODORUM_REVERSIBILIS, 2772,
        "4t^2 - 9t + 4",
        "t^8 - t^7 + t^6 - 2t^5 + 2t^4 - 2t^3 + 2t^2 - 2t + 2 - t^-1 + t^-2" },
    { "10_2", 10, TABULA_NODORUM_REVERSIBILIS, 2812,
        "t^8 - 3t^7 + 3t^6 - 3t^5 + 3t^4 - 3t^3 + 3t^2 - 3t + 1",
        "t^11 - 2t^10 + 2t^9 - 3t^8 + 3t^7 - 3t^6 + 3t^5 - 2t^4 + 2t^3 - t^2 + t" },
    { "10_3", 10, TABULA_NODORUM_REVERSIBILIS, 2852,
        "6t^2 - 13t + 6",
        "t^6 - t^5 + 2t^4 - 3t^3 + 3t^2 - 4t + 4 - 3t^-1 + 2t^-2 - t^-3 + t^-4" },
    { "10_4", 10, TABULA_NODORUM_REVERSIBILIS, 2892,
        "3t^4 - 7t^3 + 7t^2 - 7t + 3",
        "t^5 - t^4 + 2t^3 - 3t^2 + 3t - 4 + 4t^-1 - 3t^-2 + 3t^-3 - 2t^-4 + t^-5" },
    { "10_5", 10, TABULA_NODORUM_REVERSIBILIS, 2932,
        "t^8 - 3t^7 + 5t^6 - 5t^5 + 5t^4 - 5t^3 + 5t^2 - 3t + 1",
        "-t + 2 - 2t^-1 + 4t^-2 - 4t^-3 + 5t^-4 - 5t^-5 + 4t^-6 - 3t^-7 + 2t^-8 - t^-9" },
    { "10_6", 10, TABULA_NODORUM_REVERSIBILIS, 2972,
        "2t^6 - 6t^5 + 7t^4 - 7t^3 + 7t^2 - 6t + 2",
        "t^10 - 2t^9 + 3t^8 - 5t^7 + 6t^6 - 6t^5 + 5t^4 - 4t^3 + 3t^2 - t + 1" },
    { "10_7", 10, TABULA_NODORUM_REVERSIBILIS, 3012,
        "3t^4 - 11t^3 + 15t^2 - 11t + 3",
        "t^9 - 2t^8 + 3t^7 - 5t^6 + 6t^5 - 7t^4 + 7t^3 - 5t^2 + 4t - 2 + t^-1" },
    { "10_8", 10, TABULA_NODORUM_REVERSIBILIS, 3052,
        "2t^6 - 5t^5 + 5t^4 - 5t^3 + 5t^2 - 5t + 2",
        "t^8 - 2t^7 + 3t^6 - 4t^5 + 4t^4 - 4t^3 + 4t^2 - 3t + 2 - t^-1 + t^-2" },
    { "10_9", 10, TABULA_NODORUM_REVERSIBILIS, 3092,
        "t^8 - 3t^7 + 5t^6 - 7t^5 + 7t^4 - 7t^3 + 5t^2 - 3t + 1",
        "t^7 - 2t^6 + 3t^5 - 5t^4 + 6t^3 - 6t^2 + 6t - 4 + 3t^-1 - 2t^-2 + t^-3" },
    { "10_10", 10, TABULA_NODORUM_REVERSIBILIS, 3132,
        "3t^4 - 11t^3 + 17t^2 - 11t + 3",
        "-t^3 + 3t^2 - 4t + 6 - 7t^-1 + 7t^-2 - 6t^-3 + 5t^-4 - 3t^-5 + 2t^-6 - t^-7" },
    { "10_11", 10, TABULA_NODORUM_REVERSIBILIS, 3172,
        "4t^4 - 11t^3 + 13t^2 - 11t + 4",
        "t^7 - 2t^6 + 4t^5 - 6t^4 + 7t^3 - 7t^2 + 6t - 5 + 3t^-1 - t^-2 + t^-3" },
    { "10_12", 10, TABULA_NODORUM_REVERSIBILIS, 3212,
        "2t^6 - 6t^5 + 10t^4 - 11t^3 + 10t^2 - 6t + 2",
        "-t^2 + 2t - 3 + 6t^-1 - 7t^-2 + 8t^-3 - 7t^-4 + 6t^-5 - 4t^-6 + 2t^-7 - t^-8" },
    { "10_13", 10, TABULA_NODORUM_REVERSIBILIS, 3252,
        "2t^4 - 13t^3 + 23t^2 - 13t + 2",
        "t^6 - 2t^5 + 4t^4 - 6t^3 + 8t^2 - 9t + 8 - 7t^-1 + 5t^-2 - 2t^-3 + t^-4" },
    { "10_14", 10, TABULA_NODORUM_REVERSIBILIS, 3292,
        "2t^6 - 8t^5 + 12t^4 - 13t^3 + 12t^2 - 8t + 2",
        "t^10 - 3t^9 + 5t^8 - 8t^7 + 9t^6 - 9t^5 + 9t^4 - 6t^3 + 4t^2 - 2t + 1" },
    { "10_15", 10, TABULA_NODORUM_REVERSIBILIS, 3332,
        "2t^6 - 6t^5 + 9t^4 - 9t^3 + 9t^2 - 6t + 2",
        "-t^4 + 2t^3 - 3t^2 + 5t - 6 + 7t^-1 - 6t^-2 + 6t^-3 - 4t^-4 + 2t^-5 - t^-6" },
    { "10_16", 10, TABULA_NODORUM_REVERSIBILIS, 3372,
        "4t^4 - 12t^3 + 15t^2 - 12t + 4",
        "t^7 - 2t^6 + 4t^5 - 6t^4 + 7t^3 - 8t^2 + 7t - 5 + 4t^-1 - 2t^-2 + t^-3" },
    { "10_17", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 3412,
        "t^8 - 3t^7 + 5t^6 - 7t^5 + 9t^4 - 7t^3 + 5t^2 - 3t + 1",
        "-t^5 + 2t^4 - 3t^3 + 5t^2 - 6t + 7 - 6t^-1 + 5t^-2 - 3t^-3 + 2t^-4 - t^-5" },
    { "10_18", 10, TABULA_NODORUM_REVERSIBILIS, 3452,
        "4t^4 - 14t^3 + 19t^2 - 14t + 4",
        "t^7 - 3t^6 + 5t^5 - 7t^4 + 9t^3 - 9t^2 + 8t - 6 + 4t^-1 - 2t^-2 + t^-3" },
    { "10_19", 10, TABULA_NODORUM_REVERSIBILIS, 3492,
        "2t^6 - 7t^5 + 11t^4 - 11t^3 + 11t^2 - 7t + 2",
        "-t^6 + 3t^5 - 5t^4 + 7t^3 - 8t^2 + 8t - 7 + 6t^-1 - 3t^-2 + 2t^-3 - t^-4" },
    { "10_20", 10, TABULA_NODORUM_REVERSIBILIS, 3532,
        "3t^4 - 9t^3 + 11t^2 - 9t + 3",
        "t^9 - 2t^8 + 3t^7 - 4t^6 + 5t^5 - 6t^4 + 5t^3 - 4t^2 + 3t - 1 + t^-1" },
    { "10_21", 10, TABULA_NODORUM_REVERSIBILIS, 3572,
        "2t^6 - 7t^5 + 9t^4 - 9t^3 + 9t^2 - 7t + 2",
        "t^10 - 2t^9 + 3t^8 - 6t^7 + 7t^6 - 7t^5 + 7t^4 - 5t^3 + 4t^2 - 2t + 1" },
    { "10_22", 10, TABULA_NODORUM_REVERSIBILIS, 3612,
        "2t^6 - 6t^5 + 10t^4 - 13t^3 + 10t^2 - 6t + 2",
        "t^6 - 2t^5 + 4t^4 - 6t^3 + 7t^2 - 8t + 8 - 6t^-1 + 4t^-2 - 2t^-3 + t^-4" },
    { "10_23", 10, TABULA_NODORUM_REVERSIBILIS, 3652,
        "2t^6 - 7t^5 + 13t^4 - 15t^3 + 13t^2 - 7t + 2",
        "-t^2 + 3t - 5 + 8t^-1 - 9t^-2 + 10t^-3 - 9t^-4 + 7t^-5 - 4t^-6 + 2t^-7 - t^-8" },
    { "10_24", 10, TABULA_NODORUM_REVERSIBILIS, 3692,
        "4t^4 - 14t^3 + 19t^2 - 14t + 4",
        "t^9 - 2t^8 + 4t^7 - 7t^6 + 8t^5 - 9t^4 + 9t^3 - 7t^2 + 5t - 2 + t^-1" },
    { "10_25", 10, TABULA_NODORUM_REVERSIBILIS, 3732,
        "2t^6 - 8t^5 + 14t^4 - 17t^3 + 14t^2 - 8t + 2",
        "t^10 - 3t^9 + 6t^8 - 9t^7 + 10t^6 - 11t^5 + 10t^4 - 7t^3 + 5t^2 - 2t + 1" },
    { "10_26", 10, TABULA_NODORUM_REVERSIBILIS, 3772,
        "2t^6 - 7t^5 + 13t^4 - 17t^3 + 13t^2 - 7t + 2",
        "t^6 - 2t^5 + 4t^4 - 7t^3 + 9t^2 - 10t + 10 - 8t^-1 + 6t^-2 - 3t^-3 + t^-4" },
    { "10_27", 10, TABULA_NODORUM_REVERSIBILIS, 3812,
        "2t^6 - 8t^5 + 16t^4 - 19t^3 + 16t^2 - 8t + 2",
        "-t^2 + 3t - 5 + 9t^-1 - 11t^-2 + 12t^-3 - 11t^-4 + 9t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_28", 10, TABULA_NODORUM_REVERSIBILIS, 3852,
        "4t^4 - 13t^3 + 19t^2 - 13t + 4",
        "-t^3 + 3t^2 - 5t + 7 - 8t^-1 + 9t^-2 - 7t^-3 + 6t^-4 - 4t^-5 + 2t^-6 - t^-7" },
    { "10_29", 10, TABULA_NODORUM_REVERSIBILIS, 3892,
        "t^6 - 7t^5 + 15t^4 - 17t^3 + 15t^2 - 7t + 1",
        "t^7 - 3t^6 + 6t^5 - 8t^4 + 10t^3 - 11t^2 + 9t - 7 + 5t^-1 - 2t^-2 + t^-3" },
    { "10_30", 10, TABULA_NODORUM_REVERSIBILIS, 3932,
        "4t^4 - 17t^3 + 25t^2 - 17t + 4",
        "t^9 - 3t^8 + 5t^7 - 8t^6 + 10t^5 - 11t^4 + 11t^3 - 8t^2 + 6t - 3 + t^-1" },
    { "10_31", 10, TABULA_NODORUM_REVERSIBILIS, 3972,
        "4t^4 - 14t^3 + 21t^2 - 14t + 4",
        "-t^5 + 3t^4 - 5t^3 + 7t^2 - 9t + 10 - 8t^-1 + 7t^-2 - 4t^-3 + 2t^-4 - t^-5" },
    { "10_32", 10, TABULA_NODORUM_REVERSIBILIS, 4012,
        "2t^6 - 8t^5 + 15t^4 - 19t^3 + 15t^2 - 8t + 2",
        "t^6 - 3t^5 + 5t^4 - 8t^3 + 11t^2 - 11t + 11 - 9t^-1 + 6t^-2 - 3t^-3 + t^-4" },
    { "10_33", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 4052,
        "4t^4 - 16t^3 + 25t^2 - 16t + 4",
        "-t^5 + 3t^4 - 5t^3 + 8t^2 - 10t + 11 - 10t^-1 + 8t^-2 - 5t^-3 + 3t^-4 - t^-5" },
    { "10_34", 10, TABULA_NODORUM_REVERSIBILIS, 4092,
        "3t^4 - 9t^3 + 13t^2 - 9t + 3",
        "-t^3 + 2t^2 - 3t + 5 - 5t^-1 + 6t^-2 - 5t^-3 + 4t^-4 - 3t^-5 + 2t^-6 - t^-7" },
    { "10_35", 10, TABULA_NODORUM_REVERSIBILIS, 4132,
        "2t^4 - 12t^3 + 21t^2 - 12t + 2",
        "t^4 - 2t^3 + 4t^2 - 6t + 8 - 8t^-1 + 7t^-2 - 6t^-3 + 4t^-4 - 2t^-5 + t^-6" },
    { "10_36", 10, TABULA_NODORUM_REVERSIBILIS, 4172,
        "3t^4 - 13t^3 + 19t^2 - 13t + 3",
        "t^9 - 3t^8 + 4t^7 - 6t^6 + 8t^5 - 8t^4 + 8t^3 - 6t^2 + 4t - 2 + t^-1" },
    { "10_37", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 4212,
        "4t^4 - 13t^3 + 19t^2 - 13t + 4",
        "-t^5 + 2t^4 - 4t^3 + 7t^2 - 8t + 9 - 8t^-1 + 7t^-2 - 4t^-3 + 2t^-4 - t^-5" },
    { "10_38", 10, TABULA_NODORUM_REVERSIBILIS, 4252,
        "4t^4 - 15t^3 + 21t^2 - 15t + 4",
        "t^9 - 3t^8 + 5t^7 - 7t^6 + 9t^5 - 10t^4 + 9t^3 - 7t^2 + 5t - 2 + t^-1" },
    { "10_39", 10, TABULA_NODORUM_REVERSIBILIS, 4292,
        "2t^6 - 8t^5 + 13t^4 - 15t^3 + 13t^2 - 8t + 2",
        "t^10 - 3t^9 + 5t^8 - 8t^7 + 10t^6 - 10t^5 + 9t^4 - 7t^3 + 5t^2 - 2t + 1" },
    { "10_40", 10, TABULA_NODORUM_REVERSIBILIS, 4332,
        "2t^6 - 8t^5 + 17t^4 - 21t^3 + 17t^2 - 8t + 2",
        "-t^2 + 3t - 6 + 10t^-1 - 11t^-2 + 13t^-3 - 12t^-4 + 9t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_41", 10, TABULA_NODORUM_REVERSIBILIS, 4372,
        "t^6 - 7t^5 + 17t^4 - 21t^3 + 17t^2 - 7t + 1",
        "t^7 - 3t^6 + 6t^5 - 9t^4 + 11t^3 - 12t^2 + 11t - 8 + 6t^-1 - 3t^-2 + t^-3" },
    { "10_42", 10, TABULA_NODORUM_REVERSIBILIS, 4412,
        "t^6 - 7t^5 + 19t^4 - 27t^3 + 19t^2 - 7t + 1",
        "-t^5 + 4t^4 - 7t^3 + 10t^2 - 13t + 14 - 12t^-1 + 10t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "10_43", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 4452,
        "t^6 - 7t^5 + 17t^4 - 23t^3 + 17t^2 - 7t + 1",
        "-t^5 + 3t^4 - 6t^3 + 9t^2 - 11t + 13 - 11t^-1 + 9t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "10_44", 10, TABULA_NODORUM_REVERSIBILIS, 4492,
        "t^6 - 7t^5 + 19t^4 - 25t^3 + 19t^2 - 7t + 1",
        "t^7 - 4t^6 + 7t^5 - 10t^4 + 13t^3 - 13t^2 + 12t - 9 + 6t^-1 - 3t^-2 + t^-3" },
    { "10_45", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 4532,
        "t^6 - 7t^5 + 21t^4 - 31t^3 + 21t^2 - 7t + 1",
        "-t^5 + 4t^4 - 7t^3 + 11t^2 - 14t + 15 - 14t^-1 + 11t^-2 - 7t^-3 + 4t^-4 - t^-5" },
    { "10_46", 10, TABULA_NODORUM_REVERSIBILIS, 4572,
        "t^8 - 3t^7 + 4t^6 - 5t^5 + 5t^4 - 5t^3 + 4t^2 - 3t + 1",
        "t^11 - 2t^10 + 3t^9 - 4t^8 + 4t^7 - 5t^6 + 4t^5 - 3t^4 + 3t^3 - t^2 + t" },
    { "10_47", 10, TABULA_NODORUM_REVERSIBILIS, 4612,
        "t^8 - 3t^7 + 6t^6 - 7t^5 + 7t^4 - 7t^3 + 6t^2 - 3t + 1",
        "-t + 2 - 3t^-1 + 5t^-2 - 5t^-3 + 7t^-4 - 6t^-5 + 5t^-6 - 4t^-7 + 2t^-8 - t^-9" },
    { "10_48", 10, TABULA_NODORUM_REVERSIBILIS, 4652,
        "t^8 - 3t^7 + 6t^6 - 9t^5 + 11t^4 - 9t^3 + 6t^2 - 3t + 1",
        "-t^5 + 2t^4 - 4t^3 + 6t^2 - 7t + 9 - 7t^-1 + 6t^-2 - 4t^-3 + 2t^-4 - t^-5" },
    { "10_49", 10, TABULA_NODORUM_REVERSIBILIS, 4692,
        "3t^6 - 8t^5 + 12t^4 - 13t^3 + 12t^2 - 8t + 3",
        "t^13 - 3t^12 + 5t^11 - 8t^10 + 9t^9 - 10t^8 + 9t^7 - 6t^6 + 5t^5 - 2t^4 + t^3" },
    { "10_50", 10, TABULA_NODORUM_REVERSIBILIS, 4732,
        "2t^6 - 7t^5 + 11t^4 - 13t^3 + 11t^2 - 7t + 2",
        "t^10 - 2t^9 + 4t^8 - 7t^7 + 8t^6 - 9t^5 + 8t^4 - 6t^3 + 5t^2 - 2t + 1" },
    { "10_51", 10, TABULA_NODORUM_REVERSIBILIS, 4772,
        "2t^6 - 7t^5 + 15t^4 - 19t^3 + 15t^2 - 7t + 2",
        "-t^2 + 3t - 6 + 9t^-1 - 10t^-2 + 12t^-3 - 10t^-4 + 8t^-5 - 5t^-6 + 2t^-7 - t^-8" },
    { "10_52", 10, TABULA_NODORUM_REVERSIBILIS, 4812,
        "2t^6 - 7t^5 + 13t^4 - 15t^3 + 13t^2 - 7t + 2",
        "-t^6 + 3t^5 - 6t^4 + 8t^3 - 9t^2 + 10t - 8 + 7t^-1 - 4t^-2 + 2t^-3 - t^-4" },
    { "10_53", 10, TABULA_NODORUM_REVERSIBILIS, 4852,
        "6t^4 - 18t^3 + 25t^2 - 18t + 6",
        "t^12 - 3t^11 + 5t^10 - 9t^9 + 11t^8 - 12t^7 + 12t^6 - 9t^5 + 7t^4 - 3t^3 + t^2" },
    { "10_54", 10, TABULA_NODORUM_REVERSIBILIS, 4892,
        "2t^6 - 6t^5 + 10t^4 - 11t^3 + 10t^2 - 6t + 2",
        "-t^4 + 2t^3 - 4t^2 + 6t - 6 + 8t^-1 - 7t^-2 + 6t^-3 - 4t^-4 + 2t^-5 - t^-6" },
    { "10_55", 10, TABULA_NODORUM_REVERSIBILIS, 4932,
        "5t^4 - 15t^3 + 21t^2 - 15t + 5",
        "t^12 - 3t^11 + 5t^10 - 8t^9 + 9t^8 - 10t^7 + 10t^6 - 7t^5 + 5t^4 - 2t^3 + t^2" },
    { "10_56", 10, TABULA_NODORUM_REVERSIBILIS, 4972,
        "2t^6 - 8t^5 + 14t^4 - 17t^3 + 14t^2 - 8t + 2",
        "t^10 - 3t^9 + 6t^8 - 9t^7 + 10t^6 - 11t^5 + 10t^4 - 7t^3 + 5t^2 - 2t + 1" },
    { "10_57", 10, TABULA_NODORUM_REVERSIBILIS, 5012,
        "2t^6 - 8t^5 + 18t^4 - 23t^3 + 18t^2 - 8t + 2",
        "-t^2 + 3t - 6 + 10t^-1 - 12t^-2 + 14t^-3 - 12t^-4 + 10t^-5 - 7t^-6 + 3t^-7 - t^-8" },
    { "10_58", 10, TABULA_NODORUM_REVERSIBILIS, 5052,
        "3t^4 - 16t^3 + 27t^2 - 16t + 3",
        "t^6 - 3t^5 + 6t^4 - 8t^3 + 10t^2 - 11t + 10 - 8t^-1 + 5t^-2 - 2t^-3 + t^-4" },
    { "10_59", 10, TABULA_NODORUM_REVERSIBILIS, 5092,
        "t^6 - 7t^5 + 18t^4 - 23t^3 + 18t^2 - 7t + 1",
        "t^7 - 3t^6 + 6t^5 - 10t^4 + 12t^3 - 12t^2 + 12t - 9 + 6t^-1 - 3t^-2 + t^-3" },
    { "10_60", 10, TABULA_NODORUM_REVERSIBILIS, 5132,
        "t^6 - 7t^5 + 20t^4 - 29t^3 + 20t^2 - 7t + 1",
        "t^4 - 4t^3 + 8t^2 - 11t + 14 - 14t^-1 + 13t^-2 - 10t^-3 + 6t^-4 - 3t^-5 + t^-6" },
    { "10_61", 10, TABULA_NODORUM_REVERSIBILIS, 5172,
        "2t^6 - 5t^5 + 6t^4 - 7t^3 + 6t^2 - 5t + 2",
        "t^8 - 2t^7 + 3t^6 - 4t^5 + 5t^4 - 5t^3 + 4t^2 - 4t + 3 - t^-1 + t^-2" },
    { "10_62", 10, TABULA_NODORUM_REVERSIBILIS, 5212,
        "t^8 - 3t^7 + 6t^6 - 8t^5 + 9t^4 - 8t^3 + 6t^2 - 3t + 1",
        "-t + 2 - 3t^-1 + 6t^-2 - 6t^-3 + 7t^-4 - 7t^-5 + 6t^-6 - 4t^-7 + 2t^-8 - t^-9" },
    { "10_63", 10, TABULA_NODORUM_REVERSIBILIS, 5252,
        "5t^4 - 14t^3 + 19t^2 - 14t + 5",
        "t^12 - 3t^11 + 4t^10 - 7t^9 + 9t^8 - 9t^7 + 9t^6 - 7t^5 + 5t^4 - 2t^3 + t^2" },
    { "10_64", 10, TABULA_NODORUM_REVERSIBILIS, 5292,
        "t^8 - 3t^7 + 6t^6 - 10t^5 + 11t^4 - 10t^3 + 6t^2 - 3t + 1",
        "t^7 - 2t^6 + 4t^5 - 7t^4 + 8t^3 - 8t^2 + 8t - 6 + 4t^-1 - 2t^-2 + t^-3" },
    { "10_65", 10, TABULA_NODORUM_REVERSIBILIS, 5332,
        "2t^6 - 7t^5 + 14t^4 - 17t^3 + 14t^2 - 7t + 2",
        "-t^2 + 3t - 5 + 8t^-1 - 10t^-2 + 11t^-3 - 9t^-4 + 8t^-5 - 5t^-6 + 2t^-7 - t^-8" },
    { "10_66", 10, TABULA_NODORUM_REVERSIBILIS, 5372,
        "3t^6 - 9t^5 + 16t^4 - 19t^3 + 16t^2 - 9t + 3",
        "t^13 - 4t^12 + 7t^11 - 10t^10 + 12t^9 - 13t^8 + 11t^7 - 8t^6 + 6t^5 - 2t^4 + t^3" },
    { "10_67", 10, TABULA_NODORUM_CHIRALIS, 5412,
        "4t^4 - 16t^3 + 23t^2 - 16t + 4",
        "t^9 - 3t^8 + 5t^7 - 8t^6 + 10t^5 - 10t^4 + 10t^3 - 8t^2 + 5t - 2 + t^-1" },
    { "10_68", 10, TABULA_NODORUM_REVERSIBILIS, 5452,
        "4t^4 - 14t^3 + 21t^2 - 14t + 4",
        "-t^3 + 3t^2 - 5t + 8 - 9t^-1 + 9t^-2 - 8t^-3 + 7t^-4 - 4t^-5 + 2t^-6 - t^-7" },
    { "10_69", 10, TABULA_NODORUM_REVERSIBILIS, 5492,
        "t^6 - 7t^5 + 21t^4 - 29t^3 + 21t^2 - 7t + 1",
        "-t^2 + 4t - 7 + 11t^-1 - 14t^-2 + 15t^-3 - 13t^-4 + 11t^-5 - 7t^-6 + 3t^-7 - t^-8" },
    { "10_70", 10, TABULA_NODORUM_REVERSIBILIS, 5532,
        "t^6 - 7t^5 + 16t^4 - 19t^3 + 16t^2 - 7t + 1",
        "t^3 - 2t^2 + 5t - 8 + 10t^-1 - 11t^-2 + 11t^-3 - 9t^-4 + 6t^-5 - 3t^-6 + t^-7" },
    { "10_71", 10, TABULA_NODORUM_REVERSIBILIS, 5572,
        "t^6 - 7t^5 + 18t^4 - 25t^3 + 18t^2 - 7t + 1",
        "-t^5 + 3t^4 - 6t^3 + 10t^2 - 12t + 13 - 12t^-1 + 10t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "10_72", 10, TABULA_NODORUM_REVERSIBILIS, 5612,
        "2t^6 - 9t^5 + 16t^4 - 19t^3 + 16t^2 - 9t + 2",
        "t^10 - 4t^9 + 7t^8 - 10t^7 + 12t^6 - 12t^5 + 11t^4 - 8t^3 + 5t^2 - 2t + 1" },
    { "10_73", 10, TABULA_NODORUM_REVERSIBILIS, 5652,
        "t^6 - 7t^5 + 20t^4 - 27t^3 + 20t^2 - 7t + 1",
        "-t^2 + 4t - 7 + 11t^-1 - 13t^-2 + 14t^-3 - 13t^-4 + 10t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_74", 10, TABULA_NODORUM_REVERSIBILIS, 5692,
        "4t^4 - 16t^3 + 23t^2 - 16t + 4",
        "t^9 - 2t^8 + 4t^7 - 8t^6 + 9t^5 - 10t^4 + 11t^3 - 8t^2 + 6t - 3 + t^-1" },
    { "10_75", 10, TABULA_NODORUM_REVERSIBILIS, 5732,
        "t^6 - 7t^5 + 19t^4 - 27t^3 + 19t^2 - 7t + 1",
        "t^4 - 4t^3 + 7t^2 - 10t + 14 - 13t^-1 + 12t^-2 - 10t^-3 + 6t^-4 - 3t^-5 + t^-6" },
    { "10_76", 10, TABULA_NODORUM_REVERSIBILIS, 5772,
        "2t^6 - 7t^5 + 12t^4 - 15t^3 + 12t^2 - 7t + 2",
        "t^10 - 3t^9 + 6t^8 - 8t^7 + 9t^6 - 10t^5 + 8t^4 - 6t^3 + 4t^2 - t + 1" },
    { "10_77", 10, TABULA_NODORUM_REVERSIBILIS, 5812,
        "2t^6 - 7t^5 + 14t^4 - 17t^3 + 14t^2 - 7t + 2",
        "-t^2 + 2t - 4 + 8t^-1 - 9t^-2 + 11t^-3 - 10t^-4 + 8t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_78", 10, TABULA_NODORUM_REVERSIBILIS, 5852,
        "t^6 - 7t^5 + 16t^4 - 21t^3 + 16t^2 - 7t + 1",
        "t^10 - 3t^9 + 5t^8 - 9t^7 + 11t^6 - 11t^5 + 11t^4 - 8t^3 + 6t^2 - 3t + 1" },
    { "10_79", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 5892,
        "t^8 - 3t^7 + 7t^6 - 12t^5 + 15t^4 - 12t^3 + 7t^2 - 3t + 1",
        "-t^5 + 2t^4 - 5t^3 + 8t^2 - 9t + 11 - 9t^-1 + 8t^-2 - 5t^-3 + 2t^-4 - t^-5" },
    { "10_80", 10, TABULA_NODORUM_CHIRALIS, 5932,
        "3t^6 - 9t^5 + 15t^4 - 17t^3 + 15t^2 - 9t + 3",
        "t^13 - 3t^12 + 6t^11 - 10t^10 + 11t^9 - 12t^8 + 11t^7 - 8t^6 + 6t^5 - 2t^4 + t^3" },
    { "10_81", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 5972,
        "t^6 - 8t^5 + 20t^4 - 27t^3 + 20t^2 - 8t + 1",
        "-t^5 + 3t^4 - 7t^3 + 11t^2 - 13t + 15 - 13t^-1 + 11t^-2 - 7t^-3 + 3t^-4 - t^-5" },
    { "10_82", 10, TABULA_NODORUM_CHIRALIS, 6012,
        "t^8 - 4t^7 + 8t^6 - 12t^5 + 13t^4 - 12t^3 + 8t^2 - 4t + 1",
        "t^7 - 3t^6 + 5t^5 - 8t^4 + 10t^3 - 10t^2 + 10t - 7 + 5t^-1 - 3t^-2 + t^-3" },
    { "10_83", 10, TABULA_NODORUM_CHIRALIS, 6052,
        "2t^6 - 9t^5 + 19t^4 - 23t^3 + 19t^2 - 9t + 2",
        "-t^2 + 4t - 7 + 11t^-1 - 13t^-2 + 14t^-3 - 13t^-4 + 10t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_84", 10, TABULA_NODORUM_CHIRALIS, 6092,
        "2t^6 - 9t^5 + 20t^4 - 25t^3 + 20t^2 - 9t + 2",
        "-t^8 + 4t^7 - 8t^6 + 11t^5 - 14t^4 + 15t^3 - 13t^2 + 11t - 6 + 3t^-1 - t^-2" },
    { "10_85", 10, TABULA_NODORUM_CHIRALIS, 6132,
        "t^8 - 4t^7 + 8t^6 - 10t^5 + 11t^4 - 10t^3 + 8t^2 - 4t + 1",
        "-t + 3 - 4t^-1 + 7t^-2 - 8t^-3 + 9t^-4 - 9t^-5 + 7t^-6 - 5t^-7 + 3t^-8 - t^-9" },
    { "10_86", 10, TABULA_NODORUM_CHIRALIS, 6172,
        "2t^6 - 9t^5 + 19t^4 - 25t^3 + 19t^2 - 9t + 2",
        "t^6 - 3t^5 + 6t^4 - 10t^3 + 13t^2 - 14t + 14 - 11t^-1 + 8t^-2 - 4t^-3 + t^-4" },
    { "10_87", 10, TABULA_NODORUM_CHIRALIS, 6212,
        "2t^6 - 9t^5 + 18t^4 - 23t^3 + 18t^2 - 9t + 2",
        "t^6 - 4t^5 + 7t^4 - 10t^3 + 13t^2 - 13t + 13 - 10t^-1 + 6t^-2 - 3t^-3 + t^-4" },
    { "10_88", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 6252,
        "t^6 - 8t^5 + 24t^4 - 35t^3 + 24t^2 - 8t + 1",
        "-t^5 + 4t^4 - 8t^3 + 13t^2 - 16t + 17 - 16t^-1 + 13t^-2 - 8t^-3 + 4t^-4 - t^-5" },
    { "10_89", 10, TABULA_NODORUM_REVERSIBILIS, 6292,
        "t^6 - 8t^5 + 24t^4 - 33t^3 + 24t^2 - 8t + 1",
        "-t^2 + 5t - 9 + 13t^-1 - 16t^-2 + 17t^-3 - 15t^-4 + 12t^-5 - 7t^-6 + 3t^-7 - t^-8" },
    { "10_90", 10, TABULA_NODORUM_CHIRALIS, 6332,
        "2t^6 - 8t^5 + 17t^4 - 23t^3 + 17t^2 - 8t + 2",
        "t^6 - 3t^5 + 6t^4 - 9t^3 + 12t^2 - 13t + 12 - 10t^-1 + 7t^-2 - 3t^-3 + t^-4" },
    { "10_91", 10, TABULA_NODORUM_CHIRALIS, 6372,
        "t^8 - 4t^7 + 9t^6 - 14t^5 + 17t^4 - 14t^3 + 9t^2 - 4t + 1",
        "-t^5 + 3t^4 - 6t^3 + 9t^2 - 11t + 13 - 11t^-1 + 9t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "10_92", 10, TABULA_NODORUM_CHIRALIS, 6412,
        "2t^6 - 10t^5 + 20t^4 - 25t^3 + 20t^2 - 10t + 2",
        "t^10 - 4t^9 + 8t^8 - 12t^7 + 14t^6 - 15t^5 + 14t^4 - 10t^3 + 7t^2 - 3t + 1" },
    { "10_93", 10, TABULA_NODORUM_CHIRALIS, 6452,
        "2t^6 - 8t^5 + 15t^4 - 17t^3 + 15t^2 - 8t + 2",
        "-t^4 + 3t^3 - 5t^2 + 8t - 10 + 11t^-1 - 10t^-2 + 9t^-3 - 6t^-4 + 3t^-5 - t^-6" },
    { "10_94", 10, TABULA_NODORUM_CHIRALIS, 6492,
        "t^8 - 4t^7 + 9t^6 - 14t^5 + 15t^4 - 14t^3 + 9t^2 - 4t + 1",
        "t^7 - 3t^6 + 6t^5 - 9t^4 + 11t^3 - 12t^2 + 11t - 8 + 6t^-1 - 3t^-2 + t^-3" },
    { "10_95", 10, TABULA_NODORUM_CHIRALIS, 6532,
        "2t^6 - 9t^5 + 21t^4 - 27t^3 + 21t^2 - 9t + 2",
        "-t^2 + 4t - 8 + 12t^-1 - 14t^-2 + 16t^-3 - 14t^-4 + 11t^-5 - 7t^-6 + 3t^-7 - t^-8" },
    { "10_96", 10, TABULA_NODORUM_REVERSIBILIS, 6572,
        "t^6 - 7t^5 + 22t^4 - 33t^3 + 22t^2 - 7t + 1",
        "t^4 - 4t^3 + 9t^2 - 12t + 15 - 16t^-1 + 14t^-2 - 11t^-3 + 7t^-4 - 3t^-5 + t^-6" },
    { "10_97", 10, TABULA_NODORUM_REVERSIBILIS, 6612,
        "5t^4 - 22t^3 + 33t^2 - 22t + 5",
        "t^9 - 4t^8 + 7t^7 - 11t^6 + 14t^5 - 14t^4 + 14t^3 - 11t^2 + 7t - 3 + t^-1" },
    { "10_98", 10, TABULA_NODORUM_CHIRALIS, 6652,
        "2t^6 - 9t^5 + 18t^4 - 23t^3 + 18t^2 - 9t + 2",
        "t^10 - 3t^9 + 7t^8 - 11t^7 + 12t^6 - 14t^5 + 13t^4 - 9t^3 + 7t^2 - 3t + 1" },
    { "10_99", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 6692,
        "t^8 - 4t^7 + 10t^6 - 16t^5 + 19t^4 - 16t^3 + 10t^2 - 4t + 1",
        "-t^5 + 3t^4 - 7t^3 + 10t^2 - 12t + 15 - 12t^-1 + 10t^-2 - 7t^-3 + 3t^-4 - t^-5" },
    { "10_100", 10, TABULA_NODORUM_REVERSIBILIS, 6732,
        "t^8 - 4t^7 + 9t^6 - 12t^5 + 13t^4 - 12t^3 + 9t^2 - 4t + 1",
        "-t + 3 - 5t^-1 + 8t^-2 - 9t^-3 + 11t^-4 - 10t^-5 + 8t^-6 - 6t^-7 + 3t^-8 - t^-9" },
    { "10_101", 10, TABULA_NODORUM_REVERSIBILIS, 6772,
        "7t^4 - 21t^3 + 29t^2 - 21t + 7",
        "t^12 - 4t^11 + 7t^10 - 11t^9 + 13t^8 - 14t^7 + 14t^6 - 10t^5 + 7t^4 - 3t^3 + t^2" },
    { "10_102", 10, TABULA_NODORUM_CHIRALIS, 6812,
        "2t^6 - 8t^5 + 16t^4 - 21t^3 + 16t^2 - 8t + 2",
        "t^6 - 3t^5 + 6t^4 - 9t^3 + 11t^2 - 12t + 12 - 9t^-1 + 6t^-2 - 3t^-3 + t^-4" },
    { "10_103", 10, TABULA_NODORUM_REVERSIBILIS, 6852,
        "2t^6 - 8t^5 + 17t^4 - 21t^3 + 17t^2 - 8t + 2",
        "-t^2 + 3t - 6 + 10t^-1 - 11t^-2 + 13t^-3 - 12t^-4 + 9t^-5 - 6t^-6 + 3t^-7 - t^-8" },
    { "10_104", 10, TABULA_NODORUM_REVERSIBILIS, 6892,
        "t^8 - 4t^7 + 9t^6 - 15t^5 + 19t^4 - 15t^3 + 9t^2 - 4t + 1",
        "-t^5 + 3t^4 - 6t^3 + 10t^2 - 12t + 13 - 12t^-1 + 10t^-2 - 6t^-3 + 3t^-4 - t^-5" },
    { "10_105", 10, TABULA_NODORUM_REVERSIBILIS, 6932,
        "t^6 - 8t^5 + 22t^4 - 29t^3 + 22t^2 - 8t + 1",
        "t^7 - 4t^6 + 8t^5 - 12t^4 + 15t^3 - 15t^2 + 14t - 11 + 7t^-1 - 3t^-2 + t^-3" },
    { "10_106", 10, TABULA_NODORUM_CHIRALIS, 6972,
        "t^8 - 4t^7 + 9t^6 - 15t^5 + 17t^4 - 15t^3 + 9t^2 - 4t + 1",
        "t^7 - 3t^6 + 6t^5 - 10t^4 + 12t^3 - 12t^2 + 12t - 9 + 6t^-1 - 3t^-2 + t^-3" },
    { "10_107", 10, TABULA_NODORUM_CHIRALIS, 7012,
        "t^6 - 8t^5 + 22t^4 - 31t^3 + 22t^2 - 8t + 1",
        "-t^5 + 4t^4 - 8t^3 + 12t^2 - 15t + 16 - 14t^-1 + 12t^-2 - 7t^-3 + 3t^-4 - t^-5" },
    { "10_108", 10, TABULA_NODORUM_REVERSIBILIS, 7052,
        "2t^6 - 8t^5 + 14t^4 - 15t^3 + 14t^2 - 8t + 2",
        "-t^6 + 3t^5 - 5t^4 + 8t^3 - 10t^2 + 10t - 9 + 8t^-1 - 5t^-2 + 3t^-3 - t^-4" },
    { "10_109", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 7092,
        "t^8 - 4t^7 + 10t^6 - 17t^5 + 21t^4 - 17t^3 + 10t^2 - 4t + 1",
        "-t^5 + 3t^4 - 7t^3 + 11t^2 - 13t + 15 - 13t^-1 + 11t^-2 - 7t^-3 + 3t^-4 - t^-5" },
    { "10_110", 10, TABULA_NODORUM_CHIRALIS, 7132,
        "t^6 - 8t^5 + 20t^4 - 25t^3 + 20t^2 - 8t + 1",
        "t^7 - 3t^6 + 7t^5 - 11t^4 + 13t^3 - 14t^2 + 13t - 10 + 7t^-1 - 3t^-2 + t^-3" },
    { "10_111", 10, TABULA_NODORUM_REVERSIBILIS, 7172,
        "2t^6 - 9t^5 + 17t^4 - 21t^3 + 17t^2 - 9t + 2",
        "t^10 - 3t^9 + 6t^8 - 10t^7 + 12t^6 - 13t^5 + 12t^4 - 9t^3 + 7t^2 - 3t + 1" },
    { "10_112", 10, TABULA_NODORUM_REVERSIBILIS, 7212,
        "t^8 - 5t^7 + 11t^6 - 17t^5 + 19t^4 - 17t^3 + 11t^2 - 5t + 1",
        "t^3 - 4t^2 + 7t - 10 + 14t^-1 - 14t^-2 + 14t^-3 - 11t^-4 + 7t^-5 - 4t^-6 + t^-7" },
    { "10_113", 10, TABULA_NODORUM_REVERSIBILIS, 7252,
        "2t^6 - 11t^5 + 26t^4 - 33t^3 + 26t^2 - 11t + 2",
        "-t^8 + 5t^7 - 10t^6 + 14t^5 - 18t^4 + 19t^3 - 17t^2 + 14t - 8 + 4t^-1 - t^-2" },
    { "10_114", 10, TABULA_NODORUM_REVERSIBILIS, 7292,
        "2t^6 - 10t^5 + 21t^4 - 27t^3 + 21t^2 - 10t + 2",
        "t^4 - 4t^3 + 8t^2 - 12t + 15 - 15t^-1 + 15t^-2 - 11t^-3 + 7t^-4 - 4t^-5 + t^-6" },
    { "10_115", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 7332,
        "t^6 - 9t^5 + 26t^4 - 37t^3 + 26t^2 - 9t + 1",
        "-t^5 + 4t^4 - 9t^3 + 14t^2 - 17t + 19 - 17t^-1 + 14t^-2 - 9t^-3 + 4t^-4 - t^-5" },
    { "10_116", 10, TABULA_NODORUM_REVERSIBILIS, 7372,
        "t^8 - 5t^7 + 12t^6 - 19t^5 + 21t^4 - 19t^3 + 12t^2 - 5t + 1",
        "t^3 - 4t^2 + 8t - 11 + 15t^-1 - 16t^-2 + 15t^-3 - 12t^-4 + 8t^-5 - 4t^-6 + t^-7" },
    { "10_117", 10, TABULA_NODORUM_CHIRALIS, 7412,
        "2t^6 - 10t^5 + 24t^4 - 31t^3 + 24t^2 - 10t + 2",
        "-t^2 + 4t - 8 + 13t^-1 - 16t^-2 + 18t^-3 - 16t^-4 + 13t^-5 - 9t^-6 + 4t^-7 - t^-8" },
    { "10_118", 10, TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA, 7452,
        "t^8 - 5t^7 + 12t^6 - 19t^5 + 23t^4 - 19t^3 + 12t^2 - 5t + 1",
        "-t^5 + 4t^4 - 8t^3 + 12t^2 - 15t + 17 - 15t^-1 + 12t^-2 - 8t^-3 + 4t^-4 - t^-5" },
    { "10_119", 10, TABULA_NODORUM_CHIRALIS, 7492,
        "2t^6 - 10t^5 + 23t^4 - 31t^3 + 23t^2 - 10t + 2",
        "t^4 - 4t^3 + 9t^2 - 13t + 16 - 17t^-1 + 16t^-2 - 12t^-3 + 8t^-4 - 4t^-5 + t^-6" },
    { "10_120", 10, TABULA_NODORUM_REVERSIBILIS, 7532,
        "8t^4 - 26t^3 + 37t^2 - 26t + 8",
        "t^12 - 4t^11 + 8t^10 - 13t^9 + 16t^8 - 18t^7 + 17t^6 - 13t^5 + 10t^4 - 4t^3 + t^2" },
    { "10_121", 10, TABULA_NODORUM_REVERSIBILIS, 7572,
        "2t^6 - 11t^5 + 27t^4 - 35t^3 + 27t^2 - 11t + 2",
        "-t^8 + 4t^7 - 9t^6 + 14t^5 - 18t^4 + 20t^3 - 18t^2 + 15t - 10 + 5t^-1 - t^-2" },
    { "10_122", 10, TABULA_NODORUM_REVERSIBILIS, 7612,
        "2t^6 - 11t^5 + 24t^4 - 31t^3 + 24t^2 - 11t + 2",
        "t^4 - 4t^3 + 8t^2 - 13t + 17 - 17t^-1 + 17t^-2 - 13t^-3 + 9t^-4 - 5t^-5 + t^-6" },
    { "10_123", 10, TABULA_NODORUM_AMPHICHIRALIS_PLENA, 7652,
        "t^8 - 6t^7 + 15t^6 - 24t^5 + 29t^4 - 24t^3 + 15t^2 - 6t + 1",
        "-t^5 + 5t^4 - 10t^3 + 15t^2 - 19t + 21 - 19t^-1 + 15t^-2 - 10t^-3 + 5t^-4 - t^-5" },
    { "10_124", 10, TABULA_NODORUM_REVERSIBILIS, 7692,
        "t^8 - t^7 + t^5 - t^4 + t^3 - t + 1",
        "-t^10 + t^6 + t^4" },
    { "10_125", 10, TABULA_NODORUM_REVERSIBILIS, 7732,
        "t^6 - 2t^5 + 2t^4 - t^3 + 2t^2 - 2t + 1",
        "-t^4 + t^3 - t^2 + 2t - 1 + 2t^-1 - t^-2 + t^-3 - t^-4" },
    { "10_126", 10, TABULA_NODORUM_REVERSIBILIS, 7772,
        "t^6 - 2t^5 + 4t^4 - 5t^3 + 4t^2 - 2t + 1",
        "-1 + 2t^-1 - 2t^-2 + 4t^-3 - 3t^-4 + 3t^-5 - 2t^-6 + t^-7 - t^-8" },
    { "10_127", 10, TABULA_NODORUM_REVERSIBILIS, 7812,
        "t^6 - 4t^5 + 6t^4 - 7t^3 + 6t^2 - 4t + 1",
        "t^10 - 2t^9 + 3t^8 - 5t^7 + 5t^6 - 5t^5 + 4t^4 - 2t^3 + 2t^2" },
    { "10_128", 10, TABULA_NODORUM_REVERSIBILIS, 7852,
        "2t^6 - 3t^5 + t^4 + t^3 + t^2 - 3t + 2",
        "-t^10 + t^9 - 2t^8 + 2t^7 - t^6 + 2t^5 - t^4 + t^3" },
    { "10_129", 10, TABULA_NODORUM_REVERSIBILIS, 7892,
        "2t^4 - 6t^3 + 9t^2 - 6t + 2",
        "-t^5 + 2t^4 - 3t^3 + 4t^2 - 4t + 5 - 3t^-1 + 2t^-2 - t^-3" },
    { "10_130", 10, TABULA_NODORUM_REVERSIBILIS, 7932,
        "2t^4 - 4t^3 + 5t^2 - 4t + 2",
        "-t + 2 - 2t^-1 + 3t^-2 - 2t^-3 + 3t^-4 - 2t^-5 + t^-6 - t^-7" },
    { "10_131", 10, TABULA_NODORUM_REVERSIBILIS, 7972,
        "2t^4 - 8t^3 + 11t^2 - 8t + 2",
        "t^9 - 2t^8 + 3t^7 - 5t^6 + 5t^5 - 5t^4 + 5t^3 - 3t^2 + 2t" },
    { "10_132", 10, TABULA_NODORUM_REVERSIBILIS, 8012,
        "t^4 - t^3 + t^2 - t + 1",
        "t^-2 + t^-4 - t^-5 + t^-6 - t^-7" },
    { "10_133", 10, TABULA_NODORUM_REVERSIBILIS, 8052,
        "t^4 - 5t^3 + 7t^2 - 5t + 1",
        "t^9 - 2t^8 + 2t^7 - 3t^6 + 3t^5 - 3t^4 + 3t^3 - t^2 + t" },
    { "10_134", 10, TABULA_NODORUM_REVERSIBILIS, 8092,
        "2t^6 - 4t^5 + 4t^4 - 3t^3 + 4t^2 - 4t + 2",
        "t^11 - 3t^10 + 3t^9 - 4t^8 + 4t^7 - 3t^6 + 3t^5 - t^4 + t^3" },
    { "10_135", 10, TABULA_NODORUM_REVERSIBILIS, 8132,
        "3t^4 - 9t^3 + 13t^2 - 9t + 3",
        "-t^5 + 2t^4 - 4t^3 + 6t^2 - 6t + 7 - 5t^-1 + 4t^-2 - 2t^-3" },
    { "10_136", 10, TABULA_NODORUM_REVERSIBILIS, 8172,
        "t^4 - 4t^3 + 5t^2 - 4t + 1",
        "t^3 - 2t^2 + 2t - 2 + 3t^-1 - 2t^-2 + 2t^-3 - t^-4" },
    { "10_137", 10, TABULA_NODORUM_REVERSIBILIS, 8212,
        "t^4 - 6t^3 + 11t^2 - 6t + 1",
        "t^6 - 2t^5 + 3t^4 - 4t^3 + 4t^2 - 4t + 4 - 2t^-1 + t^-2" },
    { "10_138", 10, TABULA_NODORUM_REVERSIBILIS, 8252,
        "t^6 - 5t^5 + 8t^4 - 7t^3 + 8t^2 - 5t + 1",
        "2t^5 - 4t^4 + 5t^3 - 6t^2 + 6t - 5 + 4t^-1 - 2t^-2 + t^-3" },
    { "10_139", 10, TABULA_NODORUM_REVERSIBILIS, 8292,
        "t^8 - t^7 + 2t^5 - 3t^4 + 2t^3 - t + 1",
        "-t^12 + t^11 - t^10 + t^9 - t^8 + t^6 + t^4" },
    { "10_140", 10, TABULA_NODORUM_REVERSIBILIS, 8332,
        "t^4 - 2t^3 + 3t^2 - 2t + 1",
        "-t^7 + t^6 - t^5 + 2t^4 - t^3 + t^2 - t + 1" },
    { "10_141", 10, TABULA_NODORUM_REVERSIBILIS, 8372,
        "t^6 - 3t^5 + 4t^4 - 5t^3 + 4t^2 - 3t + 1",
        "t^6 - 2t^5 + 2t^4 - 3t^3 + 4t^2 - 3t + 3 - 2t^-1 + t^-2" },
    { "10_142", 10, TABULA_NODORUM_REVERSIBILIS, 8412,
        "2t^6 - 3t^5 + 2t^4 - t^3 + 2t^2 - 3t + 2",
        "-2t^10 + 2t^9 - 2t^8 + 3t^7 - 2t^6 + 2t^5 - t^4 + t^3" },
    { "10_143", 10, TABULA_NODORUM_REVERSIBILIS, 8452,
        "t^6 - 3t^5 + 6t^4 - 7t^3 + 6t^2 - 3t + 1",
        "-1 + 3t^-1 - 3t^-2 + 5t^-3 - 5t^-4 + 4t^-5 - 3t^-6 + 2t^-7 - t^-8" },
    { "10_144", 10, TABULA_NODORUM_REVERSIBILIS, 8492,
        "3t^4 - 10t^3 + 13t^2 - 10t + 3",
        "t^7 - 3t^6 + 5t^5 - 6t^4 + 7t^3 - 7t^2 + 5t - 3 + 2t^-1" },
    { "10_145", 10, TABULA_NODORUM_REVERSIBILIS, 8532,
        "t^4 + t^3 - 3t^2 + t + 1",
        "t^-2 + t^-7 - t^-8 + t^-9 - t^-10" },
    { "10_146", 10, TABULA_NODORUM_REVERSIBILIS, 8572,
        "2t^4 - 8t^3 + 13t^2 - 8t + 2",
        "-t^3 + 3t^2 - 4t + 6 - 6t^-1 + 5t^-2 - 4t^-3 + 3t^-4 - t^-5" },
    { "10_147", 10, TABULA_NODORUM_CHIRALIS, 8612,
        "2t^4 - 7t^3 + 9t^2 - 7t + 2",
        "t^5 - 3t^4 + 4t^3 - 4t^2 + 5t - 4 + 3t^-1 - 2t^-2 + t^-3" },
    { "10_148", 10, TABULA_NODORUM_CHIRALIS, 8652,
        "t^6 - 3t^5 + 7t^4 - 9t^3 + 7t^2 - 3t + 1",
        "-1 + 3t^-1 - 4t^-2 + 6t^-3 - 5t^-4 + 5t^-5 - 4t^-6 + 2t^-7 - t^-8" },
    { "10_149", 10, TABULA_NODORUM_CHIRALIS, 8692,
        "t^6 - 5t^5 + 9t^4 - 11t^3 + 9t^2 - 5t + 1",
        "t^10 - 3t^9 + 5t^8 - 7t^7 + 7t^6 - 7t^5 + 6t^4 - 3t^3 + 2t^2" },
    { "10_150", 10, TABULA_NODORUM_CHIRALIS, 8732,
        "t^6 - 4t^5 + 6t^4 - 7t^3 + 6t^2 - 4t + 1",
        "t^8 - 3t^7 + 4t^6 - 5t^5 + 5t^4 - 4t^3 + 4t^2 - 2t + 1" },
    { "10_151", 10, TABULA_NODORUM_CHIRALIS, 8772,
        "t^6 - 4t^5 + 10t^4 - 13t^3 + 10t^2 - 4t + 1",
        "-t^2 + 3t - 5 + 7t^-1 - 7t^-2 + 8t^-3 - 6t^-4 + 4t^-5 - 2t^-6" },
    { "10_152", 10, TABULA_NODORUM_REVERSIBILIS, 8812,
        "t^8 - t^7 - t^6 + 4t^5 - 5t^4 + 4t^3 - t^2 - t + 1",
        "t^13 - 2t^12 + 2t^11 - 3t^10 + 2t^9 - 2t^8 + t^7 + t^6 + t^4" },
    { "10_153", 10, TABULA_NODORUM_CHIRALIS, 8852,
        "t^6 - t^5 - t^4 + 3t^3 - t^2 - t + 1",
        "-t^4 + t^3 - t^2 + t + 1 + t^-2 - t^-3 + t^-4 - t^-5" },
    { "10_154", 10, TABULA_NODORUM_REVERSIBILIS, 8892,
        "t^6 - 4t^4 + 7t^3 - 4t^2 + 1",
        "t^12 - 2t^11 + 2t^10 - 3t^9 + 2t^8 - 2t^7 + 2t^6 + t^3" },
    { "10_155", 10, TABULA_NODORUM_REVERSIBILIS, 8932,
        "t^6 - 3t^5 + 5t^4 - 7t^3 + 5t^2 - 3t + 1",
        "t^2 - 2t + 4 - 4t^-1 + 4t^-2 - 4t^-3 + 3t^-4 - 2t^-5 + t^-6" },
    { "10_156", 10, TABULA_NODORUM_REVERSIBILIS, 8972,
        "t^6 - 4t^5 + 8t^4 - 9t^3 + 8t^2 - 4t + 1",
        "-t^2 + 3t - 4 + 6t^-1 - 6t^-2 + 6t^-3 - 5t^-4 + 3t^-5 - t^-6" },
    { "10_157", 10, TABULA_NODORUM_REVERSIBILIS, 9012,
        "t^6 - 6t^5 + 11t^4 - 13t^3 + 11t^2 - 6t + 1",
        "2t^-2 - 4t^-3 + 7t^-4 - 8t^-5 + 9t^-6 - 8t^-7 + 6t^-8 - 4t^-9 + t^-10" },
    { "10_158", 10, TABULA_NODORUM_REVERSIBILIS, 9052,
        "t^6 - 4t^5 + 10t^4 - 15t^3 + 10t^2 - 4t + 1",
        "2t^4 - 4t^3 + 6t^2 - 8t + 8 - 7t^-1 + 6t^-2 - 3t^-3 + t^-4" },
    { "10_159", 10, TABULA_NODORUM_REVERSIBILIS, 9092,
        "t^6 - 4t^5 + 9t^4 - 11t^3 + 9t^2 - 4t + 1",
        "-t^8 + 3t^7 - 5t^6 + 6t^5 - 7t^4 + 7t^3 - 5t^2 + 4t - 1" },
    { "10_160", 10, TABULA_NODORUM_REVERSIBILIS, 9132,
        "t^6 - 4t^5 + 4t^4 - 3t^3 + 4t^2 - 4t + 1",
        "-2t^7 + 3t^6 - 3t^5 + 4t^4 - 3t^3 + 3t^2 - 2t + 1" },
    { "10_161", 10, TABULA_NODORUM_REVERSIBILIS, 9172,
        "t^6 - 2t^4 + 3t^3 - 2t^2 + 1",
        "-t^11 + t^10 - t^9 + t^8 - t^7 + t^6 + t^3" },
    { "10_162", 10, TABULA_NODORUM_REVERSIBILIS, 9212,
        "3t^4 - 9t^3 + 11t^2 - 9t + 3",
        "2t - 3 + 5t^-1 - 6t^-2 + 6t^-3 - 6t^-4 + 4t^-5 - 2t^-6 + t^-7" },
    { "10_163", 10, TABULA_NODORUM_REVERSIBILIS, 9252,
        "t^6 - 5t^5 + 12t^4 - 15t^3 + 12t^2 - 5t + 1",
        "-2t^6 + 5t^5 - 7t^4 + 9t^3 - 9t^2 + 8t - 6 + 4t^-1 - t^-2" },
    { "10_164", 10, TABULA_NODORUM_REVERSIBILIS, 9292,
        "3t^4 - 11t^3 + 17t^2 - 11t + 3",
        "-2t^3 + 5t^2 - 6t + 8 - 8t^-1 + 7t^-2 - 5t^-3 + 3t^-4 - t^-5" },
    { "10_165", 10, TABULA_NODORUM_REVERSIBILIS, 9332,
        "2t^4 - 10t^3 + 15t^2 - 10t + 2",
        "2t^-1 - 4t^-2 + 6t^-3 - 6t^-4 + 7t^-5 - 6t^-6 + 4t^-7 - 3t^-8 + t^-9" },
};

constans i32 TABULA_NODORUM_NUMERUS = 250;
#line 1 "knotapel/demo_116_named_spectra/main.c"
/*
 * KNOTAPEL DEMO 116: Named Spectra
 * ================================================================
 *
 * Demo 115 computed every construction-word alternative of D112's 12
 * honest polygons exactly, but could only name classes against the 12
 * source knots (plus hand-checked trefoil composites and 8_20), and 1,432
 * of 7_2's alternatives had no Jones polynomial (their best projections
 * had 21-30 crossings). This demo re-runs D115 with two new house tools:
 *
 *   - laqueus_pd_simplificare: greedy Reidemeister I/II on the PD code
 *     of a projection. Exact (an isotopy); a result with 0 crossings is
 *     a PROOF of the unknot, and the reduced crossing count is an upper
 *     bound on the crossing number;
 *   - tabula_nodorum: the 250 prime knots up to 10 crossings (names,
 *     symmetry and PD codes from KnotInfo, polynomials computed by
 *     laqueus) and tabula_nodorum_agnoscere, which names a knot by
 *     Alexander + Jones as a prime (as drawn or mirrored) or a sum of two
 *     table knots.
 *
 * Names are KnotInfo's: "K" is the KnotInfo diagram's chirality, "K*"
 * its mirror (D115 used D112's braid chirality - Part A translates).
 * A name means Alexander AND Jones match - not a proof of type (5_1 and
 * 10_132* share both; every such ambiguity is printed, "a|b").
 * Beyond the table: span(Jones) <= crossing number for every knot
 * (Kauffman, Murasugi, Thistlethwaite), so a class whose Jones span
 * exceeds 10 is PROVEN to have more than 10 crossings; every prime knot
 * with <= 10 crossings is in the table.
 *
 *   Part A  the honest polygons (as D115) and the chirality translation
 *           D112 -> KnotInfo for the 12 sources
 *   Part B  every spectrum, named; D115's Alexander-level numbers
 *           reproduced exactly; mirror theorem checked, then used
 *   Part C  reachability with real names, everything beyond the 12, and
 *           crossing-number intervals [span(Jones), reduced crossings]
 *           for what the table cannot name
 *   Part D  stability (all fewest-vertex polygons of 6_3 and 4_1), named
 *
 * Jones: generic projection -> PD -> R1/R2 reduction; if still above 20
 * crossings, the fewest-crossing projections with direction components
 * in -1..1, then -2..2, each reduced the same way. Unknots PROVEN here:
 * reduced to 0 crossings, or Alexander 1 on a reduced diagram of <= 10
 * crossings (no nontrivial knot with <= 10 crossings has trivial
 * Alexander polynomial; the first are 11n34 and 11n42). Unknots by
 * CITATION, "unknot (TS)": Alexander 1 and Jones 1 computed on a diagram
 * of <= 20 crossings - the Jones polynomial detects the unknot for every
 * knot up to 22 crossings (Tuzun, Sikora, J. Knot Theory Ramifications
 * 27(3), 2018; extended to 24 crossings, arXiv:2003.06724) - an external
 * computer verification, kept apart from what this demo proves.
 *
 * House libraries: includes laqueus.h and tabula_nodorum.h, hence
 * latina.h (Roman numerals and Latin keywords are macros here). Build and
 * run from the repo root:
 *   ./bin/aedilis knotapel/demo_116_named_spectra/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Frozen copy: demo-snapshot.c (knotapel/archive.sh).
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
 * Test infrastructure
 * ================================================================ */

static int n_pass = 0;
static int n_fail = 0;

static void
check (
    const char *msg,
    int         ok)
{
    if (ok) {
        n_pass++;
        printf("  PASS: %s\n", msg);
    } else {
        n_fail++;
        printf("  FAIL: %s\n", msg);
    }
}

/* ================================================================
 * Data
 *
 * RAW_DATA: the "RAW" lines of knotapel/demo_114_exact_audit/
 * d112_export.txt (Demo 112's braid_to_polygon output, exported by
 * compiling D112's main.c unmodified), verbatim.
 * ================================================================ */

static const char *const RAW_DATA[] = {
    "RAW 3_1 18 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 20,-100,60 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,-105,60 0,-105,0",
    "RAW 4_1 29 0,0,0 20,1,10 20,0,20 40,-1,30 40,0,40 40,0,60 20,1,70 20,0,80 20,-100,80 20,-100,0 20,0,0 0,-1,10 0,0,20 0,0,40 20,1,50 20,0,60 40,-1,70 40,0,80 40,-105,80 40,-105,0 40,0,0 40,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 0,0,80 0,-110,80 0,-110,0",
    "RAW 5_1 26 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 20,-100,100 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,-105,100 0,-105,0",
    "RAW 5_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "RAW 6_1 48 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,1,70 20,0,80 20,0,100 40,-1,110 40,0,120 60,1,130 60,0,140 60,-100,140 60,-100,0 60,0,0 60,0,80 40,-1,90 40,0,100 20,1,110 20,0,120 20,0,140 20,-105,140 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,-1,50 40,0,60 40,0,80 60,1,90 60,0,100 60,0,120 40,-1,130 40,0,140 40,-110,140 40,-110,0 40,0,0 40,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 0,0,140 0,-115,140 0,-115,0",
    "RAW 6_2 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 0,-110,120 0,-110,0",
    "RAW 6_3 37 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,-100,120 20,-100,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,-105,120 40,-105,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,120 0,-110,120 0,-110,0",
    "RAW 7_1 34 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 0,-1,110 0,0,120 20,1,130 20,0,140 20,-100,140 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 20,1,110 20,0,120 0,-1,130 0,0,140 0,-105,140 0,-105,0",
    "RAW 7_2 56 0,0,0 20,-1,10 20,0,20 0,1,30 0,0,40 20,-1,50 20,0,60 40,-1,70 40,0,80 40,0,100 20,1,110 20,0,120 20,0,140 40,1,150 40,0,160 60,-1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,1,130 40,0,140 20,-1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,1,10 0,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,1,90 20,0,100 40,-1,110 40,0,120 60,-1,130 60,0,140 60,0,160 40,1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,180 0,-115,180 0,-115,0",
    "RAW 7_3 45 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 20,1,50 20,0,60 0,-1,70 0,0,80 20,1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-100,160 20,-100,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 0,-1,50 0,0,60 20,1,70 20,0,80 0,-1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-105,160 40,-105,0 40,0,0 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    "RAW 7_4 56 0,0,0 20,1,10 20,0,20 0,-1,30 0,0,40 0,0,60 20,-1,70 20,0,80 40,1,90 40,0,100 20,-1,110 20,0,120 20,0,140 40,-1,150 40,0,160 60,1,170 60,0,180 60,-100,180 60,-100,0 60,0,0 60,0,120 40,-1,130 40,0,140 20,1,150 20,0,160 20,0,180 20,-105,180 20,-105,0 20,0,0 0,-1,10 0,0,20 20,1,30 20,0,40 40,1,50 40,0,60 40,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 60,1,130 60,0,140 60,0,160 40,-1,170 40,0,180 40,-110,180 40,-110,0 40,0,0 40,0,40 20,-1,50 20,0,60 0,1,70 0,0,80 0,0,180 0,-115,180 0,-115,0",
    "RAW 8_18 49 0,0,0 20,-1,10 20,0,20 40,1,30 40,0,40 40,0,60 20,-1,70 20,0,80 0,1,90 0,0,100 0,0,120 20,-1,130 20,0,140 40,1,150 40,0,160 40,-100,160 40,-100,0 40,0,0 40,0,20 20,-1,30 20,0,40 0,1,50 0,0,60 0,0,80 20,-1,90 20,0,100 40,1,110 40,0,120 40,0,140 20,-1,150 20,0,160 20,-105,160 20,-105,0 20,0,0 0,1,10 0,0,20 0,0,40 20,-1,50 20,0,60 40,1,70 40,0,80 40,0,100 20,-1,110 20,0,120 0,1,130 0,0,140 0,0,160 0,-110,160 0,-110,0",
    NULL
};

#define N_KNOTS 12

static const char *const KNOT_CODE[N_KNOTS] = {
    "3_1", "4_1", "5_1", "5_2", "6_1", "6_2",
    "6_3", "7_1", "7_2", "7_3", "7_4", "8_18"
};

/* table Alexander polynomials (laqueus normal form) */
static const char *const KNOT_ALEXANDER[N_KNOTS] = {
    "t^2 - t + 1",
    "t^2 - 3t + 1",
    "t^4 - t^3 + t^2 - t + 1",
    "2t^2 - 3t + 2",
    "2t^2 - 5t + 2",
    "t^4 - 3t^3 + 3t^2 - 3t + 1",
    "t^4 - 3t^3 + 5t^2 - 3t + 1",
    "t^6 - t^5 + t^4 - t^3 + t^2 - t + 1",
    "3t^2 - 5t + 3",
    "2t^4 - 3t^3 + 3t^2 - 3t + 2",
    "4t^2 - 7t + 4",
    "t^6 - 5t^5 + 10t^4 - 13t^3 + 10t^2 - 5t + 1"
};

/* table determinants |Delta(-1)| */
static const unsigned long KNOT_DET[N_KNOTS] = {
    3, 5, 5, 7, 9, 11, 13, 7, 11, 13, 15, 45
};

/* D112's published reachability out-degree */
static const unsigned D112_OUT_DEGREE[N_KNOTS] = {
    1, 3, 1, 3, 1, 3, 10, 1, 9, 1, 1, 1
};

/* D115's published numbers (findings.md, after review I): the
 * Alexander-level results do not depend on how Jones is computed or on
 * naming, so D116 must reproduce them exactly */
static const unsigned D115_VERTS[N_KNOTS] = {
    6, 8, 10, 11, 11, 10, 11, 14, 15, 13, 13, 12
};
static const unsigned D115_DISTINCT[N_KNOTS] = {
    2, 2, 3, 4, 6, 4, 6, 4, 47, 6, 4, 6
};
static const unsigned long D115_MAX_DET[N_KNOTS] = {
    3, 5, 5, 7, 15, 11, 13, 7, 85, 13, 15, 45
};
static const unsigned D115_TRIVIAL[N_KNOTS] = {
    6, 28, 80, 176, 156, 80, 144, 1120, 1844, 676, 576, 336
};
static const unsigned D115_CERTIFIED[N_KNOTS] = {
    6, 28, 80, 176, 156, 80, 144, 1120, 106, 676, 576, 288
};
/* itself + mirror after mirror filling (D115 table "itself / mirror") */
static const unsigned D115_SELF[N_KNOTS] = {
    2, 4, 8, 16, 8, 8, 8, 32, 16, 4, 64, 8
};
static const unsigned D115_MISSING_7_2 = 1432;

#define JONES_CAP        20
#define JONES_CAP_REF    22
#define UNKNOT_CERT      10
#define TABLE_MAX        10
#define NO_DIAGRAM      999u
#define MAX_VERTS       128
#define MAX_ALTS       4096
#define MAX_CLASSES     256
#define MAX_CAND          8

/* ================================================================
 * Small helpers
 * ================================================================ */

static chorda
empty_chorda (void)
{
    chorda c;

    c.datum   = NULL;
    c.mensura = 0;
    return c;
}

static int
chorda_is (
    chorda      c,
    const char *lit)
{
    return c.datum != NULL && chorda_aequalis_literis(c, lit);
}

static int
chorda_same (
    chorda a,
    chorda b)
{
    return a.datum != NULL && b.datum != NULL && chorda_aequalis(a, b);
}

static void
print_chorda (
    chorda c)
{
    if (c.datum == NULL)
        printf("-");
    else
        printf("%.*s", (int)c.mensura, (const char*)c.datum);
}

static unsigned long
chorda_to_ulong (
    chorda c)
{
    char buf[32];
    int  k;
    int  n = c.mensura < 31 ? (int)c.mensura : 31;

    for (k = 0; k < n; k++)
        buf[k] = (char)c.datum[k];
    buf[n] = '\0';
    return strtoul(buf, NULL, 10);
}

/* "RAW <code> <n> x,y,z ..." -> vertex array */
static long
raw_polygon (
    const char *code,
    Piscina    *pool,
    Punctum   **out)
{
    size_t lc = strlen(code);
    int    k;

    for (k = 0; RAW_DATA[k] != NULL; k++) {
        const char *s = RAW_DATA[k];

        if (strncmp(s, "RAW ", 4) == 0 && strncmp(s + 4, code, lc) == 0
            && s[4 + lc] == ' ') {
            char *end;
            long  n = strtol(s + 5 + lc, &end, 10);
            long  j;

            if (n < 3 || n > MAX_VERTS)
                return 0;
            *out = (Punctum*)piscina_allocare(pool,
                (memoriae_index)n * sizeof(Punctum));
            for (j = 0; j < n; j++) {
                long x, y, z;

                x = strtol(end, &end, 10);
                y = strtol(end + 1, &end, 10);
                z = strtol(end + 1, &end, 10);
                (*out)[j] = situs_punctum((s64)x, (s64)y, (s64)z);
            }
            return n;
        }
    }
    return 0;
}

static int
make_knot (
    const Punctum *pts,
    long           n,
    Piscina       *pool,
    Laqueus       *out)
{
    i32 starts[2];

    starts[0] = 0;
    starts[1] = (i32)n;
    return laqueus_ex_punctis(pts, starts, 1, pool, out) ? 1 : 0;
}

/* the raw polygon started at vertex `shift`, reversed if rev, then
 * simplified by legal moves */
static int
variant (
    const Punctum *raw,
    long           n,
    long           shift,
    int            rev,
    Piscina       *pool,
    Laqueus       *out)
{
    Punctum *v = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    Laqueus  l;
    long     j;

    for (j = 0; j < n; j++)
        v[j] = raw[rev ? (shift - j + n) % n : (shift + j) % n];
    return make_knot(v, n, pool, &l) && laqueus_simplificare(l, pool, out);
}

/* alternative `choices` of base: vertex k >= 3 reflected through the
 * plane of vertices 0, 1, 2 iff bit k-3 is set; 0 if the reflection is
 * undefined (vertices 0, 1, 2 collinear) */
static int
build_alternative (
    Laqueus        base,
    unsigned long  choices,
    Piscina       *pool,
    Laqueus       *out)
{
    i32      n   = laqueus_numerus(base);
    Punctum *pts = (Punctum*)piscina_allocare(pool,
        (memoriae_index)n * sizeof(Punctum));
    Punctum  c0  = laqueus_vertex(base, 0);
    Punctum  c1  = laqueus_vertex(base, 1);
    Punctum  c2  = laqueus_vertex(base, 2);
    i32      k;
    int      ok  = 1;

    for (k = 0; k < n; k++) {
        Punctum v = laqueus_vertex(base, k);

        if (k >= 3 && ((choices >> (k - 3)) & 1UL)) {
            if (!situs_reflexio(c0, c1, c2, v, pool, &pts[k])) {
                ok     = 0;
                pts[k] = v;
            }
        } else {
            pts[k] = v;
        }
    }
    return make_knot(pts, (long)n, pool, out) && ok;
}

/* ================================================================
 * Exact classification
 * ================================================================ */

typedef struct {
    int           simple;
    unsigned      crossings;   /* generic projection */
    unsigned      reduced;     /* fewest crossings after R1/R2 (an upper
                                  bound on the crossing number);
                                  NO_DIAGRAM if not computed */
    unsigned long det;
    unsigned      alex_span;   /* degree of the normalized Alexander */
    int           unknot_proven;
    chorda        alexander;
    chorda        jones;       /* NULL: not computed */
    chorda        jones_mirror;
    int           jones_filled; /* taken from the mirror partner */
} Exact;

/* diagram -> PD -> greedy Reidemeister I/II */
static int
reduced_pd (
    Diagramma  d,
    Piscina   *pool,
    i32      **pd,
    i32       *c)
{
    i32 *raw;

    return diagramma_pd(d, pool, &raw)
        && laqueus_pd_simplificare(raw, diagramma_numerus(d), pool, pd, c);
}

/* jones_cap 0: Alexander only */
static Exact
classify (
    Laqueus   l,
    unsigned  jones_cap,
    Piscina  *scratch,
    Piscina  *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Exact          e;
    Diagramma      d;
    Polynomium     p;

    e.simple        = laqueus_simplex(l, scratch) ? 1 : 0;
    e.crossings     = 0;
    e.reduced       = NO_DIAGRAM;
    e.det           = 0;
    e.alex_span     = 0;
    e.unknot_proven = 0;
    e.alexander     = empty_chorda();
    e.jones         = empty_chorda();
    e.jones_mirror  = empty_chorda();
    e.jones_filled  = 0;
    if (e.simple && laqueus_diagramma_genericum(l, scratch, &d)) {
        Magnus m;
        i32   *pd = NULL;
        i32    c  = 0;

        e.crossings = (unsigned)diagramma_numerus(d);
        if (diagramma_alexander(d, scratch, &p)) {
            e.alexander = chorda_transcribere(
                polynomium_ad_chordam(p, 't', scratch), out_pool);
            e.alex_span = (unsigned)(polynomium_gradus_summus(p)
                - polynomium_gradus_imus(p));
        }
        if (diagramma_determinans(d, scratch, &m))
            e.det = chorda_to_ulong(magnus_ad_chordam(m, scratch));
        if (jones_cap > 0 && reduced_pd(d, scratch, &pd, &c)) {
            int trivial = chorda_is(e.alexander, "1");
            int radius;

            /* search other directions only while Jones is out of reach
             * or a trivial Alexander polynomial is not yet certified */
            for (radius = 1; radius <= 2; radius++) {
                Diagramma dm;
                i32      *pdm;
                i32       cm;

                if (!((unsigned)c > jones_cap
                        || (trivial && c > 0 && c > UNKNOT_CERT)))
                    break;
                if (laqueus_diagramma_minimum(l, (i32)radius, scratch, &dm)
                    && reduced_pd(dm, scratch, &pdm, &cm) && cm < c) {
                    pd = pdm;
                    c  = cm;
                }
            }
            e.reduced = (unsigned)c;
            if (c == 0) {
                e.jones        = chorda_ex_literis("1", out_pool);
                e.jones_mirror = e.jones;
            } else if ((unsigned)c <= jones_cap
                       && laqueus_jones_ex_pd(pd, c, scratch, &p)) {
                e.jones = chorda_transcribere(
                    polynomium_ad_chordam(p, 't', scratch), out_pool);
                e.jones_mirror = chorda_transcribere(polynomium_ad_chordam(
                    polynomium_inversum(p, scratch), 't', scratch), out_pool);
            }
            e.unknot_proven = c == 0 || (trivial && c <= UNKNOT_CERT);
        }
    }
    piscina_reficere(scratch, mark);
    return e;
}

static unsigned reflexio_failures = 0;

static Exact
classify_alternative (
    Laqueus        base,
    unsigned long  choices,
    unsigned       jones_cap,
    Piscina       *scratch,
    Piscina       *out_pool)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Laqueus        alt;
    Exact          e;

    if (!build_alternative(base, choices, scratch, &alt))
        reflexio_failures++;
    e = classify(alt, jones_cap, scratch, out_pool);
    piscina_reficere(scratch, mark);
    return e;
}

/* ================================================================
 * Naming with the knot table
 * ================================================================ */

typedef struct {
    chorda alexander;
    chorda jones;          /* the raw braid polygon's (D112's chirality) */
} Reference;

static Reference      refs[N_KNOTS];
static TabulaNodorum *table;

enum {
    ST_UNKNOT,      /* proven */
    ST_UNKNOT_Q,    /* Alexander 1 and Jones 1 on a <= 20-crossing diagram:
                       the unknot by Tuzun-Sikora (cited, not proven) */
    ST_TABLE,       /* agnoscere found candidates */
    ST_BEYOND,      /* no candidate, span(Jones) > 10: proven > 10 crossings */
    ST_UNMATCHED,   /* no candidate, span(Jones) <= 10 */
    ST_NOJONES      /* Jones not computed: Alexander only */
};

static const char *const STATUS_NAME[] = {
    "unknot", "unknot (TS)", "table", "beyond", "unmatched", "no Jones"
};

static unsigned
jones_span (
    chorda   j,
    Piscina *scratch)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Polynomium     p;
    unsigned       s = 0;

    if (j.datum != NULL && polynomium_ex_chorda(j, 't', scratch, &p))
        s = (unsigned)(polynomium_gradus_summus(p)
            - polynomium_gradus_imus(p));
    piscina_reficere(scratch, mark);
    return s;
}

static void
append_name (
    char       *buf,
    size_t      size,
    const char *piece)
{
    size_t n = strlen(buf);

    if (n + strlen(piece) + 2 < size) {
        if (n > 0)
            strcat(buf, "|");
        strcat(buf, piece);
    } else if (n + 4 < size && strcmp(buf + n - 3, "...") != 0) {
        strcat(buf, "...");
    }
}

static void
agnitio_name (
    const Agnitio *a,
    char          *out)
{
    if (a->secundus == NULL)
        sprintf(out, "%s%s", a->primus->titulus,
            a->primus_speculum ? "*" : "");
    else
        sprintf(out, "%s%s#%s%s", a->primus->titulus,
            a->primus_speculum ? "*" : "", a->secundus->titulus,
            a->secundus_speculum ? "*" : "");
}

/* index of the 12 sources this candidate is (prime, either chirality) */
static int
source_index (
    const Agnitio *a)
{
    int k;

    if (a->secundus != NULL)
        return -1;
    for (k = 0; k < N_KNOTS; k++)
        if (strcmp(a->primus->titulus, KNOT_CODE[k]) == 0)
            return k;
    return -1;
}

typedef struct {
    int      status;
    int      source;       /* one of the 12 (either chirality) or -1 */
    int      composite;    /* first candidate is a sum */
    unsigned candidates;
    unsigned span;         /* Jones span: lower bound on crossing number */
    unsigned cand_c[MAX_CAND];     /* crossing number (sum for a sum) */
    int      cand_prime[MAX_CAND];
    char     first[16];    /* first candidate's table name if prime */
    char     name[96];
} Naming;

static Naming
name_class (
    chorda   alexander,
    chorda   jones,
    int      unknot_proven,
    Piscina *scratch)
{
    PiscinaNotatio mark = piscina_notare(scratch);
    Naming         nm;
    Polynomium     pa, pj;
    Agnitio        cand[MAX_CAND];
    i32            n, k;

    nm.status     = ST_UNMATCHED;
    nm.source     = -1;
    nm.composite  = 0;
    nm.candidates = 0;
    nm.span       = jones_span(jones, scratch);
    nm.name[0]    = '\0';
    nm.first[0]   = '\0';
    if (chorda_is(alexander, "1") && (unknot_proven || chorda_is(jones, "1"))) {
        nm.status = unknot_proven ? ST_UNKNOT : ST_UNKNOT_Q;
        /* Jones 1 is only ever computed on a diagram of <= JONES_CAP
         * crossings (or its mirror partner's) */
        strcpy(nm.name, unknot_proven ? "unknot" : "unknot (TS)");
    } else if (jones.datum == NULL) {
        /* Alexander only: the table primes that share it */
        nm.status = ST_NOJONES;
        for (k = 0; k < (i32)tabula_nodorum_numerus(); k++) {
            const NodusTabulae *t = tabula_nodorum_nodus((i32)k);

            if (chorda_is(alexander, t->alexander)) {
                append_name(nm.name, sizeof(nm.name), t->titulus);
                nm.candidates++;
            }
        }
        if (nm.candidates == 0)
            strcpy(nm.name, "?");
    } else if (polynomium_ex_chorda(alexander, 't', scratch, &pa)
               && polynomium_ex_chorda(jones, 't', scratch, &pj)) {
        n = tabula_nodorum_agnoscere(table, pa, pj, scratch, cand, MAX_CAND);
        nm.candidates = (unsigned)n;
        if (n == 0) {
            nm.status = nm.span > TABLE_MAX ? ST_BEYOND : ST_UNMATCHED;
            strcpy(nm.name, nm.span > TABLE_MAX ? "beyond" : "?");
        } else {
            nm.status    = ST_TABLE;
            nm.composite = cand[0].secundus != NULL;
            for (k = 0; k < n && k < MAX_CAND; k++) {
                char piece[48];

                agnitio_name(&cand[k], piece);
                append_name(nm.name, sizeof(nm.name), piece);
                nm.cand_prime[k] = cand[k].secundus == NULL;
                nm.cand_c[k]     = (unsigned)cand[k].primus->transitus
                    + (cand[k].secundus != NULL
                       ? (unsigned)cand[k].secundus->transitus : 0u);
                if (k == 0 && cand[k].secundus == NULL)
                    strcpy(nm.first, cand[k].primus->titulus);
                if (nm.source < 0)
                    nm.source = source_index(&cand[k]);
            }
        }
    }
    piscina_reficere(scratch, mark);
    return nm;
}

/* ================================================================
 * Spectrum: classes (Alexander | Jones) with populations
 * ================================================================ */

typedef struct {
    chorda        key;
    chorda        alexander;
    chorda        jones;
    unsigned      count;
    Naming        nm;
    unsigned long det;
    unsigned      alex_span;
    unsigned      reduced;     /* min over members */
} Class;

typedef struct {
    Class    c[MAX_CLASSES];
    unsigned n;
    unsigned overflow;
} Spectrum;

static void
spectrum_clear (
    Spectrum *s)
{
    s->n        = 0;
    s->overflow = 0;
}

/* Alexander | Jones (or "?"), and for a trivial Alexander polynomial
 * whether the unknot is proven - so every member of a class has the
 * same name */
static chorda
class_key (
    Exact    e,
    Piscina *pool)
{
    chorda bar = chorda_ex_literis("  |  ", pool);
    chorda j   = e.jones.datum != NULL ? e.jones
               : chorda_ex_literis("?", pool);
    chorda k   = chorda_concatenare(chorda_concatenare(e.alexander, bar,
        pool), j, pool);

    if (chorda_is(e.alexander, "1"))
        k = chorda_concatenare(k, chorda_ex_literis(e.unknot_proven
            ? "  | proven" : "  | unproven", pool), pool);
    return k;
}

static void
spectrum_add (
    Spectrum *s,
    Exact     e,
    Piscina  *work,
    Piscina  *keep)
{
    chorda   key = class_key(e, work);
    unsigned k;

    for (k = 0; k < s->n; k++) {
        if (chorda_aequalis(s->c[k].key, key)) {
            s->c[k].count++;
            if (e.reduced < s->c[k].reduced)
                s->c[k].reduced = e.reduced;
            return;
        }
    }
    if (s->n >= MAX_CLASSES) {
        s->overflow++;
        return;
    }
    s->c[s->n].key       = chorda_transcribere(key, keep);
    s->c[s->n].alexander = chorda_transcribere(e.alexander, keep);
    s->c[s->n].jones     = e.jones.datum != NULL
        ? chorda_transcribere(e.jones, keep) : empty_chorda();
    s->c[s->n].count     = 1;
    s->c[s->n].det       = e.det;
    s->c[s->n].alex_span = e.alex_span;
    s->c[s->n].reduced   = e.reduced;
    s->c[s->n].nm        = name_class(e.alexander, e.jones, e.unknot_proven,
        work);
    s->n++;
}

/* ================================================================
 * Part A: base polygons and the chirality translation
 * ================================================================ */

static Laqueus  chosen[N_KNOTS];
static Punctum *raw_pts[N_KNOTS];
static long     raw_n[N_KNOTS];
static int      fewest[N_KNOTS];

static void
part_a (
    Piscina *keep,
    Piscina *scratch)
{
    int k;

    printf("\n=== Part A: honest polygons (as D115) and D112 -> KnotInfo "
        "chirality ===\n");
    printf("  %-5s %4s %7s %6s %8s  %-22s %s\n", "knot", "raw", "fewest",
        "#var", "chosen", "KnotInfo name(s)", "raw polygon reduced");
    for (k = 0; k < N_KNOTS; k++) {
        const char *code = KNOT_CODE[k];
        Laqueus     raw;
        Exact       er, ec;
        Naming      nm;
        long        n, shift;
        int         rev, best = MAX_VERTS + 1, n_best = 0;
        long        best_shift = 0;
        int         best_rev = 0;
        char        msg[200];

        n = raw_polygon(code, keep, &raw_pts[k]);
        raw_n[k] = n;
        if (n == 0 || !make_knot(raw_pts[k], n, keep, &raw)) {
            check("raw polygon parses", 0);
            continue;
        }
        er = classify(raw, JONES_CAP_REF, scratch, keep);
        refs[k].alexander = er.alexander;
        refs[k].jones     = er.jones;
        for (rev = 0; rev < 2; rev++) {
            for (shift = 0; shift < n; shift++) {
                PiscinaNotatio mark = piscina_notare(keep);
                Laqueus        v;
                int            m;

                if (variant(raw_pts[k], n, shift, rev, keep, &v)) {
                    m = (int)laqueus_numerus(v);
                    if (m < best) {
                        best       = m;
                        n_best     = 1;
                        best_shift = shift;
                        best_rev   = rev;
                    } else if (m == best) {
                        n_best++;
                    }
                }
                piscina_reficere(keep, mark);
            }
        }
        fewest[k] = n_best;
        (void)variant(raw_pts[k], n, best_shift, best_rev, keep, &chosen[k]);
        ec = classify(chosen[k], JONES_CAP_REF, scratch, keep);
        nm = name_class(er.alexander, er.jones, er.unknot_proven, scratch);
        printf("  %-5s %4ld %7d %6d  %s%-6ld  %-22s %u -> %u\n", code, n, best,
            n_best, best_rev ? "r" : "+", best_shift, nm.name, er.crossings,
            er.reduced);
        sprintf(msg, "%s: raw polygon simple, table Alexander, Jones computed",
            code);
        check(msg, er.simple && chorda_is(er.alexander, KNOT_ALEXANDER[k])
            && er.jones.datum != NULL);
        sprintf(msg, "%s: the table names the source %s (Alexander + Jones; "
            "either chirality)", code, code);
        check(msg, nm.status == ST_TABLE && nm.source == k);
        sprintf(msg, "%s: honest polygon has D115's vertex count and the raw "
            "polygon's Alexander and Jones", code);
        check(msg, ec.simple && (unsigned)best == D115_VERTS[k]
            && chorda_same(ec.alexander, er.alexander)
            && chorda_same(ec.jones, er.jones));
    }
    printf("  (KnotInfo name of D112's braid knot: K = the KnotInfo diagram's "
        "chirality, K* = its mirror)\n");
}

/* ================================================================
 * Part B: spectra
 * ================================================================ */

static Spectrum spectra[N_KNOTS];
static Exact    alt_exact[MAX_ALTS];
static unsigned missing_jones[N_KNOTS];

/* All alternatives of base classified. The mirror theorem (alternative
 * c ^ mask is the mirror image of c) is CHECKED on independently
 * computed pairs first; only then USED to fill Jones, the unknot proof
 * and the reduced crossing count from the partner. */
static unsigned long
classify_all (
    Laqueus   base,
    Piscina  *keep,
    Piscina  *scratch,
    unsigned *pair_bad,
    unsigned *filled)
{
    unsigned long count = 1UL << (laqueus_numerus(base) - 3);
    unsigned long mask  = count - 1UL;
    unsigned long c;

    *pair_bad = 0;
    *filled   = 0;
    for (c = 0; c < count; c++)
        alt_exact[c] = classify_alternative(base, c, JONES_CAP, scratch,
            keep);
    for (c = 0; c < count; c++) {
        const Exact *a = &alt_exact[c];
        const Exact *b = &alt_exact[c ^ mask];

        if (a->simple != b->simple)
            (*pair_bad)++;
        else if (a->simple
                 && (!chorda_same(a->alexander, b->alexander)
                     || (a->jones.datum != NULL && b->jones.datum != NULL
                         && !chorda_aequalis(a->jones, b->jones_mirror))))
            (*pair_bad)++;
    }
    for (c = 0; c < count; c++) {
        Exact       *a = &alt_exact[c];
        const Exact *b = &alt_exact[c ^ mask];

        if (!a->simple)
            continue;
        if (a->jones.datum == NULL && b->jones.datum != NULL
            && !b->jones_filled) {
            a->jones        = b->jones_mirror;
            a->jones_mirror = b->jones;
            a->jones_filled = 1;
            (*filled)++;
        }
        if (b->reduced < a->reduced)
            a->reduced = b->reduced;
        if (b->unknot_proven && chorda_is(a->alexander, "1"))
            a->unknot_proven = 1;
    }
    return count;
}

static void
part_b (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    int k;

    printf("\n=== Part B: named spectra ===\n");
    printf("  (a name means Alexander AND Jones match; a|b = several table "
        "matches; '[c..r]' = crossing number between span(Jones) and the "
        "reduced diagram)\n");
    for (k = 0; k < N_KNOTS; k++) {
        const char   *code = KNOT_CODE[k];
        unsigned long count, c;
        unsigned      n_simple = 0, missing = 0, trivial = 0, proven = 0;
        unsigned      pair_bad, filled, self = 0, q;
        unsigned      n_alex = 0;
        unsigned long max_det = 0;
        char          msg[200];
        Spectrum     *sp = &spectra[k];

        spectrum_clear(sp);
        if ((1UL << (laqueus_numerus(chosen[k]) - 3)) > MAX_ALTS) {
            sprintf(msg, "%s: spectrum within %d alternatives", code,
                MAX_ALTS);
            check(msg, 0);
            continue;
        }
        count = classify_all(chosen[k], keep, scratch, &pair_bad, &filled);
        for (c = 0; c < count; c++) {
            const Exact *e = &alt_exact[c];

            if (!e->simple)
                continue;
            n_simple++;
            spectrum_add(sp, *e, work, keep);
            if (e->jones.datum == NULL) missing++;
            if (chorda_is(e->alexander, "1")) {
                trivial++;
                if (e->unknot_proven) proven++;
            }
            if (e->det > max_det) max_det = e->det;
        }
        missing_jones[k] = missing;
        for (q = 0; q < sp->n; q++) {
            unsigned r;
            int      first = 1;

            for (r = 0; r < q; r++)
                if (chorda_aequalis(sp->c[r].alexander, sp->c[q].alexander))
                    first = 0;
            n_alex += (unsigned)first;
            if (sp->c[q].nm.source == k)
                self += sp->c[q].count;
        }
        printf("\n  %s: %u vertices, %lu alternatives, %u simple, %u classes, "
            "%u distinct Alexander; itself (either chirality) %u; unknots "
            "proven %u of %u trivial-Alexander (D115: %u); Jones from the "
            "mirror partner %u, still missing %u; max det %lu (source "
            "%lu)\n", code, (unsigned)laqueus_numerus(chosen[k]), count,
            n_simple, sp->n, n_alex, self, proven, trivial,
            D115_CERTIFIED[k], filled, missing, max_det, KNOT_DET[k]);
        for (q = 0; q < sp->n; q++) {
            const Class *cl = &sp->c[q];

            printf("      %-24s [%4u]  det %-4lu ", cl->nm.name, cl->count,
                cl->det);
            if (cl->nm.status != ST_UNKNOT && cl->nm.status != ST_UNKNOT_Q
                && cl->nm.status != ST_TABLE) {
                if (cl->jones.datum != NULL)
                    printf("[%u..", cl->nm.span);
                else
                    printf("[?..");
                if (cl->reduced == NO_DIAGRAM) printf("?] ");
                else printf("%u] ", cl->reduced);
            }
            print_chorda(cl->alexander);
            printf("  |  ");
            print_chorda(cl->jones);
            printf("\n");
        }
        sprintf(msg, "%s: mirror theorem on independently computed pairs",
            code);
        check(msg, pair_bad == 0);
        sprintf(msg, "%s: D115's Alexander-level numbers reproduced (%lu "
            "alternatives all simple, %u distinct Alexander, max det %lu, %u "
            "trivial)", code, count, D115_DISTINCT[k], D115_MAX_DET[k],
            D115_TRIVIAL[k]);
        check(msg, n_simple == count && sp->overflow == 0
            && n_alex == D115_DISTINCT[k] && max_det == D115_MAX_DET[k]
            && trivial == D115_TRIVIAL[k]);
        sprintf(msg, "%s: every D115 unknot certificate holds (%u proven >= "
            "%u)", code, proven, D115_CERTIFIED[k]);
        check(msg, proven >= D115_CERTIFIED[k]);
        if (k == 8) {
            sprintf(msg, "7_2: itself + mirror at least D115's %u; missing "
                "Jones %u (D115: %u)", D115_SELF[k], missing,
                D115_MISSING_7_2);
            check(msg, self >= D115_SELF[k] && missing <= D115_MISSING_7_2);
        } else {
            sprintf(msg, "%s: itself + mirror = D115's %u, no Jones missing",
                code, D115_SELF[k]);
            check(msg, self == D115_SELF[k] && missing == 0);
        }
    }
}

/* ================================================================
 * Part C: reachability, beyond the 12, and what the table cannot name
 * ================================================================ */

static void
part_c (void)
{
    int      k, j;
    unsigned status_classes[6], status_alts[6], s;

    printf("\n=== Part C: reachability (x = source j, either chirality, "
        "among i's alternatives) ===\n  %-6s", "");
    for (j = 0; j < N_KNOTS; j++)
        printf("%5s", KNOT_CODE[j]);
    printf("  out (D112)\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned out = 0, q;

        printf("  %-6s", KNOT_CODE[k]);
        for (j = 0; j < N_KNOTS; j++) {
            int hit = 0;

            for (q = 0; q < spectra[k].n; q++)
                if (spectra[k].c[q].nm.source == j)
                    hit = 1;
            printf("%5s", hit ? "x" : ".");
            out += (unsigned)hit;
        }
        printf("  %3u (%2u)\n", out, D112_OUT_DEGREE[k]);
    }
    printf("\n  named beyond the 12 sources (class [alternatives]):\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q, any = 0;

        for (q = 0; q < spectra[k].n; q++) {
            const Class *cl = &spectra[k].c[q];

            if (cl->nm.status != ST_TABLE || cl->nm.source >= 0)
                continue;
            if (!any)
                printf("    %-5s", KNOT_CODE[k]);
            printf(" %s[%u]", cl->nm.name, cl->count);
            any = 1;
        }
        if (any)
            printf("\n");
    }
    for (s = 0; s < 6; s++) {
        status_classes[s] = 0;
        status_alts[s]    = 0;
    }
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q;

        for (q = 0; q < spectra[k].n; q++) {
            status_classes[spectra[k].c[q].nm.status]++;
            status_alts[spectra[k].c[q].nm.status] += spectra[k].c[q].count;
        }
    }
    printf("\n  all spectra by status (classes / alternatives):\n");
    for (s = 0; s < 6; s++)
        printf("    %-10s %4u / %u\n", STATUS_NAME[s], status_classes[s],
            status_alts[s]);
    {
        unsigned named = 0, inconsistent = 0, small_unmatched = 0;

        printf("\n  distinct prime table knots reached (first candidate, "
            "up to mirror):\n");
        for (k = 0; k < N_KNOTS; k++) {
            char     seen[MAX_CLASSES][16];
            unsigned n_seen = 0, q, u;

            for (q = 0; q < spectra[k].n; q++) {
                const Class *cl = &spectra[k].c[q];
                int          dup = 0;

                if (cl->nm.status != ST_TABLE || cl->nm.first[0] == '\0')
                    continue;
                for (u = 0; u < n_seen; u++)
                    if (strcmp(seen[u], cl->nm.first) == 0)
                        dup = 1;
                if (!dup)
                    strcpy(seen[n_seen++], cl->nm.first);
            }
            printf("    %-5s %u\n", KNOT_CODE[k], n_seen);
        }
        /* the reduced diagram is a diagram of the knot, so its crossing
         * count is at least the crossing number of the named candidate
         * (span(V) <= c(K) holds by construction here: the Jones IS the
         * table knot's) */
        for (k = 0; k < N_KNOTS; k++) {
            unsigned q;

            for (q = 0; q < spectra[k].n; q++) {
                const Class *cl = &spectra[k].c[q];
                unsigned     m;
                int          fits = 0;

                if (cl->nm.status == ST_UNMATCHED && cl->reduced <= TABLE_MAX)
                    small_unmatched++;
                if (cl->nm.status != ST_TABLE)
                    continue;
                named++;
                for (m = 0; m < cl->nm.candidates && m < MAX_CAND; m++)
                    if (!cl->nm.cand_prime[m]
                        || cl->nm.cand_c[m] <= cl->reduced)
                        fits = 1;
                if (!fits) {
                    inconsistent++;
                    printf("    INCONSISTENT: %s in %s (reduced %u)\n",
                        cl->nm.name, KNOT_CODE[k], cl->reduced);
                }
            }
        }
        check("every table-named prime class: some candidate's crossing "
            "number <= the reduced crossing count", named > 0
            && inconsistent == 0);
        check("every unmatched class has a reduced diagram above 10 "
            "crossings (primes and two-knot sums up to 10 crossings are "
            "searched; sums of three or more are not)", small_unmatched
            == 0);
    }
    printf("\n  not named by the table (crossing number in [span(Jones), "
        "reduced]):\n");
    for (k = 0; k < N_KNOTS; k++) {
        unsigned q;

        for (q = 0; q < spectra[k].n; q++) {
            const Class *cl = &spectra[k].c[q];

            if (cl->nm.status != ST_BEYOND && cl->nm.status != ST_UNMATCHED)
                continue;
            printf("    %-5s %-9s [%4u] det %-4lu c in [%u, ", KNOT_CODE[k],
                cl->nm.name, cl->count, cl->det, cl->nm.span);
            if (cl->reduced == NO_DIAGRAM) printf("?]  ");
            else printf("%u]  ", cl->reduced);
            print_chorda(cl->alexander);
            printf("\n");
        }
    }
}

/* ================================================================
 * Part D: stability across equally honest polygons, named
 * ================================================================ */

static void
stability (
    int      k,
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    long     n = raw_n[k], shift;
    int      rev, best = (int)laqueus_numerus(chosen[k]), variants = 0;
    Spectrum uni;              /* count = number of variants */
    unsigned q, uni_overflow = 0, bad_total = 0;
    char     msg[160];

    spectrum_clear(&uni);
    printf("\n  %s: every fewest-vertex variant (%d vertices)\n", KNOT_CODE[k],
        best);
    for (rev = 0; rev < 2; rev++) {
        for (shift = 0; shift < n; shift++) {
            Laqueus       v;
            Spectrum      sp;
            unsigned long count, c;
            unsigned      pair_bad, filled;

            if (!variant(raw_pts[k], n, shift, rev, keep, &v)
                || (int)laqueus_numerus(v) != best)
                continue;
            variants++;
            spectrum_clear(&sp);
            count = classify_all(v, keep, scratch, &pair_bad, &filled);
            bad_total += pair_bad;
            for (c = 0; c < count; c++)
                if (alt_exact[c].simple)
                    spectrum_add(&sp, alt_exact[c], work, keep);
            printf("    %s%-3ld %3u classes:", rev ? "r" : "+", shift, sp.n);
            for (q = 0; q < sp.n; q++) {
                unsigned u;
                int      seen = 0;

                printf(" %s[%u]", sp.c[q].nm.name, sp.c[q].count);
                for (u = 0; u < uni.n; u++) {
                    if (chorda_aequalis(uni.c[u].key, sp.c[q].key)) {
                        uni.c[u].count++;
                        seen = 1;
                    }
                }
                if (!seen) {
                    if (uni.n < MAX_CLASSES) {
                        uni.c[uni.n]       = sp.c[q];
                        uni.c[uni.n].count = 1;
                        uni.n++;
                    } else {
                        uni_overflow++;
                    }
                }
            }
            printf("\n");
        }
    }
    printf("    union over %d variants (class [variants containing it]):",
        variants);
    for (q = 0; q < uni.n; q++)
        printf(" %s[%u]", uni.c[q].nm.name, uni.c[q].count);
    printf("\n    core (in every variant):");
    for (q = 0; q < uni.n; q++)
        if (uni.c[q].count == (unsigned)variants)
            printf(" %s", uni.c[q].nm.name);
    printf("\n");
    sprintf(msg, "%s: stability covers the %d fewest-vertex variants; union "
        "complete; mirror theorem holds in every variant", KNOT_CODE[k],
        fewest[k]);
    check(msg, variants == fewest[k] && uni_overflow == 0 && bad_total == 0);
}

static void
part_d (
    Piscina *keep,
    Piscina *scratch,
    Piscina *work)
{
    printf("\n=== Part D: stability - all fewest-vertex variants, named "
        "===\n");
    stability(6, keep, scratch, work);
    stability(1, keep, scratch, work);
}

/* ================================================================
 * main
 * ================================================================ */

int
main (void)
{
    Piscina *keep    = piscina_generare_dynamicum("d116_keep", 1 << 24);
    Piscina *scratch = piscina_generare_dynamicum("d116_scratch", 1 << 22);
    Piscina *work    = piscina_generare_dynamicum("d116_work", 1 << 20);

    printf("KNOTAPEL DEMO 116: Named Spectra\n");
    printf("================================\n");
    if (keep == NULL || scratch == NULL || work == NULL) {
        printf("pool allocation failed\n");
        return 1;
    }
    table = tabula_nodorum_aperire(keep);
    check("knot table opened (250 knots, KnotInfo 2026.10.5)",
        table != NULL && tabula_nodorum_numerus() == 250);
    printf("  table source: %s\n", TABULA_NODORUM_FONS);
    part_a(keep, scratch);
    part_b(keep, scratch, work);
    part_c();
    part_d(keep, scratch, work);
    check("every reflection through the base plane was defined",
        reflexio_failures == 0);
    printf("\n================================\n");
    printf("Results: %d pass, %d fail\n", n_pass, n_fail);
    piscina_destruere(work);
    piscina_destruere(scratch);
    piscina_destruere(keep);
    return n_fail > 0 ? 1 : 0;
}
