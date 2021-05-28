// Requirements.
var path = require('path');
const express = require('express');
const fileUpload = require('express-fileupload');
const model = require('../Model/AnomalyDetection');
const view = require('../View/view.js');

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

// This function creates the required input for the Anomaly Detection model, call it, and then call the given callback.
function detect(req, res, callback) {

    // Check existance of files and algorithm.
    let errMsg = ''
    if (!req.files) {
        errMsg += 'Error: missing training and test files.\n';
    } else if (!req.files.training_file) {
        errMsg += 'Error: missing training file.\n';
    } else if (!req.files.testing_file) {
        errMsg += 'Error: missing test file.\n';
    }
    if (req.body.algorithm === 'None') {
        errMsg += 'Error: missing algotihrm.\n';
    }
    if (errMsg !== '') {
        res.write(errMsg);
        res.end();
        return;
    }

    // Extract files' names and extensions.
    let trainName = req.files.training_file.name;
    let testName = req.files.testing_file.name;
    let ext1 = path.extname(trainName).toLowerCase();
    let ext2 = path.extname(testName).toLowerCase();

    // Verify files' extension validity.
    if (ext1 !== '.csv') {
        errMsg += 'Error: invalid file type: ' + trainName + '\n';
    }
    if (ext2 !== '.csv') {
        errMsg += 'Error: invalid file type: ' + testName + '\n';
    }
    if (errMsg !== '') {
        res.write(errMsg);
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
    model.detectAnomalies(trainData, testData, choice, threshold, (result) => callback(req, res, result))

}

function sendJSON(req, res, result) {
    let jsons = JSON.parse(result);
    if (jsons.length == 0 && jsons[0].ID === '-1') {
        res.write(jsons[0].Error);
    } else {
        res.write(result);
    }
    res.end();
}

// POST /detect call detect function and then send a JSON result to the requester.
app.post('/detect', (req, res) => detect(req, res, sendJSON))

// POST /detectFromView call detect function and then call view's sendHTML function to create something visual.
app.post('/detectFromView', (req, res) => detect(req, res, view.sendHTML))

// Listening...
app.listen(8080, () => console.log('Anomaly Detection Server is up!'));