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

        // Camera control.
        virtual void translateCamera(const glm::vec3& diff) = 0;
        virtual void rotateCamera(const glm::vec3& degrees) = 0;
        virtual void scaleCamera(GLfloat amount) = 0;
    };
}
