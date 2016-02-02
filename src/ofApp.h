/*

	Spout - Sender example

    Visual Studio 2012 using the Spout SDK

	Copyright (C) 2015 Lynn Jarvis.

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

*/
#pragma once

#include "ofMain.h"
#include "..\..\..\SpoutSDK\Spout.h" // Spout SDK
#include "resource.h"

//get rid of existen function to avoid conflict with imgui https://github.com/ocornut/imgui/issues/340
#ifdef GetWindowFont
#undef GetWindowFont
#endif


#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void windowResized(int w, int h);
		void loadText();
		void keyPressed(int key);
		void textChanged();
		void drawText();
		void menuRender();
		void loadSettings();
		void saveSettings();

		float           nextLetterTime;
		int             lineCount;
		int             letterCount;
		vector <string> seussLines;
	
		SpoutSender *spoutsender; // A sender object
		char sendername[256];     // Sender name
		GLuint sendertexture;     // Local OpenGL texture used for sharing
		bool bInitialized;        // Initialization result
		ofImage myTextureImage;   // Texture image for the 3D demo
		float rotX, rotY;
		bool InitGLtexture(GLuint &texID, unsigned int width, unsigned int height);
				
		bool bHide;
		ofxXmlSettings XML;

		ofColor color;
		float textColor[4] = { 0.0f,0.0f,0.0f,1.0f };
		ofVec2f center;
		string screenSize;
		
		string lineSize;		
		string fontName;
		int fontSelectedIndex;
		bool playText;
		bool centerMode;
		int textSize;
		int textVelocity = 10;
		ofTrueTypeFont myfont;
		vector<const char*> fontOptions;

		ofxImGui gui;
		float floatValue;

		vector<string> fonts;
		char showText[100];

};
