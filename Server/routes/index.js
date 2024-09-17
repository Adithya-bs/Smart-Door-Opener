var express = require('express');
var router = express.Router();
var path = require('path');
var axios = require('axios');

/* GET home page. */
router.get('/door_status', function (req, res, next) {
    axios.get('http://192.168.4.1/status')
        .then(function (response) {
            // handle success
            const number = parseInt(response.data);
            //console.log('Received number:', number);
            res.json({ data: number });
        })
        .catch(error => {
            console.error('Error making request:', error);
            // Send an error response if request to ESP32 failed
            res.status(500).json({ error: 'Error fetching data from ESP32 server' });


        })
    //   res.render('index', { title: 'Express' });

});



module.exports = router;
