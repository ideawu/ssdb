using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ssdb
{
    class Program
    {
        static void Main(string[] args) {
			Client client = new Client("127.0.0.1", 8888);
			List<byte[]> resp = client.request("set", "a", "100");
			foreach(byte[] bs in resp) {
				Console.WriteLine(Encoding.Default.GetString(bs));
			}

			resp = client.request("get", "a");
			foreach(byte[] bs in resp) {
				Console.WriteLine(Encoding.Default.GetString(bs));
			}

			client.set("a", "99");
			string val;
			client.get("a", out val);
			Console.WriteLine(val);

			Console.WriteLine("-----------------");
			{
				KeyValuePair<string, byte[]>[] kvs = client.scan("", "", 3);
				Console.WriteLine(kvs.Length + " kvs");
				foreach(KeyValuePair<string, byte[]> kv in kvs) {
					Console.WriteLine("    " + kv.Key + ": " + Encoding.Default.GetString(kv.Value));
				}
			}
			Console.WriteLine("-----------------");
			{
				KeyValuePair<string, Int64>[] kvs = client.zscan("z", "", Int64.MinValue, Int64.MaxValue, 3);
				Console.WriteLine(kvs.Length + " kvs");
				foreach(KeyValuePair<string, Int64> kv in kvs) {
					Console.WriteLine("    " + kv.Key + ": " + kv.Value);
				}
			}
			Console.WriteLine("-----------------");
			{
				KeyValuePair<string, Int64>[] kvs = client.zrscan("z", "", Int64.MaxValue, Int64.MinValue, 3);
				Console.WriteLine(kvs.Length + " kvs");
				foreach(KeyValuePair<string, Int64> kv in kvs) {
					Console.WriteLine("    " + kv.Key + ": " + kv.Value);
				}
			}

			Console.WriteLine(client.zsize("z").ToString());

			Console.ReadLine();

			client.close();
		}
    }
}
