#include <iostream>
#include "opencv2/opencv.hpp"
#include "aip-cpp-sdk-4.16.5/face.h"

using namespace cv;
using namespace std;
using namespace aip;

int main()
{
	VideoCapture cap(0);

    	if(!cap.isOpened())
        {	
		cout<<"Camera open fail.\n";
		return -1;
	}

	cout<<"Camera open success.\n";

	Mat img;
	Mat gray;
	Mat equalize;
	Mat faceimag;

	CascadeClassifier cl("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");

	vector<Rect> faceAll;
	vector<uchar> jpgBuf;

	std::string app_id = "34090360";
    std::string api_key = "LhlhQZrQGET0fKvNVcpv4aGW";
    std::string secret_key = "GN3ciAdfgzobF1boYEyAWjXzmYU8PctP";

    aip::Face client(app_id, api_key, secret_key);

	std::string base64img;

	Json::Value result;

	time_t sec;

	for(;;)
	{
		cap>>img;
		cvtColor(img,gray,CV_BGR2GRAY);
		equalizeHist(gray,equalize);
		cl.detectMultiScale(equalize,faceAll);

		if(faceAll.size())
		{
			rectangle(equalize,faceAll[0],Scalar(255,255,0));
			faceimag=equalize(faceAll[0]);

			imencode(".jpg",faceimag,jpgBuf);

			base64img=base64_encode((char*)jpgBuf.data(), jpgBuf.size());

			result=client.search(base64img, "BASE64", "hjj", aip::null);
			
			if(result["error_code"].asInt()==110)
			{ 
			    cout<<"baidu error"<<endl;
			    break;
			}

			if(!result["result"].isNull())
			{
				if(result["result"]["user_list"][0]["score"].asInt()>80)
				{
					cout<<result["result"]["user_list"][0]["user_id"]<<endl;
					sec=time(NULL);
					cout<<ctime(&sec)<<endl;
					putText(equalize,result["result"]["user_list"][0]["user_id"].asString(),Point(0,50),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
					putText(equalize,ctime(&sec),Point(0,100),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,0));
					
					imshow("CT",equalize);
		            waitKey(1000);
		            continue;
				}
			}

		}

		imshow("CT",equalize);
		waitKey(40);
	}

	return 0;
}
