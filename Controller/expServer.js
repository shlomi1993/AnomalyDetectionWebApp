// Requirements.
var path = require('path')
const express = require('express')
const fileUpload = require('express-fileupload')
const model = require('../Model/AnomalyDetection')
const { json } = require('express')

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
    if (!req.files.training_file || !req.files.testing_file || algorithm === 'None') {
        res.write('Missing files or algotihrm.\n')
        res.end()
        return;
    }

    // Extract files' names and extensions.
    let trainName = req.files.training_file.name
    let testName = req.files.testing_file.name
    let ext1 = path.extname(trainName).toLowerCase()
    let ext2 = path.extname(testName).toLowerCase()

    // Verify files' extension validity.
    if (ext1 !== '.csv' || ext2 !== '.csv') {
        res.write('Invalid file type.\n')
        res.end()
        return;
    }

    // Describe the ongoing process for the user.
    res.write('Find anomalies between the following files: \n')
    res.write(' 1) Train file:  ' + trainName + '\n')
    res.write(' 2) Test file:   ' + testName + '\n\n')

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
    res.write('Run ' + algorithm + ' with threshold of ' + threshold + '.\n\n')

    // Run algo.
    model.detectAnomalies(trainData, testData, choice, threshold, (result) => {
        var jsonData = JSON.parse(result)
        if(jsonData[0].ID === "-1") {
            res.write(jsonData[0].ID + "," + jsonData[0].Error)
        }
        else {
            for(var i = 0; i < jsonData.length; i++) {
                res.write("ID: " + jsonData[i].ID + "\n")
                res.write("startTimeStep: " + jsonData[i].startTimeStep + "\n")
                res.write("endTimeStep: " + jsonData[i].endTimeStep + "\n")
                res.write("property1: " + jsonData[i].property1 + "\n")
                res.write("property2: " + jsonData[i].property2 + "\n\n")
            }
        }
        // res.write(result)
        res.end()
    })

})

app.listen(8080)