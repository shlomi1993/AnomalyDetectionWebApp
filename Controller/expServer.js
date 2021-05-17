const express = require("express")
const fileupload = require("express-fileupload")
const model = require("../model/AnomalyDetection")

const app = express()

app.use(
  express.urlencoded({
    extended: false,
  })
)
app.use(fileupload())
// Determine root as ../view
app.use(express.static("../view"))

//GET retrieves resources
app.get("/", (req, res) => {
  res.sendFile("./index.html")
})

//POST submits new data to the server.
//input algo - HYBRID/REG, CSV file, another file
//output JSON - get JSON file from Model to View
app.post("/detect", (req, res) => {
  var algorithm = req.body.algorithm
  res.write("Algorithm type: " + algorithm + "\n")
  //Check if files and algorithm are valid
  if(req.files && algorithm != "None") {
    var trainFile = req.files.training_file
    var testFile = req.files.testing_file
    let choice = 1
    //Convert user choice for model function
    if(algorithm == "Hybrid Algorithm") {
      res.write("USER CHOSE HYBRID\n")
      choice = 2
    }
    var result = model.detectAnomalies(trainFile, testFile, choice, 0.9)
    res.write(result.stringify())
  }
  res.end()
})

/*
//PUT updates existing data.
app.put("", (req, res) => {
  //CHECK IF NEED TO DO
});

//DELETE removes data.
app.delete("", (req, res) => {
  //CHECK IF NEED TO DO
});
*/

app.listen(8080)

