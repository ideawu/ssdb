package com.udpwork.ssdb;

import java.util.AbstractQueue;
import java.util.Iterator;
import java.util.Queue;
import java.util.concurrent.locks.ReentrantLock;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.parser.Feature;
import com.alibaba.fastjson.serializer.SerializerFeature;

/**
 * @ClassName : SsdbQueue
 * @Description : 基于SSDB实现的持久化的、线程安全的、类型安全的Queue
 * @date : 2013年12月18日 下午4:14:46
 * @author : 玛雅牛(lifei(AT)wellbole.com)
 * @link : http://www.wellbole.com
 */
public class SsdbQueue<E> extends AbstractQueue<E> implements Queue<E>,
		java.io.Serializable {
	private static final long serialVersionUID = 3918396353646776987L;

	private final String host;

	private final int port;

	private final int timeout_ms;

	private final String queueName;
	/**
	 * 建立四条链路，并发访问，性能最好。
	 */
	private SSDB offerSsdb = null;
	private SSDB pollSsdb = null;
	private SSDB peekSsdb = null;
	private SSDB sizeSsdb = null;

	/**
	 * 锁，保证多线程情况下正确读取返回结果
	 */
	private ReentrantLock offerLocker = new ReentrantLock();
	private ReentrantLock pollLocker = new ReentrantLock();
	private ReentrantLock peekLocker = new ReentrantLock();
	private ReentrantLock sizeLocker = new ReentrantLock();
	
	public SsdbQueue(String queueName) {
		this(queueName, "127.0.0.1", 8888, 0);
	}

	public SsdbQueue(String queueName, String host) {
		this(queueName, host, 8888, 0);
	}

	public SsdbQueue(String queueName, String host, int port) {
		this(queueName, host, port, 0);
	}

	public SsdbQueue(String queueName, String host, int port, int timeout_ms) {
		this.host = host;
		this.port = port;
		this.timeout_ms = timeout_ms;
		this.queueName = queueName;
		try {
			offerSsdb = new SSDB(this.host, this.port, this.timeout_ms);
			pollSsdb = new SSDB(this.host, this.port, this.timeout_ms);
			peekSsdb = new SSDB(this.host, this.port, this.timeout_ms);
			sizeSsdb = new SSDB(this.host, this.port, this.timeout_ms);
		} catch (Exception e) {
			throw new RuntimeException("SSDB init error", e);
		}
	}

	@Override
	public boolean offer(E e) {
		try {
			offerLocker.lock();
			byte[] value = JSON
					.toJSONBytes(e, SerializerFeature.WriteClassName);
			offerSsdb.qoffer(queueName, value);
			return true;
		} catch (Exception e1) {
			throw new RuntimeException("offer error", e1);
		} finally {
			offerLocker.unlock();
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public E poll() {
		try {
			pollLocker.lock();
			byte[] value = pollSsdb.qpoll(queueName);
			if (null == value) {
				return null;
			}
			// logger.debug(new String(value));
			return (E) JSON.parse(value, Feature.SortFeidFastMatch);
		} catch (Exception e) {
			throw new RuntimeException("poll error", e);
		} finally {
			pollLocker.unlock();
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public E peek() {
		try {
			peekLocker.lock();
			byte[] value = peekSsdb.qpeek(queueName);
			if (null == value) {
				return null;
			}
			// logger.debug(new String(value));
			return (E) JSON.parse(value, Feature.SortFeidFastMatch);
		} catch (Exception e) {
			throw new RuntimeException("peek error", e);
		} finally {
			peekLocker.unlock();
		}
	}

	@Override
	public Iterator<E> iterator() {
		throw new UnsupportedOperationException("Unsupported iterator");
	}

	@Override
	public int size() {
		throw new UnsupportedOperationException("Unsupported size");
	}

	@Override
	public boolean isEmpty() {
		try {
			sizeLocker.lock();
			return sizeSsdb.qsize(queueName) > 0;
		} catch (Exception e) {
			throw new RuntimeException("get queue size error", e);
		} finally {
			sizeLocker.unlock();
		}
	}

	public void close() {
		if (this.offerSsdb != null) {
			this.offerSsdb.close();
		}
		if (this.pollSsdb != null) {
			this.pollSsdb.close();
		}
		if (this.peekSsdb != null) {
			this.peekSsdb.close();
		}
		if (this.sizeSsdb != null) {
			this.sizeSsdb.close();
		}
	}
}
