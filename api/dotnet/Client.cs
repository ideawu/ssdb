using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ssdb {
	public class Client {
		private Link link;
		private string resp_code;

		public Client(string host, int port) {
			link = new Link(host, port);
		}

		~Client() {
			this.close();
		}

		public void close() {
			link.close();
		}

		public List<byte[]> request(string cmd, params string[] args) {
			return link.request(cmd, args);
		}

		public List<byte[]> request(string cmd, params byte[][] args) {
			return link.request(cmd, args);
		}

		public List<byte[]> request(List<byte[]> req) {
			return link.request(req);
		}


		private void assert_ok() {
			if (resp_code != "ok")
			{
				throw new Exception(resp_code);
			}
		}

		private byte[] _bytes(string s)	{
			return Encoding.Default.GetBytes(s);
		}

		private string _string(byte[] bs) {
			return Encoding.Default.GetString(bs);
		}

		private KeyValuePair<string, byte[]>[] parse_scan_resp(List<byte[]> resp) {
			resp_code = _string(resp[0]);
			this.assert_ok();

			int size = (resp.Count - 1) / 2;
			KeyValuePair<string, byte[]>[] kvs = new KeyValuePair<string, byte[]>[size];
			for (int i = 0; i < size; i += 1)
			{
				string key = _string(resp[i * 2 + 1]);
				byte[] val = resp[i * 2 + 2];
				kvs[i] = new KeyValuePair<string, byte[]>(key, val);
			}
			return kvs;
		}
		 
		/***** kv *****/

		public bool exists(byte[] key) {
			List<byte[]> resp = request("exists", key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return (_string(resp[1]) == "1" ? true : false);
		}

		public bool exists(string key) {
			return this.exists(_bytes(key));
		}

		public void set(byte[] key, byte[] val) {
			List<byte[]> resp = request("set", key, val);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void set(string key, string val) {
			this.set(_bytes(key), _bytes(val));
		}

		/// <summary>
		///
		/// </summary>
		/// <param name="key"></param>
		/// <param name="val"></param>
		/// <returns>returns true if name.key is found, otherwise returns false.</returns>
		public bool get(byte[] key, out byte[] val) {
			val = null;
			List<byte[]> resp = request("get", key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			val = resp[1];
			return true;
		}

		public bool get(string key, out byte[] val) {
			return this.get(_bytes(key), out val);
		}

		public bool get(string key, out string val) {
			val = null;
			byte[] bs;
			if (!this.get(key, out bs))
			{
				return false;
			}
			val = _string(bs);
			return true;
		}

		public void del(byte[] key) {
			List<byte[]> resp = request("del", key);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void del(string key) {
			this.del(_bytes(key));
		}

		public KeyValuePair<string, byte[]>[] scan(string key_start, string key_end, Int64 limit) {
			List<byte[]> resp = request("scan", key_start, key_end, limit.ToString());
			return parse_scan_resp(resp);
		}

		public KeyValuePair<string, byte[]>[] rscan(string key_start, string key_end, Int64 limit) {
			List<byte[]> resp = request("rscan", key_start, key_end, limit.ToString());
			return parse_scan_resp(resp);
		}

		/***** hash *****/

		public void hset(byte[] name, byte[] key, byte[] val) {
			List<byte[]> resp = request("hset", name, key, val);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void hset(string name, string key, byte[] val) {
			this.hset(_bytes(name), _bytes(key), val);
		}

		public void hset(string name, string key, string val) {
			this.hset(_bytes(name), _bytes(key), _bytes(val));
		}

		/// <summary>
		///
		/// </summary>
		/// <param name="name"></param>
		/// <param name="key"></param>
		/// <param name="val"></param>
		/// <returns>returns true if name.key is found, otherwise returns false.</returns>
		public bool hget(byte[] name, byte[] key, out byte[] val) {
			val = null;
			List<byte[]> resp = request("hget", name, key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			val = resp[1];
			return true;
		}

		public bool hget(string name, string key, out byte[] val) {
			return this.hget(_bytes(name), _bytes(key), out val);
		}

		public bool hget(string name, string key, out string val) {
			val = null;
			byte[] bs;
			if (!this.hget(name, key, out bs))
			{
				return false;
			}
			val = _string(bs);
			return true;
		}

		public void hdel(byte[] name, byte[] key) {
			List<byte[]> resp = request("hdel", name, key);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void hdel(string name, string key) {
			this.hdel(_bytes(name), _bytes(key));
		}

		public bool hexists(byte[] name, byte[] key) {
			List<byte[]> resp = request("hexists", name, key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return (_string(resp[1]) == "1" ? true : false);
		}
		
		public bool hexists(string name, string key) {
			return this.hexists(_bytes(name), _bytes(key));
		}

		public Int64 hsize(byte[] name) {
			List<byte[]> resp = request("hsize", name);
			resp_code = _string(resp[0]);
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return Int64.Parse(_string(resp[1]));
		}

		public Int64 hsize(string name) {
			return this.hsize(_bytes(name));
		}

		public KeyValuePair<string, byte[]>[] hscan(string name, string key_start, string key_end, Int64 limit) {
			List<byte[]> resp = request("hscan", name, key_start, key_end, limit.ToString());
			return parse_scan_resp(resp);
		}

		public KeyValuePair<string, byte[]>[] hrscan(string name, string key_start, string key_end, Int64 limit) {
			List<byte[]> resp = request("hrscan", name, key_start, key_end, limit.ToString());
			return parse_scan_resp(resp);
		}

		public void multi_hset(byte[] name, KeyValuePair<byte[], byte[]>[] kvs)
		{
			byte[][] req = new byte[(kvs.Length * 2) + 1][];
			req[0] = name;
			for (int i = 0; i < kvs.Length; i++)
			{
				req[(2 * i) + 1] = kvs[i].Key;
				req[(2 * i) + 2] = kvs[i].Value;

			}
			List<byte[]> resp = request("multi_hset", req);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void multi_hset(string name, KeyValuePair<string, string>[] kvs)
		{
			KeyValuePair<byte[], byte[]>[] req = new KeyValuePair<byte[], byte[]>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				req[i] = new KeyValuePair<byte[], byte[]>(_bytes(kvs[i].Key), _bytes(kvs[i].Value));
			}
			this.multi_hset(_bytes(name), req);
		}

		public void multi_hdel(byte[] name, byte[][] keys)
		{
			byte[][] req = new byte[keys.Length + 1][];
			req[0] = name;
			for (int i = 0; i < keys.Length; i++)
			{
				req[i + 1] = keys[i];
			}
			List<byte[]> resp = request("multi_hdel", req);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void multi_hdel(string name, string[] keys)
		{
			byte[][] req = new byte[keys.Length][];
			for (int i = 0; i < keys.Length; i++)
			{
				req[i] = _bytes(keys[i]);
			}
			this.multi_hdel(_bytes(name), req);
		}

		public KeyValuePair<string, byte[]>[] multi_hget(byte[] name, byte[][] keys)
		{
			byte[][] req = new byte[keys.Length + 1][];
			req[0] = name;
			for (int i = 0; i < keys.Length; i++)
			{
				req[i + 1] = keys[i];
			}
			List<byte[]> resp = request("multi_hget", req);
			KeyValuePair<string, byte[]>[] ret = parse_scan_resp(resp);

			return ret;
		}

		public KeyValuePair<string, byte[]>[] multi_hget(string name, string[] keys)
		{
			byte[][] req = new byte[keys.Length][];
			for (int i = 0; i < keys.Length; i++)
			{
				req[i] = _bytes(keys[i]);
			}
			return this.multi_hget(_bytes(name), req);
		}

		/***** zset *****/

		public void zset(byte[] name, byte[] key, Int64 score) {
			List<byte[]> resp = request("zset", name, key, _bytes(score.ToString()));
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void zset(string name, string key, Int64 score) {
			this.zset(_bytes(name), _bytes(key), score);
		}

		public Int64 zincr(byte[] name, byte[] key, Int64 increment) {
			List<byte[]> resp = request("zincr", name, key, _bytes(increment.ToString()));
			resp_code = _string(resp[0]);
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return Int64.Parse(_string(resp[1]));
		}
		
		public Int64 zincr(string name, string key, Int64 increment) {
			return this.zincr(_bytes(name), _bytes(key), increment);
		}

		/// <summary>
		///
		/// </summary>
		/// <param name="name"></param>
		/// <param name="key"></param>
		/// <param name="score"></param>
		/// <returns>returns true if name.key is found, otherwise returns false.</returns>
		public bool zget(byte[] name, byte[] key, out Int64 score) {
			score = -1;
			List<byte[]> resp = request("zget", name, key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			score = Int64.Parse(_string(resp[1]));
			return true;
		}

		public bool zget(string name, string key, out Int64 score) {
			return this.zget(_bytes(name), _bytes(key), out score);
		}

		public void zdel(byte[] name, byte[] key) {
			List<byte[]> resp = request("zdel", name, key);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void zdel(string name, string key) {
			this.zdel(_bytes(name), _bytes(key));
		}

		public Int64 zsize(byte[] name) {
			List<byte[]> resp = request("zsize", name);
			resp_code = _string(resp[0]);
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return Int64.Parse(_string(resp[1]));
		}

		public Int64 zsize(string name) {
			return this.zsize(_bytes(name));
		}

		public bool zexists(byte[] name, byte[] key) {
			List<byte[]> resp = request("zexists", name, key);
			resp_code = _string(resp[0]);
			if (resp_code == "not_found")
			{
				return false;
			}
			this.assert_ok();
			if (resp.Count != 2)
			{
				throw new Exception("Bad response!");
			}
			return (_string(resp[1]) == "1" ? true : false);
		}
		
		public bool zexists(string name, string key) {
			return this.zexists(_bytes(name), _bytes(key));
		}

		public KeyValuePair<string, Int64>[] zrange(string name, Int32 offset, Int32 limit) {
			List<byte[]> resp = request("zrange", name, offset.ToString(), limit.ToString());
			KeyValuePair<string, byte[]>[] kvs = parse_scan_resp(resp);
			KeyValuePair<string, Int64>[] ret = new KeyValuePair<string, Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				string key = kvs[i].Key;
				Int64 score = Int64.Parse(_string(kvs[i].Value));
				ret[i] = new KeyValuePair<string, Int64>(key, score);
			}
			return ret;
		}
		
		public KeyValuePair<string, Int64>[] zrrange(string name, Int32 offset, Int32 limit) {
			List<byte[]> resp = request("zrrange", name, offset.ToString(), limit.ToString());
			KeyValuePair<string, byte[]>[] kvs = parse_scan_resp(resp);
			KeyValuePair<string, Int64>[] ret = new KeyValuePair<string, Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				string key = kvs[i].Key;
				Int64 score = Int64.Parse(_string(kvs[i].Value));
				ret[i] = new KeyValuePair<string, Int64>(key, score);
			}
			return ret;
		}

		public KeyValuePair<string, Int64>[] zscan(string name, string key_start, Int64 score_start, Int64 score_end, Int64 limit) {
			string score_s = "";
			string score_e = "";
			if (score_start != Int64.MinValue)
			{
				score_s = score_start.ToString();
			}
			if (score_end != Int64.MaxValue)
			{
				score_e = score_end.ToString();
			}
			List<byte[]> resp = request("zscan", name, key_start, score_s, score_e, limit.ToString());
			KeyValuePair<string, byte[]>[] kvs = parse_scan_resp(resp);
			KeyValuePair<string, Int64>[] ret = new KeyValuePair<string, Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				string key = kvs[i].Key;
				Int64 score = Int64.Parse(_string(kvs[i].Value));
				ret[i] = new KeyValuePair<string, Int64>(key, score);
			}
			return ret;
		}

		public KeyValuePair<string, Int64>[] zrscan(string name, string key_start, Int64 score_start, Int64 score_end, Int64 limit) {
			string score_s = "";
			string score_e = "";
			if (score_start != Int64.MaxValue)
			{
				score_s = score_start.ToString();
			}
			if (score_end != Int64.MinValue)
			{
				score_e = score_end.ToString();
			}
			List<byte[]> resp = request("zrscan", name, key_start, score_s, score_e, limit.ToString());
			KeyValuePair<string, byte[]>[] kvs = parse_scan_resp(resp);
			KeyValuePair<string, Int64>[] ret = new KeyValuePair<string, Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				string key = kvs[i].Key;
				Int64 score = Int64.Parse(_string(kvs[i].Value));
				ret[i] = new KeyValuePair<string, Int64>(key, score);
			}
			return ret;
		}

		public void multi_zset(byte[] name, KeyValuePair<byte[], Int64>[] kvs)
		{
			byte[][] req = new byte[(kvs.Length * 2) + 1][];
			req[0] = name;
			for (int i = 0; i < kvs.Length; i++)
			{
				req[(2 * i) + 1] = kvs[i].Key;
				req[(2 * i) + 2] = _bytes(kvs[i].Value.ToString());

			}
			List<byte[]> resp = request("multi_zset", req);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void multi_zset(string name, KeyValuePair<string, Int64>[] kvs)
		{
			KeyValuePair<byte[], Int64>[] req = new KeyValuePair<byte[], Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				req[i] = new KeyValuePair<byte[], Int64>(_bytes(kvs[i].Key), kvs[i].Value);
			}
			this.multi_zset(_bytes(name), req);
		}

		public void multi_zdel(byte[] name, byte[][] keys)
		{
			byte[][] req = new byte[keys.Length + 1][];
			req[0] = name;
			for (int i = 0; i < keys.Length; i++)
			{
				req[i + 1] = keys[i];
			}
			List<byte[]> resp = request("multi_zdel", req);
			resp_code = _string(resp[0]);
			this.assert_ok();
		}

		public void multi_zdel(string name, string[] keys)
		{
			byte[][] req = new byte[keys.Length][];
			for (int i = 0; i < keys.Length; i++)
			{
				req[i] = _bytes(keys[i]);
			}
			this.multi_zdel(_bytes(name), req);
		}

		public KeyValuePair<string, Int64>[] multi_zget(byte[] name, byte[][] keys)
		{
			byte[][] req = new byte[keys.Length + 1][];
			req[0] = name;
			for (int i = 0; i < keys.Length; i++)
			{
				req[i + 1] = keys[i];
			}
			List<byte[]> resp = request("multi_zget", req);
			KeyValuePair<string, byte[]>[] kvs = parse_scan_resp(resp);
			KeyValuePair<string, Int64>[] ret = new KeyValuePair<string, Int64>[kvs.Length];
			for (int i = 0; i < kvs.Length; i++)
			{
				string key = kvs[i].Key;
				Int64 score = Int64.Parse(_string(kvs[i].Value));
				ret[i] = new KeyValuePair<string, Int64>(key, score);
			}
			return ret;
		}

		public KeyValuePair<string, Int64>[] multi_zget(string name, string[] keys)
		{
			byte[][] req = new byte[keys.Length][];
			for (int i = 0; i < keys.Length; i++)
			{
				req[i] = _bytes(keys[i]);
			}
			return this.multi_zget(_bytes(name), req);
		}
		
	}
}



