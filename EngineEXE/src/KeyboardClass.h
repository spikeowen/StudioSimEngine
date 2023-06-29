#pragma once
#include "KeyEvent.h"
#include <vector>

using namespace std;

class KeyboardClass
{
private:
	static bool s_AutoRepeatKeys;
	static bool s_AutoRepeatChars;
	static bool s_KeyStates[256];
	static std::queue<KeyEvent> s_KeyBuffer;
	static std::queue<unsigned char> s_CharBuffer;
public:
	static void Init();
	static inline bool IsKeyPressed(const unsigned char keycode) { return s_KeyStates[keycode]; }
	static inline bool KeyBufferIsEmpty() { return s_KeyBuffer.empty(); }
	static inline bool CharBufferIsEmpty() { return s_CharBuffer.empty(); }
	static KeyEvent ReadKey();
	static unsigned char ReadChar();

	static vector<KeyEvent> ReadKeys();
	static vector<unsigned char> ReadChars();


	static void OnKeyPressed(const unsigned char key);
	static void UpdateKeyStates();
	static void OnKeyHeld(const unsigned char key);
	static void OnKeyReleased(const unsigned char key);
	static void OnCharInput(const unsigned char key);
	static inline void EnableAutoRepeatKeys() { s_AutoRepeatKeys = true; }
	static inline void DisableAutoRepeatKeys() { s_AutoRepeatKeys = false; }
	static inline void EnableAutoRepeatChars() { s_AutoRepeatChars = true; }
	static inline void DisableAutoRepeatChars() { s_AutoRepeatChars = false; }
	static void ClearKeyBuffer();
	static void Clear(std::queue<KeyEvent>& queue);
	static void Clear(std::queue<unsigned char>& queue);
	static inline bool IsKeyAutoRepeat() { return s_AutoRepeatKeys; }
	static inline bool IsCharsAutoRepeat() { return s_AutoRepeatChars; }
};