const express     = require('express');
const bodyParser  = require('body-parser');
const parseJson   = require('parse-json');
const WebSocketServer = require('uws').Server;
const wss = new WebSocketServer({ port: 4567});

const app = express();
app.set('port', process.env.PORT|| 3000);
app.use(express.static(__dirname + '/public'));
app.set('view engine', 'ejs');

data =[]
app.get('/', function(req, res) {
	res.render('index', {
		title:'Crawler',
		data: data
	});
});

function onMessage(message) {
   item = parseJson(message);
   data.push(item);
   if (data.length > 10) data.shift();
}
 
wss.on('connection', function(ws) {
	data = [];
	ws.on('message', onMessage);
    ws.send('Welcome to the big Family!');
});


app.listen(app.get('port'), function(){
	console.log('Express started on localhost: ' + app.get('port'));
}); 