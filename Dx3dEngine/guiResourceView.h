//#pragma once
//#include "guiWidget.h"
//#include "guiListWidget.h"
//#include "Texture.h"
//
//namespace gui
//{
//    class ResourceView :
//        public Widget
//    {
//	public:
//		ResourceView();
//		virtual ~ResourceView();
//
//	public:
//		void Initialize();
//		virtual void FixedUpdate() override;
//		virtual void Update() override;
//		virtual void LateUpdate() override;
//
//		void LoadTextureList();
//		void TextureClickCallback(int Index, const std::string& Item);
//		void OpenTextureFileCallback();
//		void OpenTextureFolderCallback();
//
//		Texture* GetSelectTexture()	const
//		{
//			return mSelectedTexture;
//		}
//
//	private:
//		ListWidget* mTextureList;
//		ListWidget* mFileList;
//
//		std::vector<Texture*>	mTextures;
//		Texture* mSelectedTexture;
//
//		std::string mFolderInput;
//		std::string	mSelectFolderName;
//    };
//}