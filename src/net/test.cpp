#include "server.h"

void usage(int argc, char **argv){
	printf("Usage:\n");
	printf("    %s [-d] /path/to/server.conf\n", argv[0]);
	printf("Options:\n");
	printf("    -d    run as daemon\n");
}

int main(int argc, char **argv){
	bool is_daemon = false;
	const char *conf_file = NULL;
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			is_daemon = true;
		}else{
			conf_file = argv[i];
		}
	}
	if(conf_file == NULL){
		usage(argc, argv);
		exit(1);
	}

	Server serv;
	serv.run(conf_file, is_daemon);
	return 0;
}
