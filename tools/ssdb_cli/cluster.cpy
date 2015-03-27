import util.*;

function kv_node_list(resp, time_consume){
	len_index = 0;
	count = 0;
	while(len(resp.data) > len_index){
		kv_len = int(resp.data[len_index]);
		if(kv_len < 6){
			printf('bad response!\n');
			break;
		}
		if(len(resp.data) >= len_index + kv_len){
			count += 1;
			id      = resp.data[len_index + 1];
			status  = resp.data[len_index + 2];
			range_s = resp.data[len_index + 3];
			range_e = resp.data[len_index + 4];
			ip      = resp.data[len_index + 5];
			port    = resp.data[len_index + 6];
			
			status_text = 'UNKNOWN';
			if(status == '0'){
				status_text = 'INIT';
			}else if(status == '1'){
				status_text = 'SERVING';
			}
			
			printf('id: %s\n', id);
			printf('    status: %s\n', status_text);
			printf('    range:  (\"%s\", \"%s\"]\n', range_s.encode('string-escape'), range_e.encode('string-escape'));
			printf('    ip:     %s\n', ip);
			printf('    port:   %s\n', port);
		}
		len_index += 6 + 1;
	}
	sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', count, time_consume));
}
