/*
Copyright (c) 2012-2018 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* hash commands */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

DEF_PROC(hsize);
DEF_PROC(hget);
DEF_PROC(hset);
DEF_PROC(hdel);
DEF_PROC(hincr);
DEF_PROC(hdecr);
DEF_PROC(hclear);
DEF_PROC(hgetall);
DEF_PROC(hscan);
DEF_PROC(hrscan);
DEF_PROC(hkeys);
DEF_PROC(hvals);
DEF_PROC(hlist);
DEF_PROC(hrlist);
DEF_PROC(hexists);
DEF_PROC(multi_hexists);
DEF_PROC(multi_hsize);
DEF_PROC(multi_hget);
DEF_PROC(multi_hset);
DEF_PROC(multi_hdel);
DEF_PROC(hfix);
