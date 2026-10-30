/* demo-snapshot.c - GENERATUM (knotapel/archive.sh) - DO NOT EDIT
 *
 * knotapel/demo_134_umbra_width/main.c frozen with its house-library closure as ONE file:
 * headers in dependency order, library sources with file-local
 * names renamed per file (#define/#undef), main.c last; '#line'
 * names each original file. Compile and run:
 *
 *   clang -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g demo-snapshot.c -o demo-snapshot
 *
 * Commit (library closure clean): 4ef086b491a2b17721ff8c9d79ad8f4c9df76b44
 * Regenerate: ./knotapel/archive.sh knotapel/demo_134_umbra_width/main.c
 * Verified: live build and snapshot gave byte-identical output.
 * Sources (git blob hashes):
 *   7db315706b013efbb850928c78e1c13b0fb72362  include/chorda.h
 *   6f9b7a043cebe2912c612139453b66fcb04770cb  include/chorda_aedificator.h
 *   cced5a8d05c6d921d1b0b06fdbac2598a3b2ad69  include/dependentia.h
 *   1f20f802edaf4064a438ea7ea80b2344bdf66a5c  include/filum.h
 *   53645f652dd16a7a8c8ad79df9e2e70289ee5e52  include/fractio.h
 *   f45b10ad9c303c02d43655b950600fcdab8997bb  include/latina.h
 *   e4017d31e77c476672cf353410ad9d8588ecac12  include/lectiones.h
 *   bfde2073ff18d6f5be1c3844556cc82d39ee0937  include/magnus.h
 *   cd2db07dbd9f7bb6f71ffaa27b03f7cdb8ea65ee  include/piscina.h
 *   a34b9f2536efa81a09cea36f9a9acba28fb4b2fc  include/postulata_posix.h
 *   b4c8c649c03e84eca53408282c39d5b0e24c6016  lib/chorda.c
 *   ee055a36d7e4726ad5b3731e2ca64e62e93d084c  lib/chorda_aedificator.c
 *   5c5eea4e8cb0ded214e85f92e74751560e8a63e8  lib/dependentia.c
 *   2c08b7cfb149d46f7863094276bd43b3ae62d3a0  lib/filum.c
 *   61ec3b3106345cce9ea64477aafe7d1072030be7  lib/fractio.c
 *   23f1b879af7d8137ea10af80220da1c08677db44  lib/lectiones.c
 *   fd94a8d26a4bd4b340875ac0a1551bc3f3c55e8e  lib/magnus.c
 *   c6ab1e19274a3b36ff5cfdde651e45d079905b56  lib/piscina.c
 *   afeba5fd547d6539704f535201c05dc818289842  knotapel/demo_134_umbra_width/main.c (uncommitted, embedded verbatim)
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
#line 1 "include/dependentia.h"
/* dependentia.h - Arbores dependentiae: cruces, proiectivitas, lacunae,
 * nidificatio, nucleus planaris
 *
 * Arbor dependentiae sententiae: verba positionibus 1..n, caput
 * cuiusque verbi aliud verbum aut radix artificialis (positio 0).
 * Arcus (h, d) intervallum inter h et d tegit; arcus REALES sunt quorum
 * caput verbum est, arcus RADICIS (0, r) ad verbum radicis ducit. Nomen
 * ex grammatica Modistarum (Thomas Erfordiensis, Grammatica
 * speculativa): constructio omnis ex DEPENDENTE et TERMINANTE.
 *
 * Definitiones (Kuhlmann et Nivre, "Mildly Non-Projective Dependency
 * Structures", COLING/ACL 2006, pp. 507-514; numeri definitionum
 * eorum):
 *   CRUX: arcus {a, b} et {c, e} se secant sicut a < c < b < e
 *         (termini stricte alternantes); arcus verbum commune habentes
 *         numquam se secant.
 *   PLANARIS (Def. 4): nulli duo arcus REALES se secant (arcus radicis
 *         NON additur).
 *   PROIECTIVA (Def. 3): proventus (yield, verba a verbo dominata,
 *         ipso incluso) cuiusque verbi intervallum est. Aequivalenter:
 *         nulla crux arcu radicis addito. Duae viae in functionibus
 *         diversis computantur (dependentia_cruces,
 *         dependentia_mensurare), ut probatio eas comparet.
 *   LACUNA (Def. 6): par verborum vicinorum in proventu ordinato
 *         quorum positiones plus quam I differunt; gradus lacunarum
 *         verbi = numerus earum, arboris = maximum (Def. 7).
 *   BENE NIDIFICATA (Def. 8): nulli duo subarbores disiunctae (neutra
 *         radix alteram dominatur) alternant (l1 < l2 < r1 < r2, l1, r1
 *         in una, l2, r2 in altera).
 *   GRADUS ARCUS (Def. 9): numerus componentium nexarum subgraphi ab
 *         intervallo arcus inducti (termini inclusi) quarum radix a
 *         capite arcus non dominatur; arboris = maximum. In arboribus
 *         lectio cum terminis et lectio interior idem dant (termini
 *         componentes solum per verba a capite dominata iungunt) -
 *         non electio, sed una mensura.
 *   ARCUS NON PROIECTIVUS: arcus (h, d) cuius intervallum verbum tenet
 *         ab h non dominatum (notio usitata, e.g. Nivre et Nilsson
 *         2005); aequivalenter gradus arcus > 0. Arbor proiectiva sine
 *         arcu non proiectivo. Arcus SECTUS (non planaris) alia res: in
 *         pari secantium saepe unus solus non proiectivus est.
 *   NUCLEUS PLANARIS: plurimi arcus reales quorum nulli duo se secant
 *         (programma dynamicum intervallorum, exactum).
 *
 * Lex nulla exacta: ordine verborum fortuito (uniformi) duo arcus sine
 * verbo communi se secant probabilitate EXACTE 1/3 (ordo relativus
 * quattuor terminorum uniformis in XXIV, figurae tres - nidus, latus,
 * crux - in VIII quaeque); ergo E[cruces] = paria_disiuncta / 3.
 *
 * Arbor non copiatur; functiones mensurae arborem SANAM postulant
 * (dependentia_validare) et FALSUM / -1 reddunt si non est. Memoria
 * temporaria in piscina vocantis (O(n) aut O(n^2) octeti; vocans
 * piscinam per sententiam reficere potest).
 */

#ifndef DEPENDENTIA_H
#define DEPENDENTIA_H




nomen structura {
                  i32  numerus;   /* verba 1..numerus */
    constans      i32* capita;    /* capita[k - 1] = caput verbi k; 0 =
                                   * radix artificialis */
} DependentiaArbor;

nomen enumeratio {
    DEPENDENTIA_SANA = ZEPHYRUM,
    DEPENDENTIA_VACUA,            /* numerus 0 */
    DEPENDENTIA_CAPUT_EXTRA,      /* caput extra 0..numerus, aut sui */
    DEPENDENTIA_RADIX_NULLA,      /* nullum verbum caput 0 habet */
    DEPENDENTIA_RADICES_PLURES,   /* plura verba caput 0 habent */
    DEPENDENTIA_CYCLUS            /* via capitum radicem non attingit */
} DependentiaStatus;

/* Arborem iudicare (ordine: vacua, caput extra, radices, cyclus). */
DependentiaStatus
dependentia_validare (
    constans DependentiaArbor* arbor);

nomen enumeratio {
    DEPENDENTIA_OMISSA_FACTA = ZEPHYRUM,
    DEPENDENTIA_OMISSA_NON_SANA,     /* arbor data non sana (aut NIHIL) */
    DEPENDENTIA_OMISSA_RADIX,        /* radix omitteretur: arbor non
                                       * maneret */
    DEPENDENTIA_OMISSA_MEMORIA
} DependentiaOmissa;

/* Arbor sine verbis notatis (omittere[k - 1] VERUM): reliqua ordine
 * renumerantur; filii verbi omissi capiti eius proximo non omisso
 * adhaerent (*contractiones = verba quorum caput sic mutatum est).
 * Status causam nominat (radix omissa a memoria distinguitur); capita
 * exitus in piscina, exitus tantum si FACTA. */
DependentiaOmissa
dependentia_omittere (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                 constans b32* omittere,
             DependentiaArbor* exitus,
                          i32* contractiones);

nomen structura {
    s64 cruces;            /* paria arcuum realium se secantium */
    s64 cruces_radicis;    /* arcus reales arcum radicis secantes */
    i32 arcus_secti;       /* arcus reales in cruce aliqua (cum realibus) */
    s64 paria_disiuncta;   /* paria arcuum realium sine verbo communi:
                            * ordine fortuito E[cruces] = paria / 3 */
    b32 planaris;          /* cruces == 0 (Def. 4) */
    b32 proiectiva;        /* et cruces_radicis == 0 (Def. 3 per cruces) */
} DependentiaCruces;

/* Cruces numerare. FALSUM si arbor non sana aut memoria. */
b32
dependentia_cruces (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
            DependentiaCruces* exitus);

nomen structura {
    i32 gradus_lacunarum;   /* Def. 7 */
    i32 gradus_arcuum;      /* Def. 9 */
    b32 bene_nidificata;    /* Def. 8 */
    b32 proiectiva;         /* proventus omnes intervalla (Def. 3) */
} DependentiaMensura;

/* Lacunas, gradum arcuum, nidificationem metiri; lacunae[k - 1] =
 * lacunae proventus verbi k (lacunae NIHIL = non redduntur). FALSUM si
 * arbor non sana aut memoria. */
b32
dependentia_mensurare (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          i32* lacunae,
           DependentiaMensura* exitus);

nomen structura {
    s32 cruces;            /* arcus reales quos hic arcus secat */
    b32 secat_radicem;     /* arcum radicis (0, r) secat */
    b32 non_proiectivus;   /* verbum intra intervallum a capite non
                            * dominatum (== gradus > 0) */
    i32 gradus;            /* gradus arcus (Def. 9) */
} DependentiaArcus;

/* Mensurae per arcum: arcus[k - 1] = arcus in verbum k (vocans n
 * elementa praebet); verbum radicis (arcus realis nullus): omnia 0.
 * Summa crucium = II * cruces arboris; arcus cum cruce = arcus_secti;
 * maximum graduum = gradus_arcuum. FALSUM si arbor non sana aut
 * memoria. */
b32
dependentia_arcus (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
             DependentiaArcus* arcus);

/* Nucleus planaris: plurimi arcus reales sine cruce inter se. -1 si
 * arbor non sana, aut memoria, aut numerus > 65535 (tabula n^2 et
 * indices i32 eam non ferunt). */
s32
dependentia_nucleus_planaris (
                      Piscina* piscina,
    constans DependentiaArbor* arbor);

/* Graphum crucium (nodi = arcus reales, margines = cruces):
 * componens[k - 1] = index componentis arcus in verbum k (0, 1, ...
 * ordine primi verbi), -1 pro verbo radicis (arcus realis nullus);
 * singuli inclusi. Redde numerum componentium; -1 si arbor non sana
 * aut memoria. */
s32
dependentia_componentes (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          s32* componens);

#endif /* DEPENDENTIA_H */
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
#line 1 "include/filum.h"
/* filum.h - fila legere et scribere (file I/O) */
#ifndef FILUM_H
#define FILUM_H






/* ==================================================
 * Lectio Simplex
 * ================================================== */

/* Legere filum totum in chordam (ex piscina allocatum)
 * Reddit chordam cum mensura=0 si defectu */
chorda
filum_legere_totum (
    constans character* via,
               Piscina* piscina);

/* Verificare si filum existit */
b32
filum_existit (
    constans character* via);

/* Cape mensuram fili in bytes (reddit 0 si defectu) */
memoriae_index
filum_mensura (
    constans character* via);


/* ==================================================
 * Lectio per Octetos (binaria, sequens)
 * ================================================== */

nomen structura FilumOctetorumLector FilumOctetorumLector;

/* Aperire filum pro lectione octetorum binaria, ab initio, sequens:
 * corpora magna per partes (fontes hospitii) - numquam totum in
 * memoria. Lectio notatur ut ceterae (lectiones). NIHIL si non
 * aperitur. */
FilumOctetorumLector*
filum_octeti_aperire (
    constans character* via,
               Piscina* piscina);

/* Partem legere in capsa (ad n octetos):
 * >0 octeti lecti, 0 finis fili, -1 error */
s32
filum_octeti_legere (
    FilumOctetorumLector* lector,
                      i8* capsa,
                     i32  n);

/* Claudere (tutum iterum; NIHIL tolerat) */
vacuum
filum_octeti_claudere (
    FilumOctetorumLector* lector);


/* ==================================================
 * Lectio per Lineas
 * ================================================== */

nomen structura FilumLector FilumLector;

/* Aperire filum pro lectione per lineas
 * Reddit NIHIL si defectu */
FilumLector*
filum_lector_aperire (
    constans character* via,
               Piscina* piscina);

/* Legere lineam proximam (sine charactere lineae novae)
 * Reddit VERUM si linea lecta, FALSUM ad finem vel errore
 * Linea allocata ex piscina */
b32
filum_lector_lineam_proximam (
         FilumLector* lector,
              chorda* linea_out);

/* Cape numerum versus currentem (pro errore reportando) */
i32
filum_lector_numerus_versus (
    FilumLector* lector);

/* Verificare si ad finem fili */
b32
filum_lector_finis (
    FilumLector* lector);

/* Claudere lectorem et liberare res
 * Nota: chordae ex piscina manent validae */
vacuum
filum_lector_claudere (
    FilumLector* lector);


/* ==================================================
 * Scriptio
 * ================================================== */

/* Modus aperire pro FilumScriptor */
nomen enumeratio {
    FILUM_MODUS_CREARE,   /* Creare/superscribere filum */
    FILUM_MODUS_APPENDERE /* Appendere ad filum existens */
} FilumModus;

nomen structura FilumScriptor FilumScriptor;

/* Aperire filum pro scriptione continua
 * Reddit NIHIL si defectu */
FilumScriptor*
filum_scriptor_aperire (
     constans character* via,
             FilumModus  modus,
                Piscina* piscina);

/* Scribere chordam ad filum apertum */
b32
filum_scriptor_scribere (
     FilumScriptor* scriptor,
            chorda  contentum);

/* Scribere octetos (binarios, constans - e.g. capsa parseris aliena)
 * ad filum apertum; n 0 = nihil, VERUM */
b32
filum_scriptor_octetos_scribere (
     FilumScriptor* scriptor,
       constans i8* octeti,
               i32  n);

/* Scribere literas C ad filum apertum */
b32
filum_scriptor_scribere_literis (
         FilumScriptor* scriptor,
    constans character* contentum);

/* Scribere lineam cum newline ad finem */
b32
filum_scriptor_lineam_scribere (
         FilumScriptor* scriptor,
    constans character* linea);

/* Sync ad discum (fflush) */
b32
filum_scriptor_sync (
    FilumScriptor* scriptor);

/* Claudere scriptorem et liberare res */
vacuum
filum_scriptor_claudere (
    FilumScriptor* scriptor);


/* Scribere chordam ad filum (creare/superscribere)
 * Reddit VERUM in successu */
b32
filum_scribere (
    constans character* via,
                chorda  contentum);

/* Scribere literas C ad filum (creare/superscribere) */
b32
filum_scribere_literis (
    constans character* via,
    constans character* contentum);

/* Appendere chordam ad filum existens */
b32
filum_appendere (
    constans character* via,
                chorda  contentum);

/* Appendere contentum ad filum (creatur si abest, modus 0644) per
 * write() UNUM deinde fsync() - in disco ante reditum, aut FALSUM.
 * Pro annalibus 'append-only' (rationarium vatis, index herbarii,
 * acta optchat). O_APPEND: lineae < PIPE_BUF (4096) a processibus
 * pluribus scriptae non intermiscentur. Contentum vacuum: filum
 * creatur, nihil scribitur, fsync tamen. filum_scriptor_sync
 * fflush SOLUM est - hoc discum attingit. _WIN32: nondum (FALSUM
 * cum errore nominato). */
b32
filum_appendere_firmiter (
    constans character* via,
                chorda  contentum);

/* Appendere literas C ad filum existens */
b32
filum_appendere_literis (
    constans character* via,
    constans character* contentum);


/* ==================================================
 * Manipulatio Filorum
 * ================================================== */

/* Delere filum
 * Reddit VERUM in successu */
b32
filum_delere (
    constans character* via);

/* Delere ARBOREM directorii (rm -r) SINE nexibus symbolicis sequendis.
 *
 * CUR NON directorium_ambulare: id stat() vocat, ergo nexum ad
 * directorium ut directorium tractat et IN EUM descendit - arbor aliena
 * extra viam datam deleretur. Hic lstat(): nexus UT NEXUS removetur,
 * destinatio eius intacta manet.
 *
 * Recusat NIHIL, viam vacuam et RADICEM systematis (per identitatem,
 * non per literas: "//" et "/tmp/.." quoque). Via non exsistens =
 * VERUM (nihil delendum). Filum aut nexus = ipse removetur. Directorium
 * relegitur donec vacuum sit (readdir post unlink introitus omittere
 * potest). FALSUM ad primum defectum; quod iam deletum est manet
 * deletum. POSIX solum (sub _WIN32 recusat). */
b32
filum_arborem_delere (
    constans character* via);

/* Renominare/movere filum */
b32
filum_movere (
    constans character* via_vetus,
    constans character* via_nova);

/* Copiare filum */
b32
filum_copiare (
    constans character* via_fons,
    constans character* via_destinatio);

/* Creare directorium
 * Reddit VERUM in successu, FALSUM si iam existit vel error */
b32
filum_directorium_creare (
    constans character* via);

/* Creare directorium si non existit
 * Reddit VERUM in successu (vel iam existit) */
b32
filum_directorium_creare_si_necesse (
    constans character* via);

/* Creare directorium CUM MODO dato si non existit; si iam existit,
 * modum COERCET (chmod).
 *
 * CUR SEPARATUM a creare_si_necesse: illud 0755 figit, quod pro
 * datis publicis rectum est sed pro secretis falsum. Directorium
 * socketorum moderantium ssh 0700 postulat - socketus enim sessioni
 * plenae aditum praebet, ergo nomen eius aliis usoribus ne
 * appareat quidem. Arca (thesaurus secretorum, futurum nominatum)
 * idem postulabit.
 *
 * Modum COERCET etiam in directorio praeexsistenti: directorium
 * quod alia causa 0755 creavit aliter tacite laxum maneret. */
b32
filum_directorium_creare_cum_modo (
    constans character* via,
               integer  modus);

/* Creare directorium ET OMNES PARENTES desideratos (mkdir -p)
 *
 * CUR EXSISTIT: tres creatores supra UNUM mkdir faciunt, ergo semita
 * nidificata cuius parentes absunt cum ENOENT DEFICIT - et quisquis
 * semitam struit id tacite patitur (concha, mensa, villa_agens
 * creatores simplices vocant hodie). fasciculum_scribere
 * Contents/MacOS/ creare debet antequam exsecutabile eo copiet
 * (fasciculum-spec par. V).
 *
 * Modus 0755 ut in creare_si_necesse. SINE REVERSIONE: si segmentum
 * medium deficit, quae iam creata sunt MANENT (mos ipse ipsius
 * mkdir -p).
 *
 * Reddit VERUM si semita tota post vocationem exsistit (iam
 * exsistens VERUM est, ergo idempotens); FALSUM si via NIHIL aut
 * vacua aut nimis longa, aut si segmentum creari non potest - causa
 * specifica vocationis interioris SERVATUR (filum_error_recens).
 */
b32
filum_directorium_creare_cum_parentibus (
    constans character* via);

/* Verificare si directorium existit */
b32
filum_directorium_existit (
    constans character* via);

/* Ponere modum fili (chmod)
 *
 * CUR EXSISTIT: filum_copiare ansa octetorum nuda est, ergo modum NON
 * servat - binarium in fasciculum .app copiatum non exsecutabile
 * caderet, et fasciculus sine bite +x duplici ictu sine ulla diagnosi
 * deficit (fasciculum-spec par. VII.1).
 *
 * Reddit VERUM in successu; FALSUM si via NIHIL aut chmod deficit.
 */
b32
filum_modum_ponere (
    constans character* via,
               integer  modus);


/* ==================================================
 * Interrogatio Status
 * ================================================== */

nomen structura {
    memoriae_index mensura;         /* Mensura in bytes */
               b32 est_directorium; /* Est directorium? */
               b32 est_filum;       /* Est filum regulare? */
               b32 potest_legere;   /* Legibile? */
               b32 potest_scribere; /* Scribabile? */
               b32 potest_exsequi;  /* Exsequibile? (bit +x usoris) */
} FilumStatus;

/* Cape statum/informationem fili
 * Reddit VERUM si filum existit et status captus */
b32
filum_status (
    constans character* via,
           FilumStatus* status_out);


/* ==================================================
 * Errores
 * ================================================== */

/* Cape nuntium erroris recens (literas C, non possessus a vocante)
 * Reddit errorem descriptivum vel NIHIL si nullus error */
constans character*
filum_error_recens (
    vacuum);


/* ==================================================
 * Sera (lapide feature-requests/020)
 * ================================================== */

/* SERA EXCLUSIVA super filum 'via' per flock(2): filum creatur si abest
 * et NUMQUAM deletur (existentia fili seram NON significat - sera
 * nucleo tenetur). Sera processui pertinet et a nucleo LIBERATUR cum
 * processus moritur: nulla sera stala, nulla regula aetatis. Descriptor
 * FD_CLOEXEC fert, ergo processus filii (processus_exsequi) seram non
 * hereditant nec post mortem patris tenent.
 *
 * TENE per totam seriem LEGERE -> PROBARE -> APPENDERE, non circa
 * appendere solum: status ante seram lectus iam falsus esse potest
 * (seq duplicatum). Ideo nulla 'filum_appendere_sub_sera' hic.
 *
 *   FilumSera* s = filum_seram_capere("eventa.jsonl.sera", MM, p);
 *   si (s == NIHIL) { ... occupata post II s ... }
 *   ... legere, probare, filum_appendere ...
 *   filum_seram_liberare(s);
 *
 * mora_ms: quamdiu exspectare (ZEPHYRUM = semel conari). NIHIL si
 * tempus exhaustum aut filum aperiri nequit. Duae capturae in EODEM
 * processu quoque se excludunt (flock per aperturam fili). POSIX
 * solum (sub _WIN32 NIHIL). */
nomen structura FilumSera FilumSera;

FilumSera*
filum_seram_capere (
    constans character* via,
                   i32  mora_ms,
               Piscina* piscina);

/* Seram liberare et descriptorem claudere; NIHIL tolerat; iterata
 * nihil agit. */
vacuum
filum_seram_liberare (
    FilumSera* sera);

#endif /* FILUM_H */
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
#line 1 "include/lectiones.h"
/* lectiones.h - liber lectionum: quod processus legit, quaesivit,
 * exstare confirmavit, enumeravit, scripsit (fabrica slice 2).
 *
 * Variabilis ambitus FABRICA_LECTIONES viam plagulae nominat; absens =
 * nihil fit (nulla allocatio, nullus mos mutatus). Linea una per
 * eventum, '<littera>\t<via>\n', una vocatione write() cum O_APPEND -
 * filii paralleli lineas integras servant. Via ut data scribitur
 * (relativa manet relativa); fabrica postea sigillat.
 *
 * Hoc modulo SOLO linea libri scribitur: filum, via_existit,
 * iter_directoria et ansae directoriorum aedilis eum vocant. Ipse
 * open/write crudos adhibet - canalis est, non cliens canalis.
 */
#ifndef LECTIONES_H
#define LECTIONES_H


#include <stdio.h>

nomen enumeratio {
    LECTIO_LEGIT = 0,     /* 'L' plagula lecta */
    LECTIO_ABSENS,        /* 'A' quaesita, non inventa */
    LECTIO_EXSTAT,        /* 'X' exstare confirmata, non lecta */
    LECTIO_ENUMERAVIT,    /* 'D' directorium enumeratum */
    LECTIO_SCRIPSIT,      /* 'S' plagula scripta, deleta, mota */
    LECTIO_AMBITUS        /* 'E' variabilis ambitus lecta */
} LectioGenus;

/* lineam unam appendere; nihil si variabilis absens aut via nimis
 * longa (linea numquam scinditur) */
vacuum
lectiones_notare (
                 LectioGenus  genus,
          constans character* via);

/* getenv per librum: valorem reddit (aut NIHIL) et 'E' notat -
 * linea 'E\t<titulus>\t<valor>' (absens: 'E\t<titulus>' sine
 * tabulatore secundo) */
constans character*
lectiones_ambitus (
         constans character* titulus);

/* FOPEN PER LIBRUM (fabrica plan 5 T5a) - fulcrum TRANSITORIUM pro
 * vocationibus stdio crudis quae manent (codex novus filum adhibet):
 * modus 'r'/'rb' (sine '+'): L si apertum, A si non; ceteri (w, a,
 * r+): S. Viae sub /dev/ (machinae: entropia, terminalia) NON
 * notantur - ingressus non sunt, et fabrica machinam sigillare nequit
 * (IGNOTUM: verdictum numquam reutile). */
FILE*
lectiones_fopen (
    constans character* via,
    constans character* modus);

/* STAT PER LIBRUM: stat(2) idem; X si exstat, A si non (sub /dev/
 * nihil). Status per 'structura stat' (sys/stat.h vocantis). */
structura stat;
integer
lectiones_stat (
    constans character* via,
       structura stat* status);

#endif /* LECTIONES_H */
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
/* lib/dependentia.c: statica per plagulam renominata */
#define Structura Structura_dependentia
#define _allocare _allocare_dependentia
#define _alternant _alternant_dependentia
#define _dominat _dominat_dependentia
#define _gradus_arcus _gradus_arcus_dependentia
#define _radix _radix_dependentia
#define _structura _structura_dependentia
#line 1 "lib/dependentia.c"
/* dependentia.c - Vide dependentia.h. */


#include <string.h>

/* Structura arboris ad mensuras: filii ordine positionis, tempora
 * ingressus et egressus percursus (dominatio in O(1)), fines proventus.
 * Indices 1..n (elementum 0 non adhibetur). */
nomen structura {
               i32  numerus;
    constans   i32* capita;
               i32  radix;
               i32* filius_primus;   /* 0 = nullus */
               i32* frater;          /* proximus ordine positionis, 0 = nullus */
               i32* ingressus;
               i32* egressus;
} Structura;

interior vacuum*
_allocare (
    Piscina* piscina,
        i32  numerus,
        i32  magnitudo_elementi)
{
    memoriae_index mensura = (memoriae_index)numerus
        * (memoriae_index)magnitudo_elementi;
           vacuum* p;

    si (mensura == ZEPHYRUM)
    {
        mensura = I;
    }
    p = piscina_conari_allocare(piscina, mensura);
    si (p != NIHIL)
    {
        memset(p, ZEPHYRUM, (size_t)mensura);
    }
    redde p;
}

DependentiaStatus
dependentia_validare (
    constans DependentiaArbor* arbor)
{
    i32 n;
    i32 k;
    i32 radices = ZEPHYRUM;

    si (   arbor         == NIHIL || arbor->numerus == ZEPHYRUM
        || arbor->capita == NIHIL)
    {
        redde DEPENDENTIA_VACUA;
    }
    n = arbor->numerus;
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h > n || h == k)
        {
            redde DEPENDENTIA_CAPUT_EXTRA;
        }
        si (h == ZEPHYRUM)
        {
            radices = radices + I;
        }
    }
    si (radices == ZEPHYRUM)
    {
        redde DEPENDENTIA_RADIX_NULLA;
    }
    si (radices > I)
    {
        redde DEPENDENTIA_RADICES_PLURES;
    }
    /* radix una: cyclus si via capitum post n passus radicem non
     * attingit */
    per (k = I; k <= n; k++)
    {
        i32 v       = k;
        i32 passus  = ZEPHYRUM;

        dum (v != ZEPHYRUM && passus <= n)
        {
            v       = arbor->capita[v - I];
            passus  = passus + I;
        }
        si (v != ZEPHYRUM)
        {
            redde DEPENDENTIA_CYCLUS;
        }
    }
    redde DEPENDENTIA_SANA;
}

/* Structuram parare (arbor sana requiritur). */
interior b32
_structura (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                    Structura* s)
{
    i32  n = arbor->numerus;
    i32  k;
    i32  tempus = ZEPHYRUM;
    i32  culmen = ZEPHYRUM;
    i32* acervus;
    i32* cursor;

    s->numerus  = n;
    s->capita   = arbor->capita;
    s->radix    = ZEPHYRUM;
    s->filius_primus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->frater = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->ingressus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    s->egressus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    acervus = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    cursor = (i32*)_allocare(piscina, n + I,
        (i32)magnitudo(i32));
    si (   s->filius_primus == NIHIL || s->frater == NIHIL
        || s->ingressus     == NIHIL || s->egressus == NIHIL
        || acervus          == NIHIL || cursor == NIHIL)
    {
        redde FALSUM;
    }
    /* a dextra ad sinistram in caput indicis: filii ordine positionis */
    per (k = n; k >= I; k--)
    {
        i32 h = arbor->capita[k - I];

        si (h == ZEPHYRUM)
        {
            s->radix = k;
        }
        alioquin
        {
            s->frater[k]         = s->filius_primus[h];
            s->filius_primus[h]  = k;
        }
    }
    /* percursus sine recursione */
    acervus[culmen]         = s->radix;
    culmen                  = culmen + I;
    s->ingressus[s->radix]  = tempus;
    tempus                  = tempus + I;
    cursor[s->radix]        = s->filius_primus[s->radix];
    dum (culmen > ZEPHYRUM)
    {
        i32 u = acervus[culmen - I];
        i32 c = cursor[u];

        si (c != ZEPHYRUM)
        {
            cursor[u]        = s->frater[c];
            s->ingressus[c]  = tempus;
            tempus           = tempus + I;
            cursor[c]        = s->filius_primus[c];
            acervus[culmen]  = c;
            culmen           = culmen + I;
        }
        alioquin
        {
            s->egressus[u]  = tempus;
            tempus          = tempus + I;
            culmen          = culmen - I;
        }
    }
    redde VERUM;
}

/* u dominat v (u == v incluso) */
interior b32
_dominat (
    constans Structura* s,
                   i32  u,
                   i32  v)
{
    redde (b32)(s->ingressus[u] <= s->ingressus[v]
        && s->egressus[v] <= s->egressus[u]);
}

DependentiaOmissa
dependentia_omittere (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                 constans b32* omittere,
             DependentiaArbor* exitus,
                          i32* contractiones)
{
    i32  n;
    i32  k;
    i32  m       = ZEPHYRUM;
    i32  mutata  = ZEPHYRUM;
    i32* novus;
    i32* capita;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || omittere                    == NIHIL || exitus == NIHIL)
    {
        redde DEPENDENTIA_OMISSA_NON_SANA;
    }
    n = arbor->numerus;
    per (k = I; k <= n; k++)
    {
        si (omittere[k - I] && arbor->capita[k - I] == ZEPHYRUM)
        {
            redde DEPENDENTIA_OMISSA_RADIX;
        }
    }
    novus = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    si (novus == NIHIL)
    {
        redde DEPENDENTIA_OMISSA_MEMORIA;
    }
    per (k = I; k <= n; k++)
    {
        si (!omittere[k - I])
        {
            m         = m + I;
            novus[k]  = m;
        }
    }
    capita = (i32*)_allocare(piscina, m, (i32)magnitudo(i32));
    si (capita == NIHIL)
    {
        redde DEPENDENTIA_OMISSA_MEMORIA;
    }
    per (k = I; k <= n; k++)
    {
        i32 h        = arbor->capita[k - I];
        b32 mutatum  = FALSUM;

        si (omittere[k - I])
        {
            perge;
        }
        /* radix non omissa: via capitum omissorum in verbo servato
         * desinit */
        dum (h != ZEPHYRUM && omittere[h - I])
        {
            h        = arbor->capita[h - I];
            mutatum  = VERUM;
        }
        capita[novus[k] - I] = h == ZEPHYRUM ? ZEPHYRUM : novus[h];
        si (mutatum)
        {
            mutata = mutata + I;
        }
    }
    exitus->numerus  = m;
    exitus->capita   = capita;
    si (contractiones != NIHIL)
    {
        *contractiones = mutata;
    }
    redde DEPENDENTIA_OMISSA_FACTA;
}

b32
dependentia_cruces (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
            DependentiaCruces* exitus)
{
    i32  n;
    i32  k;
    i32  i;
    i32  j;
    i32  m      = ZEPHYRUM;
    i32  radix  = ZEPHYRUM;
    i32* sinister;
    i32* dexter;
    b32* sectus;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || exitus                      == NIHIL)
    {
        redde FALSUM;
    }
    n         = arbor->numerus;
    sinister  = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    dexter    = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    sectus    = (b32*)_allocare(piscina, n, (i32)magnitudo(b32));
    si (sinister == NIHIL || dexter == NIHIL || sectus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h == ZEPHYRUM)
        {
            radix = k;
            perge;
        }
        sinister[m]  = h < k ? h : k;
        dexter[m]    = h < k ? k : h;
        m            = m + I;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    per (i = ZEPHYRUM; i < m; i++)
    {
        i32 a = sinister[i];
        i32 b = dexter[i];

        /* arcus radicis (0, radix): 0 < a < radix < b */
        si (a < radix && radix < b)
        {
            exitus->cruces_radicis = exitus->cruces_radicis + I;
        }
        per (j = i + I; j < m; j++)
        {
            i32 c = sinister[j];
            i32 e = dexter[j];

            si (a == c || a == e || b == c || b == e)
            {
                perge;   /* verbum commune: numquam crux */
            }
            exitus->paria_disiuncta = exitus->paria_disiuncta + I;
            si ((a < c && c < b && b < e) || (c < a && a < e && e < b))
            {
                exitus->cruces  = exitus->cruces + I;
                sectus[i]       = VERUM;
                sectus[j]       = VERUM;
            }
        }
    }
    per (i = ZEPHYRUM; i < m; i++)
    {
        si (sectus[i])
        {
            exitus->arcus_secti = exitus->arcus_secti + I;
        }
    }
    exitus->planaris    = (b32)(exitus->cruces == ZEPHYRUM);
    exitus->proiectiva  = (b32)(exitus->planaris
        && exitus->cruces_radicis == ZEPHYRUM);
    redde VERUM;
}

/* proventus duorum fratrum alternant? (positiones a minimo ad maximum
 * percurruntur; series notarum A/B ceteris omissis alternat si mutationes
 * >= III) */
interior b32
_alternant (
    constans Structura* s,
                   i32  a,
                   i32  b,
                   i32  initium,
                   i32  finis)
{
    i32 p;
    i32 ultima      = ZEPHYRUM;   /* 0 nulla, I a, II b */
    i32 mutationes  = ZEPHYRUM;

    per (p = initium; p <= finis; p++)
    {
        i32 nota = ZEPHYRUM;

        si (_dominat(s, a, p))
        {
            nota = I;
        }
        alioquin si (_dominat(s, b, p))
        {
            nota = II;
        }
        si (nota == ZEPHYRUM)
        {
            perge;
        }
        si (ultima != ZEPHYRUM && nota != ultima)
        {
            mutationes = mutationes + I;
            si (mutationes >= III)
            {
                redde VERUM;
            }
        }
        ultima = nota;
    }
    redde FALSUM;
}

