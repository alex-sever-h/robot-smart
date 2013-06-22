/*
 * main.cpp
 *
 *  Created on: May 21, 2013
 *      Author: alseh
 */

#include <iostream>
#include <list>
#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/glcanvas.h"
#include "wx/timer.h"
#include "wx/spinctrl.h"
#include "roboGUI.h"
#include "sensorManagement.h"
#include "mapDiscrete.hpp"
#include "MapParticle.hpp"
#include "Location.hpp"
//
//// include OpenGL
//#ifdef __WXMAC__
//#include "OpenGL/glu.h"
//#include "OpenGL/gl.h"
//#else
#include <GL/glu.h>
#include <GL/gl.h>
#include "robotModel.hpp"
//#endif


#define UPDATE_TIMER_ID 2

#define MAP_FACTOR (5)

class RoboGuiApp: public wxApp
{
	virtual bool OnInit();

	wxFrame *frameAbsolute;

	RoboGLMap    *glRoboMap;
	RoboControls *panelControls;

public:
};


bool RoboGuiApp::OnInit()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	frameAbsolute = new wxFrame((wxFrame *)NULL, -1,  wxT("RoboInterface"), wxPoint(0,0), wxSize(1124,768));

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
	glRoboMap = new RoboGLMap( (wxFrame*) frameAbsolute, args);
	sizer->Add(glRoboMap, 10, wxEXPAND);

	panelControls = new RoboControls(frameAbsolute, wxSize(0, 768));
	sizer->Add(panelControls, 1, wxEXPAND | wxDOWN);

	frameAbsolute->SetSizer(sizer);
	frameAbsolute->SetAutoLayout(true);
	frameAbsolute->Show();

	cout <<"GUI initialized" << endl;
	return true;
}

BEGIN_EVENT_TABLE(RoboGLMap, wxGLCanvas)
EVT_MOTION(RoboGLMap::mouseMoved)
EVT_LEFT_DOWN(RoboGLMap::mouseDown)
EVT_LEFT_UP(RoboGLMap::mouseReleased)
EVT_RIGHT_DOWN(RoboGLMap::rightClick)
EVT_LEAVE_WINDOW(RoboGLMap::mouseLeftWindow)
EVT_SIZE(RoboGLMap::resized)
EVT_KEY_DOWN(RoboGLMap::keyPressed)
EVT_KEY_UP(RoboGLMap::keyReleased)
EVT_MOUSEWHEEL(RoboGLMap::mouseWheelMoved)
EVT_PAINT(RoboGLMap::render)
EVT_TIMER(UPDATE_TIMER_ID, RoboGLMap::timerUpdate)
END_EVENT_TABLE()


// some useful events to use
void RoboGLMap::mouseMoved(wxMouseEvent& event)
{

}

void RoboGLMap::mouseDown(wxMouseEvent& event)
{
	cout << "clicked at: " << event.m_x << " x " << event.m_y << endl;

	Location newTarget;
	newTarget.x = ((float)event.m_x - getWidth()/2)*MAP_FACTOR;
	newTarget.y = (getHeight()/2 - (float)event.m_y)*MAP_FACTOR;

	cout << "target at: " << newTarget.x << " x " << newTarget.y << endl;

	//physicalRobot.moveAtLocation(newTarget);
}

void RoboGLMap::mouseWheelMoved(wxMouseEvent& event)
{

}

void RoboGLMap::mouseReleased(wxMouseEvent& event)
{

}

void RoboGLMap::rightClick(wxMouseEvent& event)
{

}

void RoboGLMap::mouseLeftWindow(wxMouseEvent& event)
{

}

void RoboGLMap::keyReleased(wxKeyEvent& event)
{

}


RoboGLMap::RoboGLMap(wxFrame* parent, int* args) :
									wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
									m_timer(this, UPDATE_TIMER_ID)
{
	robotClient.setRcd(&robotClientData);

	m_context = new wxGLContext(this);

	m_timer.Start(1000/30);

	printf("robomapgl started\n");

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

RoboGLMap::~RoboGLMap()
{
	delete m_context;
}

void RoboGLMap::timerUpdate(wxTimerEvent & event)
{
	Refresh();
}

void RoboGLMap::resized(wxSizeEvent& evt)
{
	wxGLCanvas::OnSize(evt);

	Refresh();
}

void RoboGLMap::keyPressed(wxKeyEvent& event)
{
	printf("keypress\n");
}

/** Inits the OpenGL viewport for drawing in 3D. */
void RoboGLMap::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);

	glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
	gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

