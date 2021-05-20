// AnomalyDetectionAPI.cpp
// Author: Shlomi Ben-Shushan ID: 311408264

// Include node.h library and all relevant cpp files.
#include <node.h>
#include "AnomalyDetectionServer2/src/main.cpp"
#include "AnomalyDetectionServer2/src/anomaly_detection_util.cpp"
#include "AnomalyDetectionServer2/src/timeseries.cpp"
#include "AnomalyDetectionServer2/src/SimpleAnomalyDetector.cpp"
#include "AnomalyDetectionServer2/src/minCircle.cpp"
#include "AnomalyDetectionServer2/src/HybridAnomalyDetector.cpp"

// Creating a namespace.
 namespace AnomalyDetectionNamespace {

    // Use all relevats V8's components for Node.js use.
    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Value;
    using v8::Array;
    using v8::Exception;
    using namespace std;

    // This functions parse args from Node.js and call linearAnomalyDetection() function.
    void linearAnomalyDetectionCaller(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        String::Utf8Value str(isolate, args[0]);
        string cppStr(*str);
        float t = stof(cppStr);
        linearAnomalyDetection(t);
    }

    // This functions parse args from Node.js and call hybridAnomalyDetection() function.
    void hybridAnomalyDetectionCaller(const FunctionCallbackInfo<Value>&args) {
        Isolate* isolate = args.GetIsolate();
        String::Utf8Value str(isolate, args[0]);
        string cppStr(*str);
        float t = stof(cppStr);
        hybridAnomalyDetection(t);
    }

    // Initialize functions to use them in Node.js.
    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "linearAnomalyDetection", linearAnomalyDetectionCaller);
        NODE_SET_METHOD(exports, "hybridAnomalyDetection", hybridAnomalyDetectionCaller);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);

 }  