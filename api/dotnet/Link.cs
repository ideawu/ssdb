using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace ssdb
{
	class Link
	{
		private TcpClient sock;
		private MemoryStream recv_buf = new MemoryStream(8 * 1024);

		public Link(string host, int port) {
			sock = new TcpClient(host, port);
			sock.NoDelay = true;
			sock.Client.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.KeepAlive, true);
		}

		~Link() {
			this.close();
		}

		public void close() {
			if(sock != null){
				sock.Close();
			}
			sock = null;
		}

		public List<byte[]> request(string cmd, params string[] args) {
			List<byte[]> req = new List<byte[]>(1 + args.Length);
			req.Add(Encoding.Default.GetBytes(cmd));
			foreach(string s in args) {
				req.Add(Encoding.Default.GetBytes(s));
			}
			return this.request(req);
		}

		public List<byte[]> request(string cmd, params byte[][] args) {
			List<byte[]> req = new List<byte[]>(1 + args.Length);
			req.Add(Encoding.Default.GetBytes(cmd));
			req.AddRange(args);
			return this.request(req);
		}

		public List<byte[]> request(List<byte[]> req) {
			MemoryStream buf = new MemoryStream();
			foreach(byte[] p in req) {
				byte[] len = Encoding.Default.GetBytes(p.Length.ToString());
				buf.Write(len, 0, len.Length);
				buf.WriteByte((byte)'\n');
				buf.Write(p, 0, p.Length);
				buf.WriteByte((byte)'\n');
			}
			buf.WriteByte((byte)'\n');

			byte[] bs = buf.GetBuffer();
			sock.GetStream().Write(bs, 0, (int)buf.Length);
			//Console.Write(Encoding.Default.GetString(bs, 0, (int)buf.Length));
			return recv();
		}

		private List<byte[]> recv() {
			while(true) {
				List<byte[]> ret = parse();
				if(ret != null) {
					return ret;
				}
				byte[] bs = new byte[8192];
				int len = sock.GetStream().Read(bs, 0, bs.Length);
				//Console.WriteLine("<< " + Encoding.Default.GetString(bs));
				recv_buf.Write(bs, 0, len);
			}
		}

		private static int memchr(byte[] bs, byte b, int offset) {
			for(int i = offset; i < bs.Length; i++) {
				if(bs[i] == b) {
					return i;
				}
			}
			return -1;
		}

		private List<byte[]> parse() {
			List<byte[]> list = new List<byte[]>();
			byte[] buf = recv_buf.GetBuffer();

			int idx = 0;
			while(true) {
				int pos = memchr(buf, (byte)'\n', idx);
				//System.out.println("pos: " + pos + " idx: " + idx);
				if(pos == -1) {
					break;
				}
				if(pos == idx || (pos == idx + 1 && buf[idx] == '\r')) {
					idx += 1; // if '\r', next time will skip '\n'
					// ignore empty leading lines
					if(list.Count == 0) {
						continue;
					} else {
						int left = (int)recv_buf.Length - idx;
						recv_buf = new MemoryStream(8192);
						if(left > 0) {
							recv_buf.Write(buf, idx, left);
						}
						return list;
					}
				}
				byte[] lens = new byte[pos - idx];
				Array.Copy(buf, idx, lens, 0, lens.Length);
				int len = Int32.Parse(Encoding.Default.GetString(lens));

				idx = pos + 1;
				if(idx + len >= recv_buf.Length) {
					break;
				}
				byte[] data = new byte[len];
				Array.Copy(buf, idx, data, 0, (int)data.Length);

				//Console.WriteLine("len: " + len + " data: " + Encoding.Default.GetString(data));
				idx += len + 1; // skip '\n'
				list.Add(data);
			}
			return null;
		}
	}
}
