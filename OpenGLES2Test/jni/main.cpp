/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include "log.h"
#include "texture.h"
#include "fileManager.h"
#include "opengl.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>

#include <android/sensor.h>
#include "../native_app_glue/android_native_app_glue.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>
using namespace std;

void shaderErrorLog(GLuint shader)
{
	GLint bufferLenght;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferLenght);
	vector<GLchar> buffer(bufferLenght);
	glGetShaderInfoLog(shader, buffer.size(), nullptr, buffer.data());
	LOG(buffer.data());
}

glm::mat4 modelTransform(int x, int y, int sx, int sy, float rotation)
{
	glm::vec2 position2(x, y);
	glm::vec2 size2(sx, sy);

	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(position2, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size2.x, 0.5f * size2.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size2.x, -0.5f * size2.y, 0.0f));
	model = glm::scale(model, glm::vec3(size2, 1.0f));
	return model;
}

static const GLchar* VERTEXSOURCE
{
	"precision highp float;\n"

	"attribute vec2 attrPosition;\n"
	"attribute vec4 attrColor;\n"
	"attribute vec2 attrTexCoord;\n"
	
	"varying vec4 varyColor;\n"
	"varying vec2 varyTexCoord;\n"
	
	"uniform mat4 unifProjection;\n"
	"uniform mat4 unifModel;\n"
	"uniform float unifLayer;\n"

	"void main()\n"
	"{\n"
	"	varyTexCoord = attrTexCoord;\n"
	"	varyColor = attrColor;\n"
	"   gl_Position = unifProjection * unifModel * vec4(attrPosition, unifLayer, 1.0);\n"
	"}\n"
};

static const GLchar* FRAGMENTSOURCE
{
	"precision mediump float;\n"

	"varying vec4 varyColor;\n"
	"varying vec2 varyTexCoord;\n"

	"uniform sampler2D unifTexture;\n"
	"uniform float unifNoTexture;\n"
	"uniform float unifFontTexture;\n"

	"void main()\n"
	"{\n"
	"	if (unifNoTexture > 0.5)\n"
	"		gl_FragColor = varyColor;\n"
	"	else\n"
	"	{\n"
    "		if (unifFontTexture > 0.5)\n"
	"		{\n"
	"			float alpha = texture2D(unifTexture, varyTexCoord).a * varyColor.a;\n"
	"			gl_FragColor = vec4(varyColor.rgb, alpha); \n"
	"		}\n"
	"		else\n"
	"		{\n"
	"			gl_FragColor = texture2D(unifTexture, varyTexCoord);\n"
	"			gl_FragColor *= varyColor;\n"
	"		}\n"
	"	}\n"
	"}\n"
};

vector<float> VERTICES = std::vector <float>
{
	// Position Vec2
	// Color Vec4
	// TexCoord Vec2
	
	//left up
	0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f,
	
	//left down
	0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f,
	
	//right down
	1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f,
	
	//right up
	1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f
};

vector<GLushort> INDICES = std::vector <GLushort>
{
	0u, 1u, 2u,
	0u, 2u, 3u
};

/**
 * Our saved state data.
 */
struct saved_state 
{
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
struct engine 
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;

