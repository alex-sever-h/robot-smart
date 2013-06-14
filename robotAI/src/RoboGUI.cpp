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

#define MAP_FACTOR 5

extern RobotModel physicalRobot;
extern MapParticle   world;


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

	physicalRobot.moveAtLocation(newTarget);
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
	int points[6][2] = {
			{+physicalRobot.getLengthMm()/2 + 20, 0 },
			{+physicalRobot.getLengthMm()/2,      -physicalRobot.getWidthMm()/2},
			{-physicalRobot.getLengthMm()/2,      -physicalRobot.getWidthMm()/2},
			{-physicalRobot.getLengthMm()/2 + 20, 0},
			{-physicalRobot.getLengthMm()/2,      +physicalRobot.getWidthMm()/2},
			{+physicalRobot.getLengthMm()/2,      +physicalRobot.getWidthMm()/2}
	};


	glColor4f(1, 0., 0.5, .5);
		glBegin(GL_POLYGON);
	for(unsigned int i = 0; i < 6; ++i)
	{
		int x, y;

		x = physicalRobot.getPositionXmm()
				+ points[i][0] * cos(physicalRobot.getOrientationRad())
				- points[i][1] * sin(physicalRobot.getOrientationRad());
		y = physicalRobot.getPositionYmm()
				+ points[i][0] * sin(physicalRobot.getOrientationRad())
				+ points[i][1] * cos(physicalRobot.getOrientationRad());

		glVertex2f(x/MAP_FACTOR + getWidth()/2, getHeight()/2 - y/MAP_FACTOR);
	}
	glEnd();
}

void RoboGLMap::drawSensors(void)
{
	vector<tyPolygon *> *sSafeAreas = physicalRobot.getSensorSafeAreas();
	vector<tyPolygon *> *sWallAreas = physicalRobot.getSensorWallAreas();
	vector<LocationWWeight> *poly;

	for(unsigned int si = 0; si < sSafeAreas->size(); ++si)
	{
		poly = sSafeAreas->at(si);
		if(poly)
		{
			glColor4f(0, 1, 1, 0.5);
			glBegin(GL_POLYGON);//since the arc is not a closed curve, this is a strip now
			for(unsigned int i = 0; i < poly->size(); ++i)
			{
				glVertex2f(poly->at(i).x/MAP_FACTOR + getWidth()/2, getHeight()/2 - poly->at(i).y/MAP_FACTOR);
			}
			glEnd();
		}
	}
	for(unsigned int si = 0; si < sWallAreas->size(); ++si)
	{
		poly = sWallAreas->at(si);
		if(poly)
		{
			glColor4f(1, 0, 0, 1);
			glBegin(GL_LINE);//since the arc is not a closed curve, this is a strip now
			for(unsigned int i = 0; i < poly->size(); ++i)
			{
				glVertex2f(poly->at(i).x/MAP_FACTOR + getWidth()/2, getHeight()/2 - poly->at(i).y/MAP_FACTOR);
			}
			glEnd();
		}
	}

	delete sSafeAreas;
	delete sWallAreas;
}

void RoboGLMap::drawMap(void)
{
	const list<LocationWWeight>* mapParticles;

	mapParticles = world.getSafeParticleList();
	glBegin(GL_POINTS);
	for(list<LocationWWeight>::const_iterator it = mapParticles->begin(); it != mapParticles->end(); ++it)
	{
		int screenX = getWidth()/2 + it->x/MAP_FACTOR;
		int screenY = getHeight()/2 - it->y/MAP_FACTOR;

		glColor4f(0, 1, 0, it->weight/40.0);
		glVertex2f(screenX, screenY);
	}
	glEnd();

	mapParticles = world.getWallParticleList();
	glBegin(GL_POINTS);
	for(list<LocationWWeight>::const_iterator it = mapParticles->begin(); it != mapParticles->end(); ++it)
	{
		int screenX = getWidth()/2 + it->x/MAP_FACTOR;
		int screenY = getHeight()/2 - it->y/MAP_FACTOR;

		glColor4f(1, 0, 0, it->weight/40.0);
		glVertex2f(screenX, screenY);
	}
	glEnd();
}

void RoboGLMap::drawAllChildren(PathNode * node)
{
	glColor4f(1, 1, .0, .5);
	glBegin(GL_LINES);
	for(list<PathNode *>::iterator it = node->children.begin(); it != node->children.end(); ++it)
	{
		glVertex2f(getWidth()/2 + node->x/MAP_FACTOR, getHeight()/2 - node->y/MAP_FACTOR);
		glVertex2f(getWidth()/2 + (*it)->x/MAP_FACTOR, getHeight()/2 - (*it)->y/MAP_FACTOR);
		//		cout << node->x << " x " << node->y << " --> " << (*it)->x << " x " <<  (*it)->y << " $ " << (*it)->cost << " + " << (*it)->heuristic << endl ;
	}
	glEnd();

	//	cout << "......................NEXT...................................................................\n";

	for(list<PathNode *>::iterator it = node->children.begin(); it != node->children.end(); ++it)
	{
		drawAllChildren(*it);
	}
}

void RoboGLMap::drawPath(void)
{
	if(physicalRobot.path)
	{
		drawAllChildren(physicalRobot.path);
	}

	glColor4f(0, 1, 0, 1);
	glBegin(GL_LINES);

	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR-5,
			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR-5);
	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR+5,
			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR+5);

	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR+5,
			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR-5);
	glVertex2f(getWidth()/2 + physicalRobot.getTarget().x/MAP_FACTOR-5,
			getHeight()/2 - physicalRobot.getTarget().y/MAP_FACTOR+5);

	glEnd();
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















