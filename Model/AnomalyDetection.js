// Written by Shlomi Ben-Shushan

// Get fs and csv.
const { forEach } = require('csv-string');
const fs = require('fs');
const adapi = require('./build/Release/AnomalyDetectionAPI');
const resultFilePath = './anomalies.txt';

class Anomaly {
    id;
    startTimeStep;
    endTimeStep;
    prop1;
    prop2;
    constructor(id, start, end, p1, p2) {
        this.id = id;
        this.startTimeStep = start;
        this.endTimeStep = end;
        this.prop1 = p1;
        this.prop2 = p2;
    }
}

function isNumeric(str) {
    if (typeof str != "string")
        return false
    return !isNaN(str) && !isNaN(parseFloat(str))
}

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

    // let firstColumns = rows[0].split(',');
    // if (isNumeric(firstColumns[0])) {
    //     let defaultHeaders = ''
    //     let lim = firstColumns.length - 1;
    //     let j;
    //     for (j = 0; j < lim; j++) {
    //         defaultHeaders += 'prop' + (j + 1) + ','; // Reminder!!!!! what to do when no headers?????
    //     }
    //     defaultHeaders += 'prop' + (j + 1) + '\n';
    //     content = defaultHeaders + content;
    // }

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
    if (type == 0) {
        adapi.linearAnomalyDetection(threshold);
    } else if (type == 1) {
        adapi.hybridAnomalyDetection(threshold);
    }
}

function readOutput(resultFilePath) {
    fs.readFile(resultFilePath, 'utf8', function(err, data) {
        if (err) throw err;
        let anomalies = [];
        let rows = data.split('\n');
        let j = 1;
        while (rows.length > 1) {
            let id = "#" + j++;
            let lim = rows.length - 1;
            let firstPair = rows[0].split('\t');
            let start = firstPair[0];
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
            anomalies.push(new Anomaly(id, start, end, prop1, prop2));
        }

        anomalies.forEach(a => {
            // console.log(a.id + " " + a.startTimeStep + " " + a.endTimeStep + " " + a.prop1 + " " + a.prop2);
            // 
        });

    });
}


function detectAnomalies(trainFilePath, testFilePath, type, threshold) {
    return new Promise((resolve, reject) => {
        reject("Success");
        resolve("Failed");
    })

    .then(cleanup())

    .then(createDataCSV(trainFilePath, "anomalyTrain.csv"))

    .then(createDataCSV(testFilePath, "anomalyTest.csv"))

    .then(runAlgo(type, threshold))

    .then(readOutput(resultFilePath))

    .catch((e) => {
        console.log(e);
    });


}

// detectAnomalies("./reg_flight.csv", "./anomaly_flight.csv", 0, 0.9)

module.exports = detectAnomalies;

/*
To-Do:
    1) Handle illigal files.
    2) JSONs.
*/