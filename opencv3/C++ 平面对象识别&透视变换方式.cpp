/*
findHomography()

    函数 findHomography() 找到两个平面之间的透视变换 H。

    参数说明：

    Mat findHomography(

        InputArray srcPoints, //原始平面中点的坐标

        InputArray dstPoints, //目标平面中点的坐标

        int method = 0, //用于计算单应性矩阵的方法

        double ransacReprojThreshold = 3,

        OutputArray mask = noArray(), //通过鲁棒法（RANSAC 或 LMEDS）设置的可选输出掩码

        const int maxIters = 2000, // RANSAC 迭代的最大次数，2000 是它可以达到的最大值

        const double confidence = 0.995 //置信度

    );
*/
/*
用于计算单应性矩阵的方法有：

0 ：使用所有点的常规方法；

RANSAC：基于 RANSAC 的鲁棒法；

LMEDS ：最小中值鲁棒法；

RHO ：基于 PROSAC 的鲁棒法；

被最小化。如果参数方法被设置为默认值 0，则函数使用所有的点对以简单的最小二乘方案计算初始单应性估计。

然而，如果不是所有的点对都符合刚性透视变换（也就是说有一些异常值），那么这个初始估计就会很差。在这种情况下，可以使用三种鲁棒法之一。方法 RANSAC，LMeDS 和 RHO 尝试使用这个子集和一个简单的最小二乘算法来估计单应矩阵的各个随机子集（每个子集有四对），然后计算计算的单应性的质量/良好度（这是 RANSAC 的内点数或 LMeD 的中值重投影误差）。然后使用最佳子集来产生单应矩阵的初始估计和内点/外点的掩码。

不管方法是否鲁棒，计算的单应性矩阵都用 Levenberg-Marquardt 方法进一步细化（仅在鲁棒法的情况下使用 inlier）以更多地减少再投影误差。

RANSAC 和 RHO 方法几乎可以处理任何异常值的比率，但需要一个阈值来区分异常值和异常值。 LMeDS 方法不需要任何阈值，但只有在超过 50％的内部值时才能正常工作。最后，如果没有异常值且噪声相当小，则使用默认方法（method = 0）。
*/

/*
perspectiveTransform(）

函数 perspectiveTransform(）执行矢量的透视矩阵变换。

参数说明：

void perspectiveTransform(

InputArray src, //输入双通道或三通道浮点数组/图像

OutputArray dst, //输出与 src 相同大小和类型的数组/图像

InputArray m //3x3 或 4x4 浮点转换矩阵

);
*/

// 平面对象识别：

#include <opencv2/opencv.hpp>

#include <opencv2/xfeatures2d.hpp>

using namespace cv;

using namespace cv::xfeatures2d;

int main()

{
    Mat src1, src2;

    src1 = imread("E:/image/image/card.jpg");

    src2 = imread("E:/image/image/cards.jpg");

    if (src1.empty() || src2.empty())

    {

        printf("can ont load images....\n");

        return -1;
    }

    imshow("image1", src1);

    imshow("image2", src2);

    int minHessian = 400;

    //选择 SURF 特征

    Ptr<SURF> detector = SURF::create(minHessian);

    std::vector<KeyPoint> keypoints1;

    std::vector<KeyPoint> keypoints2;

    Mat descriptor1, descriptor2;

    //检测关键点并计算描述符

    detector->detectAndCompute(src1, Mat(), keypoints1, descriptor1);

    detector->detectAndCompute(src2, Mat(), keypoints2, descriptor2);

    //基于 Flann 的描述符匹配器

    FlannBasedMatcher matcher;

    std::vector<DMatch> matches;

    //从查询集中查找每个描述符的最佳匹配

    matcher.match(descriptor1, descriptor2, matches);

    double minDist = 1000;

    double maxDist = 0;

    for (int i = 0; i < descriptor1.rows; i++)

    {

        double dist = matches[i].distance;

        printf("%f \n", dist);

        if (dist > maxDist)

        {

            maxDist = dist;
        }

        if (dist < minDist)

        {

            minDist = dist;
        }
    }

    // DMatch 类用于匹配关键点描述符的

    std::vector<DMatch> goodMatches;

    for (int i = 0; i < descriptor1.rows; i++)

    {

        double dist = matches[i].distance;

        if (dist < max(2\* minDist, 0.02))

        {

            goodMatches.push_back(matches[i]);
        }
    }

    Mat matchesImg;

    drawMatches(src1, keypoints1, src2, keypoints2, goodMatches, matchesImg, Scalar::all(-1),

                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    std::vector<Point2f> point1, point2;

    for (int i = 0; i < goodMatches.size(); i++)

    {

        point1.push_back(keypoints1[goodMatches[i].queryIdx].pt);

        point2.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }

    Mat H = findHomography(point1, point2, RANSAC);

    std::vector<Point2f> cornerPoints1(4);

    std::vector<Point2f> cornerPoints2(4);

    cornerPoints1[0] = Point(0, 0);

    cornerPoints1[1] = Point(src1.cols, 0);

    cornerPoints1[2] = Point(src1.cols, src1.rows);

    cornerPoints1[3] = Point(0, src1.rows);

    perspectiveTransform(cornerPoints1, cornerPoints2, H);

    //绘制出变换后的目标轮廓，由于左侧为图像 src2 故坐标点整体右移 src1.cols

    line(matchesImg, cornerPoints2[0] + Point2f(src1.cols, 0), cornerPoints2[1] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);

    line(matchesImg, cornerPoints2[1] + Point2f(src1.cols, 0), cornerPoints2[2] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);

    line(matchesImg, cornerPoints2[2] + Point2f(src1.cols, 0), cornerPoints2[3] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);

    line(matchesImg, cornerPoints2[3] + Point2f(src1.cols, 0), cornerPoints2[0] + Point2f(src1.cols, 0), Scalar(0, 255, 255), 4, 8, 0);

    //在原图上绘制出变换后的目标轮廓

    line(src2, cornerPoints2[0], cornerPoints2[1], Scalar(0, 255, 255), 4, 8, 0);

    line(src2, cornerPoints2[1], cornerPoints2[2], Scalar(0, 255, 255), 4, 8, 0);

    line(src2, cornerPoints2[2], cornerPoints2[3], Scalar(0, 255, 255), 4, 8, 0);

    line(src2, cornerPoints2[3], cornerPoints2[0], Scalar(0, 255, 255), 4, 8, 0);

    imshow("output", matchesImg);

    imshow("output2", src2);

    waitKey();

    return 0;
}
