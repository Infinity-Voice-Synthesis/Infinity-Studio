﻿#pragma once

#include "Lua/lua.hpp"
#include <JuceHeader.h>

class ILLibs
{
	static std::function<void(const juce::String&)> console_mesFunction;
	static std::function<void(const juce::String&)> console_errFunction;
	static std::function<void()> console_clsFunction;

	static std::function<bool(const juce::String&)> thread_finFunction;
	static std::function<juce::StringArray()> thread_lstFunction;
	static std::function<bool(const juce::String&)> thread_creFunction;
	static std::function<bool(const juce::String&)> thread_rmvFunction;
	static std::function<bool(const juce::String&)> thread_chkFunction;
	static std::function<bool(const juce::String&)> thread_desFunction;
	static std::function<bool(const juce::String&, const juce::String&)> thread_runFunction;
	static std::function<bool(const juce::String&, const juce::String&)> thread_execFunction;
	static std::function<void()> thread_fluFunction;

	static std::function<void* (const juce::String&, const juce::String&, size_t)> share_creFunction;
	static std::function<bool(const juce::String&, const juce::String&)> share_finFunction;
	static std::function<bool(const juce::String&, const juce::String&)> share_rmvFunction;
	static std::function<size_t(const juce::String&, const juce::String&)> share_sizFunction;
	static std::function<void* (const juce::String&, const juce::String&)> share_getFunction;
	static std::function<bool(const juce::String&)> share_cleFunction;
	static std::function<juce::StringArray(const juce::String&)> share_lstFunction;
	static std::function<void(const juce::String&)> share_lckFunction;
	static std::function<void(const juce::String&)> share_ulkFunction;

public:
	static void reg_mesFunctions(
		std::function<void(const juce::String&)> console_mesFunction,
		std::function<void(const juce::String&)> console_errFunction,
		std::function<void()> cconsole_lsFunction
	);
	static void reg_thrFunctions(
		std::function<bool(const juce::String&)> thread_finFunction,
		std::function<juce::StringArray()> thread_lstFunction,
		std::function<bool(const juce::String&)> thread_creFunction,
		std::function<bool(const juce::String&)> thread_rmvFunction,
		std::function<bool(const juce::String&)> thread_chkFunction,
		std::function<bool(const juce::String&)> thread_desFunction,
		std::function<bool(const juce::String&, const juce::String&)> thread_runFunction,
		std::function<bool(const juce::String&, const juce::String&)> thread_execFunction,
		std::function<void()> thread_fluFunction
	);
	static void reg_shrFunctions(
		std::function<void* (const juce::String&, const juce::String&, size_t)> share_creFunction,
		std::function<bool(const juce::String&, const juce::String&)> share_finFunction,
		std::function<bool(const juce::String&, const juce::String&)> share_rmvFunction,
		std::function<size_t(const juce::String&, const juce::String&)> share_sizFunction,
		std::function<void* (const juce::String&, const juce::String&)> share_getFunction,
		std::function<bool(const juce::String&)> share_cleFunction,
		std::function<juce::StringArray(const juce::String&)> share_lstFunction,
		std::function<void(const juce::String&)> share_lckFunction,
		std::function<void(const juce::String&)> share_ulkFunction
	);

	static int infinity_runtime_scriptPath(lua_State* state);
	static int infinity_runtime_scriptDir(lua_State* state);
	static int infinity_runtime_appPath(lua_State* state);
	static int infinity_runtime_appDir(lua_State* state);
	static int infinity_runtime_msleep(lua_State* state);

	static int infinity_console_println(lua_State* state);
	static int infinity_console_error(lua_State* state);
	static int infinity_console_cls(lua_State* state);

	static int infinity_thread_current(lua_State* state);
	static int infinity_thread_find(lua_State* state);
	static int infinity_thread_list(lua_State* state);
	static int infinity_thread_create(lua_State* state);
	static int infinity_thread_remove(lua_State* state);
	static int infinity_thread_check(lua_State* state);
	static int infinity_thread_destory(lua_State* state);
	static int infinity_thread_run(lua_State* state);
	static int infinity_thread_exec(lua_State* state);
	static int infinity_thread_flush(lua_State* state);

	static int infinity_thread_share_create(lua_State* state);
	static int infinity_thread_share_find(lua_State* state);
	static int infinity_thread_share_remove(lua_State* state);
	static int infinity_thread_share_size(lua_State* state);
	static int infinity_thread_share_get(lua_State* state);
	static int infinity_thread_share_clear(lua_State* state);
	static int infinity_thread_share_list(lua_State* state);
	static int infinity_thread_share_lock(lua_State* state);
	static int infinity_thread_share_unlock(lua_State* state);
};

