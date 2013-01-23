import com.udpwork.ssdb.*;

/**
 * SSDB Java client SDK demo.
 */
public class Demo {
	public static void main(String[] args) throws Exception {
		SSDB ssdb = null;
		Response resp;
		byte[] b;
		ssdb = new SSDB("127.0.0.1", 8888);
		
		/* kv */
		System.out.println("---- kv -----");
		
		ssdb.set("a", "123");
		b = ssdb.get("a");
		System.out.println(MemoryStream.repr(b));
		ssdb.del("a");
		b = ssdb.get("a");
		System.out.println(b);
		ssdb.incr("a", 10);
		
		resp = ssdb.scan("", "", 10);
		resp.print();
		resp = ssdb.rscan("", "", 10);
		resp.print();
		System.out.println("");

		/* hashmap */
		System.out.println("---- hashmap -----");

		ssdb.hset("n", "a", "123");
		b = ssdb.hget("n", "a");
		System.out.println(MemoryStream.repr(b));
		ssdb.hdel("n", "a");
		b = ssdb.hget("n", "a");
		System.out.println(b);
		ssdb.hincr("n", "a", 10);

		resp = ssdb.hscan("n", "", "", 10);
		resp.print();
		System.out.println("");

		/* zset */
		System.out.println("---- zset -----");

		double d;
		ssdb.zset("n", "a", 1.23);
		d = ssdb.zget("n", "a");
		System.out.println(d);
		ssdb.zdel("n", "a");
		d = ssdb.zget("n", "a");
		System.out.println(d);
		ssdb.zincr("n", "a", 10);

		resp = ssdb.zscan("n", "", null, null, 10);
		resp.print();
		System.out.println("");
		
		/* multi */
		ssdb.multi_set("a", "1", "b", "2");
		resp = ssdb.multi_get("a", "b");
		resp.print();
		System.out.println("");
	
		//
		ssdb.close();
	}
}
