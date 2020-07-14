#include <iostream>
#include <chrono>
using namespace std;


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main ( int argc, char** argv )
{
    // 1.读取argv[1]指定的图像
    cv::Mat image;
    image = cv::imread ( "/home/wxdl09/Openframe/FD/imageBasics-20200713/shizhe.pgm" ); //cv::imread函数读取指定路径下的图像
    // 判断图像文件是否正确读取
    if ( image.data == nullptr ) //数据不存在,可能是文件不存在
    {
        cerr<<"文件"<<argv[1]<<"不存在."<<endl;
        return 0;
    }
    
    // 2.文件顺利读取, 首先输出一些基本信息，图像尺度信息
    cout<<"原图像宽为"<<image.cols<<",高为"<<image.rows<<",通道数为"<<image.channels()<<endl;
    cv::imshow ( "image", image );      // 用cv::imshow显示图像
    cv::waitKey ( 0 );                  // 暂停程序,等待一个按键输入

    //3.灰度图转换,默认的色彩空间是BGR
    cv::Mat srcGray;
    cvtColor(image, srcGray, CV_BGR2GRAY);
    imshow("srcGray", srcGray);
    cout<<"灰度图像宽为"<<srcGray.cols<<",高为"<<srcGray.rows<<",通道数为"<<srcGray.channels()<<endl;
    cv::waitKey ( 0 );                  // 暂停程序,等待一个按键输入

    //4.对灰度图像进行高斯滤波
    cv::Mat Gaus3_image;
    GaussianBlur(srcGray,Gaus3_image,Size(3,3),2);
    imshow("Gaus3_image",Gaus3_image);
    cout<<"高斯滤波图像宽为"<<Gaus3_image.cols<<",高为"<<Gaus3_image.rows<<",通道数为"<<Gaus3_image.channels()<<endl;
    cv::waitKey ( 0 );                  // 暂停程序,等待一个按键输入

    //5.canny算子检测边缘
    cv::Mat Can_image;
    std::vector<Vec4i> lines; //定义数组获取直线
    cv::Canny(Gaus3_image,Can_image,200,350);
    imshow("srcGray_line",Can_image);
    cout<<"canny算子提线图像宽为"<<Can_image.cols<<",高为"<<Can_image.rows<<",通道数为"<<Can_image.channels()<<endl;
    cv::waitKey ( 0 );

    // 检测后图像恢复3通道，并显示在原图像中
    cv::Mat dst,image1;
    dst = Scalar::all(0);
    image1 = image.clone();
    image1.copyTo(dst,Can_image);
    imshow("image_line",dst);
    cv::waitKey ( 0 );

    cv::Mat image2;
    image2 = image-dst;
    imshow("image2",image2);
    cv::waitKey ( 0 );

    // 遍历图片，阈值判断
    for ( int i = 0; i < image2.rows; i++ )
    {
        for ( int j=0; j<image2.cols; j++ )
        {
            if(image2.at<Vec3b>(i,j)[0]==image2.at<Vec3b>(i,j)[1] && image2.at<Vec3b>(i,j)[0]==image2.at<Vec3b>(i,j)[2])
            {
                if(image2.at<Vec3b>(i,j)[0]>=0 && image2.at<Vec3b>(i,j)[0]<=55)
                    image2.at<Vec3b>(i,j)[0]=image2.at<Vec3b>(i,j)[1]=image2.at<Vec3b>(i,j)[2]=0;
                else if(image2.at<Vec3b>(i,j)[0]>55 && image2.at<Vec3b>(i,j)[0]<170)
                    image2.at<Vec3b>(i,j)[0]=image2.at<Vec3b>(i,j)[1]=image2.at<Vec3b>(i,j)[2]=128;
                else
                    image2.at<Vec3b>(i,j)[0]=image2.at<Vec3b>(i,j)[1]=image2.at<Vec3b>(i,j)[2];
            }
        }
    }
    imshow("image3",image2);
    cout<<"imag2_f"<<Can_image.cols<<endl;
    cv::waitKey ( 0 );



    cv::destroyAllWindows();
    return 0;
}
