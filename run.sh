# To run the server, please make sure the following
# components are installed on your system:
# 	node v10.19.0 or higher version (v16.0.0 recommended)
# 	npm v6.14.4 or higher version (v7.10.0 recommended)
# 	node-gyp v6.1.0 or higher version (v8.0.0 recommended)

cd ./Model
npm install
node-gyp configure
node-gyp build
cd ../Controller
npm install
node expServer.js
