#pragma once
#include "Rendering/ShaderLoader.h"
#include "Rendering/TextureLoader.h"
#include <utility>
#include <vector>
#include <string>


namespace Frost
{
    class AssetLoader
    {
        public:

        static Shader* LoadShader(const std::string& name, const std::string& vPath, const std::string& fPath)
        {

            GetAssets<Shader>().push_back({name, ShaderLoader::Create(vPath, fPath)});
            for(auto& [n, asset] : GetAssets<Shader>())
            {
                if(!name.compare(n))
                {
                    return asset.get();
                }
            }

        }

        static Texture* LoadTexture(const std::string& name, const std::string& texturePath)
        {

            GetAssets<Texture>().push_back({name, TextureLoader::Create(texturePath)});
            for(auto& [n, asset] : GetAssets<Texture>())
            {
                if(!name.compare(n))
                {
                    return asset.get();
                }
            }

        }

        private:
        template<typename T>
        using Asset = std::pair<std::string, std::shared_ptr<T>>;
        
        template<typename T>
        static std::vector<Asset<T>>& GetAssets()
        {
            static std::vector<Asset<T>> m_Assets;

            static bool m_IsRegistered = false;


            return m_Assets;
        }
 
        AssetLoader() = delete;
    };
}