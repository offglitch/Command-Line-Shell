## Test 01: Scripting Support [1 pts]

Trailing spaces should be eliminated by your command line tokenization process

```

expected=$(sh < "${TEST_DIR}/scripts/inspector.sh" 2> /dev/null)
actual=$(timeout 2 ./$SHELL_NAME < "${TEST_DIR}/scripts/inspector.sh" 2> /dev/null) || test_end

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"
echo "${actual}"

Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
System Information                                                                         System Information
------------------                                                                         ------------------
Hostname:COMET-PC                                                                          Hostname:COMET-PC
Kernel Version:4.15.0-20-generic                                                           Kernel Version:4.15.0-20-generic
Logged in Users:                                                                           Logged in Users:
comet    tty7         2019-03-31 09:47 (:0)                                                comet    tty7         2019-03-31 09:47 (:0)
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 02: Basic process execution both from PATH and with absolute path [3 pts]

```

# ---------- Test Script ----------
echo "${script}"

/bin/echo System Information
/bin/echo ------------------
/bin/echo -n Hostname: 
cat /proc/sys/kernel/hostname
echo -n Kernel Version: 
cat /proc/sys/kernel/osrelease
echo Logged in Users:
who
 
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null)
if [[ $? -eq 124 ]]; then
    test_end 1 # Exit status 124 means execution timed out
fi

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
System Information                                                                         System Information
------------------                                                                         ------------------
Hostname:COMET-PC                                                                          Hostname:COMET-PC
Kernel Version:4.15.0-20-generic                                                           Kernel Version:4.15.0-20-generic
Logged in Users:                                                                           Logged in Users:
comet    tty7         2019-03-31 09:47 (:0)                                                comet    tty7         2019-03-31 09:47 (:0)
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 03: Long command lines [1 pts]

```

timeout 15 ./$SHELL_NAME < "${TEST_DIR}/scripts/too-long.sh" &> /dev/null || test_end 1
```

## Test 04: Comment support [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
#Here's a comment
# Here's another
        # And another one! ####
echo Line 1 # This better not display
echo Line 2 #There's nothing here! #  echo No way
##################
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end
compare <(echo "${expected}") <(echo "${actual}")

