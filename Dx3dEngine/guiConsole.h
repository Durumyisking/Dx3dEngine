#pragma once
#include "guiWidget.h"
#include "EngineResource.h"
#include "guiGroupWidget.h"

namespace gui
{
	enum class eFileType
	{
		None,
		Texture,
		Material,
		Mesh,
		Component,
		GameObject,
		Scene,
		End,
	};

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

		void InitializeTargetProject();
		void FileClickCallback(const std::string& name, eFileType type);
		
		std::string GetTargetProjectPath() { return mTargetProjectPath; }
		void SetTargetProjectPath(const std::string& path) { mTargetProjectPath = path; }
		void SetTargetFiles(const std::vector<std::string>& files) { mTargetFiles = files; }

	private:
		GroupWidget* mGroup;

		std::map<std::string, Resource*> mResources;
		std::string mTargetProjectPath;
		std::vector<std::string> mTargetFiles;
	};
}
