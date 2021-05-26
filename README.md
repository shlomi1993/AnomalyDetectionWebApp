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
 	2. node v10.19.0 or higher version (v16.0.0 recommended)
 	3. npm v6.14.4 or higher version (v7.10.0 recommended)
 	4. node-gyp v6.1.0 or higher version (v8.0.0 recommended)

## Instructions

1. Please make sure your system meet the requirements.
2. Open the terminal in the server's main dictionary the and run the command: ./run.sh  
3. The sh script will run npm install in the relevant diretories and compile parts of the program using node-gyp.  
4. After server's setup is done, you can work in two ways:
   a. Using WEB interface:
      1) Open the browser and connect to the server at "localhost:8080".  
      2) Fill the form and submit.
   b. Using remote access:
      1) Send a POST request to "localhost:8080/detect" with the following data in the body:  
         a) algorithm: "Regression Algorithm" or "Circular Algorithm" or "Hybrid Algorithm".  
         b) new_threshold: a fraction between 0 and 1.  
         c) training_file: a CSV file for system training.  
         d) testing_file: a CSV file for anomaly detection.  
      2) You will get a JSON response.

## Troubleshooting

1. If you receive some errors you can file to enter the following commands:  
   cd controller  
   npm install node-gyp  
   cd ..  
   cd Model  
   node-gyp configure  
   node-gyp build  
   cd ..  
   cd controller  
   node expServer.js  