/* gradus arcus in verbum u (Def. 9; 0 pro radice): in intervallo
 * arcus radix componentis est verbum cuius caput extra intervallum
 * iacet (subgraphum arboris inductum); numerantur radices a capite
 * arcus non dominatae */
interior i32
_gradus_arcus (
    constans Structura* s,
                   i32  u)
{
    i32 h       = s->capita[u - I];
    i32 initium;
    i32 finis;
    i32 p;
    i32 gradus  = ZEPHYRUM;

    si (h == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    initium  = h < u ? h : u;
    finis    = h < u ? u : h;
    per (p = initium; p <= finis; p++)
    {
        i32 c = s->capita[p - I];

        si (   (c == ZEPHYRUM || c < initium || c > finis)
            && !_dominat(s, h, p))
        {
            gradus = gradus + I;
        }
    }
    redde gradus;
}

b32
dependentia_mensurare (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          i32* lacunae,
           DependentiaMensura* exitus)
{
    Structura  s;
          i32  n;
          i32  u;
          i32  p;
          i32* minimum;
          i32* maximum;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || exitus                      == NIHIL)
    {
        redde FALSUM;
    }
    si (!_structura(piscina, arbor, &s))
    {
        redde FALSUM;
    }
    n        = arbor->numerus;
    minimum  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    maximum  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    si (minimum == NIHIL || maximum == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    exitus->bene_nidificata = VERUM;

    /* lacunae (Def. 6-7): cursus verborum dominatorum - I */
    per (u = I; u <= n; u++)
    {
        i32 cursus  = ZEPHYRUM;
        b32 intus   = FALSUM;

        per (p = I; p <= n; p++)
        {
            b32 d = _dominat(&s, u, p);

            si (d)
            {
                si (minimum[u] == ZEPHYRUM)
                {
                    minimum[u] = p;
                }
                maximum[u] = p;
                si (!intus)
                {
                    cursus = cursus + I;
                }
            }
            intus = d;
        }
        si (lacunae != NIHIL)
        {
            lacunae[u - I] = cursus - I;
        }
        si (cursus - I > exitus->gradus_lacunarum)
        {
            exitus->gradus_lacunarum = cursus - I;
        }
    }
    exitus->proiectiva = (b32)(exitus->gradus_lacunarum == ZEPHYRUM);

    /* nidificatio (Def. 8): subarbores disiunctae alternantes habent
     * antecessores fratres (filios antecessoris communis infimi) qui
     * etiam alternant, ergo paria fratrum sufficiunt */
    per (u = I; u <= n && exitus->bene_nidificata; u++)
    {
        i32 a;

        per (a = s.filius_primus[u]; a != ZEPHYRUM
            && exitus->bene_nidificata; a = s.frater[a])
        {
            i32 b;

            per (b = s.frater[a]; b != ZEPHYRUM; b = s.frater[b])
            {
                i32 initium;
                i32 finis;

                si (maximum[a] < minimum[b] || maximum[b] < minimum[a])
                {
                    perge;
                }
                initium  = minimum[a] < minimum[b] ? minimum[a]
                    : minimum[b];
                finis    = maximum[a] > maximum[b] ? maximum[a]
                    : maximum[b];
                si (_alternant(&s, a, b, initium, finis))
                {
                    exitus->bene_nidificata = FALSUM;
                    frange;
                }
            }
        }
    }

    per (u = I; u <= n; u++)
    {
        i32 gradus = _gradus_arcus(&s, u);

        si (gradus > exitus->gradus_arcuum)
        {
            exitus->gradus_arcuum = gradus;
        }
    }
    redde VERUM;
}

b32
dependentia_arcus (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
             DependentiaArcus* arcus)
{
    Structura s;
          i32 n;
          i32 k;
          i32 q;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || arcus                       == NIHIL)
    {
        redde FALSUM;
    }
    si (!_structura(piscina, arbor, &s))
    {
        redde FALSUM;
    }
    n = arbor->numerus;
    memset(arcus, ZEPHYRUM, (size_t)n * magnitudo(DependentiaArcus));
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];
        i32 a;
        i32 b;
        i32 p;

        si (h == ZEPHYRUM)
        {
            perge;
        }
        a = h < k ? h : k;
        b = h < k ? k : h;
        /* arcus radicis (0, radix): 0 < a < radix < b */
        arcus[k - I].secat_radicem = (b32)(a < s.radix && s.radix < b);
        per (q = k + I; q <= n; q++)
        {
            i32 g = arbor->capita[q - I];
            i32 c;
            i32 e;

            si (g == ZEPHYRUM)
            {
                perge;
            }
            c = g < q ? g : q;
            e = g < q ? q : g;
            si ((a < c && c < b && b < e) || (c < a && a < e && e < b))
            {
                arcus[k - I].cruces = arcus[k - I].cruces + I;
                arcus[q - I].cruces = arcus[q - I].cruces + I;
            }
        }
        /* non proiectivus: verbum interius a capite non dominatum */
        per (p = a + I; p < b; p++)
        {
            si (!_dominat(&s, h, p))
            {
                arcus[k - I].non_proiectivus = VERUM;
                frange;
            }
        }
        arcus[k - I].gradus = _gradus_arcus(&s, k);
    }
    redde VERUM;
}

s32
dependentia_nucleus_planaris (
                      Piscina* piscina,
    constans DependentiaArbor* arbor)
{
    i32  n;
    i32  k;
    i32  longitudo;
    i32* initia;      /* CSR: dextri arcuum a sinistro i */
    i32* dextri;
    s32* f;           /* f[(i - 1) n + (j - 1)] = optimum intra [i, j] */

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || arbor->numerus > (i32)0xFFFF)
    {
        redde (s32)-I;
    }
    n       = arbor->numerus;
    initia  = (i32*)_allocare(piscina, n + II, (i32)magnitudo(i32));
    dextri  = (i32*)_allocare(piscina, n, (i32)magnitudo(i32));
    f       = (s32*)piscina_conari_allocare(piscina, (memoriae_index)n
        * (memoriae_index)n * (memoriae_index)magnitudo(s32));
    si (initia == NIHIL || dextri == NIHIL || f == NIHIL)
    {
        redde (s32)-I;
    }
    memset(f, ZEPHYRUM, (size_t)n * (size_t)n * magnitudo(s32));
    /* numerare per sinistrum, deinde collocare */
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];

        si (h != ZEPHYRUM)
        {
            i32 a = h < k ? h : k;

            initia[a + I] = initia[a + I] + I;
        }
    }
    per (k = I; k <= n; k++)
    {
        initia[k + I] = initia[k + I] + initia[k];
    }
    {
        i32* positio = (i32*)_allocare(piscina, n + I,
            (i32)magnitudo(i32));

        si (positio == NIHIL)
        {
            redde (s32)-I;
        }
        per (k = I; k <= n; k++)
        {
            positio[k] = initia[k];
        }
        per (k = I; k <= n; k++)
        {
            i32 h = arbor->capita[k - I];

            si (h != ZEPHYRUM)
            {
                i32 a = h < k ? h : k;
                i32 b = h < k ? k : h;

                dextri[positio[a]]  = b;
                positio[a]          = positio[a] + I;
            }
        }
    }
    /* f(i, j) = h(i, j) + [arcus (i, j)]; h(i, j) = max(f(i + 1, j),
     * max per arcus (i, k), i < k < j: f(i, k) + f(k, j)). Arcus (i, k)
     * longissimus ex i electus nullum arcum intra [i, k] secat; arcus
     * electi aut intra [i, k] aut intra [k, j] iacent. */
    per (longitudo = I; longitudo < n; longitudo++)
    {
        i32 i;

        per (i = I; i + longitudo <= n; i++)
        {
            i32 j        = i + longitudo;
            /* f(i + 1, j): i + 1 <= j semper */
            s32 optimum  = f[(i32)(i * n + (j - I))];
            i32 q;
            b32 arcus    = FALSUM;

            per (q = initia[i]; q < initia[i + I]; q++)
            {
                i32 kk = dextri[q];

                si (kk == j)
                {
                    arcus = VERUM;
                }
                alioquin si (kk < j)
                {
                    s32 v = f[(i32)((i - I) * n + (kk - I))]
                        + f[(i32)((kk - I) * n + (j - I))];

                    si (v > optimum)
                    {
                        optimum = v;
                    }
                }
            }
            f[(i32)((i - I) * n + (j - I))] = optimum
                + (arcus ? I : ZEPHYRUM);
        }
    }
    redde f[(i32)(n - I)];   /* f(1, n) */
}

/* radix collectionis disiunctae (compressio viae per dimidium) */
interior i32
_radix (
    i32* parens,
    i32  x)
{
    dum (parens[x] != x)
    {
        parens[x]  = parens[parens[x]];
        x          = parens[x];
    }
    redde x;
}

s32
dependentia_componentes (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          s32* componens)
{
    i32  n;
    i32  k;
    i32  q;
    s32  numerus = ZEPHYRUM;
    i32* parens;
    s32* nota;

    si (   dependentia_validare(arbor) != DEPENDENTIA_SANA
        || componens                   == NIHIL)
    {
        redde (s32)-I;
    }
    n       = arbor->numerus;
    parens  = (i32*)_allocare(piscina, n + I, (i32)magnitudo(i32));
    nota    = (s32*)_allocare(piscina, n + I, (i32)magnitudo(s32));
    si (parens == NIHIL || nota == NIHIL)
    {
        redde (s32)-I;
    }
    per (k = I; k <= n; k++)
    {
        parens[k]  = k;
        nota[k]    = (s32)-I;
    }
    /* arcus nominatur verbo dependenti */
    per (k = I; k <= n; k++)
    {
        i32 h = arbor->capita[k - I];
        i32 a;
        i32 b;

        si (h == ZEPHYRUM)
        {
            perge;
        }
        a = h < k ? h : k;
        b = h < k ? k : h;
        per (q = k + I; q <= n; q++)
        {
            i32 g = arbor->capita[q - I];
            i32 c;
            i32 e;

            si (g == ZEPHYRUM)
            {
                perge;
            }
            c = g < q ? g : q;
            e = g < q ? q : g;
            si ((a < c && c < b && b < e) || (c < a && a < e && e < b))
            {
                i32 ra = _radix(parens, k);
                i32 rb = _radix(parens, q);

                si (ra != rb)
                {
                    parens[rb] = ra;
                }
            }
        }
    }
    per (k = I; k <= n; k++)
    {
        i32 r;

        si (arbor->capita[k - I] == ZEPHYRUM)
        {
            componens[k - I] = (s32)-I;
            perge;
        }
        r = _radix(parens, k);
        si (nota[r] < ZEPHYRUM)
        {
            nota[r] = numerus;
            numerus = numerus + I;
        }
        componens[k - I] = nota[r];
    }
    redde numerus;
}
#undef Structura
#undef _allocare
#undef _alternant
#undef _dominat
#undef _gradus_arcus
#undef _radix
#undef _structura
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
/* lib/filum.c: statica per plagulam renominata */
#define _filum_error_buffer _filum_error_buffer_filum
#define _filum_error_est _filum_error_est_filum
#define _filum_error_ponere _filum_error_ponere_filum
#define _filum_error_purgare _filum_error_purgare_filum
#line 1 "lib/filum.c"




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Platform-specific headers */
#ifdef _WIN32
    #include <sys/stat.h>
    #include <io.h>
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <sys/file.h>
    #include <time.h>
#endif


/* ==================================================
 * Structura FilumLector - Internal
 * ================================================== */

structura FilumLector {
         FILUM* descriptum;
           i32  numerus_versus;
           b32  est_finis;
       Piscina* piscina;
     character  buffer[IV * MXXIV]; /* 4096 byte buffer pro legere */
};

structura FilumScriptor {
     FILUM* descriptum;
   Piscina* piscina;
};


/* ==================================================
 * Error Tracking - Thread-unsafe but simple
 * ================================================== */

hic_manens character _filum_error_buffer[CCLVI]; /* 256 bytes */
hic_manens       b32 _filum_error_est = FALSUM;

interior vacuum
_filum_error_ponere (
    constans character* message)
{
    strncpy(_filum_error_buffer, message, CCLV);
    _filum_error_buffer[CCLV]  = '\0';
    _filum_error_est           = VERUM;
}

interior vacuum
_filum_error_purgare (
    vacuum)
{
    _filum_error_est               = FALSUM;
    _filum_error_buffer[ZEPHYRUM]  = '\0';
}


/* ==================================================
 * Lectio Simplex
 * ================================================== */

chorda
filum_legere_totum (
    constans character* via,
               Piscina* piscina)
{
      structura stat st;
             FILUM* f;
    memoriae_index  mensura;
                i8* buffer;
    memoriae_index  legere_summa;
            chorda  fructus;

    si (!via || !piscina)
    {
        _filum_error_ponere("via vel piscina est NIHIL");
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    _filum_error_purgare();

    /* Invenire mensuram per stat() - velocius quam fseek/ftell */
    si (stat(via, &st) != ZEPHYRUM)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        _filum_error_ponere("stat fracta");
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    mensura = (memoriae_index)st.st_size;

    /* Aperire filum */
    f = fopen(via, "rb");
    si (!f)
    {
        _filum_error_ponere("fopen fracta");
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Allocare buffer */
    buffer = (i8*)piscina_allocare(piscina, mensura);
    si (!buffer)
    {
        fclose(f);
        _filum_error_ponere("piscina_allocare fracta");
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    /* Legere contentum */
    legere_summa = fread(buffer, I, mensura, f);
    fclose(f);

    si (legere_summa != mensura)
    {
        _filum_error_ponere("fread fracta - non omnia legere");
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }

    lectiones_notare(LECTIO_LEGIT, via);
    fructus.mensura  = (i32)mensura;
    fructus.datum    = buffer;
    redde fructus;
}

b32
filum_existit (
    constans character* via)
{
    FILUM* f;

    si (!via) redde FALSUM;

    f = fopen(via, "r");
    si (!f)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde FALSUM;
    }

    fclose(f);
    lectiones_notare(LECTIO_EXSTAT, via);
    redde VERUM;
}

memoriae_index
filum_mensura (
    constans character* via)
{
     FILUM* f;
    longus  mensura_signed;

    si (!via) redde ZEPHYRUM;

    f = fopen(via, "rb");
    si (!f)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde ZEPHYRUM;
    }
    /* mensura ex contentis pendet: lectio (L), non solum exstantia */
    lectiones_notare(LECTIO_LEGIT, via);

    si (fseek(f, ZEPHYRUM, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde ZEPHYRUM;
    }

    mensura_signed = ftell(f);
    fclose(f);

    si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

    redde (memoriae_index)mensura_signed;
}


/* ==================================================
 * Lectio per Lineas
 * ================================================== */

FilumLector*
filum_lector_aperire (
    constans character* via,
               Piscina* piscina)
{
    FilumLector* lector;

    si (!via || !piscina)
    {
        _filum_error_ponere("via vel piscina est NIHIL");
        redde NIHIL;
    }

    _filum_error_purgare();

    lector = (FilumLector*)piscina_allocare(piscina,
        magnitudo(FilumLector));
    si (!lector)
    {
        _filum_error_ponere("piscina_allocare fracta");
        redde NIHIL;
    }

    lector->descriptum = fopen(via, "r");
    si (!lector->descriptum)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        _filum_error_ponere("fopen fracta");
        redde NIHIL;
    }
    lectiones_notare(LECTIO_LEGIT, via);

    lector->numerus_versus  = ZEPHYRUM;
    lector->est_finis       = FALSUM;
    lector->piscina         = piscina;

    redde lector;
}

b32
filum_lector_lineam_proximam (
    FilumLector* lector,
         chorda* linea_out)
{
                 character* result;
            memoriae_index  longitudo;
                        i8* buffer_allocatus;
                       b32  habet_newline;
         ChordaAedificator* aedificator;
            memoriae_index  i;

    si (!lector || !linea_out)
    {
        redde FALSUM;
    }

    si (lector->est_finis)
    {
        linea_out->mensura  = ZEPHYRUM;
        linea_out->datum    = NIHIL;
        redde FALSUM;
    }

    /* Prima lectio */
    result = fgets(lector->buffer, IV * MXXIV, lector->descriptum);
    si (!result)
    {
        /* EOF vel error */
        lector->est_finis   = VERUM;
        linea_out->mensura  = ZEPHYRUM;
        linea_out->datum    = NIHIL;
        redde FALSUM;
    }

    lector->numerus_versus++;

    longitudo = strlen(lector->buffer);

    /* Verificare si habet newline ad finem */
    habet_newline = FALSUM;
    si (   longitudo > ZEPHYRUM

        && (lector->buffer[longitudo - I] == '\n'

        || lector->buffer[longitudo - I] == '\r'))
    {
        habet_newline = VERUM;
    }

    /* Si linea completa (habet newline VEL non implevit buffer),
	 * processare directe */
    si (habet_newline || longitudo < (IV * MXXIV - I))
    {
        /* Removere newline characteres */
        dum (   longitudo > ZEPHYRUM

             && (lector->buffer[longitudo - I] == '\n'

            || lector->buffer[longitudo - I] == '\r'))
        {
            lector->buffer[longitudo - I] = '\0';
            longitudo--;
        }

        /* Si linea vacua (longitudo == 0), redde chordam vacuam */
        si (longitudo == ZEPHYRUM)
        {
            linea_out->mensura  = ZEPHYRUM;
            linea_out->datum    = NIHIL;
            redde VERUM;
        }

        /* Allocare ex piscina */
        buffer_allocatus = (i8*)piscina_allocare(lector->piscina,
            longitudo);
        si (!buffer_allocatus)
        {
            _filum_error_ponere("piscina_allocare fracta");
            linea_out->mensura  = ZEPHYRUM;
            linea_out->datum    = NIHIL;
            redde FALSUM;
        }

        memcpy(buffer_allocatus, lector->buffer, longitudo);
        linea_out->mensura  = (i32)longitudo;
        linea_out->datum    = buffer_allocatus;

        redde VERUM;
    }

    /* Linea continuat - usare ChordaAedificator pro crescentia dynamica */
    aedificator = chorda_aedificator_creare(lector->piscina,
        IV * MXXIV * II);
    si (!aedificator)
    {
        _filum_error_ponere("chorda_aedificator_creare fracta");
        linea_out->mensura  = ZEPHYRUM;
        linea_out->datum    = NIHIL;
        redde FALSUM;
    }

    /* Appendere primum fragmentum */
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (!chorda_aedificator_appendere_character(aedificator,
            lector->buffer[i]))
        {
            _filum_error_ponere("chorda_aedificator_appendere fracta");
            linea_out->mensura  = ZEPHYRUM;
            linea_out->datum    = NIHIL;
            redde FALSUM;
        }
    }

    /* Legere fragmenta reliqua usque ad newline vel EOF */
    dum (VERUM)
    {
        result = fgets(lector->buffer, IV * MXXIV, lector->descriptum);
        si (!result)
        {
            /* EOF - finire lineam */
            lector->est_finis  = VERUM;
            *linea_out         = chorda_aedificator_finire(aedificator);
            redde VERUM;
        }

        longitudo = strlen(lector->buffer);

        /* Verificare newline */
        habet_newline = FALSUM;
        si (   longitudo > ZEPHYRUM

            && (lector->buffer[longitudo - I] == '\n'

            || lector->buffer[longitudo - I] == '\r'))
        {
            habet_newline = VERUM;
        }

        /* Removere newline si existit */
        dum (   longitudo > ZEPHYRUM

             && (lector->buffer[longitudo - I] == '\n'

            || lector->buffer[longitudo - I] == '\r'))
        {
            lector->buffer[longitudo - I] = '\0';
            longitudo--;
        }

        /* Appendere fragmentum */
        per (i = ZEPHYRUM; i < longitudo; i++)
        {
            si (!chorda_aedificator_appendere_character(aedificator,
                lector->buffer[i]))
            {
                _filum_error_ponere("chorda_aedificator_appendere fracta");
                linea_out->mensura  = ZEPHYRUM;
                linea_out->datum    = NIHIL;
                redde FALSUM;
            }
        }

        si (habet_newline || longitudo < (IV * MXXIV - I))
        {
            /* Linea completa */
            *linea_out = chorda_aedificator_finire(aedificator);
            redde VERUM;
        }
    }
}

i32
filum_lector_numerus_versus (
    FilumLector* lector)
{
    redde lector ? lector->numerus_versus : ZEPHYRUM;
}

b32
filum_lector_finis (
    FilumLector* lector)
{
    redde lector ? lector->est_finis : VERUM;
}

vacuum
filum_lector_claudere (
    FilumLector* lector)
{
    si (!lector) redde;

    si (lector->descriptum)
    {
        fclose(lector->descriptum);
        lector->descriptum = NIHIL;
    }

    /* Nota: FilumLector ipsa allocata ex piscina,
	 * ergo non liberatur manualiter */
}


/* ==================================================
 * FilumScriptor - Scriptio Continua
 * ================================================== */


/* ==================================================
 * FilumOctetorumLector - Lectio Binaria per Partes
 * ================================================== */

structura FilumOctetorumLector {
    FILUM* descriptum;
};

FilumOctetorumLector*
filum_octeti_aperire (
    constans character* via,
               Piscina* piscina)
{
    FilumOctetorumLector* lector;

    si (!via || !piscina)
    {
        _filum_error_ponere("via vel piscina est NIHIL");
        redde NIHIL;
    }
    _filum_error_purgare();
    lector = (FilumOctetorumLector*)piscina_allocare(piscina,
        magnitudo(FilumOctetorumLector));
    si (!lector)
    {
        _filum_error_ponere("piscina_allocare fracta");
        redde NIHIL;
    }
    /* lectiones_fopen ipsa L aut A notat */
    lector->descriptum = lectiones_fopen(via, "rb");
    si (!lector->descriptum)
    {
        _filum_error_ponere("fopen fracta");
        redde NIHIL;
    }
    redde lector;
}

