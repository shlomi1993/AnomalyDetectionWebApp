// Requirements.
const fs = require('fs')
const express = require('express')
const fileUpload = require('express-fileupload')
const model = require('../Model/AnomalyDetection')

// Using express server.
const app = express()

// Encoding.
app.use(
    express.urlencoded({
        extended: false,
    })
)

// To allow the user upload files.
app.use(fileUpload())

// Set root as ../View
app.use(express.static('../View'))

// GET retrieves resources
app.get('/', (req, res) => {
    res.sendFile('./index.html')
})

// POST submits new data to the server.
//    * input algo:  HYBRID/REG, CSV file, another file
//    * output JSON: get JSON file from Model to View
app.post('/detect', (req, res) => {

    // Check if files and algorithm are valid.
    let algorithm = req.body.algorithm
    if (!req.files || algorithm === 'None') {
        res.end()
        return;
    }

    res.write('Find anomalies between the following files: \n')
    res.write(' 1) Train file:  ' + req.files.training_file.name + '\n')
    res.write(' 2) Test file:   ' + req.files.testing_file.name + '\n\n')

    // Get binary data from user's files.
    let trainData = req.files.training_file.data
    let testData = req.files.testing_file.data

    // Convert user's choice for model function.
    let choice = 0
    if (req.body.algorithm === 'Circular Algorithm') {
        choice = 1
    } else if (req.body.algorithm === 'Hybrid Algorithm') {
        choice = 2
    }

    // Convert user's threshold for model function.
    let threshold = req.body.new_threshold

    // Tell the user that the process has began.
    res.write('Run ' + algorithm + ' with threshold of ' + threshold + '.\n')

    // Run algo.
    model.detectAnomalies(trainData, testData, choice, threshold, (result) => {
        res.write(result)
        res.end()
    })

})

app.listen(8080)