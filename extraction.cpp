#include "extraction.h"

extern "C" {

// 81-dimensions color moments features extraction
// 1. divide a image into 9 same rectangle blocks
// 2. each block contains 9-dimensions color moments features
// 3. 9-dim CM features: mean of hsv, std of hsv, skewness of hsv 
// 4. each feature need normalization
void extractCM() {
	// load image
	IplImage* image = cvLoadImage("./image/band.jpg");  
  
	// rgb to hsv
	IplImage* hsv = cvCreateImage(cvGetSize(image), image->depth, 3);  
	cvCvtColor(image, hsv, CV_BGR2HSV); 

	vector<double> CM; // 81-dimensions vector
	int height = hsv->height;
	int width = hsv->width;
	int block_height = height/1;
	int block_width = width/1;
	for(int row = 0; row < 1; row++)
		for(int col = 0; col < 1; col++) {
			// image division
			CvRect rect = cvRect(col*block_width, 
					     row*block_height, 
					     block_width, 
					     block_height); // coordinate of image block
			CvMat *mat = cvCreateMatHeader(block_height, block_width, CV_8UC1); // ceate mat header
			CvMat *hsvMat = cvGetSubRect(hsv, mat, rect); // save image block into matrix
			//IplImage *hsvSubImg = cvCreateImageHeader(cvSize(block_width, block_height), hsv->depth, 3); // create sub image
			IplImage hsvSubImg;
			cvGetImage(hsvMat, &hsvSubImg); // get subimage from mat
			getCM(&hsvSubImg);
			
			// show image
			//cvShowImage("hsvSubImg", &hsvSubImg); 
			//cvWaitKey(0);
			//cvReleaseMat(&mat);
			//cvReleaseMat(&hMat);
			//cvReleaseImage(&hSubImg);  
			//cvDestroyWindow("hsvSubImg");  
	
			// release
			cvReleaseMat(&mat);
			// cvReleaseMat(&hsvMat);
			// cvReleaseImage(&hsvSubImg);
	}
	
	// release
	cvReleaseImage(&image); 
	cvReleaseImage(&hsv);    
}

vector<double> getCM(IplImage* hsv) {
	int height = hsv->height;
	int width = hsv->width;
	int size = height*width;
	vector<double> CM; // 9-dimensions vector

	// split hsv into 3 channels - h, s, v  
	IplImage* h = cvCreateImage(cvGetSize(hsv), 8, 1);    
	IplImage* s = cvCreateImage(cvGetSize(hsv), 8, 1);    
	IplImage* v = cvCreateImage(cvGetSize(hsv), 8, 1);
	cvSplit(hsv, h, s, v, NULL);

	// mean and std, that is, first and secondary moment
	// std takes biased estimate
	// linear relation with mutiple
	CvScalar h_mean, h_std, s_mean, s_std, v_mean, v_std;
	cvAvgSdv(h, &h_mean, &h_std);
	cvAvgSdv(s, &s_mean, &s_std);
	cvAvgSdv(v, &v_mean, &v_std);

	CM.push_back(h_mean.val[0]/180.0);
	CM.push_back(s_mean.val[0]/255.0);
	CM.push_back(v_mean.val[0]/255.0/255.0);
	CM.push_back(h_std.val[0]/180.0);
	CM.push_back(s_std.val[0]/255.0);
	CM.push_back(v_std.val[0]/255.0/255.0);

	// skewness, third central moment
	// treat NaN and Inf as missing values, remove them
	// no linear relation with mutiple
	double h_skew, s_skew, v_skew;
	h_skew = getSkewness(h, h_mean, h_std);
	s_skew = getSkewness(s, s_mean, s_std);
	v_skew = getSkewness(v, v_mean, v_std);
	
	CM.push_back(h_skew);
	CM.push_back(s_skew);
	CM.push_back(v_skew);
	vector<double>::iterator iter;
	for(iter = CM.begin(); iter != CM.end(); iter++)
		cout << *iter << endl;
	
	// release
	cvReleaseImage(&h); 
	cvReleaseImage(&s); 
	cvReleaseImage(&v); 
	
	return CM;
}

double getSkewness(IplImage* img, CvScalar mean, CvScalar std) {
	double skew;
	double sum = 0.0;
	int height = img->height;
	int width = img->width;
	int size = height*width;
	uchar* data = (uchar *)img->imageData;
	for(int i = 0; i < height; i++) 
		for(int j = 0; j < width; j++)
			sum += pow(data[i*width + j] - mean.val[0], 3.0);
	// skew = pow(double(size)*(size - 1), 0.5)/(size - 2)*sum/size/pow(std.val[0]/norm, 3);
	skew = sum/size/pow(std.val[0], 3);
	
	return skew;
}

}
