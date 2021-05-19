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
    console.log('GET')
    res.sendFile('./index.html')
})

// POST submits new data to the server.
//    * input algo:  HYBRID/REG, CSV file, another file
//    * output JSON: get JSON file from Model to View
app.post('/detect', (req, res) => {

    console.log('POST')

    // Check if files and algorithm are valid.
    let algorithm = req.body.algorithm
    if (!req.files || algorithm === 'None') {
        res.end()
        return;
    }

    res.write('Looking for anomalies between ' + req.files.training_file.name + ' and ' + req.files.testing_file.name + '.')

    // Get binary data from user's files.
    let trainData = req.files.training_file.data
    let testData = req.files.testing_file.data

    // Convert user's choice for model function.
    let choice = 0
    if (req.body.algorithm === 'Hybrid Algorithm') {
        choice = 1
    }

    // Convert user's threshold for model function.
    let threshold = 0.9 // FIX THRESHOLD

    console.log('Run algo number ' + choice + ' with threshold ' + threshold + '.')

    // var p = new Promise((resolve, reject) => {
    //     var t = model.detectAnomalies(trainData, testData, choice, threshold)
    //     if (!t) reject('Failed')

    //     resolve(t)
    // }).then((t) => {
    //     res.end()
    // })

    var t = model.detectAnomalies(trainData, testData, choice, threshold)
    console.log(t)

    res.write(' SOMETHING')



    console.log('POST END')

    res.end()


})

app.listen(8080)
console.log('Listening...')

/*
To-Do:
* Need to implement PUT?
* Need to implement DELETE?
* Fix threshold.
* Remove 'console.log()'
* Hande ASYNC!!!
* Connect to View
*/