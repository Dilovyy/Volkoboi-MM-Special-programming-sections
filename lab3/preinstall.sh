#!/bin/bash
sudo apt update
sudo apt install -y libopencv-dev cmake g++ make



mkdir -p models
cd models
wget -q https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -q https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

cd ..
wget -O include/json.hpp https://github.com/nlohmann/json/releases/latest/download/json.hpp

echo "preinstall complete succesfullys"
echo "=========================================="
echo ""
echo "Now you can build using:"
echo "  ./build.sh"
