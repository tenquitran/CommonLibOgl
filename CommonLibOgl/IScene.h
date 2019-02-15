#pragma once


namespace CommonLibOgl
{
    // Interface for an OpenGL scene.
    class IScene
    {
    public:
        virtual bool initialize() = 0;

        virtual void resize(GLfloat aspectRatio) = 0;

        virtual void render() const = 0;
    };
}
