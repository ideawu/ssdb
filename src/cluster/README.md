# 分布式原语

### 数据结构

* node_list: 当前可提供服务的 data node 列表, 相当于路由表. 路由表中记录着每一个节点的数据范围, 同时, 节点自己也记录着自身的数据范围. 注意: 这两个范围可能出现暂时的不一致, 但应该是最终一致的.

### 指令

* add_node: 将一个节点添加到路由表中, 无需记 oplog.
* split_node: 将路由表中的一个节点的数据, 部分移动到新节点, 同时把新节点添加到路由表中. 需要记 oplog.
* migrate_data: 将路由表中的一个节点的数据, 部分移动到路由表中的另一个节点. 需要记 oplog.

### commit 点(oplog)

	add_node(id, ip, port, key_range)
	del_node(id, ip, port)
	
	prepare_split_node(src_id, dst_id, dst_ip, dst_port)
	
	prepare_migrate_data(src_id, dst_id)
	


