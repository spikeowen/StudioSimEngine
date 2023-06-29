#include "pch.h"

#include "KeyboardClass.h"

//using namespace std;


bool KeyboardClass::s_AutoRepeatKeys;
bool KeyboardClass::s_AutoRepeatChars;
bool KeyboardClass::s_KeyStates[256];
std::queue<KeyEvent> KeyboardClass::s_KeyBuffer;
std::queue<unsigned char> KeyboardClass::s_CharBuffer;

void KeyboardClass::Init()
{
	s_AutoRepeatChars = true;
	s_AutoRepeatKeys = true;

}

KeyEvent KeyboardClass::ReadKey()
{
	if (s_KeyBuffer.empty())
	{
		// return empty key event if no event is in the queue
		return KeyEvent();
	}
	else
	{
		// retreive the first event and remove it from the queue, after this return the event.
		KeyEvent e = s_KeyBuffer.front();
		s_KeyBuffer.pop();
		return e;
	}

}

unsigned char KeyboardClass::ReadChar()
{
	if (s_CharBuffer.empty())
	{
		// return empty key event if no event is in the queue
		return 0u;
	}
	else
	{
		// retreive the first event and remove it from the queue, after this return the event.
		unsigned char c = s_CharBuffer.front();
		s_CharBuffer.pop();
		return c;
	}
}


vector<KeyEvent> KeyboardClass::ReadKeys()
{
	vector<KeyEvent> events;
	int size = s_KeyBuffer.size();
	for (int i = 0; i < size; i++)
	{
		events.push_back(s_KeyBuffer.front());
		s_KeyBuffer.pop();
	}
	return events;
}

vector<unsigned char> KeyboardClass::ReadChars()
{
	vector<unsigned char> events;
	int size = s_CharBuffer.size();
	for (int i = 0; i < size; i++)
	{
		events.push_back(s_CharBuffer.front());
		s_CharBuffer.pop();
	}
	return events;
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	s_KeyStates[key] = true;
	s_KeyBuffer.push(KeyEvent(KeyEvent::EventType::PRESS, key));


}

void KeyboardClass::UpdateKeyStates()
{
	for (int i = 0; i < 256; i++)
	{
		if (s_KeyStates[i] == true)
		{
			s_KeyBuffer.push(KeyEvent(KeyEvent::EventType::HELD, i));
		}
	}
}

void KeyboardClass::OnKeyHeld(const unsigned char key)
{
	s_KeyStates[key] = true;
	s_KeyBuffer.push(KeyEvent(KeyEvent::EventType::HELD, key));
}


void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	s_KeyStates[key] = false;
	s_KeyBuffer.push(KeyEvent(KeyEvent::EventType::RELEASE, key));
}

void KeyboardClass::OnCharInput(const unsigned char key)
{
	s_CharBuffer.push(key);
}

void KeyboardClass::ClearKeyBuffer()
{
	Clear(s_KeyBuffer);
	Clear(s_CharBuffer);
}

void KeyboardClass::Clear(std::queue<KeyEvent>& queue)
{
	std::queue<KeyEvent> empty;
	std::swap(queue, empty);
}

void KeyboardClass::Clear(std::queue<unsigned char>& queue)
{
	std::queue<unsigned char> empty;
	std::swap(queue, empty);
}