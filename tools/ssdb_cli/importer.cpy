function run(link, filename){
	if(!os.path.exists(filename)){
		print 'Error: ' + filename + ' not exists!';
		return;
	}
	total_size = os.path.getsize(filename);
	if(total_size == 0){
		total_size = 1;
	}

	progress = 0;
	read_size = 0;
	fp = open(filename, 'r');
	lineno = 0;
	foreach(fp as line){
		lineno ++;
		read_size += len(line);
		progress_2 = int(float(read_size)/total_size * 100);
		if(progress_2 - progress >= 5 || read_size == total_size){
			progress = progress_2;
			printf("%2d%%\n", progress_2);
		}
		
		ps = line.strip().split('\t');
		if(len(ps) < 2){
			print 'Error: bad format at line ' + str(lineno) + ', abort!';
			return;
		}
		cmd = ps[0].lower();
		foreach(ps as k=>v){
			ps[k] = str(v).decode('string-escape');
		}
		
		link.request(cmd, ps[ 1 ..]);
	}
	print 'done.';
}
