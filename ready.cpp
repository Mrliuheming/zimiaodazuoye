
#include"test.h"
filter_type filter(filter_type effective, filter_type newe, filter_type max)////简单滤波器，用来限制范围
{
    if ( ( newe- effective > max ) || ( effective-newe> max ))
    {
        newe=effective;
        return effective;
    }
    else
    {
        
        return newe;
    }
}
RotatedRect& adjust(cv::RotatedRect& rec, const int mode)//预处理
   {
       using std::swap;

       float& width = rec.size.width;
       float& height = rec.size.height;
       float& angle = rec.angle;

       if (mode == WIDTH)
       {
           if (width < height)
           {
               swap(width, height);
               angle += 90.0;
           }
       }

       while (angle >= 90.0) angle -= 180.0;
       while (angle < -90.0) angle += 180.0;

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



