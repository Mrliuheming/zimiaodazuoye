
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
   vector<Point3f> Points3D;

    Points3D.push_back(Point3f(-67,-27.5,0));
    Points3D.push_back(Point3f(-67,27.5,0));
    Points3D.push_back(Point3f(67,-27.5,0));
    Points3D.push_back(Point3f(67,27.5,0));
     vector<Point2f> Points2D;
 int enemy_color;
      enemy_color = REDENEMY;
     Mat cameraMatrix=(Mat_<float>(3,3) <<2337.174430, 0.000000, 746.042769,
    0.000000 ,2334.605162 ,561.369316,
    0.000000 ,0.000000 ,1.000000);
    Mat dist=(Mat_<float>(1,5) <<-0.10632024648023272, 0.10306047267202897, -0.0005522032223838761, 0.0015989186062307333, 0.0);
    vector<Mat> hsvS,channels;
    cvtColor(frame, frameH, COLOR_BGR2HSV);
    split(frameH, hsvS);
    merge(hsvS, frameH);
    Mat dstmg,gray_img;
    cvtColor(frameH,dstmg,COLOR_BGR2HSV);
    split(dstmg,channels);
        if (!enemy_color) {                              
        gray_img = channels.at(0) - channels.at(2);
        }
        else 
        gray_img = channels.at(2) - channels.at(0);

    Mat thresHold;
    threshold(hsvS[2],thresHold,250,255,THRESH_BINARY);
   
    Mat element = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
  
    dilate(thresHold, element, element);
    vector<RotatedRect> v;
    vector<RotatedRect> R;
    vector<vector<Point>> Contour;
    findContours(element.clone(), Contour, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    for (unsigned int i = 0; i < Contour.size();i++)
    {
        double Contour_Area = contourArea(Contour[i]);

        if (Contour_Area < 5 || Contour[i].size() <= 1)
            continue;
        RotatedRect Light_Rec = fitEllipse(Contour[i]);
        Light_Rec = adjust(Light_Rec, ANGLE);
        if (Light_Rec.size.width / Light_Rec.size.height > 0.8
         
              )
     
            continue;
          
       
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
           if(AR.angle>-90&&AR.angle<-60) continue;            float h, w, yD, xD;
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


          Mat rvec,tvec;
          rvec=Mat::zeros(3,1,CV_64F);
        tvec=Mat::zeros(3,1,CV_64F);
          
         
          Point2f point[4];
           AR.points(point);
            for(int i=0;i<4;i++)
            {
                line(frame,point[i],point[(i+1)%4],Scalar(255,0,0));

                 Points2D.push_back(point[i]);
                
            }

     solvePnP(Points3D,Points2D,cameraMatrix,dist,rvec,tvec,false,SOLVEPNP_ITERATIVE);
         cout<<rvec<<endl<<endl<<tvec;
         Points2D.clear();
 
        }
    }
    finsh=clock();
    yanchi=(double)(finsh-start)/CLOCKS_PER_SEC;
    cout<<yanchi<<"秒!"<<endl;

    imshow("Ceshi", frame);
    waitKey(1);
    }

}