s32
filum_octeti_legere (
    FilumOctetorumLector* lector,
                      i8* capsa,
                     i32  n)
{
    size_t lecta;

    si (!lector || !lector->descriptum || !capsa)
    {
        redde -I;
    }
    si (n == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    lecta = fread(capsa, I, (size_t)n, lector->descriptum);
    si (lecta == ZEPHYRUM && ferror(lector->descriptum))
    {
        redde -I;
    }
    redde (s32)lecta;
}

vacuum
filum_octeti_claudere (
    FilumOctetorumLector* lector)
{
    si (lector && lector->descriptum)
    {
        fclose(lector->descriptum);
        lector->descriptum = NIHIL;
    }
}

FilumScriptor*
filum_scriptor_aperire (
     constans character* via,
             FilumModus  modus,
                Piscina* piscina)
{
         FilumScriptor* scriptor;
    constans character* modus_str;

    si (!via || !piscina)
    {
        _filum_error_ponere("via vel piscina est NIHIL");
        redde NIHIL;
    }

    _filum_error_purgare();

    scriptor = (FilumScriptor*)piscina_allocare(piscina,
        magnitudo(FilumScriptor));
    si (!scriptor)
    {
        _filum_error_ponere("piscina_allocare fracta");
        redde NIHIL;
    }

    /* Determinare modus string */
    si (modus == FILUM_MODUS_APPENDERE)
    {
        modus_str = "ab";
    }
    alioquin
    {
        modus_str = "wb";
    }

    scriptor->descriptum = fopen(via, modus_str);
    si (scriptor->descriptum)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    si (!scriptor->descriptum)
    {
        _filum_error_ponere("fopen fracta");
        redde NIHIL;
    }

    scriptor->piscina = piscina;

    redde scriptor;
}

b32
filum_scriptor_scribere (
     FilumScriptor* scriptor,
            chorda  contentum)
{
    memoriae_index scriptus;

    si (!scriptor || !scriptor->descriptum)
    {
        _filum_error_ponere("scriptor invalidus");
        redde FALSUM;
    }

    si (!contentum.datum || contentum.mensura == ZEPHYRUM)
    {
        /* Nihil scribere - successus */
        redde VERUM;
    }

    _filum_error_purgare();

    scriptus = fwrite(contentum.datum, I,
        (memoriae_index)contentum.mensura,
                      scriptor->descriptum);

    si (scriptus != (memoriae_index)contentum.mensura)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_scriptor_octetos_scribere (
    FilumScriptor* scriptor,
      constans i8* octeti,
              i32  n)
{
    memoriae_index scriptus;

    si (!scriptor || !scriptor->descriptum || (n > ZEPHYRUM && !octeti))
    {
        _filum_error_ponere("scriptor invalidus");
        redde FALSUM;
    }
    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    _filum_error_purgare();
    scriptus = fwrite(octeti, I, (memoriae_index)n,
        scriptor->descriptum);
    si (scriptus != (memoriae_index)n)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }
    redde VERUM;
}

b32
filum_scriptor_scribere_literis (
         FilumScriptor* scriptor,
    constans character* contentum)
{
    memoriae_index longitudo;
    memoriae_index scriptus;

    si (!scriptor || !scriptor->descriptum)
    {
        _filum_error_ponere("scriptor invalidus");
        redde FALSUM;
    }

    si (!contentum)
    {
        /* Nihil scribere - successus */
        redde VERUM;
    }

    _filum_error_purgare();

    longitudo = strlen(contentum);
    si (longitudo == ZEPHYRUM)
    {
        redde VERUM;
    }

    scriptus = fwrite(contentum, I, longitudo, scriptor->descriptum);

    si (scriptus != longitudo)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_scriptor_lineam_scribere (
         FilumScriptor* scriptor,
    constans character* linea)
{
    si (!scriptor || !scriptor->descriptum)
    {
        _filum_error_ponere("scriptor invalidus");
        redde FALSUM;
    }

    _filum_error_purgare();

    /* Scribere lineam */
    si (linea)
    {
        memoriae_index longitudo = strlen(linea);
        si (longitudo > ZEPHYRUM)
        {
            memoriae_index scriptus = fwrite(linea, I, longitudo,
                                             scriptor->descriptum);
            si (scriptus != longitudo)
            {
                _filum_error_ponere("fwrite fracta");
                redde FALSUM;
            }
        }
    }

    /* Scribere newline */
    si (fputc('\n', scriptor->descriptum) == EOF)
    {
        _filum_error_ponere("fputc fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_scriptor_sync (
    FilumScriptor* scriptor)
{
    si (!scriptor || !scriptor->descriptum)
    {
        _filum_error_ponere("scriptor invalidus");
        redde FALSUM;
    }

    _filum_error_purgare();

    si (fflush(scriptor->descriptum) != ZEPHYRUM)
    {
        _filum_error_ponere("fflush fracta");
        redde FALSUM;
    }

    redde VERUM;
}

vacuum
filum_scriptor_claudere (
    FilumScriptor* scriptor)
{
    si (!scriptor) redde;

    si (scriptor->descriptum)
    {
        fclose(scriptor->descriptum);
        scriptor->descriptum = NIHIL;
    }

    /* Nota: FilumScriptor ipsa allocata ex piscina,
	 * ergo non liberatur manualiter */
}


/* ==================================================
 * Scriptio
 * ================================================== */

b32
filum_scribere (
    constans character* via,
                chorda  contentum)
{
             FILUM* f;
    memoriae_index  scriptus;

    si (!via || !contentum.datum)
    {
        _filum_error_ponere("via vel contentum est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    f = fopen(via, "wb");
    si (f)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    si (!f)
    {
        _filum_error_ponere("fopen pro scribere fracta");
        redde FALSUM;
    }

    scriptus = fwrite(contentum.datum, I,
        (memoriae_index)contentum.mensura, f);
    fclose(f);

    si (scriptus != (memoriae_index)contentum.mensura)
    {
        _filum_error_ponere("fwrite fracta - non omnia scripta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_scribere_literis (
    constans character* via,
    constans character* contentum)
{
             FILUM* f;
    memoriae_index  longitudo;
    memoriae_index  scriptus;

    si (!via || !contentum)
    {
        _filum_error_ponere("via vel contentum est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    f = fopen(via, "w");
    si (f)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    si (!f)
    {
        _filum_error_ponere("fopen pro scribere fracta");
        redde FALSUM;
    }

    longitudo  = strlen(contentum);
    scriptus   = fwrite(contentum, I, longitudo, f);
    fclose(f);

    si (scriptus != longitudo)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_appendere (
    constans character* via,
                chorda  contentum)
{
             FILUM* f;
    memoriae_index  scriptus;

    si (!via || !contentum.datum)
    {
        _filum_error_ponere("via vel contentum est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    f = fopen(via, "ab");
    si (f)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    si (!f)
    {
        _filum_error_ponere("fopen pro appendere fracta");
        redde FALSUM;
    }

    scriptus = fwrite(contentum.datum, I,
        (memoriae_index)contentum.mensura, f);
    fclose(f);

    si (scriptus != (memoriae_index)contentum.mensura)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }

    redde VERUM;
}

/* vates-plan-2 T1: write() UNUM + fsync() - annales 'append-only'
 * (rationarium vatis, index herbarii, acta optchat). fflush solum
 * (filum_scriptor_sync) discum non attingit. */
b32
filum_appendere_firmiter (
    constans character* via,
                chorda  contentum)
{
#ifdef _WIN32
    (vacuum)contentum;
    _filum_error_ponere("filum_appendere_firmiter: _WIN32 nondum");
    (vacuum)via;
    redde FALSUM;
#else
    integer descriptor;
    ssize_t scripta = 0;
        b32 bene;

    si (!via || (contentum.mensura > 0 && !contentum.datum))
    {
        _filum_error_ponere("via vel contentum est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    descriptor = open(via, O_WRONLY | O_APPEND | O_CREAT, 0644);
    si (descriptor < 0)
    {
        _filum_error_ponere("open pro appendere firmiter fracta");
        redde FALSUM;
    }
    lectiones_notare(LECTIO_SCRIPSIT, via);

    si (contentum.mensura > 0)
    {
        scripta = write(descriptor, contentum.datum,
                        (size_t)contentum.mensura);
    }
    bene = (scripta == (ssize_t)contentum.mensura);
    si (!bene)
    {
        _filum_error_ponere("write fracta - non omnia scripta");
    }
    si (bene && fsync(descriptor) != 0)
    {
        _filum_error_ponere("fsync fracta");
        bene = FALSUM;
    }
    si (close(descriptor) != 0 && bene)
    {
        _filum_error_ponere("close fracta");
        bene = FALSUM;
    }
    redde bene;
#endif
}

b32
filum_appendere_literis (
    constans character* via,
    constans character* contentum)
{
             FILUM* f;
    memoriae_index  longitudo;
    memoriae_index  scriptus;

    si (!via || !contentum)
    {
        _filum_error_ponere("via vel contentum est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    f = fopen(via, "a");
    si (f)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    si (!f)
    {
        _filum_error_ponere("fopen pro appendere fracta");
        redde FALSUM;
    }

    longitudo  = strlen(contentum);
    scriptus   = fwrite(contentum, I, longitudo, f);
    fclose(f);

    si (scriptus != longitudo)
    {
        _filum_error_ponere("fwrite fracta");
        redde FALSUM;
    }

    redde VERUM;
}


/* ==================================================
 * Manipulatio Filorum
 * ================================================== */

b32
filum_delere (
    constans character* via)
{
    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    si (remove(via) != ZEPHYRUM)
    {
        _filum_error_ponere("remove fracta");
        redde FALSUM;
    }
    /* deletio = scriptura in libro (lectiones.h: 'S' scripta, deleta,
     * mota) - olim deerat: compilator obiectum temporarium legit et
     * delet, vestigium portae id 'ingressum build/ sine domino'
     * ferebat (fabrica spec 3 T6) */
    lectiones_notare(LECTIO_SCRIPSIT, via);

    redde VERUM;
}

b32
filum_arborem_delere (
    constans character* via)
{
#ifdef _WIN32
    (vacuum)via;
    _filum_error_ponere("filum_arborem_delere: POSIX solum");
    redde FALSUM;
#else
    structura stat  st;
    structura stat  radix;
               b32 deleta;
               i32 transitus;

    si (!via || via[ZEPHYRUM] == '\0')
    {
        _filum_error_ponere("via NIHIL aut vacua");
        redde FALSUM;
    }
    _filum_error_purgare();
    si (lstat(via, &st) != ZEPHYRUM)
    {
        si (errno == ENOENT)
        {
            redde VERUM;
        }
        _filum_error_ponere("lstat fracta");
        redde FALSUM;
    }
    si (!S_ISDIR(st.st_mode))
    {
        /* filum aut NEXUS: ipse removetur, numquam destinatio */
        lectiones_notare(LECTIO_SCRIPSIT, via);
        si (unlink(via) != ZEPHYRUM)
        {
            _filum_error_ponere("unlink fracta");
            redde FALSUM;
        }
        redde VERUM;
    }
    /* RADIX per IDENTITATEM (st_dev + st_ino), non per literas: "//",
     * "/." et "/tmp/.." radicem quoque nominant. Stat radicis fractus
     * = recusatio. NUMQUAM culpam hic plantes: probatio functionem in
     * "/" vocat, et recusatio fracta discum deleret. */
    si (   stat("/", &radix) != ZEPHYRUM
        || (st.st_dev == radix.st_dev && st.st_ino == radix.st_ino))
    {
        _filum_error_ponere("via radix systematis est - non deletur");
        redde FALSUM;
    }
    /* Relegere donec vacuum: readdir post unlink introitus omittere
     * potest. Transitus finiti, ne introitus quem lstat non invenit
     * (VERUM, 'nihil delendum') cursum aeternum faciat - rmdir tunc
     * deficit et nominat. */
    transitus = ZEPHYRUM;
    fac
    {
                   DIR* d;
        structura dirent* e;
              character  filius[IV * MXXIV];

        deleta  = FALSUM;
        d       = opendir(via);
        si (d == NIHIL)
        {
            _filum_error_ponere("opendir fracta");
            redde FALSUM;
        }
        dum ((e = readdir(d)) != NIHIL)
        {
            integer n;

            si (   strcmp(e->d_name, ".")  == ZEPHYRUM
                || strcmp(e->d_name, "..") == ZEPHYRUM)
            {
                perge;
            }
            n = snprintf(filius, magnitudo(filius), "%s/%s", via,
                         e->d_name);
            si (n < ZEPHYRUM || (size_t)n >= magnitudo(filius))
            {
                closedir(d);
                _filum_error_ponere("via nimis longa");
                redde FALSUM;
            }
            si (!filum_arborem_delere(filius))
            {
                closedir(d);
                redde FALSUM;
            }
            deleta = VERUM;
        }
        closedir(d);
        transitus++;
    } dum (deleta && transitus < XVI);
    si (rmdir(via) != ZEPHYRUM)
    {
        _filum_error_ponere("rmdir fracta");
        redde FALSUM;
    }
    redde VERUM;
#endif
}

b32
filum_movere (
    constans character* via_vetus,
    constans character* via_nova)
{
    si (!via_vetus || !via_nova)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    lectiones_notare(LECTIO_SCRIPSIT, via_vetus);
    lectiones_notare(LECTIO_SCRIPSIT, via_nova);
    si (rename(via_vetus, via_nova) != ZEPHYRUM)
    {
        _filum_error_ponere("rename fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_copiare (
    constans character* via_fons,
    constans character* via_destinatio)
{
             FILUM* fons;
             FILUM* dest;
         character  buffer[IV * MXXIV];
    memoriae_index  legere_bytes;
    memoriae_index  scriptus_bytes;

    si (!via_fons || !via_destinatio)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    fons = fopen(via_fons, "rb");
    si (!fons)
    {
        _filum_error_ponere("aperire fons fracta");
        redde FALSUM;
    }

    dest = fopen(via_destinatio, "wb");
    si (dest)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via_destinatio);
    }
    si (!dest)
    {
        fclose(fons);
        _filum_error_ponere("aperire destinatio fracta");
        redde FALSUM;
    }

    /* Copiare per buffer */
    dum ((legere_bytes = fread(buffer, I, IV * MXXIV, fons)) > ZEPHYRUM)
    {
        scriptus_bytes = fwrite(buffer, I, legere_bytes, dest);
        si (scriptus_bytes != legere_bytes)
        {
            fclose(fons);
            fclose(dest);
            _filum_error_ponere("fwrite in copiare fracta");
            redde FALSUM;
        }
    }

    fclose(fons);
    fclose(dest);

    redde VERUM;
}

b32
filum_modum_ponere (
    constans character* via,
               integer  modus)
{
    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

#ifdef _WIN32
    /* MSVC: _chmod ex <io.h>, mode_t non existit. Vicinum
     * filum_directorium_creare_cum_modo eandem custodiam gerit. */
    si (_chmod(via, modus) != ZEPHYRUM)
#else
    si (chmod(via, (mode_t)modus) != ZEPHYRUM)
#endif
    {
        _filum_error_ponere("chmod fracta");
        redde FALSUM;
    }

    redde VERUM;
}


/* ==================================================
 * Directoria
 * ================================================== */

b32
filum_directorium_creare (
    constans character* via)
{
    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    lectiones_notare(LECTIO_SCRIPSIT, via);
#ifdef _WIN32
    si (_mkdir(via) != ZEPHYRUM)
#else
    si (mkdir(via, 0755) != ZEPHYRUM)
#endif
    {
        _filum_error_ponere("mkdir fracta");
        redde FALSUM;
    }

    redde VERUM;
}

b32
filum_directorium_creare_si_necesse (
    constans character* via)
{
    structura stat st;

    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    /* Si iam existit, successus */
    si (stat(via, &st) == ZEPHYRUM)
    {
        si (S_ISDIR(st.st_mode))
        {
            redde VERUM;
        }
        /* Existit sed non est directorium */
        _filum_error_ponere("via existit sed non est directorium");
        redde FALSUM;
    }

    /* Non existit - creare. CONCURSUS: alter processus inter stat()
     * et mkdir() creare potest (EEXIST) - directorium tum 'iam
     * existit', ergo successus (thesaurus, II scriptores, plan 2 T3) */
    si (filum_directorium_creare(via))
    {
        redde VERUM;
    }
    si (stat(via, &st) == ZEPHYRUM && S_ISDIR(st.st_mode))
    {
        _filum_error_purgare();
        redde VERUM;
    }
    redde FALSUM;
}

b32
filum_directorium_creare_cum_modo (
    constans character* via,
               integer  modus)
{
    structura stat st;

    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    si (stat(via, &st) == ZEPHYRUM)
    {
        si (!S_ISDIR(st.st_mode))
        {
            _filum_error_ponere("via existit sed non est directorium");
            redde FALSUM;
        }
        /* IAM EXSISTIT: modum coercere. Directorium quod alia causa
		 * laxius creavit aliter tacite laxum maneret - et defectus
		 * permissionum is est qui numquam clamat. */
#ifndef _WIN32
        si (chmod(via, (mode_t)modus) != ZEPHYRUM)
        {
            _filum_error_ponere("chmod fracta");
            redde FALSUM;
        }
#endif
        redde VERUM;
    }

    lectiones_notare(LECTIO_SCRIPSIT, via);
#ifdef _WIN32
    (vacuum)modus;
    si (_mkdir(via) != ZEPHYRUM)
#else
    si (mkdir(via, (mode_t)modus) != ZEPHYRUM)
#endif
    {
        _filum_error_ponere("mkdir fracta");
        redde FALSUM;
    }
#ifndef _WIN32
    /* umask mkdir corrigit; chmod explicitus modum VERUM ponit */
    si (chmod(via, (mode_t)modus) != ZEPHYRUM)
    {
        _filum_error_ponere("chmod post mkdir fracta");
        redde FALSUM;
    }
#endif
    redde VERUM;
}

b32
filum_directorium_creare_cum_parentibus (
    constans character* via)
{
         character semita[IV * MXXIV];
    memoriae_index longitudo;
    memoriae_index i;

    si (!via)
    {
        _filum_error_ponere("via est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    longitudo = strlen(via);
    si (longitudo == ZEPHYRUM)
    {
        _filum_error_ponere("via vacua est");
        redde FALSUM;
    }
    si (longitudo >= magnitudo(semita))
    {
        _filum_error_ponere("via nimis longa");
        redde FALSUM;
    }

    memcpy(semita, via, longitudo);
    semita[longitudo] = '\0';

    /* Ab I incipimus: '/' primum semitae absolutae numquam creatur.
     * Segmentum quodque truncando terminamus, creamus, restituimus. */
    per (i = I; i < longitudo; i++)
    {
        si (semita[i] != '/')
        {
            perge;
        }
        si (semita[i - I] == '/')
        {
            perge;   /* '//' duplicatum: segmentum vacuum */
        }
        semita[i] = '\0';
        si (!filum_directorium_creare_si_necesse(semita))
        {
            /* Causa interior SERVATUR: 'mkdir fracta' aut 'via
             * existit sed non est directorium' plus dicit quam
             * nuntius generalis quem hic ponere possem */
            redde FALSUM;
        }
        semita[i] = '/';
    }

    /* Semita in '/' desinens parentes suos solos nominat */
    si (semita[longitudo - I] == '/')
    {
        redde VERUM;
    }
    redde filum_directorium_creare_si_necesse(semita);
}

b32
filum_directorium_existit (
    constans character* via)
{
    structura stat st;

    si (!via) redde FALSUM;

    si (stat(via, &st) != ZEPHYRUM)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        redde FALSUM;
    }
    lectiones_notare(LECTIO_EXSTAT, via);

    redde S_ISDIR(st.st_mode);
}


/* ==================================================
 * Interrogatio Status
 * ================================================== */

b32
filum_status (
    constans character* via,
           FilumStatus* status_out)
{
    structura stat st;

    si (!via || !status_out)
    {
        _filum_error_ponere("via vel status_out est NIHIL");
        redde FALSUM;
    }

    _filum_error_purgare();

    si (stat(via, &st) != ZEPHYRUM)
    {
        lectiones_notare(LECTIO_ABSENS, via);
        _filum_error_ponere("stat fracta");
        redde FALSUM;
    }
    lectiones_notare(LECTIO_EXSTAT, via);

    status_out->mensura = (memoriae_index)st.st_size;

#ifdef _WIN32
    status_out->est_directorium = (st.st_mode & _S_IFDIR) != ZEPHYRUM;
    status_out->est_filum       = (st.st_mode & _S_IFREG) != ZEPHYRUM;
    status_out->potest_legere   = (st.st_mode & _S_IREAD) != ZEPHYRUM;
    status_out->potest_scribere = (st.st_mode & _S_IWRITE) != ZEPHYRUM;
    status_out->potest_exsequi  = (st.st_mode & _S_IEXEC) != ZEPHYRUM;
#else
    status_out->est_directorium  = S_ISDIR(st.st_mode);
    status_out->est_filum        = S_ISREG(st.st_mode);
    status_out->potest_legere    = (st.st_mode & S_IRUSR) != ZEPHYRUM;
    status_out->potest_scribere  = (st.st_mode & S_IWUSR) != ZEPHYRUM;
    status_out->potest_exsequi   = (st.st_mode & S_IXUSR) != ZEPHYRUM;
#endif

    redde VERUM;
}


/* ==================================================
 * Errores
 * ================================================== */

constans character*
filum_error_recens (
    vacuum)
{
    si (!_filum_error_est) redde NIHIL;
    redde _filum_error_buffer;
}


/* ==================================================
 * Sera (lapide feature-requests/020)
 * ================================================== */

structura FilumSera {
    integer descriptor;    /* -I post liberationem */
};

FilumSera*
filum_seram_capere (
    constans character* via,
                   i32  mora_ms,
               Piscina* piscina)
{
#ifdef _WIN32
    (vacuum)via;
    (vacuum)mora_ms;
    (vacuum)piscina;
    redde NIHIL;
#else
     FilumSera* sera;
       integer  descriptor;
           i32  exspectatum = ZEPHYRUM;

    si (via == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    descriptor = open(via, O_RDWR | O_CREAT, 0644);
    si (descriptor < ZEPHYRUM)
    {
        redde NIHIL;    /* e.g. directorium, permissio */
    }
    /* filii (processus_exsequi) descriptorem ne hereditent: sera
     * aliter vitam patris excederet */
    (vacuum)fcntl(descriptor, F_SETFD, FD_CLOEXEC);
    dum (flock(descriptor, LOCK_EX | LOCK_NB) != ZEPHYRUM)
    {
        structura timespec pausa;

        si (errno == EINTR)
        {
            perge;
        }
        si (errno != EWOULDBLOCK || exspectatum >= mora_ms)
        {
            (vacuum)close(descriptor);
            redde NIHIL;
        }
        pausa.tv_sec   = ZEPHYRUM;
        pausa.tv_nsec  = X * M * M;    /* X ms */
        (vacuum)nanosleep(&pausa, NIHIL);
        exspectatum = exspectatum + X;
    }
    sera = (FilumSera*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(FilumSera));
    si (sera == NIHIL)
    {
        (vacuum)close(descriptor);
        redde NIHIL;
    }
    sera->descriptor = descriptor;
    redde sera;
#endif
}

vacuum
filum_seram_liberare (
    FilumSera* sera)
{
    si (sera == NIHIL || sera->descriptor < ZEPHYRUM)
    {
        redde;
    }
#ifndef _WIN32
    (vacuum)flock(sera->descriptor, LOCK_UN);
    (vacuum)close(sera->descriptor);
#endif
    sera->descriptor = -I;
}
#undef _filum_error_buffer
#undef _filum_error_est
#undef _filum_error_ponere
#undef _filum_error_purgare
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
/* lib/lectiones.c: statica per plagulam renominata */
#define _descriptor _descriptor_lectiones
#define _littera _littera_lectiones
#define _machina_est _machina_est_lectiones
#define _via_aperta _via_aperta_lectiones
#line 1 "lib/lectiones.c"
/* lectiones.c - liber lectionum (fabrica plan 2 T1). Vide lectiones.h.
 *
 * Variabilis ambitus omni vocatione legitur (vilis): probationes eam
 * ponunt et tollunt, et descriptor apertus viae NOVAE non servit.
 * Descriptor per processum servatur dum via eadem manet; filius post
 * fork descriptorem patris heredat - O_APPEND lineas integras servat.
 */


#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


hic_manens  integer  _descriptor = -I;
hic_manens character _via_aperta[IV * MXXIV];

interior character
_littera (
    LectioGenus genus)
{
    commutatio (genus)
    {
        casus LECTIO_LEGIT:       redde 'L';
        casus LECTIO_ABSENS:      redde 'A';
        casus LECTIO_EXSTAT:      redde 'X';
        casus LECTIO_ENUMERAVIT:  redde 'D';
        casus LECTIO_SCRIPSIT:    redde 'S';
        casus LECTIO_AMBITUS:     redde 'E';
    }
    redde '?';
}

vacuum
lectiones_notare (
                 LectioGenus  genus,
          constans character* via)
{
    constans character* liber;
             character  linea[IV * MXXIV + IV];
        memoriae_index  mensura_viae;

    liber = getenv("FABRICA_LECTIONES");
    si (liber == NIHIL || liber[0] == '\0')
    {
        /* absens = CLAUSUM: descriptor servatus plagulam interea
         * deletam (eadem via, inodus alius) scriberet - probatio III
         * lineas omnes ita amisit */
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
        }
        redde;
    }
    si (via == NIHIL)
    {
        redde;
    }
    si (_descriptor < 0 || strcmp(liber, _via_aperta) != 0)
    {
        si (_descriptor >= 0)
        {
            (vacuum)close(_descriptor);
            _descriptor = -I;
        }
        si (strlen(liber) >= magnitudo(_via_aperta))
        {
            redde;
        }
        _descriptor = open(liber, O_WRONLY | O_APPEND | O_CREAT, 0644);
        si (_descriptor < 0)
        {
            redde;
        }
        strcpy(_via_aperta, liber);
    }
    mensura_viae = strlen(via);
    si (mensura_viae + IV > magnitudo(linea))
    {
        redde;   /* linea numquam scinditur */
    }
    linea[0] = _littera(genus);
    linea[I] = '\t';
    memcpy(linea + II, via, mensura_viae);
    linea[II + mensura_viae] = '\n';
    {
        /* optima conatio: defectus hic tractari nequit. Valor ad
         * variabilem, non (vacuum) solum: glibc cum _FORTIFY_SOURCE
         * write() warn_unused_result notat, quod gcc mittendo in
         * vacuum NON tacet (hospitium v3 T9) */
        ssize_t scripta = write(_descriptor, linea, mensura_viae + III);

        (vacuum)scripta;
    }
}

constans character*
lectiones_ambitus (
         constans character* titulus)
{
    constans character* valor;
             character  linea[IV * MXXIV];

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }
    valor = getenv(titulus);
    si (strlen(titulus) + (valor != NIHIL ? strlen(valor) : 0) + II
            >= magnitudo(linea))
    {
        redde valor;   /* nimis longum: non notatur, ut via longa */
    }
    strcpy(linea, titulus);
    si (valor != NIHIL)
    {
        strcat(linea, "\t");
        strcat(linea, valor);
    }
    lectiones_notare(LECTIO_AMBITUS, linea);
    redde valor;
}

/* via machinae (/dev/): non ingressus */
interior b32
_machina_est (
    constans character* via)
{
    redde via != NIHIL && strncmp(via, "/dev/", V) == ZEPHYRUM;
}

FILE*
lectiones_fopen (
    constans character* via,
    constans character* modus)
{
     FILE* f = fopen(via, modus);
      b32  lectio;

    si (via == NIHIL || modus == NIHIL || _machina_est(via))
    {
        redde f;
    }
    lectio = modus[ZEPHYRUM] == 'r' && strchr(modus, '+') == NIHIL;
    si (lectio)
    {
        lectiones_notare(f != NIHIL ? LECTIO_LEGIT : LECTIO_ABSENS,
            via);
    }
    alioquin si (f != NIHIL)
    {
        lectiones_notare(LECTIO_SCRIPSIT, via);
    }
    redde f;
}

integer
lectiones_stat (
    constans character* via,
       structura stat* status)
{
    integer r = stat(via, status);

    si (via != NIHIL && !_machina_est(via))
    {
        lectiones_notare(r == ZEPHYRUM ? LECTIO_EXSTAT : LECTIO_ABSENS,
            via);
    }
    redde r;
}
#undef _descriptor
#undef _littera
#undef _machina_est
#undef _via_aperta
#line 1 "knotapel/demo_134_umbra_width/main.c"
/*
 * KNOTAPEL DEMO 134: umbra width of the toy world
 * ===============================================
 *
 * Proposal N17 (plan.md, approved 2026-10-20 after a design review;
 * brainstorm sections XI, XV, XVII). Reading left to right, a reader
 * carries what the structure has promised and not yet delivered: a word
 * waiting for its head, a verb waiting for its object. These open
 * strand ends are UMBRAE. D131 measured garden paths on its toy world;
 * D134 measures the mechanism on the same world: how many umbrae are
 * open at once (WIDTH), how deep one is when it closes (DEPTH), and
 * whether a displaced word is misread because a nearer word competes
 * for it (COMPETITION) or because the arriving word cannot single out
 * its partner (RETRIEVAL).
 *
 *   Task 0  the world, copied from D131 (the toy grammar is not a
 *           general algorithm, so not a house library): its paradigms,
 *           levels, generator against analyser, tree table, word order,
 *           hand sentences and Part B measures must reproduce D131's
 *           pinned five-word totals exactly (oracle_partb.py's
 *           numbers, carried over). Then SIX words, counts only: the
 *           generator and the analyser timed and sized, pairs by length
 *           against D131 task 0's count, projective and crossed six-word
 *           pairs, and the oracle's deterministic six-word subset. No
 *           umbra, label or garden-path number at six words in task 0.
 *
 * The world (syntax only, no semantics): nouns puella (1st, f), dominus
 * (2nd, m), donum (2nd, n), rex (3rd, m); adjectives bonus (1st/2nd),
 * fortis (3rd); verbs amat / amant (subject + object), dat / dant
 * (subject + object + dative). Cases nom, gen, dat, acc, abl. Slots:
 * exactly one nominative subject agreeing in number with the verb,
 * exactly one accusative object, for dat / dant exactly one dative, any
 * number of ablative adjuncts; a genitive depends on another noun (at
 * most one per noun); an adjective on a noun of the same case, gender
 * and number. No pro-drop, vocative or locative. Up to six words.
 * Levels: L0 Latin (Bennett's paradigms, macrons dropped), L1 ideal
 * (every reading its own form), L2 case-stripped (one form per gender
 * and number), L3 bare (as L2, adjectives invariant).
 *
 * A PAIR = (string, one analysis of it): a head per position and a
 * reading per position. Each analysis yields one string at each level,
 * so the population of pairs is the same at every level.
 *
 * Sources: Charles E. Bennett, New Latin Grammar (1895/1908/1918, public
 * domain; Project Gutenberg #15665): sec. 20 porta (= puella), 23 hortus
 * and bellum (= dominus, donum), 32 dux (= rex, regis), 63 bonus, 69
 * fortis (first-listed forms; acc. pl. -is omitted), 101 amat / amant,
 * 127 dat / dant. Cross-check: Whitaker's WORDS, INFLECTS.LAT (vendored
 * at oratio/vocabularium/la/), entries AGE X, FREQ A, read as data.
 *
 * House library: dependentia (and its closure), hence latina.h: Roman
 * numerals and Latin keywords are macros here (never name a variable I,
 * V, X, L, C, D or M, or per, si, dum, casus, ...).
 * Build and run from the repo root:
 *   ./bin/aedilis knotapel/demo_134_umbra_width/main.c &&
 *   bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
 * Quick mode for plants: D134_CELER=1 (exhaustive passes up to four
 * words; no six-word part); D134_QUINQUE=1 runs every five-word part
 * and stops before six words (about a minute); D134_TEMPORA=1 prints
 * times.
 */





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static void
fatal (
    const char *msg)
{
    printf("  FATAL: %s\n", msg);
    exit(1);
}

static int quick = 0;

/* ================================================================
 * The lexicon (Bennett, macrons dropped)
 * ================================================================ */

enum { NOM, GEN, DAT, ACC, ABL, NCASES };
enum { MASC, FEM, NEUT, NGENDERS };
enum { SG, PL, NNUMBERS };

static const char *const case_name[NCASES] = {
    "nom", "gen", "dat", "acc", "abl"
};
static const char *const gender_name[NGENDERS] = { "m", "f", "n" };
static const char *const number_name[NNUMBERS] = { "sg", "pl" };

#define NNOUNS 4
#define NADJS  2
#define NVERBS 4

typedef struct {
    const char *lemma;
    int         gender;
    int         decl, var;          /* Whitaker's declension, variant */
    const char *stem1, *stem2;      /* Whitaker's stem keys 1, 2 */
    const char *form[NNUMBERS][NCASES];
} Noun;

static const Noun nouns[NNOUNS] = {
    { "puella", FEM, 1, 1, "puell", "puell",           /* Bennett 20 */
      { { "puella", "puellae", "puellae", "puellam", "puella" },
        { "puellae", "puellarum", "puellis", "puellas", "puellis" } } },
    { "dominus", MASC, 2, 1, "domin", "domin",         /* Bennett 23 */
      { { "dominus", "domini", "domino", "dominum", "domino" },
        { "domini", "dominorum", "dominis", "dominos", "dominis" } } },
    { "donum", NEUT, 2, 2, "don", "don",               /* Bennett 23 */
      { { "donum", "doni", "dono", "donum", "dono" },
        { "dona", "donorum", "donis", "dona", "donis" } } },
    { "rex", MASC, 3, 1, "rex", "reg",                 /* Bennett 32 */
      { { "rex", "regis", "regi", "regem", "rege" },
        { "reges", "regum", "regibus", "reges", "regibus" } } },
};

typedef struct {
    const char *lemma;
    int         decl, var;
    const char *stem1, *stem2;
    const char *form[NGENDERS][NNUMBERS][NCASES];
} Adj;

static const Adj adjs[NADJS] = {
    { "bonus", 1, 1, "bon", "bon",                     /* Bennett 63 */
      { { { "bonus", "boni", "bono", "bonum", "bono" },
          { "boni", "bonorum", "bonis", "bonos", "bonis" } },
        { { "bona", "bonae", "bonae", "bonam", "bona" },
          { "bonae", "bonarum", "bonis", "bonas", "bonis" } },
        { { "bonum", "boni", "bono", "bonum", "bono" },
          { "bona", "bonorum", "bonis", "bona", "bonis" } } } },
    { "fortis", 3, 2, "fort", "fort",                  /* Bennett 69 */
      { { { "fortis", "fortis", "forti", "fortem", "forti" },
          { "fortes", "fortium", "fortibus", "fortes", "fortibus" } },
        { { "fortis", "fortis", "forti", "fortem", "forti" },
          { "fortes", "fortium", "fortibus", "fortes", "fortibus" } },
        { { "forte", "fortis", "forti", "forte", "forti" },
          { "fortia", "fortium", "fortibus", "fortia", "fortibus" } } } },
};

typedef struct {
    const char *form;
    int         number;
    int         ditrans;
} Verb;

static const Verb verbs[NVERBS] = {              /* Bennett 101, 127 */
    { "amat", SG, 0 }, { "amant", PL, 0 }, { "dat", SG, 1 }, { "dant", PL, 1 }
};

/* ================================================================
 * Readings and the four levels
 * ================================================================ */

enum { K_NOUN, K_ADJ, K_VERB };

typedef struct {
    int kind, lemma, cas, gender, number;
} Reading;

#define MAXR  128
#define MAXF  128
#define MAXRF 32
#define NLEVELS 4

static Reading readings[MAXR];
static int     n_readings = 0;

static const char *const level_name[NLEVELS] = {
    "L0 Latin", "L1 ideal", "L2 case-stripped", "L3 bare"
};

static char form_text[NLEVELS][MAXF][40];
static int  n_forms[NLEVELS];
static int  form_of[NLEVELS][MAXR];
static int  form_rd[NLEVELS][MAXF][MAXRF];
static int  form_nr[NLEVELS][MAXF];

static int
rd_noun (
    int lemma,
    int cas,
    int number)
{
    return (lemma * NNUMBERS + number) * NCASES + cas;
}

static int
rd_adj (
    int lemma,
    int gender,
    int cas,
    int number)
{
    return NNOUNS * NNUMBERS * NCASES
        + ((lemma * NGENDERS + gender) * NNUMBERS + number) * NCASES + cas;
}

static int
rd_verb (
    int v)
{
    return NNOUNS * NNUMBERS * NCASES + NADJS * NGENDERS * NNUMBERS * NCASES
        + v;
}

/* the L0 form of a reading */
static const char *
form_l0 (
    int r)
{
    const Reading *x = &readings[r];

    if (x->kind == K_NOUN) return nouns[x->lemma].form[x->number][x->cas];
    if (x->kind == K_ADJ)
        return adjs[x->lemma].form[x->gender][x->number][x->cas];
    return verbs[x->lemma].form;
}

static int
intern (
    int         lv,
    const char *text)
{
    int f;

    for (f = 0; f < n_forms[lv]; f++)
        if (strcmp(form_text[lv][f], text) == 0) return f;
    if (n_forms[lv] == MAXF) fatal("too many forms");
    snprintf(form_text[lv][n_forms[lv]], sizeof form_text[lv][0], "%s", text);
    return n_forms[lv]++;
}

static void
build_world (void)
{
    int  l, g, n, c, v, lv, r;
    char buf[40];

    for (l = 0; l < NNOUNS; l++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                Reading *x = &readings[rd_noun(l, c, n)];
                x->kind = K_NOUN; x->lemma = l; x->cas = c;
                x->gender = nouns[l].gender; x->number = n;
            }
    for (l = 0; l < NADJS; l++)
        for (g = 0; g < NGENDERS; g++)
            for (n = 0; n < NNUMBERS; n++)
                for (c = 0; c < NCASES; c++) {
                    Reading *x = &readings[rd_adj(l, g, c, n)];
                    x->kind = K_ADJ; x->lemma = l; x->cas = c;
                    x->gender = g; x->number = n;
                }
    for (v = 0; v < NVERBS; v++) {
        Reading *x = &readings[rd_verb(v)];
        x->kind = K_VERB; x->lemma = v; x->cas = -1;
        x->gender = -1; x->number = verbs[v].number;
    }
    n_readings = rd_verb(NVERBS - 1) + 1;

    for (lv = 0; lv < NLEVELS; lv++) {
        n_forms[lv] = 0;
        for (r = 0; r < n_readings; r++) {
            const Reading *x = &readings[r];
            const char    *t = form_l0(r);

            if (x->kind != K_VERB) {
                if (lv == 1) {
                    snprintf(buf, sizeof buf, "%s.%s.%s.%s",
                        x->kind == K_NOUN ? nouns[x->lemma].lemma
                                          : adjs[x->lemma].lemma,
                        case_name[x->cas], gender_name[x->gender],
                        number_name[x->number]);
                    t = buf;
                } else if (lv >= 2 && x->kind == K_NOUN) {
                    t = nouns[x->lemma].form[x->number][NOM];
                } else if (lv == 2) {
                    t = adjs[x->lemma].form[x->gender][x->number][NOM];
                } else if (lv == 3) {
                    t = adjs[x->lemma].form[MASC][SG][NOM];
                }
            }
            form_of[lv][r] = intern(lv, t);
        }
        for (r = 0; r < n_forms[lv]; r++) form_nr[lv][r] = 0;
        for (r = 0; r < n_readings; r++) {
            int f = form_of[lv][r];
            if (form_nr[lv][f] == MAXRF) fatal("too many readings per form");
            form_rd[lv][f][form_nr[lv][f]++] = r;
        }
    }
}

static int
form_kind (
    int lv,
    int f)
{
    return readings[form_rd[lv][f][0]].kind;
}

/* ================================================================
 * Whitaker's INFLECTS.LAT (data), for the cross-check
 * ================================================================ */

typedef struct {
    int  adj;                 /* 0 = N, 1 = ADJ */
    int  decl, var, cas, number;
    char gender;              /* 'M' 'F' 'N' 'C' (m or f) 'X' (any) */
    int  pos;                 /* ADJ: degree POS */
    int  key;
    char ending[12];
    int  standard;            /* AGE X and FREQ A */
} WEntry;

#define MAXW_ENTRIES 4096
static WEntry went[MAXW_ENTRIES];
static int    n_went = 0;

static int
w_case (
    const char *s)
{
    if (strcmp(s, "NOM") == 0) return NOM;
    if (strcmp(s, "GEN") == 0) return GEN;
    if (strcmp(s, "DAT") == 0) return DAT;
    if (strcmp(s, "ACC") == 0) return ACC;
    if (strcmp(s, "ABL") == 0) return ABL;
    return -1;
}

static int
read_whitaker (
    const char *path)
{
    Piscina *p = piscina_generare_dynamicum("d134_whitaker", 1 << 22);
    chorda   src = filum_legere_totum(path, p);
    i32      at = 0;

    if (src.mensura == 0) {
        piscina_destruere(p);
        return 0;
    }
    while (at < src.mensura) {
        char  line[256];
        char *tok[16];
        int   nt = 0, len = 0;
        char *s, *cm;

        while (at < src.mensura && src.datum[at] != '\n') {
            if (len < 255) line[len++] = (char)src.datum[at];
            at++;
        }
        at++;
        line[len] = '\0';
        cm = strstr(line, "--");
        if (cm != NULL) *cm = '\0';
        for (s = strtok(line, " \t\r"); s != NULL && nt < 16;
             s = strtok(NULL, " \t\r"))
            tok[nt++] = s;
        if (nt < 9) continue;
        {
            WEntry e;
            int    adj = strcmp(tok[0], "ADJ") == 0;
            int    b = adj ? 7 : 6;   /* index of the stem key */
            int    elen;

            if (!adj && strcmp(tok[0], "N") != 0) continue;
            memset(&e, 0, sizeof e);
            e.adj = adj;
            e.decl = atoi(tok[1]);
            e.var = atoi(tok[2]);
            e.cas = w_case(tok[3]);
            if (e.cas < 0) continue;  /* VOC, LOC: outside the world */
            e.number = strcmp(tok[4], "S") == 0 ? SG : PL;
            e.gender = tok[5][0];
            e.pos = adj ? strcmp(tok[6], "POS") == 0 : 1;
            if (nt < b + 4) continue;
            e.key = atoi(tok[b]);
            elen = atoi(tok[b + 1]);
            {
                /* [ending] AGE FREQ */
                int age = elen > 0 ? b + 3 : b + 2;

                if (nt < age + 2) continue;
                if (elen > 0)
                    snprintf(e.ending, sizeof e.ending, "%s", tok[b + 2]);
                e.standard = strcmp(tok[age], "X") == 0
                    && strcmp(tok[age + 1], "A") == 0;
            }
            if (n_went == MAXW_ENTRIES) fatal("too many Whitaker entries");
            went[n_went++] = e;
        }
    }
    piscina_destruere(p);
    return 1;
}

static int
w_gender_ok (
    char eg,
    int  g)
{
    if (eg == 'X') return 1;
    if (eg == 'C') return g == MASC || g == FEM;
    return (eg == 'M' && g == MASC) || (eg == 'F' && g == FEM)
        || (eg == 'N' && g == NEUT);
}

/* Whitaker's standard forms for one cell: into out[], count returned */
static int
whitaker_forms (
    int          adj,
    int          decl,
    int          var,
    int          gender,
    const char  *stem1,
    const char  *stem2,
    int          cas,
    int          number,
    char         out[][40],
    int          cap)
{
    int k, j, n = 0;

    for (k = 0; k < n_went; k++) {
        const WEntry *e = &went[k];
        char          f[40];
        int           dup = 0;

        if (e->adj != adj || e->decl != decl) continue;
        if (e->var != 0 && e->var != var) continue;
        if (e->cas != cas || e->number != number) continue;
        if (!w_gender_ok(e->gender, gender) || !e->pos || !e->standard)
            continue;
        snprintf(f, sizeof f, "%s%s", e->key == 1 ? stem1 : stem2, e->ending);
        for (j = 0; j < n; j++)
            if (strcmp(out[j], f) == 0) dup = 1;
        if (!dup && n < cap) snprintf(out[n++], 40, "%s", f);
    }
    return n;
}

/* ================================================================
 * Analyses
 * ================================================================ */

#define MAXW 6
#define MAXA 4096

enum { R_ROOT, R_NSUBJ, R_OBJ, R_IOBJ, R_OBL, R_NMOD, R_AMOD };

typedef struct {
    int head[MAXW];           /* 1-based position, 0 = root */
    int rd[MAXW];
} Analysis;

static i64
analysis_key (
    const Analysis *a,
    int             n)
{
    i64 k = (i64)n;
    int i;

    for (i = 0; i < n; i++)
        k |= ((i64)a->head[i] | ((i64)a->rd[i] << 3)) << (3 + 10 * i);
    return k;
}

static i64
bag_code (
    const int *form,
    int        n)
{
    i64 k = (i64)n;
    int i;

    for (i = 0; i < n; i++) k |= (i64)form[i] << (3 + 7 * i);
    return k;
}

/* --- the analyser: constraints, order-free by intent --- */

typedef struct {
    int head, rel, rd;
} Opt;

static int        an_lv, an_n, an_vpos, an_ditrans, an_vnum;
static const int *an_form;
static int        an_nrd[MAXW];           /* chosen noun readings */
static Opt        an_opt[MAXW][MAXW * 2 + 2];
static int        an_nopt[MAXW];
static int        an_pick[MAXW];
static Analysis  *an_out;
static int        an_count, an_cap;

static int
an_tree_ok (void)
{
    int i, j, steps;

    for (i = 0; i < an_n; i++) {
        j = i;
        steps = 0;
        while (an_opt[j][an_pick[j]].head != 0) {
            j = an_opt[j][an_pick[j]].head - 1;
            if (++steps > an_n) return 0;
        }
    }
    return 1;
}

static void
an_assign (
    int i,
    int nsubj,
    int obj,
    int iobj,
    int genmask)
{
    int k;

    if (i == an_n) {
        if (nsubj != 1 || obj != 1 || iobj != an_ditrans) return;
        if (!an_tree_ok()) return;
        if (an_count == an_cap) fatal("too many analyses for one string");
        for (k = 0; k < an_n; k++) {
            an_out[an_count].head[k] = an_opt[k][an_pick[k]].head;
            an_out[an_count].rd[k] = an_opt[k][an_pick[k]].rd;
        }
        an_count++;
        return;
    }
    for (k = 0; k < an_nopt[i]; k++) {
        const Opt *o = &an_opt[i][k];
        int        gm = genmask;

        if (o->rel == R_NSUBJ && nsubj == 1) continue;
        if (o->rel == R_OBJ && obj == 1) continue;
        if (o->rel == R_IOBJ && iobj == an_ditrans) continue;
        if (o->rel == R_NMOD) {
            if (genmask & (1 << (o->head - 1))) continue;
            gm |= 1 << (o->head - 1);
        }
        an_pick[i] = k;
        an_assign(i + 1, nsubj + (o->rel == R_NSUBJ), obj + (o->rel == R_OBJ),
            iobj + (o->rel == R_IOBJ), gm);
    }
}

static void
an_options (void)
{
    int i, j, k;

    for (i = 0; i < an_n; i++) {
        int f = an_form[i];
        int kind = form_kind(an_lv, f);

        an_nopt[i] = 0;
        if (kind == K_VERB) {
            Opt o = { 0, R_ROOT, 0 };
            o.rd = form_rd[an_lv][f][0];
            an_opt[i][an_nopt[i]++] = o;
        } else if (kind == K_NOUN) {
            const Reading *x = &readings[an_nrd[i]];
            Opt            o;

            o.rd = an_nrd[i];
            o.head = an_vpos + 1;
            if (x->cas == NOM && x->number == an_vnum) {
                o.rel = R_NSUBJ; an_opt[i][an_nopt[i]++] = o;
            }
            if (x->cas == ACC) { o.rel = R_OBJ; an_opt[i][an_nopt[i]++] = o; }
            if (x->cas == DAT && an_ditrans) {
                o.rel = R_IOBJ; an_opt[i][an_nopt[i]++] = o;
            }
            if (x->cas == ABL) { o.rel = R_OBL; an_opt[i][an_nopt[i]++] = o; }
            if (x->cas == GEN)
                for (j = 0; j < an_n; j++) {
                    if (j == i || form_kind(an_lv, an_form[j]) != K_NOUN)
                        continue;
                    o.head = j + 1; o.rel = R_NMOD;
                    an_opt[i][an_nopt[i]++] = o;
                }
        } else {
            for (j = 0; j < an_n; j++) {
                const Reading *y;

                if (j == i || form_kind(an_lv, an_form[j]) != K_NOUN) continue;
                y = &readings[an_nrd[j]];
                for (k = 0; k < form_nr[an_lv][f]; k++) {
                    const Reading *x = &readings[form_rd[an_lv][f][k]];
                    if (x->cas == y->cas && x->gender == y->gender
                        && x->number == y->number) {
                        Opt o;
                        o.head = j + 1; o.rel = R_AMOD;
                        o.rd = form_rd[an_lv][f][k];
                        an_opt[i][an_nopt[i]++] = o;
                    }
                }
            }
        }
        if (an_nopt[i] == 0) return;
    }
    an_assign(0, 0, 0, 0, 0);
}

static void
an_nouns (
    int i)
{
    int k;

    if (i == an_n) {
        an_options();
        return;
    }
    if (form_kind(an_lv, an_form[i]) != K_NOUN) {
        an_nouns(i + 1);
        return;
    }
    for (k = 0; k < form_nr[an_lv][an_form[i]]; k++) {
        an_nrd[i] = form_rd[an_lv][an_form[i]][k];
        an_nouns(i + 1);
    }
}

/* all analyses of the string form[0..n-1] at level lv */
static int
analyse (
    int        lv,
    const int *form,
    int        n,
    Analysis  *out,
    int        cap)
{
    int i, nv = 0;

    an_lv = lv; an_form = form; an_n = n;
    an_out = out; an_count = 0; an_cap = cap;
    for (i = 0; i < n; i++)
        if (form_kind(lv, form[i]) == K_VERB) {
            nv++;
            an_vpos = i;
        }
    if (nv != 1) return 0;
    an_vnum = readings[form_rd[lv][form[an_vpos]][0]].number;
    an_ditrans = verbs[readings[form_rd[lv][form[an_vpos]][0]].lemma].ditrans;
    an_nouns(0);
    return an_count;
}

/* --- the generator: structures built slot by slot --- */

typedef struct {
    int rd, head;             /* head = token index + 1, 0 = root */
} Tok;

enum { T_NP, T_ADJS, T_GEN, T_ABL };

typedef struct {
    int type, cas, head, numfix, minlem;
} Task;

#define MAXTASKS 32

static Tok gtok[MAXW];
static int gn = 0;
static int gmaxw = MAXW;
static int perms[MAXW + 1][720][MAXW];
static int nperms[MAXW + 1];

typedef struct {
    i64 bag, key;
} Entry;

typedef struct {
    Entry *e;
    long   n, cap;
} Entries;

static Entries gen_entries[NLEVELS];
static long    gen_emitted = 0;

static void
push_entry (
    Entries *v,
    i64      bag,
    i64      key)
{
    if (v->n == v->cap) {
        v->cap = v->cap ? v->cap * 2 : 1 << 16;
        v->e = (Entry *)realloc(v->e, (size_t)v->cap * sizeof(Entry));
        if (v->e == NULL) fatal("memory");
    }
    v->e[v->n].bag = bag;
    v->e[v->n].key = key;
    v->n++;
}

static int
entry_cmp (
    const void *a,
    const void *b)
{
    const Entry *x = (const Entry *)a, *y = (const Entry *)b;

    if (x->bag != y->bag) return x->bag < y->bag ? -1 : 1;
    if (x->key != y->key) return x->key < y->key ? -1 : 1;
    return 0;
}

static long
sort_unique (
    Entries *v)
{
    long i, m = 0, dups = 0;

    if (v->n == 0) return 0;
    qsort(v->e, (size_t)v->n, sizeof(Entry), entry_cmp);
    for (i = 1; i < v->n; i++) {
        if (entry_cmp(&v->e[i], &v->e[m]) == 0) {
            dups++;
            continue;
        }
        v->e[++m] = v->e[i];
    }
    v->n = m + 1;
    return dups;
}

static int
i64_cmp (
    const void *a,
    const void *b)
{
    i64 x = *(const i64 *)a, y = *(const i64 *)b;

    return x < y ? -1 : x > y ? 1 : 0;
}

static int
int_cmp (
    const void *a,
    const void *b)
{
    return *(const int *)a - *(const int *)b;
}

/* every placement of the tokens on the canonical (sorted) positions */
static void
gen_emit (void)
{
    int lv, p, i;

    gen_emitted++;
    for (lv = 0; lv < NLEVELS; lv++) {
        int form[MAXW], bag[MAXW];

        for (i = 0; i < gn; i++) bag[i] = form[i] = form_of[lv][gtok[i].rd];
        qsort(bag, (size_t)gn, sizeof(int), int_cmp);
        for (p = 0; p < nperms[gn]; p++) {
            const int *pos = perms[gn][p];   /* token i -> position pos[i] */
            Analysis   a;
            int        ok = 1;

            for (i = 0; i < gn && ok; i++)
                if (form[i] != bag[pos[i]]) ok = 0;
            if (!ok) continue;
            for (i = 0; i < gn; i++) {
                a.head[pos[i]] = gtok[i].head == 0 ? 0
                    : pos[gtok[i].head - 1] + 1;
                a.rd[pos[i]] = gtok[i].rd;
            }
            push_entry(&gen_entries[lv], bag_code(bag, gn), analysis_key(&a, gn));
        }
    }
}

static void
gen_run (
    const Task *stack,
    int         top)
{
    Task st[MAXTASKS];
    Task t;
    int  l, n, lo, hi;

    if (top == 0) {
        gen_emit();
        return;
    }
    memcpy(st, stack, (size_t)top * sizeof(Task));
    t = st[--top];
    if (top + 3 > MAXTASKS) fatal("task stack");
    switch (t.type) {
    case T_NP:
        if (gn == gmaxw) return;
        lo = t.numfix >= 0 ? t.numfix : 0;
        hi = t.numfix >= 0 ? t.numfix : NNUMBERS - 1;
        for (l = 0; l < NNOUNS; l++)
            for (n = lo; n <= hi; n++) {
                int ni = gn;

                gtok[gn].rd = rd_noun(l, t.cas, n);
                gtok[gn].head = t.head;
                gn++;
                st[top].type = T_GEN; st[top].head = ni;
                st[top + 1].type = T_ADJS; st[top + 1].head = ni;
                st[top + 1].minlem = 0;
                gen_run(st, top + 2);
                gn--;
            }
        return;
    case T_ADJS:
        gen_run(st, top);
        if (gn == gmaxw) return;
        for (l = t.minlem; l < NADJS; l++) {
            const Reading *x = &readings[gtok[t.head].rd];

            gtok[gn].rd = rd_adj(l, x->gender, x->cas, x->number);
            gtok[gn].head = t.head + 1;
            gn++;
            st[top].type = T_ADJS; st[top].head = t.head; st[top].minlem = l;
            gen_run(st, top + 1);
            gn--;
        }
        return;
    case T_GEN:
        gen_run(st, top);
        st[top].type = T_NP; st[top].cas = GEN; st[top].head = t.head + 1;
        st[top].numfix = -1;
        gen_run(st, top + 1);
        return;
    case T_ABL:
        gen_run(st, top);
        st[top].type = T_ABL;
        st[top + 1].type = T_NP; st[top + 1].cas = ABL; st[top + 1].head = 1;
        st[top + 1].numfix = -1;
        gen_run(st, top + 2);
        return;
    default:
        fatal("task");
    }
}

static void
generate (void)
{
    int  v, top;
    Task st[MAXTASKS];

    for (v = 0; v < NVERBS; v++) {
        gn = 1;
        gtok[0].rd = rd_verb(v);
        gtok[0].head = 0;
        top = 0;
        st[top].type = T_ABL; top++;
        if (verbs[v].ditrans) {
            st[top].type = T_NP; st[top].cas = DAT; st[top].head = 1;
            st[top].numfix = -1; top++;
        }
        st[top].type = T_NP; st[top].cas = ACC; st[top].head = 1;
        st[top].numfix = -1; top++;
        st[top].type = T_NP; st[top].cas = NOM; st[top].head = 1;
        st[top].numfix = verbs[v].number; top++;
        gen_run(st, top);
    }
}

static void
build_perms (void)
{
    int n, p, i, j;

    for (n = 0; n <= MAXW; n++) {
        int a[MAXW];

        for (i = 0; i < n; i++) a[i] = i;
        nperms[n] = 0;
        for (;;) {
            memcpy(perms[n][nperms[n]++], a, sizeof a);
            /* next permutation (lexicographic) */
            for (i = n - 2; i >= 0 && a[i] > a[i + 1]; i--) {}
            if (i < 0) break;
            for (j = n - 1; a[j] < a[i]; j--) {}
            p = a[i]; a[i] = a[j]; a[j] = p;
            for (i++, j = n - 1; i < j; i++, j--) {
                p = a[i]; a[i] = a[j]; a[j] = p;
            }
        }
    }
}

/* ================================================================
 * The tree table: every rooted tree on n <= 6 positions, measured once
 * ================================================================ */

static int tree_index (const int *head, int n)
{
    int k, c = 0, b = 1;

    for (k = 0; k < n; k++) {
        c += head[k] * b;
        b *= MAXW + 1;
    }
    return c;
}

#define TCODES 117649          /* 7^6 */
static unsigned char t_valid[MAXW + 1][TCODES];
static unsigned char t_proj[MAXW + 1][TCODES];
static unsigned char t_proj2[MAXW + 1][TCODES];
static unsigned char t_mild[MAXW + 1][TCODES];
static unsigned char t_npmask[MAXW + 1][TCODES];
static unsigned char t_cross[MAXW + 1][TCODES];   /* crossings, root arc
                                                     included */

static void
part_trees (void)
{
    Piscina *scratch = piscina_generare_dynamicum("d134_tree", 1 << 16);
    int      n, k, code, ncodes;
    int      cayley_ok = 1, routes_ok = 1, mild_ok = 1, rev_ok = 1;
    int      counts_ok = 1;
    long     ntrees = 0;
    static const long proj_count[MAXW + 1] = { 0, 1, 2, 7, 30, 143, 728 };
    static const long mild_count[MAXW + 1] = { 0, 1, 2, 9, 64, 569, 5652 };
    char     msg[256];

    printf("\n--- the tree table ---\n");
    for (n = 1; n <= MAXW; n++) {
        long expect = 1, count = 0, nproj = 0, nmild = 0;

        ncodes = 1;
        for (k = 0; k < n; k++) ncodes *= n + 1;
        for (k = 0; k < n - 1; k++) expect *= n;
        for (code = 0; code < ncodes; code++) {
            int               head[MAXW], c = code, self = 0;
            i32               cap[MAXW];
            DependentiaArbor  t;
            DependentiaCruces x;
            DependentiaMensura m;
            DependentiaArcus  arcs[MAXW];

            for (k = 0; k < n; k++) {
                head[k] = c % (n + 1);
                c /= n + 1;
                if (head[k] == k + 1) self = 1;
                cap[k] = (i32)head[k];
            }
            if (self) continue;
            t.numerus = (i32)n;
            t.capita = cap;
            if (dependentia_validare(&t) != DEPENDENTIA_SANA) continue;
            count++;
            piscina_vacare(scratch);
            if (!dependentia_cruces(scratch, &t, &x)
                || !dependentia_mensurare(scratch, &t, NULL, &m)
                || !dependentia_arcus(scratch, &t, arcs))
                fatal("dependentia");
            {
                int ix = tree_index(head, n), mask = 0;

                t_valid[n][ix] = 1;
                t_proj[n][ix] = (unsigned char)(x.proiectiva != 0);
                t_proj2[n][ix] = (unsigned char)(m.proiectiva != 0);
                t_mild[n][ix] = (unsigned char)(m.gradus_lacunarum <= 1
                    && m.bene_nidificata);
                for (k = 0; k < n; k++)
                    if (arcs[k].non_proiectivus) mask |= 1 << k;
                t_npmask[n][ix] = (unsigned char)mask;
                t_cross[n][ix] = (unsigned char)(x.cruces + x.cruces_radicis);
                nproj += t_proj[n][ix];
                nmild += t_mild[n][ix];
                if (t_proj[n][ix] != t_proj2[n][ix]) routes_ok = 0;
                if (t_proj[n][ix] && !t_mild[n][ix]) mild_ok = 0;
                if (t_proj[n][ix] != (mask == 0)) routes_ok = 0;
            }
        }
        if (count != expect) cayley_ok = 0;
        if (nproj != proj_count[n] || nmild != mild_count[n]) counts_ok = 0;
        ntrees += count;
    }
    /* reversal: position k -> n + 1 - k */
    for (n = 1; n <= MAXW; n++) {
        ncodes = 1;
        for (k = 0; k < n; k++) ncodes *= n + 1;
        for (code = 0; code < ncodes; code++) {
            int head[MAXW], rh[MAXW], c = code, ix, rx, mask, rmask = 0;

            for (k = 0; k < n; k++) {
                head[k] = c % (n + 1);
                c /= n + 1;
            }
            ix = tree_index(head, n);
            if (!t_valid[n][ix]) continue;
            for (k = 0; k < n; k++)
                rh[n - 1 - k] = head[k] == 0 ? 0 : n + 1 - head[k];
            rx = tree_index(rh, n);
            mask = t_npmask[n][ix];
            for (k = 0; k < n; k++)
                if (mask & (1 << k)) rmask |= 1 << (n - 1 - k);
            if (!t_valid[n][rx] || t_proj[n][rx] != t_proj[n][ix]
                || t_mild[n][rx] != t_mild[n][ix] || t_npmask[n][rx] != rmask)
                rev_ok = 0;
        }
    }
    snprintf(msg, sizeof msg,
        "trees on n <= 6 positions: n^(n-1) for every n (Cayley), %ld in all",
        ntrees);
    check(msg, cayley_ok);
    check("projective by crossings (root arc) == by interval yields, and == "
        "no non-projective arc, on every tree", routes_ok);
    check("projective => mild on every tree", mild_ok);
    check("projective trees per n = 1, 2, 7, 30, 143, 728 and mild trees = "
        "1, 2, 9, 64, 569, 5652 (D131's oracle_trees.py run to n = 6, "
        "independent Python; a pin)", counts_ok);
    check("reversal keeps projective and mild and mirrors the non-projective "
        "arcs, on every tree", rev_ok);
    piscina_destruere(scratch);
}

/* ================================================================
 * Part A
 * ================================================================ */

static void
part_paradigms (void)
{
    const char *path = "oratio/vocabularium/la/INFLECTS.LAT";
    static const struct { int gender, cas, number; const char *form; }
        extras[8] = {
        { MASC, ABL, SG, "forte" }, { FEM, ABL, SG, "forte" },
        { NEUT, ABL, SG, "forte" }, { MASC, GEN, PL, "fortum" },
        { FEM, GEN, PL, "fortum" }, { NEUT, GEN, PL, "fortum" },
        { MASC, ACC, PL, "fortis" }, { FEM, ACC, PL, "fortis" }
    };
    char out[16][40];
    int  l, g, n, c, k, j, equal = 0, cells = 0, member = 0, fcells = 0;
    int  extra_found = 0, extra_listed = 1, lv, single = 1, derived = 1;
    int  onekind = 1;
    char msg[256];

    printf("\n--- paradigms and levels ---\n");
    if (!read_whitaker(path)) fatal("cannot read INFLECTS.LAT");
    for (l = 0; l < NNOUNS; l++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Noun *x = &nouns[l];
                int m = whitaker_forms(0, x->decl, x->var, x->gender,
                    x->stem1, x->stem2, c, n, out, 16);
                cells++;
                if (m == 1 && strcmp(out[0], x->form[n][c]) == 0) equal++;
            }
    for (g = 0; g < NGENDERS; g++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Adj *x = &adjs[0];
                int m = whitaker_forms(1, x->decl, x->var, g,
                    x->stem1, x->stem2, c, n, out, 16);
                cells++;
                if (m == 1 && strcmp(out[0], x->form[g][n][c]) == 0) equal++;
            }
    snprintf(msg, sizeof msg,
        "nouns and bonus: the table (Bennett's, cited) == Whitaker (AGE X, "
        "FREQ A) in %d of %d cells", equal, cells);
    check(msg, equal == cells && cells == 70);
    for (g = 0; g < NGENDERS; g++)
        for (n = 0; n < NNUMBERS; n++)
            for (c = 0; c < NCASES; c++) {
                const Adj *x = &adjs[1];
                int m = whitaker_forms(1, x->decl, x->var, g,
                    x->stem1, x->stem2, c, n, out, 16);
                int in = 0;

                fcells++;
                for (k = 0; k < m; k++) {
                    if (strcmp(out[k], x->form[g][n][c]) == 0) {
                        in = 1;
                        continue;
                    }
                    extra_found++;
                    {
                        int listed = 0;
                        for (j = 0; j < 8; j++)
                            if (extras[j].gender == g && extras[j].cas == c
                                && extras[j].number == n
                                && strcmp(extras[j].form, out[k]) == 0)
                                listed = 1;
                        if (!listed) extra_listed = 0;
                    }
                }
                member += in;
            }
    snprintf(msg, sizeof msg,
        "fortis: Bennett's form among Whitaker's in %d of %d cells; Whitaker's "
        "%d other forms are exactly the 8 listed (abl -e, gen pl -um, acc pl "
        "-is)", member, fcells, extra_found);
    check(msg, member == fcells && fcells == 30 && extra_found == 8
        && extra_listed);

    for (lv = 0; lv < NLEVELS; lv++) {
        int f, nominal = 0;

        for (f = 0; f < n_forms[lv]; f++) {
            int kind = form_kind(lv, f), r;

            if (kind != K_VERB) nominal++;
            for (r = 0; r < form_nr[lv][f]; r++)
                if (readings[form_rd[lv][f][r]].kind != kind) onekind = 0;
            if (lv == 1 && form_nr[lv][f] != 1) single = 0;
            if (lv >= 2) {
                int r0, found = 0;
                for (r0 = 0; r0 < n_readings; r0++)
                    if (strcmp(form_l0(r0), form_text[lv][f]) == 0) found = 1;
                if (!found) derived = 0;
            }
        }
        printf("  %-17s %3d nominal forms + %d verbs\n", level_name[lv],
            nominal, NVERBS);
        snprintf(msg, sizeof msg, "%s: %d nominal forms (L0 47, L1 100, L2 17, "
            "L3 10; counted by hand from the tables)", level_name[lv], nominal);
        check(msg, nominal == (lv == 0 ? 47 : lv == 1 ? 100 : lv == 2 ? 17 : 10)
            && n_forms[lv] == nominal + NVERBS);
    }
    check("every form's readings are of one kind (noun, adjective or verb)",
        onekind);
    check("L1: every form has exactly one reading", single);
    check("L2, L3: every form is an L0 form (derived, never hand-written)",
        derived);
}

/* the analyser's set for every bag, kept for the later parts */
static Entries an_entries[NLEVELS];

static long
multinomial (
    const int *bag,
    int        n)
{
    long m = 1;
    int  i, run = 1, k;

    for (k = 2; k <= n; k++) m *= k;
    for (i = 1; i <= n; i++) {
        if (i < n && bag[i] == bag[i - 1]) {
            run++;
            continue;
        }
        for (k = 2; k <= run; k++) m /= k;
        run = 1;
    }
    return m;
}

static void
decode_bag (
    i64  code,
    int *bag,
    int *n)
{
    int i;

    *n = (int)(code & 7);
    for (i = 0; i < *n; i++) bag[i] = (int)((code >> (3 + 7 * i)) & 127);
}

static void
decode_key (
    i64       key,
    Analysis *a,
    int       n)
{
    int i;

    for (i = 0; i < n; i++) {
        i64 w = (key >> (3 + 10 * i)) & 1023;
        a->head[i] = (int)(w & 7);
        a->rd[i] = (int)(w >> 3);
    }
}

static Analysis abuf[MAXA];

/* every multiset of nominal forms with one verb: the analyser's sets */
static void
analyse_all_bags (
    int  lv,
    long *bags_tried)
{
    int nominal[MAXF], nn = 0, f, v, m;
    int idx[MAXW];

    for (f = 0; f < n_forms[lv]; f++)
        if (form_kind(lv, f) != K_VERB) nominal[nn++] = f;
    for (v = 0; v < n_forms[lv]; v++) {
        if (form_kind(lv, v) != K_VERB) continue;
        for (m = 0; m < gmaxw; m++) {      /* m nominal words + the verb */
            int i, done = 0;

            for (i = 0; i < m; i++) idx[i] = 0;
            while (!done) {
                int bag[MAXW], n = m + 1, a, na;

                for (i = 0; i < m; i++) bag[i] = nominal[idx[i]];
                bag[m] = v;
                qsort(bag, (size_t)n, sizeof(int), int_cmp);
                (*bags_tried)++;
                na = analyse(lv, bag, n, abuf, MAXA);
                for (a = 0; a < na; a++)
                    push_entry(&an_entries[lv], bag_code(bag, n),
                        analysis_key(&abuf[a], n));
                /* next multiset: non-decreasing idx */
                for (i = m - 1; i >= 0 && idx[i] == nn - 1; i--) {}
                if (i < 0) {
                    done = 1;
                } else {
                    int val = ++idx[i];
                    for (i++; i < m; i++) idx[i] = val;
                }
                if (m == 0) done = 1;
            }
        }
    }
}

static long level_pairs[NLEVELS][MAXW + 1];
static long level_strings[NLEVELS];
static long level_bags[NLEVELS];
static int  level_maxf[NLEVELS];

/* pairs by length, strings, bags and the largest |F| per level, from
 * the analyser's sets (pairs = distinct orders x |F| per bag) */
static void
sizes (void)
{
    int lv, n;

    for (lv = 0; lv < NLEVELS; lv++) {
        long i = 0;

        for (n = 0; n <= MAXW; n++) level_pairs[lv][n] = 0;
        level_strings[lv] = level_bags[lv] = 0;
        level_maxf[lv] = 0;
        while (i < an_entries[lv].n) {
            long j = i;
            int  bag[MAXW], nb;

            while (j < an_entries[lv].n && an_entries[lv].e[j].bag
                == an_entries[lv].e[i].bag)
                j++;
            decode_bag(an_entries[lv].e[i].bag, bag, &nb);
            level_pairs[lv][nb] += multinomial(bag, nb) * (j - i);
            level_strings[lv] += multinomial(bag, nb);
            level_bags[lv]++;
            if (j - i > level_maxf[lv]) level_maxf[lv] = (int)(j - i);
            i = j;
        }
    }
}

static void
part_world (void)
{
    int  lv, n;
    long gdups[NLEVELS], adups[NLEVELS], tried[NLEVELS];
    char msg[256];

    printf("\n--- generator and analyser ---\n");
    generate();
    printf("  generator: %ld abstract analyses (with repeats) up to %d words\n",
        gen_emitted, gmaxw);
    for (lv = 0; lv < NLEVELS; lv++) {
        long i;
        int  same;

        gdups[lv] = sort_unique(&gen_entries[lv]);
        tried[lv] = 0;
        analyse_all_bags(lv, &tried[lv]);
        adups[lv] = sort_unique(&an_entries[lv]);
        same = gen_entries[lv].n == an_entries[lv].n;
        for (i = 0; same && i < gen_entries[lv].n; i++)
            if (entry_cmp(&gen_entries[lv].e[i], &an_entries[lv].e[i]) != 0)
                same = 0;
        snprintf(msg, sizeof msg, "%s: generated == analysed, %ld positioned "
            "analyses on canonical strings (%ld bags tried), readings included",
            level_name[lv], an_entries[lv].n, tried[lv]);
        check(msg, same);
        snprintf(msg, sizeof msg, "%s: the analyser never yields an analysis "
            "twice", level_name[lv]);
        check(msg, adups[lv] == 0);
    }

    sizes();
    printf("\n  %-17s %8s %10s %8s %8s %10s %6s\n", "level", "bags",
        "strings", "pairs 3", "pairs 4", "pairs 5", "max|F|");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %8ld %10ld %8ld %8ld %10ld %6d\n", level_name[lv],
            level_bags[lv], level_strings[lv], level_pairs[lv][3],
            level_pairs[lv][4], level_pairs[lv][5], level_maxf[lv]);
    {
        int same = 1;

        for (lv = 1; lv < NLEVELS; lv++)
            for (n = 0; n <= MAXW; n++)
                if (level_pairs[lv][n] != level_pairs[0][n]) same = 0;
        check("the population of pairs is the same at every level, length by "
            "length", same);
    }
    check("pairs by length = oracle_partb.py's (3: 384, 4: 55,296, 5: "
        "6,696,960; = task 0's count; a pin)", level_pairs[0][1] == 0
        && level_pairs[0][2] == 0 && level_pairs[0][6] == 0
        && level_pairs[0][3] == 384 && level_pairs[0][4] == 55296
        && (quick || level_pairs[0][5] == 6696960));
}

/* sig (position q holds canonical token sig[q]) is the first order of
 * its string, in the lexicographic order of perms, iff equal tokens keep
 * their canonical order; D131 found the same orders by a list of the
 * strings seen, which costs n!^2 per bag at six words */
static int
first_order (
    const int *bag,
    const int *sig,
    int        n)
{
    int q, r;

    for (q = 0; q < n; q++)
        for (r = q + 1; r < n; r++)
            if (bag[sig[q]] == bag[sig[r]] && sig[q] > sig[r]) return 0;
    return 1;
}

/* word order cannot change F(s): every distinct order of every bag */
static void
part_order (void)
{
    int  lv;
    long order_bad = 0;
    char msg[256];

    printf("\n--- word order and F(s) ---\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        long i = 0, strings = 0, bad = 0;

        while (i < an_entries[lv].n) {
            long     j = i, s;
            int      bag[MAXW], nb, p, k;
            i64      seen[720];
            int      nseen = 0;
            Analysis canon[MAXA];
            int      nc;

            while (j < an_entries[lv].n && an_entries[lv].e[j].bag
                == an_entries[lv].e[i].bag)
                j++;
            decode_bag(an_entries[lv].e[i].bag, bag, &nb);
            nc = (int)(j - i);
            for (s = i; s < j; s++) decode_key(an_entries[lv].e[s].key,
                &canon[s - i], nb);
            for (p = 0; p < nperms[nb]; p++) {
                const int *sig = perms[nb][p];   /* new position q holds
                                                    canonical sig[q] */
                int        str[MAXW], inv[MAXW], dup = 0, na, a;
                i64        code, got[MAXA], want[MAXA];

                for (k = 0; k < nb; k++) {
                    str[k] = bag[sig[k]];
                    inv[sig[k]] = k;
                }
                code = bag_code(str, nb);
                for (k = 0; k < nseen; k++)
                    if (seen[k] == code) dup = 1;
                if (first_order(bag, sig, nb) != !dup) order_bad++;
                if (dup) continue;
                seen[nseen++] = code;
                strings++;
                na = analyse(lv, str, nb, abuf, MAXA);
                for (a = 0; a < na; a++) got[a] = analysis_key(&abuf[a], nb);
                for (a = 0; a < nc; a++) {
                    Analysis m;
                    for (k = 0; k < nb; k++) {
                        int h = canon[a].head[k];
                        m.head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                        m.rd[inv[k]] = canon[a].rd[k];
                    }
                    want[a] = analysis_key(&m, nb);
                }
                if (na != nc) {
                    bad++;
                    continue;
                }
                qsort(got, (size_t)na, sizeof(i64), i64_cmp);
                qsort(want, (size_t)nc, sizeof(i64), i64_cmp);
                if (memcmp(got, want, (size_t)na * sizeof(i64)) != 0) bad++;
            }
            i = j;
        }
        snprintf(msg, sizeof msg, "%s: F(s) of every one of %ld strings == "
            "its bag's F carried to that order", level_name[lv], strings);
        check(msg, bad == 0 && strings == level_strings[lv]);
    }
    check("first_order (equal tokens keep their canonical order) picks "
        "exactly the orders D131's list of strings seen picks, every order "
        "of every bag at every level", order_bad == 0);
}

/* ================================================================
 * Readers and hand-worked sentences
 * ================================================================ */

/* P(s), M(s), R_P(s) as membership flags over the analyses of s */
typedef struct {
    int nf, np, nm, nrp;
    int in_p[MAXA], in_m[MAXA], in_rp[MAXA];
} Readers;

static void
read_with (
    const Analysis *a,
    int             na,
    int             n,
    Readers        *r)
{
    int k;

    r->nf = na;
    r->np = r->nm = r->nrp = 0;
    for (k = 0; k < na; k++) {
        int ix = tree_index(a[k].head, n);

        if (!t_valid[n][ix]) fatal("analysis is not a tree");
        r->in_p[k] = t_proj[n][ix];
        r->in_m[k] = t_mild[n][ix];
        r->np += r->in_p[k];
        r->nm += r->in_m[k];
    }
    for (k = 0; k < na; k++) {
        r->in_rp[k] = r->np > 0 ? r->in_p[k] : 1;
        r->nrp += r->in_rp[k];
    }
}

/* cells forced over the analyses selected by sel (NULL = all) */
static void
forced_cells (
    const Analysis *a,
    int             na,
    const int      *sel,
    int             n,
    int            *attach,
    int            *reading)
{
    int i, k, first;

    *attach = *reading = 0;
    for (i = 0; i < n; i++) {
        int h = -1, rd = -1, fa = 1, fr = 1;

        first = 1;
        for (k = 0; k < na; k++) {
            if (sel != NULL && !sel[k]) continue;
            if (first) {
                h = a[k].head[i];
                rd = a[k].rd[i];
                first = 0;
                continue;
            }
            if (a[k].head[i] != h) fa = 0;
            if (a[k].rd[i] != rd) fr = 0;
        }
        if (first) fa = fr = 0;
        *attach += fa;
        *reading += fr;
    }
}

typedef struct {
    const char *title;
    int         lv;
    int         n;
    int         rd[MAXW];
    int         head[MAXW];
    const char *text;          /* the string at that level (NULL: L1) */
    int         nf, np, nm;
    int         truth_read;    /* truth in R_P(s) */
    int         truth_read_m;  /* truth in R_M(s) */
    int         attach, reading;   /* forced cells under F */
    int         attach_p;      /* forced attachment cells under P (-1: P
                                  empty) */
} Hand;

static void
part_hand (void)
{
    Hand h[9];
    int  k, i;

    printf("\n--- hand-worked sentences ---\n");
    memset(h, 0, sizeof h);
    /* bonum (m acc sg) displaced across puella's arc: unambiguous */
    h[0].title = "bonum puella dominum amat (bonum -> dominum, crossed)";
    h[0].lv = 0; h[0].n = 4; h[0].text = "bonum puella dominum amat";
    h[0].rd[0] = rd_adj(0, MASC, ACC, SG); h[0].head[0] = 3;
    h[0].rd[1] = rd_noun(0, NOM, SG);      h[0].head[1] = 4;
    h[0].rd[2] = rd_noun(1, ACC, SG);      h[0].head[2] = 4;
    h[0].rd[3] = rd_verb(0);               h[0].head[3] = 0;
    h[0].nf = 1; h[0].np = 0; h[0].nm = 1; h[0].truth_read = 1;
    h[0].attach = 4; h[0].reading = 4;
    h[0].truth_read_m = 1; h[0].attach_p = -1;
    /* the same words, nested */
    h[1].title = "puella bonum dominum amat (nested)";
    h[1].lv = 0; h[1].n = 4; h[1].text = "puella bonum dominum amat";
    h[1].rd[0] = rd_noun(0, NOM, SG);      h[1].head[0] = 4;
    h[1].rd[1] = rd_adj(0, MASC, ACC, SG); h[1].head[1] = 3;
    h[1].rd[2] = rd_noun(1, ACC, SG);      h[1].head[2] = 4;
    h[1].rd[3] = rd_verb(0);               h[1].head[3] = 0;
    h[1].nf = 1; h[1].np = 1; h[1].nm = 1; h[1].truth_read = 1;
    h[1].attach = 4; h[1].reading = 4;
    h[1].truth_read_m = 1; h[1].attach_p = 4;
    /* bona (n acc pl) displaced: syncretic, a nested misreading exists */
    h[2].title = "bona puella dona amat (bona -> dona, crossed: garden path)";
    h[2].lv = 0; h[2].n = 4; h[2].text = "bona puella dona amat";
    h[2].rd[0] = rd_adj(0, NEUT, ACC, PL); h[2].head[0] = 3;
    h[2].rd[1] = rd_noun(0, NOM, SG);      h[2].head[1] = 4;
    h[2].rd[2] = rd_noun(2, ACC, PL);      h[2].head[2] = 4;
    h[2].rd[3] = rd_verb(0);               h[2].head[3] = 0;
    h[2].nf = 2; h[2].np = 1; h[2].nm = 2; h[2].truth_read = 0;
    h[2].attach = 3; h[2].reading = 3;
    h[2].truth_read_m = 1; h[2].attach_p = 4;
    /* the same analysis at L1: the ending says which noun */
    h[3] = h[2];
    h[3].title = "the same analysis at L1 (ideal endings): no garden path";
    h[3].lv = 1; h[3].text = NULL;
    h[3].nf = 1; h[3].np = 0; h[3].nm = 1; h[3].truth_read = 1;
    h[3].attach = 4; h[3].reading = 4;
    h[3].truth_read_m = 1; h[3].attach_p = -1;
    /* neuter nominative = accusative: readings open, attachments forced */
    h[4].title = "donum donum amat (which donum is the subject?)";
    h[4].lv = 0; h[4].n = 3; h[4].text = "donum donum amat";
    h[4].rd[0] = rd_noun(2, NOM, SG);      h[4].head[0] = 3;
    h[4].rd[1] = rd_noun(2, ACC, SG);      h[4].head[1] = 3;
    h[4].rd[2] = rd_verb(0);               h[4].head[2] = 0;
    h[4].nf = 2; h[4].np = 2; h[4].nm = 2; h[4].truth_read = 1;
    h[4].attach = 3; h[4].reading = 1;
    h[4].truth_read_m = 1; h[4].attach_p = 3;
    /* case stripped: subject and object trade places */
    h[5].title = "puella dominus amat at L2 (puella dominum amat at L0)";
    h[5].lv = 2; h[5].n = 3; h[5].text = "puella dominus amat";
    h[5].rd[0] = rd_noun(0, NOM, SG);      h[5].head[0] = 3;
    h[5].rd[1] = rd_noun(1, ACC, SG);      h[5].head[1] = 3;
    h[5].rd[2] = rd_verb(0);               h[5].head[2] = 0;
    h[5].nf = 2; h[5].np = 2; h[5].nm = 2; h[5].truth_read = 1;
    h[5].attach = 3; h[5].reading = 1;
    h[5].truth_read_m = 1; h[5].attach_p = 3;
    h[6] = h[5];
    h[6].title = "puella dominum amat at L0";
    h[6].lv = 0; h[6].text = "puella dominum amat";
    h[6].nf = 1; h[6].np = 1; h[6].nm = 1; h[6].attach = 3; h[6].reading = 3;
    /* neuter plurals (the registered dona pair): bona's head is open
     * under F, forced under P - one cell of A */
    h[7].title = "amant bona dona dona (bona -> first dona; subject and "
        "object neuter plurals)";
    h[7].lv = 0; h[7].n = 4; h[7].text = "amant bona dona dona";
    h[7].rd[0] = rd_verb(1);               h[7].head[0] = 0;
    h[7].rd[1] = rd_adj(0, NEUT, NOM, PL); h[7].head[1] = 3;
    h[7].rd[2] = rd_noun(2, NOM, PL);      h[7].head[2] = 1;
    h[7].rd[3] = rd_noun(2, ACC, PL);      h[7].head[3] = 1;
    h[7].nf = 4; h[7].np = 2; h[7].nm = 4; h[7].truth_read = 1;
    h[7].truth_read_m = 1; h[7].attach = 3; h[7].reading = 1;
    h[7].attach_p = 4;
    /* two adjectives crossing to two nouns: not well-nested, and the
     * string has mild alternatives - R_M misreads it too */
    h[8].title = "amant bona bona dona dona (bona -> dona, bona -> dona, "
        "interleaved: not mild)";
    h[8].lv = 0; h[8].n = 5; h[8].text = "amant bona bona dona dona";
    h[8].rd[0] = rd_verb(1);               h[8].head[0] = 0;
    h[8].rd[1] = rd_adj(0, NEUT, NOM, PL); h[8].head[1] = 4;
    h[8].rd[2] = rd_adj(0, NEUT, ACC, PL); h[8].head[2] = 5;
    h[8].rd[3] = rd_noun(2, NOM, PL);      h[8].head[3] = 1;
    h[8].rd[4] = rd_noun(2, ACC, PL);      h[8].head[4] = 1;
    h[8].nf = 8; h[8].np = 2; h[8].nm = 6; h[8].truth_read = 0;
    h[8].truth_read_m = 0; h[8].attach = 3; h[8].reading = 1;
    h[8].attach_p = 5;

    for (k = 0; k < 9; k++) {
        const Hand *x = &h[k];
        int         str[MAXW], na, t = -1, fa, fr, ok, fap = -1, frp, rm;
        char        text[200], msg[400];
        Analysis    truth;
        Readers     r;

        text[0] = '\0';
        for (i = 0; i < x->n; i++) {
            str[i] = form_of[x->lv][x->rd[i]];
            truth.head[i] = x->head[i];
            truth.rd[i] = x->rd[i];
            if (i > 0) strcat(text, " ");
            strcat(text, form_text[x->lv][str[i]]);
        }
        na = analyse(x->lv, str, x->n, abuf, MAXA);
        for (i = 0; i < na; i++)
            if (analysis_key(&abuf[i], x->n) == analysis_key(&truth, x->n))
                t = i;
        read_with(abuf, na, x->n, &r);
        forced_cells(abuf, na, NULL, x->n, &fa, &fr);
        if (r.np > 0) forced_cells(abuf, na, r.in_p, x->n, &fap, &frp);
        rm = t >= 0 && (r.nm > 0 ? r.in_m[t] : 1);
        ok = (x->text == NULL || strcmp(text, x->text) == 0) && t >= 0
            && r.nf == x->nf && r.np == x->np && r.nm == x->nm
            && r.in_rp[t] == x->truth_read && rm == x->truth_read_m
            && fa == x->attach && fr == x->reading && fap == x->attach_p;
        snprintf(msg, sizeof msg, "%s: |F| %d, |P| %d, |M| %d, truth %s by "
            "R_P and %s by R_M, forced cells under F %d attachment / %d "
            "reading, under P %d attachment", x->title, r.nf, r.np, r.nm,
            t >= 0 && r.in_rp[t] ? "read" : "MISREAD", rm ? "read"
            : "MISREAD", fa, fr, fap);
        check(msg, ok);
    }
}

/* ================================================================
 * Part B: licensing cost A and garden paths B
 * ================================================================ */

enum { DC_ADJ, DC_GEN, DC_MIX, NDC };    /* what the displaced words are */

static const char *const dc_name[NDC] = {
    "adjectives only", "genitives only", "both"
};

typedef struct {
    s64 pairs;
    s64 a_num, a_den, ar_num;            /* projective pairs */
    s64 b_num, b_den;                    /* crossed pairs: misread by R_P */
    s64 bs_num, bs_den, bu_num, bu_den;  /* displaced word syncretic or not */
    s64 bm_num;                          /* misread by R_M */
    s64 len_a_num[MAXW + 1], len_a_den[MAXW + 1];
    s64 len_b_num[MAXW + 1], len_b_den[MAXW + 1];
    s64 cls_num[NDC], cls_den[NDC];      /* descriptive, not registered */
    s64 cls_s_num[NDC], cls_s_den[NDC];  /* ... a syncretic displaced word */
    s64 cls_u_num[NDC], cls_u_den[NDC];  /* ... all unambiguous */
    s64 vdisp;                           /* crossed pairs displacing a verb
                                          * dependent (0: 58.2) */
    s64 mild_num[2], mild_den[2];        /* crossed pairs, [1] = mild */
    s64 cr_num[5], cr_den[5];            /* by crossings 1..3, [4] = 4+ */
    s64 open_f, open_p;                  /* projective pairs: open cells
                                          * under F, under P (A = diff) */
    s64 f_proj, f_cross;                 /* sum of |F(s)| over the pairs */
} Measures;

static Measures meas[NLEVELS];
static Analysis mapped[MAXA];
static Readers  rdr;

static void
measure_level (
    int       lv,
    Measures *ms)
{
    long i = 0;

    memset(ms, 0, sizeof *ms);
    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a;
        i64      seen[720];
        int      nseen = 0, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        str[MAXW], inv[MAXW], dup = 0, faf, frf, fap = 0, frp = 0;
            i64        code;

            for (k = 0; k < nb; k++) {
                str[k] = bag[sig[k]];
                inv[sig[k]] = k;
            }
            code = bag_code(str, nb);
            for (k = 0; k < nseen; k++)
                if (seen[k] == code) dup = 1;
            if (dup) continue;
            seen[nseen++] = code;
            for (a = 0; a < nc; a++)
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
            read_with(mapped, nc, nb, &rdr);
            forced_cells(mapped, nc, NULL, nb, &faf, &frf);
            if (rdr.np > 0)
                forced_cells(mapped, nc, rdr.in_p, nb, &fap, &frp);
            for (a = 0; a < nc; a++) {
                int ix = tree_index(mapped[a].head, nb);

                ms->pairs++;
                if (rdr.in_p[a]) {
                    ms->a_num += fap - faf;
                    ms->ar_num += frp - frf;
                    ms->a_den += nb;
                    ms->len_a_num[nb] += fap - faf;
                    ms->len_a_den[nb] += nb;
                    ms->open_f += nb - faf;
                    ms->open_p += nb - fap;
                    ms->f_proj += nc;
                } else {
                    int mis = rdr.np > 0, mask = t_npmask[nb][ix];
                    int syn = 0, nadj = 0, ngen = 0, nother = 0, cls, cr;

                    ms->b_den++;
                    ms->b_num += mis;
                    ms->len_b_den[nb]++;
                    ms->len_b_num[nb] += mis;
                    ms->f_cross += nc;
                    for (k = 0; k < nb; k++) {
                        const Reading *x;

                        if (!(mask & (1 << k))) continue;
                        x = &readings[mapped[a].rd[k]];
                        if (form_nr[lv][str[k]] >= 2) syn = 1;
                        if (x->kind == K_ADJ) nadj++;
                        else if (x->kind == K_NOUN && mapped[a].head[k] != 0
                            && readings[mapped[a].rd[mapped[a].head[k] - 1]]
                            .kind == K_NOUN)
                            ngen++;
                        else nother++;
                    }
                    if (syn) {
                        ms->bs_den++;
                        ms->bs_num += mis;
                    } else {
                        ms->bu_den++;
                        ms->bu_num += mis;
                    }
                    if (!rdr.in_m[a] && rdr.nm > 0) ms->bm_num++;
                    if (nother > 0) ms->vdisp++;
                    cls = ngen == 0 ? DC_ADJ : nadj == 0 ? DC_GEN : DC_MIX;
                    ms->cls_den[cls]++;
                    ms->cls_num[cls] += mis;
                    if (syn) {
                        ms->cls_s_den[cls]++;
                        ms->cls_s_num[cls] += mis;
                    } else {
                        ms->cls_u_den[cls]++;
                        ms->cls_u_num[cls] += mis;
                    }
                    ms->mild_den[rdr.in_m[a] ? 1 : 0]++;
                    ms->mild_num[rdr.in_m[a] ? 1 : 0] += mis;
                    cr = t_cross[nb][ix] > 4 ? 4 : t_cross[nb][ix];
                    ms->cr_den[cr]++;
                    ms->cr_num[cr] += mis;
                }
            }
        }
        i = j;
    }
}

/* num / den as a percentage with two decimals, integer arithmetic */
static void
pct (
    char *buf,
    int   size,
    s64   num,
    s64   den)
{
    s64 x;

    if (den == 0) {
        snprintf(buf, (size_t)size, "-");
        return;
    }
    x = (num * 20000 + den) / (2 * den);   /* hundredths of a percent */
    snprintf(buf, (size_t)size, "%lld.%02lld%%", (long long)(x / 100),
        (long long)(x % 100));
}

/* oracle_partb.py: [quick][level] = a_num, a_den, ar_num, b_num, b_den,
 * bs_num, bs_den, bu_num, bu_den, bm_num */
static const s64 oracle_b[2][NLEVELS][10] = {
    { { 878568, 13631616, 79068, 2420352, 4018176, 758358, 1665120,
        1661994, 2353056, 383068 },
      { 884736, 13631616, 0, 2293760, 4018176, 0, 0, 2293760, 4018176,
        376832 },
      { 993296, 13631616, 37632, 3470872, 4018176, 3470872, 4018176, 0, 0,
        397112 },
      { 996352, 13631616, 29696, 3853312, 4018176, 3853312, 4018176, 0, 0,
        406528 } },
    { { 8208, 160896, 464, 8720, 15360, 2568, 5568, 6152, 9792, 0 },
      { 8192, 160896, 0, 8192, 15360, 0, 0, 8192, 15360, 0 },
      { 9760, 160896, 32, 12832, 15360, 12832, 15360, 0, 0, 0 },
      { 11264, 160896, 1024, 14336, 15360, 14336, 15360, 0, 0, 0 } },
};

/* oracle_partb.py, descriptive split: [quick][level] = for adjectives
 * only, genitives only, both: (num, den, syncretic num, den, unambiguous
 * num, den) */
static const s64 oracle_cls[2][NLEVELS][18] = {
    { { 43088, 663552, 42134, 578784, 954, 84768,
        2330624, 3108864, 673280, 869376, 1657344, 2239488,
        46640, 245760, 42944, 216960, 3696, 28800 },
      { 0, 663552, 0, 0, 0, 663552,
        2269184, 3108864, 0, 0, 2269184, 3108864,
        24576, 245760, 0, 0, 24576, 245760 },
      { 236864, 663552, 236864, 663552, 0, 0,
        3108864, 3108864, 3108864, 3108864, 0, 0,
        125144, 245760, 125144, 245760, 0, 0 },
      { 523264, 663552, 523264, 663552, 0, 0,
        3108864, 3108864, 3108864, 3108864, 0, 0,
        221184, 245760, 221184, 245760, 0, 0 } },
    { { 272, 3072, 264, 2496, 8, 576,
        8448, 12288, 2304, 3072, 6144, 9216, 0, 0, 0, 0, 0, 0 },
      { 0, 3072, 0, 0, 0, 3072,
        8192, 12288, 0, 0, 8192, 12288, 0, 0, 0, 0, 0, 0 },
      { 544, 3072, 544, 3072, 0, 0,
        12288, 12288, 12288, 12288, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 2048, 3072, 2048, 3072, 0, 0,
        12288, 12288, 12288, 12288, 0, 0, 0, 0, 0, 0, 0, 0 } },
};

/* oracle_partb.py: [quick][level] = open cells under F, under P
 * (projective pairs); B over mild crossed pairs (num, den), over the
 * others (num, den); B by crossings 1, 2, 3, 4+ (num, den each) */
static const s64 oracle_x[2][NLEVELS][14] = {
    { { 2209476, 1330908, 2264456, 3611648, 155896, 406528,
        1232342, 1788928, 788644, 1332224, 349528, 691712, 49838, 205312 },
      { 1953792, 1069056, 2154496, 3611648, 139264, 406528,
        1185792, 1788928, 743424, 1332224, 323584, 691712, 40960, 205312 },
      { 9736704, 8743408, 3131120, 3611648, 339752, 406528,
        1606828, 1788928, 1131080, 1332224, 573400, 691712, 159564, 205312 },
      { 10017792, 9021440, 3476480, 3611648, 376832, 406528,
        1739264, 1788928, 1268224, 1332224, 651776, 691712, 194048,
        205312 } },
    { { 13464, 5256, 8720, 15360, 0, 0, 6476, 10240, 2244, 5120, 0, 0, 0, 0 },
      { 12288, 4096, 8192, 15360, 0, 0, 6144, 10240, 2048, 5120, 0, 0, 0, 0 },
      { 68400, 58640, 12832, 15360, 0, 0, 8600, 10240, 4232, 5120, 0, 0, 0,
        0 },
      { 70656, 59392, 14336, 15360, 0, 0, 9728, 10240, 4608, 5120, 0, 0, 0,
        0 } },
};

/* the sums of |F(s)| over projective and over crossed pairs: [quick]
 * [level]; from oracle_partb.py with one line added (each pair adds
 * len(an) to f_proj or f_cross), D134 task 0 - mean |F| exactly */
static const s64 oracle_f[2][NLEVELS][2] = {
    { { 8549790, 17011152 }, { 6752640, 14032896 },
      { 182622792, 271261368 }, { 188821056, 285760512 } },
    { { 59118, 31824 }, { 52608, 27648 },
      { 350832, 153648 }, { 353856, 156672 } },
};

static void
part_measures (void)
{
    int  lv, n, c;
    char msg[400], s1[32], s2[32], s3[32], s4[32];

    printf("\n--- Part B: licensing cost A and garden paths B ---\n");
    printf("  A = attachment cells open under F but forced under P, over "
        "projective pairs,\n      as a share of their cells (every word's, "
        "the verb's included)\n");
    printf("  B = crossed pairs misread by R_P (nested first)\n\n");
    for (lv = 0; lv < NLEVELS; lv++) measure_level(lv, &meas[lv]);

    printf("  %-17s %10s %10s %10s %10s %10s\n", "level", "A", "A reading",
        "B", "B R_M", "mean |F|");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];
        char            f[32];

        pct(s1, 32, m->a_num, m->a_den);
        pct(s2, 32, m->ar_num, m->a_den);
        pct(s3, 32, m->b_num, m->b_den);
        pct(s4, 32, m->bm_num, m->b_den);
        {
            s64 h = ((m->f_proj + m->f_cross) * 200 + m->pairs)
                / (2 * m->pairs);              /* hundredths, rounded */
            snprintf(f, sizeof f, "%lld.%02lld", (long long)(h / 100),
                (long long)(h % 100));
        }
        printf("  %-17s %10s %10s %10s %10s %10s\n", level_name[lv], s1, s2,
            s3, s4, f);
    }
    printf("\n  exact: A = num / %lld (projective pairs' cells), B = num / %lld "
        "(crossed pairs)\n", (long long)meas[0].a_den, (long long)meas[0].b_den);
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s A %lld, B %lld\n", level_name[lv],
            (long long)meas[lv].a_num, (long long)meas[lv].b_num);

    printf("\n  B by the displaced word's form (registered split, Q2)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];

        pct(s1, 32, m->bs_num, m->bs_den);
        pct(s2, 32, m->bu_num, m->bu_den);
        printf("  %-17s syncretic %8s of %8lld   unambiguous %8s of %8lld\n",
            level_name[lv], s1, (long long)m->bs_den, s2,
            (long long)m->bu_den);
    }
    printf("\n  B by what is displaced (descriptive, NOT registered; each "
        "class split by the displaced words' forms)\n");
    for (c = 0; c < NDC; c++)
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];

            pct(s1, 32, m->cls_num[c], m->cls_den[c]);
            pct(s2, 32, m->cls_s_num[c], m->cls_s_den[c]);
            pct(s3, 32, m->cls_u_num[c], m->cls_u_den[c]);
            printf("  %-16s %-17s %8s of %8lld  syncretic %8s of %8lld  "
                "unambiguous %8s of %8lld\n", dc_name[c], level_name[lv], s1,
                (long long)m->cls_den[c], s2, (long long)m->cls_s_den[c], s3,
                (long long)m->cls_u_den[c]);
        }
    printf("\n  B by stratum (registered as reported, not predicted): mild / "
        "not mild; crossings 1, 2, 3, 4+ (root arc included)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        const Measures *m = &meas[lv];

        pct(s1, 32, m->mild_num[1], m->mild_den[1]);
        pct(s2, 32, m->mild_num[0], m->mild_den[0]);
        printf("  %-17s mild %s of %lld, not mild %s of %lld;", level_name[lv],
            s1, (long long)m->mild_den[1], s2, (long long)m->mild_den[0]);
        for (c = 1; c <= 4; c++) {
            pct(s1, 32, m->cr_num[c], m->cr_den[c]);
            printf(" %d: %s of %lld", c, s1, (long long)m->cr_den[c]);
        }
        printf("\n");
    }
    printf("\n  the open cells behind A (projective pairs): open under F, "
        "under P\n");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %lld, %lld (difference %lld = A's numerator)\n",
            level_name[lv], (long long)meas[lv].open_f,
            (long long)meas[lv].open_p,
            (long long)(meas[lv].open_f - meas[lv].open_p));
    printf("\n  by length (A; B)\n");
    for (n = 3; n <= gmaxw; n++) {
        printf("  %d words:", n);
        for (lv = 0; lv < NLEVELS; lv++) {
            pct(s1, 32, meas[lv].len_a_num[n], meas[lv].len_a_den[n]);
            pct(s2, 32, meas[lv].len_b_num[n], meas[lv].len_b_den[n]);
            printf("  %s %s; %s", lv == 0 ? "L0" : lv == 1 ? "L1" : lv == 2
                ? "L2" : "L3", s1, s2);
        }
        printf("\n");
    }

    {
        int same = 1, q = quick ? 1 : 0, ok = 1;

        for (lv = 1; lv < NLEVELS; lv++)
            if (meas[lv].a_den != meas[0].a_den
                || meas[lv].b_den != meas[0].b_den)
                same = 0;
        check("A and B have one denominator each at every level (the "
            "populations do not depend on the endings)", same);
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_b[q][lv];

            if (m->a_num != o[0] || m->a_den != o[1] || m->ar_num != o[2]
                || m->b_num != o[3] || m->b_den != o[4] || m->bs_num != o[5]
                || m->bs_den != o[6] || m->bu_num != o[7] || m->bu_den != o[8]
                || m->bm_num != o[9])
                ok = 0;
        }
        snprintf(msg, sizeof msg, "A, A (reading), B, B split, B (R_M) at "
            "every level == oracle_partb.py (independent, tree-first; a pin, "
            "%s)", quick ? "four words" : "five words");
        check(msg, ok);
        check("L1 has no syncretic form, so no reading-cell cost and no "
            "syncretic displaced word", meas[1].ar_num == 0
            && meas[1].bs_den == 0);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_cls[q][lv];

            for (c = 0; c < NDC; c++)
                if (m->cls_num[c] != o[6 * c] || m->cls_den[c] != o[6 * c + 1]
                    || m->cls_s_num[c] != o[6 * c + 2]
                    || m->cls_s_den[c] != o[6 * c + 3]
                    || m->cls_u_num[c] != o[6 * c + 4]
                    || m->cls_u_den[c] != o[6 * c + 5])
                    ok = 0;
        }
        check("descriptive split (adjectives only / genitives only / both, "
            "each by form) == oracle_partb.py (a pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++) {
            const Measures *m = &meas[lv];
            const s64      *o = oracle_x[q][lv];

            if (m->open_f != o[0] || m->open_p != o[1]
                || m->mild_num[1] != o[2] || m->mild_den[1] != o[3]
                || m->mild_num[0] != o[4] || m->mild_den[0] != o[5])
                ok = 0;
            for (c = 1; c <= 4; c++)
                if (m->cr_num[c] != o[4 + 2 * c] || m->cr_den[c] != o[5 + 2 * c])
                    ok = 0;
        }
        check("open cells under F and P, B by mildness and by crossing count "
            "== oracle_partb.py (a pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++)
            if (meas[lv].f_proj != oracle_f[q][lv][0]
                || meas[lv].f_cross != oracle_f[q][lv][1])
                ok = 0;
        check("mean |F|: the sums of |F(s)| over projective and over crossed "
            "pairs at every level == oracle_partb.py's (one line added; a "
            "pin)", ok);
        ok = 1;
        for (lv = 0; lv < NLEVELS; lv++)
            if (meas[lv].vdisp != 0 || meas[lv].open_f - meas[lv].open_p
                != meas[lv].a_num)
                ok = 0;
        check("no crossed pair displaces a verb dependent, counted directly "
            "(the root word dominates every word); open F - open P == A's "
            "numerator", ok);
    }
}

static double
seconds (
    clock_t t0)
{
    return (double)(clock() - t0) / (double)CLOCKS_PER_SEC;
}

/* times are printed only with D134_TEMPORA=1: archive.sh needs the live
 * and the frozen run byte-identical */
static int show_times = 0;

static void
tempus (
    const char *what,
    clock_t     t0)
{
    if (show_times) printf("  (%s: %.1f s)\n", what, seconds(t0));
}

/* ================================================================
 * Umbrae, width and depth; competition and retrieval (task 1)
 * ================================================================ */

enum { U_DANGLING, U_SLOT, U_ROOT };

typedef struct {
    int type;
    int from, to;           /* opened at word from (0: boundary 0, the
                               root umbra), closed at word to; open at
                               the boundaries t with from <= t < to */
    int word;               /* the dependent; the verb for the root */
} Umbra;

typedef struct {
    int   nu, nback, width;
    Umbra u[MAXW + 1];
    int   depth[MAXW + 1];      /* by the left-to-right sweep */
    int   umbra_of[MAXW + 1];   /* word (1-based) -> its arc's umbra; the
                                   verb -> the root umbra; -1: a backward
                                   attachment */
} Umbrae;

static int
relation_of (
    const Analysis *a,
    int             d)
{
    const Reading *x = &readings[a->rd[d - 1]];
    int            h = a->head[d - 1];

    if (h == 0) return R_ROOT;
    if (x->kind == K_ADJ) return R_AMOD;
    if (readings[a->rd[h - 1]].kind == K_NOUN) return R_NMOD;
    if (x->cas == NOM) return R_NSUBJ;
    if (x->cas == ACC) return R_OBJ;
    if (x->cas == DAT) return R_IOBJ;
    return R_OBL;
}

/* required relations: nsubj, obj, and iobj (which only dat / dant take) */
static int
required (
    int r)
{
    return r == R_NSUBJ || r == R_OBJ || r == R_IOBJ;
}

/* the umbrae of a pair: heads and readings only, no level */
static void
build_umbrae (
    const Analysis *a,
    int             n,
    Umbrae         *u)
{
    int d, t, j, k, i, c, m;
    int open[MAXW + 1], nopen = 0;

    u->nu = u->nback = u->width = 0;
    for (d = 1; d <= n; d++) {
        int    h = a->head[d - 1];
        Umbra *x = &u->u[u->nu];

        if (h == 0) {
            x->type = U_ROOT; x->from = 0; x->to = d;
        } else if (d < h) {
            x->type = U_DANGLING; x->from = d; x->to = h;
        } else if (required(relation_of(a, d))) {
            x->type = U_SLOT; x->from = h; x->to = d;
        } else {
            u->umbra_of[d] = -1;     /* backward attachment */
            u->nback++;
            continue;
        }
        x->word = d;
        u->depth[u->nu] = -1;
        u->umbra_of[d] = u->nu++;
    }
    for (t = 0; t < n; t++) {
        c = 0;
        for (k = 0; k < u->nu; k++)
            if (u->u[k].from <= t && t < u->u[k].to) c++;
        if (c > u->width) u->width = c;
    }
    /* depth by a sweep: when word j closes an umbra, count the umbrae
     * still open that were opened after it and outlive j */
    for (k = 0; k < u->nu; k++)
        if (u->u[k].from == 0) open[nopen++] = k;
    for (j = 1; j <= n; j++) {
        for (k = 0; k < nopen; k++) {
            const Umbra *x = &u->u[open[k]];

            if (x->to != j) continue;
            c = 0;
            for (i = 0; i < nopen; i++) {
                const Umbra *y = &u->u[open[i]];
                if (y->from > x->from && y->to > j) c++;
            }
            u->depth[open[k]] = c;
        }
        m = 0;
        for (k = 0; k < nopen; k++)
            if (u->u[open[k]].to != j) open[m++] = open[k];
        nopen = m;
        for (k = 0; k < u->nu; k++)
            if (u->u[k].from == j) open[nopen++] = k;
    }
}

/* depth, second route: the umbra arcs opened strictly inside umbra k's
 * span that outlive it (the arcs that cross it, later-opened) */
static int
depth_pairwise (
    const Umbrae *u,
    int           k)
{
    int i, c = 0;

    for (i = 0; i < u->nu; i++)
        if (u->u[k].from < u->u[i].from && u->u[i].from < u->u[k].to
            && u->u[i].to > u->u[k].to)
            c++;
    return c;
}

/* compatibility: some reading of form x and some reading of form y
 * satisfy relation r, x the dependent, y the head (local: no global
 * constraint) */
static unsigned char comp_rel[NLEVELS][R_AMOD + 1][MAXF][MAXF];
static unsigned char comp_any[NLEVELS][MAXF][MAXF];

static int
rel_ok (
    const Reading *x,
    const Reading *y,
    int            r)
{
    if (r == R_AMOD)
        return x->kind == K_ADJ && y->kind == K_NOUN && x->cas == y->cas
            && x->gender == y->gender && x->number == y->number;
    if (x->kind != K_NOUN) return 0;
    if (r == R_NMOD) return x->cas == GEN && y->kind == K_NOUN;
    if (y->kind != K_VERB) return 0;
    if (r == R_NSUBJ) return x->cas == NOM && x->number == y->number;
    if (r == R_OBJ) return x->cas == ACC;
    if (r == R_IOBJ) return x->cas == DAT && verbs[y->lemma].ditrans;
    if (r == R_OBL) return x->cas == ABL;
    return 0;
}

static void
build_comp (void)
{
    int lv, fx, fy, i, j, r;

    memset(comp_rel, 0, sizeof comp_rel);
    memset(comp_any, 0, sizeof comp_any);
    for (lv = 0; lv < NLEVELS; lv++)
        for (fx = 0; fx < n_forms[lv]; fx++)
            for (fy = 0; fy < n_forms[lv]; fy++)
                for (i = 0; i < form_nr[lv][fx]; i++)
                    for (j = 0; j < form_nr[lv][fy]; j++)
                        for (r = R_NSUBJ; r <= R_AMOD; r++)
                            if (rel_ok(&readings[form_rd[lv][fx][i]],
                                    &readings[form_rd[lv][fy][j]], r)) {
                                comp_rel[lv][r][fx][fy] = 1;
                                comp_any[lv][fx][fy] = 1;
                            }
}

/* w in d's subtree (w == d included), by walking up from w */
static int
descends (
    const Analysis *a,
    int             w,
    int             d)
{
    int steps = 0;

    while (w != 0 && steps++ <= MAXW) {
        if (w == d) return 1;
        w = a->head[w - 1];
    }
    return 0;
}

/* yields as bitmasks (bit k = word k + 1), second route for subtrees and
 * for the displaced arcs */
static void
yields (
    const Analysis *a,
    int             n,
    int            *yl)
{
    int d, pass;

    for (d = 1; d <= n; d++) yl[d] = 1 << (d - 1);
    for (pass = 0; pass < n; pass++)
        for (d = 1; d <= n; d++)
            if (a->head[d - 1] != 0) yl[a->head[d - 1]] |= yl[d];
}

typedef struct {
    int nd;
    int word[MAXW];             /* displaced dependents, 1-based, in order */
    int dep_first[MAXW];
    int competed[MAXW], retrieved[MAXW];
    int competed_rel[MAXW], retrieved_rel[MAXW];   /* own relation only */
    int competed_blind[MAXW];   /* any noun as near: morphology-blind */
    int retrieved_ns[MAXW];     /* candidates that would close a cycle
                                   set aside (descriptive) */
} Labels;

/* competition and retrieval of the displaced arcs (mask: bit d - 1) at
 * level lv */
static void
label_arcs (
    int             lv,
    const Analysis *a,
    int             n,
    int             mask,
    const Umbrae   *u,
    Labels         *lb)
{
    int f[MAXW], k, d;

    for (k = 0; k < n; k++) f[k] = form_of[lv][a->rd[k]];
    lb->nd = 0;
    for (d = 1; d <= n; d++) {
        int h, r, dist, w, c = 0, cr = 0, cb = 0, ret = 1, retr = 1;
        int rns = 1;

        if (!(mask & (1 << (d - 1)))) continue;
        h = a->head[d - 1];
        r = relation_of(a, d);
        dist = h > d ? h - d : d - h;
        for (w = 1; w <= n; w++) {
            if (w == d || w == h || (w > d ? w - d : d - w) > dist) continue;
            if (descends(a, w, d)) continue;
            if (comp_any[lv][f[d - 1]][f[w - 1]]) c = 1;
            if (comp_rel[lv][r][f[d - 1]][f[w - 1]]) cr = 1;
            if (form_kind(lv, f[w - 1]) == K_NOUN) cb = 1;
        }
        if (d < h) {
            /* h arrives: the dangling umbrae open at boundary h - 1 that
             * could depend on h, beyond the ones h closes */
            for (k = 0; k < u->nu; k++) {
                const Umbra *x = &u->u[k];

                if (x->type != U_DANGLING || x->from > h - 1 || x->to <= h - 1
                    || x->to == h)
                    continue;
                if (comp_any[lv][f[x->word - 1]][f[h - 1]]) {
                    ret = 0;
                    if (!descends(a, h, x->word)) rns = 0;
                }
                if (comp_rel[lv][r][f[x->word - 1]][f[h - 1]]) retr = 0;
            }
        } else {
            /* d arrives: the earlier words it could depend on, beyond h */
            for (w = 1; w < d; w++) {
                if (w == h) continue;
                if (comp_any[lv][f[d - 1]][f[w - 1]]) {
                    ret = 0;
                    if (!descends(a, w, d)) rns = 0;
                }
                if (comp_rel[lv][r][f[d - 1]][f[w - 1]]) retr = 0;
            }
        }
        lb->word[lb->nd] = d;
        lb->dep_first[lb->nd] = d < h;
        lb->competed[lb->nd] = c;
        lb->competed_rel[lb->nd] = cr;
        lb->competed_blind[lb->nd] = cb;
        lb->retrieved[lb->nd] = ret;
        lb->retrieved_rel[lb->nd] = retr;
        lb->retrieved_ns[lb->nd] = rns;
        lb->nd++;
    }
}

/* the checks of one pass over the pairs */
typedef struct {
    s64 pairs, crossed, disp, disp_df;
    s64 bad_count;       /* umbrae + backward attachments != arcs + 1 */
    s64 bad_depth;       /* sweep depth != pairwise depth */
    s64 bad_proj;        /* projective pair with an umbra of depth > 0 */
    s64 bad_rel;         /* a displaced arc neither amod nor nmod */
    s64 bad_disp;        /* displaced set != the yield route's */
    s64 bad_mono;        /* L0 RETRIEVED but L1 not, or L0 CLEAR but L1 not */
    s64 bad_tc;          /* the true partner not compatible (T in C, h in C) */
    s64 bad_sub;         /* competition by subtree walk != by yields */
    s64 bad_rt;          /* a root umbra missing or not the verb's */
    s64 lab[NLEVELS][MAXW + 1][4];   /* by level and length: displaced
                                        arcs, RETRIEVED, CLEAR, and
                                        INTERFERED only by a cycle */
} UChecks;

/* competition of one displaced arc again, the subtree excluded by yields */
static int
competed_by_yields (
    int             lv,
    const Analysis *a,
    int             n,
    const int      *yl,
    int             d)
{
    int h = a->head[d - 1];
    int dist = h > d ? h - d : d - h;
    int fd = form_of[lv][a->rd[d - 1]];
    int w;

    for (w = 1; w <= n; w++) {
        if (w == d || w == h || (w > d ? w - d : d - w) > dist) continue;
        if (yl[d] & (1 << (w - 1))) continue;
        if (comp_any[lv][fd][form_of[lv][a->rd[w - 1]]]) return 1;
    }
    return 0;
}

/* one pair: umbrae, the displaced arcs, labels at every level, checks */
static void
check_pair (
    const Analysis *a,
    int             n,
    UChecks        *ck)
{
    Umbrae u;
    Labels lb[NLEVELS];
    int    ix = tree_index(a->head, n), mask = t_npmask[n][ix], mask2 = 0;
    int    yl[MAXW + 1], k, d, lv, i, roots = 0;

    build_umbrae(a, n, &u);
    ck->pairs++;
    if (!t_proj[n][ix]) ck->crossed++;
    if (u.nu + u.nback != n) ck->bad_count++;
    for (k = 0; k < u.nu; k++) {
        if (u.depth[k] != depth_pairwise(&u, k)) ck->bad_depth++;
        if (t_proj[n][ix] && u.depth[k] != 0) ck->bad_proj++;
        if (u.u[k].type == U_ROOT) {
            roots++;
            if (a->head[u.u[k].word - 1] != 0
                || readings[a->rd[u.u[k].word - 1]].kind != K_VERB)
                ck->bad_rt++;
        }
    }
    if (roots != 1) ck->bad_rt++;
    yields(a, n, yl);
    for (d = 1; d <= n; d++) {
        int h = a->head[d - 1], lo, hi, w;

        if (h == 0) continue;
        lo = h < d ? h : d;
        hi = h < d ? d : h;
        for (w = lo + 1; w < hi; w++)
            if (!(yl[h] & (1 << (w - 1)))) mask2 |= 1 << (d - 1);
        if ((mask & (1 << (d - 1))) && relation_of(a, d) != R_AMOD
            && relation_of(a, d) != R_NMOD)
            ck->bad_rel++;
    }
    if (mask2 != mask) ck->bad_disp++;
    for (lv = 0; lv < NLEVELS; lv++) {
        label_arcs(lv, a, n, mask, &u, &lb[lv]);
        for (i = 0; i < lb[lv].nd; i++) {
            int h, w;

            ck->lab[lv][n][0]++;
            ck->lab[lv][n][1] += lb[lv].retrieved[i];
            ck->lab[lv][n][2] += !lb[lv].competed[i];
            ck->lab[lv][n][3] += !lb[lv].retrieved[i] && lb[lv].retrieved_ns[i];

            d = lb[lv].word[i];
            h = a->head[d - 1];
            if (competed_by_yields(lv, a, n, yl, d) != lb[lv].competed[i])
                ck->bad_sub++;
            if (lb[lv].dep_first[i]) {
                for (w = 1; w < h; w++)
                    if (a->head[w - 1] == h && !comp_any[lv][form_of[lv][
                        a->rd[w - 1]]][form_of[lv][a->rd[h - 1]]])
                        ck->bad_tc++;
            } else if (!comp_any[lv][form_of[lv][a->rd[d - 1]]][
                    form_of[lv][a->rd[h - 1]]]) {
                ck->bad_tc++;
            }
        }
    }
    for (i = 0; i < lb[0].nd; i++) {
        ck->disp++;
        ck->disp_df += lb[0].dep_first[i];
        if ((lb[0].retrieved[i] && !lb[1].retrieved[i])
            || (!lb[0].competed[i] && lb[1].competed[i])
            || (lb[0].retrieved_rel[i] && !lb[1].retrieved_rel[i])
            || (!lb[0].competed_rel[i] && lb[1].competed_rel[i]))
            ck->bad_mono++;
    }
}

/* every pair, once, by the L1 bags (multisets of readings): the labels of
 * one pair at all four levels side by side */
static void
umbra_pass (
    int      maxlen,
    UChecks *ck)
{
    long i = 0;

    memset(ck, 0, sizeof *ck);
    while (i < an_entries[1].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[1].n && an_entries[1].e[j].bag
            == an_entries[1].e[i].bag)
            j++;
        decode_bag(an_entries[1].e[i].bag, bag, &nb);
        if (nb > maxlen) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[1].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW];

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++) {
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
                check_pair(&mapped[a], nb, ck);
            }
        }
        i = j;
    }
}

static void
umbra_checks (
    const char    *where,
    const UChecks *ck,
    s64            pairs,
    s64            crossed)
{
    char msg[400];

    printf("  %s: %lld pairs, %lld crossed, %lld displaced arcs (%lld "
        "dependent first)\n", where, (long long)ck->pairs,
        (long long)ck->crossed, (long long)ck->disp, (long long)ck->disp_df);
    snprintf(msg, sizeof msg, "%s: the pass meets every pair once (%lld) and "
        "every crossed pair (%lld)", where, (long long)pairs,
        (long long)crossed);
    check(msg, ck->pairs == pairs && ck->crossed == crossed);
    snprintf(msg, sizeof msg, "%s: umbrae (root included) + backward "
        "attachments == arcs + 1, every pair; one root umbra, closed at the "
        "verb", where);
    check(msg, ck->bad_count == 0 && ck->bad_rt == 0);
    snprintf(msg, sizeof msg, "%s: depth by the sweep == depth by crossing "
        "later-opened umbra arcs, every umbra", where);
    check(msg, ck->bad_depth == 0);
    snprintf(msg, sizeof msg, "%s: a projective pair (root arc included) has "
        "depth 0 everywhere", where);
    check(msg, ck->bad_proj == 0);
    snprintf(msg, sizeof msg, "%s: displaced arcs are amod or nmod only "
        "(KC 58.2), and the per-arc set (tree table) == the yield route's",
        where);
    check(msg, ck->bad_rel == 0 && ck->bad_disp == 0);
    snprintf(msg, sizeof msg, "%s: no competitor lies in d's own subtree "
        "(competition with the subtree by walking up == by yields), every "
        "displaced arc at every level", where);
    check(msg, ck->bad_sub == 0);
    snprintf(msg, sizeof msg, "%s: the true partner is always a candidate "
        "(T in C dependent first, h in C head first), every level", where);
    check(msg, ck->bad_tc == 0);
    snprintf(msg, sizeof msg, "%s: exactly half the displaced arcs are "
        "dependent first (reversing a string keeps the pair, its displaced "
        "arcs and their kind, and swaps the two directions)", where);
    check(msg, 2 * ck->disp_df == ck->disp);
    snprintf(msg, sizeof msg, "%s: per arc, RETRIEVED at L0 => at L1 and "
        "CLEAR at L0 => at L1, the relation-specific variants too (L1 "
        "readings are a subset)", where);
    check(msg, ck->bad_mono == 0);
}

/* hand-worked sentences of plan.md: width, depths, labels, R_P */
typedef struct {
    const char *title;
    int         lv, n;
    int         rd[MAXW], head[MAXW];
    const char *text;
    int         width, root_depth, max_depth;
    int         nd;                 /* displaced arcs, in word order: */
    int         dword[MAXW];        /* the dependent */
    int         ddepth[MAXW];       /* its umbra's depth, -1: backward */
    int         dcomp[MAXW], dretr[MAXW];
    int         dns[MAXW];          /* RETRIEVED with cycle-closing
                                       candidates set aside (checked if
                                       has_ns) */
    int         has_ns;
    int         misread;            /* truth outside R_P(s) */
} HandU;

#define NHANDU 32
static HandU handu[NHANDU];
static int   n_handu = 0;

static HandU *
hand_new (
    const char *title,
    int         lv,
    const char *text,
    int         width,
    int         root_depth,
    int         max_depth,
    int         misread)
{
    HandU *x = &handu[n_handu++];

    memset(x, 0, sizeof *x);
    x->title = title;
    x->lv = lv;
    x->text = text;
    x->width = width;
    x->root_depth = root_depth;
    x->max_depth = max_depth;
    x->misread = misread;
    return x;
}

static void
hand_word (
    HandU *x,
    int    rd,
    int    head)
{
    x->rd[x->n] = rd;
    x->head[x->n] = head;
    x->n++;
}

static void
hand_disp (
    HandU *x,
    int    word,
    int    depth,
    int    competed,
    int    retrieved)
{
    x->dword[x->nd] = word;
    x->ddepth[x->nd] = depth;
    x->dcomp[x->nd] = competed;
    x->dretr[x->nd] = retrieved;
    x->dns[x->nd] = retrieved;
    x->nd++;
}

static void
hand_sentences_umbrae (void)
{
    HandU *x;

    n_handu = 0;
    /* 1: projective; also at L3 */
    x = hand_new("1. puella dominum amat", 0, "puella dominum amat", 3, 0, 0,
        0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_word(x, rd_verb(0), 0);
    handu[n_handu] = *x;
    x = &handu[n_handu++];
    x->title = "1. at L3 (puella dominus amat)";
    x->lv = 3;
    x->text = "puella dominus amat";
    x->misread = 0;
    /* 2: bonum -> dominum, gender keeps puella out */
    x = hand_new("2. bonum puella dominum amat", 0,
        "bonum puella dominum amat", 3, 0, 1, 0);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 3);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_disp(x, 1, 1, 0, 1);
    /* 3: D131's garden path */
    x = hand_new("3. bona puella dona amat", 0, "bona puella dona amat", 3, 0,
        1, 1);
    hand_word(x, rd_adj(0, NEUT, ACC, PL), 3);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(2, ACC, PL), 4);
    hand_word(x, rd_verb(0), 0);
    hand_disp(x, 1, 1, 1, 1);
    /* 4: crossed via the root arc */
    x = hand_new("4. regis puella amat dominum", 0,
        "regis puella amat dominum", 3, 1, 1, 1);
    hand_word(x, rd_noun(3, GEN, SG), 4);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 1, 0, 1, 1);
    /* 5: the golden line */
    x = hand_new("5. bona bonum amat puella dominum", 0,
        "bona bonum amat puella dominum", 4, 2, 2, 0);
    hand_word(x, rd_adj(0, FEM, NOM, SG), 4);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 5);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 1, 2, 0, 1);
    hand_disp(x, 2, 0, 0, 1);
    /* 6: competed, not misread (P empty) */
    x = hand_new("6. bona amat puella dona", 0, "bona amat puella dona", 3, 1,
        1, 0);
    hand_word(x, rd_adj(0, NEUT, ACC, PL), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(0, NOM, SG), 2);
    hand_word(x, rd_noun(2, ACC, PL), 2);
    hand_disp(x, 1, 0, 1, 1);
    /* 7: regis -> dominum, puella nearer */
    x = hand_new("7. puella regis amat dominum", 0,
        "puella regis amat dominum", 3, 1, 1, 1);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(3, GEN, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_disp(x, 2, 0, 1, 1);
    /* 8: head first; regis a backward attachment */
    x = hand_new("8. dominum amat regis puella", 0,
        "dominum amat regis puella", 2, 0, 0, 1);
    hand_word(x, rd_noun(1, ACC, SG), 2);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 1);
    hand_word(x, rd_noun(0, NOM, SG), 2);
    hand_disp(x, 3, -1, 1, 1);
    /* 11: the competitor rule skips d's own subtree (re-check L1):
     * puellarum depends on regis and is nearer than dominum, but would
     * close a cycle; puella is too far, amat a verb: CLEAR. puella and
     * dominum both come earlier and could take regis: INTERFERED. Every
     * analysis crosses the root arc */
    x = hand_new("11. puella dominum amat regis puellarum", 0,
        "puella dominum amat regis puellarum", 3, 0, 0, 0);
    hand_word(x, rd_noun(0, NOM, SG), 3);
    hand_word(x, rd_noun(1, ACC, SG), 3);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 2);
    hand_word(x, rd_noun(0, GEN, PL), 4);
    hand_disp(x, 4, -1, 0, 0);
    /* 12: INTERFERED only through a cycle - when regis arrives, the open
     * subject puellae (nom pl; gen sg reading) could depend on it, but
     * regis hangs on puellae; fortis (gen sg f) also fits puellae, nearer:
     * COMPETED; every analysis has fortis cross puellae's arc: read */
    x = hand_new("12. fortis puellae regis amant dominum", 0,
        "fortis puellae regis amant dominum", 3, 0, 1, 0);
    hand_word(x, rd_adj(1, MASC, GEN, SG), 3);
    hand_word(x, rd_noun(0, NOM, PL), 4);
    hand_word(x, rd_noun(3, GEN, SG), 2);
    hand_word(x, rd_verb(1), 0);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_disp(x, 1, 1, 1, 0);
    x->dns[0] = 1;
    x->has_ns = 1;
    /* six words (the analyser and the tree table take any length up to
     * MAXW, quick mode included), retrieval failing with competition clear. 9: dependent
     * first - when dominum arrives, the open subject puellae (nom pl) has
     * a genitive reading that could depend on it; puella is feminine, so
     * no nearer word can take bonum; one analysis, so R_P reads it */
    x = hand_new("9. bonum puellae dominum amant rege forti (six words)", 0,
        "bonum puellae dominum amant rege forti", 3, 0, 1, 0);
    hand_word(x, rd_adj(0, MASC, ACC, SG), 3);
    hand_word(x, rd_noun(0, NOM, PL), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(1), 0);
    hand_word(x, rd_noun(3, ABL, SG), 4);
    hand_word(x, rd_adj(1, MASC, ABL, SG), 5);
    hand_disp(x, 1, 1, 0, 0);
    /* 10: head first - rege, puella and dominum all come before regis and
     * could take it; none but dominum is as near (amat is a verb, fortis
     * in regis's own subtree); every analysis crosses the root arc, so P
     * is empty */
    x = hand_new("10. rege puella dominum amat regis fortis (six words)", 0,
        "rege puella dominum amat regis fortis", 4, 0, 0, 0);
    hand_word(x, rd_noun(3, ABL, SG), 4);
    hand_word(x, rd_noun(0, NOM, SG), 4);
    hand_word(x, rd_noun(1, ACC, SG), 4);
    hand_word(x, rd_verb(0), 0);
    hand_word(x, rd_noun(3, GEN, SG), 3);
    hand_word(x, rd_adj(1, MASC, GEN, SG), 5);
    hand_disp(x, 5, -1, 0, 0);
}

