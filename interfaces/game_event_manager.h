#pragma once
#include <cstddef>
#include <utils\Vector.h>

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13
class i_game_event
{
public:
	virtual ~i_game_event() {};
	virtual const char* get_name() const = 0; // get event name
	virtual bool IsReliable() const = 0; // if event handled reliable
	virtual bool is_local() const = 0; // if event is never networked
	virtual bool is_empty(const char* keyName = NULL) = 0; // check if data field exists														  // Data access
	virtual bool get_bool(const char* keyName = NULL, bool defaultValue = false) = 0;
	virtual int get_int(const char* keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long long get_uint64(char const* keyName = NULL, unsigned long long defaultValue = 0) = 0;
	virtual float get_float(const char* keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* get_string(const char* keyName = NULL, const char* defaultValue = "") = 0;
	virtual const wchar_t* get_wide_string(char const* keyName = NULL, const wchar_t* defaultValue = L"") = 0;
	virtual void set_bool(const char* keyName, bool value) = 0;
	virtual void set_int(const char* keyName, int value) = 0;
	virtual void set_uint64(const char* keyName, unsigned long long value) = 0;
	virtual void set_float(const char* keyName, float value) = 0;
	virtual void set_string(const char* keyName, const char* value) = 0;
	virtual void set_w_string(const char* keyName, const wchar_t* value) = 0;
};
class i_game_event_listener
{
public:
	virtual ~i_game_event_listener() {}
	virtual void FireGameEvent(i_game_event* Event) = 0;//main thread
	virtual int GetEventDebugID()
	{
		return 42;
	}
};
class IGameEventManager
{
public:
	virtual int _unk0(int* dwUnknown) = 0;
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;//removes all 
	virtual bool add_listener(i_game_event_listener* listener, const char* name, bool bServerSide) = 0;
	virtual bool find_listener(i_game_event_listener* listener, const char* name) = 0;
	virtual int remove_listener(i_game_event_listener* listener) = 0;
	virtual i_game_event* CreateEvent(const char* name, bool bForce, unsigned int dwUnknown) = 0;
	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent(i_game_event* event, bool bDontBroadcast = false) = 0;
	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide(i_game_event* event) = 0;
	// create a new copy of this event, must be free later
	virtual i_game_event* DuplicateEvent(i_game_event* event) = 0;
	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent(i_game_event* event) = 0;
	// write/read event to/from bitbuffer
	virtual bool SerializeEvent(i_game_event* event, void* buf) = 0;
	// create new KeyValues, must be deleted
	virtual i_game_event* UnserializeEvent(void* buf) = 0;
};

