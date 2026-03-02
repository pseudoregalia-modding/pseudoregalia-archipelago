#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Logger.hpp"
#include "Engine.hpp"
#include "StringOps.hpp"

namespace Logger {
	using namespace RC::Output;
	using namespace RC::LogLevel;
	using std::wstring;
	using std::string;

	void Logger::Log(string text, LogType type) {
		Log(StringOps::ToWide(text), type);
	}

	void Logger::Log(wstring text, LogType type) {
		switch (type) {
		case LogType::Console: {
			send<LogLevel::Default>(L"[APRandomizer] Console: " + text + L"\n");
			break;
		}
		
		case LogType::Popup:
			send<LogLevel::Verbose>(L"[APRandomizer] Popup: " + text + L"\n");
			break;

		case LogType::System: {
			send<LogLevel::Verbose>(L"[APRandomizer] System: " + text + L"\n");
			wstring markdown_text = L"<System>" + text + L"</>";
			Engine::PrintToConsole(markdown_text, text);
			break;
		}

		case LogType::Warning: {
			send<LogLevel::Warning>(L"[APRandomizer] WARNING: " + text + L"\n");
			break;
		}

		case LogType::Error: {
			send<LogLevel::Error>(L"[APRandomizer] ERROR: " + text + L"\n");
			wstring markdown_text = L"<Error>Error: " + text + L"</>";
			Engine::PrintToConsole(markdown_text, text);
			break;
		}

		default: {
			send<LogLevel::Default>(L"[APRandomizer] : " + text + L"\n");
			break;
		}
		} // End switch
	}
}