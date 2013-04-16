var SSDB = require('./SSDB.js');
var ssdb = SSDB.connect(host, port);

ssdb.set('a', new Date(), function(){
	console.log('set a');
});
ssdb.get('a', function(err, val){
	console.log('get a = ' + val);
	ssdb.close();
});


