// MowaLibs
//
// Copyright (c) 2011, Marcin Ignac / marcinignac.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// This code is intended to be used with the Cinder C++ library, http://libcinder.org
//
// Temptesta Seven font by Yusuke Kamiyamane http://p.yusukekamiyamane.com/fonts/
// "The fonts can be used free for any personal or commercial projects."

#pragma once

#include <vector>
#include "cinder/app/App.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

using namespace cinder;
using namespace ci::app;

namespace cinder { namespace sgui {
	
	//-----------------------------------------------------------------------------
	
	class Control;
	class FloatVarControl;
	class IntVarControl;
	class BoolVarControl;
	class ButtonControl;
	class LabelControl;
	class SeparatorControl;
	class ColumnControl;
	class PanelControl;
	class TextureVarControl;
	class ColorVarControl;
	class ListVarControl;
	class DropDownVarControl;
	
	//-----------------------------------------------------------------------------
	
	class SimpleGUI {
	private:
		bool enabled;
		Vec2f	mousePos;
		std::vector<Control*> controls;
		Control* selectedControl;
		
		CallbackId	cbMouseDown;
		CallbackId	cbMouseUp;
		CallbackId  cbMouseDrag;	
		
		void	init(App* app);	
	public:
		static ColorA darkColor;
		static ColorA lightColor;
		static ColorA bgColor;
		static ColorA textColor;
		static Vec2f spacing;
		static Vec2f padding;
		static Vec2f radioSize;
		static Vec2f sliderSize;
		static Vec2f labelSize;
		static Vec2f buttonSize;
		static Vec2f buttonGap;
		static Vec2f separatorSize;
		static Vec2f thumbnailSize;
		static Font textFont;
		
		enum {
			RGB,
			HSV
		};
	public:
		SimpleGUI(App* app);
		~SimpleGUI();
		bool	isSelected() { return selectedControl != NULL; }
		std::vector<Control*>& getControls() { return controls; }	
		
		bool	onMouseDown(MouseEvent event);
		bool	onMouseUp(MouseEvent event);
		bool	onMouseDrag(MouseEvent event);
		
		void	draw();
		void	drawGui();	// FBO
		void	dump();
		void	save(std::string fileName = "");
		void	load(std::string fileName = "");
		
		bool	isEnabled();
		void	setEnabled(bool state);
		
		FloatVarControl* 	addParam(const std::string& paramName, float* var, float min=0, float max=1, float defaultValue = 0);
		IntVarControl*		addParam(const std::string& paramName, int* var, int min=0, int max=1, int defaultValue = 0);
		BoolVarControl*		addParam(const std::string& paramName, bool* var, bool defaultValue = false, int groupId = -1);
		ColorVarControl*	addParam(const std::string& paramName, Color* var, Color const defaultValue = ColorA(0, 1, 1, 1), int colorModel = RGB);	// ROGER
		ColorVarControl*	addParam(const std::string& paramName, ColorA* var, ColorA const defaultValue = ColorA(0, 1, 1, 1), int colorModel = RGB);
		TextureVarControl*	addParam(const std::string& paramName, gl::Texture* var, float scale = 1.0, bool flipVert = false);
		
		// ROGER
		LabelControl*		addParam( const std::string &paramName, std::string* var);	// string control
		ListVarControl*		addParamList( const std::string &paramName, int* var, const std::map<int,std::string> &valueLabels );
		ListVarControl*		addParamDropDown( const std::string &paramName, int* var, const std::map<int,std::string> &valueLabels );
		
		ButtonControl*		addButton(const std::string& buttonName);
		LabelControl*		addLabel(const std::string& labelName);
		SeparatorControl*	addSeparator();
		ColumnControl*		addColumn(int x = 0, int y = 0);
		PanelControl*		addPanel(const std::string& panelName="");
		
		static Vec2f		getStringSize(const std::string& str);		
		static Rectf		getScaledWidthRectf(Rectf rect, float scale);
		
		// ROGER
		Control*			getControlByName(const std::string & name);
		int					getColumnWidth()	{ return SimpleGUI::sliderSize.x; };
		bool				hasChanged();
		Vec2f				getSize()			{ return mSize; };
		
		Vec2f				mSize;
		Vec2f				mOffset;
		
	private:
		// ROGER
		bool		bForceRedraw;
		// FBO
		gl::Fbo		mFbo;
		CallbackId	cbResize;
		bool		bUsingFbo;
		bool		bShouldResize;
		DropDownVarControl	*droppedList;
		
		bool		onResize( app::ResizeEvent event );
		
	};
	
	//-----------------------------------------------------------------------------
	
	class Control {
	public:
		enum Type {
			FLOAT_VAR,
			INT_VAR,
			BOOL_VAR,
			COLOR_VAR,
			TEXTURE_VAR,
			LIST_VAR,
			DROP_DOWN_VAR,
			BUTTON,
			LABEL,
			SEPARATOR,
			COLUMN,
			PANEL
		};
		
