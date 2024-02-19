
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include<time.h>
 
using namespace cv;
using namespace std;

 
const int ANGLE= 1;
const int WIDTH = 0;
 

RotatedRect &adjustRec(cv::RotatedRect &rec, const int mode)
{
    using std::swap;
 
    float &width = rec.size.width;
    float &height = rec.size.height;
    float &angle = rec.angle;
 
    if (mode == WIDTH)
    {
        if (width < height)
        {
            swap(width, height);
            angle += 90.0;
        }
    }
 
    while (angle >= 90.0)
        angle -= 180.0;
    while (angle < -90.0)
        angle += 180.0;
 
    if (mode == ANGLE)
    {
        if (angle >= 45.0)
        {
            swap(width, height);
            angle -= 90.0;
        }
        else if (angle < -45.0)
        {
            swap(width, height);
            angle += 90.0;
        }
    }
    return rec;
} 
 
int main(){

    VideoCapture cap("/home/liuheming/桌面/opencv/build/zimiao/path_to_your_video.mp4");
    
    while (true) {
            Mat frame;
          cap >> frame; 

        if (frame.empty()) {
        cout<<"Over"<<endl;
        return -1;
        } 
        Mat frameH;
    vector<Mat> hsvS;
    cvtColor(frame, frameH, COLOR_BGR2HSV);
    split(frameH, hsvS);
    equalizeHist(hsvS[2], hsvS[2]);
    merge(hsvS, frameH);
    Mat thresHold;
    threshold(hsvS[2], thresHold,240,245,THRESH_BINARY);
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
        Light_Rec = adjustRec(Light_Rec, ANGLE);
        if (Light_Rec.angle > 20 )
            continue;
        if (Light_Rec.size.width / Light_Rec.size.height > 3
                ||Contour_Area / Light_Rec.size.area() < 0.3)
            continue;
        Light_Rec. size.height *= 1.2;
        Light_Rec.size.width *= 1.2;
        v.push_back(Light_Rec);
    }
    for (size_t i = 0; i < v.size(); i++)
    {
        for (size_t j = i + 1; (j < v.size()); j++)
        {
            float Contour_angle = abs(v[i].angle - v[j].angle); 
            if (Contour_angle >= 30)
                continue;
            float Contour_Len1 = abs(v[i].size.height - v[j].size.height) / max(v[i].size.height, v[j].size.height);
            float Contour_Len2 = abs(v[i].size.width - v[j].size.width) / max(v[i].size.width, v[j].size.width);
            if (Contour_Len1 > 0.5 || Contour_Len2 > 0.5)
                continue;
            RotatedRect AR;
            AR.center.x = (v[i].center.x + v[j].center.x) / 2.; 
            AR.center.y = (v[i].center.y + v[j].center.y) / 2.; 
            AR.angle = (v[i].angle + v[j].angle) / 2.; 
            float h, w, yD, xD;
            h = (v[i].size.height + v[j].size.height) / 2; 
            w = sqrt((v[i].center.x - v[j].center.x) * (v[i].center.x - v[j].center.x) + (v[i].center.y - v[j].center.y) * (v[i].center.y - v[j].center.y));
            float ra = w / h; 
            xD = abs(v[i].center.x - v[j].center.x) / h; 
            yD = abs(v[i].center.y - v[j].center.y) / h; 
            if (ra < 1.0 || ra > 5.0 || xD < 0.5 || yD> 2.0)
                continue;
            AR.size.height = h;
            AR.size.width = w;
            R.push_back(AR);
            Point2f point1;
            Point2f point2;
            point1.x=v[i].center.x;point1.y=v[i].center.y+20;
            point2.x=v[j].center.x;point2.y=v[j].center.y-20;
            int xm= (point1.x+point2.x)/2;
            int ym = (point1.y+point2.y)/2;
            Scalar color(100, 100, 55);
            rectangle(frame, point1,point2, color, 2);
           
        }
    }

    imshow("Ceshi", frame);
    waitKey(1);
    }
 cap.release();
}
