
function show_version(link){
	try{
		resp = link.request('info', []);
		sys.stderr.write(resp.data[0] + ' ' + resp.data[2] + '\n\n');
	}catch(Exception e){
	}
}

function dbsize(link){
	resp = link.request('info', []);
	foreach(resp.data as k=>v){
		if(v != 'leveldb.stats'){
			continue;
		}
		s = resp.data[k + 1];
		lines = s.strip().split('\n');
		lines = lines[ 3 ..];
		size = 0;
		foreach(lines as line){
			ps = line.split();
			size += int(ps[2]);
		}
		return size;
	}
	return 0;
}

class SSDB_iterator_base
{
	public link;
	public finish = false;
	public batch = 2;
	public index = [];
	public key = '';
	public val = '';
	public end = '';
	
	function init(link){
		this.link = link;
	}
	
	function seek(s){
		this.key = s;
	}
	
	function set_range(s, e=''){
		this.key = s;
		this.end = e;
	}
}


class SSDB_kv_scan extends SSDB_iterator_base
{
	public items = [];

	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('scan', [this.key, this.end, this.batch]);
			if(len(resp.data['index']) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data['index'];
			this.items = resp.data['items'];
		}
		this.key = this.index.pop(0);
		this.val = this.items[this.key];
		return true;
	}
}

/*
scan = new SSDB_kv_scan();
while(kvs.next()){
	print scan.key, scan.val;
}
*/

class SSDB_hash_list extends SSDB_iterator_base
{
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('hlist', [this.key, this.end, this.batch]);
			if(len(resp.data) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data;
		}
		this.key = this.index.pop(0);
		return true;
	}
}


class SSDB_zset_list extends SSDB_iterator_base
{
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('zlist', [this.key, this.end, this.batch]);
			if(len(resp.data) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data;
		}
		this.key = this.index.pop(0);
		return true;
	}
}

/*
kvs = new SSDB_zset_list();
while(kvs.next()){
	print kvs.name;
}
*/


class SSDB_queue_list extends SSDB_iterator_base
{
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('qlist', [this.key, this.end, this.batch]);
			if(len(resp.data) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data;
		}
		this.key = this.index.pop(0);
		return true;
	}
}



class SSDB_hash_scan extends SSDB_iterator_base
{
	public name = '';
	public items = [];
	
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('hscan', [this.name, this.key, '', this.batch]);
			if(len(resp.data['index']) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data['index'];
			this.items = resp.data['items'];
		}
		this.key = this.index.pop(0);
		this.val = this.items[this.key];
		return true;
	}
}

/*
scan = new SSDB_hash_scan('n');
while(scan.next()){
	print scan.key, scan.val;
}
*/



class SSDB_zset_scan extends SSDB_iterator_base
{
	public name = '';
	public items = [];
	
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('zscan', [this.name, this.key, this.val, '', this.batch]);
			if(len(resp.data['index']) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data['index'];
			this.items = resp.data['items'];
		}
		this.key = this.index.pop(0);
		this.val = this.items[this.key];
		return true;
	}
}

/*
scan = new SSDB_zset_scan('n');
while(scan.next()){
	print scan.key, scan.val;
}
*/



class SSDB_queue_scan extends SSDB_iterator_base
{
	public items = [];
	public offset = 0;
	
	function init(link){
	}
	
	function next(){
		if(this.finish){
			return false;
		}
		if(len(this.index) == 0){
			resp = this.link.request('qrange', [this.name, this.offset, this.batch]);
			if(len(resp.data) == 0){
				this.finish = true;
				return false;
			}
			this.index = resp.data;
		}
		this.key = this.offset;
		this.val = this.index.pop(0);
		this.offset += 1;
		return true;
	}
}

/*
scan = new SSDB_queue_scan('q');
while(scan.next()){
	print scan.item;
}
*/
