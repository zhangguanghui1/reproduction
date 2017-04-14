#include "tools.hpp"
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string image_name;
    if(argc < 2)
        image_name = "test1.bmp";
    else
        image_name = argv[1];

    cv::Mat img = cv::imread(image_name, 1);
    cv::Mat_<uchar> dark_channel;
    calcDarkChannel(img, dark_channel);

    cv::Vec3b A;
    estimateAtmosphericLight(img, dark_channel, A);

    cv::Mat_<float> t, t_refine; cv::Mat t_show;
    double min_value, max_value;
    initTransMap(img, A, t, 10);
    cv::Mat tMat = t;
    cv::minMaxIdx(t, &min_value, &max_value);
    tMat.convertTo(t_show, CV_8U, 255.0/(max_value-min_value));//, 255.0*min_value/(max_value- min_value));
    
    softMatting(img, t, t_refine);
    
    cv::Mat_<cv::Vec3b> recoverImg;
    recoverSceneRadiance(img, recoverImg, t_refine, A);
    //int size[] = {3, 3};
    //cv::SparseMat_<float> CoeffMat(2, size);
    //CoeffMat.ref(0, 0) = 4; CoeffMat.ref(0, 1) = 3;
    //CoeffMat.ref(1, 0) = 3; CoeffMat.ref(1, 1) = 4; CoeffMat.ref(1, 2) = -1;
    //CoeffMat.ref(2, 1) = -1; CoeffMat.ref(2, 2) = 4;

    //cv::Mat_<float> b(3, 1);
    //b(0, 0) = 24; b(1, 0) = 30; b(2, 0) = -24;
    //cv::Mat_<float> X;
    //linearEquationSolver(CoeffMat, b, X);

    //std::cout << "X = " << X << std::endl;

    cv::namedWindow("source img");
    cv::imshow("source img", img);
    cv::namedWindow("dark_channel");
    cv::imshow("dark_channel", dark_channel);
    cv::namedWindow("transmission");
    cv::imshow("transmission", t_show);
    cv::namedWindow("recoverImg");
    cv::imshow("recoverImg", recoverImg);
    cv::waitKey();

}