static void
part_hand_umbrae (void)
{
    int k, i;

    printf("\n--- hand-worked sentences: umbrae and labels ---\n");
    hand_sentences_umbrae();
    for (k = 0; k < n_handu; k++) {
        const HandU *x = &handu[k];
        int          str[MAXW], na, t = -1, ok, mask, rootd = -1, maxd = 0;
        char         text[200], msg[1000], lab[600];
        Analysis     truth;
        Readers      r;
        Umbrae       u;
        Labels       lb;

        text[0] = '\0';
        for (i = 0; i < x->n; i++) {
            str[i] = form_of[x->lv][x->rd[i]];
            truth.head[i] = x->head[i];
            truth.rd[i] = x->rd[i];
            if (i > 0) strcat(text, " ");
            strcat(text, form_text[x->lv][str[i]]);
        }
        na = analyse(x->lv, str, x->n, abuf, MAXA);
        for (i = 0; i < na; i++)
            if (analysis_key(&abuf[i], x->n) == analysis_key(&truth, x->n))
                t = i;
        if (t < 0) {
            snprintf(msg, sizeof msg, "%s: the truth is an analysis of %s",
                x->title, text);
            check(msg, 0);
            continue;
        }
        read_with(abuf, na, x->n, &r);
        build_umbrae(&truth, x->n, &u);
        for (i = 0; i < u.nu; i++) {
            if (u.u[i].type == U_ROOT) rootd = u.depth[i];
            if (u.depth[i] > maxd) maxd = u.depth[i];
        }
        mask = t_npmask[x->n][tree_index(truth.head, x->n)];
        label_arcs(x->lv, &truth, x->n, mask, &u, &lb);
        ok = strcmp(text, x->text) == 0 && u.width == x->width
            && rootd == x->root_depth && maxd == x->max_depth
            && lb.nd == x->nd && (!r.in_rp[t]) == x->misread;
        lab[0] = '\0';
        for (i = 0; i < lb.nd; i++) {
            int  d = lb.word[i];
            int  dd = u.umbra_of[d] < 0 ? -1 : u.depth[u.umbra_of[d]];
            char one[200];

            if (i >= x->nd || d != x->dword[i] || dd != x->ddepth[i]
                || lb.competed[i] != x->dcomp[i]
                || lb.retrieved[i] != x->dretr[i]
                || (x->has_ns && lb.retrieved_ns[i] != x->dns[i]))
                ok = 0;
            snprintf(one, sizeof one, "%s%s -> %s, %s, depth %d, %s, %s%s",
                i ? "; " : "", form_text[x->lv][str[d - 1]],
                form_text[x->lv][str[truth.head[d - 1] - 1]],
                lb.dep_first[i] ? "dependent first" : "head first", dd,
                lb.competed[i] ? "COMPETED" : "CLEAR",
                lb.retrieved[i] ? "RETRIEVED" : "INTERFERED",
                x->has_ns ? (lb.retrieved_ns[i] ? " (RETRIEVED without "
                "the cycle)" : " (INTERFERED without the cycle)") : "");
            strcat(lab, one);
        }
        snprintf(msg, sizeof msg, "%s: width %d, root depth %d, max depth %d; "
            "%s; %s by R_P", x->title, u.width, rootd, maxd,
            lb.nd ? lab : "no displaced arc", r.in_rp[t] ? "read"
            : "MISREAD");
        check(msg, ok);
    }
}

