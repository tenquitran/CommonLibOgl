#pragma once


namespace CommonLibOgl
{
	struct MaterialPhong
	{
	public:
		MaterialPhong()
			: m_shininess{}, m_opacity(1.0f)    // by default, material is opaque
		{
		}

		MaterialPhong(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float opacity = 1.0f)
			: m_ambientColor(ambient), m_diffuseColor(diffuse), m_specularColor(specular), m_shininess(shininess), m_opacity(opacity)
		{
		}

	public:
		glm::vec3 m_ambientColor;
		glm::vec3 m_diffuseColor;
		glm::vec3 m_specularColor;

		// Specular shininess factor (also called shininess or glossiness).
		float m_shininess;

		// Opacity value can be from 1.0 (completely opaque) to 0.0 (completely transparent).
		float m_opacity;
	};
}
