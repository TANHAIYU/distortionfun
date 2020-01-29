#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void distortImg(const Mat &srcImg, Mat &dstImg, const float fx, const float fy, const float cx, const float cy)
{
    int imgHeight=srcImg.rows;
    int imgWidth=srcImg.cols;

    float disK=-1;

    uchar* pSrcData=(uchar*)srcImg.data;
    uchar* pDstData=(uchar*)dstImg.data;
    for (int j=0; j<imgHeight; j++)
    {
        for (int i=0; i<imgWidth; i++)
        {



            //转到摄像机坐标系
            float X=(i-cx)/fx;
            float Y=(j-cy)/fy;
            float r2=X*X+Y*Y;
            //加上畸变
            float newX=X*(1+disK*r2);
            float newY=Y*(1+disK*r2);
            //再转到图像坐标系
            float u=newX*fx+cx;
            float v=newY*fy+cy;


            //双线性插值
            int u0=static_cast<int>(floor(u));
            int v0=static_cast<int>(floor(v));
            int u1=u0+1;
            int v1=v0+1;

            float dx=u-u0;
            float dy=v-v0;
            float weight1=(1-dx)*(1-dy);
            float weight2=dx*(1-dy);
            float weight3=(1-dx)*dy;
            float weight4=dx*dy;

            int resultIdx=j*imgWidth*3+i*3;
            if (u0>=0 && u1<imgWidth && v0>=0 && v1<imgHeight)
            {
                 pDstData[resultIdx+0]=weight1*pSrcData[v0*imgWidth*3 + u0*3+0]+weight2*pSrcData[v0*imgWidth*3+u1*3+0]
                    +weight3*pSrcData[v1*imgWidth*3 +u0*3+0]+weight4*pSrcData[v1*imgWidth*3 + u1*3+0];
                 pDstData[resultIdx+1]=weight1*pSrcData[v0*imgWidth*3 + u0*3+1]+weight2*pSrcData[v0*imgWidth*3+u1*3+1]
                    +weight3*pSrcData[v1*imgWidth*3 +u0*3+1]+weight4*pSrcData[v1*imgWidth*3 + u1*3+1];
                 pDstData[resultIdx+2]=weight1*pSrcData[v0*imgWidth*3 + u0*3+2]+weight2*pSrcData[v0*imgWidth*3+u1*3+2]
                    +weight3*pSrcData[v1*imgWidth*3 +u0*3+2]+weight4*pSrcData[v1*imgWidth*3 + u1*3+2];
            }
        }
    }
}

int main()
{
    string imgPath="/home/haiyutan/master-thesis/distortionfun/";
    /// Test image
    Mat srcImg = imread(imgPath+"000000.png");
    if( !srcImg.data )
      {
      printf(" No data! -- Exiting the program \n");
        return -1;
    }
    else{
      cout<< "The size of input image:" <<srcImg.size <<endl;
    }


    //pyrDown(srcImg, srcImg);
    //pyrDown(srcImg, srcImg);
    Mat dstImg = srcImg.clone();
    dstImg.setTo(0);
    cout<< "The size of input image2:" <<srcImg.size <<endl;

    namedWindow("showImg",0);
    imshow("showImg", srcImg);
    waitKey(0);


    //scale parameters
    float scale = 1;
    float fx=static_cast<float>(718.856*scale);
    float fy=static_cast<float>(718.856*scale);
    float cx=static_cast<float>(607.1928);
    float cy=static_cast<float>(185.2157);


    distortImg(srcImg, dstImg, fx, fy, cx, cy);
    imshow("showImg", dstImg);
    cout << "The size of output image: " << dstImg.size<<endl;

    //pyrDown(dstImg, dstImg,Size(dstImg.cols/2,srcImg.rows/2));
    //pyrDown(srcImg, srcImg,Size(srcImg.cols/2,srcImg.rows/2));   //here can specify the wanted size of images by '''pyrDown( tmp, dst, Size( tmp.cols/2, tmp.rows/2 ) )'''

    imwrite( imgPath+"moon_000000.png", dstImg);
    waitKey(0);

    return 0;
}
