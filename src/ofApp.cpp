/*
	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	fontSelectedIndex = 0;

	ofDirectory dir(".");
	dir.allowExt("ttf");
	dir.listDir();
	int size = dir.size();
	for (int i = 0; i < size; i++){ 
		string filename = dir.getName(i);
		fonts.push_back(filename);
	}

	if (fonts.size() == 0) {
		exit();
	}

	for (size_t i = 0; i < fonts.size(); ++i)
		fontOptions.push_back(fonts[i].c_str());

	//required call
	gui.setup();

	ofSetVerticalSync(true);
	ofBackground(0);

	nextLetterTime = ofGetElapsedTimeMillis();
	lineCount = 0;
	letterCount = 1;

	color = ofColor(255);
	playText = true;
	center = ofVec2f(ofGetWidth()*.5, ofGetHeight()*.5);
	centerMode = 0;
	textSize = 20;

	loadText();

    // ====== SPOUT =====
	spoutsender = new SpoutSender;			// Create a Spout sender object
	//spoutsender->SetVerticalSync();
	bInitialized	= false;		        // Spout sender initialization
	strcpy(sendername, "ofSpoutText Sender");	// Set the sender name
	// Create an OpenGL texture for data transfers
	sendertexture = 0; // make sure the ID is zero for the first time
	InitGLtexture(sendertexture, ofGetWidth(), ofGetHeight());
	// Set the window icon from resources
	SetClassLong(GetActiveWindow(), GCL_HICON, (LONG)LoadIconA(GetModuleHandle(NULL), MAKEINTRESOURCEA(IDI_ICON1)));

    // ===================

	fontName = fonts[fontSelectedIndex];
	myfont.loadFont(fontName, textSize);

	bHide = false;

	loadSettings();
} // end setup


//--------------------------------------------------------------
void ofApp::update() {

	if (fonts[fontSelectedIndex] != fontName || textSize != myfont.getSize()) {
		ofApp::textChanged();
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	ofSetColor(0);

	// ====== SPOUT =====
	// A render window must be available for Spout initialization and might not be
	// available in "update" so do it now when there is definitely a render window.
	if(!bInitialized) {
		bInitialized = spoutsender->CreateSender(sendername, ofGetWidth(), ofGetHeight()); // Create the sender
	}
    // ======== Whatever whe want to transmit to the receiver ===========

	drawText();

	// ====== SPOUT =====
	if (bInitialized) {

        if(ofGetWidth() > 0 && ofGetHeight() > 0) { // protect against user minimize

            // Grab the screen into the local spout texture
            glBindTexture(GL_TEXTURE_2D, sendertexture);
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, ofGetWidth(), ofGetHeight());
            glBindTexture(GL_TEXTURE_2D, 0);

            // Send the texture out for all receivers to use
            spoutsender->SendTexture(sendertexture, GL_TEXTURE_2D, ofGetWidth(), ofGetHeight());

            // Show what it is sending
            ofSetColor(255);
            sprintf(str, "Sending as : [%s]", sendername);
            ofDrawBitmapString(str, 20, 20);

            // Show fps
            sprintf(str, "fps: %3.3d", (int)ofGetFrameRate());
            ofDrawBitmapString(str, ofGetWidth()-120, 20);
        }
	}
    // ===================
	// GUI
	menuRender();
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
		case 'l':
			loadText();
			break;
		case 'h':
			bHide = !bHide;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
    // ====== SPOUT =====
	spoutsender->ReleaseSender(); // Release the sender
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) 
{
	screenSize = ofToString(w) + "x" + ofToString(h);
	center = ofVec2f(ofGetWidth()*.5, ofGetHeight()*.5); // ofVec2f(0, 0), ofVec2f(w, h));

	// ====== SPOUT =====
	// Update the sender texture to receive the new dimensions
	// Change of width and height is handled within the SendTexture function
	if(w > 0 && h > 0) // protect against user minimize
        InitGLtexture(sendertexture, w, h);
}

// ====== SPOUT =====
bool ofApp::InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
{
    if(texID != 0) glDeleteTextures(1, &texID);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	/*
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	*/
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void ofApp::loadSettings() {
	if (XML.loadFile("settings.xml")) {
		textSize = XML.getValue("settings:textSize", textSize);
		textColor[0] = XML.getValue("settings:textColorR", textColor[0]);
		textColor[1] = XML.getValue("settings:textColorG", textColor[1]);
		textColor[2] = XML.getValue("settings:textColorB", textColor[2]);
		textColor[3] = XML.getValue("settings:textColorA", textColor[3]);
		strcpy(showText , XML.getValue("settings:showText", showText).c_str());
		textVelocity = XML.getValue("settings:textVelocity", textVelocity);
		ofLog(OF_LOG_NOTICE, "Settings Loaded!");
	}
	else {
		ofLog(OF_LOG_NOTICE, "Unable to load settings.xml");
	}
}

