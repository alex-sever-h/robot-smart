/*
 * main.h
 *
 *  Created on: May 21, 2013
 *      Author: alseh
 */

#ifndef ROBOGUI_H_
#define ROBOGUI_H_

#ifndef _glpane_
#define _glpane_

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "wx/timer.h"
#include "protobuf/robotdata.pb.h"
#include "RobotClient.hpp"
#include "RobotClientData.hpp"


//class RoboControls: public wxPanel
//{
//public:
//	RoboControls(wxWindow *parent, const wxSize &size);
//};


class RoboGLMap : public wxGLCanvas
{
	wxGLContext*	m_context;
	wxTimer         m_timer;

	RobotClient robotClient;
	RobotClientData robotClientData;

public:
	RoboGLMap(wxFrame* parent, int* args);
	virtual ~RoboGLMap();

	void resized(wxSizeEvent& evt);

	unsigned int getWidth();
	unsigned int getHeight();

	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	void timerUpdate(wxTimerEvent & event);

	void drawAllChildren(robotdata::FullPath_PathDot * pathNode);
	void drawSensors(void);
	void drawMap(void);
	void drawRobot(void);
	void drawPath(void);

	//void setSensMan(SensorManager *sensMan){this->sensMan = sensMan;}

	void drawTarget();

	DECLARE_EVENT_TABLE()
};
#endif


#endif /* MAIN_H_ */
