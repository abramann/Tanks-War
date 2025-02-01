#include "Window.h"
#include "Values.h"
#include "INI.h"
#include "Types.h"

void IWindow::create()
{
	CINI windowINI(values::WINDOW_INI);

	int fs;
	int w, h;

	windowINI.readInteger("fullscreen", fs);

	windowINI.readInteger("width", w);
	windowINI.readInteger("height", h);

	m_handle = createHandle(w, h, fs, values::GAME_NAME, false);
}

void IWindow::reset()
{
	destroy();
	create();
}

void IWindow::destroy()
{
	deleteHandle(m_handle);

	m_handle = nullptr;
}

HWindow IWindow::getHandle() const
{
	return m_handle;
}