/* ================================================================
 * Measures (task 2): B in the competition x retrieval 2 x 2 by
 * stratum; width and depth; the shares of displaced arcs RETRIEVED
 * and CLEAR; the registered rules
 * ================================================================ */

#define NSTRATA 9                 /* kind x direction */
#define MAXSEL  4096
#define NV_FULL 1125              /* pinned totals, every level with B */
#define NV_SUB  837               /* pinned totals, B at L0 and L1 only */

enum { POP_ALL, POP_F2, NPOP };   /* crossed pairs; those with |F| >= 2 */
enum { RT_NEAR, RT_FLIP, RT_FAR, RT_OTHER, NROUTES };
enum { V_HOLDS, V_FAILS, V_UNDECIDED };

static const char *const kind_name[3] = {
    "adjectives only", "genitives only", "both"
};
static const char *const dir_name[3] = {
    "dependent first", "head first", "mixed"
};
static const char *const verdict_name[3] = { "HOLDS", "FAILS", "UNDECIDED" };
static const char *const route_name[NROUTES] = {
    "as near, outside d's subtree", "as near, inside it (reversed)",
    "farther", "displaced words kept"
};

typedef struct {
    /* crossed pairs: [population][stratum][competed][interfered]
     * [pairs, misread by R_P] */
    s64 cell[NPOP][NSTRATA][2][2][2];
    s64 cell_rel[NPOP][NSTRATA][2][2][2];   /* relation-specific labels */
    s64 blind[NPOP][NSTRATA][2][2];         /* [blind-competed][pairs,
                                               misread] */
    s64 route[NSTRATA][2][NROUTES];         /* misread pairs [competed]
                                               by route */
    s64 covered_clear;                      /* covered from outside d's
                                               subtree but CLEAR (0, a
                                               theorem) */
    /* displaced arcs [adjective, genitive][dependent first, head first]
     * [arcs, RETRIEVED, CLEAR, RETRIEVED by own relation, CLEAR by own
     * relation, INTERFERED only by candidates that close a cycle] */
    s64 arcs[2][2][6];
    /* pairs by width, umbrae by depth [stratum; NSTRATA = projective] */
    s64 width[NSTRATA + 1][MAXW + 2];
    s64 depth[NSTRATA + 1][MAXW + 2];
    s64 utype[3], back;
    /* T4 (review M2), filled by the L0 pass only: displaced arcs of
     * crossed pairs with |F(s)| >= 2 AT L0 (the registered population),
     * labelled at [level][adjective, genitive][dependent first, head
     * first][arcs, RETRIEVED] */
    s64 arcs_f2[NLEVELS][2][2][2];
    /* T4 (review M1), L0 only: the genitive cell (genitives only,
     * dependent first, COMPETED, |F| >= 2) by [interfered][the
     * interferers' true relations, bit r - 1][some P analysis hangs d on
     * an interferer][some hangs an interferer on h][pairs, misread] */
    s64 gdiag[2][64][2][2][2];
} Acc;

