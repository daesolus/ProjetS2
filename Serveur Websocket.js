var WebSocketServer = require('ws').Server
  , wss = new WebSocketServer({port: 56453});
wss.on('connection', function(ws) {
	console.log('Connected');
	ws.on('close', function(close) {
        console.log('close: %s', close);
		//wss.clients.forEach(function each(client) {
    		//	client.send(message);
  		//});
    });
    ws.on('message', function(message) {
        console.log('received: %s', message);
			wss.clients.forEach(function each(client) {
    				client.send(message);
  			});

    });
    //ws.send('something');
});
