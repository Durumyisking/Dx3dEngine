#pragma once
#include "Component.h"
#include "Graphics.h"

namespace dru
{
    class CLight :
        public CComponent
    {
	public:
		CLight();
		virtual ~CLight();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

		graphics::LightAttribute GetAttribute() { return mAttribute; }

<<<<<<< Updated upstream
		void SetDiffuse(math::Vector4 _diffuse) { mAttribute.diffuse = _diffuse; }
		void SetAmbient(math::Vector4 _ambient) { mAttribute.ambient = _ambient; }
		void SetType(enums::eLightType _type) { mAttribute.type = _type; }
		void SetRadius(float _radius) { mAttribute.radius = _radius; }
		void SetAngle(float _angle) { mAttribute.angle = _angle; }
=======
		void SetDiffuse(math::Vector4 diffuse) { mAttribute.diffuse = diffuse; }
		void SetSpecular(Vector4 spec) { mAttribute.specular = spec; }
		void SetAmbient(math::Vector4 ambient) { mAttribute.ambient = ambient; }
		void SetType(enums::eLightType type) { mAttribute.type = type; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }
>>>>>>> Stashed changes

		math::Vector4 GetDiffuse() { return mAttribute.diffuse ; }
		math::Vector4 GetAmbient() { return mAttribute.ambient; }
		enums::eLightType GetType() { return mAttribute.type; }
		float GetRadius() { return mAttribute.radius; }
		float GetAngle() { return mAttribute.angle; }



	private:
		graphics::LightAttribute mAttribute;
    };
}


