

print "input 'q' to quit:";

while(true){
	printf("> ");
	line = stdin.readline();
	line = line.strip().lower();
	if(line == 'q'){
		print "bye.";
		break;
	}else{
		print 'your input:', repr(line);
	}
}
