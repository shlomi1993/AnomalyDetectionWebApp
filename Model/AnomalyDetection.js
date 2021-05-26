// Written by Shlomi Ben-Shushan

// Get FS and anomaly detection API.
const fs = require('fs');
const adapi = require('../Model/build/Release/AnomalyDetectionAPI');

// Set globals and consts for anomaly detection process. 
var propertiesArrays = [];
const resultsFile = './anomalies.txt';
const anomalyTrain = './anomalyTrain.csv';
const anomalyTest = './anomalyTest.csv';
const differentHeadersError = 'Error: files must have the same headers.';

// For each file (path) in the given array, this function remove it if it exists.
function cleanup(array) {
    propertiesArrays = [];
    array.forEach((file) => {
        try {
            if (fs.existsSync(file)) {
                fs.unlinkSync(file);
            }
        } catch (err) {
            console.error(err);
        }
    });
}

// This function gets a csv path and create a new csv fit to the c++ program.
function createDataCSV(srcFileData, dstFileData) {

    // Transform binary data to a parsable content and split it to rows.
    let base64data = Buffer.from(srcFileData, 'binary').toString('base64');
    let content = Buffer.from(base64data, 'base64').toString();
    let rows = content.split('\n');

    // Eliminate spaces -- to fit the C++ program.
    content = content.replace(' ', '');

    // Push each header in the csv to properties global array for later comparison.
    let headers = rows[0].split(',');
    properties = []
    headers.forEach(h => { properties.push(h); });
    propertiesArrays.push(properties);

    // Create a new file locally.
    fs.writeFileSync(dstFileData, content, 'utf8');

}

// This function compares the two header arrays of the two given files (train and test) and return
//false if a change is found. Else, return true (train and test files must have the same headers).
function checkHeaders() {
    if (propertiesArrays.length < 2) return false;
    let headers1 = propertiesArrays[0];
    let headers2 = propertiesArrays[1];
    let lim = headers1.length;
    if (lim != headers2.length) return false;
    for (let i = 0; i < lim; i++) {
        if (headers1[i] != headers2[i]) {
            return false;
        }
    }
    return true;
}

// This function gets an algo type and threshold and run the rlevant algo.
// If the different files doesn't have the same headers, output an error message.
function runAlgo(type, threshold) {
    if (checkHeaders() == false) {
        fs.writeFileSync('anomalies.txt', differentHeadersError, 'utf8');
    } else if (type == 0) {
        adapi.linearAnomalyDetection(threshold);
    } else if (type == 1) {
        adapi.hybridAnomalyDetection(2); // Threshold=2 forces the hybrid detector to find circular anomalies only.
    } else if (type == 2) {
        adapi.hybridAnomalyDetection(threshold);
    }
}

// This function creats a JSON object out of all the anomalies found and send it via callback.
function createJSON(resultsFile) {

    // Read anomalies.txt file.
    let data = fs.readFileSync(resultsFile, 'utf8');

    // Create a new array for all the anomalies found in the process.
    let anomalies = [];

    // If headers are different, output a JSON with only one entry -- error message.
    if (data.startsWith(differentHeadersError)) {
        let json = {
            ID: '-1',
            Error: differentHeadersError
        };
        anomalies.push(json);
    }

    // Else, parse the results in anomalies.txt file.
    else {

        // We'll parse each and ecery row.
        let rows = data.split('\n');

        // Each anomaly have an ID -- #1, #2, #3, and so on...
        let id = 1;

        // This loop parse each relevant information and store it in the relevant variable:
        //      - start for start time step.
        //      - end for end time step.
        //      - prop1 for property number 1 time step.
        //      - prop2 for schnitzel. Or for property number 2.
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

            // In each iteration, create a jsoned entry and push it to the array of anomalies.
            let json = {
                ID: '#' + id++,
                startTimeStep: start,
                endTimeStep: end,
                property1: prop1,
                property2: prop2
            };
            anomalies.push(json);

        }

    }

    // In the end, convert a jsons to strings and notify result.
    return JSON.stringify(anomalies, null, 4)

}

// This function gets train and test CSVs, a type of algo (0 for regression, 1 for hybrid) and threshold.
// in creates two CSVs fitted to the C++ program, run the anomaly detection algo according to the given
// type, and than creates a Results.json out of anomalies.txt.
function detectAnomalies(trainFilePath, testFilePath, type, threshold, callback) {
    cleanup([resultsFile, anomalyTrain, anomalyTest]);
    createDataCSV(trainFilePath, anomalyTrain);
    createDataCSV(testFilePath, anomalyTest);
    runAlgo(type, threshold);
    callback(createJSON(resultsFile));
}

// Expose detectAnomalies() for an outside use.
module.exports.detectAnomalies = detectAnomalies;