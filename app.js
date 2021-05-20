/* app.use((error, req, res, next) => { res.status(error.status || 500); res.json({ error: { message: error.message } }); }); */

var colors = require('colors'); 
const express = require('express'); 
const path = require('path');
var bodyParser = require('body-parser');

const app = express(); 
const port = process.env.PORT || 8080;

app.use(express.static(path.join(__dirname, 'index')));
app.use(bodyParser());

app.get('/',express.static(path.join(__dirname, 'index')));
app.get('/', function(req, res){
    res.sendFile(__dirname + '/index.html');
});


app.get('/messages', function(req, res){
    res.send(
        
    );
});


// app.post('/messages', function(req, res){
//   var txt_folder_name = req.body.order;
//   console.log("txt_folder_name", txt_folder_name)
// })


// app.use('/',express.static(path.join(__dirname, '/')));

const timeElapsed = Date.now();
const today = new Date(timeElapsed); 

// Console.log area
console.log('Time: '.yellow, today.toUTCString());
console.log("Client Server started at http://localhost: ".green + port.toString()+ '');

app.listen(port)