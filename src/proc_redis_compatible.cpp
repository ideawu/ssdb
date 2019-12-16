#include "proc_redis_compatible.h"
int proc_del(NetworkServer *net, Link *link, const Request &req,
             Response *resp) {
  SSDBServer *serv = (SSDBServer *)net->data;
  // 上锁
  // Locking *locker = new Locking(&serv->expiration->mutex);
  // 检查参数数量
  CHECK_NUM_PARAMS(2);
  const Bytes &name = req[1];
  int64_t count = 0;
  // 删掉key-value
  /*
  CHECK_KV_KEY_RANGE(1);
  //这一段没太看懂在做什么
  if (!link->ignore_key_range && req.size() > 1) {
    if (!serv->in_kv_range(req[1])) {
      resp->push_back("out_of_range");
      return 0;
    }
  }
  */
  if (serv->ssdb->del(req[1]) == -1) {
    resp->push_back("error");
    resp->push_back("key");
    return 0;
  } else {
    serv->expiration->del_ttl(req[1]);
  }
  // 删掉hash(hclear)
  count += serv->ssdb->hclear(name);
  // 删掉sorted set(zclear)
  std::string key_start, score_start;
  int num = 1000;
  while (num == 1000) {
    num = 0;
    ZIterator *it = serv->ssdb->zscan(name, key_start, score_start, "", 1000);
    while (it->next()) {
      key_start = it->key;
      score_start = it->score;
      if (serv->ssdb->zdel(name, key_start)) {
        resp->push_back("error");
        resp->push_back("zset");
        delete it;
        return 0;
      }
      num++;
    }
    count += num;
  }
  // 删掉 queue(qclear)
  while (1) {
    std::string item;
    int ret = serv->ssdb->qpop_front(req[1], &item);
    if (ret == 0) {
      break;
    }
    if (ret == -1) {
      return -1;
    }
    count += 1;
  }
  resp->reply_int(0, count);
  return 0;
}
int proc_multi_del(NetworkServer *net, Link *link, const Request &req,
                   Response *resp) {
  SSDBServer *serv = (SSDBServer *)net->data;
  CHECK_NUM_PARAMS(2);
  // 删除Key-value
  Locking *locker = new Locking(&serv->expiration->mutex);
  if (serv->ssdb->multi_del(req, 1) != -1) {
    for (Request::const_iterator it = req.begin() + 1; it != req.end(); it++) {
      serv->expiration->del_ttl(*it);
    }
  } else {
    resp->push_back("error");
    return 0;
  }
  delete locker;
  // 删除hash(hclear)
  for (Request::const_iterator it = req.begin() + 1; it != req.end(); it++) {
    serv->ssdb->hclear(*it);
  }
  // 删除zset
  for (Request::const_iterator it = req.begin() + 1; it != req.end();) {
    ZIterator *zit = serv->ssdb->zscan(*it, "", "", "", 1000);
    int num = 0;
    while (zit->next()) {
      if (serv->ssdb->zdel(*it, zit->key) == -1) {
        resp->push_back("error");
        delete zit;
        return 0;
      }
      num++;
    }
    delete zit;
    if (num == 0) {
      it++;
    }
  }
  // 删除queue
  for (Request::const_iterator it = req.begin() + 1; it != req.end(); it++) {
    int ret;
    while ((ret = serv->ssdb->qpop_front(*it, NULL))) {
      if (ret == -1) {
        resp->push_back("error");
        return 0;
      }
    };
  }
  resp->reply_int(0, req.size() - 1);
  return 0;
}