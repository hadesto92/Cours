var express = require('express');
var router = express.Router();
var sdk = require('aws-sdk');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.post('/upload', function(req, res, next) {

  var s3 = new sdk.S3;

  var params = {
    Bucket: 'test-aws-bucket-5790',
    Key: req.body.s3file,
    Body: req.body.s3file
  };

  //var test = req.body.s3file;
  //console.log(test[1].name);

  s3.upload(params, function (err, data) {
    console.log(err, data);
  });

  res.render('index', { title: 'Express' });
});

module.exports = router;
