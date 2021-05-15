// Written by Shlomi Ben-Shushan

// Get fs and csv.
const fs = require('fs');
const adapi = require('./build/Release/AnomalyDetectionAPI');

const resultFilePath = './anomalies.txt';
var properties = [];
var sameHeaders = 1;
var differentHeadersError = 'Error: files must have the same headers.';

function createDataCSV(srcFilePath, dstFilePath) {

    // 
    try {
        if (fs.existsSync(dstFilePath)) {
            fs.unlinkSync(dstFilePath)
        }
    } catch (err) {
        console.error(err)
    }

    let content = fs.readFileSync(srcFilePath, 'utf8');
    let rows = content.split('\n');

    let firstColumns = rows[0].split(',');
    if (properties.length === 0) {
        firstColumns.forEach(c => {
            properties.push(c);
        });
    } else {
        let lim = properties.length;
        for (let i = 0; i < lim; i++) {
            if (firstColumns[i] != properties[i]) {
                sameHeaders = 0;
            }
        }
    }
    content = content.replaceAll(' ', '');
    fs.writeFileSync(dstFilePath, content, 'utf8');

}

function cleanup(resultFilePath) {
    try {
        if (fs.existsSync(resultFilePath)) {
            fs.unlinkSync(resultFilePath)
        }
    } catch (err) {
        console.error(err);
    }
}

function runAlgo(type, threshold) {
    if (!sameHeaders) {
        fs.writeFileSync('anomalies.txt', differentHeadersError, 'utf8');
    } else if (type == 0) {
        adapi.linearAnomalyDetection(threshold);
    } else if (type == 1) {
        adapi.hybridAnomalyDetection(threshold);
    }
}

function createJSON(resultFilePath) {
    fs.readFile(resultFilePath, 'utf8', function(err, data) {
        if (err) throw err;
        let anomalies = [];
        if (data.startsWith(differentHeadersError)) {
            let json = {
                ID: '-1',
                Error: differentHeadersError
            };
            anomalies.push(json);
        } else {
            let rows = data.split('\n');
            let j = 1;
            while (rows.length > 1) {
                let lim = rows.length - 1;
                let firstPair = rows[0].split('\t');
                let start = Number(firstPair[0]);
                let prop1 = firstPair[1].split(' - ')[0];
                let prop2 = firstPair[1].split(' - ')[1];
                let end = start;
                let i;
                for (i = 1; i < lim; i++) {
                    let pair = rows[i].split('\t');
                    let tempEnd = Number(pair[0]);
                    let tempProp1 = pair[1].split(' - ')[0];
                    let tempProp2 = pair[1].split(' - ')[1];
                    if (Number(end) + 1 == tempEnd && tempProp1 === prop1 && tempProp2 === prop2) {
                        end = tempEnd;
                    } else {
                        break;
                    }
                }
                rows = rows.slice(i);
                let json = {
                    ID: '#' + j++,
                    startTimeStep: start,
                    endTimeStep: end,
                    property1: prop1,
                    property2: prop2
                };
                anomalies.push(json);
            }
        }
        anomalies = JSON.stringify(anomalies, null, 4);
        fs.writeFileSync('Results.json', anomalies);
    });
}

function detectAnomalies(trainFilePath, testFilePath, type, threshold) {
    sameHeaders = 1;
    return new Promise((resolve, reject) => {

        resolve("Success");
        reject("Failed");
    })

    .then(cleanup())

    .then(createDataCSV(trainFilePath, "anomalyTrain.csv"))

    .then(createDataCSV(testFilePath, "anomalyTest.csv"))

    .then(runAlgo(type, threshold))

    .then(createJSON(resultFilePath))

    .catch((e) => {
        console.log(e);
    });


}

// detectAnomalies("./reg_flight.csv", "./anomaly_flight.csv", 0, 0.9)

module.exports = detectAnomalies;

/*
To-Do:
    1) Handle illigal files -- content or type
    2) JSONs.
*/