# To run the server, please make sure the following
# components are installed on your system:
# 	node v14.17.0or higher version (v16.0.0 recommended)
# 	npm v7.10.0 or higher version
# 	node-gyp v8.0.0 or higher version

cd ./Model
npm install
node-gyp configure
node-gyp build
cd ../Controller
npm install
node expServer.js
echo "Anomaly Detection Server is up!"
