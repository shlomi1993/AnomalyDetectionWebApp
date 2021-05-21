# Anomaly Detection Web App

Welcome to our web app.

## Description

We were asked to create application that will be able to detect anomalies.
The application displays a form and the user should provide the following parameters:

1. Training csv file
2. Testing csv file
3. Algorithm type
4. Threshold

By given these parameters and sending them by submission, the application will return anomalies report.

The application supports MVC architecture:

1. View - "index.html" that design how the app should looks like
2. Control - "expServer.js" that is our API and includes the requested http requests: GET and POST
3. Model - "Model" folder - our server that by calling to some parameters, it returns the response to our API (controller)

## Instructions

1. Open the terminal and run the command:
2. Open the browser and connect to the server "localhost:8080"
3. Fill the form and wait for response

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
