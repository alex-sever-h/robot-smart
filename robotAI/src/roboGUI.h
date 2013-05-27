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
#include "sensorManagement.h"

class RoboGLMap : public wxGLCanvas
{
    wxGLContext*	m_context;
    wxTimer         m_timer;
    //SensorManager  *sensMan;

public:
	RoboGLMap(wxFrame* parent, int* args);
	virtual ~RoboGLMap();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);
	void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

	void drawArc(float cx, float cy, float start_angle, float arc_angle, float length);

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

	void drawSensors(void);
	void drawMap(void);

	//void setSensMan(SensorManager *sensMan){this->sensMan = sensMan;}

	DECLARE_EVENT_TABLE()
};
#endif


#endif /* MAIN_H_ */