echo "${actual}"
echo "${expected}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Line 1                                                                                     Line 1
Line 2                                                                                     Line 2
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 05: 'cd' built-in command [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
oewiofjweoiwje
owfjeiowej
owjefiowejfiwoeewoijwio
ojwfe
cd /this/is/invalid
pwd
cd /usr/bin
./pwd
cd ../..
ls
cd
pwd
 
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end 1
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
/home/comet/work/CommandLineShell/P2-offglitch-master                                      /home/comet/work/CommandLineShell/P2-offglitch-master
bin                                                                                        bin
boot                                                                                       boot
cdrom                                                                                      cdrom
core                                                                                       core
dev                                                                                        dev
etc                                                                                        etc
home                                                                                       home
initrd.img                                                                                 initrd.img
initrd.img.old                                                                             initrd.img.old
lib                                                                                        lib
lib64                                                                                      lib64
lost+found                                                                                 lost+found
media                                                                                      media
mnt                                                                                        mnt
opt                                                                                        opt
proc                                                                                       proc
root                                                                                       root
run                                                                                        run
sbin                                                                                       sbin
srv                                                                                        srv
swapfile                                                                                   swapfile
sys                                                                                        sys
tmp                                                                                        tmp
usr                                                                                        usr
var                                                                                        var
vmlinuz                                                                                    vmlinuz
/home/comet                                                                                /home/comet
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 06: 'exit' built-in command [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
echo Easiest point...
echo ...ever!
exit
echo If this prints, then exit
echo definitely isn't working!
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end 1
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Easiest point...                                                                           Easiest point...
...ever!                                                                                   ...ever!
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 07: setenv built-in command [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
setenv shell_test_variable_1 This-is-a-really-long-environment-variable-value
setenv lions
setenv shell_test_variable_2 Tigers
setenv shell_test_variable_3 Toitles
setenv
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/print-env.sh
echo Test Complete
# -------------- End --------------

actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
This-is-a-really-long-environment-variable-value                                           This-is-a-really-long-environment-variable-value
Tigers                                                                                     Tigers
Toitles                                                                                    Toitles
Test Complete                                                                              Test Complete
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 08: ^C should be ignored by the shell [1 pts]

NOTE: this test runs in scripting mode; make sure the prompt is not printed by SIGINT handler

```

output=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null)

compare <(echo "It worked") <(echo "${output}")

echo "It worked"
echo "${output}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
It worked                                                                                  It worked
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 09: History List [1 pts]

```

expected=$(HISTFILE=/dev/null HISTSIZE=100 bash \
    < "${TEST_DIR}/scripts/history-script-1.sh" 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < "${TEST_DIR}/scripts/history-script-1.sh" \
    2> /dev/null) || test_end

compare --ignore-all-space <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Command 000                                                                                Command 000
Command 001                                                                                Command 001
Command 002                                                                                Command 002
Command 003                                                                                Command 003
Command 004                                                                                Command 004
Command 005                                                                                Command 005
Command 006                                                                                Command 006
Command 007                                                                                Command 007
Command 008                                                                                Command 008
Command 009                                                                                Command 009
Command 010                                                                                Command 010
Command 011                                                                                Command 011
Command 012                                                                                Command 012
Command 013                                                                                Command 013
Command 014                                                                                Command 014
Command 015                                                                                Command 015
Command 016                                                                                Command 016
Command 017                                                                                Command 017
Command 018                                                                                Command 018
Command 019                                                                                Command 019
Command 020                                                                                Command 020
Command 021                                                                                Command 021
Command 022                                                                                Command 022
Command 023                                                                                Command 023
Command 024                                                                                Command 024
Command 025                                                                                Command 025
Command 026                                                                                Command 026
Command 027                                                                                Command 027
Command 028                                                                                Command 028
Command 029                                                                                Command 029
Command 030                                                                                Command 030
Command 031                                                                                Command 031
Command 032                                                                                Command 032
Command 033                                                                                Command 033
Command 034                                                                                Command 034
Command 035                                                                                Command 035
Command 036                                                                                Command 036
Command 037                                                                                Command 037
Command 038                                                                                Command 038
Command 039                                                                                Command 039
Command 040                                                                                Command 040
Command 041                                                                                Command 041
Command 042                                                                                Command 042
Command 043                                                                                Command 043
Command 044                                                                                Command 044
Command 045                                                                                Command 045
Command 046                                                                                Command 046
Command 047                                                                                Command 047
Command 048                                                                                Command 048
Command 049                                                                                Command 049
Command 050                                                                                Command 050
Command 051                                                                                Command 051
Command 052                                                                                Command 052
Command 053                                                                                Command 053
Command 054                                                                                Command 054
Command 055                                                                                Command 055
Command 056                                                                                Command 056
Command 057                                                                                Command 057
Command 058                                                                                Command 058
Command 059                                                                                Command 059
Command 060                                                                                Command 060
Command 061                                                                                Command 061
Command 062                                                                                Command 062
Command 063                                                                                Command 063
Command 064                                                                                Command 064
Command 065                                                                                Command 065
Command 066                                                                                Command 066
Command 067                                                                                Command 067
Command 068                                                                                Command 068
Command 069                                                                                Command 069
Command 070                                                                                Command 070
Command 071                                                                                Command 071
Command 072                                                                                Command 072
Command 073                                                                                Command 073
Command 074                                                                                Command 074
Command 075                                                                                Command 075
Command 076                                                                                Command 076
Command 077                                                                                Command 077
Command 078                                                                                Command 078
Command 079                                                                                Command 079
Command 080                                                                                Command 080
Command 081                                                                                Command 081
Command 082                                                                                Command 082
Command 083                                                                                Command 083
Command 084                                                                                Command 084
Command 085                                                                                Command 085
Command 086                                                                                Command 086
Command 087                                                                                Command 087
Command 088                                                                                Command 088
Command 089                                                                                Command 089
Command 090                                                                                Command 090
Command 091                                                                                Command 091
Command 092                                                                                Command 092
Command 093                                                                                Command 093
Command 094                                                                                Command 094
Command 095                                                                                Command 095
Command 096                                                                                Command 096
Command 097                                                                                Command 097
Command 098                                                                                Command 098
Command 099                                                                                Command 099
Command 100                                                                                Command 100
Command 101                                                                                Command 101
Command 102                                                                                Command 102
Command 103                                                                                Command 103
Command 104                                                                                Command 104
Command 105                                                                                Command 105
Command 106                                                                                Command 106
Command 107                                                                                Command 107
Command 108                                                                                Command 108
Command 109                                                                                Command 109
Command 110                                                                                Command 110
Command 111                                                                                Command 111
Command 112                                                                                Command 112
Command 113                                                                                Command 113
Command 114                                                                                Command 114
Command 115                                                                                Command 115
Command 116                                                                                Command 116
Command 117                                                                                Command 117
Command 118                                                                                Command 118
Command 119                                                                                Command 119
Command 120                                                                                Command 120
Command 121                                                                                Command 121
Command 122                                                                                Command 122
Command 123                                                                                Command 123
Command 124                                                                                Command 124
Command 125                                                                                Command 125
Command 126                                                                                Command 126
Command 127                                                                                Command 127
Command 128                                                                                Command 128
Command 129                                                                                Command 129
Command 130                                                                                Command 130
Command 131                                                                                Command 131
Command 132                                                                                Command 132
Command 133                                                                                Command 133
Command 134                                                                                Command 134
Command 135                                                                                Command 135
Command 136                                                                                Command 136
Command 137                                                                                Command 137
Command 138                                                                                Command 138
Command 139                                                                                Command 139
Command 140                                                                                Command 140
Command 141                                                                                Command 141
Command 142                                                                                Command 142
Command 143                                                                                Command 143
Command 144                                                                                Command 144
Command 145                                                                                Command 145
Command 146                                                                                Command 146
Command 147                                                                                Command 147
Command 148                                                                                Command 148
Command 149                                                                                Command 149
Command 150                                                                                Command 150
Command 151                                                                                Command 151
Command 152                                                                                Command 152
Command 153                                                                                Command 153
Command 154                                                                                Command 154
Command 155                                                                                Command 155
Command 156                                                                                Command 156
Command 157                                                                                Command 157
Command 158                                                                                Command 158
Command 159                                                                                Command 159
Command 160                                                                                Command 160
Command 161                                                                                Command 161
Command 162                                                                                Command 162
Command 163                                                                                Command 163
Command 164                                                                                Command 164
Command 165                                                                                Command 165
Command 166                                                                                Command 166
Command 167                                                                                Command 167
Command 168                                                                                Command 168
Command 169                                                                                Command 169
Command 170                                                                                Command 170
Command 171                                                                                Command 171
Command 172                                                                                Command 172
Command 173                                                                                Command 173
Command 174                                                                                Command 174
Command 175                                                                                Command 175
Command 176                                                                                Command 176
Command 177                                                                                Command 177
Command 178                                                                                Command 178
Command 179                                                                                Command 179
Command 180                                                                                Command 180
Command 181                                                                                Command 181
Command 182                                                                                Command 182
Command 183                                                                                Command 183
Command 184                                                                                Command 184
Command 185                                                                                Command 185
Command 186                                                                                Command 186
Command 187                                                                                Command 187
Command 188                                                                                Command 188
Command 189                                                                                Command 189
Command 190                                                                                Command 190
Command 191                                                                                Command 191
Command 192                                                                                Command 192
Command 193                                                                                Command 193
Command 194                                                                                Command 194
Command 195                                                                                Command 195
Command 196                                                                                Command 196
Command 197                                                                                Command 197
Command 198                                                                                Command 198
Command 199                                                                                Command 199
Command 200                                                                                Command 200
Command 201                                                                                Command 201
Command 202                                                                                Command 202
Command 203                                                                                Command 203
Command 204                                                                                Command 204
Command 205                                                                                Command 205
Command 206                                                                                Command 206
Command 207                                                                                Command 207
Command 208                                                                                Command 208
Command 209                                                                                Command 209
Command 210                                                                                Command 210
Command 211                                                                                Command 211
Command 212                                                                                Command 212
Command 213                                                                                Command 213
Command 214                                                                                Command 214
Command 215                                                                                Command 215
Command 216                                                                                Command 216
Command 217                                                                                Command 217
Command 218                                                                                Command 218
Command 219                                                                                Command 219
Command 220                                                                                Command 220
Command 221                                                                                Command 221
Command 222                                                                                Command 222
Command 223                                                                                Command 223
Command 224                                                                                Command 224
Command 225                                                                                Command 225
Command 226                                                                                Command 226
Command 227                                                                                Command 227
Command 228                                                                                Command 228
Command 229                                                                                Command 229
Command 230                                                                                Command 230
Command 231                                                                                Command 231
Command 232                                                                                Command 232
Command 233                                                                                Command 233
Command 234                                                                                Command 234
Command 235                                                                                Command 235
Command 236                                                                                Command 236
Command 237                                                                                Command 237
Command 238                                                                                Command 238
Command 239                                                                                Command 239
Command 240                                                                                Command 240
Command 241                                                                                Command 241
Command 242                                                                                Command 242
Command 243                                                                                Command 243
Command 244                                                                                Command 244
Command 245                                                                                Command 245
Command 246                                                                                Command 246
Command 247                                                                                Command 247
Command 248                                                                                Command 248
Command 249                                                                                Command 249
Command 250                                                                                Command 250
Command 251                                                                                Command 251
Command 252                                                                                Command 252
Command 253                                                                                Command 253
Command 254                                                                                Command 254
Command 255                                                                                Command 255
Command 256                                                                                Command 256
Command 257                                                                                Command 257
Command 258                                                                                Command 258
Command 259                                                                                Command 259
Command 260                                                                                Command 260
Command 261                                                                                Command 261
Command 262                                                                                Command 262
Command 263                                                                                Command 263
Command 264                                                                                Command 264
Command 265                                                                                Command 265
Command 266                                                                                Command 266
Command 267                                                                                Command 267
Command 268                                                                                Command 268
Command 269                                                                                Command 269
Command 270                                                                                Command 270
Command 271                                                                                Command 271
Command 272                                                                                Command 272
Command 273                                                                                Command 273
Command 274                                                                                Command 274
Command 275                                                                                Command 275
Command 276                                                                                Command 276
Command 277                                                                                Command 277
Command 278                                                                                Command 278
Command 279                                                                                Command 279
Command 280                                                                                Command 280
Command 281                                                                                Command 281
Command 282                                                                                Command 282
Command 283                                                                                Command 283
Command 284                                                                                Command 284
Command 285                                                                                Command 285
Command 286                                                                                Command 286
Command 287                                                                                Command 287
Command 288                                                                                Command 288
Command 289                                                                                Command 289
Command 290                                                                                Command 290
Command 291                                                                                Command 291
Command 292                                                                                Command 292
Command 293                                                                                Command 293
Command 294                                                                                Command 294
Command 295                                                                                Command 295
Command 296                                                                                Command 296
Command 297                                                                                Command 297
Command 298                                                                                Command 298
Command 299                                                                                Command 299
Command 300                                                                                Command 300
Command 301                                                                                Command 301
Command 302                                                                                Command 302
Command 303                                                                                Command 303
Command 304                                                                                Command 304
Command 305                                                                                Command 305
Command 306                                                                                Command 306
Command 307                                                                                Command 307
Command 308                                                                                Command 308
Command 309                                                                                Command 309
Command 310                                                                                Command 310
Command 311                                                                                Command 311
Command 312                                                                                Command 312
Command 313                                                                                Command 313
Command 314                                                                                Command 314
Command 315                                                                                Command 315
Command 316                                                                                Command 316
Command 317                                                                                Command 317
Command 318                                                                                Command 318
Command 319                                                                                Command 319
Command 320                                                                                Command 320
Command 321                                                                                Command 321
Command 322                                                                                Command 322
Command 323                                                                                Command 323
Command 324                                                                                Command 324
Command 325                                                                                Command 325
Command 326                                                                                Command 326
Command 327                                                                                Command 327
Command 328                                                                                Command 328
Command 329                                                                                Command 329
Command 330                                                                                Command 330
Command 331                                                                                Command 331
Command 332                                                                                Command 332
Command 333                                                                                Command 333
Command 334                                                                                Command 334
Command 335                                                                                Command 335
Command 336                                                                                Command 336
Command 337                                                                                Command 337
Command 338                                                                                Command 338
Command 339                                                                                Command 339
Command 340                                                                                Command 340
Command 341                                                                                Command 341
Command 342                                                                                Command 342
Command 343                                                                                Command 343
Command 344                                                                                Command 344
Command 345                                                                                Command 345
Command 346                                                                                Command 346
Command 347                                                                                Command 347
Command 348                                                                                Command 348
Command 349                                                                                Command 349
Command 350                                                                                Command 350
Command 351                                                                                Command 351
Command 352                                                                                Command 352
Command 353                                                                                Command 353
Command 354                                                                                Command 354
Command 355                                                                                Command 355
Command 356                                                                                Command 356
Command 357                                                                                Command 357
Command 358                                                                                Command 358
Command 359                                                                                Command 359
Command 360                                                                                Command 360
Command 361                                                                                Command 361
Command 362                                                                                Command 362
Command 363                                                                                Command 363
Command 364                                                                                Command 364
Command 365                                                                                Command 365
Command 366                                                                                Command 366
Command 367                                                                                Command 367
Command 368                                                                                Command 368
Command 369                                                                                Command 369
Command 370                                                                                Command 370
Command 371                                                                                Command 371
Command 372                                                                                Command 372
Command 373                                                                                Command 373
Command 374                                                                                Command 374
Command 375                                                                                Command 375
Command 376                                                                                Command 376
Command 377                                                                                Command 377
Command 378                                                                                Command 378
Command 379                                                                                Command 379
Command 380                                                                                Command 380
Command 381                                                                                Command 381
Command 382                                                                                Command 382
Command 383                                                                                Command 383
Command 384                                                                                Command 384
Command 385                                                                                Command 385
Command 386                                                                                Command 386
Command 387                                                                                Command 387
Command 388                                                                                Command 388
Command 389                                                                                Command 389
Command 390                                                                                Command 390
Command 391                                                                                Command 391
Command 392                                                                                Command 392
Command 393                                                                                Command 393
Command 394                                                                                Command 394
Command 395                                                                                Command 395
Command 396                                                                                Command 396
Command 397                                                                                Command 397
Command 398                                                                                Command 398
Command 399                                                                                Command 399
Command 400                                                                                Command 400
Command 401                                                                                Command 401
Command 402                                                                                Command 402
Command 403                                                                                Command 403
Command 404                                                                                Command 404
Command 405                                                                                Command 405
Command 406                                                                                Command 406
Command 407                                                                                Command 407
Command 408                                                                                Command 408
Command 409                                                                                Command 409
Command 410                                                                                Command 410
Command 411                                                                                Command 411
Command 412                                                                                Command 412
Command 413                                                                                Command 413
Command 414                                                                                Command 414
Command 415                                                                                Command 415
Command 416                                                                                Command 416
Command 417                                                                                Command 417
Command 418                                                                                Command 418
Command 419                                                                                Command 419
Command 420                                                                                Command 420
Command 421                                                                                Command 421
Command 422                                                                                Command 422
Command 423                                                                                Command 423
Command 424                                                                                Command 424
Command 425                                                                                Command 425
Command 426                                                                                Command 426
Command 427                                                                                Command 427
Command 428                                                                                Command 428
Command 429                                                                                Command 429
Command 430                                                                                Command 430
Command 431                                                                                Command 431
Command 432                                                                                Command 432
Command 433                                                                                Command 433
Command 434                                                                                Command 434
Command 435                                                                                Command 435
Command 436                                                                                Command 436
Command 437                                                                                Command 437
Command 438                                                                                Command 438
Command 439                                                                                Command 439
Command 440                                                                                Command 440
Command 441                                                                                Command 441
Command 442                                                                                Command 442
Command 443                                                                                Command 443
Command 444                                                                                Command 444
Command 445                                                                                Command 445
Command 446                                                                                Command 446
Command 447                                                                                Command 447
Command 448                                                                                Command 448
Command 449                                                                                Command 449
Command 450                                                                                Command 450
Command 451                                                                                Command 451
Command 452                                                                                Command 452
Command 453                                                                                Command 453
Command 454                                                                                Command 454
Command 455                                                                                Command 455
Command 456                                                                                Command 456
Command 457                                                                                Command 457
Command 458                                                                                Command 458
Command 459                                                                                Command 459
Command 460                                                                                Command 460
Command 461                                                                                Command 461
Command 462                                                                                Command 462
Command 463                                                                                Command 463
Command 464                                                                                Command 464
Command 465                                                                                Command 465
Command 466                                                                                Command 466
Command 467                                                                                Command 467
Command 468                                                                                Command 468
Command 469                                                                                Command 469
Command 470                                                                                Command 470
Command 471                                                                                Command 471
Command 472                                                                                Command 472
Command 473                                                                                Command 473
Command 474                                                                                Command 474
Command 475                                                                                Command 475
Command 476                                                                                Command 476
Command 477                                                                                Command 477
Command 478                                                                                Command 478
Command 479                                                                                Command 479
Command 480                                                                                Command 480
Command 481                                                                                Command 481
Command 482                                                                                Command 482
Command 483                                                                                Command 483
Command 484                                                                                Command 484
Command 485                                                                                Command 485
Command 486                                                                                Command 486
Command 487                                                                                Command 487
Command 488                                                                                Command 488
Command 489                                                                                Command 489
Command 490                                                                                Command 490
Command 491                                                                                Command 491
Command 492                                                                                Command 492
Command 493                                                                                Command 493
Command 494                                                                                Command 494
Command 495                                                                                Command 495
Command 496                                                                                Command 496
Command 497                                                                                Command 497
Command 498                                                                                Command 498
Command 499                                                                                Command 499
Command 500                                                                                Command 500
Command 501                                                                                Command 501
Command 502                                                                                Command 502
Command 503                                                                                Command 503
Command 504                                                                                Command 504
Command 505                                                                                Command 505
Command 506                                                                                Command 506
Command 507                                                                                Command 507
Command 508                                                                                Command 508
Command 509                                                                                Command 509
Command 510                                                                                Command 510
Command 511                                                                                Command 511
Command 512                                                                                Command 512
Command 513                                                                                Command 513
Command 514                                                                                Command 514
Command 515                                                                                Command 515
Command 516                                                                                Command 516
Command 517                                                                                Command 517
Command 518                                                                                Command 518
Command 519                                                                                Command 519
Command 520                                                                                Command 520
Command 521                                                                                Command 521
Command 522                                                                                Command 522
Command 523                                                                                Command 523
Command 524                                                                                Command 524
Command 525                                                                                Command 525
Command 526                                                                                Command 526
Command 527                                                                                Command 527
Command 528                                                                                Command 528
Command 529                                                                                Command 529
Command 530                                                                                Command 530
Command 531                                                                                Command 531
Command 532                                                                                Command 532
Command 533                                                                                Command 533
Command 534                                                                                Command 534
Command 535                                                                                Command 535
Command 536                                                                                Command 536
Command 537                                                                                Command 537
Command 538                                                                                Command 538
Command 539                                                                                Command 539
Command 540                                                                                Command 540
Command 541                                                                                Command 541
Command 542                                                                                Command 542
Command 543                                                                                Command 543
Command 544                                                                                Command 544
Command 545                                                                                Command 545
Command 546                                                                                Command 546
Command 547                                                                                Command 547
Command 548                                                                                Command 548
Command 549                                                                                Command 549
Command 550                                                                                Command 550
Command 551                                                                                Command 551
Command 552                                                                                Command 552
Command 553                                                                                Command 553
Command 554                                                                                Command 554
Command 555                                                                                Command 555
Command 556                                                                                Command 556
Command 557                                                                                Command 557
Command 558                                                                                Command 558
Command 559                                                                                Command 559
Command 560                                                                                Command 560
Command 561                                                                                Command 561
Command 562                                                                                Command 562
Command 563                                                                                Command 563
Command 564                                                                                Command 564
Command 565                                                                                Command 565
Command 566                                                                                Command 566
Command 567                                                                                Command 567
Command 568                                                                                Command 568
Command 569                                                                                Command 569
Command 570                                                                                Command 570
Command 571                                                                                Command 571
Command 572                                                                                Command 572
Command 573                                                                                Command 573
Command 574                                                                                Command 574
Command 575                                                                                Command 575
Command 576                                                                                Command 576
Command 577                                                                                Command 577
Command 578                                                                                Command 578
Command 579                                                                                Command 579
Command 580                                                                                Command 580
Command 581                                                                                Command 581
Command 582                                                                                Command 582
Command 583                                                                                Command 583
Command 584                                                                                Command 584
Command 585                                                                                Command 585
Command 586                                                                                Command 586
Command 587                                                                                Command 587
Command 588                                                                                Command 588
Command 589                                                                                Command 589
Command 590                                                                                Command 590
Command 591                                                                                Command 591
Command 592                                                                                Command 592
Command 593                                                                                Command 593
Command 594                                                                                Command 594
Command 595                                                                                Command 595
Command 596                                                                                Command 596
Command 597                                                                                Command 597
Command 598                                                                                Command 598
Command 599                                                                                Command 599
Command 600                                                                                Command 600
Command 601                                                                                Command 601
Command 602                                                                                Command 602
Command 603                                                                                Command 603
Command 604                                                                                Command 604
Command 605                                                                                Command 605
Command 606                                                                                Command 606
Command 607                                                                                Command 607
Command 608                                                                                Command 608
Command 609                                                                                Command 609
Command 610                                                                                Command 610
Command 611                                                                                Command 611
Command 612                                                                                Command 612
Command 613                                                                                Command 613
Command 614                                                                                Command 614
Command 615                                                                                Command 615
Command 616                                                                                Command 616
Command 617                                                                                Command 617
Command 618                                                                                Command 618
Command 619                                                                                Command 619
Command 620                                                                                Command 620
Command 621                                                                                Command 621
Command 622                                                                                Command 622
Command 623                                                                                Command 623
Command 624                                                                                Command 624
Command 625                                                                                Command 625
Command 626                                                                                Command 626
Command 627                                                                                Command 627
Command 628                                                                                Command 628
Command 629                                                                                Command 629
Command 630                                                                                Command 630
Command 631                                                                                Command 631
Command 632                                                                                Command 632
Command 633                                                                                Command 633
Command 634                                                                                Command 634
  538  echo Command 537                                                                    538 echo Command 537
  539  echo Command 538                                                                    539 echo Command 538
  540  echo Command 539                                                                    540 echo Command 539
  541  echo Command 540                                                                    541 echo Command 540
  542  echo Command 541                                                                    542 echo Command 541
  543  echo Command 542                                                                    543 echo Command 542
  544  echo Command 543                                                                    544 echo Command 543
  545  echo Command 544                                                                    545 echo Command 544
  546  echo Command 545                                                                    546 echo Command 545
  547  echo Command 546                                                                    547 echo Command 546
  548  echo Command 547                                                                    548 echo Command 547
  549  echo Command 548                                                                    549 echo Command 548
  550  echo Command 549                                                                    550 echo Command 549
  551  echo Command 550                                                                    551 echo Command 550
  552  echo Command 551                                                                    552 echo Command 551
  553  echo Command 552                                                                    553 echo Command 552
  554  echo Command 553                                                                    554 echo Command 553
  555  echo Command 554                                                                    555 echo Command 554
  556  echo Command 555                                                                    556 echo Command 555
  557  echo Command 556                                                                    557 echo Command 556
  558  echo Command 557                                                                    558 echo Command 557
  559  echo Command 558                                                                    559 echo Command 558
  560  echo Command 559                                                                    560 echo Command 559
  561  echo Command 560                                                                    561 echo Command 560
  562  echo Command 561                                                                    562 echo Command 561
  563  echo Command 562                                                                    563 echo Command 562
  564  echo Command 563                                                                    564 echo Command 563
  565  echo Command 564                                                                    565 echo Command 564
  566  echo Command 565                                                                    566 echo Command 565
  567  echo Command 566                                                                    567 echo Command 566
  568  echo Command 567                                                                    568 echo Command 567
  569  echo Command 568                                                                    569 echo Command 568
  570  echo Command 569                                                                    570 echo Command 569
  571  echo Command 570                                                                    571 echo Command 570
  572  echo Command 571                                                                    572 echo Command 571
  573  echo Command 572                                                                    573 echo Command 572
  574  echo Command 573                                                                    574 echo Command 573
  575  echo Command 574                                                                    575 echo Command 574
  576  echo Command 575                                                                    576 echo Command 575
  577  echo Command 576                                                                    577 echo Command 576
  578  echo Command 577                                                                    578 echo Command 577
  579  echo Command 578                                                                    579 echo Command 578
  580  echo Command 579                                                                    580 echo Command 579
  581  echo Command 580                                                                    581 echo Command 580
  582  echo Command 581                                                                    582 echo Command 581
  583  echo Command 582                                                                    583 echo Command 582
  584  echo Command 583                                                                    584 echo Command 583
  585  echo Command 584                                                                    585 echo Command 584
  586  echo Command 585                                                                    586 echo Command 585
  587  echo Command 586                                                                    587 echo Command 586
  588  echo Command 587                                                                    588 echo Command 587
  589  echo Command 588                                                                    589 echo Command 588
  590  echo Command 589                                                                    590 echo Command 589
  591  echo Command 590                                                                    591 echo Command 590
  592  echo Command 591                                                                    592 echo Command 591
  593  echo Command 592                                                                    593 echo Command 592
  594  echo Command 593                                                                    594 echo Command 593
  595  echo Command 594                                                                    595 echo Command 594
  596  echo Command 595                                                                    596 echo Command 595
  597  echo Command 596                                                                    597 echo Command 596
  598  echo Command 597                                                                    598 echo Command 597
  599  echo Command 598                                                                    599 echo Command 598
  600  echo Command 599                                                                    600 echo Command 599
  601  echo Command 600                                                                    601 echo Command 600
  602  echo Command 601                                                                    602 echo Command 601
  603  echo Command 602                                                                    603 echo Command 602
  604  echo Command 603                                                                    604 echo Command 603
  605  echo Command 604                                                                    605 echo Command 604
  606  echo Command 605                                                                    606 echo Command 605
  607  echo Command 606                                                                    607 echo Command 606
  608  echo Command 607                                                                    608 echo Command 607
  609  echo Command 608                                                                    609 echo Command 608
  610  echo Command 609                                                                    610 echo Command 609
  611  echo Command 610                                                                    611 echo Command 610
  612  echo Command 611                                                                    612 echo Command 611
  613  echo Command 612                                                                    613 echo Command 612
  614  echo Command 613                                                                    614 echo Command 613
  615  echo Command 614                                                                    615 echo Command 614
  616  echo Command 615                                                                    616 echo Command 615
  617  echo Command 616                                                                    617 echo Command 616
  618  echo Command 617                                                                    618 echo Command 617
  619  echo Command 618                                                                    619 echo Command 618
  620  echo Command 619                                                                    620 echo Command 619
  621  echo Command 620                                                                    621 echo Command 620
  622  echo Command 621                                                                    622 echo Command 621
  623  echo Command 622                                                                    623 echo Command 622
  624  echo Command 623 # Whoa, look, a comment!                                           624 echo Command 623 # Whoa, look, a comment!
  625  # Another neat comment, wowzers.                                                    625 # Another neat comment, wowzers.
  626  echo Command 624                                                                    626 echo Command 624
  627  echo Command 625                                                                    627 echo Command 625
  628  echo Command 626                                                                    628 echo Command 626
  629  echo Command 627                                                                    629 echo Command 627
  630  echo Command 628                                                                    630 echo Command 628
  631  echo Command 629                                                                    631 echo Command 629
  632  echo Command 630                                                                    632 echo Command 630
  633  echo Command 631                                                                    633 echo Command 631
  634  echo Command 632                                                                    634 echo Command 632
  635  echo Command 633                                                                    635 echo Command 633
  636  echo Command 634                                                                    636 echo Command 634
  637  history                                                                             637 history
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 10: History Execution !num [1 pts]

```

expected=$(HISTFILE=/dev/null HISTSIZE=100 bash \
    < "${TEST_DIR}/scripts/history-script-2.sh" 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < "${TEST_DIR}/scripts/history-script-2.sh" \
    2> /dev/null) || test_end

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Command 000                                                                                Command 000
Command 001                                                                                Command 001
Command 002                                                                                Command 002
Command 003                                                                                Command 003
Command 004                                                                                Command 004
Command 005                                                                                Command 005
Command 006                                                                                Command 006
Command 007                                                                                Command 007
Command 008                                                                                Command 008
Command 009                                                                                Command 009
Command 010                                                                                Command 010
Command 011                                                                                Command 011
Command 012                                                                                Command 012
Command 013                                                                                Command 013
Command 014                                                                                Command 014
Command 015                                                                                Command 015
Command 016                                                                                Command 016
Command 017                                                                                Command 017
Command 018                                                                                Command 018
Command 019                                                                                Command 019
Command 020                                                                                Command 020
Command 021                                                                                Command 021
Command 022                                                                                Command 022
Command 023                                                                                Command 023
Command 024                                                                                Command 024
Command 025                                                                                Command 025
Command 026                                                                                Command 026
Command 027                                                                                Command 027
Command 028                                                                                Command 028
Command 029                                                                                Command 029
Command 030                                                                                Command 030
Command 031                                                                                Command 031
Command 032                                                                                Command 032
Command 033                                                                                Command 033
Command 034                                                                                Command 034
Command 035                                                                                Command 035
Command 036                                                                                Command 036
Command 037                                                                                Command 037
Command 038                                                                                Command 038
Command 039                                                                                Command 039
Command 040                                                                                Command 040
Command 041                                                                                Command 041
Command 042                                                                                Command 042
Command 043                                                                                Command 043
Command 044                                                                                Command 044
Command 045                                                                                Command 045
Command 046                                                                                Command 046
Command 047                                                                                Command 047
Command 048                                                                                Command 048
Command 049                                                                                Command 049
Command 050                                                                                Command 050
Command 051                                                                                Command 051
Command 052                                                                                Command 052
Command 053                                                                                Command 053
Command 054                                                                                Command 054
Command 055                                                                                Command 055
Command 056                                                                                Command 056
Command 057                                                                                Command 057
Command 058                                                                                Command 058
Command 059                                                                                Command 059
Command 060                                                                                Command 060
Command 061                                                                                Command 061
Command 062                                                                                Command 062
Command 063                                                                                Command 063
Command 064                                                                                Command 064
Command 065                                                                                Command 065
Command 066                                                                                Command 066
Command 067                                                                                Command 067
Command 068                                                                                Command 068
Command 069                                                                                Command 069
Command 070                                                                                Command 070
Command 071                                                                                Command 071
Command 072                                                                                Command 072
Command 073                                                                                Command 073
Command 074                                                                                Command 074
Command 075                                                                                Command 075
Command 076                                                                                Command 076
Command 077                                                                                Command 077
Command 078                                                                                Command 078
Command 079                                                                                Command 079
Command 080                                                                                Command 080
Command 081                                                                                Command 081
Command 082                                                                                Command 082
Command 083                                                                                Command 083
Command 084                                                                                Command 084
Command 085                                                                                Command 085
Command 086                                                                                Command 086
Command 087                                                                                Command 087
Command 088                                                                                Command 088
Command 089                                                                                Command 089
Command 090                                                                                Command 090
Command 091                                                                                Command 091
Command 092                                                                                Command 092
Command 093                                                                                Command 093
Command 094                                                                                Command 094
Command 095                                                                                Command 095
Command 096                                                                                Command 096
Command 097                                                                                Command 097
Command 098                                                                                Command 098
Command 099                                                                                Command 099
Command 100                                                                                Command 100
Command 101                                                                                Command 101
Command 102                                                                                Command 102
Command 103                                                                                Command 103
Command 104                                                                                Command 104
Command 105                                                                                Command 105
Command 106                                                                                Command 106
Command 107                                                                                Command 107
Command 108                                                                                Command 108
Command 109                                                                                Command 109
Command 110                                                                                Command 110
Command 111                                                                                Command 111
Command 112                                                                                Command 112
Command 113                                                                                Command 113
Command 114                                                                                Command 114
Command 115                                                                                Command 115
Command 116                                                                                Command 116
Command 117                                                                                Command 117
Command 118                                                                                Command 118
Command 119                                                                                Command 119
Command 120                                                                                Command 120
Command 121                                                                                Command 121
Command 122                                                                                Command 122
Command 123                                                                                Command 123
Command 124                                                                                Command 124
Command 125                                                                                Command 125
Command 126                                                                                Command 126
Command 127                                                                                Command 127
Command 128                                                                                Command 128
Command 129                                                                                Command 129
Command 130                                                                                Command 130
Command 131                                                                                Command 131
Command 132                                                                                Command 132
Command 133                                                                                Command 133
Command 134                                                                                Command 134
Command 135                                                                                Command 135
Command 136                                                                                Command 136
Command 137                                                                                Command 137
Command 138                                                                                Command 138
Command 139                                                                                Command 139
Command 140                                                                                Command 140
Command 141                                                                                Command 141
Command 142                                                                                Command 142
Command 143                                                                                Command 143
Command 144                                                                                Command 144
Command 145                                                                                Command 145
Command 146                                                                                Command 146
Command 147                                                                                Command 147
Command 148                                                                                Command 148
Command 149                                                                                Command 149
Command 150                                                                                Command 150
Command 151                                                                                Command 151
Command 152                                                                                Command 152
Command 153                                                                                Command 153
Command 154                                                                                Command 154
Command 155                                                                                Command 155
Command 156                                                                                Command 156
Command 157                                                                                Command 157
Command 158                                                                                Command 158
Command 159                                                                                Command 159
Command 160                                                                                Command 160
Command 161                                                                                Command 161
Command 162                                                                                Command 162
Command 163                                                                                Command 163
Command 164                                                                                Command 164
Command 165                                                                                Command 165
Command 166                                                                                Command 166
Command 167                                                                                Command 167
Command 168                                                                                Command 168
Command 169                                                                                Command 169
Command 170                                                                                Command 170
Command 171                                                                                Command 171
Command 172                                                                                Command 172
Command 173                                                                                Command 173
Command 174                                                                                Command 174
Command 175                                                                                Command 175
Command 176                                                                                Command 176
Command 177                                                                                Command 177
Command 178                                                                                Command 178
Command 179                                                                                Command 179
Command 180                                                                                Command 180
Command 181                                                                                Command 181
Command 182                                                                                Command 182
Command 183                                                                                Command 183
Command 184                                                                                Command 184
Command 185                                                                                Command 185
Command 186                                                                                Command 186
Command 187                                                                                Command 187
Command 188                                                                                Command 188
Command 189                                                                                Command 189
Command 190                                                                                Command 190
Command 191                                                                                Command 191
Command 192                                                                                Command 192
Command 193                                                                                Command 193
Command 194                                                                                Command 194
Command 195                                                                                Command 195
Command 196                                                                                Command 196
Command 197                                                                                Command 197
Command 198                                                                                Command 198
Command 199                                                                                Command 199
Command 200                                                                                Command 200
Command 201                                                                                Command 201
Command 202                                                                                Command 202
Command 203                                                                                Command 203
Command 204                                                                                Command 204
Command 205                                                                                Command 205
Command 206                                                                                Command 206
Command 207                                                                                Command 207
Command 208                                                                                Command 208
Command 209                                                                                Command 209
Command 210                                                                                Command 210
Command 211                                                                                Command 211
Command 212                                                                                Command 212
Command 213                                                                                Command 213
Command 214                                                                                Command 214
Command 215                                                                                Command 215
Command 216                                                                                Command 216
Command 217                                                                                Command 217
Command 218                                                                                Command 218
Command 219                                                                                Command 219
Command 220                                                                                Command 220
Command 221                                                                                Command 221
Command 222                                                                                Command 222
Command 223                                                                                Command 223
Command 224                                                                                Command 224
Command 225                                                                                Command 225
Command 226                                                                                Command 226
Command 227                                                                                Command 227
Command 228                                                                                Command 228
Command 229                                                                                Command 229
Command 230                                                                                Command 230
Command 231                                                                                Command 231
Command 232                                                                                Command 232
Command 233                                                                                Command 233
Command 234                                                                                Command 234
Command 235                                                                                Command 235
Command 236                                                                                Command 236
Command 237                                                                                Command 237
Command 238                                                                                Command 238
Command 239                                                                                Command 239
Command 240                                                                                Command 240
Command 241                                                                                Command 241
Command 242                                                                                Command 242
Command 243                                                                                Command 243
Command 244                                                                                Command 244
Command 245                                                                                Command 245
Command 246                                                                                Command 246
Command 247                                                                                Command 247
Command 248                                                                                Command 248
Command 249                                                                                Command 249
Command 250                                                                                Command 250
Command 251                                                                                Command 251
Command 252                                                                                Command 252
Command 253                                                                                Command 253
Command 254                                                                                Command 254
Command 255                                                                                Command 255
Command 256                                                                                Command 256
Command 257                                                                                Command 257
Command 258                                                                                Command 258
Command 259                                                                                Command 259
Command 260                                                                                Command 260
Command 261                                                                                Command 261
Command 262                                                                                Command 262
Command 263                                                                                Command 263
Command 264                                                                                Command 264
Command 265                                                                                Command 265
Command 266                                                                                Command 266
Command 267                                                                                Command 267
Command 268                                                                                Command 268
Command 269                                                                                Command 269
Command 270                                                                                Command 270
Command 271                                                                                Command 271
Command 272                                                                                Command 272
Command 273                                                                                Command 273
Command 274                                                                                Command 274
Command 275                                                                                Command 275
Command 276                                                                                Command 276
Command 277                                                                                Command 277
Command 278                                                                                Command 278
Command 279                                                                                Command 279
Command 280                                                                                Command 280
Command 281                                                                                Command 281
Command 282                                                                                Command 282
Command 283                                                                                Command 283
Command 284                                                                                Command 284
Command 285                                                                                Command 285
Command 286                                                                                Command 286
Command 287                                                                                Command 287
Command 288                                                                                Command 288
Command 289                                                                                Command 289
Command 290                                                                                Command 290
Command 291                                                                                Command 291
Command 292                                                                                Command 292
Command 293                                                                                Command 293
Command 294                                                                                Command 294
Command 295                                                                                Command 295
Command 296                                                                                Command 296
Command 297                                                                                Command 297
Command 298                                                                                Command 298
Command 299                                                                                Command 299
Command 300                                                                                Command 300
Command 301                                                                                Command 301
Command 302                                                                                Command 302
Command 303                                                                                Command 303
Command 304                                                                                Command 304
Command 305                                                                                Command 305
Command 306                                                                                Command 306
Command 307                                                                                Command 307
Command 308                                                                                Command 308
Command 309                                                                                Command 309
Command 310                                                                                Command 310
Command 311                                                                                Command 311
Command 312                                                                                Command 312
Command 313                                                                                Command 313
Command 314                                                                                Command 314
Command 315                                                                                Command 315
Command 316                                                                                Command 316
Command 317                                                                                Command 317
Command 318                                                                                Command 318
Command 319                                                                                Command 319
Command 320                                                                                Command 320
Command 321                                                                                Command 321
Command 322                                                                                Command 322
Command 323                                                                                Command 323
Command 324                                                                                Command 324
Command 325                                                                                Command 325
Command 326                                                                                Command 326
Command 327                                                                                Command 327
Command 328                                                                                Command 328
Command 329                                                                                Command 329
Command 330                                                                                Command 330
Command 331                                                                                Command 331
Command 332                                                                                Command 332
Command 333                                                                                Command 333
Command 334                                                                                Command 334
Command 335                                                                                Command 335
Command 336                                                                                Command 336
Command 337                                                                                Command 337
Command 338                                                                                Command 338
Command 339                                                                                Command 339
Command 340                                                                                Command 340
Command 341                                                                                Command 341
Command 342                                                                                Command 342
Command 343                                                                                Command 343
Command 344                                                                                Command 344
Command 345                                                                                Command 345
Command 346                                                                                Command 346
Command 347                                                                                Command 347
Command 348                                                                                Command 348
Command 349                                                                                Command 349
Command 350                                                                                Command 350
Command 351                                                                                Command 351
Command 352                                                                                Command 352
Command 353                                                                                Command 353
Command 354                                                                                Command 354
Command 355                                                                                Command 355
Command 356                                                                                Command 356
Command 357                                                                                Command 357
Command 358                                                                                Command 358
Command 359                                                                                Command 359
Command 360                                                                                Command 360
Command 361                                                                                Command 361
Command 362                                                                                Command 362
Command 363                                                                                Command 363
Command 364                                                                                Command 364
Command 365                                                                                Command 365
Command 366                                                                                Command 366
Command 367                                                                                Command 367
Command 368                                                                                Command 368
Command 369                                                                                Command 369
Command 370                                                                                Command 370
Command 371                                                                                Command 371
Command 372                                                                                Command 372
Command 373                                                                                Command 373
Command 374                                                                                Command 374
Command 375                                                                                Command 375
Command 376                                                                                Command 376
Command 377                                                                                Command 377
Command 378                                                                                Command 378
Command 379                                                                                Command 379
Command 380                                                                                Command 380
Command 381                                                                                Command 381
Command 382                                                                                Command 382
Command 383                                                                                Command 383
Command 384                                                                                Command 384
Command 385                                                                                Command 385
Command 386                                                                                Command 386
Command 387                                                                                Command 387
Command 388                                                                                Command 388
Command 389                                                                                Command 389
Command 390                                                                                Command 390
Command 391                                                                                Command 391
Command 392                                                                                Command 392
Command 393                                                                                Command 393
Command 394                                                                                Command 394
Command 395                                                                                Command 395
Command 396                                                                                Command 396
Command 397                                                                                Command 397
Command 398                                                                                Command 398
Command 399                                                                                Command 399
Command 400                                                                                Command 400
Command 401                                                                                Command 401
Command 402                                                                                Command 402
Command 403                                                                                Command 403
Command 404                                                                                Command 404
Command 405                                                                                Command 405
Command 406                                                                                Command 406
Command 407                                                                                Command 407
Command 408                                                                                Command 408
Command 409                                                                                Command 409
Command 410                                                                                Command 410
Command 411                                                                                Command 411
Command 412                                                                                Command 412
Command 413                                                                                Command 413
Command 414                                                                                Command 414
Command 415                                                                                Command 415
Command 416                                                                                Command 416
Command 417                                                                                Command 417
Command 418                                                                                Command 418
Command 419                                                                                Command 419
-----                                                                                      -----
Command 419                                                                                Command 419
Command 322                                                                                Command 322
Command 398                                                                                Command 398
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 11: History Execution !prefix [1 pts]

```

expected=$(HISTFILE=/dev/null HISTSIZE=100 bash \
    < "${TEST_DIR}/scripts/history-script-3.sh" 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < "${TEST_DIR}/scripts/history-script-3.sh" \
    2> /dev/null) || test_end

compare <(echo "${expected}") <(echo "${actual}")

echo "${actual}"
Expected Output                                                                         | Actual Output
echo "${expected}"
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Command 000                                                                                Command 000
Command 001                                                                                Command 001
Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86    Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86
Command 003                                                                                Command 003
Command 004                                                                                Command 004
Command 005                                                                                Command 005
Command 006                                                                                Command 006
Command 007                                                                                Command 007
Command 008                                                                                Command 008
Command 009                                                                                Command 009
Command 010                                                                                Command 010
Command 011                                                                                Command 011
Command 012                                                                                Command 012
Command 013                                                                                Command 013
Command 014                                                                                Command 014
Command 015                                                                                Command 015
Command 016                                                                                Command 016
Command 017                                                                                Command 017
Command 018                                                                                Command 018
Command 019                                                                                Command 019
Command 020                                                                                Command 020
Command 021                                                                                Command 021
Command 022                                                                                Command 022
Command 023                                                                                Command 023
Command 024                                                                                Command 024
Command 025                                                                                Command 025
Command 026                                                                                Command 026
Command 027                                                                                Command 027
Command 028                                                                                Command 028
Command 029                                                                                Command 029
Command 030                                                                                Command 030
Command 031                                                                                Command 031
Command 032                                                                                Command 032
Command 033                                                                                Command 033
Command 034                                                                                Command 034
Command 035                                                                                Command 035
Command 036                                                                                Command 036
Command 037                                                                                Command 037
Command 038                                                                                Command 038
Command 039                                                                                Command 039
Command 040                                                                                Command 040
Command 041                                                                                Command 041
Command 042                                                                                Command 042
Command 043                                                                                Command 043
Command 044                                                                                Command 044
Command 045                                                                                Command 045
Command 046                                                                                Command 046
Command 047                                                                                Command 047
Command 048                                                                                Command 048
Command 049                                                                                Command 049
Command 050                                                                                Command 050
Command 051                                                                                Command 051
Command 052                                                                                Command 052
Command 053                                                                                Command 053
Command 054                                                                                Command 054
Command 055                                                                                Command 055
Command 056                                                                                Command 056
Command 057                                                                                Command 057
Command 058                                                                                Command 058
Command 059                                                                                Command 059
Command 060                                                                                Command 060
Command 061                                                                                Command 061
Command 062                                                                                Command 062
Command 063                                                                                Command 063
Command 064                                                                                Command 064
Command 065                                                                                Command 065
Command 066                                                                                Command 066
Command 067                                                                                Command 067
Command 068                                                                                Command 068
Command 069                                                                                Command 069
Command 070                                                                                Command 070
Command 071                                                                                Command 071
Command 072                                                                                Command 072
Command 073                                                                                Command 073
Command 074                                                                                Command 074
Command 075                                                                                Command 075
Command 076                                                                                Command 076
Command 077                                                                                Command 077
Command 078                                                                                Command 078
Command 079                                                                                Command 079
Command 080                                                                                Command 080
Command 081                                                                                Command 081
Command 082                                                                                Command 082
Command 083                                                                                Command 083
Command 084                                                                                Command 084
Command 085                                                                                Command 085
Command 086                                                                                Command 086
Command 087                                                                                Command 087
Command 088                                                                                Command 088
Command 089                                                                                Command 089
Command 090                                                                                Command 090
Command 091                                                                                Command 091
Command 092                                                                                Command 092
Command 093                                                                                Command 093
Command 094                                                                                Command 094
Command 095                                                                                Command 095
Command 096                                                                                Command 096
Command 097                                                                                Command 097
Command 098                                                                                Command 098
Command 099                                                                                Command 099
Command 100                                                                                Command 100
Command 101                                                                                Command 101
Command 102                                                                                Command 102
Command 103                                                                                Command 103
Command 104                                                                                Command 104
Command 105                                                                                Command 105
Command 106                                                                                Command 106
Command 107                                                                                Command 107
Command 108                                                                                Command 108
Command 109                                                                                Command 109
Command 110                                                                                Command 110
Command 111                                                                                Command 111
Command 112                                                                                Command 112
Command 113                                                                                Command 113
Command 114                                                                                Command 114
Command 115                                                                                Command 115
Command 116                                                                                Command 116
Command 117                                                                                Command 117
Command 118                                                                                Command 118
Command 119                                                                                Command 119
Command 120                                                                                Command 120
Command 121                                                                                Command 121
Command 122                                                                                Command 122
Command 123                                                                                Command 123
Command 124                                                                                Command 124
Command 125                                                                                Command 125
Command 126                                                                                Command 126
Command 127                                                                                Command 127
Command 128                                                                                Command 128
Command 129                                                                                Command 129
Command 130                                                                                Command 130
Command 131                                                                                Command 131
Command 132                                                                                Command 132
Command 133                                                                                Command 133
Command 134                                                                                Command 134
Command 135                                                                                Command 135
Command 136                                                                                Command 136
Command 137                                                                                Command 137
Command 138                                                                                Command 138
Command 139                                                                                Command 139
Command 140                                                                                Command 140
Command 141                                                                                Command 141
Command 142                                                                                Command 142
Command 143                                                                                Command 143
Command 144                                                                                Command 144
Command 145                                                                                Command 145
Command 146                                                                                Command 146
Command 147                                                                                Command 147
Command 148                                                                                Command 148
Command 149                                                                                Command 149
Command 150                                                                                Command 150
Command 151                                                                                Command 151
Command 152                                                                                Command 152
Command 153                                                                                Command 153
Command 154                                                                                Command 154
Command 155                                                                                Command 155
Command 156                                                                                Command 156
Command 157                                                                                Command 157
Command 158                                                                                Command 158
Command 159                                                                                Command 159
Command 160                                                                                Command 160
Command 161                                                                                Command 161
Command 162                                                                                Command 162
Command 163                                                                                Command 163
Command 164                                                                                Command 164
Command 165                                                                                Command 165
Command 166                                                                                Command 166
Command 167                                                                                Command 167
Command 168                                                                                Command 168
Command 169                                                                                Command 169
Command 170                                                                                Command 170
Command 171                                                                                Command 171
Command 172                                                                                Command 172
Command 173                                                                                Command 173
Command 174                                                                                Command 174
Command 175                                                                                Command 175
Command 176                                                                                Command 176
Command 177                                                                                Command 177
Command 178                                                                                Command 178
Command 179                                                                                Command 179
Command 180                                                                                Command 180
Command 181                                                                                Command 181
Command 182                                                                                Command 182
Command 183                                                                                Command 183
Command 184                                                                                Command 184
Command 185                                                                                Command 185
Command 186                                                                                Command 186
Command 187                                                                                Command 187
Command 188                                                                                Command 188
Command 189                                                                                Command 189
Command 190                                                                                Command 190
Command 191                                                                                Command 191
Command 192                                                                                Command 192
Command 193                                                                                Command 193
Command 194                                                                                Command 194
Command 195                                                                                Command 195
Command 196                                                                                Command 196
Command 197                                                                                Command 197
Command 198                                                                                Command 198
Command 199                                                                                Command 199
Command 200                                                                                Command 200
Command 201                                                                                Command 201
Command 202                                                                                Command 202
Command 203                                                                                Command 203
Command 204                                                                                Command 204
Command 205                                                                                Command 205
Command 206                                                                                Command 206
Command 207                                                                                Command 207
Command 208                                                                                Command 208
Command 209                                                                                Command 209
Command 210                                                                                Command 210
Command 211                                                                                Command 211
Command 212                                                                                Command 212
Command 213                                                                                Command 213
Command 214                                                                                Command 214
Command 215                                                                                Command 215
Command 216                                                                                Command 216
Command 217                                                                                Command 217
Command 218                                                                                Command 218
Command 219                                                                                Command 219
Command 220                                                                                Command 220
Command 221                                                                                Command 221
Command 222                                                                                Command 222
Command 223                                                                                Command 223
Command 224                                                                                Command 224
Command 225                                                                                Command 225
Command 226                                                                                Command 226
Command 227                                                                                Command 227
Command 228                                                                                Command 228
Command 229                                                                                Command 229
Command 230                                                                                Command 230
Command 231                                                                                Command 231
Command 232                                                                                Command 232
Command 233                                                                                Command 233
Command 234                                                                                Command 234
Command 235                                                                                Command 235
Command 236                                                                                Command 236
Command 237                                                                                Command 237
Command 238                                                                                Command 238
Command 239                                                                                Command 239
Command 240                                                                                Command 240
Command 241                                                                                Command 241
Command 242                                                                                Command 242
Command 243                                                                                Command 243
Command 244                                                                                Command 244
Command 245                                                                                Command 245
Command 246                                                                                Command 246
Command 247                                                                                Command 247
Command 248                                                                                Command 248
Command 249                                                                                Command 249
Command 250                                                                                Command 250
Command 251                                                                                Command 251
Command 252                                                                                Command 252
Command 253                                                                                Command 253
Command 254                                                                                Command 254
Command 255                                                                                Command 255
Command 256                                                                                Command 256
Command 257                                                                                Command 257
Command 258                                                                                Command 258
Command 259                                                                                Command 259
Command 260                                                                                Command 260
Command 261                                                                                Command 261
Command 262                                                                                Command 262
Command 263                                                                                Command 263
Command 264                                                                                Command 264
Command 265                                                                                Command 265
Command 266                                                                                Command 266
Command 267                                                                                Command 267
Command 268                                                                                Command 268
Command 269                                                                                Command 269
Command 270                                                                                Command 270
Command 271                                                                                Command 271
Command 272                                                                                Command 272
Command 273                                                                                Command 273
Command 274                                                                                Command 274
Command 275                                                                                Command 275
Command 276                                                                                Command 276
Command 277                                                                                Command 277
Command 278                                                                                Command 278
Command 279                                                                                Command 279
Command 280                                                                                Command 280
Command 281                                                                                Command 281
Command 282                                                                                Command 282
Command 283                                                                                Command 283
Command 284                                                                                Command 284
Command 285                                                                                Command 285
Command 286                                                                                Command 286
Command 287                                                                                Command 287
Command 288                                                                                Command 288
Command 289                                                                                Command 289
Command 290                                                                                Command 290
Command 291                                                                                Command 291
Command 292                                                                                Command 292
Command 293                                                                                Command 293
Command 294                                                                                Command 294
Command 295                                                                                Command 295
Command 296                                                                                Command 296
Command 297                                                                                Command 297
Command 298                                                                                Command 298
Command 299                                                                                Command 299
Command 300                                                                                Command 300
Command 301                                                                                Command 301
Command 302                                                                                Command 302
Command 303                                                                                Command 303
Command 304                                                                                Command 304
Command 305                                                                                Command 305
Command 306                                                                                Command 306
Command 307                                                                                Command 307
Command 308                                                                                Command 308
Command 309                                                                                Command 309
Command 310                                                                                Command 310
Command 311                                                                                Command 311
Command 312                                                                                Command 312
Command 313                                                                                Command 313
Command 314                                                                                Command 314
Command 315                                                                                Command 315
Command 316                                                                                Command 316
Command 317                                                                                Command 317
Command 318                                                                                Command 318
Command 319                                                                                Command 319
Command 320                                                                                Command 320
Command 321                                                                                Command 321
Command 322                                                                                Command 322
Command 323                                                                                Command 323
Command 324                                                                                Command 324
Command 325                                                                                Command 325
Command 326                                                                                Command 326
Command 327                                                                                Command 327
Command 328                                                                                Command 328
Command 329                                                                                Command 329
Command 330                                                                                Command 330
Command 331                                                                                Command 331
Command 332                                                                                Command 332
Command 333                                                                                Command 333
Command 334                                                                                Command 334
Command 335                                                                                Command 335
Command 336                                                                                Command 336
Command 337                                                                                Command 337
Command 338                                                                                Command 338
Command 339                                                                                Command 339
Command 340                                                                                Command 340
Command 341                                                                                Command 341
Command 342                                                                                Command 342
Command 343                                                                                Command 343
Command 344                                                                                Command 344
Command 345                                                                                Command 345
Command 346                                                                                Command 346
Command 347                                                                                Command 347
Command 348                                                                                Command 348
Command 349                                                                                Command 349
Command 350                                                                                Command 350
Command 351                                                                                Command 351
Command 352                                                                                Command 352
Command 353                                                                                Command 353
Command 354                                                                                Command 354
Command 355                                                                                Command 355
Command 356                                                                                Command 356
Command 357                                                                                Command 357
Command 358                                                                                Command 358
Command 359                                                                                Command 359
Command 360                                                                                Command 360
Command 361                                                                                Command 361
Command 362                                                                                Command 362
Command 363                                                                                Command 363
Command 364                                                                                Command 364
Command 365                                                                                Command 365
Command 366                                                                                Command 366
Command 367                                                                                Command 367
Command 368                                                                                Command 368
Command 369                                                                                Command 369
Command 370                                                                                Command 370
Command 371                                                                                Command 371
usr                                                                                        usr
srv                                                                                        srv
mnt                                                                                        mnt
lib64                                                                                      lib64
lib                                                                                        lib
var                                                                                        var
bin                                                                                        bin
lost+found                                                                                 lost+found
swapfile                                                                                   swapfile
initrd.img.old                                                                             initrd.img.old
cdrom                                                                                      cdrom
home                                                                                       home
boot                                                                                       boot
vmlinuz                                                                                    vmlinuz
sbin                                                                                       sbin
initrd.img                                                                                 initrd.img
media                                                                                      media
core                                                                                       core
opt                                                                                        opt
root                                                                                       root
etc                                                                                        etc
run                                                                                        run
proc                                                                                       proc
sys                                                                                        sys
dev                                                                                        dev
tmp                                                                                        tmp
Command 373                                                                                Command 373
Command 374                                                                                Command 374
Command 375                                                                                Command 375
Command 376                                                                                Command 376
Command 377                                                                                Command 377
Command 378                                                                                Command 378
Command 379                                                                                Command 379
Command 380                                                                                Command 380
Command 381                                                                                Command 381
HelloCommand 383                                                                           HelloCommand 383
Command 384                                                                                Command 384
Command 385                                                                                Command 385
Command 386                                                                                Command 386
Command 387                                                                                Command 387
Command 388                                                                                Command 388
Command 389                                                                                Command 389
Command 390                                                                                Command 390
Command 391                                                                                Command 391
Command 392                                                                                Command 392
COMET-PC                                                                                   COMET-PC
Command 394                                                                                Command 394
Command 395                                                                                Command 395
Command 396                                                                                Command 396
Command 397                                                                                Command 397
Command 398                                                                                Command 398
Command 399                                                                                Command 399
Command 400                                                                                Command 400
Command 401                                                                                Command 401
Command 402                                                                                Command 402
Command 403                                                                                Command 403
Command 404                                                                                Command 404
Command 405                                                                                Command 405
comet    tty7         2019-03-31 09:47 (:0)                                                comet    tty7         2019-03-31 09:47 (:0)
Command 407                                                                                Command 407
Command 408                                                                                Command 408
Command 409                                                                                Command 409
Command 410                                                                                Command 410
Command 411                                                                                Command 411
Command 412                                                                                Command 412
Command 413                                                                                Command 413
Command 414                                                                                Command 414
Command 415                                                                                Command 415
Command 416                                                                                Command 416
Command 417                                                                                Command 417
Command 418                                                                                Command 418
Command 419                                                                                Command 419
-----                                                                                      -----
usr                                                                                        usr
srv                                                                                        srv
mnt                                                                                        mnt
lib64                                                                                      lib64
lib                                                                                        lib
var                                                                                        var
bin                                                                                        bin
lost+found                                                                                 lost+found
swapfile                                                                                   swapfile
initrd.img.old                                                                             initrd.img.old
cdrom                                                                                      cdrom
home                                                                                       home
boot                                                                                       boot
vmlinuz                                                                                    vmlinuz
sbin                                                                                       sbin
initrd.img                                                                                 initrd.img
media                                                                                      media
core                                                                                       core
opt                                                                                        opt
root                                                                                       root
etc                                                                                        etc
run                                                                                        run
proc                                                                                       proc
sys                                                                                        sys
dev                                                                                        dev
tmp                                                                                        tmp
HelloCOMET-PC                                                                              HelloCOMET-PC
comet    tty7         2019-03-31 09:47 (:0)                                                comet    tty7         2019-03-31 09:47 (:0)
-----                                                                                      -----
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 12: History Execution !! [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
set -o history    # Your shell can ignore this
set -o histexpand # Your shell can ignore this
ls -1 -t -r /
!!
uname -a
!!
oeiwiowejweo
!!
echo Goodbye
!!
# -------------- End --------------

expected=$(HISTFILE=/dev/null HISTSIZE=100 bash \
    < <(echo "${script}") 2> /dev/null)
actual=$(timeout 5 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) || test_end

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
usr                                                                                        usr
srv                                                                                        srv
mnt                                                                                        mnt
lib64                                                                                      lib64
lib                                                                                        lib
var                                                                                        var
bin                                                                                        bin
lost+found                                                                                 lost+found
swapfile                                                                                   swapfile
initrd.img.old                                                                             initrd.img.old
cdrom                                                                                      cdrom
home                                                                                       home
boot                                                                                       boot
vmlinuz                                                                                    vmlinuz
sbin                                                                                       sbin
initrd.img                                                                                 initrd.img
media                                                                                      media
core                                                                                       core
opt                                                                                        opt
root                                                                                       root
etc                                                                                        etc
run                                                                                        run
proc                                                                                       proc
sys                                                                                        sys
dev                                                                                        dev
tmp                                                                                        tmp
usr                                                                                        usr
srv                                                                                        srv
mnt                                                                                        mnt
lib64                                                                                      lib64
lib                                                                                        lib
var                                                                                        var
bin                                                                                        bin
lost+found                                                                                 lost+found
swapfile                                                                                   swapfile
initrd.img.old                                                                             initrd.img.old
cdrom                                                                                      cdrom
home                                                                                       home
boot                                                                                       boot
vmlinuz                                                                                    vmlinuz
sbin                                                                                       sbin
initrd.img                                                                                 initrd.img
media                                                                                      media
core                                                                                       core
opt                                                                                        opt
root                                                                                       root
etc                                                                                        etc
run                                                                                        run
proc                                                                                       proc
sys                                                                                        sys
dev                                                                                        dev
tmp                                                                                        tmp
Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86    Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86
Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86    Linux COMET-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86
Goodbye                                                                                    Goodbye
Goodbye                                                                                    Goodbye
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 13: Basic pipe support (single pipe) [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
ls -1 / | sort
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null) || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
bin                                                                                        bin
boot                                                                                       boot
cdrom                                                                                      cdrom
core                                                                                       core
dev                                                                                        dev
etc                                                                                        etc
home                                                                                       home
initrd.img                                                                                 initrd.img
initrd.img.old                                                                             initrd.img.old
lib                                                                                        lib
lib64                                                                                      lib64
lost+found                                                                                 lost+found
media                                                                                      media
mnt                                                                                        mnt
opt                                                                                        opt
proc                                                                                       proc
root                                                                                       root
run                                                                                        run
sbin                                                                                       sbin
srv                                                                                        srv
swapfile                                                                                   swapfile
sys                                                                                        sys
tmp                                                                                        tmp
usr                                                                                        usr
var                                                                                        var
vmlinuz                                                                                    vmlinuz
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 14: Pipe with a large amount of lines [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
seq 10000000 | wc -l
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 15 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
10000000                                                                                   10000000
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 15: Long pipeline (several pipes in a single command) [2 pts]

```

# ---------- Test Script ----------
echo "${script}"
cat /etc/passwd | sort | sed s/:.*//
echo -----
cat /etc/passwd | sort | sed s/:.*// | grep comet
echo -----
cat /etc/passwd | sort | grep s | sed s/:.*// | tail -n 3 | head -n 2
echo -----
echo hi | cat | cat | cat | cat | sed s/h/y/g | sed s/i/o/g | shuf
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(timeout 15 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) \
    || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
_apt                                                                                       _apt
avahi-autoipd                                                                              avahi-autoipd
avahi                                                                                      avahi
backup                                                                                     backup
bin                                                                                        bin
colord                                                                                     colord
comet                                                                                      comet
cups-pk-helper                                                                             cups-pk-helper
daemon                                                                                     daemon
dnsmasq                                                                                    dnsmasq
games                                                                                      games
geoclue                                                                                    geoclue
gnats                                                                                      gnats
hplip                                                                                      hplip
irc                                                                                        irc
kernoops                                                                                   kernoops
lightdm                                                                                    lightdm
list                                                                                       list
lp                                                                                         lp
mail                                                                                       mail
man                                                                                        man
messagebus                                                                                 messagebus
news                                                                                       news
nm-openvpn                                                                                 nm-openvpn
nobody                                                                                     nobody
proxy                                                                                      proxy
pulse                                                                                      pulse
root                                                                                       root
rtkit                                                                                      rtkit
saned                                                                                      saned
speech-dispatcher                                                                          speech-dispatcher
sync                                                                                       sync
syslog                                                                                     syslog
systemd-network                                                                            systemd-network
systemd-resolve                                                                            systemd-resolve
sys                                                                                        sys
usbmux                                                                                     usbmux
uucp                                                                                       uucp
uuidd                                                                                      uuidd
www-data                                                                                   www-data
-----                                                                                      -----
comet                                                                                      comet
-----                                                                                      -----
uucp                                                                                       uucp
uuidd                                                                                      uuidd
-----                                                                                      -----
yo                                                                                         yo
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 16: Output redirection [1 pts]

This test writes several strings to files and ensures their results are correct. It also checks to make sure the permissions are set up correctly with open() -- use 0666.

```

# ---------- Test Script ----------
echo "${script}"
rm -f /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile1 /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile2
echo Hello world this is a nice long message to save to a file > /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile1
cat /etc/services > /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile2
echo This will overwrite the first message > /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile1
cat /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile1 /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile2 > /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/final_output.txt
cat /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/final_output.txt
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/get-stats.sh /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/final_output.txt
rm -f /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/final_output.txt
rm -f /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile1 /home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/testfile2
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null) || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
This will overwrite the first message                                                      This will overwrite the first message
# Network services, Internet style                                                         # Network services, Internet style
#                                                                                          #
# Note that it is presently the policy of IANA to assign a single well-known               # Note that it is presently the policy of IANA to assign a single well-known
# port number for both TCP and UDP; hence, officially ports have two entries               # port number for both TCP and UDP; hence, officially ports have two entries
# even if the protocol doesn't support UDP operations.                                     # even if the protocol doesn't support UDP operations.
#                                                                                          #
# Updated from http://www.iana.org/assignments/port-numbers and other                      # Updated from http://www.iana.org/assignments/port-numbers and other
# sources like http://www.freebsd.org/cgi/cvsweb.cgi/src/etc/services .                    # sources like http://www.freebsd.org/cgi/cvsweb.cgi/src/etc/services .
# New ports will be added on request if they have been officially assigned                 # New ports will be added on request if they have been officially assigned
# by IANA and used in the real-world or are needed by a debian package.                    # by IANA and used in the real-world or are needed by a debian package.
# If you need a huge list of used numbers please install the nmap package.                 # If you need a huge list of used numbers please install the nmap package.

tcpmux          1/tcp                           # TCP port service multiplexer             tcpmux          1/tcp                           # TCP port service multiplexer
echo            7/tcp                                                                      echo            7/tcp
echo            7/udp                                                                      echo            7/udp
discard         9/tcp           sink null                                                  discard         9/tcp           sink null
discard         9/udp           sink null                                                  discard         9/udp           sink null
systat          11/tcp          users                                                      systat          11/tcp          users
daytime         13/tcp                                                                     daytime         13/tcp
daytime         13/udp                                                                     daytime         13/udp
netstat         15/tcp                                                                     netstat         15/tcp
qotd            17/tcp          quote                                                      qotd            17/tcp          quote
msp             18/tcp                          # message send protocol                    msp             18/tcp                          # message send protocol
msp             18/udp                                                                     msp             18/udp
chargen         19/tcp          ttytst source                                              chargen         19/tcp          ttytst source
chargen         19/udp          ttytst source                                              chargen         19/udp          ttytst source
ftp-data        20/tcp                                                                     ftp-data        20/tcp
ftp             21/tcp                                                                     ftp             21/tcp
fsp             21/udp          fspd                                                       fsp             21/udp          fspd
ssh             22/tcp                          # SSH Remote Login Protocol                ssh             22/tcp                          # SSH Remote Login Protocol
telnet          23/tcp                                                                     telnet          23/tcp
smtp            25/tcp          mail                                                       smtp            25/tcp          mail
time            37/tcp          timserver                                                  time            37/tcp          timserver
time            37/udp          timserver                                                  time            37/udp          timserver
rlp             39/udp          resource        # resource location                        rlp             39/udp          resource        # resource location
nameserver      42/tcp          name            # IEN 116                                  nameserver      42/tcp          name            # IEN 116
whois           43/tcp          nicname                                                    whois           43/tcp          nicname
tacacs          49/tcp                          # Login Host Protocol (TACACS)             tacacs          49/tcp                          # Login Host Protocol (TACACS)
tacacs          49/udp                                                                     tacacs          49/udp
re-mail-ck      50/tcp                          # Remote Mail Checking Protocol            re-mail-ck      50/tcp                          # Remote Mail Checking Protocol
re-mail-ck      50/udp                                                                     re-mail-ck      50/udp
domain          53/tcp                          # Domain Name Server                       domain          53/tcp                          # Domain Name Server
domain          53/udp                                                                     domain          53/udp
tacacs-ds       65/tcp                          # TACACS-Database Service                  tacacs-ds       65/tcp                          # TACACS-Database Service
tacacs-ds       65/udp                                                                     tacacs-ds       65/udp
bootps          67/tcp                          # BOOTP server                             bootps          67/tcp                          # BOOTP server
bootps          67/udp                                                                     bootps          67/udp
bootpc          68/tcp                          # BOOTP client                             bootpc          68/tcp                          # BOOTP client
bootpc          68/udp                                                                     bootpc          68/udp
tftp            69/udp                                                                     tftp            69/udp
gopher          70/tcp                          # Internet Gopher                          gopher          70/tcp                          # Internet Gopher
finger          79/tcp                                                                     finger          79/tcp
http            80/tcp          www             # WorldWideWeb HTTP                        http            80/tcp          www             # WorldWideWeb HTTP
link            87/tcp          ttylink                                                    link            87/tcp          ttylink
kerberos        88/tcp          kerberos5 krb5 kerberos-sec     # Kerberos v5              kerberos        88/tcp          kerberos5 krb5 kerberos-sec     # Kerberos v5
kerberos        88/udp          kerberos5 krb5 kerberos-sec     # Kerberos v5              kerberos        88/udp          kerberos5 krb5 kerberos-sec     # Kerberos v5
supdup          95/tcp                                                                     supdup          95/tcp
hostnames       101/tcp         hostname        # usually from sri-nic                     hostnames       101/tcp         hostname        # usually from sri-nic
iso-tsap        102/tcp         tsap            # part of ISODE                            iso-tsap        102/tcp         tsap            # part of ISODE
acr-nema        104/tcp         dicom           # Digital Imag. & Comm. 300                acr-nema        104/tcp         dicom           # Digital Imag. & Comm. 300
acr-nema        104/udp         dicom                                                      acr-nema        104/udp         dicom
csnet-ns        105/tcp         cso-ns          # also used by CSO name server             csnet-ns        105/tcp         cso-ns          # also used by CSO name server
csnet-ns        105/udp         cso-ns                                                     csnet-ns        105/udp         cso-ns
rtelnet         107/tcp                         # Remote Telnet                            rtelnet         107/tcp                         # Remote Telnet
rtelnet         107/udp                                                                    rtelnet         107/udp
pop3            110/tcp         pop-3           # POP version 3                            pop3            110/tcp         pop-3           # POP version 3
sunrpc          111/tcp         portmapper      # RPC 4.0 portmapper                       sunrpc          111/tcp         portmapper      # RPC 4.0 portmapper
sunrpc          111/udp         portmapper                                                 sunrpc          111/udp         portmapper
auth            113/tcp         authentication tap ident                                   auth            113/tcp         authentication tap ident
sftp            115/tcp                                                                    sftp            115/tcp
nntp            119/tcp         readnews untp   # USENET News Transfer Protocol            nntp            119/tcp         readnews untp   # USENET News Transfer Protocol
ntp             123/tcp                                                                    ntp             123/tcp
ntp             123/udp                         # Network Time Protocol                    ntp             123/udp                         # Network Time Protocol
pwdgen          129/tcp                         # PWDGEN service                           pwdgen          129/tcp                         # PWDGEN service
pwdgen          129/udp                                                                    pwdgen          129/udp
loc-srv         135/tcp         epmap           # Location Service                         loc-srv         135/tcp         epmap           # Location Service
loc-srv         135/udp         epmap                                                      loc-srv         135/udp         epmap
netbios-ns      137/tcp                         # NETBIOS Name Service                     netbios-ns      137/tcp                         # NETBIOS Name Service
netbios-ns      137/udp                                                                    netbios-ns      137/udp
netbios-dgm     138/tcp                         # NETBIOS Datagram Service                 netbios-dgm     138/tcp                         # NETBIOS Datagram Service
netbios-dgm     138/udp                                                                    netbios-dgm     138/udp
netbios-ssn     139/tcp                         # NETBIOS session service                  netbios-ssn     139/tcp                         # NETBIOS session service
netbios-ssn     139/udp                                                                    netbios-ssn     139/udp
imap2           143/tcp         imap            # Interim Mail Access P 2 and 4            imap2           143/tcp         imap            # Interim Mail Access P 2 and 4
snmp            161/tcp                         # Simple Net Mgmt Protocol                 snmp            161/tcp                         # Simple Net Mgmt Protocol
snmp            161/udp                                                                    snmp            161/udp
snmp-trap       162/tcp         snmptrap        # Traps for SNMP                           snmp-trap       162/tcp         snmptrap        # Traps for SNMP
snmp-trap       162/udp         snmptrap                                                   snmp-trap       162/udp         snmptrap
cmip-man        163/tcp                         # ISO mgmt over IP (CMOT)                  cmip-man        163/tcp                         # ISO mgmt over IP (CMOT)
cmip-man        163/udp                                                                    cmip-man        163/udp
cmip-agent      164/tcp                                                                    cmip-agent      164/tcp
cmip-agent      164/udp                                                                    cmip-agent      164/udp
mailq           174/tcp                 # Mailer transport queue for Zmailer               mailq           174/tcp                 # Mailer transport queue for Zmailer
mailq           174/udp                                                                    mailq           174/udp
xdmcp           177/tcp                         # X Display Mgr. Control Proto             xdmcp           177/tcp                         # X Display Mgr. Control Proto
xdmcp           177/udp                                                                    xdmcp           177/udp
nextstep        178/tcp         NeXTStep NextStep       # NeXTStep window                  nextstep        178/tcp         NeXTStep NextStep       # NeXTStep window
nextstep        178/udp         NeXTStep NextStep       #  server                          nextstep        178/udp         NeXTStep NextStep       #  server
bgp             179/tcp                         # Border Gateway Protocol                  bgp             179/tcp                         # Border Gateway Protocol
irc             194/tcp                         # Internet Relay Chat                      irc             194/tcp                         # Internet Relay Chat
irc             194/udp                                                                    irc             194/udp
smux            199/tcp                         # SNMP Unix Multiplexer                    smux            199/tcp                         # SNMP Unix Multiplexer
smux            199/udp                                                                    smux            199/udp
at-rtmp         201/tcp                         # AppleTalk routing                        at-rtmp         201/tcp                         # AppleTalk routing
at-rtmp         201/udp                                                                    at-rtmp         201/udp
at-nbp          202/tcp                         # AppleTalk name binding                   at-nbp          202/tcp                         # AppleTalk name binding
at-nbp          202/udp                                                                    at-nbp          202/udp
at-echo         204/tcp                         # AppleTalk echo                           at-echo         204/tcp                         # AppleTalk echo
at-echo         204/udp                                                                    at-echo         204/udp
at-zis          206/tcp                         # AppleTalk zone information               at-zis          206/tcp                         # AppleTalk zone information
at-zis          206/udp                                                                    at-zis          206/udp
qmtp            209/tcp                         # Quick Mail Transfer Protocol             qmtp            209/tcp                         # Quick Mail Transfer Protocol
qmtp            209/udp                                                                    qmtp            209/udp
z3950           210/tcp         wais            # NISO Z39.50 database                     z3950           210/tcp         wais            # NISO Z39.50 database
z3950           210/udp         wais                                                       z3950           210/udp         wais
ipx             213/tcp                         # IPX                                      ipx             213/tcp                         # IPX
ipx             213/udp                                                                    ipx             213/udp
pawserv         345/tcp                         # Perf Analysis Workbench                  pawserv         345/tcp                         # Perf Analysis Workbench
pawserv         345/udp                                                                    pawserv         345/udp
zserv           346/tcp                         # Zebra server                             zserv           346/tcp                         # Zebra server
zserv           346/udp                                                                    zserv           346/udp
fatserv         347/tcp                         # Fatmen Server                            fatserv         347/tcp                         # Fatmen Server
fatserv         347/udp                                                                    fatserv         347/udp
rpc2portmap     369/tcp                                                                    rpc2portmap     369/tcp
rpc2portmap     369/udp                         # Coda portmapper                          rpc2portmap     369/udp                         # Coda portmapper
codaauth2       370/tcp                                                                    codaauth2       370/tcp
codaauth2       370/udp                         # Coda authentication server               codaauth2       370/udp                         # Coda authentication server
clearcase       371/tcp         Clearcase                                                  clearcase       371/tcp         Clearcase
clearcase       371/udp         Clearcase                                                  clearcase       371/udp         Clearcase
ulistserv       372/tcp                         # UNIX Listserv                            ulistserv       372/tcp                         # UNIX Listserv
ulistserv       372/udp                                                                    ulistserv       372/udp
ldap            389/tcp                 # Lightweight Directory Access Protocol            ldap            389/tcp                 # Lightweight Directory Access Protocol
ldap            389/udp                                                                    ldap            389/udp
imsp            406/tcp                 # Interactive Mail Support Protocol                imsp            406/tcp                 # Interactive Mail Support Protocol
imsp            406/udp                                                                    imsp            406/udp
svrloc          427/tcp                         # Server Location                          svrloc          427/tcp                         # Server Location
svrloc          427/udp                                                                    svrloc          427/udp
https           443/tcp                         # http protocol over TLS/SSL               https           443/tcp                         # http protocol over TLS/SSL
snpp            444/tcp                         # Simple Network Paging Protocol           snpp            444/tcp                         # Simple Network Paging Protocol
snpp            444/udp                                                                    snpp            444/udp
microsoft-ds    445/tcp                         # Microsoft Naked CIFS                     microsoft-ds    445/tcp                         # Microsoft Naked CIFS
microsoft-ds    445/udp                                                                    microsoft-ds    445/udp
kpasswd         464/tcp                                                                    kpasswd         464/tcp
kpasswd         464/udp                                                                    kpasswd         464/udp
urd             465/tcp         ssmtp smtps  # URL Rendesvous Directory for SSM            urd             465/tcp         ssmtp smtps  # URL Rendesvous Directory for SSM
saft            487/tcp                 # Simple Asynchronous File Transfer                saft            487/tcp                 # Simple Asynchronous File Transfer
saft            487/udp                                                                    saft            487/udp
isakmp          500/tcp                 # IPsec - Internet Security Association            isakmp          500/tcp                 # IPsec - Internet Security Association
isakmp          500/udp                 #  and Key Management Protocol                     isakmp          500/udp                 #  and Key Management Protocol
rtsp            554/tcp                 # Real Time Stream Control Protocol                rtsp            554/tcp                 # Real Time Stream Control Protocol
rtsp            554/udp                                                                    rtsp            554/udp
nqs             607/tcp                         # Network Queuing system                   nqs             607/tcp                         # Network Queuing system
nqs             607/udp                                                                    nqs             607/udp
npmp-local      610/tcp         dqs313_qmaster          # npmp-local / DQS                 npmp-local      610/tcp         dqs313_qmaster          # npmp-local / DQS
npmp-local      610/udp         dqs313_qmaster                                             npmp-local      610/udp         dqs313_qmaster
npmp-gui        611/tcp         dqs313_execd            # npmp-gui / DQS                   npmp-gui        611/tcp         dqs313_execd            # npmp-gui / DQS
npmp-gui        611/udp         dqs313_execd                                               npmp-gui        611/udp         dqs313_execd
hmmp-ind        612/tcp         dqs313_intercell        # HMMP Indication / DQS            hmmp-ind        612/tcp         dqs313_intercell        # HMMP Indication / DQS
hmmp-ind        612/udp         dqs313_intercell                                           hmmp-ind        612/udp         dqs313_intercell
asf-rmcp        623/udp         # ASF Remote Management and Control Protocol               asf-rmcp        623/udp         # ASF Remote Management and Control Protocol
qmqp            628/tcp                                                                    qmqp            628/tcp
qmqp            628/udp                                                                    qmqp            628/udp
ipp             631/tcp                         # Internet Printing Protocol               ipp             631/tcp                         # Internet Printing Protocol
ipp             631/udp                                                                    ipp             631/udp
#                                                                                          #
# UNIX specific services                                                                   # UNIX specific services
#                                                                                          #
exec            512/tcp                                                                    exec            512/tcp
biff            512/udp         comsat                                                     biff            512/udp         comsat
login           513/tcp                                                                    login           513/tcp
who             513/udp         whod                                                       who             513/udp         whod
shell           514/tcp         cmd             # no passwords used                        shell           514/tcp         cmd             # no passwords used
syslog          514/udp                                                                    syslog          514/udp
printer         515/tcp         spooler         # line printer spooler                     printer         515/tcp         spooler         # line printer spooler
talk            517/udp                                                                    talk            517/udp
ntalk           518/udp                                                                    ntalk           518/udp
route           520/udp         router routed   # RIP                                      route           520/udp         router routed   # RIP
timed           525/udp         timeserver                                                 timed           525/udp         timeserver
tempo           526/tcp         newdate                                                    tempo           526/tcp         newdate
courier         530/tcp         rpc                                                        courier         530/tcp         rpc
conference      531/tcp         chat                                                       conference      531/tcp         chat
netnews         532/tcp         readnews                                                   netnews         532/tcp         readnews
netwall         533/udp                         # for emergency broadcasts                 netwall         533/udp                         # for emergency broadcasts
gdomap          538/tcp                         # GNUstep distributed objects              gdomap          538/tcp                         # GNUstep distributed objects
gdomap          538/udp                                                                    gdomap          538/udp
uucp            540/tcp         uucpd           # uucp daemon                              uucp            540/tcp         uucpd           # uucp daemon
klogin          543/tcp                         # Kerberized `rlogin' (v5)                 klogin          543/tcp                         # Kerberized `rlogin' (v5)
kshell          544/tcp         krcmd           # Kerberized `rsh' (v5)                    kshell          544/tcp         krcmd           # Kerberized `rsh' (v5)
dhcpv6-client   546/tcp                                                                    dhcpv6-client   546/tcp
dhcpv6-client   546/udp                                                                    dhcpv6-client   546/udp
dhcpv6-server   547/tcp                                                                    dhcpv6-server   547/tcp
dhcpv6-server   547/udp                                                                    dhcpv6-server   547/udp
afpovertcp      548/tcp                         # AFP over TCP                             afpovertcp      548/tcp                         # AFP over TCP
afpovertcp      548/udp                                                                    afpovertcp      548/udp
idfp            549/tcp                                                                    idfp            549/tcp
idfp            549/udp                                                                    idfp            549/udp
remotefs        556/tcp         rfs_server rfs  # Brunhoff remote filesystem               remotefs        556/tcp         rfs_server rfs  # Brunhoff remote filesystem
nntps           563/tcp         snntp           # NNTP over SSL                            nntps           563/tcp         snntp           # NNTP over SSL
submission      587/tcp                         # Submission [RFC4409]                     submission      587/tcp                         # Submission [RFC4409]
ldaps           636/tcp                         # LDAP over SSL                            ldaps           636/tcp                         # LDAP over SSL
ldaps           636/udp                                                                    ldaps           636/udp
tinc            655/tcp                         # tinc control port                        tinc            655/tcp                         # tinc control port
tinc            655/udp                                                                    tinc            655/udp
silc            706/tcp                                                                    silc            706/tcp
silc            706/udp                                                                    silc            706/udp
kerberos-adm    749/tcp                         # Kerberos `kadmin' (v5)                   kerberos-adm    749/tcp                         # Kerberos `kadmin' (v5)
#                                                                                          #
webster         765/tcp                         # Network dictionary                       webster         765/tcp                         # Network dictionary
webster         765/udp                                                                    webster         765/udp
rsync           873/tcp                                                                    rsync           873/tcp
ftps-data       989/tcp                         # FTP over SSL (data)                      ftps-data       989/tcp                         # FTP over SSL (data)
ftps            990/tcp                                                                    ftps            990/tcp
telnets         992/tcp                         # Telnet over SSL                          telnets         992/tcp                         # Telnet over SSL
imaps           993/tcp                         # IMAP over SSL                            imaps           993/tcp                         # IMAP over SSL
pop3s           995/tcp                         # POP-3 over SSL                           pop3s           995/tcp                         # POP-3 over SSL
#                                                                                          #
# From ``Assigned Numbers'':                                                               # From ``Assigned Numbers'':
#                                                                                          #
#> The Registered Ports are not controlled by the IANA and on most systems                 #> The Registered Ports are not controlled by the IANA and on most systems
#> can be used by ordinary user processes or programs executed by ordinary                 #> can be used by ordinary user processes or programs executed by ordinary
#> users.                                                                                  #> users.
#                                                                                          #
#> Ports are used in the TCP [45,106] to name the ends of logical                          #> Ports are used in the TCP [45,106] to name the ends of logical
#> connections which carry long term conversations.  For the purpose of                    #> connections which carry long term conversations.  For the purpose of
#> providing services to unknown callers, a service contact port is                        #> providing services to unknown callers, a service contact port is
#> defined.  This list specifies the port used by the server process as its                #> defined.  This list specifies the port used by the server process as its
#> contact port.  While the IANA can not control uses of these ports it                    #> contact port.  While the IANA can not control uses of these ports it
#> does register or list uses of these ports as a convienence to the                       #> does register or list uses of these ports as a convienence to the
#> community.                                                                              #> community.
#                                                                                          #
socks           1080/tcp                        # socks proxy server                       socks           1080/tcp                        # socks proxy server
socks           1080/udp                                                                   socks           1080/udp
proofd          1093/tcp                                                                   proofd          1093/tcp
proofd          1093/udp                                                                   proofd          1093/udp
rootd           1094/tcp                                                                   rootd           1094/tcp
rootd           1094/udp                                                                   rootd           1094/udp
openvpn         1194/tcp                                                                   openvpn         1194/tcp
openvpn         1194/udp                                                                   openvpn         1194/udp
rmiregistry     1099/tcp                        # Java RMI Registry                        rmiregistry     1099/tcp                        # Java RMI Registry
rmiregistry     1099/udp                                                                   rmiregistry     1099/udp
kazaa           1214/tcp                                                                   kazaa           1214/tcp
kazaa           1214/udp                                                                   kazaa           1214/udp
nessus          1241/tcp                        # Nessus vulnerability                     nessus          1241/tcp                        # Nessus vulnerability
nessus          1241/udp                        #  assessment scanner                      nessus          1241/udp                        #  assessment scanner
lotusnote       1352/tcp        lotusnotes      # Lotus Note                               lotusnote       1352/tcp        lotusnotes      # Lotus Note
lotusnote       1352/udp        lotusnotes                                                 lotusnote       1352/udp        lotusnotes
ms-sql-s        1433/tcp                        # Microsoft SQL Server                     ms-sql-s        1433/tcp                        # Microsoft SQL Server
ms-sql-s        1433/udp                                                                   ms-sql-s        1433/udp
ms-sql-m        1434/tcp                        # Microsoft SQL Monitor                    ms-sql-m        1434/tcp                        # Microsoft SQL Monitor
ms-sql-m        1434/udp                                                                   ms-sql-m        1434/udp
ingreslock      1524/tcp                                                                   ingreslock      1524/tcp
ingreslock      1524/udp                                                                   ingreslock      1524/udp
datametrics     1645/tcp        old-radius                                                 datametrics     1645/tcp        old-radius
datametrics     1645/udp        old-radius                                                 datametrics     1645/udp        old-radius
sa-msg-port     1646/tcp        old-radacct                                                sa-msg-port     1646/tcp        old-radacct
sa-msg-port     1646/udp        old-radacct                                                sa-msg-port     1646/udp        old-radacct
kermit          1649/tcp                                                                   kermit          1649/tcp
kermit          1649/udp                                                                   kermit          1649/udp
groupwise       1677/tcp                                                                   groupwise       1677/tcp
groupwise       1677/udp                                                                   groupwise       1677/udp
l2f             1701/tcp        l2tp                                                       l2f             1701/tcp        l2tp
l2f             1701/udp        l2tp                                                       l2f             1701/udp        l2tp
radius          1812/tcp                                                                   radius          1812/tcp
radius          1812/udp                                                                   radius          1812/udp
radius-acct     1813/tcp        radacct         # Radius Accounting                        radius-acct     1813/tcp        radacct         # Radius Accounting
radius-acct     1813/udp        radacct                                                    radius-acct     1813/udp        radacct
msnp            1863/tcp                        # MSN Messenger                            msnp            1863/tcp                        # MSN Messenger
msnp            1863/udp                                                                   msnp            1863/udp
unix-status     1957/tcp                        # remstats unix-status server              unix-status     1957/tcp                        # remstats unix-status server
log-server      1958/tcp                        # remstats log server                      log-server      1958/tcp                        # remstats log server
remoteping      1959/tcp                        # remstats remoteping server               remoteping      1959/tcp                        # remstats remoteping server
cisco-sccp      2000/tcp                        # Cisco SCCP                               cisco-sccp      2000/tcp                        # Cisco SCCP
cisco-sccp      2000/udp                                                                   cisco-sccp      2000/udp
search          2010/tcp        ndtp                                                       search          2010/tcp        ndtp
pipe-server     2010/tcp        pipe_server                                                pipe-server     2010/tcp        pipe_server
nfs             2049/tcp                        # Network File System                      nfs             2049/tcp                        # Network File System
nfs             2049/udp                        # Network File System                      nfs             2049/udp                        # Network File System
gnunet          2086/tcp                                                                   gnunet          2086/tcp
gnunet          2086/udp                                                                   gnunet          2086/udp
rtcm-sc104      2101/tcp                        # RTCM SC-104 IANA 1/29/99                 rtcm-sc104      2101/tcp                        # RTCM SC-104 IANA 1/29/99
rtcm-sc104      2101/udp                                                                   rtcm-sc104      2101/udp
gsigatekeeper   2119/tcp                                                                   gsigatekeeper   2119/tcp
gsigatekeeper   2119/udp                                                                   gsigatekeeper   2119/udp
gris            2135/tcp                # Grid Resource Information Server                 gris            2135/tcp                # Grid Resource Information Server
gris            2135/udp                                                                   gris            2135/udp
cvspserver      2401/tcp                        # CVS client/server operations             cvspserver      2401/tcp                        # CVS client/server operations
cvspserver      2401/udp                                                                   cvspserver      2401/udp
venus           2430/tcp                        # codacon port                             venus           2430/tcp                        # codacon port
venus           2430/udp                        # Venus callback/wbc interface             venus           2430/udp                        # Venus callback/wbc interface
venus-se        2431/tcp                        # tcp side effects                         venus-se        2431/tcp                        # tcp side effects
venus-se        2431/udp                        # udp sftp side effect                     venus-se        2431/udp                        # udp sftp side effect
codasrv         2432/tcp                        # not used                                 codasrv         2432/tcp                        # not used
codasrv         2432/udp                        # server port                              codasrv         2432/udp                        # server port
codasrv-se      2433/tcp                        # tcp side effects                         codasrv-se      2433/tcp                        # tcp side effects
codasrv-se      2433/udp                        # udp sftp side effect                     codasrv-se      2433/udp                        # udp sftp side effect
mon             2583/tcp                        # MON traps                                mon             2583/tcp                        # MON traps
mon             2583/udp                                                                   mon             2583/udp
dict            2628/tcp                        # Dictionary server                        dict            2628/tcp                        # Dictionary server
dict            2628/udp                                                                   dict            2628/udp
f5-globalsite   2792/tcp                                                                   f5-globalsite   2792/tcp
f5-globalsite   2792/udp                                                                   f5-globalsite   2792/udp
gsiftp          2811/tcp                                                                   gsiftp          2811/tcp
gsiftp          2811/udp                                                                   gsiftp          2811/udp
gpsd            2947/tcp                                                                   gpsd            2947/tcp
gpsd            2947/udp                                                                   gpsd            2947/udp
gds-db          3050/tcp        gds_db          # InterBase server                         gds-db          3050/tcp        gds_db          # InterBase server
gds-db          3050/udp        gds_db                                                     gds-db          3050/udp        gds_db
icpv2           3130/tcp        icp             # Internet Cache Protocol                  icpv2           3130/tcp        icp             # Internet Cache Protocol
icpv2           3130/udp        icp                                                        icpv2           3130/udp        icp
isns            3205/tcp                        # iSNS Server Port                         isns            3205/tcp                        # iSNS Server Port
isns            3205/udp                        # iSNS Server Port                         isns            3205/udp                        # iSNS Server Port
iscsi-target    3260/tcp                                                                   iscsi-target    3260/tcp
mysql           3306/tcp                                                                   mysql           3306/tcp
mysql           3306/udp                                                                   mysql           3306/udp
nut             3493/tcp                        # Network UPS Tools                        nut             3493/tcp                        # Network UPS Tools
nut             3493/udp                                                                   nut             3493/udp
distcc          3632/tcp                        # distributed compiler                     distcc          3632/tcp                        # distributed compiler
distcc          3632/udp                                                                   distcc          3632/udp
daap            3689/tcp                        # Digital Audio Access Protocol            daap            3689/tcp                        # Digital Audio Access Protocol
daap            3689/udp                                                                   daap            3689/udp
svn             3690/tcp        subversion      # Subversion protocol                      svn             3690/tcp        subversion      # Subversion protocol
svn             3690/udp        subversion                                                 svn             3690/udp        subversion
suucp           4031/tcp                        # UUCP over SSL                            suucp           4031/tcp                        # UUCP over SSL
suucp           4031/udp                                                                   suucp           4031/udp
sysrqd          4094/tcp                        # sysrq daemon                             sysrqd          4094/tcp                        # sysrq daemon
sysrqd          4094/udp                                                                   sysrqd          4094/udp
sieve           4190/tcp                        # ManageSieve Protocol                     sieve           4190/tcp                        # ManageSieve Protocol
epmd            4369/tcp                        # Erlang Port Mapper Daemon                epmd            4369/tcp                        # Erlang Port Mapper Daemon
epmd            4369/udp                                                                   epmd            4369/udp
remctl          4373/tcp                # Remote Authenticated Command Service             remctl          4373/tcp                # Remote Authenticated Command Service
remctl          4373/udp                                                                   remctl          4373/udp
f5-iquery       4353/tcp                        # F5 iQuery                                f5-iquery       4353/tcp                        # F5 iQuery
f5-iquery       4353/udp                                                                   f5-iquery       4353/udp
ipsec-nat-t     4500/udp                        # IPsec NAT-Traversal [RFC3947]            ipsec-nat-t     4500/udp                        # IPsec NAT-Traversal [RFC3947]
iax             4569/tcp                        # Inter-Asterisk eXchange                  iax             4569/tcp                        # Inter-Asterisk eXchange
iax             4569/udp                                                                   iax             4569/udp
mtn             4691/tcp                        # monotone Netsync Protocol                mtn             4691/tcp                        # monotone Netsync Protocol
mtn             4691/udp                                                                   mtn             4691/udp
radmin-port     4899/tcp                        # RAdmin Port                              radmin-port     4899/tcp                        # RAdmin Port
radmin-port     4899/udp                                                                   radmin-port     4899/udp
rfe             5002/udp                        # Radio Free Ethernet                      rfe             5002/udp                        # Radio Free Ethernet
rfe             5002/tcp                                                                   rfe             5002/tcp
mmcc            5050/tcp        # multimedia conference control tool (Yahoo IM)            mmcc            5050/tcp        # multimedia conference control tool (Yahoo IM)
mmcc            5050/udp                                                                   mmcc            5050/udp
sip             5060/tcp                        # Session Initiation Protocol              sip             5060/tcp                        # Session Initiation Protocol
sip             5060/udp                                                                   sip             5060/udp
sip-tls         5061/tcp                                                                   sip-tls         5061/tcp
sip-tls         5061/udp                                                                   sip-tls         5061/udp
aol             5190/tcp                        # AIM                                      aol             5190/tcp                        # AIM
aol             5190/udp                                                                   aol             5190/udp
xmpp-client     5222/tcp        jabber-client   # Jabber Client Connection                 xmpp-client     5222/tcp        jabber-client   # Jabber Client Connection
xmpp-client     5222/udp        jabber-client                                              xmpp-client     5222/udp        jabber-client
xmpp-server     5269/tcp        jabber-server   # Jabber Server Connection                 xmpp-server     5269/tcp        jabber-server   # Jabber Server Connection
xmpp-server     5269/udp        jabber-server                                              xmpp-server     5269/udp        jabber-server
cfengine        5308/tcp                                                                   cfengine        5308/tcp
cfengine        5308/udp                                                                   cfengine        5308/udp
mdns            5353/tcp                        # Multicast DNS                            mdns            5353/tcp                        # Multicast DNS
mdns            5353/udp                                                                   mdns            5353/udp
postgresql      5432/tcp        postgres        # PostgreSQL Database                      postgresql      5432/tcp        postgres        # PostgreSQL Database
postgresql      5432/udp        postgres                                                   postgresql      5432/udp        postgres
freeciv         5556/tcp        rptp            # Freeciv gameplay                         freeciv         5556/tcp        rptp            # Freeciv gameplay
freeciv         5556/udp                                                                   freeciv         5556/udp
amqps           5671/tcp                        # AMQP protocol over TLS/SSL               amqps           5671/tcp                        # AMQP protocol over TLS/SSL
amqp            5672/tcp                                                                   amqp            5672/tcp
amqp            5672/udp                                                                   amqp            5672/udp
amqp            5672/sctp                                                                  amqp            5672/sctp
ggz             5688/tcp                        # GGZ Gaming Zone                          ggz             5688/tcp                        # GGZ Gaming Zone
ggz             5688/udp                                                                   ggz             5688/udp
x11             6000/tcp        x11-0           # X Window System                          x11             6000/tcp        x11-0           # X Window System
x11             6000/udp        x11-0                                                      x11             6000/udp        x11-0
x11-1           6001/tcp                                                                   x11-1           6001/tcp
x11-1           6001/udp                                                                   x11-1           6001/udp
x11-2           6002/tcp                                                                   x11-2           6002/tcp
x11-2           6002/udp                                                                   x11-2           6002/udp
x11-3           6003/tcp                                                                   x11-3           6003/tcp
x11-3           6003/udp                                                                   x11-3           6003/udp
x11-4           6004/tcp                                                                   x11-4           6004/tcp
x11-4           6004/udp                                                                   x11-4           6004/udp
x11-5           6005/tcp                                                                   x11-5           6005/tcp
x11-5           6005/udp                                                                   x11-5           6005/udp
x11-6           6006/tcp                                                                   x11-6           6006/tcp
x11-6           6006/udp                                                                   x11-6           6006/udp
x11-7           6007/tcp                                                                   x11-7           6007/tcp
x11-7           6007/udp                                                                   x11-7           6007/udp
gnutella-svc    6346/tcp                        # gnutella                                 gnutella-svc    6346/tcp                        # gnutella
gnutella-svc    6346/udp                                                                   gnutella-svc    6346/udp
gnutella-rtr    6347/tcp                        # gnutella                                 gnutella-rtr    6347/tcp                        # gnutella
gnutella-rtr    6347/udp                                                                   gnutella-rtr    6347/udp
sge-qmaster     6444/tcp        sge_qmaster     # Grid Engine Qmaster Service              sge-qmaster     6444/tcp        sge_qmaster     # Grid Engine Qmaster Service
sge-qmaster     6444/udp        sge_qmaster                                                sge-qmaster     6444/udp        sge_qmaster
sge-execd       6445/tcp        sge_execd       # Grid Engine Execution Service            sge-execd       6445/tcp        sge_execd       # Grid Engine Execution Service
sge-execd       6445/udp        sge_execd                                                  sge-execd       6445/udp        sge_execd
mysql-proxy     6446/tcp                        # MySQL Proxy                              mysql-proxy     6446/tcp                        # MySQL Proxy
mysql-proxy     6446/udp                                                                   mysql-proxy     6446/udp
babel           6696/udp                        # Babel Routing Protocol                   babel           6696/udp                        # Babel Routing Protocol
ircs-u          6697/tcp                # Internet Relay Chat via TLS/SSL                  ircs-u          6697/tcp                # Internet Relay Chat via TLS/SSL
afs3-fileserver 7000/tcp        bbs             # file server itself                       afs3-fileserver 7000/tcp        bbs             # file server itself
afs3-fileserver 7000/udp        bbs                                                        afs3-fileserver 7000/udp        bbs
afs3-callback   7001/tcp                        # callbacks to cache managers              afs3-callback   7001/tcp                        # callbacks to cache managers
afs3-callback   7001/udp                                                                   afs3-callback   7001/udp
afs3-prserver   7002/tcp                        # users & groups database                  afs3-prserver   7002/tcp                        # users & groups database
afs3-prserver   7002/udp                                                                   afs3-prserver   7002/udp
afs3-vlserver   7003/tcp                        # volume location database                 afs3-vlserver   7003/tcp                        # volume location database
afs3-vlserver   7003/udp                                                                   afs3-vlserver   7003/udp
afs3-kaserver   7004/tcp                        # AFS/Kerberos authentication              afs3-kaserver   7004/tcp                        # AFS/Kerberos authentication
afs3-kaserver   7004/udp                                                                   afs3-kaserver   7004/udp
afs3-volser     7005/tcp                        # volume managment server                  afs3-volser     7005/tcp                        # volume managment server
afs3-volser     7005/udp                                                                   afs3-volser     7005/udp
afs3-errors     7006/tcp                        # error interpretation service             afs3-errors     7006/tcp                        # error interpretation service
afs3-errors     7006/udp                                                                   afs3-errors     7006/udp
afs3-bos        7007/tcp                        # basic overseer process                   afs3-bos        7007/tcp                        # basic overseer process
afs3-bos        7007/udp                                                                   afs3-bos        7007/udp
afs3-update     7008/tcp                        # server-to-server updater                 afs3-update     7008/tcp                        # server-to-server updater
afs3-update     7008/udp                                                                   afs3-update     7008/udp
afs3-rmtsys     7009/tcp                        # remote cache manager service             afs3-rmtsys     7009/tcp                        # remote cache manager service
afs3-rmtsys     7009/udp                                                                   afs3-rmtsys     7009/udp
font-service    7100/tcp        xfs             # X Font Service                           font-service    7100/tcp        xfs             # X Font Service
font-service    7100/udp        xfs                                                        font-service    7100/udp        xfs
http-alt        8080/tcp        webcache        # WWW caching service                      http-alt        8080/tcp        webcache        # WWW caching service
http-alt        8080/udp                                                                   http-alt        8080/udp
puppet          8140/tcp                        # The Puppet master service                puppet          8140/tcp                        # The Puppet master service
bacula-dir      9101/tcp                        # Bacula Director                          bacula-dir      9101/tcp                        # Bacula Director
bacula-dir      9101/udp                                                                   bacula-dir      9101/udp
bacula-fd       9102/tcp                        # Bacula File Daemon                       bacula-fd       9102/tcp                        # Bacula File Daemon
bacula-fd       9102/udp                                                                   bacula-fd       9102/udp
bacula-sd       9103/tcp                        # Bacula Storage Daemon                    bacula-sd       9103/tcp                        # Bacula Storage Daemon
bacula-sd       9103/udp                                                                   bacula-sd       9103/udp
xmms2           9667/tcp        # Cross-platform Music Multiplexing System                 xmms2           9667/tcp        # Cross-platform Music Multiplexing System
xmms2           9667/udp                                                                   xmms2           9667/udp
nbd             10809/tcp                       # Linux Network Block Device               nbd             10809/tcp                       # Linux Network Block Device
zabbix-agent    10050/tcp                       # Zabbix Agent                             zabbix-agent    10050/tcp                       # Zabbix Agent
zabbix-agent    10050/udp                                                                  zabbix-agent    10050/udp
zabbix-trapper  10051/tcp                       # Zabbix Trapper                           zabbix-trapper  10051/tcp                       # Zabbix Trapper
zabbix-trapper  10051/udp                                                                  zabbix-trapper  10051/udp
amanda          10080/tcp                       # amanda backup services                   amanda          10080/tcp                       # amanda backup services
amanda          10080/udp                                                                  amanda          10080/udp
dicom           11112/tcp                                                                  dicom           11112/tcp
hkp             11371/tcp                       # OpenPGP HTTP Keyserver                   hkp             11371/tcp                       # OpenPGP HTTP Keyserver
hkp             11371/udp                                                                  hkp             11371/udp
bprd            13720/tcp                       # VERITAS NetBackup                        bprd            13720/tcp                       # VERITAS NetBackup
bprd            13720/udp                                                                  bprd            13720/udp
bpdbm           13721/tcp                       # VERITAS NetBackup                        bpdbm           13721/tcp                       # VERITAS NetBackup
bpdbm           13721/udp                                                                  bpdbm           13721/udp
bpjava-msvc     13722/tcp                       # BP Java MSVC Protocol                    bpjava-msvc     13722/tcp                       # BP Java MSVC Protocol
bpjava-msvc     13722/udp                                                                  bpjava-msvc     13722/udp
vnetd           13724/tcp                       # Veritas Network Utility                  vnetd           13724/tcp                       # Veritas Network Utility
vnetd           13724/udp                                                                  vnetd           13724/udp
bpcd            13782/tcp                       # VERITAS NetBackup                        bpcd            13782/tcp                       # VERITAS NetBackup
bpcd            13782/udp                                                                  bpcd            13782/udp
vopied          13783/tcp                       # VERITAS NetBackup                        vopied          13783/tcp                       # VERITAS NetBackup
vopied          13783/udp                                                                  vopied          13783/udp
db-lsp          17500/tcp                       # Dropbox LanSync Protocol                 db-lsp          17500/tcp                       # Dropbox LanSync Protocol
dcap            22125/tcp                       # dCache Access Protocol                   dcap            22125/tcp                       # dCache Access Protocol
gsidcap         22128/tcp                       # GSI dCache Access Protocol               gsidcap         22128/tcp                       # GSI dCache Access Protocol
wnn6            22273/tcp                       # wnn6                                     wnn6            22273/tcp                       # wnn6
wnn6            22273/udp                                                                  wnn6            22273/udp

#                                                                                          #
# Datagram Delivery Protocol services                                                      # Datagram Delivery Protocol services
#                                                                                          #
rtmp            1/ddp                   # Routing Table Maintenance Protocol               rtmp            1/ddp                   # Routing Table Maintenance Protocol
nbp             2/ddp                   # Name Binding Protocol                            nbp             2/ddp                   # Name Binding Protocol
echo            4/ddp                   # AppleTalk Echo Protocol                          echo            4/ddp                   # AppleTalk Echo Protocol
zip             6/ddp                   # Zone Information Protocol                        zip             6/ddp                   # Zone Information Protocol

#=========================================================================                 #=========================================================================
# The remaining port numbers are not as allocated by IANA.                                 # The remaining port numbers are not as allocated by IANA.
#=========================================================================                 #=========================================================================

# Kerberos (Project Athena/MIT) services                                                   # Kerberos (Project Athena/MIT) services
# Note that these are for Kerberos v4, and are unofficial.  Sites running                  # Note that these are for Kerberos v4, and are unofficial.  Sites running
# v4 should uncomment these and comment out the v5 entries above.                          # v4 should uncomment these and comment out the v5 entries above.
#                                                                                          #
kerberos4       750/udp         kerberos-iv kdc # Kerberos (server)                        kerberos4       750/udp         kerberos-iv kdc # Kerberos (server)
kerberos4       750/tcp         kerberos-iv kdc                                            kerberos4       750/tcp         kerberos-iv kdc
kerberos-master 751/udp         kerberos_master # Kerberos authentication                  kerberos-master 751/udp         kerberos_master # Kerberos authentication
kerberos-master 751/tcp                                                                    kerberos-master 751/tcp
passwd-server   752/udp         passwd_server   # Kerberos passwd server                   passwd-server   752/udp         passwd_server   # Kerberos passwd server
krb-prop        754/tcp         krb_prop krb5_prop hprop # Kerberos slave propagation      krb-prop        754/tcp         krb_prop krb5_prop hprop # Kerberos slave propagation
krbupdate       760/tcp         kreg            # Kerberos registration                    krbupdate       760/tcp         kreg            # Kerberos registration
swat            901/tcp                         # swat                                     swat            901/tcp                         # swat
kpop            1109/tcp                        # Pop with Kerberos                        kpop            1109/tcp                        # Pop with Kerberos
knetd           2053/tcp                        # Kerberos de-multiplexor                  knetd           2053/tcp                        # Kerberos de-multiplexor
zephyr-srv      2102/udp                        # Zephyr server                            zephyr-srv      2102/udp                        # Zephyr server
zephyr-clt      2103/udp                        # Zephyr serv-hm connection                zephyr-clt      2103/udp                        # Zephyr serv-hm connection
zephyr-hm       2104/udp                        # Zephyr hostmanager                       zephyr-hm       2104/udp                        # Zephyr hostmanager
eklogin         2105/tcp                        # Kerberos encrypted rlogin                eklogin         2105/tcp                        # Kerberos encrypted rlogin
# Hmmm. Are we using Kv4 or Kv5 now? Worrying.                                             # Hmmm. Are we using Kv4 or Kv5 now? Worrying.
# The following is probably Kerberos v5  --- ajt@debian.org (11/02/2000)                   # The following is probably Kerberos v5  --- ajt@debian.org (11/02/2000)
kx              2111/tcp                        # X over Kerberos                          kx              2111/tcp                        # X over Kerberos
iprop           2121/tcp                        # incremental propagation                  iprop           2121/tcp                        # incremental propagation
#                                                                                          #
# Unofficial but necessary (for NetBSD) services                                           # Unofficial but necessary (for NetBSD) services
#                                                                                          #
supfilesrv      871/tcp                         # SUP server                               supfilesrv      871/tcp                         # SUP server
supfiledbg      1127/tcp                        # SUP debugging                            supfiledbg      1127/tcp                        # SUP debugging

#                                                                                          #
# Services added for the Debian GNU/Linux distribution                                     # Services added for the Debian GNU/Linux distribution
#                                                                                          #
linuxconf       98/tcp                          # LinuxConf                                linuxconf       98/tcp                          # LinuxConf
poppassd        106/tcp                         # Eudora                                   poppassd        106/tcp                         # Eudora
poppassd        106/udp                                                                    poppassd        106/udp
moira-db        775/tcp         moira_db        # Moira database                           moira-db        775/tcp         moira_db        # Moira database
moira-update    777/tcp         moira_update    # Moira update protocol                    moira-update    777/tcp         moira_update    # Moira update protocol
moira-ureg      779/udp         moira_ureg      # Moira user registration                  moira-ureg      779/udp         moira_ureg      # Moira user registration
spamd           783/tcp                         # spamassassin daemon                      spamd           783/tcp                         # spamassassin daemon
omirr           808/tcp         omirrd          # online mirror                            omirr           808/tcp         omirrd          # online mirror
omirr           808/udp         omirrd                                                     omirr           808/udp         omirrd
customs         1001/tcp                        # pmake customs server                     customs         1001/tcp                        # pmake customs server
customs         1001/udp                                                                   customs         1001/udp
skkserv         1178/tcp                        # skk jisho server port                    skkserv         1178/tcp                        # skk jisho server port
predict         1210/udp                        # predict -- satellite tracking            predict         1210/udp                        # predict -- satellite tracking
rmtcfg          1236/tcp                        # Gracilis Packeten remote config serve    rmtcfg          1236/tcp                        # Gracilis Packeten remote config serve
wipld           1300/tcp                        # Wipl network monitor                     wipld           1300/tcp                        # Wipl network monitor
xtel            1313/tcp                        # french minitel                           xtel            1313/tcp                        # french minitel
xtelw           1314/tcp                        # french minitel                           xtelw           1314/tcp                        # french minitel
support         1529/tcp                        # GNATS                                    support         1529/tcp                        # GNATS
cfinger         2003/tcp                        # GNU Finger                               cfinger         2003/tcp                        # GNU Finger
frox            2121/tcp                        # frox: caching ftp proxy                  frox            2121/tcp                        # frox: caching ftp proxy
ninstall        2150/tcp                        # ninstall service                         ninstall        2150/tcp                        # ninstall service
ninstall        2150/udp                                                                   ninstall        2150/udp
zebrasrv        2600/tcp                        # zebra service                            zebrasrv        2600/tcp                        # zebra service
zebra           2601/tcp                        # zebra vty                                zebra           2601/tcp                        # zebra vty
ripd            2602/tcp                        # ripd vty (zebra)                         ripd            2602/tcp                        # ripd vty (zebra)
ripngd          2603/tcp                        # ripngd vty (zebra)                       ripngd          2603/tcp                        # ripngd vty (zebra)
ospfd           2604/tcp                        # ospfd vty (zebra)                        ospfd           2604/tcp                        # ospfd vty (zebra)
bgpd            2605/tcp                        # bgpd vty (zebra)                         bgpd            2605/tcp                        # bgpd vty (zebra)
ospf6d          2606/tcp                        # ospf6d vty (zebra)                       ospf6d          2606/tcp                        # ospf6d vty (zebra)
ospfapi         2607/tcp                        # OSPF-API                                 ospfapi         2607/tcp                        # OSPF-API
isisd           2608/tcp                        # ISISd vty (zebra)                        isisd           2608/tcp                        # ISISd vty (zebra)
afbackup        2988/tcp                        # Afbackup system                          afbackup        2988/tcp                        # Afbackup system
afbackup        2988/udp                                                                   afbackup        2988/udp
afmbackup       2989/tcp                        # Afmbackup system                         afmbackup       2989/tcp                        # Afmbackup system
afmbackup       2989/udp                                                                   afmbackup       2989/udp
xtell           4224/tcp                        # xtell server                             xtell           4224/tcp                        # xtell server
fax             4557/tcp                        # FAX transmission service (old)           fax             4557/tcp                        # FAX transmission service (old)
hylafax         4559/tcp                        # HylaFAX client-server protocol (new)     hylafax         4559/tcp                        # HylaFAX client-server protocol (new)
distmp3         4600/tcp                        # distmp3host daemon                       distmp3         4600/tcp                        # distmp3host daemon
munin           4949/tcp        lrrd            # Munin                                    munin           4949/tcp        lrrd            # Munin
enbd-cstatd     5051/tcp                        # ENBD client statd                        enbd-cstatd     5051/tcp                        # ENBD client statd
enbd-sstatd     5052/tcp                        # ENBD server statd                        enbd-sstatd     5052/tcp                        # ENBD server statd
pcrd            5151/tcp                        # PCR-1000 Daemon                          pcrd            5151/tcp                        # PCR-1000 Daemon
noclog          5354/tcp                        # noclogd with TCP (nocol)                 noclog          5354/tcp                        # noclogd with TCP (nocol)
noclog          5354/udp                        # noclogd with UDP (nocol)                 noclog          5354/udp                        # noclogd with UDP (nocol)
hostmon         5355/tcp                        # hostmon uses TCP (nocol)                 hostmon         5355/tcp                        # hostmon uses TCP (nocol)
hostmon         5355/udp                        # hostmon uses UDP (nocol)                 hostmon         5355/udp                        # hostmon uses UDP (nocol)
rplay           5555/udp                        # RPlay audio service                      rplay           5555/udp                        # RPlay audio service
nrpe            5666/tcp                        # Nagios Remote Plugin Executor            nrpe            5666/tcp                        # Nagios Remote Plugin Executor
nsca            5667/tcp                        # Nagios Agent - NSCA                      nsca            5667/tcp                        # Nagios Agent - NSCA
mrtd            5674/tcp                        # MRT Routing Daemon                       mrtd            5674/tcp                        # MRT Routing Daemon
bgpsim          5675/tcp                        # MRT Routing Simulator                    bgpsim          5675/tcp                        # MRT Routing Simulator
canna           5680/tcp                        # cannaserver                              canna           5680/tcp                        # cannaserver
syslog-tls      6514/tcp                        # Syslog over TLS [RFC5425]                syslog-tls      6514/tcp                        # Syslog over TLS [RFC5425]
sane-port       6566/tcp        sane saned      # SANE network scanner daemon              sane-port       6566/tcp        sane saned      # SANE network scanner daemon
ircd            6667/tcp                        # Internet Relay Chat                      ircd            6667/tcp                        # Internet Relay Chat
zope-ftp        8021/tcp                        # zope management by ftp                   zope-ftp        8021/tcp                        # zope management by ftp
tproxy          8081/tcp                        # Transparent Proxy                        tproxy          8081/tcp                        # Transparent Proxy
omniorb         8088/tcp                        # OmniORB                                  omniorb         8088/tcp                        # OmniORB
omniorb         8088/udp                                                                   omniorb         8088/udp
clc-build-daemon 8990/tcp                       # Common lisp build daemon                 clc-build-daemon 8990/tcp                       # Common lisp build daemon
xinetd          9098/tcp                                                                   xinetd          9098/tcp
mandelspawn     9359/udp        mandelbrot      # network mandelbrot                       mandelspawn     9359/udp        mandelbrot      # network mandelbrot
git             9418/tcp                        # Git Version Control System               git             9418/tcp                        # Git Version Control System
zope            9673/tcp                        # zope server                              zope            9673/tcp                        # zope server
webmin          10000/tcp                                                                  webmin          10000/tcp
kamanda         10081/tcp                       # amanda backup services (Kerberos)        kamanda         10081/tcp                       # amanda backup services (Kerberos)
kamanda         10081/udp                                                                  kamanda         10081/udp
amandaidx       10082/tcp                       # amanda backup services                   amandaidx       10082/tcp                       # amanda backup services
amidxtape       10083/tcp                       # amanda backup services                   amidxtape       10083/tcp                       # amanda backup services
smsqp           11201/tcp                       # Alamin SMS gateway                       smsqp           11201/tcp                       # Alamin SMS gateway
smsqp           11201/udp                                                                  smsqp           11201/udp
xpilot          15345/tcp                       # XPilot Contact Port                      xpilot          15345/tcp                       # XPilot Contact Port
xpilot          15345/udp                                                                  xpilot          15345/udp
sgi-cmsd        17001/udp               # Cluster membership services daemon               sgi-cmsd        17001/udp               # Cluster membership services daemon
sgi-crsd        17002/udp                                                                  sgi-crsd        17002/udp
sgi-gcd         17003/udp                       # SGI Group membership daemon              sgi-gcd         17003/udp                       # SGI Group membership daemon
sgi-cad         17004/tcp                       # Cluster Admin daemon                     sgi-cad         17004/tcp                       # Cluster Admin daemon
isdnlog         20011/tcp                       # isdn logging system                      isdnlog         20011/tcp                       # isdn logging system
isdnlog         20011/udp                                                                  isdnlog         20011/udp
vboxd           20012/tcp                       # voice box system                         vboxd           20012/tcp                       # voice box system
vboxd           20012/udp                                                                  vboxd           20012/udp
binkp           24554/tcp                       # binkp fidonet protocol                   binkp           24554/tcp                       # binkp fidonet protocol
asp             27374/tcp                       # Address Search Protocol                  asp             27374/tcp                       # Address Search Protocol
asp             27374/udp                                                                  asp             27374/udp
csync2          30865/tcp                       # cluster synchronization tool             csync2          30865/tcp                       # cluster synchronization tool
dircproxy       57000/tcp                       # Detachable IRC Proxy                     dircproxy       57000/tcp                       # Detachable IRC Proxy
tfido           60177/tcp                       # fidonet EMSI over telnet                 tfido           60177/tcp                       # fidonet EMSI over telnet
fido            60179/tcp                       # fidonet EMSI over TCP                    fido            60179/tcp                       # fidonet EMSI over TCP

# Local services                                                                           # Local services
comet  comet  19221  644                                                                   comet  comet  19221  644
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 17: Piping and redirecting a whole, whole lot of times... [1 pts]

```

# (script hidden for brevity)

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null) || test_end
compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
mount                                                                                      mount
mountpoint                                                                                 mountpoint
mt                                                                                         mt
mt-gnu                                                                                     mt-gnu
mv                                                                                         mv
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 18: Background job support [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/sleeper.sh 500 &
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/sleeper.sh 100 &
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/sleeper.sh 85 &
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/sleeper.sh 450 &
echo Done
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/kill-parent.sh
# -------------- End --------------

actual=$(timeout 1 ./$SHELL_NAME < <(echo "${script}") 2> /dev/null) || test_end 1
```

## Test 19: Background job list [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
sleep 1 &
sleep 2 &
sleep 500 &
sleep 100 &
sleep 450 &
sleep 3
jobs
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/kill-parent.sh
# -------------- End --------------

actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null)
# Interrupt

# We remove:
# - pids (unique on each system)
# - & (optional to display in the job list)
# - leading and trailing spaces
actual=$(awk '{$1=""; $0=$0; print $0}' <<< "${actual}" \
    | sed -e 's/&//g' -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
sleep 500                                                                               |  500
sleep 100                                                                               |  100
sleep 450                                                                               |  450
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 20: Environment Variable Expansion [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
echo SHELL is
echo $SHELL
echo PATH is
echo $PATH
echo TESTVAR is
echo $TESTVAR
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null)

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
SHELL is                                                                                   SHELL is
/bin/bash                                                                                  /bin/bash
PATH is                                                                                    PATH is
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/game    /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/game
TESTVAR is                                                                                 TESTVAR is
super awesome test variable                                                                super awesome test variable
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 21: Quote support [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh test1 test2 'this is a single quoted test'
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh test1 test2 "this is a single quoted test"
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh 'test1 test2'
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh "test1 test2"
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh 'test1 "test2" test3' "test4 'test5' test6"
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh 'test'
/home/comet/work/CommandLineShell/P2-offglitch-master/P2-Tests-master/scripts/num-args.sh
echo "Take a look at these quotes"
echo 'Are they "amazing" or what?!'
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null)

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

echo "${actual}"
Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
Number of args: 3                                                                          Number of args: 3
Number of args: 3                                                                          Number of args: 3
Number of args: 1                                                                          Number of args: 1
Number of args: 1                                                                          Number of args: 1
Number of args: 2                                                                          Number of args: 2
Number of args: 1                                                                          Number of args: 1
Number of args: 0                                                                          Number of args: 0
Take a look at these quotes                                                                Take a look at these quotes
Are they "amazing" or what?!                                                               Are they "amazing" or what?!
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 22: Environment Variable Expansion with Quotes [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
echo "SHELL is $SHELL"
echo "PATH is $PATH"
echo "TESTVAR is $TESTVAR"
echo "And all of them together looks like: $SHELL $PATH $TESTVAR -- wow!"
echo "! $END !"
# -------------- End --------------

expected=$(sh <(echo "${script}") 2> /dev/null)
actual=$(./$SHELL_NAME < <(echo "${script}") 2> /dev/null)

compare <(echo "${expected}") <(echo "${actual}")
echo "${expected}"

Expected Output                                                                         | Actual Output
----------------------------------------------------------------------------------------V----------------------------------------------------------------------------------------
echo "${actual}"
SHELL is /bin/bash                                                                         SHELL is /bin/bash
PATH is /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/lo    PATH is /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/lo
TESTVAR is super awesome test variable                                                     TESTVAR is super awesome test variable
And all of them together looks like: /bin/bash /usr/local/sbin:/usr/local/bin:/usr/sbin    And all of them together looks like: /bin/bash /usr/local/sbin:/usr/local/bin:/usr/sbin
! The End !                                                                                ! The End !
----------------------------------------------------------------------------------------^----------------------------------------------------------------------------------------

test_end
```

## Test 23: Basic Memory Leak Check [1 pts]

```

# ---------- Test Script ----------
echo "${script}"
ls /
asdfghjklqprewopiqwualasdf # Bad Command!
# Comment only
pwd
# -------------- End --------------

valgrind --trace-children=no --leak-check=full \
    ./$SHELL_NAME < <(echo "${script}") 2>&1 \
    | grep 'are definitely lost'
echo "${script}"
[[ $? -eq 1 ]]

test_end
```

## Test 24: Thorough Memory Leak Check [1 pts]

```

valgrind --trace-children=no --leak-check=full \
    ./$SHELL_NAME < "${TEST_DIR}/scripts/leak.sh" 2>&1 \
    | grep 'are definitely lost'
[[ $? -eq 1 ]]

test_end
```

