#pragma once

#include <functional>
#include <map>
#include <vector>

#include "vtk_header_files.h"
#include "Qt_header_files.h"

typedef std::function<void(QKeyEvent*)> QtKeyEventHandler;
typedef std::function<void()> voidFunction;

class HEventDispatcher
{
public:
	//HEventDispatcher();

	inline void AddKeyPressEventHandler(Qt::Key key, voidFunction eventHandler) { keyPressEventHandlers[key].push_back(eventHandler); }
	inline void AddKeyReleaseEventHandler(Qt::Key key, voidFunction eventHandler) { keyReleaseEventHandlers[key].push_back(eventHandler); }

	void KeyPresssEvent(QKeyEvent* event) {
		auto key = Qt::Key(event->key());
		if (keyPressEventHandlers.count(key) != 0)
		{
			auto& handlers = keyPressEventHandlers[key];
			for (auto& handler : handlers)
			{
				handler();
			}
		}
	}

	void KeyReleaseEvent(QKeyEvent* event) {
		auto key = Qt::Key(event->key());
		if (keyReleaseEventHandlers.count(key) != 0)
		{
			auto& handlers = keyReleaseEventHandlers[key];
			for (auto& handler : handlers)
			{
				handler();
			}
		}
	}

protected:
	std::map<Qt::Key, std::vector<voidFunction>> keyPressEventHandlers;
	std::map<Qt::Key, std::vector<voidFunction>> keyReleaseEventHandlers;
};