#define ACC_WORDS ((int)(sizeof(Acc) / sizeof(s64)))

static Acc acc[NLEVELS][MAXW + 1];        /* by level and exact length */
static Acc acc_sub[NLEVELS];              /* six words, the oracle's subset */
static i64 sel_codes[MAXSEL];             /* the subset's L1 bag codes */
static int n_sel = 0;
static UChecks ck_five, ck_six;

static int
subset_has (
    i64 code)
{
    int lo = 0, hi = n_sel - 1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        if (sel_codes[mid] == code) return 1;
        if (sel_codes[mid] < code) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* the L1 bag of an analysis: its readings, sorted (L1 form = reading) */
static i64
reading_code (
    const Analysis *a,
    int             n)
{
    int r[MAXW], k;

    for (k = 0; k < n; k++) r[k] = a->rd[k];
    qsort(r, (size_t)n, sizeof(int), int_cmp);
    return bag_code(r, n);
}

static int
stratum_of (
    const Analysis *a,
    const Labels   *lb)
{
    int i, nadj = 0, ngen = 0, ndf = 0, kind, dir;

    for (i = 0; i < lb->nd; i++) {
        if (readings[a->rd[lb->word[i] - 1]].kind == K_ADJ) nadj++;
        else ngen++;
        ndf += lb->dep_first[i];
    }
    kind = ngen == 0 ? 0 : nadj == 0 ? 1 : 2;
    dir = ndf == lb->nd ? 0 : ndf == 0 ? 1 : 2;
    return kind * 3 + dir;
}

/* how R_P's projective alternatives move the displaced words: some P
 * analysis puts a displaced dependent on another word at least as near
 * as its true head, outside its own subtree (NEAR) or inside it (FLIP:
 * the dependency reversed - a competitor the competition rule cannot
 * see, since attaching there in the TRUE tree would close a cycle; NEAR
 * and FLIP together are "covered"), only farther (FAR), or none moves a
 * displaced dependent (OTHER: another arc or a role changes). hp[d] =
 * the heads d takes across P(s), as a bitmask */
static int
route_of (
    const Analysis *a,
    int             n,
    const Labels   *lb,
    const int      *hp)
{
    int i, w, flip = 0, far = 0;

    for (i = 0; i < lb->nd; i++) {
        int d = lb->word[i], h = a->head[d - 1];
        int dist = h > d ? h - d : d - h;

        for (w = 1; w <= n; w++) {
            if (w == h || !(hp[d] & (1 << w))) continue;
            if ((w > d ? w - d : d - w) > dist) far = 1;
            else if (descends(a, w, d)) flip = 1;
            else return RT_NEAR;
        }
    }
    return flip ? RT_FLIP : far ? RT_FAR : RT_OTHER;
}

static void
acc_pair (
    Acc            *A,
    const Analysis *a,
    const Umbrae   *u,
    int             proj,
    const Labels   *lb,
    int             nf,
    int             misread,
    int             route)
{
    int s = NSTRATA, k, i, pp, c = 0, it = 0, cr = 0, ir = 0, cb = 0;

    if (!proj) {
        if (lb->nd == 0) fatal("a crossed pair without a displaced arc");
        s = stratum_of(a, lb);
    }
    A->width[s][u->width]++;
    for (k = 0; k < u->nu; k++) {
        A->depth[s][u->depth[k]]++;
        A->utype[u->u[k].type]++;
    }
    A->back += u->nback;
    if (proj) return;
    for (i = 0; i < lb->nd; i++) {
        s64 *x = A->arcs[readings[a->rd[lb->word[i] - 1]].kind == K_ADJ ? 0
            : 1][lb->dep_first[i] ? 0 : 1];

        x[0]++;
        x[1] += lb->retrieved[i];
        x[2] += !lb->competed[i];
        x[3] += lb->retrieved_rel[i];
        x[4] += !lb->competed_rel[i];
        x[5] += !lb->retrieved[i] && lb->retrieved_ns[i];
        c |= lb->competed[i];
        it |= !lb->retrieved[i];
        cr |= lb->competed_rel[i];
        ir |= !lb->retrieved_rel[i];
        cb |= lb->competed_blind[i];
    }
    for (pp = 0; pp < NPOP; pp++) {
        if (pp == POP_F2 && nf < 2) continue;
        A->cell[pp][s][c][it][0]++;
        A->cell[pp][s][c][it][1] += misread;
        A->cell_rel[pp][s][cr][ir][0]++;
        A->cell_rel[pp][s][cr][ir][1] += misread;
        A->blind[pp][s][cb][0]++;
        A->blind[pp][s][cb][1] += misread;
    }
    if (misread) {
        A->route[s][c][route]++;
        if (route == RT_NEAR && !c) A->covered_clear++;
    }
}

/* T4 (review M1): a pair of the genitive cell (genitives only,
 * dependent first, COMPETED; L0, |F| >= 2) keyed by its interferers -
 * the open dangling words that could also take the arriving head - by
 * their TRUE relation, and by whether a projective alternative (hp[x] =
 * the heads x takes across P(s)) hangs d on an interferer or an
 * interferer on h */
static void
genitive_cell (
    Acc            *A,
    const Analysis *a,
    int             n,
    const Umbrae   *u,
    const Labels   *lb,
    const int      *hp,
    int             misread)
{
    int i, k, c = 0, it = 0, roles = 0, pd = 0, px = 0, f[MAXW];

    if (stratum_of(a, lb) != 3) return;
    for (k = 0; k < n; k++) f[k] = form_of[0][a->rd[k]];
    for (i = 0; i < lb->nd; i++) {
        int d = lb->word[i], h = a->head[d - 1];

        c |= lb->competed[i];
        it |= !lb->retrieved[i];
        for (k = 0; k < u->nu; k++) {
            const Umbra *x = &u->u[k];

            if (x->type != U_DANGLING || x->from > h - 1 || x->to <= h - 1
                || x->to == h || !comp_any[0][f[x->word - 1]][f[h - 1]])
                continue;
            roles |= 1 << (relation_of(a, x->word) - 1);
            if (hp[d] & (1 << x->word)) pd = 1;
            if (hp[x->word] & (1 << h)) px = 1;
        }
    }
    if (!c) return;
    A->gdiag[it][roles][pd][px][0]++;
    A->gdiag[it][roles][pd][px][1] += misread;
}

/* every pair of lo..hi words at level lv, by that level's own bags (so
 * F(s) and P(s) are the string's own) */
static void
measure_umbrae (
    int lv,
    int lo,
    int hi)
{
    static unsigned char sel[MAXA];
    long                 i = 0;

    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        if (nb < lo || nb > hi) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (a = 0; a < nc; a++)
            sel[a] = (unsigned char)(nb == MAXW && n_sel > 0
                && subset_has(reading_code(&canon[a], nb)));
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW], hp[MAXW + 1], d;

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++)
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                    mapped[a].rd[inv[k]] = canon[a].rd[k];
                }
            read_with(mapped, nc, nb, &rdr);
            for (d = 0; d <= nb; d++) hp[d] = 0;
            for (a = 0; a < nc; a++)
                if (rdr.in_p[a])
                    for (d = 1; d <= nb; d++)
                        hp[d] |= 1 << mapped[a].head[d - 1];
            for (a = 0; a < nc; a++) {
                Umbrae u;
                Labels lb;
                int    ix = tree_index(mapped[a].head, nb);
                int    proj = t_proj[nb][ix], mis = 0, route = RT_OTHER;

                build_umbrae(&mapped[a], nb, &u);
                lb.nd = 0;
                if (!proj) {
                    label_arcs(lv, &mapped[a], nb, t_npmask[nb][ix], &u, &lb);
                    mis = rdr.np > 0;
                    if (mis) route = route_of(&mapped[a], nb, &lb, hp);
                }
                acc_pair(&acc[lv][nb], &mapped[a], &u, proj, &lb, nc, mis,
                    route);
                if (lv == 0 && !proj && nc >= 2) {
                    int lv2, i2;

                    genitive_cell(&acc[lv][nb], &mapped[a], nb, &u, &lb, hp,
                        mis);
                    for (lv2 = 0; lv2 < NLEVELS; lv2++) {
                        Labels lb2;

                        label_arcs(lv2, &mapped[a], nb, t_npmask[nb][ix], &u,
                            &lb2);
                        for (i2 = 0; i2 < lb2.nd; i2++) {
                            s64 *y = acc[lv][nb].arcs_f2[lv2][readings[
                                mapped[a].rd[lb2.word[i2] - 1]].kind == K_ADJ
                                ? 0 : 1][lb2.dep_first[i2] ? 0 : 1];

                            y[0]++;
                            y[1] += lb2.retrieved[i2];
                        }
                    }
                }
                if (sel[a])
                    acc_pair(&acc_sub[lv], &mapped[a], &u, proj, &lb, nc, mis,
                        route);
            }
        }
        i = j;
    }
}

static void
acc_add (
    Acc       *dst,
    const Acc *src)
{
    s64       *x = (s64 *)dst;
    const s64 *y = (const s64 *)src;
    int        k;

    for (k = 0; k < ACC_WORDS; k++) x[k] += y[k];
}

/* the pinned totals in one fixed order (oracle_d134.py prints the same):
 * B cells at the first nb levels; at L0 the relation-specific cells, the
 * blind split, the routes and covered-but-CLEAR; the arc labels at every
 * level; at L0 width, depth, umbra types and backward attachments */
static int
flatten (
    const Acc *const *lv4,
    int               nb,
    s64              *out)
{
    int m = 0, lv, pp, s, c, i, k, r;

    for (lv = 0; lv < nb; lv++)
        for (pp = 0; pp < NPOP; pp++)
            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++)
                        for (k = 0; k < 2; k++)
                            out[m++] = lv4[lv]->cell[pp][s][c][i][k];
    for (pp = 0; pp < NPOP; pp++)
        for (s = 0; s < NSTRATA; s++)
            for (c = 0; c < 2; c++)
                for (i = 0; i < 2; i++)
                    for (k = 0; k < 2; k++)
                        out[m++] = lv4[0]->cell_rel[pp][s][c][i][k];
    for (pp = 0; pp < NPOP; pp++)
        for (s = 0; s < NSTRATA; s++)
            for (c = 0; c < 2; c++)
                for (k = 0; k < 2; k++)
                    out[m++] = lv4[0]->blind[pp][s][c][k];
    for (s = 0; s < NSTRATA; s++)
        for (c = 0; c < 2; c++)
            for (r = 0; r < NROUTES; r++)
                out[m++] = lv4[0]->route[s][c][r];
    out[m++] = lv4[0]->covered_clear;
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (r = 0; r < 6; r++)
                    out[m++] = lv4[lv]->arcs[k][i][r];
    for (s = 0; s <= NSTRATA; s++)
        for (k = 0; k < MAXW + 2; k++)
            out[m++] = lv4[0]->width[s][k];
    for (s = 0; s <= NSTRATA; s++)
        for (k = 0; k < MAXW + 2; k++)
            out[m++] = lv4[0]->depth[s][k];
    for (k = 0; k < 3; k++) out[m++] = lv4[0]->utype[k];
    out[m++] = lv4[0]->back;
    return m;
}

/* the registered rules' inputs back from a pinned vector: L0 cells, the
 * arc labels at every level */
static void
unflatten (
    const s64 *v,
    int        nb,
    Acc       *lv4)
{
    int m = 0, lv, pp, s, c, i, k, r;

    for (lv = 0; lv < NLEVELS; lv++) memset(&lv4[lv], 0, sizeof(Acc));
    for (lv = 0; lv < nb; lv++)
        for (pp = 0; pp < NPOP; pp++)
            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++)
                        for (k = 0; k < 2; k++)
                            lv4[lv].cell[pp][s][c][i][k] = v[m++];
    m += 2 * NSTRATA * 8 + 2 * NSTRATA * 4 + NSTRATA * 2 * NROUTES + 1;
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (r = 0; r < 6; r++)
                    lv4[lv].arcs[k][i][r] = v[m++];
}

/* ---------------- the registered rules ---------------- */

/* a group is testable with >= 1,000 pairs and >= 2% of the set it is
 * compared within */
static int
testable (
    s64 n,
    s64 within)
{
    return n >= 1000 && 50 * n >= within;
}

static Fractio
share (
    s64      num,
    s64      den,
    Piscina *p)
{
    Fractio f = fractio_ex_s64(0);

    if (den > 0) (void)fractio_ex_s64_s64(num, den, p, &f);
    return f;
}

/* sign of (an/ad - bn/bd) - k/100, exactly; absolute gap if ab */
static int
gap_cmp (
    s64      an,
    s64      ad,
    s64      bn,
    s64      bd,
    s64      k,
    int      ab,
    Piscina *p)
{
    Fractio g = fractio_subtrahe(share(an, ad, p), share(bn, bd, p), p);
    Fractio t = fractio_ex_s64(0);

    if (ab) g = fractio_absolutum(g, p);
    (void)fractio_ex_s64_s64(k, 100, p, &t);
    return (int)fractio_compara(g, t, p);
}

/* U2a: per stratum g = { misread competed, competed, misread clear,
 * clear, stratum }; in every stratum where both groups are testable
 * B(competed) - B(clear) >= 10 points; three such strata or UNDECIDED */
static int
u2a_rule (
    s64 const (*g)[5],
    int        ns,
    int       *ntest,
    Piscina   *p)
{
    int s, fail = 0;

    *ntest = 0;
    for (s = 0; s < ns; s++) {
        if (!testable(g[s][1], g[s][4]) || !testable(g[s][3], g[s][4]))
            continue;
        (*ntest)++;
        if (gap_cmp(g[s][0], g[s][1], g[s][2], g[s][3], 10, 0, p) < 0)
            fail = 1;
    }
    if (*ntest < 3) return V_UNDECIDED;
    return fail ? V_FAILS : V_HOLDS;
}

/* U2b: per cell g = { misread interfered, interfered, misread retrieved,
 * retrieved, cell }; a cell COUNTS if both groups are testable and
 * B(retrieved) is in [10%, 90%]; every counting cell |B(interfered) -
 * B(retrieved)| <= 5 points; two counting cells or UNDECIDED */
static int
counting_cell (
    const s64 *g)
{
    return testable(g[1], g[4]) && testable(g[3], g[4])
        && 10 * g[2] >= g[3] && 10 * g[2] <= 9 * g[3];
}

static int
u2b_rule (
    s64 const (*g)[5],
    int        ncells,
    int       *ncount,
    Piscina   *p)
{
    int c, fail = 0;

    *ncount = 0;
    for (c = 0; c < ncells; c++) {
        if (!counting_cell(g[c])) continue;
        (*ncount)++;
        if (gap_cmp(g[c][0], g[c][1], g[c][2], g[c][3], 5, 1, p) > 0)
            fail = 1;
    }
    if (*ncount < 2) return V_UNDECIDED;
    return fail ? V_FAILS : V_HOLDS;
}

/* U1 over N displaced adjective arcs, r_L of them RETRIEVED at L (one
 * denominator): I(L0) - I(L2) >= 2 (I(L1) - I(L0)) and I(L0) - I(L2) >=
 * 5 points */
static int
u1_rule (
    s64 r0,
    s64 r1,
    s64 r2,
    s64 n)
{
    if (n == 0) return V_UNDECIDED;
    return r0 - r2 >= 2 * (r1 - r0) && 20 * (r0 - r2) >= n ? V_HOLDS
        : V_FAILS;
}

/* the rules' inputs from per-level totals (registered population: L0,
 * |F| >= 2) */
static void
rule_inputs (
    const Acc *const *lv4,
    s64             (*ga)[5],
    s64             (*gb)[5],
    s64              *u1)
{
    const Acc *A = lv4[0];
    int        s, c, i, kd, lv;

    for (s = 0; s < NSTRATA; s++) {
        for (i = 0; i < 5; i++) ga[s][i] = 0;
        for (i = 0; i < 2; i++) {
            ga[s][0] += A->cell[POP_F2][s][1][i][1];
            ga[s][1] += A->cell[POP_F2][s][1][i][0];
            ga[s][2] += A->cell[POP_F2][s][0][i][1];
            ga[s][3] += A->cell[POP_F2][s][0][i][0];
        }
        ga[s][4] = ga[s][1] + ga[s][3];
    }
    /* U2b: dependent-first strata (dir 0), COMPETED then CLEAR */
    for (kd = 0; kd < 3; kd++)
        for (c = 0; c < 2; c++) {
            s64 *g = gb[kd * 2 + c];
            int  cc = 1 - c;

            s = kd * 3;
            g[0] = A->cell[POP_F2][s][cc][1][1];
            g[1] = A->cell[POP_F2][s][cc][1][0];
            g[2] = A->cell[POP_F2][s][cc][0][1];
            g[3] = A->cell[POP_F2][s][cc][0][0];
            g[4] = g[1] + g[3];
        }
    /* U1: adjective arcs, both directions, RETRIEVED at L0, L1, L2 */
    u1[3] = lv4[0]->arcs[0][0][0] + lv4[0]->arcs[0][1][0];
    for (lv = 0; lv < 3; lv++)
        u1[lv] = lv4[lv]->arcs[0][0][1] + lv4[lv]->arcs[0][1][1];
}

