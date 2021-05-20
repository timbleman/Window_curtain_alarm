const express = require('express');
const router = express.Router();

var id_counter = 0
var messages = []

router.get('/', (req, res, next) => {
res.status(200).json(messages);
});

router.post('/messages', (req, res, next) => {
id_counter += 1;

let newMessage = {
    order: req.body.order
}

messages.push(newMessage)
res.status(200).json(messages);
});

module.exports = router;