// Oldmain:

const adapi = require('./build/Release/AnomalyDetectionAPI')

function isNumeric(str) {
    if (typeof str != "string")
        return false
    return !isNaN(str) && !isNaN(parseFloat(str))
}

function createDataCSV(srcFilePath, dstFilePath) {

    // Get fs and csv.
    const fs = require('fs')
    var parse = require('csv-parse')

    // Remove anomalies.txt file if exists.
    try {
        if (fs.existsSync(dstFilePath)) {
            fs.unlinkSync(dstFilePath)
        }
    } catch (err) {
        console.error(err)
    }

    // Read data and create input-files for Anomaly Detection algorithms.
    var data = [];
    fs.createReadStream(srcFilePath).pipe(parse()).on('data', (row) => {
        data.push(row);
    }).on('end', () => {
        if (isNumeric(data[0][0])) {
            var defaultHeaders = []
            let lim = data[0].length;
            for (let j = 0; j < lim; j++) {
                defaultHeaders[j] = "prop" + (j + 1);
            }
            data.unshift(defaultHeaders);
        }
        var file = fs.createWriteStream(dstFilePath);
        file.on('error', function(err) { /* error handling */ });
        data.forEach(function(v) { file.write(v.join(', ') + '\n'); });
        file.end();
        console.log("before");
    });
}

function detectAnomalies(trainFilePath, testFilePath, type, threshold) {

    // Get fs.
    const fs = require('fs')
    var resultFilePath = './anomalies.txt';

    // Remove anomalies.txt file if exists.
    try {
        fs.unlinkSync(resultFilePath)
    } catch (err) {
        console.error(err)
    }

    // Create new CSV files.
    createDataCSV(trainFilePath, "anomalyTrain.csv");
    createDataCSV(testFilePath, "anomalyTest.csv");
    console.log("after");

    // Run algo.
    if (type == 0) {
        adapi.linearAnomalyDetection(threshold);
    } else if (type == 1) {
        adapi.hybridAnomalyDetection(threshold);
    }

    // Read output.
    fs.readFile(resultFilePath, 'utf8', function(err, data) {
        if (err) throw err;
        // console.log('OK: ' + filename);
        // console.log(data)
    });

}

detectAnomalies("./reg_flight.csv", "./anomaly_flight.csv", 0, 0.9)