static void
decide (
    const Acc *const *lv4,
    int              *v,
    Piscina          *p)
{
    s64 ga[NSTRATA][5], gb[6][5], u1[4];
    int nt, ncount;

    rule_inputs(lv4, ga, gb, u1);
    v[0] = u2a_rule((s64 const (*)[5])ga, NSTRATA, &nt, p);
    v[1] = u2b_rule((s64 const (*)[5])gb, 6, &ncount, p);
    v[2] = u1_rule(u1[0], u1[1], u1[2], u1[3]);
}

static void
rules_check (void)
{
    Piscina  *p = piscina_generare_dynamicum("d134_regulae", 1 << 16);
    s64       a[3][5], b[3][5];
    int       nt, k;

    printf("\n--- the registered rules on constructed totals ---\n");
    check("testable: 1,000 of 50,000 (exactly 2%) yes; 999 of 10,000 no; "
        "1,000 of 50,001 no", testable(1000, 50000) && !testable(999, 10000)
        && !testable(1000, 50001));
    for (k = 0; k < 3; k++) {
        a[k][0] = 300; a[k][1] = 1000; a[k][2] = 200; a[k][3] = 1000;
        a[k][4] = 2000;
    }
    check("U2a: three testable strata, each gap exactly 10 points, HOLDS",
        u2a_rule((s64 const (*)[5])a, 3, &nt, p) == V_HOLDS && nt == 3);
    a[2][2] = 201;
    check("U2a: one stratum at 9.9 points FAILS", u2a_rule((s64 const (*)[5])a,
        3, &nt, p) == V_FAILS);
    a[2][2] = 200;
    a[1][1] = 999;
    check("U2a: a group of 999 pairs is not testable: two strata, UNDECIDED "
        "(even with a failing one)", u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_UNDECIDED && nt == 2);
    a[1][1] = 1000;
    a[0][0] = 1100; a[0][1] = 2000; a[0][2] = 450; a[0][3] = 1000;
    a[0][4] = 3000;
    check("U2a: 55% of 2,000 against 45% of 1,000 is exactly 10 points, "
        "HOLDS; 1,099 of 2,000 FAILS", u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_HOLDS && (a[0][0] = 1099, u2a_rule((s64 const (*)[5])a, 3, &nt, p)
        == V_FAILS));
    for (k = 0; k < 3; k++) {
        b[k][0] = 150; b[k][1] = 1000; b[k][2] = 100; b[k][3] = 1000;
        b[k][4] = 2000;
    }
    check("U2b: B(retrieved) exactly 10%, gap exactly 5 points, three "
        "counting cells, HOLDS", u2b_rule((s64 const (*)[5])b, 3, &nt, p)
        == V_HOLDS && nt == 3);
    b[1][0] = 151;
    check("U2b: one counting cell at 5.1 points FAILS",
        u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_FAILS);
    b[1][0] = 49;
    check("U2b: the gap is absolute (4.9% against 10%: 5.1 points) FAILS",
        u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_FAILS);
    b[1][0] = 150;
    b[0][2] = 99; b[1][2] = 99;
    check("U2b: B(retrieved) 9.9% does not count: one counting cell, "
        "UNDECIDED", u2b_rule((s64 const (*)[5])b, 3, &nt, p) == V_UNDECIDED
        && nt == 1);
    b[0][2] = 900; b[0][0] = 950; b[1][2] = 901; b[1][0] = 951;
    check("U2b: B(retrieved) exactly 90% counts, 90.1% does not",
        counting_cell(b[0]) && !counting_cell(b[1]));
    check("U1: 550, 600, 450 of 1,000 (both clauses at equality but the "
        "floor) HOLDS; 451 at L2 FAILS", u1_rule(550, 600, 450, 1000)
        == V_HOLDS && u1_rule(550, 600, 451, 1000) == V_FAILS);
    check("U1: a gap of exactly 5 points (100 of 2,000) HOLDS, 99 FAILS; no "
        "arcs UNDECIDED", u1_rule(550, 550, 450, 2000) == V_HOLDS
        && u1_rule(550, 550, 451, 2000) == V_FAILS
        && u1_rule(0, 0, 0, 0) == V_UNDECIDED);
    {
        static Acc t[NLEVELS];
        const Acc *t4[NLEVELS];
        s64        ga[NSTRATA][5], gb[6][5], u1[4];
        int        s2, c2, i2, k2, lv, kd, ok = 1;

        memset(t, 0, sizeof t);
        for (s2 = 0; s2 < NSTRATA; s2++)
            for (c2 = 0; c2 < 2; c2++)
                for (i2 = 0; i2 < 2; i2++)
                    for (k2 = 0; k2 < 2; k2++) {
                        t[0].cell[POP_F2][s2][c2][i2][k2] = 1000 * s2
                            + 100 * c2 + 10 * i2 + k2 + 1;
                        t[0].cell[POP_ALL][s2][c2][i2][k2] = 7;
                    }
        for (lv = 0; lv < NLEVELS; lv++) {
            t[lv].arcs[0][0][0] = 50;
            t[lv].arcs[0][1][0] = 30;
            t[lv].arcs[0][0][1] = 10 + lv;
            t[lv].arcs[0][1][1] = 20 + lv;
            t[lv].arcs[1][0][0] = t[lv].arcs[1][1][0] = 999;
            t[lv].arcs[1][0][1] = t[lv].arcs[1][1][1] = 777;
            t4[lv] = &t[lv];
        }
        rule_inputs(t4, ga, gb, u1);
        for (s2 = 0; s2 < NSTRATA; s2++) {
            const s64 (*x)[2][2] = t[0].cell[POP_F2][s2];

            if (ga[s2][0] != x[1][0][1] + x[1][1][1]
                || ga[s2][1] != x[1][0][0] + x[1][1][0]
                || ga[s2][2] != x[0][0][1] + x[0][1][1]
                || ga[s2][3] != x[0][0][0] + x[0][1][0]
                || ga[s2][4] != ga[s2][1] + ga[s2][3])
                ok = 0;
        }
        for (kd = 0; kd < 3; kd++)
            for (c2 = 0; c2 < 2; c2++) {
                const s64 (*x)[2] = t[0].cell[POP_F2][kd * 3][1 - c2];
                const s64 *g = gb[kd * 2 + c2];

                if (g[0] != x[1][1] || g[1] != x[1][0] || g[2] != x[0][1]
                    || g[3] != x[0][0] || g[4] != g[1] + g[3])
                    ok = 0;
            }
        if (u1[3] != 80 || u1[0] != 30 || u1[1] != 32 || u1[2] != 34)
            ok = 0;
        check("rule inputs from constructed totals: U2a sums each stratum's "
            "competed and clear cells (|F| >= 2), U2b reads the "
            "dependent-first strata (competed, then clear), U1 the "
            "adjective arcs at L0, L1, L2", ok);
    }
    piscina_destruere(p);
}

/* oracle_d134.py's totals in flatten's order (generated by the oracle,
 * pasted): [up to five words, up to four words (quick mode)], and the
 * six-word subset (B at L0 and L1) */
static const s64 oracle_full[2][NV_FULL] = {
    { /* python3 -I oracle_d134.py 5 */
      278558, 114, 8272, 115, 38430, 19447, 2932, 1574, 274428, 224, 12402, 5,
      9632, 5844, 31730, 15177, 5695, 0, 201, 0, 681, 386, 591, 202, 88960,
      768, 0, 0, 1191936, 985408, 93312, 69632, 0, 0, 88960, 768, 74624,
      67648, 1210624, 987392, 0, 0, 0, 0, 14336, 384, 346112, 218624, 11812,
      0, 208, 52, 34636, 5042, 14784, 7391, 0, 0, 12020, 52, 6280, 572, 43140,
      11861, 5728, 0, 6024, 0, 58732, 13506, 52396, 8164, 85152, 114, 2138,
      115, 33196, 19447, 2294, 1574, 76568, 224, 10722, 5, 7964, 5844, 27526,
      15177, 136, 0, 20, 0, 646, 386, 574, 202, 88960, 768, 0, 0, 1191936,
      985408, 93312, 69632, 0, 0, 88960, 768, 74624, 67648, 1210624, 987392,
      0, 0, 0, 0, 14336, 384, 346112, 218624, 11812, 0, 208, 52, 34636, 5042,
      14784, 7391, 0, 0, 12020, 52, 6280, 572, 43140, 11861, 5728, 0, 6024, 0,
      58732, 13506, 52396, 8164, 328192, 0, 0, 0, 0, 0, 0, 0, 328192, 0, 0, 0,
      0, 0, 0, 0, 7168, 0, 0, 0, 0, 0, 0, 0, 103424, 0, 0, 0, 1221632,
      1003520, 49152, 24576, 0, 0, 103424, 0, 87040, 78848, 1183744, 949248,
      0, 0, 0, 0, 16384, 0, 344064, 212992, 16384, 0, 0, 0, 32768, 0, 12288,
      6144, 0, 0, 16384, 0, 8192, 0, 36864, 6144, 8192, 0, 8192, 0, 69632,
      12288, 36864, 0, 86016, 0, 0, 0, 0, 0, 0, 0, 86016, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 103424, 0, 0, 0, 1221632, 1003520, 49152, 24576,
      0, 0, 103424, 0, 87040, 78848, 1183744, 949248, 0, 0, 0, 0, 16384, 0,
      344064, 212992, 16384, 0, 0, 0, 32768, 0, 12288, 6144, 0, 0, 16384, 0,
      8192, 0, 36864, 6144, 8192, 0, 8192, 0, 69632, 12288, 36864, 0, 167648,
      16044, 45596, 17186, 88952, 60784, 25996, 23778, 183736, 28788, 29508,
      4442, 24520, 21424, 90428, 63138, 3580, 0, 1444, 0, 860, 660, 1284, 620,
      0, 0, 0, 0, 1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208,
      1374208, 0, 0, 0, 0, 0, 0, 360448, 360448, 1124, 0, 1124, 1002, 37116,
      15896, 22076, 14766, 0, 0, 2248, 1002, 0, 0, 59192, 30662, 0, 0, 0, 0,
      33928, 13832, 88952, 47984, 159220, 16044, 44008, 17186, 88952, 60784,
      25996, 23778, 173720, 28788, 29508, 4442, 24520, 21424, 90428, 63138,
      1140, 0, 468, 0, 860, 660, 1284, 620, 0, 0, 0, 0, 1033216, 1033216,
      340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0, 0, 0, 0, 0, 0,
      360448, 360448, 1124, 0, 1124, 1002, 37116, 15896, 22076, 14766, 0, 0,
      2248, 1002, 0, 0, 59192, 30662, 0, 0, 0, 0, 33928, 13832, 88952, 47984,
      23296, 0, 0, 0, 231424, 187136, 73472, 71936, 0, 0, 23296, 0, 19200,
      19200, 285696, 239872, 0, 0, 0, 0, 2560, 2560, 4608, 2560, 0, 0, 0, 0,
      1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0,
      0, 0, 0, 0, 0, 360448, 360448, 0, 0, 0, 0, 30720, 26624, 30720, 28672,
      0, 0, 0, 0, 0, 0, 61440, 55296, 0, 0, 0, 0, 6144, 6144, 116736, 104448,
      23296, 0, 0, 0, 231424, 187136, 73472, 71936, 0, 0, 23296, 0, 19200,
      19200, 285696, 239872, 0, 0, 0, 0, 2560, 2560, 4608, 2560, 0, 0, 0, 0,
      1033216, 1033216, 340992, 340992, 0, 0, 0, 0, 0, 0, 1374208, 1374208, 0,
      0, 0, 0, 0, 0, 360448, 360448, 0, 0, 0, 0, 30720, 26624, 30720, 28672,
      0, 0, 0, 0, 0, 0, 61440, 55296, 0, 0, 0, 0, 6144, 6144, 116736, 104448,
      286830, 229, 0, 0, 40930, 20823, 432, 198, 274428, 224, 12402, 5, 9632,
      5844, 31730, 15177, 5876, 0, 20, 0, 716, 404, 556, 184, 103424, 8528, 0,
      0, 1177472, 977648, 93312, 69632, 0, 0, 103424, 8528, 87040, 79296,
      1183744, 967984, 0, 0, 0, 0, 16384, 512, 344064, 218496, 12924, 156, 0,
      0, 47748, 11925, 768, 404, 0, 0, 12924, 156, 6982, 669, 41534, 11660,
      6294, 52, 6706, 52, 59958, 14370, 49922, 7196, 87290, 229, 0, 0, 35070,
      20823, 420, 198, 76568, 224, 10722, 5, 7964, 5844, 27526, 15177, 136, 0,
      20, 0, 680, 404, 540, 184, 103424, 8528, 0, 0, 1177472, 977648, 93312,
      69632, 0, 0, 103424, 8528, 87040, 79296, 1183744, 967984, 0, 0, 0, 0,
      16384, 512, 344064, 218496, 12924, 156, 0, 0, 47748, 11925, 768, 404, 0,
      0, 12924, 156, 6982, 669, 41534, 11660, 6294, 52, 6706, 52, 59958,
      14370, 49922, 7196, 23296, 0, 304896, 21250, 23296, 0, 304896, 21250, 0,
      0, 7168, 588, 103424, 8528, 1270784, 1047280, 103424, 8528, 1270784,
      1047280, 0, 0, 360448, 219008, 0, 0, 61440, 12485, 0, 0, 61440, 12485,
      0, 0, 122880, 21670, 10548, 0, 112232, 21250, 10548, 0, 112232, 21250,
      0, 0, 1376, 588, 103424, 8528, 1270784, 1047280, 103424, 8528, 1270784,
      1047280, 0, 0, 360448, 219008, 0, 0, 61440, 12485, 0, 0, 61440, 12485,
      0, 0, 122880, 21670, 0, 0, 0, 229, 21018, 0, 0, 3, 0, 0, 0, 229, 21018,
      0, 0, 3, 0, 0, 0, 0, 588, 0, 0, 0, 0, 768, 0, 0, 1054080, 768, 192, 0,
      0, 768, 0, 0, 1054080, 768, 192, 0, 0, 0, 0, 0, 219008, 0, 0, 0, 0, 0,
      52, 0, 12381, 0, 52, 0, 0, 0, 52, 0, 12381, 0, 52, 0, 0, 0, 0, 0, 21534,
      0, 136, 0, 0, 465408, 438512, 398630, 464976, 398630, 1536, 465408,
      398630, 398630, 398630, 398630, 0, 1923072, 1814208, 139648, 1815936,
      160768, 6144, 1923072, 118144, 139648, 136192, 160768, 21504, 465408,
      453120, 465408, 465408, 465408, 0, 465408, 465408, 465408, 465408,
      465408, 0, 1923072, 1873920, 160768, 1873920, 160768, 0, 1923072,
      136192, 160768, 136192, 160768, 24576, 465408, 358352, 301128, 464592,
      301128, 4096, 465408, 301128, 301128, 301128, 301128, 0, 1923072,
      1484608, 4096, 1487872, 160768, 16384, 1923072, 4096, 4096, 136192,
      160768, 0, 465408, 356096, 31488, 462336, 31488, 4096, 465408, 31488,
      31488, 31488, 31488, 0, 1923072, 1475584, 4096, 1487872, 160768, 16384,
      1923072, 4096, 4096, 136192, 160768, 0, 0, 0, 12288, 190976, 124928, 0,
      0, 0, 0, 0, 115968, 138496, 73728, 0, 0, 0, 0, 0, 1024, 6144, 0, 0, 0,
      0, 0, 0, 49152, 833536, 491520, 0, 0, 0, 0, 0, 496640, 572416, 305152,
      0, 0, 0, 0, 0, 32768, 262144, 65536, 0, 0, 0, 0, 0, 0, 12288, 49152, 0,
      0, 0, 0, 0, 26624, 34816, 0, 0, 0, 0, 0, 0, 12288, 94208, 16384, 0, 0,
      0, 0, 0, 903680, 1110912, 572416, 147456, 0, 0, 1039872, 448512, 30208,
      0, 0, 0, 0, 0, 1216000, 0, 0, 0, 0, 0, 0, 0, 20992, 7680, 0, 0, 0, 0, 0,
      0, 4321280, 1908736, 139264, 0, 0, 0, 0, 0, 5060608, 0, 0, 0, 0, 0, 0,
      0, 950272, 483328, 8192, 0, 0, 0, 0, 0, 184320, 75776, 47104, 0, 0, 0,
      0, 0, 184320, 0, 0, 0, 0, 0, 0, 0, 331776, 157696, 2048, 0, 0, 0, 0, 0,
      11538048, 0, 0, 0, 0, 0, 0, 0, 13477248, 7926144, 6752640, 5551104, },
    { /* python3 -I oracle_d134.py 4 */
      1330, 0, 26, 0, 174, 130, 6, 6, 1320, 0, 36, 0, 36, 34, 144, 102, 0, 0,
      0, 0, 0, 0, 0, 0, 896, 0, 0, 0, 5120, 4096, 128, 128, 0, 0, 896, 0, 896,
      896, 4352, 3328, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 164, 130, 6, 6,
      36, 0, 34, 0, 34, 34, 136, 102, 0, 0, 0, 0, 0, 0, 0, 0, 896, 0, 0, 0,
      5120, 4096, 128, 128, 0, 0, 896, 0, 896, 896, 4352, 3328, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 1536, 0, 0, 0, 0, 0, 0, 0, 1536, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1024, 0, 0, 0, 5120, 4096, 0, 0, 0, 0, 1024, 0, 1024,
      1024, 4096, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 0, 0, 0, 5120, 4096, 0,
      0, 0, 0, 1024, 0, 1024, 1024, 4096, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1008,
      0, 188, 0, 272, 204, 68, 68, 1128, 0, 68, 0, 68, 68, 272, 204, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0,
      6144, 6144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 388, 0, 64, 0, 272, 204, 68, 68,
      384, 0, 68, 0, 68, 68, 272, 204, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0, 6144, 6144, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 256, 0, 0, 0, 1024, 768, 256, 256, 0, 0, 256, 0, 256, 256, 1024,
      768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5120, 5120, 1024, 1024, 0, 0,
      0, 0, 0, 0, 6144, 6144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 0, 0, 1024, 768,
      256, 256, 0, 0, 256, 0, 256, 256, 1024, 768, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 5120, 5120, 1024, 1024, 0, 0, 0, 0, 0, 0, 6144, 6144, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 1356, 0, 0, 0, 180, 136, 0, 0, 1320, 0, 36, 0, 36, 34,
      144, 102, 0, 0, 0, 0, 0, 0, 0, 0, 1024, 64, 0, 0, 4992, 4032, 128, 128,
      0, 0, 1024, 64, 1024, 1024, 4096, 3136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0,
      0, 170, 136, 0, 0, 36, 0, 34, 0, 34, 34, 136, 102, 0, 0, 0, 0, 0, 0, 0,
      0, 1024, 64, 0, 0, 4992, 4032, 128, 128, 0, 0, 1024, 64, 1024, 1024,
      4096, 3136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 256, 0, 1280, 136, 256, 0, 1280,
      136, 0, 0, 0, 0, 1024, 64, 5120, 4160, 1024, 64, 5120, 4160, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 200, 136, 40, 0, 200, 136, 0,
      0, 0, 0, 1024, 64, 5120, 4160, 1024, 64, 5120, 4160, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 136, 0, 0, 0, 0, 0, 0, 0, 136,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4224, 0, 0, 0, 0, 0, 0, 0,
      4224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1536, 1504, 1356, 1536, 1356,
      0, 1536, 1356, 1356, 1356, 1356, 0, 6144, 6016, 896, 6016, 1024, 0,
      6144, 896, 896, 1024, 1024, 0, 1536, 1536, 1536, 1536, 1536, 0, 1536,
      1536, 1536, 1536, 1536, 0, 6144, 6144, 1024, 6144, 1024, 0, 6144, 1024,
      1024, 1024, 1024, 0, 1536, 1280, 1196, 1536, 1196, 0, 1536, 1196, 1196,
      1196, 1196, 0, 6144, 5120, 0, 5120, 1024, 0, 6144, 0, 0, 1024, 1024, 0,
      1536, 1280, 256, 1536, 256, 0, 1536, 256, 256, 256, 256, 0, 6144, 5120,
      0, 5120, 1024, 0, 6144, 0, 0, 1024, 1024, 0, 0, 0, 0, 1536, 0, 0, 0, 0,
      0, 0, 768, 768, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6144, 0, 0,
      0, 0, 0, 0, 3072, 3072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18432,
      15744, 6144, 0, 0, 0, 4096, 2048, 0, 0, 0, 0, 0, 0, 4608, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16384, 8192, 0, 0, 0, 0, 0, 0, 18432, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147072, 0, 0, 0, 0, 0, 0, 0,
      83328, 61824, 55680, 21504, },
};
static const s64 oracle_sub[NV_SUB] = {
    /* python3 -I oracle_d134.py 6 sub */
    211161, 1361, 10732, 1419, 56316, 23514, 6647, 2961, 201369, 2646, 20524,
    134, 14151, 7300, 48812, 19175, 12771, 0, 1529, 0, 3467, 402, 3437, 360,
    48659, 2773, 44, 27, 1120567, 941710, 240782, 179060, 1773, 1773, 46930,
    1027, 33013, 30155, 1328336, 1090615, 0, 0, 0, 0, 18970, 3747, 1167054,
    779641, 17646, 8, 557, 74, 110346, 11883, 65971, 21635, 172, 0, 18031, 82,
    6157, 331, 170160, 33187, 8471, 6, 9391, 6, 158455, 23032, 324131, 56772,
    165127, 1361, 8932, 1419, 52582, 23514, 5767, 2961, 155063, 2646, 18996,
    134, 12877, 7300, 45472, 19175, 4442, 0, 762, 0, 2039, 402, 2313, 360,
    48659, 2773, 44, 27, 1120567, 941710, 240782, 179060, 1773, 1773, 46930,
    1027, 33013, 30155, 1328336, 1090615, 0, 0, 0, 0, 18970, 3747, 1167054,
    779641, 17646, 8, 557, 74, 110346, 11883, 65971, 21635, 172, 0, 18031, 82,
    6157, 331, 170160, 33187, 8471, 6, 9391, 6, 158455, 23032, 324131, 56772,
    280204, 768, 1536, 768, 2804, 1088, 312, 156, 280124, 1536, 1616, 0, 612,
    356, 2504, 888, 21204, 0, 0, 0, 0, 0, 0, 0, 58370, 2106, 0, 0, 1166430,
    963562, 185252, 115554, 2106, 2106, 56264, 0, 39722, 36184, 1311960,
    1042932, 0, 0, 0, 0, 22532, 3186, 1163492, 737970, 25722, 0, 128, 0,
    112802, 780, 55868, 15208, 272, 0, 25578, 0, 8662, 0, 160008, 15988,
    13314, 0, 13338, 0, 201978, 20238, 271818, 21026, 169692, 768, 1536, 768,
    2804, 1088, 312, 156, 169612, 1536, 1616, 0, 612, 356, 2504, 888, 6368, 0,
    0, 0, 0, 0, 0, 0, 58370, 2106, 0, 0, 1166430, 963562, 185252, 115554,
    2106, 2106, 56264, 0, 39722, 36184, 1311960, 1042932, 0, 0, 0, 0, 22532,
    3186, 1163492, 737970, 25722, 0, 128, 0, 112802, 780, 55868, 15208, 272,
    0, 25578, 0, 8662, 0, 160008, 15988, 13314, 0, 13338, 0, 201978, 20238,
    271818, 21026, 221857, 2774, 36, 6, 61987, 26267, 976, 208, 201369, 2646,
    20524, 134, 14151, 7300, 48812, 19175, 13451, 0, 849, 0, 3898, 438, 3006,
    324, 58370, 8899, 0, 0, 1111010, 935704, 240672, 178967, 2106, 2106,
    56264, 6793, 39722, 36481, 1311960, 1078190, 0, 0, 0, 0, 22532, 4623,
    1163492, 778765, 20043, 203, 0, 0, 162424, 30092, 12053, 3305, 189, 0,
    19854, 203, 7105, 431, 167372, 32966, 9620, 81, 10804, 85, 163468, 24329,
    316556, 55321, 174023, 2774, 36, 6, 57613, 26267, 736, 208, 155063, 2646,
    18996, 134, 12877, 7300, 45472, 19175, 4683, 0, 521, 0, 2292, 438, 2060,
    324, 58370, 8899, 0, 0, 1111010, 935704, 240672, 178967, 2106, 2106,
    56264, 6793, 39722, 36481, 1311960, 1078190, 0, 0, 0, 0, 22532, 4623,
    1163492, 778765, 20043, 203, 0, 0, 162424, 30092, 12053, 3305, 189, 0,
    19854, 203, 7105, 431, 167372, 32966, 9620, 81, 10804, 85, 163468, 24329,
    316556, 55321, 11303, 10, 273553, 29245, 11303, 10, 273553, 29245, 0, 0,
    21204, 762, 58370, 8899, 1351682, 1114671, 58370, 8899, 1351682, 1114671,
    0, 0, 1186024, 783388, 0, 0, 194520, 33600, 0, 0, 194520, 33600, 0, 0,
    500448, 79816, 9593, 10, 222815, 29245, 9593, 10, 222815, 29245, 0, 0,
    9556, 762, 58370, 8899, 1351682, 1114671, 58370, 8899, 1351682, 1114671,
    0, 0, 1186024, 783388, 0, 0, 194520, 33600, 0, 0, 194520, 33600, 0, 0,
    500448, 79816, 0, 0, 119, 2661, 26314, 0, 10, 151, 0, 0, 119, 2661, 26314,
    0, 10, 151, 0, 0, 0, 0, 762, 0, 0, 0, 0, 2566, 230, 4, 1118101, 2062, 605,
    2, 0, 2566, 230, 4, 1118101, 2062, 605, 2, 0, 0, 0, 0, 783388, 0, 0, 0, 0,
    16, 66, 0, 32839, 429, 250, 0, 0, 16, 66, 0, 32839, 429, 250, 0, 0, 12, 0,
    0, 78376, 886, 490, 52, 0, 786636, 682660, 614575, 785017, 614575, 10376,
    786636, 610256, 614575, 610256, 614575, 0, 3463992, 3035359, 148803,
    3042124, 175198, 44592, 3463992, 109927, 148803, 129032, 175198, 36152,
    786636, 718092, 772470, 786636, 772470, 6160, 786636, 771960, 772470,
    771960, 772470, 0, 3463992, 3175808, 175198, 3175808, 175198, 25040,
    3463992, 129032, 175198, 129032, 175198, 42974, 786636, 552938, 416194,
    782686, 416194, 13334, 786636, 409194, 416194, 409194, 416194, 0, 3463992,
    2445389, 11056, 2462176, 175198, 56286, 3463992, 10360, 11056, 129032,
    175198, 696, 786636, 544885, 27392, 771929, 27392, 13190, 786636, 27351,
    27392, 27351, 27392, 0, 3463992, 2408732, 11056, 2462176, 175198, 55638,
    3463992, 10360, 11056, 129032, 175198, 696, 0, 0, 10838, 117990, 123172,
    32856, 0, 0, 0, 0, 74271, 115060, 74855, 20670, 0, 0, 0, 0, 1376, 11507,
    8321, 0, 0, 0, 0, 0, 48258, 594644, 626474, 140676, 0, 0, 0, 0, 379402,
    550842, 377204, 102604, 0, 0, 0, 0, 78404, 574616, 475348, 57656, 0, 0, 0,
    0, 0, 27928, 122940, 43652, 0, 0, 0, 0, 59832, 82190, 52498, 0, 0, 0, 0,
    0, 32776, 249566, 199374, 18732, 0, 0, 0, 0, 291103, 483051, 276202,
    116512, 17040, 0, 1038018, 392363, 44257, 4348, 0, 0, 0, 0, 1222550, 0, 0,
    0, 0, 0, 0, 0, 72472, 25689, 1649, 0, 0, 0, 0, 0, 5021268, 2064398,
    280714, 32204, 0, 0, 0, 0, 5967976, 0, 0, 0, 0, 0, 0, 0, 3685792, 1671566,
    150218, 7624, 0, 0, 0, 0, 635904, 288438, 161462, 11590, 0, 0, 0, 0,
    719238, 0, 0, 0, 0, 0, 0, 0, 1565796, 662940, 85480, 1736, 0, 0, 0, 0,
    5652450, 0, 0, 0, 0, 0, 0, 0, 16676100, 8121600, 6670440, 8554500,
};

/* ---------------- printing ---------------- */

static void
bfmt (
    char *buf,
    int   size,
    s64   mis,
    s64   n)
{
    char t[32];

    pct(t, 32, mis, n);
    snprintf(buf, (size_t)size, "%s of %lld", t, (long long)n);
}

/* an exact difference of two shares in points, rounded to hundredths */
static void
gap_fmt (
    char    *buf,
    int      size,
    s64      an,
    s64      ad,
    s64      bn,
    s64      bd,
    Piscina *p)
{
    Fractio g = fractio_subtrahe(share(an, ad, p), share(bn, bd, p), p);
    s64     h = 0, m;

    g = fractio_multiplica(g, fractio_ex_s64(10000), p);
    (void)magnus_ad_s64(fractio_rotunda(g, p), &h);
    m = h < 0 ? -h : h;
    snprintf(buf, (size_t)size, "%s%lld.%02lld", h < 0 ? "-" : "+",
        (long long)(m / 100), (long long)(m % 100));
}

static void
stratum_label (
    char *buf,
    int   size,
    int   s)
{
    if (s == NSTRATA) snprintf(buf, (size_t)size, "projective");
    else snprintf(buf, (size_t)size, "%s, %s", kind_name[s / 3],
        dir_name[s % 3]);
}

static void
print_dist (
    const Acc  *A,
    const char *title)
{
    int  s, k;
    char lab[64];

    printf("\n  %s: pairs by WIDTH (umbrae open at once)\n  %-33s", title,
        "");
    for (k = 1; k <= MAXW; k++) printf(" %11d", k);
    printf("\n");
    for (s = NSTRATA; s >= 0; s--) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (k = 1; k <= MAXW; k++)
            printf(" %11lld", (long long)A->width[s][k]);
        printf("\n");
    }
    printf("  %s: umbrae by DEPTH (later-opened umbrae that outlive it)\n"
        "  %-33s", title, "");
    for (k = 0; k < MAXW; k++) printf(" %11d", k);
    printf("\n");
    for (s = NSTRATA; s >= 0; s--) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (k = 0; k < MAXW; k++)
            printf(" %11lld", (long long)A->depth[s][k]);
        printf("\n");
    }
    printf("  umbrae: %lld dangling, %lld slot, %lld root; %lld backward "
        "attachments\n", (long long)A->utype[U_DANGLING],
        (long long)A->utype[U_SLOT], (long long)A->utype[U_ROOT],
        (long long)A->back);
}

/* the 2 x 2 per stratum: COMPETED / CLEAR x INTERFERED / RETRIEVED */
static void
print_cells (
    const Acc  *A,
    int         rel,
    const char *title)
{
    int  s, c, i;
    char lab[64], b[48];

    printf("\n  %s (crossed pairs, |F| >= 2): B in each cell\n  %-33s %22s "
        "%22s %22s %22s\n", title, "", "competed+interfered",
        "competed+retrieved", "clear+interfered", "clear+retrieved");
    for (s = 0; s < NSTRATA; s++) {
        stratum_label(lab, 64, s);
        printf("  %-33s", lab);
        for (c = 1; c >= 0; c--)
            for (i = 1; i >= 0; i--) {
                const s64 *x = rel ? A->cell_rel[POP_F2][s][c][i]
                    : A->cell[POP_F2][s][c][i];

                bfmt(b, 48, x[1], x[0]);
                printf(" %22s", b);
            }
        printf("\n");
    }
}

static void
print_l0_extra (
    const Acc *A)
{
    int  s, c, r;
    char lab[64], b1[48], b2[48], b3[48], b4[48];
    s64  mis = 0, rt[NROUTES], rtc[NROUTES];

    printf("\n  competition, real against MORPHOLOGY-BLIND (any noun as near "
        "as the head), L0, |F| >= 2\n  %-33s %22s %22s %22s %22s\n", "",
        "competed", "clear", "blind: competed", "blind: clear");
    for (s = 0; s < NSTRATA; s++) {
        s64 cm = 0, cn = 0, lm = 0, ln = 0;
        int i;

        for (i = 0; i < 2; i++) {
            cm += A->cell[POP_F2][s][1][i][1];
            cn += A->cell[POP_F2][s][1][i][0];
            lm += A->cell[POP_F2][s][0][i][1];
            ln += A->cell[POP_F2][s][0][i][0];
        }
        stratum_label(lab, 64, s);
        bfmt(b1, 48, cm, cn);
        bfmt(b2, 48, lm, ln);
        bfmt(b3, 48, A->blind[POP_F2][s][1][1], A->blind[POP_F2][s][1][0]);
        bfmt(b4, 48, A->blind[POP_F2][s][0][1], A->blind[POP_F2][s][0][0]);
        printf("  %-33s %22s %22s %22s %22s\n", lab, b1, b2, b3, b4);
    }
    for (r = 0; r < NROUTES; r++) rt[r] = rtc[r] = 0;
    for (s = 0; s < NSTRATA; s++)
        for (c = 0; c < 2; c++)
            for (r = 0; r < NROUTES; r++) {
                rt[r] += A->route[s][c][r];
                if (!c) rtc[r] += A->route[s][c][r];
                mis += A->route[s][c][r];
            }
    pct(b1, 48, rt[RT_NEAR] + rt[RT_FLIP], mis);
    pct(b2, 48, rt[RT_FLIP], mis);
    printf("\n  MECHANISM COVERAGE (L0): of %lld misread pairs, %s have an "
        "analysis in P(s) that puts a displaced word on a word at least as "
        "near as its head (%s through a reversed dependency)\n",
        (long long)mis, b1, b2);
    printf("  misread pairs by route (all; CLEAR only):");
    for (r = 0; r < NROUTES; r++)
        printf("  %s %lld; %lld", route_name[r], (long long)rt[r],
            (long long)rtc[r]);
    printf("\n  covered from outside d's subtree but CLEAR: %lld\n",
        (long long)A->covered_clear);
}

