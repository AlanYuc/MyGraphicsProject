#include "../NCLGL/window.h"
#include "Renderer.h"
#include <algorithm>

int main()	{
	//最后一个参数换成true就是全屏
	Window w("Make your own project!", 1920, 1080, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J)) {
			renderer.ToggleScene();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_K)) {
			renderer.ToggleAutoCamera();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L)) {
			renderer.ToggleGaussianBlur();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_I)) {
			renderer.ToggleDeffered();
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_U)) {
			renderer.ToggleShowTutorialSample();
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
			renderer.RotateDirLight(10.0f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
			renderer.RotateDirLight(-10.0f);
		}
	}
	return 0;
}