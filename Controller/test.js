const detectAnomalies = require('../Model/AnomalyDetection');

detectAnomalies("../Model/reg_flight.csv", "../Model/anomaly_flight.csv", 0, 0.9)