		// ROGER
		class listVarItem {
		public:
			int				key;
			std::string		label;
			Rectf			activeAreaBase;
			Rectf			activeArea;
		};
		
		
		Vec2f	position;
		Rectf	activeArea;
		ColorA	bgColor;
		Type	type;
		std::string name;
		SimpleGUI *parentGui;
		// ROGER
		bool readOnly;
		Rectf activeAreaBase;
		Rectf backArea;
		Vec2f drawOffset;
		PanelControl *panelToSwitch;
		
		Control();	
		virtual ~Control() {};
		void setBackgroundColor(ColorA color);	
		void notifyUpdateListeners();
		virtual Vec2f draw(Vec2f pos) = 0;
		virtual std::string toString() { return ""; };
		virtual void fromString(std::string& strValue) {};
		virtual void onMouseDown(MouseEvent event) {};
		virtual void onMouseUp(MouseEvent event) {};
		virtual void onMouseDrag(MouseEvent event) {};
		// ROGER
		virtual void setReadOnly(bool b=true)			{ readOnly = b; }
		virtual bool hasChanged()						{ return false; }
		virtual bool hasResized()						{ return false; }
		virtual void switchPanel( PanelControl *p )		{ panelToSwitch = p; };		// Panel to switch ON/OFF with my value
		virtual bool isOn()								{ return true; }			// used to switch panel
	};
	
	//-----------------------------------------------------------------------------
	
	class FloatVarControl : public Control {
	public:	
		float* var;
		float min;
		float max;
		// ROGER
		float lastValue;
		bool displayValue;
		bool formatAsTime;
		int precision;
	public:
		FloatVarControl(const std::string & name, float* var, float min=0, float max=1, float defaultValue = 0);
		float getNormalizedValue();
		void setNormalizedValue(float value);
		Vec2f draw(Vec2f pos);
		std::string toString();
		void fromString(std::string& strValue);
		void onMouseDown(MouseEvent event);	
		void onMouseDrag(MouseEvent event);
		// ROGER
		void update();
		void setReadOnly(bool b=true);
		void setPrecision(int p)			{ precision = p; };
		void setDisplayValue(bool b=true)	{ displayValue=b; }
		void setFormatAsTime(bool b=true)	{ formatAsTime=b; }
		bool hasChanged()					{ return (*var != lastValue); };
		bool isOn()							{ return (*var); }		// used to switch panel
	};
	
	//-----------------------------------------------------------------------------
	
	class IntVarControl : public Control {
	public:
		int* var;
		int min;
		int max;
		// ROGER
		int lastValue;
		bool displayValue;
		int step;
		std::vector<listVarItem> items;

	public:
		IntVarControl(const std::string & name, int* var, int min=0, int max=1, int defaultValue = 0);
		float getNormalizedValue();
		void setNormalizedValue(float value);
		Vec2f draw(Vec2f pos);
		std::string toString();	
		void fromString(std::string& strValue);
		void onMouseDown(MouseEvent event);	
		void onMouseDrag(MouseEvent event);	
		// ROGER
		void update();
		void setStep(int s);
		void setReadOnly(bool b=true);
		void setDisplayValue(bool b=true)	{ displayValue = b; }
		bool hasChanged()					{ return (*var != lastValue); }
		bool isOn()							{ return (*var); }		// used to switch panel
	};
	
	//-----------------------------------------------------------------------------
	
	class BoolVarControl : public Control {
	public:
		bool* var;
		int groupId;
		// ROGER
		bool lastValue;
		std::string nameOff;
	public:
		BoolVarControl(const std::string & name, bool* var, bool defaultValue, int groupId);
		Vec2f draw(Vec2f pos);	
		std::string toString();	
		void fromString(std::string& strValue);
		void onMouseDown(MouseEvent event);
		// ROGER
		bool hasChanged()		{ return (*var != lastValue); };
		bool isOn()				{ return (*var); }		// used to switch panel
	};
	
	//-----------------------------------------------------------------------------
	
	class ColorVarControl : public Control {
	public:
		Color*	var;
		ColorA* varA;
		int		activeTrack;
		int		colorModel;
		// ROGER
		Color	lastValue;
		ColorA	lastValueA;
		bool	alphaControl;
		int		channelCount;
		Rectf	previewArea;
		Rectf	activeAreas[4];
		Rectf	activeAreasBase[4];
	public:
		ColorVarControl(const std::string & name, Color* var, Color defaultValue, int colorModel);		// ROGER
		ColorVarControl(const std::string & name, ColorA* var, ColorA defaultValue, int colorModel);
		Vec2f draw(Vec2f pos);
		std::string toString();	//saved as "r g b a"
		void fromString(std::string& strValue); //expecting "r g b a"
		void onMouseDown(MouseEvent event);	
		void onMouseDrag(MouseEvent event);
		// ROGER
		void update();
		bool hasChanged()		{ return ( alphaControl ? (*varA!=lastValueA) : (*var!=lastValue) ); };
	};
	
	//-----------------------------------------------------------------------------
	// ROGER
	
