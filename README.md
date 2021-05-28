# Anomaly Detection Web App

## Description

We were asked to create application that will be able to detect anomalies.
The application displays a form and the user should provide the following parameters:

1. Training csv file
2. Testing csv file
3. Algorithm type
4. Threshold

By given these parameters and sending them by submission, the application will return anomalies report.

The application supports MVC architecture:

1. View - "index.html" that design how the app should looks like, "result.html" that displays the form's output  
2. Control - "expServer.js" that is our API and includes the requested http requests: GET and POST.  
   GET - "/" - the method returns the "index.html" - main view  
   POST - "/Detect" - the method returns response JSON with anomalies records  
   POST - "/Detect/fromView" - we added this function in order to display our app view anomalies report  
4. Model - "Model" folder - our server that by calling to some parameters, it returns the response to our API (controller)  



## Requirements

To run the server, please make sure the following components are installed on your system:
1. Linux based OS or WSL2 for Windows (WSL Ubuntu 20.04 recommended)
2. node v10.19.0 or higher version (v14.17.0 recommended)
3. npm v6.14.4 or higher version (v7.10.0 recommended)
4. node-gyp v6.1.0 or higher version (v8.0.0 recommended)

## pre-installations:

If you need to install one (or more) of the components to meet the requirements, open a linux terminal and use the following commands:
1. It is recommend to update and upgrage the apt-get. To do so, enter the command "sudo apt-get update" and after it's done, enter "sudo apt-get upgrade -y".
2. To install node-js, use the command: "sudo apt-get install nodejs -y".
3. To install npm, use the command: "sudo apt-get install npm -y".
4. To install node-gyp, use the command: "sudo npm install -g node-gyp".

## Instructions

1. Please make sure your system meet the requirements above.
2. Open the terminal in the server's main dictionary the and run the command: "./run.sh". This script will run "npm install" in the relevant diretories to get the required dependencies, and compile C++ part of the program using node-gyp.
3. After server's setup is done, you can work in two ways:

#### Using WEB interface:

Open the browser and connect to the server at "localhost:8080". Then, Fill the form and submit.

#### Using remote access:

Send a POST request to "localhost:8080/detect" with the following data in the body:  
1. algorithm: "Regression Algorithm" or "Circular Algorithm" or "Hybrid Algorithm".
2. new_threshold: a fraction between 0 and 1.
3. training_file: a CSV file for system training.
4. testing_file: a CSV file for anomaly detection.

After making the POST request, you will get a POST response with a JSON.

## Troubleshooting

If you receive some errors with run.sh file, you can enter the following commands:
1. cd ./Model
2. npm install
3. node-gyp configure
4. node-gyp build
5. cd ../Controller
6. npm install
7. node expServer.js 
