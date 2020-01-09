#include "ofApp.h"
const int Nx = 2048;
const int Ny = 2048;
float alpha = 0.9;
float nxc = 1224.0;
float nyc = 1124.0;// -1350.0*sin(PI / 4.0);
const float r = Nx /2.0; // circle radius
float r0n = 0.08;
float r0 = r0n*r;

// for reading frames:
ofImage AllImages;
int FN = 1;
complex<double> nC;
//--------------------------------------------------------------
void ofApp::setup(){
	


	// creating the transformation matrixes
	TranfX.create(Ny, Nx, CV_32F);
	TranfY.create(Ny, Nx, CV_32F);
	
	// creating the remaping matrixes
	float theta, rho, phsi;
	float xc;
	float yc;
	//// conformal mapping.
	nC = complex<double>((nxc - (Nx / 2.0)) / (Nx / 2.0), (nyc - (Ny / 2.0)) / (Ny / 2.0));
	for (int y = 0; y < Ny; y++) {
		for (int x = 0; x < Nx; x++) {

			//		// centered normmaliced coordinates
			xc = (x - Nx / 2.0) / (Nx / 2.0);
			yc = (y - Ny / 2.0) / (Ny / 2.0);

			//Input
			complex<double>W(xc, yc);
			if (abs(W) < 1.0) {
				if (abs(W) > r0n) {
					//	W = W*(double)pow((1-abs(W)), alpha) / abs(W);
					complex<double>Wt(xc, yc);
					Wt = W*(double)ofMap(abs(W), r0n, 1.0, 1.0, 0.0) / abs(W);
					Wt = Wt*(double)pow(abs(Wt), alpha) / abs(Wt);
					//float TempAng = asin(powf(abs(W),alpha));
					//W = W / abs(W)*sin(PI / 2.0 - TempAng);
					/*	W = W*pow(1-abs(W),alpha) / abs(W);*/

					//	float the_W = powf(abs(W), alpha);
					//	W = W*((1.0 - the_W) / the_W);
					//W = W*((double)powf((1.0 - abs(W))/abs(W),alpha));
					//Output
					complex<double>Z;

					Z = (Wt + nC) / (1.0 + (conj(nC)*Wt));




					TranfX.at<float>(y, x) = Nx / 2.0 + (Nx / 2.0)*Z.real();
					TranfY.at<float>(y, x) = Ny / 2.0 + (Ny / 2.0)*Z.imag();
				}
				else {
					TranfX.at<float>(y, x) = Nx / 2.0 + xc/r0n*(Nx/2.0);
					TranfY.at<float>(y, x) = Ny / 2.0 + yc / r0n*(Ny / 2.0);
				}

			}
			else {
				TranfX.at<float>(y, x) = x;
				TranfY.at<float>(y, x) = y;
			}

		}
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){
	AllImages.loadImage("C://AfterPhd/Toaljpg/F" + ofToString(FN)+".jpg");
	ColorImage.setFromPixels(AllImages.getPixels().getData(), AllImages.getWidth(), AllImages.getHeight());
	InputCV = ColorImage.getCvImage();
	cv::remap(InputCV, OutputCV, TranfX, TranfY, cv::INTER_CUBIC);

}

//--------------------------------------------------------------
void ofApp::draw(){
	if (OutputCV.data != NULL) {
		//ofxCvColorImage OutTemp;
		//OutTemp.allocate(OutputCV.cols, OutputCV.rows);
		//OutTemp.setFromPixels(OutputCV.data, OutputCV.cols, OutputCV.rows);
		//OutTemp.draw(0,0, 512, 512);
		ofImage ToSave;
		ToSave.allocate(OutputCV.cols, OutputCV.rows, OF_IMAGE_COLOR);
		ToSave.setFromPixels(OutputCV.data, OutputCV.cols, OutputCV.rows, OF_IMAGE_COLOR);
		//ToSave.draw(0, 0, 512, 512);
		cout << FN << endl;
		ToSave.saveImage("Circles/FV" + ofToString(FN)+".jpg"); 
		FN++;
		if (FN > 5885) {
			OF_EXIT_APP(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_UP ) {
		alpha += 0.1;
		cout << alpha << endl;
	}
	else if (key == OF_KEY_DOWN) {
	
		alpha -= 0.1;
		cout << alpha << endl;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	nxc = Nx/512.0*x;
	nyc = Ny/512.0*y;
	cout << Nx / 512.0*x << endl;
	cout << Ny / 512.0*y << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
