#include"test.h"

filter_type filter(filter_type effective, filter_type newe, filter_type max);

RotatedRect& adjust(cv::RotatedRect& rec, const int mode);

 
 
int main(){

    VideoCapture cap("/home/liuheming/桌面/opencv/build/zimiao/path_to_your_video1.avi");
    clock_t start ,finsh;
    double yanchi;
    while (true) {
        start=clock();
            Mat frame;
          cap >> frame; 

        if (frame.empty()) {
        cout<<"Over"<<endl;
        return -1;
        } 
        Mat frameH;

        int stateNum = 4;
    int measureNum = 2;
    KalmanFilter KF(stateNum, measureNum, 0);

    Mat mea = Mat::zeros(measureNum, 1, CV_32F);
    KF.transitionMatrix = (Mat_<float>(stateNum, stateNum) << 1, 0, 1, 0,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1);
    
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(KF.errorCovPost, Scalar::all(1));
    randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));//卡尔曼滤波器，用来预测点的轨迹

    vector<Mat> hsvS;
    cvtColor(frame, frameH, COLOR_BGR2HSV);
    Mat hsv_img;
    hsv_img=frameH;
    split(frameH, hsvS);

    merge(hsvS, hsv_img);
    Mat thresHold;
    threshold(hsvS[2],thresHold,240,255,THRESH_BINARY);
    blur(thresHold, thresHold, Size(3,3));
    Mat element = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
    dilate(thresHold, element, element);
    vector<RotatedRect> v;
    vector<RotatedRect> R;
    vector<vector<Point>> Contour;
    findContours(element.clone(), Contour, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    for (unsigned int i = 0; i < Contour.size();i++)
    {
        double Contour_Area = contourArea(Contour[i]);

        if (Contour_Area < 15 || Contour[i].size() <= 10)
            continue;
        RotatedRect Light_Rec = fitEllipse(Contour[i]);
        Light_Rec = adjust(Light_Rec, ANGLE);
    
        if (Light_Rec.angle > 10)
            continue;
        if (Light_Rec.size.width / Light_Rec.size.height > 1.8
                ||Contour_Area / Light_Rec.size.area() < 0.68)
            continue;
        Light_Rec. size.height *= 1.1;
        Light_Rec.size.width *= 1.1;
        v.push_back(Light_Rec);
    }
    for (size_t i = 0; i < v.size(); i++)
    {
        for (size_t j = i + 1; (j < v.size()); j++)
        {     RotatedRect AR;
          
            AR.center.x = (v[i].center.x + v[j].center.x) / 2.; 
            AR.center.y = (v[i].center.y + v[j].center.y) / 2.; 

            float Contour_angle = abs(v[i].angle - v[j].angle); 
            if (Contour_angle >7)
                continue;
      
          

         
            float Contour_Len1 = abs(v[i].size.height - v[j].size.height) / max(v[i].size.height, v[j].size.height);
            float Contour_Len2 = abs(v[i].size.width - v[j].size.width) / max(v[i].size.width, v[j].size.width);
            if (Contour_Len1 > 0.25 || Contour_Len2 > 0.25)
                continue;
            AR.angle = (v[i].angle + v[j].angle) / 2.; 
            
            float h, w, yD, xD;
            h = (v[i].size.height + v[j].size.height) / 2; 
            w = sqrt((v[i].center.x - v[j].center.x) * (v[i].center.x - v[j].center.x) + (v[i].center.y - v[j].center.y) * (v[i].center.y - v[j].center.y));
            float ra = w / h; 
            xD = abs(v[i].center.x - v[j].center.x) / h; 
            yD = abs(v[i].center.y - v[j].center.y) / h; 
            if (ra <0.5 ||ra>5.0|| xD < 0.5 || yD> 2.0)
                continue;
            AR.size.height = h;
            AR.size.width = w;
            R.push_back(AR);
            Point2f point1;
            Point2f point2;
            point1.x=v[i].center.x;point1.y=v[i].center.y+30;
            point2.x=v[j].center.x;point2.y=v[j].center.y-30;
            int xm = (point1.x+point2.x)/2;
            int ym = (point1.y+point2.y)/2;
       
            AR.center.x = filter(AR.center.x,xm, DELAT_MAX);
            AR.center.y = filter(AR.center.y,ym, DELAT_MAX);
                   Mat prediction = KF.predict();
            Point predict_pt = Point((int)prediction.at<float>(0), (int)prediction.at<float>(1));

            mea.at<float>(0) = (float)AR.center.x;
            mea.at<float>(1) = (float)AR.center.y;
            KF.correct(mea);

            Scalar color(100, 100, 55);
                  
            rectangle(frame,Point(AR.center.x-30,AR.center.y-30),Point(AR.center.x+50,AR.center.y+50),color,2);
         
           
        }
    }
    finsh=clock();
    yanchi=(double)(finsh-start)/CLOCKS_PER_SEC;
    cout<<yanchi<<"秒!"<<endl;

    imshow("Ceshi", frame);
    waitKey(5);
    }

}
