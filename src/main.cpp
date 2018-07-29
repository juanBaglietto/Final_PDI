#include <stdio.h>
#include <string>
#include <vector>
#include <cv.h>
//#include "stdafx.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
void onMouse(int event, int x, int y, int flags, void *param);

using namespace cv;
RNG rng(12345);

Mat imag_orignal,imag_deteccion, imag_hsv,imag_thresholded,imag_contornos,imag_texto;
Mat imag_reducida_hsv,imag_reducida,imag_reducida_bin,mat_rot,imag_rotada,aux,imag_deteccion_texto;

int main( int, char** argv )
{
    char* directorio_imagen = argv[1];
    int low_H=0,high_H=100;
    int low_S=0,high_S=165;
    int low_V=0,high_V=170;
    int px=100;
    int py=400;
    int salir;
    char *outText;
    Size zoom(922,614);
    Size zoom_2(140,100);
    Size size_reduc;
    vector< vector<Point> > contours;
    vector<Point> approx;
    CvSeq* contornos;
    CvSeq* resultado;
    CvMemStorage *storage = cvCreateMemStorage(0);
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();



    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }


    imag_orignal=imread(directorio_imagen);
    //namedWindow("org", WINDOW_AUTOSIZE );
    //imshow("org", imag_orignal);

    cvtColor( imag_orignal, imag_hsv, COLOR_RGB2HSV );

    while(true){
       // namedWindow("Original", WINDOW_AUTOSIZE );
        namedWindow("Ajuste", WINDOW_NORMAL );

        /****************Deteccion de color***************/
        cvCreateTrackbar("LowH", "Ajuste", &low_H, 255); //Hue (0 - 179)
        cvCreateTrackbar("HighH", "Ajuste", &high_H, 255);

        cvCreateTrackbar("LowS", "Ajuste", &low_S, 255); //Saturation (0 - 255)
        cvCreateTrackbar("HighS", "Ajuste", &high_S, 255);

        cvCreateTrackbar("LowV", "Ajuste", &low_V, 255); //Value (0 - 255)
        cvCreateTrackbar("HighV", "Ajuste", &high_V, 255);


       //inRange(imag_hsv,Scalar(low_H,low_S,low_V),Scalar(high_H,high_S,high_V),imag_thresholded);

        // Primera deteccion aproximda del color amarillo
       inRange(imag_hsv,Scalar(45,76,70),Scalar(99,255, 255),imag_thresholded);


        //Apertura
        erode(imag_thresholded, imag_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imag_thresholded, imag_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


        dilate( imag_thresholded, imag_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imag_thresholded, imag_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        // reduccion de tamaño para que se vea en la pantalla

//        resize(imag_orignal,imag_orignal,Size(imag_orignal.cols*0.5,imag_orignal.rows*0.5),0,0,INTER_LINEAR);
//        resize(imag_thresholded,imag_thresholded,Size(imag_thresholded.cols*0.5,imag_thresholded.rows*0.5),0,0,INTER_LINEAR);

        resize(imag_orignal,imag_orignal,zoom);
        resize(imag_thresholded,imag_thresholded,zoom);

        /****************Deteccion de forma***************/

        imag_contornos=imag_thresholded.clone();
        aux=imag_orignal.clone();
        imag_deteccion=imag_orignal.clone();
        findContours(imag_contornos, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<Point2f>center_poly( contours.size() );
        vector<float>radius( contours.size() );
        Point centro_deteccion;


        for( int i = 0; i < contours.size(); i++ )
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i],3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );

        }

        for( int i = 0; i< contours.size(); i++ )
        {
            if(contourArea(contours[i])> 1000 )
            {
                // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

                drawContours( aux, contours_poly, i, Scalar(255), 1, 8, vector<Vec4i>(), 0, Point() );
                rectangle( imag_deteccion, boundRect[i].tl(), boundRect[i].br(), CV_RGB(255, 0, 0), 2, 8, 0 );

                // Reuccion de imagen original para selleccionar solo la parte que se quiere procesar
                imag_reducida=imag_orignal(Rect(boundRect[i].x,boundRect[i].y,boundRect[i].width,boundRect[i].height));
//                resize(imag_reducida,imag_reducida,zoom);

                // econtrar el centro del poligo se detecto
                Point center(boundRect[i].x + (boundRect[i].width / 2), boundRect[i].y + (boundRect[i].height / 2));
                ostringstream str;
                str << center.x << "," << center.y;
//                putText(imag_deteccion, str.str(), center, FONT_HERSHEY_COMPLEX_SMALL, 0.60, CV_RGB(0, 255, 0), 1, CV_AA);
                centro_deteccion= center;

            }
        }


       // resize(imag_reducida,imag_reducida,Size(imag_reducida.cols*2,imag_reducida.rows*2),0,0,INTER_LINEAR);

                /**********binarizacion imagen reducida***********/

        cvtColor( imag_reducida, imag_reducida_hsv, COLOR_RGB2HSV );

        inRange(imag_reducida_hsv,Scalar(low_H,low_S,low_V),Scalar(high_H,high_S,high_V),imag_reducida_bin);

        // deteccion del color gris de la tipografia de la caravana
    //    inRange(imag_reducida,Scalar(0,0,0),Scalar(100, 165,172 ),imag_reducida_bin);

       erode(imag_reducida_bin, imag_reducida_bin, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
       dilate( imag_reducida_bin, imag_reducida_bin, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );


//       dilate( imag_reducida_bin, imag_reducida_bin, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
//       erode(imag_reducida_bin, imag_reducida_bin, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );



           //deteccion del texto
            imag_texto= imag_reducida.clone();
            imag_deteccion_texto=imag_reducida.clone();


           Mat connected;
           Mat morphKernel = getStructuringElement(MORPH_RECT, Size(9, 1));
           morphologyEx(imag_reducida_bin, connected, MORPH_CLOSE, morphKernel);
           // find contours
           Mat mask = Mat::zeros(imag_reducida_bin.size(), CV_8UC1);
           vector<vector<Point> > contours;
           vector<Vec4i> hierarchy;
           findContours(connected, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
           // filter contours
           for(int idx = 0; idx >= 0; idx = hierarchy[idx][0]){
               Rect rect = boundingRect(contours[idx]);
               Mat maskROI(mask, rect);
               maskROI = Scalar(0, 0, 0);
               // fill the contour
               drawContours(mask, contours, idx, Scalar(255, 255, 255), CV_FILLED);
               // ratio of non-zero pixels in the filled region
               double r = (double)countNonZero(maskROI) / (rect.width * rect.height);

               // detecto solo el numero mas grande de la caravana
               //if (r > 0.2 &&(rect.height > 100 && rect.width > 150)){

               if (r > 0.45 &&(rect.height > 15 && rect.width >15)){
                   rectangle(imag_deteccion_texto, rect, Scalar(0, 255, 0), 2);
                   //recorto solo el texto de la caravana
                   imag_texto=imag_reducida_bin(Rect(rect.x,rect.y,rect.width,rect.height));

               }
           }


         //resize(imag_texto,imag_texto,zoom_2);
           resize(imag_texto,imag_texto,Size(imag_texto.cols*2,imag_texto.rows*2),0,0,INTER_LINEAR);
           dilate( imag_texto, imag_texto, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
          erode(imag_texto, imag_texto, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
//           erode(imag_texto, imag_texto, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
//           dilate( imag_texto, imag_texto, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );

       imag_rotada=imag_texto.clone();

//    /********************** metodo de calculo angulo de inclinacion**************/

//       std::vector<cv::Point> points;
//       Mat_<uchar>::iterator it = imag_texto.begin<uchar>();
//       Mat_<uchar>::iterator end = imag_texto.end<uchar>();
//       for (; it != end; ++it)
//           if (*it)
//               points.push_back(it.pos());

//       RotatedRect box = cv::minAreaRect(Mat(points));
//       Mat mat_rot = getRotationMatrix2D(box.center,box.angle,1);
//      // box.angle=-box.angle;

//       warpAffine(imag_texto,imag_rotada,mat_rot,imag_texto.size(),INTER_CUBIC,BORDER_CONSTANT);
//       //std::cout << "File "<< ": " << box.angle  << std::endl;

       // OCR
        api->SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
        api->SetImage((uchar*)imag_rotada.data, imag_rotada.size().width, imag_rotada.size().height, imag_rotada.channels(), imag_rotada.step1());
        api->Recognize(0);


        outText = api->GetUTF8Text();
        std::cout << "Texto"<< ": " << outText << std::endl;


        namedWindow("Deteccion", WINDOW_AUTOSIZE );
        namedWindow("Deteccion texto", WINDOW_AUTOSIZE );
       // namedWindow("Reducida", WINDOW_AUTOSIZE );
        namedWindow("Derecha", WINDOW_AUTOSIZE );
        namedWindow("Reducida BIN", WINDOW_AUTOSIZE );
       //namedWindow("Texto", WINDOW_AUTOSIZE );

        //imshow("Original", imag_orignal);
        imshow("Deteccion", imag_deteccion);
       // imshow("Reducida", imag_reducida);
        imshow("Reducida BIN", imag_reducida_bin);
        imshow("Derecha", imag_rotada);
        imshow("Deteccion texto", imag_deteccion_texto);
        //imshow("Texto", imag_texto);
        setMouseCallback("Original", onMouse, reinterpret_cast<void*>(&imag_orignal));


        if(salir== 'q') break;
               salir=waitKey(300);
    }

    return 0;


}

void onMouse(int event, int x, int y, int flags, void *param) {
    Mat *imagen = reinterpret_cast<Mat*>(param);
    Vec3b pixel;
    switch (event) {
    case CV_EVENT_LBUTTONDOWN:
        pixel = imagen->at<Vec3b>(y, x);
        cout << "Click en [" << x << "," << y << "] el color es:" << (int)pixel[0] << "," << (int)pixel[1] << "," << (int)pixel[2] << endl;
    }
}
