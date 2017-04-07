/*#ifndef MATZ_RENDERER_HPP
#define MATZ_RENDERER_HPP

#include <android/native_window.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <exception>
#include <Matz/sprite.hpp>

namespace Matz {

    class Renderer {
    private:
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        EGLConfig config;
        EGLint format;
        float width;
        const float height;
        GLuint program;
        const int vBufLen, iBufLen;
        int vBufPos, iBufPos, uCount;
        GLfloat *const vBuf, *const uModelBuf, *const uColorBuf;
        GLushort *const iBuf;
        GLint unifProjView, unifModel, unifColor, unifEnvColor, unifLight;
        bool cleared;

        void pushProjViewMatrix();

        void makeCurrent();

        void unmakeCurrent();

        void draw();

        void initialize();

        void deleteBufs();

        bool overflows(int verts, int inds);

    public:

        Renderer(int maxVerts, int maxInds, float height);

        virtual ~Renderer();

        void destroy();

        void setWindow(ANativeWindow *window);

        void push(const Sprite &sprite);

        void flush();

        float getWidth();

        float getHeight();

    };

    class RenderClient {
    private:
        Renderer &renderer;
    public:

        RenderClient(Renderer &renderer);

        void render(const Sprite &sprite);
    };

}

#endif*/
