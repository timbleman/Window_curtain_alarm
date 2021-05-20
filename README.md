const express = require('express');
const router = express.Router();

var id_counter = 0
var messages = []



router.get('/', (req, res, next) => {

    res.status(200).json(messages);

});



router.post('/', (req, res, next) => {
    
    id_counter += 1;

    let newMessage = {
        id: id_counter,
        name: req.body.name,
        nachname: req.body.nachname,
        time: req.body.time,
        message: req.body.message
    }

    messages.push(newMessage)

    res.status(200).json(messages);

});



module.exports = router;