	class ListVarControl : public Control {
	public:
		int* var;
		std::vector<listVarItem> items;
		int lastValue;
		int lastSize;
	public:
		ListVarControl(const std::string & name, int* var, const std::map<int,std::string> &valueLabels);
		std::string toString();
		void fromString(std::string& strValue);
		void drawHeader(Vec2f pos);
		void drawList(Vec2f pos);
		void update(const std::map<int,std::string> &valueLabels);
		bool isSelected(int i) { return ( items[i].key == *var ); }
		
		virtual void resize();
		virtual Vec2f draw(Vec2f pos);
		virtual void onMouseDown(MouseEvent event);
		virtual bool hasChanged()	{ return (*var != lastValue); }
		virtual bool hasResized()	{ return (items.size() != lastSize); }
		bool isOn()					{ return (*var); }		// used to switch panel
	};
	
	class DropDownVarControl : public ListVarControl {
	public:
		DropDownVarControl(const std::string & name, int* var, const std::map<int,std::string> &valueLabels);
		void resize();
		Vec2f draw(Vec2f pos);
		void onMouseDown(MouseEvent event);
		bool hasResized()		{ return (ListVarControl::hasResized() || lastDropped!=dropped); }
		void open()				{ dropped = true; this->resize(); }
		void close()			{ dropped = false; this->resize(); }
		
		bool dropped;
		bool lastDropped;
		Vec2f dropButtonGap;
		Rectf dropButtonActiveAreaBase, dropButtonActiveArea;
	};
	
	//-----------------------------------------------------------------------------
	
	class ButtonControl : public Control {
	private:
		bool triggerUp;
		bool pressed;
		bool lastPressed;
		CallbackMgr<bool (MouseEvent)>		callbacksClick;
		CallbackId							callbackId;
		// ROGER
		std::string lastName;
	public:
		ButtonControl(const std::string & name);
		Vec2f draw(Vec2f pos);
		void onMouseDown(MouseEvent event);
		void onMouseUp(MouseEvent event);
		void onMouseDrag(MouseEvent event);
		
		//! Registers a callback for Click events. Returns a unique identifier which can be used as a parameter to unregisterClick().
		void	registerClick( std::function<bool (MouseEvent)> callback ) { callbackId = callbacksClick.registerCb( callback ); }
		//! Registers a callback for Click events. Returns a unique identifier which can be used as a parameter to unregisterClick().
		template<typename T>
		void	registerClick( T *obj, bool (T::*callback)(MouseEvent) ) { callbackId = callbacksClick.registerCb( std::bind1st( std::mem_fun( callback ), obj ) ); }
		//! Unregisters a callback for Click events.
		void	unregisterClick() { callbacksClick.unregisterCb( callbackId ); }
		
		void fireClick();
		
		// ROGER
		void setTriggerUp(bool b=true)		{ triggerUp = b; }
		bool hasChanged()					{ return (pressed!=lastPressed || name.compare(lastName)!=0); };
		bool isOn()							{ return (pressed); }		// used to switch panel

	};
	
	//-----------------------------------------------------------------------------
	
	class LabelControl : public Control {
	public:
		LabelControl(const std::string & name);
		LabelControl(const std::string & name, std::string * var);
		void setup();
		void setText(const std::string& text);
		Vec2f draw(Vec2f pos);	
		// ROGER
		std::string * var;
		std::string lastName;
		std::string lastVar;
		bool hasChanged();
	};
	
	//-----------------------------------------------------------------------------
	
	class SeparatorControl : public Control {
	public:
		SeparatorControl();
		Vec2f draw(Vec2f pos);	
	};
	
	//-----------------------------------------------------------------------------
	
	class ColumnControl : public Control {
	public:
		int x;
		int y;
		ColumnControl(int x = 0, int y = 0);
		Vec2f draw(Vec2f pos);	
	}; 
	
	//-----------------------------------------------------------------------------
	
	class PanelControl : public Control {
	public:
		PanelControl(const std::string& panelName="");
		Vec2f draw(Vec2f pos);
		bool enabled;
		// ROGER
		bool lastEnabled;
		bool hasChanged()	{ return (enabled != lastEnabled); };
		bool hasResized()	{ return this->hasChanged(); };
	};
	
	
	
	//-----------------------------------------------------------------------------
	
	class TextureVarControl : public Control {
	public:
		gl::Texture* var;
		float scale;
		bool flipVert;
		TextureVarControl(const std::string & name, gl::Texture* var, float scale, bool flipVert = false);
		Vec2f draw(Vec2f pos);
		// ROGER
		bool resized;
		float refreshRate;
		double refreshTime;
		void resizeTexture();
		bool hasChanged();
		bool hasResized();
		void refresh()			{ refreshTime = 0; }	// force refresh
		bool isOn()				{ return (*var); }		// used to switch panel
	private:
		Vec2i texSize;
	};
	
	//-----------------------------------------------------------------------------
	
} //namespace sgui
} //namespace cinder