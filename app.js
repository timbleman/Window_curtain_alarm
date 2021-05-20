/* app.use((error, req, res, next) => { res.status(error.status || 500); res.json({ error: { message: error.message } }); }); */

var colors = require('colors'); 
const express = require('express'); 
const path = require('path');

const app = express(); 
const port = process.env.PORT || 8080;

// app.use('/',express.static(path.join(__dirname, '/')));

const timeElapsed = Date.now();
const today = new Date(timeElapsed); 
console.log('Time: '.yellow, today.toUTCString());

app.use('/',(req,res,next)=>{ 
    res.send('')
 });
 
 // sendFile will go here app.get('/', function(req, res) { res.send("everything works") });

// sendFile(path.join(__dirname, '/index.html')) app.listen(port);

console.log("Client Server started at http://localhost: ".green + port.toString()+ '');

app.listen(port)