static void
print_arcs (
    const Acc *const *lv4,
    const char       *title)
{
    int  lv, k, i, r;
    char b[5][32];
    static const char *const an[2] = { "adjective", "genitive" };

    printf("\n  %s: displaced arcs, share RETRIEVED, share CLEAR, the same by "
        "own relation, INTERFERED only through a cycle\n", title);
    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k <= 2; k++)
            for (i = 0; i <= 2; i++) {
                s64 x[6];

                if ((k == 2) != (i == 2)) continue;
                for (r = 0; r < 6; r++) {
                    int kk, ii;

                    x[r] = 0;
                    for (kk = 0; kk < 2; kk++)
                        for (ii = 0; ii < 2; ii++)
                            if ((k == 2 || kk == k) && (i == 2 || ii == i))
                                x[r] += lv4[lv]->arcs[kk][ii][r];
                }
                for (r = 1; r <= 4; r++) pct(b[r], 32, x[r], x[0]);
                printf("  %-17s %-10s %-16s %10lld arcs: %8s %8s; own "
                    "relation %8s %8s; cycle-only %lld\n", level_name[lv],
                    k == 2 ? "all" : an[k], i == 2 ? "" : dir_name[i],
                    (long long)x[0], b[1], b[2], b[3], b[4], (long long)x[5]);
            }
}

/* the registered tests with their tables; verdicts into v[3] */
static void
print_registered (
    const Acc *const *lv4,
    int              *v,
    Piscina          *p)
{
    s64  ga[NSTRATA][5], gb[6][5], u1[4];
    int  s, c, nt, ncount;
    char lab[64], b1[48], b2[48], g[32], i0[32], i1[32], i2[32];

    rule_inputs(lv4, ga, gb, u1);
    printf("\n  U2a (EXPECTED; a positive control, partly implied by the "
        "definitions): B(competed) - B(clear) >= 10 points\n");
    for (s = 0; s < NSTRATA; s++) {
        int t = testable(ga[s][1], ga[s][4]) && testable(ga[s][3], ga[s][4]);

        stratum_label(lab, 64, s);
        bfmt(b1, 48, ga[s][0], ga[s][1]);
        bfmt(b2, 48, ga[s][2], ga[s][3]);
        gap_fmt(g, 32, ga[s][0], ga[s][1], ga[s][2], ga[s][3], p);
        printf("  %-33s competed %24s  clear %24s  gap %8s  %s\n", lab, b1,
            b2, g, t ? "testable" : "not testable");
    }
    v[0] = u2a_rule((s64 const (*)[5])ga, NSTRATA, &nt, p);
    printf("  U2a %s (%d testable strata)\n", verdict_name[v[0]], nt);
    printf("\n  U2b (retrieval adds nothing; dependent-first strata): "
        "|B(interfered) - B(retrieved)| <= 5 points where B(retrieved) is "
        "in [10%%, 90%%]\n");
    for (c = 0; c < 6; c++) {
        const s64 *x = gb[c];

        bfmt(b1, 48, x[0], x[1]);
        bfmt(b2, 48, x[2], x[3]);
        gap_fmt(g, 32, x[0], x[1], x[2], x[3], p);
        printf("  %-16s %-9s interfered %24s  retrieved %24s  gap %8s  %s\n",
            kind_name[c / 2], c % 2 ? "CLEAR" : "COMPETED", b1, b2, g,
            counting_cell(x) ? "COUNTS" : !testable(x[1], x[4])
            || !testable(x[3], x[4]) ? "not testable"
            : "outside [10%, 90%]");
    }
    v[1] = u2b_rule((s64 const (*)[5])gb, 6, &ncount, p);
    printf("  U2b %s (%d counting cells)\n", verdict_name[v[1]], ncount);
    pct(i0, 32, u1[0], u1[3]);
    pct(i1, 32, u1[1], u1[3]);
    pct(i2, 32, u1[2], u1[3]);
    gap_fmt(g, 32, u1[0], u1[3], u1[2], u1[3], p);
    v[2] = u1_rule(u1[0], u1[1], u1[2], u1[3]);
    printf("\n  U1 (Latin keeps most of the ideal's retrieval; %lld displaced "
        "adjective arcs): I(L1) %s, I(L0) %s, I(L2) %s; I(L0) - I(L2) %s "
        "points against 2 (I(L1) - I(L0)) and the 5-point floor\n",
        (long long)u1[3], i1, i0, i2, g);
    printf("  U1 %s\n", verdict_name[v[2]]);
}

/* ---------------- the parts ---------------- */

static int
check_vector (
    const char *title,
    const s64  *got,
    const s64  *want,
    int         n)
{
    int k, ok = 1, first = -1;

    for (k = 0; k < n; k++)
        if (got[k] != want[k]) {
            if (first < 0) first = k;
            ok = 0;
        }
    if (getenv("D134_VECTOR") != NULL)
        for (k = 0; k < n; k++)
            printf("VECTOR %s %d %lld\n", title, k, (long long)got[k]);
    if (!ok)
        printf("  (%s: first difference at %d: %lld, the oracle %lld)\n",
            title, first, (long long)got[first], (long long)want[first]);
    return ok;
}

/* populations, labels against task 1's pass, width and depth at every
 * level, covered => competed; lengths lo..hi */
static void
measure_checks (
    const char    *where,
    int            lo,
    int            hi,
    const UChecks *ck,
    const s64     *cross_by_len)
{
    int  lv, n, s, c, i, k;
    int  pop_ok = 1, f2_ok = 1, lab_ok = 1, wd_ok = 1, cov_ok = 1;
    char msg[400];

    for (lv = 0; lv < NLEVELS; lv++)
        for (n = lo; n <= hi; n++) {
            const Acc *A = &acc[lv][n];
            s64        pairs = 0, mis = 0, mis2 = 0, x[4] = { 0, 0, 0, 0 };

            for (s = 0; s < NSTRATA; s++)
                for (c = 0; c < 2; c++)
                    for (i = 0; i < 2; i++) {
                        pairs += A->cell[POP_ALL][s][c][i][0];
                        mis += A->cell[POP_ALL][s][c][i][1];
                        mis2 += A->cell[POP_F2][s][c][i][1];
                    }
            if (pairs != cross_by_len[n]) pop_ok = 0;
            if (n <= 5 && mis != meas[lv].len_b_num[n]) pop_ok = 0;
            if (mis2 != mis) f2_ok = 0;
            for (k = 0; k < 2; k++)
                for (i = 0; i < 2; i++) {
                    x[0] += A->arcs[k][i][0];
                    x[1] += A->arcs[k][i][1];
                    x[2] += A->arcs[k][i][2];
                    x[3] += A->arcs[k][i][5];
                }
            for (k = 0; k < 4; k++)
                if (x[k] != ck->lab[lv][n][k]) lab_ok = 0;
            if (memcmp(A->width, acc[0][n].width, sizeof A->width) != 0
                || memcmp(A->depth, acc[0][n].depth, sizeof A->depth) != 0
                || memcmp(A->utype, acc[0][n].utype, sizeof A->utype) != 0
                || A->back != acc[0][n].back)
                wd_ok = 0;
            if (A->covered_clear != 0) cov_ok = 0;
        }
    snprintf(msg, sizeof msg, "%s: the 2 x 2 cells hold every crossed pair at "
        "every level%s", where, hi <= 5 ? ", and their misreads are D131's B "
        "by length" : "");
    check(msg, pop_ok);
    snprintf(msg, sizeof msg, "%s: every misread pair has |F| >= 2 (the "
        "registered population loses none)", where);
    check(msg, f2_ok);
    snprintf(msg, sizeof msg, "%s: arc labels by the per-level pass (each "
        "level's own bags) == task 1's pass (by L1 bags), level by level, "
        "length by length", where);
    check(msg, lab_ok);
    snprintf(msg, sizeof msg, "%s: width, depth, umbra types and backward "
        "attachments the same at every level, length by length", where);
    check(msg, wd_ok);
    snprintf(msg, sizeof msg, "%s: covered => competed, restated (re-check "
        "M1 meets L1): no misread pair whose P(s) moves a displaced word to a "
        "word as near as its head, OUTSIDE its own subtree, is CLEAR", where);
    check(msg, cov_ok);
}

/* The T4 review's own oracle (recensor-d134, rec.c: trees first, F(s)
 * and P(s) by grouping pairs by their string; pasted from its output).
 * The genitive cell (genitives only, dependent first, COMPETED, L0,
 * |F| >= 2), one row per nonzero key: { interfered, the interferers'
 * true relations (bit r - 1), some P analysis hangs d on an interferer,
 * some hangs an interferer on h, pairs, misread }. Up to four words, up
 * to five, six-word pairs. */
static const s64 review_gdiag_4[3][6] = {
    { 0, 0, 0, 0, 5120, 4096 },
    { 1, 1, 1, 0, 96, 96 },
    { 1, 1, 1, 1, 32, 32 },
};
static const s64 review_gdiag_5[16][6] = {
    { 0, 0, 0, 0, 1191936, 985408 },
    { 1, 1, 0, 0, 5280, 4128 },
    { 1, 1, 0, 1, 2016, 2016 },
    { 1, 1, 1, 0, 24052, 24052 },
    { 1, 1, 1, 1, 6668, 6668 },
    { 1, 4, 0, 0, 776, 776 },
    { 1, 4, 0, 1, 120, 120 },
    { 1, 4, 1, 0, 4328, 4328 },
    { 1, 4, 1, 1, 408, 408 },
    { 1, 5, 1, 0, 400, 400 },
    { 1, 5, 1, 1, 112, 112 },
    { 1, 16, 0, 0, 23296, 768 },
    { 1, 16, 0, 1, 256, 256 },
    { 1, 16, 1, 0, 1024, 1024 },
    { 1, 16, 1, 1, 23552, 23552 },
    { 1, 17, 1, 1, 1024, 1024 },
};
static const s64 review_gdiag_6[28][6] = {
    { 0, 0, 0, 0, 136687296, 114999482 },
    { 1, 1, 0, 0, 1003462, 827238 },
    { 1, 1, 0, 1, 361514, 361514 },
    { 1, 1, 1, 0, 2739976, 2739976 },
    { 1, 1, 1, 1, 796232, 796232 },
    { 1, 4, 0, 0, 188272, 170704 },
    { 1, 4, 0, 1, 93968, 93968 },
    { 1, 4, 1, 0, 600994, 600994 },
    { 1, 4, 1, 1, 173534, 173534 },
    { 1, 5, 0, 0, 3256, 3256 },
    { 1, 5, 0, 1, 4424, 4424 },
    { 1, 5, 1, 0, 64367, 64367 },
    { 1, 5, 1, 1, 50833, 50833 },
    { 1, 16, 0, 0, 7559840, 450624 },
    { 1, 16, 0, 1, 2249440, 2249440 },
    { 1, 16, 1, 0, 4447104, 4447104 },
    { 1, 16, 1, 1, 6815488, 6815488 },
    { 1, 17, 0, 0, 18432, 0 },
    { 1, 17, 0, 1, 3072, 3072 },
    { 1, 17, 1, 0, 118656, 118656 },
    { 1, 17, 1, 1, 545920, 545920 },
    { 1, 20, 1, 0, 33664, 33664 },
    { 1, 20, 1, 1, 87168, 87168 },
    { 1, 21, 1, 0, 2304, 2304 },
    { 1, 21, 1, 1, 7936, 7936 },
    { 1, 32, 0, 0, 5624, 2800 },
    { 1, 32, 0, 1, 10568, 10568 },
    { 1, 33, 1, 1, 192, 192 },
};

/* the same oracle: displaced arcs of crossed pairs with |F| >= 2,
 * [four, five, six][level][adjective, genitive][dependent first, head
 * first] = { arcs, RETRIEVED } */
static const s64 review_arcs_f2[3][NLEVELS][2][2][2] = {
    { /* up to four words */
      { { { 240, 234 }, { 240, 70 } }, { { 6144, 6016 }, { 6144, 896 } } },
      { { { 240, 240 }, { 240, 240 } }, { { 6144, 6144 }, { 6144, 1024 } } },
      { { { 240, 200 }, { 240, 90 } }, { { 6144, 5120 }, { 6144, 0 } } },
      { { { 240, 200 }, { 240, 40 } }, { { 6144, 5120 }, { 6144, 0 } } },
    },
    { /* up to five words */
      { { { 248424, 228530 }, { 248424, 187558 } }, { { 1923072, 1814208 }, { 1923072, 118144 } } },
      { { { 248424, 236136 }, { 248424, 248424 } }, { { 1923072, 1873920 }, { 1923072, 136192 } } },
      { { { 248424, 188408 }, { 248424, 149596 } }, { { 1923072, 1484608 }, { 1923072, 4096 } } },
      { { { 248424, 188228 }, { 248424, 15432 } }, { { 1923072, 1475584 }, { 1923072, 4096 } } },
    },
    { /* six-word pairs */
      { { { 84991776, 73815074 }, { 84991776, 64920246 } }, { { 409337856, 361743680 }, { 409337856, 13158400 } } },
      { { { 84991776, 76914464 }, { 84991776, 83130144 } }, { { 409337856, 376438784 }, { 409337856, 15179776 } } },
      { { { 84991776, 59294646 }, { 84991776, 45503825 } }, { { 409337856, 289893152 }, { 409337856, 1220608 } } },
      { { { 84991776, 58699264 }, { 84991776, 2699775 } }, { { 409337856, 285265920 }, { 409337856, 1220608 } } },
    },
};

/* the demo's genitive cell == the review's rows, and no other key
 * nonzero */
static int
gdiag_matches (
    const Acc *A,
    const s64 (*rows)[6],
    int        nrows)
{
    int r, it, ro, pd, px, nonzero = 0;

    for (r = 0; r < nrows; r++) {
        const s64 *x = rows[r];
        const s64 *y = A->gdiag[x[0]][x[1]][x[2]][x[3]];

        if (y[0] != x[4] || y[1] != x[5]) return 0;
    }
    for (it = 0; it < 2; it++)
        for (ro = 0; ro < 64; ro++)
            for (pd = 0; pd < 2; pd++)
                for (px = 0; px < 2; px++)
                    nonzero += A->gdiag[it][ro][pd][px][0] != 0;
    return nonzero == nrows;
}

static int
arcs_f2_match (
    const Acc *const *lv4,
    int               set)
{
    int lv, k, i, j;

    for (lv = 0; lv < NLEVELS; lv++)
        for (k = 0; k < 2; k++)
            for (i = 0; i < 2; i++)
                for (j = 0; j < 2; j++)
                    if (lv4[0]->arcs_f2[lv][k][i][j]
                        != review_arcs_f2[set][lv][k][i][j])
                        return 0;
    return 1;
}

/* T4 (review M1): the genitive cell decomposed */
static void
print_genitive_cell (
    const Acc *A)
{
    s64  rn = 0, rm = 0, gn = 0, gm = 0, on = 0, om = 0, pn = 0, pm = 0;
    int  ro, pd, px;
    char b[48];

    for (ro = 0; ro < 64; ro++)
        for (pd = 0; pd < 2; pd++)
            for (px = 0; px < 2; px++) {
                const s64 *r = A->gdiag[0][ro][pd][px];
                const s64 *x = A->gdiag[1][ro][pd][px];

                rn += r[0];
                rm += r[1];
                if ((ro & (1 << (R_NMOD - 1))) && !pd && !px) {
                    gn += x[0];
                    gm += x[1];
                } else {
                    on += x[0];
                    om += x[1];
                }
                if (pd) {
                    pn += x[0];
                    pm += x[1];
                }
            }
    printf("\n  the genitive cell (genitives only, dependent first, COMPETED; "
        "L0, |F| >= 2) by its interferers (T4 review M1):\n");
    bfmt(b, 48, rm, rn);
    printf("  %-64s %26s\n", "RETRIEVED", b);
    bfmt(b, 48, gm, gn);
    printf("  %-64s %26s\n", "INTERFERED by a genitive no projective "
        "alternative uses", b);
    bfmt(b, 48, om, on);
    printf("  %-64s %26s\n", "INTERFERED otherwise", b);
    bfmt(b, 48, pm, pn);
    printf("  %-64s %26s\n", "  of which a projective alternative hangs d "
        "on an interferer", b);
}

/* U1 in exact terms, by direction, and on the |F| >= 2 population (T4
 * review M2, L6) */
static void
print_u1_more (
    const Acc *const *lv4,
    Piscina          *p)
{
    s64  r[3], n = 0, rf[3], nf = 0;
    int  lv, i;
    char g1[32], g2[32], k[32];

    for (lv = 0; lv < 3; lv++) {
        r[lv] = lv4[lv]->arcs[0][0][1] + lv4[lv]->arcs[0][1][1];
        rf[lv] = lv4[0]->arcs_f2[lv][0][0][1] + lv4[0]->arcs_f2[lv][0][1][1];
    }
    n = lv4[0]->arcs[0][0][0] + lv4[0]->arcs[0][1][0];
    nf = lv4[0]->arcs_f2[0][0][0][0] + lv4[0]->arcs_f2[0][0][1][0];
    gap_fmt(g1, 32, r[0], n, r[2], n, p);
    gap_fmt(g2, 32, 2 * r[1], n, 2 * r[0], n, p);
    pct(k, 32, r[0] - r[2], r[1] - r[2]);
    printf("  U1 exactly: I(L0) - I(L2) %s points, 2 (I(L1) - I(L0)) %s; "
        "Latin keeps %s of the gap between L1 and L2\n", g1, g2, k);
    for (i = 0; i < 2; i++) {
        s64 x0 = lv4[0]->arcs[0][i][1], x1 = lv4[1]->arcs[0][i][1];
        s64 x2 = lv4[2]->arcs[0][i][1];

        pct(k, 32, x0 - x2, x1 - x2);
        printf("    %s adjective arcs: Latin keeps %s\n", dir_name[i], k);
    }
    gap_fmt(g1, 32, rf[0], nf, rf[2], nf, p);
    gap_fmt(g2, 32, 2 * rf[1], nf, 2 * rf[0], nf, p);
    pct(k, 32, rf[0], nf);
    printf("  U1 read on the |F| >= 2 population (%lld adjective arcs; "
        "descriptive): I(L0) %s, I(L0) - I(L2) %s against 2 (I(L1) - I(L0)) "
        "%s - the rule %s\n", (long long)nf, k, g1, g2,
        verdict_name[u1_rule(rf[0], rf[1], rf[2], nf)]);
}

static void
part_measures_five (void)
{
    static Acc pool[NLEVELS];
    const Acc *lv4[NLEVELS];
    s64        v[NV_FULL], cross_by_len[MAXW + 1];
    Acc        orc[NLEVELS];
    const Acc *olv4[NLEVELS];
    Piscina   *p = piscina_generare_dynamicum("d134_quinque", 1 << 20);
    int        lv, n, nv, vc[3], vo[3];
    clock_t    t0 = clock();
    char       title[64];

    printf("\n--- measures (task 2): up to %d words, DESCRIPTIVE ---\n",
        gmaxw);
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(acc[lv], 0, sizeof acc[lv]);
        measure_umbrae(lv, 0, gmaxw);
    }
    tempus("pass", t0);
    for (n = 0; n <= MAXW; n++) cross_by_len[n] = meas[0].len_b_den[n];
    measure_checks(quick ? "up to four words" : "up to five words", 0,
        gmaxw, &ck_five, cross_by_len);
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(&pool[lv], 0, sizeof pool[lv]);
        for (n = 0; n <= gmaxw; n++) acc_add(&pool[lv], &acc[lv][n]);
        lv4[lv] = &pool[lv];
    }
    nv = flatten(lv4, NLEVELS, v);
    if (nv != NV_FULL) fatal("vector size");
    check(quick ? "every total up to four words == oracle_d134.py's "
        "(independent; a pin, 1,125 numbers)" : "every total up to five words "
        "== oracle_d134.py's (independent, exhaustive; a pin, 1,125 numbers)",
        check_vector(quick ? "four" : "five", v, oracle_full[quick ? 1 : 0],
            NV_FULL));
    unflatten(oracle_full[quick ? 1 : 0], NLEVELS, orc);
    for (lv = 0; lv < NLEVELS; lv++) olv4[lv] = &orc[lv];
    decide(lv4, vc, p);
    decide(olv4, vo, p);
    check("the rules applied to the oracle's totals give the same three "
        "answers (wiring)", vc[0] == vo[0] && vc[1] == vo[1]
        && vc[2] == vo[2]);
    check(quick ? "up to four words: the genitive cell by its interferers, "
        "and the |F| >= 2 adjective and genitive arc labels, == the T4 "
        "review's own oracle (rec.c; a pin)" : "up to five words: the "
        "genitive cell by its interferers, and the |F| >= 2 adjective and "
        "genitive arc labels, == the T4 review's own oracle (rec.c; a pin)",
        quick ? gdiag_matches(lv4[0], review_gdiag_4, 3)
            && arcs_f2_match(lv4, 0)
        : gdiag_matches(lv4[0], review_gdiag_5, 16) && arcs_f2_match(lv4, 1));
    snprintf(title, sizeof title, "up to %d words", gmaxw);
    print_dist(lv4[0], title);
    for (lv = 0; lv < NLEVELS; lv++) {
        char t2[96];

        snprintf(t2, sizeof t2, "%s, %s", title, level_name[lv]);
        print_cells(lv4[lv], 0, t2);
    }
    print_cells(lv4[0], 1, "L0, labels by OWN RELATION");
    print_l0_extra(lv4[0]);
    print_arcs(lv4, title);
    print_genitive_cell(lv4[0]);
    printf("\n  the registered rules applied at %s - DESCRIPTIVE (registered "
        "at six words):\n", title);
    print_registered(lv4, vc, p);
    print_u1_more(lv4, p);
    piscina_destruere(p);
}

static void
part_measures_six (
    s64 cross6)
{
    const Acc *lv4[NLEVELS], *sub4[NLEVELS], *olv4[NLEVELS];
    Acc        orc[NLEVELS];
    s64        v[NV_SUB], cross_by_len[MAXW + 1];
    Piscina   *p = piscina_generare_dynamicum("d134_sex", 1 << 20);
    int        lv, n, nv, vc[3], vs[3], vo[3];
    clock_t    t0 = clock();

    printf("\n--- measures (task 2): six-word pairs - the REGISTERED "
        "verdicts ---\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        memset(&acc[lv][MAXW], 0, sizeof acc[lv][MAXW]);
        memset(&acc_sub[lv], 0, sizeof acc_sub[lv]);
        measure_umbrae(lv, MAXW, MAXW);
        lv4[lv] = &acc[lv][MAXW];
        sub4[lv] = &acc_sub[lv];
    }
    tempus("pass", t0);
    for (n = 0; n <= MAXW; n++) cross_by_len[n] = 0;
    cross_by_len[MAXW] = cross6;
    measure_checks("six words", MAXW, MAXW, &ck_six, cross_by_len);
    nv = flatten(sub4, 2, v);
    if (nv != NV_SUB) fatal("vector size");
    check("the oracle's subset (2,250 L1 bags): every total == "
        "oracle_d134.py's (independent; B at L0 and L1, labels at every "
        "level; a pin, 837 numbers - PARTIAL)", check_vector("six-subset", v,
            oracle_sub, NV_SUB));
    unflatten(oracle_sub, 2, orc);
    for (lv = 0; lv < NLEVELS; lv++) olv4[lv] = &orc[lv];
    decide(sub4, vs, p);
    decide(olv4, vo, p);
    check("the rules applied to the oracle's subset totals give the same "
        "three answers as to the demo's subset totals (wiring)", vs[0] == vo[0]
        && vs[1] == vo[1] && vs[2] == vo[2]);
    print_dist(lv4[0], "six words");
    for (lv = 0; lv < NLEVELS; lv++) {
        char t2[96];

        snprintf(t2, sizeof t2, "six words, %s", level_name[lv]);
        print_cells(lv4[lv], 0, t2);
    }
    print_cells(lv4[0], 1, "six words, L0, labels by OWN RELATION");
    print_l0_extra(lv4[0]);
    print_arcs(lv4, "six words");
    print_genitive_cell(lv4[0]);
    check("six words: the genitive cell by its interferers, and the "
        "|F| >= 2 adjective and genitive arc labels, == the T4 review's own "
        "exhaustive oracle (rec.c; a pin)", gdiag_matches(lv4[0],
            review_gdiag_6, 28) && arcs_f2_match(lv4, 2));
    printf("\n--- verdicts (registered, six-word pairs, L0, |F| >= 2) ---\n");
    print_registered(lv4, vc, p);
    print_u1_more(lv4, p);
    printf("\n  (the oracle's subset gives U2a %s, U2b %s, U1 %s - partial, "
        "for wiring only)\n", verdict_name[vs[0]], verdict_name[vs[1]],
        verdict_name[vs[2]]);
    piscina_destruere(p);
}

static void
part_umbrae_five (void)
{
    clock_t t0 = clock();
    s64     pairs = 0, crossed = 0;
    int     n;

    printf("\n--- umbrae, width, depth and labels: every pair up to %d words "
        "(checks only) ---\n", gmaxw);
    for (n = 0; n <= gmaxw; n++) pairs += level_pairs[0][n];
    crossed = meas[0].b_den;
    umbra_pass(gmaxw, &ck_five);
    tempus("pass", t0);
    umbra_checks(quick ? "up to four words" : "up to five words", &ck_five,
        pairs, crossed);
}

/* ================================================================
 * Six words: feasibility (task 0; counts only)
 * ================================================================ */

/* The oracle's six-word subset. Key: the six-word bags at L1, where
 * every reading is its own form, so an L1 bag is a multiset of readings
 * and selects the same pairs at every level. In bag_code order, the bag
 * of rank r is in the subset iff r % SUBSET_K == 0. */
#define SUBSET_K 128

static double
megabytes (
    long cap)
{
    return (double)cap * (double)sizeof(Entry) / (1024.0 * 1024.0);
}

static void
free_entries (void)
{
    int lv;

    for (lv = 0; lv < NLEVELS; lv++) {
        free(gen_entries[lv].e);
        free(an_entries[lv].e);
        memset(&gen_entries[lv], 0, sizeof gen_entries[lv]);
        memset(&an_entries[lv], 0, sizeof an_entries[lv]);
    }
    gen_emitted = 0;
}

/* projective and crossed pairs of len words at one level, by every
 * distinct order of every bag: the skeleton of the per-pair pass the
 * later tasks run (orders, analyses carried to each order, tree table) */
static void
sweep_length (
    int  lv,
    int  len,
    s64 *proj,
    s64 *cross)
{
    long i = 0;

    *proj = *cross = 0;
    while (i < an_entries[lv].n) {
        long     j = i, s;
        int      bag[MAXW], nb, p, k, a, nc;
        Analysis canon[MAXA];

        while (j < an_entries[lv].n && an_entries[lv].e[j].bag
            == an_entries[lv].e[i].bag)
            j++;
        decode_bag(an_entries[lv].e[i].bag, bag, &nb);
        if (nb != len) {
            i = j;
            continue;
        }
        nc = (int)(j - i);
        for (s = i; s < j; s++)
            decode_key(an_entries[lv].e[s].key, &canon[s - i], nb);
        for (p = 0; p < nperms[nb]; p++) {
            const int *sig = perms[nb][p];
            int        inv[MAXW];

            if (!first_order(bag, sig, nb)) continue;
            for (k = 0; k < nb; k++) inv[sig[k]] = k;
            for (a = 0; a < nc; a++) {
                for (k = 0; k < nb; k++) {
                    int h = canon[a].head[k];
                    mapped[a].head[inv[k]] = h == 0 ? 0 : inv[h - 1] + 1;
                }
                if (t_proj[nb][tree_index(mapped[a].head, nb)]) (*proj)++;
                else (*cross)++;
            }
        }
        i = j;
    }
}

static void
part_six (void)
{
    int     lv, n, same_pop = 1, same_split = 1;
    long    tried, raw = 0, gcap = 0;
    clock_t t0;
    s64     proj[NLEVELS], cross[NLEVELS];
    char    msg[400];

    printf("\n--- six words: feasibility (task 0; counts only) ---\n");
    free_entries();
    gmaxw = MAXW;
    t0 = clock();
    generate();
    tempus("generator", t0);
    for (lv = 0; lv < NLEVELS; lv++) {
        raw += gen_entries[lv].n;
        gcap += gen_entries[lv].cap;
    }
    printf("  generator: %ld abstract analyses up to %d words; %ld "
        "placements before sorting, four levels (%.0f MB)\n", gen_emitted,
        gmaxw, raw, megabytes(gcap));
    for (lv = 0; lv < NLEVELS; lv++) {
        long gd, ad, i;
        int  same;

        t0 = clock();
        gd = sort_unique(&gen_entries[lv]);
        tempus("generator sort", t0);
        tried = 0;
        t0 = clock();
        analyse_all_bags(lv, &tried);
        ad = sort_unique(&an_entries[lv]);
        tempus("analyser", t0);
        same = gen_entries[lv].n == an_entries[lv].n;
        for (i = 0; same && i < gen_entries[lv].n; i++)
            if (entry_cmp(&gen_entries[lv].e[i], &an_entries[lv].e[i]) != 0)
                same = 0;
        printf("  %-17s generator: %ld repeats; analyser over %ld bags, %ld "
            "analyses (%.0f MB)\n", level_name[lv], gd, tried,
            an_entries[lv].n, megabytes(an_entries[lv].cap));
        snprintf(msg, sizeof msg, "%s: generated == analysed up to six words, "
            "%ld positioned analyses on canonical strings, readings included",
            level_name[lv], an_entries[lv].n);
        check(msg, same);
        snprintf(msg, sizeof msg, "%s: the analyser never yields an analysis "
            "twice (six words)", level_name[lv]);
        check(msg, ad == 0);
        free(gen_entries[lv].e);
        memset(&gen_entries[lv], 0, sizeof gen_entries[lv]);
    }

    sizes();
    printf("\n  %-17s %8s %10s %8s %8s %10s %12s %6s\n", "level", "bags",
        "strings", "pairs 3", "pairs 4", "pairs 5", "pairs 6", "max|F|");
    for (lv = 0; lv < NLEVELS; lv++)
        printf("  %-17s %8ld %10ld %8ld %8ld %10ld %12ld %6d\n",
            level_name[lv], level_bags[lv], level_strings[lv],
            level_pairs[lv][3], level_pairs[lv][4], level_pairs[lv][5],
            level_pairs[lv][6], level_maxf[lv]);
    for (lv = 1; lv < NLEVELS; lv++)
        for (n = 0; n <= MAXW; n++)
            if (level_pairs[lv][n] != level_pairs[0][n]) same_pop = 0;
    check("six words: the population of pairs is the same at every level, "
        "length by length", same_pop);
    check("pairs by length up to six words: 3: 384, 4: 55,296, 5: 6,696,960 "
        "(as at five words) and 6: 809,533,440 (D131 task 0's count; a pin)",
        level_pairs[0][1] == 0 && level_pairs[0][2] == 0
        && level_pairs[0][3] == 384 && level_pairs[0][4] == 55296
        && level_pairs[0][5] == 6696960 && level_pairs[0][6] == 809533440L);
    check("the largest |F| of an L0 string up to six words is 144 (D131 "
        "task 0's count; a pin)", level_maxf[0] == 144);

    printf("\n  six-word pairs, projective and crossed: the per-pair pass "
        "(orders, analyses carried, tree table)\n");
    for (lv = 0; lv < NLEVELS; lv++) {
        t0 = clock();
        sweep_length(lv, MAXW, &proj[lv], &cross[lv]);
        printf("  %-17s projective %10lld  crossed %10lld\n",
            level_name[lv], (long long)proj[lv], (long long)cross[lv]);
        tempus("sweep", t0);
        if (proj[lv] != proj[0] || cross[lv] != cross[0]
            || proj[lv] + cross[lv] != (s64)level_pairs[lv][MAXW])
            same_split = 0;
    }
    check("six words: projective + crossed == the six-word pairs, and the "
        "split is the same at every level", same_split);
    {
        s64 pairs = 0;

        printf("\n  umbrae, width, depth and labels: every pair up to six "
            "words (checks only)\n");
        for (n = 0; n <= MAXW; n++) pairs += level_pairs[0][n];
        t0 = clock();
        umbra_pass(MAXW, &ck_six);
        tempus("pass", t0);
        umbra_checks("up to six words", &ck_six, pairs, meas[0].b_den
            + cross[0]);
    }
    {
        s64 p5, c5;

        sweep_length(0, 5, &p5, &c5);
        snprintf(msg, sizeof msg, "the same pass at five words: projective "
            "%lld, crossed %lld (D131 task 0's table: 2,694,144 and 4,002,816; "
            "a pin)", (long long)p5, (long long)c5);
        check(msg, p5 == 2694144 && c5 == 4002816);
    }

    {
        long i = 0, rank = 0;
        s64  pairs6 = 0;
        int  r, ident = 1, kk;
        static const int ks[5] = { 32, 64, 128, 256, 512 };
        long sel_bags[5];
        s64  sel_pairs[5];

        for (kk = 0; kk < 5; kk++) {
            sel_bags[kk] = 0;
            sel_pairs[kk] = 0;
        }
        for (r = 0; r < n_readings; r++)
            if (form_of[1][r] != r) ident = 0;
        check("L1: form index == reading index, so an L1 bag is a multiset "
            "of readings (the subset's key is level-free)", ident
            && n_forms[1] == n_readings);
        while (i < an_entries[1].n) {
            long j = i;
            int  bag[MAXW], nb;
            s64  pr;

            while (j < an_entries[1].n && an_entries[1].e[j].bag
                == an_entries[1].e[i].bag)
                j++;
            decode_bag(an_entries[1].e[i].bag, bag, &nb);
            if (nb == MAXW) {
                pr = (s64)multinomial(bag, nb) * (s64)(j - i);
                pairs6 += pr;
                if (rank % SUBSET_K == 0) {
                    if (n_sel == MAXSEL) fatal("subset too large");
                    sel_codes[n_sel++] = an_entries[1].e[i].bag;
                }
                for (kk = 0; kk < 5; kk++)
                    if (rank % ks[kk] == 0) {
                        sel_bags[kk]++;
                        sel_pairs[kk] += pr;
                    }
                rank++;
            }
            i = j;
        }
        printf("\n  the oracle's six-word subset: %ld six-word bags at L1 "
            "(multisets of readings), %lld pairs\n", rank,
            (long long)pairs6);
        for (kk = 0; kk < 5; kk++)
            printf("  %s K = %3d: %7ld bags, %11lld pairs (%.2f%% of the "
                "six-word pairs)\n", ks[kk] == SUBSET_K ? "->" : "  ",
                ks[kk], sel_bags[kk], (long long)sel_pairs[kk],
                100.0 * (double)sel_pairs[kk] / (double)pairs6);
        check("the six-word L1 bags hold every six-word pair", pairs6
            == (s64)level_pairs[1][MAXW]);
    }
    part_measures_six(cross[0]);
}

int
main (void)
{
    quick = getenv("D134_CELER") != NULL;
    show_times = getenv("D134_TEMPORA") != NULL;
    gmaxw = quick ? 4 : 5;
    printf("KNOTAPEL DEMO 134: umbra width of the toy world%s\n", quick
        ? " (QUICK MODE: up to four words, no six-word part)" : "");
    build_world();
    build_comp();
    build_perms();
    part_paradigms();
    part_trees();
    part_world();
    part_order();
    part_hand();
    part_measures();
    part_hand_umbrae();
    part_umbrae_five();
    part_measures_five();
    rules_check();
    if (!quick && getenv("D134_QUINQUE") == NULL) part_six();
    printf("\n=== %d pass, %d fail ===\n", n_pass, n_fail);
    return n_fail == 0 ? 0 : 1;
}
