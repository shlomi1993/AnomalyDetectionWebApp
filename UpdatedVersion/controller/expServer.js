const express = require("express");
const fileupload = require("express-fileupload");
const model = require("../model/modelName"); // according to model name

const app = express();

app.use(
  express.urlencoded({
    extended: false,
  })
);
app.use(fileupload());
app.use(express.static("../view"));

app.get("/", (req, res) => {
  res.sendFile("./index.html");
});

app.post("/detect", (req, res) => {
  res.write("In progress\n");
  if (req.files) {
    var algorithm;
    if (req.algorithm) {
      algorithm = req.algorithm.value; // need to check if syntax is ok (.value)
    }
    var trainingFile = req.files.training_file;
    var testingFile = req.files.testing_file;
    var result = model.methodName(trainingFile, testingFile, algorithm); // according to the functions in the model
    res.write(result.stringify()); // need to check how the results should be dispalyed
  }
  res.end();
});

app.listen(8080);

/*
app.get()
app.post()
app.delete() 
*/
