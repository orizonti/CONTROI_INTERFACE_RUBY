#ifndef KEY_EVENT_ENGINE_H
#define KEY_EVENT_ENGINE_H
#include <QObject>
#include <QKeyEvent>

class KeyboardFilter : public QObject
 {
    Q_OBJECT
public:
  static bool KeyControlPressed;
  static bool KeyAltPressed    ;
  static bool KeyShiftPressed    ;

  static bool isControlPressed() { return KeyControlPressed; }
  static bool isAltPressed() { return KeyControlPressed; }
  static bool isShiftPressed() { return KeyControlPressed; }
  static bool isControlAltPressed() { return KeyControlPressed && KeyAltPressed; }
signals: 
void signalControlAlt();
void signalRelease();
protected:

  bool eventFilter(QObject* obj, QEvent* event)
  {
       auto pressed = false;
            pressed = event->type() == QEvent::KeyPress;

		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		switch(key->key())
		{
		case Qt::Key_Control: KeyControlPressed = pressed; break; 
		case Qt::Key_Alt:     KeyAltPressed     = pressed; break;  
		case Qt::Key_Shift:   KeyShiftPressed   = pressed; break;  
		default: return QObject::eventFilter(obj, event);
		}

		if( isControlAltPressed()) emit signalControlAlt();
		if(!isControlAltPressed()) emit signalRelease();

	    return QObject::eventFilter(obj, event);
  }
};

#endif 
