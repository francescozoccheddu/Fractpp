
/*
#define MATZ_LOG_TAG "Renderer"

#define MATZ_ATTR_POS 0
#define MATZ_ATTR_NORM 1
#define MATZ_ATTR_IND 2
#define MATZ_UNIFV_LEN 32
#define MATZ_Z_OFFSET 0.05f

namespace Matz {

    const EGLint configAttribs[]{
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_NONE
    };

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

#include "Matz/shaders.glsl2c"

    Renderer::Renderer(int maxVerts, int maxInds, float height) :
            display(EGL_NO_DISPLAY), surface(EGL_NO_SURFACE), context(EGL_NO_CONTEXT), program(0),
            vBufLen(maxVerts * 7), vBuf(new GLfloat[vBufLen]), vBufPos(0),
            iBufLen(maxInds), iBuf(new GLushort[iBufLen]), iBufPos(0),
            uModelBuf(new GLfloat[16 * MATZ_UNIFV_LEN]), uColorBuf(new GLfloat[3 * MATZ_UNIFV_LEN]),
            uCount(0), cleared(false), height(height) {
        try {
            display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            if (display == EGL_NO_DISPLAY)
                throw std::runtime_error("No display returned");
            if (!eglInitialize(display, 0, 0))
                throw std::runtime_error("Unable to initialize");
            unmakeCurrent();
            EGLint numConfig;
            if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfig))
                throw std::runtime_error("Unable to choose config");
            if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))
                throw std::runtime_error("Unable to query config attribute");
            context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
            if (context == EGL_NO_CONTEXT)
                throw std::runtime_error("Unable to create context");
        } catch (std::exception &exception) {
            destroy();
            deleteBufs();
            throw;
        }
    }

    static GLuint createShader(GLenum type, const GLchar *source);

    void Renderer::initialize() {
        makeCurrent();
        //Depth testing
        glDepthRangef(0.0f, 1.0f);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        //Face culling
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        //Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //Other
        glDisable(GL_TEXTURE_2D);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        //Program
        GLuint vs = createShader(GL_VERTEX_SHADER, vsBasic);
        GLuint fs = createShader(GL_FRAGMENT_SHADER, fsBasic);
        program = glCreateProgram();
        if (!program)
            throw std::runtime_error("Program not created");
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glBindAttribLocation(program, MATZ_ATTR_POS, "a_position");
        glBindAttribLocation(program, MATZ_ATTR_NORM, "a_normal");
        glBindAttribLocation(program, MATZ_ATTR_IND, "a_index");
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = new char[infoLen];
                glGetProgramInfoLog(program, infoLen, NULL, infoLog);
                LOGD("Program linking error:\n%s", infoLog);
                delete[] infoLog;
            }
            throw std::runtime_error("Program not linked");
        }
        glDetachShader(program, vs);
        glDetachShader(program, fs);
        glDeleteShader(fs);
        glDeleteShader(vs);
        glUseProgram(program);
        //Program data
        glEnableVertexAttribArray(MATZ_ATTR_POS);
        glEnableVertexAttribArray(MATZ_ATTR_NORM);
        glEnableVertexAttribArray(MATZ_ATTR_IND);
        unifProjView = glGetUniformLocation(program, "u_projview");
        unifModel = glGetUniformLocation(program, "u_model");
        unifColor = glGetUniformLocation(program, "u_color");
        unifLight = glGetUniformLocation(program, "u_light");
    }

    void Renderer::deleteBufs() {
        delete[] vBuf;
        delete[] iBuf;
        delete[] uModelBuf;
        delete[] uColorBuf;
    }

    Renderer::~Renderer() {
        deleteBufs();
    }

    void Renderer::pushProjViewMatrix() {
        static const float n = 2.0f, f = 10.0f;
        const float r = width / 2.0f, t = height / 2.0f;
        static const Vec p(0, 0, 0), l(0, 0, 0);
        GLfloat m[16];
        m[0] = n / r;
        m[4] = 0;
        m[8] = 0;
        m[12] = n / r * -r;
        m[1] = 0;
        m[5] = n / t;
        m[9] = 0;
        m[13] = n / t * -t;
        m[2] = 0;
        m[6] = 0;
        m[10] = -(f + n) / (f - n);
        m[14] = (-(f + n) / (f - n)) * (-n - MATZ_Z_OFFSET) + -2 * f * n / (f - n);
        m[3] = 0;
        m[7] = 0;
        m[11] = -1;
        m[15] = (-1) * (-n - MATZ_Z_OFFSET);
        glUniform3f(unifLight, 0.5, height / 2.0f, 0);
        glUniformMatrix4fv(unifProjView, 1, GL_FALSE, m);
    }

    void Renderer::makeCurrent() {
        if (!eglMakeCurrent(display, surface, surface, context))
            throw std::runtime_error("Unable to make current");
    }

    void Renderer::unmakeCurrent() {
        if (!eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT))
            throw std::runtime_error("Unable to make void current");
    }

    void Renderer::destroy() {
        if (program)
            glDeleteProgram(program);
        unmakeCurrent();
        if (context != EGL_NO_CONTEXT)
            eglDestroyContext(display, context);
        if (surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface);
        if (display != EGL_NO_DISPLAY)
            eglTerminate(display);
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        display = EGL_NO_DISPLAY;
        program = 0;
    }

    void Renderer::setWindow(ANativeWindow *window) {
        if (display == EGL_NO_DISPLAY)
            throw std::runtime_error("No display");
        unmakeCurrent();
        if (surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface);
        if (window) {
            ANativeWindow_setBuffersGeometry(window, 0, 0, format);
            surface = eglCreateWindowSurface(display, config, window, 0);
            if (surface == EGL_NO_SURFACE)
                throw std::runtime_error("Unable to create window surface");
            if (!program)
                initialize();
            makeCurrent();
            EGLint width, height;
            if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) or
                !eglQuerySurface(display, surface, EGL_HEIGHT, &height))
                throw std::runtime_error("Unable to query surface size");
            this->width = width / (float) height * this->height;
            LOGD("Resized w: %d h: %d", width, height);
            glViewport(0, 0, width, height);
            pushProjViewMatrix();
        } else {
            surface = EGL_NO_SURFACE;
            width = 0;
        }
    }

    void Renderer::flush() {
        if (uCount > 0)
            draw();
        eglSwapBuffers(display, surface);
        cleared = false;
    }

    bool Renderer::overflows(int verts, int inds) {
        return uCount + 1 >= MATZ_UNIFV_LEN or iBufPos + inds > iBufLen or
               vBufPos + verts * 7 > iBufLen;
    }

    void Renderer::push(const Sprite &sprite) {
        const Mesh &mesh = sprite.mesh;
        if (overflows(mesh.vertCount, mesh.indCount)) {
            draw();
            if (overflows(mesh.vertCount, mesh.indCount))
                throw std::runtime_error("Mesh too large for buffer");
        }
        GLushort iOfs = (GLushort) (vBufPos / 7);
        int mv = 0;
        for (int v = 0; v < mesh.vertCount; v++) {
            for (int p = 0; p < 6; p++)
                vBuf[vBufPos++] = mesh.verts[mv++];
            vBuf[vBufPos++] = uCount;
        }
        for (int i = 0; i < mesh.indCount; i++)
            iBuf[iBufPos++] = mesh.inds[i] + iOfs;
        sprite.transform.toMatrix(uModelBuf + uCount * 16);
        int c = uCount * 3;
        uColorBuf[c + 0] = sprite.color.h;
        uColorBuf[c + 1] = sprite.color.s;
        uColorBuf[c + 2] = sprite.color.v;
        uCount++;
    }

    void Renderer::draw() {
        if (!cleared) {
            glClearColor(0, 0, 0, 0.3);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cleared = true;
        }
        glVertexAttribPointer(MATZ_ATTR_POS, 3, GL_FLOAT, GL_FALSE,
                              sizeof(GLfloat) * 7, vBuf);
        glVertexAttribPointer(MATZ_ATTR_NORM, 3, GL_FLOAT, GL_FALSE,
                              sizeof(GLfloat) * 7, vBuf + 3);
        glVertexAttribPointer(MATZ_ATTR_IND, 1, GL_FLOAT, GL_FALSE,
                              sizeof(GLfloat) * 7, vBuf + 6);
        glUniformMatrix4fv(unifModel, uCount, GL_FALSE, uModelBuf);
        glUniform3fv(unifColor, uCount, uColorBuf);
        glDrawElements(GL_TRIANGLES, iBufPos, GL_UNSIGNED_SHORT, iBuf);
        iBufPos = 0;
        vBufPos = 0;
        uCount = 0;
    }

    float Renderer::getHeight() {
        return height;
    }

    float Renderer::getWidth() {
        return width;
    }

    GLuint createShader(GLenum type, const GLchar *source) {
        GLuint shader = glCreateShader(type);
        if (shader == 0)
            throw std::runtime_error("No shader created");
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = new char[infoLen];
                glGetShaderInfoLog(shader, infoLen, 0, infoLog);
                LOGD("Shader compilation error:\n%s", infoLog);
                delete[] infoLog;
            }
            glDeleteShader(shader);
            throw std::runtime_error("Shader not compiled");
        }
        return shader;
    }

    RenderClient::RenderClient(Renderer &_renderer) : renderer(_renderer) {}

    void RenderClient::render(const Sprite &sprite) {
        renderer.push(sprite);
    }

}
 */