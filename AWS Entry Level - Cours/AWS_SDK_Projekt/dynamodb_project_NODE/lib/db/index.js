const DynamoDB = require('aws-sdk').DynamoDB;

const Instance = new DynamoDB({
    apiVersion: '2012-08-10',
    region: 'eu-west-1'
});

const tableName = 'personalData';

Instance.waitFor('tableNotExists', {
    TableName: tableName
    },
    function (err, data){
        if(err){
            return;
    }

    Instance.createTable({

        TableName: tableName,
            AttributeDefinitions: [
                {AttributeType: "S", AttributeName: "ID"}
            ],

            KeySchema: [
                {KeyType: "HASH", AttributeName: "ID"}
            ],
            ProvisionedThroughput: {
                WriteCapacityUnits: 5,
                ReadCapacityUnits: 5
            }

        }, function (err, data) {
            console.log(err, data);
        })

});

module.exports = Instance