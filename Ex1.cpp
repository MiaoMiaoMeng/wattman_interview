#include <string>  
#include <json.h>
#include <iostream>  
#include <fstream>  

#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/imgproc/imgproc.hpp>  

using namespace std;
using namespace cv;

void readJson(string path, int left_top[], int right_bottom[]); 
void useRoi(string path1, string path2, int left_top[], int right_bottom[]);

void readJson(string path, int left_top[], int right_bottom[])
{
	ifstream in( path , ios::binary);
	string name;
	Json::Reader reader;
	Json::Value value;

	//string strValue = "{\"boxes\":[{\"name\":\"box_a\",\"rectangle\":{\"left_top\":[0,10],\"right_bottom\":[200,300]}},{\"name\":\"box_b\",\"rectangle\":{\"left_top\":[100,100],\"right_bottom\":[200,300]}}]}";
	if (reader.parse(in, value))
	{
		const Json::Value arrayObj = value["boxes"];
		for (unsigned int i = 0; i < arrayObj.size(); i++)
		{
			name = arrayObj[i]["name"].asString();
			if (name == "box_b")
			{
				left_top[0] = arrayObj[i]["rectangle"]["left_top"][0].asInt();
				left_top[1] = arrayObj[i]["rectangle"]["left_top"][1].asInt();
				right_bottom[0] = arrayObj[i]["rectangle"]["right_bottom"][0].asInt();
				right_bottom[1] = arrayObj[i]["rectangle"]["right_bottom"][1].asInt();
				cout << left_top[0] <<":"<< left_top[1] << endl;
				cout << right_bottom[0] << ":" << right_bottom[1] << endl;
				break;
			}
		}
		cout << "Reading Complete!" << endl;
	}
	else
	{
		cout << "parse error\n" << endl;
	}
	in.close();
}

void useRoi(string path1, string path2, int left_top[], int right_bottom[]) {
	Mat imageBack = imread(path1);
	Mat imageFront = imread(path2);
	int boxSize_h = right_bottom[0] - left_top[0];
	int boxSize_w = right_bottom[1] - left_top[1];
	int boxPos_x = left_top[0];
	int boxPos_y = left_top[1];

	if (!imageBack.data || !imageFront.data)
	{
		cout << "读取图像有误，请重新输入路径！(以/分割)\n";
	}
	if (imageBack.cols < right_bottom[0] || imageBack.rows < right_bottom[1]) {
		Mat imageBack_resize;
		resize(imageBack, imageBack_resize, Size(right_bottom[0], right_bottom[1]));
		imageBack = imageBack_resize;
	}
	imshow("imageBack", imageBack);
	imshow("imageFront", imageFront);
	

	
	Mat imageFront_resize;
	resize(imageFront, imageFront_resize, Size(boxSize_h, boxSize_w));
	imageFront = imageFront_resize;

	Mat imageROI = imageBack(Rect(boxPos_x, boxPos_y, boxSize_h, boxSize_w));
	Mat mask;
	cvtColor(imageFront, mask, COLOR_BGR2GRAY);

	imageFront.copyTo(imageROI, mask);

	namedWindow("填充效果");
	imshow("填充效果", imageBack);


}

int main(int argc, char *argv[])
{
	int left_top[2];
	int right_bottom[2];

	string jsonPath,pic1Path,pic2Path;
	cout << "请输入json路径(以“/分隔路径”)：" << endl;
	cin >> jsonPath;
	cout << "请输入第一张图片路径（背景图）(以“/分隔路径”)：" << endl;
	cin >> pic1Path;
	cout << "请输入第二张图片路径（填充图）(以“/分隔路径”)：" << endl;
	cin >> pic2Path;
	
	readJson(jsonPath, left_top, right_bottom);  
	useRoi(pic1Path,pic2Path,left_top,right_bottom);
	system("pause");
	return 0;
}