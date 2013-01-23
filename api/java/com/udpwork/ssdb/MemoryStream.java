package com.udpwork.ssdb;

import java.util.Arrays;

public class MemoryStream {
	private int capacity;
	public int data = 0;
	public int slot = 0;
	public int size = 0;
	public byte[] buf;
	
	public MemoryStream(){
		this(4096);
	}
	
	public MemoryStream(int capacity){
		this.capacity = capacity;
		buf = new byte[capacity];
	}
	
	public MemoryStream(byte[] copy){
		this(copy, 0, copy.length);
	}
	
	public MemoryStream(byte[] copy, int off, int len){
		this.capacity = len;
		this.size = len;
		buf = Arrays.copyOfRange(copy, off, off + len);
	}
	
	public byte[] toArray(){
		byte[] ret = Arrays.copyOfRange(buf, data, data + size);
		return ret;
	}

	public static String repr(byte[] bs){
		StringBuffer sb = new StringBuffer();
		for(int i=0; i<bs.length; i++){
			char b = (char)bs[i];
			//byte b = buf[i];
			if(b == '\r'){
				sb.append("\\r");
			}else if(b == '\n'){
				sb.append("\\n");
			}else if(b == '\t'){
				sb.append("\\t");
			}else if(b == '_'){
				sb.append(b);
			}else if(Character.isLetter(b) || Character.isDigit(b)){
				sb.append(b);
			}else{
				sb.append(String.format("\\%02X", (int)b));
			}
		}
		return sb.toString();
	}

	public String repr(){
		return repr(toArray());
	}
	
	public int space(){
		return capacity - slot;
	}
	
	// try realloc
	private void realloc(int require){
		while(require > space()){
			capacity *= 2;
			buf = Arrays.copyOf(buf, capacity);
		}
	}
	
	public void nice(){
		if(data > capacity/2){
			System.arraycopy(buf, data, buf, 0, size);
			data = 0;
			slot = size;
		}
	}
	
	public void decr(int num){
		size -= num;
		data += num;
	}
	
	public void write(int b){
		this.write((byte)b);
	}
	
	public void write(byte b){
		realloc(1);
		buf[size] = b;
		size += 1;
		slot += 1;
	}
	
	public void write(String s){
		this.write(s.getBytes());
	}
	
	public void write(byte[] bs){
		this.write(bs, 0, bs.length);
	}
	
	public void write(byte[] bs, int start, int len){
		realloc(len);
		System.arraycopy(bs, start, buf, slot, len);
		size += len;
		slot += len;
	}
	
	public int memchr(int b, int offset){
		return this.memchr((byte)b, offset);
	}
	
	// return offset to data
	public int memchr(byte b, int offset){
		for(int i=data+offset; i<data+size; i++){
			if(b == buf[i]){
				return i - data;
			}
		}
		return -1;
	}
}