	GLuint programId, vertexId, fragmentId;
	GLuint vertexBuffer, indexBuffer;
	GLint positionId, colorId, texCoordId, modelId, projectionId, layerId, noTextureId, fontTextureId;
	glm::mat4 model, projection;
	Texture texture;
	FileManager* fileManager;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) 
{
	engine->fileManager = new FileManager(engine->app);
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
	const EGLint config16bpp[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_NONE
	};

	const EGLint config24bpp[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_NONE
	};

	const EGLint config32bpp[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};

	EGLint contextAttribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

	const EGLint* attribs;
	EGLint windowFormat = ANativeWindow_getFormat(engine->app->window);
	switch (windowFormat)
	{
	case WINDOW_FORMAT_RGBA_8888:
		attribs = config32bpp;
		LOG("Window format: 32 bits per pixel");
		break;
	case WINDOW_FORMAT_RGBX_8888:
		attribs = config24bpp;
		LOG("Window format: 24 bits per pixel");
		break;
	case WINDOW_FORMAT_RGB_565:
		attribs = config16bpp;
		LOG("Window format: 16 bits per pixel");
		break;
	default:
		attribs = config16bpp;
		LOG("Unknown window format!");
	}

    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
	{
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

	//***************************************************
	//opengl
	engine->programId = glCreateProgram();
	GLint result = GL_FALSE;

	//shaders
	/*
	std::string tempStr;
	if (!engine->fileManager->readAsset("andoid_vertex.glsl", tempStr))
		LOG("Failed to read andoid_vertex.glsl");
	const char* temp1 = tempStr.c_str();
	*/
	engine->vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(engine->vertexId, 1u, &VERTEXSOURCE, NULL);
	glCompileShader(engine->vertexId);
	glGetShaderiv(engine->vertexId, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE)
	{
		LOG("Vertex compile error");
		shaderErrorLog(engine->vertexId);
	}

	/*
	if (!engine->fileManager->readAsset("andoid_fragment.glsl", tempStr))
		LOG("Failed to read andoid_fragment.glsl");
	const char* temp2 = tempStr.c_str();
	*/
	engine->fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(engine->fragmentId, 1, &FRAGMENTSOURCE, NULL);
	glCompileShader(engine->fragmentId);
	glGetShaderiv(engine->fragmentId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		LOG("Fragment compile error");
		shaderErrorLog(engine->vertexId);
	}

	//link
	glAttachShader(engine->programId, engine->vertexId);
	glAttachShader(engine->programId, engine->fragmentId);
	glLinkProgram(engine->programId);
	glGetProgramiv(engine->programId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
		LOG("Shader link error");

	engine->positionId = glGetAttribLocation(engine->programId, "attrPosition");
	if (engine->positionId < 0)
		LOG("attrPosition not found");
	gl::checkError();

	engine->colorId = glGetAttribLocation(engine->programId, "attrColor");
	if (engine->colorId < 0)
		LOG("attrColor not found");
	gl::checkError();

	engine->texCoordId = glGetAttribLocation(engine->programId, "attrTexCoord");
	if (engine->texCoordId < 0)
		LOG("attrTexCoord not found");
	gl::checkError();

	engine->modelId = glGetUniformLocation(engine->programId, "unifModel");
	if (engine->modelId < 0)
		LOG("unifModel not found");
	gl::checkError();

	engine->projectionId = glGetUniformLocation(engine->programId, "unifProjection");
	if (engine->modelId < 0)
		LOG("unifProjection not found");
	gl::checkError();

	engine->layerId = glGetUniformLocation(engine->programId, "unifLayer");
	if (engine->modelId < 0)
		LOG("unifLayer not found");
	gl::checkError();

	engine->noTextureId = glGetUniformLocation(engine->programId, "unifNoTexture");
	if (engine->noTextureId < 0)
		LOG("unifNoTexture not found");
	gl::checkError();

	engine->fontTextureId = glGetUniformLocation(engine->programId, "unifFontTexture");
	if (engine->fontTextureId < 0)
		LOG("unifFontTexture not found");
	gl::checkError();

	engine->model = modelTransform(200, 400, 512, 512, 45.0f);
	engine->projection = glm::ortho(0.0f, static_cast<float>(engine->width), static_cast<float>(engine->height), 0.0f, -1.0f, 1.0f);

	glUseProgram(engine->programId);
	glUniformMatrix4fv(engine->modelId, 1, GL_FALSE, glm::value_ptr(engine->model));
	gl::checkError();
	glUniformMatrix4fv(engine->projectionId, 1, GL_FALSE, glm::value_ptr(engine->projection));
	gl::checkError();
	glUniform1f(engine->layerId, 0.5f);
	gl::checkError();
	glUniform1f(engine->noTextureId, 0.0f);
	gl::checkError();
	glUniform1f(engine->fontTextureId, 0.0f);
	gl::checkError();
	glUseProgram(0u);

	// transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// z-layer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDepthRangef(0.0, 1.0);

	// font
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//Vertex buffer
	glGenBuffers(1, &(engine->vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, engine->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, VERTICES.size() * sizeof(GLfloat), VERTICES.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0u);

	//Index buffer
	glGenBuffers(1, &(engine->indexBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES.size() * sizeof(GLushort), INDICES.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
	
	//texture
	glUseProgram(engine->programId);
	if (!engine->texture.load(engine->fileManager, "koala.png"))
		LOG("Failed to load texture!");
	glUseProgram(0u);
	return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) 
{
    if (engine->display == NULL) 
	{
        // No display.
        return;
    }
	glUseProgram(engine->programId);
	gl::checkError();
	glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,((float)engine->state.y)/engine->height, 1);
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, engine->vertexBuffer);
	gl::checkError();
	glEnableVertexAttribArray(engine->positionId);
	gl::checkError();
	glVertexAttribPointer(engine->positionId, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	gl::checkError();

	glEnableVertexAttribArray(engine->colorId);
	gl::checkError();
	glVertexAttribPointer(engine->colorId, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));
	gl::checkError();

	glEnableVertexAttribArray(engine->texCoordId);
	gl::checkError();
	glVertexAttribPointer(engine->texCoordId, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	gl::checkError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->indexBuffer);
	gl::checkError();

	engine->texture.bind();
	gl::checkError();
	glDrawElements(GL_TRIANGLES, INDICES.size(), GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
	gl::checkError();
	engine->texture.unbind();
	gl::checkError();

	glBindBuffer(GL_ARRAY_BUFFER, 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
	glUseProgram(0u);
	eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) 
{
    if (engine->display != EGL_NO_DISPLAY) 
	{
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) 
		{
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) 
		{
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
	{
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) 
	{
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) 
			{
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) 
			{
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) 
{
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) 
	{
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.

    while (1) 
	{
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) 
		{

            // Process this event.
            if (source != NULL) 
			{
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) 
			{
                if (engine.accelerometerSensor != NULL) 
				{
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) 
					{
                        if (LOG_ACCELEROMETER) 
						{
                            LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
                        }
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) 
			{
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) 
		{
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) 
			{
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
