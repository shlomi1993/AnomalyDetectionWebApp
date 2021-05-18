// Requirements.
const fs = require('fs')
const express = require('express')
const fileupload = require('express-fileupload')
const detectAnomalies = require('../Model/AnomalyDetection')

// Set consts.
let trainName = './trainFile.csv'
let testName = './testFile.csv'

// Using express server.
const app = express()

// Encoding.
app.use(
    express.urlencoded({
        extended: false,
    })
)

// To allow the user upload files.
app.use(fileupload())

// Set root as ../View
app.use(express.static('../View'))

// GET retrieves resources
app.get('/', (req, res) => {
    console.log('GET')
    res.sendFile('./index.html')
})

// POST submits new data to the server.
//    * input algo:  HYBRID/REG, CSV file, another file
//    * output JSON: get JSON file from Model to View
app.post('/detect', (req, res) => {

    console.log('POST')

    return new Promise((resolve, reject) => {

        console.log('New promise')

        // Check if files and algorithm are valid.
        let algorithm = req.body.algorithm
        if (!req.files || algorithm === 'None') {
            res.end()
            reject('No files or algo.')
            return;
        }
        resolve('Success');

    })

    // Save 1st file locally.
    .then(() => {
        console.log('Write file 1')
        fs.writeFileSync(trainName, req.files.training_file.data)
    })

    // Save 2nd file locally.
    .then(() => {
        console.log('Write file 2')
        fs.writeFileSync(testName, req.files.testing_file.data)
    })


    .then(() => {

        // Convert user's choice for model function.
        let choice = 0
        if (req.body.algorithm == 'Hybrid Algorithm') {
            choice = 1
        }

        // Convert user's threshold for model function.
        let threshold = 0.9 // FIX THRESHOLD

        console.log('Run algo with:')
        console.log("  1) " + trainName)
        console.log("  2) " + testName)
        console.log("  3) " + choice)
        console.log("  4) " + threshold)

        detectAnomalies(trainName, testName, choice, threshold)
    })

    .then(() => {
        res.end()
    })

    .catch((e) => {
        console.log(e);
    });

})

app.listen(8080)
console.log('Listening...')

/*
To-Do:
* Need to implement PUT?
* Need to implement DELETE?
* Remove 'console.log()'
* Hande ASYNC!!!
* Connect to View
*/