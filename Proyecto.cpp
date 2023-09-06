#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float spotlightOn = 0.0f;
float lightChanger = 0.0f;
float farolLight = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fabric Tokyo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram C:\Users\Dan\OneDrive\Desktop\proyecto
// ------------------------------------
	Shader lightingShader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");
	//Shader skyboxShader("C:/Users/Alpha/Documents/Visual Studio 2022/OpenGL/OpenGl/shaders/skyVertexShader.vs", "C:/Users/Alpha/Documents/Visual Studio 2022/OpenGL/OpenGL/shaders/skyFragmentShader.fs");

	Model land("model/land/land.obj");
	Model floor("model/sky/sky.obj");
	Model monster("model/monster/monster.obj");
	Model center("model/motel/SH_BUILDING_13.obj");
	Model lamp("model/lamp/lamp.obj");
	Model building("model/building/SH_BUILDING_15.obj");
	Model piso("model/building/piso.obj");

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(17.0f, 5.0f, 10.0f), // motel
		glm::vec3(-2.8f, -0.1f, 0.4f),//monster
		glm::vec3(0.0f, 20.0f, 0.0f), // moon
		glm::vec3(34.4613f, 1.84322f, -37.4228f), //lamp1
		glm::vec3(37.0531f, 1.84322f, -31.8647f), //lamp2 
		glm::vec3(39.9037f, 1.84322f, -25.7515f), //lamp3
		glm::vec3(42.4669f, 1.84322f, -20.2548f), //lamp4
		glm::vec3(47.1236f, 1.84322f, -10.2684f) //lamp5

	};


	// shader configuration
	// --------------------
	lightingShader.use();

	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 30);


	glm::vec3 pointLightColors[] = {
	  glm::vec3(0.6f, 0.1f, 0.9f),
	  glm::vec3(0.6f, 0.1f, 0.9f), //lamp1
	  glm::vec3(1.0f, 1.0f, 0.0f), //lamp2 

	};


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();


		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 70.0f);


		//directional light: ambient light
		lightingShader.setVec3("dirLight.direction", 0.1f, 1.0f, 0.2f);
		lightingShader.setVec3("pointLights[0].ambient", 20.0, 20.0, 20.0);
		lightingShader.setVec3("dirLight.diffuse", 0.28f, 0.28f, 0.28f);
		lightingShader.setVec3("dirLight.specular", 0.28f, 0.28f, 0.28f);


		// point light 0: Motel
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.16, pointLightColors[0].y * 0.15, pointLightColors[0].z * 0.16);
		lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(farolLight));
		lightingShader.setVec3("pointLights[4].specular", glm::vec3(farolLight));
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.18);
		lightingShader.setFloat("pointLights[0].quadratic", 0.07);


		// point light 1: water canal
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.15, pointLightColors[1].y * 0.15, pointLightColors[1].z * 0.15);
		lightingShader.setVec3("pointLights[1].diffuse", lightChanger + 0.6f, lightChanger + 0.4f, 0.0f);
		lightingShader.setVec3("pointLights[1].specular", lightChanger + 0.6f, lightChanger + 0.4f, 0.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.35);
		lightingShader.setFloat("pointLights[1].quadratic", 0.44);



		// point light 3: ligth 1
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.08, pointLightColors[2].y * 0.08, pointLightColors[2].z * 0.08);
		lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x + 0.2f, pointLightColors[2].y + 0.2f, pointLightColors[2].z + 0.2f);
		lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x + 0.2f, pointLightColors[2].y + 0.2f, pointLightColors[2].z + 0.2f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.07);
		lightingShader.setFloat("pointLights[2].quadratic", 0.017);

		// point light 4: ligth 2
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[4]);
		lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.08, pointLightColors[2].y * 0.08, pointLightColors[2].z * 0.08);
		lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x + 0.2f, pointLightColors[2].y + 0.2f, pointLightColors[2].z + 0.2f);
		lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x + 0.2f, pointLightColors[2].y + 0.2f, pointLightColors[2].z + 0.2f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.07);
		lightingShader.setFloat("pointLights[3].quadratic", 0.017);

		//point light 2 - moon
		lightingShader.setVec3("pointLights[4].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[4].ambient", pointLightColors[0].x * 0.08, pointLightColors[0].y * 0.08, pointLightColors[0].z * 0.08);
		lightingShader.setVec3("pointLights[4].diffuse", pointLightColors[0].x + 0.7f, pointLightColors[0].y + 0.7f, pointLightColors[0].z + 0.7f);
		lightingShader.setVec3("pointLights[4].specular", pointLightColors[0].x + 0.7f, pointLightColors[0].y + 0.7f, pointLightColors[0].z + 0.7f);
		lightingShader.setFloat("pointLights[4].constant", 1.0f);
		lightingShader.setFloat("pointLights[4].linear", 0.09);
		lightingShader.setFloat("pointLights[4].quadratic", 0.032);


		// spotLight: lantern
		lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
		lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", glm::vec3(spotlightOn));
		lightingShader.setVec3("spotLight.specular", glm::vec3(spotlightOn));
		lightingShader.setFloat("spotLight.constant", 0.8f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);


		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);



		model = glm::mat4(1.0f);

		float changeCoord = sin(glfwGetTime());
		float changeCoord2 = cos(glfwGetTime());
		float angle = glfwGetTime() * 25.0f;


		// RENDER FABRIC
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.3f, -0.1f, 1.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		land.Draw(lightingShader);

		// RENDER GENGAR
		//glm::mat4 model10 = glm::mat4(1.0f);
		//model10 = glm::translate(model10, glm::vec3(-3.0f, changeCoord * 6.0f, -40.0f));
		//model10 = glm::scale(model10, glm::vec3(0.6f, 0.6f, 0.6f));
		//model10 = glm::rotate(model10, glm::radians(angle * 1.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightingShader.setMat4("model", model10);
		//gengar.Draw(lightingShader);

		// RENDER MONSTER

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.8f, 1.1f, 2.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		monster.Draw(lightingShader);

		// RENDER MIMIKYU
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-15.0f, -0.2f, -0.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightingShader.setMat4("model", model);
		//mimikyu.Draw(lightingShader);

		// RENDER MOTEL
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(17.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(115.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		center.Draw(lightingShader);

		// RENDER floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);;
		floor.Draw(lightingShader);

		// RENDER building

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(47.934f, 0.0f, -26.135f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		building.Draw(lightingShader);

		// PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(26.6119f, -0.1f, -52.1805f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		piso.Draw(lightingShader);


		// RENDER LAMPS

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(34.4528f, 0.0f, -37.4543));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		lamp.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(37.0543f, 0.0f, -31.8753f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		lamp.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(39.8917f, 0.0f, -25.7906f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		lamp.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(42.463f, 0.0f, -20.2762f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		lamp.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(47.1271f, 0.0f, -10.2742f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		lamp.Draw(lightingShader);

		// RENDER TREVENANT
		float timeValue = glfwGetTime() * 3;
		float dinamicValue1 = cos(timeValue) * 4.0f / 1000;
		if (dinamicValue1 < 0.003) {
			dinamicValue1 = 0.003;
		}
		//
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-14.0f, -0.6f, 6.0f));
		//model = glm::scale(model, glm::vec3(dinamicValue1 * 180.0f));
		//model = glm::rotate(model, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightingShader.setMat4("model", model);
		//trevenant.Draw(lightingShader);

		// RENDER GASTLY

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-40.0f, changeCoord * 1.3f + 3.0f, changeCoord2 * -20.0f));
		//model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		//lightingShader.setMat4("model", model);
		//gastly.Draw(lightingShader);

		// RENDER SKY
		////model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 00.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));;
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//sky.Draw(lightingShader);

		float dinamic = cos(timeValue) * 0.5f;
		if (dinamic < 0) {
			dinamic = dinamic * -1.0f;
		}
		lightChanger = dinamic;

		int randNum = rand() % (500 - 0 + 1) + 0;

		if (randNum >= 0 && randNum <= 10) {
			farolLight = 0.0f;
		}
		if (randNum >= 30 && randNum <= 50) {
			farolLight = pointLightColors[0].x + 0.2f;

		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (spotlightOn == 0.0f) {
			spotlightOn = 1.0f;
		}
		else {
			spotlightOn = 0.0f;
		}

	}
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//Exercise 14 Task 2
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}