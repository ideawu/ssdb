import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import com.udpwork.ssdb.SsdbQueue;

public class QueueDemo {
	

	private static class QueueWriter implements Runnable{
		private final int count;
		private final SsdbQueue<Message> queue;
		public QueueWriter(SsdbQueue<Message> queue, int count){
			this.count = count;
			this.queue = queue;
		}
		@Override
		public void run() {
			System.out.println("QueueWriter start");
			for(int index = 0; index< this.count; index++){
				Message msg = new Message();
				msg.setAge(index);
				msg.setName("name_"+index);
				queue.offer(msg);
			}
			System.out.println("QueueWriter stop");
		}
	}
	
	private static class QueueReader implements Runnable{
		private final SsdbQueue<Message> queue;
		private final int count;
		public QueueReader(SsdbQueue<Message> queue, int count){
			this.queue = queue;
			this.count = count;
		}
		@SuppressWarnings("static-access")
		@Override
		public void run() {
			System.out.println("QueueReader start");			
			int index = 0;
			while(index < this.count){
				Message outMsg = queue.poll();
				if(null == outMsg){
					try {Thread.currentThread().sleep(10);} catch (InterruptedException e) {}
					continue;
				}
				index++;
				System.out.println(outMsg);
			}
			System.out.println("QueueReader stop");
			//read all,then close queue;
			queue.close();
		}
	}

	public static void main(String[] args) {
		int count = 100;
		SsdbQueue<Message> queue = new SsdbQueue<Message>("msg");
		//SsdbQueue<Message> queue = new SsdbQueue<Message>("msg","127.0.0.1");
		//SsdbQueue<Message> queue = new SsdbQueue<Message>("msg","127.0.0.1",8888);
		//SsdbQueue<Message> queue = new SsdbQueue<Message>("msg","127.0.0.1",8888,3000);
        QueueWriter writer = new QueueWriter(queue,count);
		QueueReader reader = new QueueReader(queue,count);
		Executor executor = Executors.newCachedThreadPool();
		executor.execute(writer);
		executor.execute(reader);
	}
}
class Message{
	private String name;
	private Integer age;
	public final String getName() {
		return name;
	}
	public final void setName(String name) {
		this.name = name;
	}
	public final Integer getAge() {
		return age;
	}
	public final void setAge(Integer age) {
		this.age = age;
	}
	@Override
	public String toString() {
		return "Message [name=" + name + ", age=" + age + "]";
	}
}
