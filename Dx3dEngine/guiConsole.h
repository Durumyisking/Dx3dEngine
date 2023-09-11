#pragma once
#include "guiWidget.h"
#include "EngineResource.h"

namespace gui
{
	class Console : public Widget
	{
	public:
		Console();
		~Console();

		void Initialize();
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetProject(std::wstring path);
		void SetProject(std::string path);
		void Clear();

		void InitializeTargetProjectPath();
		
		std::string GetTargetProjectPath() { return mTargetProjectPath; }
		void SetTargetProjectPath(std::string path) { mTargetProjectPath = path; }

	private:
		std::map<std::string, Resource*> mResources;
		std::string mTargetProjectPath;
	};
}
