#define MAXINST 436

#define noop 0
#define noop1 1
#define allocate_det_b 2
#define nondet 3
#define cut_return 4
#define return_det 5
#define unify_constant_return_det 6
#define cut_unify_value_return_det 7
#define unify_value_return_det 8
#define return_nondet 9
#define unify_constant_return_nondet 10
#define fork_unify_constant_return_nondet 11
#define fork_unify_nil_unify_value_return_nondet 12
#define unify_value_return_nondet 13
#define bp_fork 14
#define cut 15
#define cut0 16
#define unify_value_cut 17
#define fail 18
#define cut_fail 19
#define fail0 20
#define getbreg_y 21
#define putbreg_y 22
#define getpbreg_y 23
#define halt 24
#define halt0 25
#define call0 26
#define call_uv_d 27
#define call_uv_det 28
#define allocate_det 29
#define call_uv_nondet 30
#define call_uv_ot 31
#define call_uuv_d 32
#define call_uuv_det 33
#define call_uuv_nondet 34
#define call_uuv_ot 35
#define call_uvv_d 36
#define call_uvv_det 37
#define call_uvv_nondet 38
#define call_uvv_ot 39
#define call_uvc_d 40
#define call_uvc_det 41
#define call_uvc_nondet 42
#define call_uvc_ot 43
#define call_uvu_d 44
#define call_uvu_det 45
#define call_uvu_nondet 46
#define call_uvu_ot 47
#define call_uvuv_d 48
#define call_uvuv_det 49
#define call_uvuv_nondet 50
#define call_uvuv_ot 51
#define call_uuuv_d 52
#define call_uuuv_det 53
#define call_uuuv_nondet 54
#define call_uuuv_ot 55
#define call_uuuuv_d 56
#define call_uuuuv_det 57
#define call_uuuuv_nondet 58
#define call_uuuuv_ot 59
#define call0_d 60
#define call_v_d 61
#define call_v_det 62
#define call_v_nondet 63
#define call_v_ot 64
#define call_u_d 65
#define call_u_det 66
#define call_u_nondet 67
#define call_u_ot 68
#define call_2u_d 69
#define call_2u_det 70
#define call_2u_nondet 71
#define allocate_nondet 72
#define call_2u_ot 73
#define call_3u_d 74
#define call_3u_det 75
#define call_3u_nondet 76
#define call_3u_ot 77
#define call_4u_d 78
#define call_4u_det 79
#define call_4u_nondet 80
#define call_4u_ot 81
#define call_5u_d 82
#define call_5u_det 83
#define call_5u_nondet 84
#define call_5u_ot 85
#define call_6u_d 86
#define call_6u_det 87
#define call_6u_nondet 88
#define call_6u_ot 89
#define call_7u_d 90
#define call_7u_det 91
#define call_7u_nondet 92
#define call_7u_ot 93
#define call_8u_d 94
#define call_8u_det 95
#define call_8u_nondet 96
#define call_8u_ot 97
#define call_9u_d 98
#define call_9u_det 99
#define call_9u_nondet 100
#define call_9u_ot 101
#define call_cu_d 102
#define call_cu_det 103
#define call_cu_nondet 104
#define call_cu_ot 105
#define call_cuu_d 106
#define call_cuu_det 107
#define call_cuu_nondet 108
#define call_cuu_ot 109
#define call_cuuu_d 110
#define call_cuuu_det 111
#define call_cuuu_nondet 112
#define call_cuuu_ot 113
#define call_cuuuu_d 114
#define call_cuuuu_det 115
#define call_cuuuu_nondet 116
#define call_cuuuu_ot 117
#define call_var 118
#define call_var0 119
#define jmp 120
#define jmpn_eq_constant 121
#define jmpn_nil 122
#define jmpn_eq_struct 123
#define jmpn_eq_struct_fetch_v 124
#define switch_cons 125
#define switch_cons_car 126
#define switch_cons_910 127
#define switch_cons_vv 128
#define jmpn_eql_uu 129
#define jmpn_eql_uc 130
#define jmp_eql_uu 131
#define jmp_eql_uu_fail 132
#define jmp_eql_uu_ot 133
#define jmp_eql_uc 134
#define jmpn_gt_uu 135
#define jmpn_gt_ui 136
#define jmpn_gt_iu 137
#define jmpn_ge_uu 138
#define jmpn_ge_ui 139
#define jmpn_ge_iu 140
#define jmpn_id_uu 141
#define jmpn_id_uc 142
#define jmp_id_uu 143
#define jmp_id_uc 144
#define jmpn_var_y 145
#define jmpn_var_y_fail0 146
#define jmpn_var_y_ot 147
#define jmp_var_y 148
#define jmpn_atom_y 149
#define jmpn_atomic_y 150
#define jmpn_atomic_y_fail0 151
#define jmpn_atomic_y_ot 152
#define jmpn_num_y 153
#define jmpn_float_y 154
#define jmpn_int_y 155
#define hash 156
#define hash_jmpn_constant 157
#define hash_branch_constant 158
#define hash_jmpn_nil 159
#define hash_jmpn_struct 160
#define hash_branch_struct 161
#define hash_jmpn_list0 162
#define hash_jmpn_list 163
#define para_uuuv 164
#define para_uuv 165
#define para_uv 166
#define para_uuu 167
#define para_uuuw 168
#define para_uuw 169
#define para_uw 170
#define para_vv 171
#define para_cuv 172
#define para_cuuv 173
#define para_uuuc 174
#define para_uuc 175
#define para_uu 176
#define para_uc 177
#define para_u 178
#define para_v 179
#define para_w 180
#define para_c 181
#define para_nil 182
#define fetch_4v 183
#define fetch_3v 184
#define fetch_2v 185
#define fetch_v 186
#define fetch_vw 187
#define fetch_wv 188
#define fetch_w 189
#define fetch_2w 190
#define fetch_3w 191
#define fetch_4w 192
#define fetch_ws 193
#define fetch_910 194
#define fetch_45 195
#define unify_constant 196
#define fork_unify_constant 197
#define unify_nil 198
#define fork_unify_nil 199
#define unify_struct 200
#define fork_unify_struct 201
#define unify_struct_cut 202
#define unify_struct_arg_uv0 203
#define unify_struct_arg_2v0 204
#define unify_struct_arg_v0 205
#define unify_struct_arg_2u 206
#define unify_struct_arg_u 207
#define unify_struct_arg_c 208
#define unify_struct_arg_wc 209
#define unify_arg_c 210
#define unify_arg_u 211
#define unify_arg_read_u 212
#define unify_arg_v 213
#define unify_arg_v0 214
#define unify_arg_w 215
#define unify_arg_struct 216
#define unify_arg_list 217
#define unify_arg_2c 218
#define unify_arg_3c 219
#define unify_arg_v0u 220
#define unify_arg_v0c 221
#define unify_arg_uv0 222
#define unify_arg_v0w 223
#define unify_arg_wc 224
#define unify_arg_2v0 225
#define unify_arg_3v0 226
#define unify_arg_4v0 227
#define unify_arg_3u 228
#define unify_arg_2u 229
#define unify_arg_2w 230
#define unify_arg_3w 231
#define unify_arg_4w 232
#define unify_arg_5w 233
#define build_arg_c 234
#define build_arg_v 235
#define build_arg_struct 236
#define build_arg_list 237
#define build_arg_2c 238
#define build_arg_3c 239
#define build_arg_v0u 240
#define build_arg_v0c 241
#define build_arg_uv0 242
#define build_arg_v0w 243
#define build_arg_wc 244
#define build_arg_4v0 245
#define build_arg_3v0 246
#define build_arg_2v0 247
#define build_arg_v0 248
#define build_arg_5w 249
#define build_arg_4w 250
#define build_arg_3w 251
#define build_arg_2w 252
#define build_arg_w 253
#define build_arg_4u 254
#define build_arg_3u 255
#define build_arg_2u 256
#define build_arg_u 257
#define unify_list 258
#define unify_comp_list 259
#define unify_cons 260
#define cut_unify_cons_w 261
#define unify_cons_w 262
#define unify_cons_u 263
#define unify_cons_v0 264
#define cut_unify_cons_uu 265
#define unify_cons_uu 266
#define cut_unify_cons_uv 267
#define unify_cons_uv0 268
#define unify_cons_uw 269
#define unify_cons_uc 270
#define cut_unify_cons_vv 271
#define unify_cons_v0v0 272
#define cut_unify_cons_vu 273
#define unify_cons_v0u 274
#define unify_cons_v0w 275
#define cut_unify_cons_vc 276
#define unify_cons_v0c 277
#define unify_cons_wv0 278
#define unify_cons_cu 279
#define cut_unify_cons_cv 280
#define unify_cons_cv0 281
#define unify_cons_cc 282
#define unify_cons_ww 283
#define unify_cons0_v910 284
#define conc 285
#define leng 286
#define memb_le 287
#define memb_el 288
#define fork_unify_cons 289
#define fork_unify_cons_uu 290
#define fork_unify_cons_uv 291
#define fork_unify_cons_uc 292
#define fork_unify_cons_uw 293
#define fork_unify_cons_vu 294
#define fork_unify_cons_vv 295
#define fork_unify_cons_vw 296
#define fork_unify_cons_vc 297
#define fork_unify_cons_cc 298
#define fork_unify_cons_v910 299
#define unify_value 300
#define fork_unify_value 301
#define move_constant 302
#define move_struct 303
#define move_struct0 304
#define move_list 305
#define move_comp_list 306
#define move_comp_list1 307
#define move_cons 308
#define move_cons0_uv 309
#define move_cons0_uu 310
#define move_var 311
#define move_value 312
#define and 313
#define or 314
#define lshiftl 315
#define lshiftr 316
#define complement 317
#define add_uuv 318
#define add_uiv 319
#define add_u1v 320
#define add1 321
#define sub_uuv 322
#define sub_uiv 323
#define sub_u1v 324
#define sub1 325
#define sub_iuv 326
#define mul 327
#define mul_iuv 328
#define mul_uuv 329
#define div 330
#define idiv 331
#define idiv_uiv 332
#define idiv_uuv 333
#define divge 334
#define divle 335
#define mod 336
#define arg 337
#define arg0 338
#define setarg 339
#define setarg0 340
#define functor 341
#define functor_uvv 342
#define functor_vuu 343
#define functor_arity 344
#define get_clause_copy 345
#define garbage_collect 346
#define catch_clean_up 347
#define throw_ball 348
#define builtin0 349
#define builtin1 350
#define builtin2 351
#define builtin3 352
#define builtin4 353
#define move_vars 354
#define last_call 355
#define last_call_var0 356
#define last_call0 357
#define last_call_d 358
#define last_call1_d 359
#define last_call_au_d 360
#define last_call_au_det 361
#define last_call_au_nondet 362
#define last_call_au_ot 363
#define last_call1_au_d 364
#define last_call0_d 365
#define last_call0_sa_d 366
#define last_call0_sa_det 367
#define last_call0_sa_nondet 368
#define last_call0_sa_ot 369
#define tr_det_call_au 370
#define tr_det_call1_au 371
#define tr_det_call2_au 372
#define tr_det_call0 373
#define tr_nondet_call_au 374
#define tr_nondet_call1_au 375
#define tr_nondet_call2_au 376
#define tr_nondet_call0 377
#define allocate_susp 378
#define return_commit 379
#define return_delay 380
#define no_vars_gt 381
#define trigger_var_ins 382
#define trigger_var_minmax 383
#define trigger_var_dom 384
#define trigger_ins_min_max 385
#define trigger_var_any_dom 386
#define trigger_cg_event_handler 387
#define fetch_event_object 388
#define delay 389
#define end_delay 390
#define jmpn_dvar_y 391
#define jmpn_susp_var_y 392
#define interval_consistent_eq 393
#define interval_consistent_eq_nocoe 394
#define interval_consistent_ge 395
#define call_binary_constr_eq 396
#define domain_next_inst_yyy 397
#define domain_set_false_yy 398
#define domain_min_max_yyy 399
#define domain_region 400
#define domain_region_min 401
#define domain_region_max 402
#define v_in_cv_int 403
#define v_in_vc_int 404
#define u_in_cu_int 405
#define u_in_uc_int 406
#define u_eq_cu_dom 407
#define u_eq_uc_dom 408
#define bcp 409
#define activate_first_agent 410
#define activate_agents_conjunction 411
#define activate_agents_disjunction 412
#define table_allocate 413
#define table_produce 414
#define table_consume 415
#define table_check_completion 416
#define table_neck_no_reeval 417
#define last_tabled_call 418
#define table_cut 419
#define table_neck 420
#define table_eager_consume 421
#define table_set_new_bit 422
#define jmpn_unif 423
#define jmp_unif 424
#define arg_no_chk 425
#define setarg0_no_chk 426
#define bcp1 427
#define domain_nogood_region 428
#define endfile 429
#define tabsize 430
#define table_mode 431
#define asp_decode 432
#define asp_add_tuple 433
#define set_catcher_frame 434
#define filter_clauses 435