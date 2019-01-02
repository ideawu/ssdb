/*
Copyright (c) 2012-2018 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* system commands */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

DEF_PROC(qsize);
DEF_PROC(qfront);
DEF_PROC(qback);
DEF_PROC(qpush);
DEF_PROC(qpush_front);
DEF_PROC(qpush_back);
DEF_PROC(qpop);
DEF_PROC(qpop_front);
DEF_PROC(qpop_back);
DEF_PROC(qtrim_front);
DEF_PROC(qtrim_back);
DEF_PROC(qfix);
DEF_PROC(qclear);
DEF_PROC(qlist);
DEF_PROC(qrlist);
DEF_PROC(qslice);
DEF_PROC(qrange);
DEF_PROC(qget);
DEF_PROC(qset);
