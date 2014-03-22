package com.udpwork.ssdb;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Response{
	public String status;
	public List<byte[]> raw;
	/**
	 * Indicates items' order
	 */
	public List<byte[]> keys = new ArrayList<byte[]>();
	/**
	 * key-value results
	 */
	public Map<byte[], byte[]> items = new LinkedHashMap<byte[], byte[]>();
	
	public Response(List<byte[]> raw){
		this.raw = raw;
		if(raw.size() > 0){
			status = new String(raw.get(0));
		}
	}
	
	public Object exception() throws Exception{
		if(raw.size() >= 2){
			throw new Exception(new String(raw.get(1)));
		}else{
			throw new Exception("");
		}
	}

	public boolean ok(){
		return status.equals("ok");
	}
	
	public boolean not_found(){
		return status.equals("not_found");
	}
	
	public void buildMap(){
		for(int i=1; i<raw.size(); i+=2){
			byte[] k = raw.get(i);
			byte[] v = raw.get(i+1);
			keys.add(k);
			items.put(k, v);
		}
	}

	public void print(){
		System.out.println(String.format("%-15s %s", "key", "value"));
		System.out.println("---------------------");
		for (byte[] bs : keys) {
			System.out.print(String.format("%-15s", MemoryStream.repr(bs)));
			System.out.print(": ");
			System.out.print(MemoryStream.repr(items.get(bs)));
			System.out.println();
		}
	}
}
