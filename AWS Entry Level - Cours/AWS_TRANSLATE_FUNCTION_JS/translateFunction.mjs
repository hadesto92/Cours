const AWS = require('aws-sdk');

export const handler = async(event) => {
    // TODO implement
    
    let translate = new AWS.Translate();
    let body = JSON.parse(event.body);
    
    return new Promise((resolve, reject) => {
        translate.translateText({
            SourceLanguageCode: 'pl',
            TargetLanguageCode: 'en',
            Text: body.text
        }, function(err, data){
            if(err){
                reject({
                    statusCode: 500,
                    body: JSON.stringify(err)
                })
                return;
            }
            resolve({
                statusCode: 200,
                body: JSON.stringify(data)
            })
        });
    });
};