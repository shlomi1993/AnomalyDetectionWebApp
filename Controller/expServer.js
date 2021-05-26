// Requirements.
var path = require('path');
const express = require('express');
const fileUpload = require('express-fileupload');
const model = require('../Model/AnomalyDetection');

// Using express server.
const app = express();

// Encoding.
app.use(
    express.urlencoded({
        extended: false,
    })
);

// To allow the user upload files.
app.use(fileUpload());

// Set root as ../View
app.use(express.static('../View'));

// GET retrieves resources
app.get('/', (req, res) => {
    res.sendFile('./index.html');
});

// POST submits new data to the server.
//    * input algo:  HYBRID/REG, CSV file, another file
//    * output JSON: get JSON file from Model to View
app.post('/detect', (req, res) => {

    // Check existance of files and algorithm.
    if (!req.files) {
        res.write('Error: missing training and test files.\n');
        res.end();
        return;
    }
    if (!req.files.training_file) {
        res.write('Error: missing training file.\n');
        res.end();
        return;
    }
    if (!req.files.testing_file) {
        res.write('Error: missing test file.\n');
        res.end();
        return;
    }
    if (req.body.algorithm === 'None') {
        res.write('Error: missing algotihrm.\n');
        res.end();
        return;
    }

    // Extract files' names and extensions.
    let trainName = req.files.training_file.name;
    let testName = req.files.testing_file.name;
    let ext1 = path.extname(trainName).toLowerCase();
    let ext2 = path.extname(testName).toLowerCase();

    // Verify files' extension validity.
    if (ext1 !== '.csv' || ext2 !== '.csv') {
        res.write('Error: invalid file type:\n');
        if (ext1 !== '.csv') {
            res.write('\t' + trainName + ' is not a CSV type.\n');
        }
        if (ext2 !== '.csv') {
            res.write('\t' + testName + ' is not a CSV type.\n');
        }
        res.end();
        return;
    }

    // Get binary data from user's files.
    let trainData = req.files.training_file.data;
    let testData = req.files.testing_file.data;

    // Convert user's choice for model function.
    let algorithm = req.body.algorithm;
    let choice = 0;
    if (algorithm === 'Circular Algorithm') {
        choice = 1;
    } else if (algorithm === 'Hybrid Algorithm') {
        choice = 2;
    }

    // Convert user's threshold for model function.
    let threshold = req.body.new_threshold;

    // Run algo.
    model.detectAnomalies(trainData, testData, choice, threshold, (result) => {
        let jsonData = JSON.parse(result);
        if (jsonData[0].ID === '-1') {
            res.send(jsonData[0].Error);
        } else {
            let out = ''
            jsonData.forEach(json => out += JSON.stringify(json, null, 4) + '\n');
            res.send(out);
        }
        res.end();
    })

})

// Listening...
app.listen(8081, () => console.log('Anomaly Detection Server is up!'));