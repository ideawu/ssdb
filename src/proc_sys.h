/*
Copyright (c) 2012-2018 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
/* system commands */
#include "serv.h"
#include "net/proc.h"
#include "net/server.h"

DEF_PROC(flushdb);
DEF_PROC(info);
DEF_PROC(version);
DEF_PROC(dbsize);
DEF_PROC(compact);
DEF_PROC(dump);
DEF_PROC(sync140);
DEF_PROC(slaveof);
DEF_PROC(clear_binlog);

DEF_PROC(get_key_range);
DEF_PROC(ignore_key_range);
DEF_PROC(get_kv_range);
DEF_PROC(set_kv_range);
