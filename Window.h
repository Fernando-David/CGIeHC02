#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getArticulacionCofre() { return articulacionCofre; } //Fucion para obtener la articulacion del cofre
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLboolean getPrendida() { return luzPrendida; }
	GLboolean getPrendida2() { return luzPrendida2; }
	GLfloat getMovimientoCoche() { return MovimientoCoche; } //Funcion para obtener la direccion de mocimiento del coche
	GLfloat getLucespuntuales() { return lucespuntuales; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat articulacionCofre; //Articulacion del cofre
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat luzPrendida;
	GLfloat luzPrendida2;
	GLboolean lucespuntuales;
	GLfloat MovimientoCoche; //float para distiguir la direccion del coche
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