/** Inits the OpenGL viewport for drawing in 2D. */
void RoboGLMap::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glEnable(GL_TEXTURE_2D);   // textures
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

unsigned int RoboGLMap::getWidth()
{
	return GetSize().x;
}

unsigned int RoboGLMap::getHeight()
{
	return GetSize().y;
}

void RoboGLMap::drawRobot(void) {
	robotClientData.rInfoMutex.lock();
	int points[6][2] = {
			{+robotClientData.robotInfo.length()/2 + 20, 0 },
			{+robotClientData.robotInfo.length()/2,      -robotClientData.robotInfo.width()/2},
			{-robotClientData.robotInfo.length()/2,      -robotClientData.robotInfo.width()/2},
			{-robotClientData.robotInfo.length()/2 + 20, 0},
			{-robotClientData.robotInfo.length()/2,      +robotClientData.robotInfo.width()/2},
			{+robotClientData.robotInfo.length()/2,      +robotClientData.robotInfo.width()/2}
	};


	glColor4f(1, 0., 0.5, .5);
	glBegin(GL_POLYGON);
	for(unsigned int i = 0; i < 6; ++i)
	{
		int x, y;

		x = robotClientData.robotInfo.posx()
														+ points[i][0] * cos(robotClientData.robotInfo.theta())
														- points[i][1] * sin(robotClientData.robotInfo.theta());
		y = robotClientData.robotInfo.posy()
														+ points[i][0] * sin(robotClientData.robotInfo.theta())
														+ points[i][1] * cos(robotClientData.robotInfo.theta());

		glVertex2f(x/MAP_FACTOR + getWidth()/2, getHeight()/2 - y/MAP_FACTOR);
	}
	glEnd();
	robotClientData.rInfoMutex.unlock();
}


void RoboGLMap::drawSensors(void)
{
	robotClientData.sInfoMutex.lock();
	for(list<robotdata::SensorInfo *>::iterator it = robotClientData.sensorsInfo.begin();
			it != robotClientData.sensorsInfo.end(); ++it)
	{
		const int num_segments = 10;
		float theta = (*it)->anglespan() / float(num_segments - 1);
		float tangetial_factor = tanf(theta);
		float radial_factor = cosf(theta);
		float x = (*it)->distance() * cosf((*it)->anglecenter() - (*it)->anglespan()/2);
		float y = (*it)->distance() * sinf((*it)->anglecenter() - (*it)->anglespan()/2);

		glColor4f(0, 1, 1, 0.5);
		glBegin(GL_POLYGON);//since the arc is not a closed curve, this is a strip now

		glVertex2f((*it)->offsetxmapmm()/MAP_FACTOR + getWidth()/2,
				getHeight()/2 - (*it)->offsetymapmm()/MAP_FACTOR);

		//	cout << cx << " x " << cy << endl;

		for(int ii = 0; ii < num_segments; ii++)
		{
			glVertex2f(((*it)->offsetxmapmm() + x)/MAP_FACTOR + getWidth()/2,
					getHeight()/2 - ((*it)->offsetymapmm() + y)/MAP_FACTOR);

			float tx = -y;
			float ty = x;

			x += tx * tangetial_factor;
			y += ty * tangetial_factor;

			x *= radial_factor;
			y *= radial_factor;
		}
		glEnd();
	}
	robotClientData.sInfoMutex.unlock();
}

