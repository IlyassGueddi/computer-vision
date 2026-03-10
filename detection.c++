#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;

int main() {
    // Optimization 
    Net net = readNetFromDarknet("yolov3.cfg", "yolov3.weights");
    net.setPreferableBackend(DNN_BACKEND_CUDA); 
    net.setPreferableTarget(DNN_TARGET_CUDA);

    // Measuring Performance 
    double t = (double)getTickCount();
    
    
    t = ((double)getTickCount() - t) / getTickFrequency();
    printf("[STATS] Inference Time: %.3f sec | Hardware: RTX 4060\n", t);

    return 0;
}