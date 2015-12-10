#include <iostream>
#include "highgui.h"  
#include "cv.h" 
#include "math.h"
#include <vector>

using namespace std;
using namespace cv;

extern "C" {
	void extractCM();
	vector<double> getCM(IplImage* hsv);
	double getSkewness(IplImage* img, CvScalar mean, CvScalar std);
}

