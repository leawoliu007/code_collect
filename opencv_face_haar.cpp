// Haar特征检测，人脸识别算法，是用xml作为训练后的分类器做的

#include <opencv2\opencv.hpp>

#include <cstdio>

#include <cstdlib>

#include <Windows.h>

using namespace std;

int main()

{

    //加载Haar特征检测分类器

    // haarcascade_frontalface_alt.xml系OpenCV自带的分类器

    //

    // C++中用指针还是非常多的，

    //记住换行符用法，\后面不要有空格

    const char *pstrCascadeFileName =

        "D:\\opencv2.4.9\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";

    CvHaarClassifierCascade *pHaarCascade = NULL; //定义一个哈尔已有的训练过的结果

    //这里的cascade意思是一滴一滴下落---这是我的英语解词，可以忽略。

    pHaarCascade = (CvHaarClassifierCascade *)cvLoad(pstrCascadeFileName); //加载已有的xml文件

    //载入图像，这里又用到指针了

    const char *pstrImageName = "E:\\testpictures\\meizi.jpg";

    IplImage *pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_UNCHANGED); //不加改变的加载原图。

    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1); //创建八位等大小一通道空图像

    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY); //复制图像

    //人脸识别与标记

    if (pHaarCascade != NULL) //如果有xml文件，则往下执行

    {

        // CvScalar：包含四个double成员，可以用来表示B，G，R，alpha----alpha是用来表示图像的透明度

        CvScalar FaceCircleColors[] = //这是一些列颜色，可以看作调色板

            {

                {{0, 0, 255}},

                {{0, 128, 255}},

                {{0, 255, 255}},

                {{0, 255, 0}},

                {{255, 128, 0}},

                {{255, 255, 0}},

                {{255, 0, 0}},

                {{255, 0, 255}}

            };

        //内存存储器是一个可用来存储诸如序列，轮廓，图形, 子划分等动态增长数据结构的底层结构。

        CvMemStorage *pcvMStorage = cvCreateMemStorage(0);

        cvClearMemStorage(pcvMStorage); //初始化内存

        //识别

        DWORD dwTimeBegin, dwTimeEnd; // DWORD 就是 Double Word， 每个word为2个字节的长度

        //在Release版本中，该函数从0开始计时，返回自设备启动后的毫秒数（不含系统暂停时间）。

        //在Debug版本中，设备启动后便从计时器中减去180秒。这样方便测试使用该函数的代码的正确溢出处理。

        dwTimeBegin = GetTickCount();

        //稠密序列都派生自CvSeq，他们用来代表可扩展的一维数组 — 向量、栈、队列和双端队列。

        //稀疏序列派生自CvSet，CvSet也是基于CvSeq的，他们都是由节点所组成，每一个节点要么被占用，那么为空，由标志位flag决定。

        //函数 cvHaarDetectObjects 使用针对某目标物体训练的级联分类器在图像中找到包含目标物体的矩形区域，并且将这些区域作为一序列的矩形框返回。

        CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage); //在这里获得一系列矩形框

        dwTimeEnd = GetTickCount();

        printf("人脸个数： %d 识别用时： %d ms \n", pcvSeqFaces->total, dwTimeEnd - dwTimeBegin); //计算出时间和包含人脸的矩形框的个数

        //标记

        for (int i = 0; i < pcvSeqFaces->total; i++)

        {

            CvRect *r = (CvRect *)cvGetSeqElem(pcvSeqFaces, i); //从矩形框稠密数列中抽取每一个矩形框

            CvPoint center; //定义一个中点

            int radius; //定义一个半径

            center.x = cvRound((r->x + r->width * 0.5)); //获取圆心的中心点x,y坐标

            center.y = cvRound((r->y + r->height * 0.5));

            radius = cvRound((r->width + r->height) * 0.25); //简化计算，本来应该是高的一半平方和宽的一半平方，求根号

            cvCircle(pSrcImage, center, radius, FaceCircleColors[i / 8], 2); //画在图片上画圆圈，涂上颜色，
        }

        cvReleaseMemStorage(&pcvMStorage); //释放内存
    }

    //新建窗口显示图像，和销毁窗口。

    const char *pstrWindowTitle = "人脸识别";

    cvNamedWindow(pstrWindowTitle, CV_WINDOW_AUTOSIZE);

    cvShowImage(pstrWindowTitle, pSrcImage);

    cvWaitKey(0);

    cvDestroyWindow(pstrWindowTitle);

    cvReleaseImage(&pSrcImage);

    cvReleaseImage(&pGrayImage);

    return 0;
}