void RoboGLMap::drawMap(void)
{
	robotClientData.mInfoMutex.lock();
	glBegin(GL_POINTS);
	for(google::protobuf::RepeatedPtrField<robotdata::FullMap_MapParticle>::const_iterator it = robotClientData.mapInfo.safeparticles().begin();
			it != robotClientData.mapInfo.safeparticles().end(); ++it)
	{
		int screenX = getWidth()/2 + it->posx()/MAP_FACTOR;
		int screenY = getHeight()/2 - it->posy()/MAP_FACTOR;

		glColor4f(0, 1, 0, it->weight()/40.0);
		glVertex2f(screenX, screenY);
	}
	glEnd();

	glBegin(GL_POINTS);
	for(google::protobuf::RepeatedPtrField<robotdata::FullMap_MapParticle>::const_iterator it = robotClientData.mapInfo.wallparticles().begin();
			it != robotClientData.mapInfo.wallparticles().end(); ++it)
	{
		int screenX = getWidth()/2 + it->posx()/MAP_FACTOR;
		int screenY = getHeight()/2 - it->posy()/MAP_FACTOR;

		glColor4f(1, 0, 0, it->weight()/40.0);
		glVertex2f(screenX, screenY);
	}
	glEnd();
	robotClientData.mInfoMutex.unlock();
}

void RoboGLMap::drawAllChildren(robotdata::FullPath_PathDot * pathNode)
{
	glColor4f(1, 1, .0, .5);
	glBegin(GL_LINES);

	for(google::protobuf::RepeatedPtrField<robotdata::FullPath_PathDot>::const_iterator it = pathNode->nextdot().begin();
			it != pathNode->nextdot().end(); ++it)
	{
		glVertex2f(getWidth()/2 + pathNode->x()/MAP_FACTOR, getHeight()/2 - pathNode->y()/MAP_FACTOR);
		glVertex2f(getWidth()/2 + it->x()/MAP_FACTOR, getHeight()/2 - it->y()/MAP_FACTOR);
		//		cout << node->x << " x " << node->y << " --> " << (*it)->x << " x " <<  (*it)->y << " $ " << (*it)->cost << " + " << (*it)->heuristic << endl ;
	}

	glEnd();

	//	cout << "......................NEXT...................................................................\n";

	for(int i = 0; i < pathNode->nextdot_size(); ++i)
	{
		robotdata::FullPath_PathDot * helper = (robotdata::FullPath_PathDot *)&(pathNode->nextdot(i));
		drawAllChildren(helper);
	}
}

void RoboGLMap::drawPath(void)
{
	robotClientData.pInfoMutex.lock();
	if(robotClientData.pathInfo.has_firstdot())
	{
		drawAllChildren(robotClientData.pathInfo.mutable_firstdot());
	}
	robotClientData.pInfoMutex.unlock();

	//	glColor4f(0, 1, 0, 1);
	//	glBegin(GL_LINES);
	//
	//	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR-5,
	//			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR-5);
	//	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR+5,
	//			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR+5);
	//
	//	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR+5,
	//			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR-5);
	//	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR-5,
	//			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR+5);
	//
	//	glEnd();
}


void RoboGLMap::render( wxPaintEvent& evt )
{
	if(!IsShown()) return;

	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ------------- draw some 2D ----------------
	prepare2DViewport(0,0,getWidth(), getHeight());
	glLoadIdentity();


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background


	drawMap();
	drawSensors();
	drawRobot();
	drawPath();


	glFlush();
	SwapBuffers();
}



#if 0
IMPLEMENT_APP(RoboGuiApp)
#else
wxAppConsole *wxCreateApp()
{
	wxAppConsole::CheckBuildOptions("2" "." "8"
			" (" "no debug" "," "Unicode"
			",compiler with C++ ABI " "1002"
			",wx containers"
			",compatible with 2.6"
			")",
			"your program");
	return new RoboGuiApp;
}
wxAppInitializer
wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp);

RoboGuiApp& wxGetApp()
{
	return *static_cast<RoboGuiApp*>(wxApp::GetInstance());
}



RoboControls::RoboControls(wxWindow* parent, const wxSize& size) : wxPanel( parent , wxID_ANY, wxPoint(0, 0), size)
{
	this->SetBackgroundColour( wxColour(0x00FF00FF) );
	wxButton *helloButton = new wxButton(this, wxID_ANY, wxT("Hello"), wxPoint(0,0), wxSize(60,60));
	wxButton *quitButton = new wxButton(this, wxID_ANY, wxT("Quit"), wxPoint(0,50), wxSize(60,60));

	//wxSpinCtrl *moveCalibrationCtrl = new wxSpinCtrl(this, wxID_ANY, );
}



#endif















