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


#include "ofMain.h"

#include "ofGLProgrammableRenderer.h"
//#include <libs\gl3w\GL\gl3w.h>
#include <GLFW\glfw3.h>
#include <stdio.h>

#include "ofApp.h"

int main()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofGLWindowSettings glWindowSettings;
	glWindowSettings.width = 640;
	glWindowSettings.height = 360;
	glWindowSettings.setGLVersion(3,1);
	ofCreateWindow(glWindowSettings);
	ofAppGLFWWindow * window = (ofAppGLFWWindow*)ofGetWindowPtr();

	

	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);*/

	
	//glfwMakeContextCurrent(window->getGLFWWindow());
	/*ofGLProgrammableRenderer * renderer = new ofGLProgrammableRenderer(window);
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(renderer));*/
	//gl3wInit();

	ofRunApp(new ofApp());
}