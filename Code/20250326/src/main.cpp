#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"

#include <soil2/SOIL2.h>   //
#include "Sphere.h"		   //
#include "ImportedModel.h" //

using namespace std;

#define numVAOs 1
// #define numVBOs 2
#define numVBOs 4 //

#define spherePrecision 1024 //

float cameraX, cameraY, cameraZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
// GLuint vbo[numVBOs];
GLuint vbo[numVBOs * 3]; //

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

stack<glm::mat4> mvStack;

Sphere sunSphere(spherePrecision), planetSphere(spherePrecision), moonSphere(spherePrecision); //
ImportedModel shuttle("assets/models/shuttle.obj");											   //
GLuint sunTexture, planetTexture, moonTexture, shuttleTexture;								   //

/*
 */
bool setupSphereBuffers(Sphere &sphere, GLuint *vboBase)
{
	vector<int> ind = sphere.getIndices();
	vector<glm::vec3> vert = sphere.getVertices();
	vector<glm::vec2> tex = sphere.getTexCoords();
	vector<glm::vec3> norm = sphere.getNormals();

	vector<float> pvalues, tvalues, nvalues;
	int numIndices = sphere.getNumIndices();
	for (int i = 0; i < numIndices; i++)
	{
		pvalues.push_back(vert[ind[i]].x);
		pvalues.push_back(vert[ind[i]].y);
		pvalues.push_back(vert[ind[i]].z);
		tvalues.push_back(tex[ind[i]].x);
		tvalues.push_back(tex[ind[i]].y);
		nvalues.push_back(norm[ind[i]].x);
		nvalues.push_back(norm[ind[i]].y);
		nvalues.push_back(norm[ind[i]].z);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vboBase[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, pvalues.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboBase[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, tvalues.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboBase[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, nvalues.data(), GL_STATIC_DRAW);
	return true;
}

void setupVBOs(void)
{
	// float vertexPositions[108] =
	// 	{-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	// 	 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
	// 	 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	// 	 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	// 	 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	 -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	// 	 -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	// 	 -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	// 	 -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
	// 	 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
	// 	 -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	// 	 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
	// float pyramidPositions[54] =
	// 	{
	// 		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// front
	// 		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// right
	// 		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// back
	// 		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// left
	// 		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // LF
	// 		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f	// RR
	// 	};

	// //
	// float octahedronPositions[74] =
	// 	{
	// 		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// 		0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// 		-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// 		0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	// 		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	// 		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,
	// 		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
	// 		0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f};
	vector<glm::vec3> shuttleVert = shuttle.getVertices();
	vector<glm::vec2> shuttleTex = shuttle.getTextureCoords();
	vector<glm::vec3> shuttleNorm = shuttle.getNormals();

	vector<float> pvalues, tvalues, nvalues;
	for (int i = 0; i < shuttle.getNumVertices(); ++i)
	{
		pvalues.push_back(shuttleVert[i].x);
		pvalues.push_back(shuttleVert[i].y);
		pvalues.push_back(shuttleVert[i].z);
		tvalues.push_back(shuttleTex[i].x);
		tvalues.push_back(shuttleTex[i].y);
		nvalues.push_back(shuttleNorm[i].x);
		nvalues.push_back(shuttleNorm[i].y);
		nvalues.push_back(shuttleNorm[i].z);
	}

	// glGenVertexArrays(1, vao);
	// glBindVertexArray(vao[0]);
	// glGenBuffers(numVBOs, vbo);

	// glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs * 3, vbo);

	setupSphereBuffers(sunSphere, &vbo[0 * 3]);
	setupSphereBuffers(planetSphere, &vbo[1 * 3]);
	setupSphereBuffers(moonSphere, &vbo[2 * 3]);

	/*
	 */
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3 * 3]);										   //
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, pvalues.data(), GL_STATIC_DRAW); //
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3 * 3 + 1]);									   //
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, tvalues.data(), GL_STATIC_DRAW); //
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3 * 3 + 2]);									   //
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, nvalues.data(), GL_STATIC_DRAW); //
																					   // glBufferData(GL_ARRAY_BUFFER, sizeof(octahedronPositions), octahedronPositions, GL_STATIC_DRAW); //
}

void init(GLFWwindow *window)
{
	// renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	renderingProgram = Utils::createShaderProgram("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl"); //

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 12.0f;
	setupVBOs();

	sunTexture = Utils::loadTexture("assets/images/sunmap.jpg");	   //
	planetTexture = Utils::loadTexture("assets/images/earth.jpg");	   //
	moonTexture = Utils::loadTexture("assets/images/moon.jpg");		   //
	shuttleTexture = Utils::loadTexture("assets/images/spstob_1.jpg"); //
}

void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// // ----------------------  pyramid == sun
	// ---------------------- sun ----------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0 * 3]); //
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0 * 3 + 1]);		   //
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //
	glEnableVertexAttribArray(1);						   //
	glActiveTexture(GL_TEXTURE0);						   //
	glBindTexture(GL_TEXTURE_2D, sunTexture);			   //
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// glDrawArrays(GL_TRIANGLES, 0, 18);
	glDrawArrays(GL_TRIANGLES, 0, sunSphere.getNumIndices()); //
	mvStack.pop();

	// //-----------------------  cube == planet
	//----------------------- planet ----------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1 * 3]); //
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1 * 3 + 1]);		   //
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //
	glEnableVertexAttribArray(1);						   //
	glActiveTexture(GL_TEXTURE0);						   //
	glBindTexture(GL_TEXTURE_2D, planetTexture);		   //
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, planetSphere.getNumIndices()); //
	mvStack.pop();

	// //-----------------------  smaller cube == moon
	//----------------------- moon ----------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2 * 3]); //
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2 * 3 + 1]);		   //
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //
	glEnableVertexAttribArray(1);						   //
	glActiveTexture(GL_TEXTURE0);						   //
	glBindTexture(GL_TEXTURE_2D, moonTexture);			   //
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawArrays(GL_TRIANGLES, 0, moonSphere.getNumIndices()); //
	mvStack.pop();

	/*
	 */
	mvStack.pop();
	mvStack.push(mvStack.top());
	auto orgZ = glm::vec3(0.0f, 1.0f, 0.0f), transZ = glm::vec3(-1.0f, -1.0f, -1.0f);
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), acos(glm::dot(orgZ, transZ) / ((glm::length(orgZ)) * (glm::length(transZ)))), glm::cross(orgZ, transZ));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)(currentTime * 0.85), orgZ);
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 0.0f, 0.0f));
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)(currentTime * 0.85 * 3), glm::vec3(0.0, 0.0, 1.0));
	// mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime * 0.85f, glm::vec3(1.0, 1.0, 0.0));
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.9f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3 * 3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3 * 3 + 1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shuttleTexture);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, shuttle.getNumVertices());
	mvStack.pop();

	mvStack.pop();
	mvStack.pop(); // the final pop is for the view matrix
}

void window_size_callback(GLFWwindow *win, int newWidth, int newHeight)
{
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void)
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// GLFWwindow *window = glfwCreateWindow(600, 600, "Chapter 4 - program 4", NULL, NULL);
	GLFWwindow *window = glfwCreateWindow(800, 600, "Lab3: Space & shuttle", NULL, NULL); //
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
