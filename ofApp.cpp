#include "ofApp.h"
const int Nx = 2048;
const int Ny = 2048;
float alpha = 0.9;
float nxc = 1024.0;
float nyc = 1024.0;// -1350.0*sin(PI / 4.0);
const float r = Nx /2.0; // circle radius
float r0n = 0.08;
float r0 = r0n*r;

complex<double> nC;
//--------------------------------------------------------------
void ofApp::setup(){
	ImageIn.loadImage("F2217.jpg");
	ColorImage.setFromPixels(ImageIn.getPixels().getData(), ImageIn.getWidth(), ImageIn.getHeight());
	InputCV = ColorImage.getCvImage();
	// creating the transformation matrixes
	TranfX.create(Ny, Nx, CV_32F);
	TranfY.create(Ny, Nx, CV_32F);
	// center in the [-1, 1] circle
	
}

//--------------------------------------------------------------
void ofApp::update(){
 // creating the remaping matrixes
	float theta,rho,phsi;
	float xc;
	float yc;
	//// conformal mapping.
	nC = complex<double>((nxc - (Nx / 2.0)) / (Nx / 2.0), (nyc - (Ny / 2.0)) / (Ny / 2.0));
	for (int y = 0; y < Ny; y++) {
		for (int x = 0; x < Nx; x++) {

	//		// centered normmaliced coordinates
			xc = (x - Nx / 2.0)/(Nx/2.0);
			yc = (y - Ny / 2.0)/(Ny/2.0);

			//Input
			complex<double>W(xc, yc);
			if (abs(W) < 1.0) {
				if (abs(W) > r0n) {
		//	W = W*(double)pow((1-abs(W)), alpha) / abs(W);
				complex<double>Wt(xc, yc);
			Wt = W*(double)ofMap(abs(W), r0n, 1.0, 1.0, 0.0 ) / abs(W);
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
				TranfX.at<float>(y, x) = x;
				TranfY.at<float>(y, x) = y;
			}
		
				}
			else {
				TranfX.at<float>(y, x) = x;
				TranfY.at<float>(y, x) = y;			
				}
		
		}
	}

	// new center t o circle center
	//float nc2c = sqrt((nxc - Nx / 2.0)*(nxc - Nx / 2.0) + (nyc - Ny / 2.0)*(nyc - Ny / 2.0));
	//float anglenc =  atan2(nyc - Ny / 2.0, nxc - Nx / 2.0);
	//for (int y = 0; y < Ny; y++) {
	//	for (int x = 0; x < Nx; x++) {
	//		// coordenates related to new center
	//		float ncordx = x - nxc;
	//		float ncordy = y - nyc;

	//		xc = x - Nx / 2.0;
	//		yc = y - Ny / 2.0;
	//		theta = atan2(yc, xc);
	//		rho = sqrt(xc*xc + yc*yc);

	//	    // temporal angle 1
	//		float angle1 = theta;
	//		angle1 -= anglenc;
	//		// using sine theorem and the fact that sin(pi-a) = sin(a)
	//		float angle2 = asin(nc2c*sin(abs(angle1))/r);
	//		// sum of internal angles of a trianngle

	//		float angle3 = PI - ((PI-abs(angle1)) + angle2);
	//		// calculate the other triangle side
	//		float d;
	//		if ((abs(angle1) < 0.01)) {
	//			d = r - nc2c;
	//			
	//		}
	//		else if (abs(PI-angle1) < 0.01) {
	//			d = r + nc2c;
	//			
	//		}
	//		else {
	//			d = sin(angle3)*nc2c / sin(angle2);
	//			/*cout << d << endl;*/
	//		}
	//		
	//		// distance to new center
	//		float dtnc = sqrt(ncordx*ncordx + ncordy*ncordy);

	//		// now filling the transf matrixes

	//		
	//		if ((rho < r)&&(rho>r0)) {
	//			
	//			float sweepangle = asin(rho / r) - asin(r0 / r);
	//			float thefrac = 1.0 - sweepangle/(PI / 2 - asin(r0 / r));
	//			thefrac = powf( thefrac, alpha);

	//			float Transfac = d*thefrac;
	//		   /*float Transfac = d*powf((d - d*(rho - r0) / (r - r0)) / d, alpha);*/
	//			//float Transfac = d*powf((d - rho*d / r) / d, alpha);
	//			TranfX.at<float>(y, x) = nxc + Transfac*cos(theta);
	//			TranfY.at<float>(y, x) = nyc + Transfac*sin(theta);
	//		}
	//		else {
	//			TranfX.at<float>(y, x) = x;
	//			TranfY.at<float>(y, x) = y;
	//		
	//		}
	//	}
	//}

	//float newrho;
	//for (int y = 0; y < Ny; y++) {
	//	for (int x = 0; x < Nx; x++) {
	//		xc = x - Nx / 2.0;
	//		yc = y - Ny / 2.0;
	//		theta = atan2(yc, xc);
	//		rho = sqrt(xc*xc + yc*yc);
	//		
	//		if (rho < Nx/2.0) {
	//			newrho = 512.0*powf(1.0 - rho/512.0, alpha);
	//			TranfX.at<float>(y, x) = Nx / 2.0 + newrho*cos(theta);
	//			TranfY.at<float>(y, x) = Ny / 2.0 + newrho*sin(theta);
	//		}
	//		else {
	//			TranfX.at<float>(y, x) = x;
	//			TranfY.at<float>(y, x) = y;
	//		}
	//	}
	//}

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
		ToSave.draw(0, 0, 512, 512);
		//ToSave.saveImage("Salu4B.png"); 
		//OF_EXIT_APP(0);

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
