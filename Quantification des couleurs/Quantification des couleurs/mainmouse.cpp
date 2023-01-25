#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <windows.h>

using namespace cv;
using namespace std;




int main(int argc, char** argv)
{
    VideoCapture cap(0); //capturer la vidéo de la webcam

    if (!cap.isOpened())  // si ce n'est pas le cas, quittez le programme
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }


    int iLowH = 170;
    int iHighH = 179;

    int iLowS = 150;
    int iHighS = 255;

    int iLowV = 60;
    int iHighV = 255;
    Mat frame2;
    Mat frame3;


    int iLastX = -1;
    int iLastY = -1;

    //Capturer une image temporaire à partir de l'appareil photo
    Mat imgTmp;
    cap.read(imgTmp);

    //Créer une image noire avec la taille comme sortie de la caméra
    Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;
    
    int bleu = 0;
    int ballX = 300;

    std::srand(std::time(nullptr));
    int random_variable1 = std::rand() % 420;
    int xballon1 = 500;
    int cmt = 0;
    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // lire une nouvelle image de la vidéo



        if (!bSuccess) // si ce n'est pas réussi, rompre la boucle
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV ,imgHSVV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convertir l'image capturée de BGR en HSV
        cvtColor(imgOriginal, imgHSVV, COLOR_BGR2HSV); //Convertir l'image capturée de BGR en HSV

        Mat imgThresholded, imgThresholded2, imgThresholded3;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        inRange(imgHSV, Scalar(0, 160, 160), Scalar(10, 255, 255), imgThresholded);
        inRange(imgHSV, Scalar(170, 160, 160), Scalar(180, 255, 255), imgThresholded2);


        inRange(imgHSVV, Scalar(94, 80, 2), Scalar(126, 255, 255), imgThresholded3);


        frame2 = imgThresholded | imgThresholded2;

        frame3 = imgThresholded3;


        //morphological opening (removes small objects from the foreground)
        erode(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(frame2, frame2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        erode(frame3, frame3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(frame3, frame3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        //morphological closing (removes small holes from the foreground)
        dilate(imgOriginal, imgOriginal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgOriginal, imgOriginal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //Calculer les moments de l'image seuillée
        //Moments oMoments = moments(frame2);
        rectangle(imgOriginal, Point(-2, 0), Point(25, 480), Scalar(0, 0, 250), 0, 8, 0);
        rectangle(imgOriginal, Point(680, 0), Point(620, 480), Scalar(255, 0, 0), 0, 8, 0);
        
        


        long int RedPixelX = 0, RedPixelY = 0; int PixelsRedNb = 400;
        for (int i = 0; i < frame2.rows; i++) {
            for (int j = 0; j < frame2.cols; j++) {
                uchar pxl_red = frame2.at<uchar>(i, j);
                    if (pxl_red != 0)
                    {
                        PixelsRedNb++;
                        
                    }
                   
            }
           
        }
        RedPixelY = 480 - (PixelsRedNb / 480);
       


        long int greenPixelX = 0, grennPixelY = 0; int PixelsgrennNb = 400;
        for (int i = 0; i < frame3.rows; i++) {
            for (int j = 0; j < frame3.cols; j++) {
                uchar pxl_green = frame3.at<uchar>(i, j);
                if (pxl_green !=0)
                {
                    PixelsgrennNb++;

                }

            }

        }
        grennPixelY = 480 - (PixelsgrennNb / 480);

        
        double v = PixelsRedNb /PixelsgrennNb;
        float v2 = (PixelsRedNb - PixelsgrennNb)/10000;
     


        if (v > 1 && ballX<600)
        {
            ballX+= v2;
        }
        else if (v == 1) 
        
            {
                ballX=300;
            }
        else if(v<1 && ballX>50) {
                ballX -= -v2;
            }
        


        circle(imgOriginal, Point(ballX, 100), 30, Scalar(0, 0, 255), -1, 10, 0);



        circle(imgOriginal, Point(random_variable1, xballon1), 30, Scalar(255, 0, 0), -1, 10, 0);


        rectangle(imgOriginal, Point(-2, RedPixelY), Point(25, 480), Scalar(0, 0, 250), -1, 8, 0);
        
        rectangle(imgOriginal, Point(680, grennPixelY), Point(620, 480), Scalar(255, 0, 0), -1, 8, 0);
      

        if ((ballX <= random_variable1 + 50 && ballX >= random_variable1 - 50) && (100 <= xballon1 + 50 && 100 >= xballon1 - 50))
        {

            circle(imgOriginal, Point(random_variable1, xballon1), 30, Scalar(0, 0, 255), -1, 10, 0);
            // cout << '\7';
            Beep(200, 200);

            xballon1 = 500;
            cmt++;
            random_variable1 = std::rand() % 480;

        }
        stringstream ss;
        ss << cmt;
        string str = ss.str();
        string score = "SCORE : ";
        score += str;

        putText(imgOriginal, score, Point(50, 50),
            FONT_HERSHEY_SIMPLEX, 2.1,
            Scalar(255, 0, 0), 2, LINE_AA);


        if (xballon1 < 0 )
        {
            putText(imgOriginal, "Game Over ", Point(150, 200),
                FONT_HERSHEY_SIMPLEX, 2.1,
                Scalar(0, 0, 255), 2, LINE_AA);

        }

        xballon1--;

        imshow("Thresholded Image", frame2); //show the thresholded image
        imshow("Thresholded Images", frame3);

        imgOriginal = imgOriginal + imgLines;
        imshow("Original", imgOriginal); //show the original image


        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}