#include "extraction.h"

void extractCM() {
	// load image
	IplImage* image = cvLoadImage("spr.png");  
  
	// rgb to hsv
	IplImage* hsv = cvCreateImage(cvGetSize(image), 8, 3);  
	cvCvtColor(image, hsv, CV_BGR2HSV); 

	// split hsv into three channels - h, s, v  
	IplImage* h=cvCreateImage(cvGetSize(hsv), 8, 1);    
	IplImage* s=cvCreateImage(cvGetSize(hsv), 8, 1);    
	IplImage* v=cvCreateImage(cvGetSize(hsv), 8, 1);
	cvSplit(hsv, h, s, v, NULL);
	
	// mean and std, as is, first moment and secondary moment, linear relation with mutiple
	CvScalar h_cs, h_std, s_cs, s_std,  v_cs, v_std;
	cvAvgSdv(h, &h_cs, &h_std);
	cvAvgSdv(s, &s_cs, &s_std);
	cvAvgSdv(v, &v_cs, &v_std);
	cout << "mean:" << endl;
	cout<<h_cs.val[0]/180.0 << endl;
	cout<<s_cs.val[0]/255.0 << endl;
	cout<<v_cs.val[0]/255.0 << endl;

	cout << "std:" << endl;
	cout<<h_std.val[0]/180.0 << endl;
	cout<<s_std.val[0]/255.0 << endl;
	cout<<v_std.val[0]/255.0 << endl;

	// skewness, not third moment, no linear relation with mutiple
	double h_skew, s_skew, v_skew;
	double h_sum = 0.0;
	double s_sum = 0.0;
	double v_sum = 0.0;
	uchar* h_data = (uchar *)h->imageData;
	uchar* s_data = (uchar *)s->imageData;
	uchar* v_data = (uchar *)v->imageData;
	int height = h->height;
	int width = h->width;
	int size = height*width; 
	for(int i = 0; i < height; i++) 
		for(int j = 0; j < width; j++) {
		h_sum += pow(h_data[i*width + j] - h_cs.val[0], 3.0);	
		s_sum += pow(s_data[i*width + j] - s_cs.val[0], 3.0);
		v_sum += pow(v_data[i*width + j] - v_cs.val[0], 3.0);	
	}
	h_skew = double(size)/(size - 1)/(size - 2)*h_sum/pow(h_std.val[0], 3);
	s_skew = double(size)/(size - 1)/(size - 2)*s_sum/pow(s_std.val[0], 3);
	v_skew = double(size)/(size - 1)/(size - 2)*v_sum/pow(v_std.val[0], 3);
	cout << "skewness:" << endl;
	cout << h_skew << endl;
	cout << s_skew << endl;
	cout << v_skew << endl;

	cvReleaseImage(&image); 
	cvReleaseImage(&hsv);
	cvReleaseImage(&h); 
	cvReleaseImage(&s); 
	cvReleaseImage(&v); 
}