void ofApp::saveSettings() {
	XML.setValue("settings:textSize", textSize);
	XML.setValue("settings:textColorR", textColor[0]);
	XML.setValue("settings:textColorG", textColor[1]);
	XML.setValue("settings:textColorB", textColor[2]);
	XML.setValue("settings:textColorA", textColor[3]);
	XML.setValue("settings:showText", showText);
	XML.setValue("settings:textVelocity", textVelocity);	
	XML.saveFile("settings.xml");
	ofLog(OF_LOG_NOTICE, "Settings saved to xml!");
}

void ofApp::drawText() {

	ofSetColor(textColor[0] * 255, textColor[1] * 255, textColor[2] * 255, textColor[3] * 255);

	if (showText != NULL && showText[0] != 0) {
		string showCustomText(showText);
		int strWidth = myfont.stringWidth(showCustomText);
		// x and y for the drawing
		float x = (center.x * 2 - strWidth) / 2;
		float y = center.y;

		myfont.drawString(showCustomText, x, y);
	}
	else {
		// we are slowy grabbing part of the line
		string typedLine = seussLines[lineCount].substr(letterCount - 1, 1);
		string typedLenght = seussLines[lineCount].substr(0, letterCount);
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// the total width on the line
		int strWidth = myfont.stringWidth(seussLines[lineCount]) - myfont.stringWidth(typedLenght);
		if (centerMode)		
			strWidth = myfont.stringWidth(seussLines[lineCount]);
		// x and y for the drawing
		float x = (center.x * 2 - strWidth) / 2;
		float y = center.y;

		myfont.drawString(typedLine, x, y);

		if (playText) {
			// this is our timer for grabbing the next letter
			float time = ofGetElapsedTimeMillis() - nextLetterTime;
			if (time > textVelocity) {
				// increment the letter count until 
				// we reach the end of the line
				if (letterCount < (int)seussLines[lineCount].size()) {
					// move on to the next letter
					letterCount++;
					// store time for next letter type
					nextLetterTime = ofGetElapsedTimeMillis();
				}
				else {
					// wait just a flash then move on 
					// to the next line...
					if (time > 300) {
						nextLetterTime = ofGetElapsedTimeMillis();
						letterCount = 1;
						lineCount++;
						lineCount %= seussLines.size();
					}
				}
			}
		}
	}
}

void ofApp::loadText() {
	// this is our buffer to stroe the text data
	ofBuffer buffer = ofBufferFromFile("ohplaces.txt");
	seussLines.clear();
	if (buffer.size()) {
		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
			string line = *it;
			// copy the line to draw later
			// make sure its not a empty line
			if (line.empty() == false) {
				seussLines.push_back(line);
			}
			// print out the line
			cout << line << endl;
		}
	}
}

void ofApp::textChanged(){	
	fontName = fonts[fontSelectedIndex];
	myfont.loadFont(fontName, textSize);
	ofLog(OF_LOG_NOTICE, "textSizeChanged " + ofToString(fontName));
}

void ofApp::menuRender() {
	if (!bHide) {

		//required to call this at beginning
		gui.begin();

		static bool show_app_about = true;
		static float bg_alpha = 0.55f;

		if (show_app_about)
		{
			ImGui::Begin("Configurations", &show_app_about, ImVec2(500, 200), bg_alpha, ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::Text("ImGui %s", ImGui::GetVersion());
			ImGui::Separator();
			ImGui::InputText("Output Text", showText, 100);
			ImGui::Combo("Fonts", &fontSelectedIndex, &fontOptions[0], fontOptions.size());
			ImGui::SliderInt("Text Size", &textSize, 10, 40);
			ImGui::SliderInt("Text Velocity", &textVelocity, 10, 500);
			ImGui::ColorEdit4("Text Color", textColor);			
			ImGui::Checkbox("PlayText", &playText);
			ImGui::SameLine();
			ImGui::Checkbox("Center Mode", &centerMode);
			ImGui::Separator();

			if (ImGui::Button("Save Settings"))
				saveSettings();
			ImGui::SameLine();
			if (ImGui::Button("Load Settings"))
				loadSettings();
			
			ImGui::End();
		}
		//ImGui::Render();
		gui.end();
	}
}