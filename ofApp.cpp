#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->noise_param = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 50 < 20) {

		if (ofGetFrameNum() % 50 < 10) {

			this->noise_param += ofMap(ofGetFrameNum() % 50, 0, 10, 0.001, 1);
		}
		else {
		
			this->noise_param += ofMap(ofGetFrameNum() % 50, 10, 20, 1, 0.001);
		}
	}
	else {

		this->noise_param += 0.001;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	int v_span = 1;
	int u_span = 30;
	int R = 230;
	int r = 70;

	ofMesh line;
	line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	float noise_seed = ofRandom(1000);
	ofColor color;
	for (float v = 0; v <= 360; v += v_span) {

		int u_start = ofMap(ofNoise(noise_seed, cos(v * DEG_TO_RAD) * 0.9, sin(v * DEG_TO_RAD) * 0.9, this->noise_param), 0, 1, -360, 360);
		int next_u = ofMap(ofNoise(noise_seed, cos((v + v_span) * DEG_TO_RAD) * 0.9, sin((v + v_span) * DEG_TO_RAD) * 0.9, this->noise_param), 0, 1, -360, 360);
		for (int u = u_start; u < u_start + 360; u += u_span) {
			
			line.addVertex(this->make_point(R, r, u, v));
			line.addVertex(this->make_point(R, r, next_u, v + v_span));

			color.setHsb((int)ofMap(u, u_start, u_start + 360, 0, 512) % 255, 200, 255);
			ofColor line_color = color;

			line.addColor(line_color); line.addColor(line_color);
			line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 2);
			next_u += u_span;
		}
	}

	line.drawWireframe();

	this->cam.end();

	/*
	int start = 220;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}