var express = require('express');
var router = express.Router();
var instance = require('../lib/db')

/* GET home page. */
router.get('/', function(req, res, next) {

  new Promise(function (resolve, reject){
    instance.scan({
      TableName: 'personalData',
    },function (err, data) {
      if(err){
        resolve([]);
      }
      resolve(data.Items);
    });
  }).then(function (items) {
    res.render('index', { title: 'Express', items: items });
  })
});

router.post('/add', function(req, res, next) {

  instance.putItem({
    TableName: 'personalData',
    Item: {
      ID: {S: req.body.id},
      NAME: {S: req.body.name},
      SURNAME: {S: req.body.surname}
    }
  }, function (err, data){
    console.log(err, data);
  })

  res.render('index', { title: 'Express' });
});

module.exports = router;
