/* app.use((error, req, res, next) => { res.status(error.status || 500); res.json({ error: { message: error.message } }); }); */

var colors = require('colors'); 
const express = require('express'); 
const path = require('path');
var bodyParser = require('body-parser');

const app = express(); 
const port = process.env.PORT ||3000;

app.use(express.static(path.join(__dirname, 'public')));
app.use(bodyParser.urlencoded({ extended: true })); 

app.get('/',express.static(path.join(__dirname, 'index')));
// app.get('/', function(req, res){
//     res.sendFile(__dirname + '/index.html');
// });


// app.get('/messages', function(req, res){
//          myText = req.body.order;
//          res.send('Your Text:' +myText);
// });


// app.get('/messages', function(req, res){ 
//     var myText = req.body.mytext; //mytext is the name of your input box
//     res.send('Your Text:' + myText); 
// }); 


// app.post('/myform', function(req, res) {
//     var myText = req.body.mytext; //mytext is the name of your input box
//       res.send('Your Text:' +myText); 
//   });

  app.post('/https://172.20.10.2:4000', (req, res) => {
    var username = req.body.username
    console.log(username)
    res.send(username)
  })
// app.post('/messages', function(req, res){
//   var txt_folder_name = req.body.order;
//   console.log("txt_folder_name", txt_folder_name)
// })


// app.use('/',express.static(path.join(__dirname, '/')));

const timeElapsed = Date.now();
const today = new Date(timeElapsed)

// Console.log area
console.log(
"\nClient Server started at ".green + "http://localhost:".cyan + port.toString()+ '' +
"\tTime: ".yellow, today.getHours() + ':'+ today.getMinutes()
);

app.listen(port)