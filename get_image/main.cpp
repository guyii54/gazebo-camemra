#include <stdio.h>
#include <stack>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>      //setprecision()
#include <libxml/parser.h>

//https://mp.weixin.qq.com/s/48jMBVVEqQp3IR6NpUy2QA

using namespace cv;
using namespace std;

 #define COLLECT_IMAGES  //def：采集图像模式  notdef：标定模式

int main()
{
//    namedWindow("准备",1);
//    cout << "\n按空格后开始!" << endl;
//    int ckey_deviation = -1;
//    while(ckey_deviation == -1) ckey_deviation = waitKey(0);
//    ckey_deviation = ckey_deviation - ' ';
//    cout << "按键偏移量" << ckey_deviation << endl;

#ifdef COLLECT_IMAGES   //采集图像模式

    float rt = 1.0/4;
    float s = 1.0/25;

    float h;

    h = s / (2 * rt);

    cout << "高度：" << h << "m" << endl;

    return 0;

//    float rf = 2.805;
//    float s = 1.0/5;
//    float R;
//    float theta3;
//    float x;


//    rf /= 100;

//    R = (1 - rf)/rf;
//    theta3 = atan(1/R);
//    x = s*0.5/sin(theta3);

//    cout << "最远：" << x << "m" << endl << endl;
//    return 0;


/*    system("mkdir ./pictures");     //create a file to save pictures
//    ofstream image_name_file("picture_names.txt");  //create a txt to save images' names

    xmlDocPtr doc;//定义解析文档指针
    xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动)
    xmlChar *szKey;          //临时字符串变量
    char szDocName[] = "camera_tutorial.world";

    doc =xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER); //解析文件

    //检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
    //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
    //如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
    if (NULL == doc)
    {
       //文档打开错误
       return -1;
    }

    curNode = xmlDocGetRootElement(doc); //确定文档根元素

    //*检查确认当前文档中包含内容
    if (NULL == curNode)
    {
       //空得xml文件
       xmlFreeDoc(doc);
       return -2;
    }

    //*在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。
    if (xmlStrcmp(curNode->name, BAD_CAST "root"))
    {
       //分析根元素失败
       xmlFreeDoc(doc);
       return -3;
    }
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(curNode != NULL)
    {
       //取出节点中的内容
       if ((!xmlStrcmp(curNode->name, (constxmlChar *)"node2")))
       {
           szKey =xmlNodeGetContent(curNode);
           cout <<szKey << endl; //得到node2的值并输出
           xmlFree(szKey);
       }
       //查找带有属性attribute的节点
       if (xmlHasProp(curNode,BAD_CAST"b"))
       {
           propNodePtr =curNode;
       }
       curNode = curNode->next;
    }
    //查找属性
    xmlAttrPtr attrPtr = propNodePtr->properties;
    while (attrPtr != NULL)
    {
       if (!xmlStrcmp(attrPtr->name, BAD_CAST"b"))
       {
           xmlChar* szAttr =xmlGetProp(propNodePtr,BAD_CAST "b");
           cout << szAttr<< endl; //找到b的属性并输出
           xmlFree(szAttr);
       }
       attrPtr = attrPtr->next;
    }
    xmlFreeDoc(doc);
    return 0;*/


//    waitKey(0);
//    Mat frame;
//    char image_name[100];
//    int image_name_count = 1;  //using for nameing images

//    while(1)
//    {

//    }
    return 0;
#else  //标定模式
    ifstream fin("calibdata.txt"); // 标定所用图像文件的路径
    ofstream fout("caliberation_result.txt");  // 保存标定结果的文件

    int image_count=0;  // 图像数量
    Size image_size;  // 图像的尺寸

    Size board_size = Size(6,4);    // 标定板角点的尺寸
    int CornerNum = board_size.width*board_size.height;  //每张图片上总的角点数

    vector<Point2f> image_points_buf;  // 存储一幅图的角点
    vector< vector<Point2f> >  image_points_seq; // 存储所有图片的角点

    string filename;		//保存图像名字

    /*测试按键数值
    int ch = -1;

    while(1)
    {
        ch = waitKey(0) - ckey_deviation;
        cout << ch <<endl;
    }*/

    //*********************  获取角点位置  **********************
    while (getline(fin,filename))
    {
        Mat imageInput; // 读入彩色图像
        imageInput=imread(filename);
        if (image_count == 0)  // 获得图像尺寸
        {
            image_size.width = imageInput.cols;
            image_size.height =imageInput.rows;
            cout << "image_size.width = " << image_size.width << endl;
            cout << "image_size.height = " << image_size.height << endl;
        }

        image_count++;
        
        cout << "No. image is " << image_count << endl;

        // 提取角点
        if (0 == findChessboardCorners(imageInput,board_size,image_points_buf))
        {
            cout << "can not find chessboard corners!\n"; //找不到角点
            exit(1);
        }
        else
        {
            Mat view_gray;// 获取灰度图
            cvtColor(imageInput,view_gray,CV_RGB2GRAY);

            // 亚像素精确化
            find4QuadCornerSubpix(view_gray,image_points_buf,Size(11,11)); //对粗提取的角点进行精确化
            image_points_seq.push_back(image_points_buf);  //保存亚像素角点

            // 在图像上显示角点位置
            drawChessboardCorners(view_gray,board_size,image_points_buf,true); //用于在图片中标记角点
            imshow("Camera Calibration",view_gray);//显示图片
            waitKey(200);
        }
    }

    int total_corner_num = CornerNum * image_count;
    //*********************  打印角点信息  **********************
    for (int ii=0 ; ii < total_corner_num ; ii++)
    {
        if (0 == ii%CornerNum)// 是每幅图片的角点个数。此判断语句是为了输出 图片号，便于控制台观看
        {
            cout << "\n\n第" << ii/CornerNum+1 << "图片的数据:";
        }
        if (0 == ii%3)  // 此判断语句，格式化输出，便于控制台查看
        {
            cout<<endl;
        }
        else
        {
            cout.width(6);
        }
        //输出所有的角点
        cout << "( " ;
        if(image_points_seq[ii/CornerNum][ii%CornerNum].x < 10) cout << "  ";
        else if(image_points_seq[ii/CornerNum][ii%CornerNum].x < 100) cout << " ";
        cout << fixed << setprecision(3) << image_points_seq[ii/CornerNum][ii%CornerNum].x ;
        cout << " , ";
        if(image_points_seq[ii/CornerNum][ii%CornerNum].y < 10) cout << "  ";
        else if(image_points_seq[ii/CornerNum][ii%CornerNum].y < 100) cout << " ";
        cout << fixed << setprecision(3) << image_points_seq[ii/CornerNum][ii%CornerNum].y << " )";
    }
    cout << "\n\n角点提取完成！\n" << endl;

    //*********************  标定参数准备  **********************
    cout << "准备标定.." << endl;
    //棋盘三维信息
    Size square_size = Size(4,4);  // 世界坐标系下 黑白块宽度  4cm
    vector< vector<Point3f> > object_points; // 世界坐标系下 角点坐标（人为固定）

    Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); // 内参数矩阵
    Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); // 畸变系数：  k1,k2,p1,p2,k3
    vector<Mat> tvecsMat;  // 旋转向量
    vector<Mat> rvecsMat; // 平移矩阵

    //初始化世界坐标系下 角点的位置（人为设定）
    int i,j,t;
    for (t=0;t<image_count;t++)
    {
        vector<Point3f> tempPointSet;
        for (i=0;i<board_size.height;i++)
        {
            for (j=0;j<board_size.width;j++)
            {
                Point3f realPoint;
                //假设标定板放在世界坐标系中z=0的平面上
                realPoint.x = j*square_size.width;   
                realPoint.y = i*square_size.height;
                realPoint.z = 0;
                tempPointSet.push_back(realPoint);
            }
        }
        object_points.push_back(tempPointSet);
    }

    //*********************  开始标定  **********************
    calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
    cout << "标定完成！\n" << endl;

    //*********************  检验标定效果  **********************
    cout<<"评价标定结果..\n";
    double total_err = 0.0; // 所有图像的平均误差的总和
    double err = 0.0; // 每幅图像的平均误差
    vector<Point2f> image_points2; // 保存重新计算得到的投影点
    cout << "每幅图像的标定误差：\n";
    fout << "每幅图像的标定误差：\n";

    for (i=0;i<image_count;i++)
    {
        vector<Point3f> tempPointSet = object_points[i];//获取一幅图像中的角点位置（世界坐标系）

        // 将世界坐标系的点投影到图像上（根据已知的内参、旋转、平移）
        projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);

        // 计算图像上的角点 与 计算出来的图像上的点 比较
        Mat tempImagePointMat = Mat(1,image_points_seq[i].size(),CV_32FC2);
        Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
        for (int j = 0 ; j < image_points_seq[i].size(); j++)
        {
            image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);//计算出来的点
            tempImagePointMat.at<Vec2f>(0,j) = Vec2f(image_points_seq[i][j].x, image_points_seq[i][j].y);//图像上的角点
        }

        err = norm(image_points2Mat, tempImagePointMat, NORM_L2);//sqrt((x11-x12)^2 +(y11-y12)^2 + (21-x22)^2 + ...)
        total_err += err/=  CornerNum;

        cout << "第" << i+1 << "幅图像的平均误差：" << err << "像素" << endl;
        fout << "第" << i+1 << "幅图像的平均误差：" << err << "像素" << endl;
    }
    cout << "总体平均误差：" << total_err/image_count << "像素" << endl;
    fout << "总体平均误差：" << total_err/image_count << "像素\n" << endl;
    cout << "评价完成！" << endl;
    
    //*********************  保存标定结果  **********************
    cout << "\n开始保存定标结果.." << endl;

    Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); // 保存每幅图像的旋转矩阵

    fout << "相机内参数矩阵：" << endl;
    fout << cameraMatrix << endl << endl;

    fout << "畸变系数 k1,k2,p1,p2,k3：\n";
    fout << distCoeffs << endl << endl << endl;

    for (int i=0; i<image_count; i++)
    {
        fout << "第" << i+1 << "幅图像" << endl;

        fout << "平移矩阵：" << endl;
        fout << tvecsMat[i] << endl;

        //fout << "旋转向量：" << endl;
        //fout << rvecsMat[i] << endl;

        // 将旋转向量转换为相对应的旋转矩阵
        Rodrigues(rvecsMat[i],rotation_matrix);
        fout << "旋转矩阵：" << endl;
        fout << rotation_matrix << endl << endl;
    }
    cout << "完成保存" << endl;
    fout << endl;

    return 0;
#endif
}
